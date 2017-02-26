#ifndef __AGIP_UNREGIST_SERVICE_H__
#define __AGIP_UNREGIST_SERVICE_H__

#include "SysProtocol.h"

#define CMD_UNREGIST_SERVICE                  0x10004001
#define CMD_UNREGIST_SERVICE_RES              0x20004001

typedef struct _AGIP_UNREGIST_SERVICE_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Command_ID;
    uint32_t        un_Version;
    uint32_t        un_Step;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUnregistService, *PSAGIPUnregistService;

typedef struct _AGIP_UNREGIST_SERVICE_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUnregistServiceRes, *PSAGIPUnregistServiceRes;


class Engine_Export AGIPUnregistService :
    public SysProtocol
{
public:
    AGIPUnregistService(void);
    virtual ~AGIPUnregistService(void);

    uint32_t getCommandID(void);
    void     setCommandID(uint32_t unCommandID);

    uint32_t getVersion(void);
    void     setVersion(uint32_t unVersion);

    uint32_t getStep(void);
    void     setStep(uint32_t unStep);

    virtual int showInfo();

private:
    PSAGIPUnregistService m_data;
};

class Engine_Export AGIPUnregistServiceRes :
    public SysProtocol
{
public:
    AGIPUnregistServiceRes(void);
    virtual ~AGIPUnregistServiceRes(void);

    int32_t  getResultCode(void);
    void     setResultCode(int32_t nResultCode);

    virtual int showInfo();

private:
    PSAGIPUnregistServiceRes m_data;
};

#endif
