#include "charge_service.h"
#include "message_pb.hxx.pb.h"
#include "gmmessage_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "base.h"
#include "loginobj_manager.h"
#include "db_msg_pb.hxx.pb.h"
#include "template.h"
#include "tinyxml.h"
#include "ibstore.h"

#define CONST_AUDIT_COUNT_LIMIT 64

template<> CChargeServer* CSingleton< CChargeServer >::spSingleton = NULL;
CChargeServer::CChargeServer()
{
	// 是否有需要重发的消费记录
	m_bHasSomeRetryIbPay = true;

	// 是否正在重发消费记录
	m_bIsRetrying = false;

	m_bIsSendingIbItemUsed = false;


	m_bFirstLoad = true;

	for(unsigned int i = 0; i < sizeof (mSocks) /sizeof(mSocks[0]); ++i)
	{
		mSocks[i].fd = -1;
		mSocks[i].pactor = NULL;
		mSocks[i].from = FD_IS_UNKNOWN;
	}

	maxfd = 0;
}

CChargeServer::~CChargeServer()
{

}

// 创建InfoSever服务器, 内网链接
bool CChargeServer::CreateChargeServer(const char* szIpPort)
{
	//char tIP[ 64 ] = {0};
	unsigned short tPort = (unsigned short) atoi(szIpPort);

	//SplitSockAddr(szIpPort, tIP, tPort );

	if ( mChargeServer.CreateServer(tPort, NULL) != 0)
	{
		LOG_ERROR("echarge", "echarge create server failure, maybe the ip error.");
		return false;
	}

	int fd = mChargeServer.GetSocketFD(); 
	if ( fd > MAX_SOCK_NUM ) 
	{
		LOG_ERROR("echarge", "The sockfd is large than the programe permited");
		mChargeServer.Close();
		return false;
	}

	if (fd > maxfd) maxfd = fd;
	mSocks[fd].fd = fd;
	mSocks[fd].create_ts = time(0);
	mSocks[fd].from = FD_IS_LISTENER;
	mSocks[fd].addr = htonl(INADDR_ANY);
	mSocks[fd].port = tPort; 


	return true;
}

// 处理新的Gate连接
void CChargeServer::OnChargeServerAccept(int fd)
{
	// 取新的链接fd， 并将其写入mSocks		
	struct sockaddr_in cnt_addr;
	socklen_t cnt_len = sizeof(struct sockaddr);

	int cnt = accept(fd, (struct sockaddr *)&cnt_addr, (socklen_t *)&cnt_len);	
	if (cnt > 0)
	{
		if (cnt > MAX_SOCK_NUM) 
		{
			LOG_ERROR("echarge", "sockfd(%d) is large than the progame permited(%d)", cnt, MAX_SOCK_NUM);
			close(cnt);
			return;
		}

		// 是否在黑名单IP中 
		if ( CLoginServer::GetSingleton().IsBlackIP(cnt_addr.sin_addr.s_addr))
		{
			const unsigned char* p = (const unsigned char*) &(cnt_addr.sin_addr.s_addr);
			LOG_DEBUG("echarge", "%d.%d.%d.%d is in black ip list. it will be blocked", *p, *(p+1), *(p+2), *(p+3));

			close(cnt);
			return;
		}   

		// 尝试分配内存, 失败则返回
		try{
			mSocks[cnt].pactor = new CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH>;
		}catch(...)
		{
			mSocks[cnt].pactor = NULL;
			close(cnt);

			LOG_ERROR("echarge", "new CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH> error for fd(%d))", cnt);
			return;
		}

		mSocks[cnt].fd = cnt;
		mSocks[cnt].create_ts = time(0);	
		mSocks[cnt].from = FD_FROM_ECHARGING;
		mSocks[cnt].addr = cnt_addr.sin_addr.s_addr;
		mSocks[cnt].port = cnt_addr.sin_port;

		if (mSocks[cnt].pactor == NULL)
		{
			LOG_ERROR("echarge", "new CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH> error for fd(%d))", cnt);
			return;
		}

		mSocks[cnt].pactor->Accept(cnt);

		if(cnt > maxfd ) maxfd = cnt;

		// 设置成NOBLOCK
		int flags = 1;
		if ((flags = fcntl(cnt, F_GETFL, 0)) < 0 || fcntl(cnt, F_SETFL, flags | O_NONBLOCK) < 0)
		{
			LOG_ERROR("echarge","set nonblock on socket %d error!", cnt);

			//close(cnt); mSocks[cnt].fd = -1;
			UnRegistGateSockfd(cnt);
		}else
		{
			unsigned char *p = (unsigned char *) &(mSocks[cnt].addr); 
			LOG_INFO("echarge", "client [%d.%d.%d.%d:%d]", *p, *(p+1), *(p+2), *(p+3), mSocks[cnt].port);
		}
	}else
	{
		unsigned char *p = (unsigned char *) &(cnt_addr.sin_addr.s_addr);

		LOG_INFO("echarge", "accept(fd=%d) error on %d.%d.%d.%d:%d for error(%d:%s)",
				fd, *p, *(p+1), *(p+2), *(p+3), cnt_addr.sin_port, errno, strerror(errno));
	}

}
// 接受从服务器过来的消息， 包括ERating, 各个GateServer, 以及侦听GateServer的链接
void CChargeServer::RecvEChargeMsg(int vMsgCount)
{
	//int fd_ndx = -1;
	fd_set fds_read;

	// 取得当前可以读的端口的数量
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	FD_ZERO( &fds_read );

	for (int i=0; i<maxfd+1; ++i)
	{
		if ( mSocks[i].fd > 0 )//&& (mSocks[i].pactor->GetStatus() == tcs_connected  || mSocks[i].from == FD_IS_LISTENER))
		{
			FD_SET(mSocks[i].fd, &fds_read);
		}
	}

	int n= select( maxfd + 1, &fds_read, NULL, NULL, &timeout);
	if (n < 0 ) //&& errno != EINTR)
	{
		LOG_ERROR("echarge", "%s:%d, fdevent_poll failed:%s, maxfd(%d)",  __FILE__, __LINE__, strerror(errno), maxfd);
		return;
	}

	if ( n == 0 ) return;

	for (int i = 0; i < maxfd+1; ++i)
	{
		if ( mSocks[i].fd > 0 && FD_ISSET(mSocks[i].fd, &fds_read))
		{
			switch(mSocks[i].from)
			{
				case FD_IS_LISTENER:
				{
					OnChargeServerAccept(mSocks[i].fd);
					break;
				}
	
				case FD_FROM_ECHARGING:
				{

					BYTE tCodeBuff[ MAX_CODE_LEN ];
					unsigned short tBuffLen = sizeof( tCodeBuff );
					int iRet = -1;

					if ( mSocks[i].pactor == NULL || mSocks[i].fd == -1)
					{
						continue;
					}

					int r = mSocks[i].pactor->RecvData();
					if (r == ERR_RECV_REMOTE || r == ERR_RECV_FALIED)
					{
						LOG_ERROR("echarge", "charge server recv data error.");
						UnRegistGateSockfd(i);

					}else
					{
						while (1)
						{
							tBuffLen = sizeof( tCodeBuff );
							if ( mSocks[i].fd > 0 ) // 此处增加判断是以为OnEChargeMessage 有可能改变mSocks[i]
							{
								iRet = mSocks[i].pactor->GetOneCode(tBuffLen, tCodeBuff, use_network_byte);
								if ( mSocks[i].pactor->GetStatus() != tcs_connected)
								{
									UnRegistGateSockfd(i);
									break;
								}

								if ( iRet <= 0  ) break;

								if( tBuffLen == 0 || tBuffLen > sizeof(tCodeBuff))
								{
									LOG_ERROR( "echarge", "Code Len %d impossible", tBuffLen );
									break;
								}

								OnEChargeMessage(mSocks[i].fd, tCodeBuff, tBuffLen);
							}else
							{
								break;
							}
						}
					}

					break;
				}

				case FD_IS_UNKNOWN:
				{
					LOG_ERROR("echarge", "Can't find the fd %d", i);
					break;
				}
			}
		}
	}
}

// 释放fd
void CChargeServer::UnRegistGateSockfd(int fd)
{
	LK_ASSERT(fd > 0 && mSocks[fd].pactor != NULL, return);

	mSocks[fd].pactor->Close();

	mSocks[fd].fd = -1;
	if (fd == maxfd)
	{
		for (int i = maxfd - 1; i > 0 ; --i)
		{
			if (mSocks[i].fd != -1)
			{
				maxfd = i;
				break;
			}
		}
	}

	mSocks[fd].from = FD_IS_UNKNOWN;
	delete mSocks[fd].pactor;
	mSocks[fd].pactor = NULL;
}

