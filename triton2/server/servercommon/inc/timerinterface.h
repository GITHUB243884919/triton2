#ifndef __TIMER_INTERFACE__
#define __TIMER_INTERFACE__

#include "timermanager.h"
#include "timer_define.h"


#define TIMER_OTHER_INFO 4 // 定时器其他信息长度

class CMessageInternalTimeOut;


/************************************************************************/
/*  定时器虚接口                                                                     */
/************************************************************************/
class ITimerReceiver
{
public:
	
	// 设置定时器， 返回新的定时器id
	virtual int SetTimer( int nOwnerID, short nTimerMark, int nMillSec, unsigned char ucOtherInfoLen = 0, int* nOtherInfo = NULL, unsigned int nCalledTime = 1 ) = 0;

	// 设置定时器， 返回新的定时器id . (单位:秒, 推荐超长定时器>24天使用)
	virtual int SetTimerBySec( int nOwnerID, short nTimerMark, int nSec, unsigned char ucOtherInfoLen = 0, int* nOtherInfo = NULL, unsigned int nCalledTime = 1 ) = 0;
	
	// 清除指定id定时器（从本地容器删除定时器id,同时会从定时器管理器删除定时器对象), 注意延迟删除参数的作用。
	virtual int ClearTimer( int nTimerID, bool bNow = false ) = 0;

	// 清除指定类型定时器
	virtual int ClearTimer( emTimerMark nTimerMark, unsigned char ucOtherInfoLen = 0, int* nOtherInfo = NULL, bool bNow = false ) = 0;

	// 清除所有定时器（从本地容器删除定时器id,同时会从定时器管理器删除定时器对象）
	virtual int ClearAllTimer() = 0;

	// 强制超时所有定时器（从本地容器删除定时器id,同时会从定时器管理器删除定时器对象）
	//virtual int TimeOutAllTimer() = 0;

	// 强制超时一个指定定时器
	virtual int TimeOutOneTimer( int nTimerID ) = 0;
};

/************************************************************************/
/*  玩家定时器队列																		*/
/************************************************************************/
class CPlayerTimerReceiver : public ITimerReceiver
{
public:

	CPlayerTimerReceiver();
	virtual ~CPlayerTimerReceiver();

	virtual int SetTimer( int nOwnerID, short nTimerMark, int nMillSec, unsigned char ucOtherInfoLen = 0, int* nOtherInfo = NULL, unsigned int nCalledTime = 1) ;

	virtual int SetTimerBySec( int nOwnerID, short nTimerMark, int nSec, unsigned char ucOtherInfoLen = 0, int* nOtherInfo = NULL, unsigned int nCalledTime = 1 ) ;

	virtual int ClearTimer( int nTimerID, bool bNow = false );

	virtual int ClearTimer( emTimerMark nTimerMark, unsigned char ucOtherInfoLen = 0, int* nOtherInfo = NULL, bool bNow = false ) ;

	virtual int ClearAllTimer() ;

	//virtual int TimeOutAllTimer() ;

	virtual int TimeOutOneTimer( int nTimerID ) ;

private:
	enum { MAX_PLYER_TIMER_SETTED = 64 };
	typedef lk::vector< int, MAX_PLYER_TIMER_SETTED >	TimerContainer;
	
	// 玩家需要的定时器相对比较多
	TimerContainer mTimerContainer;
};

/************************************************************************/
/* NPC定时器队列                                                                      */
/************************************************************************/
class CNpcTimerReceiver : public ITimerReceiver
{
public:

	CNpcTimerReceiver();
	virtual ~CNpcTimerReceiver();

	virtual int SetTimer( int nOwnerID, short nTimerMark, int nMillSec, unsigned char ucOtherInfoLen = 0, int* nOtherInfo = NULL, unsigned int nCalledTime = 1) ;

	virtual int SetTimerBySec( int nOwnerID, short nTimerMark, int nSec, unsigned char ucOtherInfoLen = 0, int* nOtherInfo = NULL, unsigned int nCalledTime = 1 ) ;

	virtual int ClearTimer( int nTimerID, bool bNow = false );

	virtual int ClearTimer( emTimerMark nTimerMark, unsigned char ucOtherInfoLen = 0, int* nOtherInfo = NULL, bool bNow = false ) ;

	virtual int ClearAllTimer() ;

	//virtual int TimeOutAllTimer() ;

	virtual int TimeOutOneTimer( int nTimerID ) ;

private:
	enum { MAX_NPC_TIMER_SETTED = 16 };
	typedef lk::vector< int, MAX_NPC_TIMER_SETTED >	TimerContainer;

	// npc 需要的定时器相对比较少
	TimerContainer mTimerContainer;
};


#endif // __TIMER_INTERFACE__
