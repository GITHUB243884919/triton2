---------------------------Global Data---------------------------
-- 任务类型描述 (id 是任务ID的第一位) 注意：这个顺序决定了任务日志里的显示顺序！

ETaskShowType =
{
	{ id=0x0001, task_type="#task_type_name_0x0001" },
	{ id=0x0100, task_type="#task_type_name_0x0100" },
	{ id=0x0008, task_type="#task_type_name_0x0008" },
	{ id=0x0004, task_type="#task_type_name_0x0004" },
	{ id=0x1000, task_type="#task_type_name_0x1000" },
	{ id=0x0040, task_type="#task_type_name_0x0040" },
	{ id=0x0002, task_type="#task_type_name_0x0002" },
	{ id=0x0010, task_type="#task_type_name_0x0010" },
	{ id=0x0020, task_type="#task_type_name_0x0020" },
	{ id=0x0024, task_type="#task_type_name_0x0024" },
	{ id=0x0080, task_type="#task_type_name_0x0080" },
	{ id=0x0200, task_type="#task_type_name_0x0200" },
	{ id=0x0400, task_type="#task_type_name_0x0400" },
	{ id=0x0800, task_type="#task_type_name_0x0800" },
	{ id=0x2000, task_type="#task_type_name_0x2000" },
	{ id=0x4000, task_type="#task_type_name_0x4000" },
};

	EButtonType =
	{
		TYPE_WINDOW = 1,				-- 弹出一个新窗口
		TYPE_FUNC = 2,					-- 代表某种功能
	};

	EWindowType =
	{
		WINDOW_COMMON = 1,				-- 普通
		WINDOW_AWARD =2,				-- 奖励
		WINDOW_ACCEPT = 3,			-- 任务接受
	}

	EButttonFunc =
	{
		FUNC_RETURN = 1,				-- 返回
		FUNC_CLOSE = 2,					-- 关闭
		FUNC_EXTANDTASK = 3,			-- 发放任务
		FUNC_CALLBACKTASK = 4,			-- 完成任务
		FUNC_ERRORANSWER = 5,  			-- 答题中错误的选项
		FUNC_END_TALK		= 6,			-- 结束对话
	};
	

ETaskKind =
{
  KIND_SEEKITEM = 1,    --寻物, 
  KIND_SENDMAIL = 2,    --送信, 
  KIND_COLLECT = 3,     --收集， 
  KIND_ZAYI = 4,        --杂役, 
  KIND_MAKEITEM = 5,    --制造, 
  KIND_XIULIAN = 6,     --修炼， 
  KIND_KILLORGE = 7,    --杀怪
  KIND_REVENGE = 8      --报仇, 
};

ETaskCtrl =
{
  CTRL_REPEATEABLE = 1,    --任务成功后可重复
  CTRL_CANCELABLE = 2,     --任务可以被取消
  CTRL_RETRYABLE = 3,      --任务失败后可以重试
  CTRL_DIRECTCOMPLETE = 4, --任务直接完成
  CTRL_FAILONKILLED = 5,   --玩家被杀死则认为任务失败
  CTRL_SENDNPC    = 6,	-- 护送NPC模板ID
  CTRL_MAXGIRD    = 7,      --  最大格子数
  CTRL_NAME	   = 8,     --  任务名字
  CTRL_OFF_FAIL	   = 9,     -- 是否下线任务就失败
  CTRL_TASKGROUP=10,   -- 任务组ID
  CTRL_ISAUTOABORT = 11,  -- 是否失败后自动放弃
  CTRL_ISSHARE = 12,	 -- 是否完成后分享同屏玩家
  CTRL_TIMEOUT_TYPE	= 13,	 -- 限时任务时间到以后的操作
  CTRL_TIMELMT_TYPE = 14,	 -- 限时任务类型 物理还是游戏
  CTRL_IS_DAILY_FAILED=15,	-- 是否跨天失败
  CTRL_IS_TEAMLOOP=16,	 -- 是否组队环任务
  CTRL_TEAM_MEMBER=17,	 -- 队伍人数限制
  CTRL_FINISH_TIME=18,	 -- 交任务的时间限制
  CTRL_REFRESH_AWARD=19,	 -- 是否可以刷新任务奖励
  CTRL_TASK_ENDTIME=20,	-- 任务完成后过多久才能再接任务
  CTRL_TASK_PUBLISHTYPE = 21, --得到任务的发布类型

};

