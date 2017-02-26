// ********************************************************************** //
// Logic.cpp
// 网关服务器逻辑, 读取网关配置,
// 完成登陆,登出, 选择角色,创建角色,删除角色,转发场景服务器消息功能
// ********************************************************************** //

#include "westserver.h"
#include "gatelogic.h"
#include "tinyxml.h"
#include "entity.h"
#include "gateobj_manager.h"

#include "servermessage_in.h"
#include "client_comm_engine.h"
#include "nameservice.h"

#include <google/protobuf/descriptor.h>
#include "boursemodule.h"
#include "gateteammanager.h"
#include "nameservice.h"
#include "repetionmodule.h"
#include "gmservice.h"
#include "mailservice.h"
#include "fcmservice.h"
#include "dirtyword.h"
#include "activity.h"
#include "ibstoremodule.h"	  
#include "friendtype.h"
#include "friendmodule.h"  
#include "familyservice.h"
#include "rankmodule.h"
#include "family_manager.h"
#include "family.h"
#include "corpservice.h"
#include "corpsmanager.h"
#include "chatmessage_pb.hxx.pb.h"
#include "core_service.h"
#include "chatmodule.h"
#include "performance.h"
#include "judgemodule.h"
#include "world.h"
#include "promessage_pb.hxx.pb.h"
#include <list>
#include "bidmgr.h"
#include "ActivityYQS.h"
#include "globalmodule.h"
#include "taskmodule.h"
#include "taskmessage_pb.hxx.pb.h"

using namespace GateServer;

using namespace ClientCommEngine;

// ***************************************************************
// --- CConfigure
// ***************************************************************
CConfigure::CConfigure()
{
	int nOneSecond = 1000;
	nEratingSessionTimeout = 10 * nOneSecond;
	nServerSessionTimeout = 10 * nOneSecond;
	nGateTimerInterval = 1 * nOneSecond;
	nPingProxyInterval = 30 * nOneSecond;
	nClientHandleIdleSleep = 1000;
	nGameHeartBeatTimeout = 30 * nOneSecond;
	nReport2ERatingInterval = 15 * nOneSecond;

	nPlayerStateCheckInterval = 2 * nOneSecond;
	nPlayerWaitConnectTimeout = 10 * nOneSecond;
	nPlayerIdleIngateTimeout = 60 * nOneSecond;
	nPlayerWaitKickoffTimeout = 10 * nOneSecond;
	nPlayerWaitRedirectTimerout = 10 * nOneSecond;
	nPlayerOnGameStateTimeout = 10 * nOneSecond;
	nOnLineNOTimeout = 30 * nOneSecond;
	
	nTeamProSynTimeout	= 3 * nOneSecond;
	nPlayerCountPerScene = -1;
	nLoginCountOnceTimer = 2; // 每1周期最多2个人

	memset(m_szMysqlInfo, 0, sizeof(m_szMysqlInfo));

	nGameID = 504;

	bFcmServiceOn = false;
	bFcmIsFake = false;
	nFcmWarningTimer = 600 * nOneSecond;

	nPlayerActiveTimeout = 300;
	
	nDeleteProtectTime = DELETEPROTECTTIME;
	nDeleteAllowedTime = DELETEALLOWEDTIME;
	nDeleteCoolTime	= DELETECOOLTIME;
	
	nMailDeleteHour = 0;
	nMailDeleteMinute = 0;
	
	memset( nMailBackMinute, 0, sizeof( nMailBackMinute ) );
	memset( nMailBackHour, 0, sizeof( nMailBackHour ) );
	memset( nMailTableID, 0, sizeof( nMailTableID ) );
	
	mProtectLevel = 0;
	mDefaultProtectTime = DEFAULT_PASSWORD_PROTECTTIME;

	// 100304 MAOCY ADD-BEGIN
   // 角色验证问题标志
   roleValidQuestionFlag = 0;
   // 角色验证问题范围
   roleValidQuestionRange = 0;
   // 角色验证问题重试
   roleValidQuestionRetry = 0;
   // 初始化角色验证问题数目
   roleValidQuestionCount = 0;

	mCorpsCampCount = 0;
	mBourseUse = 0;
	// 初始化角色验证问题
	for(int n=0; n<ROLE_VALID_QUESTION_COUNT; n++){
		roleValidQuestions[n].id = 0;
	}
	// 初始化角色验证选项
	for(int j=0; j<ROLE_VALID_QUESTION_GROUP_COUNT; j++){
      roleValidQuestionGroups[j].id = 0;
      for(int i=0; i<ROLE_VALID_QUESTION_OPTION_COUNT; i++){
         roleValidQuestionGroups[j].options[i].id = 0;
      }
	}
	// 100304 MAOCY ADD-END
	
	for (size_t i = 0; i < ARRAY_CNT(mGameList); ++i) 
	{
		mGameList[i] = 0;
	}	
	memset( mInternalIPList, 0, sizeof( mInternalIPList ) );

	mMultiClientIP.initailize();

	mIsCheckMultiClient = 0;
	mCheckMultiClientInterval = 60 * nOneSecond;
	mDefaultClientPerIP = -1;
	mDeleteLevel = 0;
}


// ***************************************************************
// --- CGateServer
// ***************************************************************
//
// ***************************************************************
//  Function:		RecvServerMsg
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::RecvServerMsg(int vMsgCount)
{
	fd_set fds_read;
	struct timeval stMonTime;
	int iTempFD = -1;
	int iTempStatus = tcs_closed;
	int iMaxFD = 0;

	FD_ZERO( &fds_read );
	stMonTime.tv_sec = 0;
	stMonTime.tv_usec = 0;

	iTempFD = mProxyClient.GetSocket()->GetSocketFD();
	iTempStatus = mProxyClient.GetSocket()->GetStatus();

	if ( iTempFD > 0 && iTempStatus == tcs_connected )
	{
		FD_SET(iTempFD, &fds_read);
		if(iTempFD > iMaxFD)
		{
			iMaxFD = iTempFD;
		}
	}

	int iOpenFDNum = select( iMaxFD + 1, &fds_read, NULL, NULL, &stMonTime );

	if ( iOpenFDNum <= 0 )
	{
		return;
	}

	BYTE tCodeBuff[ MAX_CODE_LEN ];
	unsigned short tBuffLen = sizeof( tCodeBuff );
	int iRet = -1;

	iTempFD = mProxyClient.GetSocket()->GetSocketFD();
	if ( iTempFD >0 && FD_ISSET(iTempFD, &fds_read) )
	{
		mProxyClient.GetSocket()->RecvData();
		while ( 1 )
		{
			tBuffLen = sizeof( tCodeBuff );
			iRet = mProxyClient.GetSocket()->GetOneCode( tBuffLen, tCodeBuff );
			if ( iRet <= 0  )
			{
				//LOG_ERROR( "default", "Get One Code From Proxy server Faild %d", iRet );
				break;
			}

			if( tBuffLen == 0 || ((unsigned int) tBuffLen) > sizeof(tCodeBuff))
			{
				LOG_ERROR( "default", "Code Len %d impossible", tBuffLen );
				break;
			}

			CProxyHead tProxyHead;
			CMessage tMessage;

			int decode_ret =  pbmsg_getheadmsg((unsigned char*)tCodeBuff, tBuffLen , &tProxyHead, &tMessage);
			if( decode_ret <= 0 )
			{
				LOG_ERROR("default", "decode db message failed");
				continue;
			}
#ifdef _DEBUG_
			LOG_DEBUG( "default", "Recv Proxy(fd=%d) ProxyHead[%s]", iTempFD, tProxyHead.ShortDebugString().c_str()); 
#endif
			if ( tProxyHead.has_opflag() && CMD_KEEPALIVE == tProxyHead.opflag() && tProxyHead.srcfe() == CET_PROXYSERVER )
			{
#ifdef _DEBUG_
				LOG_DEBUG("default", "RecvFromProxy: %s", tProxyHead.ShortDebugString().c_str());
#endif
				UpdProxyActiveTmStmp();
				continue;
			}

			Message* pUnknownMessagePara = (Message*) tMessage.msgpara();
			LK_ASSERT( pUnknownMessagePara != NULL, return  );

			const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();
#ifdef _DEBUG_
			LOG_DEBUG( "default", "Recv Proxy(fd=%d) MsgName[%s] ProxyHead[%s] MsgHead[%s] MsgPara[%s] ",
					iTempFD , pDescriptor->name().c_str(), tProxyHead.ShortDebugString().c_str(),
					tMessage.msghead().ShortDebugString().c_str(), 
					pUnknownMessagePara ? pUnknownMessagePara->ShortDebugString().c_str() : NULL);
#endif
			char tmpbuf[1024] = {0};
			strcpy( tmpbuf, "rs_" );
			strcat( tmpbuf, pDescriptor->name().c_str() );
			int nProcessMsgCostTime = 0;
			PERF_FUNC_ELAPSED( tmpbuf, CGateLogic::GetSingletonPtr()->OnProxyMessage( &tMessage), nProcessMsgCostTime );
		}
	}
}

// ***************************************************************
//  Function:		RecvClientMsg
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::RecvClientMsg(int vMsgCount)
{
	int vRecvCount = 0;
	BYTE tMsgBuff[ MAX_PACKAGE_LEN ];
	short tMsgLen = sizeof( tMsgBuff );

	while( vRecvCount < vMsgCount )
	{
		tMsgLen = sizeof( tMsgBuff );
		int vRecvRet = mClientHandle->Recv ( );

		vRecvCount ++;

		/*队列已空*/
		if ( vRecvRet == CLIENTHANDLE_QUEUE_EMPTY )
		{
			struct timespec tv;
			tv.tv_sec = 0; 
			tv.tv_nsec = CGateLogic::GetSingleton().mConfig.nClientHandleIdleSleep;

			/* 如果队列空，睡眠 1 ms */
			nanosleep( &tv, NULL );

			break;
		}
		else if ( vRecvRet == CLIENTHANDLE_SUCCESS )
		{

		}
		else if ( vRecvRet == CLIENTHANDLE_DISCONNECT)
		{

		}
		else
		{
			continue;
		}
	}
}


// ***************************************************************
//  Function:		CGateLogic::OnMessageGameHeartBeatNotice
//  Description:	处理游戏服务器过来的心跳消息
//	Input:			vServerID - 服务器ID pMessage - 消息指针
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::RegistSceneServer(int nServerID, CSceneServer* tpServer)
{

	// 向注册的场景发送它需要创建的地图信息
	vector< pair< int, string > > tMapList;
	GetServerMap( tpServer->mServerName, tpServer->mServerID, tMapList );
	if ( tMapList.empty( ) == true )
	{
		LOG_NOTICE("default",  "[ %s : %d ][ %s ] Sceneserver[%d:%s] has no map, It will be run as Repetion server",
				__LK_FILE__, __LINE__, __FUNCTION__, tpServer->mServerID, tpServer->mServerName);

		return;
	}

	// 向注册的场景发送注册成功消息
	CMessage tMessage;
	CMessageServerInfoResponse tServerInfo;

	// 通知场景世界ID
	tServerInfo.set_worldid(mWorldID);
	tServerInfo.set_lineid( tpServer->mLineID );
	tServerInfo.set_gameid( CGateLogic::GetSingleton().mConfig.nGameID);

	tMessage.set_msgpara((uint32_t) &tServerInfo);
	pbmsg_sethead(&tMessage, ID_G2S_SERVER_INFO_RESPONSE, 0, 0, 0, 0, 0, 0);  

	Send2Scene( tpServer, &tMessage);

	LOG_INFO( "default", "[ %s : %d ][ %s ] SceneServer[%d:%s] at IP(%s) Regist Successfully", 
		__LK_FILE__, __LINE__, __FUNCTION__, nServerID, tpServer->mServerName, tpServer->mAddress);

	//////////////////////////////////////////////////////////////////////////
	

	int tCount = 0;
	CMessageCreateMapNotice tCreateMap; 
	for ( size_t i = 0; i < tMapList.size( ); i ++ )
	{
		int tMapKey		= tMapList[ i ].first;
		string tTplName	= tMapList[ i ].second;

		int tLineID		= CSceneMap::GetLineID( tMapKey );
		int tMapID		= CSceneMap::GetMapID( tMapKey );

		if ( tLineID <= 0 || tMapID <= 0 )
		{
			continue;
		}
		tCreateMap.add_lineid( tLineID );
		tCreateMap.add_mapid( tMapID );
		tCreateMap.add_tplname( tTplName.c_str() );
		tCount ++;
	}
	// 通知场景服务器建立地图
	tCreateMap.set_rtncode( 1 );
	tCreateMap.set_count( tCount );
	tMessage.set_msgpara((uint32_t) &tCreateMap);
	pbmsg_sethead(&tMessage, ID_G2S_CREATE_MAP_NOTICE, 0, 0, 0, 0, 0, 0);  
	Send2Scene( nServerID, &tMessage );

	//向登录服务器发送版本更新消息
	/*
	CMessageUpdateVersionNotice tUpdateVersion;
	tUpdateVersion.set_templateversion(tpSceneInfo->templateversion()); 
	tUpdateVersion.set_taskversion(tpSceneInfo->taskversion());

	tMessage.set_msgpara((uint32_t) &tUpdateVersion);
	pbmsg_sethead(&tMessage, ID_G2L_UPDATE_VERSION_NOTICE, 0, 0, 0, 0, 0, 0); 

	CProxyHead tProxyHead;
	pbmsg_setproxy(&tProxyHead, CET_OTHERSERVER, CET_OTHERSERVER, 2, 1, TRANS_P2P, CMD_NULL, NULL, 0);

	char message_buffer[SENDING_BUFFER_SIZE];
	int message_length = pbmsg_setmsg(tProxyHead, tMessage, message_buffer, sizeof(message_buffer));

	if (message_length > 0)
	{
		LOG_DEBUG("default", "Send2Login: MsgName[%s] ProxyHead[%s], MsgHead[%s], MsgPara[%s]", 
			"CMessageUpdateVersionNotice", tProxyHead.ShortDebugString().c_str(), 
			tMessage.ShortDebugString().c_str(), ((Message*) tMessage.msgpara())->ShortDebugString().c_str());

		CGateServer::GetSingleton().Send2LoginImp((BYTE*)message_buffer, message_length);
	}else
	{
		LOG_ERROR("default", "pbmsg_setmsg encoding message (%x) error", tMessage.msghead().messageid());
	}
	*/


	// 向场景发送国家信息
	CMessage tMsg;
	CMessageCountryInfo tMsgFact;
	
	tMsgFact.set_length( sizeof( mCountryArrary ) );
	tMsgFact.set_countrylist( mCountryArrary, sizeof( mCountryArrary ) );

	pbmsg_sethead(&tMsg, ID_G2S_COUNTRY_INFO, 0, 0, 0, 0, 0, 0);
	tMsg.set_msgpara((uint32_t)&tMsgFact);
	
	Send2Scene( nServerID, &tMsg );
}

void CGateServer::SendGWDataReportProto2ERating()
{
	static char szRptFlag [256] = {0};
	static int nMaxRptID = 0;

	int const_player_count_token = 1;
	if ( nMaxRptID < GetMaxSceneServerID() )  
	{
		nMaxRptID = GetMaxSceneServerID();
	}

	//TODO: 报告各个GameServer上的在线玩家
	for (int i = 1; i <= nMaxRptID; ++i)
	{
		CSceneServer *pHost = GetServerByID(i);
		if(pHost == NULL && szRptFlag[i-1] == 0) continue;

		// 只要曾经正常发送过，即置为 1, 不再重置为0了
		if ( pHost != NULL && ( unsigned int )i < sizeof(szRptFlag)) szRptFlag[i-1] = 1;

		for ( size_t j = 0; j < ARRAY_CNT(CGateLogic::GetSingleton().mConfig.mGameList); ++j)
		{
			// 取得当前的GameID
			int nThisGameID = CGateLogic::GetSingleton().mConfig.mGameList[j];
			if ( nThisGameID == 0) break;

			AGIPGWDataReport proto;		

			//TODO: GatewayID
			//proto.setGameID(CGateLogic::GetSingleton().mConfig.nGameID);	
			proto.setGameID(nThisGameID);
			proto.setGatewayID(CGateServer::GetSingleton().GetRegistID());

			// 取得此GameID上的玩家数目
			int nGameIdPlayer = 0; 
			if ( pHost != NULL )
			{
				CSceneServer::GAMEIDPLAYER::iterator iter = pHost->mRptPlayerNumber.find(nThisGameID);
				nGameIdPlayer = ( iter != pHost->mRptPlayerNumber.end() ? iter->second : 0 );
			}else
			{
				nGameIdPlayer = 0;
			}


			// 5-20 更新, erating要求
			proto.setServerID((pHost ? pHost->mServerID : i) + CGateServer::GetSingleton().GetRegistID() * 100);
			proto.addDataInfo(const_player_count_token, pHost ? nGameIdPlayer: 0);

			CAGIPSession* pSession = CGateLogic::GetSingleton().CreateSession4Proto(&proto);
			if (pSession != NULL)
			{
				int nSessionID = pSession->get_id();

#ifdef _DEBUG_
				LOG_DEBUG("erating", "[%d,%d]Send2ERating:MsgName[AGIPGWDataReport]MsgSeq[%d] MsgPara[GameID(%d) GatewayID(%d) ServerID(%d,%d)]",
						i, j, nSessionID, nThisGameID, CGateServer::GetSingleton().GetRegistID(), 
						pHost ? pHost->mServerID : 0, pHost ? nGameIdPlayer: 0);
#endif

				pSession->assign(nSessionID, em_agipsession_gwdatareport, 0, 0, 
						CGateLogic::GetSingleton().mConfig.nEratingSessionTimeout);
			}
		}
	}
}

// ***************************************************************
//  Function:		GetServerByName
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
CSceneServer* CGateServer::GetServerByName( const char* pServerName )
{
	for ( int i = 1; i <= GetMaxSceneServerID() ; i++)
	{
		CSceneServer* pHost = GetServerByID(i);
		if( pHost== NULL) continue;

		if (strcmp( pHost->mServerName, pServerName ) == 0 ) return pHost;
	}

	return NULL;
}

CSceneMap* CGateServer::GetMapByLineMap( int vLineID, int vMapID )
{
	MAPITER tFindIter = mMaps.find( CSceneMap::MakeKey( vLineID, vMapID ) );
	if ( tFindIter == mMaps.end( ) )
		return NULL;

	return (CSceneMap*) CGateObjManager::GetSingletonPtr()->GetObject(tFindIter->second);

}

int  CGateServer::GetMostIdleLineID(int vMapID)
{
	unsigned short nMoreIdleLineID = 0;
	int nLessPlayerCount = 0x7FFFFFFF;

	MAPITER it4map = mMaps.begin();
	for(;it4map != mMaps.end(); ++it4map)
	{
		CSceneMap* pMap = (CSceneMap*) CGateObjManager::GetSingletonPtr()->GetObject(it4map->second);	
		if (pMap == NULL) continue;

		if (pMap->mMapID != vMapID) continue;
		CSceneServer* tpHost = (CSceneServer*) CGateObjManager::GetSingletonPtr()->GetObject(pMap->mServerObjID);

		if (nLessPlayerCount > tpHost->PlayerCount())
		{
			nLessPlayerCount = tpHost->PlayerCount();
			nMoreIdleLineID = pMap->mLineID;
		}
	}

	return nMoreIdleLineID;
}

// ***************************************************************
//  Function:		GetServerByLineMap
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
CSceneServer* CGateServer::GetServerByLineMap( int vLineID, int vMapID )
{
	MAPITER tFindIter = mMaps.find( CSceneMap::MakeKey( vLineID, vMapID ) );
	if ( tFindIter == mMaps.end( ) )
		return NULL;

	CSceneMap* tpMap = (CSceneMap*) CGateObjManager::GetSingletonPtr()->GetObject(tFindIter->second);
	LK_ASSERT( tpMap != NULL, return NULL );

	return ((CSceneServer*) CGateObjManager::GetSingletonPtr()->GetObject(tpMap->mServerObjID));
}

// ***************************************************************
//  Function:		GetServerByID
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
CSceneServer* CGateServer::GetServerByID( int vServerID )
{
	return (CSceneServer*) CGateObjManager::GetSingletonPtr()->GetObject(GetServerObjID(vServerID));
}


// ***************************************************************
//  Function:		DeleteHost
//  Description:	通过玩家名称得到玩家数据
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::DeleteHost(CSceneServer *pHost)
{
	LK_ASSERT(pHost != NULL, return);

	// 调用响应服务器关闭的消息
	CCoreService::GetSingleton().OnEventHostDown(pHost);
	CRepetionModule::GetSingletonPtr( )->OnHostDown( pHost );

	// 删除场景下的地图
	MAPITER it4map = mMaps.begin();
	while(it4map != mMaps.end())
	{
		CSceneMap* pMap = (CSceneMap*) CGateObjManager::GetSingletonPtr()->GetObject(it4map->second);	
		if(pMap == NULL) 
		{
			++it4map;
			continue;
		}

		if (pMap->mServerObjID == pHost->get_id())
		{
			CGateObjManager::GetSingletonPtr()->DestroyObject(it4map->second);
			mMaps.erase(it4map++);
			
		}else
		{
			++it4map;
		}
	}

	// 删除对象
	CGateObjManager::GetSingletonPtr()->DestroyObject(pHost->get_id());
}

// ***************************************************************
//  Function:		Send2Scene
//  Description:	发送数据到场景
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::Send2Scene( int vServerID, CMessage* pMessage )
{
	char message_buffer[MAX_CODE_LEN];
	CProxyHead tProxyHead;

	pbmsg_setproxy(&tProxyHead, CET_OTHERSERVER, CET_GAMESERVER, 2, vServerID, TRANS_P2P, CMD_NULL, NULL, 0);
	int message_length  = pbmsg_setmsg(tProxyHead, *pMessage, message_buffer, sizeof(message_buffer));

	LK_ASSERT( pMessage != NULL , return  );
	Message* pUnknownMessagePara = (Message*) pMessage->msgpara();
	LK_ASSERT( pUnknownMessagePara != NULL, return  );
	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();

	char tmpbuf[1024] = {0};
	strcpy( tmpbuf, "ss_" );
	strcat( tmpbuf, pDescriptor->name().c_str() );
	PERF_BEGIN( tmpbuf );

	if (message_length > 0)
	{
#ifdef _DEBUG_
		LOG_DEBUG("default", "Send2Scence(ServerID=%d, Len=%d): MsgName[%s] ProxyHead[%s] MsgHead[%s] MsgPara[%s]",
			vServerID, message_length, pDescriptor->name().c_str(),	tProxyHead.ShortDebugString().c_str(), 
			pMessage->ShortDebugString().c_str(), ((Message*) pMessage->msgpara())->ShortDebugString().c_str());
#endif
		CGateServer::GetSingleton().Send2ProxyImp((BYTE*)message_buffer, message_length);
			
	}
	else
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] pbmsg_setmsg encoding message (%x:%s) error(%d)", 
				__LK_FILE__, __LINE__, __FUNCTION__, pMessage->msghead().messageid(), pDescriptor->name().c_str(), message_length);
	}
	PERF_END( tmpbuf );
}

// ***************************************************************
//  Function:		Send2Scene
//  Description:	发送数据到场景
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::Send2Scene( CSceneServer* pHost, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	char message_buffer[MAX_CODE_LEN];
	CProxyHead tProxyHead;

	if ( pHost == NULL ) // 发向所有注册的场景服务器
	{
		for ( int i = 1; i <= GetMaxSceneServerID(); i ++ )
		{
			CSceneServer *pHost = GetServerByID(i);
			if(pHost != NULL)
			{
				Send2Scene(pHost, pMessage);
			}
		}
	}
	else
	{
		LK_ASSERT( pMessage != NULL , return  );
		Message* pUnknownMessagePara = (Message*) pMessage->msgpara();
		LK_ASSERT( pUnknownMessagePara != NULL, return  );
		const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();

		char tmpbuf[1024] = {0};
		strcpy( tmpbuf, "ss_" );
		strcat( tmpbuf, pDescriptor->name().c_str() );
		PERF_BEGIN( tmpbuf );

		pbmsg_setproxy(&tProxyHead, CET_OTHERSERVER, CET_GAMESERVER, 2, pHost->mServerID, TRANS_P2P, CMD_NULL, NULL, 0);

		int message_length  = pbmsg_setmsg(tProxyHead, *pMessage, message_buffer, sizeof(message_buffer));

		if (message_length > 0)
		{
#ifdef _DEBUG_
			LOG_DEBUG("default", "Send2Scence(ServerID=%d): MsgName[%s] ProxyHead[%s] MsgHead[%s] MsgPara[%s]",
				pHost->mServerID, pDescriptor->name().c_str(),	tProxyHead.ShortDebugString().c_str(), 
				pMessage->ShortDebugString().c_str(), ((Message*) pMessage->msgpara())->ShortDebugString().c_str());
#endif
			CGateServer::GetSingleton().Send2ProxyImp((BYTE*)message_buffer, message_length);
		}
		else
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ] pbmsg_setmsg encoding message (%x:%s) error", 
					__LK_FILE__, __LINE__, __FUNCTION__, pMessage->msghead().messageid(), pDescriptor->name().c_str());
		}
		PERF_END( tmpbuf );
	}
}

