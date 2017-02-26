

longgong_class = class( );

longgong_class.kinglong_fact_skill = 40508;


longgong_class.lg_close_time = 2 * 60 * 1000;					--关闭时间
longgong_class.lg_complate_time = 30 * 60 * 1000;				--副本完成时间
longgong_class.lg_notice_time = 5 * 60 * 1000;

local lg_enco = 
{
	{ levelmin = 20, levelmax = 59, itemid = 45534 },
	{ levelmin = 60, levelmax = 99, itemid = 45534 },
	{ levelmin = 100, levelmax = 129, itemid = 45534 }
};

--[[
-- 龙王真身废话
longgong_class.lg_kinglong_fact_assignment1 = 
	{ time = 10000, other_info = { mapid = 0, flag = 1 }, endtime = 190000 };
-- 龙王真身战斗
longgong_class.lg_kinglong_fact_assignment2 = 
	{ time = 190000, other_info = { mapid = 0, flag = 2 }, endtime = 30000 };
]]--
--阻挡计时
longgong_class.lg_block_time = 60 * 1000 * 4;

local rand = 0;
longgong_class.conceal_time = 4 * 60 * 1000;
longgong_class.conceal_end_time = 30 * 1000;
local tCount = 0;

--龙宫运行数据解释
local LG_DATA_KEY =
{
	FIRST_MAGGIC_BLOCK_1				= 0;			--第一个魔法门(第一部分)
	SECOND_MAGGIC_BLOCK_1				= 1;			--第二个魔法门(第一部分)
	THIRD_MAGGIC_BLOCK_1				= 2;			--第三个魔法门(第一部分)
	FORTH_MAGGIC_BLOCK_1				= 3;			--第四个魔法门(第一部分)
	SECOND_MAGGIC_BLOCK_NUM				= 4;			--第二个魔法门记数
	THIRD_MAGGIC_BLOCK_NUM				= 5;			--第三个魔法门计数
	FORTH_MAGGIC_BLOCK_NUM				= 6;			--第四个魔法门计数
	KINGLONG_ASSIGNMENT					= 7;			--龙王计时ID
	BOY_ENTITYID						= 8;			--男孩EntityID
	GIRL_ENTITYID						= 9;			--女孩EntityID
	CLOSE_BOX							= 10;			--被最后一个魔法门被打开时候， 关闭的宝箱
	FIRST_MAGGIC_BLOCK_2				= 11;			--第一个魔法门(第二部分)
	FIRST_MAGGIC_BLOCK_3				= 12;			--第一个魔法门(第三部分)
	SECOND_MAGGIC_BLOCK_2				= 13;			--第二个魔法门(第二部分)
	THIRD_MAGGIC_BLOCK_2				= 14;			--第三个魔法门(第二部分)
	FORTH_MAGGIC_BLOCK_2				= 15;			--第四个魔法门(第二部分)
	FORTH_MAGGIC_BLOCK_3				= 16;			--第四个魔法门(第三部分)
	ALL_OGRE_NUM						= 17;			--全部小怪数量
	KILL_OGRE_NUM						= 18;			--杀怪数量
	MARK_POINT							= 19;			--积分
	MARK_POINT_END						= 20;			--最后积分
	FIVE_MAGGIC_BLOCK					= 21;			--第五个魔法门
	CONCEAL_NUM							= 22;			-- 隐藏怪物计数
};

local LG_PLAYER_DATA_KEY =
{
	MARK_POINT							= 0;
};



