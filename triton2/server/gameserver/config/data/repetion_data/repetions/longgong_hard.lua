local longgong_hard = longgong_class.new( );
longgong_hard.mapid = 59;
longgong_hard.coefficient = 4;
longgong_hard.enco_count = 10;

longgong_hard.lg_kinglong_template = 39786;						--龙王摸板
longgong_hard.lg_kinglong_mark	= 200;							--龙王积分奖励
longgong_hard.lg_kinglong_index = 58098;							--龙王CreatorIndex
longgong_hard.lg_kingstorebox_template = 43243;					--龙王死亡创建宝箱模版ID
longgong_hard.lg_kingstorebox_index = 58129;						--龙王死亡创建宝箱CreatorIndex
longgong_hard.lg_conceal_ogre_index_dream = 58002;
longgong_hard.lg_conceal_ogre_index_run = 58003;
--龙王真身
longgong_hard.lg_kinglong_fact = 
	{ index = 58099, tempid = 40335, skilltemplate = longgong_hard.kinglong_fact_skill, mark = 500 };	
--龙王真身人质
longgong_hard.lg_kinglong_fact_hostage = 
	{ boy = { index = 58118, tempid = 40299 }, girl = { index = 58119, tempid = 40300 } };

--杀怪计数
longgong_hard.lg_ogre_temp = { 39788, 39787, 39785, 39782, 39783, 40334 };

longgong_hard.lg_block[ 1 ].index = { 58136, 58137, 58138 };
longgong_hard.lg_block[ 1 ].machinery = { index = 58130, tempid = 40291 };
longgong_hard.lg_block[ 2 ].index = { 58133, 58135 };
longgong_hard.lg_block[ 2 ].ogreindex = { 58009, 58008, 58101, 58005, 58061, 58062, 58006, 58007 };
longgong_hard.lg_block[ 3 ].index = { 58140, 58141 };
longgong_hard.lg_block[ 3 ].ogreindex = { 58066, 58020 };
longgong_hard.lg_block[ 4 ].index = { 58132, 58143, 58144 };
longgong_hard.lg_block[ 4 ].ogreindex =
{
	26342, 58030, 58032, 58104, 58038, 58037, 58034, 58033, 58035, 58036, 58040, 58042, 58041, 58069, 58039,
	58051, 58052, 58050, 58053, 58049
}
longgong_hard.lg_block[ 5 ].index = { 58142 };
longgong_hard.lg_block[ 5 ].tempid = 41424;
longgong_hard.lg_block[ 6 ].index = { 58134 };

longgong_hard.lg_ogre_machinery[ 1 ].index = 58127;
longgong_hard.lg_ogre_machinery[ 1 ].ogreindex = { 58043, 58044, 58045 };
longgong_hard.lg_ogre_machinery[ 2 ].index = 58128;
longgong_hard.lg_ogre_machinery[ 2 ].ogreindex = { 58047, 58046, 58048 };
longgong_hard.lg_ogre_machinery[ 3 ].index = 58123;
longgong_hard.lg_ogre_machinery[ 3 ].ogreindex = { 58051, 58052, 58050, 58053, 58049 };
longgong_hard.lg_ogre_machinery[ 4 ].index = longgong_hard.lg_kingstorebox_index;
longgong_hard.lg_ogre_machinery[ 4 ].tempid = longgong_hard.lg_kingstorebox_template;
longgong_hard.lg_ogre_machinery[ 4 ].ogreindex = 
{ 
	longgong_hard.lg_kinglong_fact.index, longgong_hard.lg_kinglong_fact_hostage.boy.index, 
	longgong_hard.lg_kinglong_fact_hostage.girl.index, 58116, 58117 
};
longgong_hard.lg_ogre_machinery[ 5 ].index = longgong_hard.lg_kingstorebox_index;
longgong_hard.lg_ogre_machinery[ 5 ].tempid = longgong_hard.lg_kingstorebox_template;

longgong_hard:init( );

local function create_ogre( vRepetionID, vParame )--, vParame1, vParame2 )
	longgong_hard:create_ogre( vRepetionID, vParame );
end

local function ogre_death( vRepetionID, vParame )--, vEntityID, vTempID )
	longgong_hard:ogre_death( vRepetionID, vParame, 20 );
end

local function lg_time_notice( repetion_index, repetion_id, args, loop_index )
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.CLOSE, lg_notice_time, 0 );
end

local function machinery_spring( vRepetionID, vParame )
	longgong_hard:machinery_spring( vRepetionID, vParame );
end

local function time_assignment( vRepetionID, vParame )
	longgong_hard:time_assignment( vRepetionID, vParame );
end

local function lg_time_end( repetion_index, repetion_id, args, loop_index )
	longgong_hard:lg_end( repetion_id, 0 );
	RepetionClose( repetion_id );
end

local function create_player( vRepetionID, vParame )
	longgong_hard:create_player( vRepetionID, vParame );
end


repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.LONGGONG_HARD,
		playercountmax = 6,		-- 
		mapid = { 59 },
		taskid = 0,			-- into task condition 
		posx = 44,
		posy = 243,
		pernumber = 2,			-- everday into max number 
		minlevel = 20,
		maxlevel = 120,
		--check_time = 20000,		-- 检查副本人数时间(毫秒)
		question_check=0,		-- 是否进行副本答题验证
		team_level_flag = 2,
		relogin =	1,					-- 1表示可重入0表示不可重入
		closetime   = 120,	  -- 没有人时多久后关闭
		consumetimes = 2;               -- 每次进入扣除的次数，默认为1
		special = 
		{
			{ item = 63855, flag = 3 },
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

		commtakemap = { RepetionID.LONGGONG, RepetionID.LONGGONG_EASY },

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
			time = longgong_hard.lg_complate_time,
			loop = 1,
			func = lg_time_end,
			args = { },
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_hard.lg_close_time,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = longgong_hard.lg_complate_time - longgong_hard.lg_notice_time,
			loop = 1,
--			func = lg_time_notice,
			args = { },
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = longgong_hard.lg_kinglong_fact_assignment1.endtime,
			loop = 1,	
			sync = { kind = RepetionTimeSyncKind.ALL, key = 2 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_hard.lg_kinglong_fact_assignment2.endtime,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_hard.lg_block_time,
			loop = 1,
			func = lg_time_end,
			args = { },
			sync = { kind = RepetionTimeSyncKind.ALL, key = 1 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_hard.conceal_time,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 3 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_hard.conceal_end_time,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		}
	},

	event = 
	{
--		[ RepetionEventKind.TASK_END ] = { func = task_end },
		[ RepetionEventKind.OGRE_DEATH	] = { func = ogre_death },	
		[ RepetionEventKind.OGRE_CREATE ] = { func = create_ogre },
		[ RepetionEventKind.BOX_MACHINERY_SPING ] = { func = machinery_spring },
		[ RepetionEventKind.TIME_ASSIGNMENT ] = { func = time_assignment },
		[ RepetionEventKind.PLAYER_CREATE ] = { func = create_player },
	},
}


