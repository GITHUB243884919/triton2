local TASTE_STONE_LEVEL=
{
	{ min_level = 30, max_level = 39, stone = 57383 },
	{ min_level = 40, max_level = 49, stone = 57384 },
	{ min_level = 50, max_level = 59, stone = 57385 },
	{ min_level = 60, max_level = 69, stone = 57386 },
	{ min_level = 70, max_level = 79, stone = 57387 },
	{ min_level = 80, max_level = 89, stone = 57388 },
	{ min_level = 90, max_level = 100, stone = 57389 }
};

local TASTE_DATA_KEY =
{
	MACHINERY_STAGE_NO	=	1,
	TASTE_STAGE_NO		= 	2,
	OGRE_COUNT			=	3,

	DUJUN_1				=	4,
	DUJUN_2				=	5,
	DUJUN_3				= 	6,
	DUJUN_4				=	7,
	DUJUN_5				=	8,
	DUJUN_6				=	9,
	DUJUN_7				=	10,
	DUJUN_8				=	11,
	DUJUN_9				=	12,
	DUJUN_10			=	13,

	MARK_POINT			= 	14,
	MARK_POINT_MAX		=	15,
	KILL_COUNT			= 	16,

	TASTE_END			= 	17,

	SCORE				= 	18,
	DUJUN_COUNT			=	19,
	BOSS_COUNT			=	20,
};

local TASTE_DIR =
{
	FIRST		= 1,
	SECOND		= 2,
	THIRD		= 3,
	FOURTH		= 4,
	FIFTH		= 5,
};

local TASTE_PATROL =
{
	[ TASTE_DIR.FIRST ] =
	{
		{ x = 23, y = 32 }, { x = 25, y = 31 }, { x = 31, y = 29 }, { x = 33, y = 28 }, { x = 37, y = 25 },
		{ x = 41, y = 24 }, { x = 43, y = 24 }
	},
	[ TASTE_DIR.SECOND ] =
	{
		{ x = 32, y = 47 }, { x = 35, y = 44 }, { x = 39, y = 40 }, { x = 40, y = 37 }, { x = 41, y = 30 }
	},
	[ TASTE_DIR.THIRD ] =
	{
		{ x = 34, y = 47 }, { x = 39, y = 43 }, { x = 42, y = 39 }, { x = 43, y = 35 }, { x = 45, y = 31 }
	},
	[ TASTE_DIR.FOURTH ] =
	{
		{ x = 35, y = 51 }, { x = 38, y = 47 }, { x = 42, y = 43 }, { x = 44, y = 39 }, { x = 47, y = 36 },
		{ x = 50, y = 34 }
	},
	[ TASTE_DIR.FIFTH ] =
	{
		{ x = 54, y = 60 }, { x = 56, y = 56 }, { x = 57, y = 45 }, { x = 55, y = 37 }, { x = 55, y = 33 } 
	}
};

local TASTE_STAGE_LEVEL =
{
	FIRST				= 1,
	SECOND				= 2,
	THIRD				= 3,
	FOURTH				= 4,
	FIFTH				= 5,
	SIXTH				= 6,
	SEVENTH				= 7,
	EIGHTH				= 8,
	NINTH				= 9,
	TENTH				= 10,
	ELEVENTH			= 11,
	TWELFTH				= 12,
	THIRTEENTH			= 13,
	FOURTEENTH			= 14,
	FIFTEENTHLY			= 15,
	SIXTHEENTH			= 16,
	SEVENTHEENTH		= 17,
	EIGHTHEENTH			= 18,
	NINTHEENTH			= 19,
	TWENTIETH			= 20,
	TWENTYFIRST		 	= 21,
	TWENTYSECOND		= 22,
	TWENTYTHIRD			= 23,
	TWENTYFOUR			= 24,
	TWENTYFIVE			= 25,
	TWENTYSIX			= 26,
	TWENTYSEVEN			= 27,
	TWENTYEIGHTH		= 28
};

local MACHINERY =
{
	index = 93149,
};

local DEFAULT_SPACE = 600;
local DEFAULT_CREATE_BOSS_TIMEID = 1;
local DEFAULT_CREATE_OGRE_TIMEID = 2;
local DEFAULT_TIME_BEGIN_TIMEID = 4;

local DEFAULT_DUJUN_TIMEID_1    = 6;
local DEFAULT_DUJUN_MACHINERY1_TIMEID_1     = 7;
local DEFAULT_DUJUN_MACHINERY1_TIMEID_2     = 8;
local DEFAULT_DUJUN_MACHINERY1_TIMEID_3     = 9;
local DEFAULT_DUJUN_MACHINERY2_TIMEID_1     = 10;
local DEFAULT_DUJUN_MACHINERY2_TIMEID_2     = 11;
local DEFAULT_DUJUN_MACHINERY2_TIMEID_3     = 12;
local DEFAULT_DUJUN_MACHINERY3_TIMEID_1     = 13;
local DEFAULT_DUJUN_MACHINERY3_TIMEID_2     = 14;
local DEFAULT_DUJUN_MACHINERY3_TIMEID_3     = 15;
local DEFAULT_DUJUN_MACHINERY3_TIMEID_4     = 16;
local DEFAULT_DUJUN_MACHINERY4_TIMEID_1     = 17;
local DEFAULT_DUJUN_MACHINERY4_TIMEID_2     = 18;
local DEFAULT_DUJUN_MACHINERY4_TIMEID_3     = 19;
local DEFAULT_DUJUN_MACHINERY4_TIMEID_4     = 20;

local DEFAULT_MARK_CLEAR_TIMEID				= 21;

local DEFAULT_CLOSE_TIMEID					= 22;

local CLEAR_MARK_TIME_LENGTH				= 10 * 1000;

local stage = 0;
local count = 0;
local level = 0;
local count_max = 0;
local key = 0;
local score = 0;

local taste_end_info = { time = 0, mark = 0, ogre = 0, stage = 0, score = 0 }


local STONE_MAP_INFO=
{
	mapid = 94,
	posx = 52,
	posy = 26
};

local TASTE_SKILL_OGRE_LIST=
{
	93138, 93143, 93144, 93141, 93140, 93139, 93142, 93145, 93146, 93147
};

local HIEROGRAM=
{
	index = 1;
	list = { 40672, 40674, 40686, 40679, 40766, 40769, 40770, 40757, 40758, 40773 }
};

