local TOWER_MAP_DATE_KEY=
{
	FLOOR_NO			= 0,
	OGRE_COUNT			= 1,
	FLOOR_OGRE_BUFF		= 2,
	FLOOR_PLAYER_BUFF	= 3,
	BUFF_ID_67908		= 4,
	BUFF_ID_67903		= 5
};

local TOWER_STEP =
{
	FIRST	= 1,
	SECOND	= 2,
	THIRD	= 3,
	FOURTH	= 4,
	FIFTH	= 5,

	MAX		= 5,
};

local TOWER_DATA_KEY =
{
	MAX_FLOOR		= 0,
	SMALL_POINT		= 1,
	DUJUN_POINT		= 2,
	OGRE_COUNT		= 3,
	BOSS_POINT		= 4,
	STEP			= 5,
	END				= 6
};

local VIRTUAL_MAP_LIST = { 120, 120, 120, 120, 121, 121, 121, 121, 119, 119, 119, 119, 122, 122, 122, 123, 123, 123, 124, 125 };
local REAL_MAP_LIST = { 119, 120, 121, 122, 123, 124, 125, 112, 113, 126 };
local virtual_map_index = 4;

local TEL_INDEX = { 118003, 119003, 119004, 120003, 120004, 121003, 121004, 124003, 124005, 122003, 122004, 123003, 123004 };

local VIRTUAL_MAP_CREAT_TYPE =
{
	RAND	= 1,
	FIXURE	= 2,
}

local TOWER_STEP_EVENT =
{
	[ TOWER_STEP.FIRST ] = { dujun_random = 20 },
	[ TOWER_STEP.SECOND ] = { dujun_random = 40 },
	[ TOWER_STEP.THIRD ] = { dujun_random = 60 },
	[ TOWER_STEP.FOURTH ] = { dujun_random = 80 },
	[ TOWER_STEP.FIFTH ] = { dujun_random = 100 },
};

local BUFF_LIST_67908=
{
		68037, 68046, 68047, 68048, 68049, 68050, 68051, 68052, 68053, 68054, 68055, 68038,
		68056, 68057, 68058, 68059, 68060, 68061, 68062, 68063, 68064, 68065, 68039, 68066,
		68040, 68041, 68042, 68043, 68044, 68045
};

local BUFF_LIST_67903=
{
		68067, 68076, 68077, 68078, 68079, 68080, 68081, 68082, 68083, 68084, 68085, 68068,
		68086, 68087, 68088, 68089, 68090, 68091, 68092, 68093, 68094, 68095, 68069, 68096,
		68070, 68071, 68072, 68073, 68074, 68075
};

local TOWER_POINT_TYPE=
{
	SMALL		=	1,
	DUJUN		=	2,
	BOSS		=	3,
};

local OGRE_TMP_LIST =
{
	[ 68559 ] = { },
	[ 67908 ] = { time_add = 10 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1,
					create_event = 
					{
						{ 
							floor_buff_key = TOWER_MAP_DATE_KEY.BUFF_ID_67908,
						}
					},
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.SMALL }
					},
				 },
	[ 67903 ] = { time_add = 10 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					create_event =
					{
						{
							floor_buff_key = TOWER_MAP_DATE_KEY.BUFF_ID_67903,
						}
					},
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.SMALL }
					},

				},
	[ 67881 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					map_data_set =
					{
						{ key = TOWER_MAP_DATE_KEY.FLOOR_OGRE_BUFF, value = 69038 }
					},
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},
				},
	[ 67882 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					map_data_set =
					{
						{ key = TOWER_MAP_DATE_KEY.FLOOR_OGRE_BUFF, value = 69039 }
					},
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67883 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1,
					map_data_set =
					{
						{ key = TOWER_MAP_DATE_KEY.FLOOR_OGRE_BUFF, value = 69040 }
					},
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67887 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1 ,
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},
				},
	[ 67885 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					map_data_set =
					{
						{ key = TOWER_MAP_DATE_KEY.FLOOR_OGRE_BUFF, value = 69042 }
					},
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67886 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					map_data_set =
					{
						{ key = TOWER_MAP_DATE_KEY.FLOOR_PLAYER_BUFF, value = 69044 }
					},
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67884 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					map_data_set =
					{
						{ key = TOWER_MAP_DATE_KEY.FLOOR_OGRE_BUFF, value = 69041 }
					},
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},
				},
	[ 67888 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67889 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1,
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67890 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1,
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				 },
	[ 67891 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1,
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				 },
	[ 67892 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67893 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67894 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67895 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67897 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67898 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67899 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67900 ] = { 
					time_add = 30 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.DUJUN }
					},

				},
	[ 67909 ] = { 
					time_add = 60 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.BOSS, value = 400 }
					},

				},
	[ 67910 ] = { 
					time_add = 60 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					map_data_set =
					{
						{ key = TOWER_MAP_DATE_KEY.FLOOR_OGRE_BUFF, value = 69059 },
						{ key = TOWER_MAP_DATE_KEY.FLOOR_PLAYER_BUFF, value = 69058 },
					},
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.BOSS,  value = 700 }
					},

				},
	[ 67911 ] = { 
					time_add = 60 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					map_data_set =
					{
						{ key = TOWER_MAP_DATE_KEY.FLOOR_OGRE_BUFF, value = 69060 },
					},
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.BOSS, value = 1000 }
					},

				},
	[ 67912 ] = { 
					time_add = 60 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.BOSS, value = 1500 }
					},

				},
	[ 67913 ] = { 
					time_add = 60 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.BOSS, value = 1250 }
					},

				},
	[ 67914 ] = { 
					time_add = 60 * 1000, map_ogre_create_add = 1, map_ogre_death_dec = -1, 
					death_event =
					{
						{	point_add = TOWER_POINT_TYPE.BOSS, value = 1250 }
					},

				},
	[ 68559 ] = { machinery_spring = 1 },
};

