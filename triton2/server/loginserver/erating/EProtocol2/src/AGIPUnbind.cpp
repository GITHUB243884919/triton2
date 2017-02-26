#include "AGIPUnbind.h"

AGIPUnbind::AGIPUnbind(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_UNBIND, sizeof(SAGIPUnbind))
{
}

AGIPUnbind::~AGIPUnbind(void)
{
}


int AGIPUnbind::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t unGatewayID = 0;


    SysProtocol::showInfo();
    nRetCode = getGatewayID(&unGatewayID);

    printf("--------------------------------------------------------AGIPUnbind\n");
    printf("Gateway_ID:\t%u\n", unGatewayID);

    printf("--------------------------------------------------------AGIPUnbind\n");
    return S_SUCCESS;
}

//////////////////////////
/// AGIPUnbindRes

AGIPUnbindRes::AGIPUnbindRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_UNBIND_RES, sizeof(SAGIPUnbindRes))
{

}
AGIPUnbindRes::~AGIPUnbindRes(void)
{

}
int AGIPUnbindRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPUnbindRes *pSAGIPUnbindRes = (SAGIPUnbindRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPUnbindRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPUnbindRes::setResultCode(int32_t nResultCode)
{
    SAGIPUnbindRes *pSAGIPUnbindRes = (SAGIPUnbindRes *)m_pucPDU;
    pSAGIPUnbindRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPUnbindRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);

    printf("--------------------------------------------------------AGIPUnbindRes\n");
    printf("Result_Code:\t%d\n", nResultCode);

    printf("--------------------------------------------------------AGIPUnbindRes\n");

    return S_SUCCESS;
}

