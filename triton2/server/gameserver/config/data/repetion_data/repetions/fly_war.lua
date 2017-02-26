local golden_ogre   = { index = 79040 };
local silvery_ogre  = { index = 79044 };
local mr_ogre       = { index = 79052 };
local mis_ogre      = { index = 79053 };
local two_gold		= { id = 60748 };

local angle_relive_time = 5 * 1000 * 60;	-- 金角， 银角复活时间
local pavilion_relive_time = 1 * 1000 * 60;	-- 大人， 夫人复活时间

local FLY_KEY=
{
	FLY_OGRE_COUNT			= 0,
	FLY_KEY_STEP			= 1,
	FLY_JIAO_DIE_COUNT		= 2,
	FLY_LINGYAN_DIE_COUNT 	= 3,
	FLY_ALL_DEATH_COUNT		= 4,
	FLY_JIAO_RELIVE_COUNT	= 5,
	FLY_LINGYAN_RELIVE_COUNT= 6,
	FLY_IS_END				= 7
};

local FLY_OGRE_CREATE_NOTICE=
{
	FLY_TWO_GOLDEN			= 1,	-- 二郎神
	FLY_SIX_MONKEY			= 2,	-- 六耳猕猴
	FLY_DOG					= 3,	-- 哮天犬
	FLY_BIG_GOLDED			= 4,	-- 巨灵神
	FLY_COW					= 5,	-- 牛魔王
	FLY_WATER				= 6,	-- 碧水金精兽
	FLY_WIND				= 7,	-- 黄风大王
	FLY_DRON				= 8,	-- 黑龙王
	FLY_DRON_TRUE			= 9,	-- 黑龙真身
	FLY_GOLDEN_JIAO			= 10,
	FLY_SILVER_JIAO			= 11,
	FLY_MR					= 12,
	FLY_MIS					= 13
};

