local DAYANTA_DATA_KEY =
{
	DAYANTA_STEP			= 0,
	OGRE_DEATH_NUM_FLOOL_1	= 1,	
	OGRE_DEATH_NUM_FLOOL_2	= 2,
	OGRE_DEATH_NUM_FLOOL_3  = 3,
	OGRE_DEATH_NUM_FLOOL_4	= 4,
	OGRE_DEATH_NUM_FLOOL_5	= 5,
	OGRE_DEATH_NUM_FLOOL_6	= 6,
	OGRE_DEATH_NUM_FLOOL_7	= 7,
	OGRE_DEATH_NUM_FLOOL_8	= 8,
	OGRE_DEATH_NUM_FLOOL_9	= 9,
	SERIES_POINT			= 10,			--连击点	
	MARK_POINT				= 11,			-- 积分
	DAYANTA_END				= 12,
	OGRE_DEATH_COUNT		= 13,
	SERIES_COUNT_1			= 14,
	SERIES_COUNT_2			= 15,
	SERIES_COUNT_3			= 16,
	SERIES_COUNT_4			= 17,
	SERIES_COUNT_5			= 18,
	SERIES_COUNT_6			= 19,
	SERIES_COUNT_7			= 20,
	SERIES_COUNT_8			= 21
};

local DAYANTA_PLAYER_DATA_KEY =
{
	MARK_POINT				= 0,
};

local DY_TAKE_COUNT_KEY = 
{
	SERIES_POINT			= 1,
	MARK					= 2,
	TIME					= 3,
	REPE_LEVEL				= 4,
	DEATH_COUNT				= 5,
	SERIES_COUNT_1			= 6,
	SERIES_COUNT_2			= 7,
	SERIES_COUNT_3			= 8,
	SERIES_COUNT_4			= 9,
	SERIES_COUNT_5			= 10,
	SERIES_COUNT_6			= 11,
	SERIES_COUNT_7			= 12,
	SERIES_COUNT_8			= 13,
	MONEY					= 14,
};

local dy_boss = { index = 89000 };

local SERIES_LEVEL =
{
	{ num = 1, time_add = 15 * 1000, mark = 5, count_key = DAYANTA_DATA_KEY.SERIES_COUNT_1 },
	{ num = 5, time_add = 14 * 1000, mark = 6, count_key = DAYANTA_DATA_KEY.SERIES_COUNT_2 },
	{ num = 15, time_add = 12 * 1000, mark = 7, count_key = DAYANTA_DATA_KEY.SERIES_COUNT_3 },
	{ num = 30, time_add = 11 * 1000, mark = 8, count_key = DAYANTA_DATA_KEY.SERIES_COUNT_4 },
	{ num = 45, time_add = 10 * 1000, mark = 10, count_key = DAYANTA_DATA_KEY.SERIES_COUNT_5 },
	{ num = 65, time_add = 8 * 1000, mark = 12, count_key = DAYANTA_DATA_KEY.SERIES_COUNT_6 },
	{ num = 85, time_add = 7 * 1000, mark = 14, count_key = DAYANTA_DATA_KEY.SERIES_COUNT_7 },
	{ num = 110, time_add = 6 * 1000, mark = 16, count_key = DAYANTA_DATA_KEY.SERIES_COUNT_8 },
--	{ num = 135, time_add = 6 * 1000, mark = 16 }
};

