#include "corps.h"
#include "corpsmanager.h" 
#include "corpservice.h"
#include "nameservice.h"
#include "gateobj_manager.h"
#include "gatelogic.h"	
#include "core_service.h"
#include "world.h"
#include "promessage_pb.hxx.pb.h"
#include "family_manager.h"
#include "gateteam.h"
#include "teamtype.h"
#include "servermessage_pb.hxx.pb.h"
#include "gateteammanager.h"
#include "bidmgr.h"
#include "log_tools.h"

// ***********************************************************
//  Function:		CWorldManager
//  Description:	世界的构造函数
//  Output:			
//  Date:	06/10/2009
// **********************************************************
CWorldManager::CWorldManager()
{
	if ( CObj::msCreateMode )
	{
		Initailize();
	}
	else
	{
		Resume();
	}
}

// ***********************************************************
//  Function:		operator new
//  Description:	重载new函数
//  Output:			
//  Date:	06/10/2009
// **********************************************************
void *CWorldManager::operator new( size_t tSize )
{
	return ( void* ) mShmPtr->CreateSegment( tSize );	  	 
}

// ***********************************************************
//  Function:		operator delete
//  Description:	重载delete函数
//  Output:			
//  Date:	06/10/2009
// **********************************************************
void CWorldManager::operator delete( void *pPointer ) 
{		
}

// ***********************************************************
//  Function:		operator delete
//  Description:	重载delete函数
//  Output:			
//  Date:	06/10/2009
// **********************************************************
unsigned int CWorldManager::CountSize()
{
	return sizeof( CWorldManager );
}

// ***********************************************************
//  Function:		Initailize
//  Description:	初始化函数
//  Output:			
//  Date:	06/10/2009
// **********************************************************
int CWorldManager::Initailize()
{
	mWarSerial = 1;
	mWinWorld = 0;
	mTitleTime = 0;
	mSetRate = -1;
	mTaxRate = DEFAULT_RATE;
	mTaxMoney = 0;
	mCorpsMoney = 0;
	mSetTimes = 0;
	return 0;	
}

// ***********************************************************
//  Function:		Resume
//  Description:	
//  Output:			
//  Date:	06/10/2009
// **********************************************************
int CWorldManager::Resume()
{	
	return 0;
}

int CWorldManager::UpdateWorldDBInfo( int vWorldID, int vCountryID )
{
	if ( vWorldID <= WORLD_START || vWorldID >= WORLD_NUM || vCountryID < 0 || vCountryID > COUNTRY_NUM )
	{
		return -1;
	}
	CCountry *tpCountry = &( mWorld[ vWorldID ].mCountry[ vCountryID ] );

	/*char tPBBuff[20000] = { 0 };
	char tEscasecapeBuff[20000] = { 0 };
	PBOfficialList tList;

	tpCountry->SaveOfficial( tList );
	int size = tList.ByteSize();
	bool tRet = tList.SerializeToArray( tPBBuff, 20000 );
	LK_ASSERT( tRet == true, return -1 );
	char *tpEsBuffer = tEscasecapeBuff;
	int tCapRet = EscapeDataString( &tpEsBuffer, 20000, tPBBuff, tList.ByteSize() );
	LK_ASSERT( tCapRet != -1, return -1 )*/

	const char *tpSql = "update UMS_WORLD set king_power=%d, people_num=%d, wizard0=%d, wizard1=%d,  \
						wizard2=%d, wizard3=%d, wizard4=%d, wizard5=%d, convalue0=%d, convalue1=%d, convalue2=%d, \
						convalue3=%d, convalue4=%d, convalue5=%d, conlevel0=%d, conlevel1=%d, conlevel2=%d, conlevel3=%d, \
						conlevel4=%d, conlevel5=%d, conwizard0=%d, conwizard1=%d, conwizard2=%d, conwizard3=%d, conwizard4=%d, \
						conwizard5=%d, version=1 where world_id=%d and country_id=%d";
	CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_update_country, 0, 0, 5, SQL_NONE, UPDATE, 
		tpSql, tpCountry->GetKingPower(), tpCountry->GetPeopleNum(), tpCountry->GetWizard( 0 ), tpCountry->GetWizard( 1 ), 
		tpCountry->GetWizard( 2 ), tpCountry->GetWizard( 3 ), tpCountry->GetWizard( 4 ), tpCountry->GetWizard( 5 ),
		tpCountry->GetConstructValue( 0 ), tpCountry->GetConstructValue( 1 ), tpCountry->GetConstructValue( 2 ),
		tpCountry->GetConstructValue( 3 ), tpCountry->GetConstructValue( 4 ), tpCountry->GetConstructValue( 5 ),
		tpCountry->GetConstructLevel( 0 ), tpCountry->GetConstructLevel( 1 ), tpCountry->GetConstructLevel( 2 ),
		tpCountry->GetConstructLevel( 3 ), tpCountry->GetConstructLevel( 4 ), tpCountry->GetConstructLevel( 5 ),
		tpCountry->GetConstructWizard( 0 ), tpCountry->GetConstructWizard( 1 ), tpCountry->GetConstructWizard( 2 ),
		tpCountry->GetConstructWizard( 3 ), tpCountry->GetConstructWizard( 4 ), tpCountry->GetConstructWizard( 5 ),
		vWorldID, vCountryID );

	return SUCCESS;
}

int CWorldManager::UpdateWorldStatusDBInfo( int vWorldID )
{
	if ( vWorldID <= WORLD_START || vWorldID >= WORLD_NUM )
	{
		return -1;
	}
	CWorld *tpWorld = &( mWorld[ vWorldID ] );

	const char *tpSql = "update UMS_WORLDSTATUS set friend0=%d, friend1=%d, friend2=%d, friend3=%d, \
						status0=%d, status1=%d, status2=%d, status3=%d, number=%d, \
						honer_inc=%d, level_score=%d, honer_score=%d, weak_level=%d, \
						accuse_state=%d, accuse_yes=%d, accuse_no=%d, accuse_abort=%d, gate_times=%d, \
						flag_times=%d, gt_target_world=%d, gt_target_time=%d, lw_target_world=%d, lw_target_time=%d where world_id=%d";

	CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_update_country, 0, 0, 5, SQL_NONE, UPDATE, 
		tpSql, tpWorld->mFriendStatus[ 1 ], tpWorld->mFriendStatus[ 2 ], tpWorld->mFriendStatus[ 3 ], tpWorld->mFriendStatus[ 4 ], 
		tpWorld->mNationStatus[ 1 ], tpWorld->mNationStatus[ 2 ], tpWorld->mNationStatus[ 3 ], tpWorld->mNationStatus[ 4 ], tpWorld->mOpNumber,
		tpWorld->m_nHonerInc, 
		tpWorld->m_nCampLevelScore, 
		tpWorld->m_nCampHonerScore, 
		tpWorld->m_nWeekLevel, tpWorld->mAccuseState, tpWorld->mAccuseYes, tpWorld->mAccuseNo, tpWorld->mAccuseAbort,
		tpWorld->mGateTimes, tpWorld->mFlagTimes, 
		tpWorld->mGTWorld, tpWorld->mGTEndTime, tpWorld->mLWWorld, tpWorld->mLWEndTime,
		vWorldID);

	return SUCCESS;
}

