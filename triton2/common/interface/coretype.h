#pragma once

#ifdef LINUX
#include "object.h"
#endif

#include "base_define.h"
#include <time.h>
#include "errcode.h"
#include <string.h>	 

#define  MONEY_LIMIT		900000000	// 包裹金币上限
#define  ITEM_BUYBACK_MAX	20			// 玩家回购道具数量上限
#define	 BOX_LIMIT			320			// 包裹上限( 旧：180 )
#define  FUNC_LIMIT			1024		// 技能效果数上限
#define  BAGGAGE_BASE		30			// 包裹初始上限
#define	 ITEM_BUFFER_MAX	10240		// 道具字符流长度上限
#define  THROWRANGE			1
#define	 PICKRANGE			2			// 拾取范围
#define  MAXPROLEVEL		10			// 最高生产等级
#define	 ITEMUPLIMIT		10			// 装备最高升级等级
#define	 DAMAGELIMIT		128			// 伤害列表上限
#define  TEAMNUM			6			// 队伍人数上限
#define  TEAM_VALID_DISTANCE 15			// 队员之间有效距离
#define  JEWELNUM			5			// 宝石上限
#define	 PROPERTYNUM		8			// 属性上限
#define  YUANNUM			8			// 元气装备上限
#define  REFRESHTIME		10000		// 恢复时间
#define  CDGROUP			50			// 共用CD组
#define	 PDEAL_LIMIT		64			// 陷阱效果数上限
#define	 MAXLEVELDIFF		99			// 人和怪的最大等级差
#define  MAX_KITBAG_NUM     6	    	// 仓库背包格子的总和
#define  MAX_BAGGAGEKITBAG_NUM 3		// 包裹背包格子的总和
#define  EQUIP_LIMIT		21			// 装备总数
#define	 MAX_STALL_INDEX	24			// 摊位格子数上线
#define  PLAYERNUM			8		    // 每个帐号创建的角色最大人数
#define  SUITIDNUM			150			// 套装编号总数
#define  SUITPOSNUM			14			// 套装位置总数
#define  OGREINDEX			200000		// 怪物索引上限
#define	 KEYLEN				16			// 服务器密钥长度
#define	 MIN_HONOR_LEVEL	30			// 获得功勋最低等级
#define  HONOR_REFRESHTIME	1800		// 功勋获得的时间
#define  MAX_DIPLOID_WAYS	5			// 领双可扩展的最大领取方式数量
#define  PILELIMIT			10			// 能获得物品的最大堆数
#define  UPGVALUENUM		3			// 本体属性上限
#define  ITEMRESLIMIT		6			// 装备抗性升级上限
#define  COMPOSENUM			5			// 多种物品合成上限
#define	 STATENUM			10			// 特殊状态数量
#define  INVITEDLISTNUM		5			// 玩家收到邀请的最大数量
#define  MAXPLAYERNUM		100			// 需要记录日志的玩家最大数量
#define  LOGNAMELENGTH		40			// 日志名称长度
#define  SUBMITTIME			30			// 争夺NPC报名时间(分钟)
#define  MAXFIGHTTIME		60			// 争夺NPC战斗最长时间(分钟)
#define	 NOTIFYTIME			1			// 通知报名间隔(分钟)

#define  MAXKITBAGINDEXNUM	48			// 背包的格子上限


#define  AUTO_PHYSICS_NUM	3			// 自动挂机药品数量

#define  MAXSERVERNUM		6			// 场景服务器的最大数量

// GM标识
#define GM_STATE	0x10000000

#define GM_KICK_MAX		100
#define MAX_DUNGEONSERVER_NUM 6			// 全局战场的数量
#define DEFAULT_REPETION_LINE_ID	( 99 )
#define IS_REPETION_LINE_ID( line )		( ( ( line ) == DEFAULT_REPETION_LINE_ID ) \
	|| ( ( line ) == 98 ) || ( ( line ) == 90) || ( ( line ) == 91) || IS_DUNGEONSERVER_LINE( line ) == true )

#define REPETION_PLAYER_COUNT_MAX	( 400 )
#define MAP_INDEX_MAX		( 200 )		// 玩家身上的副本记录数暂时定为50，等以后副本多了还要增加

#define  IS_DUNGEONSERVER_LINE(line) ( ( line ) <= DUNGEONSERVER_LINE_ID && ( line ) > DUNGEONSERVER_LINE_ID - MAX_DUNGEONSERVER_NUM )	// 是否全局副本线ID
#define  DUNGEONSERVER_LINE_ID	97	// 全局副本线ID
#define  GLOBAL_SCENESERVER_ID 10		// 全局场景的线ID	
#define  IS_GLOBAL_SCENESERVER( line ) ( ( line ) == 10 )




// 中立服务器设置
#define NEUTRAL_LINE_ID				( 50 )
#define IS_NEUTRAL_LINE_ID( line )	( ( line ) == NEUTRAL_LINE_ID )

// 国战服务器设置
#define BATTLE_LINE_ID				( 51 )
#define IS_BATTLE_LINE_ID( line )	( ( line ) == BATTLE_LINE_ID )

// 无效参数宏
#define INVALID_LINE_ID		( 250 )
#define INVALID_MAP_INDEX	( 0 )
#define	INVALID_MAP_ID		( 0 )
#define INVALID_REPETION_INDEX		( 0 )


//enum RESULT_CODE
//{
//	RESULT_AGREE	= 0,
//	RESULT_REFUSE,
//	RESULT_IGNORE,
//};
//
//enum TEAM_UPDATE_TYPE
//{
//	ACTION_JOIN			= 0,
//	ACTION_LEAVE,
//	ACTION_KICK,
//	ACTION_ENDTEAM,
//	ACTION_CHANGEHEADER,
//};

