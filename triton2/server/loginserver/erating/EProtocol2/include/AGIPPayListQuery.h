#ifndef __AGIPPAYLISTQUERY_H__
#define __AGIPPAYLISTQUERY_H__

#include "SysProtocol.h"

#define CMD_PAY_LIST_QUERY          0x10003706
#define CMD_PAY_LIST_QUERY_RES      0x20003706

typedef struct _AGIP_PAY_LIST_QUERY_
{
    _AGIP_HEADER_   head;

    int32_t         n_Detail_Type;   // 1-增值道具消费 2-元宝流通
    int16_t         s_Subject_ID;
    uint16_t        us_Detail_Count;
    uint64_t        unn_Begin_Detail_ID;

    _AGIP_CHECKSUM_ checksum;
}SAGIPPayListQuery, *PSAGIPPayListQuery;

typedef struct _AGIP_PAY_LIST_QUERY_RES_
{
    _AGIP_HEADER_   head;

    int32_t         n_Result_Code;
    int32_t         n_Total_Amount;
    int16_t         s_Subject_ID;
    uint16_t        us_Detail_Count;
    uint64_t        unn_Detail_ID;

    _AGIP_CHECKSUM_ checksum;
}SAGIPPayListQueryRes, *PSAGIPPayListQueryRes;

class Engine_Export AGIPPayListQuery : public SysProtocol
{
public:
    AGIPPayListQuery();
    ~AGIPPayListQuery();

    int32_t         getDetailType(void) const;
    void            setDetailType(int32_t DetailType);

    int16_t         getSubjectID(void) const;
    void            setSubjectID(int16_t SubjectID);
    uint16_t        getDetailCount(void) const;
    void            setDetailCount(uint16_t DetailCount);
    uint64_t        getBeginDetailID(int32_t nFlag = 1) const;
    void            setBeginDetailID(uint64_t BeginDetailID, int32_t nFlag = 1);

    int             showInfo(void);

protected:
    PSAGIPPayListQuery  _query_buff;
};

class Engine_Export AGIPPayListQueryRes : public SysProtocol
{
public:
    AGIPPayListQueryRes();
    ~AGIPPayListQueryRes();

    int32_t         getResultCode(void) const;
    void            setResultCode(int32_t ResultCode);
    int16_t         getSubjectID(void) const;
    void            setSubjectID(int16_t SubjectID);
    int32_t         getTotalAmount(void) const;
    void            setTotalAmount(int32_t TotalAmount);

    uint16_t        getDetailCount(void) const;

    // 变长数据 返回1成功，其它为错误(index无效)
    int32_t         getDetailID(int index, uint64_t &DetailID, int32_t nFlag = 1) const;
    int32_t         setDetailID(int index, uint64_t  DetailID, int32_t nFlag = 1);
    int32_t         addDetailID(uint64_t DetailID, int32_t nFlag = 1);

    int             showInfo(void);

protected:
    void            setDetailCount(uint16_t DetailCount);

protected:
    PSAGIPPayListQueryRes   _query_res_buff;
};

#endif