local HIEROGRAM_POS=
{
	index = 1;
	list =
	{ 
		{ x = 23, y = 10 }, { x = 20, y = 44 }, { x = 49, y = 58 }, { x = 62, y = 52 }, { x = 18, y = 39 },
		{ x = 14, y = 67 }
	}
};

local TASTE_OGRE =
{
	[ 93004 ] = { path = TASTE_DIR.FIRST },	
	[ 93001 ] = { path = TASTE_DIR.SECOND },
	[ 93000 ] = { path = TASTE_DIR.THIRD },
	[ 93002 ] = { path = TASTE_DIR.FOURTH },
	[ 93003 ] = { path = TASTE_DIR.FIFTH },
	[ 93005 ] = { path = TASTE_DIR.FIRST },
	[ 93006 ] = { path = TASTE_DIR.SECOND },
	[ 93007 ] = { path = TASTE_DIR.THIRD },
	[ 93008 ] = { path = TASTE_DIR.FOURTH },
	[ 93009 ] = { path = TASTE_DIR.FIFTH },
	[ 93010 ] = { path = TASTE_DIR.FIRST },
	[ 93011 ] = { path = TASTE_DIR.SECOND },
	[ 93012 ] = { path = TASTE_DIR.THIRD },
	[ 93013 ] = { path = TASTE_DIR.FOURTH },
	[ 93014 ] = { path = TASTE_DIR.FIFTH },
	[ 93015 ] = { path = TASTE_DIR.FIRST },
	[ 93016 ] = { path = TASTE_DIR.SECOND },
	[ 93017 ] = { path = TASTE_DIR.THIRD },
	[ 93018 ] = { path = TASTE_DIR.FOURTH },
	[ 93019 ] = { path = TASTE_DIR.FIFTH },
	[ 93020 ] = { path = TASTE_DIR.FIRST },
	[ 93021 ] = { path = TASTE_DIR.SECOND },
	[ 93022 ] = { path = TASTE_DIR.THIRD },
	[ 93023 ] = { path = TASTE_DIR.FOURTH },
	[ 93024 ] = { path = TASTE_DIR.FIFTH },
	[ 93025 ] = { path = TASTE_DIR.FIRST },
	[ 93026 ] = { path = TASTE_DIR.SECOND },
	[ 93027 ] = { path = TASTE_DIR.THIRD },
	[ 93028 ] = { path = TASTE_DIR.FOURTH },
	[ 93029 ] = { path = TASTE_DIR.FIFTH },
	[ 93030 ] = { path = TASTE_DIR.FIRST },
	[ 93031 ] = { path = TASTE_DIR.SECOND },
	[ 93032 ] = { path = TASTE_DIR.THIRD },
	[ 93033 ] = { path = TASTE_DIR.FOURTH },
	[ 93034 ] = { path = TASTE_DIR.FIFTH },
	[ 93040 ] = { path = TASTE_DIR.FIRST },
	[ 93041 ] = { path = TASTE_DIR.SECOND },
	[ 93042 ] = { path = TASTE_DIR.THIRD },
	[ 93043 ] = { path = TASTE_DIR.FOURTH },
	[ 93044 ] = { path = TASTE_DIR.FIFTH },
	[ 93045 ] = { path = TASTE_DIR.FIRST },
	[ 93046 ] = { path = TASTE_DIR.SECOND },
	[ 93047 ] = { path = TASTE_DIR.THIRD },
	[ 93048 ] = { path = TASTE_DIR.FOURTH },
	[ 93049 ] = { path = TASTE_DIR.FIFTH },
	[ 93050 ] = { path = TASTE_DIR.FIRST },
	[ 93051 ] = { path = TASTE_DIR.SECOND },
	[ 93052 ] = { path = TASTE_DIR.THIRD },
	[ 93053 ] = { path = TASTE_DIR.FOURTH },
	[ 93054 ] = { path = TASTE_DIR.FIFTH },
	[ 93055 ] = { path = TASTE_DIR.FIRST },
	[ 93056 ] = { path = TASTE_DIR.SECOND },
	[ 93057 ] = { path = TASTE_DIR.THIRD },
	[ 93058 ] = { path = TASTE_DIR.FOURTH },
	[ 93059 ] = { path = TASTE_DIR.FIFTH },
	[ 93060 ] = { path = TASTE_DIR.FIRST },
	[ 93061 ] = { path = TASTE_DIR.SECOND },
	[ 93062 ] = { path = TASTE_DIR.THIRD },
	[ 93063 ] = { path = TASTE_DIR.FOURTH },
	[ 93064 ] = { path = TASTE_DIR.FIFTH },
	[ 93065 ] = { path = TASTE_DIR.FIRST },
	[ 93066 ] = { path = TASTE_DIR.SECOND },
	[ 93067 ] = { path = TASTE_DIR.THIRD },
	[ 93068 ] = { path = TASTE_DIR.FOURTH },
	[ 93069 ] = { path = TASTE_DIR.FIFTH },
	[ 93070 ] = { path = TASTE_DIR.FIRST },
	[ 93071 ] = { path = TASTE_DIR.SECOND },
	[ 93072 ] = { path = TASTE_DIR.THIRD },
	[ 93073 ] = { path = TASTE_DIR.FOURTH },
	[ 93074 ] = { path = TASTE_DIR.FIFTH },
	[ 93075 ] = { path = TASTE_DIR.FIRST },
	[ 93076 ] = { path = TASTE_DIR.SECOND },
	[ 93067 ] = { path = TASTE_DIR.THIRD },
	[ 93068 ] = { path = TASTE_DIR.FOURTH },
	[ 93069 ] = { path = TASTE_DIR.FIFTH },
	[ 93070 ] = { path = TASTE_DIR.FIRST },
	[ 93071 ] = { path = TASTE_DIR.SECOND },
	[ 93072 ] = { path = TASTE_DIR.THIRD },
	[ 93073 ] = { path = TASTE_DIR.FOURTH },
	[ 93074 ] = { path = TASTE_DIR.FIFTH },
	[ 93075 ] = { path = TASTE_DIR.FIRST },
	[ 93076 ] = { path = TASTE_DIR.SECOND },
	[ 93067 ] = { path = TASTE_DIR.THIRD },
	[ 93078 ] = { path = TASTE_DIR.FOURTH },
	[ 93079 ] = { path = TASTE_DIR.FIFTH },
	[ 93080 ] = { path = TASTE_DIR.FIRST },
	[ 93081 ] = { path = TASTE_DIR.SECOND },
	[ 93082 ] = { path = TASTE_DIR.THIRD },
	[ 93083 ] = { path = TASTE_DIR.FOURTH },
	[ 93084 ] = { path = TASTE_DIR.FIFTH },
	[ 93085 ] = { path = TASTE_DIR.FIRST },
	[ 93086 ] = { path = TASTE_DIR.SECOND },
	[ 93087 ] = { path = TASTE_DIR.THIRD },
	[ 93088 ] = { path = TASTE_DIR.FOURTH },
	[ 93089 ] = { path = TASTE_DIR.FIFTH },
	[ 93090 ] = { path = TASTE_DIR.SECOND },
	[ 93091 ] = { path = TASTE_DIR.THIRD },
	[ 93092 ] = { path = TASTE_DIR.FOURTH },
	[ 93093 ] = { path = TASTE_DIR.FIFTH },
	[ 93094 ] = { path = TASTE_DIR.FIRST },
	[ 93095 ] = { path = TASTE_DIR.FIRST },
	[ 93096 ] = { path = TASTE_DIR.SECOND },
	[ 93097 ] = { path = TASTE_DIR.THIRD },
	[ 93098 ] = { path = TASTE_DIR.FOURTH },
	[ 93099 ] = { path = TASTE_DIR.FIFTH },
	[ 93100 ] = { path = TASTE_DIR.FIRST },
	[ 93101 ] = { path = TASTE_DIR.SECOND },
	[ 93102 ] = { path = TASTE_DIR.THIRD },
	[ 93103 ] = { path = TASTE_DIR.FOURTH },
	[ 93104 ] = { path = TASTE_DIR.FIFTH },
	[ 93105 ] = { path = TASTE_DIR.FIRST },
	[ 93106 ] = { path = TASTE_DIR.SECOND },
	[ 93107 ] = { path = TASTE_DIR.THIRD },
	[ 93108 ] = { path = TASTE_DIR.FOURTH },
	[ 93109 ] = { path = TASTE_DIR.FIFTH },
	[ 93110 ] = { path = TASTE_DIR.FIRST },
	[ 93111 ] = { path = TASTE_DIR.SECOND },
	[ 93112 ] = { path = TASTE_DIR.THIRD },
	[ 93113 ] = { path = TASTE_DIR.FOURTH },
	[ 93114 ] = { path = TASTE_DIR.FIFTH },
	[ 93115 ] = { path = TASTE_DIR.FIRST },
	[ 93116 ] = { path = TASTE_DIR.SECOND },
	[ 93117 ] = { path = TASTE_DIR.THIRD },
	[ 93118 ] = { path = TASTE_DIR.FOURTH },
	[ 93119 ] = { path = TASTE_DIR.FIFTH },
	[ 93120 ] = { path = TASTE_DIR.FIRST },
	[ 93121 ] = { path = TASTE_DIR.SECOND },
	[ 93122 ] = { path = TASTE_DIR.THIRD },
	[ 93123 ] = { path = TASTE_DIR.FOURTH },
	[ 93124 ] = { path = TASTE_DIR.FIFTH },
	[ 93125 ] = { path = TASTE_DIR.FIRST, create_hier = 1, boss_count = 1 },
	[ 93126 ] = { path = TASTE_DIR.SECOND, create_hier = 1, boss_count = 1 },
	[ 93127 ] = { path = TASTE_DIR.THIRD, create_hier = 1, boss_count = 1 },
	[ 93128 ] = { path = TASTE_DIR.FOURTH, create_hier = 1, boss_count = 1 },
	[ 93129 ] = { path = TASTE_DIR.FIFTH, create_hier = 1, boss_count = 1 },
	[ 93130 ] = { path = TASTE_DIR.FIRST , create_hier = 1, boss_count = 1},
	[ 93131 ] = { path = TASTE_DIR.SECOND, create_hier = 1, boss_count = 1 },
	[ 93132 ] = { path = TASTE_DIR.THIRD, create_hier = 1, boss_count = 1 },
	[ 93133 ] = { path = TASTE_DIR.FOURTH, create_hier = 1, boss_count = 1 },
	[ 93134 ] = { path = TASTE_DIR.FIFTH, create_hier = 1, boss_count = 1 },
	[ 93150 ] = { path = TASTE_DIR.FIRST, create_hier = 1, boss_count = 1 },
	[ 93151 ] = { path = TASTE_DIR.SECOND, create_hier = 1, boss_count = 1 },
	[ 93152 ] = { path = TASTE_DIR.THIRD, create_hier = 1, boss_count = 1 },
	[ 93153 ] = { path = TASTE_DIR.FOURTH, create_hier = 1, boss_count = 1 },
	[ 93154 ] = { path = TASTE_DIR.FIFTH, create_hier = 1, boss_count = 1 },

	[ 93135 ] = { path = TASTE_DIR.SECOND , create_hier = 1, boss_count = 1},
	[ 93136 ] = { path = TASTE_DIR.THIRD, create_hier = 1, boss_count = 1 },
	[ 93137 ] = { path = TASTE_DIR.FOURTH, create_hier = 1, boss_count = 1 },

    -- 督军
    [ 93138 ] = 
	{ 
		num_key = TASTE_DATA_KEY.DUJUN_1, max_num = 1, no_count = 1, create_skill = 56196, death_buff = 56186,
		create_hier = 1, dujun_count = 1,
	},
    [ 93143 ] = 
	{ 
		num_key = TASTE_DATA_KEY.DUJUN_2, max_num = 1, no_count = 1, create_skill = 56196 , death_buff = 56186,
		create_hier = 1, dujun_count = 1,
	},
    [ 93144 ] = 
	{ 
		num_key = TASTE_DATA_KEY.DUJUN_3, max_num = 1, no_count = 1, create_skill = 56196, death_buff = 56186,
		create_hier = 1, dujun_count = 1,
	},
    [ 93141 ] = 
	{ 
		num_key = TASTE_DATA_KEY.DUJUN_4, max_num = 1, no_count = 1, create_skill = 56196 , death_buff = 56186,
		create_hier = 1, dujun_count = 1,
	},
    [ 93140 ] = 
	{ 
		num_key = TASTE_DATA_KEY.DUJUN_5, max_num = 1, no_count = 1, create_skill = 56196 , death_buff = 56186,
		create_hier = 1, dujun_count = 1,
	},
    [ 93139 ] = 
	{ 
		num_key = TASTE_DATA_KEY.DUJUN_6, max_num = 1, no_count = 1, create_skill = 56196 , death_buff = 56186,
		create_hier = 1, dujun_count = 1,
	},
    [ 93142 ] = 
	{ 
		num_key = TASTE_DATA_KEY.DUJUN_7, max_num = 1, no_count = 1, create_skill = 56196 , death_buff = 56186,
		create_hier = 1, dujun_count = 1,
	},
    [ 93145 ] = 
	{ 
		num_key = TASTE_DATA_KEY.DUJUN_8, max_num = 1, no_count = 1, create_skill = 56196 , death_buff = 56186,
		create_hier = 1, dujun_count = 1,
	},
    [ 93146 ] = 
	{ 
		num_key = TASTE_DATA_KEY.DUJUN_9, max_num = 1, no_count = 1, create_skill = 56196  , death_buff = 56186,
		create_hier = 1, dujun_count = 1,
	},
    [ 93147 ] = 
	{ 
		num_key = TASTE_DATA_KEY.DUJUN_10, max_num = 1, no_count = 1, create_skill = 56196  , death_buff = 56186,
		create_hier = 1, dujun_count = 1,
	}

};

