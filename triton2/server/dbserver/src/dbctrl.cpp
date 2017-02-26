#include "dbctrl.h"
#include "shm.h"
#include "tinyxml.h"
#include "proxymessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "oi_tea.h"
#include "performance.h"
#include "base.h"

CSharedMem* CDBCtrl::mShmPtr = NULL;
template<> CDBCtrl* CSingleton< CDBCtrl >::spSingleton = NULL;

#ifdef _POSIX_MT_
pthread_mutex_t CDBCtrl::stMutex;
#endif

STServerStatisticInfo g_stServerStatisticForPrint;

void* CDBCtrl::operator new( size_t nSize )
{
    if(!mShmPtr)
    {
        return (void *)NULL;
    }

    BYTE* pTemp = (BYTE *)mShmPtr->CreateSegment( nSize );

    return (void *)pTemp;
}
void CDBCtrl::operator delete( void *pMem )
{
}

int CDBCtrl::CountSize()
{
    int iTemp = 0;

    iTemp += sizeof(CDBCtrl);
    LOG_INFO( "dbctrl", "CDBCtrl %d", sizeof(CDBCtrl) );

    iTemp += MAXHANDLENUMBER*CDBHandle::CountSize();
    LOG_INFO( "dbctrl", "MAXHANDLENUMBER*CDBHandle::CountSize %d", MAXHANDLENUMBER*sizeof(CDBHandle::CountSize()) );

	LOG_INFO ( "dbctrl", "Total Size is %d", iTemp );

    return iTemp;
}

void CDBCtrl::SetRunFlag( int iFlag )
{
	m_iRunFlag |= iFlag;
	LOG_INFO( "dbctrl", "Set Run Flag %d, All Flag Is %d", iFlag, m_iRunFlag );
}

void CDBCtrl::ClearRunFlag( int iFlag )
{
	m_iRunFlag &= ~iFlag;
	LOG_INFO( "dbctrl", "Clear Run Flag %d", iFlag );
}

bool CDBCtrl::IsRunFlagSet( int iFlag )
{
	return ( m_iRunFlag & iFlag ) == iFlag ;
}

CDBCtrl::CDBCtrl()
{
    int i;

    if(!mShmPtr)
    {
        return;
    }

    CDBHandle::ms_pCurrentShm = mShmPtr;

    for( i = 0; i < MAXHANDLENUMBER; i++ )
    {
        m_apHandles[i] = new CDBHandle;
    }

#ifdef _ASYNC_SQL_
    for( i = 0; i < MAXHANDLENUMBER; i++ )
    {
        m_apExecers[i] = new CASqlExe;
    }
#endif

    m_sCheckPointHandle = 0;
    memset((void *)&m_stDBSvrdCfg, 0, sizeof(m_stDBSvrdCfg));

    time(&m_tLastPrintTime);

	m_iRunFlag = 0;
}

CDBCtrl::~CDBCtrl()
{
}

int CDBCtrl::Initialize( )
{

#ifdef _POSIX_MT_
	pthread_mutex_init( &stMutex, NULL);
#endif

	if ( ReadDBIpCfg( "../config/ip.xml" ) < 0 )
	{
		exit( 1 );
	}

	if( ReadDBSvrdCfg( "../config/data/dbserver.xml" ) < 0 )
	{
		exit(1);
	}
	
	//INIT_ROLLINGFILE_LOG( "sql", "../log/sql_error.log", LEVEL_DEBUG );
	return 0;
}
//
//bool DecryptAscIICode(const char* szInput, int nInput, char* szOutput, int* pnOutput)
//{
//	// 解密
//	static int tKey[ 4 ] = {4356,78534,46677,3545};
//	static unsigned char* tpKey = (unsigned char*)&tKey[0];
//
//	int nBinaryLen = 0;
//	unsigned char sBinaryCode[255+1] = {0};
//
//	size_t nAscIILen = strlen(szInput);
//	for (int i = 0; i+2 <= nAscIILen; i=i+2, ++nBinaryLen)
//	{
//		if( isxdigit(szInput[i]) && isxdigit(szInput[i+1]) )
//		{
//			#define CHARTOHEX( c ) (isdigit(( c )) ? (( c ) - '0') : (tolower(( c )) - 'a' + 10))
//			sBinaryCode[nBinaryLen] = CHARTOHEX(szInput[i]) * 16 + CHARTOHEX( szInput[i+1] );
//		}else
//		{
//			sBinaryCode[nBinaryLen] = 0;
//		}
//	}
//
//	int nResult = oi_symmetry_decrypt( sBinaryCode, nBinaryLen, tpKey, (unsigned char*) szOutput, pnOutput);
//	if ( nResult == FALSE )
//	{
//		return  false;
//	}
//
//	szOutput[*pnOutput] = 0;
//	return true;
//}

