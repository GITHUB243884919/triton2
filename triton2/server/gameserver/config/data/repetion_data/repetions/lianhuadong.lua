-- BOSS数量：(不含金角银角)
local boss_num = 5;
-- INT DATA使用规范：
-- 1-boss_num : 小妖的实体ID
-- 6-boss_num*2：人质的实体ID
-- 其他的如下：
local INT_DATA_KEY = 
{
	PIG_KING_ENTITYID = boss_num*2+1,	-- 猪大王实体ID
	JINJIAO_ENTITYID = boss_num*2+2,	-- 金角实体ID
	YINJIAO_ENTITYID = boss_num*2+3,	-- 银角实体ID
	JINJIAO_DIE_FLAG = boss_num*2+4,	-- 金角死亡标志 1=死亡
	YINJIAO_DIE_FLAG = boss_num*2+5,	-- 银角死亡标志 1=死亡
	SMALL_BOSS_DIE_NUM = boss_num*2+6,	-- 小BOSS 的死亡数量 (M)
	OGRE_DIE_NUM = boss_num*2+7,	-- 普通怪物死亡数量 (N)
	KILLJINJIAOYINJIAO_TIME = boss_num*2+8,	-- 从进入副本到杀死金角银角的时间  (T)
	MONSTER_DIE_NUM = boss_num*2+9,	-- 击杀小妖的数量  (P)
	SPECIAL_OGRE_DIE_NUM = boss_num*2+10,	-- 击杀特殊怪物的数量  (K)
};

local fb_close_time = 60 * 60 * 1000;				-- 关闭时间
local kill_time = 20 * 60 * 1000;					-- 击杀倒计时时间
local pawn_disapear_time = 5*1000;				-- 人质消失时间
local boss_add_kill_time = 5*60*1000;				-- 每次击杀BOSS 增加的剩余时间
local end_close_time = 2*60*1000;				-- 杀死金角银角以后副本重置的关闭时间
local boss_relive_time = 2*1000;					-- 金角银角复活时间
local boss_relive_shout_time = 8*1000;				-- 金角银角复活复活前废话时间(死亡到废话开始时间)

-- 莲花洞副本 评价数值类型枚举
local enLianhuadongMarkType = 
{
	MARKTYPE_FB_LEVEL	=	1,	-- 副本等级
	MARKTYPE_SAVE_NUM=2,		-- 解救人质数
	MARKTYPE_KILLNUM=3,		-- 杀怪总数
	MARKTYPE_TIME=4,			-- 通关时间（秒）
	MARKTYPE_MARK=5,			-- 评分
	MARKTYPE_EXTRA_MARK=6,	-- 额外评分
};

-- BOSS 索引列表
local boss_indexs = {91031, 91060, 91071, 91083, 91119};
-- 人质的索引ID列表
local pawn_indexs = {91033, 91068, 91144, 91141, 91122};
-- 人质的模板ID列表
local pawn_tempids = { {55962, 55963, 55964 },
				  {55965, 55966, 55967 },
				  {55974},
				  {55975},
				  {55968, 55969, 55970, 55971, 55972, 55973},
				};
				
-- 需要计数的小怪的模板ID
local ogre_tempids = {55628, 55629, 55630, 55631, 55632, 55633, 55634, 55635, 55636, 55637 };
				
-- 需要计数的特殊怪物的索引ID
local special_ogre_indexs = { 91123, 91137 };

-- 押解小妖的索引列表
local monster_indexs = {91138, 91139, 91140, 91142, 91143};
-- 对应刷出宝箱的索引列表
local box_indexs = {91147, 91152, 91151, 91150, 91149};

-- 人质给自己上BUFF的skill_id
local pawn_skill_id = 56066;

-- 人质去掉BUFF的skill_id
local pawn_removebuff_skill_id = 56158;

-- 小妖杀人质用的技能ID
local kill_pawn_skillid = 56157;

-- 狐狸索引
local fox_index = 91137;

