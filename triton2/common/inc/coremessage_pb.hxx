
// 以最快速度序列/反序列
option optimize_for = SPEED;

// core message id 枚举值
enum EMCoreMessageID
{
	ID_L2C_NOTIFY_LOGINERROR		= 0x0401; // 登陆错误通知
	ID_G2C_NOTIFY_GATEERROR			= 0x0402; // 网关错误通知
	ID_C2L_REQUEST_LOGINSERVER		= 0x0403; // 客户端登陆请求(loginserver)
	ID_L2C_NOTIFY_WAITFORQUEUE		= 0x0404; // 登录服务器通知客户端排队 
	ID_L2C_NOTIFY_REDIRECTGATE		= 0x0405; // 登陆服务器通知客户端重定向
	ID_C2G_REQUEST_LOGINGATE		= 0x0406; // 登陆网关请求
	ID_G2C_RESPONSE_LOGINGATE		= 0x0407; // 登陆网关回应
	ID_C2G_REQUEST_OBTAINCHAR		= 0x0408; // 客户端请求获取人物
	ID_G2C_NOTIFY_OBTAINCHAR		= 0x0409; // 网关通知获取人物
	ID_G2C_NOTIFY_OBTAINCHARCOMPLTE = 0x040A; // 网关通知获取人物完成
	ID_C2G_REQUEST_ENTERGAME		= 0x040B; // 客户端请求网关进入游戏
	ID_G2C_RESPONSE_ENTERGAME		= 0x040C; // 网关回应客户端进入游戏
	ID_C2S_REQUEST_LOGINSCENE		= 0x040D; // 客户端请求登陆场景
	ID_S2C_RESPONSE_LOINGSCENE		= 0x040E; // 场景回应客户端登陆
	ID_C2S_REQUEST_RETURN2GATE		= 0x040F; // 客户端请求返回网关
	ID_S2C_RESPONSE_RETURN2GATE		= 0x0410; // 回应客户端返回网关
	ID_C2G_REQUEST_CREATECHAR		= 0x0411; // 请求建立角色
	ID_G2C_RESPONSE_CREATECHAR		= 0x0412; // 回应建立角色
	ID_C2G_REQUEST_DELETECHAR		= 0x0413; // 请求删除角色
	ID_G2C_RESPONSE_DELETECHAR		= 0x0414; // 回应删除角色
	ID_S2C_NOTIFY_RECONNECTSCENE	= 0x0415; // 通知重新链接场景
	ID_C2S_REQUEST_REDIRECTSCENE	= 0x0416; // 请求重定向场景
	ID_S2C_RESPONSE_REDIRECTSCENE	= 0x0417; // 回应重定向场景
	ID_C2S_REQUEST_LEAVEGAME		= 0x0418; // 请求离开游戏
	ID_S2C_RESPONSE_LEAVEGAME		= 0x0419; // 回应离开游戏
	ID_S2C_NOTIFY_KICK				= 0x041A; // 通知客户端替人
	ID_C2S_REQUEST_PING				= 0x041B; // 客户端ping服务器
	ID_S2C_RESPONSE_PING			= 0x041C; // 服务器对ping响应
	ID_G2S_NOTIFY_KICKPLAYER		= 0x041D; // 网关踢人(可用于重复登陆时踢掉上一个玩家)
	ID_S2C_REDIRECT_ERR_CODE        = 0x041E;	
    ID_G2S_NOTIFY_LOGINQUEUE        = 0x041F;	// 网关通知客户端排队    
    ID_C2G_REQUEST_CHECKROLEPWD		= 0x0420;	// 检查密码的请求
    ID_G2C_RESPONSE_CHECKROLEPWD	= 0x0421;	// 检查密码的回应
	ID_C2S_REQUEST_CLIENTINFO		= 0x0422;	// 发送客户端主机信息
	ID_S2G_CANCEL_LOGINQUEUE        = 0x0423;	// 客户端取消排队
	ID_C2S_NOTIFY_CLIENTANTIBOTDATA	= 0x0424;	// 客户端->服务器 反外挂任意数据
	ID_S2C_NOTIFY_SERVERANTIBOTDATA	= 0x0425;	// 服务器->客户端 反外挂任意数据
	ID_C2S_NOTIFY_CLIENTANTIBOTSTARTRET = 0x0426;// 客户端->服务器 客户端反外挂启动结果	
    ID_S2C_NOTIFY_DISCONNECT        = 0x0427;   // 服务器->客户端 通知客户端断开场景连接
    ID_S2C_NOTIFY_USERAUTH          = 0x0428;   // 服务器->客户端 操作需要客户端输入鉴权码
    ID_C2S_REQUEST_USERAUTH         = 0x0429;   // 客户端->服务器 鉴权请求
    ID_S2C_RESPONSE_USERAUTH        = 0x0430;   // 服务器->客户端 鉴权回应
    
