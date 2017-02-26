#ifndef __AGIP_DELETE_GROUP_H__
#define __AGIP_DELETE_GROUP_H__

#include "SysProtocol.h"

#define CMD_DELETE_GROUP                 0x10003902
#define CMD_DELETE_GROUP_RES             0x20003902

typedef struct _AGIP_DELETE_GROUP_
{
    _AGIP_HEADER_   header;
    uint16_t        us_Game_ID;
    uint16_t        us_Pad_1;
    uint32_t        un_Group_ID;
    uint32_t        un_Role_ID;
    _AGIP_CHECKSUM_ checksum;
} SAGIPDeleteGroup, *PSAGIPDeleteGroup;

typedef struct _AGIP_DELETE_GROUP_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPDeleteGroupRes, *PSAGIPDeleteGroupRes;


class Engine_Export AGIPDeleteGroup :
    public SysProtocol
{
public:
    AGIPDeleteGroup(void);
    virtual ~AGIPDeleteGroup(void);

    uint16_t getGameID(void);
    uint32_t getGroupID(void);
    uint32_t getRoleID(void);

    void setGameID(uint16_t usGameID);
    void setGroupID(uint32_t unRoleID);
    void setRoleID(uint32_t unRoleID);
    
    virtual int showInfo();

private:
    SAGIPDeleteGroup* m_data;
};

class Engine_Export AGIPDeleteGroupRes :
    public SysProtocol
{
public:
    AGIPDeleteGroupRes(void);
    virtual ~AGIPDeleteGroupRes(void);

    int32_t getResultCode(void);

    void setResultCode(int32_t nResultCode);

    virtual int showInfo();

private:
    SAGIPDeleteGroupRes* m_data;
};

#endif
