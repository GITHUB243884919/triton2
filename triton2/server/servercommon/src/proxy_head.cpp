#include "proxy_head.h"
#include "encoder.h"
#include "log.h"

CProxyHead::CProxyHead()
{
	m_nTotalLength = 0;
	m_nSrcFE = -1;
	m_nDstFE = -1;
	m_nSrcID = -1;
	memset((void *)&m_stTransferInfo, 0, sizeof(m_stTransferInfo));
	m_byOpFlag = 0;
	m_nOptLength = 0;
	m_nBodyLength = 0;
}
CProxyHead::~CProxyHead()
{
}
int CProxyHead::Encode(char *pCode, short & nCodeLength)
{
	char *pcTemp;
	short sTempLength;

	if( !pCode )
	{
		return -1;
	}

	pcTemp = pCode;
	nCodeLength = 0;

	sTempLength = EncodeShort( &pcTemp, (unsigned short)m_nTotalLength );
	nCodeLength += sTempLength;

	sTempLength = EncodeShort( &pcTemp, (unsigned short)m_nSrcFE );
	nCodeLength += sTempLength;

	sTempLength = EncodeShort( &pcTemp, (unsigned short)m_nSrcID );
	nCodeLength += sTempLength;

	sTempLength = EncodeShort( &pcTemp, (unsigned short)m_nDstFE );
	nCodeLength += sTempLength;

	sTempLength = EncodeChar( &pcTemp, (unsigned char)m_byTransferType );
	nCodeLength += sTempLength;

	switch(m_byTransferType)
	{
		case trans_p2p:
		{
			sTempLength = EncodeShort( &pcTemp, (unsigned short)m_stTransferInfo.nDstID );
			nCodeLength += sTempLength;
			break;
		}
		case trans_p2g:
		{
			int i;
			sTempLength = EncodeShort( &pcTemp, (unsigned short)m_stTransferInfo.stSvrs.nFENumber );
			nCodeLength += sTempLength;

			for( i = 0; i < m_stTransferInfo.stSvrs.nFENumber; i++ )
			{
				sTempLength = EncodeShort( &pcTemp, (unsigned short)m_stTransferInfo.stSvrs.anFEs[i] );
				nCodeLength += sTempLength;
			}
			break;
		}
		case trans_broadcast:
		{
			break;
		}
		case trans_bykey:
		{
			sTempLength = EncodeInt( &pcTemp, m_stTransferInfo.Uin );
			nCodeLength += sTempLength;
			break;
		}
		default:
		{
			return -1;
		}
	}

	sTempLength = EncodeChar( &pcTemp, (unsigned char)m_byOpFlag );
	nCodeLength += sTempLength;

	if( m_nOptLength > sizeof(m_abyOpt) )
	{
		m_nOptLength = sizeof(m_abyOpt);
	}
	sTempLength = EncodeShort( &pcTemp, (unsigned short)m_nOptLength );
	nCodeLength += sTempLength;

	if( m_nOptLength > 0 )
	{
		sTempLength = EncodeMem( &pcTemp, (char *)m_abyOpt, m_nOptLength );
		nCodeLength += sTempLength;
	}

	sTempLength = EncodeShort( &pcTemp, (unsigned short)m_nBodyLength );
	nCodeLength += sTempLength;

	return 0;
}
int CProxyHead::Decode(char *pCode, short nCodeLength)
{
	char *pcTemp;
	short sLeftLength;
	short sTempLength;

	if( !pCode || nCodeLength <= 0 )
	{
		return -1;
	}


	pcTemp = (char *)pCode;
	sLeftLength = nCodeLength;

	sTempLength = DecodeShort( &pcTemp, (unsigned short *)&m_nTotalLength );
	sLeftLength -= sTempLength;

	sTempLength = DecodeShort( &pcTemp, (unsigned short *)&m_nSrcFE );
	sLeftLength -= sTempLength;

	sTempLength = DecodeShort( &pcTemp, (unsigned short *)&m_nSrcID );
	sLeftLength -= sTempLength;

	sTempLength = DecodeShort( &pcTemp, (unsigned short *)&m_nDstFE );
	sLeftLength -= sTempLength;

	sTempLength = DecodeChar( &pcTemp, (unsigned char *)&m_byTransferType );
	sLeftLength -= sTempLength;

	switch( m_byTransferType )
	{
		case trans_p2p:
		{
			sTempLength = DecodeShort( &pcTemp, (unsigned short *)&m_stTransferInfo.nDstID );
			sLeftLength -= sTempLength;
			break;
		}
		case trans_p2g:
		{
			int i;

			sTempLength = DecodeShort( &pcTemp, (unsigned short *)&m_stTransferInfo.stSvrs.nFENumber );
			sLeftLength -= sTempLength;
			
			for( i = 0; i < m_stTransferInfo.stSvrs.nFENumber; i++ )
			{
				sTempLength = DecodeShort( &pcTemp, (unsigned short *)&m_stTransferInfo.stSvrs.anFEs[i] );
				sLeftLength -= sTempLength;
			}
			break;
		}
		case trans_broadcast:
		{
			break;
		}
		case trans_bykey:
		{
			sTempLength = DecodeInt( &pcTemp, &m_stTransferInfo.Uin );
			sLeftLength -= sTempLength;
			break;
		}
		default:
		{
			return -1;
		}
	}


	sTempLength = DecodeChar( &pcTemp, (unsigned char *)&m_byOpFlag );
	sLeftLength -= sTempLength;

	sTempLength = DecodeShort( &pcTemp, (unsigned short *)&m_nOptLength );
	sLeftLength -= sTempLength;
	if(m_nOptLength < 0 || sLeftLength < m_nOptLength + sizeof(m_nBodyLength))
	{
		LOG_ERROR("default", "Proxyhead Optlenth  :%d , CodeLeftlen %d, Error", m_nOptLength, sLeftLength);
		return -1;
	}
	if( m_nOptLength > sizeof(m_abyOpt) )
	{
		m_nOptLength = sizeof(m_abyOpt);
	}

	if( m_nOptLength > 0 )
	{
		sTempLength = DecodeMem( &pcTemp, (char *)m_abyOpt, m_nOptLength );
		sLeftLength -= sTempLength;
	}

	sTempLength = DecodeShort( &pcTemp, (unsigned short *)&m_nBodyLength );
	sLeftLength -= sTempLength;
	if(m_nBodyLength < 0 || sLeftLength < 0)
	{
		LOG_ERROR("default", "Proxyhead BodyLen  :%d , CodeLeftlen %d, Error", m_nBodyLength, sLeftLength);
		return -1;
	}

	/*if( sLeftLength < 0 )
	{
		return -1;
	}*/

	return 0;
}


