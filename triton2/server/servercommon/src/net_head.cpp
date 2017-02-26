#include <stdio.h>
#include <stdarg.h>
#include <netinet/in.h>

#include "base.h"
#include "encoder.h"
#include "tlib_log.h"
#include "net_head.h"

CNetHead::CNetHead()
{
	m_iSrcIP = 0;
	m_iDstIP = 0;
	m_nSrcPort = 0;
	m_nDstPort = 0;
	m_tStamp = 0;
}
CNetHead::~CNetHead()
{

}
int CNetHead::Encode(char *pcOutBuffer, short &sOutLength )
{
	char *pcTemp;
	short sTempLength;

	if( !pcOutBuffer )
	{
		return -1;
	}

	pcTemp = pcOutBuffer;
	sOutLength = 0;
	sTempLength = EncodeInt( &pcTemp, m_iSrcIP );
	sOutLength += sTempLength;

	sTempLength = EncodeInt( &pcTemp, m_iDstIP );
	sOutLength += sTempLength;

	sTempLength = EncodeShort( &pcTemp, m_nSrcPort );
	sOutLength += sTempLength;

	sTempLength = EncodeShort( &pcTemp, m_nDstPort );
	sOutLength += sTempLength;

	sTempLength = EncodeInt( &pcTemp, (unsigned int)m_tStamp );
	sOutLength += sTempLength;

	return 0;
}

int CNetHead::Decode(const char *pcInBuffer, const short sInLength )
{
	char *pcTemp;
	short sLeftLength;
	short sTempLength;

	if( !pcInBuffer || sInLength <= 0 )
	{
		return -1;
	}

	pcTemp = (char *)pcInBuffer;
	sLeftLength = sInLength;
	
	sTempLength = DecodeInt( &pcTemp, &m_iSrcIP );
	sLeftLength -= sTempLength;

	sTempLength = DecodeInt( &pcTemp, &m_iDstIP );
	sLeftLength -= sTempLength;

	sTempLength = DecodeShort( &pcTemp, &m_nSrcPort );
	sLeftLength -= sTempLength;

	sTempLength = DecodeShort( &pcTemp, &m_nDstPort );
	sLeftLength -= sTempLength;

	sTempLength = DecodeInt( &pcTemp, (unsigned int *)&m_tStamp );
	sLeftLength -= sTempLength;
	
	if( sLeftLength < 0 )
	{
		return -1;
	}

	return 0;
}

void CNetHead::Print()
{
	char szTempAddrFrom[32];
	char szTempAddrTo[32];
	char szTempStamp[64];
	sockaddr_in stTempSockAddr;

	szTempStamp[0] = '\0';
	GetTimeString( (time_t)m_tStamp, szTempStamp );

	stTempSockAddr.sin_addr.s_addr = m_iSrcIP;
	stTempSockAddr.sin_port = htons(m_nSrcPort);
	SockAddrToString( &stTempSockAddr, szTempAddrFrom );

	stTempSockAddr.sin_addr.s_addr = m_iDstIP;
	stTempSockAddr.sin_port = htons(m_nDstPort);
	SockAddrToString( &stTempSockAddr, szTempAddrTo );

	LOG("Net Head From %s to %s, STAMP = %s\n",  szTempAddrFrom, szTempAddrTo, szTempStamp);
}

