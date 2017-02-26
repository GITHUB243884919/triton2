#ifndef __SYS_ENCRYPT_CONFIG_H__
#define __SYS_ENCRYPT_CONFIG_H__

#include "Macro.h"

class Engine_Export SysEncryptConfig
{
public:
    SysEncryptConfig(void);
public:
    virtual ~SysEncryptConfig(void);
    static int encrypt(
        const void    *pInput,
        int32_t        nInputLength,
        const void    *pKey,
        int32_t        nKeyLength,
        void          *pucEncryptOutput,
        int32_t        nOutputBufferSize,
        int32_t       *pnOutputLength
    );

    static int decrypt(
        const void    *pInput,
        int32_t        nInputLength,
        const void    *pKey,
        int32_t        nKeyLength,
        void          *pDecryptOutput,
        int32_t        nOutputBufferSize,
        int32_t       *pnOutputLength
    );
    
private:
    static uint16_t m_arrusDefaultKey[];

    static int crc(const uint8_t *pucInput, int32_t nInputLength, uint8_t *pucCrcResult);
};

#endif
