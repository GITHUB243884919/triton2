local longgong_easy = longgong_class.new( );
longgong_easy.mapid = 58;
longgong_easy.coefficient = 4 / 10;
longgong_easy.enco_count = 1;

longgong_easy.lg_kinglong_template = 40339;						--龙王摸板
longgong_easy.lg_kinglong_mark	= 200;							--龙王积分奖励
longgong_easy.lg_kinglong_index = 57096;							--龙王CreatorIndex
longgong_easy.lg_kingstorebox_template = 43242;					--龙王死亡创建宝箱模版ID
longgong_easy.lg_kingstorebox_index = 57129;						--龙王死亡创建宝箱CreatorIndex
longgong_easy.lg_conceal_ogre_index_dream = 57113;
longgong_easy.lg_conceal_ogre_index_run = 57114;
--龙王真身
longgong_easy.lg_kinglong_fact = 
	{ index = 57097, tempid = 40342, skilltemplate = longgong_easy.kinglong_fact_skill, mark = 500 };	
--龙王真身人质
longgong_easy.lg_kinglong_fact_hostage = { 
	boy = { index = 57118, tempid = 40299 }, girl = { index = 57119, tempid = 40300 } };

--杀怪计数
longgong_easy.lg_ogre_temp = { 40332, 40333, 40338, 40340, 40336, 40341 };


longgong_easy.lg_block[ 1 ].index = { 57135, 57136, 57137 };
longgong_easy.lg_block[ 1 ].machinery = { index = 57130, tempid = 40291 };
longgong_easy.lg_block[ 2 ].index = { 57133, 57134 };
longgong_easy.lg_block[ 2 ].ogreindex = { 57007, 57006, 57099, 57003, 57059, 57060, 57004, 57005 }; 
longgong_easy.lg_block[ 3 ].index = { 57139, 57140 };
longgong_easy.lg_block[ 3 ].ogreindex = { 57064, 57018 };
longgong_easy.lg_block[ 4 ].index = { 57132, 57142, 57143 };
longgong_easy.lg_block[ 4 ].ogreindex = 
{
	57029, 57028, 57030, 57102, 57036, 57035, 57032, 57033, 57031, 57034, 57038, 57039, 57040, 57067, 57037,
	57049, 57050, 57048, 57051, 57047
};
longgong_easy.lg_block[ 5 ].index = { 57141 };
longgong_easy.lg_block[ 5 ].tempid = 41423;
longgong_easy.lg_block[ 6 ].index = { 57144 };

longgong_easy.lg_ogre_machinery[ 1 ].index = 57127;
longgong_easy.lg_ogre_machinery[ 1 ].ogreindex = { 57041, 57042, 57043 };
longgong_easy.lg_ogre_machinery[ 2 ].index = 57128;
longgong_easy.lg_ogre_machinery[ 2 ].ogreindex = { 57045, 57044, 57046 };
longgong_easy.lg_ogre_machinery[ 3 ].index = 57123;
longgong_easy.lg_ogre_machinery[ 3 ].ogreindex = { 57049, 57050, 57048, 57051, 57047 };
longgong_easy.lg_ogre_machinery[ 4 ].index = longgong_easy.lg_kingstorebox_index;
longgong_easy.lg_ogre_machinery[ 4 ].tempid = longgong_easy.lg_kinglong_template;
longgong_easy.lg_ogre_machinery[ 4 ].ogreindex =
{
	longgong_easy.lg_kinglong_fact.index, longgong_easy.lg_kinglong_fact_hostage.boy.index, 
	longgong_easy.lg_kinglong_fact_hostage.girl.index, 57116, 57117 
};
longgong_easy.lg_ogre_machinery[ 5 ].index = longgong_easy.lg_kingstorebox_index;
longgong_easy.lg_ogre_machinery[ 5 ].tempid = longgong_easy.lg_kingstorebox_template;

longgong_easy:init( );

local function create_ogre( vRepetionID, vParame )--, vParame1, vParame2 )
	longgong_easy:create_ogre( vRepetionID, vParame );
end

local function ogre_death( vRepetionID, vParame )--, vEntityID, vTempID )
	longgong_easy:ogre_death( vRepetionID, vParame, 18 );
end

local function lg_time_notice( repetion_index, repetion_id, args, loop_index )
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.CLOSE, longgong_easy.lg_notice_time, 0 );
end

local function machinery_spring( vRepetionID, vParame )
	longgong_easy:machinery_spring( vRepetionID, vParame );
end

local function time_assignment( vRepetionID, vParame )
	longgong_easy:time_assignment( vRepetionID, vParame );
end

local function lg_time_end( repetion_index, repetion_id, args, loop_index )
	longgong_easy:lg_end( repetion_id, 0 );
	RepetionClose( repetion_id );
end

local function create_player( vRepetionID, vParame )
	longgong_easy:create_player( vRepetionID, vParame );
end

repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.LONGGONG_EASY,
		playercountmax = 6,		-- 
		mapid = { 58 },
		taskid = 0,			-- into task condition 
		posx = 44,
		posy = 243,
		pernumber = 2,			-- everday into max number 
		minlevel = 20,
		maxlevel = 110,
		question_check=0,		-- 是否进行副本答题验证
		team_level_flag = 2,
		relogin =	1,					-- 1表示可重入0表示不可重入
		closetime   = 120,	  -- 没有人时多久后关闭
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

		commtakemap = { RepetionID.LONGGONG, RepetionID.LONGGONG_HARD },

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
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 14, minute = 0, sec = 0 }
			},
			--第二个时段
			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 20, minute = 0, sec = 0 },
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
			time = longgong_easy.lg_complate_time,
			loop = 1,
			func = lg_time_end,
			args = { },
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_easy.lg_close_time,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = longgong_easy.lg_complate_time - longgong_easy.lg_notice_time,
			loop = 1,
			func = lg_time_notice,
			args = { },
			
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = longgong_easy.lg_kinglong_fact_assignment1.endtime,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 2 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_easy.lg_kinglong_fact_assignment2.endtime,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_easy.lg_block_time,
			loop = 1,
			func = lg_time_end,
			args = { },	
			sync = { kind = RepetionTimeSyncKind.ALL, key = 1 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_easy.conceal_time,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 3 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = longgong_easy.conceal_end_time,
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


