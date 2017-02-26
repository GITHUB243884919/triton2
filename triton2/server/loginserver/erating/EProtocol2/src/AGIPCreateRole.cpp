#include "AGIPCreateRole.h"

AGIPCreateRole::AGIPCreateRole(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_CREATE_ROLE, sizeof(SAGIPCreateRole))
{
}

AGIPCreateRole::~AGIPCreateRole(void)
{
}

int AGIPCreateRole::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    *punUserID = htonl(pSAGIPCreateRole->un_User_ID);
    return S_SUCCESS;
}

int AGIPCreateRole::getRoleName(char *strRoleName)
{
    ASSERT(strRoleName != NULL);
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    strncpy(strRoleName, pSAGIPCreateRole->str_Role_Name, AGIP_ROLE_NAME_LEN);
    return S_SUCCESS;
}


int AGIPCreateRole::getGender(int8_t *pcGender)
{
    ASSERT(pcGender != NULL);
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    *pcGender = pSAGIPCreateRole->c_Gender;
    return S_SUCCESS;
}

int AGIPCreateRole::getOccupation(int8_t *pcOccupation)
{
    ASSERT(pcOccupation != NULL);
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    *pcOccupation = pSAGIPCreateRole->c_Occupation;
    return S_SUCCESS;
}

/* 
int AGIPCreateRole::getInitialLevel(int8_t *pcInitialLevel)
{
    ASSERT(pcInitialLevel != NULL);
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    *pcInitialLevel = pSAGIPCreateRole->c_Initial_Level;
    return S_SUCCESS;
}
*/

int AGIPCreateRole::getInitialLevel(uint16_t* pusInitialLevel)
{
    ASSERT(pusInitialLevel != NULL);
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    *pusInitialLevel = ntohs(pSAGIPCreateRole->us_Initial_Level);
    return S_SUCCESS;
}

int AGIPCreateRole::getCommunityID(uint8_t *pucCommunityID)
{
    ASSERT(pucCommunityID != NULL);
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    *pucCommunityID = pSAGIPCreateRole->uc_Community_ID;
    return S_SUCCESS;
}

int AGIPCreateRole::getUserIP(uint32_t *punUserIP)
{
    ASSERT(punUserIP != NULL);
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    *punUserIP = htonl(pSAGIPCreateRole->un_User_IP);
    return S_SUCCESS;
}

int AGIPCreateRole::getUserPort(uint16_t *pusUserPort)
{
    ASSERT(pusUserPort != NULL);
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    *pusUserPort = htons(pSAGIPCreateRole->us_User_Port);
    return S_SUCCESS;
}
//////////
// Setter

