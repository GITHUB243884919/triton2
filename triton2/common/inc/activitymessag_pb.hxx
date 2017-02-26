// 以最快速度序列/反序列
option optimize_for = SPEED;

enum ACTIVITY_MSG_ID
{
	
	
	ID_C2S_REQUEST_CHECKANSWER					= 0x3001;		// 客户端->服务器 发送题目答案	
	ID_S2C_RESPONSE_CHECKANSER					= 0x3002;		// 服务器->客户端 发送题目答案的回复
	ID_C2S_NOTIFY_QUESTIONTIMEREND				= 0x3003;		// 客户端->服务器 本题答题时间到的通知（npc）
	ID_S2C_NOTIFY_QUESTIONNEW					= 0x3004;		// 服务器->客户端 发送新题目的通知
	ID_S2C_NOTIFY_ENDANSWER						= 0x3005;		// 服务器->客户端 答题结束		
	ID_C2S_NOTIFY_USEQUESTIONPAPER				= 0x3006;		// 客户端->服务器 使用题卷的通知（题卷）
	ID_C2S_NOTIFY_IFANSWERNOW					= 0x3007;		// 客户端->服务器 是否马上回答问题(题卷)	
	ID_C2S_NOTIFY_IFBEGINQUEST					= 0x3008;		// 客户端->服务器 是否开始回答npc处的问题(npc)	
	ID_C2S_NOTIFY_STOPANSWER					= 0x3009;		// 客户端->服务器 放弃答题		
	ID_S2C_RESPONSE_USEQUESIONPAPER				= 0x300A;		// 服务器->客户端 使用答题卷的回复消息	
	ID_S2C_NOIIFY_INVESTQUESTION				= 0x300B;		// 服务器->客户端 调查问卷的通知
	ID_C2S_NOTIFY_INVESTANSWER					= 0x300C;		// 客户端->服务器 调查问卷的答案通知	
		
		
	ID_S2C_NOTIFY_THROWHYDRANGEA				= 0x300D;		// 服务器->客户端 抛绣球通知
	ID_S2C_NOTIFY_GRABHYDRANGEAENABLE			= 0x300E;		// 服务器->客户端 可以抢绣球的通知
	
	ID_C2S_REQUEST_GRABHYDRANGEA				= 0x300F;		// 客户端->服务器 抢绣球的请求消息
	ID_S2C_RESPONSE_GRABHYDRANGEA				= 0x3010;		// 服务器->客户端 抢绣球的回应消息
	ID_S2C_NOTIFY_GRABHYDRANGEARESULT			= 0x3011;		// 服务器->客户端 抢绣球的结果通知
	
	ID_S2C_NOTIFY_HYDRANGEAREASULT				= 0x3012;		// 服务器->客户端 抢绣球活动的结果通知
	
	ID_S2C_NOTIFY_STARTHYDRANGEA				= 0x3013;		// 客户端->服务器 抢绣球活动开始的通知
	
	ID_C2S_REQUEST_THROWHYDRANGEA				= 0x3014;		// 客户端->服务器 扔绣球的请求
	ID_S2C_RESPONSE_THROWHYDRANGEA				= 0x3015;		// 服务器->客户端 扔绣球的回应消息	
	
	ID_S2C_NOTIFY_NPCTHROWSUGAR					= 0x3016;		// 服务器->客户端 NPC洒喜糖的消息通知
	
	ID_S2C_NOTIFY_SETHYDRANGEASTATUS			= 0x3017;		// 服务器->客户端 设置玩家绣球状态的通知
	
	ID_S2C_NOTIFY_STARTPRELIMINARY				= 0x3018;		// 服务器->客户端 乡试即将开始的通知	
	ID_S2C_NOTIFY_PRELIMINARYQUESTION			= 0x3019;		// 服务器->客户端 乡试的题目通知		
	ID_S2C_NOTIFY_PRELIMINARYANSWER				= 0x3020;		// 服务器->客户端 乡试的答案通知
	ID_S2C_NOTIFY_ENDPRELIMINARY				= 0x3021;		// 服务器->客户端 结束乡试的通知
	ID_C2S_REQUEST_SUBMITPRELIMINARYANSWER		= 0x3022;		// 客户端->服务器 提交题目答案的消息
	ID_S2C_RESPONSE_SUBMITPRELIMINARYANSWER		= 0x3023;		// 服务器->客户端 提交题目答案的回应消息	
	ID_C2S_REQUEST_VIEWPREMILINARYLIST			= 0x3024;		// 客户端->服务器 查看乡试榜的消息请求
	ID_S2C_RESPONSE_IEWPREMILINARYLIST			= 0x3025;		// 服务器->客户端 查看乡试榜的消息回应				
    // 上缴系统消息
    ID_C2S_HANDIN_REQUEST                       = 0x3026;   // 上缴物品请求
    ID_S2C_HANDIN_RESPONSE                      = 0x3027;   // 上缴物品回应

