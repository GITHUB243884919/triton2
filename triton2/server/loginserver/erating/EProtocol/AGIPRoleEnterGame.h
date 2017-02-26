#ifndef __AGIP_ROLE_ENTER_GAME_H__
#define __AGIP_ROLE_ENTER_GAME_H__

#include "SysProtocol.h"

#define CMD_ROLE_ENTER_GAME             0x10003105
#define CMD_ROLE_ENTER_GAME_RES         0x20003105

typedef struct _AGIP_ROLE_ENTER_GAME_
{
    _AGIP_HEADER_   header;
    uint16_t        us_Game_ID;
    uint16_t        us_Pad1;
    uint32_t        un_Gateway_ID;
    uint32_t        un_Server_ID;
    uint32_t        un_User_ID;
    uint32_t        un_Role_ID;
    uint16_t        us_Level;
    uint8_t         uc_Gender;
    uint8_t         uc_Occupation_ID;
    uint32_t        un_Corps_ID;
    uint16_t        us_Community_ID;
    uint16_t        us_Client_Port;
    uint32_t        un_Client_IP;
    int8_t          str_Client_MAC[AGIP_MAC_LEN];
    _AGIP_CHECKSUM_ checksum;
} SAGIPRoleEnterGame, *PSAGIPRoleEnterGame;

typedef struct _AGIP_ROLE_ENTER_GAME_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    int32_t         n_Balance_Count;
    int32_t         n_Subject_ID;
    int32_t         n_Amount;
    _AGIP_CHECKSUM_ checksum;
} SAGIPRoleEnterGameRes, *PSAGIPRoleEnterGameRes;


class Engine_Export AGIPRoleEnterGame :
    public SysProtocol
{
public:
    AGIPRoleEnterGame(void);
    ~AGIPRoleEnterGame(void);

    uint16_t getGameID(void);
    uint32_t getGatewayID(void);
    uint32_t getServerID(void);
    uint32_t getUserID(void);
    uint32_t getRoleID(void);
    uint16_t getLevel(void);
    uint8_t  getGender(void);
    uint8_t  getOccupationID(void);
    uint32_t getCorpsID(void);
    uint16_t getCommunityID(void);
    uint16_t getClientPort(void);
    uint32_t getClientIP(void);
    const char* getClientMAC(char* szClientMAC, size_t* punLen);

    void setGameID(uint16_t usGameID);
    void setGatewayID(uint32_t unGatewayID);
    void setServerID(uint32_t unServerID);
    void setUserID(uint32_t unUserID);
    void setRoleID(uint32_t unRoleID);
    void setLevel(uint16_t usLevel);
    void setGender(uint8_t ucGender);
    void setOccupationID(uint8_t ucOccupationID);
    void setCorpsID(uint32_t unCorpsID);
    void setCommunityID(uint16_t usCommunityID);
    void setClientPort(uint16_t usPort);
    void setClientIP(uint32_t unIP);
    void setClientMAC(const char* szMAC);

    int showInfo();

private:
    PSAGIPRoleEnterGame m_data;
};

class Engine_Export AGIPRoleEnterGameRes :
    public SysProtocol
{
    friend class AGIPRoleEnterGame;
public:
    AGIPRoleEnterGameRes(void);
    virtual ~AGIPRoleEnterGameRes(void);
    
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

    PSAGIPRoleEnterGameRes m_data;
};

#endif

