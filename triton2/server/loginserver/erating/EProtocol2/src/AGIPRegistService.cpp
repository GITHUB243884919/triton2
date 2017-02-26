#include "AGIPRegistService.h"
#include <time.h>
AGIPRegistService::AGIPRegistService(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_REGIST_SERVICE, sizeof(SAGIPRegistService))
{
    m_data = (PSAGIPRegistService)m_pucPDU;
}

AGIPRegistService::~AGIPRegistService(void)
{
}

uint32_t AGIPRegistService::getCommandID(void)
{
    return ntohl(m_data->un_Command_ID);
}

void AGIPRegistService::setCommandID(uint32_t unCommandID)
{
    m_data->un_Command_ID = htonl(unCommandID);
}

uint32_t AGIPRegistService::getVersion(void)
{
    return ntohl(m_data->un_Version);
}

void AGIPRegistService::setVersion(uint32_t unVersion)
{
    m_data->un_Version = htonl(unVersion);
}

uint32_t AGIPRegistService::getStep(void)
{
    return ntohl(m_data->un_Step);
}

void AGIPRegistService::setStep(uint32_t unStep)
{
    m_data->un_Step = htonl(unStep);
}

//////////////////////////////////////////////////////////////////////////
int AGIPRegistService::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPRegistService\n");
    printf("Command_ID  :           0x%08x\n", getCommandID());
    printf("Version     :           0x%08x\n", getVersion());
    printf("Step        :           %u\n",     getStep());
    printf("--------------------------------------------------------AGIPRegistService\n");

    return S_SUCCESS;
}


////////////////////////
// AGIPRegistServiceRes
AGIPRegistServiceRes::AGIPRegistServiceRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_REGIST_SERVICE_RES, sizeof(SAGIPRegistServiceRes))
{
    m_data = (PSAGIPRegistServiceRes)m_pucPDU;
}

AGIPRegistServiceRes::~AGIPRegistServiceRes()
{

}

int32_t AGIPRegistServiceRes::getResultCode(void)
{
    return (int32_t)(ntohl(m_data->n_Result_Code));
}

void  AGIPRegistServiceRes::setResultCode(int32_t nResultCode)
{
    m_data->n_Result_Code = (int32_t)(htonl(nResultCode));
}

int AGIPRegistServiceRes::showInfo()
{
    SysProtocol::showInfo();

    printf("--------------------------------------------------------AGIPRegistServiceRes\n");
    printf("Result_Code:            %d\n", getResultCode());
    printf("--------------------------------------------------------AGIPRegistServiceRes\n");

    return S_SUCCESS;
}


