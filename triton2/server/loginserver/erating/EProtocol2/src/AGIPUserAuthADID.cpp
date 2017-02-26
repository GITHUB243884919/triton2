#include "AGIPUserAuthADID.h"
#include <time.h>
AGIPUserAuthADID::AGIPUserAuthADID(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_AUTH_AD_ID, sizeof(SAGIPUserAuthADID))
{
}

AGIPUserAuthADID::~AGIPUserAuthADID(void)
{
}


int AGIPUserAuthADID::getUserName(char *strUserName)
{
    ASSERT(strUserName != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID *)m_pucPDU;
    memcpy(strUserName, pSAGIPUserAuthADID->str_User_Name, AGIP_USER_NAME_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthADID::getPassword(char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID *)m_pucPDU;
    memcpy(strPassword, pSAGIPUserAuthADID->str_Password, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthADID::getPasswordType(int8_t *pcPasswordType)
{
    ASSERT(pcPasswordType != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID *)m_pucPDU;
    *pcPasswordType = pSAGIPUserAuthADID->c_Password_Type;
    return S_SUCCESS;
}

int AGIPUserAuthADID::getUserPort(uint16_t *pusUserPort)
{
    ASSERT(pusUserPort != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID *)m_pucPDU;
    *pusUserPort = htons(pSAGIPUserAuthADID->us_User_Port);
    return S_SUCCESS;
}

int AGIPUserAuthADID::getUserIP(uint32_t *punUserIP)
{
    ASSERT(punUserIP != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID *)m_pucPDU;
    *punUserIP = htonl(pSAGIPUserAuthADID->un_User_IP);
    return S_SUCCESS;
}

int AGIPUserAuthADID::getMatrixPassword(char* szMatrixPassword)
{
    ASSERT(szMatrixPassword != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID*)m_pucPDU;
    memcpy(szMatrixPassword, pSAGIPUserAuthADID->str_Matrix_Password, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthADID::getMatrixCoord(char* szMatrixCoord)
{
    ASSERT(szMatrixCoord != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID*)m_pucPDU;
    memcpy(szMatrixCoord, pSAGIPUserAuthADID->str_Matrix_Coord, AGIP_MATRIX_COORD_LEN);
    return S_SUCCESS;
}

const char* AGIPUserAuthADID::getADID(char* szADID, size_t unSize)
{
    if (NULL == szADID)
    {
        return NULL;
    }

    if (AGIP_AD_ID_LENGTH > (int32_t)unSize)
    {
        return NULL;
    }

    strncpy(szADID, ((SAGIPUserAuthADID *)m_pucPDU)->sz_AD_ID, AGIP_AD_ID_LENGTH);

    return szADID;
}

int AGIPUserAuthADID::getMAC(char *strMAC)
{
    ASSERT(strMAC != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID *)m_pucPDU;
    strncpy(strMAC, pSAGIPUserAuthADID->sz_MAC, AGIP_MAC_LEN);
    return S_SUCCESS;
}

////////////////////////////
/// Setter
int AGIPUserAuthADID::setUserName(const char *strUserName)
{
    ASSERT(strUserName != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID *)m_pucPDU;
    strncpy(pSAGIPUserAuthADID->str_User_Name, strUserName, AGIP_USER_NAME_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthADID::setPassword(const char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID *)m_pucPDU;
    strncpy(pSAGIPUserAuthADID->str_Password, strPassword, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthADID::setPasswordType(int8_t cPasswordType)
{
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID *)m_pucPDU;
    pSAGIPUserAuthADID->c_Password_Type = cPasswordType;
    return S_SUCCESS;
}

int AGIPUserAuthADID::setUserPort(uint16_t usUserPort)
{
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID *)m_pucPDU;
    pSAGIPUserAuthADID->us_User_Port = htons(usUserPort);
    return S_SUCCESS;
}

int AGIPUserAuthADID::setUserIP(uint32_t unUserIP)
{
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID *)m_pucPDU;
    pSAGIPUserAuthADID->un_User_IP = htonl(unUserIP);
    return S_SUCCESS;
}

int AGIPUserAuthADID::setMatrixPassword(const char* szMatrixPassword)
{
    ASSERT(szMatrixPassword != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID*)m_pucPDU;
    strncpy(pSAGIPUserAuthADID->str_Matrix_Password, szMatrixPassword, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthADID::setMatrixCoord(const char* szMatrixCoord)
{
    ASSERT(szMatrixCoord != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID*)m_pucPDU;
    strncpy(pSAGIPUserAuthADID->str_Matrix_Coord, szMatrixCoord, AGIP_MATRIX_COORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthADID::setADID(const char *szADID)
{
    if (NULL == szADID)
    {
        return E_ERROR;
    }

    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID *)m_pucPDU;
    strncpy(pSAGIPUserAuthADID->sz_AD_ID, szADID, AGIP_AD_ID_LENGTH);

    return S_SUCCESS;
}

int AGIPUserAuthADID::setMAC(const char* szMAC)
{
    ASSERT(szMAC != NULL);
    SAGIPUserAuthADID *pSAGIPUserAuthADID = (SAGIPUserAuthADID*)m_pucPDU;
    strncpy(pSAGIPUserAuthADID->sz_MAC, szMAC, AGIP_MAC_LEN);
    return S_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
int AGIPUserAuthADID::showInfo()
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
    char szADID[AGIP_AD_ID_LENGTH + 1] = "";

    SysProtocol::showInfo();
    nRetCode = getUserName(strUserName);
    strUserName[AGIP_USER_NAME_LEN] = '\0';
    nRetCode = getPassword(strPassword);
    strPassword[AGIP_PASSWORD_LEN] = '\0';

    nRetCode = getPasswordType(&cPasswordType);
    nRetCode = getUserIP(&unUserIP);
    nRetCode = getUserPort(&usUserPort);
    memset(szMAC, 0, sizeof(szMAC));
    getMAC(szMAC);
    memset(szADID, 0, sizeof(szADID));
    getADID(szADID, sizeof(szADID));

    printf("--------------------------------------------------------AGIPUserAuthADID\n");
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
    printf("MAC  :\t%s\n", szMAC);
    printf("AD_ID:\t%s\n", szADID);
    printf("--------------------------------------------------------AGIPUserAuthADID\n");
    return S_SUCCESS;
}


////////////////////////
// AGIPUserAuthADIDRes
AGIPUserAuthADIDRes::AGIPUserAuthADIDRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_AUTH_AD_ID_RES, sizeof(SAGIPUserAuthADIDRes))
{

}

AGIPUserAuthADIDRes::~AGIPUserAuthADIDRes()
{

}


int AGIPUserAuthADIDRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserAuthADIDRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserAuthADIDRes->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::getUserType(int8_t *pcUserType)
{
    ASSERT(pcUserType != NULL);
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    *pcUserType = pSAGIPUserAuthADIDRes->c_User_Type;
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::getAdultFlag(int8_t *pcAdultFlag)
{
    ASSERT(pcAdultFlag != NULL);
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    *pcAdultFlag = pSAGIPUserAuthADIDRes->c_Adult_Flag;
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::getUserClass(int8_t *pcUserClass)
{
    ASSERT(pcUserClass != NULL);
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    *pcUserClass = pSAGIPUserAuthADIDRes->c_User_Class;
    return S_SUCCESS;
}


int AGIPUserAuthADIDRes::getUserPoint(int32_t *pnUserPoint)
{
    ASSERT(pnUserPoint != NULL);
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    *pnUserPoint = htonl(pSAGIPUserAuthADIDRes->n_User_Point);
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::getPromoterID(uint32_t *punPromoterID)
{
    ASSERT(punPromoterID != NULL);
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    *punPromoterID = htonl(pSAGIPUserAuthADIDRes->un_Promoter_ID);
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::getUserFlag(uint32_t *punUserFlag)
{
    ASSERT(punUserFlag != NULL);
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    *punUserFlag = htonl(pSAGIPUserAuthADIDRes->un_User_Flag);
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    pSAGIPUserAuthADIDRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::setUserID(uint32_t unUserID)
{
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    pSAGIPUserAuthADIDRes->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::setUserType(int8_t cUserType)
{
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    pSAGIPUserAuthADIDRes->c_User_Type = cUserType;
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::setAdultFlag(int8_t cAdultFlag)
{
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    pSAGIPUserAuthADIDRes->c_Adult_Flag = cAdultFlag;
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::setUserClass(int8_t cUserClass)
{
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    pSAGIPUserAuthADIDRes->c_User_Class = cUserClass;
    return S_SUCCESS;
}



int AGIPUserAuthADIDRes::setUserPoint(int32_t nUserPoint)
{
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    pSAGIPUserAuthADIDRes->n_User_Point = htonl(nUserPoint);
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::setPromoterID(uint32_t unPromoterID)
{
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    pSAGIPUserAuthADIDRes->un_Promoter_ID = htonl(unPromoterID);
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::setUserFlag(uint32_t unUserFlag)
{
    SAGIPUserAuthADIDRes *pSAGIPUserAuthADIDRes = (SAGIPUserAuthADIDRes *)m_pucPDU;
    pSAGIPUserAuthADIDRes->un_User_Flag = htonl(unUserFlag);
    return S_SUCCESS;
}

int AGIPUserAuthADIDRes::showInfo()
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

    printf("--------------------------------------------------------AGIPUserAuthADIDRes\n");
    printf("Result_Code:        %d\n", nResultCode);
    printf("User_ID:            %u\n", unUserID);
    printf("User_Type:          %d\n", cUserType);
    printf("Adult_Flag:         %d\n", cAdultFlag);
    printf("User_Class:         %d\n", cUserClass);
    printf("User_Role_Count:    %d\n", cUserRoleCount);
    printf("User_Point:         %d\n", nUserPoint);
    printf("Promoter_ID:        %u\n", unPromoterID);
    printf("User_Flag:          %u\n", unUserFlag);
    printf("--------------------------------------------------------AGIPUserAuthADIDRes\n");
    return S_SUCCESS;
}


