local HG_DATA_KEY =
{
	PRISON_ENTITYID			= 0;			-- 囚车实体ID
	PRISON_CAMP				= 1;			--酋车怔营
	PRISON_MOVE_TARGET		= 2;			--囚车移动目标点
	PRISON_GUARD_COUNT		= 3;			--囚车护卫数量
	BATTLE_PHASE			= 4;
};

local HG_PHASE =
{
	BATTLE_WAIT				= 0;
	PRISON_GUARD			= 1;			--第一阶段， 杀囚车护卫
	COUNT_DOWN				= 2;			-- 第二阶段， 倒计时间
	FIRE_BATTLE				= 3;			-- 进入第三阶段
}

local Camp =
{
	MONKEY		= 1,				--猴兵
	HEAVEN		= 2,				--天兵
};

local RELIVE_TIME = 20000;
local begin_time = 2 * 60 * 1000;
local count_down_begin_time = 8 * 60 * 1000;			--第二阶段初始时间

local player_list =
{
	[ Camp.MONKEY ] =
	{
		pos = { x = 23, y = 34 },
		relive_pos = { x = 23, y = 34 },
		skill = 45147
	},
	[ Camp.HEAVEN ] =
	{
		pos = { x = 139, y = 43 },
		relive_pos = { x = 139, y = 43 },
		skill = 45148
	}
};

local ogre_list =
{
	[ Camp.MONKEY ] =
	{
		tempid = { 45663, 45664 },
		index = { 54001, 54002, 54000, 54003 },
		pos = 
		{ 
			{ x = 23, y = 45 },
			{ x = 24, y = 93 },
			{ x = 32, y = 99 },
			{ x = 38, y = 104 },
			{ x = 50, y = 105 },
			{ x = 60, y = 107 },
			{ x = 74, y = 114 },
			{ x = 89, y = 117 },
			{ x = 97, y = 109 },
			{ x = 101, y = 102 },
			{ x = 107, y = 92 },
			{ x = 116, y = 79 },
			{ x = 122, y = 68 },
			{ x = 129, y = 55 }
		}
	},
	[ Camp.HEAVEN ] = 
	{
		tempid = { 45661, 45662 },
		index = { 54004, 54005, 54006, 54007 },
		pos = 
		{
			{ x = 129, y = 55 },
			{ x = 122, y = 68 },
			{ x = 116, y = 79 },
			{ x = 107, y = 92 },
			{ x = 101, y = 102 },
			{ x = 97, y = 109 },
			{ x = 89, y = 117 },
			{ x = 74, y = 114 },
			{ x = 60, y = 107 },
			{ x = 50, y = 105 },
			{ x = 38, y = 104 },
			{ x = 32, y = 99 },
			{ x = 24, y = 93 },
			{ x = 23, y = 45 },
		}
	}
};

local close_time = 2 * 60 * 1000;

local create_npc = { 54014, 54008, 54009, 54010, 54011, 54012, 54013 };

local PRISON = { index = 54014, tempid = 43256, camp = Camp.HEAVEN, start = 6 };
local PRISON_GUARD = 
{
	tempid = 43257;
	{
		index = 54008,
		pos = { x = -2, y = -2 }
	},
	{
		index = 54009,
		pos = { x = 0, y = -2 }
	},
	{
		index = 54010,
		pos = { x = 2, y = -2 }
	},
	{
		index = 54011,
		pos = { x = -2, y = 2 }
	},
	{
		index = 54012,
		pos = { x = 0, y = 2 }
	},
	{
		index = 54013,
		pos = { x = 2, y = 2 }
	}
};

local PRISON_POS =
{
	{ x = 23, y = 45 },
	{ x = 24, y = 93 },
	{ x = 32, y = 99 },
	{ x = 38, y = 104 },
	{ x = 50, y = 105 },
	{ x = 54, y = 112 },
	{ x = 60, y = 107 },
	{ x = 74, y = 114 },
	{ x = 89, y = 117 },
	{ x = 97, y = 109 },
	{ x = 101, y = 102 },
	{ x = 107, y = 92 },
	{ x = 116, y = 79 },
	{ x = 122, y = 68 },
	{ x = 129, y = 55 }
};

local campid;
local time;
local phase;
local CampTime =
{
	[ Camp.MONKEY ] = 4,
	[ Camp.HEAVEN ] = 5,
};

local function get_index( camp, now_index )
	if camp == Camp.MONKEY then
		return now_index - 1;
	else
		return now_index + 1;
	end
