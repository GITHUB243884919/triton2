#include "globalmodule.h"
#include "globalserver_pb.hxx.pb.h"
#include "gateobj_manager.h"
#include "servermessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "gatelogic.h"
#include "core_service.h"
#include "repetionmodule.h"
#include "template.h"
#include "world.h"
#include "dungeontype.h"

template<>  CGlobalModule *CSingleton<CGlobalModule>::spSingleton = NULL;
// ***************************************************************
//  Function:       OnMessage
//  Description:	接收全局服务器的消息	
//  Date:           03/30/2011
// 
// ***************************************************************
void CGlobalModule::OnMessage( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return ) 
	switch ( pMessage->mutable_msghead()->messageid() )
	{
		case ID_D2G_REQUEST_CHANGESERVER:
		{
			OnMessageRoleGlobalTele( nServerID, pMessage );
			break;
		}
				
		case ID_D2G_REQUEST_CREATEREPETION:
		{
			OnMessageCreateRepetion( nServerID, pMessage );
			break;
		}

		case ID_D2S_NOTIFY_WARENABLE:
		{
			OnMessageWarEnableNotify( nServerID, pMessage );
			break;
		}

		case ID_S2S_NOTIFY_KICKGLOBALPLAYER:
		{
			OnMessageKickPlayer( nServerID, pMessage );
			break;
		}

		case ID_S2S_NOTIFY_KICKGLOBALPLAYERACK:
		{
			OnMessageKickGlobalPlayerACK( nServerID, pMessage );
			break;
		}

		case ID_D2S_RESPONSE_GLOBALTELEPORT :
		{
			OnMessageGlobalTeleResponse( nServerID, pMessage );
			break;
		}

		case ID_S2D_REQUEST_GLOBALTELEPORT:
		{
			OnMessagePlayerGlobalTele( nServerID, pMessage );
			break;
		}
	}
}

// ***************************************************************
//  Function:       CountSize
//  Description:		
//  Date:           03/30/2011
// 
// ***************************************************************
unsigned int CGlobalModule::CountSize()
{
	return sizeof(CGlobalModule);
}

// ***************************************************************
//  Function:       OnMessageRoleGlobalTele
//  Description:	玩家请求跨区组到本服务器	
//  Date:           03/30/2011
// 
// ***************************************************************
void CGlobalModule::OnMessageRoleGlobalTele( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageChangeServerRequest *tpMsg = ( CMessageChangeServerRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )

	// 如果人数已满，不能登陆
	CTemplateGlobalMapConfig *tpConfig = (CTemplateGlobalMapConfig*)CDataStatic::GetTemp( TEMP_GLOBALMAPCONFIG );
	if ( tpConfig == NULL )
	{
		return;
	}
	if ( CWorldManager::GetSingleton().GetPlayerTotal( GLOBAL_SCENESERVER_ID, tpConfig->mMapID ) >= tpConfig->mLimit
		&& tpMsg->lineid() == GLOBAL_SCENESERVER_ID )
	{
		SendRoleGlobalTeleResponse( EDUNGEON_ERROR_MAPFULL, tpMsg->roleid(), tpMsg->worldid(), tpMsg->serverkey( ), tpMsg->serverid(), NULL, 0 );
		return;
	}

	// 检查玩家是否已经在本网关里面（如果在的话只用重定向场景,如果不在先连接网关）
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );
	if ( tpPlayer == NULL )
	{
		tpPlayer =  CPlayerManager::GetSingleton().GetPlayerByAcct( tpMsg->accountid() );
		
	}	

	if ( tpPlayer == NULL )
	{
		// 关闭连接，等待客户端再次连接
		// 创建角色，并且等待连接网关
		tpPlayer = (CGatePlayer*) CGateObjManager::GetSingletonPtr()->CreateObject(OBJTYPE_PLAYER_AT_GATE);
		if( tpPlayer == NULL) 
		{
			LOG_FATAL("default", "Player object pool can NOT malloc an object!");		
			// 发送不能连接服务器的通知
			SendRoleGlobalTeleResponse( -1, tpMsg->roleid(), tpMsg->worldid(), tpMsg->serverkey( ), tpMsg->serverid(), NULL, 0 );
			return ;
		}		
	}
	else
	{
		return;
	}	
	tpPlayer->Initialize();
	tpPlayer->SetRoleWorldID( tpMsg->worldid() );
	tpPlayer->CharID( tpMsg->roleid() );		
	tpPlayer->MapID( tpMsg->mapid() );		
	tpPlayer->mPoint.Clear();
	tpPlayer->mPoint.mX = tpMsg->posx();
	tpPlayer->mPoint.mY = tpMsg->posy();	
	tpPlayer->SetGameID( tpMsg->gameid() );
	tpPlayer->m_tLastActiveTime = time( NULL );
	tpPlayer->mStatus = CGatePlayer::em_status_inwait_wait;
	tpPlayer->mLoginToken = time( NULL );
	CGatePlayer::SaveData tdata;
	tpPlayer->mSave.push_back( tdata );	
	tpPlayer->AccountID( tpMsg->accountid() );	
	tpPlayer->mJoinBattle[ tpMsg->mapid() ].mCamp = tpMsg->teamflag();
	tpPlayer->SetMac( tpMsg->mutable_mac()->c_str() );
	tpPlayer->mSave[0].mKey = tpMsg->key();	// 使用第一个数组的值存储key
	// 根据副本key获取线ID
	if ( tpMsg->key() > 0 )
	{
		CRepetion *tpRepetion = CRepetionModule::GetSingleton().GetRepetion( tpMsg->key() );
		if ( tpRepetion != NULL )
		{
			tpPlayer->mLineID = tpRepetion->GetLineID();
		}
		else
		{
			SendRoleGlobalTeleResponse( EDUNGEON_ERROR_MAPFULL, tpMsg->roleid(), tpMsg->worldid(), tpMsg->serverkey( ), tpMsg->serverid(), NULL, 0 );
			return;
		}
	}
	else
	{
		tpPlayer->mLineID = tpMsg->lineid();	
	}
	bool tRet = CPlayerManager::GetSingleton().RegistAccountIndex( tpPlayer );	
	if ( tRet == false )
	{
		CPlayerManager::GetSingleton().RemoveAccountIDIndex( tpPlayer );
		SendRoleGlobalTeleResponse( -1, tpMsg->roleid(), tpMsg->worldid(), tpMsg->serverkey( ), tpMsg->serverid(), NULL, tpPlayer->mLoginToken );
		CGateObjManager::GetSingleton().DestroyObject( tpPlayer->get_id() );
		return;
	}

	CPlayerManager::GetSingleton().DeletePlayerFromGlobalSceneMap( tpMsg->accountid() );	

	// 发送不能连接服务器的通知
	const char *tpAddr = CGateServer::GetSingleton().GetGateServerIP();
	SendRoleGlobalTeleResponse( SUCCESS, tpMsg->roleid(), tpMsg->currentworldid(), tpMsg->serverkey( ), tpMsg->serverid(), tpAddr, tpPlayer->mLoginToken );			
}

