

local wu_ogre_remove_buff;
local function wu_red_tao_death( repetion_id, parame )
	Debug( "========fun call wu_red_tao_death=============" );
	if parame.loop_index == nil then
		parame.loop_index = 1;
	else	
		parame.loop_index = parame.loop_index + 1;
	end

	if parame.result == 0 then
		local result = CharacterHasBuf( repetion_id, parame.entityid, parame.parame.buff_id );
		if result == false then
			parame.result = 1;	
			Debug( "WU DI DONG Entity " .. parame.entityid  .. " Insert Buff " .. parame.parame.buff_id );
			EntityInsertBuff( repetion_id, parame.entityid, parame.parame.buff_id );
--			EventNoticeAll( repetion_id, RepetionNoticeEventKind.MSG, parame.parame.notify.key, parame.entityid );
			parame.result = 1;
		end
	else
		parame.result = 1;
	end
end

local function wu_player_remove_buff( repetion_id, parame )
	Debug( "========fun call wu_player_remove_buff=============" );
	if parame.result == 0 then
		local result = CharacterHasBuf( repetion_id, parame.entityid, parame.parame.buff_id );
		if result == true then
			Debug( "WU DI DONG Entity " .. parame.entityid  .. " Remove Buff " .. parame.parame.buff_id );
			EntityRemoveBuff( repetion_id, parame.entityid, parame.parame.buff_id );
		end
	end

end

local function wu_end( repetion_id, parame )
	Debug( "===========WU END=============" );
end

local function wu_changepktype( repetion_id, parame )
	Debug( "========fun call wu_changepktype=============" );
	ChangePKType( parame.entityid, parame.parame.pk_type );
end



local WU_DATA_KEY=
{
	FLOOR_1_STEP			= 1;
	FLOOR_3_OGRE_COUNT		= 2;
	FLOOR_3_RED_TAO_COUNT 	= 3;
	FLOOR_3_GREEN_TAO_COUNT	= 4;

	FLOOR_1_POINT			= 5;
	FLOOR_2_POINT			= 6;
	FLOOR_3_POINT			= 7;
	FLOOR_4_POINT			= 8;

	FLOOR_1_OGRE_POINT		= 9; 
	FLOOR_2_OGRE_POINT		= 10; 
	FLOOR_3_OGRE_POINT		= 11; 
	FLOOR_4_OGRE_POINT		= 12; 

	FLOOR_1_EXTRA_POINT		= 13;
	FLOOR_2_EXTRA_POINT		= 14;
	FLOOR_3_EXTRA_POINT		= 15;
	FLOOR_4_EXTRA_POINT		= 16;
	
	FLOOR_LAST_TIME			= 17;
	FLOOR_4_PAIWEI			= 18;
};

local function wu_add_enmity( repetion_id, parame )
	local enmity = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_4_PAIWEI )
	AddEnmity( parame.entity, enmity, 1 );
end

local function wu_floor1_pass( repetion_id, parame )	
	local last_time = GetBeginTime( repetion_id );
	local now_time = os.time( );
	last_time = now_time - last_time;
	local ex_point = 0;
	Debug( " floor 1 last_time is  " .. last_time  );
	if last_time < 40*60 then
		ex_point = (1200 / last_time)  * 250;
		add_intdata( repetion_id, WU_DATA_KEY.FLOOR_1_EXTRA_POINT, ex_point);
		Debug( " floor 1 piont is  " .. ex_point  );
	end
	SetIntData( repetion_id, WU_DATA_KEY.FLOOR_LAST_TIME, now_time );
end

local function wu_floor2_pass( repetion_id, parame )
	local last_time = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_LAST_TIME );
	local now_time = os.time( );
	last_time = now_time - last_time;
	local ex_point = 0; 
	Debug( " floor 2 last_time is  " .. last_time );
	if last_time < 20*60 then
		ex_point = (900 / last_time) * 100;
		add_intdata( repetion_id, WU_DATA_KEY.FLOOR_3_EXTRA_POINT,ex_point);
		Debug( " floor 2 piont is  " .. ex_point  );
	end
	SetIntData( repetion_id, WU_DATA_KEY.FLOOR_LAST_TIME, now_time );
end

local function wu_floor3_pass( repetion_id, parame )
	local last_time = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_LAST_TIME );
	local now_time = os.time( );
	last_time = now_time - last_time;
	local ex_point = 0; 
	Debug( " floor 3 last_time is  " .. last_time  );
	if last_time < 30*60 then
		ex_point = (900 / last_time) * 500;
		add_intdata( repetion_id, WU_DATA_KEY.FLOOR_2_EXTRA_POINT, ex_point);
		Debug( " floor 3 piont is  " .. ex_point  );
	end
	SetIntData( repetion_id, WU_DATA_KEY.FLOOR_LAST_TIME, now_time );
end

local function wu_floor4_pass( repetion_id, parame )
	local last_time = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_LAST_TIME );
	local now_time = os.time( );
	last_time = now_time - last_time;
	local ex_point = 0; 
	Debug( " floor 4 last_time is  " .. last_time  );	
	if last_time < 30*60 then
		ex_point = (900 / last_time) *1000;
		add_intdata( repetion_id, WU_DATA_KEY.FLOOR_4_EXTRA_POINT, ex_point);
		Debug( " floor 4 piont is  " .. ex_point  );
	end
	SetIntData( repetion_id, WU_DATA_KEY.FLOOR_LAST_TIME, now_time );
end

local function wu_set_entity( repetion_id, parame )
	SetIntData( repetion_id, WU_DATA_KEY.FLOOR_4_PAIWEI, parame.entity )
end

local function wu_floor_3_pass( repetion_id, parame )
	Debug( "floor 3 tao end" );
	local count = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_3_GREEN_TAO_COUNT );
	if count == 1 or count == 2 then
		CreateNpc( repetion_id, 149051 );
	elseif count == 3 or count == 4 then
		CreateNpc( repetion_id, 149050 );
	elseif count == 5 or count == 6 then
		CreateNpc( repetion_id, 149049 );
	end
end

local function wu_point_js( repetion_id, parame )
	Debug( "================ WU POINT map " .. parame.map_id .. " tmp " .. parame.tmp_id .. " ==============" );
	table.foreachi( { 144, 130, 141, 142, 143, 145, 146, 147, 148 }, function( i, v )
		if parame.map_id == v then
			if parame.tmp_id == 72898 then
				add_intdata( repetion_id, WU_DATA_KEY.FLOOR_1_POINT, 500 );				
			else
				local point_count = add_intdata( repetion_id, WU_DATA_KEY.FLOOR_1_OGRE_POINT, 1 );
				if point_count == 216 then
					add_intdata( repetion_id, WU_DATA_KEY.FLOOR_1_EXTRA_POINT, 250 );
				end
			end
		end	
	end);

	if parame.map_id == 149 then
		if parame.tmp_id == 72940 then
			add_intdata( repetion_id, WU_DATA_KEY.FLOOR_2_POINT, 750 );
		end	
		table.foreachi( { 72902, 72903, 72904, 72905, 72906, 72907 }, function( i, v )
			if parame.tmp_id == v then
				local point_count = add_intdata( repetion_id, WU_DATA_KEY.FLOOR_2_OGRE_POINT, 1 );
			end
		end);
	end

	if parame.map_id == 150 then
		if parame.tmp_id == 72949 then
			add_intdata( repetion_id, WU_DATA_KEY.FLOOR_3_POINT, 1000 );
		end
		table.foreachi( { 72946, 72902, 72905, 72948, 72903, 72906, 72947, 72904, 72907 }, function( i, v )
			if parame.tmp_id == v then
				add_intdata( repetion_id, WU_DATA_KEY.FLOOR_3_OGRE_POINT, 1 );
				local less_count = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_3_OGRE_COUNT );
				EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 1, less_count ); 
			end
		end);
	end

	if parame.map_id == 151 then
		if parame.tmp_id == 72955 or parame.tmp_id == 72956 then
			add_intdata( repetion_id, WU_DATA_KEY.FLOOR_4_POINT, 1000 );
		end
	end	
