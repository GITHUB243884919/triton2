#include "base.h"
#include "rankmodule.h"
#include "servermessage_pb.hxx.pb.h"
#include "shm.h"
#include "gatelogic.h"
#include "rankmessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "gateobj_manager.h"
#include "entity.h"
#include "nameservice.h"
#include "family.h"
#include "corps.h"
#include "family_manager.h"
#include "corpsmanager.h"
#include "activity.h"

template<> CRankModule* CSingleton< CRankModule >::spSingleton = NULL;


CRankModule::CRankModule()
{
	if( mShmPtr->GetInitMode() == SHM_INIT )
	{
		mInTimeRankSaveTimer = CWTimer(300000);
		mHalfHourTimer = CWTimer(1800000);
		
		for( int i = 0; i < REFRESHTYPE_ALL; i++ )
		{
			mRecvSceneList[i].initailize();
		}
		mLaunchFlag = 0;
		mLastDayTick=time(0);
	}
}

CRankModule::~CRankModule()
{

}

// 启动服务器
void CRankModule::OnLaunchServer( )
{
	if ( mShmPtr->GetInitMode() == SHM_INIT)
	{
		LoadCurrentPlayerRankFromDB();
		LoadInTimeLevelRankFromDB();
	}
}

// 退出服务器
void CRankModule::OnExitServer( )
{
	// 保存实时数据
	SaveInTimePlayerRankToDB( );
}

// 服务器Timer
void CRankModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	if( mInTimeRankSaveTimer.Elapse( vTickOffset) )
	{
		// 保存玩家排行榜(实时)                                                                                                                                                                                                                                                                                                                                   
		SaveInTimePlayerRankToDB();
	}

	if( mHalfHourTimer.Elapse( vTickOffset) )
	{
		// 半小时刷新
		HalfHourUpdate();
	}
}

// 每日刷新
int CRankModule::DailyUpdate( )
{
	LOG_NOTICE( "rank", "[%s:%d] begin", __FUNCTION__, __LINE__);
	CMessagePersonalRankInfoRequest msg;
	msg.set_rtype(REFRESHTYPE_DAYLY);
	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2S_REQUEST_PERSONALRANKINFO, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &msg);

	mRecvSceneList[REFRESHTYPE_DAYLY].clear();

	// 先问各场景要一次数据
	for ( int i = 1; i <= CGateServer::GetSingleton().GetMaxSceneServerID(); ++i)
	{
		CSceneServer *pScene =  CGateServer::GetSingleton().GetServerByID(i);
		if ( pScene )
		{			
			FillTopTenData<CMessagePersonalRankInfoRequest>( &msg, (int)pScene->mServerID );
			LOG_NOTICE("rank", "%s: ASK DATA! SvrID=%d", __FUNCTION__, i );
			CGateServer::GetSingleton().Send2Scene( pScene, &tMessage);
			mRecvSceneList[REFRESHTYPE_DAYLY].insert( std::make_pair(i, i ) );

		}
	}
	LOG_NOTICE( "rank", "[%s:%d] finished", __FUNCTION__, __LINE__);
	return 0;
}



// 每半小时刷新
int CRankModule::HalfHourUpdate( )
{
	LOG_NOTICE( "rank", "[%s:%d] begin", __FUNCTION__, __LINE__);

	CMessagePersonalRankInfoRequest msg;
	msg.set_rtype(REFRESHTYPE_HALFHOUR);
	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2S_REQUEST_PERSONALRANKINFO, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &msg);

	mRecvSceneList[REFRESHTYPE_HALFHOUR].clear();

	// 先问各场景要一次数据
	for ( int i = 1; i <= CGateServer::GetSingleton().GetMaxSceneServerID(); ++i)
	{
		CSceneServer *pScene =  CGateServer::GetSingleton().GetServerByID(i);
		if ( pScene )
		{
			LOG_NOTICE("rank", "%s: ASK DATA! SvrID=%d", __FUNCTION__, i );
			CGateServer::GetSingleton().Send2Scene( pScene, &tMessage);
			mRecvSceneList[REFRESHTYPE_HALFHOUR].insert( std::make_pair(i, i ) );
		}
	}

	LOG_NOTICE( "rank", "[%s:%d] finished", __FUNCTION__, __LINE__);
	return 0;
}


