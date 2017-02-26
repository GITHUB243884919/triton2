
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tinyxml.h>
#include "gatelogic.h"
#include "repetionmodule.h"
#include "gateobj_manager.h"
#include "entity.h"
#include "servermessage_in.h"
#include "core_service.h"
#include "gateteam.h"
#include "gateteammanager.h"
#include "activitytype.h"
#include "family_manager.h"
#include "corpservice.h"
#include "log_msg_pb.hxx.pb.h"
#include "nameservice.h"
#include "world.h"
#include "errcode.h"
#include "repetionmessage_pb.hxx.pb.h"

void CRepetionModule::OnMessage( int vServerID, CMessage* vpMessage )
{
	REPE_ASSERT(vpMessage != NULL && vpMessage->msgpara() != 0, return);

	CMessageHead* tpHead = vpMessage->mutable_msghead();
	unsigned short nMessageID = (unsigned short) tpHead->messageid();

	switch (nMessageID)
	{
	case ID_S2G_DESTROY_REPETION_MAP:
		{
			CMessageDestroyRepetionMap* tpMessage = ( CMessageDestroyRepetionMap* )vpMessage->msgpara();
			REPE_ASSERT( tpMessage != NULL, break );
			CRepetion* tpRepetion = CRepetionModule::GetSingletonPtr( )->GetRepetion( tpMessage->key( ) );
			REPE_ASSERT_STR( tpRepetion != NULL, return, "Key %lld", tpMessage->key( ) );

			DestroyRepetion( tpRepetion, REPETION_END_COMMON, tpMessage->parame( ) );
			break;
		}
		// 请求小队进入战场等待队列
	case ID_S2G_BATTLE_TEAM_WAIT:
		{
			CMessageBattleTeamWait* tpMsg = ( CMessageBattleTeamWait* )vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );
			TeamWaitBattle( tpMsg->charid( ), tpMsg->level( ), tpMsg->battleid( ) );

		}
		break;
		// 玩家离开战场等待队列
	case ID_S2G_BATTLE_WAIT_LEAVE_REQUEST:
		{
			CMessageBattleWaitLeaveRequest* tpMsg = ( CMessageBattleWaitLeaveRequest* )vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );
			RemoveFromWait( tpMsg->charid( ), tpMsg->battleid( ) );
		}
		break;
		// 玩家进入战场
	case ID_S2G_BATTLE_PLAYER_JOIN:
		{
			CMessageBattlePlayerJoin* tpMsg = ( CMessageBattlePlayerJoin* )vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );

			SetPlayerJoin( tpMsg->charid( ), tpMsg->battleid( ), tpMsg->key( ), time( NULL ), tpMsg->campid( ) );
		}
		break;
	case ID_S2G_BATTLE_CREATE_REQUEST:
		{
			CMessageBattleCreateRequest* tpMsg = ( CMessageBattleCreateRequest* )vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );

			OnMessageBattleCreate( vServerID, tpMsg );	
		}
		break;
	case ID_S2G_CREATE_REPETION_ERR:
		{
			CMessageCreateRepetionErr* tpMsg = ( CMessageCreateRepetionErr* )vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );

			REPE_LOG( _LOG_ERROR_, "Cannt Create Repetion( %d : %lld ) In ServerID %d", 
				tpMsg->repetionindex( ), tpMsg->key( ), vServerID );

			CRepetion* tpRepetion = GetRepetion( tpMsg->key( ) );
			REPE_ASSERT_STR( tpRepetion != NULL, break, "Repetion %d : %lld", tpMsg->repetionindex( ), 
				tpMsg->key( ) );

			DestroyRepetion( tpRepetion, REPETION_END_CREATEERR );
		}
		break;
	case ID_S2G_INTO_REPETION_TEAM:
		{
			CMessageTeamIntoRepetion* tpMsg = ( CMessageTeamIntoRepetion* )vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );
			int tRtn = IntoRepetionTeam( tpMsg );
			if( tRtn != SUCCESS )
			{
				CPlayerManager::GetSingletonPtr( )->SendRedirErrCode( tpMsg->charid( ), 
					tRtn, DEFAULT_REPETION_LINE_ID, tpMsg->mapid( ) );	
			}
		}
		break;
	case ID_S2G_INTO_REPETION_SINGLE:
		{
			CMessageSingleIntoRepetion* tpMsg = ( CMessageSingleIntoRepetion* )vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );
			int tRtn = IntoRepetionSignle( tpMsg );
			if( tRtn != SUCCESS )
			{
				CPlayerManager::GetSingletonPtr( )->SendRedirErrCode( tpMsg->charid( ), 
					tRtn, DEFAULT_REPETION_LINE_ID, tpMsg->mapid( ) );
			}
		}
		break;
                //小队向固定报名时间的副本报名
     case  ID_S2G_BATTLE_TEAM_FIX_WAIT:
        {
			CMessageBattleTeamWait* tpMsg = ( CMessageBattleTeamWait* )vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );
			int tRtn = TeamFixWaitBattle( tpMsg->charid( ), tpMsg->level( ), tpMsg->battleid( ) );
			if( tRtn != SUCCESS )
			{
				CPlayerManager::GetSingletonPtr( )->SendRedirErrCode( tpMsg->charid( ), 
					tRtn, DEFAULT_REPETION_LINE_ID, tpMsg->mapid( ) );
			}
		}
        break;
		// 玩家离开固定时间报名的副本等待队列
	case ID_S2G_BATTLE_FIX_WAIT_LEAVE_REQUEST:
		{
			CMessageBattleWaitLeaveRequest* tpMsg = ( CMessageBattleWaitLeaveRequest* )vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );
			RemoveFromFixWait( tpMsg->charid( ), tpMsg->battleid( ) );
		}
		break;
                //个人向固定时间时间开始的副本报名
    case ID_S2G_BATTLE_SINGLE_FIX_WAIT:
        {
                        //个人的报名 
            CMessageBattleSingleFixWait *tpMsg = ( CMessageBattleSingleFixWait* )vpMessage->msgpara( );

			REPE_ASSERT( tpMsg != NULL, break );
			int tRtn = SingleFixWaitBattle( tpMsg->charid( ), tpMsg->level( ), tpMsg->battleid( ) );
			if( tRtn != SUCCESS )
			{
				CPlayerManager::GetSingletonPtr( )->SendRedirErrCode( tpMsg->charid( ), 
					tRtn, DEFAULT_REPETION_LINE_ID, tpMsg->mapid( ) );
			}
        }
                break;
		//处理
	case ID_S2G_REPETION_BATTLE_WINNER_NOTIFY:
		{
			CMessageBattleWinnerNotify *tpMsg = ( CMessageBattleWinnerNotify* )vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );
			if( tpMsg->haswinner() > 0 )
			{
				CGatePlayer *tpPlayer = CPlayerManager::GetSingletonPtr()->ScenePlayer( tpMsg->charid());

				int tIntParam[ 3 ] = { 0 };
				tIntParam[ 0 ] = tpMsg->battleid();
				tIntParam[ 1 ] = tpPlayer->GetNationalityID();
				tIntParam[ 2 ] = tpMsg->level();

				CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(tpPlayer->mCharID);
				CWorldModule::GetSingleton().SendEventNotify( NOTICE_BATTLE_WINNER, 0, 0, tIntParam, ARRAY_CNT(tIntParam), 
					pRoleInfo != NULL ? (char*) pRoleInfo->RoleName() : NULL, NULL );
			}else{
				
				int tIntParam[ 2 ] = { 0 };
				tIntParam[ 0 ] = tpMsg->battleid();
				tIntParam[ 1 ] = tpMsg->level();
				
				CWorldModule::GetSingleton().SendEventNotify( NOTICE_BATTLE_NOWINNER, 0, 0, tIntParam, ARRAY_CNT(tIntParam), 
					NULL, NULL );
			}


		}
		break;
	//固定时间开启的副本不包括报名副本
	case ID_S2G_JOIN_ONTIME_OPEN_BATTLE_REQUEST:
		{
			CMessageS2GJoinOnTimeOpenBattleRequest *tpMsg = ( CMessageS2GJoinOnTimeOpenBattleRequest *)vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );

			int tRtn = JoinOnTimeOpenBattle( tpMsg );
			if( tRtn != SUCCESS )
			{
				CPlayerManager::GetSingletonPtr( )->SendRedirErrCode( tpMsg->charid( ), 
					tRtn, DEFAULT_REPETION_LINE_ID, tpMsg->mapid( ) );
			}
		}
		break;
	//副本结束消息
	case ID_S2G_BATTLE_CLOSE:
		{
			CMessageS2GBattleClose *tpMsg = (CMessageS2GBattleClose *)vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );

			int tIntParam[ 2 ] = { 0 };
			tIntParam[ 0 ] = tpMsg->battleid( );
			tIntParam[ 1 ] = tpMsg->mapid( );

			CWorldModule::GetSingleton().SendEventNotify( NOTIFY_BATTLE_CLOSE, 0, 0, tIntParam, ARRAY_CNT(tIntParam), NULL, NULL );
		}
		break;
	case ID_S2G_INTO_CORPSBOSS_REPETION:
		{
			OnMessageIntoCorpsBossRepetion( vServerID,  vpMessage );
		}
		break;
	case ID_S2G_END_CORPSBOSS_REPETION:
		{
			CMessgeS2GEndCorpsBossRepetion *tpMsg = (CMessgeS2GEndCorpsBossRepetion *)vpMessage->msgpara( );
			REPE_ASSERT( tpMsg != NULL, break );
			CCorpservice::GetSingletonPtr()->SendNotifyCorpsBossRepetionEnd( tpMsg->corpsid());

		}
		break;
	case  ID_C2S_REQUEST_ENTERLASTREPETION:
		{
			OnMessageEnterLastRepetion( vServerID, vpMessage );
			break;
		}
	default:
		{

			REPE_LOG( _LOG_ERROR_, "there is no action for the message_id(%d)", nMessageID );
			return;
		}
	}
}

