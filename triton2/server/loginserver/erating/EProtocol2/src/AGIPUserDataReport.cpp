#include "AGIPUserDataReport.h"

AGIPUserDataReport::AGIPUserDataReport(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_DATA_REPORT, sizeof(SAGIPUserDataReport) - sizeof(DATA_INFO))
{
}

AGIPUserDataReport::~AGIPUserDataReport(void)
{
}

int AGIPUserDataReport::initialize()
{
    int nRetCode = E_ERROR;

    m_usTotalLength = sizeof(SAGIPUserDataReport);
    nRetCode = SysProtocol::initialize();
    ASSERT(nRetCode == S_SUCCESS);

    return S_SUCCESS;
}

int AGIPUserDataReport::getServerID(uint32_t *punServerID)
{
    ASSERT(punServerID != NULL);
    SAGIPUserDataReport *pSAGIPUserDataReport = (SAGIPUserDataReport *)m_pucPDU;
    *punServerID = htonl(pSAGIPUserDataReport->un_Server_ID);
    return S_SUCCESS;
}

int AGIPUserDataReport::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPUserDataReport *pSAGIPUserDataReport = (SAGIPUserDataReport *)m_pucPDU;
    *punUserID = htonl(pSAGIPUserDataReport->un_User_ID);
    return S_SUCCESS;
}

int AGIPUserDataReport::getRoleID(uint32_t *punRoleID)
{
    ASSERT(punRoleID != NULL);
    SAGIPUserDataReport *pSAGIPUserDataReport = (SAGIPUserDataReport *)m_pucPDU;
    *punRoleID = htonl(pSAGIPUserDataReport->un_Role_ID);
    return S_SUCCESS;
}

int AGIPUserDataReport::getDataCount(int32_t *pnDataCount)
{
    ASSERT(pnDataCount != NULL);
    SAGIPUserDataReport *pSAGIPUserDataReport = (SAGIPUserDataReport *)m_pucPDU;
    *pnDataCount = htonl(pSAGIPUserDataReport->n_Data_Count);
    return S_SUCCESS;
}

