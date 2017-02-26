#include "dunobjmanager.h"
template<> CDunobjManager *CSingleton<CDunobjManager>::spSingleton = NULL;
CSharedMem *CDunobjManager::m_pShareMem = NULL;
CDunobjManager::CDunobjManager()
{
	if ( CObj::msCreateMode )
	{
		Initialize();
	}
	else
	{
		Resume();
	}
}

// 对象管理器初始化
int CDunobjManager::Initialize( )
{
	mWarRegObjManager.initailize();
	return 0;
}

// 创建对象
CObj*CDunobjManager::CreateObject( EObjType vObjType )
{	
	CObj* pObj = NULL;
	switch( vObjType )
	{
		case OBJTYPE_REGINFO:
		{
			pObj = mWarRegObjManager.create( );
			break;
		}
	}
	return pObj;
}

// 销毁对象
int CDunobjManager::DestroyObject( int vObjID )
{
	switch( CObj::ID2TYPE( vObjID )  )
	{
	case OBJTYPE_REGINFO:
		{
			mWarRegObjManager.remove( vObjID );
			break;
		}
	default:
		return -1;
	}
	return 0;
}

// 获取某个对象
CObj*	CDunobjManager::GetObject( int vObjID )
{
	if( vObjID == INVALID_OBJ_ID )
	{
		return NULL;
	}
	CObj* pObj = NULL;
	switch( CObj::ID2TYPE( vObjID )  )
	{
	case OBJTYPE_REGINFO:
		{
			pObj = mWarRegObjManager.find_by_id( vObjID );
			break;
		}
	default:
		break;
	}
	return pObj;
}

void * CDunobjManager::operator new ( size_t nSize )
{
	return m_pShareMem->CreateSegment( nSize );
}
void  CDunobjManager::operator delete( void *pBuffer )
{

}
unsigned int CDunobjManager::CountSize()
{
	return sizeof( CDunobjManager );
}

int CDunobjManager::Resume()
{
	mWarRegObjManager.resume();
	return 0;
}