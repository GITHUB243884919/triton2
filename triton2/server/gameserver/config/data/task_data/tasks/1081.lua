task = {
  id = 1081,    --任务ID
  kind = 7,    --任务种类
  color = 4,    --任务颜色
  name = "#td_1081_name",  
  desc = "#td_1081_desc",  
  dest = "#td_1081_dest",  
  section = "#td_1081_section",  
  give_npc = 45625,    --发放NPC
  give_map_id = 63,    --发放NPC所在的地图ID
  recv_npc = 45625,    --回收NPC
  recv_map_id = 63,    --回收NPC所在的地图ID
  hard = 0,    --任务难度
  timer = 0,    --任务时限(秒)
  cancelable = 0,    --任务能否被放弃
  repeatable = 0,    --任务成功后可否重做
  retryable = 1,    --任务失败后可否重试
  fail_on_killed = 0,    --玩家被杀死后，是否认为任务失败
  after_task = 1082,    --后置任务(自动弹出)
  direct_complete = 0,    --任务是否直接完成(不用找NPC交)
  get_metier_task = 0,    --是否任职任务
  task_level = 39,    --任务适应玩家等级
  send_npc_id = 0,    --要护送的NPC
  max_gird = 0,    --玩家与护送NPC的最大格子数
  is_open_npc = 0,    --是否在接任务时保持和NPC连接(升级任务用)
  is_fetch = 0,    --是否辅助任务(默认否)
  task_type = 1,    --任务类型
  task_hard = 6,    --任务类型及难度
  trace={ 
	"#td_1081_trace1",
	"#td_1081_trace2",
  }; 
  max_send_time = 0,    --玩家离开护送NPC的最长时间(秒)
  offline_fail = 0,    --是否玩家下线任务就失败
  group_id = 0,    --任务子类别
  is_fail_auto_abort = 0,    --是否失败时自动放弃
  is_share_complete = 0,    --同屏玩家是否分享任务成功
  task_map_id = 63,    --显示已接任务的地图ID
  can_invite = 0,    --是否可以邀请
  timeout_action = 0,    --限时任务到时间后任务状态
  time_lmt_type = 0,    --限时任务类型
  is_redirect_fail = 0,    --是否跨地图任务失败
  is_day_clear = 0,    --是否每天清除完成记录
  give_camp = 0,    --发放NPC阵营
  recv_camp = 0,    --回收NPC阵营
  is_day_fail = 0,    --是否隔天失败
  loop_type = 0,    --环任务类型
  team_member_lmt = 0,    --要求组队人数
  finish_delay_time = 0,    --玩家需要过多久才能交任务（秒）
  refresh_award_able = 0,    --任务是否可以刷新奖励
  task_reaccept_time = 0,    --任务完成后多久可以再接相同任务（秒）
  publish_type = 0,    --发布任务类型
  give_items = {};   --发放道具
  --护送NPC路径
  npc_waypoints = {};
  drop_items = {};   --掉落道具
  --接任务条件
  accept_cond = 
  {
	{func=check_before_task, args={task_id = 1080}},
  };
  --发布任务条件
  publish_cond = 
  {
  };
  --任务显示条件
  show_cond = 
  {
  };
  --完成条件
  complete_cond = 
  {
	{func=check_many_orges_by_killed, args={id1 = 44905,id2 = 44906,id3 = 44907,id4 = 0,id5 = 0,n = 20}},
	{func=check_many_orges_by_killed, args={id1 = 44908,id2 = 44909,id3 = 44910,id4 = 0,id5 = 0,n = 20}},
  };
  --任务完成奖励
  awards = 
  {
	{func=award_expr, args={expr = 112500}},
	{func=award_money, args={money = 12250}},
	{func=award_items_by_metier, args={id = 43183,n = 1,metier = 4,is_free = 0}},
	{func=award_items_by_metier, args={id = 43183,n = 1,metier = 5,is_free = 0}},
	{func=award_items_by_metier, args={id = 43182,n = 1,metier = 2,is_free = 0}},
	{func=award_items_by_metier, args={id = 43182,n = 1,metier = 3,is_free = 0}},
	{func=award_items_by_metier, args={id = 43181,n = 1,metier = 1,is_free = 0}},
	{func=award_items_by_metier, args={id = 43181,n = 1,metier = 6,is_free = 0}},
	{func=award_telant, args={money = 0}},
};
  --接受任务奖励
  accept_awards = 
  {
};
  --对话
  dialog =   {
  --接任务对话
	[1] = {
	parent_id = 0,
	window_type = 1,
	npc_talk ="#td_1081_dialog_accept_npctalk_1",
	buttons = {
        { player_talk = "#td_1081_dialog_accept_player_talk_1_1" ,dest_type = 1 ,dest_window_id = 266 ,func_code = 0},

	  }
	},
	[266] = {
	parent_id = 1,
	window_type = 1,
	npc_talk ="#td_1081_dialog_accept_npctalk_266",
	buttons = {
        { player_talk = "#td_1081_dialog_accept_player_talk_266_1" ,dest_type = 1 ,dest_window_id = 267 ,func_code = 0},

	  }
	},
	[267] = {
	parent_id = 266,
	window_type = 1,
	npc_talk ="#td_1081_dialog_accept_npctalk_267",
	buttons = {
        { player_talk = "#td_1081_dialog_accept_player_talk_267_1" ,dest_type = 1 ,dest_window_id = 268 ,func_code = 0},

	  }
	},
	[268] = {
	parent_id = 267,
	window_type = 3,
	npc_talk ="#td_1081_dialog_accept_npctalk_268",
	buttons = {
        { player_talk = "#td_1081_dialog_accept_player_talk_268_1" ,dest_type = 2 ,dest_window_id = 0 ,func_code = 3},

	  }
	},
  --任务进行对话
	[2] = {
	parent_id = 0,
	window_type = 1,
	npc_talk ="#td_1081_dialog_doing_npctalk_2",
	buttons = {

  }
},
  --完成任务对话
	[3] = {
	parent_id = 0,
	window_type = 1,
	npc_talk ="#td_1081_dialog_end_npctalk_3",
	buttons = {
        { player_talk = "#td_1081_dialog_end_player_talk_3_1" ,dest_type = 1 ,dest_window_id = 271 ,func_code = 0},

  }
},
	[271] = {
	parent_id = 3,
	window_type = 2,
	npc_talk ="#td_1081_dialog_end_npctalk_271",
	buttons = {
        { player_talk = "#td_1081_dialog_end_player_talk_271_1" ,dest_type = 2 ,dest_window_id = 0 ,func_code = 4},

  }
},
};
  --对话NPC
  dialog_with_npc = {
};
}