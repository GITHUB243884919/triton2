#include "AGIPPasswordChange.h"

AGIPPasswordChange::AGIPPasswordChange(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_PASSWORD_CHANGE, sizeof(SAGIPPasswordChange))
{
}

AGIPPasswordChange::~AGIPPasswordChange(void)
{
}

int AGIPPasswordChange::getUserID(uint32_t *punUserID)
{
    ASSERT(punUserID != NULL);
    SAGIPPasswordChange *pSAGIPPasswordChange = (SAGIPPasswordChange *)m_pucPDU;
    *punUserID = htonl(pSAGIPPasswordChange->un_User_ID);
    return S_SUCCESS;
}

int AGIPPasswordChange::getVerifyFlag(int8_t *pcVerifyFlag)
{
    ASSERT(pcVerifyFlag != NULL);
    SAGIPPasswordChange *pSAGIPPasswordChange = (SAGIPPasswordChange *)m_pucPDU;
    *pcVerifyFlag = pSAGIPPasswordChange->c_Verify_Flag;
    return S_SUCCESS;
}

int AGIPPasswordChange::getPasswordType1(int8_t *pcPasswordType)
{
    ASSERT(pcPasswordType != NULL);
    SAGIPPasswordChange *pSAGIPPasswordChange = (SAGIPPasswordChange *)m_pucPDU;
    *pcPasswordType = pSAGIPPasswordChange->c_Password_Type1;
    return S_SUCCESS;
}

int AGIPPasswordChange::getPasswordType2(int8_t *pcPasswordType)
{
    ASSERT(pcPasswordType != NULL);
    SAGIPPasswordChange *pSAGIPPasswordChange = (SAGIPPasswordChange *)m_pucPDU;
    *pcPasswordType = pSAGIPPasswordChange->c_Password_Type2;
    return S_SUCCESS;
}

