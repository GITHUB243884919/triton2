#include "AGIPDeleteRole.h"

AGIPDeleteRole::AGIPDeleteRole(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_DELETE_ROLE, sizeof(SAGIPDeleteRole))
{
}

AGIPDeleteRole::~AGIPDeleteRole(void)
{
}


int AGIPDeleteRole::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPDeleteRole *pSAGIPDeleteRole = (SAGIPDeleteRole *)m_pucPDU;
    *punUserID = htonl(pSAGIPDeleteRole->un_User_ID);
    return S_SUCCESS;
}

int AGIPDeleteRole::getRoleID(uint32_t *punRoleID)
{
    ASSERT(punRoleID != NULL);
    SAGIPDeleteRole *pSAGIPDeleteRole = (SAGIPDeleteRole *)m_pucPDU;
    *punRoleID = htonl(pSAGIPDeleteRole->un_Role_ID);
    return S_SUCCESS;
}

//////////
// Setter

int AGIPDeleteRole::setUserID(uint32_t unUserID)
{
    SAGIPDeleteRole *pSAGIPDeleteRole = (SAGIPDeleteRole *)m_pucPDU;
    pSAGIPDeleteRole->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPDeleteRole::setRoleID(uint32_t unRoleID)
{
    SAGIPDeleteRole *pSAGIPDeleteRole = (SAGIPDeleteRole *)m_pucPDU;
    pSAGIPDeleteRole->un_Role_ID = htonl(unRoleID);
    return S_SUCCESS;
}

int AGIPDeleteRole::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t   unUserID = 0;
    uint32_t   unRoleID = 0;
    

    SysProtocol::showInfo();
    nRetCode = getUserID(&unUserID);
    nRetCode = getRoleID(&unRoleID);

    printf("--------------------------------------------------------AGIPDeleteRole\n");
    printf("User_ID:\t%u\n", unUserID);
    printf("Role_ID:\t%u\n", unRoleID);
    printf("--------------------------------------------------------AGIPDeleteRole\n");
    return S_SUCCESS;
}

//////////////////////////
//AGIPDeleteRoleRes

AGIPDeleteRoleRes::AGIPDeleteRoleRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_DELETE_ROLE_RES, sizeof(SAGIPDeleteRoleRes))
{

}

AGIPDeleteRoleRes::~AGIPDeleteRoleRes(void)
{

}

int AGIPDeleteRoleRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPDeleteRoleRes *pSAGIPDeleteRoleRes = (SAGIPDeleteRoleRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPDeleteRoleRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPDeleteRoleRes::setResultCode(int32_t nResultCode)
{
    SAGIPDeleteRoleRes *pSAGIPDeleteRoleRes = (SAGIPDeleteRoleRes *)m_pucPDU;
    pSAGIPDeleteRoleRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPDeleteRoleRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);

    printf("--------------------------------------------------------AGIPCreateRoleRes\n");
    printf("Result_Code:\t%d\n", nResultCode);

    printf("--------------------------------------------------------AGIPCreateRoleRes\n");
    return S_SUCCESS;
}
