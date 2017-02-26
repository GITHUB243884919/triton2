#ifndef _RANKTYPE_
#define _RANKTYPE_

// 排行榜的数量
enum ERankType // 排行榜类型
{
	RANKTYPE_LEVEL_ALL				= 1, // 全服的等级排行
	RANKTYPE_LEVEL_MYNATION			= 2, // 我国的等级排行
	RANKTYPE_MONEY_NOBIND			= 3, // 全服的非绑定钱
	RANKTYPE_MONEY_ALL				= 4, // 所有钱
	RANKTYPE_KILL_PROT_NATION		= 5, // 杀敌数(所有护卫)
	RANKTYPE_KILL_OUT				= 6, // 杀敌数(所有出征)
	RANKTYPE_KILL_TOTAL				= 7, // 本周总杀敌
	RANKTYPE_HONOR_ALL				= 8, // 本周世界总功勋
	RANKTYPE_HONER_MYNATION			= 9, // 本周本国总功勋
	RANKTYPE_DIE					= 10, // 死亡次数(全服)
	RANKTYPE_END_TASK				= 11, // 完成任务数(全服)
	RANKTYPE_ONLINE_TIME			= 12, // 在线时长(数值：分钟)
	RANKTYPE_PANTAO_MARK			= 13, // 蟠桃副本评价积分
	RANKTYPE_LONGGONG_MARK			= 14, // 龙宫副本评价积分
	RANKTYPE_ORIGINRELIVETIMES		= 15, // 原地复活次数(全服)
	RANKTYPE_KILLSAMENATIONNUM		= 16, // 杀本阵营人数(全服)
	RANKTYPE_KILLOGRENUM			= 17, // 杀怪数(全服)
	RANKTYPE_UPFAILNUM				= 18, // 精炼失败次数(全服)
	RANKTYPE_KILL_PROT_NATION_TODAY	= 19, // 今日杀敌数(护卫)
	RANKTYPE_KILL_OUT_TODAY			= 20, // 今日杀敌数(出征)
	RANKTYPE_KILL_TOTAL_TODAY		= 21, // 今日本周总杀敌
	RANKTYPE_HONOR_ALL_TODAY		= 22, // 今日世界荣誉
	RANKTYPE_HONER_MYNATION_TODAY	= 23, // 今日阵营荣誉
	RANKTYPE_DAYANTA				= 24, // 大雁塔积分（全服）
	RANKTYPE_LIANHUADONG			= 25, // 莲花洞积分（全服）
	RANKTYPE_HUANGFENGDONG			= 26, // 黄蜂洞积分（全服）
	RANKTYPE_ACHIVE_POINT			= 27, // 成就点（全服）
	RANKTYPE_FAVOR_POINT			= 28, // 好感度（全服）
	RANKTYPE_BAD_POINT				= 29, // 猥琐度（全服）
	RANKTYPE_FAMILY_PVP				= 30, // 家族PVP积分（全服）
	RANKTYPE_CORPS_PVP				= 31, // 军团PVP积分（全服）
	RANKTYPE_FAMILY_TAO				= 32, // 家族品仙桃积分（全服）
	RANKTYPE_CHARM_VALUE			= 33, // 魅力值<奇闻异事>
	RANKTYPE_FLY_FB_MARK			= 34, // 腾云大战副本积分
	RANKTYPE_ZHANGONG				= 35, // 战功（在荣誉下面）
	RANKTYPE_ZHIGUO					= 36, // 治国力（在才华下面）
	RANKTYPE_TONGSHUAI				= 37, // 统率力（在才华下面）
	RANKTYPE_WIS					= 38, // 才华总值（在才华下面）
	RANKTYPE_YUNXIAO				= 39, // 云霄塔副本积分
	RANKTYPE_WORLD_WEEKINCCHARM		= 40, // 本周新加魅力值(世界)
	RANKTYPE_CAMP_WEEKINCCHARM		= 41, // 本周新加魅力值(本阵营)
	RANKTYPE_SHIDE					= 42, // 师德积分积累值排行榜(本阵营)
	RANKTYPE_XIANYUANMARK			= 43, // 仙缘积分积累值排行榜(本阵营)
	RANKTYPE_CHUSHITU				= 44, // 出师徒弟排行榜(本阵营)
	RANKTYPE_YUNXIAOTALEVEL			= 45, // 云霄塔层数排行(世界)
	RANKTYPE_CAMPCHARM_LASTWEEK		= 46, // 上周百花仙子阵营排行 (新加魅力值(本阵营))
	RANKTYPE_WUDIDONG				= 47, // 无底洞积分排行
	RANKTYPE_LASTONE				= 48, // 大乱斗当前积分
	RANKTYPE_TOTAL_LASTONE			= 49, // 大乱斗总积分
	RANKTYPE_GLOBALWAR				= 50, // 跨服战积分
	RANKTYPE_SPIRIT					= 51, // 铸灵评分
	RANKTYPE_CORPSBOSS_SCORE		= 52,  // 军团boss积分
	RANKTYPE_CORPSCOLLECT_ACTIVE	= 53,  // 军团采集活跃度
	RANKTYPE_NEW_YUNXIAO			= 54,  // 云霄塔60层积分
	TOTAL_RANK_NUM					 , // 总数
};

