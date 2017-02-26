#include "AGIPGWDataReportYT.h"

AGIPGWDataReportYT::AGIPGWDataReportYT(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_GW_DATA_REPORT, sizeof(SAGIPGWDataReportYT) - sizeof(DATA_INFO))
{
}

AGIPGWDataReportYT::~AGIPGWDataReportYT(void)
{
}

int AGIPGWDataReportYT::initialize()
{
    int nRetCode = E_ERROR;

    m_usTotalLength = sizeof(SAGIPGWDataReportYT) - sizeof(DATA_INFO);
    nRetCode = SysProtocol::initialize();
    ASSERT(nRetCode == S_SUCCESS);

    return S_SUCCESS;
}

int AGIPGWDataReportYT::getGameID(uint16_t *pusGameID)
{
    ASSERT(pusGameID != NULL);
    *pusGameID = 0;
    return S_SUCCESS;
}

int AGIPGWDataReportYT::getGatewayID(uint32_t *punGatewayID)
{
    ASSERT(punGatewayID != NULL);
    SAGIPGWDataReportYT *pSAGIPGWDataReportYT = (SAGIPGWDataReportYT *)m_pucPDU;
    *punGatewayID = htonl(pSAGIPGWDataReportYT->un_Gateway_ID);
    return S_SUCCESS;
}

int AGIPGWDataReportYT::getServerID(uint32_t *punServerID)
{
    ASSERT(punServerID != NULL);
    SAGIPGWDataReportYT *pSAGIPGWDataReportYT = (SAGIPGWDataReportYT *)m_pucPDU;
    *punServerID = htonl(pSAGIPGWDataReportYT->un_Server_ID);
    return S_SUCCESS;
}

int AGIPGWDataReportYT::getDataCount(int32_t *pnDataCount)
{
    ASSERT(pnDataCount != NULL);
    SAGIPGWDataReportYT *pSAGIPGWDataReportYT = (SAGIPGWDataReportYT *)m_pucPDU;
    *pnDataCount = htonl(pSAGIPGWDataReportYT->n_Data_Count);
    return S_SUCCESS;
}

