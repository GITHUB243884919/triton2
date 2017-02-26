
local pantao_hard = pantao_class.new( );
pantao_hard.coefficient = 2;
pantao_hard.enco_count = 5;
pantao_hard.player_skill=43254;
--蟠桃树摸板ID
pantao_hard.tree_template_id = 40329;
--金猴
pantao_hard.gold_stone_monkey_template = 40314;
--绿猴
pantao_hard.green_stone_monkey_template = 40315;
--红猴
pantao_hard.red_stone_monkey_template = 40313;
--黑猴
pantao_hard.black_stone_monkey_template = 40312;
--石猴摸版
pantao_hard.stone_monkey_template = 
{ 
	40310, 
	pantao_hard.black_stone_monkey_template, 
	pantao_hard.gold_stone_monkey_template, 
	pantao_hard.green_stone_monkey_template, 
	pantao_hard.red_stone_monkey_template 
};
--猴王模版ID
pantao_hard.monkey_king = { tempid = 40327, coeff = 5 };--1 };
--猴妖摸版ID
pantao_hard.monkey_ghost = { 40309, 40311 };
--猴摸版
pantao_hard.monkey_template = { 40309, 40311, pantao_hard.monkey_king.tempid, 55721 };

--六耳猕猴信息( 创建Index, 模版ID， 创建时间， 评分系数
pantao_hard.six_monkey = { index = 56152, tempid = 40328, create_time = 918000, coeff = 50 };--1 };

--死亡创建列表
pantao_hard.monkey_death_create = { 
								{ index = 56005, death = 56102, probability = 10 },
								{ index = 56002, death = 56103, probability = 10 },
								{ index = 56004, death = 56101, probability = 10 },
								{ index = 56006, death = 56104, probability = 10 },
								{ index = 56028, death = 56105, probability = 10 },
								{ index = 56011, death = 56106, probability = 10 },
								{ index = 56014, death = 56110, probability = 20 },
								{ index = 56016, death = 56113, probability = 20 },
								{ index = 56015, death = 56112, probability = 20 }
							};
--共同创建列表
pantao_hard.monkey_create_twin = {
								{ index = 56009, twin = 56107, probability = 10 },
								{ index = 56012, twin = 56108, probability = 10 },
								{ index = 56008, twin = 56109, probability = 10 },
								{ index = 56021, twin = 56114, probability = 20 },
								{ index = 56020, twin = 56115, probability = 20 },
								{ index = 56023, twin = 56116, probability = 20 },
								{ index = 56019, twin = 56117, probability = 20 },
								{ index = 56036, twin = 56118, probability = 20 },
								{ index = 56030, twin = 56119, probability = 20 },
								{ index = 56035, twin = 56155, probability = 20 },
								{ index = 56034, twin = 56121, probability = 20 },
								{ index = 56038, twin = 56122, probability = 20 },
								{ index = 56043, twin = 56123, probability = 20 },
								{ index = 56040, twin = 56115, probability = 20 },
								{ index = 56037, twin = 56125, probability = 20 },
								{ index = 56050, twin = 56129, probability = 30 },
								{ index = 56047, twin = 56126, probability = 30 },
								{ index = 56049, twin = 56127, probability = 30 },
								{ index = 56044, twin = 56128, probability = 30 },
								{ index = 56057, twin = 56130, probability = 30 },
								{ index = 56058, twin = 56134, probability = 30 },
								{ index = 56055, twin = 56131, probability = 30 },
								{ index = 56053, twin = 56132, probability = 30 },
								{ index = 56052, twin = 56135, probability = 30 },
								{ index = 56054, twin = 56133, probability = 30 },
								{ index = 56066, twin = 56136, probability = 50 },
								{ index = 56068, twin = 56137, probability = 50 },
								{ index = 56062, twin = 56138, probability = 50 },
								{ index = 56073, twin = 56139, probability = 50 },
								{ index = 56077, twin = 56140, probability = 50 },
								{ index = 56078, twin = 56141, probability = 50 },
								{ index = 56083, twin = 56142, probability = 50 },
								{ index = 56087, twin = 56143, probability = 50 },
								{ index = 56088, twin = 56144, probability = 50 },
								{ index = 56091, twin = 56149, probability = 50 },
								{ index = 56093, twin = 56147, probability = 50 },
								{ index = 56095, twin = 56145, probability = 50 },
								{ index = 56096, twin = 56150, probability = 50 },
								{ index = 56098, twin = 56148, probability = 50 },
								{ index = 56100, twin = 56146, probability = 50 }
							};

