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
#include "judgemodule.h"
#include "core_service.h"
#include "world.h"
#include "globalserver_pb.hxx.pb.h"
#include <list>

#define CheckPlayerEventAvailable( MM ,  FLAG ) \
if ( ( ( MM ) & ( FLAG ) ) == 0) \
{\
	LOG_ERROR("default", "player status (%x) not permited on evnet(%s)", MM,  __FUNCTION__); \
	return; \
}\


using namespace GateServer;

// ***************************************************************
// --- CPlayerManager 实现
// ***************************************************************
template<> CPlayerManager* CSingleton< CPlayerManager>::spSingleton = NULL;
CSharedMem* CPlayerManager::mShmPtr = NULL;

CPlayerManager::CPlayerManager()
{
	if( mShmPtr->GetInitMode() == SHM_INIT )        
	{
		Initialize();
	}else
	{
		Resume();
	}
}

CPlayerManager::~CPlayerManager()
{

}

unsigned int CPlayerManager::CountSize()
{
	return sizeof(CPlayerManager);
}

int CPlayerManager::Initialize()
{
	mPlayerInSceneByChar.initailize();	
	mPlayerInSceneByName.initailize();
	mGatePlayerByAcct.initailize();
	mPlayersConnected.initailize();
	mGlobalScenePlayerMap.initailize();
	//mMacPlayerNum.initailize();

	return 0;
}

int CPlayerManager::Resume()
{
	return 0;
}

void* CPlayerManager::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CPlayerManager::operator delete( void* pointer )
{

}

// ***************************************************************
//  Function:		KickOffGatePlayer
//  Description:	处理客户端登陆网关请求
//	Input:			vCommHandle - 通讯端口； pMessage - 消息指针
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CPlayerManager::KickOffGatePlayer(CGatePlayer* tpFirstPlayer, unsigned int nAnotherIP)
{
	LK_ASSERT( tpFirstPlayer!= NULL, return);


	CheckPlayerEventAvailable(tpFirstPlayer->mStatus, 
					CGatePlayer::em_status_ingate_idle |
					CGatePlayer::em_status_ingate_v4erating_createrole |
					CGatePlayer::em_status_ingate_v4erating_entergame  |
					CGatePlayer::em_status_ingate_v4kickoff |
					CGatePlayer::em_status_ingate_v4leavemap |
					CGatePlayer::em_status_ingate_v4loginqueue |
					CGatePlayer::em_status_ingate_v4mysql_createrole |
					CGatePlayer::em_status_ingate_v4mysql_obtainroles |
					CGatePlayer::em_status_ingate_v4scene_entergame |
					CGatePlayer::em_status_ingate_erating_createdrole |
					CGatePlayer::em_status_ingate_erating_enteredgame );

	/*
	char logbuf[256];
	if (nAnotherIP == (unsigned int) tpFirstPlayer->mClientAddr)
	{
		sprintf(logbuf, "您的帐号已在本机登陆，此帐号被强制下线");
	}else
	{
		unsigned char *p = (unsigned char *) & nAnotherIP;
		sprintf(logbuf, "您的帐号已在别处登陆, 登陆IP地址为%d.%d.%d.%d, 此帐号被强制下线",
				*p, *(p+1), *(p+2), *(p+3));
	}
	*/
	
	// 清除安全密码信息
	tpFirstPlayer->ClearPasswordInfo();

	// 如果在排队状态，则清理其排队状态
	if (tpFirstPlayer->mStatus == CGatePlayer::em_status_ingate_v4loginqueue)
	{
		RemovePlayerFromQueue(tpFirstPlayer);
		tpFirstPlayer->mCharID = 0;
		tpFirstPlayer->mCharName[0] = 0;
		tpFirstPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
		tpFirstPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);
	}

	SendGateErrorNotice(tpFirstPlayer, EGATE_PLAYER_ANOTHER_LOGIN, em_need_disconnect);
}


// ***************************************************************
//  Function:		KickOffScenePlayer
//  Description:	处理客户端登陆网关请求
//	Input:			vCommHandle - 通讯端口； pMessage - 消息指针
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CPlayerManager::KickOffScenePlayer(CGatePlayer* tpFirstPlayer, unsigned int nAnotherIP)
{
	LK_ASSERT(tpFirstPlayer != NULL, return);
	CheckPlayerEventAvailable(tpFirstPlayer->mStatus, 
			CGatePlayer::em_status_ingame_idle | 
			CGatePlayer::em_status_ongame_v4scene_logingame |
			CGatePlayer::em_status_ingame_v4scene_redirectscene );
	
	// 清除安全密码信息
	tpFirstPlayer->ClearPasswordInfo();			

	OnlyDisconnectClient(tpFirstPlayer);

	CMessageKickOffPlayerRequest tKickOffRequest;

	tKickOffRequest.set_accountid(tpFirstPlayer->mAccountID);
	tKickOffRequest.set_charid(tpFirstPlayer->mCharID);
	tKickOffRequest.set_reason(EM_KICKOUT_RELOGIN);
	tKickOffRequest.set_anotherip(nAnotherIP);
	tKickOffRequest.set_roleworldid(tpFirstPlayer->GetRoleWorldID());

	
	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tKickOffRequest);
	pbmsg_sethead(&tMessage, ID_G2S_KICKOFF_PLAYER_REQUEST, 0, 0, 0, 0, 0, 0);

	CGateServer::GetSingleton().Send2Scene(tpFirstPlayer, &tMessage);

	tpFirstPlayer->mStatus = CGatePlayer::em_status_ingate_v4kickoff;
	tpFirstPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerWaitKickoffTimeout);
}

// ***************************************************************
//  Function:		CPlayerManager::RegistPlayerComm
//  Description:	处理客户端登陆网关请求
//	Input:			vCommHandle - 通讯端口； pMessage - 消息指针
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CPlayerManager::RegistPlayerComm(CGatePlayer* tpPlayer)
{
	if (tpPlayer == NULL) return;

	pair<PLAYERITER, bool> ret = mPlayersConnected.insert( PLAYERTYPE( tpPlayer->mCommHandle, tpPlayer->get_id()) ) ;
	if (ret.second == false && ret.first->second != tpPlayer->get_id())
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] fd (%d) player obj(%d) will be occupied by player obj(%d)",
				__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCommHandle, ret.first->second, tpPlayer->get_id());

		ret.first->second = tpPlayer->get_id();
	}
}




// ***************************************************************
//  Function:		CPlayerManager::GetPlayerCountryLineID
//  Description:	处理场景服务器创建地图回应
//	Input:			nServerID - 场景服务器id; pMessage - 消息
//	OutPut:			无
//	Return:			无
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
unsigned short CPlayerManager::GetPlayerCountryLineID( CGatePlayer* vpPlayer )
{
	LK_ASSERT( vpPlayer != NULL, return 0 );

	return vpPlayer->mCountryID;
}

// ***************************************************************
//  Function:		SendRedirErrCod
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
int CPlayerManager::SendRedirErrCode( int vCharID, int vErrorCode, int vLineID, int vMapID )
{
	CGateLogic* tpLogic = CGateLogic::GetSingletonPtr();
	if( tpLogic == NULL )
	{
		return 0;
	}

	CGatePlayer* tpPlayer = ScenePlayer( vCharID );
	if( tpPlayer == NULL )
	{
		return 0;
	}

	CSceneServer* tpServer = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
	if( tpServer == NULL )
	{
		return 0;
	}

	CMessage tMsg;
	CMessageRedirectGErrorCode tMsgFact;

	tMsgFact.set_charid( vCharID );
	tMsgFact.set_errorcode( vErrorCode );
	tMsgFact.set_lineid( vLineID );
	tMsgFact.set_mapid( vMapID );

	pbmsg_sethead(&tMsg, ID_G2S_REDIRECT_G_ERROR_CODE, 0, 0, 0, 0, 0, 0);
	tMsg.set_msgpara((uint32_t) &tMsgFact);

	CGateServer::GetSingleton().Send2Scene( tpServer, &tMsg );
	
	return 0;
}


// ***************************************************************
//  Function:		OnMessagePlayerEnterConfirm
//  Description:	玩家进入场景确认
//	Input:			
//	OutPut:			
//	Return:			
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************

void CPlayerManager::RegistRoleIDIndex( CGatePlayer* tpPlayer)
{
	if (tpPlayer == NULL) return;

	pair<PLAYERITER, bool> ret4char  = 
		mPlayerInSceneByChar.insert( PLAYERTYPE( tpPlayer->mCharID, tpPlayer->get_id() ) );

	if ( ret4char.second == false)	
	{
		if ( ret4char.first->second == tpPlayer->get_id()) return;

		LOG_NOTICE("default", "[ %s : %d ][ %s ] Regist role(%d:%s) object from (%d) to (%d)", 
				__LK_FILE__, __LINE__, __FUNCTION__, 
				tpPlayer->mCharID, tpPlayer->GetCharNameShow(), ret4char.first->second, tpPlayer->get_id());

		ret4char.first->second = tpPlayer->get_id();
	}

	/*
	if (tpPlayer->mAdultFlag != 1) // 非成人玩家
	{
		LOG_DEBUG("default", "[ %s : %d ][ %s ] Role(%d:%s) has bee add Fcm list", 
				__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, tpPlayer->GetCharNameShow());

		CServiceGateFcm::GetSingleton().InsertFcmPlayer(tpPlayer->mCharID, tpPlayer->mValidSeconds);
	}
	*/
}

// ***************************************************************
//  Function:		RegistAccountIndex
//  Description:	 注册账号ID越GatePlayer的关系
//	Input:			
//	OutPut:			
//	Return:			
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
bool CPlayerManager::RegistAccountIndex( CGatePlayer* tpPlayer )
{
	pair<PLAYERITER,bool> ret =	mGatePlayerByAcct.insert( PLAYERCONTAINER::value_type(tpPlayer->mAccountID, tpPlayer->get_id()) );
	if ( ret.second == false )
	{
		LOG_FATAL("default", "[ %s : %d ][ %s ] Insert Account(%d) Duplicate ,use objid(%d) replace objid(%d)", 
			__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpPlayer->get_id(), ret.first->second );
		ret.first->second =  tpPlayer->get_id();
		return false;
	}
	return true;
}

// ***************************************************************
//  Function:		RemoveRoleIDIndex
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::RemoveRoleIDIndex(CGatePlayer* tpPlayer)
{
	if (tpPlayer == NULL) return;

	PLAYERITER  tIter4Char= mPlayerInSceneByChar.find( tpPlayer->mCharID );
	if ( tIter4Char!= mPlayerInSceneByChar.end( ) )
	{
		mPlayerInSceneByChar.erase( tIter4Char);
	}

	if (tpPlayer->mAdultFlag != 1) // 非成人玩家
	{
		LOG_DEBUG("default", "[ %s : %d ][ %s ] Role(%d:%s) has bee removed from Fcm list", 
				__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, tpPlayer->GetCharNameShow());

		CServiceGateFcm::GetSingleton().RemoveFcmPlayer(tpPlayer->mCharID);
	}
}

// ***************************************************************
//  Function:		RemoveAccountIDIndex
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::RemoveAccountIDIndex(CGatePlayer* tpPlayer)
{               
	if (tpPlayer == NULL) return;

	PLAYERITER tFindIter = mGatePlayerByAcct.find( tpPlayer->mAccountID);
	if(tFindIter != mGatePlayerByAcct.end())
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] Remove Account(%d) Index from Hashmap", 
				__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID);
		mGatePlayerByAcct.erase( tFindIter);
	}
} 

// ***************************************************************
//  Function:		RegistRoleNameIndex
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::RegistRoleNameIndex(CGatePlayer* tpPlayer)
{
	if (tpPlayer == NULL) return;

	pair<PLAYERNAME_MAP::iterator, bool> ret4name = 
		mPlayerInSceneByName.insert( PLAYERNAME_MAP::value_type(tpPlayer->mCharName, tpPlayer->get_id()) );

	if ( ret4name.second == false)	
	{
		if ( ret4name.first->second == tpPlayer->get_id()) return;

		LOG_NOTICE("default", "[ %s : %d ][ %s ] Regist role(%d:%s) object from (%d) to (%d)", 
				__LK_FILE__, __LINE__, __FUNCTION__, 
				tpPlayer->mCharID, tpPlayer->GetCharNameShow(), ret4name.first->second, tpPlayer->get_id());

		ret4name.first->second = tpPlayer->get_id();
	}
}

// ***************************************************************
//  Function:		RemoveRoleNameIndex
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::RemoveRoleNameIndex(CGatePlayer* tpPlayer)
{
	if (tpPlayer == NULL) return;

	PLAYERNAME_MAP::iterator tIter4Name = mPlayerInSceneByName.find( tpPlayer->mCharName);
	if ( tIter4Name != mPlayerInSceneByName.end( ) )
	{
		mPlayerInSceneByName.erase( tIter4Name );
	}
}

//// ***************************************************************
////  Function:		UpdateMacNum
////  Description:	
////	Input:			
////	OutPut:			无
////	Return:			
////	Others:
////  Date:			5/31/2011
//// 
//// ***************************************************************
//void CPlayerManager::UpdatePlayerNumPerMac(CGatePlayer* tpPlayer)
//{               
//
//	if (tpPlayer == NULL) return;
//	MACPLAYERNUM::iterator tIterNum = mMacPlayerNum.find( tpPlayer->GetMac());
//	if ( tIterNum != mMacPlayerNum.end() )
//	{
//		//if( --tIterNum->second < 1)
//		//{
//		//	mMacPlayerNum.erase( tIterNum );
//		//}
//		
//		for(int i = 0 ; i < tIterNum->second.size(); )
//		{
//			if(tIterNum->second[i] == tpPlayer->AccountID( ))
//			{
//				tIterNum->second.erase(i);
//				continue;
//			}
//			i++;
//		}
//
//		if ( tIterNum->second.size() == 0 )
//		{
//			mMacPlayerNum.erase( tIterNum );
//		}
//	}

//} 

// ***************************************************************
//  Function:		RemovePlayerFromQueue
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::RemovePlayerFromQueue(CGatePlayer* tpPlayer)
{
	LK_ASSERT(tpPlayer->mStatus == CGatePlayer::em_status_ingate_v4loginqueue, return);

	CSceneServer* pHost = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
	if(pHost == NULL) 
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] Can not find the Host by Account(%d) line(%d), map(%d)", 
				__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpPlayer->mLineID, tpPlayer->mMapID);

		return ;
	}

	CSceneServer::CLoginQueue::iterator iter = pHost->mLoginQueue.begin();
	while(iter != pHost->mLoginQueue.end())
	{
		if (tpPlayer->get_id() == iter->PlayerID())
		{
			LOG_NOTICE("default", "[ %s : %d ][ %s ] Erase Account(%d) from mLoginQueue ",
					__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID);

			iter = pHost->mLoginQueue.erase(iter);
			break;
		}

		iter++;
	}
}

// ***************************************************************
//  Function:		SendCountryMessage
//  Description:	发送国家消息
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			07/20/2009
// 
// ***************************************************************
void CPlayerManager::SendCountryMessage( CMessage *pMessage, int nCountryID )
{ 
	LK_ASSERT( pMessage != NULL, return )
	pMessage->mutable_msghead()->set_dstfe( FE_COUNTRY );
	pMessage->mutable_msghead()->set_dstid( nCountryID );
	CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, pMessage );
}



// ***************************************************************
//  Function:		SendCountryMessage
//  Description:	从gate上直接给某个国家的玩家发送消息
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			07/20/2009
// 
// ***************************************************************
void CPlayerManager::SendCountryMessageFromGate( CMessage *pMessage, int nCountryID )
{
	std::vector<CGatePlayer* > vPlayerList;
	PLAYERCONTAINER::iterator it = mPlayerInSceneByChar.begin();
	for( ; it != mPlayerInSceneByChar.end(); ++it )
	{
		CGatePlayer* tpPlayer = static_cast<CGatePlayer*> ( CGateObjManager::GetSingletonPtr()->GetObject(it->second) );
		if( tpPlayer == NULL )
		{
			continue;
		}
		if ( tpPlayer->mCountryID == nCountryID )
		{
			vPlayerList.push_back( tpPlayer );
		}		
	}
	CGateServer::GetSingletonPtr()->Send2Player( vPlayerList, pMessage );
}

// 从gate上直接给世界的玩家发送消息, MapID = 0全发, != 0只给指定地图的发
void CPlayerManager::SendWorldMessageFromGate( CMessage *pMessage, int vMapID )
{
	PLAYERCONTAINER::iterator it = mPlayerInSceneByChar.begin();
    std::vector<CGatePlayer*> vPlayerList;
	for( ; it != mPlayerInSceneByChar.end(); ++it )
	{
		CGatePlayer* tpPlayer = static_cast<CGatePlayer*> ( CGateObjManager::GetSingletonPtr()->GetObject(it->second) );
		if( tpPlayer == NULL )
		{
			continue;
		}
		if ( vMapID != 0 && tpPlayer->MapID() != vMapID )
		{
			continue;
		}

        vPlayerList.push_back(tpPlayer);
		//CGateServer::GetSingletonPtr()->Send2Player( tpPlayer, pMessage );
	}	

    CGateServer::GetSingletonPtr()->Send2Player( vPlayerList, pMessage );
}

// 重置玩家财产密码
int CPlayerManager::ResetRolePassword( int nRoleID )
{  
	CGatePlayer *tpPlayer = ScenePlayer( nRoleID );
	if ( tpPlayer == NULL )
	{
		// 如果玩家存在则直接更新数据库
		CRoleGlobalInfo *tpRoleInfo = CServiceRoleIDName::GetSingleton( ).GetRoleInfoByRoleID( nRoleID );
		if ( tpRoleInfo == NULL )
		{
			return -1;
		}
		const char *pPassword = "";
		UpDateRolePassword( tpRoleInfo->AccountID(), nRoleID, pPassword, 0, 0 );
	}	
	else
	{	
		tpPlayer->DeletRolePwd( nRoleID );
						
		CMessage tMsgHead;
		CMessageResetRolePasswordNotify tMsgResetRolePassword;		
		tMsgHead.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_RESETROLEPASSWORD );			
		tMsgHead.set_msgpara( (int)&tMsgResetRolePassword );
		tMsgResetRolePassword.set_roleid( nRoleID );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
	}
	
	return SUCCESS;
}

// 更新玩家的密码
void CPlayerManager::UpDateRolePassword( int nAccountID, int nRoleID, const char *pPassword, int nParam1, int nParam2 )
{
	LK_ASSERT( pPassword != NULL, return )		
	const char *tpSql = "update UMS_ROLE set role_password= '%s' where role_id = %d ";
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_updatedeltetime, nParam1, nParam2, nAccountID, SQL_NONE, UPDATE, tpSql, pPassword, nRoleID );
}

// 插入一个玩家到全局场景列表中
void CPlayerManager::InsertPlayerToGlobalSceneMap( int nAccountID, CGlobalData &rGlobalData )
{
	GLOBALSCENEPLAYERMAP::iterator it = mGlobalScenePlayerMap.find( nAccountID );
	if ( it == mGlobalScenePlayerMap.end() )
	{
		mGlobalScenePlayerMap.insert( GLOBALSCENEPLAYERMAP::value_type( nAccountID, rGlobalData ));	
	}	
}
// 从全局场景列表中删除一个玩家
void CPlayerManager::DeletePlayerFromGlobalSceneMap( int nAccountID )
{
	GLOBALSCENEPLAYERMAP::iterator it = mGlobalScenePlayerMap.find( nAccountID );
	if ( it != mGlobalScenePlayerMap.end() )
	{
		mGlobalScenePlayerMap.erase( it );
	}
}
// 踢出在其他服务器的玩家
void CPlayerManager::KickPlayerInGlobalScene( int nAccountID /* = 0 */ )
{
	if ( nAccountID != 0 )
	{
		GLOBALSCENEPLAYERMAP::iterator it = mGlobalScenePlayerMap.find( nAccountID );
		if ( it != mGlobalScenePlayerMap.end( ) )
		{
			CMessage tMsgHead;
			CMessageKickGlobalPlayerNotify tMsgKickPlayer;
			tMsgHead.mutable_msghead()->set_messageid( ID_S2S_NOTIFY_KICKGLOBALPLAYER );
			tMsgHead.set_msgpara( (int)&tMsgKickPlayer );
			tMsgKickPlayer.set_roleid( (it->second).mRoleID );
			CGateServer::GetSingleton().Send2GlobalDunserver( &tMsgHead );			
			//// 设置超时
			//CSession* pSession= (CSession*) CGateObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_SERVERSESSION );
			//if (pSession == NULL) return ;

			//int nSessionID = pSession->get_id();
			//pSession->assign( nSessionID, em_globalsession_kickglobalplayer, it->first, (it->second).mRoleID, 120000 );
		}	
		return;
	}

	GLOBALSCENEPLAYERMAP::iterator it = mGlobalScenePlayerMap.begin();
	for ( ; it != mGlobalScenePlayerMap.end(); ++it )
	{
		CMessage tMsgHead;
		CMessageKickGlobalPlayerNotify tMsgKickPlayer;
		tMsgHead.mutable_msghead()->set_messageid( ID_S2S_NOTIFY_KICKGLOBALPLAYER );
		tMsgHead.set_msgpara( (int)&tMsgKickPlayer );
		tMsgKickPlayer.set_roleid( (it->second).mRoleID );
		CGateServer::GetSingleton().Send2GlobalDunserver( &tMsgHead );
	}
}

// 判断玩家是否在其他阵营
bool CPlayerManager::IsPlayerInGlobalScene( int nAccountID )
{
	GLOBALSCENEPLAYERMAP::iterator it = mGlobalScenePlayerMap.find( nAccountID );
	if ( it != mGlobalScenePlayerMap.end() )
	{
		return true;
	}
	return false;
}

// 发送踢出全局场景玩家的结果通知消息
void CPlayerManager::SendKickGlbalPlayerACK( int nAcccountID, int nRoleID, int nRoleWorlID )
{
	CMessage tMsgHead;
	CMessageKickGlobalPlayerACK tMsgKickPlayer;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2S_NOTIFY_KICKGLOBALPLAYERACK );
	tMsgHead.set_msgpara( (int)&tMsgKickPlayer );
	tMsgKickPlayer.set_roleid( nRoleID );
	tMsgKickPlayer.set_accountid( nAcccountID );
	tMsgKickPlayer.set_roleworldid( nRoleWorlID );
	CGateServer::GetSingleton().Send2GlobalDunserver( &tMsgHead );
}

// 踢出玩家超时
void CPlayerManager::OnTimeOutKickGlobalPlayer( int nAccountID, int nRoleID )
{
	DeletePlayerFromGlobalSceneMap( nAccountID );  
}

// 获取全局信息
CGlobalData * CPlayerManager::GetGlobalData( int nAccountID )
{
	GLOBALSCENEPLAYERMAP::iterator it = mGlobalScenePlayerMap.find( nAccountID );
	if ( it != mGlobalScenePlayerMap.end() )
	{
		return &(it->second);
	}
	return NULL;
}

// ***************************************************************
//  Function:		ScenePlayer
//  Description:	通过玩家名称得到玩家数据
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
CGatePlayer* CPlayerManager::ScenePlayer( const char* pName )
{
	PLAYERNAME_MAP::iterator tFindIter = mPlayerInSceneByName.find( pName );
	if ( tFindIter == mPlayerInSceneByName.end( ) )
		return NULL;

	return static_cast<CGatePlayer*> ( CGateObjManager::GetSingletonPtr()->GetObject(tFindIter->second) );
}

// ***************************************************************
//  Function:		ScenePlayer
//  Description:	通过角色ID得到场景中的玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
CGatePlayer* CPlayerManager::ScenePlayer( int vCharID )
{
	PLAYERITER tFindIter = mPlayerInSceneByChar.find( vCharID );
	if ( tFindIter == mPlayerInSceneByChar.end( ) )
		return NULL;

	CGatePlayer* tpPlayer = static_cast<CGatePlayer*> ( CGateObjManager::GetSingletonPtr()->GetObject(tFindIter->second) );

	return tpPlayer;
}

// ***************************************************************
//  Function:		GatePlayer
//  Description:	得到网关中的玩家,通过通讯端口ID
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
CGatePlayer* CPlayerManager::GatePlayer( int vCommHandle )
{
	PLAYERITER tFindIter = mPlayersConnected.find( vCommHandle );
	if ( tFindIter == mPlayersConnected.end( ) )
	{
		return NULL;
	}
	return static_cast<CGatePlayer*> ( CGateObjManager::GetSingletonPtr()->GetObject(tFindIter->second) );
	//return tFindIter->second;
}

// ***************************************************************
//  Function:		QueuePlayer
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::QueuePlayer(CGatePlayer* tpPlayer, CSceneServer* tpHost)
{
	CMessage tMessage;

	if (tpHost->mLoginQueue.size() == CSceneServer::PLAYER_WAIT4LOGIN_CAP)
	{
		/*
		sprintf(logbuf, "当前服务器已满(共 %d 玩家), 排队已满( %d )玩家, 请选择其它服务器",
				tpHost->mPlayerNumber, tpHost->mLoginQueue.size());
		*/

		tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
		tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);

		LOG_NOTICE("default", "[ %s : %d ][ %s ] Sceneserver(%d:%s) is full(%d), waitqueue is full(%d) too.", 
				__LK_FILE__, __LINE__, __FUNCTION__, 
				tpHost->mServerID, tpHost->mServerName, tpHost->PlayerCount(),tpHost->mLoginQueue.size());


		SendGateErrorNotice(tpPlayer, EGATE_THESCENE_IS_FULLIST, em_keep_connect);
	}else
	{
		/*
		sprintf(logbuf, "当前服务器已满(共 %d 玩家), 您前面尚有( %d )玩家在排队.",
				tpHost->mPlayerNumber, tpHost->mLoginQueue.size());
		*/

		tpHost->mLoginQueue.push_back(CQueuePlayerInfo(tpPlayer->get_id(), LINEKONG::GetTickCount()));
		tpPlayer->mStatus = CGatePlayer::em_status_ingate_v4loginqueue;

		LOG_NOTICE("default", "[ %s : %d ][ %s ] Player(%d, %d:%s) is at pos %d on login queue of Sceneserver(%d:%s) ",
				__LK_FILE__, __LINE__, __FUNCTION__, 
				tpPlayer->mAccountID, tpPlayer->mCharID, tpPlayer->GetCharNameShow(), 
				tpHost->mLoginQueue.size(), tpHost->mServerID, tpHost->mServerName);

		if (tpHost->IsFull() || tpHost->mLoginQueue.size() >= tpHost->mPermitPerTimer)
		{
			CMessageQueuePlayerNotify tQueueMsg;

			tQueueMsg.set_countinscene(tpHost->PlayerCount());
			tQueueMsg.set_countinqueue(tpHost->mLoginQueue.size());
			tQueueMsg.set_yourposition(tpHost->mLoginQueue.size());
			tQueueMsg.set_elapsseconds(0);
			tQueueMsg.set_needseconds(0);

			pbmsg_sethead(&tMessage, ID_G2S_NOTIFY_LOGINQUEUE, 0, 0, 0, 0, 0, 0);
			tMessage.set_msgpara((uint32_t) &tQueueMsg);

			CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage);
		}
	}
}


// ***************************************************************
//  Function:		PlayerEnterGame
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::PlayerEnterGame(CGatePlayer* tpPlayer)
{
	LK_ASSERT(tpPlayer != NULL, return);

	/*
	#ifdef USING_ERATING
	if (CGateServer::GetSingleton().IsERatingLaunched() == true)
	{
		CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_erating_enteredgame);
	}else
	{
		CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4loginqueue);
	}
	#else
	*/
		CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4loginqueue);
	//#endif


	CSceneServer* tpHost = NULL;

	CMessage tSendMessage;
	CMessagePlayerEnterMapNotice tPlayerEnter;

	LOG_NOTICE( "default", "[ %s : %d ][ %s ]Player( %d )EnterGame, Scene Is LineID %d MapID %d", 
			__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, tpPlayer->mLineID, tpPlayer->mMapID );


	tPlayerEnter.set_lineid(tpPlayer->mLineID);
	tPlayerEnter.set_mapid(tpPlayer->mMapID);
//	if( tpPlayer->mPoint.mX != -1 && tpPlayer->mPoint.mY != -1 )
//	{
		tPlayerEnter.set_posx( tpPlayer->mPoint.mX );
		tPlayerEnter.set_posy( tpPlayer->mPoint.mY );
