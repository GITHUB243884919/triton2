// ********************************************************************** //
// MapEntity.cpp
// 场景实体的定义
// Author : Black
// ********************************************************************** //

#include "servercore.h"
#include "maptplmanager.h"
#include "mapentity.h"
#include "mapobjmanager.h"
#include "sceneobj_manager.h"
#include "proclient_svr.h"
#include "coremessage_pb.hxx.pb.h"
#include "property.h"

// ********************************************************************** //
// CMapEntity
// ********************************************************************** //

void CMapEntity::AddViewEntity( CMapEntity* pEntity )
{
	LK_ASSERT( pEntity != NULL, return );

	CEntity* pRawEntity = pEntity->GetSelfEntity();
	LK_ASSERT( pRawEntity != NULL, return  );

	if ( pRawEntity->IsPedal() )
		mSeenPedals.push_back( pEntity->get_id() );

	// 人跟人,人跟怪都可以互相添加,当怪跟怪时,要按情况划分
	if ( GetSelfEntity()->IsPlayer() == false && pEntity->GetSelfEntity()->IsPlayer() == false )
	{
		// 当2个实体其中一个为功能NPC,物品,矿,钱时,不添加可见表
		if ( 
			( GetSelfEntity()->IsFunNpc() ||
			( GetSelfEntity()->IsItem()  ||
			( GetSelfEntity()->IsMine()||
			( pEntity->GetSelfEntity()->IsFuncNpc() ||
			( pEntity->GetSelfEntity()->IsItem() ||
			( pEntity->GetSelfEntity()->IsMine()
			)
		{
			return;
		}

		// 当2个实体都为ogre时,不添加可见表
		if ( GetSelfEntity()->IsOgre() &&  pEntity->GetSelfEntity()->IsOgre() )
		{
			return;
		}

		// 当2个实体都为陷阱时,不添加可见表
		if ( GetSelfEntity()->IsPedal() && pEntity->GetSelfEntity()->IsPedal() )
		{
			return;
		}
	}
	
	mSeenEntitys.push_back( pEntity->get_id() );
}

void CMapEntity::DeleteViewEntity( int nMapEntityObjID )
{

	SeenEntityVector::iterator tIteratorSeenEntity = mSeenEntitys.begin();
	SeenEntityVector::iterator tIteratorSeenEntityEnd = mSeenEntitys.end();

	for ( ; tIteratorSeenEntity != tIteratorSeenEntityEnd; ++tIteratorSeenEntity )
	{
		int tMapEntityObjID = *tIteratorSeenEntity ;

		CMapEntity* tpMapEntity = (CMapEntity*) CSceneObjManager::GetSingletonPtr()->GetObject( tMapEntityObjID );
		if( tpMapEntity == NULL )
		{
			LOG_ERROR( "default", "Can't find MapEntity %d in MapEntity(%d)'s SeenEntityVector", tMapEntityObjID, this->get_id() );
			continue;
		}

		if ( tpMapEntity->get_id() == nMapEntityObjID )
		{
			mSeenEntitys.erase( tIteratorSeenEntity );
			break;
		}
	}


	SeenPedalVector::iterator tIteratorSeenPedal = mSeenPedals.begin();
	SeenPedalVector::iterator tIteratorSeenPedalEnd = mSeenPedals.end();
	for ( ; tIteratorSeenPedal != tIteratorSeenPedalEnd; ++tIteratorSeenPedal )
	{
		
		CMapEntity* tpMapEntityPedal = (CMapEntity*) CSceneObjManager::GetSingletonPtr()->GetObject( *tIteratorSeenPedal );
		LK_ASSERT( tpMapEntityPedal != NULL, continue );

		if ( tpMapEntityPedal->get_id() == nMapEntityObjID )
		{
			mSeenPedals.erase( tIteratorSeenPedal );
			break;
		}
	}
}

CEntity* CMapEntity::GetSelfEntity(  )
{
	return (CEntity*) CSceneObjManager::GetSingletonPtr( )->GetObject( mEntityID );
}

int CMapEntity::Initialize()
{
	mSeenEntitys.share_list_ptr__ = &CSceneObjManager::GetSingletonPtr( )->mSharedSeenEntityID;
	mSeenPedals.share_list_ptr__ = &CSceneObjManager::GetSingletonPtr( )->mSharedSeenPedalID;

	mSeenEntitys.initailize();
	mSeenPedals.initailize();

	return 0;
}

int CMapEntity::Resume()
{
	mSeenEntitys.share_list_ptr__ = &CSceneObjManager::GetSingletonPtr( )->mSharedSeenEntityID;
	mSeenPedals.share_list_ptr__ = &CSceneObjManager::GetSingletonPtr( )->mSharedSeenPedalID;

	return 0;
}

CMapEntity::CMapEntity ()
{
	if ( CObj::msCreateMode )
	{
		Initialize ();
	}
	else
	{
		Resume();
	}
}

CMapEntity::~CMapEntity ()
{

}


// ********************************************************************** //
// CMapNpc
// ********************************************************************** //
int CMapNpc::GetNpcID( )
{ 
	CEntityNpc* tpEntityNpc = (CEntityNpc*)GetSelfEntity();
	LK_ASSERT( tpEntityNpc != NULL, return 0 );

	return tpEntityNpc->mNpcID;
}




int CMapEntity::SendMessageToSeenEntitys( CMessage* tpMessage )
{
	LK_ASSERT( tpMessage != NULL, return -1 );

	// 如果可见表为空， 直接返回
	if( mSeenEntitys.size( ) == 0 )
	{
		return 0;
	}

	SeenEntityVector::iterator tBegin = mSeenEntitys.begin();
	SeenEntityVector::iterator tEnd = mSeenEntitys.end();

	// 发送目的set
	std::set<CEntity*> tSendSet;

	CEntity* tpEntity = NULL;
	CMapEntity* tpMapEntity = NULL;
	for ( ; tBegin != tEnd; ++ tBegin )
	{
		tpMapEntity = ( CMapEntity* ) CSceneObjManager::GetSingletonPtr()->GetObject( *tBegin );
		if( tpMapEntity == NULL )
		{
			LOG_ERROR( "default", "MapEntity( %d ) dosn't exist", *tBegin );
			continue;
		}
		
		tpEntity = tpMapEntity->GetSelfEntity();
		if( tpEntity != NULL && tpEntity->GetEntityType() == ENTITYTYPE_PLAYER )
		{
			tSendSet.insert( tpEntity );
		}
	}

	CSceneLogic::GetSingletonPtr()->Send2Player( tSendSet, tpMessage );
	
}

// 得到自己看到的功能NPC ID
int CMapEntity::GetSeenNpcs( vector<int>& vNpcs )
{
	SeenEntityVector::iterator tBegin = mSeenEntitys.begin();
	SeenEntityVector::iterator tEnd = mSeenEntitys.end();

	for ( ; tBegin != tEnd; ++ tBegin )
	{
		CMapEntity* tpMapEntity = (CMapEntity*)CSceneObjManager::GetSingleton().GetObject(*tBegin);

		if (tpMapEntity == NULL)
			continue;

		CEntity* tpEntity = tpMapEntity->GetSelfEntity();

		if( tpEntity == NULL )
		{
			LOG_ERROR( "default", "MapEntity( %d ) 's Entity dosn't exist", *tBegin );
			continue;
		}

		if ( tpEntity->GetEntityType() ==  ENTITYTYPE_FUNCNPC )
		{
			vNpcs.push_back(tpEntity->GetEntityID());
		}

	}
}

