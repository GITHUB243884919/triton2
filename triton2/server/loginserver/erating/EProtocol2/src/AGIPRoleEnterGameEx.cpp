#include "AGIPRoleEnterGameEx.h"

AGIPRoleEnterGameEx::AGIPRoleEnterGameEx(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_ROLE_ENTER_GAME_EX, sizeof(SAGIPRoleEnterGameEx))
{
    m_data = (PSAGIPRoleEnterGameEx)m_pucPDU;
}

AGIPRoleEnterGameEx::~AGIPRoleEnterGameEx(void)
{
}

uint32_t AGIPRoleEnterGameEx::getServerID(void)
{
    return ntohl(m_data->un_Server_ID);
}

uint32_t AGIPRoleEnterGameEx::getUserID(void)
{
    return ntohl(m_data->un_User_ID);
}

uint32_t AGIPRoleEnterGameEx::getRoleID(void)
{
    return ntohl(m_data->un_Role_ID);
}

uint16_t AGIPRoleEnterGameEx::getLevel(void)
{
    return ntohs(m_data->us_Level);
}

uint8_t  AGIPRoleEnterGameEx::getGender(void)
{
    return m_data->uc_Gender;
}

uint8_t  AGIPRoleEnterGameEx::getOccupationID(void)
{
    return m_data->uc_Occupation_ID;
}

uint32_t AGIPRoleEnterGameEx::getCorpsID(void)
{
    return ntohl(m_data->un_Corps_ID);
}

uint16_t AGIPRoleEnterGameEx::getCommunityID(void)
{
    return ntohs(m_data->us_Community_ID);
}

uint16_t AGIPRoleEnterGameEx::getClientPort(void)
{
    return ntohs(m_data->us_Client_Port);
}

uint32_t AGIPRoleEnterGameEx::getClientIP(void)
{
    return ntohl(m_data->un_Client_IP);
}

const char* AGIPRoleEnterGameEx::getClientMAC(char* szClientMAC, size_t* punLen)
{
    if ((NULL == szClientMAC) || (NULL == punLen)) {
        return NULL;
    }

    if (*punLen < AGIP_MAC_LEN) {
        return NULL;
    }

    *punLen = AGIP_MAC_LEN;
    memcpy(szClientMAC, m_data->str_Client_MAC, AGIP_MAC_LEN);
    szClientMAC[AGIP_MAC_LEN] = '\0';

    return szClientMAC;
}

void AGIPRoleEnterGameEx::setServerID(uint32_t unServerID)
{
    m_data->un_Server_ID = htonl(unServerID);
    return;
}

void AGIPRoleEnterGameEx::setUserID(uint32_t unUserID)
{
    m_data->un_User_ID = htonl(unUserID);
    return;
}

void AGIPRoleEnterGameEx::setRoleID(uint32_t unRoleID)
{
    m_data->un_Role_ID = htonl(unRoleID);
    return;
}

void AGIPRoleEnterGameEx::setLevel(uint16_t usLevel)
{
    m_data->us_Level = htons(usLevel);
    return;
}

void AGIPRoleEnterGameEx::setGender(uint8_t ucGender)
{
    m_data->uc_Gender = ucGender;
    return;
}

void AGIPRoleEnterGameEx::setOccupationID(uint8_t ucOccupationID)
{
    m_data->uc_Occupation_ID = ucOccupationID;
    return;
}

void AGIPRoleEnterGameEx::setCorpsID(uint32_t unCorpsID)
{
    m_data->un_Corps_ID = htonl(unCorpsID);
    return;
}

void AGIPRoleEnterGameEx::setCommunityID(uint16_t usCommunityID)
{
    m_data->us_Community_ID = htons(usCommunityID);
    return;
}

void AGIPRoleEnterGameEx::setClientPort(uint16_t usPort)
{
    m_data->us_Client_Port = htons(usPort);
    return;
}

void AGIPRoleEnterGameEx::setClientIP(uint32_t unIP)
{
    m_data->un_Client_IP = htonl(unIP);
    return;
}

void AGIPRoleEnterGameEx::setClientMAC(const char* szMAC)
{
    if (NULL == szMAC) {
        return;
    }

    memcpy(m_data->str_Client_MAC, szMAC, AGIP_MAC_LEN);

    return;
}

