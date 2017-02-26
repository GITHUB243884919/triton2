--accept_functor
--{
  --{check_money,  "检查玩家的金钱在给定的范围内", {min_money, max_money}},
  --{check_level,  "检查玩家的级别在给定的范围内", {min_level, max_level}},
  --{check_sex,    "检查玩家的性别为给定的值",     {sex}},
  --{check_meitre, "检查玩家的职业为给定的值",     {meitre}},
  --{check_region, "检查玩家的坐标在给定的区域内", {map, x, y, w, h}},
  --{check_bagItem,  "检查玩家的物品表t是否在玩家包裹中且数量满足", {id, n}}
--};

--awards_functor
--{
  --{function_name, function_note, {name, level}},
--};

--这类参数都是两个table作为参数的
--complete_functor
--{
  --{check_taskItem,  "检查玩家完成的数量t2是否满足要求的数量t1", t1 = {{id, n}}, t2 = {{ id, n}}};
--};

--------------------- function 的实现, 成功则返回true, 失败则返回false -----------

function check_level(t) 
  local mini = t.min_level  or 0
  local maxi = t.max_level  or 10000
  if maxi == 0 then maxi = 10000; end;
    
  local v = GetPlayer(EPlayerAttr.PLAYER_LEVEL);

  Debug("check_level " .. v .. " in (" .. mini .. ", " .. maxi .. ");")
  return v >= mini and v <= maxi
end


function check_money(t) 
  local mini = t.min_money  or 0;
  local maxi = t.max_money  or 0x1fffffff;
  if maxi == 0 then maxi = 0x1fffffff; end;
  
  local v = GetPlayer(EPlayerAttr.PLAYER_MONEY);


  Debug("check_money " .. v .. " in (" .. mini .. ", " .. maxi .. ");")
  return v >= mini and v <= maxi
end

function check_sex(t) 
  local sex = GetPlayer(EPlayerAttr.PLAYER_SEX)

  Debug("check_sex " .. sex .. " in (" .. t.sex .. ");")

  return  sex == t.sex
end

function check_metier(t) 
  local metier = GetPlayer(EPlayerAttr.PLAYER_METIER);

  Debug("check_metier " .. metier .. " in (" .. t.metier .. ");")

  return  metier == t.metier
end

function check_not_metier(t) 
  local metier = GetPlayer(EPlayerAttr.PLAYER_METIER);

  Debug("check_not_metier " .. metier .. " in (" .. t.metier .. ");")

  return  metier ~= t.metier
end

function check_region(t)
  local region = GetPlayerRegion()
  return (region.mMapID == t.map) and  (region.mX >= t.x) and region.mX <= (t.x + t.w) and region.mY >= t.y and  region.mY <= (t.y + t.h)
end

function check_items_of_bag(t) 
  if t and t.id and t.n then
    Debug("玩家收集了物品(id="..t.id..") " .. PlayerHasItem(t.id) .. "/" .. (t.n or 0));
    return PlayerHasItem(t.id) >= (t.n or 0)
  end

  return false;
end

function check_npc_owner(t)
	return CheckNpcOwner();
end

-- 检查包里没有道具
function check_items_not_in_bag(t) 
  if t and t.id and t.n then
    return PlayerHasItem(t.id) < (t.n or 0)
  end

  return false;
end


function check_task_got(t)
	if t.task_id == nil then t.task_id = 0; end;
	local state = get_task_state(t.task_id);
	if state == 0 or state == 1 then
	    return true;
	else
	    return false;
	end

end


function check_orges_by_killed(t, collector)
  if collector and t and t.id and t.n then
    Debug("玩家杀死了 (id="..t.id..")" .. collector:get_pair(t.id) .. "/" .. (t.n or 0));
    return collector:get_pair(t.id)  >= (t.n or 0)
  else
    Debug("collector is nil");
  end

  return false;
end

-- 检查并列杀怪
function check_many_orges_by_killed(t, collector)
  if collector then
    Debug("玩家杀死了 (并列, id="..t.id1..")" .. collector:get_pair(t.id1) .. "/" .. (t.n or 0));
    return collector:get_pair(t.id1)  >= (t.n or 0)
  else
    Debug("collector is nil");
  end

  return false;
end

--  检查前置任务是否已经完成
function  check_before_task(t)
--Debug("取得前置任务(" .. t.task_id )
--~ 	local pre_state = GetTaskState(t.task_id);
--~ 		
--~ 	Debug("取得前置任务(" .. t.task_id )--.. ", " .. pre_state .. ")");
--~ 	
--~ 	if pre_state == nil then Debug("nil") end;
--~ 	
--~ 	return pre_state == ETaskState.STATE_TASK_END;
if t.task_id == nil then t.task_id = 0; end;
 return IsTaskEnd(t.task_id);
end

