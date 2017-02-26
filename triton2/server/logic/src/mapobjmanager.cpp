// ********************************************************************** //
// MapObjManager.cpp
// 地图对象管理器, 管理地图对象的加载销毁,发送消息
// 地图对象是指一个地图对象,而不是地图上面的一个"东西"
// Author : Black
// ********************************************************************** //
#include "stdafx.hpp"
#include "maptplmanager.h"
#include "mapobjmanager.h"
#include "mapmessage_pb.hxx.pb.h"
#include "proclient_svr.h"
#include "sceneobj_manager.h"
#include "teamtype.h"
#include "property.h"
#include "teammodule.h"
#include "propertymodule.h"
#include "mapmodule.h"
#include "message_maker.h"
#include "repetionmodule.h"
#include "creature_refresher.h"
#include "entity_searcher_imp.h"
#include "logservertype.h"
#include "timer_define.h"
#include "worldmodule.h"
#include "activitymodule.h"


template<> CMapObjManager* CSingleton< CMapObjManager >::spSingleton = NULL;
// ********************************************************************** //
// CMapObject
// ********************************************************************** //

int CMapObject::Initialize()
{
	mPlayers.initailize();
	mPlayerWaitInCount = 0;

	mKey = INVITED_KEY_VALUE;
	memset( mBlock, 0, sizeof( mBlock ) );
	mIndex = 0;
	mLineID = INVALID_LINE_ID;
	mMapID = INVALID_MAP_ID;
	mMapName.clear( );
	mWidth = 0;
	mHeight = 0;

#ifdef _FB_
	mEntity.initailize( );
	mRepetionID = 0;
	mVirtualMapID = 0;
	memset( mData, sizeof( mData ), 0 );

//	for( unsigned int i = 0; i < ARRAY_CNT( mData ); i++ )
//	{
//		mData[ i ] = 0;
//	}

#endif

	return 0;

}

int CMapObject::Resume()
{
	CellsEntity::share_list_ptr__ = &CSceneObjManager::GetSingletonPtr( )->mSharedCellsEntityID;
	
	return 0;
}

CMapObject::CMapObject()
{
	if ( CObj::msCreateMode )
	{
		Initialize();
	} 
	else
	{
		Resume();
	}
};


CMapObject::CMapObject( unsigned short vLineID, unsigned short vMapID ) : mLineID( vLineID ), mMapID( vMapID )
{
	Initialize( );
}

CMapObject::~CMapObject( )
{
#ifdef _FB_
	//TODO:
	if( mRepetionID != INVALID_OBJ_ID )
	{
		LOG_ERROR( "rep", "[ %s : %d ][ %s ] MAP :: Repetion( %d ) Cannt Clear", __LK_FILE__, __LINE__, __FUNCTION__ );
	}
#endif 
}

int CMapObject::LoadTpl( /*const char* pTplName*/ CMapTpl* pMapTpl  )
{
	CMapTpl* tpMapTpl = pMapTpl;

	// 构造地图格元素
	mWidth		= tpMapTpl->GetWidth();
	mHeight		= tpMapTpl->GetHeight();
	mMapName.clear();
	mMapName.append( tpMapTpl->mMapName );


	CModuleManager::GetSingletonPtr( )->CreateMap( GetLineID( ), GetMapID( ), GetMapIndex( ), tpMapTpl->mMapName );


	return SUCCESS;
}

int CMapObject::CreateAllEntity( )
{
#ifndef _FB_
	CMapTpl* tpMapTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetMapID( ) );
	LK_ASSERT_STR( tpMapTpl != NULL, return ERR_UNKNOWN, "MapID %d", GetMapID( ) );

	for ( size_t i = 0; i < tpMapTpl->mCreators.size( ); i ++ )
	{
		CCreator* tpCreator = CCreatorManager::GetSingletonPtr()->GetCreator( tpMapTpl->mCreators[ i ] );
		if( tpCreator == NULL )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ]Cannt find Creator from CreatorManager, which in MapTpl, "
				"index is %d", __LK_FILE__, __LINE__, __FUNCTION__, tpMapTpl->mCreators[ i ] );
			continue;
		}
		else
		{
			CreateNpc( tpCreator, true );
		}
	}

	return SUCCESS;
#else
	CMapTpl* tpMapTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetVirtualMapID( ) );
	LK_ASSERT_STR( tpMapTpl != NULL, return ERR_UNKNOWN, "MapID %d", GetVirtualMapID( ) );

	for ( size_t i = 0; i < tpMapTpl->mCreators.size( ); i ++ )
	{
		CCreator* tpCreator = CCreatorManager::GetSingletonPtr()->GetCreator( tpMapTpl->mCreators[ i ] );
		if( tpCreator == NULL )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ]Cannt find Creator from CreatorManager, which in MapTpl, "
				"index is %d", __LK_FILE__, __LINE__, __FUNCTION__, tpMapTpl->mCreators[ i ] );
			continue;
		}
		else
		{
			CreateNpc( tpCreator, true );
		}
	}

	return SUCCESS;
#endif
}




int CMapObject::CreateNpc( CCreator* pNpcCreator, bool vInit, int vNpcID, CWTPoint vPoint, bool vDelay )
{	
	LK_ASSERT( pNpcCreator != NULL, return -1 );

	CEntity* pNpc = NULL;

	switch( pNpcCreator->GetCreatorType()  )
	{
	case ENTITYTYPE_FUNCNPC:
		{
			CNpcCreator *tNpcCreator = (CNpcCreator*)pNpcCreator;
			if ( vInit == true && tNpcCreator->mInit == 0 )
			{
				return SUCCESS;
			}
			pNpc = ( CEntityNpc* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ENTITY_FUNCNPC );
			break;	
		}
	case ENTITYTYPE_ITEM:
		{
			pNpc = ( CEntityItem* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ENTITY_ITEM );
			break;
		}
	case ENTITYTYPE_MONEY:
		{
			pNpc = ( CEntityMoney* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ENTITY_MONEY );
			break;
		}
	case ENTITYTYPE_OGRE:
		{
			COgreCreator *tOgreCreator = (COgreCreator*)pNpcCreator;
			if ( vInit == true && tOgreCreator->mInit == 0 )
			{
				return SUCCESS;
			}
			// 活动配置是否刷
			if ( tOgreCreator->mActivity < 0 )
			{
				return SUCCESS;
			}
			// 如果配了初始不刷新并且间隔太短，不能刷
			int tTime = time(NULL);
			tOgreCreator->mCoolTime = tTime;
			if ( tOgreCreator->mFuncNpc == 0 )
			{
				pNpc = ( CEntityNpc* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ENTITY_OGRE );
			}
			else
			{
				pNpc = ( CEntityNpc* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ENTITY_FUNCNPC );
			}			
			break;
		}
	case ENTITYTYPE_PET:
		{
			pNpc = ( CEntityPet* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ENTITY_PET );
			break;
		}
	case ENTITYTYPE_PEDAL:
		{
			CPedalCreator* tpPedalCreator = ( CPedalCreator* )pNpcCreator;
			if( vInit == true && tpPedalCreator->mInit == 0 )
			{
				return SUCCESS;
			}
			pNpc = ( CEntityPedal* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ENTITY_PEDAL );
			
			break;
		}
	case ENTITYTYPE_COLLECT:
		{
			CCollectCreator *tCollectCreator = (CCollectCreator*)pNpcCreator;
			if( vInit == true && tCollectCreator->mInit == 0 )
			{
				return SUCCESS;
			}			
			// 活动配置是否刷
			if ( tCollectCreator->mActivity < 0 )
			{
				return SUCCESS;
			}
			pNpc = ( CEntityCollect* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ENTITY_COLLECT );
			break;
		}
	case ENTITYTYPE_REDSTONE:
		{
			pNpc = ( CEntityRedStone* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ENTITY_EXPREDSTONE );
			break;	
		}
	case ENTITYTYPE_BOX:
		{
			CBoxCreator* tpBoxCreator = ( CBoxCreator* )pNpcCreator;
			if( vInit == true && tpBoxCreator->mInit == 0 )
			{
				return SUCCESS;
			}
			
			pNpc = ( CEntityBox* ) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ENTITY_STOREBOX );
		}
		break;
	case ENTITYTYPE_BLOCK:
		{
			CBlockCreator* tpBlockCreator = ( CBlockCreator* )pNpcCreator;
			if( vInit == true && tpBlockCreator->mInit == 0 )
			{
				return SUCCESS;
			}

			pNpc = ( CEntityBlock* )CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ENTITY_BLOCK );
		}
		break;
	case ENTITYTYPE_GHOST:
		{
			pNpc = ( CEntityGhost *)CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ENTITY_GHOST );
		}
		break;
	default:
		{
			// TODO:
			break;
		}
	}

	LK_ASSERT( pNpc != NULL, return 0 );
	
	if( pNpc->Setup( GetLineID(), GetMapID(), GetMapIndex(), pNpcCreator, &vPoint, false, vNpcID ) != true )
	{
		LOG_ERROR( "map", "[%s:%d][%s] setup npc (tmpid=%d, index=%d) failed", 
			__LK_FILE__, __LINE__, __FUNCTION__, vNpcID, pNpcCreator->GetIndex() );

		CSceneObjManager::GetSingletonPtr()->DestroyObject( pNpc->get_id() );

		return 0;
	}

    if( pNpcCreator->GetCreatorType() == ENTITYTYPE_OGRE || pNpcCreator->GetCreatorType( ) == ENTITYTYPE_FUNCNPC
        || pNpcCreator->GetCreatorType( ) == ENTITYTYPE_COLLECT )
    {
		CEntityNpc* tpNpc = ( CEntityNpc* )pNpc;
		if( CMapObjManager::GetSingletonPtr( )->IsDeny( tpNpc->GetNpcID( ) ) == true )
		{
			CSceneObjManager::GetSingletonPtr()->DestroyObject( pNpc->get_id() );
			return -1;
		}
    } 
	
	if( pNpc->IsNpc() )
	{
		COgreCreator *tpOgreCreator = (COgreCreator*)pNpcCreator;
		COgreInfo *tpOgreInfo = CActivityModule::GetSingleton().GetActivityOgreInfo( tpOgreCreator->mRefresh1[0].mOgreID );		
		if ( tpOgreInfo != NULL )
		{
			vDelay = false;
		}
	}
	
	
	if ( vDelay == true && pNpc->IsNpc() )
	{
		// 只有怪有延迟刷新
		COgreCreator *pOgreCreator = (COgreCreator*)pNpcCreator;
		int tOgreID = pOgreCreator->mRefresh1[0].mOgreID;
		CTemplateOgre *tplOgre = (CTemplateOgre*)CDataStatic::SearchTpl( tOgreID );
		int nTimerOtherInfo[ TIMER_OTHER_INFO ] = { MESSAGE_PROPERTY, 0, 0, 0}  ;
		// 通知世界BOSS刷新
		if ( tplOgre != NULL )
		{
			if( tplOgre->mDisplayLevel == CTemplateOgre::OGRESHOW_BOSS )
			{
				int tIntParam[ 3 ] = { 0 };
				tIntParam[ 0 ] = tOgreID;
				tIntParam[ 1 ] = 10;
				tIntParam[ 2 ] = pNpc->GetMapID( );
				CPropertyModule::GetSingletonPtr()->SendEventNotify( NOTICE_PREREFRESHBOSS, pNpc->GetLineID( ), tIntParam, ARRAY_CNT(tIntParam), NULL, NULL );

				// 设置提前5分通知				
				((CEntityNpc*)pNpc)->GetTimerReciver()->SetTimer( pNpc->get_id(), TIMER_NOTIFYREFRESH_FIVE, 300000, 1, nTimerOtherInfo );
			}
		}		

		// 设置10分钟后刷怪
		nTimerOtherInfo[ 1 ] = pNpcCreator->get_id();
		((CEntityNpc*)pNpc)->GetTimerReciver()->SetTimer( pNpc->get_id(), TIMER_CREATEOGRE_TEN, 600000, 2, nTimerOtherInfo );
		pNpc->mHasCreated = false;

		return pNpc->GetEntityID();
	}
	
	CSceneLogic::GetSingleton( ).CreateEntity( pNpc, pNpcCreator );

	return pNpc->GetEntityID();	

}