    ID_C2G_REQUEST_CANCELROLEDELETE	= 0x0431;	// 客户端->服务器 恢复删除角色
    ID_G2C_RESPONSE_CANCELROLEDELETE= 0x0432;	// 客户端->服务器 恢复删除角色

// 100304 MAOCY ADD-BEGIN
    ID_C2G_REQUEST_QUESTIONREQUIRE  = 0x0433;	// 客户端->服务器 请求问题验证
    ID_G2C_RESPONSE_QUESTIONREQUIRE = 0x0434;	// 服务器->客户端 给出问题
    ID_C2G_REQUEST_QUESTIONANSWER   = 0x0435;	// 客户端->服务器 请求答案验证
    ID_G2C_RESPONSE_QUESTIONANSWER  = 0x0436;	// 服务器->客户端 给出答案
// 100304 MAOCY ADD-END

// 100325 MAOCY ADD-BEGIN
   // 用户认证
   ID_C2A_REQUEST_CERTIFY       = 0x0440; // 用户到全局服务器认证请求
   ID_A2C_RESPONSE_CERTIFY      = 0x0441; // 用户到全局服务器认证应答
   ID_A2L_REQUEST_CERTIFYQUERY  = 0x0442; // 全局服务器到登录服务器认证请求
   ID_L2A_RESPONSE_CERTIFYQUERY = 0x0443; // 登录服务器到全局服务器认证应答
   ID_L2A_REQUEST_CERTIFYTOKEN  = 0x0444; // 登录服务器到全局服务器认证令牌请求
   ID_A2L_RESPONSE_CERTIFYTOKEN = 0x0445; // 登录服务器到全局服务器认证令牌应答
   // 角色数目查询
   ID_L2G_REQUEST_ROLEQUERY     = 0x0446; // 登录服务器去网关服务器查询角色信息请求
   ID_G2L_RESPONSE_ROLEQUERY    = 0x0447; // 登录服务器去网关服务器查询角色信息应答
   ID_L2A_NOTIFY_ROLEQUERY      = 0x0448; // 登录服务器向全局服务器通知角色信息
   // 服务器信息通知
   ID_L2A_NOTIFY_ONLINESTATUS   = 0x0449; // 服务器信息通知
// 100325 MAOCY ADD-END


   ///////////////////////////////////
   // 消息：转阵营（转服）消息定义	
   // author： qiufufu
   // date : 10-08-23
   //////////////////////////////////////

   ID_C2S_REQUEST_INVITE_CAMP			= 0x0450; // 客户端申请转阵营
   ID_S2C_RSPONSE_INVITE_CAMP			= 0x0451; // 服务端回复转阵营 把可转的阵营列表通知给客户端

   ID_C2S_REQUEST_CHANGE_CAMP			= 0x0452; // 客户端选择一个阵营，通知服务端转阵营
   ID_S2C_RSPONSE_CHANGE_CAMP			= 0x0453; // 服务端回复客户端 转阵营是否成功

   ID_S2C_NOTIFY_CHANGE_CAMP_SUCCESED   = 0x0454; // 世	界频道通知玩家转阵营成功
   ID_S2G_CHANGE_CAMP_NOTICE            = 0x0455; // scenserver 转换成功后通知 gateserver

   ID_C2S_REQUEST_CHANGE_CAMP_CONDITION = 0x0456; // 客户端发送查询阵营转换条件
   ID_S2C_RSPONSE_CHANGE_CAMP_CONDITION = 0x0457; // 服务端回复查询阵营转换条件

};

// 路由消息
message CMessageRouter
{
	enum MSGID { MSGROUTER = 0x0200; };

	enum EMRouterType
	{
		ROUTER_NAME = 0;
		ROUTER_CHAR = 1;
	};

	enum EMRouterReason
	{
		REASON_SUCCESS	= 0;		// 转发成功
		REASON_REDIRECT	= 1;		// 转发时，玩家正在跨场景
		REASON_OFFLINE	= 2;		// 转发时，玩家不在线
	};


	optional uint32	RouterReason	= 1;	// 是否转发成功
	optional uint32	Send2Player		= 2;	// 是否要发送给客户端
	optional uint32	SendBack		= 3;	// 是否回发消息
	optional uint32	RouterType		= 4;	// 转发类型
	optional uint32 SrcCharID		= 5;	// 发送角色ID, 如果目标为0, 表示发送给全服务器
	optional uint32 DesCharID		= 6;	// 目标角色ID
	optional string	SrcName			= 7;	// 发送角色名字
	optional string	DesName			= 8;	// 目标角色名字
};