    // 摇钱树活动
    ID_C2S_GETCAMPRANKFORYQS_REQUEST            = 0x3028;    // 获取摇钱树阵营排名(C->S)
    ID_S2C_GETCAMPRANKFORYQS_RESPONSE           = 0x3029;    // 获取摇钱树阵营排名(S->C)
    ID_S2C_YQSPHASECHANGE_NOTICE                = 0x3030;    // 摇钱树进级通知(S->C)
    ID_S2C_YQSENCOURAGE_NOTICE                  = 0x3031;    // 摇钱树奖励通知(S->C)
    ID_C2S_GETPLAYERRANKFORYQS_REQUEST          = 0x3032;    // 获取摇钱树用户贡献排名(C->S)
    ID_S2C_GETPLAYERRANKFORYQS_RESPONSE         = 0x3033;    // 获取摇钱树用户贡献排名(S->C)
	
	ID_S2C_NOTIFY_QUESTIONVERIFY				= 0x3034;		// 服务器->客户端 需要进行答题验证的通知
	ID_C2S_REQUEST_ANSWERVERIFY					= 0x3035;		// 客户端->服务器 进行答案验证的消息请求
	ID_S2C_RESPONSE_ANSWERVERIFY				= 0x3036;		// 服务器->客户端 进行答案验证的消息回应

    // 摇钱树活动新增加通知
    ID_S2C_YQSDROPITEM_NOTICE                   = 0x3037;    // 摇钱树掉落物品提示(S->C)

	// 在线送好礼消息
	ID_S2C_GIFTTIMER_NOTIFY	                    = 0x3038;    // 通知计时
	ID_C2S_GETGIFT_REQUEST	                    = 0x3039;    // 领取
	ID_S2C_GETGIFT_RESPONSE	                    = 0x303A;    // 领取 回应

    // 摇钱树活动开始消息                       
    ID_S2C_YQSSTART_NOTICE                             = 0x303B;     // 摇钱树活动开始消息(S->C)		 ]
    
    // 聚宝盆消息
    ID_C2S_REQUEST_GETTREASUREBOWLINFO			=	0x303C;     // 获取某个物品的聚宝信息请求
    ID_S2C_RESPONSE_GETTREASUREBOWLINFO			=	0x303D;     // 获取某个物品的聚宝信息的回应消息
    
    ID_C2S_REQUEST_STARTTREASUREBOWL			= 0x303F;     // 开始聚宝的消息请求
    ID_S2C_RESPONSE_STARTTREASUREBOWL			= 0x3040;     // 开始聚宝的消息回应
    ID_S2C_NOTIFY_TREASUREBOWLSUCCESS			= 0x3041;     // 聚宝成功的消息通知
    
};

message CMessageActivityYqsStart
{
    optional uint32 activityid          = 1;    // 摇钱树活动编号
    optional uint32 campid              = 2;    // 阵营编号
};

// ********************************************************************** //
// CMessageGetPlayerRankForYQSRequest 客户端 -> 场景服务器
// 获取用户贡献排行榜
// ********************************************************************** //
message CMessageGetPlayerRankForYQSRequest
{
    optional uint32 playerid            = 1;     // 玩家编号
};

// ********************************************************************** //
// CMessageGetPlayerRankForYQSResponse 场景服务器 -> 客户端
// 获取用户贡献度排行榜
// ********************************************************************** //
message PlayerRankForYQS
{
    optional uint32 rank                = 1;    // 排名
    optional uint32 userid              = 2;    // 用户ID
    optional string username            = 3;    // 用户名
    optional uint32 userlevel           = 4;    // 用户等级
    optional uint32 usermetie           = 5;    // 门派
    optional uint32 Contribute          = 6;    // 贡献度
};