end

local function wu_end_point( repetion_id, parame )
	Debug( "==================WU END POINT=====================" )
	local floor_1_ogre = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_1_OGRE_POINT ); 		
	local floor_2_ogre = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_2_OGRE_POINT ); 		
	local floor_3_ogre = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_3_OGRE_POINT ); 		
	local floor_4_ogre = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_4_OGRE_POINT ); 		

	local floor_1_point = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_1_POINT ); 		
	local floor_2_point = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_2_POINT ); 		
	local floor_3_point = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_3_POINT ); 		
	local floor_4_point = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_4_POINT ); 		

	local floor_1_extra = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_1_EXTRA_POINT ); 		
	local floor_2_extra = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_2_EXTRA_POINT ); 		
	local floor_3_extra = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_3_EXTRA_POINT ); 		
	local floor_4_extra = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_4_EXTRA_POINT ); 

	local level = GetRepeLevel( repetion_id );
	Debug( "================ floor1ex " .. floor_1_extra .. " floor2ex " .. floor_2_extra .. " ==============" );
	Debug( "================ floor3ex " .. floor_3_extra .. " floor4ex " .. floor_4_extra .. " ==============" );

	if floor_1_ogre > 216 then
		floor_1_ogre = 216;
	end

	if floor_2_ogre > 8 then
		floor_2_ogre = 8;
	end
	
	if floor_1_extra > 1000 then
		floor_1_extra = 1000;
	end
	
	if floor_2_extra > 1500 then
		floor_2_extra = 1500;
	end
	
	if floor_3_extra > 600 then
		floor_3_extra = 600;
	end
	
	if floor_4_extra > 3000 then
		floor_4_extra = 3000;
	end

	local point = 	( floor_1_ogre / 216 * 1000 ) + 
					( floor_2_ogre / 8 * 500 ) + 
					( floor_3_ogre / 29 * 500 ) ;

	point = ( point + floor_1_point + floor_2_point + floor_3_point + floor_4_point ) * level / 100;

	local extra_point = floor_1_extra + floor_2_extra + floor_3_extra + floor_4_extra;
	local cost_time = os.time( ) - GetBeginTime( repetion_id );

	if floor_1_point > 0 then
		AwardFameAll( repetion_id, 12 );
	end

	if floor_2_point > 0 then
		AwardFameAll( repetion_id, 13 );
	end

	if floor_3_point > 0 then
		AwardFameAll( repetion_id, 14 );
	end

	if floor_4_point > 0 then
		AwardFameAll( repetion_id, 15 );
	end

	EventNoticeAll( repetion_id, RepetionNoticeEventKind.MSG, 14, level );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.MSG, 15, cost_time );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.MSG, 16, floor_1_ogre + floor_2_ogre + floor_3_ogre + floor_4_ogre );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.MSG, 17, point );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.MSG, 18, extra_point );

	AddIntData2AllPlayer( repetion_id, 0, point + extra_point );
end

local WU_EVENT_TYPE=
{
	CREATE_ENTITY_RAND = 1;
	CREATE_ENTITY = 2;
	ADD_BUFF = 3;
	DEL_BUFF = 4;
	CHANGE_KEY_DATA = 5;
	DEL_BUFF_RAND = 6;
	DESTROY_ENTITY = 7;
	CHANGE_NPC_CAMP = 8,
	REFRESH_NPC_VIEW = 9,
	NPC_LOOP = 10,
	PLAYER_LOOP = 11, 
	TIME_BEGIN	= 12,
	TIME_END	= 13,
	FUN_CALL	= 14,
	MOVETO		= 15,
	CHARACTER_DIE = 16,
	EVENT_NOTICE = 17,
	CREATE_HIEROGRAM = 18,
};

local WU_EVENT_CONDITION_TYPE =
{
	KEY_DATA_VALUE = 1;
};

local WU_STATIC_KEY =
{
	RED_OGRE = 1;
	BLUE_OGRE = 2;
	YELLOW_OGRE = 3;
};

local WU_STATIC_VALUE =
{
	[ WU_STATIC_KEY.RED_OGRE ] =
	{
		head_ogre = { tmp = 72914, index = 143000 },	
		box_ogre = { tmp = 72962, index = 129001 },
		tele_index = 129002,
		plan_value = 1,
	},
	[ WU_STATIC_KEY.BLUE_OGRE ] =
	{
		head_ogre = { tmp = 72915, index = 143001 },
		box_ogre = { tmp = 72964, index = 141001 },
		tele_index = 141002,
		plan_value = 2,
	},
	[ WU_STATIC_KEY.YELLOW_OGRE ] =
	{	
		head_ogre = { tmp = 72916, index = 143002 },
		box_ogre = { tmp = 72963, index = 145001 },
		tele_index = 145002,
		plan_value = 4,
	}
};

