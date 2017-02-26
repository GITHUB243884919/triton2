// ********************************************************************** //
// MapModule.cpp
// 地图模块文件实现文件
// 完成地图的移动,跨地图功能
// Author : Black
// ********************************************************************** //

#include "stdafx.hpp"
#include "message_pb.hxx.pb.h"
#include "mapmessage_pb.hxx.pb.h"
#include "promessage_pb.hxx.pb.h"
#include "coremessage_pb.hxx.pb.h"
#include "mapmodule.h"
#include "maptplmanager.h"
#include "mapobjmanager.h"
#include "sceneobj_manager.h"
#include "entity_searcher.h"
#include "propertymodule.h"
#include "npcmodule.h"
#include "message_maker.h"
#include "logservertype.h"
#include "command.h"
#include "client_comm_engine.h"
#include "taskmodule.h"
#include "countrymessage_pb.hxx.pb.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#include "globalmodule.h"
#endif



//////////////////////////////////////////////////////////////////////////
//						异步传送
//////////////////////////////////////////////////////////////////////////


CAsynTeleport::CAsynTeleport()
{
	if( CObj::msCreateMode )
	{
		Initialize();
	}
}

CAsynTeleport::~CAsynTeleport()
{

}

int CAsynTeleport::Initialize()
{
	mTokenID = 0;
	mCharID = 0;
	memset(mCharName,0,sizeof(mCharName));
	mTimeout = time(NULL) + 120;
	mLevelLmt = 0;
	mDstChar.mNationID = 0;
	mDstChar.mCountryID = COUNTRY_NONE;
	mDstChar.mCorpsID = 0;
	mDstChar.mFamilyID = 0;
	mDstChar.mKing = 0;
	mDstChar.mCharID = 0;
	memset( &mDestination, 0, sizeof(mDestination));
	return 0;
}



//////////////////////////////////////////////////////////////////////////
//								地图模块
//////////////////////////////////////////////////////////////////////////


char CMapModule::sModuleName[ NAME_LENGTH ] = "Map_1";
CMapObjManager* gpMapObjManager = NULL;
CMapTplManager* gpMapTplManager = NULL;
template<> CMapModule* CSingleton< CMapModule >::spSingleton = NULL;

typedef bool (*MapGetEntityEstimate)( CEntity*, CEntity*, int );
// 启动服务器
void CMapModule::OnLaunchServer( )
{
	CMapObjManager::mShmPtr = mShmPtr;
	CMapTplManager::mShmPtr = mShmPtr;
	gpMapObjManager = new CMapObjManager;
	gpMapTplManager = new CMapTplManager;

	if( gpMapObjManager == NULL || gpMapTplManager == NULL )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ]Launch Err", __LK_FILE__, __LINE__, __FUNCTION__ );
		printf( "[ %s : %d ][ %s ]Launch Error, So Exit", __LK_FILE__, __LINE__, __FUNCTION__ );
		exit(1);
	}
	
	OnLoadConfig();
}

void CMapModule::OnLoadConfig()
{
	MAP_LOG* tpCfg = CSceneCfgManager::GetSingletonPtr()->GetMap_log();
	if( tpCfg != NULL )
	{
		INIT_ROLLINGFILE_LOG( tpCfg->mIdentifier, tpCfg->mDirectory, (LogLevel)tpCfg->mPriority, tpCfg->mMaxfilesize, tpCfg->mMaxbackupindex );
	}

	mMoveCheck = CSceneCfgManager::GetSingletonPtr( )->GetMap_cfg( )->mMovecheck;
	CMapObjManager::GetSingletonPtr( )->ReloadDeny( );
	const char* pcConfigPath = CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(), const_cast<char*>("scene_config/DenyTemp.xml" ));
	CSceneServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CMapModule::LoadDenyTempConfigCallback);
}

unsigned int CMapModule::CountSize()
{
	unsigned int tSize = sizeof( CMapModule );

	tSize += CMapObjManager::CountSize( );

	//delete gpMapTplManager;
	//delete gpMapObjManager;
	//
	//gpMapObjManager = NULL;
	//gpMapObjManager = NULL;

	return tSize;
}

// 退出服务器
void CMapModule::OnExitServer( )
{
	if( gpMapTplManager != NULL )
	{
		delete gpMapObjManager;
	}
	if( gpMapTplManager != NULL )
	{
		delete gpMapTplManager;
	}
}

// 服务器Timer
void CMapModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	int tTime = time(NULL);
	for ( AsynTeleport::iterator it = mAsynTeleportList.begin(); it != mAsynTeleportList.end(); )
	{
		if ( it->second.mTimeout < tTime )
		{
			mAsynTeleportList.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

// 建立场景
int CMapModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )
{

	return SUCCESS;
}

// 路由消息
void CMapModule::OnRouterMessage( CMessage* pMessage )
{
}

// 客户端发送消息
void CMapModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	switch( pMessage->msghead().messageid() )
	{
		// 客户端移动通知
		case ID_C2S_REQUEST_PLAYERPATH:
		{
			OnMessagePlayerPathRequest( pPlayer, pMessage );
			break;
		}

		// 客户端移动停止
		case ID_C2S_REQUEST_PLAYERSTOP:
		{
			OnMessagePlayerStopRequest( pPlayer, pMessage );
			break;
		}

		// pet移动
		case ID_C2S_REQUEST_PETPATH:
		{
			OnMessagePetPathRequest( pPlayer, pMessage );
			break;
		}

		// pet停止移动
		case ID_C2S_REQUEST_PETSTOP:
		{
			OnMessagePetStopRequest( pPlayer, pMessage );
			break;
		}

		default:
		{
			break;
		}
	}
}


// ***************************************************************
//  Function:		OnCreateEntity
//  Description:	地图模块建立实体
//	Input:			pEntity - 实体指针； pCreator - 创造物指针； vFirst - 是否首次创建
//	OutPut:			无
//	Return:			无
//	Others:			地图模块也采用Google ProtoBuff来存储数据
//  Date:			10/28/2008
// 
// ***************************************************************
void CMapModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	LK_ASSERT( pEntity != NULL, return );

	CMapObject* tpMap = CMapObjManager::GetSingleton( ).GetMapObject( pEntity->GetLineID( ), pEntity->GetMapID( ), pEntity->GetMapIndex( ) );
	if ( tpMap == NULL )
	{
		LOG_ERROR( "map", "[%s:%d][%s] line:%d mapid(index=%d):%d not found", 
			__LK_FILE__, __LINE__, __FUNCTION__,  pEntity->GetLineID(), pEntity->GetMapIndex(), pEntity->GetMapID() );
		return;
	}

	if ( pEntity->IsPlayer() )
	{
		CEntityPlayer* tpPlayer = (CEntityPlayer*) pEntity;
		// 检查地图专用buff
		tpPlayer->CheckBuffOnChangeMap( tpMap );

		CMessage tMessage;
		tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTICE_YOURMAPDATA );
		CMessageYourMapDataNotice tYourMap;
		//tYourMap.set_charid( pEntity->GetEntityID( ) );
		tYourMap.set_lineid( tpPlayer->GetLineID() );

		tYourMap.set_mapid( tpPlayer->GetMapID());
#ifdef _FB_
		tYourMap.set_virtualmapid( tpMap->GetVirtualMapID( ) );
#endif
/*
#ifdef _FB_
		if( tpPlayer->GetMapID() == tpMap->GetVirtualMapID( ) || tpMap->GetVirtualMapID( ) == INVALID_MAP_INDEX )
		{
			tYourMap.set_mapid( tpPlayer->GetMapID());
		}
		else
		{
			tYourMap.set_mapid( tpMap->GetVirtualMapID( ) );
		}
#else
		tYourMap.set_mapid( tpPlayer->GetMapID( ) );
#endif
*/
		tYourMap.set_mapname( tpMap->GetMapName() );
		tYourMap.set_posx( pEntity->GetPosX() );
		tYourMap.set_posy( pEntity->GetPosY() );
		tYourMap.set_direction( pEntity->GetDirection( ) );
		tYourMap.set_alivestate( pEntity->GetAliveStatus( ) );
		tYourMap.set_motionstate( ( ( CEntityPlayer* )pEntity )->GetMotionState( ) );
		int tLineID = tpPlayer->GetLineID();
		if ( IS_REPETION_LINE_ID( tpPlayer->GetLineID() ) == true
			|| IS_NEUTRAL_LINE_ID( tpPlayer->GetLineID() ) == true
			|| IS_BATTLE_LINE_ID( tpPlayer->GetLineID() ) == true )
		{
			tLineID = tpPlayer->GetNationality( );
		}
		if( CSceneServer::GetSingletonPtr()->GetCountryName( tLineID ) != NULL )
		{
			tYourMap.set_countryname( CSceneServer::GetSingleton().GetCountryName( tLineID ) );
		}
		tMessage.set_msgpara( (uint32)&tYourMap );

		CSceneLogic::GetSingleton( ).Send2Player( pEntity, &tMessage );		
	}

	// 设置地图ID
	pEntity->SetMapObjectID( tpMap->get_id() );

	int nRet = 0;
	PERF_FUNC("map::createntity", nRet = tpMap->CreateEntity( pEntity, pCreator ,vFirst) );
	if ( nRet == SUCCESS && pEntity->IsPlayer() )
	{
		CEntityPlayer* tpPlayer = (CEntityPlayer*) pEntity;
		CPropertyPlayer* pProperty = (CPropertyPlayer*)pEntity->GetProperty();
		// 如果坐标超出地图边界，或者玩家点是阻挡 让他回城
		bool bIsOutMap = (!IsCellValid( pEntity, pEntity->GetPosX(), pEntity->GetPosY() ));
		if( bIsOutMap )
		{
			LOG_WARN( "map", "[%s:%d] player charid=%d,pos(%d,%d) out of map %d now return!", __FILE__, __LINE__,
				tpPlayer->GetCharID(), pEntity->GetPosX(), pEntity->GetPosY(), pEntity->GetMapID() );
			//return ;
		}

		bool bIsStaticBlock = ((CEntityCharacter*)pEntity)->CanFly() == true ? false: IsStaticBlock( pEntity->GetLineID( ), pEntity->GetMapID( ), pEntity->GetMapIndex( ), pEntity->GetPos() );
		
		bool bIsSuperBlock = IsSuperBlock( pEntity->GetLineID( ), pEntity->GetMapID( ), pEntity->GetMapIndex( ), pEntity->GetPos() );
		
		if( bIsStaticBlock || bIsSuperBlock )
		{
			LOG_WARN( "map", "[%s:%d] player charid=%d,pos(%d,%d) in map %d is block, now return!", __FILE__, __LINE__,
				tpPlayer->GetCharID(), pEntity->GetPosX(), pEntity->GetPosY(), pEntity->GetMapID() );
		}

		if ( bIsOutMap || bIsStaticBlock || bIsSuperBlock )
		{
			Teleport( pProperty->GetTownPos( )->mLineID, pProperty->GetTownPos( )->mMapID, 
				pProperty->GetTownPos( )->mPos.mX, pProperty->GetTownPos( )->mPos.mY, pEntity ) ;
		}

		// 注意！如果做了Teleport 则后面不要再用tpMap了！这还是之前那个老的mapobj

		// 因为不管是同服跨地图,还是跨服跨地图,都要走这个流程,所以在这里判断该场景能否骑马
		( (CPropertyPlayer*) pEntity->GetProperty( ) )->ChangeHorseStateTransMap( pEntity->GetMapID( ) );
	}
}