ETaskState =
{
  STATE_NOT_START = -1,    -- 任务还没接呢
  STATE_TASK_BEGIN = 0,    -- 任务已经接受，进行中
  STATE_TASK_COMPLETE = 1, -- 任务已经完成，但还没有交
  STATE_TASK_END = 2,      -- 任务已经结束
  STATE_TASK_ACTIVE = 3,   -- 任务已经激活
};


EPlayerAttr =
{
  PLAYER_MONEY  = 1,
  PLAYER_EXPR   = 2,
  PLAYER_LEVEL  = 3,
  PLAYER_REGION = 4,
  PLAYER_SEX    = 5,
  PLAYER_METIER = 6,
};

EAwardType =
{
	AWARD_EXP = 1, --奖励经验 (n表示经验值)
	AWARD_MONEY = 2, --奖励金钱(n表示金钱值)
	AWARD_ITEM = 3, --奖励物品(id,n)
	AWARD_TASK = 4, --奖励一个任务(id表示任务ID)
	AWARD_TRANS = 5, --奖励传送，(id表示目标地图ID，x,y表示目标坐标)
	AWARD_TELANT=6, --奖励修为点
	AWARD_HONOR=7, -- 奖励功勋
	AWARD_FAMILY_GLORY		=8,     -- 奖励个人家族荣耀值
	AWARD_FAMILY_MONEY		=9,    -- 奖励家族金钱
	AWARD_FAMILY_CONTRIBUTE	=10,   -- 奖励个人家族贡献度
	AWARD_CORPS_GLORY		=11,  --  奖励个人军团荣耀值
	AWARD_CORPS_MONEY		=12,   -- 奖励军团金钱
	AWARD_CORPS_CONTRIBUTE	=13,  --  奖励个人军团贡献度
	
	AWARD_BUILD_RENSHI		=14,  --  吏部建设度
	AWARD_BUILD_HUKOU		=15,  --  户部建设度
	AWARD_BUILD_LIBIN			=16,  --  礼部建设度
	AWARD_BUILD_GUOFANG		=17,  --  兵部建设度
	AWARD_BUILD_FAYUAN		=18,  --  刑部建设度
	AWARD_BUILD_GONGCHENG	=19,  --  工部建设度
	AWARD_WIS_ARTS			=20,  --  奖励才华经验：文采
	AWARD_WIS_TACTICS			=21,  --  奖励才华经验：兵法
	AWARD_WIS_POWER			=22,  --  奖励才华经验：武力
	AWARD_WIS_WISDOM			=23,  --  奖励才华经验：谋略
	AWARD_WIS_POLITICS		=24,  --  奖励才华经验：政治
	AWARD_WIS_EDUCATE		=25,  --  奖励才华经验：修养
	AWARD_BIND_YUANBAO		=26,  --  奖励绑定元宝
	AWARD_BUFF				=27,  --  奖励BUFF
};

