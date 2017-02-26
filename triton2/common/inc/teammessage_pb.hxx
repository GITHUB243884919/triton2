import "coremessage_pb.hxx";
import "property_pb.hxx";

// 以最快速度序列/反序列
option optimize_for = SPEED;

enum TeamMessageID
{								
	ID_C2S_REQUEST_CREATETEAM						= 0x1C01;				// 自创建队伍
	ID_C2S_REQUEST_INVITE							= 0x1C02;				// 组队邀请
	ID_C2S_REQUEST_INVITERTN						= 0x1C03;				// 邀请请求返回
	ID_C2S_REQUEST_INVITEDREQUEST					= 0x1E04;				// 0x1C04 + 0x0200队员邀请的入队申请
	ID_C2S_REQUEST_SEARCH							= 0x1C05;				// 组队信息检索
	ID_C2S_REQUEST_ASKFORJOIN						= 0x1C06;				// 申请加入
	ID_C2S_REQUEST_APPLYRTN							= 0x1C07;				// 申请信息返回		client to server
	ID_C2S_REQUEST_LEAVE							= 0x1C08;				// 离队
	ID_C2S_REQUEST_NEWHEAD							= 0x1C09;				// 转移队长
	ID_C2S_REQUEST_TICK								= 0x1C0A;				// 踢人
	ID_C2S_REQUEST_ENDTEAM							= 0x1C0B;				// 解散队伍
	ID_S2C_REQUEST_PROPOSERRTN						= 0x1E0C;				// 0x1C0C + 0x0200申请人信息返回	server to client
	ID_S2C_NOTIFY_SYNITEMLIST						= 0x1E0D;				// 0x1C0D + 0x0200同步成员列表		server to client
	ID_S2C_NOTIFY_ITEMLIST							= 0x1C0E;				// 发送成员列表
	ID_S2C_NOTIFY_ENDTEAM							= 0x1C0F;				// 队伍解散				server to client
	ID_S2C_NOTIFY_ASKJOINRTN						= 0x1E10;				// 0x1C10 + 0x0200发送请求加入的返回信息	server to client
	ID_S2C_NOTIFY_TICKRTN							= 0x1C11;				// 发送踢人的返回信息	server to client
	ID_S2C_NOTIFY_ERRORMSG							= 0x1E12;				// 0x1C12 + 0x0200发送错误信息		server to client
	ID_C2S_NOTIFY_ASKTEAMMSG						= 0x1C13;				// 提交求组信息		client to server
	ID_S2C_NOTIFY_ASKTEAMTRN						= 0x1C14;				// 求租返回
	ID_S2C_NOTIFY_CONSCRIBERTN						= 0x1C15;				// 招募返回
	ID_C2S_NOTIFY_CONSCRIBEMSG						= 0x1C16;				// 提交招募信息
	ID_S2C_NOTIFY_SYNPROPERTY						= 0x1E17;				// 0x1C17 + 0x0200同步队伍成员信息	server to client
	ID_C2S_REQUEST_CHANGEDISTRIBUTE					= 0x1C18;				// 发送改变分配模式请求	client to server
	ID_S2C_NOTIFY_CHANGEDISTRIBUTERTN				= 0x1C19;				// 发送改变分配模式的返回信息 server to client
	ID_S2C_NOTIFY_SYNBUFF							= 0x1C1A;				// 同步buff信息			server to client
	ID_C2S_REQUEST_BUFFRTN							= 0x1C1B;				// 请求buff信息
	ID_C2S_REQUEST_ALIVESTATE						= 0x1E1C;				// 0x1C1C + 0x0200请求在线状态信息
	ID_C2S_REQUEST_CLEARREQUESTLIST					= 0x1C1D;				// 删除请求列表
	ID_C2S_REQUEST_CLEARINVITELIST					= 0x1C1E;				// 删除邀请列表
	ID_S2C_NOTIFY_CLEARREQUESTCALLBACK				= 0x1C1F;				// 删除请求列表结果返回
	ID_S2C_NOTIFY_CLEARINVITECALLBACK				= 0x1C20;				// 删除邀请列表结果返回
	ID_C2S_REQUEST_SEARCHMSG						= 0x1C21;				// 搜索求组消息
	ID_C2S_REQUEST_SRHCONSCRIBEMSG					= 0x1C22;				// 搜索招募消息
	ID_S2C_NOTIFY_SEARCHMSGCALLBACK					= 0x1E23;				// 0x1C23 + 0x0200搜做求组消息结果返回
	ID_S2C_NOTIFY_SRHCONSCRIBECALLBACK				= 0x1E24;				// 0x1C24 + 0x0200搜索招募消息结果返回
	ID_C2S_REQUEST_DELMSG							= 0x1C25;				// 删除求组招募消息
	ID_S2C_NOTIFY_DELMSGCALLBACK					= 0x1C26;				// 删除求组招募消息结果返回
	ID_C2S_REQUEST_ORDERFOLLOW						= 0x1C27;				// 组队跟随命令
	ID_C2S_REQUEST_REQUESTFOLLOW					= 0x1C28;				// 请求跟随队长
	ID_S2C_NOTIFY_ORDERFOLLOWRTN					= 0x1C29;				//	命令组队跟随结果返回
	ID_S2C_NOTIFY_REQUESTFOLLOWRTN					= 0x1C2A;				//	请求组队跟随结果返回
	ID_S2C_NOTIFY_2PARAMNOTIFY						= 0x1C2B;				// 两个参数的操作提示信息
	ID_S2C_NOTIFY_CHANGETEAMSTATE					= 0x1C2C;				// 发送队伍标志改变
	ID_S2S_RESPONSE_REDIRECTJOINTEAM				= 0x1E2D;				// 0x1C2D + 0x0200 跨场景入队回复
	ID_S2S_NOTIFY_REDIRECTRELATION					= 0x1E2E;				// 0x1C2E + 0x0200 跨场景产生队伍关系
	ID_S2S_NOTIFY_REDIRECTCREATETEAM				= 0x1E2F;				// 0x1C2F + 0x0200 跨场景通知创建队伍
	ID_S2C_NOTIFY_TEAMNOTICE						= 0x1C30;				// 组队通知
};