-- 狐狸路径
local fox_router = 
{
	{ x = 151, y = 62 },
	{ x = 151, y = 74 },
	{ x = 155, y = 79 },
	{ x = 155, y = 171 },
	{ x = 146, y = 180 },
	{ x = 114, y = 180 },
	{ x = 106, y = 168 },
	{ x = 99, y = 158 },
	{ x = 99, y = 136 },
	{ x = 87, y = 136 },
};

-- 野猪大王和猪大王
local pigking_index = 91123;
local yepigking_index = 91183;

-- 金角银角数据
local jinjiao_index=91184;
local yinjiao_index = 91128;

-- 金角银角复活废话定时器索引
local jinjiao_relife_shout_timer_index = boss_num+2+1;
local yinjiao_relife_shout_timer_index = boss_num+2+2;

local jinjiao_relife_timer_index = boss_num+2+3;
local yinjiao_relife_timer_index = boss_num+2+4;

-- 最终的宝箱
local final_box_index = 91148;

-- 牌子ID
local paizi_id = 45542;
local paizi_num = 3;
---------------------------------------------------------------------------------------------------------------------------------------------
-- 私有API
--------------------------------------------
-- 检查是否人质、BOSS等 是的话返回下标
local function get_charactor_arr_index( tab, value )
	if tab == nil or value == nil then
		return 0;
	end
	
	for i=1, table.getn(tab) do
		if tab[i] == value then
			return i;
		end
	end
	return 0;
end

-- 增加副本剩余击杀时间
local function add_kill_time(repetion_id)
	local left_time = GetTime(repetion_id,1);
	if left_time > 0  then 
		local new_time = left_time + boss_add_kill_time;
		Debug("LIANHUADONG-change timer1 to "..new_time);
		ChangeTime( repetion_id, 1, new_time );
	end
end

-- 变成可攻击状态
local function change_to_attackable( entity_id )
Debug( "LIANHUADONG-change_to_attackable entity=".. entity_id );
	ChangePKType( entity_id, 12481 );
end

-- 计算积分
local function dispos_mark(repetion_id)
	local fb_level = GetRepeLevel(repetion_id);
	local pass_time = GetIntData(repetion_id, INT_DATA_KEY.KILLJINJIAOYINJIAO_TIME);
	local kill_small_boss = GetIntData(repetion_id, INT_DATA_KEY.SMALL_BOSS_DIE_NUM);
	local kill_ogre_num = GetIntData(repetion_id, INT_DATA_KEY.OGRE_DIE_NUM)+kill_small_boss; 
	local kill_monster = GetIntData(repetion_id, INT_DATA_KEY.MONSTER_DIE_NUM);
	
	Debug("level="..fb_level.." kill_ogre_num="..kill_ogre_num.." pass_time="..pass_time.." kill_small_boss="..kill_small_boss.." kill_monster="..kill_monster);
	
	local mark = (1/2)*((fb_level/75)^2)*((kill_ogre_num^2/( pass_time/60+45))+kill_small_boss*40+kill_monster*50+100);
	local extra_mark = (fb_level/55)*GetIntData(repetion_id, INT_DATA_KEY.SPECIAL_OGRE_DIE_NUM)*100;
	
	-- 计算积分 发给客户端 加到玩家身上
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, enLianhuadongMarkType.MARKTYPE_FB_LEVEL, fb_level );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, enLianhuadongMarkType.MARKTYPE_SAVE_NUM, kill_monster );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, enLianhuadongMarkType.MARKTYPE_KILLNUM, kill_ogre_num );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, enLianhuadongMarkType.MARKTYPE_TIME, pass_time ); -- 秒！！
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, enLianhuadongMarkType.MARKTYPE_MARK, mark+extra_mark );
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, enLianhuadongMarkType.MARKTYPE_EXTRA_MARK, extra_mark );

	AddIntData2AllPlayer( repetion_id, 0, mark+extra_mark );
end


