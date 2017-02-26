#include "gateobj_manager.h"
#include "gateobj_manager_imp.h"
#include "base.h"

static const char* EGlobalObjTypeName[] =
{
	"OBJTYPE_START",
	"OBJTYPE_PLAYER_AT_GATE",
	"OBJTYPE_SCENE_AT_GATE",
	"OBJTYPE_MAP_AT_GATE",
	"OBJTYPE_TEAM_AT_GATE",
	"OBJTYPE_AGIPSESSION",
	"OBJTYPE_SERVERSESSION",
	"OBJTYPE_ROLEGLOBALINFO",
	"OBJTYPE_REPETIONMAP",
	"OBJTYPE_FAMILY",
	"OBJTYPE_IBTRANS",
	"OBJTYPE_CORPS",
	"OBJTYPE_MEMBER",
	"OBJTYPE_ACTIVITYITEM",		// CActivityItem
	"OBJTYPE_END"
};

template<> CGateObjManager* CSingleton< CGateObjManager >::spSingleton = NULL;

CSharedMem* CGateObjManager::mShmPtr = NULL;

CGateObjManager::CGateObjManager()
{
	if ( mShmPtr->GetInitMode() == SHM_INIT )
	{
		Initialize();
	} 
	else
	{
		Resume();
	}
}

CGateObjManager::~CGateObjManager()
{

}

void* CGateObjManager::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CGateObjManager::operator delete( void* pointer ) 
{ 
}


unsigned int CGateObjManager::CountSize( )
{
	unsigned int tSize = sizeof( CGateObjManager );

	tSize += CGateObjManagerImp::CountSize();

	return tSize;
}

int CGateObjManager::Initialize()
{
	CGateObjManagerImp::mShmPtr = mShmPtr;
	mGateObjManagerImp = new CGateObjManagerImp;
	mGateObjManagerImp->Initialize( );

	mIncreasingCount = 0;
	return 0;
}

int CGateObjManager::Resume()
{
	CGateObjManagerImp::mShmPtr = mShmPtr;
	mGateObjManagerImp = new CGateObjManagerImp;

	return 0;
}

unsigned short CGateObjManager::GetGUID( )
{	
	if ( mIncreasingCount >= 1000 )
	{
		mIncreasingCount = 0;
	}
	return mIncreasingCount++;
}

CObj* CGateObjManager::CreateObject( EGlobalObjType vObjType )
{
	CObj* pObj = NULL;

	switch( vObjType )
	{
		case OBJTYPE_PLAYER_AT_GATE:
		{
			pObj = mGateObjManagerImp->mPlayerAtGatePool.create();
			break;
		}
		case OBJTYPE_SCENE_AT_GATE:
		{
			pObj = mGateObjManagerImp->mSceneAtGatePool.create( );
			break ;
		}
		case OBJTYPE_MAP_AT_GATE:
		{
			pObj = mGateObjManagerImp->mMapAtGatePool.create( );
			break ;
		}
		case OBJTYPE_TEAM_AT_GATE:
		{
			pObj = mGateObjManagerImp->mTeamAtGatePool.create( );
			break ;
		}
		case OBJTYPE_SERVERSESSION:
		{
			pObj = mGateObjManagerImp->mServerSessionPool.create();
			break ;
		}
		case OBJTYPE_ROLEGLOBALINFO:
		{
			pObj = mGateObjManagerImp->mRoleGlobalInfoPool.create();
			break ;
		}

		case OBJTYPE_REPETION_MAP:
		{
			pObj = mGateObjManagerImp->mRepetionMapPool.create( );	
			break;
		}		  		
		case OBJTYPE_FAMILY:
		{
			pObj = mGateObjManagerImp->mFamilyPool.create();
			break;
		}
		case OBJTYPE_MEMBER:
		{
			pObj = mGateObjManagerImp->mMemberPool.create();
			break;
		}

		#ifdef USING_ERATING
		case OBJTYPE_AGIPSESSION:
		{
			pObj = mGateObjManagerImp->mAGIPSessionPool.create();
			break ;
		}
		#endif

		case OBJTYPE_IBTRANS:
		{
			pObj = mGateObjManagerImp->mIbTransPool.create( );
			break;
		} 
		
		case OBJTYPE_CORPS:
		{
			pObj = mGateObjManagerImp->mCorpsPool.create( );
			break;
		}
		
		case OBJTYPE_ACTIVITYITEM:
		{
			pObj = mGateObjManagerImp->mActivityItemsPool.create();
			break;
		}

		case OBJTYPE_UNION:
		{
			pObj = mGateObjManagerImp->mUnionPool.create();
			break;
		}
		
		case OBJTYPE_LEAGUESHOP:
		{
			pObj = mGateObjManagerImp->mLeagueShop.create( );
			break;
		}
		default:
		{
			//TODO: log it 
			break;
		}
	}

	if ( pObj == NULL )
	{
		LOG_ERROR( "default" , "Create Object type(%d: %s ) failed", 
				vObjType , EGlobalObjTypeName[vObjType-1]);
	}else
	{
		//LOG_DEBUG( "default" , "Create Object(%d) type(%d: %s ) OK", 
		//		pObj->get_id(), vObjType , EGlobalObjTypeName[vObjType-1]);
	}

	return pObj;
}


