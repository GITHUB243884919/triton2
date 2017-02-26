#ifndef __AGIP_USER_LOGIN_H__
#define __AGIP_USER_LOGIN_H__

#include "SysProtocol.h"

#define CMD_USER_LOGIN                  0x10003301
#define CMD_USER_LOGIN_RES              0x20003301

typedef struct _AGIP_USER_LOGIN_
{
    _AGIP_HEADER_   header;
    char            str_User_Name[AGIP_USER_NAME_LEN];
    char            str_Password[AGIP_PASSWORD_LEN];
    int8_t          c_Password_Type;
    int8_t          c_Pad1;
    uint16_t        us_User_Port;
    uint32_t        un_User_IP;
    char            str_Matrix_Password[AGIP_PASSWORD_LEN];
    char            str_Matrix_Coord[AGIP_MATRIX_COORD_LEN];
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserLogin, *PSAGIPUserLogin;

typedef struct _AGIP_USER_LOGIN_RES_
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
} SAGIPUserLoginRes, *PSAGIPUserLoginRes;


class Engine_Export AGIPUserLogin :
    public SysProtocol
{
public:
    enum
    {
        PASSWORD_TYPE_AUTHENTICATION = 1,
        PASSWORD_TYPE_SECURITY       = 2,
        PASSWORD_TYPE_MATRIX
    };

    AGIPUserLogin(void);

    int getUserName(char *strUserName);
    int getPassword(char *strPassword);
    int getPasswordType(int8_t *pcPasswordType);
    int getUserPort(uint16_t *pusUserPort);
    int getUserIP(uint32_t *punUserIP);
    int getMatrixPassword(char* szMatrixPassword);
    int getMatrixCoord(char* szMatrixCoord);

    int setUserName(const char *strUserName);
    int setPassword(const char *strPassword);
    int setPasswordType(int8_t cPasswordType);
    int setUserPort(uint16_t usUserPort);
    int setUserIP(uint32_t unUserIP);
    int setMatrixPassword(const char* szMatrixPassword);
    int setMatrixCoord(const char* szMatrixCoord);

    virtual int showInfo();

public:
    virtual ~AGIPUserLogin(void);
};

class Engine_Export AGIPUserLoginRes :
    public SysProtocol
{
    friend class AGIPUserLogin;
public:
    AGIPUserLoginRes(void);

    int getResultCode(int32_t *pnResultCode);
    int getUserID(uint32_t *punUserID);
    int getUserType(int8_t *pcUserType);
    int getAdultFlag(int8_t *pcAdultFlag);
    int getUserClass(int8_t *pcUserClass);
    int getUserRoleCount(int8_t *pcUserRoleCount);
    int getUserPoint(int32_t *pnUserPoint);
    int getPromoterID(uint32_t *punPromoterID);
    int getUserFlag(uint32_t *punUserFlag);

    int setResultCode(int32_t nResultCode);
    int setUserID(uint32_t unUserID);
    int setUserType(int8_t cUserType);
    int setAdultFlag(int8_t cAdultFlag);
    int setUserClass(int8_t cUserClass);
    int setUserRoleCount(int8_t cUserRoleCount);
    int setUserPoint(int32_t nUserPoint);
    int setPromoterID(uint32_t unPromoterID);
    int setUserFlag(uint32_t unUserFlag);

    virtual int showInfo();
    
public:
    virtual ~AGIPUserLoginRes(void);
};

#endif
