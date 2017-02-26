#include "corpstype.h"
#include "familytype.h"
#include "object.h"	   
#include "family_manager.h"
#include "familydbinfo_pb.hxx.pb.h"	 
#include "corpsinfo_pb.hxx.pb.h"
#include "corpsmessage_pb.hxx.pb.h"
#include "entity.h"
#include "gatelogic.h"
#include "errcode.h"	
#include "corpsmanager.h"  
#include "corpservice.h"
#include "nameservice.h"
#include "servermessage_pb.hxx.pb.h"
#include "gateobj_manager.h"
#include "familyservice.h"
#include "chatmessage_pb.hxx.pb.h"
#include "chattype.h"
#include "core_service.h"
#include "repetionmodule.h"
#include "tinyxml.h"
#include "world.h"
#include "log_tools.h"
#include "template.h"
#include "repetionmessage_pb.hxx.pb.h"
#include "gatelogic.h"
#include "gateobj_manager_imp.h"
#include "taskmodule.h"

#include <google/protobuf/descriptor.h>  

typedef ::google::protobuf::Message Message;
using namespace GateServer;	

//-------------------------------------------------------------------------
#include "bidmgr.h"
#include "battlemgr.h"

template<> CCorpservice *CSingleton< CCorpservice >::spSingleton  = NULL;
CCorpsManager *gpCorpsManager = NULL;
CBidMgr *gpCBidMgr = NULL;
CBattleMgr *gpCBattleMgr = NULL;
// ***********************************************************
//  Function:		OnLaunchServer
//  Description:	登录
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnLaunchServer( )
{  
	CCorpsManager::mShmPtr = mShmPtr;
	gpCorpsManager = new CCorpsManager;	
	LOG_DEBUG("corps","gpCorpsManager != NULL %d", gpCorpsManager);
	LK_ASSERT( gpCorpsManager != NULL, return );


	CBidMgr::mShmPtr = mShmPtr;
	gpCBidMgr = new CBidMgr;	
	LOG_DEBUG("corps","gpCBidMgr != NULL%d", gpCBidMgr);
	LK_ASSERT( gpCBidMgr != NULL, return );

	CBattleMgr::mShmPtr = mShmPtr;
	gpCBattleMgr = new CBattleMgr;	  
	LOG_DEBUG("corps","gpCBattleMgr != NULL%d", gpCBattleMgr);
	LK_ASSERT( gpCBattleMgr != NULL, return );

	if ( mShmPtr->GetInitMode() == SHM_INIT)
	{
		LOG_DEBUG("corps", "mShmPtr->GetInitMode() == SHM_INIT");
		m_bMigratieMode = false;
		//开始加载竞价数据
		StartLoadAllFromDB(TAB_BID);		
	}
	else
	{
		LOG_DEBUG("corps", "mShmPtr->GetInitMode() != SHM_INIT");
		CCorpsManager::GetSingleton().UpDateAllCorpsDBInfo( true );
	}

	ReloadCfg();

	// 注册配置加载的回调函数
	const char* pcConfigPath = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"corps/corpsconfig.xml");
	CGateServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CCorpservice::LoadCorpConfigCallback);
}

// ***********************************************************
//  Function:		CCorpservice
//  Description:	构造函数
//  Output:			
//  Date:	06/12/2009
// **********************************************************
CCorpservice::CCorpservice()
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
unsigned int CCorpservice::CountSize()
{
	unsigned int tSize = CCorpsManager::CountSize();
	tSize += CBidMgr::CountSize();
	tSize += CBattleMgr::CountSize();

	tSize += sizeof( CCorpservice );
	return tSize;
}

// ***********************************************************
//  Function:		Initialize
//  Description:	初始化函数
//  Output:			
//  Date:	06/12/2009
// **********************************************************
int CCorpservice::Initialize( )
{
	mUpdateTimer = CWTimer( CORPSTIMER_UPDATE_TIMELENGTH );
	mDisbandTimer = CWTimer( CORPSTIMER_DISBAND_TIMELENGTH );
	mBattleNoticeTimer = CWTimer( CORPSTIMER_DISBAND_TIMELENGTH/2 );
	m_CheckNpcStateTimer = CWTimer( CORPSTIMER_CHECK_NPC_STATE );
	mIsLaunched = false;
	return 0;
}

