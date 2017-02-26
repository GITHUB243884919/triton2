import "coremessage_pb.hxx";
import "property_pb.hxx";

// 以最快速度序列/反序列
option optimize_for = SPEED;

// 把所有消息的ID集中再次，防止重复定义
// task message消息段ID范围(0x1000 -- 0x1400)
enum TASK_MSG_ID
{	
	ID_C2S_REQUEST_OBTAINTASK			= 0x1000; // 客户端->服务器 玩家请求接任务
	ID_C2S_REQUEST_ENDTASK				= 0x1001; // 客户端->服务器 玩家请求交任务
	ID_S2C_NOTICE_OBTAINTASK			= 0x1002; // 服务器->客户端 获得任务通知
	ID_S2C_NOTICE_COMPLETE				= 0x1003; // 服务器->客户端 完成任务通知
	ID_S2C_NOTICE_TASKEND				= 0x1004; // 服务器->客户端 任务结束通知
	ID_S2C_NOTICE_UPDATE				= 0x1005; // 服务器->客户端 更新任务计数
	ID_S2C_NOTICE_RESET					= 0x1006; // 服务器->客户端 任务回滚通知
	ID_C2S_REQUEST_ABORTTASK			= 0x1007; // 客户端->服务器 放弃任务请求
	ID_S2C_NOTICE_TASKLIST				= 0x1008; // 服务器->客户端 给玩家发送身上的任务列表
	ID_S2C_NOTICE_NPCTASKLIST			= 0x1009; // 服务器->客户端 发生NPC绑定的任务列表
	ID_S2C_NOTICE_NPCTASKTIP			= 0x100A; // 服务器->客户端 发送NPC头顶
	ID_C2S_REQUEST_TASK_SEARCH			= 0x100B; // 客户端->服务器 玩家搜索任务请求
	ID_S2C_NOTICE_TASK_SEARCH			= 0x100C; // 服务器->客户端 搜索任务结果
	ID_C2S_INVITE_TASK_REQUEST			= 0x100D; // 客户端->服务器 玩家邀请任务请求
	ID_S2C_INVITE_TASK_NOTICE			= 0x100E; // 服务器->客户端 被邀请玩家收到的消息
	ID_C2S_INVITE_TASK_RESPONSE_REQUEST = 0x100F; // 客户端->服务器 玩家回应任务邀请函
	ID_S2C_INVITE_TASK_RESPONSE_NOTICE  = 0x1210; // 服务器->客户端 发给邀请者对方的回复  0x1010 + 0x0200 
	ID_S2S_DOPLAYER_INVITE_TASK_MSG		= 0x1211; // 0x1011 + 0x0200 {服务器消息}
	ID_S2C_PLAYER_INVITE_TASK_ERROR_MSG = 0x1212; // 0x1012 + 0x0200 发送给客户端，对方不能接受这个任务
	ID_S2C_NOTICE_ABORT_TASK			= 0x1014; // 服务器->客户端 放弃任务通知
	ID_S2C_NOTICE_TASKFAIL				= 0x1015; // 服务器->客户端 任务失败通知
	ID_S2C_NOTICE_TASKOPRFAIL			= 0x1016; // 服务器->客户端 任务操作失败通知
	ID_S2C_NOTICE_NPCSTORAGETASKLIST	= 0x1017; // 服务器->客户端 发生NPC绑定的库任务列表
	ID_C2S_REQUEST_OBTAINSCROLLTASK		= 0x1018; // 客户端->服务器 客户端请求使用仙轴来开启仙轴任务
	ID_S2C_NOTICE_TRAGEDYTASKUPDATE		= 0x1019; // 服务器->客户端 劫难任务列表
	ID_C2S_REQUEST_GETTIP				= 0x101A; // 客户端->服务器 获得指定NPC的头顶
	ID_C2S_REQUEST_ENDTALK				= 0x101B; // 客户端->服务器 玩家完成和某NPC的对话
	ID_S2C_NOTICE_NPCTALKCHANGE			= 0x101C; // 服务器->客户端 重置对话状态
	ID_C2S_REQUEST_LOCKBAG				= 0x101D; // 客户端->服务器 玩家请求锁定包裹
	ID_S2C_LOCKBAG_RESPONSE				= 0x101E; // 服务器->客户端 玩家请求锁定包裹 回应
	ID_S2C_UPDATE_CONDITIONS			= 0x101F; // 服务器->客户端 新版更新任务的消息
	ID_S2C_CLEARTASKNOTIFY				= 0x1020; // 服务器->客户端 清除任务通知
	ID_C2S_REQUEST_TAKEOFFITEM			= 0x1021; // 客户端->服务器 玩家拿掉物品请求
	ID_C2S_REQUEST_GETFAMILYTASK		= 0x1022; // 客户端->服务器 玩家获得活动类任务请求
	ID_S2C_RESPONSE_GETFAMILYTASK		= 0x1023; // 服务器->客户端 玩家获得活动类任务回应
	ID_C2S_REQUEST_GETCORPSTASK			= 0x1024; // 客户端->服务器 玩家获得军团任务请求
	ID_S2C_RESPONSE_GETCORPSTASK		= 0x1025; // 服务器->客户端 玩家获得军团任务回应
	ID_C2S_REQUEST_GETCOUNTRYTASK		= 0x1026; // 客户端->服务器 玩家获得国家任务请求
	ID_S2C_RESPONSE_GETCOUNTRYTASK		= 0x1027; // 服务器->客户端 玩家获得国家任务回应
	ID_C2S_REQUEST_GETCAMPTASK			= 0x1028; // 客户端->服务器 玩家获得阵营任务请求
	ID_S2C_RESPONSE_GETCAMPTASK			= 0x1029; // 服务器->客户端 玩家获得阵营任务回应
	ID_C2S_REQUEST_GETCHAIRMANTASK		= 0x102A; // 客户端->服务器 玩家获得掌门任务请求
	ID_S2C_RESPONSE_GETCHAIRMANTASK		= 0x102B; // 服务器->客户端 玩家获得掌门任务回应
	ID_S2C_NOTIFY_GETTEAMLOOPERR		= 0x102C; // 服务器->客户端 玩家接取组队环任务 错误
	ID_S2C_NOTIFY_UNFINISHTIMEERR		= 0x102D; // 服务器->客户端 玩家尚未到完成任务的时间 错误
	ID_C2S_REQUEST_REFRESHTASK			= 0x102E; // 客户端->服务器 玩家请求刷新任务奖励
	ID_S2C_NOTICE_TASKREFRESHFAIL		= 0x102F; // 服务器->客户端 刷新任务失败通知
	ID_S2C_NOTICE_TASKUNREFRESHED		= 0x1030; // 服务器->客户端 任务不可刷新奖励
	ID_S2C_NOTICE_TASKREFRESHRESULT		= 0x1031; // 服务器->客户端 任务刷新结果品质通知
	ID_S2C_NOTICE_REMOVETASKCOLOR		= 0x1032; // 服务器->客户端 删除本地缓存通知
	ID_S2C_NOTICE_TASKUNREACCEPT		= 0x1033; // 服务器->客户端 任务冷却时间尚未结束通知

