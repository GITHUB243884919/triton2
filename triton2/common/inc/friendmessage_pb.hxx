//  好友系统消息定义描述文件
//  作者：李彦  08-10-15
//  版本：1.0

import "coremessage_pb.hxx";

//  以最快速度序列/反序列
option optimize_for = SPEED;

//  把所有消息的ID集中再次，防止重复定义
//  friend message消息段ID范围[0x1800 -- 0x1C00)
//  末位为R的是路由消息

enum FRIEND_MSG_ID
{
	ID_C2S_FRIEND_INVITE_REQUEST                    = 0x1800 ;	// 好友邀请请求
	ID_S2C_FRIEND_INVITE_NOTICE_R                   = 0x1A01 ;  // 好友邀请通知 + 0x0200
	ID_C2S_FRIEND_INVITE_OPTION                     = 0x1802 ; 	// 好友邀请回复
	ID_S2S_FRIEND_INVITE_RESPONSE_R                 = 0x1A03 ; 	// 新增好友回应 + 0x0200
	ID_S2C_FRIEND_INVITE_RESPONSE_NOTICE            = 0x1804 ; 	// 新增好友回应 发给客户端
	ID_C2S_FRIEND_DEL_REQUEST                       = 0x1805 ;	// 删除好友请求
	ID_S2C_FRIEND_ERRCODE_NOTICE					= 0x1A06 ;  // 错误码+ 0x0200 (路由消息)
	ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE				= 0x1A07 ;  // 更新好友通知  + 0x0200
	ID_S2C_FRIEND_GETLIST_NOTICE					= 0x1808 ;  // 发给客户端的好友列表
	ID_C2S_FRIEND_GETLIST_REQUEST					= 0x1809 ;  // 客户端获得好友信息的请求
	ID_S2C_FRIEND_GROUPNAME_NOTICE					= 0x180B ;  // 发给客户端组名
	ID_S2S_FRIEND_CHANGESTATUS_NOTICE_R				= 0x1A0C ;  // 好友状态发生改变通知(服务器到服务器) + 0x0200
	ID_C2S_FRIEND_CHANGESTATUS_REQUEST				= 0x180D ;  // 改变状态请求
	ID_S2C_FRIEND_CHANGESTATUS_NOTICE				= 0x180E ;  // 好友状态发生改变通知 (发给本服务器客户端)

	ID_S2S_FRIEND_ADDFRIEND_NOTICE					= 0x1A0F ;  // 发给服务器的添加好友成功通知 + 0x0200 (路由消息)
	ID_C2S_FRIEND_CHGGROUPNAME_REQUEST				= 0x1810 ;  // 玩家修改组名请求
	ID_C2S_FRIEND_CHGFRIENDGROUP_REQUEST			= 0x1811 ;  // 玩家移动好友到其他分组的请求
	ID_C2S_FRIEND_SENDMESSAGE_REQUEST				= 0x1812 ;  // 玩家发送消息请求
	ID_S2C_FRIEND_MESSAGE_NOTICE_R					= 0x1A13 ;  // 玩家收到消息的通知
	ID_C2S_FRIEND_MOVETOBLACK_REQUEST				= 0x1814 ;  // 拖黑请求
	ID_C2S_FRIEND_ADDBLACK_REQUEST					= 0x1815 ;  // 添加黑名单
	ID_S2C_FRIEND_CHGFRIENDGROUP_NOTICE				= 0x1816 ;  // 移动分组通知
	ID_S2C_FRIEND_MOVETOBLACK_NOTICE				= 0x1817 ;  // 移动到黑名单通知
	ID_C2S_FRIEND_LOCKENEMY_REQUEST					= 0x1818 ;  // 锁定仇敌请求
	
	ID_C2S_FRIEND_GETFRIENDSSTATE_REQUEST			= 0x1819 ;  // 客户端请求刷新好友状态
	ID_S2C_FRIEND_GETFRIENDSSTATE_NOTICE			= 0x181A ;  // 客户端请求刷新好友状态 回复
	ID_C2S_FRIEND_UPDATEENEMY_REQUEST				= 0x181B ;  // 客户端请求更新仇敌

	ID_C2S_FRIEND_ADDFRIENDBYNAME_REQUEST			= 0x181C ;  // 客户端根据名字加好友

	ID_C2S_REQUEST_CHANGEMASTER						= 0x181D;	// 客户端->服务器 选择前辈请求
	ID_S2C_RESPONSE_CHANGEMASTER					= 0x1820;	// 服务器->客户端 选择前辈回应
	ID_S2C_NOTIFY_CHANGEMASTER						= 0x1821;	// 服务器->客户端 选择前辈通知

	ID_C2S_REQUEST_GETSLAVE							= 0x1822;	// 客户端->服务器 获得后辈列表
	ID_S2C_RESPONSE_GETSLAVE						= 0x1823;	// 服务器->客户端 获得后辈列表
	ID_S2C_NOTIFY_SLAVELEVELUP						= 0x1824;	// 服务器->客户端 后辈玩家升级
	ID_S2C_NOTIFY_ANSWERCHANGE						= 0x1825;	// 回复前辈选择

