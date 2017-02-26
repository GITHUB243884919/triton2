#include "AGIPUserIBPayAudit.h"

// AGIPUserIBPayAudit ---------------------------------------------------
AGIPUserIBPayAudit::AGIPUserIBPayAudit(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_IB_PAY_AUDIT, sizeof(SAGIPUserIBPayAudit))
{
}

AGIPUserIBPayAudit::~AGIPUserIBPayAudit(void)
{
}

// getter ---------------------------------------------------------------

uint32_t AGIPUserIBPayAudit::getUserID()const
{
    return ntohl( ((PSAGIPUserIBPayAudit)m_pucPDU)->un_User_ID);
}

uint32_t AGIPUserIBPayAudit::getRatingID()const
{
    return ntohl( ((PSAGIPUserIBPayAudit)m_pucPDU)->un_Rating_ID );
}

uint16_t AGIPUserIBPayAudit::getSubjectID()const
{
    return ntohs( ((PSAGIPUserIBPayAudit)m_pucPDU)->us_Subject_ID );
}

uint16_t AGIPUserIBPayAudit::getAuditCount()const
{
    return ntohs( ((PSAGIPUserIBPayAudit)m_pucPDU)->us_Audit_Count );
}

uint64_t AGIPUserIBPayAudit::getBgnDetailID(int32_t nFlag)const
{
    return ntohll(*((uint64_t*)(&(((PSAGIPUserIBPayAudit)m_pucPDU)->un_Bgn_Detail_ID_H))), nFlag);
}

uint32_t AGIPUserIBPayAudit::getBgnDetailIDH()const
{
    return ntohl( ((PSAGIPUserIBPayAudit)m_pucPDU)->un_Bgn_Detail_ID_H );
}

uint32_t AGIPUserIBPayAudit::getBgnDetailIDL()const
{
    return ntohl( ((PSAGIPUserIBPayAudit)m_pucPDU)->un_Bgn_Detail_ID_L );
}

uint64_t AGIPUserIBPayAudit::getEndDetailID(int32_t nFlag)const
{
    return ntohll(*((uint64_t*)(&(((PSAGIPUserIBPayAudit)m_pucPDU)->un_End_Detail_ID_H))), nFlag);
}

uint32_t AGIPUserIBPayAudit::getEndDetailIDH()const
{
    return ntohl( ((PSAGIPUserIBPayAudit)m_pucPDU)->un_End_Detail_ID_H );
}

uint32_t AGIPUserIBPayAudit::getEndDetailIDL()const
{
    return ntohl( ((PSAGIPUserIBPayAudit)m_pucPDU)->un_End_Detail_ID_L );
}

// setter ---------------------------------------------------------------

void AGIPUserIBPayAudit::setUserID(uint32_t unUserID)
{
    ((PSAGIPUserIBPayAudit)m_pucPDU)->un_User_ID = htonl(unUserID);
}

void AGIPUserIBPayAudit::setRatingID(uint32_t unRatingID)
{
    ((PSAGIPUserIBPayAudit)m_pucPDU)->un_Rating_ID = htonl(unRatingID);
}

void AGIPUserIBPayAudit::setSubjectID(uint16_t usSubjectID)
{
    ((PSAGIPUserIBPayAudit)m_pucPDU)->us_Subject_ID = htons(usSubjectID);
}

void AGIPUserIBPayAudit::setAuditCount(uint16_t usAuditCount)
{
    ((PSAGIPUserIBPayAudit)m_pucPDU)->us_Audit_Count = htons(usAuditCount);
}

void AGIPUserIBPayAudit::setBgnDetailID(uint64_t ullBgnDetailID, int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserIBPayAudit)m_pucPDU)->un_Bgn_Detail_ID_H))) = htonll(ullBgnDetailID, nFlag);
}

void AGIPUserIBPayAudit::setBgnDetailIDH(uint32_t unBgnDetailIDH)
{
    ((PSAGIPUserIBPayAudit)m_pucPDU)->un_Bgn_Detail_ID_H = htonl(unBgnDetailIDH);
}

void AGIPUserIBPayAudit::setBgnDetailIDL(uint32_t unBgnDetailIDL)
{
    ((PSAGIPUserIBPayAudit)m_pucPDU)->un_Bgn_Detail_ID_L = htonl(unBgnDetailIDL);
}