void CChargeServer::OnEChargeMessage(int nfd, unsigned char* message_buffer, int message_length)
{
	SysProtocol* pproto = NULL;

	unsigned int nCmdID = 0;
#ifdef USE_DECRYPTION_PROTOCAL
	SysProtocol::parseCommandID(message_buffer, message_length, &nCmdID);
#else
	SAGIPHeader*  pSAGIPHeader = (SAGIPHeader *)message_buffer;
	nCmdID = htonl(pSAGIPHeader->un_Command_ID);
#endif

	LOG_DEBUG("echarge", "Recv From ECharge MsgID(%x), MsgLen(%d)", nCmdID, message_length);

	switch  (nCmdID)
	{
		case CMD_BIND:
		{
			pproto = new AGIPBind;
			break;
		}

		case CMD_UNBIND:
		{
			pproto = new AGIPUnbind;
			break;
		}

		case CMD_HANDSET:
		{
			pproto = new AGIPHandset;
			break;
		}

		case CMD_USER_CHARGE:
		{
			pproto = new AGIPUserCharge;
			break;
		}

		case CMD_USER_CHARGE_AUDIT:
		{
			pproto = new AGIPUserChargeAudit;
			break;
		}

		case CMD_CREATE_OR_UPDATE_USER:
		{
			pproto = new AGIPCreateOrUpdateUser;
			break;
		}

		case CMD_PAY_LIST_QUERY:
		{
			pproto = new AGIPPayListQuery;
			break;
		}

		case CMD_PAY_DETAIL_DEMAND:
		{
			pproto = new AGIPPayDetailDemand;
			break;
		}

		case CMD_USER_IB_PAY_RES:
		{
			pproto = new AGIPUserIBPayRes;
			break;
		}
		case CMD_USER_CASH_EXCHANGE_EX_RES:
		{
			pproto = new AGIPUserCashExchangeExRes;
			break;
		}

		default:
		{
			LOG_ERROR("echarge", "there is no action for ECharge Message(%x)", nCmdID);
			return;
		}
	}

	if (pproto == NULL) return;

#ifdef USE_DECRYPTION_PROTOCAL
	int result = pproto->setEncodedPDU(message_buffer, message_length, 0);
#else
	int result = pproto->setDecodedPDU(message_buffer, message_length, 0);
#endif

	if (result != S_SUCCESS)
	{
		LOG_ERROR("echarge", "SetEncodePDU/SetDeCodePDU fail,CommondID(%x) Length(%d), Error(%d)",
				nCmdID, message_length, result);
		delete pproto;
		return;
	}

#ifdef _ECHARGE_DEBUG_
	if ( nCmdID != CMD_HANDSET)
	{
		printf("recv from echarge ... {\n");
		pproto->showInfo();
		printf("}\n");
	}
#endif

	switch  (nCmdID)
	{
		case CMD_BIND:
		{
			OnAGIPCmdBind(nfd, pproto);
			break;
		}

		case CMD_UNBIND:
		{
			OnAGIPCmdUnBind(nfd, pproto);
			break;
		}

		case CMD_HANDSET:
		{
			OnAGIPCmdHandSet(nfd, pproto);
			break;
		}

		case CMD_USER_CHARGE:
		{
			OnAGIPUserCharge(nfd, pproto);
			break;
		}

		case CMD_USER_CHARGE_AUDIT:
		{
			OnAGIPUserChargeAudit(nfd, pproto);
			break;
		}

		case CMD_CREATE_OR_UPDATE_USER:
		{
			OnAGIPCreateOrUpdateUser(nfd, pproto);
			break;
		}

		case CMD_PAY_LIST_QUERY:
		{
			OnAGIPPayListQuery(nfd, pproto);
			break;
		}

		case CMD_PAY_DETAIL_DEMAND:
		{
			OnAGIPPayDetailDemand(nfd, pproto);
			break;
		}

		case CMD_USER_IB_PAY_RES:
		{
			CLoginLogic::GetSingleton().OnERatingMessage(pproto);
			break;
		}
		case CMD_USER_CASH_EXCHANGE_RES:
		{

		}

		default:
		{
			LOG_ERROR("echarge", "there is no action for echarge msg (id=%x) on %s", nCmdID, __FUNCTION__);
			delete pproto;
			return ;
		}
	}

	delete pproto;
}

void CChargeServer::OnAGIPCmdBind(int nfd, SysProtocol* pSysProto)
{
	LK_ASSERT(pSysProto != NULL, return);
	AGIPBind* pproto = (AGIPBind*) pSysProto;

	uint32_t  nServerID;
	int8_t nReconnectFlag;
	char szMacAddr[12+1] = {0};
	char szGatewayCode[32+1] = {0};
	char szPassword[32+1] = {0};
	uint32_t nSequenceID;

	if (pproto->getSequenceID(&nSequenceID) != S_SUCCESS
	||	pproto->getServerID(&nServerID) != S_SUCCESS
	||  pproto->getReconnectFlag(&nReconnectFlag) != S_SUCCESS
	||  pproto->getMAC(szMacAddr, sizeof(szMacAddr) - 1 ) != NULL
	||  pproto->getPassword(szPassword) != S_SUCCESS
	||  pproto->getGatewayCode(szGatewayCode) != S_SUCCESS)
	{
		LOG_ERROR("echarge", "echarge proto error CMD_BIND(server_id = {%d}, recconect_flag = {%d}, "
					" mac_addr = {%s}, gateway_code = {%s}, password = {%s}",
				nServerID, nReconnectFlag, szMacAddr, szGatewayCode, szPassword);
		return;
	}

	int nResult = S_SUCCESS;
	//认证
	if(strcmp(szGatewayCode, CLoginServer::GetSingleton().mConfig.szEChargeCode) != 0)
	{
		LOG_ERROR("echarge", "echarge bind error because gatecode(%s,%s)", 
				szGatewayCode, CLoginServer::GetSingleton().mConfig.szEChargeCode); 

		nResult = E_GATEWAY_NOT_FOUND;
	}else
	if (strcmp(szPassword, CLoginServer::GetSingleton().mConfig.szEChargePasswd) != 0)
	{
		LOG_ERROR("echarge", "echarge bind error because passwd(%s,%s)", 
				szPassword, CLoginServer::GetSingleton().mConfig.szEChargePasswd);

		nResult = E_GATEWAY_UID_PWD_ERROR;
	}

	AGIPBindRes protores;
	protores.setGatewayID(CLoginServer::GetSingleton().mConfig.nEChargeID);
	protores.setResultCode(nResult);
	protores.setSequenceID(nSequenceID);
	protores.setGameID(pproto->getGameID());

	Send2ECharge(nfd, &protores);

	if (nResult != S_SUCCESS)
	{
		UnRegistGateSockfd(nfd);
	}
}

bool CChargeServer::Send2ECharge(int nfd, SysProtocol* pproto) 
{
	LK_ASSERT(nfd > 0 && pproto != NULL, return false);

	unsigned char message_buffer[AGIP_MAX_BUFFER_SIZE+1] = {0};
	int message_length;


	// SetGatewayID
	pproto->setGatewayID(CLoginServer::GetSingleton().mConfig.nEChargeID);

	// SetGameID
	if ( pproto->getGameID() == 0)
	{
		pproto->setGameID(CLoginLogic::GetSingleton().GetGameID());
	}

#ifdef _ECHARGE_DEBUG_
	uint32_t nCmdID = 0;
	pproto->getCommandID(&nCmdID);

	if ( nCmdID != CMD_HANDSET_RES)
	{
		printf("send to echarge ... {\n");
		pproto->showInfo();
		printf("}\n");
	}
#endif

#ifdef USE_DECRYPTION_PROTOCAL
	int result = pproto->getEncodedPDU(message_buffer, AGIP_MAX_BUFFER_SIZE, &message_length);
#else   
	int result = pproto->getDecodedPDU(message_buffer, AGIP_MAX_BUFFER_SIZE, &message_length);
#endif
	if (result != S_SUCCESS)
	{
		LOG_ERROR("echarge", "AGIPBindRes getEnCodePDU/getDecodePDU error");
		return false;
	}

	if (mSocks[nfd].pactor != NULL && mSocks[nfd].pactor->GetStatus() == tcs_connected)
	{
		if (mSocks[nfd].pactor->SendOneCode(message_length, message_buffer) != 0 ) 
		{
			UnRegistGateSockfd(nfd);
			return false;
		}
	}else
	{
		UnRegistGateSockfd(nfd);
		return false;
	}

	return true;
}