int CRankModule::OnDailyUpdate( )
{
	LOG_NOTICE( "rank", "[%s:%d] new day come", __FUNCTION__, __LINE__);

	// 处理非实时排行榜

	// 用实时的比较一次昨天的历史表

	DAILY_UPDATE(mLevelRankInTime, mLevelRankOutdated, RANKLIMIT_LEVEL , RANKTYPE_LEVEL_ALL	)	
	DAILY_UPDATE(mMoneyRankInTime[FREE_MONEY], mMoneyRankOutdated[FREE_MONEY], RANKLIMIT_MONEY_NOBIND , RANKTYPE_MONEY_NOBIND	)	
	DAILY_UPDATE(mMoneyRankInTime[ALL_MONEY], mMoneyRankOutdated[ALL_MONEY], RANKLIMIT_MONEY_ALL , RANKTYPE_MONEY_ALL	)	
	DAILY_UPDATE(mKillRankInTime[KILL_PROT_NATION], mKillRankOutdated[KILL_PROT_NATION], RANKLIMIT_KILL_PROT_NATION , RANKTYPE_KILL_PROT_NATION	)	
	DAILY_UPDATE(mKillRankInTime[KILL_OUT], mKillRankOutdated[KILL_OUT], RANKLIMIT_KILL_OUT , RANKTYPE_KILL_OUT	)	
	DAILY_UPDATE(mKillRankInTime[KILL_TOTAL], mKillRankOutdated[KILL_TOTAL], RANKLIMIT_KILL_TOTAL , RANKTYPE_KILL_TOTAL	)	
	DAILY_UPDATE(mHonorRankInTime, mHonorRankOutdated, RANKLIMIT_HONOR , RANKTYPE_HONOR_ALL	)	
	DAILY_UPDATE(mDieNumRankInTime, mDieNumRankOutdated, RANKLIMIT_DIE , RANKTYPE_DIE	)	
	DAILY_UPDATE(mEndTaskRankInTime, mEndTaskRankOutdated, RANKLIMIT_END_TASK , RANKTYPE_END_TASK	)	
	DAILY_UPDATE(mOnLineRankInTime, mOnLineRankOutdated, RANKLIMIT_ONLINE_TIME , RANKTYPE_ONLINE_TIME	)	
	DAILY_UPDATE(mPantaoRankInTime, mPantaoRankOutdated, RANKLIMIT_REPETIONMARK , RANKTYPE_PANTAO_MARK	)	
	DAILY_UPDATE(mLonggongRankInTime, mLonggongRankOutdated, RANKLIMIT_REPETIONMARK , RANKTYPE_LONGGONG_MARK	)	
	DAILY_UPDATE(mOriginReliveRankInTime, mOriginReliveRankOutdated, RANKLIMIT_ORIGINRELIVETIMES , RANKTYPE_ORIGINRELIVETIMES	)	
	DAILY_UPDATE(mKillSameNationRankInTime, mKillSameNationRankOutdated, RANKLIMIT_KILLSAMENATIONNUM , RANKTYPE_KILLSAMENATIONNUM	)	
	DAILY_UPDATE(mKillOgreRankInTime, mKillOgreRankOutdated, RANKLIMIT_KILLOGRENUM , RANKTYPE_KILLOGRENUM	)	
	DAILY_UPDATE(mUpFailRankInTime, mUpFailRankOutdated, RANKLIMIT_UPFAILNUM , RANKTYPE_UPFAILNUM	)	
	DAILY_UPDATE(mDayantaRankInTime, mDayantaRankOutdated, RANKLIMIT_REPETIONMARK , RANKTYPE_DAYANTA	)	
	DAILY_UPDATE(mLianhuadongRankInTime, mLianhuadongRankOutdated, RANKLIMIT_REPETIONMARK , RANKTYPE_LIANHUADONG	)	
	DAILY_UPDATE(mHuangfengdongRankInTime, mHuangfengdongRankOutdated, RANKLIMIT_REPETIONMARK , RANKTYPE_HUANGFENGDONG	)	
	DAILY_UPDATE(mAchiveRankInTime, mAchiveRankOutdated, RANKLIMIT_ACHIVE , RANKTYPE_ACHIVE_POINT	)	
	DAILY_UPDATE(mCharmRankInTime, mCharmRankOutdated, RANKLIMIT_CHARM , RANKTYPE_CHARM_VALUE	)	
	DAILY_UPDATE(mFlyDungRankInTime, mFlyDungRankOutdated, RANKLIMIT_REPETIONMARK , RANKTYPE_FLY_FB_MARK	)
	DAILY_UPDATE(mZhanGongInTime, mZhanGongOutdated, RANKLIMIT_COMMON , RANKTYPE_ZHANGONG    )
	DAILY_UPDATE(mZhiGuoInTime, mZhiGuoOutDated, RANKLIMIT_COMMON , RANKTYPE_ZHIGUO    )
	DAILY_UPDATE(mTongShuaiInTime, mTongShuaiOutdated, RANKLIMIT_COMMON , RANKTYPE_TONGSHUAI    )
	DAILY_UPDATE(mWisAllInTime, mWisAllOutdated, RANKLIMIT_COMMON , RANKTYPE_WIS    )
	DAILY_UPDATE(mYunxiaoInTime, mYunxiaoOutdated, RANKLIMIT_COMMON , RANKTYPE_YUNXIAO    )
	DAILY_UPDATE(mWeekCharmIncInTime, mWeekCharmIncOutdated, RANKLIMIT_COMMON , RANKTYPE_WORLD_WEEKINCCHARM    )
	DAILY_UPDATE(mShideInTime, mShideOutdated, RANKLIMIT_COMMON , RANKTYPE_SHIDE			  )
	DAILY_UPDATE(mXianYuanInTime, mXianYuanOutdated, RANKLIMIT_COMMON , RANKTYPE_XIANYUANMARK	  )
	DAILY_UPDATE(mChuShiInTime, mChuShiOutdated, RANKLIMIT_COMMON , RANKTYPE_CHUSHITU		  )
	DAILY_UPDATE(mYunLvInTime, mYunLvOutdated, RANKLIMIT_COMMON , RANKTYPE_YUNXIAOTALEVEL  )
	DAILY_UPDATE(mWuDidongInTime, mWuDidongOutdated, RANKLIMIT_COMMON , RANKTYPE_WUDIDONG  )
    DAILY_UPDATE(mDaLuanDouInTime, mDaLuanDouOutdated, RANKLIMIT_COMMON , RANKTYPE_LASTONE  )
	DAILY_UPDATE(mDaLuanDouTotalInTime, mDaLuanDouTotalOutdated, RANKLIMIT_COMMON , RANKTYPE_TOTAL_LASTONE  )
	DAILY_UPDATE(mGlobalWarInTime, mGlobalWarOutdated, RANKLIMIT_COMMON , RANKTYPE_GLOBALWAR  )
	DAILY_UPDATE(m_SpiritInTime, m_SpiritOutdated, RANKLIMIT_COMMON , RANKTYPE_SPIRIT)

	// 对于团队荣誉，在这里进行数据采集，并进行处理

	CFamilyManager::TFamilies* pFamilyList = CFamilyManager::GetSingleton().GetFamilyList();
	LK_ASSERT(pFamilyList!=NULL, return -1); 

	for ( CFamilyManager::TFamilies::iterator it = pFamilyList->begin(); it != pFamilyList->end(); ++it )
	{
		CFamily *pFamily = ( CFamily * )CGateObjManager::GetSingleton().GetObject( it->second );
		LK_ASSERT( pFamily != NULL, continue );

		mFamilyRankInTime.Update( 
			CPlayerRankTeamPoints( pFamily->GetID(), pFamily->GetName(), pFamily->GetPVPScore(),
			pFamily->GetCountryID(), 0, 0 ), true );

		mFamilyTaoRankInTime.Update( 
			CPlayerRankTeamPoints( pFamily->GetID(), pFamily->GetName(), pFamily->GetRepetionScore(),
			pFamily->GetCountryID(), 0, 0 ), true );
	}


	for ( CCorpsManager::CorpsList::iterator it = CCorpsManager::GetSingleton().m_tCorpsList.begin();
		it != CCorpsManager::GetSingleton().m_tCorpsList.end(); ++it )
	{
		CCorps *tpCorps = ( CCorps * )CGateObjManager::GetSingleton().GetObject( it->second );
		LK_ASSERT( tpCorps != NULL, continue );

		mCorpsRankInTime.Update( 
			CPlayerRankTeamPoints( tpCorps->GetCorpsID(), tpCorps->GetCorpsName(), tpCorps->GetPVPScore(),
			tpCorps->GetCorpsCountryID(), 0, 0 ), true );

		// 添加新的军团排序信息  add by zhengy
		mCorpsBossScoreRankInTime.Update(
			CPlayerRankTeamPoints( tpCorps->GetCorpsID(), tpCorps->GetCorpsName(), tpCorps->GetCorpsBossScore(),
			tpCorps->GetCorpsCountryID(), 0, 0 ), true );

		mCorpsCollectActiveRankInTime.Update(
			CPlayerRankTeamPoints( tpCorps->GetCorpsID(), tpCorps->GetCorpsName(), tpCorps->GetCorpsCollectActive(),
			tpCorps->GetCorpsCountryID(), 0, 0 ), true );
	}

	DAILY_UPDATE(mFamilyRankInTime, mFamilyRankOutdated, RANKLIMIT_TEAM , RANKTYPE_FAMILY_PVP	)	
	DAILY_UPDATE(mFamilyTaoRankInTime, mFamilyTaoRankOutdated, RANKLIMIT_TEAM , RANKTYPE_FAMILY_TAO	)	
	DAILY_UPDATE(mCorpsRankInTime, mCorpsRankOutdated, RANKLIMIT_TEAM , RANKTYPE_CORPS_PVP	)	
	DAILY_UPDATE(mCorpsBossScoreRankInTime, mCorpsBossScoreRankOutdated, RANKLIMIT_TEAM , RANKTYPE_CORPSBOSS_SCORE	)	
	DAILY_UPDATE(mCorpsCollectActiveRankInTime, mCorpsCollectActiveRankOutdated, RANKLIMIT_TEAM , RANKTYPE_CORPSCOLLECT_ACTIVE	)	


	for ( int i = 0; i < 3; ++i )
	{
		mDayKillRankOutdated[i].ClearData();
		mDayKillRankInTime[i].ClearData();
	}
	mDayHonorRankInTime.ClearData();
	mDayHonorRankOutdated.ClearData();

	mDayKillRankOutdated[0].SaveRankDataToDB( RANKTYPE_KILL_PROT_NATION_TODAY, true );  
	mDayKillRankOutdated[1].SaveRankDataToDB( RANKTYPE_KILL_OUT_TODAY, true );  
	mDayKillRankOutdated[2].SaveRankDataToDB( RANKTYPE_KILL_TOTAL_TODAY, true ); 
	mDayHonorRankOutdated.SaveRankDataToDB( RANKTYPE_HONOR_ALL_TODAY, true );


	// 通知各場景更新排行榜稱號
	CMessageNewRankTopTenNotify msg;
	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2S_NOTIFY_NEWTOPTEN, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &msg);

	// 通知各场景服务器
	for ( int i = 1; i <= CGateServer::GetSingleton().GetMaxSceneServerID(); ++i)
	{
		CSceneServer *pScene =  CGateServer::GetSingleton().GetServerByID(i);
		if ( pScene )
		{
			FillTopTenData<CMessageNewRankTopTenNotify>( &msg, (int)pScene->mServerID );
			CGateServer::GetSingleton().Send2Scene( pScene, &tMessage);
		}
	}

	LOG_NOTICE( "rank", "[%s:%d] new day finished", __FUNCTION__, __LINE__);

	// 如果现在是周一 则执行新一周
	if ( IsANewWeek( mLastDayTick, time(0) ) )
		WeeklyUpdate();
	mLastDayTick=time(0);
	return 0;
}

