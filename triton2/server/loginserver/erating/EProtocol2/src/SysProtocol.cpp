#include "SysProtocol.h"
#include "SysAlgorithmCRC.h"
#include <time.h>

//--------------------
// flag: 1 - 公版；2 - 问鼎低10位处理；3 - 问鼎低9位处理
//--------------------
uint64_t htonll(uint64_t hostlonglong, int32_t flag)
{
    if ((2 == flag) || (3 == flag)) {
        uint64_t wd_base_num = 0;

        if (2 == flag) {
            wd_base_num = 10000000000ULL;   // 10个0
        } else if (3 == flag) {
            wd_base_num = 1000000000ULL;    // 9个0
        }

        hostlonglong = ((hostlonglong / wd_base_num) << 32) + (hostlonglong % wd_base_num);
    }

    uint64_t  netlonglong = 0;
    uint8_t* phost = (uint8_t*)&hostlonglong;
    uint8_t* pnet  = (uint8_t*)&netlonglong;

    for (uint32_t i = 0; i < sizeof(hostlonglong); ++i) {
        pnet[i] = phost[sizeof(hostlonglong) - 1 - i];
    }

    return netlonglong;
}

//--------------------
// flag: 1 - 公版；2 - 问鼎低10位处理；3 - 问鼎低9位处理
//--------------------
uint64_t ntohll(uint64_t netlonglong, int32_t flag)
{
    uint64_t  hostlonglong = 0;
    uint8_t* pnet  = (uint8_t*)&netlonglong;
    uint8_t* phost = (uint8_t*)&hostlonglong;

    for (uint32_t i = 0; i < sizeof(netlonglong); ++i) {
        phost[i] = pnet[sizeof(hostlonglong) - 1 - i];
    }

    if ((2 == flag) || (3 == flag)) {
        uint64_t wd_base_num = 0;

        if (2 == flag) {
            wd_base_num = 10000000000ULL;   // 10个0
        } else if (3 == flag) {
            wd_base_num = 1000000000ULL;    // 9个0
        }

        hostlonglong = ((hostlonglong >> 32) * wd_base_num) + (0xFFFFFFFFULL & hostlonglong);
    }

    return hostlonglong;
}

const char* stringTime(time_t tm)
{
    struct tm* timeptr = localtime(&tm);
    static char result[64];
    sprintf(result, "%d-%02d-%02d_%02d:%02d:%02d",
        1900 + timeptr->tm_year,
        timeptr->tm_mon+1,
        timeptr->tm_mday,
        timeptr->tm_hour,
        timeptr->tm_min,
        timeptr->tm_sec);
    return result;
}

const char* stringTime(time_t tm, char buf[])
{
    struct tm* timeptr = localtime(&tm);
    sprintf(buf, "%d-%02d-%02d_%02d:%02d:%02d",
        1900 + timeptr->tm_year,
        timeptr->tm_mon+1,
        timeptr->tm_mday,
        timeptr->tm_hour,
        timeptr->tm_min,
        timeptr->tm_sec);
    return buf;
}

int IP2String(uint32_t ip, char* ip_string)
{
    int32_t result = E_ERROR;
    uint8_t* pucIPSeg = NULL;

    if (NULL == ip_string)
    {
        goto ExitError;
    }

    pucIPSeg = (uint8_t*)&ip;
    sprintf(ip_string, "%u.%u.%u.%u",
#if (defined(WIN32) || defined(LINUX))
        pucIPSeg[3], pucIPSeg[2], pucIPSeg[1], pucIPSeg[0]
#else
        pucIPSeg[0], pucIPSeg[1], pucIPSeg[2], pucIPSeg[3]
#endif
    );

    result = S_SUCCESS;
ExitError:

    return result;
}

SysProtocol::SysProtocol(uint8_t ucVersion, uint32_t unCommandID, uint16_t usTotalLength)
: m_pucPDU(m_arrucPDU)
, m_pSAGIPHeader((SAGIPHeader *)m_pucPDU)
, m_usTotalLength(usTotalLength)
, m_unCommandID(unCommandID)
, m_ucVersion(ucVersion)
, m_unLogBufLen(0)
{
    this->initialize();
}

SysProtocol::~SysProtocol(void)
{
    if (m_pucPDU != m_arrucPDU)
    {
        //free(m_pucPDU);
    }
}