void CChargeServer::OnAGIPCmdUnBind(int nfd, SysProtocol* pSysProto)
{
	LK_ASSERT(pSysProto != NULL, return);

	AGIPUnbind* pproto = (AGIPUnbind*) pSysProto;
	uint32_t  nGatewayID = 0;
	uint32_t  nSequenceID = 0;

	if (pproto->getGatewayID(&nGatewayID) != S_SUCCESS
	||  pproto->getSequenceID(&nSequenceID) != S_SUCCESS)
	{
		LOG_ERROR("echarge", "echarge proto error CMD_UNBIND(gateway_id = {%d})", nGatewayID);
		return;
	}

	AGIPUnbindRes protores;
	protores.setResultCode(1);
	protores.setSequenceID(nSequenceID);
	protores.setGameID(pproto->getGameID());
	
	Send2ECharge(nfd, &protores);
}

void CChargeServer::OnAGIPCmdHandSet(int nfd, SysProtocol* pSysProto)
{
	LK_ASSERT(pSysProto != NULL, return);
	AGIPHandset* pproto = (AGIPHandset*) pSysProto;
	uint32_t nSequenceID;
	pproto->getSequenceID(&nSequenceID);
	

	AGIPHandsetRes protores;
	protores.setSequenceID(nSequenceID);
	protores.setGameID(pproto->getGameID());
	Send2ECharge(nfd, &protores);
}

// 充值使用的detail_id直接使用erating的，由erating保证它的唯一
void CChargeServer::OnAGIPUserCharge(int nfd, SysProtocol* pSysProto)
{
	LK_ASSERT(pSysProto!= NULL, return);
	AGIPUserCharge* pproto = (AGIPUserCharge*) pSysProto;

	int32_t  		total_amount = 0;
	uint32_t 		nSequenceID;

	if( pproto->getSequenceID(&nSequenceID) != S_SUCCESS)
	{
		LOG_ERROR("echarge", "echarge proto error CMD_USER_CHARGE sequence error! "); 
		return;
	}

	uint32_t        rating_id = pproto->getRatingID();
	uint32_t        user_id = pproto->getUserID();
	int16_t         subject_id = pproto->getSubjectID();
	int16_t         audit_flag = pproto->getAuditFlag();
	uint64_t        detail_id = pproto->getDetailID();
	int32_t         amount = pproto->getAmount();
	uint32_t  		charge_time = pproto->getChargeTime();
	uint32_t 		game_id = pproto->getGameID();

	AGIPUserChargeRes* pprotores = new AGIPUserChargeRes;

	if(pprotores == NULL || pprotores->setSequenceID(nSequenceID) != S_SUCCESS)
	{
		LOG_ERROR("echarge", "echarge protores CMD_USER_CHARGE_RES set error");
		delete pprotores;
		return;
	}

	pprotores->setRatingID(CLoginLogic::GetSingleton().GetRatingID());
	pprotores->setUserID(user_id);
	pprotores->setSubjectID(subject_id);
	pprotores->setAuditFlag(audit_flag);
	pprotores->setDetailID(detail_id);
	pprotores->setGameID(game_id);

	if (!CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_chargeamount, nfd, (int) pprotores, 
				user_id, SQL_NEED_CALLBACK, SELECT,
				"select FuncChargeAmount(%lld, %d, %d, %d, %d, %d), %d ",
				detail_id, user_id, subject_id, amount, audit_flag, charge_time, amount))
	{
		LOG_ERROR("echarge", "object pool create serversession error on function %s!", __FUNCTION__);
		delete pprotores;
	}

	LOG_INFO("echarge", "echarge proto ok CMD_USER_CHARGE(rating_id=%d, user_id=%d, "
				"subject_id=%d, audit_flag=%d, detail_id=%d,total_amount=%d,charge_time=%d sequence_id=%d", 
				rating_id, user_id, subject_id, audit_flag, detail_id, total_amount, charge_time, nSequenceID);

}

void CChargeServer::OnSessionChargeAmount(CMessage* tpMessage, int nfd , int nParam2)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	AGIPUserChargeRes* pprotores = (AGIPUserChargeRes*) nParam2;
	bool bNeedNotice = false;

	int nTotalAmount = 0;
	if ( tpSqlResult->resultcode() == 1 && tpSqlResult->rowcount() == 1 && tpSqlResult->colcount() == 2)
	{
		int ret = atoi(tpSqlResult->fieldvalue(0).c_str());
		switch (ret)
		{
			case -11:
			{
				LOG_ERROR("echarge", "insert gms_charge_log error for key dup");
				pprotores->setResultCode(E_CHARGE_DUPLICATE) ;
				break;
			}

			case -12:
			{
				LOG_FATAL("echarge", "select from gms_charge_log error for no sequence_id and subject_id");
				pprotores->setResultCode(E_ERROR);
				break;
			}

			case -13:
			{
				LOG_ERROR("echarge", "update ums_account error for no account_id");
				pprotores->setResultCode(E_ACCOUNT_NOT_FOUND) ;
				break;
			}

			default:
			{
				if (ret >= 0)
				{
					nTotalAmount = ret;
					pprotores->setTotalAmount(nTotalAmount);
					pprotores->setResultCode(S_SUCCESS);

					bNeedNotice = true;
				}else
				{
					LOG_ERROR("debug", "there is a db proc error on %s for errcode(%d)", __FUNCTION__, ret);
					pprotores->setResultCode(E_ERROR);
				}
			}	
		}
	}else
	{
		LOG_FATAL("echarge", "there is a fatal db fetch error on %s", __FUNCTION__);
		pprotores->setResultCode(E_SYS_DATABASE_ERROR) ;
	}

	uint64_t	detail_id = pprotores->getDetailID();
	uint32_t    user_id = pprotores->getUserID();
	int16_t     subject_id = pprotores->getSubjectID();
	int32_t     amount = atoi(tpSqlResult->fieldvalue(1).c_str());

	// 向erating发送充值回应
	if (Send2ECharge(nfd, pprotores))
	{
		// 向玩家发送充值成功通知
		if ( bNeedNotice )
		{
			CMessageUserChargeNotice tChargeNotice;
			tChargeNotice.set_userid(user_id);
			tChargeNotice.set_subjectid(subject_id);
			tChargeNotice.set_amount(amount);
			tChargeNotice.set_detailid(detail_id);

			CMessage tMessage;
			tMessage.set_msgpara((uint32_t) &tChargeNotice);
			pbmsg_sethead(&tMessage, ID_M2G_USER_CHARGE_NOTICE, 0, 0, 0, 0, 0, 0);

			CLoginLogic::GetSingleton().Send2Gate(&tMessage);

			LOG_INFO("echarge", "send proto CMD_USER_CHARGE_RES ok(user_id=%d, subject_id=%d, detail_id=%d,total_amount=%d", 
					user_id, subject_id, detail_id, amount );
		}
	}else
	{
		LOG_ERROR("echarge", "Send2ECharge AGIPUserChargeRes failure!");
	}

	delete pprotores;	
}

// erating发起充值对账请求响应
void CChargeServer::OnAGIPUserChargeAudit(int nfd, SysProtocol* pSysProto)
{
	LK_ASSERT(pSysProto != NULL, return);
	AGIPUserChargeAudit* pproto = (AGIPUserChargeAudit*) pSysProto;

	uint32_t 		nSequenceID;
	if(pproto->getSequenceID(&nSequenceID) != S_SUCCESS)
	{
		LOG_ERROR("echarge", "echarge proto error CMD_USER_CHARGE_RES getSequenceID Error");
		return;
	}

	uint32_t        user_id = pproto->getUserID();
	uint32_t        rating_id = pproto->getRatingID();
	int16_t         subject_id = pproto->getSubjectID();
	uint16_t        audit_count = pproto->getAuditCount();
	uint64_t        bgn_did = pproto->getBgnDetailID();
	uint64_t        end_did = pproto->getEndDetailID();
	uint32_t		game_id = pproto->getGameID();

	AGIPUserChargeAuditRes *pprotores = new AGIPUserChargeAuditRes;
	if( pprotores == NULL )
	{
		LOG_ERROR("echarge", "echarge new AGIPUserChargeAuditRes error");
		return;
	}

	pprotores->setRatingID(CLoginLogic::GetSingleton().GetRatingID());
	pprotores->setUserID(user_id) ;
	pprotores->setSubjectID(subject_id);
	pprotores->setSequenceID(nSequenceID);
	pprotores->setGameID(game_id);

	// 将对账回应存放到session中
	if (! CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_chargeaudit, nfd, (int) pprotores, 
				user_id, SQL_NEED_CALLBACK, SELECT,
				" select sequence_id, amount from GMS_CHARGE_LOG "
				" where sequence_id >= %lld and sequence_id < %lld and account_id = %d and subject_id = %d "
				" order by sequence_id desc limit %d, %d", 
				bgn_did, end_did, user_id, subject_id, 0, CONST_AUDIT_COUNT_LIMIT))
	{
		LOG_ERROR("echarge", "object pool create serversession error on function %s!", __FUNCTION__);
		delete pprotores;
	}

	LOG_INFO("echarge", "echarge proto ok CMD_USER_CHARGE_RES(rating_id=%d, user_id=%d, "
				"subject_id=%d, audit_count=%d, bgn_did=%lld, end_did=%lld", 
				rating_id, user_id, subject_id, audit_count, bgn_did, end_did);
}

