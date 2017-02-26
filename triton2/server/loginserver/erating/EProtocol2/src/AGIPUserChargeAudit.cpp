#include "AGIPUserChargeAudit.h"

// AGIPUserChargeAudit ------------------------------------------------------------

AGIPUserChargeAudit::AGIPUserChargeAudit(void)
    : SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_CHARGE_AUDIT,
            sizeof(SAGIPUserChargeAudit))
{
}

AGIPUserChargeAudit::~AGIPUserChargeAudit(void)
{
}

// getter ---------------------------------------------------------------

uint32_t AGIPUserChargeAudit::getRatingID(void)
{
    return ntohl(((SAGIPUserChargeAudit*)m_pucPDU)->un_Rating_ID);
}

uint32_t AGIPUserChargeAudit::getUserID(void)
{
    return ntohl(((SAGIPUserChargeAudit*)m_pucPDU)->un_User_ID);
}

int16_t AGIPUserChargeAudit::getSubjectID(void)
{
    return ntohs(((SAGIPUserChargeAudit*)m_pucPDU)->s_Subject_ID);
}

uint16_t AGIPUserChargeAudit::getAuditCount(void)
{
    return ntohs(((SAGIPUserChargeAudit*)m_pucPDU)->us_Audit_Count);
}

uint64_t AGIPUserChargeAudit::getBgnDetailID(int32_t n_Flag)
{
    return ntohll(*((uint64_t*)(&(((SAGIPUserChargeAudit*)m_pucPDU)->un_Bgn_Detail_ID_H))), n_Flag);
}

uint64_t AGIPUserChargeAudit::getEndDetailID(int32_t n_Flag)
{
    return ntohll(*((uint64_t*)(&(((SAGIPUserChargeAudit*)m_pucPDU)->un_End_Detail_ID_H))), n_Flag);
}

// setter ---------------------------------------------------------------

int AGIPUserChargeAudit::setRatingID(uint32_t un_Rating_ID)
{
    ((SAGIPUserChargeAudit*)m_pucPDU)->un_Rating_ID = htonl(un_Rating_ID);
    return S_SUCCESS;
}

int AGIPUserChargeAudit::setUserID(uint32_t un_User_ID)
{
    ((SAGIPUserChargeAudit*)m_pucPDU)->un_User_ID = htonl(un_User_ID);
    return S_SUCCESS;
}

int AGIPUserChargeAudit::setSubjectID(int16_t s_Subject_ID)
{
    ((SAGIPUserChargeAudit*)m_pucPDU)->s_Subject_ID = htons(s_Subject_ID);
    return S_SUCCESS;
}

int AGIPUserChargeAudit::setAuditCount(uint16_t us_Audit_Count)
{
    ((SAGIPUserChargeAudit*)m_pucPDU)->us_Audit_Count = htons(us_Audit_Count);
    return S_SUCCESS;
}

int AGIPUserChargeAudit::setBgnDetailID(uint64_t ull_Bgn_Detail_ID, int32_t n_Flag)
{
    *((uint64_t*)(&((SAGIPUserChargeAudit*)m_pucPDU)->un_Bgn_Detail_ID_H)) = htonll(ull_Bgn_Detail_ID, n_Flag);
    return S_SUCCESS;
}

int AGIPUserChargeAudit::setEndDetailID(uint64_t ull_End_Detail_ID, int32_t n_Flag)
{
    *((uint64_t*)(&((SAGIPUserChargeAudit*)m_pucPDU)->un_End_Detail_ID_H)) = htonll(ull_End_Detail_ID, n_Flag);
    return S_SUCCESS;
}

int AGIPUserChargeAudit::showInfo()
{
    printf("--------------------------------------------------------AGIPUserChargeAudit\n");
    SysProtocol::showInfo();
    printf("Passport_ID: %u\n", getUserID());
    printf("Rating_ID:   %u\n", getRatingID());
    printf("Subject_ID:  %d\n", getSubjectID());
    printf("Audit_Count: %u\n", getAuditCount());
    printf("Bgn_DetailID:%llu\n", getBgnDetailID());
    printf("End_DetailID:%llu\n", getEndDetailID());
    printf("--------------------------------------------------------AGIPUserChargeAudit\n");

    return S_SUCCESS;
}

// AGIPUserChargeAuditRes ---------------------------------------------------------

AGIPUserChargeAuditRes::AGIPUserChargeAuditRes(void)
    : SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_CHARGE_AUDIT_RES,
            sizeof(SAGIPUserChargeAuditRes)-sizeof(SChargeInfoShort))
{
    ((SAGIPUserChargeAuditRes*)m_pucPDU)->us_Audit_Count = 0;
}

AGIPUserChargeAuditRes::~AGIPUserChargeAuditRes(void)
{
}

// getter ---------------------------------------------------------------

int32_t AGIPUserChargeAuditRes::getResultCode(void)
{
    return ntohl(((SAGIPUserChargeAuditRes*)m_pucPDU)->n_Result_Code);
}

uint32_t AGIPUserChargeAuditRes::getRatingID(void)
{
    return ntohl(((SAGIPUserChargeAuditRes*)m_pucPDU)->un_Rating_ID);
}

