#ifndef __SYS_SOCKET_ACCEPTOR_H__
#define __SYS_SOCKET_ACCEPTOR_H__
#include "SysSocketAddress.h"
#include "SysSocketStream.h"
#include "SysSocket.h"

class SysIPAddress;

class Engine_Export SysSocketAcceptor : public SysSocket
{
public:
    SysSocketAcceptor(void);

//    int initialize();
//    int release();
    int open(
        SysSocketAddress& localAddress,
        int nReusedAddress = 0,
        int nProtocolFamily = AF_INET,
        int nBackLog = 1024,
        int nProtocol = 0,
        int nNonblocking = 0
    );

    int accept(
        SysSocketStream& newStream,
        SysSocketAddress* pRemoteAddress = 0,
        int nRestart = 0,
        int nResetNewHandle = 0,
        SysTimeValue *pTimeout = 0
    );

    //int accept(SOCKET& );
    int close();

public:
    virtual ~SysSocketAcceptor(void);
private:
    SOCKET m_socketHandle;
};

#endif