//	}

	tPlayerEnter.set_key( tpPlayer->mRepetionMap[ tpPlayer->mMapID ] );

	tpHost = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
	if ( tpHost == NULL )
	{
		LOG_ERROR("default",  "[ %s : %d ][ %s ] Sceneserver is not open when role(%d) entergame  Line:%d, Map:%d", 
				__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, tpPlayer->mLineID, tpPlayer->mMapID );

		SendGateErrorNotice(tpPlayer, EGATE_THESCENE_ISNOT_ONLINE, em_keep_connect);

		//TODO: 清理玩家, 或者等玩家断开链接
		return;
	}

	// 区组ID
	tPlayerEnter.set_roleworldid( tpPlayer->GetRoleWorldID() );

	//	获取家族信息
	CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpPlayer->mCharID );	
	if ( pFamily != NULL )
	{
		tPlayerEnter.set_familyid( pFamily->GetID() );
		tPlayerEnter.set_familyname( pFamily->GetName() );	
		if( pFamily->GetRooter() == tpPlayer->mCharID )
		{
			tPlayerEnter.set_isfamilyheader( 1 );
			int tApplyNum = pFamily->GetAppplyNum();
			if ( tApplyNum > 0 )
			{
				tPlayerEnter.set_ifhasapplies( 1 );
			}				
		}
		tPlayerEnter.set_familymoney( pFamily->GetMoney() ); 			
		
		tPlayerEnter.set_familyheaderid( pFamily->GetRooter() );
	
		family_info_t *tpInfo = pFamily->GetMemberFamilyInfo( tpPlayer->mCharID  );
		if ( tpInfo != NULL )
		{
			tPlayerEnter.set_familypost( tpInfo->mFamilyPost );
			tPlayerEnter.set_familycontribute( tpInfo->mFamilyContribue );	
		}		
				
		// 如果报名了，通知进入战场
		CFamilyManager::GetSingletonPtr()->NotifyEnter( pFamily->GetID(), tpPlayer->mCharID );
		if ( pFamily->GetCorpsID() != 0 )
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( pFamily->GetCorpsID() );	
			if ( tpCorps != NULL )
			{
				tPlayerEnter.set_corpsid( tpCorps->GetCorpsID() );
				tPlayerEnter.set_corpsname( tpCorps->GetCorpsName() );
				if ( tpCorps->GetRooterID() == tpPlayer->mCharID )
				{
					tPlayerEnter.set_iscorperheader( 1 ); 
					if ( tpCorps->GetApplyNum() > 0 )
					{	
						tPlayerEnter.set_ifhascorpsapply( 1 );						
					}						
				} 	
				tPlayerEnter.set_corpsmoney( tpCorps->GetCorpsMoney() );														
				tPlayerEnter.set_corpsheaderid( tpCorps->GetRooterID() );
				
				if ( tpInfo != NULL )
				{	
					tPlayerEnter.set_corpspost( tpInfo->mCorpsPost );
					tPlayerEnter.set_corpscontribute( tpInfo->mCorpsContribue );
				}
				int tCountryID = CWorldManager::GetSingleton().GetCountryIDByMap( tpCorps->GetBattleCity( ) );
				if ( tCountryID != COUNTRY_NONE )
				{
					if ( tpPlayer->TempID( ) != tCountryID )
					{
						CWorldManager::GetSingleton().ClearOfficialID( tpPlayer );
					}
					tpPlayer->TempID( tCountryID );
				}								
			}			
		}		
	}
	CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionOfPlayer( tpPlayer->mCharID );
	if ( tpUnion != NULL )
	{
		tPlayerEnter.set_unionid( tpUnion->GetUnionID() );
		tPlayerEnter.set_unionname( tpUnion->GetUnionName() );
		tPlayerEnter.set_headcorpsid( tpUnion->GetCoreID() );
	}
	tPlayerEnter.set_countryid( tpPlayer->TempID() );
	if ( tpPlayer->TempID( ) >= 0 && tpPlayer->TempID( ) < COUNTRY_NUM )
	{
		int tWorldID = tpPlayer->GetNationalityID( );
		if ( tWorldID > WORLD_START && tWorldID < WORLD_NUM )
		{
			CCountry *tpCountry = &(  CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tpPlayer->TempID( ) ] );
			tpPlayer->OfficialID( tpCountry->GetOfficialID( tpPlayer->CharID( ) ) );
			tPlayerEnter.set_officialid( tpPlayer->OfficialID( ) );
			tPlayerEnter.set_kingid( tpCountry->GetKingID() );
			if ( tpPlayer->CharID() == tpCountry->GetKingID() )
			{
				int tIntParam[ 3 ] = { 0 };
				tIntParam[ 0 ] = 1;
				tIntParam[ 1 ] = tWorldID;
				tIntParam[ 2 ] = tpPlayer->TempID( );

				CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(tpPlayer->mCharID);
				CWorldModule::GetSingleton().SendEventNotify( NOTICE_KINGONLINE, 0, 0, tIntParam, ARRAY_CNT(tIntParam), 
						pRoleInfo != NULL ? (char*) pRoleInfo->RoleName() : NULL, NULL );
			}
		}		
	}
	else
	{
		tpPlayer->OfficialID( 0 );
	}
	
	// 查看是否是受追踪的玩家
	bool tRet = CGateLogic::GetSingleton().GetTracePlayerManager().CheckIfRoleIDNeedTrace( tpPlayer->mAccountID, tpPlayer->mCharID );
	tPlayerEnter.set_iftracedplayer( tRet );
		
	// 向场景发送玩家进入地图通知
	//tPlayerEnter.set_commhandle(tpPlayer->mCommHandle);
	tPlayerEnter.set_logintoken(tpPlayer->mLoginToken);
	tPlayerEnter.set_charid(tpPlayer->mCharID); 
	tPlayerEnter.set_accountid(tpPlayer->mAccountID);
	tPlayerEnter.set_adultflag(tpPlayer->mAdultFlag);
	if (tpPlayer->mAdultFlag != 1) //非成年人, 将有效益秒数发送给gameserver
	{
		tPlayerEnter.set_validseconds(tpPlayer->mValidSeconds);
	}
	
	// 给场景发送安全密码
	const char *tpRolePwd = tpPlayer->GetRolePwd( tpPlayer->mCharID );
	if ( tpRolePwd != NULL )
	{
		tPlayerEnter.set_password( tpRolePwd );
	}
	
	// 发送安全保护时间
	tPlayerEnter.set_protecttime( tpPlayer->GetPwdProtectTime( tpPlayer->mCharID ) );
	tPlayerEnter.set_passwordstatus( tpPlayer->GetPasswordStatus( tpPlayer->mCharID ) );
	tPlayerEnter.set_protecttimelength( tpPlayer->GetPwdProtectTimeLength( tpPlayer->mCharID ) );

	pbmsg_sethead(&tSendMessage, ID_G2S_PLAYER_ENTER_MAP_NOTICE, 0, 0, 0, 0, 0, 0);
	tSendMessage.set_msgpara((uint32_t) &tPlayerEnter);
	CGateServer::GetSingleton().Send2Scene( tpHost, &tSendMessage);

	LOG_NOTICE( "default", "[ %s : %d ][ %s ]Player( %d )EnterGame, Scene Is LineID %d MapID %d Pos( %d : %d )", 
		__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, tpPlayer->mLineID, tpPlayer->mMapID, 
		tpPlayer->mPoint.mX, tpPlayer->mPoint.mY );


	tpPlayer->mStatus = CGatePlayer::em_status_ingate_v4scene_entergame;
}

// ***************************************************************
//  Function:		SendDeleteChar2DB
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::SendDeleteChar2DB(CGatePlayer* tpPlayer, int nCharID)
{
	/*
	#ifdef USING_ERATING
	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_erating_passwdverify);
	#else
	*/
	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_idle);
	//#endif

	if (CGateLogic::GetSingleton().ExecuteSql(em_dbsession_removechar, tpPlayer->mAccountID, nCharID, 
				tpPlayer->mAccountID, SQL_NEED_CALLBACK, 
				SELECT, "call ProcRemoveRole(%d, %d)", tpPlayer->mAccountID, nCharID ))
	{
		tpPlayer->mStatus = CGatePlayer::em_status_ingate_v4mysql_removerole;
	}
}

// 向数据库发送删除角色关联数据请求 [12/11/2009 Macro]
void CPlayerManager::SendDelRoleRelationData2DB( int nAccountID, int nRoleID)
{
	char tSQL[ 1024 ] = { 0 };

	// 删除role 相关成就数据
	sprintf( tSQL, "delete from UMS_ACHIEVE where role_id = %d;", nRoleID );
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_del_achieve, nRoleID, 0, 0, SQL_NONE, DELETE, tSQL );

	// 删除评价记录
	CJudgeModule::GetSingleton().OnEventClearRoleJudgeDB( nAccountID, nRoleID );

	return ;
}


// ***************************************************************
//  Function:		CreateCha
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::SendCreateChar2DB(CGatePlayer* tpPlayer, int nRoleID)
{
	LK_ASSERT(tpPlayer != NULL, return);
	int tLineID = CGateServer::GetSingleton().GetMostIdleLineID( CGateLogic::GetSingletonPtr( )->mConfig.nMapID );

	if( tLineID == 0 )
	{
		tLineID = CGateLogic::GetSingletonPtr( )->mConfig.nLineID;
	}

	#ifdef USING_ERATING
	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_erating_createdrole);
	#else
	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_idle);
	#endif

	if (CGateLogic::GetSingleton().ExecuteSql(em_dbsession_createchar, tpPlayer->mAccountID, 0, 
				tpPlayer->mAccountID, SQL_NEED_CALLBACK, SELECT, 
				"select ProcCreateRole( '%d', '%d', '%s', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d' )", 
				nRoleID, tpPlayer->mAccountID, tpPlayer->mCharName, tpPlayer->mSexID, METIER_NOVICE, 
				CGateLogic::GetSingletonPtr( )->mConfig.nCampID, 
				tpPlayer->mFaceType, 
				tLineID,
				CGateLogic::GetSingletonPtr( )->mConfig.nMapID, 
				1, 0 ))
	{
		tpPlayer->mStatus = CGatePlayer::em_status_ingate_v4mysql_createrole;
	}
}


// ***************************************************************
//  Function:		SendObtainChars2DB
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::SendObtainChars2DB(CGatePlayer* tpPlayer)
{
	LK_ASSERT(tpPlayer != NULL, return);


	if (CGateLogic::GetSingleton().ExecuteSql(em_dbsession_obtainchars, tpPlayer->mAccountID, 0, 
				tpPlayer->mAccountID, SQL_NEED_CALLBACK, SELECT,
				"select a.role_id, a.account_id, role_name, gender_id, metier_id, nationality_id, face_type,"
				"line_id, map_id, role_level, role_expr, b.role_repetion, role_password, country_id,  role_deletedtime, lastlogin_ip, role_protecttime from UMS_ROLE a, UMS_ROLE_DETAIL_%02d b, UMS_ACCOUNT c "
				"where a.account_id = %d and a.role_id = b.role_id and c.account_id = a.account_id order by a.create_time",
				tpPlayer->mAccountID%16, tpPlayer->mAccountID ))
	{
		tpPlayer->mStatus = CGatePlayer::em_status_ingate_v4mysql_obtainroles;
	}
}

// ***************************************************************
//  Function:		GetPlayerByAcct
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************

CGatePlayer* CPlayerManager::GetPlayerByAcct(int nAccountID)
{
	PLAYERITER tFindIter = mGatePlayerByAcct.find( nAccountID );
	if ( tFindIter == mGatePlayerByAcct.end( ) )
	{
		return NULL;
	}

	return (CGatePlayer*) CGateObjManager::GetSingletonPtr()->GetObject(tFindIter->second);
}


// ***************************************************************
//  Function:		ClearScenePlayer
//  Description:	清理玩家在场景的数据，同时删除角色索引，但不删除玩家对象
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************

void CPlayerManager::ClearScenePlayer(CGatePlayer* tpPlayer)
{
	if(tpPlayer == NULL) return; 

	// 减少数据
	CWorldManager::GetSingleton().DelPlayerNum( tpPlayer->GetNationalityID(), tpPlayer->LineID(), tpPlayer->MapID() );

	// clear invitelist of player
	tpPlayer->ClearFamilyInvite();

	// force player leave team
	CServiceGateTeam::GetSingleton().ForceLeaveTeam( tpPlayer->mCharID );

	// 删除队伍数据
	CServiceGateTeam::GetSingleton().ClearTeamData( tpPlayer->mCharID );

	// 删除好感数据
	CJudgeModule::GetSingleton().DeleteJudgeRecordMemory( tpPlayer->mCharID );

	// 删除房间
	CCorpservice::GetSingleton().OnEventDeleteRoom( tpPlayer->CharID(), tpPlayer->CountryID() );

	// 如果是族长，清除房间和报名
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpPlayer->mCharID );
	if ( tpFamily != NULL )
	{
		if ( tpPlayer->mCharID == tpFamily->GetRooter() )
		{
			int tMoney = 0;
			int tRet = CFamilyManager::GetSingletonPtr()->DelRoom( tpFamily->GetID(), tMoney );
			if ( tRet == SUCCESS )
			{
				CServiceFamily::GetSingletonPtr()->AddFamilyMoney( tpPlayer->mCharID, tMoney, tpPlayer->mCharName, true );
				tpFamily->SetChallengeStatus( STATE_IDLE );
				tpFamily->SetChallengeMoney( 0 );
			}
			tRet = CFamilyManager::GetSingletonPtr()->ClearCha( tpFamily->GetID() );
		}		
	}	

	// clear repetion 
	for( unsigned int i = 0; i < sizeof( tpPlayer->mRepetionMap ) / sizeof( tpPlayer->mRepetionMap[ 0 ] ); i++ )
	{
		tpPlayer->mRepetionMap[ i ] = INVITED_KEY_VALUE;
	}

	// clear login point
	tpPlayer->mPoint.Clear( );

	// remove index 
	CPlayerManager::GetSingleton().RemoveRoleNameIndex(tpPlayer);
	CPlayerManager::GetSingleton().RemoveRoleIDIndex(tpPlayer);

	// 重置CharID, CharName
	tpPlayer->mCharName[0] = 0;
	tpPlayer->mCharID = 0;
}

// ***************************************************************
//  Function:		DestroyGatePlayer
//  Description:	清理玩家
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::DestroyGatePlayer(CGatePlayer* tpPlayer, int nLeaveReason)
{
	if (tpPlayer == NULL) return;
	
	// 更新玩家的登录IP
	UpDatePlayerAccountInfo( tpPlayer, 0, 0 );
	
	// 通知登陆服务器,玩家离开
	CMessage tMessage;
	CMessagePlayerLeaveGateNotice tPlayerLeaveGate;
	tPlayerLeaveGate.set_accountid( tpPlayer->mAccountID );
	tPlayerLeaveGate.set_reason( nLeaveReason );

	pbmsg_sethead(&tMessage, ID_G2L_PLAYER_LEAVE_GATE_NOTICE, 0, 0, 0, 0, 0, 0); 
	tMessage.set_msgpara((uint32_t) &tPlayerLeaveGate);
	
	if ( tpPlayer->GetRoleWorldID() == CGateServer::GetSingleton().GetWorldID() || tpPlayer->GetRoleWorldID() == 0 )
	{
		CGateServer::GetSingleton().Send2Login( &tMessage);
	}	

	// 删除帐号索引
	RemoveAccountIDIndex(tpPlayer);

	//更新mac地址人数
//	UpdatePlayerNumPerMac(tpPlayer);

	LOG_NOTICE("default", "[ %s : %d ][ %s ] Destroy Account(%d) from Pool", 
			__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID);
	// 删除实体
	CGateObjManager::GetSingletonPtr( )->DestroyObject(tpPlayer->get_id());
}

// ***************************************************************
//  Function:		SendGateErrorNotice
//  Description:	发送错误码和字符串
//	Input:			nErrCode - 错误码； pszErrString - 错误字符串
//	OutPut:			无
//	Return:			无
//	Others:			
//  Date:			10/29/2008
// 
// ***************************************************************
void CPlayerManager::SendGateErrorNotice(CGatePlayer* tpPlayer, int nErrCode,  
		EAfterSendGateError after , const char* pszErrString /*= NULL*/)
{
	LK_ASSERT( tpPlayer != NULL, return);

	CMessage tSendMessage;
	CMessageGateErrorNotice tGateError;

	tGateError.set_errorcode(nErrCode);
	tGateError.set_keepalive( after == em_keep_connect ? 1 : 0);
	if( pszErrString != NULL )
	{
		tGateError.set_errorstring( pszErrString );
	}

	tSendMessage.set_msgpara((uint32_t) & tGateError);
	pbmsg_sethead(&tSendMessage, ID_G2C_NOTIFY_GATEERROR, 0, 0, 0, 0, 0, 0);  

	CGateServer::GetSingleton().Send2Player(tpPlayer, &tSendMessage);

	if ( after == em_need_disconnect)
	{
		UnRegistPlayerComm(tpPlayer);
	}
}

void CPlayerManager::OnlyDisconnectClient(CGatePlayer* tpPlayer)
{
	CMessage tMsg;
	CMessageDisconnectNotify tMsgFact;
	CMessageHead* tpHead = tMsg.mutable_msghead();
	tpHead->set_messageid( ID_S2C_NOTIFY_DISCONNECT );
	tMsg.set_msgpara( (uint32)&tMsgFact );

	CGateServer::GetSingleton().Send2Player(tpPlayer, &tMsg);

	UnRegistPlayerComm(tpPlayer);
}

// ***************************************************************
//  Function:		UnRegistPlayerComm
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::UnRegistPlayerComm(CGatePlayer* tpPlayer)
{
	PLAYERITER iter4Comm = mPlayersConnected.find( tpPlayer->mCommHandle);
	if ( iter4Comm != mPlayersConnected.end( ) && iter4Comm->second == tpPlayer->get_id()) 
	{
		LOG_NOTICE("default", "Erase sockfd(%d) of AccountID(%d) from connected socks", 
					tpPlayer->mCommHandle, tpPlayer->mAccountID);


		mPlayersConnected.erase(iter4Comm);
	}
}

// ***************************************************************
//  Function:		SendObtainCharsProto2ERating
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::SendObtainCharsProto2ERating(CGatePlayer* tpPlayer)
{
	LK_ASSERT(tpPlayer != NULL && tpPlayer->mAccountID != 0, return);

	AGIPRoleList proto;
	proto.setUserID(tpPlayer->mAccountID);

	proto.setGameID(tpPlayer->GetGameID());
	CAGIPSession* pSession = CGateLogic::GetSingleton().CreateSession4Proto(&proto);

	if (pSession != NULL)
	{
		int nSessionID = pSession->get_id();                                                                    

		#ifdef _DEBUG_
		LOG_DEBUG("erating", "Send2ERating: MsgName[AGIPRoleList] MsgSeq[%d] MsgPara[ AccountID(%d) ]",
				  nSessionID, tpPlayer->mAccountID); 
		#endif

		pSession->assign(nSessionID, em_agipsession_obtainroles, tpPlayer->mAccountID, 0, 
				CGateLogic::GetSingleton().mConfig.nEratingSessionTimeout);

	}
}


// ***************************************************************
//  Function:		SendPasswdVerifyProto2ERating
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
/*
void CPlayerManager::SendPasswdVerifyProto2ERating(CGatePlayer* tpPlayer,int nCharID,const char* szPasswd,uint32_t nPasswdType)
{
	LK_ASSERT(tpPlayer != NULL , return);
	AGIPPasswordVerify proto;

	proto.setUserID(tpPlayer->mAccountID);
	proto.setPassword(szPasswd);
	proto.setPasswordType(nPasswdType);

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_idle);
	tpPlayer->mStatus = CGatePlayer::em_status_ingate_v4erating_passwdverify;
		
	CAGIPSession* pSession = CGateLogic::GetSingleton().CreateSession4Proto(&proto);

	if (pSession != NULL)
	{
		int nSessionID = pSession->get_id();                                                                    

		#ifdef _DEBUG_
		LOG_DEBUG("erating", "Send2ERating: MsgName[AGIPPasswordVerify] MsgSeq[%d] "
				"MsgPara[ AccountID(%d) Password(%s) PasswordType(%d)]",
				 nSessionID, tpPlayer->mAccountID, szPasswd, nPasswdType);
		#endif

		pSession->assign(nSessionID, em_agipsession_verifypasswd, tpPlayer->mAccountID, nCharID, 
				CGateLogic::GetSingleton().mConfig.nEratingSessionTimeout);
	}
}
*/

// ***************************************************************
//  Function:		SendCreateCharProto2ERating
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::SendCreateCharProto2ERating(CGatePlayer* tpPlayer)
{
	LK_ASSERT(tpPlayer != NULL , return);

	AGIPCreateRole proto;

	proto.setUserID(tpPlayer->mAccountID);
	proto.setRoleName(tpPlayer->mCharName);
	proto.setGender(tpPlayer->mSexID == GENDER_MALE ? AGIPCreateRole::GENDER_MALE : AGIPCreateRole::GENDER_FEMALE);
	proto.setOccupation(METIER_NOVICE);

	proto.setInitialLevel(1);
	//TODO: 国家ID
	proto.setCommunityID( CGateLogic::GetSingletonPtr( )->mConfig.nCampID );
	proto.setUserIP(tpPlayer->mClientAddr);
	proto.setUserPort(tpPlayer->mClientPort);

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_idle);
	tpPlayer->mStatus = CGatePlayer::em_status_ingate_v4erating_createrole;
		
	proto.setGameID(tpPlayer->GetGameID());
	CAGIPSession* pSession = CGateLogic::GetSingleton().CreateSession4Proto(&proto);

	if (pSession != NULL)
	{
		int nSessionID = pSession->get_id();                                                                    

		#ifdef _DEBUG_
		LOG_DEBUG("erating", "Send2ERating: MsgName[AGIPCreateRole] MsgSeq[%d] MsgPara[ AccountID(%d) "
				"CharName(%s) Gender(%d) Occupation(%d) Level(1) CommunityID(0) ClientIP(%d) ClentPort(%d) ]", 
				 nSessionID, tpPlayer->mAccountID, tpPlayer->GetCharNameShow(), tpPlayer->mSexID, 
				 METIER_NOVICE, tpPlayer->mClientAddr, tpPlayer->mClientPort );
		#endif

		pSession->assign(nSessionID, em_agipsession_createrole, tpPlayer->mAccountID, 0, 
				CGateLogic::GetSingleton().mConfig.nEratingSessionTimeout);

	}
}


// ***************************************************************
//  Function:		SendDeleteRoleProto2ERating
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::SendDeleteRoleProto2ERating(CGatePlayer* tpPlayer, int nRoleID)
{
	LK_ASSERT(tpPlayer != NULL, return);

	AGIPDeleteRole proto ;
	proto.setUserID(tpPlayer->mAccountID);
	proto.setRoleID(nRoleID);

	proto.setGameID(tpPlayer->GetGameID());
	CAGIPSession* pSession = CGateLogic::GetSingleton().CreateSession4Proto(&proto);

	if (pSession != NULL)
	{
		int nSessionID = pSession->get_id();                                                                    

		#ifdef _DEBUG_
		LOG_DEBUG("erating", "Send2ERating: MsgName[AGIPDeleteRole] MsgSeq[%d] MsgPara[ AccountID(%d) RoleID(%d) ]",
				 nSessionID, tpPlayer->mAccountID, nRoleID);
		#endif

		pSession->assign(nSessionID, em_agipsession_deleterole, tpPlayer->mAccountID, nRoleID, 
				CGateLogic::GetSingleton().mConfig.nEratingSessionTimeout);

	}
}

// ***************************************************************
//  Function:		SendResponseDelteRole
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			01/07/2010
// 
// ***************************************************************
void CPlayerManager::SendResponseDelteRole( CGatePlayer *pPlayer, int nErrcode, int nRoleID, int nRoleDeleteStatus, int nDelteStatusLeftTime )
{
	LK_ASSERT( pPlayer != NULL, return )
	//向客户端发送删除成功
	CMessage tMessage;
	CMessageDeleteCharResponse tDeleteChar;
	tDeleteChar.set_charid(nRoleID);
	tDeleteChar.set_errcode(nErrcode);
	tDeleteChar.set_deletestatus( nRoleDeleteStatus );
	tDeleteChar.set_deletestatuslefttime( nDelteStatusLeftTime );
	

	pbmsg_sethead(&tMessage, ID_G2C_RESPONSE_DELETECHAR, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tDeleteChar);

	CGateServer::GetSingleton().Send2Player( pPlayer, &tMessage );			
}

// ***************************************************************
//  Function:		SendResponseDelteRole
//  Description:	更新玩家的信息
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			01/07/2010
// 
// ***************************************************************
void CPlayerManager::UpDatePlayerInfo( CGatePlayer *tpPlayer, int nRoleID, int nParam1, int nParam2 )
{	
	LK_ASSERT( tpPlayer != NULL, return )		
	const char *tpSql = "update UMS_ROLE set role_deletedtime=%d where role_id = %d ";
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_updatedeltetime, nParam1, nParam2, tpPlayer->AccountID(), SQL_NONE, UPDATE, tpSql, tpPlayer->GetRoleDeleteTime( nRoleID ), nRoleID );
}

// ***************************************************************
//  Function:		UpDatePlayerAccountInfo
//  Description:	更新玩家的帐号信息
//	Input:			
//	OutPut:			
//	Return:			
//	Others:
//  Date:			01/25/2010
// 
// ***************************************************************
void CPlayerManager::UpDatePlayerAccountInfo( CGatePlayer *tpPlayer, int nParam1, int nParam2 )
{
	LK_ASSERT( tpPlayer != NULL, return )	
	const char *tpSql = "update UMS_ACCOUNT set lastlogin_ip=%d where account_id = %d ";
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_updateaccount, nParam1, nParam2, tpPlayer->AccountID(), SQL_NONE, UPDATE, tpSql, tpPlayer->mClientAddr, tpPlayer->AccountID() );
}

// ***************************************************************
//  Function:		SendEnterGameProto2ERating
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::SendEnterGameProto2ERating(CGatePlayer* tpPlayer, int nServerID)
{
	LK_ASSERT(tpPlayer != NULL, return);

	/*
	AGIPUserEnterGame proto;
	
	proto.setUserID(tpPlayer->mAccountID);
	proto.setRoleID(tpPlayer->mCharID);
	proto.setRoleLevel(tpPlayer->mLevel);

	proto.setServerID(CGateServer::GetSingleton().GetRegistID() * 100 + nServerID);
	proto.setRatingID(CGateServer::GetSingleton().GetRegistID());

	proto.setUserPort(htons(tpPlayer->mClientPort));
	proto.setUserIP(htonl(tpPlayer->mClientAddr));

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_idle);
	tpPlayer->mStatus = CGatePlayer::em_status_ingate_v4erating_entergame;

	CAGIPSession* pSession = CGateLogic::GetSingleton().CreateSession4Proto(&proto);
	LK_ASSERT(pSession != NULL, return);

	int nSessionID = pSession->get_id();                                                                    

	#ifdef _DEBUG_
	LOG_DEBUG("erating", "Send2ERating: MsgName[AGIPUserEnterGame] MsgSeq[%d] MsgPara[ AccountID(%d) CharID(%d) "
				  " ServerID(%d) RatingID(%d) ClientIP(%d) ClientPort(%d) ]",
				  nSessionID, tpPlayer->mAccountID, tpPlayer->mCharID,
				  CGateServer::GetSingleton().GetRegistID() * 100 + nServerID,
				  CGateServer::GetSingleton().GetRegistID(), 
				  tpPlayer->mClientAddr, tpPlayer->mClientPort);
	#endif

	pSession->assign(nSessionID, em_agipsession_entergame, (int) tpPlayer->mAccountID, 0, 
			CGateLogic::GetSingleton().mConfig.nEratingSessionTimeout);
	*/

	
	AGIPRoleEnterGameEx proto;

	proto.setGameID(tpPlayer->GetGameID());
	proto.setGatewayID(CGateServer::GetSingleton().GetRegistID());
	proto.setServerID(CGateServer::GetSingleton().GetRegistID() * 100 + nServerID);

	proto.setUserID(tpPlayer->mAccountID);
	proto.setRoleID(tpPlayer->mCharID);
	proto.setLevel(tpPlayer->mLevel);

	proto.setGender(tpPlayer->mSexID == GENDER_MALE ? AGIPCreateRole::GENDER_MALE : AGIPCreateRole::GENDER_FEMALE);
	proto.setOccupationID(tpPlayer->mMetierID);
	//proto.setCorpsID(0);
	proto.setCommunityID(tpPlayer->mCountryID);

	proto.setClientPort(htons(tpPlayer->mClientPort));
	proto.setClientIP(htonl(tpPlayer->mClientAddr));
	proto.setClientMAC(tpPlayer->GetMac());

	char buffer[32+1];
	sprintf(buffer, "%u", tpPlayer->GetCpuID());
	proto.setHardwareSN1(buffer);

	sprintf(buffer, "%u", tpPlayer->GetDiskID());
	proto.setHardwareSN2(buffer);

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4loginqueue);
	// tpPlayer->mStatus = CGatePlayer::em_status_ingate_v4erating_entergame;

	CAGIPSession* pSession = CGateLogic::GetSingleton().CreateSession4Proto(&proto);
	if(pSession == NULL) 
	{
		//TODO: 将等待队列中的玩家状态更改为未被处理	
		return;
	}

	int nSessionID = pSession->get_id();                                                                    

	#ifdef _DEBUG_
	LOG_DEBUG("erating", "Send2ERating: MsgName[AGIPRoleEnterGame] MsgSeq[%d] MsgPara[ AccountID(%d) CharID(%d) "
				  " ServerID(%d) RatingID(%d) ClientIP(%d) ClientPort(%d), GameID(%d), CPUID(%u) DISKID(%u)  ]",
				  nSessionID, tpPlayer->mAccountID, tpPlayer->mCharID,
				  CGateServer::GetSingleton().GetRegistID() * 100 + nServerID,
				  CGateServer::GetSingleton().GetRegistID(), 
				  tpPlayer->mClientAddr, tpPlayer->mClientPort,
				  tpPlayer->GetGameID(), tpPlayer->GetCpuID(), tpPlayer->GetDiskID()
				  );
	#endif

	pSession->assign(nSessionID, em_agipsession_entergame, (int) tpPlayer->mAccountID, 0, 
			CGateLogic::GetSingleton().mConfig.nEratingSessionTimeout);

}


// ***************************************************************
//  Function:		SendUserLogoutProto2ERating
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::SendUserLogoutProto2ERating(CGatePlayer* tpPlayer, CMessagePlayerLeaveMapNotice* tpPlayerLeave, int nFlag)
{
	LK_ASSERT(tpPlayer != NULL && tpPlayerLeave != NULL, return);

	AGIPUserLogout proto; 

	proto.setUserID(tpPlayer->mAccountID);
	proto.setRoleID(tpPlayerLeave->charid()); 
	proto.setRatingID(CGateServer::GetSingleton().GetRegistID());

	// very important
	proto.setLogoutFlag(tpPlayerLeave->charlevel() == 0 ? 3 : nFlag);

	proto.setRoleLevel(tpPlayerLeave->charlevel());
	proto.setOccupation(tpPlayerLeave->charmetier());
	proto.setMoney1(tpPlayerLeave->money1());
	proto.setMoney2(tpPlayerLeave->money2());
	proto.setExperience(tpPlayerLeave->exp());

	// 新增退出时候报告国家
	// proto.setOccupation(tpPlayer->mCountryID);

	proto.setGameID(tpPlayer->GetGameID());
	CAGIPSession* pSession = CGateLogic::GetSingleton().CreateSession4Proto(&proto);

	if (pSession != NULL)
	{
		int nSessionID = pSession->get_id();                                                                    

		#ifdef _DEBUG_
		LOG_DEBUG("erating", "Send2ERating: MsgName[AGIPUserLogout] MsgSeq[%d] MsgPara[ AccountID(%d) "
				"RoleID(%d) RoleName(%s) RoleLevel(%d) RoleMetier(%d)]", nSessionID, tpPlayer->mAccountID, 
				tpPlayer->mCharID, tpPlayer->GetCharNameShow(), tpPlayerLeave->charlevel(), tpPlayerLeave->charmetier());
		#endif

		pSession->assign(nSessionID, em_agipsession_userlogout, tpPlayer->mAccountID, tpPlayer->mCharID, 
				CGateLogic::GetSingleton().mConfig.nEratingSessionTimeout);

	}

	if ( nFlag == 1)
	{
		if( IS_REPETION_LINE_ID( tpPlayer->mLineID ) == true )
		{
			// repetion destroy
			CRepetionModule::GetSingletonPtr( )->PlayerOutRepetion( tpPlayer, tpPlayer->mMapID );
		}
	}
}