int CWorldManager::UpdateOfficialDBInfo( int vWorldID, int vCountryID )
{
	if ( vWorldID <= WORLD_START || vWorldID >= WORLD_NUM || vCountryID < 0 || vCountryID > COUNTRY_NUM )
	{
		return -1;
	}
	CCountry *tpCountry = &( mWorld[ vWorldID ].mCountry[ vCountryID ] );

	for ( int i = 0; i < OFFICIAL_FUNCTION_NUM; i++ )
	{
		char tPBBuff[40000] = { 0 };
		char tEscasecapeBuff[40000] = { 0 };
		PBOfficialList tList;

		tpCountry->SaveOfficial( tList, i );
		//int size = tList.ByteSize();
		bool tRet = tList.SerializeToArray( tPBBuff, 40000 );
		LK_ASSERT( tRet == true, return -1 );
		char *tpEsBuffer = tEscasecapeBuff;
		int tCapRet = EscapeDataString( &tpEsBuffer, 40000, tPBBuff, tList.ByteSize() );
		LK_ASSERT( tCapRet != -1, return -1 )

			const char *tpSql = "update UMS_WORLD set official%d='%s' where world_id=%d and country_id=%d";
		CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_update_official, 0, 0, 5, SQL_NONE, UPDATE, 
			tpSql, i, tpEsBuffer,vWorldID, vCountryID );
	}

	return SUCCESS;
}

void CWorldManager::LoadCorpsFromMap( int vWorldID, int vMapID, int vCorpsID )
{
	LOG_NOTICE( "corps", "ServerID:%d,MapID:%d,CorpsID:%d", vWorldID, vMapID, vCorpsID);

	int tWorldID = vWorldID;
	if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM )
		return;	
	CWorld *tpWorld = &(mWorld[ tWorldID ]);
	for ( size_t i = 0; i < ARRAY_CNT(tpWorld->mCountry); i++ )
	{
		if ( tpWorld->mCountry[ i ].GetMapID( ) == vMapID )
		{
			int tOldID = tpWorld->mCountry[ i ].GetCorpsID( );
			tpWorld->mCountry[ i ].SetCorpsID( vCorpsID );
			if ( tOldID != vCorpsID )
			{
				if ( i == COUNTRY_CHANGAN )
				{
					tpWorld->mCountry[ i ].SetKingPower( KING_POWER_INIVALUE1 );
				}
				else
				{
					tpWorld->mCountry[ i ].SetKingPower( KING_POWER_INIVALUE2 );
				}
			}
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( vCorpsID );
			if ( tpCorps != NULL )
			{				
				tpWorld->mCountry[ i ].mOfficialData[ OFFICIAL_LEVEL_KING ][ 0 ].mOfficialID = tpCorps->GetRooterID();
				strncpy( tpWorld->mCountry[ i ].mOfficialData[ OFFICIAL_LEVEL_KING ][ 0 ].mOfficialName, tpCorps->GetRooterName(), NAME_LENGTH );
			}		
			if ( vCorpsID == 0 )
			{
				tpWorld->mCountry[ i ].mOfficialData[ OFFICIAL_LEVEL_KING ][ 0 ].mOfficialID = 0;
				memset( tpWorld->mCountry[ i ].mOfficialData[ OFFICIAL_LEVEL_KING ][ 0 ].mOfficialName, 0, sizeof( tpWorld->mCountry[ i ].mOfficialData[ OFFICIAL_LEVEL_KING ][ 0 ].mOfficialName ) );
			}
			if ( tOldID != vCorpsID )
			{
				CWorldModule::GetSingleton().SendKingChangeNotify( tWorldID, i, 0 );
			}	
			// 获胜方换人，重置税率
			if ( tOldID != 0 && tWorldID == mWinWorld && i == COUNTRY_CHANGAN && tOldID != vCorpsID )
			{
				mSetRate = DEFAULT_RATE;
			}
		}
	}	
	CCorpservice::GetSingleton().ChangeCorpsCountry( vMapID, vCorpsID );
}

void CWorldManager::SyncNation( int vServerID )
{
	CMessage MsgHead;	
	CMessageSyncNationNotify Notify;

	for ( int i = 0; i < WORLD_NUM; i++ )
	{
		for ( int j = 0; j < WORLD_NUM; j++ )
		{
			Notify.add_nationstatus( mWorld[ i ].mNationStatus[ j ] );
		}
	}

	MsgHead.set_msgpara( (int)&Notify );
	MsgHead.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_SYNCNATION );
	if ( vServerID != 0 )
	{
		CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
	}
	else
	{
		CGateServer::GetSingleton().Send2Scene( (CSceneServer*)NULL, &MsgHead );
	}
}

void CWorldManager::DailyRefresh( )
{
	CTemplateConstructExp *tpExp = (CTemplateConstructExp*)CDataStatic::GetTemp( TEMP_CONSTRUCTEXP );
	if ( tpExp == NULL )
		return;
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		mWorld[ i ].mOpNumber = 0;
		mWorld[ i ].mGateTimes = 0;
		mWorld[ i ].mFlagTimes = 0;
		for ( int j = 0; j < COUNTRY_NUM; j++ )
		{
			CCountry *tpCountry = &(mWorld[ i ].mCountry[ j ]);
			for ( int n = 0; n < OFFICIAL_LEVEL_NUM; n++ )
			{
				for ( int m = 0; m < MAX_OFFICAL_NUM; m++ )
				{
					memset( tpCountry->mOfficialData[ n ][ m ].mUsetimes, 0, sizeof( tpCountry->mOfficialData[ n ][ m ].mUsetimes ) );
				}
			}
			if ( tpExp != NULL )
			{
				int tLevel = tpCountry->GetConstructLevel( OFFICAL_LIBIN ) - 1;
				tpCountry->SetKingPower( tpCountry->GetKingPower() + tpExp->mBonus[ tLevel > 0 ? tLevel : 0 ] );
			}			 
		}
	}
	// 刷新每日税金
	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;
	if ( mTaxRate >= (int)ARRAY_CNT(tpConfig->mTax) )
		return;
	int tIncomeMoney = mTaxMoney * ( tpConfig->mTax[ mTaxRate ].mIncomeRate / SERVER_PERCENT_FLOAT );
	if ( tIncomeMoney > tpConfig->mTax[ mTaxRate ].mLimit * SERVER_PERCENT_INT )
		mCorpsMoney = tpConfig->mTax[ mTaxRate ].mLimit;
	else
		mCorpsMoney = tIncomeMoney / SERVER_PERCENT_INT;
	mTaxMoney = 0;
	if ( mSetRate != -1 )
	{
		mTaxRate = mSetRate;
		CWorldModule::GetSingleton().SendModifyRateNotify( mTaxRate, NULL, 1, -1 );
		// 修改完置回-1
		mSetRate = -1;
	}
	// 清除每日次数
	mSetTimes = 0;
}

