-- 黄蜂洞dataKey
local Hornets_Data_Key =
{
	HORNETS_FIREBRANDB_STATUS		= 0,		--火把B的状态
	HORNETS_FIREBRANDC_STATUS		= 1,		--火把C的状态
	HORNETS_WHITEHAIREPRINCES_STATUS	= 2,		--白毛公主计时器用
	HORNETS_IMMORTALHAIRSLAY_STATUS		= 3,		--仙毛居士计时器用
	HORNETS_WALLB_ONE			= 4,		-- 不可攻击的石墙B1
	HORNETS_WALLB_TWO			= 5,		-- 不可攻击的石墙B2
	HORNETS_COUTEDOGRE			= 6,		-- 需要计数的怪物
	HORNETS_BOX				= 7,		-- 宝箱数量
	HORNETS_YELLOORGESTATUS			= 8,		-- 黄风怪状态
	HORNETS_DOORNEARFIREGEARA		= 9,		-- 靠近火把A的大门
	HORNETS_DOORNEARYELLOORGE		= 10,		-- 靠近黄风怪的大门
}
-- 可以计数的怪物ID
local CountedOgreID = 
{
	55678, 55676, 55675, 55677, 55679 --, 55683, 55681, 55680, 55682 
}

local ScoreType = 
{
	HONETSHOLE_SCORE_REPTIONLEVEL	  = 1,		--副本等级
	HONETSHOLE_SCORE_OGRENUM	  = 2,		--怪物总数
	HONETSHOLE_SCORE_TOTALTIME	  = 3,		--通关时间
	HONETSHOLE_SCORE_GENERALSCORE	  = 4,		--评分
	HONETSHOLE_SCORE_EXTRALSCORE      = 5,		--额外评分
	HONETSHOLE_SCORE_TOTALSCORE	  = 6,		--总评分
}

-- 火把开关标志
local FLAGFIREON  = 1
local FLAGFIREOFF = 0

-- 可攻击的石墙A
local StoneDoorCanbeAttackedA = { 90136, 55935 }
local StoneDoorCanbeAttackedB = { 90150, 55936 }



local MAX_NPCNUM_SAMETEMPID = 10


-- 火把机关
local FireGear = 
{	A = { index = 90128, id   = 55944 },	-- 点火机关A 
	B = { index = 90129,id	  = 55945 },	-- 点火机关B --触发计时器
	C = { index = 90131,id    = 55946 },	-- 点火机关C --触发计时器
}
-- 未点燃的火把
local FireBrand = 
{
	A = { index = 90118 , id   = 55950, },	-- 火把A
	B = { index = 90123, id	  = 55951, },	-- 火把B
	C = { index = 90122, id	  = 55952, },	-- 火把C
}

-- 点燃的火把
local FireBrandOnFire = 
{	
	-- 火把A
	A = { 
		{ index = 90125,  id = 55947, },
		{ index = 90119,  id = 55947, },
	    },	

	-- 火把B
	B = {
		{ index = 90120, id = 55948  },
		{ index = 90127, id = 55948 },
	    },

	-- 火把C
	C = {
		{ index = 90121, id = 55949 },
		{ index = 90126, id = 55949 },
	    },
}

-- 火把触发的npc阻挡改变
local FireBrandBlock = 
{
	A = {					-- 火把A被点燃时改变的阻挡
		--{ id = 55939 },
		{ id = 52433 },
		{ id = 55944 },
		{ id = 55950 },
	    },

	B = {					-- 火把B被点燃时改变的阻挡
		{ id = 55945 },
		{ id = 55951 },
	    },

	C = {					-- 火把C被点燃时改变的阻挡
		{ id = 55946 },
		{ id = 55952 },
	    },

	D = {					-- 火把B和火把C同时点亮时改变的阻挡
		--{ id = 55939 },
		{ id = 55985 },
	    },
}

-- 怪物
local Ogre = 
{
	A = { index = 90094, id = 55682 },	-- 黄风怪
	B = { index = 90136, id = 55935 },	-- 可攻击石墙A , 开启计时器
	C = { index = 90060, id = 55681 },	-- 白毛公主  
	D = { index = 90150, id = 55936 },	-- 可以攻击的石墙B, 死亡时开启计时器
	E = { index = 90083, id = 55680 },	-- 仙毛居士
	F = { index = 90135, id = 55938 },	-- 不可攻击的石墙B1
	G = { index = 90148, id = 55938 },	-- 不可攻击的石墙B2
	H = { index = 90130, id = 55943 },	-- 宝箱A
	J = { index = 90132, id = 55943 },	-- 宝箱B
	K = { index = 90107, id = 55683 },	-- 黄风大王
	L = { index = 90133, id = 55939 },	-- 黄风洞石门1，点火机关1触发关闭的石门
	M = { index = 90155, id = 55939 },	-- 黄风洞石门2，黄风怪触发关闭的石门

}

