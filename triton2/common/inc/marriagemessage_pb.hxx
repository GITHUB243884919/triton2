import "property_pb.hxx";
import "coremessage_pb.hxx";
// 以最快速度序列/反序列
option optimize_for = SPEED;

enum MarriageMessageID
{
	MARRIAGE_MESSAGE = 0x5C00;
	ID_C2S_REQUEST_APPLYMARRY				= 0x5C01;		// 请求结婚的消息请求
	ID_S2C_RESPONSE_APPLYMARRY				= 0x5C02;		// 请求结婚的消息回应
	
	ID_S2C_NOTIFY_MARRYREQUEST				= 0x5C03;		// 请求结婚的消息通知
	ID_C2S_REQUEST_REPLYMARRY				= 0x5C04;		// 答复结婚的消息请求
	ID_S2C_RESPONSE_REPLYMARRY				= 0x5C05;		// 答复结婚的消息回应
	
	ID_S2C_NOTIFY_MARRIAGETIPS				= 0x5C06;		// 婚期提醒
	
	ID_C2S_REQUEST_BEGINWEDDING				= 0x5C07;		// 开始婚礼的消息请求
	ID_S2C_RESPONSE_BEGINWEDDING			= 0x5C08;		// 开始婚礼的消息回应	
	
	ID_S2C_NOTIFY_BEGINWEDDING	 			= 0x5C09;		// 结婚另一方请求开始婚礼的通知

	ID_C2S_REQUEST_REPLYBEGINWEDDING		= 0x5C0A;		// 答复配偶开始婚礼的消息请求
	ID_S2C_RESPONSE_REPLYBEGINWEDDING		= 0x5C0B;		// 答复配偶开始婚礼的消息回应	
	
	ID_S2C_NOTIFY_WEDDINGSTART				= 0x5C0C;		// 婚礼开始的通知(全服通知)	
	
	ID_S2C_NOTIFY_ENTERMARRIAGEHALL			= 0x5C0D;		// 进入礼堂的通知
	ID_C2S_REQUEST_REPLYENTERMARRIAGEHALL	= 0x5C0E;		// 是否同意进入礼堂的答复请求
	ID_S2C_RESPONSE_REPLYENTERMARRIAGEHALL	= 0x5C0F;		// 是否同意进入礼堂的答复回应
	
	ID_C2S_REQUEST_NEWLYWEDENTERHALL		= 0x5C10;		// 新人请求进入礼堂的消息请求
	ID_S2C_RESPONSE_NEWLYWEDENTERHALL		= 0x5C11;		// 新人请求进入礼堂的消息回应
	
	ID_C2S_REQUEST_GUESTENTERHALL			= 0x5C12;		// 拥有请帖的人请求进入礼堂的消息请求	
	ID_S2C_RESPONSE_GUESTENTERHALL			= 0x5C13;		// 拥有请帖的人请求进入礼堂的消息回应
	
	ID_C2S_REQUEST_BAITANG					= 0x5C14;		// 新人拜堂的请求
	ID_S2C_RESPONSE_BAITANG					= 0x5C15;		// 新人拜堂的回应
	
	ID_S2C_NOTIFY_BAITANG					= 0x5C16;		// 新人拜堂的消息通知		
	
	ID_C2S_REQUEST_PUTKEEPSAKE				= 0x5C17;		// 放置信物的消息请求
	ID_S2C_RESPONSE_PUTKEEPSAKE				= 0x5C18;		// 放置信物的消息回应
			
	ID_C2S_REQUEST_EXCHANGKEEPSAKE			= 0x5C19;		// 交换信物的请求
	ID_S2C_RESPONSE_EXCHANGKEEPSAKE			= 0x5C1A;		// 交换信物的回应消息
	
	ID_S2C_NOTIFY_EXCHANGEKEEPSAKE			= 0x5C1B;		// 交换信物的通知
	
	ID_C2S_REQUEST_DIVORCE					= 0x5C1C;		// 离婚的消息请求
	ID_S2C_RESPONSE_DIVORCE					= 0x5C1D;		// 离婚的消息回应
	
