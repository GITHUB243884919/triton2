-- repetionid define
RepetionID =
{
	GUANGHANGONG	= 1,
	PUTUO			= 2,
	WUZHUANGGUAN	= 3,
	SHITUOLING		= 4,
	LINEYANGE		= 5,
	TIANWANG		= 6,

	LONGGONG		= 7,
	LONGGONG_EASY	= 8,
	LONGGONG_HARD	= 9,

	PANTAO			= 10,
	PANTAO_EASY		= 11,
	PANTAO_HARD		= 12,

	HUAGUOSHAN		= 13, 
	
	CHANGBAOTU		= 14,

	CORPS_BATTLE	= 15,

	FAMILY			= 16,

	DAYANTA			= 17,
	LIANHUADONG		= 18, -- 莲花洞
	HORNETSHOLE		= 19, -- 黄蜂洞
	FAMILY_TASTE	= 20, -- 家族品仙桃
	CORPS_CHALLEGE	= 21,	-- 军团挑战赛
	FAMILY_CHALLENGE= 22, --家族挑战赛
	FLY_WAR			= 23,	-- 腾云大战
	GRAND			= 24,	-- 御花园
	TOWER			= 25,	-- 通天塔
	LAST_ONE                = 26,	-- 大逃杀 
	LAST_ONE_TEAM           = 27,	-- 大逃杀组队 
	WUDIDONG		= 28,
	SHITU			= 29,	--师徒
	GLOBALWAR	= 30,	--全局副本
	CORPSBOSS               = 31,   -- 军团boss
}


--计时器开启类型
RepetionTimeBeginKind =
{
	START_BEGIN = 1,				--副本开始时候启动
	COMPLATE_BEGIN = 2,				--副本完成时候启动
	EVENT_BEGIN = 3					--事件启动
};

RepetionTimeResultKind =
{
	NONE  = 0,
	CLOSE = 1
};
--事件类型
RepetionEventKind = 
{
	PLAYER_CREATE 			= 0,		--玩家创建
	OGRE_CREATE 			= 1,		--NPC创建
	TASK_END				= 2,		--任务完成
	OGRE_DEATH				= 3,		--NPC死亡
	BOX_MACHINERY_SPING		= 4,		--机关
	TIME_ASSIGNMENT			= 5;			--计时事件
	ENTITY_MOVE_STOP		= 6;
	CLOSE_EVENT				= 7;		--副本关闭
	BEGIN_REPETION			= 8;		-- 副本开始
	EACH_OTHER				= 9,		-- 副本玩家交互
	LEAVE_REPETION                  = 10,     --玩家离开副本
	CALL_FUN				= 11,
        PLAYER_RELIVE                   = 12,          --玩家复活
	DEBUG_COMMAND		= 13,          -- 调试用的指令
	CHECK_AWARD		= 14,
};
--通知Client事件
RepetionNoticeEventKind = 
{
	NONE				= 0,			--无
	CLOSE				= 1,			--关闭
	END					= 2,			--结束
	CREATE_NPC 			= 3,			--创建NPC
	KILL				= 4,			--死亡
	COUNT_DOWN			= 5,			--倒计时开始
	COUNT_DOWN_END 		= 6,			--倒计时结束
	TAKE_COUNT			= 7,	
	RESOURCE			= 8,	
	GET				= 9,	
	CORPS_FLAG			= 10,
	CORPS_RESOURCE			= 11,
	CORPS_OGRE			= 12,
	RELIVE				= 13,
	FLAGTEMP			= 14,
	MSG				= 15,
	CORPS_BATTLEMAP		= 17,			-- 军团战争夺地图
};

RepetionEnd =
{
	UN_COMPLATE = 0,					--未完成
	COMPLATE    = 1,						--完成
};

-- 副本数据
RepetionDataKey = 
{
};

RepetionParameIndex =
{
	PARAME1				= 0,
	PARAME2				= 1,
};


RepetionTimeSyncKind = 
{
	NONE				= 0,		-- 不同步
	ALL					= 1,		-- 全部同步
	CAMP				= 2,		-- 阵营同步
};

RepetionTimeSyncKey = 
{
	CLOSE				= 0,
};

RepetionInsertItemReason =
{
	NONE				= 0,
	WIN					= 1;
	LOST				= 2;
	NEW					= 3;
	PASS				= 4;
};

RepetionInitPost =
{
    [RepetionID.LAST_ONE] = 
    {
        [1] = {
            {x = 14, y = 42},
            {x = 25, y = 54}, 
            {x = 24, y = 25}, 
            {x = 43, y = 47}, 
            {x = 30, y = 37}, 
        },
    }, 

}

RepetionTeleCharge = {
        [RepetionID.LAST_ONE] = {
                coef = 1,
                mapid = 165,
                post = {
		    {x = 14, y = 42},
		    {x = 25, y = 54}, 
		    {x = 24, y = 25}, 
		    {x = 43, y = 47}, 
		    {x = 30, y = 37}, 
                },
        },
}
