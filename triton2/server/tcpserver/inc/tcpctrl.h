/******************************************************************************
   版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

/******************************************************************************
  文件名          : tcpctrl.h
  版本号          : 1.0
  作者            : 张宏亮
  生成日期        : 2008-05-28
  最近修改        : 
  功能描述        : TCPServer的主控类完成和客户端的通讯、和mainserver的通讯，客户端的超时检测以及客户端的
  函数列表        : 
  修改历史        : 
  1 日期          : 2008-05-28
    作者          : 张宏亮
    修改内容      : 生成
*******************************************************************************/
#ifndef __TCPCTRL_H__
#define __TCPCTRL_H__

#define MAX_ERRNO_NUM 10
#define READSTAT      0
#define WRITESTAT     1
    
typedef struct 
{
    int     m_iConnIncoming;
    int     m_iConnTotal;
	int		m_iPkgRecv;
	int		m_iPkgSend;
	int		m_iPkgSizeRecv;
	int		m_iPkgSizeSend;
}TTcpStat;


/*管道标识符*/
enum enLockIdx
{
    IDX_PIPELOCK_C2S = 0,
    IDX_PIPELOCK_S2C = 1,
    IDX_PIPELOCK_A2C = 2,
};

class    CSharedMem;
class    CCodeQueue;
class    CConfigFile;


int GetUinFromCode(BYTE* pCode, short nCodeLength);




class CTCPCtrl
{
public:
    CTCPCtrl();				/*构造函数*/
    ~CTCPCtrl();			/*析构函数*/
    int Initialize();		/*初始化*/
    int PrepareToRun();
    void* operator new( size_t nSize);		/*重载的new函数*/
    void  operator delete(void *pBase);		/*重载的delete函数*/
    static CTCPCtrl* CreateTCPCtrl();		/*创建CTCPCtrl对象的实例*/
    static int CountSize();					/*计算CTCPCtrl对象占用的空间*/
    int  Run();								/*调度函数*/   
    int  SetRunFlag(int iRunFlag);
	void SetRunArgs( const char* pszArgs );
	char* GetRunArgs( );


private:        
    int  CreatePipe();                                             /*创建内存管道*/

    CCodeQueue  *CreatePipe(const char*szLockFile, char byPipeID, int iLockIdx); /*创建内存管道*/
 
    inline void ClearSocketInfo(short enError);                    /*清除socket结构信息*/
    int  CheckTimeOut();                                           /*检测客户端socket是否超时*/       
    int  GetExMessage();                                           /*接收外部数据*/
    int  RecvClientData(short nIndex);                             /*接收客户端发送的数据*/
    int  CheckWaitSendData();                                      /*检测gamesvrd是否有数据需要发送*/
    int  SendClientData(BYTE* pbyInBuf, short nInLength);          /*发送gamesvrd数据给client*/        
    int  ReadCfgFile();
    int  ReloadCfgFile();
    int  TcpWrite(SOCKET iSocket, char *pBuf, int iPackLen);
    int  TcpRead(SOCKET iSocket, char *pBuf, int iLen);
    int  WriteTcpStat();
    int        InitEpollSocket(USHORT port);
    int        eph_init();
    int        eph_socket(int domain, int type, int protocol);    
    int        eph_new_conn(int sfd);
    int        eph_close(int sfd);
    int        eph_cleanup();    

	bool IsIgnoreIP( char* szIP );

private:
    static BYTE*        pCurrentShm;                      /*内存块的指针*/     
    CConfigFile         m_stConfigFile;
    TConfig             m_stConfig;                       /*配置信息*/
    int                 m_iRunFlag;
    time_t              m_iLastTime;                      /*上次检测超时的时间*/
    time_t              m_iLastWriteStatTime;             /*上次检测超时的时间*/
    time_t              m_iNowTime;                       /*当前时间*/
    TNetHead            m_stNetHead;                      /*NetHead结构，和gamesvrd通讯用*/
    struct  timeval     m_tvWait;                         /*网络事件超时设置*/ 
    SOCKET              m_iSocket;                        /*监听socket*/        
    struct sockaddr_in  m_stSockAddr;                     /*网络地址*/
    TSocketInfo         *m_pstSocketInfo[MAX_SOCKET_NUM];	/*socket结构数组，用于记录客户端的信息*/        
    TSocketInfo         *m_pSocketInfo;                   /*当前的socket结构指针*/ 

    CSharedMem          *m_pShmCSPipe;                    /*共享内存client   ＝》 gamesvrd*/
    CSharedMem          *m_pShmSCPipe;                    /*共享内存gamesvrd ＝》 client*/

    CCodeQueue          *m_pC2SPipe;                      /*输入队列*/ 
    CCodeQueue          *m_pS2CPipe;                      /*输出队列*/

    char                m_szCSPipeFile[500];              /*本地文件*/ 
    char                m_szSCPipeFile[500];              /*本地文件*/ 

    char                m_szMsgBuf[MAX_BUF_LEN];        /*消息包缓冲(加大是为了防止game过来的消息过大)*/ 
    int                 timeout;
    TTcpStat            m_stTcpStat;
    int                 m_iWriteStatCount;
    char                m_szWriteStatBuf[1024];
    struct epoll_event  *events;
    int                 kdpfd;
    int                 maxfds;                
    struct epoll_event  m_stEpollEvent;
	char				m_szRunArgs[100];
};

#endif /* __RCCTRL_H__ */

