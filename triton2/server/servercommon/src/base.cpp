
#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include "charset.h"
#include <math.h>
#include <iconv.h>
#include <execinfo.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <dlfcn.h>
#include <pthread.h>
#include <stdarg.h>
#include <bits/stl_algo.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include "log.h"
#include "servertool.h"
#include "charset.h"
#include "client_comm_engine.h"

// 把当前时间(秒)转换为可识别的日期数字
int GetStructTime(time_t tTime, TStruTime *pstTime)
{
	struct tm *pTempTm = NULL;
	struct tm stTempTm;
	time_t timer = tTime;

	if( !pstTime )
	{
		return -1;
	}

	pTempTm = localtime_r( &timer, &stTempTm );

	if( !pTempTm )
	{
		return -1;
	}

	pstTime->m_iYear = stTempTm.tm_year + 1900;
	pstTime->m_iMon = stTempTm.tm_mon + 1;
	pstTime->m_iDay = stTempTm.tm_mday;
	pstTime->m_iHour = stTempTm.tm_hour;
	pstTime->m_iMin = stTempTm.tm_min;
	pstTime->m_iSec = stTempTm.tm_sec;
	pstTime->m_iMSec = 0;

	return 0;
}

int TimeValPlus(timeval& tvA, timeval& tvB, timeval& tvResult)
{
	timeval tvTemp;
	tvTemp.tv_sec = tvA.tv_sec + tvB.tv_sec;
	tvTemp.tv_sec += ((tvA.tv_usec+tvB.tv_usec)/1000000);
	tvTemp.tv_usec = ((tvA.tv_usec+tvB.tv_usec)%1000000);

	tvResult.tv_sec = tvTemp.tv_sec;
	tvResult.tv_usec = tvTemp.tv_usec;

	return 0;
}

int TimeValMinus(timeval& tvA, timeval& tvB, timeval& tvResult)
{
	timeval tvTemp;

	if( tvA.tv_usec < tvB.tv_usec )
	{
		tvTemp.tv_usec = (1000000 + tvA.tv_usec) - tvB.tv_usec;
		tvTemp.tv_sec = tvA.tv_sec - tvB.tv_sec - 1;
	}
	else
	{
		tvTemp.tv_usec = tvA.tv_usec - tvB.tv_usec;
		tvTemp.tv_sec  = tvA.tv_sec - tvB.tv_sec;
	}

	tvResult.tv_sec = tvTemp.tv_sec;
	tvResult.tv_usec = tvTemp.tv_usec;

	return 0;
}

int TimeValMinus(timeval& tvA, timeval& tvB)
{
	timeval tvTemp;
	int iInterval = 0;

	TimeValMinus(tvA, tvB, tvTemp);
	iInterval = tvTemp.tv_sec * 1000 + tvTemp.tv_usec / 1000;

	return iInterval;
}

int SplitSockAddr( const char* vAddress, char* vIP, short& vPort  )
{
	if ( vAddress == 0 || vIP == 0 )
	{
		return -1;
	}

	const char* tpPort = ::strchr( vAddress, ':' );

	if ( tpPort != 0 )
	{
		::memcpy( vIP, vAddress, tpPort - vAddress );

		vPort = (short)( ::atoi( tpPort + 1 ) );
	}
	
	return 0;
}

int SockAddrToString(sockaddr_in *pstSockAddr, char *szResult)
{
	char *pcTempIP = NULL;
	unsigned short nTempPort = 0;

	if( !pstSockAddr || !szResult )
	{
		return -1;
	}

	pcTempIP = inet_ntoa(pstSockAddr->sin_addr);

	if( !pcTempIP )
	{
		return -1;
	}

	nTempPort = ntohs(pstSockAddr->sin_port);

	sprintf(szResult, "%s:%d", pcTempIP, nTempPort);

	return 0;
}


