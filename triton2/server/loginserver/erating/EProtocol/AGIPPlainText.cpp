#include "AGIPPlainText.h"

#ifdef WIN32
#   define strtok_r    strtok_s
#endif
//////////////////////////////////////////////////////////////////////////
// AGIPPlainText members.
AGIPPlainText::AGIPPlainText(uint8_t ucVersion, uint32_t unCommandID,
        char sep1, char sep2, char sep3 /* =  */, char sep4 /* =  */, char sep5 /* = */ )
: SysProtocol(ucVersion, unCommandID, sizeof(_AGIP_HEADER_) + sizeof(_AGIP_CHECKSUM_))
, m_cParamSep(sep1)
, m_cKeyValSep(sep2)
, m_cParamSep2(sep3)
, m_cKeyValSep2(sep4)
, m_cGroupSep(sep5)
{
}

AGIPPlainText::~AGIPPlainText(void)
{
}


static int getValueWithKey(
    const char* pBuffer,
    int nBufLen,
    char cParamSep,
    char cKeyValSep,
    const char* pcKey,
    char* pcValue,
    int nMaxValLen
)
{
    if (nBufLen > AGIP_MAX_BUFFER_SIZE)
    {
        printf("string is too long.\n");
        return E_ERROR;
    }
    if (nBufLen <= 0 || strlen(pBuffer) < 1)
    {
        return E_ERROR;
    }

    char szBuf[AGIP_MAX_BUFFER_SIZE+1] = {0};
    memset(szBuf, 0, AGIP_MAX_BUFFER_SIZE+1);
    strncpy(szBuf, pBuffer, nBufLen);

    char* pParams = szBuf;
    char* cp1 = 0;
    char* cp2 = 0;
    while ( (cp1 = strchr(pParams, cParamSep)) != NULL )
    {
        *cp1 = '\0';// if cp1 is head, pParams = ''.
        if( (cp2 = strchr(pParams, cKeyValSep)) != NULL )
        {
            *cp2 = '\0';
            if ( 0 == strcmp(pParams, pcKey) )
            {
                strncpy(pcValue, cp2+1, nMaxValLen);// if cp2+1=cp1, szValue = ''.
                return S_SUCCESS;
            }
        }
        pParams = cp1 + 1;// if cp1 is tail, pParams = ''.
    }
    if (pParams > szBuf)// process the last param.
    {
        if( (cp2 = strchr(pParams, cKeyValSep)) != NULL )
        {
            *cp2 = '\0';
            if ( 0 == strcmp(pParams, pcKey) )
            {
                strncpy(pcValue, cp2+1, nMaxValLen);
                return S_SUCCESS;
            }
        }
    }
    return E_ERROR;
}

int AGIPPlainText::getParameter(const char* pcKey, char* pcValue, int nValLen)const
{
    char* pParams = (char*)m_pucPDU + sizeof(_AGIP_HEADER_);
    int nParamLen = (int)strlen(pParams);
    if ( nParamLen > AGIP_MAX_BODY_SIZE )
        return E_PDU_ERROR;

    return getValueWithKey(pParams, nParamLen, m_cParamSep, m_cKeyValSep,
                            pcKey, pcValue, nValLen);
}

int AGIPPlainText::setParameter(const char *szName, const char* szValue)
{
    char* szParams = (char*)m_pucPDU + sizeof(_AGIP_HEADER_);
    int nDataLen = m_usTotalLength - sizeof(_AGIP_HEADER_) - sizeof(_AGIP_CHECKSUM_);
    // param too long.
    if ( nDataLen + strlen(szName) + strlen(szValue) >= AGIP_MAX_BODY_SIZE )
    {
        printf("Parameter too long!\n");
        return E_PDU_ERROR;
    }

    strncat(szParams, &m_cParamSep, 1);
    strcat(szParams, szName);
    strncat(szParams, &m_cKeyValSep, 1);
    strcat(szParams, szValue);
    m_usTotalLength += (uint16_t)(strlen(szName) + strlen(szValue) + 2);
    this->setTotalLength(m_usTotalLength);
    return S_SUCCESS;
}

