
#ifndef __EPOLL_SOCKET_H__
#define __EPOLL_SOCKET_H__
#ifdef LINUX
#include "macro.h"
#include <map>
#include <vector>
#include <sys/select.h>
#ifndef INVALID_SOCKET
#   define INVALID_SOCKET ~0
#endif


#ifndef SOCKET_BUFFERSIZE_SEND
#   define SOCKET_BUFFERSIZE_SEND 81920
#endif

#ifndef SOCKET_BUFFERSIZE_RECV
#   define SOCKET_BUFFERSIZE_RECV 81920
#endif


#ifndef SOCKET_PINGINTERVAL
#   define SOCKET_PINGINTERVAL 100000
#endif

#ifndef SOCKET_BUFFERSIZE_ACCEPT
#   define SOCKET_BUFFERSIZE_ACCEPT 10240
#endif

#ifndef SOCKET_TIMERINTERVAL
#   define SOCKET_TIMERINTERVAL 0
#endif
#define EPOLL_MAX_CLIENT 4096

#ifdef WIN32
#   define socklen_t int
#   define pthread_t int 
#else
#   define SOCKET int
#   include <arpa/inet.h>
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netdb.h>
#   include <unistd.h>
#   include <pthread.h>
#   include <sys/epoll.h>
//#   define min(X, Y) X <= Y ? X : Y
using namespace std;
#endif
#define LISTENER_COMM_HANDLE     0
#define INVALID_COMM_HANDLE      -1
namespace LINEKONG
{
    uint64_t GetTickCount();
}

typedef unsigned char BYTE;

// ********************************************************************** //
// CSocketMessage
// ********************************************************************** //

struct CSocketMessage
{
    unsigned short	mMessageSize;
    unsigned short	mMessageID;
    unsigned int	mCheckSum;
    CSocketMessage( unsigned short vMessageSize, unsigned short vMessageID )
        : mMessageSize( vMessageSize ), mMessageID( vMessageID ), mCheckSum( BuildCheckSum( ) ) { }

    unsigned int BuildCheckSum( )
    { return mCheckSum = ~( ( (unsigned int) mMessageSize << 16 ) | ( uint32_t ) mMessageID ); }

    void SetMessageSize( unsigned short vMessageSize )
    { mMessageSize = vMessageSize; mCheckSum = BuildCheckSum( ); }

    void SetMessageID( unsigned short vMessageID )
    { mMessageID = vMessageID; mCheckSum = BuildCheckSum( ); }

    bool CheckHeader( )
    { return mCheckSum == BuildCheckSum( ); }
};

// ********************************************************************** //
// CSocketPingMessage
// ********************************************************************** //

struct CSocketPingMessage : public CSocketMessage
{
    uint32_t mTickCount;
    CSocketPingMessage( ) : CSocketMessage( sizeof( CSocketPingMessage ), 0xFFFF ),
        mTickCount( LINEKONG::GetTickCount( ) ) { }
};

// 该协议专用于唯一连接号的获取，该协议由客户端主动发起，并由服务器端分配。
struct CGatewayLoginMessage : public CSocketMessage
{
    int mConnectionID;
    CGatewayLoginMessage( ): CSocketMessage( sizeof( CGatewayLoginMessage ), 0xFFFE ), mConnectionID( 0 )
    {

    };
};

struct CClientLinuxContext
{
    // 客户端表示连接的唯一，不会被重复使用
    static int sClientID;	

    SOCKET		mSocket;
    sockaddr_in	mLocalAddr;
    sockaddr_in	mRemoteAddr;
    uint64_t	mKeepAlive;
    uint64_t	mTickCount;

    /* 以下四个参数专用于统计 */
    uint32_t	mBytesSend;   // 总发送的字节数。
    uint32_t	mBytesRecv;   // 总接收的字节数。
    uint32_t	mPackageSend; // 总发送的包数。
    uint32_t	mPackageRecv; // 总接收的包数。

    uint32_t	mSendLength;
    uint32_t	mRecvLength;
    uint8_t		mSendBuffer[ SOCKET_BUFFERSIZE_SEND ];
    uint8_t		mRecvBuffer[ SOCKET_BUFFERSIZE_RECV ];

    
    int			mClientID;

    int         mConnectionID; // 连接唯一号。当有连接上来时，将自动分配一个。

    CClientLinuxContext( )
    { ::memset( this, 0, sizeof( CClientLinuxContext ) ); sClientID ++; mClientID = sClientID; }

