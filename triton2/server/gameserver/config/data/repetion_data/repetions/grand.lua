local grand_last_time = 30 * 60 * 1000;
local grand_end_time = 2 * 60 * 1000;
local grand_sprint_last_time = 226 * 1000;
local grand_sum_last_time = 450 * 1000;

local GRAND_DATA_KEY=
{
	STEP 				= 0,	
	SEED				= 1,
	SEED_OGRE_INDEX		= 2,
	OGRE_COUNT			= 3,
	SEASON_STEP			= 4,
	OGRE_INDEX_101010	= 5,
	OGRE_INDEX_101011	= 6,
	OGRE_INDEX_101012	= 7,
	OGRE_INDEX_101013	= 8,
	OGRE_INDEX_101014	= 9,
	TREE_COUNT			= 10,
	LAST_CREATE_FURIT	= 11,
	FURIT_COUNT			= 12,
	OGRE_SUM_BOSS		= 13,
	OGRE_WINTER_SPECIAL	= 14;
	OGRE_TREE_1			= 15,
	OGRE_TREE_2			= 16,
	OGRE_TREE_3			= 17,
	OGRE_TREE_4			= 18,
	OGRE_TREE_5			= 19,
	OGRE_TREE_6			= 20,
	SEX					= 21,
};

local GRAND_BUFF=
{
	[ 99 ] = 64394,
	[ 102 ] = 64396,
	[ 103 ] = 64397,
	[ 104 ] = 64398
};

local GRAND_EVENT_KIND=
{
	NOTICE_ALL 				= 1,
	NOTICE_SINGLE 			= 2,
	CREATE_INDEX 			= 3,
	DATA_KEY_ADD			= 4,
	TIME_BEGIN				= 5,
	BUILD_SEED_KEY			= 6,
	NEXT_STEP				= 7,
	CREATE_INDEX_BY_KEY		= 8,
	DATA_KEY_SET			= 9,
	ADD_ENMITY_LIST			= 10,
	USE_SKILL_TREE			= 11,
	DESTROY					= 12,
	ATTACK_ENTITY			= 13,
	SET_KEY_ENTITY			= 14,
	TIME_END				= 15,
	END						= 16,
	DESTROY_TMP				= 17,
	CHARACTER_DIE			= 18,
	USE_SKILL_SELF			= 19,
};

local GRAND_EVENT_CONDITION =
{
	DATA_KEY_VALUE			= 1,
	INDEX_DATA_KEY			= 2,
	RAND					= 3,
	DATA_KEY_VALUE_LARGE	= 4,
	DATA_KEY_VALUE_UNEQUAL	= 5	
};

local GRAND_TIME_ID=
{
	LAST				= 1,			-- 副本总计时
	GRAND_CLOSE			= 2,			-- 副本关闭计时
	SPRING_ID			= 3,			-- 春园记时
	SPRINT_OGRE			= 4,
	SUM_OGRE			= 5,
	AUTUMN_OGRE			= 6,
};

local GRAND_TAKE_COUNT_KEY =
{
	SEED_ADD			= 1,
};

local GRAND_MSG_KEY =
{
	SEED_ADD			= 1,
};

local GRAND_STEP =
{
	UN_BEGIN	= 0,
    SPRING      = 1,
    SUMMER      = 2,
    AUTUMN      = 3,
    WINTER      = 4,
};

local OGRE_TMP_LIST=
{
	[ 62207 ] = 
	{
		create_event = 
		{
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_COUNT, parame2= 1,
				condition = { { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key= GRAND_DATA_KEY.STEP, 
					value = GRAND_STEP.SPRING } }
			}
		},
		death_event = 
		{
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_COUNT, parame2= -1,
                condition = { { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key= GRAND_DATA_KEY.STEP,
                    value = GRAND_STEP.SPRING } }
			 }
		}
	},
	[ 62209 ] =
	{
		create_event = 
		{
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_COUNT, parame2= 1,
				condition = { { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key= GRAND_DATA_KEY.STEP, 
					value = GRAND_STEP.SUMMER } }
			}
		},
		death_event = 
		{
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_COUNT, parame2= -1,
                condition = { { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key= GRAND_DATA_KEY.STEP,
                    value = GRAND_STEP.SUMMER } }
			 }
		}
	},
	[ 62213 ] =
	{
		create_event = 
		{
			{ kind = GRAND_EVENT_KIND.ADD_ENMITY_LIST, list = { 62225, 62224, 62223 } }
		},
		death_event = {	{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101016 },
						condition = { { kind = GRAND_EVENT_CONDITION.RAND, parame1 = 10 } } } }
	},
	[ 62214 ] =
	{
		create_event = 
		{
			{ kind = GRAND_EVENT_KIND.ADD_ENMITY_LIST, list = { 62225, 62224, 62223 } }
		},
		death_event = {	{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101015 },
						condition = { { kind = GRAND_EVENT_CONDITION.RAND, parame1 = 10 } } } }

	},
	[ 62215 ] =
	{
		create_event = 
		{
			{ kind = GRAND_EVENT_KIND.ADD_ENMITY_LIST, list = { 62225, 62224, 62223 } }
		},
		death_event = {	{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101017 },
						condition = { { kind = GRAND_EVENT_CONDITION.RAND, parame1 = 10 } } } }
	},
	[ 62220 ] =
	{
		death_event = 
		{
			{ kind = GRAND_EVENT_KIND.USE_SKILL_TREE, list = { 62225, 62224, 62223 }, parame1 = 64392 }
		}
	},
	[ 62222 ] =
	{
		death_event = 
		{
			{ kind = GRAND_EVENT_KIND.USE_SKILL_TREE, list = { 62225, 62224, 62223 }, parame1 = 64391 }
		}
	},
	[ 62221 ] =
	{
		death_event = 
		{
			{ kind = GRAND_EVENT_KIND.USE_SKILL_TREE, list = { 62225, 62224, 62223 }, parame1 = 64393 }
		}
	},
	[ 62226 ] = 
	{
		create_event = 
		{
			{ kind = GRAND_EVENT_KIND.DESTROY, parame1 = 18 * 1000, other_info = { } },
			{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.LAST_CREATE_FURIT },
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.FURIT_COUNT, parame2 = 1,
				debug_str = "create furite, 62226" }
		},
		death_event = 
		{
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.FURIT_COUNT, parame2 = -1,
				debug_str = "furite destroy, 62226" }
		}
	},
	[ 62227 ] =
	{
		create_event = 
		{
			{ kind = GRAND_EVENT_KIND.DESTROY, parame1 = 24 * 1000, other_info = { } },
			{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.LAST_CREATE_FURIT },
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.FURIT_COUNT, parame2 = 1,
				debug_str = "create furite, 62227" }	
		},
		death_event = 
		{
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.FURIT_COUNT, parame2 = -1,
				debug_str = "furite destroy, 62227" }
		}
	},
	[ 62228 ] =
	{
		create_event = 
		{
			{ kind = GRAND_EVENT_KIND.DESTROY, parame1 = 35 * 1000, other_info = { } },
			{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.LAST_CREATE_FURIT },
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.FURIT_COUNT, parame2 = 1,
				debug_str = "create furite, 62228" }
		},
		death_event =
		{
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.FURIT_COUNT, parame2 = -1,
				debug_str = "destroy furite, 62228" }
		}
	},
	[ 62229 ] =
	{
		create_event = 
		{
			
			{ kind = GRAND_EVENT_KIND.ATTACK_ENTITY, parame1 = GRAND_DATA_KEY.LAST_CREATE_FURIT,
				condition = { { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.SEASON_STEP, 
					value = 1 }, false_debug_str = "autu step is not 1"
					},
				debug_str = "auto ogre add furite"
			}
		}
	},
	[ 62232 ] =
	{
		create_event =
		{
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_WINTER_SPECIAL, parame2 = 1 },
		},
		death_event =
		{
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_WINTER_SPECIAL, parame2 = -1 },
		},
	},
	[ 62235 ] =
	{
		create_event =
		{
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_WINTER_SPECIAL, parame2 = 1 },
		},
		death_event =
		{
			{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_WINTER_SPECIAL, parame2 = -1 },
		},
	}
}

