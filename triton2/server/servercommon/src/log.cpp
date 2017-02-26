#include "log.h"
#include <string.h>

#ifdef USE_LOG4CPP
#	ifdef LINUX
#		ifndef LOG4CPP_HAVE_GETTIMEOFDAY 
#			define LOG4CPP_HAVE_GETTIMEOFDAY
#		endif // LOG4CPP_HAVE_GETTIMEOFDAY
#	endif // LINUX

#	include	"log4cpp/Category.hh"
#	include	"log4cpp/RollingFileAppender.hh"
#	include	"log4cpp/LinekongLayout.hh"
#endif // USE_LOG4CPP

#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

// 初始一种类型的日志
int InitLog( const char* vLogName, 
			const char* vLogDir, 
			LogLevel vPriority,
			unsigned int vMaxFileSize, 
			unsigned int vMaxBackupIndex, 
			bool vAppend )
{

#ifdef USE_LOG4CPP
	if ( NULL == vLogName || NULL == vLogDir )
	{
		return -1;
	}

	// 已经存在该名称的实例
	log4cpp::Category* tpCategory = log4cpp::Category::exists( vLogName);
	if ( tpCategory != NULL )
	{
		// 1.删除所有的appender
		tpCategory->removeAllAppenders ();

		// 2.实例化一个layout 对象
		log4cpp::Layout* tpLayout = new log4cpp::LinekongLayout();
		// 3. 初始化一个appender 对象
		log4cpp::Appender* tpAppender = new log4cpp::RollingFileAppender(vLogName, vLogDir, vMaxFileSize, vMaxBackupIndex, vAppend );
		// 4. 把layout对象附着在appender对象上
		tpAppender->setLayout(tpLayout);
		// 5. 设置additivity为false，替换已有的appender
		tpCategory->setAdditivity(false);
		// 6. 把appender对象附到category上
		tpCategory->setAppender(tpAppender);
		// 7. 设置category的优先级，低于此优先级的日志不被记录
		tpCategory->setPriority( (log4cpp::Priority::PriorityLevel) vPriority );

		return 0;
	}

	// 1实例化一个layout 对象
	log4cpp::Layout* tpLayout = new log4cpp::LinekongLayout();
	// 2. 初始化一个appender 对象
	log4cpp::Appender* tpAppender = new log4cpp::RollingFileAppender(vLogName, vLogDir, vMaxFileSize, vMaxBackupIndex, vAppend );
	// 3. 把layout对象附着在appender对象上
	tpAppender->setLayout(tpLayout);
	// 4. 实例化一个category对象
	log4cpp::Category& trCategory = log4cpp::Category::getInstance(vLogName);
	// 5. 设置additivity为false，替换已有的appender
	trCategory.setAdditivity(false);
	// 6. 把appender对象附到category上
	trCategory.setAppender(tpAppender);
	// 7. 设置category的优先级，低于此优先级的日志不被记录
	trCategory.setPriority( (log4cpp::Priority::PriorityLevel) vPriority );

#endif

	return 0;

}

int ReInitLog( const char* vLogName, 
			  LogLevel		vPriority,		/*日志等级*/
			  unsigned int	vMaxFileSize,	/*回卷文件最大长度*/
			  unsigned int	vMaxBackupIndex)		/*回卷文件个数*/
{
#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	// 已经存在该名称的实例
	log4cpp::Category* tpCategory = log4cpp::Category::exists( vLogName);
	if ( tpCategory != NULL )
	{
		log4cpp::RollingFileAppender* pAppender = (log4cpp::RollingFileAppender*) tpCategory->getAppender();
		if( pAppender == NULL )
		{
			return -1;
		}

		pAppender->setMaxBackupIndex( vMaxBackupIndex );
		pAppender->setMaximumFileSize( vMaxFileSize );
		
		tpCategory->setPriority( vPriority );
	}

#endif

	return 0;
}


int ShutdownLog( const char *pLogName )
{
#ifdef USE_LOG4CPP
	log4cpp::Category* tpGategory =  log4cpp::Category::exists( pLogName );
	if ( tpGategory != NULL )
	{
		tpGategory->removeAppender( tpGategory->getAppender( pLogName ) );
	}	
#endif
	return 0;	
}

int ShutdownAllLog(  )
{

#ifdef USE_LOG4CPP
	log4cpp::Category::shutdown( );
#endif
	return 0;
}

int log( const char* vLogName, int vPriority, const char* vFmt, ... )
{
#ifdef USE_LOG4CPP
	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( (log4cpp::Priority::PriorityLevel)vPriority, vFmt, va);
	va_end(va);
#endif
	return 0;
}


int LogDebug	( const char* vLogName, const char* vFmt, ... )
{

#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( log4cpp::Priority::DEBUG, vFmt, va);
	va_end(va);

#endif

	return 0;	
}


int LogInfo		( const char* vLogName, const char* vFmt, ... )
{
#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( log4cpp::Priority::INFO, vFmt, va);
	va_end(va);

#endif
	return 0;	
}

int LogNotice		( const char* vLogName, const char* vFmt, ... )
{
#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( log4cpp::Priority::NOTICE, vFmt, va);
	va_end(va);

#endif
	return 0;	
}