// 存储数据
bool CMapModule::OnSaveData( CEntityPlayer* pPlayer, int vSaveCode )
{	
	return true;
}

// 销毁实体
void CMapModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{
	LK_ASSERT( pEntity != NULL, return );

	CMapObject* tpMap = CMapObjManager::GetSingleton( ).GetMapObject( pEntity->GetLineID( ), 
		pEntity->GetMapID( ), pEntity->GetMapIndex( ) );

	if ( tpMap == NULL )
	{
		LOG_ERROR( "map", "[%s:%d][%s] can't find entity %d in map (line:%d, map:%d) code %d", 
			__LK_FILE__, __LINE__, __FUNCTION__, 
			pEntity->GetEntityID(), pEntity->GetLineID( ), pEntity->GetMapID( ),
			vCode );
		
		// 有可能根据lineid,mapid,index找不到地图，但是可以直接找到mapobject对象
		tpMap = pEntity->GetMapObject();
		if( tpMap != NULL )
		{
			LOG_ERROR( "map", "[%s:%d] entity %d find map by mapobject", 
				__FUNCTION__, __LINE__, pEntity->GetEntityID() );
		}
		else
		{
			return ;
		}
	}

	tpMap->DestroyEntity( pEntity );

#ifdef _FB_
	if( pEntity->IsPlayer( ) == true )
	{
		CEntityPlayer* tpPlayer = ( CEntityPlayer* )pEntity;
		LOG_DEBUG( REPETION_LOG, "[ %s : %d ][ %s ] Player( %d : %s ) Remove Map( %d : %lld ) Repetion( %d )",
			__LK_FILE__, __LINE__, __FUNCTION__, tpPlayer->GetCharID( ), tpPlayer->GetCharNameShow( ), 
			tpMap->GetMapID( ), tpMap->GetKey( ), tpMap->mRepetionID );
	}
#endif
}


bool CMapModule::CanPlayerPass( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, unsigned int vPosX, unsigned int vPosY, bool vFlyState )
{
	CMapObject* tpMapObj = CMapObjManager::GetSingleton().GetMapObject( vLineID, vMapID, vIndex );
	if ( tpMapObj == NULL )
	{
		LOG_ERROR( "map", "[ %s : %d ] [ %s ] can't find map LineID = %d MapID = %d", __LK_FILE__, __LINE__, __FUNCTION__, vLineID, vMapID );
		return false;
	}

	if ( tpMapObj->IsSuperBlock( CWTPoint( vPosX, vPosY ) ) == true )
	{
		return false;
	}	

	if ( tpMapObj->IsStaticBlock( CWTPoint( vPosX, vPosY ) ) == false || vFlyState == true )
	{
		return true;
	}

	return false;
}


// ***********************************************************
//  Function:		CanNpcPass
//  Description:	判断NPC是否可以移动到指定地图块上
//  Input:			unsigned short vLineID
//					unsigned shrot vMapID
//					unsigned int vPosX
//					unsigned int vPosY
//  OutPut:			
//  Return:			bool
//  Others:
//  Date:	10/31/2008
// **********************************************************
bool CMapModule::CanNpcPass( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, unsigned int vPosX, unsigned int vPosY, bool vFlyState )
{
	CMapObject* tpMapObj = CMapObjManager::GetSingleton().GetMapObject( vLineID, vMapID, vIndex );
	if ( tpMapObj == NULL )
	{
		LOG_ERROR( "map", "[ %s : %d ] [ %s ] can't find map LineID = %d MapID = %d", __LK_FILE__, __LINE__, __FUNCTION__, vLineID, vMapID );
		return false;
	}

	if ( tpMapObj->IsSuperBlock( CWTPoint( vPosX, vPosY ) ) == true )
	{
		return false;
	}	

	if ( tpMapObj->CanNpcPass( CWTPoint( vPosX, vPosY ) ) == true && ( tpMapObj->IsStaticBlock( CWTPoint( vPosX, vPosY ) ) == false || vFlyState == true ) )
	{
		return true;
	}

	return false;
}

// 优化，主要为npc模块服务
bool CMapModule::CanNpcPass( CEntityCharacter* tpEntity, unsigned int vPosX, unsigned int vPosY )
{
	CMapObject* tpMapObj = tpEntity->GetMapObject();
	if ( tpMapObj == NULL )
	{
		LOG_ERROR( "map", "[ %s : %d ] [ %s ] can't find map LineID = %d MapID = %d", 
			__LK_FILE__, __LINE__, __FUNCTION__, tpEntity->GetLineID(), tpEntity->GetMapID() );
		return false;
	}

	if ( tpEntity->IsNpc() == false )
	{
		return false;
	}
	
	CEntityNpc *tpNpc = (CEntityNpc*)tpEntity;
	CPropertyNPC *tpProperty = (CPropertyNPC*)tpNpc->GetProperty();

	if ( tpMapObj->IsSuperBlock( CWTPoint( vPosX, vPosY ) ) == true )
	{
		return false;
	}	

	if ( tpMapObj->CanNpcPass( CWTPoint( vPosX, vPosY ) ) == true 
		&& ( tpMapObj->IsStaticBlock( CWTPoint( vPosX, vPosY ) ) == false || tpProperty->GetStateParam( STATE_FLY ).mInState == true ) )
	{
		return true;
	}

	return false;
}


// 是否是摆摊安全区域

bool CMapModule::IsStallSafeArea(unsigned short usLineID, unsigned short usMapID, unsigned short vIndex, unsigned int vPosX, unsigned int vPosY )
{
	CMapObject* tpMapObj = CMapObjManager::GetSingleton().GetMapObject( usLineID, usMapID, vIndex );
	if ( tpMapObj == NULL )
	{
		LOG_ERROR( "map", "[ %s : %d ] [ %s ] can't find map LineID = %d MapID = %d", __LK_FILE__, __LINE__, __FUNCTION__, usLineID, usMapID );
		return false;
	}

	if ( tpMapObj->CanNpcPass( CWTPoint( vPosX, vPosY ) ) == true && tpMapObj->IsStallSaftyArea( CWTPoint( vPosX, vPosY ) ) == true )
	{
		return true;
	}

	return false;
}

// ***************************************************************
//  Function:		IsSafeRegion
//  Description:	判断坐标是否处于安全域
//	Input:			usLine	- 线
//					usMapID - 地图ID
//					usIndex	- Map Index
//					nPosX	- x坐标
//					nPosY	- y坐标
//	OutPut:			无
//	Return:			无
//	Others:			通过地图文件读入哪些范围是安全域
//  Date:			11/18/2008
// 
// ***************************************************************
//bool CMapModule::IsSafeRegion( unsigned short usLineID, unsigned short usMapID, unsigned short usIndex, int nPosX, int nPoxY  )
//{
//	CMapObject* tpMapObj = CMapObjManager::GetSingleton( ).GetMapObject( usLineID, usMapID, usIndex );
//	if ( tpMapObj == NULL )
//	{
//		LOG_ERROR( "map", "[%s:%d][%s] can't find map by line %d map %d", __LK_FILE__, __LINE__, __FUNCTION__,
//			usLineID, usMapID );
//		return false;
//	}
//
//	if ( tpMapObj->IsSafeRegion( CWTPoint( nPosX, nPoxY ) ) == true )
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//

// ***************************************************************
//  Function:		IsCellValid
//  Description:	验证格子是否有效
//	Input:			pEntity	- 实体
//					nPosX	- x坐标
//					nPosY	- y坐标
//	OutPut:			无
//	Return:			ture - 有效； false - 无效
//	Others:			无
//  Date:			11/18/2008
// 
// ***************************************************************
bool CMapModule::IsCellValid( CEntity* pEntity, int nPosX, int nPoxY )
{
	LK_ASSERT( pEntity != NULL, return false );

	CMapObject* tpMapObj	= CMapObjManager::GetSingleton( ).GetMapObject( pEntity->GetLineID( ), 
		pEntity->GetMapID( ), pEntity->GetMapIndex( ) );
	if ( tpMapObj == NULL )
	{
		LOG_WARN( "map", "[%s:%d][%s] can't find map by line %d map %d", __LK_FILE__, __LINE__, __FUNCTION__,
			pEntity->GetLineID( ), pEntity->GetMapID( ) );
		return false;
	}

	if( tpMapObj->IsValidPos( CWTPoint( nPosX, nPoxY ) ) )
	{
		return true;
	}
	else
	{
		LOG_WARN( "map", "[%s:%d] entity %d map(%d) pos(%d,%d) is not valid cell", 
			__FUNCTION__, __LINE__, pEntity->GetEntityID(), pEntity->GetMapID(), nPosX, nPoxY );
		return false;
	}
}


