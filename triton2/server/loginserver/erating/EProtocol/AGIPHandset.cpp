#include "AGIPHandset.h"

AGIPHandset::AGIPHandset(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_HANDSET, sizeof(SAGIPHandset))
{
}

AGIPHandset::~AGIPHandset(void)
{
}


AGIPHandsetRes::AGIPHandsetRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_HANDSET_RES, sizeof(SAGIPHandsetRes))
{
}

AGIPHandsetRes::~AGIPHandsetRes(void)
{
}

