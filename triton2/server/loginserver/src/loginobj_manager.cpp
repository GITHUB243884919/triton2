#include "loginobj_manager.h"
#include "loginobj_manager_imp.h"
#include "base.h"

static const char* EGlobalObjTypeName[] = 
{
	"OBJTYPE_START",
	"OBJTYPE_PLAYER_AT_LOGIN",
	"OBJTYPE_AGIPSESSION",
	"OBJTYPE_SERVERSESSION",
	"OBJTYPE_IBPAYAUDIT",
	"OBJTYPE_END"
};


template<> CLoginObjManager* CSingleton< CLoginObjManager >::spSingleton = NULL;

CSharedMem* CLoginObjManager::mShmPtr = NULL;

CLoginObjManager::CLoginObjManager()
{
	if ( mShmPtr->GetInitMode() == SHM_INIT )
	{
		Initailize();
	} 
	else
	{
		Resume();
	}
}

CLoginObjManager::~CLoginObjManager()
{

}

void* CLoginObjManager::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CLoginObjManager::operator delete( void* pointer ) 
{ 
}


unsigned int CLoginObjManager::CountSize( )
{
	unsigned int tSize = sizeof( CLoginObjManager );

	tSize += CLoginObjManagerImp::CountSize();

	return tSize;
}

int CLoginObjManager::Initailize()
{
	CLoginObjManagerImp::mShmPtr = mShmPtr;
	mLoginObjManagerImp = new CLoginObjManagerImp;
	mLoginObjManagerImp->Initailize( );

	mIncreasingCount = 0;
	return 0;
}

int CLoginObjManager::Resume()
{
	CLoginObjManagerImp::mShmPtr = mShmPtr;

	return 0;
}

unsigned short CLoginObjManager::GetGUID( )
{	
	if ( mIncreasingCount >= 1000 )
	{
		mIncreasingCount = 0;
	}
	return mIncreasingCount++;
}

CObj* CLoginObjManager::CreateObject( EGlobalObjType vObjType )
{
	CObj* pObj = NULL;

	switch( vObjType )
	{
		case OBJTYPE_PLAYER_AT_LOGIN:
		{
			pObj = mLoginObjManagerImp->mPlayerAtLoginPool.create();
			break;
		}

		case OBJTYPE_SERVERSESSION:
		{
			pObj = mLoginObjManagerImp->mServerSessionPool.create();
			break ;
		}

		#ifdef USING_ERATING
		case OBJTYPE_AGIPSESSION:
		{
			pObj = mLoginObjManagerImp->mAGIPSessionPool.create();
			break ;
		}

		case OBJTYPE_IBPAYAUDIT:
		{
			pObj = mLoginObjManagerImp->mIbPayAuditPool.create();
			break;
		}
		#endif

		default:
		{
			//TODO: log it 
			break;
		}
	}

	if ( pObj == NULL )
	{
		LOG_ERROR( "default" , "Create Object type(%d:  ) failed", vObjType );
	}else
	{
		LOG_DEBUG( "default" , "Create Object(%d)  type(%d: %s ) OK", pObj->get_id(), vObjType , EGlobalObjTypeName[vObjType-1]);
	}

	return pObj;
}


int CLoginObjManager::DestroyObject( int vObjID )
{
	//CObj* pObj = NULL;

	switch( CObj::ID2TYPE( vObjID )  )
	{
		case OBJTYPE_PLAYER_AT_LOGIN:
		{
			mLoginObjManagerImp->mPlayerAtLoginPool.remove(vObjID);
			break;
		}

		case OBJTYPE_SERVERSESSION:
		{
			mLoginObjManagerImp->mServerSessionPool.remove(vObjID);
			break ;
		}

		#ifdef USING_ERATING
		case OBJTYPE_AGIPSESSION:
		{
			mLoginObjManagerImp->mAGIPSessionPool.remove(vObjID);
			break ;
		}

		case OBJTYPE_IBPAYAUDIT:
		{
			mLoginObjManagerImp->mIbPayAuditPool.remove(vObjID);
			break;
		}
		#endif

		default:
		{
			//TODO: log it 
			return -1;
		}
	}

	LOG_DEBUG( "default", "Destory ObjType(%s)  ObjID %d", EGlobalObjTypeName[CObj::ID2TYPE(vObjID)-1], vObjID );

	return 0;	
}


CObj* CLoginObjManager::GetObject( int vObjID )
{
	if( vObjID == INVALID_OBJ_ID )
	{
		return NULL;
	}

	CObj* pObj = NULL;

	switch( CObj::ID2TYPE( vObjID )  )
	{
		case OBJTYPE_PLAYER_AT_LOGIN:
		{
			pObj = mLoginObjManagerImp->mPlayerAtLoginPool.find_by_id(vObjID);
			break;
		}

		case OBJTYPE_SERVERSESSION:
		{
			pObj = mLoginObjManagerImp->mServerSessionPool.find_by_id(vObjID);
			break ;
		}

		#ifdef USING_ERATING
		case OBJTYPE_AGIPSESSION:
		{
			pObj = mLoginObjManagerImp->mAGIPSessionPool.find_by_id(vObjID);
			break ;
		}

		case OBJTYPE_IBPAYAUDIT:
		{
			pObj = mLoginObjManagerImp->mIbPayAuditPool.find_by_id(vObjID);
			break;
		}
		#endif

		default:
		{
			//TODO: log it 
			LOG_ERROR( "default","[ %s : %d ][ %s ] type(id:%d) not registerd", __LK_FILE__, 
					__LINE__, __FUNCTION__, CObj::ID2TYPE( vObjID ) );
			break;
		}
	}

	return pObj;	
}                       

