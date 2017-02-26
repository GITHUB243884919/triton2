import "coremessage_pb.hxx";
import "property_pb.hxx";

// 以最快速度序列/反序列
option optimize_for = SPEED;

enum StallMessageID{
	ID_C2S_REQUEST_OPENSTALL		= 0x2800;		// 打开摊位
	ID_S2C_RESPONSE_OPENSTALL		= 0x2801;	
	ID_C2S_REQUEST_READPROCESS		= 0x2802;		// 进度读取
	ID_S2C_NOTIFY_STARTSTALL		= 0x2803;		// 开始摆摊：code : 可以摆摊； 摆摊失败
	ID_C2S_REQUEST_UPDATEGOODS		= 0x2804;		// 更新摊位物品
	ID_S2C_RESPONSE_UPDATEGOODS		= 0x2805;		
	ID_C2S_REQUEST_UPDATEPRICE		= 0x2806;		// 更新摊位价格
	ID_S2C_RESPONSE_UPDATEPRICE		= 0x2807;
	ID_C2S_REQUEST_CLICKSTALL		= 0x2809;		// 玩家点击摊位
	ID_S2C_RESPONSE_CLICKSTALL		= 0x280A;	
	ID_C2S_REQUEST_BUYGOODS			= 0x280B;		// 玩家购买
	ID_S2C_RESPONSE_BUYGOODS		= 0x280C;
	ID_C2S_REQUEST_CLOSESTALL		= 0x280D;		// 收摊
	ID_S2C_RESPONSE_CLOSESTALL		= 0x280E;
	ID_C2S_REQUEST_CLOSEDIALOG		= 0x280F;		// 关闭摊位界面(观察者)
	ID_C2S_REQUEST_UPDATETEXT		= 0x2810;		// 修改文字：广告、摊名
	ID_S2C_RESPONSE_UPDATETEXT		= 0x2811;

	ID_C2S_REQUEST_STALLRECORD		= 0x2812;
	ID_S2C_RESPONSE_STALLRECORD		= 0x2813;

	ID_C2S_REQUEST_CLEANRECORD		= 0x2814;
	ID_S2C_RESPONSE_CLEANRECORD		= 0x2815;
};


//***********************************************
// CMessageOpenStallRequest
//***********************************************

message CMessageOpenStallRequest
{
	optional string StallName  = 		1; 	// 摊名
	optional string AdContent  = 		2; 	// 广告内容
	optional int32 StallType   =        3;  //增加摆摊类型 0:不区分；1：金；2：元宝
};

//***********************************************
// CMessageOpenStallResponse
//***********************************************

message CMessageOpenStallResponse
{
	optional uint32		EntityID	= 		1; 	// 主角实体ID
	optional int32		ErrorCode	=		2;	// 错误码; 0-成功; !0-其他错误
};

//***********************************************
// CMessageProcessReadResult
// 客户端通知服务器进度条读取成功与否
//***********************************************

message CMessageProcessReadResult
{
	optional uint32		RtnCode	  = 		1; 	// 读取结果：true--成功；false--失败
};

//***********************************************
// CMessageStartStallResponse
// send to client
//***********************************************

message CMessageStartStallNotify
{
	optional uint32		EntityID  = 		1; 	 // 摊主entityid
	optional string		AdContent  = 		2; 	 // 广告内容
	optional string		StallName  = 		3; 	 // 摊名
	repeated uint32		StallIndex  = 		4; 
	repeated uint32		Price  = 		5; 
	repeated PBItemObject ItemList  = 		6; 
	repeated uint32		PriceType  =		7; // 物品价格类型，1：绑金；2：非绑金；4：绑定元宝；5：元宝。实际只有2,5
	optional int32		StallType  =		8; //摆摊类型
};

//***********************************************
// CMessageUpdateTextRequest
// 修改摊位信息：广告内容和摊位名称
//***********************************************

message CMessageUpdateTextRequest
{
	optional string		AdContent	 = 		1; 	// 广告内容
	optional uint32		Type  = 		2; 			// 摊位名称：STALL_NAME,广告内容：STALL_ADCONTENT
	optional string		StallName	 = 		3; 	// 摊位名称
};

//***********************************************
// CMessageUpdateTextResponse
//***********************************************

message CMessageUpdateTextResponse
{
	optional uint32		EntityID  = 		1; 	// 摊主entityid
	optional uint32		Type	 = 		2; 	// 修改类型：STALL_NAME--摊位名称；STALL_ADCONTENT--广告内容
	optional string		AdContent  = 		3; 	// 广告内容
	optional string		StallName  = 		4; 	// 摊位名称
};

//***********************************************
// CMessageUpdateStallRequest
// 更新摆摊物品和金钱
//***********************************************