function check_use_item(t)
   -- t参数：t.id 道具ID t.n 使用次数
   --Debug("id="..t.id);
   return CheckEvent( EEventForTask.EVENT_USEITEM, t.id , t.n );
end

function check_send_npc(t)
   -- t参数：t.id 道具ID t.n 使用次数
   return CheckEvent( EEventForTask.EVENT_SENDNPCCOMPLETE, t.id , 1 );
end

--以下是奖励接口 (对于金钱和经验，允许多倍奖励，由LUA程序控制)
function award_expr(t, n)    
	if n == nil or n == 0  then
		n = 1;
	end
	--lua_debug("xxxn="..n);
	if t.task_id == nil then t.task_id = 0; end;
	if t.task_id >= 1000 and task_awards[t.task_id]~=nil and task_awards[t.task_id].expr ~=0 then
		return PlayerAwardExpr((task_awards[t.task_id].expr or 0) * n);
	end
	return PlayerAwardExpr((t.expr or 0) * n);
end

function award_money(t, n)  
	if n == nil or n == 0  then
		n = 1;
	end
	--lua_debug("xxxn="..n);
	if t.task_id == nil then t.task_id = 0; end;
	if t.task_id >= 1000 and task_awards[t.task_id]~=nil and (task_awards[t.task_id].money>0 or task_awards[t.task_id].free_money>0) then
		if task_awards[t.task_id].money > 0 then
			PlayerAwardMoney(task_awards[t.task_id].money*n);
			--return true;
		end
		
		if task_awards[t.task_id].free_money > 0 then
			AwardCommonMoney(task_awards[t.task_id].free_money*n);
			
		end		
		return true;
	end
	
	-- 一般不会走下面 (在没配置表的情况下 走下面)
	  if t.is_free == nil or t.is_free == 0 then -- 绑定
		return PlayerAwardMoney((t.money or 0)*n);
	  else
		return AwardCommonMoney((t.money or 0)*n);
	  end

end

--任务刷新奖励
function award_expr_by_refresh(t,n)
	if n == nil or n <= 0  then
		n = 1;
	end
	if t.task_id == nil then t.task_id = 0; end;
	local tab = task_refresh_awards[t.task_id][t.color];
	local tab_default = task_refresh_awards[t.task_id][1];
	local val= get_level_award_exp( GetPlayer(EPlayerAttr.PLAYER_LEVEL), t.hard);
	if tab ==nil or tab_default == nil then return false;end;
	
	if t.task_id >= 1000 and task_awards[t.task_id]~=nil then
		if task_refresh_awards[t.task_id].loop_able == 1 then 
			return PlayerAwardExpr((val*tab.expr_rate or 0)*n);
		elseif tab.expr~=0 then 
			return PlayerAwardExpr((tab.expr or 0)*n);
		elseif tab.expr_rate ~=0 then
			return PlayerAwardExpr((tab_default.expr*tab.expr_rate or 0)*n);
		end
	end
	return PlayerAwardExpr((t.expr or 0) * n);
end

function award_money_by_refresh(t,n)
	if n == nil or n <= 0  then
		n = 1;
	end
	
	if t.task_id == nil then t.task_id = 0; end;
	local tab = task_refresh_awards[t.task_id][t.color];
	local tab_default = task_refresh_awards[t.task_id][1];
	local val= get_level_award_money( GetPlayer(EPlayerAttr.PLAYER_LEVEL), t.hard);
	if tab ==nil or tab_default == nil then return false;end
	
	if t.task_id >= 1000 and task_refresh_awards[t.task_id]~=nil then
	     if task_refresh_awards[t.task_id].loop_able == 1 then
		PlayerAwardMoney((val*tab.money_rate or 0)*n);
	     elseif tab.money~=0 then 
		PlayerAwardMoney((tab.money or 0)*n);
	     elseif tab.money_rate ~=0 then
		PlayerAwardMoney((tab_default.money*tab.money_rate or 0)*n);
	     end
	end
	return true;

end
function award_free_money_by_refresh(t,n)
	if n == nil or n <= 0  then
		n = 1;
	end
	
	if t.task_id == nil then t.task_id = 0; end
	if t.task_id >= 1000 and task_refresh_awards[t.task_id]~=nil then
	    if task_refresh_awards[t.task_id][t.color] ~=nil then
		if task_refresh_awards[t.task_id][t.color].free_money ~=0  then
			AwardCommonMoney((task_refresh_awards[t.task_id][t.color].free_money or 0) * n);
		elseif task_refresh_awards[t.task_id][t.color].money_rate~=0 then
			AwardCommonMoney((task_refresh_awards[t.task_id][t.color].money_rate*task_refresh_awards[t.task_id][1].free_money or 0) * n);
 		end
 		return true;
	   end
	end
	return true;