local WU_OGRE_EVENT_BY_TEMP =
{
	[ 72898 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.EVENT_NOTICE, notify = { { kind = RepetionNoticeEventKind.MSG, key = 8  } } }
		},
		death_event = 
		{ 
			{ 
				id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 147017 } ,
			},
			{
				id = WU_EVENT_TYPE.FUN_CALL, fun = wu_floor1_pass, parame = { add_point = 500 },
				notify = { { kind = RepetionNoticeEventKind.MSG, key = 23  } }
			} 
		},
	},
	[ 72908 ] = 
	{
		death_event = 
		{ 
			{ id = WU_EVENT_TYPE.DESTROY_ENTITY, list = { 73092 } },
			{ id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 129001 } }, 
			{ id = WU_EVENT_TYPE.EVENT_NOTICE, notify = { { kind = RepetionNoticeEventKind.MSG, key = 5 } } },
--			{ id = WU_EVENT_TYPE.EVENT_NOTICE, notify = { { kind = RepetionNoticeEventKind.MSG, key = 22 } } },
		},
	};
	[ 72909 ] =
	{
		death_event = 
		{ 
			{ id = WU_EVENT_TYPE.DESTROY_ENTITY, list = { 73094 } },
			{ id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 141001 } },
			{ id = WU_EVENT_TYPE.EVENT_NOTICE, notify = { { kind = RepetionNoticeEventKind.MSG, key = 6 } } },
--			{ id = WU_EVENT_TYPE.EVENT_NOTICE, notify = { { kind = RepetionNoticeEventKind.MSG, key = 22 } } },
		},
	};
	[ 72910 ] =
	{
		death_event = 
		{ 
			{ id = WU_EVENT_TYPE.DESTROY_ENTITY, list = { 73093 } },
			{ id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 145001 } },
			{ id = WU_EVENT_TYPE.EVENT_NOTICE, notify = { { kind = RepetionNoticeEventKind.MSG, key = 7 } } },
--			{ id = WU_EVENT_TYPE.EVENT_NOTICE, notify = { { kind = RepetionNoticeEventKind.MSG, key = 22 } } },
		},
	};
	[ 72914 ] =
	{
		event_fired = { 
						{ 
							fire_event_id = WU_EVENT_TYPE.DEL_BUFF, 
							condition = { parame1 = 72923 }, 
							event = 
							{ 
								{
									id = WU_EVENT_TYPE.DEL_BUFF, 
									tmp_list = { 72946, 72902, 72905 },  
									buff_list = { 72923 }
								}
							},  
						},
					  },
	};
	[ 72915 ] =
	{
		event_fired = { 
						{ 
							fire_event_id = WU_EVENT_TYPE.DEL_BUFF, 
							condition = { parame1 = 72923 }, 
							event = 
							{ 
								{
									id = WU_EVENT_TYPE.DEL_BUFF, 
									tmp_list = { 72948, 72903 },  
									buff_list = { 72923 }
								}
							},  
						},
					  },
	},
	[ 72916 ] =
	{
		event_fired = { 
						{ 
							fire_event_id = WU_EVENT_TYPE.DEL_BUFF, 
							condition = { parame1 = 72923 }, 
							event = 
							{ 
								{
									id = WU_EVENT_TYPE.DEL_BUFF, 
									tmp_list = { 72947, 72904, 72907 },  
									buff_list = { 72923 }
								}
							},  
						},
					  },
	},
	[ WU_STATIC_VALUE[ WU_STATIC_KEY.RED_OGRE ].box_ogre.tmp ] =
	{
		machinery_event = 
		{
			{
				id = WU_EVENT_TYPE.CREATE_ENTITY, list = { WU_STATIC_VALUE[ WU_STATIC_KEY.RED_OGRE ].tele_index }
			},
			{
				id = WU_EVENT_TYPE.CHANGE_KEY_DATA, 
				key = WU_DATA_KEY.FLOOR_1_STEP, 
				value = WU_STATIC_VALUE[ WU_STATIC_KEY.RED_OGRE ].plan_value,
				notify = { 
							{ kind = RepetionNoticeEventKind.MSG, key = 21 }, 
							{ kind = RepetionNoticeEventKind.MSG, key = 22 }, 
						 }
			},	
		}
	},	
	[ WU_STATIC_VALUE[ WU_STATIC_KEY.BLUE_OGRE ].box_ogre.tmp ] =
	{
		machinery_event = 
		{
			{
				id = WU_EVENT_TYPE.CREATE_ENTITY, list = { WU_STATIC_VALUE[ WU_STATIC_KEY.BLUE_OGRE ].tele_index }
			},			
			{
				id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_1_STEP, 
				value = WU_STATIC_VALUE[ WU_STATIC_KEY.BLUE_OGRE ].plan_value;
				notify = { 
							{ kind = RepetionNoticeEventKind.MSG, key = 21 }, 
							{ kind = RepetionNoticeEventKind.MSG, key = 22 } 
						 }
			}	
		}
	},	
	[ WU_STATIC_VALUE[ WU_STATIC_KEY.YELLOW_OGRE ].box_ogre.tmp ] =
	{
		machinery_event = 
		{
			{
				id = WU_EVENT_TYPE.CREATE_ENTITY, list = { WU_STATIC_VALUE[ WU_STATIC_KEY.YELLOW_OGRE ].tele_index }
			},	
			{
				id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_1_STEP, 
				value = WU_STATIC_VALUE[ WU_STATIC_KEY.YELLOW_OGRE ].plan_value;
				notify = { 
							{ kind = RepetionNoticeEventKind.MSG, key = 21 }, 
							{ kind = RepetionNoticeEventKind.MSG, key = 22 }, 
						 }
			}
		}
	},
	[ 72945 ] =
	{
		death_event = 
		{
			{ id = WU_EVENT_TYPE.DESTROY_ENTITY, list = { 72941, 72942, 72943, 72957 } },
			{ 
				id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 148174 } ,
				notify =
				{
					{ kind = RepetionNoticeEventKind.MSG, key = 9 }
				}
			}
		},
	},
	[ 72940 ] = 
	{
		death_event = 
		{
			{ id = WU_EVENT_TYPE.DESTROY_ENTITY, list = { 72941, 72942, 72943, 72957 } },
			{ id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 148179 } },
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_floor2_pass, parame = { add_point = 750 }  }
		}
	},
	[ 72952 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_RED_TAO_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_RED_TAO_COUNT, value = -1 },
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_EXTRA_POINT, value = 50 },
			{ 
				id = WU_EVENT_TYPE.NPC_LOOP, 
				list = { 72950, 72951 },  
				fun = wu_red_tao_death, 
				map_id = 150,
				parame = { buff_id = 72958 },
				notify = 
				{
					{ kind = RepetionNoticeEventKind.MSG, key = 11 }
				}
			}
		}
	},
	[ 72953 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_GREEN_TAO_COUNT, value = 1 }
		},
		death_event =
		{
	--		{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_GREEN_TAO_COUNT, value = -1 },
			{ 
				id = WU_EVENT_TYPE.PLAYER_LOOP, 
				fun = wu_red_tao_death, 
				map_id = 150,
				parame = { buff_id = 72958 },
				notify = 
				{
					{ kind = RepetionNoticeEventKind.MSG, key = 10 }
				}
			},
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_GREEN_TAO_COUNT, value = -1 },
		}
	},
	[ 72949 ] =
	{
		create_event = 
		{
			{ 
				id = WU_EVENT_TYPE.FUN_CALL, fun = wu_floor_3_pass, parame = {},
				notify = 
				{
					{ kind = RepetionNoticeEventKind.MSG,  key = 19 },
				}
			}	
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 149052 } },
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_floor3_pass, parame = { add_point = 1000 }  }
		}
	},
	[ 73218 ] =
	{
		machinery_event =
		{
			{ id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 150000 } }
		},
		
	},
	[ 72956 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.EVENT_NOTICE, notify = { { kind = RepetionNoticeEventKind.MSG, key = 12 }  } },
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_set_entity, parame = { } }
		},
		death_event = 
		{
			{ id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 150091 } },
			{ id = WU_EVENT_TYPE.TIME_END, list = { 2, 4, 5 } }
		},
	},
	[ 72955 ] =
	{
		death_event =
		{
			{ id = WU_EVENT_TYPE.TIME_END,	list = { 1 } },	
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_end, parame = { complate = RepetionEnd.COMPLATE } },
			{ id = WU_EVENT_TYPE.TIME_BEGIN, list = { 3 } },	
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_floor4_pass, parame = { add_point = 1000 }  },
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_end_point, parame = {  }  }
		}
	}, 
		
};