local OGRE_INDEX_LIST=
{
	[ 79000 ] = { die_time_add = 10 * 1000 },
	[ 79001 ] = { die_time_add = 10 * 1000 },
	[ 79002 ] = { die_time_add = 10 * 1000 },
	[ 79003 ] = { die_time_add = 10 * 1000 },
	[ 79004 ] = { create_hi = { id=61147, x=20, y=150 } },
	[ 79005 ] = { die_time_add = 10 * 1000 },
	[ 79006 ] = { create_hi = { id = 61148, x = 49, y = 177 } },
	[ 79007 ] = { die_time_add = 10 * 1000 },
	[ 79008 ] = { die_time_add = 10 * 1000 },
	[ 79009 ] = { die_time_add = 10 * 1000 },

	[ 79010 ] = { die_time_add = 10 * 1000 },
	[ 79011 ] = { die_time_add = 10 * 1000 },
	[ 79012 ] = { die_time_add = 10 * 1000 },
	[ 79013 ] = { die_time_add = 10 * 1000 },
	[ 79014 ] = { die_time_add = 10 * 1000 },
	[ 79015 ] = { die_time_add = 10 * 1000 },
	[ 79016 ] = { die_time_add = 10 * 1000 },
	[ 79017 ] = { die_time_add = 10 * 1000 },
	[ 79018 ] = { die_time_add = 10 * 1000 },
	[ 79019 ] = { die_time_add = 10 * 1000 },

	[ 79020 ] = { die_time_add = 10 * 1000 },
	[ 79021 ] = { die_time_add = 10 * 1000 },
	[ 79022 ] = { die_time_add = 10 * 1000 },
	[ 79023 ] = { die_time_add = 10 * 1000 },
	[ 79024 ] = { die_time_add = 10 * 1000 },
	[ 79025 ] = { die_time_add = 10 * 1000 },
	[ 79026 ] = { die_time_add = 10 * 1000 },
	[ 79027 ] = { die_time_add = 10 * 1000 },
	[ 79028 ] = { die_time_add = 10 * 1000 },
	[ 79029 ] = { die_time_add = 10 * 1000 },

	[ 79030 ] = { die_time_add = 10 * 1000 },
	[ 79031 ] = { create_hi = { id = 61149, x = 73, y = 188 } },
	[ 79032 ] = { die_time_add = 10 * 1000 },
	[ 79033 ] = { die_time_add = 10 * 1000 },
	[ 79034 ] = { die_time_add = 10 * 1000 },
	[ 79035 ] = { die_time_add = 10 * 1000 },
	[ 79036 ] = { die_time_add = 10 * 1000 },
	[ 79037 ] = { die_time_add = 10 * 1000 },
	[ 79038 ] = { die_time_add = 10 * 1000 },
	[ 79039 ] = { die_time_add = 10 * 1000 },

    [ golden_ogre.index ] = { die_count_key = FLY_KEY.FLY_JIAO_DIE_COUNT, die_begin_time = 2, die_close_time = 3,
		relive_count_key = FLY_KEY.FLY_JIAO_RELIVE_COUNT, 
		msg = { event_key = RepetionNoticeEventKind.MSG, param1=2, param2=angle_relive_time } },
    [ silvery_ogre.index ] = { die_count_key = FLY_KEY.FLY_JIAO_DIE_COUNT, die_begin_time = 3, die_close_time = 2,
		relive_count_key = FLY_KEY.FLY_JIAO_RELIVE_COUNT,
		msg = { event_key = RepetionNoticeEventKind.MSG, param1=3, param2=angle_relive_time } },
	[ 79041 ] = { die_time_add = 10 * 1000 },
	[ 79042 ] = { die_time_add = 10 * 1000 },
	[ 79043 ] = { die_time_add = 10 * 1000 },
	[ 79045 ] = { die_time_add = 10 * 1000 },
	[ 79046 ] = { die_time_add = 10 * 1000 },
	[ 79047 ] = { die_time_add = 10 * 1000 },
	[ 79048 ] = { die_time_add = 10 * 1000 },
	[ 79049 ] = { die_time_add = 10 * 1000 },

	[ 79050 ] = { die_time_add = 10 * 1000 },
	[ 79051 ] = { die_time_add = 10 * 1000 },
    [ mr_ogre.index ] = { die_count_key = FLY_KEY.FLY_LINGYAN_DIE_COUNT, die_begin_time = 4, die_close_time = 5, 
				create_hi = { id = 61150, x = 96, y = 170 }, relive_count_key = FLY_KEY.FLY_LINGYAN_RELIVE_COUNT,
				msg = { event_key = RepetionNoticeEventKind.MSG, param1=4, param2=pavilion_relive_time } },
    [ mis_ogre.index ] = { die_count_key = FLY_KEY.FLY_LINGYAN_DIE_COUNT, die_begin_time = 5, die_close_time = 4,
				relive_count_key = FLY_KEY.FLY_LINGYAN_RELIVE_COUNT, create_hi = { id = 61150, x = 96, y = 170 },
				msg = { event_key = RepetionNoticeEventKind.MSG, param1=5, param2=pavilion_relive_time } },
	[ 79054 ] = { un_create_count = 1 },
	[ 79055 ] = { die_time_add = 10 * 1000 },
	[ 79056 ] = { die_time_add = 10 * 1000 },
	[ 79057 ] = { die_time_add = 10 * 1000 },
	[ 79058 ] = { create_hi = { id = 61151, x = 140, y = 107 } },
	[ 79059 ] = { die_time_add = 10 * 1000 },

	[ 79060 ] = { die_time_add = 10 * 1000 },
	[ 79061 ] = { die_time_add = 10 * 1000 },
	[ 79062 ] = { die_time_add = 10 * 1000 },
	[ 79063 ] = { die_time_add = 10 * 1000 },
	[ 79064 ] = { create_hi = { id = 61152, x = 164, y = 83 } }, 
	[ 79065 ] = { die_time_add = 10 * 1000 },
	[ 79066 ] = { die_time_add = 10 * 1000 },
	[ 79067 ] = { die_time_add = 10 * 1000 },
	[ 79068 ] = { die_time_add = 10 * 1000 },
	[ 79069 ] = { die_time_add = 10 * 1000 },

	[ 79070 ] = { die_time_add = 10 * 1000 },
	[ 79071 ] = { die_time_add = 10 * 1000 },
	[ 79072 ] = { die_time_add = 10 * 1000 },
	[ 79073 ] = { die_time_add = 10 * 1000 },
	[ 79074 ] = { die_time_add = 10 * 1000 },
	[ 79075 ] = { die_time_add = 10 * 1000 },
	[ 79076 ] = { die_time_add = 10 * 1000 },
	[ 79077 ] = { die_time_add = 10 * 1000 },
	[ 79078 ] = { die_time_add = 10 * 1000 },
	[ 79079 ] = { die_time_add = 10 * 1000 },

	[ 79080 ] = { die_time_add = 10 * 1000 },
	[ 79081 ] = { die_time_add = 10 * 1000 },
	[ 79082 ] = { die_time_add = 10 * 1000 },
	[ 79083 ] = { die_time_add = 10 * 1000 },
	[ 79084 ] = { die_time_add = 10 * 1000 },
	[ 79085 ] = { die_time_add = 10 * 1000 },
	[ 79086 ] = { die_time_add = 10 * 1000 },
	[ 79087 ] = { die_time_add = 10 * 1000 },
	[ 79088 ] = { die_time_add = 10 * 1000 },
	[ 79089 ] = { die_time_add = 10 * 1000 },

	[ 79090 ] = { die_time_add = 10 * 1000 },
	[ 79091 ] = { die_time_add = 10 * 1000 },
	[ 79092 ] = { die_time_add = 10 * 1000 },
	[ 79093 ] = { die_time_add = 10 * 1000 },
	[ 79094 ] = { die_time_add = 10 * 1000 },
	[ 79095 ] = { die_time_add = 10 * 1000 },
	[ 79096 ] = { un_create_count = 1 },
	[ 79097 ] = { un_create_count = 1 },
	[ 79098 ] = { un_create_count = 1 },
	[ 79099 ] = { un_create_count = 1 },

	[ 79100 ] = { un_create_count = 1 },
	[ 79101 ] = { un_create_count = 1 },
	[ 79102 ] = { un_create_count = 1 },
	[ 79103 ] = { un_create_count = 1 },
	[ 79104 ] = { un_create_count = 1 },
	[ 79105 ] = { un_create_count = 1 },
	[ 79106 ] = { un_create_count = 1 },
	[ 79107 ] = { un_create_count = 1 },
	[ 79108 ] = { un_create_count = 1 },
	[ 79109 ] = { un_create_count = 1 },

	[ 79100 ] = { un_create_count = 1 },
	[ 79111 ] = { un_create_count = 1 },
	[ 79112 ] = { un_create_count = 1 },
	[ 79113 ] = { un_create_count = 1 },
	[ 79114 ] = { un_create_count = 1 },
--	[ 79115 ] = { die_time_add = 10 * 1000 },
	[ 79116 ] = { un_create_count = 1 }
};

