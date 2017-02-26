// =====================================================================================
// 
//       Filename:  SysAlgorithmDES.h
// 
//    Description:  对openssl的DES加密算法的封装
// 
//        Version:  1.0
//        Created:  08/26/2009 12:44:54 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Wang Xiaofeng (xfun68), xfun68@gmail.com
//        Company:  Linekong
// 
// =====================================================================================

#ifndef _EALGORITHMDES_H_
#define _EALGORITHMDES_H_

#ifdef LINUX

#include <Macro.h>

#include <OS.h>
#include <openssl/des.h>

Engine_Export int32_t set_des_key(const unsigned char des_key[8]);

Engine_Export int32_t encrypt_des_ecb(const char* inbuf,
    size_t inbuf_len,
    char* outbuf,
    size_t outbuf_size,
    size_t& outbuf_len);

Engine_Export int32_t decrypt_des_ecb(const char* inbuf,
    size_t inbuf_len,
    char* outbuf,
    size_t outbuf_size,
    size_t& outbuf_len);

#endif // LINUX

#endif // _EALGORITHMDES_H_