uint32_t AGIPUserChargeAuditRes::getUserID(void)
{
    return ntohl(((SAGIPUserChargeAuditRes*)m_pucPDU)->un_User_ID);
}

int16_t AGIPUserChargeAuditRes::getSubjectID(void)
{
    return ntohs(((SAGIPUserChargeAuditRes*)m_pucPDU)->s_Subject_ID);
}

uint16_t AGIPUserChargeAuditRes::getAuditCount(void)
{
    return ntohs(((SAGIPUserChargeAuditRes*)m_pucPDU)->us_Audit_Count);
}

int AGIPUserChargeAuditRes::getChargeInfoShort(uint16_t index,
        uint64_t&   detail_id,
        int32_t&    n_Amount,
        int32_t     n_Flag)
{
    int32_t     result = E_PDU_INVALID_FIELD;
    uint16_t     count_charge_info = 0;

    SChargeInfoShort* charge_info_short = 0;
    SAGIPUserChargeAuditRes* audit_res = (SAGIPUserChargeAuditRes*)m_pucPDU;
    
    count_charge_info = getAuditCount();

    if (index >= count_charge_info)
    {
        result = E_PDU_INVALID_FIELD;
        goto ExitError;
    }

    charge_info_short = (SChargeInfoShort*)&(audit_res->un_Detail_ID_H);
    charge_info_short += index;

    detail_id = ntohll(*(uint64_t*)(&charge_info_short->un_Detail_ID_H), n_Flag);
    n_Amount    = htonl(charge_info_short->n_Amount);

    result = S_SUCCESS;
ExitError:

    return result;
}

int AGIPUserChargeAuditRes::getChargeInfoShort(uint16_t index,
        SChargeInfoShort& charge_info_short,
        int32_t n_Flag)
{
    return getChargeInfoShort(index,
            *(uint64_t*)(&charge_info_short.un_Detail_ID_H),
            charge_info_short.n_Amount,
            n_Flag);
}

// setter ---------------------------------------------------------------

int AGIPUserChargeAuditRes::setResultCode(int32_t n_Result_Code)
{
    ((SAGIPUserChargeAuditRes*)m_pucPDU)->n_Result_Code = htonl(n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserChargeAuditRes::setRatingID(uint32_t un_Rating_ID)
{
    ((SAGIPUserChargeAuditRes*)m_pucPDU)->un_Rating_ID = htonl(un_Rating_ID);
    return S_SUCCESS;
}

int AGIPUserChargeAuditRes::setUserID(uint32_t un_User_ID)
{
    ((SAGIPUserChargeAuditRes*)m_pucPDU)->un_User_ID = htonl(un_User_ID);
    return S_SUCCESS;
}

int AGIPUserChargeAuditRes::setSubjectID(int16_t s_Subject_ID)
{
    ((SAGIPUserChargeAuditRes*)m_pucPDU)->s_Subject_ID = htons(s_Subject_ID);
    return S_SUCCESS;
}

int AGIPUserChargeAuditRes::AddChargeInfoShort(const uint64_t& detail_id,
        const int32_t& n_Amount,
        int32_t        n_Flag)
{
    int32_t result = E_PDU_INVALID_FIELD;
    uint16_t count = 0;

    SChargeInfoShort*         charge_info = 0;
    SAGIPUserChargeAuditRes*      audit_res = (SAGIPUserChargeAuditRes*)m_pucPDU;
    
    count = getAuditCount();
    charge_info = (SChargeInfoShort*)&(audit_res->un_Detail_ID_H);
    charge_info += count;

    *(uint64_t*)(&charge_info->un_Detail_ID_H) = htonll(detail_id, n_Flag);
    charge_info->n_Amount    = htonl(n_Amount);

    audit_res->us_Audit_Count = htons(++count);
    m_usTotalLength += sizeof(SChargeInfoShort);
    setTotalLength(m_usTotalLength);

    result = S_SUCCESS;

    return result;
}

int AGIPUserChargeAuditRes::AddChargeInfoShort(const SChargeInfoShort& charge_info_short, int32_t n_Flag)
{
    return this->AddChargeInfoShort(*(uint64_t*)(&charge_info_short.un_Detail_ID_H), 
            charge_info_short.n_Amount,
            n_Flag);
}

int AGIPUserChargeAuditRes::showInfo()
{
    SChargeInfoShort charge_info = {0, 0, 0};

    printf("----------------------------------------------AGIPUserChargeAuditRes\n");
    SysProtocol::showInfo();
    printf("Result_Codoe: %d\n", getResultCode());
    printf("Passport_ID:  %u\n", getUserID());
    printf("Rating_ID:    %u\n", getRatingID());
    printf("Subject_ID:   %d\n", getSubjectID());
    printf("Audit_Count:  %u\n", getAuditCount());

    for (uint16_t i = 0; i < getAuditCount(); ++i)
    {
        getChargeInfoShort(i, charge_info);   
        printf("[%03u] : Detail_ID = %llu, Amount = %d\n",
                i, *(uint64_t*)(&charge_info.un_Detail_ID_H), charge_info.n_Amount);
    }

    printf("----------------------------------------------AGIPUserChargeAuditRes\n");

    return S_SUCCESS;
}

