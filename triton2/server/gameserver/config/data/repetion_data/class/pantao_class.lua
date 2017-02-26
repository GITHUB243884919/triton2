
pantao_class = class( );

local pt_enco = 
{
	{ levelmin = 20, levelmax = 59, itemid = 45512 },
	{ levelmin = 60, levelmax = 99, itemid = 45512 },
	{ levelmin = 100, levelmax = 129, itemid = 45512 }
};

local assignment_time = 30 * 1000;								--存活时间间隔
--副本评分时间系数
local pt_time_coeff = 5 / 10;
--猴妖评分系数
local monkey_ghost_coeff = 2;

local end_time_second = 30 * 60;							--副本结束时间( 秒 )
local monkey_init_create_time = 30000;
pantao_class.end_time = end_time_second * 1000;	--副本结束时间( 豪秒 )
pantao_class.monkey_create_time = monkey_init_create_time;						--刷出第一个怪物时间
pantao_class.monkey_create_space = 1000;						--怪物刷出间隔
local monkey_create_loop_time = 
{ 	
	monkey_init_create_time, 80000, 130000, 180000, 245000, 310000, 375000, 435000, 495000, 540000, 
	585000, 625000, 665000, 702000, 739000, 774000, 809000, 844000, 905000, 909000, 915000
};

local PT_PATH_DIR =
{
	LEFT	= 1,
	MIDDLE	= 2,
	RIGHT	= 3,
};

pantao_class.PATH_LEFT		= PT_PATH_DIR.LEFT;
pantao_class.PATH_MIDDLE	= PT_PATH_DIR.MIDDLE;
pantao_class.PATH_RIGHT	= PT_PATH_DIR.RIGHT;

local ogre_path =
{
	[ PT_PATH_DIR.LEFT ]= 
	{

--		ogre_list = 
--		{
--			439, 414, 413, 442, 443, 500, 441, 508, 505, 559, 560, 524, 511, 509, 792, 562, 561, 688, 713, 711, 712
--			794, 954, 988, 796, 951, 952, 793
--	
--
--		},
			{ x = 26, y = 27 }, { x = 30, y = 24 }, { x = 34, y = 24 }
	},
	[ PT_PATH_DIR.MIDDLE ]= 
	{ 
--		ogre_list = 
--		{ 
--			350, 145, 346, 348, 349, 383, 404, 382, 381, 379, 410, 411, 412, 409, 405, 1027, 991, 1024, 1025, 1026, 
--			990, 989, 1177, 1174, 1180, 1182, 1173, 1081, 1252, 1080, 1078, 1082, 1451, 1450, 1338, 1306, 1304, 2181,
--			2186, 2189, 2233, 2269, 2316, 2330, 2341, 2342, 2344
--
--		},
			{ x = 37, y = 45 }, { x = 40 , y = 39 }, { x = 44, y = 37 }
	},
	[ PT_PATH_DIR.RIGHT ] =
	{
----		ogre_list =
--		{
--			1343, 1344, 1256, 1255, 1305, 1452, 1453, 1485, 1486, 1487, 1488, 1528, 1529, 1532, 1535, 1542, 1543, 1567,
--			1899, 2102, 2140, 2142, 2165. 2177, 2178
--
--		},
			{ x = 59, y = 54 }, { x = 57, y = 45 }, { x = 57, y = 40 }
	}
}

local PT_DATA_KEY = 
{
	TREE_ENTITYID			= 0,			--蟠桃数EntityID
	ESTAMITE_LOWER			= 1,			--下下评价次数
	ESTAMITE_LOW			= 2,			--下评价次数
	ESTAMITE_MIDDLOW		= 3,			--中下评价次数
	ESTAMITE_MIDD			= 4,			--中评价次数
	ESTAMITE_MIDDHIGH		= 5,			--中上评价次数
	ESTAMITE_HIGH			= 7,			--上评价次数
	ESTAMITE_HIGHER			= 8,			--上上评价次数
	ESTAMITE_GOD			= 9,			--神评价次数
	SERIES_POINT			= 10,			--连击点
	MARK_POINT				= 11,			--总评分
	MONKEY_KING_KILL		= 12,			--猴王击杀次数	
	MONKEY_STONE			= 13,			--石候个数
	MONKEY_GHOST			= 14,			--猴妖个数
	MONKEY_WHITE_KILL		= 15,			--杀死白石猴数量
	PT_END					= 16,			--副本结束标示
};	

