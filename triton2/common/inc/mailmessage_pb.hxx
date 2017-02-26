import "coremessage_pb.hxx";
import "property_pb.hxx";
// 以最快速度序列/反序列
option optimize_for = SPEED;

enum MailMessageID
{	
	ID_C2S_REQUEST_OPENMAILBOX           = 0x2000;          // 打开邮箱的请求
	ID_S2C_RESPONSE_OPENMAILBOX          = 0x2001;          // 打开邮箱的回应   
	ID_C2S_REQUEST_OPENSENDMAILBOX		 = 0x2002;			// 打开发件箱的请求
	ID_S2C_RESPONSE_OPENSENDMAILBOX		 = 0x2003;			// 打开发件箱的回应
	ID_C2S_REQUEST_OPENREADMAILBOX		 = 0x2004;			// 打开收件箱的请求
	ID_S2C_RESPONSE_OPENREADMAILBOX		 = 0x2005;			// 打开收件箱的回应 	
	ID_S2C_NOTIFY_MAILERRORCODE			 = 0x2006;			// 邮件错误码
	ID_C2S_REQUEST_CHANGEITEM			 = 0x2007;       	// 修改邮件附件内容的请求	
	ID_S2C_RESPONSE_CHANGEITEM 			 = 0x2008;          // 修改邮件附件内容的回复
	ID_C2S_REQUEST_MAILCANSEND           = 0x2009;			// 邮件是否可以发送
	ID_S2C_RESPONSE_MAILCANSEND          = 0x200A;			// 邮件是否可以发送的回应
	ID_S2S_RESPONSE_PLAYEONLINE			 = 0x200B;			// 玩家是否在线
	ID_C2S_REQUEST_MAILSEND				 = 0x200C;			// 发送邮件的请求
	ID_S2C_RESPONSE_MAILSEND			 = 0x200D;			// 邮件发送的回复
	ID_S2C_NOTIFY_MAILLISTINFO			 = 0x200E;			// 发送邮件列表
	ID_S2C_REQUEST_SHUTMAILBOX		     = 0x200F;			// 关闭邮箱的请求
	ID_C2S_REQUEST_SHUTREADMAILBOX		 = 0x2010;			// 关闭收件箱的请求
	ID_C2S_REQUEST_SHUTSENDMAILBOX		 = 0x2011;			// 关闭发件箱
	ID_S2C_NOTIFY_NEWMAIL				 = 0x2212;			// 新邮件通知,路由消息 +0x0200
	ID_C2S_REQUEST_READMAIL				 = 0x2013;			// 读邮件请求
	ID_S2C_RESPONSE_READMAIL			 = 0x2014;			// 邮件内容	
	ID_C2S_REQUEST_GETACCESSORY			 = 0x2015;			// 获取邮件附件						
	ID_C2S_REQUEST_DELMAIL				 = 0x2016;			// 删除邮件
	ID_C2S_NOTIFY_READMAILEND			 = 0x2017;			// 读取邮件完毕
	ID_C2S_REQUEST_BACKMAIL				 = 0x2018;			// 退信
	ID_C2S_REQUEST_LOCKMAIL				 = 0x2019;			// 锁定邮件
	ID_C2S_REQUEST_UNLOCKMAIL			 = 0x201A;			// 解锁邮件
	ID_C2S_REQUEST_GETMAILPAGE			 = 0x201B;			// 获取邮件列表 
	ID_S2C_RESPONSE_DELMAIL				 = 0x201C;			// 删除邮件的回应
	ID_S2C_RESPONSE_GETACCESSORY		 = 0x201D;			// 获取邮件附件的回应消息
	ID_S2C_RESPONSE_BACKMAIL			 = 0x201E;			// 退信的回应消息
	ID_C2S_NOTIFY_CANNCELMAILSEND		 = 0x202F;			// 取消邮件发送
	ID_S2C_NOTIFY_MAILBOXSTATUS			 = 0x2030;			// 邮件状态通知：邮件数量，有没有新邮件
	ID_C2S_REQUEST_PAYMAILMONEY			 = 0x2031;			// 付费邮件的付款请求
	ID_S2C_RESPONSE_PAYMAILMONEY		 = 0x2032;			// 付费邮件的付款回应
};


