#ifndef __AGIP_PF_CHARGE_H__
#define __AGIP_PF_CHARGE_H__

#include "SysProtocol.h"

#define CMD_PF_CHARGE                 0x30003002
#define CMD_PF_CHARGE_RES             0x40003002

// AGIPPfCharge -------------------------------------------------------------
typedef struct _AGIP_PF_CHARGE_
{
    _AGIP_HEADER_   header;
    char            sz_Order_ID[AGIP_ORDER_ID_LEN];
    char            sz_User_Name[AGIP_USER_NAME_LEN];
    int32_t         n_Charge_Money;
    int32_t         n_Charge_Amount;
    uint32_t        un_Charge_Time;
    uint32_t        un_Server_IP;
    uint32_t        un_Clinet_IP;
    _AGIP_CHECKSUM_ checksum;
} SAGIPPfCharge, *PSAGIPPfCharge;

class Engine_Export AGIPPfCharge :
    public SysProtocol
{
public:
    inline AGIPPfCharge(void);
    inline ~AGIPPfCharge(void);

    // getter ---------------------------------------------------------------
    inline const char* getOrderID(char* buf, uint32_t buf_len) const;
    inline const char* getUserName(char* buf, uint32_t buf_len) const;
    inline int32_t getChargeMoney(void) const;
    inline int32_t getChargeAmount(void) const;
    inline uint32_t getChargeTime(void) const;
    inline uint32_t getServerIP(void) const;
    inline uint32_t getClientIP(void) const;

    // setter ---------------------------------------------------------------
    inline void setOrderID(const char* szOrderID);
    inline void setUserName(const char* szUserName);
    inline void setChargeMoney(int32_t nChargeMoney);
    inline void setChargeAmount(int32_t nChargeAmount);
    inline void setChargeTime(uint32_t unChargeTime);
    inline void setServerIP(uint32_t unServerIP);
    inline void setClientIP(uint32_t unClientIP);

    inline virtual int showInfo(void);

private:
    SAGIPPfCharge* m_data;
};

// AGIPPfChargeRes ----------------------------------------------------------
typedef struct _AGIP_PF_CHARGE_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result;
    char            sz_Order_ID[AGIP_ORDER_ID_LEN];
    _AGIP_CHECKSUM_ checksum;
} SAGIPPfChargeRes, *PSAGIPPfChargeRes;

class Engine_Export AGIPPfChargeRes :
    public SysProtocol
{
public:
    inline AGIPPfChargeRes(void);
    inline ~AGIPPfChargeRes(void);

    // getter ---------------------------------------------------------------
    inline int32_t getResult(void) const;
    inline const char* getOrderID(char* buf, uint32_t buf_len) const;

    // setter ---------------------------------------------------------------
    inline void setResult(int32_t nResult);
    inline void setOrderID(const char* szOrderID);

    inline virtual int showInfo(void);

private:
    SAGIPPfChargeRes* m_data;
};

// AGIPPfCharge -------------------------------------------------------------
AGIPPfCharge::AGIPPfCharge(void) :
    SysProtocol(AGIP_CURRENT_VERSION, CMD_PF_CHARGE, sizeof(SAGIPPfCharge))
{
    m_data = (SAGIPPfCharge*)m_pucPDU;
}

AGIPPfCharge::~AGIPPfCharge(void)
{
}

const char* AGIPPfCharge::getOrderID(char* buf, uint32_t buf_len) const
{
    if (buf_len > AGIP_ORDER_ID_LEN) {
        strncpy(buf, m_data->sz_Order_ID, AGIP_ORDER_ID_LEN);
        return buf;
    }
    return NULL;
}

const char* AGIPPfCharge::getUserName(char* buf, uint32_t buf_len) const
{
    if (buf_len > AGIP_USER_NAME_LEN) {
        strncpy(buf, m_data->sz_User_Name, AGIP_USER_NAME_LEN);
        return buf;
    }
    return NULL;
}

int32_t AGIPPfCharge::getChargeMoney(void) const
{
    return ntohl(m_data->n_Charge_Money);
}

int32_t AGIPPfCharge::getChargeAmount(void) const
{
    return ntohl(m_data->n_Charge_Amount);
}

uint32_t AGIPPfCharge::getChargeTime(void) const
{
    return ntohl(m_data->un_Charge_Time);
}