// 角色删除状态
#define DELETEPROTECTTIME  3*3600*24		// 角色删除保护时间	 3*3600*24
#define DELETEALLOWEDTIME  7*3600*24		// 角色可删除时间	 7*3600*24
#define DELETECOOLTIME	   14*3600*24		// 两次删除角色操作的冷却时间   14*3600*24 

#define DELETECOOLTIMESPACE	700*3600*24		// 取消删除等待状态重设删除时间的基础时间段

#define MINLEVELDELETEROLE	0				// 角色能够进入删除等待状态的最小等级
enum EM_RoleDelteStatus
{
	ROLEDELETESTATUS_LIVE				= 0,		// 未删除状态
	ROLEDELETESTATUS_DELETEPROTECT		= 1,		// 删除保护状态
	ROLEDELETESTATUS_DELETEDALLOWED		= 2,		// 删除允许状态
	ROLEDELETESTATUS_DELETECOOLTIME		= 3,		// 删除冷却状态
	ROLEDELETESTATUS_LEVELLOW			= 4,		// 等级低可以直接删除
};


enum CampDef
{
	Zero	= 0,
	One		= 1,
	Two		= 2,
	Three	= 3,
	Four	= 4,
	Five	= 5,
	Six		= 6
};

//#define NAME_LENGTH							32

// 友好：可施放正面技能，不能攻击，可以对话
// 中立：不可施放正面技能，不能攻击，可以对话
// 敌对：可以主动攻击，不能对话
enum PKType
{
	ENEMY_WITH_OGRE		= 0x0001,		// 对怪物仇视（攻击怪物）	
	ENEMY_TO_OGRE		= 0x0002,		// 被怪物仇视（被怪物攻击）
	FRIEND_WITH_SC		= 0x0004,		// 对本国玩家友好
	FRIEND_TO_SC		= 0x0008,		// 本国玩家对其友好
	NEUTRAL_WITH_SC		= 0x0010,		// 对本国玩家中立
	NEUTRAL_TO_SC		= 0x0020,		// 本国玩家对其中立
	ENEMY_WITH_SC		= 0x0040,		// 对本国玩家敌对
	ENEMY_TO_SC			= 0x0080,		// 本国玩家对其敌对
	FRIEND_WITH_FC		= 0x0100,		// 对外国玩家友好
	FRIEND_TO_FC		= 0x0200,		// 外国玩家对其友好
	NEUTRAL_WITH_FC		= 0x0400,		// 对外国玩家中立
	NEUTRAL_TO_FC		= 0x0800,		// 外国玩家对其中立
	ENEMY_WITH_FC		= 0x1000,		// 对外国玩家敌对
	ENEMY_TO_FC			= 0x2000,		// 外国玩家对其敌对
};


#define MAX_ERROR_STR_LEN					128		// 最大错误字符串长度  

#define MAX_DIRECTION 8			// 最大方向数8个
enum EDirType
{
	DIR_DOWN			= 0,
	DIR_LEFTDOWN		= 1,
	DIR_LEFT			= 2,
	DIR_LEFTUP			= 3,
	DIR_UP				= 4,
	DIR_RIGHTUP			= 5,
	DIR_RIGHT			= 6,
	DIR_RIGHTDOWN		= 7,
};

enum EMotionType
{
	MOTION_IDLE			= 0,		// 站立状态
	MOTION_RUN			= 1,		// 奔跑状态
	MOTION_SIT			= 2,		// 打坐状态
};

enum EAliveType
{
	ALIVE_NORMAL		= 0,		// 正常状态(生存)
	ALIVE_BATTLE		= 1,		// 战斗状态(生存)
	ALIVE_DIE			= 2,		// 死亡状态
	ALIVE_GHOST			= 3,		// 灵魂状态
	ALIVE_CLEAR			= 4,		// 待销毁状态
};

enum EGenderType
{
	GENDER_MALE			= 2,		// 男性
	GENDER_FEMALE		= 3,		// 女性
};

#define  MAX_DIPLOIDSTATE	7		// 领双状态参数
enum EDiploidState
{
	DIPLOID_STATE		= 0,
	FREEZE_STATE		= 1,
	TIMEID_STATE		= 2,
	TIMERSTOP_STATE		= 3,
	LEFTVALID_STATE		= 4,
	TEMP_STATE			= 5,	// 以备另需
	ACTIVITY_STATE		= 6,	// 活动增加的双倍倍率,按照10000为单位计算
};

// 实体类型

enum EEntityType
{
	ENTITYTYPE_NONE			= 0x0000,		// 无
	ENTITYTYPE_PLAYER		= 0x0001,		// 玩家
	ENTITYTYPE_OGRE			= 0x0002,		// ogre怪物
	ENTITYTYPE_FUNCNPC		= 0x0004,		// 功能 NPC
	ENTITYTYPE_PEDAL		= 0x0008,		// 陷阱、触发器
	ENTITYTYPE_ITEM			= 0x0010,		// 物品	
	ENTITYTYPE_COLLECT		= 0x0020,		// 采集NPC
	ENTITYTYPE_MONEY		= 0x0040,		// 金币
	ENTITYTYPE_PET			= 0x0080,		// 宠物
	ENTITYTYPE_TASKNPC		= 0x0100,		// 任务NPC	
	ENTITYTYPE_BOX			= 0x0200,		// Store Box
	ENTITYTYPE_REDSTONE		= 0x0400,		// 内丹物品
	ENTITYTYPE_BLOCK		= 0x0800,		// 阻挡
	ENTITYTYPE_MAGICWEAPON	= 0x1000,		// 法宝
	ENTITYTYPE_GHOST		= 0x2000,		// 可穿越实体
};



