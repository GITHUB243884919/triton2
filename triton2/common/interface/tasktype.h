#pragma  once
//Enums
//
//enum ETask
//{
//	ERROR_TASK_SUCCESS				=	1300,
//	ERROR_TASK_HASACCEPT			=	1301,		// 任务已接受
//	ERROR_TASK_HASDONE				=	1302,		// 任务已完成
//	ERROR_TASK_OUTOFSPACE			=	1303,		// 任务列表已满
//	ERROR_TASK_DIALOGIDINVALID		=	1304,		// 任务对话ID无效
//	ERROR_TASK_TASKIDINVALID		=	1305,		// 任务ID无效
//	ERROR_TASK_NOTCOMPLETE			=	1306,		// 任务还没有完成
//	ERROR_TASK_NOSPACE				=	1307,		// 没有足够的包裹空间
//	ERROR_TASK_ACCEPTINVALID		=   1308,		// 任务开启条件不满足
//	ERROR_TASK_ABORTINVALID			=   1309,       // 任务不能被放弃
//};
//
//enum ETaskTip
//{
//	TASKTIP_NONE					= 0,			// 和任务没有任何关系
//	TASKTIP_ACTIVE					= 1,			// 有任务可以接
//	TASKTIP_BEGIN					= 2,			// 有任务可以交( 没有完成 )
//	TASKTIP_COMPLETE				= 3				// 有任务可以交( 已经完成 )
//};
//
//
//enum ETaskProgress
//{
//	TASK_BEGIN						= 0,			// 任务已经接受，进行中
//	TASK_COMPLETE					= 1,			// 任务已经完成，但还没有交
//	TASK_END						= 2,			// 任务已经结束
//	TASK_ACTIVE						= 3,			// 任务已经激活
//};

#define ONTIMER_REFRESH_TASK		(30*60)UL		// 半小时刷新个人发布
#define RECV_AWARD_LIMITS_TIME		(12*60*60)UL	// 12小时限定，不领取则个人发布任务奖励消失

enum TaskErrCode
{
	DES_INVITED_LIST_FULL		= 1501,  // 对方的被邀请列表满
	DES_INVITED_THIS			= 1502,  // 已经邀请过这个人这个任务了
};


enum TASK_DEFINE
{
	TASK_DATA_SIZE				= 16,
	TASK_LIST_SIZE				= 30,
	COMPLETE_TASK_BIT_SIZE		= 2500,
	COMPLETE_TASK_NUM			= 1500,
	FAILED_TASK_NUM				= 500,
	PUBLISH_LIST_SIZE			= 50,  
};

//// 任务类型
//enum enTaskType
//{
//	TASKTYPE_MAIN		= 1,	// 主线任务
//	TASKTYPE_LATERAL	,		// 支线任务
//	TASKTYPE_WEST		,		// 西游任务
//	TASKTYPE_TRAGEDY	,		// 劫难任务
//	TASKTYPE_LOOP		,		// 门派任务
//	TASKTYPE_STORAGE	,		// 悬赏任务
//	TASKTYPE_SCROLL		,		// 仙轴任务
//};

// 任务类型
enum enTaskType
{
	TASKTYPE_MAIN		= 0x0001,		// 主线任务
	TASKTYPE_LATERAL	= 0x0002,		// 支线任务
	TASKTYPE_WEST		= 0x0004,		// 西游任务
	TASKTYPE_TRAGEDY	= 0x0008,		// 劫难任务
	TASKTYPE_LOOP		= 0x0010,		// 门派任务
	TASKTYPE_STORAGE	= 0x0020,		// 悬赏任务
	TASKTYPE_PUBLISH	= 0x0024,		// 发布任务
	TASKTYPE_SCROLL		= 0x0040,		// 仙轴任务
	TASKTYPE_MINEMAP	= 0x0080,		// 藏宝图任务
	TASKTYPE_LUCK		= 0x0100,		// 奇缘任务
	TASKTYPE_FAMILY		= 0x0200,		// 家族任务
	TASKTYPE_CORPS		= 0x0400,		// 军团任务
	TASKTYPE_ACTIVITY	= 0x0800,		// 活动任务
	TASKTYPE_NATION		= 0x1000,		// 国家任务
	TASKTYPE_CHAIRMAN	= 0x2000,		// 掌门任务
	TASKTYPE_CAMP		= 0x4000,		// 阵营任务
};

// 劫难任务的状态
enum enTragedyTaskState
{
	TRAGEDY_ACTIVE		   ,    // 激活，可以接受
	TRAGEDY_GOT			   ,	// 已接受（客户端接受按钮灰掉）
	TRAGEDY_COMPLETED	   ,	// 已完成
	TRAGEDY_DISABLED	   ,	// 不可接
};

// 任务颜色
enum enTaskColor
{
	COLOR_WHITE   = 1, //--白
	COLOR_BLUE    = 2, //--蓝
	COLOR_PURPLE  = 3, //--紫
	COLOR_GOLD    = 4, //--金


	COLOR_TOTAL   = 4, //--颜色总数
};

#define TASK_NAME_LENGTH 128
#define TASK_DESC_LENGTH 512

#define MAX_COLLECT_TASKS 10  // 采集NPC关联的任务数量上限

#define MAX_TASK_NUM  65536  // 系统支持的最多任务数

#define MAX_EVENTS_PER_TASK 40