end

local function other_camp( camp )
	if camp == Camp.MONKEY then
		return Camp.HEAVEN;
	else
		return Camp.MONKEY;
	end
end

local function create_hg_ogre( repetion_id )
	table.foreachi( ogre_list, function( i, v )
		table.foreachi( v.index, function( k, m )
			CreateNpc( repetion_id, m ); 
		end);
	end);
end

local function create_ogre( vRepetionID, vParame )
	Debug( "Create Npc, index " .. vParame.index .. ", entityid " .. vParame.entityid .. ", tempid " .. 
		vParame.tempid );
	if vParame.index == PRISON.index then
--		NpcMove( vRepetionID, vParame.entityid, PRISON_POS[ PRISON.start ].x, PRISON_POS[ PRISON.start ].y );
--		SetIntData( vRepetionID, HG_DATA_KEY.PRISON_MOVE_TARGET, PRISON.start );
		SetIntData( vRepetionID, HG_DATA_KEY.PRISON_ENTITYID, vParame.entityid );
		SetIntData( vRepetionID, HG_DATA_KEY.PRISON_CAMP, PRISON.camp );
--		SetIntData( vRepetionID, HG_DATA_KEY.BATTLE_PHASE, HG_PHASE.PRISON_GUARD );
		return;
	else
		table.foreachi( PRISON_GUARD, function( i, v )
			if v.index == vParame.index then
				NpcFollow( vRepetionID, vParame.entityid, GetIntData( vRepetionID, HG_DATA_KEY.PRISON_ENTITYID ), 
					v.pos.x, v.pos.y );
				SetCampID( vRepetionID, vParame.entityid, PRISON.camp );			
				add_intdata( vRepetionID, HG_DATA_KEY.PRISON_GUARD_COUNT, 1 );
	--			ObserverEntityDeath( vRepetionID, vParame.entityid );
	--			return 1;
				return 1;
			end
		end);
	end

	table.foreachi( ogre_list, function( i, v )
		table.foreachi( v.tempid, function( k, m )
			if vParame.tempid == m then
--				ChangePKType( vParame.entityid, 12480 );
				ChangePKType( vParame.entityid, 12480 );
				NpcPatrol( vRepetionID, vParame.entityid, 0, v.pos );
				SetCampID( vRepetionID, vParame.entityid, i );
				return 1;
			end	
		end);
	end);	
	
	ObserverEntityDeath( vRepetionID, vParame.entityid );
end

local count;
local more_camp;
local stoppatrol = { };
local index = 0;
local add_time = 0;


local function hg_end( vRepetionID, vCamp )
	Debug( "HG END" );
--	EventNoticeByCamp( vRepetionID, vCamp, RepetionNoticeEventKind.END, RepetionEnd.COMPLATE, 0 );	
--	EventNoticeByCamp( vRepetionID, other_camp( vCamp ), RepetionNoticeEventKind.END, RepetionEnd.UN_COMPLATE, 0 );

	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.CLOSE, close_time, 0 ); 

	-- 进入第三阶段
--	SetIntData( vRepetionID, HG_DATA_KEY.BATTLE_PHASE, HG_PHASE.FIRE_BATTLE );
	

--    PlayerRelive( vRepetionID, Camp.MONKEY, 94, 124 );
--    PlayerRelive( vRepetionID, Camp.HEAVEN, 94, 124 );
	
	TimeBegin( vRepetionID, 6 );
--	RepetionClose( vRepetionID );
end

local function hg_fire_battle( vRepetionID, vCamp )

	SetIntData( vRepetionID, HG_DATA_KEY.BATTLE_PHASE, HG_PHASE.FIRE_BATTLE );

	EventNoticeByCamp( vRepetionID, vCamp, RepetionNoticeEventKind.END, RepetionEnd.COMPLATE, 0 );
	EventNoticeByCamp( vRepetionID, other_camp( vCamp ), RepetionNoticeEventKind.END, RepetionEnd.UN_COMPLATE, 0 );

	insert_item_by_camp( vRepetionID, vCamp, 45550, 3, RepetionInsertItemReason.WIN );
	insert_item_by_camp( vRepetionID, other_camp( vCamp ), 45550, 1, RepetionInsertItemReason.LOST );

	insert_item_by_camp( vRepetionID, vCamp, 66881, 1, RepetionInsertItemReason.WIN );


	TimeEnd( vRepetionID, 1 );
	TimeEnd( vRepetionID, 3 );

	SetIntData( vRepetionID, HG_DATA_KEY.PRISON_CAMP, vCamp );
	index = GetIntData( vRepetionID, HG_DATA_KEY.PRISON_MOVE_TARGET );
	index = get_index( vCamp, index );
	if PRISON_POS[ index ] ~= nil then
		NpcMove( vRepetionID, GetIntData( vRepetionID, HG_DATA_KEY.PRISON_ENTITYID ), PRISON_POS[ index ].x, 
			PRISON_POS[ index ].y );
		SetIntData( vRepetionID, HG_DATA_KEY.PRISON_MOVE_TARGET, index );
	end

	table.foreachi( CampTime, function( i, v )
		TimeEnd( vRepetionID, v );
	end);

	hg_end( vRepetionID, vCamp );