enum EVersion
{
	VERSION				= 1005,		// 版本号
};

enum EMetier
{
	METIER_NONE		= 0,			// 任意职业
	METIER_WARRIOR	= 1,			// 战士
	METIER_ROGUE	= 2,			// 剑客
	METIER_HUNTER	= 3,			// 猎人
	METIER_SORCERER = 4,			// 法师
	METIER_PRIEST	= 5,			// 道士
	METIER_DRUID	= 6,			// 幻术师
	METIER_NOVICE	= 7,			// 新手职业
};

enum EMetierBlock
{
	BLOCK_NONE		= 0,				// 任意职业
	BLOCK_WARRIOR	= 0x0001,			// 战士
	BLOCK_ROGUE		= 0x0002,			// 剑客
	BLOCK_HUNTER	= 0x0004,			// 猎人
	BLOCK_SORCERER  = 0x0008,			// 法师
	BLOCK_PRIEST	= 0x0010,			// 道士
	BLOCK_DRUID		= 0x0020,			// 幻术师
	BLOCK_NOVICE	= 0x0040,			// 新手职业
};

enum EDropType
{
	DROP_OBJECTIVE	= 0,		// 被动掉落
	DROP_SUBJECTIVE = 1,		// 主动掉落
};

// ********************************************************************** //
// CCharProfile
// ********************************************************************** //

class CCharProfile
{
public:
	unsigned int	mCharID;					// 角色ID
	unsigned int	mAccountID;					// 帐号ID
	char			mCharName[ NAME_LENGTH ];	// 角色姓名
	unsigned char	mSex;						// 角色性别
	unsigned char	mMetier;					// 角色职业
	unsigned char	mNationality;				// 角色国籍
	unsigned char	mLevel;						// 角色等级
	unsigned short	mHairType;					// 角色发型
	unsigned short	mFaceType;					// 角色脸型
	unsigned short	mLineID;					// 所在线ID
	unsigned short	mMapID;						// 所在地图ID
	unsigned short	mIndex;
	int				mHairColour;				// 角色⑸?
	int				mExp;						// 角色经验
	int				mMoney;						// 角色金钱
	int				mPosX;						// 角色位置
	int				mPosY;
	int				mStatus;					// 存活状态
	int				mOnlineTime;				// 角色在线时间
	time_t			mLastLogin;					// 角色上次登陆时间

public:


	CCharProfile( )
	{
#ifdef LINUX
		if ( CObj::msCreateMode )
		{
			Initailize();
		} 
		else
		{
			Resume();
		}
#else
		Initailize();
#endif
	}

	int Initailize( )
	{
		mCharID = 0;
		mAccountID = 0;
		mSex = 1;
		mExp = 0;
		mMetier = 0;
		mNationality = 0;
		mLevel = 0;
		mLineID = 0;
		mMapID = 0;
		mOnlineTime = 0;
		mLastLogin = 0;
		::memset( mCharName, 0, sizeof( mCharName ) ); 
		mMoney = 0;
		mPosX = 0;
		mPosY = 0;
		mStatus = 0;
		return 0;
	}

	int Resume( )
	{
		return 0;
	}



public:
	~CCharProfile( )
	{
	}
};


typedef struct OBJGUID 
{
	union {
		struct {
			time_t			tmstmp;			// 创建时的系统时间
			unsigned short  index;			// 创建时的递增编号
			unsigned short	where;			// 创建的GateID, ServerID
		}st; 

		unsigned long long  guid;			// 
	}a;

	OBJGUID() { a.guid = 0; }

	OBJGUID(unsigned short nIndex, unsigned short nGateID, unsigned char nServerID)
	{
		a.st.tmstmp = time(0);
		a.st.index = nIndex;
		a.st.where = (nGateID << 4) + nServerID;
	}

	unsigned long long value() { return a.guid; }
}OBJGUID;

enum REDIRECT_ERR
{
	REDIRECT_PLAYER_IN_MAP		= 1,
	REDIRECT_OBJECT_IS_FULL		= 2,
	REDRIECT_ERR_UNKNOW			= 3,
	REDIRECT_MAP_NOTOPEN		= 4,
	REDIRECT_MAP_PLAYER_IS_FULL = 5,
};

enum TeamUpdateType
{
	emMemberJoin				= 6,
	emMemberQuit				= 7,
	emMemberTick				= 8,
	emTeamEnd					= 9,
	emChangeHead				= 10,
};

enum LSStateType
{
	LSSTATE_NONE			= 0,
	LSSTATE_CHARPROFILE		= 0x00000001,
	LSSTATE_PROPERTY		= 0x00000002,
	LSSTATE_BAGGAGE			= 0x00000004,
	LSSTATE_STORAGE			= 0x00000008,
	LSSTATE_TASK			= 0x00000010,
	LSSTATE_FRIEND			= 0x00000020,
	LSSTATE_REPETION		= 0x00000040,
	LSSTATE_YUANBAO			= 0x00000080,
    LSSTATE_YAOQIANSHU1     = 0x00000100,
    LSSTATE_YAOQIANSHU2     = 0x00000200,

	LSSTATE_TOTAL_COMMPLTE	=	LSSTATE_CHARPROFILE + \
								LSSTATE_PROPERTY + \
								LSSTATE_BAGGAGE + \
								LSSTATE_STORAGE + \
								LSSTATE_TASK + \
								LSSTATE_FRIEND +\
								LSSTATE_REPETION +\
								LSSTATE_YUANBAO + \
                                LSSTATE_YAOQIANSHU1 + \
                                LSSTATE_YAOQIANSHU2,
};