int SysProtocol::initialize()
{
    ASSERT(m_pucPDU != NULL);
    ASSERT(m_pSAGIPHeader != NULL);
    ASSERT(AGIP_MAX_BUFFER_SIZE >= m_usTotalLength);
    //ASSERT(m_nKeyLoaded);
    memset(m_pucPDU, 0, m_usTotalLength);
    memset(m_szLogBuf, 0, sizeof(m_szLogBuf));
    this->setVersion(m_ucVersion);
    this->setCommandID(m_unCommandID);
    this->setRemainPackages(AGIP_NO_REMAIN_PACKAGES);
    this->setTotalLength(m_usTotalLength);
    return S_SUCCESS;
}

char* SysProtocol::getLogBuf(void)
{
    return m_szLogBuf + m_unLogBufLen;
}

size_t SysProtocol::getLogBufSize(void)
{
    return sizeof(m_szLogBuf) - m_unLogBufLen - 1;
}

const char* SysProtocol::toString(void)
{
    SAGIPChecksum *pSAGIPChecksum = NULL;
    pSAGIPChecksum = (SAGIPChecksum *)(m_pucPDU + m_usTotalLength - 4);

    m_unLogBufLen = snprintf(
        m_szLogBuf,
        sizeof(m_szLogBuf) - 1,
        "|len=%u|ver=0x%02x|remain=%u|cmd_id=0x%08x|seq_id=%u|game_id=%u|gw_id=%u|resv=%u|checksum=0x%04x",
        ntohs(m_pSAGIPHeader->us_Total_Length),
        m_pSAGIPHeader->uc_Version,
        m_pSAGIPHeader->uc_Remain_Packages,
        ntohl(m_pSAGIPHeader->un_Command_ID),
        ntohl(m_pSAGIPHeader->un_Sequence_ID),
        ntohl(m_pSAGIPHeader->un_Game_ID),
        ntohl(m_pSAGIPHeader->un_Gateway_ID),
        ntohs(pSAGIPChecksum->us_Resv),
        ntohs(pSAGIPChecksum->us_Checksum)
        );

    return m_szLogBuf;
}

int SysProtocol::showInfo()
{
    int      nRetCode         = E_ERROR;
    uint16_t i                = 0;
    uint16_t usTotalLength    = 0;
    uint8_t  ucVersion        = 0;
    uint8_t  ucRemainPackages = 0;
    uint32_t unCommandID      = 0;
    uint32_t unSequenceID     = 0;
    printf(">>\n");
    for (i = 0; i < m_usTotalLength; i++)
    {
        if (i%16 == 0)
        {
            printf("\n%04d:│", i);
        }
        else if (i%8 == 0)
        {
            printf(" ");
        }
        printf("%02X ", m_pucPDU[i]);
    }

    nRetCode = getTotalLength(&usTotalLength);
    nRetCode = getVersion(&ucVersion);
    nRetCode = getRemainPackages(&ucRemainPackages);
    nRetCode = getCommandID(&unCommandID);
    nRetCode = getSequenceID(&unSequenceID);
    printf("\n------------------------------------------------\n");
    printf("Total_Length:   \t%u\n",        usTotalLength);
    printf("Version     :   \t0x%02X\n",    ucVersion);
    printf("Remain_Pkgs :   \t%u\n",        ucRemainPackages);
    printf("Command_ID  :   \t0x%08X\n",    unCommandID);
    printf("Sequence_ID :   \t0x%08X\n",    unSequenceID);
    printf("Game_ID :       \t%u\n",        getGameID());
    printf("Gateway_ID :    \t%u\n",        getGatewayID());
    printf("\n<<\n");

    return S_SUCCESS;
}

int SysProtocol::calCheckSum(const void *pPDU, int32_t nLength, uint16_t *pusRetCheckSum)
{

    int nResult = E_ERROR;
    int nRetCode = E_ERROR;

    ASSERT(pusRetCheckSum != NULL);
    ASSERT(pPDU != NULL);

    // We must exclude the length of checksum.
    nRetCode = SysAlgorithmCRC::compute(pPDU, nLength, 0xFFFF, pusRetCheckSum);

    nResult = nRetCode;

    return nResult;
}

