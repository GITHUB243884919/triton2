#include "epollsocket.h"
#include <netdb.h>
#ifdef LINUX
int CClientLinuxContext::sClientID = 1;
namespace LINEKONG
{

#ifdef WIN32
    uint64_t GetTickCount()
    {
        return ::GetTickCount();
    }
#else
#   include <sys/time.h>
    uint64_t GetTickCount()
    {
        struct timeval tmval = {0};
        int nRetCode = gettimeofday(&tmval, NULL);
        if (nRetCode != 0)
        {
            return 0;
        }
        return ((uint64_t)tmval.tv_sec) * 1000 + tmval.tv_usec / 1000;
    }
#endif

}



CLinuxSocket::CLinuxSocket( )
{
    mSocket		= INVALID_SOCKET;
    mBytesSend	= 0;
    mBytesRecv	= 0;
}

CLinuxSocket::~CLinuxSocket( )
{

}

int CLinuxSocket::GetBufferLength( SOCKET vSocket /* = 0  */)
{

    int tRetCode = 0;
    if ( vSocket == 0 )
    {
        vSocket = mSocket;
    }

    int       tLength = 0; 
    uint32_t  tBytes = 0;
    socklen_t tOptLen = sizeof( int );

    tRetCode = getsockopt( vSocket, SOL_SOCKET, SO_RCVBUF, &tLength, &tOptLen );
    if ( tRetCode != 0 ) // No error happened!
    {
        tLength = -1;
        perror( "CLinuxSocket::GetBufferLength()" );
    }
    return tLength;
}

void CLinuxSocket::GetLocalName( char* pBuffer, int vLength )
{
    ::gethostname( pBuffer, vLength );
}

uint32_t CLinuxSocket::GetLocalIP( )
{
    char tName[ 4096 ] = { 0 };
    ::gethostname( tName, 4096 );

    hostent* tpHostent = ::gethostbyname( tName );

    return (uint32_t) ( (in_addr*) tpHostent->h_addr_list[0] )->s_addr;
}

char *CLinuxSocket::GetLocalIPString( )
{
    return IPDword2String( GetLocalIP( ) );
}

hostent *CLinuxSocket::GetHostByName( char* pName )
{
    return ::gethostbyname( pName );
}


char *CLinuxSocket::GetServiceName( uint16_t vPort, char* pProtocol )
{
    servent* tpServent = ::getservbyport( ::htons( vPort ), pProtocol );

    if ( tpServent == 0 )
        return 0;
    return tpServent->s_name;
}

uint16_t CLinuxSocket::GetServicePort( char* pName, char* pProtocol )
{
    servent* tpServent = ::getservbyname( pName, pProtocol );
    if ( tpServent == 0 )
        return 0;

    return ::ntohs( tpServent->s_port );
}

char *CLinuxSocket::GetProtocolName( int vNumber )
{
    protoent* tpProtoent = ::getprotobynumber( vNumber );

    if ( tpProtoent == 0 )
        return 0;

    return tpProtoent->p_name;
}

uint16_t CLinuxSocket::GetProtocolNumber( char* pName )
{
    protoent* tpProtoent = ::getprotobyname( pName );

    if ( tpProtoent == 0 )
        return 0;

    return tpProtoent->p_proto;
}

void CLinuxSocket::CreateSockAddr(sockaddr_in* pSockAddr, const char* pAddress )
{
    if ( pSockAddr == 0 || pAddress == 0 )
        return;

    const char* tpPort = ::strchr( pAddress, ':' );

    if ( tpPort != 0 )
    {
        char tAddress[ 1024 ] = { 0 };
        ::memcpy( tAddress, pAddress, tpPort - pAddress );
    
        CreateSockAddr( pSockAddr, tAddress, (uint16_t) ::atoi( tpPort + 1 ) );
    }
    else
    {
        CreateSockAddr( pSockAddr, pAddress, 0 );
    }
}

void CLinuxSocket::CreateSockAddr( sockaddr_in* pSockAddr, uint32_t vIP, uint16_t vPort )
{
    if ( pSockAddr == 0 )
        return;

    ::memset( pSockAddr, 0, sizeof( sockaddr_in ) );

    pSockAddr->sin_family		= AF_INET;
    pSockAddr->sin_port			= ::htons( vPort );
    pSockAddr->sin_addr.s_addr	= vIP;
}

void CLinuxSocket::CreateSockAddr( sockaddr_in* pSockAddr, const char* pAddress, uint16_t vPort )
{
    if ( pSockAddr == 0 || pAddress == 0 )
        return;

    uint32_t tIP = 0;

    if ( ::strcmp( pAddress, "localhost" ) == 0 || ::strcmp( pAddress, "127.0.0.1" ) == 0 )
        tIP = GetLocalIP( );
    else
        tIP = ::inet_addr( pAddress );

    if ( tIP == INADDR_NONE )
    {
        hostent* tpHostent = ::gethostbyname( pAddress );

        if ( tpHostent != 0 )
            tIP = ( (in_addr*) tpHostent->h_addr_list[0] )->s_addr;
    }

    CreateSockAddr( pSockAddr, tIP, vPort );
}

int CLinuxSocket::GetInterface( sockaddr_in* pSockAddr, int vIndex )
{
    return 0;
}

int CLinuxSocket::GetAddress( sockaddr_in* pSockAddr )
{
    socklen_t tSize = sizeof( sockaddr_in );
    if ( ::getsockname( mSocket, (sockaddr *) pSockAddr, &tSize ) == -1 )
        return errno;

    return 0;
}

int CLinuxSocket::SetOption( int vLevel, int vName, void* pValue, socklen_t vLength, SOCKET vSocket /* = 0  */)
{
    if ( vSocket == 0 )
        vSocket = mSocket;

    if ( ::setsockopt( vSocket, vLevel, vName, (char*) pValue, vLength ) == -1 )
        return errno;

    return 0;
}

int CLinuxSocket::GetOption( int vLevel, int vName, void* pValue, socklen_t* pLength, SOCKET vSocket /* = 0  */)
{
    if ( vSocket == 0 )
        vSocket = mSocket;

    if ( ::getsockopt( vSocket, vLevel, vName, (char*) pValue, pLength ) == -1 )
        return errno;

    return 0;
}

////////////////CAsyncLinuxSocket
CAsyncLinuxSocket::CAsyncLinuxSocket( )
{
    mhObject		= 0;
    mhThread		= 0;
    mhEvent			= 0;
    mThreadID		= 0;
    mTickCount		= 0;
    mLastTickCount	= 0;
    mIsMessage		= 0;
    mNeedTimer		= false;
}

CAsyncLinuxSocket::~CAsyncLinuxSocket( )
{

}


