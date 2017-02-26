#ifndef __SYS_MUTEX_ERROR_CHECK_H__
#define __SYS_MUTEX_ERROR_CHECK_H__
#include "SysMutex.h"

class Engine_Export SysMutexErrorCheck :
    public SysMutex
{
public:
    SysMutexErrorCheck(void);
public:
    virtual ~SysMutexErrorCheck(void);
    virtual int initialize();
};
#endif