end

local function movestop( vRepetionID, vParame )
	Infor( "Move Stop, index " .. vParame.index .. ", tempid " .. vParame.tempid );
	if vParame.tempid == PRISON.tempid and vParame.index == PRISON.index then
		more_camp = GetIntData( vRepetionID, HG_DATA_KEY.PRISON_CAMP );
		index = GetIntData( vRepetionID, HG_DATA_KEY.PRISON_MOVE_TARGET );
		phase = GetIntData( vRepetionID, HG_DATA_KEY.BATTLE_PHASE );
		if vParame.posx == PRISON_POS[ index ].x and vParame.posy == PRISON_POS[ index ].y then
		-- 囚车移动到达目的点
--			EventNoticeByCamp( vRepetionID, more_camp, RepetionNoticeEventKind.END, 1, 0 ); 
			if index == 1 or index == table.getn( PRISON_POS ) then
				if phase == HG_PHASE.PRISON_GUARD or phase == HG_PHASE.COUNT_DOWN then
			--		hg_end( vRepetionID, more_camp );
					hg_fire_battle( vRepetionID, more_camp );
					return;
				end
			end
		else
			Debug( "Cannt Move Point, Pos( " .. vParame.posx .. " : " .. vParame.posy .. " ) Target " .. index );
--			return;
		end

		index = get_index( more_camp, index );
		if PRISON_POS[ index ] ~= nil then
			NpcMove( vRepetionID, vParame.entityid, PRISON_POS[ index ].x, PRISON_POS[ index ].y );
			SetIntData( vRepetionID, HG_DATA_KEY.PRISON_MOVE_TARGET, index );
		end
	else
		NpcPatrol( vRepetionID, vParame.entityid, -1, stoppatrol );
	end
end


local function get_camp_count( repetion_index, repetion_id, args, loop_index )
	count = GetCampCountEncircleEntity( repetion_id, GetIntData( repetion_id, HG_DATA_KEY.PRISON_ENTITYID ), 
		args.distance, 1 );	

	if count[ "1" ] == count[ "2" ] then
		return;
	elseif count[ "1" ] > count[ "2" ] then
		more_camp = 1;
	else
		more_camp = 2;
	end

	if more_camp == GetIntData( repetion_id, HG_DATA_KEY.PRISON_CAMP ) then
		return;
	end

	Debug( "Change Camp" );
	index = GetIntData( repetion_id, HG_DATA_KEY.PRISON_MOVE_TARGET );
	index = get_index( more_camp, index );
	if PRISON_POS[ index ] ~= nil then
		NpcMove( repetion_id, GetIntData( repetion_id, HG_DATA_KEY.PRISON_ENTITYID ),
			PRISON_POS[ index ].x, PRISON_POS[ index ].y );
		SetIntData( repetion_id, HG_DATA_KEY.PRISON_MOVE_TARGET, index );
	end

	SetIntData( repetion_id, HG_DATA_KEY.PRISON_CAMP, more_camp );
--	ChangeCampIDByTempID( repetion_id, 43257, more_camp );
end


local function relive( repetion_index, repetion_id, args, loop_index )
--	PlayerRelive( repetion_id, 1, args.x, args.y );
--	PlayerRelive( repetion_id, 2, args.x, args.y );
	table.foreachi( player_list, function( i, v )
		PlayerRelive( repetion_id, i, v.relive_pos.x, v.relive_pos.y );	
	end);

	EventNoticeAll(  repetion_id, RepetionNoticeEventKind.RELIVE, RELIVE_TIME, RELIVE_TIME );
end