// 从数据库返回结果集
void CChargeServer::OnSessionChargeAudit(CMessage* tpMessage, int nfd , int nParam2)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	AGIPUserChargeAuditRes* pprotores = (AGIPUserChargeAuditRes*) nParam2;

	//int nTotalAmount = 0;
	if ( tpSqlResult->resultcode() == 1) 
	{
		int colnum = tpSqlResult->colcount();
		int rownum = tpSqlResult->rowcount();

		for (int i = 0; i < rownum ; ++i)
		{
			pprotores->AddChargeInfoShort(atoll(tpSqlResult->fieldvalue(0 + i* colnum).c_str()), 
					atoi(tpSqlResult->fieldvalue(1 + i* colnum).c_str()));
		}

		pprotores->setResultCode(S_SUCCESS);
	}else
	{
		pprotores->setResultCode(E_SYS_DATABASE_ERROR);
	}

	if (Send2ECharge(nfd, pprotores))
	{
		LOG_INFO("echarge", "Send AGIPUserChargeAuditRes to Echarge successfully!");
	}else
	{
		LOG_ERROR("echarge", "Send AGIPUserChargeAuditRes to Echarge failure!");
	}

	delete pprotores;
}

void CChargeServer::OnAGIPCreateOrUpdateUser(int nfd, SysProtocol* pSysProto)
{
	LK_ASSERT(pSysProto != NULL, return);
	AGIPCreateOrUpdateUser* pproto = (AGIPCreateOrUpdateUser*) pSysProto;

	uint32_t        user_id;
	char            user_name[AGIP_USER_NAME_LEN+1] = {0};
	char            password[AGIP_PASSWORD_LEN+1] = {0};
	int8_t          user_type;
	int8_t          user_class;
	int32_t         user_point;
	uint32_t        promoter_id;
	uint32_t        user_flag;
	uint32_t 		nSequenceID;
	
	
	if(pproto->GetUserID(user_id) != S_SUCCESS
	|| pproto->GetUserName(user_name) != S_SUCCESS
	|| pproto->GetPassword(password) != S_SUCCESS
	|| pproto->GetUserType(user_type) != S_SUCCESS
	|| pproto->GetUserClass(user_class) != S_SUCCESS
	|| pproto->GetUserPoint(user_point) != S_SUCCESS
	|| pproto->GetPromoterID(promoter_id) != S_SUCCESS
	|| pproto->GetUserFlag(user_flag) != S_SUCCESS 
	|| pproto->getSequenceID(&nSequenceID) != S_SUCCESS )
	{
		LOG_ERROR("echarge", "echarge proto error CMD_CREATE_OR_UPDATE_USER"
				" user_id=%d username=%s password=%s user_type=%d user_class=%d "
				" user_point = %d promoter_id = %d user_flag = %d", 
				user_id, user_name, password, user_type, user_class, user_point, promoter_id, user_flag);
		return;
	}

	uint32_t game_id = pproto->getGameID();

	AGIPCreateOrUpdateUserRes* pprotores = new AGIPCreateOrUpdateUserRes;
	pprotores->setSequenceID(nSequenceID);
	pprotores->setGameID(game_id);

	if(! CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_createaccount, nfd, (int) pprotores,
				user_id, SQL_NEED_CALLBACK, SELECT, 
				"select FuncInsOrUpdAcct(%d, '%s', '%s', %d, %d, %d, %d, %d) ",
				user_id, user_name, password, user_type, user_class, user_point, promoter_id, user_flag))
	{
		//TODO: 返回失败
		LOG_ERROR("echarge", "object pool create serversession error on function %s!", __FUNCTION__);
	}
}

void CChargeServer::OnSessionCreateOrUpdateUser(CMessage* tpMessage, int nfd , int nParam2)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0 && nParam2 != 0, return );	
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	AGIPCreateOrUpdateUserRes* pprotores = (AGIPCreateOrUpdateUserRes* )nParam2;

	if ( tpSqlResult->resultcode() == 1) 
	{
		pprotores->SetResultCode(S_SUCCESS);
	}else
	{
		pprotores->SetResultCode(E_ERROR);
	}

	Send2ECharge(nfd, pprotores);

	delete pprotores;
}

// gameserver->loginserver 的玩家消费请求
void CChargeServer::OnMessageUserIbExchangeNotice(CMessage* tpMessage)
{
	/*
	LK_ASSERT(tpMessage != NULL, return);
	CMessageUserIbExchangeNotice* tpIbExchange = (CMessageUserIbExchangeNotice*) tpMessage->msgpara();
	LK_ASSERT(tpIbExchange != NULL, return);

	AGIPUserIBPay proto;
	proto.setDetailID    (tpIbExchange->purchaseid());
	proto.setUserID      (tpIbExchange->accountid());
	proto.setRoleID      (tpIbExchange->roleid());
	proto.setRoleGender  (tpIbExchange->genderid() == 3 ? 1 : 2);
	proto.setRoleOccupation(tpIbExchange->metierid());
	proto.setRoleLevel   (tpIbExchange->level());
	proto.setRatingID    (CLoginLogic::GetSingleton().GetRatingID());
	proto.setIBCode      (tpIbExchange->ibcode().c_str());
	proto.setPackageFlag (tpIbExchange->packageflag());
	proto.setIBCount     (tpIbExchange->count());
	proto.setPayTime     (tpIbExchange->paytime());
	proto.setUserIP      (tpIbExchange->userip());
	proto.setSubjectID   (tpIbExchange->subjectid());
	proto.setAuditFlag   (tpIbExchange->auditflag());
	proto.setPrice       (tpIbExchange->price());
	proto.setDiscountPrice(tpIbExchange->discountprice());

	CAGIPSession* pSession = CLoginLogic::GetSingleton().CreateSession4Proto(&proto);
	if (pSession == NULL)
	{
		m_bHasSomeRetryIbPay = true;
		LOG_ERROR("echarge", "CLoginLogic::GetSingleton().CreateSession4Proto Error");
		return;
	}

	uint32_t nIbTransID = tpIbExchange->purchaseid();
	pSession->assign(pSession->get_id(), em_agipsession_ibexchange1, (int) nIbTransID, 0,
			CLoginServer::GetSingleton().mConfig.nERatingHandsetInterval);
	*/

	m_bHasSomeRetryIbPay = true;
}

// 消费的erating回应
void CChargeServer::OnAGIPMsgUserIbPayRes(SysProtocol* pSysProto, int nParam1, int nParam2)
{
	LK_ASSERT(pSysProto != NULL, return);
	AGIPUserIBPayRes * pprotores = (AGIPUserIBPayRes *) pSysProto;
	
	int nResultCode = pprotores->getResultCode();
	uint64_t ullPurchaseID = pprotores->getDetailID();

	int nWorldID = 0;
	uint32_t nTransID = 0;

	// 取得本地的transid, 与worldId
	local_trans_id(ullPurchaseID, &nWorldID, &nTransID);

	//-1460 说明记录重复
	if (nResultCode != S_SUCCESS && nResultCode != -1460 && nResultCode != -1282) 
	{
		LOG_INFO("echarge", "purhase id(%lld) is failure, error code(%d)", ullPurchaseID, nResultCode);

		// TODO: 需要记录?失败的purchase_id
		UpdateIbExchangeNoticeError(nWorldID, nTransID);

		// 有需要重发的消息记录
		m_bHasSomeRetryIbPay = true;
		return;
	}

	uint32_t nAccountID = pprotores->getUserID();
	//uint32_t nRatingID = pprotores->getRatingID();
	uint16_t nSubjectID = pprotores->getSubjectID();
	uint16_t nAuditFlag = pprotores->getAuditFlag();
	int32_t  nTotalPay = pprotores->getTotalPayAmount();

	LOG_NOTICE("echarge", "AGIPUserIbPayRes account_id(%d), purchase_id(%lld), subject_id(%d), total_pay(%d) audit_flag(%d)",
					nAccountID, ullPurchaseID, nSubjectID, nTotalPay, nAuditFlag);

	switch(nAuditFlag)
	{
		case 1: // 需要erating返回玩家的消费总额, 并从LOCKED表中删除记录
		case 2: // ERATING 不返回消费总额, 但是需要从LOCKED表中移除记录
		{
			CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_ibpayres, nAccountID, nSubjectID,
					nAccountID, SQL_NEED_CALLBACK, SELECT, 
					"select FuncUserIbPayRes(%d, %u, %d, %d, %d, %d)", nWorldID, nTransID, nAccountID, nTotalPay, nSubjectID, nAuditFlag);

			break;
		}

		case 3:
		case 4:
		default:
		{
			LOG_ERROR("echarge", "charge module has no action for this auditflag(%d)", nAuditFlag);
			break;
		}
	}
}