end
function award_telant_by_refresh(t,n)
     if t.task_id == nil then t.task_id = 0; end;
	if t.task_id >= 1000 and task_refresh_awards[t.task_id]~=nil then
		if task_refresh_awards[t.task_id][t.color]~=nil then
		     if task_refresh_awards[t.task_id][t.color].telant == 0 then
			return CommonAward(enCommonAwardCode.AWARDTYPE_TELANT, task_refresh_awards[t.task_id][1].telant or 0, 0, 0);
		     else
			return CommonAward(enCommonAwardCode.AWARDTYPE_TELANT, task_refresh_awards[t.task_id][t.color].telant or 0, 0, 0);
		     end
		end
	end
end

function award_items_by_refresh(t,n)
	Debug("check items id ");
	local tab = {};
	local tab_default = {};
	tab = task_refresh_awards[t.task_id][t.color].items;
	tab_default = task_refresh_awards[t.task_id][ 1 ].items;
	Debug("task_color is " ..t.color);
	if tab == nil then return false;end; 
	if tab_default == nil then  return false; end;
	table.foreachi(tab,function(i,v)
		PlayerAwardFreeItem(tab[i].id, tab[i].num);
	end );
	return true;
end
--根据等级奖励 
function award_expr_by_level(t, n)    
	if n == nil or n <= 0  then
		n = 1;
	end
	
	local level = GetPlayer(EPlayerAttr.PLAYER_LEVEL);
	if level == nil or level <= 0  then
		level = 1;
	end	
	n = n * level;
	if t.task_id == nil then t.task_id = 0; end;
	if t.task_id >= 1000 and task_awards[t.task_id]~=nil then
		return PlayerAwardExpr((task_awards[t.task_id].expr or 0) * n);
	end
	return PlayerAwardExpr((t.expr or 0) * n);
end

function award_money_by_level(t, n)  
--~ 	if n == nil or n == 0  then
--~ 		n = 1;
--~ 	end
--~ 	
--~ 	local level = GetPlayer(EPlayerAttr.PLAYER_LEVEL);
--~ 	if level == nil or level <= 0  then
--~ 		level = 1;
--~ 	end	
--~ 	n = n * level;
--~ 	if t.task_id == nil then t.task_id = 0; end;
--~ 	if t.task_id >= 1000 and task_awards[t.task_id]~=nil then
--~ 		if task_awards[t.task_id].money > 0 then
--~ 			PlayerAwardMoney(task_awards[t.task_id].money*n);
--~ 		end
--~ 		
--~ 		if task_awards[t.task_id].free_money > 0 then
--~ 			AwardCommonMoney(task_awards[t.task_id].free_money*n);
--~ 		end
--~ 		
--~ 		return true;
--~ 	end
--~ 	
--~ 	-- 一般不会走下面 (在没配置表的情况下 走下面)
--~ 	  if t.is_free == nil or t.is_free == 0 then -- 绑定
--~ 		return PlayerAwardMoney((t.money or 0)*n);
--~ 	  else
--~ 		return AwardCommonMoney((t.money or 0)*n);
--~ 	  end
	return true;
end


function award_money_in_loop(t,n)
	-- t.hard表示难度
	local val= get_loop_award_money( GetPlayer(EPlayerAttr.PLAYER_LEVEL), t.hard);
	if val ~= 0 then
	   if n ~= nil and n > 0  then
	     val = val * n;
	   end
	   
	    if t.is_free == nil or t.is_free == 0 then -- 绑定
		return PlayerAwardMoney(val);
	    else
		return AwardCommonMoney(val);
	    end
	end
end

function award_expr_in_loop(t,n)
	-- t.hard表示难度
	local val= get_loop_award_exp( GetPlayer(EPlayerAttr.PLAYER_LEVEL), t.hard);
	if val ~= 0 then
	  if n == nil or n == 0  then
	     return PlayerAwardExpr(val)
	  else
	     return PlayerAwardExpr(val * n)
	  end	
	end
end

function award_task(t)
if t.task_id == nil then t.task_id = 0; end;
if t.task_id == 0 then return false;end;
  return PlayerAwardTask(t.task_id)
end

function award_transfer(t)     
if t.map == 0 or t.mX ==0 or t.mY == 0 then return false;end;
  return PlayerAwardTeleport(t.map, t.mX, t.mY, t.mIsFB or 0)
end

function award_items(t)
if t.id == 0 or t.n == 0 then return false;end;
Debug("给玩家奖励道具 "..t.id.." "..t.n.."个");
  if t.is_free == nil or t.is_free == 0 then
	return PlayerAwardItem(t.id, t.n)
  else
	return PlayerAwardFreeItem(t.id, t.n)
  end 
end

function award_level_eqt(t)
if t.id == 0 or t.n == 0 then return false;end;
  return AwardLevelEqt(t.id, t.n, t.level);
end

-- 根据职业奖励
function award_items_by_metier(t)
if t.id == 0 or t.n == 0 or t.metier == 0  then return false;end;
  return PlayerAwardItemByMetier(t.id, t.n, t.metier, t.is_free or 0)
