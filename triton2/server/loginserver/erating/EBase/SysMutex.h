#ifndef __SYS_MUTEX_H__
#define __SYS_MUTEX_H__
#include "OS.h"
#include "Macro.h"

#ifdef WIN32
#define MUTEX_HANDLE CRITICAL_SECTION
#else
#define MUTEX_HANDLE pthread_mutex_t
#endif

class Engine_Export SysMutex
{

public:
    
    SysMutex();
    int lock();
    int unlock();
    int tryLock();
    virtual int initialize();
    virtual int release();
public:
    virtual ~SysMutex(void);
protected:
    MUTEX_HANDLE m_mutexHandle;
    int          m_nInitFlag;
};

#endif