	ID_C2S_REQUEST_QUERYPLAYER						= 0x1826;	// 客户端->服务器 查询人物名称
	ID_S2C_RESPONSE_QUERYPLAYER						= 0x1827;
	ID_S2C_NOTIFY_SLAVEONOFF						= 0x1828;	// 服务器->客户端 上下线通知
	ID_S2C_NOTIFY_SLAVENUM							= 0x1829;	// 服务器->客户端 后辈奖励通知
	ID_C2S_REQUEST_REMOVESLAVE						= 0x182A;	// 服务器->客户端 去除后辈请求
	ID_S2C_RESPONSE_REMOVESLAVE						= 0x182B;	
	ID_S2C_NOTIFY_REMOVESLAVE						= 0x182C;
	ID_C2S_REQUEST_REMOVEMASTER						= 0x182D;	// 服务器->客户端 去除前辈请求
	ID_S2C_RESPONSE_REMOVEMASTER					= 0x182E;	
	ID_S2C_NOTIFY_REMOVEMASTER						= 0x182F;
	ID_C2S_REQUEST_ALTERMASTERPERCENT				= 0x1830;	// 服务器->客户端 调整感恩比例
	ID_S2C_RESPONSE_ALTERMASTERPERCENT				= 0x1831;
	ID_C2S_REQUEST_GETLOWPLAYER						= 0x1832;	// 服务器->客户端 获得低等级玩家
	ID_S2C_RESPONSE_GETLOWPLAYER					= 0x1833;
	ID_C2S_REQUEST_ANSWERCHANGE						= 0x1834;	// 客户端->服务器 回复前辈选择

	ID_S2C_NOTIFY_FRIENDLEVELUP						= 0x1A35;	// 服务器->客户端 好友升级的通知(整10级)(路由消息)	
	ID_C2S_REQUEST_FRIENDBLESS						= 0x1836;	// 客户端->服务器 好友升级的祝福
	ID_S2C_RESPONSE_FRIENDBLESS						= 0x1837;	// 服务器->客户端 好友升级的祝福回应	
	ID_S2S_NOTIFY_FRIENDBLESS						= 0x1A38;	// 服务器->服务器 好友的祝福通知	
	ID_S2S_NOTIFY_MODIFYINTIMACY					= 0x1839;	// 服务器->服务器 同步好友亲密度	
	ID_S2S_NOTIFY_MODIFYLASTTIMEALLONLINE			= 0x183A;	// 服务器->服务器 最后同时在线时间	

	ID_C2S_REQUEST_CREATESWORNTEAM					= 0x183B;	// 客户端->服务器 创建结拜团体的请求
	ID_S2C_RESPONSE_CREATESWORNTEAM					= 0x183C;	// 服务器->客户端 创建结拜团体的消息回应
	ID_C2S_REQUEST_LEAVESWORNTEAM					= 0x183D;	// 客户端->服务器 离开结拜的消息请求
	ID_S2C_RESPONSE_LEAVESWORNTEAM					= 0x183E;	// 服务器->客户端 离开结拜的消息回应
	ID_C2S_REQUEST_KICKSWORNMEMBER					= 0x1840;	// 客户端->服务器 踢出结拜成员的消息请求
	ID_S2C_RESPONSE_KICKSWORNMEMBER					= 0x1841;	// 服务器->客户端 踢出结拜成员的消息回应
	ID_C2S_REQUEST_DISBANDSWORNTEAM					= 0x1842;	// 客户端->服务器 解散结拜团体的消息请求
	ID_S2C_RESPONSE_DISBANDSWORNTEAM				= 0x1843;	// 服务器->客户端 解散结拜团体的消息回应
	ID_S2C_NOTIFY_SWORNINFO							= 0x1844;	// 服务器->客户端 结拜团体信息的通知
	ID_S2C_NOTIFY_LEAVESWORNTEAM					= 0x1845;	// 服务器->客户端 离开结拜团体的消息通知
	ID_S2C_NOTIFY_KICKSWORNMEMBER					= 0x1846;	// 服务器->客户端 踢人的通知
	ID_C2S_REQUEST_CHANGESWORNNAME					= 0x1847;	// 客户端->服务器 结拜改名的请求
	ID_S2C_RESPONSE_CHANGESWORNNAME					= 0x1848;	// 服务器->客户端 结拜改名的回应
	ID_S2C_NOTIFY_CHANGESWORNNAME					= 0x1849;	// 服务器->客户端 结拜改名的通知
	ID_S2C_NOTIFY_DISBANDSWORNTEAM					= 0x184A;	// 服务器->客户端 解散结拜团体的消息通知
	ID_S2C_NOTIFY_CREATESWORNTEAM					= 0x184B;	// 服务器->客户端 结拜团体创建成功的通知消息
	
