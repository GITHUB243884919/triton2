// ********************************************************************** //
// System Socket Class 1.0 by Roland
// ********************************************************************** //

#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <vector>
#include <map>

#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "mswsock" )

namespace System
{

#define SOCKET_BUFFERSIZE_ACCEPT	10240
#define SOCKET_BUFFERSIZE_SEND		81920
#define SOCKET_BUFFERSIZE_RECV		81920
#define SOCKET_OPERATION_ACCEPT		1
#define SOCKET_OPERATION_SEND		2
#define SOCKET_OPERATION_RECV		3
#define SOCKET_OPERATION_CLOSE		4
#define SOCKET_ACCEPTNUMBER			1
#define SOCKET_TIMERINTERVAL		20
#define SOCKET_PINGINTERVAL			5000

struct	COverlapped;
struct	CClientContext;
struct	CListenSocketContext;
class	CSocket;
class	CAsyncSocket;
class	CClientSocket;
class	CServerSocket;
class	CPeerSocket;
class	CRawSocket;

// ********************************************************************** //
// CSocketManager
// ********************************************************************** //

struct CSocketManager
{
	WSADATA		mWSAData;

	CSocketManager( )
		{ ::WSAStartup( 0x0202, &mWSAData ); }
	~CSocketManager( )
		{ ::WSACleanup( ); }
};

extern CSocketManager gSocketManager;

// ********************************************************************** //
// COverlapped
// ********************************************************************** //

struct COverlapped
{
	DWORD		mInternal;
	DWORD		mInternalHigh;
	DWORD		mOffset;
	DWORD		mOffsetHigh;
	HANDLE		mhEvent;
	DWORD		mOperation;
};

// ********************************************************************** //
// CClientContext
// ********************************************************************** //

struct CClientContext
{
	// 客户端表示连接的唯一，不会被重复使用
	static int sClientID;	

	SOCKET		mSocket;
	SOCKADDR_IN	mLocalAddr;
	SOCKADDR_IN	mRemoteAddr;
	DWORD		mKeepAlive;
	DWORD		mTickCount;
	DWORD		mBytesSend;
	DWORD		mBytesRecv;
	DWORD		mSendLength;
	DWORD		mRecvLength;
	BYTE		mSendBuffer[ SOCKET_BUFFERSIZE_SEND ];
	BYTE		mRecvBuffer[ SOCKET_BUFFERSIZE_RECV ];
	OVERLAPPED	mSendOverlapped;
	OVERLAPPED	mRecvOverlapped;
	int			mClientID;
	CClientContext( )
		{ ::memset( this, 0, sizeof( CClientContext ) ); sClientID ++; mClientID = sClientID; }

	~CClientContext( )
		{  }

	DWORD GetLocalIP( )			{ return (DWORD) mLocalAddr.sin_addr.s_addr; }
	char* GetLocalIPString( )	{ return ::inet_ntoa( mLocalAddr.sin_addr ); }
	WORD  GetLocalPort( )		{ return (WORD) ::ntohs( mLocalAddr.sin_port ); }

	DWORD GetRemoteIP( )		{ return (DWORD) mRemoteAddr.sin_addr.s_addr; }
	char* GetRemoteIPString( )	{ return ::inet_ntoa( mRemoteAddr.sin_addr ); }
	WORD  GetRemotePort( )		{ return (WORD) ::ntohs( mRemoteAddr.sin_port ); }

	int  GetCommHandle( )		{ return mClientID; }
};

// ********************************************************************** //
// CListenSocketContext
// ********************************************************************** //

struct CListenSocketContext
{
	SOCKET		mSocket;
	BYTE		mAcceptBuffer[ SOCKET_BUFFERSIZE_ACCEPT ];
	OVERLAPPED	mAcceptOverlapped;

	CListenSocketContext( )
		{ ::memset( this, 0, sizeof( CListenSocketContext ) ); }
};

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
		{ return mCheckSum = ~( ( (unsigned int) mMessageSize << 16 ) | (DWORD) mMessageID ); }

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
	DWORD		mTickCount;

	CSocketPingMessage( ) : CSocketMessage( sizeof( CSocketPingMessage ), 0xFFFF ),
		mTickCount( ::timeGetTime( ) ) { }
};

// ********************************************************************** //
// CSocket
// ********************************************************************** //

class CSocket
{
protected:
	SOCKET		mSocket;

public:
	DWORD		mBytesSend;
	DWORD		mBytesRecv;

protected:
	CSocket( );
public:
	virtual ~CSocket( );

public:
	static DWORD IPString2Dword( const char* pString )
		{ return (DWORD) ::inet_addr( pString ); }

