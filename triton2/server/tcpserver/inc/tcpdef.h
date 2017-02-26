/******************************************************************************
	版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

/******************************************************************************
	文件名          : tcpdef.h
	版本号          : 1.0
	作者            : 张宏亮
	生成日期        : 2008-05-28
	最近修改        : 
	功能描述        : Tcp接入服务器宏定义文件
	函数列表        : 
	修改历史        : 
	1 日期          : 2008-05-28
	  作者          : 张宏亮
	  修改内容      : 生成
*******************************************************************************/

#ifndef __TCPDEF_H__
#define __TCPDEF_H__

#include <time.h>
#include <string.h>


#ifdef SMALL_SIZE
#define MAX_SOCKET_NUM				200				/*单个TCPCtrl支持的连接数*/
#endif

#ifdef MEDIUM_SIZE
#define MAX_SOCKET_NUM				3300				/*单个TCPCtrl支持的连接数*/
#endif

#ifdef LARGE_SIZE
#define MAX_SOCKET_NUM				13500				/*单个TCPCtrl支持的连接数*/
#endif


#define MAX_PORT_NUM				3					/*最大端口数*/
#define MAX_IGNORE_ON_ERR_IP		3					/*发生socket错误时，忽略不写日志的IP数*/

#define CSHEAD_BASE_LEN				18					/*C/S协议头部的基本大小*/
#define CSHEAD_OPT_LEN				127					/*C/S协议可选部分的最大长度*/

#define LISTEN_SOCKET				1					/*监听socket*/ 
#define CONNECT_SOCKET				2					/*连接socket*/

#define RECV_DATA					1					/*标志socket要接收数据*/
#define SEND_DATA					2					/*标志socket要发送数据*/

#define IP_LENGTH					20					/*IP地址长度*/

#define SERVER_INIT_DAEMON			1					/*程序以后台的方式运行*/
#define MAX_BUF_LEN                 (100*1024)			/*服务器发送给客户端单个包的最大长度*/ 
#define RECV_BUF_LEN				(8 * 1024)			/*接收客户端信息的缓冲区*/ 
#define PIPE_SIZE					((int)0x1000000)	/*内存管道的大小*/
#define MAX_SEND_PKGS_ONCE			1000				/*一次从内存管道中取的最大的数据包*/


#define MAIN_HAVE_SEND_DATA			1					/*后端主服务器发送过数据给某Socket*/


enum ENSocketCloseSrcEn
{
	CLOSESOCKET_PLAYER			=	1,					/*客户端主动发起断连*/
	CLOSESOCKET_MAINSERVER		=	2,					/*后台主服务器发起断连*/
};


#define CONFIG_FILE					"../config/tcpsvrd.cfg"
#define LOG_FILE					"../log/tcpsvrd"
#define MAX_PATH                    260                       /*文件的最大路径名*/
#define INVALID_SOCKET              -1                        /*无效socket句柄*/
#define SOCKET_ERROR                -1                        /*socket api的返回值*/
#define INVALID_HANDLE_VALUE        ((void * )(-1))           /*无效句柄值*/
#define closesocket(fd)              shutdown(fd,2); close(fd);                     /*关闭socket*/

#define ERRPACKETLENGTH				-2

#define RcMutex                     pthread_mutex_t           /*线程锁*/  
#define ThreadType                  void*                     /*线程类型*/ 
typedef void                        *(*ThreadFunc )(void *);  /*线程函数指针*/


typedef int							LONG;
typedef unsigned int				ULONG;
typedef short						SHORT;
typedef unsigned short				USHORT;
typedef	unsigned char				BYTE;
typedef int                         SOCKET;


typedef enum _EnRunFlag
{
	reloadcfg = 1,
	tcpexit
}EnRunFlag;


/*Socket 连接错误码*/
typedef enum _EnSocketStatus
{
	Client_Succeed			= 0,	/*客户端连接正常*/
	Err_ClientClose			= -1,	/*客户端关闭*/ 
	Err_ClientTimeout		= -2,	/*客户端超时*/
	Err_PacketError			= -3,	/*客户端发送的包错误*/ 
	Err_TCPBuffOver			= -4,	/*TCP缓冲区已满*/ 
	Err_SendToMainSvrd		= -5,	/*数据包放入内存管道失败*/
	Err_System				= -6	/*系统错误，暂时未用*/
}EnSocketStatus;



