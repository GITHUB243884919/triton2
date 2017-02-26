#include "dungeonlogic.h"
#include "lk_assert.h"
#include "dungeonmodule.h"
#include "dunobjmanager.h"

template<> CDungeonServer *CSingleton<CDungeonServer>::spSingleton = NULL;

CSharedMem * CDungeonServer::m_pShareMem = NULL;

// ***************************************************************
// Function:      CDungeonServer
// Description:   构造函数
// Others:         
// Date:           03/22/2011
// 
//***************************************************************
CDungeonServer::CDungeonServer()
{
	mLastTickCount = LINEKONG::GetTickCount();
}

// ***************************************************************
// Function:      ~CDungeonServer
// Description:  
// Others:         
// Date:           03/22/2011
// 
//***************************************************************
CDungeonServer::~CDungeonServer()
{
}


// ***************************************************************
// Function:     Initialize
// Description:  初始化函数
// Others:         
// Date:           03/22/2011
// 
//***************************************************************
int CDungeonServer::Initialize()
{
	mTimerOfKeepAlive = CWTimer(KEEPALIVE_TIME);
	ClearRunFlag();
	mServerConfig.mGlobalProxyAddr.clear();

	CDungeonLogic::m_pShareMem = m_pShareMem;
	CDungeonLogic *tpLogic = new CDungeonLogic;
	LK_ASSERT( tpLogic != NULL, return -1 );
	LOG_NOTICE( "default", "CDungeonLogic addr %6x", tpLogic);
	CLogicModule::mShmPtr = m_pShareMem;	
	CDunobjManager::m_pShareMem = m_pShareMem;
	CDunobjManager *tpObjManager = new CDunobjManager;
	LK_ASSERT( tpObjManager != NULL, return -1 )
	LOG_NOTICE( "default", "CDunobjManager addr %6x", tpObjManager);
	CModuleManager *tpModuleManager = new CModuleManager();
	LK_ASSERT( tpModuleManager != NULL, return -1 );

	CDungeonServer::GetSingleton().ReadConfig();	
	return 0;
}

// ***************************************************************
// Function:     new
// Description:  重载new函数
// Others:         
// Date:         03/22/2011
// 
//***************************************************************
void * CDungeonServer::operator new ( size_t nSize )
{
	return ( void* ) m_pShareMem->CreateSegment( nSize );
}

// ***************************************************************
// Function:     delete
// Description:  重载delete函数
// Others:         
// Date:         03/22/2011
// 
//***************************************************************
void CDungeonServer::operator delete( void *pBuffer )
{

}

// ***************************************************************
// Function:     CountSize
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
unsigned int CDungeonServer::CountSize()
{	
	unsigned int tTempSize = 0;
	unsigned int tSize = 0;

	tTempSize = sizeof( CDungeonServer );
	tSize += tTempSize;
	LOG_INFO( "default", "CDungeonServer::CountSize() = %d", tTempSize );

	tTempSize = sizeof( CDungeonLogic );
	tSize += tTempSize;
	LOG_INFO( "default", "CDungeonLogic::CountSize() = %d", tTempSize );

	tTempSize += sizeof( CDungeonModule );
	tSize += tTempSize;
	LOG_INFO( "default", "CDungeonModule::CountSize() = %d", tTempSize );

	tTempSize = sizeof( CDunobjManager );
	tSize += tTempSize;
	LOG_INFO( "default", "CDunobjManager::CountSize() = %d", tTempSize );

	return tSize;
}	