-- 怪物死亡触发的npc阻挡改变
local OgreBlock =
{
	A = {					-- 黄蜂怪触发的阻挡改变，npc死亡
			{ id = 55937  },			
	     },

	B = {					-- 可攻击石墙A触发的阻挡改变
			{ id = 55983 },
	     },
	C = {					-- 白毛公主触发的阻挡改变，需要计时器条件检查
			{ id = 55998 },
	     },

	D = {					-- 可攻击的石墙B触发的阻挡改变
			{ id = 55984 },	
	    },

	E = {					-- 仙毛居士触发的阻挡改变,计时器检查
			{ id = 55938 },		
			{ id = 55999 },		
	     },	
}	

-- 怪物死亡触发的npc刷新
local OrgeRefresh = 
{
	A = {					-- 黄蜂怪触发的npc刷新
		{ index = 90136, id = 55935  },
		{ index = 90150, id = 55936  },
	    },

	B = {					-- 可攻击石墙A触发的npc刷新

	    },

	C = {					-- 白毛公主触发的npc刷新
		{ index = 90130, id = 55943  },
	    },

	D = {					-- 可攻击的石墙B触发的npc刷新
		
	    },

	E = {					-- 仙毛居士触发触发的npc刷新		
		{ index = 90132, id = 55943 },		
	    },	
}
-- 牌子相关
local TokenID = 45538
local TokenNum = 3

-- 时间设置

local RepetionCloseTime = 2700000;	-- 副本关闭时间 3600000 ( 45分钟 )

local FireBrandSustainTime = 120000;	-- 2分钟

local WhitehairPrinceTime = 60000;	-- 1分钟

local ImmortHairLayTime	  = 60000;	-- 1分钟

-- 副本关闭倒计时时间
REPETIONCLOSE_LEFTTIME = 120000	-- 2分钟

--复活
local function relive( map_id, repetion_id, args, loop_index )
	PlayerRelive( repetion_id, 1, args.x1, args.y1 );
end

-- 副本进入关闭的通知
local function RepetionDoorClose( )
		
end

-- 判断是否是需要计数的怪物
local function IsCountedOrge( index, id )
	local i = 1
	while CountedOgreID[i] ~= nil do
		if CountedOgreID[i] == id then
			return  1
		end
		i = i + 1
	end
	return 0
end

-- 创建火把
local function creat_firebrand( vRepetionID, vParame )
	local i = 1
	while vParame[i] ~= nil do
		CreateNpc( vRepetionID, vParame[i].index )
		i = i+1
	end
end

-- 改变阻挡
local function destroyblock( vRepetionID, vParame )
	local i = 1
	while vParame[i] ~= nil do
		DestroyAliveTempOgre( vRepetionID, vParame[i].id ,MAX_NPCNUM_SAMETEMPID  )
		i = i + 1
	end
end


-- 获得副本评分
local function NoticeRepetionscore( vReptionID )
	local Level = GetRepeLevel( vReptionID )
	local OgreNum = GetIntData( vReptionID, Hornets_Data_Key.HORNETS_COUTEDOGRE )
	local Time =  ( RepetionCloseTime - GetTime( vReptionID, 1 ) ) / 1000
	local BoxNum = GetIntData( vReptionID, Hornets_Data_Key.HORNETS_BOX )
	local BoxScore = (Level/45)*BoxNum*100
	local CommonScore = ( Level / 55 )*( ( OgreNum*OgreNum )/( 50 + Time )*100 + 200 )
	local TotalScore = BoxScore + CommonScore
	
	EventNoticeAll( vReptionID, RepetionNoticeEventKind.TAKE_COUNT,  ScoreType.HONETSHOLE_SCORE_REPTIONLEVEL,Level  )
	EventNoticeAll( vReptionID, RepetionNoticeEventKind.TAKE_COUNT,  ScoreType.HONETSHOLE_SCORE_OGRENUM,OgreNum  )
	EventNoticeAll( vReptionID, RepetionNoticeEventKind.TAKE_COUNT,  ScoreType.HONETSHOLE_SCORE_TOTALTIME,Time )
	EventNoticeAll( vReptionID, RepetionNoticeEventKind.TAKE_COUNT,  ScoreType.HONETSHOLE_SCORE_GENERALSCORE,CommonScore )
	EventNoticeAll( vReptionID, RepetionNoticeEventKind.TAKE_COUNT,  ScoreType.HONETSHOLE_SCORE_EXTRALSCORE,BoxScore )
	EventNoticeAll( vReptionID, RepetionNoticeEventKind.TAKE_COUNT,  ScoreType.HONETSHOLE_SCORE_TOTALSCORE,TotalScore )
	AddIntData2AllPlayer( vReptionID, 0, TotalScore )