/************************************************************************/
/* CMessageCreateTeamSelf 客户端->服务器 自创建*/
/************************************************************************/

message CMessageCreateTeamSelf
{
						                                		
};

// *************************************************************************** //
// CMessageTeamInvite 游戏客户端-> 游戏服务器
// 邀请玩家加入队伍
// *************************************************************************** //

message CMessageTeamInvite
{
	optional uint32 EntityID		 = 		1; 	// 对方EntityID
	optional uint32 CharID			 = 		2; 	//
};

// *************************************************************************** //
// CMessageTeamInviteRtn 游戏客户端-> 游戏服务器
// 被邀请玩家对队伍邀请回应
// *************************************************************************** //
message CMessageTeamInviteRtn
{
	optional uint32		RtnCode  = 		1; 		// 邀请操作返回码: 同意、拒绝
	optional uint32		InviteID  = 		2; 		// 邀请者charid
	optional uint32		InviteEntityID  = 		3; 	// 邀请者实体id，服务器多数查询基于实体id
	optional uint32		HeadCharID  = 		4; 		// 队长角色ID
	optional uint32		TeamID  = 		5; 			// 队伍标志
};

/************************************************************************/
/* CMessageTeamInvitedRequest 队员邀请的申请入队                                                                     */
/************************************************************************/
message CMessageTeamInvitedRequestCallBack
{
	optional CMessageRouter Router  = 		1; 
	optional uint32		AccoutID  = 		2; 			// 被邀请人的accountid
	optional string		InviteeName  = 		3; 		// 被邀请人的name
	optional uint32		InviteID  = 		4; 			// 邀请者charid
	optional uint32		EntityID  = 		5; 			// 被邀请人的entityid
	optional uint32		Level  = 		6; 
	optional string		InviteName  = 		7; 			// 邀请者name
	optional uint32		Nationality	 = 		8; 		// 被邀请人的国籍 
	optional uint32		InviteeID  = 		9; 			// 被邀请人的charid
	optional uint32		Sex  = 		10; 
	optional uint32		Metier  = 		11; 
	optional uint32		Face  = 		12; 				// 被邀请人的脸型
};


