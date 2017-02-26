#ifndef __SYS_ALGORITHM_CRC32_H__
#define __SYS_ALGORITHM_CRC32_H__
#include "Macro.h"


class Engine_Export SysAlgorithmCRC32
{
public:
    SysAlgorithmCRC32(void);
public:
    virtual ~SysAlgorithmCRC32(void);
    static int compute(const void *pInput, int32_t nInputLength, uint32_t unCrc32Init, uint32_t *punCrc32Result);
};
#endif

