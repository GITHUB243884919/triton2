local _class={}
function class(super)
	local class_type={}
	class_type.ctor=false
	class_type.super=super
	class_type.new=function(...) 
			local obj={}
			do
				local create
				create = function(c,...)
					if c.super then
						create(c.super,...)
					end
					if c.ctor then
						c.ctor(obj,...)
					end
				end
 
				create(class_type,...)
			end
			setmetatable(obj,{ __index=_class[class_type] })
			return obj
		end
	local vtbl={}
	_class[class_type]=vtbl
 
	setmetatable(class_type,{__newindex=
		function(t,k,v)
			vtbl[k]=v
		end
	})
 
	if super then
		setmetatable(vtbl,{__index=
			function(t,k)
				local ret=_class[super][k]
				vtbl[k]=ret
				return ret
			end
		})
	end
 
	return class_type
end

-----------------------------class   collector----------------------
ccollector = class()

function ccollector:ctor() 
  self.vals = {};
end

function ccollector:set_pair(id, val) 
  local founded = table.foreachi(self.vals, function(i, v)
      lua_debug("---v.id="..v.id);
      if v.id == id then 
	  v.n= v.n + val; 
	  return true; 
      end
  end);

  if (not founded)  then
    local t = {}
    Debug("try insert a null value(" .. id .. ", " .. val .. ")");
    t.id = id; t.n = val;
    table.insert(self.vals, t);

    Debug("insert a null value(" .. t.id .. ", " .. t.n .. ")");
  end
end


function ccollector:get_pair(id)
  local ret = table.foreachi(self.vals, function(i, v)
    if v.id == id then 
      return v.n; 
    end
  end);

  return ret or 0;
end

function ccollector:save(pTask)
  table.foreachi(self.vals, function (i, v)
    pTask:SetTaskData((i-1)*2, v.id);
    pTask:SetTaskData((i-1)*2+1, v.n);
  end);
end

function ccollector:init(pTask)
local num = 0;
  for i = 1, 16, 2 do 
    local t1 = {};
    if pTask:GetTaskData(i-1) == 0 then break; end
    t1.id = pTask:GetTaskData(i-1);
    t1.n = pTask:GetTaskData(i);
    table.insert(self.vals, t1);
    num = num + 1
  end
  return num;
end

function ccollector:get_by_index ( index )
	local ret = { id = 0, n = 0 };
	if self.vals[index] == nil then
		return ret;
	else
		ret.id = self.vals[index].id;
		ret.n  = self.vals[index].n;
		return ret;
	end
end
----------------------------class   looptask---------------------
clooptask = class()

function clooptask:ctor()
  self.accept_time = 0;
  self.abort_time = 0;
  self.total_times = 0;
  self.today_times = 0;
  self.this_task = 0;
  self.this_state = 0;
end

function clooptask:init(pTask)
  self.accept_time = pTask:GetTaskData(0) * 256 * 256 + pTask:GetTaskData(1);
  self.abort_time = pTask:GetTaskData(2) * 256 * 256 + pTask:GetTaskData(3);

  self.total_times = pTask:GetTaskData(4);
  self.today_times = pTask:GetTaskData(5);

  self.this_task = pTask:GetTaskData(6);
  self.this_state = pTask:GetTaskData(7);
end

function clooptask:save(pTask)
  pTask:SetTaskData(0, math.mod(self.accept_time, (256 * 256)));  
  pTask:SetTaskData(1, math.mod(self.accept_time, (256 * 256)));  

  pTask:SetTaskData(2, math.mod(self.abort_time,  (256 * 256)));  
  pTask:SetTaskData(3, math.mod(self.abort_time,  (256 * 256)));  

  pTask:SetTaskData(4, self.total_times);
  pTask:SetTaskData(5, self.today_times);

  pTask:SetTaskData(6, self.this_task);
  pTask:SetTaskData(7, self.this_state);
end


-------------------------------------------------------------------- 通用概率管理系统--


-- LUA复制表  目前只支持一维数组
function copy_table(src)
	if src == nil then return;end;	
	ret={}; 
	
	table.foreachi(src, function(i,v)
		table.insert(ret, v );
	end);	
	
	return ret;
end

function debug_tab( tab, str )
lua_debug( str or "开始输出表：");
	table.foreachi(tab, function(i,v)
		lua_debug("debug table: "..i.."---"..v);
	end);
end

