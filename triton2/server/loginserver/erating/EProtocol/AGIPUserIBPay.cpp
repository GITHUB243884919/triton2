#include "AGIPUserIBPay.h"

AGIPUserIBPay::AGIPUserIBPay(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_IB_PAY, sizeof(SAGIPUserIBPay))
{
}

AGIPUserIBPay::~AGIPUserIBPay(void)
{
}

// getter ---------------------------------------------------------------

uint64_t AGIPUserIBPay::getDetailID(int32_t nFlag)const
{
    return ntohll(*((uint64_t*)(&(((PSAGIPUserIBPay)m_pucPDU)->un_Detail_ID_H))), nFlag);
}

uint32_t AGIPUserIBPay::getDetailIDH()const
{
    return ntohl( ((PSAGIPUserIBPay)m_pucPDU)->un_Detail_ID_H );
}

uint32_t AGIPUserIBPay::getDetailIDL()const
{
    return ntohl( ((PSAGIPUserIBPay)m_pucPDU)->un_Detail_ID_L );
}

uint32_t AGIPUserIBPay::getUserID()const
{
    return ntohl( ((PSAGIPUserIBPay)m_pucPDU)->un_User_ID );
}

uint32_t AGIPUserIBPay::getRoleID()const
{
    return ntohl( ((PSAGIPUserIBPay)m_pucPDU)->un_Role_ID );
}

uint8_t AGIPUserIBPay::getRoleGender()const
{
    return ((PSAGIPUserIBPay)m_pucPDU)->uc_Role_Gender;
}

uint8_t AGIPUserIBPay::getRoleOccupation()const
{
    return ((PSAGIPUserIBPay)m_pucPDU)->uc_Role_Occupation;
}

uint16_t AGIPUserIBPay::getRoleLevel()const
{
    return ntohs( ((PSAGIPUserIBPay)m_pucPDU)->us_Role_Level );
}

uint32_t AGIPUserIBPay::getRatingID()const
{
    return htonl( ((PSAGIPUserIBPay)m_pucPDU)->un_Rating_ID );
}

char* AGIPUserIBPay::getIBCode(char *szIBCode)const
{
    if (szIBCode != NULL)
        memcpy(szIBCode, ((PSAGIPUserIBPay)m_pucPDU)->sz_IB_Code, AGIP_IB_CODE_LEN);
    return szIBCode;
}

uint8_t AGIPUserIBPay::getPackageFlag()const
{
    return ((PSAGIPUserIBPay)m_pucPDU)->uc_Package_Flag;
}

uint16_t AGIPUserIBPay::getIBCount()const
{
    return ntohs( ((PSAGIPUserIBPay)m_pucPDU)->us_IB_Count );
}

int32_t AGIPUserIBPay::getPayTime()const
{
    return ntohl( ((PSAGIPUserIBPay)m_pucPDU)->n_Pay_Time );
}

uint32_t AGIPUserIBPay::getUserIP()const
{
    return ntohl( ((PSAGIPUserIBPay)m_pucPDU)->un_User_IP );
}

uint16_t AGIPUserIBPay::getSubjectID()const
{
    return ntohs( ((PSAGIPUserIBPay)m_pucPDU)->us_Subject_ID );
}

uint16_t AGIPUserIBPay::getAuditFlag()const
{
    return ntohs( ((PSAGIPUserIBPay)m_pucPDU)->us_Audit_Flag );
}

int32_t AGIPUserIBPay::getPrice()const
{
    return ntohl( ((PSAGIPUserIBPay)m_pucPDU)->n_Price );
}

int32_t AGIPUserIBPay::getDiscountPrice()const
{
    return ntohl( ((PSAGIPUserIBPay)m_pucPDU)->n_Discount_Price );
}

// setter ---------------------------------------------------------------

void AGIPUserIBPay::setDetailID(uint64_t ullDetailID, int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserIBPay)m_pucPDU)->un_Detail_ID_H))) = htonll(ullDetailID, nFlag);
}

void AGIPUserIBPay::setDetailIDH(uint32_t unDetailIDH)
{
    ((PSAGIPUserIBPay)m_pucPDU)->un_Detail_ID_H = htonl(unDetailIDH);
}

void AGIPUserIBPay::setDetailIDL(uint32_t unDetailIDL)
{
    ((PSAGIPUserIBPay)m_pucPDU)->un_Detail_ID_L = htonl(unDetailIDL);
}

void AGIPUserIBPay::setUserID(uint32_t unUserID)
{
    ((PSAGIPUserIBPay)m_pucPDU)->un_User_ID = htonl(unUserID);
}