// ***************************************************************
//  Function:		PathEntity
//  Description:	实体沿路径移动
//	Input:			pEntity		- 实体
//					nSrcPosX	- 源坐标x
//					nSrcPosY	- 源坐标y
//					nDesPosX	- 目的坐标x
//					nDesPosY	- 目的坐标y
//					usDir		- 朝向
//	OutPut:			无
//	Return:			ture - 成功; false - 失败
//	Others:
//  Date:			11/18/2008
// 
// ***************************************************************
bool CMapModule::PathEntity(CEntityCharacter* pEntity, int nSrcPosX, int nSrcPosY, int nDesPosX, int nDesPosY, unsigned short usDir )
{
	LK_ASSERT( pEntity != NULL, return false );

	CMapObject* tpMapObj = pEntity->GetMapObject();
	if ( tpMapObj == NULL )
	{
		LOG_ERROR( "map", "[%s:%d][%s] can't find map by line %d map %d", __LK_FILE__, __LINE__, __FUNCTION__,
			pEntity->GetLineID( ), pEntity->GetMapID( ) );
		return false;
	}
	
	tpMapObj->PathEntity( pEntity, CWTPoint( nSrcPosX, nSrcPosY ), CWTPoint( nDesPosX, nDesPosY ), usDir );

	return true;
}


// ***************************************************************
//  Function:		MoveEntity
//  Description:	实体移动
//	Input:			pEntity		- 实体ID
//					nPosX		- 坐标x
//					nPosY		- 坐标y
//					bStop		- 是否停止
//	OutPut:			无
//	Return:			true - 成功； false - 失败
//	Others:
//  Date:			11/18/2008
// 
// ***************************************************************
bool CMapModule::MoveEntity( CEntityCharacter* pEntity, int nPosX, int nPosY, bool bStop )
{
	CMapObject* tpMapObj	= CMapObjManager::GetSingleton( ).GetMapObject( pEntity->GetLineID( ), 
		pEntity->GetMapID( ), pEntity->GetMapIndex( ) );
	if ( tpMapObj == NULL )
	{
		LOG_ERROR( "map", "[%s:%d][%s] can't find mapobj by line %d mapid %d", __LK_FILE__, __LINE__, __FUNCTION__, 
			pEntity->GetLineID( ), pEntity->GetMapID( ));	
		return false; 
	}
	
	tpMapObj->MoveEntity( pEntity, CWTPoint( nPosX, nPosY ) );

	return true;
}


// ***************************************************************
//  Function:		Teleport
//  Description:	实体传送
//	Input:			usLineID	- 目的线ID
//					usMapID		- 目的地图ID
//					nPosX		- 目的x坐标
//					nPosY		- 目的y坐标
//					pEntity		- 实体
//	OutPut:			无
//	Return:			true - 成功; false - 失败
//	Others:			无 
//  Date:			11/18/2008
// 
// ***************************************************************
bool CMapModule::Teleport( unsigned short usLineID, unsigned short usMapID, int nPosX, int nPosY, CEntity* pEntity, unsigned short usMapIndex, KEY_TYPE vKey )
{
	LK_ASSERT( pEntity != NULL, return false );

	CMapObject* tpOldMap	= CMapObjManager::GetSingleton( ).GetMapObject( pEntity->GetLineID(), pEntity->GetMapID(), pEntity->GetMapIndex( ) );
	if ( tpOldMap == NULL )
	{	
		LOG_ERROR( "map", "[%s:%d][%s] can't find mapobj by line %d mapid %d", __LK_FILE__, __LINE__, __FUNCTION__, 
			pEntity->GetLineID(), pEntity->GetMapID() );
		return false;
	}

	unsigned short tLineID = 0;
	if( usLineID == _SELF_SERVER_LINE_ID_ )
	{
		tLineID = pEntity->GetLineID();
	}
	else if( usLineID == _NATIONALITY_LINE_ID_ )
	{
		tLineID = pEntity->GetNationality( );
	}
	else
	{
		tLineID = usLineID;
	}	

	CMapObject* tpNewMap = NULL;

	if( pEntity->GetMapID( ) != usMapID && IS_REPETION_LINE_ID( usLineID ) != true )
	{
		tpNewMap = CMapObjManager::GetSingleton( ).GetMapObject( tLineID, usMapID, INVALID_MAP_INDEX, vKey );
	}
	else
	{
		tpNewMap = CMapObjManager::GetSingleton( ).GetMapObject( tLineID, usMapID, 
			( usMapIndex == INVALID_MAP_INDEX ) ? pEntity->GetMapIndex( ) : usMapIndex, vKey );
	}

	if ( tpNewMap != NULL )
	{
		if( nPosX >= tpNewMap->GetWidth() || nPosY >= tpNewMap->GetHight() )
		{
			LOG_WARN( "map", "dest posx %d or posy %d out of range of line %d map %d", nPosX, nPosY, tLineID, usMapID );
			return false;
		}
		
		// 目的地不能是静态阻挡
		if ( tpNewMap->IsSuperBlock( CWTPoint(nPosX,nPosY) ) == true || tpNewMap->IsBlock( nPosX, nPosY ) == true
			|| ( tpNewMap->IsStaticBlock( CWTPoint(nPosX, nPosY) ) == true && ((CEntityCharacter*)pEntity)->CanFly() == false ) )
		{
			//TODO: 宠物还不能随玩家飞,暂时屏蔽掉宠物的错误日志
			if( ! pEntity->IsPet() )
			{
				LOG_ERROR( "map", "[%s:%d][%s] entity %d teleport, line %d map %d pos(%d,%d) is static block", __LK_FILE__, __LINE__, 
				__FUNCTION__, pEntity->GetEntityID(), tLineID, usMapID, nPosX, nPosY );
			}
			return false;
		}
	}

	// 玩家传送
	if ( pEntity->IsPlayer() )
	{
		CEntityPlayer* pPlayer = (CEntityPlayer*)pEntity;
		// 如果玩家正在跨服，则不再响应传送, 因为这个在传送点不停的ontimer时可能会多次发送传送给gate
//		if( pPlayer->GetCommState( ) == CEntityPlayer::STATE_REDIRECT )
//		{
//			return false;
//		}

		// 如果玩家死亡，不能传送
		if( pPlayer->IsAlive() == false )
		{
			return false;
		}

		// 处于摆摊状态，不能传送
		CPropertyPlayer* tpProperty = ( CPropertyPlayer* )pPlayer->GetProperty( );
		if ( tpProperty->GetServiceStatus() == SERVICE_STALL ) 
		{
			return false;
		}

		if ( tpNewMap != NULL )
		{
			// 如果在本服务器找到了目标场景
			CMessage tMessage;
			CMessageReconnectSceneNotice tRedirectScene;

			tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_RECONNECTSCENE );
			tMessage.set_msgpara( (uint32)&tRedirectScene );

			tRedirectScene.set_address( "" );
			tRedirectScene.set_mapname( tpNewMap->GetMapName() );
			tRedirectScene.set_lineid( tLineID );
#ifndef _FB_
			tRedirectScene.set_mapid( usMapID );
#else
			tRedirectScene.set_mapid( 
				( usMapID == tpNewMap->GetVirtualMapID( ) ) ? usMapID : tpNewMap->GetVirtualMapID( ) );
#endif
			tRedirectScene.set_posx( nPosX );
			tRedirectScene.set_posy( nPosY );
			if( CSceneServer::GetSingletonPtr()->GetCountryName( tLineID ) != NULL )
			{
				tRedirectScene.set_countryname( CSceneServer::GetSingletonPtr()->GetCountryName( tLineID ) );
			}

			CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );

			// 把老场景中的玩家删除，所有其它模块不必修改
			tpOldMap->DestroyEntity( pPlayer );

			pPlayer->SetLineID( tLineID );
			pPlayer->SetMapID( usMapID );
			pPlayer->SetPosX( nPosX );
			pPlayer->SetPosY( nPosY);
			pPlayer->SetMotionState( MOTION_IDLE );
		
			pPlayer->SetTargetPos( CWTPoint(nPosX,nPosY) );

			// 修改玩家的地图对象ID
			pPlayer->SetMapObjectID( tpNewMap->get_id() ); 			
			
			// 在新场景中加入玩家，所有其它模块不必修改
			int nRet = tpNewMap->CreateEntity( pPlayer, NULL );	 			
			
			
			if ( nRet == SUCCESS )
			{
				if( tpOldMap->GetMapID() != pPlayer->GetMapID() )
				{
					// 因为不管是同服跨地图,还是跨服跨地图,都要走这个流程,所以在这里判断该场景能否骑马
					( (CPropertyPlayer*) pEntity->GetProperty( ) )->ChangeHorseStateTransMap( tpNewMap->GetMapID( ) );

					// 通知任务
					CTaskModule::GetSingleton().OnPlayerRedirectMap( pPlayer );

					// 通知网关地图改变
					CCommandSendPlayerDirtyDataToGate tSendChangeGate( pEntity->GetEntityID() );
					tSendChangeGate.OnEventTrigger( CCommandSendPlayerDirtyDataToGate::DIRTYDATA_MAPID );

					// 检查地图专用buff
					pPlayer->CheckBuffOnChangeMap( tpNewMap );

					// 如果是齐天大战，要调用创建
					if ( IS_BATTLE_LINE_ID( pPlayer->GetLineID() ) == true )
					{
						// 暂时先用CreateEntity
						CWorldModule::GetSingleton().OnCreateEntity( pPlayer, NULL, false );
					}
				}

				// 进入副本需要记录日志
				/*
				if( IS_REPETION_LINE_ID( tpNewMap->GetLineID() ) == true )
					CSceneLogic::GetSingleton().Send2Log( BuildEnterFBLogMessage( 
							(CEntityPlayer*)pEntity, tpNewMap->GetKey(), 0 ), 
							EIHANDLE_FIVE, pPlayer->GetCharID() );
				*/

			}
			
			
		}
		else
		{
			LOG_NOTICE( "map", "[ %s : %d ][ %s ]Player( %d ) Begin Redirect Scene( %d : %d ) Pos( %d : %d )",
					__LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetCharID(), tLineID, usMapID, nPosX, nPosY );
			pPlayer->SetTLineID( tLineID );
			pPlayer->SetTMapID( usMapID );
			pPlayer->SetTX( nPosX );
			pPlayer->SetTY( nPosY );

			// 玩家处于跨服状态
			pPlayer->SetCommState( CEntityPlayer::STATE_REDIRECT );	
			pPlayer->SetRedirectTimeStamp(time(0));

			// 玩家准备离开本服务器 ( 注意：还没有离开　)
			SceneServer::CSceneLogic::GetSingletonPtr()->PlayerLeaveFromGame( pPlayer );
		}
	}
	else
	{
		if ( pEntity->IsPet() || pEntity->IsOgre() || pEntity->IsFunNpc() || pEntity->IsCollect() )
		{
			CEntityNpc* tpNpc = (CEntityNpc*) pEntity;

			if ( tpNewMap != NULL )
			{

				// 如果在本服务器找到了目标场景
				// 把老场景中的玩家删除，所有其它模块不必修改
				tpOldMap->DestroyEntity( tpNpc );

				tpNpc->SetLineID( tLineID );
				tpNpc->SetMapID( usMapID );
				tpNpc->SetPosX( nPosX );
				tpNpc->SetPosY( nPosY );
				tpNpc->SetMotionState( MOTION_IDLE );
				
				tpNpc->SetTargetPos(  CWTPoint(nPosX,nPosY) );

				// 修改pet / ogre 的地图对象ID
				tpNpc->SetMapObjectID( tpNewMap->get_id() );

				// 在新场景中加入玩家，所有其它模块不必修改
				tpNewMap->CreateEntity( tpNpc, NULL );
				CNpcModule::GetSingletonPtr()->OnTeleport( pEntity->GetEntityID(), nPosX, nPosY );
			}
			else
			{
				// 如果在本服务器没有找到目标场景
				// not player cannt send to other server
				LOG_ERROR( "map", "[ %s : %d ][ %s ]NPC Cannt send to other server, type is %d", __LK_FILE__, __LINE__,
					__FUNCTION__, pEntity->GetEntityType() );	
			}
		}
	}	

	return true;
}


