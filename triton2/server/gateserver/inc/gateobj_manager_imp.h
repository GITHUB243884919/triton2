/******************************************************************************
	版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

******************************************************************************
	文件名		: sceneobj_manager_imp.h
	版本号		: 1.0
	作者			: ZHL
	生成日期		: 2008-07-09
	最近修改		: 
	功能描述		: 管理场景服务器游戏对象,创建在共享内存中 
	函数列表		: 
	修改历史		: 
		1 日期	: 2008-07-09
		作者		: ZHL
		修改内容	: 生成
*******************************************************************************/
#ifndef _GATEOBJ_MANAGER_IMP_H_
#define _GATEOBJ_MANAGER_IMP_H_

#include "object.h"
#include "gateobj_define.h"
#include "objmanager.h"
#include "entity.h"
#include "shm.h"
#include "gateteam.h"
#include "gatelogic.h"
#include "repetionmodule.h"
#include "family.h"
#include "corps.h"
#include "corpsmanager.h"

#define OBJ_ID_START(obj_type)	(((obj_type) << OBJ_ID_BITS) + 1)
#define OBJ_ID_END(obj_type)	((((obj_type) + 1) << OBJ_ID_BITS) - 10)
#define OBJ_ID_COUNT(obj_type, count) OBJ_ID_START(obj_type), count, OBJ_ID_END(obj_type)


using namespace GateServer;

class CSharedMem;


class CGateObjManagerImp 
{
public:
	// 游戏对象
	typedef ObjManager< CGatePlayer,	OBJ_ID_COUNT( OBJTYPE_PLAYER_AT_GATE,	PLAYER_AT_GATE_CAP) > ObjMng_PlayerAtGate;
	typedef ObjManager< CSceneServer,	OBJ_ID_COUNT( OBJTYPE_SCENE_AT_GATE,	SCENE_AT_GATE_CAP)	> ObjMng_SceneAtGate;
	typedef ObjManager< CSceneMap,		OBJ_ID_COUNT( OBJTYPE_MAP_AT_GATE,		MAP_AT_GATE_CAP)	> ObjMng_MapAtGate;
	typedef ObjManager< CServerSession, OBJ_ID_COUNT( OBJTYPE_SERVERSESSION,	SERVERSESSION_CAP)  > ObjMng_ServerSession;
	typedef ObjManager< CRoleGlobalInfo,OBJ_ID_COUNT( OBJTYPE_ROLEGLOBALINFO,	ROLEGLOBALINFO_CAP) > ObjMng_RoleGlobalInfo;
	typedef ObjManager< CGateTeam,		OBJ_ID_COUNT( OBJTYPE_TEAM_AT_GATE,		TEAM_AT_GATE_CAP)   > ObjMng_TeamAtGate;
	typedef ObjManager< CRepetion,		OBJ_ID_COUNT( OBJTYPE_REPETION_MAP,		MAP_REPETION_CAP )  > ObjMng_RepetionInfo;
	typedef ObjManager< CFamily,		OBJ_ID_COUNT( OBJTYPE_FAMILY,			FAMILY_CAP )	    > ObjMng_Family;			

	typedef ObjManager< CIbTrans,       OBJ_ID_COUNT( OBJTYPE_IBTRANS,          SERVER_CAP_IBTRANS) > ObjMng_IbTrans;
	typedef ObjManager< CGateTeamMember, OBJ_ID_COUNT( OBJTYPE_MEMBER,          PLAYER_AT_GATE_CAP) > ObjMng_Member;
	typedef ObjManager< CActivityItem,	OBJ_ID_COUNT( OBJTYPE_ACTIVITYITEM, 	ACTIVITYITEM_CAP) 	> ObjMng_ActivityItems;
	typedef ObjManager< CCorps,			OBJ_ID_COUNT( OBJTYPE_CORPS,			SERVER_CAP_CORPS)	> ObjMng_Corps;
	typedef ObjManager< CUnion,			OBJ_ID_COUNT( OBJTYPE_UNION,			SERVER_CAP_CORPS)	> ObjMng_Union;
	typedef ObjManager< CLeagueShopInfo,OBJ_ID_COUNT( OBJTYPE_LEAGUESHOP,		LEAGUESHOP_CAP )	> ObjMng_LEAGUESHOP;

	#ifdef USING_ERATING
	typedef ObjManager< CAGIPSession, OBJ_ID_COUNT( OBJTYPE_AGIPSESSION, AGIPSESSION_CAP) >  ObjMng_AGIPSession;
	#endif
public:

	CGateObjManagerImp();
	~CGateObjManagerImp();

	void* operator new( size_t size );
	void  operator delete( void* pointer );

	int Initialize();
	int Resume();

	static unsigned int CountSize();

public:

	static CSharedMem* mShmPtr; // 共享内存指针

	// 游戏对象
	ObjMng_PlayerAtGate mPlayerAtGatePool;
	ObjMng_SceneAtGate	mSceneAtGatePool;
	ObjMng_MapAtGate    mMapAtGatePool;

	ObjMng_RoleGlobalInfo mRoleGlobalInfoPool;

	ObjMng_TeamAtGate	mTeamAtGatePool;

	ObjMng_RepetionInfo mRepetionMapPool;
	ObjMng_Family		mFamilyPool;	
	#ifdef USING_ERATING                                                                                        
	ObjMng_AGIPSession mAGIPSessionPool;                                                                    
	#endif                                                                                                      
	ObjMng_ServerSession mServerSessionPool;

	ObjMng_IbTrans 		mIbTransPool;
	ObjMng_Corps		mCorpsPool;
	ObjMng_Member		mMemberPool;

	ObjMng_ActivityItems mActivityItemsPool;
	ObjMng_Union		mUnionPool;
	ObjMng_LEAGUESHOP	mLeagueShop;
};

#endif


