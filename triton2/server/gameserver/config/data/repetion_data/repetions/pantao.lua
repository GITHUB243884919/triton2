
local pantao_comm = pantao_class.new( );
pantao_comm.coefficient = 1;
pantao_comm.enco_count = 3;
pantao_comm.player_skill=39672;
--蟠桃树摸板ID
pantao_comm.tree_template_id = 39633;
--金猴
pantao_comm.gold_stone_monkey_template = 37241;
--绿猴
pantao_comm.green_stone_monkey_template = 40306;
--红猴
pantao_comm.red_stone_monkey_template = 40305;
--黑猴
pantao_comm.black_stone_monkey_template = 37190;
--石猴摸版
pantao_comm.stone_monkey_template = 
{ 
	37139, 
	pantao_comm.black_stone_monkey_template, 
	pantao_comm.gold_stone_monkey_template, 
	pantao_comm.green_stone_monkey_template, 
	pantao_comm.red_stone_monkey_template 
};
--猴王模版ID
pantao_comm.monkey_king = { tempid = 37394, coeff = 5 };--1 };
--猴妖摸版ID
pantao_comm.monkey_ghost = { 37292, 37343 };
--猴摸版
pantao_comm.monkey_template = { 37292, 37343, pantao_comm.monkey_king.tempid, 55722 };

--六耳猕猴信息( 创建Index, 模版ID， 创建时间， 评分系数
pantao_comm.six_monkey = { index = 20152, tempid = 37445, create_time = 918000, coeff = 50 };--1 };

--死亡创建列表
pantao_comm.monkey_death_create = { 
								{ index = 20005, death = 20102, probability = 10 },
								{ index = 20002, death = 20103, probability = 10 },
								{ index = 20004, death = 20101, probability = 10 },
								{ index = 20006, death = 20104, probability = 10 },
								{ index = 20028, death = 20105, probability = 10 },
								{ index = 20011, death = 20106, probability = 10 },
								{ index = 20014, death = 20110, probability = 20 },
								{ index = 20016, death = 20113, probability = 20 },
								{ index = 20015, death = 20112, probability = 20 }
							};
--共同创建列表
pantao_comm.monkey_create_twin = {
								{ index = 20009, twin = 20107, probability = 10 },
								{ index = 20012, twin = 20108, probability = 10 },
								{ index = 20008, twin = 20109, probability = 10 },
								{ index = 20021, twin = 20114, probability = 20 },
								{ index = 20020, twin = 20115, probability = 20 },
								{ index = 20023, twin = 20116, probability = 20 },
								{ index = 20019, twin = 20117, probability = 20 },
								{ index = 20036, twin = 20118, probability = 20 },
								{ index = 20030, twin = 20119, probability = 20 },
								{ index = 20035, twin = 20120, probability = 20 },
								{ index = 20034, twin = 20121, probability = 20 },
								{ index = 20038, twin = 20122, probability = 20 },
								{ index = 20043, twin = 20123, probability = 20 },
								{ index = 20040, twin = 20115, probability = 20 },
								{ index = 20037, twin = 20125, probability = 20 },
								{ index = 20050, twin = 20129, probability = 30 },
								{ index = 20047, twin = 20126, probability = 30 },
								{ index = 20049, twin = 20127, probability = 30 },
								{ index = 20044, twin = 20128, probability = 30 },
								{ index = 20057, twin = 20130, probability = 30 },
								{ index = 20058, twin = 20134, probability = 30 },
								{ index = 20055, twin = 20131, probability = 30 },
								{ index = 20053, twin = 20132, probability = 30 },
								{ index = 20052, twin = 20135, probability = 30 },
								{ index = 20054, twin = 20133, probability = 30 },
								{ index = 20066, twin = 20136, probability = 50 },
								{ index = 20068, twin = 20137, probability = 50 },
								{ index = 20062, twin = 20138, probability = 50 },
								{ index = 20073, twin = 20139, probability = 50 },
								{ index = 20077, twin = 20140, probability = 50 },
								{ index = 20078, twin = 20141, probability = 50 },
								{ index = 20083, twin = 20142, probability = 50 },
								{ index = 20087, twin = 20143, probability = 50 },
								{ index = 20088, twin = 20144, probability = 50 },
								{ index = 20091, twin = 20149, probability = 50 },
								{ index = 20093, twin = 20147, probability = 50 },
								{ index = 20095, twin = 20145, probability = 50 },
								{ index = 20096, twin = 20150, probability = 50 },
								{ index = 20098, twin = 20148, probability = 50 },
								{ index = 20100, twin = 20146, probability = 50 }
							};

