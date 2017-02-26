import "familydbinfo_pb.hxx";
// 以最快速度序列/反序列
option optimize_for = SPEED;

enum FamilyMessageID
{
	ID_C2S_REQUEST_CREATFAMILY				= 	0x3400;			// 创建家族的请求
	ID_S2C_RESPONSE_CREATFAMILY				=   0x3401;			// 创建家族的回应 
											
	ID_C2S_REQUEST_INVITEMEMBER				=	0x3402;			// 邀请玩家加入家族
	ID_S2C_RESPONSE_INVITEMEMBER			=	0x3403;			// 邀请加入的回应
	ID_S2C_NOTIY_FAMILYINVITED				=	0x3404;			// 被别人邀请的通知	
	ID_C2S_REQUEST_REPLYFAMILYINVITE		=	0x3405;			// 答复家族邀请的请求
	ID_S2C_RESPONSE_REPLYFAMILYINVITE		=	0x3406;			// 答复家族邀请的回复
	ID_S2C_NOTIFY_INVITERESULT				=	0x3407;			// 邀请结果的通知
											
	ID_C2S_REQUEST_FAMILYAPPLY				=	0x3408;			// 申请加入家族	
	ID_S2C_RESPONSE_FAMILYAPPLY				=	0x3409;			// 申请加入家族的回应	
	ID_S2C_NOTIFY_MEMBERAPPLY				=	0x340A;			// 发给家族管理者玩家请求加入家族的通知
	ID_C2S_REQUEST_REPLYMEMBERAPPLY			=	0x340B;			// 回复玩家申请加入家族的请求
	ID_S2C_RESPONSE_REPLYMEMBERAPPLY		=	0x340C;			// 回复玩家申请加入家族的回应
	ID_S2C_NOTIFY_FAMILYAPPLYSUCESS			=	0x340D;			// 通知玩家申请加入家族成功
	ID_S2C_NOTIFY_FAMILYMEMBERADDED			=	0x340E;			// 通知家族成员有新成员加入（邀请和申请通用）	
	
	ID_C2S_REQUEST_POSTAPPOINT				=	0x340F;			// 家族管理者职位任免的请求
	ID_S2C_RESPONSE_POSTAPPOINT				=	0x3410;			// 家族管理者职位任免的回应
	ID_S2C_NOTIFY_POSTAPPOINT				=	0x3411;			// 家族管理者任命职位的通知
		
	ID_C2S_REQUEST_LEAVEFAMILY				=	0x3412;			// 家族成员离开家族的请求
	ID_S2C_RESPONSE_LEAVEFAMILY				=	0x3413;			// 家族成员离开家族的回应
	ID_S2C_NOTIFY_MEMBERLEAVEFAMILY			=	0x3414;			// 家族成员离开家族的通知	 
	
	ID_C2S_REQUEST_GETFAMILYMEMBERINFO		=	0x3415;			// 获取家族成员信息的请求
	ID_S2C_RESPONSE_GETFAMILYMEMBERINFO		=	0x3416;			// 获取家族成员信息的回应
	ID_C2S_REQUEST_GETFAMILYPROPERTY		=	0x3417;			// 获取家族属性信息的请求
	ID_S2C_RESPONSE_FAMILYPROPERTY			=	0x3418;			// 获取家族信息的回应：家族属性信息
	
	
	ID_C2S_REQUEST_GETALLFAMILYINFO			=	0x3419;			// 获取所有家族的基本信息									 				
	ID_S2C_RESPONSE_GETALLFAMILYINFO		=	0x341A;			// 获取所有家族基本信息的回复消息
	
	
	ID_C2S_REQUEST_GETAPPLYLIST				=	0x341B;			// 获取申请列表
	ID_S2C_RESPONSE_GETAPPLYLIST			=	0x341C;			// 发送申请列表
	
	ID_C2S_REQUEST_KICKMEMBER				=	0x341D;			// 宗正踢人的请求
	ID_S2C_RESPONSE_KICKMEMBER				=	0x341E;			// 宗正踢人的回应
	ID_S2C_NOTIFY_KICKPLAYER				=	0x341F;			// 宗正踢人的通知
	
	ID_C2S_REQUEST_DISBANDFAMILY			=	0x3420;			// 解散家族的请求
	ID_S2C_RESPONSE_DISBANDFAMILY			=	0x3421;			// 解散家族的回应
	ID_S2C_NOTIFY_DISBANDFAMILY				=	0x3422;			// 解散家族的通知
	
	ID_C2S_REQUEST_CHANGENOTICE				=	0x3423;			// 改变家族公告的请求
	ID_S2C_RESPONSE_CHANGENOTICE			=	0x3424;			// 改变家族公告的回应
	
	ID_C2S_REQUEST_APPOINTHEADER			=	0x3425;			// 宗正转让职位的请求
	ID_S2C_RESPONSE_APPOINTHEADER			=	0x3426;			// 宗正转让职位的回应消息
	ID_S2C_NOTIFY_CHANGERHEADER				=	0x3427;			// 宗正转让职位的通知	
	
	ID_S2C_NOTIFY_FAMILYPEROPERTY			=	0x3428;			// 家族属性信息通知
	
	ID_C2S_REQUEST_CLEARFAMILYAPPLYLIST		=	0x3429;			// 清空家族的申请列表的消息请求
	ID_S2C_RESPONSE_CLEARFAMILYAPPLYLIST	=	0x342A;			// 清空家族申请列表的消息回应
	
	ID_S2C_NOTIFY_JOINCORPS					=	0x342B;			// 家族加入军团的消息通知
	
	ID_C2S_REQUEST_CANCELFAMILYDISBAND		=	0x342C;			// 取消家族解散的消息请求
	ID_S2C_RESPONSE_CANCELFAMILYDISBAND		=	0x342D;			// 取消家族解散的消息回复
	
	ID_S2C_NOTIFY_CHANGEFAMILYNOTICE		=	0x342E;			// 改变家族公告的消息通知
	
	ID_C2S_REQUEST_REFRESHFAMILYINFO		=	0x342F;			// 刷新家族成员信息的请求
	ID_S2C_RESPONSE_REFRESHFAMILYINFO		=	0x3430;			// 刷新家族成员信息的回应消息
	
	

	ID_C2S_REQUEST_BIDNPC					=	0x3431;			// 族长请求争夺NPC
	ID_S2C_RESPONSE_BIDNPC					=	0x3432;			// 争夺NPC回应

	ID_S2C_NOTIFY_MEMBERSUBMIT				=	0x3433;			// 争夺报名通知
	ID_C2S_REQUEST_MEMBERSUBMIT				=	0x3434;			// 族长报名请求
	ID_S2C_RESPONSE_MEMBERSUBMIT			=	0x3435;			// 族长报名回应

	ID_C2S_REQUEST_ABORTNPC					=	0x3436;			// 族长放弃NPC请求
	ID_S2C_RESPONSE_ABORTNPC				=	0x3437;			// 族长放弃NPC回应

	ID_C2S_REQUEST_BIDINFO					=	0x3438;			// 查询争夺请求
	ID_S2C_RESPONSE_BIDINFO					=	0x3439;			// 查询争夺回应
	
	ID_C2S_REQUEST_SETFAMILYMEMBERPOST		=	0x343A;			// 设置家族成员职位的消息请求
	ID_S2C_RESPONSE_SETFAMILYMEMBERPOST		=	0x343B;			// 设置家族成员职位的消息消息回应
	
	ID_C2S_REQUEST_LEVELUPFAMILY			=	0x343C;			// 升级家族的请求
	ID_S2C_RESPONSE_LEVELUPFAMILY			=	0x343D;			// 升级家族的回应
	
	ID_C2S_REQUEST_CONTRIBUTEFAMILYMONEY	=	0x343E;			// 捐献家族金钱的消息请求
	ID_S2C_RESPONSE_CONTRIBUTEFAMILYMONEY	=	0x343F;			// 捐献家族金钱的消息回应
	
	ID_C2S_REQUEST_OBTAINFAMILYMONEY		=	0x3440;			// 从占领npc处领取家族金钱的消息请求
	ID_S2C_RESPONSE_OBTAINFAMILYMONEY		=	0x3441;			// 从占领npc处领取家族金钱的消息回应
	
	ID_S2C_NOTIFY_SETFAMILYPOST				=	0x3442;			// 设置家族职位的通知
	ID_S2C_NOTIFY_LEVELUPFAMILY				=	0x3443;			// 家族升级的通知	

