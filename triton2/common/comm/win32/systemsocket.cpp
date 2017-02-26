// ********************************************************************** //
// SystemSocket.cpp
// Windows下通讯层的IOCP实现，包括服务器和客户端
// Author: Black
// ********************************************************************** //

#include "SystemSocket.h"
using namespace System;

int CClientContext::sClientID = 0;
// ********************************************************************** //
// CSocketManager
// ********************************************************************** //

CSocketManager gSocketManager;

// ********************************************************************** //
// CSocket
// ********************************************************************** //

CSocket::CSocket( )
{
	mSocket		= INVALID_SOCKET;
	mBytesSend	= 0;
	mBytesRecv	= 0;
}

CSocket::~CSocket( )
{
}

void CSocket::GetLocalName( char* pBuffer, int vLength )
{
	::gethostname( pBuffer, vLength );
}

DWORD CSocket::GetLocalIP( )
{
	char tName[ 4096 ] = { 0 };
	::gethostname( tName, 4096 );

	hostent* tpHostent = ::gethostbyname( tName );

	return (DWORD) ( (in_addr*) tpHostent->h_addr_list[0] )->s_addr;
}

char* CSocket::GetLocalIPString( )
{
	return IPDword2String( GetLocalIP( ) );
}

HOSTENT* CSocket::GetHostByName( char* pName )
{
	return ::gethostbyname( pName );
}

char* CSocket::GetServiceName( WORD vPort, char* pProtocol )
{
	servent* tpServent = ::getservbyport( ::htons( vPort ), pProtocol );

	if ( tpServent == 0 )
		return 0;

	return tpServent->s_name;
}

WORD CSocket::GetServicePort( char* pName, char* pProtocol )
{
	servent* tpServent = ::getservbyname( pName, pProtocol );

	if ( tpServent == 0 )
		return 0;

	return ::ntohs( tpServent->s_port );
}

char* CSocket::GetProtocolName( int vNumber )
{
	protoent* tpProtoent = ::getprotobynumber( vNumber );

	if ( tpProtoent == 0 )
		return 0;

	return tpProtoent->p_name;
}

WORD CSocket::GetProtocolNumber( char* pName )
{
	protoent* tpProtoent = ::getprotobyname( pName );

	if ( tpProtoent == 0 )
		return 0;

	return tpProtoent->p_proto;
}

void CSocket::CreateSockAddr( SOCKADDR_IN* pSockAddr, const char* pAddress )
{
	if ( pSockAddr == 0 || pAddress == 0 )
		return;

	const char* tpPort = ::strchr( pAddress, ':' );

	if ( tpPort != 0 )
	{
		char tAddress[ 1024 ] = { 0 };
		::memcpy( tAddress, pAddress, tpPort - pAddress );

		CreateSockAddr( pSockAddr, tAddress, (WORD) ::atoi( tpPort + 1 ) );
	}
	else
	{
		CreateSockAddr( pSockAddr, pAddress, 0 );
	}
}

void CSocket::CreateSockAddr( SOCKADDR_IN* pSockAddr, const char* pAddress, WORD vPort )
{
	if ( pSockAddr == 0 || pAddress == 0 )
		return;

	DWORD tIP = 0;

	if ( ::strcmp( pAddress, "localhost" ) == 0 || ::strcmp( pAddress, "127.0.0.1" ) == 0 )
		tIP = GetLocalIP( );
	else
		tIP = ::inet_addr( pAddress );

	if ( tIP == INADDR_NONE )
	{
		HOSTENT* tpHostent = ::gethostbyname( pAddress );

		if ( tpHostent != 0 )
			tIP = ( (in_addr*) tpHostent->h_addr_list[0] )->s_addr;
	}

	CreateSockAddr( pSockAddr, tIP, vPort );
}

void CSocket::CreateSockAddr( SOCKADDR_IN* pSockAddr, DWORD vIP, WORD vPort )
{
	if ( pSockAddr == 0 )
		return;

	::memset( pSockAddr, 0, sizeof( SOCKADDR_IN ) );

	pSockAddr->sin_family		= AF_INET;
	pSockAddr->sin_port			= ::htons( vPort );
	pSockAddr->sin_addr.s_addr	= vIP;
}

int CSocket::GetBufferLength( SOCKET vSocket )
{
	if ( vSocket == 0 )
		vSocket = mSocket;

	int tLength = 0; DWORD tBytes = 0;
	if ( ::WSAIoctl( vSocket, FIONREAD, 0, 0, &tLength, 4, &tBytes, 0, 0 ) == SOCKET_ERROR )
		return -1;

	return tLength;
}

