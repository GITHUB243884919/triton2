#ifndef _BASE_H_
#define _BASE_H_

#include <netinet/in.h>
#include <sys/file.h>
#include <sys/types.h>
#include <vector>
#include <string>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>


#ifdef USE_COREDUMPER
#include "google/coredumper.h"
#endif

#define LOOPBACK_IF_NAME "lo"   
#define MAX_NET_CARDS 16     
#define MAC_ADDR_LEN  16

#define	MAX_CODE_LEN	64000  // 服务器间最大消息包长度

#ifdef _DEBUG_
#define RECVBUFLENGTH	4096000
#define POSTBUFLENGTH	4096000
#else
#define RECVBUFLENGTH	4096000*6
#define POSTBUFLENGTH	4096000*6
#endif


#define MINPROXYMSGLEN	10 // 最小proxy消息长度(一般是控制类型的消息)

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef False
#define	False  0
#endif

#ifndef True
#define	True   1
#endif

struct timeval;

#define I64FMTD "%llu"

typedef __int64_t   int64;
typedef __int32_t   int32;
typedef __int16_t   int16;
typedef __int8_t    int8;
typedef __uint64_t  uint64;
typedef __uint32_t  uint32;
typedef __uint16_t  uint16;
typedef __uint8_t   uint8;


typedef unsigned char BYTE;
typedef unsigned char byte;



typedef char TName[32];

typedef char TFName[64];


#define MAX_SERVER_TRANS_LEN        64000


#define __LK_FILE__	( ( strrchr( __FILE__, '/' ) == NULL ) ? __FILE__ :  strrchr( __FILE__, '/' ) + 1 )


// 是否使用google开源的coredumper库
#ifdef USE_COREDUMPER
#define WRITE_COREDUMP WriteCoreDump
#else
#define WRITE_COREDUMP
#endif

typedef enum
{
	Block = 0,
	NBlock = 1
} TBlockFlag;



typedef struct
{
	int m_iYear;
	int m_iMon;
	int m_iDay;
	int m_iHour;
	int m_iMin;
	int m_iSec;
	int m_iMSec;
} TStruTime;

// 把当前时间(秒)转换为可识别的日期数字
int GetStructTime(time_t tTime, TStruTime *pstTime);
void TrimStr( char *strInput );//分割字符串，获取单词
int TimeValPlus(timeval& tvA, timeval& tvB, timeval& tvResult);
int TimeValMinus(timeval& tvA, timeval& tvB, timeval& tvResult);
int TimeValMinus(timeval& tvA, timeval& tvB);

// 把 YYYY-MM-DD HH:MM:SS 格式的字符串时间转换成time_t
time_t TimeStrToRealTime( const char* pszTimeStrs );

//const char* GetTimeString( time_t& time, char* buff );


int SplitSockAddr( const char* vAddress, char* vIP, short& vPort );
int SockAddrToString(sockaddr_in *pstSockAddr, char *szResult);
int SplitSockAddr( const char* vAddress, char* vIP, short& vPort );


typedef std::vector<std::string> Tokens;
Tokens StrSplit(const std::string &src, const std::string &sep);


// Linux平台下返回微秒
namespace LINEKONG
{
	uint64_t GetTickCount();
}

#ifndef IS_DOUBLE_CHAR
#define IS_DOUBLE_CHAR(ch) 		( ch>=0xA0&&ch<0xFF)
#endif

// 过滤字符串中的转义字符
int EscapeDataString(char **dest, int iDstLen, char *src , int iSrcLen);

// 生成随机密钥
int GetRandomKey( char *src, int len );



/************************************************************************/
/* 时间点判断函数                                                                     */
/************************************************************************/
//一天的秒数
#define SECONDS_OF_DAY   (60*60*24)
//一小时的秒数
#define SECONDS_OF_HOUR   (60*60)
//一个星期的天数，具体星期1-6分别为对应数值，而星期日为0
#define WEEK_DAY_NUM                     7
//time函数起始时间的星期数，即1970.1.1日为星期四
#define WEEK_DAY_OFC_OMPUTETIME_BEGIN              4

// 取得UTC时间
int GetUTCAndLocalDiffSec();

// 是否是新的一天
bool IsANewDay(time_t tOldTime,  time_t tNewTime);

// 是否是新的一周
bool IsANewWeek(time_t tOldTime, time_t tNewTime);

// 是否是新的一月
bool IsANewMonth(time_t tOldTime, time_t tNewTime);

// 得到当前是星期几
int GetTimeWeekNum(time_t tTime);

// 标准time_t时间和当天的字符串的HH:MM:SS比较 0-相等；>0-大于; <0-小于
int IntTimeCompareToStringTime( time_t tNow, const char* pszTimeStr );

// 字符集转换函数
int charset_convert(const char* from_charset, const char* to_charset, char* inbuf, size_t inlen, char* outbuf, size_t* poutlen);

// 打印函数堆栈到文件,配合log4cpp日志使用,非线程安全
void print_trace_fd ( const char* fd_name );

// 获取主机信息
bool GetNicInfoForLinux( char *pMacBuffer, char *pIpBuffer );

// 创建一个文件夹
int CreatDir( const char *pDirName, int nMode );

// 计算UTF-8格式字符串的字符个数 
int GetUTF8CharLen(char* pcUTF8String, int nBufferLen);

int CalSum(const char* from, size_t len);

int get_file_length( const char* file );

// 用于数据库用户名/密码加解密
bool DecryptAscIICode(const char* szInput, int nInput, char* szOutput, int* pnOutput);

// 16进制字符串转换为整型; 0成功; !0失败
int HexToDec( const char* shex, int& idec );


#define SHMKEY_HEX_STR_LEN 8  // 共享内存hex类型key长度

// 从文件生成贡献内存用的key
int MakeShmKey( const char* pFile, char cID );

#endif