	ID_C2S_REQUEST_CHECK_GROUPPUBLISH	= 0x1034; // 客户端->服务器 玩家查看团体发布任务展板
	ID_C2S_REQUEST_PLAYERPUBLISHTASK	= 0x1035; // 客户端->服务器 玩家发布任务请求
	ID_S2C_RESPONSE_CHECK_GROUPPUBLISH	= 0x1036; // 服务器->客户端 回应查看展板
	ID_S2C_RESPONSE_PLAYERPUBLISHTASK	= 0x1037; // 服务器->客户端 玩家发布任务结果

	ID_C2S_REQUEST_CHECK_SINGLEPUBLISH	= 0x1038; // 客户端->服务器 玩家查看个人发布任务信息
	ID_C2S_REQUEST_SINGLEPUBLISHTASK	= 0x1039; // 客户端->服务器 玩家发布个人任务任务请求
	ID_C2S_REQUEST_REFRESHSINGLETASK	= 0x103A; // 客户端->服务器 玩家刷新单个颜色任务
	ID_C2S_REQUEST_OBTAINSINGLETASK		= 0x103B; // 客户端->服务器 玩家接受发布任务请求
	ID_C2S_REQUEST_RECVSINGLEAWARDS 	= 0x103C; // 客户端->服务器 玩家领取个人发布奖励