int CSocket::GetInterface( SOCKADDR_IN* pSockAddr, int vIndex )
{
	BYTE tBuffer[ 4096 ]; DWORD tBytes = 0;
	if ( ::WSAIoctl( mSocket, SIO_ADDRESS_LIST_QUERY, 0, 0, tBuffer, 4096, &tBytes, 0, 0 ) == SOCKET_ERROR )
		return ::GetLastError( );

	SOCKET_ADDRESS_LIST* tpList = (SOCKET_ADDRESS_LIST*) tBuffer;
	if ( vIndex >= tpList->iAddressCount )
		return -1;

	*pSockAddr = *( (SOCKADDR_IN*) tpList->Address[ vIndex ].lpSockaddr );

	return 0;
}

int CSocket::GetAddress( SOCKADDR_IN* pSockAddr )
{
	int tSize = sizeof( SOCKADDR_IN );
	if ( ::getsockname( mSocket, (SOCKADDR*) pSockAddr, &tSize ) == SOCKET_ERROR )
		return ::GetLastError( );

	return 0;
}

int CSocket::SetOption( int vLevel, int vName, void* pValue, int vLength, SOCKET vSocket )
{
	if ( vSocket == 0 )
		vSocket = mSocket;

	if ( ::setsockopt( vSocket, vLevel, vName, (char*) pValue, vLength ) == SOCKET_ERROR )
		return ::GetLastError( );

	return 0;
}

int CSocket::GetOption( int vLevel, int vName, void* pValue, int* pLength, SOCKET vSocket )
{
	if ( vSocket == 0 )
		vSocket = mSocket;

	if ( ::getsockopt( vSocket, vLevel, vName, (char*) pValue, pLength ) == SOCKET_ERROR )
		return ::GetLastError( );

	return 0;
}

int CSocket::SocketControl( int vCode, void* pInBuffer, int vInSize, void* pOutBuffer, int vOutSize, SOCKET vSocket )
{
	if ( vSocket == 0 )
		vSocket = mSocket;

	DWORD tBytes = 0;
	if ( ::WSAIoctl( vSocket, vCode, pInBuffer, vInSize, pOutBuffer, vOutSize, &tBytes, 0, 0 ) == SOCKET_ERROR )
		return ::GetLastError( );

	return 0;
}

// ********************************************************************** //
// CAsyncSocket
// ********************************************************************** //

CAsyncSocket::CAsyncSocket( )
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

CAsyncSocket::~CAsyncSocket( )
{
}

void CAsyncSocket::Create( bool vIsMessage )
{
	mBytesSend		= 0;
	mBytesRecv		= 0;
	mTickCount		= 0;
	mLastTickCount	= 0;
	mIsMessage		= vIsMessage;
}

void CAsyncSocket::Close( )
{
	if ( mhEvent != 0 )
        ::SetEvent( mhEvent );

	if ( mhThread != 0 && ::GetCurrentThreadId( ) != mThreadID )
	{
		mThreadID = 0;
		::WaitForSingleObject( mhThread, 10000 );
		::CloseHandle( mhThread );
	}

	if ( mhObject != 0 )
		::CloseHandle( mhObject );

	if ( mhEvent != 0 )
		::CloseHandle( mhEvent );

	mBytesSend		= 0;
	mBytesRecv		= 0;
	mhObject		= 0;
	mhThread		= 0;
	mhEvent			= 0;
	mThreadID		= 0;
	mTickCount		= 0;
	mLastTickCount	= 0;
	mIsMessage		= 0;
	::closesocket( mSocket );
}

// ********************************************************************** //
// CClientSocket
// ********************************************************************** //

CClientSocket::CClientSocket( )
{
	mhObject		= NULL;		// 事件句柄，用于等待网络消息
	mTickCount		= 0;
	mLastTickCount	= ::timeGetTime( );
	mRecvLength		= 0;
	mPing			= 0;
	mConnected		= false;
}

CClientSocket::~CClientSocket( )
{
	Close( );
}

bool CClientSocket::EventProcess( )
{
	if ( mhObject == NULL )
		return true;
	
	if ( mSocket == INVALID_SOCKET )
		return true;

	DWORD tTickCount = ::timeGetTime( );
	if ( tTickCount - mLastTickCount > SOCKET_PINGINTERVAL && mConnected )
	{
		//int tRet = SendMessage( CSocketPingMessage( ) );
		//if ( tRet != 0 )
		//{
		//	OnClose( tRet );
		//	Close( );
		//	return true;
		//}

		mLastTickCount = tTickCount;
	}

	if ( ::WaitForSingleObject( mhObject, 0 ) == WAIT_OBJECT_0 )
	{
		WSANETWORKEVENTS tEvents;
		int tRetVal = ::WSAEnumNetworkEvents( mSocket, mhObject, &tEvents );
		if ( tRetVal != 0 )
			return false;

		if ( tEvents.lNetworkEvents & FD_CONNECT )
		{
			if ( tEvents.iErrorCode[ FD_CONNECT_BIT ] == 0 )
				::WSAEventSelect( mSocket, mhObject, FD_READ | FD_WRITE | FD_CLOSE );

			OnConnect( tEvents.iErrorCode[ FD_CONNECT_BIT ] );
			if ( tEvents.iErrorCode[ FD_CONNECT_BIT ] != 0 )
				Close( );
			else
                mConnected = true;
		}

		if ( tEvents.lNetworkEvents & FD_WRITE )
			OnSend( tEvents.iErrorCode[ FD_WRITE_BIT ] );

		if ( tEvents.lNetworkEvents & FD_READ )
			OnRecv( tEvents.iErrorCode[ FD_READ_BIT ] );

		if ( tEvents.lNetworkEvents & FD_CLOSE )
		{
			OnClose( tEvents.iErrorCode[ FD_CLOSE_BIT ] );
			//Close( );
			return true;
		}
	}

	return false;
}