// ***************************************************************
//  Function:		Send2Scene
//  Description:	发送数据到玩家所在场景
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::Send2Scene( CGatePlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CSceneServer* tpServer = GetServerByLineMap( pPlayer->mLineID, pPlayer->mMapID );
	if ( tpServer == NULL )
	{
		LOG_ERROR( "default", "[%s:%d][%s] Can not find the server by LineID(%d) MapID(%d) ", 
				__LK_FILE__, __LINE__, __FUNCTION__, pPlayer->mLineID, pPlayer->mMapID );
		return;
	}

	Send2Scene( tpServer, pMessage );
}

// ***************************************************************
//  Function:		Send2Data
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::Send2Data( int nAccountID, CMessage* pMessage, int nSrcGameID /* = 0 */, int nDesGameID /* = 0 */ )
{
	CProxyHead tProxyHead;

	char message_buffer[MAX_CODE_LEN];

	pbmsg_setproxy(&tProxyHead, CET_OTHERSERVER, CET_DBSERVER, 2, 1, TRANS_P2P, CMD_NULL, NULL, 0);
	tProxyHead.set_uin(nAccountID);
	tProxyHead.set_srcgameid( nSrcGameID );
	tProxyHead.set_desgameid( nDesGameID );

	int message_length = pbmsg_setmsg(tProxyHead, *pMessage, message_buffer, sizeof(message_buffer));

	if (message_length > 0)
	{
#ifdef _DEBUG_
        char nouse[] = "SessionID: 0 SQL: \"update UMS_WORLD";
        if ( strstr(((Message*) pMessage->msgpara())->ShortDebugString().c_str(), nouse) == NULL)
        {
            LOG_DEBUG("default", "Send2Data: head=[%s], msg=[%s], para=[%s] ",
                tProxyHead.ShortDebugString().c_str(), pMessage->ShortDebugString().c_str(),               
                ((Message*) pMessage->msgpara())->ShortDebugString().c_str());                             
        } 
#endif
		CGateServer::GetSingleton().Send2ProxyImp((BYTE*)message_buffer, message_length);
	}else
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] pbmsg_setmsg encoding message (%x) error", 
				__LK_FILE__, __LINE__, __FUNCTION__, pMessage->msghead().messageid());
	}
}

// ***************************************************************
//  Function:		SendMsg2Data
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::SendMsg2Data( google::protobuf::Message* vpMsgFact, int vMsgID, int uID )
{
	CMessage	tMsg;
	CProxyHead tProxyHead;

	pbmsg_sethead( &tMsg, vMsgID, 0, 0, 0, 0, 0, 0);
	tMsg.set_msgpara( (uint32_t)vpMsgFact );

	char message_buffer[MAX_CODE_LEN];

	pbmsg_setproxy(&tProxyHead, CET_OTHERSERVER, CET_DBSERVER, 2, 1, TRANS_P2P, CMD_NULL, NULL, 0);
	tProxyHead.set_uin( uID );

	int message_length = pbmsg_setmsg(tProxyHead, tMsg, message_buffer, sizeof(message_buffer));

	if (message_length > 0)
	{
#ifdef _DEBUG_
        char nouse[] = "SessionID: 0 SQL: \"update UMS_WORLD";
        if ( strstr( vpMsgFact->ShortDebugString().c_str(), nouse) == NULL)
        {
            LOG_DEBUG("default", "Send2Data: head=[%s], msg=[%s], para=[%s] ",
                tProxyHead.ShortDebugString().c_str(), vpMsgFact->ShortDebugString().c_str(),               
                vpMsgFact->ShortDebugString().c_str());                             
        } 
#endif
		CGateServer::GetSingleton().Send2ProxyImp((BYTE*)message_buffer, message_length);
	}else
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] pbmsg_setmsg encoding message (%x) error", 
				__LK_FILE__, __LINE__, __FUNCTION__, vMsgID);
	}
}

// ***************************************************************
//  Function:		Send2GlobalDunserver
//  Description:	给全局副本服务器发送消息
//	Others:
//  Date:			03/27/2011
// 
// ***************************************************************
void CGateServer::Send2GlobalDunserver( CMessage* pMessage )
{	
	CProxyHead tProxyHead;

	char message_buffer[MAX_CODE_LEN];
	pbmsg_setproxy(&tProxyHead, CET_OTHERSERVER, CET_DUNGEON, 2, 1, TRANS_P2P, CMD_NULL, NULL, 0);	
	tProxyHead.set_srcgameid( mWorldID );
	
	int message_length = pbmsg_setmsg(tProxyHead, *pMessage, message_buffer, sizeof(message_buffer));
	if (message_length > 0)
	{
#ifdef _DEBUG_
		char nouse[] = "SessionID: 0 SQL: \"update UMS_WORLD";
		if ( strstr(((Message*) pMessage->msgpara())->ShortDebugString().c_str(), nouse) == NULL)
		{
			LOG_DEBUG("default", "Send2Dunserver: head=[%s], msg=[%s], para=[%s] ",
				tProxyHead.ShortDebugString().c_str(), pMessage->ShortDebugString().c_str(),               
				((Message*) pMessage->msgpara())->ShortDebugString().c_str());                             
		} 
#endif
		CGateServer::GetSingleton().Send2ProxyImp((BYTE*)message_buffer, message_length);
	}else
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] pbmsg_setmsg encoding message (%x) error", 
			__LK_FILE__, __LINE__, __FUNCTION__, pMessage->msghead().messageid());
	}	
}

// ***************************************************************
//  Function:		Send2OtherGate
//  Description:	给其他网关发消息
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			06/16/2008
// 
// ***************************************************************
void CGateServer::Send2OtherGate( CMessage* pMessage, int nWorldID )
{
	LK_ASSERT( pMessage != NULL && nWorldID > 0 , return )
	CProxyHead tProxyHead;
	char message_buffer[MAX_CODE_LEN];
	pMessage->mutable_msghead()->set_srcfe( FE_OTHERGATE );
	pMessage->mutable_msghead()->set_dstfe( FE_GATESERVER );
	pMessage->mutable_msghead()->set_dstid( 0 );
	pbmsg_setproxy(&tProxyHead, CET_OTHERSERVER, CET_OTHERSERVER, 2, 2, TRANS_P2P, CMD_NULL, NULL, 0);	
	tProxyHead.set_srcgameid( CGateServer::GetSingleton().GetWorldID() );
	tProxyHead.set_desgameid( nWorldID );
	int message_length = pbmsg_setmsg(tProxyHead, *pMessage, message_buffer, sizeof(message_buffer));
	if (message_length > 0)
	{
#ifdef _DEBUG_
		char nouse[] = "SessionID: 0 SQL: \"update UMS_WORLD";
		if ( strstr(((Message*) pMessage->msgpara())->ShortDebugString().c_str(), nouse) == NULL)
		{
			LOG_DEBUG("default", "Send2OtherGate: head=[%s], msg=[%s], para=[%s] ",
				tProxyHead.ShortDebugString().c_str(), pMessage->ShortDebugString().c_str(),               
				((Message*) pMessage->msgpara())->ShortDebugString().c_str());                             
		} 
#endif
		CGateServer::GetSingleton().Send2ProxyImp((BYTE*)message_buffer, message_length);
	}else
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] pbmsg_setmsg encoding message (%x) error", 
			__LK_FILE__, __LINE__, __FUNCTION__, pMessage->msghead().messageid());
	}	
}	


// ***************************************************************
//  Function:		Send2Login
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::Send2Login( CMessage* pMessage )
{
	CProxyHead tProxyHead;

	char message_buffer[MAX_CODE_LEN];

	Message* pUnknownMessagePara = (Message*) pMessage->msgpara();
	LK_ASSERT( pUnknownMessagePara != NULL, return  );
	pbmsg_setproxy(&tProxyHead, CET_OTHERSERVER, CET_OTHERSERVER, 2, 1, TRANS_P2P, CMD_NULL);
	
	int message_length = pbmsg_setmsg(tProxyHead, *pMessage, message_buffer, sizeof(message_buffer));

	if (message_length > 0)
	{
#ifdef _DEBUG_
		const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();

		LOG_DEBUG("default", "Send2Login: MsgName[%s] ProxyHead[%d:%s] MsgHead[%s] MsgPara[%s]",
				pDescriptor->name().c_str(), tProxyHead.ByteSize(),	tProxyHead.ShortDebugString().c_str(), 
				pMessage->ShortDebugString().c_str(), pUnknownMessagePara->ShortDebugString().c_str());
#endif
		//CGateServer::GetSingleton().Send2LoginImp((BYTE*) message_buffer, message_length);
		CGateServer::GetSingleton().Send2ProxyImp((BYTE*) message_buffer, message_length);
	}else
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] pbmsg_setmsg encoding message (%x) error", 
				__LK_FILE__, __LINE__, __FUNCTION__, pMessage->msghead().messageid());
	}
}


void CGateServer::Send2Log( CMessage* pMessage, int nSplitor )
{
	CProxyHead tProxyHead;

	char message_buffer[MAX_CODE_LEN];

	Message* pUnknownMessagePara = (Message*) pMessage->msgpara();
	LK_ASSERT( pUnknownMessagePara != NULL, return  );
	pbmsg_setproxy(&tProxyHead, CET_OTHERSERVER, CET_OTHERSERVER, 2, 3, TRANS_P2P, CMD_NULL);

	// for log 
	pMessage->mutable_msghead( )->set_dstid( nSplitor );
	tProxyHead.set_uin(pMessage->msghead().messageid());
	
	int message_length = pbmsg_setmsg(tProxyHead, *pMessage, message_buffer, sizeof(message_buffer));

	if (message_length > 0)
	{
#ifdef _DEBUG_
		const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();

		LOG_DEBUG("default", "Send2Log: MsgName[%s] ProxyHead[%d:%s] MsgHead[%s] MsgPara[%s]",
				pDescriptor->name().c_str(), tProxyHead.ByteSize(),	tProxyHead.ShortDebugString().c_str(), 
				pMessage->ShortDebugString().c_str(), pUnknownMessagePara->ShortDebugString().c_str());
#endif
		//CGateServer::GetSingleton().Send2LoginImp((BYTE*) message_buffer, message_length);
		CGateServer::GetSingleton().Send2ProxyImp((BYTE*) message_buffer, message_length);
	}else
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] pbmsg_setmsg encoding message (%x) error", 
				__LK_FILE__, __LINE__, __FUNCTION__, pMessage->msghead().messageid());
	}
}

// ***************************************************************
//  Function:		GetServerMap
//  Description:	得到指定服务器需要运行的地图列表
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
int CGateServer::GetServerMap( const char* pServerName, unsigned int vServerID, vector< pair< int, string > >& rMapList )
{
	TiXmlDocument tSceneDoc;
	//if( tSceneDoc.LoadFile( "../config/data/scene_config/sceneconfig.xml" ) == false )
	//{
	//	printf( "Cannt Find File %s\n", "../config/data/scene_config/sceneconfig.xml" );
	//	return ERR_UNKNOWN;
	//}

	if( tSceneDoc.LoadFile( CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"scene_config/sceneconfig.xml" )) == false )
	{
		printf( "Cannt Find File %s\n", "../config/data/scene_config/sceneconfig.xml" );
		return ERR_UNKNOWN;
	}
	TiXmlElement* tpRootEle = tSceneDoc.RootElement();
	if ( tpRootEle == NULL )
	{
		printf( "sceneconfig.xml format error!" );
		return -1;
	}

	const char* tpEleName = tpRootEle->Value();
	if ( strcmp( tpEleName, "SceneList" ) != 0 )
	{
		printf( "sceneconfig.xml format error!" );
		return -1;
	}	

	TiXmlElement* tpElement = tpRootEle->FirstChildElement( "SceneObject" );

	while ( tpElement )
	{
		const char* tpName		= tpElement->Attribute( "Name" );
		const char* tpLineID	= tpElement->Attribute( "LineID" );
		const char* tpServerID  = tpElement->Attribute( "ServerID" );

		if ( tpName == NULL || tpLineID == NULL || tpServerID == NULL )
		{
			LOG_ERROR("default", "SceneConfig Error" );
			printf( "SceneConfig Error, Can't Find Name, LineID, ServerID\n" );
			return -1;
		}

		unsigned short tLineID = ::atoi( tpLineID );
		if( tLineID > sizeof( mCountryArrary ) / sizeof( mCountryArrary[ 0 ] ) 
			&& ( IS_REPETION_LINE_ID( tLineID ) == false ) 
			&& ( IS_NEUTRAL_LINE_ID( tLineID ) == false )
			&& ( IS_BATTLE_LINE_ID( tLineID ) == false ) )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ] LineID Err, LineID Cannt equal 0 or big than %d", 
				__LK_FILE__, __LINE__, __FUNCTION__, sizeof( mCountryArrary ) / sizeof( mCountryArrary[ 0 ] ) );
			printf( "SceneConfig Error, Scene %s Line %d Is ERROR", tpName, tLineID );
			return -1;
		}
		if( ( IS_REPETION_LINE_ID( tLineID ) == false ) 
			&& ( IS_NEUTRAL_LINE_ID( tLineID ) == false ) 
			&& ( IS_BATTLE_LINE_ID( tLineID ) == false )
			&& mCountryArrary[ tLineID - 1 ].mName[ 0 ] != '\0' 
			&& strncmp( mCountryArrary[ tLineID - 1 ].mName, tpName, 
			sizeof( mCountryArrary[ tLineID - 1 ].mName ) != 0 ) )

		{
			printf( "SceneConfig Error, LineID %d Is Exist", tLineID );
			LOG_ERROR( "default", "[ %s : %d ][ %s ]LineID %d is have", __LK_FILE__, __LINE__, __FUNCTION__, tLineID );
			return -1;
		}
		else
		{
			if( IS_REPETION_LINE_ID( tLineID ) == false 
				&& IS_NEUTRAL_LINE_ID( tLineID ) == false
				&& IS_BATTLE_LINE_ID( tLineID ) == false )
			{
				strncpy( mCountryArrary[ tLineID - 1 ].mName, tpName, sizeof( mCountryArrary[ tLineID - 1 ].mName ) - 1 );
			}
		}

		unsigned int tServerID = atoi( tpServerID );

		if ( tServerID == vServerID )
		{
			CSceneServer* tpSceneServer = GetServerByID( tServerID );
			if( tpSceneServer == NULL )
			{
				LOG_ERROR( "default", "can't find server by id %d", tServerID );
				printf( "SceneConfig Error, ServerID %d Is Not Exist", tServerID );
				return -1;
			}

			// 设置服务器lineid参数
			tpSceneServer->mLineID = tLineID;

			TiXmlElement* tpSubEle = tpElement->FirstChildElement( "Map" );
			while ( tpSubEle )
			{
				const char* tpMapID = tpSubEle->Attribute( "MapID" );
				const char* tpSceneTpl = tpSubEle->Attribute( "TplName" );

				if ( tpMapID == NULL || tpSceneTpl == NULL )
				{
					tpSubEle = tpSubEle->NextSiblingElement( "Map" );
					continue;
				}				

				unsigned short tMapID = atoi( tpMapID );
				rMapList.push_back( pair< int, string >( CSceneMap::MakeKey( tLineID, tMapID ), tpSceneTpl ) );

				tpSubEle = tpSubEle->NextSiblingElement( "Map" );
			}			
		}

		tpElement = tpElement->NextSiblingElement( "SceneObject" );
	}	

	return 0;
}

template<> CGateServer* CSingleton< CGateServer >::spSingleton = NULL;
CSharedMem* CGateServer::mShmPtr = NULL; 

// ***************************************************************
//  Function:		Initialize
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
int CGateServer::Initialize( int vInitFlag )
{
	if ( vInitFlag == SHM_INIT)
	{
		mMaps.initailize();
		SetMaxSceneServerID(0);

		for (int i = 1; i <= SCENE_AT_GATE_CAP; ++i)
		{
			SetServerObjID(i, INVALID_OBJ_ID);
		}

		mERatingIsLaunched = false;
	}

	CClientHandle::mShmPtr =  mShmPtr;
	mClientHandle = new CClientHandle;

	// 初始化统计日志
	// INIT_ROLLINGFILE_LOG( "stat", "../log/stat.log", (LogLevel)LEVEL_INFO, 10000000, 10 );

	m_LoadConfigFuncList.initailize();
	return 0;
}

// ***************************************************************
//  Function:		CGateServer::SendkeepAliveToProxy
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
int CGateServer::SendkeepAliveToProxy()
{
	CProxyHead  tHead;

	char message_buffer[MAX_CODE_LEN];
	pbmsg_setproxy(&tHead, CET_OTHERSERVER, CET_PROXYSERVER, 2, 1, TRANS_P2P, CMD_KEEPALIVE);

	unsigned short message_length = tHead.ByteSize() + 4;
	*((unsigned short*) message_buffer)  = message_length;

	*((unsigned short*) (message_buffer + 2))  = tHead.ByteSize() ;

	if(tHead.SerializeToArray(message_buffer+4, message_length - 4))
	{
		mProxyClient.GetSocket()->SendOneCode(message_length, (BYTE*)message_buffer);
	}else
	{
		LOG_ERROR("default", " [ %s : %d ][ %s ] SerializeToArray error", __LK_FILE__, __LINE__, __FUNCTION__);
	}

	return 0;
} 


// ***************************************************************
//  Function:		CGateServer::Connect2ProxyServer
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
bool CGateServer::Connect2ProxyServer()
{
	// 初始化服务器
	char tIP[ 64 ] = {0};
	short tPort = 0;

	SplitSockAddr(mProxyServerAddr.c_str(), tIP, tPort );
	mProxyClient.Initialize( FE_GATESERVER, GetGateServerID(), inet_addr( tIP ), tPort );

	// 连接登录服务器和中转服务器

	if( mGateServerLanAddr.length() > 0 )
	{
		SplitSockAddr(mGateServerLanAddr.c_str(), tIP, tPort );
	}
	else
	{
		SplitSockAddr(mGateServerAddr.c_str(), tIP, tPort );
	}

	if ( mProxyClient.ConnectToServer( tIP ) )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ]  Connect to ProxyServer(%s) use %s Faild", 
				__LK_FILE__, __LINE__, __FUNCTION__, mProxyServerAddr.c_str(), tIP );
		return false;
	} 

	return true;
}

// ***************************************************************
//  Function:		CGateServer::UnRegist2LoginServer
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::UnRegist2LoginServer()
{
	CMessageGateShutDownNotice tGateShutDownNotice;
	tGateShutDownNotice.set_worldid(mRegistID);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tGateShutDownNotice);
	pbmsg_sethead(&tMessage, ID_G2L_GATE_SHUTDOWN_NOTICE, 0, 0, 0, 0, 0, 0);

	Send2Login(&tMessage);
}

// ***************************************************************
//  Function:		CGateServer::Regist2LoginServer
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::Regist2LoginServer()
{
	if (!CModuleManager::GetSingleton().IsLaunched())
	{
		//LOG_INFO("default", "There are some modules on gate that have not lauched successfully!");
		return;
	}

	// 网关服务器向登录服务器注册
	CMessageGateInfoNotice tGateInfo;
	tGateInfo.set_address( mGateServerAddr.c_str());
	tGateInfo.set_hostname( mWorldName.c_str());
	tGateInfo.set_gateversion( VERSION );
	tGateInfo.set_worldid( mWorldID ); // no use for erating.
	tGateInfo.set_gatecode( mGateCode.c_str());
	tGateInfo.set_gatepasswd(mGatePasswd.c_str());
	tGateInfo.set_gatemac( mGateMac.c_str());
	tGateInfo.set_gameid( CGateLogic::GetSingleton().mConfig.nGameID);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tGateInfo);
	pbmsg_sethead(&tMessage, ID_G2L_GATE_INFO_NOTICE, 0, 0, 0, 0, 0, 0);

	Send2Login(&tMessage);
}


//************************************
// Method:    SendHeartBeat2Login
// FullName:  GateServer::CGateServer::SendHeartBeat2Login
// Access:    public 
// Returns:   void
// Qualifier: 网关到登陆服务器心跳，附带在线人数
//************************************
void CGateServer::SendHeartBeat2Login()
{
	CMessageGate2LoginHeartBeatNotice tG2LHearBeatMsg;
	tG2LHearBeatMsg.set_playeratgate( CPlayerManager::GetSingleton().GetWorldPlayerNumber() );

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tG2LHearBeatMsg);
	pbmsg_sethead(&tMessage, ID_G2L_HEARTBEAT_NOTICE, 0, 0, 0, 0, 0, 0);
	
	Send2Login(&tMessage);
}


// ***************************************************************
//  Function:		CGateServer::Regist2ProxyServer
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::Regist2ProxyServer()
{
	CProxyHead  tHead;

	char message_buffer[MAX_CODE_LEN];
	pbmsg_setproxy(&tHead, CET_OTHERSERVER, CET_PROXYSERVER, 2, 1, TRANS_P2P, CMD_REGSERVER);

	unsigned short message_length = tHead.ByteSize() + 4;
	*((unsigned short*) message_buffer)  = message_length;

	*((unsigned short*) (message_buffer + 2))  = tHead.ByteSize() ;

	 if(tHead.SerializeToArray(message_buffer+4, message_length - 4))
	{
		LOG_INFO("default", "[ %s : %d ][ %s ] regist to proxyserver (len:%d): head=[%d:%s]",  
				__LK_FILE__, __LINE__, __FUNCTION__, message_length, tHead.ByteSize(), tHead.ShortDebugString().c_str());

		mProxyClient.GetSocket()->SendOneCode(message_length, (BYTE*)message_buffer);
	}else
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] regist to proxyserver error", __LK_FILE__, __LINE__, __FUNCTION__);
	}
}

// ***************************************************************
//  Function:		Send2ProxyImp
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
int CGateServer::Send2ProxyImp(BYTE* pMessage, int vMsgLen)
{
	return mProxyClient.GetSocket()->SendOneCode(vMsgLen, pMessage);
}

// ***************************************************************
//  Function:		Send2Player
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
int CGateServer::Send2Player( std::vector<CGatePlayer*>& vPlayerList, CMessage* pMessage )
{
	LK_ASSERT( pMessage!= NULL, return -1 );

	Message* pUnknownMessagePara = (Message*) pMessage->msgpara();
	LK_ASSERT( pUnknownMessagePara != NULL, return -1 );
	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();

	LK_ASSERT( pDescriptor != NULL, return -1 );

	char tmpbuf[1024] = {0};
	strcpy( tmpbuf, "sp_" );
	strcat( tmpbuf, pDescriptor->name().c_str() );
	PERF_BEGIN( tmpbuf );
    int tRet = mClientHandle->Send(pMessage, &vPlayerList);
	PERF_END( tmpbuf );
	return tRet;
}

int CGateServer::Send2Player( CGatePlayer* pPlayer, CMessage* pMessage )
{   
	LK_ASSERT( pMessage!= NULL && pPlayer != NULL, return -1 );
	Message* pUnknownMessagePara = (Message*) pMessage->msgpara();
	LK_ASSERT( pUnknownMessagePara != NULL, return -1 );
	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();
	LK_ASSERT( pDescriptor != NULL, return -1 );
	
	char tmpbuf[1024] = {0};
	strcpy( tmpbuf, "sp_" );
	strcat( tmpbuf, pDescriptor->name().c_str() );
	int tRet = -1;
	PERF_BEGIN( tmpbuf );
	if ( CPlayerManager::GetSingleton().GatePlayer(pPlayer->mCommHandle) == pPlayer)
	{
		tRet =  mClientHandle->Send(pMessage, pPlayer);
	}
	else
	{
		LOG_FATAL("default", "Can not send player because its handle(%d) does not be registed succuss.", 
				pPlayer->mCommHandle);
	}
	PERF_END( tmpbuf );
	return tRet;
}   