#define TASK_NPC_TALK_LENGTH 512
#define TASK_PLAYER_TALK_LENGTH 512

#define MAX_NPC_TASK_NUM 64

#define SECTION_MAX			32
#define NPC_ACTIVET_TASKS	64
#define DIALOG_OPTION		16
#define ACCEPT_TASK			512

// 最大邀请条数
#define TASK_INVITE_MAX		5

// 最大被邀请条数
#define TASK_INVITED_MAX	10

// 每轮的环任务子任务数量 
#define LOOP_SUB_NUM        10

// 劫难任务总数
#define TRAGEDY_TASK_SIZE	81

// 任务中间对话NPC最大数量
#define MAX_NPC_TALK_NUM	6

enum TaskState
{
	TASK_BEGIN						= 0,			// 任务已经接受，进行中
	TASK_COMPLETE					= 1,			// 任务已经完成，但还没有交
	TASK_END						= 2,			// 任务已经结束
	TASK_ACTIVE						= 3,			// 任务已经激活
	TASK_FAILED						= 4,			// 任务已失败
	TASK_TOEND						= 5,			// 即将结束（服务器内部使用）
};

typedef struct 
{
	unsigned short		mTaskID;			// 任务ID
	unsigned short		mTaskProgress;		// 任务进度
	int					mTimer;				// 任务时限, -1 表示没有计时，0 表示计时到
	unsigned short		mTaskData[TASK_DATA_SIZE]; //任务数据

	unsigned short		mLoopTaskID;		// 环任务ID
	unsigned short      mCompletedLoops;    // 已完成轮数
	unsigned short      mLoopID;            // 环ID
} TaskData;


enum EButtonType 
{
	TYPE_WINDOW = 1,				// 弹出一个新窗口
	TYPE_FUNC ,					// 代表某种功能
};

typedef enum EWindowType 
{
	WINDOW_NONE	  = 0,			// 无意义
	WINDOW_COMMON,				// 普通
	WINDOW_AWARD,				// 奖励
	WINDOW_ACCEPT,				// 任务接受
	WINDOW_ENDTALK,				// 结束对话窗体
}emTaskWindowType;

enum EButttonFunc 
{
	FUNC_RETURN = 1,			// 返回
	FUNC_CLOSE,					// 关闭
	FUNC_EXTANDTASK ,			// 发放任务
	FUNC_CALLBACKTASK ,			// 完成任务
	FUNC_ERRORANSWER,			// 答题的错误选项 
	FUNC_UI_UPEQT,				// 精炼界面 (与NPC对话弹出功能界面)
	FUNC_UI_PRODUCE,			// 生产界面（游戏中按P）
	FUNC_UI_GUIRD,				// 通天葫芦界面（游戏中按B弹出包裹，再左键点击相关图标打开）
	FUNC_UI_STOVE,				// 炼丹炉界面（游戏中按B弹出包裹，在右键点击相关图标打开）
	FUNC_ENDTALK,				// 结束和NPC的对话
	FUNC_UI_SKILL,				// 技能界面
};

enum ACCEPT_TASK_RETURN_CODE 
{ 
	ACCEPT_OK = 0, 
	LEVEL_FAIL = -1,
	MONEY_FAIL = -2,
	METIER_FAIL = -3,
	SEX_FAIL = -4,
	RE_GET = -5 //不能重复接
};

enum PLAYER_ATTRIB_TYPE
{
	PLAYER_MONEY  = 1,
	PLAYER_EXPR   = 2,
	PLAYER_LEVEL  = 3,
	PLAYER_REGION = 4,
	PLAYER_SEX    = 5,
	PLAYER_MEITRE = 6,
	PLAYER_CAMP	 = 7,
};



enum ETaskCtrl
{
	CTRL_REPEATEABLE		= 1,    // 任务成功后可重复
	CTRL_CANCELABLE		= 2,    // 任务可以被放弃
	CTRL_RETRYABLE		= 3,    // 任务失败后可以重试
	CTRL_DIRECTCOMPLETE	= 4,	// 任务直接完成
	CTRL_FAILONKILLED		= 5,	// 玩家被杀死则认为任务失败
	CTRL_SENDNPC,				// 护送NPC模板ID
	CTRL_MAXGIRD,				 // 最大格子数
	CTRL_NAME,					 // 任务名字
	CTRL_OFF_FAIL,				// 是否下线就失败
	CTRL_TASKGROUP,				// 任务组ID
	CTRL_ISAUTOABORT,			// 是否失败后自动放弃
	CTRL_ISSHARE,				// 是否完成后分享同屏玩家
	CTRL_TIMEOUT_TYPE,			// 限时任务时间到以后的操作
	CTRL_TIMELMT_TYPE,			 // 限时任务类型 物理还是游戏
	CTRL_IS_DAILY_FAILED=15,	 // 是否跨天失败
	CTRL_IS_TEAMLOOP=16,		// 是否组队环任务
	CTRL_TEAM_MEMBER=17,		// 队伍人数限制
	CTRL_FINISH_TIME=18,		// 交任务的时间限制
	CTRL_REFRESH_AWARD=19,		// 任务是否可以刷新奖励
	CTRL_TASK_ENDTIME=20,		// 该任务还有多久可重新做
	CTRL_TASK_PUBLISHTYPE = 21,   // 任务发布类型
	CTRL_TASK_IS_FINAL_CHILD = 22, //是否为该类子任务的最后一个
};

