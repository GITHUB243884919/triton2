#ifndef __SYS_ALGORITHM_MD5_H__
#define __SYS_ALGORITHM_MD5_H__

#include "Macro.h"

#define BLKSIZE 64

class Engine_Export SysAlgorithmMD5
{
private:
    typedef struct
    {
        uint32_t h[4];
    }MD5_Core_State;

    struct MD5Context
    {
        MD5_Core_State core;
        uint8_t block[64];
        int32_t blkused;
        uint32_t lenhi, lenlo;
    };

public:
    SysAlgorithmMD5(void);
    virtual ~SysAlgorithmMD5(void);

    static int compute(const void *pInput, int32_t nInputLength, void *pDigest);

private:
    static void MD5_Block(MD5_Core_State * s, uint32_t * block);
    static void MD5_Core_Init(MD5_Core_State * s);
    static void MD5Final(uint8_t output[16], struct MD5Context *s);
    static void MD5Update(struct MD5Context *s, const uint8_t *p, uint32_t len);
    static void MD5Init(struct MD5Context *s);
};

#endif
