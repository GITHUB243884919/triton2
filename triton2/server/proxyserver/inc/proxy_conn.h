#ifndef _PROXY_CONN_H_
#define _PROXY_CONN_H_
#include "tcp_conn.h"
#include "code_queue.h"
#include "servertool.h"
typedef CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH> CMyTCPSocket;

/*
enum eConnEntityTypes
{
	cet_gamesvr = 0,
	cet_dbsvr,
	cet_other,
	cet_proxy
};
*/

enum enConnFlags
{   
	FLG_CONN_IGNORE = 0,
	FLG_CONN_REDO   = 1,
	FLG_CONN_CTRL   = 2                                                                                           
};  

enum enEntityStates 
{
	ENTITY_OFF		= 0, 
	ENTITY_ON		= 1	
};

enum enBlockStates
{
	BLK_EMPTY    = 0,
	BLK_SAVING   = 1,
	BLK_REDOING  = 2
};  
    
class CMyTCPConn: public CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH>
{
	public:
		CMyTCPConn() {}
		~CMyTCPConn() {}

		enum enConnParas
		{
#ifdef _DEBUG_
			BLOCKQUEUESIZE = 0x2000,
#else
			BLOCKQUEUESIZE = 0x1000000,
#endif
		};

		int EstConn( int iAcceptFD );
		int IsConnCanRecv();

		int RegToCheckSet( fd_set *pCheckSet );
		int IsFDSetted( fd_set *pCheckSet );
		int RecvAllData();
		int GetOneCode( unsigned short &nCodeLength, BYTE *pCode  );
		int SendCode(unsigned short nCodeLength, BYTE *pCode, int iFlag = FLG_CONN_IGNORE);
		int CleanBlockQueue(int iQueueLength);
		int SetConnState(int iConnState);
		int GetConnState();

	private:
		int m_iConnState;

		CCodeQueue *m_pBlockQueue;
		CCodeQueue *m_pRedoQueue;
		int m_iCurrentDumpSeq;
		int m_iCurrentRedoSeq;

		int m_iBlockStatus;

#ifdef _POSIX_MT_
		pthread_mutex_t m_stMutex;          //操作的互斥变量
#endif
};

// socket连接的集合(主要用于管理所有的游戏代理服务器)
template < int MAX_CONN_NUM >
class CProxyConnSet
{
public:
	CProxyConnSet()
	{
		mActiveConnNum = 0;
	}
	~CProxyConnSet(){ }
public:
	CMyTCPConn *GetTcpConn(){ return mTcpConn; }	// 获取socket指针
	int	GetMaxConnNum(){ return MAX_CONN_NUM;  }	// 获取最大的socket数量

	int GetActiveConnNum(){ return mActiveConnNum; }	
	void SetActiveConnNum( int nNum ){ mActiveConnNum = nNum; }

	int Initialize( );		// 初始化函数	
	CMyTCPConn* GetFreeTcpConn();	// 获取一个空闲的socket
	int UnRegistTcpConnByEntityID( int nEntityID );	// 注销掉一个不用的socket
	int UnRegistTcpConnBySocketID( int nFd );	// 注销掉一个不用的socket
	CMyTCPConn* Accept( int nEntityType, int nFd, u_long nIp, u_short nPort );	// 建立一个新的socket连接
	CMyTCPConn* ServerRegiste( u_long nIp, u_short nPort, int nGameID );		// 注册一个服务器
	CMyTCPConn* GetTcpConnByIPAddr( u_long nIp, u_short nPort );	// 根据地址信息获取socket连接
	CMyTCPConn* GetTcpConnByEntityID( int nEntityID );		// 根据nEntityID获取socket连接
	CMyTCPConn& operator []( int nIndex );
private:
	int mActiveConnNum;								// 活跃的处于连接状态的socket数量
	CMyTCPConn mTcpConn[MAX_CONN_NUM];				// socket
};
#include "proxyconnset.cpp"
#endif