	ID_S2C_NOTIFY_FAMILYSUBMIT				=	0x3444;			// 通知家族报名
	ID_S2C_NOTIFY_BATTLERESULT				=	0x3445;			// 通知战场结果
	
	ID_S2C_NOTIFY_FAMILYPVPSTARLEVELUP		=	0x3446;			// PVP星级升级的通知		
																// 空缺一个消息
	ID_C2S_REQUEST_MAKEROOM					=	0x3448;			// 开设房间请求
	ID_S2C_RESPONSE_MAKEROOM				=	0x3449;			// 开设房间回应
	ID_C2S_REQUEST_CHAROOM					=	0x344A;			// 挑战房间请求
	ID_S2C_RESPONSE_CHAROOM					=	0x344B;			// 挑战房间回应
	ID_C2S_REQUEST_AGREEBATTLE				=	0x344C;			// 同意挑战请求
	ID_S2C_RESPONSE_AGREEBATTLE				=	0x344D;			// 同意挑战回应
	ID_C2S_REQUEST_QUERYALLROOM				=	0x344E;			// 查询房间列表请求
	ID_S2C_RESPONSE_QUERYALLROOM			=	0x344F;			// 查询房间列表回应
	ID_C2S_REQUEST_QUERYMYROOM				=	0x3450;			// 查询自己的房间请求
	ID_S2C_RESPONSE_QUERYMYROOM				=	0x3451;			// 查询自己的房间回应
	ID_C2S_REQUEST_QUERYMYCHA				=	0x3452;			// 查询自己的挑战请求
	ID_S2C_RESPONSE_QUERYMYCHA				=	0x3453;			// 查询自己的挑战回应
	ID_C2S_REQUEST_ABORTROOM				=	0x3454;			// 放弃房间请求
	ID_S2C_RESPONSE_ABORTROOM				=	0x3455;			// 放弃房间回应
	ID_C2S_REQUEST_ABORTCHA					=	0x3456;			// 放弃挑战请求
	ID_S2C_RESPONSE_ABORTCHA				=	0x3457;			// 放弃挑战回应	   

	ID_S2C_NOTIFY_CONTRIBUTEFAMILYMONEY		=	0x3458;			// 贡献家族金钱的通知

	ID_S2C_NOTIFY_FAMILYMONEYCHANGED		=	0x3459;			// 家族金钱改变的消息通知	
	ID_S2C_NOTIFY_USEFAMILYEXPCARD			=	0x345A;			// 使用经验令牌的通知	


	ID_C2S_REQUEST_STARTFAMILYBOSS			=	0x345B;			// 开通家族boss活动的请求
	ID_S2C_RESPONSE_STARTFAMILYBOSS			=	0x345C;			// 开通家族boss活动的回应 	
	ID_C2S_REQUEST_ENTERBOSSREPTION			=	0x345D;			// 成员进入家族副本的请求
	ID_S2C_RESPONSE_ENTERBOSSREPTION		=	0x345E;			// 成员进入家族副本的消息回应

	ID_S2C_NOTIFY_MAKEROOM					=	0x345F;			// 开设房间通知
	ID_S2C_NOTIFY_ABORTROOM					=	0x3460;			// 取消房间通知
	ID_S2C_NOTIFY_CHAROOM					=	0x3461;			// 挑战房间通知	
	ID_S2C_NOTIFY_ABORTCHA					=	0x3462;			// 取消挑战通知 
	
	ID_S2C_NOTIFY_FAMILYGLORYCHANGED		=	0x3463;			// 家族荣耀的改变的通知
	ID_S2C_NOTIFY_MEMBERCONTRIBUTECHANGED	=	0x3464;			// 改变个人贡献度的通知	 
	
	ID_C2S_REQUEST_QUERYNPCWELFARE			=	0x3465;			// 查询npc福利的请求（荣耀和金钱）
	ID_S2C_RESPONSE_QUERYNPCWELFARE			=	0x3466;			// 查询npc福利的回应消息（荣耀和金钱）
	
	ID_C2S_REQUEST_GETFAMILYMONEY			=	0x3467;			// 获取家族金钱到个人的消息请求
	ID_S2C_RESPONSE_GETFAMILYMONEY			=	0x3468;			// 获取家族金钱到个人的消息回应

	ID_S2C_NOTIFY_ABORTNPC					=	0x3469;			// 族长放弃NPC通知
	ID_S2C_NOTIFY_PREBATTLE					=	0x346A;			// 争夺战战斗前通知
	
	ID_C2S_REQUEST_GETEXPCARD				=	0x346B;			// 获取经验令牌的请求
	ID_S2C_RESPONSE_GETEXPCARD				=	0x346C;			// 获取经验令牌的回应消息
	
	ID_S2C_NOTIFY_FAMILYCANLEVELUP			=	0x346D;			// 家族可以升级的通知消息
					
	ID_S2C_NOTIFY_FAMILYREPETIONSCORE		=	0x346E;			// 家族副本的评分
	
	ID_C2S_REQUEST_GETFAMILYGOODSLIST		=	0x346F;			// 获取家族商品列表的消息请求
	ID_S2C_RESPONSE_GETFAMILYGOODSLIST		=	0x3470;			// 获取家族商品列表的消息回应
	
	ID_C2S_REQUEST_GETFAMILYEXCHANGEGOODSLIST	= 0x3471;		// 获取家族兑换商品列表的消息请求
	ID_S2C_RESPONSE_GETFAMILYEXCHANGEGOODSLIST 	= 0x3472;		// 获取家族兑换商品列表的消息回应
	
	ID_S2C_NOTIFY_CHANGEFAMILYGOODSLIST				= 0x3473;		// 改变商品列表的通知
	ID_S2C_NOTIFY_CHANGEFAMILYEXCHANGEGOODSLIST		= 0x3474;		// 改变兑换列表的通知 
	ID_S2C_NOTIFY_USEFAMILYLEAGUEITEM				= 0x3475;		// 使用家族特供物品的通知

	ID_C2S_REQUEST_GETHOMEINFO				=	0x3476;			// 获得家园信息请求
	ID_S2C_RESPONSE_GETHOMEINFO				=	0x3477;			// 获得家园信息回应

	ID_C2S_REQUEST_UPGRADEHOME				=	0x3478;			// 升级开通家园建筑物请求
	ID_S2C_RESPONSE_UPGRADEHOME				=	0x3479;			// 升级开通家园建筑物回应
	
	ID_S2C_NOTIFY_FAMILYSTABILITYCHANGED	=	0x347A;			// 家族安定度改变的通知
	
};									   

// *************************************************************************** //
//  消息头：把客户端发往场景服务器的消息转成发给网关的消息( 服务器用 )
// *************************************************************************** //
message FamilyHead
{
	optional uint32   RoleID	=	1;				// 角色ID
	optional uint32   MsgID     =	2;			    // 消息ID
	optional uint32	  EntityID	=	3;				// 在场景中的实体ID
};


// *************************************************************************** //
// CMessageCreatFamilyRequest  
// 玩家创建家族的请求
// *************************************************************************** //
message CMessageCreatFamilyRequest
{
	optional  FamilyHead	HeadMsg		= 1;	    // 消息头服务器用
	optional  string		FamilyName	= 2;		// 家族名称	
	optional  uint32		NpcEntityID = 3;		// 家族NPC的实体ID
};

// *************************************************************************** //
// CMessageCreatFamilyResponse  
// 玩家创建家族的回应
// *************************************************************************** //
// TODO:附带家族属性
message CMessageCreatFamilyResponse
{
	optional FamilyHead	HeadMsg			= 1;	    // 消息头服务器用
	optional uint32 ErrCode				= 2;			// 0 表示成功其他表示错误码
	optional uint32 FamilyID			= 3;			// 成功时的家族ID
	optional string FamilyName			= 4;			// 成功时的家族名称
};

// *************************************************************************** //
// CMessageInviteMemberRequest  
// 邀请周围玩家加入家族
// *************************************************************************** //
message CMessageInviteMemberRequest
{
	optional FamilyHead	HeadMsg			= 1;	    // 消息头服务器用
	optional uint32 RoleID				= 2;		// 受邀请玩家的实体ID	
	optional string RoleName			= 3;		// 受邀请的玩家的角色名(角色id和角色名只能使用一个参数)	
}; 

