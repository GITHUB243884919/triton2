
//////////////////////////////////////////////////////////////////////////
// CPlayerRepetion
//////////////////////////////////////////////////////////////////////////
#include "stdafx.hpp"
#include "proclient_svr.h"
#include "coremessage_pb.hxx.pb.h"
#include "promessage_pb.hxx.pb.h"
#include "sceneobj_manager.h"
#include "scenelogic.h"
#include "westserver.h"
#include "servercore.h"
#include "entity.h"
#include "template.h"
#include "property.h"
#include "entity.h"
#include "pro_define.h"
#include "propertymodule.h"
#include "scenecfg_manager.h"
#include "repetionmodule.h"
#include <llimits.h>
#include "buffer.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

int CPlayerRepetion::PB2Repetion( PBRepetion* vpPB, CEntityPlayer* vpPlayer )
{       
	LK_ASSERT( vpPB != NULL, return ERR_INVALID_PARAMETER );
	LK_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	SetBackLineID( vpPB->backlineid(  ) );
	SetBackMapID( vpPB->backmapid( ) );
	SetPosX( vpPB->backposx( ) );
	SetPosY( vpPB->backposy( ) );
	if( GetCampID( ) == 0 )
	{
		SetCampID( vpPB->campid( ) );
	}

	unsigned int tSize = vpPB->mapinfo_size( );
	unsigned int i = 0;

	if( tSize > 0 )
	{
		for( i = 0; i < tSize; i++ )
		{
			int tMapID = vpPB->mapinfo( i ).mapid( );
			CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfoByMapID( tMapID );
			// 如果是用老的数据结构，是不可能有后面这几项的
			if( tpInfo != NULL )
			{
				SetRepetion( tpInfo->GetRepetionIndex( ), vpPB->mapinfo( i ).count( ), 
					vpPB->mapinfo( i ).weekcount( ) , 0, 0, 0);
			}

			unsigned int tDataSize = vpPB->mapinfo( i ).data_size( );
			unsigned int j = 0;
			for( j = 0; j < tDataSize; j++ )
			{
				SetIntData( tpInfo->GetRepetionIndex( ), vpPB->mapinfo( i ).data( j ).index( ), 
					vpPB->mapinfo( i ).data( j ).data( ) );
			}
		}
	}
	else
	{
		tSize = vpPB->repeinfo_size( );
		for( i = 0; i < tSize; i++ )
		{
			SetRepetion( vpPB->repeinfo( i ).repeindex( ), vpPB->repeinfo( i ).count( ), 
				vpPB->repeinfo( i ).weekcount( ) , vpPB->repeinfo( i ).signup(),vpPB->repeinfo( i ).intotime());
			unsigned int tDataSize = vpPB->repeinfo( i ).repetiondata_size( );
			unsigned int j = 0;
			for( j = 0; j < tDataSize; j++ )
			{
				SetIntData( vpPB->repeinfo( i ).repeindex( ), vpPB->repeinfo( i ).repetiondata( j ).index( ), 
					vpPB->repeinfo( i ).repetiondata( j ).data( ) );
			}
		}		
	}
	mCheckResult = vpPB->checkresult();
	mAnswerCount = vpPB->answercount();
	SetOldRepeKey( vpPB->key( ) );
	mGlobalSceneStatus = vpPB->globalscenestatus();
	mGlobalWarOfflineTime = vpPB->globalwarofflinetime();
	mRepetionKey = vpPB->repetionkey();
#ifndef _FB_
	if( vpPB->inrepetion( ) == 1 && vpPB->repetionindex( ) != 0 )
	{
		LOG_NOTICE( "rep", "[ %s : %d ][ %s ] Player( %s : %d : %d ) Unusual Out Repetion %d, Count Dec",
			__LK_FILE__, __LINE__, __FUNCTION__, vpPlayer->GetCharNameShow( ), vpPlayer->GetCharID( ), 
			vpPlayer->GetEntityID( ), vpPB->repetionindex( ) );
		DecRepetionCount( vpPB->repetionindex( ) );
	}	
#endif

	return SUCCESS;
}       

