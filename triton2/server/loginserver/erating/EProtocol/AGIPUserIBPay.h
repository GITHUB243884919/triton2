#ifndef __AGIP_USER_IB_PAY_H__
#define __AGIP_USER_IB_PAY_H__
#include "SysProtocol.h"

#define CMD_USER_IB_PAY         0x10003701
#define CMD_USER_IB_PAY_RES     0x20003701

typedef struct _AGIP_USER_IB_PAY_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Detail_ID_H;
    uint32_t        un_Detail_ID_L;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    uint8_t         uc_Role_Gender;
    uint8_t         uc_Role_Occupation;
    uint16_t        us_Role_Level;
    uint32_t        un_Rating_ID;
    char            sz_IB_Code[AGIP_IB_CODE_LEN];
    uint8_t         uc_Package_Flag;
    int8_t          c_Pad;
    uint16_t        us_IB_Count;
    int32_t         n_Pay_Time;
    uint32_t        un_User_IP;
    uint16_t        us_Subject_ID;
    uint16_t        us_Audit_Flag;
    int32_t         n_Price;
    int32_t         n_Discount_Price;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserIBPay, *PSAGIPUserIBPay;


class Engine_Export AGIPUserIBPay :
    public SysProtocol
{
public:
    enum
    {
        PACKAGE_FLAG_NORMAL      = 1,
        PACKAGE_FLAG_COMBINATION = 2

    };
    AGIPUserIBPay(void);
    virtual ~AGIPUserIBPay(void);
public:
    uint64_t getDetailID(int32_t nFlag = 1)const;
    uint32_t getDetailIDH()const;
    uint32_t getDetailIDL()const;
    uint32_t getUserID()const;
    uint32_t getRoleID()const;
    uint8_t  getRoleGender()const;
    uint8_t  getRoleOccupation()const;
    uint16_t getRoleLevel()const;
    uint32_t getRatingID()const;
    char*    getIBCode(char *szIBCode)const;
    uint8_t  getPackageFlag()const;
    uint16_t getIBCount()const;
    int32_t  getPayTime()const;
    uint32_t getUserIP()const;
    uint16_t getSubjectID()const;
    uint16_t getAuditFlag()const;
    int32_t  getPrice()const;
    int32_t  getDiscountPrice()const;
    
    void setDetailID    (uint64_t ullDetailID, int32_t nFlag = 1);
    void setDetailIDH   (uint32_t unDetailIDH);
    void setDetailIDL   (uint32_t unDetailIDL);
    void setUserID      (uint32_t unUserID);
    void setRoleID      (uint32_t unRoleID);
    void setRoleGender  (uint8_t ucGender);
    void setRoleOccupation(uint8_t ucOccupation);
    void setRoleLevel   (uint16_t usLevel);
    void setRatingID    (uint32_t unRatingID);
    void setIBCode      (const char *szIBCode);
    void setPackageFlag (uint8_t ucPackageFlag);
    void setIBCount     (uint16_t usIBCount);
    void setPayTime     (int32_t nPayTime);
    void setUserIP      (uint32_t unUserIP);
    void setSubjectID   (uint16_t usSubjectID);
    void setAuditFlag   (uint16_t usAuditFlag);
    void setPrice       (int32_t nPrice);
    void setDiscountPrice(int32_t nDiscountPrice);

    virtual int showInfo();
private:

};

typedef struct _AGIP_USER_IB_PAY_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    uint32_t        un_Detail_ID_H;
    uint32_t        un_Detail_ID_L;
    uint32_t        un_User_ID;
    uint32_t        un_Rating_ID;
    uint16_t        us_Subject_ID;
    uint16_t        us_Audit_Flag;
    int32_t         n_Total_Pay_Amount;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserIBPayRes, *PSAGIPUserIBPayRes;


class Engine_Export AGIPUserIBPayRes :
    public SysProtocol
{
public:
    AGIPUserIBPayRes(void);
    virtual ~AGIPUserIBPayRes(void);
public:
    int32_t  getResultCode()const;
    uint64_t getDetailID(int32_t nFlag = 1)const;
    uint32_t getDetailIDH()const;
    uint32_t getDetailIDL()const;
    uint32_t getUserID()const;
    uint32_t getRatingID()const;
    uint16_t getSubjectID()const;
    uint16_t getAuditFlag()const;
    int32_t  getTotalPayAmount()const;

    void setResultCode  (int32_t  nResultCode);
    void setDetailID    (uint64_t ullDetailID, int32_t nFlag = 1);
    void setDetailIDH   (uint32_t unDetailIDH);
    void setDetailIDL   (uint32_t unDetailIDL);
    void setUserID      (uint32_t unUserID   );
    void setRatingID    (uint32_t unRatingID );
    void setSubjectID   (uint16_t usSubjectID);
    void setAuditFlag   (uint16_t usAuditFlag);
    void setTotalPayAmount(int32_t nTotalPayAmount);

    virtual int showInfo();
private:

};

#endif // __AGIP_USER_IB_PAY_H__