// *************************************************************************** //
// CMessageInviteMemberResponse  
// 邀请玩家加入家族的回应消息
// *************************************************************************** //
message CMessageInviteMemberResponse
{
	optional FamilyHead	HeadMsg			= 1;	    // 消息头服务器用
	optional uint32 Errcode				= 2;			// 0表示成功其他表示错误码
	optional string RoleName			= 3;			// 邀请的玩家的名字  	
};

// *************************************************************************** //
// CMessageFamilyInviteNotify  
// 被邀请加入家族的消息通知
// *************************************************************************** //
message CMessageFamilyInvitedNotify
{
	optional FamilyHead	HeadMsg			= 1;			// 消息头服务器用
	optional string PlayerName			= 2;			// 邀请者的姓名
	optional string FamiltyName			= 3;			// 邀请者所在家族名称
	optional uint32 RoleID				= 4;			// 邀请者的角色ID
	optional uint32 FamilyID			= 5;			// 邀请者的家族ID	
	optional uint32 FamilyLevel			= 6;			// 家族等级
	optional uint32 MemberNum			= 7;			// 家族成员数量	
	optional string CorpsName			= 8;			// 家族所在军团名称	     	
	optional uint32	Glory				= 9;			// 家族的荣耀值
	optional uint32	StarLevel			= 10;			// 家族星级
	optional uint32 NpcID				= 11;			// 家族npc
	optional string	FamilyHeaderName	= 12;			// 家族族长名称
	optional uint32	UnionID				= 13;			// 联盟ID
	optional string	UnionName			= 14;			// 联盟名称
};

// *************************************************************************** //
// CMessageReplyFamilyInviteRequest  
// 答复家族邀请的请求
// *************************************************************************** //
message CMessageReplyFamilyInviteRequest
{				 
	optional FamilyHead	HeadMsg			= 1;				// 消息头服务器用
	optional uint32 Result				= 2;				// 1表示同意加入家族0表示不同意2表示忽略
	optional uint32	FamilyID			= 3;				// 家族ID
	optional uint32 InvitingRoleID		= 4;				// 邀请的玩家的角色ID	
};

// *************************************************************************** //
// CMessageReplyFamilyInviteResponse
// 答复家族邀请的请求
// *************************************************************************** //
message CMessageReplyFamilyInviteResponse
{				 
	optional FamilyHead	HeadMsg			= 1;	    // 消息头服务器用
	optional uint32 Errcode				= 2;		// 0表示加入成功其他表示错误码
	optional uint32	FamilyID			= 3;		// 家族ID
	optional string	FamilyName			= 4;		// 家族名称
	optional uint32	Result				= 5;		// 1表示同意0表示拒绝
	optional uint32 CorpsID				= 6;		// 家族所在的军团ID
	optional string	CorpsName			= 7;		// 军团名称
	optional uint32	FamilyHeaderID		= 8;		// 家族首领ID
	optional uint32 CorpsHeaderID		= 9;		// 军团首领ID
	optional uint32	UnionID				= 10;		// 军团联盟ID
	optional string UnionName			= 11;		// 军团联盟名称
};

// *************************************************************************** //
// CMessageApplyFamilyRequest  
// 通过周围玩家或者家族ID申请加入家族
// *************************************************************************** //
message CMessageApplyFamilyRequest
{
	optional FamilyHead	HeadMsg			= 1;			// 消息头服务器用
	optional uint32 CharID				= 2;			// 目标玩家角色ID
	optional uint32	FamilyID			= 3;			// 家族ID( 实体ID和家族id只能有一个 )
	optional string FamilyName			= 4;			// 家族名称( 以后要用 )																									
};	

// *************************************************************************** //
// CMessageApplyFamilyResponse  
// 申请加入家族的回复消息
// *************************************************************************** //
message CMessageApplyFamilyResponse
{
	optional FamilyHead	HeadMsg	= 1;	    // 消息头服务器用
	optional uint32	Errcode		= 2;		// 0表示成功其他表示错误码
	optional string	FamilyName	= 3;		// 家族名称
	optional uint32 FamilyID	= 4;		// 家族ID  	
};

// *************************************************************************** //
// CMessageMemberApplyNotify  
// 发给家族管理者有人申请加入家族的通知
// *************************************************************************** //
message CMessageMemberApplyNotify
{
	optional FamilyHead	HeadMsg			= 1;			// 消息头服务器用
	optional string PlayerName			= 2;			// 申请人的姓名
	optional uint32	RoleID				= 3;			// 申请人的角色ID
	optional uint32	RoleLevel			= 4;			// 申请人的等级
	optional uint32	RoleMetier			= 5;			// 申请人的职业
	optional uint32	FaceType			= 6;			// 脸型	
	optional uint32 RoleSex		        = 7;			// 性别
	optional uint32 LineID				= 8;			// 国家ID
	optional uint32 MapID				= 9;			// 地图ID
};

// *************************************************************************** //
// CMessageReplyMemberApplyRequest  
// 答复玩家申请加入家族的请求
// *************************************************************************** //
message CMessageReplyMemberApplyRequest
{
	optional FamilyHead	HeadMsg			=	1;				// 消息头服务器用
	optional uint32 Result				=	2;				// 回复结果1表示批准加入0表示不同意加入2表示忽略
	optional uint32 RoleID				=	3;				// 申请人的角色ID
};


// *************************************************************************** //
// CMessageReplyMemberApplyResponse  
// 答复玩家申请加入家族的回复
// *************************************************************************** //
message CMessageReplyMemberApplyResponse  
{
	optional FamilyHead	HeadMsg			=	1;			// 消息头服务器用
	optional uint32 Errcode				=	2;			// 0表示操作成功其他表示错误码
	optional string PlayerName			=	3;			// 申请人的角色名
	optional uint32 CharID				=   4;			// 申请人的角色ID	
	optional uint32 Result				=   5;			// 1表示同意0表示拒绝	   		
}; 

// *************************************************************************** //
// CMessageApplyFamilySuccessNotify  
// 通知玩家申请加入家族成功
// *************************************************************************** //
message CMessageApplyFamilySuccessNotify
{
	optional FamilyHead	HeadMsg			=	1;			// 消息头服务器用
	optional uint32 Result				=   2;			// 1表示加入成功0表示被拒绝其他值表示加入失败
	optional uint32	FamilyID			=	3;			// 家族ID
	optional string FamilyName			=	4;			// 家族名称
	optional uint32 CorpsID				=	5;			// 军团ID
	optional string CorpsName			=	6;			// 军团名称
	optional uint32	FamilyHeader		=	7;			// 家族族长
	optional uint32 CorpsHeader			=	8;			// 军团首领
	optional uint32	UnionID				=	9;			// 军团联盟的ID
	optional string	UnionName			=	10;			// 军团联盟的名称		
};

// *************************************************************************** //
// CMessageFamilyMemberAddedNotify  
// 新成员加入家族的通知				  ::消息修改
// *************************************************************************** //
message CMessageFamilyMemberAddedNotify
{
	optional FamilyHead	HeadMsg			=	1;				// 消息头服务器用
	optional string ManagerName			=	2;				// 批准玩家加入的管理者的姓名
	optional FamilyMemberInfo RoleInfo  =	3;				// 玩家信息
};



// *************************************************************************** //
// CMessagePostAppointRequest  
// 职位任免的请求
// *************************************************************************** //
message CMessagePostAppointRequest
{
	optional FamilyHead	HeadMsg			=	1;			 // 消息头服务器用
	optional uint32 PlayerCharID		=	2;			// 操作对象的角色ID
	optional uint32 ActionKind			=	3;			// 操作类型(任职，免职，用枚举值表示)
};

// *************************************************************************** //
// CMessagePostAppointResponse  
// 职位任免的回应
// *************************************************************************** //
message CMessagePostAppointResponse
{
	optional FamilyHead	HeadMsg			=	1;				// 消息头服务器用
	optional uint32	Errcode				=	2;				// 0表示成功其他表示错误码
	optional uint32 PlayerCharID		=	3;				// 被任免的人
	optional uint32	ActionKind			=	4;				// 任免类型	
};

// *************************************************************************** //
// CMessagePostAppointNoify  
// 职位任免的通知
// *************************************************************************** //
message CMessagePostAppointNoify
{	
	optional FamilyHead	HeadMsg			=	1;				// 消息头服务器用
	optional uint32 PlayerCharID		=	2;				// 被任免的人
	optional uint32	ActionKind			=	3;				// 任免类型	
	optional string ManagerName			=	4;				// 管理者的姓名
	optional string	PlayerName			=	5;				// 被任免者的姓名
};