// 发送消息到看到指定目标的客户端
void CMapModule::GetRangeEntity( 	unsigned short vLineID, 
									unsigned short vMapID,
									unsigned short vIndex,
									unsigned short vRangeType,
									unsigned short vLength,
									unsigned short vWidth,
									CEntity* vpSrcEntity,
									CEntityCharacter** vpEntityList,
									int* vpEntityCount
								)
{
	unsigned short tRadius		= vLength;
	unsigned short tAngleLength	= vLength;

	if( vpSrcEntity == NULL || vpEntityList == NULL )
		return;

	CMapObject* tpMapObj = CMapObjManager::GetSingleton( ).GetMapObject( vLineID, vMapID, vIndex );
	if ( tpMapObj == NULL )
		return;

	CWTPoint tPointList[ FUNC_LIMIT ];
	int	tPointNumber	= 0;
	int tEntityNumber	= 0;
	CWTPoint tPoint;
	CWTPoint tSrcPoint  = vpSrcEntity->GetPos( );
	int		 tDirection	= vpSrcEntity->GetDirection( );

	int tSize = sizeof( tPointList ) / sizeof( tPointList[ 0 ] ) - 1;
	switch( vRangeType )
	{
		case CTemplateNormalSkill::RANGE_SELFCIRCLE:
		case CTemplateNormalSkill::RANGE_TARGETCIRCLE:
			{
				int tLeft   = tSrcPoint.mX - ( tRadius - 1 );
		        int tRight  = tSrcPoint.mX + ( tRadius - 1 );
		        int tTop    = tSrcPoint.mY - ( tRadius - 1 );
		        int tBottom = tSrcPoint.mY + ( tRadius - 1 );
		        for ( int i = tLeft; i <= tRight; i ++ )
        		{
                  	if( tPointNumber >= tSize )
                  	{
                  		break;
                  	}
            		for ( int j = tTop; j <= tBottom; j ++ )
            		{
                        if( tPointNumber >= tSize )
                        {
                            break;
                        }
                    	tPointList[ tPointNumber ++ ] = CWTPoint( i, j );
						if( tPointNumber >= tSize )
						{
							break;
						}
            		}
       			}
			}
			break;
		case CTemplateNormalSkill::RANGE_BOUND:
			{
	/*			int tCount = 2;
				bool tFind = true;
				tPointList[ tPointNumber++ ] = tSrcPoint;
               	if( tPointNumber >= tSize )
                {
					break;
				}

                
				while( tCount > 0 && tFind == true )
				{
					tFind = false;
	                int tLeft   = tSrcPoint.mX - ( tRadius - 1 );
    	            int tRight  = tSrcPoint.mX + ( tRadius - 1 );
        	        int tTop    = tSrcPoint.mY - ( tRadius - 1 );
            	    int tBottom = tSrcPoint.mY + ( tRadius - 1 );

                   	if( tPointNumber >= tSize )
                   	{
						break;
                   	}

                	for ( int i = tLeft; i <= tRight; i ++ )
                	{
                        if( tPointNumber >= tSize )
                        {
                            break;
                        }
                    	for ( int j = tTop; j <= tBottom; j ++ )
                    	{
                        	if( tPointNumber >= tSize )
                        	{
                            	break;
                        	}
							if( tSrcPoint.mX != i && tSrcPoint.mY != j )
							{
								CMapObject::CellsEntity* tpCell = tpMapObj->GetCell( CWTPoint( i, j ) );
								if( tpCell == NULL )
								{
									LOG_ERROR( "map", "[%s:%d] pos(%d,%d) map(%d) not valid",
										__FUNCTION__, __LINE__, i, j, tpMapObj->GetMapID() );
									continue;
								}
                        		if( tpCell->begin() != tpCell->end() )
								{
									tPointList[ tPointNumber ++ ] = CWTPoint( i, j );
									tSrcPoint = CWTPoint( i, j );
									tCount--;
									tFind = true;
									if( tPointNumber >= tSize )
                        			{
                            			break;
                        			}
								}
							}
							if( tFind == true )
								break;
                    	}
						if( tFind == true )
							break;
                	}
				}
				*/				
			}
			break;
		case CTemplateNormalSkill::RANGE_FRONTRECT:
			{
				if ( tDirection == DIR_DOWN || tDirection == DIR_UP || tDirection == DIR_LEFT || 
					tDirection == DIR_RIGHT )
				{
					for ( int i = 0; i < vLength; i ++ )
					{
                        if( tPointNumber >= tSize )
                        {
                            break;
                        }
						for ( int j = 0; j < vWidth; j ++ )
						{
                        	if( tPointNumber >= tSize )
                        	{
                            	break;
                        	}
							if ( tDirection == DIR_DOWN )
								tPoint = CWTPoint( tSrcPoint.mX + j - ( vWidth >> 1 ), tSrcPoint.mY + i + 1 );
							if ( tDirection == DIR_UP )
								tPoint = CWTPoint( tSrcPoint.mX + j - ( vWidth >> 1 ), tSrcPoint.mY - i - 1 );
							if ( tDirection == DIR_LEFT )
								tPoint = CWTPoint( tSrcPoint.mX - i - 1, tSrcPoint.mY + j - ( vWidth >> 1 ) );
							if ( tDirection == DIR_RIGHT )
								tPoint = CWTPoint( tSrcPoint.mX + i + 1, tSrcPoint.mY + j - ( vWidth >> 1 ) );
							tPointList[ tPointNumber ++ ] = tPoint;
                        	if( tPointNumber >= tSize )
                        	{
                            	break;
                        	}
						}
					}
				}

				if ( tDirection == DIR_LEFTDOWN || tDirection == DIR_LEFTUP || tDirection == DIR_RIGHTDOWN ||  
					tDirection == DIR_RIGHTUP )
				{
					for ( int i = 0; i < vLength; i ++ )
					{
						if( tPointNumber >= tSize )
                        {
                            break;
                        }
						for ( int j = 0; j < vWidth; j ++ )
						{
							if( tPointNumber >= tSize )
                           	{
                            	break;
                            } 
							if ( tDirection == DIR_LEFTDOWN )
							{
								if ( j < ( vWidth >> 1 ) )
								{
									tPoint = CWTPoint( tSrcPoint.mX - i - 1, tSrcPoint.mY + i + 1 - ( vWidth >> 1 ) + j );
								} 
								else if ( j > ( vWidth >> 1 ) )
								{
									tPoint = CWTPoint( tSrcPoint.mX - i - 1 - ( vWidth >> 1 ) + j, tSrcPoint.mY + i + 1 );
								}
								else
								{
									tPoint = CWTPoint( tSrcPoint.mX - i - 1, tSrcPoint.mY + i + 1 );
								}
								tPointList[ tPointNumber ++ ] = tPoint;
								if( tPointNumber >= tSize )
								{
									break;
								}
								
								
							}
							if ( tDirection == DIR_LEFTUP )
							{
								if ( j < ( vWidth >> 1 ) )
								{
									tPoint = CWTPoint( tSrcPoint.mX - i - 1, tSrcPoint.mY - i - 1 + ( vWidth >> 1 ) - j );
								} 
								else if ( j > ( vWidth >> 1 ) )
								{
									tPoint = CWTPoint( tSrcPoint.mX - i - 1 - ( vWidth >> 1 ) + j, tSrcPoint.mY - i - 1 );
								}
								else
								{
									tPoint = CWTPoint( tSrcPoint.mX - i - 1, tSrcPoint.mY - i - 1 );
								}
								tPointList[ tPointNumber ++ ] = tPoint;
								if( tPointNumber >= tSize )
								{
									break;
								}
								
							}
							if ( tDirection == DIR_RIGHTDOWN )
							{
								if ( j < ( vWidth >> 1 ) )
								{
									tPoint = CWTPoint( tSrcPoint.mX + i + 1, tSrcPoint.mY + i + 1 + ( vWidth >> 1 ) - j );
								} 
								else if ( j > ( vWidth >> 1 ) )
								{
									tPoint = CWTPoint( tSrcPoint.mX + i + 1 - ( vWidth >> 1 ) + j, tSrcPoint.mY + i + 1 );
								}
								else
								{
									tPoint = CWTPoint( tSrcPoint.mX + i + 1, tSrcPoint.mY + i + 1 );
								}
								tPointList[ tPointNumber ++ ] = tPoint;
								if( tPointNumber >= tSize )
								{
									break;
								}
								
							}
							if ( tDirection == DIR_RIGHTUP )
							{
								if ( j < ( vWidth >> 1 ) )
								{
									tPoint = CWTPoint( tSrcPoint.mX + i + 1, tSrcPoint.mY - i - 1 - ( vWidth >> 1 ) + j );
								} 
								else if ( j > ( vWidth >> 1 ) )
								{
									tPoint = CWTPoint( tSrcPoint.mX + i + 1 - ( vWidth >> 1 ) + j, tSrcPoint.mY - i - 1 );
								}
								else
								{
									tPoint = CWTPoint( tSrcPoint.mX + i + 1, tSrcPoint.mY - i - 1 );
								}
								tPointList[ tPointNumber ++ ] = tPoint;
								if( tPointNumber >= tSize )
								{
									break;
								}
								
							}
						}
					}
				}
			}
			break;
		case CTemplateNormalSkill::RANGE_FRONTANGLE:
			{
				if ( tDirection == DIR_DOWN || tDirection == DIR_UP || tDirection == DIR_LEFT ||  
					tDirection == DIR_RIGHT )
        		{
            		for ( int i = 0; i < tAngleLength; i ++ )
            		{
                		int tNumber = 2 * ( i + 1 ) - 1;
                		for ( int j = 0; j < tNumber; j ++ )
                		{
                    		if ( tDirection == DIR_DOWN )
                        		tPoint = CWTPoint( tSrcPoint.mX + j - ( tNumber >> 1 ), tSrcPoint.mY + i );
                    		if ( tDirection == DIR_UP )
                        		tPoint = CWTPoint( tSrcPoint.mX + j - ( tNumber >> 1 ), tSrcPoint.mY - i );
                    		if ( tDirection == DIR_LEFT )
                        		tPoint = CWTPoint( tSrcPoint.mX - i, tSrcPoint.mY + j - ( tNumber >> 1 ) );
                    		if ( tDirection == DIR_RIGHT )
                        		tPoint = CWTPoint( tSrcPoint.mX + i, tSrcPoint.mY + j - ( tNumber >> 1 ) );
                    		tPointList[ tPointNumber ++ ] = tPoint;
                            if( tPointNumber >= tSize )
                            {
								break;
                            }
                		}
            		}
        		}

        		if ( tDirection == DIR_LEFTDOWN || tDirection == DIR_LEFTUP || tDirection == DIR_RIGHTDOWN ||  
					tDirection == DIR_RIGHTUP )
        		{
            		for ( int i = 0; i < tAngleLength; i ++ )
            		{
                		for ( int j = 0; j < tAngleLength - i; j ++ )
                		{
                    		if ( tDirection == DIR_LEFTDOWN )
                        		tPoint = CWTPoint( tSrcPoint.mX - j, tSrcPoint.mY + i );
                    		if ( tDirection == DIR_LEFTUP )
                        		tPoint = CWTPoint( tSrcPoint.mX - i, tSrcPoint.mY - j );
                    		if ( tDirection == DIR_RIGHTDOWN )
                       			tPoint = CWTPoint( tSrcPoint.mX + i, tSrcPoint.mY + j );
                    		if ( tDirection == DIR_RIGHTUP )
                        		tPoint = CWTPoint( tSrcPoint.mX + j, tSrcPoint.mY - i );
                    		tPointList[ tPointNumber ++ ] = tPoint;
                            if( tPointNumber >= tSize )
                            {
                           		break;
                           	}
                		}
            		}
        		}	
			}
			break;
	}

	
	/*for ( int i = 0; i < tPointNumber; i ++ )
	{
		CMapObject::CellsEntity* tpCell = tpMapObj->GetCell( tPointList[ i ] );
		if( tpCell == NULL )
		{
			LOG_ERROR( "map", "[%s:%d] pos(%d,%d) map(%d) not valid",
				__FUNCTION__, __LINE__, tPointList[ i ].mX, tPointList[ i ].mY, tpMapObj->GetMapID() );
			continue;
		}
		CMapObject::CellsEntity::iterator tBegin = tpCell->begin();
		CMapObject::CellsEntity::iterator tEnd = tpCell->end();

		for ( ; tBegin != tEnd; ++tBegin )
		{
		CEntity* tpEntity = ( CEntity* ) CSceneObjManager::GetSingletonPtr()->GetObject( *tBegin );
		LK_ASSERT( tpEntity != NULL, continue )

		if( tpEntity->IsCharacter() == false )
		{
		continue;
		}
		vpEntityList[ tEntityNumber ++ ] = ( CEntityCharacter* )tpEntity;
		}
		}
		*/


	*vpEntityCount = tEntityNumber;

}