// ***************************************************************
//  Function:       SendRoleGlobalTeleResponse
//  Description:	发送跨服请求的回应消息	
//  Date:           03/30/2011
// 
// ***************************************************************
void CGlobalModule::SendRoleGlobalTeleResponse( int nErrcode, int nRoleID, int nWorldID, int nServerKey, int nServerID, const char *pAddr, int nLoginToken )
{
	CMessage tMsgHead;
	CMessageChangeServerResponse tMsgGlobalTele;
	tMsgHead.mutable_msghead()->set_messageid( ID_G2D_RESPONSE_CHANGESERVER );
	tMsgHead.set_msgpara( (int)&tMsgGlobalTele );
	tMsgGlobalTele.set_errcode( nErrcode );
	tMsgGlobalTele.set_roleid( nRoleID );
	tMsgGlobalTele.set_worldid( nWorldID );
	tMsgGlobalTele.set_serverkey( nServerKey );
	tMsgGlobalTele.set_serverid( nServerID );
	tMsgGlobalTele.set_logintoken( nLoginToken );
	if ( pAddr != NULL )
	{
		tMsgGlobalTele.set_addr( pAddr );
	}
	CGateServer::GetSingleton().Send2GlobalDunserver( &tMsgHead );
}

// ***************************************************************
//  Function:       OnMessageCreateRepetion
//  Description:	全局副本服务器创建副本的消息请求	
//  Date:           04/08/2011
// 
// ***************************************************************
void CGlobalModule::OnMessageCreateRepetion( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageCreateRepetionRequest *tpMsg = ( CMessageCreateRepetionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )

	KEY_TYPE tKey = CRepetionModule::GetSingletonPtr()->CreateBattleByIndex( tpMsg->reptionindex(), tpMsg->repetionlevel(), tpMsg->regkey(), tpMsg->regkey(), tpMsg->regkey() );
	if( tKey == INVITED_KEY_VALUE )
	{
		SendResponseCreateRepetion( EDUNGEON_ERROR_CREATEREPETIONERR, tpMsg->regkey(), 0 );
		LOG_ERROR( "repetion", "creat global  repetionkey error!" );
		return;
	}
	SendResponseCreateRepetion( SUCCESS, tpMsg->regkey(), tKey );
}