message CMessageRedirectErr
{
	optional int32 ErrorCode  = 		1; 
	optional uint32 CharID     = 		2; 
}

// *************************************************************************** //
// CMessageLoginErrorNotice 登陆服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageLoginErrorNotice
{
	optional string ErrorString  = 		1; 		// 错误字符串
	optional int32 ErrorCode	 = 		2; 		// 系统错误码
};


// *************************************************************************** //
// CMessageGateErrorNotice 网关服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageGateErrorNotice 
{
	optional int32 KeepAlive     = 		1;          // 是否与客户端断开链接
	optional string ErrorString	 = 		2; 			// 错误字符串
	optional int32 ErrorCode	 = 		3; 			// 系统错误码
}


// ********************************************************************** //
// CMessageLoginServerRequest　游戏客户端 -> 登陆服务器 
// 客户端登陆请求
// ********************************************************************** //
message CMessageLoginServerRequest
{
   // 103031 [GS] MAOCY ADD-BEGIN
   enum EType
   {
      TYPE_CLIENT = 0; // 客户端认证
      TYPE_GLOBAL = 1; // 全局服务器认证
   };
   // 103031 [GS] MAOCY ADD-END
   optional string UserName          =  1; // 玩家名称
   optional string Md5Code           =  2; // 客户端MD5串 
   optional uint32 TaskVersion       =  3; // 任务文件版本号
   optional string UserPsw           =  4; // 玩家密码
   optional uint32 Version           =  5; // 客户端版本号	
   optional int32  SourceID          =  6; // 安装源
   optional uint32 TemplateVersion   =  7; // 模板文件版本号
   optional uint32 MatrixType        =  8; // 鉴权方式
   optional string MatrixCode        =  9; // 鉴权码
   optional string Mac               = 10; // 客户端MAC地址
   optional string MatrixCoord       = 11; // 密保卡坐标
   // 103031 [GS] MAOCY ADD-BEGIN
   optional EType  type              = 12; // 认证类型
   optional uint32 accountId         = 13; // 账号编号
   optional uint32 token             = 14; // 令牌
   // 103031 [GS] MAOCY ADD-END
   optional uint32 cpuid             = 15; // CPUID
   optional uint32 diskid            = 16; // DISK序列
};


// ********************************************************************** //
// CMessageLoginServerResponse　登陆服务器 -> 游戏客户端
// 登陆服务器通知客户端排队
// ********************************************************************** //
message CMessageWaitForLoginNotice
{
	optional uint32	QueueNumber	 = 		1; 	// 排队顺序
};


// ********************************************************************** //
// CMessageRedirectGateNotice　登陆服务器 -> 游戏客户端
// 通知客户端重定向
// ********************************************************************** //
message CMessageRedirectGateNotice
{
	optional string	Address		 = 		1; 
	optional uint32 OtherClient	 = 		2; 
	optional uint32 LoginToken	 = 		3; 
	optional uint32	AccountID	 = 		4; 
	optional uint32	CleanCSkey	 =		5; // 是否清除cskey
};


// ********************************************************************** //
// CMessageEnterGateRequest　网关服务器 -> 游戏客户端   
// ********************************************************************** //
message CMessageLoginGateRequest
{

	optional uint32 LoginToken	 = 		1; 		// 登陆令牌
	optional uint32	AccountID	 = 		2; 		// 帐号ID
	optional uint32 TemplateVer	 = 		3; 		// 模板号
};


// ********************************************************************** //
// CMessageLoginGateResponse　网关服务器 -> 游戏客户端   
// ********************************************************************** //
message CMessageLoginGateResponse
{
	optional string     ErrorString =  1; 		// 错误字符串( 只要错误字符串不为空，就显示字符串 )
	optional int32     ErrorCode   =  2; 		// 错误码
	optional int32     GateWayID   =  3; 		// 网关ID
};


// ********************************************************************** //
// CMessageObtainCharRequest　游戏客户端 -> 网关服务器 
// ********************************************************************** //
message CMessageObtainCharRequest
{
};