function longgong_class:ctor( )
	self.lg_block	=	
	{
		{ 
			index = { },
			tempid = 40289, 									--模版ID
			flag = 												--EntityID保存 	
			{
				LG_DATA_KEY.FIRST_MAGGIC_BLOCK_1,
				LG_DATA_KEY.FIRST_MAGGIC_BLOCK_2,
				LG_DATA_KEY.FIRST_MAGGIC_BLOCK_3
			},
		},
		{ 
			index = { },
			tempid = 40289, 
			flag = 
			{
				LG_DATA_KEY.SECOND_MAGGIC_BLOCK_1,
				LG_DATA_KEY.SECOND_MAGGIC_BLOCK_2
			},
			num = LG_DATA_KEY.SECOND_MAGGIC_BLOCK_NUM,			--计数标识
																--计数怪创建CreateroIndex
		},
		{ 
			index = { },
			tempid = 40289, 
			flag =
			{ 
				LG_DATA_KEY.THIRD_MAGGIC_BLOCK_1, 
				LG_DATA_KEY.THIRD_MAGGIC_BLOCK_2
			}, 
			num = LG_DATA_KEY.THIRD_MAGGIC_BLOCK_NUM,
		},
		{ 
			index = { },
			tempid = 40294, 
			flag = 
			{ 
				LG_DATA_KEY.FORTH_MAGGIC_BLOCK_1,
				LG_DATA_KEY.FORTH_MAGGIC_BLOCK_2,
				LG_DATA_KEY.FORTH_MAGGIC_BLOCK_3
			}, 
			num = LG_DATA_KEY.FORTH_MAGGIC_BLOCK_NUM,
			timeend = 6,											--限时通过时间结束
		--	{
		--		2273, 2272, 2275, 2246, 2283, 2282, 2277, 2276, 2280, 2281, 2286, 2289, 2288, 563, 2285,
		--		1827, 1898, 1826, 1533, 1824
		--	},
			closeflag = LG_DATA_KEY.CLOSE_BOX,
		},				
		{
			index = { },
			timebegin = 6,											--限时通过时间开启
		},
		{
			index = { },
			tempid = 50222,
			flag = { LG_DATA_KEY.FIVE_MAGGIC_BLOCK, },
		},
	};

	self.lg_ogre_machinery = 
	{ 
		{ tempid = 40295 },--{ 760, 890, 1142 } };
		{ tempid = 40295 }, --{ 1523, 1341, 1515 } };
		{ tempid = 40294, entityflag = LG_DATA_KEY.CLOSE_BOX };
		{ rand = { min = 1, max = 4000 } },
		{ rand = { min = 4000, max = 10000 }, destroyentity = LG_DATA_KEY.FIVE_MAGGIC_BLOCK, 
	--		ogreindex = { self.lg_conceal_ogre_index_dream },
			begin_time_index = 7,
			end_time_index = 2,
			notice_event = 
			{
				{ key = RepetionNoticeEventKind.MSG, parame1 = 1, parame2 = 0 },
	--			{ key = RepetionNoticeEventKind.COUNT_DOWN, parame1 = 3, parame2 = longgong_class.conceal_time }
			}
		}
	};
-- 龙王真身废话
	self.lg_kinglong_fact_assignment1 = 
	{ time = 10000, other_info = { mapid = 0, flag = 1 }, endtime = 190000 };
-- 龙王真身战斗
	self.lg_kinglong_fact_assignment2 = 
	{ time = 190000, other_info = { mapid = 0, flag = 2 }, endtime = 30000 };


	

end

--[[
--龙宫阻挡NPC
longgong_class.lg_block	=	
	{
		{ 
			index = { },
			tempid = 40289, 									--模版ID
			flag = 												--EntityID保存 	
			{
				LG_DATA_KEY.FIRST_MAGGIC_BLOCK_1,
				LG_DATA_KEY.FIRST_MAGGIC_BLOCK_2,
				LG_DATA_KEY.FIRST_MAGGIC_BLOCK_3
			},
		},
		{ 
			index = { },
			tempid = 40289, 
			flag = 
			{
				LG_DATA_KEY.SECOND_MAGGIC_BLOCK_1,
				LG_DATA_KEY.SECOND_MAGGIC_BLOCK_2
			},
			num = LG_DATA_KEY.SECOND_MAGGIC_BLOCK_NUM,			--计数标识
																--计数怪创建CreateroIndex
		},
		{ 
			index = { },
			tempid = 40289, 
			flag =
			{ 
				LG_DATA_KEY.THIRD_MAGGIC_BLOCK_1, 
				LG_DATA_KEY.THIRD_MAGGIC_BLOCK_2
			}, 
			num = LG_DATA_KEY.THIRD_MAGGIC_BLOCK_NUM,
		},
		{ 
			index = { },
			tempid = 40294, 
			flag = 
			{ 
				LG_DATA_KEY.FORTH_MAGGIC_BLOCK_1,
				LG_DATA_KEY.FORTH_MAGGIC_BLOCK_2,
				LG_DATA_KEY.FORTH_MAGGIC_BLOCK_3
			}, 
			num = LG_DATA_KEY.FORTH_MAGGIC_BLOCK_NUM,
			timeend = 6,											--限时通过时间结束
		--	{
		--		2273, 2272, 2275, 2246, 2283, 2282, 2277, 2276, 2280, 2281, 2286, 2289, 2288, 563, 2285,
		--		1827, 1898, 1826, 1533, 1824
		--	},
			closeflag = LG_DATA_KEY.CLOSE_BOX,
		},				
		{
			index = { },
			timebegin = 6,											--限时通过时间开启
		}
	};

--龙宫机关宝乡箱
longgong_class.lg_ogre_machinery = { 
		{ tempid = 40295 },--{ 760, 890, 1142 } };
		{ tempid = 40295 }, --{ 1523, 1341, 1515 } };
		{ tempid = 40294, entityflag = LG_DATA_KEY.CLOSE_BOX };
		{ } 
	};