local OGRE_LIST=
{
	[ 98010 ] = {	create_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.NOTICE_ALL, key = RepetionNoticeEventKind.MSG, parame1 = 1  },
--						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.SEED, parame2 = 4 }, 
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = 1 },
						{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_3 }
					} ,
                    death_event =
                    {
                        { kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = -1 }
                    }
				},

	[ 98012 ] = { 	create_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.NOTICE_ALL, key = RepetionNoticeEventKind.MSG, parame1 = 1  },
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.SEED, parame2 = 1 }, 
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = 1 },
						{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_1 }
--						{ kind = GRAND_EVENT_KIND.TIME_BEGIN, parame1 = GRAND_TIME_ID.SPRING_ID }
					},
					death_event =
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = -1 }
					}
				},
	[ 98013 ] = { 	death_event = { { kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98012 } } } },
	[ 98014 ] = {	create_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.NOTICE_ALL, key = RepetionNoticeEventKind.MSG, parame1 = 1  },
--						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.SEED, parame2 = 8 }, 
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = 1 },
						{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_4 }
					},
					death_event =
                    {
                        { kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = -1 }
                    }	
				},
	[ 98015 ] = {	create_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.NOTICE_ALL, key = RepetionNoticeEventKind.MSG, parame1 = 1  },
--						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.SEED, parame2 = 16 }, 
						{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_5 },
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = 1 }
					},
					death_event=
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = -1 }
					}
				},
	[ 98016 ] = {	create_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.NOTICE_ALL, key = RepetionNoticeEventKind.MSG, parame1 = 1  },
--						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.SEED, parame2 = 32 }, 
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = 1 },
						{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_6 },
					}, 
                    death_event =
                    {
                        { kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = -1 }
                    }
				},
	[ 98020 ] = {   
					create_event = { { kind = GRAND_EVENT_KIND.DESTROY, parame1= 40 * 1000, other_info = { } } },
					death_event = { { kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98021 } } } 
				},
		

	[ 98021 ] = {	create_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.NOTICE_ALL, key = RepetionNoticeEventKind.MSG, parame1 = 1  },
					--	{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.SEED, parame2 = 2 }, 
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = 1 },
						{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_2 }
					}, 
                    death_event =
                    {
                        { kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = -1 }
                    }
				},
	[ 98025 ] = {   
					create_event = { { kind = GRAND_EVENT_KIND.DESTROY, parame1= 40 * 1000, other_info = { } } },
					death_event = { { kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98010 } } } 
				},	
	[ 98032 ] = {	 
					create_event = { { kind = GRAND_EVENT_KIND.DESTROY, parame1= 40 * 1000, other_info = { } } },
					death_event = { { kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98014 } } } 
				},
	[ 98042 ] = {		
					create_event = { { kind = GRAND_EVENT_KIND.DESTROY, parame1= 40 * 1000, other_info = { } } },
					death_event = { { kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98015 } } } 
				},
	[ 98044 ] = {   
					create_event = { { kind = GRAND_EVENT_KIND.DESTROY, parame1= 40 * 1000, other_info = { } } },
					death_event = { { kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98016 } } } 
				},
	[ 101005 ] = { 	
					create_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_1 },
						{ kind = GRAND_EVENT_KIND.USE_SKILL_SELF, parame1 = 65208 },
					},
					death_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = -1 },	
						{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.OGRE_TREE_1, parame2 = 0,
							debug_str = " tree 101005 die" }
					}

				 },
	[ 101006 ] = {
					create_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_2 },
						{ kind = GRAND_EVENT_KIND.USE_SKILL_SELF, parame1 = 65208 }
					},
					death_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = -1 }	,
						{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.OGRE_TREE_2, parame2 = 0,
							debug_str = " tree 101006 die" }	
					}
				 },	
	[ 101004 ] = {
					create_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_3 },	
						{ kind = GRAND_EVENT_KIND.USE_SKILL_SELF, parame1 = 65207 }
					},
                    death_event = 
                    {
                        { kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = -1 },
						{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.OGRE_TREE_3, parame2 = 0,
							debug_str = " tree 101004 die" }	
                    }
                 },	
	[ 101002 ] = {
					create_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_4 },
						{ kind = GRAND_EVENT_KIND.USE_SKILL_SELF, parame1 = 65207 }
					},
                    death_event =
                    {
                        { kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = -1 },
						{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.OGRE_TREE_4, parame2 = 0,
							debug_str = " tree 101002 die" }	
                    }
                 },
	[ 101000 ] = {
					create_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_5 },
						{ kind = GRAND_EVENT_KIND.USE_SKILL_SELF, parame1 = 65206 }
					},
                    death_event =
                    {
                        { kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = -1 },
						{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.OGRE_TREE_5, parame2 = 0,
							debug_str =" tree 101000 die" }	
                    }
                 },
	[ 101003 ] = {
					create_event = 
					{ 
						{ kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_6 },
						{ kind = GRAND_EVENT_KIND.USE_SKILL_SELF, parame1 = 65206 }
					 },
                    death_event =
                    {
                        { kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.TREE_COUNT, parame2 = -1 },
						{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.OGRE_TREE_6, parame2 = 0,
							debug_str =" tree 101003 die" }	
                    }
                 },
	[ 101010 ] = {
					create_event =
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_INDEX_101010, 
							parame2 = 1 }
					},
					death_event = 
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_INDEX_101010, 
							parame2 = -1 }
					}
				 },
	[ 101011 ] = {
					create_event =
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_INDEX_101011, 
							parame2 = 1 }
					},
					death_event = 
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_INDEX_101011, 
							parame2 = -1 }
					}
				 },

	[ 101012 ] = {
					create_event =
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_SUM_BOSS, parame2 = 1 },
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_INDEX_101012,
							 parame2 = 1 }
					},
					death_event = 
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_SUM_BOSS, parame2 = -1 },
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_INDEX_101012,
							parame2 = -1 }
					}
				 },
	[ 101013 ] = {
					create_event =
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_SUM_BOSS, parame2 = 1 },
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_INDEX_101013,
							parame2 = 1 }
					},
					death_event = 
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_SUM_BOSS, parame2 = -1 },
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_INDEX_101013,
							parame2 = -1 }
					}
				 },
	[ 101014 ] = {
					create_event =
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_SUM_BOSS, parame2 = 1 },
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_INDEX_101014, 
							parame2 = 1 }
					},
					death_event = 
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_SUM_BOSS, parame2 = -1 },
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_INDEX_101014, 
							parame2 = -1 }
					}
				 },
	[ 102001 ] = {
					create_event = 
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_COUNT, parame2 = 1 },
					},
					death_event = 
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_COUNT, parame2 = -1 },
					}
				 },
	[ 102018 ] = {
					create_event =
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_COUNT, parame2 = 1 }
					},
					death_event = 
					{
						{ kind = GRAND_EVENT_KIND.DATA_KEY_ADD, parame1 = GRAND_DATA_KEY.OGRE_COUNT, parame2 = -1 }
					}
				 },
	[ 103000 ] = {
					death_event = 
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 103004, 103001, 103003 } }
					}
				},
	[ 103004 ] = {
					death_event =
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 103002, 103006, 103007, 103008 } }
					}
				 },
	[ 103002 ] = {
				 	death_event = 
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 103029, 103010, 103017, 103020, 103024 },
							condition = 
							{		
								{
									kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE_LARGE, key = GRAND_DATA_KEY.TREE_COUNT,
									value = 2	
								}
							}		
						 },
						{ 
							kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 103010, 103017, 103020, 103024 },
							condition = 
							{
								{
									kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT, 
									value = 1
								}
							}
						},
						{ 
							kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 103010, 103017, 103020, 103024 },
							condition = 
							{
								{
									kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT, 
									value = 2
								}
							}
						},


						{ 	kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 2,
								debug_str="if tree count is 1 or 2"
						 },
						{ 	kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 1,
							condition =
							{
								{
									kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE_LARGE, key = GRAND_DATA_KEY.TREE_COUNT,
									value = 2, 
								}
							},
							debug_str = "tree count big 2"
						},
					},
				 },
	[ 103029 ] = {
					death_event = 
					{
						{ 
							kind = GRAND_EVENT_KIND.CREATE_INDEX, 
							list = { 103030, 103012, 103015, 103023, 103021, 103026},
							condition =
                            {
								{
                                	kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE_LARGE, key = GRAND_DATA_KEY.TREE_COUNT,
                                	value = 4
								}
                            }
						},
						{
							kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 103012, 103015, 103023, 103021, 103026 },
							condition =
							{
								{
									kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT,
									value = 3
								}
							}
						},
						{
							kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 103012, 103015, 103023, 103021, 103026 },
							condition =
							{
								{
									kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT,
									value = 4
								}
							}
						},
                        {   kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 2,
								debug_str = "if tree count is 3 or 4"
					 	},
                        {   kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 1,
                            condition =
                            {
                                {
									kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE_LARGE, key = GRAND_DATA_KEY.TREE_COUNT,
                                	value = 4, 
								}
                            } ,
 							debug_str = "tree count big 4"
                        }, 
					}
				 },	
	[ 103030 ] = {
					death_event =
					{
						{
							kind = GRAND_EVENT_KIND.CREATE_INDEX,
							list = { 103044, 103018, 103011, 103016, 103025, 103019, 103027 },
                            condition =
                            {
								{
                           		     kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT,
                                	value = 6
								}
                            }
						},
                        {
                            kind = GRAND_EVENT_KIND.CREATE_INDEX, 
							list = { 103018, 103011, 103016, 103025, 103019, 103027 },
                            condition =
                            {
								{
                          	    	kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT,
                            	    value = 5
								}
                            }
                        },
                        {   kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 2,
								debug_str = "if tree count is 5"
						},
                        {   kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 1,
                            condition =
                            {
                                {
									kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE_LARGE, key = GRAND_DATA_KEY.TREE_COUNT,
                                	value = 5, 
								}
                            },  
							debug_str="tree count big 5"
                        }, 
					}	
				 },
	[ 103044 ] = {
					death_event =
					{
						{
							kind = GRAND_EVENT_KIND.CREATE_INDEX,
							list = { 103005, 103013, 103014, 103040, 103041, 103042, 103009, 103022, 103028 }
						},
                        {   kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 2,
								debug_str="if tree count is 6"
						},
					}	
				 },
	[ 103047 ] = {
					create_event =
					{
						{	kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_1 }
					}
				 },
	[ 103048 ] = {
					create_event =
					{
						{	kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_2 }
					}
				 },
	[ 103050 ] = {
					create_event =
					{
						{	kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_3 }
					}
				 },
	[ 103049 ] = {
					create_event =
					{
						{	kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_4 }
					}
				 },
	[ 103053 ] = {
					create_event =
					{
						{	kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_5 }
					}
				 },
	[ 103051 ] = {
					create_event =
					{
						{	kind = GRAND_EVENT_KIND.SET_KEY_ENTITY, key = GRAND_DATA_KEY.OGRE_TREE_6 }
					}
				 },
};

