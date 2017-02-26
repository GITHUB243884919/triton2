
// ***************************************************************
// Function:      Initialize
// Description:   初始化
// Others:         
// Date:          03/23/2011
// 
//***************************************************************
template< int MAX_CONN_NUM > 
int CProxyConnSet<MAX_CONN_NUM>::Initialize()
{
	mActiveConnNum = 0;
	for ( int i = 0; i < ARRAY_CNT( mTcpConn ); ++i )
	{
		mTcpConn[i].Initialize( -1, -1, -1, -1 );
		mTcpConn[i].SetConnState( ENTITY_OFF );
	}
}

// ***************************************************************
// Function:      GetFreeTcpConn
// Description:   获取一个空闲的socket
// Others:         
// Date:          03/23/2011
// 
//***************************************************************
template< int MAX_CONN_NUM > 
CMyTCPConn* CProxyConnSet<MAX_CONN_NUM>::GetFreeTcpConn()	
{
	for ( int i = 0; i < ARRAY_CNT(mTcpConn); ++i )
	{
		if ( mTcpConn[i].GetEntityID() == 0 )
		{
			return mTcpConn[i];
		}		
	}
	return NULL;	
}

// ***************************************************************
// Function:      UnRegistTcpConnByEntityID
// Description:   注销掉一个不用的socket
// Others:         
// Date:          03/23/2011
// 
//***************************************************************
template< int MAX_CONN_NUM > 
int CProxyConnSet<MAX_CONN_NUM>::UnRegistTcpConnByEntityID( int nEntityID )
{
	for ( int i = 0; i < ARRAY_CNT(mTcpConn); ++i )
	{
		if ( mTcpConn[i].GetEntityID() == nEntityID )
		{
			mTcpConn[i].GetSocket()->Close();
			break;
		}		
	}
	return 0;	
}

// ***************************************************************
// Function:      UnRegistTcpConnBySocketID
// Description:   注销掉一个不用的socket
// Others:         
// Date:          03/23/2011
// 
//***************************************************************
template< int MAX_CONN_NUM > 
int CProxyConnSet<MAX_CONN_NUM>::UnRegistTcpConnBySocketID( int nFd )
{
	for ( int i = 0; i < ARRAY_CNT(mTcpConn); ++i )
	{
		if ( mTcpConn[i].GetSocket()->GetSocketFD() == nFd )
		{
			mTcpConn[i].GetSocket()->Close();
			break;
		}		
	}
	return 0;
}

// ***************************************************************
// Function:      Accept
// Description:   建立一个新的socket连接
// Others:         
// Date:          03/23/2011
// 
//***************************************************************
template< int MAX_CONN_NUM > 
CMyTCPConn* CProxyConnSet<MAX_CONN_NUM>::Accept( int nEntityType, int nFd, u_long nIp, u_short nPort )
{
	CMyTCPConn *tpConn = GetFreeTcpConn();
	if ( tpConn == NULL )
	{
		LOG_ERROR( "default","can not find free socket for fd:%d",nFd );
		return NULL;
	}
	tpConn->Initialize( nEntityType, 0, nIp, nPort );
	tpConn->GetSocket()->Accept( nFd );
	return tpConn;
}

// ***************************************************************
// Function:      ServerRegiste
// Description:   注册一个服务器
// Others:         
// Date:          03/23/2011
// 
//***************************************************************
template< int MAX_CONN_NUM > 
CMyTCPConn* CProxyConnSet<MAX_CONN_NUM>::ServerRegiste( u_long nIp, u_short nPort, int nGameID )
{
	CMyTCPConn *tpConn = GetTcpConnByIPAddr( nIp, nPort );
	if ( tpConn == NULL )
	{
		LOG_ERROR( "default","error occured where registe server,ip:%d", nIp );
		return NULL;
	}

	tpConn->Initialize( tpConn->GetEntityType(), nGameID, tpConn->GetConnAddr(), tpConn->GetConnPort() );
	tpConn->SetConnState( ENTITY_ON );
	return tpConn;
}

// ***************************************************************
// Function:      GetTcpConnByIPAddr
// Description:   根据地址信息获取socket连接
// Others:         
// Date:          03/23/2011
// 
//***************************************************************
template< int MAX_CONN_NUM > 
CMyTCPConn* CProxyConnSet<MAX_CONN_NUM>::GetTcpConnByIPAddr( u_long nIp, u_short nPort )
{
	for ( int i = 0; i < ARRAY_CNT(mTcpConn); ++i )
	{
		if ( mTcpConn[i].GetConnPort() == nPort && mTcpConn[i].GetConnAddr() == nIp )
		{
			return &mTcpConn[i];
		}		
	}
	return NULL;
}

// ***************************************************************
// Function:      GetTcpConnByEntityID
// Description:   根据nEntityID获取socket连接
// Others:         
// Date:          03/23/2011
// 
//***************************************************************
template< int MAX_CONN_NUM > 
CMyTCPConn* CProxyConnSet<MAX_CONN_NUM>::GetTcpConnByEntityID( int nEntityID )
{
	for ( int i = 0; i < ARRAY_CNT(mTcpConn); ++i )
	{
		if ( mTcpConn[i].GetEntityID() == nEntityID )
		{
			return &mTcpConn[i];
		}		
	}
	return NULL;
}

// ***************************************************************
// Function:      operator []201
// Description:   
// Others:         
// Date:          03/23/2011
// 
//***************************************************************
template< int MAX_CONN_NUM > 
CMyTCPConn& CProxyConnSet<MAX_CONN_NUM>::operator []( int nIndex )
{
	if ( nIndex >=0 && nIndex < ARRAY_CNT( mTcpConn ) )
	{
		return mTcpConn[nIndex];
	}
	return mTcpConn[0];
}