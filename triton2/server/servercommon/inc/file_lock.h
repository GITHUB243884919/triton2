#ifndef _FILE_LOCK_H_
#define _FILE_LOCK_H_

class CFileLock
{
public:
	CFileLock();
	~CFileLock();

	enum eParas{MAX_FILE_LOCK = 32};

	int	SetLock(int iLockIdx, const char *szFileName);
	int	DelLock(int iLockIdx);
	int	WLockWait(int iLockIdx);
	int	RLockWait(int iLockIdx);
	int	UnLock(int iLockIdx);

private:
	int m_aiLockFDs[MAX_FILE_LOCK];
};

#endif
