// =====================================================================================
// 
//       Filename:  base64.h
// 
//    Description:  Base64±àÂëÍ·ÎÄ¼þ
// 
//        Version:  1.0
//        Created:  08/24/2009 01:43:49 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Wang Xiaofeng (xfun68), xfun68@gmail.com
//        Company:  Linekong
// 
// =====================================================================================

#ifndef _SYSALGORITHM_H_
#define _SYSALGORITHM_H_

#include <Macro.h>

Engine_Export int base64_encode(const unsigned char* inbuf,
    size_t inbuf_len,
    unsigned char* outbuf,
    size_t outbuf_size,
    size_t& outbuf_len);

Engine_Export int base64_decode(const unsigned char* inbuf,
    size_t inbuf_len,
    unsigned char* outbuf,
    size_t outbuf_size,
    size_t& outbuf_len);

#endif //  _SYSALGORITHM_H_