// *************************************************************************** //
// CMessageLeaveFamilyRequest  
// 离开家族的请求
// *************************************************************************** //
message CMessageLeaveFamilyRequest	
{
	optional FamilyHead	HeadMsg			=	1;			 // 消息头服务器用
	optional uint32 MsgCode				=	2;				// 无用参数
};

// *************************************************************************** //
// CMessageLeaveFamilyResponse  
// 离开家族的回应
// *************************************************************************** //
message CMessageLeaveFamilyResponse
{
	optional FamilyHead	HeadMsg			=	1;			 // 消息头服务器用
	optional uint32 Errcode				=	2;			 // 0表示成功其他表示错误码
};

// *************************************************************************** //
// CMessageMemberLeaveFamilyNotify  
// 成员离开家族的通知
// *************************************************************************** //
message CMessageMemberLeaveFamilyNotify
{		
	optional FamilyHead	HeadMsg			=	1;			// 消息头服务器用
	optional uint32	RoleID				=	2;			// 离开家族的玩家角色ID
	optional string RoleName			=	3;			// 离开家族的玩家的角色名称
	optional uint32	PlayerLevel			=	4;			// 玩家等级
	optional uint32	PlayerPost			=	5;			// 玩家职位	
};

// *************************************************************************** //
// CMessageGetFamilyMemberInfoRequest  
// 获取家族成员信息的请求
// *************************************************************************** /
message CMessageGetFamilyMemberInfoRequest
{
	optional FamilyHead	HeadMsg			=	1;			 // 消息头服务器用
	//optional uint32	FamilyID			=	2;		// 家族ID
};

// *************************************************************************** //
// CMessageFamilyMemberInfoResponse  
// 发送家族的人物信息
// *************************************************************************** //
message CMessageGetFamilyMemberInfoResponse
{										
	optional FamilyHead			HeadMsg		=	1;			 // 消息头服务器用
	optional PBFamilyMemberList MemberInfo	=	2;			 // 玩家角色信息 		
};


// *************************************************************************** //
// CMessageGetFamilyPropertyRequest  
// 获取家族的基本属信息性的请求
// *************************************************************************** //
message CMessageGetFamilyPropertyRequest
{
	optional FamilyHead	HeadMsg			=	1;			 // 消息头服务器用
	// optional uint32	FamilyID			=	2;			 // 家族ID	
}

// *************************************************************************** //
// CMessageGetFamilyPropertyResponse  
// 获取家族的基本属信息性的回应消息
// *************************************************************************** //
message CMessageGetFamilyPropertyResponse
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional string	FamilyNotice		=	2;		// 家族公告
	optional uint32	FamilyNum			=	3;		// 家族人数
	optional uint32 FamilyLevel			=   4;		// 家族等级
	optional uint32 FamilyMoney			=	5;		// 家族金钱
	optional uint32 FamilyHeaderID		=	6;		// 族长ID
	optional string FamilyHeaderName	=	7;		// 族长名称			
	
	optional uint32	FamilyGlory			=	8;			// 家族荣誉	
	optional uint32 NpcID				=	9;			// 家族占领/竞价的NPCID
	optional uint32 BidMoney			=	10;			// 家族竞价的钱数
	optional uint32	BidTime				=	11;			// 家族竞价的时间
	optional uint32 WarNpcStatus		=	12;			// 战争npc的状态0表示已经占领1表示处于争夺当中		
	optional uint32 WarRank				=	13;			// 家族站综合排名
	optional uint32	WarWinNum			=	14;			// 家族战争胜利次数
	optional uint32	WarLostNum			=	15;			// 家族战争失败次数
	optional uint32	FamilyStarLevel		=	16;			// 家族星级
	optional uint32	WarExp				=	17;			// 家族战争积分
	optional uint32 WarMaxExp			=	18;			// 家族战争积分最大值
	optional string EnemyFamilyName		=	19;			// 对战家族名称	
	optional uint32	WarStatus			=	20;			// 战争状态
	
	optional uint32	ChallengeStatus		=	21;			// 挑战家族的状态
	optional uint32	ChallengeMoney		=	22;			// 挑战金额
	optional string	ChallengeStarted	=	23;			// 最近发起的挑战
	optional string ChallengeReceived	=	24;			// 最近收到的挑战
	optional uint32	MaxFamilyMemberNum	=	25;			// 最大人口数量
	optional uint32	CanLevelUP			=	26;			// 是否可以升级
	optional uint32	MaxPresterNum		=	27;			// 长老最大数量
	optional uint32	GloryLimit			=	28;			// 家族经验上限
	optional uint32	LevelMoney			=	29;			// 升级需要的金钱
	optional uint32	Stability			=	30;			// 升级后的家族安定度
	optional uint32	ReduceStability		=	31;			// 每天衰减的安定度
	optional uint32	ExtraRedstonExp		=	32;			// 额外的家族内丹经验加成
};


// *************************************************************************** //
// CMessageGetAllFanmilyInfoRequest  
// 获取所有家族基本信息的请求
// *************************************************************************** //
message CMessageGetAllFanmilyInfoRequest
{
	optional FamilyHead	HeadMsg			=	1;			 // 消息头服务器用
	optional uint32	CurPage				=	2;			 //	当前页数
														 // TODO:以后可能要分批发送
};

// *************************************************************************** //
// CMessageGetAllFamilyInfoResponse  
// 获取所有家族基本信息的回复
// *************************************************************************** //
message CMessageGetAllFamilyInfoResponse
{
	optional FamilyHead		HeadMsg			=	1;		// 消息头服务器用
	optional PBFamilyList	FamilyInfo 		=	2;		// 家族信息
	optional uint32			CurPage			=	3;		// 当前页数
	optional uint32			TotalPage		=	4;		// 总页数
};	   

// *************************************************************************** //
// CMessageGetApplyListRequest  
// 获取申请列表的请求
// *************************************************************************** //
message CMessageGetApplyListRequest
{
	optional FamilyHead	HeadMsg			= 1;		// 消息头服务器用
	optional uint32 FamilyID			= 2;		// 家族ID TODO:该参数没用
};

// *************************************************************************** //
// CMessageGetApplyListResponse  
// 获取申请列表的回复
// *************************************************************************** //

message CMessageGetApplyListResponse
{
	optional FamilyHead	 HeadMsg			=	1;		// 消息头服务器用
	optional PBApplyList ApplyList			=	2;		// 申请列表
};	   



// *************************************************************************** //
// CMessageKickPlayerRequest  
// 踢人的请求
// *************************************************************************** //
message CMessageKickPlayerRequest	
{		
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32 PlayerCharID		=	2;		// 玩家角色ID	
};

// *************************************************************************** //
// CMessageKickPlayerResponse  
// 踢人的回复
// *************************************************************************** //
message CMessageKickPlayerResponse
{
	optional FamilyHead	HeadMsg			=	1;			 // 消息头服务器用
	optional uint32	Errcode				=	2;		// 0表示成功其他表示错误码
	optional string	PlayerName			=	3;		// 剔除的角色名
	optional uint32 PlayerCharID		=	4;		// 玩家的角色ID
};


// *************************************************************************** //
// CMessageKickPlayerNotify
// 踢人的回复
// *************************************************************************** //
message CMessageKickPlayerNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional string	PlayerName			=	2;		// 剔除的角色名
	optional uint32 PlayerCharID		=	3;		// 剔除的角色ID
	optional string ManagerName			=	4;		// 踢人的管理者的角色ID
	optional uint32 ManagerCharID		=	5;		// 踢人的管理者的角色名
};

// *************************************************************************** //
// CMessageDisbandFamilyRequest  
// 解散家族的请求
// *************************************************************************** //
message CMessageDisbandFamilyRequest
{
	optional FamilyHead	HeadMsg			=	1;		 // 消息头服务器用
	optional uint32	MsgCode				=	2;		 // 无用参数
	optional uint32	NpcEntityID			=	3;		 // 家族NPC的实体ID
};

// *************************************************************************** //
// CMessageDisbandFamilyResponse  
// 解散家族的回复
// *************************************************************************** //
message CMessageDisbandFamilyResponse
{
	optional FamilyHead	HeadMsg			=	1;		 // 消息头服务器用
	optional uint32	Errcode				=	2;		 // 0表示成功其他表示错误码
};


