#include "AGIPUserBalanceInfo.h"

AGIPUserBalanceInfo::AGIPUserBalanceInfo(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_BALANCE_INFO, sizeof(SAGIPUserBalanceInfo))
{
}

AGIPUserBalanceInfo::~AGIPUserBalanceInfo(void)
{
}

int AGIPUserBalanceInfo::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserBalanceInfo *pSAGIPUserBalanceInfo = (SAGIPUserBalanceInfo *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserBalanceInfo->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserBalanceInfo::getRoleID(uint32_t *punRoleID)
{
    ASSERT(punRoleID != NULL);
    SAGIPUserBalanceInfo *pSAGIPUserBalanceInfo = (SAGIPUserBalanceInfo *)m_pucPDU;
    *punRoleID = htonl(pSAGIPUserBalanceInfo->un_Role_ID);
    return S_SUCCESS;
}

int AGIPUserBalanceInfo::getRatingID(uint32_t *punRatingID)
{
    ASSERT(punRatingID != NULL);
    SAGIPUserBalanceInfo *pSAGIPUserBalanceInfo = (SAGIPUserBalanceInfo *)m_pucPDU;
    *punRatingID = htonl(pSAGIPUserBalanceInfo->un_Rating_ID);
    return S_SUCCESS;
}

int AGIPUserBalanceInfo::setUserID(uint32_t unUserID)
{
    SAGIPUserBalanceInfo *pSAGIPUserBalanceInfo = (SAGIPUserBalanceInfo *)m_pucPDU;
    pSAGIPUserBalanceInfo->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserBalanceInfo::setRoleID(uint32_t unRoleID)
{
    SAGIPUserBalanceInfo *pSAGIPUserBalanceInfo = (SAGIPUserBalanceInfo *)m_pucPDU;
    pSAGIPUserBalanceInfo->un_Role_ID = htonl(unRoleID);
    return S_SUCCESS;
}

int AGIPUserBalanceInfo::setRatingID(uint32_t unRatingID)
{
    SAGIPUserBalanceInfo *pSAGIPUserBalanceInfo = (SAGIPUserBalanceInfo *)m_pucPDU;
    pSAGIPUserBalanceInfo->un_Rating_ID = htonl(unRatingID);
    return S_SUCCESS;
}

int AGIPUserBalanceInfo::showInfo()
{
    uint32_t un_User_ID = 0;
    uint32_t un_Role_ID = 0;
    uint32_t un_Rating_ID = 0;

    getUserID(&un_User_ID);
    getRoleID(&un_Role_ID);
    getRatingID(&un_Role_ID);

    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPUserBalanceInfo\n");
    printf("UserID:     %u\n", un_User_ID);
    printf("RoleID:     %u\n", un_Role_ID);
    printf("RatingID:   %u\n", un_Rating_ID);
    printf("--------------------------------------------------------AGIPUserBalanceInfo\n");

    return S_SUCCESS;
}

////////////////////////////////////////////////
AGIPUserBalanceInfoRes::AGIPUserBalanceInfoRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_BALANCE_INFO_RES, sizeof(SAGIPUserBalanceInfoRes) - sizeof(ALL_BALANCE_INFO))
{

}

AGIPUserBalanceInfoRes::~AGIPUserBalanceInfoRes(void)
{

}


int AGIPUserBalanceInfoRes::initialize()
{
    int nRetCode = E_ERROR;

    m_usTotalLength = sizeof(SAGIPUserBalanceInfoRes) - sizeof(ALL_BALANCE_INFO);
    nRetCode = SysProtocol::initialize();
    ASSERT(nRetCode == S_SUCCESS);

    return S_SUCCESS;
}

int AGIPUserBalanceInfoRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserBalanceInfoRes *pSAGIPUserBalanceInfoRes = (SAGIPUserBalanceInfoRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserBalanceInfoRes->n_Result_Code);
    return S_SUCCESS;
}


int AGIPUserBalanceInfoRes::getBalanceCount(int32_t *pnBalanceCount)
{
    ASSERT(pnBalanceCount != NULL);
    SAGIPUserBalanceInfoRes *pSAGIPUserBalanceInfoRes = (SAGIPUserBalanceInfoRes *)m_pucPDU;
    *pnBalanceCount = htonl(pSAGIPUserBalanceInfoRes->n_Balance_Count);
    return S_SUCCESS;
}

