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

#ifndef _LOGINOBJ_MANAGER_H_
#define _LOGINOBJ_MANAGER_H_

#include "entity.h"
#include "loginobj_define.h"
#include "servertool.h"
#include "array_list.h"
#include "shm.h"

class CObj;
class CLoginObjManagerImp;
class CSharedMem;


class CLoginObjManager : public CSingleton< CLoginObjManager > 
{
public:
	CLoginObjManager();
	~CLoginObjManager();

	void* operator new(size_t size);
	void  operator delete(void* pointer);

	int Initailize();
	int Resume();
	unsigned short GetGUID( );

	CObj*	CreateObject( EGlobalObjType vObjType );
	int		DestroyObject( int vObjID );
	CObj*	GetObject( int vObjID ); 

	static unsigned int CountSize( );

public:
	
	static CSharedMem* mShmPtr; // 共享内存指针


	// 游戏对象
	CLoginObjManagerImp* mLoginObjManagerImp;

	unsigned short mIncreasingCount;		// 装备自增的计数 
};

#endif
