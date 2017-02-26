#pragma once
#include "objmanager.h"
#include "servertool.h"
#include "lk_string.h"
#include "tinyxml.h"
#include "dungeontype.h"
#include "sharemem.h"
#include "shm.h"
#include "dungeonmanager.h"
#define OBJ_ID_START(obj_type)	(((obj_type) << OBJ_ID_BITS) + 1)
#define OBJ_ID_END(obj_type)	((((obj_type) + 1) << OBJ_ID_BITS) - 10)
#define OBJ_ID_COUNT(obj_type, count) OBJ_ID_START(obj_type), count, OBJ_ID_END(obj_type)
class CDunobjManager : public CSingleton< CDunobjManager >
{	
public:
	typedef ObjManager< CWarRegInfo, OBJ_ID_COUNT( OBJTYPE_REGINFO, MAX_WARREGINFO_NUM ) > CWarRegObjManager;
public:
	CDunobjManager();
	~CDunobjManager(){ }
	int Initialize( );
	int Resume();
public:
	static CSharedMem *m_pShareMem;
	void * operator new ( size_t nSize );
	void  operator delete( void *pBuffer );
	static unsigned int CountSize();
public:	
	CObj*	CreateObject( EObjType vObjType );
	int		DestroyObject( int vObjID );
	CObj*	GetObject( int vObjID ); 
public:
	CWarRegObjManager mWarRegObjManager;
};