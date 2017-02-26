option optimize_for = SPEED;

enum IBSTORE_MSG_ID
{
    ID_C2S_REQUEST_LISTIBITEMS                  = 0x4801;       // 客户端->服务器 请求商城物品列表
    ID_S2C_RESPONSE_LISTIBITEMS                 = 0x4802;       // 服务器->客户端 服务器返回商城物品列表
    ID_C2S_REQUEST_PURCHASEIBITEM               = 0x4803;       // 客户端->服务器 购买商城物品请求
    ID_S2C_RESPONSE_PURCHASEIBITEM              = 0x4804;       // 服务器->客户端 购买商城物品回应
    ID_S2C_NOTIFY_USERCHARGE                    = 0x4805;       // 玩家充值通知
    ID_S2C_NOTIFY_IBSTOREPROMOTION		= 0x4806;	// 促销信息通知

	ID_C2S_GET_FUNCCONSUMES_REQUEST				= 0x4807;		// 客户端->服务器 请求更能付费列表
	ID_S2C_GET_FUNCCONSUMES_RESPONSE			= 0x4808;		// 服务器->客户端 返回付费列表

	ID_C2S_PURCHASE_FUNC_REQUEST				= 0x4809;		// 客户端->服务器 购买功能 请求
	ID_S2C_PURCHASE_FUNC_RESPONSE				= 0x480A;		// 服务器->客户端 购买回应
};

enum IBSTORE_FLAG
{
	EM_GOODS_RECOMMEND      = 1; // 推荐
        EM_GOODS_COUNT_LIMITED  = 2; // 限量                                                                      
        EM_GOODS_TIME_LIMITED   = 3; // 限时                                                                      
        EM_GOODS_DISCOUNT       = 4; // 折扣                                                                      
        EM_GOODS_HOT            = 5; // 热销                                                                      
        EM_GOODS_NEW            = 6; // 新品                                                                      
        EM_GOODS_SELL_OUT       = 7; // 售完                                                                      
	EM_GOODS_INACTIVE       = 8; // 下架                                                                      
	EM_GOODS_TIME_COUNT	= 9; // 限时限量
}; 

// 商城货架位置描述
message PBLocation
{
	optional uint32 page = 1;  // 页面
	optional uint32 row  = 2;  // 行号
	optional uint32 col  = 3;  // 列号
};

// 金银元宝描述
message PBIbMoney
{
	optional uint32 unit    = 1;  // 计价单位 3:金元宝 4: 银元宝
	optional int32  value   = 2;  // 值
};

// 货架上的商品描述
message PBIbItemOnSale
{
	optional uint32 GoodsID       = 1;   // 商品ID
	optional uint32 ItemID        = 2;   // 物品的模板ID
	optional uint32 ItemCount     = 3;   // 单价内的物品数量
	optional int32  GoodsCount    = 4;   // 商城中此商品的数量
	optional PBIbMoney GoodsPrice = 5;   // 商品的原价
	optional PBIbMoney Discount   = 6;   // 折扣后的价格 
	optional int32  ExpiryTime    = 7;   // 此价格的截至时间
	optional int32  ColdTime      = 8;   // 冻结时间 -- 保留
	optional int32  ElapsedTime   = 9;   // 已经消耗的CD时间  -- 保留
	optional uint32 Active        = 10;  // 物品是否生效	
	optional string GoodsName     = 11;  // 默认显示名称用消息中的，如果消息没有此项，则使用ItemID在模板中的名称
	optional string GoodsDesc     = 12;  // 默认描述使用消息中的，如果消息中没有此项，则使用ItemID在模板中的描述
	optional int32 IsPack         = 13;  // 是否是包裹物品	-- 保留
	optional int32 Flag           = 14;  // 商品标签
	optional int32 LimitPerChar   = 15;  // 每玩家限制购买的数量
	optional int32 LimitType      = 16;  // 每玩家限制购买的类型
};

// 玩家打开商城界面请求
message CMessageListIbItemsRequest
{
	optional uint32 Version  =  1;   // 版本号
	optional uint32 IsOpenRMBShopUI = 2; // 是否打开RMB UI 客户端需求
};

// 玩家打开商城界面回应
message CMessageListIbItemsResponse
{
	optional int32  Error           = 1;  // 是否成功 0: 成功 1：同客户端版本一致
	optional uint32 Version         = 2;  // 商城版本号
	repeated PBIbMoney RemainMoneys = 3;  // 玩家帐号上目前的金钱列表
	repeated PBIbItemOnSale Goods   = 4;  // 商城中在售的商品列表
    	repeated PBIbStore IbStore      = 5;  // 货架, 目前只有两个根据金银元宝区分
	optional string Bulletin	= 6;  // 商城公告
	repeated PBIbSubTab FocusTab	= 7;  // 焦点物品列表
	optional uint32 IsOpenRMBShopUI = 8; // 是否打开RMB UI 客户端需求
};

// 玩家购买商品请求
message CMessagePurchaseIbItemRequest
{
	optional uint32 GoodsID      = 1;  //  欲购买的商品ID
	optional uint32 GoodsCount   = 2;  //  欲购买的商品数量
};