// 返回玩家的消费总额, 如果总额不一致，则发起对账申请
void CChargeServer::OnSessionUserIbPayRes(CMessage* tpMessage, int nAccountID , int nSubjectID)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	//int colnum = tpSqlResult->colcount();
	
	int rownum = tpSqlResult->rowcount();
	int retcode = tpSqlResult->resultcode();

	if ( retcode == 0 || rownum == 0)
	{
		LOG_ERROR("echarge", "database execute FuncUserIbPayResIsOK error.");
		return;
	}

	int nRemain = atoi(tpSqlResult->fieldvalue(0).c_str());
	if ( nRemain >= 0)
	{
		LOG_INFO("echarge", "transaction is ok, erating's total_consumed_money is match with gamedb.");
		return;
	}

	switch( nRemain )
	{
		case -13:
		{
			// 玩家帐户不存在
			LOG_FATAL("echarge", "select account_id FROM UMS_ACCOUNT WHERE account_id = %d, error", nAccountID);
			break;
		}

		case -16:
		{
			// 帐户上已消费金额和erating返回的已消费金额不一致 
			LOG_ERROR("echarge", "account_id(%d) consumed (%s)  is not match with eratig.", 
					nAccountID, nSubjectID == 5 ? "golden_yb" : "silver_yb");

			break;
		}

		default:
		{
			LOG_ERROR("echarge", "there is no action for error(%d) on  gamedb.FuncUserIbPayRes()", retcode);
		}
	}
}

// 更新交易状态为失败状态
void CChargeServer::UpdateIbExchangeNoticeError(int nWorldID, uint32_t nIbTransID)
{
	CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_ibtranserror, nIbTransID, 0,
				0, SQL_NONE, UPDATE,
				" update GMS_CONSUME_LOCKED set trans_status = 3, failure_times = failure_times + 1, last_fail_time = %d "
				" where rating_id = %d and purchase_id = %u", time(0), nWorldID, nIbTransID);
}

void CChargeServer::OnTimerOfIbPayRetry()
{
	const int nStart = 0;
	const int nOnce  = 10;

	LOG_DEBUG("echarge", "m_bHasSomeRetryIbPay(%d), m_bIsRetrying(%d), m_bFirstLoad(%d)",
				m_bHasSomeRetryIbPay, m_bIsRetrying, m_bFirstLoad);

	//if Have some error record,then load from ibpay_retry
	if (m_bHasSomeRetryIbPay == true && CLoginServer::GetSingleton().IsConnected2ERating() == true
	&& CLoginLogic::GetSingleton().IsERatingBinded() == true)
	{
		// 如果正在进行中，则此次周期不生效
		if ( m_bIsRetrying == false )
		{
			m_bIsRetrying = true;

			LoadIbPayRetryFromDB(nStart, nOnce);
		}
	}
}

void CChargeServer::OnTimerOfIbUsedRetry()
{
	const int nStart = 0;
	const int nOnce  = 100;

	//if Have some error record,then load from ibpay_retry
	if (CLoginServer::GetSingleton().IsConnected2ERating() == true
	&& CLoginLogic::GetSingleton().IsERatingBinded() == true)
	{
		if ( m_bIsSendingIbItemUsed == false)
		{
			m_bIsSendingIbItemUsed = true;
			LoadIbItemUsedFromDB(nStart, nOnce);
		}
	}
}

void CChargeServer::LoadIbPayRetryFromDB(int nStart, int nCount)
{
	if (! CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_currentibpay, nStart, nCount, 
				0, SQL_NEED_CALLBACK, SELECT,
					" select purchase_id, account_id, role_id, gender_id, metier_id, role_level, "
					" rating_id, ib_code, package_flag, item_count, pay_time, user_ip, subject_id, " 
					" audit_flag, price, discount_price, last_fail_time, failure_times, game_id "
					" from GMS_CONSUME_LOCKED where failure_times < %d limit %d, %d" ,
					//" from GMS_CONSUME_LOCKED where trans_status %s and failure_times < %d limit %d, %d" ,
					//m_bFirstLoad ? "in (1, 3)" : " = 3",
					CLoginServer::GetSingleton().mConfig.nIbPayRetryFirstTimes +
					CLoginServer::GetSingleton().mConfig.nIbPayRetrySecondTimes, nStart, nCount))
	{
		//TODO: 返回失败
		LOG_ERROR("echarge", "object pool create serversession error on function %s!", __FUNCTION__);
		m_bIsRetrying = false;
	}
}

void CChargeServer::OnSessionLoadIbPayFromDB(CMessage* tpMessage, int nParam1 , int nParam2)
{
	LK_ASSERT(tpMessage != NULL, return);
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	int colnum = tpSqlResult->colcount();
	int rownum = tpSqlResult->rowcount();

	if ( tpSqlResult->resultcode() == 0 ) 
	{
		LOG_ERROR("echarge", "%s SQL RESPONSE ERROR", __FUNCTION__);
		m_bIsRetrying = false;
		return;
	}

	// 是否有记录
	m_bHasSomeRetryIbPay = (rownum != 0);
	
	int nNow = time(0);	
	int nFirstPeriod = CLoginServer::GetSingleton().mConfig.nIbPayRetryFirstTimes;
	int nSecondPeriod = nFirstPeriod + CLoginServer::GetSingleton().mConfig.nIbPayRetrySecondTimes;
			
	for ( int j = 0; j < rownum; ++j)   
	{
		// 最后失败的时间
		int nLastFailTime = atoi(tpSqlResult->fieldvalue(16 + j* colnum).c_str());

		// 已经重试的次数
		int nRetryTimes = atoi(tpSqlResult->fieldvalue(17 + j* colnum).c_str());

		// 如果已经大于了重发的次数限制
		if ( nRetryTimes > nSecondPeriod)
		{
			continue;
		} else if ( nRetryTimes > nFirstPeriod ) //如果是第二阶段
		{
			// 判断上次重发的时间和此次的时间间隔是否超过了第二阶段设定的值
			if ( nNow - nLastFailTime < CLoginServer::GetSingleton().mConfig.nIbPayRetrySecondInterval)  
			{
				continue;
			}
		} else 
		{
			// 判断上次重发的时间和此次的时间间隔是否超过了第一阶段设定的值
			if ( nNow - nLastFailTime < CLoginServer::GetSingleton().mConfig.nIbPayRetryFirstInterval)  
			{
				continue;
			}
		}

		// 重发, audit_flag = 2 的消费记录
		AGIPUserIBPay proto;
		const int const_nAudit = 1;

		uint32_t nPurchaseID = (uint32_t) atoi(tpSqlResult->fieldvalue(0 + j* colnum).c_str());
		int nRatingId = atoi(tpSqlResult->fieldvalue(6 + j* colnum).c_str());

		proto.setDetailID (world_trans_id( nRatingId, nPurchaseID));
		proto.setUserID      (atoi(tpSqlResult->fieldvalue(1 + j* colnum).c_str()));
		proto.setRoleID      (atoi(tpSqlResult->fieldvalue(2 + j* colnum).c_str()));
		proto.setRoleGender  (atoi(tpSqlResult->fieldvalue(3 + j* colnum).c_str()) == 3 ? 1 : 2);
		proto.setRoleOccupation(atoi(tpSqlResult->fieldvalue(4 + j* colnum).c_str()));
		proto.setRoleLevel   (atoi(tpSqlResult->fieldvalue(5 + j* colnum).c_str()));
		proto.setRatingID    (CLoginLogic::GetSingleton().GetRatingID());
		proto.setIBCode      (tpSqlResult->fieldvalue(7 + j* colnum).c_str());
		proto.setPackageFlag (atoi(tpSqlResult->fieldvalue(8 + j* colnum).c_str()));
		proto.setIBCount     (atoi(tpSqlResult->fieldvalue(9 + j* colnum).c_str()));
		proto.setPayTime     (atoi(tpSqlResult->fieldvalue(10 + j* colnum).c_str()));
		proto.setUserIP      (htonl(atoi(tpSqlResult->fieldvalue(11 + j* colnum).c_str())));
		proto.setSubjectID   (atoi(tpSqlResult->fieldvalue(12 + j* colnum).c_str()));
		proto.setAuditFlag   (const_nAudit);
		proto.setPrice       (atoi(tpSqlResult->fieldvalue(14 + j* colnum).c_str()));
		proto.setDiscountPrice(atoi(tpSqlResult->fieldvalue(15 + j* colnum).c_str()));

		proto.setGameID(atoi(tpSqlResult->fieldvalue(18 + j* colnum).c_str()));
		CAGIPSession* pSession = CLoginLogic::GetSingleton().CreateSession4Proto(&proto);
		if (pSession == NULL)
		{
			LOG_ERROR("echarge", "CLoginLogic::GetSingleton().CreateSession4Proto Error");
			continue;
		}

		pSession->assign(pSession->get_id(), em_agipsession_ibexchange2, (int) nPurchaseID, 0,
				CLoginServer::GetSingleton().mConfig.nERatingHandsetInterval);
	}

	// 如果还有记录则继续
	if (rownum == nParam2 && rownum != 0)
	{
		LoadIbPayRetryFromDB(nParam1+nParam2, nParam2);
	}else
	{
		m_bFirstLoad = false;
		m_bIsRetrying = false;
	}
}

