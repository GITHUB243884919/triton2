#include "AGIPUserStateQuery.h"

AGIPUserStateQuery::AGIPUserStateQuery(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_STATE_QUERY, sizeof(SAGIPUserStateQuery))
{
}

AGIPUserStateQuery::~AGIPUserStateQuery(void)
{
}

uint32_t AGIPUserStateQuery::getUserID(void)
{
    return ntohl(((SAGIPUserStateQuery *)m_pucPDU)->un_User_ID);
}

const char* AGIPUserStateQuery::getUserName(char* szUserName, size_t unSize)
{
    if (NULL == szUserName)
    {
        return NULL;
    }

    if (unSize <= AGIP_USER_NAME_LEN)
    {
        return NULL;
    }

    memcpy(szUserName, ((SAGIPUserStateQuery *)m_pucPDU)->str_User_Name, AGIP_USER_NAME_LEN);
    szUserName[AGIP_USER_NAME_LEN] = 0;

    return szUserName;
}

// Setter
void AGIPUserStateQuery::setUserID(uint32_t unUserID)
{
    SAGIPUserStateQuery *pSAGIPUserStateQuery = (SAGIPUserStateQuery *)m_pucPDU;
    pSAGIPUserStateQuery->un_User_ID = htonl(unUserID);
}

void AGIPUserStateQuery::setUserName(const char *strUserName)
{
    if (NULL == strUserName)
    {
        return;
    }

    SAGIPUserStateQuery *pSAGIPUserStateQuery = (SAGIPUserStateQuery *)m_pucPDU;
    strncpy(pSAGIPUserStateQuery->str_User_Name, strUserName, AGIP_USER_NAME_LEN);
}

int32_t AGIPUserStateQuery::showInfo()
{
    char szUserName[AGIP_USER_NAME_LEN+1] = {0};
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserStateQuery\n");
    printf("Game_ID:    %u\n", getGameID());
    printf("User_ID:    %u\n", getUserID());
    printf("User_Name:  %s\n", getUserName(szUserName, sizeof(szUserName)));
    printf("--------------------------------------------------------AGIPUserStateQuery\n");
    return S_SUCCESS;
}
////////////////////
//AGIPUserStateQueryRes

AGIPUserStateQueryRes::AGIPUserStateQueryRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_STATE_QUERY_RES, sizeof(SAGIPUserStateQueryRes))
{

}

AGIPUserStateQueryRes::~AGIPUserStateQueryRes(void)
{

}

int32_t AGIPUserStateQueryRes::getResultCode(void)
{
    return (int32_t)ntohl(((SAGIPUserStateQueryRes *)m_pucPDU)->n_Result_Code);
}

uint32_t AGIPUserStateQueryRes::getUserState(void)
{
    return ntohl(((SAGIPUserStateQueryRes *)m_pucPDU)->un_User_State);
}

void AGIPUserStateQueryRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserStateQueryRes *pSAGIPUserStateQueryRes = (SAGIPUserStateQueryRes *)m_pucPDU;
    pSAGIPUserStateQueryRes->n_Result_Code = htonl(nResultCode);
}

void AGIPUserStateQueryRes::setUserState(uint32_t unUserState)
{
    SAGIPUserStateQueryRes *pSAGIPUserStateQueryRes = (SAGIPUserStateQueryRes *)m_pucPDU;
    pSAGIPUserStateQueryRes->un_User_State = htonl(unUserState);
}

int32_t AGIPUserStateQueryRes::showInfo()
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserStateQueryRes\n");
    printf("Result_Code: %d\n", getResultCode());
    printf("User_State:  %u\n", getUserState());
    printf("--------------------------------------------------------AGIPUserStateQueryRes\n");
    return S_SUCCESS;
}

