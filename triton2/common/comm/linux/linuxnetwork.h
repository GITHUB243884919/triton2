#pragma once
#   include <network.h>
#   include "epollsocket.h"
#ifdef LINUX
#ifdef WIN32
#else
#   include <pthread.h>
#endif

class CLinuxLock : public CLockObject
{
public:
#ifdef WIN32
    CRITICAL_SECTION    mLock;
#else
    pthread_mutex_t     mLock;
#endif
public:
    CLinuxLock( );

public:
    virtual ~CLinuxLock( );

public:
    virtual void Lock( );
    virtual void Unlock( );
};


// ********************************************************************** //
// CLinuxClientNetwork
// ********************************************************************** //

class CLinuxClientNetwork : public CClientNetwork, public CClientLinuxSocket
{
public:
    char	mBuffer[ 65535 ];
	int		mUserData;
public:
    CLinuxClientNetwork( int vUserData, OnConnectProc pConnect, OnMessageProc pMessage, OnCloseProc pClose )
	: CClientNetwork( pConnect, pMessage, pClose ), mUserData(vUserData)
    {
    }

public:
    ~CLinuxClientNetwork( )
	{
	}

public:
    virtual bool Create( const char* pAddress );
    virtual void Shutdown( );
    virtual void SendMsg( BYTE* pMessage, int vMsgLen );
    virtual void EnumEvent( );

public:
    virtual int OnConnect( int vCode );
    virtual int OnMessage( BYTE* pMessage, int vMsgLen );
    virtual int OnClose( int vCode );
    virtual void GetStatistics( CStatistics* pStatistics );
	virtual void SetUserData( int vUserData )
	{
		mUserData = vUserData;
	}
};

// ********************************************************************** //
// CLinuxServerNetwork
// ********************************************************************** //

class CLinuxServerNetwork : public CServerNetwork, public CEPollSocket
{
public:
    char	mBuffer[ 65535 ];

public:
    CLinuxServerNetwork(
        OnAcceptProc  pAccept,
        OnMessageProc pMessage,
        OnCloseProc   pClose,
        OnTimerProc   pTimer,
        CLockObject*  pLock
    ) 
    : CServerNetwork( pAccept, pMessage, pClose, pTimer, pLock )
    {
    }
    ~CLinuxServerNetwork( )
    {

    }
public:
    virtual bool Create( const char* pAddress );
    virtual void Shutdown( );
    virtual void SendMsg( int vCommHandle, BYTE* pMessage, int vMsgLen );
    virtual char* GetRemoteIPString( int vCommHandle );
    virtual unsigned short GetRemotePort( int vCommHandle );
	virtual void SetKeepAlive( int vCommHandle, unsigned int vTime );

    virtual int ShowInfo( )
    {
        return CEPollSocket::ShowInfo( );
    };

public:
    virtual int OnAccept( int vCommHandle );
    virtual int OnMessage( int vCommHandle, BYTE* pMessage, int vMsgLen );
    virtual int OnTimer( uint32_t vTickCount );
    virtual int OnClose( int vCommHandle, bool vIsLost );
    virtual void GetStatistics( int vCommHandle, CStatistics* pStatistics );
};


#endif