// ***************************************************************
// Function:     RecvServerMsg
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
void CDungeonServer::RecvServerMsg()
{
	int tSockStatus =  mGlobalProxy.GetSocket()->GetStatus();
	int tSocketFd = mGlobalProxy.GetSocket()->GetSocketFD();
	if ( tSockStatus != tcs_connected ||tSocketFd <= 0 )
	{
		// 睡眠 1 ms 
		struct timespec tv;
		tv.tv_sec = 0;
		tv.tv_nsec = 1000; 
		nanosleep( &tv, NULL );
		return;
	}
	int tMaxFd = tSocketFd;
	fd_set tReadFd;
	FD_ZERO( &tReadFd );
	FD_SET( mGlobalProxy.GetSocket()->GetSocketFD(), &tReadFd );
	timeval tTimeval;
	tTimeval.tv_sec = 0;
	tTimeval.tv_usec = 1000;
	int tNum = select( tMaxFd + 1, &tReadFd, NULL, NULL, &tTimeval );
	if ( tNum <= 0 )
	{
		return;
	}
	if ( FD_ISSET( tSocketFd, &tReadFd ) == true && tSocketFd > 0 )
	{
		char tCodeBuff[MAX_CODE_LEN] = { 0 };	
		if( mGlobalProxy.GetSocket()->RecvData() < 0 )
		{
			LOG_ERROR( "default", "[%s:%d] recv proxy code failed", __FUNCTION__, __LINE__ );
			return;
		}
		unsigned short tBuffLen = sizeof( tCodeBuff );
		int iRet = -1;

		CProxyHead tProxyHead;
		CMessage tMessage;
		
		while ( 1 )
		{
			tBuffLen = sizeof( tCodeBuff );
			iRet = mGlobalProxy.GetSocket()->GetOneCode( tBuffLen, (byte *)tCodeBuff );
			if ( iRet <= 0  )
			{
				//LOG_ERROR( "default", "Get One Code From Router server Faild %d", iRet );
				break;
			}

			if( tBuffLen == 0 || tBuffLen > sizeof(tCodeBuff))
			{
				LOG_ERROR( "default", "Code Len %d impossible", tBuffLen );
				break;
			}

			int decode_ret = pbmsg_getheadmsg( (unsigned char*)tCodeBuff, tBuffLen  , &tProxyHead, &tMessage );
			if( decode_ret <= 0 )
			{
				LOG_ERROR("default", "decode router message failed %d", decode_ret );
				continue;
			}

			if (tProxyHead.has_opflag() && CMD_KEEPALIVE == tProxyHead.opflag() && tProxyHead.srcfe() == CET_GLOBALPROXY )
			{
#ifdef _DEBUG_
				LOG_DEBUG("default", "RecvFromProxy: %s", tProxyHead.ShortDebugString().c_str());
#endif				
				continue;
			}

			if(tMessage.mutable_msghead()->has_srcfe() == false)
			{
				tMessage.mutable_msghead()->set_srcfe( FE_PROXYSERVER );
			}


			CDungeonLogic::GetSingletonPtr()->OnServerMessage( &tMessage,tProxyHead.srcgameid(), tProxyHead.srcfe(), tProxyHead.srcid() );
		}
	}
}

// ***************************************************************
// Function:     ReadConfig
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
void CDungeonServer::ReadConfig()
{
	ReadServerConfig();
}

// ***************************************************************
// Function:     ReadConfig
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
void CDungeonServer::ReadServerConfig()
{
	TiXmlDocument tDoc;
	if ( tDoc.LoadFile( "../config/ip.xml" ) == false )
	{
		printf( "read ip config error!" );
		LOG_ERROR( "default","read ip config error!" );
		exit( 0 );		
	}
	TiXmlElement *tpRoot = tDoc.RootElement();
	if ( tpRoot == NULL )
	{
		printf( "root ip config error!" );
		LOG_ERROR( "default","root ip config error!" );
		exit( 0 );
	}
	if ( tpRoot->Attribute("id") != NULL )
	{
		mServerConfig.mServerID = atoi( tpRoot->Attribute("id") );
	}
	TiXmlElement *tpGlobalServerConfig = tpRoot->FirstChildElement( "global_server" );
	if ( tpGlobalServerConfig == NULL )
	{
		printf( "global_server config error!" );
		LOG_ERROR( "default","global_server config error!" );
		exit( 0 );
	}	
	if ( tpGlobalServerConfig->Attribute( "Address" ) == NULL )
	{
		printf( "get global_server address error!" );
		LOG_ERROR( "default","get global_server address error!" );
		exit( 0 );
	}
	mServerConfig.mGlobalProxyAddr = lk::CString64( tpGlobalServerConfig->Attribute( "Address" ) );	
}

// ***************************************************************
// Function:     CheckTimer
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
void CDungeonServer::CheckTimer()
{
	unsigned int tNowCount = LINEKONG::GetTickCount();
	unsigned int tInterval = tNowCount - mLastTickCount;

	if ( tInterval < 100 )
	{
		return ;
	}	
	CDungeonLogic::GetSingleton().OnTimer( tNowCount, tInterval );
	CheckProxyAlive( tInterval );
	mLastTickCount = tNowCount;	
}

int CDungeonServer::CheckProxyAlive( int nInterval )
{
	if ( mTimerOfKeepAlive.Elapse( nInterval ) == false )
	{		
		if ( mGlobalProxy.GetSocket()->GetStatus() == tcs_connected ||mGlobalProxy.GetSocket()->GetSocketFD() > 0 )
		{
			return 0;
		}
		LOG_INFO( "default","globalproxy not connected,now connect to it!" );
		if( Connect2Proxy() == true )
		{
			if ( Regist2Proxy() == true )
			{
				KeepAliveToProxy();
			}
			else
			{
				printf( "cannot regist to proxy err!" );
				LOG_ERROR( "default","cannot regist to proxy err!" );		
			}
		}
		return 0;
	}	
}