int CWorldManager::GetCountryIDByMap( int vMapID )
{
	int tCountryID = COUNTRY_NONE;
	switch( vMapID )
	{
	case COUNTRY_MAP_CHANGAN:
		{
			tCountryID = COUNTRY_CHANGAN;
			break;
		}
	case COUNTRY_MAP_WUJI:
		{
			tCountryID = COUNTRY_WUJI;
			break;
		}
	case COUNTRY_MAP_CHECHI:
		{
			tCountryID = COUNTRY_CHECHI;
			break;
		}
	case COUNTRY_MAP_BAOXIANG:
		{
			tCountryID = COUNTRY_BAOXIANG;
			break;
		}
	case COUNTRY_MAP_NVER:
		{
			tCountryID = COUNTRY_NVER;
			break;
		}
	default:
		break;
	}
	return tCountryID;
}

void CWorldManager::ClearOfficialID( CGatePlayer *pPlayer )
{
	if ( pPlayer == NULL )
	{
		return;
	}
	if ( pPlayer->OfficialID( ) != 0 )
	{
		if ( pPlayer->CountryID( ) > WORLD_START && pPlayer->CountryID( ) < WORLD_NUM
			&& pPlayer->TempID( ) > COUNTRY_NONE && pPlayer->TempID( ) < COUNTRY_NUM )
		{
			CCountry *tpCountry = &( mWorld[ pPlayer->CountryID( ) ].mCountry[ pPlayer->TempID( ) ] );
			int tLevel = -1, tOrder = -1;
			tpCountry->ClearOfficialID( pPlayer->CharID( ), tLevel, tOrder );
			if ( tLevel != -1 && tOrder != -1 )
			{
				CWorldModule::GetSingleton().SendOfficialChangeNotify( pPlayer->CharID( ), tLevel, tOrder, pPlayer->CountryID( ), pPlayer->TempID( ), CHANGE_DEL );
			}
		}
		pPlayer->OfficialID( 0 );
	}
}

void CWorldManager::ChangePeopleNum( int vWorldID, int vCountryID, int vChangeType )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM
		&& vCountryID > COUNTRY_NONE && vCountryID < COUNTRY_NUM )
	{
		CCountry *tpCountry = &( mWorld[ vWorldID ].mCountry[ vCountryID ] );
		if ( vChangeType == CHANGE_ADD )
		{
			tpCountry->AddPeopleNum( );
		}
		if ( vChangeType == CHANGE_DEL )
		{
			tpCountry->DelPeopleNum( );
		}
	}
}

CCountry* CWorldManager::GetCountryByPlayer( CGatePlayer *pPlayer )
{
	if ( pPlayer == NULL )
	{
		return NULL;
	}
	int tWorldID = pPlayer->CountryID( );
	int tCountryID = pPlayer->TempID( );
	if ( tWorldID > WORLD_START && tWorldID < WORLD_NUM
		&& tCountryID > COUNTRY_NONE && tCountryID < COUNTRY_NUM )
	{
		return &( mWorld[ tWorldID ].mCountry[ tCountryID ] );
	}
	return NULL;
}

void CWorldManager::AddPlayerNum( int vWorldID, int vLineID, int vMapID )
{
	int tLineID = GetLineID( vLineID );
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM
		&& tLineID >= 0 && tLineID < MAX_LINEID_NUM && vMapID >= 0 && vMapID < MAX_MAPID_NUM )
	{
		mWorld[ vWorldID ].mPlayerNum[ tLineID ][ vMapID ]++;
	}
}

void CWorldManager::DelPlayerNum( int vWorldID, int vLineID, int vMapID )
{
	int tLineID = GetLineID( vLineID );
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM
		&& tLineID >= 0 && tLineID < MAX_LINEID_NUM && vMapID >= 0 && vMapID < MAX_MAPID_NUM )
	{
		mWorld[ vWorldID ].mPlayerNum[ tLineID ][ vMapID ]--;
		if ( mWorld[ vWorldID ].mPlayerNum[ tLineID ][ vMapID ] < 0 )
			mWorld[ vWorldID ].mPlayerNum[ tLineID ][ vMapID ] = 0;
	}
}

int CWorldManager::GetPlayerTotal( int vLineID, int vMapID )
{
	int tValue = 0;
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		tValue += GetPlayerNum( i, vLineID, vMapID );
	}
	return tValue;
}

int CWorldManager::GetPlayerNum( int vWorldID, int vLineID, int vMapID )
{
	int tLineID = GetLineID( vLineID );
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM
		&& tLineID >= 0 && tLineID < MAX_LINEID_NUM && vMapID >= 0 && vMapID < MAX_MAPID_NUM )
	{
		return mWorld[ vWorldID ].mPlayerNum[ tLineID ][ vMapID ];
	}
	else
	{
		return 0;
	}
}

int CWorldManager::GetLineID( int vLineID )
{
	if ( vLineID == 50 )
		return 5;
	if ( vLineID == 51 )
		return 6;
	if ( vLineID == 98 )
		return 7;
	if ( vLineID == 99 )
		return 8;
	if ( vLineID == 97 )
		return 9;
	return vLineID;
}

void CWorldManager::OnSubmit( int vWorldID, int vLevel, int vMoney )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM )
	{
		mWorld[ vWorldID ].mTotalLevel += vLevel;
		mWorld[ vWorldID ].mTotalNum ++; 
		mWorld[ vWorldID ].mSubmitMoney += vMoney;
	}
}

void CWorldManager::EndClearWar( )
{
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		mWorld[ i ].mTotalLevel = 0;
		mWorld[ i ].mTotalNum = 0;
		mWorld[ i ].mCampID = -1;
		memset( mWorld[ i ].mSource, 0, sizeof(mWorld[ i ].mSource) );
		memset( mWorld[ i ].mSeed, 0, sizeof(mWorld[ i ].mSeed) );
		mWorld[ i ].mBidMoney = 0;
		mWorld[ i ].mBidCampID = -1;
		memset( mWorld[ i ].mSeedNum, 0, sizeof(mWorld[ i ].mSeedNum) );
		memset( mWorld[ i ].mKill, 0, sizeof(mWorld[ i ].mKill) );
		mWorld[ i ].mSubmitMoney = 0;
	}
}

void CWorldManager::StartClearWar( )
{
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		memset( mWorld[ i ].mWinChar, 0, sizeof(mWorld[ i ].mWinChar) );
		mWorld[ i ].mBasicAward = 0;
		mWorld[ i ].mBestAward = 0;
		memset( mWorld[ i ].mSource, 0, sizeof(mWorld[ i ].mSource) );
		memset( mWorld[ i ].mSeed, 0, sizeof(mWorld[ i ].mSeed) );
		memset( mWorld[ i ].mSeedNum, 0, sizeof(mWorld[ i ].mSeedNum) );
		memset( mWorld[ i ].mKill, 0, sizeof(mWorld[ i ].mKill) );
	}
}

