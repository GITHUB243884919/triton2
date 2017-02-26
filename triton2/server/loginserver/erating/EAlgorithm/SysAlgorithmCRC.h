#ifndef __SYS_ALGORITHM_CRC_H__
#define __SYS_ALGORITHM_CRC_H__
#include "Macro.h"


class Engine_Export SysAlgorithmCRC
{
public:
    SysAlgorithmCRC(void);
public:
    virtual ~SysAlgorithmCRC(void);
    static int compute(const void *pInput, int32_t nInputLength, uint16_t usCrcInit, uint16_t *pusCrcResult);
};
#endif