end

-- 发送副本关闭倒计时通知
local function NoticeRepetionCloseTime( vRepetionID )

	local Time = GetTime( vRepetionID, 1 )
	if Time > REPETIONCLOSE_LEFTTIME then
		Time = REPETIONCLOSE_LEFTTIME
		TimeBegin( vRepetionID, 2 )
		--ChangeTime( vRepetionID, 1, Time )
	end
	EventNoticeAll( vRepetionID, RepetionNoticeEventKind.COUNT_DOWN,0, Time )
end

--创建NPC
local function create_ogre( vRepetionID, vParame )--, vParame1, vParame2 )


-- 添加点火机关观察事件	
	-- 火把机关A
	if FireGear.A.id == vParame.tempid  and FireGear.A.index == vParame.index then 
		ObserverMachinery( vRepetionID, vParame.entityid )
	end

	-- 火把机关B
	if FireGear.B.id == vParame.tempid  and FireGear.B.index == vParame.index then 
		ObserverMachinery( vRepetionID, vParame.entityid )
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_FIREBRANDB_STATUS, FLAGFIREOFF )
	end

	-- 火把机关C
	if FireGear.C.id == vParame.tempid  and FireGear.C.index == vParame.index then 
		ObserverMachinery( vRepetionID, vParame.entityid )
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_FIREBRANDC_STATUS, FLAGFIREOFF )
	end

--  怪物观察事件

	-- 黄风怪
	if vParame.tempid == Ogre.A.id and vParame.index == Ogre.A.index  then
		ObserverEntityDeath( vRepetionID, vParame.entityid )
	end

	-- 可攻击石墙A
	if vParame.tempid == Ogre.B.id and vParame.index == Ogre.B.index  then
		ObserverEntityDeath( vRepetionID, vParame.entityid )
	end

	-- 白毛公主
	if vParame.tempid == Ogre.C.id and vParame.index == Ogre.C.index  then
		ObserverEntityDeath( vRepetionID, vParame.entityid )
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_WHITEHAIREPRINCES_STATUS, 0 )
	end

	-- 可以攻击的石墙B
	if vParame.tempid == Ogre.D.id and vParame.index == Ogre.D.index  then
		ObserverEntityDeath( vRepetionID, vParame.entityid )
	end

	-- 仙毛居士
	if vParame.tempid == Ogre.E.id and vParame.index == Ogre.E.index  then
		ObserverEntityDeath( vRepetionID, vParame.entityid )
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_IMMORTALHAIRSLAY_STATUS, 0 )
	end

	-- 不可攻击的石墙B1
	if vParame.tempid == Ogre.F.id and vParame.index == Ogre.F.index  then	
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_WALLB_ONE, vParame.entityid )
	end

	-- 不可攻击的石墙B2		
	if vParame.tempid == Ogre.G.id and vParame.index == Ogre.G.index  then	
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_WALLB_TWO, vParame.entityid )
	end

	-- 是否是需要计数的怪物
	if IsCountedOrge(  vParame.index, vParame.tempid ) == 1 then
		ObserverEntityDeath( vRepetionID, vParame.entityid )
	end

	-- 宝箱
	if vParame.tempid == Ogre.H.id then	
		ObserverEntityDeath( vRepetionID, vParame.entityid )
	end

	-- 黄风大王
	if vParame.tempid == Ogre.K.id and vParame.index == Ogre.K.index  then	
		ObserverEntityDeath( vRepetionID, vParame.entityid )		
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_YELLOORGESTATUS, 0 )
	end
	
	-- 黄风洞石门1，点火机关1触发关闭的石门
	if vParame.tempid == Ogre.L.id and vParame.index == Ogre.L.index  then	
		ObserverEntityDeath( vRepetionID, vParame.entityid )		
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_DOORNEARFIREGEARA, vParame.entityid )
	end

	-- 黄风洞石门2，黄风怪触发关闭的石门
	if vParame.tempid == Ogre.M.id and vParame.index == Ogre.M.index  then	
		ObserverEntityDeath( vRepetionID, vParame.entityid )		
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_DOORNEARYELLOORGE, vParame.entityid )
	end
