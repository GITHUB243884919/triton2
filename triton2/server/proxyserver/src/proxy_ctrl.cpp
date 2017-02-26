#include "proxy_ctrl.h"
#include "proxy_comm.h"
#include "config.h"
#include "tinyxml.h"
#include "servermessage_pb.hxx.pb.h"


extern CFlag g_byRunFlag;

#ifdef _POSIX_MT_

pthread_mutex_t CProxyCtrl::stMutex[MAX_MUTEX_NUM];

#endif

#define SENDING_BUFFER_SIZE 10240

CProxyCtrl::CProxyCtrl()
{
	int i;
	
	m_pstMainConn = new CMyTCPConn[MAX_GAME_SERVER];
	m_pstDBConn = new CMyTCPConn[MAX_DB_SERVER];
	m_pstOtherConn = new CMyTCPConn[MAX_OTHER_SERVER];
	m_pstDunGeon = new CMyTCPConn[MAX_DUNGEON_NUM];

	m_pDBHandle = NULL;
	m_pMainHandle = NULL;
	m_pOtherHandle = NULL;
	m_pProxyHandle = NULL;
	m_pGlobalProxy = NULL;
	m_pDungeonHandle = NULL;
	/*
	 * memset((void *)&m_stProxyCfg, 0, sizeof(m_stProxyCfg));
	
	//Clear route table
	for( i = 0; i < MAXSVRNUMBER; i++ )
	{
		m_stProxyCfg.m_anMRT[i] = -1;
	}
	*/


	m_iCurrentUnRegisterNum = 0;
	memset((void *)m_astUnRegisterInfo, 0, sizeof(m_astUnRegisterInfo));
	time(&m_tLastCheckTime);
	
}

CProxyCtrl::~CProxyCtrl()
{
	if( m_pDBHandle )
	{
		delete m_pDBHandle;
		m_pDBHandle = NULL;
	}

	if( m_pMainHandle )
	{
		delete m_pMainHandle;
		m_pMainHandle = NULL;
	}

	if( m_pOtherHandle )
	{
		delete m_pOtherHandle;
		m_pOtherHandle = NULL;
	}

	if ( m_pProxyHandle )
	{
		delete m_pProxyHandle;
		m_pProxyHandle = NULL;
	}

	if ( m_pDungeonHandle )
	{
		delete m_pDungeonHandle;
		m_pDungeonHandle = NULL;
	}
	
	if ( m_pGlobalProxy )
	{
		delete m_pGlobalProxy;
		m_pGlobalProxy = NULL;
	}	

	if ( m_pstMainConn )
	{
		delete [] m_pstMainConn;
		m_pstMainConn = NULL;
	}

	if ( m_pstDBConn )
	{
		delete [] m_pstDBConn;
		m_pstDBConn = NULL;
	}

	if ( m_pstOtherConn )
	{
		delete [] m_pstOtherConn;
		m_pstOtherConn = NULL;
	}

	if ( m_pstDunGeon )
	{
		delete [] m_pstDunGeon;
		m_pstDunGeon = NULL;
	}
	
}

int CProxyCtrl::Initialize(const char *szCfgFile)
{

	int i;
	if( !szCfgFile )
	{
		return -1;
	}
	m_astProxyConn.Initialize();
	if( ReadCfg(szCfgFile) )
	{
		LOG_ERROR("default", "default", "Read config failed.\n");
		return -1;
	}

	/*
	if( LoadConnInfoFromFile(cet_gamesvr,
		(CMyTCPConn *)m_astMainConn, m_stProxyCfg.m_iMainSvrNum) )
	{
		LOG_ERROR("Load main servers failed.\n");
		return -1;
	}
	
	if( LoadConnInfoFromFile(cet_dbsvr,
		(CMyTCPConn *)m_astDBConn, m_stProxyCfg.m_iDBSvrNum) )
	{
		LOG_ERROR("Load db servers failed.\n");
		return -1;
	}

	if( LoadConnInfoFromFile(cet_other,
		(CMyTCPConn *)m_astOtherConn, m_stProxyCfg.m_iOtherEntityNum) )
	{
		LOG_ERROR("Load other entitys failed.\n");
		return -1;
	}
	*/


	//pthread_mutex_init( &stMutex, NULL);
	for(i = 0; i < MAX_MUTEX_NUM; i++)
	{
		pthread_mutex_init( &stMutex[i], NULL);
	}
	/*
	if( LoadHashMapFromFile() )
	{
		LOG_ERROR("Load hash map failed.\n");
		return -1;
	}

	if( LoadMRTFromFile() )
	{
		LOG_ERROR("Load main route table failed.\n");
	}
	*/
	
	mLastTickCount = 0;
	mAliveTimer = GLOBALPROXY_ALIVE_TIME;
	return 0;
}