void CChargeServer::LoadIbItemUsedFromDB(int nStart, int nCount)
{
	//if (! CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_loadibused, nStart, nCount, 
	//			0, SQL_NEED_CALLBACK, SELECT,
	//				" select account_id, role_id, purchase_id, gender_id, metier_id, role_level, "
	//				" last_fail_time, failure_times, game_id "
	//				" from GMS_IBITEM_USED_LOCKED where failure_times < %d limit %d, %d" ,
	//				CLoginServer::GetSingleton().mConfig.nIbUsedRetryFirstTimes +
	//				CLoginServer::GetSingleton().mConfig.nIbUsedRetrySecondTimes, nStart, nCount))
	//{
	//	//TODO: 返回失败
	//	LOG_ERROR("echarge", "object pool create serversession error on function %s!", __FUNCTION__);
	//	m_bIsSendingIbItemUsed = false;
	//}
}

void CChargeServer::OnSessionLoadIbItemUsedFromDB(CMessage* tpMessage, int nParam1 , int nParam2)
{
	LK_ASSERT(tpMessage != NULL, return);
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	int colnum = tpSqlResult->colcount();
	int rownum = tpSqlResult->rowcount();

	if ( tpSqlResult->resultcode() == 0 ) 
	{
		LOG_ERROR("echarge", "%s SQL RESPONSE ERROR", __FUNCTION__);
		m_bIsSendingIbItemUsed = false;
		return;
	}

	int nNow = time(0);	
	int nFirstPeriod = CLoginServer::GetSingleton().mConfig.nIbUsedRetryFirstTimes;
	int nSecondPeriod = nFirstPeriod + CLoginServer::GetSingleton().mConfig.nIbUsedRetrySecondTimes;

	for ( int i = 0; i < rownum; ++i)   
	{
		// 最后失败的时间
		int nLastFailTime = atoi(tpSqlResult->fieldvalue(6 + i* colnum).c_str());

		// 已经重试的次数
		int nRetryTimes = atoi(tpSqlResult->fieldvalue(7 + i* colnum).c_str());

		// 如果已经大于了重发的次数限制
		if ( nRetryTimes > nSecondPeriod)
		{
			continue;
		} else if ( nRetryTimes > nFirstPeriod ) //如果是第二阶段
		{
			// 判断上次重发的时间和此次的时间间隔是否超过了第二阶段设定的值
			if ( nNow - nLastFailTime < CLoginServer::GetSingleton().mConfig.nIbUsedRetrySecondInterval)  
			{
				continue;
			}
		} else 
		{
			// 判断上次重发的时间和此次的时间间隔是否超过了第一阶段设定的值
			if ( nNow - nLastFailTime < CLoginServer::GetSingleton().mConfig.nIbUsedRetryFirstInterval)  
			{
				continue;
			}
		}

		AGIPUserUseProduct proto;
		uint64_t ullPurchaseID = atoll( tpSqlResult->fieldvalue(2 + i* colnum).c_str());


		if(proto.setUserID       (atoi(tpSqlResult->fieldvalue(0 + i* colnum).c_str())) != S_SUCCESS
		|| proto.setRoleID       (atoi(tpSqlResult->fieldvalue(1 + i* colnum).c_str())) != S_SUCCESS
		|| proto.setPurchaseID   (ullPurchaseID) != S_SUCCESS
		|| proto.setRoleGender   (atoi(tpSqlResult->fieldvalue(3 + i* colnum).c_str()) == 3 ? 1 : 2) != S_SUCCESS
		|| proto.setRoleOccupation(atoi(tpSqlResult->fieldvalue(4 + i* colnum).c_str())) != S_SUCCESS
		|| proto.setRoleLevel    (atoi(tpSqlResult->fieldvalue(5 + i* colnum).c_str())) != S_SUCCESS)
		{
			LOG_ERROR("echarge", "echarge protores CMD_USER_USE_PRODUCT set error");
			continue;
		}

		proto.setGameID(atoi(tpSqlResult->fieldvalue(8 + i* colnum).c_str()));
		CAGIPSession* pSession = CLoginLogic::GetSingleton().CreateSession4Proto(&proto);
		if (pSession == NULL)
		{
			LOG_ERROR("echarge", "CLoginLogic::GetSingleton().CreateSession4Proto Error");
			continue;
		}


		int nWorldID = 0; 
		uint32_t nTransID = 0;
		local_trans_id( ullPurchaseID, &nWorldID, &nTransID);

		pSession->assign(pSession->get_id(), em_agipsession_ibitemused, nWorldID, (int) nTransID,
				CLoginServer::GetSingleton().mConfig.nERatingHandsetInterval);
	}

	// 如果还有记录则继续
	if (rownum == nParam2 && rownum != 0)
	{
		LoadIbItemUsedFromDB(nParam1+nParam2, nParam2);
	}else
	{
		m_bIsSendingIbItemUsed = false;
	}
}

void CChargeServer::OnMessageUserUseIbItemNotice(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	CMessageUserUseIbItemNotice* tpUsedIbItem = (CMessageUserUseIbItemNotice*) tpMessage->msgpara();
	LK_ASSERT(tpUsedIbItem != NULL, return);

/*
	AGIPUserUseProduct proto;

	if(proto.setUserID       (tpUsedIbItem->accountid()) != S_SUCCESS
	|| proto.setRoleID       (tpUsedIbItem->roleid()) != S_SUCCESS
	|| proto.setPurchaseID   (tpUsedIbItem->purchaseid()) != S_SUCCESS
	|| proto.setRoleGender   (tpUsedIbItem->genderid() == 3 ? 1 : 2) != S_SUCCESS
	|| proto.setRoleOccupation(tpUsedIbItem->metierid()) != S_SUCCESS
	|| proto.setRoleLevel    (tpUsedIbItem->level()) != S_SUCCESS)
	{
		LOG_ERROR("echarge", "echarge protores CMD_USER_USE_PRODUCT set error");
		return;
	}

	CAGIPSession* pSession = CLoginLogic::GetSingleton().CreateSession4Proto(&proto);
	if (pSession == NULL)
	{
		LOG_ERROR("echarge", "CLoginLogic::GetSingleton().CreateSession4Proto Error");
		return;
	}

	pSession->assign(pSession->get_id(), em_agipsession_ibitemused, 0, 0,
			CLoginServer::GetSingleton().mConfig.nERatingHandsetInterval);
*/
}

void CChargeServer::OnAGIPMsgUserUseProductRes(SysProtocol* pSysProto, int nParam1, int nParam2)
{
	LK_ASSERT(pSysProto != NULL, return);
	AGIPUserUseProductRes * pprotores = (AGIPUserUseProductRes *) pSysProto;
	
	int nWorldID = nParam1;
	uint32_t nTransID = (uint32_t) nParam2;

	int32_t nResultCode;
	// 返回错误时不再重发
	/*if (pprotores->getResultCode(&nResultCode) != S_SUCCESS) 
	{
		CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_null, 0, 0, 
				0, SQL_NONE, UPDATE, 
				" update GMS_IBITEM_USED_LOCKED "
				" set last_fail_time = %d,  failure_times = failure_times + 1 "
				" where rating_id = %d and purchase_id = %u ", time(0), nWorldID, nTransID);

		LOG_ERROR("echarge", "%s ERROR", __FUNCTION__);
		return;
	}*/

	//将使用记录从LOCKED表移动至LOG表
	CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_null, 0, 0, 0, SQL_NONE, DELETE, 
				"call ProcUseIbItem(%lld) ", world_trans_id(nWorldID, nTransID));
	

	LOG_DEBUG("echarge", "%s OK", __FUNCTION__);
}