int CRepetionModule::IntoRepetionSignle( CMessageSingleIntoRepetion* vpMsg )
{
	REPE_ASSERT( vpMsg != NULL, return ERR_INVALID_PARAMETER );
	CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( vpMsg->charid( ) );
	REPE_ASSERT_STR( tpPlayer != NULL, return ERR_INVALID_PARAMETER, "CharID %d", vpMsg->charid( ) );

	if( IsRepetionMap( vpMsg->mapid( ) )  == false )
	{
		return ERR_REDIRECT_MAP_NOTOPEN;
	}

	//	LOG_NOTICE( "default", "[ %s : %d ][ %s ] Player( %d ) Team  Will Redirect Into Map( %d )", __LK_FILE__,
	//		__LINE__, __FUNCTION__, vpMsg->charid( ), vpMsg->mapid( ) );

	REPE_LOG( _LOG_NOTICE_, "Player( %d ) Team  Will Redirect Into Map( %d )", vpMsg->charid( ), vpMsg->mapid( ) );

	//	CRepetion* tpRepetion = CreateRepetion( vpMsg->mapid( ), vpMsg->level( ), vpMsg->parame1( ), vpMsg->parame2( ) );
	//	REPE_ASSERT_STR( tpRepetion != NULL, return ERR_INVALID_PARAMETER, "MapID %d, Level %d, Parame1 %d, Parame2 %d",
	//			vpMsg->mapid( ), vpMsg->level( ), vpMsg->parame1( ), vpMsg->parame2( ) );



	CRepetion* tpRepetion = GetRepetion( tpPlayer->mRepetionMap[ vpMsg->mapid( ) ] );

	int i = 0;
	int tTeamChar[ TEAM_NUMBER_MAX ] = { 0 };
	int tCount = 0;
	tCount = CServiceGateTeam::GetSingletonPtr( )->GetTeamList( vpMsg->charid( ), tTeamChar, 
		sizeof( tTeamChar ) / sizeof( tTeamChar[ 0 ] ) );

	for( i = 0; i < tCount && tpRepetion == NULL; i++ )
	{
		CGatePlayer* tpMember = CPlayerManager::GetSingletonPtr( )->ScenePlayer( tTeamChar[ i ] );
		if( tpMember != NULL )
		{
			if( tpMember->mRepetionMap[ vpMsg->mapid( ) ] != INVITED_KEY_VALUE )
			{
				tpRepetion = GetRepetion( tpMember->mRepetionMap[ vpMsg->mapid( ) ] );
			}
		}
		else
		{
			//			LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Find Player( %d )", __LK_FILE__, __LINE__, __FUNCTION__,
			//				tTeamChar[ i ] );

			REPE_LOG( _LOG_ERROR_, "Cannt Find Player( %d )", tTeamChar[ i ] );
		}
	}

	if( tpRepetion == NULL )
	{
		tpRepetion = CreateRepetionByMapID( vpMsg->mapid( ), vpMsg->level( ), vpMsg->charid( ), 0,
			vpMsg->parame1( ), vpMsg->parame2( ), INVITED_KEY_VALUE, vpMsg->removeitem( ) );
	}

	if( tpRepetion == NULL )
	{
		//		LOG_ERROR( "default", "[ %s : %d ][ %s ] Player( %d ) Cannt GetRepetion MapID %d", __LK_FILE__,
		//			__LINE__, __FUNCTION__, vpMsg->charid( ), vpMsg->mapid( ) );
		REPE_LOG( _LOG_ERROR_, "Player( %d ) Cannt GetRepetion MapID %d", vpMsg->charid( ), vpMsg->mapid( ) );
		return ERR_FULL;
	}
	else
	{
		//		LOG_NOTICE( "default", "[ %s : %d ][ %s ]Player( %d ) Want Into Repetion( %d : %lld )", __LK_FILE__, __LINE__, 
		//				__FUNCTION__, vpMsg->charid( ), tpRepetion->get_id( ), tpRepetion->GetKey( ) );
		REPE_LOG( _LOG_NOTICE_, "Player( %d ) Want Into Repetion( %d : %lld )", vpMsg->charid( ), 
			tpRepetion->get_id( ), tpRepetion->GetKey( ) );

		NotifyPlayerIntoRepetion( tpPlayer, tpRepetion->GetLineID( ), vpMsg->mapid( ), tpRepetion->GetKey( ),
			vpMsg->posx( ), vpMsg->posy( ) );
	}

	return SUCCESS;
}