void CPlayerManager::ObtainCharsWrapper(int vCommHandle)
{
	// 验证是否合法玩家
	CGatePlayer* tpPlayer = GatePlayer( vCommHandle );
	if (tpPlayer == NULL) return;
	
	// 如果是其他服务器的玩家不处理
	if ( tpPlayer->GetRoleWorldID() != CGateServer::GetSingleton().GetWorldID() && tpPlayer->GetRoleWorldID() > 0 )
	{
		LOG_INFO("default", "player ( accountid:%d, roleworldid:%d ) will use role(roleid:%d) to enter game", tpPlayer->AccountID(), tpPlayer->GetRoleWorldID(), tpPlayer->CharID() );
		return;
	}

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_idle);

	#ifdef USING_ERATING
	if ( CGateServer::GetSingleton().IsERatingLaunched() == true)
	{
		// 只是发一下包, 啥也不做
		// 应erating要求取消发送
		//SendObtainCharsProto2ERating(tpPlayer);
	}
	#endif

	// 不使用ERating返回的玩家列表
	SendObtainChars2DB(tpPlayer);	

}

void CPlayerManager::CreateCharWrapper(int vCommHandle, const char* szCharName, int nSexID, int nFaceType)
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GatePlayer( vCommHandle );
	if (tpPlayer == NULL) return ;


	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_idle);

	// 验证charname的长度	
	if (strlen(szCharName) > NAME_LENGTH)
	{
		SendGateErrorNotice(tpPlayer, EGATE_ROLENAME_IS_TOOLONG, em_keep_connect);
		return ;
	}

	// 如果不是GM 验证名称是否合法
	if ( (tpPlayer->mUserStatus & GM_STATE) != GM_STATE )  // TODO:必须和场景一致
	{
		char szDirtyName[128] = {0};
		strncpy(szDirtyName, szCharName, sizeof(szDirtyName) -1 );
		if (CDirtyWord::GetSingleton().OI_Dirty_Check( 3, szDirtyName) != 0)
		{
			SendGateErrorNotice(tpPlayer, EGATE_ROLENAME_HAS_INVALIDCHAR, em_keep_connect);
			return ;
		}
	}

	CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleName(szCharName);
	if ( pRoleInfo != NULL )
	{
		SendGateErrorNotice(tpPlayer, EGATE_CREATECHAR_ON_DUPNAME, em_keep_connect);
		return ;
	}

	
	// 验证sex的范围
	if ( nSexID != GENDER_FEMALE && nSexID != GENDER_MALE)
	{
		SendGateErrorNotice(tpPlayer, EGATE_ROLESEX_ISNOT_PERMITED, em_keep_connect);
		return ;
	}

	//暂时用tpPlayer来存储变量
	strncpy(tpPlayer->mCharName, szCharName, sizeof(tpPlayer->mCharName));
	tpPlayer->mCharName[sizeof(tpPlayer->mCharName)-1] = 0;

	tpPlayer->mCountryID = CGateLogic::GetSingletonPtr( )->mConfig.nCampID;
	tpPlayer->mFaceType = nFaceType;
	tpPlayer->mSexID = nSexID;

	#ifdef USING_ERATING
	if (CGateServer::GetSingleton().IsERatingLaunched() == true)
	{
		SendCreateCharProto2ERating(tpPlayer);
	}else
	{
		SendGateErrorNotice(tpPlayer, EGATE_CREATECHAR_ON_AGNOTOPEN, em_keep_connect);
		return ;
	}
	#else
	SendCreateChar2DB(tpPlayer, 0);
	#endif
}

void CPlayerManager::DeleteCharWrapper(int vCommHandle, int nCharID, const char* szPasswd, int nPasswdType)
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GatePlayer( vCommHandle );
	if(tpPlayer == NULL) return;

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_idle);
	
	// 如果有密码，则进行密码验证
	if ( tpPlayer->CheckRolePwd( nCharID, szPasswd ) != 0 )
	{
		SendGateErrorNotice(tpPlayer, EGAME_DELETEROLE_WRONGPASSWORD, em_keep_connect);
		return;
	}	
	
	// 家族首领不能删除角色
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nCharID );
	if ( tpFamily != NULL )
	{
		if ( tpFamily->GetRooter() == (int)nCharID )
		{
			// 发送家族首领不能删除角色的通知
			SendGateErrorNotice(tpPlayer, EGAME_DELETEROLE_ON_FAMILYHEADER, em_keep_connect);
			return;
		}			
	}
	
	// 删除状态的判断
	int tDeleteProtecttime = CGateLogic::GetSingleton().mConfig.nDeleteProtectTime;
	int tDeleteAllowedtime = CGateLogic::GetSingleton().mConfig.nDeleteAllowedTime;
	int tDeleteCooltime	   = CGateLogic::GetSingleton().mConfig.nDeleteCoolTime;
	int tDeleteProtectLevel = CGateLogic::GetSingleton().mConfig.mDeleteLevel;
	int tDeleteStatusLeftTime = 0;
	int tDeleteStatus =	tpPlayer->GetRoleDeleteStatus( nCharID, tDeleteStatusLeftTime, tDeleteProtecttime, tDeleteAllowedtime, tDeleteCooltime, tDeleteProtectLevel );
	switch( tDeleteStatus )
	{
		// 正常状态，删除状态设置成功
		case ROLEDELETESTATUS_LIVE:
		{ 			
			tpPlayer->SetRoleDleteTime( nCharID, time( NULL ) );			
			SendResponseDelteRole( tpPlayer, EGAME_DELETEROLE_DELETESUCCESS, nCharID, ROLEDELETESTATUS_DELETEPROTECT, tDeleteProtecttime );
			CPlayerManager::GetSingleton().UpDatePlayerInfo( tpPlayer, nCharID, 0, 0 );
			return;
		}
		
		// 删除保护状态不允许删除
		case ROLEDELETESTATUS_DELETEPROTECT:
		{
			SendGateErrorNotice(tpPlayer, EGAME_DELETEROLE_DELETEPROTECT, em_keep_connect);
			return;
		}
		
		// 删除允许状态，可以删除
		case ROLEDELETESTATUS_DELETEDALLOWED:
		{  			
			break;
		}
		
		// 等级低，删除不受限制
		case ROLEDELETESTATUS_LEVELLOW:
		{
			break;
		}
				
		// 删除操作冷却状态，不可以进行改操作
		case ROLEDELETESTATUS_DELETECOOLTIME:
		{
			SendGateErrorNotice(tpPlayer, EGAME_DELETEROLE_DELETECOOLTIME, em_keep_connect);
			return;
		}	
	}
	
	// 删除家族成员
	if ( tpFamily != NULL )
	{
		tpFamily->Del4Member( nCharID );
	}
	
	bool bFind = false;
	for ( CGatePlayer::SaveList::iterator it = tpPlayer->mSave.begin(); it != tpPlayer->mSave.end(); ++it )
	{
		if ( (*it).mCharID == (unsigned int) nCharID )
		{
			bFind = true;
			break;
		}
	}	

	if (bFind == true)
	{
		/*
		#ifdef USING_ERATING
		if (CGateServer::GetSingleton().IsERatingLaunched() == true)
		{
			SendPasswdVerifyProto2ERating(tpPlayer, nCharID, szPasswd, nPasswdType);
		}else
		{
			SendGateErrorNotice(tpPlayer, EGATE_DELETEROLE_ON_AGNOTOPEN, em_keep_connect);
			return;
		}
		#else
		*/
		SendDeleteChar2DB(tpPlayer, nCharID );
		//#endif
		
	}else
	{
		SendGateErrorNotice(tpPlayer, EGATE_DELETEROLE_ON_DBSQL, em_keep_connect);
		return ;
	}

	return ;
}

void CPlayerManager::PlayerEnterGameWrapper(int nAccountID, int nRoleID, const char* szMac)
{
	CGatePlayer* tpPlayer = GetPlayerByAcct(nAccountID);
	if (tpPlayer == NULL) return ;	  	
	
	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_idle);
	
	//　判断角色的删除状态
	int tDeleteStatusLeftTime = 0;
	int tDeleteProtecttime = CGateLogic::GetSingleton().mConfig.nDeleteProtectTime;
	int tDeleteAllowedtime = CGateLogic::GetSingleton().mConfig.nDeleteAllowedTime;
	int tDeleteCooltime	   = CGateLogic::GetSingleton().mConfig.nDeleteCoolTime;
	int tDeleteLevel = CGateLogic::GetSingleton().mConfig.mDeleteLevel;
	int tDeleteStatus = tpPlayer->GetRoleDeleteStatus( nRoleID, tDeleteStatusLeftTime, tDeleteProtecttime, tDeleteAllowedtime, tDeleteCooltime, tDeleteLevel );
	if ( tDeleteStatus == ROLEDELETESTATUS_DELETEPROTECT || tDeleteStatus == ROLEDELETESTATUS_DELETEDALLOWED )
	{
		SendGateErrorNotice(tpPlayer, EGATE_ROLE_ENTERGAME_ONDELETESTATUS, em_keep_connect);
		return;
	}

	bool bFind = false;
	for ( unsigned int i = 0; i < tpPlayer->mSave.size(); i++ )
	{
		if ( (unsigned int) nRoleID == tpPlayer->mSave[ i ].mCharID )
		{
			tpPlayer->mCharID = tpPlayer->mSave[ i ].mCharID;
			tpPlayer->mLevel = tpPlayer->mSave[ i ].mLevel;
			tpPlayer->mFaceType = tpPlayer->mSave[ i ].mFaceType; 

			tpPlayer->mCountryID = tpPlayer->mSave[i].mCountryID;
			tpPlayer->mSexID = tpPlayer->mSave[i].mSexID;
			tpPlayer->mMetierID = tpPlayer->mSave[i].mMetierID;
			tpPlayer->mTempCoID = tpPlayer->mSave[i].mTempID;

			
			if( IS_REPETION_LINE_ID( tpPlayer->mSave[ i ].mLineID ) != true )
			{
				tpPlayer->mLineID = tpPlayer->mSave[ i ].mLineID;
				tpPlayer->mMapID = tpPlayer->mSave[ i ].mMapID;
//				tpPlayer->mPoint.Clear( );
			}else
			{
				int tRtn = SUCCESS;
				tRtn = CRepetionModule::GetSingletonPtr()->PlayerIntoRepetion(
						tpPlayer, tpPlayer->mSave[ i ].mMapID, 0, tpPlayer->mSave[ i ].mKey, NULL );


				if( tRtn == SUCCESS )
				{
					tpPlayer->mLineID = tpPlayer->mSave[ i ].mLineID;
					tpPlayer->mMapID = tpPlayer->mSave[ i ].mMapID;
					tpPlayer->mPoint.Clear( );
				}else
				{
					tpPlayer->mLineID = tpPlayer->mSave[ i ].mBackLineID;
					tpPlayer->mMapID  = tpPlayer->mSave[ i ].mBackMapID;
					tpPlayer->mPoint.Clear( );
					tpPlayer->mPoint.mX = tpPlayer->mSave[ i ].mPosX;
					tpPlayer->mPoint.mY = tpPlayer->mSave[ i ].mPosY;
				}
			}
		
			bFind = true;
		}
	}

	// 解决删除的角色有可能登录的bug
	if (bFind == false)
	{
		SendGateErrorNotice(tpPlayer, EGATE_ROLE_DOESNOT_EXIST, em_need_disconnect);
		return;
	}

	if (mBlackList.Find(nRoleID))
	{
		SendGateErrorNotice(tpPlayer, EM_KICKOUT_LOCKDATA, em_keep_connect);
		const char* pStr = mBlackList.GetStr(nRoleID);
		if (pStr == NULL)
		{
			LOG_ERROR("default", "string is NULL? role=%d", nRoleID );
			return;
		}
		int role_id=0;
		int svr_id=0;
		sscanf(pStr, "%d %d", &role_id, &svr_id );
		if ( svr_id <= 0 )
		{
			LOG_ERROR("default", "svr_id is %d?", svr_id );
			return;
		}
		// 请求场景 重新存盘
		CMessageReSavePlayerRequest msg;
		msg.set_roleid(nRoleID);
		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2S_REQUEST_RESAVEPLAYER, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &msg);

		LOG_NOTICE("default", "ask scene %d resave player %d", svr_id, nRoleID );
		CGateServer::GetSingleton().Send2Scene( svr_id, &tMessage);
		return ;
	}
	if ( IS_DUNGEONSERVER_LINE( tpPlayer->mLineID ) )
	{
		// 存成新手村的地图
		CTemplateBeginner* tpBeginner = (CTemplateBeginner*) CDataStatic::GetTemp( TEMP_BEGINNER );
		if ( tpBeginner != NULL )
		{								
			tpPlayer->mPoint = CWTPoint( tpBeginner->mPosX1, tpBeginner->mPosY1 );			
			tpPlayer->LineID( tpPlayer->GetNationalityID() );
			tpPlayer->MapID( tpBeginner->mMapID );	
		}
	}
	if ( IS_GLOBAL_SCENESERVER( tpPlayer->mLineID ) )
	{
		CTemplateGlobalMapConfig *tpGlobalConfig = ( CTemplateGlobalMapConfig * )CDataStatic::GetTemp( TEMP_GLOBALMAPCONFIG );
		if ( tpGlobalConfig != NULL )
		{
			tpPlayer->mPoint = CWTPoint( tpGlobalConfig->mOutX, tpGlobalConfig->mOutY );
			tpPlayer->LineID( tpPlayer->GetNationalityID() );
			tpPlayer->MapID( tpGlobalConfig->mOutMap );			
		}
	}

	CSceneServer* tpHost = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
	if (tpHost == NULL)
	{
		LOG_ERROR("default",  "[ %s : %d ][ %s ] Can NOT find player(%d)'s Scene by Line:%d, Map:%d", 
				__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, tpPlayer->mLineID, tpPlayer->mMapID );

		SendGateErrorNotice(tpPlayer, EGATE_THESCENE_ISNOT_ONLINE, em_keep_connect);
		if( IS_REPETION_LINE_ID( tpPlayer->mLineID ) == true )
		{
			CRepetionModule::GetSingletonPtr( )->PlayerOutRepetion( tpPlayer, tpPlayer->mMapID );
		}

		return ;
	}
	//if( szMac != NULL&& strcmp(szMac, "0") != 0 )
	//{
	//	MACPLAYERNUM::iterator tIterMac = mMacPlayerNum.find(szMac);
	//	if( tIterMac == mMacPlayerNum.end() )
	//	{
	//		MACACCOUNTVECTOR tVector;
	//		tVector.initailize();
	//		tVector.push_back( nAccountID );
	//		mMacPlayerNum.insert( MACPLAYERNUM::value_type(szMac,tVector ) );
	//	}else{
	//		if( tIterMac->second.size()  <= 3 )
	//		{
	//			bool nHas = false;
	//			//tIterMac->second++;
	//			for(int i = 0; i < tIterMac->second.size() ;i++)
	//			{
	//				if ( tIterMac->second[i] == nAccountID )
	//				{
	//					nHas = true;
	//				}
	//			}
	//			if( !nHas && tIterMac->second.size() < 3)
	//			{
	//				tIterMac->second.push_back(nAccountID);
	//			}
	//			else if( !nHas &&tIterMac->second.size() == 3 )
	//			{
	//				SendGateErrorNotice(tpPlayer, EM_KICKOUT_ANTIBOT_MULTICLIENT, em_need_disconnect );
	//				LOG_NOTICE("default", "[ %s : %d ][ %s ]on mac playernum > 3 tpPlayer(%d) ",
	//					__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID );
	//				return;
	//			}

	//		}else{ //如果一个mac下已经有3个

	//			SendGateErrorNotice(tpPlayer, EM_KICKOUT_ANTIBOT_MULTICLIENT, em_need_disconnect );
	//			LOG_NOTICE("default", "[ %s : %d ][ %s ]on mac playernum > 3 tpPlayer(%d) ",
	//				__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID );
	//			return;
	//		}
	//	}
	//}

	// 设置mac
	tpPlayer->SetMac(szMac);

	// 进入登录排队队列
	QueuePlayer(tpPlayer, tpHost);

	/*
	if (tpHost->mPlayerNumber >= tpHost->mSizePermited)
	{
		QueuePlayer(tpPlayer, tpHost);
		return ;
	}

	#ifdef USING_ERATING
	if (CGateServer::GetSingleton().IsERatingLaunched() == true)
	{
		SendEnterGameProto2ERating(tpPlayer, tpHost->mServerID);
	}else
	{
		PlayerEnterGame(tpPlayer);
	}
	#else
	PlayerEnterGame(tpPlayer);
	#endif
	*/

	return ;
}

void CPlayerManager::EnterGate(int nCommHandle, int nAccountID, int nLoginToken, int nAdultFlag, int nUserStatus, int nGameID, unsigned int nCpuID, unsigned int nDiskID)
{
	CMessagePlayerEnterGateResponse tPlayerEnter;

	tPlayerEnter.set_commhandle(nCommHandle);
	tPlayerEnter.set_accountid(nAccountID); 
	tPlayerEnter.set_logintoken(nLoginToken );
	tPlayerEnter.set_worldid(CGateServer::GetSingleton().GetRegistID());

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tPlayerEnter);
	pbmsg_sethead(&tMessage, ID_G2L_PLAYER_ENTER_GATE_RESPONSE, 0, 0, 0, 0, 0, 0);	

	LOG_NOTICE( "default", "player account %d enter gate", nAccountID );	
	// 如果玩家在全局场景则踢掉玩家	
	CGlobalData *tpData = CPlayerManager::GetSingleton().GetGlobalData( nAccountID );
	if ( tpData != NULL )
	{	
		
		if (  ( time( NULL ) - tpData->mTeleTime ) <= 30 )
		{
			LOG_NOTICE("default", "account %d is in global scene, kick it", nAccountID);		
			tPlayerEnter.set_errorcode(ELOGIN_PLAYER_IN_GAMING);
			tPlayerEnter.set_otherclient(0);
			tPlayerEnter.set_logintoken(0);
			CGateServer::GetSingleton().Send2Login( &tMessage);
			return;
		}		
		else
		{
			LOG_NOTICE( "default", "kick account %d ", nAccountID );	
			KickPlayerInGlobalScene( nAccountID );
		}
	}
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nAccountID);
	if(tpPlayer != NULL)
	{
		unsigned char* p = (unsigned char*) &(tpPlayer->mClientAddr);
		LOG_NOTICE("default", "[ %s : %d ][ %s ] Account(%d) has logined at( %d.%d.%d.%d) %d minutes ago, it's status(%x)", 
				__LK_FILE__, __LINE__, __FUNCTION__, 
				nAccountID, *(p), *(p+1), *(p+2), *(p+3), (time(0)-tpPlayer->mSockTime)/60, tpPlayer->mStatus);							
		tPlayerEnter.set_errorcode(ELOGIN_PLAYER_IN_GAMING);		
		tPlayerEnter.set_otherclient(tpPlayer->mClientAddr);
		tPlayerEnter.set_logintoken(tpPlayer->mLoginToken);
	}else
	{
		tpPlayer = (CGatePlayer*) CGateObjManager::GetSingletonPtr()->CreateObject(OBJTYPE_PLAYER_AT_GATE);
		if( tpPlayer == NULL) 
		{
			LOG_FATAL("default", "Player object pool can NOT malloc an object!");

			tPlayerEnter.set_errorcode(ELOGIN_THEWORLD_IS_FULLLIST);
			CGateServer::GetSingleton().Send2Login( &tMessage );
			return ;
		}
				
		tpPlayer->mLoginToken = nLoginToken;
		tpPlayer->mCharName[0] = 0;
		tpPlayer->mCharID = -1;
		tpPlayer->mAccountID = nAccountID;
		tpPlayer->mCommHandle = 0;

		tpPlayer->mStatus = CGatePlayer::em_status_inwait_wait;
		tpPlayer->mCheckTimer = CGateLogic::GetSingleton().mConfig.nPlayerWaitConnectTimeout;

		tpPlayer->SetGameID(nGameID);

		tpPlayer->mLineID = -1;
		tpPlayer->mMapID = -1;

		if ( CGateLogic::GetSingleton().mConfig.bFcmServiceOn == true)
		{
			tpPlayer->mAdultFlag = nAdultFlag ;
		}else
		{
			tpPlayer->mAdultFlag = 1;
		}
		
		tpPlayer->mUserStatus= nUserStatus;

		tpPlayer->SetDiskID(nDiskID);
		tpPlayer->SetCpuID(nCpuID);


		LOG_INFO("default", "[ %s : %d ][ %s ] Gateserver is prepare Account(%d) token(%d) objid(%d)",
						__LK_FILE__, __LINE__, __FUNCTION__, nAccountID, nLoginToken, tpPlayer->get_id());

		pair<PLAYERITER,bool> ret =	mGatePlayerByAcct.insert( 
				PLAYERCONTAINER::value_type(tpPlayer->mAccountID, tpPlayer->get_id()) );

		if (ret.second == false)
		{
			LOG_FATAL("default", "[ %s : %d ][ %s ] Insert Account(%d) Duplicate ,use objid(%d) replace objid(%d)", 
					__LK_FILE__, __LINE__, __FUNCTION__, 
					tpPlayer->mAccountID, tpPlayer->get_id(), ret.first->second );

			ret.first->second =  tpPlayer->get_id();
		}

		tPlayerEnter.set_errorcode(0);

	}

	if ( tpPlayer->GetRoleWorldID() == CGateServer::GetSingleton().GetWorldID() || tpPlayer->GetRoleWorldID() == 0 )
	{
		CGateServer::GetSingleton().Send2Login( &tMessage);
	}	
	return ;
}

void CPlayerManager::RemovePlayerInScene(CSceneServer* pHost)
{
	// 删除场景下地图上的玩家
	vector< int > tKickPlayers;
	PLAYERCONTAINER::iterator it4player = mPlayerInSceneByChar.begin( );
	for(; it4player != mPlayerInSceneByChar.end(); ++it4player)
	{
		CGatePlayer* tpPlayer = (CGatePlayer*) CGateObjManager::GetSingletonPtr()->GetObject(it4player->second);
		if(tpPlayer == NULL) continue;

		// 找到在该场景的所有玩家，把他们都踢掉
		CSceneServer* tpServer = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );

		if (pHost == tpServer) 
		{
			tKickPlayers.push_back(tpPlayer->mAccountID);
		}
	}
	
	// 模拟玩家下线消息 
	for ( size_t i = 0; i < tKickPlayers.size( ); i ++ )
	{		
		CGatePlayer* tpPlayer = GetPlayerByAcct( tKickPlayers[i]);
		LK_ASSERT(tpPlayer != NULL,  continue);

		// 清理场景玩家脏数据
		ClearScenePlayer(tpPlayer);

		#ifdef USE_LONG_CONNECTION
		//SendGateErrorNotice(tpPlayer, EGATE_PLAYER_WILL_DISCONNECT, em_need_disconnect);
		OnlyDisconnectClient(tpPlayer);
		#endif

		// 销毁玩家数据
		DestroyGatePlayer(tpPlayer);

		/*
		// 删除名字索引
		RemoveRoleNameIndex(tpPlayer);

		// 删除角色索引
		RemoveRoleIDIndex(tpPlayer);

		// 删除帐号索引
		RemoveAccountIDIndex(tpPlayer);

		// 销毁实体之前进行赋值
		CMessage tMessage;
		CMessagePlayerLeaveGateNotice tPlayerLeaveGate;
		tPlayerLeaveGate.set_accountid(tpPlayer->mAccountID );
		pbmsg_sethead(&tMessage, ID_G2L_PLAYER_LEAVE_GATE_NOTICE, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &tPlayerLeaveGate);

		// 删除实体
		CGateObjManager::GetSingletonPtr( )->DestroyObject(tpPlayer->get_id());

		// 通知LOGIN玩家销毁
		CGateServer::GetSingleton().Send2Login( &tMessage ); 
		*/
	}

	return;
}


// ***************************************************************
// --- CCoreService 实现
// ***************************************************************
template<> CCoreService* CSingleton< CCoreService>::spSingleton = NULL;
CCoreService::CCoreService()
{
	if( mShmPtr->GetInitMode() == SHM_INIT )        
	{
		Initialize();
	}else
	{
		Resume();
	}
}

CCoreService::~CCoreService()
{

}

unsigned int CCoreService::CountSize()
{
	unsigned int tSize = sizeof( CCoreService );
	tSize += CPlayerManager::CountSize(); 
	return tSize;
}

int CCoreService::Initialize()
{

	return 0;
}

int CCoreService::Resume()
{
	return 0;
}

void* CCoreService::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CCoreService::operator delete( void* pointer )
{

}


// ***************************************************************
//  Function:		OnAGIPMsgCreateRoleResponse
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnAGIPMsgCreateRoleResponse(SysProtocol* pSysProto, int nParam1, int nParam2)
{
	LK_ASSERT(pSysProto!= NULL, return);
	AGIPCreateRoleRes* pprotores = (AGIPCreateRoleRes*) pSysProto;

	int nAccountID = nParam1;
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nAccountID);

	if(tpPlayer == NULL)
	{
		LOG_NOTICE("erating", "[ %s : %d ][ %s ] AccountID(%d) has left.", 
				__LK_FILE__, __LINE__, __FUNCTION__, nAccountID);

		return;
	}

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4erating_createrole);
	tpPlayer->mStatus = CGatePlayer::em_status_ingate_erating_createdrole;

	int32_t nResultCode = 0;
	unsigned int nCommandID = 0;
	pSysProto->getCommandID(&nCommandID);
	LK_ASSERT(nCommandID == CMD_CREATE_ROLE_RES, return);

	pprotores->getResultCode(&nResultCode);
	if (nResultCode == S_SUCCESS)
	{
		uint32_t nRoleID = 0;
		pprotores->getRoleID(&nRoleID);

		CPlayerManager::GetSingleton().SendCreateChar2DB(tpPlayer, nRoleID);
	}else
	{
		CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, nResultCode, em_keep_connect);
		tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
		tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);
	}
}

// ***************************************************************
//  Function:		OnAGIPMsgDeleteRoleResponse
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnAGIPMsgDeleteRoleResponse(SysProtocol* pSysProto, int nParam1, int nParam2)
{
	LK_ASSERT(pSysProto!= NULL, return);
	AGIPDeleteRoleRes* pprotores = (AGIPDeleteRoleRes*) pSysProto;
	int nAccountID = nParam1;
	int nRoleID = nParam2;

	if (pprotores != NULL)
	{
		unsigned int nCommandID = 0;
		pSysProto->getCommandID(&nCommandID);
		LK_ASSERT(nCommandID == CMD_DELETE_ROLE_RES, return);

		int32_t nResultCode = 0;
		pprotores->getResultCode(&nResultCode);
		if (nResultCode == S_SUCCESS)
		{
			LOG_NOTICE("erating", "[ %s : %d ][ %s ] AccountID (%d) remove RoleID(%d) from ERatingServer", 
					__LK_FILE__, __LINE__, __FUNCTION__, nAccountID, nRoleID);
		}else
		{
			LOG_ERROR("erating", "[ %s : %d ][ %s ] AccountID (%d) remove RoleID(%d) from Erating failed(%d).",
					__LK_FILE__, __LINE__, __FUNCTION__, nAccountID, nRoleID, nResultCode);
		}
	}
}


// ***************************************************************
//  Function:		OnAGIPMsgUserLogoutResponse
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnAGIPMsgUserLogoutResponse(SysProtocol* pSysProto, int nParam1, int nParam2)
{
	LK_ASSERT(pSysProto!= NULL, return);

	AGIPUserLogoutRes* pprotores = (AGIPUserLogoutRes*) pSysProto;
	if (pprotores != NULL)
	{
		unsigned int nCommandID = 0;
		pSysProto->getCommandID(&nCommandID);
		LK_ASSERT(nCommandID == CMD_USER_LOGOUT_RES, return);
	
		int32_t nResultCode = 0;
		pprotores->getResultCode(&nResultCode);
		if (nResultCode == S_SUCCESS)
		{
			LOG_DEBUG("erating", "OnAGIPMsgUserLogoutResponse OK");
		}else
		{
			LOG_ERROR("erating", "OnAGIPMsgUserLogoutResponse fault(%d)", nResultCode);
			return ;
		}

		int32_t nBalanceCount;
		if (pprotores->getBalanceCount(&nBalanceCount) != S_SUCCESS) return;

		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct( nParam1);
		if (tpPlayer == NULL) return;

		// 如果玩家不在游戏中，则退出
		if ( tpPlayer->mStatus !=  CGatePlayer::em_status_ingame_idle) return;

		/*
		CMessage tMessage;
		CMessageFangChenMiResponse tFactMsg;

		tFactMsg.set_charid(tpPlayer->mCharID);
		for (int i = 0; i < nBalanceCount; ++i)
		{
			int nSubjectID = 0;
			int nAmount= 0;

			if (pprotores->getBalanceInfo(i, &nSubjectID, &nAmount) == S_SUCCESS)
			{
				// 非成年人才加入防沉迷系统
				if ( nSubjectID == 100 && tpPlayer->mAdultFlag != 1)
				{
					PBSubjectAmount *pSubject = tFactMsg.add_subjects();
					pSubject->set_subjectid(nSubjectID);
					pSubject->set_amount(nAmount);

					//CServiceGateFcm::GetSingleton().UpdateFcmPlayer(tpPlayer->mCharID, nAmount);
				}
			}
		}

		pbmsg_sethead(&tMessage, ID_G2S_FANGCHENMI_RESPONSE, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &tFactMsg);

		CGateServer::GetSingleton().Send2Scene(tpPlayer, &tMessage);
		*/
	}
}

