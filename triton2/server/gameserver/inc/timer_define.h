#pragma once

enum emTimerMark
{
	TIMER_SKLL_COOLDOWN				= 1, // 技能CD 和 公共CD
	TIMER_ENTITY_DISAPPER			= 2, // 实体消失
	TIMER_ENTTIY_REFRESH			= 3, // 实体回血，回蓝
	TIMER_ENTITY_RELIVE				= 4, // 实体复活
	TIMER_ENTITY_CLEAR				= 5, // 实体销毁
	TIMER_ENTITY_PROTECT			= 6, // 道具保护
	TIMER_ENTITY_ALIVE_REFRESH		= 7, // 怪物存活刷新
	TIMER_DIPLOID_TIME				= 8, // 双倍时间
	TIMER_REDSTONE_PROTECT			= 9, // 内丹保护时间
	TIMER_REDSTONE_CLEAR			= 10, // 内丹消失
	TIMER_REDSTONE_EXPMAGIC			= 11, // 内丹噬炼 	
	TIMER_ANSWERSTATUS_TOTALTIMEEND = 12, // 所有题目的答题时间到
	TIMER_ANSWERSTATUS_SINGLETIMEEND= 13, // 一道题目的答题时间到
	TIMER_REDSTONE_PRECESSTIMEOUT   = 14, // 客户端读秒超时
	//TIMER_KILLNPC_WITHHIDE_TIMEOUT  = 15, // NPC消失时间到 要先隐藏 再干掉
	TIMER_ASSIGNMENT				= 16, // Character定时任务
	TIMER_YELLOWNAME				= 17,	// 黄名
	TIMER_ITEMCHANGE				= 18,   // 物品变化(eg.猪肉变成腐烂的猪肉)
	TIMER_NOTIFYREFRESH_TEN			= 19,	// 提前10分钟通知刷怪
	TIMER_NOTIFYREFRESH_FIVE		= 20,	// 提前5分钟通知刷怪
	TIMER_CREATEOGRE_TEN			= 21,	// 10分钟后刷怪
	TIMER_MAGICWEAPON_INCMP			= 22,	// 法宝自动恢复MP
	TIMER_MAGICWEAPON_DECHP			= 23,	// 法宝自动扣减HP
	TIMER_FIREBATTLE				= 24,	// 战争触发
	TIMER_TASKTIMELIMIT				= 25,	// 限时任务
	TIMER_STARTCORPSBATTLE			= 26,	// 开始军团战
	TIMER_CDGROUP					= 27,	// CD组 (para表示cd组索引 )
	TIMER_BUFF_FUNCTION				= 28,	// buff作用
	TIMER_REPETION					= 29,	// repetion out
	TIMER_REPETION_CLEAR			= 30,	// repetion clear time out
	TIMER_ITEM_DESTROY				= 31,	// 物品销毁计时器
	TIMER_NATION_PROTECT			= 32,	// 本国非保护时间到时
	TIMER_WEDDING					= 33,	// 婚礼计时器
	TIMER_WEDDINGNOTIFY				= 34,	// 婚礼通知计时器
	TIMER_MARRIAGEAPPLY				= 35,	// 婚礼通知计时器
	TIMER_WEDDINGONSTREET			= 36,	// 婚礼游行计时器	
	
	TIMER_HYDRANGEATHROW			= 37,	// 招亲绣球的抛出时间
	TIMER_HYDRANGEAHOLD				= 38,	// 占有绣球的时间计时器
	TIMER_HYDRANGEAGRAB				= 39,	// 抢夺绣球的时间计时器										
	TIMER_HYDRANGEAACTIVITY			= 40,	// 绣球招亲的时间计时器
	TIMER_HYDRANGEASUGAR			= 41,	// 绣球招亲的的喜糖计时器
	TIMER_TITLE_DISAPPEAR			= 42,	// 称号计时器
	TIMER_OGREACTIVITY_TIMEEND		= 43,	// 怪物活动计时器
	TIMER_QUESTIONVERIFY			= 44,	// 答题验证的计时器
	TIMER_REPETIONQUESTION			= 45,	// 答题验证的计时器
	TIMER_KICKOUTBYVERIFY			= 46,	// 因为答题验证被踢出
	TIMER_REPTIONVERIFYTIME			= 47,	// 进入副本后多久进行副本答题验证
    TIMER_STOREBOXRELIVE            = 48,   // 摇钱树活动宝箱复活
	TIMER_CAMPKICKVOTETIME          = 49,   // 战场踢人的超时时间
};


// 此定义与TIMER_ASSIGNMENT绑定, 用与区分TIMER_ASSIGNMENT的不同作用类型
enum
{
	TIMER_ASSIGNMENT_REPETION		= 1,
};


// 秒timer允许误差时间
#define SEC_TIMER_ALLOW_MISTAKE 10
