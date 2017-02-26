#pragma once

#define	MAX_QUESTIONLIB_NUM			1000						// 题库数量
#define	TOTAL_QUESTION_NUM			( MAX_QUESTIONLIB_NUM*20 )	// 题目数量
#define	XML_ELEMENT_NAME_LENGTH		40							// xml名字长度
#define MAX_APPTYPE_NUM				5							// 最大应用个数
#define MAX_ANSWER_STRLEN			30							// 答案字符串的最大长度
#define LIB_ROOT_NAME				"LIBRARY_LIST"				// 题库列表xml标识名
#define LIB_LIBRARY_NAME			"LIBRARY"					// 题库xml标识名
#define LIB_QUESTION_NAME			"QUESTION"					// 题目xml标识名
#define	MAX_PLAYER_QUESTIONNUM		20							// 每个玩家最多的答题数目
#define EACH_QUESTION_TIMELENGTH	10							// 每个问题的答题时限
#define TOTAL_QUESTION_TIMELENGTH	300							// 所有问题答题时限
#define	MAX_CREATED_LIBNUM			30							// 选题时有可能产生的最大题库数量
#define MAX_RANDOM_QUESTIONNUM		50							// 随机产生的最大题目数
#define MAX_QUESTIONNUM_EACHLIB		50							// 每个题库最大的题目数量
#define MAX_INVEST_PRIZENUM			10							// 调查问卷物品奖励个数
#define MAX_INVEST_QUESTLIBNUM		40							// 调查问卷的最大题库数量
#define MAX_INVESTANSER_DBBUFFERLENGTH 1024						// 调查问卷的答案的最大字符串长度
#define ACTIVITY_TIMER_LENGTH		1800000
#define HYDRANGEA_TIMER_LENGTH      1000						// 绣球计时器的检查周期
#define HYDRANGEA_TIMENUM			10							// 活动的时间信息个数

#define MAX_NPCPRIZE_NUM			10							// npc答题奖励的最大值
#define MAX_OPTION_NUM				15							// 答题最大的选项数目
#define INVESTRECORD_TIMER_LENGTH   120000						// 调查问卷的输出时间周期
#define PRELIMINAY_PBBUFFER_LENGTH	5000						// 乡试字节长度

#define CREATELIBID( TYPE, ID ) \
TYPE*1000000+ID					\

#define  MAKETIMEID( MONTH, DAY, HOUR  ) \
MONTH*10000 + DAY*100 + HOUR;	\

#define  MAX_ACTIVITYOGRE_NUM		50							// 活动怪的最大数量 

enum EInvestType			// 调查问卷的触发类型
{
	INVEST_TYPE_TIME	= 1,		// 根据时间点触发	
	INVEST_TYPE_MAP		= 2,		// 根据出入某个地图触发
	INVEST_TYPE_ACTIVIY	= 3,		// 活动
	INVEST_TYPE_LEVEL	= 4,		// 根据等级触发	
	INVEST_TYPE_METIER	= 5,		// 根据职业触发
	INVEST_TYPE_SEX		= 6,		// 根据性别触发
};

enum EBidState
{
	STATE_BIDDING		= 0,		// 家族竞价中
	STATE_SUBMITTING	= 1,		// 家族报名中
	STATE_FIGHTING		= 2,		// 家族战斗中
};

enum EBattleResult
{
	WIN_ATTSIDE				= 1,
	WIN_DEFSIDE				= 2,
};


//************************************************************************
// 场景服务器相关活动 常量定义
//************************************************************************

enum enActivityType
{
	ACTIVITY_START,	// 活动开始
	ACTIVITY_END,	// 活动结束
	ACTIVITY_ACTION_GETGIFT = 100, // 领取奖励
	ACTIVITY_ACTION_ONCREATE = 200, // 玩家上线
	ACTIVITY_ACTION_LEVELUP = 300, // 玩家升级
};

// 活动状态
enum enActivityState
{
	ACTIVITYSTATE_NOT_START,	// 未进行(活动结束)
	ACTIVITYSTATE_STARTED,		// 进行中(活动开始)
};

#define ACTIVITY_TIME_LST_SIZE	20
#define ACTIVITY_MAPDROP_SIZE	30
#define MAX_MAP_ID				300
#define DROP_MAPS_STR_LEN		400
#define ACTIVITY_DROP_SZ		5000	// 掉落信息 总数
#define SINGLE_ACTIVITY_DROP_SZ		1000	// 每个活动掉落信息 总数
#define ACTIVITY_OGREINDEX_SZ	200	// 每个活动包含的最多的怪物索引数量
#define MAX_HYDRANGEANPC_NUM 20 // 绣球npc的最大数量
#define MAX_TEMPENTITY_NUM 20 // 每个模板配置的最大npc数量

#define MAX_TREASUREBOWL_EXCHANGENUM 20	// 聚宝盆生成的物品可能个数的数量
#define MAX_TREASUREBOWL_PROBALITY_NUM 50	// 聚宝盆兑换概率信息的最大数量
#define MAX_TREASUREBOWL_ITEM_NUM 200	// 聚宝盆能够用于对换的物品数量
#define MAX_TREASUREBOWL_SPACENUM 3		// 聚宝盆能够用于对换的物品数量


