#ifndef __SYS_TIME_VALUE_H__
#define __SYS_TIME_VALUE_H__

#include "Macro.h"

class Engine_Export SysTimeValue
{
public:
    SysTimeValue(void);
    SysTimeValue(SysTimeValue& sysTimeValue);

    static int getTickCount(uint64_t *pullMilliSecond);
    

    void setMillisecond(long lMillisecond);
    void setSecond(long lSecond);
    
    long getSecond();
    long getMilliSecond();

    operator const timeval () const;
    const timeval * getTimeval() const;
public:
    virtual ~SysTimeValue(void);
protected:
    struct timeval m_timeVal;
};

#endif