// 玩家是否可以通过
bool CMapObject::CanPlayerPass( const CWTPoint& rPos )
{
	CGridBlock* tpGrid = GetGridBlock( rPos.mX, rPos.mY );
	if( tpGrid == NULL )
	{
		return false;
	}

	return tpGrid->CanPlayerPass( );
}

// NPC是否可以通过
bool CMapObject::CanNpcPass( const CWTPoint& rPos )
{
	CGridBlock* tpGrid = GetGridBlock( rPos.mX, rPos.mY );
	if( tpGrid == NULL )
	{
		return false;
	}

	if( IsBlock( rPos.mX, rPos.mY ) == true )
	{
		return false;
	}

	return tpGrid->CanNpcPass( );

}

// 是否是相对安全区域(本国不能厮杀)
bool CMapObject::IsCtrSafeRegion( const CWTPoint& rPos )
{
	CGridBlock* tpGrid = GetGridBlock( rPos.mX, rPos.mY );
	if( tpGrid == NULL )
	{
		return false;
	}

	return tpGrid->IsCtrSafeRegion( );
}

// 是否是绝对安全区域(敌国不能厮杀)
bool CMapObject::IsAbsSafeRegion( const CWTPoint& rPoint )
{
	CGridBlock* tpGrid = GetGridBlock( rPoint.mX, rPoint.mY );
	if( tpGrid == NULL )
	{
		return false;
	}

	return tpGrid->IsAbsSafeRegion( );
}

// 是否是静态阻挡
bool CMapObject::IsStaticBlock( const CWTPoint& rPos )
{
	CGridBlock* tpGrid = GetGridBlock( rPos.mX, rPos.mY );
	if( tpGrid == NULL )
	{
		return true;
	}

	return tpGrid->IsStaticBlock( );
}

// 是否是摆摊安全区
bool CMapObject::IsStallSaftyArea(const CWTPoint& rPoint )
{
	CGridBlock* tpGrid = GetGridBlock( rPoint.mX, rPoint.mY );
	if( tpGrid == NULL )
	{
		return false;
	}
	return tpGrid->IsStallSafty();
}

// 是否是高级阻挡
bool CMapObject::IsSuperBlock( const CWTPoint& rPos )
{
	CGridBlock* tpGrid = GetGridBlock( rPos.mX, rPos.mY );
	if( tpGrid == NULL )
	{
		return true;
	}

	return tpGrid->IsSuperBlock( );
}


int CMapObject::CreateEntity( CEntity* pSrcEntity, CCreator* pCreator , bool vFirst )
{
	int tRtn = SUCCESS;

	LK_ASSERT( pSrcEntity != NULL, return ERR_INVALID_PARAMETER );

	CWTPoint tTopLeft;
	CWTPoint TBottomRight;
	GetViewRect( pSrcEntity, tTopLeft, TBottomRight );

	int tListID = 0;
	if ( pCreator != NULL )
	{
		tListID = ( ( pCreator->GetCreatorType() == ENTITYTYPE_ITEM ) ? ( (CItemCreator*) pCreator )->mListID : 0 );
	}

	pSrcEntity->SetLineID( GetLineID( ) );
	pSrcEntity->SetMapID( GetMapID( ) );
	pSrcEntity->SetIndex( GetMapIndex( ) );


	if( pSrcEntity->IsPlayer() )
	{
		tRtn = AddPlayerIntoMap( ( CEntityPlayer* )pSrcEntity, vFirst ); 
		if( pSrcEntity->GetMapID( ) != GetMapID( ) ) 
		{
			return tRtn;
		}

		int tBeginnerMapID = 0;

		if( tRtn != SUCCESS )
		{
			LOG_ERROR( "map", "[ %s : %d ][ %s ]AddPlayerIntoMap Error, rtn = %d, PlayerCount( %d : %d )", 
				__LK_FILE__, __LINE__, __FUNCTION__, GetPlayerCountMax( ), GetPlayerCount( ), tRtn );
			return tRtn;
		}

		CTemplateBeginner* tpBeginner = (CTemplateBeginner*) CDataStatic::GetTemp( TEMP_BEGINNER );
		if ( tpBeginner != NULL )
		{
			tBeginnerMapID = tpBeginner->mMapID;
		}

		if( tBeginnerMapID == GetMapID( ) )
		{
			pSrcEntity->SetVisibility( VISIBLITY_SELF );
		}
		else
		{
			pSrcEntity->SetVisibility( VISIBLITY_ALL );
		}

		VisibleViewEntityChecker tVisibleChecker( (CEntityPlayer*)pSrcEntity );
		MapRegionEntitySearcher< VisibleViewEntityChecker > tMapRegionSearcher( tVisibleChecker,tTopLeft,TBottomRight );
		tMapRegionSearcher.Visit(this);
		
		// 向周围玩家广播创建自己(player)
		if( GetMapID( ) != tBeginnerMapID )
		{
			SendCreatePlayer2Players( &tVisibleChecker.m_Players, (CEntityPlayer*)pSrcEntity );
		}

		// 向自己发送创建npc
		SendCreateNpc2Player( (CEntityPlayer*)pSrcEntity, &tVisibleChecker.m_Npcs, tListID );

		if( GetMapID( ) != tBeginnerMapID )
		{
			// 向自己发送创建player
			SendCreatePlayer2Player( (CEntityPlayer*)pSrcEntity, &tVisibleChecker.m_Players );
		}
	}
	else
	{
		// 包含了逻辑处理，不能放在里面
		VisibleViewPlayerChecker tVisibleChecker( pSrcEntity );
		MapRegionEntitySearcher< VisibleViewPlayerChecker > tMapRegionSearcher( tVisibleChecker, tTopLeft, TBottomRight );
		tMapRegionSearcher.Visit(this);

		if ( pCreator == NULL || pCreator->GetShow( ) == true )
		{
			// 向周围玩家广播创建自己(npc)
			SendCreateNpc2Players( &tVisibleChecker.m_Players, pSrcEntity, 0  );
		}		

#ifdef _FB_
		mEntity.push_back( pSrcEntity->get_id( ) );
#endif
	}

	AddEntityToCell( pSrcEntity );

	return tRtn;
}

void CMapObject::FindPath( const CWTPoint& rSrcPos, const CWTPoint& rDesPos, std::vector< CWTPoint >& rPathList )
{
	CWTPoint tMidPoint = rSrcPos;
	for ( int i = rSrcPos.mX; i < rDesPos.mX; i ++ )
	{
		tMidPoint.mX = i;
		if ( ::abs( tMidPoint.mX - rDesPos.mX ) == ::abs( tMidPoint.mY - rDesPos.mY ) )
		{
			rPathList.push_back( tMidPoint );
			rPathList.push_back( rDesPos );
			break;
		}
	}
}


// ***************************************************************
//  Function:		PathEntity
//  Description:	处理Entity寻路
//	Input:			pEntity - 玩家实体； rSrcPos - 源坐标； rDesPos - 目的
//	OutPut:			无
//	Return:			无
//	Others:
//  Date:			10/27/2008
// 
// ***************************************************************
void CMapObject::PathEntity( CEntityCharacter* pEntity, const CWTPoint& rSrcPos, const CWTPoint& rDesPos, int vDirection )
{
	LK_ASSERT( pEntity != NULL, return );

	// 非玩家类型实体
	if ( pEntity->IsNpc() )
	{
		// 怪物停止移动消息
		if( rDesPos.mX != -1 && rDesPos.mY != -1 ) 
		{
			if( pEntity->IsPet() )
			{
				//召唤兽移动距离不能超过1格
				int tLength = pEntity->GetPos( ).Length( rSrcPos );

				if( tLength > 1 )
				{
					return ;
				}
			}

			OnEntityMove( pEntity, rDesPos, (EDirType)vDirection );
		}
		pEntity->SetDirection( (EDirType)vDirection );
		CMapModule::GetSingletonPtr()->MessageBroadCast( pEntity, BuildEntityPathMessage(pEntity, rSrcPos, rDesPos) );

	}
	else // 玩家类实体
	{
		CEntityPlayer* pPlayer = (CEntityPlayer*)pEntity;

		if( pPlayer->GetPos( ) != rSrcPos )
		{
			int tLength = pPlayer->GetPos( ).Length( rSrcPos );

			if( tLength <= 1 )
			{
				CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();
				if( IsOccupiedByBlock( ( CWTPoint& )rSrcPos ) == false || tpProperty->GetStateParam( STATE_FLY ).mInState == true )
				{
					OnEntityMove( pPlayer, rSrcPos, (EDirType)vDirection );
				}
				else
				{
					LOG_ERROR( "map", "[ %s : %d ][ %s ] Player Path Error, MapID( %d ), TargetPos( %d : %d ) Is Block"
						", Player Pos( %d : %d )", __LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetMapID(), 
						rSrcPos.mX, rSrcPos.mY, pPlayer->GetPosX( ), pPlayer->GetPosY( ) );
				}
			}
			else
			{
				LOG_ERROR( "map", "[ %s : %d ][ %s ] Player( %s : %d ) Path Error, MapID( %d ) Src( %d : %d ) Des( %d"
					": %d )", __LK_FILE__,	__LINE__, __FUNCTION__, pPlayer->GetCharNameShow(), 
					pPlayer->GetCharID(), pPlayer->GetMapID(), pPlayer->GetPosX( ), 
					pPlayer->GetPosY( ), rDesPos.mX, rDesPos.mY );

				CPropertyModule::GetSingletonPtr()->SendSyncPosNotice( pPlayer, pPlayer->GetEntityID(), 
					pPlayer->GetPosX(), pPlayer->GetPosY() );

				return ;
			}

			// 如果移动成功
			if( pPlayer->GetPos( ) == rSrcPos )
			{
				pPlayer->SetDirection( (EDirType)vDirection );

				CMapModule::GetSingletonPtr()->MessageBroadCast( pPlayer, 
					BuildEntityPathMessage( pPlayer, rSrcPos, rDesPos), false, true );

				pPlayer->SetMoveDistance( tLength );
				pPlayer->SpeedCheck( );
			}
		}

	}

	// 设置目标坐标
	( ( CEntityCharacter* )pEntity )->SetTargetPos( rDesPos );
	
	return ;
}


