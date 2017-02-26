#include "AGIPCreateGroup.h"

AGIPCreateGroup::AGIPCreateGroup(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_CREATE_GROUP, sizeof(SAGIPCreateGroup))
{
    m_data = (SAGIPCreateGroup*)m_pucPDU;
}

AGIPCreateGroup::~AGIPCreateGroup(void)
{
}

void AGIPCreateGroup::setRoleID(uint32_t unRoleID)
{
    m_data->un_Role_ID = htonl(unRoleID);
    return;
}

uint32_t AGIPCreateGroup::getRoleID(void)
{
    return ntohl(m_data->un_Role_ID);
}

void AGIPCreateGroup::setGroupName(const char* szRoleName)
{
    if (NULL != szRoleName)
    {
        strncpy(m_data->str_Group_Name, szRoleName, AGIP_GROUP_NAME_LEN);
    }

    return;
}

const char* AGIPCreateGroup::getGroupName(char* szRoleNameBuf, size_t unBufSize)
{
    if (NULL == szRoleNameBuf)
    {
        return NULL;
    }

    if (unBufSize <= AGIP_GROUP_NAME_LEN)
    {
        return NULL;
    }

    memcpy(szRoleNameBuf, m_data->str_Group_Name, AGIP_GROUP_NAME_LEN);
    szRoleNameBuf[AGIP_GROUP_NAME_LEN] = 0;

    return szRoleNameBuf;
}

void AGIPCreateGroup::setGroupType(uint8_t ucGroupType)
{
    m_data->uc_Group_Type = ucGroupType;
    return;
}

uint8_t AGIPCreateGroup::getGroupType(void)
{
    return m_data->uc_Group_Type;
}

int AGIPCreateGroup::showInfo()
{
    char szGroupName[AGIP_GROUP_NAME_LEN+1] = {0};

    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPCreateGroup\n");
    printf("Game_ID     : %u\n", getGameID());
    printf("Gaateway_ID : %u\n", getGatewayID());
    printf("Role_ID     : %u\n", getRoleID());
    printf("Group_Name  : %s\n", getGroupName(szGroupName, sizeof(szGroupName)));
    printf("Group_Type  : %u\n", getGroupType());
    printf("--------------------------------------------------------AGIPCreateGroup\n");

    return S_SUCCESS;
}

////////////////////
//AGIPCreateGroupRes
AGIPCreateGroupRes::AGIPCreateGroupRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_CREATE_GROUP_RES, sizeof(SAGIPCreateGroupRes))
{
    m_data = (SAGIPCreateGroupRes*)m_pucPDU;
}

AGIPCreateGroupRes::~AGIPCreateGroupRes(void)
{

}

void AGIPCreateGroupRes::setResultCode(int32_t nResultCode)
{
    m_data->n_Result_Code = htonl(nResultCode);
    return;
}

int32_t AGIPCreateGroupRes::getResultCode(void)
{
    return ntohl(m_data->n_Result_Code);
}

void AGIPCreateGroupRes::setGroupID(uint32_t unGroupID)
{
    m_data->un_Group_ID = htonl(unGroupID);
    return;
}

uint32_t AGIPCreateGroupRes::getGroupID(void)
{
    return ntohl(m_data->un_Group_ID);
}

int AGIPCreateGroupRes::showInfo()
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPCreateGroupRes\n");
    printf("Result_Code : %d\n", getResultCode());
    printf("Group_ID    : %u\n", getGroupID());
    printf("--------------------------------------------------------AGIPCreateGroupRes\n");

    return S_SUCCESS;
}

