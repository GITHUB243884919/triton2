#ifndef __AGIP_BIND_H__
#define __AGIP_BIND_H__

#include "SysProtocol.h"

#define CMD_BIND                        0x10000001
#define CMD_BIND_RES                    0x20000001

typedef struct _AGIP_BIND_
{
    _AGIP_HEADER_   header;
    char            str_Gateway_Code[AGIP_GATEWAY_CODE_LEN];
    char            str_Password[AGIP_PASSWORD_LEN];
    char            str_MAC[AGIP_MAC_LEN];
    int8_t          c_Reconnect_Flag;
    int8_t          c_Pad;
    uint16_t        us_Pad;
    uint32_t        un_Server_ID;
    _AGIP_CHECKSUM_ checkSum;
} SAGIPBind, *PSAGIPBind;

typedef struct _AGIP_BIND_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    uint32_t        un_Gateway_ID;
    
    _AGIP_CHECKSUM_ checksum;
} SAGIPBindRes, *PSAGIPBindRes;


class Engine_Export AGIPBind :
    public SysProtocol
{
public:
    AGIPBind(void);

public:
    virtual ~AGIPBind(void);
    enum
    {
        RECONNECT_FLAG_FIRST_CONNECTION = 0,
        RECONNECT_FLAG_RECONNECTION     = 1
    };

    int getGatewayCode(char *strGatewayCode);
    int getPassword(char *strPassword);
    const char* getMAC(char *strMAC, size_t unMACSize);
    int getReconnectFlag(int8_t *pcReconnectFlag);
    int getServerID(uint32_t *punServerID);

    int setGatewayCode(const char *strGatewayCode);
    int setPassword(const char *strPassword);
    int setMAC(const char* szMAC);
    int setReconnectFlag(int8_t cReconnectFlag);
    int setServerID(uint32_t unServerID);

    virtual int showInfo();
};


class Engine_Export AGIPBindRes :
    public SysProtocol
{
    friend class AGIPBind;
public:
    AGIPBindRes(void);

public:
    virtual ~AGIPBindRes(void);
    
    int getResultCode(int32_t *pnResultCode);
    int getGatewayID(uint32_t *punGatewayID);

    int setResultCode(int32_t nResultCode);
    int setGatewayID(uint32_t unGatewayID);

    virtual int showInfo();
};

#endif