int SysProtocol::checkPDUValid(const void *pPDU, int32_t nLength, int32_t nNeedsCheckSum)
{
    int nResult                 = E_PDU_ERROR;
    int nRetCode                = E_PDU_ERROR;
    uint16_t usCheckSum         = 0;

    int32_t  nTotalLength  = 0;
    // uint8_t  ucVersion      = AGIP_INVALID_VERSION;
    // uint32_t unCommandID    = CMD_INVALID;

    ASSERT(pPDU != NULL);

    SAGIPHeader   *pSAGIPHeader = (SAGIPHeader *)pPDU;
    SAGIPChecksum *pChecksum    = (SAGIPChecksum *)((char *)pPDU + nLength - 4);

    if (pSAGIPHeader->uc_Version != m_ucVersion)
    {
        nResult = E_PDU_UNKNOWN_VERSION;
        goto ExitError;
    }
    nTotalLength = htons(pSAGIPHeader->us_Total_Length);
    if (nTotalLength != nLength)
    {
        nResult = E_PDU_LENGTH_ERROR;
        goto ExitError;
    }

    if (nNeedsCheckSum == 0)
    {
        goto ExitOK;
    }

    // We must exclude the length of checksum.
    nRetCode = SysAlgorithmCRC::compute(pPDU, nLength - 2, 0xFFFF, &usCheckSum);
    if (nRetCode != S_SUCCESS)
    {
        nResult = E_PDU_LENGTH_ERROR;
        goto ExitError;
    }

    if (ntohs(pChecksum->us_Checksum) != usCheckSum)
    {
        nResult = E_PDU_CHECKSUM_ERROR;
        goto ExitError;
    }

ExitOK:
    nResult = S_SUCCESS;
ExitError:

    return nResult;
}

int SysProtocol::getTotalLength(uint16_t *pusTotalLength)
{
    ASSERT(pusTotalLength != NULL && m_pSAGIPHeader != NULL);
    *pusTotalLength = htons(m_pSAGIPHeader->us_Total_Length);
    return S_SUCCESS;
}

int SysProtocol::getVersion(uint8_t *pucVersion)
{
    ASSERT(pucVersion != NULL && m_pSAGIPHeader != NULL);
    *pucVersion = m_pSAGIPHeader->uc_Version;
    return S_SUCCESS;
}

int SysProtocol::getRemainPackages(uint8_t *pucRemainPackages)
{
    ASSERT(pucRemainPackages != NULL && m_pSAGIPHeader != NULL);
    *pucRemainPackages = m_pSAGIPHeader->uc_Remain_Packages;
    return S_SUCCESS;
}

int SysProtocol::getCommandID(uint32_t *punCommandID)
{
    ASSERT(punCommandID != NULL && m_pSAGIPHeader != NULL);
    *punCommandID = htonl(m_pSAGIPHeader->un_Command_ID);
    return S_SUCCESS;
}

int SysProtocol::getSequenceID(uint32_t *punSequenceID)
{
    ASSERT(punSequenceID != NULL && m_pSAGIPHeader != NULL);
    *punSequenceID = htonl(m_pSAGIPHeader->un_Sequence_ID);
    return S_SUCCESS;
}

void SysProtocol::setGameID(uint32_t unGameID)
{
    ASSERT(m_pSAGIPHeader != NULL);
    m_pSAGIPHeader->un_Game_ID = (uint32_t)htonl(unGameID);
    return;
}

uint32_t SysProtocol::getGameID(void) const
{
    ASSERT(m_pSAGIPHeader != NULL);
    return (uint32_t)ntohl(m_pSAGIPHeader->un_Game_ID);
}

int32_t SysProtocol::getGameID(uint32_t* punGameID) const
{
    ASSERT(punGameID != NULL && m_pSAGIPHeader != NULL);
    *punGameID = (uint32_t)ntohl(m_pSAGIPHeader->un_Game_ID);
    return S_SUCCESS;
}

void SysProtocol::setGatewayID(uint32_t unGatewayID)
{
    ASSERT(m_pSAGIPHeader != NULL);
    m_pSAGIPHeader->un_Gateway_ID = (uint32_t)htonl(unGatewayID);
    return;
}

uint32_t SysProtocol::getGatewayID(void) const
{
    ASSERT(m_pSAGIPHeader != NULL);
    return (uint32_t)ntohl(m_pSAGIPHeader->un_Gateway_ID);
}

int32_t SysProtocol::getGatewayID(uint32_t* punGatewayID) const
{
    ASSERT(punGatewayID != NULL && m_pSAGIPHeader != NULL);
    *punGatewayID = (uint32_t)ntohl(m_pSAGIPHeader->un_Gateway_ID);
    return S_SUCCESS;
}

int SysProtocol::setTotalLength(uint16_t usTotalLength)
{
    ASSERT(m_pSAGIPHeader != NULL);
    ASSERT(AGIP_MAX_BUFFER_SIZE >= m_usTotalLength);
    m_pSAGIPHeader->us_Total_Length = htons(usTotalLength);
    return S_SUCCESS;
}

int SysProtocol::setVersion(uint8_t ucVersion)
{
    ASSERT(m_pSAGIPHeader != NULL);
    m_pSAGIPHeader->uc_Version = ucVersion;
    return S_SUCCESS;
}

int SysProtocol::setRemainPackages(uint8_t ucRemainPackages)
{
    ASSERT(m_pSAGIPHeader != NULL);
    m_pSAGIPHeader->uc_Remain_Packages = ucRemainPackages;
    return S_SUCCESS;
}

