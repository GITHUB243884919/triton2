#include "sharemem.h"
#include "shm.h"
#include "code_queue.h"
#include "clienthandle.h"
#include "lk_assert.h"
#include "network.h"
#include "macro.h"
#include "object.h"
#include "message_pb.hxx.pb.h"
#include "loginlogic.h"
#include "client_comm_engine.h"
#include "message_dispatcher.h"
#include "coremessage_pb.hxx.pb.h"
#include "loginobj_manager.h"

#include "message_factory.h"
#include <google/protobuf/descriptor.h>

CSharedMem* CClientHandle::mShmPtr = NULL;


extern  int ConvertCiientCodeToMsg3 ( void* pBuff, unsigned short usBuffLen, CCSHead* pCshead, CMessage* pMsg, void* pMsgPara, unsigned short& rusParaLen );
extern int ConvertMsgToClientCode2( const CCSHead* pCSHead, const CMessage* pMsg, void* pBuff, unsigned short& rusBuffLen );



CClientHandle::CClientHandle()
{
	int size = sizeof(CSharedMem) + CCodeQueue::CountQueueSize(MAX_PIPE_SIZE);

	char szCmd[ FILE_MAX_PATH ] = {0};

	snprintf(szCmd, sizeof(szCmd)-1,"touch %s", "./scpipefile");
	system(szCmd);

	snprintf(szCmd, sizeof(szCmd)-1,"touch %s", "./cspipefile");
	system(szCmd);

	unsigned int tkeys2c = MakeKey( "./scpipefile", 'O' );
	unsigned int tkeyc2s = MakeKey( "./cspipefile", 'I' );

	BYTE* tpS2CPipe = CreateShareMem ( tkeys2c, size);
	BYTE* tpC2SPipe = CreateShareMem ( tkeyc2s, size);

	LK_ASSERT( ( tpS2CPipe != NULL ) && ( tpC2SPipe != NULL ), exit(1) );

	CSharedMem::pbCurrentShm = tpC2SPipe;
	CCodeQueue::pCurrentShm = new CSharedMem( tkeyc2s, size );
	mC2SPipe = new CCodeQueue( MAX_PIPE_SIZE, IDX_PIPELOCK_C2S );

	CSharedMem::pbCurrentShm = tpS2CPipe;
	CCodeQueue::pCurrentShm = new CSharedMem( tkeys2c, size );
	mS2CPipe = new CCodeQueue( MAX_PIPE_SIZE, IDX_PIPELOCK_S2C );

	LK_ASSERT( ( mC2SPipe != NULL ) && ( mS2CPipe != NULL ), exit(1) );


	if ( mShmPtr->GetInitMode( ) == SHM_INIT )
	{
		LOG_DEBUG( "default" , " ClientHandle Initailize" );
		Initialize( );
	} 
	else
	{
		LOG_DEBUG( "default" , " ClientHandle Resume" );
		Resume( ); 
	}
}


CClientHandle::~CClientHandle()
{

}

int CClientHandle::Initialize( )
{
	mClientCommHash.initailize();

	return 0;
}


void* CClientHandle::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CClientHandle::operator delete( void* pointer ) 
{ 
}

int CClientHandle::Resume( )
{
	return 0;
}

unsigned int CClientHandle::CountSize()
{
	return sizeof( CClientHandle );
}

