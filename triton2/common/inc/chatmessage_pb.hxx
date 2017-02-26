import "coremessage_pb.hxx";
import "property_pb.hxx";
// 以最快速度序列/反序列
option optimize_for = SPEED;


// ********************************************************************** //
// ChatMessageID  聊天模块消息id
// 消息ID+0x0200表示该消息为路由消息
// ********************************************************************** //
enum ChatMessageID
{
	CHATMSG                       = 0x1400;        
	ID_C2S_CMESSCHATSEND          = 0x1401;          
	ID_S2C_CHATSENDCALLBACK       = 0x1402;        //0x1401 + 0x0200 
	ID_S2C_ROUTERCHATMESSAGE      = 0x1403;        //0x1403 + 0x0200
	ID_S2C_CHATRES2CLIENT         = 0x1404;  
	ID_C2S_NOTIFY_AUTOCHATMESSAGE = 0x1405;
	ID_C2S_NOTIFY_STOPAUTOCHAT    = 0x1406;
	ID_C2S_NOTIFY_USECHATITEM     = 0x1407;
	ID_C2S_NOTIFY_CLICKBUGLEITEM  = 0x1408;
	ID_C2S_REQUEST_CHECKDIRTYWORD = 0x1409;
	ID_S2C_NOTIFY_HASDIRTYWORD    = 0x140A;
};


// ********************************************************************** //
// CMessageChatSend  游戏客户端->游戏服务器
// 玩家发送频道聊天消息
// ********************************************************************** //
message CMessageChatSend
{
	optional string Msg				 = 		1;	// 消息内容
	optional string PlayerID		 = 		2;	// 目标玩家CharID 
	optional string PlayerName		 = 		3;	// 目标玩家名字
	optional uint32 Channel			 = 		4;	// 聊天频道
	repeated uint32 PkgIndex		=		5;	// 包裹索引(如：仓库，背包，任务包，身上装备等）
	repeated uint32 ItemIndex		=		6;	// 物品在包裹中的索引
	repeated uint32 ItemSequence	=		7;	// 道具序列（客户端专用）
};

// ********************************************************************** //
// CMessageChatSendCallBack　游戏服务器 -> 游戏客户端
// 将玩家的聊天信息发送给客户端
// ********************************************************************** //
message CMessageChatSendCallBack
{	
	optional CMessageRouter Router  = 		1;    
	optional uint32	RepeatedNum	    = 		2; 						   // 系统公告重复播放的次数
	optional uint32 ShowInTable     = 		3;                         // 是否在普通界面显示	
	optional string Msg             = 		4;                         // 消息内容		
	optional uint32 Channel         = 		5;                         // 聊天频道	
	repeated PBItemObject ItemInfo	=		6;							// 聊天里面的物品消息
	optional uint32 CountryID       = 		7; 						   // 国家ID,国家频道用	             
	optional uint32 NpcShoutFlag    = 		8; 						   // 如果是1个话 则表示发送的是NPC策略xml中的key值(带#) 由客户端来转义
	optional uint32	FlagID			=		9;						   // 家族ID或者军团ID家族频道和军团频道用或者队伍ID
	optional uint32	UserType		=		10;						   // 发言者的用户类型 见protype里的 enUserType枚举	
	optional uint32	DesUserType		=		11;						   // 接收者的用户类型 见protype里的 enUserType枚举	
	optional uint32	UseFlag			=		12;						   // 发言者具有特殊身份时的身份ID
	optional uint32	CampID			=		13;						   // 阵营ID
};

// ********************************************************************** //
// CMessageRouterChatMsg 　游戏服务器 -> 转发服务器
// 转发发送需要跨服的消息
// ********************************************************************** //
message CMessageRouterChatMsg 
{   
	optional CMessageRouter Router  = 		1;                           
	optional string Msg             = 		2;                          // 消息
	optional uint32 Channel         = 		3;                          // 频道
};
// ********************************************************************** //
// CMessageChatResult　   游戏服务器 -> 游戏客户端
// 发送聊天结果给玩家
// ********************************************************************** //
message  CMessageChatResult
{	

	optional uint32 ChatRes    = 		1;                     // 结果类型 定义在chattype中
	optional uint32 Channel    = 		2;                     // 玩家发言频道
	optional string Msg        = 		3;                     // 消息 
	optional string Name       = 		4;                     // 玩家名字
	optional uint32 LeftTime   = 		5;                     // 用于表示喇叭频道的剩余时间，自动喊话的剩余次数
};
// ********************************************************************** //
// CAutoChatMessage　    游戏客户端 -> 游戏服务器 
// 自动喊话消息
// ********************************************************************** //
message CAutoChatMessage
{	
	optional string Msg       = 		1;                       // 消息
	optional uint32 Channel   = 		2;                       // 喊话频道
	optional uint32 Count     = 		3;                       // 喊话次数
	optional string Name      = 		4;                       // 名字
	optional uint32 Time      = 		5;                       // 喊话时间间隔
};
// ********************************************************************** //
// CStopAutoChatMessage　   游戏客户端 -> 游戏游戏服务器
// 停止自动喊话
// ********************************************************************** //
message CStopAutoChatMessage
{	
	optional string Name      = 		1;                      // 名字
	optional uint32 Channel   = 		2;                      // 频道
};
// ********************************************************************** //
// CMessageUseItem　       游戏客户端 -> 游戏服务器 
// 使用指定的喇叭道具发言
// ********************************************************************** //
message CMessageUseItem
{	
	optional string Msg        = 		1;                     // 消息
	optional bytes	ItemInfo   = 		2; 					  // 聊天里面的物品消息
	optional uint32 ItemIndex  = 		3;                     // 消耗道具所在的包裹格子 	
};
// ********************************************************************** //
// CMesssageClickBugleItem　 游戏客户端 -> 游戏服务器
// 角色概况获取完成通知
// ********************************************************************** //
message CMesssageClickBugleItem
{	
	optional uint32 ItemIndex  = 		1;                    // 道具所在的格子	
	optional uint32 LockType   = 		2;                    // 锁定类型（解锁还是加锁 ） 
}; 

// ********************************************************************** //
// CMessageCheckString　 游戏客户端 -> 游戏服务器
// 检查发送的消息中是否含有屏蔽字
// ********************************************************************** //
message CMessageCheckDirtyWord
{
	optional string Msg   = 		1;                        // 需要检查的字符串
	optional uint32 Flag  = 		2;                        // 消息标志
};
// ********************************************************************** //
// CMessageCheckString    游戏服务器  -> 游戏客户端 
// 将屏蔽字的检查结果发送给客户端的通知
// ********************************************************************** //

message CMessageHasDirtyWord
{	
	optional string Msg           = 		1;             // 将被检查的字符串中非法词屏蔽后的字符串
	optional uint32 HasDirtyWord  = 		2;             // 含有屏蔽字的标志（1表示有非法词，0表示没有）
	optional uint32 Flag          = 		3;             // 消息标志
};
