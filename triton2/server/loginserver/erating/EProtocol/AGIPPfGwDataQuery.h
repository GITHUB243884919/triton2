#ifndef __AGIP_PF_GW_DATA_QUERY_H__
#define __AGIP_PF_GW_DATA_QUERY_H__

#include "SysProtocol.h"

#define CMD_PF_GW_DATA_QUERY           0x30002004
#define CMD_PF_GW_DATA_QUERY_RES       0x40002004

// AGIPPfGwDataQuery -------------------------------------------------------------
typedef struct _AGIP_PF_GW_DATA_QUERY_
{
    _AGIP_HEADER_   header;
    uint32_t        un_Game_ID;
    uint16_t        us_Data_Type;
    uint16_t        us_Pad_1;
    _AGIP_CHECKSUM_ checksum;
} SAGIPPfGwDataQuery, *PSAGIPPfGwDataQuery;

class Engine_Export AGIPPfGwDataQuery :
    public SysProtocol
{
public:
    AGIPPfGwDataQuery(void);
    virtual ~AGIPPfGwDataQuery(void);

public:
    uint32_t getGameID(void); 
    uint16_t getDataType(void);

    void setGameID(uint32_t unGameID); 
    void setDataType(uint16_t usDataType);

    int32_t showInfo();

private:
    SAGIPPfGwDataQuery* m_data;
};

// AGIPPfGwDataQueryRes -----------------------------------------------------
typedef struct _AGIP_PF_GW_DATA_QUERY_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    uint16_t        us_Gateway_Count;
    uint16_t        us_Pad_1;
    uint32_t        un_Gateway_ID;
    uint32_t        un_Data_Value;
    _AGIP_CHECKSUM_ checksum;
} SAGIPPfGwDataQueryRes, *PSAGIPPfGwDataQueryRes;

class Engine_Export AGIPPfGwDataQueryRes :
    public SysProtocol
{
public:
    AGIPPfGwDataQueryRes(void);
    virtual ~AGIPPfGwDataQueryRes(void);

public:
    int32_t getResult(void); 
    uint16_t getGatewayCount(void);
    int32_t getGatewayData(int32_t nIndex,
        uint32_t* punGatewayID,
        uint32_t* punDataValue);
    int32_t getGatewayData(int32_t nIndex,
        GW_DATA* pGatewayData);

    void setResult(int32_t nResult); 
    int32_t addGatewayData(uint32_t unGatewayID, uint32_t unDataValue);
    int32_t addGatewayData(const GW_DATA *pAllGatewayData);

    int32_t showInfo();

private:
    SAGIPPfGwDataQueryRes* m_data;
};


#endif

