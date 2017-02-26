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

using namespace GateServer;	
CSharedMem *CWorldManager::mShmPtr = NULL;
template<> CWorldManager * CSingleton< CWorldManager >::spSingleton = NULL;
template<> CWorldModule * CSingleton< CWorldModule >::spSingleton = NULL;
CWorldManager *gpWorldManager = NULL;





// ***********************************************************
//  Function:		OnLaunchServer
//  Description:	登录
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CWorldModule::OnLaunchServer( )
{  
	CWorldManager::mShmPtr = mShmPtr;
	gpWorldManager = new CWorldManager;	  
	LK_ASSERT( gpWorldManager != NULL, return );

	if ( mShmPtr->GetInitMode() == SHM_INIT)
	{
				
	}
}

// ***********************************************************
//  Function:		OnTimer
//  Description:	计时器
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CWorldModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	if ( mTimer.Elapse( vTickOffset ) == true )
	{
		if ( IsLaunched() == false )
		{
			return;
		}
		CWorldManager::GetSingleton().UpdateWorldGlobalDBInfo();
		for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
		{
			CWorldManager::GetSingleton().UpdateWorldStatusDBInfo( i );
			for ( int j = 0; j < COUNTRY_NUM; j++ )
			{
				CWorldManager::GetSingleton().UpdateWorldDBInfo( i, j );
				CWorldManager::GetSingleton().UpdateOfficialDBInfo( i, j );
			}
		}		
	}
	if ( mGTLWTimer.Elapse( vTickOffset ) == true )
	{
		if ( IsLaunched() == false )
		{
			return;
		}
		CWorldManager::GetSingleton().CheckGTAndLW();
	}
	if ( mWarTimer.Elapse( vTickOffset ) == true )
	{
		if ( IsLaunched() == false )
		{
			return;
		}
		// 战争信息
		CheckWarTime( );

		// 弱势阵营计算
		m_WeakProtecter.OnTimerCheckState();
	}
	if ( mWarStatus == WAR_BATTLE )
	{
		if ( mSeedTimer.Elapse( vTickOffset ) == true )
		{
			if ( IsLaunched() == false )
			{
				return;
			}
			CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
			if ( tpConfig == NULL )
				return;
			for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
			{
				for ( int j = 0; j < (int)ARRAY_CNT(tpConfig->mMapID); j++ )
				{
					if ( tpConfig->mMapID[ j ] == 0 )
						break;
					CWorldManager::GetSingleton().AddSource( i, j, CWorldManager::GetSingleton().GetSeed( i, j ) );
					SendWarSource( i, tpConfig->mMapID[ j ], CWorldManager::GetSingleton().GetSource( i, j ) );
				}
			}
		}
	}

	if ( mAccuseTimer.Elapse( vTickOffset ) == true )
	{
		if ( IsLaunched() == false )
		{
			return;
		}
		for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
		{
			CWorldManager::GetSingleton().CheckAccuseTimer( i, ACCUSE_CHECK_TIME );
		}
	}	
}

// ***********************************************************
//  Function:		OnExitServer
//  Description:	退出
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CWorldModule::OnExitServer( )
{
	if ( IsLaunched() == false )
	{
		return;
	}
	CWorldManager::GetSingleton().UpdateWorldGlobalDBInfo();
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		CWorldManager::GetSingleton().UpdateWorldStatusDBInfo( i );
		CWorldManager::GetSingleton().UpdateWorldWarDBInfo( i );		
		for ( int j = 0; j < COUNTRY_NUM; j++ )
		{
			CWorldManager::GetSingleton().UpdateWorldDBInfo( i, j );
			CWorldManager::GetSingleton().UpdateOfficialDBInfo( i, j );
		}
	}	
}

// ***********************************************************
//  Function:		CWorldModule
//  Description:	构造函数
//  Output:			
//  Date:	06/12/2009
// **********************************************************
CWorldModule::CWorldModule()
{
	if( CObj::msCreateMode ) 
	{
		Initialize();
	}
	else
	{
		Resume();
	}
}

// ***********************************************************
//  Function:		CountSize
//  Description:	
//  Output:			
//  Date:	06/12/2009
// **********************************************************
unsigned int CWorldModule::CountSize()
{
	unsigned int tSize = sizeof( CWorldManager );
	tSize += sizeof( CWorldModule );
	return tSize;
};	