// ***************************************************************
//  Function:		ExitServer
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::ExitServer()
{
	// 各场景服务的在线人数监控数据置零
	int tAccountID = 100;
	for(int i = 1; i <= GetMaxSceneServerID(); ++i)
	{
		CSceneServer *pHost = GetServerByID(i);
		if(pHost == NULL) continue;

		CGateLogic::GetSingleton().ExecuteSql( em_dbsession_null, tAccountID, 0, tAccountID, SQL_NONE, UPDATE,
			"update UMS_ONLINENO set online_number = '%d', update_time = Now() where server_id = '%d'",
			0,
			pHost->mServerID );
	}

	UnRegist2LoginServer();
}

// ***************************************************************
//  Function:		OnReloadCfg
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::OnReloadCfg()
{
	for (int i = 1; i <= GetMaxSceneServerID(); ++i)
	{
		CSceneServer *pHost = GetServerByID(i);
		if(pHost == NULL) continue;

		int n = CGateLogic::GetSingleton().mConfig.nPlayerCountPerScene;
		if ( n < pHost->mPlayerCapacity && n > 0 )
		{
			pHost->mSizePermited = n;
		}else
		{
			pHost->mSizePermited = pHost->mPlayerCapacity;
		}
	}
}

// ***************************************************************
//  Function:		OnLaunch
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::OnLaunch()
{
	// 连接中转服务器和登录服务器
	Connect2ProxyServer();

	// 网关服务器向中转服务器注册
	Regist2ProxyServer();		
}

// ***************************************************************
//  Function:		UpdateSceneRptPlayerNumber
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::UpdateSceneRptPlayerNumber(int nServerID, int nRptNumber)
{
	CSceneServer* tpServer = GetServerByID( nServerID );
	if ( tpServer == NULL )
	{
		LOG_ERROR("default",  "[ %s : %d ][ %s ] The sceneserver %d is NOT be registered", 
				__LK_FILE__, __LINE__, __FUNCTION__, nServerID );
		return;
	}

	// 各游戏服务器上报的游戏人数
	// tpServer->mRptPlayerNumber = nRptNumber;

	// 重置时钟
	tpServer->mActiveTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nGameHeartBeatTimeout);
}

// ***************************************************************
//  Function:		RegistSceneMap
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
bool CGateServer::RegistSceneMap(int nServerID, int nLineID, int nMapID)
{
	CSceneServer* tpServer = CGateServer::GetSingleton().GetServerByID( nServerID );
	if ( tpServer == NULL) return false;

	// 收到了建立地图的回应，可以注册地图
	CSceneMap* tpMap = (CSceneMap*) CGateObjManager::GetSingletonPtr()->CreateObject(OBJTYPE_MAP_AT_GATE);	
	if ( tpMap == NULL)
	{
		LOG_FATAL("default", "[ %s : %d ][ %s ] There is not enough CSceneMap object in the pool.",
				__LK_FILE__, __LINE__, __FUNCTION__ );
		return false;
	}

	tpMap->mMapID = nMapID;
	tpMap->mLineID = nLineID;
	tpMap->mServerObjID = tpServer->get_id();

	pair<MAPITER, bool> ret = 
			mMaps.insert( MAPTYPE( CSceneMap::MakeKey( nLineID, nMapID ), tpMap->get_id() ) );

	if ( ret.second == false)
	{
		CGateObjManager::GetSingletonPtr()->DestroyObject(tpMap->get_id());
		return false;
	}


	return true;
}

// ***************************************************************
//  Function:		RegistRepetionInfo
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
bool CGateServer::RegistRepetionInfo( CMessageRegistRepetionInfo* vpMsgRepetionInfo )
{
	CRepetionInfo* tpRepetionInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionMapInfo(  
		vpMsgRepetionInfo->repetionid( ) );
	if( tpRepetionInfo != NULL )
	{
		if( tpRepetionInfo->GetRepetionIndex( ) == INVALID_REPETION_INDEX )
		{
			tpRepetionInfo->SetRepetionIndex( vpMsgRepetionInfo->repetionid( ) );
			for( int i = 0; i < vpMsgRepetionInfo->mapid_size( ); i++ )
			{
				tpRepetionInfo->SetMapID( vpMsgRepetionInfo->mapid( i ) );
			}
		//	tpRepetionInfo->SetPlayerCountMax( vpMsgRepetionInfo->countmax( ) );
			tpRepetionInfo->SetPlayerCountMax( vpMsgRepetionInfo->playermax());
			tpRepetionInfo->SetBackMapID( vpMsgRepetionInfo->backmapid( ) );
			tpRepetionInfo->SetBackPos( vpMsgRepetionInfo->backposx( ), vpMsgRepetionInfo->backposy( ) );
			tpRepetionInfo->SetOnTimeOpen( vpMsgRepetionInfo->isontimeopen( ) );
			tpRepetionInfo->SetNotLimitTime( vpMsgRepetionInfo->notlimittime( ));			
			tpRepetionInfo->SetRelogin( vpMsgRepetionInfo->relogin() );
			LOG_NOTICE( "rep", "[ %s : %d ][ %s ]RepetionInfo Regist RepetionIndex Is %d PlayerCountMax is %d", 
					__LK_FILE__, __LINE__, __FUNCTION__, vpMsgRepetionInfo->repetionid( ), 
					vpMsgRepetionInfo->countmax( ) );

			return true;
		}else
		{
			LOG_ERROR( "rep", "[ %s : %d ][ %s ]RepetionInfo( RepetionIndex %d ) Is Already Registed", 
					__LK_FILE__, __LINE__, __FUNCTION__, vpMsgRepetionInfo->repetionid( ) );

			return false;
		}

	}else
	{
		LOG_ERROR( "rep", "[ %s : %d ][ %s ]RepetionInfo Cannt Get By RepetionIndex( %d )", 
				__LK_FILE__, __LINE__, __FUNCTION__, vpMsgRepetionInfo->repetionid( ) );
		return false;
	}

	return true;
}

// ***************************************************************
//  Function:		CheckSceneVersion
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
int CGateServer::CheckSceneVersion(int nServerID, int nVersion, int nTplVersion)
{
	CMessage tMessage;

	// 检查版本号是否一致
	if ( nVersion != VERSION )
	{
		LOG_WARN( "default", "[ %s : %d ][ %s ] The scenesever(%d) version(%d) dos not math gate version(%d)",
				__LK_FILE__, __LINE__, __FUNCTION__, nServerID, nVersion, VERSION);

		return -1;
	}

	// 校验 Template 版本
	if (GetMaxSceneServerID() > 0)
	{
		//如果仅有一个服务器, 且就是当前要注册的。则不验证版本	
		if (GetMaxSceneServerID() == nServerID &&  GetSceneServerNumber() == 1)
		{
			LOG_INFO("default", "[ %s : %d ][ %s ] sceneserver(%d) upgrade version (%d) to (%d)", 
					__LK_FILE__, __LINE__, __FUNCTION__, nServerID, mHostTemplateVer, nVersion);

		}else
		{
			if (mHostTemplateVer != nTplVersion)
			{
				LOG_WARN( "default", "[ %s : %d ][ %s ] The scenesever(%d) tempate version(%d) dos not math with prev version(%d)",
						__LK_FILE__, __LINE__, __FUNCTION__, nServerID, nVersion, mHostTemplateVer);

				return -1;
			}
		}
	}

	return 0;
}

// ***************************************************************
//  Function:		RegistSceneByInitial
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
int CGateServer::RegistSceneByInitial(int nServerID, const char* szSceneName,  const char* szSceneIP,
		int nTplVersion, int nCodeVersion, int nTaskVersion, int nSceneCap )
{
	CSceneServer *pHost = CGateServer::GetSingleton().GetServerByID(nServerID);
	if (pHost != NULL )
	{
		if (strcmp(pHost->mServerName, szSceneName) == 0 )
		{
			LOG_NOTICE("default", "[ %s : %d ][ %s ] Gate recv sceneserver(%d:%s) reregist request.",
					__LK_FILE__, __LINE__, __FUNCTION__, nServerID, pHost->mServerName);

			DeleteHost(pHost);

			SetServerObjID(nServerID, INVALID_OBJ_ID);
			if ( nServerID == GetMaxSceneServerID())
			{
				ResetMaxSceneServerID();
			}

		}else
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ] The sceneserver id(%d) has been registed by %s", 
					__LK_FILE__, __LINE__, __FUNCTION__, nServerID, pHost->mServerName);

			// 通知场景服务器建立失败
			CMessage tMessage;
			CMessageCreateMapNotice tCreateMap;
			tCreateMap.set_rtncode( 0 );
			tCreateMap.set_notice( "repeated server id" );

			tMessage.set_msgpara((uint32_t) &tCreateMap);
			pbmsg_sethead(&tMessage, ID_G2S_CREATE_MAP_NOTICE, 0, 0, 0, 0, 0, 0);  

			Send2Scene( nServerID, &tMessage );

			return -1;
		}
	}

	// 创建新的Server
	pHost = (CSceneServer*) CGateObjManager::GetSingletonPtr()->CreateObject(OBJTYPE_SCENE_AT_GATE);
	LK_ASSERT( pHost != NULL, return -1);

	// 对SceneServer 赋值
	strncpy(pHost->mServerName, szSceneName, sizeof(pHost->mServerName));
	pHost->mServerName[sizeof(pHost->mServerName) -1] = 0;

	strncpy(pHost->mAddress, szSceneIP, sizeof(pHost->mAddress));
	pHost->mAddress[sizeof(pHost->mAddress) -1] = 0;

	pHost->mPlayerNumber = 0;
	pHost->mRptPlayerNumber.initailize();

	pHost->mTemplateVersion = nTplVersion;
	pHost->mTaskVersion = nTaskVersion;
	pHost->mServerVersion = nCodeVersion;

	pHost->mServerID = nServerID;
	pHost->mLaunchMode = GAME_LAUNCH_BY_INITIAL;
	pHost->mLaunchTimeStampt = time(0);
	pHost->mPlayerCapacity = nSceneCap;
	pHost->mbIsFull = false;


	if ( CGateLogic::GetSingleton().mConfig.nPlayerCountPerScene > nSceneCap ) 
	{
		pHost->mSizePermited = nSceneCap;
	}else
	{
		pHost->mSizePermited = CGateLogic::GetSingleton().mConfig.nPlayerCountPerScene;
	}

	pHost->mPermitPerTimer = CGateLogic::GetSingleton().mConfig.nLoginCountOnceTimer;

	SetServerObjID(nServerID, pHost->get_id());
	if ( nServerID > GetMaxSceneServerID())
	{
		SetMaxSceneServerID( nServerID);
	}

	// 注册 Server
	RegistSceneServer(nServerID, pHost);
	mHostTemplateVer =  nTplVersion;

	// 告诉场景服务器活动列表
	CServiceActivity::GetSingleton().SendTodayList2Scene( nServerID, true );

	CBourseModule::GetSingletonPtr( )->OnHostDown( pHost );
	return 0;
}


// ***************************************************************
//  Function:		RegistSceneByRecovery
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
int CGateServer::RegistSceneByRecovery(int nServerID, const char* szSceneName,  const char* szSceneIP,
		int nTplVersion, int nCodeVersion, int nTaskVersion, int nSceneCap )
{
	CSceneServer *pHost = GetServerByID(nServerID);
	if (pHost != NULL && strcmp(pHost->mServerName, szSceneName) == 0 )
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] Sceneserver(%d:%s) will recovery, "
				"latest info: start timestampt(%d) address(%s) cap(%d) online(%d) ",
				__LK_FILE__, __LINE__, __FUNCTION__, pHost->mServerID, pHost->mServerName, 
				pHost->mLaunchTimeStampt, pHost->mAddress, pHost->mPlayerCapacity, pHost->mPlayerNumber);

		pHost->mLaunchMode = GAME_LAUNCH_BY_RECOVERY;
		pHost->mLaunchTimeStampt = time(0);

		// 注册 Server
		RegistSceneServer(nServerID, pHost);
		mHostTemplateVer =  nTplVersion;
	}else
	{
		LOG_FATAL("default", "[ %s : %d ][ %s ] Sceneserver(%d:%s) has been destoried, It CAN NOT BE recovery",
				__LK_FILE__, __LINE__, __FUNCTION__, nServerID, szSceneName);

		// 通知场景服务器建立失败
		CMessage tMessage;
		CMessageCreateMapNotice tCreateMap;
		tCreateMap.set_rtncode( 0 );
		tCreateMap.set_notice( "server data has been destroyed,can't restart at recover mode" );

		tMessage.set_msgpara((uint32_t) &tCreateMap);
		pbmsg_sethead(&tMessage, ID_G2S_CREATE_MAP_NOTICE, 0, 0, 0, 0, 0, 0);  

		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
		return -1;
	}

	return 0;	
}

// ***************************************************************
//  Function:		CheckGameServerTimeout
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::CheckGameServerTimeout()
{
	for (int i = 1; i <= GetMaxSceneServerID(); ++i)
	{
		CSceneServer* pHost = GetServerByID(i);
		if (pHost == NULL) continue;

		if ( pHost->mActiveTimer.Elapse(CGateLogic::GetSingleton().mConfig.nGateTimerInterval))
		{
			LOG_NOTICE("default", "[ %s : %d ][ %s ] Delete hosts(%d:%s) and all players(%d,%d) on it",
					__LK_FILE__, __LINE__, __FUNCTION__, 	
					pHost->mServerID, pHost->mServerName, pHost->mPlayerNumber, pHost->PlayerCount());

			DeleteHost(pHost);
			SetServerObjID(i, INVALID_OBJ_ID); 
		}
	}

	ResetMaxSceneServerID();
}

// ***************************************************************
//  Function:		KeepAlive2Proxy
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateServer::KeepAlive2Proxy()
{
	SendkeepAliveToProxy();

	if ( mProxyClient.GetSocket()->GetStatus() != tcs_connected )
			//|| (time(0) - mLastPorxyKeepAliveTime) > 3.1 * mTimerOfKeepAlive.mMaxDuration)
	{
		mProxyClient.GetSocket()->Close();

		if (Connect2ProxyServer())
		{
			Regist2ProxyServer();
		}
	}
}

void CGateServer::SaveSceneReport2DB()
{
	int tAccountID = 100;
	for(int i = 1; i <= GetMaxSceneServerID(); ++i)
	{
		CSceneServer *pHost = GetServerByID(i);
		if(pHost == NULL) continue;

		CGateLogic::GetSingleton().ExecuteSql( em_dbsession_null, tAccountID, 0, tAccountID, SQL_NONE, UPDATE,
				"update UMS_ONLINENO set online_number = '%d', update_time = Now() where server_id = '%d'",
				pHost->PlayerCount(),
				pHost->mServerID );
	}
}

void CGateServer::ReportData2Erating()
{
	if ( IsERatingLaunched())
	{
		SendGWDataReportProto2ERating();
	}
}

CGateServer::CGateServer()
{
	if ( mShmPtr->GetInitMode () == SHM_INIT )
	{
		LOG_INFO( "default", "[ %s : %d ][ %s ] CGateServer Initailize", __LK_FILE__, __LINE__, __FUNCTION__);
		Initialize(SHM_INIT);
	} 
	else
	{
		LOG_INFO( "default", "[ %s : %d ][ %s ] CGateServer Resume", __LK_FILE__, __LINE__, __FUNCTION__);
		Resume();
	}
}

int CGateServer::Resume()
{
	CClientHandle::mShmPtr =  mShmPtr;
	mClientHandle = new CClientHandle;


	return 0;
}

void* CGateServer::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CGateServer::operator delete( void* pointer )
{

}

unsigned int CGateServer::CountSize()
{
	return sizeof(CGateServer);
}

// ***************************************************************
//  Function:		DeleteExpiredMail
//  Description:	删除过期邮件
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			01/13/2010
// 
// ***************************************************************
void CGateServer::DeleteExpiredMail( )
{
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_null, 0, 0, 0, SQL_NONE, UPDATE,
		"call DelExpiredMail ");
}		  

// ***************************************************************
//  Function:		BackExpiredMail
//  Description:	退回到期邮件
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			01/13/2010
// 
// ***************************************************************
void CGateServer::BackExpiredMail( int nMailTableID )
{
	if ( nMailTableID < 1 || nMailTableID > MAXSERVERNUM )
	{
		return;
	}	
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_null, 0, 0, 0, SQL_NONE, UPDATE,
		"call BackExpiredMail_%02d ", nMailTableID );	
}

// ***************************************************************
//  Function:		CheckMailSysActionTime
//  Description:	检查并执行系统邮件相关的操作
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			01/13/2010
// 
// ***************************************************************
void CGateServer::CheckMailSysActionTime( )
{ 
	static int tMailDeleteTime = 0;	
	static int tMailBackTime[MAXSERVERNUM] = { 0 };
	
	time_t tTimeNow = time( NULL );		
	tm *tLocaltm = localtime( &tTimeNow );		
	if ( tLocaltm->tm_hour == CGateLogic::GetSingleton().mConfig.nMailDeleteHour && 
		tLocaltm->tm_min == CGateLogic::GetSingleton().mConfig.nMailDeleteMinute )
	{
		if ( tTimeNow - tMailDeleteTime >= 300 )
		{
			DeleteExpiredMail();
			tMailDeleteTime = tTimeNow;
			LOG_NOTICE( "default","Excute DeleteExpiredMail at time %d:%d ", tLocaltm->tm_hour, tLocaltm->tm_min );
		}		
	}
	
	for ( int i = 0; i < MAXSERVERNUM; ++i  )
	{
		if ( CGateLogic::GetSingleton().mConfig.nMailTableID[i] == 0 )
		{
			continue;
		}
		
		if ( tLocaltm->tm_hour == CGateLogic::GetSingleton().mConfig.nMailBackHour[i] && 
			tLocaltm->tm_min == CGateLogic::GetSingleton().mConfig.nMailBackMinute[i] )
		{
			if ( tTimeNow - tMailBackTime[i] >= 300 )
			{
				BackExpiredMail( CGateLogic::GetSingleton().mConfig.nMailTableID[i] );
				tMailBackTime[i] = tTimeNow;
				LOG_NOTICE( "default","Excute BackExpiredMail( %d ) at time %d:%d ",
					CGateLogic::GetSingleton().mConfig.nMailTableID[i], tLocaltm->tm_hour, tLocaltm->tm_min );
			}		
		}
	}
		
}
// 执行系统删除和退信操作
void CGateServer::ExcuteMailSysAction()
{
	DeleteExpiredMail();
	for ( int i = 0; i < MAXSERVERNUM; ++i  )
	{	
		if ( CGateLogic::GetSingleton().mConfig.nMailTableID[i] != 0 )
		{
			BackExpiredMail(  CGateLogic::GetSingleton().mConfig.nMailTableID[i] );
		}					
	}		
	
	time_t tTimeNow = time( NULL );
	tm *tpLocalTime = localtime( &tTimeNow );
	LOG_NOTICE( "default","Excute MailSysAction at time %d:%d", tpLocalTime->tm_hour, tpLocalTime->tm_min );
}

int CGateServer::ReLoadAppointedConfig()
{
	// 处理需reload的文件清单
	const char* pcReloadCfg = "../config/data/reload.cfg";
	if (pcReloadCfg == NULL)
	{
		return -1;
	}

	FILE* pFile = fopen(pcReloadCfg, "rb");
	if (pFile == NULL)
	{
		printf("error: can not find %s !!!\n", pcReloadCfg);
		return -1;
	}
	int nLength = get_file_length(pcReloadCfg);
	if (nLength == 0)
	{
		return -1;
	}

	char acFilePath[256] = {0};
	while (!feof(pFile))
	{
		char* pcRead = fgets(acFilePath, 256, pFile);
		if (pcRead == NULL)
		{
			continue;
		}

		// 去掉换行符
		char* pcTemp = acFilePath;
		while (pcTemp != NULL && *pcTemp != '\0')
		{
			if (*pcTemp == '\r' || *pcTemp == '\n')
			{
				*pcTemp = '\0';
			}
			pcTemp++;
		}

		int nRetCode = ReLoadOneConfig(acFilePath);
		if (nRetCode == SUCCESS)
		{
			LOG_NOTICE( "default", "xyj-gateserver  reload  %s  success! ...", acFilePath);
			printf("reload config %s  success ...\n", acFilePath);
		}
		else
		{
			LOG_NOTICE( "default", "xyj-gateserver  reload  %s fail !!!", acFilePath);
			printf("reload config %s  fail !!! ...\n", acFilePath);
		}		
	}

	return 0;
}

int CGateServer::ReLoadOneConfig(const char *pcConfigFilePath)
{
	if (pcConfigFilePath == NULL)
	{
		return -1;
	}

	LoadConfigFunclist::iterator iter;

	iter = m_LoadConfigFuncList.find(pcConfigFilePath);
	if (iter != m_LoadConfigFuncList.end())
	{
		TmLoadConfigCallback pFuncCallback = iter->second;
		if (pFuncCallback != NULL)
		{
			pFuncCallback(pcConfigFilePath);

			return 0;
		}
	}

	int nStrLen = strlen(pcConfigFilePath);
	char acKey[256] = {0};
	char acTemp[256] = {0};
	strncpy(acTemp, pcConfigFilePath, 256);
	char* pcTemp = const_cast<char*>(acTemp) + nStrLen - 1;
	char* pcExtendName = NULL;
	if (pcTemp == NULL)
	{
		return -1;
	}

	for (int i = nStrLen; i > 0; i--)
	{
		if (*(pcTemp) == '.')
		{
			pcExtendName = pcTemp;
			break;
		}
		pcTemp--;
	}

	pcTemp = const_cast<char*>(acTemp) + nStrLen - 1;
	for (int i = nStrLen; i > 0; i--)
	{
		if (*(pcTemp) == '/')
		{
			*pcTemp = '\0';
			break;
		}
		pcTemp--;
	}

	if (pcExtendName == NULL)
	{
		return -1;
	}

	snprintf(acKey, 256, "%s+%s", acTemp, pcExtendName);

	iter = m_LoadConfigFuncList.find(acKey);
	if (iter != m_LoadConfigFuncList.end())
	{
		TmLoadConfigCallback pFuncCallback = iter->second;
		if (pFuncCallback != NULL)
		{
			pFuncCallback(pcConfigFilePath);

			return 0;
		}
	}

	return -1;
}

int CGateServer::RegisterLoadConfigCallback(const char* pcConfigPath, TmLoadConfigCallback pFuncCallback)
{
	if (pcConfigPath == NULL || pFuncCallback == NULL)
	{
		return -1;
	}

	LoadConfigFunclist::value_type PairValue(pcConfigPath, pFuncCallback);
	m_LoadConfigFuncList.insert(PairValue);

	//printf( "register LoadconfigCallback for %s ...\n", pcConfigPath);
	return 0;
}

int CGateServer::RegisterLoadConfigCallback(const char* pcDirPath, const char* pcExtName, TmLoadConfigCallback pFuncCallback)
{
	if (pcDirPath == NULL || pcExtName == NULL)
	{
		return -1;
	}

	char acKey[256] = {0};
	snprintf(acKey, 256, "%s+%s", pcDirPath, pcExtName);
	RegisterLoadConfigCallback(acKey, pFuncCallback);

	return 0;
}

