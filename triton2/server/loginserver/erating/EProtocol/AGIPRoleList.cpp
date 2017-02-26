#include "AGIPRoleList.h"

AGIPRoleList::AGIPRoleList(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_ROLE_LIST, sizeof(SAGIPRoleList))
{
}

AGIPRoleList::~AGIPRoleList(void)
{
}

int AGIPRoleList::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPRoleList *pSAGIPRoleList = (SAGIPRoleList *)m_pucPDU;
    *punUserID = htonl(pSAGIPRoleList->un_User_ID);
    return S_SUCCESS;
}

int AGIPRoleList::setUserID(uint32_t unUserID)
{
    SAGIPRoleList *pSAGIPRoleList = (SAGIPRoleList *)m_pucPDU;
    pSAGIPRoleList->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPRoleList::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t unUserID = 0;


    SysProtocol::showInfo();
    nRetCode = getUserID(&unUserID);

    printf("--------------------------------------------------------AGIPRoleList\n");
    printf("User_ID:\t%u\n", unUserID);

    printf("--------------------------------------------------------AGIPRoleList\n");
    return S_SUCCESS;
}
////////////////////
//AGIPRoleListRes

AGIPRoleListRes::AGIPRoleListRes(void) : 
    SysProtocol(
            AGIP_CURRENT_VERSION, 
            CMD_ROLE_LIST_RES, 
            sizeof(SAGIPRoleListRes) - sizeof(ROLE_INFO))
{

}

AGIPRoleListRes::~AGIPRoleListRes(void)
{

}

int AGIPRoleListRes::initialize()
{
    int nRetCode = E_ERROR;

    m_usTotalLength = sizeof(SAGIPRoleListRes) - sizeof(ROLE_INFO);
    nRetCode = SysProtocol::initialize();
    ASSERT(nRetCode == S_SUCCESS);

    return S_SUCCESS;
}

int AGIPRoleListRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPRoleListRes *pSAGIPRoleListRes = (SAGIPRoleListRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPRoleListRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPRoleListRes::getRoleCount(int32_t *pnRoleCount)
{
    ASSERT(pnRoleCount != NULL);
    SAGIPRoleListRes *pSAGIPRoleListRes = (SAGIPRoleListRes *)m_pucPDU;
    *pnRoleCount = htonl(pSAGIPRoleListRes->n_Role_Count);
    return S_SUCCESS;
}

int AGIPRoleListRes::getRoleInfo(
        int32_t         nIndex, 
        uint32_t*       punRoleID, 
        char*           strRoleName,
        int8_t*         pcRoleGender,
        int8_t*         pcRoleOccupation,
        uint32_t*       punRoleCreateTime)
{
    int nResult  = E_PDU_INVALID_FIELD;

    ROLE_INFO *pRoleInfoBuffer = NULL;
    int32_t nRoleCount = 0;

    ASSERT(punRoleID != NULL && 
            strRoleName != NULL && 
            pcRoleGender != NULL && 
            pcRoleOccupation != NULL && 
            punRoleCreateTime != NULL);
    SAGIPRoleListRes *pSAGIPRoleListRes = (SAGIPRoleListRes *)m_pucPDU;
    nRoleCount = htonl(pSAGIPRoleListRes->n_Role_Count);

    if ((nIndex < 0) || (nIndex >= nRoleCount))
    {
        goto ExitError;
    }

    pRoleInfoBuffer = (ROLE_INFO *)&pSAGIPRoleListRes->un_Role_ID;
    pRoleInfoBuffer += nIndex;

    *punRoleID = htonl(pRoleInfoBuffer->unRoleID);
    strncpy(strRoleName, pRoleInfoBuffer->strRoleName, AGIP_ROLE_NAME_LEN);
    *pcRoleGender = pRoleInfoBuffer->cRoleGender;
    *pcRoleOccupation = pRoleInfoBuffer->cRoleOccupation;
    *punRoleCreateTime = htonl(pRoleInfoBuffer->unRoleCreateTime);

    nResult = S_SUCCESS;
ExitError:

    return nResult;
}

/*
int AGIPRoleListRes::getRoleInfo(int32_t nIndex, ROLE_INFO *pRoleInfo)
{
    ASSERT(pRoleInfo != NULL);
    return this->getRoleInfo(nIndex, &pRoleInfo->unRoleID, pRoleInfo->strRoleName);
}
*/

int AGIPRoleListRes::getRoleInfo(int32_t nIndex, ROLE_INFO *pRoleInfo)
{
    ASSERT(pRoleInfo != NULL);
    return this->getRoleInfo(
            nIndex, 
            &pRoleInfo->unRoleID, 
            pRoleInfo->strRoleName,
            &pRoleInfo->cRoleGender,
            &pRoleInfo->cRoleOccupation,
            &pRoleInfo->unRoleCreateTime);
}

int AGIPRoleListRes::setResultCode(int32_t nResultCode)
{
    SAGIPRoleListRes *pSAGIPRoleListRes = (SAGIPRoleListRes *)m_pucPDU;
    pSAGIPRoleListRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPRoleListRes::setRoleCount(int32_t nRoleCount)
{
    ASSERT(false && "Role Count is calculated automatically. Pls use addRoleName() method!!");
    nRoleCount = 0;
    return E_ERROR;
}

/*
int AGIPRoleListRes::addRoleInfo(uint32_t unRoleID, const char *strRoleName)
{
    int nResult  = E_PDU_INVALID_FIELD;
    int nRetCode = E_PDU_INVALID_FIELD;

    ROLE_INFO *pRoleInfoBuffer = NULL;
    int32_t nRoleCount = 0;

    ASSERT(strRoleName != NULL);
    SAGIPRoleListRes *pSAGIPRoleListRes = (SAGIPRoleListRes *)m_pucPDU;
    nRoleCount = htonl(pSAGIPRoleListRes->n_Role_Count);

    pRoleInfoBuffer =  (ROLE_INFO *)&pSAGIPRoleListRes->un_Role_ID;
    pRoleInfoBuffer += nRoleCount;

    pRoleInfoBuffer->unRoleID = htonl(unRoleID);
    strncpy(pRoleInfoBuffer->strRoleName, strRoleName, AGIP_ROLE_NAME_LEN);

    nRoleCount++;
    pSAGIPRoleListRes->n_Role_Count = htonl(nRoleCount);
    m_usTotalLength += sizeof(ROLE_INFO);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
    //ExitError:
    return nResult;
}


int AGIPRoleListRes::addRoleInfo(const ROLE_INFO *pRoleInfo)
{
    ASSERT(pRoleInfo != NULL);
    return this->addRoleInfo(pRoleInfo->unRoleID, pRoleInfo->strRoleName);
}
*/

int AGIPRoleListRes::addRoleInfo(
        uint32_t    unRoleID, 
        const char* strRoleName,
        int8_t      cRoleGender,
        int8_t      cRoleOccupation,
        uint32_t    unRoleCreateTime)
{
    int nResult  = E_PDU_INVALID_FIELD;

    ROLE_INFO *pRoleInfoBuffer = NULL;
    int32_t nRoleCount = 0;

    ASSERT(strRoleName != NULL);
    SAGIPRoleListRes *pSAGIPRoleListRes = (SAGIPRoleListRes *)m_pucPDU;
    nRoleCount = htonl(pSAGIPRoleListRes->n_Role_Count);

    pRoleInfoBuffer =  (ROLE_INFO *)&pSAGIPRoleListRes->un_Role_ID;
    pRoleInfoBuffer += nRoleCount;

    pRoleInfoBuffer->unRoleID = htonl(unRoleID);
    strncpy(pRoleInfoBuffer->strRoleName, strRoleName, AGIP_ROLE_NAME_LEN);
    pRoleInfoBuffer->cRoleGender = cRoleGender;
    pRoleInfoBuffer->cRoleOccupation = cRoleOccupation;
    pRoleInfoBuffer->unRoleCreateTime = htonl(unRoleCreateTime);

    nRoleCount++;
    pSAGIPRoleListRes->n_Role_Count = htonl(nRoleCount);
    m_usTotalLength += sizeof(ROLE_INFO);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;

    return nResult;
}


int AGIPRoleListRes::addRoleInfo(const ROLE_INFO *pRoleInfo)
{
    ASSERT(pRoleInfo != NULL);
    return this->addRoleInfo(
            pRoleInfo->unRoleID, 
            pRoleInfo->strRoleName,
            pRoleInfo->cRoleGender,
            pRoleInfo->cRoleOccupation,
            pRoleInfo->unRoleCreateTime);
}

int AGIPRoleListRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t nResultCode = 0;
    int32_t nRoleCount  = 0;
    uint32_t unRoleID   = 0;
    char     strRoleName[AGIP_ROLE_NAME_LEN + 1];
    int8_t   cRoleGender = 0;
    int8_t   cRoleOccupation = 0;
    uint32_t unRoleCreateTime = 0;


    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);
    nRetCode = getRoleCount(&nRoleCount);
    printf("--------------------------------------------------------AGIPRoleListRes\n");
    printf("Result_Code:\t%d\n", nResultCode);
    printf("Role_Count:\t%d\n", nRoleCount);
    for (int i = 0; i < nRoleCount; i++)
    {
        nRetCode = getRoleInfo(i, &unRoleID, strRoleName, &cRoleGender, &cRoleOccupation, &unRoleCreateTime);
        strRoleName[AGIP_ROLE_NAME_LEN] = '\0';
        printf("[%d]Role_ID:\t%u,Role_Name:\t%s,Role_Gender:\t%d,Role_Occupation:\t%d,Role_Create_Time:\t%u\n", i, unRoleID, strRoleName, cRoleGender, cRoleOccupation, unRoleCreateTime);
    }

    printf("--------------------------------------------------------AGIPRoleListRes\n");
    return S_SUCCESS;
}

