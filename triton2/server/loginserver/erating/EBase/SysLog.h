#ifndef __SYS_LOG_H__
#define __SYS_LOG_H__
#include "Macro.h"
#include "SysMutex.h"
#define FILE_MAX_BYTES 16777216
//16777216 / 4
#define LOG_DEFAULT_PATH "logs"

#define LOG_SUB_PATH_NAME "eRating"
// #pragma warning(disable: 4996)

class Engine_Export SysLog
{
    friend class SysLogManager;
public:
    enum LOG_TYPE
    {
        LOG_DEBUG   = 0x01,
        LOG_WARNING = 0x02,
        LOG_INFO    = 0x04,
        LOG_SYSTEM  = 0x08
    };
    enum LOG_LEVEL
    {
        LOG_LEVEL_SYSTEM = LOG_SYSTEM,
        LOG_LEVEL_DEBUG = LOG_WARNING | LOG_INFO | LOG_SYSTEM | LOG_DEBUG,
        LOG_LEVEL_USER = LOG_SYSTEM | LOG_INFO | LOG_WARNING
    };

private:
    SysLog(void);
    virtual ~SysLog(void);

    virtual int initialize(const char *pcLogPath = LOG_DEFAULT_PATH, size_t stFileMaxBytes = FILE_MAX_BYTES, SysLog::LOG_LEVEL nLogLevel = SysLog::LOG_LEVEL_DEBUG);
    virtual int release();

    int setPath(const char *pcLogPath);
    int close(FILE *pFile = 0);
    int open(const char* pcLogFileName);
    int output(SysLog::LOG_TYPE nLogType, const char *pcLogInfo);
    int setLogLevel(SysLog::LOG_LEVEL nLogLevel)
    {
        this->m_nLogLevel = nLogLevel;
        return S_SUCCESS;
    };
private:
    int getLogDateString(char* pcDateString, uint32_t unDateStringSize);
    int getLogDateTimeString(char* pcDateTimeString, uint32_t unDateTimeStringSize);
    int getLogFileName(char* pcLogFileName);

    int checkShouldChangeFile(int32_t *pnShouldChangeFile);

    FILE *m_pFile;
    
    size_t m_stFileBytesWrite; // Bytes already written.

    size_t m_stFileMaxBytes;
    size_t m_stFileCounts;

    SysMutex m_sysMutex;

    LOG_LEVEL m_nLogLevel;

    char m_szLogPath[FILE_MAX_PATH];
    char m_szLogSubPath[FILE_MAX_PATH];

    char m_szLogFileName[FILE_MAX_PATH];
    char m_szLogDateString[12];//2005-06-04
};

#endif

