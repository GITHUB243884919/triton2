#include "AGIPUserEnterGame.h"

AGIPUserEnterGame::AGIPUserEnterGame(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_ENTER_GAME, sizeof(SAGIPUserEnterGame))
{
}

AGIPUserEnterGame::~AGIPUserEnterGame(void)
{
}

int AGIPUserEnterGame::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserEnterGame->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserEnterGame::getRoleID(uint32_t *punRoleID)
{
    ASSERT(punRoleID != NULL);
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    *punRoleID = htonl(pSAGIPUserEnterGame->un_Role_ID);
    return S_SUCCESS;
}

int AGIPUserEnterGame::getServerID(uint32_t *punServerID)
{
    ASSERT(punServerID != NULL);
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    *punServerID = htonl(pSAGIPUserEnterGame->un_Server_ID);
    return S_SUCCESS;
}

int AGIPUserEnterGame::getRatingID(uint32_t *punRatingID)
{
    ASSERT(punRatingID != NULL);
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    *punRatingID = htonl(pSAGIPUserEnterGame->un_Rating_ID);
    return S_SUCCESS;
}

/*
int AGIPUserEnterGame::getRoleLevel(int8_t *pcRoleLevel)
{
    ASSERT(pcRoleLevel != NULL);
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    *pcRoleLevel = pSAGIPUserEnterGame->c_Role_Level;
    return S_SUCCESS;
}
*/

int AGIPUserEnterGame::getRoleLevel(uint16_t* pusRoleLevel)
{
    ASSERT(pusRoleLevel != NULL);
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    *pusRoleLevel = ntohs(pSAGIPUserEnterGame->us_Role_Level);
    return S_SUCCESS;
}

int AGIPUserEnterGame::getUserPort(uint16_t *pusUserPort)
{
    ASSERT(pusUserPort != NULL);
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    *pusUserPort = htons(pSAGIPUserEnterGame->us_User_Port);
    return S_SUCCESS;
}

int AGIPUserEnterGame::getUserIP(uint32_t *punUserIP)
{
    ASSERT(punUserIP != NULL);
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    *punUserIP = htonl(pSAGIPUserEnterGame->un_User_IP);
    return S_SUCCESS;
}

int AGIPUserEnterGame::setUserID(uint32_t unUserID)
{
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    pSAGIPUserEnterGame->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserEnterGame::setRoleID(uint32_t unRoleID)
{
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    pSAGIPUserEnterGame->un_Role_ID = htonl(unRoleID);
    return S_SUCCESS;
}

int AGIPUserEnterGame::setServerID(uint32_t unServerID)
{
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    pSAGIPUserEnterGame->un_Server_ID = htonl(unServerID);
    return S_SUCCESS;
}

int AGIPUserEnterGame::setRatingID(uint32_t unRatingID)
{
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    pSAGIPUserEnterGame->un_Rating_ID = htonl(unRatingID);
    return S_SUCCESS;
}

/*
int AGIPUserEnterGame::setRoleLevel(int8_t cRoleLevel)
{
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    pSAGIPUserEnterGame->c_Role_Level = cRoleLevel;
    return S_SUCCESS;
}
*/

int AGIPUserEnterGame::setRoleLevel(uint16_t usRoleLevel)
{
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    pSAGIPUserEnterGame->us_Role_Level = htons(usRoleLevel);
    return S_SUCCESS;
}

int AGIPUserEnterGame::setUserPort(uint16_t usUserPort)
{
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    pSAGIPUserEnterGame->us_User_Port = htons(usUserPort);
    return S_SUCCESS;
}

int AGIPUserEnterGame::setUserIP(uint32_t unUserIP)
{
    SAGIPUserEnterGame *pSAGIPUserEnterGame = (SAGIPUserEnterGame *)m_pucPDU;
    pSAGIPUserEnterGame->un_User_IP = htonl(unUserIP);
    return S_SUCCESS;
}

int AGIPUserEnterGame::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t   unUserID   = 0;
    uint32_t   unRoleID   = 0;
    uint32_t   unServerID = 0;
    uint32_t   unRatingID = 0;
    // int8_t     cRoleLevel = 0;
    uint16_t   usRoleLevel = 0;
    uint16_t   usUserPort = 0;
    uint32_t   unUserIP   = 0;

    SysProtocol::showInfo();
    nRetCode = getUserID(&unUserID);
    nRetCode = getRoleID(&unRoleID);
    nRetCode = getServerID(&unServerID);
    nRetCode = getRatingID(&unRatingID);
    // nRetCode = getRoleLevel(&cRoleLevel);
    nRetCode = getRoleLevel(&usRoleLevel);
    nRetCode = getUserPort(&usUserPort);
    nRetCode = getUserIP(&unUserIP);

    printf("--------------------------------------------------------AGIPUserEnterGame\n");
    printf("User_ID:\t%u\n", unUserID);
    printf("Role_ID:\t%u\n", unRoleID);
    printf("Server_ID:\t%u\n", unServerID);
    printf("Rating_ID:\t%u\n", unRatingID);

    // printf("Role_Level:\t%d\n", cRoleLevel);
    printf("Role_Level:\t%u\n", usRoleLevel);
    printf("User_Port:\t%u\n", usUserPort);
    printf("User_IP:\t%u\n", unUserIP);
    printf("--------------------------------------------------------AGIPUserEnterGame\n");
    return S_SUCCESS;
}