/*	
*	定义常用返回值
*/

#define LK_ISUCCESS			1 
#define LK_IERROR			0
#define LK_NULL				NULL
#define LK_TRUE				true
#define LK_FALSE			false
#define LK_ZERO				0
#define LK_BINDMONEY		0
#define LK_NOTBIND			1
#define LK_ONE				1

enum EM_HONETSHOLE_SCORE 
{	 
	HONETSHOLE_SCORE_REPTIONLEVEL = 1,		// 副本等级
	HONETSHOLE_SCORE_OGRENUM	  = 2,		// 怪物总数
	HONETSHOLE_SCORE_TOTALTIME	  = 3,		// 通关时间
	HONETSHOLE_SCORE_GENERALSCORE = 4,		// 评分
	HONETSHOLE_SCORE_EXTRALSCORE  = 5,		// 额外评分
	HONETSHOLE_SCORE_TOTALSCORE	  = 6,		// 总评分							 
};

enum REPETION_EVENT_NOTICE						   
{
	REPETION_NONE,								// 无事件
	REPETION_CLOSE,								// 副本关闭, 参数1 多长时间后结素
												// 大逃杀  : 参数1 = 人数不够多长时间结束，参数2 = 1
												//			参数1 = 正常多长时间结束，参数2 = 2
	REPETION_END,								// 副本结束，参数1 ( 0失败， 1成功 )
	REPETION_CREATE_NPC,						// 创建NPC， 参数1 ( npc TemplateID ), 参数2( 时间 )
	REPETION_KILL,								// NPC死亡,	 参数1 杀死NPC的EntityID, 参数2 NPC模版ID
	REPETION_COUNTDOWN,							// 副本倒计时, 不同的副本倒计时不同, 参数1为同一副本中的不同倒计时
												// 龙宫: 1 房间通过倒计时(参数1 = 1, 参数2 = 时间), 
												//		2 处决倒计时( 参数1 = 2, 参数2 = 时间 )
												//		3 龙宫神秘房间( 参数1 = 3, 参数2 = 时间 )
												// 花果山战场 : 1 开始倒计时( 参数1 = 2， 参数2 = 时间 )
												//				2 第二阶段倒计时( 参数1 = 1， 参数2 = 时间 )
												//
												// 大雁塔:	1 清空连击点倒记时( 参数1 = 1, 参数2 = 时间 )
												// 品仙桃	1 清空连击点( 参数1 = 1, 参数2 = 时间 )
												// 腾云大战 1  金角大王复活时间( 参数1 = 1， 参数2 = 时间 )
												//			2 银角大王复活时间( 参数1 = 2， 参数2 = 时间 )
												// 			3	灵烟阁大人复活时间( 参数1 = 3, 参数2  = 时间  )
												//			4  灵烟阁夫人复活时间( 参数1 = 4, 参数2 = 时间 )
												//			5 副本结束时间( 参数1=5, 参数2= 时间 )
												// 御花园	1 春园时间( 参数1 = 1， 参数2 = 2 )
												// 云霄塔   1 下层开启时间( 参数1 = 1 )
												// 大逃杀   1 pk开始倒计时（参数1 = 1， 参数2 = 时间）
												//			2 pk结束倒计时（参数1 = 2， 参数2 = 时间）
												//			3 创建火焰倒计时（参数1 = 3， 参数2 = 时间）
												//			4 a阶段结束倒计时（参数1 = 4， 参数2 = 时间）
												//          5 b阶段开始倒计时(参数1 = 5， 参数2 = 时间)
												// 无底洞 	1 第四层怪物出现倒计时
												//			2 第四层限时倒记录时间
												// 跨服战场:	1.表示战场开启倒计时 （参数1 = 1， 参数2 = 时间）
												//				2.魔法门关闭倒计时 (参数1=2，参数2=时间)												//				
	REPETION_COUNTDOWN_END,						// 副本倒计时结束, 参数0 副本关闭到计时
												// 副本倒计时结束， 参数1的意义同REPETION_COUNTDOWN
	REPETION_TAKE_COUNT,						// 副本计数. 不同的副本计数的意义不同
												// 蟠桃: 	1 连击点( 参数1 = 1, 参数2 = 连击点数 )
												// 			2 石猴数量( 参数1 = 2， 参数2 = 数量 )
												//			3 猴妖数量( 参数1 = 3， 参数2 = 数量 )
												//			4 评分( 参数1 = 4， 参数2 = 评分 )
												//			5 副本等级( 参数1 = 5 参数2 = 等级 )
												//			6 杀死石猴数量( 参数1 = 6, 参数2 = 数量 )
												//			7 下下评价次数( 参数1 = 7, 参数2 = 数量 )
												//			8 下评价次数( 参数1 = 8, 参数2 = 数量 )
												//			9 中下评价次数( 参数1 = 9, 参数2 = 数量 )
												//			10 中评价次数( 参数1 = 10, 参数2 = 数量 )
												//			11 中上评价次数( 参数1 = 11, 参数2 = 数量 )
												//			12 上评价次数( 参数1 = 12, 参数2 = 数量 )
												//			13 上上评价次数( 参数1 = 13, 参数2 = 数量 )
												//			14 神评价次数( 参数1 = 14, 参数2 = 数量 )
												// 龙宫:	1 评分( 参数1 = 1, 参数2 =  总评分 )
												//			2 等级( 参数1 = 2, 参数2 = 等级 )
												//			3 时间( 参数1 = 3， 参数2 = 时间 )
												//			4 额外评分( 参数1 = 4, 参数2 = 额外评分 )
												//			5 杀怪总数( 参数1 = 5, 参数2 = 杀怪总数 )
												// 大雁塔	1 连击点(  参数1 = 1, 参数2 = 连击点数 )
												//			2 评分( 参数1 = 2, 参数2 = 评分)
												//			3 剩余时间*( 参数1 = 3, 参数2=时间 )
												//			4 等级( 参数1 = 4, 参数2 = 等级 )
												//			5 杀怪总数( 参数1 = 5, 参数2 = 总数 )
												//			6 下下评价次数( 参数1 = 6, 参数2 = 数量 )
												//			7 下评价次数( 参数1 = 7, 参数2 = 数量 )
												//			8 中下评价次数( 参数1 = 8, 参数2 = 数量 )
												//			9 中评价次数( 参数1 = 9, 参数2 = 数量 )
												//			10 中上评价次数( 参数1 = 10, 参数2 = 数量 )
												//			11 上评价次数( 参数1 = 11, 参数2 = 数量 )
												//			12 上上评价次数( 参数1 = 12, 参数2 = 数量 )
												//			13 神评价次数( 参数1 = 13, 参数2 = 数量 )
												//			14 获得金钱( 参数1 = 14, 参数2 = 金钱 )
												// 品仙桃	1 连击点 ( 参数1 = 1, 参数2 = 连击点 )
												// 			2 结束( 参数1=2, 参数2=方式(0不成功, 1成功))
												//			3 最大连击点( 参数1=3, 参数2=数量)
												//			4 时间( 参数1= 4, 参数2=时间)
												//			5 阶段( 参数1=5， 参数2=阶段)
												//			6 杀怪总数量( 参数1=6, 参数2= 总数量 )
												//			7 等级( 参数1= 7， 参数2=等级)
												//			8 新大波开始( 参数1=8, 参数2=波数)
												//			9 机关出现( 参数1=9 )
												// 黄蜂洞	参数查看枚举EM_HONETSHOLE_SCORE中的值，参数2表示对应的数值
												// 腾云大战	1 杀怪数量 ( 参数1 = 1 ， 参数2 = 数量 )
												// 			2 消耗时间( 参数1=2, 参数2=时间)
												//			3 副本等级( 参数1=3， 参数2=等级 )
												//			4 金角银角复活次数( 参数1=4， 参数2=次数)
												//			5 灵烟阁大人夫人复活次数( 参数1=5， 参数2=次数)
												//			6 积分( 参数1=6， 参数2=积分);
												// 云霄塔   1 积分( 参数1 = 1, 参数2 = 积分 )
												//			2 最高层数( 参数1 = 2. 参数2 = 层数 )
												//			3 杀怪总数( 参数1 = 3. 参数2 = 数量 )
												// 大逃杀:	1 评分( 参数1 = 1, 参数2 =  总评分 )
												//			2 等级( 参数1 = 2, 参数2 = 等级 )
												//			3 存活时间( 参数1 = 3， 参数2 = 时间 )
												//			4 杀人总数( 参数1 = 4, 参数2 = 杀人总数 )
												//			5 杀人积分( 参数1 = 5, 参数2 = 杀人积分 )
												//			6 存活时间积分( 参数1 = 6, 参数2 = 存活时间积分 )
												//			7 场内存活人数（参数1 = 7， 参数2 =  存活人数）
												//			8 场内最高杀人数（参数1 = 8， 参数2 = 最高杀人数）
												//          9 获得经验（参数1 = 9，参数2 = 获得经验值）
												//			10退出副本形式（参数1 = 10， 参数2 =1 被火焰杀死）
												//										 参数2 =2 被人杀死
												//			11 存活时间计时开始（参数1 = 11， 参数2 = 时间）
												//			12 存活时间暂停（参数1 = 12 ，参数2 = 时间）
												//			13 当前连斩数( 参数 1= 13， 参数2=杀人数 )
												//			14 副本排名（参数 1 = 14， 参数2 = 排名 ）
												//			15 副本最高积分（参数 1 = 15， 参数2 = 积分）
												//			16 最高连斩数( 参数 1= 16， 参数2 = 最高连斩数)
												// 跨服战场	1. 个人积分(参数1=1，参数2=个人积分)
												//			2. 团队积分(参数1=2，参数2=团队积分)
												//			3. 总积分(参数1=3，参数2=总积分)												//			
												//			4. 击杀人数(参数1=4，参数2=击杀人数)
												//			5. 击杀天兵数(参数1=5，参数2=击杀天兵数)
												// 无底洞:	1 第三关剩余小怪( 参数1 = 1 )
	REPETION_RESOURCE,							// 家族战，参数1 攻方资源 参数2 守方资源
	REPETION_GET,								// 家族战，参数1 占领的CampID， 参数2 占领的Index	
	REPETION_CORPS_FLAG,						// 军团战大旗归属方,  参数1 大旗占领方的阵营ID,参数2 自己阵营ID,参数3 大旗归属军团名称
	REPETION_CORPS_RESOURCE,					// 军团战，参数1 攻方资源 参数2 守方资源
	REPETION_CORPS_OGRE,						// 军团战圣火台
	REPETION_RELIVE,							// 复活， 参数1 剩余复活时间	参数2 总共复活时间
	REPETION_FLAGTEMP,							// 军团大旗模板ID
	REPETION_MSG,								// 龙宫: 1 神秘房间开启( 参数1 =1 )
												// 家族战 参数1 = 0 通知资源上限
												// 黄蜂岭: 参数 1 = 0 表示火把B 等于 1 表示火把C 等于 2 表示停止计时, 参数2表示火把持续时间
												//	腾云大战:
												// 			1 BOSS出现( 参数1=1, 参数2=1( 二郎神 ), 2( 六耳猕猴 ),
												//									3( 哮天犬 ), 4( 巨灵神 ),
												//									5( 牛魔王 ), 6( 碧水精金兽 )
												//									7( 黄风大王 ), 8( 黑龙王 )
												//									9( 黑龙真身	), 10( 金角大王 )
												//									11( 银角大王 ), 12( 灵烟阁大人 )
												//									13( 灵烟阁夫人 )
												//			2 金角死亡银角未死( 参数1=2, 参数2=时间 )
												//			3 银角死亡金角未死( 参数1=3， 参数2=时间 )
												//			4 大人死亡夫人未死( 参数1=4， 参数2=时间)
												//			5 夫人死亡大人未死( 参数1=5， 参数2=时间 )
												//	御花园:
												//		1 获得姻缘	( 参数1 = 1 )
												//		2 姻缘溜走	( 参数1 = 2 )
												//		3 结束时候剩余姻缘树( 参数1 = 3, 参数2 = 数量 )
												//	云霄塔/云霄塔60层(五行幻境)
												//		1 当前楼层层数 ( 参数1 = 1 )
												//	无底洞
												//		1 第一层老鼠精头目苏醒( 参数1 = 1， 参数2 = 72914( 红 ), 72915( 
												//				蓝 ), 72916( 黄 )
												//		2 第一层红挑水怪( 参数1 = 2 )
												//		3 第一层蓝挑水怪( 参数1 = 3 )
												//		4 第一层黄挑水怪( 参数1 = 4 )
												//		5 第一层红挑水怪死( 参数1 = 5 )
												//		6 第一层蓝挑水怪死( 参数1 = 6 )
												//		7 第一层黄挑水怪死( 参数1 = 7 )
												//		8 第一层御厨生( 参数1 = 8 )
												//		9 第二层俾女死( 参数1 = 9 )
												//		10第三层玩家变老鼠( 参数1 = 10， 参数2 = 玩家ID )
												// 		11第三层怪变老鼠( 参数1 = 11 )
												//		12第四层排位出现( 参数1 = 12 )
												//		13第四层怪物出现( 参数1 = 13 )
												//		14 副本等级( 参数1 = 14 )
												//		15 副本时间( 参数1 = 15 )
												//		16 副本杀怪总数( 参数1 = 16 )
												//		17 副本评分( 参数1 = 17 )
												//		18 奖励计分( 参数1 = 18 )
												//		19 第三关BOSS出现( 参数1 = 19 )
												// 		20 裁判出现( 参数1 = 20 )
												//		21 地图目标( 参数1 = 21, 参数2 = 当前地图ID  )
												//		22 水缸破( 参数1 = 22 )
												//	跨服战场 1.己方战场人数（参数1=0 ， 参数2=战场人数）
												//			 2.敌方战场人数 (参数1=1 ， 参数2=战场人数）
												//			 3.击杀敌人数(参数1=2 ， 参数2=杀死的人数）												
												//			 4.己方阵营获得的积分数(参数1=3 ， 参数2=积分数）
												//			 5.复活多少次(参数1=4 ， 参数2=复活次数）
												//			 6.杀死天兵数(参数1=5 ， 参数2=天兵数）
												//			 7.敌方阵营积分(参数1=6 ， 参数2=天兵数）
												//			 8.开兵符(参数1=7 ， 参数2=(0表示己方正营1表示敌方正营） 参数3=开兵符的角色名称
												//			 10.佛祖死亡通知(参数1=8 ， 参数2=(0表示己方正营1表示敌方正营）参数3=杀死boss的玩家名
												//			 11.本方接引佛祖(参数1=9 ， 参数2=(0表示存活1表示死亡）
												//			 12.敌方接引佛祖(参数1=10 ，参数2=(0表示存活1表示死亡）
												//			 13.本方天兵数量(参数1=11 ，参数2=(表示己方天兵数量)
												//			 13.敌方天兵数量(参数1=12 ，参数2=(表示敌方天兵数量)
	REPETION_UPDATETIME,						// 同步复活时间,参数1 阵营ID 参数2 复活总时间
	REPETION_BATTLEMAP,							// 军团战争夺地图， 参数1=争夺的地图ID
	REPETION_SHOW_VALUE,						// 云霄塔60层:	1 积分( 参数1 = 1, 参数2 = 积分 )
												//				2 最高层数( 参数1 = 2. 参数2 = 层数 )
												//				3 杀怪总数( 参数1 = 3. 参数2 = 数量 )

};


