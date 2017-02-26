#ifndef _TCP_CONN_H_
#define _TCP_CONN_H_

#include "base.h"
#include "log.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <net/if.h>

//以下定义tcp连接状态
enum eTCPConnStates
{
	tcs_closed = 0,
	tcs_opened,
	tcs_connected,
	tcs_error,
	tcs_connecting,
};
//以下定义Socket类型
enum eSockTypes
{
	sot_listen = 0,
	sot_comm
};
//以下定义接受/发送错误类型
enum eRecvErrs
{
	ERR_RECV_NOSOCK = -1,
	ERR_RECV_NOBUFF = -2,
	ERR_RECV_REMOTE = -3,
	ERR_RECV_FALIED = -4
};
enum eSendErrs
{
	ERR_SEND_NOSOCK = -1,
	ERR_SEND_NOBUFF = -2,
	ERR_SEND_FAILED = -3,
};

enum eByteMode
{
	use_host_byte = 0,
	use_network_byte = 1
};

enum eLinkMode
{
	em_block_mode = 0,
	em_nblock_mode = 1
};

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
class CTCPSocket
{
public:
	CTCPSocket();
	~CTCPSocket();
	int GetSocketFD();
	int GetStatus();
	int CreateClient( char* szLocalAddr = NULL );
	int CreateServer(unsigned short unPort, char* szIPAddr=NULL );
	int ConnectTo(char *szIPAddr, unsigned short unPort);
	int ConnectTo(u_long ulIPNetAddr, u_short unPort, eLinkMode emBlock = em_block_mode);
	int Accept( int iAcceptFD );
	
	int RecvData();
	int GetOneCode( unsigned short &nCodeLength, BYTE *pCode, eByteMode emByte = use_host_byte);
	//int GetOneHttpCode( int &nCodeLength, BYTE *pHeadCode, BYTE* pBodyCode); 
	int GetOneHttpCode( int &nCodeLength, BYTE *pCode); 
	int GetOneCode32( int &iCodeLength, BYTE * pCode);
	int SendOneCode( unsigned short nCodeLength, BYTE *pCode  );
	int SendOneCode32( int nCodeLength, BYTE *pCode  );

		
	int AddToCheckSet( fd_set *pCheckSet );
	int IsFDSetted( fd_set *pCheckSet );
	int SetNBlock(int iSock);		

	int HasReserveData();
	int CleanReserveData();
	int PrintfSocketStat();
	int CheckNoblockConnecting(int nto = 0);

	void GetCriticalData(int& iReadBegin,int& iReadEnd, int& iPostBegin, int& iPostEnd);
	int Close();
	
protected:
	int m_iSocketFD;					//Socket描述子
	int m_iSocketType;					//Socket类型
	int m_iStatus;						//连接状态
#ifdef _POSIX_MT_
	pthread_mutex_t m_stMutex;			//操作的互斥变量
#endif

private:
	int  m_iReadBegin;
	int  m_iReadEnd;
	BYTE m_abyRecvBuffer[uiRecvBufLen+1];

		
	int  m_iPostBegin;
	int  m_iPostEnd;
	BYTE m_abyPostBuffer[uiSendBufLen+1];
		
};


template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
class CTCPConn
{
public:
	CTCPConn();
	~CTCPConn();

	int Initialize(short nEntityType, short nEntityID, u_long ulIPAddr, u_short unPort);
	int ConnectToServer(char* szLocalAddr  = NULL, eLinkMode emBlock = em_block_mode);
	int CreateServer();
	int GetEntityInfo(short* pnEntityType, short* pnEntityID, unsigned long* pulIpAddr);
	u_long GetConnAddr();
	u_short GetConnPort();
	short  GetEntityType();
	short  GetEntityID();
	CTCPSocket<uiRecvBufLen, uiSendBufLen>* GetSocket();

private:
	short   m_nEntityType;
	short   m_nEntityID;
	CTCPSocket<uiRecvBufLen, uiSendBufLen> m_stSocket;
	u_long m_ulIPAddr;
	u_short m_unPort;
};

#include "tcp_conn.cpp"

#endif