// 
message PBCharProfile
{
	optional uint32	CharID				= 		1; 	// 角色ID
	optional uint32 LineID				= 		2; 	// 所在线ID
	optional uint32 Sex					= 		3; 	// 角色性别
	optional uint32 HairColour			= 		4; 	// 角色发色
	optional uint32 Exp					= 		5; 	// 角色经验
	optional uint32 Level				= 		6; 	// 角色等级
	optional uint32 AccountID			= 		7; 	// 帐号ID
	optional uint32 LastLogin			= 		8; 	// 角色上次登陆时间
	optional uint32 OnlineTime			= 		9; 	// 角色在线时间
	optional uint32 Metier				= 		10; 	// 角色职业
	optional uint32 HairType			= 		11; 	// 角色发型
	optional uint32 FaceType			= 		12; 	// 角色脸型
	optional uint32 MapID				= 		13; 	// 所在地图ID
	optional string CharName			= 		14; 	// 角色姓名
	optional uint32 Nationality			= 		15; 	// 角色国籍
	optional uint32	RoleDeleteStatus	=		16;		// 角色删除状态EM_RoleDelteStatus
	optional uint32	DeleteStatusLeftTime=		17;		// 当前角色删除状态的剩余时间  	
	optional uint32	PasswordUsed		=		18;		// 是否使用了财产密码0为没有使用其他为使用	
};



// ********************************************************************** //
// CMessageObtainCharNotice　网关服务器 -> 游戏客户端
// 角色概况通知
// ********************************************************************** //
message CMessageObtainCharNotice
{
	repeated PBCharProfile	CharProfile	 = 		1;
	optional uint32			LastLoginIP	 =		2;		// 上次的登录IP 
	optional uint32			LoginIP		 =		3;		// 本次的登录IP
};


// ********************************************************************** //
// CMessageObtainCharCompleteNotice　网关服务器 -> 游戏客户端
// 角色概况获取完成通知
// ********************************************************************** //
message CMessageObtainCharCompleteNotice
{
};



// ********************************************************************** //
// CMessageEnterGameRequest　游戏客户端 -> 网关服务器 
// ********************************************************************** //
message CMessageEnterGameRequest
{
	optional uint32 AccountID  = 	1;      // 帐号ID
	optional uint32 LineID	 = 		2; 		// 场景对象ID
	optional uint32 MapID	 = 		3; 		// 地图ID
	optional uint32 CharID	 = 		4; 		// 角色ID
    optional string Mac      =      5;      // 客户端MAC地址
};


// ********************************************************************** //
// CMessageEnterGameResponse　网关服务器 -> 游戏客户端 
// ********************************************************************** //
message CMessageEnterGameResponse
{
	optional string SceneName	 = 		1; 
	optional string	Address		 = 		2; 
	optional uint32	ClearDialog	 =		3;	// 清除对话
};


// ********************************************************************** //
// CMessageLoginSceneRequest　游戏客户端 -> 场景服务器
// ********************************************************************** //
message CMessageLoginSceneRequest
{
	optional uint32 AccountID		 = 		1; 
	optional uint32 CharID			 = 		2; 
	optional uint32 LoginToken		 = 		3; 
};


// ********************************************************************** //
// CMessageEnterSceneResponse　场景服务器 -> 游戏客户端 
// ********************************************************************** //
message CMessageLoginSceneResponse
{
	optional bytes	Key			= 1;	// 密钥16, 约定俗成， 16位
	optional uint32 CSDialogID	= 2;	// 会话ID
	optional uint64 ServerTime	= 3;	// 服务器时间,毫秒
};

// ********************************************************************** //
// CMessageEnterSceneResponse　游戏客户端 -> 场景服务器
// 客户端请求返回网关
// ********************************************************************** //
message CMessageReturn2GateRequest
{
};

// ********************************************************************** //
// CMessageReturnToGateResponse　场景服务器  -> 游戏客户端
// 客户端请求返回网关
// ********************************************************************** //
message CMessageReturn2GateResponse
{
	optional uint32	ReturnType = 1; // 0 表示正常返回 1表示要返回原来网关，客户端要做一些操作
};

// ********************************************************************** //
// CMessageCreateCharRequest　游戏客户端 -> 网关服务器
// 玩家请求建立角色
// ********************************************************************** //
message CMessageCreateCharRequest
{
	optional PBCharProfile	Profile		 = 		1; 		// 角色概要
};

// ********************************************************************** //
// CMessageCreateCharResponse　网关服务器 -> 游戏客户端
// 玩家请求建立角色回应
// ********************************************************************** //
message CMessageCreateCharResponse
{
	optional uint32		CharID  = 		1; 
};



// ********************************************************************** //
// CMessageDeleteCharRequest　游戏客户端 -> 网关服务器
// 玩家请求建立角色
// ********************************************************************** //
message CMessageDeleteCharRequest
{
	optional uint32		PasswdType  = 		1; 
	optional string		Passwd	 = 		2; 
	optional uint32		CharID	 = 		3; 
};

