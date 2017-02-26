#pragma once

//#define CRYPTKEYLENGTH		32
#define LOGOUT				(-100)
#define TNetHeadSize     sizeof(TNetHead)   /*TNetHead大小*/

typedef unsigned char BYTE;


class CEntityPlayer;

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

public:
	CNetHead( ) : mSrcIP(0), mSrcPort(0), mDstIP(0), mDstPort(0),  mStamp(0), mState(0) {}
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


//// 客户端连接信息
//class CClientInfo
//{
//public:
//	unsigned int			mUin;					//帐号ID
//	unsigned int 			mClientAddr;			//客户端的网络地址
//	unsigned short 			mClientPort;			//客户端的端口号
//	unsigned int 			mSockTime;				//客户端的网络地址
//	unsigned short 			mSockIndex;				//客户端的端口号
//	int 					mNetSpeed;				//玩家网速
//	short 					mVer;					//C-S协议版本号
//	unsigned char 			mKey[CRYPTKEYLENGTH];	//加密使用的Key
//	char 					mFlag;					//客户端状态：0 - 未连接，1 - 在线， 2 - 断线
//	time_t					m_tLastActiveTime;		// 最后活跃时间
//};


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
	CLIENTHANDLE_WRONG_SOCKSTATE,		// 玩家socket状态不正确
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

public:

	CClientHandle();
	~CClientHandle();


	void* operator new(size_t size);
	void  operator delete(void* pointer);

	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);

public:

	CCodeQueue* mC2SPipe;	// tcpserver	--> gameserver
	CCodeQueue* mS2CPipe;	// gameserver	--> tcpserver
	
	static CSharedMem* mShmPtr; // 共享内存指针
	

	int Initialize( );
	int Resume( );

	// 发送原始消息给特定客户端
	int Send(CMessage* pMsg, std::vector<CEntityPlayer*>* pPlayerSet);
	
	//************************************
	// Method:    Send
	// FullName:  CClientHandle::Send
	// Access:    public 
	// Returns:   int 0 成功； !0 失败
	// Qualifier: 发送序列化后的消息2进制给单个玩家 发送序列化后的消息2进制给单个玩家
	//				该函数主要是为了提升性能，免去组装vector的成本，序列化一次，发送多次。
	// Parameter: CEntityPlayer * pPlayer  玩家
	// Parameter: BYTE * pMessageBuff	序列化后的消息2进制
	// Parameter: int nLen				序列化后的2进制长度
	// Parameter: CMessage * pMsg		原始消息（主要是为了在Send内部记录日志，打印消息内容, 统计消息ID) 
	//************************************
	int Send(CEntityPlayer* pPlayer, BYTE* pMessageBuff, int nLen, CMessage* pMsg );
	
	int Recv();

	/**
	** 带消息队列版本
	*/
	//int DecodeNetMsg( BYTE* pCodeBuff, int& vLen, CCSHead* pCSHead, CMessage* pMsg, BYTE* pMsgPara, unsigned short& usParaLen);
	
	/**
	** 不带消息队列版本
	*/
	int DecodeNetMsg( BYTE* pCodeBuff, int& nLen, CCSHead* pCSHead, CMessage* pMsg );

	// 打印对象
	void Dump(char* pBuffer, unsigned int& unLen );

	static unsigned int CountSize();

private:
	enum { MAX_EMULATOR_MSG_BUF = 60000 };
	static char msMessageBuff[ MAX_EMULATOR_MSG_BUF ];
};

