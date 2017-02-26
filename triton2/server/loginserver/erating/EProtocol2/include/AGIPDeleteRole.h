#ifndef __AGIP_DELETE_ROLE_H__
#define __AGIP_DELETE_ROLE_H__

#include "SysProtocol.h"

#define CMD_DELETE_ROLE                 0x10003103
#define CMD_DELETE_ROLE_RES             0x20003103

typedef struct _AGIP_DELETE_ROLE_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    _AGIP_CHECKSUM_ checksum;
} SAGIPDeleteRole, *PSAGIPDeleteRole;

typedef struct _AGIP_DELETE_ROLE_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPDeleteRoleRes, *PSAGIPDeleteRoleRes;

class Engine_Export AGIPDeleteRole :
    public SysProtocol
{
public:
    AGIPDeleteRole(void);

    int getUserID(uint32_t *punUserID);
    int getRoleID(uint32_t *punRoleID);
    
    int setUserID(uint32_t unUserID);
    int setRoleID(uint32_t unRoleID);

    virtual int showInfo();

public:
    virtual ~AGIPDeleteRole(void);
};

class Engine_Export AGIPDeleteRoleRes :
    public SysProtocol
{
    friend class AGIPDeleteRole;
public:
    AGIPDeleteRoleRes(void);

    int getResultCode(int32_t *pnResultCode);

    int setResultCode(int32_t nResultCode);

    virtual int showInfo();

public:
    virtual ~AGIPDeleteRoleRes(void);
};

#endif