// *************************************************************************** //
// CMessageTeamSearch 游戏客户端-> 游戏服务器
// 搜索邀请玩家
// *************************************************************************** //
message CMessageTeamSearch
{

};

// *************************************************************************** //
// CMessageTeamInviteRequest 游戏客户端-> 游戏服务器
// 入队申请
// *************************************************************************** //
message CMessageTeamRequest
{
	optional uint32		EntityID  = 		1; 		// 对方entityid (非常关心)	
	optional uint32		CharID	  = 		2; 		// 对方charid
};

// *************************************************************************** //
// CMessageTeamInviteRequestRtn 游戏客户端-> 游戏服务器
// 请求加入玩家加入
// *************************************************************************** //
message CMessageTeamRequestRtn
{
	optional uint32		CharID  = 		1; 			// 申请人角色id
	optional uint32		EntityID  = 		2; 		// 申请人entityid(非常关心)
	optional uint32		Code  = 		3; 			// 同意，拒绝
	optional uint32		AccountID  = 		4; 		// 申请人账号id
};

// *************************************************************************** //
// CMessageTeamLeave 游戏客户端-> 游戏服务器
// 离开队伍
// *************************************************************************** //
message CMessageTeamLeave
{

};

// *************************************************************************** //
// CMessageTeamNewHead 游戏客户端-> 游戏服务器
// 转移队长
// *************************************************************************** //
message CMessageTeamNewHead
{
	optional uint32 CharID  = 		1; 				// 新队长charid
	optional uint32 EntityID  = 		2; 			// 新队长实体id(务必提供)
};

// *************************************************************************** //
// CMessageTeamNewHead 游戏客户端-> 游戏服务器
// 踢人
// *************************************************************************** //
message CMessageTeamTick
{
	optional uint32 EntityID	 = 		1; 		// 对方EntityID，(非常关心)
	optional uint32 CharID		 = 		2; 		// 被踢玩家的charid
};

/************************************************************************/
/*	CMessageTeamEnd 解散队伍 
*/
/************************************************************************/
message CMessageTeamEnd 
{

};

// *************************************************************************** //
// CMessageTeamRequestPlayerCallBack 游戏服务器-> 游戏客户端
// 请求加入玩家信息发送给队长
// *************************************************************************** //
message CMessageTeamRequestPlayerCallBack
{
	// 您只需关心Member项中的数据即可
	//optional PBMember	Member = 9;				// 申请人信息
	optional CMessageRouter Router  = 		1; 
	optional uint32		EntityID  = 		2; 			// 申请人的实体id
	optional uint32		Face  = 		3; 
	optional uint32		Nationality  = 		4; 		// 申请人国籍
	optional string		Name  = 		5; 				// 申请人name
	optional uint32		AccoutID  = 		6; 
	optional uint32		Level  = 		7; 
	optional uint32		Sex  = 		8; 
	optional uint32		Metier  = 		9; 
	optional uint32		CharID  = 		10; 
};

