
local pantao_easy = pantao_class.new( );
pantao_easy.coefficient = 4 / 10;
pantao_easy.enco_count = 1;
pantao_easy.player_skill=43253;
--蟠桃树摸板ID
pantao_easy.tree_template_id = 40325;
--金猴
pantao_easy.gold_stone_monkey_template = 40321;
--绿猴
pantao_easy.green_stone_monkey_template = 40322;
--红猴
pantao_easy.red_stone_monkey_template = 40320;
--黑猴
pantao_easy.black_stone_monkey_template = 40319;
--石猴摸版
pantao_easy.stone_monkey_template = 
{ 
	40316, 
	pantao_easy.black_stone_monkey_template, 
	pantao_easy.gold_stone_monkey_template, 
	pantao_easy.green_stone_monkey_template, 
	pantao_easy.red_stone_monkey_template 
};
--猴王模版ID
pantao_easy.monkey_king = { tempid = 40323, coeff = 5 };--1 };
--猴妖摸版ID
pantao_easy.monkey_ghost = { 40317, 40318 };
--猴摸版
pantao_easy.monkey_template = { 40317, 40318, pantao_easy.monkey_king.tempid, 55720 };

--六耳猕猴信息( 创建Index, 模版ID， 创建时间， 评分系数
pantao_easy.six_monkey = { index = 55152, tempid = 40324, create_time = 918000, coeff = 50 };--1 };

--死亡创建列表
pantao_easy.monkey_death_create = { 
								{ index = 55005, death = 55102, probability = 10 },
								{ index = 55002, death = 55103, probability = 10 },
								{ index = 55004, death = 55101, probability = 10 },
								{ index = 55006, death = 55104, probability = 10 },
								{ index = 55028, death = 55105, probability = 10 },
								{ index = 55011, death = 55106, probability = 10 },
								{ index = 55014, death = 55110, probability = 20 },
								{ index = 55016, death = 55113, probability = 20 },
								{ index = 55015, death = 55112, probability = 20 }
							};
--共同创建列表
pantao_easy.monkey_create_twin = {
								{ index = 55009, twin = 55107, probability = 10 },
								{ index = 55012, twin = 55108, probability = 10 },
								{ index = 55008, twin = 55109, probability = 10 },
								{ index = 55021, twin = 55114, probability = 20 },
								{ index = 55020, twin = 55115, probability = 20 },
								{ index = 55023, twin = 55116, probability = 20 },
								{ index = 55019, twin = 55117, probability = 20 },
								{ index = 55036, twin = 55118, probability = 20 },
								{ index = 55030, twin = 55119, probability = 20 },
								{ index = 55035, twin = 55120, probability = 20 },
								{ index = 55034, twin = 55121, probability = 20 },
								{ index = 55038, twin = 55122, probability = 20 },
								{ index = 55043, twin = 55123, probability = 20 },
								{ index = 55040, twin = 55115, probability = 20 },
								{ index = 55037, twin = 55125, probability = 20 },
								{ index = 55050, twin = 55129, probability = 30 },
								{ index = 55047, twin = 55126, probability = 30 },
								{ index = 55049, twin = 55127, probability = 30 },
								{ index = 55044, twin = 55128, probability = 30 },
								{ index = 55057, twin = 55130, probability = 30 },
								{ index = 55058, twin = 55134, probability = 30 },
								{ index = 55055, twin = 55131, probability = 30 },
								{ index = 55053, twin = 55132, probability = 30 },
								{ index = 55052, twin = 55135, probability = 30 },
								{ index = 55054, twin = 55133, probability = 30 },
								{ index = 55066, twin = 55136, probability = 50 },
								{ index = 55068, twin = 55137, probability = 50 },
								{ index = 55062, twin = 55138, probability = 50 },
								{ index = 55073, twin = 55139, probability = 50 },
								{ index = 55077, twin = 55140, probability = 50 },
								{ index = 55078, twin = 55141, probability = 50 },
								{ index = 55083, twin = 55142, probability = 50 },
								{ index = 55087, twin = 55143, probability = 50 },
								{ index = 55088, twin = 55144, probability = 50 },
								{ index = 55091, twin = 55149, probability = 50 },
								{ index = 55093, twin = 55147, probability = 50 },
								{ index = 55095, twin = 55145, probability = 50 },
								{ index = 55096, twin = 55150, probability = 50 },
								{ index = 55098, twin = 55148, probability = 50 },
								{ index = 55100, twin = 55146, probability = 50 }
							};