void CWorldManager::SetCampID( int vWorldID, int vCampID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM )
	{
		mWorld[ vWorldID ].mCampID = vCampID;
	}
}

int CWorldManager::GetCampID( int vWorldID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM )
	{
		return mWorld[ vWorldID ].mCampID;
	}
	else
	{
		return -1;
	}
}

void CWorldManager::AddSource( int vWorldID, int vCampID, int vValue )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM && vCampID >= 0 && vCampID < MAX_WAR_CAMP )
	{
		mWorld[ vWorldID ].mSource[ vCampID ] += vValue;
	}
}

void CWorldManager::DelSource( int vWorldID, int vCampID, int vValue )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM && vCampID >= 0 && vCampID < MAX_WAR_CAMP )
	{
		mWorld[ vWorldID ].mSource[ vCampID ] -= vValue;
		if ( mWorld[ vWorldID ].mSource[ vCampID ] < 0 )
			mWorld[ vWorldID ].mSource[ vCampID ] = 0;
	}
}

void CWorldManager::AddSeed( int vWorldID, int vCampID, int vValue )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM && vCampID >= 0 && vCampID < MAX_WAR_CAMP )
	{
		mWorld[ vWorldID ].mSeed[ vCampID ] += vValue;
		mWorld[ vWorldID ].mSeedNum[ vCampID ]++;
	}
}

void CWorldManager::DelSeed( int vWorldID, int vCampID, int vValue )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM && vCampID >= 0 && vCampID < MAX_WAR_CAMP )
	{
		mWorld[ vWorldID ].mSeed[ vCampID ] -= vValue;
		if ( mWorld[ vWorldID ].mSeed[ vCampID ] < 0 )
			mWorld[ vWorldID ].mSeed[ vCampID ] = 0;
		mWorld[ vWorldID ].mSeedNum[ vCampID ]--;
		if ( mWorld[ vWorldID ].mSeedNum[ vCampID ] < 0 )
			mWorld[ vWorldID ].mSeedNum[ vCampID ] = 0;
	}
}

void CWorldManager::ClearSource( )
{
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		memset( mWorld[ i ].mSource, 0, sizeof(mWorld[ i ].mSource) );
		memset( mWorld[ i ].mSeed, 0, sizeof(mWorld[ i ].mSeed) );
		memset( mWorld[ i ].mSeedNum, 0, sizeof(mWorld[ i ].mSeedNum) );
		memset( mWorld[ i ].mKill, 0, sizeof(mWorld[ i ].mKill) );
	}
}

int CWorldManager::GetSource( int vWorldID, int vCampID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM && vCampID >= 0 && vCampID < MAX_WAR_CAMP )
	{
		return mWorld[ vWorldID ].mSource[ vCampID ];
	}
	else
	{
		return 0;
	}
}

int CWorldManager::GetSeed( int vWorldID, int vCampID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM && vCampID >= 0 && vCampID < MAX_WAR_CAMP )
	{
		return mWorld[ vWorldID ].mSeed[ vCampID ];
	}
	else
	{
		return 0;
	}
}

int CWorldManager::GetSeedNum( int vWorldID, int vCampID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM && vCampID >= 0 && vCampID < MAX_WAR_CAMP )
	{
		return mWorld[ vWorldID ].mSeedNum[ vCampID ];
	}
	else
	{
		return 0;
	}
}

int CWorldManager::GetKill( int vWorldID, int vCampID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM && vCampID >= 0 && vCampID < MAX_WAR_CAMP )
	{
		return mWorld[ vWorldID ].mKill[ vCampID ];
	}
	else
	{
		return 0;
	}
}

void CWorldManager::AddKill( int vWorldID, int vCampID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM && vCampID >= 0 && vCampID < MAX_WAR_CAMP )
	{
		mWorld[ vWorldID ].mKill[ vCampID ]++;
	}
}

void CWorldManager::SetBidMoney( int vWorldID, int vMoney )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM )
	{
		mWorld[ vWorldID ].mBidMoney = vMoney;
	}
}

int CWorldManager::GetBidMoney( int vWorldID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM )
	{
		return mWorld[ vWorldID ].mBidMoney;
	}
	else
	{
		return 0;
	}
}

void CWorldManager::SetBidCampID( int vWorldID, int vCampID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM )
	{
		mWorld[ vWorldID ].mBidCampID = vCampID;
	}
}

int CWorldManager::GetBidCampID( int vWorldID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM )
	{
		return mWorld[ vWorldID ].mBidCampID;
	}
	else
	{
		return 0;
	}
}

void CWorldManager::SetSubmitMoney( int vWorldID, int vMoney )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM )
	{
		mWorld[ vWorldID ].mSubmitMoney = vMoney;
	}
}

int CWorldManager::GetSubmitMoney( int vWorldID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM )
	{
		return mWorld[ vWorldID ].mSubmitMoney;
	}
	else
	{
		return 0;
	}
}

void CWorldManager::CheckBidWorld( )
{
	int tMaxWorld[ MAX_WAR_CANUM ] = { 0 };
	int tMaxMoney[ MAX_WAR_CANUM ] = { 0 };
	int tFreeWorld[ WORLD_NUM ] = { 0 };
	int tFreeNum = 0;
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		int tCampID = mWorld[ i ].mBidCampID;
		int tMoney = mWorld[ i ].mBidMoney;
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( mWorld[ i ].mCountry[ COUNTRY_CHANGAN ].GetCorpsID() );
		if ( tpCorps == NULL )
		{
			tFreeWorld[ tFreeNum++ ] = i;
			continue;
		}
		// 没钱就清空
		if ( tpCorps->GetCorpsMoney() < tMoney )
		{
			//mWorld[ i ].mBidCampID = -1;
			//mWorld[ i ].mBidMoney = 0;
			tFreeWorld[ tFreeNum++ ] = i;
			continue;
		}
		if ( tCampID >= 0 && tCampID < MAX_WAR_CANUM )
		{
			if ( tMoney > tMaxMoney[ tCampID ] )
			{
				if ( tMaxWorld[ tCampID ] != 0 )
					tFreeWorld[ tFreeNum++ ] = tMaxWorld[ tCampID ];
				tMaxWorld[ tCampID ] = i;
				tMaxMoney[ tCampID ] = tMoney;
				continue;
			}
		}
		tFreeWorld[ tFreeNum++ ] = i;
	}
	for ( int i = 0; i < MAX_WAR_CANUM; i++ )
	{
		if ( tMaxWorld[ i ] != 0 && tMaxWorld[ i ] < WORLD_NUM )
		{
			// 竞价成功，要扣钱了
			mWorld[ tMaxWorld[ i ] ].mCampID = i;
			LOG_NOTICE( "world", "bidcamp finished, world %d, camp %d, money %d", tMaxWorld[ i ], i, tMaxMoney[ i ] );
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( mWorld[ tMaxWorld[ i ] ].mCountry[ COUNTRY_CHANGAN ].GetCorpsID() );
			if ( tpCorps == NULL )
				continue;
			CCorpservice::GetSingleton().ConsumeCorpsMoney( tpCorps->GetRooterID(), tMaxMoney[ i ], REASON_WARBID );
		}
		else
		{
			if ( tFreeNum > 0 )
			{
				int tRandIndex = RAND(tFreeNum);
				int tWorldID = tFreeWorld[ tRandIndex ];
				if ( tWorldID != 0 && tWorldID < WORLD_NUM )
				{
					mWorld[ tWorldID ].mCampID = i; 
					tFreeWorld[ tRandIndex ] = tFreeWorld[ tFreeNum - 1 ];
					tFreeWorld[ tFreeNum - 1 ] = 0;
					tFreeNum--;
					LOG_NOTICE( "world", "bidcamp finished, world %d, camp %d", tWorldID, i );
				}
			}			
		}
	}
}

