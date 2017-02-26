// 以最快速度序列/反序列
option optimize_for = SPEED;


enum JudgeMessageID // MESSAGE_JUDGE = 0x4C00
{
	ID_S2C_NOTIFY_OPENJUDGE				= 0x4C01;	// 通知客户端评价系统开启
	ID_S2C_NOTIFY_CLOSEJUDGE			= 0x4C02;	// 通知客户端评价系统关闭
	
	ID_C2S_REQUEST_MODIFYJUDGESINGLE	= 0x4C03;	// 添加评价请求
	ID_S2C_RESPONSE_MODIFYJUDGESINGLE	= 0x4C04;	// 添加评价回复
	ID_C2S_REQUEST_MODIFYJUDGELIST		= 0x4C05;	// 批量修改好感、恶感请求
	ID_S2C_RESPONSE_MODIFYJUDGELIST		= 0x4C06;	// 批量修改好感、恶感回复
	ID_S2C_NOTIFY_MODIFYJUDGE			= 0x4C07;	// 修改好感、恶感通知

	ID_C2S_REQUEST_QUERYJUDGELIST		= 0x4C08;	// 查询好感、恶感列表请求
	ID_S2C_RESPONSE_QUERYJUDGELIST		= 0x4C09;	// 查询好感、恶感列表回复

	ID_S2C_NOTIFY_JUDGEMAX				= 0x4C0A;	// 好感、恶感上限更新

	ID_C2S_REQUEST_QUERYJUDGE			= 0x4C0B;	// 查询单条记录
	ID_S2C_RESPONSE_QUERYJUDGE			= 0x4C0C;
};

// 评价系统开启通知
message CMessageOpenJudgeNotice
{
	optional uint32 Type	= 1;	// 好感、恶感
	optional string Name	= 2;	// 主角 name
	optional uint32	CharID	= 3;	// 主角 charid; 
};

// 评价系统关闭通知
message CMessageCloseJudgeNotice
{
	optional uint32 Type	= 1;	// 好感、恶感
	optional string Name	= 2;	// 主角 name
	optional uint32	CharID	= 3;	// 主角 charid; 
};

// 添加评价请求
message CMessageModifySingleJudgeRequest
{
	optional uint32	Type	= 1;	// 好感、恶感
	optional uint32	SrcCharID = 2;	// 发起人 charid
	optional string DesName	= 3;	// 对方 name
	optional uint32	Value	= 4;	// value
};

// 添加评价回复
message CMessageModifySingleJudgeResponse
{
	optional uint32	RtnCode = 1;
	optional uint32	Type	= 2;	// 好感、恶感
	optional string DesName	= 3;	// 对方 name
	optional uint32	DesCharID = 4;	// 对方 charid
	optional uint32	Value	= 5;	// value
	optional uint32	Face	= 6;
	optional uint32	Sex		= 7;
};

// 修改好感、恶感请求
message CMessageModifyJudgeListRequest
{
	optional uint32	Type	= 1;	// 好感、恶感
	optional uint32	SrcCharID = 2;	// 发起人 charid
	repeated PBJudgeRecord Data = 3;
};

// 修改好感、恶感回复
message CMessageModifyJudgeListResponse
{
	optional uint32 RtnCode = 1;
	optional uint32	Type	= 2;	// 好感、恶感
};

// 修改好感、恶感通知
message CMessageModifyJudgeNotice
{
	optional uint32	Type	= 1;	// 好感、恶感
	optional string Name	= 2;	// 评价人 name
	optional uint32	CharID	= 3;	// 评价人 charid
	optional uint32	Value	= 4;	// value
	optional uint32	Face	= 5;
	optional uint32	Sex		= 6;
	optional uint32	IsAdd	= 7;	// 是添加
};

message PBJudgeRecord
{
	optional string SrcName		= 1;
	optional uint32	SrcCharID	= 2;
	optional string DesName		= 3;
	optional uint32	DesCharID	= 4;
	optional uint32	Type		= 5;
	optional uint32	Value		= 6;
	optional uint32	TimeStamp	= 7;
	optional uint32	Face		= 8;	// 
	optional uint32	Sex			= 9;	//
};

// 查询好感、恶感评价列表请求
message CMessageQueryJudgeListRequest
{
	// 一次请求，获取自己评价他人和收到的评价好感、恶感列表;
	optional uint32	CharID	= 1;	// 请求人
	optional uint32	Condition = 2;	// 按时间、按值排序
	optional uint32	Action = 3;		// 行为
};

// 查询好感、恶感评价列表回复
message CMessageQueryJudgeListResponse
{
	repeated PBJudgeRecord FavorJudgeOtherList	= 1;	// 对他人好感评价列表
	repeated PBJudgeRecord BadJudgeOtherList	= 2;	// 对他人恶感评价列表
	repeated PBJudgeRecord FavorRecJudgeList	= 3;	// 收到好感评价列表
	repeated PBJudgeRecord BadRecJudgeList		= 4;	// 收到恶感评价列表
	
	optional uint32	FavorTotal = 5;		// 好感总值
	optional uint32 BadTotal   = 6;		// 恶感总值
};

// 好感、恶感总值更新通知
message CMessageUpdateMaxJudgeValueNotice
{
	optional uint32	FavorTotal = 1;		// 好感总值
	optional uint32 BadTotal   = 2;		// 恶感总值
}

message CMessageQuerySingleRecordRequest
{
	optional uint32	SrcCharID	= 1;
	optional uint32	DesCharID	= 2;
};

message CMessageQuerySingleRecordResponse
{
	optional PBJudgeRecord FavorRecord = 1;
	optional PBJudgeRecord BadRecord = 2;
};