int CProxyCtrl::ReadCfg(const char *szCfgFile)
{
	if( !szCfgFile )
	{
		printf(" ERROR: Config file must be null string!\n");
		return -1;
	}

	TiXmlDocument tTplDoc;
	bool bRet = tTplDoc.LoadFile( szCfgFile );

	if ( !bRet )
	{
		printf(" ERROR: Config file (%s) must be a regulure xml format!\n", szCfgFile);
		return -1;
	}	

	TiXmlElement* tpRootEle = tTplDoc.RootElement();
	if ( tpRootEle == NULL )
	{
		printf(" ERROR: Config file (%s) must have a root element!\n", szCfgFile);
		return -1;
	}

	const char* tEleName = tpRootEle->Value();
	if ( strcmp( tEleName, "server_list" ) != 0 )
	{
		printf(" ERROR: Config file (%s) must have a root element named server_list!\n", szCfgFile);
		return -1;
	}

	TiXmlElement* tpElement = tpRootEle->FirstChildElement( "server" );
	while ( tpElement )
	{
		const char* szServerType = tpElement->Attribute("type");

		if (strcmp(szServerType, "local_server") == 0 )
		{
			m_stProxyCfg.m_iProxyID = atoi(tpElement->Attribute("id"));
			m_stProxyCfg.m_iProxyPort = atoi(tpElement->Attribute("port"));
			m_stProxyCfg.m_nFlag = atoi( tpElement->Attribute("flag") );

			// m_nFlag 为1是全局服务器 0是游戏服务器
			if ( m_stProxyCfg.m_nFlag == 0 )
			{
				m_stProxyCfg.m_nGameID = atoi( tpElement->Attribute("gameid") );
			}
			
		}
		else if (strcmp(szServerType, "proxy_server") == 0 )
		{			
			const char* szTempAddr = tpElement->Attribute( "addr" );
			int nServerId   = atoi(tpElement->Attribute( "id" ));

			if ( m_stProxyCfg.m_nProxyNum >= 0 && m_stProxyCfg.m_nProxyNum < MAX_PROXY_NUM && m_astProxyConn.GetTcpConnByEntityID( nServerId ) == NULL ) 
			{
				m_astProxyConn[m_stProxyCfg.m_nProxyNum].Initialize( CET_PROXYSERVER , nServerId, inet_addr(szTempAddr), 0 );
				m_stProxyCfg.m_nProxyNum++;
			}
		}
		else if ( strcmp(szServerType,"dungeon_server") == 0 )
		{
			const char* szTempAddr = tpElement->Attribute( "addr" );
			int nServerId   = atoi(tpElement->Attribute( "id" ));	
			if ( m_pstDunGeon == NULL )
			{
				LOG_ERROR("default", "m_pstDunConn is NULL!\n" );
				exit(0);
			}
			
			if ( nServerId > 0 && nServerId < MAX_DUNGEON_NUM )
			{
				m_pstDunGeon[nServerId].Initialize( CET_DUNGEON, nServerId, inet_addr(szTempAddr), 0 );
				m_stProxyCfg.m_nDungeonNum++;	
			}			
		}
		else if ( strcmp(szServerType,"global_proxy") == 0 )
		{
			const char* szTempAddr = tpElement->Attribute( "addr" );			
			int nPort		= atoi( tpElement->Attribute( "port" ) );			
			int nServerID = atoi( tpElement->Attribute( "id" ) );	
			sprintf( m_stProxyCfg.m_sGlobalProxyAddr, "%s:%d", szTempAddr, nPort );
			m_astGlobalProxy.Initialize( CET_GLOBALPROXY, nServerID, inet_addr(szTempAddr), nPort );
		}
		else if (strcmp(szServerType, "database_server") == 0 )
		{
			m_stProxyCfg.m_iDBSvrNum ++;
			const char* szTempAddr = tpElement->Attribute( "addr" );
			int nServerId   = atoi(tpElement->Attribute( "id" ));
			if ( m_pstDBConn == NULL)
			{
				LOG_ERROR("default", "m_pstDBConn is NULL!\n" );
				exit(0);
			}
			
			if (nServerId > 0 && nServerId < MAX_DB_SERVER) 
			{
				m_pstDBConn[nServerId].Initialize(CET_DBSERVER , nServerId, inet_addr(szTempAddr), 0 );
			}
		}
		
		else if (strcmp(szServerType, "game_server") == 0 )
		{
			m_stProxyCfg.m_iMainSvrNum ++;
			const char* szTempAddr = tpElement->Attribute( "addr" );
			int nServerId   = atoi(tpElement->Attribute( "id" ));
			if ( m_pstMainConn == NULL )
			{
				LOG_ERROR("default", "m_pstMainConn is NULL!\n" );
				exit( 0 );
			}
			
			if (nServerId > 0 && nServerId < MAX_GAME_SERVER) 
			{
				m_pstMainConn[nServerId].Initialize(CET_GAMESERVER , nServerId, inet_addr(szTempAddr), 0 );
			}
		}
		
		else if (strcmp(szServerType, "other_server") == 0 )
		{
			m_stProxyCfg.m_iOtherEntityNum ++;
			const char* szTempAddr = tpElement->Attribute( "addr" );
			int nServerId   = atoi(tpElement->Attribute( "id" ));
			if ( m_pstOtherConn == NULL )
			{
				LOG_ERROR("default", "m_pstOtherConn is NULL!\n" );
				exit(0);
			}
			
			if (nServerId > 0 && nServerId < MAX_OTHER_SERVER) 
			{
				m_pstOtherConn[nServerId].Initialize(CET_OTHERSERVER, nServerId, inet_addr(szTempAddr), 0 );
			}
		}

		tpElement = tpElement->NextSiblingElement( "server" );
	}	

	if( m_stProxyCfg.m_iProxyID <= 0 || m_stProxyCfg.m_iProxyPort == 0 )
	{
		printf( "read config failed\n" );
		LOG_ERROR("default", "Read config failed.\n");
		return -1;
	}

	return 0;
}

