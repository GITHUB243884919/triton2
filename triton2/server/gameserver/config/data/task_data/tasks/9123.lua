task = {
  id = 9123,    --任务ID
  kind = 0,    --任务种类
  color = 0,    --任务颜色
  name = "#td_9123_name",  
  desc = "#td_9123_desc",  
  dest = "#td_9123_dest",  
  section = "#td_9123_section",  
  give_npc = 42904,    --发放NPC
  give_map_id = 28,    --发放NPC所在的地图ID
  recv_npc = 42905,    --回收NPC
  recv_map_id = 64,    --回收NPC所在的地图ID
  hard = 0,    --任务难度
  timer = 0,    --任务时限(秒)
  cancelable = 1,    --任务能否被放弃
  repeatable = 0,    --任务成功后可否重做
  retryable = 1,    --任务失败后可否重试
  fail_on_killed = 0,    --玩家被杀死后，是否认为任务失败
  after_task = 9124,    --后置任务(自动弹出)
  direct_complete = 0,    --任务是否直接完成(不用找NPC交)
  get_metier_task = 0,    --是否任职任务
  task_level = 48,    --任务适应玩家等级
  send_npc_id = 0,    --要护送的NPC
  max_gird = 20,    --玩家与护送NPC的最大格子数
  is_open_npc = 0,    --是否在接任务时保持和NPC连接(升级任务用)
  is_fetch = 0,    --是否辅助任务(默认否)
  task_type = 8,    --任务类型
  task_hard = 7,    --任务类型及难度
  trace={ 
	"#td_9123_trace1",
  }; 
  max_send_time = 10,    --玩家离开护送NPC的最长时间(秒)
  offline_fail = 0,    --是否玩家下线任务就失败
  group_id = 0,    --任务子类别
  is_fail_auto_abort = 0,    --是否失败时自动放弃
  is_share_complete = 0,    --同屏玩家是否分享任务成功
  task_map_id = 64,    --任务所属的地图ID
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
  npc_waypoints = {{x=54,y=53,map=28},{x=54,y=70,map=28},{x=54,y=96,map=28},{x=59,y=96,map=28},{x=59,y=107,map=28},{x=70,y=110,map=28},{x=71,y=107,map=28},{x=71,y=116,map=28},{x=75,y=116,map=28},{x=75,y=121,map=28},{x=25,y=29,map=64},{x=20,y=40,map=64},{x=20,y=47,map=64},{x=18,y=47,map=64},{x=18,y=49,map=64},{x=17,y=49,map=64},{x=17,y=52,map=64},{x=15,y=52,map=64},{x=15,y=54,map=64},{x=14,y=54,map=64},{x=14,y=56,map=64},{x=12,y=56,map=64},{x=12,y=58,map=64},{x=11,y=58,map=64},{x=11,y=59,map=64},{x=9,y=59,map=64},{x=9,y=61,map=64},{x=5,y=61,map=64},{x=5,y=63,map=64},{x=2,y=63,map=64},{x=2,y=79,map=64},{x=40,y=79,map=64},{x=40,y=96,map=64},{x=44,y=96,map=64},{x=44,y=126,map=64},{x=77,y=126,map=64},{x=83,y=135,map=64}};
  drop_items = {};   --掉落道具
  --接任务条件
  accept_cond = 
  {
	{func=check_before_task, args={task_id = 9122}},
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
	{func=check_send_npc, args={id = 42904}},
  };
  --任务完成奖励
  awards = 
  {
	{func=award_expr, args={expr = 0}},
	{func=award_money, args={money = 0,is_free = 0}},
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
	npc_talk ="#td_9123_dialog_accept_npctalk_1",
	buttons = {
        { player_talk = "#td_9123_dialog_accept_player_talk_1_1" ,dest_type = 1 ,dest_window_id = 6 ,func_code = 2},

	  }
	},
	[6] = {
	parent_id = 1,
	window_type = 3,
	npc_talk ="#td_9123_dialog_accept_npctalk_6",
	buttons = {

	  }
	},
  --任务进行对话
	[2] = {
	parent_id = 0,
	window_type = 1,
	npc_talk ="#td_9123_dialog_doing_npctalk_2",
	buttons = {

  }
},
  --完成任务对话
	[3] = {
	parent_id = 0,
	window_type = 1,
	npc_talk ="#td_9123_dialog_end_npctalk_3",
	buttons = {
        { player_talk = "#td_9123_dialog_end_player_talk_3_1" ,dest_type = 1 ,dest_window_id = 4 ,func_code = 0},

  }
},
	[4] = {
	parent_id = 3,
	window_type = 2,
	npc_talk ="#td_9123_dialog_end_npctalk_4",
	buttons = {

  }
},
};
  --对话NPC
  dialog_with_npc = {
[42904] = {
	[1] = {
	parent_id = 0,
	window_type = 4,
	npc_talk ="#td_9123_dialog_npc_42904npctalk_1",
	buttons = {
        { player_talk = "#td_9123_dialog_npc_42904player_talk_1_1" ,dest_type = 2 ,dest_window_id = 0 ,func_code = 10},

  }
},
actions = 
{
	{func=start_send_npc, args={buff_time = 0,buff_id = 0}},
};

};
};
}