int CGateObjManager::DestroyObject( int vObjID )
{
	switch( CObj::ID2TYPE( vObjID )  )
	{
		case OBJTYPE_PLAYER_AT_GATE:
		{
			mGateObjManagerImp->mPlayerAtGatePool.remove(vObjID);
			break;
		}
		case OBJTYPE_SCENE_AT_GATE:
		{
			mGateObjManagerImp->mSceneAtGatePool.remove(vObjID);
			break ;
		}
		case OBJTYPE_MAP_AT_GATE:
		{
			mGateObjManagerImp->mMapAtGatePool.remove(vObjID);
			break ;
		}
		case OBJTYPE_TEAM_AT_GATE:
		{
			mGateObjManagerImp->mTeamAtGatePool.remove(vObjID);
			break ;
		}
		case OBJTYPE_SERVERSESSION:
		{
			mGateObjManagerImp->mServerSessionPool.remove(vObjID);
			break ;
		}
		case OBJTYPE_ROLEGLOBALINFO:
		{
			mGateObjManagerImp->mRoleGlobalInfoPool.remove(vObjID);
			break ;
		}

		case OBJTYPE_REPETION_MAP:
		{
			mGateObjManagerImp->mRepetionMapPool.remove( vObjID );
			break;
		}		  		
		case OBJTYPE_FAMILY:
		{
			mGateObjManagerImp->mFamilyPool.remove( vObjID );
			break;
		}
		case OBJTYPE_MEMBER:
		{
			mGateObjManagerImp->mMemberPool.remove( vObjID );
			break;
		}

		#ifdef USING_ERATING
		case OBJTYPE_AGIPSESSION:
		{
			mGateObjManagerImp->mAGIPSessionPool.remove(vObjID);
			break ;
		}
		#endif

		case OBJTYPE_IBTRANS:
		{
			mGateObjManagerImp->mIbTransPool.remove( vObjID );
			break;
		}
		
		case OBJTYPE_CORPS:
		{
			mGateObjManagerImp->mCorpsPool.remove( vObjID ); 
			break;
		}
		case OBJTYPE_ACTIVITYITEM:
		{
			mGateObjManagerImp->mActivityItemsPool.remove( vObjID );
			break;
		}
		case OBJTYPE_UNION:
		{
			mGateObjManagerImp->mUnionPool.remove( vObjID );
			break;
		}
		
		case OBJTYPE_LEAGUESHOP:
		{
			mGateObjManagerImp->mLeagueShop.remove( vObjID );
			break;
		}

		default:
		{
			//TODO: log it 
			return -1;
		}
	}

	LOG_DEBUG( "default", "Destory ObjType(%s)  ObjID %d", 
			EGlobalObjTypeName[CObj::ID2TYPE(vObjID)-1], vObjID );

	return 0;	
}


