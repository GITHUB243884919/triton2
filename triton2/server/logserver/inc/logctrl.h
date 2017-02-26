#ifndef _LOGCTRL_HPP_
#define _LOGCTRL_HPP_

#include "loghandle.h"
#include "base_log.h"
#include "lk_string.h"
#include "lk_hashmap.h"
#include "log_msg_pb.hxx.pb.h"
#include "template.h"

#define MAXPROXYNUMBER				10					//最大game server 数目
#define MAXHANDLENUMBER				(CLogHandle::HANDLECOUNT)
#define SECSOFONEHOUR				3600

#define CHECKINTERVAL				10					//检查时间间隔，单位：秒
#define PROXYKEEPALIVESECONDS		(3*CHECKINTERVAL)	//与proxy的心跳超时时间
#define MAXPROXYCODELEN				1024				//向proxy发送纯命令的最大块长度

typedef struct
{
    int   m_iServerID;
    TName m_szServiceIP;
    TName m_szServicePort;
} STGenSvrdCfg;


// 配置文件信息
struct STLogSvrdCfg
{
    int  m_iLogSvrID;
    char m_szServiceIP[32];
    char m_szServicePort[10];
    short m_sProxySvrdNum;
    short m_sLogOperMode;    // 1 cache 操作模式, 2 直接更新数据库
    short m_sCheckPointInterval;
    short m_sPrintStatInterval;  //打印服务器信息的时间间隔
    
    STGenSvrdCfg m_astProxySvrdCfg[MAXPROXYNUMBER];//proxy
	char m_szMysql[SQL_HOST_INFO_STRING];	// 本地mysql配置信息
	char m_szRemoteStorage[SQL_HOST_INFO_STRING]; // 远程存储mysql配置信息

	int mHandleLogLevel; // 线程日志等级
	int mMonitorTimer;	 // 监控输出周期
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


class CLogCtrl : public CSingleton< CLogCtrl >
{
public:
    CLogCtrl();
    ~CLogCtrl();

    int Initialize( );
    int PrepareToRun();
    int Run();

    const STLogSvrdCfg GetLogSvrConf();

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
    int ReadLogSvrdCfg(const char *szCfgFile);
	int ReadLogIpCfg(const char *szIpFile);

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
	int PrintStatistic();
	void LoadToken2String();
	void LoadCatalogDetail();

	int CreateLogDB( time_t time );
private:
	
	int m_iRunFlag;	// 运行标志

    STLogSvrdCfg m_stLogSvrdCfg;

    CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> m_astProxySvrdCon[MAXPROXYNUMBER];

    CLogHandle* m_apHandles[MAXHANDLENUMBER];
    short m_sCheckPointHandle;

    time_t m_tLastPrintTime;
	// 统计信息
	enum
	{
		BASE_LOGMSG_ID = ID_S2L_LOGMESSAGE,
		MAX_LOGMSG_TYPE = 32,
	};
	CLogStat m_tLogStat[MAX_LOGMSG_TYPE];

    time_t m_atLastPorxyKeepAliveTime[MAXPROXYNUMBER];		//最近收到proxy心跳消息时间

	// TODO: 暂时统一记录时间,以后由配置决定每个事件的记录时间
	time_t m_tLastSaveDataTime;								// 最后一次记录handle上事件数据的事件

	typedef lk::string<256> CString256;
	typedef std::map<lk::CString32, CString256 > CTOKEN2STRING;

	CTOKEN2STRING mToken2String;
	//typedef std::map <int, std::pair<int, lk::CString32> > ID2TABLE;
	typedef struct 
	{
		int first;		// number
		lk::CString32 second;	//table name
		int handle;
	}IDINFO;

	//typedef std::map <int, std::pair<int, lk::CString32> > ID2TABLE;
	typedef lk::hash_map <int, IDINFO, 100> ID2TABLE;

	ID2TABLE m_Id2Table;

public:
	// 统计需要的ID,以及各种类型
	typedef struct
	{
		unsigned int id;	// 科目ID
		char table_id; 		// 写入的表ID
		int  value_type;	// 值类型, 对于物品是物品ID
		char action_type;	// 操作类型	
		char entity_type;	// 对象类型	
	}CATALOG_INFO;

