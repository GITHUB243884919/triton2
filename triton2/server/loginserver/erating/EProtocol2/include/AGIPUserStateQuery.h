#ifndef __AGIP_USER_STATE_QUERY_H_
#define __AGIP_USER_STATE_QUERY_H_

#include "SysProtocol.h"

#define CMD_USER_STATE_QUERY                 0x10003004
#define CMD_USER_STATE_QUERY_RES             0x20003004

typedef struct _AGIP_USER_STATE_QUERY_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    char            str_User_Name[AGIP_USER_NAME_LEN];
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserStateQuery, *PSAGIPUserStateQuery;

typedef struct _AGIP_USER_STATE_QUERY_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    uint32_t        un_User_State;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserStateQueryRes, *PSAGIPUserStateQueryRes;


class Engine_Export AGIPUserStateQuery :
    public SysProtocol
{
public:
    AGIPUserStateQuery();
    virtual ~AGIPUserStateQuery();

    uint32_t getUserID();
    const char* getUserName(char* szUserName, size_t unSize);

    void setUserID(uint32_t unUserID);
    void setUserName(const char *szUserName);
    
    virtual int32_t showInfo();
};

class Engine_Export AGIPUserStateQueryRes :
    public SysProtocol
{
public:
    AGIPUserStateQueryRes();
    virtual ~AGIPUserStateQueryRes();

    int32_t getResultCode();
    uint32_t getUserState();

    void setResultCode(int32_t nResultCode);
    void setUserState(uint32_t unUserState);

    virtual int32_t showInfo();
};

#endif // __AGIP_USER_STATE_QUERY_H_