// *************************************************************************** //
// CMessageDisbandFamilyNotify  
// 解散家族的通知
// *************************************************************************** //
message CMessageDisbandFamilyNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32 ManagerCharID		=	2;		// 解散家族的管理者角色ID
	optional uint32	FamilyID			=	3;		// 解散的家族ID	
};

// *************************************************************************** //
// CMessageChangeFamilyNotice  
// 改变家族公告
// *************************************************************************** //
message CMessageChangeFamilyNoticeRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional string		Notice			=	2;		// 通知内容
};

// *************************************************************************** //
// CMessageChangeFamilyNoticeResponse  
// 改变家族公告的回复消息
// *************************************************************************** //
message CMessageChangeFamilyNoticeResponse
{
	optional FamilyHead	HeadMsg			=	1;	   // 消息头服务器用
	optional uint32 Errcode				=	2;	   // 0表示成功其他表示错误码
	optional string	Notice				=	3;	   // 公告内容
};



// *************************************************************************** //
// CMessageAppointHeaderRequest  
// 宗正转让职位的请求
// *************************************************************************** //
message CMessageAppointHeaderRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32 PlayerCharID		=	2;		//	被转让职位的角色ID
};

// *************************************************************************** //
// CMessageAppointHeaderRequest  
// 宗正转让职位的请求
// *************************************************************************** //
message CMessageAppointHeaderResponse
{
	optional FamilyHead	HeadMsg			=	1;			 // 消息头服务器用
	optional uint32 Errcode				=	2;		//	0表示成功其他表示错误码
	optional uint32 PlayerCharID		=	3;		//	被转让职位的角色ID
	optional string PlayerName			=	4;		//	被转让职位的角色名
};

// *************************************************************************** //
// CMessageChangeHeaderNotify  
// 宗正转让职位的通知
// *************************************************************************** //
message CMessageChangeHeaderNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32 PlayerCharID		=	2;		//	被转让职位的角色ID
	optional string PlayerName			=	3;		//	被转让职位的角色名
	optional uint32 ManagerID			=	4;		//	转让职位的管理者ID
	optional string ManagerName			=	5;		//	转让职位的管理者名称
};


// *************************************************************************** //
// CMessageFamilyPropertyNotify  
// 家族的属性信息通知
// *************************************************************************** //
message CMessageFamilyPropertyNotify
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用	 	
	optional PBFamilyPropery	Property		=	2;		// 家族属性
	optional uint32				FamilyLevel		=	3;		// 家族等级		
};

// *************************************************************************** //
// CMessageInviteResultNotify  
// 邀请加入的结果通知
// *************************************************************************** //
message CMessageInviteFamilyResultNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32 Result				=	2;		// 邀请结果，被拒绝还是被同意(1表示同意0表示拒绝,其他表示错误码)
	optional string RoleName			=	3;		// 被邀请人的姓名
};

// *************************************************************************** //
// CMessageClearFamilyApplyRequest  
// 清除家族申请列表的消息请求
// *************************************************************************** //
message CMessageClearFamilyApplyRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32 MsgCode				=	2;		// 无用参数
};

// *************************************************************************** //
// CMessageClearFamilyApplyResponse
// 清除家族申请列表的消息回应
// *************************************************************************** //
message CMessageClearFamilyApplyResponse
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32 Errcode				=	2;		// 错误码，0表示成功其他表示错误码
};

// *************************************************************************** //
// CMessageJoinCorpsNotify
// 家族加入军团的消息消息通知
// *************************************************************************** //
message CMessageJoinCorpsNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		CorpsID			=	2;		// 军团ID
	optional string		CorpsName		=	3;		// 军团名称
	optional uint32		JoinMethod		=	4;		// 加入方式是族长创建还是加入了其他军团用枚举值EMethodJoinCorps表示
	optional uint32		UnionID			=	5;		// 军团联盟ID
	optional string		UnionName		=	6;		// 军团联盟名称	
};

// *************************************************************************** //
// CMessageCanelFamilyDisbandRequest
// 取消解散家族的消息请求
// *************************************************************************** //
message CMessageCancelFamilyDisbandRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		NpcEntityID		=	2;		// 家族NPC的实体ID
};


// *************************************************************************** //
// CMessageCancelFamilyDisbandResponse
// 取消解散家族的消息回复
// *************************************************************************** //
message CMessageCancelFamilyDisbandResponse
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		Errcode			=	2;		// 0表示操作成功其他表示错误码
};

// *************************************************************************** //
// CMessageChangeFamilyNoticeNotify
// 改变家族公告的通知
// *************************************************************************** //
message CMessageChangeFamilyNoticeNotify
{
	  optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	  optional string		Notice			=	2;		// 公告内容
};

// *************************************************************************** //
// CMessageRefreshFamilyInfoRequest
// 刷新家族信息的请求
// *************************************************************************** //
message CMessageRefreshFamilyInfoRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		MsgCode			=	2;		// 公告内容
};

// *************************************************************************** //
// CMessageRefreshFamilyInfoResponse
// 刷新家族信息的回应
// *************************************************************************** //
message CMessageRefreshFamilyInfoResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional PBFamilyMemberList	MemberList		=	2;		// 成员信息
};


// *************************************************************************** //
// CMessageSetFamilyPostRequest
// 设置家族成员职位的消息请求
// *************************************************************************** //
message CMessageSetFamilyPostRequest
{
	optional FamilyHead	HeadMsg		=	1;	// 消息头服务器用
	optional uint32		MemberID	=	2;	// 成员角色
	optional uint32		Post		=	3;	// 职位
};

// *************************************************************************** //
// CMessageSetFamilyPostResponse
// 设置家族成员职位的消息消息回应
// *************************************************************************** //
message CMessageSetFamilyPostResponse
{
	optional FamilyHead		HeadMsg		=	1;		// 消息头服务器用
	optional uint32			Errcode		=	2;		// 错误码
	optional uint32			MemberID	=	3;		// 成员角色ID
	optional uint32			Post		=	4;		// 职位	
};

// *************************************************************************** //
// CMessageLevelUPFamilyRequest
// 升级家族的消息请求
// *************************************************************************** //
message CMessageLevelUPFamilyRequest
{  
	optional FamilyHead	HeadMsg	=	1;		// 消息头服务器用
	optional uint32		MsgCode	=	2;		// 无用参数	
};

// *************************************************************************** //
// CMessageLevelUPFamilyResponse
// 升级家族的回应
// *************************************************************************** //
message CMessageLevelUPFamilyResponse
{  
	optional FamilyHead	HeadMsg		=	1;		// 消息头服务器用
	optional uint32		Errcode		=	2;		// 错误码
	optional uint32		Level		=	3;		// 家族等级
};

// *************************************************************************** //
// CMessageContributeFamilyMoneyRequest
// 捐献家族金钱的消息请求
// *************************************************************************** //
message CMessageContributeFamilyMoneyRequest
{ 
	optional FamilyHead		HeadMsg		=	1;		// 消息头服务器用
	optional uint32			Money		=	2;		// 金钱			 				
};

// *************************************************************************** //
// CMessageContributeFamilyMoneyResponse
// 捐献家族金钱的消息回应
// *************************************************************************** //
message CMessageContributeFamilyMoneyResponse
{  
	optional FamilyHead		HeadMsg		=	1;		// 消息头服务器用
	optional uint32			Errcode		=	2;		// 错误码
	optional uint32			Money		=	3;		// 捐献的金钱
	optional uint32			Contribute	=	4;		// 捐献金钱获取的熟练度
};

// *************************************************************************** //
// CMessageObtainFamilyMoneyRequest
// 从占领npc处领取家族福利的消息请求
// *************************************************************************** //
message CMessageObtainFamilyMoneyRequest
{
	optional FamilyHead		HeadMsg		=	1;		// 消息头服务器用
	optional uint32			NpcID		=	2;		// 家族npcID
	optional uint32			NpcTmpID	=	3;		// npc的模板ID(服务器用)			
};

// *************************************************************************** //
// CMessageObtainFamilyMoneyResponse
// 从占领npc处领取家族福利的消息回应
// *************************************************************************** //
message CMessageObtainFamilyMoneyResponse
{
	optional FamilyHead		HeadMsg	=	1;		// 消息头服务器用
	optional uint32			Errcode	=	2;		// 错误码		
	optional uint32			Money	=	3;		// 领取的金钱
	optional uint32			Glory	=	4;		// 获取的荣耀值
};