/*定义返回结果*/
typedef enum _EnResult
{    
	TCP_SUCCESS = 0,                        /*成功*/            
	TCP_FAILED,                             /*失败*/     
	TCP_CONTINUERECV,                       /*消息还未接收完毕继续接收*/
	TCP_LENGTHERROR,                        /*接收的消息长度和标实的长度不一致*/    
}EnResult; 


/*当前时间*/
typedef struct _TTime
{
	ULONG		ulYear;			/*0000-9999*/
	ULONG		ulMonth;		/*00-12*/
	ULONG		ulDay;			/*01-31*/
	ULONG		ulHour;			/*00-23*/
	ULONG		ulMinute;		/*00-59*/
	ULONG		ulSecond;		/*00-59*/
}TTime;


/*程序的配置信息*/
typedef struct _TConfig 
{
	int          m_iSocketTimeOut;						/*socket的超时时间*/
	int          m_iConnTimeOut;						/*socket的超时时间*/
	int          m_iCheckTimeGap;
	int          m_iListenPortNum;						/*保存端口信息*/    
	int          m_iListenPorts[MAX_PORT_NUM];			/*保存端口信息*/    
	int          m_iTcpBufLen;							/*tcp发送缓冲区大小*/
	int          m_iMaxLogCount;			
	int          m_iWriteStatGap;						/*log服务器状态的间隔*/
	int          m_iShmMax;								/*共享内存最大数(size)*/
	int          m_iOpenFileMax;						/*文件句柄最大数*/
	int          m_iCoreFileSize;						/*core文件大小*/
	int			 m_iIgnoreOnErrIPNum;					/*发生socket错误时忽略不写日志的IP，IP总数 */
	char		 m_aszIgnoreOnErrIPs[MAX_IGNORE_ON_ERR_IP][32]; /* IP地址 */
}TConfig;


/*该结构是和mainserver通讯用的*/ 
typedef struct CNetHead
{ 
	enum { MAX_DEST_ENTITY = 1000 };

	ULONG	m_iSrcIP;					/*由tcpsvrd发给mainserver，记录了该socket的创建时间*/
	USHORT	m_nSrcPort;					/*由tcpsvrd发给mainserver，记录了该socket在数组中的位置*/
	ULONG	m_iDstIP;				/*由mainserver返回给tcpsvrd，值和m_iSrcIP相同*/ 
	USHORT	m_nDstPort;				/*由mainserver返回给tcpsvrd，值和m_iSrcPort*/
	time_t	m_tStamp;				/*接收到数据包的时间戳*/
	char	m_cState;				/*状态， < 0 说明关闭了socket*/

	CNetHead( ) : m_iSrcIP(0), m_nSrcPort(0), m_iDstIP(0), m_nDstPort(0),  m_tStamp(0), m_cState(0) {}
	~CNetHead() {}

}TNetHead;


#define TNetHeadSize     sizeof(TNetHead)   /*TNetHead大小*/


/*和客户端通讯的socket结构*/
typedef struct _TSocketInfo
{
	SOCKET  m_iSocket;						/*socket句柄*/
	int     m_iSocketType;					/*socket类型：监听socket、连接socket*/
	int     m_iSocketFlag;					/*socket标志：是否收包*/
	int     m_iRecvBytes;                   /*接收的字节数*/
	char    m_szClientIP[IP_LENGTH];		/*客户端IP地址*/
	char    m_szMsgBuf[RECV_BUF_LEN];		/*存储发送给gamesvrd的消息*/
	time_t  m_tCreateTime;					/*socket的创建时间*/
	ULONG   m_iSrcIP;						/*由tcpsvrd发给gamesvrd，记录了该socket的创建时间*/
	ULONG   m_iDstIP;						/*由gamesvrd返回给tcpsvrd，值和m_iSrcIP相同*/ 
	USHORT  m_nSrcPort;						/*由tcpsvrd发给gamesvrd，记录了该socket在数组中的位置*/
	USHORT  m_nDstPort;						/*由gamesvrd返回给tcpsvrd，值和m_iSrcPort*/
	time_t  m_tStamp;						/*接收到数据包的时间戳*/
	int     m_iSendFlag;					/*标识mainsvrd是否向其发送了包*/
	int     m_iConnectedPort;
	int		m_iUin;							/*用户唯一标识*/
} TSocketInfo;

#endif // __TCPDEF_H__