int CPlayerRepetion::Repetion2PB( PBRepetion* vpPB, KEY_TYPE vKey )
{   
	unsigned int i = 0;
	unsigned int j = 0;
	int tData = 0;

	LK_ASSERT( vpPB != NULL, return ERR_INVALID_PARAMETER );


	for( i = _REPE_INDEX_START_; i < ARRAY_CNT( mRepeData ); i++ )
	{
		PBRepetionRepe* tpInfo = vpPB->add_repeinfo( );
		if( tpInfo == NULL )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Get Enough Mem, repetionindex %d, count %d, %d, %d, %d, %d,"
				" %d", __LK_FILE__, __LINE__, __FUNCTION__, i, mRepeData[ i ].mCount, mRepeData[ i ].mIntData[ 0 ],
				mRepeData[ i ].mIntData[ 1 ], mRepeData[ i ].mIntData[ 2 ], mRepeData[ i ].mIntData[ 3 ],
				mRepeData[ i ].mIntData[ 4 ] ,mRepeData[ i ].mIntData[ 5 ]);
			continue;
		}

		tpInfo->set_repeindex( i );
		tpInfo->set_count( mRepeData[ i ].mCount );
		tpInfo->set_weekcount( mRepeData[ i ].mWeekCount );
		tpInfo->set_signup( mRepeData[ i ].mSignUp );
		tpInfo->set_intotime(mRepeData[ i ].mIntoTime );
		tpInfo->set_diecount( mRepeData[ i ].mDieCount );

		for( j = 0; j < ARRAY_CNT( mRepeData[ i ].mIntData ); j++ )
		{
			if( mRepeData[ i ].mIntData[ j ] == 0 )
			{
				continue;
			}


			if( GetIntData( i, j, &tData ) == SUCCESS )
			{
				PBRepetionData* tpData = tpInfo->add_repetiondata( );
				if( tpData == NULL )
				{
					LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Get Enough Mem, %d, %d, %d, %d, %d", __LK_FILE__,
						__LINE__, __FUNCTION__, mRepeData[ i ].mIntData[ 0 ], mRepeData[ i ].mIntData[ 1 ],
						mRepeData[ i ].mIntData[ 2 ], mRepeData[ i ].mIntData[ 3 ], mRepeData[ i ].mIntData[ 4 ] ,mRepeData[ i ].mIntData[ 5 ]);
					continue;
				}

				tpData->set_index( j );
				tpData->set_data( tData );
			}
		}	
	}


	vpPB->set_backlineid( GetBackLineID( ) );
	vpPB->set_backmapid( GetBackMapID( ) );
	vpPB->set_backposx( GetPosX( ) );
	vpPB->set_backposy( GetPosY( ) );
	vpPB->set_key( ( vKey != INVITED_KEY_VALUE ) ? vKey : GetOldRepeKey( ) );
	vpPB->set_campid( GetCampID( ) );

	vpPB->set_repetionindex( GetInRepetionIndex( ) );
	vpPB->set_inrepetion( GetInRepetion( ) );
	vpPB->set_checkresult( mCheckResult );
	vpPB->set_answercount( mAnswerCount );
	vpPB->set_globalscenestatus( mGlobalSceneStatus );
	vpPB->set_globalwarofflinetime( mGlobalWarOfflineTime );
	vpPB->set_repetionkey( mRepetionKey );
	return SUCCESS;
}  


int CPlayerRepetion::IncRepetionCount( unsigned int vIndex ) 
{ 
	LK_ASSERT( vIndex < ARRAY_CNT( mRepeData ), return ERR_INVALID_PARAMETER );
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vIndex );
	if( tpInfo != NULL )
	{
		if( mRepeData[ vIndex ].mLastSaveTime != 0 && mRepeData[ vIndex ].mLastSaveTime < tpInfo->GetLastClearTime( ) )
		{
			mRepeData[ vIndex ].mCount = 0;
		}

		// 副本计次修改 2011.0913 by lipeng
		// 每次固定扣一次改成次数可配，默认为1
		int nConsumeTime = tpInfo->GetConsumeTimes();
		mRepeData[ vIndex ].mCount += nConsumeTime; 
		mRepeData[ vIndex ].mWeekCount += nConsumeTime;
		mRepeData[ vIndex ].mLastSaveTime = tpInfo->GetLastClearTime( );
		//
		ClearDieCount( vIndex );
		SetRepetionIntoTime( vIndex );
	}
	return SUCCESS; 
}