int CRepetionModule::IntoRepetionTeam( CMessageTeamIntoRepetion* vpMsg )
{
	REPE_ASSERT( vpMsg != NULL, return ERR_INVALID_PARAMETER );

	if( IsRepetionMap( vpMsg->mapid( ) ) == false )
	{
		return ERR_REDIRECT_MAP_NOTOPEN;
	}

	//	LOG_NOTICE( "default", "[ %s : %d ][ %s ] Player( %d )'s Team  Will Redirect Into Map( %d )", __LK_FILE__,
	//		__LINE__, __FUNCTION__, vpMsg->charid( ), vpMsg->mapid( ) );

	REPE_LOG( _LOG_NOTICE_, "Player( %d )'s Team  Will Redirect Into Map( %d )", vpMsg->charid( ), vpMsg->mapid( ) );

	CRepetion* tpRepetion = CreateRepetionByMapID( vpMsg->mapid( ), vpMsg->level( ), vpMsg->charid( ), 0,
		vpMsg->parame1( ), vpMsg->parame2( ),  INVITED_KEY_VALUE, vpMsg->removeitem( ) );
	REPE_ASSERT_STR( tpRepetion != NULL, return ERR_EXIST, "MapID %d, Level %d, Parame1 %d, Parame2 %d",
		vpMsg->mapid( ), vpMsg->level( ), vpMsg->parame1( ), vpMsg->parame2( ) );

	int i = 0;
	int tTeamChar[ TEAM_NUMBER_MAX ] = { 0 };
	int tCount = 0;
	tCount = CServiceGateTeam::GetSingletonPtr( )->GetTeamList( vpMsg->charid( ), tTeamChar, 
		sizeof( tTeamChar ) / sizeof( tTeamChar[ 0 ] ) );

	//	LOG_NOTICE( "default", "[ %s : %d ][ %s ]Player( %d )'s Team PlayerCount Is %d",
	//			__LK_FILE__, __LINE__, __FUNCTION__, vpMsg->charid( ), tCount );

	REPE_LOG( _LOG_NOTICE_, "Player( %d )'s Team PlayerCount Is %d", vpMsg->charid( ), tCount );

	for( i = 0; i < tCount; i++ )
	{
		CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( tTeamChar[ i ] );
		if( tpPlayer != NULL )
		{
			//			LOG_NOTICE( "default", "[ %s : %d ][ %s ]Player( %d ) Want Into Repetion( %d ), Team Player( "
			//					"%d : %s ) In Repetion( %lld )", __LK_FILE__, __LINE__, __FUNCTION__, vpMsg->charid( ), 
			//					tpRepetion->get_id( ), tpPlayer->mCharID, tpPlayer->CharName( ), 
			//					tpRepetion->GetKey( ) );

			REPE_LOG( _LOG_NOTICE_, "Player( %d ) Want Into Repetion( %d ), Team Player( %d : %s ) In Repetion( %lld )",
				vpMsg->charid( ), tpRepetion->get_id( ), tpPlayer->mCharID, tpPlayer->CharName( ), 
				tpRepetion->GetKey( ) );

			NotifyPlayerIntoRepetion( tpPlayer, tpRepetion->GetLineID( ), vpMsg->mapid( ), tpRepetion->GetKey( ),
				vpMsg->posx( ), vpMsg->posy( ) );

			// 记录副本key			
			tpPlayer->SetRepetionKey( tpPlayer->CharID(), tpRepetion->GetKey() );
			tpRepetion->InsertPlayer( tpPlayer->CharID() );
		}
		else
		{
			//			LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Find Player( %d )", __LK_FILE__, __LINE__, __FUNCTION__,
			//				tTeamChar[ i ] );
			REPE_LOG( _LOG_ERROR_, "Cannt Find Player( %d )", tTeamChar[ i ] );
		}
	}
	return SUCCESS;
}