local WU_OGRE_EVENT_BY_INDEX =
{
	[ 143000 ] =
	{
		death_event = 
		{ 
			{ 
				id = WU_EVENT_TYPE.CREATE_ENTITY_RAND, list = { 140000, 129000, 142000 }, count = 1 ,
				notify = 
				{
					{ kind = RepetionNoticeEventKind.MSG, key = 2 }
				}
			} 
		};
	},
	[ 143001 ] =
	{
		death_event = 
		{ 
			{
				id = WU_EVENT_TYPE.CREATE_ENTITY_RAND, 
				list = { 140001, 141000, 144000 }, 
				count = 1,
				notify =
				{
					{ kind = RepetionNoticeEventKind.MSG, key = 3 }
				}
			} 
		};
	};
	[ 143002 ] =
	{
		death_event = 
		{ 
			{ 
				id = WU_EVENT_TYPE.CREATE_ENTITY_RAND, list = { 142001, 145000, 146000 }, count = 1 ,
				notify =
				{
					{ kind = RepetionNoticeEventKind.MSG, key = 4 }
				}
			} 
		};
	};
	[ 129001 ] =
	{
		death_event = { { id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 129002 } } };	
	};
	[ 141001 ] = 
	{
		death_event = { { id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 141002 } } };	
	};
	[ 145001 ] = 
	{
		death_event = { { id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 141001 } } };	
	};
	[ 149000 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149001 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149001 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149002 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149003 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149004 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149005 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149006 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149007 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149008 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149030 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149031 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149032 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149033 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149034 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149035 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149036 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149037 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149038 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149039 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149040 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149041 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149042 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149043 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149044 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149045 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149046 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149047 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 149048 ] =
	{
		create_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 1 }
		},
		death_event =
		{
			{ id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = -1 }
		}
	},
	[ 150001 ] =
	{
		create_event = 
		{
--			{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 30, y = 38 } },
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 }, tmp_list = { 72956 } },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150010 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
		--	{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 30, y = 38 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 } , tmp_list = { 72956 } },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150002 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
		--	{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 30, y = 39 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 }, tmp_list = { 72956 }  },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150011 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
		--	{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 30, y = 39 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 }, tmp_list = { 72956 }  },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150003 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
--			{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 32, y = 39 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 } , tmp_list = { 72956 } },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150012 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
		--	{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 32, y = 39 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 } , tmp_list = { 72956 } },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150004 ] =
	{
		create_event = 
		{

			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
	--		{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 30, y = 37 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 } , tmp_list = { 72956 } },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150009 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
	--		{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 30, y = 37 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 } , tmp_list = { 72956 } },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150005 ] =
	{
		create_event = 
		{
--			{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 31, y = 37 } }
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 }, tmp_list = { 72956 }  },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150016 ] =
	{
		create_event = 
		{
--			{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 31, y = 37 } }
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 }, tmp_list = { 72956 }  },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150006 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
	--		{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 32, y = 37 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 }, tmp_list = { 72956 }  },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150015 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
	--		{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 32, y = 37 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 }, tmp_list = { 72956 }  },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150007 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
	--		{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 32, y = 38 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 }, tmp_list = { 72956 }  },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150014 ] =
	{
		create_event = 
		{

			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
	--		{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 32, y = 38 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 }, tmp_list = { 72956 }  },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150008 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
	--		{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 32, y = 39 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 }, tmp_list = { 72956 }  },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 150013 ] =
	{
		create_event = 
		{
			{ id = WU_EVENT_TYPE.FUN_CALL, fun = wu_add_enmity, parame = { } },
	--		{ id = WU_EVENT_TYPE.MOVETO, pos = { x = 32, y = 39 } }
		},
		stop_event = 
		{
			{ id = WU_EVENT_TYPE.ADD_BUFF, buff_list = { 73221, 75477 }, tmp_list = { 72956 }  },
			{ id = WU_EVENT_TYPE.CHARACTER_DIE },
		}
	},
	[ 148000 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 19, y = 137 } }
--		}
	},
	[ 148001 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 22, y = 137 } }
--		}
	},
	[ 148002 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 25, y = 137 } }
--		}
	},
	[ 148003 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 19, y = 137 } }
--		}
	},
	[ 148004 ] =
	{
		death_event =
		{
			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 22, y = 140 } }
		}
	},
	[ 148005 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 25, y = 140 } }
--		}
	},
	[ 148024 ] =
	{
--		death_event =
---		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 73, y = 44 } }
--		}
	},
	[ 148025 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 76, y = 44 } }
--		}
	},
	[ 148026 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 79, y = 44 } }
--		}
	},
	[ 148027 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 75, y = 46 } }
--		}
	},
	[ 148028 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 78, y = 46 } }
--		}
	},
	[ 148044 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 69, y = 149 } }
--		}
	},
	[ 148045 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 73, y = 149 } }
--		}
	},
	[ 148046 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 71, y = 151 } }
--		}
	},
	[ 148047 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 75, y = 151 } }
--		}
	},
	[ 148071 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 140, y = 40 } }
--		}
	},
	[ 148072 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 144, y = 40 } }
--		}
	},
	[ 148070 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 142, y = 37 } }
--		}
	},
	[ 148096 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 116, y = 164 } }
--		}
	},
	[ 148097 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 118, y = 169 } }
--		}
	},
	[ 148127 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 198, y = 40 } }
--		}
	},
	[ 148168 ] =
	{
--		death_event =
--		{
--			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75096, pos = { x = 175, y = 180 } }
--		}
	},
	[ 150000 ] =
	{
		death_event =
		{
			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75102, pos = { x = 22, y = 42 } },
			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75102, pos = { x = 25, y = 46 } },
			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75102, pos = { x = 28, y = 50 } },
			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75103, pos = { x = 33, y = 28 } },
			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75103, pos = { x = 36, y = 32 } },
			{ id = WU_EVENT_TYPE.CREATE_HIEROGRAM, tmp_id = 75103, pos = { x = 39, y = 36 } },
		}
	}
};