--[[ 蟠桃副本Player数据解释 ]]--
local PT_PLAYER_DATA_KEY =
{
	MARK_POINT				= 0;
	EXP						= 1;
};

--评价系统
local Estamite = 
{ 
	--数量				等级							评价系数
	{ count = 5, level = PT_DATA_KEY.ESTAMITE_LOWER, coeff = 4 },--1 },
	{ count = 10, level = PT_DATA_KEY.ESTAMITE_LOW, coeff = 5},--1 },
	{ count = 15, level = PT_DATA_KEY.ESTAMITE_MIDDLOW, coeff = 6},--1 },
	{ count = 25, level = PT_DATA_KEY.ESTAMITE_MIDD, coeff = 8},--1 },
	{ count = 40, level = PT_DATA_KEY.ESTAMITE_MIDDHIGH, coeff = 10 },--1 },
	{ count = 60, level = PT_DATA_KEY.ESTAMITE_HIGH, coeff = 12 },--1 },
	{ count = 80, level = PT_DATA_KEY.ESTAMITE_HIGHER, coeff = 14 },--1 },
	{ count = 100, level = PT_DATA_KEY.ESTAMITE_GOD, coeff = 16 },--1 },
}

local OtherInfo = 
{
	mapid = 21,
	flag = 0
};

--临时变量
local mark_point = 0;
local random = 0;
local count = 0;
local number = 0;
local level = 0;
local tree_entityid = 0;
--副本关闭时间
pantao_class.close_time = 60 * 1000 * 2;

--神符时间
pantao_class.create_hiero_time = { 180000, 420000, 660000, 900000 };

--神符
local Hierogram = 
{
	index = 1;
	list = 
	{
		{ index = 1, tempid = { 40671 } },
		{ index = 1, tempid = { 40672 } },
		{ index = 1, tempid = { 40673 } },
		{ index = 1, tempid = { 40691 } },
		{ index = 1, tempid = { 40674 } },
--		{ index = 1, tempid = { 40675 } },
		{ index = 1, tempid = { 40690 } },
--		{ index = 1, tempid = { 40676 } },
		{ index = 1, tempid = { 40678 } },
		{ index = 1, tempid = { 40689 } },
		{ index = 1, tempid = { 40679 } },
		{ index = 1, tempid = { 40680 } },
		{ index = 1, tempid = { 40681 } },
		{ index = 1, tempid = { 40688 } },
		{ index = 1, tempid = { 40682 } },
		{ index = 1, tempid = { 40683 } },
		{ index = 1, tempid = { 40687 } },
		{ index = 1, tempid = { 40685 } },
		{ index = 1, tempid = { 40686 } },
		{ index = 1, tempid = { 40692 } },
		{ index = 1, tempid = { 40762, 40756, 40761, 40764, 40755, 40758, 40771, 40754, 40767,
								40772, 40768, 40763, 40757, 40765, 40773, 40766, 40759
							  } },
	}
};
--刷新神符坐标
local Point = 
{ 
	index = 1, 
	pos = 
	{ 
		{ x = 21, y = 39 },
		{ x = 24, y = 37 },
		{ x = 23, y = 32 },
		{ x = 23, y = 42 },
		{ x = 27, y = 40 },
		{ x = 26, y = 46 },
		{ x = 41, y = 60 },
		{ x = 45, y = 55 },
		{ x = 49, y = 61 },
		{ x = 32, y = 53 }, 
		{ x = 37, y = 49 },
		{ x = 38, y = 57 }
	}
};
--Buff技能列表
local BuffList = 
{
	index = 1,
	BuffTemp = { 40699, 40701, 40700, 40703, 40710, 40706, 40711, 40707, 40702, 40704, 40712, 40705 },
};

--DeBuff技能列表
local DeBuffList =
{
	index = 1,
	BuffTemp = { 41232, 41233, 41234, 41235, 41236, 41237 },
};
--绿猴技能列表
local GreenBuffList =
{
	index = 1,
	BuffTemp = { 41420, 41421, 41422 };--{ 40829, 40830, 40831 },
};

