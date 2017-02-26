#ifndef __AGIP_RENAME_ROLE_H__
#define __AGIP_RENAME_ROLE_H__

#include "SysProtocol.h"

#define CMD_RENAME_ROLE                 0x10003107
#define CMD_RENAME_ROLE_RES             0x20003107

typedef struct _AGIP_RENAME_ROLE_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    char            sz_Role_Name[AGIP_ROLE_NAME_LEN];
    _AGIP_CHECKSUM_ checksum;
} SAGIPRenameRole, *PSAGIPRenameRole;

typedef struct _AGIP_RENAME_ROLE_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPRenameRoleRes, *PSAGIPRenameRoleRes;

class Engine_Export AGIPRenameRole :
    public SysProtocol
{
public:
    AGIPRenameRole(void);

    uint32_t getUserID();
    uint32_t getRoleID();
	char* getRoleName(char* buffer, int nbyte = AGIP_ROLE_NAME_LEN);
    
    void setUserID(uint32_t unUserID);
    void setRoleID(uint32_t unRoleID);
	void setRoleName(const char *szRoleName);

    virtual int showInfo();

public:
    virtual ~AGIPRenameRole(void);
};

class Engine_Export AGIPRenameRoleRes :
    public SysProtocol
{
    friend class AGIPRenameRole;
public:
    AGIPRenameRoleRes(void);

    int32_t getResultCode();

    void setResultCode(int32_t nResultCode);

    virtual int showInfo();

public:
    virtual ~AGIPRenameRoleRes(void);
};

#endif