// *************************************************************************** //
// CMessageSetCorpsPostNotify
// 家族职位任免的通知
// *************************************************************************** //
message CMessageSetFamilyPostNotify
{	
	optional FamilyHead		HeadMsg			=	1;		// 消息头服务器用
	optional uint32			ManagerID		=	2;		// 管理者ID
	optional string			ManagerName		=	3;		// 管理者名称
	optional uint32			MemberID		=	4;		// 成员ID
	optional string			MemberName		=	5;		// 成员名称				
	optional uint32			OriginalPost	=	6;		// 原来的职位
	optional uint32			NewPost			=	7;		// 新的职位
};

// *************************************************************************** //
// CMessageLevelUPFamilyNotify
// 家族升级的通知
// *************************************************************************** //
message CMessageLevelUPFamilyNotify
{
	optional FamilyHead		HeadMsg			=	1;		// 消息头服务器用
	optional uint32			Level			=	2;		// 家族等级	
	optional uint32			Money			=	3;		// 家族当前金钱
	optional uint32			Glory			=	4;		// 家族当前荣耀
	optional uint32			MaxFamilyMember =	5;		// 升级后的人口上限
	optional uint32			MaxPresterNum	=	6;		// 升级后的长老数量上限
	optional uint32			LevelMoney		=	7;		// 升级需要的金钱
	optional uint32			LevelGlory		=	8;		// 升级需要的荣耀	
	optional uint32			Stability		=	9;		// 升级后的家族安定度
	optional uint32			ReduceStability	=	10;		// 升级后每天衰减的安定度
};

// *************************************************************************** //
// CMessageFamilyBidNpcRequest
// 族长请求争夺NPC
// *************************************************************************** //
message CMessageFamilyBidNpcRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		NpcID			=	2;		// 争夺的NPCID
	optional uint32		Money			=	3;		// 请求的金额
};

// *************************************************************************** //
// CMessageFamilyBidNpcResponse
// 争夺NPC回应
// *************************************************************************** //
message CMessageFamilyBidNpcResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				Result			=	2;		// 争夺的结果
	optional uint32				NpcID			=	3;		// 争夺的NPC ID
	optional uint32				Money			=	4;		// 实际扣除的金额
};

// *************************************************************************** //
// CMessageMemberSubmitNotify
// 争夺报名通知
// *************************************************************************** //
message CMessageMemberSubmitNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		NpcID			=	2;		// 争夺的NPC ID
	optional string		FamilyName		=	3;		// 对手的名称
	optional uint32		First			=	4;		// 是否初次
};

// *************************************************************************** //
// CMessageMemberSubmitRequest
// 争夺报名请求
// *************************************************************************** //
message CMessageMemberSubmitRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		NpcID			=	2;		// 争夺的NPC ID
	repeated uint32		MemberID		=	3;		// 报名的成员ID
};

// *************************************************************************** //
// CMessageMemberSubmitResponse
// 争夺报名回应
// *************************************************************************** //
message CMessageMemberSubmitResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				Result			=	2;		// 报名的结果
	optional uint32				NpcID			=	3;		// 争夺的NPC ID
};

// *************************************************************************** //
// CMessageAbortNpcRequest
// 族长放弃NPC请求
// *************************************************************************** //
message CMessageAbortNpcRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		FamilyID		=	2;		// 放弃的家族ID
	optional uint32		NpcID			=	3;		// 放弃的NPC ID
};

// *************************************************************************** //
// CMessageAbortNpcResponse
// 族长放弃NPC回应
// *************************************************************************** //
message CMessageAbortNpcResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				Result			=	2;		// 放弃的结果
	optional uint32				FamilyID		=	3;		// 放弃的家族ID
	optional uint32				NpcID			=	4;		// 放弃的NPC ID
};

// *************************************************************************** //
// CMessageBidInfoRequest
// 查询争夺请求
// *************************************************************************** //
message CMessageBidInfoRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		NpcID			=	2;		// 查询的NPC ID
};

message BidResult
{
	optional uint32				FamilyID		=	1;		// 报价的家族ID
	optional string				FamilyName		=	2;		// 报价的家族名称
	optional uint32				Money			=	3;		// 报价的价格
	optional uint32				Win				=	4;		// 胜
	optional uint32				Lost			=	5;		// 负
	optional uint32				Country			=	6;		// 国家
	optional uint32				Level			=	7;		// 星级
};

// *************************************************************************** //
// CMessageBidInfoResponse
// 查询争夺回应
// *************************************************************************** //
message CMessageBidInfoResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				Result			=	2;		// 放弃的结果
	optional uint32				NpcID			=	3;		// 查询的NPC ID
	optional string				FamilyName		=	4;		// 占有的家族名称
	repeated BidResult			BidData			=	5;		// 查询信息				
};

// *************************************************************************** //
// CMessageFamilySubmitNotify
// 报名通知
// *************************************************************************** //
message CMessageFamilySubmitNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		NpcID			=	2;		// 争夺的NPC ID
	optional string		FamilyName		=	3;		// 对方家族名
	repeated uint32		MemberID		=	4;		// 报名的成员ID
	optional uint32		ManagerID		=	5;		// 提交的管理者ID
	optional string		ManagerName		=	6;		// 提交的管理者角色名称
};

// *************************************************************************** //
// CMessageBattleResultNotify
// 战斗结果通知
// *************************************************************************** //
message CMessageBattleResultNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		NpcID			=	2;		// 争夺的NPC ID
	optional string		AttFamilyName	=	3;		// 进攻方家族名
	optional string		DefFamilyName	=	4;		// 防守方家族名
	optional uint32		WinSide			=	5;		// 获胜方（1，2）
	optional uint32		AttFamilyID		=	6;		// 进攻方ID
	optional uint32		DefFamilyID		=	7;		// 防守方ID
};


// *************************************************************************** //
// CMessagePVPStarLevelUPNotify
// pvp星级升级的通知
// *************************************************************************** //
message CMessageFamilyPVPStarLevelUPNotify
{	
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				PVPStarLevel	=	2;		// pvp星级	
};

// *************************************************************************** //
// CMessageMakeRoomRequest
// 开设房间请求
// *************************************************************************** //
message CMessageMakeRoomRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		Money			=	2;		// 开设需要的钱
	optional string		Name			=	3;		// 房间名称
};

// *************************************************************************** //
// CMessageMakeRoomResponse
// 开设房间回应
// *************************************************************************** //
message CMessageMakeRoomResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				Result			=	2;		// 开设的结果
	optional string				Name			=	3;		// 房间名称
	optional uint32				Money			=	4;		// 开设需要的钱
};

// *************************************************************************** //
// CMessageChaRoomRequest
// 挑战房间请求
// *************************************************************************** //
message CMessageChaRoomRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		FamilyID		=	2;		// 挑战的家族ID
};

// *************************************************************************** //
// CMessageChaRoomResponse
// 挑战房间回应
// *************************************************************************** //
message CMessageChaRoomResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				Result			=	2;		// 挑战的结果
	optional uint32				FamilyID		=	3;		// 挑战的家族ID
};

// *************************************************************************** //
// CMessageAgreeBattleRequest
// 同意挑战请求
// *************************************************************************** //
message CMessageAgreeBattleRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		FamilyID		=	2;		// 同意的家族ID
};

// *************************************************************************** //
// CMessageAgreeBattleResponse
// 同意挑战回应
// *************************************************************************** //
message CMessageAgreeBattleResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				Result			=	2;		// 同意的结果
	optional uint32				FamilyID		=	3;		// 同意的家族ID
};

// *************************************************************************** //
// CMessageQueryAllRoomRequest
// 查询房间列表请求
// *************************************************************************** //
message CMessageQueryAllRoomRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
};

message AllRoomInfo
{
	optional uint32		FamilyID		= 1;
	optional string		FamilyName		= 2;
	optional uint32		Money			= 3;
	optional string		RoomName		= 4;
	optional uint32		Win				= 5;
	optional uint32		Lost			= 6;
	optional string		AttName			= 7;
	optional uint32		State			= 8;
};

// *************************************************************************** //
// CMessageQueryAllRoomResponse
// 查询房间列表回应
// *************************************************************************** //
message CMessageQueryAllRoomResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				Result			=	2;		// 查询的结果
	repeated AllRoomInfo		RoomData		=	3;		// 房间信息
};

// *************************************************************************** //
// CMessageQueryMyRoomRequest
// 查询自己的房间请求
// *************************************************************************** //
message CMessageQueryMyRoomRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
};