int CGateServer::LoadIPConfig(const char *pcConfigPath, CConfigure& vConfigure)
{
	TiXmlDocument tIPDoc;

	if( tIPDoc.LoadFile(pcConfigPath) != true )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] load xml %s failed", 
			__LK_FILE__, __LINE__, __FUNCTION__, pcConfigPath );
		printf( "Cannt Find File %s\n", pcConfigPath );
		return -1;
	}

	TiXmlElement* tpIPElement = NULL;
	TiXmlElement* tpIPChildElm = NULL;
	TiXmlElement* tpIPRoot = tIPDoc.FirstChildElement( );

	tpIPElement = tpIPRoot->FirstChildElement( "server" );
	if( tpIPElement != NULL )
	{
		tpIPChildElm = tpIPElement->FirstChildElement( "gate_server" );
		if( tpIPChildElm != NULL )
		{	
			const char* pszGateAddr = tpIPChildElm->Attribute( "wan_addr" );
			if( pszGateAddr == NULL )
			{
				LOG_ERROR( "default", "[ %s : %d ][ %s ] configure file(%s) has no wan_addr node", 
					__LK_FILE__, __LINE__, __FUNCTION__, pcConfigPath);
				printf( "configure file(%s) has no gate_server <address:wan_addr> node\n", pcConfigPath );
				return -1;
			}

			CGateServer::GetSingleton().SetGateServerIP( pszGateAddr );

			const char* pszGateLanAddr = tpIPChildElm->Attribute( "lan_addr" );
			if( pszGateAddr != NULL )
			{
				CGateServer::GetSingleton().SetGateServerLanIP( pszGateLanAddr );
			}

			const char* pszGateID = tpIPChildElm->Attribute( "id" );
			if( pszGateID == NULL )
			{
				LOG_ERROR( "default", "[ %s : %d ][ %s ] configure file(%s) has no gate_server node", 
					__LK_FILE__, __LINE__, __FUNCTION__, pcConfigPath);
				printf( "configure file(%s) has no gate_server <address:id> node\n", pcConfigPath );
				return -1;
			}

			CGateServer::GetSingleton().SetGateServerID( ::atoi(pszGateID) );
		}
		else
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ] configure file(%s) has no gate_server node", 
				__LK_FILE__, __LINE__, __FUNCTION__, pcConfigPath);
			printf( "configure file(%s) has no gate_server node\n", pcConfigPath );
			return -1;
		}

		tpIPChildElm = tpIPElement->FirstChildElement("proxy_server");
		if( tpIPChildElm != NULL )
		{
			CGateServer::GetSingleton().SetProxyServerIP(tpIPChildElm->Attribute("address"));
		}
		else
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ] configure file(%s) has no proxy_server node", 
				__LK_FILE__, __LINE__, __FUNCTION__, pcConfigPath);
			printf( "configure file(%s) has no proxy_server node\n", pcConfigPath);
			return -1;	
		}

		tpIPChildElm = tpIPElement->FirstChildElement("mysql");
		if( tpIPChildElm != NULL )
		{
			strncpy(vConfigure.m_szMysqlInfo, tpIPChildElm->Attribute("mysql_info"), sizeof(vConfigure.m_szMysqlInfo));
			vConfigure.m_szMysqlInfo[sizeof(vConfigure.m_szMysqlInfo)-1] = 0;
		}else
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ] configure file(%s) has no proxy_server node", 
				__LK_FILE__, __LINE__, __FUNCTION__, pcConfigPath);
			printf( "configure file(%s) has no mysql node\n", pcConfigPath );
			return -1;	

		}

		tpIPChildElm = tpIPElement->FirstChildElement("erating");
		if( tpIPChildElm != NULL )
		{
			int nWorldID;
			int nGameID = 504; 
			tpIPChildElm->Attribute("worldid", &nWorldID);
			tpIPChildElm->Attribute("gameid", &nGameID);

			vConfigure.nGameID = nGameID;
			CGateServer::GetSingleton().SetWorldID( nWorldID );
			CGateServer::GetSingleton().SetGateCode( tpIPChildElm->Attribute("gate_code"));
			CGateServer::GetSingleton().SetGatePasswd( tpIPChildElm->Attribute("gate_passwd"));
			CGateServer::GetSingleton().SetGateMac(tpIPChildElm->Attribute("gate_mac"));
		}else
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ] configure file(%s) has no gate_server node", 
				__LK_FILE__, __LINE__, __FUNCTION__, pcConfigPath);
			printf( "configure file(%s) has no gate_server node\n", pcConfigPath );
			return -1;
		}

		tpIPChildElm = tpIPElement->FirstChildElement("gamelist");
		if ( tpIPChildElm != NULL)
		{
			Tokens tokens = StrSplit(tpIPChildElm->Attribute("value"), ",");
			if ( tokens.size() == 0)
			{
				vConfigure.mGameList[0] = vConfigure.nGameID; 
			}else
			{
				Tokens::iterator iter = tokens.begin();
				for ( size_t i =0; iter != tokens.end() && i < ARRAY_CNT(vConfigure.mGameList); ++i, ++iter)
				{
					vConfigure.mGameList[i] = atoi(iter->c_str()); 
				}
			}
		}else
		{
			vConfigure.mGameList[0] = vConfigure.nGameID; 
		}
	}


	if (CGateServer::GetSingleton().GetProxyServerIP() == NULL
		|| CGateServer::GetSingleton().GetGateServerIP() == NULL)
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] configure file(%s) proxy_server(%s) or gate_server(%s) ", 
			__LK_FILE__, __LINE__, __FUNCTION__, pcConfigPath, 
			CGateServer::GetSingleton().GetProxyServerIP(), 
			CGateServer::GetSingleton().GetGateServerIP());
	}
	return 0;
}

int CGateServer::LoadGateserverConfig(const char *pcConfigPath, CConfigure& vConfigure)
{
	LOG_INFO("default", "[ %s : %d ][ %s ] load config from file %s", 
		__LK_FILE__, __LINE__, __FUNCTION__, pcConfigPath);

	TiXmlDocument tDoc;
	if (!tDoc.LoadFile(pcConfigPath))
	{       
		LOG_ERROR( "default", "[ %s : %d ][ %s ] load xml %s failed", 
			__LK_FILE__, __LINE__, __FUNCTION__, pcConfigPath);
		printf( "Cannt Find File %s\n", pcConfigPath );
		return -1;
	} 

	TiXmlElement* tpElement = NULL;
	TiXmlElement* tpChildElm = NULL;
	TiXmlElement* tpRoot = tDoc.FirstChildElement();

	// Log
	tpElement = tpRoot->FirstChildElement( "LOG" );
	if( tpElement != NULL )
	{
		for( tpChildElm = tpElement->FirstChildElement( ); tpChildElm != NULL; 
			tpChildElm = tpChildElm->NextSiblingElement( ) )
		{
			const char* tpKey = tpChildElm->Attribute("KEY");
			const char* tpFile = tpChildElm->Attribute("FILE");
			const char* tpLevel = tpChildElm->Attribute( "LEVEL" );
			const char* tpMaxNum = tpChildElm->Attribute( "MAXNUM" );

			if( ( tpKey == NULL ) || ( tpFile == NULL ) || ( tpLevel == NULL ) || ( tpMaxNum == NULL ))
			{
				continue;
			}

			//printf( "Log Congig %s, %s, %s\n", tpKey, tpFile, tpLevel );
			INIT_ROLLINGFILE_LOG( tpKey, tpFile, ( LogLevel )atoi( tpLevel ), 10*1024*1024, atoi(tpMaxNum) );
		}
	}

	// service
	tpElement = tpRoot->FirstChildElement( "service" );
	if( tpElement != NULL )
	{
		tpChildElm = tpElement->FirstChildElement("fcm_service");
		if( tpChildElm != NULL )
		{
			const char* pTurnOn = tpChildElm->Attribute("turn_on");
			const char* pReportTimer = tpChildElm->Attribute("warning_timer");
			const char* pIsFake = tpChildElm->Attribute("is_fake");

			if ( pTurnOn != NULL && strcasecmp(pTurnOn, "true") == 0)
			{
				vConfigure.bFcmServiceOn = true;
			} else
			{
				LOG_INFO("default", "fcm_service use false "); 
			}

			if ( pReportTimer != NULL )
			{
				vConfigure.nFcmWarningTimer = atoi(pReportTimer);
			}else
			{
				LOG_INFO("default", "fcm_service report timer use default value %d", vConfigure.nFcmWarningTimer); 
			}

			if ( pIsFake != NULL && strcasecmp(pIsFake, "true") == 0)
			{
				vConfigure.bFcmIsFake = true;
			} 


		}else
		{
			LOG_INFO("default", "fcm_service use default value false "); 
		}
	}

	// limit
	tpElement = tpRoot->FirstChildElement("limit");
	if( tpElement != NULL )
	{
		tpChildElm = tpElement->FirstChildElement("player_count_per_scene");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", &(vConfigure.nPlayerCountPerScene));
		}else
		{
			LOG_INFO("default", "player_count_per_scene use default value(%d)", 
				vConfigure.nPlayerCountPerScene);
		}

		tpChildElm = tpElement->FirstChildElement("login_count_once_timer");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", &(vConfigure.nLoginCountOnceTimer));
		}else
		{
			LOG_INFO("default", "player_count_per_scene use default value(%d)", 
				vConfigure.nLoginCountOnceTimer);
		}
	}

	tpElement = tpRoot->FirstChildElement( "default" );
	if( tpElement != NULL )
	{
		tpChildElm = tpElement->FirstChildElement("player_default");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute( "camp", &(vConfigure.nCampID));
			tpChildElm->Attribute( "lineid", &(vConfigure.nLineID ) );
			tpChildElm->Attribute( "mapid", &(vConfigure.nMapID ) );
		}
	}

	tpElement = tpRoot->FirstChildElement("timeout");
	if( tpElement != NULL )
	{
		tpChildElm = tpElement->FirstChildElement("ping_proxy_interval");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", &(vConfigure.nPingProxyInterval));
		}else
		{
			LOG_INFO("default", "ping_proxy_interval use default value(%d)", 
				vConfigure.nPingProxyInterval);
		}

		tpChildElm = tpElement->FirstChildElement("report2erating_interval");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", &(vConfigure.nReport2ERatingInterval));
		}else
		{
			LOG_INFO("default", "report2erating_interval use default value(%d)", 
				vConfigure.nReport2ERatingInterval);
		}

		tpChildElm = tpElement->FirstChildElement("erating_session_timeout");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", &(vConfigure.nEratingSessionTimeout));
		}else
		{
			LOG_INFO("default", "erating_session_timeout use default value(%d)", 
				vConfigure.nEratingSessionTimeout);
		}

		tpChildElm = tpElement->FirstChildElement("server_session_timeout");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", &(vConfigure.nServerSessionTimeout));
		}else
		{
			LOG_INFO("default", "server_session_timeout use default value(%d)", 
				vConfigure.nServerSessionTimeout);
		}

		tpChildElm = tpElement->FirstChildElement("gate_timer_interval");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", &(vConfigure.nGateTimerInterval));
		}else
		{
			LOG_INFO("default", "gate_timer_interval use default value(%d)", 
				vConfigure.nGateTimerInterval);
		}

		tpChildElm = tpElement->FirstChildElement("clienthanle_idle_sleep");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", &(vConfigure.nClientHandleIdleSleep));
		}else
		{
			LOG_INFO("default", "clienthanle_idle_sleep use default value(%d)", 
				vConfigure.nClientHandleIdleSleep);
		}

		tpChildElm = tpElement->FirstChildElement("game_heartbeat_timeout");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", &(vConfigure.nGameHeartBeatTimeout));
		}else
		{
			LOG_INFO("default", "game_heartbeat_timeout use default value(%d)", 
				vConfigure.nGameHeartBeatTimeout);
		}

		tpChildElm = tpElement->FirstChildElement("player_state_check_interval");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", &(vConfigure.nPlayerStateCheckInterval));
		}else
		{
			LOG_INFO("default", "player_state_check_interval use default value(%d)", 
				vConfigure.nPlayerStateCheckInterval);
		}

		tpChildElm = tpElement->FirstChildElement("player_wait_connect_timeout");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", &(vConfigure.nPlayerWaitConnectTimeout));
		}else
		{
			LOG_INFO("default", "player_wait_connect_timeout use default value(%d)", 
				vConfigure.nPlayerWaitConnectTimeout);
		}

		tpChildElm = tpElement->FirstChildElement("player_idle_ingate_timeout");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", &(vConfigure.nPlayerIdleIngateTimeout));
		}else
		{
			LOG_INFO("default", "player_idle_ingate_timeout use default value(%d)", 
				vConfigure.nPlayerIdleIngateTimeout);
		}

		tpChildElm = tpElement->FirstChildElement("player_wait_kickoff_timeout");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", (int*)&(vConfigure.nPlayerWaitKickoffTimeout));
		}else
		{
			LOG_INFO("default", "player_wait_kickoff_timeout use default value(%d)", 
				vConfigure.nPlayerWaitKickoffTimeout);
		}

		tpChildElm = tpElement->FirstChildElement("player_wait_redirect_timeout");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", (int*)&(vConfigure.nPlayerWaitRedirectTimerout));
		}else
		{
			LOG_INFO("default", "player_wait_redirect_timeout use default value(%d)", 
				vConfigure.nPlayerWaitRedirectTimerout);
		}

		tpChildElm = tpElement->FirstChildElement("player_ongame_state_timeout");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", (int*)&(vConfigure.nPlayerOnGameStateTimeout));
		}else
		{
			LOG_INFO("default", "player_ongame_state_timeout use default value(%d)", 
				vConfigure.nPlayerOnGameStateTimeout);
		}

		tpChildElm = tpElement->FirstChildElement("team_pro_syn_timeout");
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute("value", (int*)&(vConfigure.nTeamProSynTimeout));
		}else
		{
			LOG_INFO("default", "team_pro_syn_timeout use default value(%d)", 
				vConfigure.nTeamProSynTimeout);
		}
		tpChildElm = tpElement->FirstChildElement( "onlineno_timeout" );
		if( tpChildElm != NULL )
		{
			tpChildElm->Attribute( "value", ( int* )&( vConfigure.nOnLineNOTimeout ) );
		}
		else
		{
			LOG_INFO( "default", "player_ongame_state_timeout use default value(%d)",
				vConfigure.nOnLineNOTimeout );
		}

		tpChildElm = tpElement->FirstChildElement( "player_active_timeout" );
		if( tpChildElm != NULL )
		{
			int nPlayerActiveTimeout;
			tpChildElm->Attribute( "value", &nPlayerActiveTimeout );

			vConfigure.nPlayerActiveTimeout = nPlayerActiveTimeout / 1000;
		}
		else
		{
			LOG_INFO( "default", "player_active_timeout use default value(%d) seconds",
				vConfigure.nPlayerActiveTimeout );
		}
	}

	// 角色删除相关
	tpElement = tpRoot->FirstChildElement( "DeleteInfo" );
	if ( tpElement != NULL )
	{
		tpChildElm = tpElement->FirstChildElement( "TimeInfo" );
		if ( tpChildElm != NULL )
		{
			tpChildElm->Attribute( "deleteprotecttime", &vConfigure.nDeleteProtectTime );
			vConfigure.nDeleteProtectTime *= 3600;
			tpChildElm->Attribute( "deleteallowedtime", &vConfigure.nDeleteAllowedTime );
			vConfigure.nDeleteAllowedTime *= 3600;
			tpChildElm->Attribute( "deletecooltime", &vConfigure.nDeleteCoolTime );	
			vConfigure.nDeleteCoolTime *= 3600;
			tpChildElm->Attribute("protectLevel", &vConfigure.mProtectLevel );				
			tpChildElm->Attribute("defaultprotecttime", &vConfigure.mDefaultProtectTime );	
			tpChildElm->Attribute( "deletelevel", &vConfigure.mDeleteLevel );
		}
		else
		{
			LOG_INFO( "default", "DeleteInfo  use default value: deleteprotecttime=%d, deleteallowedtime=%d, deletecooltime=%d",
				vConfigure.nDeleteProtectTime, vConfigure.nDeleteAllowedTime, vConfigure.nDeleteCoolTime );
		}	
	}

	// 邮件系统删信退信相关
	tpElement = tpRoot->FirstChildElement( "MailInfo" );
	if ( tpElement != NULL )
	{
		TiXmlElement *tpDeleteElement = tpElement->FirstChildElement( "DeleteMailTime" );
		if ( tpDeleteElement != NULL )
		{
			tpDeleteElement->Attribute( "Hour", &vConfigure.nMailDeleteHour ); 
			tpDeleteElement->Attribute( "Minute", &vConfigure.nMailDeleteMinute ); 
		}

		TiXmlElement *tpBackElement = tpElement->FirstChildElement( "BackMailTime" );
		if ( tpBackElement != NULL )
		{
			TiXmlElement *tpTimeElement = tpBackElement->FirstChildElement( "TimeInfo" );
			int i = 0;
			for ( ; tpTimeElement != NULL && i < MAXSERVERNUM; tpTimeElement = tpTimeElement->NextSiblingElement( ) )
			{		
				tpTimeElement->Attribute( "TableID", &vConfigure.nMailTableID[i] );	
				tpTimeElement->Attribute( "Hour", &vConfigure.nMailBackHour[i] );
				tpTimeElement->Attribute( "Minute", &vConfigure.nMailBackMinute[i] );
				++i;
			}								
		}			
	}
	tpElement = tpRoot->FirstChildElement( "Repetion" );
	if( tpElement != NULL )
	{
		TiXmlElement *tpDeleteElement = tpElement->FirstChildElement( "CorpsBattle" );
		if( tpDeleteElement != NULL )
		{
			tpDeleteElement->Attribute( "Number", &vConfigure.mCorpsCampCount );
		}
	}

	tpElement = tpRoot->FirstChildElement( "Bourse" );
	if( tpElement != NULL )
	{
		TiXmlElement *tpDeleteElement = tpElement->FirstChildElement( "Use" );
		if( tpDeleteElement != NULL )
		{
			tpDeleteElement->Attribute( "Value", &vConfigure.mBourseUse );
		}
	}

	tpElement = tpRoot->FirstChildElement( "InternalIP" );
	if ( tpElement != NULL )
	{
		TiXmlElement *tpExceptIPInfo = tpElement->FirstChildElement( "IPInfo" );
		int tIndex = 0;
		for ( ; tpExceptIPInfo != NULL; tpExceptIPInfo = tpExceptIPInfo->NextSiblingElement() )
		{
			if ( tpExceptIPInfo->Attribute( "IPAddr" ) != NULL )
			{
				vConfigure.mInternalIPList[tIndex] = inet_addr( tpExceptIPInfo->Attribute( "IPAddr" ) );
				++tIndex;
			}						
		}
	}

	tpElement = tpRoot->FirstChildElement( "MultiClient" );
	if ( tpElement != NULL )
	{
		// 取出单IP默认允许的最大连接数
		if ( tpElement->Attribute( "default" ) != NULL )
		{
			vConfigure.mDefaultClientPerIP = atoi(tpElement->Attribute("default"));
		}

		if ( tpElement->Attribute( "permac" ) != NULL )
		{
			vConfigure.mDefaultClientOnIPPerMac = atoi( tpElement->Attribute("permac"));
		}

		if ( tpElement->Attribute( "active" ) != NULL )
		{
			vConfigure.mIsCheckMultiClient = atoi(tpElement->Attribute("active"));
		}

		if ( tpElement->Attribute( "timer" ) != NULL )
		{
			vConfigure.mCheckMultiClientInterval = atoi(tpElement->Attribute("timer"));
		}

		TiXmlElement *tpExceptIPInfo = tpElement->FirstChildElement( "IPInfo" );
		for (; tpExceptIPInfo != NULL; tpExceptIPInfo = tpExceptIPInfo->NextSiblingElement() )
		{
			if ( tpExceptIPInfo->Attribute( "IPAddr" ) != NULL &&  tpExceptIPInfo->Attribute( "Client" ) != NULL )
			{
				std::pair<CConfigure::MULTICLIENT::iterator, bool > ret = vConfigure.mMultiClientIP.insert(
					CConfigure::MULTICLIENT::value_type(
					inet_addr(tpExceptIPInfo->Attribute( "IPAddr")), 
					atoi(tpExceptIPInfo->Attribute("Client"))
					)
					);

				if ( !ret.second )
				{
					LOG_ERROR("default", "The ip %s is not inert into the mutli client list.", 
						tpExceptIPInfo->Attribute( "IPAddr" ));
				}else
				{
					LOG_DEBUG("default", "The ip %s is inert into the mutli client list for (%s).", 
						tpExceptIPInfo->Attribute( "IPAddr" ), tpExceptIPInfo->Attribute("Client"));

				}
			}
		}
	}
	return 0;
}

void CGateServer::LoadDirtyWordConfigCallback(const char *pcConfigPath)
{
	// 读取脏词库
	CDirtyWord::GetSingletonPtr( )->OI_Dirty_start();
}

void CGateServer::LoadRoleValidConfigCallback(const char* pcConfigPath)
{
	CConfigure& vConfigure = CGateLogic::GetSingleton().mConfig;
	CGateLogic::GetSingleton().ReadRoleValidQuestionConfig(vConfigure);
}

void CGateServer::LoadTracelistCallback(const char *pcConfigPath)
{
	// 初始化需要消息追踪的玩家的列表
	CGateLogic::GetSingleton().GetTracePlayerManager().InitializeList();

	// 发送需要追踪的玩家列表  
	CGateLogic::GetSingleton().SendTracePlayerNotice();	

}

void CGateServer::LoadTemplateCallback(const char *pcConfigPath)
{
	// 重加载模板
	CDataStatic::Initialize();
}

void CGateServer::LoadIPConfigCallback(const char *pcConfigPath)
{
	CGateServer::GetSingleton().LoadIPConfig(pcConfigPath, CGateLogic::GetSingleton().mConfig);
}

void CGateServer::LoadGateserverConfigCallback(const char *pcConfigPath)
{
	CGateServer::GetSingleton().LoadGateserverConfig(pcConfigPath, CGateLogic::GetSingleton().mConfig);
	
	// 刷新配置数据
	CGateLogic::GetSingleton().RefreshTimerConfig();

	// Gateserver加载配置数据
	CGateServer::GetSingleton().OnReloadCfg();

	// 防沉迷模块刷新
	CServiceGateFcm::GetSingleton().ReloadCfg();
}

// ***************************************************************
// --- CGateLogic 
// ***************************************************************
//
template<> CGateLogic* CSingleton< CGateLogic >::spSingleton = NULL;
CSharedMem* CGateLogic::mShmPtr = NULL; 

CGateLogic::CGateLogic()
{
	mLaunchFlag = EM_LAUNCH_LOADING;
	mLaunchTimer = CWTimer(1000);
}

CGateLogic::~CGateLogic( )
{
}

// ***************************************************************
//  Function:		CGateLoic::CheckRunFlags
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::CheckRunFlags()
{
	if ( IsRunFlagSet( EFLG_CTRL_QUIT ) )
	{
		LOG_NOTICE( "default", "xyj-gateserver exit ! " );	

		// do something befor quit
		CPlayerManager::GetSingleton().KickPlayerInGlobalScene();
		CModuleManager::GetSingleton().ExitServer();

		CGateServer::GetSingleton().ExitServer();

		exit(0);
	}

	if ( IsRunFlagSet( EFLG_CTRL_RELOAD ) )
	{
		ClearRunFlag( );
		LOG_NOTICE( "default", "xyj-gateserver reload appointed config ... " );
		printf("xyj-gateserver reload appointed config ...\n");
		int nRetCode = CGateServer::GetSingleton().ReLoadAppointedConfig();
		if (nRetCode == 0)
		{
			LOG_NOTICE( "default", "xyj-gateserver reload appointed  config complete! ... " );
			printf("xyj-gateserver reload appointed  config complete! ...\n");
		}
		else
		{
			LOG_NOTICE( "default", "xyj-gateserver reload appointed  config fail! ... " );
			printf("xyj-gateserver reload appointed  config fail! ...\n");

			LOG_NOTICE("default", "xyj-gateserver start to reload all configure file...");
			printf( "xyj-gateserver start to reload all configure file...\n" );

			// 重加载模板
			CDataStatic::Initialize();

			CConfigure configure;
			if ( ReadCfg(configure) == 0) //success
			{
				mConfig = configure;


				mWaitTimer = CWTimer(mConfig.nGateTimerInterval);
				mStateTimer = CWTimer(mConfig.nPlayerStateCheckInterval);
				mPerfTimer = CWTimer(PERF_TIMER);

#ifdef USING_ERATING
				mTimerOfReport2ERating = CWTimer(mConfig.nReport2ERatingInterval);
#endif

				mTimerOfCheckMultiClientPerIP = CWTimer(mConfig.mCheckMultiClientInterval);

				CModuleManager::GetSingleton().OnReloadCfg(); 
				CGateServer::GetSingleton().OnReloadCfg();
				CGateLogic::GetSingleton().OnReloadConfig();
				CCoreService::GetSingleton().OnReloadConfig();

				LOG_NOTICE("default", "reload all configure file complete!");
				printf( "reload all configure file complete!\n" );
			}else
			{
				LOG_ERROR("default", "reload all configure file error, please recheck it and reload it.");
				printf( "ERROR: reload all configure file error, please recheck it and reload it.\n" );
			}
		}
	}
	else if ( IsRunFlagSet( EFLG_CTRL_COREDUMP ) )
	{
		ClearRunFlag( );

		LOG_NOTICE( "default", "coredump complte ! ..." );

		WRITE_COREDUMP("core.gateserver");
	}
	else if ( IsRunFlagSet( EFLG_CTRL_REFRESHRANK ) )
	{
		ClearRunFlag( );

		LOG_NOTICE( "default", "refresh rank complte ! ..." );

		CRankModule::GetSingletonPtr()->DailyUpdate( );
		//CRankModule::GetSingletonPtr()->WeeklyUpdate( );
	}
	else if ( IsRunFlagSet( EFLG_CTRL_PROCESSEXPIREDMAIL ) )
	{
		ClearRunFlag( );
		LOG_NOTICE( "default", "excute mailsysaction complte !" );
		CGateServer::GetSingleton().ExcuteMailSysAction( );	
	}
	else if( IsRunFlagSet( EFLG_CTRL_SAFESHUTDOWN ) )
	{
		CPlayerManager::GetSingleton().KickPlayerInGlobalScene();
		ClearRunFlag( );
		LOG_NOTICE( "default", "now safe shutdown server group!" );
		SendShutdownNotify(); 	

		mLaunchFlag = EM_LAUNCH_SHUTDOWNING;
	}
}