	ID_S2C_NOTIFY_DIVORCE					= 0x5C1E;		// 离婚的消息通知	
	
	ID_S2C_NOTIFY_MARRYRESULT				= 0x5C1F;		// 结婚结果通知(对方是否同意)
	
	ID_S2C_NOTIFY_BEGINWEDINGRESULT			= 0x5C20;		// 开始婚礼的结果通知(配偶是否同意)
	
	ID_C2S_REQUEST_GETCAMPMARRAIGEINFO		= 0x5C21;		// 本阵营婚礼信息的消息请求	
	ID_S2C_RESPONSE_GETCAMPMARRIAGEINFO		= 0x5C22;		// 本阵营婚礼信息的消息回应	
	
	ID_S2C_NOTIFY_PERSONMARRIED				= 0x5C23;		// 给全服发送结婚通知	
	
	ID_S2C_NOTIFY_PERSONMARRIAGEINFO		= 0x5C24;		// 个人的婚姻信息通知
	
	ID_S2C_NOTIFY_SPOUSEDIVORCE				= 0x5E25;		// 配偶离婚的通知 0x0200 
	
	ID_C2S_REQUEST_USESYSINVITATION			= 0x5C26;		// 使用系统请帖的请求
	ID_S2C_RESPONSE_USESYSINVITATION		= 0x5C27;		// 使用系统请帖的消息回应
	
	ID_S2C_NOTIFY_TIMETOPUTKEEPSAKE			= 0x5C28;		// 可以放置信物的通知
	
	ID_C2S_REQUEST_CANCELWEDDING			= 0x5C29;		// 取消婚礼的请求
	ID_S2C_RESPONSE_CANCELWEDDING			= 0x5C30;		// 取消婚礼的消息回应	
	
	ID_S2C_NOTIFY_NEWLYMARRIED				= 0x5C31;		// 新人结婚的通知
		
	ID_S2C_NOTIFY_WEDDINGEND				= 0x5C32;		// 婚礼结束的通知
	
	ID_C2S_REQUEST_STARTKEEPSAKEEXCHANGE	= 0x5C33;		// 开始信物兑换的消息请求	
	ID_S2C_RESPONSE_STARTKEEPSAKEEXCHANGE	= 0x5C34;		// 开始信物兑换的消息回应	
	ID_C2S_REQUEST_STOPKEEPSAKEEXCHANGE		= 0x5C35;		// 终止信物兑换的消息请求
	ID_S2C_RESPONSE_STOPKEEPSAKEEXCHANGE	= 0x5C36;		// 终止信物兑换的消息回应
	ID_S2C_NOTIFY_STOPKEEPSAKEEXCHANGE		= 0x5C37;		// 终止信物兑换的消息通知
	ID_C2S_REQUEST_INVITATIONEXCHANGE		= 0x5C38;		// 请帖兑换的消息请求
	ID_S2C_RESPONSE_INVITATIONEXCHANGE		= 0x5C39;		// 请帖兑换的消息回应
	
	ID_S2C_NOTIFY_EXCHANGERESULT			= 0x5C3A;		// 请帖兑换的结果	
	
	ID_C2S_REQUEST_TELETOWEDDINGNPC			= 0x5C3B;		// 传送到婚礼npc的请求消息
	ID_S2C_RESPONSE_TELETOWEDDINGNPC		= 0x5C3C;		// 传送到婚礼npc的消息回应
	
	ID_C2S_REQUEST_REVIEWWEDDING			= 0x5C3D;		// 补办重温婚礼的消息请求
	ID_S2C_RESPONSE_REVIEWWEDDING			= 0x5C3E;		// 补办重温婚礼的消息回应
	ID_S2C_NOTIFY_REVIEWWEDDING				= 0x5C3F;		// 补办重温婚礼的消息通知
};


// ********************************************************************** //
// CMessageApplyMarryRequest	ID_C2S_REQUEST_APPLYMARRY
// 请求结婚的消息请求
// ********************************************************************** //
message CMessageApplyMarryRequest
{
	optional uint32	TimeID				=	1;	// 时间ID
	optional uint32	WeddingModule		=	2;	// 婚礼规模( 0:普通 1:富贵 2:豪华 )
	optional uint32	DayID				=	3;	// 婚礼规模( 0:今天 1:明天 2:后天 )
	optional uint32	HoldWedding			=	4;	// 是否举行婚礼(1表示是，0表示否)
	optional uint32	EntityID			=	5;	// 月老npc的实体ID
};

