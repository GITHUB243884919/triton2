// ********************************************************************** //
// ModuleManager.cpp
// 模块管理器,负责维护模块的生命期
//
// Author: Black
// ********************************************************************** //
#include "stdafx.hpp"
#include "servercore.h"
#include "modulemanager.h"
#include "entity.h"
#include "errcode.h"

using namespace SceneServer;

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
	//for ( size_t i = 0; i < mModules.size( ); i ++ )
	//	delete mModules[ i ];

	//mModules.clear( );
}

//void CModuleManager::RegisterFunc( CLogicFunc rFunc )
//{
//	//FUNCITER tFindIter = mFuncList.find( rFunc.mFuncName );
//	//if ( tFindIter == mFuncList.end( ) )
//	//{
//	//	vector< CLogicFunc > tFuncList;
//	//	tFuncList.push_back( rFunc );
//	//	mFuncList.insert( FUNCTYPE( rFunc.mFuncName, tFuncList ) );
//	//	printf( "事件[%s]注册成功\r\n", rFunc.mFuncName.c_str( ) );
//	//	return;
//	//}
//
//	//tFindIter->second.push_back( rFunc );
//	//printf( "事件[%s]注册成功\r\n", rFunc.mFuncName.c_str( ) );
//
//	//mFuncList[ rFunc.mFuncID ] = rFunc;
//	
//
//}

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
void CModuleManager::RouterMessage( /*CMessageRouter*/CMessage* pMessage )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
		mModules[ i ]->OnRouterMessage( pMessage );
}

// 调用注册模块
void CModuleManager::ClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		if( pPlayer->GetCommState( ) == CEntityPlayer::STATE_INMAP ) 
		{
			mModules[ i ]->OnClientMessage( pPlayer, pMessage );
		}
		else
		{
			LOG_INFO( "default", "Player( %d : %s ) Not in Map, Cannt disposal Message %d", 
				pPlayer->GetCharID(), pPlayer->GetCharNameShow(), pMessage->msghead().messageid() );
		}
	}
}

// 调用注册模块
void CModuleManager::CreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
		mModules[ i ]->OnCreateEntity( pEntity, pCreator, vFirst );
}

// 调用注册模块
void CModuleManager::DestroyEntity( CEntity* pEntity, int vCode )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
		mModules[ i ]->OnDestroyEntity( pEntity, vCode );
}

// 调用注册模块
void CModuleManager::Timer( unsigned int vTickCount, unsigned int vTickOffset )
{
	char buff[128] = {0};

	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		strncpy( buff, mModules[ i ]->mModuleName.c_str(), sizeof(buff)-1 );
		strcat( buff, ":OnTimer" );
		PERF_FUNC( buff , mModules[ i ]->OnTimer( vTickCount, vTickOffset ) );
	}
}

// 调用注册模块
int CModuleManager::CreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		mModules[ i ]->OnCreateMap( vLineID, vMapID, vIndex, pTplName );
	}

	return SUCCESS;
}

// 调用注册模块
bool CModuleManager::FillData( CEntityPlayer* pPlayer, int vSaveCode )
{
	bool nResult = true;

	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		if(mModules[ i ]->OnSaveData( pPlayer, vSaveCode ) == false)
		{
			nResult = false;

			LOG_ERROR("default", "Module %s has an error for save player(%d:%s)", 
					mModules[i]->mModuleName.c_str(), pPlayer->GetCharID(), pPlayer->GetCharNameShow());
		}
	}

	return nResult;
}

//void CModuleManager::FireEvent( int vFuncID, CDispParams* pParams, CVariant* pResult )
//{
//	// 为了提高效率，这里不做数组越界判断，只要是通过枚举值来调用，就不会出错
//	
//	if ( mFuncList[ vFuncID ].mpHandler != NULL )
//	{
//		mFuncList[ vFuncID ].FireEvent( pParams, pResult );
//	}
//	else
//	{
//		LOG_ERROR( "default", "When FireEvent, Event %d didn't registered", vFuncID );
//	}
//}

bool CModuleManager::HasFunc( int vFuncID )
{
	//return mFuncList[ vFuncID ].mpHandler != NULL;
	return false;
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


void CModuleManager::LaunchComplete()
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		mModules[ i ]->OnLaunchComplete();
	}
}

// 玩家准备离开
void CModuleManager::OnPlayerLeave( CEntityPlayer *pPlayer, int nState )
{
	for ( size_t i = 0; i < mModules.size( ); i ++ )
	{
		mModules[ i ]->OnPlayerLeave( pPlayer, nState );
	}
}