// 半小时刷新
int CRankModule::OnHalfHourUpdate( )
{
	LOG_NOTICE( "rank", "[%s:%d] half hour come", __FUNCTION__, __LINE__);

	DAILY_UPDATE( mDayKillRankInTime[KILL_PROT_NATION]	,mDayKillRankOutdated[KILL_PROT_NATION]	,RANKLIMIT_KILL_PROT_NATION, RANKTYPE_KILL_PROT_NATION_TODAY )
	DAILY_UPDATE( mDayKillRankInTime[KILL_OUT]			,mDayKillRankOutdated[KILL_OUT]			,RANKLIMIT_KILL_OUT, RANKTYPE_KILL_OUT_TODAY )
	DAILY_UPDATE( mDayKillRankInTime[KILL_TOTAL]		,mDayKillRankOutdated[KILL_TOTAL]		,RANKLIMIT_KILL_TOTAL, RANKTYPE_KILL_TOTAL_TODAY )
	DAILY_UPDATE( mDayHonorRankInTime, mDayHonorRankOutdated, RANKLIMIT_HONOR, RANKTYPE_HONOR_ALL_TODAY )

	LOG_NOTICE( "rank", "[%s:%d] half hour finished", __FUNCTION__, __LINE__);

	return 0;
}

int CRankModule::WeeklyUpdate( )
{
	LOG_NOTICE( "rank", "[%s:%d] new week come", __FUNCTION__, __LINE__);


	for ( int i = 1; i <= CGateServer::GetSingleton().GetMaxSceneServerID(); ++i)
	{
		CSceneServer *pScene =  CGateServer::GetSingleton().GetServerByID(i);
		if ( pScene )
		{	
			CMessageActivityEventNotify msg;
			msg.set_activityid(HELPID_BAIHUAXIANZI);
			msg.set_eventid(2);
			RANK_COMMON_ALLRANK_TYPE::CCountryRank* pRank = mWeekCharmIncOutdated.GetCountryRank(i);
			if (pRank==NULL)
			{
				// 给场景服务器发个空的。。。
				msg.set_activityid(0);
				CServiceActivity::GetSingleton().SendActivityEvent2Scene(i, &msg);
				continue;
			}
			CPlayerRankCommon* pMeta = pRank->GetNodeByIndex( 0 );
			if ( pMeta != NULL && pMeta->GetCharID() != 0 )
			{
				msg.add_intparams(pMeta->GetCharID());
				msg.add_strparams(pMeta->GetCharName());
			}
			else
			{
				msg.add_intparams(0);
				msg.add_strparams("NULL");
			}
			msg.add_intparams(i);
			CServiceActivity::GetSingleton().SendActivityEvent2Scene(i, &msg);
		}
	}

	// 保存上周的百花仙子排行榜
	mLastWeekCharmInc = mWeekCharmIncOutdated;
	mLastWeekCharmInc.SaveRankDataToDB( RANKTYPE_CAMPCHARM_LASTWEEK   , true );

	mWeekCharmIncInTime.ClearData();
	mWeekCharmIncOutdated.ClearData();
	mWeekCharmIncInTime.SaveRankDataToDB( RANKTYPE_WORLD_WEEKINCCHARM   , false );
	mWeekCharmIncOutdated.SaveRankDataToDB( RANKTYPE_WORLD_WEEKINCCHARM   , true );
	return 0;
}