bool CMapModule::CreateNpc( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, CCreator* vpCreator, unsigned int& vEntityID, int vNpcID, CWTPoint vPoint, bool vDelay )
{
	if ( vpCreator == NULL )
	{
		LOG_ERROR( "map", "[ %s : %d ] [ %s ] Creator is null", __LK_FILE__, __LINE__, __FUNCTION__ );
		return false;
	}

	CMapObject* tpMapObj = CMapObjManager::GetSingleton().GetMapObject( vLineID, vMapID, vIndex );
	
	if ( tpMapObj == NULL )
	{
		LOG_ERROR( "map", "[ %s : %d ] [ %s ] tpMapObj is null, vLineID = %d, vMapID = %d", __LK_FILE__, __LINE__, __FUNCTION__, vLineID, vMapID );
		return false;
	}

	vEntityID = tpMapObj->CreateNpc( vpCreator, false, vNpcID, vPoint, vDelay );
	return true;
}



//***************************************
// Function Name : HasInStall
// Description 	 : 验证格子上是否已经有玩家在摆摊
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 2/4/2009
//***************************************

bool CMapModule::HasInStall(unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CWTPoint &rPos)
{
	CMapObject* tpMapObj = CMapObjManager::GetSingleton().GetMapObject( usLineID, usMapID, usMapIndex );
	if ( tpMapObj != NULL )
	{
		return tpMapObj->HasInStall( rPos );
	}
	return false;
}

// ***************************************************************
//  Function:		IsStaticBlock
//  Description:	判断一个点是否是静态阻挡
//	Input:			usLineID	- 线id
//					usMapID		- 地图id
//					usMapIndex	- 地图索引
//					rPos		- 坐标
//	OutPut:			无
//	Return:			ture - 有； false - 没有
//	Others:
//  Date:			11/22/2008
// 
// ***************************************************************
bool CMapModule::IsStaticBlock( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex , CWTPoint& rPos )
{
	CMapObject* tpMapObj	= CMapObjManager::GetSingleton( ).GetMapObject( usLineID, usMapID, usMapIndex );
	if ( tpMapObj != NULL )
	{
		return tpMapObj->IsStaticBlock( rPos );
	}

	return true;
}

bool CMapModule::IsSuperBlock( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex , CWTPoint& rPos )
{
	CMapObject* tpMapObj	= CMapObjManager::GetSingleton( ).GetMapObject( usLineID, usMapID, usMapIndex );
	if ( tpMapObj != NULL )
	{
		return tpMapObj->IsSuperBlock( rPos );
	}

	return true;
}

// ***************************************************************
//  Function:		GetDropPos
//  Description:	得到掉落的坐标
//	Input:			
//	OutPut:			
//	Return:			
//	Others:
//  Date:			11/22/2008
// 
// ***************************************************************
bool CMapModule::GetDropPos( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CWTPoint &vOrginPos )
{

	CMapObject* tpMapObj	= CMapObjManager::GetSingleton( ).GetMapObject( usLineID, usMapID, usMapIndex );
	if ( tpMapObj != NULL )
	{
		return tpMapObj->GetDropPos( vOrginPos );
	}

	return false;
}



int CMapModule::OnMessagePlayerPathRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return -1 );

	CMapObject* tpMap = pPlayer->GetMapObject();
	if ( tpMap == NULL )
	{
		LOG_ERROR( "map", "map line:%d mapid:%d dosn't exist", pPlayer->GetLineID(), pPlayer->GetMapID() );
		return -1 ;
	}

	CMessagePlayerPathRequest* tpPlayerPath = ( CMessagePlayerPathRequest* )pMessage->msgpara();

	// 如果玩家移动消息不在本场景，那么忽略该消息
#ifdef _FB_
	if( tpMap->GetVirtualMapID( ) != tpPlayerPath->mapid() )
	{
#endif
	if ( tpMap->GetMapID( ) != tpPlayerPath->mapid() || tpMap->GetLineID( ) != tpPlayerPath->lineid() )
	{
		LOG_WARN( "map", "[%s:%d][%s] player [%s](id=%d) 's move msg(%d,%d) dosn't match current line:%d mapid:%d", 
			__LK_FILE__, __LINE__, __FUNCTION__,
			pPlayer->GetCharNameShow(), pPlayer->GetEntityID(), 
			tpPlayerPath->mapid(), tpPlayerPath->lineid(),
			pPlayer->GetLineID(), pPlayer->GetMapID() );
		return 0 ;
	}
#ifdef _FB_
	}
#endif

	int tSpeed = ( ( CPropertyPlayer* )pPlayer->GetProperty( ) )->GetSpeed( );
