#ifndef __AGIP_USER_BUY_PRODUCT_H__
#define __AGIP_USER_BUY_PRODUCT_H__

#include "SysProtocol.h"

#define CMD_USER_BUY_PRODUCT            0x10003501
#define CMD_USER_BUY_PRODUCT_RES        0x20003501

typedef struct _AGIP_USER_BUY_PRODUCT_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    uint8_t         c_Role_Gender;
    uint8_t         c_Role_Occupation;
    uint16_t        us_Role_Level;
    char            str_IB_Code[AGIP_IB_CODE_LEN];
    int8_t          c_Package_Flag;
    int8_t          c_Pad;
    int16_t         s_Amount;
    uint32_t        un_Rating_ID;
    uint32_t        un_Server_ID;
    uint32_t        un_User_IP;
    int32_t         n_Subject_ID;
    int32_t         n_Price;
    int32_t         n_Discount_Price;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserBuyProduct, *PSAGIPUserBuyProduct;


class Engine_Export AGIPUserBuyProduct :
    public SysProtocol
{
public:
    enum
    {
        PACKAGE_FLAG_NORMAL      = 1,
        PACKAGE_FLAG_COMBINATION = 2

    };
    AGIPUserBuyProduct(void);
    virtual ~AGIPUserBuyProduct(void);
public:

    //virtual int initialize();

    int getUserID(uint32_t *punUserID);
    int getRoleID(uint32_t *punRoleID);
    int getRoleGender(int8_t *pcGender);
    int getRoleOccupation(int8_t *pcOccupation);
    int getRoleLevel(uint16_t *pusLevel);
    int getIBCode(char *strIBCode);
    int getPackageFlag(int8_t *pcPackageFlag);
    int getAmount(int16_t *psAmount);
    int getRatingID(uint32_t *punRatingID);
    int getServerID(uint32_t *punServerID);
    int getUserIP(uint32_t *punUserIP);
    int getSubjectID(int32_t *pnSubjectID);
    int getPrice(int32_t *pnPrice);
    int getDiscountPrice(int32_t *pnDiscountPrice);

    int setUserID(uint32_t unUserID);
    int setRoleID(uint32_t unRoleID);
    int setRoleGender(int8_t cGender);
    int setRoleOccupation(int8_t cOccupation);
    int setRoleLevel(uint16_t usLevel);
    int setIBCode(const char *strIBCode);
    int setPackageFlag(int8_t cPackageFlag);
    int setAmount(int16_t sAmount);
    int setRatingID(uint32_t unRatingID);
    int setServerID(uint32_t unServerID);
    int setUserIP(uint32_t unUserIP);
    int setSubjectID(int32_t nSubjectID);
    int setPrice(int32_t nPrice);
    int setDiscountPrice(int32_t nDiscountPrice);

    virtual int showInfo();
private:

};


typedef struct _AGIP_USER_BUY_PRODUCT_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    int32_t         n_Cost_Money;
    int32_t         n_Remain_Money;
    int32_t         n_IB_Count;
    uint32_t        un_Purchase_ID_Higher;
    uint32_t        un_Purchase_ID_Lower;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserBuyProductRes, *PSAGIPUserBuyProductRes;


class Engine_Export AGIPUserBuyProductRes :
    public SysProtocol
{
    friend class AGIPUserBuyProduct;
public:
    AGIPUserBuyProductRes(void);
    virtual ~AGIPUserBuyProductRes(void);
public:
    virtual int initialize();

    int getResultCode   (int32_t *pnResultCode);
    int getCostMoney    (int32_t *pnCostMoney);
    int getRemainMoney  (int32_t *pnRemainMoney);
    int getIBCount      (int32_t *pnIBCount);
    int getPurchaseID   (int32_t nIndex, uint64_t *pullPurchaseID);
    int getPurchaseID   (int32_t nIndex, PURCHASE_ID *pPurchaseID);
    int getPurchaseID   (int32_t nIndex, uint32_t *punPurchaseIDHigher, uint32_t *punPurchaseIDLower);

    int setResultCode(int32_t nResultCode);
    int setCostMoney(int32_t nCostMoney);
    int setRemainMoney(int32_t nRemainMoney);
    int addPurchaseID(uint64_t ullPurchaseID);
    int addPurchaseID(PURCHASE_ID *pPurchaseID);
    int addPurchaseID(uint32_t unPurchaseIDHigher, uint32_t unPurchaseIDLower);

    virtual int showInfo();
private:
    int setIBCount(int32_t nIBCount);
    
};




#endif