local GRAND_STEP_EVENT=
{
	[ GRAND_STEP.SPRING ] = 
	{
		fire_event =
		{
			{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98013, 98007, 98009 } },
			{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 0 }
		},

		npc_death = 
		{
			{ 
				kind = GRAND_EVENT_KIND.TIME_BEGIN, parame1 = GRAND_TIME_ID.SPRING_ID,
			  	condition = 
				{
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.SEASON_STEP, value = 0  },
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.OGRE_COUNT, value = 0 }
				}
			},
			{ 
				kind = GRAND_EVENT_KIND.TIME_BEGIN, parame1 = GRAND_TIME_ID.SPRINT_OGRE,
			  	condition = 
				{
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.SEASON_STEP, value = 0  },
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.OGRE_COUNT, value = 0 }
				}
			},
			{ 
				kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 1,
			  	condition = 
				{
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.SEASON_STEP, value = 0  },
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.OGRE_COUNT, value = 0 }
				}
			}
		},
		next_step = GRAND_STEP.SUMMER
	},
	-- 夏园
	[ GRAND_STEP.SUMMER ] =
	{
		fire_event = { 	
						{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.OGRE_COUNT, parame = 0 },
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101035, 101036, 101037, 101038 } },
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98017 } },
						{ 
							kind = GRAND_EVENT_KIND.CREATE_INDEX_BY_KEY, 
							list = { 101005, 101006, 101004, 101002, 101000, 101003	}, 
						--	data_key = GRAND_DATA_KEY.SEED 
						}
					 },
		npc_death = 
		{
			{ 
				kind = GRAND_EVENT_KIND.TIME_BEGIN, parame1 = GRAND_TIME_ID.SUM_OGRE,
			  	condition = 
				{
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.SEASON_STEP, value = 0  },
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.OGRE_COUNT, value = 0 }
				}
			},
			{ 
				kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 1,
			  	condition = 
				{
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.SEASON_STEP, value = 0  },
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.OGRE_COUNT, value = 0 }
				}
			},
			{
				kind = GRAND_EVENT_KIND.NEXT_STEP, parame1 = GRAND_STEP.AUTUMN,
				condition =
				{
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.SEASON_STEP, value = 2 },
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.OGRE_SUM_BOSS, value = 0 },
				}
			},
			{
				kind = GRAND_EVENT_KIND.DESTROY_TMP, list = { 62209, 62213, 62215, 62220, 62222, 62221, 62218,
					62215, 62217, 62219, 62213, 62216, 62214 },
				condition = 
				{
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT, value = 0 }
				},
				debug_str = "destroy all sum ogre"
			},
			{
				kind = GRAND_EVENT_KIND.END, parame1 = RepetionEnd.UN_COMPLATE,
				condition = 
				{
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT, value = 0 }
				},
				debug_str = "=======GRAND UN COMPLATE, ALL TREE DIE=========",
			}	
		},
		next_step = GRAND_STEP.AUTUMN
	},
	[ GRAND_STEP.AUTUMN ] = 
	{
		fire_event = { 	
						{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.OGRE_COUNT, parame = 0 },
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102001 } },
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101040 } },
						{ 
							kind = GRAND_EVENT_KIND.CREATE_INDEX_BY_KEY, 
							list = { 102000, 102005, 102002, 102004, 102006, 102003	}, 
--							data_key = GRAND_DATA_KEY.SEED 
						},
						{
							kind = GRAND_EVENT_KIND.DESTROY_TMP, 
							list = { 62209, 62213, 62215, 62220, 62222, 62221, 62218,
								62215, 62217, 62219, 62213, 62216, 62214 },
							debug_str = "begin autumn destroy all sum ogre"
						},
					 },
		npc_death = 
		{
			{ 
				kind = GRAND_EVENT_KIND.TIME_BEGIN, parame1 = GRAND_TIME_ID.AUTUMN_OGRE,
			  	condition = 
				{
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.SEASON_STEP, value = 0  },
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.OGRE_COUNT, value = 0 }
				}
			},
			{ 
				kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 1,
			  	condition = 
				{
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.SEASON_STEP, value = 0  },
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.OGRE_COUNT, value = 0 }
				}
			},
			{
				kind = GRAND_EVENT_KIND.NEXT_STEP, parame1 = GRAND_STEP.WINTER,
				condition = 
				{
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.SEASON_STEP, value = 2 },
					{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.FURIT_COUNT, value = 0 }
				}
			}
		}

	},
	[ GRAND_STEP.WINTER ] =
	{
		fire_event = { 	
						{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.OGRE_COUNT, parame2 = 0 },
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 103000 } },
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102077 } },
						{ 
							kind = GRAND_EVENT_KIND.CREATE_INDEX_BY_KEY, 
							list = { 103047, 103048, 103050, 103049, 103053, 103051	}, 
--							data_key = GRAND_DATA_KEY.SEED 
						}
					 },
		npc_death =
		{
			{ 
                kind = GRAND_EVENT_KIND.END, parame1 = RepetionEnd.COMPLATE,
                condition =
                {
                    { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.SEASON_STEP, value = 2  },
                    { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.OGRE_WINTER_SPECIAL, value = 0 }
                },
				debug_str = "==============Grand Complate!!!!!!!=============="
			},
		}
	}

};

