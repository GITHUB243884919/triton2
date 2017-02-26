#ifndef __AGIP_BINDNOTIFY_H__
#define __AGIP_BINDNOTIFY_H__

#include "SysProtocol.h"

#define CMD_BIND_NOTIFY                        0x10000003
#define CMD_BIND_NOTIFY_RES                    0x20000003

typedef struct _AGIP_BIND_NOTIFY_
{
    _AGIP_HEADER_   header;
    uint32_t        result_Code;        // ≥…π¶”Î∑Ò
    uint32_t        bind_IP;            // GS-host
    uint16_t        bind_Port;          // GS-port
    int16_t         op_Type;            // 0£≠bind£¨1£≠unbind
    _AGIP_CHECKSUM_ checkSum;
} SAGIPBindNotify, *PSAGIPBindNotify;

class Engine_Export AGIPBindNotify :
    public SysProtocol
{
public:
    AGIPBindNotify(void);
    virtual ~AGIPBindNotify(void);

    void        setResultCode(int32_t result_code);
    int32_t     getResultCode(void);

    void        setBindIP(uint32_t bind_ip);
    uint32_t    getBindIP(void);
    void        setBindPort(uint16_t bind_port);
    uint16_t    getBindPort(void);

    void        setOpType(uint16_t op_type);
    uint16_t    getOpType(void);

    virtual int showInfo();
};

#endif

