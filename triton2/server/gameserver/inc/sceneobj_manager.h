/******************************************************************************
   版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

******************************************************************************
  文件名          : sceneobj_manager.h
  版本号          : 1.0
  作者            : ZHL
  生成日期        : 2008-06-05
  最近修改        : 
  功能描述        : 提供游戏对象创建，删除，查找接口。用接口和实现分离的方式将编译依赖降低。
  函数列表        : 
  修改历史        : 
  1 日期          : 2008-06-05
    作者          : ZHL
    修改内容      : 生成
*******************************************************************************/

#pragma once

#include "sceneobj_define.h"
#include "servertool.h"
#include "lk_assert.h"
#include "lk_sharedvector.h"
#include "array_list.h"
#include "objmanager.h"

class CObj;
class CSceneObjManagerImp;
class CSharedMem;


class CSceneObjManager : public CSingleton< CSceneObjManager > 
{
public:
	CSceneObjManager();
	~CSceneObjManager();

	void* operator new(size_t size);
	void  operator delete(void* pointer);
	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);

	int Initailize();
	int Resume();
	unsigned short GetGUID( );

	CObj*	CreateObject( EGlobalObjType vObjType );
	int		DestroyObject( int vObjID );
	CObj*	GetObject( int vObjID ); 

	// 对象池使用情况输出
	void Dump( char* pBuffer, unsigned int& unLen );

	static unsigned int CountSize( );

public:
	typedef lk::fixed_size_memory_pool< int, SERVER_CAP_SHARE_CELLENTITY, 2> DataMng_SharedCellsEntityID;	


	typedef lk::fixed_size_memory_pool< unsigned int, SERVER_CAP_SHARE_SEEN_PLAYER, 2>	DataMng_SharedSeenPlayers;
	typedef lk::fixed_size_memory_pool< unsigned int, SERVER_CAP_SHARE_SEEN_CHARNPC, 2>	DataMng_SharedSeenCharNpcs;
	typedef lk::fixed_size_memory_pool< unsigned int, SERVER_CAP_SHARE_SEEN_STATICNPC,2>	DataMng_SharedSeenStaticNpcs;

public:
	
	static CSharedMem* mShmPtr; // 共享内存指针


	// 游戏对象
	CSceneObjManagerImp* mSceneObjManagerImp;

	// 游戏数据，主要是各种ObjectID的容器
	DataMng_SharedCellsEntityID	mSharedCellsEntityID;

	DataMng_SharedSeenPlayers		mSharedSeenPlayers;
	DataMng_SharedSeenCharNpcs		mSharedSeenCharNpcs;
	DataMng_SharedSeenStaticNpcs	mSharedSeenStaticNpcs;

	unsigned short mIncreasingCount;		// 装备自增的计数 

};