	ID_S2C_RESPONSE_CHECK_SINGLEPUBLISH	= 0x103D; // 服务器->客户端 查看个人发布信息回应
	ID_S2C_RESPONSE_SINGLEPUBLISHTASK	= 0x103E; // 服务器->客户端 个人发布结果
	ID_S2C_RESPONSE_REFRESHRESULT		= 0x103F; // 服务器->客户端 刷新个人发布任务结果

};


// 任务基础类

message TaskStruct
{
	optional int32 Timer			 = 		1; 			// 任务时限
	repeated uint32 TaskData		 = 		2; 			// 任务数据
	optional uint32 TaskProgress	 = 		3; 			// 任务进度
	repeated uint32	TalkedNpcs		 = 		4; 			// 已经完成的对话NPC
	optional uint32 TaskID			 = 		5; 			// 任务ID
};


// *************************************************************************** //
// 接收任务请求
// *************************************************************************** //

message CMessageObtainTaskRequest
{
	optional uint32 NpcEntityID		 = 		1; 		// 接任务的NPC实体ID
	optional uint32 TaskID			 = 		2; 		// 任务ID
};

// *************************************************************************** //
// 完成任务请求
// *************************************************************************** //

message CMessageTaskEndRequest
{
	optional uint32 TaskID		 = 		1; 		// 任务ID
	optional uint32 NpcEntityID	 = 		2; 		// 交任务的NPC实体ID
	optional uint32 SelectID	 = 		3; 		// 多选奖励选择的物品索引号(1开始)
	optional uint32 BagIndex	 =		4;		// 交付物品的包裹索引
};

message CMessageTaskRefreshRequest
{
	optional uint32 TaskID		 = 		1; 		// 任务ID
	optional uint32 NpcEntityID	 = 		2; 		// 交任务的NPC实体ID
	optional uint32 SelectID	 = 		3; 		// 多选奖励选择的物品索引号(1开始)
	optional uint32 RefreshItemIndex =	4;		// 刷新任务奖励的道具的包裹索引
};


message LoopTaskStruct
{
	optional uint32 TodayMultiLoops  = 		1; 			// 今日已完成的多倍奖励的环数
	optional uint32 WeekMultiLoops   = 		2; 			// 本周已完成的多倍奖励的轮数
	optional uint32 TotalMultiLoops  = 		3; 			// 每日多倍奖励的总环数
	optional uint32 LoopTaskID		 = 		4; 			// 环任务ID
	optional uint32 CompletedLoops	 = 		5; 		    // 已完成轮数
	optional uint32 NextLoopID		 = 		6; 			// 下一环的ID 没接过的时候发1
	optional uint32 LoopID			 = 		7;          // 环ID
	optional uint32 WeekTotalMultiLoops   = 8; 		    // 本周多倍奖励的总轮数
	optional uint32 AwardMultipleToday = 	9; 			// 今日前x轮多倍奖励的倍数
};

// 库任务（悬赏任务描述结构）
message StorageTaskStruct
{
	optional uint32 TodayCompletes		=	1;			// 今日完成次数
	optional uint32 TotalMultiLoops		=	2;			// 每日多倍奖励的次数
	optional uint32 AwardMultipleToday = 	3; 			// 今日前x轮多倍奖励的倍数
};

// *************************************************************************** //
// 获得任务通知
// *************************************************************************** //

message CMessageObtainTaskNotice
{

	optional TaskStruct		Task		 = 		1; 
	optional LoopTaskStruct LoopTask	 = 		2; 
};