int CDBCtrl::ReadDBSvrdCfg(const char *szCfgFile)
{
	LOG_NOTICE( "dbctrl", "Read Server Config Begin..." );

	TiXmlDocument tDoc;

	if( !tDoc.LoadFile( szCfgFile ) )
	{
		LOG_ERROR( "dbctrl", "load xml %s failed", szCfgFile );
		return -1;
	}

	TiXmlElement* tpElement = NULL;
	TiXmlElement* tpChildElm = NULL;

	// 先得到根节点
	TiXmlElement* tpRoot = tDoc.FirstChildElement();

	// Log
	tpElement = tpRoot->FirstChildElement( "LOG" );
	if( tpElement != NULL )
	{
		for( tpChildElm = tpElement->FirstChildElement( ); tpChildElm != NULL; 
			tpChildElm = tpChildElm->NextSiblingElement( ) )
		{
			const char* tpKey = tpChildElm->Attribute("KEY");
			const char* tpFile = tpChildElm->Attribute("FILE");
			const char* tpLevel = tpChildElm->Attribute( "LEVEL" );

			if( ( tpKey == NULL ) || ( tpFile == NULL ) || ( tpLevel == NULL ) )
			{
				continue;
			}

			INIT_ROLLINGFILE_LOG( tpKey, tpFile, ( LogLevel )atoi( tpLevel ) );
		}
	}

	// handle log level
	tpElement = tpRoot->FirstChildElement( "HANDLE_THREAD" );
	if( tpElement != NULL )
	{
		tpElement->Attribute("Level", (int*)&m_stDBSvrdCfg.mHandleLogLevel );	
	}
	
//	LOG_DEBUG( "dbctrl", "mysql info sting: %s", m_stDBSvrdCfg.m_szMysql );
	
	LOG_NOTICE( "dbctrl", "Read Server Config End" );

    return 0;
}

int CDBCtrl::ReadDBIpCfg(const char *szIpFile)
{
	LOG_DEBUG( "dbctrl", "Read Ip Config Begin..." );

	TiXmlDocument tDoc;

	if( !tDoc.LoadFile( szIpFile ) )
	{
		LOG_ERROR( "dbctrl", "load xml %s failed", szIpFile );
		return -1;
	}

	TiXmlElement* tpElement = NULL;
	TiXmlElement* tpChildElm = NULL;

	// 先得到根节点
	TiXmlElement* tpRoot = tDoc.FirstChildElement();

	// Mysql配置分支
	tpElement = tpRoot->FirstChildElement("MYSQL");
	if( tpElement != NULL )
	{
		const char* szAscIICode = tpElement->Attribute( "mysql_info" );
		if( szAscIICode != NULL )
		{
			Tokens tokens = StrSplit(szAscIICode, ";");
			Tokens::iterator iter;

			std::string host, port_or_socket, user, password, database;
			int port;
			char const* unix_socket;

			iter = tokens.begin();

			if(iter != tokens.end()) host = *iter++;
			if(iter != tokens.end()) port_or_socket = *iter++;
			if(iter != tokens.end()) user = *iter++;
			if(iter != tokens.end()) password = *iter++;
			if(iter != tokens.end()) database = *iter++;

			char szUser[32+1] = {0};		
			int nUserLength = sizeof(szUser);

			char szPasswd[32+1] = {0};
			int nPasswdLength = sizeof(szPasswd);

			if ( DecryptAscIICode(user.c_str(), user.length(), szUser, &nUserLength) != true)
			{
				LOG_ERROR("default", "ERROR: Can NOT decrypt user(%s) section.", user.c_str());
				return -1;
			}

			if ( DecryptAscIICode(password.c_str(), password.length(), szPasswd, &nPasswdLength) != true)
			{
				LOG_ERROR("default", "ERROR: Can NOT decrypt user(%s) section.", user.c_str());
				return -1;
			}

			char szInputToken [256] = {0}; 
			int nResult = snprintf(szInputToken, sizeof(szInputToken)-1,  "%s;%s;%s;%s;%s", 
				host.c_str(), port_or_socket.c_str(), szUser, szPasswd, database.c_str());

			if (nResult <= 0)
			{
				LOG_ERROR("default", "ERROR: Section mysql_info is too large for requirement");
				return -1;
			}

			strncpy( m_stDBSvrdCfg.m_szMysql, szInputToken, sizeof(m_stDBSvrdCfg.m_szMysql)-1 );
			LOG_ERROR( "default", "mysql info sting: %s", m_stDBSvrdCfg.m_szMysql );
		}	
	}

	// db 服务器配置分支
	tpElement = tpRoot->FirstChildElement( "DB" );
	if( tpElement != NULL )
	{
		tpChildElm = tpElement->FirstChildElement();
		if( tpChildElm != NULL )
		{	
			// Server ID
			tpChildElm->Attribute("ID", (int*)&m_stDBSvrdCfg.m_iDBSvrID );

			// IP
			const char* tpValue = tpChildElm->Attribute("IP");
			if( tpValue != NULL )
			{
				strncpy( m_stDBSvrdCfg.m_szServiceIP, tpValue, sizeof(m_stDBSvrdCfg.m_szServiceIP)-1 );
			}

			LOG_DEBUG( "dbctrl", "db server info: id: %d ip:%s ",
				m_stDBSvrdCfg.m_iDBSvrID,
				m_stDBSvrdCfg.m_szServiceIP);
		}
	}

	// Proxy服务器 配置分支
	m_stDBSvrdCfg.m_sProxySvrdNum = 0; 

	tpElement = tpRoot->FirstChildElement("PROXY");
	if( tpElement != NULL )
	{
		for( tpChildElm = tpElement->FirstChildElement(); tpChildElm != NULL;  tpChildElm = tpChildElm->NextSiblingElement())
		{
			// SERVER ID
			tpChildElm->Attribute("ID", (int*)&m_stDBSvrdCfg.m_astProxySvrdCfg[m_stDBSvrdCfg.m_sProxySvrdNum].m_iServerID );

			// IP
			const char* tpValue = tpChildElm->Attribute("IP");
			if( tpValue != NULL )
			{
				strncpy( m_stDBSvrdCfg.m_astProxySvrdCfg[m_stDBSvrdCfg.m_sProxySvrdNum].m_szServiceIP,
					tpValue,
					sizeof( m_stDBSvrdCfg.m_astProxySvrdCfg[0].m_szServiceIP)-1 );
			}

			// Port
			tpValue = tpChildElm->Attribute("Port");
			if( tpValue != NULL )
			{
				strncpy( m_stDBSvrdCfg.m_astProxySvrdCfg[m_stDBSvrdCfg.m_sProxySvrdNum].m_szServicePort, 
					tpValue,
					sizeof(m_stDBSvrdCfg.m_astProxySvrdCfg[0].m_szServicePort)-1 );
			}


			LOG_DEBUG( "dbctrl", "proxy server info: id: %d ip:%s port:%s",
				m_stDBSvrdCfg.m_astProxySvrdCfg[m_stDBSvrdCfg.m_sProxySvrdNum].m_iServerID,
				m_stDBSvrdCfg.m_astProxySvrdCfg[m_stDBSvrdCfg.m_sProxySvrdNum].m_szServiceIP,
				m_stDBSvrdCfg.m_astProxySvrdCfg[m_stDBSvrdCfg.m_sProxySvrdNum].m_szServicePort);

			m_stDBSvrdCfg.m_sProxySvrdNum ++;

		}
	}

	LOG_DEBUG( "dbctrl", "Read Ip Config End" );

	return 0;
}