-- 副本完成 触发
local function on_fb_end(repetion_id)
	Debug( "LIANHUADONG-on_fb_end" );	
	-- 先清除金角银角复活定时器 不让他们复活了
	TimeEnd( repetion_id, jinjiao_relife_shout_timer_index );
	TimeEnd( repetion_id, yinjiao_relife_shout_timer_index );
	TimeEnd( repetion_id, jinjiao_relife_timer_index );
	TimeEnd( repetion_id, yinjiao_relife_timer_index );
	
	-- 刷宝箱
	CreateNpc( repetion_id, final_box_index );
	

	local level = GetRepeLevel( repetion_id );
	-- 奖励牌子
	insert_item_all( repetion_id, paizi_id, paizi_num, RepetionInsertItemReason.WIN );
	if level >= 80 and level <= 110 then	
		insert_item_all( repetion_id, 66884, 2, RepetionInsertItemReason.WIN );
	end

	AwardFameAll( repetion_id, 3 );
	
	-- 记录完成时间
	local end_time = fb_close_time - GetTime( repetion_id, 2 );
	SetIntData( repetion_id, INT_DATA_KEY.KILLJINJIAOYINJIAO_TIME, end_time/1000 );
	
	-- 重置时间
	ChangeTime( repetion_id, 2, end_close_time );

	-- 处理积分
	dispos_mark(repetion_id);
end

-- 杀死所有人质
local function kill_all_pawns(repetion_id)
	for i = 1,  boss_num do
		-- 杀人质
		UseSkill( GetIntData( repetion_id, i ), GetIntData( repetion_id, boss_num+i ),  kill_pawn_skillid );	
	end
end
---------------------------------------------------------------------------------------------------------------------------------------------
--  事件触发函数
local function create_player( vRepetionID, vParame )
	--UseSkill( vParame.entityid, vParame.entityid, 50055 );
	--TimeBegin( vRepetionID, 1 );
end

-- 创建怪物
local function create_ogre( vRepetionID, vParame )
	Debug( "LIANHUADONG-create_ogre index=".. vParame.index.." tempid="..(vParame.tempid or 0 ) );

	-- 做怪物/NPC的初始化操作
	-- 如果是小妖
	local 	tIndex = get_charactor_arr_index( monster_indexs, vParame.index );
	if tIndex>0 and tIndex <= boss_num then
		-- 记录实体ID
		SetIntData( vRepetionID, tIndex, vParame.entityid );
		-- 注册
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		return;
	end
	
	-- 如果是小BOSS
	tIndex = get_charactor_arr_index( boss_indexs, vParame.index );
	if tIndex>0 and tIndex <= boss_num  then
		-- 注册
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		return;
	end
	
	-- 如果是小怪 注册
	tIndex = get_charactor_arr_index( ogre_tempids, vParame.tempid );
	if tIndex>0  then
		-- 注册
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		return;
	end
	
	-- 如果是特殊怪 注册
	tIndex = get_charactor_arr_index( special_ogre_indexs, vParame.index );
	if tIndex>0  then
		-- 注册
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		--return;
	end
	
	-- 如果是人质
	tIndex = get_charactor_arr_index( pawn_indexs, vParame.index );
	if tIndex>0 and tIndex <= boss_num  then
		-- 记录实体ID
		SetIntData( vRepetionID, boss_num+tIndex, vParame.entityid );
		-- 人质给自己上BUFF
		UseSkill( vParame.entityid, vParame.entityid, pawn_skill_id );
		return;
	end	
	
	-- 如果是狐狸 走路
	if  vParame.index == fox_index then
		Debug( "LIANHUADONG-fox start go! index=".. vParame.index );
		NpcPatrol( vRepetionID, vParame.entityid, 1, fox_router );
		return;
	end
	
	-- 如果是野猪大王 
	if vParame.index == yepigking_index then
		-- 注册
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		return;
	end
	
	-- 如果是猪大王 记录实体ID
	if vParame.index == pigking_index then
		Debug("LIANHUADONG-PigReg index="..pigking_index.." entityid="..vParame.entityid);
		SetIntData( vRepetionID, INT_DATA_KEY.PIG_KING_ENTITYID, vParame.entityid );
		return;
	end
	
	-- 如果是金角大王 记录实体ID
	if vParame.index == jinjiao_index then
		SetIntData( vRepetionID, INT_DATA_KEY.JINJIAO_ENTITYID, vParame.entityid );
		SetIntData( vRepetionID, INT_DATA_KEY.JINJIAO_DIE_FLAG, 0 );
		-- 注册
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		--  金角喊“为你而战，我的二弟”
		EventNoticeAll( vRepetionID, RepetionNoticeEventKind.MSG, vParame.entityid, 2 );

		return;
	end
	
	-- 如果是银角大王 记录实体ID
	if vParame.index == yinjiao_index then
		SetIntData( vRepetionID, INT_DATA_KEY.YINJIAO_ENTITYID, vParame.entityid );
		SetIntData( vRepetionID, INT_DATA_KEY.YINJIAO_DIE_FLAG, 0 );
		-- 注册
		ObserverEntityDeath( vRepetionID, vParame.entityid );
		--  银角喊“为你而战，我的大哥”
		EventNoticeAll( vRepetionID, RepetionNoticeEventKind.MSG, vParame.entityid, 2 );

		return;
	end
