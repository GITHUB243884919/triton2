#include "AGIPUserItemInfo.h"

AGIPUserItemInfo::AGIPUserItemInfo(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_ITEM_INFO, sizeof(SAGIPUserItemInfo))
{
}

AGIPUserItemInfo::~AGIPUserItemInfo(void)
{
}


int AGIPUserItemInfo::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserItemInfo *pSAGIPUserItemInfo = (SAGIPUserItemInfo *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserItemInfo->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserItemInfo::getRoleID(uint32_t *punRoleID)
{
    ASSERT(punRoleID != NULL);
    SAGIPUserItemInfo *pSAGIPUserItemInfo = (SAGIPUserItemInfo *)m_pucPDU;
    *punRoleID = htonl(pSAGIPUserItemInfo->un_Role_ID);
    return S_SUCCESS;
}

int AGIPUserItemInfo::getActivityID(uint32_t *punActivityID)
{
    ASSERT(punActivityID != NULL);
    SAGIPUserItemInfo *pSAGIPUserItemInfo = (SAGIPUserItemInfo *)m_pucPDU;
    *punActivityID = htonl(pSAGIPUserItemInfo->un_Activity_ID);
    return S_SUCCESS;
}

int AGIPUserItemInfo::getItemCode(char *strItemCode)
{
    ASSERT(strItemCode != NULL);
    SAGIPUserItemInfo *pSAGIPUserItemInfo = (SAGIPUserItemInfo *)m_pucPDU;
    strncpy(strItemCode, pSAGIPUserItemInfo->str_Item_Code, AGIP_ITEM_CODE_LEN);
    return S_SUCCESS;
}

int AGIPUserItemInfo::getItemGatewayID(uint32_t *punItemGatewayID)
{
    ASSERT(punItemGatewayID != NULL);
    SAGIPUserItemInfo *pSAGIPUserItemInfo = (SAGIPUserItemInfo *)m_pucPDU;
    *punItemGatewayID = htonl(pSAGIPUserItemInfo->un_Item_Gateway_ID);
    return S_SUCCESS;
}

int AGIPUserItemInfo::setUserID(uint32_t unUserID)
{
    SAGIPUserItemInfo *pSAGIPUserItemInfo = (SAGIPUserItemInfo *)m_pucPDU;
    pSAGIPUserItemInfo->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserItemInfo::setRoleID(uint32_t unRoleID)
{
    SAGIPUserItemInfo *pSAGIPUserItemInfo = (SAGIPUserItemInfo *)m_pucPDU;
    pSAGIPUserItemInfo->un_Role_ID = htonl(unRoleID);
    return S_SUCCESS;
}

int AGIPUserItemInfo::setActivityID(uint32_t unActivityID)
{
    SAGIPUserItemInfo *pSAGIPUserItemInfo = (SAGIPUserItemInfo *)m_pucPDU;
    pSAGIPUserItemInfo->un_Activity_ID = htonl(unActivityID);
    return S_SUCCESS;
}

int AGIPUserItemInfo::setItemCode(const char *strItemCode)
{
    ASSERT(strItemCode != NULL);
    SAGIPUserItemInfo *pSAGIPUserItemInfo = (SAGIPUserItemInfo *)m_pucPDU;
    strncpy(pSAGIPUserItemInfo->str_Item_Code, strItemCode, AGIP_ITEM_CODE_LEN);
    return S_SUCCESS;
}

int AGIPUserItemInfo::setItemGatewayID(uint32_t unItemGatewayID)
{
    SAGIPUserItemInfo *pSAGIPUserItemInfo = (SAGIPUserItemInfo *)m_pucPDU;
    pSAGIPUserItemInfo->un_Item_Gateway_ID = htonl(unItemGatewayID);
    return S_SUCCESS;
}

///////////////////////
// AGIPUserItemInfoRes
AGIPUserItemInfoRes::AGIPUserItemInfoRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_ITEM_INFO_RES, sizeof(SAGIPUserItemInfoRes))
{

}

AGIPUserItemInfoRes::~AGIPUserItemInfoRes(void)
{

}



int AGIPUserItemInfoRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserItemInfoRes *pSAGIPUserItemInfoRes = (SAGIPUserItemInfoRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserItemInfoRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserItemInfoRes::getItemNum(int32_t *pnItemNum)
{
    ASSERT(pnItemNum != NULL);
    SAGIPUserItemInfoRes *pSAGIPUserItemInfoRes = (SAGIPUserItemInfoRes *)m_pucPDU;
    *pnItemNum = htonl(pSAGIPUserItemInfoRes->n_Item_Num);
    return S_SUCCESS;
}

int AGIPUserItemInfoRes::getBeginTime(int32_t *pnBeginTime)
{
    ASSERT(pnBeginTime != NULL);
    SAGIPUserItemInfoRes *pSAGIPUserItemInfoRes = (SAGIPUserItemInfoRes *)m_pucPDU;
    *pnBeginTime = htonl(pSAGIPUserItemInfoRes->n_Begin_Time);
    return S_SUCCESS;
}

int AGIPUserItemInfoRes::getEndTime(int32_t *pnEndTime)
{
    ASSERT(pnEndTime != NULL);
    SAGIPUserItemInfoRes *pSAGIPUserItemInfoRes = (SAGIPUserItemInfoRes *)m_pucPDU;
    *pnEndTime = htonl(pSAGIPUserItemInfoRes->n_End_Time);
    return S_SUCCESS;
}

int AGIPUserItemInfoRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserItemInfoRes *pSAGIPUserItemInfoRes = (SAGIPUserItemInfoRes *)m_pucPDU;
    pSAGIPUserItemInfoRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPUserItemInfoRes::setItemNum(int32_t nItemNum)
{
    SAGIPUserItemInfoRes *pSAGIPUserItemInfoRes = (SAGIPUserItemInfoRes *)m_pucPDU;
    pSAGIPUserItemInfoRes->n_Item_Num = htonl(nItemNum);
    return S_SUCCESS;
}

int AGIPUserItemInfoRes::setBeginTime(int32_t nBeginTime)
{
    SAGIPUserItemInfoRes *pSAGIPUserItemInfoRes = (SAGIPUserItemInfoRes *)m_pucPDU;
    pSAGIPUserItemInfoRes->n_Begin_Time = htonl(nBeginTime);
    return S_SUCCESS;
}

int AGIPUserItemInfoRes::setEndTime(int32_t nEndTime)
{
    SAGIPUserItemInfoRes *pSAGIPUserItemInfoRes = (SAGIPUserItemInfoRes *)m_pucPDU;
    pSAGIPUserItemInfoRes->n_End_Time = htonl(nEndTime);
    return S_SUCCESS;
}