// ********************************************************************** //
// CMessageApplyMarryResponse	ID_S2C_RESPONSE_APPLYMARRY
// 请求结婚的消息回应
// ********************************************************************** //
message CMessageApplyMarryResponse
{
	optional uint32	Errcode		=	1;			// 错误码
};



// ********************************************************************** //
// CMessageMarryRequestNotify		 ID_S2C_NOTIFY_MARRYREQUEST
// 请求结婚的消息通知
// ********************************************************************** //
message CMessageMarryRequestNotify
{ 
	optional uint32	RoleID				=	1;		// 请求结婚的人的角色ID
	optional string RoleName			=	2;		// 请求结婚的人的角色名称
	optional uint32	Year				=	3;		// 年
	optional uint32	Month				=	4;		// 月
	optional uint32	Day					=	5;		// 日
	optional uint32	BeginTime			=	6;		// 开始时间(整点)	
	optional uint32	WeddingModule		=	7;		// 婚礼的规模		
};


// ********************************************************************** //
// CMessageReplyMarryRequest	ID_C2S_REQUEST_REPLYMARRY
// 答复结婚的消息请求
// ********************************************************************** //
message CMessageReplyMarryRequest
{
	optional uint32	Result		= 1;	// 答复结果 1表示同意结婚 0表示不同意
	optional uint32	EntityID	= 2;	// npc的实体ID
};

// ********************************************************************** //
// CMessageReplyMarryResponse	  ID_S2C_RESPONSE_REPLYMARRY
// 答复结婚的消息回应
// ********************************************************************** //
message  CMessageReplyMarryResponse
{
	optional uint32	Errcode				=	1;	// 错误码
	optional uint32	Result				=	2;	// 是否同意结婚 1表示同意 0表示不同意
	optional string SpouseName			=	3;	// 配偶名称
	optional uint32	MarriageStatus		=	4;	// 结婚状态
};

// ********************************************************************** //
// CMessageMarriageTipsNotify	  ID_S2C_NOTIFY_MARRIAGETIPS
// 婚期提醒
// ********************************************************************** //
message CMessageMarriageTipsNotify
{
	optional uint32 WeddingTime		=	1;		// 结婚日期	
	optional uint32	WeddingStatus	=	2;		// 婚礼状态(EM_WeddingStatus)
};

// ********************************************************************** //
// CMessageBeginWeddingRequest		 ID_C2S_REQUEST_BEGINWEDDING
// 开始婚礼的消息请求
// ********************************************************************** //
message  CMessageBeginWeddingRequest
{
	optional uint32	EntityID	=	1;	// npc实体ID	
};

// ********************************************************************** //
// CMessageBeginWeddinResponse		ID_S2C_RESPONSE_BEGINWEDDING
// 开始婚礼的消息回应
// ********************************************************************** //
message CMessageBeginWeddingResponse
{
	optional uint32	Errcode	=	1;
};

// ********************************************************************** //
// CMessageBeginWeddingNotify	   ID_S2C_NOTIFY_BEGINWEDDING
// 结婚另一方请求开始婚礼的通知
// ********************************************************************** //
message CMessageBeginWeddingNotify
{
	
};

// ********************************************************************** //
// CMessageReplyBeginWeddingRequest
// 答复配偶开始婚礼的消息请求
// ********************************************************************** //
message CMessageReplyBeginWeddingRequest
{
	optional uint32	 Result	=	1;					// 答复结果1表示同意0表示不同意
	optional uint32	 EntityID = 2;					// npc的实体ID
};

// ********************************************************************** //
// CMessageReplyBeginWeddingResponse
// 答复配偶开始婚礼的消息回应
// ********************************************************************** //
message CMessageReplyBeginWeddingResponse
{
	optional uint32	Errcode	=	1;				// 错误码
};	