// ***********************************************************
//  Function:		Resume
//  Description:	
//  Output:			
//  Date:	06/12/2009
// **********************************************************
int CCorpservice::Resume( )
{
	return 0;
}
//  第二天到来
void CCorpservice::OnNewDayCome()
{
	CGateObjManagerImp* pGateObjManagerImp = CGateObjManager::GetSingletonPtr()->mGateObjManagerImp;
	PUBLISHLIST mCorpsList = CTaskModule::GetSingleton().GetCorpsLimit()->mTaskList;
	for( CGateObjManagerImp::ObjMng_Corps::iterator tCorps = pGateObjManagerImp->mCorpsPool.begin( ); 
		tCorps != pGateObjManagerImp->mCorpsPool.end( ); 
		++tCorps 
		)
	{
		// 处理每个军团
		CCorps * pCorps = (CCorps*) (&(*tCorps));
		pCorps->OnNewDayCome();	
		pCorps->GetCorpsPublishInfo()->LoadPublishList( &mCorpsList );
		pCorps->SetSaveStatus( true );
	}
}
// 周计次
void CCorpservice::OnNewWeekCome()
{
	CGateObjManagerImp* pGateObjManagerImp = CGateObjManager::GetSingletonPtr()->mGateObjManagerImp;
	PUBLISHLIST mCorpsList = CTaskModule::GetSingleton().GetCorpsLimit()->mTaskList;
	for( CGateObjManagerImp::ObjMng_Corps::iterator tCorps = pGateObjManagerImp->mCorpsPool.begin( ); 
		tCorps != pGateObjManagerImp->mCorpsPool.end( ); 
		++tCorps 
		)
	{
		// 处理每个军团
		CCorps * pCorps = (CCorps*) (&(*tCorps));
		pCorps->OnNewWeekCome();
		pCorps->GetCorpsPublishInfo()->LoadPublishList( &mCorpsList );
		pCorps->SetSaveStatus( true );
	}
}

// ***********************************************************
//  Function:		OnTimer
//  Description:	计时器
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	if (!mIsLaunched)
	{
		LOG_ERROR("corps", "[%d:%s]OnTimer when Is Not Launched !", __LINE__, __FILE__);
		return;
	}

	if ( mDisbandTimer.Elapse( vTickOffset ) == true )
	{  
		CCorpsManager::GetSingleton().DeleteCorpsOnDisbandTime();
		CCorpsManager::GetSingleton().UpDateAllUnionDBInfo();
	}

	if ( mUpdateTimer.Elapse( vTickOffset ) == true )
	{			
		CCorpsManager::GetSingleton().UpDateAllCorpsDBInfo();
		CBidMgr::GetSingleton().SynDataToDB();
		CBattleMgr::GetSingleton().SynDataToDB();
	}	

	if ( mBattleNoticeTimer.Elapse( vTickOffset ) == true )
	{
		//定期发送参战通知
		CBattleMgr::GetSingleton().SendJoinNotice();
	}
	if ( m_CheckNpcStateTimer.Elapse( vTickOffset ) == true )
	{
		//定期检查竞价状态
		CBidMgr::GetSingleton().OnTimerCheckState();			
		//定期检查是否开战
		CBattleMgr::GetSingleton().OnTimerCheckState();
	}
}

// ***********************************************************
//  Function:		OnExitServer
//  Description:	退出
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnExitServer( )
{
	// 军团和军团联盟的数据信息	
	CCorpsManager::GetSingleton().UpDateAllCorpsDBInfo();
	CCorpsManager::GetSingleton().UpDateAllUnionDBInfo();

	// 竞价和战争信息
	CBidMgr::GetSingleton().SynDataToDB();
	CBattleMgr::GetSingleton().SynDataToDB();
} 

void CCorpservice::OnSessionLoadCorpsBid( CMessage* pMessage, int nStart, int nTotalCount )
{
	CBidMgr::GetSingleton().OnSessionLoadCorpsBid(pMessage, nStart, nTotalCount);
	return;
}
void CCorpservice::OnSessionLoadNpcStatus( CMessage* pMessage, int nStart, int nTotalCount )
{
	CBidMgr::GetSingleton().OnSessionLoadNpcStatus(pMessage, nStart, nTotalCount);
	return;
}

void CCorpservice::OnSessionLoadCorpsBattle( CMessage* pMessage, int nStart, int nTotalCount )
{
	CBattleMgr::GetSingleton().OnSessionLoadFromDB(pMessage, nStart, nTotalCount);
	return;
}

