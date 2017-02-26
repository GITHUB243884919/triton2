#include "AGIPGamePasswordState.h"

AGIPGamePasswordState::AGIPGamePasswordState(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_GAME_PASSWORD_STATE, sizeof(SAGIPGamePasswordState))
{
}

AGIPGamePasswordState::~AGIPGamePasswordState(void)
{
}

int AGIPGamePasswordState::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPGamePasswordState *pSAGIPGamePasswordState = (SAGIPGamePasswordState *)m_pucPDU;
    *punUserID = htonl(pSAGIPGamePasswordState->un_User_ID);
    return S_SUCCESS;
}

int AGIPGamePasswordState::getGatewayID(uint32_t *punGatewayID)
{
    ASSERT(punGatewayID != NULL);
    SAGIPGamePasswordState *pSAGIPGamePasswordState = (SAGIPGamePasswordState *)m_pucPDU;
    *punGatewayID = htonl(pSAGIPGamePasswordState->un_Gateway_ID);
    return S_SUCCESS;
}

int AGIPGamePasswordState::setUserID(uint32_t unUserID)
{
    SAGIPGamePasswordState *pSAGIPGamePasswordState = (SAGIPGamePasswordState *)m_pucPDU;
    pSAGIPGamePasswordState->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPGamePasswordState::setGatewayID(uint32_t unGatewayID)
{
    SAGIPGamePasswordState *pSAGIPGamePasswordState = (SAGIPGamePasswordState *)m_pucPDU;
    pSAGIPGamePasswordState->un_Gateway_ID = htonl(unGatewayID);
    return S_SUCCESS;
}

int AGIPGamePasswordState::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t  unUserID      = 0;
    uint32_t  unGatewayID   = 0;

    SysProtocol::showInfo();
    nRetCode = getUserID(&unUserID);
    nRetCode = getGatewayID(&unGatewayID);

    printf("--------------------------------------------------------AGIPGamePasswordState\n");
    printf("User_ID:\t%u\n", unUserID);
    printf("Gateway_ID:\t%u\n", unGatewayID);
    printf("--------------------------------------------------------AGIPGamePasswordState\n");
    return S_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
AGIPGamePasswordStateRes::AGIPGamePasswordStateRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_GAME_PASSWORD_STATE_RES, sizeof(SAGIPGamePasswordStateRes) - 2 * sizeof(GAMEPASSWORDSTATE))
{

}

AGIPGamePasswordStateRes::~AGIPGamePasswordStateRes(void)
{

}


int AGIPGamePasswordStateRes::initialize()
{
    int nRetCode = E_ERROR;

    m_usTotalLength = sizeof(SAGIPGamePasswordStateRes) - sizeof(ALL_BALANCE_INFO);
    nRetCode = SysProtocol::initialize();
    ASSERT(nRetCode == S_SUCCESS);

    return S_SUCCESS;
}

int AGIPGamePasswordStateRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPGamePasswordStateRes *pSAGIPGamePasswordStateRes = (SAGIPGamePasswordStateRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPGamePasswordStateRes->n_Result_Code);
    return S_SUCCESS;
}


int AGIPGamePasswordStateRes::getPasswordCount(int32_t *pnPasswordCount)
{
    ASSERT(pnPasswordCount != NULL);
    SAGIPGamePasswordStateRes *pSAGIPGamePasswordStateRes = (SAGIPGamePasswordStateRes *)m_pucPDU;
    *pnPasswordCount = htonl(pSAGIPGamePasswordStateRes->n_Password_Count);
    return S_SUCCESS;
}

