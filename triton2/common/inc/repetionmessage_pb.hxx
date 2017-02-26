import "coremessage_pb.hxx";


// 以最快速度序列/反序列
option optimize_for = SPEED;


// ********************************************************************** //
// RepetionMessageID  聊天模块消息id
// 消息ID+0x0200表示该消息为路由消息
// ********************************************************************** //
enum RepetionMessageID
{
	REPETIONMSG                     = 0x2C00;
	ID_S2S_REPETION_TEAM_IN       	= 0x2E01;        //0x2C01 + 0x0200 
	ID_S2C_REPETION_ERR				= 0x2E02;
	ID_S2C_REPETION_ADVENT			= 0x2C03;
	ID_S2C_REPETION_EVENT			= 0x2C04;
	ID_S2C_BATTLE_JOIN_NOTICE		= 0x2C05;
	ID_C2S_BATTLE_JOIN				= 0x2C06;
	ID_S2C_BATTLE_WAIT_RESPONSE		= 0x2C07;
	ID_C2S_BATTLE_TEAM_WAIT_REQUEST = 0x2C08;
	ID_S2C_BATTLE_WAIT_LEAVE_RESPONSE	= 0x2C09;
	ID_C2S_BATTLE_WAIT_LEAVE_REQUEST		= 0x2C0A;
	ID_C2S_REPETION_LEAVE			= 0x2C0B;
	ID_C2S_BATTLE_SINGLE_WAIT_REQUEST = 0x2C0C;
	ID_S2C_REPETIONINDEX_NOTICE		= 0x2C0D;
	ID_S2C_USEITEM_INTO_NOTICE		= 0x2C0E;
	ID_C2S_USEITEM_INTO_RESPINSE	= 0x2C10;
	ID_C2S_REPETION_FUNC			= 0x2C11;
	ID_C2S_BATTLE_TEAM_FIX_WAIT_REQUEST = 0x2C12;
	ID_C2S_BATTLE_SINGLE_FIX_WAIT_REQUEST = 0x2C13;
	ID_C2S_BATTLE_JOIN_RANDOM_PLACE  = 0x2C14;
	ID_S2C_HAVE_SIGN_UP_BATTLE		= 0x2c15;
	ID_S2C_BATTLE_ON_TIME_OPEN		= 0x2C16;
	ID_C2S_JOIN_ON_TIME_OPEN_BATTLE_REQUEST = 0x2C17;
	ID_C2S_GET_RANDOM_TELE_CHARGE_REQUEST = 0x2C18;
	ID_S2C_GET_RANDOM_TELE_CHARGE_RESPONSE = 0x2C19;
	ID_C2S_RANDOM_TELE_REQUEST = 0x2C1A;

	ID_C2S_REQUEST_APPLYGLOBALWAR	=	0x2C1B;	// 跨服战场的报名请求
	ID_S2C_RESPONSE_APPLYGLOBALWAR	=	0x2C1C;	// 跨服战场的报名回应
	ID_C2S_REQUEST_ENTERGLOBALWAR	= 	0x2C1D;	// 玩家进入跨服战场的请求
	ID_S2C_RESPONSE_ENTERGLOBALWAR	= 0x2C1E;	// 玩家进入跨服战场的回应
	ID_S2C_NOTIFY_CANENTERGLOBALWAR = 0x2C1F;	// 玩家可以进入跨服战场的通知
	ID_C2S_REQUEST_LEAVEGLOBALWAR	= 0x2C20;	// 玩家离开跨服战场的消息请求
	ID_S2C_RESPONSE_LEAVEGLOBALWAR	= 0x2C21;	// 玩家离开跨服战场的消息回应

	ID_C2S_REQUEST_KICKCAMPPLAYER= 0x2C22;	// 踢出本方阵营玩家的消息请求
	ID_S2C_RESPONSE_KICKCAMPPLAYER= 0x2C23;	// 踢出本方阵营玩家的消息回应
	ID_S2C_NOTIFY_KICKCAMPPLAYER= 0x2C24;	// 踢出本方阵营玩家的消息通知
	ID_C2S_REQUEST_VOTETOKICKPLAYER=0x2C25;	// 对踢出本方阵营玩家的表决请求
	ID_S2C_RESPONSE_VOTETOKICKPLAYER=0x2C26;	// 对踢出本方阵营玩家的表决回应	
	ID_S2C_NOTIFY_KICKRESULT	  =0x2C27;	// 踢出本方阵营玩家的消息结果
	ID_C2S_INTO_CORPSBOSSREPETION_REQUEST	= 0x2C28;	//进入军团boss副本
	ID_C2S_REQUEST_ENTERLASTREPETION  =  0x2C29;	// 副本重入的消息请求
	ID_S2C_RESPONSE_ENTERLASTREPETION  = 0x2C2A;	// 副本重入的消息回应
};