	ID_S2C_NOTIFY_LASTCONTACTER						= 0x1A4C;	// 服务器->客户端 把最后一个联系人告知客户端
	ID_S2G_NOTIFY_UPDATELASTCONTACTER				= 0x184D;   // 场景->网关 更新最后一个联系人信息
	ID_S2G_NOTIFY_UPLOADCONTACTERLIST				= 0x184E;   // 场景->网关 上传联系人列表信息
	ID_G2S_NOTIFY_CONTACTERSTATUS					= 0x1850;   // 网关->场景 将玩家状态通知给联系人
	ID_S2C_NOTIFY_CONTACTERSTATUS					= 0x1851;   // 场景->客户端 将玩家状态通知给联系人
	ID_C2S_REQUEST_REBUILDSWORN						= 0x1852;   // 客户端->场景 重建结拜的消息请求
	ID_S2C_RESPONSE_REBUILDSWORN					= 0x1853;   // 场景->客户端 重建结拜的消息回应
	ID_S2C_NOTIFY_REBUILDSWORN						= 0x1854;   // 场景->客户端 重建结拜的消息通知
};


// *************************************************************************** //
// CMessageInviteFriendRequest 客户端->游戏服务器
//
// 玩家请求增加好友 根据ID
// *************************************************************************** //
message CMessageInviteFriendRequest
{
	optional uint32 PlayerID    = 		1;  // 被加的好友角色ID
};


// *************************************************************************** //
// CMessageInviteFriendRequestByName 客户端->游戏服务器
//
// 玩家请求将mInvitedPlayerName增加为好友
// *************************************************************************** //
message CMessageInviteFriendRequestByName
{
	optional string InvitedPlayerName    = 		1;  // 被加的好友名字
};


// *************************************************************************** //
// CMessageInviteFriendNotice 游戏服务器->客户端
// 
// 服务器通知对端玩家，某人将他添加为好友, 并征求对方是否同意 (路由消息 路由信息里有名字)
// *************************************************************************** //
message CMessageInviteFriendNotice
{
	optional CMessageRouter Router			 = 		1;    
	optional uint32			SrcHead			 = 		2; 	// 邀请者的头像ID
	optional uint32         SrcCharID		 = 		3; 	// 邀请者的ID，需要客户端存储这个ID
	optional uint32			SrcMetier		 = 		4; 	// 邀请者的职业ID，也是门派ID
	optional uint32			SrcLevel		 = 		5; 	// 邀请者的等级
	optional uint32			SrcSex			 = 		6; 	// 邀请者的性别
	optional uint32         DesCharID		 = 		7; 	// 被邀请者的ID
};


// *************************************************************************** //
// CMessageInviteFriendOption 客户端->场景服务器
// 
// 对端玩家是否同意玩家的好友要求回复
// *************************************************************************** //
message CMessageInviteFriendOptionRequest 
{
	optional uint32 InvitingPlayerID		 = 		1; 	// 邀请者的ID
	optional uint32 AgreeCode				 = 		2;     // 1=同意，0=拒绝
};


// *************************************************************************** //
// CMessageInviteFriendResponse 场景服务器->场景服务器
// 
// 对端玩家是否同意玩家的好友要求回复 发到邀请者所在的服务器
// *************************************************************************** //
message CMessageInviteFriendResponseMsg
{
	optional CMessageRouter Router			 = 		1;    
	optional uint32 IsDesInMyLst			 = 		2; 	// 邀请者已经在我的好友列表中吗
	optional uint32 BeInvitingPlayerNation	 = 		3; 	// 被邀请者的国家
	optional uint32 BeInvitingPlayerHead     = 		4; 	// 被邀请者的头像
	optional string BeInvitingPlayerName     = 		5; 	// 被邀请者的名字
	optional uint32 BeInvitingPlayerSex	     = 		6; 	// 被邀请者的性别
	optional uint32	IsFriendLstFull			 = 		7; 	// 被邀请者的好友列表是否已满
	optional uint32 AgreeCode				 = 		8;     // 1=同意，0=拒绝
};

// *************************************************************************** //
// CMessageInviteFriendResponseNotice 场景服务器->客户端
// 
// 对端玩家是否同意玩家的好友要求回复 由邀请者所在的服务器发到邀请者的客户端
// *************************************************************************** //
message CMessageInviteFriendResponseNotice
{
	optional string BeInvitingPlayerName		 = 		1; 	// 被邀请者的名字
	optional uint32 AgreeCode					 = 		2;     // 1=同意，0=拒绝
};

// *************************************************************************** //
// CMessageDelFrientRequest 客户端->场景服务器
// 
// 删除好友的请求
// *************************************************************************** //
message CMessageDelFrientRequest
{
	optional uint32 Type		 = 		1; 	// 类型，EDelFriendType
	optional uint32 CharID		 = 		2; 	// 要删除的好友的char id
};


// *************************************************************************** //
// CMessageFriendErrorNotice 场景服务器->客户端
// 
// 错误码通知
// *************************************************************************** //
message CMessageFriendErrorNotice
{
	optional CMessageRouter Router			 = 		1;    
	optional int32 ErrorCode				 = 		2; 	// 错误码
};


