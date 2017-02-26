#pragma once
#define MAX_FAMILYNAME_LENGTH		40			// 家族名称长度(字符个数)
#define MAX_FAMILYNOTICE_LENGTH		300			// 家族公告长度(字符个数)
#define	MAX_FAMILYMEMBER_NUM		30			// 家族成员数量上限
#define LEVEL_CREATFAMILY			40			// 创建家族需要的等级
#define LEVEL_FAMILYHEADER			40			// 族长的等级
#define MAX_FAMILYMEMBERINFO_BUFFULENT     2048	// 家族成员信息生产的pb结构解析需要的最大数组长度
#define FAMILYTIMER_UPDATE_TIMELENGTH	30000	// 所有家族信息更新一次的时长   30秒
#define FAMILY_FETCH_COUNT			10			// 一次从数据库获取的家族数量  
#define	COST_CREATFAMILY			150000		// 创建家族需要的金钱  15金
#define LEVEL_JOINFAMILY			30			// 加入家族的最低等级
#define MAX_APPLYLIST_APPLIESNUM	30			// 申请列表的申请最大数量
#define MAX_CORPSINVITE_NUM	 		5			// 军团邀请信息的数量
#define FAMILY_DISBAND_TIMELENGTH	86400	 	// 86400 家族解散时间
#define FAMILY_APPLYCORPS_TIMELENGTH 60			// 86400申请加入军团的冷却时间(现在改为1分钟)
#define FAMILY_LEAVECORPS_TIMELENGTH 86400		// 离开军团后再次加入的冷却时间	86400
#define MAX_UTF8_BYTES				3			// utf8的汉字字节数	 
#define FAMILYAPPLY_TIMELIMIT		60			// 申请加入家族的冷却时间86400 3600*24 24小时( 现在改为1分钟 )
#define FAMILY_LEAVEFAMILY_TIMELENGTH  86400	// 离开或者解散家族后再次加入家族的冷却时间	86400
#define FAMILY_LEVEL_LIMIT			5			// 家族等级上限
#define FAMILY_MAP_ID				66			// 家族战地图ID
#define SUBMIT_LIMIT				6			// 家族战报名人数上限

#define MAX_FAMILY_NUM				14000		// 所有家族的个数,数量是场景上家族数量的4倍，场景上的家族信息只有当家族成员在这个场景上时才会建立

#define WELFARE_REFESH_TIMELENGTH	86400		// 刷新npc福利的时间间隔86400
#define MAX_NPCWELFARE_MONEY		2000000		// npc可以领取的金钱的上限
#define MAX_NPCWELFARE_GLORY		2000		// npc可以领取的荣耀的上限
#define FAMILYBOSS_MAP_ID			94			// 家族boss的地图ID	   
#define MAX_CONTRIBUTE				2000000		// 个人贡献度上限	   
#define MAX_FAMILYCORPS_MONEY		2000000	    // 家族军团的金钱上限  													
#define INVALID_FAMILYCORPSID	    -3			// 无效的家族军团ID
#define FAMILYCORPS_MAXGETNUM		50			// 获取家族军团列表时每次最多发送的家族军团数量
#define MAX_CONTRIBUTE				2000000		// 个人贡献度上限  
#define MAX_FAMILYCORPS_MONEY		2000000	    // 家族军团的金钱上限

#define MONEY_EXCHANGERATE	10000				// 家族军团金钱和诽谤定金的兑换比率

#define FAMILYTIMER_DISBAND_TIMELENGTH	600000	// 检查家族解散时间的时间间隔10分钟

#define MAX_NPCNUMOFWELFARE		30				// 可领npc福利的最大个数


#define MAXGOODSINSHOP	50						// 商店可买商品的最大数量
#define MAXLEAGUESHOPSNUM	20					// 家族军团拥有最大商店的数量

#define MAXSHOPBYTELEN	1024					// 家族商店pb结构的最大长度
#define MAXESCAPEBUFFLENGTH 2048				// 转义解析后的最大长度
#define MAX_REPETIONTIMEINFO_NUM	7			// 副本时间配置的最大个数
#define MAX_MAILINFO_NUM	20					// 邮件奖励类型的最大个数

enum EInviteList
{										
	INVITELIST_FULL		= 1,					// 玩家的邀请列表满
	INVITELIST_EXIST	= 2,					// 家族的邀请列表已经在家族中
};

enum EApplyList
{
	APPLYLIST_FULL		= 1,				   // 申请列表满
	APPLYLIST_EXIST		= 2,				   // 玩家的申请存在
};	 

enum EFamilyPost
{	
	FAMILY_POST_HEADER			= 0,					// 族长		
	FAMILY_POST_PRESBYTER		= 1,					// 长老
	FAMILY_POST_BUSINESSMAN		= 2,					// 家族商人	
	FAMILY_POST_MEMEBER			= 3,					// 家族普通成员	
}; 

enum EReply
{
	FAMILY_REPLY_REFUSE = 0,	// 拒绝申请或者邀请
	FAMILY_REPLY_AGREE	= 1,  	// 同意申请或者邀请
	FAMILY_REPLY_IGNORE = 2,	// 忽略申请或者邀请
	FAMILY_REPLY_DELETE = 3,	// 删除申请或者邀请( 服务器用 )
};

enum EMethodJoinCorps
{
	JOINCORPS_CREATBYFAMILYHEADER	= 0,		// 加入了本家族族长创建的军团
	JOINCORPS_OTHERFAMILYCORPS		= 1,		// 加入了其他家族建立的军团
};

enum EFamilyState
{
	STATE_NOBID		= 0,		// 对NPC未竞价
	STATE_CHAZING	= 1,		// 对NPC已竞价
	STATE_OWN		= 2,		// 对NPC已占有
};

