// 以最快速度序列/反序列
option optimize_for = SPEED;

enum RankMessageID // MESSAGE_RANK		= 0x3C00
{
	ID_C2S_REQUEST_PLAYERRANK			= 0x3C01;	// 客户端请求获取玩家等级排行
	ID_S2C_RESPONSE_PLAYERRANK			= 0x3C02;	// 服务器回复玩家等级排行
};


// 玩家个人类 单行排行榜数据
message PBSingleRankInfo
{
	optional string CharName		 = 		1;  // 角色名
	optional uint32	Rank			 = 		2;  // 排名
	optional uint32 ContinuousBoard	 = 		3;  // 连续上榜 
	optional uint32 Metier			 = 		4;  // 职业
	optional uint32 Country			 = 		5;  // 国籍
	optional uint32 ContinuousFirst	 = 		6;  // 蝉联第一天数 (非第一名为0) 
	optional uint32 Title			 = 		7;  // 称号
	optional uint32 CharID			 = 		8;  // 角色ID
	optional uint32 RankValue		 = 		9;  // 本榜单用于排序的数值
	optional int32	RankChange		 = 		10;  // 名次变化 1=升2=降 0=平
	optional int32	RankChangeVal	 = 		11;  // 名次变化数量 如果是0 则是新上榜【RankChange=1or2再来读】
};

/************************************************************************/
/* CMessageGetPlayerRankRequest 客户端->服务器 请求玩家排行 */
/************************************************************************/
message CMessageGetPlayerRankRequest
{
	optional uint32 RankType	 = 		1;  // 排行榜类型
	optional uint32 RecordNum	 = 		2;  // 记录条数 
	optional uint32 CharID		 = 		3;  // 请求者的角色ID (服务器用)
	optional uint32 BeginIndex	 = 		4;  // 榜单起始索引( 最小:0, 最大：榜单最大)
};

/************************************************************************/
/* CMessageGetPlayerLevelRankResponse 服务器->客户端 请求玩家排行的回应 */
/************************************************************************/
message CMessageGetPlayerRankResponse
{
	optional uint32 MyRankChange		 = 		1;  // 我的升降 1=升2=降 0=平
	optional uint32 RankType	         = 		2;  // 排行榜类型
	optional uint32 MyRankValue			 = 		3;  // 我的数值
	repeated PBSingleRankInfo RankInfo	 = 		4;  // 排行榜信息
	optional uint32 CharID				 = 		5;  // 角色ID( 服务器用 )
	optional int32	MyRankChangeVal		 = 		6;  // 名次变化数量 如果是0 则是新上榜【RankChange=1or2再来读】
	optional uint32	MyRankIndex			 = 		7;  // 我的名次 从1开始
};