int  CDBCtrl::ConnectToProxyServer()
{
    int i = 0;

    for( i = 0; i < m_stDBSvrdCfg.m_sProxySvrdNum;  i++ )
    {
        m_astProxySvrdCon[i].Initialize( CET_PROXYSERVER, m_stDBSvrdCfg.m_astProxySvrdCfg[i].m_iServerID,
            inet_addr(m_stDBSvrdCfg.m_astProxySvrdCfg[i].m_szServiceIP), atoi(m_stDBSvrdCfg.m_astProxySvrdCfg[i].m_szServicePort));
        if( m_astProxySvrdCon[i].ConnectToServer(m_stDBSvrdCfg.m_szServiceIP))
        {
            LOG_INFO( "default", "Error:connect to Proxy Server %d failed.\n", m_stDBSvrdCfg.m_astProxySvrdCfg[i].m_iServerID);
            continue;
        }

		if( RegisterToProxyServer( &(m_astProxySvrdCon[i]) ) )
        {
            LOG_ERROR( "default", "Error: Register to Proxy Server %d failed.\n", m_stDBSvrdCfg.m_astProxySvrdCfg[i].m_iServerID);
            continue;
        }
        time(&m_atLastPorxyKeepAliveTime[i]);
        LOG_INFO( "default", "Connect to Proxy server %d Succeed.\n", m_stDBSvrdCfg.m_astProxySvrdCfg[i].m_iServerID );
    }

    return i;
}

/********************************************************
  Function:     RegisterToProxyServer
  Description:  向proxy发送注册消息
  Input:          pstTcpConn:  连接指针
  Output:      
  Return:       0 :   成功 ，其他失败
  Others:		
********************************************************/
int CDBCtrl::RegisterToProxyServer(CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> *pstTcpConn)
{
	if(!pstTcpConn)
	{
		LOG_ERROR( "default", "register to Proxy Server null para.");
		return -1;
	}

	CProxyHead stProxyHead;
	char acCodeBuf[MAXPROXYCODELEN] = {0};
	short shCodeLength = sizeof(acCodeBuf);

	stProxyHead.set_srcfe( CET_DBSERVER );
	stProxyHead.set_srcid( m_stDBSvrdCfg.m_iDBSvrID );
	stProxyHead.set_dstfe( CET_PROXYSERVER ) ;
	stProxyHead.set_transfertype( TRANS_P2P );
	stProxyHead.set_opflag( CMD_REGSERVER );

	// 空出头部4bytes( ushort , ushort )放长度数据
	bool bSerializeRet = stProxyHead.SerializeToArray( acCodeBuf+4, shCodeLength );
	if( bSerializeRet != true )
	{
		LOG_ERROR( "default", "Encode ProxyHead Failed While RegisterToProxyServer." );
		return -1;
	}
	unsigned short ushProxyHeadLen = stProxyHead.ByteSize() ;
	shCodeLength = 2  + ushProxyHeadLen + 2;

	*((unsigned short*)acCodeBuf) =shCodeLength;
	*((unsigned short*)(acCodeBuf+2)) = ushProxyHeadLen;

	pstTcpConn->GetSocket()->SendOneCode(shCodeLength, (BYTE *)acCodeBuf);
	
	return 0;
}

