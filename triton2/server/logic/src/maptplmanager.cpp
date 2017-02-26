// ********************************************************************** //
// MapTplManager.cpp
// 地图模版管理器, 
// 管理所有地图模版, 即管理地图模版的生命期,地图模版指的是地图数据,
// 这份数据可以定义出很多地图对象
// Author : Black
// ********************************************************************** //
#include "stdafx.hpp"
#include "mapmodule.h"
#include "maptplmanager.h"
#include "mapobjmanager.h"
#include "sceneobj_manager.h"
#include "repetionmodule.h"

template<> CMapTplManager* CSingleton< CMapTplManager >::spSingleton = NULL;
CSharedMem* CMapTplManager::mShmPtr = NULL;

CMapTpl* CMapTplManager::CreateMapTpl(	const char* pTplName, 
		unsigned short vMapID, 
		unsigned short vLineID,
		bool bReload /*= false */
		)
{
	
	int tRtn = SUCCESS;
	
	CMapTpl* tpMapTpl = GetMapTplByID( vMapID );

	if( bReload == true )
	{
		if( tpMapTpl != NULL )
		{
			// 先初始化掉残留数据
			tpMapTpl->Final();
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		if( tpMapTpl != NULL )
		{
			return tpMapTpl;
		}
	}

	char stfile[56] ={0};
	::snprintf(stfile,sizeof(stfile) -1,"scenetpl/%s.xml", pTplName );
	char tTplName[ 128 ] = { 0 };
	//::snprintf( tTplName, sizeof(tTplName)-1, "../config/data/scenetpl/%s.xml", pTplName );
	::snprintf( tTplName, sizeof(tTplName)-1, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),stfile ));

	TiXmlDocument tTplDoc;
	tTplDoc.LoadFile( tTplName );
	TiXmlElement* tpRoot = tTplDoc.FirstChildElement("Scene_Conf");
	if( tpRoot == NULL)
	{
		LOG_ERROR( "map", "No Root Element Found" );
		return NULL;
	}

	TiXmlElement* tpElement = tpRoot->FirstChildElement( );
	for( ; tpElement != NULL; tpElement = tpElement->NextSiblingElement( ) )
	{
		std::string tEleName = tpElement->Value( );
		if ( tEleName != "Map" )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ]Map Template %s Err, First Element isn't Map", __LK_FILE__,
					__LINE__, __FUNCTION__, pTplName );

			printf( "CreateMapTpl %s Faild\n", pTplName );

			continue;
		}

		const char* tpMapName	= tpElement->Attribute( "MapName" );
		const char* tpMapID		= tpElement->Attribute( "MapID" );
		const char* tpSrcMapID	= tpElement->Attribute( "SrcMapID" );
		const char* tpPKPunish	= tpElement->Attribute( "PKPunish" );
		const char*	tpBasRide	= tpElement->Attribute( "BasRide" );
		const char* tpAdvRide	= tpElement->Attribute( "AdvRide" );
		const char* tpPKMode	= tpElement->Attribute( "PKMode" );
		const char* tpTeleItem	= tpElement->Attribute( "TeleItem" );
		const char* tpNoNation	= tpElement->Attribute( "NoNation" );
		const char* tpAbsSafe	= tpElement->Attribute( "AbsSafe" );
		const char* tpConSafe	= tpElement->Attribute( "ConSafe" );
		const char* tpType		= tpElement->Attribute( "Type" );
		const char* tpMinLevel	= tpElement->Attribute( "MinLevel" );
		const char* tpMaxLevel	= tpElement->Attribute( "MaxLevel" );
		const char* tpProtect	= tpElement->Attribute( "NationProt" );
		const char* tpFlyRide	= tpElement->Attribute( "FlyRide" );
		if ( 	tpMapName == NULL || tpMapID == NULL || tpSrcMapID == NULL || tpPKPunish == NULL || tpBasRide == NULL
				|| tpAdvRide == NULL || tpPKMode == NULL || tpTeleItem == NULL || tpNoNation == NULL 
				|| tpAbsSafe == NULL || tpConSafe == NULL || tpType == NULL )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ]Map Template %s Err, MapName Or MapID Can't find", __LK_FILE__, 
					__LINE__, __FUNCTION__, pTplName );

			continue;
		}

		// 如果找到指定地图，加载该地图
		if ( ::atoi( tpMapID ) == vMapID )
		{
			if( mMapTplCount >= ARRAY_CNT( mMapTpls ) )
			{
				LOG_ERROR( "default", "[ %s : %d ][ %s ]Cannt Operate New Space", __LK_FILE__, __LINE__, __FUNCTION__);	
				return NULL;
			}

			CMapTpl* tpMap = NULL;
			
			if( bReload == true )
			{
				tpMap = tpMapTpl;
			}
			else
			{
				tpMap = &mMapTpls[ mMapTplCount ];
				mMapTplCount++;
			}

			tpMap->mPKPunish = atoi( tpPKPunish );
			tpMap->mBasRide	 = atoi( tpBasRide );
			tpMap->mAdvRide	 = atoi( tpAdvRide );
			tpMap->mPKMode	 = ( EPKMode )atoi( tpPKMode );
			if ( tpFlyRide != NULL )
			{
				tpMap->mFlyRide = atoi( tpFlyRide );
			}
			else
			{
				tpMap->mFlyRide = false;
			}
		
			tpMap->mMapType	 = ( MAP_TYPE )atoi( tpType ); 
			
			if( tpMinLevel != NULL )
				tpMap->mMinLevelLmt = atoi( tpMinLevel );

			if( tpMaxLevel != NULL )
				tpMap->mMaxLevelLmt = atoi( tpMaxLevel );

			if ( tpProtect != NULL )
			{
				tpMap->mProtectLevel = atoi( tpProtect );
			}
			
			int tTeleItem = atoi( tpTeleItem );
			if ( tTeleItem > 0 )
			{
				tpMap->mTeleItem = false;
			}
			else
			{
				tpMap->mTeleItem = true;
			}
			
			tpMap->mNoNation = atoi( tpNoNation );
			tpMap->mAbsSafe	 = atoi( tpAbsSafe );
			tpMap->mConSafe	 = atoi( tpConSafe );
			char stfile[56] = {0};
			::snprintf( stfile,sizeof(stfile)-1,"scenetpl/%s.dat", pTplName);
			char tTplName[ 128 ] = { 0 };
			/*::snprintf( tTplName, sizeof(tTplName)-1, "../config/data/scenetpl/%s.dat", pTplName );*/
			::snprintf( tTplName, sizeof(tTplName)-1,  CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),stfile));
			FILE* tpBlock = ::fopen( tTplName, "rb" );
			if ( tpBlock == NULL )
			{
				tpMap->Final( );
				LOG_ERROR( "default", "Map[ %s ]Cannt find Countercheck info", tpMapName );
				return NULL;
			}

			tRtn = tpMap->LoadMap( tpElement, tpBlock, pTplName, vLineID, vMapID );
			if( tRtn != SUCCESS )
			{
				LOG_ERROR( "default", "Map [%s] Load Faild, Please Check Config File", tpMapName );

				tpMap->Final( );
				tpMap = NULL;
				
				fclose( tpBlock );
				tpBlock = NULL;

				return NULL;
			}

			// 读取阻挡信息
			::fclose( tpBlock );
			tpBlock = NULL;

			tpMap->mMapID	= vMapID;
			strncpy( tpMap->mMapName, tpMapName, sizeof( tpMap->mMapName ) );

			if( IS_REPETION_LINE_ID( vLineID ) == true )
			{
				CRepetionInfo* tpRepetionInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfoByMapID( vMapID );
				if( tpRepetionInfo != NULL )
				{
					tpMap->mPlayerCountMax = tpRepetionInfo->GetPlayerCountMax( );
				}
				else
				{
					tpMap->mPlayerCountMax = MAX_MAP_PLAYER_PER;
				}
			}
			else
			{
				tpMap->mPlayerCountMax = MAX_MAP_PLAYER_PER;
			}

			return tpMap; 
		}
	}

	LOG_ERROR( "default", "[ %s : %d ][ %s ]Cannt Find Map( %d ) From MapConfig", __LK_FILE__, __LINE__, 
		__FUNCTION__, vMapID );

	return NULL;
}

