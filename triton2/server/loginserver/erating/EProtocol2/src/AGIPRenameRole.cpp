#include "AGIPRenameRole.h"

AGIPRenameRole::AGIPRenameRole(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_RENAME_ROLE, sizeof(SAGIPRenameRole))
{
}

AGIPRenameRole::~AGIPRenameRole(void)
{
}

uint32_t AGIPRenameRole::getUserID()
{
    return htonl( ( (SAGIPRenameRole*)m_pucPDU )->un_User_ID );
}

uint32_t AGIPRenameRole::getRoleID()
{
    return htonl( ( (SAGIPRenameRole*)m_pucPDU )->un_Role_ID );
}

char* AGIPRenameRole::getRoleName(char* buffer, int nbyte /*= AGIP_ROLE_NAME_LEN*/)
{
    if (buffer != NULL)
        strncpy( buffer, ( (SAGIPRenameRole*)m_pucPDU )->sz_Role_Name,
            ( (nbyte > AGIP_ROLE_NAME_LEN) ? AGIP_ROLE_NAME_LEN : nbyte ) );
    return buffer;
}


//////////
// Setter

void AGIPRenameRole::setUserID(uint32_t unUserID)
{
    ( (SAGIPRenameRole*)m_pucPDU )->un_User_ID = htonl(unUserID);
}

void AGIPRenameRole::setRoleID(uint32_t unRoleID)
{
    ( (SAGIPRenameRole*)m_pucPDU )->un_Role_ID = htonl(unRoleID);
}


void AGIPRenameRole::setRoleName(const char* szRoleName)
{
    if (szRoleName != NULL)
        strncpy( ( (SAGIPRenameRole*)m_pucPDU )->sz_Role_Name, szRoleName, AGIP_ROLE_NAME_LEN );
}


int AGIPRenameRole::showInfo()
{
    char szRoleName[AGIP_ROLE_NAME_LEN+1] = {0};
    SysProtocol::showInfo();
    memset(szRoleName, 0, sizeof(szRoleName));

    printf("--------------------------------------------------------AGIPRenameRole\n");
    printf("User_ID:\t%u\n", getUserID());
    printf("Role_ID:\t%u\n", getRoleID());
    printf("Role_Name:\t%s\n", getRoleName(szRoleName, AGIP_ROLE_NAME_LEN));
    printf("--------------------------------------------------------AGIPRenameRole\n");

    return S_SUCCESS;

}

//////////////////////////
//AGIPRenameRoleRes

AGIPRenameRoleRes::AGIPRenameRoleRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_RENAME_ROLE_RES, sizeof(SAGIPRenameRoleRes))
{

}

AGIPRenameRoleRes::~AGIPRenameRoleRes(void)
{

}

int32_t AGIPRenameRoleRes::getResultCode()
{
    return htonl( ( (SAGIPRenameRoleRes*)m_pucPDU )->n_Result_Code );
}

void AGIPRenameRoleRes::setResultCode(int32_t nResultCode)
{
    ( (SAGIPRenameRoleRes *)m_pucPDU )->n_Result_Code = htonl(nResultCode);
}

int AGIPRenameRoleRes::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPRenameRoleRes\n");
    printf("Result_Code:\t%d\n", getResultCode());
    printf("--------------------------------------------------------AGIPRenameRoleRes\n");

    return S_SUCCESS;
}