int CRepetionModule::JoinOnTimeOpenBattle(  CMessageS2GJoinOnTimeOpenBattleRequest *vpMsg )
{
	REPE_ASSERT( vpMsg != NULL, return ERR_INVALID_PARAMETER );

	if( IsRepetionMap( vpMsg->mapid( ) ) == false )
	{
		return ERR_REDIRECT_MAP_NOTOPEN;
	}

	CGatePlayer *tpPlayer = CPlayerManager::GetSingletonPtr()->ScenePlayer( vpMsg->charid());
	REPE_ASSERT_STR( tpPlayer != NULL, return ERR_INVALID_PARAMETER, "CharID %d", vpMsg->charid( ) );
	CRepetion* tpRepetion = NULL;
	int tRtn = ERR_REPETION_PVP_LEVEL_NOTFOUND;

	CRepetionInfo* tpInfo = GetRepetionMapInfo( vpMsg->battleid() );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "RepetionID %d", vpMsg->battleid());

	//找到副本
	for( unsigned int i = 0; i < ARRAY_CNT( mOnTimeRepetion[ vpMsg->battleid( ) ].mOnTimeLevelLimit ); i++)
	{
		OnTimeOpenLevelLimit * tLevelLimit = &mOnTimeRepetion[ vpMsg->battleid( ) ].mOnTimeLevelLimit[ i ];
		if( tpPlayer->Level( ) >= tLevelLimit->mLevelMin && tpPlayer->Level( ) <= tLevelLimit->mLevelMax )
		{
			KEY_TYPE tKey = tLevelLimit->mKey; 
			tpPlayer->mJoinBattle[ vpMsg->mapid() ].mKey = tKey;
			tpPlayer->mJoinBattle[ vpMsg->mapid() ].mTime = time( NULL );
			tpPlayer->mJoinBattle[ vpMsg->mapid() ].mCamp = 6;
			tpPlayer->mJoinBattle[ vpMsg->mapid() ].mTimeLimit = tpInfo->NotLimitTime();
			tpRepetion = GetRepetion( tKey );
			if( tpRepetion == NULL)
			{
				return ERR_REPETION_PVP_FULL;
			}
			if( tpRepetion->GetPlayerCountCur( ) >= tpRepetion->GetPlayerCountMax( ) )
			{
				tRtn = ERR_REPETION_PVP_FULL;
				SendEventCode2Scene( tpPlayer, ERR_REPETION_PVP_FULL, tpRepetion->GetRepetionIndex( ) );
				break;
			}


			NotifyPlayerIntoRepetion( tpPlayer, tpRepetion->GetLineID( ), vpMsg->mapid( ), tpRepetion->GetKey( ),
				vpMsg->posx( ), vpMsg->posy( ) );
			REPE_LOG( _LOG_NOTICE_, "Player( %d ) Into repetion Map( %d ),repetion id is ( %d ),player level is ( %d ),limitlevel is ( %d , %d)",
				vpMsg->charid( ), vpMsg->mapid( ),
				vpMsg->battleid( ), tpPlayer->Level( ), tLevelLimit->mLevelMin, tLevelLimit->mLevelMax );
			tRtn = SUCCESS;
		}
	}

	return tRtn;

};

