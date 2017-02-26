#pragma once

#include "coretype.h"
#include "object.h"

#define PLAYER_DATALEN 60000	// 玩家TLV Buff长度

#define MAX_MESSAGE_ONCE_SEND	1024 // 组播消息时，一次可发送的最多消息个数
#define MAX_ENTITY_ONCE_SEARCH	2000 // 一次从地图上搜索的最多玩家数


// 实体声望
enum EmEntityReputation
{
	REP_FRIENDLY	= 0x0001,	// 友好
	REP_NEUTRALITY	= 0x0002,	// 中立
	REP_HOSTILITY	= 0x0004,	// 敌对

	REP_ALL			= REP_FRIENDLY | REP_NEUTRALITY | REP_HOSTILITY, // 所有

};

//  实体关系
enum EmEntityRelationShip
{
	REL_TEAM		= 0x0100,	// 队伍
	REL_FAMILIY		= 0x0200,	// 家族
	REL_FRIEND		= 0x0400,	// 好友

	REL_ALL			= REL_TEAM | REL_FAMILIY | REL_FRIEND, // 所有
};

// 可见度
enum EmEntityVisiblitiy
{
	VISIBLITY_ALL = 0,	// 所有玩家都可见
	VISIBLITY_SELF,		// 仅自己可见 
	VISIBLITY_TEAM,		// 仅队伍可见
};

// 实体类型组合( 主要为地图模块查找用 )
enum EmCombinedEntityType
{
	// 会动的实体
	COMB_ENTITYTYPE_DYNAMIC_ENTITY	= ( ENTITYTYPE_PLAYER | ENTITYTYPE_PET | ENTITYTYPE_FUNCNPC | ENTITYTYPE_OGRE ), 
	// 不会动的实体
	COMB_ENTITYTYPE_STATIC_ENTITY	= ( ENTITYTYPE_ITEM | ENTITYTYPE_MONEY | ENTITYTYPE_PEDAL ),					
	// 所有类型实体
	COMB_ENTITYTYPE_ALL				= ( COMB_ENTITYTYPE_DYNAMIC_ENTITY | COMB_ENTITYTYPE_STATIC_ENTITY ),		
};


//// 玩家服务状态
//enum EmPlayerServiceStatus
//{
//	PLAYER_DONOTHING= 0,	// 什么也没干
//	PLAYER_STALLING,		// 摆摊中...
//							// .. 其他有意义的状态
//};


// 消息中的模块标志
//enum EmModuleFlagForMessage
//{
//	MESSAGE_PROPERTY	= 0x0002,
//	MESSAGE_MAP			= 0x0003,
//	MESSAGE_TASK		= 0x0004,
//	MESSAGE_CHAT		= 0x0005,
//	MESSAGE_FRIEND		= 0x0006,
//	MESSAGE_TEAM		= 0x0007,
//	MESSAGE_MAIL		= 0x0008,
//	MESSAGE_TRADE		= 0x0009,
//	MESSAGE_STALL		= 0x000A,
//	MESSAGE_ACTIVITY	= 0x000C,
//	MESSAGE_FAMILLY		= 0x000D,
//	MESSAGE_CORPS		= 0x000E,
//};
//

// 客户端连接信息
class CClientInfo
{
public:
	CClientInfo()
	{
		if ( CObj::msCreateMode )
		{
			Initialize( );
		} 
		else
		{
			Resume( );
		}
	};

	~CClientInfo() {};

	int Initialize()
	{
		mUin = 0;
		mClientAddr = 0;
		mClientPort = 0;
		mSockTime = 0;
		mSockIndex = 0;
		mNetSpeed = 0;
		//mVer = 0;
		//memset( mKey, 0, sizeof(mKey) );
		mFlag = 0;
		m_tLastActiveTime = 0;
		return 0;
	}

	int Resume() {return 0;}

	enum  { CRYPT_KEY_LENGTH = 16};

	unsigned int			mUin;					//帐号ID
	unsigned int 			mClientAddr;			//客户端的网络地址
	unsigned short 			mClientPort;			//客户端的端口号
	unsigned int 			mSockTime;				//客户端上线时间
	unsigned short 			mSockIndex;				//客户端的端口号
	int 					mNetSpeed;				//玩家网速
	//short 					mVer;					//C-S协议版本号
	//unsigned char 			mKey[CRYPT_KEY_LENGTH];	//加密使用的Key
	char 					mFlag;					//客户端状态：0 - 未连接，1 - 在线， 2 - 断线
	time_t					m_tLastActiveTime;		// 最后活跃时间
};


