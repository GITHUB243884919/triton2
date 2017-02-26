#ifndef __SYS_LOG_MANAGER_H__
#define __SYS_LOG_MANAGER_H__
#include "SysLog.h"
#include "SysMutex.h"

class SysLog;

class Engine_Export SysLogManager
{
private:
    SysLogManager(void);
public:
    virtual ~SysLogManager(void);
    static int initialize(const char *pcLogPath = LOG_DEFAULT_PATH, size_t stFileMaxBytes = FILE_MAX_BYTES, SysLog::LOG_LEVEL nLogLevel = SysLog::LOG_LEVEL_DEBUG);
    static int release();
    static int output(SysLog::LOG_TYPE nLogType, char *pcFormat, ...);

    static int setLogLevel(SysLog::LOG_LEVEL nLogLevel);
private:
    static SysLog *m_pSysLog;
    
};

#define LOG_OUPUT SysLogManager::output

#endif
