#ifndef __AGIP_USER_SERIAL_QUERY_H__
#define __AGIP_USER_SERIAL_QUERY_H__

#include "SysProtocol.h"

#define CMD_USER_SERIAL_QUERY                        0x10003206
#define CMD_USER_SERIAL_QUERY_RES                    0x20003206
#define AGIP_MAX_USER_SERIAL_COUNT                   24

typedef struct _AGIP_USER_SERIAL_QUERY_
{
    _AGIP_HEADER_   header;

    uint32_t        un_user_id;
    uint32_t        un_activity_id;

    _AGIP_CHECKSUM_ checkSum;
} SAGIPUserSerialQuery, *PSAGIPUserSerialQuery;

typedef struct _AGIP_USER_SERIAL_QUERY_RES_
{
    _AGIP_HEADER_   header;

    int32_t         n_result_code;
    uint32_t        un_use_time;

    _AGIP_CHECKSUM_ checksum;
} SAGIPUserSerialQueryRes, *PSAGIPUserSerialQueryRes;

class Engine_Export AGIPUserSerialQuery :
    public SysProtocol
{
public:
    AGIPUserSerialQuery(void);

public:
    virtual     ~AGIPUserSerialQuery(void);

    uint32_t    getUserID(void) const;
    void        setUserID(uint32_t unUserID);

    uint32_t    getActivityID(void) const;
    void        setActivityID(uint32_t unActivityID);

    virtual int showInfo();
};


class Engine_Export AGIPUserSerialQueryRes :
    public SysProtocol
{
public:
    AGIPUserSerialQueryRes(void);

public:
    virtual     ~AGIPUserSerialQueryRes(void);

    int32_t     getResultCode(void) const;
    void        setResultCode(int32_t nResultCode);    

    uint32_t    getUseTime(void) const;
    void        setUseTime(uint32_t unUseTime);

    virtual int showInfo();
};

#endif