--临时变量
]]--
function longgong_class:init( )
	self.lg_kinglong_fact_assignment1.other_info.mapid = self.mapid;
	self.lg_kinglong_fact_assignment2.other_info.mapid = self.mapid;

	self.death_create =
	{
		{ 
			death_index = self.lg_conceal_ogre_index_dream, 
			create_index = self.lg_conceal_ogre_index_run,
		}
	};

	self.lg_conceal_ogre =
	{
		ogre_list = { self.lg_conceal_ogre_index_run, self.lg_conceal_ogre_index_dream },
		count_key = LG_DATA_KEY.CONCEAL_NUM,
		begin_time_index = 8,
		end_time_index = 7,
		notice_event = 
		{ 
--			{ key = RepetionNoticeEventKind.COUNT_DOWN_END, parame1 = 3 },
--			{ key = RepetionNoticeEventKind.CLOSE, parame1 = self.conceal_end_time }
		}
	};

	self.lg_ogre_machinery[ 5 ].ogreindex = { self.lg_conceal_ogre_index_dream }; 
--	self.lg_ogre_machinery[ 5 ].notice_event[ 2 ].parame2 = self.conceal_time;
end

local tNum = 0;
local tEntity = 0;
local tRtn = 0;
local tMark = 0;
local tTime = 0;
local tKillFact = 0;
local tLevel = 0;
local tObser = 0

function longgong_class:create_ogre( vRepetionID, vParame )--, vParame1, vParame2 )
--	Infor( "Call Longgong create_ogre " .. vParame1 .. " : " .. vParame2 );
	Debug( "Call Longgong create_ogre entityid " .. vParame.entityid .. " tempid " .. vParame.tempid .. " index "
		.. vParame.index );

	tObser = 0;

	table.foreachi( self.lg_ogre_temp, function( i, v )
		if vParame.tempid == v then
			tRtn = 0;
			ObserverEntityDeath( vRepetionID, vParame.entityid );	
			--隐藏怪物不记数
			tRtn = 	table.foreachi( self.lg_ogre_machinery, function( k, m )
						tRtn = 0;
						tRtn =	table.foreachi( m.ogreindex, function( j, o )
									if o == vParame.index then
										return 1;
									end
								end);
						if tRtn ~= nil then
							Debug( "In Marchinery k " .. k .. " tRtn " .. tRtn );
							return 1;
						end
					end);
			if tRtn ~= nil then
				return 1;
			end
			
			add_intdata( vRepetionID, LG_DATA_KEY.ALL_OGRE_NUM, 1 );
			return 1;
		end
	end);

	table.foreachi( self.death_create, function( i, v )
		if v.death_index == vParame.index then
			ObserverEntityDeath( vRepetionID, vParame.entityid );
			tObser = 1;
		end
	end);

	if vParame.tempid == self.lg_kinglong_template then
		-- 创建龙王， 建立龙王死亡观察事件
		Infor( "Observer kinglong die" );