    ~CClientLinuxContext( )
    {  }

    uint32_t    GetLocalIP( )	    { return (uint32_t) mLocalAddr.sin_addr.s_addr; }
    char*       GetLocalIPString( )	{ return ::inet_ntoa( mLocalAddr.sin_addr ); }
    uint16_t    GetLocalPort( )		{ return (uint16_t) ::ntohs( mLocalAddr.sin_port ); }

    uint32_t    GetRemoteIP( )		{ return (uint32_t) mRemoteAddr.sin_addr.s_addr; }
    char*       GetRemoteIPString( ){ return ::inet_ntoa( mRemoteAddr.sin_addr ); }
    uint16_t    GetRemotePort( )	{ return (uint16_t) ::ntohs( mRemoteAddr.sin_port ); }

    int  GetCommHandle( )		{ return mClientID; }

    int  ShowInfo( )
    {
        puts( "====================================================" );
        printf("CommHandle    = %d\n", mClientID);
        printf("Socket        = %d\n", mSocket);
        printf("LocalAddress  = %s:%u\n", GetLocalIPString( ), GetLocalPort( ) );
        printf("RemoteAddress = %s:%u\n", GetRemoteIPString( ), GetRemotePort( ) );
        puts( "====================================================" );
        return 0;
    };
};

// ********************************************************************** //
// CListenSocketContext
// ********************************************************************** //

struct CListenLinuxSocketContext
{
    SOCKET		mSocket;
    uint8_t		mAcceptBuffer[ SOCKET_BUFFERSIZE_ACCEPT ];
    int			mClientID;

    CListenLinuxSocketContext( )
    { ::memset( this, 0, sizeof( CListenLinuxSocketContext ) ); mClientID = LISTENER_COMM_HANDLE; }

    int  GetCommHandle( )		{ return mClientID; }
};


class CLinuxSocket
{
public:
    CLinuxSocket( );
    ~CLinuxSocket( );

    static uint32_t IPString2Dword( const char* pString )
    { return (uint32_t) ::inet_addr( pString ); }

    static char* IPDword2String( uint32_t vIP )
    { in_addr tAddr; tAddr.s_addr = vIP; return ::inet_ntoa( tAddr ); }

    static char* GetAddrIPString( sockaddr_in& rSockAddr )
    { return ::inet_ntoa( rSockAddr.sin_addr ); }

    static uint16_t	GetAddrPort( sockaddr_in& rSockAddr )
    { return (uint16_t) ::ntohs( rSockAddr.sin_port ); }

    operator SOCKET( ) const { return mSocket; }


    static uint16_t Net2HostW( uint16_t vValue ) { return (uint16_t) ::ntohs( vValue ); }
    static uint32_t Net2HostD( uint32_t vValue ) { return (uint32_t) ::ntohl( vValue ); }
    static uint16_t Host2NetW( uint16_t vValue ) { return (uint16_t) ::htons( vValue ); }
    static uint32_t Host2NetD( uint32_t vValue ) { return (uint32_t) ::htonl( vValue ); }

    static void     GetLocalName( char* pBuffer, int vLength );
    static uint32_t GetLocalIP( );
    static char*    GetLocalIPString( );
    static hostent*	GetHostByName( char* pName );
    static char*    GetServiceName( uint16_t vPort, char* pProtocol );
    static uint16_t GetServicePort( char* pName, char* pProtocol );
    static char*    GetProtocolName( int vNumber );
    static uint16_t GetProtocolNumber( char* pName );

    static void CreateSockAddr( sockaddr_in* pSockAddr, const char* pAddress );
    static void CreateSockAddr( sockaddr_in* pSockAddr, const char* pAddress, uint16_t vPort );
    static void CreateSockAddr( sockaddr_in* pSockAddr, uint32_t vIP, uint16_t vPort );

public:
    int         GetBufferLength( SOCKET vSocket = 0 );
    int         GetInterface( sockaddr_in* pSockAddr, int vIndex );
    int         GetAddress( sockaddr_in* pSockAddr );

    int         SetOption( int vLevel, int vName, void* pValue, socklen_t vLength, SOCKET vSocket = 0 );
    int         GetOption( int vLevel, int vName, void* pValue, socklen_t* pLength, SOCKET vSocket = 0 );
    
protected:
    SOCKET        mSocket;
    sockaddr_in   mLocalAddr;
public:
    unsigned long mBytesSend; // 发送的总字节数。
    unsigned long mBytesRecv; // 接收的总字节数。
};

