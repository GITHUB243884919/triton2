#pragma once

#include "servertool.h"
#include "scenelogic.h"

class CMessage;


class CMessageDispatcher : public CSingleton< CMessageDispatcher >
{
public:

	CMessageDispatcher( );
	~CMessageDispatcher( );

	/**
	** 向消息队列添加一个消息
	*/
	int AppendMsg( CMessage* vMessage );

	int AppendMsg( CMessage* pMsg, void* pPara, unsigned short usParaLen );

	/**
	** 向外部实体发送消息
	*/
	int PostMessage(/*CMsgHead* pMsgHead, short shMsgParaLength, const BYTE* pMsgPara,BYTE opt = ST_CLIENT_ONLINE*/);

	/**
	** 从消息队列中取出消息进行派发 
	*/
	enum {MAX_MSG_DISPACH_ONCE = 3000};//单次派发的最大消息数 
	int DispachMsg(int iCount = MAX_MSG_DISPACH_ONCE);


	enum ENMDispacherResult
	{	
		Success				= 0,  //成功	
		Err_MsgQueueFull	= -1, //消息队列满 
		Err_PushMsg			= -2, //压入消息错误
		Err_Para			= -3, //参数错误
	};

public:

	/**
	** 处理请求消息
	*/
	int ProcessMessage(CMessage* pMsg);

public:

	int ProcessInternalMessage( CMessage* pMsg );

};



