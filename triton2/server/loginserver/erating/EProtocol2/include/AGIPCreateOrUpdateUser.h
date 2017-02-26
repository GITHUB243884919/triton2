#ifndef  __AGIPCREATEORUPDATEUSER_H__
#define  __AGIPCREATEORUPDATEUSER_H__

#include "SysProtocol.h"

#define CMD_CREATE_OR_UPDATE_USER       0x10003001
#define CMD_CREATE_OR_UPDATE_USER_RES   0x20003001

typedef struct _AGIP_CREATE_OR_UPDATE_USER_
{
    _AGIP_HEADER_   header;
    uint32_t        user_id;
    char            user_name[AGIP_USER_NAME_LEN];
    char            password[AGIP_PASSWORD_LEN];
    int8_t          user_type;
    int8_t          user_class;
    int16_t         pad;
    int32_t         user_point;
    uint32_t        promoter_id;
    uint32_t        user_flag;
    _AGIP_CHECKSUM_ checksum;
} SAGIPCreateOrUpdateUser, *PSAGIPCreateOrUpdateUser;

typedef struct _AGIP_CREATE_OR_UPDATE_USER_RES_
{
    _AGIP_HEADER_   header;
    int32_t         result;
    _AGIP_CHECKSUM_ checksum;
} SAGIPCreateOrUpdateUserRes, *PSAGIPCreateOrUpdateUserRes;

class Engine_Export AGIPCreateOrUpdateUser :
    public SysProtocol
{
public:
    AGIPCreateOrUpdateUser(void);
    virtual ~AGIPCreateOrUpdateUser(void);

    int GetUserID(uint32_t&     user_id);
    int GetUserName(char*       user_name);
    int GetPassword(char*       password);
    int GetUserType(int8_t&     user_type);
    int GetUserClass(int8_t&    user_class);
    int GetUserPoint(int32_t&   user_point);
    int GetPromoterID(uint32_t& promoter_id);
    int GetUserFlag(uint32_t&   user_flag);

    int SetUserID(uint32_t      user_id);
    int SetUserName(const char* user_name);
    int SetPassword(const char* password);
    int SetUserType(int8_t      user_type);
    int SetUserClass(int8_t     user_class);
    int SetUserPoint(int32_t    user_point);
    int SetPromoterID(uint32_t  promoter_id);
    int SetUserFlag(uint32_t    user_flag);

    virtual int showInfo();
};

class Engine_Export AGIPCreateOrUpdateUserRes :
    public SysProtocol
{
public:
    AGIPCreateOrUpdateUserRes(void);
    virtual ~AGIPCreateOrUpdateUserRes(void);

    int GetResultCode(int32_t&  result_code);

    int SetResultCode(int32_t   result_code);

    virtual int showInfo();
};

#endif  /*__AGIPCREATEORUPDATEUSER_H__*/