local step = 0;
local count = 0;
local value_ = 0;
local bool_type = true;
local obser_death = 0;
local change_space = 0;
local random = 0;
local entity = 0;

local grand_change_step;

local grand_tree_key_list = { GRAND_DATA_KEY.OGRE_TREE_1,  GRAND_DATA_KEY.OGRE_TREE_2,  GRAND_DATA_KEY.OGRE_TREE_3,
				 GRAND_DATA_KEY.OGRE_TREE_4,  GRAND_DATA_KEY.OGRE_TREE_5 ,  GRAND_DATA_KEY.OGRE_TREE_6 };

local function grand_end( repetion_id, complate )
	Debug( "Grand " .. repetion_id .. " End, Compalte " .. complate );
	
	TimeEnd( repetion_id, GRAND_TIME_ID.LAST );
	TimeEnd( repetion_id, GRAND_TIME_ID.SPRING_ID );
	TimeEnd( repetion_id, GRAND_TIME_ID.SPRINT_OGRE );
	TimeEnd( repetion_id, GRAND_TIME_ID.SUM_OGRE );
	TimeEnd( repetion_id, GRAND_TIME_ID.AUTUMN_OGRE );

	TimeBegin( repetion_id, GRAND_TIME_ID.GRAND_CLOSE );


	SetIntData( repetion_id, GRAND_DATA_KEY.STEP, GRAND_STEP.UN_BEGIN );

	if complate == RepetionEnd.COMPLATE then
		count = GetIntData( repetion_id, GRAND_DATA_KEY.TREE_COUNT );	
		EventNoticeAll( repetion_id, RepetionNoticeEventKind.MSG, 3, count );

		entity = GetIntData( repetion_id, GRAND_DATA_KEY.OGRE_TREE_1 );
		if entity ~= 0 then
			Debug( "Grand " .. repetion_id .. " entity " .. entity .. " die" );
			CharacterDie( repetion_id, entity, 0 );
			CreateNpc( repetion_id, 103052 );
		end	
		entity = GetIntData( repetion_id, GRAND_DATA_KEY.OGRE_TREE_2 );
		if entity ~= 0 then
			Debug( "Grand " .. repetion_id .. " entity " .. entity .. " die" );
			CharacterDie( repetion_id, entity, 0 );
			CreateNpc( repetion_id, 103054 );
		end	
		entity = GetIntData( repetion_id, GRAND_DATA_KEY.OGRE_TREE_3 );
		if entity ~= 0 then
			Debug( "Grand " .. repetion_id .. " entity " .. entity .. " die" );
			CharacterDie( repetion_id, entity, 0 );
			CreateNpc( repetion_id, 103055 );
		end	
		entity = GetIntData( repetion_id, GRAND_DATA_KEY.OGRE_TREE_4 );
		if entity ~= 0 then
			Debug( "Grand " .. repetion_id .. " entity " .. entity .. " die" );
			CharacterDie( repetion_id, entity, 0 );
			CreateNpc( repetion_id, 103056 );
		end	
		entity = GetIntData( repetion_id, GRAND_DATA_KEY.OGRE_TREE_5 );
		if entity ~= 0 then
			Debug( "Grand " .. repetion_id .. " entity " .. entity .. " die" );
			CharacterDie( repetion_id, entity, 0 );
			CreateNpc( repetion_id, 103057 );
		end	
		entity = GetIntData( repetion_id, GRAND_DATA_KEY.OGRE_TREE_6 );
		if entity ~= 0 then
			Debug( "Grand " .. repetion_id .. " entity " .. entity .. " die" );
			CharacterDie( repetion_id, entity, 0 );
			CreateNpc( repetion_id, 103058 );
		end	
		
		count = GetIntData( repetion_id, GRAND_DATA_KEY.SEX );	
		if count == 5 then
			insert_item_all( repetion_id, 65157, 12, RepetionInsertItemReason.WIN );
		else
			insert_item_all( repetion_id, 65157, 5, RepetionInsertItemReason.WIN );
		end

		AwardFameAll( repetion_id, 5 );
	end

end