	static char* IPDword2String( DWORD vIP )
		{ in_addr tAddr; tAddr.s_addr = vIP; return ::inet_ntoa( tAddr ); }

	static char* GetAddrIPString( SOCKADDR_IN& rSockAddr )
		{ return ::inet_ntoa( rSockAddr.sin_addr ); }

	static WORD	GetAddrPort( SOCKADDR_IN& rSockAddr )
		{ return (WORD) ::ntohs( rSockAddr.sin_port ); }

	static WORD		Net2HostW( WORD vValue )	{ return (WORD) ::ntohs( vValue ); }
	static DWORD	Net2HostD( DWORD vValue )	{ return (DWORD) ::ntohl( vValue ); }
	static WORD		Host2NetW( WORD vValue )	{ return (WORD) ::htons( vValue ); }
	static DWORD	Host2NetD( DWORD vValue )	{ return (DWORD) ::htonl( vValue ); }

	static void		GetLocalName( char* pBuffer, int vLength );
	static DWORD	GetLocalIP( );
	static char*	GetLocalIPString( );
	static HOSTENT*	GetHostByName( char* pName );
	static char*	GetServiceName( WORD vPort, char* pProtocol );
	static WORD		GetServicePort( char* pName, char* pProtocol );
	static char*	GetProtocolName( int vNumber );
	static WORD		GetProtocolNumber( char* pName );

	static void CreateSockAddr( SOCKADDR_IN* pSockAddr, const char* pAddress );
	static void CreateSockAddr( SOCKADDR_IN* pSockAddr, const char* pAddress, WORD vPort );
	static void CreateSockAddr( SOCKADDR_IN* pSockAddr, DWORD vIP, WORD vPort );

public:
	operator SOCKET( ) const { return mSocket; }

	int GetBufferLength( SOCKET vSocket = 0 );
	int GetInterface( SOCKADDR_IN* pSockAddr, int vIndex );
	int GetAddress( SOCKADDR_IN* pSockAddr );

	int SetOption( int vLevel, int vName, void* pValue, int vLength, SOCKET vSocket = 0 );
	int GetOption( int vLevel, int vName, void* pValue, int* pLength, SOCKET vSocket = 0 );
	int SocketControl( int vCode, void* pInBuffer, int vInSize, void* pOutBuffer, int vOutSize, SOCKET vSocket = 0 );
};

// ********************************************************************** //
// CAsyncSocket
// ********************************************************************** //

class CAsyncSocket : public CSocket
{
protected:
	HANDLE				mhObject;		// 线程事件句柄，用于等待网络消息
	HANDLE				mhThread;		// 线程句柄
	HANDLE				mhEvent;		// 线程事件句柄，用于结束网络线程
	DWORD				mThreadID;
	DWORD				mTickCount;
	DWORD				mLastTickCount;
	DWORD				mIsMessage;
	bool				mNeedTimer;
	CRITICAL_SECTION*	mpDataCS;

public:
	CAsyncSocket( );
public:
	virtual ~CAsyncSocket( );

public:
	HANDLE	GetThreadObject( ) const	{ return mhObject; }
	HANDLE	GetThreadHandle( ) const	{ return mhThread; }
	DWORD	GetThreadID( ) const		{ return mThreadID; }

	void SetTimer( bool vTimer )				{ mNeedTimer = vTimer; }
	void SetLock( CRITICAL_SECTION* pDataCS )	{ mpDataCS = pDataCS; }

	void LockData( ) const
		{ if ( mpDataCS != 0 ) ::EnterCriticalSection( mpDataCS ); }

	void UnlockData( ) const
		{ if ( mpDataCS != 0 ) ::LeaveCriticalSection( mpDataCS ); }

	void Create( bool vIsMessage );
	void Close( );

public:
	virtual int OnEncrypt( void* pBuffer, int vLength )	{ return 0; }
	virtual int OnDecrypt( void* pBuffer, int vLength )	{ return 0; }
};

// ********************************************************************** //
// CClientSocket
// ********************************************************************** //