local PT_TAKECOUNT_KEY = 
{
	SERIES_POINT = 1,			--连击点
	STONE_MONKEY = 2,			--石猴
	MONKEY_GHOST = 3,			--猴妖
	MARK_POINT	= 4,			--评分
	REPE_LEVEL	= 5,			--副本等级
	MONKEY_WHITE_KILL = 6,		--杀死白石猴数量
	ESTAMITE_LOWER			= 7,			--下下评价次数
	ESTAMITE_LOW			= 8,			--下评价次数
	ESTAMITE_MIDDLOW		= 9,			--中下评价次数
	ESTAMITE_MIDD			= 10,			--中评价次数
	ESTAMITE_MIDDHIGH		= 11,			--中上评价次数
	ESTAMITE_HIGH			= 12,			--上评价次数
	ESTAMITE_HIGHER			= 13,			--上上评价次数
	ESTAMITE_GOD			= 14,			--神评价次数

};

local function ogre_patrol( repetion_id, entity_id, path )
	NpcPatrol( repetion_id, entity_id, 0, path );		
end

function pantao_class:pt_ogre_path( repetion_id, temp_id, entity_id, ogre_info )
	if ogre_info.path ~= nil then
		if ogre_path[ ogre_info.path ] == nil then
			return;
		end
		-- 猴妖
		table.foreachi( self.monkey_template, function( i, v )
			if v == temp_id then
				Debug( "Npc " .. temp_id .. " patrol" );
				ogre_patrol( repetion_id, entity_id, ogre_path[ ogre_info.path ] );
				return 1;
			end	
		end);
		-- 六耳猕猴
		if temp_id == self.six_monkey.tempid then
			ogre_patrol( repetion_id, entity_id, ogre_path[ ogre_info.path ] );
			return 1;
		end
	end
end

--创建神符
local function create_hierogram( vRepetionID )
	CreateHierogram( vRepetionID, Hierogram.list[ Hierogram.index ].tempid[ Hierogram.list[ Hierogram.index ].index ],
		Point.pos[ Point.index ].x, Point.pos[ Point.index ].y );

	if Point.pos[ Point.index + 1 ] == nil then
		Point.index = 1;
	else
		Point.index = Point.index + 1;
	end

	if Hierogram.list[ Hierogram.index ].tempid[ Hierogram.list[ Hierogram.index ].index + 1 ] == nil then
		Hierogram.list[ Hierogram.index ].index = 1;
	else
		Hierogram.list[ Hierogram.index ].index = Hierogram.list[ Hierogram.index ].index + 1;
	end

	if Hierogram.list[ Hierogram.index + 1 ] == nil then
		Hierogram.index = 1;
	else
		Hierogram.index = Hierogram.index + 1;
	end
end

--使用绿猴Skill
local function use_skill_green( vRepetionID, vSrcEntityID, vDesEntityID )
	UseSkill( vSrcEntityID, vDesEntityID, GreenBuffList.BuffTemp[ GreenBuffList.index ] );
	if GreenBuffList.BuffTemp[ GreenBuffList.index + 1 ] == nil then
		GreenBuffList.index = 1;
	else
		GreenBuffList.index = GreenBuffList.index + 1;
	end

end

--使用Skill
local function use_skill( vRepetionID, vSrcEntityID, vDesEntityID )
	UseSkill( vSrcEntityID, vDesEntityID, BuffList.BuffTemp[ BuffList.index ] );
	if BuffList.BuffTemp[ BuffList.index + 1 ] == nil then
		BuffList.index = 1;
	else
		BuffList.index = BuffList.index + 1;
	end
end

--使用DeBuff Skill
local function use_deskill( vRepetionID, vSrcEntityID )
	UseSkill2AllPlayer( vRepetionID, vSrcEntityID, DeBuffList.BuffTemp[ DeBuffList.index ] );
	if DeBuffList.BuffTemp[ DeBuffList.index + 1 ] == nil then
		DeBuffList.index = 1;
	else
		DeBuffList.index = DeBuffList.index + 1;
	end
end

--评价添加
local function estamite_add( vRepetionID, estamite )
	number = GetIntData( vRepetionID, estamite );
	SetIntData( vRepetionID, estamite, number + 1 );
end