message CMessageRepetionTeamIn
{	
	optional CMessageRouter Router	 = 		1; 	
	optional uint32 mapid 		 	 = 		2; 
	optional uint32 destcharid   	 = 		3; 
	optional uint32 srccharid	 	 = 		4; 
};

message CMessageRepetionErrCode
{
	optional uint32		MapID		 = 		1; 
	optional uint32		Code		 = 		2; 
	optional uint32		Parame1		 = 		3; 
	optional uint32		Parame2		 = 		4; 
	optional uint32		CharID		 = 		5; 
}

message CMessageRepetionNpcAdventNotice
{
	optional uint32		TemplateID	 = 		1; 
	optional uint32		Time		 = 		2; 
}

message CMessageRepetionEventNotic
{
	optional uint32		EventID		 = 		1; 
	optional uint32		MapID		 = 		2; 
	optional uint32		Parame1		 = 		3; 
	optional uint32		Parame2		 = 		4; 
	optional string		Parame3		 =		5;
}

message CMessageS2CBattleJoinNotice
{
	optional uint32		BattleID	 = 		1; 
	optional uint32		MapID		= 		2;
	optional uint32		Type		= 		3;
	optional uint32		Key			=		4;
}

message CMessageBattleWaitResponse
{
	optional uint32		BattleID	 = 		1; 
	optional uint32		MapID		=		2;
}

message CMessageC2SBattleJoin
{
	optional uint32		BattleID	 = 		1; 
	optional uint32		MapID		= 		2;
}

message CMessageC2SBattleJoinRandomPlace
{
	optional uint32		BattleID	 = 		1; 
	optional uint32		MapID		= 		2;
	optional uint32     Key			=		3;
}

message CMessageBattleTeamWaitRequest
{
	optional uint32		BattleID	 = 		1; 
	optional uint32		CharID		 = 		2; 
	optional uint32		MapID		= 		3;
}

message CMessageBattleSingleWaitRequest
{
	optional uint32		BattleID	=	1;
	optional uint32		CharID		= 	2;
	optional uint32		MapID		=	3;
}

message CMessageC2SBattleWaitLeaveRequest
{
	optional uint32		BattleID	 = 		1; 
	optional uint32		MapID		=		2;
}

message CMessageS2CBattleWaitLeaveResponse
{
	optional uint32		BattleID	 = 		1; 
	optional uint32		MapID		= 2;
}


message CMessageC2SRepetionLeave
{
	
}

message CMessageRepetionIndexNotice
{
	optional uint32		RepetionIndex = 1;
	optional uint32		Type		  = 2;	//（ERepetionIndexType）
	optional uint32		RoleID		  = 3;	// 角色ID
}

message CMessageRepetionUseItemIntoNotice
{
	optional uint32		RepetionIndex = 1;
	optional uint32		ItemID			= 2;
}

message CMessageRepetionUseItemIntoResponse
{

}

message CMessageRepetionFuncPlayerClick
{
	optional uint32 	NpcID			= 1;
}


message CMessageBattleTeamFixWaitRequest
{
	optional uint32		BattleID	 = 		1; 
	optional uint32		CharID		 = 		2; 
	optional uint32		MapID		= 		3;
}

message CMessageBattleSingleFixWaitRequest
{
	optional uint32		BattleID	=	1;
	optional uint32		CharID		= 	2;
	optional uint32		MapID		=	3;
}

message CMessageHaveSignUpBattle
{
	optional uint32		BattleID = 1;
};

message CMessageBattleOnTimeOpenNotice
{
	optional uint32      BattleID = 1;
	optional uint32      MapID = 2;			
};

message CMessageC2SJoinOnTimeOpenBattleRequest
{
	optional uint32		BattleID = 1;
	optional uint32		MapID = 2;
};

message CMessageC2SGetRandomTeleChargeRequest
{
	optional  uint32 NpcID = 1;
	optional  uint32 BattleID = 2;
	optional  uint32 Order = 3;
 };

message CMessageS2CGetRandomTeleChargeResponse
{
	optional uint32 BattleID = 1;
	optional uint32 Charge = 2;
	optional uint32 DieTimes = 3;
};

message CMessageC2SRrandomTeleRequest
{
	optional uint32 NpcID = 1;
	optional uint32 BattleID = 2;
	optional uint32 Order = 3;
 };

// *************************************************************************** //
// CMessageApplyGlobalWarRequest 
// 跨服战场的报名请求
// *************************************************************************** //
message CMessageApplyGlobalWarRequest
{
	optional uint32	RepetionIndex = 1;	// 副本的索引
};