local function grand_event_condition( repetion_id, index, tmp_id, entity_id, condition )
	Debug( "Grand " .. repetion_id .. " fire condition " .. condition.kind );
	if condition.kind == GRAND_EVENT_CONDITION.DATA_KEY_VALUE then
		value_ = GetIntData( repetion_id, condition.key );
		if value_ == condition.value then
			return true;	
		end
	elseif condition.kind == GRAND_EVENT_CONDITION.INDEX_DATA_KEY then
		value_ = GetIntData( repetion_id, condition.key );
		if index == value_ then
			return true;
		end
	elseif condition.kind == GRAND_EVENT_CONDITION.RAND then
		random = math.random( 1, 100 );	
		if random <= condition.parame1 then
			return true;
		end
	elseif condition.kind == GRAND_EVENT_CONDITION.DATA_KEY_VALUE_UNEQUAL then
		value_ = GetIntData( repetion_id, condition.key );
		if value_ ~= condition.value then
			return true;
		end
	elseif condition.kind == GRAND_EVENT_CONDITION.DATA_KEY_VALUE_LARGE then
		value_ = GetIntData( repetion_id, condition.key );
		if value_ > condition.value then
			return true;
		end
	else
		Error( "Grand " .. repetion_id .. " UnKnow Condition type " .. condition.kind );
	end
	
	return false;
end

local function grand_event( repetion_id, index, tmp_id, entity_id, event )
	Debug( "Grand " .. repetion_id .. " index  " .. index .. " temp " .. tmp_id .. " entity_id " .. entity_id ..
		" fire event " );
	Debug( "Event Type " .. event.kind );
	if event.condition ~= nil then
		table.foreachi( event.condition, function( i, v )
			bool_type = grand_event_condition( repetion_id, index, tmp_id, entity_id, v )
			if bool_type == false then
				if v.false_debug_str ~= nil then
					Debug( "CONDITION: " .. v.false_debug_str );
				end
				return 1;
			end
		end);

		if bool_type == false then
			return;
		end
	end

	if event.debug_str ~= nil then
		Debug( "EVENT: " .. event.debug_str );
	end

	if event.kind == GRAND_EVENT_KIND.NOTICE_ALL then
		EventNoticeAll( repetion_id, event.key, event.parame1, event.parame2 );
	elseif event.kind == GRAND_EVENT_KIND.DATA_KEY_ADD then
		add_intdata( repetion_id, event.parame1, event.parame2 );
	elseif event.kind == GRAND_EVENT_KIND.CREATE_INDEX then
		create_npc_all( repetion_id, event.list );
	elseif event.kind == GRAND_EVENT_KIND.DATA_KEY_SET then
		SetIntData( repetion_id, event.parame1, event.parame2 );
	elseif event.kind == GRAND_EVENT_KIND.TIME_BEGIN then
		TimeBegin( repetion_id, event.parame1 );
	elseif event.kind == GRAND_EVENT_KIND.BUILD_SEED_KEY then
		if event.index < 0 then
			event.index = 0 
		end
		if event.index + 1 > table.getn( event.list ) then
			event.index = 1;
		else
			event.index = event.index + 1;
		end
		Debug( "Grand " .. repetion_id .. " Seed Ogre " .. event.list[ event.index ] );
		SetIntData( repetion_id, GRAND_DATA_KEY.SEED_OGRE_INDEX, event.list[ event.index ] )
	elseif event.kind == GRAND_EVENT_KIND.CREATE_INDEX_BY_KEY then
--		count = GetIntData( repetion_id, event.data_key );
--		CreateNpcByBite( repetion_id, count, event.list );
--		entity = GetIntData( repetion_id, GRAND_DATA_KEY.OGRE_TREE_1 );
		table.foreachi( grand_tree_key_list, function( i, v )
			entity = GetIntData( repetion_id, v );
			if entity ~= 0 then
				CreateNpc( repetion_id, event.list[ i ] );	
			else
				Debug( "Grand " .. repetion_id .. " create tree index " .. i .. " is not alive" );
			end
		end);
		
	elseif event.kind == GRAND_EVENT_KIND.ADD_ENMITY_LIST then
		table.foreachi( event.list, function( i, v )
			AddEnmity2Template( repetion_id, entity_id, v, i );
		end);
	elseif event.kind == GRAND_EVENT_KIND.USE_SKILL_TREE then
--		table.foreachi( event.list, function( i, v )
--			UseSkill2Template( repetion_id, entity_id, v, event.parame1 );
--		end);
        table.foreachi( grand_tree_key_list, function( i, v )
            entity = GetIntData( repetion_id, v );
            if entity ~= 0 then
                UseSkill( entity, entity, event.parame1 );
            else
                Debug( "Grand " .. repetion_id .. " use skill tree index " .. i .. " is not alive" );
            end
        end);
	elseif event.kind == GRAND_EVENT_KIND.USE_SKILL_SELF then
		UseSkill( entity_id, entity_id, event.parame1 );
	elseif event.kind == GRAND_EVENT_KIND.DESTROY then
		ObserverAssignment( repetion_id, entity_id );
		SetAssignment( repetion_id, entity_id, event.parame1, event.other_info );
	elseif event.kind == GRAND_EVENT_KIND.ATTACK_ENTITY then
		entity = GetIntData( repetion_id, event.parame1 );
		if entity ~= 0 then
			AddEnmity( entity_id, entity, 1 );
			Debug( "Grand " .. repetion_id .. " " .. entity .. " Will Attack " .. entity_id );
		else
			Debug( "Grand " .. repetion_id .. " last attack entity is empty" );
		end
	elseif event.kind == GRAND_EVENT_KIND.TIME_END then
		TimeEnd( repetion_id, event.parame1 );
	elseif event.kind == GRAND_EVENT_KIND.NEXT_STEP then
		grand_change_step( repetion_id, event.parame1 );	
	elseif event.kind == GRAND_EVENT_KIND.SET_KEY_ENTITY then
		SetIntData( repetion_id, event.key, entity_id );
	elseif event.kind == GRAND_EVENT_KIND.END then
		grand_end( repetion_id, event.parame1 );
	elseif event.kind == GRAND_EVENT_KIND.DESTROY_TMP then
		table.foreachi( event.list, function( i, v )
			Debug( "Grand " .. repetion_id .. " destroy ogre " .. v );
			DestroyAliveTempOgre( repetion_id, v, 0 );
		end);
	elseif event.kind == GRAND_EVENT_KIND.CHARACTER_DIE then
		entity = GetIntData( repetion_id, event.key );	
		CharacterDie( repetion_id, entity_id, 0 );
	else
		Error( "Grand " .. repetion_id .. " Unknow event kind " .. event.kind );
	end
end

local function grand_fire_event_loop( repetion_id, index, tmp_id, entity_id, event_list )
	Debug( "Grand " .. repetion_id .. " index " .. index .. " tmp_id " .. tmp_id .. "entity_id " .. entity_id .. 
		" Event Begin " )
	table.foreachi( event_list, function( i, v )
		grand_event( repetion_id, index, tmp_id, entity_id, v )
	end);

	Debug( "Grand " .. repetion_id .. " index " .. index .. " tmp_id " .. tmp_id .. "entity_id " .. entity_id ..
		" Event End" );
end

function grand_change_step( repetion_id, next_step )
	if next_step == 0 then
		return;
	end

	SetIntData( repetion_id, GRAND_DATA_KEY.STEP, next_step );
	SetIntData( repetion_id, GRAND_DATA_KEY.SEASON_STEP, 0 );
	Debug( "Grand " .. repetion_id .. " Change Step " .. next_step );
	if GRAND_STEP_EVENT[ next_step ] ~= nil then
		if GRAND_STEP_EVENT[ next_step ].fire_event ~= nil then
			Debug( "Grand " .. repetion_id .. " Begin Event Loop From STEP_BEGIN, step " .. next_step );
			grand_fire_event_loop( repetion_id, 0, 0, 0, GRAND_STEP_EVENT[ next_step ].fire_event );
			Debug( "Grand " .. repetion_id .. " End Event Loop From STEP_BEGIN, step " .. next_step );
		end	
	end