void  CChargeServer::OnAGIPPayListQuery(int nfd, SysProtocol* pSysProto)
{
	LK_ASSERT(pSysProto != NULL, return);
	AGIPPayListQuery* pproto = (AGIPPayListQuery*) pSysProto;
	
	uint32_t	game_id = pproto->getGameID();
	//uint32_t	nGatewayID = pproto->getGatewayID();
	int16_t		nSubjectID = pproto->getSubjectID();
	uint16_t	nDetailCount = pproto->getDetailCount();
	uint64_t	nBeginDetailID = pproto->getBeginDetailID();
	int32_t		nDetailType = pproto->getDetailType();

	uint32_t  	nSequenceID = 0; 
	pproto->getSequenceID(&nSequenceID);

	AGIPPayListQueryRes* pprotores = new AGIPPayListQueryRes;
	pprotores->setGameID(game_id);
	pprotores->setSubjectID(nSubjectID);
	pprotores->setSequenceID(nSequenceID);

	if ( nDetailType == em_query_consume )
	{
		if(! CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_paylistquery, nfd, (int) pprotores,
					0, SQL_NEED_CALLBACK, SELECT, 
					"SELECT detail_id, discount_price * item_count FROM GMS_CONSUME_LOG "
					"WHERE detail_id >= %lld AND subject_id = %d and game_id = %d "
					"ORDER BY detail_id LIMIT %d ",
					nBeginDetailID, nSubjectID, game_id, nDetailCount ))
		{
			//TODO: 返回失败
			LOG_ERROR("echarge", "object pool create serversession error on function %s!", __FUNCTION__);
		}
	}else if ( nDetailType == em_query_stock )
	{
		// TODO:
		// 发列表
		uint32_t tDetailID = 0;
		int tWorld = 0;
		local_trans_id( nBeginDetailID, &tWorld, &tDetailID );
		if(! CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_paylistquery, nfd, (int) pprotores,
					0, SQL_NEED_CALLBACK, SELECT, 
					"SELECT rating_id * 10000000000 + bourse_detail, yb_count FROM UMS_BOURSE_LOG "
					"WHERE bourse_detail >= %d and sell_account != buy_account and rating_id = %d "
					" ORDER BY bourse_detail LIMIT 0, %d ",  tDetailID, tWorld, nDetailCount ))
		{
			//TODO: 返回失败
			LOG_ERROR("echarge", "object pool create serversession error on function %s!", __FUNCTION__);
		}

	}
}

void CChargeServer::OnSessionPayListQuery(CMessage* tpMessage, int nParam1, int nParam2)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	int nfd = nParam1;
	AGIPPayListQueryRes* pprotores = (AGIPPayListQueryRes*) nParam2;
	int16_t  nSubjectID = pprotores->getSubjectID();

	// 如果执行失败
	if ( tpSqlResult == 0)
	{
		pprotores->setResultCode(E_SYS_DATABASE_ERROR);
	}else
	{
		int rownum = tpSqlResult->rowcount();
		int colnum = tpSqlResult->colcount();
		int nTotalAmount = 0;
		for ( int i = 0; i < rownum; ++i)
		{
			pprotores->addDetailID(atoll(tpSqlResult->fieldvalue(0 + i* colnum).c_str()));
			nTotalAmount += atoi(tpSqlResult->fieldvalue(1 + i* colnum).c_str());
		}

		pprotores->setGatewayID(CLoginLogic::GetSingleton().GetRatingID());
		pprotores->setSubjectID(nSubjectID);
		pprotores->setResultCode(S_SUCCESS);
		pprotores->setTotalAmount(nTotalAmount);
	}

	Send2ECharge(nfd, pprotores);
	delete pprotores;
}

void  CChargeServer::OnAGIPPayDetailDemand(int nfd, SysProtocol* pSysProto)
{
	LK_ASSERT(pSysProto != NULL, return);
	AGIPPayDetailDemand* pproto = (AGIPPayDetailDemand*) pSysProto;
	
	uint32_t nGameID = pproto->getGameID();
	//uint32_t	nGatewayID = pproto->getGatewayID();
	uint16_t	nDetailCount = pproto->getDetailCount();
	int32_t     nDetailType = pproto->getDetailType();

	uint32_t  	nSequenceID = 0; 
	pproto->getSequenceID(&nSequenceID);

	if ( nDetailType == em_query_consume )
	{
		// 如果申请的数量是0, 直接返回成功
		if ( nDetailCount == 0)
		{
			AGIPPayDetailDemandRes protores;
			protores.setResultCode(S_SUCCESS);

			Send2ECharge(nfd, &protores);
			return;
		}

		// 拼SQL语句
		lk::string<2048> stmtin("");
		for ( int i = 0; i < nDetailCount; ++i)
		{
			uint64_t nDetailID;
			if(pproto->getDetailID(i, nDetailID) != S_SUCCESS) continue;

			if(i == 0) 
			{
				stmtin.append("%lld",  nDetailID);
			}else
			{
				stmtin.append(",%lld", nDetailID);
			}
		}

		// 如果长度为0
		if ( stmtin.length() == 0)
		{
			AGIPPayDetailDemandRes protores;
			protores.setResultCode(E_ERROR);

			Send2ECharge(nfd, &protores);
			return;
		}

		AGIPPayDetailDemandRes* pprotores = new AGIPPayDetailDemandRes ;
		pprotores->setGameID(nGameID);
		pprotores->setSequenceID(nSequenceID);

		// 去数据库查询记录
		if(! CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_paydetaildemand, nfd, (int) pprotores,
					0, SQL_NEED_CALLBACK, SELECT, 
					" select purchase_id, account_id, role_id, gender_id, metier_id, role_level, "
					" rating_id, ib_code, package_flag, item_count, pay_time, user_ip, subject_id, " 
					" audit_flag, price, discount_price from GMS_CONSUME_LOG "
					" where detail_id in ( %s )", stmtin.c_str()))
		{
			LOG_ERROR("echarge", "object pool create serversession error on function %s!", __FUNCTION__);

			pprotores->setResultCode(E_ERROR);

			Send2ECharge(nfd, pprotores);
			delete pprotores;

			return;
		}
	}else if ( nDetailType == em_query_stock)
	{
		//TODO:	
		// 如果申请的数量是0, 直接返回成功
		if ( nDetailCount == 0)
		{
			AGIPPayDetailDemandRes protores;
			protores.setResultCode(S_SUCCESS);

			Send2ECharge(nfd, &protores);
			return;
		}

		// 拼SQL语句
		lk::string<2048> stmtin("");
		for ( int i = 0; i < nDetailCount; ++i)
		{
			uint64_t nDetailID;
			if(pproto->getDetailID(i, nDetailID) != S_SUCCESS) continue;

			int tWorldID = 0;
			uint32_t tDetailList = 0;

			local_trans_id( nDetailID, &tWorldID, &tDetailList );	

			if(i == 0) 
			{
				stmtin.append("%d",  tDetailList);
			}else
			{
				stmtin.append(",%d", tDetailList);
			}
		}

		// 如果长度为0
		if ( stmtin.length() == 0)
		{
			AGIPPayDetailDemandRes protores;
			protores.setResultCode(E_ERROR);

			Send2ECharge(nfd, &protores);
			return;
		}

	    if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_loadbourselog, 2, nfd, 0, SQL_NEED_CALLBACK, SELECT,
					" select bourse_detail, buy_account, sell_account, yb_count, rating_id, bourse_date, "
					" buy_role, sell_role, type, buy_account_game_id, sell_account_game_id from UMS_BOURSE_LOG where"
					"  buy_account != sell_account and bourse_detail in ( %s ) ", stmtin.c_str() 
			) )
   		{
			//TODO: ·μ??ê§°ü
        	LOG_ERROR("echarge", "object pool create serversession error on function %s!", __FUNCTION__);
			return ;
		}

        AGIPPayDetailDemandRes pprotores;
        pprotores.setGameID(nGameID);
        pprotores.setSequenceID(nSequenceID);

        pprotores.setResultCode(1);

        Send2ECharge(nfd, &pprotores);
	}
}