local TASTE_STAGE = 
{
	[ TASTE_STAGE_LEVEL.FIRST ] =
	{
		next_stage = TASTE_STAGE_LEVEL.SECOND,
		create_ogre = { count = 5, list = { 93004, 93001, 93000, 93002, 93003 } },
	},
	[ TASTE_STAGE_LEVEL.SECOND ] =
	{
		next_stage = TASTE_STAGE_LEVEL.THIRD,
		create_ogre = { count = 5, list = { 93005, 93006, 93007, 93008, 93009 } },
	},
	[ TASTE_STAGE_LEVEL.THIRD ] =
	{
		next_stage = TASTE_STAGE_LEVEL.FOURTH,
		create_ogre = { count = 5, list = { 93010, 93011, 93012, 93013, 93014 } },
	},
	[ TASTE_STAGE_LEVEL.FOURTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.FIFTH,
		create_ogre = { count = 5, list = { 93015, 93016, 93017, 93018, 93019 } },
	},
	[ TASTE_STAGE_LEVEL.FIFTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.SIXTH,
		create_ogre = { count = 5, list = { 93020, 93021, 93022, 93023, 93024 } },
	},
	[ TASTE_STAGE_LEVEL.SIXTH ] = 
	{
		next_stage = TASTE_STAGE_LEVEL.SEVENTH,
		machinery = 1,
		create_ogre = { count = 6, list = { 93025, 93026, 93027, 93028, 93029 } },
	},
	[ TASTE_STAGE_LEVEL.SEVENTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.EIGHTH,
		create_ogre = { count = 6, list = { 93030, 93031, 93032, 93033, 93034 } },
	},
	[ TASTE_STAGE_LEVEL.EIGHTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.NINTH,
		create_ogre = { count = 6, list = { 93040, 93041, 93042, 93043, 93044 } },
	},
	[ TASTE_STAGE_LEVEL.NINTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.TENTH,
		create_ogre = { count = 6, list = { 93045, 93046, 93047, 93048, 93049 } },
	},
	[ TASTE_STAGE_LEVEL.TENTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.ELEVENTH,
		create_ogre = { count = 6, list = { 93050, 93051, 93052, 93053, 93054 } },
	},
	[ TASTE_STAGE_LEVEL.ELEVENTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.TWELFTH,
		machinery = 1,
		create_ogre = { count = 7, list = { 93055, 93056, 93057, 93058, 93059 } },
	},
	[ TASTE_STAGE_LEVEL.TWELFTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.THIRTEENTH,
		create_ogre = { count = 7, list = { 93060, 93061, 93062, 93063, 93064 } },
	},
	[ TASTE_STAGE_LEVEL.THIRTEENTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.FOURTEENTH,
		create_ogre = { count = 7, list = { 93065, 93066, 93067, 93068, 93069 } },
	},
	[ TASTE_STAGE_LEVEL.FOURTEENTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.FIFTEENTHLY,
		create_ogre = { count = 7, list = { 93070, 93071, 93072, 93073, 93074 } },
	},
	[ TASTE_STAGE_LEVEL.FIFTEENTHLY ] =
	{
		next_stage = TASTE_STAGE_LEVEL.SIXTHEENTH,
		create_ogre = { count = 7, list = { 93150, 93151, 93152, 93153, 93154 } },--93075, 93076, 93077, 93078, 93079}},
	},
	[ TASTE_STAGE_LEVEL.SIXTHEENTH ] = 
	{
		next_stage = TASTE_STAGE_LEVEL.SEVENTHEENTH,
		machinery = 1,
		create_ogre = { count = 7, list = { 93075, 93076, 93077, 93078, 93079 } },--93080, 93081, 93082, 93083, 93084}},
	},
	[ TASTE_STAGE_LEVEL.SEVENTHEENTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.EIGHTHEENTH,
		create_ogre = { count = 7, list = { 93080, 93081, 93082, 93083, 93084 } },--93085, 93086, 93087, 93088, 93089}},
	},
	[ TASTE_STAGE_LEVEL.EIGHTHEENTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.NINTHEENTH,
		create_ogre = { count = 7, list = { 93085, 93086, 93087, 93088, 93089 } },--93090, 93091, 93092, 93093, 93094}},
	},
	[ TASTE_STAGE_LEVEL.NINTHEENTH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.TWENTIETH,
		create_ogre = { count = 7, list = { 93090, 93091, 93092, 93093, 93094 } },--93095, 93096, 93097, 93098, 93099}},
	},
	[ TASTE_STAGE_LEVEL.TWENTIETH ] =
	{
		next_stage = TASTE_STAGE_LEVEL.TWENTYFIRST,
		create_ogre = { count = 7, list = { 93095, 93096, 93097, 93098, 93099 } },--93100, 93101, 93102, 93103, 93104}},
	},
	[ TASTE_STAGE_LEVEL.TWENTYFIRST ] =
	{
		next_stage = TASTE_STAGE_LEVEL.TWENTYSECOND,
		machinery = 1,
		create_ogre = { count = 8, list = { 93100, 93101, 93102, 93103, 93104 } },--93110, 93111, 93112, 93113, 93114}},
	},
	[ TASTE_STAGE_LEVEL.TWENTYSECOND ] =
	{
		next_stage = TASTE_STAGE_LEVEL.TWENTYTHIRD,
		create_ogre = { count = 8, list = { 93110, 93111, 93112, 93113, 93114 } },--93105, 93106, 93107, 93108, 93109}},
	},
	[ TASTE_STAGE_LEVEL.TWENTYTHIRD ] =
	{
		next_stage = TASTE_STAGE_LEVEL.TWENTYFOUR,
		create_ogre = { count = 8, list = { 93105, 93106, 93107, 93108, 93109 } },--93115, 93116, 93117, 93118, 93119}},
	},
	[ TASTE_STAGE_LEVEL.TWENTYFOUR ] =
	{
		next_stage = TASTE_STAGE_LEVEL.TWENTYFIVE,
		create_ogre = { count = 8, list = { 93115, 93116, 93117, 93118, 93119 } },--93120, 93121, 93122, 93123, 93124}},
	},
	[ TASTE_STAGE_LEVEL.TWENTYFIVE ] =
	{
		create_ogre = { count = 8, list = { 93120, 93121, 93122, 93123, 93124}},
	}
	
};