void CAsyncLinuxSocket::Create( bool vIsMessage )
{
    mBytesSend		= 0;
    mBytesRecv		= 0;
    mTickCount		= 0;
    mLastTickCount	= 0;
    mIsMessage		= vIsMessage;
}

void CAsyncLinuxSocket::Close(  )
{
    /*
    if ( mhEvent != 0 )
        ::SetEvent( mhEvent );

    if ( mhThread != 0 && ::GetCurrentThreadId( ) != mThreadID )
    {
        mThreadID = 0;

        ::WaitForSingleObject( mhThread, 10000 );
        ::CloseHandle( mhThread );
    }
    */

	if (!mExitFlag)
	{
		SetEvent(); // 设置事件事件结束。
		mExitFlag = true;
		WaitForEvent(); // 等待事件结束。
		DestroyEvent(); // 释放事件资源。
	}
	
#ifdef LINUX
    if ( mhObject != 0 )
        close( mhObject );

    if ( mhEvent != 0 )
        close( mhEvent );

    mBytesSend		= 0;
    mBytesRecv		= 0;
    mhObject		= 0;
    mhThread		= 0;
    mhEvent			= 0;
    mThreadID		= 0;
    mTickCount		= 0;
    mLastTickCount	= 0;
    mIsMessage		= 0;
    close( mSocket );
#endif
}

// To initialize condition
int CAsyncLinuxSocket::SetEvent()
{
#ifdef LINUX
	pthread_cond_init(&mCondition, NULL);
	pthread_mutex_init(&mMutex, NULL);
#endif
	puts("We initliazed event!");
	return 0;
}

int CAsyncLinuxSocket::WaitForEvent()
{
	puts("Waiting for exit signal...");
#ifdef LINUX
	pthread_mutex_lock(&mMutex);
	pthread_cond_wait(&mCondition, &mMutex);
	pthread_mutex_unlock(&mMutex);
#endif
	puts("Recieved exit signal and WaitForEvent() returned!");
	return 0;
}

// To awake a waiting event.
int CAsyncLinuxSocket::AwakeEvent()
{
#ifdef LINUX
	pthread_cond_signal(&mCondition);
#endif
	puts("Thread signaled!");
	return 0;
}

int CAsyncLinuxSocket::DestroyEvent()
{
	int nRetCode = 0;
#ifdef LINUX
	for (; ; )
	{
		nRetCode = pthread_cond_destroy(&mCondition);
		// If EBUSY returned, it means condition has not been released by any thread.
		if (nRetCode != EBUSY)
		{
			break;
		}
	}
	for (; ; )
	{
		nRetCode = pthread_mutex_destroy(&mMutex);
		// If EBUSY returned, it means condition has not been released by any thread.
		if (nRetCode != EBUSY)
		{
			break;
		}
	}
#endif
	puts("We destroyed event!");
	return 0;
}

void CAsyncLinuxSocket::LockData( )
{
    if ( mpDataCS != 0 )
    {
#ifdef WIN32
    
        ::EnterCriticalSection( mpDataCS ); 
    
#else
        pthread_mutex_lock( mpDataCS );
#endif
    }
}

void CAsyncLinuxSocket::UnlockData( )
{
    if ( mpDataCS != 0 )
    {
#ifdef WIN32

        ::LeaveCriticalSection( mpDataCS ); 

#else
        pthread_mutex_unlock( mpDataCS );
#endif
    }
}

// CClientLinuxSocket
CClientLinuxSocket::CClientLinuxSocket( )
{
    mhObject		 = 0;		// 事件句柄，用于等待网络消息
    mTickCount		 = 0;
    mLastTickCount	 = 0;
    mRecvLength		 = 0;
    mPing			 = 0;
    mConnected		 = false;
    mSelectedEvents  = 0;
}

CClientLinuxSocket::~CClientLinuxSocket( )
{

}

void CClientLinuxSocket::Close( )
{

    if ( mhObject != 0 )
    {
        mhObject = 0;
    }

    if ( mSocket != 0 )
    {
        ::close( mSocket );
    }

    mConnected              = false;
    mBytesSend              = 0;
    mBytesRecv              = 0;
    mhObject                = 0;
    mTickCount              = 0;
    mLastTickCount          = 0;
    mSocket                 = INVALID_SOCKET;
    mRecvLength             = 0;
    mPing                   = 0;
}

bool CClientLinuxSocket::EventProcess( )
{
    bool tRetCode = true;
    bool tResult  = true;

    CSocketPingMessage tSocketPingMessage;
    uint64_t           tTickCount = 0;

    PROCESS_CHECK_ERROR_RET( mSocket != INVALID_SOCKET, tResult, true );
	tTickCount = LINEKONG::GetTickCount( );
    if ( !mConnected  )
    {
        tRetCode = Connect( &mHostAddr );
        if ( !mConnected )
        {
            goto ExitError;
        }
    }
    if ( tTickCount - mLastTickCount > SOCKET_PINGINTERVAL )
	{
		tRetCode =this->SendMessage( &tSocketPingMessage );
        if ( tRetCode != 0 )
        {
            puts( "Send message error!" );
            OnClose( tRetCode );
            Close( );
            goto ExitError;
        }
		mLastTickCount = tTickCount;
    }

    if ( FD_ISSET( mSocket, &mExcSet ) )
    {
        FD_CLR( mSocket, &mReadSet );
        FD_CLR( mSocket, &mWriteSet );
        FD_CLR( mSocket, &mExcSet );
        OnClose( errno );
        Close( );
        mConnected = false; 
        
        goto ExitError;
    }

    if ( FD_ISSET( mSocket, &mReadSet ) )
    {
        OnRecv( 0 );
    }

    if ( FD_ISSET( mSocket, &mWriteSet ) )
    {
        OnSend( 0 );
    }

    tResult = false;
ExitError:
	return tResult;
}

int CClientLinuxSocket::Create( )
{
    int tRetCode   = -1;
    int tResult    = -1;

    mTimeVal.tv_sec  = 0;
    mTimeVal.tv_usec = 1;

    FD_ZERO( &mReadSet );
    FD_ZERO( &mWriteSet);
    FD_ZERO( &mExcSet );

    mSocket = ::socket( AF_INET, SOCK_STREAM, 0);
    PROCESS_CHECK_ERROR_RET( mSocket != -1, tResult, errno );

    tRetCode = ::fcntl( mSocket, F_GETFL, 0);
    PROCESS_CHECK_ERROR_RET( tRetCode != -1, tResult, errno );
    tRetCode |= O_NONBLOCK;
    tRetCode = ::fcntl( mSocket, F_SETFL, tRetCode);
    PROCESS_CHECK_ERROR_RET( tRetCode != -1, tResult, errno );

    tRetCode = SelectEvent( 0 );
    if ( tRetCode != 0 )
    {
        return tRetCode;
    }
    mRecvLength		= 0;
    mPing			= 0;
    mBytesSend		= 0;
    mBytesRecv		= 0;
    mTickCount		= 0;
    mLastTickCount	= 0;

    tResult = 0;
ExitError:
    return tResult;
}

