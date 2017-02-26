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

// 客户端发送消息
void CRankModule::OnMessage( int nServerID, CMessage* pMessage )
{
	switch( pMessage->msghead().messageid() )
	{
	case ID_S2G_NOTIFY_PERSONALRANKINFO:
		{
			OnMessagePersonalRankInfoReport( nServerID, pMessage );
			break;
		}
	case ID_S2G_RANK_REMOVE:
		{
			OnMessageRankRemove( nServerID, pMessage );
			break;
		}
		// 网关处理场景转发的客户端请求等级列表消息
	case ID_C2S_REQUEST_PLAYERRANK:
		{
			OnRouteMessagePlayerRankReqeust( nServerID, pMessage );
			break;
		}
		// 获取指定玩家的所有排行榜称号(Top 10)
	case ID_S2G_REQUEST_TOPTEN:
		{
			OnMessageObtainTopTenRequest( nServerID, pMessage );
			break;
		}
	default:
		{
			break;
		}
	}
}

int CRankModule::OnMessageObtainTopTenRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL ) return -1;

	CMessageObtainTopTenRequest* tpReq = ( CMessageObtainTopTenRequest* ) pMessage->msgpara();

	CRoleGlobalInfo* pRoleInfo = NULL;
	pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpReq->charid() );
	if (pRoleInfo == NULL)
	{
		LOG_ERROR("default", "no this player in get rankdata, charid=%d", tpReq->charid() );
		return -1;
	}

	CMessage tMessage;
	CMessageObtainTopTenResponse tPara;
	pbmsg_sethead( &tMessage, ID_G2S_RESPONSE_TOPTEN, 0, 0, 0, 0, 0, 0 );
	tPara.set_charid( tpReq->charid() );
	tMessage.set_msgpara( (long) &tPara );

	// 人物等级排行榜 -- 全服务器
	RANK_LEVEL_TYPE::CCountryRank* pRankLevelAllData = mLevelRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankLevelAllData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankLevelAllData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_LEVEL_ALL );
			tpTitle->set_index( i );
		}		
	}
	// 人物等级排行榜 -- 本国
	RANK_LEVEL_TYPE::CCountryRank* pRankLevelNationData = mLevelRankOutdated.GetCountryRank( pRoleInfo->CountryID() );
	LK_ASSERT( pRankLevelNationData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankLevelNationData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_LEVEL_MYNATION );
			tpTitle->set_index( i );
		}		
	}
	// 拥有财富排行榜 -- 非绑定金钱
	RANK_MONEY_TYPE::CCountryRank* pRankMoneyFreeData = mMoneyRankOutdated[FREE_MONEY].GetCountryRank( 0 );
	LK_ASSERT( pRankMoneyFreeData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankMoneyFreeData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_MONEY_NOBIND );
			tpTitle->set_index( i );
		}		
	}
	// 拥有财富排行榜 -- 所有钱
	RANK_MONEY_TYPE::CCountryRank* pRankMoneyAllData = mMoneyRankOutdated[ALL_MONEY].GetCountryRank( 0 );
	LK_ASSERT( pRankMoneyAllData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankMoneyAllData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_MONEY_ALL );
			tpTitle->set_index( i );
		}		
	}	
	// 杀敌数排行榜 -- 本周护国
	RANK_KILL_TYPE::CCountryRank* pRankKillNationData = mKillRankOutdated[KILL_PROT_NATION].GetCountryRank( 0 );
	LK_ASSERT( pRankKillNationData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankKillNationData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_KILL_PROT_NATION );
			tpTitle->set_index( i );
		}		
	}
	// 杀敌数排行榜 -- 本周出征
	RANK_KILL_TYPE::CCountryRank* pRankKillOutData = mKillRankOutdated[KILL_OUT].GetCountryRank( 0 );
	LK_ASSERT( pRankKillOutData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankKillOutData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_KILL_OUT );
			tpTitle->set_index( i );
		}		
	}
	// 杀敌数排行榜 -- 本周总杀敌数
	RANK_KILL_TYPE::CCountryRank* pRankKillTotalData = mKillRankOutdated[KILL_TOTAL].GetCountryRank( 0 );
	LK_ASSERT( pRankKillTotalData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankKillTotalData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_KILL_TOTAL );
			tpTitle->set_index( i );
		}		
	}
	// 功勋排行榜 -- 本周世界总功勋
	RANK_HONOR_TYPE::CCountryRank* pRankHonorAllData = mHonorRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankHonorAllData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankHonorAllData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_HONOR_ALL );
			tpTitle->set_index( i );
		}		
	}
	// 功勋排行榜 -- 本周本国总功勋
	RANK_HONOR_TYPE::CCountryRank* pRankHonorNationData = mHonorRankOutdated.GetCountryRank( pRoleInfo->CountryID() );
	LK_ASSERT( pRankHonorNationData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankHonorNationData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_HONER_MYNATION );
			tpTitle->set_index( i );
		}		
	}
	// 死亡次数排行榜
	RANK_DIE_TYPE::CCountryRank* pRankDieData = mDieNumRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankDieData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankDieData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_DIE );
			tpTitle->set_index( i );
		}		
	}
	// 完成任务数排行榜
	RANK_ENDTASK_TYPE::CCountryRank* pRankEndTaskData = mEndTaskRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankEndTaskData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankEndTaskData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_END_TASK );
			tpTitle->set_index( i );
		}		
	}
	// 在线时长排行榜
	RANK_ONLINE_TYPE::CCountryRank* pRankOnlineData = mOnLineRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankOnlineData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankOnlineData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_ONLINE_TIME );
			tpTitle->set_index( i );
		}		
	}
	// 蟠桃劫总积分排行榜
	RANK_PANTAO_TYPE::CCountryRank* pRankPanTaoData = mPantaoRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankPanTaoData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankPanTaoData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_PANTAO_MARK );
			tpTitle->set_index( i );
		}		
	}
	// 龙宫总积分排行榜
	RANK_LONGGONG_TYPE::CCountryRank* pRankLongGongData = mLonggongRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankLongGongData != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankLongGongData->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_LONGGONG_MARK );
			tpTitle->set_index( i );
		}		
	}

	// 原地复活次数排行榜
	RANK_ORIGINRELIVETIMES_TYPE::CCountryRank* pRankReliveTimes = mOriginReliveRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankReliveTimes != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankReliveTimes->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_ORIGINRELIVETIMES );
			tpTitle->set_index( i );
		}		
	}

	// 杀本阵营人数(全服)
	RANK_KILLSAMENATIONNUM_TYPE::CCountryRank* pRankKillSameNation = mKillSameNationRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankKillSameNation != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankKillSameNation->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_KILLSAMENATIONNUM );
			tpTitle->set_index( i );
		}		
	}

	// 杀怪数(全服)
	RANK_KILLOGRENUM_TYPE::CCountryRank* pRankKillOgre = mKillOgreRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankKillOgre != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankKillOgre->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_KILLOGRENUM );
			tpTitle->set_index( i );
		}		
	}

	// 精炼失败次数(全服)
	RANK_UPFAILNUM_TYPE::CCountryRank* pRankUpFail = mUpFailRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankUpFail != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankUpFail->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_UPFAILNUM );
			tpTitle->set_index( i );
		}		
	}

	// 排行称号新增修改 [12/18/2009 Macro]

	// 大雁塔积分排行
	RANK_DAYANTA_TYPE::CCountryRank* pRankDayanta = mDayantaRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankDayanta != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankDayanta->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_DAYANTA );
			tpTitle->set_index( i );
		}		
	}


    // 大乱斗积分排行
	RANKTYPE_LASTONE_TYPE::CCountryRank* pRankLastOne = mDaLuanDouOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankLastOne != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankLastOne->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_LASTONE );
			tpTitle->set_index( i );
		}		
	}

	// 大乱斗总积分排行
	RANKTYPE_LASTONE_TYPE::CCountryRank* pRankLastOneTotal = mDaLuanDouTotalOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankLastOneTotal != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankLastOneTotal->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_TOTAL_LASTONE );
			tpTitle->set_index( i );
		}		
	}



	// 莲花洞积分排行
	RANK_LIANHUADONG_TYPE::CCountryRank* pRankLianhuadong = mLianhuadongRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankLianhuadong != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankLianhuadong->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_LIANHUADONG );
			tpTitle->set_index( i );
		}		
	}

	// 黄蜂洞积分排行
	RANK_HUANGFENGDONG_TYPE::CCountryRank* pRankHuangfendong = mHuangfengdongRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankHuangfendong != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankHuangfendong->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_HUANGFENGDONG );
			tpTitle->set_index( i );
		}		
	}

	// 个人成就点排行
	RANK_ACHIVE_TYPE::CCountryRank* pRankAchieve = mAchiveRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankAchieve != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankAchieve->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_ACHIVE_POINT );
			tpTitle->set_index( i );
		}		
	}

	// 好感度排行
	RANK_FAVOR_TYPE::CCountryRank* pRankFavor = mFavorRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankFavor != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankFavor->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_FAVOR_POINT );
			tpTitle->set_index( i );
		}		
	}

	// 萎缩度排行
	RANK_BAD_TYPE::CCountryRank* pRankBad = mBadRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankBad != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankBad->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_BAD_POINT );
			tpTitle->set_index( i );
		}		
	}

	// 家族pvp积分排行
	RANK_FAMILY_POINTS_TYPE::CCountryRank* pRankFamily = mFamilyRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankFamily != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankFamily->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_FAMILY_PVP );
			tpTitle->set_index( i );
		}		
	}

	// 家族品仙桃排行
	RANK_FAMILY_POINTS_TYPE::CCountryRank* pRankFamilyTao = mFamilyTaoRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankFamilyTao != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankFamilyTao->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_FAMILY_TAO );
			tpTitle->set_index( i );
		}		
	}

	// 军团PVP排行
	RANK_CORPS_POINTS_TYPE::CCountryRank* pRankCorps = mCorpsRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankCorps != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankCorps->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_CORPS_PVP );
			tpTitle->set_index( i );
		}		
	}

	// 军团boss积分排行
	OBTAIN_TOP10( RANK_CORPS_POINTS_TYPE, mCorpsBossScoreRankOutdated, 0, RANKTYPE_CORPSBOSS_SCORE )

	// 军团采集活跃度排行
	OBTAIN_TOP10( RANK_CORPS_POINTS_TYPE, mCorpsCollectActiveRankOutdated, 0, RANKTYPE_CORPSCOLLECT_ACTIVE )
	
	// 今日杀敌数(护卫)排行
	RANK_KILL_TYPE::CCountryRank* pRankProtNation = mDayKillRankOutdated[ KILL_PROT_NATION ].GetCountryRank( 0 );
	LK_ASSERT( pRankProtNation != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankProtNation->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_KILL_PROT_NATION_TODAY );
			tpTitle->set_index( i );
		}		
	}

	// 今日杀敌数(出征)排行
	RANK_KILL_TYPE::CCountryRank* pRankOut = mDayKillRankOutdated[ KILL_OUT ].GetCountryRank( 0 );
	LK_ASSERT( pRankOut != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankOut->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_KILL_OUT_TODAY );
			tpTitle->set_index( i );
		}		
	}

	// 今日本周总杀敌排行
	RANK_KILL_TYPE::CCountryRank* pRankTotal = mDayKillRankOutdated[ KILL_TOTAL ].GetCountryRank( 0 );
	LK_ASSERT( pRankTotal != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankTotal->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_KILL_TOTAL_TODAY );
			tpTitle->set_index( i );
		}		
	}

	// 奇闻异事 魅力值 [2/3/2010 Macro]
	RANK_CHARM_TYPE::CCountryRank* pRankCharm = mCharmRankOutdated.GetCountryRank( 0 );
	LK_ASSERT( pRankCharm != NULL, return -1 );
	for ( int i = 0; i < TOPTEN_VALUE; i ++ )
	{
		if ( pRankCharm->mContainer[ i ].HashKey() == tpReq->charid() )
		{
			PBRankTitle* tpTitle = tPara.add_ranktitle();
			tpTitle->set_ranktype( RANKTYPE_CHARM_VALUE );
			tpTitle->set_index( i );
		}		
	}

	OBTAIN_TOP10( RANK_COMMON_WORLDRANK_TYPE, mZhanGongOutdated, 0, RANKTYPE_ZHANGONG )
	OBTAIN_TOP10( RANK_COMMON_WORLDRANK_TYPE, mZhiGuoOutDated, 0, RANKTYPE_ZHIGUO )
	OBTAIN_TOP10( RANK_COMMON_WORLDRANK_TYPE, mTongShuaiOutdated, 0, RANKTYPE_TONGSHUAI )
	OBTAIN_TOP10( RANK_COMMON_WORLDRANK_TYPE, mWisAllOutdated, 0, RANKTYPE_WIS )
	OBTAIN_TOP10( RANK_COMMON_WORLDRANK_TYPE, mYunxiaoOutdated, 0, RANKTYPE_YUNXIAO )
	OBTAIN_TOP10( RANK_COMMON_WORLDRANK_TYPE, mWuDidongOutdated, 0, RANKTYPE_WUDIDONG )
	OBTAIN_TOP10( RANK_COMMON_WORLDRANK_TYPE, mGlobalWarOutdated, 0, RANKTYPE_GLOBALWAR )
	OBTAIN_TOP10( RANK_COMMON_WORLDRANK_TYPE, m_SpiritOutdated, 0, RANKTYPE_SPIRIT )

	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
	return 0;
}

