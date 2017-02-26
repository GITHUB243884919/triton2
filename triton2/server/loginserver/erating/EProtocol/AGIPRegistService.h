#ifndef __AGIP_REGIST_SERVICE_H__
#define __AGIP_REGIST_SERVICE_H__

#include "SysProtocol.h"

#define CMD_REGIST_SERVICE                  0x10004000
#define CMD_REGIST_SERVICE_RES              0x20004000

typedef struct _AGIP_REGIST_SERVICE_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Command_ID;
    uint32_t        un_Version;
    uint32_t        un_Step;
    uint32_t        un_Game_ID;
    uint32_t        un_Gateway_ID;
    _AGIP_CHECKSUM_ checksum;
} SAGIPRegistService, *PSAGIPRegistService;

typedef struct _AGIP_REGIST_SERVICE_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPRegistServiceRes, *PSAGIPRegistServiceRes;


class Engine_Export AGIPRegistService :
    public SysProtocol
{
public:
    AGIPRegistService(void);
    virtual ~AGIPRegistService(void);

    uint32_t getCommandID(void);
    void     setCommandID(uint32_t unCommandID);

    uint32_t getVersion(void);
    void     setVersion(uint32_t unVersion);

    uint32_t getStep(void);
    void     setStep(uint32_t unStep);

    uint32_t getGameID(void);
    void     setGameID(uint32_t unGameID);

    uint32_t getGatewayID(void);
    void     setGatewayID(uint32_t unGatewayID);

    virtual int showInfo();

private:
    PSAGIPRegistService m_data;
};

class Engine_Export AGIPRegistServiceRes :
    public SysProtocol
{
public:
    AGIPRegistServiceRes(void);
    virtual ~AGIPRegistServiceRes(void);

    int32_t  getResultCode(void);
    void     setResultCode(int32_t nResultCode);

    virtual int showInfo();

private:
    PSAGIPRegistServiceRes m_data;
};

#endif