--计算评价
local function calculate_estamite( vRepetionID )
	count = GetIntData( vRepetionID, PT_DATA_KEY.SERIES_POINT );
	if count == 0 then
		return;
	end
	Debug( "Clean SERIES_POINT " .. count );
	table.foreachi( Estamite, function( i, v )
		if count == v.count then
			estamite_add( vRepetionID, v.level );

			mark_point = GetIntData( vRepetionID, PT_DATA_KEY.MARK_POINT );
			mark_point = mark_point + count * v.coeff;
			Debug( "Mark Point " .. mark_point .. " count " .. count .. " coeff " .. v.coeff );
			SetIntData( vRepetionID, PT_DATA_KEY.MARK_POINT, mark_point );
			return 1;
		end
		if count < v.count then
			if Estamite[ i - 1 ] ~= nil then
				estamite_add( vRepetionID, Estamite[ i - 1 ].level );

				mark_point = GetIntData( vRepetionID, PT_DATA_KEY.MARK_POINT );
				mark_point = mark_point + count * v.coeff;
				Debug( "Mark Point " .. mark_point .. " count " .. count .. " coeff " .. v.coeff );
				SetIntData( vRepetionID, PT_DATA_KEY.MARK_POINT, mark_point );
				return 1;
			else
				return 1;
			end
		end
		if Estamite[ i + 1 ] == nil then
			estamite_add( vRepetionID, v.level );

			mark_point = GetIntData( vRepetionID, PT_DATA_KEY.MARK_POINT );
			mark_point = mark_point + count * v.coeff;
			Debug( "Mark Point " .. mark_point .. " count " .. count .. " coeff " .. v.coeff );
			SetIntData( vRepetionID, PT_DATA_KEY.MARK_POINT, mark_point );
			return 1;
		end
	end);
end

--清除评价
local function clean_series_point( vRepetionID )
	calculate_estamite( vRepetionID );
	
	SetIntData( vRepetionID, PT_DATA_KEY.SERIES_POINT, 0 );
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.SERIES_POINT, 0 );
end

--连击点添加
local function series_point_add( vRepetionID, add )
	count = GetIntData( vRepetionID, PT_DATA_KEY.SERIES_POINT );
	count = count + add;
	SetIntData( vRepetionID, PT_DATA_KEY.SERIES_POINT, count );
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.SERIES_POINT, count );
end

--创建Player
function pantao_class:create_player( vRepetionID, vParame )--, vParame1, vParame2 )
	tCount = GetBeginTime( vRepetionID );
	tCount = ( os.time( ) - tCount ) * 1000;
	if tCount < self.monkey_create_time then
		EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.CREATE_NPC, 0, 
			self.monkey_create_time - tCount );  --通知刷普通猴
	end
	if tCount < self.six_monkey.create_time then
		--通知六耳猕猴刷出
		EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.CREATE_NPC, self.six_monkey.tempid,
			self.six_monkey.create_time - tCount );
	end
	UseSkill( vParame.entityid, vParame.entityid, self.player_skill );
	
	if tCount < self.end_time then
--		EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.CLOSE, self.end_time - tCount, 0 );
	end

	EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 
		PT_TAKECOUNT_KEY.MONKEY_WHITE_KILL, GetIntData( vRepetionID, PT_DATA_KEY.MONKEY_WHITE_KILL ) );
	EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT,
		PT_TAKECOUNT_KEY.SERIES_POINT, GetIntData( vRepetionID, PT_DATA_KEY.SERIES_POINT ) );
end