message CMessageGetPlayerRankForYQSResponse
{
    optional uint32  playerid           = 1;        // 玩家编号
    optional uint32  playerhandindawnum = 2;        // 玩家上缴的仙露数
    optional uint32  playercontribute   = 3;        // 玩家的总贡献度
    repeated PlayerRankForYQS playerrankinfo = 4;   // 贡献度排行榜

};

// ********************************************************************** //
// EncourageForHandin 客户端 -> 场景服务器
// 上缴请求
// ********************************************************************** //
message ItemForHandin
{
    optional uint32 itemtype			= 1;	// 物品类型
    optional uint32 itemid				= 2;	// 物品id
    optional uint32 itemnum			    = 3;    // 上缴数
};

message CMessageHandInRequest 
{
    optional uint32  playerId        	= 1;    // 玩家编号
    optional uint32  ActivityId		    = 2;	// 活动编号
    optional uint32  ItemNum			= 3;    // 上缴物品总数
    repeated ItemForHandin Items		= 4;	// 上缴物品ID
};

// ********************************************************************** //
// CMessageHandInResponse 场景服务器 -> 客户端
// 上缴回应
// ********************************************************************** //
message EncourageForHandin
{
    optional uint32 	encouragetype	= 1;    // 奖励类型
    optional uint32 	encourageid		= 2;    // 奖励物品id
    optional uint32	    encouragenum	= 3;	// 奖励值
};

message CMessageHandInResponse
{
    optional uint32  result		    	= 1;    // 返回上缴结果，1为成功，其余为失败。
    optional uint32  playerId        	= 2;    // 玩家编号
    optional uint32  activityId		    = 3;	// 活动编号
    optional uint32  encouragenum		= 4;    // 奖励数
    repeated	 EncourageForHandin encourages = 5; // 奖励
};

// ********************************************************************** //
// CMessageC2SGetCampRankForYQSRequest 客户端 -> 场景服务器
// 获取阵营排名请求
// ********************************************************************** //
message CMessageC2SGetCampRankForYQSRequest
{
    optional uint32  playerId           = 1;    // 玩家编号
    optional uint32  param1             = 2;    // 附属参数
};

// ********************************************************************** //
// CMessageS2CGetCampRankForYQSResponse 场景服务器 -> 客户端
// 获取阵营排名应答
// ********************************************************************** //

message CampRankForYQS
{
    optional uint32 rank				= 1;    // 排名
    optional uint32 campid				= 2;    // 阵营ID
    optional uint32 growinggrade		= 3;    // 成长度
    optional uint32 growingphase		= 4;    // 成长阶段
};

message CMessageS2CGetCampRankForYQSResponse
{
    optional uint32  result             = 1;    // 返回结果，1为成功，其他为失败
    optional uint32  param1             = 2;    // 附属参数
    optional uint32  playerId      	    = 3;    // 玩家编号
    optional uint32  playerhandindawnum = 4;    // 玩家上缴的仙露数
    optional uint32  playercontribute   = 5;    // 玩家的总贡献度
    optional uint32  campnum			= 6;    // 阵营数
    repeated  CampRankForYQS camprankinfo = 7;  // 阵营排名信息
};

// ********************************************************************** //
// CMessageYQSPhaseChangeNotice 场景服务器 -> 客户端
// 摇钱树进级通知
// ********************************************************************** //
message CMessageYQSPhaseChangeNotice
{
    optional uint32 campid				= 1;        // 阵营id
    optional uint32 isfinished			= 2;        // 是否进入进入结算阶段，1为是，0未否
    optional uint32 currphase			= 3;        // 摇钱树当前状态
};

// ********************************************************************** //
// CMessageYQSEncourageChangeNotice 场景服务器 -> 客户端
// 摇钱树奖励通知
// ********************************************************************** //
message CMessageYQSEncourageChangeNotice
{
    optional uint32 campid				= 1;        // 阵营id
    optional uint32 encourangeid		= 2;	    // 表明第几次奖励
};

