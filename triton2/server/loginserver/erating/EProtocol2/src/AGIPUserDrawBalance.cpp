#include "AGIPUserDrawBalance.h"

AGIPUserDrawBalance::AGIPUserDrawBalance(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_DRAW_BALANCE, sizeof(SAGIPUserDrawBalance))

{
}

AGIPUserDrawBalance::~AGIPUserDrawBalance(void)
{
}

int AGIPUserDrawBalance::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserDrawBalance *pSAGIPUserDrawBalance = (SAGIPUserDrawBalance *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserDrawBalance->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserDrawBalance::getRoleID(uint32_t *punRoleID)
{
    ASSERT(punRoleID != NULL);
    SAGIPUserDrawBalance *pSAGIPUserDrawBalance = (SAGIPUserDrawBalance *)m_pucPDU;
    *punRoleID = htonl(pSAGIPUserDrawBalance->un_Role_ID);
    return S_SUCCESS;
}

int AGIPUserDrawBalance::getRatingID(uint32_t *punRatingID)
{
    ASSERT(punRatingID != NULL);
    SAGIPUserDrawBalance *pSAGIPUserDrawBalance = (SAGIPUserDrawBalance *)m_pucPDU;
    *punRatingID = htonl(pSAGIPUserDrawBalance->un_Rating_ID);
    return S_SUCCESS;
}

int AGIPUserDrawBalance::getSubjectID(int32_t *pnSubjectID)
{
    ASSERT(pnSubjectID != NULL);
    SAGIPUserDrawBalance *pSAGIPUserDrawBalance = (SAGIPUserDrawBalance *)m_pucPDU;
    *pnSubjectID = htonl(pSAGIPUserDrawBalance->n_Subject_ID);
    return S_SUCCESS;
}

int AGIPUserDrawBalance::getAmount(int32_t *pnAmount)
{
    ASSERT(pnAmount != NULL);
    SAGIPUserDrawBalance *pSAGIPUserDrawBalance = (SAGIPUserDrawBalance *)m_pucPDU;
    *pnAmount = htonl(pSAGIPUserDrawBalance->n_Amount);
    return S_SUCCESS;
}

int AGIPUserDrawBalance::setUserID(uint32_t unUserID)
{
    SAGIPUserDrawBalance *pSAGIPUserDrawBalance = (SAGIPUserDrawBalance *)m_pucPDU;
    pSAGIPUserDrawBalance->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserDrawBalance::setRoleID(uint32_t unRoleID)
{
    SAGIPUserDrawBalance *pSAGIPUserDrawBalance = (SAGIPUserDrawBalance *)m_pucPDU;
    pSAGIPUserDrawBalance->un_Role_ID = htonl(unRoleID);
    return S_SUCCESS;
}

int AGIPUserDrawBalance::setRatingID(uint32_t unRatingID)
{
    SAGIPUserDrawBalance *pSAGIPUserDrawBalance = (SAGIPUserDrawBalance *)m_pucPDU;
    pSAGIPUserDrawBalance->un_Rating_ID = htonl(unRatingID);
    return S_SUCCESS;
}

int AGIPUserDrawBalance::setSubjectID(int32_t nSubjectID)
{
    SAGIPUserDrawBalance *pSAGIPUserDrawBalance = (SAGIPUserDrawBalance *)m_pucPDU;
    pSAGIPUserDrawBalance->n_Subject_ID = htonl(nSubjectID);
    return S_SUCCESS;
}

int AGIPUserDrawBalance::setAmount(int32_t nAmount)
{
    SAGIPUserDrawBalance *pSAGIPUserDrawBalance = (SAGIPUserDrawBalance *)m_pucPDU;
    pSAGIPUserDrawBalance->n_Amount = htonl(nAmount);
    return S_SUCCESS;
}

int AGIPUserDrawBalance::showInfo()
{
    uint32_t        unUserID;
    uint32_t        unRoleID;
    uint32_t        unRatingID;
    int32_t         nSubjectID;
    int32_t         nAmount;

    getUserID(&unUserID);
    getRoleID(&unRoleID);
    getRatingID(&unRatingID);
    getSubjectID(&nSubjectID);
    getAmount(&nAmount);

    SysProtocol::showInfo();

    printf(
            "---------------------------------------------------AGIPUserDrawBalance\n"
            "UserID:            %u\n"
            "RoleID:            %u\n"
            "RatingID:          %u\n"
            "SubjectID:         %d\n"
            "Amount:            %d\n"
            "---------------------------------------------------AGIPUserDrawBalance\n",
            unUserID,
            unRoleID,
            unRatingID,
            nSubjectID,
            nAmount
          );

    return S_SUCCESS;
}

//////////////////////////////////

AGIPUserDrawBalanceRes::AGIPUserDrawBalanceRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_DRAW_BALANCE_RES, sizeof(SAGIPUserDrawBalanceRes))
{

}

AGIPUserDrawBalanceRes::~AGIPUserDrawBalanceRes(void)
{

}

int AGIPUserDrawBalanceRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserDrawBalanceRes *pSAGIPUserDrawBalanceRes = (SAGIPUserDrawBalanceRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserDrawBalanceRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserDrawBalanceRes::getAmount(int32_t *pnAmount)
{
    ASSERT(pnAmount != NULL);
    SAGIPUserDrawBalanceRes *pSAGIPUserDrawBalanceRes = (SAGIPUserDrawBalanceRes *)m_pucPDU;
    *pnAmount = htonl(pSAGIPUserDrawBalanceRes->n_Amount);
    return S_SUCCESS;
}

int AGIPUserDrawBalanceRes::getLeftAmount(int32_t *pnLeftAmount)
{
    ASSERT(pnLeftAmount != NULL);
    SAGIPUserDrawBalanceRes *pSAGIPUserDrawBalanceRes = (SAGIPUserDrawBalanceRes *)m_pucPDU;
    *pnLeftAmount = htonl(pSAGIPUserDrawBalanceRes->n_Left_Amount);
    return S_SUCCESS;
}

int AGIPUserDrawBalanceRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserDrawBalanceRes *pSAGIPUserDrawBalanceRes = (SAGIPUserDrawBalanceRes *)m_pucPDU;
    pSAGIPUserDrawBalanceRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPUserDrawBalanceRes::setAmount(int32_t nAmount)
{
    SAGIPUserDrawBalanceRes *pSAGIPUserDrawBalanceRes = (SAGIPUserDrawBalanceRes *)m_pucPDU;
    pSAGIPUserDrawBalanceRes->n_Amount = htonl(nAmount);
    return S_SUCCESS;
}

int AGIPUserDrawBalanceRes::setLeftAmount(int32_t nLeftAmount)
{
    SAGIPUserDrawBalanceRes *pSAGIPUserDrawBalanceRes = (SAGIPUserDrawBalanceRes *)m_pucPDU;
    pSAGIPUserDrawBalanceRes->n_Left_Amount = htonl(nLeftAmount);
    return S_SUCCESS;
}

int AGIPUserDrawBalanceRes::showInfo()
{
    int32_t         nResultCode;
    int32_t         nAmount;
    int32_t         nLeftAmount;

    getResultCode(&nResultCode);
    getAmount(&nAmount);
    getLeftAmount(&nLeftAmount);

    SysProtocol::showInfo();

    printf(
            "---------------------------------------------------AGIPUserDrawBalanceRes\n"
            "ResultCode:        %d\n"
            "Amount:            %d\n"
            "LeftAmount:        %d\n"
            "---------------------------------------------------AGIPUserDrawBalanceRes\n",
            nResultCode,
            nAmount,
            nLeftAmount
          );

    return S_SUCCESS;
}