--创建NPC
function pantao_class:create_ogre( vRepetionID, vParame )--, vParame1, vParame2 )
	Infor( "Call Pantao " .. vRepetionID .. " create_ogre( " .. vParame.entityid .. " : " .. 
		vParame.tempid .. " : " .. vParame.index .. " )" );
	if vParame.tempid == self.tree_template_id then
		UseSkill( vParame.entityid, vParame.entityid, 39853 );
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		SetIntData( vRepetionID, PT_DATA_KEY.TREE_ENTITYID, vParame.entityid );
		return;
	end 
	table.foreachi( self.stone_monkey_template, function( i, v )
		if v == vParame.tempid then
			UseSkill( vParame.entityid, vParame.entityid, 39779 );
			ObserverEntityDeath( vRepetionID, vParame.entityid );
			Debug( "CallObserverEntityDeath, Temp " .. v );

--			count = GetIntData( vRepetionID, PT_DATA_KEY.MONKEY_STONE );
--			count = count + 1;
--			SetIntData( vRepetionID, PT_DATA_KEY.MONKEY_STONE, count );

			add_intdata( vRepetionID, PT_DATA_KEY.MONKEY_STONE, 1 );			

			if vParame.tempid == self.green_stone_monkey_template then
				Debug( "Green Create " .. vParame.entityid );
				ObserverAssignment( vRepetionID, vParame.entityid );
				SetAssignment( vRepetionID, vParame.entityid, assignment_time, OtherInfo );
				return 1;
			end

			if vParame.tempid == self.red_stone_monkey_template then
				Debug( "Red Create " .. vParame.entityid );
				ObserverAssignment( vRepetionID, vParame.entityid );
--				SetAssignment( vRepetionID, vParame.entityid, assignment_time, OtherInfo );
				random = math.random( 1, 100 );
				if random <= 40 then
					SetAliveRefresh( vRepetionID, vParame.entityid, assignment_time, 0 );
				else
					SetAliveRefresh( vRepetionID, vParame.entityid, assignment_time, 1 );
				end
				return 1;
			end

			if vParame.tempid == self.gold_stone_monkey_template then
				Debug( "Gold Create " .. vParame.entityid );
				ObserverAssignment( vRepetionID, vParame.entityid );
				SetAssignment( vRepetionID, vParame.entityid, assignment_time, OtherInfo );
				return 1;
			end

			if vParame.tempid == self.black_stone_monkey_template then
				Debug( "Black Create " .. vParame.entityid );
				random = math.random( 1, 100 );
				if random <= 30 then
					Debug( "Black Stone Will Alive Refresh" );
					SetAliveRefresh( vRepetionID, vParame.entityid, assignment_time, 0 );
				else
					OtherInfo.flag = random;
					ObserverAssignment( vRepetionID, vParame.entityid );
					SetAssignment( vRepetionID, vParame.entityid, assignment_time, OtherInfo );
					OtherInfo.flag = 0;
				end
				return 1;
			end

			table.foreachi( self.monkey_create_twin, function( k, m )
				if vParame.index == m.index then
					random = math.random( 1, 100 );
					if random <= m.probability then
						CreateNpc( vRepetionID, m.twin );
					end
					return 1;
				end
			end);

--			count = GetIntData( vRepetionID, PT_DATA_KEY.MONKEY_STONE );
--			count = count + 1;
--			SetIntData( vRepetionID, PT_DATA_KEY.MONKEY_STONE, count );
			
			return 1;
		end;
	end);
	table.foreachi( self.monkey_template, function( i, v )
		if v == vParame.tempid  then
			ObserverEntityDeath( vRepetionID, vParame.entityid );
--			AddEnmity( vParame.entityid, GetIntData( vRepetionID, PT_DATA_KEY.TREE_ENTITYID ), 1 );
			clean_series_point( vRepetionID );
			return 1;
		end;
	end);

	table.foreachi( self.monkey_ghost, function( i, v )
		if v == vParame.tempid then
			count = GetIntData( vRepetionID, PT_DATA_KEY.MONKEY_GHOST );
			count = count + 1;
			SetIntData( vRepetionID, PT_DATA_KEY.MONKEY_GHOST, count );
			return 1;
		end
	end);
--		AddEnmity2Template( vRepetionID, vParame.entityid, tree_template_id );
	
	if vParame.tempid == self.six_monkey.tempid then--six_monkey_template then
		ObserverEntityDeath( vRepetionID, vParame.entityid );
--		AddEnmity( vParame.entityid, GetIntData( vRepetionID, PT_DATA_KEY.TREE_ENTITYID ), 1 );
	end