// ***************************************************************
//  Function:		OnAGIPMsgEnterGameResponse
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnAGIPMsgEnterGameResponse(SysProtocol* pSysProto, int nParam1, int nParam2)
{
	LK_ASSERT(pSysProto!= NULL, return);
	AGIPRoleEnterGameExRes* pprotores = (AGIPRoleEnterGameExRes*) pSysProto;
	int nAccountID = nParam1;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nAccountID);
	if(tpPlayer == NULL)
	{
		LOG_NOTICE("erating", "[ %s : %d ][ %s ] AccountID(%d) has left", 
				__LK_FILE__, __LINE__, __FUNCTION__, nAccountID);

		return;
	}

	/*
	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4erating_entergame);
	tpPlayer->mStatus = CGatePlayer::em_status_ingate_erating_enteredgame;
	*/

	if (pprotores != NULL)
	{
		unsigned int nCommandID = 0;
		pSysProto->getCommandID(&nCommandID);
		LK_ASSERT(nCommandID == CMD_ROLE_ENTER_GAME_EX_RES, return);

		int32_t nResultCode = 0;
		pprotores->getResultCode(&nResultCode);
		if (nResultCode == S_SUCCESS)
		{
			int32_t nBalanceCount = 0;
			pprotores->getBalanceCount(&nBalanceCount);

			for (int i = 0; i < nBalanceCount; ++i)
			{
				int32_t nSubjectID, nAmount;
				pprotores->getBalanceInfo(i, &nSubjectID, &nAmount);

				//100 表示防沉迷系统剩余的有效益秒
				if ( nSubjectID == 100 && tpPlayer->mAdultFlag != 1)
				{
					tpPlayer->mValidSeconds = nAmount;	

					LOG_DEBUG("default", "[ %s : %d ][ %s ] Role(%d:%s) has bee add Fcm list", 
						__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, tpPlayer->GetCharNameShow());

					CServiceGateFcm::GetSingleton().InsertFcmPlayer(tpPlayer->mCharID, tpPlayer->mValidSeconds);
				}
			}

			// CPlayerManager::GetSingleton().PlayerEnterGame(tpPlayer);

		}else
		{
			CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nAccountID);
			LK_ASSERT(tpPlayer != NULL, return);

			switch (nResultCode)
			{
				case E_SYS_DATABASE_ERROR:
				case E_SYS_DATABASE_CONNECT_ERROR:
				/*
				default:
				{
					CPlayerManager::GetSingleton().PlayerEnterGame(tpPlayer);
					break;
				}
				*/
				default:
				{
					// CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, nResultCode, em_keep_connect);

					LOG_ERROR("erating", "[ %s : %d ][ %s ] AccoundID(%d) use RoleID(%d) EnterGame failure(%d) !",  
							__LK_FILE__, __LINE__, __FUNCTION__, nAccountID, tpPlayer->mCharID, nResultCode);

					// tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
					return;
				}
			}

		}
	}
}

// ***************************************************************
//  Function:		OnAGIPMsgPasswdVerifyRespons
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
/*
void CCoreService::OnAGIPMsgPasswdVerifyResponse(SysProtocol* pSysProto, int nParam1, int nParam2)
{
	LK_ASSERT(pSysProto!= NULL, return);
	AGIPPasswordVerifyRes* pprotores = (AGIPPasswordVerifyRes*) pSysProto;

	int nAccountID = nParam1;
	int nRoleID = nParam2;
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nAccountID);

	if(tpPlayer == NULL)
	{
		LOG_INFO("erating", "[ %s : %d ][ %s ] AccountID(%d) has left", 
				__LK_FILE__, __LINE__, __FUNCTION__, nAccountID);
		return;
	}

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4erating_passwdverify);
	tpPlayer->mStatus = CGatePlayer::em_status_ingate_erating_passwdverify;

	int32_t nResultCode = 0;
	unsigned int nCommandID = 0;
	pSysProto->getCommandID(&nCommandID);
	LK_ASSERT(nCommandID == CMD_PASSWORD_VERIFY_RES, return);

	pprotores->getResultCode(&nResultCode);
	if (nResultCode == S_SUCCESS)
	{
		CPlayerManager::GetSingleton().SendDeleteChar2DB(tpPlayer, nRoleID);
	}else
	{
		switch(nResultCode)
		{
			case E_ERROR:
			case E_PARAMETER_ERROR:
			case E_GATEWAY_NOT_FOUND:
			case E_USER_ACCOUNT_ERROR:
			case E_USER_PASSWORD_ERROR:
			case E_USER_SECURITYPW_ERROR:
			case E_SYS_DATABASE_ERROR:
			case E_SYS_DATABASE_CONNECT_ERROR:
			{
				#ifdef DIRECT_DEL_ROLE	
				CPlayerManager::GetSingleton().SendDeleteChar2DB(tpPlayer, nRoleID);
				#else
				tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
				tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);

				CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, nResultCode, em_keep_connect);
				#endif
				break;
			}

			default:
			{
				LOG_ERROR("erating", "[ %s : %d ][ %s ] there is no action for erating error(%d)", 
						__LK_FILE__, __LINE__, __FUNCTION__, nResultCode);
				break;
			}
		}
	}
}
*/

// ***************************************************************
//  Function:		OnAGIPMsgRoleListResponse
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnAGIPMsgRoleListResponse(SysProtocol* pSysProto, int nParam1, int nParam2)
{
	LK_ASSERT(pSysProto!= NULL, return);
	AGIPRoleListRes* pprotores = (AGIPRoleListRes*) pSysProto;

	// param1 is tplayer's mAccountID
	int nAccountID = nParam1;
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nAccountID);
	if(tpPlayer == NULL)
	{
		LOG_NOTICE("erating", "[ %s : %d ][ %s ] AccountID(%d) has left", 
				__LK_FILE__, __LINE__, __FUNCTION__, nAccountID );
		return;
	}

	int32_t nResultCode = 0;
	unsigned int nCommandID = 0;
	pSysProto->getCommandID(&nCommandID);
	LK_ASSERT(nCommandID == CMD_ROLE_LIST_RES, return);

	pprotores->getResultCode(&nResultCode);

	if (nResultCode == S_SUCCESS)
	{
		CMessage tMessage;

		int32_t nRoleCount = 0;
		pprotores->getResultCode(&nResultCode);
		for (int i = 0; i < nRoleCount; ++i)
		{
			ROLE_INFO roleinfo;
			memset(&roleinfo, 0, sizeof(ROLE_INFO));
			pprotores->getRoleInfo(i, &roleinfo);
		}
	}else
	{
		LOG_ERROR("erating", "[ %s : %d ][ %s ] Obtain AccountID(%d) Role List from ERating ERROR(%d)!", 
				__LK_FILE__, __LINE__, __FUNCTION__, nAccountID, nResultCode);
	}
}

// ***************************************************************
//  Function:		CCoreService::OnMessageRoleChangeCampNotice
//  Description:	处理客户端登陆网关请求
//	Input:			vCommHandle - 通讯端口； pMessage - 消息指针
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CCoreService::OnMessageRoleChangeCampNotice(int nServerID, CMessage* pMessage)
{
	LK_ASSERT( pMessage != NULL && pMessage->msgpara() != 0 , return );
	CMessageRoleChangeCampNotice * pChangeCampNotice = (CMessageRoleChangeCampNotice *) pMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(pChangeCampNotice->roleid());
	if (tpPlayer == NULL)
	{
		LOG_FATAL("default", "[ %s : %d ][ %s ] role_id(%d) has left gate",
				__LK_FILE__, __LINE__, __FUNCTION__, pChangeCampNotice->roleid());	
	}else
	{
		tpPlayer->mCountryID = pChangeCampNotice->camp();
	}

	CRoleGlobalInfo*pRoleInfo =  CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pChangeCampNotice->roleid());		
	if (pRoleInfo == NULL)
	{
		LOG_FATAL("default", "[ %s : %d ][ %s ] role_id(%d) is not in the global nameid table ", 
				__LK_FILE__, __LINE__, __FUNCTION__, pChangeCampNotice->roleid());
		return;
	}

	pRoleInfo->SetCountryID(pChangeCampNotice->camp());
}

// ***************************************************************
//  Function:		CCoreService::OnMessageKickOffPlayerResponse
//  Description:	处理客户端登陆网关请求
//	Input:			vCommHandle - 通讯端口； pMessage - 消息指针
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CCoreService::OnMessageKickOffPlayerResponse(int nServerID, CMessage* pMessage)
{
	LK_ASSERT( pMessage != NULL && pMessage->has_msgpara() != false , return );
	CMessageKickOffPlayerResponse* pKickOffResponse = (CMessageKickOffPlayerResponse*) pMessage->msgpara();
	LK_ASSERT(pKickOffResponse != NULL, return);
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(pKickOffResponse->accountid());
	if ( pKickOffResponse->roleworldid() > 0 )
	{
		CPlayerManager::GetSingleton().SendKickGlbalPlayerACK( pKickOffResponse->accountid(), pKickOffResponse->charid(), pKickOffResponse->roleworldid() );
	}
	if (tpPlayer == NULL)
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] Account(%d) has left game",  
				__LK_FILE__, __LINE__, __FUNCTION__, pKickOffResponse->accountid());
		return;
	}

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4kickoff);

	switch(pKickOffResponse->result()) // 失败
	{
		case EGATE_KICKOFFPLAYER_NOTIN_SCENE:
		{
			CMessage tMessage;
			CMessageLoginGateResponse tLoginGate;
			tLoginGate.set_gatewayid(CGateServer::GetSingleton().GetRegistID());

			pbmsg_sethead(&tMessage, ID_G2C_RESPONSE_LOGINGATE, 0, 0, 0, 0, 0, 0);
			tMessage.set_msgpara((uint32_t) &tLoginGate);
			CGateServer::GetSingleton().Send2Player(tpPlayer, &tMessage );

			tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
			tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);
			break;
		}

		case 0:
		{
			tpPlayer->mStatus = CGatePlayer::em_status_ingate_v4leavemap;
			break;
		}

		//TODO: 调试期间暂保留, 用来发现问题
		default:
		{
			CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, EGATE_PLAYERSTATE_ISNOT_PERMITED, em_need_disconnect);
			tpPlayer->mStatus = CGatePlayer::em_status_ingame_idle;
		}
	}
}


// ***************************************************************
//  Function:		CCoreService::OnMessagePlayerLoginGateRequest
//  Description:	a. 如果在场景，踢人
//                  b. 如果在网关，踢人
//                  c. 如果在跨场景，拒绝
//                  d. 如果是inwait，正常
//
//	Input:			vCommHandle - 通讯端口； pMessage - 消息指针
//	OutPut:			无
//	Return:			
//	Others:			output state: inwait, v4kickoff, redirect
//  Date:			10/29/2008
// 
// ***************************************************************
void CCoreService::OnMessagePlayerLoginGateRequest( int vCommHandle, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL && pMessage->has_msgpara() != false , return );
	
	CMessageLoginGateRequest* pReqMsg = (CMessageLoginGateRequest*) pMessage->msgpara();
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(pReqMsg->accountid());
	if( tpPlayer == NULL)
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] Account(%d) has left ", 
				__LK_FILE__, __LINE__, __FUNCTION__, pReqMsg->accountid());

		return;
	}

	// 绑定通讯端口
	tpPlayer->mCommHandle = vCommHandle;
	CPlayerManager::GetSingleton().RegistPlayerComm(tpPlayer);

	if (tpPlayer->mStatus == CGatePlayer::em_status_inwait_wait)
	{
		tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
		tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);
	}

	if (tpPlayer->mStatus == CGatePlayer::em_status_ingate_v4kickoff)
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] Account(%d) RoleId(%d) is going to leave, pls wait for a moment", 
				__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpPlayer->mCharID);

		return;
	}

	CMessage tMessage;
	CMessageLoginGateResponse tLoginGate;
	tLoginGate.set_gatewayid(CGateServer::GetSingleton().GetRegistID());

	pbmsg_sethead(&tMessage, ID_G2C_RESPONSE_LOGINGATE, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tLoginGate);

	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
}
//************************************
// Method:    OnMessagePersonalLevelChange
// FullName:  GateServer::CGateLogic::OnMessagePersonalLevelChange
// Access:    protected 
// Returns:   void
// Qualifier: 处理玩家个人信息改变(等级，职业，地图.....)
// Parameter: CMessage * pMessage
//************************************
void CCoreService::OnMessagePersonalLevelChange( CMessage* pMessage )
{
	CMessageRoleGameInfoReportToGateNotify* pNotifyMsg = (CMessageRoleGameInfoReportToGateNotify*) pMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pNotifyMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );
	
	CRoleGlobalInfo *tpRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( pNotifyMsg->charid() );
	LK_ASSERT( tpRoleInfo != NULL, return )

	if( pNotifyMsg->has_level() )
	{
		tpPlayer->Level( pNotifyMsg->level() );
		tpRoleInfo->SetLevel( pNotifyMsg->level() );

		// 如果出师了
		CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
		if ( tpTable != NULL )
		{
			// 要通知前辈玩家	
			int mLevel = pNotifyMsg->level();
			for ( int i = 0; i < pNotifyMsg->masterid_size(); i++ )
			{
				if ( mLevel <= tpTable->mFormalLevel && pNotifyMsg->level() % 5 == 0 )
				{
					CFriendModule::GetSingleton().SendSlaveLevelUpNotify( tpPlayer->CharID(), pNotifyMsg->masterid(i) );
				}
				if ( mLevel == tpTable->mFormalLevel )
				{
					CFriendModule::GetSingleton().SendAddNumNotify( tpPlayer->CharID(), pNotifyMsg->masterid(i), AddFormal );
				}
				if ( mLevel == tpTable->mEducatedLevel )
				{
					CFriendModule::GetSingleton().SendAddNumNotify( tpPlayer->CharID(), pNotifyMsg->masterid(i), AddSlaveNum );
				}
			}
		}
	}

	if( pNotifyMsg->has_mapid() || pNotifyMsg->has_lineid() )
	{
		int tOldLineID = tpPlayer->LineID( );
		int tOldMapID = tpPlayer->MapID( );

		tpPlayer->MapID( pNotifyMsg->mapid() );
		tpRoleInfo->SetMapID( pNotifyMsg->mapid() );
		tpPlayer->LineID( pNotifyMsg->lineid() );
		tpRoleInfo->SetLineID( pNotifyMsg->lineid() );

		CWorldManager::GetSingleton().AddPlayerNum( tpPlayer->CountryID(), tpPlayer->LineID(), tpPlayer->MapID() );
		CWorldManager::GetSingleton().DelPlayerNum( tpPlayer->CountryID(), tOldLineID, tOldMapID );
		CWorldModule::GetSingleton().SendWarSourceToSingle( tpPlayer->CharID(), tpPlayer->MapID() );
	}

	if( pNotifyMsg->has_metier() )
	{			
		tpPlayer->MetierID( pNotifyMsg->metier() );
		tpRoleInfo->SetMetier( pNotifyMsg->metier() );
	}

	if ( pNotifyMsg->has_pkvalue() )
	{
		tpPlayer->PKValue( pNotifyMsg->pkvalue() );
	}

	// ....
}


// ***************************************************************
//  Function:		CCoreService::OnMessagePlayerReturn2GateNotice
//  Description:	处理场景服务器创建地图回应
//	Input:			nServerID - 场景服务器id; pMessage - 消息
//	OutPut:			无
//	Return:			无
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CCoreService::OnMessagePlayerReturn2GateNotice( int vServerID, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->msgpara() != 0, return);
	CMessagePlayerReturn2GateNotice *tpMsg = ( CMessagePlayerReturn2GateNotice * )pMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(tpMsg->accountid());
	if(tpPlayer == NULL)
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] Account(%d), Role(%d) can not be found in gate",
				__LK_FILE__, __LINE__, __FUNCTION__, tpMsg->accountid(), tpMsg->charid());
		return;
	}
	else
	{			
		/*
		// clear invitelist of player
		tpPlayer->ClearFamilyInvite();
		
		// force player leave team
		CServiceGateTeam::GetSingleton().ForceLeaveTeam( tpPlayer->mCharID );


        for( unsigned int i = 0; i < sizeof( tpPlayer->mRepetionMap ) / sizeof( tpPlayer->mRepetionMap[ 0 ] ); i++ )
        {
            tpPlayer->mRepetionMap[ i ] = INVITED_KEY_VALUE;
        }

		tpPlayer->mPoint.Clear( );

		CPlayerManager::GetSingleton().RemoveRoleNameIndex(tpPlayer);
		CPlayerManager::GetSingleton().RemoveRoleIDIndex(tpPlayer);
		*/

		CPlayerManager::GetSingleton().ClearScenePlayer(tpPlayer);

		tpPlayer->mStatus = CGatePlayer::em_status_inwait_wait;
		tpPlayer->mCheckTimer = CWTimer( CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);

		CMessage tMessage;
		CMessagePlayerReturn2GateResponse msg;
		msg.set_charid(tpMsg->charid());
		tMessage.set_msgpara((uint32_t) &msg);
		pbmsg_sethead(&tMessage, ID_G2S_PLAYER_RETURN2GATE_RESPONSE, 0, 0, 0, 0, 0, 0);

		CGateServer::GetSingleton().Send2Scene( vServerID, &tMessage );
	}
}

// ***************************************************************
//  Function:		OnMessageFangChenMiRequest
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
/*
void CCoreService::OnMessageFangChenMiRequest( int vServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);

	CMessageFangChenMiRequest *tpMsg = ( CMessageFangChenMiRequest * )tpMessage->msgpara();

	#ifdef USING_ERATING

	if (CGateServer::GetSingleton().IsERatingLaunched() == true)
	{
		for (int i = 0; i < tpMsg->roledetail_size(); ++i)
		{
			CMessagePlayerLeaveMapNotice tPlayerLeave; 
			CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(tpMsg->roledetail(i).charid());

			tPlayerLeave.set_charid(tpMsg->roledetail(i).charid());
			tPlayerLeave.set_charlevel(tpMsg->roledetail(i).charlevel());
			tPlayerLeave.set_charmetier(tpMsg->roledetail(i).charmetier());
			tPlayerLeave.set_money1(tpMsg->roledetail(i).money1());
			tPlayerLeave.set_money2(tpMsg->roledetail(i).money2());
			tPlayerLeave.set_exp(tpMsg->roledetail(i).exp());

			CPlayerManager::GetSingleton().SendUserLogoutProto2ERating(tpPlayer, &tPlayerLeave, 2);
		}
	}

	#endif
}
*/

// ***************************************************************
//  Function:		OnMessageRedirectSErrorEvent
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnMessageRedirectSErrorEvent( int vCommHandle, CMessage* pMessage )
{
	CMessageRedirectSErrorCode* tpMsg = ( CMessageRedirectSErrorCode* )( pMessage->msgpara( ) );
	unsigned int tCharID = tpMsg->charid();
	unsigned short tLineID = tpMsg->lineid();
	//int tErrCode = tpMsg->errorcode();
	unsigned short tMapID = tpMsg->mapid();

	LOG_NOTICE( "default", "[ %s : %d ][ %s ] Player %d Want Repetion Into( %d : %d ), Error Code %d", 
			__LK_FILE__, __LINE__, __FUNCTION__, tpMsg->charid(), tLineID, tMapID, tpMsg->errorcode( ) );

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tCharID );
	if( tpPlayer != NULL )
	{
		if( IS_REPETION_LINE_ID( tLineID ) == true )
		{
			CRepetionModule::GetSingletonPtr( )->PlayerOutRepetion( tpPlayer, tMapID );
		}
		
		tpPlayer->mStatus= CGatePlayer::em_status_ingame_idle;
	}

	CPlayerManager::GetSingleton().SendRedirErrCode( tpMsg->charid(), tpMsg->errorcode( ), tLineID, tMapID );

	return;
}



// ***************************************************************
//  Function:		OnMessageRedirectEnterMapResponse
//  Description:	
//	Input:			
//	OutPut:			
//	Return:			
//	Others:
//  Date:			11/01/2008
// 
// ***************************************************************
void CCoreService::OnMessageRedirectEnterMapResponse( int vServerID, CMessage* pMessage )									  
{
	LK_ASSERT( pMessage != NULL, return );
	CMessagePlayerRedirectEnterMapResponse* tRedirectMsg = (CMessagePlayerRedirectEnterMapResponse*) pMessage->msgpara();
	LK_ASSERT( tRedirectMsg != NULL && vServerID > 0 && vServerID <= SCENE_AT_GATE_CAP, return);

	int vCharID = tRedirectMsg->charid( );
	int vLineID = tRedirectMsg->lineid( );
	int vMapID  = tRedirectMsg->mapid( );

	CSceneServer* tpHost = CGateServer::GetSingleton().GetServerByID( vServerID );
	LK_ASSERT( tpHost != NULL, return );
	tpHost->mPlayerNumber++;

	LOG_NOTICE( "default", "[ %s : %d ][ %s ] Player( %d ) Redirec Scene( %d : %d ) Create PlayerObject OK", 
			__LK_FILE__, __LINE__, __FUNCTION__, vCharID, vLineID, vMapID );

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(vCharID);
//  LK_ASSERT( tpPlayer != NULL, return );
    if( tpPlayer == NULL )
    {
        LOG_ERROR("default", "[ %s : %d ][ %s ]Player( %d ) Redirecting Scene( %d : %d ) error.", 
			__LK_FILE__, __LINE__, __FUNCTION__, vCharID, vLineID, vMapID);

		return;
    }

	//CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingame_v4scene_redirectscene);

	if ( tpPlayer->mStatus != CGatePlayer::em_status_ingame_v4scene_redirectscene 
		 && tpPlayer->mLineID != vLineID )
	{
		//通知目的场景失败
		CMessage tMsg;
		CMessageRedirectGErrorCode tMsgFact;

		tMsgFact.set_charid( vCharID );
		tMsgFact.set_errorcode( EGATE_PLAYERSTATE_ISNOT_PERMITED );

		pbmsg_sethead(&tMsg, ID_G2S_REDIRECT_G_ERROR_CODE, 0, 0, 0, 0, 0, 0);
		tMsg.set_msgpara((uint32_t) &tMsgFact);

		CGateServer::GetSingleton().Send2Scene( tpHost, &tMsg );


		
        LOG_ERROR("default", "[ %s : %d ][ %s ]Player( %d ) Redirecting Scene ( %d : %d ) timeout error.", 
			__LK_FILE__, __LINE__, __FUNCTION__, vCharID, vLineID, vMapID);
		if( IS_REPETION_LINE_ID( vLineID ) == true )
		{
			CRepetionModule::GetSingletonPtr()->PlayerOutRepetion( tpPlayer, vMapID );
		}
		return;
	}

	CSceneMap* tpMap = CGateServer::GetSingleton().GetMapByLineMap( vLineID, vMapID );
	if ( tpMap == NULL || tpMap->mServerObjID == -1 )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ]sceneserver is not open which player in", __LK_FILE__, __LINE__, 
			__FUNCTION__ );

		// 通知客户端进入游戏错误
		CMessageSceneNotOpenNotice tErrorNotice;
		CMessage tMessage;
		tErrorNotice.set_charid(vCharID);

		pbmsg_sethead(&tMessage, ID_G2S_SCENE_NOT_OPEN_NOTICE, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &tErrorNotice);

		tpPlayer->mStatus= CGatePlayer::em_status_ingame_idle;
		CGateServer::GetSingleton().Send2Scene( vServerID, &tMessage);
		CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, REDRIECT_ERR_UNKNOW, vLineID, vMapID );
		if( IS_REPETION_LINE_ID( vLineID ) == true )
		{
			CRepetionModule::GetSingletonPtr( )->PlayerOutRepetion( tpPlayer, vMapID );
		}
		return;
	}

	CSceneServer* pHost = (CSceneServer*) CGateObjManager::GetSingletonPtr()->GetObject(tpMap->mServerObjID);
	if( pHost == NULL )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ]Cannt Find SceneServer( %d : %d ) ServerIndex is %d", __LK_FILE__,
			__LINE__, __FUNCTION__ );
		if( IS_REPETION_LINE_ID( vLineID ) == true )
		{
			CRepetionModule::GetSingletonPtr( )->PlayerOutRepetion( tpPlayer, vMapID );
		}
		tpPlayer->mStatus= CGatePlayer::em_status_ingame_idle;
		CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, REDRIECT_ERR_UNKNOW, vLineID, vMapID );
		return;
	}

	CMessage tMsg;	
    CMessagePlayerRedirectResponse tPlayerRedirect;
    tPlayerRedirect.set_charid( vCharID );
    tPlayerRedirect.set_redirect( true );
    tPlayerRedirect.set_lineid( vLineID );
    tPlayerRedirect.set_mapid( vMapID );
    tPlayerRedirect.set_posx( tRedirectMsg->posx( ) );
    tPlayerRedirect.set_posy( tRedirectMsg->posy( ) );
    tPlayerRedirect.set_address( tpHost->mAddress );

    pbmsg_sethead(&tMsg, ID_G2S_PLAYER_REDIRECT_RESPONSE, 0, 0, 0, 0, 0, 0);
    tMsg.set_msgpara((uint32_t) &tPlayerRedirect);
    
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsg );
    LOG_NOTICE("default", "[ %s : %d ][ %s ] RoleID( %d ) Redirect from Scene( %d : %d ) to Scene( %d : %d )",
			__LK_FILE__, __LINE__, __FUNCTION__, vCharID, tpPlayer->mLineID, tpPlayer->mMapID, vLineID, vMapID );
    
	// 将玩家状态置为准备连接
	if( tpPlayer->mLineID != vLineID )
	{
		tpPlayer->mStatus = CGatePlayer::em_status_ongame_v4scene_logingame;
		tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerOnGameStateTimeout);
	}
	else
	{
		tpPlayer->mStatus = CGatePlayer::em_status_ingame_idle;
	}

	// 因为之后玩家的状态不可控, 因此将索引删除
	//RemoveRoleNameIndex(tpPlayer);
	//RemoveRoleIDIndex(tpPlayer);

	if( IS_REPETION_LINE_ID( tpPlayer->mLineID ) == true )
	{
		CRepetionModule::GetSingletonPtr( )->PlayerOutRepetion( tpPlayer, tpPlayer->mMapID );
	}

    tpPlayer->mLineID = vLineID;
    tpPlayer->mMapID  = vMapID;
}


// ***************************************************************
//  Function:		OnMessagePlayerRedirectScene
//  Description:	处理重定向网关消息
//	Input:			nServerID - 服务器ID; 
//	OutPut:			
//	Return:			
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CCoreService::OnMessagePlayerRedirectScene(int vServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL && pMessage->has_msgpara(), return );
	CMessagePlayerRedirectRequest* tpRedirect = (CMessagePlayerRedirectRequest*) pMessage->msgpara();
	LK_ASSERT( tpRedirect != NULL && vServerID > 0 && vServerID <= SCENE_AT_GATE_CAP, return);

	int tRtn = SUCCESS;
	int vAccountID = tpRedirect->accountid(); 
	int vCharID = tpRedirect->charid(); 
	unsigned short vLineID = tpRedirect->lineid(); 
	unsigned short vMapID = tpRedirect->mapid(); 
	unsigned short vPosX = tpRedirect->posx(); 
	unsigned short vPosY = tpRedirect->posy();
	unsigned short vTeamLevel = tpRedirect->teamlevel( );
	KEY_TYPE tKeyType = 0;

	LOG_NOTICE( "default", "[ %s : %d ][ %s ]Player( %d ) Want Redirect to Scene( %d : %d ) Pos( %d : %d )", 
			__LK_FILE__, __LINE__, __FUNCTION__, vCharID, vLineID, vMapID, vPosX, vPosY );

	CMessage tMessage;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );
	if( tpPlayer == NULL )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ]Player( %d ) Redirect to Scene( %d : %d ) Error",
			__LK_FILE__, __LINE__, __FUNCTION__, vCharID, vLineID, vMapID);
		CPlayerManager::GetSingleton().SendRedirErrCode( vCharID, REDRIECT_ERR_UNKNOW, vLineID, vMapID );	
		return;
	}

	CWorldManager::GetSingleton().DelPlayerNum( tpPlayer->GetNationalityID(), tpPlayer->LineID(), tpPlayer->MapID() );

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingame_idle);

		

	//有切仅有一个副本服务器
	CSceneMap* tpMap = CGateServer::GetSingleton().GetMapByLineMap( vLineID, vMapID );
//	if ( tpMap == NULL || tpMap->mServerObjID == -1 )
//	{
//		LOG_ERROR("default", "[ %s : %d ][ %s ]sceneserver( %d : %d ) is not open which player Redirect in", 
//			__LK_FILE__, __LINE__, __FUNCTION__, vLineID, vMapID );

//		CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, REDIRECT_MAP_NOTOPEN );	
//		return;
//	}

	if( IS_REPETION_LINE_ID( vLineID ) == true )
	{
		int tLineID = vLineID;
		bool tIsBattle = CRepetionModule::GetSingletonPtr( )->IsBattleMap( vMapID );
		if( vMapID < ARRAY_CNT( tpPlayer->mJoinBattle ) && tIsBattle == true )
		{
			if( tpPlayer->mJoinBattle[ vMapID ].mKey == INVITED_KEY_VALUE )
			{
				// not permited into battle map
				LOG_NOTICE( "default", "[ %s : %d ][ %s ] Player( %d ) Not Permited Into Battle %d ", 
						__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, vMapID );

				CPlayerManager::GetSingletonPtr( )->SendRedirErrCode( tpPlayer->mCharID, ERR_BATTLE_NOT_PERMITED,
					vLineID, vMapID );
				return;
			}

			int tTime = time( NULL );
			if( (tpPlayer->mJoinBattle[ vMapID ].mTimeLimit == false) && (tTime - tpPlayer->mJoinBattle[ vMapID ].mTime >= 60 * 5) )
			{

				LOG_NOTICE( "default", "[ %s : %d ][ %s ] Player( %d ) Want Into Battle %d, But TimeOut( %d : %d ), "
					"key %lld",	__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, vMapID, 
					tpPlayer->mJoinBattle[ vMapID ].mTime, tTime, tpPlayer->mJoinBattle[ vMapID ].mKey );

				tpPlayer->mJoinBattle[ vMapID ].mKey = INVITED_KEY_VALUE;
				tpPlayer->mJoinBattle[ vMapID ].mTime = 0;

				CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, ERR_BATTLE_TIMEOUT, vLineID,
					vMapID	);
				return;
			}

			tRtn = CRepetionModule::GetSingletonPtr()->PlayerIntoRepetion( tpPlayer, vMapID, vTeamLevel, 
					tpPlayer->mJoinBattle[ vMapID ].mKey, &tLineID );
		}
		else
		{
			// 如果是要进入可重入的副本
			tKeyType = INVITED_KEY_VALUE;
			if ( tpPlayer->mRepetionMap[ vMapID ] == INVITED_KEY_VALUE )
			{
				CRepetion *tpRepetion = CRepetionModule::GetSingleton().GetRepetion( tpPlayer->GetRepetionKey( tpPlayer->CharID() ) );
				if ( tpRepetion != NULL )
				{
					CRepetionInfo *tpRepetionInfo = tpRepetion->GetRepetionInfo();
					if ( tpRepetionInfo != NULL )
					{
						if ( tpRepetionInfo->GetRelogin() == 1 && tpRepetionInfo->GetBeginMapID() == vMapID )
						{							
							tKeyType = tpRepetion->GetKey();
						}
					}								
				}
			}			
			tRtn = CRepetionModule::GetSingletonPtr()->PlayerIntoRepetion( tpPlayer, vMapID, vTeamLevel,
				tKeyType, &tLineID );
						
		}
