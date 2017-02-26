#ifndef _NET_HEAD_H_
#define _NET_HEAD_H_

class CNetHead
{
public:
	CNetHead();
	~CNetHead();

	int  Encode( char *pcOutBuffer, short &sOutLength );
	int  Decode( const char *pcInBuffer, const short sInLength );
	void Print();

	unsigned int m_iSrcIP;
	unsigned int m_iDstIP;
	unsigned short m_nSrcPort;
	unsigned short m_nDstPort;
	time_t m_tStamp;
};


#endif