void AGIPUserIBPay::setRoleID(uint32_t unRoleID)
{
    ((PSAGIPUserIBPay)m_pucPDU)->un_Role_ID = htonl(unRoleID);
}

void AGIPUserIBPay::setRoleGender(uint8_t ucGender)
{
    ((PSAGIPUserIBPay)m_pucPDU)->uc_Role_Gender = ucGender;
}

void AGIPUserIBPay::setRoleOccupation(uint8_t ucOccupation)
{
    ((PSAGIPUserIBPay)m_pucPDU)->uc_Role_Occupation = ucOccupation;
}

void AGIPUserIBPay::setRoleLevel(uint16_t usLevel)
{
    ((PSAGIPUserIBPay)m_pucPDU)->us_Role_Level = htons(usLevel);
}

void AGIPUserIBPay::setRatingID(uint32_t unRatingID)
{
    ((PSAGIPUserIBPay)m_pucPDU)->un_Rating_ID = htonl(unRatingID);
}

void AGIPUserIBPay::setIBCode(const char *szIBCode)
{
    if(szIBCode != NULL)
        strncpy( ((PSAGIPUserIBPay)m_pucPDU)->sz_IB_Code, szIBCode, AGIP_IB_CODE_LEN );
}

void AGIPUserIBPay::setPackageFlag(uint8_t ucPackageFlag)
{
    ((PSAGIPUserIBPay)m_pucPDU)->uc_Package_Flag = ucPackageFlag;
}

void AGIPUserIBPay::setIBCount(uint16_t usIBCount)
{
    ((PSAGIPUserIBPay)m_pucPDU)->us_IB_Count = htons(usIBCount);
}

void AGIPUserIBPay::setPayTime(int32_t nPayTime)
{
    ((PSAGIPUserIBPay)m_pucPDU)->n_Pay_Time = htonl(nPayTime);
}

void AGIPUserIBPay::setUserIP(uint32_t unUserIP)
{
    ((PSAGIPUserIBPay)m_pucPDU)->un_User_IP = htonl(unUserIP);
}

void AGIPUserIBPay::setSubjectID(uint16_t usSubjectID)
{
    ((PSAGIPUserIBPay)m_pucPDU)->us_Subject_ID = htons(usSubjectID);
}

void AGIPUserIBPay::setAuditFlag(uint16_t usAuditFlag)
{
    ((PSAGIPUserIBPay)m_pucPDU)->us_Audit_Flag = htons(usAuditFlag);
}

void AGIPUserIBPay::setPrice(int32_t nPrice)
{
    ((PSAGIPUserIBPay)m_pucPDU)->n_Price = htonl(nPrice);
}

void AGIPUserIBPay::setDiscountPrice(int32_t nDiscountPrice)
{
    ((PSAGIPUserIBPay)m_pucPDU)->n_Discount_Price = htonl(nDiscountPrice);
}

int AGIPUserIBPay::showInfo()
{
    SysProtocol::showInfo();

    char     szIBCode[AGIP_IB_CODE_LEN + 1];
    getIBCode(szIBCode);
    szIBCode[AGIP_IB_CODE_LEN] = '\0';

    printf("--------------------------------------------------------AGIPUserIBPay\n");
    printf("Detail_ID:          %llu\n", getDetailID()  );
    printf("User_ID:            %u\n",   getUserID()    );
    printf("Role_ID:            %u\n",   getRoleID()    );
    printf("Role_Gender:        %d\n",   getRoleGender());
    printf("Role_Occupation:    %d\n",   getRoleOccupation());
    printf("Role_Level:         %d\n",   getRoleLevel()	);
    printf("Rating_ID:          %u\n",   getRatingID() 	);
    printf("IB_Code:            %s\n",   szIBCode		);
    printf("Package_Flag:       %d\n",   getPackageFlag());
    printf("IB_Count:           %d\n",   getIBCount()    );
    printf("Pay_Time:           %d\n",   getPayTime()    );
    printf("User_IP:            %u\n",   getUserIP()     );
    printf("Subject_ID:         %d\n",   getSubjectID()  );
    printf("Audit_Flag:         %d\n",   getAuditFlag()  );
    printf("Price:              %d\n",   getPrice()      );
    printf("Discount_Price:     %d\n",   getDiscountPrice());
    printf("--------------------------------------------------------AGIPUserIBPay\n");

    return S_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// AGIPUserIBPayRes
AGIPUserIBPayRes::AGIPUserIBPayRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_IB_PAY_RES, sizeof(SAGIPUserIBPayRes))
{
}

