#include "AGIPJointAuthen.h"

//////////////////////////////////////////////////////////////////////////
// AGIPJointAuthen
AGIPJointAuthen::AGIPJointAuthen(char sep1, char sep2)
: AGIPPlainText(AGIP_CURRENT_VERSION, CMD_JOINT_AUTHEN, sep1, sep2)
{
}

AGIPJointAuthen::~AGIPJointAuthen(void)
{
}

int AGIPJointAuthen::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPJointAuthen\n");
    printf("Params:\t%s\n", m_pucPDU + sizeof(_AGIP_HEADER_));
    printf("--------------------------------------------------------AGIPJointAuthen\n");
    return S_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
// AGIPJointAuthenRes
AGIPJointAuthenRes::AGIPJointAuthenRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_JOINT_AUTHEN_RES, sizeof(SAGIPJointAuthenRes))
{
}

AGIPJointAuthenRes::~AGIPJointAuthenRes()
{
}

int32_t AGIPJointAuthenRes::getResultCode()const
{
    return htonl(((PSAGIPJointAuthenRes)m_pucPDU)->n_Result_Code);
}

uint32_t AGIPJointAuthenRes::getUserID()const
{
    return htonl(((PSAGIPJointAuthenRes)m_pucPDU)->un_User_ID);
}

int8_t AGIPJointAuthenRes::getUserType()const
{
    return ((PSAGIPJointAuthenRes)m_pucPDU)->c_User_Type;
}

int8_t AGIPJointAuthenRes::getAdultFlag()const
{
    return ((PSAGIPJointAuthenRes)m_pucPDU)->c_Adult_Flag;
}

int8_t AGIPJointAuthenRes::getUserClass()const
{
    return ((PSAGIPJointAuthenRes)m_pucPDU)->c_User_Class;
}

int8_t AGIPJointAuthenRes::getUserRoleCount()const
{
    return ((PSAGIPJointAuthenRes)m_pucPDU)->c_User_Role_Count;
}

int32_t AGIPJointAuthenRes::getUserPoint()const
{
    return htonl(((PSAGIPJointAuthenRes)m_pucPDU)->n_User_Point);
}

uint32_t AGIPJointAuthenRes::getPromoterID()const
{
    return htonl(((PSAGIPJointAuthenRes)m_pucPDU)->un_Promoter_ID);
}

uint32_t AGIPJointAuthenRes::getUserFlag()const
{
    return htonl(((PSAGIPJointAuthenRes)m_pucPDU)->un_User_Flag);
}

void AGIPJointAuthenRes::setResultCode(int32_t nResultCode)
{
    ((PSAGIPJointAuthenRes)m_pucPDU)->n_Result_Code = htonl(nResultCode);
}

void AGIPJointAuthenRes::setUserID(uint32_t unUserID)
{
    ((PSAGIPJointAuthenRes)m_pucPDU)->un_User_ID = htonl(unUserID);
}

void AGIPJointAuthenRes::setUserType(int8_t cUserType)
{
    ((PSAGIPJointAuthenRes)m_pucPDU)->c_User_Type = cUserType;
}

void AGIPJointAuthenRes::setAdultFlag(int8_t cAdultFlag)
{
    ((PSAGIPJointAuthenRes)m_pucPDU)->c_Adult_Flag = cAdultFlag;
}

void AGIPJointAuthenRes::setUserClass(int8_t cUserClass)
{
    ((PSAGIPJointAuthenRes)m_pucPDU)->c_User_Class = cUserClass;
}

void AGIPJointAuthenRes::setUserRoleCount(int8_t cUserRoleCount)
{
    ((PSAGIPJointAuthenRes)m_pucPDU)->c_User_Role_Count = cUserRoleCount;
}

void AGIPJointAuthenRes::setUserPoint(int32_t nUserPoint)
{
    ((PSAGIPJointAuthenRes)m_pucPDU)->n_User_Point = htonl(nUserPoint);
}

void AGIPJointAuthenRes::setPromoterID(uint32_t unPromoterID)
{
    ((PSAGIPJointAuthenRes)m_pucPDU)->un_Promoter_ID = htonl(unPromoterID);
}

void AGIPJointAuthenRes::setUserFlag(uint32_t unUserFlag)
{
    ((PSAGIPJointAuthenRes)m_pucPDU)->un_User_Flag = htonl(unUserFlag);
}

int AGIPJointAuthenRes::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPJointAuthenRes\n");
    printf("Result_Code:        %d\n", getResultCode()  );
    printf("User_ID:            %u\n", getUserID()      );
    printf("User_Type:          %d\n", getUserType()    );
    printf("Adult_Flag:         %d\n", getAdultFlag()   );
    printf("User_Class:         %d\n", getUserClass()   );
    printf("User_Role_Count:    %d\n", getUserRoleCount());
    printf("User_Point:         %d\n", getUserPoint()   );
    printf("Promoter_ID:        %u\n", getPromoterID()  );
    printf("User_Flag:          %u\n", getUserFlag()    );
    printf("--------------------------------------------------------AGIPJointAuthenRes\n");
    return S_SUCCESS;
}

