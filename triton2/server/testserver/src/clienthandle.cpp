#include <sharemem.h>
#include "shm.h"
#include "code_queue.h"
#include "lk_assert.h"
#include "network.h"
#include "macro.h"
#include "object.h"
#include "client_comm_engine.h"
#include "message_pb.hxx.pb.h"
#include "message_dispatcher.h"
#include "coremessage_pb.hxx.pb.h"
#include "message_factory.h"
#include "globallogic.h"
#include "clienthandle.h"

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
	//mClientCommHash.initailize();

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

int CClientHandle::Send(CMessage* pMsg, CCertifyPlayer* pPlayer)
{
	LK_ASSERT( pMsg != NULL && pPlayer != NULL, return -1 );

	//TODO: 这里把发送buffer定为最大buffer+128, 因为消息序列化后再加密会变大18Bytes左右; 总包长依然小于tcpsvrd端的最大包长32752
	BYTE tCodeBuf[MAX_PACKAGE_LEN+128] = {0};
	int tCodeLength = sizeof(tCodeBuf);

	int tRet = EncodeNetMsg( pMsg, tCodeBuf, tCodeLength, pPlayer );
	
	if ( tRet < 0 )
	{
		LOG_ERROR( "default", "In CClientHandle::Send(),  ConvertMsgToClientCode return %d", tRet );
		return -1;
	}

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
		LOG_DEBUG("default","DecodeNetMsg errno:%d",tTempCodeLength);
		return CLIENTHANDLE_DECODE_FAILED;
	}
	in_addr tAddr;
	tAddr.s_addr = tNetHead.mSrcIP;
	int nSockIndex = tNetHead.mDstPort;
	LOG_DEBUG("default","Recv codes len:%d,ip = %s, port = %d, handle = %d",tTempCodeLength,inet_ntoa(tAddr),tNetHead.mSrcPort,nSockIndex);

	Message* pUnknownMessagePara = (Message*) tMessage.msgpara();
	if ( pUnknownMessagePara != NULL ) 
	{
		CCertifyPlayer* tpPlayer = CGlobalCertifyPlayerModule::GetSingleton().FindPlayerByComm(nSockIndex);
		CGlobalLogic::GetSingletonPtr()->OnClientMessage(nSockIndex ,&tMessage);
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

	if( tNetHead.mState < 0 )
	{
		LOG_ERROR("default",  "Client [socket:%d] close connect from global server", tSockIndex );
		return CLIENTHANDLE_SUCCESS;
	}
   LOG_DEBUG("default", "receive data. (buffer=0x%08X length=%d)", pBuff, tLen);
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
		CCertifyPlayer* tpPlayer = CGlobalCertifyPlayerModule::GetSingleton().GetPlayerByComm(tSockIndex);
		tpPlayer->socketTime = tNetHead.mDstIP;
		tpPlayer->_clientIp = tNetHead.mSrcIP;
		tpPlayer->_clientPort = tNetHead.mSrcPort;
		tpPlayer->_handle = tSockIndex;
		LOG_DEBUG("default","tPlayer:<socketTime:%d ip:%d port:%d handle: %d;>", tNetHead.mDstIP,tNetHead.mSrcIP,tNetHead.mSrcPort,tSockIndex);
		/*
		CGlobalPlayer* tpPlayer = (CGlobalPlayer*) CGlobalObjManager::GetSingletonPtr()->CreateObject(OBJTYPE_PLAYER_AT_LOGIN);	
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

int CClientHandle::EncodeNetMsg( CMessage* pMsg, BYTE* pCodeBuff, int& vLen,CCertifyPlayer* pPlayer )
{
	LK_ASSERT( pMsg != NULL && pCodeBuff != NULL && vLen > 0 && pPlayer != NULL, return -1 );

	CNetHead tNetHead;

	// 标示是否让tcpserver发送完该消息后，自动与对应客户端断开连接
	tNetHead.mSrcPort = 0;
   tNetHead.mDstPort = pPlayer->_handle;
	tNetHead.mDstIP = pPlayer->socketTime;
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