// *************************************************************************** //
// CMessageTeamNotifyCallBack 游戏服务器-> 游戏客户端
// 与客户端同步队伍成员列表
// 有这几种情形:新增成员、成员离去，队长变化，成员属性更新
// 你只需关心 Code 与 Member 两个子项
// *************************************************************************** //
message CMessageTeamNotifyCallBack
{
	
	//optional string		Name = 2;
	//optional uint32		CharID = 3;
	//optional uint32		Face = 5;
	//optional uint32		Metier = 6;
	//optional uint32		Level = 7;
	//optional uint32		Sex = 8;
	//optional uint32		EntityID = 9;	// 主角的实体id
	////optional PBMember	Member = 8;		// 主角的属性
	//optional uint32		LineID = 10;
	//optional uint32		MapID = 11;
	optional CMessageRouter Router  = 		1; 
	optional uint32		Code  = 		2; 
	optional PBMember	Member  = 		3; 
};

// *************************************************************************** //
// CMessageTeamCallBack 游戏服务器-> 游戏客户端
// 发送成员列表
// *************************************************************************** //
message CMessageTeamCallBack
{ 
	optional string		HeaderName  = 		1; 	// 队长
	optional uint32		Count  = 		2; 		// 队伍人数
	optional string		Buff  = 		3; 
	optional uint32		HeadEtyID  = 		4; 	// 队长实体id
	repeated PBMember	Member  = 		5; 		// 队员数据
	optional uint32		BuffSize  = 		6; 
	optional uint32		Distribute  = 		7; 	// 分配模式
};

// *************************************************************************** //
// CMessageTeamEndCallBack 游戏服务器-> 游戏客户端
// 队伍解散
// *************************************************************************** //
message CMessageTeamEndCallBack
{

};

// *************************************************************************** //
// CMessageTeamInviteAskCallBack 游戏服务器-> 游戏客户端
// 邀请入队信息
// *************************************************************************** //
message CMessageTeamInviteAskCallBack
{
	optional CMessageRouter Router  = 		1; 
	optional uint32			Count  = 		2; 
	optional string			HeadName  = 		3; 
	optional string			Name    = 		4; 		// 邀请者name
	optional uint32			Nationality	 = 		5; 	// 邀请者国籍
	repeated PBMember		Member  = 		6; 
	optional uint32			BuffLen  = 		7; 		// TODO deleted
	optional uint32			CharID  = 		8; 		// 邀请者charid
	optional uint32			EntityID  = 		9; 		// 邀请者的实体id
	optional uint32			TeamID  = 		10; 
	optional uint32			Distribute  = 		11; 
	optional string			Buff  = 		12; 			// TODO deleted
};

// *************************************************************************** //
// CMessageTeamTickCallBack 游戏服务器-> 游戏客户端
// 发送踢人返回信息
// *************************************************************************** //
message CMessageTeamTickCallBack
{
	optional string			Name	 = 		1; 		// 被踢玩家name
	optional uint32			CharID  = 		2; 			// 被踢的玩家
	optional uint32			EntityID  = 		3; 		// 被踢玩家实体ID
};

// *************************************************************************** //
// CMessageTeamErrMsgCallBack 游戏服务器-> 游戏客户端
// 操作失败
// *************************************************************************** //
message CMessageTeamErrMsgCallBack
{
	optional CMessageRouter Router  = 		1; 
	optional string		Name  = 		2; 
	optional uint32		Code  = 		3; 
	optional uint32		CharID  = 		4; 
};

// *************************************************************************** //
// CMessageTeamAskTeamMsg 游戏客户端-> 游戏服务器
// 发送求组信息
// *************************************************************************** //
message CMessageTeamAskTeamMsg
{
	optional uint32		ConscribeType  = 		1; 
	optional uint32		SubType  = 		2; 
	optional string		Msg  = 		3; 
};

// *************************************************************************** //
// CMessageTeamAskTeamCallBack Server-> client
// *************************************************************************** //
message CMessageTeamAskTeamCallBack
{
	optional string		Msg  = 		1; 
	optional string		Name  = 		2; 					// 发送求组信息的玩家
	optional uint32		Level  = 		3; 
	optional uint32		SubType  = 		4; 
	optional uint32		Metier  = 		5; 
	optional uint32		ConscribeType  = 		6; 
};