// ***********************************************************
//  Function:		InitializeCorpsConfig
//  Description:	初始化军团信息列表
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpservice::InitializeCorpsConfig()
{
	TiXmlDocument tDoc;
	//const char *pFileName = "../config/data/corps/corpsconfig.xml";
	const char *pFileName = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"corps/corpsconfig.xml");
	if ( !tDoc.LoadFile( pFileName ) )
	{			
		return -1;
	}

	TiXmlElement *tpElement = tDoc.FirstChildElement("CorpsConfig");
	if ( tpElement == NULL )
	{
		return -1;
	}

	TiXmlElement *tpLevelInfo = tpElement->FirstChildElement( "LevelConfig" );
	if ( tpLevelInfo == NULL )
	{
		return -1;
	}
	TiXmlElement *tpInfo = tpLevelInfo->FirstChildElement( "ConfigInfo" );
	if ( tpInfo == NULL )
	{
		return -1;
	}

	CCorpsManager::GetSingleton().GetCorpsConfig()->Initialize();
	for ( ; tpInfo != NULL; tpInfo = tpInfo->NextSiblingElement( ) )
	{
		CCorpsConfigInfo tCorpsconfig;
		tCorpsconfig.mGloryLimit			= atoi( tpInfo->Attribute( "GloryLimit" ) );
		tCorpsconfig.mLevel					= atoi( tpInfo->Attribute( "Level" ) );
		tCorpsconfig.mFamilyNumLimit		= atoi( tpInfo->Attribute( "FamilyLimit" ) );
		tCorpsconfig.mGeneralNumLimit		= atoi( tpInfo->Attribute( "GeneralNumLimit" ) );
		tCorpsconfig.mCounSellorNumLimit	= atoi( tpInfo->Attribute( "CounSellorNumLimit" ) );
		tCorpsconfig.mPvPStarScore			= atoi( tpInfo->Attribute( "PvPStarScore" ) );
		tCorpsconfig.mGloryLevelupNeeded	= atoi( tpInfo->Attribute( "GloryLevelupNeeded" ) );
		tCorpsconfig.mMoneyLevelupNeeded	= atoi( tpInfo->Attribute( "MoneyLevelUpNeeded" ) );
		tCorpsconfig.mBossRefineValueLimit	= atoi( tpInfo->Attribute( "BossRefineValueLimit" ) );
		tCorpsconfig.mBossCDLimit			= atoi( tpInfo->Attribute( "BossCDLimit")) * 1 * 60 ;
		CCorpsManager::GetSingleton().GetCorpsConfig()->InsertCorpsConfig( tCorpsconfig );
	}

	TiXmlElement *tpPropertyInfo = tpElement->FirstChildElement( "PropertyConfig" );
	if ( tpPropertyInfo == NULL )
	{
		return -1;
	}

	tpInfo = tpPropertyInfo->FirstChildElement( "PropertyInfo" );
	if ( tpInfo == NULL )
	{				
		return -1;
	}
	CCorpsPropertyConfigInfo tPropertyInfo;
	tPropertyInfo.mContributePerGold = atoi( tpInfo->Attribute( "ContributePerGold" ) );
	tPropertyInfo.mScorePerFight	 = atoi( tpInfo->Attribute( "ScorePerFight" ) );
	tPropertyInfo.mGloryPerFight	 = atoi( tpInfo->Attribute( "GloryPerFight" ) );
	tPropertyInfo.mExpPerGlory		 = atoi( tpInfo->Attribute( "ExpPerGlory" ) );
	tPropertyInfo.mExploitPerGlory	 = atoi( tpInfo->Attribute( "ExploitPerGlory" ) );
	tPropertyInfo.mNpcGlory			 = atoi( tpInfo->Attribute( "NpcGlory" ) );	
	tPropertyInfo.mNpcMoney			 = atoi( tpInfo->Attribute( "NpcMoney" ) );	
	tPropertyInfo.mChallangeMoney	 = atoi( tpInfo->Attribute( "ChallangeMoney" ) );
	tPropertyInfo.mChallangeTimesOfDay = atoi( tpInfo->Attribute( "ChallangeTimesOfDay" ) );

	CCorpsManager::GetSingleton().GetCorpsConfig()->SetCorpsPropertyConfig( tPropertyInfo );

	return SUCCESS;
}

