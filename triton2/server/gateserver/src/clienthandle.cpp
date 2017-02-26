#include "sharemem.h"
#include "shm.h"
#include "code_queue.h"
#include "clienthandle.h"
#include "lk_assert.h"
#include "network.h"
#include "macro.h"
#include "object.h"
#include "message_pb.hxx.pb.h"
#include "gatelogic.h"
#include "client_comm_engine.h"
#include "message_dispatcher.h"
#include "coremessage_pb.hxx.pb.h"
#include "gateobj_manager.h"

#include "message_factory.h"
#include <google/protobuf/descriptor.h>
#include "core_service.h"

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

int CClientHandle::Send(CMessage* pMsg, std::vector<CGatePlayer*>* pPlayerSet )
{
	LK_ASSERT( pMsg != NULL && pPlayerSet != NULL , return -1 );

	BYTE tCodeBuf[MAX_PACKAGE_LEN];
	short tCodeLength = sizeof(tCodeBuf);

	CNetHead tNetHead;
	tNetHead.mSrcPort = 0; 

	short tLen = 0;
	char* pCodeBuff = (char*)&tCodeBuf[0];

	// 空出单个NetHead长度
	pCodeBuff += sizeof(CNetHead);
	tLen += sizeof(CNetHead);

	// 空出总长度
	pCodeBuff += sizeof(unsigned short);
	tLen += sizeof(unsigned short);

	// CSHead部分序列化
	CCSHead tCSHead;
	//tCSHead.set_totallength(0);

	*(unsigned short*)pCodeBuff = (unsigned short) tCSHead.ByteSize();
	pCodeBuff += sizeof(unsigned short);
	tLen += sizeof(unsigned short);	

	tCSHead.SerializeToArray( pCodeBuff, tCodeLength-tLen );
	pCodeBuff += tCSHead.GetCachedSize();
	tLen += tCSHead.GetCachedSize();

	// message 部分序列化
	BYTE tMessageBuff[MAX_PACKAGE_LEN] = {0};
	unsigned short tMessageBuffLen = sizeof(tMessageBuff);
	if( ClientCommEngine::ConvertMsgToClientCode3( pMsg, tMessageBuff, tMessageBuffLen ) < 0 )
	{
		assert(0);
		return -1;
	}

	int tEncryptLen  = tCodeLength - tLen;

	for( std::vector<CGatePlayer*>::iterator it = pPlayerSet->begin(); it != pPlayerSet->end(); ++it )
	{
		CGatePlayer* pPlayer = *it;
		if( pPlayer == NULL ) continue;
		

		ClientCommEngine::EncryptData( EnCryptType1, pPlayer->GetCSKey( ), 
				tMessageBuff, tMessageBuffLen, (unsigned char*)pCodeBuff, &tEncryptLen );

		// NetHead
		tNetHead.mDstIP = pPlayer->mSockTime;
		tNetHead.mDstPort = pPlayer->mCommHandle;

		memcpy( &tCodeBuf[0], &tNetHead, sizeof(CNetHead) );

		// 总长度
		*((unsigned short*)&tCodeBuf[sizeof(CNetHead)]) = (unsigned short) (tEncryptLen+tLen-sizeof(CNetHead));

		int tRet = mS2CPipe->AppendOneCode ( tCodeBuf, tEncryptLen+tLen );
		if ( tRet < 0 )
		{
			LOG_ERROR( "default", "In CClientHandle::Send(),  AppendOneCode return %d", tRet );
			continue;
		}

		#ifdef _DEBUG_
		/*
		Message* pUnknownMessagePara = (Message*) pMsg->msgpara();
		LK_ASSERT( pUnknownMessagePara != NULL, return -1 );
		const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();

		unsigned char* p = (unsigned char*) &(pPlayer->mClientAddr);
		LOG_DEBUG( "default", "Send2Client(%d.%d.%d.%d:%d, fd=%d len=%d) MsgName[%s] MsgHead[%s] MsgPara[%s]", 
				*(p), *(p+1), *(p+2), *(p+3), pPlayer->mClientPort, pPlayer->mCommHandle, tCodeLength,
				pDescriptor->name().c_str(), pMsg->ShortDebugString().c_str(), 
				pUnknownMessagePara->ShortDebugString().c_str() );
		*/
		#endif
	}
	
	return 0;
}