int CClientLinuxSocket::Connect( const char* pAddress )
{
    sockaddr_in tSockAddr;
    CreateSockAddr( &tSockAddr, pAddress );

    return Connect( &tSockAddr );
}

int CClientLinuxSocket::Connect( const char* pAddress, uint16_t vPort )
{
    sockaddr_in tSockAddr;
    CreateSockAddr( &tSockAddr, pAddress, vPort );

    return Connect( &tSockAddr );
}

int CClientLinuxSocket::Connect( uint32_t vIP, uint16_t vPort )
{
    sockaddr_in tSockAddr;
    CreateSockAddr( &tSockAddr, vIP, vPort );

    return Connect( &tSockAddr );
}

int CClientLinuxSocket::Connect( sockaddr_in* pSockAddr )
{
    int tRetCode = 0;
    if ( pSockAddr == 0 )
    {
        return -1;
    }

    memcpy( &mHostAddr, pSockAddr,sizeof( sockaddr_in ) );
    tRetCode = connect( mSocket, (sockaddr *) pSockAddr, (socklen_t)sizeof( sockaddr_in ) );
    if ( tRetCode == -1 )
    {
        if ( errno != EINPROGRESS )
        {
            OnConnect( errno );
            return errno;
        }
        
    }
    else
    {
        tRetCode = 0;
#ifdef DEBUG_SHOW_COMM_CLIENT_INFO
        printf(
            "[EPOLL-Comm-Client] Connected to [%s:%u] successfully!\n",
            inet_ntoa( mHostAddr.sin_addr ),
            htons( mHostAddr.sin_port )
        );
#endif
        mConnected = true;
        OnConnect( tRetCode );
    }
    return 0;
}

int CClientLinuxSocket::Send( void* pBuffer, int vLength, uint32_t vFlags )
{
    uint8_t* tpBuffer    = (uint8_t*) pBuffer;
    int      tBytesSent  = 0;

    ssize_t  tSentLength = 0;

    do
    {
        tSentLength = send( mSocket, tpBuffer + tBytesSent, vLength - tBytesSent, vFlags );
        if ( tSentLength == -1 )
        {
            if ( errno != EAGAIN )
            {
                return errno;
            }
            tSentLength = 0;
        }
        tBytesSent += tSentLength;

    } while ( tBytesSent != vLength );
#ifdef DEBUG_SHOW_COMM_CLIENT_INFO
    printf( "[EPOLL-COMM-CLIENT] %d bytes sent!\n", tBytesSent );
#endif
    return 0;
}

int CClientLinuxSocket::Recv( void* pBuffer, int vLength, int* pBytes /* = 0 */, uint32_t vFlags /* = 0  */)
{
    ssize_t tReadLength = 0;
#ifdef DEBUG_SHOW_COMM_CLIENT_INFO
    printf( "[EPOLL-COMM-CLIENT] Starting to read data!\n" );
#endif
    if ( (tReadLength = recv( mSocket, pBuffer, vLength, vFlags )) == -1 )
    {
        if ( errno != EAGAIN )
        {
            return errno;
        }
        errno = 0;
        tReadLength = 0;
    }

    if ( pBytes != 0 )
    {
        *pBytes = tReadLength;

    }

    mBytesRecv += tReadLength;
#ifdef DEBUG_SHOW_COMM_CLIENT_INFO
    printf( "[EPOLL-COMM-CLIENT] %d bytes read!\n", tReadLength );
#endif
    return 0;
}

int CClientLinuxSocket::Shutdown( )
{
    if ( mSocket == -1 )
    {
        return 0;
    }

    if ( mConnected == false )
    {
        return 0;
    }

    // 发送FIN标志给服务器,通知客户端要离开了
    ::shutdown( mSocket, SHUT_WR );
    // 等待最后的数据,直到服务器回应ACK,收到FD_CLOSE消息
    unsigned int tWaitTimes = 0;
    uint64_t tThisTick = LINEKONG::GetTickCount( );
    while( ( LINEKONG::GetTickCount( ) - tThisTick ) < tWaitTimes && EventProcess( ) == false );
	return 0;
}

int CClientLinuxSocket::SelectEvent( int vEvent )
{
    mTickCount      = LINEKONG::GetTickCount( );

    FD_CLR( mSocket, &mReadSet );
    FD_CLR( mSocket, &mWriteSet );
    FD_CLR( mSocket, &mExcSet );

    FD_SET( mSocket, &mReadSet );
    FD_SET( mSocket, &mWriteSet );
    FD_SET( mSocket, &mExcSet );
   
    if ( select( mSocket + 1, &mReadSet, &mWriteSet, &mExcSet, &mTimeVal ) == -1 )
    {
        return -1;
    }
#ifdef DEBUG_SHOW_COMM_CLIENT_INFO
    puts( "[EPOLL-COMM-CLIENT] Select Event successfully!!" );
#endif
    return 0;
}

int CClientLinuxSocket::OnConnect( int vCode )
{
    return 0;
}

int CClientLinuxSocket::OnSend( int vCode )
{
    return 0;
}

int CClientLinuxSocket::OnRecv( int vCode )
{
    BYTE* pMessageHeader = (BYTE*) mRecvBuffer;
    uint32_t tSize = 0, tBytes = GetBufferLength( );

    while ( true )
    {
        int tLength = min( SOCKET_BUFFERSIZE_RECV - mRecvLength, tBytes );
        Recv( mRecvBuffer + mRecvLength, tLength, &tLength );
        mRecvLength += tLength;

        //while ( mRecvLength >= sizeof( CSocketMessage ) && mRecvLength >= pMessageHeader->mMessageSize )
        //{
        //    tSize = pMessageHeader->mMessageSize;

        //    if ( pMessageHeader->CheckHeader( ) == false )
        //    {
        //        continue;
        //    }

        //    if ( pMessageHeader->mMessageID == 0xFFFF )
        //    {
        //        CSocketPingMessage* tpPingMessage = (CSocketPingMessage*) pMessageHeader;

        //        mPing = ( LINEKONG::GetTickCount( ) - tpPingMessage->mTickCount ) >> 1;
        //    }
        //    else
        //    {
        //        OnMessage( pMessageHeader );
        //    }
        //    mRecvLength -= tSize;

        //    ::memmove( mRecvBuffer, mRecvBuffer + tSize, mRecvLength );
        //}

		while ( mRecvLength >= sizeof( short ) && mRecvLength >= *(short*)pMessageHeader )
		{
			tSize = *(short*)pMessageHeader;

			OnMessage( pMessageHeader, tSize );

			mRecvLength -= tSize;

			::memmove( mRecvBuffer, mRecvBuffer + tSize, mRecvLength );
		}

        tBytes -= tLength;
        break;
    }
    return 0;
}