// ***********************************************************
//  Function:		LoadFamilyFromDB
//  Description:	从数据库家族军团信息
//  Output:			
//  Date:	06/15/2009
// **********************************************************
void CCorpservice::LoadCorpsFromDB( int nStart, int nTotalCount )
{
	const char* pSqlstr = "select \
						  corps_id, \
						  corps_name, \
						  create_role_id, \
						  create_role_name, \
						  corps_level, \
						  corps_money, \
						  family_info, \
						  country_id, \
						  corps_status, \
						  disband_time, \
						  corps_glory, \
						  war_winnum, \
						  war_lostnum, \
						  war_score, \
						  war_starlevel, \
						  war_bindnpcid, \
						  war_bindmoney, \
						  war_campid, \
						  war_battlecity, \
						  corps_notice, \
						  war_welfaregettime, \
						  corps_roommoney, \
						  war_battleinfo, \
						  corps_color, \
						  corps_shopinfo, \
						  corps_forbidtime, \
						  corps_refinevalue,\
						  corps_publish, \ 
						  corpsboss_score, \
						  corpscollect_active from UMS_CORPS limit %d,%d ";

	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_loadcorps, nStart, nTotalCount, 3, SQL_NEED_CALLBACK, SELECT, 
		pSqlstr, nStart, COUNT_LOADCORPS );
}

