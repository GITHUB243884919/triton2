#ifndef __AGIP_JOINT_AUTHEN_H__
#define __AGIP_JOINT_AUTHEN_H__

#include "AGIPPlainText.h"

#define CMD_JOINT_AUTHEN                0x10003801
#define CMD_JOINT_AUTHEN_RES            0x20003801

typedef struct _AGIP_JOINT_AUTHEN_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    uint32_t        un_User_ID;
    int8_t          c_User_Type;
    int8_t          c_Adult_Flag;
    int8_t          c_User_Class;
    int8_t          c_User_Role_Count;
    int32_t         n_User_Point;
    uint32_t        un_Promoter_ID;
    uint32_t        un_User_Flag;
    _AGIP_CHECKSUM_ checksum;
} SAGIPJointAuthenRes, *PSAGIPJointAuthenRes;

class Engine_Export AGIPJointAuthen :
    public AGIPPlainText
{
public:
    AGIPJointAuthen(char sep1, char sep2);
    virtual ~AGIPJointAuthen(void);

    virtual int showInfo();
};

class Engine_Export AGIPJointAuthenRes :
    public SysProtocol
{
public:
    AGIPJointAuthenRes(void);
    virtual ~AGIPJointAuthenRes(void);

    int32_t  getResultCode()const;
    uint32_t getUserID()const;
    int8_t   getUserType()const;
    int8_t   getAdultFlag()const;
    int8_t   getUserClass()const;
    int8_t   getUserRoleCount()const;
    int32_t  getUserPoint()const;
    uint32_t getPromoterID()const;
    uint32_t getUserFlag()const;

    void setResultCode(int32_t nResultCode);
    void setUserID(uint32_t unUserID);
    void setUserType(int8_t cUserType);
    void setAdultFlag(int8_t cAdultFlag);
    void setUserClass(int8_t cUserClass);
    void setUserRoleCount(int8_t cUserRoleCount);
    void setUserPoint(int32_t nUserPoint);
    void setPromoterID(uint32_t unPromoterID);
    void setUserFlag(uint32_t unUserFlag);

    virtual int showInfo();
};

#endif//(__AGIP_JOINT_AUTHEN_H__)
