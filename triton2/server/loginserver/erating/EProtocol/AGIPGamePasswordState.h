#ifndef __AGIP_GAME_PASSWORD_STATE_H__
#define __AGIP_GAME_PASSWORD_STATE_H__
#include "SysProtocol.h"

#define CMD_GAME_PASSWORD_STATE         0x10003205
#define CMD_GAME_PASSWORD_STATE_RES     0x20003205

typedef struct _AGIP_GAME_PASSWORD_STATE_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    uint32_t        un_Gateway_ID;
    _AGIP_CHECKSUM_ checksum;
} SAGIPGamePasswordState, *PSAGIPGamePasswordState;

typedef struct _AGIP_GAME_PASSWORD_STATE_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    int32_t         n_Password_Count;
    int8_t          c_Password_Type;
    int8_t          c_Password_State;
    int16_t         s_Pad;
    _AGIP_CHECKSUM_ checksum;
} SAGIPGamePasswordStateRes, *PSAGIPGamePasswordStateRes;

class Engine_Export AGIPGamePasswordState :
    public SysProtocol
{
public:
    AGIPGamePasswordState(void);
public:
    virtual ~AGIPGamePasswordState(void);
public:
    int getUserID(uint32_t *punUserID); 
    int getGatewayID(uint32_t *punGatewayID);

    int setUserID(uint32_t unUserID); 
    int setGatewayID(uint32_t unGatewayID);

    virtual int showInfo();
};

class Engine_Export AGIPGamePasswordStateRes :
    public SysProtocol
{
    friend class AGIPGamePasswordState;
public:
    AGIPGamePasswordStateRes(void);
public:
    virtual ~AGIPGamePasswordStateRes(void);

    virtual int initialize();
public:
    int getResultCode(int32_t *pnResultCode); 
    int getPasswordCount(int32_t *pnBalanceCount);
    int getPasswordState(int32_t nIndex, uint8_t* pcPasswordType, uint8_t* pcPasswordState);
    int getPasswordState(int32_t nIndex, PGAMEPASSWORDSTATE pPasswordState);

    int setResultCode(int32_t nResultCode); 
    int addPasswordState(uint8_t cPasswordType, uint8_t cPasswordState);
    int addPasswordState(const PGAMEPASSWORDSTATE pGamePasswordState);
    int showInfo();
private:
    int setPasswordCount(uint32_t unPasswordCount);
};


#endif//(__AGIP_GAME_PASSWORD_STATE_H__)

