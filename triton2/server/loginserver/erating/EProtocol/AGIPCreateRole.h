#ifndef __AGIP_CREATE_ROLE_H__
#define __AGIP_CREATE_ROLE_H__

#include "SysProtocol.h"

#define CMD_CREATE_ROLE                 0x10003102
#define CMD_CREATE_ROLE_RES             0x20003102

typedef struct _AGIP_CREATE_ROLE_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    char            str_Role_Name[AGIP_ROLE_NAME_LEN];
    int8_t          c_Gender;
    int8_t          c_Occupation;
    uint16_t        us_Initial_Level;
    uint32_t        un_User_IP;
    uint16_t        us_User_Port;
    uint8_t         uc_Community_ID;
    int8_t          c_Pad1;
    _AGIP_CHECKSUM_ checksum;
} SAGIPCreateRole, *PSAGIPCreateRole;

typedef struct _AGIP_CREATE_ROLE_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    uint32_t        un_Role_ID;
    _AGIP_CHECKSUM_ checksum;
} SAGIPCreateRoleRes, *PSAGIPCreateRoleRes;


class Engine_Export AGIPCreateRole :
    public SysProtocol
{
public:
    AGIPCreateRole(void);
    enum
    {
        GENDER_MALE   = 1,
        GENDER_FEMALE = 2
    };

    int getUserID(uint32_t *punUserID);
    int getRoleName(char *strRoleName);
    int getGender(int8_t *pcGender);
    int getOccupation(int8_t *pcOccupation);
    // int getInitialLevel(int8_t *pcInitialLevel);
    int getInitialLevel(uint16_t* pusInitialLevel);
    int getCommunityID(uint8_t *pucCommunityID);
    int getUserIP(uint32_t *punUserIP);
    int getUserPort(uint16_t *pusUserPort);

    int setUserID(uint32_t unUserID);
    int setRoleName(const char *strRoleName);
    int setGender(int8_t cGender);
    int setOccupation(int8_t cOccupation);
    // int setInitialLevel(int8_t cInitialLevel);
    int setInitialLevel(uint16_t usInitialLevel);
    int setCommunityID(uint8_t ucCommunityID);
    int setUserIP(uint32_t unUserIP);
    int setUserPort(uint16_t usUserPort);
    
    virtual int showInfo();

public:
    virtual ~AGIPCreateRole(void);
};

class Engine_Export AGIPCreateRoleRes :
    public SysProtocol
{
    friend class AGIPCreateRole;
public:
    AGIPCreateRoleRes(void);

    int getResultCode(int32_t *pnResultCode);
    int getRoleID(uint32_t *punRoleID);

    int setResultCode(int32_t nResultCode);
    int setRoleID(uint32_t unRoleID);

    virtual int showInfo();

public:
    virtual ~AGIPCreateRoleRes(void);
};

#endif
