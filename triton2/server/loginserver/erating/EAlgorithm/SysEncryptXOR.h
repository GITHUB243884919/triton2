#ifndef __SYS_ENCRYPT_XOR_H__
#define __SYS_ENCRYPT_XOR_H__
#include "Macro.h"


class Engine_Export SysEncryptXOR
{
public:
    SysEncryptXOR(void);
public:
    virtual ~SysEncryptXOR(void);
    int encrypt(
        const uint8_t *pucInput,
        int32_t        nInputLength,
        uint8_t        ucXORInit,
        uint8_t       *pucEncryptOutput,
        int32_t       *pnOutputLength
    );
    int decrypt(
        const uint8_t *pucInput,
        int32_t        nInputLength,
        uint8_t        ucXORInit,
        uint8_t       *pucDecryptOutput,
        int32_t       *pnOutputLength
    );

};

#endif
