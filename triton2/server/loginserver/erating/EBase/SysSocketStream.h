#ifndef __SYS_SOCKET_STREAM_H__
#define __SYS_SOCKET_STREAM_H__

#include "SysSocketIO.h"
class Engine_Export SysSocketStream :
    public SysSocketIO
{
public:
    SysSocketStream(void);
    SysSocketStream(SysSocketStream& sysSocketStream);

    void operator = (SysSocketStream& sysSocketStream);
public:
    virtual ~SysSocketStream(void);

    int recv_n(
        void *pRecvBuffer,
        int nRecvBufferSize,
        int nFlags,
        const SysTimeValue *pTimeout,
        int* pnBytesRecved
    );

    int send_n(
        const void *pSendBuffer,
        int         nSendBufferSize,
        int         nFlags,
        const SysTimeValue *pTimeout,
        int        *pnBytesSent
    );

};

#endif