// 玩家购买商品回应
message CMessagePurchaseIbItemResponse
{
	optional int32 Error          = 1;  // 交易是否成功标志. 0成功，其它都是失败
	optional uint32 RoleID        = 2;  // 交易的玩家ID
	optional int32  TransID       = 3;  // 此次交易的交易ID
	optional uint32 GoodsID       = 4;  // 购买的商品ID
	optional uint32 GoodsCount    = 5;  // 购买的商品数量
	optional uint32 ItemID        = 6;  // 购买的物品的ID
	optional uint32 ItemCount     = 7;  // 单商品的包括的物品数量
	optional PBIbMoney GoodsCost  = 8;  // 玩家支付了多少
	optional PBIbMoney Remained   = 9;  // 剩余金钱数
};

// 玩家交易通知, 服务器间消息，客户端不使用
message CMessageRoleIbTransNotify
{
	optional int32 TransID    = 1;  // 交易ID
	optional int32 Successful = 2;  // 交易是否成功
};

// 三级页面 
message PBIbSubTab
{
	optional uint32 SubTabID   = 1;  // ID  
	optional string SubTabName = 2;  // 三级分类名称
	repeated uint32 GoodsID    = 3;  // 物品ID
};

// 二级页面
message PBIbTab
{
	optional uint32 TabID      = 1;  // ID    
	optional string TabName    = 2;  // 二级分类名称 
	repeated PBIbSubTab SubTab = 3;  // PBIbSubTab页面
};

// 一级页面
message PBIbStore
{
    optional uint32 PageID   = 1;  // ID
    optional string PageName = 2;  // 一级分类名称
    repeated PBIbTab Tab     = 3;  // PBIbTab页面
};

// 充值后通知玩家消息
message CMessageUserChargeNotice
{
    optional int64 DetailID = 1;    // 充值流水
    optional int32 UserID = 2;      // 帐号ID
    optional int32 SubjectID = 3;   // 货币类型
    optional int32 Amount = 4;      // 充值金额
};

// 物品剩余数量通知
message PBPromotion
{
	optional uint32 GoodsID		= 1; 	// 商品ID
	optional int32 RemainCount	= 2;	// 商品当前剩余数量
	optional int32 BeginTime 	= 3;	// 促销开始时间
	optional int32 EndTime 		= 4;	// 促销截至时间
	optional int32 MaxCount		= 5;	// 最多产生的数量
	optional int32 ColdNum 		= 6;  	// 每次CD之后产生的数量
	optional int32 ColdTime		= 7; 	// CD时间
	optional int32 ElapsedTime	= 8;	// 已经逝去的CD时间
};

message CMessageIbItemRemainNotice
{
	repeated PBPromotion promotion = 1;	// 促销信息
};

message PBConsume
{
	optional uint32	ConsumeType		= 1;	//消费类型 元宝，绑定元宝，金钱，绑定金钱
	optional uint32 ConsumeSprice	= 2;	//消费价格
	optional uint32 TamplateRate	= 3;	//模板的价格率 优先走消费价格
};
//功能付费
message PBFuncStore
{
	optional uint32 FuncID				= 1;	//功能ID
	optional string FuncName			= 2;	//功能名字
	optional uint32 FuncUnitType		= 3;	// EFuncUnitType 单位类型 次 ，小时，天
	repeated PBConsume Consume			= 4;	//消费类型，价格 
	optional uint32	FuncShow			= 5;	// 1,显示在界面，0不显示。	
	optional uint32 DefaultConsume		= 6;	// 默认的消费类型 
	optional uint32 FuncUnit			= 7;	// 单位
	optional uint32 FuncCountLimit		= 8;	// 限制数量/购买上限
};

message CMessageGetFuncConsumesRequest
{
};
message CMessageGetFuncConsumesResponse
{
	optional int32	Error			= 1;	// 是否成功 0: 成功 其他：失败
	repeated PBFuncStore FuncStore	= 2;	// 功能项
};

message CMessagePurchaseFuncRequest
{
	optional uint32 FuncID			= 1;	// 功能ID
	optional uint32 FuncCount		= 2;	// 购买数量
	optional uint32 ConsumeType		= 3;	// 购买类型 ,  BIND_MONEY 1; UNBIND_MONEY  2;  LINEKONG_MONEY   3; SILVER_YUANBAO   4; GOLDEN_YUANBAO   5
	optional uint32 ExtraData1		= 4;	// 额外的参数  对于包裹：ExtraData1 ：index 包裹位置;  
	optional uint32	ExtraData2		= 5;	// ExtraData2 ：包裹ESlotType
};

message CMessagePurchaseFuncResponse
{
	optional int32  Error			= 1;	// 是否成功 0成功，其他失败
	optional uint32 FuncID			= 2;	// 功能ID
	optional uint32 FuncCount		= 3;	// 功能数量
	optional uint32 ConsumeType		= 4;	// 消费的种类 BIND_MONEY 1; UNBIND_MONEY  2; SILVER_YUANBAO   4; GOLDEN_YUANBAO   5
	optional uint32 FuncsCost		= 5;    // 玩家支付了多少
	optional uint32 ExtraData1		= 6;	// 额外参数 对于包裹 ExtraData1: index 包裹位置
	optional uint32 ExtraData2		= 7;	//				包裹 ExtraData2: 包裹ESlotType
};
