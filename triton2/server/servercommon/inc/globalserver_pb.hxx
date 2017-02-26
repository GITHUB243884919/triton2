// 以最快速度序列/反序列
option optimize_for = SPEED;

// 全局服务器消息 0x6400 + 0x8000
enum EMGlobalServerMessageID 
{
	ID_S2D_REQUEST_GLOBALTELEPORT = 0xE400;	// 给全局服务器发送跨区的请求
	ID_D2S_RESPONSE_GLOBALTELEPORT = 0xE401;// 给全局服务器发送跨区的消息回应
	ID_D2G_REQUEST_CHANGESERVER   = 0xE402;// 全局服务器通知网关有玩家要进入
	ID_G2D_RESPONSE_CHANGESERVER  = 0xE403;// 网关通知全局服务器要跨服的玩家网关角色已经建立(可以登录)
	ID_D2G_REQUEST_CREATEREPETION = 0xE404;// 创建副本的消息请求
	ID_G2D_RESPONSE_CREATEREPETION = 0xE405;// 创建副本的消息回应

	ID_S2D_REQUEST_PLAYERAPPLYWAR  = 0xE406;// 玩家报名的请求
	ID_D2S_REQPONSE_PLAYERAPPLYWAR = 0xE407;// 玩家报名的回应
	ID_D2S_NOTIFY_WARENABLE		   = 0xE408; // 可以进入战场的通知(发往网关)
	ID_S2D_NOTIFY_PLAYERLEAVEWAR	= 0xE409; // 玩家离开战场的通知
	ID_S2D_NOTIFY_WAREND			= 0xE40A; // 战场结束的通知
	ID_S2S_NOTIFY_KICKGLOBALPLAYER	= 0xE40B; // 踢出玩家的通知
	ID_S2S_NOTIFY_KICKGLOBALPLAYERACK= 0xE40C; // 踢出玩家的通知
	ID_S2S_NOTIFY_SYNCGLOBALPLAYE	= 0xE40D; // 同步全局战场的玩家信息(清除因一些原因滞留在凌云渡中的玩家)
};

message CMapData
{
	optional uint32 MapID = 1;
	optional uint32 PosX  = 2;
	optional uint32 PosY  = 3;
	optional uint32	LineID= 4;
};

// *************************************************************************** //
// CMessageGlobalTeleportRequest  
// 给全局服务器发送跨区的请求
// *************************************************************************** //
message CMessageGlobalTeleportRequest
{
	optional uint32 ServerType = 1;	// 服务器类型，是副本服务器还是全局场景服务器
	optional uint32	WorldID	   = 2;	// 服务器区组ID		
	optional uint32	ServerID	= 3;	// 所在区组服务器的ID	
	optional uint32	RoleID		= 4;// 玩家角色ID	
	optional uint32	AccountID	= 5;// 账号ID(注册到目标网关用)

	optional uint32	OriginalServerID= 6;// 原来场景的的serverid(返回原来场景用)
	optional uint32	OriginalWorldID=7;	// 原来场景的worldid(返回原来场景用)	
	optional uint32	TeleType	=	8; // 为1表示返回原场景服务器，0表示传送到全局服务器
	optional CMapData MapData	= 9;  // 地图信息
	optional uint32	 GameID = 10;	  // 玩家账号的GameID
	optional string Mac		= 11;	  // mac地址
};

// *************************************************************************** //
// CMessageGlobalTeleportResponse  
// 给全局服务器发送跨区的消息回应
// *************************************************************************** //
message CMessageGlobalTeleportResponse
{
	optional uint32	Errcode = 1;		// 错误码
	optional string Addr	= 2;		// 服务器地址(改为网关地址)	
	optional uint32	LoginToken = 3;		
	optional uint32 RoleID = 4;			// 角色ID 
	optional uint32	LineID = 5;			// 线ID	(当目标场景在本服时需要这些信息)
	optional uint32 MapID  = 6;			// 地图ID
	optional uint32	PosX   = 7;			// 横坐标
	optional uint32	PosY	= 8;		// 纵坐标	
	optional uint64 ReptionKey = 10;	// 副本key值
	optional uint32	TeamFlag   = 11;	// 队伍标志
	optional uint32	RepetionIndex=12;	// 副本索引
	optional uint32	SceneWorldID = 13;	// 目标网关所在的世界ID
};

// *************************************************************************** //
// CMessageChangeServerRequest  
// 全局服务器通知网关有玩家要进入( 注册信息到目标网关 )
// *************************************************************************** //
message CMessageChangeServerRequest
{
	optional uint32	 RoleID			= 1;	// 角色ID
	optional uint32	 WorldID		= 2;	// 角色所在的游戏世界ID
	optional uint32	 ServerID		= 3;	// 角色所在的游戏世界服务器ID	
	optional uint32	AccountID		= 4;	// 玩家的账号ID
	optional uint32	CurrentWorldID	= 5;	// 现在所在的游戏服务器的世界ID	

	optional uint32	 MapID	  = 6;	// 要进入的地图ID
	optional uint32	 PosX	= 7;	// 要进入的地图横坐标
	optional uint32	 PosY	= 8;	// 要进入的地图纵坐标
	optional uint64	 Key	= 9;	// 如果是副本的话得有Key
	optional uint32	 ServerKey = 10;// 在全局服务器配置列表中key值
	optional uint32	 LineID  = 11;	// 要进入的服务器的线ID
	optional uint32	 TeamFlag = 12;	// 队伍标识
	optional uint32	 GameID = 13;	// 玩家账号的GameID
	optional string	Mac		= 14;	// mac地址
};

