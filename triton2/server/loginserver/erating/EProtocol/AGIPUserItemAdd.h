#ifndef __AGIP_USER_ITEM_ADD__
#define __AGIP_USER_ITEM_ADD__

#include "SysProtocol.h"

#define CMD_USER_ITEM_ADD               0x10003504
#define CMD_USER_ITEM_ADD_RES           0x20003504

typedef struct _AGIP_USER_ITEM_ADD_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Item_Gateway_ID;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    uint32_t        un_Activity_ID;
    char            str_Item_Code[AGIP_ITEM_CODE_LEN];
    int32_t         n_Item_Num;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserItemAdd, *PSAGIPUserItemAdd;

typedef struct _AGIP_USER_ITEM_ADD_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserItemAddRes, *PSAGIPUserItemAddRes;

class Engine_Export AGIPUserItemAdd :
    public SysProtocol
{
public:
    AGIPUserItemAdd(void);
public:
    virtual ~AGIPUserItemAdd(void);

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
};

class Engine_Export AGIPUserItemAddRes :
    public SysProtocol
{
    friend class AGIPUserItemAdd;
public:
    AGIPUserItemAddRes(void);
public:
    virtual ~AGIPUserItemAddRes(void);

    int getResultCode(int32_t *pnResultCode);
    int setResultCode(int32_t nResultCode);
};

#endif