CObj* CGateObjManager::GetObject( int vObjID )
{
	if( vObjID == INVALID_OBJ_ID )
	{
		return NULL;
	}

	CObj* pObj = NULL;

	switch( CObj::ID2TYPE( vObjID )  )
	{
		case OBJTYPE_PLAYER_AT_GATE:
		{
			pObj = mGateObjManagerImp->mPlayerAtGatePool.find_by_id(vObjID);
			break;
		}
		case OBJTYPE_SCENE_AT_GATE:
		{
			pObj = mGateObjManagerImp->mSceneAtGatePool.find_by_id(vObjID);
			break ;
		}
		case OBJTYPE_MAP_AT_GATE:
		{
			pObj = mGateObjManagerImp->mMapAtGatePool.find_by_id(vObjID);
			break ;
		}
		case OBJTYPE_TEAM_AT_GATE:
		{
			pObj = mGateObjManagerImp->mTeamAtGatePool.find_by_id(vObjID);
			break ;
		}
		case OBJTYPE_SERVERSESSION:
		{
			pObj = mGateObjManagerImp->mServerSessionPool.find_by_id(vObjID);
			break ;
		}
		case OBJTYPE_ROLEGLOBALINFO:
		{
			pObj = mGateObjManagerImp->mRoleGlobalInfoPool.find_by_id(vObjID);
			break ;
		}

		case OBJTYPE_REPETION_MAP:
		{
			pObj = mGateObjManagerImp->mRepetionMapPool.find_by_id( vObjID );
			break;
		}
		case OBJTYPE_FAMILY:
		{
			pObj = mGateObjManagerImp->mFamilyPool.find_by_id( vObjID );
			break;
		}	
		case OBJTYPE_MEMBER:
		{
			pObj = mGateObjManagerImp->mMemberPool.find_by_id( vObjID );
			break;
		}

		#ifdef USING_ERATING
		case OBJTYPE_AGIPSESSION:
		{
			pObj = mGateObjManagerImp->mAGIPSessionPool.find_by_id(vObjID);
			break ;
		}
		#endif

		case OBJTYPE_IBTRANS:
		{
			pObj = mGateObjManagerImp->mIbTransPool.find_by_id( vObjID );
			break;
		}  
		
		case OBJTYPE_CORPS:
		{
			pObj = mGateObjManagerImp->mCorpsPool.find_by_id( vObjID );
			break;
		}
		
		case OBJTYPE_ACTIVITYITEM:
		{
			pObj = mGateObjManagerImp->mActivityItemsPool.find_by_id( vObjID );
			break;
		}

		case OBJTYPE_UNION:
		{
			pObj = mGateObjManagerImp->mUnionPool.find_by_id( vObjID );
			break;
		}
		
		case OBJTYPE_LEAGUESHOP:
		{
			pObj = mGateObjManagerImp->mLeagueShop.find_by_id( vObjID );
			break;
		}

		default:
		{
			//TODO: log it 
			 LOG_ERROR( "default","[ %s : %d ][ %s ] type(id:%d, name:%s) not registerd", __LK_FILE__,
				__LINE__, __FUNCTION__, CObj::ID2TYPE( vObjID ), EGlobalObjTypeName[CObj::ID2TYPE(vObjID)-1]);
			break;
		}
	}

	return pObj;	
}                       


void CGateObjManager::Dump( char* pBuffer, unsigned int& unLen )
{
	unsigned int unMaxLen = unLen;
	unLen = 0;

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "--------------------------------------------------------\n");

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6s\t%6s\n", "name", "free", "total");

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mPlayerAtGatePool" ,
		mGateObjManagerImp->mPlayerAtGatePool.size(), 
		mGateObjManagerImp->mPlayerAtGatePool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mSceneAtGatePool" ,
		mGateObjManagerImp->mSceneAtGatePool.size(), 
		mGateObjManagerImp->mSceneAtGatePool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mMapAtGatePool" ,
		mGateObjManagerImp->mMapAtGatePool.size(), 
		mGateObjManagerImp->mMapAtGatePool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mRoleGlobalInfoPool" ,
		mGateObjManagerImp->mRoleGlobalInfoPool.size(), 
		mGateObjManagerImp->mRoleGlobalInfoPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mTeamAtGatePool" ,
		mGateObjManagerImp->mTeamAtGatePool.size(), 
		mGateObjManagerImp->mTeamAtGatePool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mRepetionMapPool" ,
		mGateObjManagerImp->mRepetionMapPool.size(), 
		mGateObjManagerImp->mRepetionMapPool.max_size() );
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mFamilyPool" ,
		mGateObjManagerImp->mFamilyPool.size(), 
		mGateObjManagerImp->mFamilyPool.max_size() );
#ifdef USING_ERATING        
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mAGIPSessionPool" ,
		mGateObjManagerImp->mAGIPSessionPool.size(), 
		mGateObjManagerImp->mAGIPSessionPool.max_size() );                                                             
#endif                         

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mServerSessionPool" ,
		mGateObjManagerImp->mServerSessionPool.size(), 
		mGateObjManagerImp->mServerSessionPool.max_size() );
	
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mIbTransPool",
			mGateObjManagerImp->mIbTransPool.size(),
			mGateObjManagerImp->mIbTransPool.max_size() );
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mCorpsPool",
		mGateObjManagerImp->mCorpsPool.size(),
		mGateObjManagerImp->mCorpsPool.max_size() );
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mMemberPool",
		mGateObjManagerImp->mMemberPool.size(),
		mGateObjManagerImp->mMemberPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mActivityItemsPool",
			mGateObjManagerImp->mActivityItemsPool.size(),
			mGateObjManagerImp->mActivityItemsPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mUnionPool",
		mGateObjManagerImp->mUnionPool.size(),
		mGateObjManagerImp->mUnionPool.max_size() );
	
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mLeagueShop",
		mGateObjManagerImp->mLeagueShop.size(),
		mGateObjManagerImp->mLeagueShop.max_size() );
}