local FLY_STEP=
{
	FIRST 		= 0,
	SECOND		= 1,
	THIRD		= 2,
	FOURTH		= 3,
	FIFTH		= 4,
	SIXTH		= 5,
	SEVENTH		= 6,
	EIGHTH		= 7,
	NINTH		= 8,
	TENTH		= 9,
	ELEVENTH	= 10,	
	TWELEVE		= 11
};

local CREATE_OGRE_SEQUENCE =
{
	[ FLY_STEP.FIRST ] = { 79000, 79004, 79001, 79003, 79003 },
	[ FLY_STEP.SECOND ] = { 79010, 79011, 79005, 79009, 79008, 79012, 79007 },
	[ FLY_STEP.THIRD ] = { 79006, 79014, 79013, 79016, 79015, 79018 },
	[ FLY_STEP.FOURTH ] = { 79020, 79024, 79025, 79021, 79023, 79022, 79019 },
	[ FLY_STEP.FIFTH ] = { 79031, 79026, 79030, 79017, 79028, 79029, 79054 },
	[ FLY_STEP.SIXTH ] = { 79039, 79033, 79037, 79036, 79027, 79034, 79035 },
	[ FLY_STEP.SEVENTH ] = { 79040, 79032, 79042, 79045, 79043, 79041, 79038, 79044, 79050, 79046, 79048, 79047, 79049,
							 79051, 79052, 79053, 79096, 79101, 79100, 79104, 79102, 79097, 79098, 79099, 79105, 79103,
							 79111, 79108, 79110, 79109, 79112, 79113, 79106, 79107, 79116, 79114 },
	[ FLY_STEP.EIGHTH ] = { 79058, 79056, 79057, 79055, 79063, 79059, 79060 },
	[ FLY_STEP.NINTH ] = { 79064, 79061, 79065, 79067, 79062, 79069, 79068, 79070 },
	[ FLY_STEP.TENTH ]= { 79066, 79071 },
	[ FLY_STEP.ELEVENTH ]= { 79079, 79078, 79087, 79089, 79073, 79074, 79094, 79077, 79090, 79083, 79088, 79086, 79076,
							 79075, 79092, 79072, 79084, 79085, 79082, 79081 },
	[ FLY_STEP.TWELEVE ] = { 79080, 79091, 79093, 79095 }
};

