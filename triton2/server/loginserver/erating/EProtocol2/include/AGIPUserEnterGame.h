#ifndef __AGIP_USER_ENTER_GAME_H__
#define __AGIP_USER_ENTER_GAME_H__

#include "SysProtocol.h"

#define CMD_USER_ENTER_GAME             0x10003302
#define CMD_USER_ENTER_GAME_RES         0x20003302

typedef struct _AGIP_USER_ENTER_GAME_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    uint32_t        un_Rating_ID;
    uint32_t        un_Server_ID;
    uint16_t        us_Role_Level;
    uint16_t        us_User_Port;
    uint32_t        un_User_IP;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserEnterGame, *PSAGIPUserEnterGame;

typedef struct _AGIP_USER_ENTER_GAME_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    int32_t         n_Balance_Count;
    int32_t         n_Subject_ID;
    int32_t         n_Amount;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserEnterGameRes, *PSAGIPUserEnterGameRes;


class Engine_Export AGIPUserEnterGame :
    public SysProtocol
{
public:
    AGIPUserEnterGame(void);

    int getUserID(uint32_t *punUserID);
    int getRoleID(uint32_t *punRoleID);
    int getServerID(uint32_t *punServerID);
    int getRatingID(uint32_t *punRatingID);
    // int getRoleLevel(int8_t *pcRoleLevel);
    int getRoleLevel(uint16_t* pusRoleLevel);
    int getUserPort(uint16_t *pusUserPort);
    int getUserIP(uint32_t *punUserIP);

    
    int setUserID(uint32_t unUserID);
    int setRoleID(uint32_t unRoleID);
    int setServerID(uint32_t unServerID);
    int setRatingID(uint32_t unRatingID);
    // int setRoleLevel(int8_t cRoleLevel);
    int setRoleLevel(uint16_t usRoleLevel);
    int setUserPort(uint16_t usUserPort);
    int setUserIP(uint32_t unUserIP);

    int showInfo();

public:
    virtual ~AGIPUserEnterGame(void);
};

class Engine_Export AGIPUserEnterGameRes :
    public SysProtocol
{
    friend class AGIPUserEnterGame;
public:
    AGIPUserEnterGameRes(void);
    virtual ~AGIPUserEnterGameRes(void);
    
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