void CRepetionModule::OnMessageIntoCorpsBossRepetion( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageS2GIntoCorpsBossRepetion *tpMsg = ( CMessageS2GIntoCorpsBossRepetion *)npMessage->msgpara(); 

	CGatePlayer* tpPlayer = (CGatePlayer*) CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "corps", "Player not found.ID[%d][%s:%d]", tpMsg->charid(), __LK_FILE__, __LINE__ );
		return ;
	}

	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid()) ;
	if(tpCorps == NULL)
	{
		LOG_ERROR( "corps", "can not find corps[%d][%s:%d]", tpMsg->corpsid(), __LK_FILE__, __LINE__ );
		return ;
	}
	
	if( ! tpCorps->m_CorpsRepetion.IsOpen())
	{
		//CCorpservice::GetSingletonPtr()->SendIntoCorpsBossRepetionResponse( tpMsg->charid(), ERROR_CORPBOSSREPETION_NOTOPEN );
		SendEventCode2Scene( tpPlayer, ERROR_CORPBOSSREPETION_NOTOPEN, _REPE_CORPSBOSS_INDEX_ );
		return;
	}

	KEY_TYPE tKey	= tpCorps->m_CorpsRepetion.GetRepetionKey();
	//int  tMapID		= tpCorps->m_CorpsRepetion.get	
	if ( tKey == INVITED_KEY_VALUE)
	{
		//CCorpservice::GetSingletonPtr()->SendIntoCorpsBossRepetionResponse(tpMsg->charid(), ERROR_CORPBOSSREPETION_NOTOPEN );
		SendEventCode2Scene( tpPlayer, ERROR_CORPBOSSREPETION_NOTOPEN, _REPE_CORPSBOSS_INDEX_ );
		return;
	}

	CRepetion* tpRepetion = GetRepetion( tKey );
	if( tpRepetion == NULL)
	{
		//CCorpservice::GetSingletonPtr()->SendIntoCorpsBossRepetionResponse(tpMsg->charid(), ERROR_CORPBOSSREPETION_NOTOPEN );
		SendEventCode2Scene( tpPlayer, ERROR_CORPBOSSREPETION_NOTOPEN, tpRepetion->GetRepetionIndex( ) );
		return;
	}
	
	if( tpRepetion->GetPlayerCountCur( ) >= tpRepetion->GetPlayerCountMax( ) )
	{
		SendEventCode2Scene( tpPlayer, ERR_REPETION_PVP_FULL, tpRepetion->GetRepetionIndex( ) );
		return;
	}

	NotifyPlayerIntoRepetion( tpPlayer, tpRepetion->GetLineID( ), tpMsg->mapid( ), tpRepetion->GetKey( ),
		tpMsg->postx( ), tpMsg->posty( ) );
}