// ********************************************************************** //
// CMessageDeleteCharResponse　网关服务器 -> 游戏客户端
// 玩家请求建立角色回应
// ********************************************************************** //
message CMessageDeleteCharResponse
{
	optional uint32		CharID					=	1;
	optional uint32		Errcode					=	2;	
	optional uint32		DeleteStatus			=	3;	// 删除状态
	optional uint32		DeleteStatusLeftTime	=	4;	// 当前删除状态的剩余时间
};

// ********************************************************************** //
// CMessageRedirectNotice　游戏服务器 -> 游戏客户端
// 通知玩家连接其它游戏服务器
// ********************************************************************** //
message CMessageReconnectSceneNotice 
{
	optional uint32 MapID		 = 		1; 
	optional uint32 LineID		 = 		2; 
	optional string Address		 = 		3; 
	optional uint32 PosY		 = 		4; 
	optional uint32 PosX		 = 		5; 
	optional string CountryName	 = 		6; 
	optional string MapName		 = 		7; 	
};

// ********************************************************************** //
// CMessageLoginSceneRequest　游戏客户端 -> 场景服务器
// ********************************************************************** //
message CMessageRedirectSceneRequest
{
	optional uint32		AccountID	 = 		1; 
	optional uint32		LoginToken	 = 		2; 
	optional uint32		CharID		 = 		3; 
};

// ********************************************************************** //
// CMessageRedirectSceneResponse　游戏客户端 -> 场景服务器
// ********************************************************************** //
message CMessageRedirectSceneResponse
{
};


// ********************************************************************** //
// CMessageLeaveGameRequest　游戏客户端 -> 场景服务器
// ********************************************************************** //
message CMessageLeaveGameRequest
{
	optional uint32 LeaveType	 = 		1;  // 下线方式：0 安全下线，1 强行下线
};

// ********************************************************************** //
// CMessageLeaveGameResponse　场景服务器 -> 游戏客户端
// ********************************************************************** //
message CMessageLeaveGameResponse 
{
};

// ********************************************************************** //
// CMessageKickoutNotice　场景服务器 -> 游戏客户端
// ********************************************************************** //
message CMessageKickoutNotice
{
	optional uint32 reason  = 		1; 
	optional uint32 param  = 		2; 
};

// ********************************************************************** //
// CMessageClientPingServerRequest　 游戏客户端 -> 服务器
// ********************************************************************** //
message CMessageClientPingServerRequest
{

	optional uint32 CharID			 = 		1; 	// 角色ID
	optional uint32 LatestPingValue	 = 		2; 	// client最近一次ping值( server相信客户端的值,网络波动小的话比较精确 )
	optional uint64 ClientLocalTime	 = 		3; 	// client本地时间
};


// ********************************************************************** //
// CMessageClientPingServerResponse　服务器 -> 游戏客户端
// ********************************************************************** //
message CMessageClientPingServerResponse
{
	optional uint32 CharID			 = 		1; 	// 角色ID
	optional uint64 ClientLocalTime	 = 		2; 	// 请求消息中的client本地时间，服务器直接回填
	optional uint64 ServerTime		 =		3;	// 服务器时间,毫秒
};

// ********************************************************************** //
// CMessageGateKickPlayerNotify　服务器 -> 游戏客户端
// ********************************************************************** //
message CMessageGateKickPlayerNotify
{
	optional uint32	CharID		 = 		1; 	// 角色ID
	optional uint32	KickReason	 = 		2; 	// 踢人原因
};

// ********************************************************************** //
// CMessageQueuePlayerNotify　网关服务器 -> 游戏客户端
// ********************************************************************** //
message CMessageQueuePlayerNotify
{
	optional uint32	CountInScene	 = 		1; 	// 目前场景人数
	optional uint32	ElapsSeconds	 = 		2; 	// 客户端排队已经排队的时间秒数
	optional uint32	NeedSeconds	 = 		3; 	// 客户端大概需要多长时间排队
	optional uint32	YourPosition	 = 		4; 	// 目前排队人数
	optional uint32	CountInQueue	 = 		5; 	// 目前排队人数
};

message CMessagePlayerCancelLoginQueue
{

};

// ********************************************************************** //
// CMessageCheckRolePwdRequest　 游戏客户端 -> 网关服务器 删除角色时检查2级密码的请求
// ********************************************************************** //
message CMessageCheckRolePwdRequest
{
	optional uint32 CharID	  = 		1; 
	optional string Password  = 		2; 	
};

// ********************************************************************** //
// CMessageCheckRolePwdRequest　 网关服务器 -> 游戏客户端 删除角色时检查2级密码的回应
// ********************************************************************** //
message CMessageCheckRolePwdResponse
{
	optional uint32 Errcode  = 		1; 	// 0表示密码正确 其他表示密码错误
};

