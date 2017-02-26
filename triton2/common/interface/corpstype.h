#pragma once
#define  MAX_CORPSNAME_LENGTH	40			// 军团名称
#define  LEVEL_CREATCORPS		40			// 创建军团时宗正的最少等级
#define  MAX_OFFICER_NUM		10			// 军团的官员职位个数
#define  MAX_CORPS_FAMILYNUM	20			// 军团的家族最大数量
#define  MAX_CORPSNUM			2000		// 军团的最大数量
#define  MAX_CORPSAPPLY_NUM		30			// 军团的最大申请列表
#define  CORPSDISBAND_NUMBERLIMIT	50		// 军团的解散人数
#define	 CORPS_BULLETIN_LENGTH		300		// 军团公告的字符限制
#define  CORPS_MONEY_CREATCOPRS	300000		// 创建军团需要的金钱30金
#define  MAX_COPRSPBBUFFER_LENGTH	1024	// 军团pb结构生成数组组的最大长度
#define  COUNT_LOADCORPS	10				// 加载军团信息时每次从数据库读取的次数
#define  COUNT_LOADCORPSBID	20				// 加载军团竞价信息时每次从数据库读取的次数
#define  COUNT_LOADCORPSBATTLE	20			// 加载军团战争信息时每次从数据库读取的次数
#define  CORPSTIMER_UPDATE_TIMELENGTH 30000	// 军团信息的更新时间  30秒
#define  CORPS_DISBAND_TIMELENGTH  86400	// 86400 军团的解散时间
#define	 CORPS_LEVEL_LIMIT			5		// 军团等级上限
#define  CORPS_BATTLE_NUM	100				
#define  CORPS_MAXMEMBERNUM     300			// 军团的最大人数
#define	 MAX_UNION_NUM			2000		// 联军最大数量
#define  MAX_CORPS_UNION		10			// 联军的最大军团数(包括主军团在内)
#define  MAX_OTHER_UNION		9			// 联军的其它军团
#define  MAX_CORPS_NPCOWNER		20			// 军团可占领的npc最大数
#define  MAX_BATTLESIZE			300			// 参战最多人数

#define CORPSTIMER_DISBAND_TIMELENGTH	600000	// 检查家族解散时间的时间间隔10分钟
#define CORPSTIMER_CHECK_NPC_STATE		5000	// 检查军团战竞价NPC状态定时器 5秒
#define SUBMIT_TIME_BEFORE_BATTLE		1800	//竞价结束到战斗开始之间选择阵容的时间，30分钟
#define BID_CLOSE_TIME_BUCKET			4500	//竞价结束到竞价重新开始的时间，75分钟

#define MAX_CORPS_BOSS_NUM		20				//最大军团boss数

enum ECorpsPost
{
	CORPS_POST_CORPSHEADER	= 0,	//	大将军
	CORPS_POST_GENERAL		= 1,	//	前锋将军	 
	CORPS_POST_COUNSELLOR	= 2,	//	军师
	CORPS_POST_FAMILYHEADER	= 3,	//	家族首领
	CORPS_POST_MEMBER		= 4,	//	普通成员
};

enum ECorpsMoney
{
	CRONTRIBUTEMONEY_BYMEMBER = 1,		// 个人捐献军团金钱
	CRONTRIBUTEMONEY_BYFAMILY = 2,		//	家族捐献军团金钱
};

// 军团战争状态
enum ECorpsWarStatus
{
	CORPS_WAR_FREE	= 0,	// 空闲
	CORPS_WAR_BIDING,		// 竞价中
	CORPS_WAR_WAITING,		// 竞价结束,等待参战
	CORPS_WAR_IN,			// 战斗中
};

enum ERoomAction
{
	ROOM_CLOSE = 0,
	ROOM_OPEN,
};

enum ERoomState
{
	ROOM_STATE_OPEN = 0, // 可以挑战
	ROOM_STATE_BUSY,
	ROOM_STATE_INWAR,	 // 战斗中
	ROOM_STATE_NONE,	 // 不存在
};

enum ECorpsChallegeAction
{	
	CHALLEGE_CANCEL = 0, // 取消挑战 
	CHALLEGE_REQUEST,	 // 发起挑战
};

