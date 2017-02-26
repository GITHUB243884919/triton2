#ifndef __SYS_THREAD_H__
#define __SYS_THREAD_H__

#include "Macro.h"


#ifdef WIN32
#   define RETTHREAD int
#else
#   define RETTHREAD void*
#   define HANDLE    pthread_t
#endif



class SysThreadManager;

class Engine_Export SysThread
{
    friend class SysThreadManager;
    static RETTHREAD ThreadFunc(SysThread *pSysThread);
    typedef struct _STRUCT_THREAD_ATTR_
    {
        long lFlag;
        int nForceActive;
        long lPriority;
        int nGroupID;
        int nStackSize;
    }STRUCT_THREAD_ATTR;

public:
    SysThread(void);
    SysThread(int32_t m_nID);
    SysThread(SysThread& sysThread);

    virtual int initialize(
        long lThreadFlags = THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED,
        int nForceActive = 0,
        long lPriority = DEFAULT_THREAD_PRIORITY,
        int nGroupID = -1,
        int nStackSize = 0
    );

    int start();
    
    
protected:
    virtual int workThread() = 0;
    STRUCT_THREAD_ATTR m_threadAttr;

    virtual int release();
    int terminate();
    int wait(uint32_t dwMillSecond);
    int printThreadID(const char *pLog);

    int resume();
    int suspend();
    
    
#ifdef LINUX
    pthread_attr_t m_attr;
#endif
public:
    virtual ~SysThread(void);
    
    HANDLE& getHandler()
    {
        return m_nHandler;
    }

    unsigned long& getThreadID()
    {
        return m_ulThreadID;
    }
    int32_t & getID()
    {
        return m_nID;
    }

    int32_t m_nID;
private:
    HANDLE m_nHandler;
    unsigned long m_ulThreadID;
    int m_nExitFlag;
    int m_nInitFlag;

};



#endif
