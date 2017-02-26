#pragma once

// #ifndef _FCM_SERVICE_H_
// #define _FCM_SERVICE_H_

#include "module_manager.h"
#include "entity.h"
#include "lk_hashmap.h"
#include "lk_string.h"
#include "gateobj_define.h"
#include "lk_set.h"

using namespace GateServer;	

class CChatmodule :public CDataModule, public CSingleton< CChatmodule >
{
public:
	void OnLaunchServer();
	void OnExitServer();
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	void OnMessage(int nServerID, CMessage* pMessage );
	void OnSeverMessage(int nServerID, CMessage* pMessage );
	void OnMessageFangChenMiRequest( int vServerID, CMessage* tpMessage );
	CChatmodule();
	~CChatmodule();
	int Initialize();							    
	int Resume();
	static unsigned int CountSize();
	void* operator new( size_t nsize );
	void  operator delete( void* pointer );	
	virtual bool IsLaunched() { return true; }
public:
	// 世界频道
	void OnWorldChatMsg( CMessage* pMessage );
	
	// 密语频道
	void OnP2PChatMsg( CMessage* pMessage );
	
	// 家族频道
	void OnFamilyChatMsg( CMessage* pMessage );
	
	// 军团频道
	void OnCorpsChatMsg( CMessage* pMessage );

	// 发送系统广播
	void SendSysNotice( const char *pMsg, int RepeatedNum );
	
	// 发送队伍消息
	void OnTeamMsg( CMessage* pMessage );

private:
	
	// 把聊天内容保存到日志服务器
	void SendChatMsg2Log( CMessage* pMessage );
};



