#ifndef __AGIP_USER_LOGOUT_H__
#define __AGIP_USER_LOGOUT_H__

#include "SysProtocol.h"

#define CMD_USER_LOGOUT                 0x10003303
#define CMD_USER_LOGOUT_RES             0x20003303

typedef struct _AGIP_USER_LOGOUT_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    int8_t          c_Logout_Flag;
    int8_t          c_Occupation;
    uint16_t        us_Role_Level;
    uint32_t        un_Rating_ID;
    uint32_t        un_Money1;
    uint32_t        un_Money2;
    uint32_t        un_Experience_Higher;
    uint32_t        un_Experience_Lower;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserLogout, *PSAGIPUserLogout;

class Engine_Export AGIPUserLogout :
    public SysProtocol
{
    
public:
    enum
    {
        LOGOUT_FLAG_PAYMENT_AND_LOGOUT = 1,
        LOGOUT_FLAG_PAYMENT_ONLY       = 2,

        FEE_FLAG_FREE = 0,
        FEE_FLAG_NOT_FREE = 1,

        FEE_FACTOR_NORMAL = 1

    };
    AGIPUserLogout(void);
    
    int getUserID(uint32_t *punUserID);
    int getRoleID(uint32_t *punRoleID);
    int getRatingID(uint32_t *punRatingID);
    int getLogoutFlag(int8_t *pcLogoutFlag);
    int getRoleLevel(uint16_t* pusRoleLevel);
    int getOccupation(int8_t *pcOccupation);
    int getMoney1(uint32_t *punMoney1);  // 获取金钱数 1
    int getMoney2(uint32_t *punMoney2);  // 获取金钱数 2
    int getExperience(uint64_t *pullExperience); // 获取当前的经验数。


    int setUserID(uint32_t unUserID);
    int setRoleID(uint32_t unRoleID);
    int setRatingID(uint32_t unRatingID);
    int setLogoutFlag(int8_t cLogoutFlag);
    int setRoleLevel(uint16_t usRoleLevel);
    int setOccupation(int8_t cOccupation);

    int setMoney1(uint32_t unMoney1);  // 设置金钱数 1
    int setMoney2(uint32_t unMoney2);  // 设置金钱数 2
    int setExperience(uint64_t ullExperience); // 设置当前的经验数。

    int showInfo();

public:
    virtual ~AGIPUserLogout(void);
};

typedef struct _AGIP_USER_LOGOUT_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    int32_t         n_Balance_Count;
    int32_t         n_Subject_ID;
    int32_t         n_Amount;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserLogoutRes, *PSAGIPUserLogoutRes;

class Engine_Export AGIPUserLogoutRes :
    public SysProtocol
{
    friend class AGIPUserLogout;
public:
    AGIPUserLogoutRes(void);
    virtual ~AGIPUserLogoutRes(void);

    virtual int initialize();

    int getResultCode(int32_t *pnResultCode);
    int getBalanceCount(int32_t *pnBalanceCount);
    int getBalanceInfo(int32_t nIndex, int32_t *pnSubjectID, int32_t *pnAmount);
    int getBalanceInfo(int32_t nIndex, BALANCE_INFO *pBalanceInfo);

    int setResultCode(int32_t nResultCode);

    int addBalanceInfo(int32_t nSubjectID, int32_t nAmount);
    int addBalanceInfo(const BALANCE_INFO *pBalanceInfo);

    int showInfo();

private:
    int setBalanceCount(int32_t nBalanceCount);
};

#endif