local TELE_CREATE_MODE=
{
	ALL_KILL	= 0,
	RAND_ONE	= 1,
};

local MAP_EVENT =
{
	[ 119 ] = 
	{ 
		virtual_init_create = { 118000, 118001, 118002, 118004, 118005, 118006, 118007, 118008, 118009, 118010 },
		tele_index = { 118003 } ,
		du	=
		{
			[ TOWER_STEP.FIRST ] = { 118011 },
			[ TOWER_STEP.SECOND ] = { 118011, 118012, 118014, 118015, 118016 },
			[ TOWER_STEP.THIRD ] = { 118011, 118013, 118014, 118015, 118016 },
			[ TOWER_STEP.FOURTH ] = { 118011, 118014, 118015, 118016 },
			[ TOWER_STEP.FIFTH ] = { 118011, 118015, 118016 },
		},
		tele_create_mode = TELE_CREATE_MODE.ALL_KILL,
		virtual_map = { virtual_type = VIRTUAL_MAP_CREAT_TYPE.RAND }
	},
	[ 120 ] = 
	{ 
		virtual_init_create = { 119000, 119001, 119002, 119005, 119006, 119007, 119008, 119009, 119010, 119011 },
		tele_index = { 119003 } ,
		du	=
		{
			[ TOWER_STEP.FIRST ] = { 119012 },
			[ TOWER_STEP.SECOND ] = { 119012, 119013, 119015, 119016, 119017 },
			[ TOWER_STEP.THIRD ] = { 119012, 119014, 119015, 119016, 119017 },
			[ TOWER_STEP.FOURTH ] = { 119012, 119015, 119016, 119017 },
			[ TOWER_STEP.FIFTH ] = { 119012, 119016, 119017 },
		},
		tele_create_mode = TELE_CREATE_MODE.ALL_KILL,
		virtual_map = { virtual_type = VIRTUAL_MAP_CREAT_TYPE.RAND }
	},
	[ 121 ] = 
	{ 
		virtual_init_create = { 120000, 120001, 120002, 120004, 120005, 120006, 120007, 120008, 120009, 120010, 120017},
		tele_index = { 120003 } ,
		du	=
		{
			[ TOWER_STEP.FIRST ] = { 120011 },
			[ TOWER_STEP.SECOND ] = { 120011, 120012, 120014, 120015, 120016 },
			[ TOWER_STEP.THIRD ] = { 120011, 120013, 120014, 120015, 120016 },
			[ TOWER_STEP.FOURTH ] = { 120011, 120014, 120015, 120016 },
			[ TOWER_STEP.FIFTH ] = { 120011, 120011, 120015, 120016 },
		},	
		tele_create_mode = TELE_CREATE_MODE.ALL_KILL, --TELE_CREATE_MODE.RAND_ONE,
		machinery_ogre_list = { 120018, 120019 },
		virtual_map = { virtual_type = VIRTUAL_MAP_CREAT_TYPE.RAND }
	},
	[ 122 ] = 
	{ 	
		virtual_init_create = { 121000, 121001, 121002, 121004, 121005, 121006, 121007, 121008, 121009, 121010 },
		tele_index = { 121003 }, 
		du	=
		{
			[ TOWER_STEP.FIRST ] = { 121011 },
			[ TOWER_STEP.SECOND ] = { 121011, 121012, 121014, 121015, 121016 },
			[ TOWER_STEP.THIRD ] = { 121011, 121013, 121014, 121015, 121016 },
			[ TOWER_STEP.FOURTH ] = { 121011, 121014, 121015, 121016 },
			[ TOWER_STEP.FIFTH ] = { 121011, 121015, 121016 },
		},
		tele_create_mode = TELE_CREATE_MODE.ALL_KILL,
		virtual_map = { virtual_type = VIRTUAL_MAP_CREAT_TYPE.RAND }
	},
	[ 123 ] = 
	{ 
		virtual_init_create = { 122000, 122001, 122002, 122004, 122005, 122006, 122007, 122008, 122009, 122010, 122017},
		tele_index = { 122003 },
		du	=
		{
			[ TOWER_STEP.FIRST ] = { 122011 },
			[ TOWER_STEP.SECOND ] = { 122011, 122012, 122014, 122015, 122016 },
			[ TOWER_STEP.THIRD ] = { 122011, 122013, 122014, 122015, 122016 },
			[ TOWER_STEP.FOURTH ] = { 122011, 122014, 122015, 122016 },
			[ TOWER_STEP.FIFTH ] = { 122011, 122015, 122016 },
		},
		tele_create_mode = TELE_CREATE_MODE.ALL_KILL, --TELE_CREATE_MODE.RAND_ONE,
		machinery_ogre_list = { 122018, 122019 },
		virtual_map = { virtual_type = VIRTUAL_MAP_CREAT_TYPE.RAND }
	},
	[ 124 ] = 
	{ 
		virtual_init_create = { 123000, 123001, 123002, 123004, 123005, 123006, 123007, 123008, 123009, 123010 },
		tele_index = { 123003 }, 
		du	=
		{
			[ TOWER_STEP.FIRST ] = { 123011 },
			[ TOWER_STEP.SECOND ] = { 123011, 123012, 123014, 123015, 123016 },
			[ TOWER_STEP.THIRD ] = { 123011, 123013, 123014, 123015, 123016 },
			[ TOWER_STEP.FOURTH ] = { 123011, 123014, 123015, 123016 },
			[ TOWER_STEP.FIFTH ] = { 123011, 123015, 123016 },
		},
		
		tele_create_mode = TELE_CREATE_MODE.ALL_KILL,
		virtual_map = { virtual_type = VIRTUAL_MAP_CREAT_TYPE.RAND }
	},
	[ 125 ] = 
	{ 
		virtual_init_create = { 124000, 124001, 124002, 124003, 124005, 124006, 124007, 124008, 124009, 124010, 124017},

		tele_index = { 124004 },
		du	=
		{
			[ TOWER_STEP.FIRST ] = { 124011 },
			[ TOWER_STEP.SECOND ] = { 124011, 124012, 124014, 124015, 124016 },
			[ TOWER_STEP.THIRD ] = { 124011, 124013, 124014, 124015, 124016 },
			[ TOWER_STEP.FOURTH ] = { 124011, 124014, 124015, 124016 },
			[ TOWER_STEP.FIFTH ] = { 124011, 124015, 124016 },
		},
		tele_create_mode = TELE_CREATE_MODE.ALL_KILL, --TELE_CREATE_MODE.RAND_ONE,
		machinery_ogre_list = { 124018, 124019 },
		virtual_map = { virtual_type = VIRTUAL_MAP_CREAT_TYPE.RAND }
	},
	[ 112 ] = 
	{
		tele_index = { 111000 },
		virtual_map = { virtual_type = VIRTUAL_MAP_CREAT_TYPE.RAND }
	},
	[ 113 ] =
	{
		tele_index = { 112000 },
		virtual_map = { virtual_type = VIRTUAL_MAP_CREAT_TYPE.RAND }
	},
	[ 126 ] =
	{
		step_create = 
		{
			[ TOWER_STEP.FIRST ] = { 125001 },
			[ TOWER_STEP.SECOND ] = { 125002 },
			[ TOWER_STEP.THIRD ] = { 125003 },
			[ TOWER_STEP.FOURTH ] = { 125004 },
			[ TOWER_STEP.FIFTH ] = { 125005, 125006 },
		},	
		virtual_map = { virtual_type = VIRTUAL_MAP_CREAT_TYPE.FIXURE, parame1 = 126 },
		tele_create_mode = TELE_CREATE_MODE.ALL_KILL;
--		tele_index = { 125007 };
	}
};
local FLOOR_EVENT=
{
	[ 5 ] = { pass = { 
						{ 
							create_npc_by_map = 
							{ 
								[ 119 ] = 118017,
								[ 120 ] = 119004,
								[ 121 ] = 120020,
								[ 122 ] = 121017,
								[ 123 ] = 122020,
								[ 124 ] = 123017,
								[ 125 ] = 124020
							}
 						},	
					} 
			},
	[ 9 ] = { pass = { 
						{ 
							create_npc_by_map = 
							{ 
								[ 119 ] = 118027,
								[ 120 ] = 119024,
								[ 121 ] = 120030,
								[ 122 ] = 121027,
								[ 123 ] = 122030,
								[ 124 ] = 123027,
								[ 125 ] = 124030
							}
 						},	
					} 
			},
	[ 10 ] = { pass = { 
						{ create_npc_by_map = { [ 126 ] = 125008 } },
						{ title = 284 },
						{ time_stop = { id = 1 } },
						{ time_begin = { id = 3 } }	
					  } 
			 },
	[ 15 ] = { pass = { 
						{ 
							create_npc_by_map = 
							{ 
								[ 119 ] = 118019,
								[ 120 ] = 119019,
								[ 121 ] = 120022,
								[ 122 ] = 121019,
								[ 123 ] = 122022,
								[ 124 ] = 123019,
								[ 125 ] = 124022
							}
 						}
					 } 
			},
	[ 19 ] = { pass = { 
						{ 
							create_npc_by_map = 
							{ 
								[ 119 ] = 118027,
								[ 120 ] = 119024,
								[ 121 ] = 120030,
								[ 122 ] = 121027,
								[ 123 ] = 122030,
								[ 124 ] = 123027,
								[ 125 ] = 124030
							}
 						},	
					} 
			},
	[ 20 ] = { pass = { 
						{ create_npc_by_map = { [ 126 ] = 125010 } },
						{ title = 285 },
						{ time_stop = { id = 1 } },
						{ time_begin = { id = 3 } }
					  } 
			 },
	[ 25 ] = { pass = { 
						{
							create_npc_by_map = 
							{ 
								[ 119 ] = 118021,
								[ 120 ] = 119021,
								[ 121 ] = 120024,
								[ 122 ] = 121021,
								[ 123 ] = 122024,
								[ 124 ] = 123021,
								[ 125 ] = 124024
							}
						}
					  }
			 },
	[ 29 ] = { pass = { 
						{ 
							create_npc_by_map = 
							{ 
								[ 119 ] = 118027,
								[ 120 ] = 119024,
								[ 121 ] = 120030,
								[ 122 ] = 121027,
								[ 123 ] = 122030,
								[ 124 ] = 123027,
								[ 125 ] = 124030
							}
 						},	
					} 
			},
	[ 30 ] = { pass = { 
						{ create_npc_by_map = { [ 126 ] = 125012 } },
						{ title = 286 },
						{ time_stop = { id = 1 } },
						{ time_begin = { id = 3 } }
					  } 
			 },
	[ 35 ] = { pass = { 
						{
							create_npc_by_map = 
							{ 
								[ 119 ] = 118023,
								[ 120 ] = 119023,
								[ 121 ] = 120026,
								[ 122 ] = 121023,
								[ 123 ] = 122026,
								[ 124 ] = 123023,
								[ 125 ] = 124026
							}
						}
					  } 
			 },
	[ 39 ] = { pass = { 
						{ 
							create_npc_by_map = 
							{ 
								[ 119 ] = 118027,
								[ 120 ] = 119024,
								[ 121 ] = 120030,
								[ 122 ] = 121027,
								[ 123 ] = 122030,
								[ 124 ] = 123027,
								[ 125 ] = 124030
							}
 						},	
					} 
			},
	[ 40 ] = { pass = { 
						{ create_npc_by_map = { [ 126 ] = 125014 } },
						{ title = 287 },
						{ time_stop = { id = 1 } },
						{ time_begin = { id = 3 } }
					  } 
			 },
	[ 45 ] = { pass = { 
						{
							create_npc_by_map = 
							{ 
								[ 119 ] = 118025,
								[ 120 ] = 119025,
								[ 121 ] = 120028,
								[ 122 ] = 121025,
								[ 123 ] = 122028,
								[ 124 ] = 123025,
								[ 125 ] = 124028
							}
						}

					  }, 
			 },
	[ 49 ] = { pass = { 
						{ 
							create_npc_by_map = 
							{ 
								[ 119 ] = 118027,
								[ 120 ] = 119024,
								[ 121 ] = 120030,
								[ 122 ] = 121027,
								[ 123 ] = 122030,
								[ 124 ] = 123027,
								[ 125 ] = 124030
							}
 						},	
					} 
			},
	[ 50 ] = { pass = { 
						{ create_npc_by_map = { [ 126 ] = 125016 }  },
						{ title = 288 },
						{ pass_end = 1 }
					  } 
			 },
};

