#ifndef _DBCTRL_HPP_
#define _DBCTRL_HPP_

#include "dbhandle.h"
#include "base_db.h"

#ifdef _ASYNC_SQL_
#include "asqlexe.h"
#endif

#define MAXPROXYNUMBER				10					//最大game server 数目

#ifdef _DEBUG_
#define MAXHANDLENUMBER				2
#else
#define MAXHANDLENUMBER				8
#endif 


#define SECSOFONEHOUR				3600

#define CHECKINTERVAL				10					//检查时间间隔，单位：秒
#define PROXYKEEPALIVESECONDS		(3*CHECKINTERVAL)	//与proxy的心跳超时时间
#define MAXPROXYCODELEN				1024				//向proxy发送纯命令的最大块长度
#define PERF_TIMER	60		// 一分钟打印一次统计信息  以后走配置 单位：秒

typedef struct
{
    int   m_iServerID;
    TName m_szServiceIP;
    TName m_szServicePort;
} STGenSvrdCfg;


// 配置文件信息
struct STDBSvrdCfg
{
    int  m_iDBSvrID;
    char m_szServiceIP[32];
    char m_szServicePort[10];
    short m_sProxySvrdNum;
    short m_sDBOperMode;    // 1 cache 操作模式, 2 直接更新数据库
    short m_sCheckPointInterval;
    short m_sPrintStatInterval;  //打印服务器信息的时间间隔
    
    STGenSvrdCfg m_astProxySvrdCfg[MAXPROXYNUMBER];//proxy
	char m_szMysql[SQL_HOST_INFO_STRING];

	int mHandleLogLevel; // 线程日志等级
};


// 统计信息
struct STServerStatisticInfo
{
	int m_iCtrlLoopNum;							//主循环次数
	int m_iRecvMsgNum;							//收到消息包个数
	int m_iRecvPkgSize;							//接收消息包size
	int m_iSendMsgNum;							//下发消息包个数
	int m_iSendPkgSize;							//下发消息包size
	int m_iRecvMsgNumTransByKey;				//收到trans_bykey消息个数
	int m_iRecvMsgNumTransBroadCast;			//收到trans_broadcast消息个数

	int m_iInsertNum;							//insert操作次数
	int m_iUpdateNum;							//update操作次数
	int m_iSelectNum;							//select操作次数
	int m_iDeleteNum;							//delete操作次数

	int m_iSyncInsertNum;						//同步insert操作次数
	struct timeval m_tSyncInsertTime;			//同步insert执行时间
	int m_iSyncUpdateNum;						//同步update操作次数
	struct timeval m_tSyncUpdateTime;			//同步update执行时间
	int m_iSyncSelectNum;						//同步select操作次数
	struct timeval m_tSyncSelectTime;			//同步select执行时间
	int m_iSyncDeleteNum;						//同步delete操作次数
	struct timeval m_tSyncDeleteTime;			//同步delete执行时间

	int m_iDecodeProxyMsgFailedTimes;			//解码失败次数
	int m_iEncodeProxyMsgFailedTimes;			//编码失败次数
	int m_iGetCodeFailedTimes;					//从消息队列中获取消息失败次数
	int m_iPostCodeFailedTimes;					//提交消息到消息队列中失败次数

	int m_iExecSqlAsyncFailedNum;				//异步执行失败次数
	int m_iExecSqlAsyncSuccNum;					//异步执行成功次数
	struct timeval m_tExecSqlAsyncSuccTime;		//异步执行成功时间 
};

class CSharedMem;


class CDBCtrl : public CSingleton< CDBCtrl >
{
public:
    CDBCtrl();
    ~CDBCtrl();

    int Initialize( );
    int PrepareToRun();
    int Run();

    const STDBSvrdCfg GetDBSvrConf();

    void* operator new(size_t nSize);
    void  operator delete( void* pMem);

    static int CountSize();
    static CSharedMem *mShmPtr;

	
	// 运行标志
	enum ERunFlag
	{
		EFLG_CTRL_NO		= 0x01,
		EFLG_CTRL_QUIT		= 0x02,
		EFLG_CTRL_RELOAD	= 0x04
	};

	void SetRunFlag( int iFlag );
	void ClearRunFlag( int iFlag);
	bool IsRunFlagSet( int iFlag );

#ifdef _POSIX_MT_
    static pthread_mutex_t stMutex;
#endif

private:
    int ReadDBSvrdCfg(const char *szCfgFile);
	int ReadDBIpCfg(const char *szIpFile);

    int CreateServer();
    int	ConnectToProxyServer();
    int RegisterToProxyServer(CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> *pstTcpConn);
    int SendkeepAliveToProxy(CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> *pstTcpConn);     //向proxy发送心跳消息
    int CheckRunFlags();
    int CheckAndDispatchInputMsg();
    int RoutineCheck();
    int DispatchOneCode(int iProxyIdx, int nCodeLength, BYTE* pbyCode);
    int PostInternalMsgToHandle(int iHandleID, CMessage *pMsg);
    int NotifyHandleClearComplete();

    int ProcessOneCode(char *pacCode, int shCodeLength);
	int Event(CMessage *pstMsg);
    int PrintServerStatistic();

private:
	
	int m_iRunFlag;	// 运行标志

    STDBSvrdCfg m_stDBSvrdCfg;

    CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> m_astProxySvrdCon[MAXPROXYNUMBER];

    CDBHandle* m_apHandles[MAXHANDLENUMBER];
    short m_sCheckPointHandle;

    time_t m_tLastPrintTime;

    time_t m_atLastPorxyKeepAliveTime[MAXPROXYNUMBER];    //最近收到proxy心跳消息时间

#ifdef _ASYNC_SQL_
    CASqlExe* m_apExecers[MAXHANDLENUMBER];
#endif

};

#endif