enum ETaskKind 
{
	KIND_SEEKITEM = 1,    //寻物, 
	KIND_SENDMAIL = 2,    //送信, 
	KIND_COLLECT = 3,     //收集， 
	KIND_ZAYI = 4,        //杂役, 
	KIND_MAKEITEM = 5,    //制造, 
	KIND_XIULIAN = 6,     //修炼， 
	KIND_KILLORGE = 7,    //杀怪
	KIND_REVENGE = 8      //报仇, 
};

enum ETaskInviteRet
{
	ERROR_CANT_ACCEPT ,  // 0=条件不足 
	ERROR_GOT,			 // 1=已经在身上 
	ERROR_COMPLETED,	 // 2=已经完成
	ERROR_OFFLINE,		 // 3=对方不在线
	// ERROR_DES_BAGFULL,	 // 4=对方包满了 而这个任务需要发放任务物品

};


class region
{
public:
	unsigned short mMapID;
	unsigned short mX;
	unsigned short mY;

	region(unsigned short map_id, unsigned short X, unsigned short Y):mMapID(map_id),mX(X),mY(Y){}
	region():mMapID(0),mX(0),mY(0){}
};



// ********************************************************************** //
// COgreTaskList
// ********************************************************************** //

class COgreTaskList
{
public:
	COgreTaskList( ) : mOgreID( 0 ), mNumber( 0 )
	{
		memset( mTaskID,0,sizeof(mTaskID) );
	}

public:
	unsigned int	mOgreID;
	unsigned short	mNumber;
	unsigned short	mTaskID[ 64 ];
};

// ********************************************************************** //
// CTaskDropTable
// ********************************************************************** //

class CTaskDropTable
{
public:
	unsigned int		mOgreID;				// 怪物ID
	unsigned short		mNumber;
	unsigned short		mTaskID[ 64 ];
};

class CTaskCompleteOgreItem
{
public:
	int		mID;
	int		mNum;
public:
	CTaskCompleteOgreItem( int vID = 0, int vNum = 0 )
		: mID(vID), mNum(vNum)
	{
	}
};

//用于存储已经打开过的可选奖励的模板ID和数量
class CAwardOptions
{
public:
	unsigned int mItemID;
	unsigned short mNum;
};

//记录自动接的任务的列表
class CTaskAutoGet
{
public:
	unsigned short	mTaskID;
	unsigned short	mMapID;
	unsigned short	mX;
	unsigned short	mY;
	unsigned short	mH;
	unsigned short	mW;
public:
	bool CheckInRegion(region vRegion)
	{
		return (vRegion.mMapID == mMapID) && (vRegion.mX >= mX) && vRegion.mX <= (mX + mW) && vRegion.mY >= mY &&  vRegion.mY <= (mY + mH);
	}
};



class CNpcTaskList
{
public:
	CNpcTaskList( ) : mExtendNumber( 0 ), mCallbackNumber( 0 )
	{
	}

	void clear()
	{
		mNpcID  = 0;
		mExtendNumber = 0;
		mCallbackNumber = 0;
	}

public:
	int   	mNpcID;
	unsigned short	mExtendNumber;
	unsigned short	mExtendTaskID[ MAX_NPC_TASK_NUM ];
	//short			mExtendTaskStatus[ 64 ];
	unsigned short	mCallbackNumber;
	unsigned short	mCallbackTaskID[ MAX_NPC_TASK_NUM ];
	//short			mCallbackTaskStatus[ 64 ];
	unsigned short  mLastTip;
};

// *** 事件规划：
// 事件ID 共计32bits
// 第一位空余
// NO.2 bit 是否全局唯一事件(不能同时接身上，或者干脆全部任务里只能用1次，比如精炼等，此类事件记录到玩家身上 独立编ID)MASK=0x8000
#define EVENT_MASK_GLOBAL       0x40000000
// NO.3 bit 是否需要记录的事件 MASK=0x4000
#define EVENT_MASK_RECORD       0x20000000
// NO.4 bit 是否监视的事件(为了加快速度，引入监视机制，
//          目前只有杀怪和搜集有本机制，当监视池中所有的事件响应次数都满足的时候，才会调用update接口) MASK=0x2000
#define EVENT_MASK_WATCH        0x10000000
// NO.5 bit 该事件发生的时候 强制通知客户端刷新任务
#define EVENT_MASK_NOTIFY       0x08000000
// NO.6 bit 该事件的Param是不是0，是0的话该位是1
#define EVENT_MASK_PARAM0       0x04000000
// NO.7 bit 该事件要在任务上面记录发生次数
#define EVENT_MASK_REGINTASK    0x02000000
// NO.8 bit 没用的 不用处理
#define EVENT_MASK_NOUSE		0x01000000

// 后16bits 是真正的事件ID
#define EVENT_MASK_ID           0x0000FFFF