local TOWER_PLAYER_DATA_KEY=
{
	MARK_POINT = 0,
	MARK_POINT_TOTAL = 1,
	MAX_FLOOR		= 2,
	CUR_FLOOR		= 3,
};

local tower_count = 0;
local tower_step = 0;
local virtual_map = 0;
local rand_index = 0;
local ogre_count = 0;
local tower_point = 0;
local max_floor = 0;


local function calculate_point( repetion_id )
	local step_ = GetIntData( repetion_id, TOWER_DATA_KEY.END );

	if step_ ~= 0 then
		return;	
	end		

	Debug( "calculate point" );

	tower_point = GetIntData( repetion_id, TOWER_DATA_KEY.SMALL_POINT );	
	tower_point = tower_point + GetIntData( repetion_id, TOWER_DATA_KEY.DUJUN_POINT );
	tower_point = tower_point + GetIntData( repetion_id, TOWER_DATA_KEY.BOSS_POINT );

	tower_point = tower_point * 1;


	max_floor = GetIntData( repetion_id, TOWER_DATA_KEY.MAX_FLOOR );
	ogre_count = GetIntData( repetion_id, TOWER_DATA_KEY.OGRE_COUNT );

	AddIntData2AllPlayer( repetion_id, TOWER_PLAYER_DATA_KEY.MARK_POINT, tower_point );	
	AddIntData2AllPlayer( repetion_id, TOWER_PLAYER_DATA_KEY.MARK_POINT_TOTAL, tower_point );	
	SetIntData2AllPlayerMax( repetion_id, TOWER_PLAYER_DATA_KEY.MAX_FLOOR, max_floor );
	
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 1, tower_point );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 2, max_floor );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 3, ogre_count );

	SetIntData( repetion_id, TOWER_DATA_KEY.SMALL_POINT, 0 );
	SetIntData( repetion_id, TOWER_DATA_KEY.DUJUN_POINT, 0 );
	SetIntData( repetion_id, TOWER_DATA_KEY.BOSS_POINT, 0 );
	SetIntData( repetion_id, TOWER_DATA_KEY.OGRE_COUNT, 0 );