/********************************************************
  Function:     SendkeepAliveToProxy
  Description:  向proxy发送心跳消息
  Input:          pstTcpConn:  连接指针
  Output:      
  Return:       0 :   成功 ，其他失败
  Others:		
********************************************************/
int CDBCtrl::SendkeepAliveToProxy(CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> *pstTcpConn)
{
	if(!pstTcpConn)
	{
		LOG_ERROR( "dbctrl", "Error:send keey alive to Proxy null para.\n");
		return -1;
	}

	CProxyHead  tHead;
	char message_buffer[MAXPROXYCODELEN];
	pbmsg_setproxy(&tHead, CET_DBSERVER, CET_PROXYSERVER, m_stDBSvrdCfg.m_iDBSvrID, 1, TRANS_P2P, CMD_KEEPALIVE);

	unsigned short message_length = tHead.ByteSize() + 4;
	*((unsigned short*) message_buffer)  = message_length;

	*((unsigned short*) (message_buffer + 2))  = tHead.ByteSize();

	if(tHead.SerializeToArray(message_buffer+4, message_length - 4))
	{
		pstTcpConn->GetSocket()->SendOneCode(message_length, (BYTE *)message_buffer);
	}else
	{
		LOG_ERROR("default", " [ %s : %d ][ %s ] Encode Failed ", __LK_FILE__, __LINE__, __FUNCTION__);
		return -1;
	}

	return 0;
}

// ***************************************************************
//  Function: 	DisPatchOneCode   
//  Description:分派一个消息
//  Date: 		10/09/2008
// 
// ***************************************************************
int CDBCtrl::DispatchOneCode(int iProxyIdx, int nCodeLength, BYTE* pbyCode)
{
    int iHandleChoice = -1;
    int iTempRet = 0;

    if( iProxyIdx < 0 || iProxyIdx >= m_stDBSvrdCfg.m_sProxySvrdNum || nCodeLength < MINPROXYMSGLEN || !pbyCode )
    {
        LOG_ERROR( "dbctrl", "Invalid input in CDBCtrl::DispatchOneCode.\n");
        return -1;
    }

	// 只解CProxyHead部分 ( totallen , proxyheadlen, cproxyhead ), 注意头4字节被赋值成了 proxy idx,供dbhandle使用,跳过
	unsigned short usProxyHeadLen = *((unsigned short*)(pbyCode+sizeof(int)+2));
	CProxyHead tProxyHead;
	bool bParseRet = tProxyHead.ParseFromArray( pbyCode+4+sizeof(int), usProxyHeadLen );
	if( bParseRet != true )
	{
		LOG_ERROR( "dbctrl", "Decode ProxyHead faild." );
		return -1;
	}

	//加入proxy命令处理
	if( CET_PROXYSERVER == tProxyHead.srcfe())
	{
		//处理心跳消息
		if( CMD_KEEPALIVE == tProxyHead.opflag() )
		{
			time(&m_atLastPorxyKeepAliveTime[iProxyIdx]);
			return 0;
		}
	}

    //加入proxy命令处理
    switch( tProxyHead.transfertype() )
    {
		case TRANS_P2P:
        case TRANS_BYKEY:
        {
            //g_stServerStatisticForPrint.m_iRecvMsgNumTransByKey++;

            //交给指定的Handle处理
            iHandleChoice = tProxyHead.uin() % MAXHANDLENUMBER;

            iTempRet = m_apHandles[iHandleChoice]->PostOneCode(nCodeLength, pbyCode);

            LOG_DEBUG( "dbctrl", "Post code to handle %d returns %d.", iHandleChoice, iTempRet);
            if( iTempRet >= 0 )
            {
                m_apHandles[iHandleChoice]->WakeUp();
            }
            else
            {
                g_stServerStatisticForPrint.m_iPostCodeFailedTimes++;
            }
            break;
        }
        case TRANS_BROADCAST:
        {
            //g_stServerStatisticForPrint.m_iRecvMsgNumTransBroadCast++;
            ////交给所有Handle处理
            //for( iHandleChoice = 0; iHandleChoice < MAXHANDLENUMBER; iHandleChoice++ )
            //{
            //    iTempRet = m_apHandles[iHandleChoice]->PostOneCode(nCodeLength, pbyCode);
            //    if( iTempRet >= 0 )
            //    {
            //        m_apHandles[iHandleChoice]->WakeUp();
            //    }
            //    else
            //    {
            //        g_stServerStatisticForPrint.m_iPostCodeFailedTimes++;
            //    }
            //}
            ////增加自己处理关心的消息
            //short nHeadLength = stProxyHead.m_nTotalLength - stProxyHead.m_nBodyLength;
            //ProcessOneCode(pcTempCode+nHeadLength, stProxyHead.m_nBodyLength);
            //iTempRet = 0;
            break;
        }
        default:
        {
            LOG_ERROR( "dbctrl", "In CDBCtrl::DispatchOneCode, invalid trans type %d.\n", tProxyHead.transfertype() );
            return -3;
        }
    }

    return iTempRet;
}

//处理ctrl自己关心的消息
int CDBCtrl::ProcessOneCode(char *pacCode, int shCodeLength)
{
    CMessage stTempMsg;
    int iTempRet = 0;

    if((shCodeLength <= 0) || (NULL == pacCode))
    {
        LOG_ERROR( "dbctrl", "In ProcessOneCode, invalid input para.\n");
        return -1;
    }

    iTempRet = Event( &stTempMsg );

    return iTempRet;
}

