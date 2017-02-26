#include "log.h"
#include "sock_stream.h"
#include <fcntl.h>
#include <errno.h>

CSockStream::CSockStream()
{
	m_iSocketFD = -1;
	m_iStatus = STREAM_CLOSED;

	m_iReadBegin = m_iReadEnd = 0;
	m_iPostBegin = m_iPostEnd = 0;
}

CSockStream::~CSockStream()
{
	if( STREAM_OPENED == m_iStatus && m_iSocketFD > 0 )
	{
		CloseStream();
	}
}

int CSockStream::SetUpStream(int iFD)
{
	if( iFD < 0 )
	{
		return -1;
	}

	if( m_iSocketFD > 0 && STREAM_OPENED == m_iStatus )
	{
		LOG_ERROR( "default", "Warning, setup new stream, so close the previous(%d).\n", m_iSocketFD);
		CloseStream();
	}

	m_iSocketFD = iFD;
	m_iStatus = STREAM_OPENED;
	m_iReadBegin = m_iReadEnd = 0;
	m_iPostBegin = m_iPostEnd = 0;

	int iFlags = fcntl(m_iSocketFD, F_GETFL, 0);
	iFlags |= O_NONBLOCK;
	iFlags |= O_NDELAY;
	fcntl(m_iSocketFD, F_SETFL, iFlags);

	return 0;
}

int CSockStream::CloseStream()
{
	if( m_iSocketFD > 0 )
	{
		close(m_iSocketFD);
	}

	m_iSocketFD = -1;
	m_iStatus = STREAM_CLOSED;

	return 0;
}

int CSockStream::GetSocketFD()
{
	return m_iSocketFD;
}

int CSockStream::GetStatus()
{
	return m_iStatus;
}

int CSockStream::AddToCheckSet(fd_set *pCheckSet)
{
	int iTempRet = 0;

	if( !pCheckSet )
	{
		return -1;
	}

	if( m_iSocketFD > 0 && m_iStatus == STREAM_OPENED )
	{
		FD_SET( m_iSocketFD, pCheckSet );
	}
	else if( m_iSocketFD > 0 )
	{
		CloseStream();
		iTempRet = ERR_STRM_NOSOCK;
	}

	return iTempRet;
}

int CSockStream::IsFDSetted(fd_set *pCheckSet)
{
	int iTempRet = False;

	if( !pCheckSet )
	{
		return False;
	}

	if( m_iSocketFD > 0 && STREAM_OPENED == m_iStatus )
	{
		iTempRet = FD_ISSET( m_iSocketFD, pCheckSet );
	}
	else
	{
		iTempRet = False;
	}

	return iTempRet;
}

int CSockStream::RecvData()
{
	int iRecvedBytes = 0;
	int iMaxBytes = 0;
	int iTempRet = 0;

	if( m_iSocketFD < 0 || STREAM_OPENED != m_iStatus )
	{
		return ERR_STRM_NOSOCK;
	}

	if( m_iReadEnd == m_iReadBegin )
	{
		m_iReadBegin = 0;
		m_iReadEnd = 0;
	}

	do
	{
		//如果接收缓冲区已满，停止接收数据
		if( m_iReadEnd == sizeof(m_abyRecvBuffer) )
		{
			LOG_ERROR( "default", "The recv buffer is full now(%d, %d), stop recv data, fd = %d.\n",
							m_iReadBegin, m_iReadEnd, m_iSocketFD);
			iTempRet = ERR_STRM_AGAIN;
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
			LOG_ERROR( "default", "The peer side may closed this stream, fd = %d, errno = %d.\n", m_iSocketFD, errno);
			CloseStream();
			iTempRet = ERR_STRM_PEER;
			break;
		}
		else if( errno != EAGAIN )
		{
			LOG_ERROR( "default", "Error in read, %s, socket fd = %d.\n", strerror(errno), m_iSocketFD);
			CloseStream();
			iTempRet = ERR_STRM_FAILED;
			break;
		}
	} while( iRecvedBytes > 0 );

	return iTempRet;
}

int CSockStream::GetOneCode(short &nCodeLength, BYTE *pCode)
{
	int iDataLength = 0;
	short nTempLength;

	if( !pCode )
	{
		return -1;
	}

	iDataLength = m_iReadEnd - m_iReadBegin;

	if( iDataLength <= 0 )
	{
		return 0;
	}

	if( iDataLength < sizeof(short) )
	{
		if( m_iReadEnd == sizeof(m_abyRecvBuffer) )
		{
			memcpy((void *)&m_abyRecvBuffer[0], (const void *)&m_abyRecvBuffer[m_iReadBegin], iDataLength);
			m_iReadBegin = 0;
			m_iReadEnd = iDataLength;
		}

		return 0;
	}

	memcpy((void *)&nTempLength, (const void *)&m_abyRecvBuffer[m_iReadBegin], sizeof(short));
	nTempLength = ntohs(nTempLength);
	//nTempLength = (short)ntohs(*((short *)&m_abyRecvBuffer[m_iReadBegin]));

	if( iDataLength < nTempLength )
	{
		if( m_iReadEnd == sizeof(m_abyRecvBuffer) )
		{
			memmove((void *)&m_abyRecvBuffer[0], (const void *)&m_abyRecvBuffer[m_iReadBegin], iDataLength);
			m_iReadBegin = 0;
			m_iReadEnd = iDataLength;
		}
		return 0;
	}

	nCodeLength = nTempLength;
	memcpy((void *)pCode, (const void *)&m_abyRecvBuffer[m_iReadBegin], nCodeLength);

	m_iReadBegin += nTempLength;
	if( m_iReadBegin == m_iReadEnd )
	{
		m_iReadBegin = m_iReadEnd = 0;
	}

	return 1;
}

int CSockStream::SendOneCode(short nCodeLength, BYTE *pCode)
{
	int iBytesSent = 0;
	int iBytesLeft = nCodeLength;
	BYTE *pbyTemp = NULL;
	int iTempRet = 0;

	if( !pCode )
	{
		return ERR_STRM_NOSOCK;
	}

	if( m_iSocketFD < 0 || STREAM_OPENED != m_iStatus )
	{
		return ERR_STRM_NOSOCK;
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
		}


		if( iBytesSent < 0 && errno != EAGAIN )

		{
			//出现发送错误，关闭流
			CloseStream();
			LOG_ERROR( "default", "Stream send error of %d.\n", errno);
			iTempRet = ERR_STRM_FAILED;
			break;
		}
		else if( iBytesSent < 0 )
		{
			//发送缓冲已满，停止发送，剩余数据保持原位
			iTempRet = ERR_STRM_AGAIN;
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
			//出现发送错误，关闭流
			CloseStream();
			LOG_ERROR( "default", "Stream send error of %d.\n", errno);
			iTempRet = ERR_STRM_FAILED;
			break;
		}
		else if( iBytesSent < 0 )
		{
			//Socket发送缓冲区满，则将剩余的数据放到缓存中
			memcpy((void *)&(m_abyPostBuffer[m_iPostEnd]), (const void *)pbyTemp, iBytesLeft);
			m_iPostEnd += iBytesLeft;
			//告诉调用者已经缓存,当发送成功处理
			iTempRet = 0;
			break;
		}
	}

	return iTempRet;
}

