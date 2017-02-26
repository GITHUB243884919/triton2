#ifndef _MODULE_MANAGER_H_
#define _MODULE_MANAGER_H_

#include "shm.h"
#include "sharemem.h"
#include "message_pb.hxx.pb.h"
#include "servertool.h"

namespace GateServer
{

class CDataModule
{
public:
	static CSharedMem* mShmPtr; // 共享内存指针

public:
	CDataModule()	{}
	virtual ~CDataModule()	{}

	void* operator new( size_t size );
	void  operator delete( void* pointer );

public:
	// 启动服务器
	virtual void OnLaunchServer( ) = 0;

	// 退出服务器
	virtual void OnExitServer( ) = 0;

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset ) = 0;

	// 客户端发送消息
	virtual void OnMessage( int nServerID, CMessage* pMessage )	= 0;

	// 重新加载配置文件
	virtual void ReloadCfg() { }

	// 启动是否完成
	virtual bool IsLaunched() = 0;

	// 新一天到来
	virtual void OnNewDayCome() { }

	// 新一周到来
	virtual void OnNewWeekCome() { }
	
	// 服务器启动结束
	virtual void OnLaunchComplete() { }	  
};


class CModuleManager : public CSingleton< CModuleManager >
{
private:
	std::vector< CDataModule* >				mModules;	// 模块列表
	time_t	mLastNewDayTime;		// 上一次“新一天”调用时间
	time_t	mLastNewWeakTime;		// 上一次“新一周”调用时间
	CWTimer	mNewTimer;		// 定时检测间隔(是否是新一天，新一周)

public:
	CModuleManager();
	~CModuleManager();

	void RegisterModule( CDataModule* pModule );

	void LaunchServer();

	void ExitServer();

	void OnMessage( int nServerID, CMessage* pMessage );

	void Timer( unsigned int vTickCount, unsigned int vTickOffset );

	void OnReloadCfg();

	bool IsLaunched();

	void OnNewDayCome();
	void OnNewWeekCome();	
	void OnLaunchComplete() ;
};

}

#endif