// ***************************************************************
//  Function: 	MoveEntity   
//  Description:实体移动
//  Date: 		10/24/2008
// 
// ***************************************************************
void CMapObject::MoveEntity( CEntityCharacter* pEntity, const CWTPoint& rPoint )
{
	LK_ASSERT( pEntity != NULL, return );

	// 如果实体已经死亡
	if ( pEntity->GetAliveStatus() == ALIVE_DIE )
	{
		LOG_WARN( "map", "[%s:%d][%s] Entity %d can't move or die", __LK_FILE__, __LINE__, __FUNCTION__, pEntity->GetEntityID() );
		return;	
	}

	// 非玩家实体
	if ( pEntity->IsOgre() )
	{
		/*if ( pEntity->GetEntityID() != mpLocks[ rPoint.mY * mWidth + rPoint.mX ] )
		{
			return;
		}
		mpLocks[ rPoint.mY * mWidth + rPoint.mX ] = 0;*/
	}		

	// 非生物类型， 错误
	if( pEntity->IsItem() || pEntity->IsMoney() )
	{
		LOG_ERROR( "map", "[ %s : %d  ][ %s ] Entity %d is not an charactor property", 
				__LK_FILE__, __LINE__, __FUNCTION__, pEntity->GetEntityID() );
		return ;
	}

	// property不可能为NULL
	CPropertyCharacter* tpProperty = ( CPropertyCharacter* )pEntity->GetProperty();
	if( tpProperty->Moveable() ==  false )
	{
		LOG_ERROR( "map", "[ %s : %d  ][ %s ] entity %d cann't move",__LK_FILE__, __LINE__, __FUNCTION__, pEntity->GetEntityID() );
		return;	
	}

	OnEntityMove( pEntity, rPoint, DIR_LEFTUP );
}

// ***************************************************************
//  Function:	 DestroyEntity
//  Description: 销毁实体接口
//	Input:			
//	OutPut:			
//	Return:			
//	Others:
//  Date:		02/21/2009
//				现在销毁玩家时不用发送销毁他周围实体的消息给玩家自己，客户端会自己搞定
// ***************************************************************
void CMapObject::DestroyEntity( CEntity* pEntity )
{
	int tRtn = SUCCESS;

	LK_ASSERT( pEntity != NULL, return );

	if( RemoveEntityFromCell( pEntity ) != true )
	{
		LOG_FATAL( "map", "[%s:%d] can't remove entity %d pos(%d,%d) map(%d)",
			__FUNCTION__, __LINE__, pEntity->GetEntityID(), pEntity->GetPosX(), pEntity->GetPosY(), pEntity->GetMapID() );
		return ;	
	}
	
	CWTPoint tTopLeft; 
	CWTPoint tBottomRight;
	GetViewRect( pEntity, tTopLeft, tBottomRight );

	if( pEntity->IsPlayer() )
	{
		tRtn = RemovePlayerFromMap( ( CEntityPlayer* )pEntity );
		if( tRtn != SUCCESS )
		{
			LOG_ERROR( "map", "[ %s : %d ][ %s ]RemovePlayerFromMap Fail, ErrorCode is %d", __LK_FILE__, __LINE__,
				__FUNCTION__, tRtn );
		}

		VisibleViewEntityDeleter tVisibleChecker( (CEntityPlayer*)pEntity );
		MapRegionEntitySearcher< VisibleViewEntityDeleter > tMapRegionSearcher( tVisibleChecker,tTopLeft,tBottomRight);
		tMapRegionSearcher.Visit(this);

		// 给玩家自己发送销毁可见实体消息
		SendDestroyEntity2Player( (CEntityPlayer*)pEntity, &tVisibleChecker.m_Players, &tVisibleChecker.m_Npcs );
			
		// 给看见玩家的玩家发送销毁该玩家消息
		SendDestroyEntity2Players( &tVisibleChecker.m_Players, pEntity );
		
		// 离开副本需要记录日志
		/*
		if( IS_REPETION_LINE_ID( GetLineID( ) ) == true )
			CSceneLogic::GetSingleton().Send2Log( 
					BuildExitFBLogMessage( (CEntityPlayer*)pEntity, GetKey(), 0 ), 
					EIHANDLE_SIX, ((CEntityPlayer*)pEntity)->GetCharID() );
		*/
	}
	else
	{

		VisibleViewPlayerDeleter tVisibleChecker( pEntity );
		MapRegionEntitySearcher< VisibleViewPlayerDeleter > tMapRegionSearcher( tVisibleChecker,tTopLeft,tBottomRight);
		tMapRegionSearcher.Visit(this);

		// 给看见的玩家发送销毁消息
		SendDestroyEntity2Players( &tVisibleChecker.m_Players, pEntity );

#ifdef _FB_
		for( int i = mEntity.size( ) - 1; i >= 0; i-- )
		{
			if( ( unsigned int )mEntity[ i ] == pEntity->GetEntityID( ) )
			{
				mEntity.erase( i );
				break;
			}
		}
#endif
	}

}

void CMapObject::OnTimer( unsigned int vTickOffset )
{
}