message MyRoomInfo
{
	optional uint32		FamilyID		= 1;
	optional string		FamilyName		= 2;
	optional uint32		Win				= 3;
	optional uint32		Lost			= 4;
};

// *************************************************************************** //
// CMessageQueryMyRoomResponse
// 查询自己的房间回应
// *************************************************************************** //
message CMessageQueryMyRoomResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				Result			=	2;		// 查询的结果
	optional uint32				Money			=	3;		// 房间的金额
	repeated MyRoomInfo			RoomData		=	4;		// 房间信息
	optional string				Name			=	5;		// 房间名称
};

// *************************************************************************** //
// CMessageQueryMyChaRequest
// 查询自己的挑战请求
// *************************************************************************** //
message CMessageQueryMyChaRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
};

message MyChaInfo
{
	optional uint32		FamilyID		= 1;
	optional string		FamilyName		= 2;
	optional uint32		Money			= 3;
	optional string		RoomName		= 4;
	optional uint32		Win				= 5;
	optional uint32		Lost			= 6;
};

// *************************************************************************** //
// CMessageQueryMyChaResponse
// 查询自己的挑战回应
// *************************************************************************** //
message CMessageQueryMyChaResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				Result			=	2;		// 查询的结果
	repeated MyChaInfo			RoomData		=	3;		// 房间信息
};

// *************************************************************************** //
// CMessageAbortRoomRequest
// 放弃房间请求
// *************************************************************************** //
message CMessageAbortRoomRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
};

// *************************************************************************** //
// CMessageAbortRoomResponse
// 放弃房间回应
// *************************************************************************** //
message CMessageAbortRoomResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				Result			=	2;		// 放弃的结果
};

// *************************************************************************** //
// CMessageAbortChaRequest
// 放弃挑战请求
// *************************************************************************** //
message CMessageAbortChaRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		FamilyID		=	2;		// 放弃的家族ID
};

// *************************************************************************** //
// CMessageAbortChaResponse
// 放弃挑战回应
// *************************************************************************** //
message CMessageAbortChaResponse
{
	optional FamilyHead			HeadMsg			=	1;		// 消息头服务器用
	optional uint32				Result			=	2;		// 放弃的结果
	optional uint32				FamilyID		=	3;		// 放弃的家族ID
};	  


// *************************************************************************** //
// CMessageContributeFamilyMoneyNotify
// 贡献家族金钱的通知
// *************************************************************************** //
message CMessageContributeFamilyMoneyNotify
{
	optional FamilyHead	HeadMsg			=	1;	// 消息头服务器用	
	optional uint32		FamilyMoney		=	2;	// 家族现有金钱
	optional string		MemberName		=	3;	// 捐献金钱的成员名称
	optional uint32		MemberID		=	4;	// 捐献金钱的成员角色ID
	optional uint32		Value			=	5;	// 差值
};

// *************************************************************************** //
// CMessageFamilyMoneyChangedNotify
// 家族金钱改变的消息
// *************************************************************************** //
message CMessageFamilyMoneyChangedNotify
{
	optional uint32		Money	 =	1;		// 家族当前金钱	
	optional uint32		RoleID	 =	2;		// 引起家族金钱改变的角色ID
	optional uint32		Reason	 =	3;		// 金钱改变的原因
	optional string		RoleName =	4;		// 引起家族金钱改变的角色名称
	optional int32		Value	 =	5;		// 差值
};

// *************************************************************************** //
// CMessageUseFamilyExpCardNotify
// 使用经验令牌的通知消息
// *************************************************************************** //
message CMessageUseFamilyExpCardNotify
{	
	optional string ManagerName	=	1;		// 族长名字
	optional uint32	ManagerID	=	2;		// 族长角色ID
	optional uint32	LineID		=	3;		// 族长所在线ID
	optional uint32	MapID		=	4;		// 地图ID
	optional uint32	PosX		=	5;		// x坐标
	optional uint32	PosY		=	6;		// y坐标
};

// *************************************************************************** //
// CMessageStartFamilyBossRequest
// 开通家族boss活动的请求
// *************************************************************************** //
message CMessageStartFamilyBossRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32 EnityID				=	2;		// npcID	
	optional uint32	RepetionLevel		=	3;		// 家族副本等级	
	optional uint32	Date				=	4;		// 进入副本的时间
	optional uint32	MaxTimes			=	5;		// 家族副本一天允许进入的最大次数
};

// *************************************************************************** //
// CMessageMakeRoomNotify
// 开设房间通知
// *************************************************************************** //
message CMessageMakeRoomNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		HeaderID		=	2;		// 族长ID
	optional string		HeaderName		=	3;		// 族长名字
};

// *************************************************************************** //
// CMessageStartFamilyBossResponse
// 开通家族boss活动的回复消息
// *************************************************************************** //
message CMessageStartFamilyBossResponse
{	
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		Errcode			=	2;
};

// *************************************************************************** //
// *************************************************************************** //
// CMessageEnterBossReptionRequest
// 成员进入家族副本的请求
// *************************************************************************** //
message CMessageEnterBossReptionRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32	NpcID				=	2;		// npcID
	optional uint32 FamilyID			=	3;		// 家族ID( 服务器用 )
};

// *************************************************************************** //
// CMessageEnterBossReptionResponse
// 成员进入家族副本的消息回应
// *************************************************************************** //
message CMessageEnterBossReptionResponse
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		Errcode			=	2;		// 错误码
};

// *************************************************************************** //
// CMessageAbortRoomNotify
// 取消房间通知
// *************************************************************************** //
message CMessageAbortRoomNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		HeaderID		=	2;		// 族长ID
	optional string		HeaderName		=	3;		// 族长名字
};

// *************************************************************************** //
// CMessageChaRoomNotify
// 挑战房间通知
// *************************************************************************** //
message CMessageChaRoomNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		SrcFamilyID		=	2;		// 挑战家族ID
	optional string		SrcFamilyName	=	3;		// 挑战家族名称
	optional uint32		SrcHeaderID		=	4;		// 挑战族长ID
	optional string		SrcHeaderName	=	5;		// 挑战族长名称
	optional uint32		DesFamilyID		=	6;		// 房间家族ID
	optional string		DesFamilyName	=	7;		// 房间家族名称
	optional uint32		DesHeaderID		=	8;		// 房间族长ID
	optional string		DesHeaderName	=	9;		// 房间族长名称
	optional uint32		SrcWin			=	10;		// 挑战方胜
	optional uint32		SrcLost			=	11;		// 挑战方负
};

// *************************************************************************** //
// CMessageAbortChaNotify
// 取消挑战通知
// *************************************************************************** //
message CMessageAbortChaNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		SrcFamilyID		=	2;		// 取消家族ID
	optional string		SrcFamilyName	=	3;		// 取消家族名称
	optional uint32		DesFamilyID		=	4;		// 房间家族ID
	optional string		DesFamilyName	=	5;		// 房间家族名称
	optional string		HeaderName		=	6;		// 族长名称
	optional uint32		ManagerID		=	7;		// 放弃的管理者的角色ID
	optional string		ManagerName		=	8;		// 放弃的管理者的角色名称
};

// *************************************************************************** //
// CMessageFamilyGloryChangedNotify
// 家族荣耀改变的通知
// *************************************************************************** //
message CMessageFamilyGloryChangedNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		Glory			=	2;		// 现在的荣耀值
	optional uint32		RoleID			=	3;		// 引起荣耀值改变的玩家角色ID
	optional uint32		Reason			=	4;		// 荣耀改变的原因	
	optional string		RoleName		=	5;		// 角色名称
	optional int32		Value			=	6;		// 差值
};

// *************************************************************************** //
// CMessageMemberContributeChangedNotify
// 个人贡献度改变的通知
// *************************************************************************** //
message CMessageMemberContributeChangedNotify
{ 
	optional FamilyHead	HeadMsg		=	1;		// 消息头服务器用
	optional uint32	Contribute		=	2;		// 玩家当前的贡献度
	optional uint32	RoleID			=	3;		// 玩家角色ID
	optional int32	Value			=	4;		// 差值
};

// *************************************************************************** //
// CMessageQueryNpcWelfareRequest
// 查询npc处福利的消息请求
// *************************************************************************** //
message CMessageQueryNpcWelfareRequest
{
	optional FamilyHead HeadMsg		=	1;		//	家族消息头
	optional uint32		EntityID	=	2;		//	NPC实体ID
	optional uint32		NpcTmpID	=	3;		//	npc的模板ID(服务器用)
};

