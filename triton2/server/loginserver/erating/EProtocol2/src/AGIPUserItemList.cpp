#include "AGIPUserItemList.h"

AGIPUserItemList::AGIPUserItemList(void) :
    SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_ITEM_LIST, sizeof(SAGIPUserItemList))
{
}

AGIPUserItemList::~AGIPUserItemList(void)
{
}


int AGIPUserItemList::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserItemList *pSAGIPUserItemList = (SAGIPUserItemList *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserItemList->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserItemList::getRoleID(uint32_t *punRoleID)
{
    ASSERT(punRoleID != NULL);
    SAGIPUserItemList *pSAGIPUserItemList = (SAGIPUserItemList *)m_pucPDU;
    *punRoleID = htonl(pSAGIPUserItemList->un_Role_ID);
    return S_SUCCESS;
}

int AGIPUserItemList::getActivityID(uint32_t *punActivityID)
{
    ASSERT(punActivityID != NULL);
    SAGIPUserItemList *pSAGIPUserItemList = (SAGIPUserItemList *)m_pucPDU;
    *punActivityID = htonl(pSAGIPUserItemList->un_Activity_ID);
    return S_SUCCESS;
}

int AGIPUserItemList::getItemGatewayID(uint32_t *punItemGatewayID)
{
    ASSERT(punItemGatewayID != NULL);
    SAGIPUserItemList *pSAGIPUserItemList = (SAGIPUserItemList *)m_pucPDU;
    *punItemGatewayID = htonl(pSAGIPUserItemList->un_Item_Gateway_ID);
    return S_SUCCESS;
}

int AGIPUserItemList::setUserID(uint32_t unUserID)
{
    SAGIPUserItemList *pSAGIPUserItemList = (SAGIPUserItemList *)m_pucPDU;
    pSAGIPUserItemList->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserItemList::setRoleID(uint32_t unRoleID)
{
    SAGIPUserItemList *pSAGIPUserItemList = (SAGIPUserItemList *)m_pucPDU;
    pSAGIPUserItemList->un_Role_ID = htonl(unRoleID);
    return S_SUCCESS;
}

int AGIPUserItemList::setActivityID(uint32_t unActivityID)
{
    SAGIPUserItemList *pSAGIPUserItemList = (SAGIPUserItemList *)m_pucPDU;
    pSAGIPUserItemList->un_Activity_ID = htonl(unActivityID);
    return S_SUCCESS;
}

int AGIPUserItemList::setItemGatewayID(uint32_t unItemGatewayID)
{
    SAGIPUserItemList *pSAGIPUserItemList = (SAGIPUserItemList *)m_pucPDU;
    pSAGIPUserItemList->un_Item_Gateway_ID = htonl(unItemGatewayID);
    return S_SUCCESS;
}

int AGIPUserItemList::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t  unItemGatewayID = 0;
    uint32_t  unUserID = 0;
    uint32_t  unRoleID = 0;
    uint32_t  unActivityID = 0;

    nRetCode = getItemGatewayID(&unItemGatewayID);
    nRetCode = getUserID(&unUserID);
    nRetCode = getRoleID(&unRoleID);
    nRetCode = getActivityID(&unActivityID);

    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserItemList\n");
    printf("Item_Gateway_ID:    %u\n", unItemGatewayID);
    printf("User_ID:            %u\n", unUserID);
    printf("Role_ID:            %u\n", unRoleID);
    printf("Activity_ID:        %u\n", unActivityID);
    printf("--------------------------------------------------------AGIPUserItemList\n");

    return S_SUCCESS;
}

// --------------------------------------------------------------------------------------------

AGIPUserItemListRes::AGIPUserItemListRes(void) :
    SysProtocol(
            AGIP_CURRENT_VERSION,
            CMD_USER_ITEM_LIST_RES,
            sizeof(SAGIPUserItemListRes) - sizeof(ITEM_INFO))
{
}

AGIPUserItemListRes::~AGIPUserItemListRes(void)
{

}

int AGIPUserItemListRes::initialize()
{
    int nRetCode = E_ERROR;

    m_usTotalLength = sizeof(SAGIPUserItemListRes) - sizeof(ITEM_INFO);
    nRetCode = SysProtocol::initialize();
    ASSERT(nRetCode == S_SUCCESS);

    return S_SUCCESS;
}

int AGIPUserItemListRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserItemListRes *pSAGIPUserItemListRes = (SAGIPUserItemListRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserItemListRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserItemListRes::getItemCount(int32_t *pnItemCount)
{
    ASSERT(pnItemCount != NULL);
    SAGIPUserItemListRes *pSAGIPUserItemListRes = (SAGIPUserItemListRes *)m_pucPDU;
    *pnItemCount = htonl(pSAGIPUserItemListRes->n_Item_Count);
    return S_SUCCESS;
}

int AGIPUserItemListRes::getItemInfo(
            int32_t nIndex,
            uint32_t* punActivityID,
            char*strItemCode,
            int32_t* pnItemNum,
            uint32_t* punBeginTime,
            uint32_t* punEndTime)
{
    int nResult = E_PDU_INVALID_FIELD;

    ITEM_INFO *pItemInfoBuffer = NULL;
    int32_t nItemCount = 0;

    ASSERT(punActivityID != NULL);
    ASSERT(strItemCode != NULL);
    ASSERT(pnItemNum != NULL);
    ASSERT(punBeginTime != NULL);
    ASSERT(punEndTime != NULL);

    SAGIPUserItemListRes *pSAGIPUserItemListRes = (SAGIPUserItemListRes *)m_pucPDU;
    getItemCount(&nItemCount);

    if ((nIndex < 0) || (nIndex >= nItemCount) || ((uint32_t)nIndex >= ((MAX_BUFFER_SIZE-124)/sizeof(ITEM_INFO)) ))
    {
        nResult = E_PDU_INVALID_FIELD;
        goto ExitError;
    }

    pItemInfoBuffer = (ITEM_INFO *)&pSAGIPUserItemListRes->un_Activity_ID;
    pItemInfoBuffer += nIndex;

    *punActivityID = ntohl(pItemInfoBuffer->un_Activity_ID);
    strncpy(strItemCode, pItemInfoBuffer->str_Item_Code, AGIP_ROLE_NAME_LEN);
    *pnItemNum = ntohl(pItemInfoBuffer->n_Item_Num);
    *punBeginTime = ntohl(pItemInfoBuffer->un_Begin_Time);
    *punEndTime = ntohl(pItemInfoBuffer->un_End_Time);

    nResult = S_SUCCESS;
ExitError:

    return nResult;
}

int AGIPUserItemListRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserItemListRes *pSAGIPUserItemListRes = (SAGIPUserItemListRes *)m_pucPDU;
    pSAGIPUserItemListRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPUserItemListRes::addItemInfo(
            uint32_t unActivityID,
            const char *strItemCode,
            int32_t nItemNum,
            uint32_t unBeginTime,
            uint32_t unEndTime)
{
    int nResult  = E_PDU_INVALID_FIELD;

    ITEM_INFO *pItemInfoBuffer = NULL;
    int32_t nItemCount = 0;

    ASSERT(strItemCode != NULL);

    SAGIPUserItemListRes *pSAGIPUserItemListRes = (SAGIPUserItemListRes *)m_pucPDU;
    getItemCount(&nItemCount);

    if ( (uint32_t)nItemCount >= ((MAX_BUFFER_SIZE-124)/sizeof(ITEM_INFO)) )
    {
        nResult  = E_PDU_INVALID_FIELD;
        goto ExitError;
    }

    pItemInfoBuffer =  (ITEM_INFO *)&pSAGIPUserItemListRes->un_Activity_ID;
    pItemInfoBuffer += nItemCount;

    pItemInfoBuffer->un_Activity_ID = htonl(unActivityID);
    strncpy(pItemInfoBuffer->str_Item_Code, strItemCode, AGIP_ROLE_NAME_LEN);
    pItemInfoBuffer->n_Item_Num = htonl(nItemNum);
    pItemInfoBuffer->un_Begin_Time = htonl(unBeginTime);
    pItemInfoBuffer->un_End_Time = htonl(unEndTime);

    nItemCount++;
    pSAGIPUserItemListRes->n_Item_Count = htonl(nItemCount);
    m_usTotalLength += sizeof(ITEM_INFO);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPUserItemListRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t  nResultCode = 0;
    int32_t  nItemCount  = 0;
    uint32_t unActivityID = 0;
    char     strItemCode[AGIP_ITEM_CODE_LEN+1];
    int32_t  nItemNum    = 0;
    uint32_t unBeginTime = 0;
    uint32_t unEndTime = 0;

    SysProtocol::showInfo();

    nRetCode = getResultCode(&nResultCode);
    nRetCode = getItemCount(&nItemCount);

    printf("--------------------------------------------------------AGIPUserItemListRes\n");
    printf("Result_Code:        %d\n", nResultCode);
    printf("Item_Count:         %d\n", nItemCount);

    char    time_buff[64];

    for (int i = 0; i < nItemCount; i++)
    {
        nRetCode = getItemInfo(i, &unActivityID, strItemCode, &nItemNum, &unBeginTime, &unEndTime);
        strItemCode[sizeof(strItemCode) - 1] = '\0';
        printf("Item[%d]-ActvityID: %u, Item_Code: %s, Item_Num: %d, Begin_Time: %s, End_Time: %s.\n",
                i,
                unActivityID,
                strItemCode,
                nItemNum,
                stringTime(unBeginTime),
                stringTime(unEndTime, time_buff)
        );
    }

    printf("--------------------------------------------------------AGIPUserItemListRes\n");

    return S_SUCCESS;
}

