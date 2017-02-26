// ********************************************************************** //
// ModuleManager.cpp
// 模块管理器,负责维护模块的生命期
//
// Author: Black
// ********************************************************************** //
#include "modulemanager.h"

using namespace DungeonServer;

template<> CModuleManager* CSingleton< CModuleManager >::spSingleton = NULL;

//////////////////////////////////////////////////////////////////////////
// CLogicModule
//////////////////////////////////////////////////////////////////////////
CSharedMem* CLogicModule::mShmPtr = NULL;


void* CLogicModule::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CLogicModule::operator delete( void* pointer ) { }


void* CLogicModule::operator new(size_t size, const char* file, int line)
{
	return operator new(size);
}
void CLogicModule::operator delete(void* pointer, const char* file, int line)
{

}

bool CLogicModule::IsRegist( )
{
	return mRegist;
}

//////////////////////////////////////////////////////////////////////////
// CModuleManager
//////////////////////////////////////////////////////////////////////////
CModuleManager::CModuleManager( )
{
	//memset( mFuncList, 0, sizeof( mFuncList ) );
	mModules.initailize();
}

CModuleManager::~CModuleManager( )
{
	
}

void CModuleManager::RegisterModule( CLogicModule* pModule )
{
	mModules.push_back( pModule );
	pModule->mRegist = true;
	LOG_INFO( "default", "Module [ %s ] Regist OK", pModule->mModuleName.c_str( ) );
}

void CModuleManager::LaunchServer( )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
		mModules[ i ]->OnLaunchServer( );
}

void CModuleManager::ExitServer( )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
		mModules[ i ]->OnExitServer( );
}

// 调用注册模块
void CModuleManager::Timer( unsigned int vTickCount, unsigned int vTickOffset )
{
	char buff[128] = {0};

	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{		
		mModules[ i ]->OnTimer( vTickCount, vTickOffset );
	}
}

// 加载配置
void CModuleManager::LoadConfig( )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
		mModules[ i ]->OnLoadConfig( );
}

// 新一天到来
void CModuleManager::OnNewDayCome(  )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
		mModules[ i ]->OnNewDayCome( );
}

// 新一周到来
void CModuleManager::OnNewWeekCome(  )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
		mModules[ i ]->OnNewWeekCome( );
}

bool CModuleManager::CheckLaunchServer()
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		if ( mModules[ i ]->OnCheckLaunch() == false )
			return false;
	}
	return true;
}

// 服务器启动成功
void CModuleManager::LaunchComplete()
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		mModules[ i ]->OnLaunchComplete();
	}
}


// 接收服务器消息
void CModuleManager::ServerMessage( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		mModules[ i ]->OnServerMessage( pMsg, nGameID, nSrcFe, nSrcID );
	}
}