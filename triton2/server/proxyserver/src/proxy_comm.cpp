#include "proxy_comm.h"
#include <stdlib.h>

CFlag::CFlag()
{
	m_byFlag = (BYTE)0;
}

CFlag::~CFlag()
{
}

int CFlag::Initial( BYTE byExFlag )
{
	m_byFlag = byExFlag;
	return 0;
}
unsigned char CFlag::GetAllFlag()
{
	return m_byFlag;
}
int CFlag::ClearFlag(BYTE byExFlag)
{
	BYTE byTemp;
	
	byTemp = byExFlag ^ 0xff;
	m_byFlag = m_byFlag & byTemp;

	return 0;
}

int CFlag::SetFlag( BYTE byExFlag )
{
	m_byFlag = m_byFlag | byExFlag;
	return 0;
}

int CFlag::IsFlagSet(BYTE byExFlag)
{
	if( byExFlag & m_byFlag )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int SplitMapStr(const char *pMapStr, int iMaxItemNum, short pItemArray[])
{
	char szTempStr[1024];
	char *pTemp1 = NULL;
	char *pTemp2 = NULL;
	char *pTemp3 = NULL;
	int iStart;
	int iStop, i;
	int iTempCount = 0;
	int iLength;

	if(!pMapStr  || iMaxItemNum < 0)
	{
		return -1;
	}
	memset(szTempStr, '\0', sizeof(szTempStr));
	snprintf(szTempStr, sizeof(szTempStr)-1,"%s",  pMapStr);
	pTemp1 = &szTempStr[0];
	if(strchr(pTemp1, ' '))
	{
		LOG_ERROR("default", "Error in put map str, contained tab space\n");
		return -1;
	}
	iLength = strlen(pTemp1);
	for(i = 0; i < iLength; i++)
	{
		if((pTemp1[i] > '9' || pTemp1[i] < '0') && pTemp1[i] != ',' && pTemp1[i] != '-')
		{
			LOG_ERROR("default", "Error in put map str, contained error char\n");
			return -1;
		}
	}
	while(pTemp2 = strchr(pTemp1, ','))
	{
		if(iTempCount == iMaxItemNum)
		{
			return iTempCount;
		}
		*pTemp2 = '\0';
		if(strlen(pTemp1)  ==  0)
		{
			LOG_ERROR("default", "Error:Split Map Str Failed, str in null or space\n");
			return -1;
		}

		if(pTemp3 = strchr(pTemp1, '-'))
		{
			*pTemp3 = '\0';
			iStart = atoi(pTemp1);
			iStop = atoi(pTemp3 + 1);
			if(iStop < iStart)
			{
				LOG_ERROR("default", "Error:Split Map Str Failed, Range start big(%d) than stop(%d)\n", iStart, iStop);
				return -1;
			}

			for(i = iStart; i <= iStop && iTempCount < iMaxItemNum; i++)
			{
				pItemArray[iTempCount] =  i;
				//LOG("Item[%d]= %d\n", iTempCount, i);
				iTempCount ++;

			}
		}
		else
		{
			pItemArray[iTempCount] =  atoi(pTemp1);
			//LOG("Item[%d]= %d\n", iTempCount, pItemArray[iTempCount] );
			iTempCount ++;

		}
		pTemp1 = pTemp2 + 1;
	}
	if(pTemp1 && iTempCount < iMaxItemNum)
	{
		if(pTemp3 = strchr(pTemp1, '-'))
		{
			*pTemp3 = '\0';
			iStart = atoi(pTemp1);
			iStop = atoi(pTemp3 + 1);
			if(iStop < iStart)
			{
				LOG_ERROR("default", "Error:Split Map Str Failed, Range start big(%d) than stop(%d)\n", iStart, iStop);
				return -1;
			}

			for(i = iStart; i <= iStop && iTempCount < iMaxItemNum; i++)
			{
				pItemArray[iTempCount] =  i;
				//LOG("Item[%d]= %d\n", iTempCount, i);
				iTempCount ++;
			}
		}
		else
		{
			pItemArray[iTempCount] =  atoi(pTemp1);
			//LOG("Item[%d]= %d\n", iTempCount, pItemArray[iTempCount] );
			iTempCount ++;

		}
	}
	return iTempCount;
}