// ********************************************************************** //
// CMessageWeddingStartNotify
// 婚礼开始的通知(全服通知)
// ********************************************************************** //
message CMessageWeddingStartNotify
{	
	optional string GroomName				= 1;	// 新郎名称
	optional string BrideName				= 2;	// 新娘名称
	optional uint32	WeddingTimeLength		= 3;	// 婚礼时长
	optional uint32	WeddingModule			= 4;	// 婚礼规模
	optional uint32	GroomID					= 5;	// 新郎角色ID
	optional uint32	BrideID					= 6;	// 新娘角色ID
	optional uint32	CampID					= 7;	// 阵营ID
};	

// ********************************************************************** //
// CMessageEnterMarriageHallNotify
// 进入礼堂的通知
// ********************************************************************** //
message  CMessageEnterMarriageHallNotify
{
	
};

// ********************************************************************** //
// CMessageReplyEnterMarriageHallRequest
// 是否同意进入礼堂的答复请求
// ********************************************************************** //
message CMessageReplyEnterMarriageHallRequest
{		
	optional uint32	Result = 1;			// 结果1表示同意0表示不同意	
};

// ********************************************************************** //
// CMessageReplyEnterMarriageHallResponse
// 是否同意进入礼堂的答复回应
// ********************************************************************** //
message CMessageReplyEnterMarriageHallResponse
{
	optional uint32	Errcode	=	1;		// 错误码	
};

// ********************************************************************** //
// CMessageNewlyEnterHallRequest
// 新人请求进入礼堂的消息请求
// ********************************************************************** //
message CMessageNewlyEnterHallRequest
{
	optional uint32	EntityID	=	1;	// npc实体ID	
};

// ********************************************************************** //
// CMessageNewlyEnterHallResponse
// 新人请求进入礼堂的消息回应
// ********************************************************************** //
message CMessageNewlyEnterHallResponse
{
	optional uint32	Errcode			=	1;	// 错误码
	optional uint32	WeddingModule	=	2;	// 婚礼规模
	optional uint32	BaitangEnd		=	3;	// 是否已经拜过堂：1表示拜过0表示没有拜过
};

// ********************************************************************** //
// CMessageGuestEnterHallRequest
// 拥有请帖的人请求进入礼堂的消息请求
// ********************************************************************** //
message CMessageGuestEnterHallRequest
{
	optional uint32	EntityID	=	1;		// npc的实体ID			
};
	
// ********************************************************************** //
// CMessageGuestEnterHallResponse
// 拥有请帖的人请求进入礼堂的消息回应
// ********************************************************************** //
message CMessageGuestEnterHallResponse
{
	optional uint32 Errcode	=	1;			// 错误码	
}	

// ********************************************************************** //
// CMessageBaitangRequest
// 新人拜堂的请求
// ********************************************************************** //
message CMessageBaitangRequest
{
	optional uint32	EntityID = 1;	// npc的实体ID
};

// ********************************************************************** //
// CMessageBaitangResponse
// 新人拜堂的回应
// ********************************************************************** //
message CMessageBaitangResponse
{
	optional uint32	Errcode	=	1;	// 错误码
};

// ********************************************************************** //
// CMessageBaitangNotify
// 新人拜堂的通知
// ********************************************************************** //
message CMessageBaitangNotify
{
	optional string GroomName			=	1;		// 新郎名称
	optional string	BrideName			=	2;		// 新娘名称
	optional uint32	GroomEntityID		=	3;		// 新郎的实体ID
	optional uint32 BrideEntityID		=	4;		// 新娘的实体ID
};		

// ********************************************************************** //
// CMessagePutKeepSakeRequest
// 放置信物的消息请求
// ********************************************************************** //
message CMessagePutKeepSakeRequest
{	
	optional uint32	 KeepsakeIndex = 1;		// 信物索引 -1表示取下信物
};

// ********************************************************************** //
// CMessagePutKeepSakeResponse
// 放置信物的消息回应
// ********************************************************************** //
message CMessagePutKeepSakeResponse
{	
	optional uint32	 Errcode = 1;		// 错误码
	optional uint32	 Index	 = 2;		// 信物索引 
};