end

local function tower_end( repetion_id )
	TimeEnd( repetion_id, 1 );
	TimeBegin( repetion_id, 2 );

	calculate_point( repetion_id );

	SetIntData( repetion_id, TOWER_DATA_KEY.END, 1 );
end

local function create_dujun( repetion_id, real_map, virtual_map )
	tower_step = GetIntData( repetion_id, TOWER_DATA_KEY.STEP );	
	if MAP_EVENT[ virtual_map ] ~= nil then
		if MAP_EVENT[ virtual_map ].du ~= nil then
			if TOWER_STEP_EVENT[ tower_step ] ~= nil then
				if TOWER_STEP_EVENT[ tower_step ].dujun_random ~= nil then
					local rand = math.random( 1, 100 );
					if TOWER_STEP_EVENT[ tower_step ].dujun_random < rand then
						return;
					end
				end
			end

			if MAP_EVENT[ virtual_map ].du[ tower_step ] ~= nil then
				rand_index = math.random( 1, table.getn( MAP_EVENT[ virtual_map ].du[ tower_step ] ) );	
				Debug( "repetion " .. repetion_id .. " real_map " .. real_map .. " virtual_map " .. virtual_map
					.. " index " .. MAP_EVENT[ virtual_map ].du[ tower_step ][ rand_index ] );
				CreateNpcRealMap( repetion_id, real_map,  MAP_EVENT[ virtual_map ].du[ tower_step ][ rand_index ] );
			else
				Debug( "tower step " .. tower_step );
			end
		end
	end	
