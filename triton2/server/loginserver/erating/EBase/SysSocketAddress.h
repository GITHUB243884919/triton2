/*
    Date:   2007-06-20
    comment:Add method setIPAddress(const uint32_t unIPAddress);
    Author: Zhaojun

*/
#ifndef __SYS_SOCKET_ADDREASS_H__
#define __SYS_SOCKET_ADDREASS_H__

#include "Macro.h"
#include "SysIPAddress.h"

class SysIPAddress;

class Engine_Export SysSocketAddress
{
public:
    SysSocketAddress(void);
    SysSocketAddress(const SysSocketAddress& sysSocketAddress);
    SysSocketAddress(const SysIPAddress& sysIPAddress, uint16_t usPortNumber);
public:
    virtual ~SysSocketAddress(void);
    
    int setAddress(const SysIPAddress& sysIPAddress, uint16_t usPortNumber);
    int setAddress(const char *pIPAddress, uint16_t usPortNumber);
    int setAddress(const SysSocketAddress& sysSocketAddress);

    int setIPAddress(const SysIPAddress& sysIPAddress);
    int setIPAddress(const uint32_t unIPAddress);
    int setPortNumber(uint16_t usPortNumber);

    int setSockAddr(const sockaddr_in *pSockAddr);

    int getPortNumber(uint16_t *pusPortNumber);
    int getIPAddress(SysIPAddress *pSysIPAddress);

    int getSockAddr(sockaddr_in *pSockAddr);
    int getAddressString(char *pAddressString);

    void operator  = (const SysSocketAddress& sysSocketAddress);
    int  operator == (const SysSocketAddress& sysSocketAddress);
    int  operator != (const SysSocketAddress& sysSocketAddress);

private:
    SysIPAddress m_sysIPAddress;
    uint16_t     m_usPortNumber;
};

#endif