// ***********************************************************
//  Function:		Initialize
//  Description:	初始化函数
//  Output:			
//  Date:	06/12/2009
// **********************************************************
int CWorldModule::Initialize( )
{
	mTimer = CWTimer( WORLD_SAVE_TIME );
	mTokenID = 0;
	mWorldLaunched = 0;
	mOfficialLaunched = 0;
	mStatusLaunched = 0;
	mWarTimer = CWTimer( WAR_CHECK_TIME );
	mWarStatus = WAR_NONE;
	mSeedTimer = CWTimer( WAR_SEED_TIME );
	mGMHour = 0;
	mGMMin = 0;
	mGMDay = -1;
	mWarLaunched = 0;
	mCharLaunched = 0;
	mGlobalLaunched = 0;
	mNoSubmit = 0;
	mAccuseTimer = CWTimer( ACCUSE_CHECK_TIME );
	mGTLWTimer = CWTimer( GTLW_CHECK_TIME );
	return 0;
}

// ***********************************************************
//  Function:		Resume
//  Description:	
//  Output:			
//  Date:	06/12/2009
// **********************************************************
int CWorldModule::Resume( )
{
	return 0;
}


bool CWorldModule::IsLaunched()
{
	 if ( mWorldLaunched != ( WORLD_NUM - 1 ) * COUNTRY_NUM )
		 return false;
	 if( mOfficialLaunched != ( WORLD_NUM - 1 ) * COUNTRY_NUM * OFFICIAL_FUNCTION_NUM ) 
		 return false;
	 if ( mStatusLaunched != 1 )
		 return false; 
	 if ( mWarLaunched != 1 )
		 return false; 
	 if ( mCharLaunched != 1 )
		 return false;
	 if ( mGlobalLaunched != 1 )
		 return false;
	 return true;
}

void CWorldModule::LoadWorld( )
{
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		for ( int j = 0; j < COUNTRY_NUM; j++ )
		{
			const char* pSqlstr = "select world_id, country_id, king_power, people_num, wizard0, wizard1,  \
								  wizard2, wizard3, wizard4, wizard5, convalue0, convalue1, convalue2, convalue3, convalue4, convalue5, \
								  conlevel0, conlevel1, conlevel2, conlevel3, conlevel4, conlevel5, conwizard0, conwizard1, conwizard2, conwizard3, \
								  conwizard4, conwizard5, version, official from UMS_WORLD where world_id=%d and country_id=%d";
			CGateLogic::GetSingleton().ExecuteSql( em_dbsession_load_country, 0, 0, 5, SQL_NEED_CALLBACK, SELECT, 
				pSqlstr, i, j );
			//LoadOfficial( i, j );
		}
	}		
}

void CWorldModule::LoadWorldStatus( )
{
	const char* pSqlstr = "select world_id, friend0, friend1, friend2, friend3, status0,  \
						  status1, status2, status3, number,\
						  honer_inc, \
						  level_score, \
						  honer_score, \
						  weak_level, accuse_state, accuse_yes, accuse_no, accuse_abort, gate_times, flag_times, \
						  gt_target_world, gt_target_time, lw_target_world, lw_target_time \
						  from UMS_WORLDSTATUS";
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_load_countrystatus, 0, 0, 5, SQL_NEED_CALLBACK, SELECT, 
		pSqlstr );	
}

void CWorldModule::LoadOfficial( )
{
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		for ( int j = 0; j < COUNTRY_NUM; j++ )
		{
			LoadOfficial( i, j ); 
		}
	}
}

void CWorldModule::LoadOfficial( int vWorldID, int vCountryID )
{
	for ( int i = 0; i < OFFICIAL_FUNCTION_NUM; i++ )
	{
		const char* pSqlstr = "select world_id, country_id, official%d from UMS_WORLD where world_id=%d and country_id=%d";
		CGateLogic::GetSingleton().ExecuteSql( em_dbsession_load_official, i, 0, 5, SQL_NEED_CALLBACK, SELECT, 
			pSqlstr, i, vWorldID, vCountryID );
	}	
}




void CWorldModule::AddOperation( int vSrcWorldID, int vDesWorldID, int vOpType )
{
	if ( vSrcWorldID <= WORLD_START || vSrcWorldID >= WORLD_NUM || vDesWorldID <= WORLD_START || vDesWorldID >= WORLD_NUM )
	{
		return;
	}
	CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ vSrcWorldID ] );
	tpWorld->mOperationList[ vDesWorldID ] |= vOpType;
}

void CWorldModule::SetOperation( int vSrcWorldID, int vCharID )
{
	if ( vSrcWorldID <= WORLD_START || vSrcWorldID >= WORLD_NUM )
		return;
	CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ vSrcWorldID ] );
	if ( tpWorld->mCountry[ COUNTRY_CHANGAN ].GetKingID( ) != vCharID )
		return;
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );
	if ( tpPlayer == NULL )
		return;
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{		
		if ( ( tpWorld->mOperationList[ i ] & WORLD_OP_INVITE ) == WORLD_OP_INVITE )
		{
			SendWorldQuestNotify( vCharID, i, WORLD_OP_INVITE );
		}
		if ( ( tpWorld->mOperationList[ i ] & WORLD_OP_PEACE ) == WORLD_OP_PEACE )
		{
			SendWorldQuestNotify( vCharID, i, WORLD_OP_PEACE );
		}
	}
}

