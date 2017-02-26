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
#include "servertool.h"
#include "world.h"
#include "errcode.h"

template<> CRepetionModule* CSingleton< CRepetionModule >::spSingleton = NULL;


typedef CGateObjManagerImp::ObjMng_RepetionInfo REPETION_LIST_TYPE;
#define REPETION_LIST CGateObjManager::GetSingletonPtr( )->mGateObjManagerImp->mRepetionMapPool 


KEY_TYPE CRepetionModule::MakeKey( unsigned int vParame1, unsigned int vParame2 )
{
	KEY_TYPE vKey;
	int tTime = time( NULL );

	vKey = ( ( KEY_TYPE )vParame1 << 56 ) | ( ( KEY_TYPE )( vParame2 & 0x00FFFFFF ) << 32 ) | ( ( KEY_TYPE )tTime );

	return vKey;
}

CRepetion* CRepetionModule::GetRepetion( KEY_TYPE vKey )
{
	CRepetion* tpRepetion = NULL;
	REPETION_LIST_TYPE::iterator it;

	for( it = REPETION_LIST.begin( ); it != REPETION_LIST.end( ); it++ )
	{
		tpRepetion = &( *it );
		if( tpRepetion != NULL )
		{
			if( tpRepetion->GetKey( ) == vKey )
			{
				break;
			}
		}

		tpRepetion = NULL;
	} 

	return tpRepetion;
}

CRepetion* CRepetionModule::CreateRepetion( 
		CRepetionInfo* vpInfo, 
		unsigned short vParame,
		int vCreator,
		int vType,
		int vParame1,
		int vParame2,
		KEY_TYPE vKey,
		int vRemoveItem
		)
{
	CRepetion* tpRepetion = NULL;
	int tMapID = 0;

	REPE_ASSERT( vpInfo != NULL, return NULL );
	int vReason = SUCCESS;


	REPE_LOG( _LOG_NOTICE_, "Begin Create Repetion" );


	if( vpInfo->GetBeginMapID( ) != INVALID_MAP_ID )
	{
		tMapID = vpInfo->GetBeginMapID( );
		
		CSceneServer* tpHost = NULL;
		CLineRepetionInfo* tpIdleInfo = NULL;

		while( tpHost == NULL )
		{
			tpIdleInfo = GetMostIdleLineID( tMapID );
			if( tpIdleInfo == NULL )
			{

				REPE_LOG( _LOG_ERROR_, " Cannt Get Free Line For Repetinon Map %d", tMapID );
				break;;
			}
			tpHost = CGateServer::GetSingleton().GetServerByLineMap( tpIdleInfo->GetLineID( ), tMapID );
			if( tpHost == NULL )
			{

				REPE_LOG( _LOG_NOTICE_, "Line %d Map %d Server Is Not Open Or Closed", tpIdleInfo->GetLineID( ), 
					tMapID );
				tpIdleInfo->SetDown( );
			}
		}

		if( tpHost == NULL )
		{	

			REPE_LOG( _LOG_ERROR_, "Cannt Get Repetion %d Map", tMapID );
			vReason = ERR_REPETION_CREATE_FULL;
			return NULL;
		}

		tpRepetion = ( CRepetion* )CGateObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_REPETION_MAP );
		if( tpRepetion != NULL )
		{


			REPE_LOG( _LOG_NOTICE_, "Create Repetin Object( %d ), Map %d, Line %d", tpRepetion->get_id( ), tMapID, 
				tpIdleInfo->GetLineID( ) );

			if( vKey == INVALID_MAP_ID )
			{
				KEY_TYPE tKey = MakeKey( vParame, tpRepetion->get_id( ) );
				tpRepetion->SetKey( tKey );
			}
			else
			{
				tpRepetion->SetKey( vKey );
			}

			tpRepetion->SetMapID( vpInfo->GetBeginMapID( ) );
			tpRepetion->SetLineID( tpIdleInfo->GetLineID( ) );
			tpRepetion->SetRepetionIndex( vpInfo->GetRepetionIndex( ) );

			tpRepetion->SetType( vType );
			tpRepetion->SetParam1( vParame1 );
			tpRepetion->SetParam2( vParame2 );

			tpIdleInfo->AddLineRepetionCount( );


			REPE_LOG( _LOG_NOTICE_, "Set New Repetion( %d : %d ) Key( %lld )", tpRepetion->get_id( ), 
				vpInfo->GetBeginMapID( ), tpRepetion->GetKey( ) );


			REPE_LOG( _LOG_NOTICE_, "Notice Scene( %d : %d ) Create Repetion( %d : %lld )", tpIdleInfo->GetLineID( ), 
				vpInfo->GetBeginMapID( ), tpRepetion->get_id( ), tpRepetion->GetKey( ) );

			CMessage tMsg;
			CMessageCreateRepetionNotify tCreateRepetion;

			tCreateRepetion.set_repetionindex( vpInfo->GetRepetionIndex( ) );
			tCreateRepetion.set_key( tpRepetion->GetKey( ) );
			tCreateRepetion.set_parame1( vParame1 );
			tCreateRepetion.set_parame2( vParame2 );
			tCreateRepetion.set_lineid( tpIdleInfo->GetLineID( ) );
			tCreateRepetion.set_creator( vCreator );
			tCreateRepetion.set_removeitem( vRemoveItem );

			tMsg.set_msgpara( (uint32_t) &tCreateRepetion );
			pbmsg_sethead(&tMsg, ID_G2S_NOTIFY_CREATE_REPETION, 0, 0, 0, 0, 0, 0);

			CGateServer::GetSingletonPtr( )->Send2Scene( tpHost, &tMsg );

		}
		else
		{
			vReason = ERR_REPETION_CREATE_FULL;
			REPE_LOG( _LOG_ERROR_, "Cannt Create Repetion Object" );
		}
	}
	else
	{
		vReason = ERR_REPETION_CREATE_NOTFOUND;
	}


	REPE_LOG( _LOG_NOTICE_, "End Create Repetion" );

	CreatRepetionLog( tpRepetion, vCreator, vReason );

	return tpRepetion;
}

CRepetion* CRepetionModule::CreateRepetionByMapID( 
		unsigned short vMapID, 
		unsigned short vParame, 
		int vCreator,
		int vType,
		int vParame1, 
		int vParame2, 
		KEY_TYPE vKey,
		int vRemoveItem )
{
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionMapInfoByMapID( vMapID );
	REPE_ASSERT_STR( tpInfo != NULL, return NULL, "MapID %d", vMapID );

	return CreateRepetion( tpInfo, vParame, vCreator, vType, vParame1, vParame2, vKey, vRemoveItem );
}

CRepetion* CRepetionModule::CreateRepetionByIndex( 
		int vRepetionIndex, 
		unsigned short vParame, 
		int vCreator,
		int vType,
		int vParame1, 
		int vParame2, 
		KEY_TYPE vKey )
{
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionMapInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return NULL, "vRepetionIndex %d", vRepetionIndex );

	return CreateRepetion( tpInfo, vParame, vCreator, vType, vParame1, vParame2, vKey );
}

int CRepetionModule::DestroyRepetion( CRepetion* vpRepetion, REPETION_END_REASON vReason, int vParame )
{
	REPE_ASSERT( vpRepetion != NULL, return ERR_INVALID_PARAMETER );

	REPE_LOG( _LOG_NOTICE_, "Destroy Repetion( %d : %lld ), Remove From RepetionManager, Line %d, Reason %d",
		vpRepetion->get_id( ), vpRepetion->GetKey( ), vpRepetion->GetLineID( ), vReason );


	CLineRepetionInfo* tpIdleInfo = GetLineRepetionInfo( vpRepetion->GetLineID( ) );

	if( tpIdleInfo != NULL )
	{
		tpIdleInfo->SubLineRepetionCount( );
	}
	else
	{
		REPE_LOG( _LOG_ERROR_, "Cannt Get CLineRepetionInfo" );
	}

	if( vReason != REPETION_END_COMMON )
	{
		// 非正常结束， 特殊处理
		if( ( vpRepetion->GetRepetionIndex( ) == _REPE_FAMILY_CHALLENGE_INDEX_ )
			|| ( vpRepetion->GetRepetionIndex( ) == _REPE_FAMILY_INDEX_ ) )
		{
			// 家族
			CFamilyManager::GetSingletonPtr( )->EndBattle( vpRepetion->GetKey( ), WIN_DEFSIDE );
		}
		else if( ( vpRepetion->GetRepetionIndex( ) == _REPE_CORPS_BATTLE_INDEX_ )
				|| ( vpRepetion->GetRepetionIndex( ) == _REPE_CORPS_CHALLEGE_INDEX_ ) )
		{
			//军团
			CCorpservice::GetSingletonPtr( )->OnEventBattleOver( 2, vpRepetion->GetKey( ) );
		}
		else if( ( vpRepetion->GetRepetionIndex( ) == _REPE_FAMILY_TASTE_INDEX_ ) )
		{
			CFamilyManager::GetSingletonPtr( )->EndFamilyReptionByKey( vpRepetion->GetKey( ), false );
		}
	}

	// 如果是可重入副本，发送信息给玩家，副本已经销毁
	CRepetionInfo *tpInfo = vpRepetion->GetRepetionInfo();
	if ( tpInfo != NULL )
	{
		if ( tpInfo->GetRelogin() == 1 )
		{
			int tRoleIDList[TEAMNUM] = { 0 };
			int tNum = ARRAY_CNT( tRoleIDList );
			vpRepetion->GetRoleIDList( tRoleIDList, tNum );
			for ( int i = 0; i < tNum; ++i  )
			{ 
				CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tRoleIDList[i] );
				if ( tpPlayer != NULL )
				{
					if ( tpPlayer->GetRepetionKey( tpPlayer->CharID() ) == vpRepetion->GetKey() )
					{
						CRepetionModule::GetSingleton().SendLastRepetionIndex( tpPlayer, vpRepetion->GetRepetionIndex(), REPETION_INDEX_TYPE_ENDRELOGIN );
					}					
				}								
			}						
		}		
	}
	DestroyRepetionLog( vpRepetion, vReason, vParame );	
	vpRepetion->Final( );	
	CGateObjManager::GetSingletonPtr( )->DestroyObject( vpRepetion->get_id( ) );
	return 0;	
}



