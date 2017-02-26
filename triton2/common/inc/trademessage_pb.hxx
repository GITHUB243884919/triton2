import "coremessage_pb.hxx";
import "property_pb.hxx";

// 以最快速度序列/反序列
option optimize_for = SPEED;

enum TradeMessageID
{
	ID_C2S_REQUEST_TRADEINVITE				= 0x2400;			// 交易邀请
	ID_S2C_NOTIFY_INVITERTN					= 0x2401;			// 
	ID_C2S_REQUEST_INVITEREPLY				= 0x2402;			// 处理交邀请
	ID_S2C_NOTIFY_TRADEBEGIN				= 0x2403;			// 通知开始交易
	ID_C2S_REQUEST_SENDGOODSINFO			= 0x2404;			// 发送物品信息
	ID_S2C_NOTIFY_GOODSINFO					= 0x2405;			// 通知物品信息
	ID_C2S_REQUEST_SENDTRADEMONEY			= 0x2406;			// 请求发送交易金额
	ID_S2C_NOTIFY_TRADEMONEY				= 0x2407;			// 通知交易金额
	ID_C2S_REQUEST_LOCKTRADEINFO			= 0x2408;			// 锁定交易信息
	ID_S2C_NOTIFY_TRADEINFOLOCK				= 0x2409;			// 
	ID_C2S_REQUEST_CLOSETRADE				= 0x2410;			// 请求结束交易
	ID_S2C_NOTIFY_TRADECLOSE				= 0x2411;			// 发送交易结束命令给目标
	ID_C2S_REQUEST_SUBMITTRADE				= 0x2412;			// 确定交易
	ID_S2C_NOTIFY_RESULTCODE				= 0x2413;			// 交易过程信息
};

/************************************************************************/
/* 
	CMessageTradeInvite		client send trade invite to server
*/
/************************************************************************/

message CMessageTradeInvite
{
	optional uint32 TargetEntityID  = 		1; 	// 交易受邀人	
};

/************************************************************************/
/* 
	CMessageTradeInviteCallBack		
	server notify trade invite to target client
*/
/************************************************************************/

message CMessageTradeInviteCallBack
{
	optional uint32 OriginEntityID  = 		1;  // 交易发邀人
};

/************************************************************************/
/* 
	CMessageTradeInviteReply
	client send invite process result to server
*/
/************************************************************************/

message CMessageTradeInviteReply
{
	optional uint32 OriginEntityID  = 		1; 
	optional uint32	Code  = 		2; 		// 交易邀请处理结果: 同意,拒绝，忽略
};

/************************************************************************/
/*     
	CMessageNotifyTradeBegin
	server noitfy clients to begin trade
*/
/************************************************************************/

message CMessageNotifyTradeBegin
{
	optional uint32 TargetEntityID  = 		1; 		// 通知对象
};

/************************************************************************/
/* 
	CMessageSendGoodsInfo
	client send trade goods to server
*/
/************************************************************************/

message CMessageSendGoodsInfo
{
	optional uint32 TradeType  = 		1; 		// 交易类型
	optional uint32 TradeIndex  = 		2; 		// 交易栏索引
	optional uint32 ItemIndex  = 		3; 		// 包裹索引
	optional uint32 Count  = 		4; 			// 物品数量
};

/************************************************************************/
/* 
	CMessageNotifyGoodsInfo
	server notify goods info to clients
*/
/************************************************************************/

message CMessageNotifyGoodsInfo
{
	optional bytes	GoodsBuff  = 		1; 		// 物品字符流	
	optional PBItemObject Item  = 		2; 		// 道具信息
	optional uint32 BuffLen  = 		3; 		// 
	optional uint32 Type  = 		4; 			// 操作类型：AddGoods, DelGoods, Money
	optional uint32 TradeIndex  = 		5; 		// 交易栏索引
	optional uint32 IsSelf  = 		6; 			// 自己?
	optional uint32 ItemIndex  = 		7; 		// 包裹索引
};

/************************************************************************/
/* 
	CMessageSendTradeMoney
	client send trade money to server
*/
/************************************************************************/

message CMessageSendTradeMoney
{
	optional uint32 Money  = 		1; 		// 交易金额
};

/************************************************************************/
/* 
	CMessageNotifyTradeMoney
	server notify trade money to client
*/
/************************************************************************/

message CMessageNotifyTradeMoney
{
	optional uint32 IsSelf  = 		1; 		// 自己?
	optional uint32 Money  = 		2; 		// 交易金额
};

/************************************************************************/
/* 
	CMessageLockTradeInfo
	client request server to lock trade info,include goods and money info
*/
/************************************************************************/

message CMessageLockTradeInfo
{
	optional uint32 LockState  = 		1; 		// 交易锁状态
};

/************************************************************************/
/* 
	CMessageNotifyTradeInfoLock
	server notify client to lock trade info,so as to keep trade safety
*/
/************************************************************************/

message CMessageNotifyTradeInfoLock
{
	optional uint32 IsSelf  = 		1; 			// 自己?
	optional uint32 LockState  = 		2; 		// 交易锁状态
};

/************************************************************************/
/* 
	CMessageCloseTrade
	client request server to close trade window, when cancel trade ,
	special close and end trade normally
*/
/************************************************************************/

message CMessageCloseTrade
{
	optional uint32 Code  = 		1; 			// 结束码: 取消、正常结束
};

/************************************************************************/
/* 
	CMessageNotifyTradeClose
	server notify client to close trade window, as trade has been ended
*/
/************************************************************************/

message CMessageNotifyTradeClose
{
	optional uint32 Code  = 		1; 			// 结束码：取消、正常结束
};

/************************************************************************/
/* 
	CMessageNotifyResultCode
	server notify client to show process tips
*/
/************************************************************************/

message CMessageNotifyResultCode
{
	optional uint32 Code  = 		1; 			// 提示信息码
	optional uint32 EntityID  = 		2; 		// 主角
};

/************************************************************************/
/* 
	CMessageSubmitTrade
	client request server to submit trade ,so as to a normal trade end
*/
/************************************************************************/

message CMessageSubmitTrade
{
	optional uint32 Code  = 		1; 			// 交易提交码
};