bool CWorldModule::HasOperation( int vSrcWorldID, int vDesWorldID, int vOpType )
{
	if ( vSrcWorldID <= WORLD_START || vSrcWorldID >= WORLD_NUM || vDesWorldID <= WORLD_START || vDesWorldID >= WORLD_NUM )
	{
		return false;
	}
	CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ vSrcWorldID ] );
	if ( ( tpWorld->mOperationList[ vDesWorldID ] & vOpType ) == vOpType )
	{
		return true;
	}
	return false;
}

void CWorldModule::ClearOperation( int vSrcWorldID, int vDesWorldID, int vOpType )
{
	if ( vSrcWorldID <= WORLD_START || vSrcWorldID >= WORLD_NUM || vDesWorldID <= WORLD_START || vDesWorldID >= WORLD_NUM )
	{
		return;
	}
	CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ vSrcWorldID ] );
	tpWorld->mOperationList[ vDesWorldID ] &= ~vOpType;
}

void CWorldModule::LoadWorldWar( )
{
	const char* pSqlstr = "select world_id, totallevel, totalnum, campid, bidmoney, bidcampid,  \
						  basicaward, bestaward, source0, source1, source2, submitmoney from UMS_WORLDWAR";
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_load_worldwar, 0, 0, 5, SQL_NEED_CALLBACK, SELECT, 
		pSqlstr );	
}


void CWorldModule::LoadWorldGlobal( )
{
	const char* pSqlstr = "select war_serial, war_status, war_titletime, war_winworld, \
						  tax_setrate, tax_taxrate, tax_taxmoney, tax_corpsmoney, tax_settimes, war_nosubmit from UMS_WORLDGLOBAL";
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_load_worldglobal, 0, 0, 5, SQL_NEED_CALLBACK, SELECT, 
		pSqlstr );	
}



void CWorldModule::LoadWorldWarChar( )
{
	const char* pSqlstr = "select world_id, charid, honor, killnum, rank, metier from UMS_WORLDCHAR";
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_load_worldwarchar, 0, 0, 5, SQL_NEED_CALLBACK, SELECT, 
		pSqlstr );	
}





// 获得称号处理 [1/28/2010 Macro]

/*
*	nMapID : 为占领的地图ID
*	nAction: 加入国籍、占领国家、占领野外地图(CTplTitleManager::ECndisnScope)
*/

void CWorldModule::OnEventUpdateNationTitle( int nRoleID, int nMapID, int nAction, bool bAdd )
{
	if ( nRoleID == INVALID_OBJ_ID || nMapID == INVALID_MAP_ID )
	{
		return ;
	}

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}	

	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( nRoleID );
	if ( tpCorps == NULL )
	{
		return ;
	}

	// 根据玩家行为，转换得到称号ID
	int tTitleID = CDataStatic::GetTitle( nMapID, nAction, 0 );
	if ( tTitleID == INVALID_OBJ_ID )
	{
		return ;
	}

	// 如果加入国籍，对加入者做通知
	if ( nAction == CTplTitleManager::SCOPE_NATIONALITY )
	{
		SendNationTitleUpdateNotice( tpPlayer, tTitleID, bAdd );
	}
	else
	{		
		int tMemList[ CORPS_MAXMEMBERNUM ] = { 0 };
		int tMemNum = ARRAY_CNT( tMemList );
		CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tMemList, tMemNum );
		for ( int i = 0; i < tMemNum; i ++ )
		{
			CGatePlayer* tpDesPlayer = CPlayerManager::GetSingleton().ScenePlayer( tMemList[ i ] );
			if ( tpDesPlayer == NULL )
			{
				continue;
			}
			if ( tpDesPlayer->CharID() == tpCorps->GetRooterID() )
			{
				//  野外地图山大王
				if ( IS_MAIN_CITY( nMapID ) == false )
				{
					int tKingTitleID = CDataStatic::GetTitle( nMapID, nAction, CTplTitleManager::JUNTUANZHANG );
					if( tKingTitleID != INVALID_OBJ_ID )
						SendNationTitleUpdateNotice( tpDesPlayer, tKingTitleID, bAdd );
				}
				continue;
			}
			SendNationTitleUpdateNotice( tpDesPlayer, tTitleID, bAdd );
		}
	}

	return ; 
}