// *************************************************************************** //
// MailItemBox  
// 
// *************************************************************************** //
message MailItemBox
{
	repeated PBItemObject MailItem		 = 		1; 					// 附件物品
	repeated uint32		  ItemIndex		 = 		2; 					// 附件物品在附件列表中的索引
};

// *************************************************************************** //
// CMessageOpenMailBox  
// 玩家打开邮箱的请求
// *************************************************************************** //
message CMessageOpenMailBoxRequest
{
	optional uint32 MsgCode		= 	1; 
	optional uint32	EntityID	=	2;	// npc实体ID 		
};

// *************************************************************************** //
// CMessageOpenMailBoxResponse  
// 玩家打开邮箱请求的回复
// *************************************************************************** //
message CMessageOpenMailBoxResponse
{
	optional uint32 Result  = 		1; 
};


// *************************************************************************** //
// CMessageShutDownMailBoxRequest
// 玩家打开邮箱请求的回复
// *************************************************************************** //
message CMessageShutDownMailBoxRequest
{
	optional uint32 MsgCode  = 		1; 
};

// *************************************************************************** //
// CMessageMailChangeItem          游戏客户端->游戏服务器
// 增加或者删除附件
// *************************************************************************** //
message CMessageMailChangeItem
{
	optional uint32 ItemNum		  = 		1;    // 附件物品的数量
	optional uint32 SrcIndex      = 		2;    // 源索引	
	optional uint32	MoveMethod	  = 		3; 	// 移动方式：0表示在包裹和附件列表位置之间的移动（ItemNum>0 表示从包裹到附										//件列表， ItemNum=0 表示从附件列表到包裹 ），1表示在附件列表位置上的移动
	optional uint32 DesIndex	  = 		4; 	// 目标索引
};

// *************************************************************************** //
// CMessageMailChangeItemCallBack    游戏服务器->游戏客户端
// 改变附件后的返回消息
// *************************************************************************** //

message CMessageMailChangeItemCallBack
{
	optional uint32 ItemNum		  = 		1;    // 附件物品的数量
	optional uint32 DesIndex	  = 		2; 	// 目标索引
	optional uint32	MoveMethod	  = 		3; 	// 移动方式：0表示在包裹和附件列表位置之间的移动（ItemNum>0 表示从包裹到附											//件列表， ItemNum<=0 表示从附件列表到包裹 ），1表示在附件列表位置上的移动
	optional uint32 SrcIndex      = 		4;    // 源索引	
};

// *************************************************************************** //
// CMessageMailCanSend 游戏客户端->游戏服务器
// 判断玩家是否在线
// *************************************************************************** //
message CMessageMailCanSend 
{
	optional string Name        =	1;  //邮件接收方角色名	
	optional uint32	EntityID	=	2;	// npc实体ID 		
};

// *************************************************************************** //
// CMessageMailCanSend 游戏客户端->游戏服务器
// 判断玩家是否在线
// *************************************************************************** //
message CMessageMailCanSendCallBack
{
	optional uint32 CanSendMail      = 		1;                     // 1表示可以发送，0表示不能发送
	optional uint32 PlayerCharID     = 		2;                     // 玩家charid	
};


// *************************************************************************** //
// CMessageMailSend 游戏客户端-> 游戏服务器
// 发送邮件
// *************************************************************************** //

message AccessoryInfo
{
	optional uint32	ItemIndex				 =		1;				// 附件在包裹中的索引
	optional uint32	ItemNum					 =		2;				// 附件数量
	optional uint32	ItemPos					 =		3;				// 邮件附件在附件中的位置索引		
};
message CMessageMailSend
{
	optional string Body				     = 		1;              // 邮件正文	
	optional uint32 MoneyNeedBack            = 		2;              // 需要对方支付的邮件费用
	optional string DesName					 = 		3; 				// 邮件接收方姓名
	optional uint32 DesCharID                = 		4;              // 邮件接收方的CharID
	optional string Title				     = 		5;              // 邮件标题
	optional uint32 Money			         = 		6;              // 发送的金钱
	optional uint32	EntityID				 =		7;				// npc实体ID	
	repeated AccessoryInfo Accessory		 =		8;				// 邮件附件信息
};

// *************************************************************************** //
// CMessageMailListNotify 游戏服务器 -> 游戏客户端
// 获得邮件列表
// *************************************************************************** //