int CClientHandle::Send(CMessage* pMsg, CLoginPlayer* pPlayer)
{
	LK_ASSERT( pMsg != NULL && pPlayer != NULL, return -1 );
	return Send(pMsg, pPlayer->mCommHandle, pPlayer->mSockTime, pPlayer->mClientAddr, pPlayer->mClientPort);
	
}
int CClientHandle::Send(CMessage* pMsg, uint32 vCommHandle, uint32 vSockTime, int  vClientAddr, unsigned int vClientPort)
{
	LK_ASSERT( pMsg != NULL , return -1 );

	//TODO: 这里把发送buffer定为最大buffer+128, 因为消息序列化后再加密会变大18Bytes左右; 总包长依然小于tcpsvrd端的最大包长32752
	BYTE tCodeBuf[MAX_PACKAGE_LEN+128] = {0};
	int tCodeLength = sizeof(tCodeBuf);
	LOG_DEBUG("default","EncodeNetMsg");

	int tRet = EncodeNetMsg( pMsg, tCodeBuf, tCodeLength,  vCommHandle, vSockTime);
	
	LOG_DEBUG("default","EncodeNetMsg ok.");
	if ( tRet < 0 )
	{
		LOG_ERROR( "default", "In CClientHandle::Send(),  ConvertMsgToClientCode return %d", tRet );
		return -1;
	}


#ifdef _DEBUG_
	Message* pUnknownMessagePara = (Message*) pMsg->msgpara();
	LK_ASSERT( pUnknownMessagePara != NULL, return -1 );
	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();

	unsigned char* p = (unsigned char*) &vClientAddr;
	LOG_DEBUG( "default", "Send2Client(%d.%d.%d.%d:%d, fd=%d) MsgName[%s] MsgHead[%s] MsgPara[%s]", 
			*(p), *(p+1), *(p+2), *(p+3), ntohs(vClientPort), 
			vCommHandle, pDescriptor->name().c_str(), 
			pMsg->ShortDebugString().c_str(), pUnknownMessagePara->ShortDebugString().c_str() );

#endif


	tRet = mS2CPipe->AppendOneCode ( tCodeBuf, tCodeLength );
	if ( tRet < 0 )
	{
		LOG_ERROR( "default", "In CClientHandle::Send(),  AppendOneCode return %d", tRet );
		return -1;
	}


	return 0;
}

int CClientHandle::Recv()
{
	BYTE tTempCodeBuf[MAX_PACKAGE_LEN+128] = {0};
	int tTempCodeLength = 0;

	int tTempRet = mC2SPipe->GetHeadCode((BYTE *)tTempCodeBuf, &tTempCodeLength);

	if ( tTempRet < 0 )
	{
		LOG_ERROR( "default", "When GetHeadCode from C2SPipe, error ocurr %d", tTempRet );
		return CLIENTHANDLE_QUEUE_CRASH;
	}

	if ( tTempCodeLength == 0 )
	{
		return CLIENTHANDLE_QUEUE_EMPTY;
	}

	if( tTempCodeLength < sizeof(CNetHead) )
	{
		return CLIENTHANDLE_CODE_NOTINTEGRITY;
	}

	CNetHead tNetHead;
	CCSHead tCSHead;
	CMessage tMessage;
	BYTE tMsgPara[MAX_PACKAGE_LEN] = {0};
	int tusParLen = sizeof(tMsgPara); 

	// TODO: 返回0 是客户端断连的消息
	if (DecodeNetMsg( tTempCodeBuf, tTempCodeLength, &tCSHead, &tMessage, tMsgPara, tusParLen, tNetHead) < 0)
	{
		return CLIENTHANDLE_DECODE_FAILED;
	}
	
	
	Message* pUnknownMessagePara = (Message*) tMessage.msgpara();
	if ( pUnknownMessagePara != NULL ) 
	{
#ifdef _DEBUG_
		const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();
		unsigned char* p = (unsigned char*) &tNetHead.mSrcIP;
		LOG_DEBUG( "default", "Recv Client(%d.%d.%d.%d:%d, fd=%d) MsgName[%s] MsgHead[%s] MsgPara[%s] ",
				*(p), *(p+1), *(p+2), *(p+3), ntohs(tNetHead.mSrcPort), tNetHead.mDstPort, pDescriptor->name().c_str(), 
				tMessage.msghead().ShortDebugString().c_str(), pUnknownMessagePara->ShortDebugString().c_str());
#endif
		if (tMessage.msghead().messageid() == ID_C2S_REQUEST_PING)
		{
			CMessageClientPingServerRequest* pReqMsg = (CMessageClientPingServerRequest*) tMessage.msgpara();

			CMessageClientPingServerResponse tResMsg;
			tResMsg.set_clientlocaltime( pReqMsg->clientlocaltime() );
			tResMsg.set_servertime( LINEKONG::GetTickCount() );

			CMessage tMsg;

			// 消息日志屏蔽, 临时用 msgseq 字段来标示是否记录消息
			//tMsg.mutable_msghead()->set_msgseq( 1 );

			tMsg.mutable_msghead()->set_srcfe( FE_LOGINSERVER );
			tMsg.mutable_msghead()->set_dstfe( FE_CLIENT );
			tMsg.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_PING );
			tMsg.set_msgpara( (uint32)&tResMsg );

			Send(&tMsg, tNetHead.mDstPort, tNetHead.mDstIP, tNetHead.mSrcIP, tNetHead.mSrcPort);
			return CLIENTHANDLE_SUCCESS;

		}

		if (tMessage.msghead().messageid() == ID_C2L_REQUEST_LOGINSERVER)
		{
			/*
			CMessageLoginServerRequest* tpLoginRequest = (CMessageLoginServerRequest*) tMessage.msgpara();
			LK_ASSERT(tpLoginRequest != NULL, return);
			*/

			int nSockIndex = tNetHead.mDstPort;

			// 屏蔽客户端对此消息不停重发造成的恶劣影响
			CLoginPlayer * tpPlayer = CLoginLogic::GetSingleton().GetPlayerByComm(nSockIndex);
			if ( tpPlayer != NULL 
			&& tpPlayer->mClientAddr == tNetHead.mSrcIP
			&& tpPlayer->mClientPort == tNetHead.mSrcPort
			&& tpPlayer->mSockTime == tNetHead.mDstIP )
			{
				return CLIENTHANDLE_SUCCESS;
			}

			tpPlayer = (CLoginPlayer*) CLoginObjManager::GetSingletonPtr()->CreateObject(OBJTYPE_PLAYER_AT_LOGIN);	
			LK_ASSERT(tpPlayer != NULL, return -1);

			tpPlayer->mClientAddr = tNetHead.mSrcIP;
			tpPlayer->mClientPort = tNetHead.mSrcPort;
			tpPlayer->mSockTime = tNetHead.mDstIP;
			tpPlayer->mCommHandle = nSockIndex;
			tpPlayer->mActiveTime = time(NULL); // 最后活跃时间

			tpPlayer->mWhere = CLoginPlayer::STATE_INWAIT;
			CLoginLogic::GetSingleton().RegistPlayerComm(nSockIndex, tpPlayer);
		}

		CLoginLogic::GetSingletonPtr()->OnClientMessage(&tMessage);
	}

	return CLIENTHANDLE_SUCCESS;
}