//**********************************
// CMessageSendClientInfoRequest 游戏客户端 -> 场景服务器 发送客户端主机信息
//**********************************

message CMessageSendClientInfoRequest
{
	optional string		HostInfo	 = 		1; 	// 主机信息
	optional string		MacAddr		 = 		2; 	// 主机Mac地址
};


//**********************************
// CMessageSendAntiBotDataToServerNotify 游戏客户端 -> 场景服务器 发送客户端反外挂数据消息
//**********************************
message CMessageSendAntiBotDataToServerNotify
{
	optional uint32		Length		 = 		1; 	// 数据长度
	optional bytes		ClientData	 = 		2; 	// 数据
};


//**********************************
// CMessageSendAntiBotDataToClientNotify	游戏服务器 -> 客户端 发送服务器反外挂数据消息
//**********************************
message CMessageSendAntiBotDataToClientNotify
{
	optional bytes		ServerData	 = 		1; 	// 数据
	optional uint32		Length		 = 		2; 	// 数据长度
};


//**********************************
// CMessageClientAntiBotStartRetNotify	游戏客户端 -> 服务器 发送客户端反外挂模块启动返回值
//**********************************
message CMessageClientAntiBotStartRetNotify
{
	optional uint32		StartRet	 = 		1; 	// 反外挂模块启动返回值
};

//**********************************
// CMessageDisconnectNotify	            服务器 -> 客户端 发送服务器主动断开连接消息, 用户服务器的控制。客户端不需要响应
//**********************************
message CMessageDisconnectNotify
{
};

// 服务器向客户端发送某行为需要进行鉴权的通知
message CMessageUserAuthNotify
{
    optional uint32 ActionID = 1;
    optional uint32 AuthType = 2;
};

// 客户端根据鉴权类型决定要让玩家如何操作, 并将玩家的操作返回给服务器
message CMessageUserAuthRequest
{
    optional uint32 ActionID = 1;
    optional uint32 AuthValue = 2;
};

// 服务器返回是否允许此操作
message CMessageUserAuthResponse
{
    optional uint32 ActionID = 1;
    optional uint32 OK = 2;
};


// 恢复删除角色的请求
message CMessageCancelDeleteRoleRequest
{
	 optional uint32 RoleID = 1;
};

// 恢复删除角色的回应
message CMessageCancelDeleteRoleResponse
{
	optional uint32 Errcode		= 1;	// 错误码
	optional uint32 RoleID		= 2;	// 角色ID	
};

// 100304 MAOCY ADD-BEGIN
//============================================================
// 客户端->服务器 请求问题验证
message CMessageQuestionRequireRequest
{
};

//============================================================
// 服务器->客户端 给出问题
message CMessageQuestionRequireResponse
{
	//------------------------------------------------------------
	// 状态
	enum EStatus
	{
		STATUS_SUCCESS	= 0;		// 不需要回答
		STATUS_WAIT = 1;		    // 等待回答
	};
	//------------------------------------------------------------
	optional EStatus Status    = 1; // 状态
	optional uint32 QuestionId = 2; // 问题编号
	optional string Question   = 3; // 问题内容
	optional bytes OptionA     = 4; // 选项A
	optional bytes OptionB     = 5; // 选项B
	optional bytes OptionC     = 6; // 选项C
	optional bytes OptionD     = 7; // 选项D
};

//============================================================
// 客户端->服务器 请求答案验证
message CMessageQuestionAnswerRequest
{
	//------------------------------------------------------------
	// 状态
	enum EStatus
	{
		STATUS_SUCCESS	= 0;		// 不需要回答
		STATUS_WAIT = 1;		    // 等待回答
	};
	//------------------------------------------------------------
	optional uint32 QuestionId = 1; // 问题编号
	optional bytes Answer      = 2; // 回答项
};

//============================================================
// 服务器->客户端 给出答案
message CMessageQuestionAnswerResponse
{
	//------------------------------------------------------------
	// 状态
	enum EStatus
	{
		STATUS_SUCCESS	= 0;		// 不需要回答
		STATUS_WAIT = 1;		    // 等待回答
	};
	//------------------------------------------------------------
	optional EStatus Status    = 1; // 状态
};
// 100304 MAOCY ADD-END

// 100325 MAOCY ADD-BEGIN
//============================================================
// 认证服务器状态
message CMessageLoginServerInfo{
   optional uint32 id     = 1; // 服务器编号
   optional uint32 flag   = 2; // 服务器标志
   optional uint32 status = 3; // 服务器状态
   optional uint32 count  = 4; // 角色个数
};