local ogre_floor = 
{
	{ 
		max = 10, cur = 7, key = DAYANTA_DATA_KEY.OGRE_DEATH_NUM_FLOOL_1, tele = 80012, 
		ogre_list = { 80000, 80001, 80002, 80003, 80004, 80005, 80006, 80007, 80008, 80009, 80010, 80011 } 
	},
	{ 
		max = 8, cur = 6, key = DAYANTA_DATA_KEY.OGRE_DEATH_NUM_FLOOL_2, tele = 81011, 
		ogre_list = { 81000, 81001, 81002, 81003, 81004, 81005, 81006, 81007, 81008, 81009 } 	
	},
	{ 
		max = 13, cur = 8, key = DAYANTA_DATA_KEY.OGRE_DEATH_NUM_FLOOL_3, tele = 82014, 
		ogre_list = { 82000, 82001, 82002, 82003, 82004, 82005, 82006, 82007, 82008, 82009, 82010, 82011, 82012, 82013 } 
	},
	{ 
		max = 11, cur = 3, key = DAYANTA_DATA_KEY.OGRE_DEATH_NUM_FLOOL_4, tele = 83012, 
		ogre_list = { 83000, 83001, 83002, 83003, 83004, 83005, 83006, 83007, 83008, 83009, 83010, 83011 } 
	},
	{ 
		max = 11, cur = 4, key = DAYANTA_DATA_KEY.OGRE_DEATH_NUM_FLOOL_5, tele = 84015, 
		ogre_list = { 84000, 84001, 84002, 84003, 84004, 84005, 84006, 84007, 84008, 84009, 84010, 84011, 84012, 84013, 84014 } 
	},
	{ 
		max = 12, cur = 9, key = DAYANTA_DATA_KEY.OGRE_DEATH_NUM_FLOOL_6, tele = 85015, 
		ogre_list = { 85000, 85001, 85002, 85003, 85004, 85005, 85006, 85007, 85008, 85009, 85010, 85011, 85012, 85013, 85014 } 
	},
	{ 
		max = 16, cur = 12, key = DAYANTA_DATA_KEY.OGRE_DEATH_NUM_FLOOL_7, tele = 86018, 
		ogre_list = 
		{ 86000, 86001, 86002, 86003, 86004, 86005, 86006, 86007, 86008, 86009, 86010, 86011, 86012, 86013, 86014, 86015, 86016 } 
	},
	{ 
		max = 14, cur = 10, key = DAYANTA_DATA_KEY.OGRE_DEATH_NUM_FLOOL_8, tele = 87016, 
		ogre_list = 
		{ 87000, 87001, 87002, 87003, 87004, 87005, 87006, 87007, 87008, 87009, 87010, 87011, 87012, 87013, 87014, 87015 } 
	},
	{ 
		max = 17, cur = 14, key = DAYANTA_DATA_KEY.OGRE_DEATH_NUM_FLOOL_9, tele = 88020, 
		ogre_list = 
		{ 
			88000, 88001, 88002, 88003, 88004, 88005, 88006, 88007, 88008, 88009, 88010, 88011, 88012, 88013, 88014, 
			88015, 88016, 88017, 88018, 88019
		} 
	},
	{
		max = 14, cur = 10, key = DAYANTA_DATA_KEY.OGRE_DEATH_NUM_FLOOL_10,
		ogre_list =
		{
			89000, 89001, 89002, 89003, 89004, 89005, 89006, 89007, 89008, 89009, 89010, 89011, 89012, 89014, 89013
		}
	}
--	{ max = 2, cur = 1, key = OGRE_DEATH_NUM_FLOOL_10, tele = 
};

local DY_TIME_MONEY=
{
	{ level_min = 45, level_max = 50, money = 30000 },	
	{ level_min = 51, level_max = 60, money = 30500 },	
	{ level_min = 61, level_max = 70, money = 40000 },	
	{ level_min = 71, level_max = 80, money = 50000 },	
	{ level_min = 81, level_max = 90, money = 50000 },	
	{ level_min = 91, level_max = 100, money = 50000 },	
	{ level_min = 101, level_max = 110, money = 50000 },	
	{ level_min = 111, level_max = 120, money = 50000 },	
	{ level_min = 121, level_max = 130, money = 50000 },	
	{ level_min = 131, level_max = 140, money = 50000 },	
	{ level_min = 141, level_max = 150, money = 50000 },	
	{ level_min = 151, level_max = 160, money = 50000 },	
};

local DY_TIME_MONEY_LEVEL=
{
	{ time_max = 5 * 60, per = 1 },
	{ time_max = 8 * 60, per = 0.8 },
	{ time_max = 12 * 60, per = 0.6 },
	{ time_max = 20 * 60, per = 0.4 },
	{ time_max = 60 * 60, per = 0.2 },
};

local num = 0;
local step = 0;
local time = 0;
local series = 0;
local mark = 0;
local level = 0;
local count = 0;
local money = 0;
local max_num = 0;


local function create_npc( repetion_id, parame )
	Debug( "Call DaYanTa ogre_create " .. parame.index .. " entityid " .. parame.entityid .. 
		" tempid " .. parame.tempid );

	step = GetIntData( repetion_id, DAYANTA_DATA_KEY.DAYANTA_STEP );
	if step == 0 then
		SetIntData( repetion_id, DAYANTA_DATA_KEY.DAYANTA_STEP, 1 );
	end

	table.foreachi( ogre_floor, function( i, v )
		table.foreachi( v.ogre_list, function( k, m )
			if parame.index == m then
				ObserverEntityDeath( repetion_id, parame.entityid );
				return 1;
			end
		end);
	end);
end

local function series_point_clear( repetion_id, notify )
	Debug( "repetion_id " .. repetion_id );

	series = GetIntData( repetion_id, DAYANTA_DATA_KEY.SERIES_POINT );	
	if series == 0 then
		return;
	end

	table.foreachi( SERIES_LEVEL, function( i, v )
		if SERIES_LEVEL[ i + 1 ] == nil then
			SetIntData( repetion_id, DAYANTA_DATA_KEY.MARK_POINT, series * v.mark );
			add_intdata( repetion_id, v.count_key, 1 );
		elseif series < SERIES_LEVEL[ i + 1 ].num then
			SetIntData( repetion_id, DAYANTA_DATA_KEY.MARK_POINT, series * v.mark );	
			add_intdata( repetion_id, v.count_key, 1 );
			return 1;
		end
	end);
	SetIntData( repetion_id, DAYANTA_DATA_KEY.SERIES_POINT, 0 );
	if notify == true then
		EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.SERIES_POINT, 0 );
	end