local function begin_battle( repetion_index, repetion_id, args, loop_index )

	table.foreachi( create_npc, function( i, v )
		CreateNpc( repetion_id, v );
	end);

	NpcMove( repetion_id, GetIntData( repetion_id, HG_DATA_KEY.PRISON_ENTITYID ), PRISON_POS[ PRISON.start ].x, 
		PRISON_POS[ PRISON.start ].y );
	SetIntData( repetion_id, HG_DATA_KEY.PRISON_MOVE_TARGET, PRISON.start );
	SetIntData( repetion_id, HG_DATA_KEY.BATTLE_PHASE, HG_PHASE.PRISON_GUARD );
--	EventNoticeAll( repetion_id, RepetionEventKind.COUNT_DOWN_END, 2, 0 );
	DestroyAliveTempOgre( repetion_id, 52433, 0 );

	--进入第二阶段
end


local function hg_create_npc( repetion_index, repetion_id, args, loop_index )
--	CreateNpc( repetion_id, args[ 1 ] );
	create_hg_ogre( repetion_id );
end



local function create_player( vRepetionID, vParame )
--	ObserverEntityDeath( vRepetionID, vParame.entityid );
--	time = GetTime( vRepetionID, CampTime[ GetCampID( vRepetionID, vParame.entityid ) ] );
--	EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.CLOSE, time, 0 );
	if player_list[ vParame.campid ] ~= nil then
		UseSkill( vParame.entityid, vParame.entityid, player_list[ vParame.campid ].skill );
		time = GetTime( vRepetionID, 2 );		
		EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.RELIVE, time, RELIVE_TIME );

		if GetIntData( vRepetionID, HG_DATA_KEY.BATTLE_PHASE ) == HG_PHASE.BATTLE_WAIT then
--			EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.COUNT_DOWN, 2,
--				GetTime( vRepetionID, 7 ) );
		elseif GetIntData( vRepetionID, HG_DATA_KEY.BATTLE_PHASE ) == HG_PHASE.COUNT_DOWN then
			if CampTime[ vParame.campid ] ~= nil then
--				EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.COUNT_DOWN, 1,
--					GetTime( vRepetionID, CampTime[ vParame.campid ] ) );
			end
		elseif GetIntData( vRepetionID, HG_DATA_KEY.BATTLE_PHASE ) == HG_PHASE.FIRE_BATTLE then
			EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.CLOSE, 
				GetTime( vRepetionID, 6 ) );
		end
	else
		Error( "player " .. vParame.charid .. " campid " .. vParame.campid .. " is error " );
	end
end



local function entity_death( vRepetionID, vParame )
	Debug( "Death TempID " .. vParame.tempid );
	if vParame.tempid == PRISON_GUARD.tempid then
		count = add_intdata( vRepetionID, HG_DATA_KEY.PRISON_GUARD_COUNT, -1 );
		phase = GetIntData( vRepetionID, HG_DATA_KEY.BATTLE_PHASE );
		if count == 0 and phase == HG_PHASE.PRISON_GUARD then
			-- 进入二阶段
			Debug( "Into HG_PHASE.COUNT_DOWN" );
			SetIntData( vRepetionID, HG_DATA_KEY.BATTLE_PHASE, HG_PHASE.COUNT_DOWN );
			TimeBegin( vRepetionID, 1 );
			TimeBegin( vRepetionID, 3 );
			
			create_hg_ogre( vRepetionID );
			
			table.foreachi( CampTime, function( i, v )
				TimeBegin( vRepetionID, v );
				time = GetTime( vRepetionID, v );
--			EventNoticeSingle( vRepetionID, vParame.kill, RepetionNoticeEventKind.CLOSE, time, 0 );
--				EventNoticeByCamp( vRepetionID, i, RepetionNoticeEventKind.COUNT_DOWN, 1, time );
			end);
		end
		return;
	end

	if GetIntData( vRepetionID, HG_DATA_KEY.BATTLE_PHASE ) ~= HG_PHASE.COUNT_DOWN then
		return;
	end

	if vParame.tempid == 45663 or vParame.tempid == 45661 then
		add_time = 3 * 1000;		
	end
	if vParame.tempid == 45662 or vParame.tempid == 45664 then
		add_time = 10 * 1000;
	end
	campid = GetCampID( vRepetionID, vParame.killedid );	
	campid = other_camp( campid );
	time = GetTime( vRepetionID, CampTime[ campid ] );
	time = time + add_time;
	ChangeTime( vRepetionID, CampTime[ campid ], time );		
--	EventNoticeSingle( vRepetionID, vParame.killedid, RepetionNoticeEventKind.CLOSE, time, 0 );

