#ifndef __AGIPPAYDETAILDEMAND_H__
#define __AGIPPAYDETAILDEMAND_H__

#include "SysProtocol.h"

#define CMD_PAY_DETAIL_DEMAND          0x10003707
#define CMD_PAY_DETAIL_DEMAND_RES      0x20003707

typedef struct _AGIP_PAY_DETAIL_DEMAND_
{
    _AGIP_HEADER_   head;

    int32_t         n_Detail_Type;   // 1-增值道具消费 2-元宝流通
    int16_t         s_Subject_ID;
    uint16_t        us_Detail_Count;
    uint64_t        unn_Detail_ID;

    _AGIP_CHECKSUM_ checksum;
}SAGIPPayDetailDemand, *PSAGIPPayDetailDemand;

typedef struct _AGIP_PAY_DETAIL_DEMAND_RES_
{
    _AGIP_HEADER_   head;

    int32_t         n_Result_Code;

    _AGIP_CHECKSUM_ checksum;
}SAGIPPayDetailDemandRes, *PSAGIPPayDetailDemandRes;

class Engine_Export AGIPPayDetailDemand : public SysProtocol
{
public:
    AGIPPayDetailDemand();
    ~AGIPPayDetailDemand();

    int32_t         getDetailType(void) const;
    void            setDetailType(int32_t DetailType);

    int16_t         getSubjectID(void) const;
    void            setSubjectID(int16_t SubjectID);

    // 变长数据1成功，其它为错误
    int32_t         getDetailID(int index, uint64_t &DetailID, int32_t nFlag = 1) const;
    int32_t         setDetailID(int index, uint64_t  DetailID, int32_t nFlag = 1);
    int32_t         addDetailID(uint64_t DetailID, int32_t nFlag = 1);

    uint16_t        getDetailCount(void) const;

    int             showInfo(void);

protected:
    void            setDetailCount(uint16_t DetailCount);

protected:
    PSAGIPPayDetailDemand   _demand_buff;
};

class Engine_Export AGIPPayDetailDemandRes : public SysProtocol
{
public:
    AGIPPayDetailDemandRes();
    ~AGIPPayDetailDemandRes();

    int32_t         getResultCode(void) const;
    void            setResultCode(int32_t Resultcode);

    int             showInfo(void);

protected:
    PSAGIPPayDetailDemandRes    _demand_res_buff;
};

#endif

