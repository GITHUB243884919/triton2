#ifndef __AGIP_GW_DATA_REPORTYT_H__
#define __AGIP_GW_DATA_REPORTYT_H__

#include "SysProtocol.h"

#define CMD_GW_DATA_REPORT              0x10002003
#define CMD_GW_DATA_REPORT_RES          0x20002003

typedef struct _AGIP_GW_DATA_REPORTYT_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Gateway_ID;
    uint32_t        un_Server_ID;
    int32_t         n_Data_Count;
    int32_t         n_Data_Type;
    int32_t         n_Data_Value;
    _AGIP_CHECKSUM_ checksum;
} SAGIPGWDataReportYT, *PSAGIPGWDataReportYT;

typedef struct _AGIP_GW_DATA_REPORTYT_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPGWDataReportYTRes, *PSAGIPGWDataReportYTRes;


class Engine_Export AGIPGWDataReportYT :
    public SysProtocol
{
public:
    AGIPGWDataReportYT(void);
public:
    virtual ~AGIPGWDataReportYT(void);

    virtual int initialize();

    int getGameID(uint16_t *pusGameID);
    int getGatewayID(uint32_t *punGatewayID);
    int getServerID(uint32_t *punServerID);
    int getDataCount(int32_t *pnDataCount);
    int getDataInfo(int32_t nIndex, DATA_INFO *pDataInfo);
    int getDataInfo(int32_t nIndex, int32_t *pnDataType, int32_t *pnDataValue);

    int setGameID(uint16_t usGameID);
    int setGatewayID(uint32_t unGatewayID);
    int setServerID(uint32_t unServerID);
    int addDataInfo(const DATA_INFO *pDataInfo);
    int addDataInfo(int32_t nDataType, int32_t nDataValue);

    virtual int showInfo();
private:
    int setDataCount(int32_t nDataCount);
};

class Engine_Export AGIPGWDataReportYTRes :
    public SysProtocol
{
    friend class AGIPGWDataReportYT;
public:
    AGIPGWDataReportYTRes(void);
public:
    virtual ~AGIPGWDataReportYTRes(void);

    int getResultCode(int32_t *pnResultCode);

    int setResultCode(int32_t nResultCode);

    virtual int showInfo();
};

#endif