end

local function create_step ( repetion_id )
	tower_step = GetIntData( repetion_id, TOWER_DATA_KEY.STEP );
	if MAP_EVENT[ 126 ] ~= nil then
		if MAP_EVENT[ 126 ].step_create ~= nil then
			if MAP_EVENT[ 126 ].step_create[ tower_step ] ~= nil then
				create_npc_real_map_list( repetion_id, 126, MAP_EVENT[ 126 ].step_create[ tower_step ] );
			end
		end
	end
end

local function reset_map_floor( repetion_id )
	tower_step = GetIntData( repetion_id, TOWER_DATA_KEY.STEP );
	Debug( "tow step " .. tower_step );	
	table.foreachi( REAL_MAP_LIST, function( i, v )
		SetMapIntData( repetion_id, v, TOWER_MAP_DATE_KEY.FLOOR_NO, ( tower_step - 1 ) * 10 + i );
	end);
end


local function real_map_to_virtual( repetion_id )
	table.foreachi( REAL_MAP_LIST, function( i, v )
		virtual_map = v;
		if MAP_EVENT[ v ] ~= nil then
			if MAP_EVENT[ v ].virtual_map ~= nil then
				if MAP_EVENT[ v ].virtual_map.virtual_type ~= nil then
					if MAP_EVENT[ v ].virtual_map.virtual_type == VIRTUAL_MAP_CREAT_TYPE.RAND then
						local virtual_map_index = math.random( 1, table.getn( VIRTUAL_MAP_LIST ) );
	--					virtual_map = VIRTUAL_MAP_LIST[ virtual_map_index ];
						virtual_map = 119;
					elseif MAP_EVENT[ v ].virtual_map.virtual_type == VIRTUAL_MAP_CREAT_TYPE.FIXURE then
						virtual_map = MAP_EVENT[ v ].virtual_map.parame1;
					end
				end
			end
		end
		SetVirtualMap( repetion_id, v, virtual_map );

		reset_map_floor( repetion_id );

		rand_index = math.random( 1, table.getn( BUFF_LIST_67908 ) );
		SetMapIntData( repetion_id, v, TOWER_MAP_DATE_KEY.BUFF_ID_67908, BUFF_LIST_67908[ rand_index ] );
		rand_index = math.random( 1, table.getn( BUFF_LIST_67903 ) );
		SetMapIntData( repetion_id, v, TOWER_MAP_DATE_KEY.BUFF_ID_67903, BUFF_LIST_67903[ rand_index ] );

		create_dujun( repetion_id, v, virtual_map )

		if MAP_EVENT[ virtual_map ] ~= nil then
			if MAP_EVENT[ virtual_map ].virtual_init_create ~= nil then
				create_npc_real_map_list( repetion_id, v, MAP_EVENT[ virtual_map ].virtual_init_create );
			end

			if MAP_EVENT[ virtual_map ].tele_create_mode ~= nil then
				if MAP_EVENT[ virtual_map ].tele_create_mode == TELE_CREATE_MODE.RAND_ONE then
					ogre_count = GetMapIntData( repetion_id, v, TOWER_MAP_DATE_KEY.OGRE_COUNT );
					rand_index = math.random( 1, ogre_count );	
					SetMapIntData( repetion_id, v, TOWER_MAP_DATE_KEY.OGRE_COUNT, rand_index );
					Debug( "repeiton " .. repetion_id .. " virtual map " .. virtual_map .. " real_map " .. v .. 
						" open tele ogre count " .. rand_index );
				end
			end
		end


	end);	
end


local function tower_begin( repetion_id, pararme )

	Debug( "tower begin " .. repetion_id );
	SetIntData( repetion_id, TOWER_DATA_KEY.STEP, TOWER_STEP.FIRST );
	real_map_to_virtual( repetion_id );
	create_step( repetion_id );