// *************************************************************************** //
// CMessageChangeFriendNotice 场景服务器->客户端
// 
// 好友发生变更的通知
// *************************************************************************** //
message CMessageChangeFriendNotice
{
	optional CMessageRouter Router	 = 		1;    
	optional string Name			 = 		2; 	// 要操作的好友的名字
	optional uint32 NationID		 = 		3; 	// 国籍
	optional uint32 Sex				 = 		4;     // 性别
	optional uint32 ReltoMe			 = 		5;     // 和我的关系
	optional uint32 OprCode			 = 		6;     // 操作码，具体查看宏的定义(上面5个值哪个有效由具体的操作决定，请看宏的注释)
	optional uint32 TimeOrder		 = 		7;    // 仇敌时间戳
	optional uint32 IsLock			 = 		8;    // 仇敌是否锁定 1=锁定
	optional uint32 Status			 = 		9; 	// 状态码
	optional uint32 CharID			 = 		10; 	// 要操作的好友的ID
	optional uint32 BeKilled		 = 		11;    // 被杀的次数
	optional uint32 Killed			 = 		12;    // 杀他的次数
	optional uint32 Head			 = 		13; 	// 头像ID
	optional uint32 GroupID			 = 		14;     // 分组ID  PLAYER_GROUP_COUNT+1是黑名单
	optional uint32 VipFlag			 = 		15;   // 会员标志
};

message SingleFriendData
{
	optional uint32 BeKilled		 = 		1;  // 被杀的次数
	optional uint32 Head			 = 		2;   // 头像ID
	optional uint32 Status			 = 		3;   // 状态码
	optional uint32 GroupID			 = 		4;   // 分组ID  PLAYER_GROUP_COUNT+1是黑名单
	optional uint32 CharID			 = 		5;   // 好友的角色ID  每条消息最多MAX_ONCE_FRIENDS个
	optional string Name			 = 		6;   // 名字
	optional uint32 NationID		 = 		7; 	// 国籍
	optional uint32 Relation		 = 		8;   // 和我的关系
	optional uint32 Sex				 = 		9;   // 性别
	optional uint32 IsLock			 = 		10;   // 仇敌是否锁定 1=锁定
	optional uint32 TimeOrder		 = 		11;   // 仇敌时间戳
	optional uint32 Killed			 = 		12;   // 杀他的次数
	optional uint32	VipFlag			 =		13;		// 会员标志
	optional uint32	IntimacyDegree	 =		14;		// 亲密度
};

// *************************************************************************** //
// CMessageFriendListNotice 场景服务器->客户端
// 
// 服务器根据客户端传来的好友的char id 返回好友的名字和状态，以后可能有其他信息
// *************************************************************************** //
message CMessageFriendGetListNotice
{
	repeated SingleFriendData	Friends		 = 		1;   // 所有好友
	repeated string				GroupName	 = 		2;   // 组名，肯定是PLAYER_GROUP_COUNT个
};


// *************************************************************************** //
// CMessageFriendGetListRequest 客户端->场景服务器
// 
// 客户端取得好友的名字和状态等信息，以后可能有其他信息 <作废>
// *************************************************************************** //
message CMessageFriendGetListRequest
{
};


// *************************************************************************** //
// CMessageFriendGroupNameNotice 场景服务器->客户端
// 
// 玩家进入场景的时候给客户端分组信息(组名)
// *************************************************************************** //
message CMessageFriendGroupNameNotice
{
	repeated uint32 GroupID			 = 		1;   // 组ID，肯定是PLAYER_GROUP_COUNT个
	repeated string GroupName		 = 		2;   // 组名，肯定是PLAYER_GROUP_COUNT个
};

// *************************************************************************** //
// CMessageFriendChangeStatusNotice_R 场景服务器->场景服务器
// 
// 好友状态改变通知
// *************************************************************************** //
message CMessageFriendChangeStatusNotice_R
{
	optional CMessageRouter Router	 = 		1;    
	optional uint32		    Status	 = 		2;   // 新的状态码
	optional uint32		    Rels	 = 		3;   // 关系
	optional uint32			VipFlag	 =		4;	 // 会员标志
};


// *************************************************************************** //
// CMessageFriendChangeStatusNotice 场景服务器->客户端
// 
// 好友状态改变通知
// *************************************************************************** //
message CMessageFriendChangeStatusNotice
{
	optional uint32 CharID			 = 		1;   // 好友的角色ID
	optional uint32 Status			 = 		2;   // 新的状态码
	optional uint32 Rels			 = 		3;   // 关系
	optional uint32			VipFlag	 =		4;	 // 会员标志
};


// *************************************************************************** //
// CMessageFriendChangeStatusRequest 客户端->场景服务器
// 
// 玩家改变状态请求
// *************************************************************************** //
message CMessageFriendChangeStatusRequest
{
	optional uint32 Status			 = 		1;   // 新的状态码
};

