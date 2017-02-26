#include "stdafx.hpp"
#include "mail.h"
#include "mailmanager.h"
#include "mailmodule.h"

template<> CMailManager* CSingleton< CMailManager >::spSingleton = NULL;
CSharedMem* CMailManager::mShmPtr = NULL; 

CMail* CMailManager::AddMail( CEntityPlayer *pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return NULL );
	CMail * tpMail = ( CMail * )CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_MAIL_MAILINFOR );  
	LK_ASSERT( tpMail != NULL , return NULL);		
	pPlayer->SetMailObjID( tpMail->get_id() );
	tpMail->SetEntityID( pPlayer->GetEntityID() );	
	return   tpMail;                                               
}

void CMailManager::Erase( CEntityPlayer *pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return );
	CSceneObjManager::GetSingletonPtr()->DestroyObject( pPlayer->GetMailObjID() );
	pPlayer->SetMailObjID( 0 );
}

CMail* CMailManager::GetMail( CEntityPlayer *pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return NULL )
	CMail* tpMail = ( CMail* )CSceneObjManager::GetSingletonPtr()->GetObject( pPlayer->GetMailObjID() );
	return tpMail;
}

CMailManager::~CMailManager()
{
	
}
void*  CMailManager::operator new( size_t size ) 
{
	return ( void* ) mShmPtr->CreateSegment( size );
}
void  CMailManager::operator delete( void *pointer )
{

}

void* CMailManager::operator new(size_t size, const char* file, int line)
{
	return operator new( size ) ;
}

void CMailManager::operator delete(void* pointer, const char* file, int line)
{

}

unsigned int  CMailManager::CountSize()
{
	return sizeof( CMailManager );
}

int CMailManager::Initialize()
{
	mSysMailList.initailize();
	return 0;	
}

// 插入一个系统邮件的信息
int CMailManager::InsertSysMail( int nMailID, CSysMailInfo &rMailInfo )
{ 
	mSysMailList.insert( SysMail::value_type( nMailID, rMailInfo ) );
	return SUCCESS;	
}

// 删除一个系统邮件的信息
int CMailManager::DeleteSysMail( int nMailID )
{
	SysMail::iterator it = mSysMailList.find( nMailID );
	if ( it != mSysMailList.end() )
	{
		mSysMailList.erase( it );
	}
	return SUCCESS;
}

// 获取一个系统邮件的信息
CSysMailInfo *CMailManager::GetSysMail( int nMailID )
{
	SysMail::iterator it = mSysMailList.find( nMailID );
	if ( it != mSysMailList.end() )
	{
		return &(it->second);
	}
	return NULL;
}
