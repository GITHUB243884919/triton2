#ifndef __AGIP_USER_CASH_EXCHANGE_AUDIT_H__
#define __AGIP_USER_CASH_EXCHANGE_AUDIT_H__

#include "SysProtocol.h"

#define CMD_USER_CASH_EXCHANGE_AUDIT            0x10003705
#define CMD_USER_CASH_EXCHANGE_AUDIT_RES        0x20003705
#define AGIP_MAX_CASH_EXCHANGE_AUDIT_COUNT      64

typedef struct _AGIP_USER_CASH_EXCHANGE_AUDIT_{
    _AGIP_HEADER_       header;
    uint32_t            un_gateway_id;
    int16_t             n_subject_id;
    uint16_t            un_audit_count;
    uint32_t            un_bgn_detail_id_h;
    uint32_t            un_bgn_detail_id_l;
    uint32_t            un_end_detail_id_h;
    uint32_t            un_end_detail_id_l;
}SAGIPUserCashExchangeAudit, *PSAGIPUserCashExchangeAudit;

typedef struct _AGIP_USER_CASH_EXCHANGE_AUDIT_RES_{
    _AGIP_HEADER_       header;
    int32_t             n_result_code;
    uint16_t            un_audit_count;
    int16_t             n_pad;
    uint32_t            un_detail_id_h;
    uint32_t            un_detail_id_l;
}SAGIPUserCashExchangeAuditRes, *PSAGIPUserCashExchangeAuditRes;

typedef struct _CASH_EXCHANGE_AUDIT_INFO_{
    uint32_t            un_detail_id_h;
    uint32_t            un_detail_id_l;
}SCashExchangeAuditInfo, *PSCashExchangeAuditInfo;

class Engine_Export AGIPUserCashExchangeAudit :
    public SysProtocol
{
public:
    AGIPUserCashExchangeAudit(void);

public:
    ~AGIPUserCashExchangeAudit(void);
    
    uint32_t    getGatewayID(void) const;
    void        setGatewayID(uint32_t unGatewayID);

    int16_t     getSubjectID(void) const;
    void        setSubjectID(int16_t nsSubjectID);

    uint16_t    getAuditCount(void) const;
    void        setAuditCount(uint16_t usAuditCount);

    uint64_t    getBgnDetailID(int32_t nFlag = 1) const;
    void        setBgnDetailID(uint64_t ullDetailID,int32_t nFlag = 1);

    uint64_t    getEndDetailID(int32_t nFlag = 1) const;
    void        setEndDetailID(uint64_t ullDetailID, int32_t nFlag = 1);

    virtual int showInfo();
};

class Engine_Export AGIPUserCashExchangeAuditRes :
    public SysProtocol
{
public:
    AGIPUserCashExchangeAuditRes(void);

public:
    ~AGIPUserCashExchangeAuditRes(void);

    int32_t     getResultCode(void) const;
    void        setResultCode(int32_t nResultCode);

    uint16_t    getAuditCount(void) const;

    uint64_t    getDetailID(int32_t nIndex, int32_t nFlag = 1);
    int32_t     addDetailID(uint64_t ullDetailID, int32_t nFlag = 1);

    virtual int showInfo();
protected:
    void        setAuditCount(uint16_t usAuditCount);
};

#endif