// *************************************************************************** //
// CMessageAddFriendNotice 场景服务器->场景服务器
// 
// 添加好友成功，通知被邀请者所在的场景服务器
// *************************************************************************** //
message CMessageAddFriendNotice
{
	optional CMessageRouter Router	 = 		1;    
	optional uint32 CharID			 = 		2; 	// 要操作的好友的ID
	optional uint32 ReltoMe			 = 		3;     // 和我的关系
	optional uint32 Status			 = 		4; 	// 状态码
	optional uint32 GroupID			 = 		5;     // 分组ID
};

// *************************************************************************** //
// CMessageFriendChgGroupNameRequest 客户端->场景服务器
// 
// 修改组名请求 (完成后用Group Notice通知)
// *************************************************************************** //
message CMessageFriendChgGroupNameRequest
{
	optional string NewName			 = 		1; 	// 新名字
	optional uint32 GroupID			 = 		2; 	// 要操作的分组ID 范围：1-PLAYER_GROUP_COUNT
};

// *************************************************************************** //
// CMessageFriendChgFriendGroupRequest 客户端->场景服务器
// 
// 移动好友分组请求 （只能移动好友，不能移动黑名单和仇敌）
// *************************************************************************** //
message CMessageFriendChgFriendGroupRequest
{
	optional uint32 CharID				 = 		1; 	// 好友的角色ID
	optional uint32 DesGroupID			 = 		2; 	// 目标分组ID 范围：1-PLAYER_GROUP_COUNT
};

// *************************************************************************** //
// CMessageFriendSendMessageRequest 客户端->场景服务器
// 
// 发送消息请求
// *************************************************************************** //
message CMessageFriendSendMessageRequest
{
	optional string Msg				 = 		1; 	// 消息内容，最长 MAX_FRIEND_MSG_LENGTH 
	optional uint32 CharID			 = 		2; 	// 目标char id
	optional uint32 AutoFlag		 = 		3; 	// 是否自动的消息
};


// *************************************************************************** //
// CMessageFriendMessageNotice 场景服务器->客户端
// 
// 收到消息通知
// *************************************************************************** //
message CMessageFriendMessageNotice
{
	optional CMessageRouter Router	 = 		1;    
	optional uint32 AutoFlag		 = 		2; 	// 是否自动的消息
	optional uint32 CharID			 = 		3; 	// 源char id
	optional uint32 Sex				 = 		4; 
	optional string Msg				 = 		5; 	// 消息内容，最长 MAX_FRIEND_MSG_LENGTH 
	optional uint32 FaceID			 = 		6; 	// 头像
	optional string Name			 = 		7; 
	optional uint32	SendTime		 =		18;	// 发送时间
	optional uint32	CampID			 =		19;	// 阵营ID	
};


// *************************************************************************** //
// CMessageFriendMoveToBlackRequest 客户端->场景服务器
// 
// 移动好友到黑名单请求
// *************************************************************************** //
message CMessageFriendMoveToBlackRequest
{
	optional uint32 CharID			 = 		1; 	// 目标char id
};


// *************************************************************************** //
// CMessageFriendAddBlackRequest 客户端->场景服务器
// 
// 添加黑名单请求
// *************************************************************************** //
message CMessageFriendAddBlackRequest
{
	optional string  DesName			 = 		1; 	// 目标名字
};

// *************************************************************************** //
// CMessageFriendChgFriendGroupNotice 场景服务器->客户端
// 
// 移动好友分组通知
// *************************************************************************** //
message CMessageFriendChgFriendGroupNotice
{
	optional uint32 CharID				 = 		1; 	// 好友的角色ID
	optional uint32 DesGroupID			 = 		2; 	// 目标分组ID 范围：1-PLAYER_GROUP_COUNT
};

// *************************************************************************** //
// CMessageFriendMoveToBlackNotice 场景服务器->客户端
// 
// 移动好友分到黑名单通知
// *************************************************************************** //
message CMessageFriendMoveToBlackNotice
{
	optional uint32 CharID				 = 		1; 	// 角色ID
};


// *************************************************************************** //
// CMessageFriendLockEnemyRequest 客户端->场景服务器
// 
// 锁定仇敌请求
// *************************************************************************** //
message CMessageFriendLockEnemyRequest
{
	optional uint32 CharID				 = 		1; 	// 角色ID
	optional uint32 LockType			 =      2;	// 操作类型
};


// *************************************************************************** //
// CMessageFriendGetFriendsStateRequest 客户端->场景服务器
// 
// 客户端请求好友和仇敌状态
// *************************************************************************** //
message CMessageFriendGetFriendsStateRequest
{
};

// *************************************************************************** //
// CMessageFriendGetFriendsStateNotice 场景服务器->客户端
// 
// 客户端请求好友和仇敌状态回复
// *************************************************************************** //
message CMessageFriendGetFriendsStateNotice
{
	repeated uint32	States				 = 		1; 	// 状态
	repeated uint32 Rels				 = 		2; 	// 关系(好友，仇敌)
	repeated uint32 CharIDs				 = 		3; 	// 角色ID
	repeated uint32	VipFlag				 =		4;	// vip状态
};


