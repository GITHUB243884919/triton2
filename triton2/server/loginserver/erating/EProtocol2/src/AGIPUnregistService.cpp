#include "AGIPUnregistService.h"
#include <time.h>
AGIPUnregistService::AGIPUnregistService(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_UNREGIST_SERVICE, sizeof(SAGIPUnregistService))
{
    m_data = (PSAGIPUnregistService)m_pucPDU;
}

AGIPUnregistService::~AGIPUnregistService(void)
{
}

uint32_t AGIPUnregistService::getCommandID(void)
{
    return ntohl(m_data->un_Command_ID);
}

void AGIPUnregistService::setCommandID(uint32_t unCommandID)
{
    m_data->un_Command_ID = htonl(unCommandID);
}

uint32_t AGIPUnregistService::getVersion(void)
{
    return ntohl(m_data->un_Version);
}

void AGIPUnregistService::setVersion(uint32_t unVersion)
{
    m_data->un_Version = htonl(unVersion);
}

uint32_t AGIPUnregistService::getStep(void)
{
    return ntohl(m_data->un_Step);
}

void AGIPUnregistService::setStep(uint32_t unStep)
{
    m_data->un_Step = htonl(unStep);
}

//////////////////////////////////////////////////////////////////////////
int AGIPUnregistService::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPUnregistService\n");
    printf("Command_ID  :           0x%08x\n", getCommandID());
    printf("Version     :           0x%08x\n", getVersion());
    printf("Step        :           %u\n",     getStep());
    printf("GameID      :           %u\n",     getGameID());
    printf("Gateway_ID  :           %u\n",     getGatewayID());
    printf("--------------------------------------------------------AGIPUnregistService\n");

    return S_SUCCESS;
}


////////////////////////
// AGIPUnregistServiceRes
AGIPUnregistServiceRes::AGIPUnregistServiceRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_UNREGIST_SERVICE_RES, sizeof(SAGIPUnregistServiceRes))
{
    m_data = (PSAGIPUnregistServiceRes)m_pucPDU;
}

AGIPUnregistServiceRes::~AGIPUnregistServiceRes()
{

}

int32_t AGIPUnregistServiceRes::getResultCode(void)
{
    return (int32_t)(ntohl(m_data->n_Result_Code));
}

void  AGIPUnregistServiceRes::setResultCode(int32_t nResultCode)
{
    m_data->n_Result_Code = (int32_t)(htonl(nResultCode));
}

int AGIPUnregistServiceRes::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPUnregistServiceRes\n");
    printf("Result_Code:            %d\n", getResultCode());
    printf("--------------------------------------------------------AGIPUnregistServiceRes\n");

    return S_SUCCESS;
}


