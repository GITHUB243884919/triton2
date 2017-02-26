import "privilege_pb.hxx";


// 属性定义
message PBAttrDef
{
	required uint32 	ID		= 1;	// 属性ID
	optional string 	Code		= 2;	// 属性代码
	required uint32		ValueType	= 3;	// 属性的类型
};

enum EMPrivExecType
{
	EM_PRIV_CALL_DBPROC 		= 1;	// 调用DBServer的存储过程
	EM_PRIV_CALL_LOGPROC 		= 2;	// 调用LogServer的存储过程
	EM_PRIV_REMOTE_HOST_CALL 	= 3;	// 发送到远程主机
	EM_PRIV_PLALYER_CALL 		= 4;	// 发送到指定玩家所在主机
};

enum EMFindPlayerMethod
{
	EM_FIND_BY_ACCOUNT_ID		= 1;	// 根据帐号ID查找
	EM_FIND_BY_ACCOUNT_NAME		= 2;	// 根据帐号名称查找
	EM_FIND_BY_ROLE_ID		= 3;	// 根据角色ID查找
	EM_FIND_BY_ROLE_NAME		= 4;	// 根据角色名称查找
};

message PBPrivExec
{
	required EMPrivExecType ExecType	= 1;	// 执行类型
	optional string Procedure		= 2; 	// 存储过程名称
	optional uint32	HostType		= 3;	// 主机类型
	optional uint32 HostID			= 4;	// 主机ID
	optional EMFindPlayerMethod FindMethod	= 5; 	// 按照什么方法查找玩家
	optional uint32 ParamID			= 6;	// 这个字段中什么地方
};

// 权限及输入输出参数定义 
message PBPriv
{
	required uint32 	ID		= 1;	// 权限ID 
	optional string 	Code		= 2;	// 权限代码
	repeated PBAttrDef	Input		= 3;	// 执行需要的输入参数
	required uint32		Output		= 4;	// 权限被执行返回的消息ID, 不期待返回消息体则为0
	required PBValue    	Property	= 5;	// 权限本身属性
	optional PBPrivExec	Exec		= 6;	// 执行方法
};

// 所有权限数据, 服务器启动时候装载
message CMessageAllPrivilegeNotify 
{
	repeated PBPriv 	Privileges	= 1;	// 全部权限模板内容, PBAttr是权限模板本身的属性
};

// 服务器启动通知
message CMessageServerStartUpNotify
{

};