int CWorldManager::GetWinWorld( )
{
	int tWorld[ WORLD_NUM ] = { 0 };
	int tMaxWorld = 0;
	int tMaxSource = 0;
	for ( int i = 0; i < MAX_WAR_CAMP; i++ )
	{
		tMaxWorld = 0;
		tMaxSource = 0;
		for ( int j = WORLD_FIRST; j < WORLD_NUM; j++ )
		{
			if ( mWorld[ j ].mSource[ i ] > tMaxSource )
			{
				tMaxWorld = j;
				tMaxSource = mWorld[ j ].mSource[ i ];
			}			
		}
		if ( tMaxWorld != 0 )
		{
			if ( i == 0 )
			{
				tWorld[ tMaxWorld ] += MAIN_WIN_POINT;
				CWorldModule::GetSingleton().SendWarNotify( NOTICE_WAR_RESULT_CAMP, i, tMaxWorld, MAIN_WIN_POINT, NULL );
			}
			else
			{
				tWorld[ tMaxWorld ] += OTHER_WIN_POINT;
				CWorldModule::GetSingleton().SendWarNotify( NOTICE_WAR_RESULT_CAMP, i, tMaxWorld, OTHER_WIN_POINT, NULL );
			}
		}		
	}

	tMaxWorld = 0;
	tMaxSource = 0;
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		if ( tWorld[ i ] > tMaxSource )
		{
			tMaxWorld = i;
			tMaxSource = tWorld[ i ];
		}
	}

	// 换人了重置税率
	if ( tMaxWorld != 0 && tMaxWorld != mWinWorld )
	{
		mSetRate = DEFAULT_RATE;
	}

	mWinWorld = tMaxWorld;
	LOG_NOTICE( "world", "war finished, win world %d, point %d", tMaxWorld, tMaxSource );
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( mWorld[ tMaxWorld ].mCountry[ COUNTRY_CHANGAN ].GetCorpsID() );
	CWorldModule::GetSingleton().SendWarNotify( NOTICE_WAR_RESULT_ALL, tMaxWorld, 0, 0, tpCorps == NULL ? NULL : (char*)tpCorps->GetRooterName() );
	CWorldModule::GetSingleton().SendModifyRateNotify( mTaxRate, NULL, 1, tMaxWorld );
	return tMaxWorld;
}

void CWorldManager::AddWinner( int vWorldID, int vCharID, int vHonor, int vKill, int vMetierID )
{
	if ( vWorldID < WORLD_FIRST || vWorldID >= WORLD_NUM )
		return;
	if ( vMetierID <= METIER_NONE || vMetierID >= METIER_NOVICE )
		return;

	LOG_DEBUG( "world", "add winner, world %d, charid %d, honor %d, kill %d, metier %d", vWorldID, vCharID, vHonor, vKill, vMetierID );
	if ( vHonor <  mWorld[ vWorldID ].mWinChar[ vMetierID ][ MAX_WAR_WINNER - 1 ].mHonor )
		return;
	else if ( vHonor ==  mWorld[ vWorldID ].mWinChar[ vMetierID ][ MAX_WAR_WINNER - 1 ].mHonor )
	{
		if ( vKill < mWorld[ vWorldID ].mWinChar[ vMetierID ][ MAX_WAR_WINNER - 1 ].mKill )
			return;
	}

	int tHasChar = -1;
	for ( int i = 0; i < MAX_WAR_WINNER; i++ )
	{
		if ( mWorld[ vWorldID ].mWinChar[ vMetierID ][ i ].mCharID == vCharID )
		{
			// 战功都是递增的，如果有减少的，丢弃不处理
			if ( vHonor < mWorld[ vWorldID ].mWinChar[ vMetierID ][ i ].mHonor )
				return;
			mWorld[ vWorldID ].mWinChar[ vMetierID ][ i ].mHonor = vHonor;
			mWorld[ vWorldID ].mWinChar[ vMetierID ][ i ].mKill = vKill;
			tHasChar = i;
			break;
		}
	}
	// 找到了要重新排序
	if ( tHasChar >= 0 )
	{
		ReorderWinner( vWorldID, vMetierID, tHasChar );
	}
	else
	{
		mWorld[ vWorldID ].mWinChar[ vMetierID ][ MAX_WAR_WINNER - 1 ].mCharID = vCharID;
		mWorld[ vWorldID ].mWinChar[ vMetierID ][ MAX_WAR_WINNER - 1 ].mHonor = vHonor;
		mWorld[ vWorldID ].mWinChar[ vMetierID ][ MAX_WAR_WINNER - 1 ].mKill = vKill;
		ReorderWinner( vWorldID, vMetierID, MAX_WAR_WINNER - 1 );
	}

	LOG_DEBUG( "world", "winner1 %d, 2 %d, 3 %d", mWorld[ vWorldID ].mWinChar[ vMetierID ][ 0 ].mCharID, 
		mWorld[ vWorldID ].mWinChar[ vMetierID ][ 1 ].mCharID,
		mWorld[ vWorldID ].mWinChar[ vMetierID ][ 2 ].mCharID );
}

void CWorldManager::ReorderWinner( int vWorldID, int vMetierID, int vFirst )
{
	for ( int i = vFirst; i >= 1 ; i-- )
	{
		if ( mWorld[ vWorldID ].mWinChar[ vMetierID ][ i - 1 ].mHonor < mWorld[ vWorldID ].mWinChar[ vMetierID ][ i ].mHonor )
		{
			SwapWinner( vWorldID, vMetierID, i - 1, i );
		}
		else if ( mWorld[ vWorldID ].mWinChar[ vMetierID ][ i - 1 ].mHonor == mWorld[ vWorldID ].mWinChar[ vMetierID ][ i ].mHonor )
		{
			if ( mWorld[ vWorldID ].mWinChar[ vMetierID ][ i - 1 ].mKill < mWorld[ vWorldID ].mWinChar[ vMetierID ][ i ].mKill )
			{
				SwapWinner( vWorldID, vMetierID, i - 1, i );
			}
		}
	}
}