local OGRE_TMP_LIST=
{
	[ 60773 ] = { death_time_add = 10 * 1000, death_add_count = 1},
	[ 60765 ] = { death_time_add = 10 * 1000, death_add_count = 1},
	[ 60766 ] = { death_time_add = 10 * 1000, death_add_count = 1},
	[ 60775 ] = { death_time_add = 10 * 1000, death_add_count = 1},
	[ 60771 ] = { death_time_add = 10 * 1000, death_add_count = 1},
	[ 60772 ] = { death_time_add = 10 * 1000, death_add_count = 1},
	[ 60774 ] = { death_time_add = 10 * 1000, death_add_count = 1},
	[ 60776 ] = { death_time_add = 10 * 1000, death_add_count = 1},
	[ 60770 ] = { death_time_add = 10 * 1000, death_add_count = 1},
	[ 60767 ] = { death_time_add = 10 * 1000, death_add_count = 1},
	[ 60768 ] = { death_time_add = 10 * 1000, death_add_count = 1},
	[ 60809 ] = { death_time_add = 40 * 1000, death_add_count = 1},
	[ 60756 ] = { death_time_add = 40 * 1000, death_add_count = 1, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_MR },
	[ 60757 ] = { death_time_add = 40 * 1000, death_add_count = 1, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_MIS },
	[ 60836 ] = { death_time_add = 40 * 1000, death_add_count = 1},
	[ 60761 ] = { death_time_add = 40 * 1000, death_add_count = 1},
	[ 60760 ] = { death_time_add = 40 * 1000, death_add_count = 1},
	[ 60759 ] = { death_time_add = 40 * 1000, death_add_count = 1},
	[ 60762 ] = { death_time_add = 40 * 1000, death_add_count = 1},
	[ 60764 ] = { death_time_add = 40 * 1000, death_add_count = 1},
	[ 60751 ] = { death_time_add = 40 * 1000, death_add_count = 1},
	[ 60763 ] = { death_time_add = 40 * 1000, death_add_count = 1},
	[ 60752 ] = { death_time_add = 40 * 1000, death_add_count = 1},
	[ 60755 ] = { death_time_add = 40 * 1000, death_add_count = 1},
	[ 60777 ] = { death_time_add = 40 * 1000, death_add_count = 1},
	[ 60748 ] = { death_time_add = 90 * 1000, death_add_count = 0, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_TWO_GOLDEN},
	[ 60740 ] = { death_time_add = 90 * 1000, death_add_count = 1, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_SIX_MONKEY},
	[ 60749 ] = { death_time_add = 90 * 1000, death_add_count = 1, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_DOG},
	[ 60745 ] = { death_time_add = 90 * 1000, death_add_count = 1, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_BIG_GOLDED},
	[ 60746 ] = { death_time_add = 90 * 1000, death_add_count = 1, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_COW },
	[ 60747 ] = { death_time_add = 90 * 1000, death_add_count = 1, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_WATER },
	[ 60743 ] = { death_time_add = 90 * 1000, death_add_count = 1, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_GOLDEN_JIAO},
	[ 60744 ] = { death_time_add = 90 * 1000, death_add_count = 1, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_SILVER_JIAO},
	[ 60753 ] = { death_time_add = 90 * 1000, death_add_count = 1, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_WIND },
	[ 60741 ] = { death_time_add = 90 * 1000, death_add_count = 1, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_DRON },
	[ 60742 ] = { death_time_add = 90 * 1000, death_add_count = 1, notice_key = FLY_OGRE_CREATE_NOTICE.FLY_DRON_TRUE }
};

