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

int CRepetion::Final( )
{
	PLAYER_LIST_TYPE::iterator it;

	for( it = mPlayers.begin( ); it != mPlayers.end( ); it++ )
	{
		CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( *it );
		REPE_ASSERT_STR( tpPlayer != NULL, continue, " CharID : %d", *it );
		REPE_LOG( _LOG_NOTICE_, "Repetion %lld Will Destroyed, Player %d Kicked Out", GetKey( ), tpPlayer->mCharID );

		tpPlayer->mRepetionMap[ mMapID ] = INVITED_KEY_VALUE;
	}

	mKey = INVITED_KEY_VALUE;
	mMapID = INVALID_MAP_ID;
	mLineID = INVALID_LINE_ID;
	mPlayers.clear( );
	return SUCCESS;
}
int CRepetion::GetPlayerCountByCamp( int vCampID )
{
	PLAYER_LIST_TYPE::iterator it;
	int tCount = 0;

	CRepetionInfo* tpInfo = GetRepetionInfo( );
	REPE_ASSERT_STR( tpInfo != NULL, return 0, "Index %d, Key %lld", GetRepetionIndex( ), GetKey( ) );

	for( it = mPlayers.begin( ); it != mPlayers.end( ); it++ )
	{
		CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( *it );
		REPE_ASSERT_STR( tpPlayer != NULL, continue, "Cannt GetPlayer %d", *it );

		if( tpPlayer->mJoinBattle[ tpInfo->GetBeginMapID( ) ].mCamp == vCampID )
		{
			tCount++;
		}
	}

	return tCount;
}

bool CRepetion::IsCanIntoRepeByCamp( CGatePlayer* vpPlayer )
{
	REPE_ASSERT( vpPlayer != NULL, return false );
	CRepetionInfo* tpInfo = GetRepetionInfo( );

	REPE_ASSERT_STR( tpInfo != NULL, return false, "RepetionIndex %d, Key %lld", GetRepetionIndex( ), GetKey( ) );

	if( vpPlayer->mJoinBattle[ tpInfo->GetBeginMapID( ) ].mCamp == 0 )
	{
		return true;	
	}

	int tCount = GetPlayerCountByCamp( vpPlayer->mJoinBattle[ tpInfo->GetBeginMapID( ) ].mCamp );

	for( unsigned int i = 0; i < ARRAY_CNT( mCampInfo ); i++ )
	{
		if( mCampInfo[ i ].mCampID == vpPlayer->mJoinBattle[ tpInfo->GetBeginMapID( ) ].mCamp )
		{
			if( tCount >= mCampInfo[ i ].mMaxCount )
			{
				return false;
				break;
			}
		}	
	}

	return true;
}

int CRepetion::SetCampCountMax( int vCampID, int vCampMaxCount )
{
	if( vCampID != 0 && vCampMaxCount != 0 )
	{
		for( int i = 0; i < ( int )ARRAY_CNT( mCampInfo ); i++ )
		{
			if( mCampInfo[ i ].mCampID == 0 || mCampInfo[ i ].mCampID == vCampID )
			{
				mCampInfo[ i ].mMaxCount = vCampMaxCount;
				mCampInfo[ i ].mCampID = vCampID;
				break;
			}
		}
	}

	return SUCCESS;
}

// 获取玩家角色ID
int CRepetion::GetRoleIDList( int *pRoleIDList, int &rNum )
{
	LK_ASSERT( pRoleIDList != NULL, return  -1 )
	PLAYER_LIST_TYPE::iterator it = mPlayers.begin();
	int tMaxNum = rNum;
	rNum = 0;
	for ( ; it != mPlayers.end(); ++it )
	{
		if ( rNum >= tMaxNum )
		{
			break;
		}
		pRoleIDList[rNum] = *it;
		++rNum;
	}
	return SUCCESS;
}

unsigned int CRepetion::GetPlayerCountMax( )
{
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionMapInfo( GetRepetionIndex( ) );
	unsigned int tPlayerMax = 0;
	if( tpInfo != NULL )
	{
		tPlayerMax = tpInfo->GetPlayerCountMax( );
		return min( tPlayerMax, mPlayers.max_size( ) );
	}

	return 0;
}

unsigned int CRepetion::GetPlayerCountCur( )
{
	return mPlayers.size( );
}

bool CRepetion::InRepetion( unsigned int vCharID )
{
	PLAYER_LIST_TYPE::iterator it;

	for( it = mPlayers.begin( ); it != mPlayers.end( ); it++ )
	{
		if( vCharID == *it )
		{
			return true;
		}	
	}

	return false;
}

int CRepetion::InsertPlayer( unsigned int vCharID )
{
	if( InRepetion( vCharID ) == false )
	{
		if( GetPlayerCountMax( ) > GetPlayerCountCur( ) )
		{
			mPlayers.push_back( vCharID );
			return SUCCESS;
		}
		return ERR_FULL;
	}

	return SUCCESS;//ERR_EXIST;
}

int CRepetion::RemovePlayer( unsigned int vCharID )
{
	PLAYER_LIST_TYPE::iterator it;

	for( it = mPlayers.begin( ); it != mPlayers.end( ); it++ )
	{
		if( *it == vCharID )
		{
			mPlayers.erase( it );
			return SUCCESS;
		}
	}

	return ERR_NOT_FOUND;
}

CRepetionInfo* CRepetion::GetRepetionInfo( )
{
	return CRepetionModule::GetSingletonPtr( )->GetRepetionMapInfo( GetRepetionIndex( ) );
}

CRepetionInfo* CRepetionModule::GetRepetionMapInfo( int vRepetionIndex )
{
	REPE_ASSERT_STR( vRepetionIndex >= 0, return NULL, "vRepetionIndex %d", vRepetionIndex );
	REPE_ASSERT_STR( vRepetionIndex < ( int )ARRAY_CNT( mRepetionInfo ), return NULL, 
		"vRepetionIndex %d", vRepetionIndex );

	return &mRepetionInfo[ vRepetionIndex ];
}

int CRepetion::OnTimer( unsigned int vTickOffset, DELETE_VECTOR* vDeleteList )
{
	mTimer -= vTickOffset;
	if( mTimer <= 0 )
	{
		if( vDeleteList->empty( ) == true )
		{
			vDeleteList->push_back( get_id( ) );
		}
	}	
	return SUCCESS;
}

int CRepetionInfo::ClearBattleOpenSpace( )
{
	mOpenDateSpace.clear( );

	return SUCCESS;
}

int CRepetionInfo::SetBattleOpenSpace( DateSpace* vSpace )
{
	REPE_ASSERT( vSpace != NULL, return ERR_INVALID_PARAMETER );
	mOpenDateSpace.push_back( *vSpace );
	return SUCCESS;
}

bool CRepetionInfo::InBattleTime( struct tm* vTm )
{
	DATE_SPACE_LIST::iterator it;

	if( mOpenDateSpace.size( ) == 0 )
	{
		return true;
	}

	for( it = mOpenDateSpace.begin( ); it != mOpenDateSpace.end( ); it ++ )
	{
		if( ( *it ).InDateSpace( vTm ) == true )
		{
			return true;
		}
	}

	return false;
}

bool CRepetionInfo::IsBattleOn( )
{
	return mBattleOn;
}

int CRepetionInfo::SetBattleOnStatus( bool IsOn )
{
	mBattleOn = IsOn;
	return SUCCESS;	
}