enum RES_TYPE
{
	RES_NONE	= 0,			// 无抗性
	RES_TYPE1	= 1,
	RES_TYPE2	= 2,
	RES_TYPE3	= 3,
	RES_TYPE4	= 4,
	RES_TYPE5	= 5,
	RES_TYPE6	= 6,
	RES_NUM		= 7,	
};

enum UPG_TYPE
{
	UPG_PROPERTY = 0,
	UPG_RESDEF	 = 1,
	UPG_RESATT	 = 2,
};

enum PEDAL_TYPE
{
	PEDAL_TELEPORT		= 0,		// 传送点踏板
	PEDAL_TRAP,						// 陷阱踏板
	PEDAL_HIEROGRAM,				// 神符踏板
	PEDAL_NEEDFIRE,                 // 火焰跳板
};

enum CAMP_TYPE
{
	CAMP_1 = 1,
	CAMP_2 = 2,
};

// 军团战城池副本数据
enum CORPS_DATA
{
	DATA_CAMP1_CORPS = 0,		// 阵营1对应的军团ID
	DATA_CAMP2_CORPS,		// 阵营2对应的军团ID
	DATA_FLAG_OWNER,
	DATA_BATTLE_CHALLEGE,
	DATA_BATTLE_SCRABBLE,
};

