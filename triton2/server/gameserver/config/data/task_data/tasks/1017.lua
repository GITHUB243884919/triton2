task = {
  id = 1017,    --任务ID
  kind = 2,    --任务种类
  color = 4,    --任务颜色
  name = "#td_1017_name",  
  desc = "#td_1017_desc",  
  dest = "#td_1017_dest",  
  section = "#td_1017_section",  
  give_npc = 33773,    --发放NPC
  give_map_id = 1,    --发放NPC所在的地图ID
  recv_npc = 33773,    --回收NPC
  recv_map_id = 1,    --回收NPC所在的地图ID
  hard = 0,    --任务难度
  timer = 0,    --任务时限(秒)
  cancelable = 0,    --任务能否被放弃
  repeatable = 0,    --任务成功后可否重做
  retryable = 1,    --任务失败后可否重试
  fail_on_killed = 0,    --玩家被杀死后，是否认为任务失败
  after_task = 0,    --后置任务(自动弹出)
  direct_complete = 1,    --任务是否直接完成(不用找NPC交)
  get_metier_task = 0,    --是否任职任务
  task_level = 10,    --任务适应玩家等级
  send_npc_id = 0,    --要护送的NPC
  max_gird = 0,    --玩家与护送NPC的最大格子数
  is_open_npc = 1,    --是否在接任务时保持和NPC连接(升级任务用)
  is_fetch = 0,    --是否辅助任务(默认否)
  task_type = 1,    --任务类型
  task_hard = 2,    --任务类型及难度
  trace={ 
	"#td_1017_trace1",
	"#td_1017_trace2",
	"#td_1017_trace3",
	"#td_1017_trace4",
	"#td_1017_trace5",
	"#td_1017_trace6",
  }; 
  max_send_time = 0,    --玩家离开护送NPC的最长时间(秒)
  offline_fail = 0,    --是否玩家下线任务就失败
  group_id = 0,    --任务子类别
  is_fail_auto_abort = 0,    --是否失败时自动放弃
  is_share_complete = 0,    --同屏玩家是否分享任务成功
  task_map_id = 1,    --显示已接任务的地图ID
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
	{func=check_metier, args={metier = 7}},
	{func=check_before_task, args={task_id = 1118}},
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
	{func=check_not_metier, args={metier = 7}},
  };
  --任务完成奖励
  awards = 
  {
	{func=award_expr, args={expr = 4000}},
	{func=award_money, args={money = 210}},
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
	npc_talk ="#td_1017_dialog_accept_npctalk_1",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_1_1" ,dest_type = 1 ,dest_window_id = 38 ,func_code = 0},

	  }
	},
	[38] = {
	parent_id = 1,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_38",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_38_1" ,dest_type = 1 ,dest_window_id = 39 ,func_code = 0},

	  }
	},
	[39] = {
	parent_id = 38,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_39",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_39_1" ,dest_type = 1 ,dest_window_id = 40 ,func_code = 0},
        { player_talk = "#td_1017_dialog_accept_player_talk_39_2" ,dest_type = 1 ,dest_window_id = 45 ,func_code = 0},

	  }
	},
	[40] = {
	parent_id = 39,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_40",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_40_1" ,dest_type = 1 ,dest_window_id = 41 ,func_code = 0},
        { player_talk = "#td_1017_dialog_accept_player_talk_40_2" ,dest_type = 1 ,dest_window_id = 44 ,func_code = 0},

	  }
	},
	[41] = {
	parent_id = 40,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_41",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_41_1" ,dest_type = 1 ,dest_window_id = 42 ,func_code = 0},
        { player_talk = "#td_1017_dialog_accept_player_talk_41_2" ,dest_type = 1 ,dest_window_id = 43 ,func_code = 0},

	  }
	},
	[42] = {
	parent_id = 41,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_42",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_42_1" ,dest_type = 1 ,dest_window_id = 50 ,func_code = 0},

	  }
	},
	[43] = {
	parent_id = 41,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_43",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_43_1" ,dest_type = 1 ,dest_window_id = 56 ,func_code = 0},

	  }
	},
	[44] = {
	parent_id = 40,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_44",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_44_1" ,dest_type = 1 ,dest_window_id = 52 ,func_code = 0},

	  }
	},
	[45] = {
	parent_id = 39,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_45",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_45_1" ,dest_type = 1 ,dest_window_id = 46 ,func_code = 0},
        { player_talk = "#td_1017_dialog_accept_player_talk_45_2" ,dest_type = 1 ,dest_window_id = 47 ,func_code = 0},

	  }
	},
	[46] = {
	parent_id = 45,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_46",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_46_1" ,dest_type = 1 ,dest_window_id = 59 ,func_code = 0},

	  }
	},
	[47] = {
	parent_id = 45,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_47",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_47_1" ,dest_type = 1 ,dest_window_id = 48 ,func_code = 0},
        { player_talk = "#td_1017_dialog_accept_player_talk_47_2" ,dest_type = 1 ,dest_window_id = 49 ,func_code = 0},

	  }
	},
	[48] = {
	parent_id = 47,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_48",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_48_1" ,dest_type = 1 ,dest_window_id = 62 ,func_code = 0},

	  }
	},
	[49] = {
	parent_id = 47,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_49",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_49_1" ,dest_type = 1 ,dest_window_id = 65 ,func_code = 0},

	  }
	},
	[50] = {
	parent_id = 42,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_50",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_50_1" ,dest_type = 1 ,dest_window_id = 85 ,func_code = 0},

	  }
	},
	[52] = {
	parent_id = 44,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_52",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_52_1" ,dest_type = 1 ,dest_window_id = 87 ,func_code = 0},

	  }
	},
	[56] = {
	parent_id = 43,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_56",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_56_1" ,dest_type = 1 ,dest_window_id = 86 ,func_code = 0},

	  }
	},
	[59] = {
	parent_id = 46,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_59",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_59_1" ,dest_type = 1 ,dest_window_id = 88 ,func_code = 0},

	  }
	},
	[62] = {
	parent_id = 48,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_62",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_62_1" ,dest_type = 1 ,dest_window_id = 63 ,func_code = 0},

	  }
	},
	[63] = {
	parent_id = 62,
	window_type = 3,
	npc_talk ="#td_1017_dialog_accept_npctalk_63",
	buttons = {

	  }
	},
	[65] = {
	parent_id = 49,
	window_type = 1,
	npc_talk ="#td_1017_dialog_accept_npctalk_65",
	buttons = {
        { player_talk = "#td_1017_dialog_accept_player_talk_65_1" ,dest_type = 1 ,dest_window_id = 66 ,func_code = 0},

	  }
	},
	[66] = {
	parent_id = 65,
	window_type = 3,
	npc_talk ="#td_1017_dialog_accept_npctalk_66",
	buttons = {

	  }
	},
	[85] = {
	parent_id = 50,
	window_type = 3,
	npc_talk ="#td_1017_dialog_accept_npctalk_85",
	buttons = {

	  }
	},
	[86] = {
	parent_id = 56,
	window_type = 3,
	npc_talk ="#td_1017_dialog_accept_npctalk_86",
	buttons = {

	  }
	},
	[87] = {
	parent_id = 52,
	window_type = 3,
	npc_talk ="#td_1017_dialog_accept_npctalk_87",
	buttons = {

	  }
	},
	[88] = {
	parent_id = 59,
	window_type = 3,
	npc_talk ="#td_1017_dialog_accept_npctalk_88",
	buttons = {

	  }
	},
  --任务进行对话
	[2] = {
	parent_id = 0,
	window_type = 1,
	npc_talk ="#td_1017_dialog_doing_npctalk_2",
	buttons = {
        { player_talk = "#td_1017_dialog_doing_player_talk_2_1" ,dest_type = 2 ,dest_window_id = 0 ,func_code = 2},

  }
},
  --完成任务对话
	[3] = {
	parent_id = 0,
	window_type = 1,
	npc_talk ="#td_1017_dialog_end_npctalk_3",
	buttons = {
        { player_talk = "#td_1017_dialog_end_player_talk_3_1" ,dest_type = 1 ,dest_window_id = 84 ,func_code = 0},

  }
},
	[84] = {
	parent_id = 3,
	window_type = 2,
	npc_talk ="#td_1017_dialog_end_npctalk_84",
	buttons = {
        { player_talk = "#td_1017_dialog_end_player_talk_84_1" ,dest_type = 2 ,dest_window_id = 0 ,func_code = 4},

  }
},
};
  --对话NPC
  dialog_with_npc = {
};
}