#ifndef __SYS_SOCKET_CONNECTOR_H__
#define __SYS_SOCKET_CONNECTOR_H__
#include "SysSocketIO.h"
#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif
class Engine_Export SysSocketConnector : public SysSocketIO
{
public:
    SysSocketConnector(void);
public:
    virtual ~SysSocketConnector(void);
    int connect(
        SysSocketAddress& remoteAddress,
        const SysTimeValue *pTimeout = 0,
        int nReuseAddr = 0,
        int nFlags = 0, /* Ignored */
        int nPerms = 0, /* Ignored */
        int nProtocol = 0
    );
private:
    SysSocketAddress m_remoteAddress;
    //SysSocketAddress m_localAddress;
};
#endif