int LogWarn		( const char* vLogName, const char* vFmt, ... )
{
#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( log4cpp::Priority::WARN, vFmt, va);
	va_end(va);

#endif

	return 0;	
}


int LogError		( const char* vLogName, const char* vFmt, ... )
{
#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( log4cpp::Priority::ERROR, vFmt, va);
	va_end(va);

#endif

	return 0;	
}



int LogFatal	( const char* vLogName, const char* vFmt, ... )
{

#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( log4cpp::Priority::FATAL, vFmt, va);
	va_end(va);

#endif

	return 0;	
}


//////////////////////////////////////////////////////////////////////////
// va log
//////////////////////////////////////////////////////////////////////////
int LogDebug_va	( const char* vLogName, const char* vFmt, va_list ap)
{
#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	tpCategory->logva ( log4cpp::Priority::DEBUG, vFmt, ap);
#endif
	return 0;
}


int LogNotice_va( const char* vLogName, const char* vFmt, va_list ap)
{
#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	tpCategory->logva ( log4cpp::Priority::NOTICE, vFmt, ap);
#endif
	return 0;
}

int LogInfo_va		( const char* vLogName, const char* vFmt, va_list ap )
{
#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	tpCategory->logva ( log4cpp::Priority::INFO, vFmt, ap);
#endif
	return 0;
}


int LogWarn_va		( const char* vLogName, const char* vFmt, va_list ap )
{
#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	tpCategory->logva ( log4cpp::Priority::WARN, vFmt, ap);
#endif
	return 0;
}


int LogError_va	( const char* vLogName, const char* vFmt, va_list ap )
{
#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	tpCategory->logva ( log4cpp::Priority::ERROR, vFmt, ap);
#endif
	return 0;
}

int LogFatal_va	( const char* vLogName, const char* vFmt, va_list ap )
{
#ifdef USE_LOG4CPP

	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	tpCategory->logva ( log4cpp::Priority::FATAL, vFmt, ap);
#endif
	return 0;
}


int log_va( const char* vLogName, int vPriority, const char* vFmt, va_list ap )
{
#ifdef USE_LOG4CPP
	if ( NULL == vLogName )
	{
		return -1;
	}

	log4cpp::Category* tpCategory = log4cpp::Category::exists(vLogName);
	if ( NULL == tpCategory )
	{
		return -1;
	}

	tpCategory->logva ( vPriority, vFmt, ap);
#endif

	return 0;
}


void Log_bin( const char* vLogName, void* vBin, int vBinLen )
{
#ifdef USE_LOG4CPP

	char* pBuffer = (char*)vBin;
	char tmpBuffer[60000] = {0};
	char strTemp[32] = {0};

	for (int i = 0; i < vBinLen; i++ )
	{
		if ( !(i % 16) )
		{
			sprintf(strTemp, "\n%04d>    ", i / 16 + 1);
			strcat(tmpBuffer, strTemp);
		}
		sprintf(strTemp, "%02X ", (unsigned char)pBuffer[i]);
		strcat(tmpBuffer, strTemp);
	}
	
	LogDebug( vLogName, tmpBuffer );
	
#endif

}




//////////////////////////////////////////////////////////////////////////
// default log
//////////////////////////////////////////////////////////////////////////

/*
int log( int vPriority, const char* vFmt, ... )
{
#ifdef USE_LOG4CPP

	log4cpp::Category* tpCategory = log4cpp::Category::exists("default");
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( (log4cpp::Priority::PriorityLevel)vPriority, vFmt, va);
	va_end(va);
#endif
	return 0;
}


int LogDebug	( const char* vFmt, ... )
{

#ifdef USE_LOG4CPP

	log4cpp::Category* tpCategory = log4cpp::Category::exists("default");
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( log4cpp::Priority::DEBUG, vFmt, va);
	va_end(va);

#endif

	return 0;	
}


int LogInfo		(  const char* vFmt, ... )
{
#ifdef USE_LOG4CPP
	log4cpp::Category* tpCategory = log4cpp::Category::exists("default");
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( log4cpp::Priority::INFO, vFmt, va);
	va_end(va);

#endif
	return 0;	
}



int LogWarn		( const char* vFmt, ... )
{
#ifdef USE_LOG4CPP

	log4cpp::Category* tpCategory = log4cpp::Category::exists("default");
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( log4cpp::Priority::WARN, vFmt, va);
	va_end(va);

#endif

	return 0;	
}


int LogError		( const char* vFmt, ... )
{
#ifdef USE_LOG4CPP
	log4cpp::Category* tpCategory = log4cpp::Category::exists("default");
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( log4cpp::Priority::ERROR, vFmt, va);
	va_end(va);

#endif

	return 0;	
}



int LogFatal	( const char* vFmt, ... )
{

#ifdef USE_LOG4CPP

	log4cpp::Category* tpCategory = log4cpp::Category::exists("default");
	if ( NULL == tpCategory )
	{
		return -1;
	}

	va_list va;

	va_start(va, vFmt);
	tpCategory->logva ( log4cpp::Priority::FATAL, vFmt, va);
	va_end(va);

#endif

	return 0;	
}

*/