local ogre_info =
{
	[ 55005 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55001 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55002 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55003 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55004 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55006 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55007 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55028 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55010 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55011 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55009 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55013 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55012 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55029 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55008 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55014 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55027 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55025 ]			= { path = pantao_easy.PATH_LEFT },			
	[ 55026 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55016 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55017 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55015 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55021 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55018 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55020 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55024 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55023 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55022 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55019 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55036 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55031 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55030 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55032 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55035 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55033 ]			= { path = pantao_easy.PATH_LEFT },		
	[ 55034 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55038 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55042 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55043 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55039 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55040 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55041 ]			= { path = pantao_easy.PATH_LEFT },
	[ 55037 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55050 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55046 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55047 ]		= { path = pantao_easy.PATH_MIDDLE },	
	[ 55048 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55049 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55045 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55044 ]			= { path = pantao_easy.PATH_MIDDLE },
	[ 55057 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55056 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55058 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55059 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55055 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55053 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55060 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55052 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55051 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55054 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55070 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55069 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55066 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55065 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55063 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55067 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55068 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55062 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55061 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55064 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55071 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55072 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55073 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55074 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55075 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55076 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55077 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55078 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55079 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55080 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55081 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55082 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55083 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55084 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55085 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55086 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55087 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55088 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55089 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55090 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55091 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55092 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55093 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55094 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55095 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55096 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55097 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55098 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55099 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55100 ]		= { path = pantao_easy.PATH_MIDDLE },

	-- 假六耳猕猴
	[ 55161 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55162 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55163 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55164 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55165 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55166 ]		= { path = pantao_easy.PATH_MIDDLE },
	
	-- 六耳猕猴
	[ pantao_easy.six_monkey.index ] = {	path = pantao_easy.PATH_MIDDLE, 
											death_create = { { rand = 50, index = { 55169 } },
															 { rand = 50, index = { 55167 } }
															} 
										},


	[ 55102 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55101 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55104 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55106 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55107 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55109 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55111 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55113 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55112 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55115 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55116 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55117 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55119 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55120 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55121 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55123 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55124 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55125 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55129 ]		= { path = pantao_easy.PATH_LEFT },
	[ 55127 ]		= { path = pantao_easy.PATH_MIDDLE },	
	[ 55128 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55130 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55134 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55131 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55135 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55136 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55137 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55139 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55140 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55142 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55143 ]		= { path = pantao_easy.PATH_RIGHT },
	[ 55147 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55145 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55148 ]		= { path = pantao_easy.PATH_MIDDLE },
	[ 55146 ]		= { path = pantao_easy.PATH_MIDDLE },
};


--创建Player
local function create_player( vRepetionID, vParame )--, vParame1, vParame2 )
	pantao_easy:create_player( vRepetionID, vParame );
end

--创建NPC
local function create_ogre( vRepetionID, vParame )--, vParame1, vParame2 )
	pantao_easy:create_ogre( vRepetionID, vParame );
	if ogre_info[ vParame.index ] ~= nil then
		pantao_easy:pt_ogre_path( vRepetionID, vParame.tempid, vParame.entityid, ogre_info[ vParame.index ] );
	end
end
--Npc死亡
local function ogre_death( vRepetionID, vParame )--vEntityID, vTempID )
	pantao_easy:ogre_death( vRepetionID, vParame, 21 );
	if ogre_info[ vParame.index ] ~= nil then
		pantao_easy:ogre_info_death( vRepetionID, ogre_info[ vParame.index ] );
	end
end
--创建猴
local function pt_create_monkey( repetion_index, repetion_id, args, loop_index )
	pantao_easy:pt_create_monkey( repetion_index, repetion_id, args, loop_index );
end