int AGIPUserDataReport::getDataInfo(int32_t nIndex, int32_t *pnDataType, int32_t *pnDataValue)
{
    int nResult  = E_PDU_INVALID_FIELD;

    DATA_INFO *pDataInfoBuffer = NULL;
    int32_t nDataCount = 0;

    ASSERT(pnDataType != NULL && pnDataValue != NULL);
    SAGIPUserDataReport *pSAGIPUserDataReport = (SAGIPUserDataReport *)m_pucPDU;
    nDataCount = htonl(pSAGIPUserDataReport->n_Data_Count);
    if (
        (nIndex < 0) ||
        (nIndex >= nDataCount) ||
        (nIndex >= AGIP_MAX_USER_DATA_CNT)
        )
    {
        goto ExitError;
    }
    pDataInfoBuffer = (DATA_INFO *)&pSAGIPUserDataReport->n_Data_Type;
    pDataInfoBuffer += nIndex;

    *pnDataType  = htonl(pDataInfoBuffer->nDataType);
    *pnDataValue = htonl(pDataInfoBuffer->nDataValue);

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int AGIPUserDataReport::getDataInfo(int32_t nIndex, DATA_INFO *pDataInfo)
{
    ASSERT(pDataInfo != NULL);
    return this->getDataInfo(nIndex, &pDataInfo->nDataType, &pDataInfo->nDataValue);
}

int AGIPUserDataReport::setServerID(uint32_t unServerID)
{
    SAGIPUserDataReport *pSAGIPUserDataReport = (SAGIPUserDataReport *)m_pucPDU;
    pSAGIPUserDataReport->un_Server_ID = htonl(unServerID);
    return S_SUCCESS;
}

int AGIPUserDataReport::setUserID(uint32_t unUserID)
{
    SAGIPUserDataReport *pSAGIPUserDataReport = (SAGIPUserDataReport *)m_pucPDU;
    pSAGIPUserDataReport->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPUserDataReport::setRoleID(uint32_t unRoleID)
{
    SAGIPUserDataReport *pSAGIPUserDataReport = (SAGIPUserDataReport *)m_pucPDU;
    pSAGIPUserDataReport->un_Role_ID = htonl(unRoleID);
    return S_SUCCESS;
}

int AGIPUserDataReport::setDataCount(int32_t nDataCount)
{
    ASSERT(false && "Data Count is calculated automatically. Pls use addDataInfo() method!!");
    nDataCount = 0;
    return E_ERROR;
}

int AGIPUserDataReport::addDataInfo(int32_t nDataType, int32_t nDataValue)
{
    int nResult  = E_PDU_INVALID_FIELD;
    // int nRetCode = E_PDU_INVALID_FIELD;

    DATA_INFO *pDataInfoBuffer = NULL;
    int32_t nDataCount = 0;

    SAGIPUserDataReport *pSAGIPUserDataReport = (SAGIPUserDataReport *)m_pucPDU;
    nDataCount = htonl(pSAGIPUserDataReport->n_Data_Count);

    if (nDataCount >= AGIP_MAX_USER_DATA_CNT)
    {
        return E_PARAMETER_ERROR;
    }

    pDataInfoBuffer = (DATA_INFO *)&pSAGIPUserDataReport->n_Data_Type;
    pDataInfoBuffer += nDataCount;

    pDataInfoBuffer->nDataType  = htonl(nDataType);
    pDataInfoBuffer->nDataValue = htonl(nDataValue);

    nDataCount++;
    pSAGIPUserDataReport->n_Data_Count = htonl(nDataCount);
    m_usTotalLength += sizeof(DATA_INFO);
    this->setTotalLength(m_usTotalLength);

    nResult = S_SUCCESS;
    //ExitError:
    return nResult;
}

int AGIPUserDataReport::addDataInfo(const DATA_INFO *pDataInfo)
{
    ASSERT(pDataInfo != NULL);
    return this->addDataInfo(pDataInfo->nDataType, pDataInfo->nDataValue);
}

int AGIPUserDataReport::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t  unGatewayID = 0;
    uint32_t  unServerID  = 0;
    uint32_t  unUserID    = 0;
    uint32_t  unRoleID    = 0;
    int32_t   nDataCount  = 0;
    int32_t   nDataType   = 0;
    int32_t   nDataValue  = 0;

    SysProtocol::showInfo();
    nRetCode = getGatewayID(&unGatewayID);
    nRetCode = getServerID(&unServerID);
    nRetCode = getUserID(&unUserID);
    nRetCode = getRoleID(&unRoleID);
    nRetCode = getDataCount(&nDataCount);
    printf("--------------------------------------------------------AGIPUserDataReport\n");
    printf("Gateway_ID: %u\n", unGatewayID);
    printf("Server_ID : %u\n", unServerID);
    printf("User_ID   : %u\n", unUserID);
    printf("Role_ID   : %u\n", unRoleID);
    printf("Data_Count: %d\n", nDataCount);
    for (int i = 0; i < nDataCount; i++)
    {
        nRetCode = getDataInfo(i, &nDataType, &nDataValue);
        printf("[%d]Data_Type:\t%d, \t%d\n", i, nDataType, nDataValue);
    }

    printf("--------------------------------------------------------AGIPUserDataReport\n");
    return S_SUCCESS;

}
///////////////////////
//AGIPUserDataReportRes
AGIPUserDataReportRes::AGIPUserDataReportRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_DATA_REPORT_RES, sizeof(SAGIPUserDataReportRes))
{

}

AGIPUserDataReportRes::~AGIPUserDataReportRes(void)
{

}

int AGIPUserDataReportRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUserDataReportRes *pSAGIPUserDataReportRes = (SAGIPUserDataReportRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUserDataReportRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUserDataReportRes::setResultCode(int32_t nResultCode)
{
    SAGIPUserDataReportRes *pSAGIPUserDataReportRes = (SAGIPUserDataReportRes *)m_pucPDU;
    pSAGIPUserDataReportRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPUserDataReportRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);

    printf("--------------------------------------------------------AGIPUserDataReportRes\n");
    printf("Result_Code:\t%d\n", nResultCode);

    printf("--------------------------------------------------------AGIPUserDataReportRes\n");
    return S_SUCCESS;
}