local WU_DATA_KEY_EVENT = 
{
	[ WU_DATA_KEY.FLOOR_1_STEP ] =
	{
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, 
				key = WU_DATA_KEY.FLOOR_1_STEP;
				value = WU_STATIC_VALUE[ WU_STATIC_KEY.RED_OGRE ].plan_value
			},
			id = WU_EVENT_TYPE.DEL_BUFF_RAND, 
			tmp_list = 
			{ 
				WU_STATIC_VALUE[ WU_STATIC_KEY.BLUE_OGRE ].head_ogre.tmp,  
				WU_STATIC_VALUE[ WU_STATIC_KEY.YELLOW_OGRE ].head_ogre.tmp
			},

			buff_list = { 72923 },
			count = 1,
			notify = 
			{
				{ kind = RepetionNoticeEventKind.MSG,  key = 1 }
			}
		},	
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, 
				key = WU_DATA_KEY.FLOOR_1_STEP;
				value = WU_STATIC_VALUE[ WU_STATIC_KEY.BLUE_OGRE ].plan_value
			},
			id = WU_EVENT_TYPE.DEL_BUFF_RAND, 
			tmp_list = 
			{ 
				WU_STATIC_VALUE[ WU_STATIC_KEY.RED_OGRE ].head_ogre.tmp,  
				WU_STATIC_VALUE[ WU_STATIC_KEY.YELLOW_OGRE ].head_ogre.tmp
			},

			buff_list = { 72923 },
			count = 1,
			notify = 
			{
				{ kind = RepetionNoticeEventKind.MSG,  key = 1 }
			}
		},
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, 
				key = WU_DATA_KEY.FLOOR_1_STEP;
				value = WU_STATIC_VALUE[ WU_STATIC_KEY.YELLOW_OGRE ].plan_value
			},
			id = WU_EVENT_TYPE.DEL_BUFF_RAND, 
			tmp_list = 
			{ 
				WU_STATIC_VALUE[ WU_STATIC_KEY.RED_OGRE ].head_ogre.tmp,  
				WU_STATIC_VALUE[ WU_STATIC_KEY.BLUE_OGRE ].head_ogre.tmp
			},

			buff_list = { 72923 },
			count = 1,
			notify = 
			{
				{ kind = RepetionNoticeEventKind.MSG,  key = 1 }
			}
		},
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, 
				key = WU_DATA_KEY.FLOOR_1_STEP;
				value = WU_STATIC_VALUE[ WU_STATIC_KEY.RED_OGRE ].plan_value + 
						WU_STATIC_VALUE[ WU_STATIC_KEY.BLUE_OGRE ].plan_value
			},
			id = WU_EVENT_TYPE.DEL_BUFF, 
			tmp_list = 
			{ 
				WU_STATIC_VALUE[ WU_STATIC_KEY.YELLOW_OGRE ].head_ogre.tmp,  
			},

			buff_list = { 72923 },
			notify = 
			{
				{ kind = RepetionNoticeEventKind.MSG,  key = 1 }
			}
		},
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, 
				key = WU_DATA_KEY.FLOOR_1_STEP;
				value = WU_STATIC_VALUE[ WU_STATIC_KEY.RED_OGRE ].plan_value + 
						WU_STATIC_VALUE[ WU_STATIC_KEY.YELLOW_OGRE ].plan_value
			},
			id = WU_EVENT_TYPE.DEL_BUFF, 
			tmp_list = 
			{ 
				WU_STATIC_VALUE[ WU_STATIC_KEY.BLUE_OGRE ].head_ogre.tmp,  
			},

			buff_list = { 72923 },
			notify = 
			{
				{ kind = RepetionNoticeEventKind.MSG,  key = 1 }
			}
		},
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, 
				key = WU_DATA_KEY.FLOOR_1_STEP;
				value = WU_STATIC_VALUE[ WU_STATIC_KEY.BLUE_OGRE ].plan_value + 
						WU_STATIC_VALUE[ WU_STATIC_KEY.YELLOW_OGRE ].plan_value
			},
			id = WU_EVENT_TYPE.DEL_BUFF, 
			tmp_list = 
			{ 
				WU_STATIC_VALUE[ WU_STATIC_KEY.RED_OGRE ].head_ogre.tmp,  
			},
			buff_list = { 72923 },
			notify = 
			{
				{ kind = RepetionNoticeEventKind.MSG,  key = 1 }
			}
		},
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, 
				key = WU_DATA_KEY.FLOOR_1_STEP;
				value = 0
			},
			id = WU_EVENT_TYPE.DEL_BUFF_RAND, 
			tmp_list = 
			{ 
				WU_STATIC_VALUE[ WU_STATIC_KEY.RED_OGRE ].head_ogre.tmp,  
				WU_STATIC_VALUE[ WU_STATIC_KEY.BLUE_OGRE ].head_ogre.tmp,  
				WU_STATIC_VALUE[ WU_STATIC_KEY.YELLOW_OGRE ].head_ogre.tmp,  
			},
			buff_list = { 72923 },
			count = 1,
			notify = 
			{
				{ kind = RepetionNoticeEventKind.MSG,  key = 1 }
			}
		},
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, 
				key = WU_DATA_KEY.FLOOR_1_STEP;
				value = WU_STATIC_VALUE[ WU_STATIC_KEY.BLUE_OGRE ].plan_value + 
						WU_STATIC_VALUE[ WU_STATIC_KEY.YELLOW_OGRE ].plan_value +
						WU_STATIC_VALUE[ WU_STATIC_KEY.RED_OGRE ].plan_value
			},
			id = WU_EVENT_TYPE.DEL_BUFF, 
			tmp_list = 
			{ 
				72947, 72906, 72948
			},

			buff_list = { 72923 }
		},
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, 
				key = WU_DATA_KEY.FLOOR_1_STEP;
				value = WU_STATIC_VALUE[ WU_STATIC_KEY.BLUE_OGRE ].plan_value + 
						WU_STATIC_VALUE[ WU_STATIC_KEY.YELLOW_OGRE ].plan_value +
						WU_STATIC_VALUE[ WU_STATIC_KEY.RED_OGRE ].plan_value
			},
			id = WU_EVENT_TYPE.CREATE_ENTITY, 
			list = 
			{ 
				147000	
			},
			
		},
	},
	[ WU_DATA_KEY.FLOOR_3_OGRE_COUNT ] =
	{
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, key = WU_DATA_KEY.FLOOR_3_OGRE_COUNT, value = 0
			},
			id = WU_EVENT_TYPE.CREATE_ENTITY,
			list = 
			{ 
				149027, 149009, 149010, 149011, 149012, 149013, 149014, 149015, 149016, 149017, 149018, 149019,
				149020, 149021, 149022, 149023, 149024, 149025, 149026
			},
			notify = 
			{	
				{ kind = RepetionNoticeEventKind.MSG,  key = 20 }
			}
		},
	},
	[ WU_DATA_KEY.FLOOR_3_RED_TAO_COUNT ] =
	{
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, key = WU_DATA_KEY.FLOOR_3_RED_TAO_COUNT, value = 0
			},
			id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 149028 }
		},
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, key = WU_DATA_KEY.FLOOR_3_RED_TAO_COUNT, value = 0
			},
			id = WU_EVENT_TYPE.DESTROY_ENTITY, list = { 72952, 72953, 72950, 72951 }
		},
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, key = WU_DATA_KEY.FLOOR_3_RED_TAO_COUNT, value = 0
			},
			id = WU_EVENT_TYPE.PLAYER_LOOP,
			fun = wu_player_remove_buff,
			parame = { buff_id = 72958 }, 
		}
	},
	[ WU_DATA_KEY.FLOOR_3_GREEN_TAO_COUNT ] =
	{
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, key = WU_DATA_KEY.FLOOR_3_GREEN_TAO_COUNT, value = 0
			},
			id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 149028 }
		},
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, key = WU_DATA_KEY.FLOOR_3_GREEN_TAO_COUNT, value = 0
			},
			id = WU_EVENT_TYPE.DESTROY_ENTITY, list = { 72952, 72953, 72950, 72951 }
		},
		{
			condition = 
			{
				id = WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE, key = WU_DATA_KEY.FLOOR_3_GREEN_TAO_COUNT, value = 0
			},
			id = WU_EVENT_TYPE.PLAYER_LOOP,
			fun = wu_player_remove_buff,
			parame = { buff_id = 72958 }, 
		}

	},
};


local function wu_fun( repeiton_id, parame )
	Debug( "=======================fun call ====================" );
	if parame == nil then
		Debug( "parame nil" );
	else
		Debug( "parame.index " .. parame.index );
	end
end

local function wu_event_condition( repetion_id, temp_id, index, condition )
	Debug( "WU DI DONG Condition repetion_id " .. repetion_id .. " temp_id " .. temp_id .. " index " .. index ..
		" condition id " .. condition.id );
	if condition ~= nil then
		if condition.id == WU_EVENT_CONDITION_TYPE.KEY_DATA_VALUE then
			local key_data = GetIntData( repetion_id, condition.key );
			if key_data == condition.value then
				return 1; 
			else
				return 0;
			end
		else
			Debug( "Don't Have Any Condition Type " .. condition.id );
		end	
		return 0;
	end
	
	return 1;