end
local function tower_npc_create( repetion_id, parame )
	Notice( "Call TOWER ogre_create repetion " .. repetion_id .. " index " .. parame.index .. " entityid " .. 
		parame.entityid .. " tempid " .. parame.tempid );

	if OGRE_TMP_LIST[ parame.tempid ] ~= nil then
		if OGRE_TMP_LIST[ parame.tempid ].map_ogre_create_add ~= nil then
			add_mapintdata( repetion_id, parame.real_map, TOWER_MAP_DATE_KEY.OGRE_COUNT, 
				OGRE_TMP_LIST[ parame.tempid ].map_ogre_create_add );
			ObserverEntityDeath( repetion_id, parame.entityid );
		end
		if OGRE_TMP_LIST[ parame.tempid ].machinery_spring ~= nil then
			ObserverMachinery( repetion_id, parame.entityid );
		end
		if OGRE_TMP_LIST[ parame.tempid ].map_data_set ~= nil then
			table.foreachi( OGRE_TMP_LIST[ parame.tempid ].map_data_set, function( i, v )
				SetMapIntData( repetion_id, parame.real_map, v.key, v.value );
			end);
		end
		if OGRE_TMP_LIST[ parame.tempid ].create_event ~= nil then
			table.foreachi( OGRE_TMP_LIST[ parame.tempid ].create_event, function( i, v )
				if v.insert_buff_list ~= nil then
					local rank_ndex = math.random( 1, table.getn( v.insert_buff_list ) );
					EntityInsertBuff( repetion_id, parame.entityid, v.insert_buff_list[ rank_ndex ] );	
				end	
				if v.floor_buff_key ~= nil then
					local buff_id = GetMapIntData( repetion_id, parame.real_map, v.floor_buff_key);
					if buff_id ~= 0 then
						EntityInsertBuff( repetion_id, parame.entityid, buff_id );
					end
				end
			end);
		end
	end	

	local level = GetRepeLevel( repetion_id );
	local floor = GetMapIntData( repetion_id, parame.real_map, TOWER_MAP_DATE_KEY.FLOOR_NO );
	
	NpcChangeLevel( repetion_id, parame.entityid, floor, level )

	local buffid = GetMapIntData( repetion_id, parame.real_map, TOWER_MAP_DATE_KEY.FLOOR_OGRE_BUFF );
	if buffid ~= 0 then
		EntityInsertBuff( repetion_id, parame.entityid, buffid );
	end	
end

local function tower_add_point( repetion_id, point_type, value, temp_id, floor )
	if point_type == TOWER_POINT_TYPE.SMALL then
		add_intdata( repetion_id, TOWER_DATA_KEY.SMALL_POINT, 1 );
	elseif point_type == TOWER_POINT_TYPE.DUJUN then
		add_intdata( repetion_id, TOWER_DATA_KEY.DUJUN_POINT, 1 );
	elseif point_type == TOWER_POINT_TYPE.BOSS then
		add_intdata( repetion_id, TOWER_DATA_KEY.BOSS_POINT, value );
	end
	add_intdata( repetion_id, TOWER_DATA_KEY.OGRE_COUNT, 1 );
	max_floor = GetIntData( repetion_id, TOWER_DATA_KEY.MAX_FLOOR );
	if floor > max_floor then
		SetIntData( repetion_id, TOWER_DATA_KEY.MAX_FLOOR, floor )
		Debug( "repetion " .. repetion_id .. " max floor set " .. floor );
--		SetIntData2AllPlayer( repetion_id, TOWER_DATA_KEY.MAX_FLOOR, floor );
	end
end

