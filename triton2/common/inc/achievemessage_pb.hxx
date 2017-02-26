// 以最快速度序列/反序列
option optimize_for = SPEED;

/*
 *	成就系统消息
 */

enum AchieveMessageID // MESSAGE_ACHIEVE = 0x5000
{
	ID_C2S_REQUEST_REPORTACHIEVE		= 0x5001;	// 上报成就
	ID_S2C_RESPONSE_REPORTACHIEVE		= 0x5002;

	ID_C2S_REQUEST_GETSINGLEACHIEVE		= 0x5003;	// 获取单个成就
	ID_S2C_RESPONSE_GETSINGLEACHIEVE	= 0x5004;

	ID_C2S_REQUEST_GETALLACHIEVE		= 0x5005;	// 获取所有成就
	ID_S2C_RESPONSE_GETALLACHIEVE		= 0x5006;

	ID_S2C_NOTIFY_ACHIEVENOTICE			= 0x5007;	// 成就获得通知

	ID_C2S_REQUEST_GETALLHONOR			= 0x5008;
	ID_S2C_RESPONSE_GETALLHONOR			= 0x5009;
	ID_S2C_NOTIFY_HONORFINSH			= 0x500A;

	ID_C2S_REQUEST_GETSTAGEBONUS		= 0x500B;	// 领取阶段奖励
	ID_S2C_RESPONSE_GETSTAGEBONUS		= 0x500C;	
};

// 上报成就请求
message CMessageAchieveReportRequest
{
	optional uint32	AchieveID	= 1;	// 成就ID
	optional uint32	Key			= 2;	// 	
	optional uint32	Value		= 3;	// 进展量
};

// 上报成就回复
message CMessageAchieveReportResponse
{
	optional uint32	RtnCode		= 1;
};

// 成就结构
message PBAchieve
{
	optional uint32	AchieveID	= 1;
	optional uint32	Time		= 2;	// 完成时间
	optional uint32	Status		= 3;	// 是否完成
	optional uint32	RootType	= 4;	// 主类
	repeated PBAchieveParam Param		= 5;	// 参数列表
};

message PBAchieveBuffer
{
	repeated PBAchieve  Achieve = 1;
};

// 获取成就请求
message CMessageAchieveGetSingleRequest
{
	optional uint32	AchieveID	= 1;
};

message CMessageAchieveGetSingleResponse
{
	optional PBAchieve Achieve	= 1;
};

// 获取所有成就
message CMessageAchieveGetAllRequest
{
	optional uint32		IsOpen		= 1;	// 是否打开界面
};

message CMessageAchieveGetAllResponse
{
	optional uint32		TotalPoint	= 1;	// 总成就点数
	optional PBAchieveBuffer AchieveBuffer = 2;
	optional uint32		Stage		= 3;	// 领取到的阶段
	optional uint32		IsOpen		= 4;	// 是否打开界面
};

// 成就获取通知
message CMessageAchieveFinishNotice
{
	optional PBAchieve	Achieve	= 1;
	optional uint32		TotalPoint	= 2;	// 总成就点数
	optional uint32		CharID	= 3;
	optional string		Name	= 4;
};

message PBAchieveParam
{
	optional uint32	Key		= 1;
	optional uint32	Value	= 2;
};

// db pb struct
message PBDBAchieve
{
	optional uint32	AchieveID	= 1;
	optional uint32	Time		= 2;	// 完成时间
	optional uint32	Status		= 3;	// 是否完成
	optional uint32	RootType	= 4;	// 主类
	repeated PBAchieveParam Param = 5;	// 参数列表
};

message PBDBAchieveBuffer
{
	repeated PBDBAchieve Achieve = 1;
	repeated PBHonorData HonorData = 2;
	optional uint32		CharID		= 3;
	optional uint32		TotalPoint	= 4;
	optional uint32		Stage		= 5;
};

// 获得所有功德请求
message CMessageGetAllHonorRequest
{

};

message PBHonorData
{
	optional uint32	MapID	= 1;	// 势力地图ID
	optional uint32	Time	= 2;	// 完成时间
	optional uint32	Param	= 3;	// 预留参数
};

message CMessageGetAllHonorResponse
{
	repeated PBHonorData HonorData = 1;
};

// 完成功德时的通知
message CMessageHonorFinishNotice
{
	optional PBHonorData HonorData = 1;
};

message CMessageGetStageBonusRequest
{
};

message CMessageGetStageBonusResponse
{
	optional uint32	Result	= 1;
	optional uint32	Stage	= 2;	// 领取后到达的阶段
};