end

-- 概率性多倍奖励金钱(至少奖励1倍)
function award_money_muti_prob(t, n)
--~     if t.n == nil or t.n == 0 then t.n = 1 end
--~     if t.money == nil or n == nil or t.money ==0 then return;end;
--~     
--~     local lp = math.random(10000);
--~     if lp < ( t.p or 0 ) then -- 满足概率
--~         return PlayerAwardMoney(t.money*t.n*n)
--~     else
--~         return PlayerAwardMoney(t.money*n)
--~     end	
    return true;
end

-- 概率性多倍奖励经验(至少奖励1倍)
function award_exp_muti_prob(t, n)
    if t.n == nil or t.n == 0 then t.n = 1 end
    
    if t.expr == nil or t.expr==0 or n == nil then return false; end;
    
    if math.random(10000) < ( t.p or 0 ) then -- 满足概率
        return PlayerAwardExpr(t.expr*t.n*n)
    else
        return PlayerAwardExpr(t.expr*n)
    end	
    
end

--t = {option, id, n, metier} 
-- 按照玩家的职业和玩家的选择,奖励给玩家物品
function award_option_items(t, option_id)

if t.id1 == 0 or option_id == 0 then return false;end;
local id = 0; 
local n = 0;
  if option_id == 1 then
   id =t.id1;
   n = t.n1 or 1;
  end 
  
  if option_id == 2 then
   id =t.id2;
   n = t.n2 or 1;
  end 
  
  if option_id == 3 then
   id =t.id3;
   n = t.n3 or 1;
  end 
  if option_id == 4 then
   id =t.id4;
   n = t.n4 or 1;
  end
  if option_id == 5 then
   id =t.id5;
   n = t.n5 or 1;
  end
  
  if option_id == 6 then
   id =t.id6;
   n = t.n6 or 1;
  end
  
  if option_id == 7 then
   id =t.id7;
   n = t.n7 or 1;
  end 
  
  if option_id == 8 then
   id =t.id8;
   n = t.n8 or 1;
  end
  
    if t.is_free == nil or t.is_free == 0 then
	return PlayerAwardItem(id, n)
  else
	return PlayerAwardFreeItem(id, n)
  end
  
end



function award_change_metier(t)
if t.metier == 0 then return false;end;
	return AwardChangeMetier( t.metier or 7 );
end

function award_talk_withnpc(t)
	return SetEvent( EEventForTask.EVENT_TALKCOMPLETE, t.id );
end

function award_buff(t)
	return AwardBuff( t.id );
end

function award_box(t)
	return AwardBox( t.id );
end

function check_talk_withnpc(t)
   return CheckEvent( EEventForTask.EVENT_TALKCOMPLETE, t.id , 1 );
end

function check_not_talk_withnpc(t)
  local ret = CheckEvent( EEventForTask.EVENT_TALKCOMPLETE, t.id , 1 );
  if ret == false then
	return true;
  else
        return false;
  end
end

function check_upeqt(t)
  return CheckEvent( EEventForTask.EVENT_UPEQT, 0 , 1 );
end

function check_produceweapon(t)
  return CheckEvent( EEventForTask.EVENT_PRODUCE_WEAPON, 0 , 1 );
end


function check_produceprevent(t)
  return CheckEvent( EEventForTask.EVENT_PRODUCE_PREVENT, 0 , 1 );
end


function check_producedrug(t)
  return CheckEvent( EEventForTask.EVENT_PRODUCE_DRUG, 0 , 1 );
end


function check_stove(t)
  return CheckEvent( EEventForTask.EVENT_STOVE, 0 , 1 );
end


function check_jewel(t)
  return CheckEvent( EEventForTask.EVENT_JEWEL, 0 , 1 );
end


function check_charstone(t)
  return CheckEvent( EEventForTask.EVENT_CHARSTONE, 0 , 1 );
end


function check_hole(t)
  return CheckEvent( EEventForTask.EVENT_HOLE, 0 , 1 );
end

function check_skill(t)
  return CheckEvent( EEventForTask.EVENT_SKILL, 0 , 1 );
end

function check_endloop(t)
  return CheckEvent( EEventForTask.EVENT_ENDLOOP, 0 , 1 );
end

-- 并列条件
function check_multi_cond(t, collector)
	if t ~= nil then
		    local ret = table.foreachi(t, function(i, v)  
		        if v.func ~= nil then
			    if v.func(v.args, collector) == true then 
				Debug("检查并列 成功 在第"..i.."个函数");
				return true; 
			    end;
		        end
		    end);
		    Debug("检查并列失败");
		    return ret or false;
	else
		return true;
	end
end


