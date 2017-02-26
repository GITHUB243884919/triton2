#include "container.h"
#include "stdafx.hpp"

//////////////////////////////////////////////////////////////////////////
// CSeenPlayerContainer
//////////////////////////////////////////////////////////////////////////
CSeenPlayerContainer::CSeenPlayerContainer()
{
	if( CObj::msCreateMode )
	{
		Initailize();
	}
	else
	{
		Resume();
	}
}


CSeenPlayerContainer::~CSeenPlayerContainer()
{

}

void CSeenPlayerContainer::Initailize()
{
	mPlayers.share_list_ptr__ = &CSceneObjManager::GetSingletonPtr()->mSharedSeenPlayers;
	mPlayers.initailize();
}

void CSeenPlayerContainer::Resume()
{
	mPlayers.share_list_ptr__ = &CSceneObjManager::GetSingletonPtr()->mSharedSeenPlayers;
}

void CSeenPlayerContainer::Add(unsigned int unID)
{
	mPlayers.push_back( unID );
}



void CSeenPlayerContainer::Remove( unsigned int unID)
{
	for( SeenPlayerVector::iterator it = mPlayers.begin(); it != mPlayers.end(); ++it )
	{
		if( *it == unID )
		{
			mPlayers.erase( it );
			return;
		}
	}

	LOG_ERROR( "default", "[%s:%d] %d dosn't int container", __FUNCTION__, __LINE__, unID );
}

void CSeenPlayerContainer::Show( const char* pszLogName )
{
	LOG_DEBUG( pszLogName, "CSeenPlayerContainer(size=%d),total(%d/%d) ", 
		mPlayers.size(), 
		mPlayers.share_list_ptr__->size(),
		mPlayers.share_list_ptr__->max_size());

	for( SeenPlayerVector::iterator it = mPlayers.begin(); it !=  mPlayers.end(); ++it )
	{
		LOG_DEBUG( pszLogName, "%d", *it );
	}
}


//////////////////////////////////////////////////////////////////////////
//	CSeenCharactorNpcContainer
//////////////////////////////////////////////////////////////////////////

CSeenCharactorNpcContainer::CSeenCharactorNpcContainer( )
{
	if( CObj::msCreateMode )
	{
		Initailize();
	}
	else
	{
		Resume();
	}
}

CSeenCharactorNpcContainer::~CSeenCharactorNpcContainer()
{

}

void CSeenCharactorNpcContainer::Initailize()
{
	mNpcCharactors.share_list_ptr__ = &CSceneObjManager::GetSingletonPtr()->mSharedSeenCharNpcs;
	mNpcCharactors.initailize();
}

void CSeenCharactorNpcContainer::Resume()
{
	mNpcCharactors.share_list_ptr__ = &CSceneObjManager::GetSingletonPtr()->mSharedSeenCharNpcs;
}


void CSeenCharactorNpcContainer::Add(unsigned int unID)
{
	mNpcCharactors.push_back( unID );
}

void CSeenCharactorNpcContainer::Remove( unsigned int unID)
{
	for( SeenNpcCharactorPool::iterator it = mNpcCharactors.begin(); it != mNpcCharactors.end(); ++it )
	{
		if( *it == unID )
		{
			mNpcCharactors.erase( it );
			break;
		}
	}
}

void CSeenCharactorNpcContainer::Show( const char* pszLogName )
{
	LOG_DEBUG( pszLogName, "CSeenCharactorNpcContainer(size=%d),total(%d/%d) ", 
		mNpcCharactors.size(), 
		mNpcCharactors.share_list_ptr__->size(),
		mNpcCharactors.share_list_ptr__->max_size());

	for( SeenNpcCharactorPool::iterator it = mNpcCharactors.begin(); it !=  mNpcCharactors.end(); ++it )
	{
		LOG_DEBUG( pszLogName, "%d", *it );
	}
}

//////////////////////////////////////////////////////////////////////////
// CSeenStaticNpcContainer
//////////////////////////////////////////////////////////////////////////

CSeenStaticNpcContainer::CSeenStaticNpcContainer()
{
	if( CObj::msCreateMode )
	{
		Initailize();
	}
	else
	{
		Resume();
	}
}

CSeenStaticNpcContainer::~CSeenStaticNpcContainer()
{

}

void CSeenStaticNpcContainer::Initailize()
{
	mStaticNpcs.share_list_ptr__ = &CSceneObjManager::GetSingletonPtr()->mSharedSeenStaticNpcs;
	mStaticNpcs.initailize();
}

void CSeenStaticNpcContainer::Resume()
{
	mStaticNpcs.share_list_ptr__ = &CSceneObjManager::GetSingletonPtr()->mSharedSeenStaticNpcs;
}

void CSeenStaticNpcContainer::Add(unsigned int unID)
{
	mStaticNpcs.push_back( unID );
}

void CSeenStaticNpcContainer::Remove( unsigned int unID)
{
	for( SeenNpcStaticVector::iterator it = mStaticNpcs.begin(); it != mStaticNpcs.end(); ++it )
	{
		if( *it == unID )
		{
			mStaticNpcs.erase( it );
			break;
		}
	}
}

void CSeenStaticNpcContainer::Show( const char* pszLogName )
{
	LOG_DEBUG( pszLogName, "CSeenStaticNpcContainer(size=%d),total(%d/%d) ", 
		mStaticNpcs.size(), 
		mStaticNpcs.share_list_ptr__->size(),
		mStaticNpcs.share_list_ptr__->max_size());

	for( SeenNpcStaticVector::iterator it = mStaticNpcs.begin(); it !=  mStaticNpcs.end(); ++it )
	{
		LOG_DEBUG( pszLogName, "%d", *it );
	}
}