int CClientLinuxSocket::OnMessage( BYTE* pMessage, int vMsgLen )
{
    return 0;
}

int CClientLinuxSocket::OnClose( int vCode )
{
    return 0;
}

/*
//////////////////////////////////////////////////////////////////////////////////
*/
CEPollSocket::CEPollSocket( )
: mConnectionIDSeed ( 1 ), mKeepAlive( 0 ), mCurConnection( 0 ), mMaxConnection( 0 ),
mBytesSent( 0 ), mBytesRecv( 0 ), mPackagesSent( 0 ), mPackagesRecv( 0 )
{
    mNeedTimer = true;
}

CEPollSocket::~CEPollSocket( )
{
    Close( );
}


int CEPollSocket::Create(
    const char* pAddress,
    bool        vIsMessage,
    int         vMaxConnection /* = 4096 */,
    uint32_t    vKeepAlive /* = 1800000  */
)
{
    sockaddr_in tSockAddr;
    CreateSockAddr( &tSockAddr, pAddress );
    return Create( &tSockAddr, vIsMessage, vMaxConnection, vKeepAlive );
}

int CEPollSocket::Create(
    const char* pAddress,
    uint16_t    vPort,
    bool        vIsMessage,
    int         vMaxConnection /* = 4096 */,
    uint32_t    vKeepAlive /* = 1800000  */
)
{
    sockaddr_in tSockAddr;
    CreateSockAddr( &tSockAddr, pAddress, vPort );

    return Create( &tSockAddr, vIsMessage, vMaxConnection, vKeepAlive );
}

int CEPollSocket::Create(
    uint32_t    vIP,
    uint16_t    vPort,
    bool        vIsMessage,
    int         vMaxConnection /* = 4096 */,
    uint32_t    vKeepAlive /* = 1800000  */
)
{
    sockaddr_in tSockAddr;
    CreateSockAddr( &tSockAddr, vIP, vPort );

    return Create( &tSockAddr, vIsMessage, vMaxConnection, vKeepAlive );
}

int CEPollSocket::Create(
    sockaddr_in*    pSockAddr,
    bool            vIsMessage,
    int             vMaxConnection /* = 4096 */,
    uint32_t        vKeepAlive /* = 1800000  */
)
{
    int tRetCode   = -1;
    int tResult    = -1;
    
    int tValue     = 1;

    uint8_t tpDottedIP[ 4 ];
    memset( mLocalAddrString, 0, sizeof( mLocalAddrString ) );
    memcpy( tpDottedIP, &( pSockAddr->sin_addr.s_addr ), 4 );
    memset( &mLocalAddr, 0, sizeof( sockaddr_in ) );

    PROCESS_CHECK_ERROR( pSockAddr != NULL );

    snprintf( mLocalAddrString,
        sizeof( mLocalAddrString ),
        "%u.%u.%u.%u:%u",
        tpDottedIP[ 0 ],
        tpDottedIP[ 1 ],
        tpDottedIP[ 2 ],
        tpDottedIP[ 3 ],
        htons( pSockAddr->sin_port )
        );
    mLocalAddrString[ sizeof( mLocalAddrString ) - 1 ] = '\0';
    

    CAsyncLinuxSocket::Create( vIsMessage );
    

    mSocket = socket( AF_INET, SOCK_STREAM, 0 );
    if ( mSocket == -1 )
    {
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
        puts( "[EPOLL-Comm] Create Socket error!" );
#endif
        tResult = errno;
        goto ExitError;
    }
    
    tRetCode = ::setsockopt( mSocket, SOL_SOCKET, SO_REUSEADDR, (char*) &tValue, ( socklen_t )sizeof( int ) );
    if ( tRetCode == -1 )
    {
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
        puts( "[EPOLL-Comm] Set socket option error!" );
#endif
        return errno;
    }
    tRetCode = ::fcntl(mSocket, F_GETFL, 0);
    PROCESS_CHECK_ERROR_RET( tRetCode != -1, tResult, errno );

    tRetCode |= O_NONBLOCK;

    if ( ::fcntl(mSocket, F_SETFL, tRetCode) == -1 ){
        tResult = errno;
        goto ExitError;
    }
    // 创建主句柄。
    
    mhObject = epoll_create( EPOLL_MAX_CLIENT );
    PROCESS_CHECK_ERROR_RET( mhObject != -1, tResult, errno );
    
    tRetCode = ::bind( mSocket, ( sockaddr * ) pSockAddr, sizeof( sockaddr_in ) );
    if ( tRetCode != 0 )
    {
        printf("[EPOLL-Comm] Server binded at [%s] FAILED!\n", mLocalAddrString );
        tResult = errno;
        goto ExitError;
    }
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
    printf("[EPOLL-Comm] Server binded at [%s] SUCCESSFULLY!\n", mLocalAddrString );
#endif
    memcpy( &mLocalAddr, pSockAddr, sizeof( sockaddr_in ) );

    mKeepAlive		= vKeepAlive;
    mCurConnection	= 0;
    mMaxConnection	= vMaxConnection;

    // TODO: Add epoll code here and create a new thread!!!

    tRetCode = pthread_create(
        &mhThread,                                      // Thread handle returned.
        NULL,                                           // Initial parameters. NULL indicates to use default.
        (void *(*)(void *))(CEPollSocket::EPOLLThread), // Start routine of thread.
        this                                            // Parameter of thread.
    );
    PROCESS_CHECK_ERROR_RET( tRetCode == 0, tResult, errno );

    tResult = 0;
ExitError:

    return tResult;
}

int CEPollSocket::Listen( int vQueue /* = 5  */)
{
    int tRetCode = -1;
    int tResult  = -1;

    tRetCode = ::listen( mSocket, min( vQueue, mMaxConnection ) );
    PROCESS_CHECK_ERROR_RET( tRetCode == 0, tResult, errno );
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
    printf("[EPOLL-Comm] Server[%u|%u] started to listen at [%s] SUCCESSFULLY!\n", mhObject, mSocket, mLocalAddrString );
#endif
    tResult = 0;
ExitError:
    return tResult;
}