CRepetionInfo* CRepetionModule::GetRepetionMapInfoByMapID( int vMapID )
{

	for( unsigned int i = 0; i < ARRAY_CNT( mRepetionInfo ); i++ )
	{
		if( mRepetionInfo[ i ].GetRepetionIndex( ) != INVALID_REPETION_INDEX )
		{
			if( mRepetionInfo[ i ].MapIsInRepetion( vMapID ) == true )
			{
				return &mRepetionInfo[ i ];
			}
		}
	}

	return NULL;
}

int CRepetionModule::PlayerIntoRepetion(	CGatePlayer* vpPlayer, 
											unsigned short vMapID, 
											unsigned short vTeamLevel,
											KEY_TYPE	vKey,
											int* vpLineID
										)
{
	int tRtn = SUCCESS;
	CRepetion* tpRepetion = NULL;

	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	REPE_ASSERT( vMapID != INVALID_MAP_ID, return ERR_INVALID_PARAMETER );

	if( INVITED_KEY_VALUE == vKey && vpPlayer->mRepetionMap[ vMapID ] == vKey )
	{
		REPE_LOG( _LOG_ERROR_, "Player %d Join Repetion ErrKey %lld", vpPlayer->mCharID, vKey );
		return ERR_INVALID_PARAMETER;
	}


	REPE_LOG( _LOG_NOTICE_, "Player( %d: %s ) Want Into Repetion( %lld )", vpPlayer->mCharID, 
		vpPlayer->GetCharNameShow( ), ( vKey == INVITED_KEY_VALUE ) ? vpPlayer->mRepetionMap[ vMapID ] : vKey );

	tpRepetion = GetRepetion( ( vKey == INVITED_KEY_VALUE ) ? vpPlayer->mRepetionMap[ vMapID ] : vKey );
	if( tpRepetion == NULL )
	{
		REPE_LOG( _LOG_WARN_, "Cannt Find Repetion( %lld )", 
			( vKey == INVITED_KEY_VALUE ) ? vpPlayer->mRepetionMap[ vMapID ] : vKey );
		return ERR_NOT_FOUND;
	}
		
	if( tpRepetion->IsCanIntoRepeByCamp( vpPlayer )	== false )
	{
		return ERR_REPETION_PLAYER_CAMP_FULL;
	}

	// add player into repetion
	tRtn = tpRepetion->InsertPlayer( vpPlayer->mCharID );
	if( tRtn == SUCCESS )
	{
		vpPlayer->mRepetionMap[ vMapID ] = tpRepetion->GetKey( );
		if( vpLineID != NULL )
		{
			*vpLineID = tpRepetion->GetLineID( );
		}

		REPE_LOG( _LOG_NOTICE_, "Player( %d : %s ) add into Repetion %lld", vpPlayer->mCharID, 	
			vpPlayer->GetCharNameShow( ), tpRepetion->GetKey( ) );
	}
	else
	{


		REPE_LOG( _LOG_NOTICE_, "Player( %d : %s ) cannt add into Repetion %lld, CurCount %d MaxCount %d",
			vpPlayer->mCharID, vpPlayer->GetCharNameShow( ), vMapID, tpRepetion->GetKey( ), 
			tpRepetion->GetPlayerCountCur( ), tpRepetion->GetPlayerCountMax( ) );
	}

	return tRtn;

}

int CRepetionModule::PlayerOutRepetion( CGatePlayer* vpPlayer, unsigned short vMapID )
{
	int tRtn = SUCCESS;
	CRepetion* tpRepetion = NULL;

	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	CRepetionInfo* tpInfo = GetRepetionMapInfoByMapID( vMapID );
	if( tpInfo == NULL )
	{
		LOG_DEBUG( "default", "[ %s : %d ][ %s ] Map %d Is Not Repetion", __LK_FILE__, __LINE__, __FUNCTION__, vMapID );
		return SUCCESS;
	}

	int tMapID = tpInfo->GetBeginMapID( );
	REPE_ASSERT_STR( tMapID != INVALID_MAP_ID, return ERR_INVALID_PARAMETER, "tMapID %d", tMapID );
	REPE_ASSERT_STR( ( unsigned int )tMapID < ARRAY_CNT( vpPlayer->mRepetionMap ), return ERR_INVALID_PARAMETER, 
		"vMapID %d", tMapID );

	if( vpPlayer->mRepetionMap[ tMapID ] != INVITED_KEY_VALUE )
	{
		tpRepetion = GetRepetion( vpPlayer->mRepetionMap[ tMapID ] );
		if( tpRepetion != NULL )
		{
			// 如果是可重入的副本不用清除信息
			if ( tpInfo->GetRelogin() != 1 )
			{
				tRtn = tpRepetion->RemovePlayer( vpPlayer->mCharID );
				if( tRtn == SUCCESS )
				{
					vpPlayer->mRepetionMap[ tMapID ] = INVITED_KEY_VALUE;

					REPE_LOG( _LOG_NOTICE_, "Player( %d : %s ) Out Repetion( %d : %d : %lld )", vpPlayer->mCharID,
						vpPlayer->GetCharNameShow( ), tMapID, tpRepetion->get_id( ), tpRepetion->GetKey( ),
						tpRepetion->GetPlayerCountCur( ) );

				}
				else
				{


					REPE_LOG( _LOG_ERROR_, "Player( %d : %s ) Cannt Find In Repetion( %d : %d : %lld )", vpPlayer->mCharID,
						vpPlayer->GetCharNameShow( ), tMapID, tpRepetion->get_id( ), tpRepetion->GetKey( ) );

					vpPlayer->mRepetionMap[ tMapID ] = INVITED_KEY_VALUE;
					tRtn = ERR_NOT_FOUND;
				}
			}			
		}
		else
		{


			REPE_LOG( _LOG_ERROR_, "Player( %d : %s ) Repetion( %d : %lld ) Cannt Find", vpPlayer->mCharID,
				vpPlayer->GetCharNameShow( ), vpPlayer->mRepetionMap[ tMapID ], tMapID );

			tRtn = ERR_NOT_FOUND;
		}
	} 
	else
	{


		REPE_LOG( _LOG_NOTICE_, "Player( %d : %s ) Not Have Repetion( MapID : %d )", vpPlayer->mCharID,
			vpPlayer->GetCharNameShow( ), tMapID );

		tRtn = ERR_NOT_FOUND;
	}

	return tRtn;
}

void* CRepetionModule::operator new( size_t size )
{
    return ( void* ) mShmPtr->CreateSegment( size );
}

void  CRepetionModule::operator delete( void* pointer )
{

}