local FLY_TAKECOUNT_KEY=
{
	FLY_KILL_OGRE_COUNT		= 1,
	FLY_COST_TIME			= 2,
	FLY_REPE_LEVEL			= 3,
	FLY_JIAO_RELIVE			= 4,
	FLY_LING_RELIVE			= 5,
	FLY_MARK_POINT			= 6,
	FLY_CREATE_BOSS			= 7
};



local count = 0;
local step = 0;
local level = 0;
local timespace = 0;
local markpoint = 0;

local function create_step( repetion_id )
	if GetIntData( repetion_id, FLY_KEY.FLY_IS_END ) == 1 then
		Debug( "fly_war " .. repetion_id .. " is end( create_step )" );
		return;
	end

	step = add_intdata( repetion_id, FLY_KEY.FLY_KEY_STEP, 1 );

	if CREATE_OGRE_SEQUENCE[ step ] ~= nil then
		
		create_npc_all( repetion_id, CREATE_OGRE_SEQUENCE[ step ] );
	end	
end

local function fly_end( repetion_id, is_complate )
	if GetIntData( repetion_id, FLY_KEY.FLY_IS_END ) == 1 then
		Debug( "fly_war " .. repetion_id .. " is end( fly_end )" );
		return;
	end

	Infor( "fly_war " .. repetion_id .. " end " );



	markpoint = 0;

	SetIntData( repetion_id, FLY_KEY.FLY_IS_END, 1 );

	level = GetRepeLevel( repetion_id );	
	timespace = os.time( ) - GetBeginTime( repetion_id );
	Debug( "fly_war " .. repetion_id .. " cost time " .. timespace );
	count = GetIntData( repetion_id, FLY_KEY.FLY_ALL_DEATH_COUNT );
	Debug( "fly_war " .. repetion_id .. " kill ogre " .. count );

	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, FLY_TAKECOUNT_KEY.FLY_REPE_LEVEL, level );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, FLY_TAKECOUNT_KEY.FLY_KILL_OGRE_COUNT, count );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, FLY_TAKECOUNT_KEY.FLY_COST_TIME, timespace );

	markpoint = ( count * count / ( 50 + timespace ) ) * 100;
	if is_complate == RepetionEnd.COMPLATE then
		markpoint = markpoint + 200;
	end

	count = GetIntData( repetion_id, FLY_KEY.FLY_LINGYAN_RELIVE_COUNT );
	Debug( "fly_war " .. repetion_id .. " LINEYAN Relive Count " .. count );
	markpoint = markpoint - 50 * count;
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, FLY_TAKECOUNT_KEY.FLY_LING_RELIVE, count );
	count = GetIntData( repetion_id, FLY_KEY.FLY_JIAO_RELIVE_COUNT );
	Debug( "fly_war " .. repetion_id .. " JIAO Relive Count " .. count );
	markpoint = markpoint - 100 * count;
	if markpoint <= 0 then
		markpoint = 0;
	end
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, FLY_TAKECOUNT_KEY.FLY_JIAO_RELIVE, count );

	markpoint = ( 1 + level / 60 ) * markpoint;
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, FLY_TAKECOUNT_KEY.FLY_MARK_POINT, markpoint );

	AddIntData2AllPlayer( repetion_id, 0, markpoint );	

	AwardFameAll( repetion_id, 17 );

	Debug( "fly_war " .. repetion_id .. " mark point " .. markpoint );
