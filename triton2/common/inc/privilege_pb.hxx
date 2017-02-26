// 以最快速度序列/反序列
option optimize_for = SPEED;

enum EM_GMONLINE_MSG_ID
{
	ID_G2C_NOTIFY_PRIVILEGE 	= 0x4000;	// 网关向玩家发送特权列表
	ID_C2G_REQUEST_PRIVILEGE 	= 0x4001;	// 玩家向网关申请行使特权
	ID_G2C_RESPONSE_PRIVILEGE 	= 0x4002;	// 网关向玩家回复特权结果

	ID_G2C_RESULTSET_RESPONSE	= 0x4011;	// 查询的结果集类型
};

// 定义消息类型
message PBByteMsg
{
	required uint32 ID		= 1;	// PB消息ID
	optional bytes	Content		= 2;	// PB消息内容
};

// union 类型
message PBValue
{
	required uint32 	ValueType	= 1;	// 类型ID
	optional uint32 	UInt32Value	= 2;
	optional uint64 	UInt64Value	= 3;
	optional int32  	Int32Value  	= 4;
	optional int64  	Int64Value  	= 5;
	optional float  	FloatValue	= 6;
	optional double 	DoubleValue	= 7;
	optional string 	StringValue 	= 8;
	optional PBByteMsg 	MsgValue	= 9;	 
};

// 属性取值
message PBAttrValue
{
	required uint32 	ID		= 1;	// 属性ID
	required PBValue	Value		= 2;	// 属性取值
};

// 用户权限列表
message CMessageUserPrivilegeNotify	
{
	required uint32 	UserID		= 1;	// 帐号ID
	repeated uint32		Privileges	= 2;	// 权限列表
};

// 执行命令的消息 
message CMessageExecPrivilegeRequest
{
	required uint32 	PrivID		= 1;	// 权限ID 
	repeated PBAttrValue    InputParam	= 2;	// 执行需要的输入参数赋值
};

// 命令返回
message CMessageExecPrivilegeResponse
{
	required int32		Error 		= 1;	// 执行命令的结果
	required uint32 	PrivID		= 2;	// 执行的权限
	optional PBByteMsg 	Proto		= 3;	// 执行的命令返回的结果集
	optional string 	RoleName	= 4;	// 结果集对应的角色名称
};

// 表格类型返回
message CMessageResultSetResponse
{
	optional uint32	RowCount		= 1[default=0]; // 总行数
	optional uint32	ColCount		= 2[default=0]; // 总列数
	repeated bytes	FieldValue		= 3; 		// 列值 ( 变长数组个数为 RowCount * ColCount )
	repeated uint32	FieldValueLen		= 4; 		// 列值长度 ( 变长数组个数为 RowCount * ColCount )
};