int CClientSocket::Create( )
{
	if ( ( mSocket = ::WSASocket( AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED ) ) == INVALID_SOCKET )
		return ::GetLastError( );

	int tRetVal = SelectEvent( FD_CONNECT | FD_CLOSE );

	if ( tRetVal != 0 ) return tRetVal;

	mRecvLength		= 0;
	mPing			= 0;
	mBytesSend		= 0;
	mBytesRecv		= 0;
	mTickCount		= 0;
	mLastTickCount	= 0;

	return 0;
}

int CClientSocket::Connect( const char* pAddress )
{
	SOCKADDR_IN tSockAddr;
	CreateSockAddr( &tSockAddr, pAddress );

	return Connect( &tSockAddr );
}

int CClientSocket::Connect( const char* pAddress, WORD vPort )
{
	SOCKADDR_IN tSockAddr;
	CreateSockAddr( &tSockAddr, pAddress, vPort );

	return Connect( &tSockAddr );
}

int CClientSocket::Connect( DWORD vIP, WORD vPort )
{
	SOCKADDR_IN tSockAddr;
	CreateSockAddr( &tSockAddr, vIP, vPort );

	return Connect( &tSockAddr );
}

int CClientSocket::Connect( SOCKADDR_IN* pSockAddr )
{
	if ( pSockAddr == 0 )
		return -1;

	mHostAddr = *pSockAddr;
	if ( ::WSAConnect( mSocket, (SOCKADDR*) pSockAddr, sizeof( SOCKADDR_IN ), 0, 0, 0, 0 ) == SOCKET_ERROR )
	{
		if ( ::GetLastError( ) != WSAEWOULDBLOCK )
			return ::GetLastError( );
	}

	return 0;
}

int CClientSocket::Send( void* pBuffer, int vLength, DWORD vFlags )
{
	WSABUF tWSABuffer;
	tWSABuffer.buf = (char*) pBuffer;
	tWSABuffer.len = vLength;

	do
	{
		DWORD tBytesSend = 0;
		while ( ::WSASend( mSocket, &tWSABuffer, 1, &tBytesSend, vFlags, 0, 0 ) == SOCKET_ERROR )
		{
			if ( ::GetLastError( ) != WSAEWOULDBLOCK )
				return ::GetLastError( );
		}

		tWSABuffer.buf += tBytesSend;
		tWSABuffer.len -= tBytesSend;

	}while ( tWSABuffer.len > 0 );

	mBytesSend += vLength;

	return 0;
}

int CClientSocket::Recv( void* pBuffer, int vLength, int* pBytes, DWORD vFlags )
{
	WSABUF tWSABuffer;
	tWSABuffer.buf = (char*) pBuffer;
	tWSABuffer.len = vLength;

	DWORD tBytesRecv = 0;
	if ( ::WSARecv( mSocket, &tWSABuffer, 1, &tBytesRecv, &vFlags, 0, 0 ) == SOCKET_ERROR )
	{
		if ( ::GetLastError( ) != WSAEWOULDBLOCK )
			return ::GetLastError( );
	}

	if ( pBytes != 0 )
		*pBytes = tBytesRecv;

	mBytesRecv += tBytesRecv;

	return 0;
}

int CClientSocket::Shutdown( )
{
	if ( mSocket == INVALID_SOCKET )
		return 0;
	
	if ( mhObject == NULL )
		return 0;

	if ( mConnected == false )
		return 0;

	// 发送FIN标志给服务器,通知客户端要离开了
	::shutdown( mSocket, SD_SEND );

	OnClose( 0 );
	Close( );

	// 等待最后的数据,直到服务器回应ACK,收到FD_CLOSE消息
	//while( 1 )
	//{
	//	WSANETWORKEVENTS tEvents;
	//	int tRetVal = ::WSAEnumNetworkEvents( mSocket, mhObject, &tEvents );
	//	if ( tRetVal != 0 )
	//		return false;

	//	if ( tEvents.lNetworkEvents & FD_CLOSE )
	//	{
	//		OnClose( tEvents.iErrorCode[ FD_CLOSE_BIT ] );
	//		Close( );
	//		return 0;
	//	}
	//}

	return 0;
}