// ***************************************************************
// Function:     OnLaunch
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
int CDungeonServer::OnLaunch()
{
	// 连接服务器
	if( Connect2Server() < 0 )
	{	
		LOG_WARN( "default","connetct to server faild!" );
		return -1;
	} 
	return 0;
}

// ***************************************************************
// Function:     PrepareToRun
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
int CDungeonServer::PrepareToRun()
{	
	// 加载逻辑模块
	m_pShareMem->SetInitMode( SHM_INIT );
	CObj::msCreateMode = 1;		
	CDungeonLogic::GetSingleton().Launch();	
	return 0;
}

// ***************************************************************
// Function:     Run
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
void CDungeonServer::Run()
{
	while ( true )
	{
		// 检查运行标志( 重新载入配置文件/停止服务器 )
		CheckRunFlags();

		// 检查定时器
		CheckTimer();

		// 接收服务器消息到消息队列(单次循环最大接收N个)
		RecvServerMsg( );		
	}
}

// ***************************************************************
// Function:     CheckRunFlags
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
void CDungeonServer::CheckRunFlags()
{
	if ( mRunFlag == CDungeonServer::EFLG_CTRL_RELOAD )
	{
		CDungeonLogic::GetSingleton().ReadConfig();
		ClearRunFlag();
	}
	else if ( mRunFlag == CDungeonServer::EFLG_CTRL_QUIT )
	{		
		exit( 0 );
	}
}
// ***************************************************************
// Function:     ShutDown
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
int CDungeonServer::ShutDown( )
{
}

// ***************************************************************
// Function:     Connect2Proxy
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
bool CDungeonServer::Connect2Proxy()
{
	char tIP[ 64 ] = {0};
	short tPort = 0;

	SplitSockAddr( mServerConfig.mGlobalProxyAddr.c_str(), tIP, tPort );
	mGlobalProxy.Initialize( FE_PROXYSERVER, mServerConfig.mServerID, inet_addr( tIP ), tPort );

	// 优先使用内网IP	
	if ( mGlobalProxy.ConnectToServer( tIP ) )
	{
		LOG_ERROR( "default", "Connect to ProxyServer(%s) use %s Faild", mServerConfig.mGlobalProxyAddr.c_str(), tIP );
		return false;
	}

	return true;
}

// ***************************************************************
// Function:     Connect2Server
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
int CDungeonServer::Connect2Server()
{
	if( Connect2Proxy() == true )
	{
		if ( Regist2Proxy() == true )
		{
			KeepAliveToProxy();
		}
		else
		{
			printf( "cannot regist to proxy err!" );
			LOG_ERROR( "default","cannot regist to proxy err!" );		
		}
	}
	else
	{
		printf( "can not connect to proxy!" );
		LOG_ERROR( "default","can not connect to proxy!" );
		return -1;
	}
	return 0;
}	

// ***************************************************************
// Function:     Regist2Proxy
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
bool CDungeonServer::Regist2Proxy()
{
	// 首先向 ProxyServer 发送注册消息
	CProxyHead tHead;
	char message_buffer[MAX_CODE_LEN];
	pbmsg_setproxy(&tHead, CET_DUNGEON, CET_GLOBALPROXY, mServerConfig.mServerID, 1, TRANS_P2P, CMD_REGSERVER);

	unsigned short message_length  = tHead.ByteSize() + 4;
	*((unsigned short*) message_buffer)  = message_length;
	*((unsigned short*) (message_buffer+2))  = tHead.ByteSize();

	if(tHead.SerializeToArray(message_buffer+4, message_length - 4))
	{
		LOG_DEBUG("default", "regist to proxyserver (%d): head=[%d:%s]",  message_length , tHead.ByteSize(), tHead.ShortDebugString().c_str());
		if( mGlobalProxy.GetSocket()->SendOneCode(message_length, (BYTE*)message_buffer) == 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}else
	{
		LOG_INFO("default", "regist to proxyserver error");
		return false;
	}
}

// ***************************************************************
// Function:     KeepAliveToProxy
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
void CDungeonServer::KeepAliveToProxy(  )
{

}


template<> CDungeonLogic* CSingleton< CDungeonLogic >::spSingleton = NULL;
CSharedMem *CDungeonLogic::m_pShareMem = NULL;

// ***************************************************************
// Function:     CDungeonLogic
// Description:  
// Others:         
// Date:           03/22/2011
// 
//***************************************************************
CDungeonLogic::CDungeonLogic()
{

}

// ***************************************************************
// Function:     ~CDungeonLogic
// Description:  
// Others:         
// Date:           03/22/2011
// 
//***************************************************************
CDungeonLogic::~CDungeonLogic()
{

}

// ***************************************************************
// Function:     ~CDungeonLogic
// Description:  
// Others:         
// Date:           03/22/2011
// 
//***************************************************************
int CDungeonLogic::Initialize()
{	
}

// ***************************************************************
// Function:     OnTimer
// Description:  
// Others:         
// Date:           03/22/2011
// 
//***************************************************************
void CDungeonLogic::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	CModuleManager::GetSingleton().Timer( vTickCount, vTickOffset );
}