#define CONTINUS_ONBOARD_TOPNUM 10  // top10会参与上榜统计

// 各排行榜统计的上榜天数上限
enum ERankContinusOnBoardLimit
{
	RANKLIMIT_LEVEL					= 10, // 等级排行
	RANKLIMIT_MONEY_NOBIND			= 10, // 非绑定钱
	RANKLIMIT_MONEY_ALL				= 10, // 所有钱
	RANKLIMIT_KILL_PROT_NATION		= 10, // 杀敌数(本周护国)
	RANKLIMIT_KILL_OUT				= 10, // 杀敌数(本周出征)
	RANKLIMIT_KILL_TOTAL			= 10, // 本周总杀敌
	RANKLIMIT_HONOR					= 10, // 功勋、战功(荣誉)
	RANKLIMIT_DIE					= 10, // 死亡次数(全服)
	RANKLIMIT_END_TASK				= 10, // 完成任务数(全服)
	RANKLIMIT_ONLINE_TIME			= 10, // 在线时长
	RANKLIMIT_REPETIONMARK			= 10, // 副本评价积分
	RANKLIMIT_ORIGINRELIVETIMES		= 10, // 原地复活次数(全服)
	RANKLIMIT_KILLSAMENATIONNUM		= 10, // 杀本阵营人数(全服)
	RANKLIMIT_KILLOGRENUM			= 10, // 杀怪数(全服)
	RANKLIMIT_UPFAILNUM				= 10, // 精炼失败次数(全服)
	RANKLIMIT_ACHIVE				= 10, // 成就点(全服)
	RANKLIMIT_FAVBAD				= 10, // 好感、猥琐(全服)
	RANKLIMIT_TEAM					= 10, // 团队
	RANKLIMIT_CHARM					= 10, // 奇闻异事
	RANKLIMIT_WIS					= 10, // 才华
	RANKLIMIT_COMMON				= 10, // 通用
};

// 排行榜刷新类型
enum RefreshType  
{
	REFRESHTYPE_DAYLY,			// 每日刷新
	REFRESHTYPE_HALFHOUR,		// 半小时刷新
	REFRESHTYPE_ALL,			// 全部
};

#define NEW_RANK_FLAG  10000
#define SINGLE_PAK_LINE	 200	// 	每个包报送的玩家个数

// 启动加载完成标志
#define LOADFLAG1	0x0F
#define LOADFLAG2	0xF0
#define LOADOK		0xFF

// 抽象宏
// 每日UPDATE排行榜
#define DAILY_UPDATE( InTime, OutDated, RankLimit, RankType) \
	InTime.CompareWithOld( OutDated, RankLimit );	\
	OutDated = InTime;								\
	OutDated.SaveRankDataToDB( RankType, true );    \
	OutDated.CheckRank();							\
	LOG_NOTICE("rank", "CheckRank type %d end", RankType );