// ***************************************************************
//  Function:		CGateLoic::SetRunFlag
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::SetRunFlag( int vFlag )
{
	mRunFlag = vFlag;
}

// ***************************************************************
//  Function:		CGateLoic::ClearRunFlag
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::ClearRunFlag()
{
	mRunFlag = EFLG_CTRL_NO;
}

// ***************************************************************
//  Function:		CGateLoic::IsRunFlagSet
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
bool CGateLogic::IsRunFlagSet( int vFlag )
{
	return mRunFlag == vFlag;
}

// ***************************************************************
//  Function:		CGateLoic::Launch
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::Launch()
{
	// 载入其它数据服务模块
	CModuleManager::GetSingleton().RegisterModule(new CServiceRoleIDName);
	CModuleManager::GetSingleton().RegisterModule(new CServiceGateTeam);
	CModuleManager::GetSingleton().RegisterModule(new CRepetionModule );
	CModuleManager::GetSingleton().RegisterModule(new CTaskModule);
	CModuleManager::GetSingleton().RegisterModule(new CServiceGmCmd);
	CModuleManager::GetSingleton().RegisterModule(new CFriendModule);
	CModuleManager::GetSingleton().RegisterModule(new CServiceGateMail);
	CModuleManager::GetSingleton().RegisterModule(new CServiceGateFcm);
	CModuleManager::GetSingleton().RegisterModule(new CServiceActivity);
	CModuleManager::GetSingleton().RegisterModule(new CIbStoreModule);
	CModuleManager::GetSingleton().RegisterModule(new CRankModule);
	CModuleManager::GetSingleton().RegisterModule(new CServiceFamily );
	CModuleManager::GetSingleton().RegisterModule(new CCorpservice );
	CModuleManager::GetSingleton().RegisterModule(new CCoreService);
	CModuleManager::GetSingleton().RegisterModule(new CChatmodule);
	CModuleManager::GetSingleton().RegisterModule(new CJudgeModule);
	CModuleManager::GetSingleton().RegisterModule(new CWorldModule);
	CModuleManager::GetSingleton().RegisterModule(new CBourseModule);
    CModuleManager::GetSingleton().RegisterModule(new CActivityYQS);
	CModuleManager::GetSingleton().RegisterModule(new CGlobalModule);

	// 启动服务器
	CGateServer::GetSingleton().OnLaunch();

	// 启动其它服务模块
	CModuleManager::GetSingleton().LaunchServer();		

}

// ***************************************************************
//  Function:		CGateLoic::new
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void* CGateLogic::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

// ***************************************************************
//  Function:		CGateLoic::delete
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void  CGateLogic::operator delete( void* pointer )
{

}

// ***************************************************************
//  Function:		CGateLoic::Initialize
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
int CGateLogic::Initialize(int vInitFlag)
{
	//网关
	CGateServer::mShmPtr = mShmPtr;
	CGateServer* tpServer= new CGateServer;

	//网关需要的对象池
	CGateObjManager::mShmPtr = mShmPtr;
	CGateObjManager* tpGateObjMng = new CGateObjManager;


	//消息队列
	CMessageQueue::mShmPtr = mShmPtr;
	CMessageQueue* tpMessageQueue = new CMessageQueue;

	// 没有成员变量，无需new在共享内存上
	CMessageDispatcher* tpMessageDispatcher = new CMessageDispatcher;

	// 消息工厂
	CMessageFactory* tpMessageFactory = new CMessageFactory;

	// 其它模块
	CDataModule::mShmPtr = mShmPtr;
	CModuleManager* tpModuleManager = new CModuleManager;

	// 脏词
	CDirtyWord* tpDirty = new CDirtyWord();


	if (tpServer == NULL
	|| tpGateObjMng == NULL || tpMessageQueue == NULL 
	|| tpMessageDispatcher == NULL || tpMessageFactory == NULL
	|| tpModuleManager == NULL || tpDirty == NULL)
	{
		LOG_FATAL("default", "[ %s : %d ][ %s ] Malloc Object error,pls check Mem",
			__LK_FILE__, __LINE__, __FUNCTION__ );

		printf( "Malloc Object error,pls check Mem\n" );

		exit(0);
	}

	if (ReadCfg(mConfig) != 0)
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] ReadCfg failure, exit parograme", __LK_FILE__, __LINE__, __FUNCTION__);
		printf(  "ReadCfg failure, exit parograme\n" );
		exit(0);
	}
	CDataStatic::Initialize();

	mLastTickCount = LINEKONG::GetTickCount();
	mTimerOfKeepAlive = CWTimer(mConfig.nPingProxyInterval);

	CGateServer::GetSingleton().UpdProxyActiveTmStmp();
	mTimerOfLoginKeepAlive = CWTimer( 10000 );

	mWaitTimer = CWTimer(mConfig.nGateTimerInterval);
	mStateTimer = CWTimer(mConfig.nPlayerStateCheckInterval);
	mPerfTimer = CWTimer(PERF_TIMER);
	mOnLineNOTimer = CWTimer( mConfig.nOnLineNOTimeout );
	mOnLinePlayerStatPrintTimer = CWTimer( 60000 );

	#ifdef USING_ERATING
	mTimerOfReport2ERating = CWTimer(mConfig.nReport2ERatingInterval);
	#endif

	mTimerOfCheckMultiClientPerIP = CWTimer(mConfig.mCheckMultiClientInterval);

	mRunFlag = EFLG_CTRL_NO;

	// 注册配置加载的回调函数

	const char* pcConfigPath = "../config/data/template/template_server.dat";
	CGateServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CGateServer::LoadTemplateCallback);

	pcConfigPath = "../config/ip.xml";
	CGateServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CGateServer::LoadIPConfigCallback);

	pcConfigPath = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"gate_config/gateserver.xml");
	CGateServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CGateServer::LoadGateserverConfigCallback);

	pcConfigPath = "../config/data/dirtyword";
	CGateServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, ".txt", CGateServer::LoadDirtyWordConfigCallback);

	pcConfigPath = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"question/config.xml");
	CGateServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CGateServer::LoadRoleValidConfigCallback);

	pcConfigPath = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"gate_config/tracelist.xml");
	CGateServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CGateServer::LoadTracelistCallback);


	return 0;
}

// ***************************************************************
//  Function:		CGateLoic::Resume
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
int CGateLogic::Resume()
{
	return 0;
}

// ***************************************************************
//  Function:		CGateLoigc::Run
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::Run(const int nDaemon )
{
	while(1)
	{
		// 检查运行标志
		CheckRunFlags();  

		// 检查时钟
		CheckTimer();

		// 接收服务器消息到消息队列(单次循环最大接收N个)
		CGateServer::GetSingleton().RecvServerMsg(3000);

		// 接收客户端消息到消息队列(单次循环最大接收N个)
		CGateServer::GetSingleton().RecvClientMsg(3000);

		// 处理消息队列中的消息
		CMessageDispatcher::GetSingletonPtr()->DispachMsg(); 

	}
}

// ***************************************************************
//  Function:		CheckTimer
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::CheckTimer()
{
	uint64_t tNowCount = LINEKONG::GetTickCount();

	if ( tNowCount < mLastTickCount )
	{
		mLastTickCount = tNowCount;
		return;
	}

	uint64_t tInterval = tNowCount - mLastTickCount;
	if ( tInterval < 100 )
	{
		return ;
	}

	mLastTickCount = tNowCount ;

	OnTimer( tNowCount, (unsigned int) tInterval );
}

// ***************************************************************
//  Function:		CGateLogic::CountSize
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
unsigned int CGateLogic::CountSize()
{
	unsigned int tSize = 0;
	unsigned int tTmpSize = 0;

	tTmpSize = sizeof( CGateLogic );	
	tSize += tTmpSize;
	LOG_INFO( "default", "CGateLogic CountSize: %d", tTmpSize );

	tTmpSize = CClientHandle::CountSize();
    tSize += tTmpSize;
	LOG_INFO( "default", "CClientHandle CountSize: %d", tTmpSize );

	tTmpSize = CGateServer::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CGateServer CountSize: %d", tTmpSize );

	tTmpSize = CGateObjManager::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CGateObjManager CountSize: %d", tTmpSize );

	tTmpSize = CMessageQueue::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CMessageQueue CountSize: %d", tTmpSize );

	tTmpSize = CServiceRoleIDName::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CServiceRoleIDName CountSize: %d", tTmpSize );

	tTmpSize = CServiceGateTeam::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CServiceGateTeam CountSize: %d", tTmpSize );

	tTmpSize = CRepetionModule::CountSize( );
	tSize += tTmpSize;
	LOG_INFO( "default", "CRepetionModule CountSize: %d", tTmpSize );

	tTmpSize = CServiceGmCmd::CountSize( );
	tSize += tTmpSize;
	LOG_INFO( "default", "CServiceGmCmd CountSize: %d", tTmpSize );

	tTmpSize = CFriendModule::CountSize( );
	tSize += tTmpSize;
	LOG_INFO( "default", "CFriendModule CountSize: %d", tTmpSize );

	tTmpSize = CTaskModule::CountSize( );
	tSize += tTmpSize;
	LOG_INFO( "default", "CTaskModule CountSize: %d", tTmpSize );

	tTmpSize = CServiceGateMail::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CServiceGateMail CountSize: %d", tTmpSize );

	tTmpSize = CServiceGateFcm::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CServiceGateFcm CountSize: %d", tTmpSize );

	tTmpSize = CServiceActivity::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CServiceActivity CountSize: %d", tTmpSize );

	tTmpSize = CIbStoreModule::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CIbStoreModule CountSize: %d", tTmpSize );

	tTmpSize = CRankModule::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CRankModule CountSize: %d", tTmpSize );

	tTmpSize = CServiceFamily::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CServiceFamily CountSize: %d", tTmpSize );

	tTmpSize = CCorpservice::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CCorpservice CountSize: %d", tTmpSize );

	tTmpSize = CCoreService::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CCoreService CountSize: %d", tTmpSize );

	tTmpSize = CChatmodule::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CChatmodule CountSize: %d", tTmpSize );	

	tTmpSize = CJudgeModule::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CJudgeModule CountSize: %d", tTmpSize );

	tTmpSize = CWorldModule::CountSize();
	tSize += tTmpSize; 
	LOG_INFO( "default", "CWorldModule CountSize: %d", tTmpSize );

	tTmpSize = CBourseModule::CountSize();
	tSize += tTmpSize; 
	LOG_INFO( "default", "CBourseModule CountSize: %d", tTmpSize );

    tTmpSize = CActivityYQS::CountSize();
    tSize += tTmpSize;
    LOG_INFO( "default", "CActivityYQS CountSize: %d", tTmpSize );
	
	tTmpSize = CGlobalModule::CountSize();
	tSize += tTmpSize;
	LOG_INFO( "default", "CGlobalModule CountSize: %d", tTmpSize );


	return tSize;
}

// ***************************************************************
//  Function:		CGateLogic::OnProxyMessage
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************

void CGateLogic::OnProxyMessage( CMessage* tpMessage )
{
	LK_ASSERT( tpMessage != NULL, return );

	CMessageHead* tpHead = tpMessage->mutable_msghead();
	unsigned int nMessageID = (unsigned int) tpHead->messageid();
	int vServerID = tpHead->srcid();

	/*
	if (vServerID <= 0 || vServerID > SCENE_AT_GATE_CAP)
	{
		LOG_FATAL("default", "ServerID(%d)超过了游戏允许的最多Server数量(%d)",
				vServerID, SCENE_AT_GATE_CAP);
		return;
	}
	*/
	
	// 聊天消息放到军团模块进行处理
	//if ( tpMessage->mutable_msghead()->messageid() == ID_S2C_CHATSENDCALLBACK )
	//{
	//	CCorpservice::GetSingleton().OnChatMsg( vServerID,  tpMessage );
	//	pbmsg_delmsg(tpMessage);
	//	return;
	//}	
	
	// 判断是否是发往国家的消息
	if ( tpMessage->mutable_msghead()->dstfe() == FE_COUNTRY )
	{
		SendCountryMessage( tpMessage, tpMessage->mutable_msghead()->dstid() );
		pbmsg_delmsg(tpMessage);
		return;
	}
	
	// 判断是否发往家族的消息
	if ( tpMessage->mutable_msghead()->dstfe() == FE_FAMILY )
	{
		CServiceFamily::GetSingleton().SendFamilyMsgByID( tpMessage->mutable_msghead()->dstid(), tpMessage );
		pbmsg_delmsg(tpMessage);
		return;
	}
	
	// 判断是否是否是发往军团的消息
	if ( tpMessage->mutable_msghead()->dstfe() == FE_CORPS )
	{
		CCorpservice::GetSingleton().SendCorpsMsgByID( tpMessage->mutable_msghead()->dstid(), tpMessage );
		pbmsg_delmsg(tpMessage);
		return;		
	}	   	

	// 是否发往其他网关的消息
	if ( tpMessage->mutable_msghead()->dstfe() == FE_OTHERGATE )
	{
		CGateServer::GetSingleton().Send2OtherGate( tpMessage, tpMessage->mutable_msghead()->dstid() );
		pbmsg_delmsg(tpMessage);
		return;		
	}	   	

	// 是否来自全局服务器的消息
	if ( tpMessage->mutable_msghead()->srcfe() == FE_DUNGEONSERVER )
	{
		CGlobalModule::GetSingleton().OnMessage( tpMessage->mutable_msghead()->srcid(), tpMessage );
		pbmsg_delmsg(tpMessage);
		return;
	}

	// 是否是路由消息
	if ((nMessageID & CMessageRouter::MSGROUTER) == CMessageRouter::MSGROUTER)
	{
		OnRouterMessage(vServerID, tpMessage);
		pbmsg_delmsg(tpMessage);
		return;
	}

	unsigned short nModuleClass = REGION_OF_MESSAGE( nMessageID );
	switch ( nModuleClass )
	{
		// 全局控制信息
		case MESSAGE_GLOBAL:
		case MESSAGE_PROPERTY:
		{
			OnServerControlMessage( vServerID, tpMessage );
			break;
		}
        case MESSAGE_ACTIVITY:
        {
            CActivityYQS::GetSingleton().OnMessage( vServerID, tpMessage );
            break;
        }

		case MESSAGE_CORE:
		{
			CCoreService::GetSingleton().OnServerMessage(vServerID, tpMessage);
			break;
		}

		case MESSAGE_TEAM: // 组队
		{
			CServiceGateTeam::GetSingleton().OnMessage( vServerID, tpMessage); 
			break;
		}

		case MESSAGE_GM: // GM指令
		{
			CServiceGmCmd::GetSingleton().OnMessage(vServerID, tpMessage);
			break;
		}

		case MESSAGE_FRIEND: // 好友消息
		{
			CFriendModule::GetSingleton().OnMessage( vServerID, tpMessage); 
			break;
		}

		case MESSAGE_MAIL: // Mail 消息
		{
			CServiceGateMail::GetSingleton().OnMessage( vServerID, tpMessage);
			break;
		}
		
		case MESSAGE_FAMILY: // 家族消息
		{
			CServiceFamily::GetSingleton().OnSeverMessage( vServerID, tpMessage );
			break;
		}
		case MESSAGE_CORPS: // 军团消息
		{
			CCorpservice::GetSingleton().OnSeverMessage( vServerID, tpMessage );
			break;
		}
		
		case MESSAGE_RANK:
		{
			CRankModule::GetSingleton().OnMessage( vServerID, tpMessage );
			break;
		}
	 
		case MESSAGE_REPETION: // 副本消息
		{
			CRepetionModule::GetSingleton().OnMessage( vServerID, tpMessage );
			break;
		}

		case MESSAGE_IB:	// 商城消息
		{
			CIbStoreModule::GetSingleton().OnMessage( vServerID, tpMessage );
			break;
		}
		case MESSAGE_CHAT:
		{
			CChatmodule::GetSingleton().OnSeverMessage( vServerID, tpMessage );
			break;
		}

		case MESSAGE_JUDGE:
		{
			CJudgeModule::GetSingleton().OnMessage( vServerID, tpMessage );
			break;
		}

		case MESSAGE_COUNTRY:
		{
			CWorldModule::GetSingleton().OnSeverMessage( vServerID, tpMessage );
			break;
		}
		case MESSAGE_BOURSE:
		{
			CBourseModule::GetSingletonPtr( )->OnServerMessage( vServerID, tpMessage );
		}
		break;

		case MESSAGE_GLOBALDUNGEON:
		{
			CGlobalModule::GetSingleton().OnMessage( vServerID, tpMessage );
			break;
		}
		case MESSAGE_TASK:
		{
			CTaskModule::GetSingleton().OnMessage(vServerID, tpMessage );
			break;
		}
		default:
		{
			LOG_NOTICE("default", "No Action fo MessageID(%d)",  tpMessage->msghead().messageid());
			break;
		}   
	}

	pbmsg_delmsg(tpMessage);
}

// ***************************************************************
//  Function:		CGateLogic::OnRouterMessage 
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::OnRouterMessage(int vServerID, CMessage* tpMessage)
{
	if (tpMessage == NULL) return;

	CGateServer * tpLogic = CGateServer::GetSingletonPtr( );
	//CMessageHead* tpHead = tpMessage->mutable_msghead();

	Message* pUnknownMessage = (Message*) tpMessage->msgpara();

	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessage->GetDescriptor();
	const ::google::protobuf::Reflection*  pReflection = pUnknownMessage->GetReflection();

	if (pDescriptor == NULL || pReflection == NULL) return; 

	const ::google::protobuf::FieldDescriptor*  pFieldDescriptor = pDescriptor->FindFieldByNumber(1);

	if (pFieldDescriptor == NULL) return;

	assert(pFieldDescriptor->type() == ::google::protobuf::FieldDescriptor::TYPE_MESSAGE);

	const Message& tRouter = pReflection->GetMessage(*pUnknownMessage, pFieldDescriptor); 		

	CMessageRouter* tpRouter = (CMessageRouter*) &tRouter;	
	

	if ( tpRouter->routertype() == CMessageRouter::ROUTER_NAME )
	{
		//CMessageRouterByName* tpRouterByName = (CMessageRouterByName*) tpRouter;
		CSceneServer* tpSrcScene = tpLogic->GetServerByID( vServerID );
		if ( tpSrcScene == NULL )
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ]Can not find SceneServer by ID(%d)", 
					__LK_FILE__, __LINE__, __FUNCTION__, vServerID);

			return;
		}

		// 如果目标名称为空, 发送给所有场景
		if ( tpRouter->has_desname() == false || tpRouter->desname().length() == 0)
		{
			tpLogic->Send2Scene( (CSceneServer*) NULL, tpMessage );
			return;
		}
		else
		{
			// 消息发送给具体客户端
			CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpRouter->desname().c_str() );

			// 如果目标找不到了，将消息打回
			if ( tpPlayer == NULL )
			{
				tpRouter->set_sendback(1);
				tpRouter->set_routerreason(CMessageRouter::REASON_OFFLINE);
				tpLogic->Send2Scene( tpSrcScene, tpMessage );
				return;
			}
			
			// 如果目标找到了，设置目标的CharID
			tpRouter->set_descharid( tpPlayer->mCharID );

			// 如果目标找到了，但是不在场景中
			if ( tpPlayer->mLineID == -1 && tpPlayer->mMapID == -1 )
			{
				tpRouter->set_sendback(1);
				tpRouter->set_routerreason(CMessageRouter::REASON_REDIRECT);

				tpLogic->Send2Scene( tpSrcScene, tpMessage );
				return;
			}

			// 得到目标玩家所在服务器
			CSceneServer* tpDesScene = tpLogic->GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
			if ( tpDesScene == NULL )
			{
				tpRouter->set_sendback(1);
				tpRouter->set_routerreason(CMessageRouter::REASON_REDIRECT);

				tpLogic->Send2Scene( tpSrcScene, tpMessage );
				return;
			}

			tpRouter->set_sendback(0);
			tpRouter->set_routerreason(CMessageRouter::REASON_SUCCESS);

			tpLogic->Send2Scene( tpDesScene, tpMessage );

			tpRouter->set_sendback(1);
			tpRouter->set_routerreason(CMessageRouter::REASON_SUCCESS);

			tpLogic->Send2Scene( tpSrcScene, tpMessage );
		}
	}

	if ( tpRouter->routertype() == CMessageRouter::ROUTER_CHAR )
	{
		//CMessageRouterByCharID* tpRouterByCharID = (CMessageRouterByCharID*) tpRouter;
		CSceneServer* tpSrcScene = tpLogic->GetServerByID( vServerID );
		if ( tpSrcScene == NULL )
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ]Can not find SceneServer by ID(%d)", 
					__LK_FILE__, __LINE__, __FUNCTION__, vServerID);
			return;
		}

		// 如果目标账号等于0, 发送给所有场景
		if ( tpRouter->descharid() == 0 )
		{
			tpLogic->Send2Scene( (CSceneServer*) NULL, tpMessage );
			return;
		}

		// 如果该消息发送给具体客户端
		if ( tpRouter->descharid() != 0 )
		{
			// 如果目标找不到了，将消息打回
			CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpRouter->descharid() );
			if ( tpPlayer == NULL )
			{
				tpRouter->set_sendback(1);
				tpRouter->set_routerreason(CMessageRouter::REASON_OFFLINE);

				tpLogic->Send2Scene( tpSrcScene, tpMessage );
				return;
			}
			
			// 如果目标找到了，设置目标的CharID
			tpRouter->set_descharid( tpPlayer->mCharID );

			// 如果目标找到了，但是不再场景中
			if ( tpPlayer->mLineID == -1 && tpPlayer->mMapID == -1 )
			{
				tpRouter->set_sendback(1);
				tpRouter->set_routerreason(CMessageRouter::REASON_REDIRECT);

				tpLogic->Send2Scene( tpSrcScene, tpMessage );
				return;
			}

			// 得到目标玩家所在服务器
			CSceneServer* tpDesScene = tpLogic->GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
			if ( tpDesScene == NULL )
			{
				tpRouter->set_sendback(1);
				tpRouter->set_routerreason(CMessageRouter::REASON_REDIRECT);

				tpLogic->Send2Scene( tpSrcScene, tpMessage );
				return;
			}

			tpRouter->set_sendback(0);
			tpRouter->set_routerreason(CMessageRouter::REASON_SUCCESS);

			tpLogic->Send2Scene( tpDesScene, tpMessage);

			tpRouter->set_sendback(1);
			tpRouter->set_routerreason(CMessageRouter::REASON_SUCCESS);

			tpLogic->Send2Scene( tpSrcScene, tpMessage);
		}
		return;
	}
}