local ogre_info =
{
	[ 20005 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20001 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20002 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20003 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20004 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20006 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20007 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20028 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20010 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20011 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20009 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20013 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20012 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20029 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20008 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20014 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20027 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20025 ]			= { path = pantao_comm.PATH_LEFT },			
	[ 20026 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20016 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20017 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20015 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20021 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20018 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20020 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20024 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20023 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20022 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20019 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20036 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20031 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20030 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20032 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20035 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20033 ]			= { path = pantao_comm.PATH_LEFT },		
	[ 20034 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20038 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20042 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20043 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20039 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20040 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20041 ]			= { path = pantao_comm.PATH_LEFT },
	[ 20037 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20050 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20046 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20047 ]		= { path = pantao_comm.PATH_MIDDLE },	
	[ 20048 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20049 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20045 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20044 ]			= { path = pantao_comm.PATH_MIDDLE },
	[ 20057 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20056 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20058 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20059 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20055 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20053 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20060 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20052 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20051 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20054 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20070 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20069 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20066 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20065 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20063 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20067 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20068 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20062 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20061 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20064 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20071 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20072 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20073 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20074 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20075 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20076 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20077 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20078 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20079 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20080 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20081 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20082 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20083 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20084 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20085 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20086 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20087 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20088 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20089 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20090 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20091 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20092 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20093 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20094 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20095 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20096 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20097 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20098 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20099 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20100 ]		= { path = pantao_comm.PATH_MIDDLE },

	-- 假六耳猕猴
	[ 20161 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20162 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20163 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20164 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20165 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20166 ]		= { path = pantao_comm.PATH_MIDDLE },
	
	-- 六耳猕猴
	[ pantao_comm.six_monkey.index ] = {	path = pantao_comm.PATH_MIDDLE, 
											death_create = { { rand = 50, index = { 20169 } },
															 { rand = 50, index = { 20167 } }
															} 
										},


	[ 20102 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20101 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20104 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20106 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20107 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20109 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20111 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20113 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20112 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20115 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20116 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20117 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20119 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20120 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20121 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20123 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20124 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20125 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20129 ]		= { path = pantao_comm.PATH_LEFT },
	[ 20127 ]		= { path = pantao_comm.PATH_MIDDLE },	
	[ 20128 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20130 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20134 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20131 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20135 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20136 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20137 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20139 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20140 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20142 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20143 ]		= { path = pantao_comm.PATH_RIGHT },
	[ 20147 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20145 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20148 ]		= { path = pantao_comm.PATH_MIDDLE },
	[ 20146 ]		= { path = pantao_comm.PATH_MIDDLE },
};


--创建Player
local function create_player( vRepetionID, vParame )--, vParame1, vParame2 )
	pantao_comm:create_player( vRepetionID, vParame );
end

--创建NPC
local function create_ogre( vRepetionID, vParame )--, vParame1, vParame2 )
	pantao_comm:create_ogre( vRepetionID, vParame );
	if ogre_info[ vParame.index ] ~= nil then
		pantao_comm:pt_ogre_path( vRepetionID, vParame.tempid, vParame.entityid, ogre_info[ vParame.index ] );
	end
end
--Npc死亡
local function ogre_death( vRepetionID, vParame )--vEntityID, vTempID )
	pantao_comm:ogre_death( vRepetionID, vParame, 22 );
	if ogre_info[ vParame.index ] ~= nil then
		pantao_comm:ogre_info_death( vRepetionID, ogre_info[ vParame.index ] );
	end
