#ifndef __AGIP_USER_BALANCE_INFO_H__
#define __AGIP_USER_BALANCE_INFO_H__

#include "SysProtocol.h"

#define CMD_USER_BALANCE_INFO           0x10003402
#define CMD_USER_BALANCE_INFO_RES       0x20003402

typedef struct _AGIP_USER_BALANCE_INFO_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    uint32_t        un_Rating_ID;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserBalanceInfo, *PSAGIPUserBalanceInfo;

typedef struct _AGIP_USER_BALANCE_INFO_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    int32_t         n_Balance_Count;
    int32_t         n_Subject_ID;
    int32_t         n_Amount;
    int32_t         n_Left_Amount;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserBalanceInfoRes, *PSAGIPUserBalanceInfoRes;

class Engine_Export AGIPUserBalanceInfo :
    public SysProtocol
{
public:
    AGIPUserBalanceInfo(void);
public:
    virtual ~AGIPUserBalanceInfo(void);
public:
    int getUserID(uint32_t *punUserID); 
    int getRoleID(uint32_t *punRoleID);
    int getRatingID(uint32_t *punRatingID);

    int setUserID(uint32_t unUserID); 
    int setRoleID(uint32_t unRoleID);
    int setRatingID(uint32_t unRatingID);

    int showInfo();
};

class Engine_Export AGIPUserBalanceInfoRes :
    public SysProtocol
{
    friend class AGIPUserBalanceInfo;
public:
    AGIPUserBalanceInfoRes(void);
public:
    virtual ~AGIPUserBalanceInfoRes(void);

    virtual int initialize();
public:
    int getResultCode(int32_t *pnResultCode); 
    int getBalanceCount(int32_t *pnBalanceCount);
    int getBalanceInfo(int32_t nIndex, int32_t *pnSubjectID, int32_t *pnAmount, int32_t *pnLeftAmount);
    int getBalanceInfo(int32_t nIndex, ALL_BALANCE_INFO *pAllBalanceInfo);

    int setResultCode(int32_t nResultCode); 
    int addBalanceInfo(int32_t nSubjectID, int32_t nAmount, int32_t nLeftAmount);
    int addBalanceInfo(const ALL_BALANCE_INFO *pAllBalanceInfo);
    int showInfo();
private:
    int setBalanceCount(int32_t nBalanceCount);
};


#endif