// *************************************************************************** //
// CMessageFriendUpdateEnemyRequest 客户端->场景服务器
// 
// 玩家被杀后，向服务器请求添加仇敌
// *************************************************************************** //
message CMessageFriendUpdateEnemyRequest
{
	optional uint32 KillerCharID		 = 		1;   // 谁杀的
	optional uint32 DelEnemyCharID		 = 		2;   // 待删除的仇敌的charID，没有就不填
};

// 修改前辈玩家请求
message CMessageChangeMasterRequest
{
	optional string MasterName	= 1;
	optional uint32	CharID		= 2;
	optional uint32	NPCEntityID	= 3;
	repeated uint32	MasterID	= 4;
};

message CMessageChangeMasterResponse
{
	optional uint32	Result		= 1;
	optional uint32 MasterID	= 2;
	optional string	MasterName	= 3;
	optional uint32	CharID		= 4;
	optional uint32 Level		= 5;
	optional uint32 Metier		= 6;
};

message CMessageChangeMasterNotify
{
	optional SlaveInfo	Slave		= 1;
	optional uint32		CharID		= 2;
};

// 获得后辈信息
message CMessageGetSlaveRequest
{
	repeated uint32	SlaveID		= 1;
	optional uint32	CharID		= 2;
	optional uint32	NPCEntityID	= 3;
	repeated uint32 MasterID	= 4;
};

message SlaveInfo
{
	optional uint32	SlaveID		= 1;
	optional string SlaveName	= 2;
	optional uint32	Level		= 3;
	optional uint32	Metier		= 4;
	optional uint32	Face		= 5;
	optional uint32	Sex			= 6;
	optional uint32	Online		= 7;
};

message CMessageGetSlaveResponse
{
	optional uint32	   Result	= 1;	
	repeated SlaveInfo Slave	= 2;
	optional uint32	   CharID	= 3;
	repeated SlaveInfo Master	= 4;
};

// 后辈升级通知
message CMessageSlaveLevelUpNotify
{
	optional SlaveInfo	Slave	= 1;
	optional uint32		CharID	= 2;
};

message CMessageAnswerChangeNotify
{
	optional SlaveInfo	Slave	= 1;
	optional uint32		CharID	= 2;
};

message CMessageAnswerChangeRequest
{
	optional uint32		Result	= 1;
	optional uint32		CharID	= 2;
	optional uint32		SlaveID	= 3;
	optional uint32		Error	= 4;
};

message CMessageQueryPlayerRequest
{
	repeated uint32		QueryID	= 1;
	optional uint32		CharID	= 2;
};

message CMessageQueryPlayerResponse
{
	repeated SlaveInfo  Info	  = 1;	
	optional uint32		CharID	  = 2;
};

// 前辈后辈上下线互相通知
message CMessageSlaveOnOffNotify
{
	optional uint32		SrcCharID	 = 1;
	optional string		SrcCharName  = 2;
	optional uint32		Online		 = 3;
	repeated uint32		DesCharID	 = 4;
	optional uint32		Level		 = 5;
};

message CMessageSlaveNumNotify
{
	repeated uint32		SlaveValue	 = 1;
	optional uint32		SlavePoint	 = 2;
	optional uint32		MasterPoint	 = 3;
	optional uint32		SlaveTotal	 = 4;
	optional uint32		MasterTotal	 = 5;
};

message CMessageRemoveSlaveRequest
{
	optional uint32		SlaveID		=  1;
	optional uint32		CharID		=  2;
	optional uint32		NPCEntityID	= 3;
};

message CMessageRemoveSlaveResponse
{
	optional uint32		Result		=	1;
	optional uint32		CharID		=	2;
	optional uint32		SlaveID		=	3;
	optional uint32		Param		=	4;
};

message CMessageRemoveSlaveNotify
{
	optional uint32		CharID		=	1;
	optional uint32		MasterID	=	2;	
};

message CMessageRemoveMasterRequest
{
	optional uint32		MasterID	=  1;
	optional uint32		CharID		=  2;
	optional uint32		NPCEntityID	= 3;
};

message CMessageRemoveMasterResponse
{
	optional uint32		Result		=	1;
	optional uint32		CharID		=	2;
	optional uint32		MasterID	=	3;
};

message CMessageRemoveMasterNotify
{
	optional uint32		CharID		=	1;
	optional uint32		SlaveID		=	2;
	optional uint32		Param		=	3;
};

message CMessageAlterMasterPercentRequest
{
	optional uint32		MasterID	= 1;
	optional uint32		Percent		= 2;
};

message CMessageAlterMasterPercentResponse
{
	optional uint32		Result		= 1;
	optional uint32		MasterID	= 2;
	optional uint32		Percent		= 3;
};

message CMessageGetLowPlayerRequest
{
	optional uint32	NPCEntityID	= 1;
};

message CMessageGetLowPlayerResponse
{
	optional uint32	   Result	= 1;	
	repeated SlaveInfo Player	= 2;	// 查找结果
	repeated SlaveInfo Required	= 3;	// 默认返回一部分
};