AGIPUserIBPayRes::~AGIPUserIBPayRes(void)
{
}

int32_t  AGIPUserIBPayRes::getResultCode()const
{
    return ntohl( ((PSAGIPUserIBPayRes)m_pucPDU)->n_Result_Code );
}

uint64_t AGIPUserIBPayRes::getDetailID(int32_t nFlag)const
{
    return ntohll(*((uint64_t*)(&(((PSAGIPUserIBPayRes)m_pucPDU)->un_Detail_ID_H))), nFlag);
}

uint32_t AGIPUserIBPayRes::getDetailIDH()const
{
    return ntohl( ((PSAGIPUserIBPayRes)m_pucPDU)->un_Detail_ID_H );
}

uint32_t AGIPUserIBPayRes::getDetailIDL()const
{
    return ntohl( ((PSAGIPUserIBPayRes)m_pucPDU)->un_Detail_ID_L );
}

uint32_t AGIPUserIBPayRes::getUserID()const
{
    return ntohl( ((PSAGIPUserIBPayRes)m_pucPDU)->un_User_ID );
}

uint32_t AGIPUserIBPayRes::getRatingID()const
{
    return ntohl( ((PSAGIPUserIBPayRes)m_pucPDU)->un_Rating_ID );
}

uint16_t AGIPUserIBPayRes::getSubjectID()const
{
    return ntohs( ((PSAGIPUserIBPayRes)m_pucPDU)->us_Subject_ID );
}

uint16_t AGIPUserIBPayRes::getAuditFlag()const
{
    return ntohs( ((PSAGIPUserIBPayRes)m_pucPDU)->us_Audit_Flag );
}

int32_t  AGIPUserIBPayRes::getTotalPayAmount()const
{
    return ntohl( ((PSAGIPUserIBPayRes)m_pucPDU)->n_Total_Pay_Amount );
}

//////////////////////////////////////////////////////////////////////////

void AGIPUserIBPayRes::setResultCode(int32_t nResultCode)
{
    ((PSAGIPUserIBPayRes)m_pucPDU)->n_Result_Code = htonl(nResultCode);
}

void AGIPUserIBPayRes::setDetailID(uint64_t ullDetailID, int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserIBPayRes)m_pucPDU)->un_Detail_ID_H))) = htonll(ullDetailID, nFlag);
}

void AGIPUserIBPayRes::setDetailIDH(uint32_t unDetailIDH)
{
    ((PSAGIPUserIBPayRes)m_pucPDU)->un_Detail_ID_H = htonl(unDetailIDH);
}

void AGIPUserIBPayRes::setDetailIDL(uint32_t unDetailIDL)
{
    ((PSAGIPUserIBPayRes)m_pucPDU)->un_Detail_ID_L = htonl(unDetailIDL);
}

void AGIPUserIBPayRes::setUserID(uint32_t unUserID)
{
    ((PSAGIPUserIBPayRes)m_pucPDU)->un_User_ID = htonl(unUserID);
}

void AGIPUserIBPayRes::setRatingID(uint32_t unRatingID)
{
    ((PSAGIPUserIBPayRes)m_pucPDU)->un_Rating_ID = htonl(unRatingID);
}

void AGIPUserIBPayRes::setSubjectID(uint16_t usSubjectID)
{
    ((PSAGIPUserIBPayRes)m_pucPDU)->us_Subject_ID = htons(usSubjectID);
}

void AGIPUserIBPayRes::setAuditFlag(uint16_t usAuditFlag)
{
    ((PSAGIPUserIBPayRes)m_pucPDU)->us_Audit_Flag = htons(usAuditFlag);
}

void AGIPUserIBPayRes::setTotalPayAmount(int32_t nTotalPayAmount)
{
    ((PSAGIPUserIBPayRes)m_pucPDU)->n_Total_Pay_Amount = htonl(nTotalPayAmount);
}

int AGIPUserIBPayRes::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPUserIBPayRes\n");
    printf("Result_Code     :\t%d\n",   getResultCode());
    printf("Detail_ID       :\t%llu\n", getDetailID()  );
    printf("User_ID         :\t%u\n",   getUserID()    );
    printf("Rating_ID       :\t%u\n",   getRatingID()  );
    printf("Subject_ID      :\t%d\n",   getSubjectID() );
    printf("Audit_Flag      :\t%d\n",   getAuditFlag() );
    printf("Total_Pay_Amount:\t%d\n",   getTotalPayAmount());
    printf("--------------------------------------------------------AGIPUserIBPayRes\n");

    return S_SUCCESS;
}

