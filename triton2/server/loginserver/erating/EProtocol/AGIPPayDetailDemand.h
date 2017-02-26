#ifndef __AGIPPAYDETAILDEMAND_H__
#define __AGIPPAYDETAILDEMAND_H__

#include "SysProtocol.h"

#define CMD_PAY_DETAIL_DEMAND          0x1000450B
#define CMD_PAY_DETAIL_DEMAND_RES      0x2000450B

typedef struct _AGIP_PAY_DETAIL_DEMAND_
{
    _AGIP_HEADER_   head;

    uint16_t        us_Game_ID;
    int16_t         pad;
    uint32_t        un_Gateway_ID;
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

    uint16_t        getGameID(void) const;
    void            setGameID(uint16_t GameID);
    uint32_t        getGatewayID(void) const;
    void            setGatewayID(uint32_t GatewayID);
    int16_t         getSubjectID(void) const;
    void            setSubjectID(int16_t SubjectID);

    // 变长数据 1成功，其它为错误
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
    void            setResultCode(int32_t ResultCode);

    int             showInfo(void);

protected:
    PSAGIPPayDetailDemandRes    _demand_res_buff;
};

#endif

