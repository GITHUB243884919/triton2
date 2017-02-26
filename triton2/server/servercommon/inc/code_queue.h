#ifndef _CODE_QUEUE_H_
#define _CODE_QUEUE_H_

#include "shm.h"

#define QUEUERESERVELENGTH		8	//预留部分的长度

class CCodeQueue
{
public:
	CCodeQueue();
	CCodeQueue( int nTotalSize, int iLockIdx = -1 );
	~CCodeQueue();

	void* operator new( size_t nSize );
	void  operator delete( void *pBase );
	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);


	int Initialize( int nTotalSize );
	int Resume( int nTotalSize );
	int AppendOneCode(const BYTE *pInCode, int sInLength);
	int GetHeadCode(BYTE *pOutCode, int *psOutLength);
	int PeekHeadCode(BYTE *pOutCode, int *psOutLength);
	int DeleteHeadCode();
	int GetOneCode(int iCodeOffset, int nCodeLength, BYTE *pOutCode, int *psOutLength);
	int IsQueueEmpty();

	int DumpToFile(const char *szFileName);
	int LoadFromFile(const char *szFileName);
	int CleanQueue();


	static size_t CountQueueSize(int iBufSize);
	static CSharedMem *pCurrentShm;


	void GetCriticalData(int& iBegin, int& iEnd, int& iLeft);

protected:
	
private:
	int IsQueueFull();
	int SetFullFlag( int iFullFlag );

	int GetCriticalData(int *piBeginIdx, int *piEndIdx);
	int SetCriticalData(int iBeginIdx, int iEndIdx);

	struct _tagHead
	{
		int m_nSize;
		int m_iCodeBufOffSet;
		int m_nBegin;
		int m_nEnd;
		int m_iLockIdx;
	} m_stQueueHead;

	BYTE* m_pbyCodeBuffer;
};

#endif