//	LOG_NOTICE( "map", "[ %s : %d ][ %s ] Player( %d : %d : %s ) Player move info "
//		"new path info( map:%ld line:%ld dir:%ld src[%ld:%ld], des[%ld:%ld] ) "
//		"old pos info( map:%hd line:%hd dir:%d pox[%d:%d], t[%d:%d] ) speed(%d) time(%lld)!",
//		__LK_FILE__, __LINE__, __FUNCTION__, 
//		pPlayer->GetEntityID(), pPlayer->GetCharID(), pPlayer->GetCharNameShow(), 
//		tpPlayerPath->mapid(), tpPlayerPath->lineid(), tpPlayerPath->direction(), 
//		tpPlayerPath->srcposx(), tpPlayerPath->srcposy(), tpPlayerPath->desposx(), tpPlayerPath->desposy(),
//		pPlayer->GetMapID(), pPlayer->GetLineID(), pPlayer->GetDirection(),
//   	pPlayer->GetPosX(), pPlayer->GetPosY(), pPlayer->GetTX(), pPlayer->GetTY(), 
//      tSpeed, tpPlayerPath->timecheck());

    unsigned long long lullClientTimeNow     = tpPlayerPath->timecheck();

    // 在玩家移动后进行玩家速度检测
    if( pPlayer->GetPos( ) != CWTPoint( tpPlayerPath->srcposx(), tpPlayerPath->srcposy() ) )
    {
        // 客户端发包频率检测
    	bool lbRetForCheckClientSpeed = pPlayer->mSpeedChecker.CheckClientSpeed( pPlayer->GetEntityID(),
                                                                                 pPlayer->GetCharID(),
                                                                                 pPlayer->GetCharNameShow(),
                                                                                 lullClientTimeNow, tSpeed );
    	if( lbRetForCheckClientSpeed == false )
    	{
    		LOG_ERROR( "map", "[ %s : %d ][ %s ] Player( %d : %d : %s ) check client speed failed( %s )!",
	    		__LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetEntityID( ), pPlayer->GetCharID( ), pPlayer->GetCharNameShow(),
	    		pPlayer->mSpeedChecker.DebugInfoForClientSpeed( lullClientTimeNow  ).c_str() );
	    	SceneServer::CSceneLogic::GetSingletonPtr()->KickPlayer( pPlayer->GetCharID(), EM_KICKOUT_SPEEDCHEATING);
		    return 0;
	    }
    
	    // 客户端与服务器发包时间差检测
        bool lbRetForCheckCSTimeDiff = pPlayer->mSpeedChecker.CheckCSTimeDiff( pPlayer->GetEntityID(),
                                                                               pPlayer->GetCharID(),
                                                                               pPlayer->GetCharNameShow(),
                                                                               lullClientTimeNow );
        if( lbRetForCheckCSTimeDiff == false )
        {
            LOG_ERROR( "map", "[ %s : %d ][ %s ] Player( %d : %d : %s ) check cs time diffs failed( %s )!",
                __LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetEntityID( ), pPlayer->GetCharID( ), pPlayer->GetCharNameShow(),
                pPlayer->mSpeedChecker.DebugInfoForCSTimeDiff().c_str() );
            SceneServer::CSceneLogic::GetSingletonPtr()->KickPlayer( pPlayer->GetCharID(), EM_KICKOUT_SPEEDCHEATING2);
            return 0;
        }
    }
	
    // 如果不能让玩家控制,或者不能移动
	if ( pPlayer->GetMoveable() == false )
	{
		//LOG_WARN( "map", "player [%s:%d] is uncontolable now", pPlayer->GetCharNameShow(), pPlayer->GetEntityID() );
		CMessage tMessage;
		CMessagePlayerCanntMoveCallBack tCallBack;
		tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTICE_CANNTMOVE );
		tMessage.set_msgpara( (uint32)&tCallBack );			
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
		return -1;
	}
	
	// 处于摆摊状态，不让移动！
	CPropertyPlayer* tpProperty = ( CPropertyPlayer* )pPlayer->GetProperty( );
	if ( tpProperty->GetServiceStatus() == SERVICE_STALL ) 
	{
		return -1;
	}

	if( mMoveCheck != 0 )
	{	
		if( pPlayer->GetPosX( ) != ( int )tpPlayerPath->srcposx() 
			|| pPlayer->GetPosY( ) != (int)tpPlayerPath->srcposy( ) )
		{
			int tSpeed = ( ( CPropertyPlayer* )pPlayer->GetProperty( ) )->GetSpeed( );
			unsigned int tLastTickCount = CSceneServer::GetSingletonPtr( )->GetLastTickCount( );
			if( pPlayer->GetServerMoveTime( ) != 0 )
			{
				if( pPlayer->GetClientMoveTime( ) > ( long long )tpPlayerPath->timecheck( ) )
				{
					LOG_ERROR( "map", "[ %s : %d ][ %s ] Player( %d : %d : %s ) LastMoveTime %lld Bigger Than MoveTime"
						" %lld", __LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetEntityID( ), pPlayer->GetCharID( ), 
						pPlayer->GetCharNameShow( ), pPlayer->GetClientMoveTime( ), tpPlayerPath->timecheck( ) );
					pPlayer->SpeedCheckTeleport( );
					return 0;
				}

				if( ( int )( ( tLastTickCount - pPlayer->GetServerMoveTime( ) ) + ( ( tSpeed >> 1 ) ) ) < tSpeed )
				{
					if( ( int )( tpPlayerPath->timecheck( ) - pPlayer->GetClientMoveTime( ) ) > ( ( tSpeed ) ) )
					{
						pPlayer->SpeedCheckTeleport( );
						LOG_ERROR( "map", "[ %s : %d ][ %s ] Player( %d : %d : %s ) Speed Error LastTickCount %d, "
							"ServerMoveTime %d, ClientMoveTime %lld, Msg TimeCheck %lld, ServerSpeed %d", __LK_FILE__, 
							__LINE__, __FUNCTION__, pPlayer->GetEntityID( ), pPlayer->GetCharID( ), 
							pPlayer->GetCharNameShow( ), tLastTickCount, pPlayer->GetServerMoveTime( ), 
							pPlayer->GetClientMoveTime( ), tpPlayerPath->timecheck( ), tSpeed );

						return 0;
					}
				}
			}
			pPlayer->SetClientMoveTime( tpPlayerPath->timecheck( ) );
			pPlayer->SetServerMoveTime( tLastTickCount );	
		}
	}
	
	tpMap->PathEntity( pPlayer, CWTPoint( tpPlayerPath->srcposx(), tpPlayerPath->srcposy() ), CWTPoint( tpPlayerPath->desposx(), tpPlayerPath->desposy() ), tpPlayerPath->direction() );

	return 0;
}



// 处理停止移动消息
int CMapModule::OnMessagePlayerStopRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return -1 );

	CMapObject* tpMap = pPlayer->GetMapObject();
	if ( tpMap == NULL )
	{
		LOG_ERROR( "map", "[ %s : %d ] [ %s ] can't find mapobj LineID = %d, MapID = %d",
			__LK_FILE__, __LINE__, __FUNCTION__,
			pPlayer->GetLineID(), pPlayer->GetMapID() );
		return -1;
	}

	CMessagePlayerStopRequest* tpEntityStop = (CMessagePlayerStopRequest*) pMessage->msgpara();

	// 如果玩家停止消息不在本场景,那么忽略该消息
	if ( tpMap->GetMapID( ) != tpEntityStop->mapid() || tpMap->GetLineID( ) != tpEntityStop->lineid() )
	{
		LOG_WARN( "map", "[ %s : %d ] [ %s ] not in this map", __LK_FILE__, __LINE__, __FUNCTION__ );
		return -1;
	}

	LOG_INFO( "map", "[ %s : %d ][ %s ] Player( %d : %d : %s ) stop request, mapid( %d ) line( %d ), "
		"TargetPos( %d : %d ), Player Pos( %d : %d )", 
		__LK_FILE__, __LINE__, __FUNCTION__,
		pPlayer->GetEntityID( ), pPlayer->GetCharID( ), pPlayer->GetCharNameShow( ),
		tpEntityStop->mapid(), tpEntityStop->lineid(), 
		tpEntityStop->posx(), tpEntityStop->posy(),pPlayer->GetPosX( ), pPlayer->GetPosY( ) );

	if ( pPlayer->GetMoveable() == false || pPlayer->GetAliveStatus() == ALIVE_DIE )
	{
		//LOG_ERROR( "map", "[%s:%d][%s] Entity %d can't move or die", __LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetEntityID() );
		return -1;	
	}

	//// 创建方式设置为移动
	//pPlayer->SetCreatMethod( PLAYERCREAT_MOVE ); 
	
	CWTPoint tDesPos = CWTPoint(tpEntityStop->posx(), tpEntityStop->posy());
	
	int tLength = pPlayer->GetPos( ).Length( tDesPos );
	if( tLength <= 1 )
	{
		CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();
		if( tpMap->IsOccupiedByBlock( ( CWTPoint& )tDesPos ) == false || tpProperty->GetStateParam( STATE_FLY ).mInState == true )
		{
			// 停止的时候一定要调用一次，因为肯定和客户端差一格
			tpMap->OnEntityMove( pPlayer, tDesPos, (EDirType)0 );
		}
		else
		{
			LOG_ERROR( "map", "[ %s : %d ][ %s ] Player Path Error, MapID( %d ), TargetPos( %d : %d ) Is Block"
				", Player Pos( %d : %d )", __LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetMapID(), 
				tDesPos.mX, tDesPos.mY, pPlayer->GetPosX( ), pPlayer->GetPosY( ) );
		}
	}
	else
	{
		LOG_ERROR( "map", "[ %s : %d ][ %s ] Player( %s : %d ) Path Error, MapID( %d ) Src( %d : %d ) Des( %d"
			": %d )", __LK_FILE__,	__LINE__, __FUNCTION__, ((CEntityPlayer*)pPlayer)->GetCharNameShow(), 
			((CEntityPlayer*)pPlayer)->GetCharID(), pPlayer->GetMapID(), pPlayer->GetPosX( ), 
			pPlayer->GetPosY( ), tDesPos.mX, tDesPos.mY );

		CPropertyModule::GetSingletonPtr()->SendSyncPosNotice( pPlayer, pPlayer->GetEntityID(), 
			pPlayer->GetPosX(), pPlayer->GetPosY() );

		return 0;
	}


	// 停止的时候一定要调用一次，因为肯定和客户端差一格
	//tpMap->OnEntityMove( pPlayer, CWTPoint(tpEntityStop->posx(), tpEntityStop->posy()), (EDirType)0 );

	// 使用静态消息，提高效率
	static CMessage tMessage;
	static CMessageEntityStopNotice tStop;

	tStop.set_entityid( pPlayer->GetEntityID() );
	tStop.set_posx( pPlayer->GetPosX() );
	tStop.set_posy( pPlayer->GetPosY() );
	//tStop.set_direction( pPlayer->GetDirection() );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTICE_ENTITYSTOP );
	tMessage.set_msgpara( (uint32)&tStop );
	CMapModule::GetSingletonPtr()->MessageBroadCast( pPlayer, &tMessage );

	return 0;
}