-- 取东西
function get_items(t)
	Debug("收东西 "..t.id.." "..t.n.."个");
	if t.id == 0 or t.n == 0 then
		return false;
	else
		return PlayerRemoveItem(t.id, t.n)
	end
end


-- 把NPC变成怪物
function change_npc_to_ogre(t, vTaskID)
	ChangeCurNpc2Ogre(vTaskID);
end 

-- 干掉当前NPC
function  destroy_npc(t)
	DestroyNpc();
end


-- 开始护送NPC
function start_send_npc(t)
if t.task_id == nil then t.task_id = 0; end;
	local param = { npcid = t.npc_id or 0, maxgird = t.max_gird or 0,taskid = t.task_id or 0, leavetime = t.max_time or 0,  bufftime =  t.buff_time or 0,  buffid = t.buff_id or 0, wait_time=t.wait_time or 0 };
	return StartSendNpc( param, t.player_id or 0 );
end

-- 在指定地点刷出NPC
function create_funcnpc(t)
	local param = { npc_id = t.npc_id or 0, posx = t.pos_x or 0, posy = t.pos_y or 0, map_id = t.map_id or 0 };
	return CreateNpc( param, t.player_id or 0 );
end

-- 检查每天完成某组任务次数
function check_today_completedbygroup(t)
	local tTimes = GetTodayTimesByGroupID( t.group_id or 0 );
	--lua_debug("check_today_completedbygroup tTimes:"..tTimes.." max="..(t.max_times or 0));
	return tTimes < (t.max_times or 0) ;
end
--check weekly completed  task
function check_week_completedbygroup(t)
	local tTimes = GetWeekTimesByGroupID( t.group_id or 0 );
	return tTimes < (t.max_times or 0) ;
end
--check daily teamloop completed  task
function check_todayteam_loop_num(t)
	local ret = CommonCheck(enCommonCheckCode.CHECKTYPE_TEAM_LOOPNUM, t.times, 0, 0 );
	return ret;
end

-- 检查是不是家族成员
function check_is_family_member(t)
	local tRet = CommonCheck( enCommonCheckCode.CHECKTYPE_ISFAMILYMEMBER, 0, 0, 0 );
	--lua_debug("check_is_family_member: ret="..tRet);
	return tRet;
end

-- 检查是不是家族族长
function check_is_family_head(t)
	return CommonCheck( enCommonCheckCode.CHECKTYPE_ISFAMILYHEAD, 0, 0, 0 );
end

-- 检查是不是军团成员
function check_is_corps_member(t)
	return CommonCheck( enCommonCheckCode.CHECKTYPE_ISCORPSMEMBER, 0, 0, 0 );
end
-- 奖励修为点
function award_telant(t)
if t.task_id == nil then t.task_id = 0; end;
	if t.task_id >= 1000 and task_awards[t.task_id]~=nil then
		return CommonAward(enCommonAwardCode.AWARDTYPE_TELANT, task_awards[t.task_id].telant or 0, 0, 0);
	end
end

function award_family_honor(t)
return CommonAward(enCommonAwardCode.AWARDTYPE_FAMILY_HONOR, t.value or 0, 0, 0);
end

function award_family_money(t)
return CommonAward(enCommonAwardCode.AWARDTYPE_FAMILY_MONEY, t.value or 0, 0, 0);
end

function award_family_contribute(t)
return CommonAward(enCommonAwardCode.AWARDTYPE_FAMILY_CONTRIBUTE, t.value or 0, 0, 0);
end

function award_corps_glory(t)
return CommonAward(enCommonAwardCode.AWARDTYPE_CORPS_GLORY, t.value or 0, 0, 0);
end

function award_corps_money(t)
return CommonAward(enCommonAwardCode.AWARDTYPE_CORPS_MONEY, t.value or 0, 0, 0);
end

function award_corps_contribute(t)
return CommonAward(enCommonAwardCode.AWARDTYPE_CORPS_CONTRIBUTE, t.value or 0, 0, 0);
end
function award_corps_collectactivity(t)
return CommonAward(enCommonAwardCode.AWARDTYPE_CORPS_COLLECTACTIVE, t.value or 0, 0 , 0)
end

-- 完成条件：检查杀死外国人x个
function check_killforeigner(t, collector)
  if collector and t and t.num then
    return collector:get_pair(enSpecialID.SPECIALID_KILLFOREIGNER)  >= (t.num or 0)
  else
    lua_error("collector is nil2");
  end

  return false;
end

-- 接任务条件： 检查称号
function check_hastitle(t)
	return CommonCheck( enCommonCheckCode.CHECKTYPE_HASTITLE, t.id or 0, 0, 0 );
end

-- 奖励： 奖励称号
function award_title(t)
	return CommonAward(enCommonAwardCode.AWARDTYPE_TITLE, t.id or 0, 0, 0);
end

-- 邀请家族成员
function award_invite_family_members( t )
if t.task_id == nil then t.task_id = 0; end;
	return InviteFamilyMembers( t.task_id or 0, t.player_id or 0 );