// ***********************************************************
//  Function:		OnSessionLoadCorps
//  Description:	从数据库读取军团信息
//  Output:			
//  Date:	06/15/2009
// **********************************************************
void CCorpservice::OnSessionLoadCorps( CMessage* pMessage, int nStart, int nTotalCount )
{
	LK_ASSERT( pMessage != NULL, return )
		CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	if ( tpMsg == NULL )
	{
		LOG_ERROR( "corps","[%s,%d] load corps error, database error! ",__FUNCTION__, __LINE__ );
		printf( "[%s,%d] load corps error, database error! ",__FUNCTION__, __LINE__ );
		exit( 0 );
	}
	int tcolnum = tpMsg->colcount();
	int trownum = tpMsg->rowcount();	
	if ( tcolnum != 30 )
	{
		LOG_ERROR( "corps","Load Corps failed!" );
		printf( "Load corps failed!" );
		exit( 0 );
	}
	if ( tpMsg->resultcode() == 1 )
	{
		for ( int i = 0; i < trownum; ++i )
		{	
			//加载军团信息
			CCorps *tpCorps = ( CCorps * )CGateObjManager::GetSingleton().CreateObject( OBJTYPE_CORPS );
			LK_ASSERT( tpCorps != NULL, continue )
				tpCorps->SetCorpsID( atoi( tpMsg->fieldvalue( i*tcolnum + 0 ).c_str() ) );
			tpCorps->SetCorpsName( tpMsg->fieldvalue( i*tcolnum + 1 ).c_str() );
			tpCorps->SetRooterID( atoi( tpMsg->fieldvalue( i*tcolnum + 2 ).c_str() ) );
			tpCorps->SetRooterName( tpMsg->fieldvalue( i*tcolnum + 3 ).c_str() );
			tpCorps->SetCorpsLevel( atoi( tpMsg->fieldvalue( i*tcolnum + 4 ).c_str() ) );		
			tpCorps->SetCorpsMoney( atoi( tpMsg->fieldvalue( i*tcolnum + 5 ).c_str() ) );

			PBCorpsFamilyInfoList tCorpsFamilyInfo;
			bool tPaseRet = tCorpsFamilyInfo.ParseFromArray( tpMsg->fieldvalue( i*tcolnum + 6 ).c_str(),
				tpMsg->fieldvalue( i*tcolnum + 6 ).length() );			
			if ( tPaseRet == true )
			{
				tpCorps->GetCorpsFamilyInfoFromPB( tCorpsFamilyInfo );
			}	
			tpCorps->SetCorpsCountryID( atoi( tpMsg->fieldvalue( i*tcolnum + 7 ).c_str()  ) );
			tpCorps->SetCorpsStatus( atoi( tpMsg->fieldvalue( i*tcolnum + 8 ).c_str()  ) );
			tpCorps->SetCorpsDisbandTime( atoi( tpMsg->fieldvalue( i*tcolnum + 9 ).c_str()  ) );		

			tpCorps->SetCorpsGlory( atoi( tpMsg->fieldvalue( i*tcolnum + 10 ).c_str() ) );
			tpCorps->SetVictoryNum( atoi( tpMsg->fieldvalue( i*tcolnum + 11 ).c_str() ) );
			tpCorps->SetLoserTimes( atoi( tpMsg->fieldvalue( i*tcolnum + 12 ).c_str() ) );			
			tpCorps->SetPVPScore( atoi( tpMsg->fieldvalue( i*tcolnum + 13 ).c_str() ) );
			tpCorps->SetPVPStarLevel( atoi( tpMsg->fieldvalue( i*tcolnum + 14 ).c_str() ) );
			tpCorps->SetBidNpcID( atoi( tpMsg->fieldvalue( i*tcolnum + 15 ).c_str() ) );
			tpCorps->SetBidMoney( atoi( tpMsg->fieldvalue( i*tcolnum + 16 ).c_str() ) );			
			tpCorps->SetCorpsCamp( atoi( tpMsg->fieldvalue( i*tcolnum + 17 ).c_str() ) );
			
			int nTmpBattleCity = ( atoi( tpMsg->fieldvalue( i*tcolnum + 18 ).c_str() ) );

			tpCorps->SetBulletin( tpMsg->fieldvalue( i*tcolnum + 19 ).c_str() );
			// tpCorps->SetCorpsWelfareGetTime( atoi( tpMsg->fieldvalue( i*tcolnum + 20 ).c_str() ) );
			tpCorps->SetRoomMoney( atoi( tpMsg->fieldvalue( i*tcolnum + 21 ).c_str() ) );

			// 军盟ID初始化为-1,如果有军盟在加载军盟数据的时候会被重新设置
			// DengLichuan@2010-06-30
			tpCorps->SetUnionID(INVALID_OBJ_ID);

			// battle info

			if (m_bMigratieMode)
			{
				//第一次切换做迁移数据时使用
				PBCorpsBattleList tBattleInfo;
				if ( tBattleInfo.ParseFromArray( tpMsg->fieldvalue( i*tcolnum + 22 ).c_str(), 
					tpMsg->fieldvalue( i*tcolnum + 22 ).length() ) )
				{
					tpCorps->SetFromPB( &tBattleInfo );
				}
			}

			// 军团颜色
			tpCorps->SetColor( atoi( tpMsg->fieldvalue( i*tcolnum + 23 ).c_str() ) );

			// 商店信息
			PBLeagueShopsListInfo tLeagueShopInfo;
			const char *pShopField = tpMsg->fieldvalue( i*tcolnum + 24 ).c_str();
			int pShopFieldLen = tpMsg->fieldvalue( i*tcolnum + 24 ).length();

			if ( tLeagueShopInfo.ParseFromArray( pShopField, pShopFieldLen) == true )
			{
				tpCorps->GetCorpsShop(  )->SetShopListInfo( tLeagueShopInfo );
			}
			else
			{
				LOG_ERROR( "Corps", "load Corps ( name: %s, ID:%d ) error, shop info wrong",tpCorps->GetCorpsNameShow(), tpCorps->GetCorpsID() );	
			}	

			tpCorps->SetForbidTime( atoi( tpMsg->fieldvalue( i*tcolnum + 25 ).c_str() ) );
			tpCorps->SetBossRefineValue( atoi( tpMsg->fieldvalue( i*tcolnum + 26 ).c_str() ) );
			
			// 军团发布信息
			PBCorpsPublishInfo tCorpsPublishInfo;
			bool tPubRet = tCorpsPublishInfo.ParseFromArray( tpMsg->fieldvalue( i*tcolnum + 27 ).c_str(),
				tpMsg->fieldvalue( i*tcolnum + 27 ).length() );			
			if ( tPubRet == true )
			{
				tpCorps->GetCorpsPublishInfoFromPB( tCorpsPublishInfo );
			}	
			tpCorps->SetCorpsBossScore( atoi( tpMsg->fieldvalue( i*tcolnum + 28 ).c_str() ) );
			tpCorps->SetCorpsCollectActive( atoi( tpMsg->fieldvalue( i*tcolnum + 29 ).c_str() ) );

			tpCorps->SetSaveStatus( CCorps::CORPS_INFONOCHANGE );

			int tRet = CCorpsManager::GetSingleton().InsertCoprsInfo( tpCorps );
			if ( tRet == -1 )
			{
				CGateObjManager::GetSingleton().DestroyObject( tpCorps->get_id() );
			}

			// 回滚开房间得金钱 
			if ( tpCorps->GetRoomMoney() > 0 )
			{
				tpCorps->ContributeCorpsMoney( tpCorps->GetRoomMoney() );
				tpCorps->SetRoomMoney( 0 );
			}

			// 主城信息迁移至NpcStauts
			if (1 == CBidMgr::GetSingleton().m_nDataState)
			{
				tpCorps->SetBattleCity(nTmpBattleCity);
			}
			else
			{
				tpCorps->m_nBattleCity = CBidMgr::GetSingleton().GetMainCity(tpCorps->GetCorpsID());
			}
				
			if ( tpCorps->GetBattleCity() != INVALID_MAP_ID )
			{
				CWorldManager::GetSingleton().LoadCorpsFromMap( tpCorps->GetCorpsCountryID( ), tpCorps->GetBattleCity( ), tpCorps->GetCorpsID( ) );
			}
			else
			{
				OnEventCityUpgrade(tpCorps, tpCorps->GetCorpsCountryID());
			}
		}

		LOG_NOTICE("corps", "Load corps data start %d num %d total %d", nStart, trownum, nTotalCount );

		if ( trownum < COUNT_LOADCORPS )
		{
			if ( nTotalCount < nStart + trownum )
			{
				printf( "[%s,%d]load corps error, incorrect number :%d total number:%d", __FUNCTION__, __LINE__, nStart + trownum, nTotalCount );
				LOG_ERROR( "corps","[%s,%d]load corps error, incorrect number :%d total number:%d", __FUNCTION__, __LINE__, nStart + trownum, nTotalCount );
				exit( 0 );				
			}
		}
		else
		{
			if ( trownum + nStart < nTotalCount )
			{
				//继续加载军团数据
				LoadCorpsFromDB( nStart + trownum, nTotalCount );
			}			
		}

		// 加载完成
		if ( trownum + nStart >= nTotalCount )
		{
			if (m_bMigratieMode || 1 == CBidMgr::GetSingleton().m_nDataState)
			{
				//如果迁移了数据，立即向数据库保存一次NPC状态，表示迁移成功
				if (m_bMigratieMode)
				{				
					LOG_NOTICE( "corps","MigratieMode=true SynDataToDB");
				}
				else
				{
					LOG_NOTICE( "corps","Migrate Main City Info, SynDataToDB");
				}
				CBidMgr::GetSingleton().SynDataToDB();
			}

			//printf( "load corps end, total number:%d", nTotalCount );
			LOG_NOTICE( "corps","Load Corps Complete, Total Number:%d", nTotalCount );
			LoadUnionFromDB( 0, COUNT_LOADCORPS );
			CWorldModule::GetSingleton().LoadWorldGlobal( );
			CWorldModule::GetSingleton().LoadWorldWar( ); 
			CWorldModule::GetSingleton().LoadWorldWarChar( );
			CWorldModule::GetSingleton().LoadWorldStatus( ); 
			CWorldModule::GetSingleton().LoadWorld( );			 
		}
	}	
	else
	{
		printf( "[%s,%d] load corps error, database error! ",__FUNCTION__, __LINE__ );
		LOG_ERROR( "corps","[%s,%d] load corps error, database error! ",__FUNCTION__, __LINE__ );
		exit( 0 );
	}			    
}