end
--创建猴
local function pt_create_monkey( repetion_index, repetion_id, args, loop_index )
	pantao_comm:pt_create_monkey( repetion_index, repetion_id, args, loop_index );
end

local function pt_create_monkey_time( repetion_index, repetion_id, args, loop_index )
	pantao_comm:pt_create_monkey_time( repetion_index, repetion_id, args, loop_index );
end

local function time_assignment( vRepetionID, vParame ) 
	pantao_comm:time_assignment( vRepetionID, vParame );
end

local function create_hierogram_time( mapid, vRepetionID, args, loop_index )
	pantao_comm:create_hierogram_time( mapid, vRepetionID, args, loop_index );
end
--蟠桃副本结束

local function pt_end_time( repetion_index, repetion_id, args, loop_index )
	pantao_comm:pt_end( repetion_id, RepetionEnd.UN_COMPLATE );
end

local function move_stop( repetion_id, parame )
	pantao_comm:move_stop( repetion_id, parame );
end

-- 副本配置如下
repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.PANTAO,
		playercountmax = 6,		-- 副本最大人数
		mapid = { 21 },			-- 副本地图ID
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

		commtakemap = { RepetionID.PANTAO_EASY, RepetionID.PANTAO_HARD },

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
			time = pantao_comm.end_time,
			loop = 1,
			func = pt_end_time,
			args = { },	
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		-- 副本关闭计时器
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = pantao_comm.close_time,
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
			time = pantao_comm.monkey_create_time,
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
			time = pantao_comm.monkey_create_space,
			loop = 5,
			func = pt_create_monkey,
			args = {
						{ 20005 },
						{ 20001 },
						{ 20002 },
						{ 20003 },
						{ 20004 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 5,
			func = pt_create_monkey,
			args = {
						{ 20006 },
						{ 20007 },
						{ 20028 },
						{ 20010 },
						{ 20011 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 5,
			func = pt_create_monkey,
			args = {
						{ 20009 },
						{ 20013 },
						{ 20012 },
						{ 20029 },
						{ 20008 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 4,
			func = pt_create_monkey,
			args = {
						{ 20014 },
						{ 20027, 20025 },
						{ 20026, 20016 },
						{ 20017, 20015 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 4,
			func = pt_create_monkey,
			args = {
						{ 20021 },
						{ 20018, 20020 },
						{ 20024, 20023 },
						{ 20022, 20019 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 4,
			func = pt_create_monkey,
			args = {
						{ 20036 },
						{ 20031, 20030 },
						{ 20032, 20035 },
						{ 20033, 20034 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 3,
			func = pt_create_monkey,
			args = {
						{ 20038, 20042 },
						{ 20043, 20039 },
						{ 20040, 20041, 20037 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 3,
			func = pt_create_monkey,
			args = {
						{ 20050, 20046 },
						{ 20047, 20048 },
						{ 20049, 20045, 20044 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20057, 20056, 20058, 20059, 20055 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20053, 20060, 20052, 20051, 20054 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20070, 20069, 20066, 20065, 20063 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20067, 20068, 20062, 20061, 20064 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20071, 20072, 20073, 20074, 20075 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20076, 20077, 20078, 20079, 20080 },
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20081, 20082, 20083, 20084, 20085 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20086, 20087, 20088, 20089, 20090 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20091, 20092, 20093, 20094, 20095 }
				   }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20096, 20097, 20098, 20099, 20100 }
				   }
		},
		-- 刷新六耳猕猴
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.six_monkey.create_time,--six_monkey_create_time,
			loop = 1,
			func = create_npc,
			args = { pantao_comm.six_monkey.index }--20152 }  -- 六耳猕猴索引，来自wordscene.xml
		},
		-- 创建神符
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.create_hiero_time[ 1 ],
			loop = 4,
			func = create_hierogram_time,
			args = { }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20153, 20154, 20155, 20156 }
				   }
		},	
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20157, 20158, 20159, 20160 }
				   }
		},	
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pantao_comm.monkey_create_space,
			loop = 1,
			func = pt_create_monkey,
			args = {
						{ 20161, 20162, 20163, 20164, 20165, 20166 }
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


