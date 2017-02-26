local battle_close_time = 45 * 60 * 1000;					--关闭时间
local repetion_close_time = 45 * 60 * 1000 + 10000;
local forage_time	= 60000; -- 粮草车刷新时间


local resource1 = 200;		--攻方资源
local resource2 = 200;		--守方资源
local resourcemax = 500;	--资源最大值

local begintime1 = 30000;
local begintime2 = 25000;

local flagindex = 70002;		--大旗索引
local flagtemp  = 45145;	--大旗模板

local flagowner = 2;		--大旗归属阵营ID

local blocktemp = 52433;
local beginbattletime = 120000;

local forageindex1    = 70001;	--粮草车索引
local forageindex2    = 70000;
local foragetemp1     = 49761;	--粮草车模板
local foragetemp2     = 49762;

local citygate_entityid = 0;

local gate_click_time = 0;
local gate_temp = 46839;
local gate_key = 57452;
local gate_index = 70086;
local gate_index1 = 70088;

local campid;
local time;
local CampTime =
{
	[ 1 ] = 5,
	[ 2 ] = 6,
};

local CorpsDataKey = 
{
		DATA_CAMP1_CORPS        = 0,
        DATA_CAMP2_CORPS        = 1,
        DATA_FLAG_OWNER         = 2,
        DATA_BATTLE_CHALLEGE    = 3,
        DATA_BATTLE_SCRABBLE    = 4,
        DATA_CAMP1_RESOURCE     = 5,
        DATA_CAMP2_RESOURCE     = 6,
        DATA_BEGIN_STATE        = 7,
        DATA_BLOCK_NUM          = 8,
		DATA_CLOSE_STATE		= 9,
		DATA_GATE_STATE			= 10,
	DATA_FLAG_ENTITYID	    = 11,
};

local CorpsNoticeKey = 
{
	CORPS_NOTICE_DESFORAGEDEAD = 0x1001,	-- 对方粮草车被击毁
	CORPS_NOTICE_SRCFORAGEDEAD = 0x1002,	-- 己方粮草车被击毁
	CORPS_NOTICE_FORAGEUPDATE  = 0x1003,	-- 粮草车已刷新
	CORPS_NOTICE_GATEKILLED    = 0x1004,	-- 城门被破
	CORPS_NOTICE_FLAGCHANGE    = 0x1005,	-- 大旗变化
};

local router1 = 
{
	{ x = 128, y = 188 },
	{ x = 128, y = 183 },
	{ x = 126, y = 179 },
	{ x = 124, y = 175 },
	{ x = 122, y = 172 },
};

local router2 = 
{
	{ x = 57, y = 240 },
	{ x = 52, y = 240 },
	{ x = 46, y = 239 },
	{ x = 44, y = 237 },
};

local function change_time( vRepetionID, camp_id )
	local tTime = 0;

	if camp_id == 1 then
		tTime = 10 + ( resourcemax - GetIntData( vRepetionID, CorpsDataKey.DATA_CAMP1_RESOURCE ) )/20;
       	tTime = tTime * 1000;
		ChangeTimeSpace( vRepetionID, 1, tTime );
		EventNoticeByCamp( vRepetionID, 1, RepetionNoticeEventKind.RELIVE, tTime, tTime );
	end
	if camp_id == 2 then
		tTime = 10 + ( resourcemax - GetIntData( vRepetionID, CorpsDataKey.DATA_CAMP2_RESOURCE ) )/20;
		tTime = tTime * 1000;
        ChangeTimeSpace( vRepetionID, 2, tTime );
		EventNoticeByCamp( vRepetionID, 2, RepetionNoticeEventKind.RELIVE, tTime, tTime );
	end
end


local function other_camp( camp )
    if camp == 1 then
        return 2;
    else
        return 1;
    end
end

local function increase_resource( vRepetionID, camp_id )
	local temp = 0;	

	if camp_id == 1 then
		temp = GetIntData( vRepetionID, CorpsDataKey.DATA_CAMP1_RESOURCE );
		temp = temp + 80;
		if temp > resourcemax then
			temp = resourcemax;
		end
		SetIntData( vRepetionID, CorpsDataKey.DATA_CAMP1_RESOURCE, temp );
	end
	if camp_id == 2 then
		temp = GetIntData( vRepetionID, CorpsDataKey.DATA_CAMP2_RESOURCE );
		temp = temp + 80;
		if temp > resourcemax then
			temp = resourcemax;
		end
		SetIntData( vRepetionID, CorpsDataKey.DATA_CAMP2_RESOURCE, temp );
	end
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.CORPS_RESOURCE, GetIntData( vRepetionID, CorpsDataKey.DATA_CAMP1_RESOURCE ), GetIntData( vRepetionID, CorpsDataKey.DATA_CAMP2_RESOURCE ) );
end