int CCorpservice::OnSessionCreateUnionDBInfo( CMessage* pMessage, int nHeadID, int nUnionID )
{
	LK_ASSERT( pMessage != NULL, return  -1 );
	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return -1 );	
	CUnion *tpUnion = ( CUnion * )CGateObjManager::GetSingleton().GetObject( nUnionID );
	LK_ASSERT( tpUnion != NULL, return -1 );
	CCorps *tpHead = ( CCorps * )CCorpsManager::GetSingleton().GetCorpsByID( tpUnion->GetCoreID() );
	LK_ASSERT( tpHead != NULL, return -1 );
	if( tpMsg->resultcode() == 1 )
	{
#ifndef _GLOBAL_NAME_
		tpUnion->SetUnionID( tpMsg->latestautoincid() );
#endif

		CCorpsManager::GetSingleton().m_UnionList.insert( CCorpsManager::UnionList::value_type( tpUnion->GetUnionID(), tpUnion->get_id() ) );
		CCorps *tpHeadCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpUnion->GetCoreID() );
		if ( tpHeadCorps != NULL )
		{
			tpHeadCorps->SetUnionID( tpUnion->GetUnionID() );
		}
		tpHeadCorps->ClearTempUnion( );
		for ( CUnion::CorpsVector::iterator it = tpUnion->mCorpsList.begin(); it != tpUnion->mCorpsList.end(); ++it )
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( *it );
			if ( tpCorps != NULL )
			{
				tpCorps->SetUnionID( tpUnion->GetUnionID() );
			}
		}
		LOG_NOTICE( "default", "[ %s : %d ][ %s ]Create Union Success UnionID %d, HeadID %d", __LK_FILE__, __LINE__, 
			__FUNCTION__, nUnionID, nHeadID );
		CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->ScenePlayer( tpHead->GetRooterID() );
		if ( tpPlayer != NULL )
		{
			SendCreateUnionResponse( tpPlayer, tpHead->GetRooterID(), SUCCESS, tpUnion->GetUnionID() );
			SendCreateUnionNotify( tpPlayer->mCountryID, tpHead->GetRooterID(), tpUnion );
		}

	}
	else
	{
		CGateObjManager::GetSingleton().DestroyObject( nUnionID );
	}
	return 0;
}