/*
int CProxyCtrl::LoadConnInfoFromFile(short nConnType, CMyTCPConn* pConns, int& iConnNum)
{
	FILE *fpTempFile = NULL;
	char szLineBuf[1024];
	int  iReadRows = 0;
	int  iReadColomns = 0;
	int  i;

	int  iTempSvrID = 0;
	TName szTempAddr;
	char *szFileName = NULL;

	if( !pConns )
	{
		return -1;
	}

	if( iConnNum <= 0 )
	{
		return 0;
	}

	switch(nConnType)
	{
		case cet_gamesvr:
		{
			szFileName = (char *)m_stProxyCfg.m_szMainSvrsFile;
			break;
		}
		case cet_dbsvr:
		{
			szFileName = (char *)m_stProxyCfg.m_szDBSvrsFile;
			break;
		}
		case cet_other:
		{
			szFileName = (char *)m_stProxyCfg.m_szOthEnsFile;
			break;
		}
		default:
		{
			return 0;
		}
	}

	fpTempFile = fopen(szFileName, "r");

	if( !fpTempFile )
	{
		LOG_ERROR("Open file %s failed.\n", szFileName);
		return -1;
	}

	//首先清空配置
	for( i = 0; i < MAXSVRNUMBER; i++ )
	{
		pConns[i].Initialize(nConnType, -1, 0, 0);
	}

	iReadRows = 0;
	while( iReadRows < iConnNum && !feof(fpTempFile) )
	{
		fgets((char *)szLineBuf, sizeof(szLineBuf), fpTempFile);
		TrimStr( (char *)szLineBuf );
		if( szLineBuf[0] == '#' )
		{
			continue;
		}
		LOG_DEBUG("config line:%s\n", szLineBuf);
		iReadColomns = sscanf(szLineBuf, "%d%s", &iTempSvrID, szTempAddr);
		if( iReadColomns < 2 )
		{
			
			continue;
		}

		if( nConnType == cet_gamesvr )
		{
			pConns[(iTempSvrID-1)%MAXSVRNUMBER].Initialize( nConnType, iTempSvrID, inet_addr(szTempAddr), 0 );
		}
		else
		{
			if( iTempSvrID <= 0 || iTempSvrID > MAXSVRNUMBER )
			{
				continue;
			}
			pConns[iTempSvrID-1].Initialize( nConnType, iTempSvrID, inet_addr(szTempAddr), 0 );
		}

		iReadRows++;
	}

	fclose(fpTempFile);

	iConnNum = iReadRows;

	return 0;
}

int CProxyCtrl::LoadHashMapFromFile()
{
	FILE *fpTempFile = NULL;
	char szLineBuf[1024];
	int  iReadRows = 0;
	int  iReadColomns = 0;
	int  i;
	char szTmpHashKey[500];
	short ashTempHashKey[MAXHASHMAPITEMNUMBER];
	int iHashKeyNum = 0;
	int iTempSvr1, iTempSvr2;

	fpTempFile = fopen(m_stProxyCfg.m_szMapFile, "r");
	if( !fpTempFile )
	{
		LOG_ERROR("Open file %s failed.\n", m_stProxyCfg.m_szMapFile);
		return -1;
	}


#ifdef _POSIX_MT_

	//pthread_mutex_lock( &stMutex );
	pthread_mutex_lock( &stMutex[MUTEX_HASHMAP] );

#endif	


	//首先清空配置
	for( i = 0; i < MAXHASHMAPITEMNUMBER; i++ )
	{
		m_stProxyCfg.m_astHashMapTable[i].m_nPrevSvrID = -1;
		m_stProxyCfg.m_astHashMapTable[i].m_nCurrentSvrID = -1;
	}

	iReadRows = 0;
	while( !feof(fpTempFile) )
	{
		fgets((char *)szLineBuf, sizeof(szLineBuf), fpTempFile);
		TrimStr( (char *)szLineBuf );
		if( szLineBuf[0] == '#' )
		{
			continue;
		}

		iReadColomns = sscanf(szLineBuf, "%s%d%d", szTmpHashKey, &iTempSvr1, &iTempSvr2);
		if( iReadColomns < 3 )
		{
			continue;
		}

		iHashKeyNum = SplitMapStr(szTmpHashKey, MAXHASHMAPITEMNUMBER, ashTempHashKey);

		for(i = 0; i < iHashKeyNum; ++i)
		{
			if(ashTempHashKey[i] < 0 || ashTempHashKey[i] > MAXHASHMAPITEMNUMBER)
			{
				continue;
			}

			m_stProxyCfg.m_astHashMapTable[ashTempHashKey[i]].m_nPrevSvrID = iTempSvr1;
			m_stProxyCfg.m_astHashMapTable[ashTempHashKey[i]].m_nCurrentSvrID = iTempSvr2;
		}

		iReadRows++;
	}


#ifdef _POSIX_MT_

	//pthread_mutex_unlock( &stMutex );
	pthread_mutex_unlock( &stMutex[MUTEX_HASHMAP] );

#endif


	fclose(fpTempFile);

#ifdef _DEBUG_
	for( i = 0; i < MAXHASHMAPITEMNUMBER; i++ )
	{
		LOG_DEBUG("Key = %d, PrevSvr = %d, CurrSvr = %d\n", i, m_stProxyCfg.m_astHashMapTable[i].m_nPrevSvrID, m_stProxyCfg.m_astHashMapTable[i].m_nCurrentSvrID);
	}
#endif

	return 0;
}


int CProxyCtrl::LoadMRTFromFile()
{
	FILE *fpTempFile = NULL;
	char szLineBuf[1024];
	int  iReadRows = 0;
	int  iReadColomns = 0;
	int  i;
	int iTempSvr1, iTempSvr2;

	if(!m_stProxyCfg.m_szMRTFile)
	{
		return -1;
	}

	fpTempFile = fopen((const char *)m_stProxyCfg.m_szMRTFile, "r");
	if( !fpTempFile )
	{
		LOG_ERROR("Open file %s failed.\n", m_stProxyCfg.m_szMRTFile);
		return -1;
	}

#ifdef _POSIX_MT_
	pthread_mutex_lock( &stMutex[MUTEX_ROUTETAB] );
#endif	

	//首先清空配置
	for( i = 0; i < MAXSVRNUMBER; i++ )
	{
		m_stProxyCfg.m_anMRT[i] = -1;
	}

	iReadRows = 0;
	while( !feof(fpTempFile) )
	{
		fgets((char *)szLineBuf, sizeof(szLineBuf), fpTempFile);
		if(feof(fpTempFile))
		{
			break;
		}
		TrimStr( (char *)szLineBuf );
		if( szLineBuf[0] == '#' )
		{
			continue;
		}

		iReadColomns = sscanf(szLineBuf, "%d%d", &iTempSvr1, &iTempSvr2);
		if( iReadColomns < 2 )
		{
			continue;
		}
		if( iTempSvr1 < (m_stProxyCfg.m_iProxyID - 1)*MAXSVRNUMBER ||
			iTempSvr1 >= m_stProxyCfg.m_iProxyID*MAXSVRNUMBER ||
			iTempSvr2 < (m_stProxyCfg.m_iProxyID - 1)*MAXSVRNUMBER ||
			iTempSvr2 >= m_stProxyCfg.m_iProxyID*MAXSVRNUMBER )	
		{
			continue;
		}
		m_stProxyCfg.m_anMRT[(iTempSvr1-1)%MAXSVRNUMBER] = iTempSvr2-1;
		iReadRows++;
	}

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &stMutex[MUTEX_ROUTETAB] );
#endif

	fclose(fpTempFile);

#ifdef _DEBUG_
	for( i = 0; i < MAXSVRNUMBER; i++ )
	{
		if(m_stProxyCfg.m_anMRT[i] >= 0)
		{
			LOG_DEBUG("DST = %d, TARGET = %d\n",
					i+(m_stProxyCfg.m_iProxyID - 1)*MAXSVRNUMBER,
					m_stProxyCfg.m_anMRT[i]);
		}
	}
#endif

	return 0;
}
*/