int CClientHandle::Send(CMessage* pMsg, CGatePlayer* pPlayer)
{

	//TODO: 这里把发送buffer定为最大buffer+128, 因为消息序列化后再加密会变大18Bytes左右; 总包长依然小于tcpsvrd端的最大包长32752
	BYTE tCodeBuf[MAX_PACKAGE_LEN+128]  = {0};
	int tCodeLength = sizeof(tCodeBuf);

	int tRet = EncodeNetMsg( pMsg, tCodeBuf, tCodeLength, pPlayer );

	if ( tRet < 0 )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] In CClientHandle::Send(),  ConvertMsgToClientCode return %d", 
				__LK_FILE__, __LINE__, __FUNCTION__, tRet );
		return -1;
	}

	tRet = mS2CPipe->AppendOneCode ( tCodeBuf, tCodeLength );
	if ( tRet < 0 )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] In CClientHandle::Send(),  AppendOneCode return %d", 
				__LK_FILE__, __LINE__, __FUNCTION__, tRet );

		return -1;
	}

#ifdef _DEBUG_
	Message* pUnknownMessagePara = (Message*) pMsg->msgpara();
	LK_ASSERT( pUnknownMessagePara != NULL, return -1 );
	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();

	unsigned char* p = (unsigned char*) &(pPlayer->mClientAddr);
	LOG_DEBUG( "default", "Send2Client(%d.%d.%d.%d:%d, fd=%d len=%d) MsgName[%s] MsgHead[%s] MsgPara[%s]", 
			*(p), *(p+1), *(p+2), *(p+3), pPlayer->mClientPort, pPlayer->mCommHandle, tCodeLength,
			pDescriptor->name().c_str(), pMsg->ShortDebugString().c_str(), 
			pUnknownMessagePara->ShortDebugString().c_str() );
#endif

	return 0;
}

int CClientHandle::Recv()
{

	BYTE tTempCodeBuf[MAX_PACKAGE_LEN] = {0};
	int tTempCodeLength = 0;

	int tTempRet = mC2SPipe->GetHeadCode((BYTE *)tTempCodeBuf, &tTempCodeLength);

	if ( tTempRet < 0 )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] When GetHeadCode from C2SPipe, error ocurr %d", 
				__LK_FILE__, __LINE__, __FUNCTION__, tTempRet );
		return CLIENTHANDLE_QUEUE_CRASH;
	}

	if ( tTempCodeLength == 0 )
	{
		return CLIENTHANDLE_QUEUE_EMPTY;
	}

	if( tTempCodeLength < (int) sizeof(CNetHead) )
	{
		return CLIENTHANDLE_CODE_NOTINTEGRITY;
	}

	CNetHead tNetHead;
	CCSHead tCSHead;
	CMessage tMessage;
	BYTE tMsgPara[MAX_PACKAGE_LEN] = {0};
	int tusParLen = sizeof(tMsgPara); 

	if ( DecodeNetMsg( tTempCodeBuf, tTempCodeLength, &tCSHead, &tMessage, tMsgPara, tusParLen, tNetHead) != CLIENTHANDLE_SUCCESS )
	{
		return CLIENTHANDLE_DECODE_FAILED;
	}

#ifdef _DEBUG_	
	Message* pUnknownMessagePara = (Message*) tMessage.msgpara();
	LK_ASSERT( pUnknownMessagePara != NULL, return -1 );

	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();
	unsigned char* p = (unsigned char*) &tNetHead.mSrcIP;
	LOG_DEBUG( "default", "Recv Client(%d.%d.%d.%d:%d, fd=%d) MsgName[%s] MsgHead[%s] MsgPara[%s] ",
			*(p), *(p+1), *(p+2), *(p+3), tNetHead.mSrcPort, tNetHead.mDstPort, pDescriptor->name().c_str(), 
			tMessage.msghead().ShortDebugString().c_str(), pUnknownMessagePara->ShortDebugString().c_str());
