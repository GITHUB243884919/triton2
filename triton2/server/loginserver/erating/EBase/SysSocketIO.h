#ifndef __SYS_SOCKET_IO_H__
#define __SYS_SOCKET_IO_H__
#include "SysSocket.h"
#include "SysTimeValue.h"

class SysSocket;
class SysTimeValue;

class Engine_Export SysSocketIO :
    public SysSocket
{
public:
    SysSocketIO(void);
public:
    virtual ~SysSocketIO(void);
    int recv(
        void   *pRecvBuffer,
        int     nRecvBufferSize,
        int    *pnRecvBytes,
        int     nFlags,
        const SysTimeValue *pTimeout = 0
    );

    int send(
        const void *pSendBuffer,
        int         nSendBufferSize,
        int        *pnSendBytes,
        int         nFlags,
        const SysTimeValue *pTimeout = 0
    );

};

#endif
