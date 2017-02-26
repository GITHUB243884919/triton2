#ifdef _DEBUG_
#define  SENDBUFSIZE 3000000
#else
#define  SENDBUFSIZE 8000000
#endif

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
CTCPSocket<uiRecvBufLen, uiSendBufLen>::CTCPSocket()
{
	m_iSocketFD = -1;
	m_iSocketType = sot_comm;
	m_iStatus = tcs_closed;

#ifdef _POSIX_MT_
	pthread_mutex_init( &m_stMutex, NULL );
#endif

	m_iReadBegin = 0;
	m_iReadEnd = 0;
	m_iPostBegin = m_iPostEnd = 0;
	
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
CTCPSocket<uiRecvBufLen, uiSendBufLen>::~CTCPSocket()
{
	if( m_iStatus != tcs_closed && m_iSocketFD > 0 )
	{
		Close();
	}
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::CreateClient(char* szLocalAddr /* = NULL  */)
{
	socklen_t iOptVal = 0;
	int iOptLen = sizeof(int);

	if(m_iStatus != tcs_closed && m_iSocketFD > 0)
	{
		Close();
	}

	m_iSocketType = sot_comm;

	m_iSocketFD = socket(AF_INET, SOCK_STREAM, 0);

	if( m_iSocketFD < 0 )
	{
		m_iStatus = tcs_closed;
		m_iSocketFD = -1;

		return -1;
	}

	if( szLocalAddr )
	{
		sockaddr_in stTempSockAddr;

		memset((void *)&stTempSockAddr, 0, sizeof(sockaddr_in));
		stTempSockAddr.sin_family = AF_INET;
		stTempSockAddr.sin_addr.s_addr = inet_addr(szLocalAddr);
		bind(m_iSocketFD, (const struct sockaddr *)&stTempSockAddr, sizeof(stTempSockAddr));
	}

	iOptLen = sizeof(socklen_t);
	iOptVal = SENDBUFSIZE;
	if (setsockopt(m_iSocketFD, SOL_SOCKET, SO_SNDBUF, (const void *)&iOptVal, iOptLen))
	{
		LOG_ERROR( "default", "Set send buffer size to %d failed!\n", iOptVal);
		return -1;
	}
	if (getsockopt(m_iSocketFD, SOL_SOCKET, SO_SNDBUF, (void *)&iOptVal, (socklen_t *)&iOptLen) < 0)
	{
		// 系统默认会设置成传入参数的2倍
		if( (int)(SENDBUFSIZE * 2) != (int)(iOptVal) )
		{
			LOG_ERROR( "default", "Set Send buf of socket is %d.\n", iOptVal);
		}
	}

	m_iStatus = tcs_opened;
	m_iReadBegin = m_iReadEnd = 0;
	m_iPostBegin = m_iPostEnd = 0;
	return 0;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::CreateServer(unsigned short unPort, char* szIPAddr/* =NULL  */)
{
	sockaddr_in stTempSockAddr;
	int iReusePortFlag = 1;
	 socklen_t iOptVal = 0;
	int iOptLen = sizeof(int);
	
	if(m_iStatus != tcs_closed && m_iSocketFD > 0)
	{
		Close();
	}

	m_iSocketType = sot_listen;

	m_iSocketFD = socket(AF_INET, SOCK_STREAM, 0);

	if( m_iSocketFD < 0 )
	{
		m_iStatus = tcs_closed;
		m_iSocketFD = -1;

		return -1;
	}

	if( setsockopt(m_iSocketFD, SOL_SOCKET, SO_REUSEADDR, &iReusePortFlag, sizeof(int)) )
	{
		LOG_ERROR( "default", "Set socket addr reusable failed, %s.\n", strerror(errno));
	}

	memset((void *)&stTempSockAddr, 0, sizeof(sockaddr_in));
	stTempSockAddr.sin_family = AF_INET;
	stTempSockAddr.sin_port = htons(unPort);

	if( szIPAddr )
	{
		stTempSockAddr.sin_addr.s_addr = inet_addr(szIPAddr);
	}
	else
	{
		stTempSockAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	}

	if( bind(m_iSocketFD, (const struct sockaddr *)&stTempSockAddr, sizeof(stTempSockAddr)) )
	{
		Close();
		LOG_ERROR("default", "Bind failed, %s.\n", strerror(errno));
		return -1;
	}

	iOptLen = sizeof(socklen_t);
	iOptVal = SENDBUFSIZE;
	if (setsockopt(m_iSocketFD, SOL_SOCKET, SO_SNDBUF, (const void *)&iOptVal, iOptLen))
	{
		LOG_ERROR( "default", "Set send buffer size to %d failed!\n", iOptVal);
		//return -1;
	}
	if (getsockopt(m_iSocketFD, SOL_SOCKET, SO_SNDBUF, (void *)&iOptVal, (socklen_t *)&iOptLen) < 0)
	{
		// 系统默认会设置成传入参数的2倍
		if( (int)(SENDBUFSIZE * 2) != (int)(iOptVal) )
		{
			LOG_ERROR( "default", "Set Send buf of socket is %d.\n", iOptVal);
		}
	}
	if( listen(m_iSocketFD, 1024) < 0 )
	{
		LOG_ERROR( "default", "Listen failed, %s.\n", strerror(errno));
		Close();
		return -1;
	}

	SetNBlock( m_iSocketFD );
	m_iStatus = tcs_opened;

	return 0;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::ConnectTo(u_long ulIPNetAddr, u_short unPort, eLinkMode emBlock)
{
	sockaddr_in stTempAddr;

	if( m_iStatus != tcs_opened || m_iSocketFD < 0 )
	{
		return -2;
	}

	memset((void *)&stTempAddr, 0, sizeof(sockaddr_in));
	stTempAddr.sin_family = AF_INET;
	stTempAddr.sin_port = htons(unPort);
	stTempAddr.sin_addr.s_addr = ulIPNetAddr;

	if ( emBlock == em_nblock_mode )
	{
		SetNBlock(m_iSocketFD);		
	}

	if( connect(m_iSocketFD, (const struct sockaddr *)&stTempAddr, sizeof(stTempAddr)) )
	{
		if (emBlock == em_block_mode)
		{
			Close();
			return -3;
		}

		// emBlock == nblock_mode
		if (errno != EINPROGRESS) 	
		{
			Close();
			return -3;
		}

		m_iStatus = tcs_connecting;

		return 0;
	}

	if ( emBlock == em_block_mode)
	{
		SetNBlock( m_iSocketFD );
	}

	m_iReadBegin = m_iReadEnd = 0;
	m_iPostBegin = m_iPostEnd = 0;
	m_iStatus = tcs_connected;

	return 0;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::CheckNoblockConnecting(int nto)
{
	struct timeval tv; 
	tv.tv_sec = nto / 1000000; 
	tv.tv_usec = nto % 1000000;

	fd_set readfds, writefds, execpfds; 

	FD_ZERO(&writefds); 
	FD_SET(m_iSocketFD, &writefds); 
	readfds = writefds;
	execpfds = writefds;

	int n = select(m_iSocketFD+1, &readfds, &writefds, &execpfds, &tv);

	if ( n < 0 )
	{
		LOG_ERROR("default", "select error(%d:%s)", errno, strerror(errno));
		Close();
		return -4;
	}

	if ( n == 0 )
	{
		if (nto != 0 )
		{
			LOG_ERROR("default", "select timeout of (%d sec %d usec)", tv.tv_sec, tv.tv_usec);
		}

		//Close();
		return 0;
	}

	if (!FD_ISSET(m_iSocketFD, &readfds) && !FD_ISSET(m_iSocketFD, &writefds)) 
	{
		LOG_ERROR("default", "描述字 %d 不能读写", m_iSocketFD);
		Close();
		return -6;
	}

	int err = 0;
	socklen_t len = sizeof(sockaddr);
	if (getsockopt(m_iSocketFD, SOL_SOCKET, SO_ERROR, &err, &len) < 0 || err != 0)
	{
		LOG_ERROR("default", "connect failure on getsockopt(SO_ERROR) err(%d)",  err);
		Close();
		return -7;
	}


	m_iReadBegin = m_iReadEnd = 0;
	m_iPostBegin = m_iPostEnd = 0;
	m_iStatus = tcs_connected;

	return 0;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::ConnectTo(char *szIPAddr, unsigned short unPort)
{
	sockaddr_in stTempAddr;

	if( !szIPAddr )
	{
		return -1;
	}

	if( m_iStatus != tcs_opened || m_iSocketFD < 0 )
	{
		return -2;
	}

	memset((void *)&stTempAddr, 0, sizeof(sockaddr_in));
	stTempAddr.sin_family = AF_INET;
	stTempAddr.sin_port = htons(unPort);
	stTempAddr.sin_addr.s_addr = inet_addr(szIPAddr);

	if( connect(m_iSocketFD, (const struct sockaddr *)&stTempAddr, sizeof(stTempAddr)) )
	{
		Close();
		return -3;
	}

	SetNBlock( m_iSocketFD );
	m_iReadBegin = m_iReadEnd = 0;
	m_iPostBegin = m_iPostEnd = 0;
	m_iStatus = tcs_connected;

	return 0;
}

// -1 --- Invalid input fd, -2 ----- socket already connected
template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::Accept(int iAcceptFD)
{
	int iTempRet = 0;

	if( iAcceptFD < 0 )
	{
		return -1;
	}
#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif
	if( m_iSocketFD > 0 && m_iStatus == tcs_connected )
	{
		LOG_ERROR( "default", "Warning, another connection request from remote, close the previous(%d).\n", m_iSocketFD);
		Close();
		//iTempRet = -2;
	}

	m_iSocketFD = iAcceptFD;
	m_iSocketType = sot_comm;
	m_iStatus = tcs_connected;
	m_iReadBegin = 0;
	m_iReadEnd = 0;
	m_iPostBegin = m_iPostEnd = 0;
	SetNBlock( m_iSocketFD );

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif
	return iTempRet;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::Close()
{
	if( m_iSocketFD > 0 )
	{
		close(m_iSocketFD);
	}

	m_iSocketFD = -1;
	m_iStatus = tcs_closed;

	return 0;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::GetSocketFD()
{
	int iTmpFD = -1;

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	iTmpFD = m_iSocketFD;

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return iTmpFD;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::GetStatus()
{
	int iTmpStatus = tcs_closed;

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	iTmpStatus = m_iStatus;

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return iTmpStatus;
}

//返回值：-1 ：Socket状态错误；-2 ：接收缓冲区已满；-3 ：对端断开连接；-4 ：接收错误
template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::RecvData()
{
	int iRecvedBytes = 0;
	int iTempRet = 0;

	char szPeerAddr[32];
	sockaddr stPeerAddr;
	socklen_t iPeerAddrLen = sizeof(stPeerAddr); 
	
	//使用互斥锁保护临界区代码
#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	if( m_iSocketFD < 0 || m_iStatus != tcs_connected )
	{
#ifdef _POSIX_MT_
		pthread_mutex_unlock( &m_stMutex );
#endif
		return ERR_RECV_NOSOCK;
	}

	if( m_iReadEnd == m_iReadBegin )
	{
		m_iReadBegin = 0;
		m_iReadEnd = 0;
	}
	
	do
	{
		
		if( m_iReadEnd == sizeof(m_abyRecvBuffer) )
		{
			LOG_ERROR( "default", "The recv buffer is full now(%d, %d), stop recv data, fd = %d.\n", m_iReadBegin, m_iReadEnd, m_iSocketFD);
			iTempRet = ERR_RECV_NOBUFF;
			break;
		}

		iRecvedBytes = recv(m_iSocketFD, &m_abyRecvBuffer[m_iReadEnd],
							sizeof(m_abyRecvBuffer)-m_iReadEnd, 0);
		if( iRecvedBytes > 0 )
		{
			m_iReadEnd += iRecvedBytes;
		}
		else if( iRecvedBytes == 0 )
		{
			getpeername(m_iSocketFD, &stPeerAddr, &iPeerAddrLen);
			SockAddrToString((sockaddr_in *)&stPeerAddr, szPeerAddr);
			LOG_ERROR( "default", "The remote site %s may closed this conn, fd = %d, errno = %d.\n", szPeerAddr, m_iSocketFD, errno);
			Close();
			iTempRet = ERR_RECV_REMOTE;
			break;
		}
		else if( errno != EAGAIN )
		{
			getpeername(m_iSocketFD, &stPeerAddr, &iPeerAddrLen);
			SockAddrToString((sockaddr_in *)&stPeerAddr, szPeerAddr);
			LOG_ERROR( "default", "Error in read, %s, socket fd = %d, remote site %s.\n", strerror(errno), m_iSocketFD, szPeerAddr);
			Close();
			iTempRet = ERR_RECV_FALIED;
			break;
		}
	} while( iRecvedBytes > 0 );

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif
	
	return iTempRet;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::GetOneCode(unsigned short &nCodeLength, BYTE *pCode, eByteMode emByte)
{
	unsigned short shMaxBufferLen = nCodeLength;
	int iDataLength = 0;
	unsigned short nTempLength;
	
	if( !pCode )
	{
		return -1;
	}
	
	//RecvData();

	iDataLength = m_iReadEnd - m_iReadBegin;

	if( iDataLength <= 0 )
	{
		return 0;
	}

	if( iDataLength < (int) sizeof(short) )
	{
		if( m_iReadEnd == sizeof(m_abyRecvBuffer) )
		{
			memcpy((void *)&m_abyRecvBuffer[0], (const void *)&m_abyRecvBuffer[m_iReadBegin], iDataLength);
			m_iReadBegin = 0;
			m_iReadEnd = iDataLength;
		}

		return 0;
	}

	if (emByte == use_network_byte)
	{
		nTempLength = ntohs(*((unsigned short *)&m_abyRecvBuffer[m_iReadBegin]));
	}else
	{
		nTempLength = /*ntohs*/(*((unsigned short *)&m_abyRecvBuffer[m_iReadBegin]));
	}

	if( nTempLength == 0 || nTempLength+sizeof(short) > sizeof(m_abyRecvBuffer) )
	{
		m_iReadBegin = m_iReadEnd = 0;
		Close();
		return -2;
	}

	if( iDataLength < (int) nTempLength )
	{
		if( m_iReadEnd == sizeof(m_abyRecvBuffer) )
		{
			memcpy((void *)&m_abyRecvBuffer[0], (const void *)&m_abyRecvBuffer[m_iReadBegin], iDataLength);
			m_iReadBegin = 0;
			m_iReadEnd = iDataLength;
		}

		return 0;
	}


	int iTempRet = 1;
	nCodeLength = nTempLength;

	if( nCodeLength < shMaxBufferLen )
	{
		memcpy((void *)pCode, (const void *)&m_abyRecvBuffer[m_iReadBegin], nCodeLength);
	}
	else
	{	
		iTempRet = -2;
		Close();
		return iTempRet;
	}

	m_iReadBegin += nTempLength;

	if( m_iReadBegin == m_iReadEnd )
	{
		m_iReadBegin = m_iReadEnd = 0;
	}
	
	return iTempRet;
}


template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::GetOneHttpCode(int &nCodeLength, BYTE *pCode)
{
	int shMaxBufferLen = nCodeLength;
	int iDataLength = 0;
	unsigned int nTempLength;
	
	if( !pCode )
	{
		return -1;
	}
	
	iDataLength = m_iReadEnd - m_iReadBegin;

	if( iDataLength <= 0 || iDataLength > shMaxBufferLen)
	{
		return 0;
	}


	memcpy((void *)pCode, (const void *)&m_abyRecvBuffer[m_iReadBegin], iDataLength);
	pCode[iDataLength] = 0;

	const char* LENTH_TOKEN = "Content-Length";
	const char* HEAD_BODY_SPLITTER = "\n\n" ;
	const char* HEAD_BODY_SPLITTER_NEW = "\r\n\r\n" ;
	int nHeadSplitterLen = 0;
	
	char* pSpliter = strstr((const char*) pCode, HEAD_BODY_SPLITTER);
	if ( pSpliter == NULL )
	{
		pSpliter = strstr((const char*) pCode, HEAD_BODY_SPLITTER_NEW);
		nHeadSplitterLen =  strlen(HEAD_BODY_SPLITTER_NEW);
	}
	else
	{
		nHeadSplitterLen = strlen( HEAD_BODY_SPLITTER );
	}
	
	if (pSpliter != NULL)
	{
		char* pToken = strstr((const char*) pCode, LENTH_TOKEN);
		if (pToken != NULL)
		{
			char* pStart = strstr(pToken + strlen(LENTH_TOKEN), ":");
			char* pEnd = NULL;

			if (pStart != NULL) pEnd = strstr(pStart, "\n");

			if (pStart != NULL && pEnd != NULL && pStart < pSpliter && pEnd <= pSpliter )
			{
				nTempLength = atoi(pStart + 1) + (pSpliter - (char*) pCode) + nHeadSplitterLen;
			}else
			{
				//协议格式不正确
				m_iReadBegin = m_iReadEnd = 0;
				Close();
				return -1;
			}
		}else
		{
			//协议格式不正确
			m_iReadBegin = m_iReadEnd = 0;
			Close();
			return -1;
		}
	}else
	{
		if (iDataLength > 4096)  
		{
			//协议格式不正确
			m_iReadBegin = m_iReadEnd = 0;
			Close();
			return -1;
		}

		if( m_iReadEnd == sizeof(m_abyRecvBuffer) )
		{
			memcpy((void *)&m_abyRecvBuffer[0], (const void *)&m_abyRecvBuffer[m_iReadBegin], iDataLength);
			m_iReadBegin = 0;
			m_iReadEnd = iDataLength;
		}

		return 0;
	}

	if( nTempLength == 0 || nTempLength > sizeof(m_abyRecvBuffer) )
	{
		m_iReadBegin = m_iReadEnd = 0;
		Close();
		return -2;
	}

	if( iDataLength < (int)(nTempLength) )
	{
		if( m_iReadEnd == sizeof(m_abyRecvBuffer) )
		{
			memcpy((void *)&m_abyRecvBuffer[0], (const void *)&m_abyRecvBuffer[m_iReadBegin], iDataLength);
			m_iReadBegin = 0;
			m_iReadEnd = iDataLength;
		}

		return 0;
	}


	int iTempRet = 1;
	nCodeLength = nTempLength;

	if( nCodeLength < shMaxBufferLen )
	{
		memcpy((void *)pCode, (const void *)&m_abyRecvBuffer[m_iReadBegin], nCodeLength);
	}
	else
	{	
		iTempRet = -2;
		Close();
		return iTempRet;
	}

	m_iReadBegin += nTempLength;

	if( m_iReadBegin == m_iReadEnd )
	{
		m_iReadBegin = m_iReadEnd = 0;
	}
	
	return iTempRet;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::GetOneCode32( int &iCodeLength, BYTE * pCode)
{
	int iMaxBufferLen = iCodeLength;
	int iDataLength = 0;
	int iTempLength = 0;
	
	if( !pCode )
	{
		return -1;
	}
	
	//RecvData();

	iDataLength = m_iReadEnd - m_iReadBegin;

	if( iDataLength <= 0 )
	{
		return 0;
	}

	if( iDataLength < sizeof(int) )
	{
		if( m_iReadEnd == sizeof(m_abyRecvBuffer) )
		{
			memcpy((void *)&m_abyRecvBuffer[0], (const void *)&m_abyRecvBuffer[m_iReadBegin], iDataLength);
			m_iReadBegin = 0;
			m_iReadEnd = iDataLength;
		}

		return 0;
	}

	iTempLength = (int)/*ntohl*/(*((int *)&m_abyRecvBuffer[m_iReadBegin]));

	if( iTempLength <= 0 || iTempLength+sizeof(int) > sizeof(m_abyRecvBuffer) )
	{
		m_iReadBegin = m_iReadEnd = 0;
		Close();
		return -2;
	}

	if( iDataLength < iTempLength )
	{
		if( m_iReadEnd == sizeof(m_abyRecvBuffer) )
		{
			memcpy((void *)&m_abyRecvBuffer[0], (const void *)&m_abyRecvBuffer[m_iReadBegin], iDataLength);
			m_iReadBegin = 0;
			m_iReadEnd = iDataLength;
		}

		return 0;
	}


	int iTempRet = 1;
	iCodeLength = iTempLength;

	if( iCodeLength < iMaxBufferLen )
	{
		memcpy((void *)pCode, (const void *)&m_abyRecvBuffer[m_iReadBegin], iCodeLength);
	}
	else
	{	
		iTempRet = -2;
		Close();
		return iTempRet;
	}

	m_iReadBegin += iTempLength;

	if( m_iReadBegin == m_iReadEnd )
	{
		m_iReadBegin = m_iReadEnd = 0;
	}
	
	return iTempRet;
}


//返回值说明：-1：参数错误或状态非法；-2：发送缓冲区满；-3：发送系统错误；
template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::SendOneCode(unsigned short nCodeLength, BYTE *pCode)
{
	int iBytesSent = 0;
	int iBytesLeft = nCodeLength;
	BYTE *pbyTemp = NULL;
	int iTempRet = 0;
	
	if( !pCode )
	{
		return ERR_SEND_NOSOCK;
	}

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	
	if( m_iSocketFD < 0 || m_iStatus != tcs_connected )
	{
#ifdef _POSIX_MT_
		pthread_mutex_unlock( &m_stMutex );
#endif
		return ERR_SEND_NOSOCK;
	}
	
	//首先检查是否有滞留数据
	iBytesLeft = m_iPostEnd - m_iPostBegin;
	pbyTemp = &(m_abyPostBuffer[m_iPostBegin]);
	while( iBytesLeft > 0 )
	{
		iBytesSent = send(m_iSocketFD, (const char *)pbyTemp, iBytesLeft, 0);
		
		if( iBytesSent > 0 )
		{
			pbyTemp += iBytesSent;
			iBytesLeft -= iBytesSent;
			m_iPostBegin += iBytesSent;
		}


		if( iBytesSent < 0 && errno != EAGAIN )

		{
			m_iStatus = tcs_error;
			iTempRet = ERR_SEND_FAILED;
			break;
		}
		else if( iBytesSent < 0 )
		{
			LOG_ERROR( "default", "[%s:%d] send failed, errorno %d", __FUNCTION__, __LINE__, errno );
			iTempRet = ERR_SEND_NOBUFF;
			break;
		}
	}

	if( iBytesLeft == 0 )
	{
		//滞留数据发送成功，则继续发送本次提交的数据
		m_iPostBegin = m_iPostEnd = 0;
	}
	else
	{
		//否则，直接返回
#ifdef _POSIX_MT_
		pthread_mutex_unlock( &m_stMutex );
#endif	
		return iTempRet;
	}

	//发送本次提交的数据
	iBytesLeft = nCodeLength;
	pbyTemp = pCode;

	while( iBytesLeft > 0 )
	{
		iBytesSent = send(m_iSocketFD, (const char *)pbyTemp, iBytesLeft, 0);
		
		if( iBytesSent > 0 )
		{
			pbyTemp += iBytesSent;
			iBytesLeft -= iBytesSent;
		}


		if( iBytesSent < 0 && errno != EAGAIN )

		{
			m_iStatus = tcs_error;
			iTempRet = ERR_SEND_FAILED;
			break;
		}
		
		else if( iBytesSent < 0 )
		{
			LOG_ERROR( "default", "[%s:%d] send failed, errorno %d", __FUNCTION__, __LINE__, errno );
			//Socket发送缓冲区满，则将剩余的数据放到缓存中
			memcpy((void *)&(m_abyPostBuffer[m_iPostEnd]), (const void *)pbyTemp, iBytesLeft);
			m_iPostEnd += iBytesLeft;
			iTempRet = ERR_SEND_NOBUFF;
			break;
		}
		
	}

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return iTempRet;
}

//返回值说明：-1：参数错误或状态非法；-2：发送缓冲区满；-3：发送系统错误；
template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::SendOneCode32(int iCodeLength, BYTE *pCode)
{
	int iBytesSent = 0;
	int iBytesLeft = iCodeLength;
	BYTE *pbyTemp = NULL;
	int iTempRet = 0;
	
	if( !pCode )
	{
		return ERR_SEND_NOSOCK;
	}

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	
	if( m_iSocketFD < 0 || m_iStatus != tcs_connected )
	{
#ifdef _POSIX_MT_
		pthread_mutex_unlock( &m_stMutex );
#endif
		return ERR_SEND_NOSOCK;
	}
	
	//首先检查是否有滞留数据
	iBytesLeft = m_iPostEnd - m_iPostBegin;
	pbyTemp = &(m_abyPostBuffer[m_iPostBegin]);
	while( iBytesLeft > 0 )
	{
		iBytesSent = send(m_iSocketFD, (const char *)pbyTemp, iBytesLeft, 0);
		
		if( iBytesSent > 0 )
		{
			pbyTemp += iBytesSent;
			iBytesLeft -= iBytesSent;
			m_iPostBegin += iBytesSent;
		}


		if( iBytesSent < 0 && errno != EAGAIN )

		{
			m_iStatus = tcs_error;
			iTempRet = ERR_SEND_FAILED;
			break;
		}
		else if( iBytesSent < 0 )
		{
			iTempRet = ERR_SEND_NOBUFF;
			break;
		}
	}

	if( iBytesLeft == 0 )
	{
		//滞留数据发送成功，则继续发送本次提交的数据
		m_iPostBegin = m_iPostEnd = 0;
	}
	else
	{
		//否则，直接返回
#ifdef _POSIX_MT_
		pthread_mutex_unlock( &m_stMutex );
#endif	
		return iTempRet;
	}

	//发送本次提交的数据
	iBytesLeft = iCodeLength;
	pbyTemp = pCode;

	while( iBytesLeft > 0 )
	{
		iBytesSent = send(m_iSocketFD, (const char *)pbyTemp, iBytesLeft, 0);
		
		if( iBytesSent > 0 )
		{
			pbyTemp += iBytesSent;
			iBytesLeft -= iBytesSent;
		}


		if( iBytesSent < 0 && errno != EAGAIN )

		{
			m_iStatus = tcs_error;
			iTempRet = ERR_SEND_FAILED;
			break;
		}
		
		else if( iBytesSent < 0 )
		{
			//Socket发送缓冲区满，则将剩余的数据放到缓存中
			memcpy((void *)&(m_abyPostBuffer[m_iPostEnd]), (const void *)pbyTemp, iBytesLeft);
			m_iPostEnd += iBytesLeft;
			iTempRet = ERR_SEND_NOBUFF;
			break;
		}
		
	}

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return iTempRet;
}


template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::AddToCheckSet(fd_set *pCheckSet)
{
	int iTempRet = 0;

	if( !pCheckSet )
	{
		return -1;
	}

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	if( m_iSocketFD > 0 && m_iStatus == tcs_connected )
	{
		FD_SET( m_iSocketFD, pCheckSet );
	}
	else if( m_iSocketFD > 0 )
	{
		Close();
		iTempRet = -2;
	}

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return iTempRet;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::IsFDSetted(fd_set *pCheckSet)
{
	int iTempRet = False;

	if( !pCheckSet )
	{
		return False;
	}

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	if( m_iSocketFD > 0 && m_iStatus == tcs_connected )
	{
		iTempRet = FD_ISSET( m_iSocketFD, pCheckSet );
	}
	else
	{
		iTempRet = False;
	}

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return iTempRet;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::SetNBlock(int iSock)
{

	int iFlags;
	iFlags = fcntl(iSock, F_GETFL, 0);
	iFlags |= O_NONBLOCK;
	iFlags |= O_NDELAY;
	fcntl(iSock, F_SETFL, iFlags);
	return 0;

}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
void CTCPSocket<uiRecvBufLen, uiSendBufLen>::GetCriticalData(int& iReadBegin,int& iReadEnd, int& iPostBegin, int& iPostEnd)
{
	iReadBegin = m_iReadBegin;
	iReadEnd = m_iReadEnd;
	iPostBegin = m_iPostBegin;
	iPostEnd = m_iPostEnd;
}


template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::HasReserveData()
{
	if(m_iPostEnd - m_iPostBegin > 0)
	{
		return True;
	}
	else
	{
		return False;
	}
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPSocket<uiRecvBufLen, uiSendBufLen>::CleanReserveData()
{
	int iBytesSent = 0, iBytesLeft = 0, iBytesCleaned = 0, iTempRet = 0;
	BYTE *pbyTemp = NULL;

	if( m_iSocketFD < 0 || m_iStatus != tcs_connected )
	{
		return ERR_SEND_NOSOCK;
	}
	iBytesLeft = m_iPostEnd - m_iPostBegin;
	pbyTemp = &(m_abyPostBuffer[m_iPostBegin]);
	while( iBytesLeft > 0 )
	{
		iBytesSent = send(m_iSocketFD, (const char *)pbyTemp, iBytesLeft, 0);
		if( iBytesSent > 0 )
		{
			pbyTemp += iBytesSent;
			iBytesLeft -= iBytesSent;
			m_iPostBegin += iBytesSent;
			iBytesCleaned += iBytesSent;
		}
#ifdef WIN32
		if( iBytesSent < 0 && WSAGetLastError() != WSAEWOULDBLOCK )
#else
		if( iBytesSent < 0 && errno != EAGAIN )
#endif
		{
			m_iStatus = tcs_error;
			iTempRet = ERR_SEND_FAILED;
			break;
		}
		else if( iBytesSent < 0 )
		{
			iTempRet = ERR_SEND_NOBUFF;
			break;                                                                                                
		}
	}
	if( iBytesLeft == 0 )                                                                                         
	{
		m_iPostBegin = m_iPostEnd = 0;                                                                            
		iTempRet = 0;                                                                                             
	}
	//SAY("%d bytes is cleaned, left %d bytes.\n", iBytesCleaned, iBytesLeft);                                      
	return iTempRet;                                                                                              
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
CTCPConn<uiRecvBufLen, uiSendBufLen>::CTCPConn()
{
	m_ulIPAddr = 0;
	m_nEntityID = -1;
	m_nEntityType = -1;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
CTCPConn<uiRecvBufLen, uiSendBufLen>::~CTCPConn()
{
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPConn<uiRecvBufLen, uiSendBufLen>::Initialize(short nEntityType, short nEntityID, u_long ulIPAddr, u_short unPort)
{
	m_ulIPAddr = ulIPAddr;
	m_unPort = unPort;
	m_nEntityID = nEntityID;
	m_nEntityType = nEntityType;

	return 0;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPConn<uiRecvBufLen, uiSendBufLen>::ConnectToServer(char* szLocalAddr, eLinkMode emBlock)
{
	if( m_nEntityID <= 0 )
	{
		return -1;
	}

	if( m_stSocket.CreateClient(szLocalAddr) )
	{
		LOG_ERROR( "default", "Conn create client of  %d failed.\n", m_nEntityID);
		return -1;
	}

	return m_stSocket.ConnectTo(m_ulIPAddr, m_unPort, emBlock);
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPConn<uiRecvBufLen, uiSendBufLen>::CreateServer()
{
	struct in_addr in;
	if( m_nEntityID <= 0 )
	{
		return -1;
	}
	
	in.s_addr =  m_ulIPAddr;
	if( m_stSocket.CreateServer(m_unPort, inet_ntoa(in)))
	{
		LOG_ERROR( "default", "Conn create DB Listen Server  %d failed.\n", m_nEntityID);
		return -1;
	}
	return 0;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
CTCPSocket<uiRecvBufLen, uiSendBufLen>* CTCPConn<uiRecvBufLen, uiSendBufLen>::GetSocket()
{
	return &m_stSocket;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
int CTCPConn<uiRecvBufLen, uiSendBufLen>::GetEntityInfo(short* pnEntityType, short* pnEntityID, unsigned long* pulIpAddr)
{
	if( !pnEntityType || !pnEntityID || !pulIpAddr )
	{
		return -1;
	}

	*pnEntityType = m_nEntityType;
	*pnEntityID = m_nEntityID;
	*pulIpAddr = m_ulIPAddr;

	return 0;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
u_long CTCPConn<uiRecvBufLen, uiSendBufLen>::GetConnAddr()
{
	return m_ulIPAddr;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
u_short CTCPConn<uiRecvBufLen, uiSendBufLen>::GetConnPort()
{
	return m_unPort;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
short CTCPConn<uiRecvBufLen, uiSendBufLen>::GetEntityType()
{
	return m_nEntityType;
}

template<unsigned int uiRecvBufLen, unsigned int uiSendBufLen>
short CTCPConn<uiRecvBufLen, uiSendBufLen>::GetEntityID()
{
	return m_nEntityID;
}

