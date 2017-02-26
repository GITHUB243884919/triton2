// ********************************************************************** //
// ModuleManager.cpp
// 模块管理器,负责维护模块的生命期
//
// Author: Black
// ********************************************************************** //

#include "module_manager.h"
#include "log.h"
#include "base.h"
using namespace GateServer;

template<> CModuleManager* CSingleton< CModuleManager >::spSingleton = NULL;

CSharedMem* CDataModule::mShmPtr = NULL;


// ***************************************************************
//  Function:		SendGateErrorNotice
//  Description:	发送错误码和字符串
//	Input:			nErrCode - 错误码； pszErrString - 错误字符串
//	OutPut:			无
//	Return:			无
//	Others:			
//  Date:			10/29/2008
// 
// ***************************************************************
void* CDataModule::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

// ***************************************************************
//  Function:		SendGateErrorNotice
//  Description:	发送错误码和字符串
//	Input:			nErrCode - 错误码； pszErrString - 错误字符串
//	OutPut:			无
//	Return:			无
//	Others:			
//  Date:			10/29/2008
// 
// ***************************************************************
void  CDataModule::operator delete( void* pointer ) { }

// ***************************************************************
//  Function:		CModuleManager
//  Description:	构造函数
//	Input:			
//	OutPut:			无
//	Return:			无
//	Others:			
//  Date:			11/12/2008
// 
// ***************************************************************
CModuleManager::CModuleManager( )
{
	mLastNewDayTime = time(0);
	mLastNewWeakTime = time(0);
	mNewTimer = CWTimer(60000);	// 检查时间 精度 1分钟
}

// ***************************************************************
//  Function:		~CModuleManager
//  Description:	析构函数	
//	Input:			
//	OutPut:			无
//	Return:			无
//	Others:			
//  Date:			11/12/2008
// 
// ***************************************************************
CModuleManager::~CModuleManager( )
{
}

// ***************************************************************
//  Function:		RegisterModule
//  Description:	注册模块
//	Input:			
//	OutPut:			无
//	Return:			无
//	Others:			
//  Date:			11/12/2008
// 
// ***************************************************************
void CModuleManager::RegisterModule( CDataModule* pModule )
{
	mModules.push_back( pModule );
}

// ***************************************************************
//  Function:		LaunchServer
//  Description:	Gate启动的时候通知各模块
//	Input:			
//	OutPut:			无
//	Return:			无
//	Others:			
//  Date:			11/12/2008
// 
// ***************************************************************
void CModuleManager::LaunchServer( )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		mModules[ i ]->OnLaunchServer( );
	}
}

// ***************************************************************
//  Function:		ExitServer
//  Description:	GateServer退出的时候通知各模块
//	Input:			
//	OutPut:			无
//	Return:			无
//	Others:			
//  Date:			11/12/2008
// 
// ***************************************************************
void CModuleManager::ExitServer()
{
	for ( size_t i = 0; i < mModules.size(); i ++ )
	{
		mModules[i]->OnExitServer();
	}
}

// 调用注册模块
// ***************************************************************
//  Function:		OnMessage
//  Description:	响应GameServer传送过来玩家消息
//	Input:			
//	OutPut:			
//	Return:			
//	Others:			
//  Date:			11/12/2008
// 
// ***************************************************************
void CModuleManager::OnMessage(int nServerID, CMessage* pMessage )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		if( mModules[i] != NULL)
		{
			mModules[ i ]->OnMessage( nServerID, pMessage );
		}
	}
}


// ***************************************************************
//  Function:		Timer
//  Description:	
//	Input:			
//	OutPut:			无
//	Return:			无
//	Others:			
//  Date:			11/12/2008
// 
// ***************************************************************
void CModuleManager::Timer( unsigned int vTickCount, unsigned int vTickOffset )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
		mModules[ i ]->OnTimer( vTickCount, vTickOffset );

	// 是否新一天/新一周
	if( mNewTimer.Elapse( vTickOffset ) )
	{
		time_t tNow = time(NULL);
		if( IsANewDay( mLastNewDayTime, tNow ) )
		{
			OnNewDayCome();
			mLastNewDayTime = tNow;
		}
		if( IsANewWeek( mLastNewWeakTime, tNow) )
		{
			OnNewWeekCome();
			mLastNewWeakTime = tNow;
		}
	}
}


void CModuleManager::OnReloadCfg()
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		mModules[ i ]->ReloadCfg();
	}
}

bool CModuleManager::IsLaunched()
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		if (! mModules[ i ]->IsLaunched()) 
		{
			LOG_WARN("default", "Module(%d) is not launch successfully!", i);
			return false;
		}
	}

	return true;
}

void CModuleManager::OnNewDayCome()
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
		mModules[ i ]->OnNewDayCome( );
}

void CModuleManager::OnNewWeekCome()
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
		mModules[ i ]->OnNewWeekCome( );
}

void CModuleManager::OnLaunchComplete()
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
		mModules[ i ]->OnLaunchComplete( );
}
