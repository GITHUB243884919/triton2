#ifndef _AGIP_UNBIND_H__
#define _AGIP_UNBIND_H__

#include "SysProtocol.h"

#define CMD_UNBIND                      0x10000002
#define CMD_UNBIND_RES                  0x20000002

typedef struct _AGIP_UNBIND_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Gateway_ID;
    _AGIP_CHECKSUM_ checkSum;
} SAGIPUnbind, *PSAGIPUnbind;

typedef struct _AGIP_UNBIND_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUnbindRes, *PSAGIPUnbindRes;


class Engine_Export AGIPUnbind :
    public SysProtocol
{
public:
    AGIPUnbind(void);
    
    int getGatewayID(uint32_t *punGatewayID);

    int setGatewayID(uint32_t unGatewayID);

    virtual int showInfo();
public:
    virtual ~AGIPUnbind(void);
};

class Engine_Export AGIPUnbindRes :
    public SysProtocol
{
    friend class AGIPUnbind;
public:
    AGIPUnbindRes(void);

public:
    virtual ~AGIPUnbindRes(void);

    int getResultCode(int32_t *pnResultCode);
    int setResultCode(int32_t nResultCode);
    virtual int showInfo();
};


#endif