enum EEventForTask  // 32位数，最高位表示是否需要记录的事件
{
	// 不需要记录的事件
	// 需要监视的事件
	EVENT_ITEMCHANGE	= 0x18000001,             // 物品变化 Param是物品模板ID  ★强制刷新客户端任务
	EVENT_OGREDIE		= 0x18000002,             // 怪物死亡 Param是怪物ID  【监视】★强制刷新客户端任务
	// 不需要监视的事件
	EVENT_CHANGEMONEY	= 0x04000001,             // 金钱变化 Param是0
	EVENT_LEVELCHANGE	= 0x04000002,             // 等级变化 Param是0
	EVENT_MOVEREGION	= 0x04000003,             // 玩家移动 Param是0 无用
	EVENT_PLAYERDIE		= 0x04000004,             // 玩家死亡 Param是  无用
	EVENT_NPCDIE		= 0x04000005,             // NPC死亡  无用
	EVENT_METIERCHANGE	= 0x04000006,			    // 玩家职业改变  Param是0
	EVENT_TASKCHANGE	= 0x04000007,			    // 玩家身上任务改变 Param是物品任务ID
	EVENT_KILLLEVELOGRE	= 0x04000008,				// 按等级杀怪
	EVENT_BUFFCHANGED	= 0x04000009,				// 玩家BUFF已变化

	// 需要记录的事件(不论有无相关任务都记录  慎重!!) (才能在lua里check和clear) (必须是保证身上有相关任务才能走这里！)
	EVENT_USEITEM			  = 0x20000001,		// 玩家使用道具 Param是物品ID
	EVENT_SENDNPCCOMPLETE	  = 0x20000002,		// 玩家护送NPC到达指定位置 Param是NPC模板ID
	EVENT_TALKCOMPLETE		  = 0x20000003,		// 对话完成  Param是NPC模板ID

	// 全局唯一事件(支持快速访问，最多32个 也需要记录，但是身上有相关任务才记录) (clear的时候要清除)
	EVENT_UPEQT				  = 0x64000001,		// 精炼某种物品（在精炼师处使用了一个精炼石）
	EVENT_PRODUCE_WEAPON      = 0x64000002,		// 生产了武器（使用青龙或者朱雀精魂生产了任意一件武器）
	EVENT_PRODUCE_PREVENT	  = 0x64000003,		// 生产了防具（使用玄武或者白虎精魄生产了任意一件防具）
	EVENT_PRODUCE_DRUG		  = 0x64000004,		// 生产了药品（使用风羽花，仙参，佛莲叶，灵芝，雪莲生产了任意一种药品）
	EVENT_STOVE				  = 0x64000005,		// 炼制某种物品——将某件装备放入炼丹炉中，增加这个炼丹炉的化金值（现在可能叫元气值）
	EVENT_JEWEL				  = 0x64000006,		// 镶嵌宝石（在通天葫芦界面中，将宝石镶嵌到装备上）
	EVENT_CHARSTONE			  = 0x64000007,		// 镶嵌符文石(在通天葫芦界面中，将符文灵石镶嵌到装备上)
	EVENT_HOLE				  = 0x64000008,		// 为某种物品打孔（在通天葫芦界面中，用化金丹为某个物品打孔）
	EVENT_SKILL				  = 0x64000009,		// 学习技能
	EVENT_ENDLOOP			  = 0x6400000A,		// 完成一轮环任务

	// 发生时 需要在任务上面记录发生次数的事件
	EVENT_COMPLETESTORAGETASK = 0x02000001,		// 完成了一次库任务
	EVENT_KILLFOREIGNER		  = 0x02000002,		// 杀死外国人
	EVENT_COMPLETE_LOOP		  = 0x02000003,		// 完成环任务
	EVENT_COMPLETE_QUESTION	  = 0x02000004,		// 完成每日答题

	// 临时用的事件代码 用于从Lua传递特殊信息到C++  非通用逻辑 如果有特殊逻辑 则在注册事件的地方处理
	EVENT_PROTECT_FLAG		  = 0x01000001,		// 守旗任务 param1是NPC模板ID
};

#define  MAX_SAVE_EVENT 20

// 最多的路点数量
#define MAX_WAY_POINTS 100

// 玩家身上的库任务列表数量
#define MAX_STORAGETASK_LIST_NUM 10
#define STORAGE_SIZE			 1000

// 玩家身上最多的库任务
#define MAX_ACCEPT_STORAGETASKS 10

enum ETaskRefreshType 
{
	REFRESH_DAY  = 1, //-- 每天刷
	REFRESH_BACK = 2, //-- 交完任务刷
	REFRESH_RMB  = 3, //--用RMB道具刷
};

#define _MAX_INT 1000000000

#define MAX_LOOP_SUB_TASKS		    100		// 每轮门派任务最大子任务数

enum ETaskConditions
{ 
	CONDITION_OGREDIE			= 1, // 怪物死了
	CONDITION_ITEM				= 2, // 物品变化（可能增加或减少）
	// 以下这三个和上面的有实质的不同 负数说明是替代模板ID作为标志的 而不是真正的条件 实际下面这3个的条件还是第一个
	// 这种设计不太好 不清晰 不过还算灵活
	CONDITION_LEVELOGRE			= -1,	// 等级杀怪
	CONDITION_KILL_FOREIGNER	= -2,	// 杀死外国人
	CONDITION_COMPLETE_STORAGE	= -3,	// 完成悬赏
	CONDITION_COMPLETE_LOOP		= -4,	// 完成环任务
	CONDITION_COMPLETE_QUESTION	= -5,	// 完成每日答题
};

// 和NPC对话状态
enum ENpcTalkState
{
	NONE_TALKED		= 1,
};