bool CRankModule::IsLaunched() 
{	
	return (mLaunchFlag == LOADOK); 
}



// 从数据库载入当前等级排名(历史表)
int CRankModule::LoadCurrentPlayerRankFromDB( )
{
	LOG_NOTICE( "rank", "[%s] begin ", __FUNCTION__ );
	for (int i = 1; i <= TOTAL_RANK_NUM; ++i ) // 分排行榜加载
	{
		for (int j = 1; j <= MAX_COUNTRY+1; ++j ) // 每个排行榜再分排行榜国家加载
		{
			char sql[10240] = {0};
#ifdef _SERVER_214_
			sprintf( sql, "SELECT role_id, role_name, rank_value, role_country, order_title, metier_id, "
				" rank_type, order_change, coutinue_onboard, coutinue_first, order_id, rank_country, value2 "
				" FROM UMS_RANK_PLAYER_OUTDATED_214 "
				" WHERE rank_date = ( SELECT MAX(rank_date) FROM UMS_RANK_PLAYER_OUTDATED_214) "
				" AND rank_type =%d AND rank_country = %d ORDER BY rank_value desc;", i, j ) ;
#else
			sprintf( sql, "SELECT role_id, role_name, rank_value, role_country, order_title, metier_id, "
				" rank_type, order_change, coutinue_onboard, coutinue_first, order_id, rank_country, value2 "
				" FROM UMS_RANK_PLAYER_OUTDATED"
				" WHERE rank_date = ( SELECT MAX(rank_date) FROM UMS_RANK_PLAYER_OUTDATED ) "
				" AND rank_type =%d AND rank_country = %d ORDER BY rank_value desc;", i, j ) ;
#endif
			if ( CGateLogic::GetSingleton().ExecuteSql( em_dbsession_query_rank_player_outdated, 0, 0, 
				i*10+j, SQL_NEED_CALLBACK, SELECT, sql) == false )
			{
				LOG_ERROR( "rank", "[%s:%d] exec sql %s failed", __FUNCTION__, __LINE__, sql );
				continue;
			}
		}
	}

	LOG_NOTICE( "rank", "[%s] finish ", __FUNCTION__ );
	return 0;
}