class CClientSocket : public CSocket
{
public:
	HANDLE		mhObject;		// 事件句柄，用于等待网络消息
	DWORD		mTickCount;
	DWORD		mLastTickCount;
	DWORD		mRecvLength;
	BYTE		mRecvBuffer[ SOCKET_BUFFERSIZE_RECV ];
	DWORD		mPing;
	bool		mConnected;
	SOCKADDR_IN mHostAddr;
public:
	CClientSocket( );
public:
	virtual ~CClientSocket( );

protected:
	void Close( );

public:
	bool EventProcess( );
	int Create( );
	int Connect( const char* pAddress );
	int Connect( const char* pAddress, WORD vPort );
	int Connect( DWORD vIP, WORD vPort );
	int Connect( SOCKADDR_IN* pSockAddr );
	int Send( void* pBuffer, int vLength, DWORD vFlags = 0 );
	int Recv( void* pBuffer, int vLength, int* pBytes = 0, DWORD vFlags = 0 );
	int Shutdown( );
	int SelectEvent( int vEvent );

	DWORD GetServerIP( );
	char* GetServerIPString( );
	WORD  GetServerPort( );
	DWORD GetConnectTime( );

	int SendMessage( CSocketMessage& rMessage )
		{ return Send( &rMessage, rMessage.mMessageSize ); }

public:
	virtual int OnConnect( int vCode );
	virtual int OnSend( int vCode );
	virtual int OnRecv( int vCode );
	virtual int OnMessage( BYTE* pMessage, int vMsgLen );
	virtual int OnClose( int vCode );
};

// ********************************************************************** //
// CServerSocket
// ********************************************************************** //

class CServerSocket : public CAsyncSocket
{
public:
	typedef std::pair< int, CClientContext* >			CLIENTTYPE;
	typedef std::map< int, CClientContext* >			CLIENTCONTAINER;
	typedef std::map< int, CClientContext* >::iterator	CLIENTITER;

public:
	DWORD									mKeepAlive;
	DWORD									mLastTickCount;
	int										mCurConnection;
	int										mMaxConnection;
	CLIENTCONTAINER							mClientSockets;
	CListenSocketContext*					mpListenSockets;

public:
	CServerSocket( );
public:
	virtual ~CServerSocket( );

protected:
	static DWORD __stdcall IOCPThread( void* pParam );

protected:
	CClientContext* GetClientContext( int vCommHandle );

public:
	DWORD	GetKeepAlive( ) const		{ return mKeepAlive; }
	int		GetCurConnection( ) const	{ return mCurConnection; }
	int		GetMaxConnection( ) const	{ return mMaxConnection; }

	int Create( const char* pAddress, bool vIsMessage, int vMaxConnection = 4096, DWORD vKeepAlive = 1800000 );
	int Create( const char* pAddress, WORD vPort, bool vIsMessage, int vMaxConnection = 4096, DWORD vKeepAlive = 1800000 );
	int Create( DWORD vIP, WORD vPort, bool vIsMessage, int vMaxConnection = 4096, DWORD vKeepAlive = 1800000 );
	int Create( SOCKADDR_IN* pSockAddr, bool vIsMessage, int vMaxConnection = 4096, DWORD vKeepAlive = 1800000 );
	int Listen( int vQueue = 5 );
	int Accept( );
	int Send( int vCommHandle, void* pBuffer, int vLength, OVERLAPPED* pOverlapped = 0, DWORD vFlags = 0 );
	int Recv( int vCommHandle, void* pBuffer, int vLength, OVERLAPPED* pOverlapped = 0, int* pBytes = 0, DWORD vFlags = 0 );
	int Close( );

	int SetKeepAlive( int vCommHandle, DWORD vKeepAliveInterval );
	int CheckAlive( int vCommHandle, DWORD vTickCount );
	int FillBuffer( int vCommHandle, void* pBuffer, int vLength );
	int FlushBuffer( int vCommHandle, DWORD vFlags = 0 );
	int CloseSocket( int vCommHandle );
	int ReleaseSocket( int vCommHandle );
	void SendMessage( int vCommHandle, CSocketMessage& rMessage )
		{ Send( vCommHandle, &rMessage, rMessage.mMessageSize ); }
	void FillMessage( int vCommHandle, CSocketMessage& rMessage )
		{ FillBuffer( vCommHandle, &rMessage, rMessage.mMessageSize ); }

	CClientContext*  GetClientContext( DWORD vCommHandle );

	bool CheckContext( int vCommHandle );
	void DumpClient( );

public:
	virtual int OnAccept( int vCommHandle );
	virtual int OnSend( int vCommHandle, int vBytes );
	virtual int OnRecv( int vCommHandle, int vBytes );
	virtual int OnMessage( int vCommHandle, BYTE* pMessage, int vMsgLen );
	virtual int OnTimer( DWORD vTickCount );
	virtual int OnClose( int vCommHandle );
};

};