// ***************************************************************
//  Function:       SendResponseCreateRepetion
//  Description:	全局副本服务器创建副本的消息回应	
//  Date:           04/08/2011
// 
// ***************************************************************
void CGlobalModule::SendResponseCreateRepetion( int nErrcode, int nRegKey, KEY_TYPE nKey )
{	
	CMessage tMsgHead;
	CMessageCreateRepetionResponse tMsgCreateRepetion;
	tMsgHead.mutable_msghead()->set_messageid( ID_G2D_RESPONSE_CREATEREPETION );
	tMsgHead.set_msgpara( (int)&tMsgCreateRepetion );
	tMsgCreateRepetion.set_errcode( nErrcode );
	tMsgCreateRepetion.set_regkey( nRegKey );
	tMsgCreateRepetion.set_reptionkey( nKey );
	CGateServer::GetSingleton().Send2GlobalDunserver( &tMsgHead );
}

// ***************************************************************
//  Function:       OnMessageWarEnableNotify
//  Description:	玩家进入副本的消息通知	
//  Date:           04/08/2011
// 
// ***************************************************************
void CGlobalModule::OnMessageWarEnableNotify( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )			
	CMessageWarEnableNotify *tpMsg = ( CMessageWarEnableNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );
	if ( tpPlayer != NULL )
	{
		CGateServer::GetSingleton().Send2Scene( tpPlayer, pMessage );
	}		
}

// ***************************************************************
//  Function:       OnMessageKickPlayer
//  Description:	踢掉玩家的通知	
//  Date:           04/08/2011
// 
// ***************************************************************
void CGlobalModule::OnMessageKickPlayer( int nServerID, CMessage *pMessage )
{
	CMessageKickGlobalPlayerNotify *tpMsg = ( CMessageKickGlobalPlayerNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LOG_NOTICE( "default","player(role_id:%d) will be kicked", tpMsg->roleid() );
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );
	if ( tpPlayer != NULL )
	{		
		LOG_NOTICE( "default","player(role_id:%d,status:%d) will be kicked", tpMsg->roleid(), tpPlayer->mStatus );
		CPlayerManager::GetSingleton().KickOffGatePlayer( tpPlayer, 0 );
		CPlayerManager::GetSingleton().KickOffScenePlayer( tpPlayer, 0 );
	}
}

// ***************************************************************
//  Function:       踢出玩家的消息通知
//  Description:	OnMessageKickGlobalPlayerACK	
//  Date:           04/08/2011
// 
// ***************************************************************
void CGlobalModule::OnMessageKickGlobalPlayerACK( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageKickGlobalPlayerACK *tpMsg = ( CMessageKickGlobalPlayerACK * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CPlayerManager::GetSingleton().DeletePlayerFromGlobalSceneMap( tpMsg->accountid() );
}

// ***************************************************************
//  Function:       玩家请求跨服的通知(来自场景)
//  Description:	OnMessagePlayerGlobalTele	
//  Date:           04/08/2011
// 
// ***************************************************************
void CGlobalModule::OnMessagePlayerGlobalTele( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageGlobalTeleportRequest *tpMsg = ( CMessageGlobalTeleportRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );
	if ( tpPlayer != NULL )
	{			
		// 设置mac地址
		tpMsg->set_mac( tpPlayer->GetMac() );
		// 记录状态
		if ( tpMsg->teletype() == EGLOBALTELE_TYPE_GLOBALSERVER )
		{
			CGlobalData tGlobalData;
			tGlobalData.mRoleID = tpPlayer->CharID();
			CPlayerManager::GetSingleton().InsertPlayerToGlobalSceneMap( tpPlayer->AccountID(), tGlobalData );
		}				
		CGateServer::GetSingleton().Send2GlobalDunserver( pMessage );
	}
}

// ***************************************************************
//  Function:       玩家请求跨服的回应消息(来自场景)
//  Description:	OnMessageGlobalTeleResponse	
//  Date:           04/08/2011
// 
// ***************************************************************
void CGlobalModule::OnMessageGlobalTeleResponse( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageGlobalTeleportResponse *tpMsg = ( CMessageGlobalTeleportResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );
	if ( tpPlayer != NULL )
	{		
		if ( tpMsg->errcode() != SUCCESS || tpMsg->has_addr() == false )
		{			
			CPlayerManager::GetSingleton().DeletePlayerFromGlobalSceneMap( tpPlayer->AccountID() );
		}						
		CGateServer::GetSingleton().Send2Scene( tpPlayer, pMessage );
	}
}
