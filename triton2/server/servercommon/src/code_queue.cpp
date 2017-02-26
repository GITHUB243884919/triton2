#include "code_queue.h"
#include <stdio.h>
#include <string.h>

CSharedMem* CCodeQueue::pCurrentShm = NULL;


size_t CCodeQueue::CountQueueSize(int iBufSize)
{
	size_t iTempSize = 0;

	iTempSize += sizeof(CCodeQueue);
	if( iBufSize > 0 )
	{
		iTempSize += iBufSize;
	}

	return iTempSize;
}

void* CCodeQueue::operator new(size_t nSize)
{
	BYTE* pTemp;

	if( !pCurrentShm )
	{
		return (void *)NULL;
	}

	pTemp = (BYTE *)(pCurrentShm->CreateSegment(nSize));
	
	return (void *)pTemp;
}

void CCodeQueue::operator delete(void *pBase)
{
}


void* CCodeQueue::operator new(size_t size, const char* file, int line)
{
	return operator new(size);
}
void CCodeQueue::operator delete(void* pointer, const char* file, int line)
{

}


CCodeQueue::CCodeQueue()
{
	m_stQueueHead.m_nBegin = 0;
	m_stQueueHead.m_nEnd = 0;
	m_stQueueHead.m_nSize = 0;
	m_stQueueHead.m_iCodeBufOffSet = -1;
	m_stQueueHead.m_iLockIdx = -1;
}

CCodeQueue::CCodeQueue(int nTotalSize, int iLockIdx /* = -1  */)
{
	if( !pCurrentShm )
	{
		m_stQueueHead.m_nBegin = 0;
		m_stQueueHead.m_nEnd = 0;
		m_stQueueHead.m_nSize = 0;
		m_stQueueHead.m_iCodeBufOffSet = -1;
		m_stQueueHead.m_iLockIdx = -1;
		return;
	}

	if( pCurrentShm->GetInitMode() == SHM_INIT )
	{
		m_stQueueHead.m_iLockIdx = iLockIdx;
		Initialize( nTotalSize );
	}
	else
	{
		Resume( nTotalSize );
	}
}

CCodeQueue::~CCodeQueue()
{
}

int CCodeQueue::Initialize( int nTotalSize )
{
	BYTE *pbyCodeBuf;
	
	m_stQueueHead.m_nSize = nTotalSize;
	m_stQueueHead.m_nBegin = 0;
	m_stQueueHead.m_nEnd = 0;

	pbyCodeBuf = (BYTE *)(pCurrentShm->CreateSegment((size_t)nTotalSize));

	if( !pbyCodeBuf )
	{
		m_stQueueHead.m_iCodeBufOffSet = -1;
		return -1;
	}

	m_stQueueHead.m_iCodeBufOffSet = (int)((BYTE *)pbyCodeBuf - (BYTE *)this); 

	return 0;
}

int CCodeQueue::Resume(int nTotalSize)
{
	if( !pCurrentShm )
	{
		return -1;
	}

	pCurrentShm->CreateSegment((size_t)nTotalSize);

	return 0;
}

int CCodeQueue::GetCriticalData(int *piBeginIdx, int *piEndIdx)
{
	if( piBeginIdx )
	{
		*piBeginIdx = m_stQueueHead.m_nBegin;
	}
	if( piEndIdx )
	{
		*piEndIdx = m_stQueueHead.m_nEnd;
	}

	return 0;
}

int CCodeQueue::SetCriticalData(int iBeginIdx, int iEndIdx)
{
	if( iBeginIdx >= 0 && iBeginIdx < m_stQueueHead.m_nSize )
	{
		m_stQueueHead.m_nBegin = iBeginIdx;
	}
	if( iEndIdx >= 0 && iEndIdx < m_stQueueHead.m_nSize )
	{
		m_stQueueHead.m_nEnd = iEndIdx;
	}

	return 0;
}