void CWorldManager::SwapWinner( int vWorldID, int vMetierID, int vSrcIndex, int vDesIndex )
{
	int tTempCharID = mWorld[ vWorldID ].mWinChar[ vMetierID ][ vSrcIndex ].mCharID;
	int tTempHonor = mWorld[ vWorldID ].mWinChar[ vMetierID ][ vSrcIndex ].mHonor;
	int tTempKill = mWorld[ vWorldID ].mWinChar[ vMetierID ][ vSrcIndex ].mKill;
	mWorld[ vWorldID ].mWinChar[ vMetierID ][ vSrcIndex ].mCharID = mWorld[ vWorldID ].mWinChar[ vMetierID ][ vDesIndex ].mCharID;
	mWorld[ vWorldID ].mWinChar[ vMetierID ][ vSrcIndex ].mHonor = mWorld[ vWorldID ].mWinChar[ vMetierID ][ vDesIndex ].mHonor;
	mWorld[ vWorldID ].mWinChar[ vMetierID ][ vSrcIndex ].mKill = mWorld[ vWorldID ].mWinChar[ vMetierID ][ vDesIndex ].mKill;
	mWorld[ vWorldID ].mWinChar[ vMetierID ][ vDesIndex ].mCharID = tTempCharID;
	mWorld[ vWorldID ].mWinChar[ vMetierID ][ vDesIndex ].mHonor = tTempHonor;
	mWorld[ vWorldID ].mWinChar[ vMetierID ][ vDesIndex ].mKill = tTempKill;
}

int	CWorldManager::CheckAward( )
{
	int tTotalMoney = 0;
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		// 如果是获胜方
		if ( i == mWinWorld )
		{
			continue;
		}
		tTotalMoney += mWorld[ i ].mSubmitMoney;
	}
	int tParam = mWorld[ mWinWorld ].mTotalLevel - mWorld[ mWinWorld ].mTotalNum * AWARD_PARAM;
	if ( tParam == 0 )
		mWorld[ mWinWorld ].mBasicAward = 0;
	else
		mWorld[ mWinWorld ].mBasicAward = tTotalMoney / SERVER_PERCENT_FLOAT * WIN_PERCENT / tParam;
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		mWorld[ i ].mBestAward = tTotalMoney / BEST_PARAM; 
	}	
	LOG_DEBUG( "world", "award checked, totalmoney %d, totallevel %d, totalnum %d", tTotalMoney, mWorld[ mWinWorld ].mTotalLevel, mWorld[ mWinWorld ].mTotalNum );
	LOG_NOTICE( "world", "award checked, basic award %d, best award %d", mWorld[ mWinWorld ].mBasicAward,  mWorld[ mWinWorld ].mBestAward );
	return 0;
}

int CWorldManager::GetBasicAward( int vWorldID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM )
	{
		return mWorld[ vWorldID ].mBasicAward;
	}
	else
	{
		return 0;
	}
}

int CWorldManager::GetBestAward( int vWorldID )
{
	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM )
	{
		return mWorld[ vWorldID ].mBestAward;
	}
	else
	{
		return 0;
	}
}

int CWorldManager::GetRank( int vWorldID, int vCharID, int vMetierID )
{
	if ( vWorldID < WORLD_FIRST || vWorldID >= WORLD_NUM )
		return -1;
	if ( vMetierID <= METIER_NONE || vMetierID >= METIER_NOVICE )
		return -1;

	for ( int i = 0; i < MAX_WAR_WINNER; i++ )
	{
		if ( mWorld[ vWorldID ].mWinChar[ vMetierID][ i ].mCharID == vCharID )
		{
			return i;
		}
	}
	return -1;
}

int CWorldManager::UpdateWorldWarDBInfo( int vWorldID )
{
	if ( vWorldID <= WORLD_START || vWorldID >= WORLD_NUM )
	{
		return -1;
	}
	CWorld *tpWorld = &( mWorld[ vWorldID ] );

	const char *tpSql = "update UMS_WORLDWAR set totallevel=%d, totalnum=%d, campid=%d, bidmoney=%d,  \
						bidcampid=%d, basicaward=%d, bestaward=%d, source0=%d, \
						source1=%d, source2=%d, submitmoney=%d where world_id=%d";
	CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_update_worldwar, 0, 0, 5, SQL_NONE, UPDATE, 
		tpSql, tpWorld->mTotalLevel, tpWorld->mTotalNum, tpWorld->mCampID, tpWorld->mBidMoney, 
		tpWorld->mBidCampID, tpWorld->mBasicAward, tpWorld->mBestAward, tpWorld->mSource[ 0 ], 
		tpWorld->mSource[ 1 ], tpWorld->mSource[ 2 ], tpWorld->mSubmitMoney, vWorldID );

	return SUCCESS;
}

int CWorldManager::UpdateWorldGlobalDBInfo( )
{
	const char *tpSql = "update UMS_WORLDGLOBAL set war_serial=%d, war_status=%d, war_titletime=%d, war_winworld=%d, \
						tax_setrate=%d, tax_taxrate=%d, tax_taxmoney=%d, tax_corpsmoney=%d, tax_settimes=%d, war_nosubmit=%d";
	CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_update_worldglobal, 0, 0, 5, SQL_NONE, UPDATE, 
		tpSql, mWarSerial, CWorldModule::GetSingleton().mWarStatus, mTitleTime, mWinWorld, mSetRate, mTaxRate, mTaxMoney, mCorpsMoney, mSetTimes, 
		CWorldModule::GetSingleton().mNoSubmit );

	return SUCCESS;
}

int CWorldManager::UpdateWorldWarCharDBInfo( int vWorldID )
{
	if ( vWorldID <= WORLD_START || vWorldID >= WORLD_NUM )
	{
		return -1;
	}
	CWorld *tpWorld = &( mWorld[ vWorldID ] );

	for ( int i = METIER_NONE; i < METIER_NOVICE; i++ )
	{
		for ( int j = 0; j < MAX_WAR_WINNER; j++ )
		{
			const char *tpSql = "update UMS_WORLDCHAR set charid=%d, honor=%d, killnum=%d where world_id=%d and metier=%d and rank=%d";
			CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_update_worldwarchar, 0, 0, 5, SQL_NONE, UPDATE, 
				tpSql, tpWorld->mWinChar[ i ][ j ].mCharID, tpWorld->mWinChar[ i ][ j ].mHonor, tpWorld->mWinChar[ i ][ j ].mKill, vWorldID, i, j );
		}
	}	

	return SUCCESS;
}