local MACHINERY_STAGE_LEVEL =
{
	NONE		= 0,
	FIRST		= 1,
	SECOND		= 2,
	THIRD		= 3,
	FOURTH		= 4,
};
local MACHINERY_STAGE =
{
    [ MACHINERY_STAGE_LEVEL.NONE ] =
    {
        next_stage = MACHINERY_STAGE_LEVEL.FIRST,
        ogre_list = { 93127 },
    },
    [ MACHINERY_STAGE_LEVEL.FIRST ] =
    {
        next_stage = MACHINERY_STAGE_LEVEL.SECOND,
        ogre_list = { 93131 },
        begin_time = { DEFAULT_DUJUN_MACHINERY1_TIMEID_1, DEFAULT_DUJUN_MACHINERY1_TIMEID_2,
            DEFAULT_DUJUN_MACHINERY1_TIMEID_3 }
    },
    [ MACHINERY_STAGE_LEVEL.SECOND ] =
    {
        next_stage = MACHINERY_STAGE_LEVEL.THIRD,
        ogre_list = { 93135 },
        begin_time = { DEFAULT_DUJUN_MACHINERY2_TIMEID_1, DEFAULT_DUJUN_MACHINERY2_TIMEID_2,
            DEFAULT_DUJUN_MACHINERY2_TIMEID_3 }

    },
    [ MACHINERY_STAGE_LEVEL.THIRD ] =
    {
        next_stage = MACHINERY_STAGE_LEVEL.FOURTH,
        ogre_list = { 93136 },
        begin_time = { DEFAULT_DUJUN_MACHINERY3_TIMEID_1, DEFAULT_DUJUN_MACHINERY3_TIMEID_2,
            DEFAULT_DUJUN_MACHINERY3_TIMEID_3, DEFAULT_DUJUN_MACHINERY3_TIMEID_4 }

    },
    [ MACHINERY_STAGE_LEVEL.FOURTH ] =
    {
        ogre_list = { 93137 },
        begin_time = { DEFAULT_DUJUN_MACHINERY4_TIMEID_1, DEFAULT_DUJUN_MACHINERY4_TIMEID_2,
            DEFAULT_DUJUN_MACHINERY4_TIMEID_3, DEFAULT_DUJUN_MACHINERY4_TIMEID_4 }
    }
};

