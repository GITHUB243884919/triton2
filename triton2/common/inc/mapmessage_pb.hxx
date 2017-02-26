
// 以最快速度序列/反序列
option optimize_for = SPEED;


// mapmessage消息段ID范围(0x0C00 -- 0x0CFF)
enum MAP_MSG_ID
{
	ID_C2S_NOTICE_ENTITYPATH					= 0x0C01; // 通用实体移动通知
	ID_S2C_NOTICE_ENTITYSTOP					= 0x0C02; // 通用实体停止移动通知
	ID_S2C_NOTICE_DESTROYENTITY					= 0x0C03; // 实体销毁通知
	ID_S2C_NOTICE_YOURMAPDATA					= 0x0C04; // 通知玩家地图数据
	ID_S2C_NOTICE_TELEPORT						= 0x0C05; // 实体传送通知 
	ID_C2S_REQUEST_PLAYERPATH					= 0x0C06; // 玩家移动请求
	ID_C2S_REQUEST_PLAYERSTOP					= 0x0C07; // 玩家停止请求
	ID_S2C_NOTICE_ENTITYPOSRESET				= 0x0C08; // 实体重置坐标通知
	ID_S2C_NOTICE_CANNTMOVE						= 0x0C09; // 实体不能移动通知
	ID_C2S_REQUEST_PETPATH						= 0x0C0A; // pet移动请求
	ID_C2S_REQUEST_PETSTOP						= 0x0C0B; // pet停止请求
};


// *************************************************************************** //
// CMessageDestroyEntityNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageDestroyEntityNotice
{
	repeated uint32 EntityID  = 		1; 
};

// *************************************************************************** //
// CMessageYourMapDataNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageYourMapDataNotice
{
	optional uint32 MotionState  = 	1; 
	optional uint32 MapID  = 		2; 
	optional uint32 LineID  = 		3; 
	optional uint32 PosX  = 		4; 
	optional uint32 CharID  = 		5; 
	optional string MapName  = 		6; 
	optional uint32 AliveState  = 	7; 
	optional uint32 PosY  = 		8; 
	optional uint32 Direction  = 	9; 
	optional string CountryName  = 	10; 
	optional uint32 VirtualMapID =	11;
};

// *************************************************************************** //
// CMessageTeleportNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageTeleportNotice
{
	optional uint32 PosX  = 		1; 
	optional uint32 PosY  = 		2; 
};


// *************************************************************************** //
// CMessagePlayerPathRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessagePlayerPathRequest
{
	optional uint32 SrcPosY  = 		1; 
	optional uint32 LineID  = 		2; 
	optional uint32 DesPosX  = 		3; 
	optional uint32 Direction  = 	4; 
	optional uint32 SrcPosX  = 		5; 
	optional uint32 DesPosY  = 		6; 
	optional uint32 MapID  = 		7; 
	optional uint64 TimeCheck	=		8;
};


// *************************************************************************** //
// CMessagePlayerStopRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessagePlayerStopRequest
{
	optional uint32 PosY  = 		1; 
	optional uint32 LineID  = 		2; 
	optional uint32 Direction  = 	3; 
	optional uint32 PosX  = 		4; 
	optional uint32 MapID  = 		5; 
	optional uint64	TimeCheck	=	6;
};


// *************************************************************************** //
// CMessageEntityPathNotice 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageEntityPathNotice
{
	optional uint32 EntityID  = 	1; 
	optional uint32 SrcPosY  = 		2; 
	optional uint32 NetSpeed  = 	3; 
	optional uint32 Direction  = 	4; 
	optional uint32 DesPosX  = 		5; 
	optional uint32 DesPosY  = 		6; 
	optional uint32 SrcPosX  = 		7; 
};

// *************************************************************************** //
// CMessageEntityStopNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageEntityStopNotice
{
	optional uint32 Direction  =	1; 
	optional uint32 NetSpeed  = 	2; 
	optional uint32 EntityID  = 	3; 
	optional uint32 PosX  = 		4; 
	optional uint32 PosY  = 		5; 
};



// *************************************************************************** //
// CMessageEntityPosReSetCallBack 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageEntityPosReSetCallBack
{
	optional uint32 Y  = 		1; 
	optional uint32 X  = 		2; 
	optional uint32 EntityID  = 3; 
};

// *************************************************************************** //
// CMessagePlayerCanntMoveCallBack 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessagePlayerCanntMoveCallBack
{
};



// *************************************************************************** //
// CMessagePetPathRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessagePetPathRequest
{
	optional uint32 SrcPosY  = 		1; 
	optional uint32 LineID  = 		2; 
	optional uint32 DesPosX  = 		3; 
	optional uint32 Direction  = 	4; 
	optional uint32 SrcPosX  = 		5; 
	optional uint32 DesPosY  = 		6; 
	optional uint32 MapID  = 		7;
	optional uint32 EntityID	=	8; // 实体ID
};


// *************************************************************************** //
// CMessagePetStopRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessagePetStopRequest
{
	optional uint32 PosY		= 	1; 
	optional uint32 LineID		= 	2; 
	optional uint32 Direction	= 	3; 
	optional uint32 PosX		= 	4; 
	optional uint32 MapID		= 	5; 
	optional uint32 EntityID	=	6; // 实体ID
};