// 从数据库载入实时等级排名
int CRankModule::LoadInTimeLevelRankFromDB( )
{
	LOG_NOTICE( "rank", "[%s] begin ", __FUNCTION__ );
	
	for (int i = 1; i <= TOTAL_RANK_NUM; ++i ) // 按排行榜分表
	{
		for (int j = 1; j <= MAX_COUNTRY+1; ++j ) // 每个排行榜再分排行榜国家加载
		{
			char sql[10240] = {0};
	#ifdef _SERVER_214_
			sprintf( sql, "SELECT role_id, role_name, rank_value, role_country, order_title, metier_id, "
					" rank_type, order_id, rank_country, value2 FROM UMS_RANK_PLAYER_214 WHERE rank_type=%d AND rank_country=%d", i, j) ;
	#else
			sprintf( sql, "SELECT role_id, role_name, rank_value, role_country, order_title, metier_id, "
					" rank_type, order_id, rank_country, value2 FROM UMS_RANK_PLAYER WHERE rank_type=%d AND rank_country=%d", i, j) ;
	#endif
			if ( CGateLogic::GetSingleton().ExecuteSql( em_dbsession_query_rank_player, 0, 0, 
				i*20+j, SQL_NEED_CALLBACK, SELECT, sql) == false )
			{
				LOG_ERROR( "rank", "[%s:%d] exec sql %s failed", __FUNCTION__, __LINE__, sql );
				continue;
			}
		}
	}

	LOG_NOTICE( "rank", "[%s] finish ", __FUNCTION__ );
	return 0;
}

