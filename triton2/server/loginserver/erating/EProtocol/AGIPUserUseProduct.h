#ifndef __AGIP_USER_USE_PRODUCT_H__
#define __AGIP_USER_USE_PRODUCT_H__

#include "SysProtocol.h"

#define CMD_USER_USE_PRODUCT            0x10003502
#define CMD_USER_USE_PRODUCT_RES        0x20003502

typedef struct _AGIP_USER_USE_PRODUCT_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    uint32_t        un_Purchase_ID_Higher;
    uint32_t        un_Purchase_ID_Lower;
    uint8_t         c_Role_Gender;
    uint8_t         c_Role_Occupation;
    uint16_t        us_Role_Level;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserUseProduct, *PSAGIPUserUseProduct;


class Engine_Export AGIPUserUseProduct :
    public SysProtocol
{
public:
    AGIPUserUseProduct(void);
    virtual ~AGIPUserUseProduct(void);

    int getUserID       (uint32_t   *punUserID      );
    int getRoleID       (uint32_t   *punRoleID      );
    int getPurchaseID   (uint64_t   *pullPurchaseID, int32_t flag = 1);
    int getPurchaseID   (PURCHASE_ID *psPurchaseID  );
    int getPurchaseID   (uint32_t   *punPurchaseIDHigher, uint32_t *punPurchaseIDLower);
    int getRoleGender   (int8_t     *pcGender       );
    int getRoleOccupation(int8_t    *pcOccupation   );
    int getRoleLevel    (uint16_t   *pusLevel       );
    
    int setUserID       (uint32_t   unUserID        );
    int setRoleID       (uint32_t   unRoleID        );
    int setPurchaseID   (uint64_t   ullPurchaseID, int32_t flag = 1);
    int setPurchaseID   (const PURCHASE_ID& sPurchaseID);
    int setPurchaseID   (uint32_t unPurchaseIDHigher, uint32_t unPurchaseIDLower);
    int setRoleGender   (int8_t     cGender         );
    int setRoleOccupation(int8_t    cOccupation     );
    int setRoleLevel    (uint16_t   usLevel         );

    virtual int showInfo();
};


typedef struct _AGIP_USER_USE_PRODUCT_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserUseProductRes, *PSAGIPUserUseProductRes;


class Engine_Export AGIPUserUseProductRes :
    public SysProtocol
{
public:
    AGIPUserUseProductRes(void);
    virtual ~AGIPUserUseProductRes(void);

    int getResultCode(int32_t *pnResultCode);

    int setResultCode(int32_t nResultCode);

    virtual int showInfo();
};

#endif