EEventForTask =
{
	--  不需要记录的事件
	--  需要监视的事件
	EVENT_ITEMCHANGE  = 0x18000001,             --  物品变化 Param是物品模板ID 【监视】 ★强制刷新客户端任务
	EVENT_OGREDIE     = 0x18000002,             --  怪物死亡 Param是怪物ID  【监视】★强制刷新客户端任务
	--  不需要监视的事件
	EVENT_CHANGEMONEY = 0x04000001,             --  金钱变化 Param是0
	EVENT_LEVELCHANGE = 0x04000002,             --  等级变化 Param是0
	EVENT_MOVEREGION  = 0x04000003,             --  玩家移动 Param是0 无用
	EVENT_PLAYERDIE   = 0x04000004,             --  玩家死亡 Param是  无用
	EVENT_NPCDIE      = 0x04000005,             --  NPC死亡  无用
	EVENT_METIERCHANGE= 0x04000006,			    --  玩家职业改变  Param是0
	EVENT_TASKCHANGE  = 0x04000007,			    --  玩家身上任务改变 Param是物品任务ID
	EVENT_KILLLEVELOGRE= 0x04000008,			-- 按等级杀怪
	EVENT_BUFFCHANGED	= 0x04000009,				-- 玩家BUFF已变化

	--  需要记录的事件(不论有无相关任务都记录) (才能在lua里check和clear)
	EVENT_USEITEM			  = 0x20000001,		--  玩家使用道具 Param是物品ID
	EVENT_SENDNPCCOMPLETE	  = 0x20000002,		--  玩家护送NPC到达指定位置 Param是NPC模板ID
	EVENT_TALKCOMPLETE		  = 0x20000003,		--  对话完成  Param是NPC模板ID

	--  全局唯一事件(支持快速访问，最多32个) (clear的时候要清除,当然也需要记录)
	EVENT_UPEQT				  = 0x64000001,		--  精炼某种物品（在精炼师处使用了一个精炼石）
	EVENT_PRODUCE_WEAPON      = 0x64000002,		--  生产了武器（使用青龙或者朱雀精魂生产了任意一件武器）
	EVENT_PRODUCE_PREVENT	  = 0x64000003,		--  生产了防具（使用玄武或者白虎精魄生产了任意一件防具）
	EVENT_PRODUCE_DRUG		  = 0x64000004,		--  生产了药品（使用风羽花，仙参，佛莲叶，灵芝，雪莲生产了任意一种药品）
	EVENT_STOVE				  = 0x64000005,		--  炼制某种物品——将某件装备放入炼丹炉中，增加这个炼丹炉的化金值（现在可能叫元气值）
	EVENT_JEWEL				  = 0x64000006,		--  镶嵌宝石（在通天葫芦界面中，将宝石镶嵌到装备上）
	EVENT_CHARSTONE			  = 0x64000007,		--  镶嵌符文石(在通天葫芦界面中，将符文灵石镶嵌到装备上)
	EVENT_HOLE				  = 0x64000008,		--  为某种物品打孔（在通天葫芦界面中，用化金丹为某个物品打孔）};
	EVENT_SKILL				  = 0x64000009,		-- 学习技能
	EVENT_ENDLOOP			  = 0x6400000A,		-- 完成一轮环任务

	-- 发生时 需要在任务上面记录发生次数的事件
	EVENT_COMPLETESTORAGETASK 	  = 0x02000001,		-- 完成了一次库任务	
	EVENT_KILLFOREIGNER		  = 0x02000002,		-- 杀死外国人
	EVENT_COMPLETE_LOOP		  = 0x02000003,		-- 完成环任务
	EVENT_COMPLETE_QUESTION	  = 0x02000004,		-- 完成每日答题
	
	-- 临时用的事件代码 用于从Lua传递特殊信息到C++  非通用逻辑 如果有特殊逻辑 则在注册事件的地方处理
	EVENT_PROTECT_FLAG			  = 0x01000001;		-- 守旗任务 param1是NPC模板ID
};

	
MAX_NPC_TASKS = 128; -- NPC身上最多任务数量
STORAGE_SIZE  = 10; -- 库任务每次抽取数量
TRAGEDY_TASK_SIZE =81; -- 劫难任务数量

LOOP_SIZE  = 10; -- 环务每次抽取数量

LOOP_START=0;
STORAGE_START=100;
TRAGEDY_START=200; -- 劫难任务是200开头的描述文件
CHAIRMAN_SIZE  = 10; -- 掌门任务每次抽取数量

ETaskTip=
{
	TASKTIP_NONE					= 0,			-- 和任务没有任何关系
	TASKTIP_ACTIVE					= 1,			-- 有任务可以接
	TASKTIP_BEGIN					= 2,			-- 有任务可以交( 没有完成 )
	TASKTIP_COMPLETE				= 3,				-- 有任务可以交( 已经完成 )
	TASKTIP_LOOPTYPE				= 4,			-- have loop task
};

ETaskColor=
{
	WHITE = 1, --白
	BLUE   = 2, --蓝
	PURPLE= 3, --紫
	GOLD   = 4, --金
};

ETaskRefreshType = 
{
	DAY  = 1, -- 每天刷
	BACK= 2, -- 交完任务刷
	RMB  = 3, --用RMB道具刷
}

