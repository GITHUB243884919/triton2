#ifndef _PROXY_HEAD_HPP_
#define _PROXY_HEAD_HPP_

#include "base.h"

#define MAXSVRNUMBER 256


#define BASEPXYHEADLENGTH			14
#define PROXY_RESERVE				((BYTE)0x01)
#define PROXY_CMD					((BYTE)0x02)



#define MAXPROXYOPTLENGTH			256
enum enProxyCmds
{
	CMD_NULL	= 0,
	CMD_REGKEY	= 1,
	CMD_START	= 2, 
	CMD_REGSERVER = 3,               //服务器注册
	CMD_KEEPALIVE = 4,                 //心跳
};


/*以下定义转发类型*/
enum eTransTypes
{
	trans_p2p = 0,			//点到点
	trans_p2g,				//点到组
	trans_broadcast,		//同型广播
	trans_bykey				//根据关键字转发
};


class CProxyHead
{
public:
	CProxyHead();
	~CProxyHead();
	int Encode(char *pCode, short & nCodeLength);
	int Decode(char *pCode, short nCodeLength);
	void Print();
		
	short m_nTotalLength;
	short m_nSrcFE;
	short m_nSrcID;
	short m_nDstFE;
	BYTE m_byTransferType;
	union _U
	{
		unsigned int Uin;
		short nDstID;
		struct _tagSvrs
		{
			short nFENumber;
			short anFEs[MAXSVRNUMBER];
		} stSvrs;
	} m_stTransferInfo;
	BYTE m_byOpFlag;
	short m_nOptLength;
	BYTE m_abyOpt[MAXPROXYOPTLENGTH];
	short m_nBodyLength;
};

#endif
