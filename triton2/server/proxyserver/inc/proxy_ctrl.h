#ifndef _PROXY_CTRL_HPP_
#define _PROXY_CTRL_HPP_

#include "proxy_handle.h"
#include "proxy_conn.h"

//以下定义所有运行标志
#define FLG_CTRL_QUIT			((BYTE)0x01)	//通知主控对象退出
#define FLG_CTRL_RELOAD			((BYTE)0x02)	//重读配置

#define MAX_TMP_BUF_LEN                               1024           //注册消息最大长度
#define MAX_UNREGISTER_NUM                      100             //当前未注册的最大个数

#define CHECK_INTERVAL_SECONDS              10             //检查时间间隔
#define CHECK_TIMEOUT_SECONDS                 30            //连接未注册的超时时间
#define GLOBALPROXY_ALIVE_TIME 1000

//当前连接上，但还未注册结构信息
struct STUnRegisterSocketInfo
{
	int m_iSocketFD;                    //连接套接口
	u_long m_ulIPAddr;                  //连接ip地址
	time_t  m_tAcceptTime;              //连接发起时间
};

class CProxyCtrl
{
public:
	CProxyCtrl();
	~CProxyCtrl();


	enum _enParas
	{
		MAX_MUTEX_NUM		= 2,
		MUTEX_HASHMAP		= 0,
		MUTEX_ROUTETAB		= 1
	};
	

	int Initialize(const char *szCfgFile);
	int PrepareToRun();
	int Run();


#ifdef _POSIX_MT_
	static pthread_mutex_t stMutex[MAX_MUTEX_NUM];
#endif
	

protected:

private:

	int ReadCfg(const char *szCfgFile);
	int PrintStatistic( );

	//int LoadConnInfoFromFile(short nConnType, CMyTCPConn* pConns, int& iConnNum);
	//int LoadHashMapFromFile();
	//int LoadMRTFromFile();

	// 检查与全局代理服务器的连接状态(如果设置了全局服务器，而且没有连接上的话，则不停重连)
	int CheckGloalProxyStatus();
	bool Regist2GlobalProxy();
	int KeepAliveToGlobalProxy();
	int CheckTimer( );
	int CheckAliveTime( unsigned int nTime );

	int CheckRunFlags();
	int CheckConnRequest();
	int CheckRoutines();       //定时检查
	CMyTCPConn* GetConnByAddrAndID(short shSrcFE, short shSrcID, u_long ulSrcAddr, int nGameID );

	int ReceiveAndProcessRegister(int iUnRegisterIdx);  //接收并处理注册消息
	int DeleteOneUnRegister(int iUnRegisterIdx);    //删除一个未注册的连接
	

	void  pbmsg_setproxy(CProxyHead* pProxy, unsigned int nSrcFE, unsigned int nDstFE, unsigned int nSrcID, 
		unsigned int nDstID, EMTransType emTransTp = TRANS_P2P, EMProxyCmd emProxyCmd = CMD_NULL, 
		unsigned char* szOption = NULL, unsigned int nOptlen = 0);

	TProxyCfg m_stProxyCfg;
	CMyTCPSocket m_stListenSocket;					// (全局和游戏代理服务器监听用的socket)

	//CMyTCPConn m_astMainConn[MAX_GAME_SERVER];
	CMyTCPConn* m_pstMainConn;
	//CMyTCPConn m_astDBConn[MAX_DB_SERVER];
	CMyTCPConn* m_pstDBConn;
	//CMyTCPConn m_astOtherConn[MAX_OTHER_SERVER];	
	CMyTCPConn* m_pstOtherConn;
	
	CMyTCPConn m_astGlobalProxy;					// 连接全局服务器用

	CProxySet	 m_astProxyConn;	// 代理服务器
	//CMyTCPConn m_astDunGeon[MAX_DUNGEON_NUM];	// 全局副本服务器
	CMyTCPConn* m_pstDunGeon;
	

	CProxyHandle* m_pMainHandle;
	CProxyHandle* m_pDBHandle;
	CProxyHandle* m_pOtherHandle;
	CProxyHandle *m_pProxyHandle;	// 代理服务器
	CProxyHandle* m_pDungeonHandle;					// 全局游戏服务器
	CProxyHandle* m_pGlobalProxy;					// 全局代理服务器
	

	///更改proxy的连接验证机制
	///之前为依据ip，现更改为id和ip共同验证
	int m_iCurrentUnRegisterNum;
	STUnRegisterSocketInfo m_astUnRegisterInfo[MAX_UNREGISTER_NUM];               //最大未注册结构
	time_t m_tLastCheckTime;
	unsigned int mLastTickCount;
	int mAliveTimer;
};

#endif
