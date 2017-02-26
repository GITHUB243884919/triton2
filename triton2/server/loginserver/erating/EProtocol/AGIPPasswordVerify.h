#ifndef __AGIP_PASSWORD_VERIFY_H__
#define __AGIP_PASSWORD_VERIFY_H__

#include "SysProtocol.h"

#define CMD_PASSWORD_VERIFY             0x10003202
#define CMD_PASSWORD_VERIFY_RES         0x20003202

typedef struct _AGIP_PASSWORD_VERIFY_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    char            str_Password[AGIP_PASSWORD_LEN];
    int8_t          c_Password_Type;
    int8_t          c_Pad1;
    int16_t         s_Pad2;
    _AGIP_CHECKSUM_ checksum;
} SAGIPPasswordVerify, *PSAGIPPasswordVerify;

typedef struct _AGIP_PASSWORD_VERIFY_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPPasswordVerifyRes, *PSAGIPPasswordVerifyRes;

class Engine_Export AGIPPasswordVerify :
    public SysProtocol
{
public:
    enum
    {
        PASSWORD_TYPE_AUTHENTICATION = 1,
        PASSWORD_TYPE_SECURITY,
        PASSWORD_TYPE_MATRIX,
        PASSWORD_TYPE_DYNAMIC
    };
    AGIPPasswordVerify(void);

    int getUserID(uint32_t *punUserID);
    int getPassword(char *strPassword);
    int getPasswordType(int8_t *pcPasswordType);

    int setUserID(uint32_t unUserID);
    int setPassword(const char *strPassword);
    int setPasswordType(int8_t cPasswordType);
    
    virtual int showInfo();

public:
    virtual ~AGIPPasswordVerify(void);
};

class Engine_Export AGIPPasswordVerifyRes :
    public SysProtocol
{
    friend class AGIPPasswordVerify;
public:
    AGIPPasswordVerifyRes(void);

    int getResultCode(int32_t *pnResultCode);
    int setResultCode(int32_t nResultCode);

    virtual int showInfo();
public:
    virtual ~AGIPPasswordVerifyRes(void);
};

#endif