int CEPollSocket::Accept( )
{
    int tRetCode = -1;
    int tResult  = -1;

    socklen_t   tHostAddrLen  = sizeof( sockaddr_in );
    sockaddr_in tRemoteAddr   = { 0 };
    SOCKET      tClientSocket = INVALID_SOCKET;
    int         tClientID     = -1;
    
    tClientSocket = ::accept( mSocket, ( sockaddr * )&tRemoteAddr, &tHostAddrLen );
    tRetCode      = AddEPollSocket( tClientSocket, tRemoteAddr, &tClientID );
    PROCESS_CHECK_ERROR_RET( tRetCode == 0, tResult, tRetCode );
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
    printf("[EPOLL-Comm] Client [%s:%u] logs in.\n", inet_ntoa( tRemoteAddr.sin_addr ), htons( tRemoteAddr.sin_port ) );
#endif
    tRetCode = OnAccept( tClientID );

    tResult = 0;
ExitError:
    if ( tResult != 0 )
    {
        if ( tClientSocket != INVALID_SOCKET )
        {
            ::shutdown( tClientSocket, SHUT_WR ); // Shut down socket gracefully.
            ::close( tClientSocket );             // Close opened socket only because of failure to register new context.
        }
    }
    return tResult;
}
int CEPollSocket::Send( CClientLinuxContext* vpClientContext, uint32_t vFlags )
{
    int tRetCode = -1;
    int tResult  = -1;

    ASSERT( vpClientContext != NULL );
    OnEncrypt( vpClientContext->mSendBuffer, vpClientContext->mSendLength );
    
    int      tBytesSent  = 0;

    ssize_t  tSentLength = 0;

    do
    {
        tSentLength = send(
            vpClientContext->mSocket, 
            vpClientContext->mSendBuffer + tBytesSent, 
            vpClientContext->mSendLength - tBytesSent,
            vFlags
        );
        if ( tSentLength == -1 )
        {
            if ( errno != EAGAIN )
            {
                return errno;
            }
            tSentLength = 0;
        }

        tBytesSent += tSentLength;

    } while ( tBytesSent != vpClientContext->mSendLength );
    vpClientContext->mSendLength = 0;
    vpClientContext->mBytesSend += vpClientContext->mSendLength;
    mBytesSend                  += vpClientContext->mSendLength;

    if ( mBytesSend > 1000  )
    {
        sleep( vpClientContext->mClientID );
        tResult = -1;
        goto ExitError;
    }
    tResult = 0;
ExitError:
    return tResult;
}
int CEPollSocket::Send( int vCommHandle, void* pBuffer, int vLength, uint32_t vFlags /* = 0  */)
{
    CClientLinuxContext* tpContext = GetClientContext( vCommHandle );
    if ( tpContext == NULL )
    {
        return 0;
    }

    return Send( tpContext, vFlags );
}

int CEPollSocket::Recv( int vCommHandle, void* pBuffer, int vLength, int* pBytes /* = 0 */, uint32_t vFlags /* = 0  */)
{
    CClientLinuxContext* tpContext = GetClientContext( vCommHandle );
    if ( tpContext == NULL )
    {
        return 0;
    }
    ssize_t tBytesRecv = 0;
    
    if ( ( tBytesRecv = recv( tpContext->mSocket, pBuffer, vLength, vFlags ) ) == -1 )
        return errno;

    if ( pBytes != 0 )
    {
        *pBytes = tBytesRecv;
    }
    tpContext->mBytesRecv += tBytesRecv; mBytesRecv += tBytesRecv;

    OnDecrypt( pBuffer, tBytesRecv );
    return 0;
}

int CEPollSocket::Recv( CClientLinuxContext* vpClientContext, int* pBytes /* = 0 */, uint32_t vFlags /* = 0  */)
{
    int tRetCode = -1;
    int tResult  = -1;

    ASSERT( vpClientContext != NULL );
    ssize_t tBytesRecv = 0;

    tBytesRecv = ::recv(
        vpClientContext->mSocket,
        vpClientContext->mRecvBuffer + vpClientContext->mRecvLength,
        SOCKET_BUFFERSIZE_RECV       - vpClientContext->mRecvLength,
        vFlags
    );
    if ( tBytesRecv == -1 )
    {
        tResult = errno;
        goto ExitError;
        return errno;
    }

    if ( pBytes != 0 )
    {
        *pBytes = tBytesRecv;
    }
    vpClientContext->mBytesRecv += tBytesRecv;
    mBytesRecv += tBytesRecv;

    OnDecrypt( vpClientContext->mRecvBuffer, tBytesRecv );
    tResult = 0;
ExitError:
    return tResult;
}

int CEPollSocket::Close( ) 
{
    // 全部关闭。
    CLIENTITER tBeginIter	= mClientSockets.begin( );
    CLIENTITER tEndIter		= mClientSockets.end( );
    // 肯定是有问题的。
    for ( ; tBeginIter != tEndIter; tBeginIter ++ )
    {
        CloseSocket( tBeginIter->second->GetCommHandle( ), false );
    }

    mClientSockets.clear( );
    CAsyncLinuxSocket::Close( );
    
    mKeepAlive		= 0;
    mCurConnection	= 0;
    mMaxConnection	= 0;
    RemoveEPollSocket( mListenLinuxSocketContext.mClientID );
    return 0;
}


int CEPollSocket::SetKeepAlive(int vCommHandle, uint32_t vKeepAliveInterval )
{
    CClientLinuxContext* tpContext = GetClientContext( vCommHandle );
    if ( tpContext == NULL )
        return 0;

    mKeepAlive = vKeepAliveInterval;
    tpContext->mKeepAlive = vKeepAliveInterval;
    tpContext->mTickCount = LINEKONG::GetTickCount( );

    return 0;
}


int CEPollSocket::CheckAlive( int vCommHandle, uint32_t vTickCount )
{
    return 0;
    if ( vCommHandle != 0 )
    {
        CClientLinuxContext* tpContext = GetClientContext( vCommHandle );
        if ( tpContext == NULL )
            return 0;

        if ( tpContext->mKeepAlive < vTickCount - tpContext->mTickCount )
        {
            CloseSocket( vCommHandle, true );
        }
    }
    else
    {

        CLIENTITER tBeginIter	= mClientSockets.begin( );
        CLIENTITER tEndIter		= mClientSockets.end( );
        for ( ; tBeginIter != tEndIter; tBeginIter ++ )
        {
            CheckAlive( tBeginIter->second->GetCommHandle( ), vTickCount );
        }
    }

    return 0;
}