int CRepetionModule::TeamWaitBattle( int vCharID, int vLevel, unsigned int vBattleID )
{

	CRepetionInfo* tpInfo = GetRepetionMapInfo( vBattleID );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "vBattleID %d", vBattleID );
	REPE_ASSERT_STR( tpInfo->GetRepetionIndex( ) != INVALID_REPETION_INDEX, return ERR_INVALID_PARAMETER, "vBattleID %d",
		vBattleID );

	unsigned int i = 0;
	unsigned int j = 0;
	int tRepetionIndex = tpInfo->GetRepetionIndex( );

	REPE_ASSERT_STR( tRepetionIndex >= 0, return ERR_INVALID_PARAMETER, "tRepetinIndex %d", tRepetionIndex );
	REPE_ASSERT_STR( tRepetionIndex < ( int )ARRAY_CNT( mWait ), return ERR_INVALID_PARAMETER, "tRepetionIndex %d", 
		tRepetionIndex );

	int tMemberList[ TEAM_NUMBER_MAX ] = { 0 };	
	int tNum = ( int )ARRAY_CNT( tMemberList );	
	BattleWaitPlayer tWait;
	int tRtn = ERR_REPETION_PVP_LEVEL_NOTFOUND;

	for( i = 0; i < ARRAY_CNT( mWait[ tRepetionIndex ] ); i++ )
	{
		if( vLevel >= mWait[ tRepetionIndex ][ i ].mLevelMin )
		{
			if( mWait[ tRepetionIndex ][ i ].mLevelMax != 0 )
			{
				if( vLevel > mWait[ tRepetionIndex ][ i ].mLevelMax )
				{
					continue;
				}
			}
			
			if( mWait[ tRepetionIndex ][ i ].mList.size( ) >= mWait[ tRepetionIndex ][ i ].mList.max_size( ) )
			{
				//等待队列满
				tRtn = ERR_REPETION_PVP_WAITLIST_FULL;
				break;
			}
			
			tNum = CServiceGateTeam::GetSingletonPtr( )->GetTeamList( vCharID, tMemberList, tNum );

			for( j = 0; ( int )j < tNum && j < ARRAY_CNT( tWait.mPlayers ) && j < ARRAY_CNT( tMemberList ); j++ )
			{
				CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( tMemberList[ j ] );
				REPE_ASSERT_STR( tpPlayer != NULL, continue, "Cannt Get ScenePlayer By %d", tMemberList[ j ] );

				if( tpInfo->MapIsInRepetion( tpPlayer->mMapID ) == true )
				{


					REPE_LOG( _LOG_INFO_, "Player( %d ) In Battle( %d : %lld ) Cannt Join Wait List", tpPlayer->mCharID,
						vBattleID, tpPlayer->mJoinBattle[ tpPlayer->mMapID ].mKey );
					continue;
				}

				RemoveFromWait( tMemberList[ j ], vBattleID );
				tWait.mPlayers[ j ] = tMemberList[ j ];

				CMessageBattleWaitNotice tWaitNotice;
				CMessage tMsg;

				tWaitNotice.set_charid( tMemberList[ j ] );
				tWaitNotice.set_battleid( vBattleID ); 
				tWaitNotice.set_mapid( tpInfo->GetBeginMapID( ) );

				tMsg.set_msgpara( (uint32_t) &tWaitNotice );
				pbmsg_sethead(&tMsg, ID_G2S_BATTLE_WAIT_NOTICE, 0, 0, 0, 0, 0, 0);

				CGateServer::GetSingletonPtr( )->Send2Scene( tpPlayer, &tMsg );	
			}

			tWait.mPlayerCount = j;

			mWait[ tRepetionIndex ][ i ].mList.push_back( tWait );

			tRtn = SUCCESS;

			break;
		}
	}

	return tRtn;
}

int CRepetionModule::RemoveFromWait( int vCharID, unsigned int vBattleID )
{
	CRepetionInfo* tpInfo = GetRepetionMapInfo( vBattleID );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "vBattleID %d", vBattleID );
	REPE_ASSERT_STR( tpInfo->GetRepetionIndex( ) != INVALID_REPETION_INDEX, return ERR_INVALID_PARAMETER, "vBattleID %d",
		vBattleID );

	unsigned int i = 0;
	unsigned int j = 0;
	int tRepetionIndex = tpInfo->GetRepetionIndex( );
	BATTLE_WAIT_LIST::iterator it;

	REPE_ASSERT_STR( tRepetionIndex >= 0, return ERR_INVALID_PARAMETER, "tRepetinIndex %d", tRepetionIndex );
	REPE_ASSERT_STR( tRepetionIndex < ( int )ARRAY_CNT( mWait ), return ERR_INVALID_PARAMETER, "tRepetionIndex %d", 
		tRepetionIndex );

	for( i = 0; i < ARRAY_CNT( mWait[ tRepetionIndex ] ); i++ )
	{
		if( mWait[ tRepetionIndex ][ i ].mLevelMin == 0 )
		{
			continue;
		}

		for( it = mWait[ tRepetionIndex ][ i ].mList.begin( ); it != mWait[ tRepetionIndex ][ i ].mList.end( ); it++ )
		{
			BattleWaitPlayer* tpWaitPlayer = &(*it);
			REPE_ASSERT_STR( tpWaitPlayer != NULL, continue, "Cannt Get Iteraot Type Form lk::vector" );

			if( tpWaitPlayer->mPlayerCount == 0 )
			{
				continue;
			}

			for( j = 0; j < ARRAY_CNT( tpWaitPlayer->mPlayers ); j++ )
			{
				if( tpWaitPlayer->mPlayers[ j ] == 0 )
				{

					REPE_LOG( _LOG_DEBUG_, "Wait List Iterator PlayerCount Is 0" );
					continue;
				}

				if( tpWaitPlayer->mPlayers[ j ] == vCharID )
				{
					// 找到Player, 将其从等待列表中删除

					CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( vCharID );

					REPE_ASSERT_STR( tpPlayer != NULL, continue, "Cannt Get ScenePlayer %d, MayBe Is Leaved", vCharID )
					tpWaitPlayer->mPlayers[ j ] = 0;
					tpWaitPlayer->mPlayerCount--;

					if( tpWaitPlayer->mPlayerCount <= 0 )
					{
						mWait[ tRepetionIndex ][ i ].mList.erase( it );			
					}

					CMessageBattleWaitLeaveResponse tMsg;
					CMessage tMessage;


					tMsg.set_charid( vCharID );
					tMsg.set_battleid( vBattleID );
					tMsg.set_mapid( tpInfo->GetBeginMapID( ) );

					tMessage.set_msgpara( (uint32_t) &tMsg );
					pbmsg_sethead(&tMessage, ID_G2S_BATTLE_WAIT_LEAVE_RESPONSE, 0, 0, 0, 0, 0, 0);

					CGateServer::GetSingletonPtr( )->Send2Scene( tpPlayer, &tMessage );
					
					return SUCCESS;
					break;
				}
			}		// for tpWaitPlayer->mPlayers
		}			// for mList	
	}				// for mWait

	return ERR_NOT_FOUND;
}

bool CRepetionModule::InWaitList( int vCharID, unsigned int vBattleID )
{
	CRepetionInfo* tpInfo = GetRepetionMapInfo( vBattleID );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "vBattleID %d", vBattleID );
	REPE_ASSERT_STR( tpInfo->GetRepetionIndex( ) != INVALID_REPETION_INDEX, return ERR_INVALID_PARAMETER, "vBattleID %d",
		vBattleID );

	unsigned int i = 0;
	unsigned int j = 0;
	int tRepetionIndex = tpInfo->GetRepetionIndex( );
	BATTLE_WAIT_LIST::iterator it;

	REPE_ASSERT_STR( tRepetionIndex >= 0, return ERR_INVALID_PARAMETER, "tRepetinIndex %d", tRepetionIndex );
	REPE_ASSERT_STR( tRepetionIndex < ( int )ARRAY_CNT( mWait ), return ERR_INVALID_PARAMETER, "tRepetionIndex %d", 
		tRepetionIndex );

	for( i = 0; i < ARRAY_CNT( mWait[ tRepetionIndex ] ); i++ )
	{
		if( mWait[ tRepetionIndex ][ i ].mLevelMin == 0 )
		{
			continue;
		}

		for( it = mWait[ tRepetionIndex ][ i ].mList.begin( ); it != mWait[ tRepetionIndex ][ i ].mList.end( ); it++ )
		{
			BattleWaitPlayer* tpWaitPlayer = &(*it);
			REPE_ASSERT_STR( tpWaitPlayer != NULL, continue, "Cannt Get Iteraot Type Form lk::vector" );

			if( tpWaitPlayer->mPlayerCount == 0 )
			{
				continue;
			}

			for( j = 0; j < ARRAY_CNT( tpWaitPlayer->mPlayers ); j++ )
			{
				if( tpWaitPlayer->mPlayers[ i ] == 0 )
				{

					REPE_LOG( _LOG_ERROR_, "Wait List Iterator PlayerCount Is 0" );
					continue;
				}

				if( tpWaitPlayer->mPlayers[ i ] == vCharID )
				{
					return true;
					break;
				}
			}		// for tpWaitPlayer->mPlayers
		}			// for mList	
	}				// for mWait

	return false;

}


void CRepetionModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{

	if( mTimeOut.Elapse( vTickOffset ) )
	{
		DELETE_VECTOR tDeleteList;
		tDeleteList.initailize( );
		REPETION_LIST_TYPE::iterator it;

		for( it = REPETION_LIST.begin( ); it != REPETION_LIST.end( ); it++ )
		{
			CRepetion* tpRepetion = &( *it );
			
			tpRepetion->OnTimer( 2 * 60 * 1000, &tDeleteList );
		} 

		for( DELETE_VECTOR::iterator tDelIt = tDeleteList.begin( ); tDelIt != tDeleteList.end( ); tDelIt++ )
		{
			CRepetion* tpRepetion = ( CRepetion* )CGateObjManager::GetSingletonPtr( )->GetObject( *tDelIt );
			REPE_ASSERT_STR( tpRepetion != NULL, continue, "ID %d", *tDelIt );
			CRepetionModule::GetSingletonPtr( )->DestroyRepetion( tpRepetion, REPETION_END_TIMEOUT );
			tpRepetion = NULL;	
		}	
	}


	if( mTimer.Elapse( vTickOffset ) )
	{
		for( unsigned int i = 0; i < ARRAY_CNT( mRepetionInfo ); i++ )
		{
			int tRepetionIndex = mRepetionInfo[ i ].GetRepetionIndex( );

			if( tRepetionIndex < 0 )
			{
				continue;
			}

			if( tRepetionIndex >= ( int )ARRAY_CNT( mWait ) )
			{
				continue;
			}

			if( tRepetionIndex == INVALID_REPETION_INDEX )
			{
				continue;
			}

	
			time_t tim = time( NULL );
			struct tm* local_time = localtime( &tim );
			bool tIsBattleTime = mRepetionInfo[ i ].InBattleTime( local_time );
			if( tIsBattleTime == false )
			{
				if( mRepetionInfo[ i ].IsBattleOn( ) == true )
				{
					// 清除等待队列
					CleanWaitList( i );
					//清楚固定时间报名的等待队列
					CleanFixWaitList( i );

					mRepetionInfo[ i ].SetBattleOnStatus( false );
				}
				//固定开放的副本 
				mOnTimeRepetion[ tRepetionIndex ].mOnTimeLevelLimit->mIsCreate = false;
				continue;
			}
		
			mRepetionInfo[ i ].SetBattleOnStatus( true );
				
			for( unsigned int j = 0; j < ARRAY_CNT( mWait[ tRepetionIndex ] ); j++ )
			{
				BattleWait* tpWait = &mWait[ tRepetionIndex ][ j ];	
				if( tpWait->mLevelMin == 0 )
				{
					continue;
				}

				if( tpWait->mList.size( ) >= 2 )
				{
					// 相互匹配队伍找到, 通知可以进入战场， 并且从等待列表中删除
					
					int vParame = ( tpWait->mLevelMax > tpWait->mLevelMin ) ? 
						( tpWait->mLevelMax + tpWait->mLevelMin ) >> 1 : tpWait->mLevelMin;
					time_t tTime = time( NULL );
					unsigned int m = 0;
					CRepetion* tpRepetion = CreateRepetionByMapID( mRepetionInfo[ i ].GetBeginMapID( ), vParame, 0, 0 );
					REPE_ASSERT( tpRepetion != NULL, return );		// 副本资源耗尽
					KEY_TYPE tKey = tpRepetion->GetKey( ); 
					
					BATTLE_WAIT_LIST::iterator it;

					for( m = 0; m < 2; m++ )
					{
						it = tpWait->mList.begin( );
						unsigned int k = 0;
						for( k = 0; k < ARRAY_CNT( ( *it ).mPlayers ); k++ )
						{
							int tCharID = ( *it ).mPlayers[ k ];
							if( tCharID != 0 )
							{
								SetPlayerJoin( tCharID, mRepetionInfo[ i ].GetRepetionIndex( ), tKey, tTime, m + 1 );
							}
						}

						tpWait->mList.erase( it );
					}
				}
			}
			
//add by yucx  2010年 09月 11日 星期六 13:38:37 HKT
			for( unsigned int j = 0; j < ARRAY_CNT( mFixWait[ tRepetionIndex ] ); j++ )
			{

				BattleFixWait* tpFixWait = &mFixWait[ tRepetionIndex ][ j ];	
				if( tpFixWait->mLevelMin == 0 )
				{
					continue;
				}


				if( tpFixWait->mList.size( ) > 0 )
				{
					// 相互匹配队伍找到, 通知可以进入战场， 并且从等待列表中删除


					int vParame = ( tpFixWait->mLevelMax > tpFixWait->mLevelMin ) ? 
						 tpFixWait->mLevelMax : tpFixWait->mLevelMin;
					time_t tTime = time( NULL );
				
					CRepetion* tpRepetion = CreateRepetionByMapID( mRepetionInfo[ i ].GetBeginMapID( ), vParame, 0, 0 ,tpFixWait->mLevelMin, tpFixWait->mLevelMax);
					REPE_ASSERT( tpRepetion != NULL, return );		// 副本资源耗尽
					KEY_TYPE tKey = tpRepetion->GetKey( ); 

					BATTLE_FIX_WAIT_LIST::iterator it ;


					 while( !tpFixWait->mList.empty())
					 {


						 it = tpFixWait->mList.begin( );

						 unsigned int k = 0;
						 for( k = 0; k < ARRAY_CNT( ( *it ).mPlayers ); k++ )
						 {
							 int tCharID = ( *it ).mPlayers[ k ];
							 if( tCharID != 0 )
							 {

								//此处极其丑陋，为了让 阵营取到NULL Campid 设为 现在阵营id 可谓 1,2  当大于2时 取到NULL 5
								//add by yucx 2010年 09月 18日 星期六 09:59:20 HKT
								 SetPlayerJoin( tCharID, mRepetionInfo[ i ].GetRepetionIndex( ), tKey, tTime, 5);
							 }
						 }
						it = tpFixWait->mList.erase( it );

					 }
				 }
				
			}
			//add by yucx 固定时间开始的副本 Tue Dec  7 15:36:15 HKT 2010
			if( mRepetionInfo[ i ].IsOnTimeOpen() )
			{
				if ( mOnTimeRepetion[ tRepetionIndex ].mOnTimeLevelLimit->mIsCreate == false )
				{
					int tRet = CreateOnTimeOpenRepetion( tRepetionIndex );
					if ( tRet == SUCCESS )
					{
						mOnTimeRepetion[ tRepetionIndex ].mOnTimeLevelLimit->mIsCreate = true;
					}
					
				}
				
			}
		}

	}
}

int CRepetionModule::SetWaitListLevel( int vRepetionIndex, int vLevelMin, int vLevelMax )
{
	CRepetionInfo* tpInfo = GetRepetionMapInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "vRepetion %d", vRepetionIndex );
	REPE_ASSERT_STR( tpInfo->GetRepetionIndex( ) != INVALID_REPETION_INDEX, return ERR_INVALID_PARAMETER, 
		"vRepetionIndex %d", vRepetionIndex );

	int tRepetionIndex = tpInfo->GetRepetionIndex( );

	REPE_ASSERT_STR( tRepetionIndex >= 0, return ERR_INVALID_PARAMETER, "tRepetinIndex %d", tRepetionIndex );
	REPE_ASSERT_STR( tRepetionIndex < ( int )ARRAY_CNT( mWait ), return ERR_INVALID_PARAMETER, "tRepetionIndex %d", 
		tRepetionIndex );

	for( unsigned int i = 0; i < ARRAY_CNT( mWait[ tRepetionIndex ] ); i++ )
	{
		if( mWait[ tRepetionIndex ][ i ].mLevelMin == 0 )
		{
			mWait[ tRepetionIndex ][ i ].mLevelMin = vLevelMin;
			mWait[ tRepetionIndex ][ i ].mLevelMax = vLevelMax;
			break;
		}
	}

	return SUCCESS;
}

CRepetionModule::CRepetionModule()
{
	if( CObj::msCreateMode == 1 )
	{
		mTimer = CWTimer( 2000 );
		mTimeOut = CWTimer( 1000 * 60 * 2 );
	}
	

	INIT_ROLLINGFILE_LOG( REPE_LOG_KEY, "../log/repetion.log", LEVEL_DEBUG );
}



int CRepetionModule::OnMessageBattleCreate( int vServerID, CMessageBattleCreateRequest* vpMsg )
{
	CRepetion* tpRepetion = CreateRepetionByMapID( vpMsg->battleid( ), vpMsg->level( ), 0, 0 );
	if( tpRepetion == NULL )
	{
		return INVITED_KEY_VALUE;
	}

	CMessageBattleCreateResponse tMsgFact;
	CMessage tMsg;

	tMsgFact.set_battleid( vpMsg->battleid( ) );
	tMsgFact.set_key( tpRepetion->GetKey( ) );

	tMsg.set_msgpara( (uint32_t) &tMsgFact );
	pbmsg_sethead( &tMsg, ID_G2S_BATTLE_CREATE_RESPONSE, 0, 0, 0, 0, 0, 0 );

	CGateServer::GetSingletonPtr( )->Send2Scene( vServerID, &tMsg );	
	return SUCCESS;
}