void CChargeServer::OnSessionPayDetailDemand(CMessage* tpMessage, int nfd, int pMsg)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	AGIPPayDetailDemandRes* pprotores = (AGIPPayDetailDemandRes*) pMsg;
	// 如果执行失败
	if ( tpSqlResult == 0)
	{
		pprotores->setResultCode(E_SYS_DATABASE_ERROR);
		Send2ECharge(nfd, pprotores);

		delete pprotores;
		return;
	}

	// 发送执行成功
	pprotores->setResultCode(S_SUCCESS);
	if(Send2ECharge(nfd, pprotores) != true)
	{
		LOG_ERROR("echarge", "Send AGIPPayDetailDemandRes failure");
		delete pprotores;

		return;
	}

	// 取得记录条数
	int rownum = tpSqlResult->rowcount();
	int colnum = tpSqlResult->colcount();

	for ( int j = 0; j < rownum; ++j)
	{
		AGIPUserIBPay proto;
		const int const_nAudit = 2;

		uint32_t nPurchaseID = (uint32_t) atoi(tpSqlResult->fieldvalue(0 + j* colnum).c_str());
		int nRatingId = atoi(tpSqlResult->fieldvalue(6 + j* colnum).c_str());

		proto.setDetailID (world_trans_id( nRatingId, nPurchaseID));
		proto.setUserID      (atoi(tpSqlResult->fieldvalue(1 + j* colnum).c_str()));
		proto.setRoleID      (atoi(tpSqlResult->fieldvalue(2 + j* colnum).c_str()));
		proto.setRoleGender  (atoi(tpSqlResult->fieldvalue(3 + j* colnum).c_str()) == 3 ? 1 : 2);
		proto.setRoleOccupation(atoi(tpSqlResult->fieldvalue(4 + j* colnum).c_str()));
		proto.setRoleLevel   (atoi(tpSqlResult->fieldvalue(5 + j* colnum).c_str()));
		proto.setRatingID    (CLoginLogic::GetSingleton().GetRatingID());
		proto.setIBCode      (tpSqlResult->fieldvalue(7 + j* colnum).c_str());
		proto.setPackageFlag (atoi(tpSqlResult->fieldvalue(8 + j* colnum).c_str()));
		proto.setIBCount     (atoi(tpSqlResult->fieldvalue(9 + j* colnum).c_str()));
		proto.setPayTime     (atoi(tpSqlResult->fieldvalue(10 + j* colnum).c_str()));
		proto.setUserIP      (htonl(atoi(tpSqlResult->fieldvalue(11 + j* colnum).c_str())));
		proto.setSubjectID   (atoi(tpSqlResult->fieldvalue(12 + j* colnum).c_str()));
		proto.setAuditFlag   (const_nAudit);
		proto.setPrice       (atoi(tpSqlResult->fieldvalue(14 + j* colnum).c_str()));
		proto.setDiscountPrice(atoi(tpSqlResult->fieldvalue(15 + j* colnum).c_str()));

		proto.setGameID(pprotores->getGameID());
		CAGIPSession* pSession = CreateSession4Proto(&proto, nfd);
		if (pSession == NULL)
		{
			LOG_ERROR("echarge", "CreateSession4Proto AGIPUserIBPay Error or Send AGIPUserIBPay error.");
			break;
		}else
		{
			LOG_DEBUG("echarge", "Send2Echarge AGIPUserIBPay %d%010d ", nRatingId, nPurchaseID);
		}
	}

	delete pprotores;
}

CAGIPSession* CChargeServer::CreateSession4Proto(SysProtocol* pproto, int nfd)
{
	// 创建本地Session, 保留请求的消息指针
	CAGIPSession* pSession= (CAGIPSession*) CLoginObjManager::GetSingletonPtr()->CreateObject(OBJTYPE_AGIPSESSION);
	if (pSession == NULL)
	{
		LOG_ERROR("default", "alloc an AGIPSession failure from objects pool.");
		return NULL;
	}

	int session_id = pSession->get_id();
	pproto->setSequenceID(session_id);

#ifdef _ECHARGE_DEBUG_
	unsigned int nCommandID = 0;
	pproto->getCommandID(&nCommandID);
	if ( nCommandID != CMD_HANDSET) 
	{
		printf("send to echarge ... {\n");
		pproto->showInfo();
		printf("}\n");
	}
#endif

	if (CChargeServer::GetSingleton().Send2ECharge(nfd, pproto) != true)
	{
		LOG_ERROR("echarge", "Send2ECharge Error");
		CLoginObjManager::GetSingletonPtr()->DestroyObject(pSession->get_id());

		pSession = NULL;
	}

	return pSession;
}

int CChargeServer::LoadBourseLog( )
{
    if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_loadbourselog, 1, 0, 0, SQL_NEED_CALLBACK, SELECT,
					" select bourse_detail, buy_account, sell_account, yb_count, rating_id, bourse_date, buy_role," 
					" sell_role, type, buy_account_game_id, sell_account_game_id from UMS_BOURSE_LOG_LOCKED where"
					" failure_times < %d and buy_account != sell_account limit 0, 10", 
					CLoginServer::GetSingleton().mConfig.nIbPayRetryFirstTimes +		
					CLoginServer::GetSingleton().mConfig.nIbPayRetrySecondTimes
		) )
    {
        //TODO: ·μ??ê§°ü
        LOG_ERROR("echarge", "object pool create serversession error on function %s!", __FUNCTION__);
    }

	return SUCCESS;
}

int CChargeServer::OnSessionLoadBourseLog(CMessage* tpMessage, int nParam1 , int nParam2)
{
	LK_ASSERT(tpMessage != NULL, return ERR_UNKNOWN );
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	int colnum = tpSqlResult->colcount();
	int rownum = tpSqlResult->rowcount();

	if ( tpSqlResult->resultcode() == 0 ) 
	{
		LOG_ERROR("echarge", "%s SQL RESPONSE ERROR", __FUNCTION__);
		m_bIsRetrying = false;
		return ERR_UNKNOWN;
	}
	// 是否有记录
	m_bHasSomeRetryIbPay = (rownum != 0);
	
//	int nNow = time(0);	
			
	for ( int j = 0; j < rownum; ++j)   
	{
		AGIPUserCashExchangeEx1 proto;

		uint32_t nEratingID = atoi(tpSqlResult->fieldvalue(4 + j* colnum).c_str());
		uint32_t nDetailID = atoi(tpSqlResult->fieldvalue(0 + j* colnum).c_str());
		
		proto.setDetailID( world_trans_id( nEratingID, nDetailID ) );
		proto.setDstUserID( atoi(tpSqlResult->fieldvalue(1 + j* colnum).c_str( ) ) );
		proto.setSrcUserID( atoi(tpSqlResult->fieldvalue(2 + j* colnum).c_str( ) ) );
		proto.setSubjectID( 5 );
		proto.setAmount( atoi(tpSqlResult->fieldvalue(3 + j* colnum).c_str( ) ) );
		proto.setFlag( 1 );
		proto.setExchangeTime( atoi(tpSqlResult->fieldvalue(5 + j* colnum).c_str( ) ) );
		proto.setSrcRoleID( atoi(tpSqlResult->fieldvalue(7 + j* colnum).c_str( ) ) );
		proto.setDstRoleID( atoi(tpSqlResult->fieldvalue(6 + j* colnum).c_str( ) ) );
		proto.setType(atoi(tpSqlResult->fieldvalue(8 + j*colnum).c_str()));

		proto.setSrcGameID( atoi( tpSqlResult->fieldvalue( 10 + j*colnum ).c_str( ) ) );
		proto.setDstGameID( atoi( tpSqlResult->fieldvalue( 9 + j*colnum ).c_str( ) ) );

		if( nParam1 == 1 )
		{
	        CAGIPSession* pSession = CLoginLogic::GetSingleton().CreateSession4Proto(&proto);
    	    if (pSession == NULL)
			{
            	LOG_ERROR("echarge", "[ %s : %d ][ %s ] Cannt Get Sesson Error", __LK_FILE__, __LINE__, __FUNCTION__ );
	            continue;
    	    }

			pSession->assign( pSession->get_id(), em_agipsession_exchange, nEratingID, nDetailID, 
				CLoginServer::GetSingleton().mConfig.nERatingHandsetInterval );
		}
		else
		{
			Send2ECharge(nParam2, &proto);
		}
//        pSession->assign(pSession->get_id(), em_agipsession_ibexchange2, (int) 
	}

	return SUCCESS;

}

int CChargeServer::OnAGIPMSGExchange( SysProtocol* pSysProto, int nParam1, int nParam2 )
{
	LK_ASSERT(pSysProto != NULL, return ERR_UNKNOWN );
	AGIPUserCashExchangeEx1Res * pprotores = (AGIPUserCashExchangeEx1Res *) pSysProto;
	
	int32_t nResultCode = pprotores->getResultCode( );

	if (( nResultCode != S_SUCCESS ) && ( nResultCode != -1282))	
	{
		UpdateExchangeError( nParam1, nParam2 );	
		LOG_ERROR( "echarge", "exchange %d, %d Erating Error, ResultCode %d", nParam1, nParam2, nResultCode );
		return ERR_UNKNOWN;
	}

	//将使用记录从LOCKED表移动至LOG表
	CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_null, 0, 0, 0, SQL_NONE, DELETE, 
		"delete from UMS_BOURSE_LOG_LOCKED where bourse_detail = %d and rating_id = %d ", nParam2, nParam1 );
	

	LOG_NOTICE( "echarge", "exchange %d, %d OK", nParam1, nParam2 );

	return SUCCESS;
}

int CChargeServer::UpdateExchangeError( int nParame1, int nParame2 )
{
	CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_null, 0, 0, 0, SQL_NONE, UPDATE,  
   		" update  UMS_BOURSE_LOG_LOCKED set failure_times = failure_times + 1 where bourse_detail = %d and"
		" rating_id = %d ", nParame2, nParame1 );

	LOG_DEBUG( "echarge", "exchange %d, %d failure_times add", nParame1, nParame2 );

	return SUCCESS;
}