local TASTE_TAKE_COUNT_KEY = 
{
	MARK_POINT		= 1,
	TASTE_END		= 2,
	MARK_POINT_MAX	= 3,
	TIME			= 4,	
	STAGE			= 5,
	OGRE_COUNT		= 6,
	LEVEL			= 7,
	NEW_STAGE		= 8,
	NEW_MACHINERY	= 9
};

local ESTAMITE =
{
	{ max_count = 30, score = 20 },
	{ max_count = 60, score = 50 },
	{ max_count = 90, score = 80 },
	{ max_count = 125, score = 120 },
	{ max_count = 150, score = 150 }, 
	{ max_count = 250, score = 330 },
	{ max_count = 500, score = 500 },
	{ max_count = 1500, score = 1000 }
};

local function taste_set_mark( repetion_id, cur_count )
	if cur_count == 1 then
		TimeBegin( repetion_id, DEFAULT_MARK_CLEAR_TIMEID );
	elseif cur_count ~= 0 then
		ChangeTime( repetion_id, DEFAULT_MARK_CLEAR_TIMEID, CLEAR_MARK_TIME_LENGTH );
	end

	SetIntData( repetion_id, TASTE_DATA_KEY.MARK_POINT, cur_count );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, TASTE_TAKE_COUNT_KEY.MARK_POINT, cur_count );
end

local function taste_clear_mark( repetion_id )
	count = GetIntData( repetion_id, TASTE_DATA_KEY.MARK_POINT );
	count_max = GetIntData( repetion_id, TASTE_DATA_KEY.MARK_POINT_MAX );

	if count > count_max then
		SetIntData( repetion_id, TASTE_DATA_KEY.MARK_POINT_MAX, count );
	end

	table.foreachi( ESTAMITE, function( i, v )
		if count_max <= v.max_count then
			score = v.score * count_max;
			add_intdata( repetion_id, TASTE_DATA_KEY.SCORE, score );
			return 1;
		end
	end);	

	taste_set_mark( repetion_id, 0 );
end

local function taste_add_mark( repetion_id )
	count = GetIntData( repetion_id, TASTE_DATA_KEY.MARK_POINT );
	taste_set_mark( repetion_id, count + 1 );
end

local function taste_create_hierogram( repetion_id )
	CreateHierogram( repetion_id, HIEROGRAM.list[ HIEROGRAM.index ], HIEROGRAM_POS.list[ HIEROGRAM_POS.index ].x,
		HIEROGRAM_POS.list[ HIEROGRAM_POS.index ].y );

	count = table.getn( HIEROGRAM.list );
	if HIEROGRAM.index >= count then
		HIEROGRAM.index = 1;
	else
		HIEROGRAM.index = HIEROGRAM.index + 1;
	end

	count = table.getn( HIEROGRAM_POS.list );
	if HIEROGRAM_POS.index >= count then
		HIEROGRAM_POS.index = 1;
	else
		HIEROGRAM_POS.index = HIEROGRAM_POS.index + 1;
	end
end

