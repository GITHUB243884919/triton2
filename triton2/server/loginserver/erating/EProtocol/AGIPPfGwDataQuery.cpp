#include "AGIPPfGwDataQuery.h"

#ifdef E_ERROR
#undef E_ERROR
#endif // E_ERROR
#define E_ERROR -1

AGIPPfGwDataQuery::AGIPPfGwDataQuery(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_PF_GW_DATA_QUERY, sizeof(SAGIPPfGwDataQuery))
{
    m_data = (SAGIPPfGwDataQuery*)m_pucPDU;
}

AGIPPfGwDataQuery::~AGIPPfGwDataQuery(void)
{
}

uint32_t AGIPPfGwDataQuery::getGameID(void)
{
    return ntohl(m_data->un_Game_ID);
} 

uint16_t AGIPPfGwDataQuery::getDataType(void)
{
    return ntohs(m_data->us_Data_Type);
}

void AGIPPfGwDataQuery::setGameID(uint32_t unGameID)
{
    m_data->un_Game_ID = htonl(unGameID);
} 

void AGIPPfGwDataQuery::setDataType(uint16_t usDataType)
{
    m_data->us_Data_Type = htons(usDataType);
}

int32_t AGIPPfGwDataQuery::showInfo()
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPPfGwDataQuery\n");
    printf("Game_ID:        %u\n", getGameID());
    printf("Data_Type:      %u\n", getDataType());
    printf("--------------------------------------------------------AGIPPfGwDataQuery\n");
    return S_SUCCESS;
}

////////////////////////////////////////////////
AGIPPfGwDataQueryRes::AGIPPfGwDataQueryRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_PF_GW_DATA_QUERY_RES, sizeof(SAGIPPfGwDataQueryRes) - sizeof(GW_DATA))
{
    m_data = (SAGIPPfGwDataQueryRes*)m_pucPDU;
}

AGIPPfGwDataQueryRes::~AGIPPfGwDataQueryRes(void)
{
}

int32_t AGIPPfGwDataQueryRes::getResult(void)
{
    return (int32_t)ntohl(m_data->n_Result_Code);
}

uint16_t AGIPPfGwDataQueryRes::getGatewayCount(void)
{
    return ntohs(m_data->us_Gateway_Count);
}

int32_t AGIPPfGwDataQueryRes::getGatewayData(int32_t nIndex,
    uint32_t* punGatewayID,
    uint32_t* punDataValue)
{
    ASSERT(punGatewayID != NULL && punDataValue != NULL);

    int32_t nResult = E_ERROR;
    uint16_t usGatewayCount = 0;
    GW_DATA *pAllGatewayDataBuffer = NULL;

    usGatewayCount = getGatewayCount();

    if ((nIndex < 0) || (nIndex >= (int32_t)usGatewayCount)) {
        nResult = E_PDU_INVALID_FIELD;
        goto ExitError;
    }
    pAllGatewayDataBuffer = (GW_DATA*)&m_data->un_Gateway_ID;
    pAllGatewayDataBuffer += nIndex;

    *punGatewayID = htonl(pAllGatewayDataBuffer->unGatewayID);
    *punDataValue = htonl(pAllGatewayDataBuffer->unDataValue);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int32_t AGIPPfGwDataQueryRes::getGatewayData(int32_t nIndex, GW_DATA *pAllGatewayData)
{
    ASSERT(pAllGatewayData != NULL);
    return this->getGatewayData(nIndex,
        &pAllGatewayData->unGatewayID,
        &pAllGatewayData->unDataValue);
}

void AGIPPfGwDataQueryRes::setResult(int32_t nResult)
{
    m_data->n_Result_Code = (int32_t)htonl(nResult);
}

int32_t AGIPPfGwDataQueryRes::addGatewayData(uint32_t unGatewayID, uint32_t unDataValue)
{
    int32_t nResult  = E_ERROR;
    uint16_t usGatewayCount = 0;
    GW_DATA *pAllGatewayDataBuffer = NULL;

    usGatewayCount = getGatewayCount();
    if (usGatewayCount >= AGIP_MAX_GW_DATA_CNT) {
        nResult = E_PDU_INVALID_FIELD;
        goto ExitError;
    }

    pAllGatewayDataBuffer = (GW_DATA*)&m_data->un_Gateway_ID;
    pAllGatewayDataBuffer += usGatewayCount;

    pAllGatewayDataBuffer->unGatewayID = htonl(unGatewayID);
    pAllGatewayDataBuffer->unDataValue = htonl(unDataValue);

    usGatewayCount++;
    m_data->us_Gateway_Count = htons(usGatewayCount);
    m_usTotalLength += sizeof(GW_DATA);
    setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int32_t AGIPPfGwDataQueryRes::addGatewayData(const GW_DATA *pAllGatewayData)
{
    ASSERT(pAllGatewayData != NULL);
    return this->addGatewayData(pAllGatewayData->unGatewayID,
        pAllGatewayData->unDataValue);
}

int32_t AGIPPfGwDataQueryRes::showInfo()
{
    GW_DATA gatewayData;
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPPfGwDataQueryRes\n");
    printf("Result_Code:        %d\n", getResult());
    printf("Gateway_Count:      %u\n", getGatewayCount());
    printf("Gateway_Data:       ");
    for (uint16_t i = 0; i < getGatewayCount(); i++) {
        getGatewayData((int32_t)i, &gatewayData);
        if (0 == i) {
            printf("[%03d] Gateway_ID = %u Data = %u\n",
                i,
                gatewayData.unGatewayID,
                gatewayData.unDataValue);
        } else {
            printf("                    [%03d] Gateway_ID = %u Data = %u\n",
                i,
                gatewayData.unGatewayID,
                gatewayData.unDataValue);
        }
    }
    printf("--------------------------------------------------------AGIPPfGwDataQueryRes\n");
    return S_SUCCESS;
}