end 

-- 怪物死了
local function entity_death( vRepetionID, vParame )
Debug( "LIANHUADONG-entity_death index=".. vParame.index.." tempid="..(vParame.tempid or 0 ) );
	-- 如果是BOSS死了 
	local tIndex = get_charactor_arr_index( boss_indexs, vParame.index );
	if tIndex>0 and tIndex <= boss_num  then
		-- 增加时间
		add_kill_time(vRepetionID);
		-- 对应小妖变成可攻击状态
		change_to_attackable( GetIntData( vRepetionID, tIndex ) );
		-- 如果是第五个BOSS 则刷狐狸出来 
		if  tIndex == 5 then
			CreateNpc( vRepetionID, fox_index );
		end
		-- 设置已经死亡的小BOSS的数量
		local tNowNum = GetIntData( vRepetionID, INT_DATA_KEY.SMALL_BOSS_DIE_NUM )+1;
		SetIntData( vRepetionID, INT_DATA_KEY.SMALL_BOSS_DIE_NUM, tNowNum );
		Debug("LIANHUADONG-NOWNUM="..tNowNum.." bossnum="..boss_num);
		return;
	end
	
	-- 如果是小妖死了
	tIndex = get_charactor_arr_index( monster_indexs, vParame.index );
	if tIndex>0 and tIndex <= boss_num  then
		if GetTime(vRepetionID,1) > 0 then -- 剩余击杀时间 才算数
			--  对应的人质去掉捆绑BUFF
			UseSkill( GetIntData( vRepetionID, boss_num+tIndex ), GetIntData( vRepetionID, boss_num+tIndex ),  pawn_removebuff_skill_id );
			--  人质说话
			EventNoticeAll( vRepetionID, RepetionNoticeEventKind.MSG, GetIntData(vRepetionID, boss_num+tIndex), 1 );
			-- 开启人质消失定时器
			TimeBegin( vRepetionID, tIndex+2 );
			-- 计数
			local tNowNum = GetIntData( vRepetionID, INT_DATA_KEY.MONSTER_DIE_NUM )+1;
			SetIntData( vRepetionID, INT_DATA_KEY.MONSTER_DIE_NUM, tNowNum );
			if tNowNum == boss_num then -- 小BOSS全杀完了
				TimeEnd(vRepetionID, 1);
			end
		else -- 算杀了个小怪
			local tNowNum = GetIntData( vRepetionID, INT_DATA_KEY.OGRE_DIE_NUM )+1;
			SetIntData( vRepetionID, INT_DATA_KEY.OGRE_DIE_NUM, tNowNum );
		end

		return;
	end
	
	-- 野猪大王死了
	if vParame.index == yepigking_index then
		-- 把猪大王变成可攻击的
		change_to_attackable(GetIntData( vRepetionID, INT_DATA_KEY.PIG_KING_ENTITYID));
		return;
	end
	
	-- 金角死了
	if vParame.index == jinjiao_index then
		-- 设置死亡标记
		SetIntData( vRepetionID, INT_DATA_KEY.JINJIAO_DIE_FLAG, 1 );
		
		--  检查银角是不是也死了 如果false 则加入复活定时器
		if GetIntData( vRepetionID, INT_DATA_KEY.YINJIAO_DIE_FLAG ) == 1 then -- 银角也死了
			on_fb_end(vRepetionID);
		else
			TimeBegin( vRepetionID, jinjiao_relife_shout_timer_index );
		end
		return;
	end
	
	-- 银角死了
	if vParame.index == yinjiao_index then
		-- 设置死亡标记
		SetIntData( vRepetionID, INT_DATA_KEY.YINJIAO_DIE_FLAG, 1 );
		
		--  检查银角是不是也死了 如果false 则加入复活定时器
		if GetIntData( vRepetionID, INT_DATA_KEY.JINJIAO_DIE_FLAG ) == 1 then -- 金角也死了
			on_fb_end(vRepetionID);
		else
			TimeBegin( vRepetionID, yinjiao_relife_shout_timer_index );
		end
		return;
	end
	
	-- 小怪死了
	tIndex = get_charactor_arr_index( ogre_tempids, vParame.tempid );
	if tIndex>0 and tIndex <= table.getn(ogre_tempids)  then
		local tNowNum = GetIntData( vRepetionID, INT_DATA_KEY.OGRE_DIE_NUM )+1;
		SetIntData( vRepetionID, INT_DATA_KEY.OGRE_DIE_NUM, tNowNum );
		return;
	end
	
	-- 特殊怪物死了
	tIndex = get_charactor_arr_index( special_ogre_indexs, vParame.index );
	if tIndex>0 and tIndex <= table.getn(special_ogre_indexs)  then
		local tNowNum = GetIntData( vRepetionID, INT_DATA_KEY.SPECIAL_OGRE_DIE_NUM )+1;
		SetIntData( vRepetionID, INT_DATA_KEY.SPECIAL_OGRE_DIE_NUM, tNowNum );
		return;
	end
