#ifndef __AGIP_SERIALNO_H__
#define __AGIP_SERIALNO_H__

#include "SysProtocol.h"

#define CMD_SERIALNO_VERIFY             0x10003203
#define CMD_SERIALNO_VERIFY_RES         0x20003203

typedef struct _AGIP_SERIALNO_VERIFY_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    char            str_Serial_NO[AGIP_SERIAL_NO_LEN];
    int8_t          c_Serial_Type;
    int8_t          Pad1;
    int16_t         Pad2;
    _AGIP_CHECKSUM_ checksum;
} SAGIPSerialNOVerify, *PSAGIPSerialNOVerify;

typedef struct _AGIP_SERIALNO_VERIFY_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    _AGIP_CHECKSUM_ checksum;
} SAGIPSerialNOVerifyRes, *PSAGIPSerialNOVerifyRes;

class Engine_Export AGIPSerialNOVerify :
    public SysProtocol
{
public:
    AGIPSerialNOVerify(void);
    
    enum
    {
        SERIALNO_TYPE_CDKEY = 1
    };
    int getUserID(uint32_t *punUserID);
    int getSerialNO(char *strSerialNO);
    int getSerialType(int8_t *pcSerialType);

    int setUserID(uint32_t unUserID);
    int setSerialNO(const char *strSerialNO);
    int setSerialType(int8_t cSerialType);

    virtual int showInfo();

public:
    virtual ~AGIPSerialNOVerify(void);
};

class Engine_Export AGIPSerialNOVerifyRes :
    public SysProtocol
{
    friend class AGIPSerialNOVerify;
public:
    AGIPSerialNOVerifyRes(void);

    int getResultCode(int32_t *pnResultCode);

    int setResultCode(int32_t nResultCode);
    virtual int showInfo();

public:
    virtual ~AGIPSerialNOVerifyRes(void);
};

#endif