// 莲花洞副本 评价数值类型枚举
enum enLianhuadongMarkType
{
	MARKTYPE_FB_LEVEL	=	1,	// 副本等级
	MARKTYPE_SAVE_NUM,		// 解救人质数
	MARKTYPE_KILLNUM,		// 杀怪总数
	MARKTYPE_TIME,			// 通关时间（秒）
	MARKTYPE_MARK,			// 评分
	MARKTYPE_EXTRA_MARK,	// 额外评分
};


// 副本MapID定义
#define _REPE_LONGGONG_MAP_ID_ 					( 14 )
#define _REPE_LONGGONG_EASY_MAP_ID_ 			( 58 )
#define _REPE_LONGGONG_HARD_MAP_ID_ 			( 59 )
#define _REPE_PANTAO_MAP_ID_ 					( 21 )
#define _REPE_PANTAO_EASY_MAP_ID_ 				( 56 )
#define _REPE_PANTAO_HARD_MAP_ID_ 				( 57 )
#define _REPE_DAYANTA_MAP_ID_ 					( 81 )
#define _REPE_FAMILY_TASTE_MAP_ID_ 				( 94 )		// 品仙桃
#define _REPE_FAMILY_TASTE_DIE_MAP_ID_          ( 94 )      // 品仙桃地府
#define _REPE_LIANHUADONG_MAP_ID_ 				( 92 )
#define _REPE_HUANGFENGDONG_MAP_ID_ 			( 91 )
#define _REPE_HUAGUOSHAN_MAP_ID_ 				( 55 )		// 大战花果山地图ID
#define _REPE_FAMILY_MAP_ID_					( 66 )		// 家族战地图
#define _REPE_CORPS_MAP_ID_						( 71 )		// 军团战地图
#define _REPE_FLY_WAR_MAP_ID_					( 80 )		// 飞行空战地图
#define _REPE_GRAND_MAP_ID_						( 99 )		// 御花园
#define _REPE_TOWER_MAP_ID_						( 119 )		// 通天塔
#define _REPE_WUDITON_MAP_ID_					( 144 )		// 无底洞
#define _REPE_DALUANDOU_MAP_ID_                 ( 165)     // 大乱斗
#define _REPE_SHITU_MAP_ID_						( 160 )