// *************************************************************************** //
// CMessageFriendLevelUpNotify
// 好友升级的通知
// *************************************************************************** //
message CMessageFriendLevelUpNotify
{
	optional CMessageRouter Router	=	1;  // 路由消息
	optional uint32	Level			=	2;	// 好友等级
	optional string	Name			=	3;	// 好友姓名		
	optional uint32	RoleID			=	4;	// 好友角色ID
};

// *************************************************************************** //
// CMessageFriendBlessRequest
// 祝福好友升级的请求
// *************************************************************************** //
message CMessageFriendBlessRequest
{
	optional uint32	 RoleID	= 1;	// 好友角色ID
};


// *************************************************************************** //
// CMessageFriendBlessResponse
// 祝福好友升级的回应
// *************************************************************************** //
message CMessageFriendBlessResponse
{
	optional uint32	 Errcode	= 1;	// 错误码
};

// *************************************************************************** //
// CMessageFriendBlessNotify
// 好友的祝福通知
// *************************************************************************** //
message CMessageFriendBlessNotify
{
	optional CMessageRouter Router		= 1;
	optional string			FriendName	= 2;	// 好友姓名
	optional uint32			Exp			= 3;	// 经验值
};

// *************************************************************************** //
// CMessageModifyIntimacyNogify
// 同步好友亲密度
// *************************************************************************** //
message CMessageModifyIntimacyNogify
{
	optional CMessageRouter Router			= 1; // 
	optional uint32			ChangeValue		= 2; // 改变的数值
	optional uint32			IntimacyDegree  = 3; // 改变后的亲密度
	optional uint32			Type			= 4; // 操作类型(EM_EntimacyChangeType)
	optional uint32			FriendID		= 5; // 亲密度改变的好友ID	
	optional uint32			UpdateTime		= 7; // 亲密度增加的改变时间
	optional uint32			DailyIntimacy	= 8; // 每天增加的亲密度数值
};

// *************************************************************************** //
// CMessageModifyLastTimeAllOnLineNotify
// 最后同时在线时间
// *************************************************************************** //
message CMessageModifyLastTimeAllOnLineNotify
{
	optional CMessageRouter Router			= 1; // 
	optional uint32			Time			= 2; // 时间值
	optional uint32			FriendID		= 3; // 好友角色ID
};

// *************************************************************************** //
// CMessageCreateSwornTeamRequest
// 创建结拜团体的请求
// *************************************************************************** //
message CMessageCreateSwornTeamRequest
{
	optional string	SwornName	=	1;		// 团体名称
};

// *************************************************************************** //
// CMessageCreateSwornTeamResponse
// 创建结拜团体的回应消息
// *************************************************************************** //
message CMessageCreateSwornTeamResponse
{
	optional uint32	Errcode = 1;	
};

// *************************************************************************** //
// CMessageLeaveSwornTeamRequest
// 离开结拜的消息请求
// *************************************************************************** //
message CMessageLeaveSwornTeamRequest
{
	
};

// *************************************************************************** //
// CMessageLeaveSwornTeamResponse
// 离开结拜的消息回应
// *************************************************************************** //
message CMessageLeaveSwornTeamResponse
{
	optional uint32	Errcode = 1;																				
};

// *************************************************************************** //
// CMessageKickSwornMemberRequest
// 踢出结拜成员的消息请求
// *************************************************************************** //
message CMessageKickSwornMemberRequest
{
	optional uint32	RoleID = 1;
};

// *************************************************************************** //
// CMessageKickSwornMemberResponse
// 踢出结拜成员的消息请求
// *************************************************************************** //
message CMessageKickSwornMemberResponse
{
	optional uint32	Errcode = 1;	
};

// *************************************************************************** //
// CMessageDisbandSwornTeamRequest
// 解散团体的消息请求
// *************************************************************************** //
message CMessageDisbandSwornTeamRequest
{

};

// *************************************************************************** //
// CMessageDisbandSwornTeamResponse
// 解散团体的消息回应
// *************************************************************************** //
message CMessageDisbandSwornTeamResponse
{
	optional uint32	Errcode = 1;	
};

// *************************************************************************** //
// CMessageSwornInfoNotify
// 结拜团体信息的通知
// *************************************************************************** //
message CMsgSwornMember
{
	optional uint32 RoleID		= 1;	
	optional string	RoleName	= 2;	// 角色名称
	optional uint32	RolePos		= 3;	// 结拜排名
	optional uint32	FaceID		= 4;	// 脸型
	optional uint32	RoleLevel	= 5;	// 角色等级
	optional uint32	MetierID	= 6;	// 门派ID	
	optional uint32	GenderID	= 7;	// 性别
};
message CMessageSwornInfoNotify
{	
	repeated CMsgSwornMember  SwornMember = 1;
	optional string			  SwornName	  = 2;	// 结拜名称
	optional uint32			  RoleID	  = 3;	// 角色ID	
};

