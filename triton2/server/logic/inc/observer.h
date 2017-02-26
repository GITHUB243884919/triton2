#pragma once

#include "errcode.h"

class CEntity;
class CEntityCharacter;
class CCharacterObserver;
class CEntityBox;

//class CCharacterObservable;

/************************************************************************/
/* 观察者                                                                     */
/************************************************************************/
//class IObserver : public CObj
//{
//public:
//	virtual void Update( IObservable* observerable ) = 0;
//};


/************************************************************************/
/* 被观察者                                                                    */
/************************************************************************/
//class IObservable
//{
//public:
//	virtual void RegisterObserver( IObserver* observer ) = 0;
//	virtual void NotifyObservers( ) = 0;
//};


/************************************************************************/
// 实体被观察者
// 考虑到静态容器占空间问题，所有观察者用一个容器来装，
// 具体事件让观察者来挑选了，暂时不按事件类型来区分观察者。                                                            */
/************************************************************************/
class CCharacterObservable //: public IObservable
{	
public:
	typedef lk::vector< unsigned int, 400 > _ObserverContainer;
	enum Observe_Type
	{
		CHARACTER_DEATH 	= 0,
		MACHINERY_SPRING,
		TIME_ASSIGNMENT,
		MOVE_STOP,
		CHARACTER_OBERVER_END,
	};
public:

	CCharacterObservable();
	virtual ~CCharacterObservable();

	virtual void RegisterObserver( CCharacterObserver* observer, Observe_Type vType  );
	virtual void LogoutObserver(  CCharacterObserver* observer, Observe_Type vType  );
//	virtual void NotifyObservers( ) ;

	void CharacterDeathNotity( unsigned int vKilled, unsigned int vKiller );
	void MachinerySpring( CEntityBox* tpMachinery, CEntity* vpSpring );
	void TimeAssignment( CEntityCharacter* vpCharacter, int vTimerID, int vOtherInfoLength, int* vpOtherInfo );
	void MoveStop( CEntity* vpEntity );
protected:
	
	_ObserverContainer	mObservers[ CHARACTER_OBERVER_END ];
};


class CCharacterObserver : public CObj
{
public:
	virtual int CharacterDeath( CEntityCharacter* vKilled, CEntityCharacter* vKiller ) { return SUCCESS; }
	virtual int MachinerySpring( CEntityBox* vpMachinery, CEntity* vpSpringed ) { return SUCCESS; }
	virtual int TimeAssignment( CEntityCharacter* vpCharacter, int vTimerID, int vOtherInfoLength, 
		int* vpOtherInfo )
	{ 
		return SUCCESS; 
	}
	virtual int MoveStop( CEntity* vpEntity ){ return SUCCESS; }
};