// 副本索引
enum
{
	_REPE_INDEX_START_			 = 1, // 副本起始索引从1开始，不是0 

	_REPE_GUANGHANGONG_INDEX_    = 1,	// 广寒宫幻境
    _REPE_PUTUO_INDEX_           = 2,	// 普陀山幻境
    _REPE_WUZHUANGGUAN_INDEX_    = 3,	// 五庄观幻境
    _REPE_SHITUOLING_INDEX_      = 4,	// 狮驼岭幻境
    _REPE_LINEYANGE_INDEX_       = 5,	// 凌烟阁幻境
    _REPE_TIANWANG_INDEX_        = 6,	// 天王殿幻境
    _REPE_LONGGONG_INDEX_        = 7,	// 龙宫
    _REPE_LONGGONG_EASY_INDEX_   = 8,	// 简易龙宫
    _REPE_LONGGONG_HARD_INDEX_   = 9,	// 精英龙宫
    _REPE_PANTAO_INDEX_          = 10,	// 蟠桃
    _REPE_PANTAO_EASY_INDEX_     = 11,	// 简易蟠桃
    _REPE_PANTAO_HARD_INDEX_     = 12,	// 精英蟠桃
    _REPE_HUAGUOSHAN_INDEX_      = 13,	// 花果山
    _REPE_CHANGBAOTU_INDEX_      = 14,	// 藏宝图副本
    _REPE_CORPS_BATTLE_INDEX_    = 15,	// 军团战
    _REPE_FAMILY_INDEX_          = 16,	// 家族战
    _REPE_DAYANTA_INDEX_         = 17,	// 大雁塔
    _REPE_LIANHUADONG_INDEX_     = 18,	// 莲花洞
    _REPE_HORNETSHOLE_INDEX_     = 19,	// 黄风洞
    _REPE_FAMILY_TASTE_INDEX_    = 20,	// 家族品仙桃
	_REPE_CORPS_CHALLEGE_INDEX_	 = 21,	// 军团挑战赛
	_REPE_FAMILY_CHALLENGE_INDEX_= 22,	// 家族挑战赛
	_REPE_FLY_WAR				 = 23,	// 腾云大战
	_REPE_GRAND_INDEX_			 = 24,	// 御花园
	_REPE_TOWNER_INDEX_			 = 25,	// 通天塔
	_REPE_LASTONE_INDEX_		 = 26,	// 大逃杀
	_REPE_LASTONE_TEAM_INDEX_	 = 27,	// 大逃杀组队
	_REPE_WUDIDONG_INDEX_		 = 28,	// 无底洞
	_REPE_SHITU_INDEX_			 = 29,  // 师徒副本
	_REPE_GLOBALWAR_INDEX_		 = 30,  // 全局战场副本
	_REPE_CORPSBOSS_INDEX_		 = 31,  // 军团boss副本
	_REPE_NEWTOWER_INDEX_		 = 32,	// 云霄塔60层

	_REPE_MAX_COUNT_,
};



// ping包时间定义
#define _PLAYER_PING_TIME_MAX_					( 10 * 1000 )

// 阵营列表
enum EServerList
{
	EM_JUXIAN	= 1,	// 聚仙
	EM_QIANKUN	= 2,	// 乾坤
	EM_PUTI		= 3,	// 菩提
	EM_HUNYUAN	= 4,	// 混元
};


