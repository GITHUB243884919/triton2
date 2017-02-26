#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include <string>
#include "log.h"
#include "servertool.h"
#include "lk_vector.h"
#include "lk_string.h"
#include "shm.h"
#include "message_pb.hxx.pb.h"
namespace DungeonServer
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
		
		// 加载配置
		virtual void OnLoadConfig( )	= 0;

		// 新一天到来
		virtual void OnNewDayCome(  ) { }

		// 新一周到来
		virtual void OnNewWeekCome(  ) { }

		// 场景启动完毕检查
		virtual bool OnCheckLaunch( ) = 0;

		// 服务器启动成功
		virtual void OnLaunchComplete( ){ }

		// 接收服务器消息
		virtual void OnServerMessage( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID ) {}

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
		void Timer( unsigned int vTickCount, unsigned int vTickOffset );		
		
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

		// 接收服务器消息
		void ServerMessage( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID );
	};

}
#endif