local function tower_npc_death( repetion_id, parame )
	if OGRE_TMP_LIST[ parame.tempid ] ~= nil then
		if OGRE_TMP_LIST[ parame.tempid ].map_ogre_create_add ~= nil then
			tower_count = add_mapintdata( repetion_id, parame.real_map, TOWER_MAP_DATE_KEY.OGRE_COUNT, 
				OGRE_TMP_LIST[ parame.tempid ].map_ogre_death_dec );
			Notice( "OGRE_DEATH repetion " .. repetion_id .. " real_map " .. parame.real_map .. " ogre_count " .. 
				tower_count );
		end
		if OGRE_TMP_LIST[ parame.tempid ].time_add ~= nil then
			local time = GetTime( repetion_id, 1 );
			ChangeTime( repetion_id, 1, time + OGRE_TMP_LIST[ parame.tempid ].time_add );
		end
		if OGRE_TMP_LIST[ parame.tempid ].map_data_set ~= nil then
			table.foreachi( OGRE_TMP_LIST[ parame.tempid ].map_data_set, function( i, v )
				if v.key == TOWER_MAP_DATE_KEY.FLOOR_OGRE_BUFF then
					NpcRemoveBuffByMapID( repetion_id, 0, v.value, parame.real_map );
				end
				if v.key == TOWER_MAP_DATE_KEY.FLOOR_PLAYER_BUFF then
					PlayerRemoveBuffByMapID( repetion_id, v.value, parame.real_map );
				end

				SetMapIntData( repetion_id, parame.real_map, v.key, 0 );
			end);
		end
		if OGRE_TMP_LIST[ parame.tempid ].death_event ~= nil then
			table.foreachi( OGRE_TMP_LIST[ parame.tempid ].death_event, function( i, v )
				if v.point_add ~= nil then
					local floor = GetMapIntData( repetion_id, parame.real_map, TOWER_MAP_DATE_KEY.FLOOR_NO );
					tower_add_point( repetion_id, v.point_add, v.value, parame.tempid, floor );		
				end
			end);
		end
	end	

	if tower_count == 0 then
		local floor = GetMapIntData( repetion_id, parame.real_map, TOWER_MAP_DATE_KEY.FLOOR_NO );
		if MAP_EVENT[ parame.real_map ] ~= nil then
			if MAP_EVENT[ parame.real_map ].tele_index ~= nil and floor < 50 then
	--			calculate_point( repetion_id );
				create_npc_all( repetion_id, MAP_EVENT[ parame.real_map ].tele_index );
			end
			if floor < 50 then
				calculate_point( repetion_id );
			end
		end

		if floor == 10 then
			AwardFameAll( repetion_id, 7 );
		end

		if floor == 20 then
			AwardFameAll( repetion_id, 8 );
		end

		if floor == 30 then
			AwardFameAll( repetion_id, 9 );
		end

		if floor == 40 then
			AwardFameAll( repetion_id, 10 );
		end

		if floor == 50 then
			AwardFameAll( repetion_id, 11 );
		end

		-- 20层以上要广播
		if floor > 20 then
			local name = GetNameByCharID( repetion_id, parame.killercharid );	
			Debug( "tower " .. floor .. " name " .. name );	
			if name ~= nil and name ~= 0 then
				local msgparame = { integer = { floor }, string = { name } };
				SendEventNotice( repetion_id, -45977, msgparame );
			else
				Debug( "error name " .. name );
			end
		end

		if FLOOR_EVENT[ floor ] ~= nil then
			if FLOOR_EVENT[ floor ].pass ~= nil then
				Debug( "floor pass event " .. table.getn( FLOOR_EVENT[ floor ].pass ) );
				table.foreachi( FLOOR_EVENT[ floor ].pass, function( i, v )
					if v.item ~= nil then
						insert_item_all( repetion_id, v.item, v.count, RepetionInsertItemReason.PASS );	
					elseif v.create_npc_by_map ~= nil then
						if v.create_npc_by_map[ parame.virtual_map ] ~= nil then
							Debug(" create npc " .. v.create_npc_by_map[ parame.virtual_map ] );
							CreateNpcRealMap( repetion_id, parame.real_map, v.create_npc_by_map[ parame.virtual_map ] );
						else
							Debug( "temp " .. parame.temp_id .. " death, virtual map " .. parame.virtual_map .. 
								" pass empty" );
						end
					elseif v.pass_end ~= nil then
						tower_end( repetion_id );
					elseif v.title ~= nil then
						AddTitleAll( repetion_id, v.title, 0 );
					elseif v.time_stop ~= nil then
						TimePause( repetion_id, v.time_stop.id );
					elseif v.time_begin ~= nil then
						TimeBegin( repetion_id, v.time_begin.id );
					else
						Debug( "err 2" );	
					end
				end);
			end
		end
	end	
end

local function tower_machinery_spring( repetion_id, parame )
	if MAP_EVENT[ parame.virtual_map ] ~= nil then
		if MAP_EVENT[ parame.virtual_map ].machinery_ogre_list ~= nil then
			create_npc_real_map_list( repetion_id, parame.real_map, 
				MAP_EVENT[ parame.virtual_map ].machinery_ogre_list  );
			CharacterDie( repetion_id, parame.machineryid );
		end
	end	
end

local function tower_each_other( repetion_id, parame )
	Debug( "repetion " .. repetion_id .." ========EACH OTHER==========" );
	PlayerReliveSys( repetion_id );
	tower_step = add_intdata( repetion_id, TOWER_DATA_KEY.STEP, 1 );
	real_map_to_virtual( repetion_id );
	create_step( repetion_id );		
	AllTeleport( repetion_id, 119, 20, 20, 128 );
	
end

local function tower_next_step( repetion_index, repetion_id, args, loop_index )
	Debug( "repetion " .. repetion_id .." ========NEXT STEP==========" );
	TimeEnd( repetion_id, 3 );
	PlayerReliveSys( repetion_id );
	tower_step = add_intdata( repetion_id, TOWER_DATA_KEY.STEP, 1 );
	Debug( "repeiton " .. repetion_id .. " new step is " .. tower_step );
	real_map_to_virtual( repetion_id );
	create_step( repetion_id );		
	TimeRestart( repetion_id, 1 );
	Debug( "repetion time " .. GetTime( repetion_id, 1 ) );
	AllTeleport( repetion_id, 119, 20, 20, 128 );
end



