#pragma once
// ***************************************************************
//  File:	log.h   version:  1.0
//	Date:	05/23/2008
// 	Desc:	log的实现基于开源软件Log4Cpp-1.0, 在之上进行了封装和宏开关.
//			可以实现服务器常用的回卷文件格式的日志文件,支持日志分级.
//			对所有类型的日志进行统一管理,用名称进行
//  
//  Copyright (C) 2008 - LineKong Entertainment Technology Co.,Ltd
//						All Rights Reserved 
// ***************************************************************

// 日志系统开关

#include <stdarg.h>


#ifdef USE_LOG4CPP
#	define INIT_ROLLINGFILE_LOG		InitLog					// 初始化一种日志类型（基于回卷文件)
#	define LOG_SHUTDOWN_ALL			ShutdownAllLog()		// 关闭所有类型日志
#	define LOG_SHUTDOWN				ShutdownLog				// 根据日志名关闭日志
#ifdef _DEBUG_
#	define LOG_DEBUG(logname, fmt, ... )				LogDebug(logname, fmt, ##__VA_ARGS__ )
#else
#	define LOG_DEBUG(logname, fmt, ... )				
#endif // !_DEBUG_
#	define LOG_NOTICE(logname, fmt, ... )				LogNotice(logname, fmt, ##__VA_ARGS__ )
#	define LOG_INFO(logname, fmt, ... )					LogInfo(logname, fmt, ##__VA_ARGS__ )
#	define LOG_WARN(logname, fmt, ... )					LogWarn(logname, fmt, ##__VA_ARGS__ )
#	define LOG_ERROR(logname, fmt, ... )				LogError(logname, fmt, ##__VA_ARGS__ )
#	define LOG_FATAL(logname, fmt, ... )				LogFatal(logname, fmt, ##__VA_ARGS__ )
#	define LOG(logname, fmt, ... )						Log(logname, fmt, ##__VA_ARGS__ )
#	define RE_INIT_LOG(logname, fmt, ... )				ReInitLog
#else
#	define INIT_ROLLINGFILE_LOG(logname, fmt, ... )	
#	define LOG_SHUTDOWN_ALL(logname, fmt, ... )	
#	define LOG_SHUTDOWN(logname, fmt, ... )	
#	define LOG_NOTICE(logname, fmt, ... )
#	define LOG_DEBUG(logname, fmt, ... )
#	define LOG_INFO(logname, fmt, ... )	
#	define LOG_WARN(logname, fmt, ... )
#	define LOG_ERROR(logname, fmt, ... )
#	define LOG_FATAL(logname, fmt, ... )
#	define LOG(logname, fmt, ... )
#	define RE_INIT_LOG				
#endif // !USE_LOG4CPP


//	日志等级
// NOTSET <  DEBUG < INFO  < WARN < LEVEL_NOTICE < ERROR  < FATAL 
enum LogLevel
{	
	LEVEL_FATAL  = 0,
	LEVEL_ERROR  = 300,
	LEVEL_WARN   = 400,
	LEVEL_NOTICE = 500,
	LEVEL_INFO   = 600,
	LEVEL_DEBUG  = 700,
	LEVEL_NOTSET = 800,
};


// ***************************************************************
//  Function: 	InitLog   
//  Description:初始化一种类型的日志：如果该类型日志已存在，则重新初始化； 
//				如果不存在，则创建。
//  Date: 		05/23/2008
// 
// ***************************************************************
int InitLog( const char*	vLogName,						/*日志类型的名称(关键字,由此定位到日志文件)*/
			const char*		vLogDir,						/*文件名称(路径)*/
			LogLevel		vPriority = LEVEL_NOTSET,		/*日志等级*/
			unsigned int	vMaxFileSize = 10*1024*1024,	/*回卷文件最大长度*/
			unsigned int	vMaxBackupIndex = 40,			/*回卷文件个数*/
			bool			vAppend = true);				/*是否截断(默认即可)*/



// ***************************************************************
//  Function: 	InitLog   
//  Description:重新给已存在的日志赋值，但是不能改变日志的名称，以及定位的文件。
//  Date: 		05/23/2008
// 
// ***************************************************************
int ReInitLog( const char* vLogName, 
			  LogLevel		vPriority = LEVEL_NOTSET,		/*日志等级*/
			  unsigned int	vMaxFileSize = 10*1024*1024,	/*回卷文件最大长度*/
			  unsigned int	vMaxBackupIndex = 40);			/*回卷文件个数*/
						


// ***************************************************************
//  Function: 	ShutdownAllLog   
//  Description:关闭所有类新的日志，(包括文件句柄和清除相关对象),在程序退出前用.
//  Date: 		05/23/2008
// 
// ***************************************************************
int ShutdownAllLog( );

// ***************************************************************
//  Function: 	ShutdownLog   
//  Description:关闭日志( 根据日志名称 )
//  Date: 		07/25/2009
// 
// ***************************************************************
int ShutdownLog( const char *pLogName );


// ***************************************************************
//  Function: 	LogXXX   
//  Description:具体日志记录函数，写入创建时关联的文件.
//  Date: 		05/23/2008
// 
// ***************************************************************
int LogDebug	( const char* vLogName, const char* vFmt, ... );
int LogInfo		( const char* vLogName, const char* vFmt, ... );
int LogNotice	( const char* vLogName, const char* vFmt, ... );
int LogWarn		( const char* vLogName, const char* vFmt, ... );
int LogError	( const char* vLogName, const char* vFmt, ... );
int LogFatal	( const char* vLogName, const char* vFmt, ... );
int log			( const char* vLogName, int vPriority, const char* vFmt, ... );

void Log_bin( const char* vLogName, void* vBin, int vBinLen );

int LogDebug_va	( const char* vLogName, const char* vFmt, va_list ap);
int LogNotice_va( const char* vLogName, const char* vFmt, va_list ap);
int LogInfo_va	( const char* vLogName, const char* vFmt, va_list ap );
int LogWarn_va	( const char* vLogName, const char* vFmt, va_list ap );
int LogError_va	( const char* vLogName, const char* vFmt, va_list ap );
int LogFatal_va	( const char* vLogName, const char* vFmt, va_list ap );
int log_va		( const char* vLogName, int vPriority, const char* vFmt, va_list ap );


//int LogDebug	( const char* vFmt, ... );
//int LogInfo		( const char* vFmt, ... );
//int LogWarn		( const char* vFmt, ... );
//int LogError	( const char* vFmt, ... );
//int LogFatal	( const char* vFmt, ... );
//int log( int vPriority, const char* vFmt, ... );

