#ifndef _THREAD_HPP_
#define _THREAD_HPP_

#include <pthread.h>

#ifdef _DEBUG_
#define TRACE_DEBUG ThreadLogDebug
#else
#define TRACE_DEBUG
#endif

#define TRACE_ERROR ThreadLogError

#ifdef _DEBUG_
#define DTRACE ThreadLog
#else
#define DTRACE
#endif

enum eRunStatus
{
	rt_init = 0,
	rt_blocked = 1,
	rt_running = 2,
	rt_stopped = 3
};

typedef struct
{
	char szThreadKey[32];
	char szLogBaseName[200];
	long lMaxLogSize;
	int iMaxLogNum;
} TLogCfg;

void* ThreadProc( void *pvArgs );

class CThread
{
public:
	CThread();
	virtual ~CThread();

	virtual int PrepareToRun() = 0;
	virtual int Run() = 0;
	virtual int IsToBeBlocked() = 0;

	int CreateThread();
	int WakeUp();
	int StopThread();
	void ThreadLogInit(char *sPLogBaseName, long lPMaxLogSize, int iPMaxLogNum, int iShow, int iLevel = 0);

protected:
	int CondBlock();
	void ThreadLogDebug(const char *sFormat, ...);
	void ThreadLogError(const char *sFormat, ...);

	pthread_t m_hTrd;
	pthread_attr_t m_stAttr;
	pthread_mutex_t m_stMutex;
	pthread_cond_t m_stCond;
	int m_iRunStatus;
	char m_abyRetVal[64];

	TLogCfg m_stLogCfg;

private:
};


#endif