int CEPollSocket::FillBuffer( int vCommHandle, void* pBuffer, int vLength )
{
    if ( vCommHandle != 0 )
    {
        CClientLinuxContext* tpContext = GetClientContext( vCommHandle );
        if ( tpContext == NULL )
        {
            return 0;
        }

        this->mBytesSent    += vLength;
        this->mPackagesSent ++;

        if ( tpContext->mSendLength + vLength > SOCKET_BUFFERSIZE_SEND )
        {
            FlushBuffer( vCommHandle );
        }
        ::memcpy( tpContext->mSendBuffer + tpContext->mSendLength, pBuffer, vLength );
        tpContext->mSendLength += vLength;
    }
    else if ( vCommHandle == 0 )
    {
        CLIENTITER tBeginIter	= mClientSockets.begin( );
        CLIENTITER tEndIter		= mClientSockets.end( );
        for ( ; tBeginIter != tEndIter; tBeginIter ++ )
        {
            FillBuffer( tBeginIter->second->GetCommHandle( ), pBuffer, vLength );
        }
    }
    return 0;

}

int CEPollSocket::FlushBuffer( int vCommHandle, uint32_t vFlags /* = 0  */)
{
    int tRetCode = -1;

    if ( vCommHandle != 0 )
    {
        CClientLinuxContext* tpContext = GetClientContext( vCommHandle );
        if ( tpContext == NULL )
        {
            return 0;
        }

        if ( tpContext->mSendLength != 0 )
        {
            tRetCode = Send( tpContext, vFlags ); // 有可能失败。失败时如何处理？
            if ( tRetCode != 0 )
            {
                CloseSocket( tpContext, true );
            }
        }

        
    }
    else if ( vCommHandle == 0 )
    {
        CLIENTITER tBeginIter	= mClientSockets.begin( );
        CLIENTITER tEndIter		= mClientSockets.end( );
        for ( ; tBeginIter != tEndIter; tBeginIter ++ )
        {
            FlushBuffer( tBeginIter->second->GetCommHandle( ), vFlags );
        }
    }

    return 0;
}


int CEPollSocket::CloseSocket( int vCommHandle, bool vIsLost )
{
    CClientLinuxContext* tpClientContext = GetClientContext( vCommHandle );
    if ( tpClientContext == NULL )
    {
        return 0;
    }

    return CloseSocket( tpClientContext , vIsLost );
}

// 直接关闭处理器！！
int CEPollSocket::CloseSocket( CClientLinuxContext* vpClientContext, bool vIsLost )
{
    int tRetCode = -1;
    int tResult  = -1;

    SOCKET  tSocket     = INVALID_SOCKET;
    int     tCommHandle = INVALID_COMM_HANDLE;

    tSocket     = vpClientContext->mSocket;
    tCommHandle = vpClientContext->mClientID;

    ASSERT( tSocket != INVALID_SOCKET );

    if ( vpClientContext->mSendLength != 0 && vIsLost == false )
    {
        FlushBuffer( vpClientContext->mClientID );
    }
    mCurConnection --;

    OnClose( vpClientContext->mClientID, vIsLost );

    tRetCode = RemoveEPollSocket( vpClientContext );
    ASSERT( tRetCode == 0 );

    ::shutdown( tSocket, SHUT_WR );
    ::close( tSocket );
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
    puts( "[EPOLL-COMM] We closed client connection and released all corresponding resources!" );
#endif
    return 0;
}

int CEPollSocket::ReleaseSocket( int vCommHandle )
{
    
    CLIENTITER           tClientIter   = mClientSockets.find( vCommHandle );
    CClientLinuxContext *tpClientContext = NULL;

    if ( tClientIter == mClientSockets.end( ) )
    {
        return 0;
    }

    //OnClose( vCommHandle, false );

    tpClientContext = tClientIter->second;

    return RemoveEPollSocket( vCommHandle );
}

int CEPollSocket::ShowFlowInfo( )
{
    puts( "===================================================" );
    puts( "====              EPOLL COMM INFO              ====" );
    printf( "\tBytes R/S statistics.\n" );
    printf( "\tSend: [%d], Recv: [%d], AVG: [ BPS].\n", mBytesRecv, mBytesSent, 0 );

    printf( "\tPackages R/S statistics.\n" );
    printf( "\tSend: [%d], Recv: [%d], AVG: [ BPS].\n", mPackagesRecv, mPackagesSent, 0 );
    puts( "===================================================" );
}

bool CEPollSocket::CheckContext( int vCommHandle )
{
    return GetClientContext( vCommHandle ) != NULL;
}

void CEPollSocket::DumpClient( )
{

}

CClientLinuxContext* CEPollSocket::GetClientContext( int vCommHandle )
{
    CLIENTITER tFindIter = mClientSockets.find( vCommHandle );
    if ( tFindIter == mClientSockets.end( ) )
    {
        return NULL;
    }
    return tFindIter->second;
}


int CEPollSocket::RemoveClientLinuxContext( int vCommHandle )
{
    int tRetCode = -1;
    int tResult  = -1;

    CClientLinuxContext *tpClientLinuxContext = NULL;
    CLIENTITER tClientContainerIter = mClientSockets.find( vCommHandle );
    
    if ( tClientContainerIter != mClientSockets.end( )  )
    {
        tpClientLinuxContext = tClientContainerIter->second;
        ASSERT( tpClientLinuxContext != NULL );
        delete tpClientLinuxContext;
        tpClientLinuxContext = NULL;
        mClientSockets.erase( tClientContainerIter );
    }
    else
    {
        goto ExitError;
    }
    tResult = 0;
ExitError:
    return tResult;
}
// 添加Socket。
int CEPollSocket::AddEPollSocket( SOCKET vSocket, sockaddr_in &vrRemoteAddr, int *vpCommHandle )
{
    int tRetCode = -1;
    int tResult  = -1;

    int tCommHandle = 0; 

    CClientLinuxContext *tpClientContext = NULL;
    epoll_event tEvent = { 0 };
    

    ASSERT( vpCommHandle != NULL );

    // 非侦听端口。
    if ( vSocket != mSocket )
    {
        tpClientContext = new CClientLinuxContext( ); // Client ID is retrieved automatically!
        PROCESS_CHECK_ERROR_RET( tpClientContext != NULL, tResult, E_SYS_NOT_ENOUGH_MEMORY );
        
        // 初始化参数。
        tpClientContext->mSocket     = vSocket;
        tpClientContext->mTickCount  = LINEKONG::GetTickCount( );
        tpClientContext->mRemoteAddr = vrRemoteAddr;
        
        // CommHandle.
        tCommHandle                  = tpClientContext->GetCommHandle( );
        
        // We insert the ClientID-ContextPointer into client container.
        mClientSockets.insert( CLIENTCONTAINER::value_type( tCommHandle, tpClientContext ) );
    }
    else
    {
        // 初始化。
        tCommHandle                       = mListenLinuxSocketContext.GetCommHandle( );
        mListenLinuxSocketContext.mSocket = vSocket;
    }

    tEvent.events   = EPOLLIN | EPOLLOUT | EPOLLERR;
    tEvent.data.u32 = tCommHandle;

    tRetCode = epoll_ctl( mhObject, EPOLL_CTL_ADD, vSocket, &tEvent );
    if ( tRetCode == -1 )
    {
        PROCESS_CHECK_ERROR_RET(
            ( errno != EBADF  ) &&
            ( errno != EPERM  ) &&
            ( errno != EINVAL ) &&
            ( errno != ENOMEM ),
            tResult,
            errno
        );
    }
    
    if ( vpCommHandle != NULL )
    {
        *vpCommHandle = tCommHandle;
    }
    tResult = 0;
ExitError:
    if ( tResult != 0 && tpClientContext != NULL )
    {
        tRetCode = RemoveClientLinuxContext( tCommHandle );
        if ( tRetCode != 0 )
        {
            delete tpClientContext;
        }
        tpClientContext = NULL;
    }
    return tResult;
}

