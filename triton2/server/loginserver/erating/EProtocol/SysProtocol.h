#ifndef __SYS_PROTOCOL_H__
#define __SYS_PROTOCOL_H__

#include <assert.h>
#include <stdio.h>
#define SYS_MEMORY_INIT_HANDLERS 1024

#include <OS.h>
#include <OperationCode.h>

#define FILE_MAX_PATH   256
#define MAX_BUFFER_SIZE 1024

#ifndef ASSERT
#   define ASSERT assert
#endif

#ifdef WIN32
#   define Engine_Export __declspec(dllexport)
#else
#   define Engine_Export
#endif

#ifndef NULL
#   define NULL 0
#endif


#define THR_NEW_LWP         0
#define THR_JOINABLE        0
#define THR_INHERIT_SCHED   0

#if !defined (DEFAULT_THREAD_PRIORITY)
#  define DEFAULT_THREAD_PRIORITY (-0x7fffffffL - 1L)
#endif /* DEFAULT_THREAD_PRIORITY */


#define LOGGER SysLogManager::output

#define PROCESS_CHECK_ERROR(X) do{if (!(X)){goto ExitError;}}while(false)
#define PROCESS_CHECK_OK(X)    do{if (X){goto ExitOK;}}while(false)
#define PROCESS_CHECK_ERROR_RET(X, RET, RET_CODE) do{if (!(X)){RET = RET_CODE; goto ExitError;}}while(false)


#define AGIP_MAX_BUFFER_SIZE            1024
#define AGIP_MAX_BODY_SIZE              1000
#define AGIP_MIN_MSG_SIZE               (sizeof(SAGIPHeader) + sizeof(SAGIPChecksum))
// AGIP field size.
#define AGIP_LENGTH_FIELD_SIZE          2

#define AGIP_INVALID_VERSION            0x00
#define AGIP_CURRENT_VERSION            0x10
#define AGIP_NO_REMAIN_PACKAGES         0x00

#define CMD_INVALID                     0x00000000

enum _AGIP_ARRAY_FIELD_LEN_
{
    AGIP_GATEWAY_CODE_LEN   = 32,
    AGIP_MAC_LEN            = 12,
    AGIP_MAX_IP_LEN         = 15,
    AGIP_IP4_STR_LEN        = 15,

    AGIP_USER_NAME_LEN      = 32,
    AGIP_ROLE_NAME_LEN      = 32,
    AGIP_ORDER_ID_LEN       = 32,

    AGIP_PASSWORD_LEN       = 32,
    AGIP_SERIAL_NO_LEN      = 32,
    AGIP_IB_CODE_LEN        = 32,
    AGIP_ITEM_CODE_LEN      = 32,
    AGIP_MATRIX_COORD_LEN   = 16,

    AGIP_CARD_NUM_LEN       = 32,
    AGIP_CARD_CODE_LEN      = 32,
    AGIP_IDCARD_NUM_LEN     = 18,

    AGIP_MAX_IB_AUDIT_CNT   = 64,
    AGIP_MAX_GW_DATA_CNT    = 120,

    AGIP_INTEGER_STR_LEN    = 11
};

typedef struct _AGIP_HEADER_
{
    uint16_t us_Total_Length;
    uint8_t  uc_Version;
    uint8_t  uc_Remain_Packages;
    uint32_t un_Command_ID;
    uint32_t un_Sequence_ID;
}SAGIPHeader, *PSAGIPHeader;

typedef struct _AGIP_CHECKSUM_
{
    uint16_t    us_Resv;
    uint16_t    us_Checksum;
}SAGIPChecksum, *PSAGIPChecksum;

typedef struct _PRICE_INFO_
{
    int32_t nSubjectID;
    int32_t nPrice;
    int32_t nDiscountPrice;
}PRICE_INFO;

typedef struct _ITEM_INFO_ {
    uint32_t        un_Activity_ID;
    char            str_Item_Code[AGIP_ITEM_CODE_LEN];
    int32_t         n_Item_Num;
    uint32_t        un_Begin_Time;
    uint32_t        un_End_Time;
} ITEM_INFO;

