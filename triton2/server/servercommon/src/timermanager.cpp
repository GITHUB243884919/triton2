#include "timermanager.h"
#include "shm.h"
#include "message_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "message_dispatcher.h"
#include "scenelogic.h"

template<> CTimerMng* CSingleton< CTimerMng >::spSingleton = NULL;
CSharedMem* CTimerMng::mShmPtr = NULL;


int CTimerItem::Resume()
{
	return 0;
}

int CTimerItem::Initialize( )
{
	mOwnerID = 0;
	mTimerMark = 0;;
	mMillSeconds = 0;
	mSeconds = 0;
	mOtherInfoLength = 0;
	mCalledNum	= 1;
	mIsCleared = 0;
	memset( mOtherInfo, 0, sizeof(mOtherInfo) );
	mDurationSeconds = 0;
	return 0;
}

CTimerMng::CTimerMng()
{
	if( CObj::msCreateMode )
	{
		Initialize();
	}
}

CTimerMng::~CTimerMng()
{

}



void* CTimerMng::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CTimerMng::operator delete( void* pointer ) 
{ }

void* CTimerMng::operator new(size_t size, const char* file, int line)
{
	return operator new(size);
}
void CTimerMng::operator delete(void* pointer, const char* file, int line)
{

}

int CTimerMng::CountSize()
{
	return sizeof(CTimerMng);
}


int CTimerMng::Initialize( )
{
	mShortTimerQueue.initailize_pool();
	mMiddleTimerQueue.initailize_pool();
	mLongTimerQueue.initailize_pool();

	mDeleteList.initailize();
	mTimeoutList.initailize();
	return 0;
}


int CTimerMng::SetTimer(int nOwnerID, short nTimerMark, int nMillSec, unsigned char ucOtherInfoLen /*= 0*/, int* nOtherInfo /*= NULL*/, int nCalledTime /*= 1*/)
{
	CTimerItem* pTimer = NULL;
	if ( nMillSec < TIMER_DIVIDE_1 )
	{
		pTimer = (CTimerItem*) mShortTimerQueue.create();
	}
	else if ( nMillSec >= TIMER_DIVIDE_1 && nMillSec <= TIMER_DIVIDE_2 )
	{
		pTimer = (CTimerItem*) mMiddleTimerQueue.create();
	}
	else if ( nMillSec > TIMER_DIVIDE_2 )
	{
		pTimer = (CTimerItem*) mLongTimerQueue.create();
	}
	
	LK_ASSERT( pTimer != NULL, return INVALID_OBJ_ID );

	pTimer->mOwnerID = nOwnerID;
	pTimer->mTimerMark = nTimerMark;
	pTimer->mMillSeconds = nMillSec;
	pTimer->mOtherInfoLength = ucOtherInfoLen;

	if( ucOtherInfoLen > 0)
	{
		memcpy( pTimer->mOtherInfo, nOtherInfo, sizeof(int)*ucOtherInfoLen );
	}

	LOG_DEBUG( "pro", "timer id(%d), mark(%d) set at %u by %d", pTimer->get_id(), nTimerMark, SceneServer::CSceneServer::GetSingletonPtr()->mLastTickCount, nOwnerID );
	
	pTimer->mCalledNum = nCalledTime;
	pTimer->mDurationSeconds = nMillSec;

	return pTimer->get_id();
}



//************************************
// Method:    SetTimerBySec
// FullName:  CTimerMng::SetTimerBySec
// Access:    public 
// Returns:   int
// Qualifier: 以秒为单位设置长定时器，只有超过MAX_MILLSECONDS_OF_TIMER的才算长定时器
// Parameter: int nOwnerID
// Parameter: short nTimerMark
// Parameter: int nSec		单位： 秒
// Parameter: unsigned char ucOtherInfoLen
// Parameter: int * nOtherInfo
// Parameter: int nCalledTime
//************************************
int CTimerMng::SetTimerBySec( int nOwnerID, short nTimerMark, int nSec, unsigned char ucOtherInfoLen/* = 0*/, int* nOtherInfo /*= NULL*/, int nCalledTime/* = 1*/ )
{	
	//// 必须超过阀门才允许
	//if ( nSec < MAX_MILLSECONDS_OF_TIMER/1000 )
	//{
	//	return INVALID_OBJ_ID;	
	//}
	//
	CTimerItem* pTimer = (CTimerItem*) mLongTimerQueue.create();

	LK_ASSERT( pTimer != NULL, return INVALID_OBJ_ID );

	pTimer->mOwnerID = nOwnerID;
	pTimer->mTimerMark = nTimerMark;
	pTimer->mSeconds = nSec;
	pTimer->mMillSeconds = 0;
	pTimer->mOtherInfoLength = ucOtherInfoLen;

	if( ucOtherInfoLen > 0)
	{
		memcpy( pTimer->mOtherInfo, nOtherInfo, sizeof(int)*ucOtherInfoLen );
	}

	LOG_DEBUG( "pro", "timer id(%d), mark(%d) set at %u by %d", pTimer->get_id(), nTimerMark, SceneServer::CSceneServer::GetSingletonPtr()->mLastTickCount, nOwnerID );

	pTimer->mCalledNum = nCalledTime;
	pTimer->mDurationSeconds = nSec;

	return pTimer->get_id();	
}

