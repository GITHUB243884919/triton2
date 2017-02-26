#include "AGIPUserCashExchangeAudit.h"
AGIPUserCashExchangeAudit::AGIPUserCashExchangeAudit(void)
 : SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_CASH_EXCHANGE_AUDIT, sizeof(SAGIPUserCashExchangeAudit))
{
}

AGIPUserCashExchangeAudit::~AGIPUserCashExchangeAudit(void)
{
}

uint32_t AGIPUserCashExchangeAudit::getGatewayID(void) const
{
     return ntohl( ((PSAGIPUserCashExchangeAudit)m_pucPDU)->un_gateway_id );
}

void AGIPUserCashExchangeAudit::setGatewayID(uint32_t nGatewayID)
{
    ((PSAGIPUserCashExchangeAudit)m_pucPDU)->un_gateway_id = htonl(nGatewayID);
}

int16_t AGIPUserCashExchangeAudit::getSubjectID(void) const
{
    return ntohs( ((PSAGIPUserCashExchangeAudit)m_pucPDU)->n_subject_id );
}

void AGIPUserCashExchangeAudit::setSubjectID(int16_t nsSubjectID)
{
    ((PSAGIPUserCashExchangeAudit)m_pucPDU)->n_subject_id = htons(nsSubjectID);
}

uint16_t AGIPUserCashExchangeAudit::getAuditCount(void) const
{
    return ntohs( ((PSAGIPUserCashExchangeAudit)m_pucPDU)->un_audit_count );
}

void AGIPUserCashExchangeAudit::setAuditCount(uint16_t usAuditCount)
{
    ((PSAGIPUserCashExchangeAudit)m_pucPDU)->un_audit_count = htons(usAuditCount);
}

uint64_t AGIPUserCashExchangeAudit::getBgnDetailID(int32_t nFlag) const
{
    return ntohll(*((uint64_t*)(&(((PSAGIPUserCashExchangeAudit)m_pucPDU)->un_bgn_detail_id_h))), nFlag);
}

void AGIPUserCashExchangeAudit::setBgnDetailID(uint64_t ullDetailID,int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserCashExchangeAudit)m_pucPDU)->un_bgn_detail_id_h))) = htonll(ullDetailID, nFlag);
}

uint64_t AGIPUserCashExchangeAudit::getEndDetailID(int32_t nFlag) const
{
    return ntohll(*((uint64_t*)(&(((PSAGIPUserCashExchangeAudit)m_pucPDU)->un_end_detail_id_h))), nFlag);
}

void AGIPUserCashExchangeAudit::setEndDetailID(uint64_t ullDetailID, int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserCashExchangeAudit)m_pucPDU)->un_end_detail_id_h))) = htonll(ullDetailID, nFlag);
}

int AGIPUserCashExchangeAudit::showInfo()
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserCashExchangeAudit\n");

    printf("gateway_id      %u\n",      getGatewayID());
    printf("subject_id      %d\n",      getSubjectID());
    printf("audit_count     %d\n",      getAuditCount());
    printf("bgn_detail_id   %llu\n",    getBgnDetailID());
    printf("end_detail_id   %llu\n",    getEndDetailID());

    printf("--------------------------------------------------------AGIPUserCashExchangeAudit\n");

    return S_SUCCESS;   
}


//AGIPUserCashExchangeAuditRes
AGIPUserCashExchangeAuditRes::AGIPUserCashExchangeAuditRes(void)
 : SysProtocol(AGIP_CURRENT_VERSION,  CMD_USER_CASH_EXCHANGE_AUDIT_RES, 
               sizeof(SAGIPUserCashExchangeAuditRes)-sizeof(SCashExchangeAuditInfo)
               )
{
}

AGIPUserCashExchangeAuditRes::~AGIPUserCashExchangeAuditRes(void)
{
}

int32_t AGIPUserCashExchangeAuditRes::getResultCode(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeAuditRes)m_pucPDU)->n_result_code);
}

void AGIPUserCashExchangeAuditRes::setResultCode(int32_t nResultCode)
{
    ((PSAGIPUserCashExchangeAuditRes)m_pucPDU)->n_result_code = htonl(nResultCode);
}

uint16_t AGIPUserCashExchangeAuditRes::getAuditCount(void) const
{
    return ntohs( ((PSAGIPUserCashExchangeAuditRes)m_pucPDU)->un_audit_count );
}

void AGIPUserCashExchangeAuditRes::setAuditCount(uint16_t usAuditCount)
{
    ((PSAGIPUserCashExchangeAuditRes)m_pucPDU)->un_audit_count = htons(usAuditCount);
}

uint64_t AGIPUserCashExchangeAuditRes::getDetailID(int32_t nIndex, int32_t nFlag)
{
    uint64_t ullDetailID  = 0;
    
    PSAGIPUserCashExchangeAuditRes pRes = (PSAGIPUserCashExchangeAuditRes)m_pucPDU;
    uint16_t usAuditCount = ntohs(pRes->un_audit_count);
    PSCashExchangeAuditInfo pInfo = (PSCashExchangeAuditInfo)&pRes->un_detail_id_h;

    if ((nIndex < 0) || (nIndex >= usAuditCount)) 
    {
        goto ExitError;
    }
    
    pInfo += nIndex;

    ullDetailID = ntohll(*((uint64_t*)(&(pInfo->un_detail_id_h))), nFlag);
ExitError:
    return ullDetailID;
}

int32_t AGIPUserCashExchangeAuditRes::addDetailID(uint64_t ullDetailID, int32_t nFlag)
{
    int32_t result = E_PDU_INVALID_FIELD;

    PSAGIPUserCashExchangeAuditRes pRes = (PSAGIPUserCashExchangeAuditRes)m_pucPDU;
    uint16_t usAuditCount = ntohs(pRes->un_audit_count);
    PSCashExchangeAuditInfo pInfo = (PSCashExchangeAuditInfo)&pRes->un_detail_id_h;
    uint64_t *p_ullDetailID = NULL;

    if (usAuditCount>=AGIP_MAX_CASH_EXCHANGE_AUDIT_COUNT)
    {
        goto ExitError;
    }

    pInfo += usAuditCount;
    p_ullDetailID = (uint64_t*)(&(pInfo->un_detail_id_h));
    *p_ullDetailID = htonll(ullDetailID, nFlag);

    usAuditCount++;
    pRes->un_audit_count = htons(usAuditCount);

    m_usTotalLength += sizeof(SCashExchangeAuditInfo);
    this->setTotalLength(m_usTotalLength);

    result = S_SUCCESS;
ExitError:
    return result;
}

int AGIPUserCashExchangeAuditRes::showInfo()
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserCashExchangeAuditRes\n");
    uint16_t usAuditCount = getAuditCount(); 

    printf("result_code     %u\n",      getResultCode());
    printf("audit_count     %d\n",      usAuditCount);
    
    for (int i=0; i<usAuditCount; i++)
    {
        printf("Index[%02d]: detail_id = %llu\n", i, getDetailID(i));
    }

    printf("--------------------------------------------------------AGIPUserCashExchangeAuditRes\n");

    return S_SUCCESS;   
}

