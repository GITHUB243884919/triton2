#ifndef __AGIP_CREATE_GROUP_H__
#define __AGIP_CREATE_GROUP_H__

#include "SysProtocol.h"

#define CMD_CREATE_GROUP                 0x10003901
#define CMD_CREATE_GROUP_RES             0x20003901

const uint32_t AGIP_GROUP_NAME_LEN = 32;

typedef struct _AGIP_CREATE_GROUP_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Role_ID;
    char            str_Group_Name[AGIP_GROUP_NAME_LEN];
    uint8_t         uc_Group_Type;
    uint8_t         uc_Pad_2;
    uint8_t         uc_Pad_3;
    _AGIP_CHECKSUM_ checksum;
} SAGIPCreateGroup, *PSAGIPCreateGroup;

typedef struct _AGIP_CREATE_GROUP_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    uint32_t        un_Group_ID;
    _AGIP_CHECKSUM_ checksum;
} SAGIPCreateGroupRes, *PSAGIPCreateGroupRes;


class Engine_Export AGIPCreateGroup :
    public SysProtocol
{
public:
    AGIPCreateGroup(void);
    virtual ~AGIPCreateGroup(void);

    uint32_t getRoleID(void);
    const char* getGroupName(char* szRoleNameBuf, size_t unBufSize);
    uint8_t getGroupType(void);

    void setRoleID(uint32_t unRoleID);
    void setGroupName(const char* szRoleName);
    void setGroupType(uint8_t ucGroupType);
    
    virtual int showInfo();

private:
    SAGIPCreateGroup* m_data;
};

class Engine_Export AGIPCreateGroupRes :
    public SysProtocol
{
public:
    AGIPCreateGroupRes(void);
    virtual ~AGIPCreateGroupRes(void);

    int32_t getResultCode(void);
    uint32_t getGroupID(void);

    void setResultCode(int32_t nResultCode);
    void setGroupID(uint32_t unGroupID);

    virtual int showInfo();

private:
    SAGIPCreateGroupRes* m_data;
};

#endif