// *************************************************************************** //
// CMessageCreateGlobalRoleInfoNotify  
// 网关通知全局服务器要跨服的玩家网关角色已经建立(可以登录)
// *************************************************************************** //
message CMessageChangeServerResponse
{
	optional uint32	Errcode		=	1;	// 错误码
	optional uint32	RoleID		=	2;	// 角色ID
	optional uint32	WorldID		=	3;	// 游戏世界ID
	optional uint32	ServerKey	=	4;	// 在全局服务器配置列表中key值
	optional uint32	ServerID	=	5;	// 玩家所在服务器的ID
	optional string	Addr		=	6;	// 网关地址
	optional uint32	LoginToken	=	7;	// 目标网关返回的令牌	
};

// *************************************************************************** //
// CMessageCreateRepetionRequest  
// 创建副本的消息请求
// *************************************************************************** //
message CMessageCreateRepetionRequest
{
	optional uint32	 ReptionIndex = 1;			// 副本索引
	optional uint32	 RegKey		  = 2;			// 战场信息的注册ID
	optional uint64	 RepetionLevel= 3;			// 副本等级
};

// *************************************************************************** //
// CMessageCreateRepetionResponse  
// 创建副本的消息回应
// *************************************************************************** //
message CMessageCreateRepetionResponse
{
	optional uint32	Errcode			= 1;	// 错误码
	optional uint32	RegKey			= 2;	// 战场信息的注册ID
	optional uint64	ReptionKey		= 3;	// 创建好的副本key
};

// *************************************************************************** //
// CMessagePlayerApplyWarRequest  ID_S2D_REQUEST_PLAYERAPPLYWAR
// 玩家报名的请求
// *************************************************************************** //
message CMessagePlayerApplyWarRequest
{
	optional uint32	WorldID		= 1;		// 玩家的世界ID
	optional uint32	ServerID	= 2;		// 玩家所在的场景ID
	optional uint32	RoleID		= 3;		// 玩家的角色ID
	optional uint32	RoleLevel	= 4;		// 玩家的等级
	optional uint32	RepetionIndex = 5;		// 副本索引
};

// *************************************************************************** //
// CMessagePlayerApplyWarResponse  ID_D2S_REQPONSE_PLAYERAPPLYWAR
// 玩家报名的回应
// *************************************************************************** //
message CMessagePlayerApplyWarResponse
{
	optional uint32	Errcode = 1;
	optional uint32	RoleID = 2;	
	optional uint32	RegKey	= 3;
	optional uint32	CanStart = 4;	// 是否可以立即进入战场
	optional uint32	MinLevel = 5;	// 所在等级段的最低等级
	optional uint32	MaxLevel = 6;	// 所在等级段的最高等级
	optional uint32	TeamFlag = 7;	// 阵营ID
};

// *************************************************************************** //
// CMessageWarEnableNotify  ID_D2S_NOTIFY_WARENABLE
// 可以进入战场的通知
// *************************************************************************** //
message CMessageWarEnableNotify
{
	optional uint32	WarRegKey = 1;		// 战场注册ID
	optional uint32	RoleID	= 2;		// 角色ID
	optional uint32 TeamFlag = 3;		// 阵营ID
};

// *************************************************************************** //
// CMessagePlayerLeaveWarNotify  ID_S2D_NOTIFY_PLAYERLEAVEWAR
// 玩家离开战场的通知
// *************************************************************************** //
message CMessagePlayerLeaveWarNotify
{
	optional uint32 RoleID = 1;		// 角色ID
	optional uint32 Reason = 2;		// 离开战场的原因
};

// *************************************************************************** //
// CMessageWarEndNotify  ID_S2D_NOTIFY_WAREND
// 战场结束的通知
// *************************************************************************** //
message CMessageWarEndNotify
{
	optional uint64	ReptionKey = 1;
	optional uint32	WarRegKey = 2;
	optional uint32	Reason	= 3;	// 结束的原因
};

// *************************************************************************** //
// CMessageKickGlobalPlayerNotify  ID_S2S_NOTIFY_KICKGLOBALPLAYER
// 踢出玩家的通知
// *************************************************************************** //
message CMessageKickGlobalPlayerNotify
{
	optional uint32	RoleID = 1;
	optional uint32	Reason	= 2;	// 踢出的原因
};

// *************************************************************************** //
// CMessageKickGlobalPlayerResultNotify  ID_S2S_NOTIFY_KICKGLOBALPLAYERACK
// 踢出玩家的结果通知
// *************************************************************************** //
message CMessageKickGlobalPlayerACK
{
	optional uint32	RoleID		= 1;
	optional uint32	AccountID	= 2;	// 账号ID
	optional uint32	RoleWorldID = 3;	// 玩家所在的游戏世界ID
}

// *************************************************************************** //
// CMessageSyncGlobalPlayeNotify  ID_S2S_NOTIFY_SYNCGLOBALPLAYE
// 同步全局战场的玩家信息(清除因一些原因滞留在凌云渡中的玩家)
// *************************************************************************** //
// 每个区组在凌云渡中的玩家
message PBWorldPlayer
{
	optional uint32	WorldID = 1;
	repeated uint32	RoleID	= 2;
};
message CMessageSyncGlobalPlayeNotify
{
	repeated PBWorldPlayer WorldPlayerList = 1; // 凌云渡中各个区组玩家的信息																													
};