int AGIPGamePasswordStateRes::getPasswordState(int32_t nIndex, uint8_t* pcPasswordType, uint8_t* pcPasswordState)
{
    int nResult  = E_PDU_INVALID_FIELD;

    PGAMEPASSWORDSTATE pGamePasswordStateBuffer;
    ASSERT(pcPasswordType != NULL && pcPasswordState != NULL);

    SAGIPGamePasswordStateRes *pSAGIPGamePasswordStateRes = (SAGIPGamePasswordStateRes *)m_pucPDU;
    int32_t nPasswordCount = htonl(pSAGIPGamePasswordStateRes->n_Password_Count);
    if (
        (nIndex < 0) ||
        (nIndex >= nPasswordCount)
    )
    {
        goto ExitError;
    }
    pGamePasswordStateBuffer = (PGAMEPASSWORDSTATE)&pSAGIPGamePasswordStateRes->c_Password_Type;
    pGamePasswordStateBuffer += nIndex;

    *pcPasswordType  = pGamePasswordStateBuffer->cPasswordType;
    *pcPasswordState = pGamePasswordStateBuffer->cPasswordState;

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPGamePasswordStateRes::getPasswordState(int32_t nIndex, PGAMEPASSWORDSTATE pGamePasswordState)
{
    ASSERT(pGamePasswordState != NULL);
    return this->getPasswordState(nIndex, &pGamePasswordState->cPasswordType, &pGamePasswordState->cPasswordState);
}


int AGIPGamePasswordStateRes::setResultCode(int32_t nResultCode)
{
    SAGIPGamePasswordStateRes *pSAGIPGamePasswordStateRes = (SAGIPGamePasswordStateRes *)m_pucPDU;
    pSAGIPGamePasswordStateRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}


int AGIPGamePasswordStateRes::setPasswordCount(uint32_t unPasswordCount)
{
    ASSERT(false && "Password Count is calculated automatically. Pls use addPasswordState() method!!");
    unPasswordCount = 0;
    return E_ERROR;
}

int AGIPGamePasswordStateRes::addPasswordState(uint8_t cPasswordType, uint8_t cPasswordState)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    SAGIPGamePasswordStateRes *pSAGIPGamePasswordStateRes = (SAGIPGamePasswordStateRes *)m_pucPDU;
    int32_t nPasswordCount = htonl(pSAGIPGamePasswordStateRes->n_Password_Count);

    PGAMEPASSWORDSTATE pAllBalanceInfoBuffer  = (PGAMEPASSWORDSTATE)&pSAGIPGamePasswordStateRes->c_Password_Type;
    pAllBalanceInfoBuffer += nPasswordCount;

    pAllBalanceInfoBuffer->cPasswordType  = cPasswordType;
    pAllBalanceInfoBuffer->cPasswordState = cPasswordState;

    pSAGIPGamePasswordStateRes->n_Password_Count = htonl(++nPasswordCount);
    // 一次增加两个PasswordState的长度。
    if (nPasswordCount%2)
    {
        m_usTotalLength += 2 * sizeof(GAMEPASSWORDSTATE);
    }
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
    //ExitError:
    return nResult;
}

int AGIPGamePasswordStateRes::addPasswordState(const PGAMEPASSWORDSTATE pGamePasswordState)
{
    ASSERT(pGamePasswordState != NULL);
    return this->addPasswordState(pGamePasswordState->cPasswordType, pGamePasswordState->cPasswordState);
}

int AGIPGamePasswordStateRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode   = 0;
    int32_t   nPasswordCount = 0;
    GAMEPASSWORDSTATE gamePasswordState = {0, 0};

    SysProtocol::showInfo();

    nRetCode = getResultCode(&nResultCode);
    nRetCode = getPasswordCount(&nPasswordCount);
    printf("--------------------------------------------------------AGIPGamePasswordStateRes\n");
    printf("Result_Code:\t%d\n", nResultCode);
    printf("Password_Count:\t%d\n", nPasswordCount);
    for (int i = 0; i < nPasswordCount; i++)
    {
        nRetCode = getPasswordState(i, &gamePasswordState);
        printf(
            "Index[%d]: PasswordType = %d, PasswordState = %d.\n",
            i,
            gamePasswordState.cPasswordType,
            gamePasswordState.cPasswordState
        );
    }
    printf("--------------------------------------------------------AGIPGamePasswordStateRes\n");
    return S_SUCCESS;
}