int CRepetionModule::SetPlayerJoin( int vCharID, int vBattleID, KEY_TYPE vKey, int vTime, int vCampID, int vCampCountMax /* = 0 */, bool bIntoLastRepetion /* = false */ )
{
	// 通知可以进入战场
	CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( vCharID );
	if( tpPlayer == NULL )
	{
		REPE_LOG( _LOG_INFO_, " Cannt Get GatePlayer %d, Fuck Zhang 3", vCharID );
		return ERR_INVALID_PARAMETER;
	}

	if( vKey == INVITED_KEY_VALUE )
	{
		REPE_LOG( _LOG_ERROR_, "nvalid Parame Key %d, BattleID %d, player %d : %s", vKey, vBattleID, vCharID,
			tpPlayer->GetCharNameShow( ) );
		return ERR_INVALID_PARAMETER;
	}	

	CRepetion* tpRepetion = GetRepetion( vKey );
	REPE_ASSERT_STR( tpRepetion != NULL, return ERR_INVALID_PARAMETER, "Cannt Get Repetion, By Key %lld, BattleID %d",
		vKey, vBattleID );

	CRepetionInfo* tpInfo = GetRepetionMapInfo( vBattleID );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "RepetionID %d", vBattleID );

	if( tpInfo->MapIsInRepetion( tpPlayer->mMapID ) == true )
	{
		REPE_LOG( _LOG_INFO_, "Player( %d ) Is In Will Join Battle( %d : %lld ), Cannt Join New %lld",
			vCharID, vBattleID, tpPlayer->mJoinBattle[ vBattleID ].mKey, vKey );
		return SUCCESS;
	}

	if( vCampCountMax > 0 )
	{
		int tCount = tpRepetion->GetPlayerCountByCamp( vCampID );
		if( tCount >= vCampCountMax )
		{
			// 阵营人数满
			return ERR_REPETION_PLAYER_CAMP_FULL;
		}
	}

	tpRepetion->SetCampCountMax( vCampID, vCampCountMax );

	REPE_LOG( _LOG_NOTICE_, "Player ( %d : %s ) Is Will Join Battle( %d : %lld )", tpPlayer->mCharID,
		tpPlayer->GetCharNameShow( ), vBattleID, vKey );

	tpPlayer->mJoinBattle[ tpInfo->GetBeginMapID( ) ].mKey = vKey;
	tpPlayer->mJoinBattle[ tpInfo->GetBeginMapID( ) ].mTime = vTime;
	tpPlayer->mJoinBattle[ tpInfo->GetBeginMapID( ) ].mCamp = vCampID;

	CMessageBattleJoinNotice tMsg;
	CMessage tMessage;

	tMessage.set_msgpara( (uint32_t) &tMsg );
	pbmsg_sethead(&tMessage, ID_G2S_BATTLE_JOIN_NOTICE, 0, 0, 0, 0, 0, 0);

	tMsg.set_charid( vCharID );
	tMsg.set_battleid( vBattleID );
	tMsg.set_mapid( tpInfo->GetBeginMapID( ) );
	tMsg.set_type( tpRepetion->GetType( ) );
	tMsg.set_battletime( tpRepetion->GetInitTime() );
	tMsg.set_key( vKey );
	tMsg.set_intolastrepetion( bIntoLastRepetion );
	CGateServer::GetSingletonPtr( )->Send2Scene( tpPlayer, &tMessage );
	return SUCCESS;
}

bool CRepetionModule::IsBattleMap( int vMapID )
{
	CRepetionInfo* tpInfo = GetRepetionMapInfoByMapID( vMapID );
	if( tpInfo == NULL )
	{
		return false;
	}

	if( tpInfo->MapIsInRepetion( vMapID ) == false )
	{
		return false;
	}	

	if( tpInfo->GetRepetionIndex( ) == INVALID_REPETION_INDEX )
	{
		return false;
	}
	
	if( tpInfo->GetRepetionIndex( ) < 0 )
	{
		return false;
	}

	if( tpInfo->GetRepetionIndex( ) >= ( int )ARRAY_CNT( mWait ) )
	{
		return false;
	}

	if( mWait[ tpInfo->GetRepetionIndex( ) ][ 0 ].mLevelMin == 0 )
	{
		return false;
	}

	return true;
}

KEY_TYPE CRepetionModule::CreateBattleByIndex( int vBattleID, int vLevel, int vCreator, int vType, int vParame1, int vParame2 )
{
	CRepetion* tpRepetion = CreateRepetionByIndex( vBattleID, vLevel, vCreator, vType, vParame1, vParame2 );
	if( tpRepetion == NULL )
	{
		return INVITED_KEY_VALUE;
	}

	return tpRepetion->GetKey( );
}

int CRepetionModule::PlayerJoin( int vCharID, int vBattleID, KEY_TYPE vKey, int vCampID, int vCampCountMax /* = 0 */, bool nIntoLastRepetion /* = false */ )
{
	return SetPlayerJoin( vCharID, vBattleID, vKey, time( NULL ), vCampID, vCampCountMax, nIntoLastRepetion );
}

CLineRepetionInfo* CRepetionModule::RegistRepetionMap( int vLineID, int vMapID )
{
	REPE_ASSERT_STR( vLineID >= ( int )ARRAY_CNT( mLineRepetion ), return NULL, "LineID %d, MaxLineID %d", vLineID, 
		ARRAY_CNT( mLineRepetion ) );

	unsigned int i = 0;
	for( i = 0; i < ARRAY_CNT( mLineRepetion ); i++ )
	{
		if( mLineRepetion[ i ].GetLineID( ) == INVALID_LINE_ID )
		{
			mLineRepetion[ i ].SetLineID( vLineID );
		}

		if(  mLineRepetion[ i ].GetLineID( ) == vLineID )
		{
			mLineRepetion[ i ].RegisteMap( vMapID );
			return &mLineRepetion[ i ];
		}
	}

	return NULL;
}

CLineRepetionInfo* CRepetionModule::GetLineRepetionInfo( int vLineID )
{
    REPE_ASSERT_STR( vLineID >= ( int )ARRAY_CNT( mLineRepetion ), return NULL, "LineID %d, MaxLineID %d", vLineID,
        ARRAY_CNT( mLineRepetion ) );

	unsigned int i = 0;
	for( i = 0; i < ARRAY_CNT( mLineRepetion ); i++ )
	{
		if( mLineRepetion[ i ].GetLineID( ) == INVALID_LINE_ID )
		{
			return NULL;
		}

		if(  mLineRepetion[ i ].GetLineID( ) == vLineID )
		{
			return &mLineRepetion[ i ];
		}
	}

	return NULL;
}

CLineRepetionInfo* CRepetionModule::GetMostIdleLineID( int vMapID )
{
	unsigned int i = 0;
	int tFreeCountMax = 0;
	int tFreeLineID = INVALID_LINE_ID;

	for( i = 0; i < ARRAY_CNT( mLineRepetion ); i++ )
	{
		if( mLineRepetion[ i ].GetLineID( ) == INVALID_LINE_ID )
		{
			continue;
		}
	
		if( mLineRepetion[ i ].IsRegisteMap( vMapID ) == false )
		{
			continue;
		}
			
		int tCount = mLineRepetion[ i ].GetFreeCount( );
		if( tCount > tFreeCountMax )
		{
			tFreeLineID = i;
			tFreeCountMax = tCount;

//			LOG_NOTICE( "default", "[ %s : %d ][ %s ] FreeCount %d, Max %d, LineID %d, MapID %d", __LK_FILE__,
//				__LINE__, __FUNCTION__, tCount, tFreeCountMax, mLineRepetion[ i ].GetLineID( ), vMapID );
			REPE_LOG( _LOG_NOTICE_, "FreeCount %d, Max %d, LineID %d, MapID %d", tCount, tFreeCountMax,
				mLineRepetion[ i ].GetLineID( ), vMapID );
		}
	}

	return ( tFreeLineID != INVALID_LINE_ID ) ? &mLineRepetion[ tFreeLineID ] : NULL;
}



int CRepetionModule::NotifyPlayerIntoRepetion( 
	CGatePlayer* vpPlayer, 
	int vLineID, 
	int vMapID, 
	KEY_TYPE vKey, 
	int vPosX, 
	int vPosY
	)
{
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	
	vpPlayer->mRepetionMap[ vMapID ] = vKey;

	CMessage tMsg;
	CMessageIntoRepetionNotify tNotify;

	tNotify.set_lineid( vLineID );
	tNotify.set_mapid( vMapID );
	tNotify.set_key( vKey );
	tNotify.set_posx( vPosX );
	tNotify.set_posy( vPosY );
	tNotify.set_charid( vpPlayer->mCharID );

	tMsg.set_msgpara( ( uint32_t ) &tNotify );
	pbmsg_sethead( &tMsg, ID_G2S_INTO_REPETION_NOTIFY, 0, 0, 0, 0, 0, 0 );


	CGateServer::GetSingletonPtr( )->Send2Scene( vpPlayer, &tMsg );

	return SUCCESS;
}



