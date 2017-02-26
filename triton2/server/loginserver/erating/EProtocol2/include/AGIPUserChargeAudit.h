#ifndef  __AGIP_USER_CHARGE_AUDIT_H__
#define  __AGIP_USER_CHARGE_AUDIT_H__

#include "SysProtocol.h"

#define CMD_USER_CHARGE_AUDIT           0x10003404
#define CMD_USER_CHARGE_AUDIT_RES       0x20003404

typedef struct _AGIP_USER_CHARGE_AUDIT_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    uint32_t        un_Rating_ID;
    int16_t         s_Subject_ID;
    uint16_t        us_Audit_Count;
    uint32_t        un_Bgn_Detail_ID_H;
    uint32_t        un_Bgn_Detail_ID_L;
    uint32_t        un_End_Detail_ID_H;
    uint32_t        un_End_Detail_ID_L;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserChargeAudit, *PSAGIPUserChargeAudit;

typedef struct _CHARGE_INFO_SHORT_
{
    uint32_t        un_Detail_ID_H;
    uint32_t        un_Detail_ID_L;
    int32_t         n_Amount;
} SChargeInfoShort, *PSChargeInfoShort;

typedef struct _AGIP_USER_CHARGE_AUDIT_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    uint32_t        un_User_ID;
    uint32_t        un_Rating_ID;
    int16_t         s_Subject_ID;
    uint16_t        us_Audit_Count;
    uint32_t        un_Detail_ID_H;
    uint32_t        un_Detail_ID_L;
    int32_t         n_Amount;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserChargeAuditRes, *PSAGIPUserChargeAuditRes;

class Engine_Export AGIPUserChargeAudit :
    public SysProtocol
{
public:
    AGIPUserChargeAudit(void);
    virtual ~AGIPUserChargeAudit(void);

    uint32_t getRatingID(void);
    uint32_t getUserID(void);
    int16_t  getSubjectID(void);
    uint16_t getAuditCount(void);
    uint64_t getBgnDetailID(int32_t n_Flag = 1);
    uint64_t getEndDetailID(int32_t n_Flag = 1);

    int setRatingID(uint32_t       un_Rating_ID);
    int setUserID(uint32_t         un_User_ID);
    int setSubjectID(int16_t       s_Subject_ID);
    int setAuditCount(uint16_t     us_Audit_Count);
    int setBgnDetailID(uint64_t    ull_Bgn_Detail_ID, int32_t n_Flag = 1);
    int setEndDetailID(uint64_t    ull_End_Detail_ID, int32_t n_Flag = 1);

    virtual int showInfo();
};

class Engine_Export AGIPUserChargeAuditRes :
    public SysProtocol
{
public:
    AGIPUserChargeAuditRes(void);
    virtual ~AGIPUserChargeAuditRes(void);

    int32_t  getResultCode(void);
    uint32_t getRatingID(void);
    uint32_t getUserID(void);
    int16_t  getSubjectID(void);
    uint16_t getAuditCount(void);

    int getChargeInfoShort(uint16_t us_Index,
        uint64_t&   ull_Detail_ID,
        int32_t&    n_Amount,
        int32_t     n_Flag = 1);
    int getChargeInfoShort(uint16_t us_Index,
        SChargeInfoShort& charge_info_short,
        int32_t     n_Flag = 1);

    int setResultCode(int32_t      n_Result_Code);
    int setRatingID(uint32_t       un_Rating_ID);
    int setUserID(uint32_t     un_User_ID);
    int setSubjectID(int16_t       s_Subject_ID);

    int AddChargeInfoShort(const uint64_t& ull_Detail_ID,
        const int32_t& n_Amount,
        int32_t        n_Flag = 1);
    int AddChargeInfoShort(const SChargeInfoShort& charge_info_short,
        int32_t        n_Flag = 1);

    virtual int showInfo();
};

#endif  /*__AGIP_USER_CHARGE_AUDIT_H__*/