//************************************
// Method:    ClearTimer
// FullName:  CTimerMng::ClearTimer
// Access:    public 
// Returns:   int
// Qualifier: 该函数比较特殊，为了防止在循环中边遍历边删除，除非强制指明马上删除，否则都是
//				加到一个删除列表里面，统一删除。
// Parameter: int nTimerID
// Parameter: bool bNow
//************************************
int CTimerMng::ClearTimer(int nTimerID, bool bNow /*= false*/)
{
	if( bNow )
	{
		switch( CObj::ID2TYPE( nTimerID ) )
		{
		case TIMER_OBJECT_SHORT_ID:
			{
				mShortTimerQueue.remove( nTimerID );
				break;
			}
		case TIMER_OBJECT_MIDDLE_ID:
			{
				mMiddleTimerQueue.remove( nTimerID );
				break;
			}
		case TIMER_OBJECT_LONG_ID:
			{
				mLongTimerQueue.remove( nTimerID );
				break;
			}
		default:
			break;
		}
	}
	else 
	{
		CTimerItem* pTimer = CTimerMng::GetSingletonPtr()->GetTimer( nTimerID );
		if( pTimer != NULL )
		{
			pTimer->mIsCleared = 1;
			mDeleteList.push_back( nTimerID );
		}
	}

	 return 0;
}


int CTimerMng::ForceTimeOut( int nTimerID )
{
	CTimerItem* pTimerItem = NULL;
	switch( CObj::ID2TYPE( nTimerID ) )
	{
	case TIMER_OBJECT_SHORT_ID:
		{
			pTimerItem = (CTimerItem*) mShortTimerQueue.find_by_id( nTimerID );
			break;
		}
	case TIMER_OBJECT_MIDDLE_ID:
		{
			pTimerItem = (CTimerItem*) mMiddleTimerQueue.find_by_id( nTimerID );
			break;
		}
	case TIMER_OBJECT_LONG_ID:
		{
			pTimerItem = (CTimerItem*) mLongTimerQueue.find_by_id( nTimerID );
			break;
		}
	default:
		break;
	}
	
	if( pTimerItem == NULL )
	{
		LOG_ERROR( "default", "[%s:%s:%d] can't find timerobject %d", __FILE__, __FUNCTION__, __LINE__, nTimerID );
		return -1;
	}

	return TimeOut( pTimerItem );
}

int CTimerMng::TimeOut(CTimerItem* pTimerItem)
{
	//LOG_DEBUG( "pro", "timer id(%d), mark(%d) timeout at %u", pTimerItem->get_id(), pTimerItem->mTimerMark, SceneServer::CSceneServer::GetSingletonPtr()->mLastTickCount );

	static CMessage sMessage;
	sMessage.mutable_msghead()->set_srcfe( FE_TIMER );
	sMessage.mutable_msghead()->set_messageid( ID_INTER_TIMEOUT );
	sMessage.set_msgpara( (unsigned int) pTimerItem );
	return CMessageDispatcher::GetSingletonPtr()->ProcessInternalMessage( &sMessage );
}



