#ifndef __AGIP_USER_DATA_REPORT_H__
#define __AGIP_USER_DATA_REPORT_H__

#include "SysProtocol.h"

#define CMD_USER_DATA_REPORT            0x10003601
#define CMD_USER_DATA_REPORT_RES        0x20003601

const int32_t AGIP_MAX_USER_DATA_CNT = 20;

typedef struct _AGIP_USER_DATA_REPORT_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Gateway_ID;
    uint32_t        un_Server_ID;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    int32_t         n_Data_Count;
    int32_t         n_Data_Type;
    int32_t         n_Data_Value;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserDataReport, *PSAGIPUserDataReport;

typedef struct _AGIP_USER_DATA_REPORT_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserDataReportRes, *PSAGIPUserDataReportRes;

class Engine_Export AGIPUserDataReport :
    public SysProtocol
{
public:
    AGIPUserDataReport(void);
    virtual ~AGIPUserDataReport(void);

public:
    virtual int initialize();

    int getGatewayID(uint32_t *punGatewayID);
    int getServerID(uint32_t *punServerID);
    int getUserID(uint32_t *punUserID);
    int getRoleID(uint32_t *punRoleID);
    int getDataCount(int32_t *pnDataCount);
    int getDataInfo(int32_t nIndex, DATA_INFO *pDataInfo);
    int getDataInfo(int32_t nIndex, int32_t *pnDataType, int32_t *pnDataValue);

    int setGatewayID(uint32_t unGatewayID);
    int setServerID(uint32_t unServerID);
    int setUserID(uint32_t unUserID);
    int setRoleID(uint32_t unRoleID);
    int addDataInfo(const DATA_INFO *pDataInfo);
    int addDataInfo(int32_t nDataType, int32_t nDataValue);

    virtual int showInfo();
private:
    int setDataCount(int32_t nDataCount);
};


class Engine_Export AGIPUserDataReportRes :
    public SysProtocol
{
    friend class AGIPUserDataReport;
public:
    AGIPUserDataReportRes(void);
public:
    virtual ~AGIPUserDataReportRes(void);

    int getResultCode(int32_t *pnResultCode);

    int setResultCode(int32_t nResultCode);

    virtual int showInfo();
};

#endif