end

-- 检查按等级杀怪条件
function check_level_orges_by_killed(t, collector)
  if collector and t and t.min_level and t.max_level then
    return collector:get_pair(cale_kill_level_ogre_id(t.min_level , t.max_level))  >= (t.n or 0)
  else
    lua_error("collector is nil");
  end

  return false;
end

-- 接任务条件：检查家族平均等级
function check_family_member_avg_level( t )
	local lev = GetFamilyAvgLevel();
	if lev == nil then
		return false;
	else
		lua_debug("check_family_member_avg_level -- "..lev);
		  local mini = t.min_level  or 0
		  local maxi = t.max_level  or 10000
		  if maxi == 0 then maxi = 10000; end;

		  return lev >= mini and lev <= maxi;
	end
	return false;
end

function award_honor(t)
return CommonAward(enCommonAwardCode.AWARDTYPE_HONOR, t.value or 0, 0, 0);
end

-- 检查是否完成了悬赏任务
function check_complete_storagetask(t, collector)
  if collector and t and t.n then
    return collector:get_pair(enSpecialID.SPECIALID_COMPLETESTORAGE)  >= (t.n or 0)
  else
    lua_error("collector is nil");
  end

  return false;
end


-- 检查互斥
function check_mutex_by_group(t)
	return CheckMutexByTask(t.group_id or 0);
end


-- 检查时间段
function check_time_part(t)
	local hour = tonumber( os.date("%H", os.time() ) );
	local minu = tonumber( os.date("%M", os.time() ) );
	lua_debug( "check time: now: "..hour .. ":"..minu );
	
	if (t.start_hour or 0) <= hour and  (t.end_hour or 0) >= hour then  -- 小时符合
		if (t.start_hour or 0) == hour and (t.start_minu or 0) > minu then
			return false;
		end
		
		if (t.end_hour or 0) == hour and (t.end_minu or 0) <  minu then
			return false;
		end
	else
		return  false;
	end
end

-- 奖励绑定银元宝
function award_bind_yinyuanbao(t)
	return CommonAward(enCommonAwardCode.AWARDTYPE_BINDYINGYUANBAO, t.value or 0, 0, 0);
end

--奖励竞技声望
function award_combat_fame(t)	
	return CommonAward(enCommonAwardCode.AWARDTYPE_COMBAT_FAME, t.value or 0, 0, 0);
end
--奖励副本声望
function award_repe_fame(t)	
	return CommonAward(enCommonAwardCode.AWARDTYPE_REPE_FAME, t.value or 0, 0, 0);
end
--奖励历练声望
function award_common_fame(t)
	return CommonAward(enCommonAwardCode.AWARDTYPE_COMMON_FAME, t.value or 0, 0, 0);
end
-- 奖励邮件
function award_send_mail(t)
	SendSysMail(t);
end

-- 检查BUFF
function  check_buff(t)
	return CommonCheck( enCommonCheckCode.CHECKTYPE_BUFF, t.buff_id or 0, 0, 0 );
end

-- 检查没有BUFF
function  check_no_buff(t)
	return not CommonCheck( enCommonCheckCode.CHECKTYPE_BUFF, t.buff_id or 0, 0, 0 );
end

-- 检查NPC状态 （目前只是检查NPC活着）
function  check_npc_state(t)
-- 以后要是有其他状态 得改接口
	return  GetWatchNpcState( t.npc_id or 0 ) ;
end

-- 守旗任务专用完成条件
function check_protect_flag(t)
	return false;
end

-- 奖励国家建设度
function award_nation_build(t)
	if t.build_type == nil or t.build_type == 0 or country_build_point[t.build_type] == nil or country_build_point[t.build_type].build_type == nil then
		lua_error("nation table error! btype="..(t.build_type or 0));
		return false;
	end
	local award_build_points = 0;
	table.foreachi( country_build_point[t.build_type].build_type, function(i,v)
		local point = CommonGetData(v);
		if country_build_point[t.build_type].points[point] ~= nil and country_build_point[t.build_type].points[point][v] ~= nil then
			award_build_points = award_build_points + country_build_point[t.build_type].points[point][v];
			lua_debug("now award_build_points:"..award_build_points.. " point="..point.." v="..v);
		end
		
	end);
	
	return CommonAward(enCommonAwardCode.AWARDTYPE_COUNTRYBUILD, award_build_points, t.build_type-1, 0);
end

-- 检查国家
function check_country(t)
	return CommonCheck( enCommonCheckCode.CHECKTYPE_COUNTRY, t.country_id or 9999, 0, 0 );
end

-- 读表根据等级奖励经验金钱
function award_expr_by_level_table(t)
	-- t.hard表示难度
	local val= get_level_award_exp( GetPlayer(EPlayerAttr.PLAYER_LEVEL), t.hard);
	if val ~= 0 then
	  if n == nil or n == 0  then
	     return PlayerAwardExpr(val)
	  else
	     return PlayerAwardExpr(val * n)
	  end	
	end