// 把实时等级排行写入数据库
int CRankModule::SaveInTimePlayerRankToDB( )
{
	LOG_NOTICE( "rank", "[%s] begin ", __FUNCTION__ );

	mLevelRankInTime.SaveRankDataToDB( RANKTYPE_LEVEL_ALL, false );                 
	mMoneyRankInTime[FREE_MONEY].SaveRankDataToDB( RANKTYPE_MONEY_NOBIND, false );     
	mMoneyRankInTime[ALL_MONEY].SaveRankDataToDB( RANKTYPE_MONEY_ALL, false );      
	mKillRankInTime[KILL_PROT_NATION].SaveRankDataToDB( RANKTYPE_KILL_PROT_NATION, false );
	mKillRankInTime[KILL_OUT].SaveRankDataToDB( RANKTYPE_KILL_OUT, false );        
	mKillRankInTime[KILL_TOTAL].SaveRankDataToDB( RANKTYPE_KILL_TOTAL, false );      
	mHonorRankInTime.SaveRankDataToDB( RANKTYPE_HONOR_ALL, false );                 
	mDieNumRankInTime.SaveRankDataToDB( RANKTYPE_DIE, false );                
	mEndTaskRankInTime.SaveRankDataToDB( RANKTYPE_END_TASK, false );               
	mOnLineRankInTime.SaveRankDataToDB( RANKTYPE_ONLINE_TIME, false );     
	mPantaoRankInTime.SaveRankDataToDB( RANKTYPE_PANTAO_MARK, false );     
	mLonggongRankInTime.SaveRankDataToDB( RANKTYPE_LONGGONG_MARK, false );     
	mOriginReliveRankInTime.SaveRankDataToDB( RANKTYPE_ORIGINRELIVETIMES	, false );     
	mKillSameNationRankInTime.SaveRankDataToDB( RANKTYPE_KILLSAMENATIONNUM	, false );     
	mKillOgreRankInTime.SaveRankDataToDB( RANKTYPE_KILLOGRENUM			, false );     
	mUpFailRankInTime.SaveRankDataToDB( RANKTYPE_UPFAILNUM			, false );     
	mDayKillRankInTime[KILL_PROT_NATION].SaveRankDataToDB( RANKTYPE_KILL_PROT_NATION, false );
	mDayKillRankInTime[KILL_OUT].SaveRankDataToDB( RANKTYPE_KILL_OUT, false );        
	mDayKillRankInTime[KILL_TOTAL].SaveRankDataToDB( RANKTYPE_KILL_TOTAL, false );      
	
	mDayHonorRankInTime.SaveRankDataToDB( RANKTYPE_HONOR_ALL_TODAY, false );      
	mDayantaRankInTime.SaveRankDataToDB( RANKTYPE_DAYANTA		, false );      
	mLianhuadongRankInTime.SaveRankDataToDB( RANKTYPE_LIANHUADONG	, false );      
	mHuangfengdongRankInTime.SaveRankDataToDB( RANKTYPE_HUANGFENGDONG	, false );      
	mAchiveRankInTime.SaveRankDataToDB( RANKTYPE_ACHIVE_POINT	, false );      
	mFavorRankInTime.SaveRankDataToDB( RANKTYPE_FAVOR_POINT	, false );      
	mBadRankInTime.SaveRankDataToDB( RANKTYPE_BAD_POINT		, false );   
	mCharmRankInTime.SaveRankDataToDB( RANKTYPE_CHARM_VALUE		, false );   
	mFlyDungRankInTime.SaveRankDataToDB( RANKTYPE_FLY_FB_MARK	, false );      
	mZhanGongInTime.SaveRankDataToDB( RANKTYPE_ZHANGONG   , false );
	mZhiGuoInTime.SaveRankDataToDB( RANKTYPE_ZHIGUO   , false );
	mTongShuaiInTime.SaveRankDataToDB( RANKTYPE_TONGSHUAI   , false );
	mWisAllInTime.SaveRankDataToDB( RANKTYPE_WIS   , false );
	mYunxiaoInTime.SaveRankDataToDB( RANKTYPE_YUNXIAO   , false );
	mWeekCharmIncInTime.SaveRankDataToDB( RANKTYPE_WORLD_WEEKINCCHARM   , false );
	mShideInTime.SaveRankDataToDB( RANKTYPE_SHIDE			  , false );
	mXianYuanInTime.SaveRankDataToDB( RANKTYPE_XIANYUANMARK	  , false );
	mChuShiInTime.SaveRankDataToDB( RANKTYPE_CHUSHITU		  , false );
	mYunLvInTime.SaveRankDataToDB( RANKTYPE_YUNXIAOTALEVEL		, false );
	mWuDidongInTime.SaveRankDataToDB( RANKTYPE_WUDIDONG   , false );
	mDaLuanDouInTime.SaveRankDataToDB( RANKTYPE_LASTONE ,  false );
	mDaLuanDouTotalInTime.SaveRankDataToDB( RANKTYPE_TOTAL_LASTONE ,  false );
	mGlobalWarInTime.SaveRankDataToDB( RANKTYPE_GLOBALWAR ,  false );
	m_SpiritInTime.SaveRankDataToDB(RANKTYPE_SPIRIT ,  false);

	LOG_NOTICE( "rank", "[%s] finish ", __FUNCTION__ );
	return 0;
}

