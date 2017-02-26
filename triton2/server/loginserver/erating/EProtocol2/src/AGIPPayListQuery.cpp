#include "AGIPPayListQuery.h"

AGIPPayListQuery::AGIPPayListQuery() :
    SysProtocol(AGIP_CURRENT_VERSION, CMD_PAY_LIST_QUERY, sizeof(SAGIPPayListQuery))
{
    _query_buff = (PSAGIPPayListQuery)m_pucPDU;
}

AGIPPayListQuery::~AGIPPayListQuery()
{
}

int32_t AGIPPayListQuery::getDetailType(void) const
{
    return (int32_t)ntohl(_query_buff->n_Detail_Type);
}

void AGIPPayListQuery::setDetailType(int32_t DetailType)
{
    _query_buff->n_Detail_Type = (int32_t)htonl(DetailType);
}

int16_t AGIPPayListQuery::getSubjectID(void) const
{
    return (int16_t)ntohs(_query_buff->s_Subject_ID);
}

void AGIPPayListQuery::setSubjectID(int16_t SubjectID)
{
    _query_buff->s_Subject_ID = (int16_t)htons(SubjectID);
}

uint16_t AGIPPayListQuery::getDetailCount(void) const
{
    return ntohs(_query_buff->us_Detail_Count);
}

void AGIPPayListQuery::setDetailCount(uint16_t DetailCount)
{
    _query_buff->us_Detail_Count = htons(DetailCount);
}

uint64_t AGIPPayListQuery::getBeginDetailID(int32_t nFlag) const
{
    return ntohll(_query_buff->unn_Begin_Detail_ID, nFlag);
}

void AGIPPayListQuery::setBeginDetailID(uint64_t BeginDetailID, int32_t nFlag)
{
    _query_buff->unn_Begin_Detail_ID = htonll(BeginDetailID, nFlag);
}

int AGIPPayListQuery::showInfo(void)
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPPayListQuery\n");
    printf("DetailType:             %d\n", getDetailType());
    printf("SubjectID:              %d\n", getSubjectID());
    printf("DetailCount:            %u\n", getDetailCount());
    printf("BeginDetailID:          %llu\n", getBeginDetailID());
    printf("--------------------------------------------------------AGIPPayListQuery\n");

    return S_SUCCESS;
}

AGIPPayListQueryRes::AGIPPayListQueryRes() :
    SysProtocol(AGIP_CURRENT_VERSION, CMD_PAY_LIST_QUERY_RES, sizeof(SAGIPPayListQueryRes))
{
    _query_res_buff = (PSAGIPPayListQueryRes)m_pucPDU;
}

AGIPPayListQueryRes::~AGIPPayListQueryRes()
{
}

int32_t AGIPPayListQueryRes::getResultCode(void) const
{
    return ntohl(_query_res_buff->n_Result_Code);
}

void AGIPPayListQueryRes::setResultCode(int32_t ResultCode)
{
    _query_res_buff->n_Result_Code = htonl(ResultCode);
}

int16_t AGIPPayListQueryRes::getSubjectID(void) const
{
    return (int16_t)ntohs(_query_res_buff->s_Subject_ID);
}

void AGIPPayListQueryRes::setSubjectID(int16_t SubjectID)
{
    _query_res_buff->s_Subject_ID = (int16_t)htons(SubjectID);
}

int32_t AGIPPayListQueryRes::getTotalAmount(void) const
{
    return (int32_t)ntohl(_query_res_buff->n_Total_Amount);
}

void AGIPPayListQueryRes::setTotalAmount(int32_t TotalAmount)
{
    _query_res_buff->n_Total_Amount = (int32_t)htonl(TotalAmount);
}

uint16_t AGIPPayListQueryRes::getDetailCount(void) const
{
    return ntohs(_query_res_buff->us_Detail_Count);
}

void AGIPPayListQueryRes::setDetailCount(uint16_t DetailCount)
{
    _query_res_buff->us_Detail_Count = htons(DetailCount);
}

int32_t AGIPPayListQueryRes::getDetailID(int index, uint64_t &DetailID, int32_t nFlag) const
{
    if (index >= getDetailCount() || index < 0)
    {
        return E_ERROR;
    }

    DetailID = ntohll(*(&(_query_res_buff->unn_Detail_ID) + index), nFlag);
    return S_SUCCESS;
}

int32_t AGIPPayListQueryRes::setDetailID(int index, uint64_t  DetailID, int32_t nFlag)
{
    if (sizeof(SAGIPPayListQueryRes) + sizeof(DetailID) * index >= AGIP_MAX_BUFFER_SIZE - 44 /* sizeof(SAGIPDataPackage) */ )
        return E_ERROR;

    *(&(_query_res_buff->unn_Detail_ID) + index) = htonll(DetailID, nFlag);

    m_usTotalLength += sizeof(DetailID);
    setTotalLength(m_usTotalLength);

    setDetailCount(getDetailCount() + 1);

    return S_SUCCESS;
}

int32_t AGIPPayListQueryRes::addDetailID(uint64_t DetailID, int32_t nFlag)
{
    return setDetailID(getDetailCount(), DetailID, nFlag);
//    uint16_t detail_count = getDetailCount();
//
//    if (sizeof(SAGIPPayListQueryRes) + sizeof(DetailID) * detail_count >= AGIP_MAX_BUFFER_SIZE - 44)
//        return E_ERROR;
//
//    *(&(_query_res_buff->unn_Detail_ID) + detail_count) = htonll(DetailID, nFlag);
//
//    m_usTotalLength += sizeof(DetailID);
//    setTotalLength(m_usTotalLength);
//
//    setDetailCount(detail_count + 1);
//
//    return S_SUCCESS;
}

int AGIPPayListQueryRes::showInfo(void)
{
    SysProtocol::showInfo();

    printf("-----------------------------------------------------AGIPPayListQueryRes\n");
    printf("ResultCode:             %d\n", getResultCode());
    printf("SubjectID:              %d\n", getSubjectID());
    printf("DetailCount:            %u\n", getDetailCount());
    printf("TotalAmount:            %d\n", getTotalAmount());

    uint16_t detail_count = getDetailCount();
    uint64_t detail_id;
    for (uint16_t index = 0; index < detail_count; ++index)
    {
        // index有效，此处不需要检测其返回值
        (void)getDetailID(index, detail_id);
        printf("DetailID[%03d]:         %llu\n", index, detail_id);
    }

    printf("-----------------------------------------------------AGIPPayListQueryRes\n");

    return S_SUCCESS;
}


