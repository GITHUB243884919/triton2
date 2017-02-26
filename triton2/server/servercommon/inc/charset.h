#ifndef _CHARSET_H_
#define _CHARSET_H_

// 下面代码摘自mysql5的源码，如果字符集发生变化，需要从ctype-xxx.c中找到对应的代码
// 目前只支持GBK和UTF-8两种字符集

/* wm_wc and wc_mb return codes */
#define MY_CS_ILSEQ     0     /* Wrong by sequence: wb_wc                   */
#define MY_CS_ILUNI     0     /* Cannot encode Unicode to charset: wc_mb    */
#define MY_CS_TOOSMALL  -101  /* Need at least one byte:    wc_mb and mb_wc */
#define MY_CS_TOOSMALL2 -102  /* Need at least two bytes:   wc_mb and mb_wc */
#define MY_CS_TOOSMALL3 -103  /* Need at least three bytes: wc_mb and mb_wc */
/* These following three are currently not really used */
#define MY_CS_TOOSMALL4 -104  /* Need at least 4 bytes: wc_mb and mb_wc */
#define MY_CS_TOOSMALL5 -105  /* Need at least 5 bytes: wc_mb and mb_wc */
#define MY_CS_TOOSMALL6 -106  /* Need at least 6 bytes: wc_mb and mb_wc */


// gbk
#define isgbkhead(c) (0x81<=(uchar)(c) && (uchar)(c)<=0xfe)
#define isgbktail(c) ((0x40<=(uchar)(c) && (uchar)(c)<=0x7e) || (0x80<=(uchar)(c) && (uchar)(c)<=0xfe))


typedef unsigned char uchar;
typedef unsigned long ulong;
typedef unsigned int uint;

#define USE_MB 1
#define TRUE 1
#define my_wc_t ulong

ulong escape_string_for_mysql(char *to, ulong to_length, const char *from, ulong length);


#endif // _CHARSET_H_