CMapTpl* CMapTplManager::GetMapTplByID( unsigned short vMapID )
{
//	if( vMapID < sizeof( mMapTpls ) / sizeof( mMapTpls[ 0 ] ) )
//	{
//		return mMapTpls[ vMapID ];
//	}

	for( unsigned int i = 0; i < mMapTplCount && i < ARRAY_CNT( mMapTpls ); i++ )
	{
		if( mMapTpls[ i ].mMapID == vMapID )
		{
			return &mMapTpls[ i ];
		}
	}

	return NULL;
}

CGridBlock* CMapTpl::GetGridBlock( unsigned int vWidth, unsigned int vHeight )
{
	if( vWidth >= (unsigned int)GetWidth() || vHeight >= (unsigned int)GetHeight() )
	{
		return NULL;
	} 

	return mGrids+ vHeight * GetWidth() +  vWidth;
}


int CMapTpl::LoadMap( TiXmlElement* vpElement, FILE* vpBlock, const char* pTplName, unsigned short vLineID, 
		unsigned short vMapID )
{
	LK_ASSERT( vpElement != NULL && vpBlock != NULL, return ERR_INVALID_PARAMETER );


	TiXmlElement* tpMapEle = vpElement->FirstChildElement( );
	for( ; tpMapEle != NULL; tpMapEle = tpMapEle->NextSiblingElement( ) )
	{
		std::string tEleName = tpMapEle->Value( );
		if ( tEleName == "Size" )
		{
			const char* tpCX = tpMapEle->Attribute( "cx" );
			const char* tpCY = tpMapEle->Attribute( "cy" );
			LK_ASSERT ( tpCX != NULL && tpCY != NULL, return ERR_INVALID_PARAMETER );
			mWidth	= ::atoi( tpCX );
			mHeight	= ::atoi( tpCY );
		}
		
		// 如果是传送点建立器
		if ( tEleName == "TeleportData" )
		{
			TiXmlElement* tpTeleEle = tpMapEle->FirstChildElement( );
			for( ; tpTeleEle != NULL; tpTeleEle = tpTeleEle->NextSiblingElement( ) )
			{
				std::string tEleName = tpTeleEle->Value( );
				if ( tEleName == "Teleport" )
				{
					CPedalCreator* tpPedalCreator = ( CPedalCreator* )CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_CREATOR_PEDAL );
					LK_ASSERT( tpPedalCreator != NULL, continue );
					
					if( tpPedalCreator->LoadFromConfig( tpTeleEle ) != true )
					{
						CSceneObjManager::GetSingletonPtr()->DestroyObject( tpPedalCreator->get_id() );
						LK_ASSERT( false, continue;)
					}

					tpPedalCreator->mDefaultLineID = vLineID;
					tpPedalCreator->mDefaultMapID = vMapID;
					tpPedalCreator->mMapIndex = vMapID;
				
					int tRtn = CCreatorManager::GetSingletonPtr()->InsertCreatorBase( tpPedalCreator->GetIndex(), tpPedalCreator );
					if( tRtn != 0 )
					{
						CSceneObjManager::GetSingletonPtr()->DestroyObject( tpPedalCreator->get_id() );
						LK_ASSERT( false, continue;)
					}
					mCreators.push_back( tpPedalCreator->GetIndex() );
				}
			}					
		}

		// 如果是NPC建立器
		if ( tEleName == "NpcData" )
		{
			TiXmlElement* tpNpcEle = tpMapEle->FirstChildElement( );
			for( ; tpNpcEle != NULL; tpNpcEle = tpNpcEle->NextSiblingElement( ) )
			{
				std::string tEleName = tpNpcEle->Value( );
				
				CCreator* tpNpcCreator = NULL;
				if ( tEleName == "Npc" )
				{
					tpNpcCreator = ( CCreator* )CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_CREATOR_NPC );
				}
				else if( tEleName == "Collect" )
				{
					tpNpcCreator = ( CCreator* )CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_CREATOR_COLLECT );
					tpNpcCreator->mMapIndex = vMapID;
				}
				LK_ASSERT( tpNpcCreator != NULL, continue );

				if( tpNpcCreator->LoadFromConfig( tpNpcEle ) != true )
				{
					CSceneObjManager::GetSingletonPtr()->DestroyObject( tpNpcCreator->get_id() );
					LK_ASSERT( false, continue;)
				}

				int tRtn = CCreatorManager::GetSingletonPtr()->InsertCreatorBase( tpNpcCreator->GetIndex(), tpNpcCreator );
				if( tRtn != 0 )
				{
					CSceneObjManager::GetSingletonPtr()->DestroyObject( tpNpcCreator->get_id() );
					LK_ASSERT( false, continue;)
				}
				mCreators.push_back( tpNpcCreator->GetIndex() );
			}	
			
		}

		// 如果是Ogre建立器
		if ( tEleName == "OgreData" )
		{
			TiXmlElement* tpOgreEle = tpMapEle->FirstChildElement( );
			for( ; tpOgreEle != NULL; tpOgreEle = tpOgreEle->NextSiblingElement( ) )
			{
				std::string tEleName = tpOgreEle->Value( );
				if ( tEleName == "Ogre" )
				{
					COgreCreator* tpNpcCreator = ( COgreCreator* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_CREATOR_OGRE );
					LK_ASSERT ( tpNpcCreator != NULL, continue );
					
					if( tpNpcCreator->LoadFromConfig( tpOgreEle ) != true )
					{
						CSceneObjManager::GetSingletonPtr()->DestroyObject( tpNpcCreator->get_id() );
						LK_ASSERT( false, continue;)
					}
					tpNpcCreator->mLineID = vLineID;
					tpNpcCreator->mMapID = vMapID;
					tpNpcCreator->mMapIndex = vMapID;
				
					int tRtn = CCreatorManager::GetSingletonPtr()->InsertCreatorBase( tpNpcCreator->GetIndex(), tpNpcCreator );
					if( tRtn != SUCCESS )
					{
						CSceneObjManager::GetSingletonPtr()->DestroyObject( tpNpcCreator->get_id() );
						LK_ASSERT( false, continue;)
					}
					mCreators.push_back( tpNpcCreator->GetIndex() );
				}
			}					
		}


		// 如果是StoreBox建立器
		if ( tEleName == "BoxData" )
		{
			TiXmlElement* tpBoxEle = tpMapEle->FirstChildElement( );
			for( ; tpBoxEle != NULL; tpBoxEle = tpBoxEle->NextSiblingElement( ) )
			{
				std::string tEleName = tpBoxEle->Value( );
				if ( tEleName == "Box" )
				{
					CBoxCreator* tpNpcCreator = ( CBoxCreator* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_CREATOR_BOX );
					LK_ASSERT ( tpNpcCreator != NULL, continue );
	
					if( tpNpcCreator->LoadFromConfig( tpBoxEle ) != true )
					{
						CSceneObjManager::GetSingletonPtr()->DestroyObject( tpNpcCreator->get_id() );
						LK_ASSERT( false, continue;)
					}

					int tRtn = CCreatorManager::GetSingletonPtr()->InsertCreatorBase( tpNpcCreator->GetIndex(), tpNpcCreator );
					if( tRtn != SUCCESS )
					{
						CSceneObjManager::GetSingletonPtr()->DestroyObject( tpNpcCreator->get_id() );
						LK_ASSERT( false, continue;)
					}
					mCreators.push_back( tpNpcCreator->GetIndex() );					
				}
			}
		}

		if( tEleName == "BlockData" )
		{
			TiXmlElement* tpBlockEle = tpMapEle->FirstChildElement( );
			for( ; tpBlockEle != NULL; tpBlockEle = tpBlockEle->NextSiblingElement( ) )
			{
				std::string tEleName = tpBlockEle->Value( );
				if ( tEleName == "Block" )
				{
					CBlockCreator* tpNpcCreator = ( CBlockCreator* ) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_CREATOR_BLOCK );
					LK_ASSERT ( tpNpcCreator != NULL, continue );

					if( tpNpcCreator->LoadFromConfig( tpBlockEle ) != true )
					{
						CSceneObjManager::GetSingletonPtr()->DestroyObject( tpNpcCreator->get_id() );
						LK_ASSERT( false, continue;)
					}

					int tRtn = CCreatorManager::GetSingletonPtr()->InsertCreatorBase( tpNpcCreator->GetIndex(), tpNpcCreator );
					if( tRtn != SUCCESS )
					{
						CSceneObjManager::GetSingletonPtr()->DestroyObject( tpNpcCreator->get_id() );
						LK_ASSERT( false, continue;)
					}
					mCreators.push_back( tpNpcCreator->GetIndex() );					
				}
			}

		}
	}

	if( sizeof( CGridBlock ) * mWidth * mHeight > ( int )sizeof( mGrids ) )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Map( %s ) Width( %d ) Height( %d ) Bigger Than mGridsCountMax( %d )",
			__LK_FILE__, __LINE__, __FUNCTION__, pTplName, mWidth, mHeight, sizeof( mGrids ) );
		return ERR_UNKNOWN;
	}
	
	::fread( mGrids, sizeof( CGridBlock ), mWidth * mHeight, vpBlock );
	
	return SUCCESS;
}