end


local function create_ogre( repetion_id, parame )
	Debug( "fly_war create ogre, index " .. parame.index .." tempid " .. parame.tempid .. " entityid " .. 
		parame.entityid );
	if OGRE_INDEX_LIST[ parame.index ] ~= nil then

		-- 
		if OGRE_INDEX_LIST[ parame.index ].un_create_count == nil then
			-- 记录创建数量 
			add_intdata( repetion_id, FLY_KEY.FLY_OGRE_COUNT, 1 );
		end
		
		ObserverEntityDeath( repetion_id, parame.entityid );
	end	

	if OGRE_TMP_LIST[ parame.tempid ] ~= nil then
		if OGRE_TMP_LIST[ parame.tempid ].notice_key ~= nil then
			EventNoticeAll( repetion_id, RepetionNoticeEventKind.MSG, 1, OGRE_TMP_LIST[ parame.tempid ].notice_key );
		end
	end
end

local function entity_death( repetion_id, parame )
	Debug( "fly_war ogre death, index " .. parame.index .. " tempid " .. parame.tempid .. " entityid " ..
		parame.killedid );
	if GetIntData( repetion_id, FLY_KEY.FLY_IS_END ) == 1 then
		return;
	end
	if OGRE_INDEX_LIST[ parame.index ] ~= nil then

		if OGRE_INDEX_LIST[ parame.index ].die_count_key ~= nil then
			count = add_intdata( repetion_id, OGRE_INDEX_LIST[ parame.index ].die_count_key, 1 );
			Debug( "index " .. parame.index .. " death , Key " .. OGRE_INDEX_LIST[ parame.index ].die_count_key .. 
				" die count " .. count );
			if count >= 2 then
				TimeEnd( repetion_id, OGRE_INDEX_LIST[ parame.index ].die_close_time );	
			else
				if count == 1 then
					if OGRE_INDEX_LIST[ parame.index ].msg ~= nil then
						EventNoticeAll( repetion_id, OGRE_INDEX_LIST[ parame.index ].msg.event_key,
							OGRE_INDEX_LIST[ parame.index ].msg.param1, OGRE_INDEX_LIST[ parame.index ].msg.param2);

					end
				end

				TimeBegin( repetion_id, OGRE_INDEX_LIST[ parame.index ].die_begin_time );
			end
		end

		if OGRE_INDEX_LIST[ parame.index ].un_create_count == nil then
			-- 创建怪物杀完， 创建下一批
			count = add_intdata( repetion_id, FLY_KEY.FLY_OGRE_COUNT, -1 );
			if count <= 0 then
				create_step( repetion_id );				
			end
		end

		if OGRE_INDEX_LIST[ parame.index ].create_hi ~= nil then
			if OGRE_INDEX_LIST[ parame.index ].die_count_key ~= nil then
				count = GetIntData( repetion_id, OGRE_INDEX_LIST[ parame.index ].die_count_key );
				if count >= 2 then
					CreateHierogram( repetion_id, OGRE_INDEX_LIST[ parame.index ].create_hi.id, 
						OGRE_INDEX_LIST[ parame.index ].create_hi.x, OGRE_INDEX_LIST[ parame.index ].create_hi.y );		
				end
			else
				CreateHierogram( repetion_id, OGRE_INDEX_LIST[ parame.index ].create_hi.id, 
					OGRE_INDEX_LIST[ parame.index ].create_hi.x, OGRE_INDEX_LIST[ parame.index ].create_hi.y );
			end
		end




		if OGRE_TMP_LIST[ parame.tempid ] ~= nil then
			time = GetTime( repetion_id, 1 );
			ChangeTime( repetion_id, 1, time + OGRE_TMP_LIST[ parame.tempid ].death_time_add );
		end

		count = add_intdata( repetion_id, FLY_KEY.FLY_ALL_DEATH_COUNT, 1 );

		if parame.tempid == two_gold.id then
			fly_end( repetion_id, FLY_KEY.COMPLATE );
			TimeEnd( repetion_id, 1 );
			TimeBegin( repetion_id, 6 );
		end
	end