end

local function wu_event_doing( repetion_id, temp_id, index, event, entity )
	Debug( "WU DI DONG " .. repetion_id .. " tmp_id " .. temp_id .. " index " .. index .. " donging event count " 
		.. table.getn( event ) );
	table.foreachi( event, function( i, v ) 
		if v.condition ~= nil then
			local condition_rtn = wu_event_condition( repetion_id, temp_id, index, v.condition );
			if condition_rtn == 0 then
				return;	
			end
			
		end
		local notice_date = 0;
		if v.id == WU_EVENT_TYPE.CREATE_ENTITY_RAND then
			local loop_index = 0;
			for loop = 1, v.count, 1 do
				local rand_index = math.random( 1, table.getn( v.list ) );
				Debug( "WU DI DONG " .. repetion_id .. " tmp_id " .. temp_id .. " index " .. index .. 
					" Create Npc " .. v.list[ rand_index ]  );
				notice_date = v.list[ rand_index ];
				CreateNpc( repetion_id, v.list[ rand_index ] );
			end 
		elseif v.id == WU_EVENT_TYPE.ADD_BUFF then	
			table.foreachi( v.buff_list, function( o, p )
				table.foreachi( v.tmp_list, function( m, n )
					NpcInsertBuffByMapID( repetion_id, n, p, v.map_id );
				end);	
			end);
		elseif v.id == WU_EVENT_TYPE.DEL_BUFF then
			Debug( "WU DI DONG Doing Event DEL BUFF" );
			table.foreachi( v.tmp_list, function( j, w )
				table.foreachi( v.buff_list, function( k, x )
					wu_ogre_remove_buff( repetion_id, w, x );
				end);
			end);
			notice_date = v.tmp_list[ 1 ];
		elseif v.id == WU_EVENT_TYPE.DEL_BUFF_RAND then
			Debug( "WU DI DONG Doing Event DEL BUFF RAND" );
			local loop_index = 0;
			for loop = 1, v.count, 1 do
				local rand_index = math.random( 1, table.getn( v.tmp_list ) );
				table.foreachi( v.buff_list, function( j, w )
					wu_ogre_remove_buff( repetion_id, v.tmp_list[ rand_index ], w );
				end);
				notice_date = v.tmp_list[ rand_index ];
			end 
		elseif v.id == WU_EVENT_TYPE.CREATE_ENTITY then
			Debug( "WU DI DONG Doing Event Create Npc" );
			create_npc_all( repetion_id, v.list );	
		elseif v.id == WU_EVENT_TYPE.CHANGE_KEY_DATA then
			Debug( "WU DI DONG Doing Event Change Data Key" );
			add_intdata( repetion_id, v.key, v.value );
			if WU_DATA_KEY_EVENT[ v.key ] ~= nil then
				wu_event_doing( repetion_id, temp_id, index, WU_DATA_KEY_EVENT[ v.key ] );	
			end
		elseif v.id == WU_EVENT_TYPE.DESTROY_ENTITY then
			Debug( "WU DI DONG Doing Event Destroy Entity" );
			table.foreachi( v.list, function( m, n )	
				DestroyAliveTempOgre( repetion_id, n, 0 );
			end);
		elseif v.id == WU_EVENT_TYPE.CHARACTER_DIE then
			CharacterDie( repetion_id, entity, 0 );
		elseif v.id == WU_EVENT_TYPE.CHANGE_NPC_CAMP then
			table.foreachi( v.list, function( m, n )
				ChangeCampIDByTempID( repetion_id, n, v.camp_id );
			end);
		elseif v.id == WU_EVENT_TYPE.REFRESH_NPC_VIEW then
			table.foreachi( v.list, function( m, n )	
				NpcRefreshView( repetion_id, n, v.map_id );
			end);
		elseif v.id == WU_EVENT_TYPE.NPC_LOOP then
			local local_parame = { fun = v.fun, parame = v.parame, result = 0 };

			table.foreach( v.list, function( m, n )
				local_parame.loop_temp = n;
				Debug( "NPC LOOP Event In Lua" );
				if v.map_id == nil then
					NpcLoop4TempInLuaByMapID( repetion_id, n, 0, local_parame  );
				else
					NpcLoop4TempInLuaByMapID( repetion_id, n, v.map_id, local_parame );
				end
			end);
		elseif v.id == WU_EVENT_TYPE.PLAYER_LOOP then
			local local_parame = { fun = v.fun, parame = v.parame, result = 0 };

			if v.map_id == nil then
				PlayerLoopInLuaByMapID( repetion_id, 0, local_parame  );
			else
				PlayerLoopInLuaByMapID( repetion_id, v.map_id, local_parame );
			end
		elseif v.id == WU_EVENT_TYPE.TIME_BEGIN then
			table.foreachi( v.list, function( k, m )
				TimeBegin( repetion_id, m );
			end);
		elseif v.id == WU_EVENT_TYPE.TIME_END then
			table.foreachi( v.list, function( k, m )
				TimeEnd( repetion_id, m );
			end);
		elseif v.id == WU_EVENT_TYPE.FUN_CALL then
			local local_parame = { temp_id = temp_id, index = index, entity = entity, parame = v.parame }	
			v.fun( repetion_id, local_parame );
		elseif v.id == WU_EVENT_TYPE.MOVETO then
			Debug( "Entity " .. entity .. " MoveTo " .. v.pos.x .. " " .. v.pos.y );
			NpcMove( repetion_id, entity, v.pos.x, v.pos.y );
		elseif v.id == WU_EVENT_TYPE.EVENT_NOTICE then
			Debug( "Notify Event" );
		elseif v.id == WU_EVENT_TYPE.CREATE_HIEROGRAM then
			local mapid = GetEntityMapID( repetion_id, entity );
			CreateHierogramByMap( repetion_id, mapid, v.tmp_id, v.pos.x, v.pos.y );	
		else
			Debug( "WU DI DONG Don't Have Any Event Type " .. v.id );
		end;

		if v.notify ~= nil then
			table.foreachi( v.notify, function( k, m )
				EventNoticeAll( repetion_id, m.kind, m.key, notice_date );	
			end);
		end
	end);
end

local function wu_floor4_npc_pos( repetion_id, parame )
	Debug( "============fun call wu_floor4_npc_pos================" );
	if 	( parame.posx >= parame.parame.left.x 
		and parame.posx <= parame.parame.right.x )
		and ( parame.posy >= parame.parame.left.y
		and parame.posy <= parame.parame.right.y )
	then
		Debug( "wu_floor4_npc_pos call entity " .. parame.entityid .. " pos( " .. parame.posx .. ", " .. parame.posy .. " )" );	

		if WU_OGRE_EVENT_BY_INDEX[ parame.index ] ~= nil then
			if WU_OGRE_EVENT_BY_INDEX[ parame.index ].stop_event ~= nil then
				wu_event_doing( repetion_id, parame.loop_temp, parame.index, 
					WU_OGRE_EVENT_BY_INDEX[ parame.index ].stop_event, parame.entityid );
			end
		end
	end
end




