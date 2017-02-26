#include "base.h"
#include "file_lock.h"


CFileLock::CFileLock()
{
	int i;

	for( i = 0; i < MAX_FILE_LOCK; i++ )
	{
		m_aiLockFDs[i] = -1;
	}
}

CFileLock::~CFileLock()
{
}

int CFileLock::SetLock(int iLockIdx, const char *szFileName)
{
	FILE *fp;
	int  iTempFD;

	if( !szFileName || iLockIdx < 0 || iLockIdx >= MAX_FILE_LOCK )
	{
		return -1;
	}

	fp = fopen(szFileName, "r+");

	if( !fp )
	{
		return -2;
	}

	iTempFD = fileno( fp );

	if( iTempFD < 0)
	{
		return -3;
	}

	m_aiLockFDs[iLockIdx] = iTempFD;

	return 0;

}

int CFileLock::RLockWait(int iLockIdx)
{
	int iTempRet;

	if( iLockIdx < 0 || iLockIdx >= MAX_FILE_LOCK || m_aiLockFDs[iLockIdx] < 0 )
	{
		return 0;
	}

	iTempRet = flock(m_aiLockFDs[iLockIdx], LOCK_SH);

	return iTempRet;
}

int CFileLock::WLockWait(int iLockIdx)
{
	int iTempRet;

	if( iLockIdx < 0 || iLockIdx >= MAX_FILE_LOCK || m_aiLockFDs[iLockIdx] < 0 )
	{
		return 0;
	}

	iTempRet = flock(m_aiLockFDs[iLockIdx], LOCK_EX);

	return iTempRet;
}

int CFileLock::UnLock(int iLockIdx)
{
	int iTempRet;

	if( iLockIdx < 0 || iLockIdx >= MAX_FILE_LOCK || m_aiLockFDs[iLockIdx] < 0 )
	{
		return 0;
	}

	iTempRet = flock(m_aiLockFDs[iLockIdx], LOCK_UN);

	return iTempRet;
}


