#include "AGIPBindYT.h"

AGIPBindYT::AGIPBindYT(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_BIND, sizeof(SAGIPBindYT))
{
    
}

AGIPBindYT::~AGIPBindYT(void)
{
}


int AGIPBindYT::getGatewayCode(char *strGatewayCode)
{
    ASSERT(strGatewayCode != NULL);
    SAGIPBindYT *pSAGIPBindYT = (SAGIPBindYT *)m_pucPDU;
    strncpy(strGatewayCode, pSAGIPBindYT->str_Gateway_Code, AGIP_GATEWAY_CODE_LEN);
    return S_SUCCESS;
}

int AGIPBindYT::getPassword(char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPBindYT *pSAGIPBindYT = (SAGIPBindYT *)m_pucPDU;
    strncpy(strPassword, pSAGIPBindYT->str_Password, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPBindYT::getMAC(char *strMAC)
{
    ASSERT(strMAC != NULL);
    SAGIPBindYT *pSAGIPBindYT = (SAGIPBindYT *)m_pucPDU;
    snprintf(
        strMAC,
        18,
        "%02X-%02X-%02X-%02X-%02X-%02X",
        pSAGIPBindYT->uc_MAC[0],
        pSAGIPBindYT->uc_MAC[1],
        pSAGIPBindYT->uc_MAC[2],
        pSAGIPBindYT->uc_MAC[3],
        pSAGIPBindYT->uc_MAC[4],
        pSAGIPBindYT->uc_MAC[5]
    );
    return S_SUCCESS;
}

int AGIPBindYT::getReconnectFlag(int8_t *pcReconnectFlag)
{
    ASSERT(pcReconnectFlag != NULL);
    SAGIPBindYT *pSAGIPBindYT = (SAGIPBindYT *)m_pucPDU;
    *pcReconnectFlag = pSAGIPBindYT->c_Reconnect_Flag;
    return S_SUCCESS;
}

int AGIPBindYT::getServerID(uint32_t *punServerID)
{
    ASSERT(punServerID != NULL);
    SAGIPBindYT *pSAGIPBindYT = (SAGIPBindYT *)m_pucPDU;
    *punServerID = htonl(pSAGIPBindYT->un_Server_ID);
    return S_SUCCESS;
}

/////////////////////////
// Setter
int AGIPBindYT::setGatewayCode(const char *pcGatewayCode)
{
    ASSERT(pcGatewayCode != NULL);
    SAGIPBindYT *pSAGIPBindYT = (SAGIPBindYT *)m_pucPDU;
    strncpy(pSAGIPBindYT->str_Gateway_Code, pcGatewayCode, AGIP_GATEWAY_CODE_LEN);
    return S_SUCCESS;
}

int AGIPBindYT::setPassword(const char *pcPassword)
{
    ASSERT(pcPassword != NULL);
    SAGIPBindYT *pSAGIPBindYT = (SAGIPBindYT *)m_pucPDU;
    strncpy(pSAGIPBindYT->str_Password, pcPassword, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPBindYT::setMAC(const uint8_t arrucMAC[6])
{
    ASSERT(arrucMAC != NULL);
    SAGIPBindYT *pSAGIPBindYT = (SAGIPBindYT *)m_pucPDU;
    memcpy(pSAGIPBindYT->uc_MAC, arrucMAC, 6);
    return S_SUCCESS;
}

int AGIPBindYT::setReconnectFlag(int8_t cReconnectFlag)
{
    SAGIPBindYT *pSAGIPBindYT = (SAGIPBindYT *)m_pucPDU;
    pSAGIPBindYT->c_Reconnect_Flag = cReconnectFlag;
    return S_SUCCESS;
}

int AGIPBindYT::setServerID(uint32_t unServerID)
{
    SAGIPBindYT *pSAGIPBindYT = (SAGIPBindYT *)m_pucPDU;
    pSAGIPBindYT->un_Server_ID = htonl(unServerID);
    return S_SUCCESS;
}

int AGIPBindYT::showInfo()
{
    int nRetCode        = E_ERROR;
    char strGatewayCode[AGIP_GATEWAY_CODE_LEN+1] = {0};
    char strPassword[AGIP_PASSWORD_LEN+1] = {0};
    char strMAC[18] = {0};
    int8_t cReconnectFlag = 0;
    uint32_t unServerID = 0;

    SysProtocol::showInfo();
    nRetCode = getGatewayCode(strGatewayCode);
    nRetCode = getPassword(strPassword);
    nRetCode = getMAC(strMAC);
    nRetCode = getReconnectFlag(&cReconnectFlag);
    nRetCode = getServerID(&unServerID);
    
    strGatewayCode[sizeof(strGatewayCode) - 1] = '\0';
    strPassword[sizeof(strPassword) - 1] = '\0';
    strMAC[sizeof(strMAC) - 1] = '\0';

    printf("--------------------------------------------------------AGIPBindYT\n");
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
    printf("--------------------------------------------------------AGIPBindYT\n");
    return S_SUCCESS;
}


//////////////////////////
/// AGIPBindYTRes
    
AGIPBindYTRes::AGIPBindYTRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_BIND_RES, sizeof(SAGIPBindYTRes))
{

}

AGIPBindYTRes::~AGIPBindYTRes(void)
{

}

int AGIPBindYTRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPBindYTRes *pSAGIPBindYTRes = (SAGIPBindYTRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPBindYTRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPBindYTRes::getGatewayID(uint32_t *punGatewayID)
{
    ASSERT(punGatewayID != NULL);
    SAGIPBindYTRes *pSAGIPBindYTRes = (SAGIPBindYTRes *)m_pucPDU;
    *punGatewayID = htonl(pSAGIPBindYTRes->un_Gateway_ID);
    return S_SUCCESS;
}

int AGIPBindYTRes::setResultCode(int32_t nResultCode)
{
    SAGIPBindYTRes *pSAGIPBindYTRes = (SAGIPBindYTRes *)m_pucPDU;
    pSAGIPBindYTRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPBindYTRes::setGatewayID(uint32_t unGatewayID)
{
    SAGIPBindYTRes *pSAGIPBindYTRes = (SAGIPBindYTRes *)m_pucPDU;
    pSAGIPBindYTRes->un_Gateway_ID = htonl(unGatewayID);
    return S_SUCCESS;
}

int AGIPBindYTRes::showInfo()
{
    int nRetCode        = E_ERROR;
    
    int32_t  nResultCode = 0;
    uint32_t unGatewayID = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);
    nRetCode = getGatewayID(&unGatewayID);

    printf("--------------------------------------------------------AGIPBindYTRes\n");
    printf("Result_Code:        %d\n", nResultCode);
    printf("Gateway_ID:         %u\n",  unGatewayID);
    printf("--------------------------------------------------------AGIPBindYTRes\n");
    return S_SUCCESS;
}