void TrimStr( char *strInput )//分割字符串，获取单词
{
	char *pb;
	char *pe;
	int iTempLength;
	if( strInput == NULL )
	{
		return;
	}

	iTempLength = strlen(strInput);

	if( iTempLength == 0 )
	{
		return;
	}

	pb = strInput;

	while (((*pb == ' ') || (*pb == '\t') || (*pb == '\n') || (*pb == '\r')) && (*pb != 0))
	{
		pb ++;//当遇到控制字符的时候字符串指针++
	}

	pe = &strInput[iTempLength-1];  //pe指针指向strInput的最后一个字符

	while ((pe >= pb) && ((*pe == ' ') || (*pe == '\t') || (*pe == '\n') || (*pe == '\r')))
	{
		pe --;
	}
	*(pe+1) = '\0';

	strcpy( strInput, pb );

	return;

}//拷贝字符串里面的单词（以空格回车换行等隔开）到pb里面


Tokens StrSplit(const std::string &src, const std::string &sep)
{
	Tokens r;
	std::string s;
	for (std::string::const_iterator i = src.begin(); i != src.end(); i++)
	{
		if (sep.find(*i) != std::string::npos)
		{
			if (s.length()) r.push_back(s);
			s = "";
		}
		else
		{
			s += *i;
		}
	}
	if (s.length()) r.push_back(s);
	return r;
}



// 时间函数
namespace LINEKONG
{

#ifdef WIN32
uint64_t GetTickCount()
{
	return ::GetTickCount();
}
#else
#   include <sys/time.h>
uint64_t GetTickCount()
{
	struct timeval tmval = {0};
	int nRetCode = gettimeofday(&tmval, NULL);
	if (nRetCode != 0)
	{
		return 0;
	}
	return ((uint64_t)tmval.tv_sec) * 1000 + tmval.tv_usec / 1000;
}
#endif

}

#define IS_DOUBLE_CHAR(ch) 		( ch>=0xA0&&ch<0xFF)
#define IS_GBK_DOUBLE_CHAR(ch)	( ch>0x80&&ch<0xff)

//
//// 过滤字符串中的转义字符
//int EscapeDataString2(char **dest, char *src , int iSrcLen)
//{
//	int i, j;
//	char *pTemp ;
//
//	i = j = 0;
//	pTemp = *dest ;
//	while (i < iSrcLen ) {
//		
//		if( IS_GBK_DOUBLE_CHAR( (unsigned char)src[i] ) )
//		{
//			pTemp[j++] = (unsigned char)src[i++];
//			pTemp[j++] = (unsigned char)src[i++];
//			continue;
//		}
//
//		switch((unsigned char)src[i]) {
//			case '\0':
//				pTemp[j++] = '\\';
//				pTemp[j++] = '0';
//				break;
//			case '\n':
//				pTemp[j++] = '\\';
//				pTemp[j++] = 'n';
//				break;
//			case '\r':
//				pTemp[j++] = '\\';
//				pTemp[j++] = 'r';
//				break;
//			case 26: /* '\Z' */
//				pTemp[j++] = '\\';
//				pTemp[j++] = 'Z';
//				break;
//			case '\'':
//				pTemp[j++] = '\\';
//				pTemp[j++] = '\'';
//				break;
//			case '\\':
//				pTemp[j++] = '\\';
//				pTemp[j++] = '\\';
//				break ;
//			case '\"':
//				pTemp[j++] = '\\';
//				pTemp[j++] = '\"';
//				break;
//
//			default:
//				pTemp[j++] = src[i];
//		}
//		i++;
//	}
//
//	(*dest) += j ;
//
//	return j;
//}


// ***************************************************************
//  Function:		EscapeDataString
//  Description:	封装mysql原生escape_string_for_mysql接口，过滤buff中的转义字符,
//					附带转换字符集功能，这里是GBK
//	Input:			dest - 目的buff地址	， iDstLen - 目的buff长度
//					src - 源buff, iSrcLen - 源buff长度
//	OutPut:			无
//	Return:			~0(-1) 错误 ; 否则 成功
//	Others:			【警告】如果换了字符集，一定要修改此函数
//  Date:			10/27/2008
// 
// ***************************************************************
int EscapeDataString(char **dest, int iDstLen, char *src , int iSrcLen)
{
	return escape_string_for_mysql(*dest, iDstLen, src, iSrcLen);
}


