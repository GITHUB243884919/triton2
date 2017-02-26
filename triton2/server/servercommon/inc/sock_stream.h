#ifndef _SOCK_STREAM_H_
#define _SOCK_STREAM_H_

#include "base.h"

class CSockStream
{
public:
	CSockStream();
	~CSockStream();

	enum _enParas
	{
		RCVBUFSIZE	= 1024000 * 8,
		SNDBUFSIZE	= 1024000 * 8,
		STREAM_CLOSED = 0,
		STREAM_OPENED = 1,
		ERR_STRM_NOSOCK = -1,
		ERR_STRM_AGAIN	= -2,
		ERR_STRM_PEER	= -3,
		ERR_STRM_FAILED	= -4,
	};

	int SetUpStream(int iFD);
	int GetSocketFD();
	int GetStatus();

	int RecvData();
	int GetOneCode(short &nCodeLength, BYTE *pCode);
	int SendOneCode(short nCodeLength, BYTE *pCode);
	int AddToCheckSet(fd_set *pCheckSet);
	int IsFDSetted(fd_set *pCheckSet);

protected:
	int CloseStream();

	int m_iSocketFD;					//SocketÃèÊö×Ó
	int m_iStatus;						//Á÷×´Ì¬

private:
	int  m_iReadBegin;
	int  m_iReadEnd;
	BYTE m_abyRecvBuffer[RCVBUFSIZE];

	int  m_iPostBegin;
	int  m_iPostEnd;
	BYTE m_abyPostBuffer[SNDBUFSIZE];
};


#endif