int CClientHandle::DecodeNetMsg( BYTE* pCodeBuff, int & vLen, CCSHead* pCSHead, 
			CMessage* pMsg, BYTE* pMsgPara, int & usParaLen, CNetHead& tNetHead)
{
	LK_ASSERT( pCodeBuff != NULL && vLen >= sizeof(CNetHead) && pMsg != NULL, return -1 );

	BYTE* pBuff = pCodeBuff;
	int tLen = vLen;

	//TODO: 应该首先解密 （必要的话还可以压缩）
	//tNetHead.ParseFromArray( pBuff, sizeof(CNetHead) );
	
	memcpy( &tNetHead, pBuff, sizeof(CNetHead));

	pBuff += sizeof(tNetHead);
	tLen -= sizeof(tNetHead);

	// 在tcpserver 上的socket index 
	short tSockIndex = tNetHead.mDstPort;

	// 如果是断连消息, 调用玩家离开方法，销毁对象
	if( tNetHead.mState < 0 )
	{
		LOG_ERROR("default",  "Client [socket:%d] close connect from loginserver", tSockIndex );

		CLoginPlayer * tpPlayer = CLoginLogic::GetSingleton().GetPlayerByComm(tSockIndex);
		if (tpPlayer != NULL )
		{
			LK_ASSERT(tSockIndex == tpPlayer->mCommHandle && tpPlayer->mSockTime == tNetHead.mDstIP, return -1);
			CLoginLogic::GetSingleton().PlayerLeaveFromLogin(tpPlayer);
		}

		return CLIENTHANDLE_SUCCESS;
	}
			   
	int nRet = ClientCommEngine::ConvertClientCodeToMsg2( (void*)pBuff, (unsigned short)tLen, pCSHead, pMsg);
	if( nRet < 0 )
	{
		LOG_ERROR( "default", "convert code failed %d", nRet );
		return -1;
	}

	CMessageHead* tpHead = pMsg->mutable_msghead();

	// 如果是首次连接, 创建对象
	int nDialogID = (int)pCSHead->dialogid( ); 
	if( nDialogID <= 0 )
	{
		/*
		CLoginPlayer* tpPlayer = (CLoginPlayer*) CLoginObjManager::GetSingletonPtr()->CreateObject(OBJTYPE_PLAYER_AT_LOGIN);	
		LK_ASSERT(tpPlayer != NULL, return -1);

		tpPlayer->mClientAddr = tNetHead.mSrcIP;
		tpPlayer->mClientPort = tNetHead.mSrcPort;
		tpPlayer->mSockTime = tNetHead.mDstIP[0];
		tpPlayer->mCommHandle = tNetHead.mDstPort[0];
		tpPlayer->mActiveTime = time(NULL); // 最后活跃时间
		*/
	}

	tpHead->set_srcid (tSockIndex );

	return CLIENTHANDLE_SUCCESS;
}