--function wu_ogre_remove_buff_condition( repetion_id, parame )
--	Debug( "========fun call wu_ogre_remove_buff_condition=============" );
--	if parame.private_parame ~= nil then
--		if parame.private_parame.index == nil then
--			parame.private_parame.index = 1;
--			Debug( "Add parame.private_parame.index" );
--		else
--			parame.private_parame.index = parame.private_parame.index + 1;
--			local index = parame.private_parame.index;
--			Debug( "parame.private_parame.index" .. index  );
--		end
--	else
--		Debug( "private nil" )
--	end
--end


local function wu_ogre_fire_event( repetion_id, tmp_id, index, fire_event, condit, parame )
	Debug( "WU DI DONG " ..  repetion_id .. " tmp_id " .. tmp_id .. " index " .. index .. " event " .. condit .. 
		" fire" );
	table.foreachi( fire_event, function( i, v )
		Debug( " 2 " );
		if v.fire_event_id == condit then
			if v.condition ~= nil then
				Debug( "3" );
				if v.condition.parame1 ~= parame then
					Debug( "4" );
					return 1;
				end
			end
			Debug( "5" );
			wu_event_doing( repetion_id, tmp_id, index, v.event );
		end
	end);
end

function wu_ogre_remove_buff( repetion_id, tmp_id, buff_id )
	Debug( "WU DI DONG tmp_id " .. tmp_id .. " remove buff " .. buff_id );
	if WU_OGRE_EVENT_BY_TEMP[ tmp_id ] ~= nil then
	    if WU_OGRE_EVENT_BY_TEMP[ tmp_id ].event_fired ~= nil then
			wu_ogre_fire_event( repetion_id, tmp_id, 0, WU_OGRE_EVENT_BY_TEMP[ tmp_id ].event_fired,
				WU_EVENT_TYPE.DEL_BUFF, buff_id );
	    end
	end
	NpcRemoveBuff( repetion_id, tmp_id, buff_id );
end



local function wu_ogre_death( repetion_id, parame )
	Debug( "WU DI DONG " .. repetion_id .. " entity death index " .. parame.index .. " tempid " .. parame.tempid );
	if WU_OGRE_EVENT_BY_INDEX[ parame.index ] ~= nil then
		if WU_OGRE_EVENT_BY_INDEX[ parame.index ].death_event ~= nil then
			wu_event_doing( repetion_id, parame.tempid, parame.index, 
				WU_OGRE_EVENT_BY_INDEX[ parame.index ].death_event, parame.killedid );
		end
	end
	if WU_OGRE_EVENT_BY_TEMP[ parame.tempid ] ~= nil then
		if WU_OGRE_EVENT_BY_TEMP[ parame.tempid ].death_event ~= nil then
			wu_event_doing( repetion_id, parame.tempid, parame.index, 
				WU_OGRE_EVENT_BY_TEMP[ parame.tempid ].death_event, parame.killedid );
		end
	end

	local js_parame = { map_id = parame.real_map, tmp_id = parame.tempid };

	wu_point_js( repetion_id, js_parame );

end

local function wu_create_player( repetion_id, parame )
	EventNoticeSingle( repetion_id, parame.entityid, RepetionNoticeEventKind.MSG, 21, parame.mapid )
	if parame.mapid == 150 then
		local less_count = GetIntData( repetion_id, WU_DATA_KEY.FLOOR_3_OGRE_COUNT );
		EventNoticeSingle( repetion_id, parame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 1, less_count );
	end
end

local function wu_create_ogre( repetion_id, parame )
	Debug( "WU DI DONG " .. repetion_id .. " create entity index " .. parame.index .. " tempid " .. parame.tempid );
	local is_observer_ogre_death = 0;
	local is_observer_ogre_machinery = 0;
	if WU_OGRE_EVENT_BY_INDEX[ parame.index ] ~= nil then
		if WU_OGRE_EVENT_BY_INDEX[ parame.index ].death_event ~= nil then
			is_observer_ogre_death = 1;
		end
		if WU_OGRE_EVENT_BY_INDEX[ parame.index ].machinery_event ~= nil then
			is_observer_ogre_machinery = 1;
		end
		if WU_OGRE_EVENT_BY_INDEX[ parame.index ].create_event ~= nil then
			wu_event_doing( repetion_id, parame.tempid, parame.index, 
				WU_OGRE_EVENT_BY_INDEX[ parame.index ].create_event, parame.entityid );
		end
	end
	if WU_OGRE_EVENT_BY_TEMP[ parame.tempid ] ~= nil then
		if WU_OGRE_EVENT_BY_TEMP[ parame.tempid ].death_event ~= nil then
			is_observer_ogre_death = 1;
		end
		if WU_OGRE_EVENT_BY_TEMP[ parame.tempid ].machinery_event ~= nil then
			is_observer_ogre_machinery = 1;
		end
		if WU_OGRE_EVENT_BY_TEMP[ parame.tempid ].create_event ~= nil then
			wu_event_doing( repetion_id, parame.tempid, parame.index, 
				WU_OGRE_EVENT_BY_TEMP[ parame.tempid ].create_event, parame.entityid );
		end
	end	

--	if is_observer_ogre_death == 1 then
		ObserverEntityDeath( repetion_id, parame.entityid );
--	end
	if is_observer_ogre_machinery == 1 then
		ObserverMachinery( repetion_id, parame.entityid );
	end
end

local function wu_begin( repetion_id, parame )

	local begin_event =
	{
		{ 
			id = WU_EVENT_TYPE.CHANGE_KEY_DATA, key = WU_DATA_KEY.FLOOR_1_STEP, value = 0 
		},
	};
	wu_event_doing( repetion_id, 72914, 0, begin_event );
--	wu_ogre_remove_buff( repetion_id, 72914, 72923 );
end

local function wu_machinery_spring( repetion_id, parame )
	Debug( "WU DI DONG " .. repetion_id .. " entity machinery index " .. parame.index .. " tempid " .. parame.tempid );
	if WU_OGRE_EVENT_BY_INDEX[ parame.index ] ~= nil then
		if WU_OGRE_EVENT_BY_INDEX[ parame.index ].machinery_event ~= nil then
			wu_event_doing( repetion_id, parame.tempid, parame.index, 
				WU_OGRE_EVENT_BY_INDEX[ parame.index ].machinery_event );
		end
	end
	if WU_OGRE_EVENT_BY_TEMP[ parame.tempid ] ~= nil then
		Debug( "7" );
		if WU_OGRE_EVENT_BY_TEMP[ parame.tempid ].machinery_event ~= nil then
			Debug( "8" );
			wu_event_doing( repetion_id, parame.tempid, parame.index, 
				WU_OGRE_EVENT_BY_TEMP[ parame.tempid ].machinery_event );
		end
	end

	CharacterDie( vRepetionID, parame.machineryid, 0 );
end