local function taste_stage_add( repetion_id )
	stage = GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_STAGE_NO );
	if TASTE_STAGE[ stage ] ~= nil then
		if TASTE_STAGE[ stage ].next_stage ~= nil then
			SetIntData( repetion_id, TASTE_DATA_KEY.TASTE_STAGE_NO, TASTE_STAGE[ stage ].next_stage );
			stage = TASTE_STAGE[ stage ].next_stage
			Debug( "New Stage " .. stage );
		else
			SetIntData( repetion_id, TASTE_DATA_KEY.TASTE_END, 1 );
			Debug( "Stage " .. stage .. " Next Stage Is NULL" );
		end
	else
		Debug( "Stage " .. stage .. " Is NULL" );
	end

	return stage;
end

local function create_ogre( repetion_id, parame )	
	Debug( "repetion " .. repetion_id .. " create ogre( entityid " .. parame.entityid .. " index " .. parame.index );
	if GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_END ) == 2 then
		return;
	end

	if MACHINERY.index == parame.index then
		ObserverMachinery( repetion_id, parame.entityid );
		return;
	end
	
	if TASTE_OGRE[ parame.index ] ~= nil  then
--		ChangePKType( parame.entityid, 3121 );
		if TASTE_OGRE[ parame.index ].no_count == nil then
			add_intdata( repetion_id, TASTE_DATA_KEY.OGRE_COUNT, 1 );
		end

		ObserverEntityDeath( repetion_id, parame.entityid );
		if TASTE_PATROL[ TASTE_OGRE[ parame.index ].path ] ~= nil then
			NpcPatrol( repetion_id, parame.entityid, 0, TASTE_PATROL[ TASTE_OGRE[ parame.index ].path ] );	
		else
			Debug( "Index " .. parame.index .. " path is null" );
		end

		table.foreachi( TASTE_SKILL_OGRE_LIST, function( i, v )
			if TASTE_OGRE[ v ].create_skill ~= nil then
				if TASTE_OGRE[ v ].num_key ~= nil then
					count = GetIntData( repetion_id, TASTE_OGRE[ v ].num_key );
					if count > 0 then
						UseSkill( parame.entityid, parame.entityid, TASTE_OGRE[ v ].create_skill );
					end
				end
			end
		end);

		if TASTE_OGRE[ parame.index ].num_key ~= nil then
			add_intdata( repetion_id, TASTE_OGRE[ parame.index ].num_key, 1 );
			Debug( "Create Dujun " .. parame.index );
		end

		if TASTE_OGRE[ parame.index ].create_skill ~= nil then
			UseSkill2Template( repetion_id, parame.entityid, 0, TASTE_OGRE[ parame.index ].create_skill );
--			UseSkill2AllPlayer( repetion_id, parame.entityid, TASTE_OGRE[ parame.index ].create_skill );
		end	


	else
		Debug( "Index " .. parame.index .. " Not In TASTE_OGRE" );
	end
end

local function create_player( repetion_id, parame )
	if parame.new == 1 then	
		insert_item( repetion_id, parame.entityid, 57483, 4, RepetionInsertItemReason.NEW );
	end 

	stage = GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_STAGE_NO );
	EventNoticeSingle( repetion_id, parame.entityid, RepetionNoticeEventKind.TAKE_COUNT, TASTE_TAKE_COUNT_KEY.NEW_STAGE,
		stage );
end

local function taste_create_boss( repetion_index, repetion_id, args, loop_index )
	stage = GetIntData( repetion_id, TASTE_DATA_KEY.MACHINERY_STAGE_NO );
	if MACHINERY_STAGE[ stage ] ~= nil then
		create_npc_all( repetion_id, MACHINERY_STAGE[ stage ].ogre_list );
	end
end

local function create_comm_ogre( repetion_id, loop_index )
	stage = GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_STAGE_NO );
	if TASTE_STAGE[ stage ] ~= nil then
		if TASTE_STAGE[ stage ].create_ogre ~= nil then
			create_npc_all( repetion_id, TASTE_STAGE[ stage ].create_ogre.list );	

			Debug( "index " .. loop_index .. " stage " .. stage );

			if loop_index >= TASTE_STAGE[ stage ].create_ogre.count then
				TimeEnd( repetion_id, DEFAULT_CREATE_OGRE_TIMEID );
				stage = taste_stage_add( repetion_id );
			end

			
		end
	end	

	return stage;
end

local function taste_create_ogre( repetion_index, repetion_id, args, loop_index )
	create_comm_ogre( repetion_id, loop_index );
end

local function taste_begin_create_time( repetion_id )
	stage = GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_STAGE_NO );

	TimeBegin( repetion_id, DEFAULT_CREATE_OGRE_TIMEID );

	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, TASTE_TAKE_COUNT_KEY.NEW_STAGE, stage );

    if TASTE_STAGE[ stage ] ~= nil then
        if TASTE_STAGE[ stage ].space ~= nil then
            ChangeTime( repetion_id, DEFAULT_CREATE_OGRE_TIMEID, TASTE_STAGE[ stage ].space );
        end
    end
end

local function taste_begin_loop_time( repetion_id )	
	taste_begin_create_time( repetion_id );
	TimeBegin( repetion_id, DEFAULT_TIME_BEGIN_TIMEID );
end

local function taste_begin_time( repetion_index, repetion_id, args, loop_index )
	Debug( "Taste Begin" );
	SetIntData( repetion_id, TASTE_DATA_KEY.TASTE_STAGE_NO, TASTE_STAGE_LEVEL.FIRST );
	taste_begin_loop_time( repetion_id );

	level = GetRepeLevel( repetion_id );
	table.foreachi( TASTE_STONE_LEVEL, function( i, v )
		if level >= v.min_level and level <= v.max_level then
			CreateRedstone( repetion_id, v.stone, GetParame( repetion_id, 0 ), STONE_MAP_INFO );
		end
	end);

--	stage = create_comm_ogre( repetion_id );
--	TimeBegin( repetion_id, DEFAULT_TIME_BEGIN_TIMEID );
--	if TASTE_STAGE[ stage ] ~= nil then
--		if TASTE_STAGE[ stage ].space ~= nil then
--			ChangeTime( repetion_id, DEFAULT_TIME_BEGIN_TIMEID, TASTE_STAGE[ stage ].space );
--		end
--	end
end