// *************************************************************************** //
// 任务完成通知
// *************************************************************************** //

message CMessageTaskCompleteNotice
{
	optional uint32		TaskID  = 		1; 		// 任务ID
};

// *************************************************************************** //
// 任务结束通知
// *************************************************************************** //

message CMessageTaskEndNotice
{
	optional uint32 TaskID  = 		1; 		// 任务ID
	optional StorageTaskStruct StorageTask	=	2;	// 相关库任务数据
};

// *************************************************************************** //
// 更新任务计数通知
// *************************************************************************** //

message CMessageUpdateTaskNotice
{
	optional TaskStruct Task  = 		1; 
};

// *************************************************************************** //
// 任务重置通知
// *************************************************************************** //

message CMessageResetTaskNotice
{
	optional uint32 TaskID  = 		1; 		// 任务ID
};


// *************************************************************************** //
// 放弃任务请求
// *************************************************************************** //

message CMessageAbortTaskRequest
{
	optional uint32 TaskID  = 		1; 			// 任务ID
	optional uint32 NpcEntityID  = 		2;     // 没用
};

// *************************************************************************** //
// 得到玩家身上的任务列表通知
// *************************************************************************** //

message CMessageTaskListNotice
{

	optional bytes		CompletedBuf	 = 		1;   // 已完成任务的buf，和服务器内存中的保持一致
	optional uint32     TaskNum			 = 		2;   // 玩家身上的任务数量
	optional LoopTaskStruct LoopTask	 = 		3;   // 环任务数据
	repeated TaskStruct Tasks			 = 		4;   // 玩家身上的任务列表
	repeated uint32		CompletedTasks	 =		5;	 // 玩家已完成的任务列表(不可重复做的那些)
	optional StorageTaskStruct StorageTask	=	6;	 // 库任务数据结构
	repeated uint32		ChairManSectionIDs	=	7;	 // 掌门任务章节ID列表
	repeated uint32		RepeateTasks			=	8;	 // 玩家已完成的任务列表(可重复做的那些)
	repeated uint32		EndTime				=	9;	 // 任务完成的时间
};


// ********************************************************************** //
// CNpcTaskList
// ********************************************************************** //
//
message NpcTaskList
{
	repeated uint32     ExtendTaskID	 = 		1; 
	optional uint32     CallbackNumber	 = 		2; 
	optional uint32     ExtendNumber	 = 		3; 
	optional uint32     NpcID			 = 		4; 
	repeated uint32     CallbackTaskID	 = 		5; 
};


// *************************************************************************** //
// 得到NPC身上的任务列表
// *************************************************************************** //

message CMessageNpcTaskListNotice
{
	optional NpcTaskList   List  = 		1; 
};


// *************************************************************************** //
// 得到NPC头顶显示
// *************************************************************************** //

message CMessageNpcTaskTipNotice
{
	repeated uint32   NpcEntityID	 = 		1;  // NPC的实体ID
	repeated uint32   Tip			 = 		2; 
	optional uint32   IdType		 = 		3;  // ID类型 0 实体ID  1 模板ID
};


// *************************************************************************** //
// 玩家搜索任务请求
// *************************************************************************** //

message CMessagePlayerSearchRequest
{
};


// *************************************************************************** //
// 玩家搜索任务应答
// *************************************************************************** //

message CMessagePlayerSearchResponse
{
	repeated uint32   TaskIDs	 = 		1;  // NPC的实体ID
};


// *************************************************************************** //
// 玩家邀请任务   客户端->服务器
// *************************************************************************** //

message CMessagePlayerInviteTaskRequest
{
	repeated string   PlayerNames	 = 		1;  // 被邀请的玩家的名字
	optional uint32	  TaskID		 = 		2;  // 任务ID
};


// *************************************************************************** //
// 被邀请玩家收到的消息   服务器->客户端
// *************************************************************************** //

