#include "AGIPPromoterUserList.h"

// AGIPPromoterUserList
AGIPPromoterUserList::AGIPPromoterUserList(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_PROMOTER_USER_LIST, sizeof(SAGIPPromoterUserList))
{
    m_data = (SAGIPPromoterUserList*)m_pucPDU;
}

AGIPPromoterUserList::~AGIPPromoterUserList(void)
{
}

uint16_t AGIPPromoterUserList::getGameID(void) const
{
    return ntohs(m_data->us_Game_ID);
}

void AGIPPromoterUserList::setGameID(const uint16_t usGameID)
{
    m_data->us_Game_ID = htons(usGameID);
    return;
}

uint32_t AGIPPromoterUserList::getGatewayID(void) const
{
    return ntohl(m_data->un_Gateway_ID);
}

void AGIPPromoterUserList::setGatewayID(const uint32_t unGatewayID)
{
    m_data->un_Gateway_ID = htonl(unGatewayID);
    return;
}

uint32_t AGIPPromoterUserList::getUserID(void) const
{
    return ntohl(m_data->un_User_ID);
}

int32_t AGIPPromoterUserList::setUserID(const uint32_t unUserID)
{
    m_data->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int32_t AGIPPromoterUserList::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPPromoterUserList\n");
    printf("User_ID       :         %u\n", getUserID());
    printf("--------------------------------------------------------AGIPPromoterUserList\n");

    return S_SUCCESS;
}

// AGIPPromoterUserListRes
AGIPPromoterUserListRes::AGIPPromoterUserListRes(void) : 
    SysProtocol(
            AGIP_CURRENT_VERSION, 
            CMD_PROMOTER_USER_LIST_RES, 
            sizeof(SAGIPPromoterUserListRes) - sizeof(PROMOTER_USER_INFO)
            )
{
    m_data = (SAGIPPromoterUserListRes*)m_pucPDU;
}

AGIPPromoterUserListRes::~AGIPPromoterUserListRes(void)
{
}

int32_t AGIPPromoterUserListRes::initialize()
{
    int32_t nRetCode = E_ERROR;

    m_usTotalLength = sizeof(SAGIPPromoterUserListRes) - sizeof(PROMOTER_USER_INFO);
    nRetCode = SysProtocol::initialize();
    ASSERT(nRetCode == S_SUCCESS);

    return S_SUCCESS;
}

int32_t AGIPPromoterUserListRes::getResultCode(void) const
{
    return ntohl(m_data->n_Result_Code);
}

int32_t AGIPPromoterUserListRes::setResultCode(const int32_t nResultCode)
{
    m_data->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int32_t AGIPPromoterUserListRes::getAllUserCount(void) const
{
    return ntohl(m_data->n_All_User_Count);
}

int32_t AGIPPromoterUserListRes::setAllUserCount(const int32_t nAllUserCount)
{
    m_data->n_All_User_Count = htonl(nAllUserCount);
    return S_SUCCESS;
}

int32_t AGIPPromoterUserListRes::getUserCount(void) const
{
    return ntohl(m_data->n_User_Count);
}

int32_t AGIPPromoterUserListRes::getUserInfo(
    int32_t nIndex,
    PROMOTER_USER_INFO& userInfo
    ) const
{
    int32_t nResult  = E_PDU_INVALID_FIELD;

    int32_t nUserCount = 0;
    nUserCount = ntohl(m_data->n_User_Count);

    if ((nIndex < 0) || (nIndex >= nUserCount) || (nIndex >= AGIPPromoterUserListRes::MAX_USER_COUNT))
    {
        goto ExitError;
    }

    {
        PROMOTER_USER_INFO *pUserInfoBuffer = (PROMOTER_USER_INFO*)(&m_data->str_User_Name);
        pUserInfoBuffer += nIndex;

        memcpy(userInfo.str_User_Name, pUserInfoBuffer->str_User_Name, AGIP_USER_NAME_LEN);
    }

    nResult = S_SUCCESS;
ExitError:

    return nResult;
}

int32_t AGIPPromoterUserListRes::addUserInfo(const PROMOTER_USER_INFO& userInfo)
{
    int32_t nResult  = E_PDU_INVALID_FIELD;

    int32_t nUserCount = getUserCount();

    if ((nUserCount < 0) || (nUserCount >= AGIPPromoterUserListRes::MAX_USER_COUNT))
    {
        goto ExitError;
    }

    {
        PROMOTER_USER_INFO *pUserInfoBuffer = (PROMOTER_USER_INFO *)(&m_data->str_User_Name);
        pUserInfoBuffer += nUserCount;

        memcpy(pUserInfoBuffer->str_User_Name, userInfo.str_User_Name, AGIP_USER_NAME_LEN);
    }

    nUserCount++;
    m_data->n_User_Count = htonl(nUserCount);
    m_usTotalLength += sizeof(PROMOTER_USER_INFO);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
ExitError:

    return nResult;
}

int32_t AGIPPromoterUserListRes::showInfo()
{
    PROMOTER_USER_INFO userInfo;
    char strUserName[AGIP_USER_NAME_LEN + 1] = {0};

    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPPromoterUserListRes\n");
    printf("Result_Code   :         %d\n", getResultCode());
    printf("All_User_Count:         %d\n", getAllUserCount());
    printf("User_Count    :         %d\n", getUserCount());

    for (int32_t i = 0; i < getUserCount(); i++)
    {
        if (S_SUCCESS == getUserInfo(i, userInfo))
        {
            memcpy(strUserName, userInfo.str_User_Name, AGIP_USER_NAME_LEN);
            strUserName[AGIP_USER_NAME_LEN] = '\0';
            printf(
                "User[%02d]    :             "
                "                   User_Name=%s",
                i + 1,
                strUserName);
        }
        else
        {
            break;
        }
    }
    printf("--------------------------------------------------------AGIPPromoterUserListRes\n");

    return S_SUCCESS;
}

