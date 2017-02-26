#ifndef __AGIP_ACTIVITY_ITEM_QUERY_H__
#define __AGIP_ACTIVITY_ITEM_QUERY_H__

#include "SysProtocol.h"

#define CMD_ACTIVITY_ITEM_QUERY              0x10003509
#define CMD_ACTIVITY_ITEM_QUERY_RES          0x20003509

typedef struct _AGIP_ACTIVITY_ITEM_QUERY_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    uint32_t        un_Activity_ID;
    uint16_t        us_Role_Level;
    uint16_t        us_Pad1;
    uint32_t        un_Pad2;
    _AGIP_CHECKSUM_ checksum;
} SAGIPActivityItemQuery, *PSAGIPActivityItemQuery;

typedef struct _AGIP_ACTIVITY_ITEM_QUERY_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    int32_t         n_Item_Count;
    uint32_t        un_Activity_ID;
    char            sz_Item_Code[AGIP_ITEM_CODE_LEN];
    int32_t         n_Item_Num;
    uint32_t        un_Begin_Time;
    uint32_t        un_End_Time;
    _AGIP_CHECKSUM_ checksum;
} SAGIPActivityItemQueryRes, *PSAGIPActivityItemQueryRes;

class Engine_Export AGIPActivityItemQuery :
    public SysProtocol
{
public:
    AGIPActivityItemQuery(void);
    virtual ~AGIPActivityItemQuery(void);

    uint32_t getUserID();
    uint32_t getRoleID();
    uint32_t getActivityID();
    uint16_t getRoleLevel();

    void setUserID(uint32_t unUserID);
    void setRoleID(uint32_t unRoleID);
    void setActivityID(uint32_t unActivityID);
    void setRoleLevel(uint16_t usRoleLevel);

    virtual int showInfo();
};

class Engine_Export AGIPActivityItemQueryRes :
    public SysProtocol
{
    friend class AGIPActivityItemQuery;
public:
    AGIPActivityItemQueryRes(void);
    virtual ~AGIPActivityItemQueryRes(void);

    int32_t getResultCode();
    int32_t getItemCount();
    int getItemInfo(
            int32_t nIndex, 
            uint32_t* punActivityID,
            char* strItemCode, 
            int32_t* pnItemNum,
            uint32_t* punBeginTime,
            uint32_t* punEndTime);

    void setResultCode(int32_t nResultCode);
    int addItemInfo(
            uint32_t unActivityID, 
            const char* strItemCode, 
            int32_t nItemNum,
            uint32_t unBeginTime,
            uint32_t unEndTime);

    virtual int showInfo();
};

#endif

