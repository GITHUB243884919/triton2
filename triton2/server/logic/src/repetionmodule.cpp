#include "stdafx.hpp"
#include "entity.h"
#include "property.h"
#include "message_pb.hxx.pb.h"
#include "coremessage_pb.hxx.pb.h"
#include "promessage_pb.hxx.pb.h"
#include  "sceneobj_manager.h"
#include "friendmodule.h"
#include "mapmodule.h"
#include "teammodule.h"
#include "propertymodule.h"
#include "errcode.h"
#include "repetionmodule.h"
#include "mapmodule.h"
#include "taskmodule.h"
#include "teammodule.h"
#include "mapobjmanager.h"
#include "repetionmessage_pb.hxx.pb.h"
#include "sceneobj_manager_imp.h"
#include "property_pb.hxx.pb.h"
#include "timer_define.h"
#include "npcmodule.h"
#include "entity_searcher.h"
#include "entity_searcher_imp.h"
#include "propertymodule.h"
#include "familymodule.h"
#include "servermessage_pb.hxx.pb.h"
#include "logmodule.h"
#include "gmmessage_pb.hxx.pb.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

enum{
        INTO_REPECTION_CHECK = 0,
        SIGNUP_REPECTION_CHECK =1,
};

template<> CRepetionModule* CSingleton< CRepetionModule >::spSingleton = NULL;

char CRepetionModule::sModuleName[ NAME_LENGTH ] = "Repetion";


unsigned int CRepetionModule::CountSize( )
{
	unsigned int tSize = sizeof( CRepetionModule );	
	return tSize;
}
// 启动服务器
void CRepetionModule::OnLaunchServer( )
{		
	OnLoadConfig( );	
}


// 退出服务器
void CRepetionModule::OnExitServer( )
{
	if( mLua != NULL ) 
	{
		lua_close( mLua );
		mLua = NULL;
	}
}

// 服务器Timer
void CRepetionModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{	
	if( mRepetionTime.Elapse( vTickOffset ) == true )
	{
		static unsigned int sLastTick = vTickCount;
		int tTickOffset = vTickCount - sLastTick;
		sLastTick = vTickCount;

		std::vector< int > tDelete;

		CSceneObjManagerImp* pSceneneObjManagerImp = CSceneObjManager::GetSingletonPtr( )->mSceneObjManagerImp;
		CSceneObjManagerImp::ObjMng_Repetion::iterator it;
		for( 	it = pSceneneObjManagerImp->mRepetion.begin( );
				it != pSceneneObjManagerImp->mRepetion.end( );
				it++
		   )
		{
			CRepetion* tpRepetion = &( *it );
			tpRepetion->OnTimer( tTickOffset, &tDelete );
		}

		std::vector< int >::iterator tDeleteIt;
		for( tDeleteIt = tDelete.begin(); tDeleteIt != tDelete.end( ); tDeleteIt++ )
		{
			CRepetion* tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( *tDeleteIt );
			if( tpRepetion != NULL )
			{
			//	DestroyRepetion( tpRepetion );
				tpRepetion->AllPlayerLeave( );
			}
		}
	}
}

// 建立场景
int CRepetionModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )
{
#ifdef _FB_
#endif
	return SUCCESS;
}

// 路由消息
void CRepetionModule::OnRouterMessage( CMessage* pMessage )
{
	REPE_ASSERT( pMessage != NULL, return );
	REPE_ASSERT( pMessage->has_msghead( ), return );
	const CMessageHead& tHead = pMessage->msghead( );

	switch( tHead.messageid( ) )
	{
		case ID_G2S_BATTLE_JOIN_NOTICE:
			{
				CMessageBattleJoinNotice* tpMsg = ( CMessageBattleJoinNotice* )pMessage->msgpara( );
				REPE_ASSERT( tpMsg != NULL, break );


				CEntityPlayer* tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid( ) );
				REPE_ASSERT_STR( tpPlayer != NULL, break, "CharID %d", tpMsg->charid( ) );

				// 如果有全局信息的话直接传送到副本
				CGlobalTeleInfo *tpTeleInfo = CGlobalModule::GetSingleton().GetGlobalTeleInfo( tpMsg->charid() );
				if ( tpTeleInfo != NULL )
				{
					// 直接传送					
					CRepetionInfo* tpInfo = GetRepetionInfo( tpMsg->battleid( ) );
					REPE_ASSERT_STR( tpInfo != NULL, break, "Cannt Get RepetionInfo %d", tpMsg->battleid( ) );
					PlayerIntoRepetion( tpPlayer, tpInfo->GetBeginMapID( ), 0, 0 );					
					CGlobalModule::GetSingleton().DeleteGlobalTeleInfo( tpMsg->charid() );
					return;
				}
				
				// 是否副本重入
				if ( tpMsg->intolastrepetion() == true )
				{
					// 直接传送					
					CRepetionInfo* tpInfo = GetRepetionInfo( tpMsg->battleid( ) );
					REPE_ASSERT_STR( tpInfo != NULL, break, "Cannt Get RepetionInfo %d", tpMsg->battleid( ) );					
					PlayerIntoRepetion( tpPlayer, tpInfo->GetBeginMapID( ), tpInfo->GetPosX(), tpInfo->GetPosY(), 0, true );	
					return;
				}
				
				CMessageS2CBattleJoinNotice tMsg;
				CMessage tMessage;

				tMsg.set_battleid( tpMsg->battleid( ) );
				tMsg.set_type( tpMsg->type( ) );


				tMessage.mutable_msghead()->set_messageid( ID_S2C_BATTLE_JOIN_NOTICE );
				tMessage.set_msgpara( (uint32)&tMsg );

				CSceneLogic::GetSingletonPtr( )->Send2Player( tpPlayer, &tMessage );

	
				LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Player( %s : %d ) Can Join Battle %d", __LK_FILE__,
					__LINE__, __FUNCTION__, tpPlayer->GetName( ), tpPlayer->GetCharID( ),  tpMsg->battleid( ) );

				if( tpMsg->battleid( ) == _REPE_CORPS_BATTLE_INDEX_ )
				{
					// 配置为0的话，表示不需要buff约束
					int nBattleBuff = CSceneCfgManager::GetSingletonPtr()->GetCorps_cfg()->mEnterbattlebuffid;

					if(	( nBattleBuff != 0 ) && ( tpPlayer->HasBuff( nBattleBuff ) == false ) )
					{
						// 有权进入进团战的玩家会上一个buff "火线驰援", 15分钟内buff消失，消失了就不能再进入
						int nBuffLeftTime =  std::max( ( SCRABBLE_BATTLE_BUFFTIME -  (int)( time(NULL) - tpMsg->battletime() ) ), 0 ) * 1000;
						CPropertyModule::GetSingletonPtr()->InsertBuff( tpPlayer, nBattleBuff, nBuffLeftTime );
					}
				}
				//把报名状态清零
				CPropertyPlayer* pProperty = (CPropertyPlayer*) tpPlayer->GetProperty();
				REPE_ASSERT_STR( pProperty != NULL, break, "CharID %d", tpMsg->charid( ) );

				CPlayerRepetion* tpPRepetion = pProperty->GetRepetion( );
				REPE_ASSERT_STR( tpPRepetion != NULL, break, "CharID %d", tpMsg->charid( ) );

				tpPRepetion->RepetionClearSignUp( tpMsg->battleid( ));
				
			}
			break;

		case ID_G2S_BATTLE_WAIT_NOTICE:
			{
				CMessageBattleWaitNotice* tpMsg = ( CMessageBattleWaitNotice* )pMessage->msgpara( );
				REPE_ASSERT( tpMsg != NULL, break );

				CEntityPlayer* tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid( ) );
				REPE_ASSERT_STR( tpPlayer != NULL, break, "CharID %d", tpMsg->charid( ) );
				
				CPropertyPlayer* pProperty = (CPropertyPlayer*) tpPlayer->GetProperty();
				REPE_ASSERT_STR( pProperty != NULL, break, "CharID %d", tpMsg->charid( ) );
				
				CPlayerRepetion* tpPRepetion = pProperty->GetRepetion( );
				REPE_ASSERT_STR( tpPRepetion != NULL, break, "CharID %d", tpMsg->charid( ) );

				tpPRepetion->RepetionSignUp( tpMsg->battleid( ) );

				CMessageBattleWaitResponse tMsg;
				CMessage tMessage;

				tMsg.set_battleid( tpMsg->battleid( ) );

				tMessage.mutable_msghead()->set_messageid( ID_S2C_BATTLE_WAIT_RESPONSE );
				tMessage.set_msgpara( (uint32)&tMsg );

				CSceneLogic::GetSingletonPtr( )->Send2Player( tpPlayer, &tMessage );
				
				LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Player( %s : %d ) Wait Battle %d Begin", __LK_FILE__, 
					__LINE__, __FUNCTION__, tpPlayer->GetName( ), tpPlayer->GetCharID( ),  tpMsg->battleid( ) );

				CMessageHaveSignUpBattle tSignUpMsg;

				tSignUpMsg.set_battleid( tpMsg->battleid() );
				tMessage.mutable_msghead()->set_messageid( ID_S2C_HAVE_SIGN_UP_BATTLE );
				tMessage.set_msgpara( (uint32)&tSignUpMsg );

				CSceneLogic::GetSingletonPtr( )->Send2Player( tpPlayer, &tMessage );

			}
			break;
		case ID_G2S_BATTLE_WAIT_LEAVE_RESPONSE:
			{
				CMessageBattleWaitLeaveResponse* tpMsg = ( CMessageBattleWaitLeaveResponse* )pMessage->msgpara( );
				REPE_ASSERT( tpMsg != NULL, break );

				CEntityPlayer* tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid( ) );
				REPE_ASSERT_STR( tpPlayer != NULL, break, "CharID %d", tpMsg->charid( ) );
				int tBattleID = tpMsg->battleid( );

				CMessageS2CBattleWaitLeaveResponse tMsg;

				CMessage tMessage;

				tMsg.set_battleid( tBattleID );

				tMessage.mutable_msghead()->set_messageid( ID_S2C_BATTLE_WAIT_LEAVE_RESPONSE );
				tMessage.set_msgpara( (uint32)&tMsg );

				 CSceneLogic::GetSingletonPtr( )->Send2Player( tpPlayer, &tMessage );

				LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Player( %s : %d ) Leave Wait Battle %d List",	__LK_FILE__,
					__LINE__, __FUNCTION__, tpPlayer->GetName( ), tpPlayer->GetCharID( ),  tpMsg->battleid( ) );
				
			}
			break;
		case ID_G2S_BATTLE_CREATE_RESPONSE:
			{
				CMessageBattleCreateResponse* tpMsg = ( CMessageBattleCreateResponse* )pMessage->msgpara( );
				REPE_ASSERT( tpMsg != NULL, break );
				printf( "CreateBattle Success %d : %lld\n", tpMsg->battleid( ), tpMsg->key( ) );	
			}
			break;
		case ID_G2S_INTO_REPETION_NOTIFY:
			{
				CMessageIntoRepetionNotify* tpMsg = ( CMessageIntoRepetionNotify* )pMessage->msgpara( );

				REPE_ASSERT( tpMsg != NULL, break );

				CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByCharID( tpMsg->charid( ) );
				REPE_ASSERT_STR( tpPlayer != NULL, break, "CharID %d", tpMsg->charid( ) );
				PlayerIntoRepetion( tpPlayer, tpMsg->mapid( ), tpMsg->posx( ), tpMsg->posy( ), tpMsg->key() );
			}
			break;
		case ID_G2S_REPETION_EVENT_CODE:
			{
				CMessageG2SRepetionEventCode* tpMsg = ( CMessageG2SRepetionEventCode* )pMessage->msgpara( );
				CRepetionInfo* tpInfo = GetRepetionInfo( tpMsg->repetionid( ) );
				CEntityPlayer* tpPlayer = ( CEntityPlayer* )CSceneLogic::GetSingletonPtr( )->GetPlayerByCharID( 
					tpMsg->roleid( ) );
				REPE_ASSERT_STR( tpPlayer != NULL, break, " CharID %d", tpMsg->roleid( ) );		
				REPE_ASSERT_STR( tpInfo != NULL, break, " RepetionIndex %d", tpMsg->repetionid( ) );
				SendErrCode( tpPlayer, tpMsg->code( ), tpInfo->GetBeginMapID( ) );			
			}
			break;
		case ID_S2C_REPETIONINDEX_NOTICE:
			{
				CMessageRepetionIndexNotice *tpMsg = ( CMessageRepetionIndexNotice * )pMessage->msgpara( );
				LK_ASSERT( tpMsg != NULL, return )
				CEntityPlayer* tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->roleid() );
				if ( tpPlayer != NULL )
				{
					tpMsg->set_roleid( 0 );
					CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
				}				
				break;
			}
		default:
			break;
	}
}

// 客户端发送消息
void CRepetionModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	REPE_ASSERT( pMessage != NULL, return );
	REPE_ASSERT( pMessage->has_msghead( ), return );
	const CMessageHead& tHead = pMessage->msghead( );

	switch( tHead.messageid( ) )
	{
		case ID_C2S_BATTLE_JOIN:
			{
				CMessageC2SBattleJoin* tpMsg = ( CMessageC2SBattleJoin* )pMessage->msgpara( );
//				RedirectRepetionMap( pPlayer, tpMsg->battleid( ), 0, 0, NULL ); 
				int tRtn = PlayerCanIntoRepetion( pPlayer, tpMsg->battleid( ) );

				if( tRtn == SUCCESS )
				{
					CRepetionInfo* tpInfo = GetRepetionInfo( tpMsg->battleid( ) );
					REPE_ASSERT_STR( tpInfo != NULL, break, "Cannt Get RepetionInfo %d", tpMsg->battleid( ) );
					PlayerIntoRepetion( pPlayer, tpInfo->GetBeginMapID( ), 0, 0 );
				}
			}
			break;
        case ID_C2S_BATTLE_JOIN_RANDOM_PLACE:
             {
                 CMessageC2SBattleJoinRandomPlace *tpMsg = (CMessageC2SBattleJoinRandomPlace *)pMessage->msgpara();  

                 CRepetionInfo* tpInfo = GetRepetionInfo( tpMsg->battleid( ) );
                 REPE_ASSERT_STR( tpInfo != NULL, break, "Cannt Get RepetionInfo %d", tpMsg->battleid( ) );


                 PlayerIntoRepetionRandomPlace(pPlayer,tpInfo );

                 }
             break;
		case ID_C2S_BATTLE_TEAM_WAIT_REQUEST:
			{
				CMessageBattleTeamWaitRequest* tpMsg = ( CMessageBattleTeamWaitRequest* )pMessage->msgpara( );
				TeamWaitBattle( pPlayer, tpMsg->battleid( ), NULL );	
			}
			break;
		case ID_C2S_BATTLE_WAIT_LEAVE_REQUEST:
			{
				CMessageC2SBattleWaitLeaveRequest* tpMsg = ( CMessageC2SBattleWaitLeaveRequest* )pMessage->msgpara( );

				CMessageBattleWaitLeaveRequest tMsg;
				CMessage tMessage;

				tMsg.set_charid( pPlayer->GetCharID( ) );
				tMsg.set_battleid( tpMsg->battleid( ) );

				tMessage.mutable_msghead()->set_messageid( ID_S2G_BATTLE_WAIT_LEAVE_REQUEST );
				tMessage.set_msgpara( (uint32)&tMsg );

				CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMessage );	
			}
			break;
         case ID_C2S_BATTLE_TEAM_FIX_WAIT_REQUEST:
            {
				CMessageBattleTeamFixWaitRequest* tpMsg = ( CMessageBattleTeamFixWaitRequest* )pMessage->msgpara( );
				TeamFixWaitBattle( pPlayer, tpMsg->battleid( ), NULL );	
            }
            break;
          case ID_C2S_BATTLE_SINGLE_FIX_WAIT_REQUEST:
            {
                CMessageBattleSingleFixWaitRequest *tpMsg = ( CMessageBattleSingleFixWaitRequest* )pMessage->msgpara( );  
				SingleFixWaitBattle( pPlayer, tpMsg->battleid( ), NULL );	
			}
			break;
		case ID_C2S_REPETION_LEAVE:
			{
#ifdef _FB_
				PlayerLeaveRepetion( pPlayer );
//				CPropertyPlayer* tpProperty = ( CPropertyPlayer* )pPlayer->GetProperty( );
//				if( tpProperty == NULL )
//				{
//					return;
//				}

//				if( pPlayer->IsAlive( ) == false )
//				{
//					CPropertyModule::GetSingletonPtr( )->PlayerRelive( pPlayer, RELIVE_WEAKGRAVE, 100, 100 );
//				}
//
//				CMapModule::GetSingletonPtr( )->Teleport(	tpProperty->GetRepetion( )->GetBackLineID( ), 
//						tpProperty->GetRepetion( )->GetBackMapID( ),
//						tpProperty->GetRepetion( )->GetPosX( ),
//						tpProperty->GetRepetion( )->GetPosY( ),
//						pPlayer,
//						pPlayer->GetMapIndex( )
//						);
#endif
			}	
			break;
		case ID_C2S_USEITEM_INTO_RESPINSE:
			{

			}
			break;
		case ID_C2S_REPETION_FUNC:
			{
				OnMessageNpcFunc( pPlayer, pMessage );
			}
			break;
		case ID_C2S_JOIN_ON_TIME_OPEN_BATTLE_REQUEST:
			{
				CMessageC2SJoinOnTimeOpenBattleRequest *tpMsg = ( CMessageC2SJoinOnTimeOpenBattleRequest * )pMessage->msgpara( );
				CRepetionInfo* tpInfo = GetRepetionInfo( tpMsg->battleid( ) );
				REPE_ASSERT_STR( tpInfo != NULL, break, "Cannt Get RepetionInfo %d", tpMsg->battleid( ) );
				int tRtn = PlayerCanIntoRepetion( pPlayer, tpMsg->battleid( ) );
				if( tRtn == SUCCESS )
				{
					CMessageS2GJoinOnTimeOpenBattleRequest tMsg;
					CMessage tMessage;
					tMsg.set_charid( pPlayer->GetCharID( ) );
					tMsg.set_battleid( tpMsg->battleid( ) );
					tMsg.set_mapid( tpMsg->mapid( ) );

					lua_tinker::table ret = lua_tinker::call<lua_tinker::table , int > (mLua, "get_random_place",tpInfo->GetRepetionIndex() );

					int nDstPostX = ret.get<int>("x");
					int nDstPostY = ret.get<int>("y");
					tMsg.set_posx( nDstPostX );
					tMsg.set_posy( nDstPostY );
					tMsg.set_level( pPlayer->GetLevel( ) );

					tMessage.mutable_msghead()->set_messageid( ID_S2G_JOIN_ONTIME_OPEN_BATTLE_REQUEST );
					tMessage.set_msgpara( (uint32)&tMsg );

					CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMessage );
				}
			
			}
			break;
		case ID_C2S_GET_RANDOM_TELE_CHARGE_REQUEST:
			{

				//OnMessageRandomTele( pPlayer, pMessage );
				OnMessageGetRandomTeleCharge( pPlayer, pMessage );

			}
			break;
		case ID_C2S_RANDOM_TELE_REQUEST:
			{
				OnMessageRandomTele( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_APPLYGLOBALWAR:
			{
				OnMessagePlayerApplyWar( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_ENTERGLOBALWAR:
			{
				OnMessagePlayerEnterGlobalWar( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_LEAVEGLOBALWAR:
			{
				OnMessagePlayerLeaveGlobalWarRequest( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_KICKCAMPPLAYER:
			{
				OnMessageKickCampPlayerRequest( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_VOTETOKICKPLAYER:
			{
				OnMessageVoteToKickPlayer( pPlayer, pMessage );
				break;
			}
		case ID_C2S_INTO_CORPSBOSSREPETION_REQUEST:
			{
				OnMessageIntoCorpsBossRepetion( pPlayer, pMessage);
				break;
			}		
		case ID_C2S_REQUEST_ENTERLASTREPETION:
			{				
				CMessageEnterLastReptionRequest *tpMsg = ( CMessageEnterLastReptionRequest * )pMessage->msgpara();
				if ( tpMsg != NULL )
				{
					tpMsg->set_roleid( pPlayer->GetCharID() );
					CSceneLogic::GetSingleton().Send2Gate( pMessage );
				}
				break;
			}
		default:
			break;
	}
}

// 建立实体
void CRepetionModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
#ifdef _FB_
	REPE_ASSERT( pEntity != NULL, return );

	CEntityOgre* 	tpOgreEntity = NULL;
	unsigned int	tTempID = 0;

	if( IS_REPETION_LINE_ID( pEntity->GetLineID( ) ) != true )
	{
		return;
	}

	if( pEntity->IsOgre()
		|| ( pEntity->IsBox() )
		|| ( pEntity->IsBlock() )
		|| ( pEntity->IsNpc() )
	  )
	{
		REPE_ASSERT( pCreator != NULL, return );

		CMapObject* tpMapObj = ( CMapObject* )CMapObjManager::GetSingletonPtr( )->GetMapObject( pEntity->GetLineID( ), 
				pEntity->GetMapID( ), pEntity->GetMapIndex( ) );

		REPE_ASSERT_STR( tpMapObj != NULL, return, "GetMapObject( %d, %d, %d )  Return Value Is NULL, EntityID %d", 
				pEntity->GetLineID( ), pEntity->GetMapID( ), pEntity->GetMapIndex( ), pEntity->GetEntityID( ) );

		tpOgreEntity = ( CEntityOgre* )pEntity;
		tTempID = pEntity->GetProperty( )->GetPropertyTempID( );
		unsigned int vParam = GetParameFromKey( tpMapObj->GetKey( ) );
		
		( ( CEntityNpc* )pEntity )->ChangeNpcByLevel( vParam );
	
		CRepetion* tpRepetion = tpMapObj->GetRepetion( );
		REPE_ASSERT( tpRepetion != NULL, return );	

		CRepetionInfo* tpRepetionInfo = tpRepetion->GetRepetionInfo( );
		REPE_ASSERT( tpRepetionInfo != NULL, return );

	
		lua_tinker::table vParame( mLua );
		vParame.set( "entityid", pEntity->GetEntityID( ) );
		vParame.set( "tempid", pEntity->GetNpcID( ) );
		vParame.set( "index", pCreator->GetIndex( ) );
		vParame.set( "real_map", tpMapObj->GetMapID( ) );
		vParame.set( "virtual_map", tpMapObj->GetVirtualMapID( ) );
		PERF_FUNC( "Repetion_DoEvent", tpRepetion->DoEvent( tpRepetionInfo, REPETION_CREATE_OGRE_EVENT, &vParame ) );
	}

	else if( pEntity->IsPlayer()  == true )
	{
		CEntityPlayer* tpPlayer = ( CEntityPlayer* )pEntity;
		CPlayerRepetion* tpPlayerRepetion = GetPlayerRepetion( tpPlayer );
		REPE_ASSERT( tpPlayerRepetion != NULL, return );

		// 先清掉副本key（进入副本之前赋值）
		tpPlayerRepetion->SetRepetionKey( 0 );

		// 如果骑马的话下马
		CPropertyPlayer *tpProperty = ( CPropertyPlayer* )tpPlayer->GetProperty();		
		tpProperty->ChangeHorseStateTransMap( tpPlayer->GetMapID() );

		// 跨服副本的战场阵营信息存储在CPlayerRepetion
		if ( tpPlayer->GetCampID() == 0 && IS_DUNGEONSERVER_LINE(  CSceneLogic::GetSingleton().GetLineID() ) )
		{
			tpPlayer->SetCampID( tpPlayerRepetion->GetCampID() );			
		}
		CMapObject* tpMapObj = ( CMapObject* )CMapObjManager::GetSingletonPtr( )->GetMapObject( pEntity->GetLineID( ), 
				pEntity->GetMapID( ), pEntity->GetMapIndex( ) );
		REPE_ASSERT_STR( tpMapObj != NULL, return, "GetMapObject( %d, %d, %d )  Return Value Is NULL", 
				pEntity->GetLineID( ), pEntity->GetMapID( ), pEntity->GetMapIndex( ) );


		CRepetion* tpRepetion = tpMapObj->GetRepetion( );
		REPE_ASSERT( tpRepetion != NULL, return );	

		CRepetionInfo* tpRepetionInfo = GetRepetionInfoByMapID( tpMapObj->GetMapID( ) );
		REPE_ASSERT( tpRepetionInfo != NULL, return );

		LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ]Player( %d : %s ) Into Repetion( %d ), Map( %d : %d :  %d :%lld )"
			" All Count %d : %d",
			__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->GetCharID( ), tpPlayer->GetCharNameShow( ), 
			tpRepetion->get_id( ), tpMapObj->GetLineID( ), tpMapObj->GetMapID( ), tpMapObj->GetMapIndex( ), 
			tpMapObj->GetKey( ), tpMapObj->GetPlayerCount( ), tpMapObj->GetPlayerWaitInCount( )  );


	}

#else
	if( pEntity->IsPlayer( ) == true )
	{
		NotifyRepetionIndex( ( CEntityPlayer* )pEntity, 0 );

		CEntityPlayer* tpPlayer = ( CEntityPlayer* )pEntity;
		CPlayerRepetion* tpPlayerRepetion = GetPlayerRepetion( tpPlayer );
		REPE_ASSERT( tpPlayerRepetion != NULL, return );


		for( int i = 1 ;i < _REPE_MAX_COUNT_ ;++i)
		{
			CRepetionInfo *tpRepeInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( i );
			//发送副本是否报名的信息
			int tSignUpTime = tpPlayerRepetion->GetSignUpTime( i );
			if( tpRepeInfo->IsInNowSignUpTime( tSignUpTime ) == true)
			{
				CMessageHaveSignUpBattle tSignUpMsg;
				CMessage tMessage;

				tSignUpMsg.set_battleid( i );
				tMessage.mutable_msghead()->set_messageid( ID_S2C_HAVE_SIGN_UP_BATTLE );
				tMessage.set_msgpara( (uint32)&tSignUpMsg );

				CSceneLogic::GetSingletonPtr( )->Send2Player( tpPlayer, &tMessage );

			}
			//判断此时间段是否 已经进入过此副本
			int tIntoRepetionTime = tpPlayerRepetion->GetIntoTime( i );
			//这个时间段还没进入过副本
			if( tpRepeInfo->IsInOpenTime( ) && (tpRepeInfo->IsInNowOpenTime( tIntoRepetionTime ) == false) && vFirst == true)
			{
				CMessageBattleOnTimeOpenNotice tOpenNotice;
				CMessage tMessage;
				tOpenNotice.set_battleid( i );
				tOpenNotice.set_mapid( tpRepeInfo->GetBeginMapID( ) );
				tMessage.mutable_msghead()->set_messageid( ID_S2C_BATTLE_ON_TIME_OPEN );
				tMessage.set_msgpara( ( uint32 )&tOpenNotice );

				CSceneLogic::GetSingletonPtr( )->Send2Player( tpPlayer, &tMessage );

			}
		}

	}
#endif //_FB_


	return;
}

// 销毁实体
void CRepetionModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{
#ifdef _FB_
	LK_ASSERT( pEntity != NULL, return );
	if( pEntity->IsPlayer( ) == true )
	{
		CEntityPlayer* tpPlayer = ( CEntityPlayer* )pEntity;

		CRepetion* tpRepetion = GetRepetion( tpPlayer );

		REPE_ASSERT_STR( tpRepetion != NULL, return, "Cannt Find Player( %d : %d : %s ) Repetion, Map( %d : %d : %d  )",
			tpPlayer->GetEntityID( ), tpPlayer->GetCharID( ), tpPlayer->GetCharNameShow( ), tpPlayer->GetLineID( ),
			tpPlayer->GetMapID( ), tpPlayer->GetMapIndex( ) );		
		
		if( tpRepetion->IsOverMinTime( ) == false && tpRepetion->IsSysClose( ) == false )
		{
			return;
		}

		if( tpRepetion->GetPlayerCount( ) == 0 )
		{
			// 副本中无人, 关闭副本
	//		DestroyRepetion( tpRepetion );			

			// 如果可重入，暂时先不关闭
			if ( tpRepetion->CanRelogin() == true )
			{
				tpRepetion->SetEmptyTime( time(NULL) );
			}			
			else
			{
				tpRepetion->SetClear( );
			}				
		}	
	}
#endif
}

bool CRepetionModule::OnSaveData( CEntityPlayer *pPlayer, int vSaveCode )
{
	REPE_ASSERT( pPlayer != NULL, return false);

	char tEscapStr[ PLAYER_DATALEN ] = { 0 };
	char tBuffer[ PLAYER_DATALEN ] = { 0 };
	int  tBufferLen = sizeof( tBuffer );
	char* tPointer = tEscapStr;

	CPlayerRepetion* tpRepetion = ( ( CPropertyPlayer* )pPlayer->GetProperty( ) )->GetRepetion( );
	CMapObject* tpMapObj = CMapObjManager::GetSingletonPtr( )->GetMapObject( pPlayer->GetLineID( ), 
		pPlayer->GetMapID( ), pPlayer->GetMapIndex( ) );
	
	if( tpMapObj == NULL )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] Cannt Get MapObject( %d, %d )",	__LK_FILE__, __LINE__,
			__FUNCTION__, pPlayer->GetMapID( ), pPlayer->GetMapIndex( ) );
	}

	int tIsCallBack = SQL_NEED_CALLBACK;
	if ( vSaveCode == SAVE_TIMETRIGER )
	{
		tIsCallBack = SQL_NONE;
	}
	else 
	{
		if( vSaveCode == SAVE_REDIRECT )
		{
			tpRepetion->SetCampID( 0 );
		}
		tpRepetion->SetUnRepetion( );	
	}

	if ( vSaveCode == SAVE_LEAVEGAME || vSaveCode == SAVE_RETURN2GATE || vSaveCode == SAVE_KICKOUT )
	{
#ifdef _FB_		
		if ( IS_DUNGEONSERVER_LINE( CSceneLogic::GetSingleton().GetLineID() ) )
		{				
			if( CGlobalModule::GetSingleton().GetGlobalteleType( pPlayer->GetCharID() )!= EGLOBALTELE_TYPE_RETURNTOGAME )
			{
				tpRepetion->SetGlobalSceneStatus( EGLOBALWAR_STATUS_INWAR );
				tpRepetion->SetGlobalWarOffLineTime( time(NULL) );
				tpRepetion->SetCampID( pPlayer->GetCampID() );
				CGlobalModule::GetSingleton().PlayerLeaveGlobalWar( pPlayer->GetCharID(), EREASON_LEAVEWAR_OFFLINE );
			}					
			else if ( vSaveCode == SAVE_KICKOUT )
			{
				CGlobalModule::GetSingleton().PlayerLeaveGlobalWar( pPlayer->GetCharID(), EREASON_LEAVEWAR_RETURNBACK );
			}
			
		}
#endif
	}	

	PBRepetion tPBRepetion;

	tpRepetion->Repetion2PB( &tPBRepetion, ( tpMapObj == NULL ) ? INVITED_KEY_VALUE : tpMapObj->GetKey( ) );

	tPBRepetion.SerializePartialToArray( tBuffer, tBufferLen );

	int tRealLen = EscapeDataString( &tPointer, sizeof( tEscapStr ) - 1, tBuffer, tPBRepetion.ByteSize( ) );
	REPE_ASSERT( tRealLen != 1 && "while escape_data_string, dst buff overflow", return false);



	CMessagePlayerProperty* tpSave = CSceneLogic::GetSingletonPtr( )->GetSavePlayerProperty( );
	tpSave->mutable_role_repetion( )->CopyFrom( tPBRepetion );
	return true;
}

void CRepetionModule::OnLoadConfig( )
{
	REPE_LOG* tpLog = CSceneCfgManager::GetSingletonPtr()->GetRepe_log();
	if( tpLog != NULL )
	{
		INIT_ROLLINGFILE_LOG( REPETION_LOG, "../log/repetion.log", (LogLevel)tpLog->mPriority );
	}

	LOG_INFO( REPETION_LOG, "Begin Load RepetionMap Info" );

	unsigned int i = 0;
	for( i = 0; i < ARRAY_CNT( mRepetionInfo ); i++ )
	{
		mRepetionInfo[ i ].Clear( );
	}

	//RegisterLua( "../config/data/repetion_data" );
	RegisterLua( ( const char* )CSceneLogic::GetSingleton().GetConfigDataFileNamePath( 
		CSceneLogic::GetSingletonPtr()->GetGameID( ), ( char* )"repetion_data" ) );

	// 注册Lua文件的加载回调函数
	const char* pcDirPath = CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID( ), "repetion_data");
	CSceneServer::GetSingleton().RegisterLoadConfigCallback(pcDirPath, ".lua", CRepetionModule::LoadLuaFileCallback);

	pcDirPath = CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID( ), "repetion_data/class");
	CSceneServer::GetSingleton().RegisterLoadConfigCallback(pcDirPath, ".lua", CRepetionModule::LoadLuaFileCallback);

	pcDirPath = CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID( ), "repetion_data/repetions");
	CSceneServer::GetSingleton().RegisterLoadConfigCallback(pcDirPath, ".lua", CRepetionModule::LoadLuaFileCallback);
	LOG_INFO( REPETION_LOG, "End Load RepetionMap Info" );
}

CRepetionInfo* CRepetionModule::GetRepetionInfoByMapID( int vMapID, int vIndex )
{
	CRepetionInfo* tpRepetionInfo = NULL;

//	if( vMapID < sizeof( mRepetionInfo ) / sizeof( mRepetionInfo[ 0 ] ) )
//	{
//		tpRepetionInfo = &mRepetionInfo[ vMapID ];
//	}

	unsigned int i = 0;
	for( i = ( unsigned int )vIndex; i < ARRAY_CNT( mRepetionInfo ); i++ )
	{
		if( mRepetionInfo[ i ].GetRepetionIndex( ) != INVALID_REPETION_INDEX )
		{
			if( mRepetionInfo[ i ].MapIsInRepetion( vMapID ) == true )
			{
				tpRepetionInfo = &mRepetionInfo[ i ];
				break;
			}	
		}
	}

	return tpRepetionInfo;
}

//Player Into Repetion Condition( EveryDay Time Add Task ) Check
int CRepetionModule::PlayerIntoRepetionCheck( CEntityPlayer* vpPlayer, CRepetionInfo* vpInfo, int vCheckFlag )
{
	unsigned int tCount = 0;
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	REPE_ASSERT( vpInfo != NULL, return ERR_INVALID_PARAMETER );

	CPropertyPlayer* tpPro = ( CPropertyPlayer* )vpPlayer->GetProperty( ); 

	if( vpPlayer->GetAliveStatus( ) == ALIVE_DIE )
	{
		return ERR_REPETION_DIE;
	}

	if( tpPro->GetLevel( ) < vpInfo->GetMinLevel( ) )
	{
		LOG_WARN( REPETION_LOG, "[ %s : %d ][ %s ]Player( %d : %d : %s ) Want Redirect Repetion( %d ), But" 
			" Level( %d ) Less Repetion Need( %d )", __LK_FILE__, __LINE__, __FUNCTION__, vpPlayer->GetEntityID( ),
			vpPlayer->GetCharID( ), vpPlayer->GetCharNameShow( ), vpInfo->GetRepetionIndex( ), tpPro->GetLevel( ), 
			vpInfo->GetMinLevel( ) );
		return ERR_REPETION_LEVEL_MIN;
	}

	if( tpPro->GetLevel( ) > vpInfo->GetMaxLevel( ) && vpInfo->GetMaxLevel( ) != 0  )
	{
		LOG_WARN( REPETION_LOG, "[ %s : %d ][ %s ]Player( %d : %d : %s ) Want Redirect Repetion( %d ), But" 
			" Level( %d ) More than Repetion Need( %d )", __LK_FILE__, __LINE__, __FUNCTION__, vpPlayer->GetEntityID( ),
			vpPlayer->GetCharID( ), vpPlayer->GetCharNameShow( ), vpInfo->GetRepetionIndex( ), tpPro->GetLevel( ), 
			vpInfo->GetMaxLevel( ) );
		return ERR_REPETION_LEVEL_MAX;
	}

	if( IS_EFFECTIVE_VALUE( vpInfo->GetTaskID( ) ) == true )
	{
		if( CTaskModule::GetSingletonPtr( )->IsTaskOnPlayer( vpPlayer, vpInfo->GetTaskID( ) ) == false )
		{
			LOG_WARN( REPETION_LOG, "[ %s : %d ][ %s ]Player( %d : %d : %s ) Want Redirect Repetion( %d ), But Not "
				"Have Task( %d )", __LK_FILE__, __LINE__, __FUNCTION__, vpPlayer->GetEntityID( ),
				vpPlayer->GetCharID( ), vpPlayer->GetCharNameShow( ), vpInfo->GetRepetionIndex( ), 
				vpInfo->GetTaskID( ) );
			return ERR_REPETION_NEED_TASK;
		}
	}

	if( IS_EFFECTIVE_VALUE( vpInfo->GetPlayerPerNumber( ) ) == true )
	{
		if( CSceneCfgManager::GetSingletonPtr( )->GetRepetion_cfg( )->mNumcheck != 0 )
		{
			if( CHECK_FLAG( vCheckFlag, CHECK_NUMBER_KEY ) == true )
			{
				tCount = vpInfo->GetPlayerAllTakeCount( tpPro );
				// 副本记次修改，2011.0913 by lipeng
				if( (tCount + vpInfo->GetConsumeTimes()) > vpInfo->GetPlayerPerNumber( ) ) 
				{
					LOG_WARN( REPETION_LOG, "[ %s : %d ][ %s ]Player( %d : %d : %s ) Want Redirect RepetionMap( %d ), "
							"But EveryDayTime( %d : %d ) Is Full",  __LK_FILE__, __LINE__, __FUNCTION__, 
							vpPlayer->GetEntityID( ), vpPlayer->GetCharID( ), vpPlayer->GetCharNameShow( ), 
							vpInfo->GetRepetionIndex( ), vpInfo->GetPlayerPerNumber( ), tCount );
					return ERR_REPETION_EVERYDAY_TIME_FULL;	
				}
			}
		}

	}

	if( IS_EFFECTIVE_VALUE( vpInfo->GetPlayerWeekPerNumber( ) ) == true )
	{
		if( CSceneCfgManager::GetSingletonPtr( )->GetRepetion_cfg( )->mNumcheck != 0 )
		{
			unsigned int tWeekCount = 0;
			vpInfo->GetWeekPlayerAllTakeCount( tpPro, tWeekCount );
			if( (tWeekCount + vpInfo->GetConsumeTimes()) > vpInfo->GetPlayerWeekPerNumber( ) )
			{
				LOG_WARN( REPETION_LOG, "[ %s : %d ][ %s ]Player( %d : %d : %s ) Want Redirect RepetionMap( %d ), But" 
					" WeekTime( %d : %d ) Is Full",  __LK_FILE__, __LINE__, __FUNCTION__, vpPlayer->GetEntityID( ),
					vpPlayer->GetCharID( ), vpPlayer->GetCharNameShow( ), vpInfo->GetRepetionIndex( ), 
					vpInfo->GetPlayerPerNumber( ), tWeekCount );
				return ERR_REPETION_WEEK_TIME_FULL;		
			}
		}
	}
	
	return SUCCESS;
}

int CRepetionInfo::GetPlayerAllTakeCount( CPropertyPlayer* vpPro )
{
	int tAllCount = 0;
	unsigned int tCount = 0;

	vpPro->GetRepetion( )->GetRepetionCount( GetRepetionIndex( ), &tCount );
	tAllCount += tCount;

	for( unsigned int i = 0; i < ARRAY_CNT( mCommTakeMap ); i++ )
	{
		if( mCommTakeMap[ i ] == INVALID_MAP_ID )
		{
			break;
		}

		tCount = 0;

		vpPro->GetRepetion( )->GetRepetionCount( mCommTakeMap[ i ], &tCount );
		tAllCount += tCount;
	}

	return tAllCount;
}


int CRepetionInfo::GetWeekPlayerAllTakeCount( CPropertyPlayer* vpPro, unsigned int &WeekCount )
{
	unsigned int tCount = 0;
	WeekCount = 0;

	vpPro->GetRepetion( )->GetRepetionWeekCount( GetRepetionIndex( ), &tCount );
	WeekCount += tCount;

	for( unsigned int i = 0; i < ARRAY_CNT( mCommTakeMap ); i++ )
	{
		if( mCommTakeMap[ i ] == INVALID_MAP_ID )
		{
			break;
		}

		tCount = 0;

		vpPro->GetRepetion( )->GetRepetionWeekCount( mCommTakeMap[ i ], &tCount );
		WeekCount += tCount;
	}

	return SUCCESS;
}

bool CRepetionModule::RedirectRepetionCheck( 
	CEntityPlayer* vpPlayer,
	int* vpPlayerListID, 
	int vPlayerCount, 
	CRepetionInfo* vpInfo, 
	CEntityNpc* vpNpc,
	int vCheckFlag,
	const SpecialItem** vpSpecialItem ,
        int flag,
	int *vNeedItem,
	bool *vpCheckedNecessity
	)
{
	bool tCheck = true;
	int i = 0;
	int tPlayerCount = 0;
	int tRtn = SUCCESS;

	REPE_ASSERT( vpPlayerListID != NULL, return false );
	REPE_ASSERT( vpInfo != NULL, return false );

	for( i = 0; i < vPlayerCount; i++ )
	{
		if( vpPlayerListID[ i ] != 0 )
		{
			CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByCharID( vpPlayerListID[ i ] );
			if( tpPlayer == NULL )
			{
				// TeamMember Not All In Same Scene, Cannt Into Repetion, Return Err
				// 队伍中有玩家不在同一服务器内
				SendErrCode( vpPlayerListID, vPlayerCount, ERR_REPETION_TEAM_MEMBER_UNINSAMESCENE, 
					vpPlayerListID[ i ], vpInfo->GetBeginMapID( ), vpPlayerListID[ i ] );
				LOG_NOTICE( REPETION_LOG, "[ %s : %d ][ %s ] Cannt Get Player( %d )",
					__LK_FILE__, __LINE__, __FUNCTION__, vpPlayerListID[ i ] );
				tCheck = false;
				continue;
			}

			if( vpNpc != NULL )
			{
				if( tpPlayer->IsWithinDistance( vpNpc, 15 ) == false )
				{
					// 玩家离进入NPC远
					SendErrCode( vpPlayerListID, vPlayerCount, ERR_REPETION_TEAM_MEMBER_FAR, tpPlayer->GetCharID( ),
						vpInfo->GetBeginMapID( ), vpPlayerListID[ i ] );
					tCheck = false;
					continue;
				}
			}

			tRtn = PlayerIntoRepetionCheck( tpPlayer, vpInfo, vCheckFlag );
			if( tRtn != SUCCESS )
			{
				// 不满足进入副本条件
				if( tRtn == ERR_REPETION_EVERYDAY_TIME_FULL || tRtn == ERR_REPETION_WEEK_TIME_FULL )
				{	
					// 每日进入次数满
					SendErrCode( vpPlayerListID, vPlayerCount, ( ERepetion )tRtn, tpPlayer->GetCharID( ),
						vpInfo->GetBeginMapID( ), vpInfo->GetPlayerPerNumber( ), tpPlayer->GetCharID( ) );
				}
				else if( tRtn == ERR_REPETION_NEED_TASK )
				{
					// 无副本进入需要任务
					SendErrCode( vpPlayerListID, vPlayerCount, ( ERepetion )tRtn, tpPlayer->GetCharID( ), 
						vpInfo->GetBeginMapID( ), vpInfo->GetTaskID( ), tpPlayer->GetCharID( ) );
				}
				else if( tRtn == ERR_REPETION_LEVEL_MIN  )
				{
					// 不符合进入副本最低等级
					SendErrCode( vpPlayerListID, vPlayerCount, ( ERepetion )tRtn, tpPlayer->GetCharID( ),
						vpInfo->GetBeginMapID( ), tpPlayer->GetLevel( ), vpInfo->GetMinLevel( ) );
				}
				else if( tRtn == ERR_REPETION_LEVEL_MAX )
				{
					// 不符合进入副本最高等级
					SendErrCode( vpPlayerListID, vPlayerCount, ( ERepetion )tRtn, tpPlayer->GetCharID( ),
						vpInfo->GetBeginMapID( ), tpPlayer->GetLevel( ), vpInfo->GetMaxLevel( ) );
				}
				else if( tRtn == ERR_REPETION_DIE )
				{
					// 玩家处于死亡状态
					SendErrCode( vpPlayerListID, vPlayerCount, ( ERepetion )tRtn, tpPlayer->GetCharID( ),
						vpInfo->GetBeginMapID( ), tpPlayer->GetCharID( ) );
				}

				tCheck = false;
			}

			tPlayerCount++;	
		}
		else
		{
			// Player Not In Same Scene
			continue;
		}
	}
	if( CSceneCfgManager::GetSingletonPtr( )->GetRepetion_cfg( )->mPlayercheck != 0 )
	{
		if( CHECK_FLAG( vCheckFlag, CHECK_COUNT_KEY ) == true )
		{
			if( ( tCheck == true ) && ( vPlayerCount < vpInfo->GetPlayerCount( ) ) )
			{
				if( vPlayerCount == 1 )
				{
					CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByCharID( vpPlayerListID[ 0 ] );	
					if( vpInfo->IsSpecialCheck( tpPlayer, REPE_JOIN_COUNT ) == true )
					{
						if( vpSpecialItem != NULL )
						{
							*vpSpecialItem = GetSpecialUnCheck( vpPlayer, vpInfo, vCheckFlag, CHECK_COUNT_KEY );
							if( *vpSpecialItem != NULL )
							{
								return false;
							}
						}

						SendErrCode( vpPlayerListID, vPlayerCount, ERR_REPETION_PLAYERNUMBER_LESS, 0, 
								vpInfo->GetBeginMapID( ), vpInfo->GetPlayerCount( ), tPlayerCount );
						tCheck = false;

					}
				}
				else
				{
					if( vpSpecialItem != NULL )
					{
						*vpSpecialItem = GetSpecialUnCheck( vpPlayer, vpInfo, vCheckFlag, CHECK_COUNT_KEY );
						if( *vpSpecialItem != NULL )
						{
							return false;
						}
					}

					SendErrCode( vpPlayerListID, vPlayerCount, ERR_REPETION_PLAYERNUMBER_LESS, 0, 
						vpInfo->GetBeginMapID( ), vpInfo->GetPlayerCount( ), tPlayerCount );
					tCheck = false;
				}
			}
		}
	}

	if( IS_EFFECTIVE_VALUE( vpInfo->GetPlayerCountMax( ) ) == true )
	{
		if( vPlayerCount > vpInfo->GetPlayerCountMax( ) )
		{
			SendErrCode( vpPlayerListID, vPlayerCount, ERR_REPETION_PLAYERNUMBER_MORE, 0,
					vpInfo->GetBeginMapID( ), vpInfo->GetPlayerCountMax( ), tPlayerCount );
			tCheck = false;
		}
	}

    if(flag == INTO_REPECTION_CHECK )
	{
		if( IntoRepetionTimeCheck( vpInfo ) == false && CHECK_FLAG( vCheckFlag, CHECK_TIME_KEY ) == true )
		{
		// 不在副本活动的时间内
//		SendTeamErrCode( vpPlayer, ERR_REPETION_HOUR, vMapID );
			if( vpSpecialItem != NULL )
			{
					*vpSpecialItem = GetSpecialUnCheck( vpPlayer, vpInfo, vCheckFlag, CHECK_TIME_KEY );
				if( *vpSpecialItem != NULL )
				{
						return false;
				}
			}

				SendErrCode( vpPlayerListID, vPlayerCount, ERR_REPETION_HOUR, 0, vpInfo->GetBeginMapID( ) );
				tCheck = false;
		}
	}else if( flag == SIGNUP_REPECTION_CHECK  ){
                //检查是否在报名时间内 add by yucx 2010年 08月 30日 星期一 21:04:41
         if( SignUpRepetionTimeCheck( vpInfo ) == false )
         {
              SendErrCode( vpPlayerListID, vPlayerCount, ERR_REPETION_HOUR, 0, vpInfo->GetBeginMapID( ) );
              tCheck = false;
         }
    }

	if( NeedNecessityCheck( vpInfo ) == true && *vpCheckedNecessity == false )
	{	
		 *vpCheckedNecessity = true;
		int HasItem = 0;
		CPropertyModule::GetSingletonPtr()->HasItem( vpPlayer->GetEntityID(), vpInfo->GetNecessity(  ),1, HasItem, 0 );

		if( HasItem != 1 )
		{
			//return ERR_REPETION_NO_NECESSITY;
			// 玩家没有进入副本必须品
			SendErrCode( vpPlayerListID, vPlayerCount, ERR_REPETION_NO_NECESSITY, 0,vpInfo->GetBeginMapID( ) );
			return false;
			 
		}else{

			if( tCheck == true && vNeedItem != NULL && *vNeedItem == 0 )
			{
				*vNeedItem = vpInfo->GetNecessity();
				return false;
			}
			
		}
	}

	if( NeedSpeciaCheck( vpInfo) == true )
	{
		if( vpInfo->GetSpecialCheck() & REPETION_SHITUCHECK )
		{

			CEntityPlayer* tpHeader = CTeamModule::GetSingletonPtr( )->GetTeamHeader( vpPlayer );
			
			//检查师徒关系 起码人数是2
			if(tpHeader == NULL || vPlayerCount < 2)
			{
				SendErrCode( vpPlayerListID, vPlayerCount, ERR_REPETION_SHITU_SHIP, 0,vpInfo->GetBeginMapID( ) );
				tCheck = false;
			}else
			{
				
				CPropertyPlayer *tpHeaderProperty = (CPropertyPlayer*)tpHeader->GetProperty();
				

				for( i = 0; i < vPlayerCount; i++ )
				{
				
					if( vpPlayerListID[ i ] != 0 && tpHeader->GetCharID() != vpPlayerListID[ i ])
					{
						bool tpIsMaster = false;
						CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByCharID( vpPlayerListID[ i ] );
						if( tpPlayer == NULL)
						{
							continue;
						}
						
						
						CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
						
						if(tpProperty == NULL) 
						{
							continue;
						}
			
						for ( CPropertyPlayer::MatrixList::iterator it = tpHeaderProperty->mMatrixList.begin(); it != tpHeaderProperty->mMatrixList.end(); ++it )
						{
							if ( (unsigned int)(*it) == tpPlayer->GetCharID() )
							{
								tpIsMaster = true;
							}
						}	
			
						if( tpIsMaster == false)
						{
							SendErrCode( vpPlayerListID, vPlayerCount, ERR_REPETION_SHITU_SHIP, 0 ,vpInfo->GetBeginMapID( ) );
							return false;
						}
						
					}
				}

			}
		} //REPETION_SHITUCHECK
		
	}


	return tCheck;
}

int CRepetionModule::RedirectRepetionMap(	CEntityPlayer* vpPlayer, 
		unsigned short vMapID, 
		unsigned short vPosX,  
		unsigned short vPosY,
		CEntityNpc* vpNpc,
		int vCheckFlag,
		int vRemoveItem,
		bool vCheckedNecessity
		)
{
	int tPlayerCharID[ TEAM_NUMBER ] = { 0 };
	unsigned short tPosX = vPosX;
	unsigned short tPosY = vPosY;
	int tRtn = SUCCESS;
	int tPlayerCount = 0;
	bool tCheck = true;

	REPE_ASSERT( vpPlayer != NULL, return  ERR_INVALID_PARAMETER );

	CRepetionInfo* tpRepetionInfo = GetRepetionInfoByMapID( vMapID );
	REPE_ASSERT( tpRepetionInfo != NULL, return ERR_INVALID_PARAMETER );

	if( tpRepetionInfo->MapIsInRepetion( vMapID ) == false )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Map( %d ) Is Not A Repetion Map", __LK_FILE__, __LINE__,
				__FUNCTION__, vMapID );
		return ERR_UNKNOWN;
	}

	if( vPosX == 0 || vPosY == 0 )
	{
		tPosX = tpRepetionInfo->GetPosX( );
		tPosY = tpRepetionInfo->GetPosY( );
	}

	if( NotInWaitRedirMap( vpPlayer ) == false )
	{
		return ERR_UNKNOWN;
	}

//	if( CHECK_FLAG( vCheckFlag, CHECK_TIME_KEY ) == true )
//	{
//		if( SpecialTimeIntoRepe( vpPlayer, tpRepetionInfo, tPosX, tPosY, vpNpc ) == SUCCESS ) 
//		{
//			return SUCCESS;
//		}
//	}
	const SpecialItem* tpSpecialItem = NULL;
	int tpNeedItem = 0; 

	if( tpRepetionInfo->IsTeamIn( ) == true )
	{
		//当副本需求队伍同时进入的时候
		CEntityPlayer* tpHeader = CTeamModule::GetSingletonPtr( )->GetTeamHeader( vpPlayer );
		if( tpHeader != NULL && tpHeader != vpPlayer )
		{
			SendErrCode( vpPlayer, ERR_REPETION_ISNOT_HEADER, vMapID );
			return ERR_REPETION_ISNOT_HEADER;
		}
		else if( tpHeader == NULL )
		{
			SendErrCode( vpPlayer, ERR_REPETION_ISNOT_HEADER, vMapID );
			return ERR_REPETION_ISNOT_HEADER;
		}
		tRtn = CTeamModule::GetSingletonPtr( )->OnGetMemberList( vpPlayer->GetCharID( ), tPlayerCharID, 
			ARRAY_CNT( tPlayerCharID ), tPlayerCount );
		if ( tRtn < 0 )
		{
			LOG_ERROR("team", "repetion get team list failed.id=%d,[%s:%d]", vpPlayer->GetCharID(), __LK_FILE__, __LINE__);
			return ERR_UNKNOWN;
		}	

		tCheck = RedirectRepetionCheck( vpPlayer, tPlayerCharID, tPlayerCount, tpRepetionInfo, vpNpc, vCheckFlag, 
			&tpSpecialItem, 0, &tpNeedItem ,&vCheckedNecessity );

		if( tCheck == true )
		{
			// 扣除玩家身上的钥匙
			if ( vRemoveItem != 0 )
			{
				bool tBind = true;
				uint64_t ullGUID = 0;
				int tRtn = CPropertyModule::GetSingletonPtr( )->PlayerRemoveItemByID( vpPlayer, vRemoveItem, 1, tBind, &ullGUID );
				if( tRtn == SUCCESS )
				{
					LogEventLostItemByRepetion( vpPlayer, ullGUID, vRemoveItem, 1 );
				}		
			}
			CMessage tMsg;
			CMessageTeamIntoRepetion tInto;

			tInto.set_mapid( vMapID );
			tInto.set_charid( vpPlayer->GetCharID( ) );
			tInto.set_posx( tPosX );
			tInto.set_posy( tPosY );
		//	tInto.set_level( vpPlayer->GetTeamLevel( ) );

			tInto.set_level( ( tpRepetionInfo->GetTeamLevelFlag( ) == REPE_LEVEL_MAX ) ? 
				vpPlayer->GetTeamMaxLevel( ) : ( ( tpRepetionInfo->GetTeamLevelFlag( ) == REPE_LEVEL_MIN ) ? 
				vpPlayer->GetTeamMinLevel( ) : vpPlayer->GetTeamLevel( ) ) );

			tInto.set_removeitem( vRemoveItem );

			tMsg.mutable_msghead()->set_messageid( ID_S2G_INTO_REPETION_TEAM );
			tMsg.set_msgpara( (uint32)&tInto );

			CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMsg );
		}
		else
		{
			if( tpSpecialItem != NULL )
			{
				RedirectRepetionMap( vpPlayer, vMapID, vPosX, vPosY, vpNpc, 
					CHECK_REAL_GET( vCheckFlag, tpSpecialItem->mFlag ), tpSpecialItem->mItemID, vCheckedNecessity );	
			}

			if( tpNeedItem != 0)
			{
				RedirectRepetionMap( vpPlayer, vMapID, vPosX, vPosY, vpNpc, 
					vCheckFlag,tpNeedItem , vCheckedNecessity);
			}
		}
	}
	else
	{
		tPlayerCharID[ 0 ] = vpPlayer->GetCharID( );
		tPlayerCount = 1;

		tCheck = RedirectRepetionCheck( vpPlayer, tPlayerCharID, tPlayerCount, tpRepetionInfo, vpNpc, vCheckFlag, 
			&tpSpecialItem,0 ,&tpNeedItem, &vCheckedNecessity );
		if( tCheck == true )
		{
			// 扣除玩家身上的钥匙
			if ( vRemoveItem != 0 )
			{
				bool tBind = true;
				uint64_t ullGUID = 0;
				int tRtn = CPropertyModule::GetSingletonPtr( )->PlayerRemoveItemByID( vpPlayer, vRemoveItem, 1, tBind, &ullGUID );
				if( tRtn == SUCCESS )
				{
					LogEventLostItemByRepetion( vpPlayer, ullGUID, vRemoveItem, 1 );
				}		
			}						
			CMessage tMsg;
			CMessageSingleIntoRepetion tInto;

			tInto.set_mapid( vMapID );
			tInto.set_charid( vpPlayer->GetCharID( ) );
			tInto.set_posx( tPosX );
			tInto.set_posy( tPosY );
	//		tInto.set_level( vpPlayer->GetTeamLevel( ) );
			tInto.set_level( ( tpRepetionInfo->GetTeamLevelFlag( ) == REPE_LEVEL_MAX ) ? 
				vpPlayer->GetTeamMaxLevel( ) : ( ( tpRepetionInfo->GetTeamLevelFlag( ) == REPE_LEVEL_MIN ) ? 
				vpPlayer->GetTeamMinLevel( ) : vpPlayer->GetTeamLevel( ) ) );

			tInto.set_removeitem( vRemoveItem );

			tMsg.mutable_msghead()->set_messageid( ID_S2G_INTO_REPETION_SINGLE );
			tMsg.set_msgpara( (uint32)&tInto );

			CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMsg );
		}
		else
		{
			if( tpSpecialItem != NULL )
			{
				RedirectRepetionMap( vpPlayer, vMapID, vPosX, vPosY, vpNpc, 
					CHECK_REAL_GET( vCheckFlag, tpSpecialItem->mFlag ), tpSpecialItem->mItemID ,vCheckedNecessity);
			} 

			if( tpNeedItem != 0 )
			{
				RedirectRepetionMap( vpPlayer, vMapID, vPosX, vPosY, vpNpc, 
					 vCheckFlag, tpNeedItem ,vCheckedNecessity );
			}
		}
	}	

	return SUCCESS;
}

int CRepetionModule::PlayerIntoRepetion( CEntityPlayer* vpPlayer, int vMapID, int vPosX, int vPosY, KEY_TYPE vKey /* = 0 */, bool bIntoLastRepetion /* = false */ )
{
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	CPlayerRepetion* tpPlayerRepe = ( ( CPropertyPlayer* )vpPlayer->GetProperty( ) )->GetRepetion( );
	if( IS_REPETION_LINE_ID( vpPlayer->GetLineID( ) ) != true )
	{
		tpPlayerRepe->SetBackLineID( vpPlayer->GetLineID( ) );
		tpPlayerRepe->SetBackMapID( vpPlayer->GetMapID( ) );
		tpPlayerRepe->SetPosX( vpPlayer->GetPosX( ) );
		tpPlayerRepe->SetPosY( vpPlayer->GetPosY( ) );
	}

	// 进入一个新副本， 次数增加
	CRepetionInfo *tpRepetionInfo = GetRepetionInfoByMapID( vMapID );
	if ( tpRepetionInfo != NULL && bIntoLastRepetion == false )
	{
		tpPlayerRepe->IncRepetionCount( tpRepetionInfo->GetRepetionIndex( ) );		
		tpPlayerRepe->SetRepetionKey( vKey );
	}	
	CMapModule::GetSingletonPtr( )->Teleport( DEFAULT_REPETION_LINE_ID, vMapID, vPosX, vPosY, vpPlayer, INVALID_MAP_INDEX, vKey );
	vpPlayer->SetCommState( CEntityPlayer::STATE_INMAP );
	return SUCCESS;
}

bool CRepetionModule::NotInWaitRedirMap( CEntityPlayer* vpPlayer )
{
	return true;
}

int CRepetionModule::RegisterLua( const char* vpLuaDir )
{
	int tRtn = SUCCESS;
	char tLuaFile[ 256 ] = { 0 };

//	if( mLua == NULL )
//	{
	if( mLua != NULL )
	{
		lua_close( mLua );
		mLua = NULL;
	}

	mLua = lua_open( );
	if( mLua != NULL )
	{
		luaL_openlibs( mLua );

		lua_tinker::def( mLua, "Debug",                         &CRepetionModule::Debug );
		lua_tinker::def( mLua, "Error",							&CRepetionModule::Error ); 
		lua_tinker::def( mLua, "Infor",							&CRepetionModule::Infor );
		lua_tinker::def( mLua, "Notice",						&CRepetionModule::Notice );
		lua_tinker::def( mLua, "RegisterRepetion",				&CRepetionModule::RegisterRepetion );
		lua_tinker::def( mLua, "RegisteOpenTime",				&CRepetionModule::RegisteOpenTime );
		lua_tinker::def( mLua, "RegisteSignUpTime",				&CRepetionModule::RegisteSignUpTime );
		lua_tinker::def( mLua, "RegisteBattleOpenSpace",		&CRepetionModule::RegisteBattleOpenSpace );
		lua_tinker::def( mLua, "RegisteEvent",					&CRepetionModule::RegisteEvent );
		lua_tinker::def( mLua, "CreateNpc",						&CRepetionModule::CreateNpc );
		lua_tinker::def( mLua, "CreateNpcByBite",				&CRepetionModule::CreateNpcByBite );
		lua_tinker::def( mLua, "CreateNpcRealMap",				&CRepetionModule::CreateNpcRealMap );
		lua_tinker::def( mLua, "RegisteTimeEvent",				&CRepetionModule::RegisteTimeEvent );
		lua_tinker::def( mLua, "UseSkill",						&CRepetionModule::UseSkill );
		lua_tinker::def( mLua, "AddEnmity2Template",			&CRepetionModule::AddEnmity2Template );
		lua_tinker::def( mLua, "UseSkill2Template",				&CRepetionModule::UseSkill2Template );
		lua_tinker::def( mLua, "TimeBegin",						&CRepetionModule::TimeBegin );
		lua_tinker::def( mLua, "TimeRestart",					&CRepetionModule::TimeRestart );
		lua_tinker::def( mLua, "ChangeTimeSpace",				&CRepetionModule::ChangeTimeSpace );
		lua_tinker::def( mLua, "TimeEnd",						&CRepetionModule::TimeEnd );
		lua_tinker::def( mLua, "TimePause",						&CRepetionModule::TimePause );
		lua_tinker::def( mLua, "EventClose",					&CRepetionModule::EventClose );
		lua_tinker::def( mLua, "ObserverEntityDeath",			&CRepetionModule::ObserverEntityDeath );
		lua_tinker::def( mLua, "ObserverMachinery",				&CRepetionModule::ObserverMachinery );
		lua_tinker::def( mLua, "DestroyAliveTempOgre",			&CRepetionModule::DestroyAliveTempOgre );
		lua_tinker::def( mLua, "EventNoticeAll",				&CRepetionModule::EventNoticeAll );
		lua_tinker::def( mLua, "EventNoticeSingle",				&CRepetionModule::EventNoticeSingle );
		lua_tinker::def( mLua, "SetIntData",					&CRepetionModule::SetIntData );
		lua_tinker::def( mLua, "GetIntData",					&CRepetionModule::GetIntData );
		lua_tinker::def( mLua, "CharacterDie",					&CRepetionModule::CharacterDie );
		lua_tinker::def( mLua, "ChangePKType",					&CRepetionModule::ChangePKType );	
		lua_tinker::def( mLua, "AllPlayerAddEnmitiy",			&CRepetionModule::AllPlayerAddEnmitiy );
		lua_tinker::def( mLua, "ObserverAssignment", 			&CRepetionModule::ObserverAssignment );
		lua_tinker::def( mLua, "SetAssignment",					&CRepetionModule::SetAssignment );
		lua_tinker::def( mLua, "ClearAssignment",				&CRepetionModule::ClearAssignment );
		lua_tinker::def( mLua, "ClearEnmity", 					&CRepetionModule::ClearEnmity );
		lua_tinker::def( mLua, "AddEnmity",						&CRepetionModule::AddEnmity );
		lua_tinker::def( mLua, "ClearAssignmentAll",			&CRepetionModule::ClearAssignmentAll );
		lua_tinker::def( mLua, "SetAliveRefresh",				&CRepetionModule::SetAliveRefresh );
		lua_tinker::def( mLua, "CreateHierogram", 				&CRepetionModule::CreateHierogram );
		lua_tinker::def( mLua, "CreateHierogramByMap",			&CRepetionModule::CreateHierogramByMap );
		lua_tinker::def( mLua, "Teleport",						&CRepetionModule::Teleport );
		lua_tinker::def( mLua, "TeleportByMap",					&CRepetionModule::TeleportByMap );
		lua_tinker::def( mLua, "UseSkill2AllPlayer",			&CRepetionModule::UseSkill2AllPlayer );
		lua_tinker::def( mLua, "SetIntData2AllPlayer",			&CRepetionModule::SetIntData2AllPlayer );
		lua_tinker::def( mLua, "AddIntData2AllPlayer",			&CRepetionModule::AddIntData2AllPlayer );
		lua_tinker::def( mLua, "SetIntData2AllPlayerMax",		&CRepetionModule::SetIntData2AllPlayerMax );
		lua_tinker::def( mLua, "GetRepeLevel", 					&CRepetionModule::GetRepeLevel );
		lua_tinker::def( mLua, "GetBeginTime",					&CRepetionModule::GetBeginTime );
		lua_tinker::def( mLua, "RegisterBattleCamp",			&CRepetionModule::RegisterBattleCamp );
		lua_tinker::def( mLua, "RegisterBattleWaitLevel",		&CRepetionModule::RegisterBattleWaitLevel );
		lua_tinker::def( mLua, "RepetionClose",					&CRepetionModule::RepetionClose );
		lua_tinker::def( mLua, "NpcMove",						&CRepetionModule::NpcMove );
		lua_tinker::def( mLua, "NpcFollow",						&CRepetionModule::NpcFollow );
		lua_tinker::def( mLua, "RegisterCommTakeMap",			&CRepetionModule::RegisterCommTakeMap );
		lua_tinker::def( mLua, "GetCampCountEncircleEntity",	&CRepetionModule::GetCampCountEncircleEntity );
		lua_tinker::def( mLua, "GetPos",						&CRepetionModule::GetPos );
		lua_tinker::def( mLua, "NpcPatrol",						&CRepetionModule::NpcPatrol );
		lua_tinker::def( mLua, "SetCampID",						&CRepetionModule::SetCampID );
		lua_tinker::def( mLua, "ChangeCampIDByTempID",			&CRepetionModule::ChangeCampIDByTempID );
		lua_tinker::def( mLua, "PlayerRelive",					&CRepetionModule::PlayerRelive );
		lua_tinker::def( mLua, "PlayerReliveByMapID",			&CRepetionModule::PlayerReliveByMapID );
		lua_tinker::def( mLua, "PlayerReliveSys",				&CRepetionModule::PlayerReliveSys );
		lua_tinker::def( mLua, "GetTime",						&CRepetionModule::GetTime );
		lua_tinker::def( mLua, "ChangeTime",					&CRepetionModule::ChangeTime );
		lua_tinker::def( mLua, "GetCampID",						&CRepetionModule::GetCampID );
		lua_tinker::def( mLua, "EventNoticeByCamp",				&CRepetionModule::EventNoticeByCamp );
		lua_tinker::def( mLua, "SendBattleEndNotifyByFamily",	&CRepetionModule::SendBattleEndNotifyByFamily );
		lua_tinker::def( mLua, "ChangeMode",					&CRepetionModule::ChangeMode );
		lua_tinker::def( mLua, "ChangeCampFlagByCorps",				&CRepetionModule::ChangeCampFlagByCorps );
		lua_tinker::def( mLua, "BattleResultNoticeByCorps",			&CRepetionModule::BattleResultNoticeByCorps );
		lua_tinker::def( mLua, "GetParame",						&CRepetionModule::GetParame );
		lua_tinker::def( mLua, "InsertItem",					&CRepetionModule::InsertItem );
		lua_tinker::def( mLua, "InsertItemByCamp",				&CRepetionModule::InsertItemByCamp );
		lua_tinker::def( mLua, "InsertItemAll",					&CRepetionModule::InsertItemAll );
		lua_tinker::def( mLua, "InsertMoneyAll",				&CRepetionModule::InsertMoneyAll );
		lua_tinker::def( mLua, "NpcRemoveBuff", 				&CRepetionModule::NpcRemoveBuff );
		lua_tinker::def( mLua, "NpcRemoveBuffByMapID",			&CRepetionModule::NpcRemoveBuffByMapID );
		lua_tinker::def( mLua, "NpcInsertBuffByMapID",			&CRepetionModule::NpcInsertBuffByMapID );
		lua_tinker::def( mLua, "PlayerRemoveBuffByMapID",		&CRepetionModule::PlayerRemoveBuffByMapID );
		lua_tinker::def( mLua, "CreateRedstone",				&CRepetionModule::CreateRedstone );
		lua_tinker::def( mLua, "GetRepetionKey",				&CRepetionModule::GetRepetionKey );
		lua_tinker::def( mLua, "FamilyRepetionInfoEnd",			&CRepetionModule::FamilyRepetionInfoEnd );
		lua_tinker::def( mLua, "GetBackMapID",					&CRepetionModule::GetBackMapID );
		lua_tinker::def( mLua, "SendBattleMapNoticeByCorps",	&CRepetionModule::SendBattleMapNoticeByCorps );
		lua_tinker::def( mLua, "SetVirtualMap",    				&CRepetionModule::SetVirtualMap );
		lua_tinker::def( mLua, "SetMapIntData",    				&CRepetionModule::SetMapIntData );
		lua_tinker::def( mLua, "GetMapIntData",    				&CRepetionModule::GetMapIntData );
		lua_tinker::def( mLua, "AllTeleport", 					&CRepetionModule::AllTeleport );
		lua_tinker::def( mLua, "NpcChangeLevel",				&CRepetionModule::NpcChangeLevel );
		lua_tinker::def( mLua, "EntityRemoveBuff",				&CRepetionModule::EntityRemoveBuff );
		lua_tinker::def( mLua, "EntityInsertBuff",				&CRepetionModule::EntityInsertBuff );
		lua_tinker::def( mLua, "SetTodayTimesByGroupID",		&CRepetionModule::SetTodayTimesByGroupID );
		lua_tinker::def( mLua, "PlayerLeave",		            &CRepetionModule::PlayerLeave);
		lua_tinker::def( mLua, "KickPlayerFromTeam",		    &CRepetionModule::KickPlayerFromTeam);
		lua_tinker::def( mLua, "AddTitleAll", 					&CRepetionModule::AddTitleAll );
		lua_tinker::def( mLua, "SendEventNotice",				&CRepetionModule::SendEventNotice );
		lua_tinker::def( mLua, "GetNameByCharID",				&CRepetionModule::GetNameByCharID );
		lua_tinker::def( mLua, "PlayerInsertBuffByMapID",		&CRepetionModule::PlayerInsertBuffByMapID );
		lua_tinker::def( mLua, "GetPlayerNumByMapID",			&CRepetionModule::GetPlayerNumByMapID );
		lua_tinker::def( mLua, "AttachPlayerState",				&CRepetionModule::AttachPlayerState );
		lua_tinker::def( mLua, "SetIntData2Player",				&CRepetionModule::SetIntData2Player );
		lua_tinker::def( mLua, "GetIntData2Player",				&CRepetionModule::GetIntData2Player );
		lua_tinker::def( mLua, "AddIntData2Player",				&CRepetionModule::AddIntData2Player );
		lua_tinker::def( mLua, "EventNoticeAllInPlayerData",	&CRepetionModule::EventNoticeAllInPlayerData );
		lua_tinker::def( mLua, "ObtainRepetionExp",				&CRepetionModule::ObtainRepetionExp );
		lua_tinker::def( mLua, "CreateNeedfire" ,				&CRepetionModule::CreateNeedfire );
		lua_tinker::def( mLua, "SendEventNotify",				&CRepetionModule::SendEventNotify);
//		lua_tinker::def( mLua, "CreateNpcDynamic",				&CRepetionModule::PlayerCreateNpcDynamic);
		lua_tinker::def( mLua, "PlayerReliveBySys",				&CRepetionModule::PlayerReliveBySys); 
		lua_tinker::def( mLua, "DealBattleWinner",				&CRepetionModule::DealBattleWinner); 
		lua_tinker::def( mLua, "SendSysMailForRepetion",		&CRepetionModule::SendSysMailForRepetion); 
		lua_tinker::def( mLua, "GetRepetionPlayerID",			&CRepetionModule::GetRepetionPlayerID);
		lua_tinker::def( mLua, "NpcRefreshView",				&CRepetionModule::NpcRefreshView );
		lua_tinker::def( mLua, "CallFun",						&CRepetionModule::CallFun );
		lua_tinker::def( mLua, "CharacterHasBuf",				&CRepetionModule::CharacterHasBuf );
		lua_tinker::def( mLua, "NpcLoop4TempInLuaByMapID",		&CRepetionModule::NpcLoop4TempInLuaByMapID );
		lua_tinker::def( mLua, "PlayerLoopInLuaByMapID",		&CRepetionModule::PlayerLoopInLuaByMapID );
		lua_tinker::def( mLua, "PlayerLeaveByMapID",			&CRepetionModule::PlayerLeaveByMapID );
		lua_tinker::def( mLua, "GetPlayerDieCount",				&CRepetionModule::GetPlayerDieCountInRepetion );
		lua_tinker::def( mLua, "GetLevelInLua",					&CRepetionModule::GetEntityLevel);
		lua_tinker::def( mLua, "SendMessageInLua",				&CRepetionModule::SendMessageInLua);
		lua_tinker::def( mLua, "GetEntityMapID",				&CRepetionModule::GetMapID );
		lua_tinker::def( mLua, "GetEntityMapObjID",				&CRepetionModule::GetMapObjectID );
		lua_tinker::def( mLua, "EndGlobalWar" ,					&CRepetionModule::EndGlobalWar);
		lua_tinker::def( mLua, "GetTemplateLevelExp",			&CRepetionModule::GetTemplateLevelExp );		
		lua_tinker::def( mLua, "AddIntDataByCampID",			&CRepetionModule::AddIntDataByCampID );
		lua_tinker::def( mLua, "AwardFameAll",					&CRepetionModule::AwardFameAll );
		lua_tinker::def( mLua, "AwardFameAllCamp1",				&CRepetionModule::AwardFameAllCamp1 );
		lua_tinker::def( mLua, "AwardFameAllCamp2",				&CRepetionModule::AwardFameAllCamp2 );
		lua_tinker::def( mLua, "AwardFameSingle",				&CRepetionModule::AwardFameSingle );
		lua_tinker::def( mLua, "CheckAward",					&CRepetionModule::CheckAward );
		lua_tinker::def( mLua, "CreateNpcDynamic",				&CRepetionModule::CreateNpcDynamic );
		lua_tinker::def( mLua, "RepeLogScore",					&CRepetionModule::RepeLogScore );

		lua_tinker::class_add<CDir>( mLua, "CDir");
		lua_tinker::class_mem<CDir>( mLua, "bad",   &CDir::bad );
		lua_tinker::class_con<CDir>( mLua, lua_tinker::constructor<CDir, const char*> );
		lua_tinker::class_def<CDir>( mLua, "file", &CDir::file );

		snprintf( tLuaFile, sizeof( tLuaFile ) - 1, "%s/repetion_class.lua", vpLuaDir );
		lua_tinker::dofile( mLua, tLuaFile );
		snprintf( tLuaFile, sizeof( tLuaFile ) - 1, "%s/repetion_global.lua", vpLuaDir );
		lua_tinker::dofile( mLua, tLuaFile );
		snprintf( tLuaFile, sizeof( tLuaFile ) - 1, "%s/repetion_app.lua", vpLuaDir );
		lua_tinker::dofile( mLua, tLuaFile );
		snprintf( tLuaFile, sizeof( tLuaFile ) - 1, "%s/repetion_func.lua", vpLuaDir );
		lua_tinker::dofile( mLua, tLuaFile );


		tRtn = lua_tinker::call<int, const char*>( mLua, "init_repetion", vpLuaDir );
	}
	else
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Lua Open Error", __LK_FILE__, __LINE__, __FUNCTION__ );
		tRtn = ERR_UNKNOWN;
	}

	return tRtn;
}

void CRepetionModule::Debug( const char* vpStr )
{
	if( vpStr != NULL )
	{
		LOG_DEBUG( REPETION_LOG, "[ Lua ] %s", vpStr );
	}
}

void CRepetionModule::Error( const char* vpStr )
{
	if( vpStr != NULL )
	{
		LOG_ERROR( REPETION_LOG, "[ Lua ] %s", vpStr );
	}
}

void CRepetionModule::Infor( const char* vpStr )
{
	if( vpStr != NULL )
	{
		LOG_INFO( REPETION_LOG, "[ Lua ] %s", vpStr );
	}
}

void CRepetionModule::Notice( const char* vpStr )
{
	if( vpStr != NULL )
	{
		LOG_NOTICE( REPETION_LOG, "[ Lua ] %s", vpStr );
	}
}

// Regist RepetionMap Info from Lua
void CRepetionModule::RegisterRepetion( lua_tinker::table vTable/*CRepetionInfo* vpRepetion*/ )
{
	lua_tinker::table tMapIDTable = vTable.get< lua_tinker::table >( "mapid" );
	int	tRepetionID	= vTable.get< int >( "repetion_index" );
	int i = 0;
	OpenDate tDate;

	CRepetionInfo* tpRepetion = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( tRepetionID );
	if( tpRepetion == NULL )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Register RepetionMap MapID( %d ) Err", __LK_FILE__, __LINE__, 
				__FUNCTION__, tRepetionID );
		return;
	}

	if( tpRepetion->GetBeginMapID( ) != INVALID_MAP_ID )
	{
		if( tpRepetion->GetBeginMapID( ) != tMapIDTable.get< int >( 0 ) )
		{
			// repetionID vRepetionID IsUesed
			LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] RepetionID %d Is Used, Cannt Registe New", __LK_FILE__, 
				__LINE__, __FUNCTION__, tRepetionID );
			return;
		}
	}

	tpRepetion->SetRepetionIndex( tRepetionID );
	for( i = 1; i <= tMapIDTable.getn( ); i++ )
	{
		tpRepetion->SetMapID( tMapIDTable.get< int >( i ) );
	}

	tpRepetion->SetPlayerCountMax( vTable.get< unsigned int >( "playercountmax" ) );
	tpRepetion->SetPos( vTable.get< unsigned short >( "posx" ), vTable.get< unsigned short >( "posy" ) );
	tpRepetion->SetTaskID( vTable.get< unsigned short >( "taskid" ) );
	tpRepetion->SetPlayerPerNumber( vTable.get< unsigned short >( "pernumber" ) );
	tpRepetion->SetPlayerWeekPerNumber( vTable.get< unsigned short >( "weekpernumber" ) );
	tpRepetion->SetMinLevel( vTable.get< unsigned short >( "minlevel" ) );
	tpRepetion->SetMaxLevel( vTable.get< unsigned short >( "maxlevel" ) );
	tpRepetion->SetUnRelive( vTable.get< int >( "unrelive" ) );
	tpRepetion->SetMinCheckTime( vTable.get< int >( "check_time" ) );
	tpRepetion->SetQuestionCheck( vTable.get< int >( "question_check" ) );
	tpRepetion->SetTeamLevelFlag( ( REPE_LEVEL )vTable.get< int >( "team_level_flag" ) );
	tpRepetion->SetIsOnTimeOpen( vTable.get< int >( "ontimeopen" ) );
	tpRepetion->SetNotLimitTime( vTable.get< int >( "notlimittime"));
	tpRepetion->SetDelayTime( vTable.get< int >("delay_time"));

	//通过lua脚本设置副本最短存在时间
	int tTimeSecond = vTable.get< int >( "mintimer" );
	tpRepetion->SetMinTimer( tTimeSecond * 1000 );
	LOG_DEBUG(REPETION_LOG, "RepetionID:%d, MinTime:%d sec", tRepetionID, tTimeSecond);

	lua_tinker::table tSpecCheckList = vTable.get< lua_tinker::table >( "join_check_key" );	
	for( i = 1; i <= tSpecCheckList.getn( ); i++ )
	{
		lua_tinker::table tSpecCheck = tSpecCheckList.get< lua_tinker::table >( i );
		tpRepetion->SetJoinSpecCheck( tSpecCheck.get<unsigned int>( "min_level" ), 
			tSpecCheck.get<unsigned int>( "max_level" ), tSpecCheck.get<unsigned int>( "key" ) );
	}


	lua_tinker::table tTeam = vTable.get< lua_tinker::table >( "team" );
	if( tTeam.get< bool >( "teamin" ) == true )
	{
		tpRepetion->SetTeamIn( );
		tpRepetion->SetPlayerCount( tTeam.get< unsigned short >( "count" ) );
		REPE_ASSERT( tpRepetion->GetPlayerCount( ) <= tpRepetion->GetPlayerCountMax(  ), return )
	}


	lua_tinker::table tTable = vTable.get< lua_tinker::table >( "backpos" );
	tpRepetion->SetBackMapID( tTable.get< unsigned short >( "mapid" ) );
	tpRepetion->SetBackPos( tTable.get< unsigned short >( "posx" ), tTable.get< unsigned short >( "posy" ) );

	lua_tinker::table tSpecial = vTable.get< lua_tinker::table >( "special" );
	for( i = 1; i <= tSpecial.getn( ); i++ )
	{
		lua_tinker::table tSpecialItem = tSpecial.get< lua_tinker::table >( i );
		tpRepetion->SetSpecialItem( tSpecialItem.get< int >( "item" ), tSpecialItem.get< int >( "flag" ) );
	}	

	tpRepetion->SetNecessity( vTable.get< int >( "necessity" ) );
	
	tpRepetion->SetSpecialCheck( vTable.get< int >( "special_check" ));
	tpRepetion->SetPlayerDataToClientIndex( vTable.get< int >( "to_cleint_index" ) );
	
	// 副本可重入配置
	tpRepetion->SetRelogin( vTable.get<int>( "relogin" ) );
	tpRepetion->SetCloseTime( vTable.get<int>( "closetime" ) );

	// 每次进入需要扣除的次数
	int nConsumeTimes = vTable.get<int>( "consumetimes" );
	if (nConsumeTimes <= 1)
	{
		// 默认为1次
		nConsumeTimes = 1;
	}
	tpRepetion->SetConsumeTimes(nConsumeTimes);
}

void CRepetionModule::RegisteOpenTime( int vRepetionIndex, lua_tinker::table vTable )
{
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return, "RepetionIndex %d", vRepetionIndex );

	DateSpace tSpace;

	lua_tinker::table tOpenStartTable = vTable.get< lua_tinker::table >( "start" ); 
	tSpace.mBegin.mYear = tOpenStartTable.get< int >( "year" );
	tSpace.mBegin.mMonth = tOpenStartTable.get< int >( "month" );
	tSpace.mBegin.mWeek = tOpenStartTable.get< int >( "week" );
	tSpace.mBegin.mDay = tOpenStartTable.get< int >( "day" );
	tSpace.mBegin.mMinute = tOpenStartTable.get< int >( "minute" );
	tSpace.mBegin.mHr = tOpenStartTable.get< int >( "hr" );
	tSpace.mBegin.mSec = tOpenStartTable.get< int >( "sec" );
	
	lua_tinker::table tOpenFinishTable = vTable.get< lua_tinker::table >( "finish" );
	tSpace.mEnd.mYear = tOpenFinishTable.get< int >( "year" );
	tSpace.mEnd.mMonth = tOpenFinishTable.get< int >( "month" );
	tSpace.mEnd.mWeek = tOpenFinishTable.get< int >( "week" );
	tSpace.mEnd.mDay = tOpenFinishTable.get< int >( "day" );
	tSpace.mEnd.mMinute = tOpenFinishTable.get< int >( "minute" );
	tSpace.mEnd.mHr = tOpenFinishTable.get< int >( "hr" );
	tSpace.mEnd.mSec = tOpenFinishTable.get< int >( "sec" );

	tpInfo->SetOpenDate( &tSpace );

}
//add by yucx 2010年 08月 26日 星期四 17:56:58
void CRepetionModule::RegisteSignUpTime( int vRepetionIndex, lua_tinker::table vTable )
{
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return, "RepetionIndex %d", vRepetionIndex );

	DateSpace tSpace;

	lua_tinker::table tOpenStartTable = vTable.get< lua_tinker::table >( "start" ); 
	tSpace.mBegin.mYear = tOpenStartTable.get< int >( "year" );
	tSpace.mBegin.mMonth = tOpenStartTable.get< int >( "month" );
	tSpace.mBegin.mWeek = tOpenStartTable.get< int >( "week" );
	tSpace.mBegin.mDay = tOpenStartTable.get< int >( "day" );
	tSpace.mBegin.mMinute = tOpenStartTable.get< int >( "minute" );
	tSpace.mBegin.mHr = tOpenStartTable.get< int >( "hr" );
	tSpace.mBegin.mSec = tOpenStartTable.get< int >( "sec" );
	
	lua_tinker::table tOpenFinishTable = vTable.get< lua_tinker::table >( "finish" );
	tSpace.mEnd.mYear = tOpenFinishTable.get< int >( "year" );
	tSpace.mEnd.mMonth = tOpenFinishTable.get< int >( "month" );
	tSpace.mEnd.mWeek = tOpenFinishTable.get< int >( "week" );
	tSpace.mEnd.mDay = tOpenFinishTable.get< int >( "day" );
	tSpace.mEnd.mMinute = tOpenFinishTable.get< int >( "minute" );
	tSpace.mEnd.mHr = tOpenFinishTable.get< int >( "hr" );
	tSpace.mEnd.mSec = tOpenFinishTable.get< int >( "sec" );

	tpInfo->SetSignUpDate( &tSpace );

}
void CRepetionModule::RegisteBattleOpenSpace( unsigned int vRepetionIndex, lua_tinker::table vBattleOpen )
{
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return, "RepetionIndex %d", vRepetionIndex );

	for( int i = 1; i <= vBattleOpen.getn( ); i++ )
	{
		DateSpace tSpace;

		lua_tinker::table tDateSpaceTable = vBattleOpen.get< lua_tinker::table >( i );

		lua_tinker::table tOpenStartTable = tDateSpaceTable.get< lua_tinker::table >( "start" ); 
		tSpace.mBegin.mYear = tOpenStartTable.get< int >( "year" );
		tSpace.mBegin.mMonth = tOpenStartTable.get< int >( "month" );
		tSpace.mBegin.mWeek = tOpenStartTable.get< int >( "week" );
		tSpace.mBegin.mDay = tOpenStartTable.get< int >( "day" );
		tSpace.mBegin.mMinute = tOpenStartTable.get< int >( "minute" );
		tSpace.mBegin.mHr = tOpenStartTable.get< int >( "hr" );
		tSpace.mBegin.mSec = tOpenStartTable.get< int >( "sec" );

		lua_tinker::table tOpenFinishTable = tDateSpaceTable.get< lua_tinker::table >( "finish" );
		tSpace.mEnd.mYear = tOpenFinishTable.get< int >( "year" );
		tSpace.mEnd.mMonth = tOpenFinishTable.get< int >( "month" );
		tSpace.mEnd.mWeek = tOpenFinishTable.get< int >( "week" );
		tSpace.mEnd.mDay = tOpenFinishTable.get< int >( "day" );
		tSpace.mEnd.mMinute = tOpenFinishTable.get< int >( "minute" );
		tSpace.mEnd.mHr = tOpenFinishTable.get< int >( "hr" );
		tSpace.mEnd.mSec = tOpenFinishTable.get< int >( "sec" );

		tpInfo->SetBattleOpenSpace( &tSpace );	
	}
}


CMapObject* CRepetion::GetMapObject( int vIndex )
{
	REPE_ASSERT_STR( vIndex >= 0, return NULL, "vIndex %d", vIndex );
	REPE_ASSERT_STR( vIndex < ( int )ARRAY_CNT( mMapObjIDList ), return NULL, "vIndex %d", vIndex );

//	REPE_ASSERT_STR( mMapObjIDList[ vIndex ].mMapObjID != INVALID_OBJ_ID, return NULL, "vIndex %d", vIndex );
	if( mMapObjIDList[ vIndex ].mMapObjID == INVALID_OBJ_ID )
	{
		return NULL;
	}

	CMapObject* tpMapObj = CMapObjManager::GetSingletonPtr( )->GetMapObject( 
		mMapObjIDList[ vIndex ].mMapObjID );

	REPE_ASSERT_STR( tpMapObj != NULL, return NULL, "vIndex %d, mMapObjID %d, RepetionID %d : %lld", vIndex, 
		mMapObjIDList[ vIndex ].mMapObjID, get_id( ), GetKey( ) );

	return tpMapObj;
}

// Repetion Load Config
int CRepetion::Load( CRepetionInfo* vpRepetionInfo  )
{
#ifdef _FB_
	unsigned int i = 0;

	REPE_ASSERT( vpRepetionInfo != NULL, return ERR_INVALID_PARAMETER );

	SetRepetionIndex( vpRepetionInfo->GetRepetionIndex( ) );

	for( i = 0; i < ARRAY_CNT( mTimer ) && mTimeCount < ARRAY_CNT( mTimer ); i++ )
	{
		const RTimeEvent* tpTimeEvent = vpRepetionInfo->GetTimeEvent( i );
		if( tpTimeEvent != NULL )
		{
			if( tpTimeEvent->mType == UN_BEGIN )
			{
				continue;
			}

			mTimer[ mTimeCount ].mEvent = *tpTimeEvent;
			mTimeCount++;
		}
	}

	for( i = 0; i < ARRAY_CNT( mEventOpen ); i++ )
	{
		mEventOpen[ i ] = vpRepetionInfo->GetEvent( ( RepetionEvent )i );
	}

	mMinTimer = vpRepetionInfo->GetMinTimer( );
#endif

	return SUCCESS;
}

void CRepetion::OnTimer( unsigned int vTickCount, std::vector< int >* vpDelete )
{
#ifdef _FB_
	for( int i = mList.size( ) - 1; i >= 0; i-- )
	{
		ENTITY_TARGET_POS tTargetPos = mList[ i ];
		CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( tTargetPos.mEntityID );
		if( tpEntity == NULL || tpEntity->IsNpc( ) == false )
		{
			mList.erase( mList.begin( ) + i );
			continue;
		}

		CEntityNpc* tpNpc = static_cast< CEntityNpc* >(tpEntity);

		if( tpNpc->IsAlive( ) == true )
		{
			
			if( tpEntity->GetPosX( ) == tTargetPos.mPosX && tpEntity->GetPosY( ) == tTargetPos.mPosY )
			{

				LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Entity( %d ) Pos( %d : %d ) Target( %d : %d )", __LK_FILE__,
					__LINE__, __FUNCTION__, tpEntity->GetEntityID( ), tpEntity->GetPosX( ), tpEntity->GetPosY( ),
					tTargetPos.mPosX, tTargetPos.mPosY );

				mList.erase( mList.begin( ) + i );

				CCreator* tpCreator = ( CCreator* )CSceneObjManager::GetSingletonPtr( )->GetObject( 
					tpNpc->GetCreatorID( ));

				lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );

				vParame.set( "entityid", tpEntity->GetEntityID( ) );
				vParame.set( "posx",	tpEntity->GetPosX( ) );
				vParame.set( "posy",	tpEntity->GetPosY( ) );
				vParame.set( "index", 	( tpCreator != NULL ) ? tpCreator->GetIndex( ) : 0 );
				vParame.set( "tempid",	tpNpc->GetNpcID( ) );

				DoEvent( GetRepetionInfo( ), REPETION_ENTITY_MOVE_STOP, &vParame );

			}
		}
	}

	if( mMinTimer > 0 )
	{
		mMinTimer = ( mMinTimer >= ( int )vTickCount ) ? mMinTimer - ( int )vTickCount : 0;
		if( mMinTimer == 0 )
		{
			if( GetPlayerCount( ) == 0 )
			{	
				// 如果是可重入副本,检查是否可以关闭
				if ( RepetionRemainAlive() == false )
				{
					SetClear();
				}						
			}

			if( GetRemoveItem( ) != 0 )
			{
				SetSysClose( );
				AllPlayerLeave( );	
			}
		}
	}

	if( GetMinCheckTime( ) > 0 )
	{
		SetMinCheckTime( GetMinCheckTime( ) - ( int )vTickCount );
		if( GetMinCheckTime( ) <= 0 )
		{
			CRepetionInfo* tpInfo = GetRepetionInfo( );
			if( tpInfo != NULL )
			{
				if( CSceneCfgManager::GetSingletonPtr( )->GetRepetion_cfg( )->mPlayercheck != 0 )
				{
					int tIsSpec = PlayerEntityLoop( CRepetion::AllPlayerInSpecLevel, 0, 0 );
					if( tIsSpec != SUCCESS && GetPlayerCount( ) < tpInfo->GetPlayerCount( ) )
					{
						SetSysClose( );
						AllPlayerLeave( true );
					}
					else
					{
						if( GetPlayerCount( ) == 0 && GetPlayerJoinTimes( ) != 0 )
						{
							SetSysClose( );
							AllPlayerLeave( true );	
						}
					}
				}
			}
		}
	}	
#endif
}

int CRepetion::TimeBegin( RTIME_TYPE vType )
{
#ifdef _FB_
	for(unsigned int i = 0; i < mTimeCount && i < ARRAY_CNT( mTimer ); i++ )
	{
		if( mTimer[ i ].mEvent.mType == vType &&  mTimer[ i ].mIsBegin == false )
		{
			TimeBeginByIndex( i );
		}
	}

#endif
	return SUCCESS;
}

int CRepetion::TimeEnd( RTIME_TYPE vType )
{
#ifdef _FB_
	for( unsigned int i = 0; i < mTimeCount && i < ARRAY_CNT( mTimer ); i++ )
	{
		if( mTimer[ i ].mEvent.mType == vType &&  mTimer[ i ].mIsBegin == true )
		{
			TimeEndByIndex( i );
		}
	}
	
#endif
	return SUCCESS;
}

int CRepetion::TimePauseByIndex( unsigned int vIndex )
{
#ifdef _FB_
	REPE_ASSERT_STR( vIndex < mTimeCount, return ERR_INVALID_PARAMETER, "Index %d, TimeCount %d", vIndex, mTimeCount );

	if( mTimer[ vIndex ].mIsBegin == false )
	{
		return SUCCESS;
	}

	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ]RepetionIndex( %d ) Time( %d : %d : %d ) TimeID %d Pause", __LK_FILE__,
		__LINE__, __FUNCTION__, GetRepetionIndex( ), vIndex, mTimer[ vIndex ].mEvent.mType, 
		mTimer[ vIndex ].mEvent.mResult, mTimer[ vIndex ].mTimeID );

	mTimer[ vIndex ].mTime = GetTime( vIndex );;
	mTimer[ vIndex ].mIsBegin = false;
//	mTimer[ vIndex ].mLoopCount = 0; //mTimer[ i ].mEvent.mLoopCountMax;

	mTimer[ vIndex ].ClearTimer( );

	if( mTimer[ vIndex ].mEvent.mSync == SYNC_ALL )
	{
		CRepetionModule::EventNoticeAll( get_id( ), REPETION_COUNTDOWN_END, mTimer[ vIndex ].mEvent.mSyncKey, 0 );
	}
	else if( mTimer[ vIndex ].mEvent.mSync == SYNC_CAMP )
	{
		CRepetionModule::EventNoticeByCamp( get_id( ), mTimer[ vIndex ].mEvent.mCamp, REPETION_COUNTDOWN_END, 
			mTimer[ vIndex ].mEvent.mSyncKey, 0 );
	}

#endif
	return SUCCESS;

}

int CRepetion::TimeEndByIndex( unsigned int vIndex )
{
#ifdef _FB_
	REPE_ASSERT_STR( vIndex < mTimeCount, return ERR_INVALID_PARAMETER, "Index %d, TimeCount %d", vIndex, mTimeCount );

	if( mTimer[ vIndex ].mIsBegin == false )
	{
		return SUCCESS;
	}

	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ]RepetionIndex( %d ) Time( %d : %d : %d ) TimeID %d End", __LK_FILE__,
		__LINE__, __FUNCTION__, GetRepetionIndex( ), vIndex, mTimer[ vIndex ].mEvent.mType, 
		mTimer[ vIndex ].mEvent.mResult, mTimer[ vIndex ].mTimeID );

	mTimer[ vIndex ].mIsBegin = false;
	mTimer[ vIndex ].mTime = 0;
	mTimer[ vIndex ].mLoopCount = 0; //mTimer[ i ].mEvent.mLoopCountMax;

	mTimer[ vIndex ].ClearTimer( );

	if( mTimer[ vIndex ].mEvent.mSync == SYNC_ALL )
	{
		CRepetionModule::EventNoticeAll( get_id( ), REPETION_COUNTDOWN_END, mTimer[ vIndex ].mEvent.mSyncKey, 0 );
	}
	else if( mTimer[ vIndex ].mEvent.mSync == SYNC_CAMP )
	{
		CRepetionModule::EventNoticeByCamp( get_id( ), mTimer[ vIndex ].mEvent.mCamp, REPETION_COUNTDOWN_END, 
			mTimer[ vIndex ].mEvent.mSyncKey, 0 );
	}

#endif
	return SUCCESS;
}


int CRepetionModule::DestroyRepetion( CRepetion* vpRepetion )
{
#ifdef _FB_
	REPE_ASSERT( vpRepetion != NULL, return ERR_INVALID_PARAMETER );

	CMessage tMsg;
	CMessageDestroyRepetionMap tMsgFact;

	tMsg.set_msgpara((uint32_t) &tMsgFact );
	tMsg.mutable_msghead()->set_messageid( ID_S2G_DESTROY_REPETION_MAP );

	tMsgFact.set_key( vpRepetion->GetKey( ) );
	tMsgFact.set_repetionindex( vpRepetion->GetRepetionIndex( ) );
	tMsgFact.set_parame( vpRepetion->GetParame( 0 ) );

	CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMsg );

	vpRepetion->End( );

	for( int i = 0; i < vpRepetion->GetMapObjSizeMax( ); i++ )
	{
		CMapObject* tpMapObj = vpRepetion->GetMapObject( i );
		if( tpMapObj == NULL )
		{
			break;
		}	
		tpMapObj->SetRepetionID( INVALID_OBJ_ID );

		CMapObjManager::GetSingletonPtr( )->DestroyMapObject( tpMapObj );
	}
	// 通知全局服务器副本结束
	if ( IS_DUNGEONSERVER_LINE( CSceneLogic::GetSingleton().GetLineID() ) )
	{
		CGlobalModule::GetSingleton().EndGlobalWar( EREASON_ENDWAR_REPETIONEND, vpRepetion->GetParame( 0 ) );
	}	
	CSceneObjManager::GetSingletonPtr( )->DestroyObject( vpRepetion->get_id( ) );
#endif
	return SUCCESS;
}

int CRepetionModule::SendErrCode(	CEntityPlayer* vpPlayer, 
									int vErrCode, 
									unsigned short vMapID, 
									int vParame1, 
									int vParame2 
								)
{
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	CMessage tMsg;
	CMessageRepetionErrCode tErrMsg;

	tErrMsg.set_code( vErrCode );
//	tErrMsg.set_entityid( vpPlayer->get_id( ) );
	tErrMsg.set_charid( vpPlayer->GetCharID( ) );
	tErrMsg.set_parame1( vParame1 );
	tErrMsg.set_parame2( vParame2 );
	tErrMsg.set_mapid( vMapID );

	tMsg.mutable_msghead()->set_messageid( ID_S2C_REPETION_ERR );
	tMsg.set_msgpara( (uint32)&tErrMsg );

	CSceneLogic::GetSingletonPtr( )->Send2Player( vpPlayer->GetCharID( ), &tMsg );

	return SUCCESS;
}

int CRepetionModule::SendErrCode( 
	int* vpPlayerListID, 
	int vPlayerCount, 
	int vErrCode,
	int vPlayerCharID,
	unsigned short vMapID,
	int vParame1,
	int vParame2
	)
{
	CMessage tMsg;
	CMessageRepetionErrCode tErrMsg;

	tErrMsg.set_code( vErrCode );
	//		tErrMsg.set_entityid( vpPlayer->get_id( ) );
	tErrMsg.set_charid( vPlayerCharID );
	tErrMsg.set_parame1( vParame1 );
	tErrMsg.set_parame2( vParame2 );
	tErrMsg.set_mapid( vMapID );

	tMsg.mutable_msghead()->set_messageid( ID_S2C_REPETION_ERR );
	tMsg.set_msgpara( (uint32)&tErrMsg );

	for( int i = 0; i < vPlayerCount; i++ )
	{
		CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByCharID( vpPlayerListID[ i ] );
		if( tpPlayer != NULL )
		{
			CSceneLogic::GetSingletonPtr( )->Send2Player( tpPlayer, &tMsg );
		}
	}

	return SUCCESS;
}

int CRepetionModule::SendTeamErrCode(	CEntityPlayer* vpPlayer, 
										ERepetion vErrCode, 
										unsigned short vMapID,
										int vParame1, 
										int vParame2 
									)
{
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

//	CEntityPlayer* vpHead = CTeamModule::GetSingletonPtr( )->GetTeamHeader( vpPlayer );
//	if( vpHead == NULL )
//	{
//		return ERR_UNKNOWN;
//		vpHead = vpPlayer;
//	}

	int tCharArr[ TEAM_NUMBER ] = { 0 };
	int tRtn = 0, tCount = 0;
	tRtn = CTeamModule::GetSingletonPtr( )->OnGetMemberList( vpPlayer->GetCharID( ), tCharArr, ARRAY_CNT( tCharArr ), tCount );
	if ( tRtn < 0 )
	{
		LOG_ERROR( "team", "repetion,get team list failed.id=%d,[%s:%d]", vpPlayer->GetCharID(), __LK_FILE__, __LINE__ );
		return -1;
	}

	for( int i = 0; i < tCount; i++ )
	{
		if( tCharArr[ i ] == 0 )
		{
			continue;
		}
		
		CMessage tMsg;
		CMessageRepetionErrCode tErrMsg;

		tErrMsg.set_code( vErrCode );
//		tErrMsg.set_entityid( vpPlayer->get_id( ) );
		tErrMsg.set_charid( vpPlayer->GetCharID( ) );
		tErrMsg.set_parame1( vParame1 );
		tErrMsg.set_parame2( vParame2 );
		tErrMsg.set_mapid( vMapID );

		tMsg.mutable_msghead()->set_messageid( ID_S2C_REPETION_ERR );
		tMsg.set_msgpara( (uint32)&tErrMsg );

		CSceneLogic::GetSingletonPtr( )->Send2Player( tCharArr[ i ], &tMsg );
	}

	return SUCCESS;
}


int CRepetionModule::EndTask( CEntityPlayer* vpPlayer, unsigned int vTaskID, bool vIsComplate )
{
#ifdef _FB_
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	if( IS_REPETION_LINE_ID( vpPlayer->GetLineID( ) ) == true )
	{
		CRepetionInfo* tpRepetionInfo = GetRepetionInfoByMapID( vpPlayer->GetMapID( ) );
		if( tpRepetionInfo != NULL )
		{
			if( tpRepetionInfo->GetTaskID( ) == vTaskID )
			{
				CRepetion* tpRepetion = GetRepetion( vpPlayer );
				if( tpRepetion == NULL )
				{
					LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Cannt Get Repetion", __LK_FILE__, __LINE__, 
							__FUNCTION__ );
					return ERR_NOT_FOUND;
				}
				// Complate Task, Begin Task End Event
//				tpRepetion->TimeBegin( COMPLETE_TASK_BEGIN );
				lua_tinker::table vParame( mLua );
				vParame.set( "taskid", vTaskID );
				vParame.set( "complate", ( vIsComplate ==  true ) ? 1 : 0 );
//				PERF_FUNC( "Rpetion_DoEvent", tpRepetion->DoEvent( tpRepetionInfo, REPETION_TASK_END, vTaskID, ( vIsComplate ==  true ) ? 1 : 0 ) );
				PERF_FUNC( "Rpetion_DoEvent", tpRepetion->DoEvent( tpRepetionInfo, REPETION_TASK_END, &vParame ) );
			}
		}
	}
#endif //_FB_
	return SUCCESS;
}

CRepetion* CRepetionModule::GetRepetion( CEntityCharacter* vpPlayer )
{
	CRepetion* tpRepetion = NULL;
#ifdef _FB_
	REPE_ASSERT( vpPlayer != NULL, return NULL );

	CMapObject* tpMap = CMapObjManager::GetSingletonPtr( )->GetMapObject( vpPlayer->GetLineID( ), 
			vpPlayer->GetMapID( ), vpPlayer->GetMapIndex( ) );
	if( tpMap == NULL )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Cann't find MapObject( %d : %d : %d )", __LK_FILE__, __LINE__, 
			__FUNCTION__, vpPlayer->GetLineID( ), vpPlayer->GetMapID( ), vpPlayer->GetMapIndex( ) );
		return NULL;
	}
	tpRepetion = tpMap->GetRepetion( );
#endif // _FB_
	return tpRepetion;
}

int CRepetionModule::CleanRepetion( CPropertyPlayer* vpPlayer )
{
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	CPlayerRepetion* tpRepetion = vpPlayer->GetRepetion( );
	REPE_ASSERT( tpRepetion != NULL, return ERR_UNKNOWN );
	
	tpRepetion->CleanRepetion( );

	return SUCCESS;
}

void CRepetionModule::RegisteEvent( int vRepetionIndex, RepetionEvent vEventType )
{
#ifdef _FB_
	CRepetionInfo* tpRepetionInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vRepetionIndex );
	if( tpRepetionInfo == NULL )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Cannt Get RepetionInfo From RepetionIndex( %d )", __LK_FILE__, 
			__LINE__, __FUNCTION__, vRepetionIndex );
		return;
	}

	tpRepetionInfo->RegisteEvent( vEventType ); 
#endif
	return;
}


//int CRepetion::DoEvent( CRepetionInfo* vpRepetionInfo, RepetionEvent vEventType, int vParame1, int vParame2 )
int CRepetion::DoEvent( CRepetionInfo* vpRepetionInfo, RepetionEvent vEventType, lua_tinker::table *vParame )
{
#ifdef _FB_
	REPE_ASSERT( vpRepetionInfo != NULL, return ERR_INVALID_PARAMETER );

	if( vpRepetionInfo->GetEvent( vEventType ) == true )
	{
//		lua_tinker::call< int, unsigned short, unsigned int, RepetionEvent, int, int>( 
//			CRepetionModule::GetSingletonPtr( )->mLua, "do_event", vpRepetionInfo->GetMapID( ), 
//			get_id( ), vEventType, vParame1, vParame2 );
		lua_tinker::call< int, unsigned short, unsigned int, RepetionEvent, lua_tinker::table >( 
			CRepetionModule::GetSingletonPtr( )->mLua, "do_event", vpRepetionInfo->GetRepetionIndex( ),
			get_id( ), vEventType, *vParame );
		return SUCCESS;
	}
#endif
	return SUCCESS;
}

void CRepetionModule::CreateNpc( unsigned int vRepetionID, unsigned int vIndex )
{
#ifdef _FB_
	CCreator* tpCreator = CCreatorManager::GetSingletonPtr( )->GetCreator( vIndex );
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpCreator != NULL, return, "Cannt GetCreator From Index( %d )", vIndex );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "Cannt GetRepetion( %d )", vRepetionID );

	CMapObject* tpMapObject = NULL;//tpRepetion->GetMapObject( );

	for( int i = 0; i < tpRepetion->GetMapObjSizeMax( ); i++ )
	{
		tpMapObject = tpRepetion->GetMapObject( i );
		if( tpMapObject == NULL )
		{
			break;
		}

		if( tpMapObject->IsCreatorIn( vIndex ) == true )
		{
			break;
		} 

		tpMapObject = NULL;
	}

	REPE_ASSERT_STR( tpMapObject != NULL, return, "Cannt GetMapObject( RepetionID = %d )", vRepetionID );

	int ret = tpMapObject->CreateNpc( tpCreator, false );

	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ]Create Npc, RepetionIndex( %d : %d :%lld ) CreateIndex %d, Map( %d : %d"
		" : %d ), ret %d", __LK_FILE__, __LINE__, __FUNCTION__, tpRepetion->GetRepetionIndex( ), tpRepetion->get_id( ),
		tpRepetion->GetKey( ), vIndex, tpMapObject->GetLineID( ), tpMapObject->GetMapID( ), 
		tpMapObject->GetMapIndex( ), ret );
#endif	

	return;
}

int CRepetion::TimeEvent( unsigned int vTimeIndex )
{
#ifdef _FB_
	REPE_ASSERT( vTimeIndex < ARRAY_CNT( mTimer ), return ERR_INVALID_PARAMETER );


	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ]RepetionIndex( %d ) Time( %d : %d : %d ) Fire Lua Event",
		__LK_FILE__, __LINE__, __FUNCTION__, GetRepetionIndex( ), vTimeIndex, mTimer[ vTimeIndex ].mEvent.mType, 
		mTimer[ vTimeIndex ].mEvent.mResult );

	lua_tinker::call< int, unsigned short, unsigned int, int, unsigned int >( 
		CRepetionModule::GetSingletonPtr( )->mLua, "time_event", GetRepetionIndex( ), get_id( ), 
		vTimeIndex + 1, GetLoopCount( vTimeIndex ) + 1 );
#endif
	return SUCCESS;
}

unsigned int CRepetion::GetLoopCount( unsigned int vTimeIndex )
{
#ifdef _FB_
	REPE_ASSERT( vTimeIndex < ARRAY_CNT( mTimer ), return ERR_INVALID_PARAMETER );

	return mTimer[ vTimeIndex ].mLoopCount;
#endif
	return 0;
}

//void CRepetionModule::RegisteTimeEvent(	unsigned short vMapID,
//										unsigned int vTimeIndex, 
//										RTIME_TYPE vType, 
//										RTIME_RESULT vResult,
//										unsigned int vTime,
//										int vLoopCount,
//										bool vIsCallFunc,
//										lua_tinker::table vSync
//									)
void CRepetionModule::RegisteTimeEvent( int vRepetionIndex,
										unsigned int vTimeIndex,
										lua_tinker::table vTimeInfo,
										bool vIsCallFunc
									)
{
#ifdef _FB_
	CRepetionInfo* tpRepetionInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vRepetionIndex );
	if( tpRepetionInfo == NULL )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %s ][ %s ]Cannt GetRepetionInfo( %d )", __LK_FILE__, __LINE__, __FUNCTION__,
			vRepetionIndex );
		return;
	}

//	tpRepetionInfo->SetTimeEvent( vTimeIndex - 1, vType, vResult, vTime, vLoopCount, vIsCallFunc, vSync ); 
	tpRepetionInfo->SetTimeEvent( vTimeIndex - 1, vTimeInfo.get<RTIME_TYPE>( "kind" ), 
		vTimeInfo.get<RTIME_RESULT>( "result" ), vTimeInfo.get<int>( "time" ), vTimeInfo.get<int>( "loop" ), 
		vIsCallFunc, vTimeInfo.get<lua_tinker::table>( "sync" ) );

#endif
	return;
}


void CRepetionModule::UseSkill( unsigned int vSrcEntityID, unsigned int vDesEntityID, unsigned int vSkillTempID )
{
#ifdef _FB_
	CEntity* tpSrcEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vSrcEntityID );
	CEntity* tpDesEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vDesEntityID );
	CTemplateSkill* tpSkill = ( CTemplateSkill* )CDataStatic::SearchTpl( vSkillTempID );

	REPE_ASSERT_STR( tpSrcEntity != NULL, return, "ERR_INVALID_PARAMETER, SrcEntityID %d", vSrcEntityID );
	REPE_ASSERT_STR( tpDesEntity != NULL, return, "ERR_INVALID_PARAMETER, SrcEntityID %d", vDesEntityID );
	REPE_ASSERT_STR( tpSkill != NULL, return, "ERR_INVALID_PARAMETER, TempID %d", vSkillTempID );

	REPE_ASSERT_STR( tpSrcEntity->IsCharacter(), return, "Fact Type %d", tpSrcEntity->GetEntityType( ) );
	REPE_ASSERT_STR( tpDesEntity->IsCharacter(), return, "Fact Type %d", tpDesEntity->GetEntityType( ) );

	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] UseSkill From vSrcEntityID %d To vDesEntity %d, SkillTempID %d",
		__LK_FILE__, __LINE__, __FUNCTION__, vSrcEntityID, vDesEntityID, vSkillTempID );

	CPropertyModule::GetSingletonPtr( )->UseSkillForServer( ( CEntityCharacter* )tpSrcEntity,
		( CEntityCharacter* )tpDesEntity, tpSkill, NULL, 0 );
	
#endif
	return;
}


void CRepetionModule::AddEnmity2Template(	unsigned int vRepetionID, 
											unsigned int vSrcEntityID, 
											unsigned int vDesTemplateID,
											unsigned int vEnmityValue )
{
#ifdef _FB_

	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return, "ERR_INVALID_PARAMETER, Repetion ID is %d", vRepetionID );

	tpRepetion->NpcEntityLoop( vDesTemplateID, CRepetion::AddEnmity, vSrcEntityID, vEnmityValue );

#endif
	return;
}


CRepetion* CRepetionModule::GetRepetion( unsigned int vObjID )
{
	return ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( vObjID );
}

int CRepetion::NpcEntityLoop( unsigned int vTemplateID, Fun_EntityLoop vFun, int vParame1, int vParame2 )
{
#ifdef _FB_
	bool tContinue = true;

	for( int j = 0; j < ( int )ARRAY_CNT( mMapObjIDList ); j++ )
	{
		if( mMapObjIDList[ j ].mMapObjID == INVALID_OBJ_ID )
		{
			break;
		}
		CMapObject* tpMapObj = GetMapObject( j );

		if ( tpMapObj == NULL )
		{
			break;
		}

		for( int i = tpMapObj->mEntity.size( ) - 1; i >= 0; i-- )
		{
			CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( tpMapObj->mEntity[ i ] );

			REPE_ASSERT( tpEntity != NULL, continue );

			if( tpEntity->IsCharacter() == false )
			{
				continue;	
			}

			if( vTemplateID != 0 && ( tpEntity->GetNpcID( ) != vTemplateID ) )
			{
				continue;
			}


			if( vFun != NULL )
			{
				tContinue = vFun( this, ( CEntityCharacter* )tpEntity, vParame1, vParame2 );
			}

			if( tContinue != true )
			{
				return SUCCESS;
			}
		}
	}
#endif

	return SUCCESS;
}

int CRepetion::NpcEntityLoopByMap( 
		unsigned int vTemplateID, 
		Fun_EntityLoop vFun, 
		int vMapID, 
		int vParame1, 
		int vParame2 )
{
#ifdef _FB_
	bool tContinue = true;

	CMapObject* tpMapObj = GetMapObjectByMapID( vMapID );
	REPE_ASSERT_STR( tpMapObj != NULL, return ERR_INVALID_PARAMETER, "vMapID %d", vMapID );


	for( int i = tpMapObj->mEntity.size( ) - 1; i >= 0; i-- )
	{
		CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( tpMapObj->mEntity[ i ] );

		REPE_ASSERT( tpEntity != NULL, continue );

		if( tpEntity->IsCharacter() == false )
		{
			continue;	
		}

		if( vTemplateID != 0 && ( tpEntity->GetNpcID( ) != vTemplateID ) )
		{
			continue;
		}


		if( vFun != NULL )
		{
			tContinue = vFun( this, ( CEntityCharacter* )tpEntity, vParame1, vParame2 );
		}

		if( tContinue != true )
		{
			return SUCCESS;
		}
	}
#endif

	return SUCCESS;
}



void CRepetionModule::UseSkill2Template(	unsigned int vRepetionID,
											unsigned int vSrcEntityID,
											unsigned int vDesTemplateID,
											unsigned int vSkillTempID
									  )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return, "ERR_INVALID_PARAMETER, Repetion ID is %d", vRepetionID );


	tpRepetion->NpcEntityLoop( vDesTemplateID, CRepetion::UseSkill, vSrcEntityID, vSkillTempID );

	return;
#endif
}

bool CRepetion::UseSkill( CRepetion* vpRepetion, CEntityCharacter* vpDesEntity, int vSrcEntityID, int vSkillTempID )
{
#ifdef _FB_	
	REPE_ASSERT( vpDesEntity != NULL, return true );

	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vSrcEntityID );
	CTemplateSkill* tpSkill = ( CTemplateSkill* )CDataStatic::SearchTpl( vSkillTempID );

	REPE_ASSERT_STR( tpEntity != NULL, return true, "ERR_INVALID_PARAMETER, EntityID Is %d", vSrcEntityID );
	REPE_ASSERT_STR( tpSkill != NULL, return true, "ERR_INVALID_PARAMETER, SkillID Is %d", vSkillTempID );

	REPE_ASSERT_STR( tpEntity->IsCharacter(), return true, "ERR_INVALID_PARAMETER" );

    LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] UseSkill From vSrcEntityID %d To vDesEntity %d, SkillTempID %d",
        __LK_FILE__, __LINE__, __FUNCTION__, vSrcEntityID, vpDesEntity->GetEntityID( ), vSkillTempID );

	CPropertyModule::GetSingletonPtr( )->UseSkillForServer( ( CEntityCharacter* )tpEntity,
		vpDesEntity, tpSkill, NULL, 0 );
#endif
	return true;
}

bool CRepetion::AddEnmity( CRepetion* vpRepetion, CEntityCharacter* vpDesEntity, int vSrcEntityID, int vEnmityValue )
{
#ifdef _FB_
	CEntityCharacter* pEntityChar = (CEntityCharacter*)CSceneObjManager::GetSingletonPtr()->GetObject(vSrcEntityID);
	REPE_ASSERT( pEntityChar != NULL, return false );
	CNpcModule::GetSingletonPtr( )->OnAttacked( vpDesEntity, pEntityChar, vEnmityValue );
	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] SrcEntity( %d ) Add Enmity Into DesEntity( %d ), Value( %d )", 
		__LK_FILE__, __LINE__, __FUNCTION__, vSrcEntityID, vpDesEntity->GetEntityID( ), vEnmityValue );
#endif
	return true;
}

void CRepetionModule::TimeBegin( unsigned int vRepetionID, unsigned int vTimeIndex )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT( tpRepetion != NULL, return );
	tpRepetion->TimeBeginByIndex( vTimeIndex - 1 );
#endif
}

void CRepetionModule::TimeRestart( unsigned int vRepetionID, unsigned int vTimeIndex )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT( tpRepetion != NULL, return );
	tpRepetion->TimeRestartByIndex( vTimeIndex - 1 );
#endif
}


int CRepetion::TimeRestartByIndex( unsigned int vIndex )
{
#ifdef _FB_
	REPE_ASSERT_STR( vIndex < mTimeCount, return ERR_INVALID_PARAMETER, "Index %d, TimeCount %d", vIndex, mTimeCount );

	if( mTimer[ vIndex ].mIsBegin == true )
	{
		return SUCCESS;
	}

	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] RepetionIndex( %d ) Time( %d : %d : %d ) Restart", __LK_FILE__,
		__LINE__, __FUNCTION__, GetRepetionIndex( ), vIndex, mTimer[ vIndex ].mEvent.mType, 
		mTimer[ vIndex ].mEvent.mResult );

	mTimer[ vIndex ].mIsBegin = true;
//	mTimer[ vIndex ].mTime = mTimer[ vIndex ].mEvent.mInitTime;
//	mTimer[ vIndex ].mLoopCount = 0; //mTimer[ i ].mEvent.mLoopCountMax;

	mTimer[ vIndex ].SetTimer( get_id( ), vIndex, mTimer[ vIndex ].mTime );

	CTimerItem* tpTimer = CTimerMng::GetSingletonPtr( )->GetTimer( mTimer[ vIndex ].mTimeID );
	REPE_ASSERT_STR( tpTimer != NULL, return ERR_NO_ENOUGH_MEMORY, "RepetionID %d, Index %d, TimeID %d", get_id( ), 
		vIndex, mTimer[ vIndex ].mTimeID );

	if( mTimer[ vIndex ].mEvent.mSync == SYNC_ALL )
	{
		CRepetionModule::EventNoticeAll( get_id( ), REPETION_COUNTDOWN, mTimer[ vIndex ].mEvent.mSyncKey, 
			tpTimer->mMillSeconds );
	}
	else if( mTimer[ vIndex ].mEvent.mSync == SYNC_CAMP )
	{
		CRepetionModule::EventNoticeByCamp( get_id( ), mTimer[ vIndex ].mEvent.mCamp, REPETION_COUNTDOWN, 
			mTimer[ vIndex ].mEvent.mSyncKey, tpTimer->mMillSeconds );
	}

#endif
	return SUCCESS;

}

int CRepetion::TimeBeginByIndex( unsigned int vIndex )
{
#ifdef _FB_
	REPE_ASSERT_STR( vIndex < mTimeCount, return ERR_INVALID_PARAMETER, "Index %d, TimeCount %d", vIndex, mTimeCount );

	if( mTimer[ vIndex ].mIsBegin == true )
	{
		return SUCCESS;
	}

	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] RepetionIndex( %d ) Time( %d : %d : %d ) Begin", __LK_FILE__,
		__LINE__, __FUNCTION__, GetRepetionIndex( ), vIndex, mTimer[ vIndex ].mEvent.mType, 
		mTimer[ vIndex ].mEvent.mResult );

	mTimer[ vIndex ].mIsBegin = true;
	mTimer[ vIndex ].mTime = mTimer[ vIndex ].mEvent.mInitTime;
	mTimer[ vIndex ].mLoopCount = 0; //mTimer[ i ].mEvent.mLoopCountMax;

	mTimer[ vIndex ].SetTimer( get_id( ), vIndex );

	CTimerItem* tpTimer = CTimerMng::GetSingletonPtr( )->GetTimer( mTimer[ vIndex ].mTimeID );
	REPE_ASSERT_STR( tpTimer != NULL, return ERR_NO_ENOUGH_MEMORY, "RepetionID %d, Index %d, TimeID %d", get_id( ), 
		vIndex, mTimer[ vIndex ].mTimeID );

	if( mTimer[ vIndex ].mEvent.mSync == SYNC_ALL )
	{
		CRepetionModule::EventNoticeAll( get_id( ), REPETION_COUNTDOWN, mTimer[ vIndex ].mEvent.mSyncKey, 
			tpTimer->mMillSeconds );
	}
	else if( mTimer[ vIndex ].mEvent.mSync == SYNC_CAMP )
	{
		CRepetionModule::EventNoticeByCamp( get_id( ), mTimer[ vIndex ].mEvent.mCamp, REPETION_COUNTDOWN, 
			mTimer[ vIndex ].mEvent.mSyncKey, tpTimer->mMillSeconds );
	}

#endif
	return SUCCESS;
}

void CRepetionModule::ChangeTimeSpace( unsigned int vRepetionID, unsigned int vTimeIndex, unsigned int vTimeSpace )
{
#ifdef _FB_
	int tRtn = SUCCESS;

	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT( tpRepetion != NULL, return );

	tRtn = tpRepetion->ChangeTimeSpace( vTimeIndex - 1, vTimeSpace );
	if( tRtn != SUCCESS )
	{
		LOG_ERROR( REPETION_LOG,"[ %s : %d ][ %s ]Call ChangeTimeSpace Is Rtn( %d ) Error", __LK_FILE__,
			__LINE__, __FUNCTION__, tRtn );
	}
#endif	
	return;
}

int CRepetion::ChangeTimeSpace( unsigned int vTimeIndex, unsigned int vTimeSpace )
{
#ifdef _FB_
	REPE_ASSERT( vTimeIndex < mTimeCount, return ERR_INVALID_PARAMETER );

	if( mTimer[ vTimeIndex ].mIsBegin == true )
	{
		LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Change TimeSpace( %d : %d )", __LK_FILE__, __LINE__, 
			__FUNCTION__, mTimer[ vTimeIndex ].mEvent.mInitTime, vTimeSpace );
		mTimer[ vTimeIndex ].mEvent.mInitTime = vTimeSpace;

		CTimerItem* tpTime = CTimerMng::GetSingletonPtr( )->GetTimer( mTimer[ vTimeIndex ].mTimeID );
		REPE_ASSERT_STR( tpTime != NULL, return NULL, "Cannt Get Repetion( %d : %lld ) Time( %d ) Timer( %d )",
			get_id( ), GetKey( ), vTimeIndex, mTimer[ vTimeIndex ].mTimeID );

		tpTime->mMillSeconds = vTimeSpace;
		tpTime->mDurationSeconds = vTimeSpace;
	}
	else
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] Change TimeSpace( %d ) Is Not Begin", __LK_FILE__, __LINE__,
			__FUNCTION__, vTimeIndex );
	}
#endif
	return SUCCESS;
}

int CRepetionInfo::SetTimeEvent(	unsigned int vIndex,
								RTIME_TYPE vType,
								RTIME_RESULT vResult,
								unsigned int vTime,
								int vLoopCount,
								bool vIsCallFunc,
								lua_tinker::table vSync
							)
{
	if( vIndex >= ARRAY_CNT( mTimeEvent ) )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %d : %d ), Time FUll", __LK_FILE__, __LINE__, 
				__FUNCTION__, GetRepetionIndex( ), GetBeginMapID( ) );
		return ERR_INVALID_PARAMETER;
	}
	mTimeEvent[ vIndex ].mType = vType;
	mTimeEvent[ vIndex ].mResult = vResult;
	mTimeEvent[ vIndex ].mInitTime = vTime;
	mTimeEvent[ vIndex ].mIsCallFunc = vIsCallFunc;
	mTimeEvent[ vIndex ].mLoopCountMax = vLoopCount;
	mTimeEvent[ vIndex ].mSync = vSync.get< RTIME_SYNC >( "kind" );
	mTimeEvent[ vIndex ].mCamp = vSync.get< int >( "camp" );
	mTimeEvent[ vIndex ].mSyncKey = vSync.get< int >( "key" );

	return SUCCESS;

}

int CRepetion::PlayerJumpTowerFloor( CEntityPlayer* tpPlayer,int tstep,int tfloor)
{
#ifdef _FB_
	REPE_ASSERT( tpPlayer != NULL, return ERR_INVALID_PARAMETER );


	lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );

	CRepetionInfo* tpRepetionInfo = GetRepetionInfo();
	REPE_ASSERT( tpRepetionInfo != NULL, return ERR_INVALID_PARAMETER );

	vParame.set( "entityid", tpPlayer->get_id() );
	vParame.set( "playerlevel",tpPlayer->GetLevel() );
	vParame.set( "playermapid",tpPlayer->GetMapID());
	vParame.set( "jfloor", tfloor);
    vParame.set( "jstep", tstep);

    PERF_FUNC( "Repetion_DoEvent", DoEvent( GetRepetionInfo( ), REPETION_DEBUG_COMMAND, &vParame ) );

#endif
	return SUCCESS;
}


int  CRepetionModule::PlayerJumpTower(CEntityPlayer* tpPlayer,int vstep,int vfloor)
{
	

#ifdef _FB_
	CMapObject* obj = tpPlayer->GetMapObject();
	REPE_ASSERT_STR( obj != NULL, return -1, "Cannt Get Repetion( %d )" );
	CRepetion* tpRepetion = obj->GetRepetion();
	REPE_ASSERT_STR( tpRepetion != NULL, return -1, "Cannt Get Repetion( %d )" );
	tpRepetion->PlayerJumpTowerFloor(tpPlayer,vstep,vfloor);
#endif
	return 0;
}


int CRepetion::Begin( )
{
#ifdef _FB_
	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %d : %lld )  Begin", __LK_FILE__, __LINE__, __FUNCTION__, 
		get_id( ), GetKey( ) );
	SetBeginTime( time( NULL ) );

	lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );

	CRepetionInfo* tpRepetionInfo = GetRepetionInfo();
	REPE_ASSERT( tpRepetionInfo != NULL, return ERR_INVALID_PARAMETER );
	vParame.set( "parame1", GetParame( 0 ) );
	vParame.set( "parame2", GetParame( 1 ) );

	DoEvent( GetRepetionInfo( ), REPETION_EVENT_BEGIN, &vParame );	

	TimeBegin( START_BEGIN );

#endif
	return SUCCESS;
}

int CRepetion::End( )
{
#ifdef _FB_

    lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );
    
    DoEvent( GetRepetionInfo( ), REPETION_CLOSE_EVENT, &vParame );

	ClearAllTimer( );

	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %d : %lld ) End", __LK_FILE__, __LINE__, __FUNCTION__, 
		get_id( ), GetKey( ) );
#endif
	return SUCCESS;
}

void CRepetionModule::EventClose( unsigned int vRepetionID, RepetionEvent vEventType )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "Cannt Get Repetion( %d )", vRepetionID );
	tpRepetion->EventClose( vEventType );
#endif
}

int CRepetion::EventClose( RepetionEvent vEventType )
{
#ifdef _FB_
	REPE_ASSERT_STR( vEventType < ( int )ARRAY_CNT( mEventOpen ), return ERR_INVALID_PARAMETER, "vEventType %d", 
		vEventType );
	mEventOpen[ vEventType ] = false;
#endif
	return SUCCESS;
}

void CRepetionModule::TimeEnd( unsigned int vRepetionID, unsigned int vTimeIndex )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "Cannt Get Repetion( %d )", vRepetionID );
	tpRepetion->TimeEndByIndex( vTimeIndex - 1 );
#endif
}

void CRepetionModule::TimePause( unsigned int vRepetionID, unsigned int vTimeIndex )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "Cannt Get Repetion( %d )", vRepetionID );
	tpRepetion->TimePauseByIndex( vTimeIndex - 1 );
#endif
}

int CRepetion::CharacterDeath( CEntityCharacter* vKilled, CEntityCharacter* vKiller )
{
#ifdef _FB_
	REPE_ASSERT( vKilled != NULL, return ERR_INVALID_PARAMETER );
	if( vKilled->IsCharacter() )
	{
		int tKillerCharID = 0;
		
		if( vKiller != NULL )
		{
			LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Entity %d die, killed by entity %d", __LK_FILE__, __LINE__,
				__FUNCTION__, vKilled->GetEntityID(), vKiller->GetEntityID() );
		}
		else
		{
			LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Entity %d die, killer cann't find", __LK_FILE__, __LINE__,
				__FUNCTION__, vKilled->GetEntityID() );
		}

		if( vKiller == NULL )
		{
			tKillerCharID = 0;
		}
		else if( vKiller->IsPlayer() == true )
		{
			tKillerCharID = ( ( CEntityPlayer* )vKiller )->GetCharID( );
		}
		else if( vKiller->IsPet() )
		{
			int tOwnID = vKiller->GetOwnerID( );
			CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( tOwnID );
			if( tpEntity == NULL )
			{
				LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Cannt GetEntity for %d", __LK_FILE__, __LINE__, __FUNCTION__,
					tOwnID );
				return ERR_UNKNOWN;
			}

			if( tpEntity->IsPlayer()  == false )
			{
				LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Entity %d Is Not A Player", __LK_FILE__, __LINE__,
					__FUNCTION__, tOwnID );
				return ERR_UNKNOWN;
			}

			tKillerCharID = ( ( CEntityPlayer* )tpEntity )->GetCharID( );
		}
		else
		{
			LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Entity %d die, Killer Is Not A Player or Pet", __LK_FILE__,
				__LINE__, __FUNCTION__, vKilled->GetEntityID( ) );
//			return ERR_UNKNOWN;
		}
		
		int tKilledSex = -1;
		if( vKilled->IsPlayer())
		{
			tKilledSex = (( CEntityPlayer* )vKilled)->GetSex();

			CPlayerRepetion* tpPlayerRepetion = CRepetionModule::GetSingletonPtr( )->GetPlayerRepetion( ( CEntityPlayer* )vKilled );
			tpPlayerRepetion->AddDieCount( GetRepetionIndex() );

		}
		
		CMapObject* tpMapObj = ( vKilled == NULL ) ? NULL : vKilled->GetMapObject( );

		CCreator* tpCreator = ( CCreator* )CSceneObjManager::GetSingletonPtr( )->GetObject( vKilled->GetCreatorID( ) );
		
		lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );
		vParame.set( "killercharid", tKillerCharID );
		vParame.set( "killerid", ( vKiller == NULL ) ? 0 : vKiller->GetEntityID( ) );
		vParame.set( "killedid", vKilled->GetEntityID( ) );
		vParame.set( "killedisplayer",(vKilled->IsPlayer() == true) ? 1 : 0);
		vParame.set( "killedlevel", vKilled->GetLevel());
		vParame.set( "killedposx", vKilled->GetPosX());
		vParame.set( "killedposy", vKilled->GetPosY());
		vParame.set( "killedsex", tKilledSex );
		vParame.set( "killerlevel", ( vKiller == NULL ) ? 0 : vKiller->GetLevel());
		vParame.set( "tempid",  vKilled->GetNpcID( ) );
		vParame.set( "index", ( tpCreator != NULL ) ? tpCreator->GetIndex( ) : 0 );
        vParame.set( "killercampid", ( vKiller == NULL ) ? 0 : vKiller->GetCampID( ) );
        vParame.set( "killedcampid", ( vKilled == NULL ) ? 0 : vKilled->GetCampID( ) );	
		vParame.set( "real_map", ( tpMapObj == NULL ) ? 0 : tpMapObj->GetMapID( ) );
		vParame.set( "virtual_map", ( tpMapObj == NULL ) ? 0 : tpMapObj->GetVirtualMapID( ) );
		

		vKilled->LogoutObserver( this, CCharacterObservable::CHARACTER_DEATH );

		PERF_FUNC( "Repetion_DoEvent", DoEvent( GetRepetionInfo( ), REPETION_OGRE_DIE, &vParame ) );

	
	}
#endif
	return SUCCESS;
}

CRepetionInfo* CRepetion::GetRepetionInfo( )
{
	CRepetionInfo* tpRepetionInfo = NULL;
#ifdef _FB_
	tpRepetionInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( GetRepetionIndex( ) );
#endif
	return tpRepetionInfo;
}

void CRepetionModule::ObserverEntityDeath( unsigned int vRepetionID, unsigned int vEntityID )
{
#ifdef _FB_
	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ]ObserverEntityDeath %d, %d", __LK_FILE__, __LINE__, __FUNCTION__, 
		vRepetionID, vEntityID );

	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );

	REPE_ASSERT_STR( tpRepetion != NULL, return, "EntityID %d", vRepetionID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );

	if( tpEntity->IsCharacter() )
	{
		CEntityCharacter* tpCharacter = ( CEntityCharacter* )tpEntity;
		tpCharacter->RegisterObserver( tpRepetion, CCharacterObservable::CHARACTER_DEATH );
	}
	else
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Entity( %d ) Not A Character, Type Is %d", 
			__LK_FILE__, __LINE__, __FUNCTION__, vEntityID, tpEntity->GetEntityType( ) );
	}
#endif
}

int CRepetionModule::DestroyAliveTempOgre( unsigned int vRepetionID, unsigned int vTempID, unsigned int vCount )
{
#ifdef _FB_
	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] DestroyAliveTempOgre %d, %d", __LK_FILE__, 
		__LINE__, __FUNCTION__, vRepetionID, vTempID );
	
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return 0, "Cannt GetRepetion( %d )", vRepetionID );
	int tCount = 0;

	for( int i = 0; i < tpRepetion->GetMapObjSizeMax( ); i++ )
	{

		CMapObject* tpMapObj = tpRepetion->GetMapObject( i );
		if( tpMapObj == NULL )
		{
			break;
		}

		tCount += tpMapObj->DestroyAliveTempOgreEntity( vTempID, vCount - tCount );
		//	CPropertyModule::GetSingletonPtr( )->ResultListSend( );

	}
	return tCount;
#endif
	return 0;
}

void CRepetionModule::EventNoticeAll(	unsigned int vRepetionID, 
										REPETION_EVENT_NOTICE vEvent, 
										int vParame1, 
										int vParame2 
									)
{
#ifdef _FB_
	CMessage tMsg;
	CMessageRepetionEventNotic tMsgFact;

	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT( tpRepetion != NULL, return );

	CRepetionInfo* tpInfo = tpRepetion->GetRepetionInfo( );
	REPE_ASSERT( tpInfo != NULL, return );

	tMsgFact.set_mapid( tpInfo->GetBeginMapID( ) );
	tMsgFact.set_eventid( vEvent );
	tMsgFact.set_parame1( vParame1 );
	tMsgFact.set_parame2( vParame2 );

    tMsg.mutable_msghead()->set_messageid( ID_S2C_REPETION_EVENT );
	tMsg.set_msgpara( (uint32)&tMsgFact );

	for( int i = 0; i < tpRepetion->GetMapObjSizeMax( ); i++ )
	{
		CMapObject* tpMapObj = tpRepetion->GetMapObject( i );
		if( tpMapObj == NULL )
		{
			break;
		}

		tpMapObj->SendMsg2Player( &tMsg );	
	}

	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] Send CMessageRepetionEventNotic To All Player In Repetion( %d : %d ),"
		" EventID : %d , Parame1 : %d, Parame2 : %d", __LK_FILE__, __LINE__, __FUNCTION__, 
		tpRepetion->GetRepetionIndex( ), tpRepetion->get_id( ), vEvent, vParame1, vParame2 );
#endif
}

void CRepetionModule::EventNoticeSingle(	unsigned int vRepetionID,
											unsigned int vEntityID,
											REPETION_EVENT_NOTICE vEvent,
											int vParame1,
											int vParame2
										)
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT( tpEntity != NULL, return );
	REPE_ASSERT( tpEntity->IsPlayer()  == true, return );
	
	CMessage tMsg;
	CMessageRepetionEventNotic tMsgFact;

	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT( tpRepetion != NULL, return );

	CRepetionInfo* tpInfo = tpRepetion->GetRepetionInfo( );
	REPE_ASSERT( tpInfo != NULL, return );

	tMsgFact.set_mapid( tpInfo->GetBeginMapID( ) );
	tMsgFact.set_eventid( vEvent );
	tMsgFact.set_parame1( vParame1 );
	tMsgFact.set_parame2( vParame2 );

	tMsg.mutable_msghead()->set_messageid( ID_S2C_REPETION_EVENT );
	tMsg.set_msgpara( (uint32)&tMsgFact );

	CSceneLogic::GetSingletonPtr( )->Send2Player( ( CEntityPlayer* )tpEntity, &tMsg );

	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] Send Player( %s : %d : %d ) CMessageRepetionEventNotic( RepetionIndex"
		" : %d,eventid : %d, parame1 : %d, parame2 : %d", __LK_FILE__, __LINE__, __FUNCTION__, 
		( ( CEntityPlayer* )tpEntity )->GetCharNameShow( ), ( ( CEntityPlayer* )tpEntity )->GetCharID( ), 
		tpEntity->GetEntityID( ), tpRepetion->GetRepetionIndex( ), vEvent, vParame1, vParame2 );
#endif
}

bool CRepetionModule::IntoRepetionTimeCheck( CRepetionInfo* vInfo )
{
	REPE_ASSERT( vInfo != NULL, return false );

	return vInfo->IsInOpenTime( );
}

bool CRepetionModule::SignUpRepetionTimeCheck( CRepetionInfo* vInfo )
{
	REPE_ASSERT( vInfo != NULL, return false );

	return vInfo->IsInSignUpTime( );
}

int CRepetionInfo::SetOpenDate( DateSpace* vpSpace )
{
	if( mDateSpace.size( ) >= mDateSpace.max_size( ) )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %d : %d ) OpenTime Is Full, size is %d max_size is %d",
			__LK_FILE__, __LINE__, __FUNCTION__,  GetRepetionIndex( ), GetBeginMapID( ), mDateSpace.size( ), 
			mDateSpace.max_size( ) );
		return ERR_FULL;
	}

	mDateSpace.push_back( *vpSpace );
	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %d : %d ) Insert OpenTime", __LK_FILE__, __LINE__,
		__FUNCTION__, GetRepetionIndex( ), GetBeginMapID( ) );

	return SUCCESS;
}

int CRepetionInfo::SetBattleOpenSpace( DateSpace* vpSpace )
{
	if( mBattleOpenSpace.size( ) >= mBattleOpenSpace.max_size( ) )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %d : %d ) OpenTime Is Full, size is %d max_size is %d",
			__LK_FILE__, __LINE__, __FUNCTION__,  GetRepetionIndex( ), GetBeginMapID( ), mDateSpace.size( ), 
			mBattleOpenSpace.max_size( ) );
		return ERR_FULL;
	}

	mBattleOpenSpace.push_back( *vpSpace );
	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %d : %d ) Insert OpenTime", __LK_FILE__, __LINE__,
		__FUNCTION__, GetRepetionIndex( ), GetBeginMapID( ) );

	return SUCCESS;

}

const DateSpace* CRepetionInfo::GetBattleOpenSpace( int vIndex )
{
	if( vIndex < 0 || ( unsigned int )vIndex >= mBattleOpenSpace.size( ) )
	{
		return NULL;
	}

	return &mBattleOpenSpace[ vIndex ];
}

bool CRepetionInfo::IsInOpenTime( )
{
	if( CSceneCfgManager::GetSingletonPtr( )->GetRepetion_cfg( )->mTimecheck == 0 )
	{
		return true;
	}

	time_t tTime = time( NULL );
	struct tm* tTm = localtime( &tTime );
	REPE_ASSERT( tTm != NULL, return false );

	DATA_SPACE_LIST::iterator it;

	for( it = mDateSpace.begin( ); it != mDateSpace.end( ); it++ )
	{
		DateSpace tDateSpace = *it;

		if( tDateSpace.InDateSpace( tTm ) == true )
		{
			return true;
		}
	}

	return ( mDateSpace.size( ) == 0 ) ? true : false;
}
//add by yucx 2010年 08月 26日 星期四 16:22:28 设置报名时间
int CRepetionInfo::SetSignUpDate( DateSpace* vpSpace )
{
	if( mSignUpList.size( ) >= mSignUpList.max_size( ) )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %d : %d ) signuptime is Full, size is %d max_size is %d",
			__LK_FILE__, __LINE__, __FUNCTION__,  GetRepetionIndex( ), GetBeginMapID( ), mSignUpList.size( ), 
			mSignUpList.max_size( ) );
		return ERR_FULL;
	}

	mSignUpList.push_back( *vpSpace );
	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %d : %d ) Insert SignUptime", __LK_FILE__, __LINE__,
		__FUNCTION__, GetRepetionIndex( ), GetBeginMapID( ) );

	return SUCCESS;
}

bool CRepetionInfo::IsInSignUpTime( )
{
        //是否用 signupcheck ?
	if( CSceneCfgManager::GetSingletonPtr( )->GetRepetion_cfg( )->mTimecheck == 0 )
	{
		return true;
	}

	time_t tTime = time( NULL );
	struct tm* tTm = localtime( &tTime );
	REPE_ASSERT( tTm != NULL, return false );

	DATA_SPACE_LIST::iterator it;

	for( it = mSignUpList.begin( ); it != mSignUpList.end( ); it++ )
	{
		DateSpace tSignUpList= *it;

		if( tSignUpList.InDateSpace( tTm ) == true )
		{
			return true;
		}
	}

	return ( mSignUpList.size( ) == 0 ) ? true : false;
}
int CRepetion::SetData( int vKey, int vData )
{
#ifdef _FB_
	REPE_ASSERT( vKey >= 0, return ERR_INVALID_PARAMETER );
	REPE_ASSERT( ( unsigned int )vKey < sizeof( mData ), return ERR_INVALID_PARAMETER );

	mData[ vKey ].mUData.mIValue = vData;
#endif
	return SUCCESS;
}

int CRepetion::GetData( int vKey )
{
#ifdef _FB_
    REPE_ASSERT( vKey >= 0, return ERR_INVALID_PARAMETER );
	REPE_ASSERT( ( unsigned int )vKey < sizeof( mData ), return ERR_INVALID_PARAMETER );
	
	return mData[ vKey ].mUData.mIValue;
#endif
	return SUCCESS;
}

void CRepetionModule::SetIntData( unsigned int vRepetionID, int vKey, int vData )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	
	tpRepetion->SetData( vKey, vData );

	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %lld : %d : %d ) Set Data %d, index %d", __LK_FILE__, 
		__LINE__, __FUNCTION__, tpRepetion->GetKey( ), tpRepetion->GetRepetionIndex( ), tpRepetion->get_id( ),  
		vData, vKey );
#endif
	return;
}

int CRepetionModule::GetIntData( unsigned int vRepetionID, int vKey )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return ERR_INVALID_PARAMETER, "RepetionID %d", vRepetionID );

	int tData = tpRepetion->GetData( vKey );

	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %lld : %d : %d ) Get Data %d, index %d", __LK_FILE__, 
		__LINE__, __FUNCTION__, tpRepetion->GetKey( ), tpRepetion->GetRepetionIndex( ), tpRepetion->get_id( ), 
		tData, vKey );
	
	return tData;
	
#endif
	return 0;
}

int CRepetion::MachinerySpring( CEntityBox* vpMachinery, CEntity* vpSpringer )
{
#ifdef _FB_
	REPE_ASSERT( vpMachinery != NULL, return ERR_INVALID_PARAMETER );
	REPE_ASSERT( vpSpringer != NULL, return ERR_INVALID_PARAMETER );

	CRepetionInfo* tpRepetionInfo = GetRepetionInfo( );
	REPE_ASSERT( tpRepetionInfo != NULL, return ERR_INVALID_PARAMETER );
	CCreator* tpCreator = ( CCreator* )CSceneObjManager::GetSingletonPtr( )->GetObject( vpMachinery->GetCreatorID( ) );
	
	CMapObject* tpMapObj = vpMachinery->GetMapObject( );
	REPE_ASSERT_STR( tpMapObj != NULL, return ERR_INVALID_PARAMETER, "MapID %d, Index %d, Line %d", 
		vpMachinery->GetMapID( ), vpMachinery->GetMapIndex( ), vpMachinery->GetLineID( ) );
	

	lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );
	int tCharID = 0;
	if ( vpSpringer->IsPlayer()  )
	{
		tCharID = ( ( CEntityPlayer * )vpSpringer )->GetCharID();
	}
	
	vParame.set( "machineryid", vpMachinery->GetEntityID( ) );
	vParame.set( "springid", vpSpringer->GetEntityID( ) );
	vParame.set( "tempid", vpMachinery->GetNpcID( ) );
	vParame.set( "index", ( tpCreator != NULL ) ? tpCreator->GetIndex( ) : 0 );
	vParame.set( "campid", ( vpSpringer->IsCharacter( ) == true ) ? ( ( CEntityCharacter* )vpSpringer )->GetCampID( ) 
		: 0 );
	vParame.set( "virtual_map", tpMapObj->GetVirtualMapID( ) );
	vParame.set( "real_map", tpMapObj->GetMapID( ) );
	vParame.set( "springcharid", tCharID );

	PERF_FUNC( "Repetion_DoEvent", DoEvent( tpRepetionInfo, REPETION_BOX_MACHINERY_SPRING_EVENT, &vParame ) );
#endif

	return SUCCESS;
}

void CRepetionModule::ObserverMachinery( unsigned int vRepetionID, unsigned int vEntityID )
{
#ifdef _FB_
	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ]ObserverMachinery %d, %d", __LK_FILE__, __LINE__, __FUNCTION__, 
		vRepetionID, vEntityID );

	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );

	REPE_ASSERT_STR( tpRepetion != NULL, return, "EntityID %d", vRepetionID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );

	if( tpEntity->IsCharacter() )
	{
		CEntityCharacter* tpCharacter = ( CEntityCharacter* )tpEntity;
		tpCharacter->RegisterObserver( tpRepetion, CCharacterObservable::MACHINERY_SPRING );
	}
	else
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Entity( %d ) Not A Character, Type Is %d", 
			__LK_FILE__, __LINE__, __FUNCTION__, vEntityID, tpEntity->GetEntityType( ) );
	}
#endif
}

void CRepetionModule::CharacterDie( unsigned int vRepetionID, unsigned int vEntityID, unsigned int vIsAward )
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );

//	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID Is %d", vEntityID );
	if( tpEntity == NULL )
    {
    	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Warning: Entity( %d ) Is NULL", __LK_FILE__, __LINE__,
			__FUNCTION__, vEntityID );
        return;
    }

	REPE_ASSERT_STR( tpEntity->IsCharacter(), return, "EntityID Is %d, Type Is"
			" %d", vEntityID, tpEntity->GetEntityType( ) );

	CEntityCharacter* tpCharacter = ( CEntityCharacter* )tpEntity;

//	tpCharacter->SetAliveStatus( ALIVE_DIE );
//	tpCharacter->AddFunResult( tpCharacter->GetEntityID(), FUNC_DIE, RESULT_NONE, tpCharacter->GetPosX( ), 
//		tpCharacter->GetPosY( ) );
//	CPropertyModule::GetSingletonPtr( )->ResultListSend( );
//	tpCharacter->TimeForDie( );

	tpCharacter->EntityDie( tpCharacter, false, ( vIsAward == 0 ) ? false : true );
	CPropertyModule::GetSingletonPtr( )->ResultListSend( );

#endif
}

void CRepetionModule::ChangePKType( unsigned int vEntityID, unsigned int vPKType )
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsNpc(), return, "EntityID %d, "
		"EntityType %d", vEntityID, tpEntity->GetEntityType( ) );

	CEntityNpc* tpNpc = static_cast<CEntityNpc*>( tpEntity );
	tpNpc->SetPKType( vPKType );

	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] ChangePKType %d, EntityID %d", __LK_FILE__, __LINE__, __FUNCTION__,
		vPKType, vEntityID );

	CPropertyModule::GetSingletonPtr( )->NotifyClientNpcPKTypeChanged( tpNpc );
#endif
}

void CRepetionModule::AllPlayerAddEnmitiy( unsigned int vRepetionID, unsigned int vEntityID, int vEnmityValue )
{
#ifdef _FB_	
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );
	tpRepetion->PlayerEntityLoop( CRepetion::AddEnmity, vEntityID, vEnmityValue );
#endif
}


int CRepetion::PlayerEntityLoop( Fun_EntityLoop vFun, int vParame1, int vParame2 )
{
#ifdef _FB_
	bool tContinue = true;
	for( int j = 0; j < ( int )ARRAY_CNT( mMapObjIDList ); j++ )
	{
		if( mMapObjIDList[ j ].mMapObjID == INVALID_OBJ_ID )
		{
			break;
		}

		CMapObject* tpMapObj = GetMapObject( j );

		if( tpMapObj == NULL )
		{
			break;
		}

		for( int i = tpMapObj->mPlayers.size( ) - 1; i >= 0; i-- )
		{
			CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( tpMapObj->mPlayers[ i ] );

			REPE_ASSERT( tpEntity != NULL, continue );

			if( tpEntity->IsPlayer()  == false )
			{
				continue;	
			}

			if( vFun != NULL )
			{
				tContinue = vFun( this, ( CEntityCharacter* )tpEntity, vParame1, vParame2 );
			}

			if( tContinue != true )
			{
				return ERR_UNKNOWN;
			}
		}
	}
#endif

	return SUCCESS;
}

int CRepetion::PlayerEntityLoopByMap( Fun_EntityLoop vFun, int vMapID, int vParame1, int vParame2 )
{
#ifdef _FB_
	bool tContinue = true;

	CMapObject* tpMapObj = GetMapObjectByMapID( vMapID );

	REPE_ASSERT_STR( tpMapObj != NULL, return ERR_INVALID_PARAMETER, "MapID %d", vMapID );

	for( int i = tpMapObj->mPlayers.size( ) - 1; i >= 0; i-- )
	{
		CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( tpMapObj->mPlayers[ i ] );

		REPE_ASSERT( tpEntity != NULL, continue );

		if( tpEntity->IsPlayer()  == false )
		{
			continue;	
		}

		if( vFun != NULL )
		{
			tContinue = vFun( this, ( CEntityCharacter* )tpEntity, vParame1, vParame2 );
		}

		if( tContinue != true )
		{
			return SUCCESS;
		}
	}
#endif

	return SUCCESS;
}


void CRepetionModule::ObserverAssignment( unsigned int vRepetionID, unsigned int vEntityID )
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsCharacter(), return, "Entity %d, EntityType %d",
		vEntityID, tpEntity->GetEntityType( ) );

	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "ObjID %d", vRepetionID );

	( ( CEntityCharacter* )tpEntity )->RegisterObserver( tpRepetion, CCharacterObservable::TIME_ASSIGNMENT );
#endif
}

int CRepetionModule::SetAssignment( unsigned int vRepetionID, unsigned int vEntityID, int vTime,
		lua_tinker::table vTable )
{
#ifdef _FB_
	int             tOtherInfo[MAX_TIMER_PARA_NUM] = { 0 };

	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return ERR_INVALID_PARAMETER, "EntityID %d", vEntityID );

	tOtherInfo[ 0 ] = MESSAGE_PROPERTY;
	tOtherInfo[ 1 ] = TIMER_ASSIGNMENT_REPETION;
	tOtherInfo[ 2 ] = vTable.get< int >( "mapid" );
	tOtherInfo[ 3 ] = vTable.get< int >( "flag" );

	return tpEntity->GetTimerReciver()->SetTimer( tpEntity->GetEntityID( ), TIMER_ASSIGNMENT, vTime, 4, tOtherInfo );
#endif

	return 0;
}

int CRepetion::TimeAssignment( CEntityCharacter* vpCharacter, int vTimerID, int vOtherInfoLength, int* vpOtherInfo )
{
#ifdef _FB_
	REPE_ASSERT_STR( vpCharacter != NULL, return ERR_INVALID_PARAMETER, "ERR_INVALID_PARAMETER" );
	REPE_ASSERT_STR( vpOtherInfo != NULL, return ERR_INVALID_PARAMETER, "ERR_INVALID_PARAMETER" );

	if( vpOtherInfo[ 1 ] != TIMER_ASSIGNMENT_REPETION )
	{
		return ERR_INVALID_PARAMETER;
	}

	REPE_ASSERT( vOtherInfoLength != 3, return ERR_INVALID_PARAMETER );
	REPE_ASSERT_STR( vpCharacter->IsNpc(), return ERR_INVALID_PARAMETER, 
		"EntityID %d, Type %d", vpCharacter->GetEntityID( ), vpCharacter->GetEntityType( ) );

	CEntityNpc* tpNpc = static_cast<CEntityNpc*>( vpCharacter  );

	if( vpCharacter->IsAlive( ) == false )
	{
		// 以近死亡NPC无TimeAssignment作用
		CRepetionModule::GetSingletonPtr( )->ClearAssignment( get_id( ), vpCharacter->get_id( ), vTimerID );
		LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Death Npc( %d : %d )'s TimeAssignment Come", __LK_FILE__,
			__LINE__, __FUNCTION__, vpCharacter->GetEntityID( ), tpNpc->GetNpcID( ) );
		return SUCCESS;
	}

	CCreator* tpCreator = ( CCreator* )CSceneObjManager::GetSingletonPtr( )->GetObject( vpCharacter->GetCreatorID( ) );

	lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );
	vParame.set( "mapid", vpOtherInfo[ 2 ] );
	vParame.set( "flag", vpOtherInfo[ 3 ] );
	vParame.set( "timeid", vTimerID );
	vParame.set( "tempid",  tpNpc->GetNpcID( ) );
	vParame.set( "index", ( tpCreator != NULL ) ? tpCreator->GetIndex( ) : 0 );
	vParame.set( "entityid", vpCharacter->GetEntityID( ) );

	PERF_FUNC( "Repetion_DoEvent", DoEvent( GetRepetionInfo( ), REPETION_TIMEASSIGNMENT, &vParame ) );
#endif

	return SUCCESS;
}

void CRepetionModule::ClearAssignment( unsigned int vRepetionID, unsigned int vEntityID, int vTimeID )
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );
	tpEntity->GetTimerReciver()->ClearTimer( vTimeID );
#endif
}

void CRepetionModule::ClearEnmity( unsigned int vEntityID )
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsNpc(), return, "EntityID %d, EntityType %d",
		vEntityID, tpEntity->GetEntityType( ) );
		
	CEntityNpc* tpNpc = static_cast<CEntityNpc*>(tpEntity);
	CNpcPolicy* tpPolicy = ( CNpcPolicy* )CSceneObjManager::GetSingletonPtr( )->GetObject( 
		tpNpc->GetPolicyID( ) );	
	REPE_ASSERT_STR( tpPolicy != NULL, return, "Entity %d, Policy %d", vEntityID, 
		tpNpc->GetPolicyID( ) );

	tpPolicy->ResetTarget( );
#endif
}

void CRepetionModule::AddEnmity( unsigned int vSrcEntityID, unsigned int vDesEntityID, int vEnmity )
{
#ifdef _FB_
	CEntityCharacter* pSrcEntity = (CEntityCharacter*)CSceneObjManager::GetSingletonPtr()->GetObject(vSrcEntityID);
	REPE_ASSERT_STR( pSrcEntity != NULL, return, "SrcEntityID %d, DesEntityID %d", vSrcEntityID, vDesEntityID );
	CEntityCharacter* pDesEntity = (CEntityCharacter*)CSceneObjManager::GetSingletonPtr()->GetObject( vDesEntityID );
	REPE_ASSERT_STR( pDesEntity != NULL, return, "SrcEntityID %d, DesEntityID %d", vSrcEntityID, vDesEntityID );

	CNpcModule::GetSingletonPtr( )->OnAttacked( pDesEntity, pSrcEntity, vEnmity );
	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] SrcEntity( %d ) Add Enmity Into DesEntity( %d ), Value( %d )", 
		__LK_FILE__, __LINE__, __FUNCTION__, vSrcEntityID, vDesEntityID, vEnmity );

#endif
}

void CRepetionModule::ClearAssignmentAll( unsigned int vRepetionID, unsigned int vEntityID, lua_tinker::table vTable )
{
#ifdef _FB_
	int	tOtherInfo[MAX_TIMER_PARA_NUM] = { 0 };

	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );

	tOtherInfo[ 0 ] = MESSAGE_PROPERTY;
	tOtherInfo[ 1 ] = TIMER_ASSIGNMENT_REPETION;
	tOtherInfo[ 2 ] = vTable.get< int >( "mapid" );
	tOtherInfo[ 3 ] = vTable.get< int >( "flag" );

	tpEntity->GetTimerReciver()->ClearTimer( TIMER_ASSIGNMENT, 4, tOtherInfo );
#endif
}

void CRepetionModule::SetAliveRefresh( unsigned int vRepetionID, unsigned int vEntityID, int vTime, int vIndex )
{
#ifdef _FB_
	int nTimerOtherInfo[ TIMER_OTHER_INFO ] = { MESSAGE_PROPERTY, 0, 0, 0 } ;
	nTimerOtherInfo[ 1 ] = vIndex;

	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsOgre( ) == true, return, "Type %d, EntityID %d", tpEntity->GetEntityType( ),
		tpEntity->GetEntityID( ) );

	((CEntityOgre*)tpEntity)->GetTimerReciver()->SetTimer( tpEntity->get_id(), TIMER_ENTITY_ALIVE_REFRESH, 
		vTime, 1, nTimerOtherInfo );
#endif
}

void CRepetionModule::CreateHierogram( unsigned int vRepetionID, unsigned int vTempID, int vPosX, int vPosY )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );
	CTplHierogram* tpHierogram = ( CTplHierogram* )CDataStatic::SearchTpl( vTempID );
	REPE_ASSERT_STR( tpHierogram != NULL, return, "TempID %d", vTempID );
	CMapObject* tpMapObj = tpRepetion->GetMapObject( 0 );
	REPE_ASSERT_STR( tpMapObj != NULL, return, "Cannt Get Repetion MapObj" );
	CWTPoint tPoint = CWTPoint( vPosX, vPosY );

	CPropertyModule::GetSingletonPtr( )->CreateHierogram( tpHierogram, &tPoint, tpMapObj );
#endif
}

void CRepetionModule::CreateHierogramByMap( unsigned int vRepetionID, int vMapID, unsigned int vTempID, int vPosX, int vPosY )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );
	CTplHierogram* tpHierogram = ( CTplHierogram* )CDataStatic::SearchTpl( vTempID );
	REPE_ASSERT_STR( tpHierogram != NULL, return, "TempID %d", vTempID );
	CMapObject* tpMapObj = tpRepetion->GetMapObjectByMapID( vMapID );
	REPE_ASSERT_STR( tpMapObj != NULL, return, "Cannt Get Repetion MapObj %d", vMapID );
	CWTPoint tPoint = CWTPoint( vPosX, vPosY );

	CPropertyModule::GetSingletonPtr( )->CreateHierogram( tpHierogram, &tPoint, tpMapObj );
#endif
}

void CRepetionModule::Teleport( unsigned int vRepetionID, unsigned int vEntityID, int vPosX, int vPosY )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );

	CMapModule::GetSingletonPtr( )->Teleport( tpEntity->GetLineID( ), tpEntity->GetMapID( ), vPosX, vPosY, tpEntity, 
		tpEntity->GetMapIndex( ) );
#endif
}

void CRepetionModule::TeleportByMap( 
	unsigned int vRepetionID, 
	unsigned int vEntityID, 
	int vMapID, 
	int vPosX, 
	int vPosY )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );


	CMapObject* tpMapObj = tpRepetion->GetMapObjectByMapID( vMapID );	

	if( tpMapObj != NULL )
	{
		CMapModule::GetSingletonPtr( )->Teleport( 
			tpEntity->GetLineID( ), 
			tpMapObj->GetMapID( ), 
			vPosX, 
			vPosY, 
			tpEntity, 
			tpMapObj->GetMapIndex( ),
			tpRepetion->GetKey( ) 
		);
	}
#endif
}

void CRepetionModule::UseSkill2AllPlayer( unsigned int vRepetionID, unsigned int vSrcEntityID, 
		unsigned int vSkillTempID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "vRepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoop( CRepetion::UseSkill, vSrcEntityID, vSkillTempID );
#endif
}

void CRepetionModule::SetIntData2AllPlayer( unsigned int vRepetionID, unsigned int vIndex, int vIntData )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "vRepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoop( CRepetion::SetIntData, vIndex, vIntData );
#endif
	return;
}


void CRepetionModule::SetIntData2AllPlayerMax( unsigned int vRepetionID, unsigned int vIndex, int vIntData )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "vRepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoop( CRepetion::SetIntDataMax, vIndex, vIntData );
#endif
	return;
}

bool CRepetion::SetIntData( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vIndex, int vIntData )
{
#ifdef _FB_
	int tRtn = SUCCESS;
	int tData = 0;
	REPE_ASSERT( vpRepetion != NULL, return true );
	REPE_ASSERT( vpEntity != NULL, return true );
	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return false, "EntityType Is %d", vpEntity->GetEntityType( ) );
	CPropertyPlayer* tpPlayer = ( CPropertyPlayer* )vpEntity->GetProperty( );

	tpPlayer->GetRepetion( )->GetIntData( vpRepetion->GetRepetionIndex( ), vIndex, &tData );
	tRtn = tpPlayer->GetRepetion( )->SetIntData( vpRepetion->GetRepetionIndex( ), vIndex, vIntData );
	int delta = vIntData - tData ;
	if (vpRepetion->GetRepetionIndex( ) == _REPE_GLOBALWAR_INDEX_ && vIndex == 4 && delta != 0 )
	{
		LogEventPlayerRepeScore( (CEntityPlayer* )vpEntity, tData, delta, 1);
	}
	
	REPE_ASSERT_STR( tRtn == SUCCESS, return false, "Rtn %d", tRtn );
#endif
	return true;
}

bool CRepetion::SetIntDataMax( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vIndex, int vIntData )
{
#ifdef _FB_
	int tRtn = SUCCESS;

	REPE_ASSERT( vpRepetion != NULL, return true );
	REPE_ASSERT( vpEntity != NULL, return true );
	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return false, "EntityType Is %d", vpEntity->GetEntityType( ) );
	CPropertyPlayer* tpPlayer = ( CPropertyPlayer* )vpEntity->GetProperty( );

	int tData = 0;

	tRtn = tpPlayer->GetRepetion( )->GetIntData( vpRepetion->GetRepetionIndex( ), vIndex, &tData );
	REPE_ASSERT_STR( tRtn == SUCCESS, return false, "Get Rtn %d", tRtn );

	if( vIntData > tData )
	{
		tRtn = tpPlayer->GetRepetion( )->SetIntData( vpRepetion->GetRepetionIndex( ), vIndex, vIntData );
		REPE_ASSERT_STR( tRtn == SUCCESS, return false, "Rtn %d", tRtn );
	}
#endif
	return true;
}

void CRepetionModule::AddIntData2AllPlayer( unsigned int vRepetionID, unsigned int vIndex, int vIntData )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "vRepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoop( CRepetion::AddIntData, vIndex, vIntData );
#endif
	return;
}

bool CRepetion::AddIntData( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vIndex, int vIntData )
{
#ifdef _FB_
	int tRtn = SUCCESS;
	int tData = 0;

	CRepetionInfo* tpInfo = vpRepetion->GetRepetionInfo( );
	REPE_ASSERT_STR( tpInfo != NULL, return true, "Repetion %d, %lld", vpRepetion->get_id( ), vpRepetion->GetKey( ) );

	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return false, "EntityType Is %d", vpEntity->GetEntityType( ) );
	CPropertyPlayer* tpPlayer = ( CPropertyPlayer* )vpEntity->GetProperty( );

	tRtn = tpPlayer->GetRepetion( )->GetIntData( tpInfo->GetRepetionIndex( ), vIndex, &tData );
	REPE_ASSERT_STR( tRtn == SUCCESS, return false, "Get Rtn %d", tRtn );

	tRtn = tpPlayer->GetRepetion( )->SetIntData( tpInfo->GetRepetionIndex( ), vIndex, vIntData + tData );
	REPE_ASSERT_STR( tRtn == SUCCESS, return false, "Get Rtn %d", tRtn );

	if (vpRepetion->GetRepetionIndex( ) == _REPE_GLOBALWAR_INDEX_ && vIndex == 4 && vIntData != 0)
	{
		LogEventPlayerRepeScore( (CEntityPlayer* )vpEntity, tData, vIntData, 1);
	}

	tpPlayer->GetRepetion( )->GetIntData( tpInfo->GetRepetionIndex( ), vIndex, &tData ); 
	
	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Player( %s : %d ) Repetion %d Data( %d : %d )", __LK_FILE__, __LINE__,
		__FUNCTION__, ( ( CEntityPlayer* )vpEntity )->GetName( ), ( ( CEntityPlayer* )vpEntity )->GetCharID( ), 
		vpEntity->GetMapID( ), vIndex, tData );
#endif
	return true;
}

int CRepetionModule::GetRepeLevel( unsigned int vRepetionID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return 0, "RepetionID %d", vRepetionID );

	unsigned int vParam = GetParameFromKey( tpRepetion->GetKey( ) );

	return vParam;
#endif

	return 0;
}

int CRepetionModule::GetBeginTime( unsigned int vRepetionID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return 0, "RepetionID %d", vRepetionID );

	return tpRepetion->GetBeginTime( );
#endif
	return 0;
}

int CRepetionModule::TeamWaitBattle( CEntityPlayer* vpPlayer, int vBattleID, CEntityNpc* vpNpc )
{
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	REPE_ASSERT_STR( vBattleID < MAP_INDEX_MAX, return ERR_INVALID_PARAMETER, "BattleID %d, Max MapIndex %d", 
		vBattleID, MAP_INDEX_MAX );

	int tPlayerCharID[ TEAM_NUMBER ] = { 0 };
	int tRtn = SUCCESS;
	int tPlayerCount = 0;
	bool tCheck = true;

	REPE_ASSERT( vpPlayer != NULL, return  ERR_INVALID_PARAMETER );

	CRepetionInfo* tpRepetionInfo = GetRepetionInfo( vBattleID );
	REPE_ASSERT( tpRepetionInfo != NULL, return ERR_INVALID_PARAMETER );

	if( tpRepetionInfo->IsBattle( ) == false )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Map( %d ) Is Not A Repetion Map", __LK_FILE__, __LINE__,
				__FUNCTION__, vBattleID );
		return ERR_UNKNOWN;
	}

	if( NotInWaitRedirMap( vpPlayer ) == false )
	{
		return ERR_UNKNOWN;
	}

	if( tpRepetionInfo->IsTeamIn( ) == true )
	{
		//当副本需求队伍同时进入的时候
		CEntityPlayer* tpHeader = CTeamModule::GetSingletonPtr( )->GetTeamHeader( vpPlayer );
		if( tpHeader != NULL && tpHeader != vpPlayer )
		{
			SendErrCode( vpPlayer, ERR_REPETION_ISNOT_HEADER, vBattleID );
			return ERR_REPETION_ISNOT_HEADER;
		}
		else if( tpHeader == NULL )
		{
			SendErrCode( vpPlayer, ERR_REPETION_ISNOT_HEADER, vBattleID );
			return ERR_REPETION_ISNOT_HEADER;
		}
		tRtn = CTeamModule::GetSingletonPtr( )->OnGetMemberList( vpPlayer->GetCharID( ), tPlayerCharID, 
			ARRAY_CNT( tPlayerCharID ), tPlayerCount );
		if ( tRtn < 0 )
		{
			LOG_ERROR("team", "repetion get team list failed.id=%d,[%s:%d]", vpPlayer->GetCharID(), __LK_FILE__, __LINE__);
			return ERR_UNKNOWN;
		}	
	}
	else
	{
		tPlayerCharID[ 0 ] = vpPlayer->GetCharID( );
		tPlayerCount = 1;
	}	

	tCheck = RedirectRepetionCheck( vpPlayer, tPlayerCharID, tPlayerCount, tpRepetionInfo, vpNpc );
	if( tCheck != true )
	{
		return ERR_UNKNOWN;
	}	

	//TODO:	

	CMessageBattleTeamWait tWaitMsg;
	CMessage tMsg;

	tWaitMsg.set_charid( vpPlayer->GetCharID( ) );
	tWaitMsg.set_battleid( vBattleID );
	tWaitMsg.set_level( ( tpRepetionInfo->GetTeamLevelFlag( ) == REPE_LEVEL_MAX ) ? 
		vpPlayer->GetTeamMaxLevel( ) : ( ( tpRepetionInfo->GetTeamLevelFlag( ) == REPE_LEVEL_MIN ) ? 
		vpPlayer->GetTeamMinLevel( ) : vpPlayer->GetTeamLevel( ) ) );

	tMsg.mutable_msghead()->set_messageid( ID_S2G_BATTLE_TEAM_WAIT );
	tMsg.set_msgpara( (uint32)&tWaitMsg );

	CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMsg );

	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Player( %s : %d ) Team Request Wait Battle %d", __LK_FILE__, __LINE__, 
		__FUNCTION__, vpPlayer->GetName( ), vpPlayer->GetCharID( ), vBattleID );
	
	return SUCCESS;
}



void CRepetionModule::RegisterBattleCamp( int vRepetionIndex, lua_tinker::table vTable )
{
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return, "vBattleID %d", vRepetionIndex );

	tpInfo->SetBattleCamp( vTable );
}

int CRepetionInfo::SetBattleCamp( lua_tinker::table vTable )
{
	int tCampID = vTable.get< int >( "campid" );

	REPE_ASSERT_STR( tCampID <= ( int )ARRAY_CNT( mCamp ), return ERR_INVALID_PARAMETER, "CampID %d, BattleID %d", 
		tCampID, GetBeginMapID( ) );

	mCamp[ tCampID  ].mCampID = tCampID;
	mCamp[ tCampID  ].mPosX = vTable.get< int >( "posx" );
	mCamp[ tCampID  ].mPosY = vTable.get< int >( "posy" ); 

	return SUCCESS;

}

int CRepetionInfo::SetWaitListLevel( unsigned int vLevelMin, unsigned int vLevelMax )
{

	for( unsigned int i = 0; i < ARRAY_CNT( mWaitLevel ); i++ )
	{
		if( mWaitLevel[ i ].mLevelMin != 0 )
		{
			continue;
		}
		
		mWaitLevel[ i ].mLevelMin = vLevelMin;
		mWaitLevel[ i ].mLevelMax = vLevelMax;

		break;
	}

	return SUCCESS;
	
}

void CRepetionModule::RegisterBattleWaitLevel( int vRepetionIndex, lua_tinker::table vTable )
{
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return, "vBattleID %d", vRepetionIndex );

	tpInfo->SetWaitListLevel( vTable.get< unsigned int >( "levelmin" ), vTable.get< unsigned int >( "levelmax" ) );	
}

void CRepetionModule::RepetionClose( unsigned int vRepetionID )
{
#ifdef _FB_
	CRepetion* tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( vRepetionID );
	if( tpRepetion != NULL )
	{
//		CRepetionModule::GetSingletonPtr( )->DestroyRepetion( tpRepetion );
		tpRepetion->AllPlayerLeave( );		
		// 立马销毁副本
		tpRepetion->SetClear();
	}
#endif
}

void CRepetionModule::NpcMove( int vRepetionID, int vEntityID, int vPosX, int vPosY )
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );
	
	REPE_ASSERT_STR( tpEntity->IsNpc( ) == true, return, "EntityType %d", tpEntity->GetEntityType( ) );

	CEntityNpc* tpNpc = static_cast<CEntityNpc*>(tpEntity);
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	tpRepetion->PushMoveEntity( vEntityID, vPosX, vPosY );

	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] NpcEntity( %d : %d ) Will Move From Pos( %d : %d ) To Target( %d : %d"
		" )", __LK_FILE__, __LINE__, __FUNCTION__, vEntityID, tpNpc->GetNpcID( ), 
		tpEntity->GetPosX( ), tpEntity->GetPosY( ), vPosX, vPosY );

	CNpcModule::GetSingletonPtr( )->OnSleep( tpNpc, false, 1 );
	CNpcModule::GetSingletonPtr( )->OnMoveNpc( vEntityID, vPosX, vPosY );
	
#endif
}

void CRepetionModule::NpcFollow( int vRepetionID, int vEntityID, int vFollowedID, int vOffsetX, int vOffsetY )
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsNpc( ) == true, return, "EntityType %d", tpEntity->GetEntityType( ) );
	
	CEntityNpc* tpNpc = static_cast<CEntityNpc*>(tpEntity);
	CNpcModule::GetSingletonPtr( )->OnSleep( tpNpc, false );
	CNpcModule::GetSingletonPtr( )->OnFollow( tpNpc, vFollowedID, vOffsetX, vOffsetY );
#endif
}

lua_tinker::table CRepetionModule::GetCampCountEncircleEntity( 
	int vRepetionID, 
	int vEntityID, 
	int vDistance, 
	EEntityType vCampEntityType )
{
	lua_tinker::table tTable( CRepetionModule::GetSingletonPtr( )->mLua );
#ifdef _FB_
	char tKey[ 128 ] = { 0 };
	int  tCampID[ CAMP_MAX_COUNT ] = { 0 };
	int	 tCampCount = 2;
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );

	REPE_ASSERT_STR( tpEntity != NULL, return tTable, "EntityID %d", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsCharacter( ) == true, return tTable, "EntityID %d, Type %d", vEntityID, 
		tpEntity->GetEntityType( ) );

	CCampCheck tCheck( ( CEntityCharacter* )tpEntity );
	tCheck.SetDistance( vDistance );
	tCheck.SetEntityType( vCampEntityType );
	std::vector< CEntity* > tEnemys;

	SeenPlayerContainserSearcher< CCampCheck > tSearcher( tCheck, tEnemys );	

	for( int i = 1; i <= tCampCount && i <= ( int )ARRAY_CNT( tCampID ); i++ )
	{
		tCampID[ i - 1 ] = i;
		tCheck.AddCampFlag( tCampID[ i - 1 ] );
	}

	tSearcher.Visit( tpEntity );

	for( int i = 0; i < tCampCount && i <= ( int )ARRAY_CNT( tCampID ); i++ )
	{
		snprintf( tKey, sizeof( tKey ), "%d", tCampID[ i ] );
		tTable.set( tKey, tCheck.GetCampCount( tCampID[ i ] ) );
	}

#endif	
	return tTable;
}

void CRepetionModule::RegisterCommTakeMap( int vRepetionIndex, int vCommMapID )
{
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vRepetionIndex );
	tpInfo->SetCommTakeMap( vCommMapID );
}


bool CCampCheck::operator( )( CEntity* vpTarget )
{
	REPE_ASSERT( vpTarget != NULL, return false );
	if( vpTarget->IsCharacter( ) == true )
	{
		CEntityCharacter* tpCharacter = ( CEntityCharacter* )vpTarget;
		if( vpTarget->GetEntityType( ) == mEntityType )
		{
			if( mpSearchEntity->IsWithinDistance( vpTarget, mDistance ) == false )
			{
				return false;
			}
			for( unsigned int i = 0; i < ARRAY_CNT( mCount ); i++ )
			{
				if( mCount[ i ].mCampFlag == tpCharacter->GetCampID( ) )
				{
					mCount[ i ].mCampCount++;
					break;
				}
			}
		}
	}
	return false;
}

bool CCampCheck::AddCampFlag( int vCamp )
{
	for( unsigned int i = 0; i < ARRAY_CNT( mCount ); i++ )
	{
		if( mCount[ i ].mCampFlag == vCamp )
		{
			return true;
		}
		if( mCount[ i ].mCampFlag == 0 )
		{
			mCount[ i ].mCampFlag = vCamp;
			return true;
		}
	}	

	return false;
}

int CCampCheck::GetCampCount( int vCamp )
{
	for( unsigned int i = 0; i < ARRAY_CNT( mCount ); i++ )
	{
		if( mCount[ i ].mCampFlag == vCamp )
		{
			return mCount[ i ].mCampCount;
		}
	}	

	return 0;
}

lua_tinker::table CRepetionModule::GetPos( int vRepetionID, int vEntityID )
{
	lua_tinker::table vPos( CRepetionModule::GetSingletonPtr( )->mLua );
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return vPos, "EntityID %d", vEntityID );

	vPos.set< int >( "x", tpEntity->GetPosX( ) );
	vPos.set< int >( "y", tpEntity->GetPosY( ) );
#endif
	return vPos;
}

void CRepetionModule::NpcPatrol( int vRepetionID, int vEntityID, int vStart, lua_tinker::table vRoute )
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsNpc( ) == true, return, "EntityType %d, EntityID %d", tpEntity->GetEntityType( ), 
		vEntityID );

	CRepetion* tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	lua_tinker::table tT( CRepetionModule::GetSingletonPtr()->mLua );

	int tX[ 100 ] = { 0 };
	int tY[ 100 ] = { 0 };
	int tLen = vRoute.getn( );

	tX[ 0 ] = tpEntity->GetPosX( );
	tY[ 0 ] = tpEntity->GetPosY( );

	for( int i = 1; i <= tLen && i < ( int )ARRAY_CNT( tX ); i++ )
	{
		lua_tinker::table tPos = vRoute.get< lua_tinker::table >( i );
		tX[ i ] = tPos.get< int >( "x" );
		tY[ i ] = tPos.get< int >( "y" );	
	}

	tLen++;

	if( vStart >= 0 )
	{
		( ( CEntityNpc* )tpEntity )->RegisterObserver( tpRepetion, CCharacterObservable::MOVE_STOP );
	}

	CNpcModule::GetSingletonPtr( )->SetPatrol( ( CEntityNpc* )tpEntity, tX, tY, tLen, 
		( vStart > 0 ) ? vStart - 1 : vStart );
	
#endif
}

int CRepetion::MoveStop( CEntity* vpEntity )
{
#ifdef _FB_
	REPE_ASSERT( vpEntity != NULL, return ERR_INVALID_PARAMETER );
	
	int vCreatorIndex = 0;
	int vTempID	= 0;
	int vMetier = 0;

	if( vpEntity->IsNpc( ) == true )
	{
		CEntityNpc* tpNpc = static_cast<CEntityNpc*>(vpEntity);

		CCreator* tpCreator = ( CCreator* )CSceneObjManager::GetSingletonPtr( )->GetObject( tpNpc->GetCreatorID( ) );
		vCreatorIndex = ( tpCreator != NULL ) ? tpCreator->GetIndex( ) : 0;
		vTempID = tpNpc->GetNpcID( );
	}
	else if( vpEntity->IsPlayer( ) == true )
	{
		vMetier = ( ( CEntityPlayer* )vpEntity )->GetMetier( );
	}

	lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );
	vParame.set( "tempid", vTempID );
	vParame.set( "index", vCreatorIndex );
	vParame.set( "entityid", vpEntity->GetEntityID( ) );
	vParame.set( "metier",	vMetier );	
	vParame.set( "posx", vpEntity->GetPosX( ) );
	vParame.set( "posy", vpEntity->GetPosY( ) );

	PERF_FUNC( "Repetion_DoEvent", DoEvent( GetRepetionInfo( ), REPETION_ENTITY_MOVE_STOP, &vParame ) );
#endif
	return SUCCESS;
}

void CRepetionModule::SetCampID( int vRepetionID, int vEntityID, int vCampID )
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsCharacter( ) == true, return, "EntityType %d, EntityID %d", tpEntity->GetEntityType( ),
		vEntityID );

	( ( CEntityCharacter* )tpEntity )->SetCampID( vCampID ); 
#endif
}

void CRepetionModule::ChangeCampIDByTempID( int vRepetionID, int vDesTemplateID, int vCampID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return, "ERR_INVALID_PARAMETER, Repetion ID is %d", vRepetionID );

	tpRepetion->NpcEntityLoop( vDesTemplateID, CRepetion::SetCampID, vCampID, 0 );
#endif
}

bool CRepetion::SetCampID( CRepetion* vpRepetion, CEntityCharacter* tpCharacter, int vCampID, int vValue )
{
#ifdef _FB_
	REPE_ASSERT( tpCharacter != NULL, return true );
	tpCharacter->SetCampID( vCampID );
	return true;
#endif
	return true;
}

void CRepetionModule::PlayerRelive( int vRepetionID, int vCampID, int vPosX, int vPosY )
{
#ifdef _FB_
	int tPos[ 2 ] = { 0 };

	tPos[ 0 ] = vPosX;
	tPos[ 1 ] = vPosY;	

	CRepetion* tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoop( CRepetion::PlayerRelive, vCampID, ( int )tPos );
#endif
}

bool CRepetion::PlayerRelive( CRepetion* vpRepetion, CEntityCharacter* tpCharacter, int vCampID, int vPosPoint )
{
#ifdef _FB_
	REPE_ASSERT( tpCharacter != NULL, return true );
	REPE_ASSERT_STR( tpCharacter->IsPlayer( ) == true, return true, "EntityType %d, EntityID %d", 
			tpCharacter->GetEntityType( ), tpCharacter->GetEntityType( ) );

	int* tPos = ( int* )vPosPoint;
	CEntityPlayer* tpPlayer = ( CEntityPlayer* )tpCharacter;

	if( tpPlayer->GetCampID( ) == vCampID )
	{
		CRepetionModule::GetSingletonPtr( )->RelivePlayer( tpPlayer, vpRepetion, tPos[ 0 ], tPos[ 1 ] );
	}
#endif
	return true;
}

int CRepetionModule::PlayerIntoBattleByKey( 
		CEntityPlayer* vpPlayer, 
		int vBattleID, 
		int vCampID,
		KEY_TYPE vKey,  
		CEntityNpc* vpNpc )
{
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	CMessageBattlePlayerJoin tJoin;
	CMessage tMsg;

	int tPlayerListID = vpPlayer->GetCharID( );
	int tPlayerCount = 1;
	bool tBool = true;

	CRepetionInfo* tpInfo = GetRepetionInfo( vBattleID );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "vBattleID %d Isn't A BattleMap", vBattleID );

	tBool = RedirectRepetionCheck( vpPlayer, &tPlayerListID, tPlayerCount, tpInfo, vpNpc );
	if( tBool == false )
	{
		return ERR_UNKNOWN;
	}

	tJoin.set_battleid( vBattleID );
	tJoin.set_key( vKey );
	tJoin.set_charid( vpPlayer->GetCharID( ) );
	tJoin.set_campid( vCampID );
	tMsg.mutable_msghead()->set_messageid( ID_S2G_BATTLE_PLAYER_JOIN );
	tMsg.set_msgpara( (uint32)&tJoin );

	CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMsg );	

//	CMapModule::GetSingletonPtr( )->Teleport( vpPlayer->GetLineID( ), vpPlayer->GetMapID( ), 0, 0, vpPlayer, 
//		vpPlayer->GetMapIndex( ) );
		
	return SUCCESS;
}

int CRepetionModule::CreateBattle( int vBattleID, int vLevel )
{
	CMessageBattleCreateRequest tMsgCreate;
	CMessage tMsg;

	tMsgCreate.set_battleid( vBattleID );
	tMsgCreate.set_level( vLevel );
	
	tMsg.mutable_msghead()->set_messageid( ID_S2G_BATTLE_CREATE_REQUEST );
	tMsg.set_msgpara( (uint32)&tMsgCreate );

	CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMsg );

	return SUCCESS;
}

int CRepetion::GetTime( unsigned int vTimeIndex )
{
#ifdef _FB_
	REPE_ASSERT( vTimeIndex < mTimeCount, return 0 );

	if( mTimer[ vTimeIndex ].mIsBegin == true )
	{
//		return mTimer[ vTimeIndex ].mTime;
		CTimerItem* tpTimer = CTimerMng::GetSingletonPtr( )->GetTimer( mTimer[ vTimeIndex ].mTimeID );
		REPE_ASSERT_STR( tpTimer != NULL, return 0, "RepetionID %d, Index %d, TimeID %d", get_id( ),
			vTimeIndex, mTimer[ vTimeIndex ].mTimeID );
		return tpTimer->mMillSeconds;
	}
#endif 
	return 0;
}

int CRepetionModule::GetTime( int vRepetionID, unsigned int vTimeIndex )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return 0, "ERR_INVALID_PARAMETER, Repetion ID is %d", vRepetionID );

	return tpRepetion->GetTime( vTimeIndex - 1 );
#endif
	return SUCCESS;
}

void CRepetion::ChangeTime( unsigned int vTimeIndex, int vTime )
{
#ifdef _FB_
	REPE_ASSERT_STR( vTimeIndex < mTimeCount && vTimeIndex < ARRAY_CNT( mTimer ), return,
		"vTimeIndex %d Out, TimeCount %d TimeIndexMax %d", vTimeIndex, mTimeCount, ARRAY_CNT( mTimer ) );

	if( mTimer[ vTimeIndex ].mIsBegin == true )
	{
		CTimerItem* tpTimer = CTimerMng::GetSingletonPtr( )->GetTimer( mTimer[ vTimeIndex ].mTimeID );
		REPE_ASSERT_STR( tpTimer != NULL, return, "RepetionID %d, TimeIndex %d, TimeID %d Cannt Find )", get_id( ),
			vTimeIndex,  mTimer[ vTimeIndex ].mTimeID );

		tpTimer->mMillSeconds = vTime;

		mTimer[ vTimeIndex ].mTime = vTime;

		if( mTimer[ vTimeIndex ].mEvent.mSync == SYNC_ALL )
		{
			CRepetionModule::EventNoticeAll( get_id( ), REPETION_COUNTDOWN, mTimer[ vTimeIndex ].mEvent.mSyncKey, 
				 vTime );
		}
		else if( mTimer[ vTimeIndex ].mEvent.mSync == SYNC_CAMP )
		{
			CRepetionModule::EventNoticeByCamp( get_id( ), mTimer[ vTimeIndex ].mEvent.mCamp, REPETION_COUNTDOWN, 
				mTimer[ vTimeIndex ].mEvent.mSyncKey, vTime );
		}
	}
	else
	{
		LOG_ERROR( REPETION_LOG, "[%s : %d ][ %s ] Repetion %d TimeIndex %d Is Not Begin", __LK_FILE__, __LINE__,
			__FUNCTION__, get_id( ), vTimeIndex  );
	}
#endif
}

void CRepetionModule::ChangeTime( int vRepetionID, unsigned int vTimeIndex, int vTime )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return, "ERR_INVALID_PARAMETER, Repetion ID is %d", vRepetionID );

	tpRepetion->ChangeTime( vTimeIndex - 1, vTime );
#endif
}

int CRepetionModule::GetCampID( int vRepetionID, int vEntityID )
{
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return 0, "EntityID %d", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsCharacter( ) == true, return 0, "EntityID %d, Type %d", vEntityID, 
		tpEntity->GetEntityType( ) );

	return ( ( CEntityCharacter* )tpEntity )->GetCampID( );
}

void CRepetionModule::EventNoticeByCamp( 
	unsigned int vRepetionID, 
	int vCampID, 
	REPETION_EVENT_NOTICE vEvent, 
	int vParame1, 
	int vParame2,
	const char *pParam3, 
	int vParame4 )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );	
	CRepetionInfo* tpInfo = tpRepetion->GetRepetionInfo( );
	REPE_ASSERT_STR( tpInfo != NULL, return, "RepetionID %d", vRepetionID );

	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Send CMessageRepetionEventNotic To Camp %d Player In Map( %d : %d : "
		"%lld), EventID : %d , Parame1 : %d, Parame2 : %d", __LK_FILE__, __LINE__, __FUNCTION__, vCampID, 
		tpInfo->GetBeginMapID( ), tpRepetion->get_id( ), tpRepetion->GetKey( ), vEvent, vParame1, vParame2 );

	int tParame[ 2 ] = { 0 };
	tParame[ 0 ] = vParame1;
	tParame[ 1 ] = vParame2;

	CMessage tMsg;
	CMessageRepetionEventNotic tMsgFact;


	tMsgFact.set_mapid( tpInfo->GetBeginMapID( ) );
	tMsgFact.set_eventid( vEvent );
	tMsgFact.set_parame1( vParame1 );
	tMsgFact.set_parame2( vParame2 );
	if ( pParam3 != NULL )
	{
		tMsgFact.set_parame3( pParam3 );
	}	
	tMsg.mutable_msghead()->set_messageid( ID_S2C_REPETION_EVENT );
	tMsg.set_msgpara( (uint32)&tMsgFact );

	tpRepetion->PlayerEntityLoop( CRepetion::NoticeEventByCamp, vCampID, ( int )&tMsg );
#endif
}

void CRepetionModule::SendBattleMsgByCamp( int nRepetionID, int nCampID, CMessage *pMsg )
{
#ifdef _FB_	
	CRepetion* tpRepetion = GetRepetion( nRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", nRepetionID );	
	CRepetionInfo* tpInfo = tpRepetion->GetRepetionInfo( );
	REPE_ASSERT_STR( tpInfo != NULL, return, "RepetionID %d",  nRepetionID );
	tpRepetion->PlayerEntityLoop( CRepetion::NoticeEventByCamp, nCampID, ( int )pMsg );
#endif
}
bool CRepetion::NoticeEventByCamp( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vCampID, int vMsg )
{
	REPE_ASSERT( vpEntity != NULL, return true );
	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return false, "EntityType Is %d", vpEntity->GetEntityType( ) );
	CMessage* tpMsg = ( CMessage* )vMsg;

	if( vpEntity->GetCampID( ) == vCampID )
	{
		CSceneLogic::GetSingletonPtr( )->Send2Player( ( CEntityPlayer* )vpEntity, tpMsg );
	}
	return true;
}


bool CRepetionModule::IsBattleMap( int vMapID )
{
	CRepetionInfo* tpRepetionInfo = GetRepetionInfoByMapID( vMapID );
	if( tpRepetionInfo == false )
	{
		return false;
	}

	return tpRepetionInfo->IsBattle( );
}

void CRepetionModule::SendBattleEndNotifyByFamily( int vRepetionID, int vWinSide )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID ); 
	REPE_ASSERT_STR( tpRepetion != NULL, return, "Repetion %d", vRepetionID );

	CFamilyModule::GetSingletonPtr( )->SendBattleEndNotify( tpRepetion->GetKey( ), vWinSide );
#endif
	return;
}

void CRepetionModule::ChangeMode( int vRepetionID, int vEntityID )
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* ) CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsBlock( ) == true, return, "EntityID %d, EntityType %d", vEntityID, 
		tpEntity->GetEntityType( ) )

	( ( CEntityBlock* )tpEntity )->ChangeMode( );
#endif
}

void CRepetionModule::ChangeCampFlagByCorps( int vRepetionID, int vEntityID, int vCampID, int bAll /* = 0 */ )
{
#ifdef _FB_
	
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );	

	CMessage tMessage;
	CMessageCorpsBattleFlagRequest tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_BATTLEFLAG );
	tMessage.set_msgpara( (long) &tPara );

	
	tPara.set_campid( vCampID );
	tPara.set_battleid( vRepetionID );
	tPara.set_battlekey( tpRepetion->GetKey() );
	if ( bAll == 0 )
	{
		CEntityPlayer* tpPlayer = ( CEntityPlayer* )CSceneLogic::GetSingleton().GetEntity( vEntityID );
		if ( tpPlayer == NULL )
		{
			return ;
		}

		tPara.set_charid( tpPlayer->GetCharID() );
		tPara.set_isall( (int) false );
	}
	else
	{
		tPara.set_isall( (int) true ); 
	}
	
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
	return ;	 
#endif
}

void CRepetionModule::BattleResultNoticeByCorps( int vRepetionID, int vWinCamp )
{
#ifdef _FB_
	CMessage tMessage;
	CMessageNotifyBattleResult2Gate tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_BATTLERESULT );
	tMessage.set_msgpara( (long) &tPara );

	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT( tpRepetion != NULL, return );

	CRepetionInfo* tpInfo = tpRepetion->GetRepetionInfo( );

	REPE_ASSERT( tpInfo != NULL, return );

	tPara.set_battleid( tpInfo->GetBeginMapID() );
	tPara.set_wincamp( vWinCamp );
	tPara.set_battlekey( tpRepetion->GetKey() );

	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
#endif
}

void CRepetionModule::SendBattleMapNoticeByCorps( int vRepetionID, int vEntityID )
{
#ifdef _FB_
	CMessage tMessage;
	CMessageCorpsBattleMapNotice tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_BATTLEMAP );
	tMessage.set_msgpara( (long) &tPara );

	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT( tpRepetion != NULL, return );

	CRepetionInfo* tpInfo = tpRepetion->GetRepetionInfo( );

	REPE_ASSERT( tpInfo != NULL, return );

	CEntityPlayer* tpPlayer = (CEntityPlayer*) CSceneLogic::GetSingleton().GetEntity( vEntityID );
	if ( tpPlayer == NULL )
	{
		return ;
	}
	
	tPara.set_roleid( tpPlayer->GetCharID() );
	tPara.set_battlekey( tpRepetion->GetKey() );

	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
#endif
}


int CRepetionModule::GetParame( int vRepetionID, int vIndex )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return ERR_INVALID_PARAMETER, "RepetionID %d", vRepetionID );

	return tpRepetion->GetParame( vIndex );
#endif
	return 0;
}


int CRepetionModule::RelivePlayer( CEntityPlayer* vpPlayer, CRepetion* vpRepetion, int vPosX, int vPosY, int vMapID )
{
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	CPropertyPlayer* tpProperty = ( CPropertyPlayer* )vpPlayer->GetProperty( );

	if( vpPlayer->IsAlive( ) == true )
	{
		return SUCCESS;	
	}

	vpPlayer->SetAliveStatus( ALIVE_NORMAL );	

	tpProperty->Refresh( );
	vpPlayer->NotifyPropertyRefresh( );

	//tpProperty->SetCurAP( 100 );
	tpProperty->SetCurHP( tpProperty->GetMaxHP( ) );
	tpProperty->SetCurMP( tpProperty->GetMaxMP( ) );

	CMessage tPlayerRelife;
	CMessagePlayerReliveNotice tMessage;
	bool tBool = CPropertyModule::GetSingletonPtr( )->CreateMessagePlayerReliveNotice( 
			&tPlayerRelife, 
			&tMessage, 
			vpPlayer->GetEntityID( ),
			vpPlayer->GetPosX( ),
			vpPlayer->GetPosY( ),
			tpProperty->GetCurHP( ),
			tpProperty->GetCurMP( ),
			tpProperty->GetMaxHP( ),
			tpProperty->GetMaxMP( )
			);
	if ( tBool == true )
	{
		CMapModule::GetSingletonPtr()->MessageBroadCast( vpPlayer, &tPlayerRelife, false, true );
	}	

	if( vpRepetion == NULL || vMapID == 0 || vMapID == vpPlayer->GetMapID( ) )
	{
		CMapModule::GetSingleton().Teleport( vpPlayer->GetLineID( ), vpPlayer->GetMapID( ), vPosX, vPosY, vpPlayer, 
			vpPlayer->GetMapIndex( ) );
	}
	else
	{
		CMapObject* tpObj = vpRepetion->GetMapObject( vMapID );
		if( tpObj != NULL )
		{
			CMapModule::GetSingleton().Teleport( vpPlayer->GetLineID( ), tpObj->GetMapID( ), vPosX, vPosY, vpPlayer,
				tpObj->GetMapIndex( ) );
		}
	}

	return SUCCESS;
}

bool CRepetion::AllPlayerRelive( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vParame1, int vParame2 )
{
	REPE_ASSERT( vpEntity != NULL, return true );
	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return true, "EntityType %d", vpEntity->GetEntityType( ) );

	CRepetionModule::GetSingletonPtr( )->RelivePlayer( ( CEntityPlayer* )vpEntity, vpRepetion, vpEntity->GetPosX( ), 
		vpEntity->GetPosY( ) );
	
	return true;
}

int CRepetion::PlayerJoinRepetion( CEntityPlayer* vpPlayer,bool vFirst )
{
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	CPlayerRepetion* tpPlayerRepetion = CRepetionModule::GetSingletonPtr( )->GetPlayerRepetion( vpPlayer );
	REPE_ASSERT( tpPlayerRepetion != NULL, return ERR_INVALID_PARAMETER );


	CMapObject* tpMapObj = ( CMapObject* )CMapObjManager::GetSingletonPtr( )->GetMapObject( vpPlayer->GetLineID( ), 
			vpPlayer->GetMapID( ), vpPlayer->GetMapIndex( ) );
	REPE_ASSERT_STR( tpMapObj != NULL, return ERR_INVALID_PARAMETER, 
			"GetMapObject( %d, %d, %d )  Return Value Is NULL", 
			vpPlayer->GetLineID( ), vpPlayer->GetMapID( ), vpPlayer->GetMapIndex( ) );

	//		tpPlayerRepetion->AddRepetionCount( tpMapObj->GetMapID( ) );


	CRepetionInfo* tpRepetionInfo = GetRepetionInfo( );
	REPE_ASSERT( tpRepetionInfo != NULL, return ERR_INVALID_PARAMETER );

	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ]Player( %d : %s ) Into Repetion( %d ), Map( %d : %d :  %d :%lld )"
			" All Count %d : %d",
			__LK_FILE__, __LINE__, __FUNCTION__, vpPlayer->GetCharID( ), vpPlayer->GetCharNameShow( ), 
			get_id( ), tpMapObj->GetLineID( ), tpMapObj->GetMapID( ), tpMapObj->GetMapIndex( ), 
			tpMapObj->GetKey( ), tpMapObj->GetPlayerCount( ), tpMapObj->GetPlayerWaitInCount( )  );

	//应张宏亮强烈要求修改
	if( CRepetionModule::GetSingletonPtr( )->IsBattleMap( vpPlayer->GetMapID( ) ) == true )
	{
		vpPlayer->SetUnHandle( UN_HANDLE_DROP_FOE_DIE );
		vpPlayer->SetUnHandle( UN_HANDLE_PROCESS_FOE_DIE );
		vpPlayer->SetUnHandle( UN_HANDLE_PROCESS_FOR_ATTACK );
		vpPlayer->SetUnHandle( UN_HANDLE_HONOR );
		vpPlayer->SetUnHandle( UN_HANDLE_DURA );
	}
	else
	{
		vpPlayer->SetUnHandle( UN_HANDLE_DROP_FOE_DIE );
		vpPlayer->SetUnHandle( UN_HANDLE_PROCESS_FOE_DIE );
		vpPlayer->SetUnHandle( UN_HANDLE_PROCESS_FOR_ATTACK );
		vpPlayer->SetUnHandle( UN_HANDLE_HONOR );
		vpPlayer->SetUnHandle( UN_HANDLE_DURA );
	}


	CRepetionModule::GetSingletonPtr( )->NotifyRepetionIndex( vpPlayer, tpRepetionInfo->GetRepetionIndex( ) );


	lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );
	vParame.set( "entityid", vpPlayer->GetEntityID( ) );
	vParame.set( "campid", vpPlayer->GetCampID( ) ); 
	vParame.set( "corpsid", vpPlayer->GetCorpsID() );
	vParame.set( "charid", vpPlayer->GetCharID( ) );
	vParame.set( "new", ( tpPlayerRepetion->GetOldRepeKey( ) != GetKey( ) ) ? 1 : 0 ); 
	vParame.set( "mapid", tpMapObj->GetMapID( ) );
	vParame.set( "sex", vpPlayer->GetSex( ) );
	vParame.set( "first" , vFirst == true? 1 : 0 );
	
	bool vQuestionCheck = false;
	if( tpPlayerRepetion->GetOldRepeKey( ) != GetKey( ) )
	{
		// 进入一个新副本， 次数增加
		//tpPlayerRepetion->IncRepetionCount( tpRepetionInfo->GetRepetionIndex( ) );
		AddPlayerJoinTimes( );
		tpPlayerRepetion->ClearQuestionVerifyInfo();
		vQuestionCheck = true;
	}	

	tpPlayerRepetion->SetOldRepeKey( GetKey( ) );

	//		PERF_FUNC( "Rpetion_DoEvent", tpRepetion->DoEvent( tpRepetionInfo, REPETION_CREATE_PLAYER_EVENT, pEntity->GetEntityID( ) ) );
	PERF_FUNC( "Rpetion_DoEvent", DoEvent( tpRepetionInfo, REPETION_CREATE_PLAYER_EVENT, &vParame ) );

	tpPlayerRepetion->SetInRepetion( );
	tpPlayerRepetion->SetInRepetionIndex( tpRepetionInfo->GetRepetionIndex( ) );
	
	for( unsigned int i = 0; i < mTimeCount && i < ARRAY_CNT( mTimer ); i++ )
	{
		if( mTimer[ i ].mIsBegin == true )
		{
			CTimerItem* tpItem = CTimerMng::GetSingletonPtr( )->GetTimer( mTimer[ i ].mTimeID );
			REPE_ASSERT_STR( tpItem != NULL, continue, "RepetionID %d, Index %d, TimeID %d", get_id( ), i, 
				mTimer[ i ].mTimeID );
			if( mTimer[ i ].mEvent.mSync == SYNC_ALL )
			{
				CRepetionModule::EventNoticeSingle( get_id( ), vpPlayer->GetEntityID( ), REPETION_COUNTDOWN, 
						mTimer[ i ].mEvent.mSyncKey, tpItem->mMillSeconds );
			}
			else if( mTimer[ i ].mEvent.mSync == SYNC_CAMP )
			{
				if( mTimer[ i ].mEvent.mCamp == vpPlayer->GetCampID( ) )
				{
					CRepetionModule::EventNoticeSingle( get_id( ), vpPlayer->GetEntityID( ), REPETION_COUNTDOWN, 
							mTimer[ i ].mEvent.mSyncKey, tpItem->mMillSeconds );
				}
			}
		}
	}
	
	// 改为进入副本前扣除钥匙
	bool tUseItem = false;
	if ( GetRemoveItem() > 0 )
	{
		tUseItem = true;
		SetRemoveItem( 0 );
	}
	
	if ( tUseItem == true && RemoveSpecialItem( vpPlayer ) == SUCCESS )
	{
		tpPlayerRepetion->SetCheckResult( true );	
	}		

	// 10秒后副本答题验证	
	if ( vQuestionCheck == true  && tpPlayerRepetion->GetCheckResult() == false )
	{
		int tOtherInfo[2] = { 0 };
		tOtherInfo[0] = MESSAGE_REPETION;
		tOtherInfo[1] = GetRepetionIndex();

		int tTimeLength = CSceneCfgManager::GetSingleton().GetRepetion_cfg()->mRepetionverifytime;
		CTimerMng::GetSingleton().SetTimer( vpPlayer->GetEntityID(), TIMER_REPTIONVERIFYTIME, tTimeLength*1000, 2, tOtherInfo );	 			
	}	
	return SUCCESS;
}


void CRepetionModule::InsertItem( int vRepetionID, int vEntityID, int vItemID, int vCount, lua_tinker::table vParame )
{
#ifdef _FB_
	//char tCharParame[ 128 ] = { 0 };

	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );
	
	CRepetionInfo* tpRepetionInfo = tpRepetion->GetRepetionInfo( );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d", vEntityID );
	
	REPE_ASSERT_STR( tpEntity->IsPlayer( ) == true, return, "EntityType %d, EntityID %d",
		tpEntity->GetEntityType( ), vEntityID );
	
	CEntityPlayer* tpPlayer = ( CEntityPlayer* )tpEntity;

	//snprintf( tCharParame, sizeof( tCharParame ), "Key %lld", tpRepetion->GetKey( ) );

	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] Player( %d : %d ) Insert Item( %d %d ) From Repetion )", __LK_FILE__, 
		__LINE__, __FUNCTION__, tpPlayer->GetCharID( ), tpPlayer->GetEntityID( ), vItemID, vCount );

	CItemObject * pInsItemPile = 
		CPropertyModule::GetSingletonPtr( )->PlayerInsertItem(( CEntityPlayer* )tpEntity, vItemID, vCount );

	if (pInsItemPile != NULL)
	{
		LogEventGetItemByRepetion(( CEntityPlayer* )tpEntity, pInsItemPile->GetGUID(), vItemID, vCount,
			tpRepetionInfo->GetBeginMapID( ), vParame.get<int>( "reason" ), tpRepetion->GetKey( ));
	}
#endif
	return;	
}

void CRepetionModule::InsertItemByCamp( int vRepetionID, int vCampID, int vItemID, int vCount, 
		lua_tinker::table vParame )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	RepetionInsertItem tInsertItem;

	tInsertItem.mItemID = vItemID;
	tInsertItem.mCount = vCount;
	tInsertItem.mReason = vParame.get<int>("reason");	

	tpRepetion->PlayerEntityLoop( CRepetion::InsertItemByCamp, vCampID, ( int )(&tInsertItem) );
#endif
	return;
}

bool CRepetion::InsertItemByCamp( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vCampID, int vInsertItemPtr )
{
#ifdef _FB_
	//char tCharParame[ 128 ] = { 0 };

	RepetionInsertItem* tpInsertItem = ( RepetionInsertItem* )vInsertItemPtr;
	REPE_ASSERT( vpEntity != NULL, return true );

	if( vpEntity->GetCampID( ) != vCampID )
	{
		return true;
	}

	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return true, "EntityID %d, EntityType %d", vpEntity->GetEntityID( ),
		vpEntity->GetEntityType( ) );

	CEntityPlayer* tpPlayer = ( CEntityPlayer* )vpEntity;

	CRepetionInfo* tpInfo = vpRepetion->GetRepetionInfo( );

	REPE_ASSERT_STR( tpInfo != NULL, return true, "RepetionID %d", vpRepetion->get_id( ) );

	//snprintf( tCharParame, sizeof( tCharParame ), "Key %lld", vpRepetion->GetKey( ) );

	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] Player( %d : %d ) Insert Item( %d %d ) From Repetion )", __LK_FILE__, 
		__LINE__, __FUNCTION__, tpPlayer->GetCharID( ), tpPlayer->GetEntityID( ), tpInsertItem->mItemID, 
		tpInsertItem->mCount );

	CItemObject * pInsItemPile = 
		CPropertyModule::GetSingletonPtr( )->PlayerInsertItem( tpPlayer, tpInsertItem->mItemID, tpInsertItem->mCount);

	if (pInsItemPile != NULL)
	{
		LogEventGetItemByRepetion(tpPlayer, pInsItemPile->GetGUID(), tpInsertItem->mItemID, tpInsertItem->mCount,
			tpInfo->GetBeginMapID( ), tpInsertItem->mReason, vpRepetion->GetKey( ));
	}

#endif
	return true;
}

void CRepetionModule::InsertItemAll( int vRepetionID, int vItemID, int vCount, lua_tinker::table vParame )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	RepetionInsertItem tInsertItem;

	tInsertItem.mItemID = vItemID;
	tInsertItem.mCount = vCount;
	tInsertItem.mReason = vParame.get<int>("reason");	

	tpRepetion->PlayerEntityLoop( CRepetion::InsertItemAll, 0, ( int )(&tInsertItem) );
#endif
	return;
}

bool CRepetion::InsertItemAll( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vParame1, int vInsertItemPtr )
{
#ifdef _FB_
	//char tCharParame[ 128 ] = { 0 };

	RepetionInsertItem* tpInsertItem = ( RepetionInsertItem* )vInsertItemPtr;
	REPE_ASSERT( vpEntity != NULL, return true );

	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return true, "EntityID %d, EntityType %d", vpEntity->GetEntityID( ),
		vpEntity->GetEntityType( ) );

	CEntityPlayer* tpPlayer = ( CEntityPlayer* )vpEntity;

	CRepetionInfo* tpInfo = vpRepetion->GetRepetionInfo( );

	REPE_ASSERT_STR( tpInfo != NULL, return true, "RepetionID %d", vpRepetion->get_id( ) );

	//snprintf( tCharParame, sizeof( tCharParame ), "Key %lld", vpRepetion->GetKey( ) );

	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] Player( %d : %d ) Insert Item( %d %d ) From Repetion )", __LK_FILE__, 
		__LINE__, __FUNCTION__, tpPlayer->GetCharID( ), tpPlayer->GetEntityID( ), tpInsertItem->mItemID, 
		tpInsertItem->mCount );

	CItemObject * pInsItemPile = 
		CPropertyModule::GetSingletonPtr( )->PlayerInsertItem( tpPlayer, tpInsertItem->mItemID, tpInsertItem->mCount);
	
	if ( pInsItemPile != NULL)
	{
		LogEventGetItemByRepetion(tpPlayer, pInsItemPile->GetGUID(), tpInsertItem->mItemID, tpInsertItem->mCount,
			tpInfo->GetBeginMapID( ), tpInsertItem->mReason, vpRepetion->GetKey( ) );
	}
#endif
	return true;
}

void CRepetionModule::OnTimeoutMessage( CMessage* vpMsg )
{
#ifdef _FB_
	CTimerItem* pTimerItem = (CTimerItem*) vpMsg->msgpara();
	CRepetion* tpRepetion = GetRepetion( pTimerItem->mOwnerID );
	if( tpRepetion == NULL )
	{
		LOG_ERROR( REPETION_LOG, "[%s:%d] TimeID %d, RepetionID %d", __FUNCTION__, __LINE__,
			pTimerItem->get_id( ), pTimerItem->mOwnerID );
		CTimerMng::GetSingletonPtr()->ClearTimer( pTimerItem->get_id() );
		return;
	}
	
	if( pTimerItem->mTimerMark == TIMER_REPETION )
	{
		tpRepetion->OnTimeout( pTimerItem );	
	}
	else if ( pTimerItem->mTimerMark == TIMER_REPETIONQUESTION )
	{		
		CEntityPlayer *tpPlayer = ( CEntityPlayer * )CSceneLogic::GetSingleton().GetEntity( pTimerItem->mOwnerID );
		if ( tpPlayer != NULL )
		{
			CPlayerRepetion* tpPlayerRepetion = CRepetionModule::GetSingletonPtr( )->GetPlayerRepetion( tpPlayer );
			LK_ASSERT( tpPlayerRepetion != NULL, return )		
				tpPlayerRepetion->SetAnswerCount( tpPlayerRepetion->GetAnswerCount() + 1 );
			int tTimeLength = CSceneCfgManager::GetSingleton().GetRepetion_cfg()->mQuestionanswertime;
			CActivityModule::GetSingleton().NotifyQuestionVerify( tpPlayer, VERIFYKIND_REPETION, VERIFYTYPE_TIMEOUT|VERIFYTYPE_OFFLINE, tTimeLength );													
		}		
		CTimerMng::GetSingleton().ClearTimer( pTimerItem->get_id() );
	}
	
	else if ( pTimerItem->mTimerMark == TIMER_KICKOUTBYVERIFY )
	{
		CEntityPlayer *tpPlayer = ( CEntityPlayer * )CSceneLogic::GetSingleton().GetEntity( pTimerItem->mOwnerID );
		if ( tpPlayer != NULL )
		{
			PlayerLeaveRepetion( tpPlayer );
		}
		CTimerMng::GetSingleton().ClearTimer( pTimerItem->get_id() );
	}
	else if ( pTimerItem->mTimerMark == TIMER_REPTIONVERIFYTIME )
	{
		CEntityPlayer *tpPlayer = ( CEntityPlayer * )CSceneLogic::GetSingleton().GetEntity( pTimerItem->mOwnerID );
		if ( tpPlayer != NULL )
		{	
			int tRepetionIndex = pTimerItem->mOtherInfo[1];
			CRepetionInfo *tpRepetionInfo  = GetRepetionInfo( tRepetionIndex );
			if ( tpRepetionInfo != NULL )
			{
				CheckRepetionQuestionVerify( tpRepetionInfo, tpPlayer );
			}				
		}
		CTimerMng::GetSingleton().ClearTimer( pTimerItem->get_id() );
	}
		
	else if( pTimerItem->mTimerMark == TIMER_REPETION_CLEAR )
	{
		tpRepetion->ClearTimeout( );
	}
	else if ( pTimerItem->mTimerMark == TIMER_CAMPKICKVOTETIME )
	{
		EndVoteToCampKick( pTimerItem->mOwnerID );
		CTimerMng::GetSingleton().ClearTimer( pTimerItem->get_id() );
	}
	
	else
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] UnKnown Time( %d : %d ), Repetion( %d : %d : %lld )", __LK_FILE__,
			__LINE__, __FUNCTION__, pTimerItem->get_id( ), pTimerItem->mTimerMark, tpRepetion->get_id( ), 
			tpRepetion->GetRepetionIndex( ), tpRepetion->GetKey( ) );
	}
#endif
}

void CRepetionModule::OnPlayerLeave( CEntityPlayer* pPlayer, int nState )
{
#ifdef _FB_
	LK_ASSERT( pPlayer != NULL, return )

	// 玩家离开游戏
	CRepetion* tpRepetion = GetRepetion( pPlayer );
	if ( tpRepetion == NULL )
	{
		return;
	}
	lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );
	vParame.set( "entityid", pPlayer->get_id() );
	vParame.set( "playerlevel",pPlayer->GetLevel() );
	vParame.set( "playername", pPlayer->GetName() );
	vParame.set( "playercountryid",pPlayer->GetCountryID() );
	vParame.set( "playerlevel",pPlayer->GetLevel() );
	vParame.set( "playermapid",pPlayer->GetMapID());
	PERF_FUNC( "Repetion_DoEvent", tpRepetion->DoEvent( tpRepetion->GetRepetionInfo( ), REPETION_LEAVE_EVENT, &vParame ) );									
#endif
}

void CRepetion::OnTimeout( CTimerItem* vpTimer )
{
#ifdef _FB_
	LK_ASSERT( vpTimer != NULL, return )  	

	REPE_ASSERT_STR( vpTimer != NULL, return, "RepetionID %d", get_id( ) );	
	int tIndex = vpTimer->mOtherInfo[ 1 ];
	REPE_ASSERT_STR( tIndex >= 0 && tIndex < ( int )ARRAY_CNT( mTimer ), return, "RepetionID %d, TimerID %d, tIndex %d",
			get_id( ), vpTimer->get_id( ), tIndex );

	REPE_ASSERT_STR( mTimer[ tIndex ].IsActivityTimer( vpTimer ) == true, return, "RepetionID %d, TimerID %d, "
			"tIndex %d, mTimer.mTimeID %d", get_id( ), vpTimer->get_id( ), tIndex, mTimer[ tIndex ].mTimeID ); 

	if( mTimer[ tIndex ].mEvent.mIsCallFunc == true )
	{
		TimeEvent( tIndex );	
	}

	switch( mTimer[ tIndex ].mEvent.mResult )
	{
		// Close Repetion Event
		case CLOSE:
			{
				AllPlayerLeave( );
				SetClear( );
				return;
			}
			break;
		default:
			break;
	}

	// call Lua TimeEvent


	vpTimer->mCalledNum = 5;			// 循环次数由Repetion来控制， BS Timer的次数控制	
	vpTimer->mDurationSeconds = mTimer[ tIndex ].mEvent.mInitTime;		// 循环间隔有可能会被修改

	mTimer[ tIndex ].mLoopCount++;
	if( mTimer[ tIndex ].mLoopCount <= mTimer[ tIndex ].mEvent.mLoopCountMax )
	{
		mTimer[ tIndex ].mTime = mTimer[ tIndex ].mEvent.mInitTime;
		if( mTimer[ tIndex ].mLoopCount >= mTimer[ tIndex ].mEvent.mLoopCountMax )
		{
			TimeEndByIndex( tIndex );
		}
	}
	else
	{
		mTimer[ tIndex ].mTime = mTimer[ tIndex ].mEvent.mInitTime;

		if( mTimer[ tIndex ].mEvent.mSync == SYNC_ALL )
		{
			CRepetionModule::EventNoticeAll( get_id( ), REPETION_COUNTDOWN, mTimer[ tIndex ].mEvent.mSyncKey, 
					mTimer[ tIndex ].mTime );
		}
		else if( mTimer[ tIndex ].mEvent.mSync == SYNC_CAMP )
		{
			CRepetionModule::EventNoticeByCamp( get_id( ), mTimer[ tIndex ].mEvent.mCamp, REPETION_COUNTDOWN, 
					mTimer[ tIndex ].mEvent.mSyncKey, 0 );
		}
	}
#endif
}

void CRepetion::ClearAllTimer( )
{
	unsigned int i = 0;
	for( i = 0; i < ARRAY_CNT( mTimer ); i++ )
	{	
		mTimer[ i ].ClearTimer( );
	}
}

CRepetionInfo* CRepetionModule::GetRepetionInfo( int vRepetionIndex )
{
	REPE_ASSERT_STR( vRepetionIndex > 0, return NULL, "vRepetionIndex %d", vRepetionIndex );
	REPE_ASSERT_STR( vRepetionIndex < ( int )ARRAY_CNT( mRepetionInfo ), return NULL, "vRepetionIndex %d", 
		vRepetionIndex );
	return &mRepetionInfo[ vRepetionIndex ];
}

bool CRepetionModule::IsRepetionMap( int vMapID )
{
	unsigned int i = 0;
	for( i = 0; i < ARRAY_CNT( mRepetionInfo ); i++ )
	{
		if( mRepetionInfo[ i ].GetRepetionIndex( ) != INVALID_REPETION_INDEX )
		{
			if( mRepetionInfo[ i ].MapIsInRepetion( vMapID ) == true )
			{
				return true;
			}	
		}
	}

	return false;
}


// 创建副本
CRepetion* CRepetionModule::CreateRepetion( int vRepetionIndex, int vLineID, KEY_TYPE vKey, int vCreator, 
	int vRemoveItem, int vParam1, int vParam2 )
{
	CRepetion* tpRepetion = NULL;
#ifdef _FB_
	CMapObject* tpMapObj[ REPE_MAP_COUNT_MAX ] = { NULL };
	int i = 0;

	CRepetionInfo* tpInfo = GetRepetionInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return NULL, "vRepetionIndex %d", vRepetionIndex );
	REPE_ASSERT_STR( tpInfo->GetRepetionIndex( ) != INVALID_REPETION_INDEX, return NULL, "vRepetionIndex %d",
		vRepetionIndex );
	
	for( i = 0; i < tpInfo->GetMapSizeMax( ) && i < ( int )ARRAY_CNT( tpMapObj ); i++ )
	{
		int tMapID = tpInfo->GetMapID( i );
		if( tMapID == INVALID_MAP_ID )
		{
			break;
		}

		tpMapObj[ i ] = CMapObjManager::GetSingletonPtr( )->CreateMap( vLineID, tMapID, INVALID_MAP_INDEX, "worldscene",
			vKey );

		if( tpMapObj[ i ] == NULL )
		{
			LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] CreateMap( %d ) fail", __LK_FILE__, __LINE__, __FUNCTION__,
				tMapID );
			goto CREATE_FAIL;
		}
	}

	// 地图创建成功, 创建副本	
	tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_REPETION );
	if( tpRepetion == NULL )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Create Repetion Error", __LK_FILE__, __LINE__, __FUNCTION__ );
		goto CREATE_FAIL;
	}

	
	tpRepetion->Load( tpInfo );
	tpRepetion->SetKey( vKey );
	for( i = 0; i < ( int )ARRAY_CNT( tpMapObj ); i++ )
	{
		if( tpMapObj[ i ] != NULL )
		{
			tpRepetion->SetMapObjID( tpMapObj[ i ]->GetMapID( ), tpMapObj[ i ]->get_id( ) );
			tpMapObj[ i ]->SetRepetionID( tpRepetion->get_id( ) );
			tpMapObj[ i ]->CreateAllEntity( );
		}
	}

	tpRepetion->SetCreator( vCreator );

	tpRepetion->SetRemoveItem( vRemoveItem );	

	if( tpRepetion->GetRemoveItem( ) == 0 )
	{
		// 副本不是用道具创建, 需要检查副本人数 
		tpRepetion->SetMinCheckTime( tpInfo->GetMinCheckTime( ) );
	}
	tpRepetion->SetParame( 0 , vParam1 );
	tpRepetion->SetParame( 1 , vParam2 );

	tpRepetion->Begin( );

	return tpRepetion;

CREATE_FAIL:
	// 创建副本地图失败, 销毁已经创建地图
	for( i = 0; i < ( int )ARRAY_CNT( tpMapObj ); i++ )
	{
		if( tpMapObj[ i ] == NULL )
		{
			continue;
		}

		CMapObjManager::GetSingletonPtr( )->DestroyMapObject( tpMapObj[ i ] );
	}
	return NULL;	
#endif
	return tpRepetion;
}

CMapObject* CRepetion::GetMapObjectByMapID( int vMapID )
{
#ifdef _FB_
	for( int i = 0; i < GetMapObjSizeMax( ); i++ )
	{
		if( mMapObjIDList[ i ].mMapID == vMapID )
		{
			return GetMapObject( i );
		}
		if( mMapObjIDList[ i ].mMapID == INVALID_OBJ_ID )
		{
			break;
		}
	}
	return NULL;
#endif
	return NULL;
}

int CRepetion::GetPlayerCount( )
{
#ifdef _FB_
	int tCount = 0;
	for( int i = 0; i < GetMapObjSizeMax( ); i++ )
	{
		if( mMapObjIDList[ i ].mMapID == INVALID_MAP_ID || mMapObjIDList[ i ].mMapObjID == INVALID_OBJ_ID )
		{
			break;
		}

		CMapObject* tpMapObj = GetMapObject( i );
		if( tpMapObj == NULL )
		{
			break;
		}

		tCount += tpMapObj->mPlayers.size( );
	}

	return tCount;
#endif
	return 0;
}


// 是否可以重入
bool CRepetion::CanRelogin()
{
#ifdef _FB_
	CRepetionInfo *tpRepetionInfo = CRepetionModule::GetSingleton().GetRepetionInfo( mRepetionIndex );
	if ( tpRepetionInfo != NULL )
	{
		if ( tpRepetionInfo->GetRelogin() == 1 )
		{
			return true;
		}		
	}	
#endif
	return false;
}

// 人数为0并且超过最短存活时间时是否继续保留副本
bool CRepetion::RepetionRemainAlive()
{
#ifdef _FB_	
	CRepetionInfo *tpRepetionInfo = CRepetionModule::GetSingleton().GetRepetionInfo( mRepetionIndex );
	if ( tpRepetionInfo == NULL )
	{
		return false;
	}
	if ( tpRepetionInfo->GetRelogin() != 1 )
	{
		return false;
	}		
	if ( GetPlayerCount() > 0 )
	{
		return true;
	}
	if ( time( NULL ) - mEmptyTime < tpRepetionInfo->GetCloseTime() )
	{
		return true;
	}		
#endif
	return false;
}

CRepetion* CRepetionModule::GetRepetion( CEntity* vpEntity )
{
#ifdef _FB_
	REPE_ASSERT( vpEntity != NULL, return NULL );
	CMapObject* tpMapObj = CMapObjManager::GetSingletonPtr( )->GetMapObject( vpEntity->GetLineID( ), 
		vpEntity->GetMapID( ), vpEntity->GetMapIndex( ) );

	return ( tpMapObj == NULL ) ? NULL : tpMapObj->GetRepetion( );
#endif 
	return NULL;
}

int CRepetion::AllPlayerLeave( bool vDecCount )
{
#ifdef _FB_
	bool tIsHavePlayerLeave = false;
	for( int j = 0; j < GetMapObjSizeMax( ); j++ )
	{
		CMapObject* tpMapObj = GetMapObject( j );
		if( tpMapObj == NULL )
		{
			break;
		}

		int tSize = tpMapObj->mPlayers.size( );
		int i = 0;

		for( i = tSize - 1; i >= 0; i-- )
		{
			CEntityPlayer* tpPlayer = ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( 
					tpMapObj->mPlayers[ i ] );
			if( tpPlayer == NULL )
			{
				continue;
			}

			CPropertyPlayer* tpProperty = ( CPropertyPlayer* )tpPlayer->GetProperty( );
			if( tpProperty == NULL )
			{
				continue;
			}

			if( tpPlayer->IsAlive( ) == false )
			{
				CPropertyModule::GetSingletonPtr( )->PlayerRelive( tpPlayer, RELIVE_SYSTEM, 1, 1 );
			}

			if( vDecCount == true )
			{
				CPlayerRepetion* tpPlayerRepe = CRepetionModule::GetSingletonPtr( )->GetPlayerRepetion( tpPlayer );
				tpPlayerRepe->DecRepetionCount( GetRepetionIndex( ) );
			}
			/*lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );

			vParame.set( "entityid", tpPlayer->get_id() );
			vParame.set( "playerlevel",tpPlayer->GetLevel() );
			vParame.set( "playername", tpPlayer->GetName() );
			vParame.set( "playercountryid",tpPlayer->GetCountryID() );
			vParame.set( "playerlevel",tpPlayer->GetLevel() );
			vParame.set( "playermapid",tpPlayer->GetMapID());
			PERF_FUNC( "Repetion_DoEvent", DoEvent( GetRepetionInfo( ), REPETION_LEAVE_EVENT, &vParame ) );*/

			tIsHavePlayerLeave = true;
			if ( IS_DUNGEONSERVER_LINE( CSceneLogic::GetSingleton().GetLineID() ) )
			{
				CGlobalMapData tMapData;
				tMapData.mLineID = tpProperty->GetRepetion( )->GetBackLineID( );
				tMapData.mMapID = tpProperty->GetRepetion()->GetBackMapID();
				tMapData.mPosX = tpProperty->GetRepetion()->GetPosX();
				tMapData.mPosY = tpProperty->GetRepetion()->GetPosY();
				CGlobalModule::GetSingleton().GlobalTelePort( tpPlayer, EGLOBALTELE_TYPE_RETURNTOGAME, ESERVERTYPE_DUNGEON, &tMapData );
			}
			else
			{
				CMapModule::GetSingletonPtr( )->Teleport(	tpProperty->GetRepetion( )->GetBackLineID( ), 
					tpProperty->GetRepetion( )->GetBackMapID( ),
					tpProperty->GetRepetion( )->GetPosX( ),
					tpProperty->GetRepetion( )->GetPosY( ),
					tpPlayer,
					tpPlayer->GetMapIndex( )
					);
			}			
		}
	}

	if( tIsHavePlayerLeave == false )
	{
		SetClear( );
	}
	
#endif
	return SUCCESS;
}

int CRepetion::SetClear( )
{
#ifdef _FB_
	if( IsInClear( ) == true )
	{
		return SUCCESS;
	}

	int             tOtherInfo[MAX_TIMER_PARA_NUM] = { 0 };
	tOtherInfo[ 0 ] = MESSAGE_REPETION;
	int		tDelayTime = this->GetRepetionInfo()->GetDelayTime();
	mClearTimeID = CTimerMng::GetSingletonPtr( )->SetTimer( get_id( ), TIMER_REPETION_CLEAR, tDelayTime, 1, tOtherInfo );
	if( mClearTimeID == INVALID_OBJ_ID )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %d : %d : %lld Set ClearTimeID Fail, Now Destroy" "					Immediately", __LK_FILE__, __LINE__, __FUNCTION__, get_id( ), GetRepetionIndex( ), GetKey( ) );
		CRepetionModule::GetSingletonPtr( )->DestroyRepetion( this );
	}
	else
	{
		LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] Repetion( %d : %d : %lld ) Will Destroy, ClearTimeID %d", 
			__LK_FILE__, __LINE__, __FUNCTION__, get_id( ), GetRepetionIndex( ), GetKey( ), mClearTimeID );
	}
#endif
	return SUCCESS;
}

int CRepetion::ClearTimeout( )
{
#ifdef _FB_
	 CTimerMng::GetSingletonPtr( )->ClearTimer( mClearTimeID, false );
	CRepetionModule::GetSingletonPtr( )->DestroyRepetion( this );
#endif
	return SUCCESS;
}

int CRepetionModule::GetJoinRepetionCountMax( int vMapID )
{
	CRepetionInfo* tpInfo = GetRepetionInfoByMapID( vMapID );

	REPE_ASSERT_STR( tpInfo != NULL, return 0, "MapID %d Not A RepetionMapID", vMapID );

	return tpInfo->GetPlayerPerNumber( );
}

int CRepetionModule::GetPlayerJoinRepetionCount( int vMapID, CEntityPlayer* vpPlayer )
{
	unsigned int tCount = 0;
	REPE_ASSERT_STR( vpPlayer != NULL, return 0, "Player Is NULL" );
	CPropertyPlayer* tpPlayer = ( CPropertyPlayer* )vpPlayer->GetProperty( );

	tpPlayer->GetRepetion( )->GetRepetionCount( vMapID, &tCount );

	return tCount;
}

int CRepetionModule::GetPlayerJoinRepetionTakeCount( int vMapID, CEntityPlayer* vpPlayer )
{
    REPE_ASSERT_STR( vpPlayer != NULL, return 0, "Player Is NULL" );

	CRepetionInfo* tpInfo = GetRepetionInfoByMapID( vMapID );
	REPE_ASSERT_STR( tpInfo != NULL, return 0, "MapID %d Not A RepetionMapID", vMapID );
    CPropertyPlayer* tpPlayer = ( CPropertyPlayer* )vpPlayer->GetProperty( );       
                                                                 
    return tpInfo->GetPlayerAllTakeCount( tpPlayer );
}

void CRepetionModule::InsertMoneyAll( int vRepetionID, int vMoney, int vBind )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoop( CRepetion::InsertMoneyAll, vMoney, vBind );
#endif
}

bool CRepetion::InsertMoneyAll( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vMoney, int vBind )
{
	REPE_ASSERT( vpEntity != NULL, return true );

	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return true, "EntityID %d, EntityType %d", vpEntity->GetEntityID( ),
		vpEntity->GetEntityType( ) );

	CEntityPlayer* tpPlayer = ( CEntityPlayer* )vpEntity;

	CRepetionInfo* tpInfo = vpRepetion->GetRepetionInfo( );

	REPE_ASSERT_STR( tpInfo != NULL, return true, "RepetionID %d", vpRepetion->get_id( ) );

	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] Player( %d : %d ) Insert Money( %d ) From Repetion( %lld ) )", 
		__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->GetCharID( ), tpPlayer->GetEntityID( ), vMoney, 
		vpRepetion->GetKey( ) );

	// 如果钱超过携带上限，则发系统邮件
	CTemplateLevelMoneyLmt* tpMoneyTable = (CTemplateLevelMoneyLmt*)CDataStatic::GetTemp(TEMP_MONEYLMT_CFG);
	LK_ASSERT( tpMoneyTable!=NULL, return false);
	LK_ASSERT( tpPlayer->GetLevel() >= 0 && tpPlayer->GetLevel() < TMP_MAX_GRADE, return false);

	if ( vBind != 0 ) // 绑定钱
	{
		// 判断钱是否溢出
		if ( tpPlayer->GetBindMoney()+vMoney > (int)tpMoneyTable->mLmtTable[tpPlayer->GetLevel()].mBagBindMoneyLmt )
		{
			// 绑定钱 多出的扔掉
			CPropertyModule::GetSingleton().SendProErrorNotice(tpPlayer, ERR_BAG_BIND_MONEY_UPLMT);
			vMoney = std::min((int)(tpMoneyTable->mLmtTable[tpPlayer->GetLevel()].mBagBindMoneyLmt-tpPlayer->GetBindMoney()), vMoney);
		}
	}
	else // 非绑定钱
	{
		// 判断钱是否溢出
		if ( ( tpPlayer->GetMoney()+vMoney ) > ( int )tpMoneyTable->mLmtTable[tpPlayer->GetLevel()].mBagMoneyLmt )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice(tpPlayer, ERR_REPETION_OVERMONEYLMT_SYSMAIL);
			// 发系统邮件
			CMailModule::GetSingleton().SendSysMailBySysID( SYSMAIL_FB_AWARD, tpPlayer->GetName(), tpPlayer->GetCharID(),
				NULL, NULL, 0, tpPlayer->GetNationality(), vMoney ); 
			return true;
		}
	}

	CPropertyModule::GetSingletonPtr( )->PlayerChangeMoney( tpPlayer, vMoney, false, ( vBind != 0 ) ? true : false );

	return true;
}

bool CRepetion::RemoveBuff( CRepetion* vpRepetion, CEntityCharacter* vpDesEntity, int vBuffTempID, int vParame2)
{
#ifdef _FB_	
	REPE_ASSERT( vpDesEntity != NULL, return true );

	if( vpDesEntity->HasBuff( vBuffTempID ) )
	{
		vpDesEntity->EntityRemoveBuff( vBuffTempID );
		CPropertyModule::GetSingletonPtr( )->ResultListSend( );
	}

#endif
	return true;

}

bool CRepetion::InsertBuff( CRepetion* vpRepetion, CEntityCharacter* vpDesEntity, int vBuffTempID, int vParame2)
{
#ifdef _FB_	
	REPE_ASSERT( vpDesEntity != NULL, return true );
	CTemplateBuff* tpBuffTpl = (CTemplateBuff*) CDataStatic::SearchTpl( vBuffTempID );

	REPE_ASSERT( tpBuffTpl != NULL, return true );
	vpDesEntity->EntityInsertBuff( vpDesEntity,  tpBuffTpl);
	CPropertyModule::GetSingletonPtr( )->ResultListSend( );
#endif
	return true;

}

bool CRepetion::LeaveRepetion( CRepetion* vpRepetion, CEntityCharacter* vpDesEntity, int vParame1, int vParame2)
{
#ifdef _FB_	
	REPE_ASSERT( vpDesEntity != NULL, return true );
	CRepetionModule::GetSingletonPtr( )->PlayerLeaveRepetion((CEntityPlayer *) vpDesEntity);
#endif
	return true;

}
bool CRepetion::AddIntDataByCamp( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vCampID, int vParam )
{
#ifdef _FB_	
	
	REPE_ASSERT( vpEntity != NULL, return true );
	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return false, "EntityType Is %d", vpEntity->GetEntityType( ) );
	CEntityPlayer *tpPlayer = ( CEntityPlayer * )vpEntity;
	int *tpData = (int*)vParam;
	if( vpEntity->GetCampID( ) == vCampID && tpData != NULL )
	{
		AddIntData( vpRepetion, vpEntity, tpData[0], tpData[1] );
	}
	return true;
	
#endif
	return true;
}

bool CRepetion::GetCampPlayerCount( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vCampID, int vParam )
{
#ifdef _FB_
	REPE_ASSERT( vpEntity != NULL, return true );
	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return false, "EntityType Is %d", vpEntity->GetEntityType( ) );	
	int *tpData = (int*)vParam;
	if( vpEntity->GetCampID( ) == vCampID && tpData != NULL )
	{
		*tpData+=1;
	}
	return true;
#endif
}

// 参数vParam是CCampMsgData类型
bool CRepetion::SendMsgByCamp( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vCampID, int vParam )
{
#ifdef _FB_	
	REPE_ASSERT( vpEntity != NULL, return true );
	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return false, "EntityType Is %d", vpEntity->GetEntityType( ) );
	CEntityPlayer *tpPlayer = ( CEntityPlayer * )vpEntity;
	CCampMsgData *tpData = (CCampMsgData*)vParam;	
	LK_ASSERT( tpData != NULL, return true )
	if( tpPlayer->GetCampID( ) == vCampID && tpData->IsExceptRole( tpPlayer->GetCharID() ) == false )
	{
		CSceneLogic::GetSingletonPtr( )->Send2Player( ( CEntityPlayer* )vpEntity, tpData->mpMsgParam );
	}
#endif
	return true;
}
void CRepetionModule::NpcRemoveBuff( int vRepetionID, int vDesTemplateID, int vBuffTempID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return, "ERR_INVALID_PARAMETER, Repetion ID is %d", vRepetionID );

	tpRepetion->NpcEntityLoop( vDesTemplateID, CRepetion::RemoveBuff, vBuffTempID, 0 );

	return;
#endif
}

void CRepetionModule::NpcRemoveBuffByMapID( int vRepetionID, int vDesTemplateID, int vBuffTempID, int vMapID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return, "ERR_INVALID_PARAMETER, Repetion ID is %d", vRepetionID );

	tpRepetion->NpcEntityLoopByMap( vDesTemplateID, CRepetion::RemoveBuff, vMapID, vBuffTempID, 0 );

	return;
#endif
}

void CRepetionModule::NpcInsertBuffByMapID( int vRepetionID, int vDesTemplateID, int vBuffTempID, int vMapID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return, "ERR_INVALID_PARAMETER, Repetion ID is %d", vRepetionID );

	if( vMapID == 0 )
	{
		tpRepetion->NpcEntityLoop( vDesTemplateID, CRepetion::InsertBuff, vBuffTempID, 0 );
	}
	else
	{
		tpRepetion->NpcEntityLoopByMap( vDesTemplateID, CRepetion::InsertBuff, vMapID, vBuffTempID, 0 );
	}

	return;
#endif
}

void CRepetionModule::PlayerRemoveBuffByMapID( int vRepetionID,  int vBuffTempID, int vMapID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return, "ERR_INVALID_PARAMETER, Repetion ID is %d", vRepetionID );

	tpRepetion->PlayerEntityLoopByMap( CRepetion::RemoveBuff, vMapID, vBuffTempID, 0 );

	return;
#endif
}

void CRepetionModule::PlayerInsertBuffByMapID( int vRepetionID,  int vBuffTempID, int vMapID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return, "ERR_INVALID_PARAMETER, Repetion ID is %d", vRepetionID );

	tpRepetion->PlayerEntityLoopByMap( CRepetion::InsertBuff, vMapID, vBuffTempID, 0 );

	return;
#endif
}
/* --------------------------------------------------------------------------*/
/*                                                     
* @Synopsis  获得地图上的人数                                                                               *     
* @Param                                      
* @Param  
*  
* @Returns                                             
*/
/* ----------------------------------------------------------------------------*/
int  CRepetionModule::GetPlayerNumByMapID(int vRepetionID, int vMapID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );

	REPE_ASSERT_STR( tpRepetion != NULL, return ERR_INVALID_PARAMETER, "ERR_INVALID_PARAMETER, Repetion ID is %d", vRepetionID );

	CMapObject* tpMapObj = tpRepetion->GetMapObjectByMapID( vMapID );
	REPE_ASSERT_STR( tpMapObj != NULL, return ERR_INVALID_PARAMETER, "MapID %d", vMapID );
	
	return tpMapObj->mPlayers.size();

#endif
	return 0;
}

void CRepetionModule::CreateRedstone( 
		int vRepetionID, int vStoneTempID, int vFamilyID, lua_tinker::table vMapInfo )
{
#ifdef _FB_
	int tMapID = vMapInfo.get< int >( "mapid" );
	int tPosX = vMapInfo.get< int >( "posx" );
	int tPosY = vMapInfo.get< int >( "posy" );
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	CMapObject* tpMapObj = tpRepetion->GetMapObjectByMapID( tMapID );
	REPE_ASSERT_STR( tpMapObj != NULL, return, "RepetionID %d, MapID %d", vRepetionID, tMapID );

	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Create Family Stone, Repetion( %d : %lld ), Map %d, Family %d",
		__LK_FILE__, __LINE__, __FUNCTION__, vRepetionID, tpRepetion->GetKey( ), tpMapObj->GetMapID( ), vFamilyID );

	CPropertyModule::GetSingletonPtr( )->CreatFamilyRepetionRedstone( 
		vStoneTempID, vFamilyID, tpMapObj->GetLineID( ), tpMapObj->GetMapID( ), tPosX, tPosY, 
		tpMapObj->GetMapIndex( ), tpMapObj->GetKey( ) );	
#endif
}

bool CRepetionModule::IsUnRelive( int vMapID )
{
	CRepetionInfo* tpInfo = GetRepetionInfoByMapID( vMapID );
	REPE_ASSERT_STR( tpInfo != NULL, return false, "vMapID %d", vMapID );

	return tpInfo->IsUnRelive( );
}

int CRepetionModule::PlayerCanIntoRepetion( CEntityPlayer* vpPlayer, int vRepetionIndex )
{
	int tRtn = SUCCESS;
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	CRepetionInfo* tpInfo = GetRepetionInfo( vRepetionIndex );

	if( tpInfo == NULL )
	{
//		SendErrCode( vpPlayer, ERR_REDIRECT_MAP_NOTOPEN, vMapID );
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] Cannt Find RepetionIndex %d RepetionInfo, Player %d : %s", 
			__LK_FILE__, __LINE__, __FUNCTION__, vRepetionIndex, vpPlayer->GetCharID( ), vpPlayer->GetCharNameShow( ) );
		return ERR_REDIRECT_MAP_NOTOPEN;
	}

	tRtn = PlayerIntoRepetionCheck( vpPlayer, tpInfo, CHECK_ALL_FLAG );
	if( tRtn != SUCCESS )
	{
		// 不满足进入副本条件
		if( tRtn == ERR_REPETION_EVERYDAY_TIME_FULL || tRtn == ERR_REPETION_WEEK_TIME_FULL )
		{	
			// 每日进入次数满
			SendErrCode( vpPlayer, ( ERepetion )tRtn, tpInfo->GetBeginMapID( ), 
				tpInfo->GetPlayerPerNumber( ), vpPlayer->GetCharID( ) );
		}
		else if( tRtn == ERR_REPETION_NEED_TASK )
		{
			// 无副本进入需要任务
			SendErrCode( vpPlayer, ( ERepetion )tRtn, tpInfo->GetBeginMapID( ), tpInfo->GetTaskID( ), 
				vpPlayer->GetCharID( ) );
		}
		else if( tRtn == ERR_REPETION_LEVEL_MIN )
		{
			// 小于进入副本最低等级
			SendErrCode( vpPlayer, ( ERepetion )tRtn, 
					tpInfo->GetBeginMapID( ), vpPlayer->GetLevel( ), tpInfo->GetMinLevel( ) );
		}
		else if( tRtn == ERR_REPETION_DIE )
		{
			// 玩家处于死亡状态
			SendErrCode( vpPlayer, ( ERepetion )tRtn, tpInfo->GetBeginMapID( ), vpPlayer->GetCharID( ) );
		}
	}


	return tRtn;
}

KEY_TYPE CRepetionModule::GetRepetionKey( int vRepetionID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return 0, "vRepetionID %d", vRepetionID );

	return tpRepetion->GetKey( );
#endif
	return 0;
}

void CRepetionModule::FamilyRepetionInfoEnd( int vRepetionID, int vFamilyID, lua_tinker::table vTable )
{
	int vTime = vTable.get<int>( "time" );
	int vMark = vTable.get<int>( "mark" );
	int vOgreCount = vTable.get<int>( "ogre" );
	int vLevel = CRepetionModule::GetRepeLevel( vRepetionID );
	int vStage = vTable.get<int>( "stage" );
	int vScore = vTable.get<int>( "score" );

	CFamilyModule::GetSingletonPtr( )->ClearFamilyRepetionInfo( vFamilyID, vLevel, vOgreCount, vTime, vStage, vStage, 
		vMark, vScore );
}


int CRepetionModule::GetBackMapID( int vRepetionID, int vEntityID )
{
#ifdef _FB_
	CEntityPlayer* vpPlayer = ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( vpPlayer != NULL, return INVALID_MAP_ID, "EntityID %d", vEntityID );
	CPropertyPlayer* vpProperty = ( CPropertyPlayer* )vpPlayer->GetProperty( );

	return vpProperty->GetRepetion( )->GetBackMapID( );
#endif
	return 0;
}

int CRepetionModule::NotifyRepetionIndex( CEntityPlayer* vpPlayer, int vRepetionIndex )
{
	CMessage tMsg;
	CMessageRepetionIndexNotice tNotice;

	tNotice.set_repetionindex( vRepetionIndex );

	tMsg.set_msgpara( ( long )&tNotice );
	tMsg.mutable_msghead()->set_messageid( ID_S2C_REPETIONINDEX_NOTICE );


	CSceneLogic::GetSingleton().Send2Player( vpPlayer, &tMsg );

	return SUCCESS;
}

int CRepetionModule::PlayerSingleIntoRepe( CEntityPlayer* vpPlayer, int vRepeIndex, int vPosX, int vPosY )
{
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	CRepetionInfo* tpInfo = GetRepetionInfo( vRepeIndex );

	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, " RepetionIndex %d ", vRepeIndex );

	CMessage tMsg;
	CMessageSingleIntoRepetion tInto;

	tInto.set_mapid( tpInfo->GetBeginMapID( ) );
	tInto.set_charid( vpPlayer->GetCharID( ) );
	tInto.set_posx( vPosX );
	tInto.set_posy( vPosY );
	tInto.set_level( vpPlayer->GetTeamLevel( ) );

	tMsg.mutable_msghead()->set_messageid( ID_S2G_INTO_REPETION_SINGLE );
	tMsg.set_msgpara( (uint32)&tInto );

	CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMsg );

	return SUCCESS;
}



const SpecialItem* CRepetionModule::GetSpecialUnCheck( CEntityPlayer* vpPlayer, CRepetionInfo* vpInfo, int vCheckFlag, 
	int vUnCheckFlag )
{
	REPE_ASSERT( vpPlayer != NULL, return NULL );
	REPE_ASSERT( vpInfo != NULL, return NULL );

	unsigned int tIndex = 0;
	const SpecialItem* tpSpecialItem = NULL;
	int tUnCheckFlag = UN_CHECK_FLAG_ADD( GET_UNCHECK_FLAG( vCheckFlag ), vUnCheckFlag );
	while( ( tpSpecialItem = vpInfo->GetSpecialItem( tUnCheckFlag, &tIndex ) ) != NULL )
	{
		int tBackType = 0;
		if( CPropertyModule::GetSingletonPtr( )->HasItem( vpPlayer->GetEntityID( ), 
				( unsigned int )tpSpecialItem->mItemID, 1,  tBackType ) == -1 )
		{
			tIndex++;
			continue;
		}

		return tpSpecialItem;

	}
	
	return NULL;
}


// 清除指定副本次数一次
int CRepetionModule::DelteRepetionCount( CEntityPlayer* pPlayer, int nRepetionIdx )
{
	//TODO:目前只处理了普通副本和家族品仙桃
	CPropertyPlayer* tpPlayerPro = (CPropertyPlayer*) pPlayer->GetProperty();
	CPlayerRepetion* tpRepetion = tpPlayerPro->GetRepetion();
	tpRepetion->DecRepetionCount( nRepetionIdx );
	// TODO: 因为很少调用到这里，所以帮助信息全部发送一遍，等要优化的时候再只发副本信息
	CActivityModule::GetSingletonPtr()->SendHelpInfo2Client( pPlayer );

	// 品仙桃特殊处理，还要发向网关
	if( nRepetionIdx == _REPE_FAMILY_TASTE_INDEX_ )
	{
		CMessageChangeGateNotify tChangeNotify;
		CMessage	tMsg;
		tMsg.mutable_msghead( )->set_messageid( ID_S2G_CHANGEGATE_NOTIFY );
		tMsg.set_msgpara( (uint32)&tChangeNotify );
		tChangeNotify.set_type( CMD_FAMILY_REPETIME );
		CFamilyInfo *tpFamilyInfo = CFamilyModule::GetSingleton().GetFamilyInfo( pPlayer->GetFamilyID() );
		if( tpFamilyInfo == NULL )
		{
			tChangeNotify.set_name( tpFamilyInfo->GetFamilyName() );					// 名字
			tChangeNotify.set_param1( -1 );		// 修改数值
			CSceneLogic::GetSingletonPtr()->Send2Gate( &tMsg );	
		}
	}
	return 0;
}


int CRepetion::RemoveSpecialItem( CEntityPlayer* vpPlayer )
{
	CRepetionInfo* tpInfo = GetRepetionInfo( );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_NOT_FOUND, "Index %d, Key %lld", GetRepetionIndex( ), GetKey( ) );

	if( GetRemoveItem( ) == 0 )
	{
		return SUCCESS;
	}

	if( ( unsigned int )GetCreator( ) != vpPlayer->GetCharID( ) )
	{
		return SUCCESS;
	}

	bool tBind = true;
	uint64_t ullGUID = 0;
	int tRtn = CPropertyModule::GetSingletonPtr( )->PlayerRemoveItemByID( vpPlayer, GetRemoveItem( ), 1, 
		tBind, &ullGUID );

	if( tRtn != 0 )
	{
		return ERR_NOT_FOUND;
	}

	LogEventLostItemByRepetion( vpPlayer, ullGUID, GetRemoveItem( ), 1 );
	SetRemoveItem( 0 );

	return SUCCESS;
}

void CRepetionModule::CreateNpcByBite( unsigned int vRepetionID, int vBite, lua_tinker::table vList )
{
	int i = 0;
	for( i = 1; i <= vList.getn( ); i++ )
	{
		unsigned int tIndex = vList.get< unsigned int >( i );
		if( ( vBite & ( 1 << ( i - 1 ) ) ) != 0 )
		{
			CRepetionModule::CreateNpc( vRepetionID, tIndex );
		}
	}
}

int CRepetionModule::SetVirtualMap( int vRepetionID, int vRealMapID, int vVirtualMapID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return ERR_INVALID_PARAMETER, "RepetionID %d", vRepetionID );

	CMapObject* tpMapObject = tpRepetion->GetMapObjectByMapID( vRealMapID );
	REPE_ASSERT_STR( tpMapObject != NULL, return ERR_INVALID_PARAMETER, "RepetionID %d", vRepetionID );

	return tpMapObject->SetVirtualMapID( vVirtualMapID );

#endif	

	return SUCCESS;
}

int CRepetionModule::SetMapIntData( int vRepetionID, int vRealMapID, int vKey, int vData )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return ERR_INVALID_PARAMETER, "RepetionID %d", vRepetionID );

    CMapObject* tpMapObject = tpRepetion->GetMapObjectByMapID( vRealMapID );
    REPE_ASSERT_STR( tpMapObject != NULL, return ERR_INVALID_PARAMETER, "MapID %d", vRealMapID );

	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Set Repetion %d : %d : %lld, RealMapID %d, VirtualMapID %d, Key %d, "
		" Data %d", __LK_FILE__, __LINE__, __FUNCTION__, tpRepetion->GetRepetionIndex( ), tpRepetion->get_id( ), 
		tpRepetion->GetKey( ), tpMapObject->GetMapID( ), tpMapObject->GetVirtualMapID( ), vKey, vData );

	return tpMapObject->SetMapData( vKey, vData );
#endif

	return 0;
}

int CRepetionModule::GetMapIntData( int vRepetionID, int vRealMapID, int vKey )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return ERR_INVALID_PARAMETER, "RepetionID %d", vRepetionID );

    CMapObject* tpMapObject = tpRepetion->GetMapObjectByMapID( vRealMapID );
    REPE_ASSERT_STR( tpMapObject != NULL, return ERR_INVALID_PARAMETER, "MapID %d", vRealMapID );

	int tValue =  tpMapObject->GetMapData( vKey );
	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Get Repetion %d : %d : %lld, RealMapID %d, VirtualMapID %d, Key %d, "
		" Data %d", __LK_FILE__, __LINE__, __FUNCTION__, tpRepetion->GetRepetionIndex( ), tpRepetion->get_id( ),
        tpRepetion->GetKey( ), tpMapObject->GetMapID( ), tpMapObject->GetVirtualMapID( ), vKey, tValue );

	return tValue;
	
#endif

	return 0;
}

void CRepetionModule::CreateNpcRealMap( unsigned int vRepetionID, int vRealMapID, int vIndex )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return , "RepetionID %d", vRepetionID );

    CMapObject* tpMapObject = tpRepetion->GetMapObjectByMapID( vRealMapID );
    REPE_ASSERT_STR( tpMapObject != NULL, return , "MapID %d", vRealMapID );

	CCreator* tpCreator = CCreatorManager::GetSingletonPtr( )->GetCreator( vIndex );
	REPE_ASSERT_STR( tpCreator != NULL, return , "Creator %d", vIndex );

	tpMapObject->CreateNpc( tpCreator );

#endif
	return ;
}

int CRepetionModule::EachOther( CEntityPlayer* vpPlayer, int vKey, int vParame1, int vParame2 ) 
{ 
#ifdef _FB_ 
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	CMapObject* tpObj = vpPlayer->GetMapObject( );
	REPE_ASSERT_STR( tpObj != NULL, return ERR_INVALID_PARAMETER, " Player %d : %d : %s, Map %d, Not Found", 
		vpPlayer->GetEntityID( ), vpPlayer->GetCharID( ), vpPlayer->GetCharNameShow( ), vpPlayer->GetMapID( ) );

    CRepetion* tpRepetion = tpObj->GetRepetion( ); 
    REPE_ASSERT_STR( tpRepetion != NULL, return ERR_INVALID_PARAMETER, "RepetionID %d", tpObj->GetRepetionID( ) ); 
 
    lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua ); 
 
    vParame.set( "key", vKey ); 
    vParame.set( "parame1", vParame1 ); 
    vParame.set( "parame2", vParame2 ); 
 
    PERF_FUNC( "Repetion_DoEvent", tpRepetion->DoEvent( tpRepetion->GetRepetionInfo( ), REPETION_EACH_OTHER, 
		&vParame ) ); 
#endif 
 
    return SUCCESS; 
}

void CRepetionModule::AllTeleport( unsigned int vRepetionID, int vMapID, int vPosX, int vPosY, int vUnTeleMapID )
{

#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return , "RepetionID %d", vRepetionID );

	int tParame[ 3 ] = { 0 };
	tParame[ 0 ] = vPosX;
	tParame[ 1 ] = vPosY;
	tParame[ 2 ] = vUnTeleMapID;

	tpRepetion->PlayerEntityLoop( CRepetion::AllTeleport, vMapID, (int)tParame );
#endif

	return ;

}

bool CRepetion::AllTeleport( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vMapID, int vParame2 )
{
	REPE_ASSERT( vpRepetion != NULL, return false );
	REPE_ASSERT( vpEntity != NULL, return false );
	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return ERR_INVALID_PARAMETER, "EntityID %d Type %d", 
		vpEntity->GetEntityID( ), vpEntity->GetEntityType( ) );

	CMapObject* tpMapObj = vpRepetion->GetMapObjectByMapID( vMapID );	
	REPE_ASSERT_STR( tpMapObj != NULL, return false, "RepetionIndex %d, MapID %d", vpRepetion->GetRepetionIndex( ),
		vMapID );

	int tPosX = (( int* )vParame2)[ 0 ];
	int tPosY = (( int* )vParame2)[ 1 ];
	int tUnTeleMapID = (( int* )vParame2)[ 2 ];

	if( vpEntity->GetMapID( ) != tUnTeleMapID )
	{
		CMapModule::GetSingletonPtr( )->Teleport( tpMapObj->GetLineID( ), tpMapObj->GetMapID( ), tPosX, tPosY,
			vpEntity, tpMapObj->GetMapIndex( ), vpRepetion->GetKey( ) );
	}

	return true;
}

int CRepetionModule::NpcChangeLevel( int vRepetionID, int vEntityID, int vChangeLevel, int vLevel )
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return ERR_INVALID_PARAMETER, " EntityID %d", vEntityID );

	REPE_ASSERT_STR( tpEntity->IsNpc( ) == true, return ERR_INVALID_PARAMETER, " EntityID %d Is Not Npc, Type %d",
		vEntityID, tpEntity->GetEntityType( ) );

	if( vChangeLevel > 0 )
	{	
		( ( CEntityNpc* )tpEntity )->ChangeNpcByLevel( vChangeLevel );
	}

	if( vLevel > 0 )
	{
		( ( CEntityNpc* )tpEntity )->SetLevel( vLevel );
		if( vLevel != vChangeLevel )
		{
			( ( CEntityNpc* )tpEntity )->NotifyPropertyRefresh( );
		}
	}
#endif
	return SUCCESS;
}

int CRepetionModule::OnMessageNpcFunc( CEntityPlayer* vpPlayer, CMessage* vpMsg )
{
#ifdef _FB_
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	CMessageRepetionFuncPlayerClick* vpNpcFunc = ( CMessageRepetionFuncPlayerClick* )vpMsg->msgpara( );

	REPE_ASSERT( vpNpcFunc != NULL, return ERR_INVALID_PARAMETER );

	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vpNpcFunc->npcid( ) );
	REPE_ASSERT_STR( tpEntity != NULL, return ERR_INVALID_PARAMETER, " EntityID %d", vpNpcFunc->npcid( ) );
	REPE_ASSERT_STR( tpEntity->IsFunNpc( ) == true, return ERR_INVALID_PARAMETER, " EntityID %d Is Not FunNpc, Type %d",
		vpNpcFunc->npcid( ), tpEntity->GetEntityType( ) );

	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( ( ( CEntityFuncNpc* )tpEntity )->GetNpcID( ) );
	REPE_ASSERT_STR( tpTmpNpc != NULL, return ERR_INVALID_PARAMETER, " EntityID %d Cantt Get Temp %d", 
		vpNpcFunc->npcid( ), ( ( CEntityFuncNpc* )tpEntity )->GetNpcID( ) );

	if( tpTmpNpc->mFBFunc != 0 )
	{
		EachOther( vpPlayer, tpTmpNpc->mFBFunc, tpTmpNpc->mFBFuncParame1, tpTmpNpc->mFBFuncParame2 );	
	}
#endif
	return SUCCESS;
}


int CRepetionModule::EntityRemoveBuff( int vRepetionID, int vEntityID, int vBuffID )
{
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return ERR_INVALID_PARAMETER, " EntityID %d Is NULL", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsCharacter( ) == true, return ERR_INVALID_PARAMETER, " Entity %d Type %d",
		vEntityID, tpEntity->GetEntityType( ) );

	LOG_DEBUG( REPETION_LOG,  "[ %s : %d ][ %s ] Entity %d Remove Buff %d", __LK_FILE__, __LINE__, __FUNCTION__, 
		vEntityID, vBuffID );

	( ( CEntityCharacter* )tpEntity )->EntityRemoveBuff( vBuffID );
	CPropertyModule::GetSingletonPtr( )->ResultListSend( );
	
	return SUCCESS;
	
}

int CRepetionModule::EntityInsertBuff( int vRepetionID, int vEntityID, int vBuffID )
{
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return ERR_INVALID_PARAMETER, " EntityID %d Is NULL", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsCharacter( ) == true, return ERR_INVALID_PARAMETER, " Entity %d Type %d",
		vEntityID, tpEntity->GetEntityType( ) );

	CTemplateBuff* tpTpl = ( CTemplateBuff* )CDataStatic::SearchTpl( vBuffID );
	REPE_ASSERT_STR( tpTpl != NULL, return ERR_INVALID_PARAMETER, " BuffID %d Cannt Find", vBuffID );

	LOG_DEBUG( REPETION_LOG,  "[ %s : %d ][ %s ] Entity %d Insert Buff %d", __LK_FILE__, __LINE__, __FUNCTION__, 
		vEntityID, vBuffID );

	( ( CEntityCharacter* )tpEntity )->EntityInsertBuff( ( CEntityCharacter* )tpEntity, tpTpl );
	CPropertyModule::GetSingletonPtr( )->ResultListSend( );
	
	return SUCCESS;
}

int CRepetionModule::GetPlayerRepeFloorNo( CEntityPlayer* pPlayer )
{
#ifdef _FB_
	CMapObject* tpMapObj = pPlayer->GetMapObject( );
	if( tpMapObj == NULL )
	{
		return 0;
	}

	return tpMapObj->GetMapData( 0 );
#endif
	return 0;
}

void CRepetionModule::SetTodayTimesByGroupID( int vRepetionID, int vEntityID, int vTaskGroupID, int vCount )
{
#ifdef _FB_
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return, "EntityID %d ", vEntityID );
	REPE_ASSERT_STR( tpEntity->IsPlayer( ) == true, return, "EntityID %d, Type %d", vEntityID, 
		tpEntity->GetEntityType( ) );

	CTaskModule::GetSingletonPtr( )->SetTodayTimesByGroupID( ( CEntityPlayer* )tpEntity, vTaskGroupID, vCount );
#endif
	return;
}

void CRepetionModule::CheckRepetionQuestionVerify( CRepetionInfo *pRepetion, CEntityPlayer *pPlayer )
{
#ifdef _FB_
	LK_ASSERT( pRepetion != NULL && pPlayer != NULL, return )  			
	
	// 判断是否需要进行答题验证
	if ( pRepetion->GetQuestionCheck() == 0 )
	{
		return;
	}
	
	// 副本
	CPlayerRepetion* tpCheckPlayerRepetion = CRepetionModule::GetSingletonPtr( )->GetPlayerRepetion( pPlayer );
	LK_ASSERT( tpCheckPlayerRepetion != NULL, return )		
	if( tpCheckPlayerRepetion->GetCheckResult() == true )
	{
		return;
	}
	
	// 判断是否有队员是会员或者使用钥匙进入的
	CEntityPlayer *TeamPlayer[TEAMNUM] = { 0 };
	int tRtn = 0, TeamNum = 0;				   	
	tRtn = CTeamModule::GetSingletonPtr()->OnGetMemberList( pPlayer, TeamPlayer, ARRAY_CNT( TeamPlayer ), TeamNum );
	if ( tRtn >=0 && TeamNum >= 1 )
	{
		for ( int i = 0; i < TeamNum; ++i )
		{
			if ( TeamPlayer[i] == NULL )
			{
				continue;
			}	

			// 有会员不用检查
			CPropertyPlayer *tpProperty = ( CPropertyPlayer * )TeamPlayer[i]->GetProperty();
			if ( tpProperty->IsVip() )
			{
				tpCheckPlayerRepetion->SetCheckResult( true );
				return;
			}
			
			CPlayerRepetion* tpPlayerRepetion = CRepetionModule::GetSingletonPtr( )->GetPlayerRepetion( TeamPlayer[i] );
			if (tpPlayerRepetion == NULL )
			{
				continue;
			}
			
			// 使用道具不用检查
			if ( tpPlayerRepetion->GetAnswerCount() == 0 && tpPlayerRepetion->GetCheckResult() == true )
			{
				tpCheckPlayerRepetion->SetCheckResult( true );
				return;
			}										
		}		
	}
	else
	{
		CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
		if ( tpProperty->IsVip() == true )
		{
			tpCheckPlayerRepetion->SetCheckResult( true );
			return;
		}
	}  	
	
	if ( tpCheckPlayerRepetion->GetAnswerCount() >= ( int )CSceneCfgManager::GetSingleton().GetRepetion_cfg()->mWrongtimes )
	{
		// 踢出玩家副本次数加1,过几秒后再踢人		
		int tOtherInfo = MESSAGE_REPETION;	
		int tKickOutTime = CSceneCfgManager::GetSingleton().GetRepetion_cfg()->mRepetionkickouttime;
		CTimerMng::GetSingleton().SetTimer( pPlayer->GetEntityID(), TIMER_KICKOUTBYVERIFY, tKickOutTime*1000, 1, &tOtherInfo );		
		return;
	}
		
	
	int tTimeLength = CSceneCfgManager::GetSingleton().GetRepetion_cfg()->mTimelowlimit;		
	if ( tpCheckPlayerRepetion->GetAnswerCount() == 0 )
	{
		tTimeLength +=  RAND( CSceneCfgManager::GetSingleton().GetRepetion_cfg()->mTimehighlimit - tTimeLength );
	}
	
	// 设置定时器
	/*tpPlayerRepetion->SetAnswerCount( tpPlayerRepetion->GetAnswerCount() + 1 );	
	CActivityModule::GetSingleton().NotifyQuestionVerify( pPlayer, VERIFYKIND_REPETION, VERIFYTYPE_TIMEOUT|VERIFYTYPE_OFFLINE, tTimeLength );*/
	int tOtherInfo = MESSAGE_REPETION;	
	CTimerMng::GetSingleton().SetTimer( pPlayer->GetEntityID(), TIMER_REPETIONQUESTION, tTimeLength*1000, 1, &tOtherInfo );		
#endif	
}

void CRepetionModule::RepetionQuestionVerifyResult( CEntityPlayer *pPlayer, int tCheckResult, int nReason )
{
#ifdef _FB_
	LK_ASSERT( pPlayer != NULL, return )	
	CPlayerRepetion* tpPlayerRepetion = CRepetionModule::GetSingletonPtr( )->GetPlayerRepetion( pPlayer );
	LK_ASSERT( tpPlayerRepetion != NULL, return )		
	if ( nReason == VERIFYREASON_OFFLINE )
	{
		return;
	}
	
	/*if (nReason == VERIFYREASON_TIMEOUT  )
	{
		// 踢出玩家副本次数加1
		PlayerLeaveRepetion( pPlayer );
		return;
	}
	*/
	
	if ( tCheckResult == true )
	{
		tpPlayerRepetion->SetCheckResult( true );
		return;	
	}  	
	
	if ( tpPlayerRepetion->GetAnswerCount() >= ( int )CSceneCfgManager::GetSingleton().GetRepetion_cfg()->mWrongtimes )
	{
		// 踢出玩家副本次数加1
		PlayerLeaveRepetion( pPlayer );
		return;
	}	
	
	tpPlayerRepetion->SetAnswerCount( tpPlayerRepetion->GetAnswerCount()+1 );
	int tTimeLength = CSceneCfgManager::GetSingleton().GetRepetion_cfg()->mQuestionanswertime;
	CActivityModule::GetSingleton().NotifyQuestionVerify( pPlayer, VERIFYKIND_REPETION, VERIFYTYPE_TIMEOUT|VERIFYTYPE_OFFLINE, tTimeLength );
#endif
}


bool CRepetion::AllPlayerInSpecLevel( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vParame1,int vParame2 )
{
#ifdef _FB_
	REPE_ASSERT( vpRepetion != NULL, return false );
	REPE_ASSERT( vpEntity != NULL, return false );
	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return false, "Entity %d Type %d", vpEntity->GetEntityID( ),
		vpEntity->GetEntityType( ) );
	CRepetionInfo* tpInfo = vpRepetion->GetRepetionInfo( );
	REPE_ASSERT_STR( tpInfo != NULL, return false, "Repetion %d, Index %d, Key %lld", vpRepetion->get_id( ),
		 vpRepetion->GetRepetionIndex( ), vpRepetion->GetKey( ) );

	if( tpInfo->IsSpecialCheck( ( CEntityPlayer* )vpEntity, REPE_JOIN_COUNT ) == true )
	{
		return false;
	}
	
#endif
	return true;
}

void CRepetionModule::PlayerReliveByMapID( int vRepetionID, int vMapID, int vPosX, int vPosY )
{
#ifdef _FB_
	int tPos[ 2 ] = { 0 };

	tPos[ 0 ] = vPosX;
	tPos[ 1 ] = vPosY;	

	CRepetion* tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoop( CRepetion::PlayerReliveByMapID, vMapID, ( int )tPos );

#endif
}

void CRepetionModule::PlayerReliveSys( int vRepetionID )
{
#ifdef _FB_

	CRepetion* tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoop( CRepetion::PlayerReliveSys, 0, 0 );

#endif
}

bool CRepetion::PlayerReliveByMapID( CRepetion* vpRepetion, CEntityCharacter* vpCharacter, int vMapID, int vPosPoint )
{
#ifdef _FB_
	REPE_ASSERT( vpCharacter != NULL, return true );
	REPE_ASSERT_STR( vpCharacter->IsPlayer( ) == true, return true, "EntityType %d, EntityID %d", 
			vpCharacter->GetEntityType( ), vpCharacter->GetEntityType( ) );

	int* tPos = ( int* )vPosPoint;
	CEntityPlayer* tpPlayer = ( CEntityPlayer* )vpCharacter;

	CRepetionModule::GetSingletonPtr( )->RelivePlayer( tpPlayer, vpRepetion, tPos[ 0 ], tPos[ 1 ], vMapID );
#endif
	return true;
}

bool CRepetion::PlayerReliveSys( CRepetion* vpRepetion, CEntityCharacter* vpCharacter, int vParame1, int vParame2 )
{
#ifdef _FB_
	REPE_ASSERT( vpCharacter != NULL, return true );
	REPE_ASSERT_STR( vpCharacter->IsPlayer( ) == true, return true, "EntityType %d, EntityID %d", 
			vpCharacter->GetEntityType( ), vpCharacter->GetEntityType( ) );

	CEntityPlayer* tpPlayer = ( CEntityPlayer* )vpCharacter;

	CPropertyModule::GetSingletonPtr( )->PlayerRelive( tpPlayer, RELIVE_WEAKGRAVE, 0, 0 );
#endif
	return true;
}

int CRepetionModule::AddTitleAll( int vRepetionID, int vTitleID, int vEntityID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return ERR_INVALID_PARAMETER, " RepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoop( CRepetion::AddTitle, vTitleID, vEntityID );
#endif
	return SUCCESS;
}

bool CRepetion::AddTitle( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vTitleID, int vEntityID )
{
#ifdef _FB_
	REPE_ASSERT( vpRepetion != NULL, return true );
	REPE_ASSERT( vpEntity != NULL, return true );
	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return true, " EntityID %d, EntityType %d", 
		vpEntity->GetEntityID( ), vpEntity->GetEntityType( ) );
	
	if( vEntityID == 0 || ( unsigned int )vEntityID == vpEntity->GetEntityID( ) )
	{
		CPropertyModule::GetSingletonPtr( )->OnPlayerAddTitleEvent( ( CEntityPlayer* )vpEntity, vTitleID );
		if( vEntityID != 0 )
		{
			return false;
		}
	}
#endif
	return true;
}

int CRepetionModule::SendEventNotice( int vRepetionID, int vEventID, lua_tinker::table vParame )
{
#ifdef _FB_
	CMessageEventNotify tNotify;
	CMessage tMsg;

	tNotify.set_eventid( vEventID );	

	lua_tinker::table vInteger = vParame.get< lua_tinker::table >( "integer" );
	lua_tinker::table vString = vParame.get< lua_tinker::table >( "string" );

	for( int i = 1; i <= vInteger.getn( ); i++ )
	{
		int tInt = vInteger.get< int >( i );	
		tNotify.add_intparam( tInt );
	}

	for( int i = 1; i <= vString.getn( ); i++ )
	{
		char* tStr = vString.get< char* >( i );
		tNotify.add_stringparam( tStr );
	}

	tMsg.set_msgpara( (uint32)&tNotify );
	tMsg.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_EVENT );

	CSceneLogic::GetSingleton().SendWorldMessage( &tMsg );

#endif
	return SUCCESS;
}

const char* CRepetionModule::GetNameByCharID( int vRepetionID, int vCharID )
{
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByCharID( vCharID );
	REPE_ASSERT_STR( tpPlayer != NULL, return NULL, "vEntity %d", vCharID );

	return tpPlayer->GetName( );
}

int CRepetionModule::PlayerLeaveRepetion( CEntityPlayer* vpPlayer, bool vCanBack, bool vDecCount )
{
#ifdef _FB_
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	CPropertyPlayer* tpProperty = ( CPropertyPlayer* )vpPlayer->GetProperty( );
	if( tpProperty == NULL )
	{
		return ERR_INVALID_PARAMETER;
	}

	if( vpPlayer->IsAlive( ) == false )
	{
		CPropertyModule::GetSingletonPtr( )->PlayerRelive( vpPlayer, RELIVE_WEAKGRAVE, 100, 100 );
	}

	CPlayerRepetion* tpPlayerRepe = tpProperty->GetRepetion( );

	CRepetion* tpRepetion = GetRepetion( vpPlayer );
	if( vDecCount == true )
	{
		if( tpRepetion != NULL )
		{
			tpPlayerRepe->DecRepetionCount( tpRepetion->GetRepetionIndex( ) );
		}
	}

	if( vCanBack == true )
	{
		tpPlayerRepe->SetOldRepeKey( INVITED_KEY_VALUE );		
	}
	
	if ( tpRepetion != NULL )
	{
		
		/*lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );

		vParame.set( "entityid", vpPlayer->get_id() );
		vParame.set( "playerlevel",vpPlayer->GetLevel());
		vParame.set( "playername", vpPlayer->GetName());
		vParame.set( "playercountryid",vpPlayer->GetCountryID() );
		vParame.set( "playermapid",vpPlayer->GetMapID());
		PERF_FUNC( "Repetion_DoEvent", tpRepetion->DoEvent( tpRepetion->GetRepetionInfo( ), REPETION_LEAVE_EVENT, &vParame ) );*/
	}
	if ( IS_DUNGEONSERVER_LINE( CSceneLogic::GetSingleton().GetLineID() ) == true )
	{		
		CGlobalMapData tMapData;
		tMapData.mLineID = tpProperty->GetRepetion( )->GetBackLineID( );
		tMapData.mMapID = tpProperty->GetRepetion()->GetBackMapID();
		tMapData.mPosX = tpProperty->GetRepetion()->GetPosX();
		tMapData.mPosY = tpProperty->GetRepetion()->GetPosY();
		CGlobalModule::GetSingleton().GlobalTelePort( vpPlayer, EGLOBALTELE_TYPE_RETURNTOGAME, ESERVERTYPE_DUNGEON, &tMapData );
	}
	else
	{
		CMapModule::GetSingletonPtr( )->Teleport(	tpProperty->GetRepetion( )->GetBackLineID( ), 
			tpProperty->GetRepetion( )->GetBackMapID( ),
			tpProperty->GetRepetion( )->GetPosX( ),
			tpProperty->GetRepetion( )->GetPosY( ),
			vpPlayer,
			vpPlayer->GetMapIndex( )
			);
	}	
	
#endif
	return SUCCESS;
}

/* --------------------------------------------------------------------------*/
/* add by yucx  2010年 08月 30日 星期一 21:03:03
 * @Synopsis 处理组队模式对固定时间开放的副本的报名
 * 玩家进入固定时间开放的副本的等待队列， 比如大逃杀
 *            
 * @Param vpPlayer  
 * @Param vBattleID  副本索引
 * @Param vpNpc
 * 
 * @Returns int 
 */
/* ----------------------------------------------------------------------------*/
int CRepetionModule::TeamFixWaitBattle( CEntityPlayer* vpPlayer, int vBattleID, CEntityNpc* vpNpc )
{
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	REPE_ASSERT_STR( vBattleID < MAP_INDEX_MAX, return ERR_INVALID_PARAMETER, "BattleID %d, Max MapIndex %d", 
		vBattleID, MAP_INDEX_MAX );
	int tPlayerCharID[ TEAM_NUMBER ] = { 0 };
	int tRtn = SUCCESS;
	int tPlayerCount = 0;
	bool tCheck = true;

	REPE_ASSERT( vpPlayer != NULL, return  ERR_INVALID_PARAMETER );

	CRepetionInfo* tpRepetionInfo = GetRepetionInfo( vBattleID );
	REPE_ASSERT( tpRepetionInfo != NULL, return ERR_INVALID_PARAMETER );
	if( tpRepetionInfo->IsBattle( ) == false )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Map( %d ) Is Not A Repetion Map", __LK_FILE__, __LINE__,
				__FUNCTION__, vBattleID );
		return ERR_UNKNOWN;
	}

	if( NotInWaitRedirMap( vpPlayer ) == false )
	{
		return ERR_UNKNOWN;
	}

        //当副本需求队伍同时进入的时候
	if( tpRepetionInfo->IsTeamIn( ) == true )
	{
		int tTeamID = CTeamModule::GetSingletonPtr( )->InTeam( vpPlayer->GetCharID() );
		
		if( tTeamID == 0)
		{
			SendErrCode( vpPlayer, ERR_REPETION_ISNOT_HEADER, vBattleID );
			return ERR_REPETION_ISNOT_HEADER;
		}

		CEntityPlayer* tpHeader = CTeamModule::GetSingletonPtr( )->GetTeamHeader( vpPlayer );
		if( tpHeader != NULL && tpHeader != vpPlayer )
		{
			SendErrCode( vpPlayer, ERR_REPETION_ISNOT_HEADER, vBattleID );
			return ERR_REPETION_ISNOT_HEADER;
		}
		else if( tpHeader == NULL )
		{
			SendErrCode( vpPlayer, ERR_REPETION_ISNOT_HEADER, vBattleID );
			return ERR_REPETION_ISNOT_HEADER;
		}
		tRtn = CTeamModule::GetSingletonPtr( )->OnGetMemberList( vpPlayer->GetCharID( ), tPlayerCharID, 
			ARRAY_CNT( tPlayerCharID ), tPlayerCount );
		if ( tRtn < 0 )
		{
			LOG_ERROR("team", "repetion get team list failed.id=%d,[%s:%d]", vpPlayer->GetCharID(), __LK_FILE__, __LINE__);
			return ERR_UNKNOWN;
		}	
	}
	else
	{
		tPlayerCharID[ 0 ] = vpPlayer->GetCharID( );
		tPlayerCount = 1;
                //这应该返回错误,传向客户端一个不在队伍里的错误消息
	}	

	tCheck = RedirectRepetionCheck( vpPlayer, tPlayerCharID, tPlayerCount, tpRepetionInfo, vpNpc ,0, NULL, 1);
	if( tCheck != true )
	{
		return ERR_UNKNOWN;
	}	

	//TODO:	

	CMessageBattleTeamFixWait tFixWaitMsg;
	CMessage tMsg;

	tFixWaitMsg.set_charid( vpPlayer->GetCharID( ) );
	tFixWaitMsg.set_battleid( vBattleID );
	tFixWaitMsg.set_level( vpPlayer->GetTeamLevel( ) );

	tMsg.mutable_msghead()->set_messageid( ID_S2G_BATTLE_TEAM_FIX_WAIT	);
	tMsg.set_msgpara( (uint32)&tFixWaitMsg );

	CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMsg );

	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Player( %s : %d ) Team Request Fix Wait Battle %d", __LK_FILE__, __LINE__, 
		__FUNCTION__, vpPlayer->GetName( ), vpPlayer->GetCharID( ), vBattleID );
	
	return SUCCESS;
}

 /* --------------------------------------------------------------------------*/
 /* add by yucx  2010年 08月 30日 星期一 21:03:03
   * @Synopsis 处理单人模式对固定时间开放的副本的报名
   * 玩家进入固定时间开放的副本的等待队列， 比如大逃杀
   *            
   * @Param vpPlayer  
   * @Param vBattleID  副本索引
   * @Param vpNpc
   * 
   * @Returns   
   */
 /* ----------------------------------------------------------------------------*/
int CRepetionModule ::SingleFixWaitBattle( CEntityPlayer* vpPlayer, int vBattleID, CEntityNpc* vpNpc  )
{

	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	REPE_ASSERT_STR( vBattleID < MAP_INDEX_MAX, return ERR_INVALID_PARAMETER, "BattleID %d, Max MapIndex %d", 
		vBattleID, MAP_INDEX_MAX );

	int tPlayerCharID =  0 ;
	
	int tPlayerCount = 0;
	bool tCheck = true;

	REPE_ASSERT( vpPlayer != NULL, return  ERR_INVALID_PARAMETER );

	CRepetionInfo* tpRepetionInfo = GetRepetionInfo( vBattleID );
	REPE_ASSERT( tpRepetionInfo != NULL, return ERR_INVALID_PARAMETER );

	if( tpRepetionInfo->IsBattle( ) == false )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ]Map( %d ) Is Not A Repetion Map", __LK_FILE__, __LINE__,
				__FUNCTION__, vBattleID );
		return ERR_UNKNOWN;
	}

	if( NotInWaitRedirMap( vpPlayer ) == false )
	{
		return ERR_UNKNOWN;
	}

        tPlayerCharID = vpPlayer->GetCharID( );
        tPlayerCount = 1;

	tCheck = RedirectRepetionCheck( vpPlayer, &tPlayerCharID, tPlayerCount, tpRepetionInfo, vpNpc ,0,  NULL, 1);
	if( tCheck != true )
	{
		return ERR_UNKNOWN;
	}	

	//TODO:	

	CMessageBattleSingleFixWait tFixWaitMsg;
	CMessage tMsg;

	tFixWaitMsg.set_charid( vpPlayer->GetCharID( ) );
	tFixWaitMsg.set_battleid( vBattleID );
	tFixWaitMsg.set_level( vpPlayer->GetTeamLevel( ) );

	tMsg.mutable_msghead()->set_messageid(ID_S2G_BATTLE_SINGLE_FIX_WAIT );
	tMsg.set_msgpara( (uint32)&tFixWaitMsg );

	CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMsg );

	LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Player( %s : %d ) Single Request Fix Wait Battle %d", __LK_FILE__, __LINE__, 
		__FUNCTION__, vpPlayer->GetName( ), vpPlayer->GetCharID( ), vBattleID );
	
	return SUCCESS;
}

/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 玩家离开副本 
* 
* @Param vRepetionID
* @Param vEntityID
*/
/* ----------------------------------------------------------------------------*/

void CRepetionModule::PlayerLeave( unsigned int vRepetionID, unsigned int vEntityID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	CEntityPlayer* tpPlayer= ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpPlayer != NULL, return, "EntityID %d", vEntityID );

    CRepetionModule::GetSingletonPtr()-> PlayerLeaveRepetion( tpPlayer );
#endif
}

/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 把玩家踢出队伍 
* 
* @Param vEntityID
* 
* @Returns   
*/
/* ----------------------------------------------------------------------------*/

void CRepetionModule:: KickPlayerFromTeam( unsigned int vEntityID)
{
#ifdef _FB_
	CEntityPlayer* tpPlayer= ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpPlayer != NULL, return, "EntityID %d", vEntityID );

        int tTeamID = CTeamModule::GetSingleton().InTeam( tpPlayer->GetCharID());
        if(INVALID_OBJ_ID == tTeamID)
        {
            return ;
        }

        CMessage tMessage;
        CMessageLeaveGateTeamRequest tPara;
        tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_LEAVETEAM );
        tPara.set_charid( tpPlayer->GetCharID() );
        tMessage.set_msgpara( (long) &tPara );
        CSceneLogic::GetSingleton().Send2Gate( &tMessage );     

        // 清除下玩家的内丹状态
        CPropertyModule::GetSingleton().ChangeTeamLeaverStatus( tpPlayer );
        
        return; 

#endif
}
//* --------------------------------------------------------------------------*/
//*                                                     
//* @Synopsis   给玩家 加状态                                                                               *     
//* @Param  vState 状态值                                    
//* @Param  vFrom 添加形式 
//*  
//* @Returns                                             
//*/
//* ----------------------------------------------------------------------------*/

void CRepetionModule::AttachPlayerState(unsigned int vEntityID, EState vState, int vParames1, int vParames2, int vIndex )
{
#ifdef _FB_
	CEntityPlayer* tpPlayer= ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpPlayer != NULL, return, "EntityID %d", vEntityID );
	CPropertyPlayer *pProperty = ( CPropertyPlayer * )tpPlayer->GetProperty();

	pProperty->AttachState(vState, vParames1, vParames2, vIndex, NONE);
	
	return ;

#endif
}

/* --------------------------------------------------------------------------*/
/*                                                     
* @Synopsis 把玩家传入副本的一个随机位置 坐标在lua脚本里配置                                                                                 *     
* @Param vpPlayer                                      
* @Param   vpInfo
*  
* @Returns                                             
*/
/* ----------------------------------------------------------------------------*/

int CRepetionModule:: PlayerIntoRepetionRandomPlace( CEntityPlayer* vpPlayer, 
                                                CRepetionInfo *vpInfo,
                                                int vKey)
{
	REPE_ASSERT( vpPlayer != NULL && vpInfo != NULL, return ERR_INVALID_PARAMETER );

        lua_tinker::table ret = lua_tinker::call<lua_tinker::table , int > (mLua, "get_random_place",vpInfo->GetRepetionIndex() );

        int nDstPostX = ret.get<int>("x");
        int nDstPostY = ret.get<int>("y");

		LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] player to repetion's random post is ( %d, %d )",
			__LK_FILE__, __LINE__, __FUNCTION__, nDstPostX, nDstPostY );

        //RedirectRepetionMap( vpPlayer, vpInfo->GetBeginMapID( ), nDstPostX , nDstPostY );
		PlayerIntoRepetion( vpPlayer, vpInfo->GetBeginMapID( ), nDstPostX , nDstPostY , vKey);
		return SUCCESS;
}
/* --------------------------------------------------------------------------*/
/*                                                     
* @Synopsis 给玩家设置数据                                                                              *     
* @Param vpPlayer                                      
* @Param   vpInfo
*  
* @Returns                                             
*/
/* ----------------------------------------------------------------------------*/
void CRepetionModule::SetIntData2Player( unsigned int vRepetionID,unsigned int vEntityID,  unsigned int vIndex, int vIntData )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "vRepetionID %d", vRepetionID );

	CEntityPlayer* tpPlayer= ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpPlayer != NULL, return, "EntityID %d", vEntityID );

	CRepetion::SetIntData(tpRepetion, tpPlayer,  vIndex,  vIntData );

#endif
	return;
}
/* --------------------------------------------------------------------------*/
/*                                                     
* @Synopsis 给玩家加数据                                                                              *     
* @Param vpPlayer                                      
* @Param   vpInfo
*  
* @Returns                                             
*/
/* ----------------------------------------------------------------------------*/
void CRepetionModule::AddIntData2Player( unsigned int vRepetionID,unsigned int vEntityID,  unsigned int vIndex, int vIntData )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "vRepetionID %d", vRepetionID );

	CEntityPlayer* tpPlayer= ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpPlayer != NULL, return, "EntityID %d", vEntityID );

	CRepetion::AddIntData(tpRepetion, tpPlayer,  vIndex,  vIntData );

#endif
	return;
}

/* --------------------------------------------------------------------------*/
/*                                                     
* @Synopsis 从玩家取数据                                                                              *     
* @Param                                       
* @Param   
*  
* @Returns                                             
*/
/* ----------------------------------------------------------------------------*/
int CRepetionModule::GetIntData2Player( unsigned int vRepetionID,unsigned int vEntityID,  unsigned int vIndex)
{
	int nIntData = 0;
#ifdef _FB_
	
	bool tRet ;
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return -1, "vRepetionID %d", vRepetionID );

	CEntityPlayer* tpPlayer= ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpPlayer != NULL, return -1, "EntityID %d", vEntityID );

	tRet = CRepetion::GetIntData(tpRepetion, tpPlayer,  vIndex,  &nIntData );
	REPE_ASSERT_STR( tRet == true, return -1, "tRet %d", tRet );

#endif
	return nIntData;;
}
/* --------------------------------------------------------------------------*/
/*                                                     
* @Synopsis 从玩家取数据                                                                              *     
* @Param                                       
* @Param   
*  
* @Returns                                             
*/
/* ----------------------------------------------------------------------------*/
bool CRepetion::GetIntData( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vIndex, int * pIntData )
{
#ifdef _FB_
	int tRtn = SUCCESS;


	REPE_ASSERT( vpRepetion != NULL, return true );
	REPE_ASSERT( vpEntity != NULL, return true );
	REPE_ASSERT_STR( vpEntity->IsPlayer( ) == true, return false, "EntityType Is %d", vpEntity->GetEntityType( ) );
	CPropertyPlayer* tpPlayer = ( CPropertyPlayer* )vpEntity->GetProperty( );

	tRtn = tpPlayer->GetRepetion( )->GetIntData( vpRepetion->GetRepetionIndex( ), vIndex, pIntData );
	REPE_ASSERT_STR( tRtn == SUCCESS, return false, "Rtn %d", tRtn );
#endif
	return true;
}

/* --------------------------------------------------------------------------*/
/*                                                     
* @Synopsis  给所有玩家发送 CPlayerRepetion 中的 mIntData                                                                 *     
* @Param1	                                       
* @Param2  event 类型   
* @Param3  vParame1
* @Param4  mIntData索引
* @Returns                                             
*/
/* ----------------------------------------------------------------------------*/

void CRepetionModule::EventNoticeAllInPlayerData(	unsigned int vRepetionID, 
									 REPETION_EVENT_NOTICE vEvent, 
									 int vParame1, 
									 int vIndex 
									 )
{
#ifdef _FB_
	CMessage tMsg;
	CMessageRepetionEventNotic tMsgFact;
	int tData = 0;

	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT( tpRepetion != NULL, return );
	int nReptionIndex = tpRepetion->GetRepetionIndex( );

	CRepetionInfo* tpInfo = tpRepetion->GetRepetionInfo( );
	REPE_ASSERT( tpInfo != NULL, return );

	tMsgFact.set_mapid( tpInfo->GetBeginMapID( ) );
	tMsgFact.set_eventid( vEvent );
	tMsgFact.set_parame1( vParame1 );

	tMsg.mutable_msghead()->set_messageid( ID_S2C_REPETION_EVENT );
	//tMsg.set_msgpara( (uint32)&tMsgFact );

	for( int i = 0; i < tpRepetion->GetMapObjSizeMax( ); i++ )
	{
		CMapObject* tpMapObj = tpRepetion->GetMapObject( i );
		if( tpMapObj == NULL )
		{
			break;
		}

		tpMapObj->SendMsg2Player( &tMsg );	

		CMapObject::PLAYER_VECTOR::iterator it;

		for( it = tpMapObj->mPlayers.begin( ); it !=  tpMapObj->mPlayers.end( ); it++ )
		{
			unsigned int tEntityID = *it;
			CEntityPlayer* tpPlayer = ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( tEntityID );

			LK_ASSERT( tpPlayer != NULL, continue );
			CPropertyPlayer* tpProperty = ( CPropertyPlayer* ) tpPlayer->GetProperty();
			int tRtn = tpProperty->GetRepetion( )->GetIntData( nReptionIndex, vIndex, &tData );
			REPE_ASSERT_STR( tRtn == SUCCESS, return , "Get Rtn %d", tRtn );
			tMsgFact.set_parame2( tData);
			tMsg.set_msgpara( (uint32)&tMsgFact );

			CSceneLogic::GetSingletonPtr( )->Send2Player( tpPlayer, &tMsg );
		}
	}


	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ] Send CMessageRepetionEventNotic To All Player In Repetion( %d : %d ),"
		" EventID : %d , Parame1 : %d,by player data", __LK_FILE__, __LINE__, __FUNCTION__, 
		tpRepetion->GetRepetionIndex( ), tpRepetion->get_id( ), vEvent, vParame1 );
#endif
}


/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 玩家获得经验
* 
* @Param vRepetionID
* @Param vEntityID
*/ 
/* ----------------------------------------------------------------------------*/

void CRepetionModule::ObtainRepetionExp(unsigned int vRepetionID, unsigned int vEntityID, unsigned int vExp )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	CEntityPlayer* tpPlayer= ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpPlayer != NULL, return, "EntityID %d", vEntityID );

	int tCharID = tpPlayer->GetCharID();
	CPropertyModule::GetSingletonPtr( )->PlayerObtainExp( tpPlayer, vExp, EXPREASON_ACTIVITY, &tCharID, 1 );
#endif
}

/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 创建陷阱
* 
* @Param vRepetionID
* @Param vEntityID
*/ 
/* ----------------------------------------------------------------------------*/
void CRepetionModule::CreateNeedfire( unsigned int vRepetionID, unsigned int vTempID, int vPosX, int vPosY, int vMapID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );
	CTemplateTrapSkill* tpTrapTpl = ( CTemplateTrapSkill* )CDataStatic::SearchTpl( vTempID );
	REPE_ASSERT_STR( tpTrapTpl != NULL, return, "TempID %d", vTempID );
	CMapObject* tpMapObj = tpRepetion->GetMapObjectByMapID( vMapID );
	REPE_ASSERT_STR( tpMapObj != NULL, return, "Cannt Get Repetion MapObj" );


	CPedalCreator tPedal;
 
	tPedal.mPedalType	= PEDAL_NEEDFIRE;
	tPedal.mTempID		= tpTrapTpl->mTempID;

	int tTouchRange  = tpTrapTpl->mTouchRange - 1;
	int tActionRange = tpTrapTpl->mApplyRange - 1;
	tPedal.mSrcRect		= CWTRect( vPosX- tTouchRange,vPosY - tTouchRange, vPosX + tTouchRange, vPosY + tTouchRange );
	tPedal.mActRect		= CWTRect( vPosX - tActionRange, vPosY - tActionRange, vPosX + tActionRange, vPosY + tActionRange );
	tPedal.mLineID		= tpMapObj->GetLineID( );
	tPedal.mMapID		= tpMapObj->GetMapID( );
	tPedal.mSkillID		= tpTrapTpl->mSkillID;
	tPedal.mSkillLevel	= tpTrapTpl->mSkillLevel;

	tpMapObj->CreateNpc( &tPedal );
	
#endif
}
/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 发广播消息
* 
* @Param vRepetionID
* @Param vEntityID
*/ 
/* ----------------------------------------------------------------------------*/
void CRepetionModule::SendEventNotify( int vEventID, int vCountryID, lua_tinker::table vTableInt, lua_tinker::table vTableString )
{
#ifdef _FB_
	//暂时设为5 
	int nParam[5] = {0};
	
	if( vTableInt.getn() > 5 )
	{

		LOG_DEBUG( REPETION_LOG, "table length > 5, " );
		return ;
	}

	for(int i = 0 ;i < 5 && i < vTableInt.getn(); ++i)
	{
		nParam[i] = vTableInt.get< int >( i );

	}
	CPropertyModule::GetSingleton().SendEventNotify( vEventID, vCountryID, nParam, vTableInt.getn(), vTableString.get< char *>( 1 ), vTableString.get< char * >( 2 ));
#endif
}

/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 动态创建npc
* 
* @Param vRepetionID
* @Param vEntityID
*调用CPropertyModule 接口
*/ 
/* ----------------------------------------------------------------------------*/
void CRepetionModule:: PlayerCreateNpcDynamic(  unsigned int vEntityID , unsigned int vTempID,unsigned int vEntityType, int vPosX,int vPosY )
{
#ifdef _FB_

	CEntityPlayer* tpPlayer= ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpPlayer != NULL, return, "EntityID %d", vEntityID );


	CPropertyModule::GetSingleton().CreateNpc( tpPlayer, vTempID, ( EEntityType ) vEntityType, tpPlayer->GetPos());

#endif

}
/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 判断是否在报名时间
* 
* @Param vRepetionID
* @Param vEntityID
*
*/ 
/* ----------------------------------------------------------------------------*/
bool CRepetionInfo:: IsInNowSignUpTime( int vTime )
{
	//是否用 signupcheck ?
	if( CSceneCfgManager::GetSingletonPtr( )->GetRepetion_cfg( )->mTimecheck == 0 )
	{
		return true;
	}
	
	time_t tvTime = ( time_t )vTime;
	
	struct tm* tpTm = localtime( &tvTime );
	REPE_ASSERT( tpTm != NULL, return false );
	struct tm vTm ;
	memcpy( &vTm, tpTm,sizeof(struct tm));


	time_t tTime = time( NULL );
	struct tm *tTm = localtime( &tTime) ;
	REPE_ASSERT( tTm != NULL, return false );
	
	if( tTm->tm_year != vTm.tm_year || tTm->tm_mon != vTm.tm_mon || tTm->tm_mday != vTm.tm_mday)
	{
		return false;
	}
	DATA_SPACE_LIST::iterator it;

	for( it = mSignUpList.begin( ); it != mSignUpList.end( ); it++ )
	{
		DateSpace tSignUpList= *it;

		if( tSignUpList.InDateSpace( tTm ) == true )
		{

			if( tSignUpList.InDateSpace( &vTm ) == true )
			{
				return true;
			}
		}
	}

	return  false;
}


/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 判断是否在副本开放时间
* 
* @Param vRepetionID
* @Param vEntityID
*
*/ 
/* ----------------------------------------------------------------------------*/
bool CRepetionInfo:: IsInNowOpenTime( int vTime )
{
	////是否用 signupcheck ?
	//if( CSceneCfgManager::GetSingletonPtr( )->GetRepetion_cfg( )->mTimecheck == 0 )
	//{
	//	return true;
	//}

	time_t tvTime = ( time_t )vTime;

	struct tm* tpTm = localtime( &tvTime );
	REPE_ASSERT( tpTm != NULL, return false );
	struct tm vTm ;
	memcpy( &vTm, tpTm,sizeof(struct tm));


	time_t tTime = time( NULL );
	struct tm *tTm = localtime( &tTime) ;
	REPE_ASSERT( tTm != NULL, return false );

	if( tTm->tm_year != vTm.tm_year || tTm->tm_mon != vTm.tm_mon || tTm->tm_mday != vTm.tm_mday)
	{
		return false;
	}
	DATA_SPACE_LIST::iterator it;

	for( it = mDateSpace.begin( ); it != mDateSpace.end( ); it++ )
	{
		DateSpace tDateSpace = *it;

		if( tDateSpace.InDateSpace( tTm ) == true )
		{

			if( tDateSpace.InDateSpace( &vTm ) == true )
			{
				return true;
			}
		}
	}

	return  false;
}

/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 单个玩家系统复活
* 
* @Param vRepetionID
* @Param vEntityID
*/ 
/* ----------------------------------------------------------------------------*/

void CRepetionModule::PlayerReliveBySys( unsigned int vRepetionID, unsigned int vEntityID ,int vHPPer, int vMPPer )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	CEntityPlayer* tpPlayer= ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpPlayer != NULL, return, "EntityID %d", vEntityID );


	if( tpPlayer->IsAlive( ) == false )
	{

		CPropertyModule::GetSingletonPtr( )->PlayerRelive( tpPlayer, RELIVE_SYSTEM, vHPPer, vMPPer );
	}


#endif
}

/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 处理对战副本获胜者
* 
* @Param vRepetionID
* @Param vHasWinner 1 有获胜方  0  无获胜方
* @Param vEntityID
*/ 
/* ----------------------------------------------------------------------------*/

void CRepetionModule:: DealBattleWinner( unsigned int vRepetionID,unsigned int vHasWinner, unsigned int vLevel, unsigned int vEntityID)
{
//#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	
	CMessageBattleWinnerNotify tWinnerNotify;
	CMessage tMsg;

	switch( tpRepetion->GetRepetionIndex())
	{
	case _REPE_LASTONE_INDEX_:  //处理大逃杀获胜者
		{
			if( vHasWinner > 0 )
			{
				CEntityPlayer* tpPlayer= ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
				REPE_ASSERT_STR( tpPlayer != NULL, return, "EntityID %d", vEntityID );

				tWinnerNotify.set_haswinner( vHasWinner );
				tWinnerNotify.set_charid( tpPlayer->GetCharID( ) );
				tWinnerNotify.set_battleid( tpRepetion->GetRepetionIndex() );
				tWinnerNotify.set_level( vLevel);

				LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ]  send battle winner %d Player( %s : %d )", __LK_FILE__, __LINE__, 
					__FUNCTION__,vRepetionID , tpPlayer->GetName( ), tpPlayer->GetCharID( ) );

			}else{
				tWinnerNotify.set_haswinner( vHasWinner );
				tWinnerNotify.set_charid( 0 );
				tWinnerNotify.set_battleid( tpRepetion->GetRepetionIndex() );
				tWinnerNotify.set_level( vLevel );
			}

		}
		break;
	default:
		{

		}
	}

	tMsg.mutable_msghead()->set_messageid( ID_S2G_REPETION_BATTLE_WINNER_NOTIFY );
	tMsg.set_msgpara( (uint32)&tWinnerNotify );

	CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMsg );


//#endif
}


/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 发系统邮件
* 
* @Param vRepetionID
* @Param vEntityID
*/ 
/* ----------------------------------------------------------------------------*/

// 发系统邮件
void CRepetionModule::SendSysMailForRepetion( unsigned int vRepetionID, lua_tinker::table param )
{


	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	int nItemIDs[5] = { param.get<int>("item_id1"), param.get<int>("item_id2"), param.get<int>("item_id3")
		, param.get<int>("item_id4"), param.get<int>("item_id5") };

	int nItemNums[5] = { param.get<int>("item_num1"), param.get<int>("item_num2"), param.get<int>("item_num3")
		, param.get<int>("item_num4"), param.get<int>("item_num5") };

	int nTotalNum = 0;
	for ( int i = 0; i < 5; ++i )
	{
		if ( nItemIDs[i] == 0 )
			break;
		++nTotalNum;
	}
	
	
	CEntityPlayer* tpPlayer = ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( param.get<int>("entity_id") );

	REPE_ASSERT( tpPlayer != NULL, return  );

	CMailModule::GetSingleton().SendSysMail( tpPlayer->GetCharName(), tpPlayer->GetCharID()
		, param.get<char*>("title"), param.get<char*>("id"), param.get<char*>("body"), nItemIDs, nItemNums, nTotalNum
		, tpPlayer->GetCampID(), param.get<int>("money") ) ;

	return ;
}

lua_tinker::table CRepetionModule::GetRepetionPlayerID(  unsigned int vRepetionID )
{
	lua_tinker::table tTable( CRepetionModule::GetSingletonPtr( )->mLua );
//#define _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return tTable, "RepetionID %d", vRepetionID );

	int index = 0;
	char index_char[ 128 ] = {0}; 
	for( int i = 0 ; i < tpRepetion->GetMapObjSizeMax( ); ++i)
	{
		CMapObject * tpMapObj = tpRepetion->GetMapObject( i );
		if(  NULL == tpMapObj )
		{
			break;
		}

		for( int j = tpMapObj->mPlayers.size() - 1; j>= 0; j-- )
		{
			CEntityPlayer* tpPlayer = ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( tpMapObj->mPlayers[ j ] );

			REPE_ASSERT( tpPlayer != NULL, continue );
			index++;
			memset( index_char, 0 ,sizeof( index_char ) );
			snprintf( index_char , sizeof(index_char) - 1, "%d", index);
		
			tTable.set( index_char, tpPlayer->GetEntityID() );
		}
	}
//#endif
	return tTable;

}

void CRepetionModule::NpcRefreshView( int vRepetionID, int vTempID, int vMapID )
{
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return , "RepetionID %d", vRepetionID );
	
	if( vMapID == INVALID_MAP_ID )
	{
		tpRepetion->NpcEntityLoop( vTempID, CRepetion::RefreshNpcView, 0, 0 );
	}
	else
	{
		tpRepetion->NpcEntityLoopByMap( vTempID, CRepetion::RefreshNpcView, vMapID, 0, 0 );
	}
}

bool CRepetion::RefreshNpcView( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vParame1, int vParame2 )
{
    CWTPoint tTopLeft;
    CWTPoint TBottomRight;

	CMapObject* tpMapObj = vpEntity->GetMapObject( );
	REPE_ASSERT_STR( tpMapObj != NULL, return true, "Cannt Get  Map %d, RepetionIndex %d", vpEntity->GetMapID( ),
		vpRepetion->GetRepetionIndex( ) );
	
	if( vpEntity->GetNpcID( ) == true )
	{	
		( ( CEntityNpc* )vpEntity )->GetSeenNpcsContainer( ).Initailize( );
	}

    tpMapObj->GetViewRect( vpEntity, tTopLeft, TBottomRight );

	VisibleViewPlayerChecker tVisibleChecker( vpEntity );
	MapRegionEntitySearcher< VisibleViewPlayerChecker > tMapRegionSearcher( tVisibleChecker, tTopLeft, TBottomRight );
	tMapRegionSearcher.Visit( tpMapObj );	

	return true;
}

int CRepetionModule::CallFun( int vRepetionID, lua_tinker::table vParame )
{
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
    REPE_ASSERT_STR( tpRepetion != NULL, return SUCCESS, "RepetionID %d", vRepetionID );	

	PERF_FUNC( "Repetion_DoEvent", tpRepetion->DoEvent( tpRepetion->GetRepetionInfo( ), 
		REPETION_CALL_FUN, &vParame ) );

	return SUCCESS;
}

bool CRepetionModule::CharacterHasBuf( int vRepetionID, int vEntityID, int vBuffID )
{
	CEntity* tpEntity = CSceneLogic::GetSingletonPtr( )->GetEntity( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return false, "Cannt GetEntity EntityID %d", vEntityID );
	
	if( tpEntity->IsCharacter( ) == false )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %d ] Entity %d Is Not A Character, Type %d", __LK_FILE__, __LINE__,
			__FUNCTION__, vEntityID, tpEntity->GetEntityType( ) );
		return false;
	}

	return ( ( CEntityCharacter* )tpEntity )->EntityHasBuff( vBuffID );
}

void CRepetionModule::NpcLoop4TempInLuaByMapID( int vRepetionID, int vTempID, int vMapID, lua_tinker::table vParame )
{
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, " RepetionID %d", vRepetionID );
	tpRepetion->NpcEntityLoopInLuaByMapID( vTempID, vMapID, &vParame );
	return;
}

int CRepetion::NpcEntityLoopInLuaByMapID( unsigned int vTempID, int vMapID, lua_tinker::table* vParame )
{
#ifdef _FB_

	for( int j = 0; j < ( int )ARRAY_CNT( mMapObjIDList ); j++ )
	{
		if( mMapObjIDList[ j ].mMapObjID == INVALID_OBJ_ID )
		{
			break;
		}
		CMapObject* tpMapObj = GetMapObject( j );

		if ( tpMapObj == NULL )
		{
			break;
		}

		if( vMapID != INVALID_MAP_ID && tpMapObj->GetMapID( ) != vMapID )
		{
			continue;
		}

		for( int i = tpMapObj->mEntity.size( ) - 1; i >= 0; i-- )
		{
			CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( tpMapObj->mEntity[ i ] );

			REPE_ASSERT( tpEntity != NULL, continue );

			if( tpEntity->IsNpc() == false )
			{
				continue;	
			}

			if( vTempID != 0 && ( tpEntity->GetNpcID( ) != vTempID ) )
			{
				continue;
			}

			CEntityNpc* tpNpc = ( CEntityNpc* )tpEntity;

			CCreator* tpCreator = ( CCreator* )CSceneObjManager::GetSingletonPtr( )->GetObject( 
				tpNpc->GetCreatorID( ) );

			vParame->set< int >( "index", ( tpCreator != NULL ) ? tpCreator->GetIndex( ) : 0 );
			vParame->set< int >( "entityid", tpEntity->GetEntityID( ) );
			vParame->set< int >( "posx", tpEntity->GetPosX( ) );
			vParame->set< int >( "posy", tpEntity->GetPosY( ) );
			vParame->set< int >( "temp_id", tpNpc->GetNpcID( ) );
				
			PERF_FUNC( "Repetion_DoEvent", DoEvent( GetRepetionInfo( ), REPETION_CALL_FUN, vParame ) );

			int tRet = vParame->get<int>( "result" );
			if( tRet != 0 )
			{
				return SUCCESS;
			}
		}
	}
#endif
	return SUCCESS;
}

int CRepetion::PlayerLoopInLua( int vMapID, lua_tinker::table* vParame )
{
#ifdef _FB_

	for( int j = 0; j < ( int )ARRAY_CNT( mMapObjIDList ); j++ )
	{
		if( mMapObjIDList[ j ].mMapObjID == INVALID_OBJ_ID )
		{
			break;
		}

		CMapObject* tpMapObj = GetMapObject( j );

		if( tpMapObj == NULL )
		{
			break;
		}

		if( vMapID != INVALID_OBJ_ID && vMapID != tpMapObj->GetMapID( ) )
		{
			continue;
		}

		for( int i = tpMapObj->mPlayers.size( ) - 1; i >= 0; i-- )
		{
			CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( tpMapObj->mPlayers[ i ] );

			REPE_ASSERT( tpEntity != NULL, continue );

			if( tpEntity->IsPlayer()  == false )
			{
				continue;	
			}

			vParame->set< int >( "entityid", tpEntity->GetEntityID( ) );
				
			PERF_FUNC( "Repetion_DoEvent", DoEvent( GetRepetionInfo( ), REPETION_CALL_FUN, vParame ) );

			int tRet = vParame->get<int>( "result" );
			if( tRet != 0 )
			{
				return SUCCESS;
			}

		}
	}

#endif
	return SUCCESS;
}

void CRepetionModule::PlayerLoopInLuaByMapID( int vRepetionID, int vMapID, lua_tinker::table vTable )
{
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, " RepetionID %d", vRepetionID );
	tpRepetion->PlayerLoopInLua( vMapID, &vTable );

	return;
}


/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 玩家离开副本
* 
* @Param vRepetionID
* @Param vMapID
*/ 
/* ----------------------------------------------------------------------------*/
int CRepetionModule::GetMapID( int vRepetionID, int vEntityID )
{
	int tMapID = INVALID_MAP_ID;
#ifdef _FB_
	CEntity* tpEntity = CSceneLogic::GetSingletonPtr( )->GetEntity( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return tMapID, "Repetion %d, EntityID %d", vRepetionID, vEntityID );
	tMapID = tpEntity->GetMapID( );
#endif
	return tMapID;
}

int CRepetionModule::GetMapObjectID( int vRepetionID, int vEntityID )
{
	int tMapObjID = INVALID_OBJ_ID;
#ifdef _FB_
	CEntity* tpEntity = CSceneLogic::GetSingletonPtr( )->GetEntity( vEntityID );
	REPE_ASSERT_STR( tpEntity != NULL, return tMapObjID, "Repetion %d, EntityID %d", vRepetionID, vEntityID );
	tMapObjID = tpEntity->GetMapObjectID( );
#endif
	return tMapObjID;

}


// ***********************************************************
//  Function:		EndGlobalWar
//  Description:	全服战场结束通知 				
//  Output:			
//  Date:	04/12/2011
// **********************************************************
void CRepetionModule::EndGlobalWar( int vRepetionID, lua_tinker::table vTable )
{
#ifdef _FB_
	CRepetion *tpRepetion = GetRepetion( vRepetionID );
	if ( tpRepetion != NULL )
	{
		CGlobalModule::GetSingleton().EndGlobalWar( EREASON_ENDWAR_BATTLEEND, tpRepetion->GetParame( 0 ) );
	}
#endif
}

void CRepetionModule::PlayerLeaveByMapID( int vRepetionID, int vMapID )
{
#ifdef _FB_


	CRepetion* tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoopByMap( CRepetion::LeaveRepetion, vMapID, 0, 0 );

#endif
}


/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 特殊传送
* 
* @Param pPlayer
* @Param messge
*/ 
/* ----------------------------------------------------------------------------*/
void CRepetionModule::OnMessageRandomTele( CEntityPlayer *pPlayer,CMessage * pMessage )
{
#ifdef _FB_
	REPE_ASSERT_STR( pPlayer != NULL || pMessage != NULL, return,"ERR_INVALID_PARAMETER");
	CMessageC2SGetRandomTeleChargeRequest *tpRandomTele = (CMessageC2SGetRandomTeleChargeRequest *)pMessage->msgpara( );

	REPE_ASSERT_STR( tpRandomTele != NULL, return, "tpRandomTele is NULL");

	CPropertyPlayer* tpSrcProperty = (CPropertyPlayer*)pPlayer->GetProperty();

	// check service status
	// just now, confirm that it can't teleport while in trade or stall service
	if ( tpSrcProperty->CanTeleport() == false )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_INSERVICE );
		return ;
	}	

	int nDstLineID = 0;
	int nDstMapID = 0;
	int nDstPosX = 0;
	int nDstPosY = 0;
	int nMoneyNeed = 0;
	int tNeedTempID = 0;
	CEntityNpc* pNpc = NULL;


	if( tpRandomTele->npcid() != 0 )
	{
		pNpc = dynamic_cast< CEntityNpc* >( CSceneObjManager::GetSingletonPtr( )->GetObject( 
			tpRandomTele->npcid( ) ) );
		if ( pNpc == NULL )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TELENONPC );
			return;
		}
		if( !pPlayer->IsWithinDistance( pNpc, MAX_NPC_TALK_GIRD) ) // 在合法距离之内才判断
		{

			LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] player is not near npc", __LK_FILE__, __LINE__, __FUNCTION__ );
			return;
		}

		CTemplateNpc *tpNpc = (CTemplateNpc*)CDataStatic::SearchTpl( pNpc->GetNpcID( ) );
		if ( tpNpc == NULL || tpNpc->mRandomTeleService == 0 )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TELENONPC );
			return;
		}



		CTemplateReliveTable* tpFeeTable = ( CTemplateReliveTable* ) CDataStatic::GetTemp( TEMP_RELIVETABLE );
		if ( tpFeeTable == NULL )
		{
			LOG_ERROR( REPETION_LOG, "[ %s : %d ] [ %s ] cannt find template TEMPTYPE = %d", __LK_FILE__, __LINE__, __FUNCTION__, TEMP_RELIVETABLE );
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TELENONPC );
			return ;
		}

		lua_tinker::table ret = lua_tinker::call<lua_tinker::table , int > (mLua, "get_tele_charge",tpNpc->mRandomTeleService,pPlayer->GetEntityID( ) );

		// 检查金币+绑定金钱是否足够传送
		unsigned int tBaggageMoney = ( unsigned int )( tpSrcProperty->GetMoney() );


		nMoneyNeed = ret.get< float >("charge") * ((unsigned int)tpFeeTable->mFee[ pPlayer->GetLevel() ]);

		lua_tinker::table nPost = ret.get< lua_tinker::table >( "post" );

		nDstPosX = nPost.get< int>( "x" );
		nDstPosY = nPost.get< int>( "y" );
		nDstMapID = ret.get< int >( "mapid" );

		if ( tBaggageMoney < nMoneyNeed )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TELENOFEE );
			return;
		}	

		pPlayer->SetCreatMethod( PLAYERCREAT_TELEPORT ); 

		// 先扣除费用和道具
		if( nMoneyNeed  > 0 )
		{
			if( CPropertyModule::GetSingleton().PlayerChangeMoney( pPlayer, nMoneyNeed, true, false ) == 0 ) 
			{
				LogEventLostMoneyByNpcTeleport(pPlayer, CYuanBao::em_unbind_money, nMoneyNeed, pNpc != NULL ? pNpc->GetNpcID( ) : 0); 		
			}
			else
			{
				CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TELENOFEE  );
				return;
			}
		}

	}else
	{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TELENONPC );
			return;
	}

	//int tRtn = CRepetionModule::GetSingletonPtr( )->RedirectRepetionMap( pPlayer, nDstMapID, nDstPosX, nDstPosY, pNpc );
	

	CMapObject* tpMapObj = pPlayer->GetMapObject( );
	if( tpMapObj == NULL )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Find Player( %d : %d : %s ) Map( %d : %d : %d )",
			__LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetEntityID( ), pPlayer->GetCharID( ),
			pPlayer->GetCharNameShow( ), pPlayer->GetLineID( ), pPlayer->GetMapID( ), pPlayer->GetMapIndex( ));
		return;
	}
	bool tRtn = CMapModule::GetSingleton().Teleport(nDstLineID,nDstMapID,nDstPosX,nDstPosY,pPlayer,
		INVALID_MAP_INDEX,
		( tpMapObj != NULL ) ? tpMapObj->GetKey( ) : INVITED_KEY_VALUE );


	if( tRtn != true )
	{
		// 传送失败，金钱回滚
		if( nMoneyNeed > 0 )
		{
			CPropertyModule::GetSingleton().PlayerChangeMoney( pPlayer, nMoneyNeed, false, false );
			LogEventGetMoneyByTeleErr(pPlayer, nMoneyNeed ); 
		}

		//// 传送失败，道具回滚
		//if( tNeedTempID > 0 )
		//	PlayerInsertItem( pPlayer, tNeedTempID , 1 );
	}


	// 修改创建方式为移动
	pPlayer->SetCreatMethod( PLAYERCREAT_MOVE );
#endif
}


int CRepetionModule::GetPlayerDieCountInRepetion( unsigned int vRepetionIndex, unsigned int  vEntityID )
{
#ifdef _FB_
		CEntityPlayer* tpPlayer = ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );

		CPropertyPlayer* pProperty = (CPropertyPlayer*) tpPlayer->GetProperty( );
		REPE_ASSERT_STR( pProperty != NULL, return ERR_INVALID_PARAMETER, "vRepetionIndex is %d",vRepetionIndex);

		CPlayerRepetion* tpPRepetion = pProperty->GetRepetion( );
		REPE_ASSERT_STR( tpPRepetion != NULL, return ERR_INVALID_PARAMETER, "tpPRepetion is %d", vRepetionIndex);

		return tpPRepetion->GetDieCount( vRepetionIndex );
#endif
	return 0;
}



void CRepetionModule::OnMessageGetRandomTeleCharge( CEntityPlayer *pPlayer,CMessage * pMessage )
{
#ifdef _FB_
	REPE_ASSERT_STR( pPlayer != NULL || pMessage != NULL, return,"ERR_INVALID_PARAMETER");
	CMessageC2SGetRandomTeleChargeRequest *tpRandomTele = (CMessageC2SGetRandomTeleChargeRequest *)pMessage->msgpara( );

	REPE_ASSERT_STR( tpRandomTele != NULL, return, "tpRandomTele is NULL");

	CPropertyPlayer* tpSrcProperty = (CPropertyPlayer*)pPlayer->GetProperty();

	// check service status
	// just now, confirm that it can't teleport while in trade or stall service
	if ( tpSrcProperty->CanTeleport() == false )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_INSERVICE );
		return ;
	}	

	int nMoneyNeed = 0;
	CEntityNpc* pNpc = NULL;

	pNpc = dynamic_cast< CEntityNpc* >( CSceneObjManager::GetSingletonPtr( )->GetObject( 
		tpRandomTele->npcid( ) ) );
	if ( pNpc == NULL )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TELENONPC );
		return;
	}
	if( !pPlayer->IsWithinDistance( pNpc, MAX_NPC_TALK_GIRD) ) // 在合法距离之内才判断
	{

		LOG_ERROR( REPETION_LOG, "[ %s : %d ][ %s ] player is not near npc", __LK_FILE__, __LINE__, __FUNCTION__ );
		return;
	}

	CTemplateNpc *tpNpc = (CTemplateNpc*)CDataStatic::SearchTpl( pNpc->GetNpcID( ) );
	if ( tpNpc == NULL || tpNpc->mRandomTeleService == 0 )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TELENONPC );
		return;
	}


	CTemplateReliveTable* tpFeeTable = ( CTemplateReliveTable* ) CDataStatic::GetTemp( TEMP_RELIVETABLE );
	if ( tpFeeTable == NULL )
	{
		LOG_ERROR( REPETION_LOG, "[ %s : %d ] [ %s ] cannt find template TEMPTYPE = %d", __LK_FILE__, __LINE__, __FUNCTION__, TEMP_RELIVETABLE );
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TELENONPC );
		return ;
	}


	int tRepetionIndex = tpNpc->mRandomTeleService;

	lua_tinker::table ret = lua_tinker::call<lua_tinker::table , int > (mLua, "get_tele_charge",tpNpc->mRandomTeleService,pPlayer->GetEntityID( ) );

	nMoneyNeed = ret.get< float >("charge") * ((unsigned int)tpFeeTable->mFee[  pPlayer->GetLevel() ]) ;
	int nDieTimes = ret.get< int >("dietimes");

	CMessageS2CGetRandomTeleChargeResponse tMsg;
	CMessage tMessage;

	tMsg.set_battleid( tRepetionIndex );
	tMsg.set_charge( nMoneyNeed );
	tMsg.set_dietimes( nDieTimes );


	tMessage.mutable_msghead()->set_messageid( ID_S2C_GET_RANDOM_TELE_CHARGE_RESPONSE );
	tMessage.set_msgpara( (uint32)&tMsg );

	 CSceneLogic::GetSingletonPtr( )->Send2Player( pPlayer, &tMessage );
#endif
}

int CRepetionModule::GetEntityLevel( unsigned int  vEntityID)
{
#ifdef _FB_

	CEntityCharacter* tpCharacter = ( CEntityCharacter* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpCharacter != NULL, return ERR_INVALID_PARAMETER, "tpCharacter is NULL");
	return tpCharacter->GetLevel();
#endif	
	return 0;
}

int CRepetionModule:: SendMessageInLua( int vMessageID,lua_tinker::table vMessage )
{
#ifdef _FB_
	switch(vMessageID)
	{
	case ID_S2G_BATTLE_CLOSE:
		{
			CMessage tMessage;
			CMessageS2GBattleClose tBattleClose;
			tMessage.mutable_msghead()->set_messageid( ID_S2G_BATTLE_CLOSE );
			tMessage.set_msgpara( (uint32)&tBattleClose );
			tBattleClose.set_battleid(vMessage.get<int>( "battleid"));
			tBattleClose.set_mapid(vMessage.get<int>( "mapid" ));
			CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMessage );	

		}
		break;
	case ID_S2G_END_CORPSBOSS_REPETION:
		{
			CMessage tMessage;
			CMessgeS2GEndCorpsBossRepetion tMsgEndRepetion;
			tMessage.mutable_msghead()->set_messageid( ID_S2G_END_CORPSBOSS_REPETION );
			tMessage.set_msgpara( (uint32)&tMsgEndRepetion);
			tMsgEndRepetion.set_battleid( vMessage.get<int>("battleid"));
			tMsgEndRepetion.set_corpsid( vMessage.get<int>("corpsid"));
			tMsgEndRepetion.set_mapid( vMessage.get<int>("mapid"));
			CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMessage );	
		}
		break;
	default:
		break;
	}
#endif
	return 0;
}

void CRepetionModule::AddIntDataByCampID( unsigned int vRepetionID,unsigned int vCampID,  unsigned int vIndex, int vIntData )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );	
	CRepetionInfo* tpInfo = tpRepetion->GetRepetionInfo( );
	REPE_ASSERT_STR( tpInfo != NULL, return, "RepetionID %d", vRepetionID );

	int tData[2] = { vIndex, vIntData };
	tpRepetion->PlayerEntityLoop( CRepetion::AddIntDataByCamp, vCampID, ( int )tData );
#endif	
}


bool CRepetionModule::NeedNecessityCheck( CRepetionInfo* vInfo )
{
	REPE_ASSERT( vInfo != NULL, return false );

	return vInfo->NeedNecessity( );
}

bool CRepetionModule::NeedSpeciaCheck( CRepetionInfo * vInfo )
{
	REPE_ASSERT( vInfo != NULL, return false );

	return vInfo->NeedSpecialCheck();
}


/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 获得模板经验表经验值
* 
* @Param vRepetionID
* @Param vEntityID
* @Param vTemplateID
*/ 
/* ----------------------------------------------------------------------------*/

long CRepetionModule::GetTemplateLevelExp(unsigned int vRepetionID, unsigned int vEntityID, unsigned int vTemplateID )
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return 0, "RepetionID %d", vRepetionID );

	CEntityPlayer* tpPlayer= ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	REPE_ASSERT_STR( tpPlayer != NULL, return 0, "EntityID %d", vEntityID );


	long tExp = 0;
	 CTemplateLevelExp* tpLevelExp = (CTemplateLevelExp*) CDataStatic::SearchTpl( vTemplateID );
	 if(NULL != tpLevelExp)
	 {     
		 tExp =(long)tpLevelExp->mLevelExp[ tpPlayer->GetLevel() ];    
	 }
	
	 return tExp;
#endif
}

// ***********************************************************
//  Function:		OnMessagePlayerApplyWar
//  Description:	跨服战场报名 				
//  Output:			
//  Date:	04/13/2011
// **********************************************************
void CRepetionModule::OnMessagePlayerApplyWar( CEntityPlayer *pPlayer,CMessage * pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	// 大雷音寺内不能报名
	if ( IS_GLOBAL_SCENESERVER( CSceneLogic::GetSingleton().GetLineID() ) == true )
	{
		return;
	}

	// 时间判断(通过活动)
	// 判断等级
	if ( pPlayer->GetLevel() < MAX_GLOBALWAR_APPLYLEVEL )
	{
		CGlobalWarInfo tGlobalWarInfo;
		SendResponsePlayerApplyWar( pPlayer, EDUNGEON_ERROR_LEVELLOW, tGlobalWarInfo, 0, 0 );
		return;
	}	
	CGlobalModule::GetSingleton().PlayerApplyWar( pPlayer );
}

// ***********************************************************
//  Function:		SendResponsePlayerApplyWar
//  Description:	跨服战场报名回应消息 				
//  Output:			
//  Date:	04/13/2011
// **********************************************************
void CRepetionModule::SendResponsePlayerApplyWar( CEntityPlayer *pPlayer, int nErrcode, CGlobalWarInfo &rGlobalWarInfo, int nMinLevel,int nMaxLevel )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageApplyGlobalWarResponse tMsgApplyGlobalWar;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_APPLYGLOBALWAR );
	tMsgHead.set_msgpara( (int)&tMsgApplyGlobalWar );
	tMsgApplyGlobalWar.set_errcode( nErrcode );
	tMsgApplyGlobalWar.set_minlevel( nMinLevel );
	tMsgApplyGlobalWar.set_maxlevel( nMaxLevel );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		OnMessagePlayerEnterGlobalWar
//  Description:	玩家进入跨服战场的消息请求 				
//  Output:			
//  Date:	04/13/2011
// **********************************************************
void CRepetionModule::OnMessagePlayerEnterGlobalWar( CEntityPlayer *pPlayer,CMessage * pMessage )
{
	// 副本中也可以进入跨服战
	if ( IS_DUNGEONSERVER_LINE( CSceneLogic::GetSingleton().GetLineID() ) )
	{
		return;
	}
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageEnterGlobalWarRequest *tpMsg = ( CMessageEnterGlobalWarRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	

	// 判断状态
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	CPlayerRepetion *tpReption = tpProperty->GetRepetion();
	if ( tpReption->GetGlobalSceneStatus() != EGLOBALWAR_STATUS_APPLYSUCESS )
	{
		SendResponsePlayerEnterGlobalWar( pPlayer, EDUNGEON_ERROR_NOTAPPLIED );
		return;
	}	
	if (  time( NULL ) - tpReption->GetGlobalWarOffLineTime() > MAX_WAITTIME )
	{
		SendResponsePlayerEnterGlobalWar( pPlayer, EDUNGEON_ERROR_APPLYTIMEOUT );
		return;
	}
	if ( pPlayer->GetAliveStatus() == ALIVE_DIE )
	{
		SendResponsePlayerEnterGlobalWar( pPlayer, EDUNGEON_ERROR_DEATH );
		return;
	}

	// 记录原始坐标
	CPlayerRepetion* tpPlayerRepetion = ( ( CPropertyPlayer* )pPlayer->GetProperty( ) )->GetRepetion( );
	CRepetionInfo *tpRepetionInfo = GetRepetionInfo( _REPE_GLOBALWAR_INDEX_ );
	if ( tpRepetionInfo == NULL )
	{
		return;
	}
	if ( CSceneLogic::GetSingleton().GetLineID() == pPlayer->GetNationality()
		|| IS_NEUTRAL_LINE_ID( CSceneLogic::GetSingleton().GetLineID() ) )
	{
		tpPlayerRepetion->SetBackLineID( CSceneLogic::GetSingleton().GetLineID() );
		tpPlayerRepetion->SetBackMapID( pPlayer->GetMapID( ) );
		tpPlayerRepetion->SetPosX( pPlayer->GetPosX( ) );
		tpPlayerRepetion->SetPosY( pPlayer->GetPosY( ) );
	}
	else
	{
		tpPlayerRepetion->SetBackLineID( pPlayer->GetNationality() );
		tpPlayerRepetion->SetBackMapID( tpRepetionInfo->GetBackMapID() );
		tpPlayerRepetion->SetPosX( tpRepetionInfo->GetBackPosX() );
		tpPlayerRepetion->SetPosY( tpRepetionInfo->GetBackPosY() );
	}
	CGlobalModule::GetSingleton().GlobalTelePort( pPlayer, EGLOBALTELE_TYPE_GLOBALSERVER );
}

// ***********************************************************
//  Function:		SendResponsePlayerEnterGlobalWar
//  Description:	玩家进入跨服战场的消息回应		
//  Output:			
//  Date:	04/13/2011
// **********************************************************
void CRepetionModule::SendResponsePlayerEnterGlobalWar( CEntityPlayer *pPlayer, int nErrcode )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageEnterGlobalWarResponse tMsgEnterGlobalWar;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ENTERGLOBALWAR );
	tMsgHead.set_msgpara( (int)&tMsgEnterGlobalWar );
	tMsgEnterGlobalWar.set_errcode( nErrcode );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );  
}

// ***********************************************************
//  Function:		SendNotifyCanEnterGlobalWar
//  Description:	玩家可以进入跨服战场的消息通知		
//  Output:			
//  Date:	04/13/2011
// **********************************************************
void CRepetionModule::SendNotifyCanEnterGlobalWar( CEntityPlayer *pPlayer, int nRepetionIndex )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;	
	CMessageCanEnterGlobalWarNotify tMsgCanEnterGlobalWar;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CANENTERGLOBALWAR );
	tMsgHead.set_msgpara( (int)&tMsgCanEnterGlobalWar );
	tMsgCanEnterGlobalWar.set_repetionindex( nRepetionIndex );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		OnMessagePlayerLeaveGlobalWarRequest
//  Description:	离开战场的消息请求		
//  Output:			
//  Date:	04/13/2011
// **********************************************************
void CRepetionModule::OnMessagePlayerLeaveGlobalWarRequest( CEntityPlayer *pPlayer,CMessage * pMessage )
{
	LK_ASSERT( pPlayer != NULL, return )			
	CMessagePlayerLeaveGlobalWarRequest *tpMsg = ( CMessagePlayerLeaveGlobalWarRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	
	SendResponsePlayerLeaveGlobalWar( pPlayer, SUCCESS );
	if ( IS_DUNGEONSERVER_LINE( CSceneLogic::GetSingleton().GetLineID() ) == true )
	{		
#ifdef _FB_
		PlayerLeaveRepetion( pPlayer );			
#endif	
	}				
	else
	{		
		CGlobalModule::GetSingleton().PlayerLeaveGlobalWar( pPlayer->GetCharID(), EREASON_LEAVEWAR_APPLYFAILED );		
	}
}

// ***********************************************************
//  Function:		SendResponsePlayerLeaveGlobalWar
//  Description:	离开战场的消息回应
//  Output:			
//  Date:	04/13/2011
// **********************************************************
void CRepetionModule::SendResponsePlayerLeaveGlobalWar( CEntityPlayer *pPlayer, int nErrcode )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessagePlayerLeaveGlobalWarResponse tMsgLeavelGlobalWar;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_LEAVEGLOBALWAR );
	tMsgHead.set_msgpara( (int)&tMsgLeavelGlobalWar );
	tMsgLeavelGlobalWar.set_errcode( nErrcode );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		OnMessageKickCampPlayerRequest
//  Description:	踢出玩家的消息请求
//  Output:			
//  Date:	05/31/2011
// **********************************************************
void CRepetionModule::OnMessageKickCampPlayerRequest( CEntityPlayer *pPlayer,CMessage * pMessage )
{	
#ifdef _FB_
	// 判断是不是跨服战场
	if ( IS_DUNGEONSERVER_LINE( CSceneLogic::GetSingleton().GetLineID() ) == false )
	{
		return;
	}	
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageKickCampPlayerRequest *tpMsg = ( CMessageKickCampPlayerRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )		
	CPlayerRepetion *tpPlayerRepetion = ((CPropertyPlayer *)pPlayer->GetProperty())->GetRepetion();
	CEntityPlayer *tpKickedPlayer = dynamic_cast< CEntityPlayer * >(CSceneLogic::GetSingleton().GetEntity( tpMsg->entityid() ));
	LK_ASSERT( tpKickedPlayer != NULL, return )	
	if ( tpKickedPlayer->GetEntityID() == pPlayer->GetEntityID() )
	{
		return;
	}
	
	CPlayerRepetion *tpKickedPlayerRepetion = ((CPropertyPlayer *)tpKickedPlayer->GetProperty())->GetRepetion();
	if ( tpPlayerRepetion->GetOldRepeKey() != tpKickedPlayerRepetion->GetOldRepeKey() )
	{
		SendResponseKickCampPlayer( pPlayer, ERR_REPETION_GLOBALWAR_NOTSAMEREPETION );
		return;
	}
	CRepetion *tpRepetion = GetRepetion( pPlayer );
	if ( tpRepetion == NULL )
	{
		SendResponseKickCampPlayer( pPlayer, ERR_REPETION_KICKCAMPPLAYER_NOTINBATTLE );
		return;
	}
	
	if ( tpPlayerRepetion->GetCampID() != tpKickedPlayerRepetion->GetCampID() )
	{
		SendResponseKickCampPlayer( pPlayer, ERR_REPETION_GLOBALWAR_NOTSAMECAMP );
		return;
	}

	// 查看是否已经有投票存在
	CCampKickVote *tpCampVoteCheck = GetCampKickVote( tpRepetion->get_id() );
	if ( tpCampVoteCheck != NULL )
	{
		SendResponseKickCampPlayer( pPlayer, ERR_REPETION_GLOBALWAR_CAMPKICKON );
		return;
	}

	// 建立一个投票信息
	CCampKickVote tCampKickVote;
	tCampKickVote.mSponsorRoleID = pPlayer->GetCharID();
	tCampKickVote.mKickedRoleID = tpKickedPlayer->GetCharID();	
	tpRepetion->PlayerEntityLoop( CRepetion::GetCampPlayerCount, tpPlayerRepetion->GetCampID(), (int)&tCampKickVote.mCampRoleNum );	
	tCampKickVote.InsertRoleVoteResult( pPlayer->GetCharID(), 1 );
	InsertCampKickVote( tpRepetion->get_id(), tCampKickVote );

	// 发送通知
	SendNotifyKickCampPlayer( tpRepetion, pPlayer, tpKickedPlayer );

	// 建立超时定时器
	int tOtherInfo = MESSAGE_REPETION;
	int tTimeLength = CSceneCfgManager::GetSingleton().GetRepetion_cfg()->mVotetime*1000;
	CTimerMng::GetSingleton().SetTimer( tpRepetion->get_id(), TIMER_CAMPKICKVOTETIME, tTimeLength, 1, &tOtherInfo );
	SendNotifyKickCampPlayer( tpRepetion, pPlayer, tpKickedPlayer );
	SendResponseKickCampPlayer( pPlayer, SUCCESS );
#endif
}

// ***********************************************************
//  Function:		SendResponseKickCampPlayer
//  Description:	踢出玩家的消息回应
//  Output:			
//  Date:	05/31/2011
// **********************************************************
void CRepetionModule::SendResponseKickCampPlayer( CEntityPlayer *pPlayer, int nErrcode )
{
#ifdef _FB_
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageKickCampPlayerResponse tMsgKickCampPlayer;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_KICKCAMPPLAYER );
	tMsgHead.set_msgpara( (int)&tMsgKickCampPlayer );
	tMsgKickCampPlayer.set_errcode( nErrcode );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
#endif
}

// ***********************************************************
//  Function:		SendNotifyKickCampPlayer
//  Description:	踢出玩家的消息通知
//  Output:			
//  Date:	05/31/2011
// **********************************************************
void CRepetionModule::SendNotifyKickCampPlayer( CRepetion *tpRepetion, CEntityPlayer *pSponsorPlayer, CEntityPlayer *pKickedPlayer )
{
#ifdef _FB_
	LK_ASSERT( pSponsorPlayer != NULL && pKickedPlayer != NULL && tpRepetion != NULL, return )
	CMessage tMsgHead;
	CMessageKickCampPlayerNotify tKickCampPlayer;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_KICKCAMPPLAYER );
	tMsgHead.set_msgpara( (int)&tKickCampPlayer );
	tKickCampPlayer.set_sponsorrolename( pSponsorPlayer->GetCharName() );
	tKickCampPlayer.set_entityid( pKickedPlayer->GetEntityID() );
	tKickCampPlayer.set_rolename( pKickedPlayer->GetCharName() );

	int tExceptRoleID[] = { pSponsorPlayer->GetCharID(), pKickedPlayer->GetCharID() };
	CRepetion::CCampMsgData tMsgData;
	tMsgData.mExceptRoleNum = 2;
	tMsgData.mExceptRoleID = tExceptRoleID;
	tMsgData.mpMsgParam = &tMsgHead;
	tpRepetion->PlayerEntityLoop( CRepetion::SendMsgByCamp, pSponsorPlayer->GetCampID(), (int)&tMsgData );
#endif
}

// ***********************************************************
//  Function:		OnMessageVoteToKickPlayer
//  Description:	踢出玩家的表决请求
//  Output:			
//  Date:	05/31/2011
// **********************************************************
void CRepetionModule::OnMessageVoteToKickPlayer( CEntityPlayer *pPlayer,CMessage * pMessage )
{
#ifdef _FB_
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageVoteToKickPlayerRequest *tpMsg = ( CMessageVoteToKickPlayerRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	
	CRepetion *tpRepetion = GetRepetion( pPlayer );
	if ( tpRepetion == NULL )
	{
		SendResponseVoteToKickPlayer( pPlayer, ERR_REPETION_KICKCAMPPLAYER_NOTONLINE );
		return;
	}

	CCampKickVote *tpCampKickVote = GetCampKickVote( tpRepetion->get_id() );
	if ( tpCampKickVote == NULL )
	{
		SendResponseVoteToKickPlayer( pPlayer, ERR_REPETION_KICKCAMPPLAYER_NOTONLINE );
		return;
	}
	
	// 被踢的人已经下线
	CEntityPlayer *tpKickedPlayer = dynamic_cast< CEntityPlayer * >( CSceneLogic::GetSingleton().GetEntity( tpMsg->entityid() ) );
	if ( tpKickedPlayer == NULL )
	{
		SendResponseVoteToKickPlayer( pPlayer, ERR_REPETION_KICKCAMPPLAYER_NOTONLINE );
		return;
	}		
	
	CRepetion *tpKickedRepetion = GetRepetion( tpKickedPlayer );
	if ( tpKickedRepetion == NULL )
	{
		SendResponseVoteToKickPlayer( pPlayer, ERR_REPETION_KICKCAMPPLAYER_NOTONLINE );
		return;
	}	
	
	// 判断是否已经投票
	if ( tpCampKickVote->CheckVoteValid( pPlayer->GetCharID() ) == false )
	{
		SendResponseVoteToKickPlayer( pPlayer, ERR_REPETION_KICKCAMPPLAYER_VOTED );
		return;
	}

	tpCampKickVote->InsertRoleVoteResult( pPlayer->GetCharID(), tpMsg->result() );
	SendResponseVoteToKickPlayer( pPlayer, SUCCESS );

	// 计算人数
	if ( tpCampKickVote->GetVotedNum() >= tpCampKickVote->mCampRoleNum - 1 )
	{
		int tVoteRate = CSceneCfgManager::GetSingleton().GetRepetion_cfg()->mCampkickrate;
		int tRealRate = 0;
		if ( tpCampKickVote->mCampRoleNum > 0 )
		{
			tRealRate = tpCampKickVote->GetAgreeNum()*100/tpCampKickVote->mCampRoleNum;
		}		
		int tResult = 0;
		if ( tRealRate >= tVoteRate )
		{
			tResult = 1;
		}				
		// 发送踢人结果
		SendKickResultNotify( tpRepetion, pPlayer->GetCampID(), tpCampKickVote->mSponsorRoleID, tpKickedPlayer->GetCharID(), tpKickedPlayer->GetCharName(), tResult );

		// 请离战场
		if ( tResult == 1 )
		{
			CPropertyPlayer *tpProperty = ( CPropertyPlayer * )tpKickedPlayer->GetProperty();
			CGlobalMapData tMapData;		
			tMapData.mLineID = tpProperty->GetRepetion( )->GetBackLineID( );
			tMapData.mMapID = tpProperty->GetRepetion()->GetBackMapID();
			tMapData.mPosX = tpProperty->GetRepetion()->GetPosX();
			tMapData.mPosY = tpProperty->GetRepetion()->GetPosY();
			CGlobalModule::GetSingleton().GlobalTelePort( tpKickedPlayer, EGLOBALTELE_TYPE_RETURNTOGAME, ESERVERTYPE_DUNGEON, &tMapData );
		}		
		DeleteCampKickVote( tpRepetion->get_id() );
	}	
#endif
}

// ***********************************************************
//  Function:		SendResponseVoteToKickPlayer
//  Description:	踢出玩家的表决回应
//  Output:			
//  Date:	05/31/2011
// **********************************************************
void CRepetionModule::SendResponseVoteToKickPlayer( CEntityPlayer *pPlayer, int nErrcode )
{
#ifdef _FB_
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageVoteToKickPlayerResponse tMsgVoteToKickPlayer;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_VOTETOKICKPLAYER );
	tMsgHead.set_msgpara( (int)&tMsgVoteToKickPlayer );
	tMsgVoteToKickPlayer.set_errcode( nErrcode );	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
#endif
}


// ***********************************************************
//  Function:		SendKickResultNotify
//  Description:	踢出玩家的表决回应
//  Output:			
//  Date:	05/31/2011
// **********************************************************
void CRepetionModule::SendKickResultNotify( CRepetion *tpRepetion, int nCampID, int nSponsorRoleID, int nKickedRoleID, const char* pKickedRoleName, int nResult )
{
#ifdef _FB_
	LK_ASSERT( tpRepetion != NULL && pKickedRoleName != NULL, return )
	CMessage tMsgHead;
	CMesssageKickResultNoitfy tMsgKickResult;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_KICKRESULT );
	tMsgHead.set_msgpara( (int)&tMsgKickResult );
	tMsgKickResult.set_result( nResult );
	tMsgKickResult.set_sponsorroleid( nSponsorRoleID );
	tMsgKickResult.set_kickedrolename( pKickedRoleName );
	tMsgKickResult.set_kickedroleid( nKickedRoleID );
	CRepetion::CCampMsgData tData;
	tData.mpMsgParam = &tMsgHead;
	tpRepetion->PlayerEntityLoop( CRepetion::SendMsgByCamp, nCampID,(int)&tData );
#endif
}

// ***********************************************************
//  Function:		EndVoteToCampKick
//  Description:	结束踢人投票
//  Output:			
//  Date:	05/31/2011
// **********************************************************
void CRepetionModule::EndVoteToCampKick( int nRepetionID )
{
#ifdef _FB_
	CRepetion *tpRepetion = GetRepetion( nRepetionID );
	if ( tpRepetion == NULL )
	{		
		DeleteCampKickVote( nRepetionID ); 
		return;
	}		 
	CCampKickVote *tpCampKickVote = GetCampKickVote( nRepetionID );
	if ( tpCampKickVote == NULL )
	{
		return;
	}	
	CEntityPlayer *tpKickedPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpCampKickVote->mKickedRoleID );
	if ( tpKickedPlayer == NULL )
	{
		DeleteCampKickVote( nRepetionID ); 
		return;
	}
	int tVoteRate = CSceneCfgManager::GetSingleton().GetRepetion_cfg()->mCampkickrate;
	int tResult = 0;
	if ( tpCampKickVote->mCampRoleNum > 0 )
	{
		int tRealRate = tpCampKickVote->GetAgreeNum()*100 / tpCampKickVote->mCampRoleNum;
		if ( tRealRate > tVoteRate )
		{
			tResult = 1;
		}		
	}
	SendKickResultNotify( tpRepetion, tpKickedPlayer->GetCampID(), tpCampKickVote->mSponsorRoleID, tpKickedPlayer->GetCharID(), tpKickedPlayer->GetCharName(), tResult );
	if ( tResult == 1 )
	{	
		CGlobalMapData tMapData;
		CPropertyPlayer *tpProperty = ( CPropertyPlayer * )tpKickedPlayer->GetProperty();
		tMapData.mLineID = tpProperty->GetRepetion( )->GetBackLineID( );
		tMapData.mMapID = tpProperty->GetRepetion()->GetBackMapID();
		tMapData.mPosX = tpProperty->GetRepetion()->GetPosX();
		tMapData.mPosY = tpProperty->GetRepetion()->GetPosY();
		CGlobalModule::GetSingleton().GlobalTelePort( tpKickedPlayer, EGLOBALTELE_TYPE_RETURNTOGAME, ESERVERTYPE_DUNGEON, &tMapData );
	}	
	DeleteCampKickVote( nRepetionID );
#endif
}

// ***********************************************************
//  Function:		InsertCampKickVote
//  Description:	插入一个副本的踢人投票信息
//  Output:			
//  Date:	05/31/2011
// **********************************************************
int CRepetionModule::InsertCampKickVote( int nRepeitonID, CCampKickVote &rCampKickVote )
{
#ifdef _FB_
	mCampKickVoteList.insert( CampKickVoteList::value_type( nRepeitonID, rCampKickVote ) );	
#endif	
	return SUCCESS;
}

// ***********************************************************
//  Function:		GetCampKickVote
//  Description:	获取一个副本的踢人投票信息
//  Output:			
//  Date:	05/31/2011
// **********************************************************
CCampKickVote *CRepetionModule::GetCampKickVote( int nRepeitonID )
{
#ifdef _FB_
	CampKickVoteList::iterator it = mCampKickVoteList.find( nRepeitonID );
	if ( it != mCampKickVoteList.end() )
	{
		return &(it->second);
	}
	return NULL;
#endif
	return NULL;
}

// ***********************************************************
//  Function:		DeleteCampKickVote
//  Description:	删除一个副本的踢人投票信息
//  Output:			
//  Date:	05/31/2011
// **********************************************************
int CRepetionModule::DeleteCampKickVote( int nRepetionID )
{
#ifdef _FB_
	CampKickVoteList::iterator it = mCampKickVoteList.find( nRepetionID );
	if ( it != mCampKickVoteList.end() )
	{
		mCampKickVoteList.erase( it );
	}
#endif
	return SUCCESS;
}

void CRepetionModule::LoadLuaFileCallback(const char* pcConfigPath)
{
	if (CRepetionModule::GetSingleton().m_bReloadFlag)
	{
		return;
	}
	
	unsigned int i = 0;
	for( i = 0; i < ARRAY_CNT( CRepetionModule::GetSingleton().mRepetionInfo ); i++ )
	{
		CRepetionModule::GetSingleton().mRepetionInfo[ i ].Clear( );
	}

	CRepetionModule::GetSingleton().RegisterLua( ( const char* )CSceneLogic::GetSingleton().GetConfigDataFileNamePath( CSceneLogic::GetSingletonPtr()->GetGameID( ), 
		        ( char* )"repetion_data" ) );
	CRepetionModule::GetSingleton().m_bReloadFlag = true;

}

bool CRepetion::AwardFameByIndex( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vIndex, int vPercent )
{
	CRepetionInfo* tpRepetionInfo = vpRepetion->GetRepetionInfo( );
	if( tpRepetionInfo == NULL )
		return false;

	if ( vpEntity == NULL || vpEntity->IsPlayer() == false )
		return false;

	CEntityPlayer *tpPlayer = (CEntityPlayer *)vpEntity;

	CTemplateRepeFameTable *tpConfig = ( CTemplateRepeFameTable * )CDataStatic::GetTemp( TEMP_REPEFAMETABLE );
	if ( tpConfig == NULL )
		return false;
	if ( vIndex < 0 || vIndex > ARRAY_CNT(tpConfig->mFameType) )
		return false;
	int tType = tpConfig->mFameType[ vIndex ];
	int tValue = tpConfig->mFameValue[ vIndex ] * vPercent / SERVER_PERCENT_FLOAT;
	if ( tType == FAME_TYPE_COMBAT )
	{
		CPropertyModule::GetSingleton().ChangeCombatFame( tpPlayer, tValue );
		if ( tpConfig->mItemValue[ vIndex ] > 0 )
		{
			CItemObject * pInsItemPile = CPropertyModule::GetSingletonPtr( )->PlayerInsertItem( tpPlayer, tpConfig->mCombatItemID, tpConfig->mItemValue[ vIndex ] );
			if (pInsItemPile != NULL)
			{
				LogEventGetItemByRepetion( tpPlayer, pInsItemPile->GetGUID(), tpConfig->mCombatItemID, tpConfig->mItemValue[ vIndex ],
					tpRepetionInfo->GetBeginMapID( ), 0, vpRepetion->GetKey( ));
			}
		}
	}
	else if ( tType == FAME_TYPE_REPE )
	{
		CPropertyModule::GetSingleton().ChangeRepeFame( tpPlayer, tValue );
		if ( tpConfig->mItemValue[ vIndex ] > 0 )
		{
			CItemObject * pInsItemPile = CPropertyModule::GetSingletonPtr( )->PlayerInsertItem( tpPlayer, tpConfig->mRepeItemID, tpConfig->mItemValue[ vIndex ] );
			if (pInsItemPile != NULL)
			{
				LogEventGetItemByRepetion( tpPlayer, pInsItemPile->GetGUID(), tpConfig->mRepeItemID, tpConfig->mItemValue[ vIndex ],
					tpRepetionInfo->GetBeginMapID( ), 0, vpRepetion->GetKey( ));
			}
		}
	}
	else if ( tType == FAME_TYPE_COMMON )
	{
		CPropertyModule::GetSingleton().ChangeCommonFame( tpPlayer, tValue );
		if ( tpConfig->mItemValue[ vIndex ] > 0 )
		{
			CItemObject * pInsItemPile = CPropertyModule::GetSingletonPtr( )->PlayerInsertItem( tpPlayer, tpConfig->mCommonItemID, tpConfig->mItemValue[ vIndex ] );
			if (pInsItemPile != NULL)
			{
				LogEventGetItemByRepetion( tpPlayer, pInsItemPile->GetGUID(), tpConfig->mCommonItemID, tpConfig->mItemValue[ vIndex ],
					tpRepetionInfo->GetBeginMapID( ), 0, vpRepetion->GetKey( ));
			}
		}
	}
	return true;
}

void CRepetionModule::AwardFameAll( int vRepetionID, int vIndex, int vPercent )
{
#ifdef _FB_

	if ( vPercent == 0 || vPercent > SERVER_PERCENT_INT )
		vPercent = SERVER_PERCENT_INT;

	CRepetion* tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoop( CRepetion::AwardFameByIndex, vIndex, vPercent );

#endif
}

bool CRepetion::AwardFameCamp1( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vIndex, int vPercent )
{
	if ( vpEntity == NULL )
		return false;
	if ( vpEntity->GetCampID() != 1 )
		return false;
	return AwardFameByIndex( vpRepetion, vpEntity, vIndex, vPercent );
}

bool CRepetion::AwardFameCamp2( CRepetion* vpRepetion, CEntityCharacter* vpEntity, int vIndex, int vPercent )
{
	if ( vpEntity == NULL )
		return false;
	if ( vpEntity->GetCampID() != 2 )
		return false;
	return AwardFameByIndex( vpRepetion, vpEntity, vIndex, vPercent );
}

void CRepetionModule::AwardFameAllCamp1( int vRepetionID, int vIndex, int vPercent )
{
#ifdef _FB_

	CRepetion* tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoop( CRepetion::AwardFameCamp1, vIndex, vPercent );

#endif
}

void CRepetionModule::AwardFameAllCamp2( int vRepetionID, int vIndex, int vPercent )
{
#ifdef _FB_

	CRepetion* tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	tpRepetion->PlayerEntityLoop( CRepetion::AwardFameCamp2, vIndex, vPercent );

#endif
}

void CRepetionModule::AwardFameSingle( int vRepetionID, int vEntityID, int vIndex, int vPercent )
{
#ifdef _FB_
	CEntityCharacter* tpEntity = (CEntityCharacter*)CSceneLogic::GetSingleton( ).GetEntity( vEntityID );
	CRepetion* tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );
	tpRepetion->AwardFameByIndex( tpRepetion, tpEntity, vIndex, vPercent );
#endif
}

void CRepetionModule::CheckAward( int vRepetionID, int vWinCamp )
{
#ifdef _FB_

	CRepetion* tpRepetion = ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );

	tpRepetion->PlayerCheckAward( vWinCamp );

#endif
}

int CRepetion::PlayerCheckAward( int vWinCamp )
{
#ifdef _FB_
	bool tContinue = true;
	for( int j = 0; j < ( int )ARRAY_CNT( mMapObjIDList ); j++ )
	{
		if( mMapObjIDList[ j ].mMapObjID == INVALID_OBJ_ID )
		{
			break;
		}

		CMapObject* tpMapObj = GetMapObject( j );

		if( tpMapObj == NULL )
		{
			break;
		}

		for( int i = tpMapObj->mPlayers.size( ) - 1; i >= 0; i-- )
		{
			CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( tpMapObj->mPlayers[ i ] );

			REPE_ASSERT( tpEntity != NULL, continue );

			if( tpEntity->IsPlayer()  == false )
			{
				continue;	
			}

			CEntityPlayer *tpPlayer = (CEntityPlayer*)tpEntity;

			lua_tinker::table vParame( CRepetionModule::GetSingletonPtr( )->mLua );

			vParame.set( "entityid", tpEntity->get_id() );
			vParame.set( "campid", tpPlayer->GetCampID() );
			vParame.set( "wincamp", vWinCamp );
			DoEvent( GetRepetionInfo( ), REPETION_CHECK_AWARD , &vParame );
		}
	}
#endif

	return SUCCESS;
}

void CRepetionModule:: OnMessageIntoCorpsBossRepetion( CEntityPlayer *pPlayer, CMessage *pMessage )
{
//#ifdef _FB_
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageIntoCorpsBossRepetionRequest *tpMsg = (CMessageIntoCorpsBossRepetionRequest *)pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	

	CRepetionInfo* tpInfo = GetRepetionInfo( tpMsg->battleid( ) );
	REPE_ASSERT_STR( tpInfo != NULL, return, "Cannt Get RepetionInfo %d", tpMsg->battleid( ) );
	int tRtn = PlayerCanIntoRepetion( pPlayer, tpMsg->battleid( ) );
	int tPosX = tpInfo->GetPosX( );
	int  tPosY = tpInfo->GetPosY( );

#ifdef _FB_		

	CPropertyPlayer* tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();

	if ( IS_REPETION_LINE_ID( CSceneLogic::GetSingleton().GetLineID() ) && 
		tpProperty->GetRepetion()->GetInRepetionIndex() == tpMsg->battleid() )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_WORLD_SUMMONINREP );
		return;
	}
#endif

	if( tRtn == SUCCESS )
	{
		CMessageS2GIntoCorpsBossRepetion tMsg;
		CMessage tMessage;
		tMsg.set_battleid( tpMsg->battleid() );
		tMsg.set_charid( pPlayer->GetCharID() );
		tMsg.set_corpsid( pPlayer->GetCorpsID());
		tMsg.set_mapid( tpInfo->GetBeginMapID());
		tMsg.set_postx( tPosX );
		tMsg.set_posty( tPosY );

		tMessage.mutable_msghead()->set_messageid( ID_S2G_INTO_CORPSBOSS_REPETION );
		tMessage.set_msgpara( (uint32)&tMsg );

		CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMessage );

		
	}


//#endif
}



void CRepetionModule::CreateNpcDynamic( unsigned int vRepetionID, int vMapID, int  vTempID,  lua_tinker::table vParame )
{
	CRepetion* tpRepetion = GetRepetion( vRepetionID );
	REPE_ASSERT_STR( tpRepetion != NULL, return, "RepetionID %d", vRepetionID );
	CMapObject* tpMapObj = tpRepetion->GetMapObjectByMapID( vMapID );
	REPE_ASSERT_STR( tpMapObj != NULL, return, "Cannt Get Repetion MapObj %d", vMapID );
	CWTPoint tPoint = CWTPoint( vParame.get< int >("posx"),  vParame.get< int >("posy") );

	CCreator* pCreater = NULL;
	COgreCreator tOgre;
	tOgre.set_id(0);
	CBoxCreator tBox;
	tBox.set_id( 0 );

	switch( vParame.get< int >("npc_type"))
	{
	case ENTITYTYPE_OGRE:
		{
			tOgre.mRefresh1[ 0 ].mOgreID = vTempID;
			tOgre.mRefresh1[ 0 ].mOgreRand = 10000;

			pCreater = &tOgre;
		}
		break;
	case ENTITYTYPE_BOX:
		{
			pCreater = &tBox;
		}
		break;
	default:
		break;
	}

	tpMapObj->CreateNpc( pCreater, false, vTempID, tPoint );
}


void CRepetionModule::RepeLogScore( unsigned int vRepetionID, unsigned int vEntityID, int vCurrentScore, int vDeltaScore,  int vScoreType  )
{
#ifdef _FB_
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( vEntityID );
	LK_ASSERT( tpPlayer != NULL, return);
	LogEventPlayerRepeScore(tpPlayer, vCurrentScore, vDeltaScore, vScoreType);
#endif
}

bool CRepetionModule::PlayerOpenBoxLimited(  CEntityPlayer *pPlayer)
{
#ifdef _FB_
	LK_ASSERT( pPlayer != NULL, return true);
	CPropertyPlayer* pProperty = (CPropertyPlayer*) pPlayer->GetProperty( );
	CPlayerRepetion *pPlayerRepetion = pProperty->GetRepetion( );

	int nLimit = 0;

	switch( pPlayerRepetion->GetInRepetionIndex())
	{
	case _REPE_CORPSBOSS_INDEX_ :

		pPlayerRepetion->GetIntData(_REPE_CORPSBOSS_INDEX_, 0, &nLimit );
		break;
	default:
			break;
	}
	if ( nLimit == 1)
	{
		return true;
	}
	
	return false;

#endif

}