// ***************************************************************
//  Function:		OnEntityMoveIn
//  Description:	触发实体移动进入事件，1.触发传送 2.触发陷阱
//	Input:			pMapEntity - 进入的地图实体
//					rOldPos - 旧坐标
//					vOldDirection - 旧朝向
//	OutPut:			无
//	Return:			无
//	Others:
//  Date:			10/25/2008
//					客户端可以自行销毁超出视野的对象，服务器管好自己的就行了，不必再发消息
// 
// ***************************************************************
void CMapObject::OnEntityMove( CEntityCharacter* pEntity, const CWTPoint& rPos, EDirType vDirection )
{
	LK_ASSERT( pEntity != NULL , return );

	if ( pEntity->GetPosX() == rPos.mX && pEntity->GetPosY() == rPos.mY )
	{
		//LOG_WARN( "map", "onentitymove dst pos is src pos (%d,%d)", pEntity->GetPosX(), pEntity->GetPosY() );
		return ;
	}

	if ( IsValidPos( rPos ) == false  )
	{
		LOG_ERROR( "map", "Can't find Cell, invalid Pos(%d,%d)", rPos.mX, rPos.mY );
		return ;
	}

	if ( pEntity->IsPlayer() )
	{
		CEntityPlayer *tpPlayer = (CEntityPlayer*)pEntity;
		if ( tpPlayer->CanRemainFly( ) == false  )
		{
			CMessage tMessage;
			CMessageEntityPosReSetCallBack tResetPosMsg;

			tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTICE_ENTITYPOSRESET );
			tResetPosMsg.set_entityid( pEntity->GetEntityID() );
			tResetPosMsg.set_x( pEntity->GetPosX() );
			tResetPosMsg.set_y( pEntity->GetPosY() );

			tMessage.set_msgpara( (uint32)(&tResetPosMsg) );	
			CSceneLogic::GetSingletonPtr()->Send2Player( pEntity, &tMessage );

			LOG_ERROR( "map", "[%s:%d][%s] entity %d move, map(%d) pos(%d,%d) is static block", __LK_FILE__, __LINE__, 
				__FUNCTION__, pEntity->GetEntityID(), GetMapID(), rPos.mX, rPos.mY );

			return;
		}
	}	

	// 目的地不能是静态阻挡
	if( IsStaticBlock( rPos ) == true || IsBlock( rPos.mX, rPos.mY ) )
	{
		if( pEntity->IsPlayer() )
		{
//			CEntityPlayer *tpPlayer = (CEntityPlayer*)pEntity;
			if ( pEntity->CanFly() == false )
			{
				CMessage tMessage;
				CMessageEntityPosReSetCallBack tResetPosMsg;

				tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTICE_ENTITYPOSRESET );
				tResetPosMsg.set_entityid( pEntity->GetEntityID() );
				tResetPosMsg.set_x( pEntity->GetPosX() );
				tResetPosMsg.set_y( pEntity->GetPosY() );

				tMessage.set_msgpara( (uint32)(&tResetPosMsg) );	
				CSceneLogic::GetSingletonPtr()->Send2Player( pEntity, &tMessage );

				LOG_ERROR( "map", "[%s:%d][%s] entity %d move, map(%d) pos(%d,%d) is static block", __LK_FILE__, __LINE__, 
					__FUNCTION__, pEntity->GetEntityID(), GetMapID(), rPos.mX, rPos.mY );

				return;
			}			
			
		}
		else
		{
			if ( pEntity->CanFly() == false )
			{
				return;
			}			
		}		
	}


	CWTPoint tTopLeft, tBottomRight;
	GetViewRect( pEntity, tTopLeft, tBottomRight );
	CWTRect tSrcRect( tTopLeft, tBottomRight );

	if( RemoveEntityFromCell( pEntity ) != true )
	{
		LOG_ERROR( "map", "Can't find MapEntityID by Entity Id=%d, Pos(%d,%d)", pEntity->GetEntityID(), pEntity->GetPosX(), pEntity->GetPosY());
		return;
	}

	// 设置新坐标
	CWTPoint tOldPos		= pEntity->GetPos( );

	pEntity->SetPosX( rPos.mX );
	pEntity->SetPosY( rPos.mY );


	GetViewRect( pEntity, tTopLeft, tBottomRight );
	CWTRect tDesRect( tTopLeft, tBottomRight );

	// 如果走到了目标
	if ( pEntity->GetTargetPos( ) == rPos )
		pEntity->SetHasTarget( false );

	// 通知看到自己的客户端, 建立自己
	CWTRect tRectList[ 2 ];

	int tNumber = tSrcRect.SubtractRect( tDesRect, tRectList );
	if( tNumber > 0 )
	{
		if( pEntity->IsPlayer() )
		{	
			// 先访问第一个矩形
			VisibleViewEntityChecker tVisibleChecker( (CEntityPlayer*)pEntity );
			MapRegionEntitySearcher< VisibleViewEntityChecker > tMapRegionSearcher( tVisibleChecker,tRectList[0].TopLeft(),tRectList[0].BottomRight());
			tMapRegionSearcher.Visit(this);

			int tBeginnerMapID = 0;
			CTemplateBeginner* tpBeginner = (CTemplateBeginner*) CDataStatic::GetTemp( TEMP_BEGINNER );
			if ( tpBeginner != NULL )
			{
				tBeginnerMapID = tpBeginner->mMapID;
			}
			
			// 设置创建方式为移动创建
			( (CEntityPlayer*)pEntity )->SetCreatMethod( PLAYERCREAT_MOVE );


			// 如果有第二个矩形，访问它; 不会有多于2个的情况
			if( tNumber == 2 )
			{
				tMapRegionSearcher.m_TopLeft = tRectList[1].TopLeft();
				tMapRegionSearcher.m_BottomRight = tRectList[1].BottomRight();
				tMapRegionSearcher.Visit(this);
			}

			if( GetMapID( ) != tBeginnerMapID )
			{
				// 向周围玩家广播创建自己(player)
				SendCreatePlayer2Players( &tVisibleChecker.m_Players, (CEntityPlayer*)pEntity );
			}	 
		
			
			// 向自己发送创建npc
			SendCreateNpc2Player( (CEntityPlayer*)pEntity, &tVisibleChecker.m_Npcs );

			if( GetMapID( ) != tBeginnerMapID )
			{
				// 向自己发送创建player
				SendCreatePlayer2Player( (CEntityPlayer*)pEntity, &tVisibleChecker.m_Players );
			}

		}
		else
		{
			VisibleViewPlayerChecker tVisibleChecker( pEntity );
			MapRegionEntitySearcher< VisibleViewPlayerChecker > tMapRegionSearcher( tVisibleChecker,tRectList[0].TopLeft(),tRectList[0].BottomRight());
			tMapRegionSearcher.Visit(this);

			if( tNumber == 2 )
			{
				tMapRegionSearcher.m_TopLeft = tRectList[1].TopLeft();
				tMapRegionSearcher.m_BottomRight = tRectList[1].BottomRight();
				tMapRegionSearcher.Visit(this);
			}

			// 向周围玩家广播创建自己(npc)
			SendCreateNpc2Players( &tVisibleChecker.m_Players, pEntity );
		}
	}


	//// 通知看不到自己的客户端, 删除自己
	tNumber = tDesRect.SubtractRect( tSrcRect, tRectList );
	if ( tNumber > 0 )
	{
		if( pEntity->IsPlayer() )
		{
			VisibleViewEntityDeleter tVisibleChecker( (CEntityPlayer*)pEntity );
			MapRegionEntitySearcher< VisibleViewEntityDeleter > tMapRegionSearcher( tVisibleChecker,tRectList[0].TopLeft(),tRectList[0].BottomRight());
			tMapRegionSearcher.Visit(this);

			if( tNumber == 2 )
			{
				tMapRegionSearcher.m_TopLeft = tRectList[1].TopLeft();
				tMapRegionSearcher.m_BottomRight = tRectList[1].BottomRight();
				tMapRegionSearcher.Visit(this);
			}

			// 给玩家自己发送销毁可见实体消息
			SendDestroyEntity2Player( (CEntityPlayer*)pEntity, &tVisibleChecker.m_Players, &tVisibleChecker.m_Npcs );

			// 给看见玩家的玩家发送销毁该玩家消息
			SendDestroyEntity2Players( &tVisibleChecker.m_Players, pEntity );
		}
		else
		{
			VisibleViewPlayerDeleter tVisibleChecker( pEntity );
			MapRegionEntitySearcher< VisibleViewPlayerDeleter > tMapRegionSearcher( tVisibleChecker,tRectList[0].TopLeft(),tRectList[0].BottomRight());
			tMapRegionSearcher.Visit(this);

			if( tNumber == 2 )
			{
				tMapRegionSearcher.m_TopLeft = tRectList[1].TopLeft();
				tMapRegionSearcher.m_BottomRight = tRectList[1].BottomRight();
				tMapRegionSearcher.Visit(this);
			}

			// 给看见的玩家发送销毁消息
			SendDestroyEntity2Players( &tVisibleChecker.m_Players, pEntity );
		}

	}

	// 添加到新地图格
	AddEntityToCell( pEntity );
}


/************************************************************************/
/*    CMapObjManager                                                                   */
/************************************************************************/

CSharedMem* CMapObjManager::mShmPtr = NULL;

CMapObjManager::CMapObjManager()
{

	if ( mShmPtr->GetInitMode( ) == SHM_INIT )
	{
#ifndef _FB_
		mMapObjects.initailize ();	  		
#endif
			
		mDenyList.clear();		
#ifdef _FB_
		unsigned int i = 0;
		mFreeMapObjQueue.initailize( );
		for( i = 0; i < ARRAY_CNT( mMapObjID ); i++ )
		{
			mMapObjID[ i ] = INVALID_OBJ_ID;
			if( i != INVALID_MAP_INDEX )
			{
				mFreeMapObjQueue.push( i );
			}
		}
#endif
	} 
	else
	{
		;
	}
	
}


CMapObjManager::~CMapObjManager( )
{
}

void* CMapObjManager::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CMapObjManager::operator delete( void* pointer ) { }

void* CMapObjManager::operator new(size_t size, const char* file, int line)
{
	return operator new( size );
}
void CMapObjManager::operator delete(void* pointer, const char* file, int line) {}

CMapObject* CMapObjManager::CreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName, KEY_TYPE vKey )
{

	CMapTpl* tpMapTpl = CMapTplManager::GetSingleton( ).CreateMapTpl( pTplName, vMapID, vLineID );
	if ( tpMapTpl == NULL )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ]LoadMap Err, MapID is %d", __LK_FILE__, __LINE__, __FUNCTION__, vMapID );
		return NULL;
	}

#ifndef _FB_
	int tMapKey = MakeKey( vLineID, vMapID, vIndex );
	
	CMapObject* tpMapObj = NULL;

	if( ( tpMapTpl->GetWidth() <= CMapObjectS::msWidth ) && ( tpMapTpl->GetHeight() <= CMapObjectS::msHeight) )
	{
		tpMapObj = ( CMapObject* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_MAP_S_OBJECT );
	}
	else if( (tpMapTpl->GetWidth() <= CMapObjectM::msWidth) && (tpMapTpl->GetHeight() <= CMapObjectM::msHeight) )
	{
		tpMapObj = ( CMapObject* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_MAP_M_OBJECT );
	}
	else if( (tpMapTpl->GetWidth() <= CMapObjectL::msWidth) && (tpMapTpl->GetHeight() <= CMapObjectL::msHeight) )
	{
		tpMapObj = ( CMapObject* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_MAP_L_OBJECT );
	}
	else
	{
		LOG_FATAL( "default", "MapSize %d * %dto large",  tpMapTpl->GetWidth(), tpMapTpl->GetHeight() );
		return NULL;
	}

	if( tpMapObj == NULL )
	{
		LOG_FATAL( "default", "map %s size %d * %d create failed", tpMapTpl->mMapName, tpMapTpl->GetWidth(), tpMapTpl->GetHeight() );
		return NULL;
	}

	LOG_NOTICE( "default", "map %d size %d * %d create success ", vMapID, tpMapTpl->GetWidth(), tpMapTpl->GetHeight() );

	tpMapObj->SetLineID( vLineID );
	tpMapObj->SetMapID( vMapID );
	tpMapObj->SetMapIndex( vIndex );
	
	mMapObjects.insert( std::pair< int, int >( tMapKey, tpMapObj->get_id() ) );

	tpMapObj->LoadTpl( tpMapTpl );
	tpMapObj->CreateAllEntity( );

	return tpMapObj;
#else
	int tIndex = INVALID_MAP_INDEX;
	int tRtn = SUCCESS;

	if( mFreeMapObjQueue.empty( ) == false )
	{
		tIndex = mFreeMapObjQueue.front( );
	}
	else
	{
		return NULL;
	}
	

	CMapObject* tpMapObj = NULL;
	if( ( tpMapTpl->GetWidth() <= CMapObjectS::msWidth ) && ( tpMapTpl->GetHeight() <= CMapObjectS::msHeight) )
	{
		tpMapObj = ( CMapObject* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_MAP_S_OBJECT );
	}
	else if( (tpMapTpl->GetWidth() <= CMapObjectM::msWidth) && (tpMapTpl->GetHeight() <= CMapObjectM::msHeight) )
	{
		tpMapObj = ( CMapObject* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_MAP_M_OBJECT );
	}
	else if( (tpMapTpl->GetWidth() <= CMapObjectL::msWidth) && (tpMapTpl->GetHeight() <= CMapObjectL::msHeight) )
	{
		tpMapObj = ( CMapObject* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_MAP_L_OBJECT );
	}
	else
	{
		LOG_FATAL( "default", "MapSize %d * %dto large",  tpMapTpl->GetWidth(), tpMapTpl->GetHeight() );
		return NULL;
	}


	if( tpMapObj == NULL )
	{
		LOG_FATAL( "default", "map %s size %d * %d create failed", tpMapTpl->mMapName, tpMapTpl->GetWidth(), tpMapTpl->GetHeight() );
		return NULL;
	}

	tpMapObj->SetLineID( vLineID );
	tpMapObj->SetMapID( vMapID );
	tpMapObj->SetMapIndex( tIndex );	
	tpMapObj->SetKey( vKey );

	tpMapObj->mVirtualMapID  = vMapID;

	mMapObjID[ tIndex ] = tpMapObj->get_id( );

	tRtn = tpMapObj->LoadTpl( tpMapTpl );
	if( tRtn == SUCCESS )
	{
		if( CRepetionModule::GetSingletonPtr( )->IsRepetionMap( vMapID ) == false )
		{
			tpMapObj->CreateAllEntity( );
		}

		mFreeMapObjQueue.pop( );
//		CRepetionModule::GetSingletonPtr( )->OnCreateMap( vLineID, vMapID, tIndex, pTplName ); 
	}
	else
	{
		CMapObjManager::GetSingletonPtr( )->DestroyMapObject( tpMapObj );
		mMapObjID[ tIndex ] = INVALID_OBJ_ID;
		mFreeMapObjQueue.push( tIndex );
		return NULL;
	}

	LOG_INFO( "map", "[ %s : %d ][ %s ] Create MapObject( %d : %d : %d : %lld )", __LK_FILE__, __LINE__, __FUNCTION__,
		tpMapObj->get_id( ), tpMapObj->GetMapID( ), tpMapObj->GetMapIndex( ), tpMapObj->GetKey( ) );

	return tpMapObj;
 