// 处理场景上报的个人排行信息
int CRankModule::OnMessagePersonalRankInfoReport( int nServerID, CMessage* pMessage )
{
	CMessagePersonalRankInfoNotify* pRankInfoNotify = (CMessagePersonalRankInfoNotify*) pMessage->msgpara();

	LOG_NOTICE( "rank", "[%s] begin ", __FUNCTION__ );
	LOG_NOTICE("rank", "[%s] size=%d flag=%d rtype=%d isover=%d from svr %d", __FUNCTION__
		, pRankInfoNotify->personalranks_size(), pRankInfoNotify->flag(), pRankInfoNotify->rtype()
		, pRankInfoNotify->isover(), nServerID );

	if (pRankInfoNotify->personalranks_size() >1)
	{
		LOG_NOTICE("rank", "func=%s size=%d flag=%d rtype=%d isover=%d from svr %d", __FUNCTION__
			, pRankInfoNotify->personalranks_size(), pRankInfoNotify->flag(), pRankInfoNotify->rtype()
			, pRankInfoNotify->isover(), nServerID );
	}

	for( int i = 0; i < pRankInfoNotify->personalranks_size(); i++ )
	{
		const PBPersonalRankInfo& rPersonalRankInfo = pRankInfoNotify->personalranks(i);

		LOG_NOTICE("rank", "test-debug func=%s, player come! id=%d type=%d", __FUNCTION__, rPersonalRankInfo.charid(), pRankInfoNotify->rtype() );

		if ( pRankInfoNotify->flag() == 0 || pRankInfoNotify->rtype() == REFRESHTYPE_DAYLY || pRankInfoNotify->rtype() == REFRESHTYPE_ALL )
		{
			// 如果等级为0 直接pass 视为无效数据
			if ( rPersonalRankInfo.level() == 0 )
			{
				LOG_ERROR("rank", "dispos scee data player %d's level is 0", rPersonalRankInfo.charid() );
				continue;
			}
			CPlayerLevelRank	lPRIForLevel( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.level(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier(),
				0,0,0,
				0);
			lPRIForLevel.SetValue2(rPersonalRankInfo.exp() );
			LOG_NOTICE( "rank", "[%s] update level rank info %s", __FUNCTION__, lPRIForLevel.Show() );
			mLevelRankInTime.Update( lPRIForLevel, true	);

			mMoneyRankInTime[FREE_MONEY].Update(
				CPlayerMoneyRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.money(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mMoneyRankInTime[ALL_MONEY].Update(
				CPlayerMoneyRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.money()+rPersonalRankInfo.boundedmoney(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);
			mKillRankInTime[KILL_PROT_NATION].Update(
				CPlayerKillRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.killenemynumin(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);
			mKillRankInTime[KILL_OUT].Update(
				CPlayerKillRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.killenemynumout(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mKillRankInTime[KILL_TOTAL].Update(
				CPlayerKillRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.killenemynumin()+rPersonalRankInfo.killenemynumout(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mHonorRankInTime.Update( 
				CPlayerHonorRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.honor(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mDieNumRankInTime.Update( 
				CPlayerDieNumRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.dienum(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mEndTaskRankInTime.Update( 
				CPlayerEndTaskRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.completedtasks(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mOnLineRankInTime.Update( 
				CPlayerOnLineRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.onlinetime(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mPantaoRankInTime.Update( 
				CPlayerPantaoRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.pantaomark(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mLonggongRankInTime.Update( 
				CPlayerLonggongRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.longgongmark(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mOriginReliveRankInTime.Update( 
				CPlayerOriginReliveTimes( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.originrelivetimes(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mKillSameNationRankInTime.Update( 
				CPlayerRankKillSameNationNum( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.killsamenationnum(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mKillOgreRankInTime.Update( 
				CPlayerRankKillOgreNum( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.killogrenum(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mUpFailRankInTime.Update( 
				CPlayerRankUpFailNum( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.upfailnum(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mDayantaRankInTime.Update( 
				CPlayerRankDayanta( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.dayanta(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mDaLuanDouInTime.Update( 
				CPlayerRankLastOne( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.daluandou(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mDaLuanDouTotalInTime.Update( 
				CPlayerRankLastOne( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.daluandoutotal(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mGlobalWarInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.globalwar(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mLianhuadongRankInTime.Update( 
				CPlayerRankLianhuadong( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.lianhuadong(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mHuangfengdongRankInTime.Update( 
				CPlayerRankHuangfengdong( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.huangfengdong(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mAchiveRankInTime.Update( 
				CPlayerRankAchive( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.achivepoint(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mCharmRankInTime.Update( 
				CPlayerRankCharm( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.charmvalue(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mFlyDungRankInTime.Update( 
				CPlayerRankFlyDung( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.flydungmark(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mZhanGongInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.zhangong(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mZhiGuoInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.zhiguo(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mTongShuaiInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.tongshuai(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mWisAllInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.wisall(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mYunxiaoInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.yunxiao(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mWeekCharmIncInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.weekinccharm(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mShideInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.shide(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mXianYuanInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.xianyuanmark(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mChuShiInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.chushitu(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mYunLvInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.yunxiaolevel(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mWuDidongInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.wudidongmark(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			m_SpiritInTime.Update( 
				CPlayerRankCommon( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.spiritscore(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);
		}
		if( pRankInfoNotify->flag() == 0 || pRankInfoNotify->rtype() == REFRESHTYPE_HALFHOUR || pRankInfoNotify->rtype() == REFRESHTYPE_ALL) // 处理半小时刷新的
		{
			mDayKillRankInTime[KILL_PROT_NATION].Update(
				CPlayerKillRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.todaykillenemynumin(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);
			mDayKillRankInTime[KILL_OUT].Update(
				CPlayerKillRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.todaykillenemynumout(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);

			mDayKillRankInTime[KILL_TOTAL].Update(
				CPlayerKillRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.todaykillenemynumin()+rPersonalRankInfo.todaykillenemynumout(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);
			mDayHonorRankInTime.Update(
				CPlayerHonorRank( rPersonalRankInfo.charid(),
				(const char*)rPersonalRankInfo.charname().c_str(),
				rPersonalRankInfo.todayhonor(),
				rPersonalRankInfo.country(),
				rPersonalRankInfo.title(),
				rPersonalRankInfo.metier()
				), true
				);
		}
	}

	if (pRankInfoNotify->flag() == 1)  // 如果是网关主动要的
	{
		// 如果是最后1个包
		if ( pRankInfoNotify->isover() == 1 )
		{
			LK_ASSERT(  ( 0 <= pRankInfoNotify->rtype() ) && ( pRankInfoNotify->rtype() < REFRESHTYPE_ALL ) , return -1 );
			
			mRecvSceneList[pRankInfoNotify->rtype()].erase(nServerID);

			LOG_NOTICE( "rank", "server %d rankinfo(%d) return,  %d server left",
				nServerID, pRankInfoNotify->rtype(), mRecvSceneList[pRankInfoNotify->rtype()].size() );

			for( RECV_SCENE_LIST_TYPE::iterator itFind = mRecvSceneList[pRankInfoNotify->rtype()].begin();
				itFind != mRecvSceneList[pRankInfoNotify->rtype()].end();
				++itFind )
			{
				LOG_INFO( "rank", "server %d left in the recvmap", itFind->second );
			}

			// 各场景都返回了 可以刷了
			if( mRecvSceneList[pRankInfoNotify->rtype()].empty() )
			{
				if ( pRankInfoNotify->rtype() == REFRESHTYPE_DAYLY )
				{
					OnDailyUpdate();
				}
				else if( pRankInfoNotify->rtype() == REFRESHTYPE_HALFHOUR )
				{
					OnHalfHourUpdate();
				}
			}

		}
	}

	LOG_NOTICE( "rank", "[%s] finished ", __FUNCTION__ );

	return 0;
}

int CRankModule::OnMessageRankRemove( int nServerID, CMessage* pMsg )
{
	LOG_NOTICE( "rank", "[%s] begin ", __FUNCTION__ );
	CMessageRankRemove* pRankRemove = ( CMessageRankRemove* )pMsg->msgpara( );

	LOG_NOTICE("rank", "func=%s ranktype=%d charname=%s ", __FUNCTION__
		, pRankRemove->ranktype(), pRankRemove->charname().c_str() );

	// 从排行榜中删除单个角色
	if( pRankRemove->has_charname() && pRankRemove->has_ranktype() )
	{
		CRoleGlobalInfo* tpInfo = CServiceRoleIDName::GetSingleton( ).GetRoleInfoByRoleName( 
			pRankRemove->charname( ).c_str( ) );	
		LK_ASSERT( tpInfo != NULL, return SUCCESS )

			RemovePlayerFromRank( tpInfo->RoleID( ), pRankRemove->ranktype() );
	}
	// 重新刷新排行榜
	else if( pRankRemove->refresh() == 1 )
	{
		LOG_NOTICE( "default", "gm refresh rank ! ..." );

		CRankModule::GetSingletonPtr()->DailyUpdate( );
		//CRankModule::GetSingletonPtr()->WeeklyUpdate( );	
	}

	LOG_NOTICE( "rank", "[%s] finished ", __FUNCTION__ );
	return SUCCESS;
}
// 请求玩家排行榜
int CRankModule::OnRouteMessagePlayerRankReqeust( int nServerID, CMessage* pMessage )
{
	LOG_NOTICE( "rank", "[%s] begin ", __FUNCTION__ );
	CMessageGetPlayerRankRequest* pRankReq = (CMessageGetPlayerRankRequest*)pMessage->msgpara();
	int nBeginIndex = pRankReq->beginindex();
	int nNum = pRankReq->recordnum();
	CMessageGetPlayerRankResponse tRetMsg;
	CPlayerRankMeta* pRankMeta = NULL;
	int nMaxSize = 0;

	CRoleGlobalInfo* pRoleInfo = NULL;
	pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( pRankReq->charid() );
	if (pRoleInfo == NULL)
	{
		LOG_ERROR("default", "no this player in get rankdata, charid=%d", pRankReq->charid() );
		return -1;
	}

	switch( pRankReq->ranktype() )
	{
	case RANKTYPE_LEVEL_ALL:
		{
			nNum = std::min( nNum , (int)MAX_LEVEL_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_LEVEL_TYPE::CCountryRank* pRankData = mLevelRankOutdated.GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_LEVEL_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_LEVEL_MYNATION:
		{
			nNum = std::min( nNum , (int)MAX_LEVEL_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_LEVEL_TYPE::CCountryRank* pRankData = mLevelRankOutdated.GetCountryRank( pRoleInfo->CountryID() );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_LEVEL_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_MONEY_NOBIND:
		{
			nNum = std::min( nNum , (int)MAX_MONEY_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_MONEY_TYPE::CCountryRank* pRankData = mMoneyRankOutdated[FREE_MONEY].GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();

			RANK_MONEY_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_MONEY_ALL:
		{
			nNum = std::min( nNum , (int)MAX_MONEY_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_MONEY_TYPE::CCountryRank* pRankData = mMoneyRankOutdated[ALL_MONEY].GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();

			RANK_MONEY_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_KILL_PROT_NATION:
		{
			nNum = std::min( nNum , (int)MAX_KILL_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_KILL_TYPE::CCountryRank* pRankData = mKillRankOutdated[KILL_PROT_NATION].GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();

			RANK_KILL_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_KILL_OUT:
		{
			nNum = std::min( nNum , (int)MAX_KILL_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_KILL_TYPE::CCountryRank* pRankData = mKillRankOutdated[KILL_OUT].GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_KILL_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_KILL_TOTAL:
		{
			nNum = std::min( nNum , (int)MAX_KILL_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_KILL_TYPE::CCountryRank* pRankData = mKillRankOutdated[KILL_TOTAL].GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_KILL_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_HONOR_ALL:
		{
			nNum = std::min( nNum , (int)MAX_HONOR_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_HONOR_TYPE::CCountryRank* pRankData = mHonorRankOutdated.GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_HONOR_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_HONER_MYNATION:
		{
			nNum = std::min( nNum , (int)MAX_HONOR_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_HONOR_TYPE::CCountryRank* pRankData = mHonorRankOutdated.GetCountryRank( pRoleInfo->CountryID() );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_HONOR_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_DIE:
		{
			nNum = std::min( nNum , (int)MAX_DIE_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_DIE_TYPE::CCountryRank* pRankData = mDieNumRankOutdated.GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_DIE_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_END_TASK:
		{
			nNum = std::min( nNum , (int)MAX_ENDTASK_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_ENDTASK_TYPE::CCountryRank* pRankData = mEndTaskRankOutdated.GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_ENDTASK_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_ONLINE_TIME:
		{
			nNum = std::min( nNum , (int)MAX_ONLINE_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_ONLINE_TYPE::CCountryRank* pRankData = mOnLineRankOutdated.GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_ONLINE_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_PANTAO_MARK:
		{
			nNum = std::min( nNum , (int)MAX_PANTAO_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_PANTAO_TYPE::CCountryRank* pRankData = mPantaoRankOutdated.GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_PANTAO_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_LONGGONG_MARK:
		{
			nNum = std::min( nNum , (int)MAX_LONGGONG_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_LONGGONG_TYPE::CCountryRank* pRankData = mLonggongRankOutdated.GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_LONGGONG_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}



	case RANKTYPE_ORIGINRELIVETIMES:
		{
			nNum = std::min( nNum , (int)MAX_LONGGONG_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_ORIGINRELIVETIMES_TYPE::CCountryRank* pRankData = mOriginReliveRankOutdated.GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_ORIGINRELIVETIMES_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_KILLSAMENATIONNUM:
		{
			nNum = std::min( nNum , (int)MAX_LONGGONG_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_KILLSAMENATIONNUM_TYPE::CCountryRank* pRankData = mKillSameNationRankOutdated.GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_KILLSAMENATIONNUM_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_KILLOGRENUM:
		{
			nNum = std::min( nNum , (int)MAX_LONGGONG_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_KILLOGRENUM_TYPE::CCountryRank* pRankData = mKillOgreRankOutdated.GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_KILLOGRENUM_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_UPFAILNUM:
		{
			nNum = std::min( nNum , (int)MAX_LONGGONG_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_UPFAILNUM_TYPE::CCountryRank* pRankData = mUpFailRankOutdated.GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_UPFAILNUM_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_KILL_PROT_NATION_TODAY:
		{
			nNum = std::min( nNum , (int)MAX_KILL_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_KILL_TYPE::CCountryRank* pRankData = mDayKillRankOutdated[KILL_PROT_NATION].GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();

			RANK_KILL_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_KILL_OUT_TODAY:
		{
			nNum = std::min( nNum , (int)MAX_KILL_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_KILL_TYPE::CCountryRank* pRankData = mDayKillRankOutdated[KILL_OUT].GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_KILL_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}
	case RANKTYPE_KILL_TOTAL_TODAY:
		{
			nNum = std::min( nNum , (int)MAX_KILL_RANK_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_KILL_TYPE::CCountryRank* pRankData = mDayKillRankOutdated[KILL_TOTAL].GetCountryRank( 0 );
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
			RANK_KILL_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );
			if ( it != pRankData->mKeyMap.end() )
			{
				LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
				tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
				tRetMsg.set_myrankindex( it->second+1 );
			}
			break;
		}

		PLAYER_GET_RANK( RANKTYPE_HONOR_ALL_TODAY,MAX_HONOR_RANK_NUM ,RANK_HONOR_TYPE, mDayHonorRankOutdated, 0 )
			PLAYER_GET_RANK( RANKTYPE_HONER_MYNATION_TODAY,MAX_HONOR_RANK_NUM ,RANK_HONOR_TYPE, mDayHonorRankOutdated, pRoleInfo->CountryID() )
			PLAYER_GET_RANK( RANKTYPE_DAYANTA,MAX_DAYANTA_NUM ,RANK_DAYANTA_TYPE, mDayantaRankOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_LIANHUADONG,MAX_LIANHUADONG_NUM ,RANK_LIANHUADONG_TYPE, mLianhuadongRankOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_HUANGFENGDONG,MAX_HUANGFENGDONG_NUM,RANK_HUANGFENGDONG_TYPE, mHuangfengdongRankOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_ACHIVE_POINT,MAX_RANK_ACHIVE_NUM,RANK_ACHIVE_TYPE, mAchiveRankOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_FAVOR_POINT,MAX_RANK_FAVOR_NUM,RANK_FAVOR_TYPE, mFavorRankOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_BAD_POINT,MAX_RANK_BAD_NUM,RANK_BAD_TYPE, mBadRankOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_CHARM_VALUE,MAX_RANK_CHARM_NUM,RANK_CHARM_TYPE, mCharmRankOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_FLY_FB_MARK,MAX_FLY_DUNG_NUM ,RANK_FLY_DUNG_TYPE, mFlyDungRankOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_ZHANGONG,MAX_COMMON_NUM ,RANK_COMMON_WORLDRANK_TYPE, mZhanGongOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_ZHIGUO,MAX_COMMON_NUM ,RANK_COMMON_WORLDRANK_TYPE, mZhiGuoOutDated, 0)
			PLAYER_GET_RANK( RANKTYPE_TONGSHUAI,MAX_COMMON_NUM ,RANK_COMMON_WORLDRANK_TYPE, mTongShuaiOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_WIS,MAX_COMMON_NUM ,RANK_COMMON_WORLDRANK_TYPE, mWisAllOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_YUNXIAO,MAX_COMMON_NUM ,RANK_COMMON_WORLDRANK_TYPE, mYunxiaoOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_WORLD_WEEKINCCHARM,MAX_COMMON_NUM ,RANK_COMMON_ALLRANK_TYPE, mWeekCharmIncOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_CAMP_WEEKINCCHARM,MAX_COMMON_NUM ,RANK_COMMON_ALLRANK_TYPE, mWeekCharmIncOutdated, pRoleInfo->CountryID())

			PLAYER_GET_RANK( RANKTYPE_SHIDE			,MAX_COMMON_NUM ,RANK_COMMON_WORLDRANK_TYPE, mShideOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_XIANYUANMARK	,MAX_COMMON_NUM ,RANK_COMMON_WORLDRANK_TYPE, mXianYuanOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_CHUSHITU		,MAX_COMMON_NUM ,RANK_COMMON_WORLDRANK_TYPE, mChuShiOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_YUNXIAOTALEVEL,MAX_COMMON_NUM ,RANK_COMMON_WORLDRANK_TYPE, mYunLvOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_CAMPCHARM_LASTWEEK,MAX_COMMON_NUM ,RANK_COMMON_ALLRANK_TYPE, mLastWeekCharmInc, pRoleInfo->CountryID())
			PLAYER_GET_RANK( RANKTYPE_WUDIDONG,MAX_COMMON_NUM ,RANK_COMMON_WORLDRANK_TYPE, mWuDidongOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_LASTONE,MAX_COMMON_NUM ,RANKTYPE_LASTONE_TYPE, mDaLuanDouOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_TOTAL_LASTONE,MAX_COMMON_NUM ,RANKTYPE_LASTONE_TYPE, mDaLuanDouTotalOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_GLOBALWAR,MAX_COMMON_NUM ,RANK_COMMON_WORLDRANK_TYPE, mGlobalWarOutdated, 0)
			PLAYER_GET_RANK( RANKTYPE_SPIRIT, MAX_COMMON_NUM ,RANK_COMMON_WORLDRANK_TYPE, m_SpiritOutdated, 0)


			// 如果是团队点数 要先得到家族，再得到点数 还要设置上myValue
	case RANKTYPE_FAMILY_PVP:
		{
			nNum = std::min( nNum , (int)MAX_FAMILY_HONOR_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_FAMILY_POINTS_TYPE::CCountryRank* pRankData =mFamilyRankOutdated.GetCountryRank(0);
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();

			// 团队的key是团队ID
			CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( pRankReq->charid() );
			if ( pFamily != NULL )
			{
				RANK_FAMILY_POINTS_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pFamily->GetID() );
				if ( it != pRankData->mKeyMap.end() )
				{
					LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
					tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
					tRetMsg.set_myrankindex( it->second+1 );
				}
				tRetMsg.set_myrankvalue(pFamily->GetPVPScore());
			}
			break;
		}

	case RANKTYPE_FAMILY_TAO:
		{
			nNum = std::min( nNum , (int)MAX_FAMILY_HONOR_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_FAMILY_POINTS_TYPE::CCountryRank* pRankData =mFamilyTaoRankOutdated.GetCountryRank(0);
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();

			// 团队的key是团队ID
			CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( pRankReq->charid() );
			if ( pFamily != NULL )
			{
				RANK_FAMILY_POINTS_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pFamily->GetID() );
				if ( it != pRankData->mKeyMap.end() )
				{
					LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
					tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
					tRetMsg.set_myrankindex( it->second+1 );
				}
				tRetMsg.set_myrankvalue(pFamily->GetRepetionScore());
			}
			break;
		}

	case RANKTYPE_CORPS_PVP:
		{
			nNum = std::min( nNum , (int)MAX_CORPS_HONOR_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_CORPS_POINTS_TYPE::CCountryRank* pRankData = mCorpsRankOutdated.GetCountryRank(0);
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();

			// 团队的key是团队ID
			//CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( pRankReq->charid() );
			CCorps* pCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer(pRankReq->charid());
			if ( pCorps != NULL )
			{
				RANK_CORPS_POINTS_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pCorps->GetCorpsID() );
				if ( it != pRankData->mKeyMap.end() )
				{
					LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
					tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
					tRetMsg.set_myrankindex( it->second+1 );
				}
				tRetMsg.set_myrankvalue(pCorps->GetPVPScore());
			}
			break;
		}

	case RANKTYPE_CORPSBOSS_SCORE:
		{
			nNum = std::min( nNum , (int)MAX_CORPS_HONOR_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_CORPS_POINTS_TYPE::CCountryRank* pRankData = mCorpsBossScoreRankOutdated.GetCountryRank(0);
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();
		
			CCorps* pCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer(pRankReq->charid());
			if ( pCorps != NULL )
			{
				RANK_CORPS_POINTS_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pCorps->GetCorpsID() );
				if ( it != pRankData->mKeyMap.end() )
				{
					LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
					tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
					tRetMsg.set_myrankindex( it->second+1 );
				}
				tRetMsg.set_myrankvalue(pCorps->GetCorpsBossScore());
			}
			break;
		}
	case RANKTYPE_CORPSCOLLECT_ACTIVE:
		{
			nNum = std::min( nNum , (int)MAX_CORPS_HONOR_NUM );
			if ( nNum <= 0 )
				return -1;
			RANK_CORPS_POINTS_TYPE::CCountryRank* pRankData = mCorpsCollectActiveRankOutdated.GetCountryRank(0);
			LK_ASSERT( pRankData != NULL, return -1 );
			pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;
			nMaxSize = (int)pRankData->Size();

			CCorps* pCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer(pRankReq->charid());
			if ( pCorps != NULL )
			{
				RANK_CORPS_POINTS_TYPE::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pCorps->GetCorpsID() );
				if ( it != pRankData->mKeyMap.end() )
				{
					LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );
					tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());
					tRetMsg.set_myrankindex( it->second+1 );
				}
				tRetMsg.set_myrankvalue(pCorps->GetCorpsCollectActive());
			}
			break;
		}

		

	default:
		{
			LOG_ERROR("default", "error rank type:%d", pRankReq->ranktype() );
			return -1;
		}
	}

	// 设置升降数值
	int tChange = tRetMsg.myrankchange();
	if ( tChange > 0 )
	{
		tRetMsg.set_myrankchange( 1 );
		tRetMsg.set_myrankchangeval( tChange );
	}
	else if ( tChange < 0 )
	{
		tRetMsg.set_myrankchange( 2 );
		tRetMsg.set_myrankchangeval( 0-tChange );
	}

	if ( tChange == NEW_RANK_FLAG ) // 新上榜的
		tRetMsg.set_myrankchangeval( 0 );

	for ( int i = nBeginIndex; i < std::min( nNum, nMaxSize ); ++i )
	{
		PBSingleRankInfo* pbRankData = tRetMsg.add_rankinfo();
		LK_ASSERT( pbRankData != NULL, continue );
		pRankMeta[i].FillMessageData( pbRankData );
		pbRankData->set_rank(i);
	}
	tRetMsg.set_charid(pRankReq->charid());
	tRetMsg.set_ranktype( pRankReq->ranktype() );
	// 获得我的数值和排名升降 (到场景取)


	// 显示用户排行榜信息
	if ( nBeginIndex==0 && pRankReq->ranktype()==RANKTYPE_LEVEL_ALL )
	{
		int liRankNum = tRetMsg.rankinfo_size();
		for ( int i=0; i<liRankNum; i++ )
		{
			const PBSingleRankInfo& lRankInfo = tRetMsg.rankinfo( i );
			LOG_NOTICE( "rank", "[ %s ] Get rank(%s) info for char(%d), "
				"Charid(%d), CharName(%s), Rank(%d)", __FUNCTION__,
				"RANKTYPE_LEVEL_ALL", pRankReq->charid(),
				lRankInfo.charid(), lRankInfo.charname().c_str(), lRankInfo.rank() );
		}
	}
	
	

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_S2C_RESPONSE_PLAYERRANK, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tRetMsg);
	CGateServer::GetSingleton().Send2Scene(nServerID, &tMessage);
	
	LOG_NOTICE( "rank", "[%s] finished ", __FUNCTION__ );
	return 0;
}

// 加载等级排行信息
int CRankModule::OnSessionLoadIntimePlayerRankInfoFromDB( CMessage* tpMessage, int nParam1, int nParam2 )
{	
	LOG_DEBUG( "default", "[%s]", __FUNCTION__ );

	CMessageExecuteSqlResponse3* pSqlResponse = (CMessageExecuteSqlResponse3*)tpMessage->msgpara();

	unsigned int nColNum = pSqlResponse->colcount();
	unsigned int nRowNum = pSqlResponse->rowcount();
	LOG_INFO( "rank", "[LoadInTime] col=%d, row=%d", nColNum, nRowNum );

	for( unsigned int tRowIndex = 0; (tRowIndex < nRowNum) && ( tRowIndex < MAX_LEVEL_RANK_NUM ); ++tRowIndex)
	{
		int nRankType = atoi( pSqlResponse->fieldvalue( 6 + tRowIndex * nColNum ).c_str() );

		LOG_INFO( "rank", "[LoadInTime] RankType = %d tRowIndex=%d nColNum=%d nRowNum=%d tRowIndex*nColNum=%d", 
			nRankType, tRowIndex, nColNum, nRowNum, (tRowIndex*nColNum) );
		switch( nRankType ) // flag
		{
		case RANKTYPE_LEVEL_MYNATION:
		case RANKTYPE_LEVEL_ALL :
			{
				mLevelRankInTime.OnLoadRankData( 
					CPlayerLevelRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					0, 0, 0, atoll( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_MONEY_NOBIND:
			{
				mMoneyRankInTime[FREE_MONEY].OnLoadRankData(
					CPlayerMoneyRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_MONEY_ALL:
			{
				mMoneyRankInTime[ALL_MONEY].OnLoadRankData(
					CPlayerMoneyRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_KILL_PROT_NATION:
			{
				mKillRankInTime[KILL_PROT_NATION].OnLoadRankData(
					CPlayerKillRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_KILL_OUT:
			{
				mKillRankInTime[KILL_OUT].OnLoadRankData(
					CPlayerKillRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_KILL_TOTAL:
			{
				mKillRankInTime[KILL_TOTAL].OnLoadRankData(
					CPlayerKillRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_HONOR_ALL:			
		case RANKTYPE_HONER_MYNATION:	
			{
				mHonorRankInTime.OnLoadRankData( 
					CPlayerHonorRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_DIE:
			{
				mDieNumRankInTime.OnLoadRankData( 
					CPlayerDieNumRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_END_TASK:
			{
				mEndTaskRankInTime.OnLoadRankData( 
					CPlayerEndTaskRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_ONLINE_TIME:
			{
				mOnLineRankInTime.OnLoadRankData( 
					CPlayerOnLineRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_PANTAO_MARK:
			{
				mPantaoRankInTime.OnLoadRankData( 
					CPlayerPantaoRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_LONGGONG_MARK:
			{
				mLonggongRankInTime.OnLoadRankData( 
					CPlayerLonggongRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}


		case RANKTYPE_ORIGINRELIVETIMES:
			{
				mOriginReliveRankInTime.OnLoadRankData( 
					CPlayerOriginReliveTimes( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}

		case RANKTYPE_KILLSAMENATIONNUM:
			{
				mKillSameNationRankInTime.OnLoadRankData( 
					CPlayerRankKillSameNationNum( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}

		case RANKTYPE_KILLOGRENUM:
			{
				mKillOgreRankInTime.OnLoadRankData( 
					CPlayerRankKillOgreNum( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}

		case RANKTYPE_UPFAILNUM:
			{
				mUpFailRankInTime.OnLoadRankData( 
					CPlayerRankUpFailNum( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_KILL_PROT_NATION_TODAY:
			{
				mDayKillRankInTime[KILL_PROT_NATION].OnLoadRankData(
					CPlayerKillRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_KILL_OUT_TODAY:
			{
				mDayKillRankInTime[KILL_OUT].OnLoadRankData(
					CPlayerKillRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_KILL_TOTAL_TODAY:
			{
				mDayKillRankInTime[KILL_TOTAL].OnLoadRankData(
					CPlayerKillRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
			LOAD_INTIME_FROM_DB( RANKTYPE_HONOR_ALL_TODAY,		mDayHonorRankInTime,		CPlayerHonorRank)		
				LOAD_INTIME_FROM_DB( RANKTYPE_DAYANTA,				mDayantaRankInTime,			CPlayerRankDayanta)
				LOAD_INTIME_FROM_DB( RANKTYPE_LIANHUADONG,			mLianhuadongRankInTime,		CPlayerRankLianhuadong)
				LOAD_INTIME_FROM_DB( RANKTYPE_HUANGFENGDONG,		mHuangfengdongRankInTime,	CPlayerRankHuangfengdong)
				LOAD_INTIME_FROM_DB( RANKTYPE_ACHIVE_POINT,			mAchiveRankInTime,			CPlayerRankAchive)
				LOAD_INTIME_FROM_DB( RANKTYPE_FAVOR_POINT,			mFavorRankInTime,			CPlayerRankFavor)
				LOAD_INTIME_FROM_DB( RANKTYPE_BAD_POINT	,			mBadRankInTime,				CPlayerRankBadPoint)
				LOAD_INTIME_FROM_DB( RANKTYPE_CHARM_VALUE	,		mCharmRankInTime,			CPlayerRankCharm)
				//LOAD_INTIME_FROM_DB( RANKTYPE_FAMILY_PVP,			mFamilyRankInTime,			CPlayerRankTeamPoints)
				//LOAD_INTIME_FROM_DB( RANKTYPE_CORPS_PVP	,			mCorpsRankInTime,			CPlayerRankTeamPoints)
				//LOAD_INTIME_FROM_DB( RANKTYPE_FAMILY_TAO,			mFamilyTaoRankInTime, 		CPlayerRankTeamPoints)
				LOAD_INTIME_FROM_DB( RANKTYPE_FLY_FB_MARK,			mFlyDungRankInTime,			CPlayerRankFlyDung)
				LOAD_INTIME_FROM_DB( RANKTYPE_ZHANGONG,          mZhanGongInTime,         CPlayerRankCommon)
				LOAD_INTIME_FROM_DB( RANKTYPE_ZHIGUO,          mZhiGuoInTime,         CPlayerRankCommon)
				LOAD_INTIME_FROM_DB( RANKTYPE_TONGSHUAI,          mTongShuaiInTime,         CPlayerRankCommon)
				LOAD_INTIME_FROM_DB( RANKTYPE_WIS,          mWisAllInTime,         CPlayerRankCommon)
				LOAD_INTIME_FROM_DB( RANKTYPE_YUNXIAO,          mYunxiaoInTime,         CPlayerRankCommon)
				LOAD_INTIME_FROM_DB( RANKTYPE_WORLD_WEEKINCCHARM,         mWeekCharmIncInTime,         CPlayerRankCommon)
				LOAD_INTIME_FROM_DB( RANKTYPE_CAMP_WEEKINCCHARM,          mWeekCharmIncInTime,         CPlayerRankCommon)

				LOAD_INTIME_FROM_DB( RANKTYPE_SHIDE			,          mShideInTime,         CPlayerRankCommon)
				LOAD_INTIME_FROM_DB( RANKTYPE_XIANYUANMARK	,          mXianYuanInTime,         CPlayerRankCommon)
				LOAD_INTIME_FROM_DB( RANKTYPE_CHUSHITU		,          mChuShiInTime,         CPlayerRankCommon)
				LOAD_INTIME_FROM_DB( RANKTYPE_YUNXIAOTALEVEL,          mYunLvInTime,         CPlayerRankCommon)
				LOAD_INTIME_FROM_DB( RANKTYPE_WUDIDONG,				 mWuDidongInTime,         CPlayerRankCommon)
                LOAD_INTIME_FROM_DB( RANKTYPE_LASTONE,				 mDaLuanDouInTime,         CPlayerRankLastOne)
				LOAD_INTIME_FROM_DB( RANKTYPE_TOTAL_LASTONE,		 mDaLuanDouTotalInTime,    CPlayerRankLastOne)
				LOAD_INTIME_FROM_DB( RANKTYPE_GLOBALWAR,			 mGlobalWarInTime,			CPlayerRankCommon)
				LOAD_INTIME_FROM_DB( RANKTYPE_SPIRIT,				m_SpiritInTime,			CPlayerRankCommon)
		case RANKTYPE_FAMILY_PVP:
		case RANKTYPE_CORPS_PVP:
		case RANKTYPE_FAMILY_TAO:
		case RANKTYPE_CORPSBOSS_SCORE:
		case RANKTYPE_CORPSCOLLECT_ACTIVE:
			{
				break;
			}
		default:
			{
				LOG_ERROR("rank", "error rank type in OnSessionLoadInTimePlayerRankInfoFromDB, type=%d", atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ) );
				break;
			}
		}
	}
	mLaunchFlag |= LOADFLAG1;
	return 0;
}

// 加载历史排行信息
int CRankModule::OnSessionLoadOutdatedPlayerRankInfoFromDB( CMessage* tpMessage, int nParam1, int nParam2 )
{
	LOG_DEBUG( "rank", "[%s] begine ", __FUNCTION__ );

	CMessageExecuteSqlResponse3* pSqlResponse = (CMessageExecuteSqlResponse3*)tpMessage->msgpara();

	unsigned int nColNum = pSqlResponse->colcount();
	unsigned int nRowNum = pSqlResponse->rowcount();
	LOG_INFO( "rank", "[LoadOutdated] col=%d, row=%d", nColNum, nRowNum );

	for( unsigned int tRowIndex = 0; (tRowIndex < nRowNum) && ( tRowIndex < MAX_LEVEL_RANK_NUM ); ++tRowIndex)
	{
		int nRankType = atoi( pSqlResponse->fieldvalue( 6 + tRowIndex * nColNum ).c_str() );
		if (tRowIndex == 0)
			LOG_INFO( "rank", "[LoadOutdated] RankType = %d", nRankType );
		switch( nRankType ) // flag
		{
		case RANKTYPE_LEVEL_MYNATION:
		case RANKTYPE_LEVEL_ALL :
			{
				mLevelRankOutdated.OnLoadRankData( 
					CPlayerLevelRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoll( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_MONEY_NOBIND:
			{
				mMoneyRankOutdated[FREE_MONEY].OnLoadRankData(
					CPlayerMoneyRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_MONEY_ALL:
			{
				mMoneyRankOutdated[ALL_MONEY].OnLoadRankData(
					CPlayerMoneyRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_KILL_PROT_NATION:
			{
				mKillRankOutdated[KILL_PROT_NATION].OnLoadRankData(
					CPlayerKillRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_KILL_OUT:
			{
				mKillRankOutdated[KILL_OUT].OnLoadRankData(
					CPlayerKillRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_KILL_TOTAL:
			{
				mKillRankOutdated[KILL_TOTAL].OnLoadRankData(
					CPlayerKillRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_HONOR_ALL:			
		case RANKTYPE_HONER_MYNATION:	
			{
				mHonorRankOutdated.OnLoadRankData( 
					CPlayerHonorRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_DIE:
			{
				mDieNumRankOutdated.OnLoadRankData( 
					CPlayerDieNumRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_END_TASK:
			{
				mEndTaskRankOutdated.OnLoadRankData( 
					CPlayerEndTaskRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_ONLINE_TIME:
			{
				mOnLineRankOutdated.OnLoadRankData( 
					CPlayerOnLineRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}

		case RANKTYPE_PANTAO_MARK:
			{
				mPantaoRankOutdated.OnLoadRankData( 
					CPlayerPantaoRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}

		case RANKTYPE_LONGGONG_MARK:
			{
				mLonggongRankOutdated.OnLoadRankData( 
					CPlayerLonggongRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}


		case RANKTYPE_ORIGINRELIVETIMES:
			{
				mOriginReliveRankOutdated.OnLoadRankData( 
					CPlayerOriginReliveTimes( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}

		case RANKTYPE_KILLSAMENATIONNUM:
			{
				mKillSameNationRankOutdated.OnLoadRankData( 
					CPlayerRankKillSameNationNum( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}

		case RANKTYPE_KILLOGRENUM:
			{
				mKillOgreRankOutdated.OnLoadRankData( 
					CPlayerRankKillOgreNum( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}

		case RANKTYPE_UPFAILNUM:
			{
				mUpFailRankOutdated.OnLoadRankData( 
					CPlayerRankUpFailNum( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}

		case RANKTYPE_KILL_PROT_NATION_TODAY:
			{
				mDayKillRankOutdated[KILL_PROT_NATION].OnLoadRankData(
					CPlayerKillRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_KILL_OUT_TODAY:
			{
				mDayKillRankOutdated[KILL_OUT].OnLoadRankData(
					CPlayerKillRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;
			}
		case RANKTYPE_KILL_TOTAL_TODAY:
			{
				mDayKillRankOutdated[KILL_TOTAL].OnLoadRankData(
					CPlayerKillRank( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )
					);
				break;

			}
			LOAD_OUTDATED_FROM_DB( RANKTYPE_HONOR_ALL_TODAY,	mDayHonorRankOutdated,			CPlayerHonorRank)		
				LOAD_OUTDATED_FROM_DB( RANKTYPE_DAYANTA	,			mDayantaRankOutdated,			CPlayerRankDayanta)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_LIANHUADONG,		mLianhuadongRankOutdated,		CPlayerRankLianhuadong)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_HUANGFENGDONG,		mHuangfengdongRankOutdated,		CPlayerRankHuangfengdong)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_ACHIVE_POINT,		mAchiveRankOutdated,			CPlayerRankAchive)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_FAVOR_POINT,		mFavorRankOutdated,				CPlayerRankFavor)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_BAD_POINT,			mBadRankOutdated,				CPlayerRankBadPoint)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_FAMILY_PVP,			mFamilyRankOutdated,			CPlayerRankTeamPoints)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_CORPS_PVP,			mCorpsRankOutdated,				CPlayerRankTeamPoints)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_CORPSBOSS_SCORE,			mCorpsBossScoreRankOutdated,				CPlayerRankTeamPoints)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_CORPSCOLLECT_ACTIVE,		mCorpsCollectActiveRankOutdated,				CPlayerRankTeamPoints)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_FAMILY_TAO,			mFamilyTaoRankOutdated, 		CPlayerRankTeamPoints)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_CHARM_VALUE,		mCharmRankOutdated, 			CPlayerRankCharm)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_FLY_FB_MARK,		mFlyDungRankOutdated, 			CPlayerRankFlyDung)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_ZHANGONG,        mZhanGongOutdated,           CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_ZHIGUO,        mZhiGuoOutDated,           CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_TONGSHUAI,        mTongShuaiOutdated,           CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_WIS,        mWisAllOutdated,           CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_WORLD_WEEKINCCHARM,  mWeekCharmIncOutdated,           CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_CAMP_WEEKINCCHARM,   mWeekCharmIncOutdated,           CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_YUNXIAO,   mYunxiaoOutdated,           CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_YUNXIAOTALEVEL,   mYunLvOutdated,           CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_SHIDE,   mShideOutdated,           CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_XIANYUANMARK,   mXianYuanOutdated,           CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_CHUSHITU,   mChuShiOutdated,           CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_CAMPCHARM_LASTWEEK,   mLastWeekCharmInc,           CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_WUDIDONG,   mWuDidongOutdated,           CPlayerRankCommon)
                LOAD_OUTDATED_FROM_DB( RANKTYPE_LASTONE,   mDaLuanDouOutdated,           CPlayerRankLastOne)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_TOTAL_LASTONE,   mDaLuanDouTotalOutdated,    CPlayerRankLastOne)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_GLOBALWAR,   mGlobalWarOutdated,    CPlayerRankCommon)
				LOAD_OUTDATED_FROM_DB( RANKTYPE_SPIRIT,   m_SpiritOutdated,    CPlayerRankCommon)

		default:
			{
				LOG_ERROR("rank", "error rank type in OnSessionLoadOutdatedPlayerRankInfoFromDB, type=%d", atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ) );
				break;
			}
		}
	}

	LOG_DEBUG( "rank", "[%s] finished ", __FUNCTION__ );
	mLaunchFlag |= LOADFLAG2;

	return 0;
}