-- LUA 把一个表的前n个元素追加到另外的表
function append_table(src, des, n)
	if n==0 then return; end;
	if src == nil then return;end;	
	if des == nil then return;end;	
	
	for i=1,n do
		if src[i] ~= nil then
			table.insert(des, src[i]);
		else
			return;
		end
	end
end

-- 从数组里随机抽成员 下面那个函数要用 外围减减
function out_member(ret, num, member_array)
	local tmp=math.random(num);
	table.insert( ret, member_array[tmp] );
	member_array[tmp]=member_array[num];
end

-- 获得刷新随机数
-- eg: tab={10,20,5,15}  返回 1,,2,3或4  表示命中了哪个断
function get_rand_index( tab )
	--debug_tab(tab);
	local n = table.getn( tab );
	local sum=0;
	for i=1, n do
		sum = sum+tab[i];
	end
	if sum == 0 then
		return 0;
	end
	local rnd = math.random(sum);
	local last_sum = 0;
	local ret = 0;
	for i=1, n do
		last_sum = last_sum + tab[i] or 0;
		ret = i;
		if rnd <= last_sum then
			--lua_error("get_rand_index rnd="..rnd.." sum="..sum.." n="..n.."  return="..i);
			return i;
		end
	end
	--lua_debug("get_rand_index return n="..n);
	return ret;
end


-- 根据概率获得任务 传入一级索引 和各二级索引对应的概率数值 (下标要对应 数量要一致)
-- 参数表：大索引，概率表，要抽出的总数，[必出的二级索引ID号](可选) , 是否某项抽空即返回
function get_tasks_by_prob( index, prob_tab, total_num, must_index, is_empty_stop )
	local ret = {};
	if prob_tasks[index] == nil then
		lua_error("nil!! in get_tasks_by_prob index ="..index);
		return ret;
	end 
	
	if table.getn(prob_tab) ~= table.getn(prob_tasks[index]) then
		lua_error("call in get_tasks_by_prob prob_tab num ="..table.getn(prob_tab).."prob_tasks[index]="..table.getn(prob_tasks[index]) );
		return ret;
	end
	
	if total_num <= 0 then
		lua_error("call in get_tasks_by_prob total_num ="..total_num);
		return ret;
	end
	
	local swap_tab={};
	local member_left_num = {};
	table.foreachi( prob_tasks[index], function (i, v)
		swap_tab[i]=copy_table(v);
		table.insert(member_left_num, table.getn(v));
		--lua_debug("n="..table.getn(v));
	end); 
	local i = 0;
	local swap_prob_tab=copy_table(prob_tab);
	
	if must_index ~= nil and must_index > 0 then
		if must_index > table.getn(prob_tasks[index]) then
			lua_error("error in get_tasks_by_prob, must_index="..must_index.." index="..index );
		else
			out_member( ret, member_left_num[must_index] , swap_tab[must_index] );
			member_left_num[must_index] = member_left_num[must_index]-1;
			i = i + 1;
		end
	end
	while i< total_num do
		local rnd_index = get_rand_index( swap_prob_tab );
		--lua_error("index="..rnd_index);
		if rnd_index == 0 then
			lua_error("WARN:get_tasks_by_prob not enough! index="..index.."fact num="..i.." not"..total_num);
			return ret;
		end
		if member_left_num[rnd_index] == 0 or member_left_num[rnd_index] == nil then
			swap_prob_tab[rnd_index]=0;
			if is_empty_stop == 1 then
				return ret;
			end 
		else
			out_member( ret, member_left_num[rnd_index] , swap_tab[rnd_index] );
			member_left_num[rnd_index] = member_left_num[rnd_index]-1;
			i = i + 1;
		end
	end	
	
	return ret;
end 





--~   math.randomseed(os.time())
--~ function lua_error(str)
--~ print(str);
--~ end

--~ function lua_debug(str)
--~ print(str);
--~ end

--~ prob_tasks[1]={};
--~ prob_tasks[1][1]={11,12,13};
--~ prob_tasks[1][2]={21,25};
--~ prob_tasks[2]={};
--~ prob_tasks[3]={54};

--~ local a=10;
--~ local b=20;
--~ local pt={a,b};

--~ for i=1,50 do
--~ debug_tab( get_tasks_by_prob( 1, pt, 3 ) );
--~ --debug_tab( prob_tasks[1][1],"prob_tasks[1][1]" );
--~ end
--~ debug_tab(pt, "pt");
