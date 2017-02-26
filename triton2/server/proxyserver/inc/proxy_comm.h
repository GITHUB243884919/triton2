#ifndef _PROXY_COMM_H_
#define _PROXY_COMM_H_

#include "base.h"
#include "log.h"

class CFlag
{
	public:
		CFlag();
		~CFlag();
		int Initial( BYTE byExFlag );
		BYTE GetAllFlag();
		int ClearFlag( BYTE byExFlag );
		int SetFlag( BYTE byExFlag );
		int IsFlagSet( BYTE byExFlag );
	private:
		BYTE m_byFlag;
};

int SplitMapStr(const char *pMapStr, int iMaxItemNum, short pItemArray[]);

#endif