local ogre_info =
{
	[ 56005 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56001 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56002 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56003 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56004 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56006 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56007 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56028 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56010 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56011 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56009 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56013 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56012 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56029 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56008 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56014 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56027 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56025 ]			= { path = pantao_hard.PATH_LEFT },			
	[ 56026 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56016 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56017 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56015 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56021 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56018 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56020 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56024 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56023 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56022 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56019 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56036 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56031 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56030 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56032 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56035 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56033 ]			= { path = pantao_hard.PATH_LEFT },		
	[ 56034 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56038 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56042 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56043 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56039 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56040 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56041 ]			= { path = pantao_hard.PATH_LEFT },
	[ 56037 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56050 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56046 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56047 ]		= { path = pantao_hard.PATH_MIDDLE },	
	[ 56048 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56049 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56045 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56044 ]			= { path = pantao_hard.PATH_MIDDLE },
	[ 56057 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56056 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56058 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56059 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56055 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56053 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56060 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56052 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56051 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56054 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56070 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56069 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56066 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56065 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56063 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56067 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56068 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56062 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56061 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56064 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56071 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56072 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56073 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56074 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56075 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56076 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56077 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56078 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56079 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56080 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56081 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56082 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56083 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56084 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56085 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56086 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56087 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56088 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56089 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56090 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56091 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56092 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56093 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56094 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56095 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56096 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56097 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56098 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56099 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56100 ]		= { path = pantao_hard.PATH_MIDDLE },

	-- 假六耳猕猴
	[ 56161 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56162 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56163 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56164 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56165 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56166 ]		= { path = pantao_hard.PATH_MIDDLE },
	
	-- 六耳猕猴
	[ pantao_hard.six_monkey.index ] = {	path = pantao_hard.PATH_MIDDLE, 
											death_create = { { rand = 50, index = { 56169 } },
															 { rand = 50, index = { 56167 } }
															} 
										},


	[ 56102 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56101 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56104 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56106 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56107 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56109 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56111 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56113 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56112 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56115 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56116 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56117 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56119 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56155 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56121 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56123 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56124 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56125 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56129 ]		= { path = pantao_hard.PATH_LEFT },
	[ 56127 ]		= { path = pantao_hard.PATH_MIDDLE },	
	[ 56128 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56130 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56134 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56131 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56135 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56136 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56137 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56139 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56140 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56142 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56143 ]		= { path = pantao_hard.PATH_RIGHT },
	[ 56147 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56145 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56148 ]		= { path = pantao_hard.PATH_MIDDLE },
	[ 56146 ]		= { path = pantao_hard.PATH_MIDDLE },
};


--创建Player
local function create_player( vRepetionID, vParame )--, vParame1, vParame2 )
	pantao_hard:create_player( vRepetionID, vParame );
end

--创建NPC
local function create_ogre( vRepetionID, vParame )--, vParame1, vParame2 )
	pantao_hard:create_ogre( vRepetionID, vParame );
	if ogre_info[ vParame.index ] ~= nil then
		pantao_hard:pt_ogre_path( vRepetionID, vParame.tempid, vParame.entityid, ogre_info[ vParame.index ] );
	end
end
--Npc死亡
local function ogre_death( vRepetionID, vParame )--vEntityID, vTempID )
	pantao_hard:ogre_death( vRepetionID, vParame, 23 );
	if ogre_info[ vParame.index ] ~= nil then
		pantao_hard:ogre_info_death( vRepetionID, ogre_info[ vParame.index ] );
	end
end
--创建猴
local function pt_create_monkey( repetion_index, repetion_id, args, loop_index )
	pantao_hard:pt_create_monkey( repetion_index, repetion_id, args, loop_index );
end

local function pt_create_monkey_time( repetion_index, repetion_id, args, loop_index )
	pantao_hard:pt_create_monkey_time( repetion_index, repetion_id, args, loop_index );
end

local function time_assignment( vRepetionID, vParame ) 
	pantao_hard:time_assignment( vRepetionID, vParame );
end

local function create_hierogram_time( mapid, vRepetionID, args, loop_index )
	pantao_hard:create_hierogram_time( mapid, vRepetionID, args, loop_index );
end
--蟠桃副本结束

local function pt_end_time( repetion_index, repetion_id, args, loop_index )
	pantao_hard:pt_end( repetion_id, RepetionEnd.UN_COMPLATE );
end

local function move_stop( repetion_id, parame )
	pantao_hard:move_stop( repetion_id, parame );
end