int CDBCtrl::Event(CMessage *pstMsg)
{
    int iTempRet = 0;

    if(NULL == pstMsg)
    {
        LOG_ERROR( "dbctrl", "In Event, invalid input para.\n");
        return -1;
    }


    return iTempRet;
}

int CDBCtrl::CheckRunFlags()
{	
	int i;

    if( IsRunFlagSet( EFLG_CTRL_RELOAD ) )
    {
		ReadDBIpCfg( "../config/ip.xml" );
        ReadDBSvrdCfg( "../config/data/dbserver.xml" );
        for( i = 0; i < MAXHANDLENUMBER; i++ )
        {
            m_apHandles[i]->ProcessThreadEnd();
			m_apHandles[i]->InitLogFile( NULL, NULL, (LogLevel)m_stDBSvrdCfg.mHandleLogLevel );
#ifdef _ASYNC_SQL_
			m_apExecers[i]->StopThread();
			m_apExecers[i]->InitLogFile( NULL, NULL, (LogLevel)m_stDBSvrdCfg.mHandleLogLevel );
#endif
        }
        ClearRunFlag( EFLG_CTRL_RELOAD );
    }

    if( ! IsRunFlagSet( EFLG_CTRL_QUIT ) )
    {
        return 0;
    }

    //停止所有的线程
    for( i = 0; i < MAXHANDLENUMBER; i++ )
    {
        m_apHandles[i]->ProcessThreadEnd();
        m_apHandles[i]->StopThread();
#ifdef _ASYNC_SQL_
        m_apExecers[i]->StopThread();
#endif
    }

    LOG_NOTICE("default", "All threads stopped, main control quit.");
	
	LOG_NOTICE( "default", "xyj-dbserver exit !");

    exit(0);

    return 0;
}

int CDBCtrl::CheckAndDispatchInputMsg()
{
    fd_set fds_read;
    struct timeval stMonTime;
    int i, iTempFD = -1, iTempStatus = tcs_closed;
    int iOpenFDNum = 0;
    int iCount = 0;
    socklen_t iSockAddrSize;
    unsigned short nTmpCodeLength;
    BYTE  abyCodeBuf[MAX_SERVER_TRANS_LEN];
    char szTempIP[30];

    FD_ZERO(&fds_read);
    stMonTime.tv_sec = 0;
    stMonTime.tv_usec = 10000;

	for(i = 0; i < m_stDBSvrdCfg.m_sProxySvrdNum; i++)
	{
		iTempFD = m_astProxySvrdCon[i].GetSocket()->GetSocketFD();
		iTempStatus = m_astProxySvrdCon[i].GetSocket()->GetStatus();

		if(iTempFD > 0 && iTempStatus == tcs_connected)
		{
			//SAY("Check proxy(%d), socket fd = %d.\n", m_astProxyConn[i].GetEntityID(), iTempFD);
			FD_SET(iTempFD, &fds_read);
		}
	}

    iOpenFDNum = select(FD_SETSIZE, &fds_read, NULL, NULL, &stMonTime);

    if(iOpenFDNum <= 0)
    {
        return 0;
    }


	for(i = 0; i < m_stDBSvrdCfg.m_sProxySvrdNum; i++)
	{
		iTempFD = m_astProxySvrdCon[i].GetSocket()->GetSocketFD();
		if(iTempFD < 0)
		{
			continue;
		}


		if(FD_ISSET(iTempFD, &fds_read))
		{
			LOG_DEBUG( "dbctrl", "Proxy(%d) had req to process.", m_astProxySvrdCon[i].GetEntityID());
			m_astProxySvrdCon[i].GetSocket()->RecvData();
			while(1)
			{
				nTmpCodeLength = sizeof(abyCodeBuf) - sizeof(int);
				if(!(m_astProxySvrdCon[i].GetSocket()->GetOneCode(nTmpCodeLength, (BYTE *)&abyCodeBuf[sizeof(int)]) > 0))
				{
             		  break;
				}

				LOG_DEBUG( "dbctrl", "Get one code from ( EntityType:%d, EntityID:%d) Len=%d, dispatch it",
					m_astProxySvrdCon[i].GetEntityType(),
					m_astProxySvrdCon[i].GetEntityID(),
					nTmpCodeLength);
				memcpy((void *)abyCodeBuf, (const void *)&i, sizeof(int));
				nTmpCodeLength += sizeof(int);
				DispatchOneCode(i, nTmpCodeLength, (BYTE *)abyCodeBuf);
				iCount++;
			}

		}
    }	


    return iCount;
}

/*
int CDBCtrl::CreateServer()
{
    short unTempPort = (u_short)atoi((const char *)m_stDBSvrdCfg.m_szServicePort);
    if(m_stServerConn.Initialize(cet_dbsvr, m_stDBSvrdCfg.m_iDBSvrID,   inet_addr(m_stDBSvrdCfg.m_szServiceIP), unTempPort) < 0)
    {
        SAY("Error in CDBCtrl::CreateServer  Initialize Conn  failed\n");
        return -1;
    }
    return m_stServerConn.CreateServer();
}*/