local function jump_tower_floor(repetion_id,parame)
	Debug( "repetion " .. repetion_id .." ========JUMP==========" );
	local cur_floor = GetMapIntData( repetion_id, parame.playermapid, TOWER_MAP_DATE_KEY.FLOOR_NO );
	local maxfloor = 10*TOWER_STEP.MAX;
	local step = GetIntData( repetion_id, TOWER_DATA_KEY.STEP );
	local vfloor =  parame.jfloor ;
	local vstep = parame.jstep + 1;
	local temp = vfloor - (vstep-1)*10 ;
	if  (cur_floor > vfloor) or (vfloor > maxfloor )   then
		Debug( "jump faid by incorrent floor number" );
		return ;
	elseif step == vstep then
		PlayerReliveSys( repetion_id );
		AllTeleport( repetion_id, REAL_MAP_LIST[temp], 20, 20, 128 );
	else 
		for i = 1,vstep - step do
			TimeEnd( repetion_id, 3 );
			PlayerReliveSys( repetion_id );
			tower_step = add_intdata( repetion_id, TOWER_DATA_KEY.STEP, 1 );
			real_map_to_virtual( repetion_id );
			create_step( repetion_id );
			TimeRestart( repetion_id, 1 );
		end
		AllTeleport( repetion_id, REAL_MAP_LIST[temp] , 20, 20, 128 );
	end
end
	

local function tower_create_player( repetion_id, parame )
	EntityRemoveBuff( repetion_id, parame.entityid, 69044 );
	EntityRemoveBuff( repetion_id, parame.entityid, 69058 );
	
	-- 通知楼层层数
	local floor = GetMapIntData( repetion_id, parame.mapid, TOWER_MAP_DATE_KEY.FLOOR_NO );
	EventNoticeSingle( repetion_id, parame.entityid, RepetionNoticeEventKind.MSG, 1, floor );

	local buffid = GetMapIntData( repetion_id, parame.mapid, TOWER_MAP_DATE_KEY.FLOOR_PLAYER_BUFF );
	if buffid ~= 0 then
		EntityInsertBuff( repetion_id, parame.entityid, buffid );
	end

	if parame.new == 1 then
		insert_item( repetion_id, parame.entityid, 69957, 5, RepetionInsertItemReason.NEW );
		
		SetTodayTimesByGroupID( repetion_id, parame.entityid, 530001, 0 );
		SetTodayTimesByGroupID( repetion_id, parame.entityid, 530002, 0 );
		SetTodayTimesByGroupID( repetion_id, parame.entityid, 530003, 0 );
		SetTodayTimesByGroupID( repetion_id, parame.entityid, 530004, 0 );
		SetTodayTimesByGroupID( repetion_id, parame.entityid, 530005, 0 );
		SetTodayTimesByGroupID( repetion_id, parame.entityid, 530006, 0 );
		SetTodayTimesByGroupID( repetion_id, parame.entityid, 530007, 0 );
		SetTodayTimesByGroupID( repetion_id, parame.entityid, 530008, 0 );
		SetTodayTimesByGroupID( repetion_id, parame.entityid, 530009, 0 );
		SetTodayTimesByGroupID( repetion_id, parame.entityid, 530010, 0 );

	end

	local max_floor = GetIntData( repetion_id, 	TOWER_DATA_KEY.MAX_FLOOR );
	if floor >  ( max_floor + 1 )  then
--		AllTeleport( repetion_id, 119, 20, 20, 128 );
		Debug( "player " .. parame.entityid .. " teleport, repetion " .. repetion_id .. " floor " .. floor .. 
			", max_floor " .. max_floor );
		TeleportByMap( repetion_id, parame.entityid, 119, 20, 20 );
	else
		Debug( "repetion " .. repetion_id .. " player " .. parame.entityid .. " into map " .. parame.mapid .. 
			" floor " .. floor  .. ", max_floor " .. max_floor );
	end
end

local function tower_time_end( repetion_index, repetion_id, args, loop_index )
	tower_end( repetion_id );
end

repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.TOWER,
		playercountmax = 6,		-- 
		mapid = { 119, 120, 121, 122, 123, 124, 125, 126, 112, 113, 128 },
		taskid = 0,			-- into task condition 
		posx = 18,
		posy = 34,
	--	teamin = true,
		pernumber = 1,			-- everday into max number 
                weekpernumber = 1,
		minlevel = 50,
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
			{
				start = { year = 0, month = 0, week = 6, day = 0, hr = 0, minute = 5, sec = 0 },
				finish = { year = 0, month = 0, week = 6, day = 0, hr = 23, minute = 55, sec = 00 }
			},
			{
				start = { year = 0, month = 0, week = 7, day = 0, hr = 0, minute = 5, sec = 0 },
				finish = { year = 0, month = 0, week = 7, day = 0, hr = 23, minute = 55, sec = 00 }
			},
		},
	},

	timer = 
	{ 
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 1000 * 60 * 30,
			loop = 1,
			func = tower_time_end,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = 1000* 60 * 2,
			loop = 1,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 1000* 30,
			loop = 1,
			func = tower_next_step,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 1 }
		},
	},
    event = 
    {
		[ RepetionEventKind.OGRE_DEATH  ] = { func = tower_npc_death },
        [ RepetionEventKind.OGRE_CREATE ] = { func = tower_npc_create },
        [ RepetionEventKind.PLAYER_CREATE ] = { func = tower_create_player },
        [ RepetionEventKind.BEGIN_REPETION ] = { func = tower_begin },
    --    [ RepetionEventKind.TIME_ASSIGNMENT ] = { func = grand_assignment },
		[ RepetionEventKind.BOX_MACHINERY_SPING ] = { func = tower_machinery_spring },
		[ RepetionEventKind.EACH_OTHER ] = { func = tower_each_other },
		[ RepetionEventKind.DEBUG_COMMAND ] = { func = jump_tower_floor },
    },
}