int CRepetionModule::OnHostDown( CSceneServer *pHost )
{
	REPETION_LIST_TYPE::iterator it;
	for( it = REPETION_LIST.begin( ); it != REPETION_LIST.end( ); )
	{
		CRepetion* tpRepetion = &( *( it ) );
		if( tpRepetion != NULL )
		{
			CSceneMap* tpSceneMap = CGateServer::GetSingletonPtr( )->GetMapByLineMap( tpRepetion->GetLineID( ),
				tpRepetion->GetMapID( ) );
			if( tpSceneMap == NULL )
			{
				it++;
				DestroyRepetion( tpRepetion, REPETION_END_REBOOT );
				continue;
			}
	
			if( tpSceneMap->mServerObjID == pHost->get_id( ) )
			{
				it++;
				DestroyRepetion( tpRepetion, REPETION_END_REBOOT );
				continue;
			}	
		}	
		it++;
	}
	
	return SUCCESS;
}

bool CRepetionModule::IsRepetionMap( int vMapID )
{
	CRepetionInfo* tpInfo = GetRepetionMapInfoByMapID( vMapID );
	return ( tpInfo != NULL ) ? tpInfo->MapIsInRepetion( vMapID ) : false;
}

int CRepetionModule::RemoveFromAllWait( int vCharID )
{
	for( unsigned int i = 0; i < ARRAY_CNT( mRepetionInfo ); i++ )
	{
		if( IsBattleMap( mRepetionInfo[ i ].GetBeginMapID( ) ) == true )
		{
			CRepetionInfo* tpInfo = GetRepetionMapInfoByMapID( mRepetionInfo[ i ].GetBeginMapID( ) );
			if( tpInfo != NULL )
			{
				RemoveFromWait( vCharID, tpInfo->GetRepetionIndex( ) );
			}
		}
	}

	return SUCCESS;
}



int CRepetionModule::CreatRepetionLog( CRepetion* vpRepetion, int vCreator, int vParame )
{
	CMessageLogRepetion tLog;
	CMessage tMsg;

	tLog.set_repetionkey( ( vpRepetion != NULL ) ? vpRepetion->GetKey( ) : INVITED_KEY_VALUE );
	tLog.set_parame1( vCreator );
	tLog.set_index( ( vpRepetion != NULL ) ? vpRepetion->GetRepetionIndex( ) : INVALID_REPETION_INDEX );
	tLog.set_optime( LINEKONG::GetTickCount() / 1000 );
	tLog.set_parame2( vParame );
	tLog.set_type( 1 );
	tLog.set_lineid( ( vpRepetion != NULL ) ? vpRepetion->GetLineID( ) : INVALID_LINE_ID );

	
    tMsg.set_msgpara((uint32_t) &tLog );
    pbmsg_sethead(&tMsg, ID_G2L_REPETION, 0, 0, 0, 0, 0, 0);

	CGateServer::GetSingletonPtr( )->Send2Log( &tMsg, 0 );

	return SUCCESS;
}

int CRepetionModule::DestroyRepetionLog( CRepetion* vpRepetion, int vReason, int vParame )
{
	CMessageLogRepetion tLog;
	CMessage tMsg;

	tLog.set_repetionkey( ( vpRepetion != NULL ) ? vpRepetion->GetKey( ) : INVITED_KEY_VALUE );
	tLog.set_parame2( vReason );
	tLog.set_type( 2 );
	tLog.set_optime( LINEKONG::GetTickCount() / 1000 );
	tLog.set_lineid( ( vpRepetion != NULL ) ? vpRepetion->GetLineID( ) : INVALID_LINE_ID );
	tLog.set_parame1( vParame );
	tLog.set_index( ( vpRepetion != NULL ) ? vpRepetion->GetRepetionIndex( ) : INVALID_REPETION_INDEX );

	
    tMsg.set_msgpara((uint32_t) &tLog );
    pbmsg_sethead(&tMsg, ID_G2L_REPETION, 0, 0, 0, 0, 0, 0);

	CGateServer::GetSingletonPtr( )->Send2Log( &tMsg, 0 );

	return SUCCESS;
}

int CRepetionModule::SetBattleOpenSpace( int vRepetionIndex, const PBDateSpace* vSpace )
{
	CRepetionInfo* tpInfo = GetRepetionMapInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, " vRepetionIndex %d", vRepetionIndex );
	REPE_ASSERT( vSpace != NULL, return ERR_INVALID_PARAMETER );

	DateSpace tSpace;
	tSpace.mBegin.mYear = vSpace->start( ).year( );
	tSpace.mBegin.mDay = vSpace->start( ).day( );
	tSpace.mBegin.mMonth = vSpace->start( ).month( );
	tSpace.mBegin.mWeek = vSpace->start( ).week( );
	tSpace.mBegin.mHr = vSpace->start( ).hr( );
	tSpace.mBegin.mMinute = vSpace->start( ).minute( );
	tSpace.mBegin.mSec = vSpace->start( ).sec( );

	tSpace.mEnd.mYear = vSpace->finish( ).year( );
	tSpace.mEnd.mDay = vSpace->finish( ).day( );
	tSpace.mEnd.mMonth = vSpace->finish( ).month( );
	tSpace.mEnd.mWeek = vSpace->finish( ).week( );
	tSpace.mEnd.mHr = vSpace->finish( ).hr( );
	tSpace.mEnd.mMinute = vSpace->finish( ).minute( );
	tSpace.mEnd.mSec = vSpace->finish( ).sec( );

	tpInfo->SetBattleOpenSpace( &tSpace );


	return SUCCESS;
}

int CRepetionModule::ClearBattleOpenSpace( int vRepetionIndex )
{
	CRepetionInfo* tpInfo = GetRepetionMapInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, " %d", vRepetionIndex );

	return tpInfo->ClearBattleOpenSpace( );
}

int CRepetionModule::CleanWaitList( int vRepetionIndex )
{
	REPE_ASSERT_STR( vRepetionIndex > 0 && vRepetionIndex < ( int )ARRAY_CNT( mWait ), return ERR_INVALID_PARAMETER, 
		"vRepetionIndex %d", vRepetionIndex );	
	
	for( unsigned int i = 0; i < ARRAY_CNT( mWait[ vRepetionIndex ] ); i++ )
	{
		BATTLE_WAIT_LIST::iterator it = mWait[ vRepetionIndex ][ i ].mList.begin( );
		for( ; it != mWait[ vRepetionIndex ][ i ].mList.end( ); it++ )
		{
			BattleWaitPlayer* tpWaitPlayer = &(*it);
			for( unsigned int j = 0; j < ARRAY_CNT( tpWaitPlayer->mPlayers ) 
				&& j < ( unsigned int  )tpWaitPlayer->mPlayerCount; j++ )
			{
				int tRoleID = tpWaitPlayer->mPlayers[ j ];
				REPE_LOG( _LOG_DEBUG_, "[ %s : %d ][ %s ] Role %d, Leave Battle %d Wait List", __LK_FILE__, __LINE__,
					__FUNCTION__, tRoleID,vRepetionIndex );
				CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( tRoleID );
				SendEventCode2Scene( tpPlayer, REPETION_BATTLE_LEAVE_WAITLIST, vRepetionIndex );
				
			}

			tpWaitPlayer->Clear( );
		}				
		mWait[ vRepetionIndex ][ i ].mList.clear( );
	}
	return SUCCESS;
}

/* --------------------------------------------------------------------------*/
/*                                                     
* @Synopsis 把固定时间的报名清除， （报名的时间段绝对不可以重合）                                                                              *     
* @Param vRepetionIndex      副本索引                              
* @Returns                                             
*/
/* ----------------------------------------------------------------------------*/

int CRepetionModule::CleanFixWaitList( int vRepetionIndex )
{
	REPE_ASSERT_STR( vRepetionIndex > 0 && vRepetionIndex < ( int )ARRAY_CNT( mFixWait ), return ERR_INVALID_PARAMETER, 
		"vRepetionIndex %d", vRepetionIndex );	

	for( unsigned int i = 0; i < ARRAY_CNT( mFixWait[ vRepetionIndex ] ); i++ )
	{
		BATTLE_WAIT_LIST::iterator it = mFixWait[ vRepetionIndex ][ i ].mList.begin( );
		for( ; it != mFixWait[ vRepetionIndex ][ i ].mList.end( ); it++ )
		{
			BattleWaitPlayer* tpWaitPlayer = &(*it);
			for( unsigned int j = 0; j < ARRAY_CNT( tpWaitPlayer->mPlayers ) 
				&& j < ( unsigned int  )tpWaitPlayer->mPlayerCount; j++ )
			{
				int tRoleID = tpWaitPlayer->mPlayers[ j ];
				REPE_LOG( _LOG_DEBUG_, "[ %s : %d ][ %s ] Role %d, Leave Battle %d Wait List", __LK_FILE__, __LINE__,
					__FUNCTION__, tRoleID,vRepetionIndex );
				CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( tRoleID );
				SendEventCode2Scene( tpPlayer, REPETION_BATTLE_LEAVE_WAITLIST, vRepetionIndex );

			}

			tpWaitPlayer->Clear( );
		}				
		mFixWait[ vRepetionIndex ][ i ].mList.clear( );
	}
	return SUCCESS;
}