void CCorpservice::OnSessionLoadUnion( CMessage* pMessage, int nStart, int nCount )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	int tcolnum = tpMsg->colcount();
	int trownum = tpMsg->rowcount();	
	if ( tpMsg->resultcode() == 1 )
	{
		for ( int i = 0; i < trownum; ++i )
		{	
			CUnion *tpUnion = ( CUnion * )CGateObjManager::GetSingleton().CreateObject( OBJTYPE_UNION );
			LK_ASSERT( tpUnion != NULL, continue );
			tpUnion->SetUnionID( atoi( tpMsg->fieldvalue( i*tcolnum + 0 ).c_str() ) );			
			tpUnion->SetCoreID( atoi( tpMsg->fieldvalue( i*tcolnum + 1 ).c_str() ) );
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpUnion->GetCoreID() );
			if ( tpCorps != NULL )
			{
				tpCorps->SetUnionID( tpUnion->GetUnionID() );
			}
			tpUnion->SetUnionName( tpMsg->fieldvalue( i*tcolnum + 2 ).c_str() );
			tpUnion->SetBulletin( tpMsg->fieldvalue( i*tcolnum + 3 ).c_str() );

			// 从数据库加载也不能加载超过MAX_CORPS_UNION-1个， 因为主军团自己不包括在内
			for ( int j = 0; j < MAX_CORPS_UNION-1; j++ )
			{
				int tCorpsID = atoi( tpMsg->fieldvalue( i*tcolnum + 4 + j ).c_str() );
				CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tCorpsID );
				if ( tpCorps != NULL )
				{
					tpCorps->SetUnionID( tpUnion->GetUnionID() );
					tpUnion->mCorpsList.push_back( tCorpsID );
				}				
			}

			CCorpsManager::GetSingleton().m_UnionList.insert( CCorpsManager::UnionList::value_type( tpUnion->GetUnionID(), tpUnion->get_id() ) );
			LOG_NOTICE( "default", "[ %s : %d ][ %s ]Create Union Success UnionID %d HeadID %d", __LK_FILE__, __LINE__, 
				__FUNCTION__, tpUnion->GetUnionID(), tpUnion->GetCoreID() );
		}

		LOG_NOTICE( "corps", "load corps union, return row %d total %d", trownum, nCount );

		if ( trownum >= nCount )
		{
			LoadUnionFromDB( nStart + nCount, nCount );
		}
		else
		{
			LOG_NOTICE( "corps", "load all data from DB OK!");
			mIsLaunched = true;
		}
	}
	else
	{
		printf( "load corps union failed\n" );
		LOG_ERROR( "corps", "[%s:%d] load corps union failed", __FUNCTION__, __LINE__ );
		exit(0);
	}
}

void CCorpservice::LoadUnionFromDB( int nStart, int nCount )
{
	const char* pSqlstr = "select union_id, union_head, union_name, union_notice, corps0, corps1, corps2, corps3, corps4, corps5, \
						  corps6, corps7, corps8 from UMS_UNION limit %d,%d ";
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_load_union, nStart, nCount, 3, SQL_NEED_CALLBACK, SELECT, 
		pSqlstr, nStart, nCount );
}

// ***********************************************************
//  Function:		ReloadCfg
//  Description:	重载配置文件
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CCorpservice::ReloadCfg()
{
	int nRet = InitializeCorpsConfig();
	if( nRet < 0 )
	{
		printf( "read corps config failed\n" );
		LOG_FATAL( "corps", "[%s:%d] read corps config faild ", __FUNCTION__, __LINE__ );		
		exit( 0 );
	}
}

