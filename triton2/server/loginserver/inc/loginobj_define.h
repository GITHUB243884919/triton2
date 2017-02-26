#ifndef _LOGINOBJ_DEFINE_H_
#define _LOGINOBJ_DEFINE_H_
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
	#define PLAYER_AT_LOGIN_CAP		10000		//  玩家最多在线人数
	#define AGIPSESSION_CAP			100000		//  允许同时认证的最大玩家数量 
	#define SERVERSESSION_CAP		100000		//  允许同时认证的最大玩家数量 
	#define IBPAYAUDIT_CAP			(PLAYER_AT_LOGIN_CAP)		//  允许同时认证的最大玩家数量 
#else
	#define PLAYER_AT_LOGIN_CAP		20000		// 玩家最多在线人数
	#define AGIPSESSION_CAP			20000		// 允许同时认证的最大玩家数量 
	#define SERVERSESSION_CAP		20000		// 允许同时认证的最大玩家数量 
	#define IBPAYAUDIT_CAP			(PLAYER_AT_LOGIN_CAP)		//  允许同时认证的最大玩家数量 
#endif // ! _DEBUG_

// 全局对象类型
enum EGlobalObjType
{
	OBJTYPE_START = 0x01,		// start 
	OBJTYPE_PLAYER_AT_LOGIN,	// CLoginPlayer
	OBJTYPE_AGIPSESSION, 		// CAGIPSession
	OBJTYPE_SERVERSESSION, 		// CServerSession
	OBJTYPE_IBPAYAUDIT,			// CIbPayAudit
	OBJTYPE_END					// end
};

#endif