int CRepetionModule::SendEventCode2Scene( CGatePlayer* vpPlayer, int vEventCode, int vRepetionID )
{
	REPE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	CMessageG2SRepetionEventCode tMove;
	CMessage tMsg;

	tMove.set_roleid( vpPlayer->CharID( ) );
	tMove.set_code( vEventCode );
	tMove.set_repetionid( vRepetionID );

	tMsg.set_msgpara( (uint32_t) &tMove );
	pbmsg_sethead( &tMsg, ID_G2S_REPETION_EVENT_CODE, 0, 0, 0, 0, 0, 0 );

	CGateServer::GetSingleton().Send2Scene( vpPlayer, &tMsg );

	return SUCCESS;
}

//队伍固定时间报名

/* --------------------------------------------------------------------------*/
/** 
* @Synopsis 组队固定时间报名 
* 
* @Param vCharID
* @Param vLevel
* @Param vBattleID
* 
* @Returns   
    */
/* ----------------------------------------------------------------------------*/

int CRepetionModule::TeamFixWaitBattle( int vCharID, int vLevel, unsigned int vBattleID )
{

	CRepetionInfo* tpInfo = GetRepetionMapInfo( vBattleID );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "vBattleID %d", vBattleID );
	REPE_ASSERT_STR( tpInfo->GetRepetionIndex( ) != INVALID_REPETION_INDEX, return ERR_INVALID_PARAMETER, "vBattleID %d",
		vBattleID );

	unsigned int i = 0;
	unsigned int j = 0;
	int tRepetionIndex = tpInfo->GetRepetionIndex( );

	REPE_ASSERT_STR( tRepetionIndex >= 0, return ERR_INVALID_PARAMETER, "tRepetinIndex %d", tRepetionIndex );
	REPE_ASSERT_STR( tRepetionIndex < ( int )ARRAY_CNT( mFixWait ), return ERR_INVALID_PARAMETER, "tRepetionIndex %d", 
		tRepetionIndex );

	int tMemberList[ TEAM_NUMBER_MAX ] = { 0 };	
	int tNum = ( int )ARRAY_CNT( tMemberList );	
	BattleWaitPlayer tWait;
	int tRtn = ERR_REPETION_PVP_LEVEL_NOTFOUND;

	for( i = 0; i < ARRAY_CNT( mFixWait[ tRepetionIndex ] ); i++ )
	{
		if( vLevel >= mFixWait[ tRepetionIndex ][ i ].mLevelMin )
		{
			if( mFixWait[ tRepetionIndex ][ i ].mLevelMax != 0 && vLevel > mFixWait[ tRepetionIndex ][ i ].mLevelMax)
			{
                                continue;
			}

			tNum = CServiceGateTeam::GetSingletonPtr( )->GetTeamList( vCharID, tMemberList, tNum );

			if( mFixWait[ tRepetionIndex ][ i ].mList.size( ) + tNum > mFixWait[ tRepetionIndex ][ i ].mList.max_size( ) )
			{
				//等待队列满
				tRtn = ERR_REPETION_PVP_WAITLIST_FULL;
				break;
			}


			for( j = 0; ( int )j < tNum && j < ARRAY_CNT( tWait.mPlayers ) && j < ARRAY_CNT( tMemberList ); j++ )
			{
				CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( tMemberList[ j ] );
				REPE_ASSERT_STR( tpPlayer != NULL, continue, "Cannt Get ScenePlayer By %d", tMemberList[ j ] );

				if( tpInfo->MapIsInRepetion( tpPlayer->mMapID ) == true )
				{


					REPE_LOG( _LOG_INFO_, "Player( %d ) In Battle( %d : %lld ) Cannt Join Fix Wait List", tpPlayer->mCharID,
						vBattleID, tpPlayer->mJoinBattle[ tpPlayer->mMapID ].mKey );
					continue;
				}

				RemoveFromWait( tMemberList[ j ], vBattleID );
				tWait.mPlayers[ j ] = tMemberList[ j ];

				CMessageBattleWaitNotice tWaitNotice;
				CMessage tMsg;

				tWaitNotice.set_charid( tMemberList[ j ] );
				tWaitNotice.set_battleid( vBattleID ); 
				tWaitNotice.set_mapid( tpInfo->GetBeginMapID( ) );

				tMsg.set_msgpara( (uint32_t) &tWaitNotice );
				pbmsg_sethead(&tMsg, ID_G2S_BATTLE_WAIT_NOTICE, 0, 0, 0, 0, 0, 0);

				CGateServer::GetSingletonPtr( )->Send2Scene( tpPlayer, &tMsg );	
			}
                        

			tWait.mPlayerCount = j;

			mFixWait[ tRepetionIndex ][ i ].mList.push_back( tWait );

			tRtn = SUCCESS;

			break;

         }
     }

        return tRtn;
}

/* --------------------------------------------------------------------------*/
/** 
* @Synopsis  单人固定时间报名
* 
* @Param vCharID
* @Param vLevel
* @Param vBattleID
* 
* @Returns   
*/
/* ----------------------------------------------------------------------------*/

int CRepetionModule::SingleFixWaitBattle( int vCharID, int vLevel, unsigned int vBattleID )
{
	CRepetionInfo* tpInfo = GetRepetionMapInfo( vBattleID );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "vBattleID %d", vBattleID );
	REPE_ASSERT_STR( tpInfo->GetRepetionIndex( ) != INVALID_REPETION_INDEX, return ERR_INVALID_PARAMETER, "vBattleID %d",
		vBattleID );

	unsigned int i = 0;
	unsigned int j = 0;
	int tRepetionIndex = tpInfo->GetRepetionIndex( );

	REPE_ASSERT_STR( tRepetionIndex >= 0, return ERR_INVALID_PARAMETER, "tRepetinIndex %d", tRepetionIndex );
	REPE_ASSERT_STR( tRepetionIndex < ( int )ARRAY_CNT( mFixWait ), return ERR_INVALID_PARAMETER, "tRepetionIndex %d", 
		tRepetionIndex );

	int tRtn = ERR_REPETION_PVP_LEVEL_NOTFOUND;
	BattleWaitPlayer tWait;


	for( i = 0; i < ARRAY_CNT( mFixWait[ tRepetionIndex ] ); i++ )
	{
		if( vLevel >= mFixWait[ tRepetionIndex ][ i ].mLevelMin )
		{
			if( mFixWait[ tRepetionIndex ][ i ].mLevelMax != 0 && vLevel > mFixWait[ tRepetionIndex ][ i ].mLevelMax)
			{
                                continue;
			}

			if( mFixWait[ tRepetionIndex ][ i ].mList.size( ) >= mFixWait[ tRepetionIndex ][ i ].mList.max_size( ) )
			{
				//等待队列满
				tRtn = ERR_REPETION_PVP_WAITLIST_FULL;
				break;
			}
                        
                        CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( vCharID );
						REPE_ASSERT_STR( tpPlayer != NULL, return ERR_INVALID_PARAMETER, "vCharID %d", vCharID );

                        if( tpInfo->MapIsInRepetion( tpPlayer->mMapID ) == true )
                        {


                                REPE_LOG( _LOG_INFO_, "Player( %d ) In Battle( %d : %lld ) Cannt Join Fix Wait List", tpPlayer->mCharID,
                                        vBattleID, tpPlayer->mJoinBattle[ tpPlayer->mMapID ].mKey );
                                continue;
                        }

                        RemoveFromWait( vCharID , vBattleID );
                        tWait.mPlayers[ 0 ] = vCharID;

                        CMessageBattleWaitNotice tWaitNotice;
                        CMessage tMsg;

                        tWaitNotice.set_charid( vCharID );
                        tWaitNotice.set_battleid( vBattleID ); 
                        tWaitNotice.set_mapid( tpInfo->GetBeginMapID( ) );

                        tMsg.set_msgpara( (uint32_t) &tWaitNotice );
                        pbmsg_sethead(&tMsg, ID_G2S_BATTLE_WAIT_NOTICE, 0, 0, 0, 0, 0, 0);

                        CGateServer::GetSingletonPtr( )->Send2Scene( tpPlayer, &tMsg );	

			tWait.mPlayerCount = 1;

			mFixWait[ tRepetionIndex ][ i ].mList.push_back( tWait );

			tRtn = SUCCESS;

			break;
           }
        }
        return tRtn;
}