int AGIPPlainText::showInfo()
{
    SysProtocol::showInfo();

    uint32_t unCommandID = 0;
    getCommandID(&unCommandID);

    printf("--------------------------------------------------------AGIPPlainText\n");
    printf("Command: %X ©¦ Params: %s\n", unCommandID, m_pucPDU + sizeof(_AGIP_HEADER_));
    printf("--------------------------------------------------------AGIPPlainText\n");

    return S_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// AGIPPfItemSend members
int AGIPPfItemSend::setItemDetail(int nCount, const ItemInfo* psItemArray)
{
    int nRetCode = E_ERROR;
    //int nResult = E_ERROR;

    ASSERT(psItemArray != NULL);

    char szDetail[AGIP_MAX_BODY_SIZE+1] = {0};
    memset(szDetail, 0, AGIP_MAX_BODY_SIZE+1);
    char szItemNum[AGIP_INTEGER_STR_LEN+1] = {0};
    int nItemInfoLen = AGIP_ITEM_CODE_LEN + AGIP_INTEGER_STR_LEN + 19;
    for (int i=0; i<nCount; ++i)
    {
        memset(szItemNum, 0, AGIP_INTEGER_STR_LEN+1);
        if (strlen(szDetail) + nItemInfoLen > AGIP_MAX_BODY_SIZE)
        {
            printf("too many items.\n");
            return E_PDU_ERROR;
        }
        strcpy(szDetail+strlen(szDetail), "ItemCode");
        szDetail[strlen(szDetail)] = m_cKeyValSep2;

        strncpy(szDetail+strlen(szDetail), (psItemArray+i)->szItemCode, AGIP_ITEM_CODE_LEN);
        szDetail[strlen(szDetail)] = m_cParamSep2;
        strcpy(szDetail+strlen(szDetail), "ItemNum");
        szDetail[strlen(szDetail)] = m_cKeyValSep2;

        sprintf(szItemNum, "%d", (psItemArray+i)->nItemNum);
        strncpy(szDetail+strlen(szDetail), szItemNum, AGIP_INTEGER_STR_LEN);
        szDetail[strlen(szDetail)] = m_cGroupSep;
    }

    nRetCode = setParameter("Detail", szDetail);
    if (nRetCode != S_SUCCESS)
    {
        return E_ERROR;
    }

    return S_SUCCESS;
}

int AGIPPfItemSend::setItemDetail(int nCount, const void* pItemCodeArray, int pnElementSize, const int* pnItemNumArray)
{
    int nRetCode = E_ERROR;
    //int nResult = E_ERROR;

    ASSERT(pItemCodeArray != NULL);
    ASSERT(pnItemNumArray != NULL);

    ItemInfo* psItemArray = new ItemInfo[nCount];
    memset(psItemArray, 0, nCount*sizeof(ItemInfo));


    for (int i=0; i<nCount; ++i)
    {
        strncpy( (psItemArray+i)->szItemCode, (char*)pItemCodeArray + i*pnElementSize, AGIP_ITEM_CODE_LEN);
        (psItemArray+i)->nItemNum = *(pnItemNumArray+i);
    }

    nRetCode = setItemDetail(nCount, psItemArray);
    if (nRetCode != S_SUCCESS)
    {
        return E_ERROR;
    }

    delete[] psItemArray;
    return S_SUCCESS;
}

int AGIPPfItemSend::getItemDetail(int* pnCount, ItemInfo* psItemArray)
{
    int nRetCode = E_ERROR;
 
    ASSERT(psItemArray != NULL);

    char szDetail[AGIP_MAX_BODY_SIZE] = {0};
    nRetCode = getParameter("Detail", szDetail, AGIP_MAX_BODY_SIZE);
    if (nRetCode != S_SUCCESS)
    {
        return nRetCode;
    }

    char szItemNum[AGIP_INTEGER_STR_LEN+1] = {0};
    char szGroupSep[] = {m_cGroupSep, '\0'};
    char* pcToken = 0;
    char* pch = strtok_r(szDetail, szGroupSep, &pcToken);
    int i = 0;
    while (pch != NULL && *pnCount > 0)
    {
        nRetCode = getValueWithKey(pch, (int)strlen(pch), m_cParamSep2,
                        m_cKeyValSep2, "ItemCode", (psItemArray+i)->szItemCode, AGIP_ITEM_CODE_LEN);
        memset(szItemNum, 0, AGIP_INTEGER_STR_LEN);
        nRetCode = getValueWithKey(pch, (int)strlen(pch), m_cParamSep2,
                        m_cKeyValSep2, "ItemNum", szItemNum, AGIP_INTEGER_STR_LEN);
        if (S_SUCCESS == nRetCode)
        {
            (psItemArray+i)->nItemNum = atoi(szItemNum);
        }
        i++;
        --(*pnCount);
        pch = strtok_r(NULL, szGroupSep, &pcToken);
    }
    *pnCount = i;

    return S_SUCCESS;
}

int AGIPPfItemSend::getItemDetail(int* pnCount, void* pItemCodeArray, int nElementSize, int* pnItemNumArray)
{
    int nRetCode = E_ERROR;

    ASSERT(pItemCodeArray != NULL);
    ASSERT(pnItemNumArray != NULL);

    ItemInfo* psItemArray = new ItemInfo[*pnCount];
    memset(psItemArray, 0, (*pnCount)*sizeof(ItemInfo));

    nRetCode = getItemDetail(pnCount, psItemArray);
    if (nRetCode != S_SUCCESS)
    {
        return E_ERROR;
    }

    for (int i=0; i<*pnCount; ++i)
    {
        strncpy((char*)pItemCodeArray + i*nElementSize, (psItemArray+i)->szItemCode, AGIP_ITEM_CODE_LEN);
        *(pnItemNumArray+i) = (psItemArray+i)->nItemNum;
    }

    delete[] psItemArray;

    return S_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// AGIPPfBalanceQueryRes members
int AGIPPfBalanceQueryRes::setBalanceDetail(int nCount, const BalanceInfo* psBalanceArray)
{
    int nRetCode = E_ERROR;
    //int nResult = E_ERROR;

    ASSERT(psBalanceArray != NULL);

    char szDetail[AGIP_MAX_BODY_SIZE+1] = {0};
    memset(szDetail, 0, AGIP_MAX_BODY_SIZE+1);
    char szNumber[AGIP_INTEGER_STR_LEN+1] = {0};
    int nBalanceInfoLen = 2 * AGIP_INTEGER_STR_LEN + 19;
    for (int i=0; i<nCount; ++i)
    {
        if (strlen(szDetail) + nBalanceInfoLen > AGIP_MAX_BODY_SIZE)
        {
            printf("too many items.\n");
            return E_PDU_ERROR;
        }
        strcpy(szDetail+strlen(szDetail), "SubjectID");
        szDetail[strlen(szDetail)] = m_cKeyValSep2;

        memset(szNumber, 0, AGIP_INTEGER_STR_LEN);
        sprintf(szNumber, "%d", (psBalanceArray+i)->nSubjectID);
        strncpy(szDetail+strlen(szDetail), szNumber, AGIP_INTEGER_STR_LEN);
        szDetail[strlen(szDetail)] = m_cParamSep2;
        
        strcpy(szDetail+strlen(szDetail), "Amount");
        szDetail[strlen(szDetail)] = m_cKeyValSep2;

        memset(szNumber, 0, AGIP_INTEGER_STR_LEN);
        sprintf(szNumber, "%d", (psBalanceArray+i)->nAmount);
        strncpy(szDetail+strlen(szDetail), szNumber, AGIP_INTEGER_STR_LEN);
        szDetail[strlen(szDetail)] = m_cGroupSep;
    }

    nRetCode = setParameter("Detail", szDetail);
    if (nRetCode != S_SUCCESS)
    {
        return E_ERROR;
    }

    return S_SUCCESS;
}

int AGIPPfBalanceQueryRes::setBalanceDetail(int nCount, const int* pnSubjectArray, const int* pnAmountArray)
{
    int nRetCode = E_ERROR;
    //int nResult = E_ERROR;

    ASSERT(pnSubjectArray != NULL);
    ASSERT(pnAmountArray != NULL);

    BalanceInfo* psBalanceArray = new BalanceInfo[nCount];
    memset(psBalanceArray, 0, nCount*sizeof(BalanceInfo));


    for (int i=0; i<nCount; ++i)
    {
         (psBalanceArray+i)->nSubjectID = *(pnSubjectArray+i);
         (psBalanceArray+i)->nAmount = *(pnAmountArray+i);
    }

    nRetCode = setBalanceDetail(nCount, psBalanceArray);
    if (nRetCode != S_SUCCESS)
    {
        return E_ERROR;
    }

    delete[] psBalanceArray;
    return S_SUCCESS;
}

int AGIPPfBalanceQueryRes::getBalanceDetail(int* pnCount, BalanceInfo* psBalanceArray)
{
    int nRetCode = E_ERROR;

    ASSERT(psBalanceArray != NULL);

    char szDetail[AGIP_MAX_BODY_SIZE] = {0};
    nRetCode = getParameter("Detail", szDetail, AGIP_MAX_BODY_SIZE);
    if (nRetCode != S_SUCCESS)
    {
        return nRetCode;
    }

    char szNumber[AGIP_INTEGER_STR_LEN+1] = {0};
    char szGroupSep[] = {m_cGroupSep, '\0'};
    char* pcToken = 0;
    char* pch = strtok_r(szDetail, szGroupSep, &pcToken);
    int i = 0;
    while (pch != NULL && *pnCount > 0)
    {
        memset(szNumber, 0, AGIP_INTEGER_STR_LEN);
        nRetCode = getValueWithKey(pch, (int)strlen(pch), m_cParamSep2,
                        m_cKeyValSep2, "SubjectID", szNumber, AGIP_INTEGER_STR_LEN);
        if (S_SUCCESS == nRetCode)
        {
            (psBalanceArray + i)->nSubjectID = atoi(szNumber);
        }

        memset(szNumber, 0, AGIP_INTEGER_STR_LEN);
        nRetCode = getValueWithKey(pch, (int)strlen(pch), m_cParamSep2,
                        m_cKeyValSep2, "Amount", szNumber, AGIP_INTEGER_STR_LEN);
        if (S_SUCCESS == nRetCode)
        {
            (psBalanceArray + i)->nAmount = atoi(szNumber);
        }
        i++;
        --(*pnCount);
        pch = strtok_r(NULL, szGroupSep, &pcToken);
    }
    *pnCount = i;

    return S_SUCCESS;
}

int AGIPPfBalanceQueryRes::getBalanceDetail(int* pnCount, int* pnSubjectArray, int* pnAmountArray)
{
    int nRetCode = E_ERROR;

    ASSERT(pnSubjectArray != NULL);
    ASSERT(pnAmountArray != NULL);

    BalanceInfo* psBalanceArray = new BalanceInfo[*pnCount];
    memset(psBalanceArray, 0, (*pnCount)*sizeof(BalanceInfo));

    nRetCode = getBalanceDetail(pnCount, psBalanceArray);
    if (nRetCode != S_SUCCESS)
    {
        return E_ERROR;
    }

    for (int i=0; i<*pnCount; ++i)
    {
        *(pnSubjectArray+i) = (psBalanceArray+i)->nSubjectID;
        *(pnAmountArray+i) = (psBalanceArray+i)->nAmount;
    }

    delete[] psBalanceArray;
    return S_SUCCESS;
}