// ***************************************************************
//  Function:		CGateLogic::OnAGIPSessionTimeout
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::OnAGIPSessionTimeout(int nSessionClass, int nParam1, int nParam2)
{
	switch(nSessionClass)
	{
		case em_agipsession_obtainroles:
		{
			CCoreService::GetSingleton().OnAGIPMsgRoleListTimeOut(nParam1, nParam2);
			break;
		}

		case em_agipsession_createrole:
		{
			CCoreService::GetSingleton().OnAGIPMsgCreateRoleTimeOut(nParam1, nParam2);
			break;
		}

		case em_agipsession_deleterole:
		{
			CCoreService::GetSingleton().OnAGIPMsgDeleteRoleTimeOut(nParam1, nParam2);
			break;
		}

		case em_agipsession_gwdatareport:
		{
			LOG_ERROR("erating", "OnAGIPMsgGWDataReportResponse failure Because of ERating Timeout");
			break;
		}

		case em_agipsession_userlogout:
		{
			CCoreService::GetSingleton().OnAGIPMsgUserLogoutTimeOut(nParam1, nParam2);
			break;
		}

		case em_agipsession_entergame:
		{
			CCoreService::GetSingleton().OnAGIPMsgEnterGameTimeOut(nParam1, nParam2);
			break;
		}

		/*
		case em_status_ingame_v4erating_activityitem:
		{
			CServiceActivity::GetSingleton().OnAGIPMsgUserItemListTimeout(nParam1, nParam2);
			break;
		}

		case em_status_ingame_v4erating_itemminus:
		{
			CServiceActivity::GetSingleton().OnAGIPMsgUserItemMinusTimeout(nParam1, nParam2);
			break;
		}
		*/


		/*
		case em_agipsession_verifypasswd:
		{
			CCoreService::GetSingleton().OnAGIPMsgPasswdVerifyTimeOut(nParam1, nParam2);
			break;
		}
		*/
	
		case em_agipsession_creategroup:
		{
			switch( nParam2)
			{
				case em_family_group:
				{
					CServiceFamily::GetSingleton().OnAGIPTimeoutCreateGroup(nParam1, nParam2);
					break;
				}

				case em_corps_group:
				{
					CCorpservice::GetSingleton().OnAGIPTimeoutCreateCorps(nParam1, nParam2);
					break;
				}

				case em_union_group:
				{
					CCorpservice::GetSingleton().OnAGIPTimeoutCreateUnion(nParam1, nParam2);
					break;
				}

				default:
				{
					break;
				}
			}
			
			break;
		}

		default:
		{
			LOG_ERROR("erating", "[ %s : %d ][ %s ] There is no action for the erating session Class (%d)", 
					__LK_FILE__, __LINE__, __FUNCTION__, nSessionClass);
			break;
		}
	}
}

// ***************************************************************
//  Function:		CGateLogic::FireAGIPSession
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::FireAGIPSession(int nSessionClass, SysProtocol* pproto, 
		int nParam1, int nParam2, int nParam3, int nParam4, const char* szParam5)
{
	LK_ASSERT(pproto != NULL, return);

	switch(nSessionClass)
	{
		case em_agipsession_obtainroles:
		{
			CCoreService::GetSingleton().OnAGIPMsgRoleListResponse(pproto, nParam1, nParam2);
			break;
		}

		case em_agipsession_createrole:
		{
			CCoreService::GetSingleton().OnAGIPMsgCreateRoleResponse(pproto, nParam1, nParam2);
			break;
		}

		case em_agipsession_deleterole:
		{
			CCoreService::GetSingleton().OnAGIPMsgDeleteRoleResponse(pproto, nParam1, nParam2);
			break;
		}

		case em_agipsession_gwdatareport:
		{
			OnAGIPMsgGWDataReportResponse(pproto, nParam1, nParam2);
			break;
		}

		case em_agipsession_userlogout:
		{
			CCoreService::GetSingleton().OnAGIPMsgUserLogoutResponse(pproto, nParam1, nParam2);
			break;
		}

		case em_agipsession_entergame:
		{
			CCoreService::GetSingleton().OnAGIPMsgEnterGameResponse(pproto, nParam1, nParam2);
			break;
		}

		/*
		case em_agipsession_verifypasswd:
		{
			CCoreService::GetSingleton().OnAGIPMsgPasswdVerifyResponse(pproto, nParam1, nParam2);
			break;
		}
		*/

		case em_agipsession_useritemlist:
		{
			CServiceActivity::GetSingleton().OnAGIPSessionUserItemList(pproto, nParam1, nParam2, nParam3, nParam4);
			break;
		}

		case em_agipsession_useritemminus:
		{
			CServiceActivity::GetSingleton().OnAGIPSessionUserItemMinus(pproto, nParam1, nParam2, nParam3, nParam4, szParam5);
			break;
		}

		case em_agipsession_creategroup:
		{
			switch( nParam2)
			{
				case em_family_group:
				{
					CServiceFamily::GetSingleton().OnAGIPSessionCreateGroup(pproto, nParam1, nParam2);
					break;
				}

				case em_corps_group:
				{
					CCorpservice::GetSingleton().OnAGIPSessionCreateCorps(pproto, nParam1, nParam2);
					break;
				}

				case em_union_group:
				{
					CCorpservice::GetSingleton().OnAGIPSessionCreateUnion(pproto, nParam1, nParam2);
					break;
				}

				default:
				{
					break;
				}
			}

			break;
		}

		case em_agipsession_queryyuanbao:
		{
			CIbStoreModule::GetSingleton().OnAGIPSessionQueryYuanbao(pproto, nParam1, nParam2);	
			break;
		}

		case em_agipsession_drawyuanbao:
		{
			CIbStoreModule::GetSingleton().OnAGIPSessionDrawYuanbao(pproto, nParam1, nParam2);	
			break;
		}

		default:
		{
			LOG_ERROR("erating", "[ %s : %d ][ %s ] There is no action for the Session Class (%d)", 
					__LK_FILE__, __LINE__, __FUNCTION__, nSessionClass);
			break;
		}
	}
}

// ***************************************************************
//  Function:		CGateLogic::OnMessageGameHeartBeatNotice
//  Description:	处理游戏服务器过来的心跳消息
//	Input:			vServerID - 服务器ID pMessage - 消息指针
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::OnMessageGameHeartBeatNotice( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL && pMessage->has_msgpara() != false , return );
	CMessageGameHeartBeatNotice* pReqMsg = (CMessageGameHeartBeatNotice*) pMessage->msgpara();
	LK_ASSERT( pReqMsg != NULL && nServerID == (int) pReqMsg->serverid(), return); 


	// 更新服务器的人数
	// CGateServer::GetSingleton().UpdateSceneRptPlayerNumber(nServerID, pReqMsg->charids_size());

	CSceneServer* tpServer = CGateServer::GetSingleton().GetServerByID( nServerID );
	if ( tpServer == NULL )
	{
		LOG_ERROR("default",  "[ %s : %d ][ %s ] The sceneserver %d is NOT be registered", 
				__LK_FILE__, __LINE__, __FUNCTION__, nServerID );
		return;
	}

	// 重置时钟
	tpServer->mActiveTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nGameHeartBeatTimeout);

	// 重置人数
	tpServer->mRptPlayerNumber.clear();

	time_t tNow  = time(NULL);
	for( int i = 0; i < pReqMsg->charids_size(); i++ )
	{
		int nRoleID = pReqMsg->charids(i);
		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID ) ;
		if (tpPlayer )
		{
			tpPlayer->m_tLastActiveTime = tNow;
			int nGameID = tpPlayer->GetGameID();

			// 赋值
			CSceneServer::GAMEIDPLAYER::iterator iter = tpServer->mRptPlayerNumber.find(nGameID);
			if ( iter != tpServer->mRptPlayerNumber.end()) 
			{
				iter->second ++;
			}else
			{
				tpServer->mRptPlayerNumber.insert(CSceneServer::GAMEIDPLAYER::value_type(nGameID, 1));
			}
		}else
		{
			CMessageKickOffPlayerRequest tKickOffRequest;

			tKickOffRequest.set_charid(nRoleID);
			tKickOffRequest.set_reason(EM_KICKOUT_NOTINGATE);

			CMessage tMessage;
			tMessage.set_msgpara((uint32_t) &tKickOffRequest);
			pbmsg_sethead(&tMessage, ID_G2S_KICKOFF_PLAYER_REQUEST, 0, 0, 0, 0, 0, 0);

			CGateServer::GetSingleton().Send2Scene(nServerID, &tMessage);

			LOG_ERROR("default", "[ %s : %d ][ %s ] player (%d) is null, pls check it",
					                __LK_FILE__, __LINE__, __FUNCTION__, nRoleID );
		}
	}
}


// ***************************************************************
//  Function:		OnMessageCreateMapResponse
//  Description:	处理场景服务器创建地图回应
//	Input:			nServerID - 场景服务器id; pMessage - 消息
//	OutPut:			无
//	Return:			无
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CGateLogic::OnMessageCreateMapResponse(int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL && pMessage->has_msgpara(), return );
	CMessageCreateMapResponse* tpCreateMap = (CMessageCreateMapResponse*)pMessage->msgpara();
	LK_ASSERT( tpCreateMap != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	CSceneServer* tpServer = CGateServer::GetSingleton().GetServerByID( nServerID );
	LK_ASSERT ( tpServer != NULL, return );

	for ( int i = 0; i < tpCreateMap->lineid_size(); ++ i )
	{
		if (CGateServer::GetSingleton().RegistSceneMap(nServerID, tpCreateMap->lineid(i), tpCreateMap->mapid(i)))
		{
			LOG_INFO("default", "[ %s : %d ][ %s ] SceneMap(%d:%s) on line(%d) is created successfully on server(%s)",
				__LK_FILE__, __LINE__, __FUNCTION__, 
				tpCreateMap->mapid(i), tpCreateMap->tplname( i ).c_str(), tpCreateMap->lineid(i), tpServer->mServerName);
		}
	}
	// 向场景发送信息
	CFamilyManager::GetSingletonPtr()->SendFamilyIDName( nServerID );
	CFamilyManager::GetSingletonPtr()->SendNpcList( nServerID );
	CBidMgr::GetSingleton().SendCorpsNpcList( nServerID );
	CWorldManager::GetSingleton().SyncNation( nServerID );
	CWorldModule::GetSingleton().SendReliveChange( nServerID );
	CWorldModule::GetSingleton().SendSyncWarStatus( nServerID );
	CWorldModule::GetSingleton().SendAllKingNotify( nServerID );
	CWorldModule::GetSingleton().SendCreateWarStoneNotify( 0, nServerID );
	CWorldModule::GetSingleton().SendModifyRateNotify( CWorldManager::GetSingleton().mTaxRate, NULL, 1, CWorldManager::GetSingleton().mWinWorld );
	// 同步弱国列表
	CWorldModule::GetSingleton().m_WeakProtecter.SendCampWeakList( nServerID );
	// 同步国探乱舞
	CWorldManager::GetSingleton().SendGTAndLWToScene( nServerID );
}

// ***************************************************************
//  Function:		OnMessageRegistRepetionInfo
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			无
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CGateLogic::OnMessageRegistRepetionInfo( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL && pMessage->has_msgpara( ) && nServerID, return );
	CMessageRegistRepetionInfo* tpMsgRepetionInfo = ( CMessageRegistRepetionInfo* )pMessage->msgpara( );

	if ( CGateServer::GetSingleton().RegistRepetionInfo( tpMsgRepetionInfo ) == true )
	{
		for( int i = 0; i < tpMsgRepetionInfo->level_size( ); i++ )
		{
			BattleLevel* tpLevel = tpMsgRepetionInfo->mutable_level( i );
			CRepetionModule::GetSingletonPtr( )->SetWaitListLevel( 
					tpMsgRepetionInfo->repetionid( ), tpLevel->levelmin(), tpLevel->levelmax() );
			//固定时间报名的等级
			CRepetionModule::GetSingletonPtr( )->SetFixWaitListLevel( 
				tpMsgRepetionInfo->repetionid( ), tpLevel->levelmin(), tpLevel->levelmax() );

			//固定时间开启副本的等级段
			CRepetionModule::GetSingletonPtr()->SetOnTimeOpenListLevel(
				tpMsgRepetionInfo->repetionid( ), tpLevel->levelmin(), tpLevel->levelmax() );
		}
	}        

	if( tpMsgRepetionInfo->mapid_size( ) > 0 )
	{
		CLineRepetionInfo* tpLineInfo = CRepetionModule::GetSingletonPtr( )->RegistRepetionMap( 
			tpMsgRepetionInfo->lineid( ), tpMsgRepetionInfo->mapid( 0 ) );

		LK_ASSERT_STR( tpLineInfo != NULL, return, "Line %d, Map %d", tpMsgRepetionInfo->lineid( ), 
			tpMsgRepetionInfo->mapid( 0 ) );
	
		tpLineInfo->SetLineRepetionCountMax( tpMsgRepetionInfo->countmax( ) );
	}

	CRepetionModule::GetSingletonPtr( )->ClearBattleOpenSpace( tpMsgRepetionInfo->repetionid( ) );	
	for( int i = 0; i < tpMsgRepetionInfo->space_size( ); i++ )
	{
		CRepetionModule::GetSingletonPtr( )->SetBattleOpenSpace( ( int )tpMsgRepetionInfo->repetionid( ),
			tpMsgRepetionInfo->mutable_space( i ) );
	}
}

// ***************************************************************
//  Function:		OnMessageServerInfoNotice
//  Description:	处理场景服务器通知服务器信息通知
//	Input:			nServerID - 场景服务器id ; pMessage - 消息
//	OutPut:			无
//	Return:			无
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CGateLogic::OnMessageServerInfoNotice(int nServerID, CMessage* pMessage )
{
	LOG_DEBUG( "default", "%s", __FUNCTION__ );

	LK_ASSERT( pMessage != NULL && pMessage->has_msgpara(), return );
	CMessageServerInfoNotice* tpSceneInfo = (CMessageServerInfoNotice*)pMessage->msgpara();
	LK_ASSERT( tpSceneInfo != NULL && nServerID == (int) tpSceneInfo->serverid() , return );

	// 允许在线更新，不检查版本号	
	if (CGateServer::GetSingleton().CheckSceneVersion(nServerID,  
				tpSceneInfo->serverversion(), tpSceneInfo->templateversion()) != 0)
	{
		/*
		CMessage tMessage;

		// 通知场景服务器建立失败
		CMessageCreateMapNotice tCreateMap;
		tCreateMap.set_rtncode( 0 );
		tCreateMap.set_notice( "server or template version does not match, please check ... " );

		tMessage.set_msgpara((uint32_t) &tCreateMap);
		pbmsg_sethead(&tMessage, ID_G2S_CREATE_MAP_NOTICE, 0, 0, 0, 0, 0, 0);  

		CGateServer::GetSingleton().Send2Scene(nServerID, &tMessage );

		return;
		*/
	}

	// 验证网关是否已经启动完成
	if ( mLaunchFlag != EM_LAUNCH_LOACOMPLETE )
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] the gate sever isn't launch finished! Can't accept server( id:%d,name:%s,address:%s ) regist message",
			__LK_FILE__, __LINE__, __FUNCTION__, tpSceneInfo->serverid(), tpSceneInfo->servername().c_str(), tpSceneInfo->address().c_str() );
		return; 
	}

	switch (tpSceneInfo->launchmode())
	{
		// 初始化模式启动
		case GAME_LAUNCH_BY_INITIAL: 
		{
			CGateServer::GetSingleton().RegistSceneByInitial(
					tpSceneInfo->serverid(),
					tpSceneInfo->servername().c_str(),
					tpSceneInfo->address().c_str(),
					tpSceneInfo->templateversion(),
					tpSceneInfo->serverversion(),
					tpSceneInfo->taskversion(),
					tpSceneInfo->playercapacity()
					);
			break;
		}

		// 恢复模式启动
		case GAME_LAUNCH_BY_RECOVERY: 
		{
			CGateServer::GetSingleton().RegistSceneByRecovery(
					tpSceneInfo->serverid(),
					tpSceneInfo->servername().c_str(),
					tpSceneInfo->address().c_str(),
					tpSceneInfo->templateversion(),
					tpSceneInfo->serverversion(),
					tpSceneInfo->taskversion(),
					tpSceneInfo->playercapacity()
					);
			break;
		}

		default:
		{
			return;
		}
	}
}

// ***************************************************************
//  Function:		OnMessageERatingStateNotice
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			无
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CGateLogic::OnMessageERatingStateNotice(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageERatingStateNotice* tpBindMsg = (CMessageERatingStateNotice*) tpMessage->msgpara();

	// 校验本地的worldid
	CGateServer::GetSingleton().SetRegistID(tpBindMsg->gatewayid());


	char buffer[12] = {0};
	sprintf(buffer, "%d", tpBindMsg->gatewayid());
	if ( strlen(buffer) == 6)
	{
		if ( CGateServer::GetSingleton().GetWorldID() != ((buffer[0] - 0x30) * 100 + atoi(&buffer[4])))
		{
			printf("Warnning! Your worldid in config/ip.xml is not match with erating !\n");
		}
	}

	CGateServer::GetSingleton().SetERatingIsLaunched( tpBindMsg->binded() == 1);
}

// ***************************************************************
//  Function:		FireServerSession
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			无
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CGateLogic::FireServerSession(int nSessionClass, CMessage* tpMessage, int nParam1, int nParam2)
{
	LK_ASSERT(tpMessage != NULL, return);

	switch(nSessionClass)
	{
        case em_dbsession_activityyqs_start:
        {
            CActivityYQS::GetSingleton().OnSessionActivityStart( tpMessage, nParam1, nParam2 );
            break;
        }
        case em_dbsession_activityyqs_end:
        {
            CActivityYQS::GetSingleton().OnSessionActivityEnd( tpMessage, nParam1, nParam2 );
            break;
        }
		case em_dbsession_createchar:
		{
			CCoreService::GetSingleton().OnSessionCreateChar(tpMessage, nParam1, nParam2);
			break;
		}

		case em_dbsession_obtainchars:
		{
			CCoreService::GetSingleton().OnSessionObtainChars(tpMessage, nParam1, nParam2);
			break;
		}
		
		case em_dbsession_allroles:
		{
			CServiceRoleIDName::GetSingleton().OnSessionLoadRoleInfoFromDB(tpMessage, nParam1, nParam2);	
			break;
		}

		case em_dbsession_rolelocation:
		{
			CServiceGmCmd::GetSingleton().OnSessionLoadRoleLocationResult(tpMessage, nParam1, nParam2);
			break;
		}

		case em_dbsession_taskinfo:
		{
			CServiceGmCmd::GetSingleton().OnSessionLoadTaskInfoResult(tpMessage, nParam1, nParam2);
			break;
		}

		case em_dbsession_freezeaccount:
		case em_dbsession_sysmail:
		{
			CServiceGmCmd::GetSingleton().OnSessionFreezeAccountResult(tpMessage, nParam1, nParam2);
			break;
		}

		case em_dbsession_unfreezeaccount:
		{
			CServiceGmCmd::GetSingleton().OnSessionUnFreezeAccountResult(tpMessage, nParam1, nParam2);
			break;
		}

		case em_dbsession_removechar:
		{
			CCoreService::GetSingleton().OnSessionDeleteChar(tpMessage, nParam1, nParam2);
			break;
		}

		case em_dbsession_recoverrole:
		{
			CServiceGmCmd::GetSingleton().OnSessionRecoverRoleResult(tpMessage, nParam1, nParam2);
			break;
		}

		case em_dbsession_userstorageinfo:
		{
			CServiceGmCmd::GetSingleton().OnSessionLoadStorageInfoResult(tpMessage, nParam1, nParam2);
			break;
		}

		case em_dbsession_roledetail:
		{
			CServiceGmCmd::GetSingleton().OnSessionLoadRoleDetailResult(tpMessage, nParam1, nParam2);
			break;
		}

		case em_dbsession_rolemail:
		{
			CServiceGmCmd::GetSingleton().OnSessionLoadRoleMailResult(tpMessage, nParam1, nParam2, 0);
			break;
		}

		case em_dbsession_roletitle:
		{
			CServiceGmCmd::GetSingleton().OnSessionLoadRoleMailResult(tpMessage, nParam1, nParam2, 1);
			break;
		}

		case em_dbsession_roleequip:
		{
			CServiceGmCmd::GetSingleton().OnSessionLoadRoleEquipmentResult(tpMessage, nParam1, nParam2);
			break;
		}

		case em_dbsession_rolepackage:
		{
			CServiceGmCmd::GetSingleton().OnSessionLoadRolePackInfoResult(tpMessage, nParam1, nParam2);
			break;
		}
			
		case em_dbsession_iteminpack:
		{
			CServiceGmCmd::GetSingleton().OnSessionLoadRoleItemInPackResult(tpMessage, nParam1, nParam2);
			break;
		}

		case em_dbsession_roleskill:
		{
			CServiceGmCmd::GetSingleton().OnSessionLoadRoleSkillInfoResult(tpMessage, nParam1, nParam2);
			break;
		}

		case em_dbsession_creatfamily:
		{
			CServiceFamily::GetSingleton().OnCreatFamilyDBInfo( tpMessage, nParam1, nParam2 );
			break;
		}
		case em_dbsession_allfamilies:
		{
			CServiceFamily::GetSingleton().OnSessionLoadFamilyFromDB( tpMessage, nParam1, nParam2 );
			break;
		}
		case em_dbsession_getfamilynum:
		{
			CServiceFamily::GetSingleton().OnSessionGetFamilyCountFromDB( tpMessage, nParam1, nParam2 );
			break;
		}
	
		case em_dbsession_getcorpsbidnum:
			{
				CCorpservice::GetSingleton().OnSessionGetDataNumFromDB( tpMessage, nParam1, nParam2, TAB_BID);
				break;
			}
		case em_dbsession_getnpcstatusnum:
			{
				CCorpservice::GetSingleton().OnSessionGetDataNumFromDB( tpMessage, nParam1, nParam2, TAB_NPC);
				break;
			}
		case em_dbsession_getcorpsbattlenum:
			{
				CCorpservice::GetSingleton().OnSessionGetDataNumFromDB( tpMessage, nParam1, nParam2, TAB_BATTLE);
				break;
			}
		case em_dbsession_getcorpsnum:
			{
				CCorpservice::GetSingleton().OnSessionGetDataNumFromDB( tpMessage, nParam1, nParam2, TAB_CORPS);
				break;
			}
		case em_dbsession_creatcorps:
		{
			CCorpservice::GetSingleton().OnSessionCreatCorpsDBInfo( tpMessage, nParam1, nParam2 );
			break;			 
		}
		case em_dbsession_loadcorps:
		{
			CCorpservice::GetSingleton().OnSessionLoadCorps( tpMessage, nParam1, nParam2 );
			break; 
		}
		case em_dbsession_load_corpsbid:
			{
				CCorpservice::GetSingleton().OnSessionLoadCorpsBid( tpMessage, nParam1, nParam2 );
				break;
			}
		case em_dbsession_load_npcstatus:
			{
				CCorpservice::GetSingleton().OnSessionLoadNpcStatus( tpMessage, nParam1, nParam2 );
				break;
			}
		case em_dbsession_load_corpsbattle:
			{
				CCorpservice::GetSingleton().OnSessionLoadCorpsBattle( tpMessage, nParam1, nParam2 );
				break;
			}
		case em_dbsession_socialinfo:
		{
			CServiceGmCmd::GetSingleton().OnSessionUserSocialInfoResult( tpMessage, nParam1, nParam2);	
			break;
		}

		case em_dbsession_query_rank_player:
		{
			CRankModule::GetSingleton().OnSessionLoadIntimePlayerRankInfoFromDB( tpMessage, nParam1, nParam2 );
			break;
		}

		case em_dbsession_query_rank_player_outdated:
		{
			CRankModule::GetSingleton().OnSessionLoadOutdatedPlayerRankInfoFromDB( tpMessage, nParam1, nParam2 );
			break;
		}

		case em_dbsession_save_rank:
		{
			break;
		}

		case em_dbsession_rolecount:
		{
			CServiceRoleIDName::GetSingleton().OnSessionLoadRoleCountFromDB(tpMessage, nParam1, nParam2);
			break;
		}
		case em_dbsession_query_judge:
		case em_dbsession_query_recjudge:
		case em_dbsession_save_judge_favor:
		case em_dbsession_save_judge_bad:
		case em_dbsession_delete_judge_favor:
		case em_dbsession_delete_judge_bad:
		case em_dbsession_query_rec_judge_sum:
			{
				CJudgeModule::GetSingleton().OnDBMessage( tpMessage, nParam1, nParam2, nSessionClass );
				break;
			}
		case em_dbsession_create_union:
		{
			CCorpservice::GetSingleton().OnSessionCreateUnionDBInfo( tpMessage, nParam1, nParam2 );
			break;
		}
		case em_dbsession_load_union:
		{
			CCorpservice::GetSingleton().OnSessionLoadUnion( tpMessage, nParam1, nParam2 );
			break;
		}
		case em_dbsession_load_country:
		{
			CWorldModule::GetSingleton().OnSessionLoadWorld( tpMessage );
			break;
		}
		case em_dbsession_load_countrystatus:
		{
			CWorldModule::GetSingleton().OnSessionLoadWorldStatus( tpMessage );
			break;
		}
		case em_dbsession_load_official:
		{
			CWorldModule::GetSingleton().OnSessionLoadOfficial( tpMessage, nParam1 );
			break;
		}

		case em_dbsession_ibstoreinfo:
		{
			CIbStoreModule::GetSingleton().OnSessionLoabIbStoreInfoFromDB(tpMessage, nParam1, nParam2);
			break;
		}

	    case em_dbsession_marriageinfo:
	    {	
			CServiceGmCmd::GetSingleton().OnSessionSpouseInfoResult(  tpMessage, nParam1, nParam2 );
			break;
		}
		case em_dbsession_load_worldwar:
		{
			CWorldModule::GetSingleton().OnSessionLoadWorldWar( tpMessage );
			break;
		}
		case em_dbsession_load_worldglobal:
		{
			CWorldModule::GetSingleton().OnSessionLoadWorldGlobal( tpMessage );
			break;
		}
		case em_dbsession_load_worldwarchar:
		{
			CWorldModule::GetSingleton().OnSessionLoadWorldWarChar( tpMessage );
			break;
		}
		case em_dnsession_proc_active_role_info:
			{
				CWorldModule::GetSingleton().m_WeakProtecter.OnSessionGetActiveRoleInfo( tpMessage );
				break;
			}
		case em_dbsession_check_bourse:
		{
			CBourseModule::GetSingletonPtr( )->OnSessionCheckFire( tpMessage, nParam1, nParam2 );
			break;
		}
		case em_dbsession_matixinfo:
		{
			CServiceGmCmd::GetSingleton().OnSessionMatrixInfo( tpMessage, nParam1, nParam2 );
			break;
		}		
		case em_dbsession_getglobalroledata:
		{
			CCoreService::GetSingleton().OnSessionGetGlobalRoleData( tpMessage, nParam1, nParam2 );
			break;
		}
		default:
		{
			LOG_ERROR("default", "There is no action for the db session Class (%d)", nSessionClass);
			break;
		}
	}
}