// ********************************************************************** //
// CMessageYQSEncourageChangeNotice 场景服务器 -> 客户端
// 摇钱树奖励通知
// ********************************************************************** //
message CMessageYQSDropItemNotice
{
    optional uint32 playerid            = 1;        // 人物编号
    optional uint32 itemid				= 2;        // 物品编号
    optional uint32 itemnum		        = 3;	    // 物品数量
};



// 答题的消息


// *************************************************************************** //
// CMessageCheckAnswerRequest  游戏客户端->游戏服务器
// 发送题目答案进行检查的请求
// *************************************************************************** //
message CMessageCheckAnswerRequest
{
	optional uint32	  Answer	  = 		1; 		//  题目答案
	optional uint32   QuestionID  = 		2; 		//  题目 ID	
};

// *************************************************************************** //
// CMessageCheckAnswerResponse  游戏服务器->游戏客户端
// 对题目答案检查结果的回复
// *************************************************************************** //
message CMessageCheckAnswerResponse
{
	optional uint32  QuestionID		 = 		1; 	   // 题目id
	optional uint32  QuestionIndex   = 		2; 	   // 问题索引
	optional uint32  Result			 = 		3; 	   // 答案是否正确1表示正确0表示错误	 
	optional uint32  IfLastQuestion  = 		4; 	   // 是否最后一道题目 1 表示是 0 表示不是
};


// *************************************************************************** //
// CMessageQuestionTimeEnd   游戏客户端->游戏服务器
// 一道题目答题时间结束的通知
// *************************************************************************** //
message CMessageQuestionTimeEnd	
{
	optional uint32  QuestionID	  = 		1; 	   // 题目id
};


// *************************************************************************** //
// CMessageQuestionNew		游戏服务器->游戏客户端
// 新题目的通知
// *************************************************************************** //
message CMessageQuestionNew
{
	optional uint32  QuestionIndex  = 		1; 	   // 题目索引
	optional uint32  QuestionID	    = 		2; 	   // 题目id
};


// *************************************************************************** //
// CMessageAnswerEnd		游戏服务器->游戏客户端
// 所有题目回答结束
// *************************************************************************** //
message CMessageAnswerEnd
{
	optional uint32  EndType	  = 		1; 	   // 结束类型 超时，题目答完	
};


// *************************************************************************** //
// CMessageUseQuestionPaper		游戏客户端->游戏服务器
// 使用题卷的通知
// *************************************************************************** //
message CMessageUseQuestionPaper			  
{
	optional uint32  ItemIndex	  = 		1; 	   // 题卷在包裹里面的索引
};


// *************************************************************************** //
// CMessageIfAnswerNow		游戏客户端->游戏服务器
// 是否马上答题的通知
// *************************************************************************** //
message CMessageIfAnswerNow		  
{	
	optional uint32  Result		  = 		1; 	   // 是否答题 1 表示答题 0 表示不答题
};


// *************************************************************************** //
// CMessageIfBeginQuest		游戏客户端->游戏服务器
// 是否开始npc答题
// *************************************************************************** //
message CMessageIfBeginQuest  
{
	optional uint32  IfBegin	  = 		1; 	   // 是否开始答题 1表示开始 0表示放弃
};

// *************************************************************************** //
// CMessageStopAnser		游戏客户端->游戏服务器
// 放弃答题
// *************************************************************************** //
message CMessageStopAnser
{
	optional uint32  EndType	  = 		1; 	   // 结束类型打断, 主动放弃
};

// *************************************************************************** //
// CMessageUsePaperResponse		游戏客户端->游戏服务器
// 使用答题卷的回复
// *************************************************************************** //
message CMessageUsePaperResponse
{
	optional uint32  Result	  = 		1; 	   // 1.表示可以答题 0.表示不能答题
};

// *************************************************************************** //
// CMessageInvestQuestionNotify		游戏服务器->游戏客户端
// 问卷调查的题目通知
// *************************************************************************** //
message CMessageInvestQuestionNotify
{
	repeated uint32	 QuestionID  = 		1; 	// 题目ID
};

// *************************************************************************** //
// CMessageInvestAnswerNotify		游戏客户端->游戏服务器
// 调查问卷的答案通知
// *************************************************************************** //
message CMessageInvestAnswerNotify
{
	repeated uint32	QuestionID			 = 		1; 
	repeated uint32 QuestionAnswer		 = 		2; 
	optional uint32 IfAnswerNow			 = 		3; 		// 是否现在答题
};