message CMessagePlayerInviteTaskNotice
{
	optional uint32   TaskID             = 		1;  // 任务ID
	optional string   SrcPlayerName      = 		2;  // 邀请者的名字
};



// *************************************************************************** //
// 玩家回应任务邀请函   客户端->服务器
// *************************************************************************** //

message CMessagePlayerInviteTaskResponseRequest
{
	optional uint32	  TaskID         = 		1;  // 任务ID
	optional uint32   AgreeFlag      = 		2;  // 1为同意，0为拒绝
	optional string   SrcPlayerName	 = 		3;  // 邀请者的名字
};

// *************************************************************************** //
// 发给邀请者对方的回复 可能不在同一服务器，于是用路由消息  服务器->客户端
// *************************************************************************** //

message CMessagePlayerInviteTaskResponseNotice
{
	optional CMessageRouter Router			   = 		1;  // 他们不一定在同一个场景上
	optional string			SrcPlayerName      = 		2;  // 被邀请者的名字
	optional uint32			TaskID             = 		3;  // 任务ID
	optional uint32			AgreeFlag          = 		4;  // 1为同意，0为拒绝
};

// *************************************************************************** //
// 发送到玩家所在场景服务器进行处理   服务器->服务器
// *************************************************************************** //

message CMessageDoPlayerInviteTaskMsg
{
	optional CMessageRouter Router		 = 		1;    
	optional uint32	  TaskID             = 		2;  // 任务ID
};


// *************************************************************************** //
// 被邀请者不能做这个任务，通知邀请者。。。   服务器->客户端
// *************************************************************************** //

message CMessagePlayerInviteTaskErrorMsg
{
	optional CMessageRouter Router		 = 		1;    
	optional uint32	  TaskID             = 		2;  // 任务ID
	optional uint32   TypeID			 = 		3;  // 失败类型: 见ETaskInviteRet
	optional string   MyName             = 		4;  // 被邀请者的名字
};


// *************************************************************************** //
// 放弃任务通知
// *************************************************************************** //

message CMessageAbortTaskNotice
{
	optional uint32 TaskID  = 		1; 		// 任务ID
};

// *************************************************************************** //
// 任务失败通知
// *************************************************************************** //

message CMessageTaskFailNotice
{
	optional uint32 TaskID  = 		1; 		// 任务ID
};

// *************************************************************************** //
// 未到交任务时间通知
// *************************************************************************** //

message CMessageUnFinishTaskTime
{
	optional uint32 TaskID		= 		1; 		// 任务ID
	optional uint32 NeedTime	= 		2;		//完成任务还需要的时间
};


// *************************************************************************** //
// 包裹满等情况下引起任务操作失败通知客户端清UI
// *************************************************************************** //



message CMessageTaskOprFailNotice
{
};

// 任务不可刷新
message CMessageTaskUnRefreshNotice
{
	optional uint32 TaskID		= 		1; 		// 任务ID
};


// 任务刷新结果
message CMessageTaskRefreshResult
{
	optional uint32 TaskID		= 		1; 		// 任务ID
	optional uint32 TaskColor	= 		2;		
	optional uint32 TaskNumber	= 		3; 
};

// 通知客户端，删除存储的任务品质
message CMessageRmoveTaskColor
{
	optional uint32 TaskID		= 		1; 		// 任务ID
	optional uint32 TaskColor	= 		2;
};

// 任务刷新失败
message CMessageTaskRefreshFailNotice
{
	optional uint32 TaskID		= 		1; 		// 任务ID
};

// 任务尚未冷却
message CMessageUnReaccpetTask
{
	optional uint32 TaskID		= 		1; 		// 任务ID
	optional uint32 NeedTime	= 		2;
};

// *************************************************************************** //
// 得到NPC身上的库任务列表
// *************************************************************************** //

message CMessageNpcStorageTaskListNotice
{
	optional NpcTaskList   List  = 		1; 
};

// *************************************************************************** //
// 使用仙轴 开启仙轴任务请求
// *************************************************************************** //

message CMessageObtainScrollTaskRequest
{
	optional uint32 ScrollID   = 		1; 		// 仙轴
};