//*************************************************************************** //
// CMessageTeamConscirbeMsg 游戏客户端-> 游戏服务器
// 发送招募信息
// *************************************************************************** //
message CMessageTeamConscribeMsg
{
	optional uint32		ConscribeType  = 		1; 
	optional uint32		SubType  = 		2; 
	optional uint32		MaxLevel  = 		3; 
	optional string		Msg  = 		4; 
	optional uint32		MinLevel  = 		5; 
	optional uint32		Metier  = 		6; 
};

/************************************************************************/
/*    CMessageTeamConscribeCallBack                                                                  */
/************************************************************************/
message CMessageTeamConscribeCallBack
{
	optional uint32		MaxLevel  = 		1; 
	optional uint32		Metier  = 		2; 
	optional string		Msg  = 		3; 
	optional uint32		Count  = 		4; 
	optional uint32		MinLevel  = 		5; 
	optional uint32		ConscribeType  = 		6; 
	optional uint32		SubType  = 		7; 
	optional string		HeaderName  = 		8; 
};

//************************************************************************/
//CMessageTeamPropertyNotifyCallBack 服务器端->客户端
//同步队伍信息
//
//************************************************************************/


message CMessageTeamPropertyNotifyCallBack
{
	//optional uint32		CharID = 2;
	//optional string		Name = 3;
	//optional uint32		HP = 4;
	//optional uint32		MP = 5;
	//optional uint32		AP = 6;
	//optional uint32		Metier = 7;
	//optional uint32		Level = 8;
	//optional uint32		XPos = 9;
	//optional uint32		YPos = 10;
	//optional uint32		LineID = 11;	// 当前活动的服务器ID
	//optional uint32		MapID = 12;		// 当前活动的地图ID
	optional CMessageRouter Router  = 		1; 
	repeated PBMember	ItemPro  = 		2; 		// 队员pro
	optional uint32		ItemSize  = 		3; 		// 队员数量
};

//***********************************************************************/
// CMessageTeamChangeDistribute客户端->服务器端
//改变分配模式
//
//************************************************************************/

message CMessageTeamChangeDistribute
{
	optional uint32		Distribute   = 		1; 
};

/************************************************************************/
/* CMessageTeamChangeDistributeCallBack 服务器端->客户端
发送改变分配模式的返回信息
*/
/************************************************************************/


message CMessageTeamChangeDistributeCallBack
{
	optional uint32		Distribute  = 		1; 
};

/************************************************************************/
/* CMessageTeamNotifyBuffCallBack服务器端->客户端
同步Buff信息
*/
/************************************************************************/

message CMessageTeamNotifyBuffCallBack
{
	optional uint32		IsAdd  = 		1; 			// true -- 增加buff, false -- 删除buff
	optional string		Name  = 		2; 
	optional uint32		CharID  = 		3; 
	optional uint32		BuffID  = 		4; 
};

/************************************************************************/
/* CMessageTeamBuffCallBack                                                                     */
/************************************************************************/
message CMessageTeamBuffCallBack
{
	optional uint32		BuffSize	 = 		1;  
	optional string		BuffList	 = 		2; 
	optional string		Name		 = 		3; 
	optional uint32		CharID		 = 		4; 
	optional uint32		BuffCount	 = 		5; 
};

///************************************************************************/
///*   CMessageTeamAliveStateCallBack                                                                   */
///************************************************************************/
//
//message CMessageTeamAliveStateCallBack
//{
//	optional CMessageRouter Router = 1;
//	optional uint32			IsAlive = 2;	// 在线、离线、死亡、跨场景
//	optional string			Name = 3;
//	optional uint32			CharID = 4;
//};

/************************************************************************/
/* CMessageTeamClearRequestList    清除申请列表                                                                  */
/************************************************************************/

message CMessageTeamClearRequestList 
{
	optional uint32			CharID  = 		1; 		// 清除对象的charid
};

/************************************************************************/
/* CMessageTeamClearInviteList    清除邀请列表                                                                  */
/************************************************************************/

