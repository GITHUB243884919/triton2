#ifndef __SYS_IPC_SAP_H__
#define __SYS_IPC_SAP_H__
#include "Macro.h"
#include "SysTimeValue.h"


class Engine_Export SysIPCSAP
{
protected:
    SysIPCSAP(void);
    virtual ~SysIPCSAP(void);
public:
    int setHandle(SYS_HANDLE handle);
    int getHandle(SYS_HANDLE *pHandle);

    int getHandleReady(
        const SysTimeValue *pTimeout,
        int nReadReadyFlag,
        int nWriteReadyFlag,
        int nExceptionReadyFlag,
        int *pnIsHandleReady
    );
    operator SYS_HANDLE ();
    int getHandleReadReady(const SysTimeValue *pTimeout, int *pnIsHandleReady);
    int getHandleWriteReady(const SysTimeValue *pTimeout, int *pnIsHandleReady);
    int getHandleExceptionReady(const SysTimeValue *pTimeout, int *pnIsHandleReady);

    int getFlags(int *pnFlags);
    int setFlags(int nFlags);
    int clearFlags(int nFlags);

    int enterSendTimedWait(const SysTimeValue *pTimeout, int &nFlags);
    int enterRecvTimedWait(const SysTimeValue *pTimeout, int &nFlags);
    int recordAndSetNonBlockingMode(int &nFlags);
    int restoreNonBlockingMode(int &nFlags);
    
protected:
    SYS_HANDLE m_handle;
};

#endif
