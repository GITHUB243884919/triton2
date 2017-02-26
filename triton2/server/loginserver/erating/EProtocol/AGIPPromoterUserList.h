#ifndef __AGIP_PROMOTER_USER_LIST_H__
#define __AGIP_PROMOTER_USER_LIST_H__

#include "SysProtocol.h"

#define CMD_PROMOTER_USER_LIST                   0x10004101
#define CMD_PROMOTER_USER_LIST_RES               0x20004101

// AGIPPromoterUserList

typedef struct _AGIP_PROMOTER_USER_LIST_
{
    _AGIP_HEADER_   header;
    uint16_t        us_Game_ID;
    uint16_t        pad;
    uint32_t        un_Gateway_ID;
    uint32_t        un_User_ID;
    _AGIP_CHECKSUM_ checksum;
} SAGIPPromoterUserList, *PSAGIPPromoterUserList;

class Engine_Export AGIPPromoterUserList :
    public SysProtocol
{
public:
    AGIPPromoterUserList(void);
    ~AGIPPromoterUserList(void);

    uint16_t getGameID(void) const;
    void setGameID(const uint16_t usGameID);

    uint32_t getGatewayID(void) const;
    void setGatewayID(const uint32_t unGatewayID);
    
    uint32_t getUserID(void) const;
    int32_t setUserID(const uint32_t unUserID);

    virtual int32_t showInfo();

private:
    SAGIPPromoterUserList* m_data;
};

// AGIPPromoterUserListRes

typedef struct _AGIP_PROMOTER_USER_LIST_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    int32_t         n_All_User_Count;
    int32_t         n_User_Count;
    char            str_User_Name[AGIP_USER_NAME_LEN];
    _AGIP_CHECKSUM_ checksum;
} SAGIPPromoterUserListRes, *PSAGIPPromoterUserListRes;

typedef struct _PROMOTER_USER_INFO_
{
    char            str_User_Name[AGIP_USER_NAME_LEN];
} PROMOTER_USER_INFO;

class Engine_Export AGIPPromoterUserListRes :
    public SysProtocol
{
public:
    enum {
        MAX_USER_COUNT = 20
    };
public:
    AGIPPromoterUserListRes(void);
    ~AGIPPromoterUserListRes(void);

    virtual int32_t initialize();
public:
    int32_t getResultCode(void) const;
    int32_t setResultCode(const int32_t nResultCode);

    int32_t getAllUserCount(void) const;
    int32_t setAllUserCount(const int32_t nAllUserCount);

    int32_t getUserCount(void) const;

    int32_t getUserInfo(int32_t nIndex, PROMOTER_USER_INFO& userInfo) const;
    int32_t addUserInfo(const PROMOTER_USER_INFO& userInfo);

    virtual int32_t showInfo();
    
private:
    SAGIPPromoterUserListRes* m_data;
};

#endif // __AGIP_PROMOTER_USER_LIST_H__