--	EventNoticeByCamp( vRepetionID, campid, RepetionNoticeEventKind.COUNT_DOWN, 1, time );

--	ObserverEntityDeath( vRepetionID, vParame.killedid );
end




local function hg_camp1_lost( repetion_index, repetion_id, args, index )
	Infor( "Camp1 lost" );
--	hg_end( repetion_id, Camp.HEAVEN );
	hg_fire_battle( repetion_id, Camp.HEAVEN );
end

local function hg_camp2_lost( repetion_index, repetion_id, args, index )
	Infor( "Camp2 lost" );
--	hg_end( repetion_id, Camp.MONKEY );
	hg_fire_battle( repetion_id, Camp.MONKEY )
end

local function hg_333( repetion_index, repetion_id, args, index )
--	PlayerRelive( repetion_id, Camp.MONKEY, 94, 124 );
--    PlayerRelive( repetion_id, Camp.HEAVEN, 94, 124 );

	RepetionClose( repetion_id );
end

-- 副本配置如下
repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.HUAGUOSHAN,
		playercountmax = 12,		-- 副本最大人数
		mapid = { 55 },--90,			-- 副本地图ID
		taskid = 0,			-- 进入副本的前置任务
		posx = 94,				-- 暂时不用
		posy = 124,				-- 暂时不用
		pernumber = 1,			-- 每天进入副本的次数； 0-不限
		minlevel = 30,			-- 最低进入等级
		maxlevel = 110,
		unrelive = 1,
		team_level_flag = 1,

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
		open = 
		{
			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 12, minute = 0, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 12, minute = 30, sec = 0 }
			},
			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 13, minute = 30, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 14, minute = 0, sec = 0 }
			},
			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 20, minute = 0, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 20, minute = 30, sec = 0 }
			},
			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 21, minute = 30, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 22, minute = 0, sec = 0 }
			},
		},

	},



    battle =
    {
        camp =
        {
            { campid = Camp.MONKEY, posx = player_list[ Camp.MONKEY ].pos.x, posy = player_list[ Camp.MONKEY ].pos.y },
            { campid = Camp.HEAVEN, posx = player_list[ Camp.HEAVEN ].pos.x, posy = player_list[ Camp.HEAVEN ].pos.y }
        },

        level =
        {
			{ levelmin = 30,    levelmax = 49 },
			{ levelmin = 50,    levelmax = 69 },
			{ levelmin = 70,    levelmax = 99 },
			{ levelmin = 100,    levelmax = 129 },
			{ levelmin = 130,    levelmax = 159 },
        },        
		open =  
		{
			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 12, minute = 30, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 12, minute = 30, sec = 10 }
			},
			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 14, minute = 0, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 14, minute = 0, sec = 10 }
			},
			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 20, minute = 30, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 20, minute = 30, sec = 10 }
			},
			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 22, minute = 0, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 22, minute = 0, sec = 10 }
			},
		}
    },
	-- 定时器
	timer = 
	{ 
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 1000,
			loop = 0,
			func = get_camp_count,
			args = { distance = 10, camp = { one = 1, two = 2 } }
		},
			
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = RELIVE_TIME,
			loop = 0,
			func = relive,
			args = { x = 94, y = 124 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 30000,
			loop = 0,
			func = hg_create_npc,
			args = { 54001, 54002, 54000, 54003, 54004, 54005, 54006, 54007 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = count_down_begin_time,
			loop = 1,
			func = hg_camp1_lost,
			args = { },
			sync = { kind = RepetionTimeSyncKind.CAMP, key = 1, camp = Camp.MONKEY }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result  = RepetionTimeResultKind.NONE,
			time = count_down_begin_time,
			loop = 1,
			func = hg_camp2_lost,
			args = { },
			sync = { kind = RepetionTimeSyncKind.CAMP, key = 1, camp = Camp.HEAVEN }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = close_time,
			loop = 1,
--			func = hg_333,
			args = { }
		},
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = begin_time,
			loop = 1,
			func = begin_battle,
			args = { },
			sync = { kind = RepetionTimeSyncKind.ALL, key = 2 }
		}
	},
	event = 
	{
		[ RepetionEventKind.OGRE_CREATE ] = { func = create_ogre },
		[ RepetionEventKind.ENTITY_MOVE_STOP ] = { func = movestop },
		[ RepetionEventKind.PLAYER_CREATE ] = { func = create_player },
		[ RepetionEventKind.OGRE_DEATH ] = { func = entity_death },
	},
}