#endif
}

CMapObject* CMapObjManager::GetMapObject( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex,
	KEY_TYPE vKey )
{
#ifndef _FB_
	int tMapKey = MakeKey( vLineID, vMapID, vIndex );
	MapObjMap::iterator tFindIter = mMapObjects.find( tMapKey );
	if ( tFindIter != mMapObjects.end( ) )
	{
		return  ( CMapObject* ) CSceneObjManager::GetSingletonPtr()->GetObject( tFindIter->second );
	}

	return NULL;
#else
	unsigned int i = 0;
	CMapObject* tpObj = NULL;
	if( vIndex != INVALID_MAP_INDEX 
		&& vIndex < ARRAY_CNT( mMapObjID )  
	)
	{
		tpObj = ( CMapObject* )CSceneObjManager::GetSingletonPtr( )->GetObject( mMapObjID[ vIndex ] );
		if( tpObj != NULL )
		{
			if( ( tpObj->GetLineID( ) == vLineID ) && ( tpObj->GetMapID( ) == vMapID ) )
			{
				return tpObj;
			}
			else
			{
				tpObj = NULL;
			}
		}
	}
	else if( vIndex == INVALID_MAP_INDEX /*&& vLineID != _REPETION_LINE_ID_*/ )
	{
		for( i = 1; i <= ARRAY_CNT( mMapObjID ); i++ )
		{
			tpObj = ( CMapObject* )CSceneObjManager::GetSingletonPtr( )->GetObject( mMapObjID[ i ] );
			if( tpObj != NULL )
			{
				if( ( tpObj->GetLineID( ) == vLineID ) && ( tpObj->GetMapID( ) == vMapID ) )
				{
					if( vKey != INVITED_KEY_VALUE )
					{
						if( vKey == tpObj->GetKey( ) )
						{
							return tpObj;
						}
					}
					else
					{
						return tpObj;
					}
				}		
			}
			tpObj = NULL;
		}
	}

	return tpObj;
#endif
}

CMapObject* CMapObjManager::GetMapObject( unsigned int vObjID )
{
	CMapObject* tObj = ( CMapObject* )CSceneObjManager::GetSingletonPtr( )->GetObject( vObjID );
	return tObj;
}

int CMapObjManager::DestroyMapObject( CMapObject* vpMapObj )
{
	//CMapObject::PLAYER_VECTOR::iterator tPlayerIt;
	//for( tPlayerIt = vpMapObj->mPlayers.begin( ); tPlayerIt != vpMapObj->mPlayers.end( ); tPlayerIt++ )
	//{
	//	CSceneObjManager::GetSingletonPtr( )->DestroyObject( ( *tPlayerIt ) );
	//}

#ifdef _FB_
	int tIndex = vpMapObj->GetMapIndex( );

	LOG_INFO( "map", "[ %s : %d ][ %s ] DestroyMapObject( %d : %d : %d : %lld ) PlayerCount( %d : %d )", __LK_FILE__,
		__LINE__, __FUNCTION__, vpMapObj->get_id( ), vpMapObj->GetMapID( ), vpMapObj->GetMapIndex( ), 
		vpMapObj->GetKey( ), vpMapObj->GetPlayerCount( ), vpMapObj->GetPlayerWaitInCount( ) );
	for( int i = vpMapObj->mEntity.size( ) - 1; i >= 0; i-- )
	{
		CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( vpMapObj->mEntity[ i ] );
		if( tpEntity == NULL )
		{
			continue;
		}
//		tpEntity->TimeForDie( );
		CSceneLogic::GetSingletonPtr( )->DestroyEntity( tpEntity, SAVE_KICKOUT );
	}



//	CRepetion* tpRepetion = vpMapObj->GetRepetion( );
//	if( tpRepetion != NULL )
//	{
//		CMessage tMsg;
//		CMessageDestroyRepetionMap tMsgFact;

//		tMsg.set_msgpara((uint32_t) &tMsgFact );
//		tMsg.mutable_msghead()->set_messageid( ID_S2G_DESTROY_REPETION_MAP );

//		tMsgFact.set_key( vpMapObj->GetKey( ) );
//		tMsgFact.set_mapid( vpMapObj->GetMapID( ) );

//		CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMsg );

//		tpRepetion->End( );

////		CSceneObjManager::GetSingletonPtr( )->DestroyObject( tpRepetion->get_id( ) );

//		vpMapObj->SetRepetionID( INVALID_OBJ_ID );
//	}

	if( mMapObjID[ tIndex ] == vpMapObj->get_id( ) )
	{
		mMapObjID[ tIndex ] = INVALID_MAP_INDEX;
		mFreeMapObjQueue.push( tIndex );
	}

#endif
	CSceneObjManager::GetSingletonPtr( )->DestroyObject( vpMapObj->get_id( ) );

	return SUCCESS;
}

int CMapObjManager::MakeKey( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex )
{
	return ( vLineID << 28 ) | ( vMapID << 16 ) | vIndex;
}

unsigned int CMapObjManager::CountSize()
{
	return sizeof( CMapObjManager );
}


bool CMapObject::GetDropPos( CWTPoint &vOrginPos )
{
	CWTPoint tInner[ 8 ];
	tInner[ 0 ] = CWTPoint( -1, -1 );
	tInner[ 1 ] = CWTPoint( -1, 0 );
	tInner[ 2 ] = CWTPoint( -1, 1 );
	tInner[ 3 ] = CWTPoint( 0, 1 );
	tInner[ 4 ] = CWTPoint( 1, 1 );
	tInner[ 5 ] = CWTPoint( 1, 0 );
	tInner[ 6 ] = CWTPoint( 1, -1 );
	tInner[ 7 ] = CWTPoint( 0, -1 );
	CWTPoint tOuter[ 16 ];
	tOuter[ 0 ] = CWTPoint( -2, -2 );
	tOuter[ 1 ] = CWTPoint( -2, 0 );
	tOuter[ 2 ] = CWTPoint( -2, 2 );
	tOuter[ 3 ] = CWTPoint( -2, -1 );
	tOuter[ 4 ] = CWTPoint( -2, 1 );
	tOuter[ 5 ] = CWTPoint( 2, -2 );
	tOuter[ 6 ] = CWTPoint( 2, 0 );
	tOuter[ 7 ] = CWTPoint( 2, 2 );
	tOuter[ 8 ] = CWTPoint( 2, -1 );
	tOuter[ 9 ] = CWTPoint( 2, 1 );
	tOuter[ 10 ] = CWTPoint( 0, -2 );
	tOuter[ 11 ] = CWTPoint( 0, 2 );
	tOuter[ 12 ] = CWTPoint( 1, -2 );
	tOuter[ 13 ] = CWTPoint( 1, 2 );
	tOuter[ 14 ] = CWTPoint( -1, -2 );
	tOuter[ 15 ] = CWTPoint( -1, 2 );

	CWTPoint tDesPos;

	// 判断内圈
	bool tFind = false;
	for ( unsigned int i = 0; i < ARRAY_CNT(tInner); i++ )
	{
		tDesPos = vOrginPos + tInner[ i ];
		if ( IsStaticBlock( tDesPos ) > 0 || IsSuperBlock( tDesPos ) > 0 )
		{
			continue;
		}
		
		// 搜索坐标格子里面是否已经有物品类型
		MapCellDropPosSearcher tDropPosSearcher( tDesPos, this );
		tDropPosSearcher.Visit();
		
		if ( tDropPosSearcher.mHasItem == false )
		{
			vOrginPos = tDesPos;
			return true;
		}
	}
	if ( tFind == false )
	{
		for ( unsigned int i = 0; i < ARRAY_CNT(tOuter); i++ )
		{
			tDesPos = vOrginPos + tOuter[ i ];
			if ( IsStaticBlock( tDesPos ) > 0 || IsSuperBlock( tDesPos ) > 0 )
			{
				continue;
			}

			MapCellDropPosSearcher tDropPosSearcher( tDesPos, this );
			tDropPosSearcher.Visit();

			if ( tDropPosSearcher.mHasItem == false )
			{
				tFind = true;
				break;
			}
		}
	}
	// 如果没找到，在内圈随机放
	if ( tFind == false )
	{
		for ( unsigned int i = 0; i < ARRAY_CNT(tInner); i++ )
		{
			if ( IsStaticBlock( vOrginPos + tInner[ i ] ) == 0 || IsSuperBlock( vOrginPos + tInner[ i ] ) == 0 )
			{
				vOrginPos = vOrginPos + tInner[ i ];
				return true;
			}
		}
	}
	else
		vOrginPos = tDesPos;
	return true;
}

// 得到NPC创建点(没找到合适的 就返回false) (不同类型的可以在一个格子)
bool CMapObject::GetNpcCreatePos( CWTPoint &vOrginPos, EEntityType nType )
{
	CWTPoint tDesPos;

	// 判断内圈
	for ( int j = vOrginPos.mY; j < std::min( vOrginPos.mY + CREATENPC_RANGE_HEIGHT, (int)GetHight()); j = j+2 )
	{
		for ( int i = vOrginPos.mX; i < std::min( vOrginPos.mX + CREATENPC_RANGE_WIDE, (int)GetWidth()); i=i+2 )
		{
			tDesPos = CWTPoint( i, j );
			if ( IsStaticBlock( tDesPos ) > 0 || IsSuperBlock( tDesPos ) > 0 )
			{
				continue;
			}

			// 搜索坐标格子里面是否已经NPC
			MapCellNpcCreatePosSearcher tPosSearcher( tDesPos, this, nType );
			tPosSearcher.Visit();

			if ( tPosSearcher.mHasNpc == false )
			{
				vOrginPos = tDesPos;
				return true;
			}
		}
	}

	return false;
}