void CRepetionModule::OnMessageEnterLastRepetion( int nServerID, CMessage* npMessage ) //副本重入的消息
{
	LK_ASSERT( npMessage != NULL, return );
	CMessageEnterLastReptionRequest *tpMsg = ( CMessageEnterLastReptionRequest *)npMessage->msgpara(); 
	LK_ASSERT( tpMsg != NULL, return ) 
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );
	LK_ASSERT( tpPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageEnterLastReptionResponse tEnterLastRepetion;	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ENTERLASTREPETION );
	tMsgHead.set_msgpara( (int)&tEnterLastRepetion );

	KEY_TYPE tKey = tpPlayer->GetRepetionKey( tpPlayer->CharID() );
	CRepetion *tpRepetion = GetRepetion( tKey );
	if ( tpRepetion == NULL )
	{
		tEnterLastRepetion.set_errcode( ERR_REPETION_REPETIONDESTORIED );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMsgHead );
		return;
	}
	CRepetionInfo *tpRepetionInfo = tpRepetion->GetRepetionInfo();
	if ( tpRepetionInfo == NULL )
	{
		tEnterLastRepetion.set_errcode( ERR_REPETION_REPETIONDESTORIED );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMsgHead );		
		return;
	}
	if ( tpRepetionInfo->GetRelogin() != 1 || tpRepetionInfo->GetRepetionIndex() != (int)tpMsg->index() )
	{
		tEnterLastRepetion.set_errcode( ERR_REPETION_REPETIONDESTORIED );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMsgHead );		
		return;
	}
	PlayerJoin( tpPlayer->CharID(), tpMsg->index(), tKey, 0, 0, true );
}

void CRepetionModule::SendLastRepetionIndex( CGatePlayer *pPlayer, int nIndex, ERepetionIndexType eType )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageRepetionIndexNotice tMsgRepetionIndex;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_REPETIONINDEX_NOTICE );
	tMsgHead.set_msgpara( (int)&tMsgRepetionIndex );
	tMsgRepetionIndex.set_repetionindex( nIndex );
	tMsgRepetionIndex.set_type( eType );
	tMsgRepetionIndex.set_roleid( pPlayer->CharID( ) );
	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );		
}