int CClientHandle::EncodeNetMsg( CMessage* pMsg, BYTE* pCodeBuff, int& vLen, CLoginPlayer* pPlayer )
{
	LK_ASSERT( pMsg != NULL && pCodeBuff != NULL && vLen > 0 && pPlayer != NULL, return -1 );

	return EncodeNetMsg(pMsg, pCodeBuff, vLen, pPlayer->mCommHandle, pPlayer->mSockTime);
}
int CClientHandle::EncodeNetMsg( CMessage* pMsg, BYTE* pCodeBuff, int& vLen, uint32 vCommnHandle, uint32 vSockTime )
{
	LK_ASSERT( pMsg != NULL && pCodeBuff != NULL && vLen > 0 , return -1 );

	CNetHead tNetHead;

	// 标示是否让tcpserver发送完该消息后，自动与对应客户端断开连接
	tNetHead.mSrcPort = 0;
	if( pMsg->msghead().messageid() == ID_L2C_NOTIFY_LOGINERROR )
	{
		CMessageLoginErrorNotice * tpErrorNotice = (CMessageLoginErrorNotice *) pMsg->msgpara();
		if ( tpErrorNotice &&
			tpErrorNotice->errorcode() != ELOGIN_CLIENT_NEED_MATRIXCODE  && tpErrorNotice->errorcode() != ELOGIN_CLIENT_NEED_STATICMATRIXCODE )
		{
			tNetHead.mSrcPort = 1; 
		}

	}

	if (pMsg->msghead().messageid() == ID_L2C_NOTIFY_REDIRECTGATE ) 
	{
		tNetHead.mSrcPort = 1; 
	}

	//tNetHead.mDstIP = pPlayer->mSockTime;
	//tNetHead.mDstPort = pPlayer->mCommHandle;
	tNetHead.mDstIP = vSockTime;
	tNetHead.mDstPort = vCommnHandle;


	int tLen = 0;

	// NetHead 序列化
	//tNetHead.SerializeToArray( pCodeBuff, sizeof(CNetHead) );

	memcpy( pCodeBuff, &tNetHead, sizeof(CNetHead));

	int tNetHeadSize = sizeof(tNetHead);
	pCodeBuff += tNetHeadSize;
	tLen += tNetHeadSize;

	CCSHead tCSHead;
	int tusClientCodeLen = vLen - tLen;

	LK_ASSERT(tusClientCodeLen > 0 && tusClientCodeLen  < 0xFFFF, return -1);
	unsigned short usTempShort = ( unsigned short ) tusClientCodeLen ;
	// 序列化&加密
	if( ClientCommEngine::ConvertMsgToClientCode2( &tCSHead, pMsg, pCodeBuff, usTempShort , ClientCommEngine::tpKey ) < 0 )
	{
		LOG_ERROR( "default", "[%s:%d] convert failed , len %d", __FUNCTION__, __LINE__, usTempShort );
		return -1;
	}
	tusClientCodeLen = usTempShort;

	tLen += tusClientCodeLen;

	// 返回值
	vLen = tLen;

	return 0;
}
