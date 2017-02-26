#include "AGIPUserUseProduct.h"

AGIPUserUseProduct::AGIPUserUseProduct(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_USE_PRODUCT, sizeof(SAGIPUserUseProduct))
{
}

AGIPUserUseProduct::~AGIPUserUseProduct(void)
{
}

int AGIPUserUseProduct::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserUseProduct *pSAGIPUserUseProduct = (SAGIPUserUseProduct *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserUseProduct->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserUseProduct::getRoleID(uint32_t *punRoleID)
{
    ASSERT(punRoleID != NULL);
    SAGIPUserUseProduct *pSAGIPUserUseProduct = (SAGIPUserUseProduct *)m_pucPDU;
    *punRoleID = htonl(pSAGIPUserUseProduct->un_Role_ID);
    return S_SUCCESS;
}

int AGIPUserUseProduct::getPurchaseID(uint64_t *pullPurchaseID, int32_t flag)
{
    ASSERT(pullPurchaseID != NULL);
    SAGIPUserUseProduct *pSAGIPUserUseProduct = (SAGIPUserUseProduct *)m_pucPDU;
    uint32_t* punPurchaseID = NULL;
    punPurchaseID = (uint32_t*)pullPurchaseID;

    uint64_t ullDetailID = 0;
    memcpy(&ullDetailID, &pSAGIPUserUseProduct->un_Purchase_ID_Higher, sizeof(uint64_t));

    *pullPurchaseID = ntohll(ullDetailID, flag);

    return S_SUCCESS;
}

int AGIPUserUseProduct::getPurchaseID(PURCHASE_ID *psPurchaseID)
{
    ASSERT(psPurchaseID != NULL);
    SAGIPUserUseProduct *pSAGIPUserUseProduct = (SAGIPUserUseProduct *)m_pucPDU;
    psPurchaseID->un_Purchase_ID_Higher = ntohl(pSAGIPUserUseProduct->un_Purchase_ID_Higher);
    psPurchaseID->un_Purchase_ID_Lower  = ntohl(pSAGIPUserUseProduct->un_Purchase_ID_Lower);
    return S_SUCCESS;
}

int AGIPUserUseProduct::getPurchaseID(uint32_t *punPurchaseIDHigher, uint32_t *punPurchaseIDLower)
{
    ASSERT(punPurchaseIDHigher != NULL);
    ASSERT(punPurchaseIDLower  != NULL);
    SAGIPUserUseProduct *pSAGIPUserUseProduct = (SAGIPUserUseProduct *)m_pucPDU;
    *punPurchaseIDHigher = ntohl(pSAGIPUserUseProduct->un_Purchase_ID_Higher);
    *punPurchaseIDLower  = ntohl(pSAGIPUserUseProduct->un_Purchase_ID_Lower);
    return S_SUCCESS;
}

int AGIPUserUseProduct::getRoleGender(int8_t *pcGender)
{
    ASSERT(pcGender != NULL);
    *pcGender = ((SAGIPUserUseProduct*) m_pucPDU)->c_Role_Gender;
    return S_SUCCESS;
}

int AGIPUserUseProduct::getRoleOccupation(int8_t *pcOccupation)
{
    ASSERT(pcOccupation != NULL);
    *pcOccupation = ((SAGIPUserUseProduct*) m_pucPDU)->c_Role_Occupation;
    return S_SUCCESS;
}

int AGIPUserUseProduct::getRoleLevel(uint16_t *pusLevel)
{
    ASSERT(pusLevel != NULL);
    *pusLevel = ntohs( ((SAGIPUserUseProduct*) m_pucPDU)->us_Role_Level );
    return S_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
int AGIPUserUseProduct::setUserID(uint32_t unUserID)
{
    SAGIPUserUseProduct *pSAGIPUserUseProduct = (SAGIPUserUseProduct *)m_pucPDU;
    pSAGIPUserUseProduct->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserUseProduct::setRoleID(uint32_t unRoleID)
{
    SAGIPUserUseProduct *pSAGIPUserUseProduct = (SAGIPUserUseProduct *)m_pucPDU;
    pSAGIPUserUseProduct->un_Role_ID = htonl(unRoleID);
    return S_SUCCESS;
}

int AGIPUserUseProduct::setPurchaseID(uint64_t ullPurchaseID, int32_t flag)
{
    SAGIPUserUseProduct *pSAGIPUserUseProduct = (SAGIPUserUseProduct *)m_pucPDU;
    uint32_t* punPurchaseID = NULL;

    punPurchaseID = (uint32_t*)&ullPurchaseID;

    uint64_t ullDetailID = htonll(ullPurchaseID, flag);
    memcpy(&pSAGIPUserUseProduct->un_Purchase_ID_Higher, &ullDetailID, sizeof(uint64_t));

    return S_SUCCESS;
}

int AGIPUserUseProduct::setPurchaseID(const PURCHASE_ID& sPurchaseID)
{
    SAGIPUserUseProduct *pSAGIPUserUseProduct = (SAGIPUserUseProduct *)m_pucPDU;
    pSAGIPUserUseProduct->un_Purchase_ID_Higher = htonl(sPurchaseID.un_Purchase_ID_Higher);
    pSAGIPUserUseProduct->un_Purchase_ID_Lower  = htonl(sPurchaseID.un_Purchase_ID_Lower);
    return S_SUCCESS;
}

int AGIPUserUseProduct::setPurchaseID(uint32_t unPurchaseIDHigher, uint32_t unPurchaseIDLower)
{
    SAGIPUserUseProduct *pSAGIPUserUseProduct = (SAGIPUserUseProduct *)m_pucPDU;
    pSAGIPUserUseProduct->un_Purchase_ID_Higher = htonl(unPurchaseIDHigher);
    pSAGIPUserUseProduct->un_Purchase_ID_Lower  = htonl(unPurchaseIDLower);
    return S_SUCCESS;
}

int AGIPUserUseProduct::setRoleGender(int8_t cGender)
{
    ((SAGIPUserUseProduct*) m_pucPDU)->c_Role_Gender = cGender;
    return S_SUCCESS;
}

int AGIPUserUseProduct::setRoleOccupation(int8_t cOccupation)
{
    ((SAGIPUserUseProduct*) m_pucPDU)->c_Role_Occupation = cOccupation;
    return S_SUCCESS;
}

int AGIPUserUseProduct::setRoleLevel(uint16_t usLevel)
{
    ((SAGIPUserUseProduct*) m_pucPDU)->us_Role_Level = htons(usLevel);
    return S_SUCCESS;
}

int AGIPUserUseProduct::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t   unUserID = 0;
    uint32_t   unRoleID = 0;
    uint64_t   ullPurchaseID = 0;

    SysProtocol::showInfo();
    nRetCode = getUserID(&unUserID);
    nRetCode = getRoleID(&unRoleID);
    nRetCode = getPurchaseID(&ullPurchaseID);
    printf("--------------------------------------------------------AGIPUserUseProduct\n");
    printf("User_ID:\t%u\n", unUserID);
    printf("Role_ID:\t%u\n", unRoleID);
    printf("Purchase_ID:\t%llu\n", ullPurchaseID);
    printf("--------------------------------------------------------AGIPUserUseProduct\n");
    return S_SUCCESS;
}
///////////////////////////
// AGIPUserUseProductRes

AGIPUserUseProductRes::AGIPUserUseProductRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_USE_PRODUCT_RES, sizeof(SAGIPUserUseProductRes))
{

}

AGIPUserUseProductRes::~AGIPUserUseProductRes(void)
{

}

int AGIPUserUseProductRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserUseProductRes *pSAGIPUserUseProductRes = (SAGIPUserUseProductRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserUseProductRes->n_Result_Code);

    return S_SUCCESS;
}

int AGIPUserUseProductRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserUseProductRes *pSAGIPUserUseProductRes = (SAGIPUserUseProductRes *)m_pucPDU;
    pSAGIPUserUseProductRes->n_Result_Code = htonl(nResultCode);

    return S_SUCCESS;
}

int AGIPUserUseProductRes::showInfo()
{
    int nRetCode        = E_ERROR;
    int32_t   nResultCode = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);
    printf("--------------------------------------------------------AGIPUserUseProductRes\n");
    printf("Result_Code:\t%d\n", nResultCode);
    printf("--------------------------------------------------------AGIPUserUseProductRes\n");
    return S_SUCCESS;
}


