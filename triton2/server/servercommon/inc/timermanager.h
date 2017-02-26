#ifndef _TIMERMNG_H_
#define _TIMERMNG_H_

#include "object.h"
#include "servertool.h"
#include "objmanager.h"
#include "lk_vector.h"

class CSharedMem;

#define OBJ_ID_START(obj_type)	(((obj_type) << OBJ_ID_BITS) + 1)
#define OBJ_ID_END(obj_type)	((((obj_type) + 1) << OBJ_ID_BITS) - 10)
#define OBJ_ID_COUNT(obj_type, count) OBJ_ID_START(obj_type), count, OBJ_ID_END(obj_type)

#define MAX_TIMER_PARA_NUM 4
#define TIMER_OBJECT_SHORT_ID  0xFD	// timer对象type, 没有实际意义
#define TIMER_OBJECT_MIDDLE_ID 0xFE	// timer对象type, 没有实际意义
#define TIMER_OBJECT_LONG_ID   0xFF	// timer对象type, 没有实际意义
#define TIMER_DIVIDE_1	60000		// 短中Timer分界
#define TIMER_DIVIDE_2	1800000		// 中长Timer分界
#define TIMER_CHECK_1	1000		// 中Timer检查
#define TIMER_CHECK_2	5000		// 长Timer检查

#define MAX_MILLSECONDS_OF_TIMER  2073600000 // 定时器微秒数最大为24天 3600*1000*24*24

/**
** @class CTimerItem
**
** 定时器对象
**
*/
class CTimerItem : public CObj
{
public:

	enum emTimerType
	{
		SHORT_TIMER		= 0,	// 短定时器
		MIDDLET_TIMER	= 1,	// 中定时器
		LONG_TIMER		= 2,	// 长定时器
	};

	CTimerItem()
	{
		if( CObj::msCreateMode )
			Initialize();
		else
			Resume();
	}


	virtual ~CTimerItem(){}

	virtual int Resume();
	virtual int Initialize( );

	// 强制超时(定时器被从外部强行调用timeout)
	inline bool IsForceTimeOut( ) { return mMillSeconds >= 0 ; }

public:

	int				mOwnerID;		// 定时器拥有者实体ID
	int				mSeconds;		// 流逝时间, s
	int				mMillSeconds;	// 流逝时间，ms	
	short			mTimerMark;		// 定时器标志
	char			mIsCleared;		// 定时器状态, 是否已经被标识为清除，防止极端情况下被删除的timer再次作用( 0-否；1-是）
	unsigned char	mOtherInfoLength;
	int				mOtherInfo[MAX_TIMER_PARA_NUM];
	int				mCalledNum;		// 执行次数
	int				mDurationSeconds; // 定时器时常(不会变化，【注意】对于长定时器无效)
}; /* CTimerItem */


/**
** @class CTimerMng
**
** 定时器管理
**
*/
class CTimerMng : public CSingleton<CTimerMng>
{
public:

	CTimerMng();
	~CTimerMng();

	void* operator new( size_t nSize );
	void  operator delete( void *pMem );

	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);

	/**
	** 初始化： 在共享内存预创建多个timer对象
	*/
	int Initialize( );

	/** 
	** 检查定时器队列，删除过期定时器
	*/
	int CheckTimerQueue( unsigned int nTimeOffset );

	/**
	** 设置定时器
	*/
	int SetTimer(int nOwnerID, short nTimerMark, int nMillSec, unsigned char ucOtherInfoLen = 0, int* nOtherInfo = NULL, int nCalledTime = 1);
	
	//  注意：只有超过24天的计时器才允许使用该接口。 计时单位：秒
	int SetTimerBySec( int nOwnerID, short nTimerMark, int nSec, unsigned char ucOtherInfoLen = 0, int* nOtherInfo = NULL, int nCalledTime = 1 );

	/**
	** 清除定时器(某些情况下不能在循环中删除)
	*/
	int ClearTimer(int vTimerID, bool bNow = false );

	/**
	** 强制性超时
	*/
	int ForceTimeOut( int nTimerID );

	/**
	** 获取定时器剩余时长
	*/
	int GetLeftTime( int nTimerID, int& nSec, int& nMillSec );

	/**
	** 输出统计
	*/
	void Dump( char* pBuffer, unsigned int& unLen );

	/**
	** 得到定时器
	*/ 
	CTimerItem* GetTimer( int nTimerID );

	void CheckTimer( CTimerItem* pTimerItem, unsigned int nTimeOffset );

	static CSharedMem *mShmPtr;

	static int CountSize();

private:

	/**
	** 处理超时定时器
	*/
	int TimeOut(CTimerItem* pTimerItem);

private:
	enum
	{
		TIMER_PRECISION		= 200,		// 定时器精度 200 ms 
		TIMER_QUEUE_LENGTH	= 160000,	// 定时器最大个数
	};

	typedef ObjManager< CTimerItem,	OBJ_ID_COUNT( TIMER_OBJECT_SHORT_ID, TIMER_QUEUE_LENGTH	) >		 ShortTimerObjMng;
	typedef ObjManager< CTimerItem,	OBJ_ID_COUNT( TIMER_OBJECT_MIDDLE_ID, TIMER_QUEUE_LENGTH	) >	 MiddleTimerObjMng;
	typedef ObjManager< CTimerItem,	OBJ_ID_COUNT( TIMER_OBJECT_LONG_ID, TIMER_QUEUE_LENGTH	) >		 LongTimerObjMng;

	ShortTimerObjMng		mShortTimerQueue;
	MiddleTimerObjMng		mMiddleTimerQueue;
	LongTimerObjMng			mLongTimerQueue;
	unsigned int			mCheckTimer1;
	unsigned int			mCheckTimer2;

	typedef lk::vector< int, 5000 > DeleteList;
	typedef lk::vector< int, 8000 > TimeoutList;

	DeleteList		mDeleteList;
	TimeoutList		mTimeoutList;



}; /* CTimerMng */



#endif /* _TIMERMNG_H_ */

