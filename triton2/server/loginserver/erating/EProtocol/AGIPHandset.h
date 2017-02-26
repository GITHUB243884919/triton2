#ifndef __AGIP_HANDSET_H__
#define __AGIP_HANDSET_H__

#include "SysProtocol.h"

#define CMD_HANDSET                     0x10001001
#define CMD_HANDSET_RES                 0x20001001

typedef struct _AGIP_HANDSET_
{
    _AGIP_HEADER_   header;
    _AGIP_CHECKSUM_ checksum;
} SAGIPHandset, *PSAGIPHandset;

typedef struct _AGIP_HANDSET_RES_
{
    _AGIP_HEADER_   header;
    _AGIP_CHECKSUM_ checksum;
} SAGIPHandsetRes, *PSAGIPHandsetRes;


class Engine_Export AGIPHandset :
    public SysProtocol
{
public:
    AGIPHandset(void);
public:
    virtual ~AGIPHandset(void);
};


class Engine_Export AGIPHandsetRes :
    public SysProtocol
{
public:
    AGIPHandsetRes(void);
public:
    virtual ~AGIPHandsetRes(void);
};

#endif
