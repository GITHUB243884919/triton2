#include "AGIPUserBuyProduct.h"

AGIPUserBuyProduct::AGIPUserBuyProduct(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_BUY_PRODUCT, sizeof(SAGIPUserBuyProduct))
{
}

AGIPUserBuyProduct::~AGIPUserBuyProduct(void)
{
}

int AGIPUserBuyProduct::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserBuyProduct->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getRoleID(uint32_t *punRoleID)
{
    ASSERT(punRoleID != NULL);
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *punRoleID = htonl(pSAGIPUserBuyProduct->un_Role_ID);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getRoleGender(int8_t *pcGender)
{
    ASSERT(pcGender != NULL);
    SAGIPUserBuyProduct* pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *pcGender = pSAGIPUserBuyProduct->c_Role_Gender;
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getRoleOccupation(int8_t *pcOccupation)
{
    ASSERT(pcOccupation != NULL);
    SAGIPUserBuyProduct* pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *pcOccupation = pSAGIPUserBuyProduct->c_Role_Occupation;
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getRoleLevel(uint16_t *pusLevel)
{
    ASSERT(pusLevel != NULL);
    SAGIPUserBuyProduct* pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *pusLevel = ntohs(pSAGIPUserBuyProduct->us_Role_Level);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getIBCode(char *strIBCode)
{
    ASSERT(strIBCode != NULL);
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    memcpy(strIBCode, pSAGIPUserBuyProduct->str_IB_Code, AGIP_IB_CODE_LEN);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getPackageFlag(int8_t *pcPackageFlag)
{
    ASSERT(pcPackageFlag != NULL);
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *pcPackageFlag = pSAGIPUserBuyProduct->c_Package_Flag;
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getAmount(int16_t *psAmount)
{
    ASSERT(psAmount != NULL);
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *psAmount = htons(pSAGIPUserBuyProduct->s_Amount);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getRatingID(uint32_t *punRatingID)
{
    ASSERT(punRatingID != NULL);
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *punRatingID = htonl(pSAGIPUserBuyProduct->un_Rating_ID);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getServerID(uint32_t *punServerID)
{
    ASSERT(punServerID != NULL);
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *punServerID = htonl(pSAGIPUserBuyProduct->un_Server_ID);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getUserIP(uint32_t *punUserIP)
{
    ASSERT(punUserIP != NULL);
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *punUserIP = htonl(pSAGIPUserBuyProduct->un_User_IP);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getSubjectID(int32_t *pnSubjectID)
{
    ASSERT(pnSubjectID != NULL);
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *pnSubjectID = htonl(pSAGIPUserBuyProduct->n_Subject_ID);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getPrice(int32_t *pnPrice)
{
    ASSERT(pnPrice != NULL);
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *pnPrice = htonl(pSAGIPUserBuyProduct->n_Price);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::getDiscountPrice(int32_t *pnDiscountPrice)
{
    ASSERT(pnDiscountPrice != NULL);
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    *pnDiscountPrice = htonl(pSAGIPUserBuyProduct->n_Discount_Price);
    return S_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

int AGIPUserBuyProduct::setUserID(uint32_t unUserID)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setRoleID(uint32_t unRoleID)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->un_Role_ID = htonl(unRoleID);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setRoleGender(int8_t cGender)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->c_Role_Gender = cGender;
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setRoleOccupation(int8_t cOccupation)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->c_Role_Occupation = cOccupation;
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setRoleLevel(uint16_t usLevel)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->us_Role_Level = htons(usLevel);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setIBCode(const char *strIBCode)
{
    ASSERT(strIBCode != NULL);
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    strncpy(pSAGIPUserBuyProduct->str_IB_Code, strIBCode, AGIP_IB_CODE_LEN);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setPackageFlag(int8_t cPackageFlag)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->c_Package_Flag = cPackageFlag;
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setAmount(int16_t sAmount)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->s_Amount = htons(sAmount);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setRatingID(uint32_t unRatingID)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->un_Rating_ID = htonl(unRatingID);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setServerID(uint32_t unServerID)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->un_Server_ID = htonl(unServerID);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setUserIP(uint32_t unUserIP)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->un_User_IP = htonl(unUserIP);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setSubjectID(int32_t nSubjectID)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->n_Subject_ID = htonl(nSubjectID);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setPrice(int32_t nPrice)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->n_Price = htonl(nPrice);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::setDiscountPrice(int32_t nDiscountPrice)
{
    SAGIPUserBuyProduct *pSAGIPUserBuyProduct = (SAGIPUserBuyProduct *)m_pucPDU;
    pSAGIPUserBuyProduct->n_Discount_Price = htonl(nDiscountPrice);
    return S_SUCCESS;
}

int AGIPUserBuyProduct::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t   unUserID = 0;
    uint32_t   unRoleID = 0;
    int8_t     cRoleGender = 0;
    int8_t     cRoleOccupation = 0;
    uint16_t   usRoleLevel = 0;
    char       strIBCode[AGIP_IB_CODE_LEN + 1];
    int8_t     cPackageFlag = 0;
    int16_t    sAmount      = 0;
    uint32_t   unRatingID   = 0;
    uint32_t   unServerID   = 0;
    int32_t    nSubjectID   = 0;
    int32_t    nPrice       = 0;
    int32_t    nDiscountPrice = 0;

    SysProtocol::showInfo();
    nRetCode = getUserID(&unUserID);
    nRetCode = getRoleID(&unRoleID);
    nRetCode = getRoleGender(&cRoleGender);
    nRetCode = getRoleOccupation(&cRoleOccupation);
    nRetCode = getRoleLevel(&usRoleLevel);
    nRetCode = getIBCode(strIBCode);
    strIBCode[AGIP_IB_CODE_LEN] = '\0';
    nRetCode = getPackageFlag(&cPackageFlag);
    nRetCode = getAmount(&sAmount);
    nRetCode = getRatingID(&unRatingID);
    nRetCode = getServerID(&unServerID);
    nRetCode = getSubjectID(&nSubjectID);
    nRetCode = getPrice(&nPrice);
    nRetCode = getDiscountPrice(&nDiscountPrice);
    printf("--------------------------------------------------------AGIPUserBuyProduct\n");
    printf("User_ID:\t%u\n", unUserID);
    printf("Role_ID:\t%u\n", unRoleID);
    printf("Role_Gender:\t%d\n", cRoleGender);
    printf("Role_Occupation:\t%d\n", cRoleOccupation);
    printf("Role_Level:\t%d\n", usRoleLevel);
    printf("IB_Code:\t%s\n", strIBCode);
    printf("Package_Flag:\t%d\n", cPackageFlag);
    printf("Amount:\t%d\n", sAmount);
    printf("Rating_ID:\t%u\n", unRatingID);
    printf("Server_ID:\t%u\n", unServerID);

    printf("Subject_ID:\t%d\n", nSubjectID);
    printf("Price:\t%d\n", nPrice);
    printf("Discount_Price:\t%d\n", nDiscountPrice);
    printf("--------------------------------------------------------AGIPUserBuyProduct\n");
    return S_SUCCESS;
}

///////////////////////////
// AGIPUserBuyProductRes
AGIPUserBuyProductRes::AGIPUserBuyProductRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_BUY_PRODUCT_RES, sizeof(SAGIPUserBuyProductRes) - sizeof(PURCHASE_ID) )
{

}

AGIPUserBuyProductRes::~AGIPUserBuyProductRes(void)
{

}

int AGIPUserBuyProductRes::initialize()
{
    int nRetCode = E_ERROR;

    m_usTotalLength = sizeof(SAGIPUserBuyProductRes) - sizeof(PURCHASE_ID);
    nRetCode = SysProtocol::initialize();
    ASSERT(nRetCode == S_SUCCESS);

    return S_SUCCESS;
}

int AGIPUserBuyProductRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserBuyProductRes *pSAGIPUserBuyProductRes = (SAGIPUserBuyProductRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserBuyProductRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserBuyProductRes::getCostMoney(int32_t *pnCostMoney)
{
    ASSERT(pnCostMoney != NULL);
    SAGIPUserBuyProductRes *pSAGIPUserBuyProductRes = (SAGIPUserBuyProductRes *)m_pucPDU;
    *pnCostMoney = htonl(pSAGIPUserBuyProductRes->n_Cost_Money);
    return S_SUCCESS;
}

int AGIPUserBuyProductRes::getRemainMoney(int32_t *pnRemainMoney)
{
    ASSERT(pnRemainMoney != NULL);
    SAGIPUserBuyProductRes *pSAGIPUserBuyProductRes = (SAGIPUserBuyProductRes *)m_pucPDU;
    *pnRemainMoney = htonl(pSAGIPUserBuyProductRes->n_Remain_Money);
    return S_SUCCESS;
}

int AGIPUserBuyProductRes::getIBCount(int32_t *pnIBCount)
{
    ASSERT(pnIBCount != NULL);
    SAGIPUserBuyProductRes *pSAGIPUserBuyProductRes = (SAGIPUserBuyProductRes *)m_pucPDU;
    *pnIBCount = htonl(pSAGIPUserBuyProductRes->n_IB_Count);
    return S_SUCCESS;
}

int AGIPUserBuyProductRes::getPurchaseID(int32_t nIndex, uint64_t *pullPurchaseID)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    PURCHASE_ID *pPurchaseIDBuffer = NULL;
    int32_t      nIBCount          = 0;

    uint32_t    *punPurchaseID     = NULL;

    ASSERT(pullPurchaseID != NULL);
    SAGIPUserBuyProductRes *pSAGIPUserBuyProductRes = (SAGIPUserBuyProductRes *)m_pucPDU;
    nIBCount = htonl(pSAGIPUserBuyProductRes->n_IB_Count);
    if (
        (nIndex < 0) ||
        (nIndex >= nIBCount)
        )
    {
        goto ExitError;
    }
    pPurchaseIDBuffer = (PURCHASE_ID *)&pSAGIPUserBuyProductRes->un_Purchase_ID_Higher;
    pPurchaseIDBuffer += nIndex;

    punPurchaseID = (uint32_t *)pullPurchaseID;

#if (defined(LINUX) || defined(WIN32))
    punPurchaseID[1] = htonl(pPurchaseIDBuffer->un_Purchase_ID_Higher);
    punPurchaseID[0] = htonl(pPurchaseIDBuffer->un_Purchase_ID_Lower);
#else
    punPurchaseID[0] = pPurchaseIDBuffer->un_Purchase_ID_Higher;
    punPurchaseID[1] = pPurchaseIDBuffer->un_Purchase_ID_Lower;
#endif

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPUserBuyProductRes::getPurchaseID(int32_t nIndex, PURCHASE_ID *pPurchaseID)
{
    int nResult  = E_PDU_INVALID_FIELD;
    int nRetCode = E_PDU_INVALID_FIELD;

    ASSERT(pPurchaseID != NULL);
    
    nRetCode = getPurchaseID( nIndex, &pPurchaseID->un_Purchase_ID_Higher, &pPurchaseID->un_Purchase_ID_Lower);
    if (nRetCode != S_SUCCESS)
    {
        nResult = nRetCode;
        goto ExitError;
    }

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPUserBuyProductRes::getPurchaseID(int32_t nIndex, uint32_t *punPurchaseIDHigher, uint32_t *punPurchaseIDLower)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;


    PURCHASE_ID *pPurchaseIDBuffer = NULL;
    int32_t      nIBCount          = 0;

    // uint32_t    *punPurchaseID     = NULL;

    ASSERT(punPurchaseIDHigher != NULL);
    ASSERT(punPurchaseIDLower  != NULL);
    SAGIPUserBuyProductRes *pSAGIPUserBuyProductRes = (SAGIPUserBuyProductRes *)m_pucPDU;
    nIBCount = htonl(pSAGIPUserBuyProductRes->n_IB_Count);
    if (
        (nIndex < 0) ||
        (nIndex >= nIBCount)
        )
    {
        goto ExitError;
    }
    pPurchaseIDBuffer = (PURCHASE_ID *)&pSAGIPUserBuyProductRes->un_Purchase_ID_Higher;
    pPurchaseIDBuffer += nIndex;

    *punPurchaseIDHigher = htonl(pPurchaseIDBuffer->un_Purchase_ID_Higher);
    *punPurchaseIDLower  = htonl(pPurchaseIDBuffer->un_Purchase_ID_Lower);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}


int AGIPUserBuyProductRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserBuyProductRes *pSAGIPUserBuyProductRes = (SAGIPUserBuyProductRes *)m_pucPDU;
    pSAGIPUserBuyProductRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPUserBuyProductRes::setCostMoney(int32_t nCostMoney)
{
    SAGIPUserBuyProductRes *pSAGIPUserBuyProductRes = (SAGIPUserBuyProductRes *)m_pucPDU;
    pSAGIPUserBuyProductRes->n_Cost_Money = htonl(nCostMoney);
    return S_SUCCESS;
}

int AGIPUserBuyProductRes::setRemainMoney(int32_t nRemainMoney)
{
    SAGIPUserBuyProductRes *pSAGIPUserBuyProductRes = (SAGIPUserBuyProductRes *)m_pucPDU;
    pSAGIPUserBuyProductRes->n_Remain_Money = htonl(nRemainMoney);
    return S_SUCCESS;
}

int AGIPUserBuyProductRes::setIBCount(int32_t nIBCount)
{
    SAGIPUserBuyProductRes *pSAGIPUserBuyProductRes = (SAGIPUserBuyProductRes *)m_pucPDU;
    pSAGIPUserBuyProductRes->n_IB_Count= htonl(nIBCount);
    return E_ERROR;
}

#include <iostream>
using namespace std;

int AGIPUserBuyProductRes::addPurchaseID(uint64_t ullPurchaseID)
{
    cout << "********************* " << ullPurchaseID << endl;
    int nResult  = E_PDU_INVALID_FIELD;

    PURCHASE_ID *pPurchaseIDBuffer = NULL;
    int32_t      nIBCount          = 0;

    uint32_t    *punPurchaseID     = NULL;
    SAGIPUserBuyProductRes *pSAGIPUserBuyProductRes = (SAGIPUserBuyProductRes *)m_pucPDU;
    nIBCount = htonl(pSAGIPUserBuyProductRes->n_IB_Count);

    pPurchaseIDBuffer = (PURCHASE_ID *)&pSAGIPUserBuyProductRes->un_Purchase_ID_Higher;
    pPurchaseIDBuffer += nIBCount;

    punPurchaseID = (uint32_t *)&ullPurchaseID;
#if (defined(LINUX) || defined(WIN32))
    pPurchaseIDBuffer->un_Purchase_ID_Higher = htonl(punPurchaseID[1]);
    pPurchaseIDBuffer->un_Purchase_ID_Lower  = htonl(punPurchaseID[0]);
#else
    pPurchaseIDBuffer->un_Purchase_ID_Lower  = punPurchaseID[0];
    pPurchaseIDBuffer->un_Purchase_ID_Higher = punPurchaseID[1];
#endif

    cout << "********************* " << pPurchaseIDBuffer->un_Purchase_ID_Higher << endl;
    cout << "********************* " << pPurchaseIDBuffer->un_Purchase_ID_Lower << endl;
    nIBCount++;
    pSAGIPUserBuyProductRes->n_IB_Count = htonl(nIBCount);
    m_usTotalLength += sizeof(PURCHASE_ID);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
    //ExitError:
    return nResult;
}

int AGIPUserBuyProductRes::addPurchaseID(PURCHASE_ID *pPurchaseID)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    uint64_t ullPurchaseID = 0;

    ASSERT(pPurchaseID != NULL);

    ullPurchaseID  = pPurchaseID->un_Purchase_ID_Higher;
    ullPurchaseID  = ullPurchaseID << 32;
    ullPurchaseID += pPurchaseID->un_Purchase_ID_Lower;

    nResult = addPurchaseID(ullPurchaseID);
    //ExitError:
    return nResult;
}

int AGIPUserBuyProductRes::addPurchaseID(uint32_t unPurchaseIDHigher, uint32_t unPurchaseIDLower)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    uint64_t ullPurchaseID = 0;

    ullPurchaseID  = unPurchaseIDHigher;
    ullPurchaseID  = ullPurchaseID << 32;
    ullPurchaseID += unPurchaseIDLower;

    nResult = addPurchaseID(ullPurchaseID);
    //ExitError:
    return nResult;
}


int AGIPUserBuyProductRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode = 0;
    int32_t   nCostMoney  = 0;
    int32_t   nRemainMoney = 0;
    int32_t   nIBCount     = 0;
    uint64_t  ullPurchaseID = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);
    nRetCode = getCostMoney(&nCostMoney);
    nRetCode = getRemainMoney(&nRemainMoney);
    nRetCode = getIBCount(&nIBCount);

    printf("--------------------------------------------------------AGIPUserBuyProductRes\n");
    printf("Result_Code:    %d\n", nResultCode);
    printf("Cost_Money:     %d\n", nCostMoney);
    printf("Remain_Money:   %d\n", nRemainMoney);
    printf("IB_Count:       %d\n", nIBCount);

    for (int i = 0; i < nIBCount; i++)
    {
        nRetCode = getPurchaseID(i, &ullPurchaseID);
        printf(
            "Index[%d]: Purchase_ID = %llu\n",
            i,
            ullPurchaseID
        );
    }
    printf("--------------------------------------------------------AGIPUserBuyProductRes\n");
    return S_SUCCESS;
}

