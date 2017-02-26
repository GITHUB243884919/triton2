#ifndef __SYS_MUTEX_RECURSIVE_H__
#define __SYS_MUTEX_RECURSIVE_H__
#include "SysMutex.h"

class Engine_Export SysMutexRecursive :
    public SysMutex
{
public:
    SysMutexRecursive(void);
    virtual int initialize();
public:
    virtual ~SysMutexRecursive(void);
};

#endif