int CEPollSocket::RemoveEPollSocket( CClientLinuxContext* vpContext )
{
    int tRetCode  = -1;
    int tResult   = -1;

    epoll_event tEvent  = { 0 };

    SOCKET      tSocket = INVALID_SOCKET;

    ASSERT( vpContext != NULL );

    tEvent.events  = EPOLLIN | EPOLLOUT | EPOLLERR;
    tEvent.data.fd = vpContext->mClientID;
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
    puts("[EPOLL-COMM] We are clearing the following client context..." );
    vpContext->ShowInfo( );
#endif 
    tSocket = vpContext->mSocket;
    tRetCode = RemoveClientLinuxContext( vpContext->mClientID );
    ASSERT( tRetCode == 0 );
 
    tRetCode = epoll_ctl( mhObject, EPOLL_CTL_DEL, tSocket, &tEvent );
    if ( tRetCode == -1 )
    {
        if (
            ( errno == EBADF  ) ||
            ( errno == EPERM  ) ||
            ( errno == EINVAL ) ||
            ( errno == ENOMEM )
            )
        {
            tResult = errno;
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
            puts( "[EPOLL-COMM] Clearing client context error!" );
#endif
            goto ExitError;
        }
    }
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
    puts("[EPOLL-COMM] Client context cleared!" );
#endif
    tResult = 0;
ExitError:
    return tResult;
}

/*
    函数说明：将Socket从EPOLL控制中去除。
            这里，我们假设被去除的Socket的上下文中保存的数据已经全部处理完毕。
    参数说明：
        vSocket - 待删除的Socket。
    返回参数：
        0    - 成功；
        else - 失败。
*/
int CEPollSocket::RemoveEPollSocket( int vCommHandle )
{
    int tRetCode  = -1;
    int tResult   = -1;

    epoll_event tEvent  = { 0 };

    SOCKET      tSocket = INVALID_SOCKET;

    CClientLinuxContext* tpClientContext = NULL;

    tEvent.events  = EPOLLIN | EPOLLOUT | EPOLLERR;
    tEvent.data.fd = vCommHandle;


    if ( vCommHandle != LISTENER_COMM_HANDLE )
    {
        tpClientContext = GetClientContext( vCommHandle );
        ASSERT( tpClientContext != NULL || ( 0 && "Must exist. Otherwise you shall check bug!!!" ));// Must exist. Otherwise you shall check bug,
        tRetCode = RemoveEPollSocket( tpClientContext );
        PROCESS_CHECK_ERROR_RET( tRetCode == 0, tResult, tRetCode );
        goto ExitOK;
    }
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
    puts("[EPOLL-COMM] We are clearing the following listener context..." );
#endif
    tRetCode = epoll_ctl( mhObject, EPOLL_CTL_DEL, tSocket, &tEvent );
    if ( tRetCode == -1 )
    {
        if (
            ( errno == EBADF  ) ||
            ( errno == EPERM  ) ||
            ( errno == EINVAL ) ||
            ( errno == ENOMEM )
        )
        {
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
            puts( "[EPOLL-COMM] Clearing listener context error!" );
#endif
            tResult = errno;
            goto ExitError;
        }
    }
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
    puts( "[EPOLL-COMM] Listener context cleared!" );
#endif
ExitOK:

    tResult = 0;
ExitError:
    return tResult;
}

int CEPollSocket::CheckConnectionID( int vOldConnectionID, int* vpNewConnectionID )
{
    CONNECTION_TEMP_BUFFER_CONTAINER::iterator tTempBufferIter = mConnectionTempBuffers.find( vOldConnectionID );
    if ( tTempBufferIter == mConnectionTempBuffers.end( ) )
    {
        *vpNewConnectionID = mConnectionIDSeed++;

    }
    else
    {
        *vpNewConnectionID = vOldConnectionID;
        // TODO: Add your own code here!
    }
    
    return 0;
}

int CEPollSocket::OnAccept( int vCommHandle )
{
    return 0;
}

int CEPollSocket::OnSend( int vCommHandle, int vBytes )
{
    return 0;
}

int CEPollSocket::OnRecv( int vCommHandle, int vBytes )
{
    int tResult  = 1;
    int tRetCode = 1;

    uint32_t tSize = 0;
    BYTE*       tpMessageHeader       = NULL;
    CGatewayLoginMessage* tpGatewayLoginMessage = NULL;

    CClientLinuxContext *tpClientContext = GetClientContext( vCommHandle );
    PROCESS_CHECK_ERROR_RET( tpClientContext != NULL, tResult, 0 );

    if ( mIsMessage )
    {
        tpMessageHeader  = (BYTE*) tpClientContext->mRecvBuffer;

        tpClientContext->mRecvLength += vBytes;

        while (
            ( tpClientContext->mRecvLength >= sizeof( short )     ) &&
            ( tpClientContext->mRecvLength >= *(short*)tpMessageHeader )
        )
        {
            tSize = *(short*)tpMessageHeader;  // Current Message length.
            //if ( tpMessageHeader->CheckHeader( ) == false )
            //{
            //    continue;
            //}
            
            //switch( tpMessageHeader->mMessageID )
            //{
            //case 0xFFFF: // PING包
            //    // 直接回复。
            //    FillBuffer( vCommHandle, tpMessageHeader, tpMessageHeader->mMessageSize );

            //    break;
            //case 0xFFFE: // 登录协议包。
            //    
            //    tpGatewayLoginMessage = (CGatewayLoginMessage*) tpMessageHeader;
            //    tpGatewayLoginMessage->mConnectionID = 0;

            //    // 若有缓存待发，则请从下面的函数处理。
            //    tRetCode = CheckConnectionID(
            //        tpGatewayLoginMessage->mConnectionID, &tpGatewayLoginMessage->mConnectionID
            //    );
            //    tpClientContext->mConnectionID = tpGatewayLoginMessage->mConnectionID;// 设置新的连接号。
            //    FillBuffer( vCommHandle, tpMessageHeader, tpMessageHeader->mMessageSize );
            //    break;
            //default:
            //    tRetCode = OnMessage( vCommHandle, tpMessageHeader );
            //    if ( tRetCode != 0 )
            //    {
            //        tResult = 1;
            //        goto ExitError;
            //    }
            //    break;

            //}

			OnMessage ( vCommHandle, tpMessageHeader, tSize );

            tpClientContext->mRecvLength -= tSize;
            ::memmove(
                tpClientContext->mRecvBuffer,
                tpClientContext->mRecvBuffer + tSize,
                tpClientContext->mRecvLength
            );

            this->mBytesRecv    += tSize;
            this->mPackagesRecv ++;
        }
    }
    tResult = 0;
ExitError:
    return tRetCode;
}

