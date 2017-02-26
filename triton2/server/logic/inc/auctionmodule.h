#pragma once

#ifdef WIN32
#include <hash_map>
using namespace stdext;
#else
#ifdef LINUX
#include <ext/hash_map>
using namespace __gnu_cxx;
#endif
#endif



// ********************************************************************** //
// CFriendModule
// ********************************************************************** //
class CAuctionModule : public CLogicModule
{

public:
	static char			sModuleName[ NAME_LENGTH ];
public:
	CAuctionModule( ) : CLogicModule( sModuleName )	{	}
	
public:
	// 启动服务器
	virtual void OnLaunchServer( );

	// 退出服务器
	virtual void OnExitServer( );

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );

	// 建立场景
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, const char* pTplName );

	// 路由消息
	virtual void OnRouterMessage( CMessageRouter* pMessage );

	// 客户端发送消息
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity , CCreator* pCreator, bool vFirst);

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );

		// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

	void OnAuctionOpen( CDispParams *pParams, CVariant *pResult );
	void OnAuctionClose( CDispParams *pParams, CVariant *pResult );
};