// 2级密码的长度
#define  MAX_PASSWORD_LENGTH 32


// 安全密码的状态
enum EPasswordStatus
{
	EM_PASSWORD_NOTSET		=	1,	// 未设置状态
	EM_PASSWORD_PROTECTTIME	=	2,	// 安全保护时间
	EM_PASSWORD_PASSLOCKED	=	3,	// 密码保护状态
	EM_PASSWORD_PASSFREE	=	4,	// 密码验证已经通过的状态
};

// 安全密码影响的操作
enum EPassWordAction 
{
	EM_PASSWORD_ACTION_SETPASSWORD = 0x0001,	// 设置密码
	EM_PASSWORD_ACTION_GAMEACTION  = 0x0002,	// 游戏操作
};

// 安全保护时间的最小值
#define  MIN_PASSWORD_PROTECTIME 60		// 一分钟

// 安全保护时间的最大值
#define  MAX_PASSWORD_PROTECTIME 3600   // 一小时

// 安全保护时间的默认时间
#define	  DEFAULT_PASSWORD_PROTECTTIME 180 // 三分钟


// bourse
#define BOURSE_DB_YB_KEY		( 1 )
#define	BOURSE_DB_MONEY_KEY		( 2 )

#define BOURSE_DB_THREAD_KEY	( 5 )


// 客户端控制文件 参数3 长度
#define CLI_CTRL_PARM3_LEN		256


// 100421 MAOCY ADD-BEGIN
//============================================================
// <T>网关服务器标志。</T>
//============================================================
enum EServerGatewayFlag{
   EServerGatewayFlag_Normal,    // 普通
   EServerGatewayFlag_Recommend, // 推荐
};

//============================================================
// <T>网关服务器状态。</T>
//============================================================
enum EServerGatewayStatus{
   EServerGatewayStatus_Unknown, // 未知(K)
   EServerGatewayStatus_Update,  // 维护(U)
   EServerGatewayStatus_Full,    // 火爆(F)
   EServerGatewayStatus_Rush,    // 繁忙(R)
   EServerGatewayStatus_Busy,    // 忙碌(B)
   EServerGatewayStatus_Idle,    // 空闲(I)
};
// 100421 MAOCY ADD-END


//元宝摆摊
enum EStallType{
	EM_STALL_TYPE_ALL = 0,  //所有
	EM_STALL_TYPE_MONEY = 1, //非绑金摆摊
	EM_STALL_TYPE_YB = 2, //元宝摆摊
};

enum EMIbstoreLimit
{
        EM_LIMIT_BY_LIFECYCLE   = 0,	// 角色限购
        EM_LIMIT_BY_HOUR        = 1,	// 角色每小时限购
        EM_LIMIT_BY_6HOUR       = 2,	// 角色每6小时限购
        EM_LIMIT_BY_DAY         = 3,	// 角色每日限购
        EM_LIMIT_BY_WEEK        = 4,	// 角色每周限购 (0-6)
        EM_LIMIT_BY_MONTH       = 5,	// 角色每月限购
        EM_LIMIT_BY_YEAR        = 6,	// 角色每年限购
        EM_LIMIT_BY_LEVEL       = 7,	// 角色每等级限购
        EM_LIMIT_BY_5LEVEL      = 8,	// 角色每5级限购
        EM_LIMIT_BY_10LEVEL     = 9,	// 角色每10级限购
}; 

#define BIND_MONEY 1
#define UNBIND_MONEY  2
#define LINEKONG_MONEY   3
#define SILVER_YUANBAO   4
#define GOLDEN_YUANBAO   5
#define COMBAT_FAME		 6
#define REPE_FAME		 7
#define COMMON_FAME		 8


//功能付费的功能项，暂时就这几个
enum EFuncConsumeType
{
	RELIVE_BY_FUNCCONSUME			= 100000,	//复活
	DEXP_BY_FUNCCONSUME				= 100001,	//双倍经验
	BLACKPILLSTIME_BY_FUNCCONSUME	= 100002,	//乾坤丸（黑狗）
	VIPTIME_BY_FUNCCONSUME			= 100003,	//VIP时间
	BAGTIME12_BY_FUNCCONSUME		= 100004,	//12格包裹时间
	BAGTIME14_BY_FUNCCONSUME		= 100005,	//14格包裹时间
	BAGTIME16_BY_FUNCCONSUME		= 100006,	//16格包裹时间
	BAGTIME18_BY_FUNCCONSUME		= 100007,	//18格包裹时间
	PROTECTTIME_BY_FUNCCONSUME		= 100008,	//护法金丹buff
};
//功能付费 类型
enum{
	FUNC_BIND_MONEY				= BIND_MONEY,		////绑定金钱消费
	FUNC_UNBIND_MONEY			= UNBIND_MONEY,		//金钱消费
	FUNC_LINEKONG_MONEY			= LINEKONG_MONEY,	//? 蓝港币？
	FUNC_SILVER_YB				= SILVER_YUANBAO,	//绑定元宝消费
	FUNC_GOLDED_YB				= GOLDEN_YUANBAO,	//元宝消费
	FUNC_CONSUME_COUNT			= 5,
};

enum EFuncUnitType
{
	FUNC_UNIT_ONCE		= 1,	// 一次
	FUNC_UNIT_H			= 2,	// 小时
	FUNC_UNIT_D			= 3,	// 天
};

enum ERepetionIndexType
{
	REPETION_INDEX_TYPE_COMMON		= 0,	// 普通副本可以进入
	REPETION_INDEX_TYPE_RELOGIN		= 1,	// 可以重新进入的副本通知
	REPETION_INDEX_TYPE_ENDRELOGIN	= 2,	// 可重入副本关闭
};