end

--Npc死亡
local function ogre_death( vRepetionID, vParame )--vEntityID, vTempID )

-- 黄蜂怪死亡事件
	if vParame.tempid == Ogre.A.id and vParame.index == Ogre.A.index then
		-- 消耗阻挡
		destroyblock( vRepetionID, OgreBlock.A )
		CharacterDie( vRepetionID, GetIntData( vRepetionID, Hornets_Data_Key.HORNETS_WALLB_ONE ), 0 )

		-- 刷新npc
		creat_firebrand( vRepetionID, OrgeRefresh.A )

		--计数
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_COUTEDOGRE,  GetIntData(  vRepetionID, Hornets_Data_Key.HORNETS_COUTEDOGRE ) + 1 )
	end

-- 可攻击的石墙A死亡事件
	if vParame.tempid == Ogre.B.id and vParame.index == Ogre.B.index then

		-- 消耗阻挡
		destroyblock( vRepetionID, OgreBlock.B )

		-- 设置状态
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_WHITEHAIREPRINCES_STATUS, 1  )

		-- 开启计时器
		TimeBegin( vRepetionID, 5 )
	end

-- 白毛公主死亡事件
	if vParame.tempid == Ogre.C.id and vParame.index == Ogre.C.index then

		-- 消耗阻挡
		local PrincesStatus = GetIntData( vRepetionID, Hornets_Data_Key.HORNETS_WHITEHAIREPRINCES_STATUS )
		if PrincesStatus == 1 then
			destroyblock( vRepetionID, OgreBlock.C )
			CharacterDie( vRepetionID, GetIntData( vRepetionID, Hornets_Data_Key.HORNETS_WALLB_TWO ), 0 )
			creat_firebrand( vRepetionID, OrgeRefresh.C )
		end

		-- 怪物计数
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_COUTEDOGRE,  GetIntData(  vRepetionID, Hornets_Data_Key.HORNETS_COUTEDOGRE ) + 1 )
	end

--  可以攻击的石墙B死亡事件
	if vParame.tempid == Ogre.D.id and vParame.index == Ogre.D.index then
		-- 消耗阻挡
		destroyblock( vRepetionID, OgreBlock.D )

		-- 设置状态
		SetIntData(  vRepetionID, Hornets_Data_Key.HORNETS_IMMORTALHAIRSLAY_STATUS, 1 )

		-- 开启计时器
		TimeBegin( vRepetionID, 6 )
	end

-- 仙毛居士死亡事件
	if vParame.tempid == Ogre.E.id and vParame.index == Ogre.E.index then

		-- 消耗阻挡
		local LayStatus =  GetIntData( vRepetionID, Hornets_Data_Key.HORNETS_IMMORTALHAIRSLAY_STATUS )

		if LayStatus == 1 then		 
			destroyblock( vRepetionID, OgreBlock.E )
			creat_firebrand( vRepetionID, OrgeRefresh.E )
		end
		
		-- 怪物计数
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_COUTEDOGRE,  GetIntData(  vRepetionID, Hornets_Data_Key.HORNETS_COUTEDOGRE ) + 1 )
	end

-- 是否是需要计数的怪物
	if IsCountedOrge(  vParame.index, vParame.tempid ) == 1 then		
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_COUTEDOGRE,  GetIntData(  vRepetionID, Hornets_Data_Key.HORNETS_COUTEDOGRE ) + 1 )
	end

-- 黄风大王
	if vParame.tempid == Ogre.K.id and vParame.index == Ogre.K.index  then			
		NoticeRepetionCloseTime( vRepetionID )
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_YELLOORGESTATUS, 1 )
		InsertItemAll( vRepetionID, TokenID, TokenNum  )

		-- 怪物计数
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_COUTEDOGRE,  GetIntData(  vRepetionID, Hornets_Data_Key.HORNETS_COUTEDOGRE ) + 1 )

		-- 发送评分
		NoticeRepetionscore( vRepetionID )

		AwardFameAll( vRepetionID, 4 );
	end