CMyTCPConn* CProxyCtrl::GetConnByAddrAndID(short shSrcFE, short shSrcID, u_long ulSrcAddr, int nGameID )
{
	CMyTCPConn *pastTcpConn = NULL;

	switch(shSrcFE)
	{
		case CET_GAMESERVER:
		{
			pastTcpConn = m_pstMainConn;
			break;
		}
		case CET_DBSERVER:
		{
			pastTcpConn = m_pstDBConn;
			break;
		}
		case CET_OTHERSERVER:
		{
			pastTcpConn = m_pstOtherConn;
			break;
		}

		// 单独处理游戏代理服务器
		case CET_PROXYSERVER:
		{
			pastTcpConn = m_astProxyConn.GetTcpConnByEntityID( nGameID );
			if ( pastTcpConn == NULL )
			{
				return NULL;
			}
			if( ulSrcAddr != pastTcpConn->GetConnAddr() )				
			{
				return NULL;
			}
			return pastTcpConn;
		}
		case CET_DUNGEON:
		{
			pastTcpConn = m_pstDunGeon;
			break;	
		}
		default:
		{
			LOG_ERROR("default", "Error unknow SrcFe:%d \n", shSrcFE);
			return  NULL;
		}
	}

	if(shSrcID <= 0 || shSrcID >= MaxServer(shSrcFE))
	{
		LOG_ERROR("default", "Invalid srcid %d, out of size\n", shSrcID );
		return NULL;
	}

	if (pastTcpConn+shSrcID == NULL)
	{
		return NULL;
	}

	if( (ulSrcAddr == ( pastTcpConn+shSrcID )->GetConnAddr()) 
		&& (shSrcID == ( pastTcpConn+shSrcID )->GetEntityID()))
	{
		return pastTcpConn + shSrcID;
	}

	/*
	for( int i = 0; i < MAXSVRNUMBER; i++ )
	{
		//if( (ulSrcAddr == (pastTcpConn+i)->GetConnAddr()) && (shSrcID == (pastTcpConn+i)->GetEntityID()-1) )
		if( (ulSrcAddr == (pastTcpConn+i)->GetConnAddr()) && (shSrcID == (pastTcpConn+i)->GetEntityID()))
		{
			return pastTcpConn+i;
		}
	}
	*/

	return NULL;
}

int CProxyCtrl::CheckRunFlags()
{
	if( g_byRunFlag.IsFlagSet(FLG_CTRL_QUIT) )
	{
		LOG_NOTICE("default", "xyj-proxyserver exit !");

		//停止所有的线程
		m_pMainHandle->StopThread();
		m_pDBHandle->StopThread();
		m_pOtherHandle->StopThread();
		m_pProxyHandle->StopThread();	// 代理服务器
		m_pDungeonHandle->StopThread();					// 全局游戏服务器
		m_pGlobalProxy->StopThread();		
		LOG_NOTICE("default", "All threads stopped, proxy control quit.\n");

		exit(0);
	}

	if( g_byRunFlag.IsFlagSet(FLG_CTRL_RELOAD) )
	{
		const char* szCfgFile = "../config/proxysvrd.xml";
		
		g_byRunFlag.ClearFlag(FLG_CTRL_RELOAD);

		LOG_NOTICE("default", "xyj-proxyserver reload config ...");
		//printf( "Have got command to reload cfg , now reloading ...\n" );

		if( ReadCfg(szCfgFile) )
		{
			LOG_ERROR("default", "Read config failed.\n");
			printf( "Read config failed\n" );
			return -1;
		}

		/*
		if( LoadConnInfoFromFile(cet_gamesvr,
			(CMyTCPConn *)m_astMainConn, m_stProxyCfg.m_iMainSvrNum) )
		{
			LOG_ERROR("Load main servers failed.\n");
			return -1;
		}
		
		if( LoadConnInfoFromFile(cet_dbsvr,
			(CMyTCPConn *)m_astDBConn, m_stProxyCfg.m_iDBSvrNum) )
		{
			LOG_ERROR("Load db servers failed.\n");
			return -1;
		}

		if( LoadConnInfoFromFile(cet_other,
			(CMyTCPConn *)m_astOtherConn, m_stProxyCfg.m_iOtherEntityNum) )
		{
			LOG_ERROR("Load other entitys failed.\n");
			return -1;
		}

		if( LoadHashMapFromFile() )
		{
			LOG_ERROR("Reload hash map failed.\n");
			return -1;
		}

		if(LoadMRTFromFile())
		{
			LOG_ERROR("Reload main route table failed.\n");
			return -1;
		}
		*/

		LOG_INFO("default", "Successfully refreshed config.\n");
		printf( "reload cfg completed!\n" );
	}

	return 0;
}