end


local function dy_end( repetion_id,  is_complate )
	series_point_clear( repetion_id, false );

	mark = GetIntData( repetion_id, DAYANTA_DATA_KEY.MARK_POINT );	
	Debug( "dy end, complate " .. is_complate );
	if is_complate == RepetionEnd.COMPLATE then
		Debug( "dy commit" );
		mark = mark + 100;	
	end

	Debug( "DY mark" .. mark );
	
	time = GetBeginTime( repetion_id );
	time = os.time( ) - time;	

	if time > 30 * 60 * 1000 then
		count = 0;
	else
		count = time;
	end

	level = GetRepeLevel( repetion_id );

	Debug( "dy cost time " .. count );

	mark = mark + ( 3100 - count ) / ( count + 100 ) * 100;
	mark = mark * ( 1 + level / 50 );

	table.foreachi( DY_TIME_MONEY, function( i, v )
		if level >= v.level_min then
			if level <= v.level_max then
				table.foreachi( DY_TIME_MONEY_LEVEL, function( k, m )
					if count <= m.time_max then
						money = v.money * m.per;
						InsertMoneyAll( repetion_id, money, 0 );
						Debug( "DY " .. repetion_id .. " GetMoney " .. money );
						return 1;
					end
				end);	
				return 1;
			end
		end
	end);
	
	if is_complate == RepetionEnd.COMPLATE then
		insert_item_all( repetion_id, 45525 , 3, RepetionInsertItemReason.WIN );
		if level >= 80 and level <= 110 then
			insert_item_all( repetion_id, 66885, 5, RepetionInsertItemReason.WIN );
		end
	end

	AddIntData2AllPlayer( repetion_id, DAYANTA_PLAYER_DATA_KEY.MARK_POINT, mark );	

	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.MARK, mark );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.TIME, time );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.REPE_LEVEL, level );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.DEATH_COUNT, 
		GetIntData( repetion_id, DAYANTA_DATA_KEY.OGRE_DEATH_COUNT ) );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.SERIES_COUNT_1,
		GetIntData( repetion_id, DAYANTA_DATA_KEY.SERIES_COUNT_1 ) );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.SERIES_COUNT_2,
		GetIntData( repetion_id, DAYANTA_DATA_KEY.SERIES_COUNT_2 ) );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.SERIES_COUNT_3,
		GetIntData( repetion_id, DAYANTA_DATA_KEY.SERIES_COUNT_3 ) );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.SERIES_COUNT_4,
		GetIntData( repetion_id, DAYANTA_DATA_KEY.SERIES_COUNT_4 ) );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.SERIES_COUNT_5,
		GetIntData( repetion_id, DAYANTA_DATA_KEY.SERIES_COUNT_5 ) );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.SERIES_COUNT_6,
		GetIntData( repetion_id, DAYANTA_DATA_KEY.SERIES_COUNT_6 ) );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.SERIES_COUNT_7,
		GetIntData( repetion_id, DAYANTA_DATA_KEY.SERIES_COUNT_7 ) );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.SERIES_COUNT_8,
		GetIntData( repetion_id, DAYANTA_DATA_KEY.SERIES_COUNT_8 ) );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.MONEY, money );

	AwardFameAll( repetion_id, 6 );
end


local function npc_death( repetion_id, parame )
	Debug( "Call DaYanTa ogre_death " .. parame.index .. " entityid " .. parame.killedid );

	if GetIntData( repetion_id, DAYANTA_DATA_KEY.DAYANTA_END ) == 1 then
		Infor( "DaYanTa Is End, RepetionID .. " .. repetion_id );
		return;
	end

	add_intdata( repetion_id, DAYANTA_DATA_KEY.OGRE_DEATH_COUNT, 1 );

	step = GetIntData( repetion_id, DAYANTA_DATA_KEY.DAYANTA_STEP );

    table.foreachi( ogre_floor, function( i, v )
        table.foreachi( v.ogre_list, function( k, m )
            if parame.index == m then
				if step ~= i then
					return 1;
				end
				if v.tele ~= nil then
					num = add_intdata( repetion_id, v.key, 1 );
					if num >= v.cur then
						v.cur = v.cur + 1;
						if v.cur >= v.max then
							v.cur = 1;
						end
						CreateNpc( repetion_id, v.tele );
						add_intdata( repetion_id, DAYANTA_DATA_KEY.DAYANTA_STEP, 1 );
						return 1;
					end
				end
            end                     
        end);                       
    end);

	num = add_intdata( repetion_id, DAYANTA_DATA_KEY.SERIES_POINT, 1 );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.SERIES_POINT, num );
	
	if num == 1 then
		TimeBegin( repetion_id, 2 );
	else	