// ***********************************************************
//  Function:		OnEntityMove
//  Description:	用于其它模块调用mapobjmanager的OnEntityMove接口
//  Input:			CEntity* pEntity
//					CWTPoint& vDesPoint
//  OutPut:			
//  Return:			void
//  Others:
//  Date:	11/05/2008
// **********************************************************
void CMapModule::OnEntityMove( CEntityCharacter* pEntity, const CWTPoint& vDesPoint )
{
	if ( pEntity == NULL )
	{
		LOG_ERROR( "map", "[ %s : %d ] [ %s ] agrument pEntity is null", __LK_FILE__, __LINE__, __FUNCTION__ );
		return;
	}
	
	CMapObject* tpMap = CMapObjManager::GetSingleton( ).GetMapObject( pEntity->GetLineID(), pEntity->GetMapID(), pEntity->GetMapIndex() );
	if ( tpMap == NULL )
	{
		LOG_ERROR( "map", "[ %s : %d ] [ %s ] map line:%d mapid:%d dosn't exist", __LK_FILE__, __LINE__, __FUNCTION__, pEntity->GetLineID(), pEntity->GetMapID() );
		return;
	}

	tpMap->OnEntityMove( pEntity, vDesPoint, pEntity->GetDirection( ) );
}

// ***************************************************************
//  Function:		GetMapName
//  Description:	根据线ID, 地图ID, 地图index 得到地图名称
//	Input:			usLineID - 线ID
//					usMapID - 地图ID
//					usMapIndex - 地图索引
//	OutPut:			
//	Return:			
//	Others:
//  Date:		11/20/2008
// 
// ***************************************************************
const char* CMapModule::GetMapName( unsigned short usLineID, unsigned short usMapID,  unsigned short usMapIndex  )
{
	CMapObject* tpMapObj	= CMapObjManager::GetSingleton( ).GetMapObject( usLineID, usMapID, usMapIndex );
	if ( tpMapObj == NULL )
	{	
		LOG_ERROR( "map", "can't find line %d, map %d when EntityCharge",  usLineID, usMapID );
		return NULL;
	}

	return tpMapObj->GetMapName();

}


// 玩家向周围可见玩家广播
//void CMapModule::MessageBroadCast( CEntityPlayer* pPlayer, CMessage* pMessage, bool bToSelf /*= false*/, bool bTeamOnly /*= false*/ )
//{
//	LK_ASSERT( (pPlayer != NULL) && (pMessage != NULL), return );
//
//	static BYTE byMessageBuffer[ MAX_PACKAGE_LEN ] = {0};
//	unsigned short ushLen = sizeof( byMessageBuffer );
//
//	if( ClientCommEngine::ConvertMsgToClientCode3( pMessage, (void*)byMessageBuffer, ushLen ) < 0 )
//	{
//		LOG_FATAL( "default", "[%s:%d] message %d serialize too long", __FUNCTION__, __LINE__, pMessage->msghead().messageid() );
//		return ;
//	}
//
//	CEntity* pTargetEntity = NULL;
//	CSeenPlayerContainer& rSeenPlayerContainer = pPlayer->GetSeenPlayersContainer();
//
//	CSeenPlayerContainer::SeenPlayerVector::iterator it = rSeenPlayerContainer.mPlayers.begin();
//	for ( ; it != rSeenPlayerContainer.mPlayers.end(); )
//	{
//		pTargetEntity = ( CEntity* ) CSceneObjManager::GetSingletonPtr()->GetObject( *it );
//		if( pTargetEntity == NULL )
//		{
//			LOG_ERROR( "map", "[%s:%d] can't find player %d in player %d's pos(%d,%d), map(%d) viewlist", 
//				__FUNCTION__, __LINE__, *it, pPlayer->GetEntityID(), pPlayer->GetPosX(), pPlayer->GetPosY(),
//				pPlayer->GetMapID() );
//		
//			rSeenPlayerContainer.mPlayers.erase( it++ );
//
//			continue;
//		}
//
//		++it;
//
//		if( pTargetEntity->IsPlayer() == false )
//		{
//			continue;
//		}
//
//		if( bTeamOnly && ( pPlayer->IsTeamTo( pTargetEntity ) ==  false ))
//		{	
//			continue;
//		}
//
//		if( ! bToSelf && ( pTargetEntity == pPlayer ) )
//		{
//			continue;
//		}
//
//		CSceneLogic::GetSingletonPtr()->Send2Player( (CEntityPlayer*)pTargetEntity, byMessageBuffer, ushLen, pMessage ) ;
//	}
//
//	// 发给自己
//	if( bToSelf == true )
//	{
//		CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, byMessageBuffer, ushLen, pMessage ) ;
//	}
//
//}

// NPC向周围可见玩家广播
//void CMapModule::MessageBroadCast( CEntityNpc* pNpc, CMessage* pMessage )
//{
//	LK_ASSERT( pNpc != NULL && pMessage != NULL, return );
//
//	static BYTE byMessageBuffer[ MAX_PACKAGE_LEN ] = {0};
//	unsigned short ushLen = sizeof( byMessageBuffer );
//
//	if( ClientCommEngine::ConvertMsgToClientCode3( pMessage, (void*)byMessageBuffer, ushLen ) < 0 )
//	{
//		LOG_FATAL( "default", "[%s:%d] message %d serialize too long", __FUNCTION__, __LINE__, pMessage->msghead().messageid() );
//		return ;
//	}
//
//	CEntity* pTargetEntity = NULL;
//	CSeenPlayerContainer& rSeenPlayerContainer = pNpc->GetSeenPlayersContainer();
//
//	CSeenPlayerContainer::SeenPlayerVector::iterator it = rSeenPlayerContainer.mPlayers.begin();
//	for ( ; it != rSeenPlayerContainer.mPlayers.end(); )
//	{
//		pTargetEntity = ( CEntity* ) CSceneObjManager::GetSingletonPtr()->GetObject( *it );
//		if( pTargetEntity == NULL )
//		{
//			LOG_ERROR( "map", "[%s:%d] can't find target %d in npc %d's pos(%d,%d), map(%d) templateid(%d) viewlist", 
//				__FUNCTION__, __LINE__, *it, pNpc->GetEntityID(), pNpc->GetPosX(), pNpc->GetPosY(),
//				pNpc->GetMapID(), pNpc->GetNpcID() );
//
//			rSeenPlayerContainer.mPlayers.erase( it++ );
//			continue;
//		}
//
//		++it;
//
//		if( pTargetEntity->IsPlayer() )
//			CSceneLogic::GetSingletonPtr()->Send2Player( (CEntityPlayer*)pTargetEntity, byMessageBuffer, ushLen, pMessage ) ;
//	}
//}

// 玩家附属物(只限pet,pedal,redstone)向周围可见队伍玩家广播
//void CMapModule::MessageBroadCast( CEntity* pPlayerDependency, CMessage* pMessage, bool bTeamOnly /*= false */ )
//{
//	LK_ASSERT( pPlayerDependency, return );
//
//	static BYTE byMessageBuffer[ MAX_PACKAGE_LEN ] = {0};
//	unsigned short ushLen = sizeof( byMessageBuffer );
//
//	if( ClientCommEngine::ConvertMsgToClientCode3( pMessage, (void*)byMessageBuffer, ushLen ) < 0 )
//	{
//		LOG_FATAL( "default", "[%s:%d] message %d serialize too long", __FUNCTION__, __LINE__, pMessage->msghead().messageid() );
//		return ;
//	}
//
//	CEntity* pTargetEntity = NULL;
//	CSeenPlayerContainer& rSeenPlayerContainer = pPlayerDependency->GetSeenPlayersContainer();
//
//	CSeenPlayerContainer::SeenPlayerVector::iterator it = rSeenPlayerContainer.mPlayers.begin();
//	for ( ; it != rSeenPlayerContainer.mPlayers.end(); )
//	{
//		pTargetEntity = ( CEntity* ) CSceneObjManager::GetSingletonPtr()->GetObject( *it );
//		if( pTargetEntity == NULL )
//		{
//			LOG_ERROR( "map", "[%s:%d] can't find entity id %d in entity %d's map(%d) pos(%d,%d)view list.",
//				__FUNCTION__, __LINE__, *it, pPlayerDependency->GetEntityID(), pPlayerDependency->GetMapID(),
//				pPlayerDependency->GetPosX(), pPlayerDependency->GetPosY() );
//
//			// 如果可见表里的实体已经不在了，删掉，其实是有错误发生了。
//			rSeenPlayerContainer.mPlayers.erase( it++ );
//
//			continue;
//		}
//
//		++it;
//
//		if( pTargetEntity->IsPlayer() == false )
//		{
//			continue;
//		}
//
//		// 只有pet和玩家施放的pedal会走到这里
//		if( bTeamOnly )
//		{
//			CEntity* pOwner = pPlayerDependency->GetOwner();
//			LK_ASSERT( pOwner != NULL, return );
//			if( pOwner->IsPlayer() && ( pOwner->IsTeamTo( pTargetEntity ) == true ) )
//				CSceneLogic::GetSingletonPtr()->Send2Player( (CEntityPlayer*)pTargetEntity, byMessageBuffer, ushLen, pMessage );
//		}
//		else
//		{
//			CSceneLogic::GetSingletonPtr()->Send2Player( (CEntityPlayer*)pTargetEntity, byMessageBuffer, ushLen, pMessage );
//		}
//	}
//}