//		tRtn = CRepetionModule::GetSingletonPtr()->PlayerIntoRepetion( tpPlayer, vMapID, vTeamLevel,
//				INVITED_KEY_VALUE, &tLineID );


		// INVITED_KEY_VALUE );


//		tRtn = CRepetionModule::GetSingletonPtr()->PlayerIntoRepetion( tpPlayer, vMapID, vTeamLevel, 
//			INVITED_KEY_VALUE );
		if( tRtn != SUCCESS )
		{
			if( tRtn == ERR_BUSY )
			{
				CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, REDIRECT_OBJECT_IS_FULL, tLineID,
					vMapID );
			}
			else if( tRtn == ERR_FULL || ERR_REPETION_PLAYER_CAMP_FULL == tRtn )
			{
				CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, REDIRECT_MAP_PLAYER_IS_FULL, 
					tLineID, vMapID );
			}
			else if( tRtn != ERR_EXIST )
			{
				CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, REDIRECT_OBJECT_IS_FULL, tLineID,
					vMapID );
			}
			else if( tRtn != ERR_NOT_FOUND )
			{
				CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, REDIRECT_MAP_NOTOPEN, tLineID,
					vMapID );
			}
			
			if( tRtn != ERR_EXIST )
			{	
				//CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, REDRIECT_ERR_UNKNOW  );
				return;
			}
		}

		tpMap = CGateServer::GetSingleton().GetMapByLineMap( tLineID, vMapID );
		if ( tpMap == NULL || tpMap->mServerObjID == -1 )
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ]sceneserver( %d : %d ) is not open which player Redirect in", 
					__LK_FILE__, __LINE__, __FUNCTION__, tLineID, vMapID );

			CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, REDIRECT_MAP_NOTOPEN, tLineID, vMapID);	
			CRepetionModule::GetSingletonPtr( )->PlayerOutRepetion( tpPlayer, vMapID );
			return;
		}	
	}

	if ( tpMap == NULL || tpMap->mServerObjID == -1 )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ]sceneserver( %d : %d ) is not open which player Redirect in", 
			__LK_FILE__, __LINE__, __FUNCTION__, vLineID, vMapID );

		CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, REDIRECT_MAP_NOTOPEN, vLineID, vMapID );	
		return;
	}

	// 通知场景服务器玩家切换
	CMessagePlayerRedirectMapNotice tPlayerEnter;

	tPlayerEnter.set_commhandle(tpPlayer->mCommHandle); 
	tPlayerEnter.set_logintoken( tpPlayer->mLoginToken ); 
	tPlayerEnter.set_charid( vCharID ); 
	tPlayerEnter.set_lineid( tpMap->mLineID ); 
	tPlayerEnter.set_mapid( vMapID); 
	//tPlayerEnter.set_mapname( tpMap->mMapName); 
	tPlayerEnter.set_posx( vPosX); 
	tPlayerEnter.set_posy( vPosY); 
	tPlayerEnter.set_accountid( vAccountID );

	if( tpRedirect->has_charname() == true )
	{
		tPlayerEnter.set_charname( tpRedirect->charname( ).c_str( ) ); 	
	}
	tPlayerEnter.set_gender( tpRedirect->gender( ) );
	tPlayerEnter.set_metier( tpRedirect->metier( ) );
	tPlayerEnter.set_nationality( tpRedirect->nationality( ) );
	tPlayerEnter.set_facetype( tpRedirect->facetype( ) );
	tPlayerEnter.set_charlevel( tpRedirect->charlevel( ) );
	tPlayerEnter.set_exp( tpRedirect->exp( ) );
	tPlayerEnter.set_lastlogin( tpRedirect->lastlogin( ) );
	tPlayerEnter.set_datalength( tpRedirect->datalength( ) );
	if( tpRedirect->has_data( ) == true )
	{
		tPlayerEnter.set_data( tpRedirect->mutable_data( )->c_str(), tpRedirect->datalength() );
	}
	tPlayerEnter.set_teamflag( tpRedirect->teamflag( ) );

	if( IS_REPETION_LINE_ID( vLineID ) == true )
	{
		tPlayerEnter.set_key( tpPlayer->mRepetionMap[ vMapID ] );
		tPlayerEnter.set_camp( tpPlayer->mJoinBattle[ vMapID ].mCamp );
	}
	else
	{
		tPlayerEnter.set_key( INVITED_KEY_VALUE );
	}
	if ( tKeyType > 0  )
	{
		tPlayerEnter.set_key( tKeyType );
	}

	tPlayerEnter.set_pettempid( tpRedirect->pettempid() );
	tPlayerEnter.set_petonlinetime( tpRedirect->petonlinetime() );
	tPlayerEnter.set_petmode( tpRedirect->petmode() );
	
	// 设置密码和安全保护时间
	const char* pPwd = tpPlayer->GetRolePwd( tpPlayer->mCharID );
	if( pPwd != NULL )
	{
		tPlayerEnter.set_password( pPwd );
	}
	tPlayerEnter.set_protecttime( tpPlayer->GetPwdProtectTime( tpPlayer->mCharID ) );
	tPlayerEnter.set_protecttimelength( tpPlayer->GetPwdProtectTimeLength( tpPlayer->mCharID ) );

	// 设置玩家所在区组的世界ID
	if ( tpPlayer->GetRoleWorldID() > 0 && tpPlayer->GetRoleWorldID() != CGateServer::GetSingleton().GetWorldID() )
	{
		tPlayerEnter.set_roleworldid( tpPlayer->GetRoleWorldID() );
	}

	//	获取家族和军团信息
	CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpPlayer->mCharID );
	if ( pFamily != NULL )
	{
		tPlayerEnter.set_familyid( pFamily->GetID() );
		tPlayerEnter.set_familyname( pFamily->GetName() );	
		if( pFamily->GetRooter() == tpPlayer->mCharID )
		{
			tPlayerEnter.set_isfamilyheader( 1 );
			int tApplyNum = pFamily->GetAppplyNum();
			if ( tApplyNum > 0 )
			{
				tPlayerEnter.set_ifhasapplies( 1 );
			} 					
		}
		tPlayerEnter.set_familymoney( pFamily->GetMoney() );	
		
		family_info_t *tpInfo = pFamily->GetMemberFamilyInfo( tpPlayer->mCharID );
		if ( tpInfo != NULL )
		{
			tPlayerEnter.set_familypost( tpInfo->mFamilyPost );	
			tPlayerEnter.set_familycontribute( tpInfo->mFamilyContribue );
		}		
		tPlayerEnter.set_familyheaderid( pFamily->GetRooter() );
		if ( pFamily->GetCorpsID() != 0 )
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( pFamily->GetCorpsID() );	
			if ( tpCorps != NULL )
			{
				tPlayerEnter.set_corpsid( tpCorps->GetCorpsID() );
				tPlayerEnter.set_corpsname( tpCorps->GetCorpsName() );
				if ( tpCorps->GetRooterID() == tpPlayer->mCharID )
				{
					tPlayerEnter.set_iscorperheader( 1 ); 
					if ( tpCorps->GetApplyNum() > 0 )
					{	
						tPlayerEnter.set_ifhascorpsapply( 1 );						
					}																				
				} 	
				tPlayerEnter.set_corpsmoney( tpCorps->GetCorpsMoney() );						
				tPlayerEnter.set_corpsheaderid( tpCorps->GetRooterID() );
				if ( tpInfo != NULL )
				{
					tPlayerEnter.set_corpspost( tpInfo->mCorpsPost );
					tPlayerEnter.set_corpscontribute( tpInfo->mCorpsContribue );
				}
				int tCountryID = CWorldManager::GetSingleton().GetCountryIDByMap( tpCorps->GetBattleCity( ) );
				if ( tpPlayer->TempID( ) == COUNTRY_NONE )
				{
					tpPlayer->TempID( tCountryID );
				}	
			} 			
		}		
	}
	CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionOfPlayer( tpPlayer->mCharID );
	if ( tpUnion != NULL )
	{
		tPlayerEnter.set_unionid( tpUnion->GetUnionID() );
		tPlayerEnter.set_unionname( tpUnion->GetUnionName() );
		tPlayerEnter.set_headcorpsid( tpUnion->GetCoreID() );
	}
	tPlayerEnter.set_countryid( tpPlayer->TempID() );
	if ( tpPlayer->TempID( ) >= 0 && tpPlayer->TempID( ) < COUNTRY_NUM )
	{
		int tWorldID = tpPlayer->GetNationalityID( );
		if ( tWorldID > WORLD_START && tWorldID < WORLD_NUM )
		{
			CCountry *tpCountry = &(  CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tpPlayer->TempID( ) ] );
			tPlayerEnter.set_officialid( tpCountry->GetOfficialID( tpPlayer->CharID( ) ) );
			tPlayerEnter.set_kingid( tpCountry->GetKingID() );
		}		
	}
	
	// 密码状态
	tPlayerEnter.set_passwordstatus( tpPlayer->GetPasswordStatus( tpPlayer->CharID() ) );
	
	// 查看是否是受追踪的玩家
	bool tRet = CGateLogic::GetSingleton().GetTracePlayerManager().CheckIfRoleIDNeedTrace( tpPlayer->mAccountID, tpPlayer->mCharID );
	tPlayerEnter.set_iftracedplayer( tRet );		


	// if player in team, load team data
	// CServiceGateTeam::GetSingleton().MakeTeamPB( tPlayerEnter.mutable_teamdata(), tpPlayer->mCharID );
	CServiceGateTeam::GetSingleton().MakeTeamMsgOnRedirect( tPlayerEnter.mutable_teammsg(), tpPlayer->mCharID );

	// 增加防沉迷内容
	tPlayerEnter.set_adultflag(tpPlayer->mAdultFlag);
	if (tpPlayer->mAdultFlag != 1) //非成年人, 将有效益秒数发送给gameserver
	{
		int nElapsedTime = time(NULL) - tpPlayer->mSockTime; 
		tPlayerEnter.set_validseconds(tpPlayer->mValidSeconds > nElapsedTime ? (tpPlayer->mValidSeconds - nElapsedTime) : 0);
	}

	// 透传加解密 密钥
	if( tpRedirect->has_encryptkey() )
		tPlayerEnter.set_encryptkey( tpRedirect->encryptkey().c_str(), KEYLEN );

	pbmsg_sethead(&tMessage, ID_G2S_REDIRECT_MAP_NOTICE, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tPlayerEnter);

	CSceneServer* pHost = (CSceneServer*) CGateObjManager::GetSingletonPtr()->GetObject(tpMap->mServerObjID);
	if( pHost == NULL )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Can NOT find sceneserver( %d : %d ) ServerIndex is %d", 
				__LK_FILE__, __LINE__, __FUNCTION__,  vLineID, vMapID, tpMap->mServerObjID);

		CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, REDRIECT_ERR_UNKNOW, vLineID, vMapID  );
		return;
	}	

	CGateServer::GetSingleton().Send2Scene( pHost, &tMessage);

	if( tpMap->mLineID == tpPlayer->mLineID )
	{
		LOG_INFO( "default", "[ %s : %d ][ %s ] Player( %d ) Redirect Into Same Scene( %d : %d ) ( %d : %d )", 
			__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, tpPlayer->mLineID, tpPlayer->mMapID, 
			tpMap->mLineID, tpMap->mMapID );
	}
	else
	{
		tpPlayer->mStatus = CGatePlayer::em_status_ingame_v4scene_redirectscene;
	}

	
	tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerWaitRedirectTimerout);

} 


// ***************************************************************
//  Function:		OnMessageScenePlayerEnterMapResponse
//  Description:	处理场景通知网关已经准备好接收玩家进入消息
//	Input:			
//	OutPut:			
//	Return:			
//	Others:
//  Date:			10/29/2008
// 
// ***************************************************************
void CCoreService::OnMessageScenePlayerEnterMapResponse( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL && pMessage->has_msgpara(), return );
	CMessagePlayerEnterMapResponse* tpEnterMap = (CMessagePlayerEnterMapResponse*) pMessage->msgpara();
	LK_ASSERT( tpEnterMap != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	CSceneServer* tpHost = CGateServer::GetSingleton().GetServerByID( nServerID );
	LK_ASSERT( tpHost != NULL, return );

	CMessage tMessage;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(tpEnterMap->accountid());
	if (tpEnterMap->errorcode() != 0)
	{
		if (tpPlayer == NULL)
		{
			LOG_NOTICE("default", "[ %s : %d ][ %s ] Account(%d) has left ", 
					__LK_FILE__, __LINE__, __FUNCTION__, tpEnterMap->accountid());
			return;
		}

		CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4scene_entergame);


		if ( tpEnterMap->errorcode() == EGAME_SCENESERVER_IS_FULLLIST)
		{
			//CPlayerManager::GetSingleton().QueuePlayer(tpPlayer, tpHost);
			tpHost->ResetQueuePlayer(tpPlayer);
			tpHost->mbIsFull = true;
			return;
		}

		if( tpEnterMap->errorcode( ) == EGAME_SCENESERVER_MAP_IS_EMPTY )
		{
			if( IS_REPETION_LINE_ID( tpEnterMap->lineid( ) ) )
			{

			}

		}
		else if( EGAME_SCENESERVER_MAP_NOT_EXIST == tpEnterMap->errorcode( ) )
		{
			if( IS_REPETION_LINE_ID( tpEnterMap->lineid( ) )  == true )
			{
				CRepetionModule::GetSingletonPtr( )->PlayerOutRepetion( tpPlayer, tpPlayer->mMapID );	

				for ( unsigned int i = 0; i < tpPlayer->mSave.size(); i++ )
				{
					if ( (unsigned int) tpPlayer->mCharID == tpPlayer->mSave[ i ].mCharID )
					{
						tpPlayer->mLineID = tpPlayer->mSave[ i ].mBackLineID;
						tpPlayer->mMapID  = tpPlayer->mSave[ i ].mBackMapID;
						tpPlayer->mPoint.Clear( );
						tpPlayer->mPoint.mX = tpPlayer->mSave[ i ].mPosX;
						tpPlayer->mPoint.mY = tpPlayer->mSave[ i ].mPosY;

						tpPlayer->mSave[ i ].mMapID = tpPlayer->mSave[ i ].mBackMapID;
						tpPlayer->mSave[ i ].mLineID = tpPlayer->mSave[ i ].mBackLineID;
					}
				}
			}
			CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, tpEnterMap->errorcode(), em_keep_connect);
			tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
			tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);

		}
		else
		{
			CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, tpEnterMap->errorcode(), em_keep_connect);
			tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
			tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);
		}

		/*
		#ifdef USING_ERATING
		if (mERatingIsLaunched == true)
		{
			CMessagePlayerLeaveMapNotice tPlayerLeave; 
			SendUserLogoutProto2ERating(tpPlayer, &tPlayerLeave);
		}
		#endif
		*/

		tpHost->RemoveQueuePlayer(tpPlayer);
		return;	
	}

	if(tpPlayer == NULL)
	{
		tpHost->mPlayerNumber++;
		LOG_INFO("default", "[ %s : %d ][ %s ] AccountID(%d) has left", 
				__LK_FILE__, __LINE__, __FUNCTION__, tpEnterMap->accountid());
		return;
	}

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4scene_entergame);

	// 通知客户端进入游戏回应
	CMessageEnterGameResponse tEnterGame;
	tEnterGame.set_scenename(tpHost->mServerName);
	tEnterGame.set_address( tpHost->mAddress);

	pbmsg_sethead(&tMessage, ID_G2C_RESPONSE_ENTERGAME, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tEnterGame);

	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage);

	// TODO: 通知信息服务器玩家已经进入游戏了，可以释放登陆排队队列
	CMessagePlayerLoginCompleteNotice tPlayerLogin;
	tPlayerLogin.set_accountid( tpPlayer->mAccountID );

	tMessage.Clear();
	pbmsg_sethead(&tMessage, ID_G2S_PLAYER_LOGIN_COMPLETE_NOTICE, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tPlayerLogin); 
	
	// 如果是其他区组的玩家不用通知
	if ( tpPlayer->GetRoleWorldID() == CGateServer::GetSingleton().GetWorldID() || tpPlayer->GetRoleWorldID() == 0 )
	{
		CGateServer::GetSingleton().Send2Login(&tMessage);
		LOG_INFO( "default", "[ %s : %d ][ %s ] Account(%d) will use Role(%d) enter sceneserver(%d:%s)", 
			__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpEnterMap->charid(), nServerID, tpHost->mServerName);	
	}	

	// 将玩家从端口索引中删除, 此时玩家对象已经在场景中创建了
	// 玩家收到CMessageEnterGameResponse， 即要断开了与Gate的连接

	tpPlayer->mLineID		= tpEnterMap->lineid();
	tpPlayer->mMapID		= tpEnterMap->mapid();
	tpPlayer->mCharID		= tpEnterMap->charid();
	
	CRoleGlobalInfo *tpPlayerInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpPlayer->mCharID );
	if ( tpPlayerInfo != NULL )
	{
		tpPlayer->mCountryID = tpPlayerInfo->CountryID();
	}
	

	if (tpHost != NULL) 
	{
		tpHost->mPlayerNumber++;
		tpHost->RemoveQueuePlayer(tpPlayer);
	}

	#ifndef USE_LONG_CONNECTION
	CPlayerManager::GetSingleton().UnRegistPlayerComm(tpPlayer);
	#endif

	// 加入到超时队列
	tpPlayer->mStatus = CGatePlayer::em_status_ongame_v4scene_logingame;
	tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerOnGameStateTimeout);

}

// ***************************************************************
//  Function:		OnMessagePlayerEnterConfirm
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnMessagePlayerEnterConfirm( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL && pMessage->has_msgpara(), return );
	CMessagePlayerEnterConfirmNotice* tpPlayerEnter = (CMessagePlayerEnterConfirmNotice*)pMessage->msgpara();
	LK_ASSERT( tpPlayerEnter != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	int vAccountID = tpPlayerEnter->accountid();
	int vCharID = tpPlayerEnter->charid(); 
	const char* pName = tpPlayerEnter->charname().c_str(); 
	unsigned short vLineID = tpPlayerEnter->lineid(); 
	unsigned short vMapID = tpPlayerEnter->mapid();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(vAccountID);
	LK_ASSERT(tpPlayer != NULL, return);
	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ongame_v4scene_logingame );


	tpPlayer->mLineID		= vLineID;
	tpPlayer->mMapID		= vMapID;
	tpPlayer->mCharID		= vCharID;

	strncpy(tpPlayer->mCharName, pName, sizeof(tpPlayer->mCharName));
	tpPlayer->mCharName[sizeof(tpPlayer->mCharName)-1] = 0;

	tpPlayer->mFaceType		= tpPlayerEnter->face();
	tpPlayer->mSexID		= tpPlayerEnter->sex();
	tpPlayer->mMetierID		= tpPlayerEnter->metier();
	tpPlayer->mLevel		= tpPlayerEnter->level();
	tpPlayer->mCountryID	= tpPlayerEnter->nationality();

	tpPlayer->SetCSKey(tpPlayerEnter->cskey().c_str());

	CPlayerManager::GetSingleton().RegistRoleIDIndex(tpPlayer);
	CPlayerManager::GetSingleton().RegistRoleNameIndex(tpPlayer);

	tpPlayer->m_tLastActiveTime = time(0);
	tpPlayer->mStatus = CGatePlayer::em_status_ingame_idle;

	CCorpservice::GetSingleton().OnFireCorpsBattleEvent( tpPlayer->mCharID, nServerID );

	if ( !IS_REPETION_LINE_ID( vLineID ) )
	{
		CJudgeModule::GetSingleton().LoadJudgeDataFromDB( tpPlayer );
	}
	CWorldModule::GetSingleton().SetOperation( vLineID, vCharID );
	// 玩家登录
	CWorldManager::GetSingleton().AddPlayerNum( tpPlayer->GetNationalityID(), tpPlayer->LineID(), tpPlayer->MapID() );
	CWorldModule::GetSingleton().SendWarSourceToSingle( tpPlayer->CharID(), tpPlayer->MapID() );
	// 如果是防沉迷玩家, 发送防沉迷通知
	if ( tpPlayer->mAdultFlag != 1)	
	{
		CServiceGateFcm::GetSingleton().SendFcmNotification(tpPlayer);
	}	 
	// 验证是否需要通知安定度	
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpPlayer->mCharID );
	if ( tpFamily != NULL )
	{
		CServiceFamily::GetSingleton().JudgeFamilyStability( tpFamily, tpPlayer );					
	}
	
	// 通知弹劾
	CWorldModule::GetSingleton().SendAccuseKingNotify( tpPlayer->GetNationalityID( ), tpPlayer );
	CWorldModule::GetSingleton().SendAccuseStatusNotify( tpPlayer->GetNationalityID( ), tpPlayer );

	// 通知可重入副本信息
	KEY_TYPE tRepetionKey = tpPlayer->GetRepetionKey( tpPlayer->CharID() );
	CRepetion *tpRepetion = CRepetionModule::GetSingleton().GetRepetion( tRepetionKey );
	if ( tpRepetion != NULL )
	{
		CRepetionInfo *tpRepetionInfo = tpRepetion->GetRepetionInfo();
		if ( tpRepetionInfo != NULL )
		{
			if ( tpRepetionInfo->GetRelogin() == 1 )
			{
				CRepetionModule::GetSingleton().SendLastRepetionIndex( tpPlayer, tpRepetionInfo->GetRepetionIndex(), REPETION_INDEX_TYPE_RELOGIN );
			}
		}
	}
}

// ***************************************************************
//  Function:		OnMessageCheckRolePwd
//  Description:	删除角色时检查2级密码的请求
//	Input:			
//	OutPut:			
//	Return:			
//	Others:
//  Date:			05/05/2009
// 
// ***************************************************************
void CCoreService::OnMessageCheckRolePwd( int vCommHandle, CMessage *pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().GatePlayer( vCommHandle );
	LK_ASSERT( pGatePlayer != NULL, return )
	CMessageCheckRolePwdRequest *tpMsg = ( CMessageCheckRolePwdRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	int tret = pGatePlayer->CheckRolePwd( tpMsg->charid(), tpMsg->password().c_str() );
	CMessage MsgHead;
	CMessageCheckRolePwdResponse MsgCheckRolePwd;
	MsgHead.mutable_msghead()->set_messageid( ID_G2C_RESPONSE_CHECKROLEPWD );
	MsgHead.set_msgpara( (int)&MsgCheckRolePwd );
	if ( tret == 0 )
	{
		MsgCheckRolePwd.set_errcode( 0 );
		pGatePlayer->DeletRolePwd( tpMsg->charid() );
	}
	else
	{
		MsgCheckRolePwd.set_errcode( ERR_PASSWORD_WRONGPASSWORD );
	}	
	CGateServer::GetSingleton().Send2Player( pGatePlayer, &MsgHead );
}


// ***************************************************************
//  Function:		OnTimer
//  Description:	通过玩家名称得到玩家数据
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::CheckPlayerStatusTimeout()
{
	int nInWaitNumber = 0;	
	int nOnGameNumber = 0;	

	int nCheckInterval = CGateLogic::GetSingleton().mConfig.nPlayerStateCheckInterval;


	CGateObjManagerImp::ObjMng_PlayerAtGate & tObjPool = CGateObjManager::GetSingleton().mGateObjManagerImp->mPlayerAtGatePool;
	CGateObjManagerImp::ObjMng_PlayerAtGate ::iterator iter = tObjPool.begin();

	while(iter != tObjPool.end())
	{
		CGatePlayer* tpPlayer = &(*iter);
		if (tpPlayer == NULL) continue;

		// 避免因为删除造成的iter无效
		iter++;

		switch (tpPlayer->mStatus)
		{
			case CGatePlayer::em_status_inwait_wait:
			{
				if ( tpPlayer->mCheckTimer.Elapse( nCheckInterval ) == true )
				{
					LOG_NOTICE("default", "[ %s : %d ][ %s ] Account(%d) token(%d) em_status_inwait_wait timeout, destory it",
							__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpPlayer->mLoginToken);

					// 如果是其他区的账号，则清除账号ID的记录
					if ( tpPlayer->GetRoleWorldID() != CGateServer::GetSingleton().GetWorldID() )
					{
						CPlayerManager::GetSingleton().RemoveAccountIDIndex( tpPlayer );
					}
					CPlayerManager::GetSingleton().DestroyGatePlayer(tpPlayer);
				}else
				{
					nInWaitNumber ++;
				}

				break;
			}

			case CGatePlayer::em_status_ingate_idle:
			{
				if ( tpPlayer->mCheckTimer.Elapse( nCheckInterval ) == true )
				{
					LOG_NOTICE("default", "[ %s : %d ][ %s ] Account(%d) token(%d) em_status_ingate_idle timeout . destory it",
							__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpPlayer->mLoginToken);

					CPlayerManager::GetSingleton().SendGateErrorNotice(
							tpPlayer, EGATE_PLAYER_STILLINGATE_TOLONG, em_need_disconnect);

					CPlayerManager::GetSingleton().DestroyGatePlayer(tpPlayer);
				}

				break;
			}

			case CGatePlayer::em_status_ingate_v4kickoff:
			{
				if ( tpPlayer->mCheckTimer.Elapse( nCheckInterval ) == true )
				{
					CPlayerManager::GetSingleton().SendGateErrorNotice(
							tpPlayer, EGATE_PLAYERSTATE_ISNOT_PERMITED, em_need_disconnect);
					tpPlayer->mStatus = CGatePlayer::em_status_ingame_idle;
				}

				break;
			}

			case CGatePlayer::em_status_ongame_v4scene_logingame:
			{
				/*TODO: 正式上线的时候可以关闭,依赖服务器间心跳来保障, 不再需要
				if ( tpPlayer->mCheckTimer.Elapse( nCheckInterval ) == true )
				{
					LOG_ERROR("default", "[ %s : %d ][ %s ] Account(%d) Role(%d) em_status_ongame_v4scene_logingame timeout, destory it",
							__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpPlayer->mCharID);

					#ifdef USE_LONG_CONNECTION
					CPlayerManager::GetSingleton().SendGateErrorNotice(
							tpPlayer, EGATE_PLAYER_WILL_DISCONNECT, em_need_disconnect);
					#endif


					CPlayerManager::GetSingleton().DestroyGatePlayer(tpPlayer);
				}else
				{
					nOnGameNumber ++;
				}
				*/
					
				nOnGameNumber ++;
				break;
			}

			case CGatePlayer::em_status_ingame_v4scene_redirectscene:
			{
				if ( tpPlayer->mCheckTimer.Elapse( nCheckInterval ) == true )
				{
					LOG_ERROR("default", "[ %s : %d ][ %s ] Account(%d) Role(%d) em_status_ingame_v4scene_redirectscene timeout",
							__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpPlayer->mCharID);

					tpPlayer->mStatus = CGatePlayer::em_status_ingame_idle;
					CPlayerManager::GetSingleton().SendRedirErrCode( tpPlayer->mCharID, REDIRECT_MAP_NOTOPEN, 0, 0 );	
				}

				break;
			}

			default:
			{
				break;
			}
		}
	}
}

// ***************************************************************
//  Function:		CheckPlayerActiveTimeout	
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CPlayerManager::CheckPlayerActiveTimeout()
{
	//int nOneSecond = 1000; 
	time_t tNow = time(NULL);
	time_t tTimeOut = (time_t) CGateLogic::GetSingleton().mConfig.nPlayerActiveTimeout;

	for ( PLAYERCONTAINER::iterator it = mPlayerInSceneByChar.begin(); it != mPlayerInSceneByChar.end();  )
	{
		CGatePlayer* tpPlayer =  ScenePlayer( it->first );
		if( tpPlayer == NULL )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ] can't find gate player by char id %d", 
					__LK_FILE__, __LINE__, __FUNCTION__, it->first );

			++it;
			continue;
		}

		it++;

		// 长时间不活跃，说明场景上已经没有该玩家了，发生了某种错误或者丢包导致
		if( (tNow - tpPlayer->m_tLastActiveTime) > tTimeOut ) 
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ] Role(%d:%s) has idle for so long time(%d:%d), so erase it", 
					__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, tpPlayer->GetCharNameShow(), tNow - tpPlayer->m_tLastActiveTime, tTimeOut);

			/*
			RemoveRoleNameIndex(tpPlayer);
			RemoveRoleIDIndex(tpPlayer);
			*/

			// 清理网关上场景玩家的脏数据
			ClearScenePlayer(tpPlayer);

			#ifdef USE_LONG_CONNECTION
			//SendGateErrorNotice( tpPlayer, EGATE_PLAYER_WILL_DISCONNECT, em_need_disconnect);
			OnlyDisconnectClient(tpPlayer);
			#endif

			// 销毁玩家
			DestroyGatePlayer(tpPlayer, 0);
		}
	}
}