// ********************************************************************** //
// CAsyncSocket
// ********************************************************************** //

class CAsyncLinuxSocket : public CLinuxSocket
{
public:
    int             mhObject;		// 线程事件句柄，用于等待网络消息
    pthread_t       mhThread;		// 线程句柄
    int             mhEvent;		// 线程事件句柄，用于结束网络线程
    uint32_t        mThreadID;
    uint64_t        mTickCount;
    uint64_t        mLastTickCount;
    uint32_t        mIsMessage;
    bool            mNeedTimer;
#ifdef WIN32
    CRITICAL_SECTION *mpDataCS;
#else
    pthread_mutex_t  *mpDataCS;
#endif

#ifdef LINUX
	pthread_cond_t  mCondition;
	pthread_mutex_t mMutex;
#endif
	int mExitFlag;
	int SetEvent();
	int WaitForEvent();
	int AwakeEvent();
	int DestroyEvent();
public:
    CAsyncLinuxSocket( );
public:
    virtual ~CAsyncLinuxSocket( );

public:
    int      GetThreadObject( ) const	{ return mhObject; }
    int      GetThreadHandle( ) const	{ return mhThread; }
    uint32_t GetThreadID( ) const		{ return mThreadID; }

    void SetTimer( bool vTimer )				{ mNeedTimer = vTimer; }

    
    void LockData( );

    void UnlockData( );
#ifdef WIN32
    void SetLock( CRITICAL_SECTION* pDataCS )
    {
        mpDataCS = pDataCS;
    }
#else
    void SetLock( pthread_mutex_t *pDataCS )
    {
        mpDataCS = pDataCS;
    }
#endif
    void Create( bool vIsMessage );
    void Close( );

public:
    virtual int OnEncrypt( void* pBuffer, int vLength )	{ return 0; }
    virtual int OnDecrypt( void* pBuffer, int vLength )	{ return 0; }
};



// ********************************************************************** //
// CClientSocket
// ********************************************************************** //

class CClientLinuxSocket : public CLinuxSocket
{
public:

    int         mhObject;		// 事件句柄，用于等待网络消息
    uint64_t    mTickCount;
    uint64_t	mLastTickCount;
    uint32_t	mRecvLength;
    uint8_t		mRecvBuffer[ SOCKET_BUFFERSIZE_RECV ];
    uint64_t    mPing;
    bool		mConnected;
    sockaddr_in mHostAddr;

    int         mSelectedEvents;
public:
    CClientLinuxSocket( );
public:
    virtual ~CClientLinuxSocket( );

protected:
    void Close( );

public:
    bool EventProcess( );
    int Create( );
    int Connect( const char* pAddress );
    int Connect( const char* pAddress, uint16_t vPort );
    int Connect( uint32_t vIP, uint16_t vPort );
    int Connect( sockaddr_in* pSockAddr );
    int Send( void* pBuffer, int vLength, uint32_t vFlags = 0 );
    int Recv( void* pBuffer, int vLength, int* pBytes = 0, uint32_t vFlags = 0 );
    int Shutdown( );
    int SelectEvent( int vEvent );

    uint32_t GetServerIP( );
    char*    GetServerIPString( );
    uint16_t GetServerPort( );
    uint32_t GetConnectTime( );

    int SendMessage( CSocketMessage *pMessage )
    {
        return Send( pMessage, pMessage->mMessageSize );
    }

        
public:
    virtual int OnConnect( int vCode );
    virtual int OnSend( int vCode );
    virtual int OnRecv( int vCode );
    virtual int OnMessage( BYTE* pMessage, int vMsgLen );
    virtual int OnClose( int vCode );

protected:
    fd_set mReadSet;
    fd_set mWriteSet;
    fd_set mExcSet;
    struct timeval mTimeVal;
};


class  CEPollSocket : public CAsyncLinuxSocket
{
public:
    CEPollSocket( );
    ~CEPollSocket( );
public:
    typedef std::pair< int, CClientLinuxContext* >			CLIENTTYPE;
    typedef std::map< int, CClientLinuxContext* >			CLIENTCONTAINER;

    typedef std::map< int, CClientLinuxContext* >::iterator	CLIENTITER;

    typedef std::map< int, void * > CONNECTION_TEMP_BUFFER_CONTAINER;
    
    uint32_t mKeepAlive;
    uint32_t mLastTickCount;
    int      mCurConnection;
    int      mMaxConnection;
    int      mConnectionIDSeed;