// 加载实时榜
#define LOAD_INTIME_FROM_DB( Rank_Type, Data_Obj, Data_Class ) \
		case Rank_Type: \
			{\
				Data_Obj.OnLoadRankData(\
					Data_Class( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),\
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),\
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),\
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),\
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),\
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() )\
					), atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() )\
					, atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() )\
					);\
				break;\
			}

// 加载历史榜
#define LOAD_OUTDATED_FROM_DB( Rank_Type, Data_Obj, Data_Class ) \
		case Rank_Type:\
			{\
				Data_Obj.OnLoadRankData(\
					Data_Class( atoi( pSqlResponse->fieldvalue( 0 + tRowIndex * nColNum ).c_str() ),\
					(const char*)pSqlResponse->fieldvalue( 1 + tRowIndex * nColNum ).c_str(),\
					atoi( pSqlResponse->fieldvalue( 2 + tRowIndex * nColNum ).c_str() ),\
					atoi( pSqlResponse->fieldvalue( 3 + tRowIndex * nColNum ).c_str() ),\
					atoi( pSqlResponse->fieldvalue( 4 + tRowIndex * nColNum ).c_str() ),\
					atoi( pSqlResponse->fieldvalue( 5 + tRowIndex * nColNum ).c_str() ),\
					atoi( pSqlResponse->fieldvalue( 7 + tRowIndex * nColNum ).c_str() ),\
					atoi( pSqlResponse->fieldvalue( 8 + tRowIndex * nColNum ).c_str() ),\
					atoi( pSqlResponse->fieldvalue( 9 + tRowIndex * nColNum ).c_str() )\
					), atoi( pSqlResponse->fieldvalue( 10 + tRowIndex * nColNum ).c_str() )\
					, atoi( pSqlResponse->fieldvalue( 11 + tRowIndex * nColNum ).c_str() )\
					);\
				break;\
			}

// 响应玩家
#define PLAYER_GET_RANK( Rank_Type, MaxNum, Rank_Obj_Type, Rank_Obj, World_ID )\
		case Rank_Type:\
			{\
				nNum = std::min( nNum , (int)MaxNum );\
				if ( nNum <= 0 )\
					return -1;\
				Rank_Obj_Type::CCountryRank* pRankData = Rank_Obj.GetCountryRank( World_ID );\
				LK_ASSERT( pRankData != NULL, return -1 );\
				pRankMeta = (CPlayerRankMeta*)pRankData->mContainer;\
				nMaxSize = (int)pRankData->Size();\
				Rank_Obj_Type::CCountryRank::KeyMap::iterator it = pRankData->mKeyMap.find( pRankReq->charid() );\
				if ( it != pRankData->mKeyMap.end() )\
				{\
					LK_ASSERT( it->second >= 0 && it->second < pRankData->Size(), return -1 );\
					tRetMsg.set_myrankchange(pRankData->mContainer[it->second].GetRankChange());\
					tRetMsg.set_myrankindex( it->second+1 );\
				}\
				break;\
			}

// 填充称号
#define FILL_TOP_TEN( Rank_Type, Rank_OutDated  ) \
		tpTopTen = vpMsg->add_toptenlist(); \
		Rank_OutDated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );\
		tpTopTen->set_ranktype( Rank_Type );\
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )\
		{\
			tpTopTen->add_charlist( tCharList[i] );\
			tCharList[ i ] = 0; \
		}

// 获得称号
#define OBTAIN_TOP10( RANK_TYPE, Rank_OutDated, CountryID, Rank_Type )\
		RANK_TYPE::CCountryRank* pRank##Rank_OutDated = Rank_OutDated.GetCountryRank( CountryID );\
		LK_ASSERT( pRank##Rank_OutDated != NULL, return -1 );\
		for ( int i = 0; i < TOPTEN_VALUE; i ++ )\
		{\
			if ( pRank##Rank_OutDated->mContainer[ i ].HashKey() == tpReq->charid() )\
			{\
				PBRankTitle* tpTitle = tPara.add_ranktitle();\
				tpTitle->set_ranktype( Rank_Type );\
				tpTitle->set_index( i );\
			}		\
		}

#endif