void AGIPUserIBPayAudit::setEndDetailID(uint64_t ullEndDetailID, int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserIBPayAudit)m_pucPDU)->un_End_Detail_ID_H))) = htonll(ullEndDetailID, nFlag);
}

void AGIPUserIBPayAudit::setEndDetailIDH(uint32_t unEndDetailIDH)
{
    ((PSAGIPUserIBPayAudit)m_pucPDU)->un_End_Detail_ID_H = htonl(unEndDetailIDH);
}

void AGIPUserIBPayAudit::setEndDetailIDL(uint32_t unEndDetailIDL)
{
    ((PSAGIPUserIBPayAudit)m_pucPDU)->un_End_Detail_ID_L = htonl(unEndDetailIDL);
}

int AGIPUserIBPayAudit::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPUserIBPayAudit\n");
    printf("User_ID:             %u\n",   getUserID()       );
    printf("Rating_ID:           %u\n",   getRatingID()     );
    printf("Subject_ID:          %d\n",   getSubjectID()    );
    printf("Audit_Count:         %u\n",   getAuditCount()   );
    printf("Bgn_Detail_ID:       %llu\n", getBgnDetailID());
    printf("End_Detail_ID:       %llu\n", getEndDetailID()  );
    printf("--------------------------------------------------------AGIPUserIBPayAudit\n");

    return S_SUCCESS;
}

// AGIPUserIBPayAuditRes ------------------------------------------------
AGIPUserIBPayAuditRes::AGIPUserIBPayAuditRes(void) : 
    SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_IB_PAY_AUDIT_RES,
        sizeof(SAGIPUserIBPayAuditRes)-sizeof(SIBPayAuditInfo))
{
}

AGIPUserIBPayAuditRes::~AGIPUserIBPayAuditRes(void)
{
}

// getter ---------------------------------------------------------------

int32_t  AGIPUserIBPayAuditRes::getResultCode()const
{
    return ntohl( ((PSAGIPUserIBPayAuditRes)m_pucPDU)->n_Result_Code );
}

uint32_t AGIPUserIBPayAuditRes::getUserID()const
{
    return ntohl( ((PSAGIPUserIBPayAuditRes)m_pucPDU)->un_User_ID );
}

uint32_t AGIPUserIBPayAuditRes::getRatingID()const
{
    return ntohl( ((PSAGIPUserIBPayAuditRes)m_pucPDU)->un_Rating_ID );
}

uint16_t AGIPUserIBPayAuditRes::getSubjectID()const
{
    return ntohs( ((PSAGIPUserIBPayAuditRes)m_pucPDU)->us_Subject_ID );
}

uint16_t AGIPUserIBPayAuditRes::getAuditCount()const
{
    return ntohs( ((PSAGIPUserIBPayAuditRes)m_pucPDU)->us_Audit_Count );
}