//============================================================
// 用户到全局服务器认证请求
message CMessageCertifyRequest{
   optional string passport        =  1; // 登录账号
   optional string password        =  2; // 登录密码
   optional string md5Code         =  3; // 客户端MD5串 
   optional string mac             =  4; // 客户端MAC地址
   optional int32  sourceId        =  5; // 安装源
   optional uint32 version         =  6; // 客户端版本号	
   optional uint32 taskVersion     =  7; // 任务文件版本号
   optional uint32 templateVersion =  8; // 模板文件版本号
   optional uint32 matrixType      =  9; // 鉴权方式
   optional string matrixCode      = 10; // 鉴权码
   optional string matrixCoord     = 11; // 密保卡坐标
};

//============================================================
// 用户到全局服务器认证应答
message CMessageCertifyResponse
{
   //------------------------------------------------------------
   // 状态
   enum EStatus
   {
      STATUS_SUCCESS = 0; // 验证成功
      STATUS_INVALID = 1; // 验证失败
      STATUS_FAILURE = 2; // 执行失败
   };
   //------------------------------------------------------------
   optional EStatus                 status    = 1; // 状态
   optional uint32                  playerId  = 2; // 玩家编号
   optional uint32                  accountId = 3; // 账号编号
   optional uint32                  token     = 4; // 令牌
   repeated CMessageLoginServerInfo servers   = 5; // 服务器信息
   optional uint32                  errorCode = 6; // 错误代码
};

//============================================================
// 全局服务器到登录服务器认证请求
message CMessageCertifyQueryRequest
{
   optional uint32 playerId        =  1; // 玩家编号
   optional uint32 accountId       =  2; // 账号编号
   optional string passport        =  3; // 登录账号
   optional string password        =  4; // 登录密码
   optional string md5Code         =  5; // 客户端MD5串 
   optional uint32 playerHost      =  6; // 客户端地址
   optional uint32 playerPort      =  7; // 客户端端口
   optional string playerMac       =  8; // 客户端MAC地址
   optional int32  sourceId        =  9; // 安装源
   optional uint32 version         = 10; // 客户端版本号	
   optional uint32 taskVersion     = 11; // 任务文件版本号
   optional uint32 templateVersion = 12; // 模板文件版本号
   optional uint32 matrixType      = 13; // 鉴权方式
   optional string matrixCode      = 14; // 鉴权码
   optional string matrixCoord     = 15; // 密保卡坐标
};

//============================================================
// 登录服务器到全局服务器认证应答
message CMessageCertifyQueryResponse
{
   //------------------------------------------------------------
   // 状态
   enum EStatus
   {
      STATUS_SUCCESS = 0; // 验证成功
      STATUS_FAILURE = 1; // 执行失败
   };
   //------------------------------------------------------------
   optional EStatus status    = 1; // 状态
   optional uint32  playerId  = 2; // 玩家编号
   optional uint32  accountId = 3; // 账号编号
   optional uint32  errorCode = 4; // 错误代码
};

//============================================================
// 登录服务器到全局服务器认证令牌请求
message CMessageCertifyTokenRequest
{
   optional uint32 playerId  = 1; // 玩家编号
   optional uint32 accountId = 2; // 账号编号
   optional uint32 token     = 3; // 登录账号
   optional uint32 handle    = 4; // 句柄
};

//============================================================
// 登录服务器到全局服务器认证令牌应答
message CMessageCertifyTokenResponse
{
   //------------------------------------------------------------
   // 状态
   enum EStatus
   {
      STATUS_SUCCESS = 0; // 验证成功
      STATUS_FAILURE = 1; // 执行失败
   };
   //------------------------------------------------------------
   optional EStatus status          =  1; // 状态
   optional uint32  playerId        =  2; // 玩家编号
   optional uint32  accountId       =  3; // 账号编号
   optional uint32  handle          =  4; // 句柄
   optional string  passport        =  5; // 登录账号
   optional string  password        =  6; // 登录密码
   optional uint32  playerHost      =  7; // 客户端地址
   optional uint32  playerPort      =  8; // 客户端端口
   optional string  playerMac       =  9; // 客户端MAC地址
   optional string  md5Code         = 10; // 客户端MD5串 
   optional int32   sourceId        = 11; // 安装源
   optional uint32  version         = 12; // 客户端版本号	
   optional uint32  taskVersion     = 13; // 任务文件版本号
   optional uint32  templateVersion = 14; // 模板文件版本号
   optional uint32  matrixType      = 15; // 鉴权方式
   optional string  matrixCode      = 16; // 鉴权码
   optional string  matrixCoord     = 17; // 密保卡坐标
};

