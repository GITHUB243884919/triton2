#pragma once
#include "Network.h"
#include "SystemSocket.h"
using namespace System;

// ********************************************************************** //
// CWin32Lock
// ********************************************************************** //

class CWin32Lock : public CLockObject
{
public:
	CRITICAL_SECTION	mLock;

public:
	CWin32Lock( )
	{
		::InitializeCriticalSection( &mLock );
	}

public:
	virtual ~CWin32Lock( )
	{
		::DeleteCriticalSection( &mLock );
	}

public:
	virtual void Lock( );
	virtual void Unlock( );
};

// ********************************************************************** //
// CWin32ClientNetwork
// ********************************************************************** //

class CWin32ClientNetwork : public CClientNetwork, public CClientSocket
{
public:
	int		mUserData;
public:
	CWin32ClientNetwork( int vUserData, OnConnectProc pConnect, OnMessageProc pMessage, OnCloseProc pClose ) : CClientNetwork( pConnect, pMessage, pClose ), mUserData( vUserData )
	{
	}
	CWin32ClientNetwork()
	{
	}
	
public:
	void Init( int vUserData, OnConnectProc pConnect, OnMessageProc pMessage, OnCloseProc pClose)
	{
		mUserData = vUserData;
		CClientNetwork::Init(pConnect, pMessage, pClose);
	}
	virtual bool Create( const char* pAddress );
	virtual void Shutdown( );
	virtual void SendMsg( BYTE* pMessage, int vMsgLen );
	virtual void EnumEvent( );
	virtual void GetStatistics( CStatistics* pStatistics )
	{
	}
	
	virtual void SetUserData( int vUserData )
	{
		mUserData = vUserData;
	}

public:
	virtual int OnConnect( int vCode );
	virtual int OnMessage( BYTE* pMessage, int vMsgLen );
	virtual int OnClose( int vCode );
};

// ********************************************************************** //
// CWin32ServerNetwork
// ********************************************************************** //

class CWin32ServerNetwork : public CServerNetwork, public CServerSocket
{
public:
	CWin32ServerNetwork( OnAcceptProc pAccept, OnMessageProc pMessage, OnCloseProc pClose, OnTimerProc pTimer, CLockObject* pLock ) : CServerNetwork( pAccept, pMessage, pClose, pTimer, pLock )
	{
		CServerSocket::SetLock( &( (CWin32Lock*) pLock )->mLock );
	}

public:
	virtual bool Create( const char* pAddress );
	virtual void Shutdown( );
	virtual void SendMsg( int vCommHandle, BYTE* pMessage, int vMsgLen );
	virtual char* GetRemoteIPString( int vCommHandle );
	virtual unsigned short GetRemotePort( int vCommHandle );
	virtual void SetKeepAlive( int vCommHandle, unsigned int vTime );
	
	virtual void GetStatistics( int vCommHandle, CStatistics* pStatistics )	
	{
	}

public:
	virtual int OnAccept( int vCommHandle );
	virtual int OnMessage( int vCommHandle, BYTE* pMessage, int vMsgLen );
	virtual int OnTimer( DWORD vTickCount );
	virtual int OnClose( int vCommHandle );
};