end
--蟠桃副本结束
function pantao_class:pt_end( vRepetionID, vIsComplate )
	if GetIntData( vRepetionID, PT_DATA_KEY.PT_END ) ~= 0 then
		return;
	end

	SetIntData( vRepetionID, PT_DATA_KEY.PT_END, 1 );

	TimeBegin( vRepetionID, 2 );

	TimeEnd( vRepetionID, 1 );
	TimeEnd( vRepetionID, 3 );
	TimeEnd( vRepetionID, 4 );
	TimeEnd( vRepetionID, 5 );

	calculate_estamite( vRepetionID );

	mark_point = GetIntData( vRepetionID, PT_DATA_KEY.MARK_POINT );	
	

	if RepetionEnd.COMPLATE == vIsComplate then
		count = GetBeginTime( vRepetionID );
		count = os.time( ) - count;
		count = ( end_time_second - count ) * pt_time_coeff;
		Debug( "Pt Time Mark Is " .. count );
	
		level = GetRepeLevel( vRepetionID );		

		table.foreachi( pt_enco, function( i, v )
			if level >= v.levelmin and level <= v.levelmax then
				insert_item_all( vRepetionID, v.itemid, self.enco_count, RepetionInsertItemReason.WIN );
			end
		end);

		mark_point = mark_point + count;
	end

	Debug( "1 Mark_Point Is " .. mark_point );

	--我日啊， 嫉妒鄙视策划的公式
	mark_point = mark_point * ( 1 +  GetRepeLevel( vRepetionID ) / 60 ) * self.coefficient;

	Debug( "2 Mark_Point Is " .. mark_point );
	if RepetionEnd.COMPLATE == vIsComplate then
		mark_point = mark_point * 12 / 10;
	end
	
	SetIntData( vRepetionID, PT_DATA_KEY.MARK_POINT, mark_point );
	AddIntData2AllPlayer( vRepetionID, PT_PLAYER_DATA_KEY.MARK_POINT, mark_point );

	Debug( "Pt End, Mark_Point Is " .. mark_point );

	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.STONE_MONKEY, 
		GetIntData( vRepetionID, PT_DATA_KEY.MONKEY_STONE ) );
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.MONKEY_GHOST,
		GetIntData( vRepetionID, PT_DATA_KEY.MONKEY_GHOST ) );
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.MARK_POINT, mark_point );
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.REPE_LEVEL,
		GetRepeLevel( vRepetionID ) );

	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.ESTAMITE_LOWER, 
		GetIntData( vRepetionID, PT_DATA_KEY.ESTAMITE_LOWER ) );	
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.ESTAMITE_LOW, 
		GetIntData( vRepetionID, PT_DATA_KEY.ESTAMITE_LOW ) );	
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.ESTAMITE_MIDDLOW, 
		GetIntData( vRepetionID, PT_DATA_KEY.ESTAMITE_MIDDLOW ) );
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.ESTAMITE_MIDD, 
		GetIntData( vRepetionID, PT_DATA_KEY.ESTAMITE_MIDD ) );
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.ESTAMITE_MIDDHIGH, 
		GetIntData( vRepetionID, PT_DATA_KEY.ESTAMITE_MIDDHIGH ) );
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.ESTAMITE_HIGH, 
		GetIntData( vRepetionID, PT_DATA_KEY.ESTAMITE_HIGH ) );
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.ESTAMITE_HIGHER, 
		GetIntData( vRepetionID, PT_DATA_KEY.ESTAMITE_HIGHER ) );
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.ESTAMITE_GOD, 
		GetIntData( vRepetionID, PT_DATA_KEY.ESTAMITE_GOD ) );

	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.END, vIsComplate, 0 ); 
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.CLOSE, self.close_time, 0 ); 
end

local rand = 0;
--Npc死亡
function pantao_class:ogre_death( vRepetionID, vParame, vIndex )--vEntityID, vTempID )
	Infor( "Call Pantao ogre_death( " .. vParame.killedid .. " : " .. vParame.tempid .. " : " .. vParame.index .. 
		" ), RepetionID " .. vRepetionID .. ", killer " .. vParame.killerid );
	if vParame.tempid == self.tree_template_id then
		Infor( "repetion " .. vRepetionID .. " tree die" );
		self:pt_end( vRepetionID, RepetionEnd.UN_COMPLATE );
		SetIntData( vRepetionID, PT_DATA_KEY.TREE_ENTITYID, 0 );
	else
		if vParame.tempid == self.six_monkey.tempid then--six_monkey_template then
			Debug( "six monkey die" .. vRepetionID );
			table.foreachi( self.stone_monkey_template, function( i, v )
				DestroyAliveTempOgre( vRepetionID, v, 0 );
			end);
			table.foreachi( self.monkey_template, function( i, v )
				DestroyAliveTempOgre( vRepetionID, v, 0 );
			end);	

			EventNoticeAll( vRepetionID, RepetionNoticeEventKind.KILL, vParame.killercharid, vParame.tempid );

			--添加杀死六耳猕猴得分
			add_intdata( vRepetionID, PT_DATA_KEY.MARK_POINT, self.six_monkey.coeff );

			self:awardall( vRepetionID, vIndex );			

			self:pt_end( vRepetionID, RepetionEnd.COMPLATE );