-- 副本配置如下
repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.PANTAO_HARD,
		playercountmax = 6,		-- 副本最大人数
		mapid = { 57 },			-- 副本地图ID
		taskid = 0,			-- 进入副本的前置任务
		posx = 6,				-- 暂时不用
		posy = 81,				-- 暂时不用
		pernumber = 2,			-- 每天进入副本的次数； 0-不限
                weekpernumber = 7,
		minlevel = 20,			-- 最低进入等级
		maxlevel = 120,
		team_level_flag = 2,
		relogin =	1,	-- 1表示可重入0表示不可重入
		closetime   = 120,	  -- 没有人时多久后关闭
		special = 
		{
			{ item = 63854, flag = 3 },
		},
		-- 组队信息
		team = 
		{
			teamin = true,		-- 是否组队进入； ture-需要组队; false - 不需组队
			count = 3,			-- 组队人数下限
		},
		

		backpos = 
		{
			mapid = 1,			-- 暂时不用
			posx = 33,			-- 暂时不用	
			posy = 13			-- 暂时不用
		},

		commtakemap = { RepetionID.PANTAO, RepetionID.PANTAO_EASY },

        join_check_key=
        {
            { min_level=20, max_level=30, key=6 }
        },

		-- 副本开放时间
		open = 
		{
			-- 年, 月，周， 日， 时, 分， 秒
			-- 0 是不做限时， 周是每月第几周
			--第一个时段
			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 0, minute = 3, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 23, minute = 59, sec = 0 }
			},
			-- 最多支持五个时段
		}

	},
	-- 定时器
	timer = 
	{ 
		-- 副本开始时定时器开始
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.end_time,
			loop = 1,
			func = pt_end_time,
			args = { },	
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		-- 副本关闭计时器
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = pantao_hard.close_time,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		-- 副本其他计时器
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 0,					--因为时间事件第一次不作用
			loop = 1,
		},
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_time,
			loop = 21,
			func = pt_create_monkey_time,
			args = {
						{ index = 5 },
						{ index = 6 },
						{ index = 7 },
						{ index = 8 },
						{ index = 9 },
						{ index = 10 },
						{ index = 11 },
						{ index = 12 },
						{ index = 13 },
						{ index = 14 },
						{ index = 15 },
						{ index = 16 },
						{ index = 17 },
						{ index = 18 },
						{ index = 19 },
						{ index = 20 },
						{ index = 21 },
						{ index = 22 },
						{ index = 25 },
						{ index = 26 },
						{ index = 27 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 5,
			func = pt_create_monkey,
			args = {
						{ 56005 },
						{ 56001 },
						{ 56002 },
						{ 56003 },
						{ 56004 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 5,
			func = pt_create_monkey,
			args = {
						{ 56006 },
						{ 56007 },
						{ 56028 },
						{ 56010 },
						{ 56011 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 5,
			func = pt_create_monkey,
			args = {
						{ 56009 },
						{ 56013 },
						{ 56012 },
						{ 56029 },
						{ 56008 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 4,
			func = pt_create_monkey,
			args = {
						{ 56014 },
						{ 56027, 56025 },
						{ 56026, 56016 },
						{ 56017, 56015 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 4,
			func = pt_create_monkey,
			args = {
						{ 56021 },
						{ 56018, 56020 },
						{ 56024, 56023 },
						{ 56022, 56019 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 4,
			func = pt_create_monkey,
			args = {
						{ 56036 },
						{ 56031, 56030 },
						{ 56032, 56035 },
						{ 56033, 56034 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 3,
			func = pt_create_monkey,
			args = {
						{ 56038, 56042 },
						{ 56043, 56039 },
						{ 56040, 56041, 56037 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 3,
			func = pt_create_monkey,
			args = {
						{ 56050, 56046 },
						{ 56047, 56048 },
						{ 56049, 56045, 56044 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56057, 56056, 56058, 56059, 56055 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56053, 56060, 56052, 56051, 56054 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56070, 56069, 56066, 56065, 56063 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56067, 56068, 56062, 56061, 56064 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56071, 56072, 56073, 56074, 56075 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56076, 56077, 56078, 56079, 56080 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56081, 56082, 56083, 56084, 56085 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56086, 56087, 56088, 56089, 56090 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56091, 56092, 56093, 56094, 56095 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56096, 56097, 56098, 56099, 56100 }
				   }
		},
		-- 刷新六耳猕猴
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.six_monkey.create_time,--six_monkey_create_time,
			loop = 1,
			func = create_npc,
			args = { pantao_hard.six_monkey.index }--56152 }  -- 六耳猕猴索引，来自wordscene.xml
		},
		-- 创建神符
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.create_hiero_time[ 1 ],
			loop = 4,
			func = create_hierogram_time,
			args = { }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56153, 56154, 56155, 56156 }
				   }
		},	
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56157, 56158, 56159, 56160 }
				   }
		},	
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_hard.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 56161, 56162, 56163, 56164, 56165, 56166 }
				   }
		},	
	},
	event = 
	{
		[ RepetionEventKind.PLAYER_CREATE ] = { func = create_player },
		[ RepetionEventKind.OGRE_CREATE ] = { func = create_ogre },
		[ RepetionEventKind.OGRE_DEATH	] = { func = ogre_death },
		[ RepetionEventKind.TIME_ASSIGNMENT ] = { func = time_assignment },
		[ RepetionEventKind.ENTITY_MOVE_STOP ] = { func = move_stop }
	},
}


