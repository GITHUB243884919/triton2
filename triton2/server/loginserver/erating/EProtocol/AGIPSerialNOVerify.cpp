#include "AGIPSerialNOVerify.h"


AGIPSerialNOVerify::AGIPSerialNOVerify(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_SERIALNO_VERIFY, sizeof(SAGIPSerialNOVerify))
{
}

AGIPSerialNOVerify::~AGIPSerialNOVerify(void)
{
}

int AGIPSerialNOVerify::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPSerialNOVerify *pSAGIPSerialNOVerify = (SAGIPSerialNOVerify *)m_pucPDU;
    *punUserID = htonl(pSAGIPSerialNOVerify->un_User_ID);
    return S_SUCCESS;
}

int AGIPSerialNOVerify::getSerialNO(char *strSerialNO)
{
    ASSERT(strSerialNO != NULL);
    SAGIPSerialNOVerify *pSAGIPSerialNOVerify = (SAGIPSerialNOVerify *)m_pucPDU;
    strncpy(strSerialNO, pSAGIPSerialNOVerify->str_Serial_NO, AGIP_SERIAL_NO_LEN);
    return S_SUCCESS;
}

int AGIPSerialNOVerify::getSerialType(int8_t *pcSerialType)
{
    ASSERT(pcSerialType != NULL);
    SAGIPSerialNOVerify *pSAGIPSerialNOVerify = (SAGIPSerialNOVerify *)m_pucPDU;
    *pcSerialType = pSAGIPSerialNOVerify->c_Serial_Type;
    return S_SUCCESS;
}

int AGIPSerialNOVerify::setUserID(uint32_t unUserID)
{
    SAGIPSerialNOVerify *pSAGIPSerialNOVerify = (SAGIPSerialNOVerify *)m_pucPDU;
    pSAGIPSerialNOVerify->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPSerialNOVerify::setSerialNO(const char *strSerialNO)
{
    ASSERT(strSerialNO != NULL);
    SAGIPSerialNOVerify *pSAGIPSerialNOVerify = (SAGIPSerialNOVerify *)m_pucPDU;
    strncpy(pSAGIPSerialNOVerify->str_Serial_NO, strSerialNO, AGIP_SERIAL_NO_LEN);
    return S_SUCCESS;
}

int AGIPSerialNOVerify::setSerialType(int8_t cSerialType)
{
    SAGIPSerialNOVerify *pSAGIPSerialNOVerify = (SAGIPSerialNOVerify *)m_pucPDU;
    pSAGIPSerialNOVerify->c_Serial_Type = cSerialType;
    return S_SUCCESS;
}

int AGIPSerialNOVerify::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t  unUserID      = 0;
    char      strSerialNO[AGIP_SERIAL_NO_LEN + 1];
    int8_t    cSerialType = 0;

    SysProtocol::showInfo();
    nRetCode = getUserID(&unUserID);
    nRetCode = getSerialNO(strSerialNO);
    strSerialNO[AGIP_SERIAL_NO_LEN] = '\0';
    nRetCode = getSerialType(&cSerialType);


    printf("--------------------------------------------------------AGIPSerialNOVerify\n");
    printf("User_ID:\t%u\n", unUserID);
    printf("SerialNO:\t%s\n", strSerialNO);
    switch(cSerialType)
    {
    case SERIALNO_TYPE_CDKEY:
        printf("Serial_Type:\t%d(CDKEY)\n", cSerialType);
        break;
    default:
        printf("Serial_Type:\t%d(Others)\n", cSerialType);
        break;
    }
    printf("--------------------------------------------------------AGIPSerialNOVerify\n");
    return S_SUCCESS;
}
/////////////////////////
// AGIPSerialNOVerifyRes
AGIPSerialNOVerifyRes::AGIPSerialNOVerifyRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_SERIALNO_VERIFY_RES, sizeof(SAGIPSerialNOVerifyRes))
{

}
AGIPSerialNOVerifyRes::~AGIPSerialNOVerifyRes(void)
{

}

int AGIPSerialNOVerifyRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPSerialNOVerifyRes *pSAGIPSerialNOVerifyRes = (SAGIPSerialNOVerifyRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPSerialNOVerifyRes->n_Result_Code);
    return S_SUCCESS;
}
int AGIPSerialNOVerifyRes::setResultCode(int32_t nResultCode)
{
    SAGIPSerialNOVerifyRes *pSAGIPSerialNOVerifyRes = (SAGIPSerialNOVerifyRes *)m_pucPDU;
    pSAGIPSerialNOVerifyRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPSerialNOVerifyRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);

    printf("--------------------------------------------------------AGIPSerialNOVerifyRes\n");
    printf("Result_Code:\t%d\n", nResultCode);

    printf("--------------------------------------------------------AGIPSerialNOVerifyRes\n");
    return S_SUCCESS;
}