// 将实体加入到cell
bool CMapObject::AddEntityToCell( CEntity* pEntity )
{
	LK_ASSERT( pEntity != NULL, return false );

	CMapCell* tpCell = GetCell( CWTPoint( pEntity->GetPosX(), pEntity->GetPosY()) );
	if( tpCell == NULL )
	{
		LOG_ERROR( "map", "[%s:%d] entity %d positon(%d,%d) out of map %d", __FUNCTION__, __LINE__, 
			pEntity->GetEntityID(), pEntity->GetPosX(), pEntity->GetPosY(), this->GetMapID() );
		return false;
	}

	tpCell->AddEntityToCell( pEntity );	

	// 动态阻挡添加入CellEntity
	if( pEntity->IsBlock() )
	{
		//CEntityBlock* tpEntity = ( CEntityBlock* )pEntity;
		CTemplateBlock* tpTpl = ( CTemplateBlock* )CDataStatic::SearchTpl( pEntity->GetNpcID( ) );
		LK_ASSERT_STR( tpTpl != NULL, return false, "Cannt GetTpl %d, EntityID %d", pEntity->GetNpcID( ), 
			pEntity->GetEntityID( ) );

		unsigned int i = 0;

		for( i = 0; i < ARRAY_CNT( tpTpl->mPos ); i++ )
		{
			if( tpTpl->mPos[ i ].mX == 0 && tpTpl->mPos[ i ].mY == 0 )
			{
				break;
			}

			int tX = pEntity->GetPosX( ) + tpTpl->mPos[ i ].mX;
			int tY = pEntity->GetPosY( ) + tpTpl->mPos[ i ].mY;

			AddBlock( tX, tY );
		}
	}


#ifdef _DEBUG_
	//LOG_DEBUG( "map" , "add %d to cell(%d,%d), size %d", pEntity->GetEntityID(),  pEntity->GetPosX(), pEntity->GetPosY(), tpCell->size());
	//LOG_DEBUG( "map" , "cell(%d,%d) has %d entity, all cell entity %d " ,  pEntity->GetPosX(), pEntity->GetPosY(), tpCell->size(), CellsEntity::share_list_ptr__->max_size()- CellsEntity::share_list_ptr__->size() );
	//CellsEntity::iterator it ;
	//for( it = tpCell->begin(); it != tpCell->end(); ++it )
	//{
	//	LOG_DEBUG( "map", "%d", *it );
	//}
#endif 
	return true;
}

// 将实体从cell中移除
bool CMapObject::RemoveEntityFromCell( CEntity* pEntity )
{
	CMapCell* tpCell = GetCell( CWTPoint( pEntity->GetPosX(), pEntity->GetPosY()) );
	if( tpCell == NULL )
	{ 
		LOG_ERROR( "map", "[%s:%d] entity %d positon(%d,%d) out of map %d", __FUNCTION__, __LINE__, 
			pEntity->GetEntityID(), pEntity->GetPosX(), pEntity->GetPosY(), this->GetMapID() );
		return false;
	}

	bool bRemoved = tpCell->RemoveEntityFromCell( pEntity );

	// 动态阻挡从CellEntity中删除
	if( pEntity->IsBlock() && ( bRemoved == true ) )
	{
		//CEntityBlock* tpEntity = ( CEntityBlock* )pEntity;
		CTemplateBlock* tpTpl = ( CTemplateBlock* )CDataStatic::SearchTpl( pEntity->GetNpcID( ) );
		LK_ASSERT_STR( tpTpl != NULL, return false, "Cannt GetTpl %d, EntityID %d", pEntity->GetNpcID( ), 
				pEntity->GetEntityID( ) );

		unsigned int i = 0;

		for( i = 0; i < ARRAY_CNT( tpTpl->mPos ); i++ )
		{
			if( tpTpl->mPos[ i ].mX == 0 && tpTpl->mPos[ i ].mY == 0 )
			{
				break;
			}

			int tX = pEntity->GetPosX( ) + tpTpl->mPos[ i ].mX;
			int tY = pEntity->GetPosY( ) + tpTpl->mPos[ i ].mY;

			RemoveBlock( tX, tY );
		}
	}


#ifdef _DEBUG_
	//LOG_DEBUG( "map" , "remove %d from cell(%d,%d), size %d", pEntity->GetEntityID(), pEntity->GetPosX(), pEntity->GetPosY(), tpCell->size());
	//LOG_DEBUG( "map" , "cell(%d,%d) has %d entity, all cell entity %d" ,  pEntity->GetPosX(), pEntity->GetPosY(), tpCell->size(), CellsEntity::share_list_ptr__->max_size()- CellsEntity::share_list_ptr__->size() );
	//CellsEntity::iterator it ;
	//for( it = tpCell->begin(); it != tpCell->end(); ++it )
	//{
	//	LOG_DEBUG( "map", "%d", *it );
	//}
#endif 

	return bRemoved;
}

// 坐标是否被阻挡NPC占领
bool CMapObject::IsOccupiedByBlock( CWTPoint& rPos )
{
	if (  IsValidPos( rPos ) == false )
	{
		return true;
	}
	else
	{
		return IsBlock( rPos.mX, rPos.mY );
	}

	return true;
}


// check if any entity has do stall on this grid
bool CMapObject::HasInStall(const CWTPoint &rPoint)
{
	MapCellStallSearcher tStallSearcher( rPoint, this );
	tStallSearcher.Visit();
	
	return tStallSearcher.mHasStall;
}

//Get Map GridBlock
CGridBlock* CMapObject::GetGridBlock( unsigned int vWidth, unsigned int vHeight )
{
#ifndef _FB_
	CMapTpl* tpTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetMapID( ) );
	if( tpTpl != NULL )
	{
		return tpTpl->GetGridBlock( vWidth, vHeight );
	}

	return NULL;
#else
	CMapTpl* tpTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetVirtualMapID( ) );
	if( tpTpl != NULL )
	{
		return tpTpl->GetGridBlock( vWidth, vHeight );
	}

	return NULL;
#endif

}

int CMapObject::GetPlayerCountMax( )
{
#ifndef _FB_
	CMapTpl* tpTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetMapID( ) );
	if( tpTpl != NULL )
	{
		return tpTpl->mPlayerCountMax;
	}

	return 0;
#else
	CMapTpl* tpTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetMapID( ) );
	if( tpTpl != NULL )
	{
		return tpTpl->mPlayerCountMax;
	}

	return 0;
#endif
}

int CMapObject::AddPlayerIntoMap( CEntityPlayer* vpPlayer ,bool vFirst)
{
#ifdef _FB_
	CRepetion* tpRepetion = GetRepetion( );
	if( tpRepetion != NULL )
	{
		tpRepetion->PlayerJoinRepetion( vpPlayer ,vFirst);
	}
	else
	{
		CRepetionModule::GetSingletonPtr( )->NotifyRepetionIndex( vpPlayer, 0 );
	}
#endif
	if( GetPlayerCount( ) - GetPlayerWaitInCount( ) < GetPlayerCountMax( ) )
	{
		if( vpPlayer->GetLineID( ) != GetLineID( ) 
				|| vpPlayer->GetMapID( ) != GetMapID( )
				|| vpPlayer->GetMapIndex( ) != GetMapIndex( )
		  )
		{
			mPlayers.push_back( vpPlayer->GetEntityID() );

				//	DelPlayerWaitInCount( );
			return SUCCESS;
		}
		else
		{
			PLAYER_VECTOR::iterator it;
			for( it = mPlayers.begin(); it != mPlayers.end( ); it++ )
			{
				if( *it == (int)vpPlayer->GetEntityID( ) )
				{
					//			DelPlayerWaitInCount( );
					return ERR_UNKNOWN;
				}
			}
			mPlayers.push_back( vpPlayer->GetEntityID( ) );
#ifdef _FB_
			//            CRepetion* tpRepetion = GetRepetion( );
			//			if( tpRepetion != NULL )
			//			{
			//          	tpRepetion->PlayerJoinRepetion( vpPlayer );
			//		}
#endif
			//	DelPlayerWaitInCount( );
			return SUCCESS;
		}
	}
	//	DelPlayerWaitInCount( );
	return ERR_FULL;
}

int CMapObject::RemovePlayerFromMap( CEntityPlayer* vpPlayer )
{
	PLAYER_VECTOR::iterator it;
	if( vpPlayer->GetLineID( ) == GetLineID( )
		&& vpPlayer->GetMapID( ) == GetMapID( )
		&& vpPlayer->GetMapIndex( ) == GetMapIndex( )
	)
	{
		for( it = mPlayers.begin( ); it != mPlayers.end( ); it++ )
		{
			if( *it == (int)vpPlayer->GetEntityID( ) )
			{
				mPlayers.erase( it );
				return SUCCESS;
			}
		}		
	}

	return ERR_NOT_FOUND;
}

int CMapObject::AddPlayerWaitInCount( )
{
	mPlayerWaitInCount = ( mPlayerWaitInCount >= 0 ) ? mPlayerWaitInCount + 1 : 1;
	return mPlayerWaitInCount;
}

int CMapObject::DelPlayerWaitInCount( )
{
	mPlayerWaitInCount = ( mPlayerWaitInCount > 0 ) ? mPlayerWaitInCount - 1 : 0;
	return mPlayerWaitInCount;
}

int CMapObject::GetPlayerWaitInCount( )
{
	mPlayerWaitInCount = ( mPlayerWaitInCount >= 0 ) ? mPlayerWaitInCount : 0;
	return mPlayerWaitInCount;
}

bool CMapObject::IsFull( )
{
	return ( GetPlayerCount( ) >= GetPlayerCountMax( ) );
}

int CMapObject::GetPlayerCount( )
{
	return mPlayers.size( ) + GetPlayerWaitInCount( );
}