message CMessageUpdateStallRequest
{
	optional uint32		ItemIndex	 = 		1; 		// 包裹中的index
	optional uint32		Type		 = 		2; 		// 更新类型：STALL_PUSH--上架；STALL_DOWN--下架
	optional uint32		StallIndex   = 		3; 		// 摊位索引
	optional uint32		ItemNum		 = 		4; 		// 物品数量
	optional uint32		Price		 = 		5; 		// 物品上架价格
	optional uint32		PriceType	 =		6;		// 物品价格类型，1：绑金；2：非绑金；4：绑定元宝；5：元宝。实际只有2,5
};

//***********************************************
// CMessageUpdateStallResponse
//***********************************************

message CMessageUpdateStallResponse
{
	optional uint32		EntityID	 = 		1; 	 // 摊主entityid
	optional uint32		StallIndex	 = 		2; 	 // 摊位索引
	optional PBItemObject Item		 = 		3; 	 // 道具
	optional uint32		Price		 = 		4; 
	optional uint32		Type		 = 		5; 	 // 上架，下架(STALL_PUSH--上架；STALL_DOWN--下架)
	optional uint32		PriceType	 =		6;		// 物品价格类型，1：绑金；2：非绑金；4：绑定元宝；5：元宝。实际只有2,5
};

//***********************************************
// CMessageUpdatePriceRequest
//***********************************************

message CMessageUpdatePriceRequest
{
	optional uint32		Price	 = 		1; 	// 更新价格
	optional uint32		StallIndex  = 		2; 	// 摊位索引
	optional uint32		PriceType  =   3;	
};

//***********************************************
// CMessageUpdatePriceResponse
//***********************************************

message CMessageUpdatePriceResponse
{
	optional uint32		Price	 = 		1; 	// 价格
	optional uint32		StallIndex  = 		2; 	// 摊位索引
	optional uint32		EntityID  = 		3; 	// 摊主entityid
	optional uint32		PriceType  =   4;	
};

//***********************************************
// CMessageCloseStallRequest
//***********************************************

message CMessageCloseStallRequest
{
	optional uint32	IsSaved  = 		1; 		// 是否保存记录(true:save log, false:cancel log)
};

//***********************************************
// CMessageCloseStallResponse
//***********************************************

message CMessageCloseStallResponse
{
	optional uint32		EntityID  = 		1; 	// 摊主entityid
};

//***********************************************
// CMessageClickStallRequest
//***********************************************

message CMessageClickStallRequest
{
	optional uint32		EntityID	 = 		1; 	// 摊主实体ID
};

//***********************************************
// CMessageClickStallResponse
//***********************************************

message CMessageClickStallResponse
{
	repeated PBItemObject ItemList	 = 		1; 	// 摊位商品	
	repeated uint32		StallIndex  = 		2; 		// 摊位索引
	optional uint32		EntityID  = 		3; 		// 摊主entityid
	repeated uint32		Price   = 		4; 
	repeated uint32		PriceType =		5;
};

//***********************************************
// CMessageBuyGoodsRequest
//***********************************************

message CMessageBuyGoodsRequest
{
	optional uint32		Price  = 		1; 		// 道具价格
	optional uint32		EntityID  = 		2; 	// 摊主entityid
	optional uint32		Number  = 		3; 		// 购买数量
	optional uint32		StallIndex  = 		4; 	 // 摊位索引
	optional uint32		ItemID  = 		5; 		// 道具id
	optional uint32		PriceType =		6;
};

//***********************************************
// CMessageBuyGoodsResponse
// 通知客户端物品已经出售，删除该道具from摊位
// 多个玩家可能在打开摊位，刷新一下摊位
//***********************************************

message CMessageBuyGoodsResponse
{
	optional uint32		EntityID  = 		1; 	 // 摊主entityid
	optional uint32		StallIndex  = 		2; 	 // 摊位索引
	optional uint32		Number	 = 		3; 	 // 购买数量
	optional uint32		BuyerEntityID = 4;	 // 购买者的实体ID
};

//***********************************************
// CMessageCloseStallDlgRequest
// 观察者关闭摊位界面
//***********************************************

message CMessageCloseStallDlgRequest
{
	optional uint32		EntityID  = 		1; 	// 摊主entityid
};

message PBStallRecord
{
	optional string MasterName = 1;
	optional string BuyerName  = 2;
	optional uint32	ItemID	   = 3;
	optional uint32	Number	   = 4;
	optional uint32	Price	   = 5;
	optional uint32	Time       = 6;
	optional uint32	Action     = 7;  // 动作: 0-售出  1-因为摊主金钱超过而下架
	optional uint32	Tax		   = 8;	 // 税
	optional uint32 PriceType  = 9;  // 费用类型
	optional uint32	Fee		   = 10;	 // 手续费，避免以后出现同时收费和税的情况，该字段单列
};

// 查询摊位交易记录请求
message CMessageStallGetRecordRequest
{
	optional uint32	EntityID = 1;
};

// 查询摊位交易记录回复
message CMessageStallGetRecordResponse
{
	optional uint32	EntityID = 1;
	repeated PBStallRecord Record = 2;
};

// 清空记录
message CMessageStallCleanRecordRequest
{

};
 
// 清空记录
message CMessageStallCleanRecordResponse
{
	optional uint32	RtnCode = 1;
};