#include "stdafx.hpp"
#include "message_queue.h"
#include "code_queue.h"
#include "message_factory.h"

CSharedMem* CMessageQueue::mShmPtr = NULL;

template<> CMessageQueue* CSingleton< CMessageQueue >::spSingleton = NULL;

CMessageQueue::CMessageQueue( )
{
	CCodeQueue::pCurrentShm = mShmPtr;
	mMsgArea = new CCodeQueue( MSG_QUEUE_LEN );

	Initialize();

}

CMessageQueue::~CMessageQueue( )
{
	
}

void* CMessageQueue::operator new( size_t nSize )
{
	BYTE* pTemp;

	if( !mShmPtr )
	{
		return (void*)NULL;
	}

	pTemp = (BYTE *)mShmPtr->CreateSegment( nSize );

	return (void *)pTemp;
}

void  CMessageQueue::operator delete( void *pMem )
{
}

void* CMessageQueue::operator new(size_t size, const char* file, int line)
{
	return operator new ( size );
}
void CMessageQueue::operator delete(void* pointer, const char* file, int line)
{

}


int CMessageQueue::Initialize( )
{
	mMsgQueue.initailize ();
	mCurrentMsgNum = 0;
	return 0;
}

int CMessageQueue::Resume( )
{
	return 0;
}


unsigned int CMessageQueue::CountSize( )
{
	return sizeof( CMessageQueue ) + CCodeQueue::CountQueueSize( MSG_QUEUE_LEN );
}

int CMessageQueue::Push( CMessage* pMsg, void* pPara, unsigned short usParaLen )
{
	if( pMsg == NULL || pPara == NULL )
	{
		return -1;
	}

	pMsg->set_msgpara( usParaLen );

	mMsgQueue.push( *pMsg );
	// TODO; 这里用para临时来标示消息体长度

	if( pMsg->msgpara() != 0 )
	{
		int tRet = mMsgArea->AppendOneCode( (const BYTE*)(pPara), usParaLen );
		if ( tRet < 0 )
		{
			LOG_ERROR( "default", "In CMessageQueue::Push(), append one code failed err=%d.", tRet );
			return Err_System;
		}

		mCurrentMsgNum ++;
	}

	return 0 ;
}


int CMessageQueue::Pop( CMessage& rMsg )
{
	Message* tpInputMsg = (CMessage*) rMsg.msgpara();
	if( tpInputMsg != NULL )
	{
#ifndef _USE_MESSAGE_BUFF_
		delete tpInputMsg;
#else
		//TODO: 这里是不科学的，因为只析构掉了基类， 派生类没有析构掉
		// 但是，因为是静态空间，下次new就又覆盖掉了，所以不会出错。
		tpInputMsg->~Message();
#endif
		rMsg.set_msgpara( 0 );
	}

	if( mMsgQueue.empty() )
	{
		return Err_QueueEmpty;
	}

	rMsg = mMsgQueue.front();
	mMsgQueue.pop();


	BYTE tBuffer[ MAX_CODE_LEN ];
	int tBufferLen = sizeof( tBuffer );

	if( rMsg.msgpara() != 0 )
	{
		int tRet = mMsgArea->GetHeadCode( tBuffer, &tBufferLen );
		if ( tRet < 0 )
		{
			LOG_ERROR( "default", "In CMessageQueue::Pop(), GetHeadCode failed , err = %d", tRet );
			return Err_System;
		}
	}
	
	Message* tpMsgPara = CMessageFactory::GetSingletonPtr()->CreateMessage( rMsg.msghead().messageid());
	if( tpMsgPara == NULL )
	{
		return Err_System;
	}

	if( rMsg.msgpara() != 0 )
	{
		if( tpMsgPara->ParseFromArray(tBuffer, tBufferLen) != true )
		{
			LOG_ERROR( "default", "[%s:%d] decode error while parse msgpara, len %d", __FUNCTION__, __LINE__, tBufferLen );
			return Err_Decode;
		}
	}

	rMsg.set_msgpara ( (unsigned int)tpMsgPara );

	return Success;
}


void CMessageQueue::Dump(char* pBuffer, unsigned int& unLen )
{
	unsigned int unMaxLen = unLen;
	unLen = 0;

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "--------------------------------------------------------\n");

	//unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6s\t%6s\n", "name", "free", "total");

	unLen += snprintf( pBuffer+unLen,  unMaxLen-unLen, "%20s\t%6d\t%6d\n", "msgqueue", mMsgQueue.size(), mMsgQueue.max_size() );

	int nBegin = 0;
	int nEnd = 0;
	int nLeft = 0;

	mMsgArea->GetCriticalData( nBegin, nEnd, nLeft );

	unLen += snprintf( pBuffer+unLen,  unMaxLen-unLen, "%20s\t%6d\t%6d\n", "msgarea", nLeft, MSG_QUEUE_LEN );


}