//////////////////////////////////////////////////////////////////////////
/// 开始加载数据库表中的所有数据，先获取总数
/// @param _Type
/// 1 bid
/// 2 battle
/// 3 corps
void CCorpservice::StartLoadAllFromDB(int nType)
{	
	char* pSqlStr = NULL;
	int SessinType;
	switch (nType)
	{
	case TAB_BID:
		pSqlStr = (char*)"select count(*) from UMS_CORPS_BID ";
		SessinType = em_dbsession_getcorpsbidnum;
		break;
	case TAB_NPC:
		pSqlStr = (char*)"select count(*) from UMS_NPC_STATUS";
		SessinType = em_dbsession_getnpcstatusnum;
		break;
	case TAB_BATTLE:
		pSqlStr = (char*)"select count(*) from UMS_CORPS_BATTLE";
		SessinType = em_dbsession_getcorpsbattlenum;
		break;
	case TAB_CORPS:
		pSqlStr = (char*)"select count(*) from UMS_CORPS";
		SessinType = em_dbsession_getcorpsnum;
		break;
	default:
		return;
	}

	LOG_INFO("corps", "Start load corps DB, table type %d", nType);

	CGateLogic::GetSingleton().ExecuteSql( SessinType, 0, 0, 3, SQL_NEED_CALLBACK, SELECT, pSqlStr );	
}

// ***********************************************************
//  Function:		OnSessionGetDataNumFromDB
//  Description:	从数据库获取指定数据表内的数据数量的回应消息
//  Output:			
//  Date:	01/07/2009
// **********************************************************
void CCorpservice::OnSessionGetDataNumFromDB( CMessage *pMessage, int nParam1, int nParam2, int nTableType )
{		
	LK_ASSERT( pMessage != NULL, return )	
		CMessageExecuteSqlResponse3* tpMessage = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	if ( tpMessage != NULL )
	{
		// 获取成功
		if ( tpMessage->resultcode() == 1 )
		{
			if ( tpMessage->fieldvalue_size() < 1 )
			{
				LOG_ERROR( "corps","error occured when get corps number because of no value, Table:%d", nTableType);
				exit( 0 );
				return;
			}

			int tTotalCount = atoi( tpMessage->fieldvalue( 0 ).c_str() );
			LOG_INFO("corps", "Read Type %d From DB, NUM=%d", nTableType, tTotalCount);
			if (tTotalCount < 1)
			{
				if (TAB_NPC == nTableType)
				{
					//此时NPC状态表为空，准备从历史数据中迁移NPC状态
					m_bMigratieMode = true;
				}
			}
			int nstart = 0;	 
			if (TAB_BID == nTableType)
			{
				CBidMgr::GetSingleton().LoadCorpsBidFromDB( nstart, tTotalCount );
			} 
			else if (TAB_NPC == nTableType)
			{
				CBidMgr::GetSingleton().LoadNpcStatusFromDB( nstart, tTotalCount );
			}
			else if (TAB_BATTLE == nTableType)
			{
				CBattleMgr::GetSingleton().LoadFromDB( nstart, tTotalCount );
			}
			else if (TAB_CORPS == nTableType)
			{
				LoadCorpsFromDB( nstart, tTotalCount );
			}
		}
		else
		{
			LOG_ERROR( "corps","error occured when get corps number , database error, Table:%d", nTableType);
			exit( 0 );
		}

	}
	else
	{
		LOG_ERROR( "corps","error occured when get corps number, empty message, Table:%d", nTableType);
		exit( 0 );
	}
}

void CCorpservice::LoadCorpConfigCallback(const char *pcConfigPath)
{
	int nRet = CCorpservice::GetSingleton().InitializeCorpsConfig();
	if( nRet < 0 )
	{
		printf( "read corps config failed\n" );
		LOG_FATAL( "corps", "[%s:%d] read corps config faild ", __FUNCTION__, __LINE__ );		
		exit( 0 );
	}
}

int CCorpservice::GetCorpsBossType( int nBossID)
{
	CTemplateCorpsBOSSTable * tpBossTable = (CTemplateCorpsBOSSTable * )CDataStatic::GetTemp( TEMP_CORPSBOSSTABLE );
	if ( tpBossTable ==NULL)
	{
		LOG_ERROR( "corps", "can not find table[%d][%s:%d]",TEMP_CORPSBOSSTABLE , __LK_FILE__, __LINE__ );
		return CORPS_BOSS_TYPE_ERROR;
	}

	for ( unsigned int i = 0; i < ARRAY_CNT(tpBossTable->mBOSSID) && tpBossTable->mBOSSID[i] != 0;i++)
	{
		if ( nBossID == tpBossTable->mBOSSID[i])
		{
			return CORPS_COMMON_BOSS_TYPE;
		}
	}
	for( unsigned int i = 0; i < ARRAY_CNT(tpBossTable->mHardBOSSID) && tpBossTable->mHardBOSSID[i] != 0; i++ )
	{
		if ( nBossID == tpBossTable->mHardBOSSID[i])
		{
			return CORPS_HARD_BOSS_TYPE;
		}
	}
	
	return CORPS_BOSS_TYPE_ERROR;
}