//message TragedyStruct
//{
//	optional uint32		TragedyIndex    = 1; // 劫难任务索引 1开始
//	optional fixed32	NextTragedyTask = 2; // 对应的 下一个劫难子任务ID 可能为0
//	optional uint32		TragedyState	= 3; // 对应的劫难任务状态
//};

// *************************************************************************** //
// 劫难任务 列表/更新消息
// *************************************************************************** //
message CMessageTragedyTaskUpdateNotice
{
	repeated TragedyStruct TragedyInfo   = 		1; 
	optional uint32		   IsLogin		 = 		2;  // 是否登录时候发的  
};


// *************************************************************************** //
// 客户端获得指定NPC的头顶
// *************************************************************************** //

message CMessageGetTipRequest
{
	repeated uint32   NpcEntityID	= 		1;  // NPC的实体ID
	optional uint32   IdType		= 		2;  // ID类型 0 实体ID  1 模板ID
};


// *************************************************************************** //
// 玩家和某NPC对话完成
// *************************************************************************** //

message CMessageEndTalkRequest
{
	optional uint32 NpcEntityID		 = 		1; 	// NPC实体ID
	optional uint32 TaskID			 = 		2; 	// 任务ID
};


// *************************************************************************** //
//  玩家和NPC对话状态改变
// *************************************************************************** //

message CMessageNpcTalkStateChangeNotice
{
	optional uint32 TaskID			 = 		1; 	// 任务ID
	optional uint32 State			 = 		2; 	// 对话状态 1=已对话 0=未对话
	optional uint32 NpcTempID		 = 		3; 	// NPC模板ID
};


// *************************************************************************** //
// 玩家交任务放上去东西 锁定包裹 请求
// *************************************************************************** //

message CMessageLockBaggageRequest
{
	optional uint32 ItemIndex			 = 		1; 	// 索引
};

// *************************************************************************** //
// 玩家交任务放上去东西 锁定包裹 回应
// *************************************************************************** //

message CMessageLockBaggageResponse
{
	optional uint32 ItemIndex			 = 		1; 	// 索引
};


message TaskCondition
{
	optional fixed32 TaskID			 = 		1; 	// 任务ID
	optional fixed32 Condition		 = 		2; 	// 条件枚举
	optional fixed32 TempID			 = 		3; 	// 模板ID
	optional uint32  Num			 = 		4; 	// 怪物的话表示数量  物品该字段无效
};

// *************************************************************************** //
// 通知客户端更新任务 以人为单位 不以任务为单位
// *************************************************************************** //
message CMessageUpdateTaskConditions
{
	repeated  TaskCondition  Contions  = 		1; 
};


// *************************************************************************** //
// 通知客户端 清空
// *************************************************************************** //
message CMessageClearTaskNotify
{

};

// *************************************************************************** //
// 玩家取下交任务放上去的东西 锁定包裹 请求
// *************************************************************************** //

message CMessageTakeOffItemRequest
{
};

// *************************************************************************** //
// 玩家请求家族任务列表
// *************************************************************************** //
message CMessageGetFamilyTaskRequest
{
	repeated uint32 TaskTypes		=	1;		// 任务类型 详见 enTaskGroups 枚举
};

message SingleFamilyTaskResponse
{
	optional uint32 TaskIDs				=	1;		// 任务ID
	optional uint32 TodayTimes			=	2;		// 所属分类当天已完成次数
};

// *************************************************************************** //
// 玩家请求家族任务列表 回应
// *************************************************************************** //
message CMessageGetFamilyTaskResponse
{
	repeated SingleFamilyTaskResponse Tasks	=	1;
};


// *************************************************************************** //
// 玩家请求军团任务列表
// *************************************************************************** //
message CMessageGetCorpsTaskRequest
{
};

// *************************************************************************** //
// 玩家请求军团任务列表 回应
// *************************************************************************** //
message CMessageGetCorpsTaskResponse
{
	repeated SingleFamilyTaskResponse Tasks	=	1;
};