int CMapTpl::Final( )
{
	for ( CREATOR_LIST_TYPE::iterator it = mCreators.begin(); it != mCreators.end(); ++it )
	{
		CCreator* pCreator = CCreatorManager::GetSingletonPtr()->GetCreator( *it );
		if( pCreator == NULL )
		{
			continue;
		}

		if( pCreator->GetAttachedEntityID() > 0 )
		{
			CEntity* pEntity = CSceneLogic::GetSingletonPtr()->GetEntity( pCreator->GetAttachedEntityID() );
			if( pEntity != NULL )
			{
				CSceneLogic::GetSingletonPtr()->DestroyEntity( pEntity, 0 );
			}
		}

		// 去掉刷新列表和Creator列表里的信息
		CCreatorManager::GetSingletonPtr()->RemoveCreator( pCreator );

		CSceneObjManager::GetSingletonPtr()->DestroyObject( pCreator->get_id() );
	
	}

	mCreators.clear();

	Initial( );
	
	return 0;
}


//--------------------------------------------------------------------
// CMapTplManager
//--------------------------------------------------------------------

CMapTplManager::CMapTplManager()
{
	if ( mShmPtr->GetInitMode() == SHM_INIT )
	{
		unsigned int i = 0;
		for( i = 0; i < ARRAY_CNT( mMapTpls ); i++ )
		{
			mMapTpls[ i ].Initial( );
		}
		mMapTplCount = 0;
	}
	else
	{

	}
}

CMapTplManager::~CMapTplManager( )
{

}

void* CMapTplManager::operator new( size_t tSize )
{
	return ( void* ) mShmPtr->CreateSegment( tSize );
}

void CMapTplManager::operator delete( void* pointer ) { }

void* CMapTplManager::operator new(size_t size, const char* file, int line)
{
	return operator new( size );
}
void CMapTplManager::operator delete(void* pointer, const char* file, int line) {}

bool CMapTpl::IsCreatorIn( int vCreatorIIndex )
{
	CREATOR_LIST_TYPE::iterator it;
	
	for( it = mCreators.begin( ); it != mCreators.end( ); it++ )
	{
		if( *it == vCreatorIIndex )
		{
			return true;
		}
	}	

	return false;
}