local function create_ogre( vRepetionID, vParame )
	local tNum = 0;
	
	local tBegin = GetIntData( vRepetionID, CorpsDataKey.DATA_BEGIN_STATE );
	if tBegin == 0 then
		SetIntData( vRepetionID, CorpsDataKey.DATA_CAMP1_RESOURCE, resource1 );
		SetIntData( vRepetionID, CorpsDataKey.DATA_CAMP2_RESOURCE, resource2 );
		SetIntData( vRepetionID, CorpsDataKey.DATA_FLAG_OWNER, 2 );
		SetIntData( vRepetionID, CorpsDataKey.DATA_BEGIN_STATE, 1 );
	end	

	--城门
	if vParame.tempid == gate_temp then
		SetIntData( vRepetionID, CorpsDataKey.DATA_GATE_STATE, 1 );
		SetCampID( vRepetionID, vParame.entityid, 2 );
	end
	if vParame.tempid == gate_key then
		ObserverMachinery( vRepetionID, vParame.entityid );
	end

	if vParame.tempid == blocktemp then
		tNum = GetIntData( vRepetionID, CorpsDataKey.DATA_BLOCK_NUM );
		tNum = tNum + 1;
		SetIntData( vRepetionID, CorpsDataKey.DATA_BLOCK_NUM, tNum );
	end
	
	--大旗
	if vParame.index == flagindex then
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		SetCampID( vRepetionID, vParame.entityid, GetIntData( vRepetionID, CorpsDataKey.DATA_FLAG_OWNER ) );
		UseSkill( vParame.entityid, vParame.entityid, 39853 );
		UseSkill( vParame.entityid, vParame.entityid, 50229 );
		SetIntData( vRepetionID, CorpsDataKey.DATA_FLAG_ENTITYID, vParame.entityid );
	end

	if vParame.index == forageindex1 then
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		NpcPatrol( vRepetionID, vParame.entityid, 1, router2 );
		SetCampID( vRepetionID, vParame.entityid, 1 );
		UseSkill( vParame.entityid, vParame.entityid, 50229 );
	end
	if vParame.index == forageindex2 then
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		NpcPatrol( vRepetionID, vParame.entityid, 1, router1 );
		SetCampID( vRepetionID, vParame.entityid, 2 );
		UseSkill( vParame.entityid, vParame.entityid, 50229 );
	end
end

local function create_player( vRepetionID, vParame )
	local tCount = 0;

	tCount = GetBeginTime( vRepetionID );
	tCount = ( os.time( ) - tCount ) * 1000;
	
	-- 获取大旗所用者阵营
	--flagowner = 2;
	
	-- 玩家进入副本后，通知客户端副本关闭时间
	if tCount < battle_close_time then
		EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.CLOSE, repetion_close_time - tCount, 0 ); 
	end	

	if vParame.campid == 1 then 
		SetIntData( vRepetionID, CorpsDataKey.DATA_CAMP1_CORPS, vParame.corpsid );
		UseSkill( vParame.entityid, vParame.entityid, 50053 );
		tCount = GetTime( vRepetionID, 1 );
		EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.RELIVE, tCount, begintime1 );
	elseif vParame.campid == 2 then
		SetIntData( vRepetionID, CorpsDataKey.DATA_CAMP2_CORPS, vParame.corpsid );
		UseSkill( vParame.entityid, vParame.entityid, 50055 );
		tCount = GetTime( vRepetionID, 2 );              
        EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.RELIVE, tCount, begintime2 );
	end

	-- 通知客户端大旗归属哪个阵营
	ChangeCampFlagByCorps( vRepetionID, vParame.entityid, GetIntData( vRepetionID, CorpsDataKey.DATA_FLAG_OWNER ) );
	EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.FLAGTEMP, flagtemp, 0 );
	-- 通知客户端粮草资源
	EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.CORPS_RESOURCE, GetIntData( vRepetionID, CorpsDataKey.DATA_CAMP1_RESOURCE ), GetIntData( vRepetionID, CorpsDataKey.DATA_CAMP2_RESOURCE ) );

	-- 通知争夺的城池
	SendBattleMapNoticeByCorps( vRepetionID, vParame.entityid );

	local tTime = GetTime( vRepetionID, 5 );
	if tTime > 0 then
		EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.COUNT_DOWN, 2, tTime );
	end
end