int CClientSocket::SelectEvent( int vEvent )
{
	if ( mhObject == 0 )
		mhObject = ::CreateEvent( 0, false, false, 0 );

	if ( ::WSAEventSelect( mSocket, mhObject, vEvent ) == SOCKET_ERROR )
		return ::GetLastError( );

	mTickCount = ::timeGetTime( );

	return 0;
}

DWORD CClientSocket::GetServerIP( )
{
	SOCKADDR_IN tSockAddr = { 0 }; int tSize = sizeof( SOCKADDR_IN );
	if ( ::getpeername( mSocket, (SOCKADDR*) &tSockAddr, &tSize ) == SOCKET_ERROR )
		return 0;

	return (DWORD) tSockAddr.sin_addr.s_addr;
}

char* CClientSocket::GetServerIPString( )
{
	return IPDword2String( GetServerIP( ) );
}

WORD CClientSocket::GetServerPort( )
{
	SOCKADDR_IN tSockAddr = { 0 }; int tSize = sizeof( SOCKADDR_IN );
	if ( ::getpeername( mSocket, (SOCKADDR*) &tSockAddr, &tSize ) == SOCKET_ERROR )
		return 0;

	return (WORD) ::ntohs( tSockAddr.sin_port );
}

DWORD CClientSocket::GetConnectTime( )
{
	int tTime = 0, tSize = 4;
	GetOption( SOL_SOCKET, SO_CONNECT_TIME, &tTime, &tSize );

	return (DWORD) tTime;
}

int CClientSocket::OnConnect( int vCode )
{
	return 0;
}

int CClientSocket::OnSend( int vCode )
{
	return 0;
}

int CClientSocket::OnRecv( int vCode )
{
	//CSocketMessage* pMessageHeader = (CSocketMessage*) mRecvBuffer;

	//DWORD tSize = 0, tBytes = GetBufferLength( );

	//while ( tBytes > 0 )
	//{
	//	int tLength = min( SOCKET_BUFFERSIZE_RECV - mRecvLength, tBytes );
	//	Recv( mRecvBuffer + mRecvLength, tLength, &tLength );
	//	mRecvLength += tLength;

	//	while ( mRecvLength >= sizeof( CSocketMessage ) && mRecvLength >= pMessageHeader->mMessageSize )
	//	{
	//		tSize = pMessageHeader->mMessageSize;
	//		mRecvLength -= tSize;

	//		if ( pMessageHeader->CheckHeader( ) == false )
	//			continue;

	//		if ( pMessageHeader->mMessageID == 0xFFFF )
	//		{
	//			CSocketPingMessage* tpPingMessage = (CSocketPingMessage*) pMessageHeader;

	//			mPing = ( ::timeGetTime( ) - tpPingMessage->mTickCount ) >> 1;
	//		}
	//		else
	//		{
	//			OnMessage( pMessageHeader );
	//		}

	//		::memmove( mRecvBuffer, mRecvBuffer + tSize, mRecvLength );
	//	}

	//	tBytes -= tLength;
	//}


	//----------------------------
	BYTE* pMessageHeader = (BYTE*) mRecvBuffer;

	DWORD tSize = 0, tBytes = GetBufferLength( );

	while ( tBytes > 0 )
	{
		int tLength = min( SOCKET_BUFFERSIZE_RECV - mRecvLength, tBytes );
		Recv( mRecvBuffer + mRecvLength, tLength, &tLength );
		mRecvLength += tLength;

		while ( mRecvLength >= sizeof(int) && mRecvLength >= *(short*)pMessageHeader)
		{
			tSize = *(short*)pMessageHeader ;
			if( tSize == 0 )
			{
				break;
			}
			
			mRecvLength -= tSize;

			//if ( pMessageHeader->CheckHeader( ) == false )
			//	continue;

			//if ( pMessageHeader->mMessageID == 0xFFFF )
			//{
			//	CSocketPingMessage* tpPingMessage = (CSocketPingMessage*) pMessageHeader;

			//	mPing = ( ::timeGetTime( ) - tpPingMessage->mTickCount ) >> 1;
			//}
			//else
			//{
				OnMessage( pMessageHeader, tSize );
			//}

			::memmove( mRecvBuffer, mRecvBuffer + tSize, mRecvLength );
		}

		tBytes -= tLength;
	}


	//----------------------------
	return 0;
}

int CClientSocket::OnMessage( BYTE* pMessage, int vMsgLen )
{
	return 0;
}

int CClientSocket::OnClose( int vCode )
{
	return 0;
}

void CClientSocket::Close( )
{
	if ( mhObject != NULL )
		::CloseHandle( mhObject );

	if ( mSocket != NULL )
		::closesocket( mSocket );

	mConnected		= false;
	mBytesSend		= NULL;
	mBytesRecv		= NULL;
	mhObject		= NULL;
	mTickCount		= NULL;
	mLastTickCount	= NULL;
	mSocket			= INVALID_SOCKET;
	mRecvLength		= 0;
	mPing			= 0;
}

// ********************************************************************** //
// CServerSocket
// ********************************************************************** //