--			mark_point = GetIntData( vRepetionID, PT_DATA_KEY.MARK_POINT );
--			mark_point = mark_point + six_monkey.coeff
--			SetIntData( vRepetionID, PT_DATA_KEY.MARK_POINT, mark_point );
		else
			EventNoticeAll( vRepetionID, RepetionNoticeEventKind.KILL, vParame.killercharid, vParame.tempid );
			ClearAssignmentAll( vRepetionID, vParame.killedid, OtherInfo );

			if 	vParame.tempid == self.stone_monkey_template[ 1 ] 
				or vParame.tempid == self.monkey_template[ 1 ] 
				or vParame.tempid == self.monkey_template[ 2 ] then
				count = GetIntData( vRepetionID, PT_DATA_KEY.MONKEY_WHITE_KILL );
				count = count + 1;
				SetIntData( vRepetionID, PT_DATA_KEY.MONKEY_WHITE_KILL, count );
				EventNoticeAll( vRepetionID, RepetionNoticeEventKind.TAKE_COUNT, PT_TAKECOUNT_KEY.MONKEY_WHITE_KILL, 
					count );	
			end


			if vParame.tempid == self.red_stone_monkey_template then
				Debug( "Red Stone Money  " .. vParame.killedid .. " Death Event Begin" );
				Debug( "Destroy All Monkey" );
				table.foreachi( self.stone_monkey_template, function( i, v )
					DestroyAliveTempOgre( vRepetionID, v, 0 );
				end);
				table.foreachi( self.monkey_template, function( i, v )
					DestroyAliveTempOgre( vRepetionID, v, 0 );
				end);	
				Debug( "Red Stone Money  " .. vParame.killedid .. " Death Event End" );
			end

			if vParame.tempid == self.black_stone_monkey_template then
				--Black Stone Money Death Event
				Debug( "Black Stone Money  " .. vParame.killedid .. " Death Event Begin" );
				random = math.random( 1, 100 );
				if random > 90 then
					Debug( "Destroy All Monkey, Random " .. random );
					table.foreachi( self.stone_monkey_template, function( i, v )
						DestroyAliveTempOgre( vRepetionID, v, 0 );
					end);
					table.foreachi( self.monkey_template, function( i, v )
						DestroyAliveTempOgre( vRepetionID, v, 0 );
					end);	
				
				elseif random <= 30  then
					Debug( "Nothing To Do, Random " .. random );
				else 
					Debug( "Insert Buff, Random " .. random );
					use_skill( vRepetionID, vParame.killerid, vParame.killerid );
				end
				
				Debug( "Black Stone Money  " .. vParame.killedid .. " Death Event End" );
			end

			if vParame.tempid == self.gold_stone_monkey_template then
				Debug( "Gold Stone Money  " .. vParame.killedid .. " Death Event Begin" );
				create_hierogram( vRepetionID );
				Debug( "Gold Stone Money " .. vParame.killedid .. " Death Event End" );
			end

			if vParame.tempid == self.green_stone_monkey_template then
				Debug( "Green Stone Money " .. vParame.killedid .. " Death Event Begin" );
				random = math.random( 1, 100 );
				if random <= 10 then
					Debug( "Destroy A Monkey, Random " .. random );
					count = table.foreachi( self.stone_monkey_template, function( i, v )
								count = DestroyAliveTempOgre( vRepetionID, v, 1 );
								if count > 0 then
									return 1;
								end
							end);
					if count == 0 then
						table.foreachi( self.monkey_template, function( i, v )
							count = DestroyAliveTempOgre( vRepetionID, v, 1 );
							if count > 0 then
								return 1;
							end
						end);
					end
				elseif random <= 40 then
					Debug( "Use Skill To All Player, Random " .. random );
					use_skill_green( vRepetionID, vParame.killerid, vParame.killerid );
				else
					Debug( "Refresh Hierogram, Random " .. random );
					create_hierogram( vRepetionID );
				end

				Debug( "Green Stone Money" .. vParame.killedid .. "  Death Event End" );
			end
			
			if vParame.tempid == self.monkey_king.tempid then
				count = GetIntData( vRepetionID, PT_DATA_KEY.MONKEY_KING_KILL );
				count = count + 1;
				SetIntData( vRepetionID, PT_DATA_KEY.MONKEY_KING_KILL, count );

				add_intdata( vRepetionID, PT_DATA_KEY.MARK_POINT, self.monkey_king.coeff );
			end

			table.foreachi( self.stone_monkey_template, function( i, v )
				if v == vParame.tempid then
					series_point_add( vRepetionID, 1 );
					return 1;
				end
			end);

			table.foreachi( self.monkey_death_create, function( i, v )
				if v.index == vParame.index then
					random = math.random( 1, 100 );
					if random <= v.probability then
						CreateNpc( vRepetionID, v.death );
					end
					return 1;
				end
			end);
	
			table.foreachi( self.monkey_ghost, function( i, v )
				if v == vParame.tempid then
					--杀死猴妖， 添加评分
					add_intdata( vRepetionID, PT_DATA_KEY.MARK_POINT, monkey_ghost_coeff );
					return 1;
				end
			end);
			
		end
	end

	EventClose( vRepetionID, RepetionEventKind.OGRE_DEATH );
	EventClose( vRepetionID, RepetionEventKind.TASK_END );