// *************************************************************************** //
// CMessageThrowHydrangeaNotify		服务器->客户端 
// 抛绣球通知			   ID_S2C_NOTIFY_THROWHYDRANGEA
// *************************************************************************** //
message CMessageThrowHydrangeaNotify
{
	optional uint32	ThrowType		= 1;			// 1表示系统抛出的绣球0表示玩家抛出的绣球
	optional string	ThrowerName		= 2;			// 抛绣球的玩家名称
	optional string ReceiverName	= 3;			// 绣球接收者的名称
	optional uint32	SrcPosX			= 4;			// 绣球发送者的横坐标
	optional uint32	SrcPosY			= 5;			// 绣球发送者的纵坐标
	optional uint32	DesPosX			= 6;			// 绣球接收者的横坐标
	optional uint32 DesPosY			= 7;			// 绣球接收者的纵坐标	
};

// *************************************************************************** //
// CMessageGrabHydrangeaEnableNotify		服务器->客户端
// 可以抢绣球的通知		ID_S2C_NOTIFY_GRABHYDRANGEAENABLE
// *************************************************************************** //
message CMessageGrabHydrangeaEnableNotify
{
	optional string	HydrangeaHolder			=	1;			// 绣球持有者姓名	
};

// *************************************************************************** //
// CMessageGrabHydrangeaRequest		客户端->服务器 
// 抢绣球的请求消息		   ID_C2S_REQUEST_GRABHYDRANGEA
// *************************************************************************** //
message CMessageGrabHydrangeaRequest
{
	optional uint32	MsgCode	=	1;				// 无用参数	
};

// *************************************************************************** //
// CMessageGrabHydrangeaResponse		服务器->客户端
// 抢绣球的回应消息			  ID_S2C_RESPONSE_GRABHYDRANGEA
// *************************************************************************** //
message CMessageGrabHydrangeaResponse
{
	optional uint32	Errcode	=	1;			// 抢绣球的错误码	
};

// *************************************************************************** //
// CMessageGrabHydrangeaResultNotify 服务器->客户端
// 抢绣球的结果通知	   ID_S2C_NOTIFY_GRABHYDRANGEARESULT
// *************************************************************************** //
message CMessageGrabHydrangeaResultNotify
{
	optional string	SrcName	=	1;				// 绣球原持有者名称
	optional string DesName	=	2;				// 绣球现有持有者名称
	optional uint32	SrcPosX	=	3;				// 绣球原持有者的横坐标
	optional uint32	SrcPosY	=	4;				// 绣球原持有者的纵坐标
	optional uint32	DesPosX	=	5;				// 绣球现有持有者的横坐标
	optional uint32	DesPosY	=	6;				// 绣球现有持有者的纵坐标			
};

// *************************************************************************** //
// CMessageHydrangeaResultNotify 服务器->客户端
// 抢绣球活动的结果通知		 ID_S2C_NOTIFY_HYDRANGEAREASULT
// *************************************************************************** //
message CMessageHydrangeaResultNotify
{
	optional string RoleName	=	1;			// 绣球获得者的名称
	optional uint32	RoleSex		=	2;			// 性别
};
// *************************************************************************** //
// CMessageStartHydrangeaNotify 服务器->客户端
// 抢绣球活动的开始的通知		 ID_S2C_NOTIFY_STARTHYDRANGEA
// *************************************************************************** //
message CMessageStartHydrangeaNotify
{
	optional uint32	MapID	=	1;				// 地图ID
	optional uint32	PosX	=	2;				// 横坐标
	optional uint32	PosY	=	3;				// 纵坐标
};

// *************************************************************************** //
// CMessageThrowHydrangeaRequest  客户端->服务器 
// 扔绣球的请求	 ID_C2S_REQUEST_THROWHYDRANGEA
// *************************************************************************** //
message CMessageThrowHydrangeaRequest
{
	optional uint32	MsgCode	=	1;			// 无用参数
};

// *************************************************************************** //
// CMessageThrowHydrangeaResponse  服务器->客户端 
// 扔绣球的回应消息	 ID_S2C_RESPONSE_THROWHYDRANGEA
// *************************************************************************** //
message CMessageThrowHydrangeaResponse
{
	optional uint32	Errcode	=	1;			// 扔绣球的回应消息
};