// 任务组ID定义  用于组策略  (数值不能改！否则就乱掉了！)
enum enTaskGroups
{
	TASKGROUP_START					=   0x00010000,		// 原始版的任务组ID起始
	TASKGROUP_FAMILY_HONNER			=	0x00010001,		// 家族荣耀任务
	TASKGROUP_FAMILY_KILL			=	0x00010002,		// 家族除妖任务
	TASKGROUP_FAMILY_SEND			=	0x00010003,		// 家族护送任务
	TASKGROUP_FAMILY_CHALANGE		=	0x00010004,		// 家族挑战任务
	TASKGROUP_CORPS_IN				=	0x00020001,		// 军团守边任务
	TASKGROUP_CORPS_OUT				=	0x00020002,		// 军团扰边任务
	TASKGROUP_CORPS_CHALANGE		=	0x00020003,		// 军团挑战任务
	TASKGROUP_MINEMAP				=	0x00040001,		// 藏宝图任务
	TASKGROUP_ACTIVITY				=	0x00080001,		// 活动类任务
	TASKGROUP_SAVEFLAG1				=	0x00100001,		// 守旗1
	TASKGROUP_SAVEFLAG2				=	0x00100002,		// 守旗2
	TASKGROUP_SAVEFLAG3				=	0x00100003,		// 守旗3
	TASKGROUP_SAVEFLAG4				=	0x00100004,		// 守旗4
	TASKGROUP_COUNTRY1				=	0x00200001,		// 兵部建设
	TASKGROUP_COUNTRY2				=	0x00200002,		// 吏部建设
	TASKGROUP_COUNTRY3				=	0x00200003,		// 礼部建设
	TASKGROUP_COUNTRY4				=	0x00200004,		// 刑部建设
	TASKGROUP_COUNTRY5				=	0x00200005,		// 工部建设
	TASKGROUP_COUNTRY6				=	0x00200006,		// 户部建设
	TASKGROUP_COUNTRY7				=	0x00200007,		// 毛遂自荐
	TASKGROUP_COUNTRY_ALL			=	0x00200008,		// 所有建设任务

	TASKGROUP_CAMP_TF1				=	0x00400001,		// 阵营讨伐任务1
	TASKGROUP_CAMP_TF2				=	0x00400002,		// 阵营讨伐任务2
	TASKGROUP_CAMP_RB				=	0x00400003,		// 阵营扰边任务
	TASKGROUP_CAMP_SB				=	0x00400004,		// 阵营守边任务
	TASKGROUP_CAMP_SQ1				=	0x00400005,		// 阵营守旗1
	TASKGROUP_CAMP_SQ2				=	0x00400006,		// 阵营守旗2
	TASKGROUP_CAMP_SQ3				=	0x00400007,		// 阵营守旗3
	TASKGROUP_CAMP_SQ4				=	0x00400008,		// 阵营守旗4

	// 新加的活动分类 暂时临时加类型 以后彻底整理这块
	TASKGROUP_XUNXIAN1 = 524290, // 活动寻仙1
	TASKGROUP_XUNXIAN2 = 524291, // 活动寻仙2
	TASKGROUP_XUNXIAN3 = 524292, // 活动寻仙3
	TASKGROUP_XUNXIAN4 = 524293, // 活动寻仙4
	TASKGROUP_SISHEN1	=	524294	,
	TASKGROUP_SISHEN2	=	524295	,
	TASKGROUP_SISHEN3	=	524296	,
	TASKGROUP_SISHEN4	=	524297	,
	TASKGROUP_SISHEN5	=	524298	,
	TASKGROUP_SISHEN6	=	524299	,
	TASKGROUP_SISHEN7	=	524300	,
	TASKGROUP_SISHEN8	=	524301	,

	TASKGROUP_SPRING1	=	524302	,
	TASKGROUP_SPRING2	=	524303	,
	TASKGROUP_SPRING3	=	524304	,
	TASKGROUP_SPRING4	=	524305	,
	TASKGROUP_SPRING5	=	524306	,
	TASKGROUP_SPRING6	=	524307	,
	TASKGROUP_SPRING7	=	524308	,
	TASKGROUP_SPRING8	=	524309	,
	TASKGROUP_SPRING9	=	524310	,
	TASKGROUP_SPRING10	=	524311	,
	TASKGROUP_SPRING11	=	524312	,
	TASKGROUP_SPRING12	=	524313	,

	TASKGROUP_KILLZHUREN1	=	524314	,  // 杀死主人1
	TASKGROUP_KILLZHUREN2	=	524315	,  // 杀死主人2
	TASKGROUP_KILLZHUREN3	=	524316	,  // 杀死主人3
	TASKGROUP_KILLZHUREN4	=	524317	,  // 杀死主人4

	TASKGROUP_KILLWEIBIN1	=	524318	,  // 杀死卫兵1
	TASKGROUP_KILLWEIBIN2	=	524319	,  // 杀死卫兵2
	TASKGROUP_KILLWEIBIN3	=	524320	,  // 杀死卫兵3
	TASKGROUP_KILLWEIBIN4	=	524321	,  // 杀死卫兵4

	TASKGROUP_CITAN1			=	524322	,  // 刺探任务1
	TASKGROUP_CITAN2		=	524323	,  // 刺探任务2
	TASKGROUP_CITAN3			=	524324	,  // 刺探任务3
	TASKGROUP_CITAN4			=	524325	,  // 刺探任务4

