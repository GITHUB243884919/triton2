#ifndef __SYS_SOCKET_H__
#define __SYS_SOCKET_H__
#include "SysIPCSAP.h"
#include "SysSocketAddress.h"


#ifdef WIN32
#define socklen_t int
#endif 
class SysSocketAddress;

class Engine_Export SysSocket : public SysIPCSAP
{
public:

    int setOption(
        int nLevel,
        int nOption,
        void *pOptionVal,
        socklen_t nOptionLen
    ) const;

    int getOption(
        int nLevel,
        int nOption,
        void *pOptionVal,
        socklen_t  *nOptionLen
    ) const;

    int close(void);

    int open(int nType, int nProtocolFamily, int nProtocol, int nReuseAddr);

    int getLocalAddress(SysSocketAddress *pSysSocketAddress);
    int getRemoteAddress(SysSocketAddress *pSysSocketAddress);

    virtual int initialize();
    virtual int release();

protected:
    SysSocket(void);
    virtual ~SysSocket(void);
    SysSocketAddress m_localAddress;
    SysSocketAddress m_remoteAddress;
private:
    static int n_InitFlag;
};

#endif

