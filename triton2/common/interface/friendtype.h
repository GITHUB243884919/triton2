#ifndef _FRIEND_TYPE_H_
#define _FRIEND_TYPE_H_

#define FRIENDS_LIMIT 220
#define ENEMIES_LIMIT 100
//#define STRANGER_LIMIT 50

#define BLACKLIST_LIMIT 50

#define MAX_APPLY_NUM 10 // 每个玩家最多的同时申请添加好友的条数

#define PLAYER_NAME_SIZE 32
#define GROUP_NAME_SIZE 32 

#define PLAYER_GROUP_COUNT 4

// 最大组名长度
#define GROUP_NAME_LENGTH 32

// 每条消息最多好友个数 <走配置文件>
#define MAX_ONCE_FRIENDS 30

// 同意(被邀请成为好友)
#define AGREE_CODE 1
#define REFUSE_CODE 0

// 每条消息最大长度
#define MAX_FRIEND_MSG_LENGTH 240
#define MAX_FRIEND_MSGBUF_LENGTH 4096

// 每条

enum EFriendModuleErrorID
{
	ERR_INSFRIEND_ON_FULL,
	ERR_INSFRIEND_ON_DECLINE
};


enum EFriendModuleSelectOption
{
	ESELECT_OPTION_IS_ACCEPT,
	ESELECT_OPTION_IS_DECLINE
};

// 我的好友和我的关系 (只能在后面添加！！)
enum EPlayerRels 
{ 
	YOUR_HUSBAND ,	// 丈夫
	YOUR_WIFE ,			// 妻子
	YOUR_BROTHER ,		// 兄弟
	YOUR_TEACHER ,		// 师傅
	YOUR_WATERSHOOT ,    // 徒弟	
	YOUR_FAMILY ,		// 家族
	YOUR_CORPS ,			// 军团
	YOUR_FRIEND, 
	YOUR_ENEMY , 
	YOUR_BLACKLIST , 
	YOUR_STRANGER ,	    // 陌生人
	YOUR_LATEST_CONTACTER, // 最近联系人，非逻辑上的关系种类！
};

#define IS_FRIEND(rel)  (rel<=YOUR_FRIEND) // 是否好友

//玩家状态
enum EPlayerShow 
{ 
	AT_OFFLINE = 0,		// 离线
	AT_ONLINE ,		// 在线
	AT_BUSY,		// 忙碌
	AT_LEAVE,		// 离开
	AT_SUSPEND,		// 挂机
	AT_FIGHT,		// 战斗	
	AT_NOTFAZE,		// 勿扰
	AT_HIDE,		// 隐身
};

enum EKilledTypde 
{ 
	KILLED_OTHER, 
	BE_KILLED_BY_OTHER,
	BY_MANUAL
};

enum EOprFriendCode
{
	ADD_TO_STRANGER,    // 添加到陌生人，有效字段：ID，名字, rel2me
	ADD_TO_FRIEND,		// 添加到好友列表，所有字段都有效
	DEL_FRIEND,			// 删除好友，仅ID有效
	ENEMY_MOVEFIRST,	// 把这个仇敌移动到第一位, ID、杀他次数、被杀次数,时间戳 有效
	ADD_TO_ENEMY,		// 添加仇敌 除了分组 都有效
	DEL_ENEMY,			// 从仇敌列表中删除，仅ID有效
	LOCK_ENEMY,			// 锁定仇敌，ID有效
	DEL_BLACK,			// 删除黑名单 ID,关系有效
	ADD_TO_BLACK,		// 添加黑名单 ID 名字 头像 性别 关系 状态 有效
	UNLOCK_ENEMY,		// 解锁仇敌，ID有效
};

// 删除好友的类型标示
enum EDelFriendType
{
	TYPE_FRIEND ,
	TYPE_BLACK ,
	TYPE_EMEMY ,
	TYPE_DELRELS,	// 解除某种关系
};

// 从GATE取数据的标志
enum enGateFlags
{
	FLAG_GETFRIEND_STATE    = 0x0001,   // 取好友状态
	FLAG_GETFRIEND_ALL      = 0x0002,	 // 取其他信息，包括头像等
	FLAG_GETENEMY_STATE     = 0x0003,
	FLAG_GETENEMY_ALL       = 0x0004,
	FLAG_GETBLACK			= 0x0005,
	FLAG_GETCONTACTER		= 0x0006,
	FLAG_GETCONTACTER_STATE	= 0x0007,
};

// 去网关取charid 的标记 类似session
enum enGetCharIDType
{
	TYPE_ADD_FRIEND,
	TYPE_ADD_BLACK ,
};

// 锁定仇敌 操作类型
enum enLockOprType
{
	LOCK_TYPE_LOCK = 1,  // 锁定
	LOCK_TYPE_FREE = 2,  // 解锁
};

// 减少亲密度的类型
enum EM_EntimacyChangeType
{
	ENTIMACY_CHANGE_TYPE_MODIFY			= 0, // 调整
	ENTIMACY_CHANGE_TYPE_UPDATE			= 1, // 直接更新
	ENTIMACY_CHANGE_TYPE_KILL			= 2, // 因为被杀而减少
	ENTIMACY_CHANGE_TYPE_LEAVESWORN		= 3, // 因为离开结拜而减少
	ENTIMACY_CHANGE_TYPE_TEAM			= 4, // 因为组队改变亲密度
	ENTIMACY_CHANGE_TYPE_DISBANDSWORN	= 5, // 因为结拜解散
	ENTIMACY_CHANGE_TYPE_DIVORCE		= 6, // 因为离婚减少亲密度
	ENTIMACY_CHANGE_TYPE_FLOWER			= 7, // 因为送花改变亲密度
};

// 最大一次处理数量
#define  MAX_ONCE_DISPOS 30

// 升级受祝福的次数最大分段
#define  MAX_FRIENDBLESS_NUM	50

// 最近联系人上限
#define LATEST_CONTACT_MAX 30 

#endif
