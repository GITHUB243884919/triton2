#pragma once

#include "lk_hashmap.h"
#include "entity.h"

//#define CRYPTKEYLENGTH		32
#define LOGOUT				(-100)
#define TNetHeadSize     sizeof(TNetHead)   /*TNetHead大小*/

typedef unsigned char BYTE;


// 网络头 （用于和tcpserver通讯）
class CNetHead
{
public:
	unsigned int	mSrcIP;	
	unsigned short	mSrcPort;
	unsigned int	mDstIP;
	unsigned short	mDstPort;
	time_t			mStamp;
	char			mState; // 状态， < 0 说明关闭了socket


	CNetHead( ) : mSrcIP(0), mSrcPort(0), mDstIP(0), mDstPort(0), mStamp(0), mState(0) {}
	~CNetHead() {}

};


// 管道标识符
enum enLockIdx
{
	IDX_PIPELOCK_C2S = 0,
	IDX_PIPELOCK_S2C = 1,
	IDX_PIPELOCK_A2C = 2,
};


// 客户端连接状态
enum enClientState
{
	CLIENT_NOTCONNECT = 0,
	CLIENT_ONLINE,
	CLIENT_OFFLINE
};

// 错误码
enum ClientHandleErrCode
{
	CLIENTHANDLE_SUCCESS = 0,
	CLIENTHANDLE_INVALID_PARA,			// 参数错误
	CLIENTHANDLE_QUEUE_FULL,			// 队列满
	CLIENTHANDLE_QUEUE_EMPTY,			// 队列空
	CLIENTHANDLE_CODE_NOTINTEGRITY,		// 数据不完整
	CLIENTHANDLE_DECODE_FAILED,			// 解码失败
	CLIENTHANDLE_ENCODE_FAILED,			// 编码失败
	CLIENTHANDLE_QUEUE_CRASH,			// 队列内部错误
	CLIENTHANDLE_EVIL_PKG,				// 非法网络包
	CLIENTHANDLE_DISCONNECT,			// 客户端断开链接
};

// 类前置声明
class CCSHead;
class CMessage;
class CCodeQueue;
class CSharedMem;

// 接入端连接器
class CClientHandle 
{
	//管道大小
	enum {MAX_PIPE_SIZE = 0x1000000};
	typedef void ( *OnMessageProc )( int vCommHandle, CMessage* pMessage );

public:

	CClientHandle();
	~CClientHandle();


	void* operator new(size_t size);
	void  operator delete(void* pointer);

public:

	CCodeQueue* mC2SPipe;	// tcpserver	--> gameserver
	CCodeQueue* mS2CPipe;	// gameserver	--> tcpserver
	
	static CSharedMem* mShmPtr; // 共享内存指针
	

	int Initialize( );
	int Resume( );

	int Send(CMessage* pMsg, CGatePlayer* pPlayer);
	int Send(CMessage* pMsg, std::vector<CGatePlayer*>* pPlayerSet );
	int Recv();


	int DecodeNetMsg( BYTE* pCodeBuff, int& vLen, CCSHead* pCSHead, 
			CMessage* pMsg, BYTE* pMsgPara, int& usParaLen, CNetHead& tNetHead);

	int EncodeNetMsg( CMessage* pMsg, BYTE* pCodeBuff, int& vLen, CGatePlayer* pPlayer);

	static unsigned int CountSize();
public:

};