end

-- 读表根据等级奖励经验金钱
function award_money_by_level_table(t)
	-- t.hard表示难度
	local val= get_level_award_money( GetPlayer(EPlayerAttr.PLAYER_LEVEL), t.hard);
	if val ~= 0 then
	   if n ~= nil and n > 0  then
	     val = val * n;
	   end
	   
	    if t.is_free == nil or t.is_free == 0 then -- 绑定
		return PlayerAwardMoney(val);
	    else
		return AwardCommonMoney(val);
	    end
	end
end

-- 检查发布
function check_publish_times(t)
	return CommonCheck( enCommonCheckCode.CHECKTYPE_PUBLISH, t.taskid or 0, 0, 0 );
end

-- 上报才华值
function award_up_wis(t)
	CommonAward(enCommonAwardCode.AWARDTYPE_UPWIS, 0, 0, 0);
	
	--　最新要求，上报才华要上报建设度
--~ 	for i=1,6 do
--~ 		local ret={};
--~ 		ret.build_type = i;
--~ 		award_nation_build(ret);
--~ 	end
	-- 先去掉 等作为的新公式
	return true;
end

-- 奖励才华经验
function award_wis_exp(t)
	return CommonAward(enCommonAwardCode.AWARDTYPE_WISEXP, t.wtype or 0, t.val or 0, 0);
end

-- 检查所在家族占领了指定NPC
function check_family_own_npc(t)
	return CommonCheck( enCommonCheckCode.CHECKTYPE_FAMILY_NPC, t.npc_id or 0, 0, 0 );
end

-- 检查所在军团占领了指定NPC
function check_corps_own_npc(t)
	return CommonCheck( enCommonCheckCode.CHECKTYPE_CORPS_NPC, t.npc_id or 0, 0, 0 );
end

-- 检查完成环任务
function check_complete_looptask(t, collector)
  if collector and t and t.n then
    return collector:get_pair(enSpecialID.SPECIALID_COMPLETELOOPTASK)  >= (t.n or 0)
  else
    lua_error("cdcollector is nil");
  end

  return false;
end

function award_option_items_by_metier(t, option_id)
  local metier = GetPlayer(EPlayerAttr.PLAYER_METIER);

  Debug("award_option_items " .. metier .. " in (" .. t.metier .. ");")

  if t.metier==nil or metier ~= t.metier then
	return false;
  end
  
if t.id1 == 0 or option_id == 0 then return false;end;
local id = 0; 
local n = 0;
  if option_id == 1 then
   id =t.id1;
   n = t.n1 or 1;
  end 
  
  if option_id == 2 then
   id =t.id2;
   n = t.n2 or 1;
  end 
  
  if option_id == 3 then
   id =t.id3;
   n = t.n3 or 1;
  end 
  if option_id == 4 then
   id =t.id4;
   n = t.n4 or 1;
  end
  if option_id == 5 then
   id =t.id5;
   n = t.n5 or 1;
  end
  
  if option_id == 6 then
   id =t.id6;
   n = t.n6 or 1;
  end
  
  if option_id == 7 then
   id =t.id7;
   n = t.n7 or 1;
  end 
  
  if option_id == 8 then
   id =t.id8;
   n = t.n8 or 1;
  end
  
    if t.is_free == nil or t.is_free == 0 then
	return PlayerAwardItem(id, n)
  else
	return PlayerAwardFreeItem(id, n)
  end
  
end

function check_activity_on(t)
	return CommonCheck( enCommonCheckCode.CHECKTYPE_ACTIVITY, t.id or 0, 0, 0 );
end


-- 检查日期段
function check_date(t)
	local now=tonumber(os.date("%Y%m%d", os.time() ));
	if t.start_year == nil or t.start_year==0 then t.start_year=2010;end;
	if t.end_year == nil or t.end_year==0 then t.end_year=2010;end;
	local start_dt= tonumber(string.format( "%04d%02d%02d", t.start_year, (t.start_month or 0), (t.start_day or 0) ));
	local end_dt=tonumber(string.format( "%04d%02d%02d", t.end_year, (t.end_month or 0), (t.end_day or 0) ));
	return now>=start_dt and now <=end_dt;
end

-- 检查注册日期早于
function check_reg_before(t)
	return CommonCheck(enCommonCheckCode.CHECKTYPE_REG_DATE, t.year, t.month, t.day );
end

-- 检查完成答题次数
function check_ques_num(t)
	return CommonCheck(enCommonCheckCode.CHECKTYPE_QUES_NUM, t.num, 0, 0 );
end