void CWorldModule::UpdateAllWar( )
{
	if ( IsLaunched() == false )
	{
		return;
	}
	CWorldManager::GetSingleton().UpdateWorldGlobalDBInfo();
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		CWorldManager::GetSingleton().UpdateWorldStatusDBInfo( i );
		CWorldManager::GetSingleton().UpdateWorldWarDBInfo( i );
		CWorldManager::GetSingleton().UpdateWorldWarCharDBInfo( i );
	}
}



void CWorldModule::CheckWarTime( )
{
	time_t tval;
	struct tm *now;        
	time(&tval);
	now = localtime(&tval);

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;
	int tConfigDay = mGMDay == -1 ? now->tm_wday : mGMDay;
	int tSubmitConfigTime = tpConfig->mSubmitBegin.mHour * 60 + tpConfig->mSubmitBegin.mMinute;
	int tBidConfigTime = tpConfig->mBidEnd.mHour * 60 + tpConfig->mBidEnd.mMinute;
	int tBeginConfigTime = tpConfig->mBattleBegin.mHour * 60 + tpConfig->mBattleBegin.mMinute;
	int tEndConfigTime = tpConfig->mBattleEnd.mHour * 60 + tpConfig->mBattleEnd.mMinute;
	int tNoticeConfigTime = tpConfig->mNoticeBegin.mHour * 60 + tpConfig->mNoticeBegin.mMinute;
	int tNowTime =  now->tm_hour * 60 + now->tm_min;
	if ( mGMHour != 0 && mGMMin != 0 )
	{
		tNowTime = mGMHour * 60 + mGMMin;
	}
	// 如果报名开始
	if ( tConfigDay == tpConfig->mSubmitBegin.mDay && tNowTime >= tSubmitConfigTime )
	{
		if ( mWarStatus == WAR_NONE )
		{
			mWarStatus = WAR_SUBMIT;
			SendWarNotify( NOTICE_WAR_STATUSCHANGED, mWarStatus, 0, 0, NULL );
		}
	}
	// 如果竞价结束
	if ( tConfigDay == tpConfig->mBidEnd.mDay && tNowTime >= tBidConfigTime )
	{
		if ( mWarStatus == WAR_NONE || mWarStatus == WAR_SUBMIT )
		{
			mWarStatus = WAR_BIDOVER;
			SendWarNotify( NOTICE_WAR_STATUSCHANGED, mWarStatus, 0, 0, NULL );
			CWorldManager::GetSingleton().CheckBidWorld( );
			SendWarCamp( );
			SendReliveChange( 0 );
			UpdateAllWar( );
		}
	}
	if ( tConfigDay == tpConfig->mBattleBegin.mDay && tNowTime >= tNoticeConfigTime && tNowTime < tBeginConfigTime )
	{
		if ( mWarStatus == WAR_NONE || mWarStatus == WAR_SUBMIT || mWarStatus == WAR_BIDOVER )
		{
			mWarStatus = WAR_NOTICE;
			SendWarNotify( NOTICE_WAR_STATUSCHANGED, mWarStatus, 0, 0, NULL );
		}
	}
	// 如果国战正式开始
	if ( tConfigDay == tpConfig->mBattleBegin.mDay && tNowTime >= tBeginConfigTime && tNowTime < tEndConfigTime )
	{
		if ( mWarStatus != WAR_BATTLE )
		{
			mWarStatus = WAR_BATTLE;
			SendWarNotify( NOTICE_WAR_STATUSCHANGED, mWarStatus, 0, 0, NULL );
			CWorldManager::GetSingleton().StartClearWar( );
			SendSyncWarStatus( 0 );
			UpdateAllWar( );
			SendCreateWarStoneNotify( 0, 0 );
		}
	}
	// 如果国战结束
	if ( tConfigDay == tpConfig->mBattleBegin.mDay && tNowTime >= tEndConfigTime )
	{
		if ( mWarStatus != WAR_NONE )
		{
			mWarStatus = WAR_NONE;
			SendWarNotify( NOTICE_WAR_STATUSCHANGED, mWarStatus, 0, 0, NULL );
			CWorldManager::GetSingleton().GetWinWorld();
			CWorldManager::GetSingleton().CheckAward( );
			CWorldManager::GetSingleton().EndClearWar( );			
			CWorldManager::GetSingleton().mWarSerial++;
			CWorldManager::GetSingleton().mTitleTime = time(NULL) + tpConfig->mTitleValidTime;
			SendSyncWarStatus( 0 );
			// 强制调存盘
			UpdateAllWar( );
		}
	}
}

void CWorldModule::OnNewDayCome( )
{
	CWorldManager::GetSingleton().DailyRefresh( );
}