void CRankModule::OnNewDayCome()
{
	LOG_NOTICE( "rank", "[%s] begin ", __FUNCTION__ );
	DailyUpdate();
	LOG_NOTICE( "rank", "[%s] finish ", __FUNCTION__ );
}

void CRankModule::OnNewWeekCome()
{
	//LOG_NOTICE( "rank", "[%s] begin ", __FUNCTION__ );
	//WeeklyUpdate();
	//LOG_NOTICE( "rank", "[%s] finish ", __FUNCTION__ );
}


// 从榜中删除指定角色
// 参数：角色ID，排行榜类型，0表示全部
int CRankModule::RemovePlayerFromRank( int nCharID, int nRankType )
{
	LOG_NOTICE( "rank", "[%s] begin ", __FUNCTION__ );

	if ( nRankType == 0 || nRankType == RANKTYPE_LEVEL_ALL || nRankType == RANKTYPE_LEVEL_MYNATION ) // 清等级榜
	{
		mLevelRankInTime.RemoveFromRank( nCharID );	// 实时等级排行( 计算用 )
		mLevelRankOutdated.RemoveFromRank( nCharID );
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_MONEY_NOBIND ) // 清等非绑定金
	{
		mMoneyRankInTime[FREE_MONEY].RemoveFromRank( nCharID );	// 实时非绑定金钱排行( 计算用 )
		mMoneyRankOutdated[FREE_MONEY].RemoveFromRank( nCharID );	
	}
	
	if ( nRankType == 0 || nRankType == RANKTYPE_MONEY_ALL ) // 清等非绑定金
	{
		mMoneyRankInTime[ALL_MONEY].RemoveFromRank( nCharID );	// 实时全部金钱排行( 计算用 )
		mMoneyRankOutdated[ALL_MONEY].RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_KILL_PROT_NATION ) // 清等护卫
	{
		mKillRankInTime[KILL_PROT_NATION].RemoveFromRank( nCharID );	
		mKillRankOutdated[KILL_PROT_NATION].RemoveFromRank( nCharID );
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_KILL_OUT )
	{
		mKillRankInTime[KILL_OUT].RemoveFromRank( nCharID );	
		mKillRankOutdated[KILL_OUT].RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_KILL_TOTAL )
	{
		mKillRankInTime[KILL_TOTAL].RemoveFromRank( nCharID );	
		mKillRankOutdated[KILL_TOTAL].RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_HONOR_ALL )
	{
		mHonorRankInTime.RemoveFromRank( nCharID );	
		mHonorRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_HONOR_ALL || nRankType == RANKTYPE_HONER_MYNATION )
	{
		mHonorRankInTime.RemoveFromRank( nCharID );	
		mHonorRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_DIE )
	{
		mDieNumRankInTime.RemoveFromRank( nCharID );	
		mDieNumRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_END_TASK )
	{
		mEndTaskRankInTime.RemoveFromRank( nCharID );	
		mEndTaskRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_ONLINE_TIME )
	{
		mOnLineRankInTime.RemoveFromRank( nCharID );	
		mOnLineRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_PANTAO_MARK )
	{
		mPantaoRankInTime.RemoveFromRank( nCharID );	
		mPantaoRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_LONGGONG_MARK )
	{
		mLonggongRankInTime.RemoveFromRank( nCharID );	
		mLonggongRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_ORIGINRELIVETIMES )
	{
		mOriginReliveRankInTime.RemoveFromRank( nCharID );	
		mOriginReliveRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_KILLSAMENATIONNUM )
	{
		mKillSameNationRankInTime.RemoveFromRank( nCharID );	
		mKillSameNationRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_KILLOGRENUM )
	{
		mKillOgreRankInTime.RemoveFromRank( nCharID );	
		mKillOgreRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_UPFAILNUM )
	{
		mUpFailRankInTime.RemoveFromRank( nCharID );	
		mUpFailRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_KILL_PROT_NATION_TODAY ) // 护卫
	{
		mDayKillRankInTime[KILL_PROT_NATION].RemoveFromRank( nCharID );	
		mDayKillRankOutdated[KILL_PROT_NATION].RemoveFromRank( nCharID );
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_KILL_OUT_TODAY )
	{
		mDayKillRankInTime[KILL_OUT].RemoveFromRank( nCharID );	
		mDayKillRankOutdated[KILL_OUT].RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_KILL_TOTAL_TODAY )
	{
		mDayKillRankInTime[KILL_TOTAL].RemoveFromRank( nCharID );	
		mDayKillRankOutdated[KILL_TOTAL].RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_HONOR_ALL_TODAY || nRankType == RANKTYPE_HONER_MYNATION_TODAY )
	{
		mDayHonorRankInTime.RemoveFromRank( nCharID );	
		mDayHonorRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_DAYANTA )
	{
		mDayantaRankInTime.RemoveFromRank( nCharID );	
		mDayantaRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_LIANHUADONG )
	{
		mLianhuadongRankInTime.RemoveFromRank( nCharID );	
		mLianhuadongRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_HUANGFENGDONG )
	{
		mHuangfengdongRankInTime.RemoveFromRank( nCharID );	
		mHuangfengdongRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_ACHIVE_POINT )
	{
		mAchiveRankInTime.RemoveFromRank( nCharID );	
		mAchiveRankOutdated.RemoveFromRank( nCharID );	
	}

	// TODO 好感度猥琐度排行榜暂无
	// TODO 团体积分暂时不处理

	if ( nRankType == 0 || nRankType == RANKTYPE_CHARM_VALUE )
	{
		mCharmRankInTime.RemoveFromRank( nCharID );	
		mCharmRankOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_FLY_FB_MARK )
	{
		mFlyDungRankOutdated.RemoveFromRank( nCharID );	
		mFlyDungRankInTime.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_ZHANGONG )
	{
		mZhanGongInTime.RemoveFromRank( nCharID );	
		mZhanGongOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_ZHIGUO )
	{
		mZhiGuoInTime.RemoveFromRank( nCharID );	
		mZhiGuoOutDated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_TONGSHUAI )
	{
		mTongShuaiInTime.RemoveFromRank( nCharID );	
		mTongShuaiOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_WIS )
	{
		mWisAllInTime.RemoveFromRank( nCharID );	
		mWisAllOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_YUNXIAO )
	{
		mYunxiaoInTime.RemoveFromRank( nCharID );	
		mYunxiaoOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_WORLD_WEEKINCCHARM || nRankType == RANKTYPE_CAMP_WEEKINCCHARM )
	{
		mWeekCharmIncInTime.RemoveFromRank( nCharID );	
		mWeekCharmIncOutdated.RemoveFromRank( nCharID );	
	}

	// 新排行榜
	if ( nRankType == 0 || nRankType == RANKTYPE_SHIDE )
	{
		mShideInTime.RemoveFromRank( nCharID );	
		mShideOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_XIANYUANMARK )
	{
		mXianYuanInTime.RemoveFromRank( nCharID );	
		mXianYuanOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_CHUSHITU )
	{
		mChuShiInTime.RemoveFromRank( nCharID );	
		mChuShiOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_YUNXIAOTALEVEL )
	{
		mYunLvInTime.RemoveFromRank( nCharID );	
		mYunLvOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_WUDIDONG )
	{
		mWuDidongInTime.RemoveFromRank( nCharID );	
		mWuDidongOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_CAMPCHARM_LASTWEEK )
	{
		mLastWeekCharmInc.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_LASTONE )
	{
		mDaLuanDouInTime.RemoveFromRank( nCharID );	
		mDaLuanDouOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_TOTAL_LASTONE )
	{
		mDaLuanDouTotalInTime.RemoveFromRank( nCharID );	
		mDaLuanDouTotalOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_GLOBALWAR )
	{
		mGlobalWarInTime.RemoveFromRank( nCharID );	
		mGlobalWarOutdated.RemoveFromRank( nCharID );	
	}

	if ( nRankType == 0 || nRankType == RANKTYPE_SPIRIT )
	{
		m_SpiritInTime.RemoveFromRank( nCharID );	
		m_SpiritOutdated.RemoveFromRank( nCharID );	
	}

	LOG_NOTICE( "rank", "[%s] finish ", __FUNCTION__ );
	return 0;
}
