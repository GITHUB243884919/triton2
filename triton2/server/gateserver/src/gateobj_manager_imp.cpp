#include "gateobj_manager_imp.h"

CSharedMem* CGateObjManagerImp::mShmPtr = NULL; 

CGateObjManagerImp::CGateObjManagerImp( )
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

CGateObjManagerImp::~CGateObjManagerImp()
{
}


void* CGateObjManagerImp::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CGateObjManagerImp::operator delete( void* pointer ) 
{ }

unsigned int CGateObjManagerImp::CountSize()
{
	LOG_DEBUG("gateserver", "CGatePlayer:	%10d * %5d = %10d", 
			sizeof(CGatePlayer), PLAYER_AT_GATE_CAP, sizeof(ObjMng_PlayerAtGate));

	LOG_DEBUG("gateserver", "CSceneServer:	%10d * %5d = %10d", 
			sizeof(CSceneServer), SCENE_AT_GATE_CAP, sizeof(ObjMng_SceneAtGate));

	LOG_DEBUG("gateserver", "CSceneMap:	%10d * %5d = %10d", 
			sizeof(CSceneMap), MAP_AT_GATE_CAP,	sizeof( ObjMng_MapAtGate));

	LOG_DEBUG("gateserver", "CServerSession:   %10d * %5d = %10d",
			sizeof(CServerSession), SERVERSESSION_CAP, sizeof(ObjMng_ServerSession));

	LOG_DEBUG("gateserver", "CRoleGlobalInfo:   %10d * %5d = %10d",
			sizeof(CRoleGlobalInfo), ROLEGLOBALINFO_CAP, sizeof(ObjMng_RoleGlobalInfo));

	#ifdef USING_ERATING
	LOG_DEBUG("gateserver", "CAGIPSession: %10d * %5d = %10d",
			sizeof(CAGIPSession), AGIPSESSION_CAP,  sizeof(ObjMng_AGIPSession));
	#endif

	LOG_DEBUG("gateserver", "CIbTrans:   %10d * %5d = %10d",
			sizeof(CIbTrans), SERVER_CAP_IBTRANS, sizeof(ObjMng_IbTrans));

	LOG_DEBUG("gateserver", "CIbTrans:   %10d * %5d = %10d",
			sizeof(CIbTrans), SERVER_CAP_IBTRANS, sizeof(ObjMng_ActivityItems));
	
	LOG_DEBUG("gateserver", "CLeagueShop:   %10d * %5d = %10d",
		sizeof(CLeagueShopInfo), LEAGUESHOP_CAP, sizeof(ObjMng_LEAGUESHOP));
	
	return sizeof( CGateObjManagerImp );
}

int CGateObjManagerImp::Initialize( )
{
	mPlayerAtGatePool.initailize_pool( );
	mSceneAtGatePool.initailize_pool( );
	mMapAtGatePool.initailize_pool( );
	mTeamAtGatePool.initailize_pool( );
	mServerSessionPool.initailize_pool( );
	mRoleGlobalInfoPool.initailize_pool( );
	mRepetionMapPool.initailize_pool( );
	mFamilyPool.initailize_pool();
	mIbTransPool.initailize_pool();
	mMemberPool.initailize();
	mActivityItemsPool.initailize_pool();
	mCorpsPool.initailize_pool();
	mUnionPool.initailize_pool();
	mLeagueShop.initailize_pool();

	#ifdef USING_ERATING
	mAGIPSessionPool.initailize_pool( );
	#endif

	return 0;
}

int CGateObjManagerImp::Resume()
{
	mPlayerAtGatePool.resume_pool( );
	mSceneAtGatePool.resume_pool( );
	mMapAtGatePool.resume_pool( );
	mTeamAtGatePool.resume_pool( );
	mServerSessionPool.resume_pool( );
	mRoleGlobalInfoPool.resume_pool( );
	mRepetionMapPool.resume_pool( );
	mFamilyPool.resume_pool();
	mIbTransPool.resume_pool();
	mMemberPool.resume_pool();
	mActivityItemsPool.resume_pool();
	mCorpsPool.resume_pool();
	mUnionPool.resume_pool();
	mLeagueShop.resume_pool();

#ifdef USING_ERATING
	mAGIPSessionPool.resume_pool( );
#endif

	return 0;
}