-- 宝箱数量
	if vParame.tempid == Ogre.H.id then
		local BoxNum = GetIntData( vRepetionID, Hornets_Data_Key.HORNETS_BOX ) + 1
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_BOX, BoxNum  )
	end
end

local function time_assignment( vRepetionID, vParame )

end

--宝箱事件
local function machinery_spring( vRepetionID, vParame )

-- 火把A的处理
	if vParame.index == FireGear.A.index and vParame.tempid == FireGear.A.id then

		-- 消耗未点燃的火把
		destroyblock( vRepetionID, FireBrandBlock.A )
		CharacterDie( vRepetionID,  GetIntData( vRepetionID, Hornets_Data_Key.HORNETS_DOORNEARFIREGEARA ), 0 )
		-- 创建点燃的火把
		creat_firebrand( vRepetionID, FireBrandOnFire.A )
	end

-- 火把B的处理
	if vParame.index == FireGear.B.index and vParame.tempid == FireGear.B.id then

		-- 消耗未点燃的火把
		destroyblock( vRepetionID, FireBrandBlock.B )

		-- 创建点燃的火把
		creat_firebrand( vRepetionID, FireBrandOnFire.B )

		-- 设置火把状态
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_FIREBRANDB_STATUS, FLAGFIREON  )

		local FirebrandCStatus = GetIntData( vRepetionID, Hornets_Data_Key.HORNETS_FIREBRANDC_STATUS )

		if FirebrandCStatus == FLAGFIREOFF then
		
			-- 创建计时器
			TimeBegin( vRepetionID, 3 )

			-- 通知火把B已经点燃
			EventNoticeAll( vRepetionID, RepetionNoticeEventKind.MSG, 1, FireBrandSustainTime ) 
		else
			
			-- 清除阻挡
			destroyblock( vRepetionID, FireBrandBlock.D )

			-- 清除大门
			CharacterDie( vRepetionID,  GetIntData( vRepetionID, Hornets_Data_Key.HORNETS_DOORNEARYELLOORGE ), 0 )

			-- 通知停止火把倒计时
			EventNoticeAll( vRepetionID, RepetionNoticeEventKind.MSG, 2, 0 ) 
		end
	end

-- 火把C的处理
	if vParame.index == FireGear.C.index and vParame.tempid == FireGear.C.id then

		-- 消耗未点燃的火把
		destroyblock( vRepetionID, FireBrandBlock.C )

		-- 创建点燃的火把
		creat_firebrand( vRepetionID, FireBrandOnFire.C )

		-- 设置火把状态
		SetIntData( vRepetionID, Hornets_Data_Key.HORNETS_FIREBRANDC_STATUS, FLAGFIREON  )

		local FirebrandBStatus = GetIntData( vRepetionID, Hornets_Data_Key.HORNETS_FIREBRANDB_STATUS )

		if FirebrandBStatus == FLAGFIREOFF then

			-- 创建计时器
			TimeBegin( vRepetionID, 4 )

			-- 通知火把C已经点燃		
			EventNoticeAll( vRepetionID, RepetionNoticeEventKind.MSG, 0, FireBrandSustainTime ) 

		else
			-- 清除阻挡
			destroyblock( vRepetionID, FireBrandBlock.D )

			-- 清除大门
			CharacterDie( vRepetionID,  GetIntData( vRepetionID, Hornets_Data_Key.HORNETS_DOORNEARYELLOORGE ), 0 )

			-- 通知停止火把倒计时
			EventNoticeAll( vRepetionID, RepetionNoticeEventKind.MSG, 2, 0 ) 
		end
	end
end

-- 副本关闭
local function  close_reption( map_id, repetion_id, args, index )
	--local yellorgestatus = GetIntData( repetion_id, Hornets_Data_Key.HORNETS_YELLOORGESTATUS )
	--if yellorgestatus == 1 then
		--NoticeRepetionscore( repetion_id )
	--end

	RepetionClose( repetion_id )
end

-- 创建角色
local function create_player( vRepetionID, vParame )
	
end