end

local function create_player( repetion_id, parame )
	if parame.new == 1 then
		Debug( "Grand " .. repetion_id .. " Player " .. parame.charid .. " : " .. parame.entityid .. 
		" Join Sex " .. parame.sex );
		count = add_intdata( repetion_id, GRAND_DATA_KEY.SEX, parame.sex );
	else
		count = GetIntData( repetion_id, GRAND_DATA_KEY.SEX );
	end	

	if count == 5 then
		if GRAND_BUFF[ parame.mapid ] ~= nil then
			Debug( "Grand " .. repetion_id .. " map_id " .. parame.mapid );
			UseSkill2AllPlayer( repetion_id, parame.entityid, GRAND_BUFF[ parame.mapid ] );
			UseSkill( parame.entityid, parame.entityid, GRAND_BUFF[ parame.mapid ] );
		end
	else
		Debug( "Grand " .. repetion_id .. " not a man and a woman" );
	end	

	if parame.mapid == 102 then
		UseSkill( parame.entityid, parame.entityid, 64395 );
	end	
	if parame.mapid == 103 then
		UseSkill( parame.entityid, parame.entityid, 65165 );
	end
end

local function create_npc( repetion_id, parame )
	Debug( "Call Grand Create Npc " .. parame.index .. " entity_id " .. parame.entityid .. " tempid" .. parame.tempid )	
	obser_death = 0;

	step = GetIntData( repetion_id, GRAND_DATA_KEY.STEP );
	if step == GRAND_STEP.UN_BEGIN then
		Debug( "Grand " .. repetion_id .. " Is Not Begin" );	
		return;	
	end

	if OGRE_LIST[ parame.index ] ~= nil then
		if OGRE_LIST[ parame.index ].create_event ~= nil then
			Debug( "Grand " .. repetion_id .. " Begin Event Loop From NPC_CREATE, index " .. parame.index );
			grand_fire_event_loop( repetion_id, parame.index, parame.tempid, parame.entityid, 
				OGRE_LIST[ parame.index ].create_event );
			Debug( "Grand " .. repetion_id .. " End Event Loop From NPC_CREATE, index " .. parame.index );
		end

	
		if OGRE_LIST[ parame.index ].ogre_base_event ~= nil or OGRE_LIST[ parame.index ].death_event ~= nil then
			obser_death =  1;
		end
	end	

	if OGRE_TMP_LIST[ parame.tempid ] ~= nil then
		if OGRE_TMP_LIST[ parame.tempid ].create_event ~= nil then
			Debug( "Grand " .. repetion_id .. " Begin Event Loop From NPC_CREATE, temp " .. parame.tempid );
			grand_fire_event_loop( repetion_id, parame.index, parame.tempid, parame.entityid,
				OGRE_TMP_LIST[ parame.tempid ].create_event );
			Debug( "Grand " .. repetion_id .. " End Event Loop From NPC_CREATE, temp " .. parame.tempid );
		end
		if OGRE_TMP_LIST[ parame.tempid ].death_event ~= nil then
			obser_death = 1;
		end
	end

	if obser_death ~= 0 then
		ObserverEntityDeath( repetion_id, parame.entityid );
	end
end

local function npc_death( repetion_id, parame )
	Debug( "Call Grand ogre_death " .. parame.index .. " entityid " .. parame.killedid );

	if OGRE_LIST[ parame.index ] ~= nil then
		if OGRE_LIST[ parame.index ].death_event ~= nil then
			Debug( "Grand " .. repetion_id .. " Begin Event Loop From NPC_DEATH, index " .. parame.index );
			grand_fire_event_loop( repetion_id, parame.index, parame.tempid, parame.killedid, 
				OGRE_LIST[ parame.index ].death_event );
			Debug( "Grand " .. repetion_id .. " End Event Loop From NPC_DEATH, index " .. parame.index );
		end
	end	

	if OGRE_TMP_LIST[ parame.tempid ] ~= nil then
		if OGRE_TMP_LIST[ parame.tempid ].death_event ~= nil then
			Debug( "Grand " .. repetion_id .. " Begin Event Loop From NPC_DEATH, temp " .. parame.tempid );
			grand_fire_event_loop( repetion_id, parame.index, parame.tempid, parame.killedid,
				OGRE_TMP_LIST[ parame.tempid ].death_event )	
			Debug( "Grand " .. repetion_id .. " End Event Loop From NPC_DEATH, temp " .. parame.tempid );
		end	
	end

	step = GetIntData( repetion_id, GRAND_DATA_KEY.STEP );	
	Debug( "Grand " .. repetion_id .. " Step " .. step );

	if GRAND_STEP_EVENT[ step ] ~= nil then
		if GRAND_STEP_EVENT[ step ].npc_death ~= nil then
			Debug( "Grand " .. repetion_id .. " Begin Event Loop From NPC_DEATH GRAND_STEP_EVENT, temp" .. 
				parame.tempid );
			grand_fire_event_loop( repetion_id, parame.index, parame.tempid, parame.killedid, 
				GRAND_STEP_EVENT[ step ].npc_death );
			Debug( "Grand " .. repetion_id .. " End Event Loop From NPC_DEATH GRAND_STEP_EVENT, temp " .. 
				parame.tempid );
		end
	end
end

local function grand_season_time_end( repetion_index, repetion_id, args, loop_index )
	Debug( "Grand Season "  .. args.season .. " end" );
	step = GetIntData( repetion_id, GRAND_DATA_KEY.STEP );
	if GRAND_STEP_EVENT[ step ] ~= nil then
		if GRAND_STEP_EVENT[ step ].next_step ~= nil then
			grand_change_step( repetion_id, GRAND_STEP_EVENT[ step ].next_step );
		end	
	end
end

local function grand_time_end( repetion_index, repetion_id, args, loop_index )
	grand_end( repetion_id, RepetionEnd.UN_COMPLATE );
end


local function grand_begin( repetion_id, pararme )
	Debug( "Grand " .. repetion_id .. " Begin" );
	grand_change_step( repetion_id, GRAND_STEP.SPRING );
end

