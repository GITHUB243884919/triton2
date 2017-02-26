#include "AGIPUserLogin.h"
#include <time.h>
AGIPUserLogin::AGIPUserLogin(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_LOGIN, sizeof(SAGIPUserLogin))
{
}

AGIPUserLogin::~AGIPUserLogin(void)
{
}


int AGIPUserLogin::getUserName(char *strUserName)
{
    ASSERT(strUserName != NULL);
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin *)m_pucPDU;
    memcpy(strUserName, pSAGIPUserLogin->str_User_Name, AGIP_USER_NAME_LEN);
    return S_SUCCESS;
}

int AGIPUserLogin::getPassword(char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin *)m_pucPDU;
    memcpy(strPassword, pSAGIPUserLogin->str_Password, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserLogin::getPasswordType(int8_t *pcPasswordType)
{
    ASSERT(pcPasswordType != NULL);
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin *)m_pucPDU;
    *pcPasswordType = pSAGIPUserLogin->c_Password_Type;
    return S_SUCCESS;
}

int AGIPUserLogin::getUserPort(uint16_t *pusUserPort)
{
    ASSERT(pusUserPort != NULL);
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin *)m_pucPDU;
    *pusUserPort = htons(pSAGIPUserLogin->us_User_Port);
    return S_SUCCESS;
}

int AGIPUserLogin::getUserIP(uint32_t *punUserIP)
{
    ASSERT(punUserIP != NULL);
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin *)m_pucPDU;
    *punUserIP = htonl(pSAGIPUserLogin->un_User_IP);
    return S_SUCCESS;
}

int AGIPUserLogin::getMatrixPassword(char* szMatrixPassword)
{
    ASSERT(szMatrixPassword != NULL);
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin*)m_pucPDU;
    memcpy(szMatrixPassword, pSAGIPUserLogin->str_Matrix_Password, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserLogin::getMatrixCoord(char* szMatrixCoord)
{
    ASSERT(szMatrixCoord != NULL);
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin*)m_pucPDU;
    memcpy(szMatrixCoord, pSAGIPUserLogin->str_Matrix_Coord, AGIP_MATRIX_COORD_LEN);
    return S_SUCCESS;
}
////////////////////////////
/// Setter
int AGIPUserLogin::setUserName(const char *strUserName)
{
    ASSERT(strUserName != NULL);
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin *)m_pucPDU;
    strncpy(pSAGIPUserLogin->str_User_Name, strUserName, AGIP_USER_NAME_LEN);
    return S_SUCCESS;
}

int AGIPUserLogin::setPassword(const char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin *)m_pucPDU;
    strncpy(pSAGIPUserLogin->str_Password, strPassword, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserLogin::setPasswordType(int8_t cPasswordType)
{
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin *)m_pucPDU;
    pSAGIPUserLogin->c_Password_Type = cPasswordType;
    return S_SUCCESS;
}

int AGIPUserLogin::setUserPort(uint16_t usUserPort)
{
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin *)m_pucPDU;
    pSAGIPUserLogin->us_User_Port = htons(usUserPort);
    return S_SUCCESS;
}

int AGIPUserLogin::setUserIP(uint32_t unUserIP)
{
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin *)m_pucPDU;
    pSAGIPUserLogin->un_User_IP = htonl(unUserIP);
    return S_SUCCESS;
}

