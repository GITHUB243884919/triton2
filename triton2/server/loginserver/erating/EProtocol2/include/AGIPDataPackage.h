#ifndef __AGIP_DATA_PACKAGE_H__
#define __AGIP_DATA_PACKAGE_H__

#include "SysProtocol.h"

#define CMD_DATA_PACKAGE        0x10006001
#define CMD_DATA_PACKAGE_RES    0x20006001

#undef  LENGOFSUBLENGTH
#define LENGOFSUBLENGTH         (sizeof(SAGIPDataPackage) - sizeof(_AGIP_HEADER_) - sizeof(_AGIP_CHECKSUM_))

typedef struct _AGIP_DATA_PACKAGE_
{
    _AGIP_HEADER_       header;
    uint16_t            sub_Length;
    int16_t             package_Type;
    uint32_t            ip;
    uint16_t            port;
    int16_t             flag;

	uint32_t			package_Time_L;		// 用于记录打包时间的低32位

    uint32_t            command_ID;
    _AGIP_CHECKSUM_     checksum;
}SAGIPDataPackage, *PSAGIPDataPackage;

class Engine_Export AGIPDataPackage : public SysProtocol
{
public:
    AGIPDataPackage();
    ~AGIPDataPackage();

    uint16_t    getSubLength(void) const;
    int16_t     getPackageType(void) const;
    void        setPackageType(int16_t package_type);
    uint32_t    getIP(void) const;
    void        setIP(uint32_t ip);
    uint16_t    getPort(void) const;
    void        setPort(uint16_t port);
    int16_t     getFlag(void) const;
    void        setFlag(int16_t flag);
	uint32_t	getPackageTimeL(void);
	void		setPackageTimeL(uint32_t package_time_l);
    uint32_t    getCmdID(void) const;
    void        setCmdID(uint32_t command_id);

    // total len
    uint16_t    getTotalLength(void) const;

    // data len
    uint16_t    getDataLength(void) const;

    int         setData(const void *data, size_t data_len);
    int         setData(SysProtocol *proto);
    uint32_t    getData(void *buff_ref, size_t buff_size) const;

    int         showInfo(void);

protected:
    void        setSubLength(uint16_t sub_len);
};

#endif