#endif

	if (tMessage.msghead().messageid() == ID_C2G_REQUEST_LOGINGATE)
	{
		// 更新CGatePlayer的某些信息
		CMessageLoginGateRequest* tpEnterGate = (CMessageLoginGateRequest*) tMessage.msgpara();

		if (tpEnterGate) 
		{

			CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(tpEnterGate->accountid());
			if (tpPlayer)
			{
				if (tpEnterGate->logintoken() != (unsigned int)tpPlayer->mLoginToken)
				{
					LOG_FATAL("default", "[ %s : %d ][ %s ] player token %d not match account(%d) %d", 
						__LK_FILE__, __LINE__, __FUNCTION__, 
						tpPlayer->mLoginToken, tpPlayer->mAccountID, tpEnterGate->logintoken());

					return CLIENTHANDLE_SUCCESS;
				}

				// 登录即重置cskey
				tpPlayer->ResetCSKey();

				// 如果是服务器间传送的话直接登录服务器				
				if ( tpPlayer->GetRoleWorldID() > 0 )
				{
					tpPlayer->mClientAddr = tNetHead.mSrcIP;
					tpPlayer->mClientPort = tNetHead.mSrcPort;
					tpPlayer->mSockTime  = tNetHead.mDstIP; 									
					CCoreService::GetSingleton().PlayerDirectEnterGame( tNetHead.mDstPort, tpPlayer );
					GateServer::CGateLogic::GetSingletonPtr()->OnClientMessage(&tMessage);
					return CLIENTHANDLE_SUCCESS;
				}							

				if ( CPlayerManager::GetSingleton().IsPlayerInGlobalScene( tpEnterGate->accountid() ) == true )
				{
					CPlayerManager::GetSingleton().KickPlayerInGlobalScene( tpEnterGate->accountid() );	
				}
				

				//TODO: How close this invalid connection
				switch(tpPlayer->mStatus)
				{
					// 原则1:对于存在与网关的链接的,关闭之前的链接,用新的链接代替
					// 原则2:对于不不存在链接的玩家，向目的地发送踢人请求
					// 原则3:对于踢人请求的链接，处理同原则1					
					case CGatePlayer::em_status_ingame_idle:
					case CGatePlayer::em_status_ongame_v4scene_logingame:
					case CGatePlayer::em_status_ingame_v4scene_redirectscene:
					{

						LOG_INFO("deafult", "[ %s : %d ][ %s ] account(%d) role(%d:%s) is gaming now, it's status(%d)",
								__LK_FILE__, __LINE__, __FUNCTION__, 
								tpPlayer->mAccountID, tpPlayer->mCharID, tpPlayer->mCharName, tpPlayer->mStatus);

						CPlayerManager::GetSingleton().KickOffScenePlayer(tpPlayer, tNetHead.mSrcIP);
						break;
					}

					/*
					case CGatePlayer::em_status_ingame_v4scene_redirectscene:
					{
						LOG_NOTICE("default", "[ %s : %d ][ %s ] account(%d) role(%d:%s) is redirect scene server, so can not be kicked off", 
							__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpPlayer->mCharID, tpPlayer->mCharName);

						break;
					}
					*/

					case CGatePlayer::em_status_inwait_wait:
					{
						LOG_NOTICE("default", "[ %s : %d ][ %s ] Account(%d) login gate normally", 
								__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID);
						break;
					}

					// 如果是双连接，这个地方有问题
					case CGatePlayer::em_status_ingate_v4kickoff: 
					case CGatePlayer::em_status_ingate_v4leavemap: 
					{
						LOG_NOTICE("default", "[ %s : %d ][ %s ] Account(%d) logining process will be rejected", 
								__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID);

						//use a dirty player
						CGatePlayer* pDirtyPlayer = 
							(CGatePlayer*) CGateObjManager::GetSingletonPtr()->CreateObject(OBJTYPE_PLAYER_AT_GATE);

						pDirtyPlayer->Initialize();

						pDirtyPlayer->mClientAddr = tNetHead.mSrcIP;
						pDirtyPlayer->mClientPort = tNetHead.mSrcPort;
						pDirtyPlayer->mSockTime  = tNetHead.mDstIP; 
						pDirtyPlayer->mCommHandle = tNetHead.mDstPort;
						pDirtyPlayer->mStatus = tpPlayer->mStatus; 

						CPlayerManager::GetSingleton().RegistPlayerComm(pDirtyPlayer);
						CPlayerManager::GetSingleton().SendGateErrorNotice(
								pDirtyPlayer, EGATE_PLAYERSTATE_ISNOT_PERMITED, em_need_disconnect);

						CGateObjManager::GetSingletonPtr( )->DestroyObject(pDirtyPlayer->get_id());

						return CLIENTHANDLE_SUCCESS;
					}

					case CGatePlayer::em_status_ingate_idle: 
					case CGatePlayer::em_status_ingate_v4loginqueue: 
					case CGatePlayer::em_status_ingate_v4scene_entergame: 
					case CGatePlayer::em_status_ingate_v4erating_createrole: 
					case CGatePlayer::em_status_ingate_v4erating_passwdverify:
					case CGatePlayer::em_status_ingate_v4erating_entergame: 

					case CGatePlayer::em_status_ingate_v4mysql_createrole: 
					case CGatePlayer::em_status_ingate_v4mysql_obtainroles: 
					case CGatePlayer::em_status_ingate_v4mysql_removerole:

					case CGatePlayer::em_status_ingate_erating_createdrole: 
					case CGatePlayer::em_status_ingate_erating_enteredgame: 
					case CGatePlayer::em_status_ingate_erating_passwdverify:
					default:
					{
						if (tpEnterGate->logintoken() != (unsigned int)tpPlayer->mLoginToken)
						{
							LOG_FATAL("default", "[ %s : %d ][ %s ] player token %d not match account(%d) %d", 
									__LK_FILE__, __LINE__, __FUNCTION__,
								tpPlayer->mLoginToken, tpPlayer->mAccountID, tpEnterGate->logintoken());

							return CLIENTHANDLE_SUCCESS;
						}

						LOG_INFO("deafult", "[ %s : %d ][ %s ] account(%d) is already in gate, it's status is (%x). so kick off", 
								__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpPlayer->mStatus);

						CPlayerManager::GetSingleton().KickOffGatePlayer(tpPlayer, tNetHead.mSrcIP);

						break;
					}
				}

				tpPlayer->mClientAddr = tNetHead.mSrcIP;
				tpPlayer->mClientPort = tNetHead.mSrcPort;
				tpPlayer->mSockTime  = tNetHead.mDstIP; 
				tpPlayer->mCommHandle = tNetHead.mDstPort;

				tpPlayer->m_tLastActiveTime = time(0);
				LOG_INFO("default", "[ %s : %d ][ %s ] update player's socktime(%d) and sockhandle(%d)", 
						__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mSockTime, tpPlayer->mCommHandle);
			}else
			{
				//use a dirty player close this connection.
				CGatePlayer *pDirtyPlayer = 
					(CGatePlayer*) CGateObjManager::GetSingletonPtr()->CreateObject(OBJTYPE_PLAYER_AT_GATE);
				pDirtyPlayer->Initialize();

				pDirtyPlayer->mClientAddr = tNetHead.mSrcIP;
				pDirtyPlayer->mClientPort = tNetHead.mSrcPort;
				pDirtyPlayer->mSockTime  = tNetHead.mDstIP; 
				pDirtyPlayer->mCommHandle = tNetHead.mDstPort;

				CPlayerManager::GetSingleton().RegistPlayerComm(pDirtyPlayer);
				CPlayerManager::GetSingleton().SendGateErrorNotice(
						pDirtyPlayer, EGATE_PLAYERSTATE_ISNOT_PERMITED, em_need_disconnect);

				CGateObjManager::GetSingletonPtr( )->DestroyObject(pDirtyPlayer->get_id());

				unsigned char* p = (unsigned char*) &tNetHead.mSrcIP;
				LOG_FATAL("default", "Recv Client(%d.%d.%d.%d:%d, fd=%d) Can NOT find gateplayer by accountid(%d)!",
						*(p), *(p+1), *(p+2), *(p+3), tNetHead.mSrcPort, tNetHead.mDstPort, tpEnterGate->accountid());
			}
		}
	}

	GateServer::CGateLogic::GetSingletonPtr()->OnClientMessage(&tMessage);

	return CLIENTHANDLE_SUCCESS;
}