    int      mBytesSent;
    int      mBytesRecv;
    int      mPackagesSent;
    int      mPackagesRecv;

    CLIENTCONTAINER                  mClientSockets;
    CONNECTION_TEMP_BUFFER_CONTAINER mConnectionTempBuffers;

    // 将释放的，不用的上下文移到该处。
    std::vector< CClientLinuxContext* >     mUnusedClientContextContainter;
    
    int ShowInfo( )
    {
        CClientLinuxContext* tpClientLinuxContext = NULL;
        CLIENTITER           tClientContainerIter = mClientSockets.begin( );
        while ( tClientContainerIter != mClientSockets.end( ) )
        {
            tpClientLinuxContext = tClientContainerIter->second;
            ASSERT( tpClientLinuxContext != NULL && ( 0 || "Element must be not null in ClientSocketContainer!!!" ) );
            tpClientLinuxContext->ShowInfo( );
            tClientContainerIter++;
        }
        return 0;
    };  
protected:
    CClientLinuxContext *GetClientContext( int vCommHandle );

    int AddEPollSocket( SOCKET vSocket, sockaddr_in &vrRemoteAddr, int *vpCommHandle );

    int RemoveClientLinuxContext( int vCommHandle );

    // int RemoveUnsedClientLinuxContext( int *vpEffectedCount  );

    int RemoveEPollSocket( int vCommHandle );
    int RemoveEPollSocket( CClientLinuxContext* vpContext );

    int CheckConnectionID( int vOldConnectionID, int* vpNewConnectionID );


    char mLocalAddrString[ 32 ];
protected:
    static int EPOLLThread( void* pParam );
    CListenLinuxSocketContext mListenLinuxSocketContext;

public:
    uint32_t    GetKeepAlive( )     const   { return mKeepAlive; }
    int	        GetCurConnection( ) const   { return mCurConnection; }
    int         GetMaxConnection( ) const   { return mMaxConnection; }

    
    int Create( const char* pAddress, bool vIsMessage, int vMaxConnection = 4096, uint32_t vKeepAlive = 1800000 );
    int Create( const char* pAddress, uint16_t vPort, bool vIsMessage, int vMaxConnection = 4096, uint32_t vKeepAlive = 1800000 );
    int Create( uint32_t vIP, uint16_t vPort, bool vIsMessage, int vMaxConnection = 4096, uint32_t vKeepAlive = 1800000 );
    int Create( sockaddr_in* pSockAddr, bool vIsMessage, int vMaxConnection = 4096, uint32_t vKeepAlive = 1800000 );
    int Listen( int vQueue = 5 );
    int Accept( );
    int Send( int vCommHandle, void* pBuffer, int vLength, uint32_t vFlags = 0 );
    int Send( CClientLinuxContext *vpClientContext, uint32_t vFlags = 0 );
    int Recv( int vCommHandle, void* pBuffer, int vLength, int* pBytes = 0, uint32_t vFlags = 0 );
    int Recv( CClientLinuxContext *vpClientContext, int* pBytes = 0, uint32_t vFlags = 0 );
    int Close( );

    int SetKeepAlive( int vCommHandle, uint32_t vKeepAliveInterval );
    int CheckAlive( int vCommHandle, uint32_t vTickCount );
    int FillBuffer( int vCommHandle, void* pBuffer, int vLength );
    int FlushBuffer( int vCommHandle, uint32_t vFlags = 0 );

    int CloseSocket( int vCommHandle, bool vIsLost );
    int CloseSocket( CClientLinuxContext *vpClientContext, bool vIsLost );
    int ReleaseSocket( int vCommHandle );
    
    int ShowFlowInfo( );

    void SendMessage( int vCommHandle, CSocketMessage* pMessage )
    {
        Send( vCommHandle, pMessage, pMessage->mMessageSize );
    }
    void FillMessage( int vCommHandle, CSocketMessage* pMessage )
    {
        FillBuffer( vCommHandle, pMessage, pMessage->mMessageSize );
    }

    bool CheckContext( int vCommHandle );
    void DumpClient( );

public:
    virtual int OnAccept( int vCommHandle );
    virtual int OnSend( int vCommHandle, int vBytes );
    virtual int OnRecv( int vCommHandle, int vBytes );
    virtual int OnMessage( int vCommHandle, BYTE* pMessage, int vMsgLen );
    virtual int OnTimer( uint32_t vTickCount );
    virtual int OnClose( int vCommHandle, bool vIsLost );
};

#endif

#endif