	TASKGROUP_LIULI1			= 524326, // 琉璃盏任务1
	TASKGROUP_LIULI2			= 524327, // 琉璃盏任务2
	TASKGROUP_LIULI3			= 524328, // 琉璃盏任务3
	TASKGROUP_LIULI4			= 524329, // 琉璃盏任务4
	TASKGROUP_LIULI5			= 524330, // 琉璃盏任务5
	TASKGROUP_LIULI6			= 524331, // 琉璃盏任务6
	TASKGROUP_LIULI7			= 524332, // 琉璃盏任务7
	TASKGROUP_LIULI8			= 524333, // 琉璃盏任务8

	TASKGROUP_MARUI				= 530000,	// 53W以后的 马锐自由分配
	TASKGROUP_YUEGONG1			= 530014,   // 月宫使者1
	TASKGROUP_YUEGONG2			= 530015,   // 月宫使者2
	TASKGROUP_YUEGONG3			= 530016,   // 月宫使者3
	TASKGROUP_YUEGONG4			= 530017,   // 月宫使者4
	
	TASKGROUP_NEW_LIULZ1		=  530019, //  新琉璃盏1
	TASKGROUP_NEW_LIULZ2		=  530020, //  新琉璃盏2
	TASKGROUP_NEW_LIULZ3		=  530021, //  新琉璃盏3
	TASKGROUP_NEW_LIULZ4		=  530022, //  新琉璃盏4
	TASKGROUP_NEW_LIULZ5		=  530023, //  新琉璃盏5
	TASKGROUP_NEW_LIULZ6		=  530024, //  新琉璃盏6

	TASKGROUP_NEWYEAR_SZ1		= 530026, // 新年使者1
	TASKGROUP_NEWYEAR_SZ2		= 530027, // 新年使者2
	TASKGROUP_NEWYEAR_SZ3		= 530028, // 新年使者3
	TASKGROUP_NEWYEAR_SZ4		= 530029, // 新年使者4

	TASKGROUP_QMJ1				= 530043, // 清明节1
	TASKGROUP_QMJ2				= 530044, // 清明节2
	TASKGROUP_QMJ3				= 530045, // 清明节3
	TASKDAILY_MOBAI				= 530049, // 每日膜拜

	TASKGROUP_BALANCE1			= 530054,  //平衡活动1
	TASKGROUP_BALANCE2			= 530055,  //平衡活动2
	TASKGROUP_BALANCE3			= 530056,  //平衡活动3
	TASKGROUP_BALANCE4			= 530057,  //平衡活动4


	TASKGROUP_HAVOC_HEAVEN1		= 530063,  // 72变闹天宫任务1
	TASKGROUP_HAVOC_HEAVEN2		= 530065,  // 72变闹天宫任务1
};

// 最多任务组数量
#define  MAX_TASKGROUP_NUM	200

// 通用条件检查 条件码
enum enCommonCheckCode
{
	CHECKTYPE_ISFAMILYMEMBER	=	1	,		// 是否家族成员
	CHECKTYPE_ISFAMILYHEAD		=	2	,		// 是否家族族长
	CHECKTYPE_ISCORPSMEMBER		=	3	,		// 是否军团成员
	CHECKTYPE_HASTITLE			=	4	,		// 是否有某称号   参数1是称号ID
	CHECKTYPE_BUFF				=	5	,		// 检查身上是否有某个BUFF
	CHECKTYPE_COUNTRY			=	6	,		// 检查国家
	CHECKTYPE_PUBLISH			=	7	,		// 检查发布 且可接大于0
	CHECKTYPE_FAMILY_NPC		=	8	,		// 检查所在家族占领了NPC
	CHECKTYPE_CORPS_NPC			=	9	,		// 检查所在军团占领了NPC
	CHECKTYPE_ACTIVITY			=	10	,		// 检查指定活动开启
	CHECKTYPE_REG_DATE			=	11	,		// 检查注册日期早于...
	CHECKTYPE_QUES_NUM			=	12	,		// 检查答题次数
	CHECKTYPE_HYDRANGEASTATUS	=	13	,		// 检查绣球状态
	CHECKTYPE_IN_CAMP			=	14	,		// 检查服务器阵营
	CHECKTYPE_ONLINESECS		=	15	,		// 检查累计在线时间 秒
	CHECKTYPE_WEEKNATION		=	16	,		// 弱国任务
	CHECKTYPE_IN_MY_SELF		=	17	,		// 检查只能在本阵营接任务
	CHECKTYPE_IS_NATIONKILL		=	18	,		// 检查是否在国探期间
	CHECKTYPE_IS_LUANWU			=	19	,		// 检查是否在乱舞期间
	CHECKTYPE_IS_MASTER			=	20	,		// 检查是否师傅
	CHECKTYPE_IS_TEAMHEAD		=	21	,		// 检查是否队长
	CHECKTYPE_TEAM_MEMBER		=	22	,		// 检查队伍人数
	CHECKTYPE_ISVIP_PLAYER		= 	23	,		// 检查是否是VIP
	CHECKTYPE_ISSWORNED_PLAYER	= 	24	,		// 检查是否已经结拜
};