// *************************************************************************** //
// CMessageApplyGlobalWarResponse 
// 跨服战场的报名回应
// *************************************************************************** //
message CMessageApplyGlobalWarResponse
{
	optional uint32	Errcode		= 1;	// 错误码
	optional uint32	MinLevel	= 2;	// 所在等级段的最低等级
	optional uint32	MaxLevel	= 3;	// 所在等级段的最高等级	
};

// *************************************************************************** //
// CMessageEnterGlobalWarRequest 
// 进入跨服战场的消息请求
// *************************************************************************** //
message CMessageEnterGlobalWarRequest
{
	optional uint32 RepetionIndex = 1;
};


// *************************************************************************** //
// CMessageEnterGlobalWarResponse 
// 进入跨服战场的消息回应
// *************************************************************************** //
message CMessageEnterGlobalWarResponse
{
	optional uint32	Errcode = 1;
};


// *************************************************************************** //
// CMessageCanEnterGlobalWarNotify 
// 玩家进入跨服战场的回应
// *************************************************************************** //
message CMessageCanEnterGlobalWarNotify
{
	optional uint32	RepetionIndex = 1;
};

// *************************************************************************** //
// CMessagePlayerLeaveGlobalWarRequest 
// 玩家离开跨服战场的消息请求
// *************************************************************************** //
message CMessagePlayerLeaveGlobalWarRequest
{
	optional uint32	RepetionIndex = 1;
};

// *************************************************************************** //
// CMessagePlayerLeaveGlobalWarResponse 
// 玩家离开跨服战场的消息回应
// *************************************************************************** //
message CMessagePlayerLeaveGlobalWarResponse
{
	optional uint32	ErrCode = 1;
};

// *************************************************************************** //
// CMessageKickCampPlayerRequest ID_C2S_REQUEST_KICKCAMPPLAYER
// 踢出本方阵营玩家的消息请求
// *************************************************************************** //
message CMessageKickCampPlayerRequest
{
	optional uint32 EntityID = 1;		// 被踢出的玩家的实体ID
};

// *************************************************************************** //
// CMessageKickCampPlayerResponse ID_S2C_RESPONSE_KICKCAMPPLAYER
// 踢出本方阵营玩家的消息回应
// *************************************************************************** //
message CMessageKickCampPlayerResponse
{
	optional uint32 ErrCode = 1;		
};

// *************************************************************************** //
// CMessageKickCampPlayerNotify ID_S2C_NOTIFY_KICKCAMPPLAYER
// 踢出本方阵营玩家的消息通知
// *************************************************************************** //
message CMessageKickCampPlayerNotify
{
	optional string SponsorRoleName = 1; // 发起人的角色名称
	optional string RoleName = 2;		 // 被踢玩家的姓名
	optional uint32 EntityID = 3;		 // 被踢玩家的实体ID
};

// *************************************************************************** //
// CMessageVoteToKickPlayerRequest ID_C2S_REQUEST_VOTETOKICKPLAYER
// 对踢出本方阵营玩家的表决请求
// *************************************************************************** //
message CMessageVoteToKickPlayerRequest
{
	optional uint32 EntityID = 1;	// 实体ID
	optional uint32 Result   = 2;	// 表决结果
};

// *************************************************************************** //
// CMessageVoteToKickPlayerResponse ID_S2C_RESPONSE_VOTETOKICKPLAYER
// 对踢出本方阵营玩家的表决回应
// *************************************************************************** //
message CMessageVoteToKickPlayerResponse
{
	optional uint32 Errcode = 1;	// 错误码
};	

// *************************************************************************** //
// CMesssageKickResultNoitfy ID_S2C_NOTIFY_KICKRESULT
// 踢出本方阵营玩家的消息结果
// *************************************************************************** //
message CMesssageKickResultNoitfy
{
	optional uint32	SponsorRoleID	= 1;	// 发起人角色ID
	optional uint32 KickedRoleID	= 2;	// 被踢人的角色ID
	optional string KickedRoleName	= 3;	// 被踢出角色的姓名
	optional uint32	Result			= 4;	// 结果，被踢出成功还是没有成功
};

//************************************************************************** //
//CMessageIntoCorpsBossRepetionRequest 
//
//************************************************************************** //
message CMessageIntoCorpsBossRepetionRequest
{
	optional uint32      BattleID	= 1;				//副本索引
//	optional uint32      MapID		= 2;		
};

//************************************************************************** //
// CMessageEnterLastReptionRequest 
// 副本重入的消息请求
//************************************************************************** //
message CMessageEnterLastReptionRequest
{
	optional uint32 Index = 1;	
	optional uint32	RoleID = 2;	 // 角色ID
};

//************************************************************************** //
// CMessageEnterLastReptionResponse 
// 副本重入的消息回应
//************************************************************************** //
message CMessageEnterLastReptionResponse
{
	optional uint32	 Errcode = 1;
};