void CGateLogic::OnServerSessionTimeout(int nSessionClass, int nParam1, int nParam2)
{
	switch(nSessionClass)
	{
		case em_dbsession_createchar:
		{
			CCoreService::GetSingleton().OnTimeOUtCreateChar(nParam1, nParam2);
			break;
		}

		case em_dbsession_obtainchars:
		{
			CCoreService::GetSingleton().OnTimeOUtObtainChars(nParam1, nParam2);
			break;
		}

		case em_dbsession_removechar:
		{
			CCoreService::GetSingleton().OnTimeOutDeleteChar(nParam1, nParam2);
			break;
		}
		case em_globalsession_kickglobalplayer :
		{
			CPlayerManager::GetSingleton().OnTimeOutKickGlobalPlayer( nParam1, nParam2 );
			break;
		}	
		case em_dbsession_getglobalroledata : 
		{
			CCoreService::GetSingleton().OnTimeOutGetGlobalRoleData( nParam1, nParam2 );
			break;
		}
		default:
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ] There is no timeout action for dbsession (%d)", 
					__LK_FILE__, __LINE__, __FUNCTION__, nSessionClass);
			break;
		}
	}
}

void CGateLogic::OnMessageExecuteSqlResponse(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	CMessageExecuteSqlResponse3* tpExecuteSql = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();
	LK_ASSERT(tpExecuteSql != NULL, return);

	int nSessionID = tpExecuteSql->sessionid();
	CServerSession* pSession = (CServerSession*) CGateObjManager::GetSingletonPtr()->GetObject(nSessionID);

	if (pSession == NULL)
	{
		LOG_ERROR("default","[ %s : %d ][ %s ] Get session failure by objectID(%d)", 
				__LK_FILE__, __LINE__, __FUNCTION__, nSessionID);
		return;
	}

	FireServerSession(pSession->GetSessionType(), tpMessage, pSession->GetParam1(), pSession->GetParam2());
	CGateObjManager::GetSingletonPtr()->DestroyObject(nSessionID);
}

void CGateLogic::OnMessageDestroyRepetionMap( int nServerID, CMessage* pMessage )
{
	//TODO:
	LK_ASSERT( pMessage != NULL, return );
	LK_ASSERT( pMessage->has_msgpara( ), return );
	CMessageDestroyRepetionMap* tpMessage = ( CMessageDestroyRepetionMap* ) pMessage->msgpara();
	
 	CRepetion* tpRepetion = CRepetionModule::GetSingletonPtr( )->GetRepetion( tpMessage->key( ) );
	LK_ASSERT_STR( tpRepetion != NULL, return, "Key %lld", tpMessage->key( ) );

	CRepetionModule::GetSingletonPtr( )->DestroyRepetion( tpRepetion, REPETION_END_COMMON );
}


#ifdef USING_ERATING
void CGateLogic::CheckAGIPSessionTimeout()
{
	CGateObjManagerImp::ObjMng_AGIPSession* pAGIPSessions = NULL;
	if ( CGateObjManager::GetSingleton().mGateObjManagerImp != NULL) 
	{
		pAGIPSessions = &(CGateObjManager::GetSingleton().mGateObjManagerImp->mAGIPSessionPool);
	}
	
	if(pAGIPSessions == NULL) return;

	std::list< int > lstDelObjs;

	CGateObjManagerImp::ObjMng_AGIPSession::iterator iter4ag = pAGIPSessions->begin();
	for(;iter4ag != pAGIPSessions->end(); iter4ag++)
	{
		CAGIPSession* pSession =  &(*iter4ag);
		if (pSession != NULL)
		{
			if(pSession->GetTimer().Elapse( mConfig.nGateTimerInterval ) == true)
			{
				OnAGIPSessionTimeout(pSession->GetSessionType(), pSession->GetParam1(), pSession->GetParam2());

				//need to be deleted
				lstDelObjs.push_back(pSession->get_id());
			}
		}
	}

	std::list< int >::iterator iter = lstDelObjs.begin();
	for(; iter != lstDelObjs.end(); ++iter)
	{
		CGateObjManager::GetSingletonPtr( )->DestroyObject(*iter);
	}
}
#endif
void CGateLogic::CheckMultiClientPerIP()
{
	if (mConfig.mDefaultClientPerIP == -1 || mConfig.mIsCheckMultiClient == 0) return;

	//typedef std::map<uint32_t, std::vector<int> > ipnumber_t;
	//ipnumber_t ipmap;
	typedef std::map<string, std::vector<int> > macnumber_t;
	typedef std::map<uint32, macnumber_t > ipnumber_t;
	ipnumber_t ipmap;

	CGateObjManagerImp::ObjMng_PlayerAtGate& tObjPool = CGateObjManager::GetSingleton().mGateObjManagerImp->mPlayerAtGatePool;
	CGateObjManagerImp::ObjMng_PlayerAtGate::iterator iter4ip = tObjPool.begin();
	for(; iter4ip != tObjPool.end(); ++iter4ip)
	{
		CGatePlayer* tpPlayer = &(*iter4ip);
		if (tpPlayer == NULL || tpPlayer->mStatus != CGatePlayer::em_status_ingame_idle) continue;


		ipnumber_t::iterator iter4char = ipmap.find((uint32_t) tpPlayer->mClientAddr);
		if ( iter4char != ipmap.end())
		{
		//	macnumber_t tmacmap;
			macnumber_t:: iterator itermac = iter4char->second.find( tpPlayer->GetMac()); 
			if ( itermac != iter4char->second.end())
			{
				itermac->second.push_back(tpPlayer->mCharID);
			}else{
				//macnumber_t tmacmap;
				std::vector<int> v;
				v.push_back( tpPlayer->mCharID );

				iter4char->second.insert( macnumber_t::value_type(  tpPlayer->GetMac() ,v  ) );
				
			}
			//iter4char->second.push_back(tpPlayer->mCharID);
		}else
		{
			std::vector<int> v;
			v.push_back(tpPlayer->mCharID);

			macnumber_t tmacnumber;
			tmacnumber.insert( macnumber_t::value_type( tpPlayer->GetMac(), v));

			ipmap.insert(ipnumber_t::value_type((uint32_t) tpPlayer->mClientAddr, tmacnumber));
		}
	}

	ipnumber_t::iterator iter4num = ipmap.begin();
	for ( ; iter4num != ipmap.end(); ++iter4num)
	{                                       
		int tInIpSize = 0;
		macnumber_t::iterator itermac  =  iter4num->second.begin();
		for ( ;itermac != iter4num->second.end(); ++itermac )
		{
			tInIpSize += itermac->second.size();
		}
		if ( tInIpSize > mConfig.mDefaultClientPerIP )
		{
			CConfigure::MULTICLIENT::iterator iter4multi = mConfig.mMultiClientIP.find(iter4num->first);
			if (iter4multi == mConfig.mMultiClientIP.end() 
			|| ( tInIpSize > iter4multi->second && iter4multi->second != -1 ))
			{
				// 不在允许的列表中
			//	std::vector<int>::iterator iter4char = iter4num->second.begin();
				macnumber_t::iterator itermac2  =  iter4num->second.begin();
				

				for(; itermac2 != iter4num->second.end(); ++itermac2)
				{
					std::vector<int>::iterator iter4char = itermac2->second.begin();
					for (; iter4char != itermac2->second.end() ; ++ iter4char )
					{
					
						CMessageKickOffPlayerRequest tKickOffRequest;

						tKickOffRequest.set_charid(*iter4char);
						tKickOffRequest.set_reason(EM_KICKOUT_MULTICLIENT);

						CMessage tMessage;
						tMessage.set_msgpara((uint32_t) &tKickOffRequest);
						pbmsg_sethead(&tMessage, ID_G2S_KICKOFF_PLAYER_REQUEST, 0, 0, 0, 0, 0, 0);

						CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(*iter4char);

						if (tpPlayer != NULL)
						{
							CGateServer::GetSingleton().Send2Scene(tpPlayer, &tMessage);
							LOG_NOTICE("default", "[ %s : %d ][ %s ] player (%d) is kicked out by multi clients by ip.",
									__LK_FILE__, __LINE__, __FUNCTION__, *iter4char );
						}
					}
				}
			}
			return;
		}

		itermac = iter4num->second.begin();
		for ( ;itermac != iter4num->second.end(); ++itermac )
		{
			CConfigure::MULTICLIENT::iterator iter4multi = mConfig.mMultiClientIP.find(iter4num->first);
			
#ifdef _DEBUG_

			std::vector<int>::iterator iter4char_debug = itermac->second.begin();
			for(; iter4char_debug != itermac->second.end() ; ++iter4char_debug)
			{
				LOG_DEBUG("default", "[ %s : %d ][ %s ] player(%d)----mac[%s]",
					__LK_FILE__, __LINE__, __FUNCTION__,*iter4char_debug ,itermac->first.c_str() );
			}

			/////////test
			

			//////
#endif

			//mac 地址为12个蛋的不检查
			if ( itermac->first.compare("000000000000") == 0)
			{
				continue;
			}
			if ( itermac->second.size() > mConfig.mDefaultClientOnIPPerMac 
				&&  (iter4multi == mConfig.mMultiClientIP.end() 
				|| ( itermac->second.size() > iter4multi->second && iter4multi->second != -1) )
				)
			{
				std::vector<int>::iterator iter4char2 = itermac->second.begin();
				
				for (; iter4char2 != itermac->second.end() ; ++iter4char2 )
				{

					CMessageKickOffPlayerRequest tKickOffRequest;

					tKickOffRequest.set_charid(*iter4char2);
					tKickOffRequest.set_reason(EM_KICKOUT_MULTICLIENT);

					CMessage tMessage;
					tMessage.set_msgpara((uint32_t) &tKickOffRequest);
					pbmsg_sethead(&tMessage, ID_G2S_KICKOFF_PLAYER_REQUEST, 0, 0, 0, 0, 0, 0);

					CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(*iter4char2);

					if (tpPlayer != NULL)
					{
						CGateServer::GetSingleton().Send2Scene(tpPlayer, &tMessage);
						LOG_NOTICE("default", "[ %s : %d ][ %s ] player (%d) is kicked out by multi clients by mac.",
							__LK_FILE__, __LINE__, __FUNCTION__, *iter4char2 );
					}
				}

			}
		}
		
	}

}

void CGateLogic::CheckDBSessionTimeout()
{
	CGateObjManagerImp::ObjMng_ServerSession* pServerSessions = NULL;
	if ( CGateObjManager::GetSingleton().mGateObjManagerImp != NULL) 
	{
		pServerSessions = &(CGateObjManager::GetSingleton().mGateObjManagerImp->mServerSessionPool);
	}
	
	if(pServerSessions == NULL) return;

	std::list< int > lstDelObjs;

	//size_t nSessions = pServerSessions->size();
	CGateObjManagerImp::ObjMng_ServerSession::iterator iter4db = pServerSessions->begin();
	for(;iter4db != pServerSessions->end(); iter4db++)
	{
		CServerSession* pSession =  &(*iter4db);
		if (pSession != NULL)
		{
			if(pSession->GetTimer().Elapse( mConfig.nGateTimerInterval ) == true)
			{
				OnServerSessionTimeout(pSession->GetSessionType(), pSession->GetParam1(), pSession->GetParam2());

				//need to be deleted
				lstDelObjs.push_back(pSession->get_id());
			}
		}
	}

	std::list< int >::iterator iter = lstDelObjs.begin();
	for(; iter != lstDelObjs.end(); ++iter)
	{
		CGateObjManager::GetSingletonPtr( )->DestroyObject(*iter);
	}
}

void CGateLogic::OnTimer(unsigned int vTickCount, unsigned int vTickOffset )
{
	// 不是那么精确的时钟，减少判断次数
	if ( mWaitTimer.Elapse( vTickOffset ) == true )
	{
		int nGateInterval = mWaitTimer.mMaxDuration;

		if (mStateTimer.Elapse(nGateInterval) == true)
		{
			CCoreService::GetSingleton().CheckPlayerStatusTimeout();

			// 不在检查在场景游戏中的玩家超时,依赖服务器重启或服务器超时来管理场景玩家
			CPlayerManager::GetSingleton().CheckPlayerActiveTimeout();

			// 打印对象资源使用情况
			char cObjDumpBuff[20480] = {0};
			unsigned int unBuffLen = sizeof(cObjDumpBuff);
			CGateObjManager::GetSingletonPtr()->Dump(cObjDumpBuff, unBuffLen);
			LOG_INFO( "stat", "%s", cObjDumpBuff );
		}

		// 判断是否需要打印统计信息
		if (mPerfTimer.Elapse(nGateInterval) == true)
		{
			PERF_LOG;
		}

		// TODO: 放开
		CGateServer::GetSingleton().CheckGameServerTimeout();
		
		// 检查是否有可执行系统邮件操作
		CGateServer::GetSingleton().CheckMailSysActionTime();

		// 检查DBSESSION 的超时
		CheckDBSessionTimeout();

		// 检查AGIP的SESSION超时
		CheckAGIPSessionTimeout();

		//  各模块响应时钟
		CModuleManager::GetSingleton().Timer(vTickCount, nGateInterval);

		// 处理排队队列中的玩家, 测试的使用用秒来计数，比较明显
		//CCoreService::GetSingleton().DealQueuePlayer(vTickCount);

		//// 判断网关是否启动成功
		//if ( CModuleManager::GetSingleton().IsLaunched() )
		//{
		//	printf( "\n" );
		//	printf("------------------------------------\n");
		//	printf("|  xyj-gateserver startup success! |\n");
		//	printf("------------------------------------\n");

		//	LOG_INFO( "default", "xyj-gateserver startup success!" );
		//}
	}

	// 检查启动
	if ( mLaunchFlag == EM_LAUNCH_LOADING )
	{
		if ( (mLaunchTimer.Elapse( vTickOffset ) == true) && CModuleManager::GetSingleton().IsLaunched() )
		{
			// 通知各个模块启动完毕
			CModuleManager::GetSingleton().OnLaunchComplete();
			mLaunchFlag = EM_LAUNCH_LOACOMPLETE;
			printf( "\n" );
			printf("------------------------------------\n");
			printf("|  xyj-gateserver startup success! |\n");
			printf("------------------------------------\n");
			LOG_NOTICE("default", "------------------------------------");
			LOG_NOTICE("default", "|  xyj-gateserver startup success! |");
			LOG_NOTICE("default", "------------------------------------");
		}
	}
	else if( mLaunchFlag == EM_LAUNCH_SHUTDOWNCOMPLETE )
	{
		// do something befor quit
		CModuleManager::GetSingleton().ExitServer();

		CGateServer::GetSingleton().ExitServer();

		LOG_NOTICE("default", "gate have been safe shutdown, now exit ...");

		exit( 0 );
	}


	// 处理排队队列中的玩家
	CCoreService::GetSingleton().DealQueuePlayer(vTickCount);

	#ifdef USING_ERATING
	if (mTimerOfReport2ERating.Elapse( vTickOffset ) == true)
	{
		CGateServer::GetSingleton().ReportData2Erating();
	}
	#endif

    if( mOnLineNOTimer.Elapse( vTickOffset ) == true )
	{
		CGateServer::GetSingleton().SaveSceneReport2DB();
	}

	if (mTimerOfKeepAlive.Elapse( vTickOffset ) == true)
	{
		CGateServer::GetSingleton().KeepAlive2Proxy();
	}

	if ( mTimerOfLoginKeepAlive.Elapse( vTickOffset ) == true )
	{
		CGateServer::GetSingleton().SendHeartBeat2Login();
	}

	if ( mConfig.mIsCheckMultiClient )
	{
		if ( mTimerOfCheckMultiClientPerIP.Elapse( vTickOffset ) == true )
		{
			CheckMultiClientPerIP();
		}
	}
}

// ***************************************************************
//  Function:		ExecuteSql
//  Description:	通过账号,指定角色进入游戏
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
bool CGateLogic::ExecuteSql( int vSessionType, int vParam1, int vParam2, 
					int nAccountID, int vCallBack, SQLTYPE vSqlType, const char* pSQL, ... )
{
	int nSessionID = 0;
	if ( vCallBack & SQL_NEED_CALLBACK )
	{
		CSession* pSession= (CSession*) CGateObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_SERVERSESSION );
		if (pSession == NULL) return false;

		nSessionID = pSession->get_id();
		pSession->assign( nSessionID, vSessionType, vParam1, vParam2, mConfig.nServerSessionTimeout);
	}

	va_list tArgs;

	char tSqlCommand[ 30000 ] = {0};

	/*LK_ASSERT(vBufferSize < 4096, return);*/
	va_start( tArgs, pSQL );
	::vsprintf( tSqlCommand, pSQL, tArgs );
	va_end( tArgs );
	CMessageExecuteSqlRequest msg;

	msg.set_sessionid(nSessionID);
	msg.set_sql(tSqlCommand);
	msg.set_callback( vCallBack & SQL_NEED_CALLBACK ? 1 : 0 );

	if( vCallBack & SQL_LATEST_INC_ID )
	{
		msg.set_latestautoincid(1);
	}

	msg.set_sqltype(vSqlType);
	msg.set_sessiontype( vParam1 );
	
	CMessage tMessage ;
	pbmsg_sethead(&tMessage, ID_S2S_EXECUTESQL_REQUEST, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &msg);

	int tSrcGameID = 0;
	int tDesGameID = 0;
	if (  nAccountID > 0 && CPlayerManager::GetSingletonPtr() != NULL )
	{
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct( nAccountID );
		if ( tpPlayer != NULL )
		{
			if ( tpPlayer->GetRoleWorldID() > 0 && tpPlayer->GetRoleWorldID() != CGateServer::GetSingleton().GetWorldID() )
			{
				tSrcGameID = CGateServer::GetSingleton().GetWorldID();
				tDesGameID = tpPlayer->GetRoleWorldID();
			}
		}
	}	

	CGateServer::GetSingleton().Send2Data(nAccountID, &tMessage, tSrcGameID, tDesGameID );

	return true;
}

void CGateLogic::OnReloadConfig()
{
	CGateServer::GetSingleton().OnReloadCfg();
	CModuleManager::GetSingleton().OnReloadCfg();	
	
	// 发送需要追踪的玩家列表  
	SendTracePlayerNotice();	
}

#ifdef USING_ERATING
// ***************************************************************
//  Function:		OnMessageERatingProtoStream
//  Description:	处理	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::OnMessageERatingProtoStream(CMessage* pMessage)
{
	LK_ASSERT( pMessage != NULL && pMessage->has_msgpara() != false , return );
	CMessageERatingProtoStream* pstream = (CMessageERatingProtoStream*) pMessage->msgpara();
	LK_ASSERT( pstream != NULL, return); 
	
	int message_length = pstream->length();
	LK_ASSERT(message_length < MAX_CODE_LEN, return);

	unsigned char message_buffer[MAX_CODE_LEN];
	memcpy(message_buffer, pstream->buffer().c_str(), message_length);

	SysProtocol* pproto = CreateERatingMessage(message_buffer, message_length);	
	if (pproto == NULL) return;

	uint32_t nCommandID = 0;
	pproto->getCommandID(&nCommandID);

	uint32_t nSequenceID = 0;
	pproto->getSequenceID(&nSequenceID);

	uint16_t nLength = 0;
	pproto->getTotalLength(&nLength);

	/*
	LOG_INFO("default", "Recv From ERATING MSGID(%x) GWID(%d) GameID(%d), SEQUENCE(%d), LENGTH(%d)",  
			nCommandID, pproto->getGatewayID(), pproto->getGameID(), nSequenceID,  nLength);
	*/

	OnERatingMessage(pproto);
	delete pproto;
}

SysProtocol* CGateLogic::CreateERatingMessage(unsigned char* message_buffer, short message_length)
{
	SysProtocol* pproto = NULL;

	unsigned int nCmdID = 0;
#ifdef USE_DECRYPTION_PROTOCAL
	SysProtocol::parseCommandID(message_buffer, message_length, &nCmdID);
#else
	SAGIPHeader*  pSAGIPHeader = (SAGIPHeader *)message_buffer;
	nCmdID = htonl(pSAGIPHeader->un_Command_ID);
#endif

	switch  (nCmdID)
	{
		case CMD_GW_DATA_REPORT_RES:
		{
			pproto = new AGIPGWDataReportRes;
			break;
		}

		case CMD_ROLE_LIST_RES:
		{
			pproto = new AGIPRoleListRes;
			break;
		}

		case CMD_CREATE_ROLE_RES:
		{
			pproto = new AGIPCreateRoleRes;
			break;
		}

		case CMD_DELETE_ROLE_RES:
		{
			pproto = new AGIPDeleteRoleRes;
			break;
		}

		/*
		case CMD_USER_ENTER_GAME_RES:
		{
			pproto = new AGIPUserEnterGameRes;
			break;
		}
		*/

		case CMD_ROLE_ENTER_GAME_EX_RES:
		{
			pproto = new AGIPRoleEnterGameExRes;
			break;
		}

		case CMD_USER_LOGOUT_RES:
		{
			pproto = new AGIPUserLogoutRes;
			break;
		}

		/*
		case CMD_PASSWORD_VERIFY_RES:
		{
			pproto = new AGIPPasswordVerifyRes;
			break;
		}
		*/

		case CMD_USER_ITEM_LIST_RES:
		{
			pproto = new AGIPUserItemListRes;
			break;
		}

		case CMD_USER_ITEM_MINUS_RES:
		{
			pproto = new AGIPUserItemMinusRes;
			break;
		}

		case CMD_CREATE_GROUP_RES:
		{
			pproto = new AGIPCreateGroupRes;
			break;
		}

		case CMD_USER_BALANCE_INFO_RES:
		{
			pproto = new AGIPUserBalanceInfoRes;
			break;
		}

		case CMD_USER_DRAW_BALANCE_RES:
		{
			pproto = new AGIPUserDrawBalanceRes;
			break;
		}

		default:
		{
			LOG_ERROR("erating", "[ %s : %d ][ %s ] gateserrver did NOT unpack msg(id = %d,  length = %d)", 
					__LK_FILE__, __LINE__, __FUNCTION__, nCmdID, message_length);        
			return NULL;
		}
	}

	if (pproto == NULL)
	{
		LOG_ERROR("erating", "[ %s : %d ][ %s ] gateserver unpack msg(id = %d,  length = %d) error", 
				__LK_FILE__, __LINE__, __FUNCTION__, nCmdID, message_length);

		return NULL;
	}

	#ifdef USE_DECRYPTION_PROTOCAL
	int result = pproto->setEncodedPDU(message_buffer, message_length, 0);
	#else
	int result = pproto->setDecodedPDU(message_buffer, message_length, 0);
	#endif

	if (result != S_SUCCESS)
	{
		LOG_ERROR("erating", "[ %s : %d ][ %s ] SetEncodePDU/SetDeCodePDU CommandID(%x) Failure, Length(%d), Reason(%d)",
				__LK_FILE__, __LINE__, __FUNCTION__, nCmdID, message_length, result);


		delete pproto;
		return NULL;
	}

	#ifdef _ERATING_DEBUG_
	if ( nCmdID != CMD_GW_DATA_REPORT_RES)
	{
		printf("gate recv from eraring {\n");
		pproto->showInfo();
		printf("}");
	}
	#endif 

	return pproto;
}

