// ********************************************************************** //
// Win32Network.cpp
// Network抽象接口的Windows版实现
// Author: Black
// ********************************************************************** //

#include "win32network.h"
// ********************************************************************** //
// CWin32Lock
// ********************************************************************** //

void CWin32Lock::Lock( )
{
	::EnterCriticalSection( &mLock );
}

void CWin32Lock::Unlock( )
{
	::LeaveCriticalSection( &mLock );
}

// ********************************************************************** //
// CWin32ServerNetwork
// ********************************************************************** //
bool CWin32ClientNetwork::Create( const char* pAddress )
{
	if ( CClientSocket::Create( ) != 0 )
		return false;

	if ( CClientSocket::Connect( pAddress ) != 0 )
		return false;

    return true;
}

void CWin32ClientNetwork::Shutdown( )
{
	CClientSocket::Shutdown( );
}

void CWin32ClientNetwork::SendMsg( BYTE* pMessage, int vMsgLen )
{
	Send( pMessage, vMsgLen );
}

void CWin32ClientNetwork::EnumEvent( )
{
	EventProcess( );
}

int CWin32ClientNetwork::OnConnect( int vCode )
{
	FireConnect( vCode, mUserData );
	return 0;
}

int CWin32ClientNetwork::OnMessage( BYTE* pMessage, int vMsgLen )
{
	FireMessage( (BYTE*) pMessage, vMsgLen, mUserData );
	return 0;
}

int CWin32ClientNetwork::OnClose( int vCode )
{
	FireClose( vCode, mUserData );
	return 0;
}

// ********************************************************************** //
// CWin32ServerNetwork
// ********************************************************************** //
bool CWin32ServerNetwork::Create( const char* pAddress )
{
	if ( CServerSocket::Create( pAddress, true ) != 0 )
		return false;

	if ( CServerSocket::Listen( 5 ) != 0 )
		return false;

	CServerSocket::SetLock( &( (CWin32Lock*) mpLock )->mLock );
	return true;
}

void CWin32ServerNetwork::Shutdown( )
{
	CServerSocket::Close( );
}

char* CWin32ServerNetwork::GetRemoteIPString( int vCommHandle )
{
	CClientContext* tpContext = GetClientContext( vCommHandle );
	if ( tpContext == NULL )
		return NULL;

	return tpContext->GetRemoteIPString( );
}

void CWin32ServerNetwork::SetKeepAlive( int vCommHandle, unsigned int vKeepAlive )
{
	CClientContext* tpContext = GetClientContext( vCommHandle );
	if ( tpContext == NULL )
		return;

	CServerSocket::SetKeepAlive( vCommHandle, vKeepAlive );
}

unsigned short CWin32ServerNetwork::GetRemotePort( int vCommHandle )
{
	CClientContext* tpContext = GetClientContext( vCommHandle );
	if ( tpContext == NULL )
		return 0;

	return tpContext->GetRemotePort( );
}

void CWin32ServerNetwork::SendMsg( int vCommHandle, BYTE* pMessage, int vMsgLen )
{
	FillBuffer( vCommHandle, pMessage, vMsgLen );
}

int CWin32ServerNetwork::OnAccept( int vCommHandle )
{
	FireAccept( vCommHandle );
	return 0;
}

int CWin32ServerNetwork::OnMessage( int vCommHandle, BYTE* pMessage, int vMsgLen )
{
	FireMessage( vCommHandle,  pMessage, vMsgLen );
	return 0;
}

int CWin32ServerNetwork::OnTimer( DWORD vTickCount )
{
	FireTimer( vTickCount );
	return 0;
}

int CWin32ServerNetwork::OnClose( int vCommHandle )
{
	FireClose( vCommHandle );
	return 0;
}
