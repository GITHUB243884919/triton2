

// 以最快速度序列/反序列
option optimize_for = SPEED;

//Proxy层面的服务器实体类型
enum EMProxyEntityType
{
	CET_PROXYSERVER	= 1; // 转发服务器
	CET_GAMESERVER	= 2; // 游戏类服务器 ( 场景服务器，副本服务器 ), 该类型服务器是最多的
	CET_DBSERVER	= 3; // 数据库类服务器
	CET_OTHERSERVER	= 4; // 其他类服务器 ( 登录服务器:1 网关服务器:2 日志服务器:3 )
	CET_GLOBALPROXY	= 6; // 全局代理服务器	
	CET_DUNGEON		= 7; // 全局副本服务器
};

enum EMProxyCmd 
{
	CMD_NULL		= 0;
	CMD_REGKEY		= 1;
	CMD_START		= 2;  
	CMD_REGSERVER   = 3;  //服务器注册
	CMD_KEEPALIVE   = 4;  //心跳
};

enum EMTransType
{
	TRANS_P2P		= 0; //点到点
	TRANS_P2G		= 2; //点到组
	TRANS_BROADCAST = 3; //同型广播
	TRANS_BYKEY		= 4; //根据关键字转发
};

// 服务器与中转服务器通讯的消息头
message CProxyHead
{
    optional uint32			TotalLength		= 1;	// 消息的长度 
    optional uint32			SrcFE			= 2;	// 从哪种类型的服务器发起
    optional uint32			SrcID			= 3;	// 从那个服务器发起
    optional uint32			DstFE			= 4;	// 到那种类型的服务器
    optional EMTransType	TransferType	= 5;
    optional uint32			DstID			= 6;	// 到哪个服务器
    optional uint32			Uin				= 7;	// 未用
    optional EMProxyCmd		OpFlag			= 8;	// 操作类型
    optional uint32			OptLength		= 9 [default = 0];
    optional bytes			Option			= 10;
	optional uint32			SrcGameID		= 11;	// 源代理服务器的GameID
	optional uint32			DesGameID		= 12;	
};