// *************************************************************************** //
// CMessageThrowSugarNotify  服务器->客户端 
// npc散喜糖的消息通知	 ID_S2C_NOTIFY_NPCTHROWSUGAR
// *************************************************************************** //
message CMessageThrowSugarNotify
{
	optional uint32	NpcEntityID	=	1;		// npc的实体ID
};

// *************************************************************************** //
// CMessageSetHydrangeaStatusNotify  服务器->客户端 
// 改变玩家绣球状态的消息通知	  ID_S2C_NOTIFY_SETHYDRANGEASTATUS
// *************************************************************************** //
message CMessageSetHydrangeaStatusNotify
{
	optional uint32	RoleEntityID	= 1;	// 玩家实体ID
	optional uint32	HydrangeaStatus = 2;	// 绣球状态
};

// *************************************************************************** //
// CMessageStartPreliminaryNotify  服务器->客户端 
// 服务器->客户端 乡试即将开始的通知
// *************************************************************************** //
message CMessageStartPreliminaryNotify
{
	
};			

// *************************************************************************** //
// CMessagePreliminaryQuestionNotify  服务器->客户端 
// 乡试的题目通知
// *************************************************************************** //
message CMessagePreliminaryQuestionNotify
{
	optional uint32	QuestionID			=	1;		// 题目ID
	optional uint32	QuestionIndex		=	2;		// 题目索引
};

// *************************************************************************** //
// CMessagePremilinaryAnswerNotify  服务器->客户端
// 乡试的答案通知						  
// *************************************************************************** //
message CMessagePreliminaryAnswerNotify
{
	optional uint32	QuestionID		=	1;		// 题目ID
	optional uint32	QuestionAnswer	=	2;		// 题目答案
	repeated string PlayerInList	=	3;		// 及时榜中的玩家列表
	repeated uint32	ExtraScore		=	4;		// 玩家获得的额外积分
	optional uint32	QuestionIndex	=	5;		// 题目索引
};

// *************************************************************************** //
// CMessageEndPrimilinaryNotify  服务器->客户端
// 结束乡试的通知	 
// *************************************************************************** //
message CMessageEndPreliminaryNotify
{
	
};

// *************************************************************************** //
// CMessageSubmitPremiliynaryAnswerRequest 客户端->服务器
// 提交题目答案的消息		 
// *************************************************************************** //
message CMessageSubmitPreliminaryAnswerRequest
{		
	optional uint32	QuestionAnswer	= 1;	// 题目答案
};

// *************************************************************************** //
// CMessageSubmitPremiliynaryAnswerResponse 服务器->客户端
// 提交题目答案的回应消息		 
// *************************************************************************** //
message CMessageSubmitPreliminaryAnswerResponse
{		
	optional uint32 Errcode =   1;		// 错误码
};

// *************************************************************************** //
// CMessageViewPremilinaryListRequest 客户端->服务器
// 查看乡试榜的消息请求			 
// *************************************************************************** //
message CMessageViewPreliminaryListRequest
{
	optional uint32	NpcID	=	1;		// npc的实体ID
	optional uint32	Type	=	2;		// EPremilinaryListType
};


// *************************************************************************** //
// CMessageViewPremilinaryListRequest 服务器->客户端
// 查看乡试榜的消息回应
// *************************************************************************** //
message PBPreliminaryInfo		
{		
	optional string RoleName		= 1;	// 角色名称
	optional uint32 Score			= 2;	// 得分情况
	optional uint32	RightNum		= 3;	// 答对的题目数量
	optional uint32	TotalNum		= 4;	// 总共答题数	
};
message CMessageViewPreliminaryResponse			
{
	repeated PBPreliminaryInfo PremilinaryInfoList = 1;	// 乡试皇榜个人信息		
	optional uint32			   Type				   = 2;	// EPremilinaryListType
};

// *************************************************************************** //
// CMessageQuestionVerifyNotify 服务器->客户端
// 需要进行答题验证的通知
// *************************************************************************** //
message CMessageQuestionVerifyNotify
{	
	optional uint32 QuestionId = 1; // 问题编号
	optional string Question   = 2; // 问题内容
	optional bytes OptionA     = 3; // 选项A
	optional bytes OptionB     = 4; // 选项B
	optional bytes OptionC     = 5; // 选项C
	optional bytes OptionD     = 6; // 选项D
};