// ***************************************************************
//  Function:		DealQueuePlayer
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::DealQueuePlayer(unsigned int nNowTick)
{
	for ( int i = 1; i <= CGateServer::GetSingleton().GetMaxSceneServerID(); i ++ )
	{
		CSceneServer *pHost = CGateServer::GetSingleton().GetServerByID(i);
		if(pHost == NULL) continue;


		// 重置周期内登录的人数
		pHost->mPlayerPerTimer = 0;

		CSceneServer::CLoginQueue::iterator iter = pHost->mLoginQueue.begin();
		CMessage tMessage;


		while(iter != pHost->mLoginQueue.end())
		{
			// 如果找不到玩家或者玩家去做其它操作了, 将玩家从等待队列中移除
			CGatePlayer* tpPlayer = (CGatePlayer*) CGateObjManager::GetSingletonPtr()->GetObject(iter->PlayerID());
			if (tpPlayer == NULL 
			|| (tpPlayer->mStatus != CGatePlayer::em_status_ingate_v4loginqueue
			&&  tpPlayer->IsDealedFromQueue() != true))
			{
				iter = pHost->mLoginQueue.erase(iter);
				continue;
			}

			// 如果要登录的服务器不存在了, 将玩家从等待队列中移除
			CSceneServer* tpPlayerHost = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
			if (pHost != tpPlayerHost)
			{
				iter = pHost->mLoginQueue.erase(iter);
				continue;
			}

			if ( pHost->IsFull() == false )
			{
				// 处理登录
				if ( pHost->mPlayerPerTimer < pHost->mPermitPerTimer)
				{
					//tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
					// 如果没有被处理 
					if ( tpPlayer->mStatus == CGatePlayer::em_status_ingate_v4loginqueue)
					{
						#ifdef USING_ERATING
						if (CGateServer::GetSingleton().IsERatingLaunched() == true)
						{
							CPlayerManager::GetSingleton().SendEnterGameProto2ERating(tpPlayer, pHost->mServerID);
							CPlayerManager::GetSingleton().PlayerEnterGame(tpPlayer);
						}else
						{
							CPlayerManager::GetSingleton().PlayerEnterGame(tpPlayer);
						}

						#else
						CPlayerManager::GetSingleton().PlayerEnterGame(tpPlayer);
						#endif

						// 不做移除处理, 根据回应处理
						//iter = pHost->mLoginQueue.erase(iter);
						pHost->mPlayerPerTimer ++;
						continue;
					}
				}
			}

			// 处理排队, 
			if((nNowTick - iter->ReportTick()) > 15000 && pHost->IsFull() )
			//if( pHost->IsFull() || pHost->mPlayerPerTimer >= pHost->mPermitPerTimer)
			{
				iter->SetReportTick(nNowTick);

				CMessageQueuePlayerNotify tQueueMsg;

				tQueueMsg.set_countinscene(pHost->PlayerCount());
				tQueueMsg.set_countinqueue(pHost->mLoginQueue.size());

				tQueueMsg.set_yourposition(distance(pHost->mLoginQueue.begin(), iter) + 1);

				tQueueMsg.set_elapsseconds((nNowTick - iter->QueueTick())/1000);
				tQueueMsg.set_needseconds(0);

				pbmsg_sethead(&tMessage, ID_G2S_NOTIFY_LOGINQUEUE, 0, 0, 0, 0, 0, 0);
				tMessage.set_msgpara((uint32_t) &tQueueMsg);

				CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage);
			}

			iter++;
		}
	}
}

// ***************************************************************
//  Function:		OnMessagePlayerCancelLoginQueue
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnMessagePlayerCancelLoginQueue( int vCommHandle, CMessage* tpMessage)
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GatePlayer( vCommHandle );
	LK_ASSERT( tpPlayer != NULL, return );
	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4loginqueue);

	CSceneServer* pHost = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
	if (pHost == NULL) return;

	CSceneServer::CLoginQueue::iterator iter = pHost->mLoginQueue.begin();
	for(;iter != pHost->mLoginQueue.end(); ++iter)
	{
		if (tpPlayer->get_id() == iter->PlayerID() )
		{
			pHost->mLoginQueue.erase(iter);
			break;
		}
	}

	tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
	tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);
}

// ***************************************************************
//  Function:		OnMessagePlayerEnterGameRequest
//  Description:	通过账号,指定角色进入游戏
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnMessagePlayerEnterGameRequest(int vCommHandle, CMessage* tpMessage)
{
   // 100309 MAOCY ADD-BEGIN
   CConfigure& vConfigure = CGateLogic::GetSingleton().mConfig;
   LOG_INFO("default", "[%s:%d][%s] Player enter request message. [handle=%d]",
         __LK_FILE__, __LINE__, __FUNCTION__, vCommHandle);
   // 获得用户信息
   CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GatePlayer(vCommHandle);
   if(NULL == tpPlayer){
      return;
   }
   if ( CPlayerManager::GetSingleton().IsPlayerInGlobalScene( tpPlayer->AccountID() ) )
   {
	   CPlayerManager::GetSingleton().KickPlayerInGlobalScene( tpPlayer->AccountID() );
   }
   // 检查用户是否已经认证过
   CMessage tMessage;
   if(vConfigure.roleValidQuestionFlag && !tpPlayer->mRoleValid){
      // 生成验证问题
      CMessageQuestionRequireResponse response;
      SRoleValidQuestionOption options[4];
      BuildQuestionRequireResponse(tpPlayer, response, options);
      // 回应请求
      pbmsg_sethead(&tMessage, ID_G2C_RESPONSE_QUESTIONREQUIRE, 0, 0, 0, 0, 0, 0);
      tMessage.set_msgpara((uint32_t)&response);
      // 回应请求
      CGateServer::GetSingleton().Send2Player(tpPlayer, &tMessage);
      return;
   }
   // 100309 MAOCY ADD-END
	CWTPoint tPoint;
	LK_ASSERT(tpMessage != NULL, return);
	CMessageEnterGameRequest* tpEnterGame = (CMessageEnterGameRequest*) tpMessage->msgpara();
	LK_ASSERT(tpEnterGame != NULL, return);

	CPlayerManager::GetSingleton().PlayerEnterGameWrapper(
			tpEnterGame->accountid(), tpEnterGame->charid(), tpEnterGame->mac().c_str());
}

// ***************************************************************
//  Function:		OnMessagePlayerDeleteCharRequest
//  Description:	
// 				1. 向ERATING发送密码认证请求
// 				2. 认证成功后向数据库提及申请
// 				3. 数据库执行成功，发送通知请求给ERATING
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnMessagePlayerDeleteCharRequest( int vCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	CMessageDeleteCharRequest* tpDeleteChar = (CMessageDeleteCharRequest*) tpMessage->msgpara();

	LK_ASSERT(tpDeleteChar != NULL, return);
	
	//TODO: 内网不能删除帐号临时方案
	//tpDeleteChar->set_passwd("000000");
	//tpDeleteChar->set_passwdtype(2);

	CPlayerManager::GetSingleton().DeleteCharWrapper(
			vCommHandle, tpDeleteChar->charid(), tpDeleteChar->passwd().c_str(), tpDeleteChar->passwdtype());

}

// ***************************************************************
//  Function:		OnDeleteChar
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnSessionDeleteChar( CMessage* tpMessage, int vParamater1, int vParamater2 )
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageExecuteSqlResponse3* pMessage = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	int nAccountID = vParamater1;
	unsigned int nCharID = vParamater2;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nAccountID);
	if(tpPlayer == NULL)
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] Account(%d) has left", 
				__LK_FILE__, __LINE__, __FUNCTION__, vParamater1);

		return;
	}

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4mysql_removerole);

	tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
	tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);

	//执行成功
	if ( pMessage->resultcode() == 1 )
	{
		bool bFind = false;
		for ( CGatePlayer::SaveList::iterator it = tpPlayer->mSave.begin(); it != tpPlayer->mSave.end(); ++it )
		{
			if ( (*it).mCharID == nCharID )
			{
				tpPlayer->mSave.erase( it );
				bFind = true;
				break;
			}
		}	

		if (bFind == false) return;

		#ifdef USING_ERATING
		// notice erating delete role.
		if (CGateServer::GetSingleton().IsERatingLaunched() == true)
		{
			CPlayerManager::GetSingleton().SendDeleteRoleProto2ERating(tpPlayer, nCharID);
		}
		#endif

		

		//删除名称服务中的ID
		CServiceRoleIDName::GetSingleton().RemoveRoleInfoByID(nCharID);

		// 清除玩家所有的申请信息
		CFamilyManager::GetSingleton().DeleteAllApplyOfPlayer( nCharID );

		// 清楚角色相关的所有数据
		CPlayerManager::GetSingleton().SendDelRoleRelationData2DB( nAccountID, nCharID );


		//向客户端发送删除成功
		CMessage tMessage;
		CMessageDeleteCharResponse tDeleteChar;
		tDeleteChar.set_charid(nCharID);

		pbmsg_sethead(&tMessage, ID_G2C_RESPONSE_DELETECHAR, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &tDeleteChar);
		
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
	}else // 如果执行失败
	{
		CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, EGATE_DELETEROLE_ON_DBSQL, em_keep_connect);
		tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
		return;
	}
}

// ***************************************************************
//  Function:		OnMessagePlayerCreateCharRequest
//  Description:	玩家请求建立角色
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnMessagePlayerCreateCharRequest( int vCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	CMessageCreateCharRequest* tpCreateChar = (CMessageCreateCharRequest*) tpMessage->msgpara();

	LK_ASSERT(tpCreateChar != NULL, return);
	const PBCharProfile& rProfile = tpCreateChar->profile();

	CPlayerManager::GetSingleton().CreateCharWrapper( 
			vCommHandle, rProfile.charname().c_str(), rProfile.sex(), rProfile.facetype());

}

// ***************************************************************
//  Function:		OnSessionCreateChar
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnSessionCreateChar( CMessage* tpMessage, int vParamater1, int vParamater2 )
{
	LK_ASSERT(tpMessage != NULL, return);

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(vParamater1);
	if(tpPlayer == NULL)
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] AccountID(%d) has left", 
				__LK_FILE__, __LINE__, __FUNCTION__, vParamater1 );
		return;
	}

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4mysql_createrole);

	tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
	tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);

	// 如果执行成功
	CMessage tMessage;

	CMessageExecuteSqlResponse3* pMessage = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	if (pMessage != NULL)
	{
		int colnum = pMessage->colcount();
		int rownum = pMessage->rowcount();

		if ( pMessage->resultcode() == 1 )
		{
			int tCharID = 0;

			if(rownum == 1 && colnum > 0)
			{
				tCharID = ::atoi( pMessage->fieldvalue(0).c_str() );
			}

			if ( tCharID > 0 )
			{
				// 插入空记录
				CGateLogic::GetSingleton().ExecuteSql(em_dbsession_null, vParamater1, tCharID, vParamater1, SQL_NONE, INSERT, 
							"INSERT INTO UMS_ROLE_DETAIL_%02d ( role_id ) VALUES ( %d )", vParamater1%16, tCharID);

				int tLineID = CGateServer::GetSingleton().GetMostIdleLineID( CGateLogic::GetSingletonPtr( )->mConfig.nMapID );
				if( tLineID == 0 )
				{
					tLineID = CGateLogic::GetSingletonPtr( )->mConfig.nLineID;
				}

				CMessageCreateCharResponse tCreateChar;
				tCreateChar.set_charid( tCharID );

				tMessage.set_msgpara((uint32_t) &tCreateChar);
				pbmsg_sethead(&tMessage, ID_G2C_RESPONSE_CREATECHAR, 0, 0, 0, 0, 0, 0);  

				CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );

				CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().CreateRoleInfo( 
						tCharID, tpPlayer->mCharName, vParamater1);
				pRoleInfo->SetCountryID(tpPlayer->mCountryID);
				pRoleInfo->SetGenderID(tpPlayer->mSexID);
				pRoleInfo->SetFaceType(tpPlayer->mFaceType);
				//pRoleInfo->SetAccountID(vParamater1);

				tpPlayer->mSave.push_back( CGatePlayer::SaveData( tCharID, 
//							CGateServer::GetSingletonPtr( )->mConfig.nLineID, 
							tLineID,
							CGateLogic::GetSingletonPtr( )->mConfig.nMapID, 
							tpPlayer->mCountryID,
							1,	
							tpPlayer->mSexID,
							METIER_NOVICE,
							0, 
							0,
							-1,
							-1,
							1,
							INVITED_KEY_VALUE,
							-1
							) );
			}
			else
			{
				int nErrCode = EGATE_CREATECHAR_ON_DBSQL;

				switch (tCharID)
				{
					case -12:
						{
							nErrCode = EGATE_CREATECHAR_ON_DUPNAME;
							break;
						}

					case -11:
						{
							nErrCode = EGATE_CREATECHAR_ON_LISTFULL;	
							break;
						}

				}

				CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, nErrCode, em_keep_connect );
				tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
				return;
			}
		}

		// 如果执行失败
		if ( pMessage->resultcode() == 0 )
		{
			CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, EGATE_CREATECHAR_ON_DBSQL, em_keep_connect);
			tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
			return;
		}
	}
}

// ***************************************************************
//  Function:		OnMessagePlayerObtainCharsRequest
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnMessagePlayerObtainCharsRequest( int vCommHandle , CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	CMessageObtainCharRequest* tpObtianChar = (CMessageObtainCharRequest*) tpMessage->msgpara();
	LK_ASSERT(tpObtianChar != NULL, return);

	CPlayerManager::GetSingleton().ObtainCharsWrapper(vCommHandle);
}

// ***************************************************************
//  Function:		OnSessionObtainChars
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnSessionObtainChars( CMessage* tpMessage, int vParamater1, int vParamater2 )
{
	LK_ASSERT(tpMessage != NULL, return);

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(vParamater1);
	if(tpPlayer == NULL)
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] AccountID(%d) has left", 
						__LK_FILE__, __LINE__, __FUNCTION__, vParamater1); 
		return;
	}

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4mysql_obtainroles);

	tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
	tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);

	CMessage tMessage;

	CMessageExecuteSqlResponse3* pMessage = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	CMessageObtainCharNotice tObtainChar;

	if (pMessage != NULL)
	{
		int colnum = pMessage->colcount();
		int rownum = pMessage->rowcount();

		if ( pMessage->resultcode() == 1 )
		{
			tpPlayer->mSave.clear();
			tpPlayer->mRolePwd.clear();
			PBRepetion tPBRepetion;

			for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
			{
				tPBRepetion.Clear( );

				PBCharProfile* tpProfile = tObtainChar.add_charprofile();

				int tCharID = atoi(pMessage->fieldvalue(0 + tRowIndex * colnum).c_str());
				tpProfile->set_charid	( tCharID );
				tpProfile->set_accountid(	atoi(pMessage->fieldvalue(1 + tRowIndex * colnum).c_str()));
				tpProfile->set_charname ( 		 pMessage->fieldvalue(2 + tRowIndex * colnum).c_str());
				tpProfile->set_sex		(	atoi(pMessage->fieldvalue(3 + tRowIndex * colnum).c_str()));
				tpProfile->set_metier	(	atoi(pMessage->fieldvalue(4 + tRowIndex * colnum).c_str()));
				unsigned int tNationality = atoi(pMessage->fieldvalue(5 + tRowIndex * colnum).c_str());
				tpProfile->set_nationality(	tNationality );
				tpProfile->set_facetype (	atoi(pMessage->fieldvalue(6 + tRowIndex * colnum).c_str()));
				unsigned short tLineID = atoi(pMessage->fieldvalue(7 + tRowIndex * colnum).c_str());
				tpProfile->set_lineid	( tLineID );
				unsigned short tMapID = atoi(pMessage->fieldvalue(8 + tRowIndex * colnum).c_str());
				tpProfile->set_mapid	( tMapID );

				unsigned short nLevel = atoi(pMessage->fieldvalue(9 + tRowIndex * colnum).c_str());
				tpProfile->set_level	( nLevel );
				tpProfile->set_exp		(	atoi(pMessage->fieldvalue(10 + tRowIndex * colnum).c_str()));

				
//				pMessage->fieldlen( 11 + tRowIndex * colnum )

				tPBRepetion.ParseFromArray( pMessage->fieldvalue( 11 + tRowIndex * colnum ).c_str( ), 
											pMessage->fieldvaluelen( 11 + tRowIndex * colnum ) );
				int tCountryID = atoi(pMessage->fieldvalue(13 + tRowIndex * colnum).c_str());				
				// 修改外网已经出现的错误值
				if ( tCountryID == 127 )
				{
					tCountryID = COUNTRY_NONE;
				}
				
				int tRoleDeleteTime = atoi(pMessage->fieldvalue(14 + tRowIndex * colnum).c_str());	
				int tLastLoginIP = atoi(pMessage->fieldvalue(15+tRowIndex*colnum).c_str());
				int tProtectTime = atoi(pMessage->fieldvalue(16+tRowIndex*colnum).c_str());
#ifdef _DEBUG_
				LOG_DEBUG( "default", "PbRepetion of charid %d : %s", tCharID, tPBRepetion.ShortDebugString().c_str() );
#endif
				tpPlayer->mSave.push_back( CGatePlayer::SaveData( tCharID, tLineID, tMapID, tNationality, nLevel,
					atoi(pMessage->fieldvalue(3 + tRowIndex * colnum).c_str()),	
					atoi(pMessage->fieldvalue(4 + tRowIndex * colnum).c_str()),
					tPBRepetion.backlineid( ), tPBRepetion.backmapid( ), tPBRepetion.backposx( ), 
					tPBRepetion.backposy( ), tpProfile->facetype(), tPBRepetion.repetionkey( ) > 0 ? tPBRepetion.repetionkey( ) : tPBRepetion.key() , tCountryID, tRoleDeleteTime ) );

				LOG_NOTICE( "default", "[ %s : %d ][ %s ] Load Player( %d ) Now LineID %d, MapID %d, MapKey %lld,"
					"BackLineID %d, BackMapID %d BackPos( %d : %d )", __LK_FILE__, __LINE__, __FUNCTION__, 
					tCharID, tLineID, tMapID, tPBRepetion.key( ), tPBRepetion.backlineid( ), tPBRepetion.backmapid( ), 
					 tPBRepetion.backposx( ), tPBRepetion.backposy( ) );
				const char *pPwd = 	 pMessage->fieldvalue( 12 + tRowIndex * colnum ).c_str( );				
				tpPlayer->ModifyRolePwd( tCharID, pPwd );
				CGatePlayer::CPasswordData tPasswodData;
				
				// 根据等级设置默认时长
				if ( tProtectTime == 0 )
				{
					if ( nLevel >= CGateLogic::GetSingleton().mConfig.mProtectLevel )
					{
						tProtectTime = CGateLogic::GetSingleton().mConfig.mDefaultProtectTime;
					}  				
				}
				
				tPasswodData.mProtectTime = tProtectTime;				
				tpPlayer->SetPassWordData( tCharID, tPasswodData );
				int tDeleteStatusLeftTime = 0;
				int tDeleteProtecttime = CGateLogic::GetSingleton().mConfig.nDeleteProtectTime;
				int tDeleteAllowedtime = CGateLogic::GetSingleton().mConfig.nDeleteAllowedTime;
				int tDeleteCooltime	   = CGateLogic::GetSingleton().mConfig.nDeleteCoolTime;
				int tDeleteLevel =	CGateLogic::GetSingleton().mConfig.mDeleteLevel;
				int tDeleteStatus = tpPlayer->GetRoleDeleteStatus( tCharID, tDeleteStatusLeftTime, tDeleteProtecttime, tDeleteAllowedtime, tDeleteCooltime, tDeleteLevel ); 
				tpProfile->set_roledeletestatus( tDeleteStatus );
				tpProfile->set_deletestatuslefttime( tDeleteStatusLeftTime );
				if ( pPwd[0] != '\0' )
				{
					tpProfile->set_passwordused( 1 );
				}			  								
				if ( CGateLogic::GetSingleton().CheckIfIpInInternalIPList( tLastLoginIP ) == true )
				{
					tLastLoginIP = tpPlayer->mClientAddr;							
				}								
				tObtainChar.set_lastloginip( tLastLoginIP );													
			}	
			
											
			tObtainChar.set_loginip( tpPlayer->mClientAddr );
			
			pbmsg_sethead(&tMessage, ID_G2C_NOTIFY_OBTAINCHAR, 0, 0, 0, 0, 0, 0);
			tMessage.set_msgpara((uint32_t) &tObtainChar);

			CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );		
		}else
		{
			CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, EGATE_ROLELIST_ON_DBSQL, em_keep_connect );
			return;
		}
	}
}

// ***************************************************************
//  Function:		OnMessagePlayerLeaveMapNotice
//  Description:	玩家将要进入网关
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnMessagePlayerLeaveMapNotice( int nServerID, CMessage* tpMessage)
{
	// 验证消息来源
	LK_ASSERT(tpMessage != NULL && tpMessage->has_msgpara(), return);
	CMessagePlayerLeaveMapNotice* tpPlayerLeave = (CMessagePlayerLeaveMapNotice*) tpMessage->msgpara();
	LK_ASSERT( tpPlayerLeave != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	if ( tpPlayerLeave->globalsceneworldid() > 0 )
	{		
		CGlobalData *tpGlobalData = CPlayerManager::GetSingleton().GetGlobalData( tpPlayerLeave->accountid() );
		if ( tpGlobalData != NULL )
		{
			tpGlobalData->mGlobalWorldID = tpPlayerLeave->globalsceneworldid();
			tpGlobalData->mStatus = CGlobalData::EGLOBAL_STATUS_INGLOBALSCENE;
		}
		else
		{
			CGlobalData tGlobalData;
			tGlobalData.mGlobalWorldID = tpPlayerLeave->globalsceneworldid();
			tGlobalData.mRoleID = tpPlayerLeave->charid();
			tGlobalData.mStatus = CGlobalData::EGLOBAL_STATUS_INGLOBALSCENE;
			CPlayerManager::GetSingleton().InsertPlayerToGlobalSceneMap( tpPlayerLeave->accountid(), tGlobalData );
		}		
	}

	// 取得消息涉及的玩家
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct( tpPlayerLeave->accountid());
	

	// 如果存盘未成功，添加到未正常存盘文件中
	if ( tpPlayerLeave->saveflag() == EM_SAVE2FILE)
	{
		CPlayerManager::GetSingleton().mBlackList.Append(nServerID, tpPlayerLeave->charid(), tpPlayer ? tpPlayer->GetCharNameShow() : "NONAME");
	}
	
	// 取得消息从那个服务器来, 直接将其计数-1
	CSceneServer* tpFromHost = CGateServer::GetSingleton().GetServerByID(nServerID);
	if(tpFromHost == NULL)
	{
		LOG_WARN("default", "[ %s : %d ][ %s ] The host(id:%d) has been unregisted", 
			__LK_FILE__, __LINE__, __FUNCTION__, nServerID);
		return;
	}else
	{
		tpFromHost->mPlayerNumber--;
		tpFromHost->mbIsFull = false;
	}

	// 如果玩家为空
	if ( tpPlayer == NULL ) 
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] AccountID(%d) has left", 
						__LK_FILE__, __LINE__, __FUNCTION__, tpPlayerLeave->accountid());
		return;
	}

	// 如果是其他场景的玩家删除信息
	if ( tpPlayer->GetRoleWorldID() > 0 && tpPlayer->GetRoleWorldID() != CGateServer::GetSingleton().GetWorldID() )
	{
		CPlayerManager::GetSingleton().SendKickGlbalPlayerACK( tpPlayer->AccountID(), tpPlayer->CharID(), tpPlayer->GetRoleWorldID() );
	}
	
	// 取得玩家所在的服务器
	CSceneServer* pWhichHost = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
	if ( pWhichHost == NULL)
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] The host(line_id: %d, map_id: %d) has been unregisted", 
				__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mLineID, tpPlayer->mMapID);
		return ;
	}

	// 如果不是玩家所在服务器返回的离开游戏的消息，都不处理
	if ( pWhichHost != tpFromHost )
	{
		// 因为跨服引起的原场景过来的离开消息
		LOG_NOTICE("default", "[ %s : %d ][ %s ] Account(%d) CharID(%d) state(%x) player at(%d:%s), "
				"but logout sceneserver message from (%d:%s), leave reason(%d)", 
				__LK_FILE__, __LINE__, __FUNCTION__,  
				tpPlayer->mAccountID, tpPlayer->mCharID, tpPlayer->mStatus, 
				tpFromHost->mServerID, tpFromHost->mServerName, 
				pWhichHost->mServerID, pWhichHost->mServerName, tpPlayerLeave->reason());

		return;
	}

	switch(tpPlayer->mStatus)
	{
		case CGatePlayer::em_status_ingame_idle:
		case CGatePlayer::em_status_ingame_v4scene_redirectscene:
		{
			switch( tpPlayerLeave->reason())
			{
				case EM_LEAVE_REDIRECT:
				{					
					break;
				}

				case EM_LEAVE_RETURN2GATE: 
				{
					CRepetionModule::GetSingletonPtr( )->RemoveFromAllWait( tpPlayer->mCharID );

					#ifdef USING_ERATING
					if (CGateServer::GetSingleton().IsERatingLaunched() == true)
					{
						//if ( tpPlayerLeave->globalsceneworldid() <= 0 )
						{
							CPlayerManager::GetSingleton().SendUserLogoutProto2ERating(tpPlayer, tpPlayerLeave);
						}						
					}
					#endif
					
					CPlayerManager::GetSingleton().ClearScenePlayer(tpPlayer);

					CMessage tMsg;
					CMessageReturn2GateResponse tMsgFact;
					CMessageHead* tpHead = tMsg.mutable_msghead();
					tpHead->set_messageid( ID_S2C_RESPONSE_RETURN2GATE );
					tMsg.set_msgpara( (uint32)&tMsgFact );

					CGateServer::GetSingleton().Send2Player(tpPlayer, &tMsg );


					tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
					tpPlayer->mCheckTimer = CWTimer( CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);
					tpPlayer->ResetCSKey();
	

					/*
					LOG_FATAL("default", "Impossible User(%d:%d) status (%x) On leave reason(%d), pls check!",
							 tpPlayer->mAccountID, tpPlayer->mCharID, tpPlayer->mStatus, tpPlayerLeave->reason());
					*/
					break;
				}

				case EM_LEAVE_LEAVEGAME:
				case EM_LEAVE_KICKOUT:
				default:
				{
					// sb国王下线要提示
					if ( tpPlayer->TempID( ) >= 0 && tpPlayer->TempID( ) < COUNTRY_NUM )
					{
						int tWorldID = tpPlayer->LineID( );
						if ( tWorldID > WORLD_START && tWorldID < WORLD_NUM )
						{
							CCountry *tpCountry = &(  CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tpPlayer->TempID( ) ] );
							if ( tpPlayer->CharID() == tpCountry->GetKingID() )
							{
								int tIntParam[ 3 ] = { 0 };
								tIntParam[ 0 ] = 0;
								tIntParam[ 1 ] = tWorldID;
								tIntParam[ 2 ] = tpPlayer->TempID( );
								CWorldModule::GetSingleton().SendEventNotify( NOTICE_KINGONLINE, 0, 0, tIntParam, ARRAY_CNT(tIntParam), (char*)tpPlayer->CharName(), NULL );
							}
						}		
					}

					CRepetionModule::GetSingletonPtr( )->RemoveFromAllWait( tpPlayer->mCharID );

					#ifdef USING_ERATING
					if (CGateServer::GetSingleton().IsERatingLaunched() == true)
					{
						//if ( tpPlayerLeave->globalsceneworldid() <= 0 )
						{
							CPlayerManager::GetSingleton().SendUserLogoutProto2ERating(tpPlayer, tpPlayerLeave);
						}						
					}					
					#endif

					// 清理玩家脏数据
					CPlayerManager::GetSingleton().ClearScenePlayer(tpPlayer);

					#ifdef USE_LONG_CONNECTION
					
					/*
					CPlayerManager::GetSingleton().SendGateErrorNotice( 
							tpPlayer, EGATE_PLAYER_WILL_DISCONNECT, em_need_disconnect);
					*/

					CPlayerManager::GetSingleton().OnlyDisconnectClient(tpPlayer);
					#endif

					// 销毁玩家
					CPlayerManager::GetSingleton().DestroyGatePlayer(tpPlayer, tpPlayerLeave->reason());
					break;
				}
			}

			break;
		}

		// 网关主动踢场景玩家的等待状态
		case CGatePlayer::em_status_ingate_v4leavemap: 
		case CGatePlayer::em_status_ingate_v4kickoff:
		{

			CRepetionModule::GetSingletonPtr( )->RemoveFromAllWait( tpPlayer->mCharID );

			#ifdef USING_ERATING
			if (CGateServer::GetSingleton().IsERatingLaunched() == true)
			{
				CPlayerManager::GetSingleton().SendUserLogoutProto2ERating(tpPlayer, tpPlayerLeave);
			}
			#endif

			if ( CPlayerManager::GetSingleton().GatePlayer(tpPlayer->mCommHandle) == tpPlayer)
			{
				// 玩家被踢下线, 向玩家发送登录网关回应
				tpPlayer->ResetCSKey();

				CMessage tMessage;
				CMessageLoginGateResponse tLoginGate;
				tLoginGate.set_gatewayid(CGateServer::GetSingleton().GetRegistID());

				pbmsg_sethead(&tMessage, ID_G2C_RESPONSE_LOGINGATE, 0, 0, 0, 0, 0, 0);
				tMessage.set_msgpara((uint32_t) &tLoginGate);
				CGateServer::GetSingleton().Send2Player(tpPlayer, &tMessage );

				// 清理网关上与此玩家的场景内容
				CPlayerManager::GetSingleton().ClearScenePlayer(tpPlayer);

				// 重置玩家状态
				tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
				tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);
			}else
			{
				// 说明是玩家没连接，有可能是GM踢玩家下线的

				// 清理网关上与此玩家的场景内容
				CPlayerManager::GetSingleton().ClearScenePlayer(tpPlayer);

				#ifdef USE_LONG_CONNECTION
				/*
				CPlayerManager::GetSingleton().SendGateErrorNotice( 
						tpPlayer, EGATE_PLAYER_WILL_DISCONNECT, em_need_disconnect);
				*/

				CPlayerManager::GetSingleton().OnlyDisconnectClient(tpPlayer);

				#endif

				// 销毁玩家实体
				CPlayerManager::GetSingleton().DestroyGatePlayer(tpPlayer, tpPlayerLeave->reason());
			}

			break;
		}

		// 跨服等待或者登录等待状态
		case CGatePlayer::em_status_ongame_v4scene_logingame:
		{
			LOG_NOTICE("default", "[ %s : %d ][ %s ] Account(%d) CharID(%d) state: em_status_ongame_v4scene_logingame,"
					  	" logout sceneserver(%d:%s) reason(%d)",  
					__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpPlayer->mCharID, 
					tpFromHost->mServerID, tpFromHost->mServerName, tpPlayerLeave->reason());


			CRepetionModule::GetSingletonPtr( )->RemoveFromAllWait( tpPlayer->mCharID );

			#ifdef USING_ERATING
			if (CGateServer::GetSingleton().IsERatingLaunched() == true)
			{
				CPlayerManager::GetSingleton().SendUserLogoutProto2ERating(tpPlayer, tpPlayerLeave, 3);
			}
			#endif

			// 清理场景玩家在网关上的脏数据
			CPlayerManager::GetSingleton().ClearScenePlayer(tpPlayer);


			#ifdef USE_LONG_CONNECTION
			/*
			CPlayerManager::GetSingleton().SendGateErrorNotice( 
						tpPlayer, EGATE_PLAYER_WILL_DISCONNECT, em_need_disconnect);
			*/

			CPlayerManager::GetSingleton().OnlyDisconnectClient(tpPlayer);
			#endif

			// 销毁玩家实体
			CPlayerManager::GetSingleton().DestroyGatePlayer(tpPlayer, tpPlayerLeave->reason());

			break;
		}

		// 返回网关
		case CGatePlayer::em_status_inwait_wait:
		{
			/*
			if (tpPlayerLeave->reason() == LEAVE_RETURN2GATE)
			{
				#ifdef USING_ERATING
				if (CGateServer::GetSingleton().IsERatingLaunched() == true)
				{
					CPlayerManager::GetSingleton().SendUserLogoutProto2ERating(tpPlayer, tpPlayerLeave);
				}
				#endif

				CMessage tMsg;
				CMessageReturn2GateResponse tMsgFact;
				CMessageHead* tpHead = tMsg.mutable_msghead();
				tpHead->set_messageid( ID_S2C_RESPONSE_RETURN2GATE );
				tMsg.set_msgpara( (uint32)&tMsgFact );

				CGateServer::GetSingleton().Send2Player(tpPlayer, &tMsg );


				tpPlayer->ResetCSKey();
				tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
			}
			*/

			break;
		}

		// 如果踢人的时候玩家就离开了
		/*
		case CGatePlayer::em_status_ingate_v4kickoff:
		{
			LOG_INFO("default", "[ %s : %d ][ %s ] Account(%d) CharID(%d) state: em_status_ingate_v4kickoff,"
					  	" logout sceneserver(%d:%s) reason(%d)",  
					__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpPlayer->mCharID, 
					tpFromHost->mServerID, tpFromHost->mServerName, tpPlayerLeave->reason());

			// 清理场景玩家在网关上的脏数据
			CPlayerManager::GetSingleton().ClearScenePlayer(tpPlayer);

			break;
		}
		*/

		default:
		{
			LOG_FATAL("default", "Player(%d:%s) left sceneserver(%d:%s) ,but gateplayer status is %x", 
					tpPlayer->mCharID, tpPlayer->GetCharNameShow(), tpFromHost->mServerID, tpFromHost->mServerName, tpPlayer->mStatus);
		}
	}
}


