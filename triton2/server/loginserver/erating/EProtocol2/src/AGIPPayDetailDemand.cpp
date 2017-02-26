#include "AGIPPayDetailDemand.h"

AGIPPayDetailDemand::AGIPPayDetailDemand() :
    SysProtocol(AGIP_CURRENT_VERSION, CMD_PAY_DETAIL_DEMAND, sizeof(SAGIPPayDetailDemand))
{
    _demand_buff = (PSAGIPPayDetailDemand)m_pucPDU;
}

AGIPPayDetailDemand::~AGIPPayDetailDemand()
{
}

int32_t AGIPPayDetailDemand::getDetailType(void) const
{
    return (int32_t)ntohl(_demand_buff->n_Detail_Type);
}

void AGIPPayDetailDemand::setDetailType(int32_t DetailType)
{
    _demand_buff->n_Detail_Type = (int32_t)htonl(DetailType);
}

int16_t AGIPPayDetailDemand::getSubjectID(void) const
{
    return (int16_t)ntohs(_demand_buff->s_Subject_ID);
}

void AGIPPayDetailDemand::setSubjectID(int16_t SubjectID)
{
    _demand_buff->s_Subject_ID = (int16_t)htons(SubjectID);
}

int32_t AGIPPayDetailDemand::getDetailID(int index, uint64_t &DetailID, int32_t nFlag) const
{
    if (index >= getDetailCount() || index < 0)
    {
        return E_ERROR;
    }

    DetailID = ntohll(*(&(_demand_buff->unn_Detail_ID) + index), nFlag);
    return S_SUCCESS;
}

int32_t AGIPPayDetailDemand::setDetailID(int index, uint64_t  DetailID, int32_t nFlag)
{
    if (sizeof(SAGIPPayDetailDemand) + sizeof(DetailID) * index >= AGIP_MAX_BUFFER_SIZE - 44 /* sizeof(SAGIPDataPackage) */ )
        return E_ERROR;

    *(&(_demand_buff->unn_Detail_ID) + index) = htonll(DetailID, nFlag);

    m_usTotalLength += sizeof(DetailID);
    setTotalLength(m_usTotalLength);

    setDetailCount(getDetailCount() + 1);

    return S_SUCCESS;
}

int32_t AGIPPayDetailDemand::addDetailID(uint64_t DetailID, int32_t nFlag)
{
    return setDetailID(getDetailCount(), DetailID, nFlag);
}

uint16_t AGIPPayDetailDemand::getDetailCount(void) const
{
    return ntohs(_demand_buff->us_Detail_Count);
}

void AGIPPayDetailDemand::setDetailCount(uint16_t DetailCount)
{
    _demand_buff->us_Detail_Count = htons(DetailCount);
}

int AGIPPayDetailDemand::showInfo(void)
{
    SysProtocol::showInfo();

    printf("-----------------------------------------------------AGIPPayDetailDemand\n");
    printf("DetailType:             %d\n", getDetailType());
    printf("SubjectID:              %d\n", getSubjectID());
    printf("DetailCount:            %u\n", getDetailCount());

    uint16_t detail_count = getDetailCount();
    uint64_t detail_id;
    for (uint16_t index = 0; index < detail_count; ++index)
    {
        // index有效，此处不需要检测其返回值
        (void)getDetailID(index, detail_id);
        printf("DetailID[%03d]:         %llu\n", index, detail_id);
    }

    printf("-----------------------------------------------------AGIPPayDetailDemand\n");

    return S_SUCCESS;
}

AGIPPayDetailDemandRes::AGIPPayDetailDemandRes() :
    SysProtocol(AGIP_CURRENT_VERSION, CMD_PAY_DETAIL_DEMAND_RES, sizeof(SAGIPPayDetailDemandRes))
{
    _demand_res_buff = (PSAGIPPayDetailDemandRes)m_pucPDU;
}

AGIPPayDetailDemandRes::~AGIPPayDetailDemandRes()
{
}

int32_t AGIPPayDetailDemandRes::getResultCode(void) const
{
    return ntohl(_demand_res_buff->n_Result_Code);
}

void AGIPPayDetailDemandRes::setResultCode(int32_t ResultCode)
{
    _demand_res_buff->n_Result_Code = htonl(ResultCode);
}

int AGIPPayDetailDemandRes::showInfo(void)
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------AGIPPayDetailDemandRes\n");
    printf("ResultCode:             %d\n", getResultCode());
    printf("--------------------------------------------------AGIPPayDetailDemandRes\n");

    return S_SUCCESS;
}