end

-- 击杀BOSS倒计时到
local function on_kill_boss_timeout( repetion_index, repetion_id, args, loop_index )
	-- 所有看守小妖杀死人质
	kill_all_pawns(repetion_id);
end

-- 人质消失 创建宝箱
local function on_pawn_disapear( repetion_index, repetion_id, args, loop_index )
Debug( "LIANHUADONG-on_pawn_disapear index=".. args.pawn_index );
	if args.pawn_index == nil or args.pawn_index <= 0 or args.pawn_index > table.getn(pawn_tempids) then
		Error("pawn index error!");
		return;
	end
	
	-- 人质消失
	for i = 1, table.getn(pawn_tempids[args.pawn_index])  do
		DestroyAliveTempOgre(repetion_id, pawn_tempids[args.pawn_index][i], 1 );
	end
	
	-- 创建宝箱	
	CreateNpc( repetion_id, box_indexs[args.pawn_index] );
end

-- 金角银角复活
local function on_jinjiao_relive( repetion_index, repetion_id, args, loop_index )
	SetIntData( repetion_id, INT_DATA_KEY.JINJIAO_DIE_FLAG, 0 );
	-- 复活金角
	CreateNpc( repetion_id, jinjiao_index );
end

local function on_yinjiao_relive( repetion_index, repetion_id, args, loop_index )
	SetIntData( repetion_id, INT_DATA_KEY.YINJIAO_DIE_FLAG, 0 );
	-- 复活银角
	CreateNpc( repetion_id, yinjiao_index );
end

