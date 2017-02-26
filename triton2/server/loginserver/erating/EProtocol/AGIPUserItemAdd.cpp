#include "AGIPUserItemAdd.h"

AGIPUserItemAdd::AGIPUserItemAdd(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_ITEM_ADD, sizeof(SAGIPUserItemAdd))
{
}

AGIPUserItemAdd::~AGIPUserItemAdd(void)
{
}

int AGIPUserItemAdd::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserItemAdd *pSAGIPUserItemAdd = (SAGIPUserItemAdd *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserItemAdd->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserItemAdd::getRoleID(uint32_t *punRoleID)
{
    ASSERT(punRoleID != NULL);
    SAGIPUserItemAdd *pSAGIPUserItemAdd = (SAGIPUserItemAdd *)m_pucPDU;
    *punRoleID = htonl(pSAGIPUserItemAdd->un_Role_ID);
    return S_SUCCESS;
}

int AGIPUserItemAdd::getActivityID(uint32_t *punActivityID)
{
    ASSERT(punActivityID != NULL);
    SAGIPUserItemAdd *pSAGIPUserItemAdd = (SAGIPUserItemAdd *)m_pucPDU;
    *punActivityID = htonl(pSAGIPUserItemAdd->un_Activity_ID);
    return S_SUCCESS;
}

int AGIPUserItemAdd::getItemCode(char *strItemCode)
{
    ASSERT(strItemCode != NULL);
    SAGIPUserItemAdd *pSAGIPUserItemAdd = (SAGIPUserItemAdd *)m_pucPDU;
    strncpy(strItemCode, pSAGIPUserItemAdd->str_Item_Code, AGIP_ITEM_CODE_LEN);
    return S_SUCCESS;
}

int AGIPUserItemAdd::getItemNum(int32_t *pnItemNum)
{
    ASSERT(pnItemNum != NULL);
    SAGIPUserItemAdd *pSAGIPUserItemAdd = (SAGIPUserItemAdd *)m_pucPDU;
    *pnItemNum = htonl(pSAGIPUserItemAdd->n_Item_Num);
    return S_SUCCESS;
}

int AGIPUserItemAdd::getItemGatewayID(uint32_t *punItemGatewayID)
{
    ASSERT(punItemGatewayID != NULL);
    SAGIPUserItemAdd *pSAGIPUserItemAdd = (SAGIPUserItemAdd *)m_pucPDU;
    *punItemGatewayID = htonl(pSAGIPUserItemAdd->un_Item_Gateway_ID);
    return S_SUCCESS;
}

int AGIPUserItemAdd::setUserID(uint32_t unUserID)
{
    SAGIPUserItemAdd *pSAGIPUserItemAdd = (SAGIPUserItemAdd *)m_pucPDU;
    pSAGIPUserItemAdd->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserItemAdd::setRoleID(uint32_t unRoleID)
{
    SAGIPUserItemAdd *pSAGIPUserItemAdd = (SAGIPUserItemAdd *)m_pucPDU;
    pSAGIPUserItemAdd->un_Role_ID = htonl(unRoleID);
    return S_SUCCESS;
}

int AGIPUserItemAdd::setActivityID(uint32_t unActivityID)
{
    SAGIPUserItemAdd *pSAGIPUserItemAdd = (SAGIPUserItemAdd *)m_pucPDU;
    pSAGIPUserItemAdd->un_Activity_ID = htonl(unActivityID);
    return S_SUCCESS;
}

int AGIPUserItemAdd::setItemCode(const char *strItemCode)
{
    ASSERT(strItemCode != NULL);
    SAGIPUserItemAdd *pSAGIPUserItemAdd = (SAGIPUserItemAdd *)m_pucPDU;
    strncpy(pSAGIPUserItemAdd->str_Item_Code, strItemCode, AGIP_ITEM_CODE_LEN);
    return S_SUCCESS;
}

int AGIPUserItemAdd::setItemNum(int32_t nItemNum)
{
    SAGIPUserItemAdd *pSAGIPUserItemAdd = (SAGIPUserItemAdd *)m_pucPDU;
    pSAGIPUserItemAdd->n_Item_Num = htonl(nItemNum);
    return S_SUCCESS;
}

int AGIPUserItemAdd::setItemGatewayID(uint32_t unItemGatewayID)
{
    SAGIPUserItemAdd *pSAGIPUserItemAdd = (SAGIPUserItemAdd *)m_pucPDU;
    pSAGIPUserItemAdd->un_Item_Gateway_ID = htonl(unItemGatewayID);
    return S_SUCCESS;
}

///////////////////////
// AGIPUserItemAddRes

AGIPUserItemAddRes::AGIPUserItemAddRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_ITEM_ADD_RES, sizeof(SAGIPUserItemAddRes))
{

}

AGIPUserItemAddRes::~AGIPUserItemAddRes(void)
{

}

int AGIPUserItemAddRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserItemAddRes *pSAGIPUserItemAddRes = (SAGIPUserItemAddRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserItemAddRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserItemAddRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserItemAddRes *pSAGIPUserItemAddRes = (SAGIPUserItemAddRes *)m_pucPDU;
    pSAGIPUserItemAddRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

