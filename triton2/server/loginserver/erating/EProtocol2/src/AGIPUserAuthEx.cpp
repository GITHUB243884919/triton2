#include "AGIPUserAuthEx.h"
#include <time.h>
AGIPUserAuthEx::AGIPUserAuthEx(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_AUTH_EX, sizeof(SAGIPUserAuthEx))
{
}

AGIPUserAuthEx::~AGIPUserAuthEx(void)
{
}


int AGIPUserAuthEx::getUserName(char *strUserName)
{
    ASSERT(strUserName != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx *)m_pucPDU;
    memcpy(strUserName, pSAGIPUserAuthEx->str_User_Name, AGIP_USER_NAME_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthEx::getPassword(char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx *)m_pucPDU;
    memcpy(strPassword, pSAGIPUserAuthEx->str_Password, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthEx::getPasswordType(int8_t *pcPasswordType)
{
    ASSERT(pcPasswordType != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx *)m_pucPDU;
    *pcPasswordType = pSAGIPUserAuthEx->c_Password_Type;
    return S_SUCCESS;
}

int AGIPUserAuthEx::getUserPort(uint16_t *pusUserPort)
{
    ASSERT(pusUserPort != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx *)m_pucPDU;
    *pusUserPort = htons(pSAGIPUserAuthEx->us_User_Port);
    return S_SUCCESS;
}

int AGIPUserAuthEx::getUserIP(uint32_t *punUserIP)
{
    ASSERT(punUserIP != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx *)m_pucPDU;
    *punUserIP = htonl(pSAGIPUserAuthEx->un_User_IP);
    return S_SUCCESS;
}

int AGIPUserAuthEx::getMatrixPassword(char* szMatrixPassword)
{
    ASSERT(szMatrixPassword != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx*)m_pucPDU;
    memcpy(szMatrixPassword, pSAGIPUserAuthEx->str_Matrix_Password, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthEx::getMatrixCoord(char* szMatrixCoord)
{
    ASSERT(szMatrixCoord != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx*)m_pucPDU;
    memcpy(szMatrixCoord, pSAGIPUserAuthEx->str_Matrix_Coord, AGIP_MATRIX_COORD_LEN);
    return S_SUCCESS;
}

const char* AGIPUserAuthEx::getADID(char* szADID, size_t unSize)
{
    if (NULL == szADID)
    {
        return NULL;
    }

    if (AGIP_AD_ID_LENGTH > (int32_t)unSize)
    {
        return NULL;
    }

    strncpy(szADID, ((SAGIPUserAuthEx *)m_pucPDU)->sz_AD_ID, AGIP_AD_ID_LENGTH);

    return szADID;
}

int AGIPUserAuthEx::getMAC(char *strMAC)
{
    ASSERT(strMAC != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx *)m_pucPDU;
    strncpy(strMAC, pSAGIPUserAuthEx->sz_MAC, AGIP_MAC_LEN);
    return S_SUCCESS;
}

////////////////////////////
/// Setter
int AGIPUserAuthEx::setUserName(const char *strUserName)
{
    ASSERT(strUserName != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx *)m_pucPDU;
    strncpy(pSAGIPUserAuthEx->str_User_Name, strUserName, AGIP_USER_NAME_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthEx::setPassword(const char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx *)m_pucPDU;
    strncpy(pSAGIPUserAuthEx->str_Password, strPassword, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthEx::setPasswordType(int8_t cPasswordType)
{
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx *)m_pucPDU;
    pSAGIPUserAuthEx->c_Password_Type = cPasswordType;
    return S_SUCCESS;
}

int AGIPUserAuthEx::setUserPort(uint16_t usUserPort)
{
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx *)m_pucPDU;
    pSAGIPUserAuthEx->us_User_Port = htons(usUserPort);
    return S_SUCCESS;
}

int AGIPUserAuthEx::setUserIP(uint32_t unUserIP)
{
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx *)m_pucPDU;
    pSAGIPUserAuthEx->un_User_IP = htonl(unUserIP);
    return S_SUCCESS;
}

int AGIPUserAuthEx::setMatrixPassword(const char* szMatrixPassword)
{
    ASSERT(szMatrixPassword != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx*)m_pucPDU;
    strncpy(pSAGIPUserAuthEx->str_Matrix_Password, szMatrixPassword, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthEx::setMatrixCoord(const char* szMatrixCoord)
{
    ASSERT(szMatrixCoord != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx*)m_pucPDU;
    strncpy(pSAGIPUserAuthEx->str_Matrix_Coord, szMatrixCoord, AGIP_MATRIX_COORD_LEN);
    return S_SUCCESS;
}

int AGIPUserAuthEx::setADID(const char *szADID)
{
    if (NULL == szADID)
    {
        return E_ERROR;
    }

    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx *)m_pucPDU;
    strncpy(pSAGIPUserAuthEx->sz_AD_ID, szADID, AGIP_AD_ID_LENGTH);

    return S_SUCCESS;
}

void AGIPUserAuthEx::setHardwareSN1(const char* szHardwareSN)
{
    if (NULL == szHardwareSN)
    {
        return;
    }

    memcpy(((SAGIPUserAuthEx *)m_pucPDU)->sz_Hardware_SN1, szHardwareSN, AGIP_HARDWARE_SN_LENGTH);
}

const char* AGIPUserAuthEx::getHardwareSN1(char* szHardwareSN)
{
    if (NULL == szHardwareSN)
    {
        return NULL;
    }

    memcpy(szHardwareSN, ((SAGIPUserAuthEx *)m_pucPDU)->sz_Hardware_SN1, AGIP_HARDWARE_SN_LENGTH);

    return szHardwareSN;
}

void AGIPUserAuthEx::setHardwareSN2(const char* szHardwareSN)
{
    if (NULL == szHardwareSN)
    {
        return;
    }

    memcpy(((SAGIPUserAuthEx *)m_pucPDU)->sz_Hardware_SN2, szHardwareSN, AGIP_HARDWARE_SN_LENGTH);
}

const char* AGIPUserAuthEx::getHardwareSN2(char* szHardwareSN)
{
    if (NULL == szHardwareSN)
    {
        return NULL;
    }

    memcpy(szHardwareSN, ((SAGIPUserAuthEx *)m_pucPDU)->sz_Hardware_SN2, AGIP_HARDWARE_SN_LENGTH);

    return szHardwareSN;
}

int AGIPUserAuthEx::setMAC(const char* szMAC)
{
    ASSERT(szMAC != NULL);
    SAGIPUserAuthEx *pSAGIPUserAuthEx = (SAGIPUserAuthEx*)m_pucPDU;
    strncpy(pSAGIPUserAuthEx->sz_MAC, szMAC, AGIP_MAC_LEN);
    return S_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
int AGIPUserAuthEx::showInfo()
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
    char szHardwareSN[AGIP_HARDWARE_SN_LENGTH + 1] = "";

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

    printf("--------------------------------------------------------AGIPUserAuthEx\n");
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
    case PASSWORD_TYPE_DYNAMIC:
        printf("Password_Type\t%d(Dynamic-Password)\n", (int32_t)cPasswordType);
        nRetCode = getMatrixPassword(szMatrixPassword);
        szMatrixPassword[sizeof(szMatrixPassword) - 1] = '\0';
        printf("Dynamic_Password:\t%s\n", szMatrixPassword);
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
    memset(szHardwareSN, 0, sizeof(szHardwareSN));
    printf("Hardware_SN1:   %s\n", getHardwareSN1(szHardwareSN));
    memset(szHardwareSN, 0, sizeof(szHardwareSN));
    printf("Hardware_SN2:   %s\n", getHardwareSN2(szHardwareSN));
    printf("--------------------------------------------------------AGIPUserAuthEx\n");
    return S_SUCCESS;
}


////////////////////////
// AGIPUserAuthExRes
AGIPUserAuthExRes::AGIPUserAuthExRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_AUTH_EX_RES, sizeof(SAGIPUserAuthExRes))
{

}

AGIPUserAuthExRes::~AGIPUserAuthExRes()
{

}


int AGIPUserAuthExRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserAuthExRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserAuthExRes::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserAuthExRes->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserAuthExRes::getUserType(int8_t *pcUserType)
{
    ASSERT(pcUserType != NULL);
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    *pcUserType = pSAGIPUserAuthExRes->c_User_Type;
    return S_SUCCESS;
}

int AGIPUserAuthExRes::getAdultFlag(int8_t *pcAdultFlag)
{
    ASSERT(pcAdultFlag != NULL);
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    *pcAdultFlag = pSAGIPUserAuthExRes->c_Adult_Flag;
    return S_SUCCESS;
}

int AGIPUserAuthExRes::getUserClass(int8_t *pcUserClass)
{
    ASSERT(pcUserClass != NULL);
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    *pcUserClass = pSAGIPUserAuthExRes->c_User_Class;
    return S_SUCCESS;
}


int AGIPUserAuthExRes::getUserPoint(int32_t *pnUserPoint)
{
    ASSERT(pnUserPoint != NULL);
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    *pnUserPoint = htonl(pSAGIPUserAuthExRes->n_User_Point);
    return S_SUCCESS;
}

int AGIPUserAuthExRes::getPromoterID(uint32_t *punPromoterID)
{
    ASSERT(punPromoterID != NULL);
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    *punPromoterID = htonl(pSAGIPUserAuthExRes->un_Promoter_ID);
    return S_SUCCESS;
}

int AGIPUserAuthExRes::getUserFlag(uint32_t *punUserFlag)
{
    ASSERT(punUserFlag != NULL);
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    *punUserFlag = htonl(pSAGIPUserAuthExRes->un_User_Flag);
    return S_SUCCESS;
}

int AGIPUserAuthExRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    pSAGIPUserAuthExRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPUserAuthExRes::setUserID(uint32_t unUserID)
{
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    pSAGIPUserAuthExRes->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserAuthExRes::setUserType(int8_t cUserType)
{
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    pSAGIPUserAuthExRes->c_User_Type = cUserType;
    return S_SUCCESS;
}

int AGIPUserAuthExRes::setAdultFlag(int8_t cAdultFlag)
{
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    pSAGIPUserAuthExRes->c_Adult_Flag = cAdultFlag;
    return S_SUCCESS;
}

int AGIPUserAuthExRes::setUserClass(int8_t cUserClass)
{
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    pSAGIPUserAuthExRes->c_User_Class = cUserClass;
    return S_SUCCESS;
}



int AGIPUserAuthExRes::setUserPoint(int32_t nUserPoint)
{
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    pSAGIPUserAuthExRes->n_User_Point = htonl(nUserPoint);
    return S_SUCCESS;
}

int AGIPUserAuthExRes::setPromoterID(uint32_t unPromoterID)
{
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    pSAGIPUserAuthExRes->un_Promoter_ID = htonl(unPromoterID);
    return S_SUCCESS;
}

int AGIPUserAuthExRes::setUserFlag(uint32_t unUserFlag)
{
    SAGIPUserAuthExRes *pSAGIPUserAuthExRes = (SAGIPUserAuthExRes *)m_pucPDU;
    pSAGIPUserAuthExRes->un_User_Flag = htonl(unUserFlag);
    return S_SUCCESS;
}

int AGIPUserAuthExRes::showInfo()
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

    printf("--------------------------------------------------------AGIPUserAuthExRes\n");
    printf("Result_Code:        %d\n", nResultCode);
    printf("User_ID:            %u\n", unUserID);
    printf("User_Type:          %d\n", cUserType);
    printf("Adult_Flag:         %d\n", cAdultFlag);
    printf("User_Class:         %d\n", cUserClass);
    printf("User_Role_Count:    %d\n", cUserRoleCount);
    printf("User_Point:         %d\n", nUserPoint);
    printf("Promoter_ID:        %u\n", unPromoterID);
    printf("User_Flag:          %u\n", unUserFlag);
    printf("--------------------------------------------------------AGIPUserAuthExRes\n");
    return S_SUCCESS;
}