end

--创建猴
function pantao_class:pt_create_monkey( map_id, repetion_id, args, loop_index )
	if args[ loop_index ] ~= nil then
		table.foreachi( args[ loop_index ], function( i, v )
			CreateNpc( repetion_id, v );
		end);
	end
end

function pantao_class:pt_create_monkey_time( map_id, repetion_id, args, loop_index )
	if args[ loop_index ] ~= nil then
		TimeBegin( repetion_id, args[ loop_index ].index );
		if monkey_create_loop_time[ loop_index ] ~= nil then
			if monkey_create_loop_time[ loop_index + 1 ] ~= nil then
				Debug( "Change This Time Space" .. monkey_create_loop_time[ loop_index + 1 ] - monkey_create_loop_time[ loop_index ] .. " index " .. loop_index );
				ChangeTimeSpace( repetion_id, 4, 
					monkey_create_loop_time[ loop_index + 1 ] - monkey_create_loop_time[ loop_index ] );
			else
			--	Error( "monkey_create_loop_time[ " .. loop_index + 1 .. " ] is nil" );
			end
		else
			Error( "monkey_create_loop_time[ " .. loop_index .. " ] is nil" );	
		end
	end
end

function pantao_class:time_assignment( vRepetionID, vParame ) 
    Debug( "Call Assignment, vParame index " .. vParame.index .. " tempid " .. vParame.tempid .. " flag " .. 
		vParame.flag );

	ClearAssignment( vRepetionID, vParame.entityid, vParame.timeid );
	CharacterDie( vRepetionID, vParame.entityid, 0 );

	if vParame.tempid == self.red_stone_monkey_template then
		Debug( "Red Assignment " .. vParame.entityid );
		random = math.random( 1, 100 );
	end

	if vParame.tempid == self.green_stone_monkey_template then
		Debug( "Green Assignment " .. vParame.entityid );
	end

	if vParame.tempid == self.black_stone_monkey_template then
		Debug( "Black Assignment " .. vParame.entityid .. " Begin" );
		random = vParame.flag;

		if random > 40 then
			Debug( "Do Nothing, Random " .. random );
		else
			Debug( "UseSkill DeBuff, Random " .. random );
			tree_entityid = GetIntData( vRepetionID, PT_DATA_KEY.TREE_ENTITYID );
			if tree_entityid ~= 0 then
			use_deskill( vRepetionID, tree_entityid );
			else
				Infor( "repetion " .. vRepetionID .. " tree is nil" );
			end
		end
		Debug( "Black Assignment " .. vParame.entityid .. " End" );
	end

	if vParame.tempid == self.gold_stone_monkey_template then
		Debug( "Gold Assignment " .. vParame.entityid );
	end

end


function pantao_class:create_hierogram_time( mapid, vRepetionID, args, loop_index )
	create_hierogram( vRepetionID );
	create_hierogram( vRepetionID );
	create_hierogram( vRepetionID );
	create_hierogram( vRepetionID );
	create_hierogram( vRepetionID );

	if self.create_hiero_time[ loop_index + 1 ]	~= nil then
		ChangeTimeSpace( vRepetionID, 24, self.create_hiero_time[ loop_index + 1 ] - 
			self.create_hiero_time[ loop_index ] );
	end
end

function pantao_class:move_stop( repetion_id, parame )
	tree_entityid = GetIntData( repetion_id, PT_DATA_KEY.TREE_ENTITYID );
	if tree_entityid ~= 0 then
		AddEnmity( parame.entityid, tree_entityid, 1 );	
	else
		Infor( "repetion " .. repetion_id .. " tree is nil" );
	end
end

function pantao_class:ogre_info_death( repetion_id, ogre_info )
	if ogre_info.death_create ~= nil then
		table.foreachi( ogre_info.death_create, function( i, v ) 
--			rand = math.random( 1, 100 );
--			if rand <= v.rand then
				table.foreachi( v.index, function( k, m )
					CreateNpc( repetion_id, m );
				end);
--			end
		end);
	end
end

function pantao_class:awardall( vRepetionID, vParame )
	AwardFameAll( vRepetionID, vParame );
end