message CMailInfo
{
	optional string Title			 = 		1; 		// 标题
	optional uint32 Money			 = 		2;        // 附件携带的金钱
	optional uint32 Sendtime  		 = 		3; 		// 时间
	optional uint32 ExpiredTime		 = 		4; 		// 结束时间
	optional uint32 MoneyNeedBack	 = 		5; 		// 需要支付的金钱
	optional uint32 MailLevel		 = 		6; 		// 标志（普通邮件，包裹邮件，系统邮件）	
	optional uint32 MailType		 = 		7; 		// 邮件状态
	optional uint32 MailID			 = 		8; 		// 邮件id
	optional string Srcname 		 = 		9; 		// 发件人
	optional uint32 ItemNum			 = 		10; 		// 附件数量	
};

message CMessageMailListNotify
{	
	repeated CMailInfo  MailList    = 		1;  
};

// *************************************************************************** //
// CMessageMailErrorCode 游戏服务器 -> 游戏客户端
// 邮件错误消息
// *************************************************************************** //
message CMessageMailErrorCode
{
	optional int32 ErrorCode  = 		1; 
};
// *************************************************************************** //
// CMessageShutDownMailBoxRequest 游戏客户端->游戏服务器
// 关闭邮箱
// *************************************************************************** //
message CMessageShutMailBoxRequest
{
	optional uint32 MsgCode  = 		1; 	
};

// *************************************************************************** //
// CMessageShutReadMailBoxRequest 游戏客户端->游戏服务器
// 关闭收件箱
// *************************************************************************** //
message CMessageShutReadMailBoxRequest
{
	optional uint32 MsgCode  = 		1; 
};

// *************************************************************************** //
// CMessageShutSendMailBoxRequest 游戏客户端->游戏服务器
// 关闭发件箱
// *************************************************************************** //
message CMessageShutSendMailBoxRequest
{
	optional uint32 MsgCode  = 		1; 
};

// *************************************************************************** //
// CMessageMailNewNotify 游戏服务器 -> 游戏客户端
// 新邮件通知,基本信息
// *************************************************************************** //
message CMessageNewMailNotify
{		
	optional CMessageRouter Router  = 		1;  	// 路由消息	
	optional uint32		HasList	    = 		2; 		// 是否有邮件基本信息1表示有，0表示没有		
	optional CMailInfo  MailList    = 		3; 		// 邮件基本信息
};
// *************************************************************************** //
// CMessageMailRead 游戏客户端-> 游戏服务器
// 得到邮件具体信息
// *************************************************************************** //
message CMessageMailRead
{	
	optional uint32 MailID		= 	1;		// 邮件ID
	optional uint32	EntityID	=	2;		// 实体ID
};

// *************************************************************************** //
// CMessageMailReadCallBack 游戏服务器->游戏客户端
// 获得邮件具体信息
// *************************************************************************** //
message CMessageMailReadResponse
{	
	optional uint32 MailID					 = 		1; 		// 邮件ID
	optional string MailBody                 = 		2;         // 邮件正文
	repeated uint32  MailItemIndex			 = 		3; 		// 附件在附件列表中的索引
	repeated PBItemObject Accessory          = 		4;         // 附件
};

// *************************************************************************** //
// CMessageGetMailAccessory  游戏客户端->游戏服务器
// 获取邮件附件
// *************************************************************************** //
message CMessageGetAccessory
{
	optional uint32 MailID       = 		1; 		// 邮件ID
	optional uint32 ItemIndex    = 		2; 		// 附件物品索引
	optional uint32 IsMoney		 = 		3; 		// 是不是金钱
	optional uint32	EntityID	 =		4;		// 实体ID		
}; 

// *************************************************************************** //
// CMessageMailReadCallBack 游戏服务器->游戏客户端
// 获得邮件具体信息
// *************************************************************************** //
message CMessageDelMail
{						
	optional uint32 MailId		 =	1;	// 邮件ID
	optional uint32 EntityID	 =	2;	// 实体ID		
};


// *************************************************************************** //
// CMessageReadMailEnd 游戏客户端->游戏服务器
// 读取邮件完毕
// *************************************************************************** //
message CMessageReadMailEnd
{
	optional uint32 MailID			 = 	1; 	// 邮件ID	
};