--		ObserverEntityDeath( vRepetionID, vParame1 );	
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		return;
	end


	if vParame.index == self.lg_kinglong_fact.index then
--		ChangePKType( vParame.entityid, 12480 );
--		AllPlayerAddEnmitiy( vRepetionID, vParame.entityid, 1 );
		ObserverAssignment( vRepetionID, vParame.entityid );
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		SetAssignment( vRepetionID, vParame.entityid, self.lg_kinglong_fact_assignment1.time, 
			self.lg_kinglong_fact_assignment1.other_info );
		SetIntData( vRepetionID, LG_DATA_KEY.KINGLONG_ASSIGNMENT,  SetAssignment( vRepetionID, vParame.entityid, 
			self.lg_kinglong_fact_assignment2.time, self.lg_kinglong_fact_assignment2.other_info ) );
--		EventNoticeAll( vRepetionID, RepetionNoticeEventKind.COUNT_DOWN, 2, self.lg_kinglong_fact_assignment1.endtime );
--		ChangeTimeSpace( repetion_id, 2, lg_kinglong_fact_assignment1.endtime );
		TimeEnd( vRepetionID, 2 );
		TimeBegin( vRepetionID, 4 );
		return;
	end

	if vParame.index == self.lg_kinglong_fact_hostage.boy.index then
		Debug( "Create boy" );
		SetIntData( vRepetionID, LG_DATA_KEY.BOY_ENTITYID, vParame.entityid );
		return;
	end

	if vParame.index == self.lg_kinglong_fact_hostage.girl.index then
		Debug( "Create Girl" );
		SetIntData( vRepetionID, LG_DATA_KEY.GIRL_ENTITYID, vParame.entityid );
		return;
	end

	table.foreachi( self.lg_ogre_machinery, function( i, v )
		if v.index == vParame.index then
			ObserverMachinery( vRepetionID, vParame.entityid );
			if v.entityflag ~= nil then
				SetIntData( vRepetionID, v.entityflag, vParame.entityid );
			end
			return 1;
		end
	end);

--	table.foreachi( lg_general_ogre, function( i, v )
--		if v.index == vParame.index  then
--			ObserverEntityDeath( vRepetionID, vParame.entityid );
--			return;
--		end
--	end);

	table.foreachi( self.lg_conceal_ogre.ogre_list, function( i, v )
		if v == vParame.index then
			tCount = add_intdata( vRepetionID,  self.lg_conceal_ogre.count_key, 1 );
			if tObser == 0 then
				ObserverEntityDeath( vRepetionID, vParame.entityid );
			end
			return 1;
		end
	end);


	table.foreachi( self.lg_block, function( i, v )
		tRtn = 0;
		tRtn = 	table.foreachi( v.index, function( k, m )
					if m == vParame.index then
					--阻挡门
						if v.flag ~= nil then
							SetIntData( vRepetionID, v.flag[ k ], vParame.entityid );
						end
						if k == 1 then
							if v.timebegin ~= nil then
								ObserverEntityDeath( vRepetionID, vParame.entityid );
							end
							if v.timeend ~= nil then
								ObserverEntityDeath( vRepetionID, vParame.entityid );
							end
						end
						return 1;
					end
				end);
		if tRtn == 1 then
			return 1;
		end
		if v.machinery ~= nil then
			--开门机关
			if v.machinery.index == vParame.index then
				ObserverMachinery( vRepetionID, vParame.entityid );
				return 1;
			end
		end
		
		if v.ogreindex ~= nil then
			--杀怪计数
			table.foreachi( v.ogreindex, function( k, m )

				if m == vParame.index then
					tNum = 0;
					tNum = GetIntData( vRepetionID, v.num ) + 1;
					SetIntData( vRepetionID, v.num, tNum );
					--
--					ObserverEntityDeath( vRepetionID, vParame.entityid );
					return 1;
				end
			end);
		end
	end);
end

function longgong_class:lg_end( repetion_id, kill )  	--kill 是否杀死龙王真身( 0 未, 其他是 )
--	tNum = GetIntData( repetion_id, LG_DATA_KEY.KILL_OGRE_NUM );

	--先减去最后积分
