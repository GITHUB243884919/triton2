#pragma once
#define MAX_WAR_PLAYER_NUM 40		// 每个战场的最大人数
#define MAX_WAR_REGINFO_NUM 1000	// 最大能够注册的战场信息
#define WAR_START_NUM 40			// 开启战场需要的人数
#define MAX_WARREGINFO_NUM 10000	//  最大的注册信息数量

#define MAX_GLOBALSERVER_NUM 100	// 全局服务器的数量
#define MAX_GLOBALTELEINFO_NUM	1000 // 战场数量
#define MAX_WAITPLAYER_NUM 2000		 // 最大战场等待玩家的数量
#define MAX_LEVLESECTION_NUM 30		 // 最大等级段
#define MAX_OFFLINETIME 60			 // 最大不删除离线时间
#define MAX_WAITTIME 30				// 进入副本的等待时间
#define MAX_GLOBALWAR_APPLYLEVEL 60	// 最大报名等级

#define MAX_PLAYER_INLIST	400		// 每个等级段等待队列的最大人数
#define MAX_WARREGINFO_INLIST 100	// 每个等级段开启战场的最大数量
#define MAX_WARNUM_CANSTART	20		// 报名人数满的战场数目
#define MAX_GLOBAL_REPETION_NUM 10	// 同时开启的全局副本种类数量
#define MAX_GLOBAL_SERVERLIST_NUM 20// 服务器列表的数量
#define MAX_GAMECONFIG_NUM	 40		// 游戏区组的配置信息
#define MAX_GLOBALPLAYER_NUM 4000	// 游戏区组的配置信息
enum EObjType
{
	OBJTYPE_REGINFO = 1,
};

enum EServerType
{
	ESERVERTYPE_DUNGEON			= 0,	// 副本服务器
	ESERVERTYPE_SCENESER		= 1,	// 普通场景服务器	
};

enum EReason_LeaveWar
{
	EREASON_LEAVEWAR_RETURNBACK	=	0,	// 因为返回原来场景而离开战场
	EREASON_LEAVEWAR_OFFLINE	=	1,	// 因为下线或者掉线而离开战场
	EREASON_LEAVEWAR_APPLYFAILED=	2,	// 报名失败
};

enum EReason_EndWar
{
	EREASON_ENDWAR_REPETIONEND	=	0,	// 因为副本结束而结束战场
	EREASON_ENDWAR_BATTLEEND	=	1,	// 战斗结束
};

enum EGlobalTeleType
{
	EGLOBALTELE_TYPE_GLOBALSERVER		 = 0,	// 传送到全局服务器
	EGLOBALTELE_TYPE_RETURNTOGAME		 = 1,	// 返回到原来场景服务器
	EGLOBALTELE_TYPE_RETURNTOGATE		 = 2,	// 返回到原来网关服务器
};