// *************************************************************************** //
// CMessageBackMail  游戏客户端->游戏服务器
// 退信
// *************************************************************************** //
message CMessageBackMail
{
	optional string MailBody	=	1; 				// 退信的邮件正文
	optional uint32 MailID		=	2; 				// 邮件ID
	optional uint32 EntityID	=	3;				// 实体ID			
};

// *************************************************************************** //
// CMessageDelMailResponse  游戏服务器->游戏客户端
// 删除邮件的回应消息
// *************************************************************************** //
message CMessageDelMailResponse
{
	optional uint32  MailID      = 		1; 			// 邮件ID			 	
};

// *************************************************************************** //
// CMessageGetAccessoryResponse  游戏服务器->游戏客户端
// 获取邮件附件的消息回应
// *************************************************************************** //
message CMessageGetAccessoryResponse
{
	optional uint32 ItemIndex    = 		1; 		// 附件物品索引
	optional uint32 MailID       = 		2; 		// 邮件ID	
	optional uint32 IsMoney		 = 		3; 		// 是不是金钱
	optional uint32 IfDelMail	 = 		4; 		// 是否删除邮件1表示删除邮件0表示不用删除		 	
};

// *************************************************************************** //
// CMessageBackMailResponse  游戏服务器->游戏客户端
// 退信的回应消息
// *************************************************************************** //
message CMessageBackMailResponse
{
	optional uint32 MailID       = 		1; 		// 邮件ID	
};


// *************************************************************************** //
// CMessageCannelMailSend  游戏服务器->游戏客户端
// 取消邮件发送的消息
// *************************************************************************** //
message CMessageCannelMailSend
{
	optional uint32 MailID		=	1;	// 邮件ID	
};


/*以下的消息后期用*/


// *************************************************************************** //
// CMessageLockMail  游戏客户端->游戏服务器
// 锁定邮件
// *************************************************************************** //
message CMessageLockMail
{	
	optional uint32 MailID			 = 		1; 				// 邮件ID	
};

// *************************************************************************** //
// CMessageUnLockMail  游戏客户端->游戏服务器
// 解锁邮件
// *************************************************************************** //
message CMessageUnLockMail
{
	optional uint32 MailID			 = 		1; 				// 邮件ID	
};

// *************************************************************************** //
// CMessageGetMailPageRequest  游戏客户端->游戏服务器
// 客户端的换页要求
// *************************************************************************** //
message CMessageGetMailPageRequest
{
	optional uint32  ClientCode      = 		1; 				 	
};



// *************************************************************************** //
// CMessageOpenSendMailBox
// 玩家打开送件箱的请求
// *************************************************************************** //
message CMessageOpenSendMailBox
{
	optional uint32 MsgCode  = 		1; 
};

// *************************************************************************** //
// CMessageOpenSendMailBoxResponse
// 玩家打开送件箱的回应
// *************************************************************************** //
message CMessageOpenSendMailBoxResponse
{
	optional uint32 Result  = 		1; 
};

// *************************************************************************** //
// CMessageOpenReadMailBox 
// 打开收件箱的请求
// *************************************************************************** //
message CMessageOpenReadMailBox
{
	optional uint32 MsgCode  = 		1; 
};

// *************************************************************************** //
// CMessageOpenReadMailBoxResponse
// 打开收件箱的回应
// *************************************************************************** //
message CMessageOpenReadMailBoxResponse
{
	optional uint32 Result  = 		1; 
};

// *************************************************************************** //
// CMessageMailBoxStatusNotify
// 邮件初始状态查询
// *************************************************************************** //
message CMessageMailBoxStatusNotify
{
	optional uint32 MailCount   = 		1; 					// 邮件数量不会大于客户端能够显示的数量
	optional uint32 NewMailCount  = 		2; 				// 新邮件的数量
};

// *************************************************************************** //
// CMessageMailBoxStatusNotify
// 付费邮件的付款请求
// *************************************************************************** //
message CMessagePayMailMoneyRequest
{
	optional uint32 MailID 	=	1;
};	

// *************************************************************************** //
// CMessagePayMoneyResponse
// 付费邮件的付款回应
// *************************************************************************** //
message CMessagePayMailMoneyResponse
{
	optional uint32	Errcode			=	1;		// 错误码
	optional uint32	ExpiredTime		=	2;		// 过期时间
	optional uint32	MailID			=	3;		// 邮件ID
};	