int CRepetionModule::RemoveFromFixWait( int vCharID, unsigned int vBattleID )
{
	CRepetionInfo* tpInfo = GetRepetionMapInfo( vBattleID );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "vBattleID %d", vBattleID );
	REPE_ASSERT_STR( tpInfo->GetRepetionIndex( ) != INVALID_REPETION_INDEX, return ERR_INVALID_PARAMETER, "vBattleID %d",
		vBattleID );

	unsigned int i = 0;
	unsigned int j = 0;
	int tRepetionIndex = tpInfo->GetRepetionIndex( );
	BATTLE_FIX_WAIT_LIST::iterator it;

	REPE_ASSERT_STR( tRepetionIndex >= 0, return ERR_INVALID_PARAMETER, "tRepetinIndex %d", tRepetionIndex );
	REPE_ASSERT_STR( tRepetionIndex < ( int )ARRAY_CNT( mFixWait ), return ERR_INVALID_PARAMETER, "tRepetionIndex %d", 
		tRepetionIndex );

	for( i = 0; i < ARRAY_CNT( mFixWait[ tRepetionIndex ] ); i++ )
	{
		if( mFixWait[ tRepetionIndex ][ i ].mLevelMin == 0 )
		{
			continue;
		}

		for( it = mFixWait[ tRepetionIndex ][ i ].mList.begin( ); it != mFixWait[ tRepetionIndex ][ i ].mList.end( ); it++ )
		{
			BattleWaitPlayer* tpWaitPlayer = &(*it);
			REPE_ASSERT_STR( tpWaitPlayer != NULL, continue, "Cannt Get Iteraot Type Form lk::vector" );

			if( tpWaitPlayer->mPlayerCount == 0 )
			{
				continue;
			}

			for( j = 0; j < ARRAY_CNT( tpWaitPlayer->mPlayers ); j++ )
			{
				if( tpWaitPlayer->mPlayers[ j ] == 0 )
				{

					REPE_LOG( _LOG_DEBUG_, "Wait List Iterator PlayerCount Is 0" );
					continue;
				}

				if( tpWaitPlayer->mPlayers[ j ] == vCharID )
				{
					// 找到Player, 将其从等待列表中删除

					CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( vCharID );

					REPE_ASSERT_STR( tpPlayer != NULL, continue, "Cannt Get ScenePlayer %d, MayBe Is Leaved", vCharID )
					tpWaitPlayer->mPlayers[ j ] = 0;
					tpWaitPlayer->mPlayerCount--;

					if( tpWaitPlayer->mPlayerCount <= 0 )
					{
						mFixWait[ tRepetionIndex ][ i ].mList.erase( it );			
					}

					CMessageBattleWaitLeaveResponse tMsg;
					CMessage tMessage;


					tMsg.set_charid( vCharID );
					tMsg.set_battleid( vBattleID );
					tMsg.set_mapid( tpInfo->GetBeginMapID( ) );

					tMessage.set_msgpara( (uint32_t) &tMsg );
					pbmsg_sethead(&tMessage, ID_G2S_BATTLE_WAIT_LEAVE_RESPONSE, 0, 0, 0, 0, 0, 0);

					CGateServer::GetSingletonPtr( )->Send2Scene( tpPlayer, &tMessage );
					
					return SUCCESS;
				}
			}		// for tpWaitPlayer->mPlayers
		}			// for mList	
	}				// for mWait

	return ERR_NOT_FOUND;
}
int CRepetionModule::SetFixWaitListLevel( int vRepetionIndex, int vLevelMin, int vLevelMax )
{
	CRepetionInfo* tpInfo = GetRepetionMapInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "vRepetion %d", vRepetionIndex );
	REPE_ASSERT_STR( tpInfo->GetRepetionIndex( ) != INVALID_REPETION_INDEX, return ERR_INVALID_PARAMETER, 
		"vRepetionIndex %d", vRepetionIndex );

	int tRepetionIndex = tpInfo->GetRepetionIndex( );

	REPE_ASSERT_STR( tRepetionIndex >= 0, return ERR_INVALID_PARAMETER, "tRepetinIndex %d", tRepetionIndex );
	REPE_ASSERT_STR( tRepetionIndex < ( int )ARRAY_CNT( mWait ), return ERR_INVALID_PARAMETER, "tRepetionIndex %d", 
		tRepetionIndex );

	for( unsigned int i = 0; i < ARRAY_CNT( mFixWait[ tRepetionIndex ] ); i++ )
	{
		if( mFixWait[ tRepetionIndex ][ i ].mLevelMin == 0 )
		{
			mFixWait[ tRepetionIndex ][ i ].mLevelMin = vLevelMin;
			mFixWait[ tRepetionIndex ][ i ].mLevelMax = vLevelMax;
			break;
		}
	}

	return SUCCESS;
}

int CRepetionModule::SetOnTimeOpenListLevel( int vRepetionIndex, int vLevelMin, int vLevelMax )
{
	CRepetionInfo* tpInfo = GetRepetionMapInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "vRepetion %d", vRepetionIndex );
	REPE_ASSERT_STR( tpInfo->GetRepetionIndex( ) != INVALID_REPETION_INDEX, return ERR_INVALID_PARAMETER, 
		"vRepetionIndex %d", vRepetionIndex );

	int tRepetionIndex = tpInfo->GetRepetionIndex( );

	REPE_ASSERT_STR( tRepetionIndex >= 0, return ERR_INVALID_PARAMETER, "tRepetinIndex %d", tRepetionIndex );
	REPE_ASSERT_STR( tRepetionIndex < ( int )ARRAY_CNT( mWait ), return ERR_INVALID_PARAMETER, "tRepetionIndex %d", 
		tRepetionIndex );

	for( unsigned int i = 0; i < ARRAY_CNT( mOnTimeRepetion[ tRepetionIndex ].mOnTimeLevelLimit ); i++ )
	{
		if( mOnTimeRepetion[ tRepetionIndex ].mOnTimeLevelLimit[ i ].mLevelMin == 0 )
		{
			mOnTimeRepetion[ tRepetionIndex ].mOnTimeLevelLimit[ i ].mLevelMin = vLevelMin;
			mOnTimeRepetion[ tRepetionIndex ].mOnTimeLevelLimit[ i ].mLevelMax = vLevelMax;
			break;
		}
	}

	return SUCCESS;
}

int CRepetionModule::CreateOnTimeOpenRepetion( int vRepetionIndex )
{
	CRepetionInfo* tpInfo = GetRepetionMapInfo( vRepetionIndex );
	REPE_ASSERT_STR( tpInfo != NULL, return ERR_INVALID_PARAMETER, "vRepetion %d", vRepetionIndex );

	for ( unsigned int j = 0 ;j < ARRAY_CNT( mOnTimeRepetion[ vRepetionIndex].mOnTimeLevelLimit ); j++)
	{

		OnTimeOpenLevelLimit * tLevelLimit = &mOnTimeRepetion[ vRepetionIndex ].mOnTimeLevelLimit[ j ];
		if ( tLevelLimit->mLevelMin == 0 )
		{
			continue;
		}

		int vParame = ( tLevelLimit->mLevelMax > tLevelLimit->mLevelMin ) ? 
			tLevelLimit->mLevelMax : tLevelLimit->mLevelMin;			
		CRepetion* tpRepetion = CreateRepetionByMapID( mRepetionInfo[ vRepetionIndex ].GetBeginMapID( ), vParame, 0, 0 );
		// 副本资源耗尽	
		//创建没有成功，应该销毁前面创建的
		if ( tpRepetion == NULL)
		{
			for ( unsigned int m = 0; j < ARRAY_CNT( mOnTimeRepetion[ vRepetionIndex ].mOnTimeLevelLimit ); j++)
			{
				OnTimeOpenLevelLimit * tLevelLimit = &mOnTimeRepetion[ vRepetionIndex].mOnTimeLevelLimit[ m ];
				if ( tLevelLimit->mLevelMin == 0 )
				{
					continue;
				}
				CRepetion * tpRep =GetRepetion( tLevelLimit->mKey );
				if ( tpRep != NULL )
				{
					DestroyRepetion( tpRep, REPETION_END_COMMON);
				}
				
			}
			REPE_LOG( _LOG_ERROR_, "ontime open repetion create lost repetion_index is %d", vRepetionIndex );
			return ERR_FULL;
		}

		KEY_TYPE tKey = tpRepetion->GetKey( );
		tLevelLimit->mKey = tKey;

	}


	int tIntParam[ 2 ] = { 0 };
	tIntParam[ 0 ] = vRepetionIndex;
	tIntParam[ 1 ] = mRepetionInfo[ vRepetionIndex ].GetBeginMapID( );

	CWorldModule::GetSingleton().SendEventNotify( NOTIFY_BATTLE_ONTIME_OPEN, 0, 0, tIntParam, ARRAY_CNT(tIntParam), NULL, NULL );

	REPE_LOG( _LOG_INFO_, "repetion  open %d", vRepetionIndex );
	return SUCCESS;

}