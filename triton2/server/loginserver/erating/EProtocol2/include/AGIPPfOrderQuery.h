#ifndef __AGIP_PF_ORDER_QUERY_H__
#define __AGIP_PF_ORDER_QUERY_H__

#include "SysProtocol.h"

#define CMD_PF_ORDER_QUERY                 0x30003405
#define CMD_PF_ORDER_QUERY_RES             0x40003405

// AGIPPfOrderQuery ---------------------------------------------------------
typedef struct _AGIP_PF_ORDER_QUERY_
{
    _AGIP_HEADER_   header;
    char            sz_Order_ID[AGIP_ORDER_ID_LEN];
    uint16_t        us_Order_Type;
    uint16_t        un_Pad_1;
    _AGIP_CHECKSUM_ checksum;
} SAGIPPfOrderQuery, *PSAGIPPfOrderQuery;

class Engine_Export AGIPPfOrderQuery :
    public SysProtocol
{
public:
    inline AGIPPfOrderQuery(void);
    inline ~AGIPPfOrderQuery(void);

    // getter ---------------------------------------------------------------
    inline const char* getOrderID(char* buf, uint32_t buf_len) const;
    inline uint16_t getOrderType(void) const;

    // setter ---------------------------------------------------------------
    inline void setOrderID(const char* szOrderID);
    inline void setOrderType(uint16_t usOrderType);

    inline virtual int showInfo(void);

private:
    SAGIPPfOrderQuery* m_data;
};

// AGIPPfOrderQueryRes ------------------------------------------------------
typedef struct _AGIP_PF_ORDER_QUERY_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result;
    _AGIP_CHECKSUM_ checksum;
} SAGIPPfOrderQueryRes, *PSAGIPPfOrderQueryRes;

class Engine_Export AGIPPfOrderQueryRes :
    public SysProtocol
{
public:
    inline AGIPPfOrderQueryRes(void);
    inline ~AGIPPfOrderQueryRes(void);

    // getter ---------------------------------------------------------------
    inline int32_t getResult(void) const;

    // setter ---------------------------------------------------------------
    inline void setResult(int32_t nResult);

    inline virtual int showInfo(void);

private:
    SAGIPPfOrderQueryRes* m_data;
};

// AGIPPfOrderQuery ---------------------------------------------------------
AGIPPfOrderQuery::AGIPPfOrderQuery(void) : 
    SysProtocol(AGIP_CURRENT_VERSION, CMD_PF_ORDER_QUERY, sizeof(SAGIPPfOrderQuery))
{
    m_data = (SAGIPPfOrderQuery*)m_pucPDU;
}

AGIPPfOrderQuery::~AGIPPfOrderQuery(void)
{
}

// getter ---------------------------------------------------------------
const char* AGIPPfOrderQuery::getOrderID(char* buf, uint32_t buf_len) const
{
    if (buf_len > AGIP_ORDER_ID_LEN) {
        strncpy(buf, m_data->sz_Order_ID, AGIP_ORDER_ID_LEN);
        return buf;
    }
    return NULL;
}

uint16_t AGIPPfOrderQuery::getOrderType(void) const
{
    return ntohs(m_data->us_Order_Type);
}

// setter ---------------------------------------------------------------
void AGIPPfOrderQuery::setOrderID(const char* szOrderID)
{
    ASSERT(szOrderID != NULL);
    strncpy(m_data->sz_Order_ID, szOrderID, AGIP_ORDER_ID_LEN);
}

void AGIPPfOrderQuery::setOrderType(uint16_t usOrderType)
{
    m_data->us_Order_Type = htons(usOrderType);
}

int AGIPPfOrderQuery::showInfo(void)
{
    char buf[64] = {0};
    SysProtocol::showInfo();
    printf("------------------------------------------------------------AGIPPfOrderQuery\n");
    printf("Order_ID:           %s\n", getOrderID(buf, sizeof(buf)));
    printf("Order_Type:         %u\n", getOrderType());
    printf("------------------------------------------------------------AGIPPfOrderQuery\n");
    return S_SUCCESS;
}

// AGIPPfOrderQueryRes ------------------------------------------------------
AGIPPfOrderQueryRes::AGIPPfOrderQueryRes(void) : 
    SysProtocol(AGIP_CURRENT_VERSION, CMD_PF_ORDER_QUERY_RES, sizeof(SAGIPPfOrderQueryRes))
{
    m_data = (SAGIPPfOrderQueryRes*)m_pucPDU;
}

AGIPPfOrderQueryRes::~AGIPPfOrderQueryRes(void)
{
}

// getter ---------------------------------------------------------------
int32_t AGIPPfOrderQueryRes::getResult(void) const
{
    return ntohl(m_data->n_Result);
}


void AGIPPfOrderQueryRes::setResult(int32_t nResult)
{
    m_data->n_Result = htonl(nResult);
}

int AGIPPfOrderQueryRes::showInfo(void)
{
    SysProtocol::showInfo();
    printf("------------------------------------------------------------AGIPPfOrderQueryRes\n");
    printf("Result:   %d\n", getResult());
    printf("------------------------------------------------------------AGIPPfOrderQueryRes\n");
    return S_SUCCESS;
}

#endif /*__AGIP_PF_ORDER_QUERY_H__*/