// ********************************************************************** //
// CMessageExchangeKeepsakeRequest
// 交换信物的请求
// ********************************************************************** //
message CMessageExchangeKeepsakeRequest
{
		
};

// ********************************************************************** //
// CMessageExchangeKeepsakeResponse
// 交换信物的回应
// ********************************************************************** //
message CMessageExchangeKeepsakeResponse
{
	optional uint32	Errcode	= 1;		// 错误码
};

// ********************************************************************** //
// CMessageExchangeKeepsakeNotify
// 交换信物的通知
// ********************************************************************** //
message CMessageExchangeKeepsakeNotify
{	
	optional string GroomName				= 1;	// 新郎名称
	optional PBItemObject BrideKeepsake		= 2;	// 新娘送给新郎的信物
	optional string BrideName				= 3;	// 新娘名称
	optional PBItemObject GroomKeepsake		= 4;	// 新郎送给新娘的信物
};


// ********************************************************************** //
// CMessageDivorceRequest
// 离婚的消息请求
// ********************************************************************** //
message CMessageDivorceRequest
{
	optional uint32	EntityID	=	1;		// npc的实体ID
};
		 
// ********************************************************************** //
// CMessageDivorceResponse
// 离婚的消息请求
// ********************************************************************** //
message CMessageDivorceResponse
{
	optional uint32	Errcode	=	1;	// 错误码
};

// ********************************************************************** //
// CMessageDivorceResponse
// 离婚的消息请求
// ********************************************************************** //
message CMessageDivorceNotify
{		
	optional string GroomName		=	1;	// 新郎名称
	optional uint32	GroomCharID		=	2;	// 新郎角色ID
	optional string BrideName		=	3;	// 新娘名称
	optional uint32	BrideCharID		=	4;	// 新娘的角色ID	
};

// ********************************************************************** //
// CMessageMarryResultNotify
// 结婚结果通知(对方是否同意)
// ********************************************************************** //
message CMessageMarryResultNotify
{
	optional uint32	Result				= 1;	// 是否同意结婚 1表示同意 0表示不同意
	optional string SpouseName			= 2;	// 配偶名称
	optional uint32	MarriageStatus		= 3;	// 结婚状态
	optional uint32	WeddingTime			= 4;	// 婚礼时间
};

// ********************************************************************** //
// CMessageBeginWeddingResultNofiy
// 开始婚礼的结果通知(配偶是否同意)
// ********************************************************************** //
message CMessageBeginWeddingResultNofiy
{
	optional uint32	Result		= 1;	// 配偶是否同意举行 1表示同意 0表示不同意
	optional string SpouseName	= 2;	// 配偶名称
};

// ********************************************************************** //
// CMessageGetCampMarriageInfoRequest
// 本阵营婚礼信息的消息请求
// ********************************************************************** //	
message CMessageGetCampMarriageInfoRequest
{		
	optional uint32	EntityID	=	1;	// npc的实体ID	
};

// ********************************************************************** //
// CMessageGetCampMarriageInfoResponse
// 本阵营婚礼信息的消息回应
// ********************************************************************** //
message PBMarriageInfo	
{ 
	optional string GroomName			=	1;		// 新郎名称
	optional string BrideName			=	2;		// 新娘名称	
	optional uint32	WeddingTime			=	3;		// 婚礼的举行时间
	optional uint32	WeddingEndTime		=	4;		// 婚礼的结束时间  
	optional uint32	CommoAllowed		=	5;		// 是否能够用于普通婚礼1表示是0表示否
	optional uint32	TimeUsed			=	6;		// 该时间段是否已经被用1表示被用过0否
	optional uint32	TimeID				=	7;		// 改时间段的ID	
};

message PBMarriageList
{
	repeated PBMarriageInfo MarriageInfo	=	1;		// 今天本阵营的婚礼信息
	optional uint32	Year					=	2;		// 年
	optional uint32	Month					=	3;		// 月
	optional uint32	Day						=	4;		// 日
};