//============================================================
// 登录服务器去网关服务器查询角色信息请求
message CMessageRoleQueryRequest
{
   optional uint32 playerId  = 1; // 玩家编号
   optional uint32 accountId = 2; // 账号编号
};

//============================================================
// 网关服务器到登录服务器查询角色信息请求应答
message CMessageRoleQueryResponse
{
   //------------------------------------------------------------
   // 状态
   enum EStatus
   {
      STATUS_SUCCESS = 0; // 成功
      STATUS_FAILURE = 1; // 失败
   };
   //------------------------------------------------------------
   optional EStatus status    = 1; // 状态
   optional uint32  playerId  = 2; // 玩家编号
   optional uint32  accountId = 3; // 账号编号
   optional uint32  roleCount = 4; // 角色个数
};

//============================================================
// 登录服务器向全局服务器通知角色信息通知
message CMessageRoleQueryNotify
{
   //------------------------------------------------------------
   // 状态
   enum EStatus
   {
      STATUS_SUCCESS = 0; // 成功
      STATUS_FAILURE = 1; // 失败
   };
   //------------------------------------------------------------
   optional EStatus status    = 1; // 状态
   optional uint32  playerId  = 2; // 玩家编号
   optional uint32  accountId = 3; // 账号编号
   optional uint32  loginId   = 4; // 账号编号
   optional uint32  roleCount = 5; // 角色个数
};

//============================================================
// 服务器信息通知
message CMessageOnlineStatusNotify
{
   optional uint32 serverId = 1; // 服务器编号
   optional uint32 total    = 2; // 总人数
   optional uint32 current  = 3; // 当前人数
};
// 100325 MAOCY ADD-END


///////////////////////////////////
// 消息：转阵营（转服）消息定义	
// author： qiufufu
// date : 10-08-23
//////////////////////////////////////

message CMessageInviteCampRequest// 客户端申请转阵营
{
	optional uint32  playerId        = 1; // 玩家编号
	optional uint32  currentCampId   = 2; //玩家当前的阵营id
	optional uint32  npcId			 = 3; //npcid
};
message  CampLevel
{
	optional uint32 camp_id			 = 1;	// 阵营ID
	optional uint32 weak_level		 = 2;	// 阵营等级
};
message CMessageInviteCampResponse			// 服务端回复转阵营 把可转的阵营列表通知给客户端
{
	repeated CampLevel  camp_list     = 1;  //可转阵营的列表
	optional uint32		errorcode       = 2;  //0 为成功，其他为失败

};

message CMessageChangeSingleCampRequest // 客户端选择一个阵营，通知服务端转阵营
{
	optional uint32  playerId        = 1; // 玩家编号
	optional  uint32 crrentCampId    = 2; //当前的阵营id
	optional  uint32 targetCampId    = 3; //目标的阵营id
	optional uint32  npcId			 = 4; //npcid
};

message CMessageChangeSingleCampResponse// 服务端回复客户端 转阵营是否成功
{
	optional uint32		 campId			 = 1; //转换后的阵营id 
	optional uint32		 errorcode		 = 2; //0为成功，其他为失败
	optional uint32      playerId		 = 3; //角色 id

};

message CMessageChangeCampSuccesNotify // 世界频道通知玩家转阵营成功
{	
	optional string  playerName  = 1; // 玩家名字
	optional uint32  campId      = 2; // 成功转入的阵营 
	optional uint32  srcCampId   = 3; // 原始玩家阵营id
	optional uint32  playerId    = 4; // 玩家id

};
message CMessageChangeCamp2GateNotice //scenserver 转换成功后通知 gateserver
{

	optional uint32  playerId    = 1; // 玩家编号
	optional uint32  campId		 = 2; //玩家转换后的阵营id
};

message CMessageChangeCampConditionRequest // 客户端发送查询阵营转换条件
{
	optional uint32  playerId    = 1; //玩家编号
	optional uint32  npcId		 = 2; //npcid
};

message CMessageChangeCampConditionResponse // 服务端回复查询阵营转换条件
{
	    
	optional uint32  isHaveTeam		 = 1; //是否有组队
	optional uint32  isHaveFamily	 = 2; //是否有家族
	optional uint32  isHaveCountry	 = 3; //是否有国家
	optional uint32  isHaveMaster    = 4; //是否有师徒
	optional uint32  isHaveMarriage  = 5; //是否有结婚
	optional uint32  isHavePayMail   = 6; //是否有付费邮件
	optional uint32  ishaveEnoughLevel =7; //是否达到足够的等级
	optional uint32  errorcode		 = 8; //0为成功，其他为失败
	optional uint32	 isHaveSworn	 = 9;	// 是否有结拜
};