	typedef  lk::hash_map<unsigned int, int, 5000> ACTION2CATALOG;
	typedef  lk::hash_map<unsigned int, CATALOG_INFO, 2000> CATALOGDICT;
	typedef  lk::hash_map<int, int , 256> ITEMACTIONGROUP;

	int OnTime( );

	bool EvtTimeOut( time_t tCurTime );

	const char* GetTableName(int id)
	{
		ID2TABLE::const_iterator iter = m_Id2Table.find(id);
		return iter != m_Id2Table.end() ? (iter->second).second.c_str() : NULL;
	}

	const char* Token2String(const char* szToken)
	{
		if ( szToken[0] != '#') return szToken;

		CTOKEN2STRING::iterator iter = mToken2String.find(lk::CString32(&szToken[1]));
		return (iter == mToken2String.end() ? szToken : iter->second.c_str());
	}

	int  GetTableCount( int id)
	{
		ID2TABLE::const_iterator iter = m_Id2Table.find(id);
		return iter != m_Id2Table.end() ? (iter->second).first : 0;
	}

	int* Act2Catalog(int Key)
	{
		ACTION2CATALOG::iterator iter = m_Action2Catalog.find(Key);
		if (iter != m_Action2Catalog.end())	
		{
			return &(iter->second);
		}

		return NULL;
	}

	CATALOG_INFO* GetCatalog(int Key)
	{
		CATALOGDICT::iterator iter = m_CatalogDict.find(Key);
		if (iter != m_CatalogDict.end())	
		{
			return &(iter->second);
		}

		return NULL;
	}

	int ItemLife(int nCatalogType, int emAction)
	{
		int key = (nCatalogType << 8) + emAction;
		ITEMACTIONGROUP::iterator iter = m_Life.find(key);
		if ( iter != m_Life.end())
		{
			return iter->second;
		}

		return -1;
	}

	int CatalogID(int nCatalogType, int nEntityType, int nActionType, int nValueType);

	typedef  lk::hash_map<unsigned int, int, 5000> TExpExceptDict;
	typedef  lk::hash_map<unsigned int, int, 500> TItemFilterDict;
	bool IsExpLimited(int nExpType, int nExpValue)
	{
		int nLimit = 0;
		switch(nExpType) 
		{
			case EM_UPGRADE_ROLE_EXP:
			{
				nLimit = m_nMinRoleExp;
				break;
			}

			case EM_UPGRADE_TALENT_EXP:
			{
				nLimit = m_nMinTalentExp;
				break;
			}

			case EM_UPGRADE_MW_EXP:
			{
				nLimit = m_nMinMWExp;
				break;
			}
		}

		return nExpValue < nLimit;
	}

	bool IsExcept(int nRoleID)
	{
		return m_ExpExceptRole.find(nRoleID) != m_ExpExceptRole.end();
	}

	bool AddFilterItem(int nItemID, int nOptype)
	{
		return m_ItemFilter.insert(TItemFilterDict::value_type((nOptype << 24) + nItemID, 0)).second; 
	}

	bool AddFilterDrop(int nItemID)
	{
		return m_DropFilter.insert(TItemFilterDict::value_type(nItemID, 0)).second; 
	}

	bool IsFilterDrop(int nItemID)
	{
		return m_DropFilter.find((nItemID)) != m_DropFilter.end();
	}

	bool IsFilterItem(int nItemID, int nOptype)
	{
		return m_ItemFilter.find((nOptype << 24) + nItemID)  != m_ItemFilter.end();
	}

private:
	ACTION2CATALOG m_Action2Catalog;
	CATALOGDICT m_CatalogDict;

	ITEMACTIONGROUP m_Life;

	// for exp limit
	TExpExceptDict m_ExpExceptRole;
	TItemFilterDict m_ItemFilter;
	TItemFilterDict m_DropFilter;

	int m_nMinMWExp;
	int m_nMinRoleExp;
	int m_nMinTalentExp;
};

#endif