local function wu_each_other( repetion_id, parame )
	Debug( "repetion " .. repetion_id .." ========WU EACH OTHER==========" );

	if parame.key == 2 then
		local each_other =
		{
			{ id = WU_EVENT_TYPE.CREATE_ENTITY, list = { 150000 } },
			{ id = WU_EVENT_TYPE.TIME_BEGIN,	list = { 2, 4, 5 } },
			{ id = WU_EVENT_TYPE.DESTROY_ENTITY, list = { 73218 } }
		}
		wu_event_doing( repetion_id, 0, 0, each_other );
	elseif parame.key == 1 then
		local change_camp = 
		{ 
			{ id = WU_EVENT_TYPE.CHANGE_NPC_CAMP, list = { 72953 }, camp_id = 1 },
			{ id = WU_EVENT_TYPE.CHANGE_NPC_CAMP,list = { 72950, 72951 }, camp_id = 2 },
			{ id = WU_EVENT_TYPE.REFRESH_NPC_VIEW, list = { 72950, 72953, 72951 }, map_id = 0 },
			{ id = WU_EVENT_TYPE.DEL_BUFF, tmp_list = { 72951, 72950 }, buff_list = { 73203 } },
			{ id = WU_EVENT_TYPE.DESTROY_ENTITY, list = { 73199 } },
			{ 
				id = WU_EVENT_TYPE.NPC_LOOP, 
				list = { 72952 }, 
				fun = wu_changepktype, 
				map_id = 150, 
				parame = { pk_type = 12480 },
            }
		};

		wu_event_doing( repetion_id, 0, 0, change_camp );	
	end
end

local function wu_call_fun( repetion_id, parame )
	parame.fun( repetion_id, parame );
end

local function wu_create_npc( repetion_index, repetion_id, args, loop_index )
	local use_index = { };
	local ogre_count = 0;
	local loop_i = 0;
	while ogre_count < 3 and loop_i < 20
	do
		local create_index = math.random( 1, table.getn( args ) );
		local is_use = 0;
		table.foreachi( use_index, function( i, v ) 
			if v == create_index then
				is_use = 1;
				return;
			end	
			if i >= 10 then
				return;
			end
		end);
		if is_use == 0 then	
			create_npc_all( repetion_id, args[ create_index ] );
			ogre_count = ogre_count + 1;
		end
		loop_i = loop_i + 1;	
	end

	EventNoticeAll( repetion_id, RepetionNoticeEventKind.MSG, 13, 0 );

	ChangeTimeSpace( repetion_id, 2, 1 * 60 * 1000 );

	Debug( "Create Npc loop " .. loop_index .. " create index " .. tostring( use_index[ 1 ] ) .. " " .. tostring( use_index[ 2 ] ) .. " " .. tostring( use_index[ 3 ] ) );
end

local function wu_end_time( repetion_index, repetion_id, args, loop_index )
	wu_end_point( repetion_id, nil );
	TimeBegin( repetion_id, 3 );
end

local function wu_floor4_end_time( repetion_index, repetion_id, args, loop_index )
	wu_end_point( repetion_id, nil );
	TimeEnd( repetion_id, 1 );
	TimeBegin( repetion_id, 3 );
	DestroyAliveTempOgre( repetion_id, 72956, 0 )
end


local function wu_npc_time( repetion_index, repetion_id, args, loop_index )
	if args.event == nil then
		return;
	end

	wu_event_doing( repetion_id, 0, 0, args.event );
end

local function wu_movestop( repetion_id, parame )
	Debug( "WU DI DONG " .. repetion_id .. " move entity index " .. parame.index .. " tempid " .. parame.tempid );
	local is_observer_ogre_death = 0;
	local is_observer_ogre_machinery = 0;
	if WU_OGRE_EVENT_BY_INDEX[ parame.index ] ~= nil then
		if WU_OGRE_EVENT_BY_INDEX[ parame.index ].stop_event ~= nil then
			wu_event_doing( repetion_id, parame.tempid, parame.index, 
				WU_OGRE_EVENT_BY_INDEX[ parame.index ].stop_event, parame.entityid );
		end
	end
	if WU_OGRE_EVENT_BY_TEMP[ parame.tempid ] ~= nil then
		if WU_OGRE_EVENT_BY_TEMP[ parame.tempid ].stop_event ~= nil then
			wu_event_doing( repetion_id, parame.tempid, parame.index, 
				WU_OGRE_EVENT_BY_TEMP[ parame.tempid ].stop_event, parame.entityid );
		end
	end	
end

repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.WUDIDONG,
		playercountmax = 6,		-- 
		mapid = { 144, 130, 141, 142, 143, 145, 146, 147, 148, 149,  150, 151 },
		taskid = 0,			-- into task condition 
		posx = 32,
		posy = 25,
		pernumber = 1,			-- everday into max number 
                weekpernumber = 1,
		minlevel = 80,
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
			{
				start = { year = 0, month = 0, week = 6, day = 0, hr = 0, minute = 5, sec = 0 },
				finish = { year = 0, month = 0, week = 6, day = 0, hr = 23, minute = 55, sec = 0 }
			},
			{
				start = { year = 0, month = 0, week = 7, day = 0, hr = 0, minute = 5, sec = 0 },
				finish = { year = 0, month = 0, week = 7, day = 0, hr = 23, minute = 55, sec = 0 }
			},
		}
	},

	timer = 
	{ 
		[ 1 ] =
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 1000 * 60 * 90,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 },
			loop = 1,
			func = wu_end_time,
			args = { }
		},
		[ 2 ] =
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 10 * 1000, --1 * 60 * 1000,
			loop = 8,
			func = wu_create_npc,
			args = 
			{ 
				{ 150001, 150010, 150010, 150010, 150010 },
				{ 150002, 150011, 150011, 150011, 150011 },
				{ 150003, 150012, 150012, 150012, 150012 },
				{ 150004, 150009, 150009, 150009, 150009 },
				{ 150005, 150016, 150016, 150016, 150016 }, 
				{ 150006, 150015, 150015, 150015, 150015 },
				{ 150007, 150014, 150014, 150014, 150014 },
				{ 150008, 150013, 150013, 150013, 150013 },
			},
--			sync = { kind = RepetionTimeSyncKind.ALL, key = 1 }
		},	
		[ 3 ] =
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = 1000 * 60 * 2,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 },
		},
		[ 4 ] =
		{
			kind =  RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
            time = 1000,
            loop = 3600 * 60,
            func = wu_npc_time,
			args = 
			{
				event = 
				{
					{ 	
						id = WU_EVENT_TYPE.NPC_LOOP, 
						fun = wu_floor4_npc_pos,
						list = { 73262, 73263, 73264, 73265, 73266, 73267, 73268, 73269, 73270 },
						map_id = 151,
						parame = { left = { x = 30, y = 37 }, right = { x = 32, y = 39 } },
					}
				}
			}
		},
		[ 5 ] =
		{
			kind =  RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = 1000 * 60 * 10,
			sync = { kind = RepetionTimeSyncKind.ALL, key = 2 },
			loop = 1,
			func = wu_floor4_end_time,
			args = { }
		},
	},

	event = 
	{
		[ RepetionEventKind.OGRE_DEATH	] = { func = wu_ogre_death },	
		[ RepetionEventKind.OGRE_CREATE ] = { func = wu_create_ogre },
		[ RepetionEventKind.BEGIN_REPETION ] = { func = wu_begin },
		[ RepetionEventKind.BOX_MACHINERY_SPING ] = { func = wu_machinery_spring },
		[ RepetionEventKind.PLAYER_CREATE ] = { func = wu_create_player },
		[ RepetionEventKind.EACH_OTHER ] = { func = wu_each_other },
		[ RepetionEventKind.CALL_FUN ] = { func = wu_call_fun },
--		[ RepetionEventKind.ENTITY_MOVE_STOP ] = { func = wu_movestop },
	},

}