CServerSocket::CServerSocket( )
{
	mKeepAlive		= 0;
	mCurConnection	= 0;
	mMaxConnection	= 0;
	mNeedTimer		= true;
	mpListenSockets	= NULL;
}

CServerSocket::~CServerSocket( )
{
	Close( );
}

DWORD CServerSocket::IOCPThread( void* pParam )
{
	CServerSocket* tpServerSocket = (CServerSocket*) pParam;

	if ( tpServerSocket == 0 )
		return -1;

	while ( true )
	{
		if ( tpServerSocket->mThreadID != ::GetCurrentThreadId( ) )
			break;

		CClientContext* tpContext = 0; OVERLAPPED* tpOverlapped = 0;
		int tRetVal = ::WaitForSingleObject( tpServerSocket->mhEvent, 0 );
		if ( tRetVal == WAIT_OBJECT_0 )
			break;

		tpServerSocket->LockData( );

		DWORD tBytesTran = 0, tTickCount = ::timeGetTime( ), tWaitTime = tTickCount - tpServerSocket->mTickCount;

		if ( tWaitTime >= SOCKET_TIMERINTERVAL )
		{
			if ( tpServerSocket->mNeedTimer )
				tpServerSocket->OnTimer( tTickCount );

			tpServerSocket->FlushBuffer( 0 );

			// 检查连接是否正常
			tpServerSocket->CheckAlive( NULL, tTickCount );

			tpServerSocket->mTickCount = tTickCount;

			tWaitTime = SOCKET_TIMERINTERVAL;
		}
		else
		{
			tWaitTime = SOCKET_TIMERINTERVAL - tWaitTime;
		}

		tpServerSocket->UnlockData( );
	
		int tCommHandle = -1;
		if ( ::GetQueuedCompletionStatus( tpServerSocket->mhObject, &tBytesTran, (DWORD*) &tCommHandle, (WSAOVERLAPPED**) &tpOverlapped, tWaitTime ) == 0 )
		{
			int tRetError = ::GetLastError( );
			if ( tRetError == WAIT_TIMEOUT )
				continue;
		}

		if ( tCommHandle != -1 )
			tpContext = tpServerSocket->GetClientContext( tCommHandle );

		if ( tpOverlapped == 0 )
			continue;

		if ( tpContext != 0 && tpServerSocket->CheckContext( tCommHandle ) == false )
			continue;

		tpServerSocket->LockData( );

		if ( tpOverlapped == &tpServerSocket->mpListenSockets->mAcceptOverlapped )
		{
			CListenSocketContext* tpListenContext = tpServerSocket->mpListenSockets;
			tpServerSocket->mpListenSockets = NULL;
			if ( tpServerSocket->mCurConnection >= tpServerSocket->mMaxConnection )
			{
				::shutdown( tpListenContext->mSocket, SD_SEND );
				::closesocket( tpListenContext->mSocket );

				delete tpListenContext;
				tpServerSocket->Accept( );
				break;
			}

			tpContext = new CClientContext;

			tpContext->mSocket						= tpListenContext->mSocket;
			tpContext->mKeepAlive					= tpServerSocket->mKeepAlive;
			tpContext->mTickCount					= ::timeGetTime( );

			SOCKADDR_IN* tpLocalAddr	= 0;
			SOCKADDR_IN* tpRemoteAddr	= 0;

			int tSize1 = sizeof( SOCKADDR_IN );
			int tSize2 = sizeof( SOCKADDR_IN );

			::GetAcceptExSockaddrs( tpListenContext->mAcceptBuffer, 0, sizeof( SOCKADDR_IN ) + 16,
				sizeof( SOCKADDR_IN ) + 16, (SOCKADDR**) &tpLocalAddr, &tSize1, (SOCKADDR**) &tpRemoteAddr, &tSize2 );

			tpContext->mLocalAddr	= *tpLocalAddr;
			tpContext->mRemoteAddr	= *tpRemoteAddr;

			tpServerSocket->mClientSockets.insert( CLIENTTYPE( tpContext->GetCommHandle( ), tpContext ) );

			delete tpListenContext;
			tpServerSocket->mCurConnection ++;

			if ( tpServerSocket->OnAccept( tpContext->GetCommHandle( ) ) == 0 )
			{
				::CreateIoCompletionPort( (HANDLE) tpContext->mSocket, tpServerSocket->mhObject, (DWORD) tpContext->GetCommHandle( ), 0 );

				tpServerSocket->Recv( tpContext->GetCommHandle( ), tpContext->mRecvBuffer, SOCKET_BUFFERSIZE_RECV, &tpContext->mRecvOverlapped );
			}

			tpServerSocket->Accept( );
		}

		if ( tpOverlapped == &tpContext->mSendOverlapped )
		{
			if ( tpContext != NULL )
			{
                tpContext->mBytesSend += tBytesTran;

				tpServerSocket->mBytesSend += tBytesTran;
				tpServerSocket->OnSend( tpContext->GetCommHandle( ), tBytesTran );
			}
		}

		if ( tpOverlapped == &tpContext->mRecvOverlapped )
		{
			if ( tpContext != NULL )
			{
				// 收到客户端发送的FIN
				if ( tBytesTran == 0 )
					tpServerSocket->CloseSocket( tCommHandle );
				else
				{
					tpContext->mTickCount  = ::timeGetTime( );
					tpContext->mBytesRecv += tBytesTran;

					tpServerSocket->mBytesRecv += tBytesTran;
					tpServerSocket->OnRecv( tpContext->GetCommHandle( ), tBytesTran );
				}
			}
		}

		tpServerSocket->UnlockData( );
	}
	
	return 0;
}