int CDBCtrl::PrepareToRun()
{
    int i;

#ifdef _ASYNC_SQL_
    int aiSocketPair[2];
#endif

    if(ConnectToProxyServer() < 0)
    {
		LOG_ERROR( "dbctrl", "Error: in CDBCtrl::PrepareToRun connect proxy  server  failed! try to reconnect later ... \n");
		printf( "Error: connect proxy  server  failed, try to reconnect later ... \n" ); 
		//return -1;
	}
	
    //初始化连接DB的互斥变量
    pthread_mutex_init( &CDBHandle::stDBConnMutex, NULL );

    for(i = 0; i < MAXHANDLENUMBER; i++)
    {	
#ifdef _ASYNC_SQL_
        if(socketpair(AF_LOCAL, SOCK_STREAM, 0, aiSocketPair))
        {
            LOG_ERROR( "dbctrl", "In CDBCtrl::PrepareToRun, create local pair failed of %d, %s.\n", errno, strerror(errno));
            return -1;
        }
        int iRcvLen, iSndLen, iErr,iOptVal, iOptLen = sizeof(int);
	
        iRcvLen = iSndLen = 0x400000;
        iErr = setsockopt(aiSocketPair[0] , SOL_SOCKET, SO_SNDBUF, &iSndLen, iOptLen);
        if(iErr)
        {
			LOG_ERROR( "dbctrl", "In CDBCtrl::PrepareToRun, set pair socket send buff  failed of %d", iErr );
			return -1;
        }
        if(!getsockopt(aiSocketPair[0], SOL_SOCKET, SO_SNDBUF, (void *)&iOptVal, (socklen_t *)&iOptLen))
        {
			// 系统默认会设置成传入参数的2倍
			if( (int)(iSndLen * 2) != (int)(iOptVal) )
			{
				  LOG_ERROR( "dbctrl", "Pair Socket Send buf of %d  is %d.\n", i, iOptVal);
			}
          
        }

        iErr = setsockopt(aiSocketPair[1], SOL_SOCKET, SO_RCVBUF, &iRcvLen, iOptLen);
        if(iErr)
        {
            LOG_ERROR( "dbctrl", "In CDBCtrl::PrepareToRun, set pair socket rec buff  failed of %d", iErr);
            return -1;
        }
        if(!getsockopt(aiSocketPair[1], SOL_SOCKET, SO_RCVBUF, (void *)&iOptVal, (socklen_t *)&iOptLen))
        {
			// 系统默认会设置成传入参数的2倍
			if( (int)(iRcvLen * 2) != (int)(iOptVal) )
			{
				 LOG_ERROR( "dbctrl", "Pair Socket Receive buf of %d is %d.", i, iOptVal);
			}
        }


        if(m_apHandles[i]->Initialize(i, &m_astProxySvrdCon[0], aiSocketPair[0] ) <  0)
        {
            LOG_ERROR( "dbctrl", "In CDBCtrl::PrepareToRun, Initialize handle failed\n");
            return -1;
        }
		m_apHandles[i]->InitLogFile( NULL, NULL, (LogLevel)m_stDBSvrdCfg.mHandleLogLevel );

        m_apExecers[i]->Initialize(i+MAXHANDLENUMBER, aiSocketPair[1]);
		m_apExecers[i]->InitLogFile( NULL, NULL, (LogLevel)m_stDBSvrdCfg.mHandleLogLevel );
		m_apExecers[i]->SetHostInfoString( CDBCtrl::GetSingletonPtr()->GetDBSvrConf().m_szMysql );
#else	
        m_apHandles[i]->Initialize(i, &m_astProxySvrdCon[0]);
		m_apHandles[i]->InitLogFile( NULL, NULL, (LogLevel)m_stDBSvrdCfg.mHandleLogLevel );
#endif	
    }
	


    LOG_INFO( "dbctrl", "Handles initialized OK, now begin to create threads.\n");
    for(i = 0; i < MAXHANDLENUMBER; i++)
    {
        if(m_apHandles[i]->CreateThread())
        {
            LOG_ERROR( "dbctrl", "Create handle thread %d failed.\n", i);
            return -1;
        }

#ifdef _ASYNC_SQL_
        if( m_apExecers[i]->CreateThread() )
        {
            LOG_ERROR( "dbctrl", "Create Execer thread %d failed.\n", i);
            return -1;
        }
#endif	
    }

    LOG_INFO( "dbctrl", "Successfully create %d threads to handle request.\n", i);

    return 0;
}

int CDBCtrl::Run()
{
    while(True)
    {
        CheckRunFlags();

        RoutineCheck();

        CheckAndDispatchInputMsg();

        g_stServerStatisticForPrint.m_iCtrlLoopNum++;
    }

    return 0;
}

const STDBSvrdCfg CDBCtrl::GetDBSvrConf()
{
    return m_stDBSvrdCfg;
}