local function grand_assignment( repetion_id, parame )
	step = GetIntData( repetion_id, GRAND_DATA_KEY.STEP );

	Debug( "Grand " .. repetion_id .. " Ogre( index " .. parame.index .. " tempid " .. parame.tempid .. " entityid "
		.. parame.entityid .. " ) time assignment, step " .. step );

	if step == GRAND_STEP.AUTUMN then
		if OGRE_LIST[ parame.index ] ~= nil then
			if OGRE_LIST[ parame.index ].death_event ~= nil then
				Debug( "Grand " .. repetion_id .. " Begin Event Loop From Assignment, index " .. parame.index );
				grand_fire_event_loop( repetion_id, parame.index, parame.tempid, parame.entityid, 
					OGRE_LIST[ parame.index ].death_event );
				Debug( "Grand " .. repetion_id .. " End Event Loop From Assignment, index " .. parame.index );
			end
		end	

		if OGRE_TMP_LIST[ parame.tempid ] ~= nil then
			if OGRE_TMP_LIST[ parame.tempid ].death_event ~= nil then
				Debug( "Grand " .. repetion_id .. " Begin Event Loop From Assignment, temp " .. parame.tempid );
				grand_fire_event_loop( repetion_id, parame.index, parame.tempid, parame.entityid,
					OGRE_TMP_LIST[ parame.tempid ].death_event )	
				Debug( "Grand " .. repetion_id .. " End Event Loop From Assignment, temp " .. parame.tempid );
			end	
		end

		step = GetIntData( repetion_id, GRAND_DATA_KEY.STEP );	
	
		if GRAND_STEP_EVENT[ step ] ~= nil then
			if GRAND_STEP_EVENT[ step ].npc_death ~= nil then
				Debug( "Grand " .. repetion_id .. " Begin Event Loop From NPC_DEATH GRAND_STEP_EVENT, temp" .. 
					parame.tempid );
				grand_fire_event_loop( repetion_id, parame.index, parame.tempid, parame.entityid, 
					GRAND_STEP_EVENT[ step ].npc_death );
				Debug( "Grand " .. repetion_id .. " End Event Loop From NPC_DEATH GRAND_STEP_EVENT, temp " .. 
					parame.tempid );
			end
		end
	elseif step == GRAND_STEP.SPRING then
		EventNoticeAll( repetion_id, RepetionNoticeEventKind.MSG, 2, 0 );
	end


	CharacterDie( repetion_id, parame.entityid, 0 );
end

local function grand_time_create( repetion_index, repetion_id, args, loop_index )
	Debug( "Grand " .. repetion_id .. " Begin Event Loop From TIME, loop_index " .. loop_index );
	if args.time_fire[ loop_index ] ~= nil then
		grand_fire_event_loop( repetion_id, 0, 0, 0, args.time_fire[ loop_index ] )		
	end
	Debug( "Grand " .. repetion_id .. " End Event Loop From TIME, loop_index " .. loop_index );

	if args.change_time_space ~= nil then
		if args.change_time_space[ loop_index ] ~= nil and args.change_time_space[ loop_index + 1 ] ~= nil then
			change_space = math.abs( args.change_time_space[ loop_index + 1 ] - args.change_time_space[ loop_index ] );	
			ChangeTimeSpace( repetion_id, args.time_id, change_space * 1000 );
		end
	end
end

repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.GRAND,
		playercountmax = 2,		-- 
		mapid = { 99, 102, 103, 104 },
		taskid = 0,			-- into task condition 
		posx = 19,
		posy = 128,
	--	teamin = true,
		pernumber = 1,			-- everday into max number 
                weekpernumber = 3,
		minlevel = 40,
		maxlevel = 120,
		--check_time = 20000,		-- 增加
		question_check=0,		-- 是否进行副本答题验证
		relogin =	1,	-- 1表示可重入0表示不可重入
		closetime   = 120,	  -- 没有人时多久后关闭		
		team = 
		{
			teamin = true,		
			count = 2,			-- team redirect player number less
		},

		special = 
		{
			{ item = 75487, flag = 3 },
			{ item = 75488, flag = 3 }
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
				start = { year = 0, month = 0, week = 0, day = 0, hr = 13, minute = 00, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 15, minute = 00, sec = 0 }
			},

			--第二个时段
			{
				start = { year = 0, month = 0, week = 0, day = 0, hr = 18, minute = 00, sec = 0 },
				finish = { year = 0, month = 0, week = 0, day = 0, hr = 21, minute = 00, sec = 0 }
			},
		}
	},

	timer = 
	{ 
		[ GRAND_TIME_ID.LAST ] =
        {
            kind = RepetionTimeBeginKind.START_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = grand_last_time,
            loop = 1,
            func = grand_time_end,
			args = { },
            sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
        },
		[ GRAND_TIME_ID.GRAND_CLOSE ] =
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.CLOSE,
            time = grand_end_time,
            sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
        },
		[ GRAND_TIME_ID.SPRING_ID ] =
		{
		    kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = grand_sprint_last_time,
            loop = 1,
            func = grand_season_time_end,
			args = { season = GRAND_STEP.SPRING },
            sync = { kind = RepetionTimeSyncKind.ALL, key = 1 }
		},
		[ GRAND_TIME_ID.SPRINT_OGRE ] =
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 4 * 1000,
            loop = 5,
            func = grand_time_create,
			args = 
				{ 
					time_fire = 
					{
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98008, 98018, 98020, 98019 } }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98024, 98025, 98026, 98027, 98028 } }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98006, 98022, 98030, 98031, 98032 } }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98042, 98041, 98039, 98035, 98036, 98037} }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 98043, 98044, 98045, 98046, 98047, 98048} }
						}
					},

					change_time_space = { 4 , 42 , 87 , 123, 161 },
					time_id = GRAND_TIME_ID.SPRINT_OGRE;
				},
		},
		[ GRAND_TIME_ID.SUM_OGRE ] =
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 1 * 1000,
            loop = 22,
            func = grand_time_create,
			args = 
				{ 
					time_fire = 
					{
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101001, 101007,  101009 },
								debug_str = "begin create sum ogre" }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101001, 101007,  101009 },
								debug_str = "create sum ogre 2" }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101001, 101007,  101009 },
								debug_str = "create sum ogre 3" }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101012 },
								debug_str = "create sum ogre 4" }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101019, 101020 }, --, 101021, 101022 },
								condition = { { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
									key = GRAND_DATA_KEY.OGRE_INDEX_101012, value = 1 } },
								debug_str = "create sum ogre  5" }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101019, 101020 }, --, 101021, 101022 },
								condition = { { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
									key = GRAND_DATA_KEY.OGRE_INDEX_101012, value = 1 } },
								debug_str = "create sum ogre 6" }

						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101019, 101020, 101021 }, --, 101022 },
								condition = { { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
									key = GRAND_DATA_KEY.OGRE_INDEX_101012, value = 1 } },
								debug_str = "create sum ogre 7" }

						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101001, 101007,  101009 },
								debug_str = "create sum ogre 8" }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101001, 101007,  101009 },
								debug_str = "create sum ogre 9" }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101001, 101007,  101009 },
								debug_str = "create sum ogre 10" }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101013 },
								debug_str = "create sum ogre 11" }
						},	
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101011, 101010 },
								condition = { { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
									key = GRAND_DATA_KEY.OGRE_INDEX_101013, value = 1 } },
								debug_str = "create sum ogre 12" }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101024, 101028 }, --, 101026, 101027 },
								condition = { 
									{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
										key = GRAND_DATA_KEY.OGRE_INDEX_101011, value = 1 },
									{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
										key = GRAND_DATA_KEY.OGRE_INDEX_101010, value = 1 } 
								},
								debug_str = "create sum ogre 13"
							}
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101024, 101028 }, --, 101026, 101027 },
								condition = 
								{ 
									{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
										key = GRAND_DATA_KEY.OGRE_INDEX_101011, value = 1 },
									{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
										key = GRAND_DATA_KEY.OGRE_INDEX_101010, value = 1 } 
								},
								debug_str = "create sum ogre 14"
							}
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101024, 101028, 101026 }, --, 101027 },
								condition = 
								{ 
									{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
										key = GRAND_DATA_KEY.OGRE_INDEX_101011, value = 1 },
									{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
										key = GRAND_DATA_KEY.OGRE_INDEX_101010, value = 1 } 
								},
								debug_str = "create sum ogre 15"
							}
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101001, 101007,  101009 },
								debug_str = "create sum ogre 16" }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101001, 101007,  101009 },
								debug_str = "create sum ogre 17" }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101001, 101007,  101009 },
								debug_str = "create sum ogre 18" }
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101014 },
								debug_str = "create sum ogre 19" },
							{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 2 }
						},	
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101029, 101030, 101034, 101018  },
						--		101032, 101023, 101025, 101039 },
								condition = { { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
										key = GRAND_DATA_KEY.OGRE_INDEX_101014, value = 1 } },
								debug_str = "create sum ogre 20"
							}
						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101029, 101030, 101034, 101018 },
						--		101032, 101023, 101025, 101039 },
								condition = { { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
										key = GRAND_DATA_KEY.OGRE_INDEX_101014, value = 1 } },
								debug_str = "create sum ogre 21"
							}

						},
						{
							{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 101029, 101030, 101034, 101018, 101031 },
						--		101032, 101023, 101025, 101039 },
								condition = { { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, 
										key = GRAND_DATA_KEY.OGRE_INDEX_101014, value = 1 } },
								debug_str = "create sum ogre end"
							},		
							{ kind = GRAND_EVENT_KIND.TIME_END, parame1 = GRAND_TIME_ID.SUM_OGRE,
								debug_str = "sum create ogre time end"
							 }

						}
					},

					change_time_space = { 1, 24, 48, 80, 85, 100, 120, 124, 148, 172,  200, 206, 210, 220, 230, 244, 
						268, 292, 320, 326, 358, 390 },
					time_id = GRAND_TIME_ID.SUM_OGRE;
				},
		},
		[ GRAND_TIME_ID.AUTUMN_OGRE ] =
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = 6 * 1000,
            loop = 21,
            func = grand_time_create,
			args = 
			{
				time_id = GRAND_TIME_ID.AUTUMN_OGRE;
				change_time_space = { 6, 22, 38, 54, 70, 86, 102, 118, 134, 150, 166, 182, 205, 228, 251, 274, 296, 
					318, 345, 372, 400 },
				time_fire = 
				{
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102022, 102045 },
							debug_str = "create autu ogre begin"
						}
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102016, 102044 },
								debug_str = "create autu ogre 2"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102024, 102041 },
								debug_str = "create autu ogre 3"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102017, 102039 },
								debug_str = "create autu ogre 4"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102023, 102032 },
								debug_str = "create autu ogre 5"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102019, 102033 },
								debug_str = "create autu ogre 6"
						},
					},

					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102025, 102034 },
							condition = 
							{ 
								{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT, 
									value = 1 }
							},
						},
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102028, 102057 },
							condition = 
							{
								{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE_LARGE, key = GRAND_DATA_KEY.TREE_COUNT,
								 value = 1  },
							},
							debug_str = "create autu ogre 7"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102025, 102034 },
							condition = 
							{ 
								{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT, 
									value = 1 }
							},
						},
						{ kind = GRAND_EVENT_KIND.TIME_END, parame1 = GRAND_TIME_ID.AUTUMN_OGRE,
							condition = 
							{ 
								{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT, 
									value = 1 }
							},
							debug_str = "auto create ogre stop, tree count 1"
						},
						{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP,  parame2 = 2,
							condition = 
							{ 
								{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT, 
									value = 1 }
							},
						},
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102026, 102055 },
								debug_str = "create autu ogre  8",
							condition = 
							{
								{ kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE_LARGE, key = GRAND_DATA_KEY.TREE_COUNT,
								 value = 1  },
							},

						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102010, 102053 },
								debug_str = "create autu ogre 9"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102007, 102051 },
								debug_str = "create autu ogre 10"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102027, 102049, 102050 },
								debug_str = "create autu ogre 11"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102029, 102047, 102048 },
								debug_str = "create autu ogre 12"
						},
                        { kind = GRAND_EVENT_KIND.TIME_END, parame1 = GRAND_TIME_ID.AUTUMN_OGRE,
                            condition =
                            {
                                { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT, 
                                    value = 2 }  
                            },                                                                     
                            debug_str = "auto create ogre stop, tree count 2"                      
                        }, 
                        { kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 2,
                            condition =
                            {
                                { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT,
                                    value = 2 }
                            },
                            debug_str = "auto create ogre stop, tree count 2"
                        },
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102009, 102063, 102064 },
								debug_str = "create autu ogre 13"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102030, 102061, 102062 },
								debug_str = "create autu ogre 14"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102011, 102059, 102060 },
								debug_str = "create autu ogre 15"
						},
                        { kind = GRAND_EVENT_KIND.TIME_END, parame1 = GRAND_TIME_ID.AUTUMN_OGRE,
                            condition =
                            {
                                { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT,
                                    value = 3 }
                            },
                            debug_str = "auto create ogre stop, tree count 3"
                        },
                        { kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 2,
                            condition =
                            {
                                { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT,
                                    value = 3 }
                            },
                        },
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102012, 102069, 102070 },
								debug_str = "create autu ogre 16"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102031, 102067, 102068 },
								debug_str = "create autu ogre 17"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102013, 102065, 102066 },
								debug_str = "create autu ogre 18"
						},
                        { kind = GRAND_EVENT_KIND.TIME_END, parame1 = GRAND_TIME_ID.AUTUMN_OGRE,
                            condition =
                            {
                                { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT,
                                    value = 4 }
                            },
                            debug_str = "auto create ogre stop, tree count 4"
                        },
                        { kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 2,
                            condition =
                            {
                                { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT,
                                    value = 4 }
                            },
                        },

					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102014, 102073, 102074 },
								debug_str = "create autu ogre 19"
						},
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102015, 102071, 102072 },
							debug_str = "create autu ogre 20"
						},
                        { kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 2,
                            condition =
                            {
                                { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT,
                                    value = 5 }
                            },
                        },
                        { kind = GRAND_EVENT_KIND.TIME_END, parame1 = GRAND_TIME_ID.AUTUMN_OGRE,
                            condition =
                            {
                                { kind = GRAND_EVENT_CONDITION.DATA_KEY_VALUE, key = GRAND_DATA_KEY.TREE_COUNT,
                                    value = 5 }
                            },
                            debug_str = "auto create ogre stop, tree count 5"
                        },
					},
					{
						{ kind = GRAND_EVENT_KIND.CREATE_INDEX, list = { 102020, 102075, 102076 },
								debug_str = "create autu ogre 21"
						},
						{ kind = GRAND_EVENT_KIND.DATA_KEY_SET, parame1 = GRAND_DATA_KEY.SEASON_STEP, parame2 = 2 }
					}

				}
			}
		}
	},

  
	event = 
	{
		[ RepetionEventKind.OGRE_DEATH	] = { func = npc_death },	
		[ RepetionEventKind.OGRE_CREATE ] = { func = create_npc },
		[ RepetionEventKind.PLAYER_CREATE ] = { func = create_player },
		[ RepetionEventKind.BEGIN_REPETION ] = { func = grand_begin },
		[ RepetionEventKind.TIME_ASSIGNMENT ] = { func = grand_assignment },
	},
}


