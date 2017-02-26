/******************************************************************************
	版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司
	
******************************************************************************
	文件名          : event.h
	版本号          : 1.0
	作者            : 
	生成日期        : 2009-06-24
	最近修改        : 
	功能描述        : 打算把游戏内的事件封装起来，结合观察者模式，逐个通知各个观察者，
						让游戏逻辑更加清晰，耦合度更低。 先实现几个接口，等待后期重构时
						大规模应用。
	函数列表        : 
	修改历史        : 
	1 日期          : 2009-06-24
	作者			: ZHL
	修改内容		: 生成
*******************************************************************************/

#pragma once

#include "servertool.h"
#include "coretype.h"
#include "lk_vector.h"


//enum EventType
//{
//	EVENT_DROPITEM    =  1,			// 掉落物品
//	EVENT_CREATEOGRE  =  2,			// 创建怪物
//	EVENT_PUSHINFB	  =  3,			// 传入副本
//};
//
//class CEvent;
//class CEventHandler;
//
//class CEvent
//{
//public:
//	int mEventID;
//
//	char mStrBuffer[ 1000 ];
//	int  mParam1;
//	int	 mParam2;
//	int  mParam3;
//	int  mParam4;
//
//public:
//	CEvent( )
//	{
//		mEventID = 0;
//		memset( mStrBuffer, 0, sizeof( mStrBuffer ) );
//		mParam1 = 0;
//		mParam2 = 0;
//		mParam3 = 0;
//		mParam4 = 0;
//	}
//	~CEvent( ){ }
//
//	CEvent( int vEventID, char *vStrBuffer, unsigned int vLen, int vParam1, int vParam2, int vParam3, int vParam4 ) : mEventID( vEventID ), 
//		mParam1( vParam1 ), mParam2( vParam2 ), mParam3( vParam3 ), mParam4( vParam4 )
//	{
//		memset( mStrBuffer, 0, sizeof( mStrBuffer ) );
//		memcpy( mStrBuffer, vStrBuffer, min( vLen, sizeof( mStrBuffer ) ) );
//	}
//
//	inline void Assign( int vEventID, char *vStrBuffer, unsigned int vLen, int vParam1, int vParam2, int vParam3, int vParam4 ) 
//	{
//		mEventID = vEventID;
//		mParam1 = vParam1;
//		mParam2 = vParam2;
//		mParam3 = vParam3;
//		mParam4 = vParam4;
//		memset( mStrBuffer, 0, sizeof( mStrBuffer ) );
//		memcpy( mStrBuffer, vStrBuffer, min( vLen, sizeof( mStrBuffer ) ) );
//	}
//};
//
//class CEventHandler
//{
//public:
//	static int OnEvent( CEvent vEvent );
//};
//



// -----------------------------------------------------------------------------

class CEvent;

// 事件观察者
class CEventObserver
{
	virtual int OnEventTrigger( CEvent* pEvent ) = 0;
};

// -----------------------------------------------------------------------------

enum emEventTypeForOB
{
	OB_EVENT_LEVELUP	= 1,
};


class CEvent
{
public:
	
	int RegisterObserver( CEventObserver* pObserver );
	int NotifyObservers( );

	inline int GetType( ) { return mEventType; }

private:
	typedef lk::vector<unsigned int, 30> _ObserverList;
	_ObserverList mObservers;
	int mEventType;
};


// 玩家升级事件
class CEventLevelUp : public CEvent
{
	unsigned int mEntityID;
	unsigned int mOldLevel; // 老等级
};

// 玩家跨地图
class CEventChangeMap : public CEvent
{

};