--		ChangeTime( repetion_id, 2, 10 * 1000 );
		time = GetTime( repetion_id, 2 );

		table.foreachi( SERIES_LEVEL, function( i, v )
			if num > v.num then
				if SERIES_LEVEL[ i + 1 ] ~= nil then
					if num <= SERIES_LEVEL[ i + 1 ].num then
						ChangeTime( repetion_id, 2, time + v.time_add );
					end
				else
					ChangeTime( repetion_id, 2, time + v.time_add );	
				end
			end
		end);	
	end

	-- 最终BOSS死亡， 副本完成， 进入关闭到记时
	if parame.index == dy_boss.index then
		Debug( "BOSS Die" );
		SetIntData( repetion_id, DAYANTA_DATA_KEY.DAYANTA_END, 1 );
		dy_end( repetion_id, RepetionEnd.COMPLATE );
		
		TimeEnd( repetion_id, 1 );
		TimeBegin( repetion_id, 3 );
	end
end


local function series_point_clear_time( repetion_index, repetion_id, args, loop_index )
	series_point_clear( repetion_id, true );
--	series = GetIntData( repetion_id, DAYANTA_DATA_KEY.SERIES_POINT );	
--	if series == 0 then
--		return;
--	end
--
--	table.foreachi( SERIES_LEVEL, function( i, v )
--		if SERIES_LEVEL[ i + 1 ] == nil then
--			SetIntData( repetion_id, DAYANTA_DATA_KEY.MARK_POINT, series * v.mark );
--			add_intdata( repetion_id, v.count_key, 1 );
--		elseif series < SERIES_LEVEL[ i + 1 ].num then
--			SetIntData( repetion_id, DAYANTA_DATA_KEY.MARK_POINT, series * v.mark );	
--			add_intdata( repetion_id, v.count_key, 1 );
--			return 1;
--		end
--	end);
--	SetIntData( repetion_id, DAYANTA_DATA_KEY.SERIES_POINT, 0 );
--	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, DY_TAKE_COUNT_KEY.SERIES_POINT, 0 );
end


local function dy_end_time( repetion_index, repetion_id, args, loop_index )
	dy_end( repetion_id, RepetionEnd.UN_COMPLATE );
	RepetionClose( repetion_id );
end

repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.DAYANTA,
		playercountmax = 6,		-- 
		mapid = { 81, 82, 83, 84, 85, 86, 87, 88, 89, 90 },
		taskid = 0,			-- into task condition 
		posx = 24,
		posy = 36,
	--	teamin = true,
		pernumber = 1,			-- everday into max number 
                weekpernumber = 2,
		minlevel = 50,
		maxlevel = 120,
		relogin =	1,	-- 1表示可重入0表示不可重入
		closetime   = 120,	  -- 没有人时多久后关闭
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

		open = 
		{
			-- 年, 月，周， 日， 时, 分， 秒
			-- 0 是不做限时， 周是每月第几周
			--第一个时段

			{
				start = { year = 0, month = 0, week = 5, day = 0, hr = 0, minute = 5, sec = 0 },
				finish = { year = 0, month = 0, week = 5, day = 0, hr = 23, minute = 55, sec = 0 }
			},
			{
				start = { year = 0, month = 0, week = 6, day = 0, hr = 0, minute = 3, sec = 0 },
				finish = { year = 0, month = 0, week = 6, day = 0, hr = 23, minute = 55, sec = 0 }
			},
			--第二个时段
			{
				start = { year = 0, month = 0, week = 7, day = 0, hr = 0, minute = 5, sec = 0 },
				finish = { year = 0, month = 0, week = 7, day = 0, hr = 23, minute = 55, sec = 0 }
			},
			-- 最多支持五个时段
		}

	},

	timer = 
	{ 
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 1000 * 60 * 30,
			loop = 1,
			func = dy_end_time,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			loop = 1,
			time = 1000 * 15,
			func = series_point_clear_time,
			sync = { kind = RepetionTimeSyncKind.ALL,key = 1 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			loop = 1,
			time = 2 * 60 * 1000,
			sync = { kind = RepetionTimeSyncKind.ALL,key = 0 }	
		}
	},

	event=
	{
		[ RepetionEventKind.OGRE_CREATE ] = { func = create_npc },
		[ RepetionEventKind.OGRE_DEATH ] = { func = npc_death }
	}
}


