#include "AGIPUserCharge.h"

//////////////////////////////////////////////////////////////////////////
// AGIPUserCharge members

AGIPUserCharge::AGIPUserCharge(void) : 
    SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_CHARGE, sizeof(SAGIPUserCharge))
{
}

AGIPUserCharge::~AGIPUserCharge(void)
{
}

uint64_t AGIPUserCharge::getDetailID(int32_t nFlag) const
{
    return ntohll(*((uint64_t*)&((PSAGIPUserCharge)m_pucPDU)->un_Detail_ID_H), nFlag);
}

uint32_t AGIPUserCharge::getDetailIDH()const
{
    return ntohl( ((PSAGIPUserCharge)m_pucPDU)->un_Detail_ID_H );
}

uint32_t AGIPUserCharge::getDetailIDL()const
{
    return ntohl( ((PSAGIPUserCharge)m_pucPDU)->un_Detail_ID_L );
}

uint32_t AGIPUserCharge::getUserID()const
{
    return ntohl( ((PSAGIPUserCharge)m_pucPDU)->un_User_ID );
}

uint32_t AGIPUserCharge::getRatingID()const
{
    return ntohl( ((PSAGIPUserCharge)m_pucPDU)->un_Rating_ID );
}

uint16_t AGIPUserCharge::getSubjectID()const
{
    return ntohs( ((PSAGIPUserCharge)m_pucPDU)->us_Subject_ID );
}

int16_t  AGIPUserCharge::getAuditFlag()const
{
    return ntohs( ((PSAGIPUserCharge)m_pucPDU)->s_Audit_Flag );
}

int32_t  AGIPUserCharge::getAmount()const
{
    return ntohl( ((PSAGIPUserCharge)m_pucPDU)->n_Amount );
}

uint32_t AGIPUserCharge::getChargeTime()const
{
    return ntohl( ((PSAGIPUserCharge)m_pucPDU)->un_Charge_Time );
}

// setter ---------------------------------------------------------------
void AGIPUserCharge::setDetailID(uint64_t ullDetailID, int32_t nFlag)
{
    *(uint64_t*)(&((SAGIPUserCharge*)m_pucPDU)->un_Detail_ID_H) = htonll(ullDetailID, nFlag);
}

void AGIPUserCharge::setDetailIDH(uint32_t unDetailIDH)
{
    ((PSAGIPUserCharge)m_pucPDU)->un_Detail_ID_H = htonl(unDetailIDH);
}

void AGIPUserCharge::setDetailIDL(uint32_t unDetailIDL)
{
    ((PSAGIPUserCharge)m_pucPDU)->un_Detail_ID_L = htonl(unDetailIDL);
}

void AGIPUserCharge::setUserID(uint32_t unUserID)
{
    ((PSAGIPUserCharge)m_pucPDU)->un_User_ID = htonl(unUserID);
}

void AGIPUserCharge::setRatingID(uint32_t unRatingID)
{
    ((PSAGIPUserCharge)m_pucPDU)->un_Rating_ID = htonl(unRatingID);
}

void AGIPUserCharge::setSubjectID (uint16_t usSubjectID )
{
    ((PSAGIPUserCharge)m_pucPDU)->us_Subject_ID = htons(usSubjectID);
}

void AGIPUserCharge::setAuditFlag (int16_t sAuditFlag)
{
    ((PSAGIPUserCharge)m_pucPDU)->s_Audit_Flag = htons(sAuditFlag);
}

void AGIPUserCharge::setAmount(int32_t  nAmount)
{
    ((PSAGIPUserCharge)m_pucPDU)->n_Amount = htonl(nAmount);
}

void AGIPUserCharge::setChargeTime(uint32_t unChargeTime)
{
    ((PSAGIPUserCharge)m_pucPDU)->un_Charge_Time = htonl(unChargeTime);
}

int AGIPUserCharge::showInfo()
{
    SysProtocol::showInfo();

    printf("----------------------------------------------AGIPUserCharge\n");
    printf("Detail_ID:          %llu\n",getDetailID() );
    printf("Passport_ID:        %u\n",  getUserID()   );
    printf("Rating_ID:          %d\n",  getRatingID() );
    printf("Subject_ID:         %d\n",  getSubjectID());
    printf("Audit_Flag:         %u\n",  getAuditFlag());
    printf("Charge_Amount:      %d\n",  getAmount()   );
    printf("Charge_Time:        %s\n",  stringTime(getChargeTime()));
    printf("----------------------------------------------AGIPUserCharge\n");

    return S_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// AGIPUserChargeRes members:

AGIPUserChargeRes::AGIPUserChargeRes(void) : 
    SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_CHARGE_RES, sizeof(SAGIPUserChargeRes))
{
}

