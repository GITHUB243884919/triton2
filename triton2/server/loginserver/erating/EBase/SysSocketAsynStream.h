#ifndef __SYS_SOCKET_ASYN_STREAM_H__
#define __SYS_SOCKET_ASYN_STREAM_H__

#include "SysSocketIO.h"
class Engine_Export SysSocketAsynStream
    : public SysSocketIO
{
public:
    SysSocketAsynStream(void);
    SysSocketAsynStream(SysSocketAsynStream& sysSocketAsynStream);

    void operator = (SysSocketAsynStream& sysSocketAsynStream);
public:
    virtual ~SysSocketAsynStream(void);
    int recv_n(
        void *pRecvBuffer,
        int   nRecvBufferSize,
        int*  pnBytesRecved,
        SysTimeValue *pTimeValue = NULL
    );

    int send_n(
        const void *pSendBuffer,
        int         nSendBufferSize,
        int        *pnBytesSent
    );

};
#endif