// ***************************************************************
// Function:    ~operator new
// Description:  
// Others:         
// Date:           03/24/2011
// 
//***************************************************************
void* CDungeonLogic::operator new( size_t size )
{
	return ( void* ) m_pShareMem->CreateSegment( size );
}

// ***************************************************************
// Function:    operator delete
// Description:  
// Others:         
// Date:           03/24/2011
// 
//***************************************************************
void  CDungeonLogic::operator delete( void* pointer )
{
		
}

// ***************************************************************
// Function:   CountSize  
// Date:       03/24/2011
// 
//***************************************************************
unsigned int CDungeonLogic::CountSize( )
{
	return sizeof( CDungeonLogic );		
}

// ***************************************************************
// Function:    ReadConfig  
// Date:        03/24/2011
//***************************************************************
int  CDungeonLogic::ReadConfig()
{
	CModuleManager::GetSingleton().LoadConfig();
}

// ***************************************************************
// Function:    ReadConfig  
// Date:        03/24/2011
//***************************************************************
void CDungeonLogic::Launch()
{
	// 注册模块
	CModuleManager::GetSingleton().RegisterModule( new CDungeonModule );	

	// 模块初始化
	CModuleManager::GetSingleton().LaunchServer();	

	// 读取配置文件
	CDungeonLogic::GetSingleton().ReadConfig();

	// 连接全局代理服务器
	CDungeonServer::GetSingleton().OnLaunch();
}

// ***************************************************************
// Function:    OnServerMessage
// Description:  
// Others:         
// Date:           03/24/2011
//***************************************************************
void CDungeonLogic::OnServerMessage( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID )
{		
	LK_ASSERT( pMsg != NULL, return )
	CModuleManager::GetSingleton().ServerMessage( pMsg, nGameID, nSrcFe, nSrcID );
}


// ***************************************************************
// Function:     Send2Server
// Description:  
// Others:         
// Date:         03/24/2011 
//***************************************************************
bool CDungeonLogic::Send2Server( CMessage *pMsg, int nGameID, int nDesFe, int nDesID )
{
	LK_ASSERT( pMsg != NULL, return false )
	CProxyHead tProxyHead;
	static BYTE buffer[ MAX_CODE_LEN ];
	unsigned short len = sizeof( buffer );

	Message* pUnknownMessagePara = (Message*) pMsg->msgpara();
	LK_ASSERT( pUnknownMessagePara != NULL, return false);
	int tServerID = CDungeonServer::GetSingleton().mServerConfig.mServerID;
	pbmsg_setproxy( &tProxyHead, CET_DUNGEON, nDesFe, tServerID, nDesID, TRANS_P2P, CMD_NULL );
	tProxyHead.set_desgameid( nGameID );

	pMsg->mutable_msghead()->set_srcfe( FE_DUNGEONSERVER );
	pMsg->mutable_msghead()->set_srcid( tServerID );

	len = pbmsg_setmsg<CProxyHead>( tProxyHead, *pMsg, (char*)buffer, len );

	if( len <= 0 )
	{
		LOG_ERROR( "default", "Serialize for router message failed. %d", len );
		return false;
	}

	char tszMessageid[16] = {0};
	snprintf( tszMessageid, sizeof(tszMessageid)-1, "%d->gate", pMsg->mutable_msghead()->messageid() );

	int nRet = CDungeonServer::GetSingleton().mGlobalProxy.GetSocket()->SendOneCode( len, (BYTE*)buffer ) ;
	if ( nRet < 0 )
	{
		LOG_ERROR( "default", "[%s:%d] send 2 gate len = %d failed %d", __FUNCTION__, __LINE__, len, nRet );
	}

	return ( nRet == 0 );
}



// ***************************************************************
// Function:     Send2Gate
// Description:  
// Others:         
// Date:         03/26/2011 
//***************************************************************
void CDungeonLogic::Send2Gate( CMessage *pMsg, int nGameID, int nServerID /* = 2 */ )
{
	if ( pMsg != NULL )
	{
		Send2Server( pMsg, nGameID, CET_OTHERSERVER, nServerID );
	}
}

// ***************************************************************
// Function:     Send2Scene
// Description:  
// Others:         
// Date:         03/27/2011 
//***************************************************************
void CDungeonLogic::Send2Scene( CMessage *pMsg, int nGameID, int nServerID )
{
	if ( pMsg != NULL )
	{
		Send2Server( pMsg, nGameID, CET_GAMESERVER, nServerID );
	}
}

