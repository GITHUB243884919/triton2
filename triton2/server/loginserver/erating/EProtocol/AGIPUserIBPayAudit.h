#ifndef  __AGIP_USER_IB_PAY_AUDIT_H__
#define  __AGIP_USER_IB_PAY_AUDIT_H__

#include "SysProtocol.h"

#define CMD_USER_IB_PAY_AUDIT           0x10003702
#define CMD_USER_IB_PAY_AUDIT_RES       0x20003702

typedef struct _AGIP_USER_IB_PAY_AUDIT_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    uint32_t        un_Rating_ID;
    uint16_t        us_Subject_ID;
    uint16_t        us_Audit_Count;
    uint32_t        un_Bgn_Detail_ID_H;
    uint32_t        un_Bgn_Detail_ID_L;
    uint32_t        un_End_Detail_ID_H;
    uint32_t        un_End_Detail_ID_L;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserIBPayAudit, *PSAGIPUserIBPayAudit;

class Engine_Export AGIPUserIBPayAudit :
    public SysProtocol
{
public:
    AGIPUserIBPayAudit(void);
    virtual ~AGIPUserIBPayAudit(void);

    uint32_t getUserID()const;
    uint32_t getRatingID()const;
    uint16_t getSubjectID()const;
    uint16_t getAuditCount()const;
    uint64_t getBgnDetailID(int32_t nFlag = 1)const;
    uint32_t getBgnDetailIDH()const;
    uint32_t getBgnDetailIDL()const;
    uint64_t getEndDetailID(int32_t nFlag = 1)const;
    uint32_t getEndDetailIDH()const;
    uint32_t getEndDetailIDL()const;

    void setUserID(uint32_t unUserID);
    void setRatingID(uint32_t unRatingID);
    void setSubjectID(uint16_t usSubjectID);
    void setAuditCount(uint16_t usAuditCount);
    void setBgnDetailID(uint64_t ullBgnDetailID, int32_t nFlag = 1);
    void setBgnDetailIDH(uint32_t unBgnDetailIDH);
    void setBgnDetailIDL(uint32_t unBgnDetailIDL);
    void setEndDetailID(uint64_t ullEndDetailID, int32_t nFlag = 1);
    void setEndDetailIDH(uint32_t unEndDetailIDH);
    void setEndDetailIDL(uint32_t unEndDetailIDL);

    virtual int showInfo();
};

typedef struct _AGIP_USER_IB_PAY_AUDIT_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    uint32_t        un_User_ID;
    uint32_t        un_Rating_ID;
    uint16_t        us_Subject_ID;
    uint16_t        us_Audit_Count;
    uint32_t        un_Detail_ID_H;
    uint32_t        un_Detail_ID_L;
    int32_t         n_Pay_Amount;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserIBPayAuditRes, *PSAGIPUserIBPayAuditRes;

typedef struct _IB_PAY_AUDIT_INFO_
{
    uint32_t        un_Detail_ID_H;
    uint32_t        un_Detail_ID_L;
    int32_t         n_Pay_Amount;
} SIBPayAuditInfo, *PSIBPayAuditInfo;

class Engine_Export AGIPUserIBPayAuditRes :
    public SysProtocol
{
public:
    AGIPUserIBPayAuditRes(void);
    virtual ~AGIPUserIBPayAuditRes(void);

public:
    int32_t  getResultCode()const;
    uint32_t getUserID()const;
    uint32_t getRatingID()const;
    uint16_t getSubjectID()const;
    uint16_t getAuditCount()const;
    int getIBPayAuditInfo(int32_t nIndex, uint64_t* pullDetailID, int32_t* pnPayAmount, int32_t nFlag = 1)const;
    int getIBPayAuditInfo(int32_t nIndex, PSIBPayAuditInfo pIBPayAuditInfo, int32_t nFlag = 1)const;

    void setResultCode(int32_t  nResultCode );
    void setUserID    (uint32_t unUserID    );
    void setRatingID  (uint32_t unRatingID  );
    void setSubjectID (uint16_t usSubjectID );

    int addIBPayAuditInfo(uint64_t ullDetailID, int32_t nPayAmount, int32_t nFlag = 1);
    int addIBPayAuditInfo(const PSIBPayAuditInfo pIBPayAuditInfo, int32_t nFlag = 1);

    virtual int showInfo();
protected:
    void setAuditCount(uint16_t usAuditCount);

};

#endif  /*__AGIP_USER_IB_PAY_AUDIT_H__*/