int CPlayerRepetion::DecRepetionCount( unsigned int vIndex )
{
	LK_ASSERT( vIndex < ARRAY_CNT( mRepeData ), return ERR_INVALID_PARAMETER );
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vIndex );
	if( tpInfo != NULL )
	{
		mRepeData[ vIndex ].mLastSaveTime = tpInfo->GetLastClearTime( );
	}

	if( mRepeData[ vIndex ].mWeekCount > 0 )
	{
		mRepeData[ vIndex ].mWeekCount--;
	}

	return ( mRepeData[ vIndex ].mCount > 0 ) ? --mRepeData[ vIndex ].mCount : mRepeData[ vIndex ].mCount;
}
int CPlayerRepetion::SetRepetion( unsigned int vIndex, unsigned int vCount, unsigned int vWeekCount ,unsigned int vSignUp ,
								 unsigned int vIntoTime,
								 unsigned int vDieCount)
{
	LK_ASSERT( vIndex < ARRAY_CNT( mRepeData ), return ERR_INVALID_PARAMETER );
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vIndex );
	if( tpInfo != NULL )
	{
		mRepeData[ vIndex ].mLastSaveTime = tpInfo->GetLastClearTime( );
	}

	mRepeData[ vIndex ].mCount = vCount;	
	mRepeData[ vIndex ].mWeekCount = vWeekCount;
	mRepeData[ vIndex ].mSignUp = vSignUp ;
	mRepeData[ vIndex ].mIntoTime = vIntoTime;
	mRepeData[ vIndex ].mDieCount = vDieCount;
	return SUCCESS;
}
int CPlayerRepetion::ChangeRepetionCount( unsigned int vIndex, int vCount )
{
	LK_ASSERT( vIndex < ARRAY_CNT( mRepeData ), return ERR_INVALID_PARAMETER );

	mRepeData[ vIndex ].mCount = mRepeData[ vIndex ].mCount + vCount;
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vIndex );
	if( tpInfo != NULL )
	{
		mRepeData[ vIndex ].mLastSaveTime = tpInfo->GetLastClearTime( );
	}

	if( mRepeData[ vIndex ].mCount < 0 )
	{
		mRepeData[ vIndex ].mCount = 0;
	}

	return mRepeData[ vIndex ].mCount;
}
int CPlayerRepetion::GetRepetionCount( unsigned int vIndex, unsigned int* vCount )
{
	LK_ASSERT( vIndex < ARRAY_CNT( mRepeData ), return ERR_INVALID_PARAMETER );
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vIndex );
	if( tpInfo != NULL && tpInfo->GetLastClearTime( ) > mRepeData[ vIndex ].mLastSaveTime )
	{
		*vCount = 0;
		mRepeData[ vIndex ].mCount = 0;
		mRepeData[ vIndex ].mLastSaveTime = tpInfo->GetLastClearTime( );
	}
	else
	{
		*vCount = mRepeData[ vIndex ].mCount;
	}
	return SUCCESS;
}

int CPlayerRepetion::GetRepetionWeekCount( unsigned int vIndex, unsigned int* vCount )
{
	LK_ASSERT( vIndex < ARRAY_CNT( mRepeData ), return ERR_INVALID_PARAMETER );
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( vIndex );
	if( tpInfo != NULL && tpInfo->GetLastClearTime( ) > mRepeData[ vIndex ].mLastSaveTime )
	{
		*vCount = 0;
		mRepeData[ vIndex ].mWeekCount = 0;
	}
	else
	{
		*vCount = mRepeData[ vIndex ].mWeekCount;
	}

	return SUCCESS;
}

int CPlayerRepetion::CleanWeekRepetion( )
{
	unsigned int i = 0;
	for( i = 0; i < ARRAY_CNT( mRepeData ); i++ )
	{
		mRepeData[ i ].mWeekCount = 0;			
	}

	return SUCCESS;
}

int CPlayerRepetion:: RepetionSignUp( unsigned int vIndex)
{
	mRepeData[ vIndex ].mSignUp = time( NULL );
	return SUCCESS;
}

int CPlayerRepetion:: RepetionClearSignUp( unsigned int vIndex)
{
	mRepeData[ vIndex ].mSignUp = 0;
	return SUCCESS;
}

int CPlayerRepetion:: GetSignUpTime( unsigned int vindex )
{
	return  mRepeData[ vindex ].mSignUp ;

}

int CPlayerRepetion:: SetRepetionIntoTime( unsigned int vIndex )
{
	mRepeData[ vIndex ].mIntoTime = time( NULL );
	return SUCCESS;
}
int CPlayerRepetion:: RepetionClearIntoTime( unsigned int vIndex )
{
	mRepeData[ vIndex ].mIntoTime = 0;
	return SUCCESS;
}
int CPlayerRepetion:: GetIntoTime( unsigned int vIndex )
{
	return mRepeData[ vIndex ].mIntoTime;
}

int CPlayerRepetion::AddDieCount( unsigned int vIndex )
{
	return ++mRepeData[ vIndex ].mDieCount;
}
int CPlayerRepetion::GetDieCount( unsigned int vIndex )
{
	return mRepeData[ vIndex ].mDieCount;
}
int CPlayerRepetion::ClearDieCount( unsigned int vIndex )
{
	return mRepeData[ vIndex ].mDieCount = 0;
}