--	tMark = GetIntData( repetion_id, LG_DATA_KEY.MARK_POINT_END );

--	AddIntData2AllPlayer( repetion_id, LG_PLAYER_DATA_KEY.MARK_POINT, -tMark );

	tMark = GetIntData( repetion_id, LG_DATA_KEY.MARK_POINT );
--	if tNum == GetIntData( repetion_id, LG_DATA_KEY.ALL_OGRE_NUM ) then
--		tMark = tMark + 100;
--		Debug( "Kill All Ogre, Mark Add 100" );
--	end
--	tLevel = GetRepeLevel( repetion_id );

--	tTime = GetBeginTime( repetion_id );
--	tTime = os.time( ) - tTime;

--	tMark = ( tMark + ( tNum * 10 / 9 ) * ( tNum * 10 / 9 ) / ( 50 + tTime ) * 100 ) * ( 1 + tLevel / 60 );
--	tMark = ( tMark + 100 * tNum * tNum / ( 50 + tTime ) ) * ( 1 + tLevel / 60 ) * 4 / 10;

	if kill ~= 0 then
		tKillFact = ( ( 1 + tLevel / 60 ) )* self.lg_kinglong_fact.mark;
		AddIntData2AllPlayer( repetion_id, LG_PLAYER_DATA_KEY.MARK_POINT, tKillFact );
	else
		tKillFact = 0;
		tNum = GetIntData( repetion_id, LG_DATA_KEY.KILL_OGRE_NUM );

--		tMark = GetIntData( repetion_id, LG_DATA_KEY.MARK_POINT );
		if tNum == GetIntData( repetion_id, LG_DATA_KEY.ALL_OGRE_NUM ) then
			tMark = tMark + 100;
			Debug( "Kill All Ogre, Mark Add 100" );
		end
		tLevel = GetRepeLevel( repetion_id );

		tTime = GetBeginTime( repetion_id );
		tTime = os.time( ) - tTime;



		--	tMark = ( tMark + ( tNum * 10 / 9 ) * ( tNum * 10 / 9 ) / ( 50 + tTime ) * 100 ) * ( 1 + tLevel / 60 );
		tMark = ( tMark + 100 * tNum * tNum / ( 50 + tTime ) ) * ( 1 + tLevel / 60 ) * self.coefficient ;

		AddIntData2AllPlayer( repetion_id, LG_PLAYER_DATA_KEY.MARK_POINT, tMark );

		SetIntData( repetion_id, LG_DATA_KEY.MARK_POINT, tMark );
		--杀怪总数
		EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 5, tNum );
		--时间
		EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 3, tTime );
		--等级
		EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 2, tLevel );
	end

	tMark = tMark + tKillFact;
	Debug( "AllMark Is " .. tMark );

	SetIntData( repetion_id,  LG_DATA_KEY.MARK_POINT_END, tMark );
	--总评分
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 1, tMark );
	--额外奖励
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 4, tKillFact );

end

function longgong_class:ogre_death( vRepetionID, vParame, vIndex )--, vEntityID, vTempID )
	Infor( "Call longgong ogre_death " .. vParame.index );

	table.foreachi( self.lg_ogre_temp, function( i, v )
		if vParame.tempid == v then
			tRtn = 0;
				
			--隐藏怪物不记数
			tRtn = 	table.foreachi( self.lg_ogre_machinery, function( k, m )
						tRtn = 0;
						tRtn =	table.foreachi( m.ogreindex, function( j, o )
									if o == vParame.index then
										return 1;
									end
								end);
						if tRtn ~= nil then
							return 2;
						end
					end);
			if tRtn ~= nil then
				return 3;
			end

			add_intdata( vRepetionID, LG_DATA_KEY.KILL_OGRE_NUM, 1 );	
			return 1;
		end
	end);

	if vParame.tempid == self.lg_kinglong_template then
		Infor( "kinglong die, create storebox" );
		CreateNpc( vRepetionID, self.lg_kingstorebox_index );

		TimeEnd( vRepetionID, 1 );
		TimeEnd( vRepetionID, 3 );
		TimeBegin( vRepetionID, 2 );	
	
		EventNoticeAll( vRepetionID, RepetionNoticeEventKind.KILL, vParame.entityid, vParame.tempid );
		EventNoticeAll( vRepetionID, RepetionNoticeEventKind.END, RepetionEnd.COMPLATE , 0 );