-- 任务类型
ETaskType =
{
	TASKTYPE_MAIN		= 0x0001,		-- 主线任务
	TASKTYPE_LATERAL	= 0x0002,		-- 支线任务
	TASKTYPE_WEST		= 0x0004,		-- 西游任务
	TASKTYPE_TRAGEDY	= 0x0008,		-- 劫难任务
	TASKTYPE_LOOP		= 0x0010,		-- 门派任务
	TASKTYPE_STORAGE	= 0x0020,		-- 悬赏任务
	TASKTYPE_PUBLISH	= 0x0024,		-- 发布任务
	TASKTYPE_SCROLL	= 0x0040,		-- 仙轴任务
	TASKTYPE_MINEMAP	= 0x0080,		-- 藏宝图任务
	TASKTYPE_LUCK		= 0x0100,		-- 奇缘任务
	TASKTYPE_FAMILY	= 0x0200,		-- 家族任务
	TASKTYPE_CORPS	= 0x0400,		-- 军团任务
	TASKTYPE_ACTIVITY	= 0x0800,		-- 活动任务
	TASKTYPE_NATION	= 0x1000,		-- 国家任务
	TASKTYPE_CHAIRMAN	= 0x2000,		-- 掌门任务
	TASKTYPE_CAMP		= 0x4000,		-- 阵营任务
};

	
ISDEBUG = 1;  -- DEBUG日志开关

MAX_INT = 1000000000;
GET_PLAYER_CAMP		= 7;

-- 任务组ID定义  用于组策略  (数值不能改！否则就乱掉了！)
enTaskGroups =
{
	TASKGROUP_START				=   0x00010000,		-- 原始版的任务组ID起始

	TASKGROUP_FAMILY_HONNER		=	0x00010001,		-- 家族荣耀任务
	TASKGROUP_FAMILY_KILL			=	0x00010002,		-- 家族除妖任务
	TASKGROUP_FAMILY_SEND			=	0x00010003,		-- 家族护送任务
        TASKGROUP_FAMILY_CHALANGE              =       0x00010004,             -- 家族挑战任务

	TASKGROUP_CORPS_IN			=	0x00020001,		-- 军团守边任务
	TASKGROUP_CORPS_OUT			=	0x00020002,		-- 军团扰边任务
        TASKGROUP_CORPS_CHALANGE                =       0x00020003,             -- 军团挑战任务 
	TASKGROUP_MINEMAP				=	0x00040001,		-- 藏宝图任务
};

-- 通用条件检查 条件码
enCommonCheckCode =
{
	CHECKTYPE_ISFAMILYMEMBER	=	1	,		-- 是否家族成员
	CHECKTYPE_ISFAMILYHEAD		=	2	,		-- 是否家族族长
	CHECKTYPE_ISCORPSMEMBER	=	3	,		-- 是否军团成员
	CHECKTYPE_HASTITLE			=	4	,		--  是否有某称号   参数1是称号ID
	CHECKTYPE_BUFF			=	5	,		-- 检查身上是否有某个BUFF
	CHECKTYPE_COUNTRY			=	6	,		-- 检查国家
	CHECKTYPE_PUBLISH			=	7	,		-- 检查发布 且可接大于0
	CHECKTYPE_FAMILY_NPC		=	8	,		-- 检查所在家族占领了NPC
	CHECKTYPE_CORPS_NPC		=	9	,		-- 检查所在军团占领了NPC
	CHECKTYPE_ACTIVITY			=	10	,		-- 检查指定活动开启
	CHECKTYPE_REG_DATE		=	11	,		-- 检查注册日期早于
	CHECKTYPE_QUES_NUM		=	12	,		-- 检查答题次数
	CHECKTYPE_HYDRANGEASTATUS	=	13	,		-- 检查绣球状态
	CHECKTYPE_IN_CAMP			=	14	,		-- 检查服务器阵营
	CHECKTYPE_ONLINESECS		=	15	,		-- 检查累计在线时间 秒
	CHECKTYPE_WEEKNATION		=	16	,		-- 弱国任务
	CHECKTYPE_IN_MY_SELF		=	17	,		-- 检查只能在本阵营接任务
	CHECKTYPE_IS_NATIONKILL		=	18	,		-- 检查是否在国探期间
	CHECKTYPE_IS_LUANWU		=	19	,		-- 检查是否在乱舞期间
	CHECKTYPE_IS_MASTER		=	20	,		-- 检查是否师傅
	CHECKTYPE_IS_TEAMHEAD		=	21	,		-- 检查是否队长
	CHECKTYPE_TEAM_MEMBER		=	22	,		-- 检查队伍人数
	CHECKTYPE_ISVIP_PLAYER		= 	23	,		-- 检查是否是VIP
	CHECKTYPE_ISSWORNED_PLAYER	= 	24	,		-- 检查是否已经结拜
};

