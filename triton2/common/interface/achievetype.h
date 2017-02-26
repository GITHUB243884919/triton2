#pragma once
/*
 *	成就类型文件
 */

#define MAX_PARAM			10		// 成就条件数量上限
#define MAX_ACHIEVE_NUM		300		// 成就数量上限

#define PALYER_ACHIEVE_TLV_BUFFER 1024 * 10

#define INVALID_ARRAY(X) ( ( X < 1 ? true : false ) )
#define ACHIEVE_FINISH	1

// 成就类型
enum ACHIEVEMENT_TYPE
{
	emAchieveSpcLevel				= 1,		// 人物等级
	emAchieveTalentLevel			= 2,		// 造化等级(c)
	emAchieveMeltingEquip			= 3,		//炼丹炉
	emAchieveSpcRankTitle			= 4,		//排行榜称号
	emAchieveSpcTitleNum			= 5,		//称号数量
	emAchieveOpenPillNum			= 6,		//开启内丹数量
	emAchievePacketGridNum			= 7,		//背包格子数量
	emAchieveUseGoldBugle			= 8,		//使用金喇叭道具
	emAchieveCompleteLuckTask		= 9,		//完成奇缘任务
	emAchieveCreateFamily			= 10,		//创建家族
	emAchieveJoinFamily				= 11,		//加入家族
	emAchieveAddFriend				= 12,		//添加好友
	emAchieveJoinCorps				= 13,		//加入军团
	emAchieveCreateCorps			= 14,		//创建军团
	emAchieveDoubleExp				= 15,		//领双
	emAchieveObtainObjID			= 16,		//获得指定ID物品
	emAchieveObtainObjType			= 17,		//获得指定类型物品
	emAchieveEquipLevel				= 18,		//装备等级
	emAchieveEquipGrade				= 19,		//装备品级
	emAchieveEquipInsertRuneNum		= 20,		//装备镶嵌符文的数量
	emAchieveEquipInsertJewelNum	= 21,		//装备镶嵌的宝石数量
	emAchieveCollectCard			= 22,		//收集卡片
	emAchieveInsertJewel			= 23,		//镶嵌宝石 c
	emAchieveEquipUpg				= 24,		//装备精炼 c
	emAchieveInsertRune				= 25,		//镶嵌符文 c
	emAchieveMemberContribute		= 26,		//家族贡献度
	emAchieveCorpsContribute		= 27,		//军团贡献度
	emAchieveFamilyRedPaper			= 28,		//使用家族红包
	emAchieveCorpsRedPaper			= 29,		//使用军团红包
	emAchieveFamilySuccess			= 30,		//家族战获胜
	emAchieveCorpsSuccess			= 31,		//军团战胜利
	emAchieveCorpsTask				= 32,		//军团任务
	emAchieveJudgeSystemGiveFavorPeopleNum	= 33,	//对多少人进行过好感度评价 s
	emAchieveJudgeSystemIGiveFavorPeopleNum = 34,	//多少人对我进行过好感度评价 s
	emAchieveJudgeSystemIGiveFavorValue		= 35,	// 获得某人对我的好感度达到某值s
	emAchieveJudgeSystemIGiveFavorTotal		= 36,	//获得的好感度总值s
	emAchieveJudgeSystemGiveFavorValue		= 37,	// 对某人的好感度s
	emAchieveSuccessTaskNum					= 38,	// 完成任务数量
	emAchieveSuccessStorageTaskNum			= 39,	// 完成黄榜任务数量
	emAchieveSuccessDesignateTask			= 40,	// 完成指定任务
	emAchieveSuccessDesignateTaskMapID		= 41,	// 完成指定地图任务
	emAchieveSuccessStorageTaskMapID		= 42,	// 完成指定地图黄榜任务
	emAchieveSuccessMenpaiTaskLoopTime		= 43,	// 完成师门任务次数
	emAchieveMineMapTask					= 44,	// 完成藏宝图任务
	emAchieveSuccessMainTaskNum				= 45,	// 完成主线任务数量
	emAchieveSuccessAchieve					= 46,	// 完成指定成就
	emAchieveMapGroping						= 47,	// 地图探索
	emAchieveNPCStudyCook					= 48,	// 通过NPC学习烹饪配方
	emAcheiveResearchCookNum				= 49,	// 创造配方数量
	emAcheiveResearchCookID					= 50,	// 创造指定配方ID
	emAchieveStudyCookNum					= 51,	// 学习烹饪数量
	emAchieveCookLevel						= 52,	// 烹饪进阶
	emAchieveUseItem						= 53,	// 使用物品
	emAchieveCookExp						= 54,	// 烹饪的熟练度
	emAchieveStudyPillMakeNum				= 55,	// 学习炼丹数量
	emAchievePillMakeLevel					= 56,	// 炼丹进阶
	emAchieveStudyCollectNum				= 57,	// 学习采集数量
	emAchieveCollectLevel					= 58,	// 采集进阶
	emAchieveStudyGoldMakeNum				= 59,	// 学习点金数量
	emAchieveGoldMakeLevel					= 60,	// 点金进阶
	emAchieveStudyArtificerNum				= 61,	// 学习巧匠数量
	emAchieveArtificerLevel					= 62,	// 巧匠进阶
	emAchieveFirstLifeSkill					= 63,	// 第一次入职一个生活职业
	emAchieveKillOgreNum					= 64,	// 杀怪计数
	emAchieveKillOgreID						= 65,	// 杀死指定ID怪
	emAchieveObtainFullSkillMagicWeaponID	= 66,	// 获得指定ID的全技能法宝
	emAchieveCleanSkillPointTime			= 67,	// 重置技能点
	emAchieveCleanPotentialPointTime		= 68,	// 重置潜能点
	emAchieveSkillBuff						= 69,	// 指定Buff
	emAchieveMapID							= 70,	// 指定地图
	emAchieveUseItemType					= 71,	// 使用指定类型物品
	emAchieveEasyPantaoKillNum				= 72,	// 简易蟠桃连杀点
	emAchieveNormalPantaoKillNum			= 73,	// 普通蟠桃连杀点
	emAchieveHardPantaoKillNum				= 74, 	// 精英蟠桃连杀点	
	emAchieveLonggongScore					= 75,	// 龙宫获得积分
	emAchievePantaoScore					= 76,	// 蟠桃获得积分
	emAchieveDungeonQuest					= 77,	// 副本探索
	emAchievePopularizeLevel				= 78,   // 仙缘等级
	emAchievePopularizeEducatedNum				= 79,   // 出师弟子数量
	emAchievePopularizeCopperNum				= 80,   // 青铜仙缘数量
	emAchievePopularizeSilverNum				= 81,   // 白银仙缘数量
	emAchievePopularizeGoldNum				= 82,   // 黄金仙缘数量
	emAchievePopularizeDiamondNum				= 83,   // 钻石仙缘数量
 	emAchievePopularizeMasterTypeNum			= 84,   // 师德数量
	emAchievePopularizeSlaveTypeNum				= 85,   // 仙缘数量
};

// 条件类型
enum CONDITION_TYPE
{
	emSameKindCnt	= 1,	// 同类计数
	emDiffKindCnt,			// 不同类计数
	emEnumerate,			// 列举
	emTrustReport,			// 上报即获得
	emSpecial,				// 需特别处理
	emCountReSet,			// 直接重置数量		
};

// 关系类型
enum RELATION_TYPE
{
	emRelationOr = 1,	// 或
	emRelationAnd,
};
