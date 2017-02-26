#ifndef _GATEOBJ_DEFINE_H_
#define _GATEOBJ_DEFINE_H_
#if 0
/******************************************************************************
   版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

******************************************************************************
  文件名          : sceneobj_define.h
  版本号          : 1.0
  作者            : ZHL
  生成日期        : 2008-06-26
  最近修改        : 
  功能描述        : 定义场景服务器各种游戏对象预分配数量
  函数列表        : 
  修改历史        : 
  1 日期          : 2008-06-26
    作者          : ZHL
    修改内容      : 生成
*******************************************************************************/
#endif

// 下面定义服务器负载
#ifdef _DEBUG_
	#define PLAYER_AT_GATE_CAP	100	// 玩家最大人数
	#define SCENE_AT_GATE_CAP   15		// 网关挂几台场景
	#define MAP_AT_GATE_CAP		700		// 网关挂多少地图
	#define TEAM_AT_GATE_CAP	PLAYER_AT_GATE_CAP		// 跨服队伍数量
	#define AGIPSESSION_CAP     100	// 同时向erating发起的session
	#define SERVERSESSION_CAP   1000	// 同时服务器内部seesion
	#define ROLEGLOBALINFO_CAP	50000	// 玩家的数量
	#define MAP_REPETION_CAP	50		// 同时存在副本数量
	#define FAMILY_CAP			80
	#define SERVER_CAP_IBTRANS  (PLAYER_AT_GATE_CAP*2)
	#define SERVER_CAP_CORPS	80
	#define ACTIVITYITEM_CAP 	(PLAYER_AT_GATE_CAP*2)
	#define LEAGUESHOP_CAP		(FAMILY_CAP*20)	
	#define	CORPS_NPC_CAP		100		//军团争夺战NPC最大数量
#else
	#define PLAYER_AT_GATE_CAP	8000	// 玩家最大人数
	#define SCENE_AT_GATE_CAP	25		// 网关挂几台场景
	#define MAP_AT_GATE_CAP		1500	// 网关挂多少地图
	#define TEAM_AT_GATE_CAP	2000
	#define AGIPSESSION_CAP		2000
	#define SERVERSESSION_CAP	4000
	#define ROLEGLOBALINFO_CAP	1000000	// 玩家的数量
	#define MAP_REPETION_CAP	1000 	// 同时存在副本数量
	#define FAMILY_CAP			6000	// 家族的所有数量
	#define SERVER_CAP_IBTRANS  (PLAYER_AT_GATE_CAP*2)
	#define SERVER_CAP_CORPS	3000	 
	#define ACTIVITYITEM_CAP 	(PLAYER_AT_GATE_CAP*2)
	#define LEAGUESHOP_CAP		(FAMILY_CAP*20)	
	#define	CORPS_NPC_CAP		200		//军团争夺战NPC最大数量
#endif // ! _DEBUG_

// 全局对象类型
enum EGlobalObjType
{
	OBJTYPE_START = 0x01,		// start 
	OBJTYPE_PLAYER_AT_GATE,		// CGatePlayer
	OBJTYPE_SCENE_AT_GATE,		// CSceneServer
	OBJTYPE_MAP_AT_GATE,		// CSceneMap
	OBJTYPE_TEAM_AT_GATE,		// CGateTeam
	OBJTYPE_AGIPSESSION,        // CAGIPSession
	OBJTYPE_SERVERSESSION,      // CServerSession
	OBJTYPE_ROLEGLOBALINFO,		// CRoleGlobalInfo
	OBJTYPE_REPETION_MAP,
	OBJTYPE_FAMILY,				// CFamily
	OBJTYPE_IBTRANS,			// CIbTrans
	OBJTYPE_CORPS,				
	OBJTYPE_MEMBER,				// Team member
	OBJTYPE_ACTIVITYITEM,		// CActivityItem
	OBJTYPE_UNION,
	OBJTYPE_LEAGUESHOP,			// CLeagueShopInfo
	OBJTYPE_END,				// end
};

#endif