int AGIPUserLogin::setMatrixPassword(const char* szMatrixPassword)
{
    ASSERT(szMatrixPassword != NULL);
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin*)m_pucPDU;
    strncpy(pSAGIPUserLogin->str_Matrix_Password, szMatrixPassword, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserLogin::setMatrixCoord(const char* szMatrixCoord)
{
    ASSERT(szMatrixCoord != NULL);
    SAGIPUserLogin *pSAGIPUserLogin = (SAGIPUserLogin*)m_pucPDU;
    strncpy(pSAGIPUserLogin->str_Matrix_Coord, szMatrixCoord, AGIP_MATRIX_COORD_LEN);
    return S_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
int AGIPUserLogin::showInfo()
{
    int nRetCode        = E_ERROR;

    char strUserName[AGIP_USER_NAME_LEN + 1];
    char strPassword[AGIP_PASSWORD_LEN + 1];
    char szMatrixPassword[AGIP_PASSWORD_LEN + 1];
    char szMatrixCoord[AGIP_MATRIX_COORD_LEN + 1];
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

    printf("--------------------------------------------------------AGIPUserLogin\n");
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
    printf("--------------------------------------------------------AGIPUserLogin\n");
    return S_SUCCESS;
}


////////////////////////
// AGIPUserLoginRes
AGIPUserLoginRes::AGIPUserLoginRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_LOGIN_RES, sizeof(SAGIPUserLoginRes))
{

}

AGIPUserLoginRes::~AGIPUserLoginRes()
{

}


int AGIPUserLoginRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserLoginRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserLoginRes::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserLoginRes->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserLoginRes::getUserType(int8_t *pcUserType)
{
    ASSERT(pcUserType != NULL);
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    *pcUserType = pSAGIPUserLoginRes->c_User_Type;
    return S_SUCCESS;
}

int AGIPUserLoginRes::getAdultFlag(int8_t *pcAdultFlag)
{
    ASSERT(pcAdultFlag != NULL);
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    *pcAdultFlag = pSAGIPUserLoginRes->c_Adult_Flag;
    return S_SUCCESS;
}

int AGIPUserLoginRes::getUserClass(int8_t *pcUserClass)
{
    ASSERT(pcUserClass != NULL);
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    *pcUserClass = pSAGIPUserLoginRes->c_User_Class;
    return S_SUCCESS;
}

int AGIPUserLoginRes::getUserRoleCount(int8_t *pcUserRoleCount)
{
    ASSERT(pcUserRoleCount != NULL);
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    *pcUserRoleCount = pSAGIPUserLoginRes->c_User_Role_Count;
    return S_SUCCESS;
}

int AGIPUserLoginRes::getUserPoint(int32_t *pnUserPoint)
{
    ASSERT(pnUserPoint != NULL);
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    *pnUserPoint = htonl(pSAGIPUserLoginRes->n_User_Point);
    return S_SUCCESS;
}

int AGIPUserLoginRes::getPromoterID(uint32_t *punPromoterID)
{
    ASSERT(punPromoterID != NULL);
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    *punPromoterID = htonl(pSAGIPUserLoginRes->un_Promoter_ID);
    return S_SUCCESS;
}

int AGIPUserLoginRes::getUserFlag(uint32_t *punUserFlag)
{
    ASSERT(punUserFlag != NULL);
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    *punUserFlag = htonl(pSAGIPUserLoginRes->un_User_Flag);
    return S_SUCCESS;
}

int AGIPUserLoginRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    pSAGIPUserLoginRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPUserLoginRes::setUserID(uint32_t unUserID)
{
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    pSAGIPUserLoginRes->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserLoginRes::setUserType(int8_t cUserType)
{
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    pSAGIPUserLoginRes->c_User_Type = cUserType;
    return S_SUCCESS;
}

int AGIPUserLoginRes::setAdultFlag(int8_t cAdultFlag)
{
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    pSAGIPUserLoginRes->c_Adult_Flag = cAdultFlag;
    return S_SUCCESS;
}

int AGIPUserLoginRes::setUserClass(int8_t cUserClass)
{
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    pSAGIPUserLoginRes->c_User_Class = cUserClass;
    return S_SUCCESS;
}

int AGIPUserLoginRes::setUserRoleCount(int8_t cUserRoleCount)
{
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    pSAGIPUserLoginRes->c_User_Role_Count = cUserRoleCount;
    return S_SUCCESS;
}

int AGIPUserLoginRes::setUserPoint(int32_t nUserPoint)
{
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    pSAGIPUserLoginRes->n_User_Point = htonl(nUserPoint);
    return S_SUCCESS;
}

int AGIPUserLoginRes::setPromoterID(uint32_t unPromoterID)
{
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    pSAGIPUserLoginRes->un_Promoter_ID = htonl(unPromoterID);
    return S_SUCCESS;
}

int AGIPUserLoginRes::setUserFlag(uint32_t unUserFlag)
{
    SAGIPUserLoginRes *pSAGIPUserLoginRes = (SAGIPUserLoginRes *)m_pucPDU;
    pSAGIPUserLoginRes->un_User_Flag = htonl(unUserFlag);
    return S_SUCCESS;
}

int AGIPUserLoginRes::showInfo()
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
    nRetCode = getUserRoleCount(&cUserRoleCount);
    nRetCode = getUserPoint(&nUserPoint);
    nRetCode = getPromoterID(&unPromoterID);
    nRetCode = getUserFlag(&unUserFlag);

    printf("--------------------------------------------------------AGIPUserLoginRes\n");
    printf("Result_Code:        %d\n", nResultCode);
    printf("User_ID:            %u\n", unUserID);
    printf("User_Type:          %d\n", cUserType);
    printf("Adult_Flag:         %d\n", cAdultFlag);
    printf("User_Class:         %d\n", cUserClass);
    printf("User_Role_Count:    %d\n", cUserRoleCount);
    printf("User_Point:         %d\n", nUserPoint);
    printf("Promoter_ID:        %u\n", unPromoterID);
    printf("User_Flag:          %u\n", unUserFlag);
    printf("--------------------------------------------------------AGIPUserLoginRes\n");
    return S_SUCCESS;
}