--		EventNoticeAll( vRepetionID, RepetionNoticeEventKind.CLOSE, self.lg_close_time, 0 ); 

		add_intdata( vRepetionID, LG_DATA_KEY.MARK_POINT, self.lg_kinglong_mark ); 

		tLevel = GetRepeLevel( vRepetionID );

		table.foreachi( lg_enco, function( i, v )
			if tLevel >= v.levelmin and tLevel <= v.levelmax then
				insert_item_all( vRepetionID, v.itemid, self.enco_count, RepetionInsertItemReason.WIN );
			end
		end);

		self:lg_end( vRepetionID, 0 );
		
		self:awardall( vRepetionID, vIndex );

--		return;
	end

	if vParame.index == self.lg_kinglong_fact.index then					--龙王真身死亡
		--清除计时器
		ClearAssignment( vRepetionID, vParame.killedid, GetIntData( vRepetionID, LG_DATA_KEY.KINGLONG_ASSIGNMENT ) );

		--处决倒计时
--		EventNoticeAll( vRepetionID, RepetionNoticeEventKind.CLOSE, self.lg_kinglong_fact_assignment2.endtime, 0 );
		TimeEnd( vRepetionID, 4 );
		TimeBegin( vRepetionID, 5 );

--		add_intdata( vRepetionID, LG_DATA_KEY.MARK_POINT, LG_DATA_KEY.MARK_POINT );
		self:lg_end( vRepetionID, 1 );

--		return;
	end

	table.foreachi( self.lg_block, function( i, v )								
		if v.index[ 1 ] == vParame.index then
			if v.timebegin ~= nil then
				TimeBegin( vRepetionID, v.timebegin );
--				EventNoticeAll( vRepetionID, RepetionNoticeEventKind.COUNT_DOWN, 1, self.lg_block_time );
			end
			if v.timeend ~= nil then
				TimeEnd( vRepetionID, v.timeend );
--				EventNoticeAll( vRepetionID, RepetionNoticeEventKind.COUNT_DOWN_END, 1, 0 );
			end
			if v.closeflag ~= nil then
				tEntity  = GetIntData( vRepetionID, v.closeflag );
				if tEntity ~= 0 then
					CharacterDie( vRepetionID, tEntity, 0 );
				end
			end
		end

		if v.ogreindex ~= nil then										--门杀怪记数
			table.foreachi( v.ogreindex, function( k, m )
				if vParame.index == m then
					tNum = 0;
					tNum = GetIntData( vRepetionID, v.num ) - 1;
					if tNum <= 0 then
						table.foreachi( v.flag, function( j, o )
							CharacterDie( vRepetionID, GetIntData( vRepetionID, o ), 1 );
						end);
					end
					SetIntData( vRepetionID, v.num, tNum );
					return;
				end
			end);
		end
	end);

	table.foreachi( self.death_create, function( i, v )
		if v.death_index == vParame.index then
			CreateNpc( vRepetionID, v.create_index );
		end
	end);

	table.foreachi( self.lg_conceal_ogre.ogre_list, function( i, v )
		if v == vParame.index then
			tCount = add_intdata( vRepetionID,  self.lg_conceal_ogre.count_key, -1 );
			if tCount == 0 then
				if self.lg_conceal_ogre.begin_time_index ~= nil then
					TimeBegin( vRepetionID, self.lg_conceal_ogre.begin_time_index );
				end
				if self.lg_conceal_ogre.end_time_index ~= nil then
					TimeEnd( vRepetionID, self.lg_conceal_ogre.end_time_index );
				end

				table.foreachi( self.lg_conceal_ogre.notice_event, function( k, m )
					EventNoticeAll( vRepetionID, m.key, m.parame1, m.parame2 );
				end);
			end

			return 1;
		end
	end);
end