AGIPUserChargeRes::~AGIPUserChargeRes(void)
{
}

int32_t  AGIPUserChargeRes::getResultCode()const
{
    return ntohl( ((PSAGIPUserChargeRes)m_pucPDU)->n_Result_Code );
}

uint64_t AGIPUserChargeRes::getDetailID(int32_t nFlag)const
{
    return ntohll(*((uint64_t*)&((PSAGIPUserChargeRes)m_pucPDU)->un_Detail_ID_H), nFlag);
}

uint32_t AGIPUserChargeRes::getDetailIDH()const
{
    return ntohl( ((PSAGIPUserChargeRes)m_pucPDU)->un_Detail_ID_H );
}

uint32_t AGIPUserChargeRes::getDetailIDL()const
{
    return ntohl( ((PSAGIPUserChargeRes)m_pucPDU)->un_Detail_ID_L );
}

uint32_t AGIPUserChargeRes::getUserID()const
{
    return ntohl( ((PSAGIPUserChargeRes)m_pucPDU)->un_User_ID );
}

uint32_t AGIPUserChargeRes::getRatingID()const
{
    return ntohl( ((PSAGIPUserChargeRes)m_pucPDU)->un_Rating_ID );
}

uint16_t  AGIPUserChargeRes::getSubjectID() const
{
    return ntohs( ((PSAGIPUserChargeRes)m_pucPDU)->us_Subject_ID );
}

int16_t  AGIPUserChargeRes::getAuditFlag() const
{
    return ntohs( ((PSAGIPUserChargeRes)m_pucPDU)->s_Audit_Flag );
}

int32_t  AGIPUserChargeRes::getTotalAmount()const
{
    return ntohl( ((PSAGIPUserChargeRes)m_pucPDU)->n_Total_Amount );
}

void AGIPUserChargeRes::setResultCode (int32_t  nResultCode)
{
    ((PSAGIPUserChargeRes)m_pucPDU)->n_Result_Code = htonl(nResultCode);
}

void AGIPUserChargeRes::setDetailID(uint64_t ullDetailID, int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserChargeRes)m_pucPDU)->un_Detail_ID_H))) = htonll(ullDetailID, nFlag);
}

void AGIPUserChargeRes::setDetailIDH(uint32_t unDetailIDH)
{
    ((PSAGIPUserChargeRes)m_pucPDU)->un_Detail_ID_H = htonl(unDetailIDH);
}

void AGIPUserChargeRes::setDetailIDL(uint32_t unDetailIDL)
{
    ((PSAGIPUserChargeRes)m_pucPDU)->un_Detail_ID_L = htonl(unDetailIDL);
}

void AGIPUserChargeRes::setUserID(uint32_t unUserID)
{
    ((PSAGIPUserChargeRes)m_pucPDU)->un_User_ID = htonl(unUserID);
}

void AGIPUserChargeRes::setRatingID(uint32_t unRatingID )
{
    ((PSAGIPUserChargeRes)m_pucPDU)->un_Rating_ID = htonl(unRatingID);
}

void AGIPUserChargeRes::setSubjectID(uint16_t usSubjectID)
{
    ((PSAGIPUserChargeRes)m_pucPDU)->us_Subject_ID = htons(usSubjectID);
}

void AGIPUserChargeRes::setAuditFlag(int16_t  sAuditFlag )
{
    ((PSAGIPUserChargeRes)m_pucPDU)->s_Audit_Flag = htons(sAuditFlag);
}

void AGIPUserChargeRes::setTotalAmount(int32_t  nTotalAmount)
{
    ((PSAGIPUserChargeRes)m_pucPDU)->n_Total_Amount = htonl(nTotalAmount);
}

int AGIPUserChargeRes::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------AGIPUserChargeRes\n");
    printf("Result_Code:        %d\n",  getResultCode() );
    printf("Detail_ID:          %llu\n",getDetailID()   );
    printf("Passport_ID:        %d\n",  getUserID()     );
    printf("Rating_ID:          %d\n",  getRatingID()   );
    printf("Subject_ID:         %d\n",  getSubjectID()  );
    printf("Audit_Flag:         %d\n",  getAuditFlag()  );
    printf("Total_Charge_Amount:%d\n",  getTotalAmount());
    printf("--------------------------------------------------AGIPUserChargeRes\n");

    return S_SUCCESS;
}

