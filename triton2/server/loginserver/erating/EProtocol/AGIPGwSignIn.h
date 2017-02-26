#ifndef __AGIP_GW_SIGN_IN_H__
#define __AGIP_GW_SIGN_IN_H__

#include "SysProtocol.h"

#define CMD_GW_SIGN_IN                 0x30000001
#define CMD_GW_SIGN_IN_RES             0x40000001

// AGIPGwSignIn -------------------------------------------------------------
typedef struct _AGIP_GW_SIGN_IN_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Game_ID;
    char            sz_Gw_Code[AGIP_GATEWAY_CODE_LEN];
    char            sz_Password[AGIP_PASSWORD_LEN];
    _AGIP_CHECKSUM_ checksum;
} SAGIPGwSignIn, *PSAGIPGwSignIn;

class Engine_Export AGIPGwSignIn :
    public SysProtocol
{
public:
    inline AGIPGwSignIn(void);
    inline ~AGIPGwSignIn(void);

    // getter ---------------------------------------------------------------
    inline uint32_t getGameID(void) const;
    inline const char* getGwCode(char* buf, uint32_t buf_len) const;
    inline const char* getPassword(char* buf, uint32_t buf_len) const;

    // setter ---------------------------------------------------------------
    inline void setGameID(uint32_t unGameID);
    inline void setGwCode(const char* szGwCode);
    inline void setPassword(const char* szPassword);

    inline virtual int showInfo(void);

private:
    SAGIPGwSignIn* m_data;
};

// AGIPGwSignInRes ----------------------------------------------------------
typedef struct _AGIP_GW_SIGN_IN_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result;
    _AGIP_CHECKSUM_ checksum;
} SAGIPGwSignInRes, *PSAGIPGwSignInRes;

class Engine_Export AGIPGwSignInRes :
    public SysProtocol
{
public:
    inline AGIPGwSignInRes(void);
    inline ~AGIPGwSignInRes(void);

    // getter ---------------------------------------------------------------
    inline int32_t getResult(void) const;

    // setter ---------------------------------------------------------------
    inline void setResult(int32_t nResult);

    inline virtual int showInfo(void);

private:
    SAGIPGwSignInRes* m_data;
};

// AGIPGwSignIn -------------------------------------------------------------
AGIPGwSignIn::AGIPGwSignIn(void) :
    SysProtocol(AGIP_CURRENT_VERSION, CMD_GW_SIGN_IN, sizeof(SAGIPGwSignIn))
{
    m_data = (SAGIPGwSignIn*)m_pucPDU;
}

AGIPGwSignIn::~AGIPGwSignIn(void)
{
}

uint32_t AGIPGwSignIn::getGameID(void) const
{
    return ntohl(m_data->un_Game_ID);
}

const char* AGIPGwSignIn::getGwCode(char* buf, uint32_t buf_len) const
{
    if (buf_len > AGIP_GATEWAY_CODE_LEN) {
        strncpy(buf, m_data->sz_Gw_Code, AGIP_GATEWAY_CODE_LEN);
        return buf;
    }
    return NULL;
}

const char* AGIPGwSignIn::getPassword(char* buf, uint32_t buf_len) const
{
    if (buf_len > AGIP_PASSWORD_LEN) {
        strncpy(buf, m_data->sz_Password, AGIP_PASSWORD_LEN);
        return buf;
    }
    return NULL;
}

// setter ---------------------------------------------------------------

void AGIPGwSignIn::setGameID(uint32_t unGameID)
{
    m_data->un_Game_ID = htonl(unGameID);
}

void AGIPGwSignIn::setGwCode(const char* szGwCode)
{
    ASSERT(szGwCode != NULL);
    strncpy(m_data->sz_Gw_Code, szGwCode, AGIP_ORDER_ID_LEN);
}

void AGIPGwSignIn::setPassword(const char* szPassword)
{
    ASSERT(szPassword != NULL);
    strncpy(m_data->sz_Password, szPassword, AGIP_USER_NAME_LEN);
}

int AGIPGwSignIn::showInfo(void)
{
    char buf[64] = {0};
    SysProtocol::showInfo();
    printf("------------------------------------------------------------AGIPGwSignIn\n");
    printf("Game_ID:       %u\n", getGameID());
    printf("Gw_Code:       %s\n", getGwCode(buf, sizeof(buf)));
    printf("Password:      %s\n", getPassword(buf, sizeof(buf)));
    printf("------------------------------------------------------------AGIPGwSignIn\n");
    return S_SUCCESS;
}

// AGIPGwSignInRes ----------------------------------------------------------
AGIPGwSignInRes::AGIPGwSignInRes(void) : 
    SysProtocol(AGIP_CURRENT_VERSION, CMD_GW_SIGN_IN_RES, sizeof(SAGIPGwSignInRes))
{
    m_data = (SAGIPGwSignInRes*)m_pucPDU;
}

AGIPGwSignInRes::~AGIPGwSignInRes(void)
{
}

// getter ---------------------------------------------------------------
int32_t AGIPGwSignInRes::getResult(void) const
{
    return ntohl(m_data->n_Result);
}

// setter ---------------------------------------------------------------
void AGIPGwSignInRes::setResult(int32_t nResult)
{
    m_data->n_Result = htonl(nResult);
}

int AGIPGwSignInRes::showInfo(void)
{
    SysProtocol::showInfo();
    printf("------------------------------------------------------------AGIPGwSignInRes\n");
    printf("Result:   %d\n", getResult());
    printf("------------------------------------------------------------AGIPGwSignInRes\n");
    return S_SUCCESS;
}


#endif /*__AGIP_GW_SIGN_IN_H__*/