int CDBCtrl::RoutineCheck()
{
	static time_t iLastCheckTime = time(NULL);
	static time_t iLastPrintPerf = time(NULL);
	time_t tNow = time(NULL);

	if(tNow - iLastCheckTime >= CHECKINTERVAL )
	{
		iLastCheckTime = tNow;
		// 和proxy保持心跳
		for(int i = 0; i < m_stDBSvrdCfg.m_sProxySvrdNum; i++)
		{
			if( (m_astProxySvrdCon[i].GetSocket()->GetStatus() == tcs_connected) 
				&& (m_astProxySvrdCon[i].GetSocket()->GetSocketFD() > 0)
				&& (tNow - m_atLastPorxyKeepAliveTime[i] < PROXYKEEPALIVESECONDS))
			{
				SendkeepAliveToProxy(&(m_astProxySvrdCon[i]));
				continue;
			}

			LOG_ERROR("default", "Proxy[%d](ID = %d) is not connected, try to reconnect it",i, m_astProxySvrdCon[i].GetEntityID());

			// 如果已经断开了， 则重新连接
			if(m_astProxySvrdCon[i].ConnectToServer(m_stDBSvrdCfg.m_szServiceIP))
			{
				LOG_ERROR("default", "Connect proxy[%d] failed.", i);
				continue;
			}

			// 然后注册
			if(RegisterToProxyServer(&(m_astProxySvrdCon[i])))
			{
				LOG_ERROR("default", "Register proxy[%d] failed.", i);
				continue;
			}

			time(&m_atLastPorxyKeepAliveTime[i]);
			LOG_INFO("default", "Connect to proxy %d succeeded.", i);
		}
	}

	// 检查是否该打印统计日志了
	if(tNow - iLastPrintPerf >= PERF_TIMER )
	{
		iLastPrintPerf = tNow;
		for ( int i = 0; i < MAXHANDLENUMBER; ++i )
		{
			if ( m_apHandles[i] != NULL )
			{
				m_apHandles[i]->m_nSavePerfLogFlag = 1;
			}
		}
	}

	return 0;
}