local function entity_death( vRepetionID, vParame )
	local temp = 0;
	local owner = 0;

	if vParame.index == flagindex then
		temp = GetIntData( vRepetionID, CorpsDataKey.DATA_FLAG_OWNER );	
		owner = other_camp( temp );
		SetIntData( vRepetionID, CorpsDataKey.DATA_FLAG_OWNER, owner );
		ChangeCampFlagByCorps( vRepetionID, 0, GetIntData( vRepetionID, CorpsDataKey.DATA_FLAG_OWNER ), 1 );
		CreateNpc( vRepetionID, flagindex );
		EventNoticeAll( vRepetionID, RepetionNoticeEventKind.MSG, CorpsNoticeKey.CORPS_NOTICE_FLAGCHANGE, 0 );
	end
	
	if vParame.index == forageindex1 then
		increase_resource( vRepetionID, 2 );
		CreateNpc( vRepetionID, forageindex1 );
		EventNoticeByCamp( vRepetionID, 1, RepetionNoticeEventKind.MSG, CorpsNoticeKey.CORPS_NOTICE_SRCFORAGEDEAD, 0 );
		EventNoticeByCamp( vRepetionID, 2, RepetionNoticeEventKind.MSG, CorpsNoticeKey.CORPS_NOTICE_DESFORAGEDEAD, 0 );
	end
	
	if vParame.index == forageindex2 then                                           
        increase_resource( vRepetionID, 1 );
		CreateNpc( vRepetionID, forageindex2 );
		EventNoticeByCamp( vRepetionID, 2, RepetionNoticeEventKind.MSG, CorpsNoticeKey.CORPS_NOTICE_SRCFORAGEDEAD, 0 );
        EventNoticeByCamp( vRepetionID, 1, RepetionNoticeEventKind.MSG, CorpsNoticeKey.CORPS_NOTICE_DESFORAGEDEAD, 0 );
        end  
end

local function battle_end( vRepetionID, vCamp )
	EventNoticeByCamp( vRepetionID, vCamp, RepetionNoticeEventKind.END, 1, 0 );	
	EventNoticeByCamp( vRepetionID, other_camp( vCamp ), RepetionNoticeEventKind.END, 0, 0 );

	BattleResultNoticeByCorps( vRepetionID, vCamp );
	
	insert_item_by_camp( vRepetionID, vCamp, 45568, 10, RepetionInsertItemReason.WIN );
	insert_item_by_camp( vRepetionID, other_camp( vCamp ), 45568, 3, RepetionInsertItemReason.LOST );
	
	--结束了，增加无敌buff
	EntityInsertBuff( vRepetionID, GetIntData( vRepetionID, CorpsDataKey.DATA_FLAG_ENTITYID ), 71199 );
end

local function repetion_close( repetion_index, repetion_id, args, loop_index )
	RepetionClose( repetion_id );
end

local function battle_close( repetion_index, repetion_id, args, loop_index )
	local flag = GetIntData( repetion_id, CorpsDataKey.DATA_FLAG_OWNER );
	
	battle_end( repetion_id, flag );

	SetIntData( repetion_id, CorpsDataKey.DATA_CLOSE_STATE, 1 );
end

local function force_close( vRepetionID, vParame )
	local flag = GetIntData( vRepetionID, CorpsDataKey.DATA_FLAG_OWNER );	

	local state = GetIntData( vRepetionID, CorpsDataKey.DATA_CLOSE_STATE );

	if state == 1 then
		return ;
	end
	
	battle_end( vRepetionID, flag );
	RepetionClose( vRepetionID );
end

local function relive1( repetion_index, repetion_id, args, loop_index )
    PlayerRelive( repetion_id, 1, args.x1, args.y1 );
	change_time( repetion_id, 1 );
end

local function relive2( repetion_index, repetion_id, args, loop_index )
	PlayerRelive( repetion_id, 2, args.x1, args.y1 );
	change_time( repetion_id, 2 );
end

local function machinery_spring( vRepetionID, vParame )
	if vParame.tempid == gate_key then
		local tGateStatus = GetIntData( vRepetionID, CorpsDataKey.DATA_GATE_STATE );
		if tGateStatus == 1 then
			DestroyAliveTempOgre( vRepetionID, gate_temp, 2 );
			SetIntData( vRepetionID, CorpsDataKey.DATA_GATE_STATE, 0 );
		elseif tGateStatus == 0 then
			CreateNpc( vRepetionID, gate_index );
			CreateNpc( vRepetionID, gate_index1 );
			SetIntData( vRepetionID, CorpsDataKey.DATA_GATE_STATE, 1 );
		end
		ObserverMachinery( vRepetionID, vParame.entityid );
	end
end

local function entity_move_stop( vRepetionID, vParame )
	if vParame.index == forageindex1 then
		DestroyAliveTempOgre( vRepetionID, foragetemp1, 1 );
		increase_resource( vRepetionID, 1 );
	end
	if vParame.index == forageindex2 then
        DestroyAliveTempOgre( vRepetionID, foragetemp2, 1 );
		increase_resource( vRepetionID, 2 );
    end
end

