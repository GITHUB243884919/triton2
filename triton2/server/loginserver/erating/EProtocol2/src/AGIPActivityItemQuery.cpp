#include "AGIPActivityItemQuery.h"

AGIPActivityItemQuery::AGIPActivityItemQuery(void) : 
    SysProtocol(AGIP_CURRENT_VERSION, CMD_ACTIVITY_ITEM_QUERY, sizeof(SAGIPActivityItemQuery))
{
}

AGIPActivityItemQuery::~AGIPActivityItemQuery(void)
{
}

uint32_t AGIPActivityItemQuery::getUserID()
{
    return htonl( ( (SAGIPActivityItemQuery*)m_pucPDU )->un_User_ID );
}

uint32_t AGIPActivityItemQuery::getRoleID()
{
    return htonl( ( (SAGIPActivityItemQuery*)m_pucPDU )->un_Role_ID );
}

uint32_t AGIPActivityItemQuery::getActivityID()
{
    return htonl( ( (SAGIPActivityItemQuery*)m_pucPDU )->un_Activity_ID );
}

uint16_t AGIPActivityItemQuery::getRoleLevel()
{
    return htons( ( (SAGIPActivityItemQuery*)m_pucPDU )->us_Role_Level );
}


//////////////////////////////////////////////////////////////////////////

void AGIPActivityItemQuery::setUserID(uint32_t unUserID)
{
    ( (SAGIPActivityItemQuery*)m_pucPDU )->un_User_ID = htonl(unUserID);
}

void AGIPActivityItemQuery::setRoleID(uint32_t unRoleID)
{
    ( (SAGIPActivityItemQuery*)m_pucPDU )->un_Role_ID = htonl(unRoleID);
}

void AGIPActivityItemQuery::setActivityID(uint32_t unActivityID)
{
    ( (SAGIPActivityItemQuery*)m_pucPDU )->un_Activity_ID = htonl(unActivityID);
}

void AGIPActivityItemQuery::setRoleLevel(uint16_t usRoleLevel)
{
    ( (SAGIPActivityItemQuery*)m_pucPDU )->us_Role_Level = htons(usRoleLevel);
}


int AGIPActivityItemQuery::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPActivityItemQuery\n");
    printf("User_ID:            %u\n", getUserID());
    printf("Role_ID:            %u\n", getRoleID());
    printf("Activity_ID:        %u\n", getActivityID());
    printf("Role_Level:         %u\n", getRoleLevel());
    printf("--------------------------------------------------------AGIPActivityItemQuery\n");

    return S_SUCCESS;
}

// --------------------------------------------------------------------------------------------

AGIPActivityItemQueryRes::AGIPActivityItemQueryRes(void) : 
    SysProtocol(AGIP_CURRENT_VERSION, CMD_ACTIVITY_ITEM_QUERY_RES,
        sizeof(SAGIPActivityItemQueryRes) - sizeof(ITEM_INFO))
{
}

AGIPActivityItemQueryRes::~AGIPActivityItemQueryRes(void)
{
}

int32_t AGIPActivityItemQueryRes::getResultCode()
{
    return htonl( ( (SAGIPActivityItemQueryRes*)m_pucPDU )->n_Result_Code );
}

int32_t AGIPActivityItemQueryRes::getItemCount()
{
    return htonl( ( (SAGIPActivityItemQueryRes*)m_pucPDU )->n_Item_Count );
}

int AGIPActivityItemQueryRes::getItemInfo(
            int32_t nIndex,
            uint32_t* punActivityID,
            char* strItemCode,
            int32_t* pnItemNum,
            uint32_t* punBeginTime,
            uint32_t* punEndTime)
{
    int nResult = E_PDU_INVALID_FIELD;

    ITEM_INFO *pItemInfoBuffer = NULL;

    ASSERT(punActivityID != NULL);
    ASSERT(strItemCode != NULL);
    ASSERT(pnItemNum != NULL);
    ASSERT(punBeginTime != NULL);
    ASSERT(punEndTime != NULL);

    SAGIPActivityItemQueryRes *pSAGIPActivityItemQueryRes = (SAGIPActivityItemQueryRes *)m_pucPDU;
    int32_t nItemCount = getItemCount();

    if ( (nIndex < 0) || (nIndex >= nItemCount) || (nIndex >= (MAX_BUFFER_SIZE-124)/(int32_t)sizeof(ITEM_INFO) ) )
    {
        nResult = E_PDU_INVALID_FIELD;
        goto ExitError;
    }

    pItemInfoBuffer = (ITEM_INFO *)&pSAGIPActivityItemQueryRes->un_Activity_ID;
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

void AGIPActivityItemQueryRes::setResultCode(int32_t nResultCode)
{
    ( (SAGIPActivityItemQueryRes*)m_pucPDU )->n_Result_Code = htonl(nResultCode);
}

int AGIPActivityItemQueryRes::addItemInfo(
            uint32_t unActivityID, 
            const char *strItemCode, 
            int32_t nItemNum,
            uint32_t unBeginTime,
            uint32_t unEndTime) 
{
    int nResult  = E_PDU_INVALID_FIELD;

    ITEM_INFO *pItemInfoBuffer = NULL;

    ASSERT(strItemCode != NULL);

    SAGIPActivityItemQueryRes *pSAGIPActivityItemQueryRes = (SAGIPActivityItemQueryRes *)m_pucPDU;
    int32_t nItemCount = getItemCount();

    if ( nItemCount >= (MAX_BUFFER_SIZE-124)/(int32_t)sizeof(ITEM_INFO) )
    {
        nResult  = E_PDU_INVALID_FIELD;
        goto ExitError;
    }

    pItemInfoBuffer =  (ITEM_INFO *)&pSAGIPActivityItemQueryRes->un_Activity_ID;
    pItemInfoBuffer += nItemCount;

    pItemInfoBuffer->un_Activity_ID = htonl(unActivityID);
    strncpy(pItemInfoBuffer->str_Item_Code, strItemCode, AGIP_ROLE_NAME_LEN);
    pItemInfoBuffer->n_Item_Num = htonl(nItemNum);
    pItemInfoBuffer->un_Begin_Time = htonl(unBeginTime);
    pItemInfoBuffer->un_End_Time = htonl(unEndTime);

    nItemCount++;
    pSAGIPActivityItemQueryRes->n_Item_Count = htonl(nItemCount);
    m_usTotalLength += sizeof(ITEM_INFO);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPActivityItemQueryRes::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t unActivityID = 0;
    char     strItemCode[AGIP_ITEM_CODE_LEN+1];
    int32_t  nItemNum    = 0;
    uint32_t unBeginTime = 0;
    uint32_t unEndTime = 0;

    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPActivityItemQueryRes\n");
    printf("Result_Code:        %d\n", getResultCode());
    printf("Item_Count:         %d\n", getItemCount());

    for (int i = 0; i < getItemCount(); i++)
    {
        nRetCode = getItemInfo(i, &unActivityID, strItemCode, &nItemNum, &unBeginTime, &unEndTime);
        strItemCode[sizeof(strItemCode) - 1] = '\0';
        printf("Item[%d]-ActvityID: %u, Item_Code: %s, Item_Num: %d, Begin_Time: %s, End_Time: %s.\n", 
                i, 
                unActivityID, 
                strItemCode, 
                nItemNum, 
                stringTime(unBeginTime), 
                stringTime(unEndTime) 
        );
    }

    printf("--------------------------------------------------------AGIPActivityItemQueryRes\n");

    return S_SUCCESS;
}

