#include "loginobj_manager_imp.h"

CSharedMem* CLoginObjManagerImp::mShmPtr = NULL; 

CLoginObjManagerImp::CLoginObjManagerImp( )
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

CLoginObjManagerImp::~CLoginObjManagerImp()
{
}


void* CLoginObjManagerImp::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CLoginObjManagerImp::operator delete( void* pointer ) 
{ }

unsigned int CLoginObjManagerImp::CountSize()
{
	LOG_DEBUG("loginserver", "CLoginPlayer:	%10d * %5d = %10d", 
			sizeof(CLoginPlayer), PLAYER_AT_LOGIN_CAP, sizeof(ObjMng_PlayerAtLogin));

	LOG_DEBUG("loginserver", "CServerSession:	%10d * %5d = %10d", 
			sizeof(CServerSession), SERVERSESSION_CAP, sizeof(ObjMng_ServerSession));

#ifdef USING_ERATING
	LOG_DEBUG("loginserver", "CAGIPSession:	%10d * %5d = %10d", 
			sizeof(CAGIPSession), AGIPSESSION_CAP,	sizeof(ObjMng_AGIPSession));

	LOG_DEBUG("loginserver", "CIbPayAudit:	%10d * %5d = %10d", 
			sizeof(CIbPayAudit), IBPAYAUDIT_CAP,	sizeof(ObjMng_IbPayAudit));
#endif

	return sizeof( CLoginObjManagerImp );
}

int CLoginObjManagerImp::Initailize( )
{
	mPlayerAtLoginPool.initailize_pool( );
	mServerSessionPool.initailize_pool( );
#ifdef USING_ERATING
	mAGIPSessionPool.initailize_pool( );
	mIbPayAuditPool.initailize_pool();
#endif

	return 0;
}

int CLoginObjManagerImp::Resume()
{
	mPlayerAtLoginPool.resume_pool( );
	mServerSessionPool.resume_pool( );
#ifdef USING_ERATING
	mAGIPSessionPool.resume_pool( );
	mIbPayAuditPool.resume_pool();
#endif

	return 0;
}