int CServerSocket::Create( const char* pAddress, bool vIsMessage, int vMaxConnection, DWORD vKeepAlive )
{
	SOCKADDR_IN tSockAddr;
	CreateSockAddr( &tSockAddr, pAddress );

	return Create( &tSockAddr, vIsMessage, vMaxConnection, vKeepAlive );
}

int CServerSocket::Create( const char* pAddress, WORD vPort, bool vIsMessage, int vMaxConnection, DWORD vKeepAlive )
{
	SOCKADDR_IN tSockAddr;
	CreateSockAddr( &tSockAddr, pAddress, vPort );

	return Create( &tSockAddr, vIsMessage, vMaxConnection, vKeepAlive );
}

int CServerSocket::Create( DWORD vIP, WORD vPort, bool vIsMessage, int vMaxConnection, DWORD vKeepAlive )
{
	SOCKADDR_IN tSockAddr;
	CreateSockAddr( &tSockAddr, vIP, vPort );

	return Create( &tSockAddr, vIsMessage, vMaxConnection, vKeepAlive );
}

int CServerSocket::Create( SOCKADDR_IN* pSockAddr, bool vIsMessage, int vMaxConnection, DWORD vKeepAlive )
{
	if ( pSockAddr == 0 )
		return -1;

	CAsyncSocket::Create( vIsMessage );

	if ( ( mSocket = ::WSASocket( AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED ) ) == INVALID_SOCKET )
		return ::GetLastError( );

	int tValue = 1;
	if ( ::setsockopt( mSocket, SOL_SOCKET, SO_REUSEADDR, (char*) &tValue, sizeof( int ) ) == SOCKET_ERROR )
		return ::GetLastError( );

	if ( ::bind( mSocket, (SOCKADDR*) pSockAddr, sizeof( SOCKADDR_IN ) ) == SOCKET_ERROR )
		return ::GetLastError( );

	mKeepAlive		= vKeepAlive;
	mCurConnection	= 0;
	mMaxConnection	= vMaxConnection;

	mhObject = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, 0 );
	mhThread = ::CreateThread( 0, 0, CServerSocket::IOCPThread, this, 0, &mThreadID );
	mhEvent	 = ::CreateEvent( 0, false, false, 0 );
	return 0;
}

int CServerSocket::Listen( int vQueue )
{
	if ( ::listen( mSocket, min( vQueue, mMaxConnection ) ) == SOCKET_ERROR )
		return ::GetLastError( );

	if ( ::CreateIoCompletionPort( (HANDLE) mSocket, mhObject, 0, 0 ) == 0 )
		return ::GetLastError( );

	for ( int i = 0; i < SOCKET_ACCEPTNUMBER; i ++ )
	{
		int tRetVal = Accept( );

		if ( tRetVal != 0 ) return tRetVal;
	}

	return 0;
}

int CServerSocket::Accept( )
{
	SOCKET tAccept = ::WSASocket( AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED );
	if ( tAccept == INVALID_SOCKET )
		return ::GetLastError( );

	CListenSocketContext* tpContext = new CListenSocketContext;

	tpContext->mSocket	= tAccept;
	mpListenSockets		= tpContext;
	DWORD tBytes;
	if ( ::AcceptEx( mSocket, tAccept, tpContext->mAcceptBuffer, 0, sizeof( SOCKADDR_IN ) + 16,
		 sizeof( SOCKADDR_IN ) + 16, &tBytes, (OVERLAPPED*) &tpContext->mAcceptOverlapped ) == 0 )
	{
		if ( ::GetLastError( ) != ERROR_IO_PENDING )
			return ::GetLastError( );
	}

	return 0;
}

int CServerSocket::Send( int vCommHandle, void* pBuffer, int vLength, OVERLAPPED* pOverlapped, DWORD vFlags )
{
	CClientContext* tpContext = GetClientContext( vCommHandle );
	if ( tpContext == NULL )
		return 0;

	WSABUF tWSABuffer;
	tWSABuffer.buf = (char*) pBuffer;
	tWSABuffer.len = vLength;

	OnEncrypt( pBuffer, vLength );

	DWORD tBytesSend = 0;
	while ( ::WSASend( tpContext->mSocket, &tWSABuffer, 1, &tBytesSend, vFlags, (WSAOVERLAPPED*) pOverlapped, 0 ) == SOCKET_ERROR )
	{
		if ( ::GetLastError( ) == ERROR_IO_PENDING )
			return ERROR_IO_PENDING;

		if ( ::GetLastError( ) != WSAEWOULDBLOCK )
		{
			CloseSocket( vCommHandle );
			return ::GetLastError( );
		}
	}

	tpContext->mBytesSend += vLength; mBytesSend += vLength;

	return 0;
}