int SysProtocol::setCommandID(uint32_t unCommandID)
{
    ASSERT(m_pSAGIPHeader != NULL);
    m_pSAGIPHeader->un_Command_ID = htonl(unCommandID);
    return S_SUCCESS;
}

int SysProtocol::setSequenceID(uint32_t unSequenceID)
{
    ASSERT(m_pSAGIPHeader != NULL);
    m_pSAGIPHeader->un_Sequence_ID = htonl(unSequenceID);
    return S_SUCCESS;
}

int SysProtocol::cloneSequenceID(const SysProtocol *pSysProtocol)
{
    ASSERT(pSysProtocol != NULL);
    m_pSAGIPHeader->un_Sequence_ID = pSysProtocol->m_pSAGIPHeader->un_Sequence_ID;
    return S_SUCCESS;
}

int SysProtocol::cloneGameID(const SysProtocol *pSysProtocol)
{
    ASSERT(pSysProtocol != NULL);
    m_pSAGIPHeader->un_Game_ID = pSysProtocol->m_pSAGIPHeader->un_Game_ID;
    return S_SUCCESS;
}

int SysProtocol::cloneGatewayID(const SysProtocol *pSysProtocol)
{
    ASSERT(pSysProtocol != NULL);
    m_pSAGIPHeader->un_Gateway_ID = pSysProtocol->m_pSAGIPHeader->un_Gateway_ID;
    return S_SUCCESS;
}

int SysProtocol::clone(const SysProtocol *pSysProtocol)
{
    ASSERT(pSysProtocol != NULL);
    cloneSequenceID(pSysProtocol);
    cloneGameID(pSysProtocol);
    cloneGatewayID(pSysProtocol);
    return S_SUCCESS;
}

int SysProtocol::setDecodedPDU(const void *pPDU, int32_t nPDULength, int32_t nNeedsCheckSum)
{
    int nRetCode = E_ERROR;
    int nResult  = E_ERROR;

    // size_t stPDUOutputLength = 0;

    ASSERT(pPDU != NULL);
    ASSERT(nPDULength >= 16 && nPDULength <= AGIP_MAX_BUFFER_SIZE);
    //nRetCode        = SysProtocol::decrypt(pPDU, stPDULength, m_pucPDU, &stPDUOutputLength);
    m_usTotalLength = (uint16_t)nPDULength;
    nRetCode = checkPDUValid(pPDU, nPDULength, nNeedsCheckSum);
    if (nRetCode != S_SUCCESS)
    {
        nResult = nRetCode;
        goto ExitError;
    }

    memcpy(m_pucPDU, pPDU, nPDULength);
    nResult = S_SUCCESS;

ExitError:
    return nResult;
}

/***********************************************************************
    To get PDU pointer, and sent it directly.
***********************************************************************/

int SysProtocol::getDecodedPDU(void *pPDUBuffer, uint16_t usBufferSize, int32_t *pnPDULength)
{
    int nRetCode = E_ERROR;
    // int nResult  = E_ERROR;

    uint16_t usTotalLength = 0;
    uint16_t usCheckSum = 0;


    SAGIPChecksum *pSAGIPChecksum = NULL;
    ASSERT(pPDUBuffer  != NULL);
    ASSERT(pnPDULength != NULL);

    //pPDUPtr = m_pucPDU;
    nRetCode = this->getTotalLength(&usTotalLength);
    ASSERT(usBufferSize >= m_usTotalLength);

    pSAGIPChecksum = (SAGIPChecksum *)(m_pucPDU + m_usTotalLength - 4);
    pSAGIPChecksum->us_Resv = 0x0000;
    nRetCode = SysAlgorithmCRC::compute(m_pucPDU, m_usTotalLength - 2, 0xFFFF, &usCheckSum);
    ASSERT(nRetCode == S_SUCCESS);
    pSAGIPChecksum->us_Checksum = htons(usCheckSum);
    *pnPDULength = (uint32_t)m_usTotalLength;
    memcpy(pPDUBuffer, m_pucPDU, m_usTotalLength);
    return S_SUCCESS;
}

int SysProtocol::compare(const SysProtocol *pSysProtocol, int *pnBytesEqual)
{
    int nBytesEqual = 0;

    ASSERT(pnBytesEqual != NULL);

    *pnBytesEqual = 0;

    nBytesEqual = memcmp(this->m_pucPDU, pSysProtocol->m_pucPDU, m_usTotalLength);
    if (nBytesEqual == 0)
    {
        *pnBytesEqual = 1;
    }
    return S_SUCCESS;
}

