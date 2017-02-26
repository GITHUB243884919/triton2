#ifndef __AGIP_USER_ITEM_MINUS_H__
#define __AGIP_USER_ITEM_MINUS_H__

#include "SysProtocol.h"

#define CMD_USER_ITEM_MINUS             0x10003505
#define CMD_USER_ITEM_MINUS_RES         0x20003505

typedef struct _AGIP_USER_ITEM_MINUS_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Item_Gateway_ID;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    uint32_t        un_Activity_ID;
    char            str_Item_Code[AGIP_ITEM_CODE_LEN];
    int32_t         n_Item_Num;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserItemMinus, *PSAGIPUserItemMinus;

typedef struct _AGIP_USER_ITEM_MINUS_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserItemMinusRes, *PSAGIPUserItemMinusRes;

class Engine_Export AGIPUserItemMinus :
    public SysProtocol
{
public:
    AGIPUserItemMinus(void);
public:
    virtual ~AGIPUserItemMinus(void);

    int getUserID(uint32_t *punUserID);
    int getRoleID(uint32_t *punRoleID);
    int getActivityID(uint32_t *punActivityID);
    int getItemCode(char *strItemCode);
    int getItemNum(int32_t *pnItemNum);
    int getItemGatewayID(uint32_t *punItemGatewayID);

    int setUserID(uint32_t unUserID);
    int setRoleID(uint32_t unRoleID);
    int setActivityID(uint32_t unActivityID);
    int setItemCode(const char *strItemCode);
    int setItemNum(int32_t nItemNum);
    int setItemGatewayID(uint32_t unItemGatewayID);

    virtual int showInfo();
};

class Engine_Export AGIPUserItemMinusRes :
    public SysProtocol
{
    friend class AGIPUserItemAdd;
public:
    AGIPUserItemMinusRes(void);
public:
    virtual ~AGIPUserItemMinusRes(void);

    int getResultCode(int32_t *pnResultCode);
    int setResultCode(int32_t nResultCode);

    virtual int showInfo();
};

#endif