//// 把时间转换成字符串写到buff中，buff最少64Bytes
//const char* GetTimeString( time_t& time, char* buff )
//{
//	struct tm newtime;   
//	localtime_r( &time, &newtime ); 
//
//	int year	= newtime.tm_year+1900;   
//	int month	= newtime.tm_mon+1;   
//	int day		= newtime.tm_mday;     
//	int hour	= newtime.tm_hour;     
//	int minute	= newtime.tm_min;     
//	int seconds	= newtime.tm_sec;
//
//	snprintf(buff, 64, "[%04d-%02d-%02d %02d:%02d:%02d]", year, month, day, hour, minute, seconds );
//	
//	return buff;
//}


// ***************************************************************
//  Function:		GetRandomKey
//  Description:	生成长度16的随机密钥
//	Input:			src - 源buff, len - 源buff长度
//	OutPut:			无
//	Return:			~0(-1) 错误 ; 否则 成功
//	Others:			
//  Date:			10/2/2009
// 
// ***************************************************************

int GetRandomKey( char *src, int len )
{
	for( int i = 0; i < len; i++ )
	{
		src[ i ] = '\001' + rand() % 255;
	}
	return 0;
}


/************************************************************************/
/* 时间点判断函数                                                                     */
/************************************************************************/

int GetUTCAndLocalDiffSec()
{
	/*因为虚拟机存在问题,所以采用新方法得到时区差
	struct timezone tvTimeZone;
	gettimeofday(NULL, &tvTimeZone);

	//将西区时间转化为东区，time()得到的时间加上此时间即可    
	return (-tvTimeZone.tz_minuteswest*60)	;
	*/
	struct tm *pTempTm = NULL;
	time_t tSecondDay = SECONDS_OF_HOUR;

	pTempTm = localtime(&tSecondDay);
	if( !pTempTm )
	{
		return 0;
	}

	int iDiffSeconds = (pTempTm->tm_mday -2)*SECONDS_OF_HOUR
		+pTempTm->tm_hour*SECONDS_OF_HOUR;

	return iDiffSeconds;
}