int CCodeQueue::IsQueueFull()
{
	int nTempMaxLength = 0;
	int nTempBegin = -1;
	int nTempEnd = -1;

	
	GetCriticalData( &nTempBegin, &nTempEnd );
	if( nTempBegin == nTempEnd )
	{
		nTempMaxLength = m_stQueueHead.m_nSize;
	}
	else if( nTempBegin > nTempEnd )
	{
		nTempMaxLength = nTempBegin - nTempEnd;		
	}
	else
	{
		nTempMaxLength = (m_stQueueHead.m_nSize - nTempEnd) + nTempBegin;
	}

	//重要：最大长度应该减去预留部分长度，保证首尾不会相接
	nTempMaxLength -= QUEUERESERVELENGTH;

	if( nTempMaxLength > 0 )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

//Error code: -1 invalid para; -2 not enough; -3 data crashed
//This function only set the end idx of code queue
int CCodeQueue::AppendOneCode(const BYTE *pInCode, int sInLength)
{
	int nTempMaxLength = 0;
	int nTempRt = -1;
	int nTempBegin = -1;
	int nTempEnd = -1;
	//char pcTempBuf[8192];
	BYTE *pbyCodeBuf;
	
	BYTE *pTempSrc = NULL;
	BYTE *pTempDst = NULL;
	unsigned int i;

	if( !pInCode || sInLength <= 0 )
	{
		return -1;
	}
	if( m_stQueueHead.m_iCodeBufOffSet <= 0 || m_stQueueHead.m_nSize <= 0 )
	{
		return -1;
	}
	pbyCodeBuf = (BYTE *)((BYTE *)this + m_stQueueHead.m_iCodeBufOffSet);

	//首先判断是否队列已满
	if( IsQueueFull() )		//if( m_stQueueHead.m_nFullFlag )
	{
		return -2;
	}

	if( GetCriticalData(&nTempBegin, &nTempEnd) )
	{
		return -3;
	}


	if( nTempBegin < 0 || nTempBegin >= m_stQueueHead.m_nSize
		|| nTempEnd < 0 || nTempEnd >= m_stQueueHead.m_nSize )
	{
		/*LOG("In CCodeQueue::AppendOneCode, data crashed: begin = %d, end = %d",
						nTempBegin, nTempEnd);*/
		
		nTempEnd = nTempBegin;
		SetCriticalData( -1, nTempEnd );

		return -3;
	}

	if( nTempBegin == nTempEnd )
	{
		nTempMaxLength = m_stQueueHead.m_nSize;
	}
	else if( nTempBegin > nTempEnd )
	{
		nTempMaxLength = nTempBegin - nTempEnd;		
	}
	else
	{
		nTempMaxLength = (m_stQueueHead.m_nSize - nTempEnd) + nTempBegin;
	}

	//重要：最大长度应该减去预留部分长度，保证首尾不会相接
	nTempMaxLength -= QUEUERESERVELENGTH;

	nTempRt = nTempEnd;
	
	if( (int)(sInLength + sizeof(short)) > nTempMaxLength || sInLength > 0xFFFF)
	{
		return -2;
	}


	unsigned short usInLength = (unsigned short) sInLength;

	pTempDst = &pbyCodeBuf[0];
	pTempSrc = (BYTE *)&usInLength;

	for( i = 0; i < sizeof(usInLength); i++ )
	{
		pTempDst[nTempEnd] = pTempSrc[i];
		nTempEnd = (nTempEnd+1) % m_stQueueHead.m_nSize;
	}

	if( nTempBegin > nTempEnd )
	{
		memcpy((void *)&pbyCodeBuf[nTempEnd], (const void *)pInCode, (size_t)sInLength );
	}
	else
	{
		if( (int)sInLength > (m_stQueueHead.m_nSize - nTempEnd) )
		{
			memcpy((void *)&pbyCodeBuf[nTempEnd], (const void *)&pInCode[0], (size_t)(m_stQueueHead.m_nSize - nTempEnd) );
			memcpy((void *)&pbyCodeBuf[0],(const void *)&pInCode[(m_stQueueHead.m_nSize - nTempEnd)], (size_t)(sInLength + nTempEnd - m_stQueueHead.m_nSize) );
		}
		else
		{
			memcpy((void *)&pbyCodeBuf[nTempEnd], (const void *)&pInCode[0], (size_t)sInLength);
		}
	}
	nTempEnd = (nTempEnd + sInLength) % m_stQueueHead.m_nSize;

	SetCriticalData( -1, nTempEnd );

	return nTempRt;
}

int CCodeQueue::PeekHeadCode(BYTE *pOutCode, int *psOutLength)
{
	int nTempMaxLength = 0;
	int nTempRet = -1;
	int nTempBegin = -1;
	int nTempEnd = -1;
	BYTE *pTempSrc;
	BYTE *pTempDst;
	unsigned int i;
	BYTE *pbyCodeBuf;

	if( !pOutCode || !psOutLength )
	{
		return -1;
	}
	if( m_stQueueHead.m_iCodeBufOffSet <= 0 || m_stQueueHead.m_nSize <= 0 )
	{
		return -1;
	}
	
	pbyCodeBuf = (BYTE *)((BYTE *)this + m_stQueueHead.m_iCodeBufOffSet);
	
	if( GetCriticalData(&nTempBegin, &nTempEnd) )
	{
		return -1;
	}
	nTempRet = nTempBegin;

	if( nTempBegin == nTempEnd )
	{
		*psOutLength = 0;
		return nTempRet;
	}

	if( nTempBegin < nTempEnd )
	{
		nTempMaxLength = nTempEnd - nTempBegin;
	}
	else
	{
		nTempMaxLength = m_stQueueHead.m_nSize - nTempBegin + nTempEnd;
	}

	if( nTempMaxLength < (int)sizeof(short) )
	{
		*psOutLength = 0;
		return -3;
	}

	unsigned short usOutLength = 0;

	pTempDst = (BYTE *)&usOutLength;
	pTempSrc = (BYTE *)&pbyCodeBuf[0];
	for( i = 0; i < sizeof(short); i++ )
	{
		pTempDst[i] = pTempSrc[nTempBegin];
		nTempBegin = (nTempBegin+1) % m_stQueueHead.m_nSize; 
	}

	*psOutLength = usOutLength;

	if( (*psOutLength) > (int)(nTempMaxLength - sizeof(short)) )
	{
		*psOutLength = 0;
		return -3;
	}

	pTempDst = (BYTE *)&pOutCode[0];

	if( nTempBegin < nTempEnd )
	{
		memcpy((void *)pTempDst, (const void *)&pTempSrc[nTempBegin], (size_t)(*psOutLength));
	}
	else
	{
		//如果出现分片，则分段拷贝
		if( m_stQueueHead.m_nSize - nTempBegin < *psOutLength)
		{
			memcpy((void *)pTempDst, (const void *)&pTempSrc[nTempBegin], (size_t)(m_stQueueHead.m_nSize - nTempBegin));
			pTempDst += (m_stQueueHead.m_nSize - nTempBegin);
			memcpy((void *)pTempDst, (const void *)&pTempSrc[0], (size_t)(*psOutLength+nTempBegin-m_stQueueHead.m_nSize));
		}
		else	//否则，直接拷贝
		{
			memcpy((void *)pTempDst, (const void *)&pTempSrc[nTempBegin], (size_t)(*psOutLength));
		}
	}
	nTempBegin = (nTempBegin + (*psOutLength)) % m_stQueueHead.m_nSize;

	return nTempRet;
}

//This function just change the begin idx of code queue
int CCodeQueue::GetHeadCode(BYTE *pOutCode, int *psOutLength)
{
	int nTempMaxLength = 0;
	int nTempRet = -1;
	int nTempBegin = -1;
	int nTempEnd = -1;
	BYTE *pTempSrc;
	BYTE *pTempDst;
	unsigned int i;
	BYTE *pbyCodeBuf;

	if( !pOutCode || !psOutLength )
	{
		return -1;
	}
	if( m_stQueueHead.m_iCodeBufOffSet <= 0 || m_stQueueHead.m_nSize <= 0 )
	{
		return -2;
	}

	pbyCodeBuf = (BYTE *)((BYTE *)this + m_stQueueHead.m_iCodeBufOffSet);
	
	if( GetCriticalData(&nTempBegin, &nTempEnd) )
	{
		return -3;
	}
	nTempRet = nTempBegin;

	if( nTempBegin == nTempEnd )
	{
		*psOutLength = 0;
		return nTempRet;
	}


	if( nTempBegin < nTempEnd )
	{
		nTempMaxLength = nTempEnd - nTempBegin;
	}
	else
	{
		nTempMaxLength = m_stQueueHead.m_nSize - nTempBegin + nTempEnd;
	}

	if( nTempMaxLength < (int)sizeof(short) )
	{
		*psOutLength = 0;
		nTempBegin = nTempEnd;
		SetCriticalData(nTempBegin, -1);
		return -4;
	}

	unsigned short usOutLength;
	pTempDst = (BYTE *)&usOutLength;
	pTempSrc = (BYTE *)&pbyCodeBuf[0];
	for( i = 0; i < sizeof(short); i++ )
	{
		pTempDst[i] = pTempSrc[nTempBegin];
		nTempBegin = (nTempBegin+1) % m_stQueueHead.m_nSize; 
	}

	*psOutLength = usOutLength;

	if( (*psOutLength) > (int)(nTempMaxLength - sizeof(short)) || *psOutLength < 0 )
	{
		*psOutLength = 0;
		nTempBegin = nTempEnd;
		SetCriticalData(nTempBegin, -1);
		return -5;
	}

	pTempDst = (BYTE *)&pOutCode[0];

	if( nTempBegin < nTempEnd )
	{
		memcpy((void *)pTempDst, (const void *)&pTempSrc[nTempBegin], (size_t)(*psOutLength));
	}
	else
	{
		//如果出现分片，则分段拷贝
		if( m_stQueueHead.m_nSize - nTempBegin < *psOutLength)
		{
			memcpy((void *)pTempDst, (const void *)&pTempSrc[nTempBegin], (size_t)(m_stQueueHead.m_nSize - nTempBegin));
			pTempDst += (m_stQueueHead.m_nSize - nTempBegin);
			memcpy((void *)pTempDst, (const void *)&pTempSrc[0], (size_t)(*psOutLength+nTempBegin-m_stQueueHead.m_nSize));
		}
		else	//否则，直接拷贝
		{
			memcpy((void *)pTempDst, (const void *)&pTempSrc[nTempBegin], (size_t)(*psOutLength));
		}
	}
	nTempBegin = (nTempBegin + (*psOutLength)) % m_stQueueHead.m_nSize;

	SetCriticalData(nTempBegin, -1);

	return nTempRet;
}

//This function just change the begin idx of code queue
int CCodeQueue::DeleteHeadCode()
{
	short sTempShort = 0;
	int nTempMaxLength = 0;
	int nTempRet = -1;
	int nTempBegin = -1;
	int nTempEnd = -1;

	BYTE *pTempSrc;
	BYTE *pTempDst;
	BYTE *pbyCodeBuf;

	if( m_stQueueHead.m_iCodeBufOffSet <= 0 || m_stQueueHead.m_nSize <= 0 )
	{
		return -1;
	}

	pbyCodeBuf = (BYTE *)((BYTE *)this + m_stQueueHead.m_iCodeBufOffSet);
	
	if( GetCriticalData(&nTempBegin, &nTempEnd) )
	{
		return -1;
	}
	nTempRet = nTempBegin;

	if( nTempBegin == nTempEnd )
	{
		return nTempRet;
	}

	if( nTempBegin < nTempEnd )
	{
		nTempMaxLength = nTempEnd - nTempBegin;
	}
	else
	{
		nTempMaxLength = m_stQueueHead.m_nSize - nTempBegin + nTempEnd;
	}
	
	if( nTempMaxLength < ( int )sizeof(short) )
	{
		nTempBegin = nTempEnd;
		SetCriticalData(nTempBegin, -1);
		return -3;
	}

	pTempDst = (BYTE *)&sTempShort;
	pTempSrc = (BYTE *)&pbyCodeBuf[0];
	for( unsigned int i = 0; i < sizeof(short); i++ )
	{
		pTempDst[i] = pTempSrc[nTempBegin];
		nTempBegin = (nTempBegin+1) % m_stQueueHead.m_nSize; 
	}

	if( sTempShort > (int)(nTempMaxLength - sizeof(short)) || sTempShort < 0 )
	{
		nTempBegin = nTempEnd;
		SetCriticalData(nTempBegin, -1);
		return -3;
	}

	nTempBegin = (nTempBegin + sTempShort) % m_stQueueHead.m_nSize;

	SetCriticalData(nTempBegin, -1);
	
	return nTempBegin;
}

int CCodeQueue::GetOneCode(int iCodeOffset, int nCodeLength, BYTE *pOutCode, int *psOutLength)
{
	short nTempShort = 0;
	int iTempMaxLength = 0;
	int iTempBegin;
	int iTempEnd;
	int iTempIdx;
	BYTE *pTempSrc;
	BYTE *pTempDst;
	BYTE *pbyCodeBuf;

	if( m_stQueueHead.m_iCodeBufOffSet <= 0 || m_stQueueHead.m_nSize <= 0 )
	{
		return -1;
	}

	pbyCodeBuf = (BYTE *)((BYTE *)this + m_stQueueHead.m_iCodeBufOffSet);

	if( !pOutCode || !psOutLength )
	{
		//LOG("In GetOneCode, invalid input paraments.\n");
		return -1;
	}

	if( iCodeOffset < 0 || iCodeOffset >= m_stQueueHead.m_nSize)
	{
		/*LOG("In GetOneCode, invalid code offset %d.\n", iCodeOffset);*/
		return -1;
	}
	if( nCodeLength < 0 || nCodeLength >= m_stQueueHead.m_nSize )
	{
		/*LOG("In GetOneCode, invalid code length %d.\n", nCodeLength);*/
		return -1;
	}

	if( GetCriticalData(&iTempBegin, &iTempEnd) )
	{
		return -1;
	}
	
	if( iTempBegin == iTempEnd )
	{
		*psOutLength = 0;
		return 0;
	}

	if( iTempBegin == iCodeOffset )
	{
		return GetHeadCode(pOutCode, psOutLength);
	}

	/*LOG("Warning: Get code is not the first one, there might be sth wrong.\n");*/

	if( iCodeOffset < iTempBegin || iCodeOffset >= iTempEnd )
	{
		/*LOG("In GetOneCode, code offset out of range.\n");*/
		*psOutLength = 0;
		return -1;
	}
	
	if( iTempBegin < iTempEnd )
	{
		iTempMaxLength = iTempEnd - iTempBegin;		
	}
	else
	{
		iTempMaxLength = m_stQueueHead.m_nSize - iTempBegin + iTempEnd;
	}

	if( iTempMaxLength < ( int )sizeof(short) )
	{
		*psOutLength = 0;
		iTempBegin = iTempEnd;
		SetCriticalData(iTempBegin, -1);
		return -3;
	}

	pTempDst = (BYTE *)&nTempShort;
	pTempSrc = (BYTE *)&pbyCodeBuf[0];
	iTempIdx = iCodeOffset;
	for( unsigned int i = 0; i < sizeof(short); i++ )
	{
		pTempDst[i] = pTempSrc[iTempIdx];
		iTempIdx = (iTempIdx+1) % m_stQueueHead.m_nSize; 
	}

	if( nTempShort > (int)(iTempMaxLength - sizeof(short)) || nTempShort < 0 || nTempShort != nCodeLength )
	{
		/*LOG("Can't get code, code length not matched.\n");*/
		*psOutLength = 0;
		return -2;
	}

	SetCriticalData(iCodeOffset, -1);
	
	return GetHeadCode( pOutCode, psOutLength );
}

int CCodeQueue::IsQueueEmpty()
{
	int iTempBegin;
	int iTempEnd;

	GetCriticalData(&iTempBegin, &iTempEnd);

	if( iTempBegin == iTempEnd)
	{
		return 1;
	}

	return 0;
}

void CCodeQueue::GetCriticalData(int& iBegin, int& iEnd, int& iLeft)
{
	iBegin = m_stQueueHead.m_nBegin;
	iEnd = m_stQueueHead.m_nEnd;

	int iTempMaxLength = 0;
	
	if( iBegin == iEnd )
	{
		iTempMaxLength = m_stQueueHead.m_nSize;
	}
	else if( iBegin > iEnd )
	{
		iTempMaxLength = iBegin - iEnd;		
	}
	else
	{
		iTempMaxLength = (m_stQueueHead.m_nSize - iEnd) + iBegin;
	}

	//重要：最大长度应该减去预留部分长度，保证首尾不会相接
	iTempMaxLength -= QUEUERESERVELENGTH;

	iLeft = iTempMaxLength;
	
}

int CCodeQueue::DumpToFile(const char *szFileName)
{
	if( !szFileName || !m_pbyCodeBuffer )
	{
		return -1;
	}

	FILE *fpDumpFile = fopen(szFileName, "w");
	int iPageSize = 4096, iPageCount = 0, i;
	BYTE *pPage = m_pbyCodeBuffer;

	if( !fpDumpFile )
	{
		return -1;
	}

	fwrite((const void *)&m_stQueueHead, sizeof(m_stQueueHead), 1, fpDumpFile);
	iPageCount = m_stQueueHead.m_nSize/iPageSize;
	for( i = 0; i < iPageCount; i++ )
	{
		fwrite((const void *)pPage, iPageSize, 1, fpDumpFile);
		pPage += iPageSize;
	}
	fwrite((const void *)pPage, m_stQueueHead.m_nSize - iPageSize*iPageCount, 1, fpDumpFile);

	fclose(fpDumpFile);

	return 0;                                                                                                     
}                                                                                                                 

int CCodeQueue::LoadFromFile(const char *szFileName)                                                              
{                                                                                                                 
	if( !szFileName || !m_pbyCodeBuffer )                                                                         
	{                                                                                                             
		return -1;                                                                                                
	}                                                                                                             

	FILE *fpDumpFile = fopen(szFileName, "r");                                                                    
	int iPageSize = 4096, iPageCount = 0, i;                                                                      
	BYTE *pPage = m_pbyCodeBuffer;                                                                                

	if( !fpDumpFile )                                                                                             
	{                                                                                                             
		return -1;                                                                                                
	}                                                                                                             

	fread((void *)&m_stQueueHead, sizeof(m_stQueueHead), 1, fpDumpFile);                                          
	iPageCount = m_stQueueHead.m_nSize/iPageSize;                                                                 
	for( i = 0; i < iPageCount; i++ )                                                                             
	{                                                                                                             
		fread((void *)pPage, iPageSize, 1, fpDumpFile);                                                           
		pPage += iPageSize;                                                                                       
	}
	fread((void *)pPage, m_stQueueHead.m_nSize - iPageSize*iPageCount, 1, fpDumpFile);                            

	fclose(fpDumpFile);                                                                                           

	return 0;   
}

int CCodeQueue::CleanQueue()                                                                                      
{
	m_stQueueHead.m_nBegin = 0;                                                                                   
	m_stQueueHead.m_nEnd = 0;                                                                                     

	return 0;
}