void CWorldManager::GetAwardBySingle( int vWorldID, int vLevel, int vCharID, int vMetierID, int &vBasicAward, int &vBestAward )
{
	vBasicAward = CWorldManager::GetSingleton().GetBasicAward( vWorldID ) * ( vLevel - AWARD_PARAM );

	int tRank = CWorldManager::GetSingleton().GetRank( vWorldID, vCharID, vMetierID );
	if ( tRank != -1 )
	{
		int tRate = 0;
		if ( tRank == 0 )
			tRate = FIRST_PERCENT;
		else if ( tRank == 1 )
			tRate = SECOND_PERCENT;
		else if ( tRank == 2 )
			tRate = THIRD_PERCENT;
		vBestAward = CWorldManager::GetSingleton().GetBestAward( vWorldID ) / SERVER_PERCENT_FLOAT * tRate;
	}
}

CWorld* CWorldManager::GetWorldByID(int nWorldID)
{
	if (IS_WORLDID(nWorldID))
	{
		return (mWorld+nWorldID);
	}
	return NULL;
}

bool CWorldManager::IsWinner(int nWorldID)
{
	if (nWorldID == mWinWorld)
	{
		return true;
	}
	return false;
}

void CWorldManager::CheckAccuseTimer( int vWorldID, int vOffset )
{
	if ( vWorldID <= WORLD_START || vWorldID >= WORLD_NUM )
	{
		return;
	}
	CWorld *tpWorld = &( mWorld[ vWorldID ] );

	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpWorld->mCountry[ COUNTRY_CHANGAN ].GetCorpsID() );
	if ( tpCorps == NULL )
	{
		return;
	}

	if ( tpWorld->mCountry[ COUNTRY_CHANGAN ].GetKingPower() != tpWorld->mCountry[ COUNTRY_CHANGAN ].GetLastPower() )
	{
		CWorldModule::GetSingleton().SendAccuseInfoNotify( NOTICE_ACCUSE_POWERCHANGE, vWorldID, tpWorld->mCountry[ COUNTRY_CHANGAN ].GetLastPower(), 
			tpWorld->mCountry[ COUNTRY_CHANGAN ].GetKingPower(), 0, 0, tpCorps->GetRooterName(), tpCorps->GetRooterID() );
		tpWorld->mCountry[ COUNTRY_CHANGAN ].SetLastPower( tpWorld->mCountry[ COUNTRY_CHANGAN ].GetKingPower() );
	}

	if ( tpWorld->mCountry[ COUNTRY_CHANGAN ].GetKingPower() <= 0 )
	{
		if ( tpWorld->mAccuseState == STATE_NOACCUSE )
		{
			CWorldModule::GetSingleton().SendAccuseInfoNotify( NOTICE_ACCUSE_TOBE, vWorldID, vWorldID, 0, 0, 0, tpCorps->GetRooterName() );
			tpWorld->mAccuseState = STATE_TOBE;
		}
		// 已经弹劾过直接下台
		else if ( tpWorld->mAccuseState == STATE_ACCUSED )
		{
			tpWorld->mAccuseState = STATE_NOACCUSE;
			CWorldModule::GetSingleton().SendAccuseInfoNotify( NOTICE_ACCUSE_DIRECT, 0, vWorldID, 0, 0, 0, tpCorps->GetRooterName() );
			SysForceAccuse( vWorldID, COUNTRY_MAP_CHANGAN );
			LogWorldAction( vWorldID, EM_WORLD_FORCE, 0, 0, 0 );
			tpCorps->SetForbidTime( time(NULL) + ACCUSE_FORBID_TIME );
			return;
		}
	}

	CTemplateAccuseConfig *tpConfig = (CTemplateAccuseConfig*)CDataStatic::GetTemp( TEMP_ACCUSECONFIG );
	if ( tpConfig == NULL )
		return;

	time_t tval;
	struct tm *now;        
	time(&tval);
	now = localtime(&tval);
	int tConfigNoticeMin = tpConfig->mAccuseNoticeHour * 60 + tpConfig->mAccuseNoticeMinute;
	int tConfigEndMin = tpConfig->mAccuseEndHour * 60 + tpConfig->mAccuseEndMinute;
	int tNowMin = now->tm_hour * 60 + now->tm_min;

	if ( tpWorld->mGMAccuse == STATE_FORCEACCUSE )
	{
		tConfigNoticeMin = tNowMin;
	}
	if ( tConfigNoticeMin <= tNowMin && tConfigEndMin >= tNowMin && tpWorld->mAccuseState == STATE_TOBE )
	{
		tpWorld->mAccuseState = STATE_PREPARE;
	}	

	if ( tpWorld->mGMAccuse == STATE_FORCEEND )
	{
		tConfigEndMin = tNowMin;
	}

	if ( tConfigEndMin <= tNowMin && tpWorld->mAccuseState == STATE_ACCUSING )
	{
		// 弹劾参与人数不足
		int tTotal = tpWorld->mAccuseYes + tpWorld->mAccuseNo;
		CCountry *tpCountry = &(tpWorld->mCountry[ COUNTRY_CHANGAN ]);
		if ( tTotal < tpConfig->mAccusePerson )
		{
			CWorldModule::GetSingleton().SendAccuseInfoNotify( NOTICE_ACCUSE_LESSPEOPLE, 0, vWorldID, tpWorld->mAccuseYes, tpWorld->mAccuseNo, 0, tpCorps->GetRooterName() );
			tpCountry->SetKingPower( tpCountry->GetKingPower() + tpConfig->mAwardPower );
		}
		// 弹劾比例不足
		else if ( tpWorld->mAccuseYes / (float)tTotal < tpConfig->mAccusePercent / SERVER_PERCENT_FLOAT )
		{
			CWorldModule::GetSingleton().SendAccuseInfoNotify( NOTICE_ACCUSE_LESSPERCENT, 0, vWorldID, tpWorld->mAccuseYes, tpWorld->mAccuseNo, 0, tpCorps->GetRooterName() );
			tpCountry->SetKingPower( tpCountry->GetKingPower() + tpConfig->mAwardPower );
		}
		else
		{
			CWorldModule::GetSingleton().SendAccuseInfoNotify( NOTICE_ACCUSE_SUCCESS, 0, vWorldID, tpWorld->mAccuseYes, tpWorld->mAccuseNo, 0, tpCorps->GetRooterName() );
			SysForceAccuse( vWorldID, COUNTRY_MAP_CHANGAN );
			tpCorps->SetForbidTime( time(NULL) + ACCUSE_FORBID_TIME );
		}
		LogWorldAction( vWorldID, EM_WORLD_ACCUSEEND, tpWorld->mAccuseYes, tpWorld->mAccuseNo, tpWorld->mAccuseAbort );
		tpWorld->mAccuseState = STATE_ACCUSED;
		tpWorld->mAccuseYes = 0;
		tpWorld->mAccuseNo = 0;
		tpWorld->mAccuseAbort = 0;
	}

	if ( tpWorld->mAccuseState == STATE_PREPARE ) 
	{
		if ( tpWorld->mAccusePreNoticeTimer.Elapse( vOffset ) == true )
		{
			CWorldModule::GetSingleton().SendAccuseInfoNotify( NOTICE_ACCUSE_PREPARE, vWorldID, vWorldID, 0, 0, 0, tpCorps->GetRooterName() );
		}
		if ( tpWorld->mAccusePrepareTimer.Elapse( vOffset ) == true )
		{
			tpWorld->mAccuseState = STATE_ACCUSING;
			CWorldModule::GetSingleton().SendAccuseInfoNotify( NOTICE_ACCUSE_BEGIN, vWorldID, vWorldID, 0, 0, 0, tpCorps->GetRooterName() );
			CWorldModule::GetSingleton().SendAccuseKingNotify( vWorldID, NULL );	
			LogWorldAction( vWorldID, EM_WORLD_ACCUSEBEGIN, 0 , 0, 0 );
		}
	}

	if ( tpWorld->mAccuseState == STATE_ACCUSING )
	{
		if ( tpWorld->mAccuseNoticeTimer.Elapse( vOffset ) == true )
		{
			CWorldModule::GetSingleton().SendAccuseInfoNotify( NOTICE_ACCUSE_HALFHOUR, vWorldID, vWorldID, tpWorld->mAccuseYes, tpWorld->mAccuseNo, 0, tpCorps->GetRooterName() );
		}
		if ( tpWorld->mAccuseSyncTimer.Elapse( vOffset ) == true )
		{
			CWorldModule::GetSingleton().SendAccuseStatusNotify( vWorldID, NULL );
		}
	}
}