enum EBattleMap
{
	CORPS_BATTLE_SCABBLE = 71, // 争霸赛地图
	CORPS_BATTLE_MAP = 80,	// 挑战赛地图
};

enum ECorpsBattleType
{
	TYPE_BATTLE_CHALLEGE = 0,	// 挑战赛
	TYPE_BATTLE_SCRABBLE,		// 争霸赛
	TYPE_BATTLE_ALL				// 所有类型 Add by DLC @2010-7-13
};

enum ECorpsErrCode
{
	CORPS_NOTICE_DESFORAGEDEAD = 0x1001,	// 对方粮草车被击毁
	CORPS_NOTICE_SRCFORAGEDEAD,				// 己方粮草车被击毁
	CORPS_NOTICE_FORAGEUPDATE,				// 粮草车已刷新
	CORPS_NOTICE_GATEKILLED,				// 城门被破
	CORPS_NOTICE_FLAGCHANGE,				// 大旗变化
};

enum ETypeCorpsFamily
{
	TYPE_FAMILY = 0,
	TYPE_CORPS,
};

#define GAMBLE_FUND_PERCENT		90	// 挑战赛赔率(90%)
#define PERCENT	100.0f

enum EMakeUnionOP
{
	EM_CORPS_MAKE_INFO	= 0,		// 查询信息
	EM_CORPS_MAKE_MODIFY = 1,		// 修改信息
	EM_CORPS_MAKE_INVITE = 2,		// 邀请签名
	EM_CORPS_MAKE_SIGN = 3,			// 签名
	EM_CORPS_MAKE_CLOSE = 4,		// 关闭
};

// 军团战进展阶段
enum ECorspBattleStep
{
	emBidStep	= 0,	// 竞价阶段
	emSubmitStep,		// 报名阶段
	emFightStep,		// 战争阶段
	emFinshStep,		// 战争结束
};

// 是否是主城 [12/2/2009 Macro]
#define IS_MAIN_CITY( x ) ( \
	x == COUNTRY_MAP_CHANGAN || x == COUNTRY_MAP_WUJI \
	|| x == COUNTRY_MAP_CHECHI  || x == COUNTRY_MAP_BAOXIANG \
	|| x == COUNTRY_MAP_NVER )
#define IS_CHANGAN( x ) ( x == COUNTRY_MAP_CHANGAN ? true : false )

// 挑战次数 [12/2/2009 Macro]
#define MAX_CHALLEGE_TIMES	3

enum EOrgType
{
	emTypeCorps = 0,	// 军团
	emTypeUnion,	// 联盟
};

// 主城、野地图类型
enum ECorpsMapType
{
	emMapMain = 0,	// 主城
	emMapNormal,	// 野地图
};

// 
#define ID_SWAP 10000000
#define GATENPCID(npcid,countryid)	( npcid + countryid * ID_SWAP )	//计算网关NPCID
#define SCENENPCID(id,countryid)	( id - countryid * ID_SWAP )	//还原场景NPCID
#define SCENENPCID_2(id)			(id % ID_SWAP)					//还原场景NPCID
#define SCENESERVERID(id)			(id / ID_SWAP)					//还原场景ServerID
// 验证国籍ID是否正确
#define IS_COUNTRYID(x) ( ( x > 0 && x < 5 ) ? true : false )

// 
enum ENationTitleAction
{
	emNoneAction = 0,	// 无行为
	emJoinNationality,	// 加入国籍
	emHoldNation,		// 占领国家
	emHoldWildMap,		// 占领野外地图
};

// 进入战场的buff时间15分钟
#define SCRABBLE_BATTLE_BUFFTIME	(15*60)


//=========================================================================
// 弱国系统相关定义
//=========================================================================
// 阵营弱势级别
enum EM_WeakLevel
{
	EWL_Normal					= 0,	// 非弱势阵营
	EWL_Level_1					= 1,	// 弱势一级阵营
	EWL_Level_2					= 2,	// 弱势二级阵营
	EWL_Level_3					= 3,	// 弱势三级阵营
};

// 奖励类型
enum EM_AwardType
{
	EAT_Buff					= 0,
	EAT_GiftPackage				= 1,	// 礼包
	EAT_Task					= 2,	
};