int CServerSocket::Recv( int vCommHandle, void* pBuffer, int vLength, OVERLAPPED* pOverlapped, int* pBytes, DWORD vFlags )
{
	CClientContext* tpContext = GetClientContext( vCommHandle );
	if ( tpContext == NULL )
		return 0;

	WSABUF tWSABuffer;
	tWSABuffer.buf = (char*) pBuffer;
	tWSABuffer.len = vLength;

	DWORD tBytesRecv = 0;
	if ( ::WSARecv( tpContext->mSocket, &tWSABuffer, 1, &tBytesRecv, &vFlags, (WSAOVERLAPPED*) pOverlapped, 0 ) == SOCKET_ERROR )
		return ::GetLastError( );

	if ( pBytes != 0 )
		*pBytes = tBytesRecv;

	tpContext->mBytesRecv += tBytesRecv; mBytesRecv += tBytesRecv;

	OnDecrypt( pBuffer, tBytesRecv );

	return 0;
}

int CServerSocket::Close( )
{
	CAsyncSocket::Close( );

	CLIENTITER tBeginIter	= mClientSockets.begin( );
	CLIENTITER tEndIter		= mClientSockets.end( );
	while( tBeginIter != tEndIter )
	{
		CLIENTITER tNextIter = tBeginIter; 
		tNextIter ++;
		CloseSocket( tBeginIter->second->GetCommHandle( ) );
		tBeginIter = tNextIter;
	}

	mClientSockets.clear( );
	if ( mpListenSockets != NULL )
	{
		::closesocket( mpListenSockets->mSocket );
		delete mpListenSockets;
		mpListenSockets = NULL;
	}

	mKeepAlive		= 0;
	mCurConnection	= 0;
	mMaxConnection	= 0;

	return 0;
}

int CServerSocket::SetKeepAlive( int vCommHandle, DWORD vKeepAlive )
{
	CClientContext* tpContext = GetClientContext( vCommHandle );
	if ( tpContext == NULL )
		return 0;

	mKeepAlive = vKeepAlive;
	tpContext->mKeepAlive = vKeepAlive;
	tpContext->mTickCount = ::timeGetTime( );

	return 0;
}

int CServerSocket::CheckAlive( int vCommHandle, DWORD vTickCount )
{
	if ( vCommHandle != 0 )
	{
		CClientContext* tpContext = GetClientContext( vCommHandle );
		if ( tpContext == NULL )
			return 0;

		if ( tpContext->mKeepAlive < vTickCount - tpContext->mTickCount )
			CloseSocket( vCommHandle );
	}
	else
	{
		CLIENTITER tBeginIter	= mClientSockets.begin( );
		CLIENTITER tEndIter		= mClientSockets.end( );
		while( tBeginIter != tEndIter )
		{
			CLIENTITER tNextIter = tBeginIter; 
			tNextIter ++;
			CheckAlive( tBeginIter->second->GetCommHandle( ), vTickCount );
			tBeginIter = tNextIter;
		}
	}

	return 0;
}

void CServerSocket::DumpClient( )
{
}

int CServerSocket::FillBuffer( int vCommHandle, void* pBuffer, int vLength )
{
	if ( vCommHandle != 0 )
	{
		CClientContext* tpContext = GetClientContext( vCommHandle );
		if ( tpContext == NULL )
			return 0;

		if ( tpContext->mSendLength + vLength > SOCKET_BUFFERSIZE_SEND )
			FlushBuffer( vCommHandle );

		::memcpy( tpContext->mSendBuffer + tpContext->mSendLength, pBuffer, vLength );
		tpContext->mSendLength += vLength;
	}
	else if ( vCommHandle == 0 )
	{
		CLIENTITER tBeginIter	= mClientSockets.begin( );
		CLIENTITER tEndIter		= mClientSockets.end( );
		while( tBeginIter != tEndIter )
		{
			CLIENTITER tNextIter = tBeginIter; 
			tNextIter ++;
			FillBuffer( tBeginIter->second->GetCommHandle( ), pBuffer, vLength );
			tBeginIter = tNextIter;
		}			
	}

	return 0;
}

