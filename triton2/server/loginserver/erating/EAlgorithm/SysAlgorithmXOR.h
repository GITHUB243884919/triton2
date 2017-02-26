#ifndef __SYS_ALGORITHM_XOR_H__
#define __SYS_ALGORITHM_XOR_H__
#include "Macro.h"

class Engine_Export SysAlgorithmXOR
{
public:
    SysAlgorithmXOR(void);
public:
    virtual ~SysAlgorithmXOR(void);
    int compute(const void *pInput, size_t stInputLength, uint8_t ucXorInit, uint8_t *pucXorResult);
};
#endif
