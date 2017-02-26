#include "AGIPUserAuth.h"
#include <time.h>
AGIPUserAuth::AGIPUserAuth(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_AUTH, sizeof(SAGIPUserAuth))
{
}

AGIPUserAuth::~AGIPUserAuth(void)
{
}


int AGIPUserAuth::getUserName(char *strUserName)
{
    ASSERT(strUserName != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth *)m_pucPDU;
    memcpy(strUserName, pSAGIPUserAuth->str_User_Name, AGIP_USER_NAME_LEN);
    return S_SUCCESS;
}

int AGIPUserAuth::getPassword(char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth *)m_pucPDU;
    memcpy(strPassword, pSAGIPUserAuth->str_Password, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuth::getPasswordType(int8_t *pcPasswordType)
{
    ASSERT(pcPasswordType != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth *)m_pucPDU;
    *pcPasswordType = pSAGIPUserAuth->c_Password_Type;
    return S_SUCCESS;
}

int AGIPUserAuth::getUserPort(uint16_t *pusUserPort)
{
    ASSERT(pusUserPort != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth *)m_pucPDU;
    *pusUserPort = htons(pSAGIPUserAuth->us_User_Port);
    return S_SUCCESS;
}

int AGIPUserAuth::getUserIP(uint32_t *punUserIP)
{
    ASSERT(punUserIP != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth *)m_pucPDU;
    *punUserIP = htonl(pSAGIPUserAuth->un_User_IP);
    return S_SUCCESS;
}

int AGIPUserAuth::getMatrixPassword(char* szMatrixPassword)
{
    ASSERT(szMatrixPassword != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth*)m_pucPDU;
    memcpy(szMatrixPassword, pSAGIPUserAuth->str_Matrix_Password, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuth::getMatrixCoord(char* szMatrixCoord)
{
    ASSERT(szMatrixCoord != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth*)m_pucPDU;
    memcpy(szMatrixCoord, pSAGIPUserAuth->str_Matrix_Coord, AGIP_MATRIX_COORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuth::getMAC(char *strMAC)
{
    ASSERT(strMAC != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth *)m_pucPDU;
    strncpy(strMAC, pSAGIPUserAuth->sz_MAC, AGIP_MAC_LEN);
    return S_SUCCESS;
}

////////////////////////////
/// Setter
int AGIPUserAuth::setUserName(const char *strUserName)
{
    ASSERT(strUserName != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth *)m_pucPDU;
    strncpy(pSAGIPUserAuth->str_User_Name, strUserName, AGIP_USER_NAME_LEN);
    return S_SUCCESS;
}

int AGIPUserAuth::setPassword(const char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth *)m_pucPDU;
    strncpy(pSAGIPUserAuth->str_Password, strPassword, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuth::setPasswordType(int8_t cPasswordType)
{
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth *)m_pucPDU;
    pSAGIPUserAuth->c_Password_Type = cPasswordType;
    return S_SUCCESS;
}

int AGIPUserAuth::setUserPort(uint16_t usUserPort)
{
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth *)m_pucPDU;
    pSAGIPUserAuth->us_User_Port = htons(usUserPort);
    return S_SUCCESS;
}

int AGIPUserAuth::setUserIP(uint32_t unUserIP)
{
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth *)m_pucPDU;
    pSAGIPUserAuth->un_User_IP = htonl(unUserIP);
    return S_SUCCESS;
}

int AGIPUserAuth::setMatrixPassword(const char* szMatrixPassword)
{
    ASSERT(szMatrixPassword != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth*)m_pucPDU;
    strncpy(pSAGIPUserAuth->str_Matrix_Password, szMatrixPassword, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuth::setMatrixCoord(const char* szMatrixCoord)
{
    ASSERT(szMatrixCoord != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth*)m_pucPDU;
    strncpy(pSAGIPUserAuth->str_Matrix_Coord, szMatrixCoord, AGIP_MATRIX_COORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuth::setMAC(const char* szMAC)
{
    ASSERT(szMAC != NULL);
    SAGIPUserAuth *pSAGIPUserAuth = (SAGIPUserAuth*)m_pucPDU;
    strncpy(pSAGIPUserAuth->sz_MAC, szMAC, AGIP_MAC_LEN);
    return S_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
int AGIPUserAuth::showInfo()
{
    int nRetCode        = E_ERROR;

    char strUserName[AGIP_USER_NAME_LEN + 1];
    char strPassword[AGIP_PASSWORD_LEN + 1];
    char szMatrixPassword[AGIP_PASSWORD_LEN + 1];
    char szMatrixCoord[AGIP_MATRIX_COORD_LEN + 1];
    char szMAC[AGIP_MAC_LEN + 1];
    int8_t cPasswordType = 0;
    uint32_t unUserIP    = 0;
    uint16_t usUserPort  = 0;
    uint8_t *pucIPSeg    = 0;

    SysProtocol::showInfo();
    nRetCode = getUserName(strUserName);
    strUserName[AGIP_USER_NAME_LEN] = '\0';
    nRetCode = getPassword(strPassword);
    strPassword[AGIP_PASSWORD_LEN] = '\0';

    nRetCode = getPasswordType(&cPasswordType);
    nRetCode = getUserIP(&unUserIP);
    nRetCode = getUserPort(&usUserPort);
    memset(szMAC, 0, sizeof(szMAC));
    nRetCode = getMAC(szMAC);
    szMAC[sizeof(szMAC) - 1] = '\0';

    printf("--------------------------------------------------------AGIPUserAuth\n");
    printf("User_Name:\t%s\n", strUserName);
    printf("Password:\t%s\n", strPassword);

    switch(cPasswordType)
    {
    case PASSWORD_TYPE_AUTHENTICATION:
        printf("Password_Type:\t%d(Authentication)\n", cPasswordType);
        break;
    case PASSWORD_TYPE_SECURITY:
        printf("Password_Type:\t%d(Security-Code Verify)\n", cPasswordType);
        break;
    case PASSWORD_TYPE_MATRIX:
        printf("Password_Type:\t%d(Matrix-Password-Card)\n", cPasswordType);
        nRetCode = getMatrixPassword(szMatrixPassword);
        szMatrixPassword[AGIP_PASSWORD_LEN] = '\0';
        nRetCode = getMatrixCoord(szMatrixCoord);
        szMatrixCoord[AGIP_MATRIX_COORD_LEN] = '\0';
        printf("Matrix_Password:\t%s\n", szMatrixPassword);
        printf("Matrix_Coord:\t%s\n", szMatrixCoord);
        break;
    default:
        printf("Password_Type:\t%d(Others)\n", cPasswordType);
        break;
    }

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
    printf("MAC      :\t%s\n", szMAC);
    printf("--------------------------------------------------------AGIPUserAuth\n");
    return S_SUCCESS;
}


////////////////////////
// AGIPUserAuthRes
AGIPUserAuthRes::AGIPUserAuthRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_AUTH_RES, sizeof(SAGIPUserAuthRes))
{

}

AGIPUserAuthRes::~AGIPUserAuthRes()
{

}


int AGIPUserAuthRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserAuthRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserAuthRes::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserAuthRes->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserAuthRes::getUserType(int8_t *pcUserType)
{
    ASSERT(pcUserType != NULL);
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    *pcUserType = pSAGIPUserAuthRes->c_User_Type;
    return S_SUCCESS;
}

int AGIPUserAuthRes::getAdultFlag(int8_t *pcAdultFlag)
{
    ASSERT(pcAdultFlag != NULL);
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    *pcAdultFlag = pSAGIPUserAuthRes->c_Adult_Flag;
    return S_SUCCESS;
}

int AGIPUserAuthRes::getUserClass(int8_t *pcUserClass)
{
    ASSERT(pcUserClass != NULL);
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    *pcUserClass = pSAGIPUserAuthRes->c_User_Class;
    return S_SUCCESS;
}


int AGIPUserAuthRes::getUserPoint(int32_t *pnUserPoint)
{
    ASSERT(pnUserPoint != NULL);
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    *pnUserPoint = htonl(pSAGIPUserAuthRes->n_User_Point);
    return S_SUCCESS;
}

int AGIPUserAuthRes::getPromoterID(uint32_t *punPromoterID)
{
    ASSERT(punPromoterID != NULL);
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    *punPromoterID = htonl(pSAGIPUserAuthRes->un_Promoter_ID);
    return S_SUCCESS;
}

int AGIPUserAuthRes::getUserFlag(uint32_t *punUserFlag)
{
    ASSERT(punUserFlag != NULL);
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    *punUserFlag = htonl(pSAGIPUserAuthRes->un_User_Flag);
    return S_SUCCESS;
}

int AGIPUserAuthRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    pSAGIPUserAuthRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPUserAuthRes::setUserID(uint32_t unUserID)
{
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    pSAGIPUserAuthRes->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserAuthRes::setUserType(int8_t cUserType)
{
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    pSAGIPUserAuthRes->c_User_Type = cUserType;
    return S_SUCCESS;
}

int AGIPUserAuthRes::setAdultFlag(int8_t cAdultFlag)
{
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    pSAGIPUserAuthRes->c_Adult_Flag = cAdultFlag;
    return S_SUCCESS;
}

int AGIPUserAuthRes::setUserClass(int8_t cUserClass)
{
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    pSAGIPUserAuthRes->c_User_Class = cUserClass;
    return S_SUCCESS;
}



int AGIPUserAuthRes::setUserPoint(int32_t nUserPoint)
{
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    pSAGIPUserAuthRes->n_User_Point = htonl(nUserPoint);
    return S_SUCCESS;
}

int AGIPUserAuthRes::setPromoterID(uint32_t unPromoterID)
{
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    pSAGIPUserAuthRes->un_Promoter_ID = htonl(unPromoterID);
    return S_SUCCESS;
}

int AGIPUserAuthRes::setUserFlag(uint32_t unUserFlag)
{
    SAGIPUserAuthRes *pSAGIPUserAuthRes = (SAGIPUserAuthRes *)m_pucPDU;
    pSAGIPUserAuthRes->un_User_Flag = htonl(unUserFlag);
    return S_SUCCESS;
}

int AGIPUserAuthRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t  nResultCode = 0;
    uint32_t unUserID    = 0;
    int8_t   cUserType   = 0;
    int8_t   cAdultFlag  = 0;
    int8_t   cUserClass  = 0;
    int8_t   cUserRoleCount = 0;
    int32_t  nUserPoint     = 0;
    uint32_t unPromoterID   = 0;
    uint32_t unUserFlag     = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);
    nRetCode = getUserID(&unUserID);
    nRetCode = getUserType(&cUserType);
    nRetCode = getAdultFlag(&cAdultFlag);
    nRetCode = getUserClass(&cUserClass);
    nRetCode = getUserPoint(&nUserPoint);
    nRetCode = getPromoterID(&unPromoterID);
    nRetCode = getUserFlag(&unUserFlag);

    printf("--------------------------------------------------------AGIPUserAuthRes\n");
    printf("Result_Code:        %d\n", nResultCode);
    printf("User_ID:            %u\n", unUserID);
    printf("User_Type:          %d\n", cUserType);
    printf("Adult_Flag:         %d\n", cAdultFlag);
    printf("User_Class:         %d\n", cUserClass);
    printf("User_Role_Count:    %d\n", cUserRoleCount);
    printf("User_Point:         %d\n", nUserPoint);
    printf("Promoter_ID:        %u\n", unPromoterID);
    printf("User_Flag:          %u\n", unUserFlag);
    printf("--------------------------------------------------------AGIPUserAuthRes\n");
    return S_SUCCESS;
}