/** 
** 检查定时器队列，删除过期定时器
*/
int CTimerMng::CheckTimerQueue( unsigned int nTimeOffset )
{

	// 先清除待销毁列表里的对象，否则会有消息处理过程中已经删除的timer再次作用
	if( mDeleteList.size() > 0 )
	{
		DeleteList::iterator itBegin = mDeleteList.begin();
		DeleteList::iterator itEnd = mDeleteList.end();

		for( ; itBegin != itEnd; ++itBegin )
		{
			ClearTimer( *itBegin, true );
		}

		mDeleteList.clear();
	}

	CTimerItem* pTimerItem = NULL;
	for( ShortTimerObjMng::iterator it = mShortTimerQueue.begin(); it != mShortTimerQueue.end(); ++it )
	{
		pTimerItem = &(*it);
		CheckTimer( pTimerItem, nTimeOffset );
	}
	mCheckTimer1 += nTimeOffset;
	mCheckTimer2 += nTimeOffset;
	if ( mCheckTimer1 >= TIMER_CHECK_1 )
	{
		for( MiddleTimerObjMng::iterator it = mMiddleTimerQueue.begin(); it != mMiddleTimerQueue.end(); ++it )
		{
			pTimerItem = &(*it);
			CheckTimer( pTimerItem, mCheckTimer1 );
		}
		mCheckTimer1 = 0;
	}
	if ( mCheckTimer2 >= TIMER_CHECK_2 )
	{
		for( LongTimerObjMng::iterator it = mLongTimerQueue.begin(); it != mLongTimerQueue.end(); ++it )
		{
			pTimerItem = &(*it);
			CheckTimer( pTimerItem, mCheckTimer2 );
		}
		mCheckTimer2 = 0;
	}


	// 先清除待销毁列表里的对象，否则会有消息处理过程中已经删除的timer再次作用
	TimeoutList::iterator itBegin = mTimeoutList.begin();
	TimeoutList::iterator itEnd = mTimeoutList.end();
	
	CTimerItem* pTimeoutedTimer = NULL;
	for( ; itBegin != itEnd; ++itBegin )
	{
		pTimeoutedTimer = (CTimerItem*)(*itBegin);
		
		// 已经标识为清除的timer就不要再超时，走逻辑处理了。
		if( pTimeoutedTimer->mIsCleared == 1 )
			continue;

		TimeOut( pTimeoutedTimer );
	} 

	mTimeoutList.clear();

	return 0;
}

void CTimerMng::CheckTimer( CTimerItem* pTimerItem, unsigned int nTimeOffset )
{
	if ( pTimerItem == NULL )
	{
		return;
	}
	pTimerItem->mMillSeconds -= nTimeOffset;
		
	if( pTimerItem->mMillSeconds <= 0 )
	{	
		// 这个分支很少走到，超过几天的timer很少
		if( pTimerItem->mSeconds > 0 )
		{
			if( pTimerItem->mSeconds > MAX_MILLSECONDS_OF_TIMER/1000)
			{
				pTimerItem->mMillSeconds += MAX_MILLSECONDS_OF_TIMER;
				pTimerItem->mSeconds -= MAX_MILLSECONDS_OF_TIMER/1000;
			}
			else
			{
				pTimerItem->mMillSeconds += pTimerItem->mSeconds*1000;
				pTimerItem->mSeconds = 0;
			}
			
			return ;
		}

		mTimeoutList.push_back( (int)pTimerItem );

		if( -- pTimerItem->mCalledNum > 0 )
		{
			pTimerItem->mMillSeconds = pTimerItem->mDurationSeconds;
		}
	}
}


void CTimerMng::Dump(char* pBuffer, unsigned int& unLen )
{
	unsigned int unMaxLen = unLen;
	unLen = 0;

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "--------------------------------------------------------\n");

	//unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6s\t%6s\n", "name", "free", "total");

	unLen += snprintf( pBuffer+unLen,  unMaxLen-unLen, "%20s\t%6d\t%6d\n", "timer_short", mShortTimerQueue.size(), mShortTimerQueue.max_size() );
	unLen += snprintf( pBuffer+unLen,  unMaxLen-unLen, "%20s\t%6d\t%6d\n", "timer_middle", mMiddleTimerQueue.size(), mMiddleTimerQueue.max_size() );
	unLen += snprintf( pBuffer+unLen,  unMaxLen-unLen, "%20s\t%6d\t%6d\n", "timer_long", mLongTimerQueue.size(), mLongTimerQueue.max_size() );
}


CTimerItem* CTimerMng::GetTimer( int nTimerID )
{
	CTimerItem *pTimerItem = NULL;
	switch( CObj::ID2TYPE( nTimerID ) )
	{
	case TIMER_OBJECT_SHORT_ID:
		{
			pTimerItem = (CTimerItem*) mShortTimerQueue.find_by_id( nTimerID );
			break;
		}
	case TIMER_OBJECT_MIDDLE_ID:
		{
			pTimerItem = (CTimerItem*) mMiddleTimerQueue.find_by_id( nTimerID );
			break;
		}
	case TIMER_OBJECT_LONG_ID:
		{
			pTimerItem = (CTimerItem*) mLongTimerQueue.find_by_id( nTimerID );
			break;
		}
	default:
		break;
	}
	return pTimerItem;
}


int CTimerMng::GetLeftTime( int nTimerID, int& nSec, int& nMillSec )
{
	CTimerItem* pTimer = GetTimer( nTimerID );
	if( pTimer != NULL )
	{
		nSec = pTimer->mSeconds;
		nMillSec = pTimer->mMillSeconds;
		return 0;
	}
	else
	{
		return -1;
	}
}