// 是否是新的一天
bool IsANewDay(time_t tOldTime,  time_t tNewTime)
{
	int iTimeZoneSeconds = GetUTCAndLocalDiffSec();
	int iDayOld = (tOldTime+iTimeZoneSeconds)/SECONDS_OF_DAY;
	int iDayNew = (tNewTime+iTimeZoneSeconds)/SECONDS_OF_DAY;
	if(iDayNew > iDayOld)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 是否是新的一周
bool IsANewWeek(time_t tOldTime, time_t tNewTime)
{
	int iTimeZoneSeconds = GetUTCAndLocalDiffSec();
	int iDayOld = (tOldTime+iTimeZoneSeconds)/SECONDS_OF_DAY;
	int iDayNew = (tNewTime+iTimeZoneSeconds)/SECONDS_OF_DAY;
	int iWeekOld = (iDayOld + WEEK_DAY_OFC_OMPUTETIME_BEGIN-1)/WEEK_DAY_NUM;
	int iWeekNew = (iDayNew + WEEK_DAY_OFC_OMPUTETIME_BEGIN-1)/WEEK_DAY_NUM;
	if(iWeekNew > iWeekOld)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// 是否是新的一月
bool IsANewMonth(time_t tOldTime, time_t tNewTime)
{
	TStruTime stOldTime = {0}, stNewTime = {0};
	GetStructTime(tOldTime, &stOldTime);
	GetStructTime(tNewTime, &stNewTime);
	if((stNewTime.m_iYear > stOldTime.m_iYear)
		|| ((stNewTime.m_iYear == stOldTime.m_iYear) && (stNewTime.m_iMon > stOldTime.m_iMon)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 得到当前是星期几
int GetTimeWeekNum(time_t tTime)
{
	int iTimeZoneSeconds = GetUTCAndLocalDiffSec();
	int iDay = (tTime+iTimeZoneSeconds)/SECONDS_OF_DAY;
	int iWeekIdx = (iDay + WEEK_DAY_OFC_OMPUTETIME_BEGIN-1)%WEEK_DAY_NUM;

	return iWeekIdx+1;
}

// 字符集转换
int charset_convert(const char* from_charset, const char* to_charset, char* inbuf, size_t inlen, char* outbuf, size_t* poutlen)
{  
	iconv_t   cd; 
	//int rc;
	char **pin = &inbuf;
	char **pout = &outbuf; 

	cd = iconv_open(to_charset, from_charset);  
	if   (cd==0)   return   -1; 

	memset(outbuf, 0, *poutlen);  

	if( iconv(cd, pin, &inlen, pout, poutlen) == (size_t) -1 ) 
	{
		iconv_close(cd);
		return -1;  
	}

	iconv_close(cd);

	return   0;  
}


// 获取主机信息
bool GetNicInfoForLinux(  char *pMacBuffer, char *pIpBuffer )
{   
	int skfd = -1, if_cnt = 0, if_idx = 0;;   

	struct ifreq if_buf[MAX_NET_CARDS];   
	struct ifconf ifc;   

	/*char mac_addr[MAC_ADDR_LEN];  */ 
	int netmask = 0;   

	bool ip_get = FALSE;
	bool netmask_get = FALSE;   

	// struct list_head *ip_info_list = NULL;   

	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)   
	{   
		// LOG_ERROR("Get NIC info, Create socket error");   
		return FALSE;   
	}   

	ifc.ifc_len = sizeof(if_buf);   
	ifc.ifc_buf = (char *)if_buf;   

	if (!ioctl(skfd, SIOCGIFCONF, (char *)&ifc))   
	{   
		if_cnt = (ifc.ifc_len) / (sizeof(struct ifreq));   

		for (if_idx = 0; if_idx < if_cnt; ++if_idx)   
		{   
			/* ignore local loopback interface */  
			if (strcmp(LOOPBACK_IF_NAME, if_buf[if_idx].ifr_name) == 0)   
				continue;   

			/* get interface flags */  
			if (!(ioctl(skfd, SIOCGIFFLAGS, (char *)(&if_buf[if_idx]))))   
			{   
				/* this interface is up and running */  
				if ((if_buf[if_idx].ifr_flags & IFF_UP) &&   
					(if_buf[if_idx].ifr_flags & IFF_RUNNING))   
				{   
					if (!(ioctl(skfd, SIOCGIFHWADDR, (char *)(&if_buf[if_idx]))))  
					{   
						if ( pMacBuffer != NULL )
						{
							sprintf( pMacBuffer,"%02X-%02X-%02X-%02X-%02X-%02X",
									(unsigned char )if_buf[if_idx].ifr_hwaddr.sa_data[0],
									(unsigned char )if_buf[if_idx].ifr_hwaddr.sa_data[1],  
									(unsigned char )if_buf[if_idx].ifr_hwaddr.sa_data[2],
									(unsigned char )if_buf[if_idx].ifr_hwaddr.sa_data[3],
									(unsigned char )if_buf[if_idx].ifr_hwaddr.sa_data[4],
									(unsigned char )if_buf[if_idx].ifr_hwaddr.sa_data[5] );
						}				

						/* add new NIC */  
						// ip_info_list = AddNicToNewNicInfoList(mac_addr, MAC_ADDR_LEN, &g_newNicInfoList);   
					}   

					/* get ip address of this NIC */  
					if (!(ioctl(skfd, SIOCGIFADDR, (char *)(&if_buf[if_idx]))))   
					{   
						unsigned char *tpIPAddr = (unsigned char *)&( (struct sockaddr_in *) (&(if_buf[if_idx].ifr_addr)))->sin_addr.s_addr;   
						if ( pIpBuffer != NULL )
						{							
							sprintf( pIpBuffer, "%d.%d.%d.%d", *tpIPAddr, *( tpIPAddr + 1 ), *( tpIPAddr + 2 ),*( tpIPAddr + 3 ) );
						}
						// ip_get = TRUE;   
					}   

					/* get ip netmask */  
					if (!(ioctl(skfd, SIOCGIFNETMASK, (char *)(&if_buf[if_idx]))))   
					{   
						netmask = ((struct sockaddr_in *)(&(if_buf[if_idx].ifr_netmask)))->sin_addr.s_addr;   

						// netmask_get = TRUE;   
					}   

					/* insert ip to ip list */  
					if (( ip_get == TRUE  ) && (  netmask_get == TRUE ))   
					{   
						// AddIpToIpList(ip, netmask, ip_info_list);   

						ip_get = FALSE;   
						netmask_get = FALSE;   
					}   
				}   
			}   
		}   /* end of for (if_idx = 0; if_idx < if_cnt; ++if_idx) */  
	}   

	close(skfd);   

	return TRUE;   
}  


// 打印堆栈
void print_trace_fd ( const char* fd_name )
{

#ifndef _DEBUG_
	// release版本下确保不会因为频繁打印堆栈导致程序无法运行
	static time_t tWriteTime = time(NULL);
	static int tWriteCount = 0;

	if( ++tWriteCount > 1 )
	{
		// 允许连续打印2次堆栈
		if( tWriteTime < ( time(NULL) - 30 ) )
		{
			tWriteTime = time(NULL);
			tWriteCount = 0;
		}
		else
		{
			return;
		}
	}

#endif


	void *array[32] = {0};
	size_t size = 0;
	char **strings = NULL;

	size = backtrace (array, 32);
	strings = backtrace_symbols (array, size);

	LOG_NOTICE( fd_name, "=== obtained %zd stack frames ===", size);

	for (size_t i = 0; i < size; i++)
		LOG_NOTICE( fd_name, "#%02d %s", i, strings[i]);

	LOG_NOTICE( fd_name, " ", size);

	free (strings);
}

// 创建一个文件夹
int CreatDir( const char *pDirName, int nMode )
{
	struct stat tDirStat;
	int tRet = stat( pDirName, &tDirStat );	
	
	if ( tRet < 0 )
	{
		return mkdir( pDirName, nMode );		
	}	

	if( S_ISDIR( tDirStat.st_mode ) )
	{
		return 0;		
	}
	
	return mkdir( pDirName, nMode );
}


//************************************
// Method:    GetUTF8CharLen
// FullName:  GetUTF8CharLen
// Access:    public 
// Returns:   int UTF8类型字符个数
// Qualifier: 计算UTF-8格式字符串的字符个数 
// Parameter: char * pcUTF8String  (传入的必须是UTF8类型的串)
// Parameter: int nBufferLen 推荐用一个比较大的长度，不推荐使用strlen, 不安全
//************************************
int GetUTF8CharLen(char* pcUTF8String, int nBufferLen)
{
	int nCharLen = 0;
	if (pcUTF8String == NULL || nBufferLen <= 0)
	{
		return 0;
	}
	while (nBufferLen > 0 && *pcUTF8String != 0)
	{
		if ((BYTE)(pcUTF8String[0] & 0xE0) == (BYTE)0xE0)
		{
			nCharLen ++;
			pcUTF8String += 3;
			nBufferLen -= 3;
			continue;
		}
		else if ((BYTE)(pcUTF8String[0] & 0xC0) == (BYTE)0xC0)
		{
			nCharLen ++;
			pcUTF8String += 2;
			nBufferLen -= 2;
			continue;
		}
		else
		{
			nCharLen ++;
			pcUTF8String += 1;
			nBufferLen -= 1;
		}
	}
	return nCharLen;
}

int CalSum(const char* from, size_t len)
{
	int nCheckSum = 0; 

	const unsigned char* p = (const unsigned char*) from;
	for (size_t i =0; i < len; ++i, ++p) nCheckSum += (*p);

	return nCheckSum;
}


int get_file_length( const char* file )
{
	int rtn = 0;
	struct stat buf;
	rtn = stat( file, &buf );
	if( rtn == 0 )
	{
		return buf.st_size;
	}

	return 0;
}



bool DecryptAscIICode(const char* szInput, int nInput, char* szOutput, int* pnOutput)
{
	static int tKey[ 4 ] = {4356,78534,46677,3545};
	static unsigned char* tpKey = (unsigned char*)&tKey[0];

	int nBinaryLen = 0;
	unsigned char sBinaryCode[255+1] = {0};

	size_t nAscIILen = strlen(szInput);

#define CHARTOHEX( c ) (isdigit(( c )) ? (( c ) - '0') : (tolower(( c )) - 'a' + 10))
	for (size_t i= 0; i+2 <= nAscIILen; i=i+2, ++nBinaryLen)
	{
		if( isxdigit(szInput[i]) && isxdigit(szInput[i+1]) )
		{
			sBinaryCode[nBinaryLen] = CHARTOHEX(szInput[i]) * 16 + CHARTOHEX( szInput[i+1] );
		}else
		{
			sBinaryCode[nBinaryLen] = 0;
		}
	}
#undef  CHARTOHEX

	int nResult = oi_symmetry_decrypt( sBinaryCode, nBinaryLen, tpKey, (unsigned char*) szOutput, pnOutput);
	if ( nResult == FALSE )
	{
		return  false;
	}

	szOutput[*pnOutput] = 0;
	return true;
}



// 把 YYYY-MM-DD HH:MM:SS 格式的字符串时间转换成time_t
time_t TimeStrToRealTime( const char* szTimeStr )
{
	if( szTimeStr == NULL )
	{
		return time(0);
	}

	char *str1, *str2, *token, *subtoken;
	char *saveptr1, *saveptr2;
	char timestr_buffer[32] = {0};
	strncpy( timestr_buffer, szTimeStr, sizeof(timestr_buffer) );
	
	char subtoken_buffer[6][8] = {{0}};
	int subtoken_num = 0;

	for( str1 = timestr_buffer; ; str1 = NULL )
	{
		token = strtok_r(str1, " ", &saveptr1);
		if (token == NULL)
			break;

		for (str2 = token; ; str2 = NULL) 
		{
			subtoken = strtok_r(str2, "-:", &saveptr2);
			if (subtoken == NULL)
				break;

			strncpy( subtoken_buffer[subtoken_num++], subtoken, 8 );
		}
	}

	if ( subtoken_num != 6 )
		return time(0);


	struct tm stTempTm;

	stTempTm.tm_year = atoi( subtoken_buffer[0] ) - 1900;
	stTempTm.tm_mon = atoi( subtoken_buffer[1] ) - 1;
	stTempTm.tm_mday = atoi( subtoken_buffer[2] );
	stTempTm.tm_hour = atoi( subtoken_buffer[3] );
	stTempTm.tm_min = atoi( subtoken_buffer[4] );
	stTempTm.tm_sec = atoi( subtoken_buffer[5] );
	
	return mktime( &stTempTm );

}


int IntTimeCompareToStringTime( time_t tNow, const char* pszTimeStr )
{
	if( pszTimeStr == NULL )
	{
		return 1;
	}

	struct tm *pTempTm = NULL;
	time_t timer = time(NULL);
	pTempTm = localtime( &timer );

	sscanf( pszTimeStr, "%d:%d:%d", &pTempTm->tm_hour, &pTempTm->tm_min, &pTempTm->tm_sec );

	if( tNow == mktime( pTempTm ) ) 
	{
		return 0;
	}

	return ( tNow > mktime( pTempTm ) ) ? 1 : -1 ;

}


// 16进制字符串转换为整型; 0成功; !0失败
int HexToDec( const char* shex, int& idec ) 
{
	int   i,mid;
	int   len   =   strlen(   shex   );
	if(   len>8   )     return   -1;   
	mid   =   0;   idec   =   0;
	for(   i=0;i<len;i++   )
	{
		if(   shex[i]>='0'&&shex[i]<='9'   )   mid   =   shex[i]-'0';
		else   if(   shex[i]>='a'&&shex[i]<='f'   )   mid   =   shex[i]   -'a'   +10;
		else   if(   shex[i]>='A'&&shex[i]<='F'   )   mid   =   shex[i]   -'A'   +10;
		else   return  -1;
		mid   <<=   ((len-i-1)<<2);
		idec   |=   mid;
	}
	return   0;   
}  


int MakeShmKey( const char* pFile, char cID )
{
	// 从pipe文件读出 key, key格式是16进制，字符串长度8字节
	char szKey[SHMKEY_HEX_STR_LEN+1] = {0};
	FILE *pPipeFile = fopen( pFile, "rb" );
	if( pPipeFile )
	{
		fseek( pPipeFile, 0L, SEEK_END );
		long lFileLen = ftell( pPipeFile );
		fseek( pPipeFile, 0L, SEEK_SET );

		if( lFileLen > SHMKEY_HEX_STR_LEN )
		{
			fread( &szKey, SHMKEY_HEX_STR_LEN , 1, pPipeFile  );
			LOG_NOTICE( "default", "read key %s from file %s", szKey, pFile );
		}
		fclose( pPipeFile );

	}

	int tKey = 0;

	if( szKey[0] != 0 )
	{
		HexToDec( szKey, tKey );
		return tKey;
	}
	else
	{
		return ftok( pFile, cID);
	}
}