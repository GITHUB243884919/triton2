#ifndef __AGIP_USER_CHARGE_H__
#define __AGIP_USER_CHARGE_H__

#include "SysProtocol.h"

#define CMD_USER_CHARGE                 0x10003401
#define CMD_USER_CHARGE_RES             0x20003401

typedef struct _AGIP_USER_CHARGE_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Detail_ID_H;
    uint32_t        un_Detail_ID_L;
    uint32_t        un_User_ID;
    uint32_t        un_Rating_ID;
    uint16_t        us_Subject_ID;
    int16_t         s_Audit_Flag;
    int32_t         n_Amount;
    uint32_t        un_Charge_Time;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserCharge, *PSAGIPUserCharge;

class Engine_Export AGIPUserCharge :
    public SysProtocol
{
public:
    AGIPUserCharge(void);
    virtual ~AGIPUserCharge(void);

    uint64_t getDetailID(int32_t nFlag = 1)const;
    uint32_t getDetailIDH()const;
    uint32_t getDetailIDL()const;
    uint32_t getUserID()const;
    uint32_t getRatingID()const;
    uint16_t getSubjectID()const;
    int16_t  getAuditFlag()const;
    int32_t  getAmount()const;
    uint32_t getChargeTime()const;

    void setDetailID  (uint64_t ullDetailID, int32_t nFlag = 1);
    void setDetailIDH (uint32_t unDetailIDH);
    void setDetailIDL (uint32_t unDetailIDL);
    void setUserID    (uint32_t unUserID    );
    void setRatingID  (uint32_t unRatingID  );
    void setSubjectID (uint16_t usSubjectID );
    void setAuditFlag (int16_t  sAuditFlag  );
    void setAmount    (int32_t  nAmount     );
    void setChargeTime(uint32_t unChargeTime);

    virtual int showInfo();
};

typedef struct _AGIP_USER_CHARGE_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    uint32_t        un_Detail_ID_H;
    uint32_t        un_Detail_ID_L;
    uint32_t        un_User_ID;
    uint32_t        un_Rating_ID;
    uint16_t        us_Subject_ID;
    int16_t         s_Audit_Flag;
    int32_t         n_Total_Amount;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserChargeRes, *PSAGIPUserChargeRes;

class Engine_Export AGIPUserChargeRes :
    public SysProtocol
{
public:
    AGIPUserChargeRes(void);
    virtual ~AGIPUserChargeRes(void);

    int32_t  getResultCode()const;
    uint64_t getDetailID(int32_t nFlag = 1)const;
    uint32_t getDetailIDH()const;
    uint32_t getDetailIDL()const;
    uint32_t getUserID()const;
    uint32_t getRatingID()const;
    uint16_t getSubjectID()const;
    int16_t  getAuditFlag()const;
    int32_t  getTotalAmount()const;

    void setResultCode (int32_t  nResultCode);
    void setDetailID   (uint64_t ullDetailID, int32_t nFlag = 1);
    void setDetailIDH  (uint32_t unDetailIDH);
    void setDetailIDL  (uint32_t unDetailIDL);
    void setUserID     (uint32_t unUserID   );
    void setRatingID   (uint32_t unRatingID );
    void setSubjectID  (uint16_t usSubjectID);
    void setAuditFlag  (int16_t  sAuditFlag );
    void setTotalAmount(int32_t  nTotalAmount);

    virtual int showInfo();
};

#endif /*__AGIP_USER_CHARGE_H__*/

