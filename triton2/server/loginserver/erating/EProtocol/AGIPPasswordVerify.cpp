#include "AGIPPasswordVerify.h"

AGIPPasswordVerify::AGIPPasswordVerify(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_PASSWORD_VERIFY, sizeof(SAGIPPasswordVerify))
{
}

AGIPPasswordVerify::~AGIPPasswordVerify(void)
{
}

int AGIPPasswordVerify::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPPasswordVerify *pSAGIPPasswordVerify = (SAGIPPasswordVerify *)m_pucPDU;
    *punUserID = htonl(pSAGIPPasswordVerify->un_User_ID);
    return S_SUCCESS;
}

int AGIPPasswordVerify::getPassword(char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPPasswordVerify *pSAGIPPasswordVerify = (SAGIPPasswordVerify *)m_pucPDU;
    strncpy(strPassword, pSAGIPPasswordVerify->str_Password, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPPasswordVerify::getPasswordType(int8_t *pcPasswordType)
{
    ASSERT(pcPasswordType != NULL);
    SAGIPPasswordVerify *pSAGIPPasswordVerify = (SAGIPPasswordVerify *)m_pucPDU;
    *pcPasswordType = pSAGIPPasswordVerify->c_Password_Type;
    return S_SUCCESS;
}

int AGIPPasswordVerify::setUserID(uint32_t unUserID)
{
    SAGIPPasswordVerify *pSAGIPPasswordVerify = (SAGIPPasswordVerify *)m_pucPDU;
    pSAGIPPasswordVerify->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPPasswordVerify::setPassword(const char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPPasswordVerify *pSAGIPPasswordVerify = (SAGIPPasswordVerify *)m_pucPDU;
    strncpy(pSAGIPPasswordVerify->str_Password, strPassword, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPPasswordVerify::setPasswordType(int8_t cPasswordType)
{
    SAGIPPasswordVerify *pSAGIPPasswordVerify = (SAGIPPasswordVerify *)m_pucPDU;
    pSAGIPPasswordVerify->c_Password_Type = cPasswordType;
    return S_SUCCESS;
}

int AGIPPasswordVerify::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t  unUserID      = 0;
    char      strPassword[AGIP_PASSWORD_LEN + 1];
    int8_t    cPasswordType = 0;

    SysProtocol::showInfo();
    nRetCode = getUserID(&unUserID);
    nRetCode = getPassword(strPassword);
    strPassword[AGIP_PASSWORD_LEN] = '\0';
    nRetCode = getPasswordType(&cPasswordType);


    printf("--------------------------------------------------------AGIPPasswordVerify\n");
    printf("User_ID:\t%u\n", unUserID);
    printf("Password:\t%s\n", strPassword);
    switch(cPasswordType)
    {
    case PASSWORD_TYPE_AUTHENTICATION:
        printf("Password_Type:\t%d(Authentication)\n", cPasswordType);
        break;
    case PASSWORD_TYPE_SECURITY:
        printf("Password_Type:\t%d(Security Code Verify)\n", cPasswordType);
        break;
    default:
        printf("Password_Type:\t%d(Others)\n", cPasswordType);
        break;
    }
    printf("--------------------------------------------------------AGIPPasswordVerify\n");
    return S_SUCCESS;
}
////////////////////
//AGIPPasswordVerifyRes

AGIPPasswordVerifyRes::AGIPPasswordVerifyRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_PASSWORD_VERIFY_RES, sizeof(SAGIPPasswordVerifyRes))
{

}

AGIPPasswordVerifyRes::~AGIPPasswordVerifyRes(void)
{

}

int AGIPPasswordVerifyRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPPasswordVerifyRes *pSAGIPPasswordVerifyRes = (SAGIPPasswordVerifyRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPPasswordVerifyRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPPasswordVerifyRes::setResultCode(int32_t nResultCode)
{
    SAGIPPasswordVerifyRes *pSAGIPPasswordVerifyRes = (SAGIPPasswordVerifyRes *)m_pucPDU;
    pSAGIPPasswordVerifyRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPPasswordVerifyRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);

    printf("--------------------------------------------------------AGIPPasswordVerifyRes\n");
    printf("Result_Code:\t%d\n", nResultCode);

    printf("--------------------------------------------------------AGIPPasswordVerifyRes\n");
    return S_SUCCESS;
}