// ***************************************************************
//  Function:		OnMessagePlayerEnterGateNotice
//  Description:	a. 如果能够根据account_id索引到，则告诉loginsrver，玩家已经在线
//                  b. 否则创建玩家，状态inwait
//	Input:			
//	OutPut:			无
//	Return:			
//	Others:
//  Date:			11/11/2008
// 
// ***************************************************************
void CCoreService::OnMessagePlayerEnterGateNotice( CMessage* tpMessage)
{
	CMessagePlayerEnterGateNotice* tpLoginNotice = (CMessagePlayerEnterGateNotice*) tpMessage->msgpara();
	if (tpLoginNotice == NULL) return;

	// 如果没有成人标志，默认为成年人
	CPlayerManager::GetSingleton().EnterGate(
			tpLoginNotice->commhandle(), 
			tpLoginNotice->accountid(), 
			tpLoginNotice->logintoken(), 
			tpLoginNotice->has_adultflag() ? tpLoginNotice->adultflag() : 1,
			tpLoginNotice->userstatus(),tpLoginNotice->gameid(),
			tpLoginNotice->cpuid(), 
			tpLoginNotice->diskid()
			);
}

void CCoreService::OnEventHostDown(CSceneServer* pHost)
{
	if ( pHost == NULL) return;


	CPlayerManager::GetSingleton().RemovePlayerInScene(pHost);
}

void CCoreService::OnAGIPMsgRoleListTimeOut( int nParam1, int nParam2)
{
	LOG_ERROR("erating", "[ %s : %d ][ %s ] Obtain AccountID(%d) Role List from ERating TIMEOUT!",
		__LK_FILE__, __LINE__, __FUNCTION__, nParam1 );
}

void CCoreService::OnTimeOUtObtainChars(int nParam1, int nParam2)
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nParam1);        
	if(tpPlayer == NULL)                                     
	{                                                        
		LOG_NOTICE("default", "[ %s : %d ][ %s ] AccountID(%d) has left for em_dbsession_obtainchars", 
				__LK_FILE__, __LINE__, __FUNCTION__, nParam1);
		return;                                              
	}                                                        

	CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, EGATE_ROLELIST_ON_DBTIMEOUT, em_keep_connect);
	tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
}

// 跨服时获取数据
void CCoreService::SendGetGlobalRoleData2DB( CGatePlayer *pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return );

	if (CGateLogic::GetSingleton().ExecuteSql( em_dbsession_getglobalroledata, pPlayer->mAccountID, 0, 
		pPlayer->mAccountID, SQL_NEED_CALLBACK, SELECT,
		"select a.role_id, a.account_id, role_name, gender_id, metier_id, nationality_id, face_type,"
		"line_id, map_id, role_level, role_expr, b.role_repetion, role_password, country_id,  role_deletedtime, lastlogin_ip, role_protecttime from UMS_ROLE a, UMS_ROLE_DETAIL_%02d b, UMS_ACCOUNT c "
		"where a.account_id = %d and a.role_id = b.role_id and c.account_id = a.account_id order by a.create_time",
		pPlayer->mAccountID%16, pPlayer->mAccountID ))
	{
		pPlayer->mStatus = CGatePlayer::em_status_ingate_v4mysql_obtainroles;
	}
}

// 跨服时的数据处理
void CCoreService::OnSessionGetGlobalRoleData( CMessage* tpMessage, int vParamater1, int vParamater2 )
{
	LK_ASSERT(tpMessage != NULL, return);

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(vParamater1);
	if(tpPlayer == NULL)
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] AccountID(%d) has left", 
			__LK_FILE__, __LINE__, __FUNCTION__, vParamater1); 
		return;
	}

	CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_v4mysql_obtainroles);

	tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
	tpPlayer->mCheckTimer = CWTimer(CGateLogic::GetSingleton().mConfig.nPlayerIdleIngateTimeout);

	CMessage tMessage;

	CMessageExecuteSqlResponse3* pMessage = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	CMessageObtainCharNotice tObtainChar;

	if ( pMessage != NULL )
	{
		int colnum = pMessage->colcount();
		int rownum = pMessage->rowcount();

		if ( pMessage->resultcode() == 1 )
		{
			KEY_TYPE tKey = tpPlayer->mSave[0].mKey;
			tpPlayer->mSave.clear();
			tpPlayer->mRolePwd.clear();
			PBRepetion tPBRepetion;
			for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
			{
				tPBRepetion.Clear( );
				int tCharID = atoi(pMessage->fieldvalue(0 + tRowIndex * colnum).c_str());								
				unsigned short tLineID = atoi(pMessage->fieldvalue(7 + tRowIndex * colnum).c_str());
				unsigned short tMapID = atoi(pMessage->fieldvalue(8 + tRowIndex * colnum).c_str());				
				unsigned short nLevel = atoi(pMessage->fieldvalue(9 + tRowIndex * colnum).c_str());		
				tPBRepetion.ParseFromArray( pMessage->fieldvalue( 11 + tRowIndex * colnum ).c_str( ), pMessage->fieldvaluelen( 11 + tRowIndex * colnum ) );
				int tCountryID = atoi(pMessage->fieldvalue(13 + tRowIndex * colnum).c_str());				
				// 修改外网已经出现的错误值
				if ( tCountryID == 127 )
				{
					tCountryID = COUNTRY_NONE;
				}				
				unsigned int tNationality = atoi(pMessage->fieldvalue(5 + tRowIndex * colnum).c_str());
				int tRoleDeleteTime = atoi(pMessage->fieldvalue(14 + tRowIndex * colnum).c_str());					
				int tProtectTime = atoi(pMessage->fieldvalue(16+tRowIndex*colnum).c_str());				
#ifdef _DEBUG_
				LOG_DEBUG( "default", "PbRepetion of charid %d : %s", tCharID, tPBRepetion.ShortDebugString().c_str() );
#endif
				tpPlayer->mSave.push_back( CGatePlayer::SaveData( tCharID, tLineID, tMapID, tNationality, nLevel,
					atoi(pMessage->fieldvalue(3 + tRowIndex * colnum).c_str()),	
					atoi(pMessage->fieldvalue(4 + tRowIndex * colnum).c_str()),
					tPBRepetion.backlineid( ), tPBRepetion.backmapid( ), tPBRepetion.backposx( ), 
					tPBRepetion.backposy( ), 0, tPBRepetion.key( ), tCountryID, tRoleDeleteTime ) );

				LOG_NOTICE( "default", "[ %s : %d ][ %s ] Load Player( %d ) Now LineID %d, MapID %d, MapKey %lld,"
					"BackLineID %d, BackMapID %d BackPos( %d : %d )", __LK_FILE__, __LINE__, __FUNCTION__, 
					tCharID, tLineID, tMapID, tPBRepetion.key( ), tPBRepetion.backlineid( ), tPBRepetion.backmapid( ), 
					tPBRepetion.backposx( ), tPBRepetion.backposy( ) );
				const char *pPwd = 	 pMessage->fieldvalue( 12 + tRowIndex * colnum ).c_str( );				
				tpPlayer->ModifyRolePwd( tCharID, pPwd );
				CGatePlayer::CPasswordData tPasswodData;
				
				if ( tCharID == tpPlayer->CharID() )
				{
					tpPlayer->CountryID( tCountryID );
					tpPlayer->SetNationalityID( tNationality );
					tpPlayer->TempID( tpPlayer->mSave[tRowIndex].mTempID );
				}

				// 根据等级设置默认时长
				if ( tProtectTime == 0 )
				{
					if ( nLevel >= CGateLogic::GetSingleton().mConfig.mProtectLevel )
					{
						tProtectTime = CGateLogic::GetSingleton().mConfig.mDefaultProtectTime;
					}  				
				}

				tPasswodData.mProtectTime = tProtectTime;				
				tpPlayer->SetPassWordData( tCharID, tPasswodData );
			}	
			if ( tKey > 0 && tpPlayer->mSave.size() > 0 )
			{
				tpPlayer->mSave[0].mKey = tKey;
			}
			

			// 判断是否在网关中
			if ( IS_REPETION_LINE_ID( tpPlayer->mLineID ) == true  )
			{
				int tRtn = SUCCESS;
				tRtn = CRepetionModule::GetSingletonPtr()->PlayerIntoRepetion( tpPlayer, tpPlayer->mMapID, 0, tpPlayer->mSave[0].mKey, NULL );
				if ( tRtn != SUCCESS )
				{
					LOG_ERROR("default",  "[ %s : %d ][ %s ]  player(%d) can not enter into repetion(lineid:%d,mapid:%d,roleworldid:%d)", 
						__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, tpPlayer->mLineID, tpPlayer->mMapID, tpPlayer->GetRoleWorldID() );
				}			
			}

			CSceneServer* tpHost = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
			if (tpHost == NULL)
			{
				LOG_ERROR("default",  "[ %s : %d ][ %s ] Can NOT find player(%d)'s Scene by Line:%d, Map:%d", 
					__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mCharID, tpPlayer->mLineID, tpPlayer->mMapID );
				CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, EGATE_THESCENE_ISNOT_ONLINE, em_keep_connect);
				if( IS_REPETION_LINE_ID( tpPlayer->mLineID ) == true )
				{
					CRepetionModule::GetSingletonPtr( )->PlayerOutRepetion( tpPlayer, tpPlayer->mMapID );
				}
				return ;
			}				
			// 进入登录排队队列
			CPlayerManager::GetSingleton().QueuePlayer(tpPlayer, tpHost);
		}
		else
		{
			CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, EGATE_ROLELIST_ON_DBSQL, em_keep_connect );
			return;
		}
	}	
}
// 跨服时的数据获取超时
void CCoreService::OnTimeOutGetGlobalRoleData(int nParam1, int nParam2)
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nParam1);        
	if(tpPlayer == NULL)                                     
	{                                                        
		LOG_NOTICE("default", "[ %s : %d ][ %s ] AccountID(%d) has left for em_dbsession_getglobalroledata", 
			__LK_FILE__, __LINE__, __FUNCTION__, nParam1);
		return;                                              
	}                                                        

	CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, EGATE_ROLELIST_ON_DBTIMEOUT, em_keep_connect);
	tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
}
void CCoreService::OnAGIPMsgCreateRoleTimeOut(int nParam1, int nParam2)
{
	int nAccountID = nParam1;                                         
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nAccountID);              

	if(tpPlayer == NULL)                                              
	{                                                                 
		LOG_NOTICE("erating", "[ %s : %d ][ %s ] AccountID(%d) has left", 
				__LK_FILE__, __LINE__, __FUNCTION__, nAccountID);     
		return;                                                       
	}                                                                 

	CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, EGATE_CREATECHAR_ON_AGTIMEOUT, em_keep_connect);
	tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
}

void CCoreService::OnTimeOUtCreateChar(int nParam1, int nParam2 )
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nParam1);
	if(tpPlayer == NULL)
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] AccountID(%d) has left for em_dbsession_createchar",
				__LK_FILE__, __LINE__, __FUNCTION__, nParam1);
		return;
	}

	CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, EGATE_CREATECHAR_ON_DBTIMEOUT, em_keep_connect);
	tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;
}

void CCoreService::OnAGIPMsgDeleteRoleTimeOut(int nParam1, int nParam2)
{
	LOG_NOTICE("erating", "[ %s : %d ][ %s ] Delete AccountID(%d) RoleID(%d) failure because of ERating TimeOut",
			__LK_FILE__, __LINE__, __FUNCTION__, nParam1, nParam2); 

	int nAccountID = nParam1;                                       
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nAccountID);            
	if(tpPlayer != NULL)                                            
	{                                                               
		tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;     
	} 
}

void CCoreService::OnTimeOutDeleteChar(int nParam1, int nParam2)
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nParam1);         
	if(tpPlayer == NULL)                                      
	{                                                         
		LOG_NOTICE("default", "[ %s : %d ][ %s ] AccountID(%d) has left for em_dbsession_removechar", 
				__LK_FILE__, __LINE__, __FUNCTION__, nParam1);
		return;                                               
	}                                                         

	CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, EGATE_DELETEROLE_ON_DBTIMEOUT, em_keep_connect);
	tpPlayer->mStatus = CGatePlayer::em_status_ingate_idle;   
}

// 取消删除角色的消息请求
void CCoreService::OnMessageCancelDeleteRole( int vCommHandle, CMessage *pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
	CMessageCancelDeleteRoleRequest *tpMsg = ( CMessageCancelDeleteRoleRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CGatePlayer *tpPlayer =  CPlayerManager::GetSingleton().GatePlayer( vCommHandle );
	LK_ASSERT( tpPlayer != NULL, return )
	int tDeleteStatusLeftTime = 0;
	int tDeleteProtecttime = CGateLogic::GetSingleton().mConfig.nDeleteProtectTime;
	int tDeleteAllowedtime = CGateLogic::GetSingleton().mConfig.nDeleteAllowedTime;
	int tDeleteCooltime	   = CGateLogic::GetSingleton().mConfig.nDeleteCoolTime;
	int tDeleteLevel =	CGateLogic::GetSingleton().mConfig.mDeleteLevel;		
	int tDeleteStatus = tpPlayer->GetRoleDeleteStatus( tpMsg->roleid(), tDeleteStatusLeftTime, tDeleteProtecttime, tDeleteAllowedtime, tDeleteCooltime, tDeleteLevel );
	if ( tDeleteStatus == ROLEDELETESTATUS_DELETEPROTECT || tDeleteStatus == ROLEDELETESTATUS_DELETEDALLOWED )
	{
		// 设置时间,设置为删除冷却状态(删除时间大于现在时间)		
		tpPlayer->SetRoleDleteTime( tpMsg->roleid(), DELETECOOLTIMESPACE + tDeleteCooltime + tpPlayer->GetRoleDeleteTime( tpMsg->roleid() ) );
		
		CPlayerManager::GetSingleton().UpDatePlayerInfo( tpPlayer, tpMsg->roleid(), 0, 0 );
		
		CMessage tMsgHead;
		CMessageCancelDeleteRoleResponse tMsgCancelDelteRole;
		tMsgHead.mutable_msghead()->set_messageid( ID_G2C_RESPONSE_CANCELROLEDELETE );
		tMsgHead.set_msgpara( (int)&tMsgCancelDelteRole );
		pbmsg_sethead(&tMsgHead, ID_G2C_RESPONSE_CANCELROLEDELETE, 0, 0, 0, 0, 0, 0);  

		
		tMsgCancelDelteRole.set_roleid( tpMsg->roleid() );
		tMsgCancelDelteRole.set_errcode( SUCCESS );
		CGateServer::GetSingleton().Send2Player(tpPlayer, &tMsgHead);
	}		
}

void CCoreService::OnAGIPMsgUserLogoutTimeOut(int nParam1, int nParam2)
{
	LOG_ERROR("erating", "OnAGIPMsgUserLogoutResponse  fault because of ERatingTimeout");
}

void CCoreService::OnAGIPMsgEnterGameTimeOut(int nParam1, int nParam2)
{
	int nAccountID = nParam1;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nAccountID);
	if(tpPlayer == NULL)
	{
		LOG_NOTICE("default", "[ %s : %d ][ %s ] AccountID(%d) has left", 
				__LK_FILE__, __LINE__, __FUNCTION__, nAccountID);
		return;
	}

	LOG_ERROR("default", "[ %s : %d ][ %s ] AccoundID(%d) use RoleID(%d) EnterGame failure Because of ERating Timeout!",  
			__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, tpPlayer->mCharID);

	// CPlayerManager::GetSingleton().PlayerEnterGame(tpPlayer);
}

/*
void CCoreService::OnAGIPMsgPasswdVerifyTimeOut(int nParam1, int nParam2)
{
	int nAccountID = nParam1;                             
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(nAccountID);  
	if(tpPlayer == NULL)                                  
	{                                                     
		LOG_INFO("erating", "AccountID(%d) has left",     
				__LK_FILE__, __LINE__, __FUNCTION__, nAccountID);
		return;                                           
	}                                                     

	// Erating认证超时, 不能删除角色
	LOG_ERROR("default", "[ %s : %d ][ %s ] AccoundID(%d) delete role(%d) failure Because of ERating Timeout!",
				__LK_FILE__, __LINE__, __FUNCTION__, nParam1, nParam2);
	CPlayerManager::GetSingleton().SendGateErrorNotice(
			tpPlayer, EGATE_DELETEROLE_ON_AGNOTOPEN, em_keep_connect);
}
*/

void CCoreService::OnServerMessage(int nServerID, CMessage* tpMessage)
{
	CMessageHead* tpHead = tpMessage->mutable_msghead();
	unsigned short nMessageID = (unsigned short) tpHead->messageid();

	switch( nMessageID ) 
	{
		// 玩家请求切换场景
		case ID_S2G_PLAYER_REDIRECT_REQUEST:
		{
			OnMessagePlayerRedirectScene( nServerID, tpMessage );
			break;
		}

		// 玩家重定向场景后的进入地图响应
		case ID_S2G_REDIRECT_ENTER_MAP_RESPONSE:
		{
			OnMessageRedirectEnterMapResponse( nServerID, tpMessage );
			break;
		}

		// 玩家重定向场景错误
		case ID_S2G_REDIRECT_S_ERROR_CODE:
		{
			OnMessageRedirectSErrorEvent( nServerID, tpMessage );	
			break;
		}

		// 场景通知网关玩家登录游戏完成
		case ID_S2G_PLAYER_ENTER_CONFIRM_NOTICE:
		{
			OnMessagePlayerEnterConfirm(nServerID, tpMessage);
			break;
		}

		// 场景通知网关已经准备好接收玩家进入
		case ID_S2G_PLAYER_ENTER_MAP_RESPONSE:
		{
			OnMessageScenePlayerEnterMapResponse( nServerID, tpMessage );
			break;
		}

		// 场景通知玩家将要离开
		case ID_S2G_PLAYER_LEAVE_MAP_NOTICE:
		{
			OnMessagePlayerLeaveMapNotice( nServerID, tpMessage);
			break;
		}

		// 登录服务器通知玩家要来了,网关记录登陆帐号和令牌,等待连接
		case ID_L2G_PLAYER_ENTER_GATE_NOTICE:
		{
			OnMessagePlayerEnterGateNotice(tpMessage);
			break;
		}

		// 网关踢人回应消息
		case ID_S2G_KICKOFF_PLAYER_RESPONSE:
		{
			OnMessageKickOffPlayerResponse(nServerID, tpMessage);
			break;
		}

		case ID_S2G_ENTITY_GM_MOVE:
		{
			OnMessageEntityGMMove( nServerID, tpMessage );
			break;
		}
		
		case ID_S2G_ROLE_MOVE:
		{
			OnMessageRoleMove( nServerID, tpMessage );
			break;
		}
		
		case ID_S2G_NOTIFY_CHANGEPASSWORDSTATUS:
		{
			OnMessageChangePassWordStatus( nServerID, tpMessage );
			break;
		}

		// 防沉迷请求
		/*
		case ID_S2G_FANGCHENMI_REQUEST:
		{
			CServiceGateFcm::GetSingleton().OnMessage(nServerID, tpMessage);
			break;
		}
		*/

		// 游戏服务器通知玩家要回来了,网关记录登陆帐号和令牌,等待连接
		/*
		case ID_S2G_PLAYER_RETURN2GATE_NOTICE:
		{
			OnMessagePlayerReturn2GateNotice(nServerID, tpMessage);
			break;
		}
		*/

		case ID_S2G_ROLECHANGECAMP_NOTICE:
		{
			OnMessageRoleChangeCampNotice(nServerID, tpMessage);
			break;
		}
		case ID_S2G_RESPONSE_RESAVEPLAYER:
		{
			OnMessageResavePlayerSuccess(nServerID, tpMessage);
			break;
		}

		case ID_S2G_LISTIBITEMS_REQUEST:
		case ID_S2G_PURCHASEIBITEM_REQUEST:
		case ID_S2G_ROLEIBTRANS_NOTIFY:
		case ID_S2G_SYNIBSTORE_REQUEST:
		case ID_S2G_SYNFUNCSTORE_REQUEST:
		{
			CIbStoreModule::GetSingleton().OnMessage(nServerID, tpMessage);	
			break;
		}

		// 玩家个人信息改变
		case ID_S2G_NOTIFY_ROLEGAMEINFO2GATE:
		{
			OnMessagePersonalLevelChange( tpMessage );
			break;
		}

		// 玩家会员标志改变
		case ID_S2G_NOTIFY_VIPFLAG:
		{
			OnMessageUpdateVipFlag( nServerID, tpMessage );
			break;
		}

      // 100331 [GS] MAOCY ADD-BEGIN
      // 登录服务器查询角色信息请求
      case ID_L2G_REQUEST_ROLEQUERY:
		  {
			  OnMessageRoleQuery( nServerID, tpMessage );
			  break;
		  }
		  //scenserver 验证转阵营条件成功后 发送给gate看 gateserver是否验证成功
	  case ID_C2S_REQUEST_CHANGE_CAMP:
		  {
			  OnMessageChangeCampRequest(tpMessage);
			  break;
		  }
		   //scensever 删除道具成功 ，通知gate 修改排行榜信息，设置阵营id。
	  case ID_S2G_CHANGE_CAMP_NOTICE:
		  {
			  OnMessageChangeCampNotice(tpMessage);
			  break;
		  }
	  case ID_C2S_REQUEST_INVITE_CAMP:
		  {
			  OnMessageCampWeakRequest(tpMessage);
			  break;
		  }
		 
      // 100331 [GS] MAOCY ADD-END

      default:
		{
			LOG_ERROR( "default", "[%s:%d] no one care this messgeid %d",
				__FUNCTION__, __LINE__, tpMessage->msghead().messageid() );
			break;
		}
	}
}

void CCoreService::OnClientMessage(CMessage* tpMessage)
{
	CMessageHead* tpHead = tpMessage->mutable_msghead();
	unsigned short nMessageID = (unsigned short) tpHead->messageid();
	int vCommHandle = tpHead->srcid();

	switch(nMessageID)
	{
		// 玩家进入网关请求
		case ID_C2G_REQUEST_LOGINGATE:
		{
			OnMessagePlayerLoginGateRequest( vCommHandle,  tpMessage );
			break;
		}

		// 玩家请求建立角色
		case ID_C2G_REQUEST_CREATECHAR:
		{
			OnMessagePlayerCreateCharRequest( vCommHandle,  tpMessage );
			break;
		}

		// 玩家请求删除角色
		case ID_C2G_REQUEST_DELETECHAR:
		{
			OnMessagePlayerDeleteCharRequest( vCommHandle, tpMessage);
			break;
		}

		// 玩家请求角色列表
		case ID_C2G_REQUEST_OBTAINCHAR:
		{
			OnMessagePlayerObtainCharsRequest(vCommHandle, tpMessage);
			break;
		}

		// 玩家进入游戏请求
		case ID_C2G_REQUEST_ENTERGAME:
		{
			OnMessagePlayerEnterGameRequest( vCommHandle, tpMessage);
			break;
		}
		
		
		// 删除角色时的密码检查	
		case ID_C2G_REQUEST_CHECKROLEPWD:
		{
			OnMessageCheckRolePwd( vCommHandle, tpMessage  );
			break;
		}	
		
		// 取消删除角色
		case ID_C2G_REQUEST_CANCELROLEDELETE:
		{
			OnMessageCancelDeleteRole( vCommHandle, tpMessage );
			break;
		}	
		

		// 玩家取消排队
		case ID_S2G_CANCEL_LOGINQUEUE:
		{
			OnMessagePlayerCancelLoginQueue( vCommHandle, tpMessage  );
			break;
		}

      // 100304 MAOCY ADD-BEGIN
      //------------------------------------------------------------
      // 请求问题
      case ID_C2G_REQUEST_QUESTIONREQUIRE:
         {
            OnMessageQuestionRequireRequest(vCommHandle, tpMessage);
            break;
         }

         //------------------------------------------------------------
         // 答案验证
      case ID_C2G_REQUEST_QUESTIONANSWER:
         {
            OnMessageQuestionAnswerRequest(vCommHandle, tpMessage);
            break;
         }
      // 100304 MAOCY ADD-END

      // Ping
		case ID_C2S_REQUEST_PING:
		{
			OnMessagePlayerPingRequest( vCommHandle, tpMessage);
			break;
		}

		default:
		{
			pbmsg_delmsg(tpMessage);
			return;
		}
	}

	pbmsg_delmsg(tpMessage);
}

void CCoreService::OnEventPlayerDisconnectGate(int nSockIndex)
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GatePlayer(nSockIndex);
	if (tpPlayer == NULL) 
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] can NOT find player which sockfd is %d",
				__LK_FILE__, __LINE__, __FUNCTION__, nSockIndex );
		return;
	}

	if ( nSockIndex != tpPlayer->mCommHandle )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] why the sockfd(%d) which closed is NOT match player(%d)",
				__LK_FILE__, __LINE__, __FUNCTION__, nSockIndex, tpPlayer->mCommHandle );

		return;
	}

	LOG_NOTICE("default",  "[ %s : %d ][ %s ] accountid(%d) Client [socket:%d] close connetc from gateserver",
			__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->mAccountID, nSockIndex );

	CPlayerManager::GetSingleton().UnRegistPlayerComm(tpPlayer);

	switch (tpPlayer->mStatus)
	{
		case CGatePlayer::em_status_inwait_wait:
		{
			// 1. return2gate的这个状态，已经ClearScenePlayer了, 因此此处不处理; 
			CPlayerManager::GetSingleton().DestroyGatePlayer(tpPlayer);

			break;
		}

		case CGatePlayer::em_status_ingame_idle:
		case CGatePlayer::em_status_ongame_v4scene_logingame:
		case CGatePlayer::em_status_ingame_v4scene_redirectscene:
		case CGatePlayer::em_status_ingate_v4kickoff:
		case CGatePlayer::em_status_ingate_v4leavemap:
		{
			//    等待场景销毁玩家的时候处理
			break;
		}

		case CGatePlayer::em_status_ingate_v4loginqueue:
		{
			CPlayerManager::GetSingleton().RemovePlayerFromQueue(tpPlayer);
			CPlayerManager::GetSingleton().DestroyGatePlayer(tpPlayer);
			break;
		}

		case CGatePlayer::em_status_ingate_idle:
		default:
		{
			CPlayerManager::GetSingleton().DestroyGatePlayer(tpPlayer);
			break;
		}
	}
}

void CCoreService::OnLaunchServer()
{
	CPlayerManager::mShmPtr = mShmPtr;
	new CPlayerManager;

	// 注册加载黑名单的回调函数
	CGateServer::GetSingleton().RegisterLoadConfigCallback("../config/black.lst", CCoreService::LoadBlackListConfigCallback);
}

void CCoreService::OnExitServer()
{

}

void CCoreService::OnTimer(unsigned int vTickCount, unsigned int vTickOffset )
{

}

void CCoreService::OnMessage(int nServerID, CMessage* tpMessage)
{

}

void CCoreService::OnReloadConfig()
{
	bool ret  = CPlayerManager::GetSingleton().mBlackList.Load();
	LOG_INFO("default", "[ %s : %d ][ %s ] black list roles(%d) reload %s", 
			__LK_FILE__, __LINE__, __FUNCTION__, 
			CPlayerManager::GetSingleton().mBlackList.Size(), ret ? "successfully" : "failure");
}

int CBlackList::Size()
{
	return mList.size();
}

bool CBlackList::Append(int nServerID, int nRoleID, const char* szRoleName)
{
	if ( Find(nRoleID) )
	{
		LOG_INFO("default", "[ %s : %d ][ %s ] ../config/black.lst has already role(%d) ", 
				__LK_FILE__, __LINE__, __FUNCTION__, nRoleID);
		return true;
	}

	extern int errno;
	FILE* fp = fopen("../config/black.lst", "a");
	if ( fp == NULL)
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] Can NOT open or created file for role(%d), role_name(%s) ../config/black.lst, reason(%s)", 
				__LK_FILE__, __LINE__, __FUNCTION__, nRoleID, szRoleName ? szRoleName : "NULL", strerror(errno));

		return false;
	}

	time_t now = time(NULL);
	struct tm *tblock = localtime(&now);
	
	lk::CString128 buffer("");
	buffer.append("%d\t%02d\t%s\t%s", nRoleID, nServerID, szRoleName ? szRoleName : "NULL", asctime(tblock));

	if ( fputs(buffer.c_str(), fp) == EOF)
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] Can NOT append black list for role(%d), role_name(%s) to ../config/black.lst, reason(%s)", 
				__LK_FILE__, __LINE__, __FUNCTION__, nRoleID, szRoleName ? szRoleName : "NULL", strerror(errno));
		fclose(fp);

		return false;
	}

	if (fclose(fp))
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] fclose ../config/black.lst error. reason(%s)", 
				__LK_FILE__, __LINE__, __FUNCTION__, strerror(errno));
		return false;
	}

	pair<BLACKLIST::iterator, bool> ret = mList.insert(BLACKLIST::value_type(nRoleID, buffer));
	if (! ret.second )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] insert into %s to blacklist error, pls check ", 
				__LK_FILE__, __LINE__, __FUNCTION__, buffer.c_str());

		return false;
	}

	LOG_INFO("default", "[ %s : %d ][ %s ] black list push back %s successfully.",  
			__LK_FILE__, __LINE__, __FUNCTION__, buffer.c_str());

	return true;
}