int CProxyCtrl::CheckTimer()
{
	unsigned int tNowCount = LINEKONG::GetTickCount();
	unsigned int tInterval = tNowCount - mLastTickCount;

	if ( tInterval < 100 )
	{
		return 0;
	}		
	mLastTickCount = tNowCount;	
	return 0;
}
int CProxyCtrl::CheckConnRequest()
{
	fd_set fds_read;
	timeval tvListen;
	int i = 0, iTmp;
	int iListenSocketFD = -1;
	int iNewSocketFD = -1;
	struct sockaddr_in stConnAddr;
	TName szConnAddr;
	int iMaxSocketFD = -1;
	
	socklen_t iAddrLength = sizeof(stConnAddr);

	tvListen.tv_sec = 0;
	tvListen.tv_usec = 100000;
	FD_ZERO( &fds_read );
	iListenSocketFD = m_stListenSocket.GetSocketFD();

	FD_SET( iListenSocketFD, &fds_read );
	iMaxSocketFD = iListenSocketFD;

	for(i = 0; i < m_iCurrentUnRegisterNum; ++i)
	{
		FD_SET( m_astUnRegisterInfo[i].m_iSocketFD, &fds_read );
		if(m_astUnRegisterInfo[i].m_iSocketFD > iMaxSocketFD)
		{
			iMaxSocketFD = m_astUnRegisterInfo[i].m_iSocketFD;
		}	
	}
	
	iTmp = select( iMaxSocketFD + 1, &fds_read, NULL, NULL, &tvListen );
		
	if( iTmp <= 0 )
	{
		if( iTmp < 0 )
		{
			LOG_ERROR("default", "Select error, %s.\n", strerror(errno));
		}
		return iTmp;
	}

	if ( FD_ISSET(iListenSocketFD, &fds_read) )
	{
		iNewSocketFD = accept(iListenSocketFD, (struct sockaddr *)&stConnAddr, &iAddrLength);
		//性能优化分析
		/* 正式部署的时候可以将此注释打开, 目前测试都是在同一台主机上
		if(stConnAddr.sin_addr.s_addr  == inet_addr("127.0.0.1"))
		{
			//外网经常收到127.0.0.1的连接，所以直接丢弃这些
			close(iNewSocketFD);
		}
		else
		{
		*/
			SockAddrToString( &stConnAddr, (char *)szConnAddr );
			LOG_INFO("default", "Get a conn request from %s socket fd %d.\n", szConnAddr, iNewSocketFD);

			 if(m_iCurrentUnRegisterNum >= MAX_UNREGISTER_NUM)
			 {
			 	LOG_ERROR("default", "Error there is no empty space(cur num: %d) to record.\n", m_iCurrentUnRegisterNum);
				close(iNewSocketFD);
			 }
			else
			{
				m_astUnRegisterInfo[m_iCurrentUnRegisterNum].m_iSocketFD = iNewSocketFD;
				m_astUnRegisterInfo[m_iCurrentUnRegisterNum].m_ulIPAddr = stConnAddr.sin_addr.s_addr;
				m_astUnRegisterInfo[m_iCurrentUnRegisterNum].m_tAcceptTime = time(NULL);
				m_iCurrentUnRegisterNum++;
			}
		//}
	}
	
	//注意这里是从后向前扫描，因为在此过程中
	//有可能会删除当前元素并把最后一个填充到此处
	for(i = m_iCurrentUnRegisterNum-1; i >= 0; --i)
	{
		if ( FD_ISSET(m_astUnRegisterInfo[i].m_iSocketFD, &fds_read) )
		{
			ReceiveAndProcessRegister(i);
		}
	}
	return 1;
}