local function get_total_time( vCampID )
	local tTime = 0;
	if vCampID == 1 then
                tTime = 10 + ( resourcemax - GetIntData( vRepetionID, CorpsDataKey.DATA_CAMP1_RESOURCE ) )/20;
                tTime = tTime * 1000;
	end
	if vCampID == 2 then
		tTime = 10 + ( resourcemax - GetIntData( vRepetionID, CorpsDataKey.DATA_CAMP2_RESOURCE ) )/20;
                tTime = tTime * 1000;
	end
	return tTime;
end

local function consume_resource( repetion_index, repetion_id, args, loop_index )
	local temp = 0;

	temp = GetIntData( repetion_id, CorpsDataKey.DATA_CAMP1_RESOURCE );
	temp = temp - 1;
	if temp < 0 then
		temp = 0;
	end
	SetIntData( repetion_id, CorpsDataKey.DATA_CAMP1_RESOURCE, temp );

	--change_time( repetion_id, 1 );

	temp = GetIntData( repetion_id, CorpsDataKey.DATA_CAMP2_RESOURCE );
	temp = temp - 1;
	if temp < 0 then
		temp = 0;
	end
	SetIntData( repetion_id, CorpsDataKey.DATA_CAMP2_RESOURCE, temp );
	
	--change_time( repetion_id, 2 );

	EventNoticeAll( repetion_id, RepetionNoticeEventKind.CORPS_RESOURCE, GetIntData( repetion_id, CorpsDataKey.DATA_CAMP1_RESOURCE ), GetIntData( repetion_id, CorpsDataKey.DATA_CAMP2_RESOURCE ) );
end

local function destroy_block( repetion_index, repetion_id, args, loop_index )
	DestroyAliveTempOgre( repetion_id, blocktemp, GetIntData( repetion_id, CorpsDataKey.DATA_BLOCK_NUM ) );
end

local function refresh_forage( repetion_index, repetion_id, args, loop_index )
	DestroyAliveTempOgre( repetion_id, foragetemp1, 1 );
	DestroyAliveTempOgre( repetion_id, foragetemp2, 1 );
	CreateNpc( repetion_id, forageindex1 );
	CreateNpc( repetion_id, forageindex2 );
end

-- 副本配置如下
repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.CORPS_BATTLE,
		playercountmax = 100,		-- 副本最大人数
		mapid = { 71 },			-- 副本地图ID
		taskid = 0,			-- 进入副本的前置任务
		posx = 94,				-- 暂时不用
		posy = 124,				-- 暂时不用
		pernumber = 0,			-- 每天进入副本的次数； 0-不限
		minlevel = 20,			-- 最低进入等级
		maxlevel = 110,
		unrelive = 1,
		
		
		mintimer = 900,			-- 副本最短存在时间，单位：秒, 默认为5分钟，如果小于5分钟无效

		-- 组队信息
		team = 
		{
			teamin = false,		-- 是否组队进入； ture-需要组队; false - 不需组队
			count = 1,			-- 组队人数下限
		},
		

		backpos = 
		{
			mapid = 1,			-- 暂时不用
			posx = 33,			-- 暂时不用	
			posy = 13			-- 暂时不用
		},
	},


    battle =
    {
        camp =
        {
            { campid = 1, posx = 49, posy = 170 },
            { campid = 2, posx = 77, posy = 142 }
        },

        level =
        {
            { levelmin = 10,    levelmax = 0 },
        },          
    },
	-- 定时器
	timer = 
	{ 			
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = begintime1,
			loop = 0,
			func = relive1,
			args = { x1 = 21, y1 = 191 },
		},
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = begintime2,
			loop = 0,
			func = relive2,
			args = { x1 = 89, y1 = 72 },
		},
		{	
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = repetion_close_time,
			loop = 1,
			func = repetion_close,
			args = { },
		},
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 1000,
			loop = 0,
			func = consume_resource,
			args = { },
		},
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = beginbattletime,
			loop = 1,
			func = destroy_block,
			args = { },
		},
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = battle_close_time,
			loop = 1,
			func = battle_close,
			args = { },
		},
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = forage_time,
            loop = 0,
            func = refresh_forage,
            args = { },
		},
	},
	event = 
	{
		[ RepetionEventKind.PLAYER_CREATE ] = { func = create_player },
		[ RepetionEventKind.OGRE_CREATE ] = { func = create_ogre },		
		[ RepetionEventKind.OGRE_DEATH ] = { func = entity_death },
		[ RepetionEventKind.BOX_MACHINERY_SPING ] = { func = machinery_spring },
		[ RepetionEventKind.ENTITY_MOVE_STOP ] = { func = entity_move_stop },
		[ RepetionEventKind.CLOSE_EVENT ] = { func = force_close },
	},
}