bool CBlackList::Load()
{
	const char* szFileName = "../config/black.lst";

	mList.clear();

	extern int errno;
	FILE* fp = fopen(szFileName, "r");
	if ( fp == NULL )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] Can NOT open file %s, reason(%s)", 
				__LK_FILE__, __LINE__, __FUNCTION__, szFileName, strerror(errno));
		return false;
	}

	char buffer[256];
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		int nRoleID = atoi(buffer);
		pair<BLACKLIST::iterator, bool> ret = mList.insert(BLACKLIST::value_type(nRoleID, lk::CString128(buffer)));

		if (! ret.second )
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ] insert into %s to blacklist error, pls check ", 
					__LK_FILE__, __LINE__, __FUNCTION__, buffer);
			continue;
		}
	}

	LOG_INFO("default", "[ %s : %d ][ %s ] Load roles(%d) from ../config/black.lst", 
			__LK_FILE__, __LINE__, __FUNCTION__, mList.size());

	return fclose(fp) == 0;
}

bool CBlackList::Find(int nRoleID)
{
	BLACKLIST::iterator iter = mList.find(nRoleID);
	return iter != mList.end();
}

const char* CBlackList::GetStr(int nRoleID)
{
	BLACKLIST::iterator iter = mList.find(nRoleID);
	if( iter != mList.end() )
		return iter->second.c_str();
	return NULL;
}

bool CBlackList::Remove(int nRoleID)
{
	BLACKLIST::iterator iter = mList.find(nRoleID);
	if ( iter != mList.end())
	{
		mList.erase(iter);

		const char* szWriteFile = "../config/black.lst~";
		FILE* fpwrite = fopen(szWriteFile, "w+");

		extern int errno;

		if ( fpwrite == NULL) 
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ] fopen ../config/black.lst~ error for write",
					__LK_FILE__, __LINE__, __FUNCTION__);
			return false;
		}

		BLACKLIST::iterator iter = mList.begin();					
		for(; iter != mList.end(); ++iter)
		{
			if ( fputs(iter->second.c_str(), fpwrite)  == EOF) 
			{
				LOG_ERROR("default", "[ %s : %d ][ %s ] fprintf %s to ../config/black.lst~ error for write", 
						__LK_FILE__, __LINE__, __FUNCTION__, iter->second.c_str());
				return false;
			}
		}

		if (fclose(fpwrite) != 0)
		{
			return false;
		}

		const char* szReadFile = "../config/black.lst";
		if ( rename(szWriteFile, szReadFile) != 0)
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ] rename %s to %s error , reason(%s)", 
					__LK_FILE__, __LINE__, __FUNCTION__, szReadFile, szWriteFile, strerror(errno));
			return false;
		}
	}

	return true;
}

// 处理ping回应
void CCoreService::OnMessagePlayerPingRequest(int vCommHandle, CMessage* tpMessage)
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GatePlayer( vCommHandle );
	LK_ASSERT( tpPlayer != NULL && tpMessage != NULL && tpMessage->msgpara() != 0, return );

	CMessageClientPingServerRequest* pReqMsg = (CMessageClientPingServerRequest*) tpMessage->msgpara();

	CMessage tMessage;
	CMessageClientPingServerResponse tResMsg;
	tResMsg.set_clientlocaltime( pReqMsg->clientlocaltime() );
	
	pbmsg_sethead(&tMessage, ID_S2C_RESPONSE_PING, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResMsg);

	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage);
}

// 100304 MAOCY ADD-BEGIN
//============================================================
// 角色验证时，建立问题答案
//============================================================
void CCoreService::BuildQuestionOptionData(SRoleValidQuestionOption* pInputOption, SRoleValidQuestionOption* pOutputOption){
   CConfigure& vConfigure = CGateLogic::GetSingleton().mConfig;
   // 设置基础数据
   int length = pInputOption->length;
   int dateLength = length >> 1;
   uint16* pInput = (uint16*)pInputOption->data;
   uint16* pOutput = (uint16*)pOutputOption->data;
   pOutputOption->groupId = pInputOption->groupId;
   memcpy(pOutputOption->filename, pInputOption->filename, ROLE_VALID_QUESTION_DESCRIPTION_LENGTH);
   pOutputOption->id = pInputOption->id;
   pOutputOption->length = length ;
   // 生成混码
   int type = rand() % 2;
   uint16 randColor[24];
   int clength = (rand() % 12) + 12;
   for(int n=0; n<clength; n++){
      randColor[n] = rand() % vConfigure.roleValidQuestionRange;
   }
   // 生成图形
   uint16 bgcolor = rand() % 0xFFFF;
   for(int n=0; n<dateLength; n++){
      uint16 color = *pInput++;
      if(0 == color){
         color = bgcolor;
      }else if(type > 0){
         uint16 rcolor = randColor[n % clength];
         uint16 r = color >> 11;
         r = (r+rcolor < 0x1F) ? r + rcolor : 0x1F;
         uint16 g = (color >> 5) & 0x3F;
         g = (g+rcolor < 0x3F) ? g + rcolor : 0x3F;
         uint16 b = color & 0x001F;
         b = (b+rcolor < 0x1F) ? b + rcolor : 0x1F;
         color = (r<<11) | (g<<5) | b;
      }else{
         uint16 rcolor = randColor[n % clength];
         uint16 r = color >> 11;
         r = (r > rcolor) ? r - rcolor : 0;
         uint16 g = (color >> 5) & 0x3F;
         g = (g > rcolor) ? g - rcolor : 0;
         uint16 b = color & 0x001F;
         b = (b > rcolor) ? b - rcolor : 0;
         color = (r<<11) | (g<<5) | b;
      }
      *pOutput++ = color;
   }
}

//============================================================
// 角色验证时，请求问题
//============================================================
void CCoreService::BuildQuestionRequireResponse(CGatePlayer* tpPlayer, CMessageQuestionRequireResponse& response, SRoleValidQuestionOption* pResults)
{
   CConfigure& vConfigure = CGateLogic::GetSingleton().mConfig;
   // 设置种子
   srand(time(NULL));
   // 随机获取一个问题
   SRoleValidQuestionOption* pOptions[4];
   SRoleValidQuestionOption* pResultOption;
   SRoleValidQuestionOption* pResultOptions[4];
   SRoleValidQuestion& question = vConfigure.roleValidQuestions[rand() % vConfigure.roleValidQuestionCount];
   LOG_DEBUG("default", "[%s:%d][%s] Random question. (question=%d, type=%d, result=%d)",
            __LK_FILE__, __LINE__, __FUNCTION__, question.id, question.typeId, question.resultId);
   // 生成其他所有选项
   int count = 0;
   SRoleValidQuestionOption* pAllOptions[ROLE_VALID_QUESTION_OPTION_COUNT * ROLE_VALID_QUESTION_GROUP_COUNT];
   for(int j=0; j<ROLE_VALID_QUESTION_GROUP_COUNT; j++){
      if(j != question.resultId){
         SRoleValidQuestionGroup& group = vConfigure.roleValidQuestionGroups[j];
         for(int i=0; i<group.count; i++){
            pAllOptions[count++] = &group.options[i];
         }
      }
   }
   // 根据类型获取问题
   switch(question.typeId){
      case 0:{
         // 获取第一个结果选项
         SRoleValidQuestionGroup& resultGroup = vConfigure.roleValidQuestionGroups[question.resultId];
         pResultOption = &resultGroup.options[rand() % resultGroup.count];
         pOptions[0] = pResultOption;
         // 随机选出3个其他选项
         for(int n=1; n<4; n++){
            int index = rand() % count;
            LOG_DEBUG("default", "[%s:%d][%s] Option random (n=%d, index=%d)", __LK_FILE__, __LINE__, __FUNCTION__, n, index);
            pOptions[n] = pAllOptions[index];
         }
         break;
         }
      // 四个组中选取非结果的一个(另外3个随机一组)
      case 1:{
         // 随机选出一个结果选项
         pResultOption = pAllOptions[rand() % count];
         pOptions[0] = pResultOption;
         // 随机选出一个其他选项
         SRoleValidQuestionGroup& resultGroup = vConfigure.roleValidQuestionGroups[question.resultId];
         for(int n=1; n<4; n++){
            int index = rand() % resultGroup.count;
            LOG_DEBUG("default", "[%s:%d][%s] Option random (n=%d, index=%d)", __LK_FILE__, __LINE__, __FUNCTION__, n, index);
            pOptions[n] = &resultGroup.options[index];
         }
         break;
         }
      // 四个组中选取非结果的一个(另外3个为一组)
      case 2:{
         // 获得两组不一样结果组
         int resultIndex = rand() % ROLE_VALID_QUESTION_GROUP_COUNT;
         SRoleValidQuestionGroup* pResultGroup = &vConfigure.roleValidQuestionGroups[resultIndex];
         SRoleValidQuestionGroup* pOtherGroups[ROLE_VALID_QUESTION_GROUP_COUNT];
         int othrerIndex = 0;
         for(int n=0; n<ROLE_VALID_QUESTION_GROUP_COUNT; n++){
            if(n != resultIndex){
               pOtherGroups[othrerIndex++] = &vConfigure.roleValidQuestionGroups[n];
            }
         }
         SRoleValidQuestionGroup* pOtherGroup = pOtherGroups[rand() % (ROLE_VALID_QUESTION_GROUP_COUNT-1)];
         // 生成答案
         pOptions[0] = &pResultGroup->options[rand() % pResultGroup->count];
         for(int n=1; n<ROLE_VALID_QUESTION_GROUP_COUNT; n++){
            pOptions[n] = &pOtherGroup->options[rand() % pOtherGroup->count];
         }
         break;
         }
   }
   // 混码处理
   for(int n=0; n<4; n++){
      BuildQuestionOptionData(pOptions[n], &pResults[n]);
   }
   for(int n=0; n<4; n++){
      pOptions[n] = &pResults[n];
   }
   // 记住结果
   pResultOption = &pResults[0];
   LOG_INFO("default", "[%s:%d][%s] Member question result. (question=%d, group=%d, option=%d, file=%s(%d)",
         __LK_FILE__, __LINE__, __FUNCTION__, question.id,
         pResultOption->groupId, pResultOption->id, pResultOption->filename, pResultOption->length);
   tpPlayer->mRoleOptionDataLength = pResultOption->length;
   memcpy(tpPlayer->mRoleOptionData, pResultOption->data, pResultOption->length);
   // 随机选出4个结果
   for(int n=0; n<ROLE_VALID_QUESTION_GROUP_COUNT; n++){
      int index = rand() % (ROLE_VALID_QUESTION_GROUP_COUNT - n);
      LOG_DEBUG("default", "[%s:%d][%s] Result random (index=%d)", __LK_FILE__, __LINE__, __FUNCTION__, index);
      pResultOptions[n] = pOptions[index];
      for(int i=index; i<ROLE_VALID_QUESTION_GROUP_COUNT - 1; i++){
         pOptions[i] = pOptions[i+1];
      }
   }
   // 填充信息
   response.set_status(CMessageQuestionRequireResponse::STATUS_WAIT);
   response.set_questionid(question.id);
   response.set_question(question.description);
   for(int n=0; n<4; n++){
      LOG_DEBUG("default", "[%s:%d][%s] Send question (question=%d, group=%d, option=%d, file=%s(%d)",
         __LK_FILE__, __LINE__, __FUNCTION__, question.id,
         pResultOptions[n]->groupId, pResultOptions[n]->id, pResultOptions[n]->filename, pResultOptions[n]->length);
   }
   // 设置选项
   response.set_optiona(pResultOptions[0]->data, pResultOptions[0]->length);
   response.set_optionb(pResultOptions[1]->data, pResultOptions[1]->length);
   response.set_optionc(pResultOptions[2]->data, pResultOptions[2]->length);
   response.set_optiond(pResultOptions[3]->data, pResultOptions[3]->length);
}

//============================================================
// 角色验证时，请求问题
//============================================================
void CCoreService::OnMessageQuestionRequireRequest(int vCommHandle, CMessage* npMessage)
{
   LOG_DEBUG("default", "[%s:%d][%s] Receive question require message. [handle=%d]",
      __LK_FILE__, __LINE__, __FUNCTION__, vCommHandle);
   CConfigure& vConfigure = CGateLogic::GetSingleton().mConfig;
   // 获得用户信息
   CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GatePlayer(vCommHandle);
   if(NULL == tpPlayer){
      return;
   }
   CMessage tMessage;
   // 判断是否使用
   if( vConfigure.roleValidQuestionFlag )
   {     
	  // 生成验证问题
	  CMessageQuestionRequireResponse response;
	  SRoleValidQuestionOption options[4];
	  BuildQuestionRequireResponse(tpPlayer, response, options);
	  // 回应请求
	  pbmsg_sethead(&tMessage, ID_G2C_RESPONSE_QUESTIONREQUIRE, 0, 0, 0, 0, 0, 0);
	  tMessage.set_msgpara((uint32_t)&response);
	  // 回应请求
	  CGateServer::GetSingleton().Send2Player(tpPlayer, &tMessage);
	  /*}*/
	  LOG_DEBUG("default", "[%s:%d][%s] Receive question answer success send. [handle=%d]",
		  __LK_FILE__, __LINE__, __FUNCTION__, vCommHandle);
   }  
   else
   {
	   // 不用认证
	   CMessageQuestionAnswerResponse response;
	   response.set_status(CMessageQuestionAnswerResponse::STATUS_SUCCESS);
	   pbmsg_sethead(&tMessage, ID_G2C_RESPONSE_QUESTIONANSWER, 0, 0, 0, 0, 0, 0);
	   tMessage.set_msgpara((uint32_t)&response);
	   // 回应请求
	   CGateServer::GetSingleton().Send2Player(tpPlayer, &tMessage);	
   } 
}

//============================================================
// 角色验证时，答案验证
//============================================================
void CCoreService::OnMessageQuestionAnswerRequest(int vCommHandle, CMessage* npMessage)
{
   LOG_DEBUG("default", "[%s:%d][%s] Receive question answer message. [handle=%d]",
         __LK_FILE__, __LINE__, __FUNCTION__, vCommHandle);
   CConfigure& vConfigure = CGateLogic::GetSingleton().mConfig;
   // 获得传入消息
   CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GatePlayer(vCommHandle);
   if(NULL == tpPlayer){
      return;
   }
   CMessageQuestionAnswerRequest* pRequest = (CMessageQuestionAnswerRequest*)npMessage->msgpara();
   std::string answer = pRequest->answer();
   // 判断结果
   bool result = false;
   if(0 == memcmp(tpPlayer->mRoleOptionData, answer.c_str(), answer.length())){
      result = true;
      LOG_DEBUG("default", "[%s:%d][%s] User select option success.", __LK_FILE__, __LINE__, __FUNCTION__);
   }
   // 设置认证消息
   tpPlayer->mRoleValid = result;
   // 填充信息
   CMessage tMessage;
   if(result){
      // 剩余次数重置
      tpPlayer->mRoleValidRetry = 0;
      // 回复问题通过
      CMessageQuestionAnswerResponse response;
      response.set_status(CMessageQuestionAnswerResponse::STATUS_SUCCESS);
      pbmsg_sethead(&tMessage, ID_G2C_RESPONSE_QUESTIONANSWER, 0, 0, 0, 0, 0, 0);
      tMessage.set_msgpara((uint32_t)&response);
      // 回应请求
      LOG_DEBUG("default", "[%s:%d][%s] Receive question answer success send. [handle=%d]",
         __LK_FILE__, __LINE__, __FUNCTION__, vCommHandle);
      CGateServer::GetSingleton().Send2Player(tpPlayer, &tMessage);
   }else{
      // 剩余次数减少
      tpPlayer->mRoleValidRetry++;
      if(tpPlayer->mRoleValidRetry > vConfigure.roleValidQuestionRetry){
         // 恢复失败计数器
         tpPlayer->mRoleValidRetry = 0;
         // 踢出用户
		 LOG_NOTICE("default", "[%s:%d][%s] Receive question answer send kick out account:%d [handle=%d]",
            __LK_FILE__, __LINE__, __FUNCTION__,  tpPlayer->AccountID(), vCommHandle );
         CPlayerManager::GetSingleton().SendGateErrorNotice(tpPlayer, EGATE_ROLE_QUESTION_FAILURE, em_need_disconnect);
         CPlayerManager::GetSingleton().DestroyGatePlayer(tpPlayer);
      }else{
         // 发送消息
         CMessageQuestionRequireResponse response;
         SRoleValidQuestionOption options[4];
         BuildQuestionRequireResponse(tpPlayer, response, options);
         pbmsg_sethead(&tMessage, ID_G2C_RESPONSE_QUESTIONREQUIRE, 0, 0, 0, 0, 0, 0);
         tMessage.set_msgpara((uint32_t)&response);
         // 回应请求
         LOG_INFO("default", "[%s:%d][%s] Receive question answer failure send. [handle=%d, retry=%d]",
            __LK_FILE__, __LINE__, __FUNCTION__, vCommHandle, tpPlayer->mRoleValidRetry);
         CGateServer::GetSingleton().Send2Player(tpPlayer, &tMessage);
      }
   }
}
// 100304 MAOCY ADD-END

// 100331 [GS] MAOCY ADD-BEGIN
//============================================================
// 登录服务器查询角色信息请求
//============================================================
void CCoreService::OnMessageRoleQuery(int nServerID, CMessage* pMessage){
   // 获得查询信息
   CMessageRoleQueryRequest* pRequest = (CMessageRoleQueryRequest*)pMessage->msgpara();
   int playerId = pRequest->playerid();
   int accountId = pRequest->accountid();
   // 获得角色个数
   int roleCount = CServiceRoleIDName::GetSingleton().RoleCount(accountId);
   // 设置返回信息
   CMessageRoleQueryResponse response;
   response.set_status(CMessageRoleQueryResponse::STATUS_SUCCESS);
   response.set_accountid(accountId);
   response.set_playerid(playerId);
   response.set_rolecount(roleCount);
   // 发送消息给登录服务器
   CMessage tMessage;
   pbmsg_sethead(&tMessage, ID_G2L_RESPONSE_ROLEQUERY, 0, 0, 0, 0, 0, 0); 
   tMessage.set_msgpara((uint32_t) &response);
   CGateServer::GetSingleton().Send2Login(&tMessage);
}
// 100331 [GS] MAOCY ADD-END

void CCoreService::OnMessageEntityGMMove( int nServerID, CMessage* vpMsg )
{
	CMessageGateEntityGMMove* tpMsg = ( CMessageGateEntityGMMove* )vpMsg->msgpara( );

	CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( tpMsg->rolename( ).c_str( ) );

	if( tpPlayer != NULL )
	{
		if( tpMsg->key( ) == INVITED_KEY_VALUE )
		{
			CMessageSceneEntityGMMove tMove;
			CMessage tMsg;

			tMove.set_roleid( tpPlayer->CharID( ) );
			tMove.set_lineid( tpMsg->lineid( ) );
			tMove.set_mapid( tpMsg->mapid( ) );
			tMove.set_posx( tpMsg->posx( ) );
			tMove.set_posy( tpMsg->posy( ) );
			tMove.set_key( tpMsg->key( ) );

			tMsg.set_msgpara( (uint32_t) &tMove );
			pbmsg_sethead( &tMsg, ID_G2S_ENTITY_GM_MOVE, 0, 0, 0, 0, 0, 0 );

			CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsg );
		}
		else
		{
			CRepetionModule::GetSingletonPtr( )->NotifyPlayerIntoRepetion( tpPlayer, tpMsg->lineid( ),
				tpMsg->mapid( ), tpMsg->key( ), tpMsg->posx( ), tpMsg->posy( ) );	
		}
	}
}


void CCoreService::OnMessageRoleMove( int vCommHandle, CMessage* pMessage )
{
	CMessageS2GRoleMove* tpMsg = ( CMessageS2GRoleMove* )pMessage->msgpara( );

	CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( tpMsg->desrole( ).c_str( ) );

	if( tpPlayer != NULL )
	{
		CMessageG2SRoleMove tMove;
		CMessage tMsg;

		tMove.set_srcrole( tpMsg->srcrole( ).c_str( ) );
		tMove.set_desrole( tpMsg->desrole( ).c_str( ) );

		tMsg.set_msgpara( (uint32_t) &tMove );
		pbmsg_sethead( &tMsg, ID_G2S_ROLE_MOVE, 0, 0, 0, 0, 0, 0 );

		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsg );
	}
	else
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Find Player( %s ) Form ScenePlayer", __LK_FILE__,
			__LINE__, __FUNCTION__, tpMsg->desrole( ).c_str( )  );
	}
}


// 处理玩家安全密码状态改变的消息
void CCoreService::OnMessageChangePassWordStatus(int vCommHandle, CMessage* pMessage )
{	
	LK_ASSERT( pMessage != NULL, return )	
	CMessageChangePasswordStatusNotify *tpMsg = ( CMessageChangePasswordStatusNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	
	
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );
	LK_ASSERT( tpPlayer != NULL, return )
	tpPlayer->SetPasswordStatus( tpMsg->roleid(), tpMsg->pwdstatus() );
	if ( tpMsg->has_password() )
	{
		tpPlayer->ModifyRolePwd( tpMsg->roleid(), tpMsg->mutable_password()->c_str() );
	}
}

// 修改网关玩家vip标志 [2/1/2010 Macro]
void CCoreService::OnMessageUpdateVipFlag(int nServerID, CMessage *npMessage)
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageSGProVipFlagNotice* tpNotice = ( CMessageSGProVipFlagNotice* ) npMessage->msgpara();

	CRoleGlobalInfo* tpRole = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpNotice->roleid() );
	if ( tpRole == NULL )
	{
		return ;
	}

	tpRole->SetVipFlag( tpNotice->vipflag() );	
	
	return ;
}


// ***************************************************************
//  Function:		CCoreService::OnMessageResavePlayerSuccess
//  Description:	处理重新存盘成功
// ***************************************************************
void CCoreService::OnMessageResavePlayerSuccess(int nServerID, CMessage* pMessage)
{
	LK_ASSERT( pMessage != NULL && pMessage->msgpara() != 0 , return );
	CMessageReSavePlayerResponse* pResponse = (CMessageReSavePlayerResponse *) pMessage->msgpara();

	if ( pResponse->result() == 0 )
	{
		// 换line map id
		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(pResponse->accountid());
		if (tpPlayer == NULL) 
			return ;

		CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingate_idle);

		bool bFind = false;
		for ( unsigned int i = 0; i < tpPlayer->mSave.size(); i++ )
		{
			if ( pResponse->roleid() == tpPlayer->mSave[ i ].mCharID )
			{
				tpPlayer->mSave[ i ].mLineID = pResponse->lineid();
				tpPlayer->mSave[ i ].mMapID  = pResponse->mapid();
				tpPlayer->mSave[ i ].mPosX   = pResponse->posx();
				tpPlayer->mSave[ i ].mPosY   = pResponse->posy();
				bFind= true;
				break;
			}
		}
		if ( !bFind )
		{
			LOG_ERROR("default", "[RESAVE] role %d not in acc %d?", pResponse->roleid(),pResponse->accountid() );
			return;
		}
		CPlayerManager::GetSingleton().mBlackList.Remove(pResponse->roleid());
		LOG_NOTICE("default", "player %d has been removed from black.lst", pResponse->roleid() );
	}
}

///////////////////////////////////
// 消息：转阵营（转服）消息
// author： qiufufu
// date : 10-08-23
//////////////////////////////////////

//申请强弱列表
void CCoreService::OnMessageCampWeakRequest(CMessage *pMessage)
{
	LK_ASSERT( pMessage != NULL && pMessage->msgpara() != 0 , return );

	CMessageInviteCampRequest* pRequest = (CMessageInviteCampRequest *) pMessage->msgpara();
	LK_ASSERT( pRequest !=NULL , return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pRequest->playerid());
	LK_ASSERT( tpPlayer != NULL, return );

	//通知客户端 阵营列表
	CMessage tMsg;

	CMessageInviteCampResponse camplist;
	camplist.set_errorcode(SUCCESS);

	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		CampLevel* camp_list = camplist.add_camp_list();
		if ( camp_list != NULL)
		{
			camp_list->set_camp_id(i);

			camp_list->set_weak_level(CWorldModule::GetSingleton().m_WeakProtecter.GetWeekLevelByWordID(i));
		}
	}
	pbmsg_sethead(&tMsg, ID_S2C_RSPONSE_INVITE_CAMP, 0, 0, 0, 0, 0, 0);
	tMsg.set_msgpara((uint32_t) &camplist);

	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMsg );
	



}
//scenserver 验证条件成功后 发送给gate看 gateserver是否验证成功
void CCoreService::OnMessageChangeCampRequest(CMessage *pMessage)
{
	LK_ASSERT( pMessage != NULL && pMessage->msgpara() != 0 , return );

	CMessageChangeSingleCampRequest* pRequest = (CMessageChangeSingleCampRequest *) pMessage->msgpara();
	LK_ASSERT( pRequest !=NULL , return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pRequest->playerid());
	LK_ASSERT( tpPlayer != NULL, return );
	
	////判断转入的是否是齐天阵营
	//if (pRequest->targetcampid() == CWorldManager::GetSingleton().mWinWorld )
	//{
	//	SendChangeCampErrorResponse(tpPlayer,ERROR_CHANGE_CAMP_IN_BEST);
	//	return;
	//}
	////判断是否有拍卖

	//gate验证成功，回复scenserver
	CMessage tMsg;
	CMessageChangeSingleCampResponse tSuccesMsg;
	tSuccesMsg.set_errorcode(SUCCESS);
	tSuccesMsg.set_campid(pRequest->targetcampid());
	tSuccesMsg.set_playerid(pRequest->playerid());
	pbmsg_sethead(&tMsg, ID_S2C_RSPONSE_CHANGE_CAMP, 0, 0, 0, 0, 0, 0);
	tMsg.set_msgpara((uint32_t) &tSuccesMsg);
	CGateServer::GetSingleton().Send2Scene(tpPlayer, &tMsg);
}
//scensever 删除道具成功 ，gate 修改排行榜信息，设置阵营id。
void CCoreService::OnMessageChangeCampNotice(CMessage *pMessage)
{
	LK_ASSERT( pMessage != NULL && pMessage->msgpara() != 0 , return );

	CMessageChangeCamp2GateNotice* pNotice = ( CMessageChangeCamp2GateNotice *) pMessage->msgpara();
	LK_ASSERT( pNotice !=NULL , return );
	int oldcampId = 0;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pNotice->playerid());
	//修改阵营id
	if (tpPlayer == NULL)
	{
		LOG_FATAL("default", "[ %s : %d ][ %s ] role_id(%d) has left gate",
			__LK_FILE__, __LINE__, __FUNCTION__, pNotice->playerid());	
		return;
	}else
	{	//记录原始阵营id
		oldcampId = tpPlayer->mCountryID;

		tpPlayer->mCountryID = pNotice->campid();
	}

	
	//修改排行 信息
	CRankModule::GetSingleton().RemovePlayerFromRank( pNotice->playerid(), 0 );

	//通知客户端 修改阵营成功
	CMessage tMsg;

	CMessageChangeSingleCampResponse tErrorMsg;
	tErrorMsg.set_errorcode(SUCCESS);
	tErrorMsg.set_campid(pNotice->campid());
	pbmsg_sethead(&tMsg, ID_S2C_RSPONSE_CHANGE_CAMP, 0, 0, 0, 0, 0, 0);
	tMsg.set_msgpara((uint32_t) &tErrorMsg);

	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMsg );

	////世界广播 转阵营
	//CMessage tMessage;
	//CMessageChangeCampSuccesNotify tSuccesNotify;
	//tSuccesNotify.set_campid(pNotice->campid());
	//tSuccesNotify.set_srccampid(oldcampId);
	//tSuccesNotify.set_playerid(pNotice->playerid());
	//tSuccesNotify.set_playername(tpPlayer->CharName());

	//tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGE_CAMP_SUCCESED);
	//tMessage.set_msgpara( (uint32)&tSuccesNotify );

	//CPlayerManager::GetSingleton().SendWorldMessageFromGate( &tMessage );

	CRoleGlobalInfo*pRoleInfo =  CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pNotice->playerid());		
	if (pRoleInfo == NULL)
	{
		LOG_FATAL("default", "[ %s : %d ][ %s ] role_id(%d) is not in the global nameid table ", 
			__LK_FILE__, __LINE__, __FUNCTION__, pNotice->playerid());
		return;
	}
	pRoleInfo->SetCountryID(pNotice->campid());

}
// 发送给客户端 转阵营失败消息
void CCoreService::SendChangeCampErrorResponse(CGatePlayer* tpPlayer, ChangeCamp errorCode)
{
	CMessage tMsg;

	CMessageChangeSingleCampResponse tErrorMsg;
	tErrorMsg.set_errorcode(errorCode);
	pbmsg_sethead(&tMsg, ID_S2C_RSPONSE_CHANGE_CAMP, 0, 0, 0, 0, 0, 0);
	tMsg.set_msgpara((uint32_t) &tErrorMsg);

	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMsg );
}

// 玩家连接网关后直接进入场景
void CCoreService::PlayerDirectEnterGame( int nCommHandle, CGatePlayer *pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return )		
	pPlayer->mCommHandle = nCommHandle;
	CPlayerManager::GetSingleton().RegistPlayerComm( pPlayer );		
	SendGetGlobalRoleData2DB( pPlayer );
}

void CCoreService::LoadBlackListConfigCallback(const char* pcConfigPath)
{
	bool ret  = CPlayerManager::GetSingleton().mBlackList.Load();
	LOG_INFO("default", "[ %s : %d ][ %s ] black list roles(%d) reload %s", 
		__LK_FILE__, __LINE__, __FUNCTION__, 
		CPlayerManager::GetSingleton().mBlackList.Size(), ret ? "successfully" : "failure");
}