end 

local function fly_create_entity( repetion_index, repetion_id, args, loop_index )
	if GetIntData( repetion_id, FLY_KEY.FLY_IS_END ) == 1 then
		return;
	end
	create_npc_all( repetion_id, args );	
	table.foreachi( args, function( i, v )
		if OGRE_INDEX_LIST[ v ] ~= nil then
			if OGRE_INDEX_LIST[ v ].relive_count_key ~= nil then
				add_intdata( repetion_id, OGRE_INDEX_LIST[ v ].relive_count_key, 1 );
			end

			if OGRE_INDEX_LIST[ v ].die_count_key ~= nil then
				add_intdata( repetion_id, OGRE_INDEX_LIST[ v ].die_count_key, -1 );
			end
		end
	end);
end

local function fly_time_end( repetion_index, repetion_id, args, loop_index )
	if GetIntData( repetion_id, FLY_KEY.FLY_IS_END ) == 1 then
		return;
	end
	fly_end( repetion_id, RepetionEnd.UN_COMPLATE );
	RepetionClose( repetion_id );	
end


repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.FLY_WAR,
		playercountmax = 6,		-- 
		mapid = { 80 },
		taskid = 0,			-- into task condition 
		posx = 19,
		posy = 104,
	--	teamin = true,
		pernumber = 1,			-- everday into max number 
                weekpernumber = 4,
		minlevel = 40,
		maxlevel = 120,
		relogin =	1,					-- 1表示可重入0表示不可重入
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
				-- 副本开放时间
		open = 
		{
			-- 年, 月，周， 日， 时, 分， 秒
			-- 0 是不做限时， 周是每月第几周
			--第一个时段

			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 0, minute = 5, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 23, minute = 55, sec = 0 }
			},
			-- 最多支持五个时段
		}
	},

	timer = 
	{ 
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 1000 * 60 * 10,
			loop = 0,
			func = fly_time_end,
			args = {  },
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},


        
        {               
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = angle_relive_time,
            loop = 1,
            func = fly_create_entity,
            args = { golden_ogre.index },
            sync = { kind = RepetionTimeSyncKind.ALL, key = 1 }
        },  
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = angle_relive_time,
            loop = 1,
            func = fly_create_entity,
            args = { silvery_ogre.index },
            sync = { kind = RepetionTimeSyncKind.ALL, key = 2 }
        },
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = pavilion_relive_time,
            loop = 1,
            func = fly_create_entity,
            args = { mr_ogre.index },
            sync = { kind = RepetionTimeSyncKind.ALL, key = 3 }
        },  
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = pavilion_relive_time,
            loop = 1,
            func = fly_create_entity,
            args = { mis_ogre.index },
            sync = { kind = RepetionTimeSyncKind.ALL, key = 4 }
        },
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = 1000 * 60 * 2,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},

	},

	event = 
	{
		[ RepetionEventKind.OGRE_CREATE ] = { func = create_ogre },
--		[ RepetionEventKind.PLAYER_CREATE ] = { func = create_player },
		[ RepetionEventKind.OGRE_DEATH ] = { func = entity_death },
--		[ RepetionEventKind.BOX_MACHINERY_SPING ] = { func = machinery_spring },
--		[ RepetionEventKind.CLOSE_EVENT ] = { func = force_close },
	},
}