message CMessageTeamClearInviteList 
{
	optional uint32			CharID  = 		1; 		// 清除对象的charid
};

/************************************************************************/
/* CMessageTeamClearRequestCallBack    清除申请列表callback                                                                  */
/************************************************************************/

message CMessageTeamClearRequestCallBack
{
	optional uint32			CharID  = 		1; 		// 清除对象的charid
};

/************************************************************************/
/* CMessageTeamClearInviteCallBack    清除邀请列表callback                                                                  */
/************************************************************************/

message CMessageTeamClearInviteCallBack
{
	optional uint32			CharID  = 		1; 		// 清除对象的charid
};

//************************************************************************/
// CMessageTeamSearchMsg  
//	求组搜索	
//
//************************************************************************/

message CMessageTeamSearchMsg
{
	optional uint32		ConscibeType  = 		1; 	// 一级条件
	optional uint32		SubType  = 		2; 		// 二级条件
	optional uint32		PageIndex  = 		3; 		// 分页号
	optional uint32		MsgType  = 		4; 		// 初始窗口、求组条件搜索
};

/************************************************************************/
/* CMessageTeamSearchMsgCallBack                                                                     */
/************************************************************************/

message CMessageTeamSearchMsgCallBack
{
	optional CMessageRouter Router  = 		1; 
	optional uint32		TotalPage  = 		2; 		// 总页数
	optional bytes		MsgBuff   = 		3; 
	optional uint32		MsgSize  = 		4; 		// 信息数量
	optional uint32		PageIndex  = 		5; 		// 分页号
	optional uint32		BuffLen  = 		6; 		// 字符长度
	optional uint32		MsgType  = 		7; 
	optional uint32		MsgCount  = 		8; 	
	repeated PBTeamMsg	Msg  = 		9; 			// 搜索得到招募信息
};

/************************************************************************/
/* CMessageTeamSearchConscribeMsg                                                                     */
/************************************************************************/

message CMessageTeamSearchConscribeMsg
{
	optional uint32		MsgType  = 		1; 		// 初始界面、招募条件搜索
	optional uint32		MaxLevel  = 		2; 
	optional uint32		PageIndex  = 		3; 		// 分页号
	optional uint32		MinLevel  = 		4; 		
	optional uint32		ConscribeType  = 		5; 	// 一级条件
	optional uint32		Metier  = 		6; 
	optional uint32		SubType  = 		7; 		// 二级条件
};

/************************************************************************/
/* CMessageTeamSearchConscribeCallBack                                                                     */
/************************************************************************/

message CMessageTeamSearchConscribeCallBack
{
	optional CMessageRouter Router  = 		1; 
	optional bytes		MsgBuff   = 		2; 
	optional uint32		BuffLen  = 		3; 			// 字符长度
	optional uint32		MsgType  = 		4; 
	optional uint32		MsgCount  = 		5; 	
	optional uint32		PageIndex  = 		6; 		// 分页号
	repeated PBTeamMsg	Msg  = 		7; 
	optional uint32		MsgSize  = 		8; 		// 信息数量
	optional uint32		TotalPage  = 		9; 		// 总页数
};

/************************************************************************/
/* CMessageTeamDeleteMsg                                                                     */
/************************************************************************/

message CMessageTeamDeleteMsg 
{
	optional uint32		MsgType  = 		1; 	// 求组？招募
	optional uint32		CharID  = 		2; 
};

/************************************************************************/
/* CMessageTeamDeleteMsgCallBack                                                                     */
/************************************************************************/

message CMessageTeamDeleteMsgCallBack
{
	optional uint32			EntityID  = 		1; 
	optional uint32			MsgType  = 		2; 
};

/************************************************************************/
/* CMessageTeamOrderFollow        命令队伍跟随                                                             */
/************************************************************************/

message CMessageTeamOrderFollow 
{
	optional uint32			Code  = 		1; 	// 1--命令跟随，0--取消跟随 
};

