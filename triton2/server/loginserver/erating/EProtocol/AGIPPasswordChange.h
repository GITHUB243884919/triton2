#ifndef __AGIP_PASSWORD_CHANGE_H__
#define __AGIP_PASSWORD_CHANGE_H__

#include "SysProtocol.h"

#define CMD_PASSWORD_CHANGE             0x10003204
#define CMD_PASSWORD_CHANGE_RES         0x20003204

typedef struct _AGIP_PASSWORD_CHANGE_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    int8_t          c_Verify_Flag;
    int8_t          c_Pad1;
    int8_t          c_Password_Type1;
    int8_t          c_Password_Type2;
    char            str_Password1[AGIP_PASSWORD_LEN];
    char            str_Password2[AGIP_PASSWORD_LEN];
    _AGIP_CHECKSUM_ checksum;
} SAGIPPasswordChange, *PSAGIPPasswordChange;

typedef struct _AGIP_PASSWORD_CHANGE_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPPasswordChangeRes, *PSAGIPPasswordChangeRes;


class Engine_Export AGIPPasswordChange :
    public SysProtocol
{
public:
    enum
    {
//        PASSWORD_TYPE_AUTHENTICATION = 1,
        PASSWORD_TYPE_SECURITY       = 2,
        PASSWORD_TYPE_ITEMS         = 100,
        PASSWORD_TYPE_MONEY         = 101
    };
    AGIPPasswordChange(void);

    int getUserID(uint32_t *punUserID);
    int getVerifyFlag(int8_t *pcVerifyFlag);
    int getPasswordType1(int8_t *pcPasswordType);
    int getPasswordType2(int8_t *pcPasswordType);
    int getPassword1(char *strPassword);
    int getPassword2(char *strPassword);

    int setUserID(uint32_t unUserID);
    int setVerifyFlag(int8_t cVerifyFlag);
    int setPasswordType1(int8_t cPasswordType);
    int setPasswordType2(int8_t cPasswordType);
    int setPassword1(const char *strPassword);
    int setPassword2(const char *strPassword);
    
    virtual int showInfo();

public:
    virtual ~AGIPPasswordChange(void);
};

class Engine_Export AGIPPasswordChangeRes :
    public SysProtocol
{
    friend class AGIPPasswordChange;
public:
    AGIPPasswordChangeRes(void);

    int getResultCode(int32_t *pnResultCode);
    int setResultCode(int32_t nResultCode);

    virtual int showInfo();
public:
    virtual ~AGIPPasswordChangeRes(void);
};

#endif

