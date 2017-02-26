#ifndef __SYS_SOCKET_ASYN_ACCEPTOR_H__
#define __SYS_SOCKET_ASYN_ACCEPTOR_H__

#include "SysSocketAddress.h"
#include "SysSocketAsynStream.h"
#include "SysSocket.h"

class Engine_Export SysSocketAsynAcceptor
    : public SysSocket
{
public:
    SysSocketAsynAcceptor(void);
    int open(SysSocketAddress& localAddress, int nAddrReused);

    int accept(
        SysSocketAsynStream &newStream,
        SysSocketAddress    *pRemoteAddress = 0
    );

public:
    virtual ~SysSocketAsynAcceptor(void);
};
#endif