int CServerSocket::FlushBuffer( int vCommHandle, DWORD vFlags )
{
	if ( vCommHandle != 0 )
	{
		CClientContext* tpContext = GetClientContext( vCommHandle );
		if ( tpContext == NULL )
			return 0;

		if ( tpContext->mSendLength != 0 )
			Send( vCommHandle, tpContext->mSendBuffer, tpContext->mSendLength, &tpContext->mSendOverlapped, vFlags );

		tpContext->mSendLength = 0;
	}
	else if ( vCommHandle == 0 )
	{
		CLIENTITER tBeginIter	= mClientSockets.begin( );
		CLIENTITER tEndIter		= mClientSockets.end( );
		while( tBeginIter != tEndIter )
		{
			CLIENTITER tNextIter = tBeginIter; 
			tNextIter ++;
			FlushBuffer( tBeginIter->second->GetCommHandle( ), vFlags );
			tBeginIter = tNextIter;
		}			
	}

	return 0;
}

int CServerSocket::CloseSocket( int vCommHandle )
{
	CLIENTITER		tFindIter = mClientSockets.find( vCommHandle );
	if ( tFindIter == mClientSockets.end( ) )
		return 0;

	CClientContext* tpContext = tFindIter->second;
	if ( tpContext == NULL )
		return 0;

	if ( tpContext->mSocket != INVALID_SOCKET )
	{
		mCurConnection --;

		OnClose( vCommHandle );

		::shutdown( tpContext->mSocket, SD_SEND );
		::closesocket( tpContext->mSocket );
		if ( tFindIter != mClientSockets.end( ) )
		{
			delete tFindIter->second;
			mClientSockets.erase( tFindIter );
		}
	}
	return 0;
}

bool CServerSocket::CheckContext( int vCommHandle )
{
	return GetClientContext( vCommHandle ) != NULL;
}

int CServerSocket::OnAccept( int vCommHandle )
{
	return 0;
}

int CServerSocket::OnSend( int vCommHandle, int vBytes )
{
	return 0;
}

CClientContext* CServerSocket::GetClientContext( int vCommHandle )
{
	CLIENTITER tFindIter = mClientSockets.find( vCommHandle );
	if ( tFindIter == mClientSockets.end( ) )
		return NULL;

	return tFindIter->second;
}

int CServerSocket::OnRecv( int vCommHandle, int vBytes )
{
	CClientContext* tpContext = GetClientContext( vCommHandle );
	if ( tpContext == NULL )
		return 0;

	//if ( mIsMessage )
	//{
	//	CSocketMessage* pMessageHeader = (CSocketMessage*) tpContext->mRecvBuffer;

	//	DWORD tSize = 0; tpContext->mRecvLength += vBytes;

	//	while ( tpContext->mRecvLength >= sizeof( CSocketMessage ) && tpContext->mRecvLength >= pMessageHeader->mMessageSize )
	//	{
	//		tSize = pMessageHeader->mMessageSize;
	//		tpContext->mRecvLength -= tSize;

	//		if ( pMessageHeader->CheckHeader( ) == false )
	//			continue;

	//		if ( pMessageHeader->mMessageID == 0xFFFF )
	//		{
	//			tpContext->mTickCount = ::timeGetTime( );
	//			FillBuffer( vCommHandle, pMessageHeader, pMessageHeader->mMessageSize );
	//		}
	//		else if ( OnMessage( vCommHandle, pMessageHeader ) != 0 )
	//		{
	//			return 1;
	//		}

	//		::memmove( tpContext->mRecvBuffer, tpContext->mRecvBuffer + tSize, tpContext->mRecvLength );
	//	}
	//}


	if ( mIsMessage )
	{
		BYTE* pMessageHeader = (BYTE*) tpContext->mRecvBuffer;

		DWORD tSize = 0; tpContext->mRecvLength += vBytes;

		while ( tpContext->mRecvLength >= sizeof( int ) && tpContext->mRecvLength >= *(short*)pMessageHeader )
		{
			tSize = *(short*)pMessageHeader;
			tpContext->mRecvLength -= tSize;

			//if ( pMessageHeader->CheckHeader( ) == false )
			//	continue;

			//if ( pMessageHeader->mMessageID == 0xFFFF )
			//{
			//	tpContext->mTickCount = ::timeGetTime( );
			//	FillBuffer( vCommHandle, pMessageHeader, pMessageHeader->mMessageSize );
			//}
			//else if ( OnMessage( vCommHandle, pMessageHeader ) != 0 )
			//{
			//	return 1;
			//}
			
			OnMessage ( vCommHandle,  pMessageHeader, tSize );

			::memmove( tpContext->mRecvBuffer, tpContext->mRecvBuffer + tSize, tpContext->mRecvLength );
		}
	}

	Recv( vCommHandle, tpContext->mRecvBuffer + tpContext->mRecvLength,
		SOCKET_BUFFERSIZE_RECV - tpContext->mRecvLength, &tpContext->mRecvOverlapped );

	return 0;
}

int CServerSocket::OnMessage( int vCommHandle, BYTE* pMessage, int vMsgLen )
{
	return 0;
}

int CServerSocket::OnTimer( DWORD vTickCount )
{
	return 0;
}

int CServerSocket::OnClose( int vCommHandle )
{
	return 0;
}