// *************************************************************************** //
// 玩家请求国家任务列表
// *************************************************************************** //
message CMessageGetCountryTaskRequest
{
};

// *************************************************************************** //
// 玩家请求国家任务列表 回应
// *************************************************************************** //
message CMessageGetCountryTaskResponse
{
	repeated SingleFamilyTaskResponse Tasks	=	1;
};


// *************************************************************************** //
// 玩家请求阵营任务列表
// *************************************************************************** //
message CMessageGetCampTaskRequest
{
};

// *************************************************************************** //
// 玩家请求阵营任务列表 回应
// *************************************************************************** //
message CMessageGetCampTaskResponse
{
	repeated SingleFamilyTaskResponse Tasks	=	1;
};

// *************************************************************************** //
// 玩家请求掌门任务列表
// *************************************************************************** //
message CMessageGetChairManTaskRequest
{
	optional uint32		NpcEntityID	= 1; // 正在操作的NPC实体ID
};

// *************************************************************************** //
// 玩家请求掌门任务列表 回应
// *************************************************************************** //

message CMessageGetChairManTaskResponse
{
	repeated SingleChairManTask TaskIDs	=	1;		// 任务ID
};


// *************************************************************************** //
// 玩家接取组队环任务 失败通知
// *************************************************************************** //

message CMessageGetTeamLoopErrNotify
{
	optional string CharName = 1;
	optional uint32 ErrCode  = 2; // 消息码 参见errcode
};


message PBPubslishList
{
	optional uint32 TaskID			= 1;
	optional uint32 TaskState		= 2;
};

//******************************************//
//发布任务消息
//请求
//*******************************************//

// 玩家检查发布任务展板
message CMessageCheckGroupPublishRequest
{
	optional uint32 PublishType		= 1;
};

message CMessagePublishGroupTaskRequest
{
	optional uint32 TaskID			= 1;
	optional uint32 PublishType		= 2;
};

// **************************************
// 发布任务  消息回应


// 查看展板请求的回应
message CMessageCheckGroupPublishResponse
{
	optional uint32 PublishType		= 1;
	repeated PBPubslishList Tasks	= 2;
};

// 玩家发布任务请求的回应

message CMessagePublishGroupTaskResponse
{
	optional uint32 TaskID			= 1;
	optional uint32 PublishType		= 2;
};


// ****************************************//
// 个人发布消息
// ****************************************//

// 查看个人发布
message CMessagePlayerCheckSinglePublish
{
	
};

// 个人发布
message CMessagePlayerSinglePublishTask
{
	optional uint32 TaskColor		= 1;
	optional uint32 ItemIndex		= 2;
};

// 刷新个人发布任务
message CMessageRefreshSingleTaskRequest
{
	optional uint32 TaskColor		= 1;
	optional uint32 ItemIndex		= 2;
};

// 玩家揭榜个人发布任务
message CMessagePlayerGetPublishTaskRequest
{
	optional uint32 TaskColor		= 1;
};

// 领取发布奖励
message CMessagePlayerGetAwardsRequest
{
	optional uint32 TaskColor		= 1;
};

//***************************************
//  消息：服务器->客户端
//	功能：个人发布任务
//***************************************

// single color rank
message PBSingleColorRank
{
	optional uint32 PubNumber		= 1;
	optional uint32 RecvTasks		= 2;
	optional uint32 RecvAwards		= 3;
	optional uint32 CurrentTask		= 4;
};

// check single response 
message CMessageCheckSinglePublishResponse
{
	optional uint32 TodayPublish			= 1;
	optional uint32 OncePublish				= 2;
	optional uint32 TotalFinish				= 3;
	repeated PBSingleColorRank SingleRank	= 4;
};

// single publish response
message CMessagePlayerSinglePublishResponse
{
	optional uint32 TaskColor		= 1;
};

// refresh task response
message CMessageRefreshSingleTaskResponse
{
	optional uint32 TaskColor		= 1;
	optional uint32 TaskID			= 2;
};