int CClientHandle::DecodeNetMsg( BYTE* pCodeBuff, int& vLen, CCSHead* pCSHead, 
			CMessage* pMsg, BYTE* pMsgPara, int& usParaLen, CNetHead& tNetHead)
{
	LK_ASSERT( pCodeBuff != NULL && vLen >= (int) sizeof(CNetHead) && pMsg != NULL, return -1 );

	BYTE* pBuff = pCodeBuff;
	int tLen = vLen;

	//TODO: 应该首先解密 （必要的话还可以压缩）
	//tNetHead.ParseFromArray( pBuff, sizeof(CNetHead) );
	memcpy( &tNetHead, pBuff, sizeof(CNetHead));

	pBuff += sizeof(tNetHead);
	tLen -= sizeof(tNetHead);

	// 在tcpserver 上的socket index 
	short tSockIndex = tNetHead.mDstPort;

	// 是否是断连消息
	if( tNetHead.mState < 0 )
	{
		CCoreService::GetSingleton().OnEventPlayerDisconnectGate(tSockIndex);
		return CLIENTHANDLE_DISCONNECT;
	}

	// 全部使用默认key来解析
	//int nRet = ClientCommEngine::ConvertClientCodeToMsg2( (void*)pBuff, tLen, pCSHead, pMsg );
	CGatePlayer* tpCommPlayer = CPlayerManager::GetSingletonPtr()->GatePlayer( tSockIndex );
	int nRet = 0;

	if ( tpCommPlayer == NULL )
	{
		nRet = ClientCommEngine::ConvertClientCodeToMsg2( (void*)pBuff, tLen, pCSHead, pMsg, ClientCommEngine::tpKey );
	}
	else
	{
		nRet = ClientCommEngine::ConvertClientCodeToMsg2( (void*)pBuff, tLen, pCSHead, pMsg,  tpCommPlayer->GetCSKey( ));
	}


	if( nRet < 0 )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] convert code failed %d, %d bytes", 
				__LK_FILE__, __LINE__, __FUNCTION__, nRet , tLen);

		return -1;
	}

	CMessageHead* tpHead = pMsg->mutable_msghead();

	tpHead->set_srcid (tSockIndex );

	return CLIENTHANDLE_SUCCESS;
}