/************************************************************************/
/* CMessageTeamOrderFollowCallBack                                                                     */
/************************************************************************/

message CMessageTeamOrderFollowCallBack 
{
	optional uint32			CharID		 = 		1; 
	optional string			HeaderName	 = 		2; 
	optional uint32			Code		 = 		3; 	// 1--命令跟随，0--取消跟随 
};

/************************************************************************/
/* CMessageTeamRequestFollow                  请求组队跟随                     */
/************************************************************************/

message CMessageTeamRequestFollow 
{
	optional uint32			Code		 = 		1; 	// 1--请求跟随， 0--取消跟随
};

/************************************************************************/
/* CMessageTeamRequestFollowCallBack                                                                     */
/************************************************************************/

message CMessageTeamRequestFollowCallBack
{
	optional string Name  = 		1; 			// 请求跟随者
	optional uint32	CharID  = 		2; 
};

/************************************************************************/
/* CMessageTeamTwoParamCallBack    操作时需要2个参数完成提示的消息                         */
/************************************************************************/

message CMessageTeamTwoParamCallBack 
{
	optional string		DesName  = 		1; 				// right parameter
	optional uint32		DesCharID  = 		2; 
	optional string		SrcName  = 		3; 				// left parameter
	optional uint32		Code  = 		4; 
	optional uint32		SrcCharID  = 		5; 
};

message CMessageTeamStateChange
{
	optional uint32		TeamFlag  = 		1; 	// 主角队伍标志，为角色ID 切记！！！
	optional uint32		CharID  = 		2; 		// 主角角色ID
	optional uint32		EntityID  = 		3; 	// 暂时保留				
};

//******************************************************
// CMessageRedirectJoinTeamResponse
//******************************************************

message CMessageRedirectJoinTeamResponse
{
	optional CMessageRouter Router   = 		1; 
	optional uint32 ItemID			 = 		2; 
	optional uint32	HeadID			 = 		3; 
};

//******************************************************
// CMessageRedirectTeamRelationNotice
// 场景产生队伍关系的通知
//******************************************************

message CMessageRedirectTeamRelationNotice
{
	optional CMessageRouter Router  = 		1; 
	optional uint32		DesCharID  = 		2; 	// 接受人
	optional uint32		IsAdd	   = 		3; 	// true -- 创建， false --删除
	optional uint32		SrcCharID  = 		4; 	// 发起人
	optional string		DesName	   = 		5; 
	optional uint32		Type	   = 		6; 	// 邀请？申请
	optional string		SrcName	   = 		7; 
};

//****************************************************
// 跨场景通知对方创建队伍
//****************************************************

message CMessageRedirectCreateTeamNotice
{
	optional CMessageRouter Router  = 		1; 
	optional uint32		CharID	   = 		2; 
	optional uint32		SrcCharID  = 		3; 
	optional string		CharName   = 		4; 
};

//**********************************
// CMessageTeamNotice 组队提示
//**********************************

message CMessageTeamNotice
{
	optional uint32 	Code	 = 		1; 
	optional string		Param2	 = 		2; 
	optional string		Param1	 = 		3; 	// default
};

////******************************************************
//// CMessageUpdateHeadTipsNotice
//// 服务器 -> 客户端 通知客户端更新头顶显示
//// TeamFlag参数
////******************************************************
//
//message CMessageUpdateHeadTipsNotice
//{
//	optional uint32			EntityID	= 1;	// 主角实体id
//	optional uint32			TeamFlag	= 2;	// 主角队伍标志，为队长的EntityID
//	optional uint32			AddClaim	= 3;	// 为bool类型，true:显示，false:清除
//	optional PBClaimMsg		ClaimMsg	= 4;	// 求组信息
//	optional uint32			AddRecruit	= 5;	// 为bool类型，true:显示，false:清除
//	optional PBRecruitMsg	RecruitMsg	= 6;	// 招募信息
//};