typedef struct _PURCHASE_ID_
{
    uint32_t        un_Purchase_ID_Higher;
    uint32_t        un_Purchase_ID_Lower;
}PURCHASE_ID;

typedef struct _DATA_INFO_
{
    int32_t nDataType;
    int32_t nDataValue;
}DATA_INFO;

typedef struct _ROLE_INFO_
{
    uint32_t unRoleID;
    char     strRoleName[AGIP_ROLE_NAME_LEN];
    int8_t   cRoleGender;
    int8_t   cRoleOccupation;
    uint32_t unRoleCreateTime;
}ROLE_INFO;

typedef struct _BALANCE_INFO_
{
    int32_t nSubjectID;
    int32_t nAmount;
}BALANCE_INFO;

typedef struct _ALL_BALANCE_INFO_
{
    int32_t nSubjectID;
    int32_t nAmount;
    int32_t nLeftAmount;
}ALL_BALANCE_INFO;

typedef struct _GAME_PASSWORD_STATE_
{
    uint8_t          cPasswordType;
    uint8_t          cPasswordState;
} GAMEPASSWORDSTATE, *PGAMEPASSWORDSTATE;

typedef struct _GW_DATA_
{
    uint32_t unGatewayID;
    uint32_t unDataValue;
} GW_DATA;

uint64_t htonll(uint64_t hostlonglong, int32_t flag = 1);
uint64_t ntohll(uint64_t netlonglong, int32_t flag = 1);

Engine_Export const char* stringTime(time_t tm);
Engine_Export int IP2String(uint32_t ip, char* ip_string);

class Engine_Export SysProtocol
{
public:
    SysProtocol(uint8_t ucVersion, uint32_t unCommandID, uint16_t usTotalLength);
    virtual ~SysProtocol(void);
    
    virtual int initialize();

    int setDecodedPDU(const void *pPDU, int32_t nPDULength, int32_t nNeedsCheckSum = 1);
    int getDecodedPDU(void *pPDUBuffer, uint16_t usBufferSize, int32_t *pnPDULength);
    int getTotalLength(uint16_t *pusTotalLength);
    int getVersion(uint8_t *pucVersion);
    int getRemainPackages(uint8_t *pucRemainPackages);
    int getCommandID(uint32_t *punCommandID);
    int getSequenceID(uint32_t *punSequenceID);

    int setRemainPackages(uint8_t ucRemainPackages);
    int setSequenceID(uint32_t unSequenceID);
    int setCommandID(uint32_t unCommandID);

    /* Copy sequence ID from pSysProtocol directly. */
    int cloneSequenceID(const SysProtocol *pSysProtocol);
    /* Compare binary stream between this and pSysProtocol. */
    int compare(const SysProtocol *pSysProtocol, int *pnBytesEqual);

    virtual int showInfo();

protected:
    int setTotalLength(uint16_t usTotalLength);
    int setVersion(uint8_t ucVersion);

    int checkPDUValid(const void *pPDU, int32_t nLength, int32_t nNeedsCheckSum = 1);
    static int calCheckSum(const void *pPDU, int32_t nLength, uint16_t *pusRetCheckSum);

    uint8_t        *m_pucPDU;
    SAGIPHeader    *m_pSAGIPHeader;
    uint16_t        m_usTotalLength;

private:
    static int parseCommandID(const void *pPDU, int32_t nPDULength, uint32_t *punCommandID);
    int setEncodedPDU(const void *pPDU, int32_t nPDULength, int32_t nNeedsCheckSum = 1);
    int getEncodedPDU(void *pPDUBuffer, uint16_t usBufferSize, int32_t *pnPDULength);

    uint32_t        m_unCommandID;
    uint8_t         m_ucVersion;
    uint8_t         m_arrucPDU[AGIP_MAX_BUFFER_SIZE];
    static int      m_nKeyLoaded;
};

#endif

