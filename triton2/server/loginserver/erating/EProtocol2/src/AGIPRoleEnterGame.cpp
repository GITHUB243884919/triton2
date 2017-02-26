#include "AGIPRoleEnterGame.h"

AGIPRoleEnterGame::AGIPRoleEnterGame(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_ROLE_ENTER_GAME, sizeof(SAGIPRoleEnterGame))
{
    m_data = (PSAGIPRoleEnterGame)m_pucPDU;
}

AGIPRoleEnterGame::~AGIPRoleEnterGame(void)
{
}

uint32_t AGIPRoleEnterGame::getServerID(void)
{
    return ntohl(m_data->un_Server_ID);
}

uint32_t AGIPRoleEnterGame::getUserID(void)
{
    return ntohl(m_data->un_User_ID);
}

uint32_t AGIPRoleEnterGame::getRoleID(void)
{
    return ntohl(m_data->un_Role_ID);
}

uint16_t AGIPRoleEnterGame::getLevel(void)
{
    return ntohs(m_data->us_Level);
}

uint8_t  AGIPRoleEnterGame::getGender(void)
{
    return m_data->uc_Gender;
}

uint8_t  AGIPRoleEnterGame::getOccupationID(void)
{
    return m_data->uc_Occupation_ID;
}

uint32_t AGIPRoleEnterGame::getCorpsID(void)
{
    return ntohl(m_data->un_Corps_ID);
}

uint16_t AGIPRoleEnterGame::getCommunityID(void)
{
    return ntohs(m_data->us_Community_ID);
}

uint16_t AGIPRoleEnterGame::getClientPort(void)
{
    return ntohs(m_data->us_Client_Port);
}

uint32_t AGIPRoleEnterGame::getClientIP(void)
{
    return ntohl(m_data->un_Client_IP);
}

const char* AGIPRoleEnterGame::getClientMAC(char* szClientMAC, size_t* punLen)
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

void AGIPRoleEnterGame::setServerID(uint32_t unServerID)
{
    m_data->un_Server_ID = htonl(unServerID);
    return;
}

void AGIPRoleEnterGame::setUserID(uint32_t unUserID)
{
    m_data->un_User_ID = htonl(unUserID);
    return;
}

void AGIPRoleEnterGame::setRoleID(uint32_t unRoleID)
{
    m_data->un_Role_ID = htonl(unRoleID);
    return;
}

void AGIPRoleEnterGame::setLevel(uint16_t usLevel)
{
    m_data->us_Level = htons(usLevel);
    return;
}

void AGIPRoleEnterGame::setGender(uint8_t ucGender)
{
    m_data->uc_Gender = ucGender;
    return;
}

void AGIPRoleEnterGame::setOccupationID(uint8_t ucOccupationID)
{
    m_data->uc_Occupation_ID = ucOccupationID;
    return;
}

void AGIPRoleEnterGame::setCorpsID(uint32_t unCorpsID)
{
    m_data->un_Corps_ID = htonl(unCorpsID);
    return;
}

void AGIPRoleEnterGame::setCommunityID(uint16_t usCommunityID)
{
    m_data->us_Community_ID = htons(usCommunityID);
    return;
}

void AGIPRoleEnterGame::setClientPort(uint16_t usPort)
{
    m_data->us_Client_Port = htons(usPort);
    return;
}

void AGIPRoleEnterGame::setClientIP(uint32_t unIP)
{
    m_data->un_Client_IP = htonl(unIP);
    return;
}

void AGIPRoleEnterGame::setClientMAC(const char* szMAC)
{
    if (NULL == szMAC) {
        return;
    }

    memcpy(m_data->str_Client_MAC, szMAC, AGIP_MAC_LEN);

    return;
}

int AGIPRoleEnterGame::showInfo()
{
    char szIP[64] = {0};
    IP2String(getClientIP(), szIP);
    szIP[63] = '\0';

    char szMAC[64] = {0};
    size_t unMACLen = sizeof(szMAC);
    getClientMAC(szMAC, &unMACLen);

    printf("--------------------------------------------------------AGIPRoleEnterGame\n");
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
    printf("--------------------------------------------------------AGIPRoleEnterGame\n");

    return S_SUCCESS;
}

// AGIPRoleEnterGameRes
AGIPRoleEnterGameRes::AGIPRoleEnterGameRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_ROLE_ENTER_GAME_RES, sizeof(SAGIPRoleEnterGameRes) - sizeof(BALANCE_INFO))
{
    m_data = (PSAGIPRoleEnterGameRes)m_pucPDU;
}

AGIPRoleEnterGameRes::~AGIPRoleEnterGameRes()
{

}

int AGIPRoleEnterGameRes::initialize()
{
    int nRetCode = E_ERROR;

    m_usTotalLength = sizeof(SAGIPRoleEnterGameRes) - sizeof(BALANCE_INFO);
    nRetCode = SysProtocol::initialize();
    ASSERT(nRetCode == S_SUCCESS);

    return S_SUCCESS;
}

int AGIPRoleEnterGameRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPRoleEnterGameRes *pSAGIPRoleEnterGameRes = (SAGIPRoleEnterGameRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPRoleEnterGameRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPRoleEnterGameRes::getBalanceCount(int32_t *pnBalanceCount)
{
    ASSERT(pnBalanceCount != NULL);
    SAGIPRoleEnterGameRes *pSAGIPRoleEnterGameRes = (SAGIPRoleEnterGameRes *)m_pucPDU;
    *pnBalanceCount = htonl(pSAGIPRoleEnterGameRes->n_Balance_Count);
    return S_SUCCESS;
}

int AGIPRoleEnterGameRes::getBalanceInfo(int32_t nIndex, int32_t *pnSubjectID, int32_t *pnAmount)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    BALANCE_INFO *pBalanceInfoBuffer = NULL;
    int32_t nBalanceCount = 0;

    ASSERT(pnSubjectID != NULL && pnAmount != NULL);
    SAGIPRoleEnterGameRes *pSAGIPRoleEnterGameRes = (SAGIPRoleEnterGameRes *)m_pucPDU;
    nBalanceCount = htonl(pSAGIPRoleEnterGameRes->n_Balance_Count);
    if (
        (nIndex < 0) ||
        (nIndex >= nBalanceCount)
        )
    {
        goto ExitError;
    }
    pBalanceInfoBuffer = (BALANCE_INFO *)&pSAGIPRoleEnterGameRes->n_Subject_ID;
    pBalanceInfoBuffer += nIndex;

    *pnSubjectID = htonl(pBalanceInfoBuffer->nSubjectID);
    *pnAmount    = htonl(pBalanceInfoBuffer->nAmount);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPRoleEnterGameRes::getBalanceInfo(int32_t nIndex, BALANCE_INFO *pBalanceInfo)
{
    ASSERT(pBalanceInfo != NULL);
    return this->getBalanceInfo(nIndex, &pBalanceInfo->nSubjectID, &pBalanceInfo->nAmount);
}


int AGIPRoleEnterGameRes::setResultCode(int32_t nResultCode)
{
    SAGIPRoleEnterGameRes *pSAGIPRoleEnterGameRes = (SAGIPRoleEnterGameRes *)m_pucPDU;

    pSAGIPRoleEnterGameRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPRoleEnterGameRes::setBalanceCount(int32_t nBalanceCount)
{
    ASSERT(false && "Balance Count is calculated automatically. Pls use addBalanceInfo() method!!");
    nBalanceCount = 0;
    return E_ERROR;
}

int AGIPRoleEnterGameRes::addBalanceInfo(int32_t nSubjectID, int32_t nAmount)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    BALANCE_INFO *pBalanceInfoBuffer = NULL;
    int32_t nBalanceCount = 0;

    SAGIPRoleEnterGameRes *pSAGIPRoleEnterGameRes = (SAGIPRoleEnterGameRes *)m_pucPDU;
    nBalanceCount = htonl(pSAGIPRoleEnterGameRes->n_Balance_Count);

    pBalanceInfoBuffer = (BALANCE_INFO *)&pSAGIPRoleEnterGameRes->n_Subject_ID;
    pBalanceInfoBuffer += nBalanceCount;

    pBalanceInfoBuffer->nSubjectID = htonl(nSubjectID);
    pBalanceInfoBuffer->nAmount    = htonl(nAmount);

    nBalanceCount++;
    pSAGIPRoleEnterGameRes->n_Balance_Count = htonl(nBalanceCount);
    m_usTotalLength += sizeof(BALANCE_INFO);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
    //ExitError:
    return nResult;
}

int AGIPRoleEnterGameRes::addBalanceInfo(const BALANCE_INFO *pBalanceInfo)
{
    ASSERT(pBalanceInfo != NULL);
    return this->addBalanceInfo(pBalanceInfo->nSubjectID, pBalanceInfo->nAmount);
}

int AGIPRoleEnterGameRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode   = 0;
    int32_t   nBalanceCount = 0;
    BALANCE_INFO balanceInfo = {0, 0};

    SysProtocol::showInfo();

    nRetCode = getResultCode(&nResultCode);
    nRetCode = getBalanceCount(&nBalanceCount);
    printf("--------------------------------------------------------AGIPRoleEnterGameRes\n");
    printf("Result_Code:\t%d\n", nResultCode);
    printf("Balance_Count:\t%d\n", nBalanceCount);
    for (int i = 0; i < nBalanceCount; i++)
    {
        nRetCode = getBalanceInfo(i, &balanceInfo);
        printf("Index[%d]: Subject_ID = %d, Amount = %d.\n", i, balanceInfo.nSubjectID, balanceInfo.nAmount);
    }

    printf("--------------------------------------------------------AGIPRoleEnterGameRes\n");
    return S_SUCCESS;
}

