#ifndef _DBHANDLE_HPP_
#define _DBHANDLE_HPP_

#include "base_db.h"
#include "servertool.h"
#include "thread.h"
#include "tcp_conn.h"
#include "asqlexe.h"
#include "shm.h"
#include "proxymessage_pb.hxx.pb.h"
#include "message_pb.hxx.pb.h"
#include "code_queue.h"
#include "performance.h"
#include "db_msg_pb.hxx.pb.h"
#include "log_msg_pb.hxx.pb.h"
#include "lk_hashmap.h"



#define HASHVALUE					512
#define VALUENEEDEDPERLEVEL			100
#define SAVEBRAINRECORDINTERVAL		1382400  //16天的秒数
#define SECONDS_OF_YEAR				365*24*60*60
#define INPUTQUEUELENGTH			0x1000000

#define MAXERRORMSGLEN				512                  //错误内存块最大长度
#define MAXMSGSTATNUM				203

// 统计的SQL语句前x字节 截断
#define PERF_SQL_LEN				39

#ifdef _DEBUG_

#define PERF_BEGIN_DB( funcname )											\
	CPerfInfo& funcname##_info = GetPerfInfo( funcname );					\
	Clock funcname##_a;														\
	if( funcname##_info.mPerfLog == true )									\
		{																	\
		funcname##_a.start( );												\
		}

#define PERF_END_DB( funcname )												\
	if( funcname##_info.mPerfLog == true)									\
		{																	\
		funcname##_info.AddCalled( funcname##_a.stop( ) );					\
		}

#define PERF_DB( funcname, func )										\
	do																	\
	{																	\
		CPerfInfo& info = GetPerfInfo( funcname );						\
		if( info.mPerfLog == true )										\
			{															\
				Clock a;												\
				a.start( );												\
				func;													\
				info.AddCalled( a.stop( ) );							\
			}															\
			else														\
			{															\
				func;													\
			}															\
	}while(0);

#else

#define PERF_BEGIN_DB( funcname )  funcname	;
#define PERF_END_DB( funcname )	   funcname ;
#define PERF_DB( funcname, func )	 func ;

#endif 

enum enDBOperMode
{
    DBO_Cache = 1,			//cache 操作方式
    DBO_DirectUpdate = 2,	//直接更新方式
};


struct STMsgProcessStatInfo
{
	int m_iMsgID;
	int m_iMsgTotal;
	timeval m_tvMsgProcTotal;
	timeval m_tvMaxMsgProc;
};


// 根据唯一id得到道具表编号
#define UIN_TO_ITEMTABLEID( uin )  (uin%16)


struct CExchangeAccountLog
{
	bool mIsNeedSend;
	CMessageExchangeLog mLog;
	
	CExchangeAccountLog( )
	{
		Clear( );
	}	

	void Clear( )
	{
		mIsNeedSend = false;
		mLog.Clear( );
	}
};

class CDBHandle : public CThread
{
private:
	typedef bool ( CDBHandle::*TRAN_EVENT )( int* vTranCode, int* vPrarme1, int* vParame2 );
public:
	CDBHandle();
	~CDBHandle();

	CExchangeAccountLog mAccountLog[ 2 ];

	int SetExchangeAccountLog( unsigned int vIndex, int vAccount, int vRole, int vOperate, int* vParame = NULL, 
		int64_t vDetailID1 = 0, int64_t vDetailID2 = 0 );
	int ExchangeAccountLogSend( const CMessageHead* vpHead );
	int ExchangeAccountLogUnSend( );

	enum _enHandleParas
	{
		//HANDLECOUNT = 8,
		MULTIFACTOR = 1,
		RPTCHECKGAP = 3600,
		ROUTETABSIZE = 1024
	};
      	       														
	virtual int IsToBeBlocked();
	virtual int PrepareToRun();
	virtual int Run();
	virtual int ProcessThreadEnd();

#ifdef _ASYNC_SQL_
	int Initialize( int iHandleID, CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH>* pProxySvrdConns,  int iPipeFD = -1 );
	int ExecSqlAsync( const char *szSql );
#else
	int Initialize( int iHandleID, CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH>* pGSvrdConns);
#endif

	int PostOneCode( int nCodeLength, BYTE* pCode);

	int InitLogFile( const char* vLogName, const char* vLogDir, LogLevel vPriority  = LEVEL_NOTSET , unsigned int vMaxFileSize  = 10*1024*1024 , unsigned int vMaxBackupIndex  = 1 , bool vAppend  = true  );
     
    void* operator new(size_t nSize);
    void  operator delete( void* pMem);

    static int CountSize();
    static CSharedMem *ms_pCurrentShm;
    static pthread_mutex_t stDBConnMutex;
    static int m_iDBSvrID;
    static short m_sDBOperMode;

	int		m_nSavePerfLogFlag;// 写统计日志的标记
	inline CPerfInfo& GetPerfInfo(const char* name)
	{
		unsigned int hashCode = CPerfStat::DJBHash(name);
		CPerfIndex perfIndex(hashCode, name);
		return msPerfMap[perfIndex];
	}
private:
	PerfMap msPerfMap;	// 本线程用于统计的记录统计信息的容器
	int OtherInit();
	int OtherResume();

	int GetOneCode(int& nCodeLength, BYTE* pCode);
	int ConvertCodeToMsg(int nCodeLength, BYTE * pCode, CProxyHead * pProxyHead, CMessage * pMsg);
	int ConvertMsgToCode(int & nCodeLength, BYTE * pCode, CProxyHead * pProxyHead, CMessage * pMsg);

	int GetBourseInfo( int vSessionID, const CMessageHead* vpHead );
//	int GetBourse
	int GetBourseAccount( int vSessionID, int vAccount, int vCode, const CMessageHead* vpHead, int vResultCode, 
			int vParame1, int vParame2 );
	int CreateBourseAccount( int vAccount );

	int SendMessageTo(CMessage *pMsg, short nDstEnTypt, short nDstEnID, unsigned int unUin = 0);
	int SendResponse(CMessage *pMsg);
	int Event(CMessage *pMsg);

	int GetBoureseAccountDB( int vAccount, int* vpMoney, int* vpYB );
	int BourseQueryInsertDetail( PBBourseDetailList* tpList, const char* tpSql );
	int BoursePlayerListGet( int vAccountID, const CMessageHead* vpHead );

	bool BourseDocHistory( int vDoc, const char* vTable );

//	int GetSequestRequest( CMessage* pMsg );	
	int OnMessageGetBourseInfo( CMessage* pMsg );
	int OnMessageGetBourseAccount( CMessage* pMsg );
	int OnMessageBourseIncAccount( CMessage* pMsg );
	int OnMessageBourseDecAccount( CMessage* pMsg );
	int ProcessExecuteSqlRequest( CMessage* pMsg );
	int ProcessLoadPlayerItemInfoRequest( CMessage* pMsg );
	int ProcessSavePlayerItemInfoRequest( CMessage* pMsg );
	int ProcessSavePlayerRequest( CMessage* pMsg );
	int ProcessLoadPlayerInfo( CMessage* pMsg );
	int ProcessLoadPlayerProperty( CMessage* pMsg );
	int ProcessBourseAddDoc( CMessage* pMsg );
	int ProcessBoursePlayerListGet( CMessage* pMsg );
	int ConnectToLocalDB();
	int UpdatePlayerInfoRequest( CMessage* pMsg );
	int ProcessBourseTrade( CMessage* pMsg );
	int ProcessBourseDecBuyDoc( CMessage* pMsg );
	int ProcessBourseDecSellDoc( CMessage* pMsg );
	int ProcessBourseLogQuery( CMessage* pMsg );

	
	int TranDoingEvent( TRAN_EVENT vEvent, int* vTranCode, int* vParame1, int* vParme2 );
	//inc account tran event
	bool IncAccountTran( int* vTranCode, int* vParame1, int* vParame2 );
	bool DecAccountTran( int* vTranCode, int* vParame1, int* vParame2 );
	bool AddBuyDoc( int* vTranCode, int* vParame1, int* vParame2 );
	bool AddSellDoc( int* vTranCode, int* vParame1, int* vParame2 );
	bool BourseTradeTran( int* vTranCode, int* vPrice, int* vParame2 );
	bool DecBuyDoc( int* vTranCode, int* vParame1, int* vParame2 );
	bool DecSellDoc( int* vTranCode, int* vParame1, int* vParame2 );

private:
		
    int m_iHandleID;
    int m_iLDBInited;
	
	 /*! infoString should be formated like hostname;username;password;database. */
	char m_szHostInfoString[SQL_HOST_INFO_STRING];
    Database* m_pDatabase;

	CProxyHead m_stCurrentProxyHead;	     //当前处理请求的Proxy头部

    CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH>* m_pProxySvrdConns;	//指向所有的proxy server 链接

    time_t m_tLastCheckTime;

#ifdef _ASYNC_SQL_
    int m_iPipeFD;
    CSockStream m_sstmPipe;
#endif

    CCodeQueue* m_pInputQueue;    //输入消息队列

    int m_aiRouteTable[ROUTETABSIZE];

	typedef lk::hash_map<lk::CString32, int, 64> CField2Table;
	CField2Table mField2Table;

};


#endif

