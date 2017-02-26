#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include <string>
#include "servertool.h"
#include "lk_vector.h"
#include "lk_string.h"

class CEntityPlayer;
class CEntity;
class CCreator;
class CMessage;
class CSharedMem;

namespace SceneServer
{
	class CLogicModule;

// ********************************************************************** //
// CLogicModule
// ********************************************************************** //

class CLogicModule
{
public:
	lk::string<64> mModuleName;

	static CSharedMem* mShmPtr; // 共享内存指针
	bool mRegist;

public:
	CLogicModule( const char* pModuleName ) /*: mModuleName( pModuleName )*/
	{
		mModuleName.clear();
		mModuleName.append( pModuleName );
	}

	void* operator new( size_t size );
	void  operator delete( void* pointer );
	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);

	bool IsRegist( );

public:
	virtual ~CLogicModule( )	{	}

public:
	// 启动服务器
	virtual void OnLaunchServer( )																		= 0;

	// 退出服务器
	virtual void OnExitServer( )																		= 0;

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset )							= 0;

	// 建立场景
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )		= 0;

	// 路由消息
	virtual void OnRouterMessage( CMessage* pMessage )													= 0;

	// 客户端发送消息
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )							= 0;

	// 内部超时消息
	virtual void OnTimeoutMessage( CMessage* pMessage )													{}

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )					= 0;

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode )											= 0;

	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode )									= 0;

	// 加载配置
	virtual void OnLoadConfig( )	= 0;

	// 新一天到来
	virtual void OnNewDayCome(  ) { return; }

	// 新一周到来
	virtual void OnNewWeekCome(  ) { return; }

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) = 0;

	// 服务器启动成功
	virtual void OnLaunchComplete( )	{}

	// 玩家准备离开
	virtual void OnPlayerLeave( CEntityPlayer* pPlayer, int nState ){}
};

class CModuleManager : public CSingleton< CModuleManager >
{
public:
	lk::vector< CLogicModule*, 50 >				mModules;	// 模块列表	
	enum { MAX_LOGICFUC_NUM = 2000 };

public:
	CModuleManager( );
	~CModuleManager( );

public:

	// 注册模块
	void RegisterModule( CLogicModule* pModule );

	// 调用注册模块
	void LaunchServer( );

	// 调用注册模块
	void ExitServer( );

	// 调用注册模块
	void RouterMessage( CMessage* pMessage );

	// 调用注册模块
	void ClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 调用注册模块
	void CreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst );

	// 调用注册模块
	void DestroyEntity( CEntity* pEntity, int vCode );

	// 调用注册模块
	void Timer( unsigned int vTickCount, unsigned int vTickOffset );

	// 调用注册模块
	int CreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );

	// 调用注册模块
	bool FillData( CEntityPlayer* pPlayer, int vSaveCode );

	// 判断是否有某个功能
	bool HasFunc( int vFuncID );

	// 加载配置
	void LoadConfig( );

	// 新一天到来
	void OnNewDayCome( );

	// 新一周到来
	void OnNewWeekCome( );

	// 启动是否成功
	bool CheckLaunchServer( );

	// 服务器启动成功
	void LaunchComplete();

	// 玩家准备离开
	void OnPlayerLeave( CEntityPlayer *pPlayer, int nState );
};

}
#endif


