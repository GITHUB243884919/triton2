#ifndef __AGIP_USER_ITEM_LIST_H__
#define __AGIP_USER_ITEM_LIST_H__

#include "SysProtocol.h"

#define CMD_USER_ITEM_LIST              0x10003508
#define CMD_USER_ITEM_LIST_RES          0x20003508

typedef struct _AGIP_USER_ITEM_LIST_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Item_Gateway_ID;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    uint32_t        un_Activity_ID;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserItemList, *PSAGIPUserItemList;

typedef struct _AGIP_USER_ITEM_LIST_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    int32_t         n_Item_Count;
    uint32_t        un_Activity_ID;
    char            str_Item_Code[AGIP_ITEM_CODE_LEN];
    int32_t         n_Item_Num;
    uint32_t        un_Begin_Time;
    uint32_t        un_End_Time;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserItemListRes, *PSAGIPUserItemListRes;

class Engine_Export AGIPUserItemList :
    public SysProtocol
{
public:
    AGIPUserItemList(void);
public:
    virtual ~AGIPUserItemList(void);
    int getUserID(uint32_t *punUserID);
    int getRoleID(uint32_t *punRoleID);
    int getActivityID(uint32_t *punActivityID);
    int getItemGatewayID(uint32_t *punItemGatewayID);

    int setUserID(uint32_t unUserID);
    int setRoleID(uint32_t unRoleID);
    int setActivityID(uint32_t unActivityID);
    int setItemGatewayID(uint32_t unItemGatewayID);

    virtual int showInfo();
};

class Engine_Export AGIPUserItemListRes :
    public SysProtocol
{
    friend class AGIPUserItemList;
public:
    AGIPUserItemListRes(void);
    virtual int initialize();
public:
    virtual ~AGIPUserItemListRes(void);

    int getResultCode(int32_t *pnResultCode);
    int getItemCount(int32_t *pnItemCount);
    int getItemInfo(
            int32_t nIndex, 
            uint32_t* punActivityID,
            char*strItemCode, 
            int32_t* pnItemNum,
            uint32_t* punBeginTime,
            uint32_t* punEndTime);

    int setResultCode(int32_t nResultCode);
    int addItemInfo(
            uint32_t unActivityID, 
            const char *strItemCode, 
            int32_t nItemNum,
            uint32_t unBeginTime,
            uint32_t unEndTime);

    virtual int showInfo();
private:
    //int setItemCount(int32_t nItemCount);
};

#endif