-- 金角银角复活前废话
local function on_jinjiao_relive_shout( repetion_index, repetion_id, args, loop_index )
	-- 金角即将复活 银角先废话
	--  银角喊“复活吧，我的大哥”
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.MSG, GetIntData(repetion_id, INT_DATA_KEY.YINJIAO_ENTITYID), 1 );
	TimeBegin( repetion_id, jinjiao_relife_timer_index );
end

local function on_yinjiao_relive_shout( repetion_index, repetion_id, args, loop_index )
	-- 银角即将复活 金角先废话
	--  金角喊“复活吧，我的二弟”
	EventNoticeAll( repetion_id, RepetionNoticeEventKind.MSG, GetIntData(repetion_id, INT_DATA_KEY.JINJIAO_ENTITYID), 1 );
	TimeBegin( repetion_id, yinjiao_relife_timer_index );
end

-- 副本关闭
local function on_fb_close( repetion_index, repetion_id, args, loop_index )
	-- 处理积分
	dispos_mark(repetion_id);
	
	-- 关闭！
	RepetionClose(repetion_id);
end


---------------------------------------------------------------------------------------------------------------------------------------------
repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.LIANHUADONG,
		playercountmax = 6,		-- 
		mapid = { 92 },
		taskid = 0,			-- into task condition 
		posx = 24,
		posy = 308,
	--	teamin = true,
		pernumber = 2,			-- everday into max number 
		minlevel = 60,
		maxlevel = 120,
		weekpernumber = 7,
		--check_time = 20000,  -- 服务器最少人数检查时间，毫秒
		relogin =	1,					-- 1表示可重入0表示不可重入
		closetime   = 120,	  -- 没有人时多久后关闭
		team = 
		{
			teamin = true,		
			count = 3,			-- team redirect player number less
		},
		
		backpos = 
		{
			mapid = 67,
			posx = 99,
			posy = 20
		},
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
		-- 击杀BOSS倒计时
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = kill_time,
			loop = 1,
			func = on_kill_boss_timeout,
			args = { },
			sync = { kind = RepetionTimeSyncKind.ALL, key = 1 }
		},
		-- 副本关闭倒计时
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = fb_close_time,
			loop = 1,
			func = on_fb_close,
			args = { },
			sync = { kind = RepetionTimeSyncKind.ALL, key = 0 }
		},
		-- 人质解绑到消失计时处理 (index=1~5)
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pawn_disapear_time,
			loop = 1,
			func = on_pawn_disapear,
			args = { pawn_index = 1 },
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pawn_disapear_time,
			loop = 1,
			func = on_pawn_disapear,
			args = { pawn_index = 2 },
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pawn_disapear_time,
			loop = 1,
			func = on_pawn_disapear,
			args = { pawn_index = 3 },
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pawn_disapear_time,
			loop = 1,
			func = on_pawn_disapear,
			args = { pawn_index = 4 },
		},
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = pawn_disapear_time,
			loop = 1,
			func = on_pawn_disapear,
			args = { pawn_index = 5 },
		},
		-- 金角复活废话
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = boss_relive_shout_time,
			loop = 1,
			func = on_jinjiao_relive_shout,
			args = { },
		},
		-- 银角复活废话
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = boss_relive_shout_time,
			loop = 1,
			func = on_yinjiao_relive_shout,
			args = { },
		},
		-- 金角复活
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = boss_relive_time,
			loop = 1,
			func = on_jinjiao_relive,
			args = { },
		},
		-- 银角复活
		{
			kind = RepetionTimeBeginKind.EVENT_BEGIN,
			result = RepetionTimeResultKind.NONE,
			time = boss_relive_time,
			loop = 1,
			func = on_yinjiao_relive,
			args = { },
		},
	},
	event = 
	{
		[ RepetionEventKind.PLAYER_CREATE ] = { func = create_player },
		[ RepetionEventKind.OGRE_CREATE ] = { func = create_ogre },		
		[ RepetionEventKind.OGRE_DEATH ] = { func = entity_death },
	},
}


