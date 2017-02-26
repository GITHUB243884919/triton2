local longgong_comm = longgong_class.new( );
longgong_comm.mapid = 14;
longgong_comm.coefficient = 1;
longgong_comm.enco_count = 3;

longgong_comm.lg_kinglong_template = 37088;						--龙王摸板
longgong_comm.lg_kinglong_mark	= 200;							--龙王积分奖励
longgong_comm.lg_kinglong_index = 13098;							--龙王CreatorIndex
longgong_comm.lg_kingstorebox_template = 40049;					--龙王死亡创建宝箱模版ID
longgong_comm.lg_kingstorebox_index = 13129;						--龙王死亡创建宝箱CreatorIndex
longgong_comm.lg_conceal_ogre_index_dream = 13017;
longgong_comm.lg_conceal_ogre_index_run = 13018;
--龙王真身
longgong_comm.lg_kinglong_fact = 
	{ index = 13099, tempid = 40301, skilltemplate = longgong_comm.kinglong_fact_skill, mark = 500 };	
--龙王真身人质
longgong_comm.lg_kinglong_fact_hostage = 
	{ boy = { index = 13118, tempid = 40299 }, girl = { index = 13119, tempid = 40300 } };

--杀怪计数
longgong_comm.lg_ogre_temp = { 36833, 36782, 37037, 36935, 36884, 40077 };

longgong_comm.lg_block[ 1 ].index = { 13137, 13136, 13138 };
longgong_comm.lg_block[ 1 ].machinery = { index = 13130, tempid = 40291 };
longgong_comm.lg_block[ 2 ].index = { 13134, 13135 };
longgong_comm.lg_block[ 2 ].ogreindex = { 13007, 13006, 13101, 13003, 13061, 13062, 13004, 13005 };
longgong_comm.lg_block[ 3 ].index = { 13140, 13141 };
longgong_comm.lg_block[ 3 ].ogreindex = { 13066, 13020 };
longgong_comm.lg_block[ 4 ].index = { 13132, 13143, 13144 };
longgong_comm.lg_block[ 4 ].ogreindex =			
{
	13031, 13030, 13032, 13104, 13038, 13037, 13034, 13033, 13035, 13036, 13040, 13042, 13041, 13069, 13039,
	13051, 13052, 13050, 13053, 13049
};
longgong_comm.lg_block[ 5 ].index = { 13142 };
longgong_comm.lg_block[ 5 ].tempid = 40283;
longgong_comm.lg_block[ 6 ].index = { 13133 };

longgong_comm.lg_ogre_machinery[ 1 ].index = 13127;
longgong_comm.lg_ogre_machinery[ 1 ].ogreindex = { 13043, 13044, 13045 };
longgong_comm.lg_ogre_machinery[ 2 ].index = 13128;
longgong_comm.lg_ogre_machinery[ 2 ].ogreindex = { 13048, 13046, 13047 };
longgong_comm.lg_ogre_machinery[ 3 ].index = 13123;
longgong_comm.lg_ogre_machinery[ 3 ].ogreindex = { 13051, 13052, 13050, 13053, 13049 };
longgong_comm.lg_ogre_machinery[ 4 ].index = longgong_comm.lg_kingstorebox_index;
longgong_comm.lg_ogre_machinery[ 4 ].tempid = longgong_comm.lg_kingstorebox_template;
longgong_comm.lg_ogre_machinery[ 4 ].ogreindex =
{
	longgong_comm.lg_kinglong_fact.index, longgong_comm.lg_kinglong_fact_hostage.boy.index,
	longgong_comm.lg_kinglong_fact_hostage.girl.index, 13116, 13117
};
longgong_comm.lg_ogre_machinery[ 5 ].index = longgong_comm.lg_kingstorebox_index;
longgong_comm.lg_ogre_machinery[ 5 ].tempid = longgong_comm.lg_kingstorebox_template;


longgong_comm:init( );

--临时变量

local function create_ogre( vRepetionID, vParame )--, vParame1, vParame2 )
	longgong_comm:create_ogre( vRepetionID, vParame );
end

local function ogre_death( vRepetionID, vParame )--, vEntityID, vTempID )
	longgong_comm:ogre_death( vRepetionID, vParame, 19 );
end

local function lg_time_notice( repetion_index, repetion_id, args, loop_index )
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.CLOSE, longgong_comm.lg_notice_time, 0 );
end

local function machinery_spring( vRepetionID, vParame )
	longgong_comm:machinery_spring( vRepetionID, vParame );
end

local function time_assignment( vRepetionID, vParame )
	longgong_comm:time_assignment( vRepetionID, vParame );
end

local function lg_time_end( repetion_index, repetion_id, args, loop_index )
	longgong_comm:lg_end( repetion_id, 0 );
	RepetionClose( repetion_id );
end


local function create_player( vRepetionID, vParame )
	longgong_comm:create_player( vRepetionID, vParame );
end

repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.LONGGONG,
		playercountmax = 6,		-- 
		mapid = { longgong_comm.mapid },
		taskid = 0,			-- into task condition 
		posx = 44,
		posy = 243,
		pernumber = 2,			-- everday into max number 
--		weekpernumber = 2,
		minlevel = 20,
		maxlevel = 120,
		--check_time = 20000,		-- 检查副本人数时间(毫秒)
		question_check=0,		-- 是否进行副本答题验证
		team_level_flag = 2,
		relogin =	1,					-- 1表示可重入0表示不可重入
		closetime   = 120,	  -- 没有人时多久后关闭
		
		special = 
		{
			{ item = 63855, flag = 3 }
		},
		team = 
		{
			teamin = true,		
			count = 3,			-- team redirect player number less
		},


		backpos = 
		{
			mapid = 1,
			posx = 33,
			posy = 13
		},
	
		join_check_key=
		{
			{ min_level=20, max_level=30, key=6 }
		},
		
		commtakemap = { RepetionID.LONGGONG_EASY, RepetionID.LONGGONG_HARD },

		-- 副本开放时间
		open = 
		{
			-- 年, 月，周， 日， 时, 分， 秒
			-- 0 是不做限时， 周是每月第几周
			--第一个时段

			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 10, minute = 0, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 13, minute = 0, sec = 0 }
			},
			--第二个时段
			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 21, minute = 0, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 24, minute = 0, sec = 0 }
			},
			-- 最多支持五个时段
		}

	},

	timer = 
	{ 
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = longgong_comm.lg_complate_time,
			loop = 1,
			func = lg_time_end,
			args = { },
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_comm.lg_close_time,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = longgong_comm.lg_complate_time - longgong_comm.lg_notice_time,
			loop = 1,
--			func = lg_time_notice,
			args = { }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = longgong_comm.lg_kinglong_fact_assignment1.endtime,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 2 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_comm.lg_kinglong_fact_assignment2.endtime,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = longgong_comm.lg_block_time,
			loop = 1,
			func = lg_time_end,
			args = { },
			sync = { kind = RepetionTimeSyncKind.ALL, key = 1 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_comm.conceal_time,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 3 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_comm.conceal_end_time,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		}
	},

	event = 
	{
		[ RepetionEventKind.OGRE_DEATH	] = { func = ogre_death },	
		[ RepetionEventKind.OGRE_CREATE ] = { func = create_ogre },
		[ RepetionEventKind.BOX_MACHINERY_SPING ] = { func = machinery_spring },
		[ RepetionEventKind.TIME_ASSIGNMENT ] = { func = time_assignment },
		[ RepetionEventKind.PLAYER_CREATE ] = { func = create_player },
	},
}