void CMapModule::MessageBroadCast( CEntity* pEntity, CMessage* pMessage, bool bTeamOnly /*= false*/, bool bToSelf /* = false*/  )
{
	LK_ASSERT( (pEntity != NULL) && (pMessage != NULL), return );

	static BYTE byMessageBuffer[ MAX_PACKAGE_LEN ] = {0};
	unsigned short ushLen = sizeof( byMessageBuffer );

	if( ClientCommEngine::ConvertMsgToClientCode3( pMessage, (void*)byMessageBuffer, ushLen ) < 0 )
	{
		LOG_FATAL( "default", "[%s:%d] message %d serialize too long", __FUNCTION__, __LINE__, pMessage->msghead().messageid() );
		return ;
	}

	CEntity* pTargetEntity = NULL;
	CSeenPlayerContainer& rSeenPlayerContainer = pEntity->GetSeenPlayersContainer();

	CSeenPlayerContainer::SeenPlayerVector::iterator it = rSeenPlayerContainer.mPlayers.begin();
	for ( ; it != rSeenPlayerContainer.mPlayers.end(); )
	{
		pTargetEntity = ( CEntity* ) CSceneObjManager::GetSingletonPtr()->GetObject( *it );
		if( pTargetEntity == NULL )
		{
			LOG_ERROR( "map", "[%s:%d] can't find player %d in entity %d's pos(%d,%d), map(%d) viewlist", 
				__FUNCTION__, __LINE__, *it, pEntity->GetEntityID(), pEntity->GetPosX(), pEntity->GetPosY(),
				pEntity->GetMapID() );

			rSeenPlayerContainer.mPlayers.erase( it++ );

			continue;
		}

		++it;

		if( pTargetEntity->IsPlayer() == false )
		{
			continue;
		}

		if( bTeamOnly && ( pEntity->IsTeamTo( pTargetEntity ) ==  false ))
		{	
			continue;
		}

		if( ! bToSelf && ( pTargetEntity == pEntity ) )
		{
			continue;
		}

		CSceneLogic::GetSingletonPtr()->Send2Player( (CEntityPlayer*)pTargetEntity, byMessageBuffer, ushLen, pMessage ) ;
	}

	// 发给自己
	if( bToSelf == true && pEntity->IsPlayer() )
	{
		CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(pEntity);
		CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, byMessageBuffer, ushLen, pMessage ) ;
	}
}

// ***********************************************************
//  Function:		GetRandomPos
//  Description:	用于其它模块调用mapobjmanager的OnEntityMove接口
//  Input:			CEntity* pEntity
//					CWTPoint& vDesPoint
//  OutPut:			
//  Return:			void
//  Others:
//  Date:	11/05/2008
// **********************************************************
CWTPoint CMapModule::GetRandomPos( CEntityCharacter* pEntity )
{
	if ( pEntity == NULL )
	{
		LOG_ERROR( "map", "[ %s : %d ] [ %s ] agrument pEntity is null", __LK_FILE__, __LINE__, __FUNCTION__ );
		return CWTPoint( );
	}

	CMapObject* tpMap = CMapObjManager::GetSingleton( ).GetMapObject( pEntity->GetLineID(), pEntity->GetMapID(), pEntity->GetMapIndex() );
	if ( tpMap == NULL )
	{
		LOG_ERROR( "map", "[ %s : %d ] [ %s ] map line:%d mapid:%d dosn't exist", __LK_FILE__, __LINE__, __FUNCTION__, pEntity->GetLineID(), pEntity->GetMapID() );
		return CWTPoint( );
	}

	return CWTPoint( RAND(tpMap->GetWidth() - 1), RAND(tpMap->GetHight() - 1) );
}



//************************************
// Method:    OnMessagePetPathRequest
// FullName:  CMapModule::OnMessagePetPathRequest
// Access:    private 
// Returns:   int
// Qualifier: 处理pet移动
// Parameter: CEntityPlayer * pPlayer
// Parameter: CMessage * pMessage
//************************************
int CMapModule::OnMessagePetPathRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessagePetPathRequest* pPetPath = (CMessagePetPathRequest*)pMessage->msgpara();
	
	// 是不是自己的宠物
	if( pPetPath->entityid() != (unsigned int)pPlayer->GetPetID() )
	{
		return -1;
	}

	CEntityPet* pPet = pPlayer->GetPet();
	if( pPet == NULL )
	{
		LOG_ERROR( "map", "player %d|%s dosn't have pet %d",
			pPlayer->GetEntityID(), pPlayer->GetCharNameShow(), pPlayer->GetPetID() );
		return -1;
	}

	CMapObject* tpMap = pPet->GetMapObject();
	if ( tpMap == NULL )
	{
		LOG_ERROR( "map", "map line:%d mapid:%d dosn't exist", pPet->GetLineID(), pPet->GetMapID() );
		return -1 ;
	}

	// 如果玩家移动消息不在本场景，那么忽略该消息
	if ( tpMap->GetMapID( ) != pPetPath->mapid() || tpMap->GetLineID( ) != pPetPath->lineid() )
	{
		LOG_WARN( "map", "[%s:%d][%s] pet (id=%d) 's move msg(%d,%d) dosn't match current line:%d mapid:%d", 
			__LK_FILE__, __LINE__, __FUNCTION__,
			pPet->GetEntityID(), pPetPath->mapid(), pPetPath->lineid(),
			pPlayer->GetLineID(), pPlayer->GetMapID() );
		return 0 ;
	}

	// 如果不能让玩家控制,或者不能移动
	if ( pPet->GetContorlable( ) == false || pPet->GetMoveable() == false )
	{
		LOG_WARN( "map", "pet [%d] is uncontolable now",  pPet->GetEntityID() );
		CMessage tMessage;
		CMessagePlayerCanntMoveCallBack tCallBack;
		tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTICE_CANNTMOVE );
		tMessage.set_msgpara( (uint32)&tCallBack );
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
		return -1;
	}

	tpMap->PathEntity( pPet, CWTPoint( pPetPath->srcposx(), pPetPath->srcposy() ), CWTPoint( pPetPath->desposx(), pPetPath->desposy() ), pPetPath->direction() );

	return 0;
}



//************************************
// Method:    OnMessagePetStopRequest
// FullName:  CMapModule::OnMessagePetStopRequest
// Access:    private 
// Returns:   int
// Qualifier: 处理pet停止移动
// Parameter: CEntityPlayer * pPlayer
// Parameter: CMessage * pMessage
//************************************
int CMapModule::OnMessagePetStopRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return -1 );

	CMessagePetStopRequest* tpPetStop = (CMessagePetStopRequest*) pMessage->msgpara();

	// 是不是自己的宠物
	if( tpPetStop->entityid() != (unsigned int)pPlayer->GetPetID() )
	{
		return -1;
	}

	CEntityPet* pPet = pPlayer->GetPet();
	if( pPet == NULL )
	{
		LOG_ERROR( "map", "player %d|%s dosn't have pet %d",
			pPlayer->GetEntityID(), pPlayer->GetCharNameShow(), pPlayer->GetPetID() );
		return -1;
	}

	CMapObject* tpMap = pPet->GetMapObject();
	if ( tpMap == NULL )
	{
		LOG_ERROR( "map", "[ %s : %d ] [ %s ] can't find mapobj LineID = %d, MapID = %d",
			__LK_FILE__, __LINE__, __FUNCTION__,
			pPlayer->GetLineID(), pPlayer->GetMapID() );
		return -1;
	}


	// 如果玩家停止消息不在本场景,那么忽略该消息
	if ( tpMap->GetMapID( ) != tpPetStop->mapid() || tpMap->GetLineID( ) != tpPetStop->lineid() )
	{
		LOG_WARN( "map", "[ %s : %d ] [ %s ] not in this map", __LK_FILE__, __LINE__, __FUNCTION__ );
		return -1;
	}

	if ( pPet->GetMoveable() == false || pPet->GetAliveStatus() == ALIVE_DIE )
	{
		LOG_ERROR( "map", "[%s:%d][%s] Entity %d can't move or die", __LK_FILE__, __LINE__, __FUNCTION__, pPet->GetEntityID() );
		return -1;	
	}

	// 停止的时候一定要调用一次，因为肯定和客户端差一格
	tpMap->OnEntityMove( pPet, CWTPoint(tpPetStop->posx(), tpPetStop->posy()), (EDirType)0 );
	
	CMessage tMessage;
	CMessageEntityStopNotice tStop;

	tStop.set_entityid( pPet->GetEntityID() );
	tStop.set_posx( pPet->GetPosX() );
	tStop.set_posy( pPet->GetPosY() );

	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTICE_ENTITYSTOP );
	tMessage.set_msgpara( (uint32)&tStop );

	CMapModule::GetSingletonPtr()->MessageBroadCast( pPet, &tMessage );
	return 0;
}



// 增加异步传送
int CMapModule::AddAsynTeleport( CAsynTeleport& rAsynTele )
{
	mAsynTeleportList.insert( std::pair<int,CAsynTeleport>(rAsynTele.mTokenID,rAsynTele) );
	return 0;
}

// 查询异步传送
CAsynTeleport* CMapModule::GetAsyncTeleport( int nTeleTokenID )
{
	AsynTeleport::iterator it = mAsynTeleportList.find( nTeleTokenID );
	if( it != mAsynTeleportList.end() )
	{
		return &(it->second);
	}

	return NULL;
}

// 删除异步传送
int CMapModule::DeleteAsynTeleport( int nTeleTokenID )
{
	mAsynTeleportList.erase( nTeleTokenID );
	return 0;
}

// 玩家等级是否满足地图限制等级
bool CMapModule::IsLevelMatched( CEntityPlayer* pPlayer, int nMapID )
{
	CMapTpl* tpTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( nMapID );
	if( tpTpl != NULL )
	{
		if(  (tpTpl->GetMinLevelLmt() > 0 ) && ( pPlayer->GetLevel() < tpTpl->GetMinLevelLmt() ) )
		{
			return false;
		}
			
		if( ( tpTpl->GetMaxLevellmt() > 0 ) && ( pPlayer->GetLevel() > tpTpl->GetMaxLevellmt()) ) 
		{
			return false;
		}

		return true;
	}
	
	//TODO: 暂时屏蔽,找不到地图的话是跨服传
	return true;
}

void CMapModule::OnLaunchComplete( )
{
	CCreatorManager::GetSingleton().RefreshPedals( );
}

void CMapModule::LoadDenyTempConfigCallback(const char *pcConfigPath)
{
	CMapObjManager::GetSingletonPtr( )->ReloadDeny( );
}