int AGIPGWDataReportYT::getDataInfo(int32_t nIndex, int32_t *pnDataType, int32_t *pnDataValue)
{
    ASSERT(pnDataType != NULL && pnDataValue != NULL);

    int nResult  = E_PDU_INVALID_FIELD;

    DATA_INFO *pDataInfoBuffer = NULL;
    int32_t nDataCount = 0;

    SAGIPGWDataReportYT *pSAGIPGWDataReportYT = (SAGIPGWDataReportYT *)m_pucPDU;
    nDataCount = ntohl(pSAGIPGWDataReportYT->n_Data_Count);
    if (
        (nIndex < 0) ||
        (nIndex >= nDataCount)
        )
    {
        goto ExitError;
    }
    pDataInfoBuffer = (DATA_INFO *)&pSAGIPGWDataReportYT->n_Data_Type;
    pDataInfoBuffer += nIndex;

    *pnDataType  = ntohl(pDataInfoBuffer->nDataType);
    *pnDataValue = ntohl(pDataInfoBuffer->nDataValue);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPGWDataReportYT::getDataInfo(int32_t nIndex, DATA_INFO *pDataInfo)
{
    ASSERT(pDataInfo != NULL);
    return this->getDataInfo(nIndex, &pDataInfo->nDataType, &pDataInfo->nDataValue);
}

int AGIPGWDataReportYT::setGameID(uint16_t usGameID)
{
    usGameID = 0;
    return S_SUCCESS;
}

int AGIPGWDataReportYT::setGatewayID(uint32_t unGatewayID)
{
    SAGIPGWDataReportYT *pSAGIPGWDataReportYT = (SAGIPGWDataReportYT *)m_pucPDU;
    pSAGIPGWDataReportYT->un_Gateway_ID = htonl(unGatewayID);
    return S_SUCCESS;
}

int AGIPGWDataReportYT::setServerID(uint32_t unServerID)
{
    SAGIPGWDataReportYT *pSAGIPGWDataReportYT = (SAGIPGWDataReportYT *)m_pucPDU;
    pSAGIPGWDataReportYT->un_Server_ID = htonl(unServerID);
    return S_SUCCESS;
}

int AGIPGWDataReportYT::setDataCount(int32_t nDataCount)
{
    ASSERT(false && "Data Count is calculated automatically. Pls use addDataInfo() method!!");
    nDataCount = 0;
    return E_ERROR;
}

int AGIPGWDataReportYT::addDataInfo(int32_t nDataType, int32_t nDataValue)
{
    int nResult  = E_PDU_INVALID_FIELD;

    DATA_INFO *pDataInfoBuffer = NULL;
    int32_t nDataCount = 0;

    SAGIPGWDataReportYT *pSAGIPGWDataReportYT = (SAGIPGWDataReportYT *)m_pucPDU;
    nDataCount = htonl(pSAGIPGWDataReportYT->n_Data_Count);

    pDataInfoBuffer = (DATA_INFO *)&(pSAGIPGWDataReportYT->n_Data_Type);
    pDataInfoBuffer += nDataCount;

    pDataInfoBuffer->nDataType  = htonl(nDataType);
    pDataInfoBuffer->nDataValue = htonl(nDataValue);

    nDataCount++;
    pSAGIPGWDataReportYT->n_Data_Count = htonl(nDataCount);
    
    m_usTotalLength += sizeof(DATA_INFO);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
    return nResult;
}

int AGIPGWDataReportYT::addDataInfo(const DATA_INFO *pDataInfo)
{
    ASSERT(pDataInfo != NULL);
    return this->addDataInfo(pDataInfo->nDataType, pDataInfo->nDataValue);
}

int AGIPGWDataReportYT::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t  unGatewayID = 0;
    uint32_t  unServerID  = 0;
    int32_t   nDataCount  = 0;
    int32_t   nDataType   = 0;
    int32_t   nDataValue  = 0;

    SysProtocol::showInfo();
    nRetCode = getGatewayID(&unGatewayID);
    nRetCode = getServerID(&unServerID);
    nRetCode = getDataCount(&nDataCount);
    printf("--------------------------------------------------------AGIPGWDataReportYT\n");
    printf("Gateway_ID: %u\n", unGatewayID);
    printf("Server_ID : %u\n", unServerID);
    printf("Data_Count: %d\n", nDataCount);
    for (int i = 0; i < nDataCount; i++)
    {
        nRetCode = getDataInfo(i, &nDataType, &nDataValue);
        printf("[%d]Data_Type:\t%d, \t%d\n", i, nDataType, nDataValue);
    }

    printf("--------------------------------------------------------AGIPGWDataReportYT\n");
    return S_SUCCESS;

}
///////////////////////
//AGIPGWDataReportYTRes
AGIPGWDataReportYTRes::AGIPGWDataReportYTRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_GW_DATA_REPORT_RES, sizeof(SAGIPGWDataReportYTRes))
{

}

AGIPGWDataReportYTRes::~AGIPGWDataReportYTRes(void)
{

}

int AGIPGWDataReportYTRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPGWDataReportYTRes *pSAGIPGWDataReportYTRes = (SAGIPGWDataReportYTRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPGWDataReportYTRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPGWDataReportYTRes::setResultCode(int32_t nResultCode)
{
    SAGIPGWDataReportYTRes *pSAGIPGWDataReportYTRes = (SAGIPGWDataReportYTRes *)m_pucPDU;
    pSAGIPGWDataReportYTRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPGWDataReportYTRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);

    printf("--------------------------------------------------------AGIPGWDataReportYTRes\n");
    printf("Result_Code:\t%d\n", nResultCode);

    printf("--------------------------------------------------------AGIPGWDataReportYTRes\n");
    return S_SUCCESS;
}

