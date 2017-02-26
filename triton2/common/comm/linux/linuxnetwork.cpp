#include "linuxnetwork.h"
#ifdef LINUX

CLinuxLock::CLinuxLock( )
{
    int tRetCode = 0;
    tRetCode = pthread_mutex_init( &mLock, NULL );
    if ( tRetCode != 0 )
    {
        perror( "Error when constructing CLinuxLock" );
    }
   
}

CLinuxLock::~CLinuxLock( )
{
    int tRetCode = 0;
    tRetCode = pthread_mutex_destroy( &mLock );
    if ( tRetCode != 0 )
    {
        perror( "Error when destructing CLinuxLock" );
    }
}

// ¼ÓËø
void CLinuxLock::Lock( )
{
    int tRetCode = 0;   
    tRetCode = pthread_mutex_lock( &mLock );
    if ( tRetCode != 0 )
    {
        perror( "Error when destructing CLinuxLock" );
    }
}

// ½âËø
void CLinuxLock::Unlock( )
{
    int tRetCode = 0;   
#ifdef WIN32
    ::LeaveCriticalSection( &mLock );
#else // LINUX
    tRetCode = pthread_mutex_unlock( &mLock );
    if ( tRetCode != 0 )
    {
        perror( "Error when destructing CLinuxLock" );
    }
#endif
}


// ********************************************************************** //
// CLinuxClientNetwork
// ********************************************************************** //
bool CLinuxClientNetwork::Create( const char* pAddress )
{
    if ( CClientLinuxSocket::Create( ) != 0 )
        return false;

    if ( CClientLinuxSocket::Connect( pAddress ) != 0 )
        return false;

    return true;
}

void CLinuxClientNetwork::Shutdown( )
{
    CClientLinuxSocket::Shutdown( );
}

void CLinuxClientNetwork::SendMsg( BYTE* pMessage, int vMsgLen )
{
    Send( (char*) pMessage, vMsgLen );
}


void CLinuxClientNetwork::EnumEvent( )
{
    EventProcess( );
}

int CLinuxClientNetwork::OnConnect( int vCode )
{
	FireConnect( vCode, mUserData );
	return 0;
}

int CLinuxClientNetwork::OnMessage( BYTE* pMessage, int vMsgLen )
{
	FireMessage( pMessage, vMsgLen , mUserData );
    return 0;
}

int CLinuxClientNetwork::OnClose( int vCode )
{
	FireClose( vCode, mUserData );
    return 0;
}

void CLinuxClientNetwork::GetStatistics( CStatistics* pStatistics )
{

}

// ********************************************************************** //
// CLinuxServerNetwork
// ********************************************************************** //

bool CLinuxServerNetwork::Create( const char* pAddress )
{
    if ( CEPollSocket::Create( pAddress, true ) != 0 )
        return false;

    if ( CEPollSocket::Listen( 5 ) != 0 )
    {
        return false;
    }

    CEPollSocket::SetLock( &( ( ( CLinuxLock * ) mpLock )->mLock)  );
    return true;
}

void CLinuxServerNetwork::Shutdown( )
{
    CEPollSocket::Close( );
}

char* CLinuxServerNetwork::GetRemoteIPString( int vCommHandle )
{
    CClientLinuxContext* tpContext = GetClientContext( vCommHandle );
    if ( tpContext == NULL )
        return NULL;

    return tpContext->GetRemoteIPString( );
}

void CLinuxServerNetwork::SetKeepAlive( int vCommHandle, unsigned int vTime )
{
}

unsigned short CLinuxServerNetwork::GetRemotePort( int vCommHandle )
{
    CClientLinuxContext* tpContext = GetClientContext( vCommHandle );
    if ( tpContext == NULL )
        return 0;

    return tpContext->GetRemotePort( );
}

void CLinuxServerNetwork::SendMsg( int vCommHandle, BYTE* pMessage, int vMsgLen )
{
    //CSocketMessage tSocketMessage( pMessage->mMessageSize + sizeof( CSocketMessage ), pMessage->mMessageID );
    //FillBuffer( vCommHandle, &tSocketMessage, sizeof( CSocketMessage ) );
    FillBuffer( vCommHandle, ( (char*) pMessage ), vMsgLen );
}

int CLinuxServerNetwork::OnAccept( int vCommHandle )
{
    FireAccept( vCommHandle );
    return 0;
}

int CLinuxServerNetwork::OnMessage( int vCommHandle, BYTE* pMessage, int vMsgLen )
{
    //CMessage* tpMessage = (CMessage*) mBuffer;
    //tpMessage->mMessageID	= pMessage->mMessageID;
    //tpMessage->mMessageSize	= pMessage->mMessageSize - sizeof( CSocketMessage );
    //::memcpy( ( (char*) tpMessage ) + sizeof( CMessage ), ( (char*) pMessage ) + sizeof( CSocketMessage ), tpMessage->mMessageSize );
    FireMessage( vCommHandle, pMessage, vMsgLen );
    return 0;
}

int CLinuxServerNetwork::OnTimer( uint32_t vTickCount )
{
    FireTimer( vTickCount );
    return 0;
}

int CLinuxServerNetwork::OnClose( int vCommHandle, bool vIsLost )
{
    FireClose( vCommHandle );
    return 0;
}

void CLinuxServerNetwork::GetStatistics( int vCommHandle, CStatistics* pStatistics )
{
    
}
#endif
