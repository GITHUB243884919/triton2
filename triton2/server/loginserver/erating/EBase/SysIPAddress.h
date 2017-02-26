#ifndef __SYS_IP_ADDRESS_H__
#define __SYS_IP_ADDRESS_H__
#include "Macro.h"

class Engine_Export SysIPAddress
{
public:
    SysIPAddress(void);
    SysIPAddress(const char *pcIPAddress);
    SysIPAddress(const SysIPAddress& sysIPAddress);
public:

    int setIPAddress(const SysIPAddress& sysIPAddress);

    int setIPAddress(const char *pcIPAddress);
    int setIPAddress(const uint32_t unIPAddress);

    int getIPAddressString(char *pcIPAddress);

    operator const uint32_t() const;
    
    void operator = (const SysIPAddress &sysIPAddress);
    void operator = (const uint32_t unIPAddress);

    virtual ~SysIPAddress(void);
private:
    uint32_t m_unIPAddress;
};

#endif
