#include "AGIPTransferGroup.h"

// AGIPTransferGroup
AGIPTransferGroup::AGIPTransferGroup(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_TRANSFER_GROUP, sizeof(SAGIPTransferGroup))
{
    m_data = (SAGIPTransferGroup*)m_pucPDU;
}

AGIPTransferGroup::~AGIPTransferGroup(void)
{
}

void AGIPTransferGroup::setGroupID(uint32_t unGroupID)
{
    m_data->un_Group_ID = htonl(unGroupID);
    return;
}

uint32_t AGIPTransferGroup::getGroupID(void)
{
    return ntohl(m_data->un_Group_ID);
}

void AGIPTransferGroup::setFromRoleID(uint32_t unRoleID)
{
    m_data->un_From_Role_ID = htonl(unRoleID);
    return;
}

uint32_t AGIPTransferGroup::getFromRoleID(void)
{
    return ntohl(m_data->un_From_Role_ID);
}

void AGIPTransferGroup::setToRoleID(uint32_t unRoleID)
{
    m_data->un_To_Role_ID = htonl(unRoleID);
    return;
}

uint32_t AGIPTransferGroup::getToRoleID(void)
{
    return ntohl(m_data->un_To_Role_ID);
}

int AGIPTransferGroup::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPTransferGroup\n");
    printf("Group_ID    : %u\n", getGroupID());
    printf("FromRole_ID : %u\n", getFromRoleID());
    printf("ToRole_ID   : %u\n", getToRoleID());
    printf("--------------------------------------------------------AGIPTransferGroup\n");

    return S_SUCCESS;
}

// AGIPTransferGroupRes
AGIPTransferGroupRes::AGIPTransferGroupRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_TRANSFER_GROUP_RES, sizeof(SAGIPTransferGroupRes))
{
    m_data = (SAGIPTransferGroupRes*)m_pucPDU;
}

AGIPTransferGroupRes::~AGIPTransferGroupRes(void)
{
}

void AGIPTransferGroupRes::setResultCode(int32_t nResultCode)
{
    m_data->n_Result_Code = htonl(nResultCode);
    return;
}

int32_t AGIPTransferGroupRes::getResultCode(void)
{
    return ntohl(m_data->n_Result_Code);
}

int AGIPTransferGroupRes::showInfo()
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPTransferGroupRes\n");
    printf("Result_Code : %d\n", getResultCode());
    printf("--------------------------------------------------------AGIPTransferGroupRes\n");

    return S_SUCCESS;
}