-- 火把B的计时时间到
local function Timeout_FirebrandB( map_id, repetion_id, args, index )

	local FireBrandCStatus = GetIntData( repetion_id, Hornets_Data_Key.HORNETS_FIREBRANDC_STATUS )

	-- 如果火把C已经灭了,则刷新火把
	if  FireBrandCStatus ~= FLAGFIREON then
		SetIntData( repetion_id, Hornets_Data_Key.HORNETS_FIREBRANDB_STATUS, FLAGFIREOFF ) -- 设置状态
		destroyblock( repetion_id, FireBrandOnFire.B ) --消耗火把
		CreateNpc( repetion_id, FireGear.B.index )
		CreateNpc( repetion_id, FireBrand.B.index )			  -- 创建点火机关和未点燃火把
	end
end

-- 火把C的计时时间到
local function Timeout_FirebrandC( map_id, repetion_id, args, index )

	local FireBrandBStatus = GetIntData( repetion_id, Hornets_Data_Key.HORNETS_FIREBRANDB_STATUS )

	-- 如果火把已经灭了,则重新刷新火把
	if  FireBrandBStatus ~= FLAGFIREON then	
		SetIntData( repetion_id, Hornets_Data_Key.HORNETS_FIREBRANDC_STATUS, FLAGFIREOFF ) -- 设置状态

		destroyblock( repetion_id, FireBrandOnFire.C )			  -- 清除阻挡

		CreateNpc( repetion_id, FireGear.C.index )			  -- 创建点火机关和未点燃火把
		CreateNpc( repetion_id, FireBrand.C.index )			  
	end
end

-- 白毛公主的计时器到时间
local function Timeout_WhitehairPrince( map_id, repetion_id, args, index )

	SetIntData( repetion_id, Hornets_Data_Key.HORNETS_WHITEHAIREPRINCES_STATUS, 0  )

end

-- 仙毛居士的计时器到时
local function Timeout_ImmortalHairsLay( map_id, repetion_id, args, index )
	SetIntData( repetion_id, Hornets_Data_Key.HORNETS_IMMORTALHAIRSLAY_STATUS, 0  )
end


-- 副本配置如下
repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.HORNETSHOLE,
		playercountmax = 6,		-- 副本最大人数
		mapid = { 91 },			-- 副本地图ID
		taskid = 0,			-- 进入副本的前置任务
		posx = 9,			-- 暂时不用
		posy = 277,			-- 暂时不用
		pernumber = 1,			-- 每天进入副本的次数； 0-不限
		minlevel = 40,			-- 最低进入等级
		maxlevel = 120,			-- 
		--check_time = 20000,		-- 检查副本人数时间(毫秒)
                weekpernumber = 4,
		question_check=0,		-- 是否进行副本答题验证
		relogin =	1,					-- 1表示可重入0表示不可重入
		closetime   = 120,	  -- 没有人时多久后关闭
		special = 
		{
			{ item = 63856, flag = 3 },
		},
		-- 组队信息
		team = 
		{
			teamin = true,		-- 是否组队进入； ture-需要组队; false - 不需组队
			count = 3,		-- 组队人数下限
		},
		

		backpos = 
		{
			mapid = 1,			-- 暂时不用
			posx = 33,			-- 暂时不用	
			posy = 13,			-- 暂时不用
		},

		-- 副本开放时间
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
	-- 定时器
	timer = 
	{ 
		-- 副本开始时定时器开始
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = RepetionCloseTime,			
			loop = 1,
			func = close_reption,
			args = { },
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 },
		},
		

		-- 副本关闭计时器
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = REPETIONCLOSE_LEFTTIME,
			loop = 1,
			func = close_reption,
			args = { },

		},

		-- 火把B计时器
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = FireBrandSustainTime,
			loop = 1,
			func = Timeout_FirebrandB,
			args = { }
		},
		
		-- 火把C计时器
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = FireBrandSustainTime,
			loop = 1,
			func = Timeout_FirebrandC,
			args = { }
		},
		

		-- 白毛公主的计时器
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = FireBrandSustainTime,
			loop = 1,
			func = Timeout_WhitehairPrince,
			args = { }
		},

		-- 仙毛居士的计时器
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = FireBrandSustainTime,
			loop = 1,
			func = Timeout_ImmortalHairsLay,
			args = { }
		},
	},

	event = 
	{
		[ RepetionEventKind.PLAYER_CREATE ] = { func = create_player },
		[ RepetionEventKind.OGRE_CREATE ] = { func = create_ogre },
		[ RepetionEventKind.OGRE_DEATH	] = { func = ogre_death },
		[ RepetionEventKind.TIME_ASSIGNMENT ] = { func = time_assignment },
		[ RepetionEventKind.BOX_MACHINERY_SPING ] = { func = machinery_spring },
	},
}



 
