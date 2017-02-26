#ifndef _LOGHANDLE_HPP_
#define _LOGHANDLE_HPP_

#include "base_log.h"
#include "servertool.h"
#include "thread.h"
#include "tcp_conn.h"
#include "shm.h"
#include "proxymessage_pb.hxx.pb.h"
#include "message_pb.hxx.pb.h"
#include "code_queue.h"
#include "lk_vector.h"
#include "db_msg_pb.hxx.pb.h"
#include "logevent.h"
#include "logmonitor.h"

#define MAXEICONTAINERNUM			10		// 每个HANDLE最多可以缓存多少个不同种类的事件
#define HASHVALUE					512
#define VALUENEEDEDPERLEVEL			100
#define SAVEBRAINRECORDINTERVAL		1382400  //16天的秒数
#define SECONDS_OF_YEAR				365*24*60*60
#define INPUTQUEUELENGTH			0x1000000

#define MAXERRORMSGLEN				512                  //错误内存块最大长度
#define MAXMSGSTATNUM				203

class Database;

enum enLogOperMode
{
    LOGO_Cache = 1,			//cache 操作方式
    LOGO_DirectUpdate = 2,	//直接更新方式
};


struct STMsgProcessStatInfo
{
	int m_iMsgID;
	int m_iMsgTotal;
	timeval m_tvMsgProcTotal;
	timeval m_tvMaxMsgProc;
};

class CLogHandle : public CThread
{
public:
	CLogHandle();
	~CLogHandle();

	enum _enHandleParas
	{
		HANDLECOUNT = MAX_HANDLE_NUM,
		MULTIFACTOR = 1,
		RPTCHECKGAP = 3600,
		ROUTETABSIZE = 1024
	};
      	       														
	virtual int IsToBeBlocked();
	virtual int PrepareToRun();
	virtual int Run();
	virtual int ProcessThreadEnd();

	int Initialize( int iHandleID, CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH>* pGSvrdConns, const char* pMysqInfo );

	int PostOneCode( int nCodeLength, BYTE* pCode);

	int InitLogFile( const char* vLogName, const char* vLogDir, LogLevel vPriority  = LEVEL_NOTSET , unsigned int vMaxFileSize  = 10*1024*1024 , unsigned int vMaxBackupIndex  = 1 , bool vAppend  = true  );
     
    void* operator new(size_t nSize);
    void  operator delete( void* pMem);

    static int CountSize();
    static CSharedMem *ms_pCurrentShm;
    static pthread_mutex_t stLogConnMutex;
    static int m_iLogSvrID;
    static short m_sLogOperMode;


private:
	int OtherInit();
	int OtherResume();

	int GetOneCode(int& nCodeLength, BYTE* pCode);
	int ConvertCodeToMsg(int nCodeLength, BYTE * pCode, CProxyHead * pProxyHead, CMessage * pMsg);
	int ConvertMsgToCode(int & nCodeLength, BYTE * pCode, CProxyHead * pProxyHead, CMessage * pMsg);

	int SendMessageTo(CMessage *pMsg, short nDstEnTypt, short nDstEnID, unsigned int unUin = 0);
	int SendResponse(CMessage *pMsg);
	int Event(CMessage *pMsg);
	
	int ConnectToLocalLog();


private:
		
    int m_iHandleID;
    int m_iLLogInited;
	
	 /*! infoString should be formated like hostname;username;password;database. */
	char m_szHostInfoString[SQL_HOST_INFO_STRING];
    Database* m_pDatabase; // 本地mysql
	Database* m_pRemoteDatabase; // 远程存储mysql
	

	CProxyHead m_stCurrentProxyHead;	     //当前处理请求的Proxy头部

    CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH>* m_pProxySvrdConns;	//指向所有的proxy server 链接

    time_t m_tLastCheckTime;

    CCodeQueue* m_pInputQueue;    //输入消息队列

    int m_aiRouteTable[ROUTETABSIZE];

	std::string m_sSQL;

	std::map<int, CLogEvent*>  m_EventMap;
	std::map<int, CLogMonitor*> m_MonitorMap;

	int m_nMonitorTimer;

public:
	int OnMessageHandleTimeOut( );
	int ProcessExecuteSqlRequest( CMessage* pMsg );
	int ProcessNormalLogMessage( CMessage* pMsg );
	int SaveLogEventToDB(const char* stmtsql, CLogStat& rLogStat);
	int Catalog(int nCatalogType, int nActionType, 
			int nValueType, int nEntityType, int nDeltaValue);

	CLogStat* GetLogStat( int iMsgID );
};

#endif