////////////////////////////
// AGIPUserEnterGameRes
AGIPUserEnterGameRes::AGIPUserEnterGameRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_ENTER_GAME_RES, sizeof(SAGIPUserEnterGameRes) - sizeof(BALANCE_INFO))
{

}

AGIPUserEnterGameRes::~AGIPUserEnterGameRes()
{

}

int AGIPUserEnterGameRes::initialize()
{
    int nRetCode = E_ERROR;

    m_usTotalLength = sizeof(SAGIPUserEnterGameRes) - sizeof(BALANCE_INFO);
    nRetCode = SysProtocol::initialize();
    ASSERT(nRetCode == S_SUCCESS);

    return S_SUCCESS;
}

int AGIPUserEnterGameRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserEnterGameRes *pSAGIPUserEnterGameRes = (SAGIPUserEnterGameRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserEnterGameRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserEnterGameRes::getBalanceCount(int32_t *pnBalanceCount)
{
    ASSERT(pnBalanceCount != NULL);
    SAGIPUserEnterGameRes *pSAGIPUserEnterGameRes = (SAGIPUserEnterGameRes *)m_pucPDU;
    *pnBalanceCount = htonl(pSAGIPUserEnterGameRes->n_Balance_Count);
    return S_SUCCESS;
}

int AGIPUserEnterGameRes::getBalanceInfo(int32_t nIndex, int32_t *pnSubjectID, int32_t *pnAmount)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    BALANCE_INFO *pBalanceInfoBuffer = NULL;
    int32_t nBalanceCount = 0;

    ASSERT(pnSubjectID != NULL && pnAmount != NULL);
    SAGIPUserEnterGameRes *pSAGIPUserEnterGameRes = (SAGIPUserEnterGameRes *)m_pucPDU;
    nBalanceCount = htonl(pSAGIPUserEnterGameRes->n_Balance_Count);
    if (
        (nIndex < 0) ||
        (nIndex >= nBalanceCount)
        )
    {
        goto ExitError;
    }
    pBalanceInfoBuffer = (BALANCE_INFO *)&pSAGIPUserEnterGameRes->n_Subject_ID;
    pBalanceInfoBuffer += nIndex;

    *pnSubjectID = htonl(pBalanceInfoBuffer->nSubjectID);
    *pnAmount    = htonl(pBalanceInfoBuffer->nAmount);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPUserEnterGameRes::getBalanceInfo(int32_t nIndex, BALANCE_INFO *pBalanceInfo)
{
    ASSERT(pBalanceInfo != NULL);
    return this->getBalanceInfo(nIndex, &pBalanceInfo->nSubjectID, &pBalanceInfo->nAmount);
}


int AGIPUserEnterGameRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserEnterGameRes *pSAGIPUserEnterGameRes = (SAGIPUserEnterGameRes *)m_pucPDU;

    pSAGIPUserEnterGameRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPUserEnterGameRes::setBalanceCount(int32_t nBalanceCount)
{
    ASSERT(false && "Balance Count is calculated automatically. Pls use addBalanceInfo() method!!");
    nBalanceCount = 0;
    return E_ERROR;
}

int AGIPUserEnterGameRes::addBalanceInfo(int32_t nSubjectID, int32_t nAmount)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    BALANCE_INFO *pBalanceInfoBuffer = NULL;
    int32_t nBalanceCount = 0;

    SAGIPUserEnterGameRes *pSAGIPUserEnterGameRes = (SAGIPUserEnterGameRes *)m_pucPDU;
    nBalanceCount = htonl(pSAGIPUserEnterGameRes->n_Balance_Count);

    pBalanceInfoBuffer = (BALANCE_INFO *)&pSAGIPUserEnterGameRes->n_Subject_ID;
    pBalanceInfoBuffer += nBalanceCount;

    pBalanceInfoBuffer->nSubjectID = htonl(nSubjectID);
    pBalanceInfoBuffer->nAmount    = htonl(nAmount);

    nBalanceCount++;
    pSAGIPUserEnterGameRes->n_Balance_Count = htonl(nBalanceCount);
    m_usTotalLength += sizeof(BALANCE_INFO);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
    //ExitError:
    return nResult;
}

int AGIPUserEnterGameRes::addBalanceInfo(const BALANCE_INFO *pBalanceInfo)
{
    ASSERT(pBalanceInfo != NULL);
    return this->addBalanceInfo(pBalanceInfo->nSubjectID, pBalanceInfo->nAmount);
}

int AGIPUserEnterGameRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode   = 0;
    int32_t   nBalanceCount = 0;
    BALANCE_INFO balanceInfo = {0, 0};

    SysProtocol::showInfo();

    nRetCode = getResultCode(&nResultCode);
    nRetCode = getBalanceCount(&nBalanceCount);
    printf("--------------------------------------------------------AGIPUserEnterGameRes\n");
    printf("Result_Code:\t%d\n", nResultCode);
    printf("Balance_Count:\t%d\n", nBalanceCount);
    for (int i = 0; i < nBalanceCount; i++)
    {
        nRetCode = getBalanceInfo(i, &balanceInfo);
        printf("Index[%d]: Subject_ID = %d, Amount = %d.\n", i, balanceInfo.nSubjectID, balanceInfo.nAmount);
    }

    printf("--------------------------------------------------------AGIPUserEnterGameRes\n");
    return S_SUCCESS;
}