/********************************************************
  Function:     ReceiveAndProcessRegister
  Description:  接收并处理注册消息
  Input:          iUnRegisterIdx:  未注册结构索引
  Output:      
  Return:       0 :   成功 ，其他失败
  Others:		
********************************************************/
int CProxyCtrl::ReceiveAndProcessRegister( int iUnRegisterIdx )
{
	char acTmpBuf[MAX_TMP_BUF_LEN] = {0};
	int iRecvedBytes = 0;
	CProxyHead stTmpProxyHead;
	CMyTCPConn* pAcceptConn = NULL;
	int iAcceptRst = 0;

	if(iUnRegisterIdx < 0 || iUnRegisterIdx >= m_iCurrentUnRegisterNum)
	{
		LOG_ERROR("default", "Error in ReceiveAndProcessRegister, Unregister idx(%d) is invalid\n", iUnRegisterIdx);
		return -1;		
	}

	int iSocketFD = m_astUnRegisterInfo[iUnRegisterIdx].m_iSocketFD;
	u_long ulIPAddr = m_astUnRegisterInfo[iUnRegisterIdx].m_ulIPAddr;

	//iRecvedBytes = recv(iSocketFD, acTmpBuf, sizeof(acTmpBuf), 0);
	iRecvedBytes = recv(iSocketFD, acTmpBuf, 2, 0);

	if (iRecvedBytes == 2)
	{
		iRecvedBytes = recv(iSocketFD, acTmpBuf + 2, *((unsigned short*)(acTmpBuf)) - 2 , 0);
	}

	if( iRecvedBytes == 0 )
	{
		LOG_ERROR("default", "The remote site may closed this conn, fd = %d, errno = %d.\n", iSocketFD, errno);
		DeleteOneUnRegister(iUnRegisterIdx);
		close(iSocketFD);
		return -1;
	}
	if( iRecvedBytes < 0 )
	{
		if(errno != EAGAIN)
		{
			LOG_ERROR("default", "Error in read conn, fd = %d, errno = %d.\n", iSocketFD, errno);
			DeleteOneUnRegister(iUnRegisterIdx);
			close(iSocketFD);
		}
		return -1;
	}

	iRecvedBytes += 2;

	DeleteOneUnRegister(iUnRegisterIdx);
	
	// 先注释掉，需要修改
	/*
	if( iRecvedBytes < BASEPXYHEADLENGTH)
	{
		LOG_ERROR("default", "Error not enough len, fd = %d, recv len = %d.\n", iSocketFD, iRecvedBytes);
		close(iSocketFD);
		return -1;	
	}
	*/


	// 接收总长度
	unsigned short iPkgSize = *((unsigned short*)(acTmpBuf));
	assert(iPkgSize == iRecvedBytes);

	// CProxyHead长度
	unsigned short iProxyHeadSize = *((unsigned short*)(acTmpBuf+2));
	if (stTmpProxyHead.ParseFromArray(acTmpBuf+4, iProxyHeadSize) == false)
	{       
		LOG_ERROR("default", "decode error, stream_length is %d", iProxyHeadSize );
		return -1;
	}
	else
	{
		LOG_DEBUG("default", "Recv(%d): %s", iRecvedBytes, stTmpProxyHead.ShortDebugString().c_str());
	}   

	/*
	if( stTmpProxyHead.Decode(acTmpBuf, iRecvedBytes))
	{
		LOG_ERROR("default", "Error decode proxy head failed, fd = %d, recv len = %d.\n", iSocketFD, iRecvedBytes);
		close(iSocketFD);
		return -1;
	}
	*/

	if( ( ( stTmpProxyHead.dstfe() != CET_PROXYSERVER ) &&  ( stTmpProxyHead.dstfe() != CET_GLOBALPROXY ) ) || (stTmpProxyHead.opflag() != CMD_REGSERVER))
	{
		LOG_ERROR("default", "Error proxy head para is invalid, fd = %d, dstFE = %d, Opflag= %d.\n", 
			iSocketFD, stTmpProxyHead.dstfe(), stTmpProxyHead.opflag());
		close(iSocketFD);
		return -1;
	}

	pAcceptConn = GetConnByAddrAndID(stTmpProxyHead.srcfe(), stTmpProxyHead.srcid(), ulIPAddr, stTmpProxyHead.srcgameid() );
	if(pAcceptConn == NULL)
	{
		struct sockaddr_in stConnAddr;
		stConnAddr.sin_addr.s_addr = ulIPAddr;
		stConnAddr.sin_port = 0;

		char szIP[64] = {0};
		SockAddrToString( &stConnAddr, szIP );

		LOG_ERROR("default", "Source FE(%d) , ID(%d) or addr(%s) is not valid of socket fd(%d) .\n", 
			stTmpProxyHead.srcfe(), stTmpProxyHead.srcid(), szIP, iSocketFD);
			
		close(iSocketFD);
		return -1;
	}
	

	//iAcceptRst = pAcceptConn->GetSocket()->Accept(iNewSocketFD);
	iAcceptRst = pAcceptConn->EstConn(iSocketFD);

	if( iAcceptRst < 0 )
	{
		if( iAcceptRst == -2 )
		{
			LOG_ERROR("default", "The server had connected, ignore the conn request (fd: %d).\n", iSocketFD);
		}
		close(iSocketFD);
		return -1;
	}

	LOG_INFO("default", "Conn(%d, %d) connected OK, the FD is %d.\n",
		pAcceptConn->GetEntityType(), pAcceptConn->GetEntityID(), iSocketFD);

	// 
	switch( pAcceptConn->GetEntityType() )
	{
		case CET_GAMESERVER:
		{
			LOG_DEBUG("default", "Wake up server thread of main svrs.\n");
		
			pAcceptConn->SetConnState(ENTITY_ON);
		
			m_pMainHandle->WakeUp();
			break;
		}
		case CET_DBSERVER:
		{
			LOG_DEBUG("default", "Wake up server thread of db svrs.\n");
			
			pAcceptConn->SetConnState(ENTITY_ON);
			
			m_pDBHandle->WakeUp();
			break;
		}
		case CET_OTHERSERVER:
		{
			LOG_DEBUG("default", "Wake up server thread of other svrs.\n");
		
			pAcceptConn->SetConnState(ENTITY_ON);
		
			m_pOtherHandle->WakeUp();
			break;
		}
		case CET_PROXYSERVER:
		{
			LOG_DEBUG("default", "Wake up server thread of other ProxyServer.\n");

			pAcceptConn->SetConnState(ENTITY_ON);

			m_pProxyHandle->WakeUp();
			break;
		}

		case CET_DUNGEON:
		{
			LOG_DEBUG("default", "Wake up server thread of  Dungeon.\n");
			pAcceptConn->SetConnState(ENTITY_ON);
			m_pDungeonHandle->WakeUp();
			break;
		}
		default:
		{
			LOG_ERROR("default", "Unknown error, the accepted conn has invalid entity type %d\n", pAcceptConn->GetEntityType());
			return -1;
		}
	}	//end switch


	return 0;
}

/********************************************************
  Function:     DeleteOneUnRegister
  Description:  删除一个未注册的连接
  Input:          iUnRegisterIdx:  未注册结构索引
  Output:      
  Return:       0 :   成功 ，其他失败
  Others:		
********************************************************/
int CProxyCtrl::DeleteOneUnRegister(int iUnRegisterIdx)
{
	if(iUnRegisterIdx < 0 || iUnRegisterIdx >= m_iCurrentUnRegisterNum)
	{
		LOG_ERROR("default", "Error in DeleteOneUnRegister, Unregister idx(%d) is invalid\n", iUnRegisterIdx);
		return -1;		
	}

	m_iCurrentUnRegisterNum--;
	if((m_iCurrentUnRegisterNum > 0) && (iUnRegisterIdx < m_iCurrentUnRegisterNum))
	{
		m_astUnRegisterInfo[iUnRegisterIdx] = m_astUnRegisterInfo[m_iCurrentUnRegisterNum];
	}

	return 0;
}

/********************************************************
  Function:     CheckRoutines
  Description:  定时检查
  Input:         
  Output:      
  Return:       0 :   成功 ，其他失败
  Others:		
********************************************************/
int CProxyCtrl::CheckRoutines()
{
	time_t tCurrentTime = 0;
	time( &tCurrentTime );

	if ( tCurrentTime - m_tLastCheckTime >= CHECK_INTERVAL_SECONDS )
	{
		m_tLastCheckTime = tCurrentTime;
		// 检查全局服务器的连接状态
		CheckGloalProxyStatus();
		for(int i = m_iCurrentUnRegisterNum-1; i >= 0; --i)
		{
			if(tCurrentTime - m_astUnRegisterInfo[i].m_tAcceptTime > CHECK_TIMEOUT_SECONDS)
			{
				int iSocketFD = m_astUnRegisterInfo[i].m_iSocketFD;
				LOG_ERROR("default", "Wait register timeout so close it, fd = %d.\n", iSocketFD);
				DeleteOneUnRegister(i);
				close(iSocketFD);
			}
		}
 
		PrintStatistic();		
	}

	return 0;
}