void CGateLogic::OnERatingMessage(SysProtocol* pproto)
{
	unsigned int nSessionID = 0;
	pproto->getSequenceID(&nSessionID);

	CAGIPSession* pSession = (CAGIPSession*)  CGateObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if (pSession == NULL)
	{   
		LOG_ERROR("erating", "[ %s : %d ][ %s ] Get SessionObj fail, ObjectID(%d)", 
				__LK_FILE__, __LINE__, __FUNCTION__, nSessionID);
		return;
	}

	FireAGIPSession(pSession->GetSessionType(), pproto, 
			pSession->GetParam1(), pSession->GetParam2(), pSession->mParameter3, pSession->mParameter4, pSession->mParameter5);

	CGateObjManager::GetSingletonPtr()->DestroyObject(nSessionID);
}


void CGateLogic::OnAGIPMsgGWDataReportResponse(SysProtocol* pSysProto, int nParam1, int nParam2)
{
	LK_ASSERT(pSysProto!= NULL, return);

	AGIPGWDataReportRes* pprotores = (AGIPGWDataReportRes*) pSysProto;

	if (pprotores != NULL)
	{
		unsigned int nCommandID = 0;
		pSysProto->getCommandID(&nCommandID);
		LK_ASSERT(nCommandID == CMD_GW_DATA_REPORT_RES, return);

		int32_t nResultCode = 0;
		pprotores->getResultCode(&nResultCode);
		if (nResultCode == S_SUCCESS)
		{
			LOG_INFO("erating", "Send to ERating GWDataReport OK");
		}else
		{
			LOG_ERROR("erating", "Send to ERating GWDataReport Failure(%d)", nResultCode);
		}
	}
}

CAGIPSession* CGateLogic::CreateSession4Proto(SysProtocol* pproto)
{
	CAGIPSession* pSession= (CAGIPSession*) CGateObjManager::GetSingletonPtr()->CreateObject(OBJTYPE_AGIPSESSION);
	if (pSession == NULL)
	{
		LOG_ERROR("erating", "[ %s : %d ][ %s ] Create AGIPSessionObj Failure",
				__LK_FILE__, __LINE__, __FUNCTION__);

		return NULL;
	}

	int session_id = pSession->get_id();
	pproto->setSequenceID(session_id);

	unsigned char message_buffer[AGIP_MAX_BUFFER_SIZE+1];
	int message_length;

	// 如果不存在GameID, 默认设置西游GameID
	if (pproto->getGameID() == 0)
	{
		pproto->setGameID(mConfig.nGameID);	
	}

	// 设置网关
	pproto->setGatewayID(CGateServer::GetSingleton().GetRegistID());

#ifdef USE_DECRYPTION_PROTOCAL
	int result = pproto->getEncodedPDU(message_buffer, AGIP_MAX_BUFFER_SIZE, &message_length);
#else
	int result = pproto->getDecodedPDU(message_buffer, AGIP_MAX_BUFFER_SIZE, &message_length);
#endif

	unsigned int nCommandID = 0;
	pproto->getCommandID(&nCommandID);

	if (result != S_SUCCESS)
	{
		CGateObjManager::GetSingletonPtr()->DestroyObject(pSession->get_id());
		LOG_ERROR("erating", "[ %s : %d ][ %s ] getEncodePDU/getDeCodePDU Proto(%x) failure: %d", 
				__LK_FILE__, __LINE__, __FUNCTION__, nCommandID, result);

		return NULL;
	}

	#ifdef _ERATING_DEBUG_
	if ( nCommandID != CMD_GW_DATA_REPORT)
	{
		printf("gate send 2 erating {\n");
		pproto->showInfo();
		printf("}");
	}
	#endif 

	CMessage tMessage;
	CMessageERatingProtoStream tProtoStream;
	tProtoStream.set_length(message_length);
	tProtoStream.set_buffer(message_buffer, message_length);

	pbmsg_sethead(&tMessage, ID_X2X_ERATING_PROTO_STREAM, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tProtoStream);

	CGateServer::GetSingleton().Send2Login(&tMessage);

	return pSession;
}

#endif

// ***************************************************************
//  Function:		CGateServer::OnServerControlMessage
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::OnServerControlMessage(int nServerID, CMessage* pMsg)
{
	switch( pMsg->msghead().messageid() ) 
	{
		// 场景建立地图之后的回应
		case ID_S2G_CREATE_MAP_RESPONSE:
		{
			OnMessageCreateMapResponse(nServerID, pMsg);
			break;
		}

		// 副本使用
		case ID_S2G_REGIST_REPETION_INFO:
		{
			OnMessageRegistRepetionInfo( nServerID, pMsg );
			break;
		}

		// 场景注册到网关
		case ID_S2G_SERVER_INFO_NOTICE:
		{
			OnMessageServerInfoNotice(nServerID, pMsg);
			break;
		}

		// 销毁副本地图
		case ID_S2G_DESTROY_REPETION_MAP:
		{
			OnMessageDestroyRepetionMap( nServerID, pMsg );
			break;
		}

		// 数据服务器给过来的SQL结果
		case ID_S2S_EXECUTESQL_RESPONSE:
		{
			OnMessageExecuteSqlResponse(pMsg);
			break;
		}

		// 场景服务器过来的心跳消息
		case ID_S2G_GAME_HEART_BEAT_NOTICE:
		{
			OnMessageGameHeartBeatNotice(nServerID, pMsg);
			break;
		}

		// 登录服务器对GateInfo的回应
		case ID_L2G_GATE_INFO_RESPONSE:
		{
			CMessageGateInfoResponse* tpGateInfo = (CMessageGateInfoResponse*) pMsg->msgpara();

			if ( tpGateInfo->errorcode() == ERROR_SUCCESS )
			{
				LOG_NOTICE( "default", "register gate success" );
			} else if ( tpGateInfo->errorcode() == ERROR_OUTOFDATE )
			{
				LOG_ERROR( "default", "register gate failed，gateserver version dosn't match" );
			}

			break;
		}

		case ID_L2G_LOGINSVRSTARTUP_NOTICE:
		{
			CGateServer::GetSingleton().Regist2LoginServer();
			break;
		}

		case ID_S2C_NOTIFY_OBTAINFLOWER: // 广播给客户端即可
		{
			CPlayerManager::GetSingleton().SendWorldMessageFromGate( pMsg );	
			break;
		}

#ifdef USING_ERATING
		// 消息转换protocol -> protobuf
		case ID_X2X_ERATING_PROTO_STREAM:
		{
			OnMessageERatingProtoStream(pMsg);
			break;
		}

		// ERating bind模式改变
		case ID_L2G_ERATING_STATUS_NOTICE:
		{
			OnMessageERatingStateNotice(pMsg);
			break;
		}
#endif
	case ID_S2G_ACK_SAFE_SHUTDOWN:
		{
			OnMessageShutdownAck( pMsg );
			break;
		}
	case ID_S2C_NOTIFY_EVENT:
		{
			CMessageEventNotify *tpNotify = (CMessageEventNotify*)pMsg->msgpara();
			CPlayerManager::GetSingleton().SendCountryMessageFromGate( pMsg, tpNotify->countryid() );
			break;
		};
		default:
		{
			LOG_ERROR( "default", "[%s:%d] no one care this messgeid %d",
				__FUNCTION__, __LINE__, pMsg->msghead().messageid() );
			break;
		}
	}

}

// 100304 MAOCY ADD-BEGIN
//============================================================
// 加载角色验证信息
//============================================================
void CGateLogic::ReadRoleValidQuestionConfig(CConfigure& vConfigure)
{
	//const char* pFileName = "../config/data/question/config.xml";
	const char* pFileName = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"question/config.xml");
	LOG_INFO("default", "[%s:%d][%s] Load role valid question config. (file=%s)",
			__LK_FILE__, __LINE__, __FUNCTION__, pFileName);
	// 打开设置文件
	TiXmlDocument tDoc;
	if(!tDoc.LoadFile(pFileName))
	{
		LOG_ERROR( "default", "[%s:%d][%s] Load xml %s failed", 
				__LK_FILE__, __LINE__, __FUNCTION__, pFileName);
		return;
	}
	TiXmlElement* tpRoot = tDoc.FirstChildElement();
	// 读取组定义信息
	TiXmlElement* tpGroups = tpRoot->FirstChildElement("Groups");
	if(NULL != tpGroups)
	{
		// 处理所有组
		TiXmlElement* tpGroup = tpGroups->FirstChildElement("Group");
		while(NULL != tpGroup){
			// 获得组编号
			int groupId = atoi(tpGroup->Attribute("id"));
			const char* groupName = tpGroup->Attribute("name");
         SRoleValidQuestionGroup& group = vConfigure.roleValidQuestionGroups[groupId];
			// 处理所有选项
			TiXmlElement* tpOption = tpGroup->FirstChildElement("Option");
			while(NULL != tpOption){
				// 获得组编号(每组最多64个)
            int optionId = atoi(tpOption->Attribute("id"));
            group.count = optionId + 1;
				SRoleValidQuestionOption& option = group.options[optionId];
				// 设置选项编号
            option.groupId = groupId;
            option.id = optionId;
				// 设置选项资源
				const char* resource = tpOption->Attribute("resource");
            /*sprintf(option.filename, "../config/data/question/%s/%s", groupName, resource);*/
				char stfile[56] = {0};
				sprintf(stfile,"question/%s/%s", groupName, resource);
				sprintf(option.filename, CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,stfile));
				int handle = open(option.filename, O_RDONLY);
				option.length = read(handle, option.data, ROLE_VALID_QUESTION_RESOURCE_LENGTH);
            close(handle);
            LOG_INFO("default", "[%s:%d][%s] Read question resource file. [group=%d, id=%d, file=%s(%d)]",
               __LK_FILE__, __LINE__, __FUNCTION__, groupId, optionId, option.filename, option.length);
				// 处理下一个
				tpOption = tpOption->NextSiblingElement("Option");
			}
			// 处理下一个
			tpGroup = tpGroup->NextSiblingElement("Group");
		}
	}
	// 读取问题定义信息
	TiXmlElement* tpQuestions = tpRoot->FirstChildElement("Questions");
	if(NULL != tpQuestions)
	{
      vConfigure.roleValidQuestionFlag = atoi(tpQuestions->Attribute("use_flag"));
      vConfigure.roleValidQuestionRange = atoi(tpQuestions->Attribute("random_range"));
      vConfigure.roleValidQuestionRetry = atoi(tpQuestions->Attribute("retry_count"));
		// 处理所有组
		TiXmlElement* tpQuestion = tpQuestions->FirstChildElement("Question");
		while(NULL != tpQuestion){
			// 获得组编号
			int questionId = atoi(tpQuestion->Attribute("id"));
         vConfigure.roleValidQuestionCount = questionId + 1;
			SRoleValidQuestion& question = vConfigure.roleValidQuestions[questionId];
			// 设置编号
			question.id = questionId;
         question.typeId = atoi(tpQuestion->Attribute("type"));
         question.resultId = atoi(tpQuestion->Attribute("result"));
			// 设置名称(名称不得大于1K)
			strcpy(question.description, tpQuestion->Attribute("description"));
         LOG_INFO("default", "[%s:%d][%s] Read question. [id=%d, description=%s]",
            __LK_FILE__, __LINE__, __FUNCTION__, questionId, question.description);
			// 处理下一个
			tpQuestion = tpQuestion->NextSiblingElement("Question");
		}
	}
   LOG_INFO("default", "[%s:%d][%s] Load question resource success. [file=%s]",
         __LK_FILE__, __LINE__, __FUNCTION__, pFileName);
}
// 100304 MAOCY ADD-END

// ***************************************************************
//  Function:		ReadCfg
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
int CGateLogic::ReadCfg(CConfigure& vConfigure)
{
	const char* pcIPConfigPath = "../config/ip.xml";
	int nRetCode = CGateServer::GetSingleton().LoadIPConfig(pcIPConfigPath, vConfigure);
	if (nRetCode != 0)
	{
		return -1;
	}

	const char* pcGateserverConfigPath = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"gate_config/gateserver.xml");
	nRetCode = CGateServer::GetSingleton().LoadGateserverConfig(pcGateserverConfigPath, vConfigure);
	if (nRetCode != 0)
	{
		return -1;
	}
	
	if (CGateServer::GetSingleton().GetProxyServerIP() == NULL
	|| CGateServer::GetSingleton().GetGateServerIP() == NULL)
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] configure file(%s) proxy_server(%s) or gate_server(%s) ", 
				__LK_FILE__, __LINE__, __FUNCTION__, pcIPConfigPath, 
				CGateServer::GetSingleton().GetProxyServerIP(), 
				CGateServer::GetSingleton().GetGateServerIP());

		return -1;
	}

	// 读取脏词库
	CDirtyWord::GetSingletonPtr( )->OI_Dirty_start();
	
	// 初始化需要消息追踪的玩家的列表
	mTracePlayerManager.InitializeList();

	// 100304 MAOCY ADD-BEGIN
	// 读取用户角色验证问题的设置信息
	ReadRoleValidQuestionConfig(vConfigure);
	// 100304 MAOCY ADD-END

	// 为host中玩家数限制变量赋值
	//CGateLogic::GetSingletonPtr()->SetPlayerCountPerScene( vConfigure.nPlayerCountPerScene );	
	return 0;
}

// ***************************************************************
//  Function:		OnClientMessage
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CGateLogic::OnClientMessage( CMessage* tpMessage )
{
	CCoreService::GetSingleton( ).OnClientMessage( tpMessage );
} 

// ***************************************************************
//  Function:		SendCountryMessage
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			07/20/2009
// 
// ***************************************************************
void CGateLogic::SendCountryMessage( CMessage *pMessage, int nCountryID )
{ 
	LK_ASSERT( pMessage != NULL, return )	
	CPlayerManager::GetSingleton().SendCountryMessage( pMessage, nCountryID );
}

// ***************************************************************
//  Function:		SendTracePlayerNotice
//  Description:	发送需要追踪的玩家列表到场景服务器
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			07/24/2009
// 
// ***************************************************************
void CGateLogic::SendTracePlayerNotice(  )
{			
	if ( mTracePlayerManager.mPlayerList.empty() )
	{
		return;
	}		   
		
	CMessage tMsgHead;	
	CMessageNotifyTracedPlayer tMsgNotifyTracedPlayer;	
	tMsgHead.mutable_msghead()->set_messageid( ID_G2S_TRACEDPLAYER_NOTIFY );
	tMsgHead.set_msgpara( (int)&tMsgNotifyTracedPlayer );
	CTracePlayerManager::PlayerList::iterator tItor = mTracePlayerManager.mPlayerList.begin();
	for ( ; tItor != mTracePlayerManager.mPlayerList.end(); ++tItor  )
	{
		CTracedPlayerInfo *tpPlayerInfo = tMsgNotifyTracedPlayer.add_playerlist();
		tpPlayerInfo->set_accountid( tItor->first );
		
		for ( unsigned int i = 0; i < (tItor->second).mRoleList.size(); ++i )
		{
			tpPlayerInfo->add_roleid( (tItor->second).mRoleList[i] );  
		}
	}
	CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tMsgHead );
}


//获取 config 配置文件的目录
const char* CGateLogic::GetConfigDataFileNamePath(int nGameID, const char * filename)
{
	memset( mConfigtmpbuf ,0, 1024 );

	snprintf(mConfigtmpbuf, 1024, "../config/data_%d/%s",nGameID,filename);
	FILE* file = fopen( mConfigtmpbuf, "rb" );	
	if ( file )
	{
		return mConfigtmpbuf;
	}
	else
	{
		memset(mConfigtmpbuf,0,1024);
		snprintf(mConfigtmpbuf, 1024, "../config/data/%s",filename);
	}
	return mConfigtmpbuf;


}

// ***************************************************************
//  Function:		InsertPlayerInfo
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			07/24/2009
// 
// ***************************************************************
int	CTracePlayerManager::InsertPlayerInfo( int nAccountID, int *pRoleIDList, int nRoleIDNum )
{
	CTraceCharInfo tCharInfo;
	if ( pRoleIDList != NULL && nRoleIDNum > 0 )
	{
		for ( int i = 0; i < PLAYERNUM && i < nRoleIDNum; ++i )
		{	 
			tCharInfo.mRoleList.push_back( pRoleIDList[i] );
		}		
	} 	
	mPlayerList.insert( PlayerList::value_type( nAccountID, tCharInfo ) );
	return SUCCESS;
}

// ***************************************************************
//  Function:		InsertPlayerInfo
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			07/24/2009
// 
// ***************************************************************
int	CTracePlayerManager::DeletePlayerInfo( int nAccountID )
{	
	PlayerList::iterator tItor = mPlayerList.find( nAccountID );
	if ( tItor != mPlayerList.end() )
	{
		mPlayerList.erase( tItor );
		return SUCCESS;
	}					
	return 1;
}

// ***************************************************************
//  Function:		CheckIfRoleIDNeedTrace
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			07/24/2009
// 
// ***************************************************************
bool CTracePlayerManager::CheckIfRoleIDNeedTrace( int nAccoutID, int nRoleID )
{	
	PlayerList::iterator tItor = mPlayerList.find( nAccoutID );
	if ( tItor == mPlayerList.end() )
	{	 
		return false;
	}
	if ( (tItor->second).mRoleList.empty() )
	{
		return true;
	}				
	for ( unsigned int i = 0; i < (tItor->second).mRoleList.size(); ++i )
	{
		if ( nRoleID == (tItor->second).mRoleList[i] )
		{
			return true;
		}		
	}
	return false;				
}

// ***************************************************************
//  Function:		InitializeList
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			07/24/2009
// 
// ***************************************************************
int	CTracePlayerManager::InitializeList(  )
{
	TiXmlDocument tDoc;
	/*const char *tpFileName = "../config/data/gate_config/tracelist.xml";*/
	const char *tpFileName = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"gate_config/tracelist.xml");
	if ( !tDoc.LoadFile( tpFileName ) )
	{
		printf( "Cann't find file %s\n", "../config/data/gate_config/tracelist.xml" );
		return -1;
	}
	mPlayerList.clear();  	
	TiXmlElement *tpRootElement = tDoc.FirstChildElement( "playerlist" );	
	LK_ASSERT( tpRootElement != NULL, return -1 )	
	TiXmlElement *tpAccountElement = tpRootElement->FirstChildElement( "account" );
	LK_ASSERT( tpAccountElement != NULL, return -1 )
	TiXmlElement *tpRoleElement = NULL;
	int tAccount = 0;	  	
	for ( ; tpAccountElement != NULL; tpAccountElement = tpAccountElement->NextSiblingElement( ) )
	{
		tAccount = atoi( tpAccountElement->Attribute( "accountid" ) );
		int tRoleIDList[PLAYERNUM] = { 0 };
		int tRoleNum = 0;
		for ( tpRoleElement = tpAccountElement->FirstChildElement( "character" );
				tpRoleElement != NULL; 
				tpRoleElement = tpRoleElement->NextSiblingElement() )
		{
			tRoleIDList[tRoleNum++] = atoi( tpRoleElement->Attribute( "roleid" ) ); 
		}
		InsertPlayerInfo( tAccount, tRoleIDList, tRoleNum );
	}
	return 0;
}

void CGateLogic::SendCreateGroupProto2ERating(int nRoleID, const char* szGroupName, int nGroupType, int nObjectID)
{
	AGIPCreateGroup proto;		

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID ) ;
	if (tpPlayer == NULL) return;

	//TODO: GatewayID
	proto.setGameID(tpPlayer->GetGameID());
	proto.setRoleID(nRoleID);
	proto.setGroupName(szGroupName);
	proto.setGroupType(nGroupType);

	CAGIPSession* pSession = CreateSession4Proto(&proto);
	if (pSession != NULL)
	{
		int nSessionID = pSession->get_id();

#ifdef _DEBUG_
		LOG_DEBUG("erating", "Send2ERating:MsgName[AGIPCreateGroup]MsgSeq[%d] MsgPara[RoleID(%d) Group(%d,%s)]",
				nSessionID, nRoleID, nGroupType, szGroupName);
#endif

		pSession->assign(nSessionID, em_agipsession_creategroup, nObjectID, nGroupType, 
				mConfig.nEratingSessionTimeout);
	}
}

// 安全关闭通知
void CGateLogic::SendShutdownNotify()
{
	CMessage tMessage;
	CMessgeSafeShutdownNotify tShutdownNotify;

	tMessage.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_SAFE_SHUTDOWN );
	tMessage.set_msgpara( (uint32)&tShutdownNotify );
	
	// 发给各场景
	for ( int i = 1; i <= CGateServer::GetSingleton().GetMaxSceneServerID(); ++i)
	{
		CSceneServer *pScene =  CGateServer::GetSingleton().GetServerByID(i);
		if ( pScene != NULL )
		{	
			LOG_NOTICE("default", "notify scene%d shutdown ", i );
			CGateServer::GetSingleton().Send2Scene( pScene, &tMessage);
		}
	}

}

int CGateLogic::OnMessageShutdownAck( CMessage* pMessage )
{
	CMessgeSafeShutdownAck* pAckMsg = (CMessgeSafeShutdownAck*) pMessage->msgpara();
	
	CSceneServer* pHost = CGateServer::GetSingletonPtr()->GetServerByID( pAckMsg->serverid() );
	if (pHost == NULL)
	{
		LOG_ERROR( "default", "server id %d already exit", pAckMsg->serverid() );
		return -1;
	}

	LOG_NOTICE( "default", "server id %d shutdown ", pAckMsg->serverid() );

	CGateServer::GetSingletonPtr()->DeleteHost(pHost);
	CGateServer::GetSingletonPtr()->SetServerObjID( pAckMsg->serverid(), INVALID_OBJ_ID); 

	CGateServer::GetSingletonPtr()->ResetMaxSceneServerID();

	// 如果所有场景都关闭了
	if( CGateServer::GetSingletonPtr()->GetMaxSceneServerID() == 0 )
	{

		// 只有网关主动发起的停机，在全部场景都停完以后，网关才停机。
		// 如果只是各个场景单独停机，网关不需要退出。
		if( mLaunchFlag == EM_LAUNCH_SELFSHUTDOWNING )
		{
			mLaunchFlag = EM_LAUNCH_SHUTDOWNCOMPLETE;	
		}

		LOG_NOTICE( "default", "all server have been shutdown ");
	}

	return 0;
}

// 检查是否在ip列表里面
bool CGateLogic::CheckIfIpInInternalIPList( int nIPAddr )
{
	for (unsigned int i = 0; i < sizeof( mConfig.mInternalIPList ); ++i )
	{
		if ( mConfig.mInternalIPList[i] == 0 )
		{
			continue;
		}
		
		if ( mConfig.mInternalIPList[i] == nIPAddr )
		{
			return true;
		}
	}
	
	return false;
}

void CGateLogic::RefreshTimerConfig()
{
	mWaitTimer = CWTimer(mConfig.nGateTimerInterval);
	mStateTimer = CWTimer(mConfig.nPlayerStateCheckInterval);
	mPerfTimer = CWTimer(PERF_TIMER);

#ifdef USING_ERATING
	mTimerOfReport2ERating = CWTimer(mConfig.nReport2ERatingInterval);
#endif

	mTimerOfCheckMultiClientPerIP = CWTimer(mConfig.mCheckMultiClientInterval);
}