//************************************
// Method:    SendCreateNpc2Player
// FullName:  CMapObject::SendCreateNpc2Player
// Access:    public 
// Returns:   int
// Qualifier: 向单个玩家发送创建多个NPC的消息
// Parameter: CEntityPlayer * pReceiver
// Parameter: std::vector<CEntity * > * pNpcVec
// Parameter: int nListID
//************************************
int CMapObject::SendCreateNpc2Player( CEntityPlayer* pReceiver, std::vector<CEntity*> *pNpcVec, int nListID )
{
	if( pNpcVec->empty() )
	{
		return 0;
	}

	// 这种情况几乎没有，怎么有几百只怪站在一起的情况? 开玩笑？
	if( pNpcVec->size() > MAX_NPC_NUM_OF_CREATE_MSG )
	{
		std::vector<CEntity*>::iterator itBlockBegin = pNpcVec->begin();
		std::vector<CEntity*>::iterator itBlockEnd = pNpcVec->end();

		int nSendOnce = 0;

		// 分包发送，一个包最多发MAX_NPC_NUM_OF_CREATE_MSG只
		for( ; itBlockBegin < itBlockEnd;  )
		{
			if( itBlockEnd - itBlockBegin > MAX_NPC_NUM_OF_CREATE_MSG )
			{
				nSendOnce = MAX_NPC_NUM_OF_CREATE_MSG;
			}
			else
			{
				nSendOnce = (itBlockEnd - itBlockBegin);
			}
			
			// 构造临时Vector
			std::vector<CEntity*> tEachNpcVec( itBlockBegin, itBlockBegin + nSendOnce );
		
			itBlockBegin += ( nSendOnce + 1);

			CSceneLogic::GetSingletonPtr()->Send2Player( pReceiver, BuildCreateNpcProMessage( &tEachNpcVec, nListID, pReceiver) );				
		}
	}
	else
	{
		CSceneLogic::GetSingletonPtr()->Send2Player( pReceiver, BuildCreateNpcProMessage( pNpcVec, nListID, pReceiver) );			
	}

	return 0;
}


//************************************
// Method:    SendCreatePlayer2Player
// FullName:  CMapObject::SendCreatePlayer2Player
// Access:    public 
// Returns:   int
// Qualifier: 向单个玩家发送创建多个Player的消息
// Parameter: CEntityPlayer * pReceiver
// Parameter: std::vector<CEntityPlayer * > * pPlayerVec
//************************************
int CMapObject::SendCreatePlayer2Player( CEntityPlayer* pReceiver, std::vector<CEntityPlayer*> *pPlayerVec )
{
	if( pPlayerVec->empty() )
	{
		return 0;
	}

	// 这种情况不常有，除非是国战，军团战, 下面的处理方式效率稍差，但不是瓶颈
	if(  pPlayerVec->size() > MAX_PLAYER_NUM_OF_CREATE_MSG )
	{

		std::vector<CEntityPlayer*>::iterator itBlockBegin = pPlayerVec->begin();
		std::vector<CEntityPlayer*>::iterator itBlockEnd = pPlayerVec->end();
		
		int nSendOnce = 0;

		for( ; itBlockBegin < itBlockEnd;  )
		{
			if( itBlockEnd - itBlockBegin > MAX_PLAYER_NUM_OF_CREATE_MSG )
			{
				nSendOnce = MAX_PLAYER_NUM_OF_CREATE_MSG;
			}
			else
			{
				nSendOnce = (itBlockEnd - itBlockBegin);
			}

			std::vector<CEntityPlayer*> tEachPlayerVec( itBlockBegin, itBlockBegin + nSendOnce );

			itBlockBegin += ( nSendOnce + 1 );

			CSceneLogic::GetSingletonPtr()->Send2Player( pReceiver, BuildCreatePlayerProMessage( &tEachPlayerVec ) );
			CWorldModule::GetSingleton().SendPlayInfoM2SRequest( pReceiver, &tEachPlayerVec );
		}
	}
	else
	{
		CSceneLogic::GetSingletonPtr()->Send2Player( pReceiver, BuildCreatePlayerProMessage( pPlayerVec ) );  			
		CWorldModule::GetSingleton().SendPlayInfoM2SRequest( pReceiver, pPlayerVec );
	}

	return 0;
}


//************************************
// Method:    SendCreatePlayer2Players
// FullName:  CMapObject::SendCreatePlayer2Players
// Access:    public 
// Returns:   int
// Qualifier: 向多个玩家发送创建单个Player的消息
// Parameter: std::vector<CEntityPlayer * > * pReceiver
// Parameter: CEntityPlayer * pSinglePlayer
//************************************
int CMapObject::SendCreatePlayer2Players( std::vector<CEntityPlayer*>* pReceiver, CEntityPlayer* pSinglePlayer )
{
	if( pReceiver->empty() )
	{
		return 0;
	}

	CSceneLogic::GetSingletonPtr()->Send2Player( pReceiver , BuildCreatePlayerProMessage( pSinglePlayer ) );
	CWorldModule::GetSingleton().SendPlayInfoS2MRequest( pReceiver, pSinglePlayer );
	
	return 0;
}


//************************************
// Method:    SendCreateNpc2Players
// FullName:  CMapObject::SendCreateNpc2Players
// Access:    public 
// Returns:   int
// Qualifier: 向多个玩家发送创建单个Npc的消息
// Parameter: std::vector<CEntityPlayer * > * pReceiver
// Parameter: CEntity * pSingleNpc
// Parameter: int nListID
//************************************
int CMapObject::SendCreateNpc2Players(  std::vector<CEntityPlayer*>* pReceiver, CEntity* pSingleNpc, int nListID )
{
	LK_ASSERT( pSingleNpc != NULL, return -1 );

	// 向周围玩家广播创建自己(npc)
	if( pReceiver->empty() )
	{
		return 0;
	}

	// 必须单独发，因为涉及刷任务npc头顶显示。
	for( std::vector<CEntityPlayer*>::iterator it = pReceiver->begin(); it != pReceiver->end(); ++ it )
	{
		CSceneLogic::GetSingletonPtr()->Send2Player( *it , BuildCreateNpcProMessage( pSingleNpc, nListID, *it ) );
	}
	
	return 0;
}



//************************************
// Method:    SendDestroyEntity2Players
// FullName:  CMapObject::SendDestroyEntity2Players
// Access:    public 
// Returns:   int
// Qualifier: 向单个玩家发送销毁多个实体的消息(包括NPC和玩家)
// Parameter: CEntityPlayer * pReceiver
// Parameter: std::vector<CEntityPlayer * > * pPlayerToBeDestroy
// Parameter: std::vector<CEntity * > pNpcToBeDestroy
//************************************
int CMapObject::SendDestroyEntity2Player( CEntityPlayer* pReceiver, std::vector<CEntityPlayer*>* pPlayerToBeDestroy, std::vector<CEntity*>* pNpcToBeDestroy )
{
	if( pPlayerToBeDestroy->size() + pNpcToBeDestroy->size() == 0 )
	{
		return 0;
	}

	CSceneLogic::GetSingletonPtr()->Send2Player( pReceiver, BuildDestroyEntityMessage( pPlayerToBeDestroy, pNpcToBeDestroy ) );
	
	return 0;	
}


//************************************
// Method:    SendDestroyEntity2Players
// FullName:  CMapObject::SendDestroyEntity2Players
// Access:    public 
// Returns:   int
// Qualifier: 向多个玩家发送销毁单个实体的消息
// Parameter: std::vector<CEntityPlayer * > * pReceiver
// Parameter: CEntity * pEntityToBeDestroy
//************************************
int CMapObject::SendDestroyEntity2Players( std::vector<CEntityPlayer*>* pReceiver, CEntity* pEntityToBeDestroy )
{
	if( pReceiver->empty() )
	{
		return 0;
	}

	CSceneLogic::GetSingletonPtr()->Send2Player( pReceiver, BuildDestroyEntityMessage( pEntityToBeDestroy  ) );

	return 0;

}


#ifdef _FB_
int CMapObject::DestroyAliveTempOgreEntity( unsigned int vTemplateID, unsigned int vCount )
{
	unsigned int tCount = 0;
	for( int i = mEntity.size( ) - 1; i >= 0; i-- )
	{
		if( vCount != 0 )
		{
			if( tCount >= vCount )
			{
				break;
			}
		}
		
		CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( mEntity[ i ] );
		if( tpEntity != NULL )
		{
			if( tpEntity->IsNpc( ) == true )
			{
				CEntityNpc* tpOgre = static_cast< CEntityNpc* >(tpEntity);
				if( ( tpEntity->IsAlive( ) == true ) && ( tpOgre->GetNpcID( ) == vTemplateID ) )
				{
//					CSceneLogic::GetSingletonPtr( )->DestroyEntity( tpEntity, SAVE_KICKOUT );
					tpOgre->EntityDie( tpOgre, true, false );					
					CPropertyModule::GetSingletonPtr( )->ResultListSend( );
					tCount++;
				}
			}
		}
		else
		{
//			mEntity.erase( i );
		}
	}
	
	return tCount;
}
#endif

void CMapObject::SendMsg2Player( CMessage* vpMsg )
{
	PLAYER_VECTOR::iterator it;

	for( it = mPlayers.begin( ); it != mPlayers.end( );  )
	{
		unsigned int tEntityID = *it;
		CEntityPlayer* tpPlayer = ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( tEntityID );

		if( tpPlayer == NULL )
		{
			LOG_ERROR( "map", "map (id:%d objid:%d line:%d) can't find player entityid:%d, so remove it",
				GetMapID(), get_id(), GetLineID(), tEntityID );
			it = mPlayers.erase( it );
		}
		else
		{
			++it;
		}

		CSceneLogic::GetSingletonPtr( )->Send2Player( tpPlayer, vpMsg );
	}
}

bool CMapObject::IsBlock( int mX, int mY )
{
	int tNum = mY * mWidth + mX;
	if( BLOCK_INDEX( tNum ) >= ( int )ARRAY_CNT( mBlock ) )
	{
		return true;
	}

	int tCell = mBlock[ BLOCK_INDEX( tNum ) ];

	return ( tCell & ( 1 << BLOCK_CELL_INDEX( tNum ) ) ) != 0;
}

int CMapObject::AddBlock( int mX, int mY )
{
	int tNum = mY * mWidth + mX;
	if( BLOCK_INDEX( tNum ) >= ( int )ARRAY_CNT( mBlock ) )
	{
		return ERR_INVALID_PARAMETER;
	}

	int tCell = mBlock[ BLOCK_INDEX( tNum ) ];

	mBlock[ BLOCK_INDEX( tNum ) ] = tCell | ( 1 << BLOCK_CELL_INDEX( tNum ) );

	return SUCCESS;
}

int CMapObject::RemoveBlock( int mX, int mY )
{
	int tNum = mY * mWidth + mX;
	if( BLOCK_INDEX( tNum ) >= ( int )ARRAY_CNT( mBlock ) )
	{
		return ERR_INVALID_PARAMETER;
	}

	int tCell = mBlock[ BLOCK_INDEX( tNum ) ];

	mBlock[ BLOCK_INDEX( tNum ) ] = tCell & ( ~( 1 << BLOCK_CELL_INDEX( tNum ) ) );

	return SUCCESS;
}



CMapCell* CMapObjectL::GetCell( const CWTPoint& rPos  )
{
	if ( rPos.mY < GetHight() && rPos.mX < GetWidth( ) && rPos.mX >= 0 && rPos.mY >= 0 )
	{
		return &mCells[ rPos.mX + rPos.mY * GetWidth() ];
	}
	else
	{
		LOG_WARN( "map", "x %d, y %d out of map %d", rPos.mX, rPos.mY, GetMapID() );
		return NULL;
	}

}

