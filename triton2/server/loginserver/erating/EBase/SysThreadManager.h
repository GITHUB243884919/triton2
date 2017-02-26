#ifndef __SYS_THREAD_MANAGER_H__
#define __SYS_THREAD_MANAGER_H__
#include <vector>
#include "SysThread.h"
using namespace std;

class Engine_Export SysThreadManager
{
    typedef vector<SysThread*> PTHREADLIST;
protected:
    SysThreadManager(void);
public:
    virtual ~SysThreadManager(void);
    static int initialize();
    static int wait();
    static int addSysThread(SysThread* pSysThread);

    static int hasSysThread(const SysThread *pSysThread, int *pnHasThread);

    static int removeSysThread(SysThread* pSysThread);
    
    static int release();
    static int createThread(
        long flags = THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED,
        int nThreads = 1,
        int force_active = 0,
        long priority = DEFAULT_THREAD_PRIORITY,
        int nGroupID = -1,
        SysThread *pSysThread = NULL,
        int arrnThreadHandles[] = NULL,
        void *arrzpStack[] = NULL,
        uint32_t arrunStackSize[] = NULL,
        int arrnThreadIDs[] = NULL
    );
private:
    PTHREADLIST m_pThreadList;
    static SysThreadManager* m_pSysThreadManager;
};

#endif