int CEPollSocket::OnMessage( int vCommHandle, BYTE* pMessage, int vMsgLen )
{
    return 0;
}

int CEPollSocket::OnTimer( uint32_t vTickCount )
{
    return 0;
}

int CEPollSocket::OnClose( int vCommHandle, bool vIsLost )
{
    return 0;
}


int CEPollSocket::EPOLLThread( void* pParam )
{
    
    int tRetCode   = 0;
    int tResult    = 0;

    int tOptionVal = 0;

    SOCKET      tClientSocket = INVALID_SOCKET;
    uint32_t    tEventValue   = 0;

    int32_t     tRecvBytes    = 0;
    int         tCommHandle   = 0;
    sockaddr_in tLocalAddr    = { 0 };

    CClientLinuxContext *tpClientContext = NULL;

    struct epoll_event *tpEvents, tEvents = { 0 };
    int          tCommHandleCount         = 0;
    CEPollSocket *tpServerSocket          = ( CEPollSocket * )pParam;

    uint64_t tTickCount = 0;
    uint64_t tWaitTime  = 0;

    // 最大的客户端连接数。
    tpEvents = new epoll_event[ EPOLL_MAX_CLIENT ];
    PROCESS_CHECK_ERROR( tpEvents != NULL );

    memset( tpEvents, 0, sizeof( epoll_event ) * EPOLL_MAX_CLIENT );

    // 只捕捉以下三种事件。    
    tEvents.events  = EPOLLIN | EPOLLOUT | EPOLLERR;
    tEvents.data.fd = 0;

    PROCESS_CHECK_ERROR( tpServerSocket->mSocket != INVALID_SOCKET );
    
    tRetCode = tpServerSocket->AddEPollSocket( tpServerSocket->mSocket, tpServerSocket->mLocalAddr, &tCommHandle );
    PROCESS_CHECK_ERROR_RET( tRetCode == 0, tResult, tRetCode );
    
	tpServerSocket->mExitFlag = false;
    while (!tpServerSocket->mExitFlag)
    {
        tTickCount = LINEKONG::GetTickCount( );
        tWaitTime  = tTickCount - tpServerSocket->mTickCount;
        
        tpServerSocket->LockData( );
        if ( tWaitTime >= SOCKET_TIMERINTERVAL )
        {
            if ( tpServerSocket->mNeedTimer )
            {
                tpServerSocket->OnTimer( ( uint32_t )tTickCount );
            }
            tpServerSocket->FlushBuffer( 0 );

            // 检查连接是否正常
            tpServerSocket->CheckAlive( 0, tTickCount );
            tpServerSocket->mTickCount = tTickCount;

            tWaitTime = SOCKET_TIMERINTERVAL;
        }
        else
        {
            tWaitTime = SOCKET_TIMERINTERVAL - tWaitTime;
        }
        tpServerSocket->UnlockData( );

        tCommHandleCount = epoll_wait( tpServerSocket->mhObject, tpEvents, EPOLL_MAX_CLIENT, 0 );
        usleep( 1 );
        for ( int n = 0; n < tCommHandleCount; n++ )
        {
            tCommHandle  = tpEvents[n].data.u32;
            tEventValue  = tpEvents[n].events;

            
            tpClientContext = tpServerSocket->GetClientContext( tCommHandle );
            
            // 这里是侦听端口。
            if (
                ( tCommHandle == tpServerSocket->mListenLinuxSocketContext.mClientID ) &&
                ( tEventValue  &  EPOLLIN )
            )
            {
                /// Accept
                tRetCode = tpServerSocket->Accept( );
                continue;
            }
            
            
            // 以下为非侦听端口。
            if ( tpClientContext == NULL )
            {
#ifdef DEBUG_SHOW_COMM_SERVER_INFO
                puts( "[EPOLL-COMM] Invalid CommHandle found!" );
#endif
                continue;
            }

            tpServerSocket->mTickCount = tTickCount;


            if ( tEventValue & EPOLLERR ) // Error has happened.
            {
                // TODO: Add your code to operate the error control!!!
                if ( tpClientContext->mSocket != INVALID_SOCKET )
                {
                    tpServerSocket->LockData( );
                    tpServerSocket->CloseSocket( tpClientContext, true );
                    tpServerSocket->UnlockData( );
                }
                continue;
            }
            tpServerSocket->LockData( );
            if ( tEventValue & EPOLLIN )
            {
                // New data in
                tpServerSocket->Recv( tpClientContext, &tRecvBytes, 0 );
                if ( tRecvBytes == 0 )
                {
                    
                    tpServerSocket->CloseSocket( tpClientContext, false );
                    
                }
                else if ( tRecvBytes > 0 )
                {
                    tpServerSocket->OnRecv( tCommHandle, tRecvBytes );
                }

            }

            if ( tEventValue & EPOLLOUT )
            {
                // You can send your data.....
                if ( tWaitTime >= SOCKET_TIMERINTERVAL )
                {
                    if ( tCommHandle != -1 )
                    {
                        tpServerSocket->FlushBuffer( tCommHandle );
                        tpServerSocket->CheckAlive( tCommHandle, tTickCount );
                    }
                    tWaitTime = SOCKET_TIMERINTERVAL;
                }
            }
            tpServerSocket->UnlockData( );
        }
    }
    tResult = 0;
ExitError:
	tpServerSocket->AwakeEvent();
    if ( tpEvents != NULL )
    {
        delete tpEvents;
        tpEvents = NULL;
    }

    return tResult;
}

#endif
