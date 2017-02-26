
#include "timerinterface.h"
#include "object.h"
//#include "timermanager.h"


//////////////////////////////////////////////////////////////////////////
// CPlayerTimerReceiver
//////////////////////////////////////////////////////////////////////////

CPlayerTimerReceiver::CPlayerTimerReceiver()
{
	if( CObj::msCreateMode )
		mTimerContainer.initailize();
}

CPlayerTimerReceiver::~CPlayerTimerReceiver()
{
}

int CPlayerTimerReceiver::SetTimer( int nOwnerID, short nTimerMark, int nMillSec, unsigned char ucOtherInfoLen /*= 0*/, int *nOtherInfo /*= NULL*/, unsigned int nCalledTime /*= 1*/)
{
	int nTimerID = CTimerMng::GetSingletonPtr()->SetTimer( nOwnerID, nTimerMark, nMillSec, ucOtherInfoLen, nOtherInfo, nCalledTime );
	if( nTimerID == INVALID_OBJ_ID )
	{
		LOG_ERROR( "pro", "player %d SetTimer faild", nOwnerID);
		return -1;
	}
	LOG_DEBUG("pro", "player %d created timer:%d",nOwnerID, nTimerID );
	mTimerContainer.push_back( nTimerID );
	return nTimerID;
}


int CPlayerTimerReceiver::SetTimerBySec( int nOwnerID, short nTimerMark, int nSec, unsigned char ucOtherInfoLen /*= 0*/, int* nOtherInfo /*= NULL*/, unsigned int nCalledTime /*= 1 */)
{
	int nTimerID = CTimerMng::GetSingletonPtr()->SetTimerBySec( nOwnerID, nTimerMark, nSec, ucOtherInfoLen, nOtherInfo, nCalledTime );
	if( nTimerID == INVALID_OBJ_ID )
	{
		LOG_ERROR( "pro", "player %d SetTimer faild", nOwnerID);
		return -1;
	}
	LOG_DEBUG("pro", "player %d created timer:%d",nOwnerID, nTimerID );
	mTimerContainer.push_back( nTimerID );
	return nTimerID;
}

//************************************
// Method:    ClearTimer
// FullName:  CPlayerTimerReceiver::ClearTimer
// Access:    public 
// Returns:   int
// Qualifier: 该函数比较特殊，为了防止在循环中边遍历边删除，除非强制指明马上删除，否则都是
//				加到一个删除列表里面，统一删除。
// Parameter: int nTimerID
// Parameter: bool bNow
//************************************
int CPlayerTimerReceiver::ClearTimer(int nTimerID, bool bNow /*= false*/ )
{
	for( TimerContainer::iterator it = mTimerContainer.begin(); it != mTimerContainer.end(); ++it )
	{
		if( *it == nTimerID )
		{
			mTimerContainer.erase( it );
			CTimerMng::GetSingletonPtr()->ClearTimer( nTimerID, bNow );
			return 0;
		}
	}
	LOG_DEBUG("pro", "timer %d will be clear", nTimerID );

	return -1;
}

int CPlayerTimerReceiver::ClearTimer( emTimerMark nTimerMark, unsigned char ucOtherInfoLen /*= 0*/, int* nOtherInfo /*= NULL*/, bool bNow /*= false*/ )
{
	for( TimerContainer::iterator it = mTimerContainer.begin(); it != mTimerContainer.end();  )
	{
		CTimerItem* pTimerItem = CTimerMng::GetSingletonPtr()->GetTimer( *it );
		LK_ASSERT( pTimerItem != NULL, return -1 );
	
		if( pTimerItem->mTimerMark == (short)(nTimerMark) )
		{
			if ( ucOtherInfoLen > 0 )
			{
				unsigned char i = 0;
				// 循环查找，一般不会走到循环里面去。
				for( i = 0; i < ucOtherInfoLen; i++ )
				{
					if( pTimerItem->mOtherInfo[ i ] != nOtherInfo[ i ] )
					{
						break;
					}
				}

				if( i < ucOtherInfoLen )
				{
					mTimerContainer.erase( it );
					CTimerMng::GetSingletonPtr()->ClearTimer( pTimerItem->get_id(), bNow );
					return 0;
				}
			}
			else
			{
				it = mTimerContainer.erase( it );
				CTimerMng::GetSingletonPtr()->ClearTimer( pTimerItem->get_id(), bNow );
				continue;
			}
		}

		++it;
	}

	return 0;
}


int CPlayerTimerReceiver::ClearAllTimer()
{
	for( TimerContainer::iterator it = mTimerContainer.begin(); it != mTimerContainer.end(); ++it )
	{
		CTimerMng::GetSingletonPtr()->ClearTimer( *it );
	}

	mTimerContainer.clear();

	return 0;
}
//
//int CPlayerTimerReceiver::TimeOutAllTimer()
//{
//	TimerContainer::iterator it = mTimerContainer.begin();
//	for( ; it != mTimerContainer.end(); )
//	{
//		CTimerMng::GetSingletonPtr()->ForceTimeOut( *it );
//		++it ;
//	}
//	
//	return 0;
//}