// *************************************************************************** //
// CMessageQueryNpcWelfareResponse
// 查询npc处福利的消息回应（荣耀和金钱）
// *************************************************************************** //
message CMessageQueryNpcWelfareResponse
{
	optional FamilyHead HeadMsg		=	1;		//	家族消息头
	optional uint32		Money		=	2;		//	可以领取的金钱
	optional uint32		Glory		=	3;		//	可以领取的荣耀值
	optional uint32		Errcode		=	4;
};

// *************************************************************************** //
// CMessageGetFamilyMoneyRequest
//  获取家族金钱到个人的消息请求
// *************************************************************************** //
message  CMessageGetFamilyMoneyRequest
{
	optional FamilyHead HeadMsg		=	1;		//	家族消息头
	optional uint32		FamilyMoney	=	2;		//	要获取的金锭	
};

// *************************************************************************** //
// CMessageGetFamilyMoneyResponse
//  获取家族金钱到个人的消息回应
// *************************************************************************** //
message  CMessageGetFamilyMoneyResponse
{
	optional FamilyHead HeadMsg		=	1;		// 家族消息头
	optional uint32		Errcode		=	2;		// 错误码
	optional uint32		FamilyMoney	=	3;		// 要获取的金锭	
};

// *************************************************************************** //
// CMessageAbortNpcNotify
// 族长放弃NPC通知
// *************************************************************************** //
message CMessageAbortNpcNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		NpcID			=	2;		// 放弃的NPC ID
	optional string		HeaderName		=	3;		// 族长的名字
};

// *************************************************************************** //
// CMessagePreBattleNotify
// 家族战战斗前通知
// *************************************************************************** //
message CMessagePreBattleNotify
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		NpcID			=	2;		// 战斗的NPC ID
	optional string		AttFamilyName	=	3;		// 挑战方家族名称
	optional string		DefFamilyName	=	4;		// 防守方家族名称
	optional uint32		Money			=	5;		// 竞价的钱数
};

// *************************************************************************** //
// CMessageGetExpCardRequest
// 领取家族令牌的请求
// *************************************************************************** //
message CMessageGetExpCardRequest
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		Msgcode			=	2;		// 无用参数
};

// *************************************************************************** //
// CMessageGetExpCardResponse
// 获取经验令牌的回应
// *************************************************************************** //
message CMessageGetExpCardResponse
{
	optional FamilyHead	HeadMsg			=	1;		// 消息头服务器用
	optional uint32		Errcode			=	2;		// 错误码
	optional uint32		RedStoneID		=	3;		// 家族内丹ID
};

// *************************************************************************** //
// CMessageFamilyCanLevelUPNotify
// 家族可以升级的通知消息
// *************************************************************************** //
message CMessageFamilyCanLevelUPNotify
{
	optional FamilyHead HeadMsg		= 1;		// 家族消息头	
};

// *************************************************************************** //
// CMessageFamilyRepetionScoreNotify
// 家族副本评分的通知
// *************************************************************************** //
message CMessageFamilyRepetionScoreNotify
{		
	optional uint32	 RepetionLevel	=	1;	// 副本难度
	optional uint32	 OgreNum		=	2;	// 杀怪数目
	optional uint32	 TimeLength		=	3;	// 副本通关时间
	optional uint32	 WaveNum		=	4;	// 第几波
	optional uint32	 MaxHitNum		=	5;	// 最大连击数
	optional uint32	 Score			=	6;	// 副本总分
};

// *************************************************************************** //
// CMessageGetFamilyGoodsListRequest
// 获取家族商品列表的消息请求
// *************************************************************************** //
message GoodItem
{
	optional uint32 ItemID		= 1;
	optional uint32	ItemNum		= 2;
	optional uint32	ComposeID	= 3;	// 复杂合成ID，兑换用
};

message CMessageGetFamilyGoodsListRequest
{
	optional FamilyHead HeadMsg = 1;		// 家族消息头
	optional uint32	FamilyID =	2;			// 家族Id(不用填值)
	optional uint32	EntityID =  3;			// npc实体ID
	optional uint32	NpcTmpID =	4;			// npc模板ID
};

// *************************************************************************** //
// CMessageGetFamilyGoodsListResponse
// 获取家族商品列表的消息回应
// *************************************************************************** //
message CMessageGetFamilyGoodsListResponse
{	
	optional FamilyHead HeadMsg		= 1;		// 家族消息头
	optional uint32		Errcode		= 2;		// 错误码
	repeated GoodItem	GoodsList	= 3;		// 家族已经买过的商品列表
	optional uint32		NpcTmpID	= 4;		// npc的模板ID(服务器用)	
};

// *************************************************************************** //
// CMessageGetFamilyExchangeGoodsListRequest
// 获取家族兑换商品列表的消息请求
// *************************************************************************** //
message CMessageGetFamilyExchangeGoodsListRequest
{
	optional FamilyHead HeadMsg = 1;		// 家族消息头
	optional uint32	FamilyID =	2;			// 家族Id(不用填值)
	optional uint32	EntityID =  3;			// npc实体ID	
	optional uint32	NpcTmpID	= 4;		// npc的模板ID(服务器用)	
};

// *************************************************************************** //
// CMessageGetFamilyExchangeGoodsListResponse
// 获取家族兑换商品列表的消息回应
// *************************************************************************** //
message CMessageGetFamilyExchangeGoodsListResponse
{
	optional FamilyHead HeadMsg		= 1;		// 家族消息头
	optional uint32		Errcode		= 2;		// 错误码
	repeated GoodItem	GoodsList	= 3;		// 能够兑换的商品列表包括数量
	optional uint32		NpcTmpID	= 4;		// npc的模板ID		
};

// *************************************************************************** //
// CMessageChangeGoodsListNotify
// 家族商店购买的物品列表发生变化的通知
// *************************************************************************** //
message CMessageChangeFamilyGoodsListNotify
{
	repeated GoodItem	GoodList	= 1;		// 发生变化的商品的列表
	optional uint32		NpcTmpID	= 2;		// npc的模板ID
};

// *************************************************************************** //
// CMessageChangeExchangeGoodsListNotify
// 家族兑换商店物品列表发生变化的通知
// *************************************************************************** //
message CMessageChangeFamilyExchangeGoodsListNotify
{
	repeated GoodItem	GoodList	= 1;		// 发生变化的兑换商品的列表	
};

// *************************************************************************** //
// CMessageChangeExchangeGoodsListNotify
// 家族兑换商店物品列表发生变化的通知
// *************************************************************************** //
message CMessageUseFamilyLeagueItemNotify
{
	repeated GoodItem	GoodList	= 1;		// 发生变化的兑换商品的列表	
	optional uint32		ManagerID	= 2;		// 管理者角色ID
	optional string		ManagerName	= 3;		// 管理者角色名称	
};

// 获得家族家园信息请求
message CMessageGetHomeInfoRequest
{
	optional uint32		CharID		= 1;
	optional uint32		FamilyID	= 2;
};


// 获得家族家园信息回应
message CMessageGetHomeInfoResponse
{
	optional uint32		CharID		= 1;
	optional uint32		FamilyID	= 2;
	optional uint32		Result		= 3;
	repeated uint32		Wizard		= 4;		// 家族才华
	repeated uint32		Construct	= 5;		// 家族建设度
	repeated uint32		Level		= 6;		// 家族建设等级
	repeated uint32		UpData		= 7;		// 升到下一级数值
};


// 升级开通家族家园建筑物请求
message CMessageUpgradeHomeRequest
{
	optional uint32		CharID		= 1;	
	optional uint32		Type		= 2;
};

// 升级开通家族家园建筑物回应
message CMessageUpgradeHomeResponse
{
	optional uint32		CharID		= 1;	
	optional uint32		Type		= 2;		// 升级类型
	optional uint32		UpData		= 3;		// 升到下一级数值
};

// 家族安定度改变的通知
message CMessageFamilyStabilityChangedNotify
{
	optional uint32		Reason		= 1;		// 改变的原因
	optional uint32		Stability	= 2;		// 当前家族安定度	
	optional uint32		Value		= 3;		// 改变的安定度值
	optional uint32		RoleID		= 4;		// 让家族安定度改变的角色ID
	optional string		RoleName	= 5;		// 让家族安定度改变的角色名称
	optional uint32		OnLineTime	= 6;		// 使家族安定度增加的累积在线时间(单位:小时)
};