message CMessageGetCampMarriageInfoResponse
{
	optional PBMarriageList	MarriageListA	=	1;		// 今天本阵营的婚礼信息
	optional PBMarriageList MarriageListB	=	2;		// 明天本阵营的婚礼信息
	optional PBMarriageList MarriageListC	=	3;		// 后天本阵营的婚礼信息		
};

// ********************************************************************** //
// CMessagePersonMarriedNotify
// 给全服发送结婚通知
// ********************************************************************** //	
message CMessagePersonMarriedNotify
{
	optional string GroomName			= 1;			// 新郎名称
	optional string BrideName			= 2;			// 新娘名称
	optional uint32	ServerSequence		= 3;			// 服务器第几对
};

// ********************************************************************** //
// CMessagePersonMarriageInfoNotify
// 个人的婚姻信息通知
// ********************************************************************** //
message CMessagePersonMarriageInfoNotify
{
	optional string SpouseName		=	1;		// 配偶姓名
	optional uint32	MarriageStatus	=	2;		// 婚姻状态
	optional uint32	WeddingTime		=	3;		// 婚礼的开始时间
	optional uint32	RoleID			=	4;		// 角色ID
};

// ********************************************************************** //
// CMessageSpouseDivorceNotify
// 配偶离婚的通知
// ********************************************************************** //
message CMessageSpouseDivorceNotify
{
	optional CMessageRouter Router  =	1;    	
};

// ********************************************************************** //
// CMessageUseSysInvitationRequest
// 使用系统请帖的请求
// ********************************************************************** //
message CMessageUseSysInvitationRequest
{
	optional uint32	ItemIndex		= 1;		// 系统请帖的索引
	optional string	ReceivedName	= 2;		// 请帖接受人的名字
	optional uint32	ReceivedID		= 3;		// 请帖接受人的角色ID
};

// ********************************************************************** //
// CMessageUseSysInvitationResponse
// 使用系统请帖的请求
// ********************************************************************** //
message CMessageUseSysInvitationResponse
{
	optional uint32	Errcode	=	1;		// 错误码
};

// ********************************************************************** //
// CMessageTimetoPutKeepsakeNotify
// 可以放置信物的通知
// ********************************************************************** //
message CMessageTimeToPutKeepsakeNotify
{ 
	optional uint32	 GroomRoleID = 1;		// 新郎角色ID
	optional uint32	 BrideRoleID = 2;		// 新娘角色ID	
};

// ********************************************************************** //
// CMessageCancelWeddingRequest
// 取消婚礼的请求
// ********************************************************************** //
message CMessageCancelWeddingRequest
{
	optional uint32 EntityID = 1;		// npc实体ID	
};

// ********************************************************************** //
// CMessageCancelWeddingRequest
// 取消婚礼的消息回应
// ********************************************************************** //
message CMessageCancelWeddingResponse
{
	optional uint32 Errcode	= 2;		// 取消婚礼的消息回应 
};
	
// ********************************************************************** //
// CMessageNewlyMarriedNotify
// 新人结婚的通知 全服或者全阵营通知
// ********************************************************************** //
message CMessageNewlyMarriedNotify
{
	optional string  BrideName			= 1;		// 新娘名称
	optional string	 GroomName			= 2;		// 新郎名称
	optional uint32	 WeddingTime		= 3;		// 婚礼时间
	optional uint32	 WeddingTimeLength	= 4;		// 婚礼时长
	optional uint32	 WeddingModule		= 5;		// 婚礼规模	 0 普通 1 富贵 2 豪华
	optional uint32	 CampID				= 6;		// 阵营ID
	optional uint32	 SeverSequence		= 7;		// 全服第几对新人
	optional uint32	 BrideID			= 8;		// 新娘ID
	optional uint32	 GroomID			= 9;		// 新郎ID
};

// ********************************************************************** //
// CMessageWeddingEndNotify
// 婚礼结束的通知
// ********************************************************************** //
message CMessageWeddingEndNotify
{
	
};

// ********************************************************************** //
// CMessageStartKeepsakeExchangeRequest
// 开始信物交换的消息请求
// ********************************************************************** //
message CMessageStartKeepsakeExchangeRequest
{
	optional uint32	EntityID	=	1;	
};