local function taste_end( repetion_id, is_complate )
	if GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_END ) == 2 then
		Debug( "family taste " .. repetion_id .. " is end" );
		return;
	end

	Infor( "family taste " .. repetion_id .."end, commplate " .. is_complate );
	TimeBegin( repetion_id, DEFAULT_CLOSE_TIMEID );

	SetIntData( repetion_id, TASTE_DATA_KEY.TASTE_END, 2 );

	taste_clear_mark( repetion_id );	

	count_max = GetIntData( repetion_id, TASTE_DATA_KEY.MARK_POINT_MAX );
	count = GetBeginTime( repetion_id );
	count = os.time( ) - count;

	level = GetRepeLevel( repetion_id );

	score = ( level / 60 ) * ( level / 60 ) * GetIntData( repetion_id, TASTE_DATA_KEY.SCORE ) + 
		( level / 50 ) * ( 100  * GetIntData( repetion_id, TASTE_DATA_KEY.BOSS_COUNT ) + 
		10 * GetIntData( repetion_id, TASTE_DATA_KEY.DUJUN_COUNT ) )

	taste_end_info.time = count;
	taste_end_info.mark = count_max;
	taste_end_info.ogre = GetIntData( repetion_id, TASTE_DATA_KEY.KILL_COUNT );
	taste_end_info.stage = GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_STAGE_NO );
	taste_end_info.score = score;

	FamilyRepetionInfoEnd( repetion_id, GetParame( repetion_id, 0 ), taste_end_info );

	AwardFameAll( repetion_id, 16 );

--	level = GetRepeLevel( repetion_id );
--
--	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, TASTE_TAKE_COUNT_KEY.TASTE_END, is_complate );
--	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, TASTE_TAKE_COUNT_KEY.MARK_POINT_MAX, count_max );
--	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, TASTE_TAKE_COUNT_KEY.TIME, count );
--	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, TASTE_TAKE_COUNT_KEY.STAGE, 
--		GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_STAGE_NO ) );
--	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, TASTE_TAKE_COUNT_KEY.OGRE_COUNT, 
--		GetIntData( repetion_id, TASTE_DATA_KEY.KILL_COUNT ));
--	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, TASTE_TAKE_COUNT_KEY.LEVEL, level );
end

local function taste_create_ogre_begin( repetion_index, repetion_id, args, loop_index )
	if GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_END ) == 2 then
		return;
	end
	Debug( "Create Ogre Begin" );
	taste_begin_loop_time( repetion_id );
end

local function machinery_spring( repetion_id, parame )
	if GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_END ) == 2 then
		return;
	end

	taste_begin_loop_time( repetion_id );	

	--
	stage = GetIntData( repetion_id, TASTE_DATA_KEY.MACHINERY_STAGE_NO );
	if MACHINERY_STAGE[ stage ] ~= nil then
		if MACHINERY_STAGE[ stage ].next_stage ~= nil then
			-- 修改机关阶段
			SetIntData( repetion_id, TASTE_DATA_KEY.MACHINERY_STAGE_NO, MACHINERY_STAGE[ stage ].next_stage );

			TimeBegin( repetion_id, DEFAULT_CREATE_BOSS_TIMEID );

			CharacterDie( repetion_id, parame.machineryid, 0 );
		end

		
        if MACHINERY_STAGE[ stage ].begin_time ~= nil then
            table.foreachi( MACHINERY_STAGE[ stage ].begin_time, function( i, v )
                TimeBegin( repetion_id, v );
            end);
        end

		stage = GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_STAGE_NO );
--		EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, TASTE_TAKE_COUNT_KEY.NEW_STAGE, stage );
	else
		Error( "Machinery Stage " .. stage .. " Is Nil" );
	end

	level = GetRepeLevel( repetion_id );
	table.foreachi( TASTE_STONE_LEVEL, function( i, v )
		if level >= v.min_level and level <= v.max_level then
			CreateRedstone( repetion_id, v.stone, GetParame( repetion_id, 0 ), STONE_MAP_INFO );
		end
	end);
end

local function move_stop( repetion_id, parame )
	entity_stop_patrol( repetion_id, parame.entityid );
end

local function ogre_death( repetion_id, parame )
	Debug( "Taste Ogre Death, index" .. parame.index );

	if GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_END ) == 2 then
		return;
	end

	taste_add_mark( repetion_id );

	add_intdata( repetion_id, TASTE_DATA_KEY.KILL_COUNT, 1 );

	if TASTE_OGRE[ parame.index ] ~= nil then
		if TASTE_OGRE[ parame.index ].no_count == nil then
			count = add_intdata( repetion_id, TASTE_DATA_KEY.OGRE_COUNT, -1 );
			if count == 0 then
				stage = GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_STAGE_NO );
				if TASTE_STAGE[ stage ] ~= nil then
					-- 创建开启下次机关
					if TASTE_STAGE[ stage ].machinery ~= nil then
						Debug("Create Machinery " .. repetion_id );
						CreateNpc( repetion_id, MACHINERY.index );
						EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 
							TASTE_TAKE_COUNT_KEY.NEW_MACHINERY, 0 )
					end
					-- 判断结束
					if GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_END ) == 1 then
						if TASTE_STAGE[ stage ].next_stage == nil then
							taste_end( repetion_id, RepetionEnd.COMPLATE );							
						end
					end
				else
					Error( "Taste Stage " .. stage .. " Is Nil" );
				end
			end
		end

		if TASTE_OGRE[ parame.index ].create_hier ~= nil then
			taste_create_hierogram( repetion_id );
		end

		if TASTE_OGRE[ parame.index ].num_key ~= nil then
			add_intdata( repetion_id, TASTE_OGRE[ parame.index ].num_key, -1 );
			Debug( "Death Dujun " .. parame.index );
		end

		if TASTE_OGRE[ parame.index ].death_buff ~= nil then
			NpcRemoveBuff( repetion_id, 0, TASTE_OGRE[ parame.index ].death_buff );
--			UseSkill2Template( repetion_id, parame.killedid, 0, TASTE_OGRE[ parame.index ].death_skill );
--			UseSkill2AllPlayer( repetion_id, parame.entityid, TASTE_OGRE[ parame.index ].death_skill );
		end

		if TASTE_OGRE[ parame.index ].boss_count ~= nil then
			add_intdata( repetion_id, TASTE_DATA_KEY.BOSS_COUNT, 1 );
		end

		if TASTE_OGRE[ parame.index ].dujun_count ~= nil then
			add_intdata( repetion_id, TASTE_DATA_KEY.DUJUN_COUNT, 1 );
		end
	end
end

local function machinery_create_dujun( repetion_index, repetion_id, args, loop_index )
	if GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_END ) == 2 then
		return;
	end
	table.foreachi( args, function( i, v ) 
		if TASTE_OGRE[ v ] ~= nil then
			if TASTE_OGRE[ v ].num_key ~= nil then
				num = GetIntData( repetion_id, TASTE_OGRE[ v ].num_key );
				if num >= TASTE_OGRE[ v ].max_num then
					return 1;
				end
			end
		end
		
		CreateNpc( repetion_id, v );
	end);
