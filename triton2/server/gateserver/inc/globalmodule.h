#pragma once


#include "module_manager.h"
#include "entity.h"
#include "lk_hashmap.h"
#include "lk_string.h"
#include "lk_set.h"
using namespace GateServer;	

class CGlobalModule :public CDataModule, public CSingleton< CGlobalModule >
{
public:
	CGlobalModule(){}
	~CGlobalModule(){ }
public:
	// 启动服务器
	virtual void OnLaunchServer( ){}

	// 退出服务器
	virtual void OnExitServer( ) {}

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset ) { }

	// 客户端发送消息
	virtual void OnMessage( int nServerID, CMessage* pMessage );

	// 重新加载配置文件
	virtual void ReloadCfg() { }

	// 启动是否完成
	virtual bool IsLaunched() { return true; }

	// 新一天到来
	virtual void OnNewDayCome() { }

	// 新一周到来
	virtual void OnNewWeekCome() { }

	// 服务器启动结束
	virtual void OnLaunchComplete() { }	
	
	// 
	static unsigned int CountSize();
public:
	
	// 玩家请求跨区组到本服务器
	void OnMessageRoleGlobalTele( int nServerID, CMessage *pMessage );
	void SendRoleGlobalTeleResponse( int nErrcode, int nRoleID, int nWorldID, int nServerKey, int nServerID, const char *pAddr, int nLoginToken );

	// 全局副本服务器创建副本的消息请求
	void OnMessageCreateRepetion( int nServerID, CMessage *pMessage );

	// 全局副本服务器创建副本的消息回应
	void SendResponseCreateRepetion( int nErrcode, int nRegKey, KEY_TYPE nKey );

	// 玩家进入副本的消息通知
	void OnMessageWarEnableNotify( int nServerID, CMessage *pMessage );

	// 踢掉玩家的通知
	void OnMessageKickPlayer( int nServerID, CMessage *pMessage );

	// 踢出玩家的消息通知
	void OnMessageKickGlobalPlayerACK( int nServerID, CMessage *pMessage );

	// 玩家请求跨服的通知(来自场景)
	void OnMessagePlayerGlobalTele( int nServerID, CMessage *pMessage );

	// 玩家请求跨服的回应消息
	void OnMessageGlobalTeleResponse( int nServerID, CMessage *pMessage );
};