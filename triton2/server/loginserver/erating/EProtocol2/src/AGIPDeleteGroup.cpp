#include "AGIPDeleteGroup.h"

// AGIPDeleteGroup
AGIPDeleteGroup::AGIPDeleteGroup(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_DELETE_GROUP, sizeof(SAGIPDeleteGroup))
{
    m_data = (SAGIPDeleteGroup*)m_pucPDU;
}

AGIPDeleteGroup::~AGIPDeleteGroup(void)
{
}

void AGIPDeleteGroup::setGroupID(uint32_t unGroupID)
{
    m_data->un_Group_ID = htonl(unGroupID);
    return;
}

uint32_t AGIPDeleteGroup::getGroupID(void)
{
    return ntohl(m_data->un_Group_ID);
}

void AGIPDeleteGroup::setRoleID(uint32_t unRoleID)
{
    m_data->un_Role_ID = htonl(unRoleID);
    return;
}

uint32_t AGIPDeleteGroup::getRoleID(void)
{
    return ntohl(m_data->un_Role_ID);
}

int AGIPDeleteGroup::showInfo()
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPDeleteGroup\n");
    printf("Group_ID    : %u\n", getGroupID());
    printf("Role_ID     : %u\n", getRoleID());
    printf("--------------------------------------------------------AGIPDeleteGroup\n");

    return S_SUCCESS;
}

// AGIPDeleteGroupRes
AGIPDeleteGroupRes::AGIPDeleteGroupRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_DELETE_GROUP_RES, sizeof(SAGIPDeleteGroupRes))
{
    m_data = (SAGIPDeleteGroupRes*)m_pucPDU;
}

AGIPDeleteGroupRes::~AGIPDeleteGroupRes(void)
{
}

void AGIPDeleteGroupRes::setResultCode(int32_t nResultCode)
{
    m_data->n_Result_Code = htonl(nResultCode);
    return;
}

int32_t AGIPDeleteGroupRes::getResultCode(void)
{
    return ntohl(m_data->n_Result_Code);
}

int AGIPDeleteGroupRes::showInfo()
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPDeleteGroupRes\n");
    printf("Result_Code : %d\n", getResultCode());
    printf("--------------------------------------------------------AGIPDeleteGroupRes\n");

    return S_SUCCESS;
}