int AGIPUserIBPayAuditRes::getIBPayAuditInfo(int32_t nIndex, uint64_t* pullDetailID, int32_t* pnPayAmount, int32_t nFlag)const
{
    int nResult  = E_PDU_INVALID_FIELD;

    ASSERT(pullDetailID != NULL && pnPayAmount != NULL);

    PSAGIPUserIBPayAuditRes pUserIBPayAuditRes = (PSAGIPUserIBPayAuditRes)m_pucPDU;
    uint64_t* pullPDUDetailID = NULL;
    uint16_t usAuditCount = ntohs(pUserIBPayAuditRes->us_Audit_Count);
    PSIBPayAuditInfo pIBPayAuditInfoBuffer = (PSIBPayAuditInfo)&pUserIBPayAuditRes->un_Detail_ID_H;

    if ( (nIndex < 0) || (nIndex >= usAuditCount) )
    {
        goto ExitError;
    }
    pIBPayAuditInfoBuffer += nIndex;
    pullPDUDetailID = (uint64_t*)(&(pIBPayAuditInfoBuffer->un_Detail_ID_H));

    *pullDetailID  = ntohll(*pullPDUDetailID, nFlag);
    *pnPayAmount   = ntohl (pIBPayAuditInfoBuffer->n_Pay_Amount);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPUserIBPayAuditRes::getIBPayAuditInfo(int32_t nIndex, PSIBPayAuditInfo pIBPayAuditInfo, int32_t nFlag)const
{
    return getIBPayAuditInfo(nIndex, (uint64_t*)&pIBPayAuditInfo->un_Detail_ID_H, &pIBPayAuditInfo->n_Pay_Amount, nFlag);
}

// setter ---------------------------------------------------------------

void AGIPUserIBPayAuditRes::setResultCode(int32_t nResultCode)
{
    ((PSAGIPUserIBPayAuditRes)m_pucPDU)->n_Result_Code = htonl(nResultCode);
}

void AGIPUserIBPayAuditRes::setUserID(uint32_t unUserID)
{
    ((PSAGIPUserIBPayAuditRes)m_pucPDU)->un_User_ID = htonl(unUserID);
}

void AGIPUserIBPayAuditRes::setRatingID(uint32_t unRatingID)
{
    ((PSAGIPUserIBPayAuditRes)m_pucPDU)->un_Rating_ID = htonl(unRatingID);
}

void AGIPUserIBPayAuditRes::setSubjectID(uint16_t usSubjectID)
{
    ((PSAGIPUserIBPayAuditRes)m_pucPDU)->us_Subject_ID = htons(usSubjectID);
}

void AGIPUserIBPayAuditRes::setAuditCount(uint16_t usAuditCount)
{
    ((PSAGIPUserIBPayAuditRes)m_pucPDU)->us_Audit_Count = htons(usAuditCount);
}

int AGIPUserIBPayAuditRes::addIBPayAuditInfo(uint64_t ullDetailID, int32_t nPayAmount, int32_t nFlag)
{
    int nResult  = E_PDU_INVALID_FIELD;

    uint64_t* pullDetailID = NULL;
    PSAGIPUserIBPayAuditRes pUserIBPayAuditRes = (PSAGIPUserIBPayAuditRes)m_pucPDU;
    uint16_t usAuditCount = ntohs(pUserIBPayAuditRes->us_Audit_Count);
    PSIBPayAuditInfo pIBPayAuditInfoBuffer = (PSIBPayAuditInfo)&pUserIBPayAuditRes->un_Detail_ID_H;

    if (usAuditCount >= AGIP_MAX_IB_AUDIT_CNT)
        goto ExitError;

    pIBPayAuditInfoBuffer += usAuditCount;
    pullDetailID = (uint64_t*)(&((pIBPayAuditInfoBuffer->un_Detail_ID_H)));

    *pullDetailID = htonll(ullDetailID, nFlag);
    pIBPayAuditInfoBuffer->n_Pay_Amount  = htonl(nPayAmount);
 
    usAuditCount ++;
    pUserIBPayAuditRes->us_Audit_Count = htons(usAuditCount);
    m_usTotalLength += sizeof(SIBPayAuditInfo);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPUserIBPayAuditRes::addIBPayAuditInfo(const PSIBPayAuditInfo pIBPayAuditInfo, int32_t nFlag)
{
    uint64_t* pullDetailID = (uint64_t*)(&(pIBPayAuditInfo->un_Detail_ID_H));
    return addIBPayAuditInfo(*pullDetailID, pIBPayAuditInfo->n_Pay_Amount, nFlag);
}

int AGIPUserIBPayAuditRes::showInfo()
{
    int nRetCode = E_ERROR;

    SIBPayAuditInfo IBPayAuditInfo = {0, 0, 0};
    uint16_t usAuditCount = getAuditCount() ;
    uint64_t* pullDetailID = (uint64_t*)(&(IBPayAuditInfo.un_Detail_ID_H));

    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPUserIBPayAuditRes\n");
    printf("Result_Code:    %d\n", getResultCode());
    printf("User_ID:        %u\n", getUserID()    );
    printf("Rating_ID:      %u\n", getRatingID()  );
    printf("Subject_ID:     %d\n", getSubjectID() );
    printf("Audit_Count:    %d\n", usAuditCount   );
    for (int i = 0; i < usAuditCount; i++)
    {
        nRetCode = getIBPayAuditInfo(i, &IBPayAuditInfo);
        printf(
            "Index[%02d]: Detail_ID = %llu, Pay_Amount = %d.\n",
            i,
            *pullDetailID,
            IBPayAuditInfo.n_Pay_Amount
        );
    }
    printf("--------------------------------------------------------AGIPUserIBPayAuditRes\n");
    return S_SUCCESS;
}

