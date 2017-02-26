#ifndef __AGIP_TRANSFER_GROUP_H__
#define __AGIP_TRANSFER_GROUP_H__

#include "SysProtocol.h"

#define CMD_TRANSFER_GROUP                 0x10003903
#define CMD_TRANSFER_GROUP_RES             0x20003903

typedef struct _AGIP_TRANSFER_GROUP_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Group_ID;
    uint32_t        un_From_Role_ID;
    uint32_t        un_To_Role_ID;
    _AGIP_CHECKSUM_ checksum;
} SAGIPTransferGroup, *PSAGIPTransferGroup;

typedef struct _AGIP_TRANSFER_GROUP_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPTransferGroupRes, *PSAGIPTransferGroupRes;


class Engine_Export AGIPTransferGroup :
    public SysProtocol
{
public:
    AGIPTransferGroup(void);
    virtual ~AGIPTransferGroup(void);

    uint32_t getGroupID(void);
    uint32_t getFromRoleID(void);
    uint32_t getToRoleID(void);

    void setGroupID(uint32_t unRoleID);
    void setFromRoleID(uint32_t unRoleID);
    void setToRoleID(uint32_t unRoleID);
    
    virtual int showInfo();

private:
    SAGIPTransferGroup* m_data;
};

class Engine_Export AGIPTransferGroupRes :
    public SysProtocol
{
public:
    AGIPTransferGroupRes(void);
    virtual ~AGIPTransferGroupRes(void);

    int32_t getResultCode(void);

    void setResultCode(int32_t nResultCode);

    virtual int showInfo();

private:
    SAGIPTransferGroupRes* m_data;
};

#endif
