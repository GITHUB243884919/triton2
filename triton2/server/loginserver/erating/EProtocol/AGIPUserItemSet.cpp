#include "AGIPUserItemSet.h"

AGIPUserItemSet::AGIPUserItemSet(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_ITEM_SET, sizeof(SAGIPUserItemSet))
{
}

AGIPUserItemSet::~AGIPUserItemSet(void)
{
}

int AGIPUserItemSet::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserItemSet *pSAGIPUserItemSet = (SAGIPUserItemSet *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserItemSet->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserItemSet::getRoleID(uint32_t *punRoleID)
{
    ASSERT(punRoleID != NULL);
    SAGIPUserItemSet *pSAGIPUserItemSet = (SAGIPUserItemSet *)m_pucPDU;
    *punRoleID = htonl(pSAGIPUserItemSet->un_Role_ID);
    return S_SUCCESS;
}

int AGIPUserItemSet::getActivityID(uint32_t *punActivityID)
{
    ASSERT(punActivityID != NULL);
    SAGIPUserItemSet *pSAGIPUserItemSet = (SAGIPUserItemSet *)m_pucPDU;
    *punActivityID = htonl(pSAGIPUserItemSet->un_Activity_ID);
    return S_SUCCESS;
}

int AGIPUserItemSet::getItemCode(char *strItemCode)
{
    ASSERT(strItemCode != NULL);
    SAGIPUserItemSet *pSAGIPUserItemSet = (SAGIPUserItemSet *)m_pucPDU;
    strncpy(strItemCode, pSAGIPUserItemSet->str_Item_Code, AGIP_ITEM_CODE_LEN);
    return S_SUCCESS;
}

int AGIPUserItemSet::getItemNum(int32_t *pnItemNum)
{
    ASSERT(pnItemNum != NULL);
    SAGIPUserItemSet *pSAGIPUserItemSet = (SAGIPUserItemSet *)m_pucPDU;
    *pnItemNum = htonl(pSAGIPUserItemSet->n_Item_Num);
    return S_SUCCESS;
}

int AGIPUserItemSet::getItemGatewayID(uint32_t *punItemGatewayID)
{
    ASSERT(punItemGatewayID != NULL);
    SAGIPUserItemSet *pSAGIPUserItemSet = (SAGIPUserItemSet *)m_pucPDU;
    *punItemGatewayID = htonl(pSAGIPUserItemSet->un_Item_Gateway_ID);
    return S_SUCCESS;
}

int AGIPUserItemSet::setUserID(uint32_t unUserID)
{
    SAGIPUserItemSet *pSAGIPUserItemSet = (SAGIPUserItemSet *)m_pucPDU;
    pSAGIPUserItemSet->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserItemSet::setRoleID(uint32_t unRoleID)
{
    SAGIPUserItemSet *pSAGIPUserItemSet = (SAGIPUserItemSet *)m_pucPDU;
    pSAGIPUserItemSet->un_Role_ID = htonl(unRoleID);
    return S_SUCCESS;
}

int AGIPUserItemSet::setActivityID(uint32_t unActivityID)
{
    SAGIPUserItemSet *pSAGIPUserItemSet = (SAGIPUserItemSet *)m_pucPDU;
    pSAGIPUserItemSet->un_Activity_ID = htonl(unActivityID);
    return S_SUCCESS;
}

int AGIPUserItemSet::setItemCode(const char *strItemCode)
{
    ASSERT(strItemCode != NULL);
    SAGIPUserItemSet *pSAGIPUserItemSet = (SAGIPUserItemSet *)m_pucPDU;
    strncpy(pSAGIPUserItemSet->str_Item_Code, strItemCode, AGIP_ITEM_CODE_LEN);
    return S_SUCCESS;
}

int AGIPUserItemSet::setItemNum(int32_t nItemNum)
{
    SAGIPUserItemSet *pSAGIPUserItemSet = (SAGIPUserItemSet *)m_pucPDU;
    pSAGIPUserItemSet->n_Item_Num = htonl(nItemNum);
    return S_SUCCESS;
}

int AGIPUserItemSet::setItemGatewayID(uint32_t unItemGatewayID)
{
    SAGIPUserItemSet *pSAGIPUserItemSet = (SAGIPUserItemSet *)m_pucPDU;
    pSAGIPUserItemSet->un_Item_Gateway_ID = htonl(unItemGatewayID);
    return S_SUCCESS;
}

///////////////////////
// AGIPUserItemSetRes

AGIPUserItemSetRes::AGIPUserItemSetRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_ITEM_SET_RES, sizeof(SAGIPUserItemSetRes))
{

}

AGIPUserItemSetRes::~AGIPUserItemSetRes(void)
{

}

int AGIPUserItemSetRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserItemSetRes *pSAGIPUserItemSetRes = (SAGIPUserItemSetRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserItemSetRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserItemSetRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserItemSetRes *pSAGIPUserItemSetRes = (SAGIPUserItemSetRes *)m_pucPDU;
    pSAGIPUserItemSetRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