int AGIPCreateRole::setUserID(uint32_t unUserID)
{
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    pSAGIPCreateRole->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPCreateRole::setRoleName(const char *strRoleName)
{
    ASSERT(strRoleName != NULL);
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    strncpy(pSAGIPCreateRole->str_Role_Name, strRoleName, AGIP_ROLE_NAME_LEN);
    return S_SUCCESS;
}

int AGIPCreateRole::setGender(int8_t cGender)
{
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    pSAGIPCreateRole->c_Gender = cGender;
    return S_SUCCESS;
}

int AGIPCreateRole::setOccupation(int8_t cOccupation)
{
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    pSAGIPCreateRole->c_Occupation = cOccupation;
    return S_SUCCESS;
}

/*
int AGIPCreateRole::setInitialLevel(int8_t cInitialLevel)
{
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    pSAGIPCreateRole->c_Initial_Level = cInitialLevel;
    return S_SUCCESS;
}
*/

int AGIPCreateRole::setInitialLevel(uint16_t usInitialLevel)
{
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    pSAGIPCreateRole->us_Initial_Level = htons(usInitialLevel);
    return S_SUCCESS;
}

int AGIPCreateRole::setCommunityID(uint8_t ucCommunityID)
{
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    pSAGIPCreateRole->uc_Community_ID = ucCommunityID;
    return S_SUCCESS;
}

int AGIPCreateRole::setUserIP(uint32_t unUserIP)
{
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    pSAGIPCreateRole->un_User_IP = htonl(unUserIP);
    return S_SUCCESS;
}

int AGIPCreateRole::setUserPort(uint16_t usUserPort)
{
    SAGIPCreateRole *pSAGIPCreateRole = (SAGIPCreateRole *)m_pucPDU;
    pSAGIPCreateRole->us_User_Port = htons(usUserPort);
    return S_SUCCESS;
}

int AGIPCreateRole::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t unUserID      = 0;
    char     strRoleName[AGIP_ROLE_NAME_LEN + 1];
    int8_t   cGender       = 0;
    int8_t   cOccupation   = 0;
    // int8_t   cInitialLevel  = 0;
    uint16_t usInitialLevel = 0;
    uint8_t  ucCommunityID = 0;
    uint32_t unUserIP      = 0;
    uint16_t usUserPort    = 0;
    uint8_t  *pucIPSeg     = 0;

    SysProtocol::showInfo();
    nRetCode = getUserID(&unUserID);
    nRetCode = getRoleName(strRoleName);
    strRoleName[AGIP_ROLE_NAME_LEN] = '\0';
    nRetCode = getGender(&cGender);
    nRetCode = getOccupation(&cOccupation);
    // nRetCode = getInitialLevel(&cInitialLevel);
    nRetCode = getInitialLevel(&usInitialLevel);
    nRetCode = getCommunityID(&ucCommunityID);
    nRetCode = getUserIP(&unUserIP);
    nRetCode = getUserPort(&usUserPort);

    printf("--------------------------------------------------------AGIPCreateRole\n");
    printf("User_ID:\t%u\n", unUserID);
    printf("Role_Name:\t%s\n",  strRoleName);
    switch(cGender)
    {
    case GENDER_MALE:
        printf("Gender:\t%d(Male)\n", cGender);
        break;
    case GENDER_FEMALE:
        printf("Gender:\t%d(Female)\n", cGender);
        break;
    default:
        printf("Gender:\t%d(Others)\n", cGender);
    }
    printf("Occupation:\t%d\n", cOccupation);
    // printf("Initial_Level:\t%d\n", cIntialLevel);
    printf("Initial_Level:\t%u\n", usInitialLevel);
    printf("Community_ID:\t%u\n", ucCommunityID);
    
#if (defined(WIN32) || defined(LINUX))
    pucIPSeg = (uint8_t *)&unUserIP;
    printf("User_IP:\t%08X(%u.%u.%u.%u)\n",
        unUserIP,
        pucIPSeg[3], pucIPSeg[2], pucIPSeg[1], pucIPSeg[0]
    );
#else
    pucIPSeg = (uint8_t *)&unUserIP;
    printf("User_IP:\t%08X(%u.%u.%u.%u)\n",
        unUserIP,
        pucIPSeg[0], pucIPSeg[1], pucIPSeg[2], pucIPSeg[3]
    );
#endif
    printf("User_Port:\t%u\n", usUserPort);
    printf("--------------------------------------------------------AGIPCreateRole\n");
    return S_SUCCESS;
}
////////////////////
//AGIPCreateRoleRes

AGIPCreateRoleRes::AGIPCreateRoleRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_CREATE_ROLE_RES, sizeof(SAGIPCreateRoleRes))
{

}

AGIPCreateRoleRes::~AGIPCreateRoleRes(void)
{

}

int AGIPCreateRoleRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPCreateRoleRes *pSAGIPCreateRoleRes = (SAGIPCreateRoleRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPCreateRoleRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPCreateRoleRes::getRoleID(uint32_t *punRoleID)
{
    ASSERT(punRoleID != NULL);
    SAGIPCreateRoleRes *pSAGIPCreateRoleRes = (SAGIPCreateRoleRes *)m_pucPDU;
    *punRoleID = htonl(pSAGIPCreateRoleRes->un_Role_ID);
    return S_SUCCESS;
}

int AGIPCreateRoleRes::setResultCode(int32_t nResultCode)
{
    SAGIPCreateRoleRes *pSAGIPCreateRoleRes = (SAGIPCreateRoleRes *)m_pucPDU;
    pSAGIPCreateRoleRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPCreateRoleRes::setRoleID(uint32_t unRoleID)
{
    SAGIPCreateRoleRes *pSAGIPCreateRoleRes = (SAGIPCreateRoleRes *)m_pucPDU;
    pSAGIPCreateRoleRes->un_Role_ID = htonl(unRoleID);
    return S_SUCCESS;
}

int AGIPCreateRoleRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode = 0;
    uint32_t  unRoleID    = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);
    nRetCode = getRoleID(&unRoleID);

    printf("--------------------------------------------------------AGIPCreateRoleRes\n");
    printf("Result_Code:\t%d\n", nResultCode);
    printf("Role_ID:\t%u\n",  unRoleID);
   
    printf("--------------------------------------------------------AGIPCreateRoleRes\n");
    return S_SUCCESS;
}