int CProxyCtrl::PrepareToRun()
{
	if(m_stListenSocket.CreateServer(m_stProxyCfg.m_iProxyPort))
	{
		return -1;
	}

	m_pMainHandle = new CProxyHandle;
	m_pDBHandle = new CProxyHandle;
	m_pOtherHandle = new CProxyHandle;

	m_pGlobalProxy = new CProxyHandle;
	m_pProxyHandle = new CProxyHandle;
	m_pDungeonHandle = new CProxyHandle;

	if( !m_pMainHandle || !m_pDBHandle || !m_pOtherHandle || !m_pGlobalProxy || !m_pProxyHandle || !m_pDungeonHandle )
	{
		return -1;
	}

	m_pMainHandle->Initialize(CET_GAMESERVER, (CMyTCPConn *)m_pstMainConn, 
					(CMyTCPConn *)m_pstDBConn, (CMyTCPConn *)m_pstOtherConn, &m_astProxyConn, (CMyTCPConn *)m_pstDunGeon, &m_astGlobalProxy, &m_stProxyCfg);
	m_pDBHandle->Initialize(CET_DBSERVER, (CMyTCPConn *)m_pstMainConn, 
					(CMyTCPConn *)m_pstDBConn, (CMyTCPConn *)m_pstOtherConn, &m_astProxyConn, (CMyTCPConn *)m_pstDunGeon, &m_astGlobalProxy, &m_stProxyCfg);
	m_pOtherHandle->Initialize(CET_OTHERSERVER, (CMyTCPConn *)m_pstMainConn, 
					(CMyTCPConn *)m_pstDBConn, (CMyTCPConn *)m_pstOtherConn, &m_astProxyConn, (CMyTCPConn *)m_pstDunGeon, &m_astGlobalProxy, &m_stProxyCfg);
	m_pGlobalProxy->Initialize(CET_GLOBALPROXY, (CMyTCPConn *)m_pstMainConn, 
					(CMyTCPConn *)m_pstDBConn, (CMyTCPConn *)m_pstOtherConn, &m_astProxyConn, (CMyTCPConn *)m_pstDunGeon, &m_astGlobalProxy, &m_stProxyCfg);
	m_pProxyHandle->Initialize(CET_PROXYSERVER, (CMyTCPConn *)m_pstMainConn, 
					(CMyTCPConn *)m_pstDBConn, (CMyTCPConn *)m_pstOtherConn, &m_astProxyConn, (CMyTCPConn *)m_pstDunGeon, &m_astGlobalProxy, &m_stProxyCfg);
	m_pDungeonHandle->Initialize(CET_DUNGEON, (CMyTCPConn *)m_pstMainConn, 
					(CMyTCPConn *)m_pstDBConn, (CMyTCPConn *)m_pstOtherConn, &m_astProxyConn, (CMyTCPConn *)m_pstDunGeon, &m_astGlobalProxy, &m_stProxyCfg);

	if( m_pMainHandle->CreateThread() )
	{
		return -1;
	}

	if( m_pDBHandle->CreateThread() )
	{
		return -1;
	}

	if( m_pOtherHandle->CreateThread() )
	{
		return -1;
	}
	
	if ( m_pGlobalProxy->CreateThread() )
	{
		return -1;
	}
	
	if ( m_pDungeonHandle->CreateThread() )
	{
		return -1;
	}

	if ( m_pProxyHandle->CreateThread() )
	{
		return -1;
	}	
	return 0;
}

int CProxyCtrl::Run()
{

	while(True)
	{
		CheckRunFlags();

		CheckConnRequest();

		CheckRoutines();	
	}	//end while

	return 0;
}




int CProxyCtrl::PrintStatistic( )
{
	FILE *fpStatistic = NULL;
	char fname[128];
	TStruTime stTempNow;
	char szCurDate[32];
	char szCurDateTime[32];
	char szCurMon[32];
	char szStatisticDir[64];

	GetStructTime( time(NULL), &stTempNow );

	snprintf(szCurMon, sizeof(szCurMon)-1, "%04d-%02d", stTempNow.m_iYear, stTempNow.m_iMon);
	snprintf(szCurDate, sizeof(szCurDate)-1,"%04d-%02d-%02d", stTempNow.m_iYear, stTempNow.m_iMon, stTempNow.m_iDay);
	snprintf(szCurDateTime, sizeof(szCurDateTime)-1,"%04d-%02d-%02d %02d:%02d:%02d", stTempNow.m_iYear, stTempNow.m_iMon, stTempNow.m_iDay, stTempNow.m_iHour, stTempNow.m_iMin, stTempNow.m_iSec);
	snprintf(szStatisticDir, sizeof(szStatisticDir)-1,"../statistic/%s", szCurMon);

	snprintf( fname, sizeof(fname)-1,"%s/log-%s.txt", szStatisticDir, szCurDate);
	DIR* dpStatistic = opendir( szStatisticDir);
	if ( !dpStatistic )
	{
		if ( mkdir(szStatisticDir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) )
		{
			return -1;
		}
	}
	else
	{
		closedir( dpStatistic);
	}
	fpStatistic = fopen( fname , "a+" );

	if ( !fpStatistic )
	{
		return -1;
	}

	fprintf(fpStatistic, "\n=======================%s======================\n", szCurDateTime);


	fprintf( fpStatistic, "MainHandle : Recv Code: %8d , Len : %10d\n" ,
		m_pMainHandle->GetProxyStat().m_nTransferCodeNum, m_pMainHandle->GetProxyStat().m_nTransferCodeLength );
	memset( &m_pMainHandle->GetProxyStat(), 0, sizeof(STProxyStat) );

	fprintf( fpStatistic, "DBHandle   : Recv Code: %8d , Len : %10d\n" ,
		m_pDBHandle->GetProxyStat().m_nTransferCodeNum, m_pDBHandle->GetProxyStat().m_nTransferCodeLength );
	memset( &m_pDBHandle->GetProxyStat(), 0, sizeof(STProxyStat) );
	
	fprintf( fpStatistic, "OtherHandle: Recv Code: %8d , Len : %10d\n" , 
		m_pOtherHandle->GetProxyStat().m_nTransferCodeNum, m_pOtherHandle->GetProxyStat().m_nTransferCodeLength );
	memset( &m_pOtherHandle->GetProxyStat(), 0, sizeof(STProxyStat) );

	fprintf( fpStatistic, "ProxyHandle: Recv Code: %8d , Len : %10d\n" , 
		m_pProxyHandle->GetProxyStat().m_nTransferCodeNum, m_pProxyHandle->GetProxyStat().m_nTransferCodeLength );
	memset( &m_pProxyHandle->GetProxyStat(), 0, sizeof(STProxyStat) );

	fprintf( fpStatistic, "GlobalProxy: Recv Code: %8d , Len : %10d\n" , 
		m_pGlobalProxy->GetProxyStat().m_nTransferCodeNum, m_pGlobalProxy->GetProxyStat().m_nTransferCodeLength );
	memset( &m_pGlobalProxy->GetProxyStat(), 0, sizeof(STProxyStat) );

	fprintf( fpStatistic, "DungeonHandle: Recv Code: %8d , Len : %10d\n" , 
		m_pDungeonHandle->GetProxyStat().m_nTransferCodeNum, m_pDungeonHandle->GetProxyStat().m_nTransferCodeLength );
	memset( &m_pDungeonHandle->GetProxyStat(), 0, sizeof(STProxyStat) );

	fclose(fpStatistic);

	return 0;
}