CMapCell* CMapObjectM::GetCell( const CWTPoint& rPos  )
{
	if ( rPos.mY < GetHight() && rPos.mX < GetWidth( ) && rPos.mX >= 0 && rPos.mY >= 0 )
	{
		return &mCells[ rPos.mX + rPos.mY * GetWidth() ];
	}
	else
	{
		LOG_WARN( "map", "x %d, y %d out of map %d", rPos.mX, rPos.mY, GetMapID() );
		return NULL;
	}

}

CMapCell* CMapObjectS::GetCell( const CWTPoint& rPos  )
{
	if ( rPos.mY < GetHight() && rPos.mX < GetWidth( ) && rPos.mX >= 0 && rPos.mY >= 0 )
	{
		return &mCells[ rPos.mX + rPos.mY * GetWidth() ];
	}
	else
	{
		LOG_WARN( "map", "x %d, y %d out of map %d", rPos.mX, rPos.mY, GetMapID() );
		return NULL;
	}

}


// 是否是绝对安全区
bool CMapObject::IsAbsSafe( )
{
#ifndef _FB_
	CMapTpl* tpTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetMapID( ) );
	if ( tpTpl == NULL )
	{
		return false;
	}
	return ( tpTpl->mAbsSafe == 1 );
#else
	CMapTpl* tpTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetVirtualMapID( ) );
	if ( tpTpl == NULL )
	{
		return false;
	}
	return ( tpTpl->mAbsSafe == 1 );
#endif
}


/* 得到可见范围(前提是地图肯定超过一屏) (能看见我的)
** 
** 在遇到地图边界时该算法有误差，会出现明明在同一屏却看不到的情况。
** 对这个问题只能容忍，期待美术以后作图时都把地图周围画上阻挡，就不会有玩家达到边界的情况了。
**
*/
int CMapObject::GetViewRect( CEntity* pEntity, CWTPoint& rLeftTop, CWTPoint& rRightBottom )
{
	LK_ASSERT(pEntity!=NULL, return -1);
	
	int nTopLeftDeltaX = std::max( pEntity->GetPosX() - VIEW_RADIUS_X, 0 );
	int nTopLeftDeltaY = std::max( pEntity->GetPosY() - VIEW_RADIUS_Y, 0 );
	int nBottomRightDeltaX = std::min( pEntity->GetPosX() + VIEW_RADIUS_X, (int)GetWidth()-1 );
	int nBottomRightDeltaY = std::min( pEntity->GetPosY() + VIEW_RADIUS_Y, (int)GetHight()-1 );	

	//// 靠近上侧 要通知到顶部
	//if (pEntity->GetPosY() <= VIEW_RADIUS_Y*2)
	//{
	//	nTopLeftDeltaY = 0;
	//	if ( pEntity->GetPosY() <= VIEW_RADIUS_Y ) // 在一倍以内 自动加大下面
	//	{
	//		nBottomRightDeltaY = std::min( VIEW_RADIUS_Y*2, (int)GetHight()-1 );
	//	}
	//}

	//// 靠近左侧 要通知到左边
	//if (pEntity->GetPosX() <= VIEW_RADIUS_X*2)
	//{
	//	nTopLeftDeltaX = 0;
	//	if ( pEntity->GetPosX() <= VIEW_RADIUS_X ) // 在一倍以内 自动加大右面
	//	{
	//		nBottomRightDeltaX = std::min( VIEW_RADIUS_X*2, (int)GetWidth()-1 );
	//	}
	//}

	//// 靠近下侧 要通知底部
	//if (pEntity->GetPosY() >= GetHight() - VIEW_RADIUS_Y*2)
	//{
	//	nBottomRightDeltaY = GetHight()-1;
	//	if ( pEntity->GetPosY() >= GetHight() - VIEW_RADIUS_Y ) // 在一倍以内 自动加大上面
	//	{
	//		nTopLeftDeltaY = std::max( 0, (int)GetHight() - VIEW_RADIUS_Y*2 );
	//	}
	//}

	//// 靠近右侧 要通知到右边
	//if (pEntity->GetPosX() >= GetWidth() - VIEW_RADIUS_X*2)
	//{
	//	nBottomRightDeltaX = GetWidth()-1;
	//	if ( pEntity->GetPosX() >= GetWidth() - VIEW_RADIUS_X ) // 在一倍以内 自动加大左面
	//	{
	//		nTopLeftDeltaX = std::max( 0, (int)GetWidth() - VIEW_RADIUS_X*2 );
	//	}
	//}

	rLeftTop = CWTPoint( nTopLeftDeltaX, nTopLeftDeltaY );
	rRightBottom = CWTPoint( nBottomRightDeltaX, nBottomRightDeltaY );

	return 0;
}

bool CMapObject::IsCreatorIn( int vCreatorIndex )
{
	CMapTpl* tTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetMapID( ) );
	LK_ASSERT_STR( tTpl != NULL, return false, "MapID %d", GetMapID( ) );

	return tTpl->IsCreatorIn( vCreatorIndex );
}


//////////////////////////////////////////////////////////////////////////
// CMapCell
//////////////////////////////////////////////////////////////////////////
CMapCell::CMapCell( ) 
{
	CellsEntity::share_list_ptr__ = &CSceneObjManager::GetSingletonPtr( )->mSharedCellsEntityID;

	if( CObj::msCreateMode == SHM_INIT )
		mCells.initailize();
}

CMapCell::~CMapCell( ) {}


MAP_TYPE CMapObject::GetMapType( )
{
#ifndef  _FB_
	MAP_TYPE tMapType = COMM_MAP;
	
	CMapTpl* tpTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetMapID( ) );

	if( tpTpl != NULL )
	{
		tMapType = tpTpl->mMapType;	
	}

	return tMapType;
#else
	MAP_TYPE tMapType = COMM_MAP;
	
	CMapTpl* tpTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetVirtualMapID( ) );

	if( tpTpl != NULL )
	{
		tMapType = tpTpl->mMapType;	
	}

	return tMapType;
#endif
}

bool CMapObject::IsCommMap( )
{
	return GetMapType( ) == COMM_MAP;
}

bool CMapObject::IsGroundMap( )
{
	return GetMapType( ) == GROUND_MAP;
}

bool CMapObject::IsFBMap( )
{
	return GetMapType( ) == FB_MAP;
}

bool CMapObject::IsBattleMap( )
{
	return GetMapType( ) == BATTLE_MAP;
}

bool CMapObject::IsSchoolMap( )
{
	return GetMapType( ) == SCHOOL_MAP;
}




short CMapObject::GetMinLevelLmt( ) 
{
#ifndef _FB_
	CMapTpl* tTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetMapID( ) );
	if( tTpl == NULL )
	{
		return 0;		
	}
	return tTpl->GetMinLevelLmt();
#else
	CMapTpl* tTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetVirtualMapID( ) );
	if( tTpl == NULL )
	{
		return 0;		
	}
	return tTpl->GetMinLevelLmt();
#endif
}


short CMapObject::GetMaxLevellmt( ) 
{
#ifndef _FB_
	CMapTpl* tTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetMapID( ) );
	if( tTpl == NULL )
	{
		return 0;		
	}
	return tTpl->GetMaxLevellmt();
#else
	CMapTpl* tTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( GetVirtualMapID( ) );
	if( tTpl == NULL )
	{
		return 0;		
	}
	return tTpl->GetMaxLevellmt();
#endif
}

#ifdef _FB_
int CMapObject::SetVirtualMapID( unsigned int vMapID )
{
	CMapTpl* tpTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( vMapID );
	LK_ASSERT_STR( tpTpl != NULL, return ERR_INVALID_PARAMETER, "MapID %d", vMapID );

	for( int i = mEntity.size( ) - 1; i >= 0; i-- )
	{
		CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr( )->GetObject( mEntity[ i ] );
		if( tpEntity == NULL )
		{
			continue;
		}

		CSceneLogic::GetSingletonPtr( )->DestroyEntity( tpEntity, SAVE_KICKOUT );
	}

	for( unsigned int i = 0; i < sizeof( mData ); i++ )
	{
		mData[ i ] = 0;
	}

	LoadTpl( tpTpl );	
	CreateAllEntity( );

	mVirtualMapID = vMapID;

	return SUCCESS;
}

int CMapObject::GetVirtualMapID( )
{
	return mVirtualMapID;
}

int CMapObject::SetMapData( int vIndex, int vData  )
{
	REPE_ASSERT_STR( vIndex >= 0 && vIndex < ( int )ARRAY_CNT( mData ), return ERR_INVALID_PARAMETER, "Index %d", 
		vIndex );		
	
	mData[ vIndex ] = vData;

	return mData[ vIndex ];
}

int CMapObject::GetMapData( int vIndex )
{	
	REPE_ASSERT_STR( vIndex >= 0 && vIndex < ( int )ARRAY_CNT( mData ), return ERR_INVALID_PARAMETER, "Index %d", 
		vIndex );		
	return mData[ vIndex ];
}


#endif




int CMapObjManager::ReloadDeny( )
{
	TiXmlDocument tDoc;
    /*tDoc.LoadFile( "../config/data/scene_config/DenyTemp.xml" );*/
	tDoc.LoadFile( CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(), const_cast<char*>("scene_config/DenyTemp.xml" )));
	TiXmlElement* tpRootEle = tDoc.FirstChildElement( "TempList" );
	if( tpRootEle == NULL )
	{
		return ERR_NOT_FOUND;
	}

	mDenyList.clear( );

	for( TiXmlElement* tpElement = tpRootEle->FirstChildElement( ); tpElement != NULL; 
			tpElement = tpElement->NextSiblingElement( ) )
	{
		if( mDenyList.size( ) < mDenyList.max_size( ) )
		{
			mDenyList.push_back( atoi( tpElement->Attribute("ID") ) );
		}
		else
		{
			printf( "deny ogre list full\n" );
			LOG_WARN( "map", "[ %s : %d ][ %s ] deny ogre list full", __LK_FILE__, __LINE__, __FUNCTION__ );
			break;
		}
	}

	return SUCCESS;
}


bool CMapObjManager::IsDeny( int vTempID )
{
	DENY_LIST_TYPE::iterator it = mDenyList.begin( );
	
	for( ;it != mDenyList.end( ); it++ )	
	{
		if( ( *it ) == vTempID )
		{
			return true;
		}
	}
	return false;
}



