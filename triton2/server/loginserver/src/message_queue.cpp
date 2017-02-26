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

//int CMessageQueue::Push( CMessage* vMessage )
//{
//	LK_ASSERT( vMessage != NULL, return Err_Param );
//
//	if ( mCurrentMsgNum >= MSG_CAPABILITY )
//	{
//		return Err_QueueFull;
//	}
//
//	int tRet = mMsgArea->AppendOneCode( (const BYTE*)(vMessage), vMessage->mMessageSize );
//	if ( tRet < 0 )
//	{
//		LOG_ERROR( "default", "In CMessageQueue::Push(), append one code failed err=%d.", tRet );
//		return Err_System;
//	}
//	
//	mCurrentMsgNum ++;
//
//
//	return 0;
//}


int CMessageQueue::Push( CMessage* pMsg, void* pPara, int usParaLen )
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


int CMessageQueue::Pop( BYTE* vBuff, int & vBuffLen )
{

	if ( mCurrentMsgNum <= 0 )
	{
		return Err_QueueEmpty;
	}

	BYTE tBuffer[ MAX_CODE_LEN ];
	int tBufferLen = sizeof( tBuffer );

	int tRet = mMsgArea->GetHeadCode( tBuffer, &tBufferLen );
	if ( tRet < 0 )
	{
		LOG_ERROR( "default", "In CMessageQueue::Pop(), GetHeadCode failed , err = %d", tRet );
		return Err_System;
	}
	
	mCurrentMsgNum --;

	// TODO: 临时这么做啊，等待面向流的消息体系
	memcpy( vBuff, tBuffer, tBufferLen );
	vBuffLen = tBufferLen;
	
	return Success;

	//CMessage tMessage;
	//if ( tMessage.Decode( tBuffer, tBufferLen ) < 0 )
	//{
	//	LOG_ERROR( "default" , "In CMessageQueue::Pop(), DecodeMessage faild, message id = %d", tMessage.mMessageID );
	//	return Err_Decode；
	//}
	//
	//if( vMessage != NULL )
	//{
	//	delete vMessage;
	//}

	//vMessage = CreateMessage( tMessage.mMessageID );
	//if ( vMessage == NULL )
	//{
	//	LOG_ERROR( "default", "In CMessageQueue::Pop(), Create Message faild, message id = %d", tMessage.mMessageID );
	//}

	return 0;
}


int CMessageQueue::Pop( CMessage& rMsg )
{
	Message* tpInputMsg = (CMessage*) rMsg.msgpara();
	if( tpInputMsg != NULL )
	{
		delete tpInputMsg;
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
			return Err_Decode;
		}
	}

	rMsg.set_msgpara ( (unsigned int)tpMsgPara );

	return Success;
}