-- 通用奖励 代码
enCommonAwardCode =
{
	AWARDTYPE_TELANT				=	1	,		-- 奖励修为点(参数1：点数)
	AWARDTYPE_FAMILY_HONOR		=	2	,		-- 奖励个人家族荣耀值
	AWARDTYPE_FAMILY_MONEY		=	3	,		-- 奖励家族金钱
	AWARDTYPE_FAMILY_CONTRIBUTE	=	4	,		-- 奖励个人家族贡献度
	AWARDTYPE_CORPS_GLORY			=	5	,		-- 奖励个人军团荣耀值
	AWARDTYPE_CORPS_MONEY			=	6	,		-- 奖励军团金钱
	AWARDTYPE_CORPS_CONTRIBUTE		=	7	,		-- 奖励个人军团贡献度
	AWARDTYPE_TITLE				=	8	,		-- 奖励称号
	AWARDTYPE_HONOR				=	9	,		-- 奖励功勋
	AWARDTYPE_BINDYINGYUANBAO		=	10	,		-- 奖励绑定的银元宝
	AWARDTYPE_COUNTRYBUILD		=	11	,		-- 奖励国家建设度
	AWARDTYPE_UPWIS				=	12	,		-- 上报才华
	AWARDTYPE_WISEXP				=	13	,		-- 奖励才华经验
	AWARDTYPE_CLEARHYDRANGEASTATUS	=	14	,		-- 清除绣球状态
	AWARDTYPE_COMBAT_FAME			=	15	,		-- 奖励竞技声望
	AWARDTYPE_REPE_FAME			=	16	,		-- 奖励副本声望
	AWARDTYPE_COMMON_FAME		=	17	,		-- 奖励历练声望
	AWARDTYPE_CORPS_COLLECTACTIVE	=	18	,		-- 奖励军团（采集）活跃度

};

--  鉴于模板ID不可能是负数，也就是说最高位永远不可能为1，所以制定一堆特殊的内部ID 用于实现特殊的需要计数的功能
enSpecialID=
{
	SPECIALID_COMPLETESTORAGE			=	0x80000001,	-- 完成悬赏任务
	SPECIALID_KILLFOREIGNER				=	0x80000002,	-- 杀死外国人
	SPECIALID_COMPLETELOOPTASK			=	0x80000003,	-- 完成环任务
	SPECIALID_COMPLETEQUESTION			=	0x80000004,	-- 完成每日答题
};


---------------------------------------
emPublishCheck = 
{
	CHECK_CORPS_LEVEL		= 1,
	CHECK_CORPS_MONEY		= 2,
	CHECK_CORPS_POSITION	= 3,
};
AllTasks={};
LoopTasks = {}; -- 记录全局环任务文件
SortedLoops = {}; -- 排序后的环任务 eg: {[1]={1,3,5...} (按职业索引 里面按等级排序)
--StorageTasks={};  -- 这个存储的是按照颜色等的索引后的任务表
AllStorageTasks={}; -- 这个用于存储单个task文件 1XX的 用于读取全局配置
AllChairManTasks={}; -- 掌门任务
AllChairManIDList={};  -- 掌门任务ID列表
--TragedyTasks={};
FirstStorageTasks = 0; -- 第一个库任务ID
npc_extend_tasks ={};
npc_back_tasks = {};
prob_tasks={}; -- 索引所有的会按概率刷的任务 索引下标就是大ID （1000以内的那些）
group_tasks={};  -- 任务组
beginer_mapid=22; -- 花果山地图ID
DayClearTasks={}; -- 每天需要清除的任务ID们
------------------------------------------------------全局函数
-- 计算根据等级杀怪的事件ID
function cale_kill_level_ogre_id( min_level, max_level )
	return (min_level or 0)*10000+(max_level or 0);
end

-- 和上面这个相反的函数
function get_ogre_level_by_caledid( caled_id )
	local ret = {low=0, high=0};
	if caled_id == 0 or caled_id == nil then
		return ret;
	end
	ret.high = math.mod( caled_id, 10000 );
	ret.low  = math.ceil(caled_id/10000);
	if ret.high > 0 then
		ret.low = ret.low -1;
	end
	return ret;
end

-- 弱阵营 积分 换倍数表
week_camp_level_exp=
{
	[-3]=1.6;
	[-2]=1.7;
	[-1]=1.8;
	[0]=1.8;
	[1]=1.8;
	[2]=1.9;
	[3]=2.0;
};

-- 乱舞 固定倍数
luanwu_exp=1.4;

-- 每轮组队环任务次数
team_loop_num=20;