void CWorldManager::SysForceAccuse( int vWorldID, int vMapID )
{
	if ( vWorldID <= WORLD_START || vWorldID >= WORLD_NUM )
	{
		return;
	}
	CWorld *tpWorld = &( mWorld[ vWorldID ] );
	int tCountryID = GetCountryIDByMap( vMapID );
	if ( tCountryID < 0 || tCountryID >= COUNTRY_NUM )
	{
		return;
	}
	CCountry *tpCountry = &(tpWorld->mCountry[ tCountryID ]);
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpCountry->GetCorpsID() );
	if ( tpCorps == NULL )
		return;
	int tNpcID = tpCorps->HasOwnThisMap( vMapID );
	tpCorps->ClearNpcBattleInfo( tNpcID ); 
	CBidMgr::GetSingleton().SetNpcOwner(GATENPCID( tNpcID, vWorldID ), 0);
	if ( IS_MAIN_CITY( vMapID ) )
	{
		tpCorps->SetBattleCity( 0 );
		LoadCorpsFromMap( vWorldID, vMapID, 0 );
		CCorpservice::GetSingleton().OnEventCityUpgrade( tpCorps, vWorldID );
	}
	CCorpsManager::GetSingleton().SendCorpsNpcSingle( tpCorps->GetCorpsCountryID(), tNpcID, tpCorps->GetCorpsID(), false );
}

void CWorldManager::ResetAccuseState( int vWorldID )
{
	if ( vWorldID <= WORLD_START || vWorldID >= WORLD_NUM )
	{
		return;
	}
	CWorld *tpWorld = &( mWorld[ vWorldID ] );
	tpWorld->mAccuseState = STATE_NOACCUSE;
}

int CWorldManager::GetGTTarget( int nWorldID )
{
	if ( nWorldID <= WORLD_START || nWorldID >= WORLD_NUM )
	{
		return 0;
	}
	return mWorld[ nWorldID ].mGTWorld;
}

int CWorldManager::GetGTEndTime( int nWorldID )
{
	if ( nWorldID <= WORLD_START || nWorldID >= WORLD_NUM )
	{
		return 0;
	}
	return mWorld[ nWorldID ].mGTEndTime;
}

int CWorldManager::GetLWTarget( int nWorldID )
{
	if ( nWorldID <= WORLD_START || nWorldID >= WORLD_NUM )
	{
		return 0;
	}
	return mWorld[ nWorldID ].mLWWorld;
}

int CWorldManager::GetLWEndTime( int nWorldID )
{
	if ( nWorldID <= WORLD_START || nWorldID >= WORLD_NUM )
	{
		return 0;
	}
	return mWorld[ nWorldID ].mLWEndTime;
}

int CWorldManager::SetGT( int nSrcWorld, int nDesWorld, int nTime )
{
	if ( nSrcWorld <= WORLD_START || nSrcWorld >= WORLD_NUM || nDesWorld < WORLD_START || nDesWorld >= WORLD_NUM )
	{
		return -1;
	}
	mWorld[ nSrcWorld ].mGTWorld = nDesWorld;
	mWorld[ nSrcWorld ].mGTEndTime = nTime;
	return 0;
}

int CWorldManager::SetLW( int nSrcWorld, int nDesWorld, int nTime )
{
	if ( nSrcWorld <= WORLD_START || nSrcWorld >= WORLD_NUM || nDesWorld < WORLD_START || nDesWorld >= WORLD_NUM )
	{
		return -1;
	}
	mWorld[ nSrcWorld ].mLWWorld = nDesWorld;
	mWorld[ nSrcWorld ].mLWEndTime = nTime;
	return 0;
}

int CWorldManager::CheckGTAndLW( )
{
	int tNowTime = time(NULL);
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		if ( GetGTEndTime( i ) <= tNowTime && GetGTTarget( i ) != 0 )
		{
			CWorldModule::GetSingleton().SendMakeGTNotify( i, GetGTTarget( i ), 0, NULL );
			SetGT( i, 0, 0 );
		}
		if ( GetLWEndTime( i ) <= tNowTime && GetLWTarget( i ) != 0 )
		{
			CWorldModule::GetSingleton().SendMakeLWNotify( i, GetLWTarget( i ), 0, NULL );
			SetLW( i, 0, 0 );
		}
	}
	return 0;
}

int CWorldManager::SendGTAndLWToScene( int nServerID )
{
	CMessage tNotifyMessage;
	CMessageSyncGTAndLWNotify tNotify;
	tNotifyMessage.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_SYNCGTANDLW );
	tNotifyMessage.set_msgpara( (long) &tNotify );

	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		CWorld *tpWorld = &( mWorld[ i ] );
		if ( tpWorld->mGTWorld != 0 && tpWorld->mGTEndTime != 0 )
		{
			GTAndLWInfo *tpInfo = tNotify.add_gtinfo();
			tpInfo->set_srcworldid( i );
			tpInfo->set_desworldid( tpWorld->mGTWorld );
			tpInfo->set_targettime( tpWorld->mGTEndTime );
		}
		if ( tpWorld->mLWWorld != 0 && tpWorld->mLWEndTime != 0 )
		{
			GTAndLWInfo *tpInfo = tNotify.add_lwinfo();
			tpInfo->set_srcworldid( i );
			tpInfo->set_desworldid( tpWorld->mLWWorld );
			tpInfo->set_targettime( tpWorld->mLWEndTime );
		}
	}
	CGateServer::GetSingleton().Send2Scene( nServerID, &tNotifyMessage );
	return 0;
}