enum EChaState
{
	STATE_IDLE		= 0,		// 挑战空闲中
	STATE_WAITING	= 1,		// 挑战等待中
	STATE_BUSY		= 2,		// 挑战战斗中
};

enum EBattleType
{
	TYPE_BATTLE_NPC		= 0,
	TYPE_BATTLE_PLAYER		= 1,
};

enum EFamilyStatus
{
	FAMILY_STATUS_FAMILYEXPCARD	= ( 1 << 0 ),		// 经验令牌的领取状态
	FAMILY_STATUS_BOSSCHALLENGE	= ( 1 << 1 ),		// 挑战家族boss的状态	
};

enum EProChangedReason
{
	REASON_WAR				= 0,					// 因为家族争夺战
	REASON_TASK				= 1,					// 因为任务
	REASON_NPCWELFARE		= 2,					// 领取占领npc的福利
	REASON_CONTRIBUTE		= 3,					// 因为个人贡献	
	REASON_MEMBEREXP		= 4,					// 因为成员的经验值
	REASON_MEMBERAP			= 5,					// 因为成员的功勋值
	REASON_ACTIVIYCONSUME	= 6,					// 因为活动消耗
	REASON_LEVELUP			= 7,					// 因为升级
	REASON_HEADERTAKE		= 8,					// 因为领钱
	REASON_CHALLENGEWAR		= 9,					// 因为挑战赛
	REASON_WARWIN			= 10,					// 因为争夺赛获胜
	REASON_CHALLENGEWARWIND	= 11,					// 因为挑战赛获取
	REASON_MAKEREDPAPER		= 12,					// 因为购买红包
	REASON_STARTREPETION	= 13,					// 因为开启副本
	REASON_USEMONEYITEM		= 14,					// 因为使用金锭道具
	REASON_WARBID			= 15,					// 阵营战竞价扣钱
	REASON_ONLINETIME		= 16,					// 在线时间累积
	REASON_LOWSTABILITY		= 17,					// 安定度过低
	REASON_REDUCEONTIME		= 18,					// 定时衰减
	REASON_USECORPCRYSTAL	= 19,					// 上交军团boss魂魄
	REASON_USECORPBOSSSTONE	= 20,					// 炼妖单
	REASON_SUMMONCORPBOSS = 21,					//召唤军团boss消耗
};


enum EmRight
{
	EM_CREATE_OFFICER			= 	0x80000000,		// 建立, 修改, 删除职位的权力
	EM_GRANT_RIGHTS				= 	0x40000000, 	// 授予和修改官职的权力的权力
	EM_INVITE_MEMBER 			= 	0x00000001,		// 邀请成员的权力
	EM_APPROVE_APPLY 			= 	0x00000002,		// 审批申请加入的权力
	EM_FIRE_MEMBER 				= 	0x00000004, 	// 开除成员的权力
	EM_GRANT_SETOFFICER			=  	0x00000008,		// 设置下级官职的权力
	EM_LIST_APPLY 				= 	0x00000010,		// 罗列申请的权力
	EM_CHANGE_BULLETIN 			= 	0x00000020,		// 修改公告的权力
	EM_ACTVITY_REDSTONE			= 	0x00000040,		// 领取家族内丹的权利
	EM_TASK_CONVOY				=	0x00000080,		// 家族护送任务的领取
	EM_MONEY_REIN				=	0x00000100,		// 金钱的支配
	EM_WAR_RID					=	0x00000200,		// 竞价
	EM_WAR_HALL					=	0x00000400,		// 大厅报名
	EM_WAR_CHALLENGE			=	0x00000800,		// 挑战和接受挑战
	EM_WAR_SETFIGHTER			=	0x00001000,		// 安排参战人员
	EM_WELFARE_OBTAIN			=	0x00002000,		// 领取占领npc福利
	EM_WARNPC_GIVEUP			=	0x00004000,		// 放弃占领npc
	EM_RIGHT_DISBAND			=	0x00008000,		// 解散家族或者军团的权限
	EM_RIGHT_CANCELDISBAND		=	0x00010000,		// 解散家族或者军团的权限
	EM_RIGHT_CHALLENGEROOM		=	0x00020000,		// 挑战赛操作房间的权利
	EM_RIGHT_BUYFAMILYCORPSITEM	=	0x00040000,		// 购买家族军团物品的权利
	EM_RIGHT_TASK				=	0x00080000,		// 家族任务
	EM_RIGHT_STARTFAMILYBOSS	=	0x00100000,		// 开启家族副本的权利
	EM_RIGHT_STARTCORPSBOSS		=	0x00200000,		// 代开军团boss副本的权利
};


enum EHomeType
{
	HOME_MAIN				= 0,		// 主屋
	HOME_WUFANG				= 1,		// 武坊
	HOME_GONGFANG			= 2,		// 工坊
	HOME_SHUFANG			= 3,		// 书坊
	HOME_NUM				= 4,
};

#define MAX_ORNAMENT_NUM	10
#define MAX_VEGETABLE_NUM	100

enum EOrganLogType
{
	ORGANLOGTYPE_CORPS	= 0,	// 军团日志
	ORGANLOGTYPE_FAMILY = 1,	// 家族日志
};

#define  MAX_STABILITY_TIME_NUM 20	// 安定度每个等级段最大的时间段数

#define  MAX_STABILITY_LEVEL_NUM 50	// 安定度的等级段数
#define  MAX_FAMILYTASK_NUM 200	// 家族任务的最大数量
#define  FAMILY_INITIAL_STABILITY 200 

#define  MAX_STABILITY_REDSTONE_NUM 20 // 每一等级安定度对应内丹的最大分段数