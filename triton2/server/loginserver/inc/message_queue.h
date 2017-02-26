/******************************************************************************
	版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

 ******************************************************************************
	文件名		: message_queue.h
	版本号		: 1.0
	作者		: ZHL
	生成日期: 2008-08-10
	最近修改: 
	功能描述		: 消息队列，统一管理来自客户端和服务器的消息
	函数列表		: 
	修改历史		: 
		1 日期		: 2008-08-10
		作者		: ZHL
		修改内容	: 生成
*******************************************************************************/

#pragma once

#include "network.h"
#include "servertool.h"
#include "lk_queue.h"

class CCodeQueue;
class CSharedMem;



class CMessageQueue : public CSingleton<CMessageQueue>
{
public:

	//MsgQueue返回值
	enum EMsgQueueResult
	{
		Success			=  0, //成功
		Err_QueueFull	= -1, //队列满 
		Err_QueueEmpty	= -2, //队列空
		Err_UnKnown		= -5, //未知错误
		Err_System		= -6, //系统错误
		Err_Param		= -7, //参数错误
		Err_Decode		= -8, //解码错误
	};

	//MsgQueue容量
	enum 
	{
		MSG_AVG_SIZE	= 200,		// Message平均大小
		MSG_CAPABILITY	= 16000,	// Queue的Message容量
		MSG_QUEUE_LEN =  MSG_AVG_SIZE * MSG_CAPABILITY,
	};

public:
	CMessageQueue( );
	~CMessageQueue( );

	void* operator new( size_t nSize );
	void  operator delete( void *pMem );


	/**
	** 初始化
	*/
	int Initialize( );
	int Resume( );


	/**
	** 消息队列是否已满 
	*/
	bool IsQueueFull()	/*{ return mCurrentMsgNum == MSG_CAPABILITY; }*/
	{
		return mMsgQueue.size() == MSG_CAPABILITY ;
	}

	/**
	** 消息队列是否为空
	*/
	bool ISQueueEmpty() /*{ return mCurrentMsgNum == 0; }*/
	{
		return mMsgQueue.empty();
	}
	/**
	** 向消息队列中压入一个消息头和一段带长度的消息体 
	*/
	int Push( CMessage* vMessage );
	int Push( CMessage* pMsg, void* pPara, int usParaLen );

	/**
	** 从消息队列中弹出一个完整消息
	*/
	int Pop( BYTE* vBuff, int& vBuffLen );
	int Pop( CMessage& rMsg );

	static CSharedMem* mShmPtr;
	static unsigned int CountSize();
private:

	typedef lk::queue< CMessage, MSG_CAPABILITY > MsgQueue;

	MsgQueue mMsgQueue;
	CCodeQueue* mMsgArea;
	unsigned int mCurrentMsgNum;
};