int CProxyCtrl::CheckGloalProxyStatus()
{
	if ( m_stProxyCfg.m_nFlag == 1 )
	{
		return 0;
	}
	
	// 不用连接全局代理服务器	
	if ( m_astGlobalProxy.GetSocket()->GetStatus() == tcs_connected && m_astGlobalProxy.GetSocket()->GetSocketFD() > 0 )
	{
		return 0;
	}	
	
	// 需要连接全局代理服务器
	int tRet = m_astGlobalProxy.ConnectToServer( );
	if ( tRet )
	{
		LOG_WARN( "default","globalproxy can't be connected" );
		return 0;
	}	

	// 注册到全局服务器
	if ( Regist2GlobalProxy() == false )
	{
		return -1;
	}	
	else
	{
		m_astGlobalProxy.SetConnState( ENTITY_ON );	
		m_pGlobalProxy->WakeUp();
	}
	return 0;
}


bool CProxyCtrl::Regist2GlobalProxy()
{
	CProxyHead tHead;	
	pbmsg_setproxy(&tHead, CET_PROXYSERVER, CET_GLOBALPROXY, m_stProxyCfg.m_nGameID, 1, TRANS_P2P, CMD_REGSERVER);
	tHead.set_srcgameid( m_stProxyCfg.m_nGameID );	
	char message_buffer[MAX_TMP_BUF_LEN];
	unsigned short message_length  = tHead.ByteSize() + 4;
	*((unsigned short*) message_buffer)  = message_length;
	*((unsigned short*) (message_buffer+2))  = tHead.ByteSize();

	if(tHead.SerializeToArray(message_buffer+4, message_length - 4))
	{
		LOG_DEBUG("default", "regist to proxyserver (%d): head=[%d:%s]",  message_length , tHead.ByteSize(), tHead.ShortDebugString().c_str());
		if( m_astGlobalProxy.GetSocket()->SendOneCode(message_length, (BYTE*)message_buffer) == 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}else
	{
		LOG_INFO("default", "regist to GlobalProxy error");
		return false;
	}
}

int CProxyCtrl::KeepAliveToGlobalProxy()
{		
	CProxyHead tHead;	
	pbmsg_setproxy( &tHead, CET_PROXYSERVER, CET_GLOBALPROXY, m_stProxyCfg.m_nGameID, 1, TRANS_P2P, CMD_KEEPALIVE );
	tHead.set_srcgameid( m_stProxyCfg.m_nGameID );	
	char message_buffer[MAX_TMP_BUF_LEN];
	unsigned short message_length  = tHead.ByteSize() + 4;
	*((unsigned short*) message_buffer)  = message_length;
	*((unsigned short*) (message_buffer+2))  = tHead.ByteSize();

	if(tHead.SerializeToArray(message_buffer+4, message_length - 4))
	{
		LOG_DEBUG("default", "keep alive to proxyserver (%d): head=[%d:%s]",  message_length , tHead.ByteSize(), tHead.ShortDebugString().c_str());
		if( m_astGlobalProxy.GetSocket()->SendOneCode(message_length, (BYTE*)message_buffer) == 0 )
		{
			return 0;
		}
		else
		{
			// 关闭连接
			return -1;
		}
	}
	else
	{
		LOG_INFO("default", "keep alive to GlobalProxy error");
		return -1;
	}
}

int CProxyCtrl::CheckAliveTime( unsigned int nTime )
{
	mAliveTimer -= nTime;
	if ( mAliveTimer <= 0 )
	{
		CheckGloalProxyStatus();
		//KeepAliveToGlobalProxy();
		mAliveTimer = GLOBALPROXY_ALIVE_TIME;
	}	
	return 0;
}

void CProxyCtrl::pbmsg_setproxy(CProxyHead* pProxy, unsigned int nSrcFE, unsigned int nDstFE, unsigned int nSrcID, 
					unsigned int nDstID, EMTransType emTransTp, EMProxyCmd emProxyCmd, unsigned char* szOption, unsigned int nOptlen)
{
	if (nSrcFE) pProxy->set_srcfe( nSrcFE );
	if (nDstFE) pProxy->set_dstfe( nDstFE );
	if (nSrcID) pProxy->set_srcid( nSrcID );
	if (nDstID) pProxy->set_dstid( nDstID );

	pProxy->set_transfertype(emTransTp);
	pProxy->set_opflag(emProxyCmd);

	if (szOption && nOptlen)
	{
		pProxy->set_optlength(nOptlen);
		pProxy->set_option(szOption, nOptlen);
	}
}