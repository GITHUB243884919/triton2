#ifndef _CHARGE_SERVER_H_
#define _CHARGE_SERVER_H_

#include <time.h>
#include "loginlogic.h"
#include "servermessage_in.h"
#include "db_msg_pb.hxx.pb.h"

class CChargeServer: public CSingleton < CChargeServer >
{
public:
	enum 
	{
		em_query_consume  = 1,
	    em_query_stock    = 2,
	};

	bool CreateChargeServer(const char* szIpPort);
	int GetStatus() { return mChargeServer.GetStatus(); }
	void OnChargeServerAccept(int fd);
	void RecvEChargeMsg(int vMsgCount);
	void UnRegistGateSockfd(int fd);

	CChargeServer();
	~CChargeServer();

	void OnEChargeMessage(int nfd, unsigned char* message_buffer, int message_length);
	bool Send2ECharge(int nfd, SysProtocol* pproto);

	//CMD_BIND
	void OnAGIPCmdBind(int nfd, SysProtocol* pproto);

	//CMD_UNBIND
	void OnAGIPCmdUnBind(int nfd, SysProtocol* pproto);

	//CMD_HANDSET
	void OnAGIPCmdHandSet(int nfd, SysProtocol* pproto);

	//CMD_USER_CHARGE
	void OnAGIPUserCharge(int nfd, SysProtocol* pproto);
	void OnSessionChargeAmount(CMessage* tpMessage, int nfd , int nParam2);

	//CMD_USER_CHARGE_AUDIT
	void OnAGIPUserChargeAudit(int nfd, SysProtocol* pproto);
	void OnSessionChargeAudit(CMessage* tpMessage, int nfd , int nParam2);

	//CMD_CREATE_OR_UPDATE_USER
	void OnAGIPCreateOrUpdateUser(int nfd, SysProtocol* pproto);
	void OnSessionCreateOrUpdateUser(CMessage* tpMessage, int nfd , int nParam2);

	//CMD_USER_IB_PAY
	void OnMessageUserIbExchangeNotice(CMessage* tpMessage);

	//CMD_USER_IB_PAY_RES
	void OnAGIPMsgUserIbPayRes(SysProtocol* pSysProto, int nParam1, int nParam2);

	// OnSessionUserIbPayRes
	void OnSessionUserIbPayRes(CMessage* tpMessage, int nAccount , int nSubjectID);

	//CMD_USER_USE_PRODUCT
	void OnMessageUserUseIbItemNotice(CMessage* tpMessage);

	//CMD_USER_USE_PRODUCT_RES
	void OnAGIPMsgUserUseProductRes(SysProtocol* pSysProto, int nParam1, int nParam2);

	//load all sending msg from db, and transfer it to erating.	
	void LoadIbPayRetryFromDB(int nStart, int nCount);

	//OnSession Load IbPay Current records
	void OnSessionLoadIbPayFromDB(CMessage* tpMessage, int nParam1 , int nParam2);

	//OnUserIbPayResIsAndit method
	void OnUserIbPayResIsAudit(uint32_t nAccountID, int nStart, int nCount);

	//OnTimerOfIbPayRetry
	void OnTimerOfIbPayRetry();

	//OnTimerOfIbUsedRetry
	void OnTimerOfIbUsedRetry();

	//UpdateIbExchangeError
	void UpdateIbExchangeNoticeError(int nRatingID, uint32_t nTransID);

	//LoadIbItemUsedFromDB 
	void LoadIbItemUsedFromDB(int nStart, int nCount);

	//OnSessionLoadIbItemUsedFromDB
	void OnSessionLoadIbItemUsedFromDB(CMessage* tpMessage, int nParam1 , int nParam2);

	// 设置是否正在load记录的状态
	void SetRetrying(bool bRetry) { m_bIsRetrying = bRetry; }

	// 新对账协议
	void OnAGIPPayListQuery(int nfd, SysProtocol* pSysProto);
	void OnSessionPayListQuery(CMessage* tpMessage, int nParam1, int nParam2);

	void OnAGIPPayDetailDemand(int nfd, SysProtocol* pSysProto);
	void OnSessionPayDetailDemand(CMessage* tpMessage, int nParam1, int nParam2);

	CAGIPSession* CreateSession4Proto(SysProtocol* pproto, int nfd);

	void SetIsSendingIbItemUsed(bool flag) { m_bIsSendingIbItemUsed = flag; }

	int LoadBourseLog( );
	int OnSessionLoadBourseLog( CMessage* tpMessage, int nParam1 , int nParam2);

	int OnAGIPMSGExchange( SysProtocol* pSysProto, int nParam1, int nParam2 );
	int UpdateExchangeError( int nParame1, int nParame2 );

private:
	enum EFDType
	{ 
		FD_IS_LISTENER, 
		FD_FROM_ECHARGING, 
		FD_IS_UNKNOWN,
	};

	// 与 socket 相关的
	enum 
	{
		MAX_SOCK_NUM = 128 
	};

	int maxfd;
	struct 
	{
		int fd;				// file descriptor

		time_t create_ts; 	// carete timestampt
		time_t active_ts;	// active timestampt
		EFDType from;

		int addr;			// from ip
		int port;			// from port
		CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH>* pactor;	

		int session_id;		// session_id, default 0;
	}mSocks[MAX_SOCK_NUM+1];  // fd的信息存储在mSock[fd]的位置

	// 短链接，大概1个连接只会同时处理一个或几个事务
	// list<int> mSessionList;

	CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH> mChargeServer;

	// Some Error Retry Record in tab.
	bool m_bHasSomeRetryIbPay;
	bool m_bIsRetrying;
	bool m_bFirstLoad;
	bool m_bIsSendingIbItemUsed;
};


#endif
