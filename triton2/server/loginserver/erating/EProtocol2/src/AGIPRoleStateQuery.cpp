#include "AGIPRoleStateQuery.h"

AGIPRoleStateQuery::AGIPRoleStateQuery(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_ROLE_STATE_QUERY, sizeof(SAGIPRoleStateQuery))
{
}

AGIPRoleStateQuery::~AGIPRoleStateQuery(void)
{
}

uint32_t AGIPRoleStateQuery::getUserID(void)
{
    return (uint32_t)htonl(((SAGIPRoleStateQuery *)m_pucPDU)->un_User_ID);
}

const char* AGIPRoleStateQuery::getRoleName(void)
{
    return ((SAGIPRoleStateQuery *)m_pucPDU)->str_Role_Name;
}

// Setter
void AGIPRoleStateQuery::setUserID(uint32_t unUserID)
{
    SAGIPRoleStateQuery *pSAGIPRoleStateQuery = (SAGIPRoleStateQuery *)m_pucPDU;
    pSAGIPRoleStateQuery->un_User_ID = htonl(unUserID);
}

void AGIPRoleStateQuery::setRoleName(const char *strRoleName)
{
    ASSERT(strRoleName != NULL);
    SAGIPRoleStateQuery *pSAGIPRoleStateQuery = (SAGIPRoleStateQuery *)m_pucPDU;
    strncpy(pSAGIPRoleStateQuery->str_Role_Name, strRoleName, AGIP_ROLE_NAME_LEN);
}

int32_t AGIPRoleStateQuery::showInfo()
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPRoleStateQuery\n");
    printf("User_ID:    %u\n", getUserID());
    printf("Role_Name:  %s\n", getRoleName());
    printf("--------------------------------------------------------AGIPRoleStateQuery\n");
    return S_SUCCESS;
}
////////////////////
//AGIPRoleStateQueryRes

AGIPRoleStateQueryRes::AGIPRoleStateQueryRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_ROLE_STATE_QUERY_RES, sizeof(SAGIPRoleStateQueryRes))
{

}

AGIPRoleStateQueryRes::~AGIPRoleStateQueryRes(void)
{

}

int32_t AGIPRoleStateQueryRes::getResultCode(void)
{
    return htonl(((SAGIPRoleStateQueryRes *)m_pucPDU)->n_Result_Code);
}

uint32_t AGIPRoleStateQueryRes::getRoleState(void)
{
    return (uint32_t)htonl(((SAGIPRoleStateQueryRes *)m_pucPDU)->un_Role_State);
}

void AGIPRoleStateQueryRes::setResultCode(int32_t nResultCode)
{
    SAGIPRoleStateQueryRes *pSAGIPRoleStateQueryRes = (SAGIPRoleStateQueryRes *)m_pucPDU;
    pSAGIPRoleStateQueryRes->n_Result_Code = htonl(nResultCode);
}

void AGIPRoleStateQueryRes::setRoleState(uint32_t unRoleState)
{
    SAGIPRoleStateQueryRes *pSAGIPRoleStateQueryRes = (SAGIPRoleStateQueryRes *)m_pucPDU;
    pSAGIPRoleStateQueryRes->un_Role_State = htonl(unRoleState);
}

int32_t AGIPRoleStateQueryRes::showInfo()
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPRoleStateQueryRes\n");
    printf("Result_Code: %d\n", getResultCode());
    printf("Role_State:  %u\n", getRoleState());
    printf("--------------------------------------------------------AGIPRoleStateQueryRes\n");
    return S_SUCCESS;
}