void AGIPRoleEnterGameEx::setHardwareSN1(const char* szHardwareSN)
{
    if (NULL == szHardwareSN)
    {
        return;
    }

    memcpy(((SAGIPRoleEnterGameEx *)m_pucPDU)->sz_Hardware_SN1, szHardwareSN, AGIP_HARDWARE_SN_LENGTH);
}

const char* AGIPRoleEnterGameEx::getHardwareSN1(char* szHardwareSN)
{
    if (NULL == szHardwareSN)
    {
        return NULL;
    }

    memcpy(szHardwareSN, ((SAGIPRoleEnterGameEx *)m_pucPDU)->sz_Hardware_SN1, AGIP_HARDWARE_SN_LENGTH);

    return szHardwareSN;
}

void AGIPRoleEnterGameEx::setHardwareSN2(const char* szHardwareSN)
{
    if (NULL == szHardwareSN)
    {
        return;
    }

    memcpy(((SAGIPRoleEnterGameEx *)m_pucPDU)->sz_Hardware_SN2, szHardwareSN, AGIP_HARDWARE_SN_LENGTH);
}

const char* AGIPRoleEnterGameEx::getHardwareSN2(char* szHardwareSN)
{
    if (NULL == szHardwareSN)
    {
        return NULL;
    }

    memcpy(szHardwareSN, ((SAGIPRoleEnterGameEx *)m_pucPDU)->sz_Hardware_SN2, AGIP_HARDWARE_SN_LENGTH);

    return szHardwareSN;
}

int AGIPRoleEnterGameEx::showInfo()
{
    char szIP[64] = {0};
    IP2String(getClientIP(), szIP);
    szIP[63] = '\0';

    char szMAC[64] = {0};
    size_t unMACLen = sizeof(szMAC);
    getClientMAC(szMAC, &unMACLen);

    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPRoleEnterGameEx\n");
    printf("Server_ID:     %u\n", getServerID());
    printf("User_ID:       %u\n", getUserID());
    printf("Role_ID:       %u\n", getRoleID());
    printf("Level:         %u\n", getLevel());
    printf("Gender:        %u\n", getGender());
    printf("Occupation_ID: %u\n", getOccupationID());
    printf("Community_ID:  %u\n", getCommunityID());
    printf("Port:          %u\n", getClientPort());
    printf("IP:            %s\n", szIP);
    printf("MAC:           %s\n", szMAC);

    char szHardwareSN[AGIP_HARDWARE_SN_LENGTH + 1] = "";
    memset(szHardwareSN, 0, sizeof(szHardwareSN));
    printf("Hardware_SN1:  %s\n", getHardwareSN1(szHardwareSN));
    memset(szHardwareSN, 0, sizeof(szHardwareSN));
    printf("Hardware_SN2:  %s\n", getHardwareSN2(szHardwareSN));

    printf("--------------------------------------------------------AGIPRoleEnterGameEx\n");

    return S_SUCCESS;
}

// AGIPRoleEnterGameExRes
AGIPRoleEnterGameExRes::AGIPRoleEnterGameExRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_ROLE_ENTER_GAME_EX_RES, sizeof(SAGIPRoleEnterGameExRes) - sizeof(BALANCE_INFO))
{
    m_data = (PSAGIPRoleEnterGameExRes)m_pucPDU;
}

AGIPRoleEnterGameExRes::~AGIPRoleEnterGameExRes()
{

}

int AGIPRoleEnterGameExRes::initialize()
{
    int nRetCode = E_ERROR;

    m_usTotalLength = sizeof(SAGIPRoleEnterGameExRes) - sizeof(BALANCE_INFO);
    nRetCode = SysProtocol::initialize();
    ASSERT(nRetCode == S_SUCCESS);

    return S_SUCCESS;
}

int AGIPRoleEnterGameExRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPRoleEnterGameExRes *pSAGIPRoleEnterGameExRes = (SAGIPRoleEnterGameExRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPRoleEnterGameExRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPRoleEnterGameExRes::getBalanceCount(int32_t *pnBalanceCount)
{
    ASSERT(pnBalanceCount != NULL);
    SAGIPRoleEnterGameExRes *pSAGIPRoleEnterGameExRes = (SAGIPRoleEnterGameExRes *)m_pucPDU;
    *pnBalanceCount = htonl(pSAGIPRoleEnterGameExRes->n_Balance_Count);
    return S_SUCCESS;
}

int AGIPRoleEnterGameExRes::getBalanceInfo(int32_t nIndex, int32_t *pnSubjectID, int32_t *pnAmount)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    BALANCE_INFO *pBalanceInfoBuffer = NULL;
    int32_t nBalanceCount = 0;

    ASSERT(pnSubjectID != NULL && pnAmount != NULL);
    SAGIPRoleEnterGameExRes *pSAGIPRoleEnterGameExRes = (SAGIPRoleEnterGameExRes *)m_pucPDU;
    nBalanceCount = htonl(pSAGIPRoleEnterGameExRes->n_Balance_Count);
    if (
        (nIndex < 0) ||
        (nIndex >= nBalanceCount)
        )
    {
        goto ExitError;
    }
    pBalanceInfoBuffer = (BALANCE_INFO *)&pSAGIPRoleEnterGameExRes->n_Subject_ID;
    pBalanceInfoBuffer += nIndex;

    *pnSubjectID = htonl(pBalanceInfoBuffer->nSubjectID);
    *pnAmount    = htonl(pBalanceInfoBuffer->nAmount);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPRoleEnterGameExRes::getBalanceInfo(int32_t nIndex, BALANCE_INFO *pBalanceInfo)
{
    ASSERT(pBalanceInfo != NULL);
    return this->getBalanceInfo(nIndex, &pBalanceInfo->nSubjectID, &pBalanceInfo->nAmount);
}


int AGIPRoleEnterGameExRes::setResultCode(int32_t nResultCode)
{
    SAGIPRoleEnterGameExRes *pSAGIPRoleEnterGameExRes = (SAGIPRoleEnterGameExRes *)m_pucPDU;

    pSAGIPRoleEnterGameExRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPRoleEnterGameExRes::setBalanceCount(int32_t nBalanceCount)
{
    ASSERT(false && "Balance Count is calculated automatically. Pls use addBalanceInfo() method!!");
    nBalanceCount = 0;
    return E_ERROR;
}

int AGIPRoleEnterGameExRes::addBalanceInfo(int32_t nSubjectID, int32_t nAmount)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    BALANCE_INFO *pBalanceInfoBuffer = NULL;
    int32_t nBalanceCount = 0;

    SAGIPRoleEnterGameExRes *pSAGIPRoleEnterGameExRes = (SAGIPRoleEnterGameExRes *)m_pucPDU;
    nBalanceCount = htonl(pSAGIPRoleEnterGameExRes->n_Balance_Count);

    pBalanceInfoBuffer = (BALANCE_INFO *)&pSAGIPRoleEnterGameExRes->n_Subject_ID;
    pBalanceInfoBuffer += nBalanceCount;

    pBalanceInfoBuffer->nSubjectID = htonl(nSubjectID);
    pBalanceInfoBuffer->nAmount    = htonl(nAmount);

    nBalanceCount++;
    pSAGIPRoleEnterGameExRes->n_Balance_Count = htonl(nBalanceCount);
    m_usTotalLength += sizeof(BALANCE_INFO);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
    //ExitError:
    return nResult;
}

int AGIPRoleEnterGameExRes::addBalanceInfo(const BALANCE_INFO *pBalanceInfo)
{
    ASSERT(pBalanceInfo != NULL);
    return this->addBalanceInfo(pBalanceInfo->nSubjectID, pBalanceInfo->nAmount);
}

int AGIPRoleEnterGameExRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode   = 0;
    int32_t   nBalanceCount = 0;
    BALANCE_INFO balanceInfo = {0, 0};

    SysProtocol::showInfo();

    nRetCode = getResultCode(&nResultCode);
    nRetCode = getBalanceCount(&nBalanceCount);
    printf("--------------------------------------------------------AGIPRoleEnterGameExRes\n");
    printf("Result_Code:\t%d\n", nResultCode);
    printf("Balance_Count:\t%d\n", nBalanceCount);
    for (int i = 0; i < nBalanceCount; i++)
    {
        nRetCode = getBalanceInfo(i, &balanceInfo);
        printf("Index[%d]: Subject_ID = %d, Amount = %d.\n", i, balanceInfo.nSubjectID, balanceInfo.nAmount);
    }

    printf("--------------------------------------------------------AGIPRoleEnterGameExRes\n");
    return S_SUCCESS;
}

