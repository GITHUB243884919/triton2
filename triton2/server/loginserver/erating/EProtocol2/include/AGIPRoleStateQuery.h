#ifndef __AGIP_ROLE_STATE_QUERY_H_
#define __AGIP_ROLE_STATE_QUERY_H_

#include "SysProtocol.h"

#define CMD_ROLE_STATE_QUERY                 0x10003104
#define CMD_ROLE_STATE_QUERY_RES             0x20003104

typedef struct _AGIP_ROLE_STATE_QUERY_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    char            str_Role_Name[AGIP_ROLE_NAME_LEN];
    _AGIP_CHECKSUM_ checksum;
} SAGIPRoleStateQuery, *PSAGIPRoleStateQuery;

typedef struct _AGIP_ROLE_STATE_QUERY_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    uint32_t        un_Role_State;
    _AGIP_CHECKSUM_ checksum;
} SAGIPRoleStateQueryRes, *PSAGIPRoleStateQueryRes;


class Engine_Export AGIPRoleStateQuery :
    public SysProtocol
{
public:
    AGIPRoleStateQuery();
    virtual ~AGIPRoleStateQuery();

    uint32_t getUserID();
    const char* getRoleName();

    void setUserID(uint32_t unUserID);
    void setRoleName(const char *strRoleName);
    
    virtual int32_t showInfo();
};

class Engine_Export AGIPRoleStateQueryRes :
    public SysProtocol
{
public:
    AGIPRoleStateQueryRes();
    virtual ~AGIPRoleStateQueryRes();

    int32_t getResultCode();
    uint32_t getRoleState();

    void setResultCode(int32_t nResultCode);
    void setRoleState(uint32_t unRoleState);

    virtual int32_t showInfo();
};

#endif // __AGIP_ROLE_STATE_QUERY_H_

