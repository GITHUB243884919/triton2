#include "stdafx.hpp"
#include "message_dispatcher.h"
#include "message_queue.h"
#include "scenelogic.h"
#include "message_factory.h"
#include "coremessage_pb.hxx.pb.h"
#include "promessage_pb.hxx.pb.h"
#include "mapmessage_pb.hxx.pb.h"

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

	switch( vMsg->msghead().srcfe() )
	{
	case FE_DATASERVER:
		{
#ifdef _DEBUG_
			LOG_NOTICE( "default", "---- Recv Data Msg[ %s ][id: 0x%04x / %d] ----", pDescriptor->name().c_str(), vMsg->msghead().messageid(), vMsg->msghead().messageid());
			LOG_DEBUG( "default", "[%s]", ((Message*)vMsg->msgpara())->ShortDebugString().c_str() );
#endif
			PERF_FUNC( pDescriptor->name().c_str(), SceneServer::CSceneServer::GetSingletonPtr( )->ProcessDataMsg( vMsg ) );

			break;
		}
	case FE_DUNGEONSERVER:
		{
			CSceneServer::GetSingleton().ProcessDunserverMsg( vMsg );	
			break;
		}
	case FE_PROXYSERVER:
	case FE_GATESERVER:
	case FE_LOGINSERVER:
	case FE_SCENCESERVER:
		{
#ifdef _DEBUG_
			LOG_NOTICE( "default", "---- Recv Router Msg[ %s ][id: 0x%04x / %d] ----", pDescriptor->name().c_str(), vMsg->msghead().messageid(), vMsg->msghead().messageid() );
			LOG_DEBUG( "default", "[%s]", ((Message*)vMsg->msgpara())->ShortDebugString().c_str() );
#endif
			PERF_FUNC( pDescriptor->name().c_str(), SceneServer::CSceneServer::GetSingletonPtr( )->ProcessRouterMsg( vMsg ) );

			break;
		}
	case FE_CLIENT:
		{
#ifdef _DEBUG_
			// 屏蔽一些消息，不打印
			if( vMsg->msghead().messageid() != ID_C2S_REQUEST_PING 
				&& vMsg->msghead().messageid() != ID_C2S_REQUEST_PLAYERPATH 
				&& vMsg->msghead().messageid() != ID_C2S_REQUEST_PETPATH 
				&& vMsg->msghead().messageid() != ID_C2S_NOTIFY_DIRECTION )
			{
				LOG_NOTICE( "default", "---- Recv Client(%d) Msg[ %s ][id: 0x%04x / %d] ----", vMsg->msghead().dstid(), pDescriptor->name().c_str(), vMsg->msghead().messageid(), vMsg->msghead().messageid() );
				LOG_DEBUG( "default", "[%s]", ((Message*)vMsg->msgpara())->ShortDebugString().c_str() );
			}
#endif
			int nProcessMsgCostTime = 0;
			PERF_FUNC_ELAPSED( pDescriptor->name().c_str(), SceneServer::CSceneServer::GetSingletonPtr( )->ProcessClientMsg( vMsg ), nProcessMsgCostTime );
			// 超过一秒就打印
			if( nProcessMsgCostTime > 500000 )
			{
				LOG_FATAL( "default", "### Client(%d)'s Msg Cost Too Much Time %d ###", vMsg->msghead().dstid(), nProcessMsgCostTime );
				LOG_FATAL( "default", "[%s]", ((Message*)vMsg->msgpara())->ShortDebugString().c_str() );
			}

			break;
		}
	default:
		{
			LOG_ERROR( "default", "In  CMessageDispatcher::ProcessMessage(), Invalid MsgSrc %d", vMsg->msghead().srcfe() );
			break;
		}
	}

	Message* pMsgPara = (Message*) vMsg->msgpara();

	// 内存回收
	if( pMsgPara != NULL )
	{
#ifdef _USE_MESSAGE_BUFF_ // 如果使用了placement new, 则只析消息即可
		pMsgPara->~Message();
#else
		delete pMsgPara;
#endif
		vMsg->set_msgpara( (uint32)(NULL) );
	}

	return 0;
}

int CMessageDispatcher::ProcessInternalMessage( CMessage* pMsg )
{
	switch( pMsg->msghead().srcfe() )
	{
	case FE_TIMER:
		{
			PERF_FUNC( "ProcessInternalMessage", SceneServer::CSceneServer::GetSingletonPtr( )->ProcessTimeOutMsg( pMsg ) );
			break;
		}
	default:
		{
			//LOG_ERROR( "default", "In  CMessageDispatcher::ProcessMessage(), Invalid MsgSrc %d", vMsg->msghead().srcfe() );
			break;
		}
	}

	return 0;
}