// 通用奖励码
enum enCommonAwardCode
{
	AWARDTYPE_TELANT				=	1	,		// 奖励修为点(参数1：点数)
	AWARDTYPE_FAMILY_HONOR			=	2	,		// 奖励个人家族荣耀值
	AWARDTYPE_FAMILY_MONEY			=	3	,		// 奖励家族金钱
	AWARDTYPE_FAMILY_CONTRIBUTE		=	4	,		// 奖励个人家族贡献度
	AWARDTYPE_CORPS_GLORY			=	5	,		// 奖励个人军团荣耀值
	AWARDTYPE_CORPS_MONEY			=	6	,		// 奖励军团金钱
	AWARDTYPE_CORPS_CONTRIBUTE		=	7	,		// 奖励个人军团贡献度
	AWARDTYPE_TITLE					=	8	,		// 奖励称号
	AWARDTYPE_HONOR					=	9	,		// 奖励功勋
	AWARDTYPE_BINDYINGYUANBAO		=	10	,		// 奖励绑定的银元宝
	AWARDTYPE_COUNTRYBUILD			=	11	,		// 奖励国家建设度
	AWARDTYPE_UPWIS					=	12	,		// 上报才华
	AWARDTYPE_WISEXP				=	13	,		// 奖励才华经验
	AWARDTYPE_CLEARHYDRANGEASTATUS	=	14	,		// 清除绣球状态
	AWARDTYPE_COMBAT_FAME			=	15	,		// 奖励竞技声望
	AWARDTYPE_REPE_FAME				=	16	,		// 奖励副本声望
	AWARDTYPE_COMMON_FAME			=	17	,		// 奖励历练声望
	AWARDTYPE_CORPS_COLLECTACTIVE	=	18	,		// 奖励军团（采集）活跃度

};

// 客户端请求repeated字段限制
enum enClientLimit
{
	// 客户端一次请求 活动任务种类数 上限
	CLIENTREQUESTMAX_GETEVERYDAYTASK	=	20,
	// 客户端一次请求 tips 上限
	CLIENTREQUESTMAX_GETTIPS			=	300,
};

// 护送NPC跨地图后等待时间 （秒）
#define SEND_NPC_REDIRECT_WAIT_TIME	3

// 为了提速 或做特殊处理标记 定义特殊条件枚举
enum enTaskExFlags
{
	EXFLAG_LEVELKILLOGRE				=	0x0001,	// 按等级杀怪
	EXFLAG_KILL_FOREIGNER				=	0x0002,	// 杀外国人
	EXFLAG_COMPLETE_STORAGE				=	0x0004, // 完成悬赏
	EXFLAG_PROTECT_FLAG_TASK			=	0x0008, // 守旗任务
	EXFLAG_COMPLETE_LOOP				=	0x0010, // 完成环任务
	EXFLAG_COMPLETE_QUESTION			=	0x0020, // 完成答题任务
};

// 鉴于模板ID不可能是负数，也就是说最高位永远不可能为1，所以制定一堆特殊的内部ID 用于实现特殊的需要计数的功能
enum enSpecialID
{
	SPECIALID_COMPLETESTORAGE			=	0x80000001,	// 完成悬赏任务
	SPECIALID_KILLFOREIGNER				=	0x80000002,	// 杀死外国人
	SPECIALID_COMPLETELOOPTASK			=	0x80000003,	// 完成环任务
	SPECIALID_COMPLETEQUESTION			=	0x80000004,	// 完成每日答题
};

// 用于客户端显示的奖励类型
enum enShowAwardType 
{
	SHOWAWARD_EXP = 1, //奖励经验 (n表示经验值)
	SHOWAWARD_MONEY = 2, //奖励金钱(n表示金钱值)
	SHOWAWARD_ITEM = 3, //奖励物品(id,n)
	SHOWAWARD_TASK = 4, //奖励一个任务(id表示任务ID)
	SHOWAWARD_TRANS = 5, //奖励传送，(id表示目标地图ID，x,y表示目标坐标)
	SHOWAWARD_TELANT=6, //奖励修为点
	SHOWAWARD_HONOR=7, // 奖励功勋
	SHOWAWARD_FAMILY_GLORY		=8,     // 奖励个人家族荣耀值
	SHOWAWARD_FAMILY_MONEY		=9,    // 奖励家族金钱
	SHOWAWARD_FAMILY_CONTRIBUTE	=10,   // 奖励个人家族贡献度
	SHOWAWARD_CORPS_GLORY		=11,  //  奖励个人军团荣耀值
	SHOWAWARD_CORPS_MONEY		=12,   // 奖励军团金钱
	SHOWAWARD_CORPS_CONTRIBUTE	=13,  //  奖励个人军团贡献度
	SHOWAWARD_BUILD_RENSHI		=14,  //  吏部建设度
	SHOWAWARD_BUILD_HUKOU		=15,  //  户部建设度
	SHOWAWARD_BUILD_LIBIN		=16,  //  礼部建设度
	SHOWAWARD_BUILD_GUOFANG		=17,  //  兵部建设度
	SHOWAWARD_BUILD_FAYUAN		=18,  //  刑部建设度
	SHOWAWARD_BUILD_GONGCHENG	=19,  //  工部建设度
	SHOWAWARD_WIS_ARTS			=20,  //  奖励才华经验：文采
	SHOWAWARD_WIS_TACTICS		=21,  //  奖励才华经验：兵法
	SHOWAWARD_WIS_POWER			=22,  //  奖励才华经验：武力
	SHOWAWARD_WIS_WISDOM		=23,  //  奖励才华经验：谋略
	SHOWAWARD_WIS_POLITICS		=24,  //  奖励才华经验：政治
	SHOWAWARD_WIS_EDUCATE		=25,  //  奖励才华经验：修养
	SHOWAWARD_BIND_YUANBAO		=26,  //  奖励绑定元宝
	SHOWAWARD_BUFF				=27,  //  奖励BUFF
	SHOWAWARD_COMBAT_FAME		=28,  //  奖励声望：竞技
	SHOWAWARD_REPE_FAME			=29,  //  奖励声望：副本
	SHOWAWARD_COMMON_FAME		=30,  //  奖励声望：历练
};