// *************************************************************************** //
// CMessageSwornInfoNotify ID_S2C_NOTIFY_CREATESWORNTEAM
// 结拜团体创建成功的通知消息
// *************************************************************************** //
message CMessageCreateSwornTeamNotify
{
	repeated CMsgSwornMember  SwornMember = 1;
	optional string			  SwornName	  = 2;	// 结拜名称
	optional uint32			  MasterID	  = 3;	// 老大的角色ID
};

// *************************************************************************** //
// CMessageLeaveSwornTeamNotify ID_S2C_NOTIFY_LEAVESWORNTEAM
// 离开结拜团体的消息通知
// *************************************************************************** //
message CMessageLeaveSwornTeamNotify
{
	optional CMessageRouter Router = 1;
	optional uint32	 RoleID		= 2;	// 离开人的角色ID
	optional uint32	 MasterID	= 3;	// 如果离开的人是老大的话，新老大的角色ID	
};

// *************************************************************************** //
// CMessageKickSwornMemberNotify 
// 踢人的通知
// *************************************************************************** //
message CMessageKickSwornMemberNotify
{
	optional CMessageRouter Router = 1;
	optional uint32			RoleID = 2;
};

// *************************************************************************** //
// CMessageChangeSwornNameRequest
// 结拜改名的请求
// *************************************************************************** //
message CMessageChangeSwornNameRequest
{
	optional string	 SwornName = 1;
};

// *************************************************************************** //
// CMessageChangeSwornNameResponse
// 结拜改名的回应
// *************************************************************************** //
message CMessageChangeSwornNameResponse
{
	optional uint32	Errcode	=	1;
};

// *************************************************************************** //
// CMessageChangeSwornNameNotify ID_S2C_NOTIFY_CHANGESWORNNAME
// 结拜改名的通知
// *************************************************************************** //
message CMessageChangeSwornNameNotify
{
	optional CMessageRouter Router = 1;
	optional string	 SwornName = 2;
};

// *************************************************************************** //
// CMessageDisabanSwornTeamNotify 
// 结拜解散的通知
// *************************************************************************** //
message CMessageDisbandSwornTeamNotify
{
	optional CMessageRouter Router = 1;
	optional uint32	SwornID = 2;
};

// *************************************************************************** //
// CMessageLatestContactChangeStatusNotice 场景服务器->客户端
// 
// 最近联系人状态改变通知
// *************************************************************************** //
message CMessageLatestContactChangeStatusNotice
{
	optional uint32 CharID			 = 		1;   // 好友的角色ID
	optional uint32 Status			 = 		2;   // 新的状态码
	optional uint32	VipFlag			 =		3;	 // 会员标志
};

// *************************************************************************** //
// CMessageLastContacterNotice 场景服务器->客户端
// 
// 最后一个联系人通知
// *************************************************************************** //
message CMessageLastContacterNotice
{
	optional CMessageRouter Router		= 		1;  
	optional uint32 CharID				= 		2;   // 角色ID
	optional uint32 Sex					= 		3; 
	optional uint32 FaceID				= 		4; 	 // 头像
	optional string Name				= 		5; 
	optional uint32 NationID			= 		6;   // 所属阵营
};

// *************************************************************************** //
// CMessageUpdateLastContacterNotice 场景服务器->网关服务器
// 
// 最后一个联系人通知
// *************************************************************************** //
message CMessageUpdateLastContacterNotice
{
	optional uint32 SenderID		= 		1;   // 消息发送者角色ID
	optional uint32 ReceiverID		= 		2;   // 消息接收者角色ID
};

// *************************************************************************** //
// CMessageUploadContacterList 场景服务器->网关服务器
// 
// 上传最近联系人列表
// *************************************************************************** //
message CMessageUploadContacterList
{
	optional uint32 PlayerID		= 1;	// 玩家角色id
	repeated uint32 ContacterID		= 2;	// 联系人角色id
};

// *************************************************************************** //
// CMessageUploadContacterList 网关服务器->场景服务器
// 
// 将玩家的状态通知给联系人
// *************************************************************************** //
message CMessageContacterStatusNotifyG2S
{
	optional uint32 PlayerID		= 1;	// 玩家角色id
	optional uint32 PlayerStatus	= 2;	// 玩家状态
	optional uint32 ContacterID		= 3;    // 联系人
};

// *************************************************************************** //
// CMessageRebuildSwornRequest
// 重建结拜的消息请求
// *************************************************************************** //
message CMessageRebuildSwornRequest
{

};

// *************************************************************************** //
// CMessageRebuildSwornResponse
// 重建结拜的消息回应
// *************************************************************************** //
message CMessageRebuildSwornResponse
{
	optional uint32	 Errcode = 1;	//错误码
};

// *************************************************************************** //
// CMessageRebuildSwornNotify
// 重建结拜的消息通知
// *************************************************************************** //
message CMessageRebuildSwornNotify
{
	repeated CMsgSwornMember  SwornMember = 1;	// 新的成员列表
	optional string			  SwornName	  = 2;	// 结拜名称
	repeated uint32			  NewMember	  = 3;	// 新人角色ID		
};