int CDBCtrl::PrintServerStatistic()
{
    //FILE *fpStatistic = NULL;
    //char fname[128];
    //TStruTime stTempNow;
    //char szCurDate[32];
    //char szCurDateTime[32];
    //char szCurAddr[32];
    //char szCurMon[32];
    //char szStatisticDir[64];
    //int iAveHandleTime;
    //int i, iTotal;

    //GetCurStructTime( &stTempNow );
    //snprintf(szCurMon, sizeof(szCurMon)-1,"%04d-%02d", stTempNow.m_iYear, stTempNow.m_iMon);
    //snprintf(szCurDate,sizeof(szCurDate)-1, "%04d-%02d-%02d", stTempNow.m_iYear, stTempNow.m_iMon, stTempNow.m_iDay);
    //snprintf(szCurDateTime, sizeof(szCurDateTime)-1,"%04d-%02d-%02d %02d:%02d:%02d", stTempNow.m_iYear, stTempNow.m_iMon, stTempNow.m_iDay, stTempNow.m_iHour, stTempNow.m_iMin, stTempNow.m_iSec);
    //snprintf(szStatisticDir,sizeof(szStatisticDir)-1, "../statistic/%s", szCurMon);

    //snprintf( fname, sizeof(fname)-1,"%s/dbs-%s.txt", szStatisticDir, szCurDate);
    //DIR* dpStatistic = opendir( szStatisticDir);
    //if ( !dpStatistic )
    //{
    //    if ( mkdir(szStatisticDir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) )
    //    {
    //        TLib_Log_LogMsg("in CDBCtrl::PrintServerStatistic, can't create bill dir %s.\n", szStatisticDir);
    //        return Wrong;
    //    }
    //}
    //else
    //{
    //    closedir( dpStatistic);
    //}
    //fpStatistic = fopen( fname , "a+" );

    //if ( !fpStatistic )
    //{
    //    TLib_Log_LogMsg("Open statistic file %s failed.\n", fname);
    //    return Wrong;
    //}
    //fprintf(fpStatistic, "\n=======================%s======================\n", szCurDateTime);
    ////打印系统统计信息
    //fprintf(fpStatistic, "CtrlLoopNum     :       %8d\n"
    //                     "ReceiveMsgNum   :       %8d        Size :       %d\n"
    //                     "SendMsgNum      :       %8d        Size :       %d\n"
    //                     "ReceiveMsgNumTransByKey         :      %8d\n"
    //                     "ReceiveMsgNumTransBroadCast     :      %8d\n",
    //                 g_stServerStatisticForPrint.m_iCtrlLoopNum, 
    //                 g_stServerStatisticForPrint.m_iRecvMsgNum, g_stServerStatisticForPrint.m_iRecvPkgSize,
    //                 g_stServerStatisticForPrint.m_iSendMsgNum, g_stServerStatisticForPrint.m_iSendPkgSize, 
    //                 g_stServerStatisticForPrint.m_iRecvMsgNumTransByKey,
    //                 g_stServerStatisticForPrint.m_iRecvMsgNumTransBroadCast);

    //fprintf(fpStatistic, "InsertNum     :          %d\n"
    //                     "UpdateNum     :          %d\n"
    //                     "SelectNum     :          %d\n"
    //                     "DeleteNum     :          %d\n",
    //                g_stServerStatisticForPrint.m_iInsertNum,
    //                g_stServerStatisticForPrint.m_iUpdateNum,
    //                g_stServerStatisticForPrint.m_iSelectNum,
    //                g_stServerStatisticForPrint.m_iDeleteNum);

    //iAveHandleTime = 0;
    //if(g_stServerStatisticForPrint.m_iSyncInsertNum > 0)
    //{
    //    iAveHandleTime = 1000000 * g_stServerStatisticForPrint.m_tSyncInsertTime.tv_sec;
    //    iAveHandleTime += g_stServerStatisticForPrint.m_tSyncInsertTime.tv_usec;
    //    iAveHandleTime /= g_stServerStatisticForPrint.m_iSyncInsertNum;
    //}
    //fprintf(fpStatistic, "SyncInsertNum   :%8d, CostTime:%d.%06d(sec), AVE:%u(usec) \n",
    //                g_stServerStatisticForPrint.m_iSyncInsertNum,
    //                g_stServerStatisticForPrint.m_tSyncInsertTime.tv_sec,
    //                g_stServerStatisticForPrint.m_tSyncInsertTime.tv_usec,
    //                iAveHandleTime);

    //iAveHandleTime = 0;
    //if(g_stServerStatisticForPrint.m_iSyncUpdateNum > 0)
    //{
    //    iAveHandleTime = 1000000 * g_stServerStatisticForPrint.m_tSyncUpdateTime.tv_sec;
    //    iAveHandleTime += g_stServerStatisticForPrint.m_tSyncUpdateTime.tv_usec;
    //    iAveHandleTime /= g_stServerStatisticForPrint.m_iSyncUpdateNum;
    //}
    //fprintf(fpStatistic, "SyncUpdateNum   :%8d, CostTime:%d.%06d(sec), AVE:%u(usec) \n",
    //                g_stServerStatisticForPrint.m_iSyncUpdateNum,
    //                g_stServerStatisticForPrint.m_tSyncUpdateTime.tv_sec,
    //                g_stServerStatisticForPrint.m_tSyncUpdateTime.tv_usec,
    //                iAveHandleTime);

    //iAveHandleTime = 0;
    //if(g_stServerStatisticForPrint.m_iSyncSelectNum > 0)
    //{
    //    iAveHandleTime = 1000000 * g_stServerStatisticForPrint.m_tSyncSelectTime.tv_sec;
    //    iAveHandleTime += g_stServerStatisticForPrint.m_tSyncSelectTime.tv_usec;
    //    iAveHandleTime /= g_stServerStatisticForPrint.m_iSyncSelectNum;
    //}
    //fprintf(fpStatistic, "SyncSelectNum   :%8d, CostTime:%d.%06d(sec), AVE:%u(usec) \n",
    //                g_stServerStatisticForPrint.m_iSyncSelectNum,
    //                g_stServerStatisticForPrint.m_tSyncSelectTime.tv_sec,
    //                g_stServerStatisticForPrint.m_tSyncSelectTime.tv_usec,
    //                iAveHandleTime);

    //iAveHandleTime = 0;
    //if(g_stServerStatisticForPrint.m_iSyncDeleteNum > 0)
    //{
    //    iAveHandleTime = 1000000 * g_stServerStatisticForPrint.m_tSyncDeleteTime.tv_sec;
    //    iAveHandleTime += g_stServerStatisticForPrint.m_tSyncDeleteTime.tv_usec;
    //    iAveHandleTime /= g_stServerStatisticForPrint.m_iSyncDeleteNum;
    //}
    //fprintf(fpStatistic, "SyncDeleteNum   :%8d, CostTime:%d.%06d(sec), AVE:%u(usec) \n",
    //                g_stServerStatisticForPrint.m_iSyncDeleteNum,
    //                g_stServerStatisticForPrint.m_tSyncDeleteTime.tv_sec,
    //                g_stServerStatisticForPrint.m_tSyncDeleteTime.tv_usec,
    //                iAveHandleTime);

    //fprintf(fpStatistic, "DecodeProxyMsgFailedTimes   :          %d\n"
    //                     "EncodeProxyMsgFailedTimes   :          %d\n"
    //                     "GetCodeFailedTimes          :          %d\n"
    //                     "PostCodeFailedTimes         :          %d\n",
    //                g_stServerStatisticForPrint.m_iDecodeProxyMsgFailedTimes,
    //                g_stServerStatisticForPrint.m_iEncodeProxyMsgFailedTimes,
    //                g_stServerStatisticForPrint.m_iGetCodeFailedTimes,
    //                g_stServerStatisticForPrint.m_iPostCodeFailedTimes);

    //iAveHandleTime = 0;
    //if(g_stServerStatisticForPrint.m_iExecSqlAsyncSuccNum > 0)
    //{
    //    iAveHandleTime = 1000000 * g_stServerStatisticForPrint.m_tExecSqlAsyncSuccTime.tv_sec;
    //    iAveHandleTime += g_stServerStatisticForPrint.m_tExecSqlAsyncSuccTime.tv_usec;
    //    iAveHandleTime /= g_stServerStatisticForPrint.m_iExecSqlAsyncSuccNum;
    //}
    //fprintf(fpStatistic, "ExecSqlAsyncFailedNum :%d\n"
		  //       "ExecSqlAsyncSuccNum   :%d, CostTime:%d.%06d(sec), AVE:%u(usec) \n",
    //                g_stServerStatisticForPrint.m_iExecSqlAsyncFailedNum,
    //                g_stServerStatisticForPrint.m_iExecSqlAsyncSuccNum,
    //                g_stServerStatisticForPrint.m_tExecSqlAsyncSuccTime.tv_sec,
    //                g_stServerStatisticForPrint.m_tExecSqlAsyncSuccTime.tv_usec,
    //                iAveHandleTime);

    //fclose(fpStatistic);
    return 0;
}


