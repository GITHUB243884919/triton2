#include "AGIPBind.h"

AGIPBind::AGIPBind(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_BIND, sizeof(SAGIPBind))
{
    
}

AGIPBind::~AGIPBind(void)
{
}


int AGIPBind::getGatewayCode(char *strGatewayCode)
{
    ASSERT(strGatewayCode != NULL);
    SAGIPBind *pSAGIPBind = (SAGIPBind *)m_pucPDU;
    strncpy(strGatewayCode, pSAGIPBind->str_Gateway_Code, AGIP_GATEWAY_CODE_LEN);
    return S_SUCCESS;
}

int AGIPBind::getPassword(char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPBind *pSAGIPBind = (SAGIPBind *)m_pucPDU;
    strncpy(strPassword, pSAGIPBind->str_Password, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

const char* AGIPBind::getMAC(char *strMAC, size_t unMACSize)
{
    ASSERT(strMAC != NULL);

    if (unMACSize <= AGIP_MAC_LEN)
    {
        return NULL;
    }

    memcpy(strMAC, ((SAGIPBind *)m_pucPDU)->str_MAC, AGIP_MAC_LEN);
    strMAC[AGIP_MAC_LEN] = 0;

    return strMAC;
}

int AGIPBind::getReconnectFlag(int8_t *pcReconnectFlag)
{
    ASSERT(pcReconnectFlag != NULL);
    SAGIPBind *pSAGIPBind = (SAGIPBind *)m_pucPDU;
    *pcReconnectFlag = pSAGIPBind->c_Reconnect_Flag;
    return S_SUCCESS;
}

int AGIPBind::getServerID(uint32_t *punServerID)
{
    ASSERT(punServerID != NULL);
    SAGIPBind *pSAGIPBind = (SAGIPBind *)m_pucPDU;
    *punServerID = htonl(pSAGIPBind->un_Server_ID);
    return S_SUCCESS;
}

/////////////////////////
// Setter
int AGIPBind::setGatewayCode(const char *pcGatewayCode)
{
    ASSERT(pcGatewayCode != NULL);
    SAGIPBind *pSAGIPBind = (SAGIPBind *)m_pucPDU;
    strncpy(pSAGIPBind->str_Gateway_Code, pcGatewayCode, AGIP_GATEWAY_CODE_LEN);
    return S_SUCCESS;
}

int AGIPBind::setPassword(const char *pcPassword)
{
    ASSERT(pcPassword != NULL);
    SAGIPBind *pSAGIPBind = (SAGIPBind *)m_pucPDU;
    strncpy(pSAGIPBind->str_Password, pcPassword, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPBind::setMAC(const char* szMAC)
{
    ASSERT(szMAC != NULL);
    memcpy(((SAGIPBind *)m_pucPDU)->str_MAC, szMAC, AGIP_MAC_LEN);
    return S_SUCCESS;
}

int AGIPBind::setReconnectFlag(int8_t cReconnectFlag)
{
    SAGIPBind *pSAGIPBind = (SAGIPBind *)m_pucPDU;
    pSAGIPBind->c_Reconnect_Flag = cReconnectFlag;
    return S_SUCCESS;
}

int AGIPBind::setServerID(uint32_t unServerID)
{
    SAGIPBind *pSAGIPBind = (SAGIPBind *)m_pucPDU;
    pSAGIPBind->un_Server_ID = htonl(unServerID);
    return S_SUCCESS;
}

int AGIPBind::showInfo()
{
    int nRetCode        = E_ERROR;
    char strGatewayCode[AGIP_GATEWAY_CODE_LEN+1] = {0};
    char strPassword[AGIP_PASSWORD_LEN+1] = {0};
    char strMAC[AGIP_MAC_LEN+1] = {0};
    int8_t cReconnectFlag = 0;
    uint32_t unServerID = 0;

    SysProtocol::showInfo();
    nRetCode = getGatewayCode(strGatewayCode);
    nRetCode = getPassword(strPassword);
    getMAC(strMAC, sizeof(strMAC));
    nRetCode = getReconnectFlag(&cReconnectFlag);
    nRetCode = getServerID(&unServerID);
    
    strGatewayCode[sizeof(strGatewayCode) - 1] = '\0';
    strPassword[sizeof(strPassword) - 1] = '\0';
    strMAC[sizeof(strMAC) - 1] = '\0';

    printf("--------------------------------------------------------AGIPBind\n");
    printf("Gateway_Code:       %s\n", strGatewayCode);
    printf("Password:           %s\n", strPassword);
    printf("MAC:                %s\n", strMAC);
    switch(cReconnectFlag )
    {
    case RECONNECT_FLAG_FIRST_CONNECTION:
        printf("Reconnect_Flag:     %d(First Connection)\n", cReconnectFlag);
        break;
    case RECONNECT_FLAG_RECONNECTION:
        printf("Reconnect_Flag:     %d(Reconnection)\n", cReconnectFlag);
        break;
    default:
        printf("Reconnect_Flag:     %d(INVALID)\n", cReconnectFlag);
        break;
    }
    printf("Server_ID:          %u\n", unServerID);
    printf("--------------------------------------------------------AGIPBind\n");
    return S_SUCCESS;
}


//////////////////////////
/// AGIPBindRes
    
AGIPBindRes::AGIPBindRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_BIND_RES, sizeof(SAGIPBindRes))
{

}

AGIPBindRes::~AGIPBindRes(void)
{

}

int AGIPBindRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPBindRes *pSAGIPBindRes = (SAGIPBindRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPBindRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPBindRes::setResultCode(int32_t nResultCode)
{
    SAGIPBindRes *pSAGIPBindRes = (SAGIPBindRes *)m_pucPDU;
    pSAGIPBindRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPBindRes::showInfo()
{
    int nRetCode        = E_ERROR;
    
    int32_t  nResultCode = 0;
    uint32_t unGatewayID = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);
    nRetCode = getGatewayID(&unGatewayID);

    printf("--------------------------------------------------------AGIPBindRes\n");
    printf("Result_Code:        %d\n", nResultCode);
    printf("Gateway_ID:         %u\n", unGatewayID);
    printf("--------------------------------------------------------AGIPBindRes\n");
    return S_SUCCESS;
}