function longgong_class:machinery_spring( vRepetionID, vParame )
	Debug( "Machinery Spring RepetionID " .. vRepetionID .. " index "  .. vParame.index );
	rand = math.random( 1, 10000 );
	table.foreachi( self.lg_ogre_machinery, function( i, v )
		if v.index == vParame.index then
			if v.rand ~= nil then
				Debug( "Rand " .. rand .. " index " .. i );
				if rand < v.rand.min or rand >= v.rand.max then
					return;
				end
			end
			table.foreachi( v.ogreindex, function( k, m )
				CreateNpc( vRepetionID, m );
			end);
			if v.entityflag ~= nil then
				SetIntData( vRepetionID, v.entityflag, 0 );
			end
			if v.destroyentity ~= nil then
				CharacterDie( vRepetionID, GetIntData( vRepetionID, v.destroyentity ), 0 );		
			end
			if v.begin_time_index ~= nil then
				TimeBegin( vRepetionID, v.begin_time_index );
			end
			if v.end_time_index ~= nil then
				TimeEnd( vRepetionID, v.end_time_index );
			end

			if v.notice_event ~= nil then
				table.foreachi( v.notice_event, function( k, m )
					EventNoticeAll( vRepetionID, m.key, m.parame1, m.parame2 );
				end);
			end
		end
	end);

	table.foreachi( self.lg_block, function( i, v )
		if v.machinery ~= nil then
			if vParame.index == v.machinery.index then
				table.foreachi( v.flag, function( k, m )
					CharacterDie( vRepetionID,  GetIntData( vRepetionID, m ), 0 );
				end);
			end
		end
	end);

	CharacterDie( vRepetionID, vParame.machineryid, 0 );
end

function longgong_class:time_assignment( vRepetionID, vParame )
	Debug( "Call Assignment, vParame index " .. vParame.index .. " tempid " .. vParame.tempid .. " flag " .. 
		vParame.flag );
	if vParame.index == self.lg_kinglong_fact.index and vParame.tempid == self.lg_kinglong_fact.tempid then
		if vParame.flag == self.lg_kinglong_fact_assignment1.other_info.flag then
			ChangePKType( vParame.entityid, 12480 );
			AllPlayerAddEnmitiy( vRepetionID, vParame.entityid, 1 );
			ClearAssignment( vRepetionID, vParame.entityid, vParame.timeid );
			UseSkill( vParame.entityid, vParame.entityid, self.lg_kinglong_fact.skilltemplate );
		end

		if vParame.flag == self.lg_kinglong_fact_assignment2.other_info.flag then
			ChangePKType( vParame.entityid, 3121 );
			ClearEnmity( vParame.entityid, 0 );
			ClearAssignment( vRepetionID, vParame.entityid, vParame.timeid );
--			EventNoticeAll( vRepetionID, RepetionNoticeEventKind.CLOSE, self.lg_kinglong_fact_assignment2.endtime, 0 );
--			ChangeTimeSpace( vRepetionID, 2, lg_kinglong_fact_assignment2.endtime );
			TimeEnd( vRepetionID, 4 );
			TimeBegin( vRepetionID, 5 );
			Teleport( vRepetionID, vParame.entityid, 24,29 );

			ChangePKType( GetIntData( vRepetionID, LG_DATA_KEY.BOY_ENTITYID ), 2 );
			ChangePKType( GetIntData( vRepetionID, LG_DATA_KEY.GIRL_ENTITYID ), 2 );
			AddEnmity( vParame.entityid, GetIntData( vRepetionID, LG_DATA_KEY.BOY_ENTITYID ), 1 );
			AddEnmity( vParame.entityid, GetIntData( vRepetionID, LG_DATA_KEY.GIRL_ENTITYID ), 1 );
		end
	end
end



function longgong_class:create_player( vRepetionID, vParame )
	tCount = GetBeginTime( vRepetionID );
	tCount = ( os.time( ) - tCount ) * 1000;

	if tCount < self.lg_complate_time then
--		EventNoticeSingle( vRepetionID, vParame.entityid, RepetionNoticeEventKind.CLOSE, 
--			self.lg_complate_time - tCount, 0 ); 
	end
end

function longgong_class:awardall( vRepetionID, vParame )
	AwardFameAll( vRepetionID, vParame );
end