local function pt_create_monkey_time( repetion_index, repetion_id, args, loop_index )
	pantao_easy:pt_create_monkey_time( repetion_index, repetion_id, args, loop_index );
end

local function time_assignment( vRepetionID, vParame ) 
	pantao_easy:time_assignment( vRepetionID, vParame );
end

local function create_hierogram_time( mapid, vRepetionID, args, loop_index )
	pantao_easy:create_hierogram_time( mapid, vRepetionID, args, loop_index );
end
--蟠桃副本结束

local function pt_end_time( repetion_index, repetion_id, args, loop_index )
	pantao_easy:pt_end( repetion_id, RepetionEnd.UN_COMPLATE );
end

local function move_stop( repetion_id, parame )
	pantao_easy:move_stop( repetion_id, parame );
end

-- 副本配置如下
repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.PANTAO_EASY,
		playercountmax = 6,		-- 副本最大人数
		mapid = { 56 },			-- 副本地图ID
		taskid = 0,			-- 进入副本的前置任务
		posx = 6,				-- 暂时不用
		posy = 81,				-- 暂时不用
		pernumber = 2,			-- 每天进入副本的次数； 0-不限
                weekpernumber = 7,
		minlevel = 20,			-- 最低进入等级
		maxlevel =100,
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

		commtakemap = { RepetionID.PANTAO, RepetionID.PANTAO_HARD },

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
			time = pantao_easy.end_time,
			loop = 1,
			func = pt_end_time,
			args = { },
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		-- 副本关闭计时器
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = pantao_easy.close_time,
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
			time = pantao_easy.monkey_create_time,
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
			time = pantao_easy.monkey_create_space,
			loop = 5,
			func = pt_create_monkey,
			args = {
						{ 55005 },
						{ 55001 },
						{ 55002 },
						{ 55003 },
						{ 55004 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 5,
			func = pt_create_monkey,
			args = {
						{ 55006 },
						{ 55007 },
						{ 55028 },
						{ 55010 },
						{ 55011 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 5,
			func = pt_create_monkey,
			args = {
						{ 55009 },
						{ 55013 },
						{ 55012 },
						{ 55029 },
						{ 55008 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 4,
			func = pt_create_monkey,
			args = {
						{ 55014 },
						{ 55027, 55025 },
						{ 55026, 55016 },
						{ 55017, 55015 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 4,
			func = pt_create_monkey,
			args = {
						{ 55021 },
						{ 55018, 55020 },
						{ 55024, 55023 },
						{ 55022, 55019 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 4,
			func = pt_create_monkey,
			args = {
						{ 55036 },
						{ 55031, 55030 },
						{ 55032, 55035 },
						{ 55033, 55034 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 3,
			func = pt_create_monkey,
			args = {
						{ 55038, 55042 },
						{ 55043, 55039 },
						{ 55040, 55041, 55037 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 3,
			func = pt_create_monkey,
			args = {
						{ 55050, 55046 },
						{ 55047, 55048 },
						{ 55049, 55045, 55044 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55057, 55056, 55058, 55059, 55055 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55053, 55060, 55052, 55051, 55054 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55070, 55069, 55066, 55065, 55063 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55067, 55068, 55062, 55061, 55064 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55071, 55072, 55073, 55074, 55075 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55076, 55077, 55078, 55079, 55080 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55081, 55082, 55083, 55084, 55085 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55086, 55087, 55088, 55089, 55090 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55091, 55092, 55093, 55094, 55095 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55096, 55097, 55098, 55099, 55100 }
				   }
		},
		-- 刷新六耳猕猴
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.six_monkey.create_time,--six_monkey_create_time,
			loop = 1,
			func = create_npc,
			args = { pantao_easy.six_monkey.index }--55152 }  -- 六耳猕猴索引，来自wordscene.xml
		},
		-- 创建神符
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.create_hiero_time[ 1 ],
			loop = 4,
			func = create_hierogram_time,
			args = { }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55153, 55154, 55155, 55156 }
				   }
		},	
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55157, 55158, 55159, 55160 }
				   }
		},	
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_easy.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 55161, 55162, 55163, 55164, 55165, 55166 }
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


