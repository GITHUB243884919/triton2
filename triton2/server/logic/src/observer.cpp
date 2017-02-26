#include "stdafx.hpp"
#include "observer.h"
#include "entity.h"


/************************************************************************/
/*	CEntityObserverable                                                               */
/************************************************************************/
CCharacterObservable::CCharacterObservable()
{
	if( CObj::msCreateMode )
	{
		for( unsigned int i = 0; i < ARRAY_CNT( mObservers ); i++ )
		{
			mObservers[ i ].initailize();
		}
	}
}

CCharacterObservable::~CCharacterObservable()
{

}

void CCharacterObservable::RegisterObserver( CCharacterObserver* observer, Observe_Type vType )
{
	LK_ASSERT( vType < ( int )ARRAY_CNT( mObservers ), return );

	bool vFinded = false;

	for( int i = mObservers[ vType ].size( ) - 1; i >= 0; i-- )
	{
		unsigned int vObjID = mObservers[ vType ][ i ];

		if( vObjID == ( unsigned int )observer->get_id( ) )
		{
			vFinded = true;
			break;
		}
	}
	if( vFinded == false )
	{
		mObservers[ vType ].push_back( observer->get_id() );
	}
}

void CCharacterObservable::CharacterDeathNotity( unsigned int vKilled, unsigned int vKiller )
{
	CEntityCharacter* tpKilled = ( CEntityCharacter* )CSceneObjManager::GetSingletonPtr( )->GetObject( vKilled );
	CEntityCharacter* tpKiller = ( CEntityCharacter* )CSceneObjManager::GetSingletonPtr( )->GetObject( vKiller );

	LK_ASSERT_STR( tpKilled != NULL, return, "EntityID %d", vKilled );
//	LK_ASSERT_STR( tpKiller != NULL, return, "EntityID %d", vKiller );
	
	for( int i = mObservers[ CHARACTER_DEATH ].size( ) - 1; i >= 0; i-- )
	{
		unsigned int vObjID = mObservers[ CHARACTER_DEATH ][ i ];
		CCharacterObserver* tpObserver = 
			( CCharacterObserver* )CSceneObjManager::GetSingletonPtr( )->GetObject( vObjID );

		LK_ASSERT_STR(tpObserver != NULL, continue, "ObjID = %d, Loop I = %d", vObjID, i );

		tpObserver->CharacterDeath( tpKilled, tpKiller );
	}
}

void CCharacterObservable::MachinerySpring( CEntityBox* tpMachinery, CEntity* tpSpring )
{
	LK_ASSERT( tpSpring != NULL, return );
	LK_ASSERT( tpMachinery != NULL, return );

	int i = 0;

	for( i = mObservers[ MACHINERY_SPRING ].size( ) - 1; i >= 0; i-- )
	{
		unsigned int vObjID = mObservers[ MACHINERY_SPRING ][ i ];
		CCharacterObserver* tpObserver = 
			( CCharacterObserver* )CSceneObjManager::GetSingletonPtr( )->GetObject( vObjID );

		LK_ASSERT_STR(tpObserver != NULL, continue, "ObjID = %d, Loop I = %d", vObjID, i );

		tpObserver->MachinerySpring( tpMachinery, tpSpring );
	}
}

void CCharacterObservable::LogoutObserver( CCharacterObserver* observer, Observe_Type vType  )
{
    LK_ASSERT( vType < ( int )ARRAY_CNT( mObservers ), return );

	for( int i = mObservers[ vType ].size( ) - 1; i >= 0; i-- )
	{
		unsigned int vObjID = mObservers[ vType ][ i ];

		if( vObjID == ( unsigned int )observer->get_id( ) )
		{
			mObservers[ vType ].erase( i );
		}
	}
}

/*
void CEntityObserverable::NotifyObservers( ) 
{
	for( _ObserverContainer::iterator iBegin = mObservers.begin();iBegin != mObservers.end(); ++iBegin )
	{
		((IObserver*)(&(*iBegin)))->Update( this );
	}
}
*/

void CCharacterObservable::TimeAssignment( CEntityCharacter* vpCharacter, int vTimerID, int vOtherInfoLength, 
										int* vpOtherInfo 
									)
{
	LK_ASSERT( vpCharacter != NULL, return );
	LK_ASSERT( vpOtherInfo != NULL, return );

	int i = 0;

	for( i = mObservers[ TIME_ASSIGNMENT ].size( ) - 1; i >= 0; i-- )
	{
		unsigned int vObjID = mObservers[ TIME_ASSIGNMENT ][ i ];
		CCharacterObserver* tpObserver = 
			( CCharacterObserver* )CSceneObjManager::GetSingletonPtr( )->GetObject( vObjID );

		LK_ASSERT_STR(tpObserver != NULL, continue, "ObjID = %d, Loop I = %d", vObjID, i );

		tpObserver->TimeAssignment( vpCharacter, vTimerID, vOtherInfoLength, vpOtherInfo );
	}

}

void CCharacterObservable::MoveStop( CEntity* vpEntity )
{
	LK_ASSERT( vpEntity != NULL, return );

	int i = 0;

	for( i = mObservers[ MOVE_STOP ].size( ) - 1; i >= 0; i-- )
	{
		unsigned int vObjID = mObservers[ MOVE_STOP ][ i ];
		CCharacterObserver* tpObserver = 
			( CCharacterObserver* )CSceneObjManager::GetSingletonPtr( )->GetObject( vObjID );

		LK_ASSERT_STR(tpObserver != NULL, continue, "ObjID = %d, Loop I = %d", vObjID, i );

		tpObserver->MoveStop( vpEntity );
	}

}
	