// 抛绣球的方式
enum Em_HydrangeaThrowType
{
	THROWTYPE_BYPLAYER = 0,		// 绣球被玩家抛出
	THROWTYPE_BYNPC    = 1,		// 绣球被系统抛出	
};	

// 抢绣球的最大人数
#define  MAX_HYDRANGEARABER_NUM 100

// 抢绣球活动的状态
enum Em_HydrangeaActivityStatus
{
	HYDRANGEASTATUS_NULL				= 0,	// 活动未开始或者结束
	HYDRANGEASTATUS_START				= 1,	// 活动已经开始
	HYDRANGEASTATUS_HOLD				= 2,	// 绣球被某个人占有的状态
	HYDRANGEASTATUS_GRAB				= 3,	// 绣球抢夺状态	
};

// 绣球状态
enum Em_HydrangeaStatus
{
	PLAYER_HYDRANGEASTATUS_NULL		=	0,		// 没有绣球状态
	PLAYER_HYDRANGEASTATUS_HOLD		=	1,		// 活动过程中持有绣球
	PLAYER_HYDRANGEASTATUS_OWEN		=	2,		// 结束后获得绣球
};

// 乾坤袋相关
// 程序内部使用的奖励类型
enum enum_luckybag_award_type
{
	LUCKYBAG_AWARD_ITEM = 1, // 奖励道具
	LUCKYBAG_AWARD_MONEY = 2, // 奖励金钱
	LUCKYBAG_AWARD_BAG = 3, // 奖励包（已经在LUA中做了奖励）
	LUCKYBAG_AWARD_OTHER = 4, // 其他奖励（已经在LUA中做了奖励）
	LUCKYBAG_AWARD_TELEPORT = 5, // 传送
};

// 答案选项
enum EOptionType 
{
	OPTION_A = 0x0001,
	OPTION_B = 0x0002,
	OPTION_C = 0x0004,
	OPTION_D = 0x0008,
	OPTION_E = 0x0010,	
	OPTION_F = 0x0020,	
	OPTION_G = 0x0040,	
	OPTION_H = 0x0080,	
	OPTION_I = 0x0100,	
	OPTION_J = 0x0200,	
	OPTION_K = 0x0400,	
	OPTION_L = 0x0800,	
	OPTION_M = 0x1000,	
};

// 调查问卷的状态
enum EInvestStatus
{
	INVEST_STATUS_LEVELINVEST = ( 1 >> 0 ),	// 根据等级所做的调查问卷
};

// 科举榜的类型
enum EPreliminaryListType
{
	PREMILINARYLISTTYPE_TUESDAY		=	1,		//	周2榜
	PREMILINARYLISTTYPE_THURSDAY	=	2,		//	周4榜		
};	

#define  MAX_PRELIMINARY_TOPLISTNUM 50		// 乡试榜的最大人数
#define  MAX_PRELIMINARY_QUESTIONNUM 100	// 乡试题目的最大数量

enum EPreliminaryStatus
{
	PRELIMINARYSTATUS_NULL				= 0,		// 活动未开始
	PRELIMINARYSTATUS_READY				= 1,		// 活动倒计时状态		
	PRELIMINARYSTATUS_ANSWERQUESTION	= 2,		// 活动答题状态
	PRELIMINARYSTATUS_NOTIFYANSWER		= 3,		// 答案验证状态
};

enum ECampLimitType							// 阵营消息的限制类型
{
	CAMPLIMITTYPE_NULL	= 0,				// 没有限制
	CAMPLIMITTYPE_LEVEL	= ( 1 << 0 ),		// 等级限制
};

// 怪物活动
enum EOgreActivityType
{
	OGREACTIVITY_NULL			=	0,	// 没有活动信息
	OGREACTIVITY_STARTOGRESIEGE	=	1,	// 怪物攻城开始的提示信息
	OGREACTIVITY_ENDOGRESIEGE	=	2,	// 怪物攻城结束的提示信息		
};

// 活动Event ID
enum enActivityEvents
{
	ACTIVITY_EVENT_START		= 1, // 活动开启
	ACTIVITY_EVENT_END			= 2, // 活动结束
	ACTIVITY_EVENT_USEITEM		= 3, // 使用活动物品
	ACTIVITY_EVENT_USR			= 100, // 100以后的是用户自定义
};



// 答题验证的种类
enum EQuestionVerifyKind
{
	VERIFYKIND_NULL		= 0,			// 无验证
	VERIFYKIND_REPETION = 1,			// 副本答题验证
										// 其他
};

// 答题验证的方式
enum EQuestionVerifyType
{
	VERIFYTYPE_NULL		=	0,				// 没有限制
	VERIFYTYPE_TIMEOUT	=	( 1 << 0 ),		// 超时为错限制
	VERIFYTYPE_OFFLINE	=	( 1 << 1 ),		// 下线为错的限制
};

// 答题验证的结果
enum EQuestionVerifyReasultReason
{
	VERIFYREASON_ANSWER		= 0,		// 答题获取的结果
	VERIFYREASON_TIMEOUT	= 1,		// 超时获取的结果
	VERIFYREASON_OFFLINE	= 2,		// 掉线获取的结果				
};

// 地图掉落
enum EMapDropType
{
	DROPTYPE_ADD, // 添加
	DROPTYPE_REPLACE, // 替换
};