int AGIPUserBalanceInfoRes::getBalanceInfo(int32_t nIndex, int32_t *pnSubjectID, int32_t *pnAmount, int32_t *pnLeftAmount)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    ALL_BALANCE_INFO *pAllBalanceInfoBuffer = NULL;
    int32_t nBalanceCount = 0;

    ASSERT(pnSubjectID != NULL && pnAmount != NULL && pnLeftAmount != NULL);
    SAGIPUserBalanceInfoRes *pSAGIPUserBalanceInfoRes = (SAGIPUserBalanceInfoRes *)m_pucPDU;
    nBalanceCount = htonl(pSAGIPUserBalanceInfoRes->n_Balance_Count);
    if (
        (nIndex < 0) ||
        (nIndex >= nBalanceCount)
    )
    {
        goto ExitError;
    }
    pAllBalanceInfoBuffer  = (ALL_BALANCE_INFO *)&pSAGIPUserBalanceInfoRes->n_Subject_ID;
    pAllBalanceInfoBuffer += nIndex;

    *pnSubjectID  = htonl(pAllBalanceInfoBuffer->nSubjectID);
    *pnAmount     = htonl(pAllBalanceInfoBuffer->nAmount);
    *pnLeftAmount = htonl(pAllBalanceInfoBuffer->nLeftAmount);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPUserBalanceInfoRes::getBalanceInfo(int32_t nIndex, ALL_BALANCE_INFO *pAllBalanceInfo)
{
    ASSERT(pAllBalanceInfo != NULL);
    return this->getBalanceInfo(nIndex, &pAllBalanceInfo->nSubjectID, &pAllBalanceInfo->nAmount, &pAllBalanceInfo->nLeftAmount);
}


int AGIPUserBalanceInfoRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserBalanceInfoRes *pSAGIPUserBalanceInfoRes = (SAGIPUserBalanceInfoRes *)m_pucPDU;
    pSAGIPUserBalanceInfoRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}


int AGIPUserBalanceInfoRes::setBalanceCount(int32_t nBalanceCount)
{
    ASSERT(false && "Balance Count is calculated automatically. Pls use addBalanceInfo() method!!");
    nBalanceCount = 0;
    return E_ERROR;
}

int AGIPUserBalanceInfoRes::addBalanceInfo(int32_t nSubjectID, int32_t nAmount, int32_t nLeftAmount)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    ALL_BALANCE_INFO *pAllBalanceInfoBuffer = NULL;
    int32_t nBalanceCount = 0;

    SAGIPUserBalanceInfoRes *pSAGIPUserBalanceInfoRes = (SAGIPUserBalanceInfoRes *)m_pucPDU;
    nBalanceCount = htonl(pSAGIPUserBalanceInfoRes->n_Balance_Count);

    pAllBalanceInfoBuffer  = (ALL_BALANCE_INFO *)&pSAGIPUserBalanceInfoRes->n_Subject_ID;
    pAllBalanceInfoBuffer += nBalanceCount;

    pAllBalanceInfoBuffer->nSubjectID  = htonl(nSubjectID);
    pAllBalanceInfoBuffer->nAmount     = htonl(nAmount);
    pAllBalanceInfoBuffer->nLeftAmount = htonl(nLeftAmount);

    nBalanceCount++;
    pSAGIPUserBalanceInfoRes->n_Balance_Count = htonl(nBalanceCount);
    m_usTotalLength += sizeof(ALL_BALANCE_INFO);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
    //ExitError:
    return nResult;
}

int AGIPUserBalanceInfoRes::addBalanceInfo(const ALL_BALANCE_INFO *pAllBalanceInfo)
{
    ASSERT(pAllBalanceInfo != NULL);
    return this->addBalanceInfo(pAllBalanceInfo->nSubjectID, pAllBalanceInfo->nAmount, pAllBalanceInfo->nLeftAmount);
}

int AGIPUserBalanceInfoRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode   = 0;
    int32_t   nBalanceCount = 0;
    ALL_BALANCE_INFO allBalanceInfo = {0, 0, 0};

    SysProtocol::showInfo();

    nRetCode = getResultCode(&nResultCode);
    nRetCode = getBalanceCount(&nBalanceCount);
    printf("--------------------------------------------------------AGIPUserBalanceInfoRes\n");
    printf("Result_Code:\t%d\n", nResultCode);
    printf("Balance_Count:\t%d\n", nBalanceCount);
    for (int i = 0; i < nBalanceCount; i++)
    {
        nRetCode = getBalanceInfo(i, &allBalanceInfo);
        printf(
            "Index[%d]: Subject_ID = %d, Amount = %d, Left_Amount = %d.\n",
            i,
            allBalanceInfo.nSubjectID,
            allBalanceInfo.nAmount,
            allBalanceInfo.nLeftAmount
        );
    }
    printf("--------------------------------------------------------AGIPUserBalanceInfoRes\n");
    return S_SUCCESS;
}