// 限时任务超时处理
enum enTimeOutType
{
	TIMEOUT_FAIL		=	0,	// 失败
	TIMEOUT_COMPLETE	=	1,	// 任务完成
};

// 限时方式
enum enTimeLmtType
{
	TIMETYPE_GAME		=	0,	// 游戏时间
	TIMETYPE_PHYSICAL,			// 物理时间
};

// NPC监视器最大容器数量
#define NPC_MONITOR_SIZE	10

// 触发码 对应下面这个操作码的前高16位
enum enActiveCode
{
	NPC_DIE			=	0,	// NPC死亡
	MAX_ACTION_TYPES,		// 总数
};

// 触发操作码 构成：高16位：触发类型  低16位：操作类型
enum enTaskActionType
{
	NPC_DIE_FAIL	=	0x00000001,	// NPC死亡任务失败
};

// 通用获得码 用于获得各种数值
enum enCommonGetCode
{
	GET_WIZARD_POLITICS = 1,	// 玩家才华值-政治
	GET_WIZARD_WISDOM	= 2,	// 玩家才华值-谋略
	GET_WIZARD_ARTS		= 3,	// 玩家才华值-文采
	GET_WIZARD_EDUCATE	= 4,	// 玩家才华值-修养
	GET_WIZARD_TACTICS	= 5,	// 玩家才华值-兵法
	GET_WIZARD_POWER	= 6,	// 玩家才华值-武力
	// 以上6个不能动！LUA里是按这顺序用的 这和excel是有关的
	GET_PLAYER_CAMP		= 7,    // 玩家的阵营
};

// 任务传送类型
enum enTeleType
{
	TELEPORT_COMMON, // 普通
	TELEPORT_INFB,   // 传入副本
	TELEPORT_INMENPAI,   // 传入门派
	TELEPORT_OUTMENPAI,  // 传出门派
};

// 最多支持发布的任务类型数
#define MAX_PUBLISH_TYPES	20

// 任务发布状态
enum enTaskPublishState
{
	STATE_TODAY		= 1,	// 今天有效
	STATE_TOMORROW	= 2,	// 明天有效
};

// 藏宝图任务 每天计数次数
#define CANGBAOTU_DAY_TOTAL 10

// 掌门任务子任务的数量
#define CHAIRMAN_TASK_NUM	10

// 掌门任务状态
enum enChairManTaskState
{
	CHAIRMAN_STATE_ENABLED	= 0,  // 可接
	CHAIRMAN_STATE_GOT		= 1,  // 已接
	CHAIRMAN_STATE_END		= 2,  // 已完成
};


// 额外奖励定义
enum EXTRA_DEFINE
{
	LOOP_TASK = 1,	// 环任务
	TRAGEDY_TASK = 2, // 劫难任务
	CHAIRMAN_TASK = 3, // 掌门任务
	TEAMLOOP_ENDLOOP = 4, // 组队环任务 完成一轮奖励
	TEAMLOOP_TEAMEXTRA = 5, // 组队环任务 组队额外奖励
};

// 发布任务类型
enum emPublisType
{

	NOT_PUBLISH_TASK_TYPE		= 0, // 不是发布任务类型
	CORPS_PUBLISH_TASK   		= 1, // 军团发布
	COUNTRY_PUBLISH_TASK  		= 2, // 国家发布
	FAMILY_PUBLISH_TASK  		= 3, // 家族发布
	CAMP_PUBLISH_TASK 			= 4, // 阵营发布
	UNION_PUBLISH_TASK			= 5, // 军盟发布

	TOTLE_PUBLISH_TYPE			= 5, // 总类型数

	SINGLE_PUBLISH_TASK 		= 6, // 个人发布
};



// 发布任务次数限制
enum emPublishLimits
{
	TODAY_MAX_PUBLISH_MUN		= 10,
	WEEK_MAX_PUBLISH_NUM		= 50,
};

// 发布任务状态
enum emPublishState
{
	UNEXIST_TASK			= 0,
	UNPUBLISH_TASK			= 1,
	PUBLISHED_TASK			= 2,
	ACCEPTED_TASK			= 3,
	RECIEVE_TASK			= 4,
	FINISHED_TASK			= 5,
};

enum emPublishCheck
{
	CHECK_CORPS_LEVEL		= 1,
	CHECK_CORPS_MONEY		= 2,
	CHECK_CORPS_POSITION	= 3,
};


// 个人发布信息
enum emSinglePublish
{
	SINGLE_PUBLISH_TASK_LEVEL	= 70,    // 个人发布需要的最低等级
	SINGLE_OBTAIN_PUBLISH_LEVEL = 70,	 // 接受个人发布的最低等级

	MAX_SINGLE_PUBLISH_BUFFER	= 500,	  // 每个颜色发布列表的最大长度

};