int AGIPPasswordChange::getPassword1(char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPPasswordChange *pSAGIPPasswordChange = (SAGIPPasswordChange *)m_pucPDU;
    strncpy(strPassword, pSAGIPPasswordChange->str_Password1, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPPasswordChange::getPassword2(char *strPassword)
{
    ASSERT(strPassword != NULL);
    SAGIPPasswordChange *pSAGIPPasswordChange = (SAGIPPasswordChange *)m_pucPDU;
    strncpy(strPassword, pSAGIPPasswordChange->str_Password2, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}


int AGIPPasswordChange::setUserID(uint32_t unUserID)
{
    SAGIPPasswordChange *pSAGIPPasswordChange = (SAGIPPasswordChange *)m_pucPDU;
    pSAGIPPasswordChange->un_User_ID = htonl(unUserID);
    return S_SUCCESS;
}

int AGIPPasswordChange::setVerifyFlag(int8_t cVerifyFlag)
{
    SAGIPPasswordChange *pSAGIPPasswordChange = (SAGIPPasswordChange *)m_pucPDU;
    pSAGIPPasswordChange->c_Verify_Flag = cVerifyFlag;
    return S_SUCCESS;
}

int AGIPPasswordChange::setPasswordType1(int8_t cPasswordType1)
{
    SAGIPPasswordChange *pSAGIPPasswordChange = (SAGIPPasswordChange *)m_pucPDU;
    pSAGIPPasswordChange->c_Password_Type1 = cPasswordType1;
    return S_SUCCESS;
}

int AGIPPasswordChange::setPasswordType2(int8_t cPasswordType2)
{
    SAGIPPasswordChange *pSAGIPPasswordChange = (SAGIPPasswordChange *)m_pucPDU;
    pSAGIPPasswordChange->c_Password_Type2 = cPasswordType2;
    return S_SUCCESS;
}

int AGIPPasswordChange::setPassword1(const char *strPassword1)
{
    ASSERT(strPassword1 != NULL);
    SAGIPPasswordChange *pSAGIPPasswordChange = (SAGIPPasswordChange *)m_pucPDU;
    strncpy(pSAGIPPasswordChange->str_Password1, strPassword1, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}

int AGIPPasswordChange::setPassword2(const char *strPassword2)
{
    ASSERT(strPassword2 != NULL);
    SAGIPPasswordChange *pSAGIPPasswordChange = (SAGIPPasswordChange *)m_pucPDU;
    strncpy(pSAGIPPasswordChange->str_Password2, strPassword2, AGIP_PASSWORD_LEN);
    return S_SUCCESS;
}


int AGIPPasswordChange::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t  unUserID      = 0;
    int8_t    cVerifyFlag   = 0;

    char      strPassword1[AGIP_PASSWORD_LEN + 1];
    int8_t    cPasswordType1 = 0;
    
    char      strPassword2[AGIP_PASSWORD_LEN + 1];
    int8_t    cPasswordType2 = 0;

    SysProtocol::showInfo();
    nRetCode = getUserID(&unUserID);
    nRetCode = getVerifyFlag(&cVerifyFlag);

    nRetCode = getPassword1(strPassword1);
    strPassword1[AGIP_PASSWORD_LEN] = '\0';
    nRetCode = getPasswordType1(&cPasswordType1);

    nRetCode = getPassword2(strPassword2);
    strPassword2[AGIP_PASSWORD_LEN] = '\0';
    nRetCode = getPasswordType2(&cPasswordType2);


    printf("--------------------------------------------------------AGIPPasswordChange\n");
    printf("User_ID:\t%u\n", unUserID);
    printf("Verify_Flag:\t%d\n", cVerifyFlag);

    printf("Password1:\t%s\n", strPassword1);
    switch(cPasswordType1)
    {
    //case PASSWORD_TYPE_AUTHENTICATION:
    //    printf("Password_Type:\t%d(Authentication)\n", cPasswordType);
    //    break;
    case PASSWORD_TYPE_SECURITY:
        printf("Password_Type1:\t%d(Security Code Verify)\n", cPasswordType1);
        break;
    case PASSWORD_TYPE_ITEMS:
        printf("Password_Type1:\t%d(User Items)\n", cPasswordType1);
        break;
    case PASSWORD_TYPE_MONEY:
        printf("Password_Type1:\t%d(User Money)\n", cPasswordType1);
        break;
    default:
        printf("Password_Type1:\t%d(Others)\n", cPasswordType1);
        break;
    }

    printf("Password2:\t%s\n", strPassword2);
    switch(cPasswordType2)
    {
    //case PASSWORD_TYPE_AUTHENTICATION:
    //    printf("Password_Type:\t%d(Authentication)\n", cPasswordType);
    //    break;
    //case PASSWORD_TYPE_SECURITY:
    //    printf("Password_Type:\t%d(Security Code Verify)\n", cPasswordType2);
    //    break;
    case PASSWORD_TYPE_ITEMS:
        printf("Password_Type2:\t%d(User Items)\n", cPasswordType2);
        break;
    case PASSWORD_TYPE_MONEY:
        printf("Password_Type2:\t%d(User Money)\n", cPasswordType2);
        break;
    default:
        printf("Password_Type2:\t%d(Others)\n", cPasswordType2);
        break;
    }
printf("--------------------------------------------------------AGIPPasswordChange\n");
    return S_SUCCESS;
}
////////////////////
//AGIPPasswordChangeRes

AGIPPasswordChangeRes::AGIPPasswordChangeRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_PASSWORD_CHANGE_RES, sizeof(SAGIPPasswordChangeRes))
{

}

AGIPPasswordChangeRes::~AGIPPasswordChangeRes(void)
{

}

int AGIPPasswordChangeRes::getResultCode(int32_t *pnResultCode)
{
    ASSERT(pnResultCode != NULL);
    SAGIPPasswordChangeRes *pSAGIPPasswordChangeRes = (SAGIPPasswordChangeRes *)m_pucPDU;
    *pnResultCode = htonl(pSAGIPPasswordChangeRes->n_Result_Code);
    return S_SUCCESS;
}

int AGIPPasswordChangeRes::setResultCode(int32_t nResultCode)
{
    SAGIPPasswordChangeRes *pSAGIPPasswordChangeRes = (SAGIPPasswordChangeRes *)m_pucPDU;
    pSAGIPPasswordChangeRes->n_Result_Code = htonl(nResultCode);
    return S_SUCCESS;
}

int AGIPPasswordChangeRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t   nResultCode = 0;

    SysProtocol::showInfo();
    nRetCode = getResultCode(&nResultCode);

    printf("--------------------------------------------------------AGIPPasswordChangeRes\n");
    printf("Result_Code:\t%d\n", nResultCode);

    printf("--------------------------------------------------------AGIPPasswordChangeRes\n");
    return S_SUCCESS;
}