int CPlayerTimerReceiver::TimeOutOneTimer( int nTimerID )
{
	CTimerMng::GetSingletonPtr()->ForceTimeOut( nTimerID );
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// CNpcTimerReceiver
//////////////////////////////////////////////////////////////////////////

CNpcTimerReceiver::CNpcTimerReceiver()
{
	if ( CObj::msCreateMode )
	{
		mTimerContainer.initailize();
	}
}

CNpcTimerReceiver::~CNpcTimerReceiver()
{

}

int CNpcTimerReceiver::SetTimer(int nOwnerID, short nTimerMark, int nMillSec, unsigned char ucOtherInfoLen /*= 0*/, int *nOtherInfo /*= NULL*/, unsigned int nCalledTime /*= 1*/)
{
	int nTimerID = CTimerMng::GetSingletonPtr()->SetTimer( nOwnerID, nTimerMark, nMillSec, ucOtherInfoLen, nOtherInfo, nCalledTime );
	if( nTimerID == INVALID_OBJ_ID )
	{
		LOG_ERROR( "pro", "player %d SetTimer faild", nOwnerID );
		return -1;
	}

	mTimerContainer.push_back( nTimerID );
	return nTimerID;
}

int CNpcTimerReceiver::SetTimerBySec( int nOwnerID, short nTimerMark, int nSec, unsigned char ucOtherInfoLen /*= 0*/, int* nOtherInfo /*= NULL*/, unsigned int nCalledTime /*= 1 */)
{
	int nTimerID = CTimerMng::GetSingletonPtr()->SetTimerBySec( nOwnerID, nTimerMark, nSec, ucOtherInfoLen, nOtherInfo, nCalledTime );
	if( nTimerID == INVALID_OBJ_ID )
	{
		LOG_ERROR( "pro", "player %d SetTimer faild", nOwnerID );
		return -1;
	}

	mTimerContainer.push_back( nTimerID );
	return nTimerID;
}


//************************************
// Method:    ClearTimer
// FullName:  CNpcTimerReceiver::ClearTimer
// Access:    public 
// Returns:   int
// Qualifier: 该函数比较特殊，为了防止在循环中边遍历边删除，除非强制指明马上删除，否则都是
//				加到一个删除列表里面，统一删除。
// Parameter: int nTimerID
// Parameter: bool bNow
//************************************
int CNpcTimerReceiver::ClearTimer(int nTimerID, bool bNow /*= false*/ )
{
	for( TimerContainer::iterator it = mTimerContainer.begin(); it != mTimerContainer.end(); ++it )
	{
		if( *it == nTimerID )
		{
			mTimerContainer.erase( it );
			CTimerMng::GetSingletonPtr()->ClearTimer( nTimerID, bNow );
			return 0;
		}
	}

	return -1;
}

int CNpcTimerReceiver::ClearAllTimer()
{
	for( TimerContainer::iterator it = mTimerContainer.begin(); it != mTimerContainer.end(); ++it )
	{
		CTimerMng::GetSingletonPtr()->ClearTimer( *it );
	}

	mTimerContainer.clear();
	
	return 0;
}

//
//int CNpcTimerReceiver::TimeOutAllTimer()
//{
//	for( TimerContainer::iterator it = mTimerContainer.begin(); it != mTimerContainer.end(); ++it )
//	{
//		CTimerMng::GetSingletonPtr()->ForceTimeOut( *it );
//	}
//
//	return 0;
//}


int CNpcTimerReceiver::TimeOutOneTimer( int nTimerID ) 
{
	CTimerMng::GetSingletonPtr()->ForceTimeOut( nTimerID );

	return 0;
}


int CNpcTimerReceiver::ClearTimer( emTimerMark nTimerMark, unsigned char ucOtherInfoLen /*= 0*/, int* nOtherInfo /*= NULL*/, bool bNow /*= false*/ )
{
	for( TimerContainer::iterator it = mTimerContainer.begin(); it != mTimerContainer.end();  )
	{
		CTimerItem* pTimerItem = CTimerMng::GetSingletonPtr()->GetTimer( *it );
		LK_ASSERT( pTimerItem != NULL, return -1 );

		if( pTimerItem->mTimerMark == (short)(nTimerMark) )
		{
			if ( ucOtherInfoLen > 0 )
			{
				unsigned char i = 0;
				// 循环查找，一般不会走到循环里面去。
				for( i = 0; i < ucOtherInfoLen; i++ )
				{
					if( pTimerItem->mOtherInfo[ i ] != nOtherInfo[ i ] )
					{
						break;
					}
				}

				if( i == ucOtherInfoLen )
				{
					mTimerContainer.erase( it );
					CTimerMng::GetSingletonPtr()->ClearTimer( pTimerItem->get_id(), bNow );
					return 0;
				}
			}
			else
			{
				it = mTimerContainer.erase( it );
				CTimerMng::GetSingletonPtr()->ClearTimer( pTimerItem->get_id(), bNow );
				continue;
			}
		}

		++it;
	}

	return 0;
}