end 

local function taste_clear_mark_time( repetion_index, repetion_id, args, loop_index )
	if GetIntData( repetion_id, TASTE_DATA_KEY.TASTE_END ) == 2 then
		return;
	end
	taste_clear_mark( repetion_id );
--	count = GetIntData( repetion_id, TASTE_DATA_KEY.MARK_POINT );
--	count_max = GetIntData( repetion_id, TASTE_DATA_KEY.MARK_POINT_MAX );

--	if count > count_max then
--		SetIntData( repetion_id, TASTE_DATA_KEY.MARK_POINT_MAX, count );
--	end

--	taste_set_mark( repetion_id, 0 );
end

local function taste_end_time( repetion_index, repetion_id, args, loop_index )
	taste_end( repetion_id, RepetionEnd.UN_COMPLATE );
end

local function taste_close( repetion_id, parame )
	taste_end( repetion_id, RepetionEnd.UN_COMPLATE );
end

repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.FAMILY_TASTE,
		playercountmax = 25,		-- 
		mapid = { 94, 95 },
		taskid = 0,			-- into task condition 
		posx = 26,
		posy = 70,
			--	teamin = true,
		pernumber = 0,			-- everday into max number 
		minlevel = 10,
		maxlevel = 110,

		team = 
		{
			teamin = true,		
			count = 1,			-- team redirect player number less
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
				start = { year = 0, month = 0, week = 6, day = 0, hr = 0, minute = 0, sec = 0 },
				finish = { year = 0, month = 0, week = 7, day = 0, hr = 0, minute = 0, sec = 0 }
			},
--			--第二个时段
--			{
--				start = { year = 0, month = 0, week = 0, day = 0, hr = 0, minute = 0, sec = 0 },
--				finish = { year = 0, month = 0, week = 0, day = 0, hr = 0, minute = 0, sec = 0 }
--			},
			-- 最多支持五个时段
		}

	},

    battle =
    {
        camp =
        {
            { campid = 0, posx = 13, posy = 68 },
        },

        level =
        {
			{ levelmin = 10,    levelmax = 49 },
        },          
    },

	timer = 
	{ 
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 1000 * 400,
			func = taste_create_boss,
			loop = 1,
			args = { }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 1500,
			func = taste_create_ogre,
			loop = 0,
			args = { }	
		},
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 1000 * 60,
			func = taste_begin_time,
			loop = 1,
			args = { }	
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 100 * 1000,
			func = taste_create_ogre_begin,
			loop = 4,
			args = { }
		},

       -- 副本结束
        {
            kind = RepetionTimeBeginKind.START_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 60 * 60 * 1000,
			func = taste_end_time,
			loop = 1,
			args = { },
            sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
        },

        -- 副本开始创建督军
        [ DEFAULT_DUJUN_TIMEID_1 ] = 
        {   
            kind = RepetionTimeBeginKind.START_BEGIN, 
            result = RepetionTimeResultKind.NONE, 
            time = 310 * 1000, 
            func = machinery_create_dujun, 
            loop = 1, 
            args = { 93138 } 
        },   
        
        -- 机关一开启创建督军
        [ DEFAULT_DUJUN_MACHINERY1_TIMEID_1 ] = 
        {   
            kind = RepetionTimeBeginKind.EVENT_BEGIN, 
            result = RepetionTimeResultKind.NONE, 
            time = 150 * 1000, 
            func = machinery_create_dujun, 
            loop = 1, 
            args = { 93143 }     
        }, 
        [ DEFAULT_DUJUN_MACHINERY1_TIMEID_2 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 250 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93144 }
        },
        [ DEFAULT_DUJUN_MACHINERY1_TIMEID_3 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 350 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93141 }
        },

        -- 机关二开启创建督军
        [ DEFAULT_DUJUN_MACHINERY2_TIMEID_1 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 150 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93140 }
        },
        [ DEFAULT_DUJUN_MACHINERY2_TIMEID_2 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 250 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93139, 93142 }
        },
        [ DEFAULT_DUJUN_MACHINERY2_TIMEID_3 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 350 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93145, 93146 }
        },

        -- 机关三开启创建督军
        [ DEFAULT_DUJUN_MACHINERY3_TIMEID_1 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 150 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93141 }
        },
        [ DEFAULT_DUJUN_MACHINERY3_TIMEID_2 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 250 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93140, 93139 }
        },
        [ DEFAULT_DUJUN_MACHINERY3_TIMEID_3 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 350 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93142, 93145 }
        },
        [ DEFAULT_DUJUN_MACHINERY3_TIMEID_4 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 450 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93146, 93147 }
        },

        -- 机关四开启创建督军
        [ DEFAULT_DUJUN_MACHINERY4_TIMEID_1 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 120 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93138, 93143 }
        },
        [ DEFAULT_DUJUN_MACHINERY4_TIMEID_2 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 220 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93144, 93141 }
        },
        [ DEFAULT_DUJUN_MACHINERY4_TIMEID_3 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 320 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93140, 93139, 93142 }
        },
        [ DEFAULT_DUJUN_MACHINERY4_TIMEID_4 ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 420 * 1000,
            func = machinery_create_dujun,
            loop = 1,
            args = { 93146, 93147 }
        },

		-- 清空连击点
		[ DEFAULT_MARK_CLEAR_TIMEID ] = 
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = CLEAR_MARK_TIME_LENGTH,
			func = taste_clear_mark_time,
			loop = 1,
			args = { },
			sync = { kind = RepetionTimeSyncKind.ALL,key = 1 }
		},

		-- 副本关闭
		[ DEFAULT_CLOSE_TIMEID ] =
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = 2 * 60 * 1000,
			sync = { kind = RepetionTimeSyncKind.ALL,key = 0 }
		}
	},

	event = 
	{
		[ RepetionEventKind.OGRE_CREATE ] = { func = create_ogre },
		[ RepetionEventKind.PLAYER_CREATE ] = { func = create_player },
		[ RepetionEventKind.BOX_MACHINERY_SPING ] = { func = machinery_spring },
		[ RepetionEventKind.ENTITY_MOVE_STOP ] = { func = move_stop },
		[ RepetionEventKind.OGRE_DEATH ] = { func = ogre_death },  
		[ RepetionEventKind.CLOSE_EVENT ] = { func = taste_close }
	},
}



