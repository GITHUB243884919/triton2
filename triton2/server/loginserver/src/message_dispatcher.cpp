#include "message_dispatcher.h"
#include "message_queue.h"
#include "loginlogic.h"
#include "log.h"
#include "lk_assert.h"
#include "message_factory.h"
#include <google/protobuf/descriptor.h>
#include "servermessage_in.h"

template<> CMessageDispatcher* CSingleton< CMessageDispatcher >::spSingleton = NULL;

CMessageDispatcher::CMessageDispatcher( )
{

}

CMessageDispatcher::~CMessageDispatcher( )
{

}

//int CMessageDispatcher::AppendMsg( CMessage* vMessage )
//{
//	if (  vMessage == NULL )
//	{
//		return Err_Para;
//	}
//	
//	if ( CMessageQueue::GetSingletonPtr( )->IsQueueFull( ) )
//	{
//		return Err_MsgQueueFull;
//	}
//
//	int iRet = CMessageQueue::GetSingletonPtr( )->Push( vMessage );
//
//	if ( iRet != CMessageQueue::Success )
//	{
//		return iRet;
//	}
//	
//	return Success;
//}


int CMessageDispatcher::AppendMsg( CMessage* pMsg, void* pPara, unsigned short usParaLen )
{
	if( pMsg == NULL || pPara == NULL )
	{
		return Err_Para;
	}

	if ( CMessageQueue::GetSingletonPtr( )->IsQueueFull( ) )
	{
		return Err_MsgQueueFull;
	}
	
	return CMessageQueue::GetSingletonPtr( )->Push( pMsg, pPara, usParaLen );

}

int CMessageDispatcher::DispachMsg(int iCount /* = MAX_MSG_DISPACH_ONCE */)
{

	BYTE tMsgBuff[ MAX_CODE_LEN ];
	short tMsgLen = sizeof( tMsgBuff );

	CMessage tMessage;

	int tDspCount = 0;
	int tPopRet = 0;

	//消息队列空的时候必定结束 
	do
	{
		tPopRet = CMessageQueue::GetSingletonPtr( )->Pop( tMessage );

		if( tPopRet != CMessageQueue::Success )
		{	
			//对非 队列空错误 打印以下日志
			if( tPopRet != CMessageQueue::Err_QueueEmpty )
			{
				LOG_ERROR( "default", "error %d occur while pop msg form queue.", tPopRet );
			}

			break;
		}



		ProcessMessage ( &tMessage );


		//单次派发限制, 跳出
		if( ++tDspCount > iCount)
		{
			LOG_ERROR( "default" , "msg dispached upto limit(%d).", iCount);
			break;
		}

	}while( tPopRet != CMessageQueue::Err_QueueEmpty );


	return 0;
}


int CMessageDispatcher::ProcessMessage( CMessage* vMsg )
{
	LK_ASSERT( vMsg != NULL , return -1 );
	Message* pUnknownMessagePara = (Message*) vMsg->msgpara();
	LK_ASSERT( pUnknownMessagePara != NULL, return -1 );

	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();

	LOG_DEBUG( "default", "message_head = [%s]", vMsg->msghead().ShortDebugString().c_str());
	
	switch( vMsg->msghead().srcfe() )
	{
		case FE_CLIENT:
		{
			LOG_DEBUG( "default", "---- Recv Client(%d) Msg[ %s ][id: 0x%04x / %d] ----", 
					vMsg->msghead().dstid(), pDescriptor->name().c_str(), vMsg->msghead().messageid(), vMsg->msghead().messageid() );
			LOG_DEBUG( "default", "[%s]", ((Message*)vMsg->msgpara())->ShortDebugString().c_str() );
			LOG_DEBUG( "default", "\n" );
			CLoginLogic::GetSingletonPtr()->OnClientMessage( vMsg );
			break;
		}

		default:
		{
			LOG_ERROR( "default", "In  CMessageDispatcher::ProcessMessage(), Invalid MsgSrc %d", vMsg->msghead().srcfe() );
			pbmsg_delmsg(vMsg);
			break;
		}
	}
	
	return 0;
}