int CClientHandle::EncodeNetMsg( CMessage* pMsg, BYTE* pCodeBuff, int& vLen, CGatePlayer* pPlayer)
{
	LK_ASSERT( pMsg != NULL && pCodeBuff != NULL && vLen > 0 && pPlayer != NULL, return -1 );

	CNetHead tNetHead;

	tNetHead.mSrcPort = 0;
	if( pMsg->msghead().messageid() == ID_G2C_NOTIFY_GATEERROR )
	{
		CMessageGateErrorNotice* tpGateErrorNotice = (CMessageGateErrorNotice*) pMsg->msgpara();
		if (tpGateErrorNotice != NULL && tpGateErrorNotice->keepalive() != 1)
		{
			tNetHead.mSrcPort = 1; 
		}
	}else if ( pMsg->msghead().messageid() == ID_S2C_NOTIFY_DISCONNECT ) 
	{
			tNetHead.mSrcPort = 1; 
	}
	#ifndef USE_LONG_CONNECTION
	else if ( pMsg->msghead().messageid() == ID_G2C_RESPONSE_ENTERGAME)
	{
		tNetHead.mSrcPort = 1;
	}
	#endif
	else
	{
		tNetHead.mSrcPort = 0;
	}

	tNetHead.mDstIP = pPlayer->mSockTime;
	tNetHead.mDstPort = pPlayer->mCommHandle;

	int tLen = 0;

	// NetHead 序列化
	//tNetHead.SerializeToArray( pCodeBuff, sizeof(CNetHead) );
	memcpy( pCodeBuff, &tNetHead, sizeof(CNetHead));

	int tNetHeadSize = sizeof(tNetHead);
	pCodeBuff += tNetHeadSize;
	tLen += tNetHeadSize;

	CCSHead tCSHead;
	int tusClientCodeLen = vLen - tLen;
	LK_ASSERT(tusClientCodeLen > 0 && tusClientCodeLen <= 0xFFFF, return -1);

	unsigned short usTempShort = (unsigned short) tusClientCodeLen;
	if( ClientCommEngine::ConvertMsgToClientCode2( &tCSHead, pMsg, pCodeBuff, usTempShort, pPlayer->GetCSKey()) < 0 )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] message %d convert failed",
			__LK_FILE__, __LINE__, __FUNCTION__, pMsg->msghead().messageid() );
		return -1;
	}
	tusClientCodeLen = usTempShort;


	tLen += tusClientCodeLen;

	// 返回值
	vLen = tLen;

	return 0;
}