-- 检查完成每日答题次数
function check_ques_num(t, collector)
  if collector and t and t.num then
    return collector:get_pair(enSpecialID.SPECIALID_COMPLETEQUESTION)  >= (t.num or 0)
  else
    lua_error("cdcollector is nil");
  end

  return false;
	
end

-- 检查绣球状态
function check_hydrangea_status(t)
	return CommonCheck( enCommonCheckCode.CHECKTYPE_HYDRANGEASTATUS, t.status or 0, 0, 0 );
end 

-- 奖励 清除绣球状态
function award_clear_hydrangea_status(t)
	return CommonAward(enCommonAwardCode.AWARDTYPE_CLEARHYDRANGEASTATUS, 0, 0, 0);
end

-- 检查每天完成某组任务次数
function check_today_min_completedbygroup(t)
	local tTimes = GetTodayTimesByGroupID( t.group_id or 0 );
	return tTimes >= (t.min_times or 0) ;
end

-- 检查不在某阵营出现
function check_not_in_camp(t)
	local bIsCamp = CommonCheck( enCommonCheckCode.CHECKTYPE_IN_CAMP, t.camp, 0, 0 );
	return not bIsCamp;
end

-- 检查累计在线时间达到
function check_onlinesec_over(t)
	return CommonCheck( enCommonCheckCode.CHECKTYPE_ONLINESECS, t.sec, 0, 0 );
end

-- 检查注册日期晚于
function check_reg_after(t)
	local ret = CommonCheck(enCommonCheckCode.CHECKTYPE_REG_DATE, t.year, t.month, t.day );
	return not ret;
end

-- 检查弱国任务
function check_weeknation(t)
	local ret = CommonCheck(enCommonCheckCode.CHECKTYPE_WEEKNATION, 0, 0, 0 );
	return ret;
end

-- 检查每天接取某组任务次数
function check_today_acceptedbygroup(t)
	local tTimes = GetTodayAcceptTimesByGroupID( t.group_id or 0 );
	--lua_debug("check_today_completedbygroup tTimes:"..tTimes.." max="..(t.max_times or 0));
	return tTimes < (t.max_times or 0) ;
end

-- 检查只能在本阵营接任务 在本阵营 返回true
function check_in_my_camp(t)
	local bIsCamp = CommonCheck( enCommonCheckCode.CHECKTYPE_IN_MY_SELF, 0, 0, 0 );
	return  bIsCamp;
end

-- 读表根据等级奖励经验金钱 【国探专用】
function award_expr_by_level_table_for_nation_kill(t)
	-- t.hard表示难度
	local val= get_level_award_exp_for_nation_kill( GetPlayer(EPlayerAttr.PLAYER_LEVEL), t.hard, t.task_id);
	if val ~= 0 then
	     return PlayerAwardExpr(val)
	end
end

-- 读表根据等级奖励经验金钱 【乱舞专用】
function award_expr_by_level_table_for_luanwu(t)
	local val= get_level_award_exp( GetPlayer(EPlayerAttr.PLAYER_LEVEL), t.hard);
	if val == nil then return false;end;
	if CommonCheck( enCommonCheckCode.CHECKTYPE_IS_LUANWU, 0, 0, 0) == true then
		val=0;
	end
	if val ~= 0 then
	     return PlayerAwardExpr(val)
	end
end

-- 检查是否师傅
function check_is_master(t)
	local ret = CommonCheck(enCommonCheckCode.CHECKTYPE_IS_MASTER, 0, 0, 0 );
	return ret;
end

-- 检查是否队长
function check_is_teamhead(t)
	local ret = CommonCheck(enCommonCheckCode.CHECKTYPE_IS_TEAMHEAD, 0, 0, 0 );
	return ret;
end

-- 检查队伍人数
function check_team_member(t)

	local ret = CommonCheck(enCommonCheckCode.CHECKTYPE_TEAM_MEMBER, t.num, 0, 0 );
	return ret;
end
--检查是否是VIP
function check_is_vip_player(t)	

	local ret = CommonCheck(enCommonCheckCode.CHECKTYPE_ISVIP_PLAYER, t.vip, 0, 0 );	
	return ret;
end

function check_is_sworn_player(t)	

	local ret = CommonCheck(enCommonCheckCode.CHECKTYPE_ISSWORNED_PLAYER, t.sworn, 0, 0 );	
	return ret;
end
--检查军团等级
function check_corps_level(t)
	local ret = PublishCheck(emPublishCheck.CHECK_CORPS_LEVEL, t.min_level, 0, 0 );	
	return ret;
end
--检查军团金钱
function check_corps_money(t)
	local ret = PublishCheck(emPublishCheck.CHECK_CORPS_MONEY, t.money, 0, 0 );	
	return ret;
end
--检查玩家军团职位职位
function check_corps_position(t)
	local ret = PublishCheck(emPublishCheck.CHECK_CORPS_POSITION, t.min_position, 0, 0 );	
	return ret;
end