// *************************************************************************** //
// CMessageAnswerVerifyRequest 客户端->服务器
// 进行答案验证的消息请求
// *************************************************************************** //
message CMessageAnswerVerifyRequest
{
	optional uint32 QuestionId = 1; // 问题编号
	optional bytes Answer      = 2; // 回答项
};

// *************************************************************************** //
// CMessageAnswerVerifyResponse 服务器->客户端
// 进行答案验证的消息回应
// *************************************************************************** //
message CMessageAnswerVerifyResponse
{
	optional uint32 Errcode   = 1; // 错误码
};

// *************************************************************************** //
// 服务器->客户端
// 在线送好礼 开始计时通知
// *************************************************************************** //
message CMessageStartGiftTimerNotify
{
	optional uint32 Secs	 = 1; // 计时的秒钟
	optional uint32 GiftId   = 2; // 礼包的模板ID
	optional uint32 GiftType = 3; // 礼包的类型 0-时间 1-等级
};

// *************************************************************************** //
// 客户端->服务器
// 在线送好礼 领取礼包请求
// *************************************************************************** //
message CMessageGetOnlineGiftRequest
{
	optional uint32 GiftType = 1; // 礼包的类型 0-时间 1-等级
};

// *************************************************************************** //
// 服务器->客户端
// 在线送好礼 领取礼包回应
// *************************************************************************** //
message CMessageGetOnlineGiftResponse
{
	optional uint32 RetCode   = 1; // 0 成功 否则失败
	optional uint32 GiftType  = 2; // 礼包的类型 0-时间 1-等级
};

// *************************************************************************** //
// CMessageGetTreasureBowlInfoRequest 客户端->服务器
// 获取某个物品的聚宝信息请求
// *************************************************************************** //
message CMessageGetTreasureBowlInfoRequest
{
	optional uint32	 ItemIndex = 1;		// 要聚宝的物品索引	
};

// *************************************************************************** //
// CMessageGetTreasureBowlInfoResponse 服务器->客户端
// 获取某个物品的聚宝信息的回应消息
// *************************************************************************** //
message CMessageGetTreasureBowlInfoResponse
{
	optional uint32	 Errcode		= 1;		// 错误码
	optional uint32	 ItemUsedTimes	= 2;		// 物品已经聚宝的次数
	optional uint32	 ItemTotalTimes	= 3;		// 物品总共可以聚宝的次数
	optional uint32	 AllUsedTimes	= 4;		// 所有物品的聚宝次数
	optional uint32	 AllTotalTimes	= 5;		// 所有物品的总共聚宝次数
	optional uint32	 TaxMoney		= 6;		// 聚宝需要花费的金钱
	optional uint32	 Index			= 7;		// 物品索引位置
};

// *************************************************************************** //
// CMessageStartTreasureBowlRequest 客户端->服务器
// 开始聚宝的消息请求
// *************************************************************************** //
message CMessageStartTreasureBowlRequest
{
	optional uint32 ItemIndex	=	1;	// 待聚宝物品的索引
	optional uint32	EntityID	=	2;	// npc的实体ID
};

// *************************************************************************** //
// CMessageStartTreasureBowlResponse 客户端->服务器
// 开始聚宝的消息回应
// *************************************************************************** //
message CMessageStartTreasureBowlResponse
{
	optional uint32	Errcode		= 1;	// 错误码
	optional uint32	ItemIndex	= 2;	// 物品索引	
	optional uint32	NewItemID	= 3;	// 新兑换出来的物品ID
};

// *************************************************************************** //
// CMessageTreasureBowlSuccessNotify 服务器->客户端
// 聚宝成功的提示
// *************************************************************************** //
message CMessageTreasureBowlSuccessNotify
{
	optional string	RoleName	=	1;	// 角色名称
	optional uint32	OldItemID	=	2;	// 聚宝前的物品ID
	optional uint32	NewItemID	=	3;	// 聚宝后的物品ID
	optional uint32	ItemNum		=	4;	// 聚宝后的生成物品数量
};
										