// ********************************************************************** //
// CMessageStartKeepsakeExchangeResponse
// 开始信物兑换的消息回应
// ********************************************************************** //
message CMessageStartKeepsakeExchangeResponse
{
	optional uint32	 Errcode = 1;		
};

// ********************************************************************** //
// CMessageStopKeepsakeExchangeRequest
// 终止信物兑换的消息请求
// ********************************************************************** //
message CMessageStopKeepsakeExchangeRequest
{
	
};

// ********************************************************************** //
// CMessageStopKeepsakeExchangeResponse
// 终止信物兑换的消息回应
// ********************************************************************** //
message CMessageStopKeepsakeExchangeResponse
{
	optional uint32 Errcode = 1;
};

// ********************************************************************** //
// CMessageStopKeepsakeExchangeNotify
// 终止信物兑换的消息通知
// ********************************************************************** //
message CMessageStopKeepsakeExchangeNotify
{	
		
};

// ********************************************************************** //
// CMessageInvitationExchangeRequest
// 请帖兑换的消息请求
// ********************************************************************** //
message CMessageInvitationExchangeRequest
{
	optional uint32	ItemIndex = 1;		// 物品索引
	optional uint32	EntityID  = 2;		// npc的实体ID	
};

// ********************************************************************** //
// CMessageInvitationExchangeResponse
// 请帖兑换的消息回应
// ********************************************************************** //
message CMessageInvitationExchangeResponse
{
	optional uint32	Errcode = 1;		// 物品索引
};

// ********************************************************************** //
// CMessageExchangeResultNotify
// 信物兑换的结果通知
// ********************************************************************** //
message CMessageExchangeResultNotify
{
	optional uint32	GroomRoleID = 1;	// 新郎角色ID
	optional uint32	BrideRoleID = 2;	// 新娘角色ID
	optional uint32	GroomItemID	= 3;	// 新郎给新娘的物品ID
	optional uint32	BrideItemID = 4;	// 新娘给新郎的物品ID
	optional string	BrideName	= 5;	// 新娘名称
	optional string GroomName	= 6;	// 新郎名称
};

// ********************************************************************** //
// CMessageTeleToWeddingNpcRequest
// 传送到结婚npc的消息请求
// ********************************************************************** //
message CMessageTeleToWeddingNpcRequest
{
	
};

// ********************************************************************** //
// CMessageTeleToWeddingNpcRequest
// 传送到结婚npc的消息请求
// ********************************************************************** //
message CMessageTeleToWeddingNpcResponse
{
	optional uint32	 Errcode = 1;
};

// ********************************************************************** //
// CMessageReviewWeddingRequest
// 补办重温婚礼的消息请求
// ********************************************************************** //
message CMessageReviewWeddingRequest
{
	optional uint32	TimeID				=	1;	// 时间ID
	optional uint32	WeddingModule		=	2;	// 婚礼规模( 0:普通 1:富贵 2:豪华 )
	optional uint32	DayID				=	3;	// 婚礼规模( 0:今天 1:明天 2:后天 )	
	optional uint32	EntityID			=	4;	// 月老npc的实体ID
};

// ********************************************************************** //
// CMessageTeleToWeddingNpcRequest
// 补办重温婚礼的消息回应
// ********************************************************************** //
message CMessageReviewWeddingResponse
{
	optional uint32	 Errcode		   =	6;
};

// ********************************************************************** //
// CMessageTeleToWeddingNpcRequest
// 补办重温婚礼的消息通知
// ********************************************************************** //
message CMessageReviewWeddingNotify
{
	optional string  BrideName			= 1;		// 新娘名称
	optional string	 GroomName			= 2;		// 新郎名称
	optional uint32	 WeddingTime		= 3;		// 婚礼时间
	optional uint32	 WeddingTimeLength	= 4;		// 婚礼时长
	optional uint32	 WeddingModule		= 5;		// 婚礼规模	 0 普通 1 富贵 2 豪华
	optional uint32	 CampID				= 6;		// 阵营ID	
	optional uint32	 BrideID			= 7;		// 新娘ID
	optional uint32	 GroomID			= 8;		// 新郎ID
};