uint32_t AGIPPfCharge::getServerIP(void) const
{
    return ntohl(m_data->un_Server_IP);
}

uint32_t AGIPPfCharge::getClientIP(void) const
{
    return ntohl(m_data->un_Clinet_IP);
}

// setter ---------------------------------------------------------------

void AGIPPfCharge::setOrderID(const char* szOrderID)
{
    ASSERT(szOrderID != NULL);
    strncpy(m_data->sz_Order_ID, szOrderID, AGIP_ORDER_ID_LEN);
}

void AGIPPfCharge::setUserName(const char* szUserName)
{
    ASSERT(szUserName != NULL);
    strncpy(m_data->sz_User_Name, szUserName, AGIP_USER_NAME_LEN);
}

void AGIPPfCharge::setChargeMoney(int32_t nChargeMoney)
{
    m_data->n_Charge_Money = htonl(nChargeMoney);
}

void AGIPPfCharge::setChargeAmount(int32_t nChargeAmount)
{
    m_data->n_Charge_Amount = htonl(nChargeAmount);
}

void AGIPPfCharge::setChargeTime(uint32_t unChargeTime)
{
    m_data->un_Charge_Time = htonl(unChargeTime);
}

void AGIPPfCharge::setServerIP(uint32_t unServerIP)
{
    m_data->un_Server_IP = htonl(unServerIP);
}

void AGIPPfCharge::setClientIP(uint32_t unClientIP)
{
    m_data->un_Clinet_IP = htonl(unClientIP);
}

int AGIPPfCharge::showInfo(void)
{
    char szIP[64] = {'\0'};
    char buf[64] = {'\0'};
    SysProtocol::showInfo();
    printf("------------------------------------------------------------AGIPPfCharge\n");
    printf("Order_ID:      %s\n", getOrderID(buf, sizeof(buf)));
    printf("User_Name      %s\n", getUserName(buf, sizeof(buf)));
    printf("Charge_Money:  %d\n", getChargeMoney());
    printf("Charge_Amount: %d\n", getChargeAmount());
    printf("Charge_Time:   %s\n", stringTime(getChargeTime()));
    IP2String(getServerIP(), szIP);
    printf("Server_IP:     %s\n", szIP);
    IP2String(getClientIP(), szIP);
    printf("Client_IP:     %s\n", szIP);
    printf("------------------------------------------------------------AGIPPfCharge\n");
    return S_SUCCESS;
}

// AGIPPfChargeRes ----------------------------------------------------------
AGIPPfChargeRes::AGIPPfChargeRes(void) : 
    SysProtocol(AGIP_CURRENT_VERSION, CMD_PF_CHARGE_RES, sizeof(SAGIPPfChargeRes))
{
    m_data = (SAGIPPfChargeRes*)m_pucPDU;
}

AGIPPfChargeRes::~AGIPPfChargeRes(void)
{
}

// getter ---------------------------------------------------------------
int32_t AGIPPfChargeRes::getResult(void) const
{
    return ntohl(m_data->n_Result);
}

const char* AGIPPfChargeRes::getOrderID(char* buf, uint32_t buf_len) const
{
    if (NULL == buf) {
        return NULL;
    }

    if (buf_len <= AGIP_ORDER_ID_LEN) {
        return NULL;
    }

    memcpy(buf, m_data->sz_Order_ID, AGIP_ORDER_ID_LEN);
    buf[AGIP_ORDER_ID_LEN] = 0;

    return buf;
}


// setter ---------------------------------------------------------------
void AGIPPfChargeRes::setResult(int32_t nResult)
{
    m_data->n_Result = htonl(nResult);
}

void AGIPPfChargeRes::setOrderID(const char* szOrderID)
{
    ASSERT(szOrderID != NULL);
    strncpy(m_data->sz_Order_ID, szOrderID, AGIP_ORDER_ID_LEN);
}

int AGIPPfChargeRes::showInfo(void)
{
    char buf[64] = {0};
    SysProtocol::showInfo();
    printf("------------------------------------------------------------AGIPPfChargeRes\n");
    printf("Result:   %d\n", getResult());
    printf("Order_ID: %s\n", getOrderID(buf, sizeof(buf)));
    printf("------------------------------------------------------------AGIPPfChargeRes\n");
    return S_SUCCESS;
}


#endif /*__AGIP_PF_CHARGE_H__*/

