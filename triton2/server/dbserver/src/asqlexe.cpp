#include "asqlexe.h"
#include "DatabaseMysql.h"
#include "lk_assert.h"
#include <sys/time.h>

CASqlExe::CASqlExe()
{
	m_iHandleID = -1;
	m_iPipeFD = -1;
	m_iLDBInited = False;
	memset((void *)&m_stSqlStat, 0, sizeof(m_stSqlStat));
	memset((void *)&m_tvLastCheck, 0, sizeof(m_tvLastCheck));
	memset((void *)&m_tvRunTime, 0, sizeof(m_tvRunTime));

	m_pDatabase = new DatabaseMysql;
	if( m_pDatabase == NULL )
	{
		printf( "new DatabaseMysql failed\n" );
		exit(1);
	}
}

CASqlExe::~CASqlExe()
{
}


int CASqlExe::PrintStatInfo()
{
	char perf_key[60] = {0};
	strncpy( perf_key, m_stLogCfg.szThreadKey, 40 );
	strcat( perf_key , "_perf" ) ;

	LOG_NOTICE( perf_key, "EXEC SQL %d times, COST %d.%06d, MAX COST %d.%06d, MAX SQL:\n(%s)",
		m_stSqlStat.m_iSqlCount, m_stSqlStat.m_tvSqlCost.tv_sec,
		m_stSqlStat.m_tvSqlCost.tv_usec, m_stSqlStat.m_tvSlowestSql.tv_sec,
		m_stSqlStat.m_tvSlowestSql.tv_usec, m_stSqlStat.m_szSlowestSql);

	memset((void *)&m_stSqlStat, 0, sizeof(m_stSqlStat));

	return 0;
}



int CASqlExe::OnTimeCheck()
{
	timeval tvNow, tvGap;
	int iRunSecs = 0;

	gettimeofday(&tvNow, NULL);
	iRunSecs = m_tvRunTime.tv_sec;

	TimeValMinus(tvNow, m_tvLastCheck, tvGap);
	TimeValPlus(m_tvRunTime, tvGap, m_tvRunTime);
	m_tvLastCheck.tv_sec = tvNow.tv_sec;
	m_tvLastCheck.tv_usec = tvNow.tv_usec;

	if( iRunSecs == m_tvRunTime.tv_sec )
	{
		return 0;
	}

	if( (m_tvRunTime.tv_sec%60) == 0 )
	{
		PrintStatInfo();
	}

	return 0;
}

int CASqlExe::Initialize(int iHandleID, int iPipeFD)
{
	TFName szThreadLogFile;

	m_iHandleID = iHandleID;
	m_iPipeFD = iPipeFD;
	
	m_sstmPipe.SetUpStream(m_iPipeFD);

	//snprintf( szThreadLogFile, sizeof(szThreadLogFile)-1,"../log/dbexecer%d_", m_iHandleID );

	//ThreadLogInit(szThreadLogFile, 0x1000000, 5, 0);

	//m_pDatabase = new DatabaseMysql;
	//m_pDatabase->InitLog( szThreadLogFile, szThreadLogFile, LEVEL_DEBUG, 0x1000000, 5  );
	//
	//LK_ASSERT( m_pDatabase != NULL, return -1 );

	return 0;
}

void CASqlExe::SetHostInfoString(const char* host)
{
	strncpy( m_szHostInfoString, host, sizeof(m_szHostInfoString)-1 );
}


int CASqlExe::InitLogFile( const char* vLogName, const char* vLogDir, LogLevel vPriority  /*= LEVEL_NOTSET*/ , unsigned int vMaxFileSize  /*= 10*1024*1024*/ , unsigned int vMaxBackupIndex  /*= 1*/ , bool vAppend  /*= true*/  )
{
	TFName szThreadLogFile;

	// 设置服务器日志
	snprintf( szThreadLogFile, sizeof(szThreadLogFile)-1,"../log/dbexecer%d_", m_iHandleID);

	ThreadLogInit( szThreadLogFile, vMaxFileSize, vMaxBackupIndex, 0, vPriority);

	LK_ASSERT( m_pDatabase != NULL, return -1 );

	m_pDatabase->InitLog( szThreadLogFile, szThreadLogFile, vPriority, vMaxFileSize,vMaxBackupIndex );
}


int CASqlExe::IsToBeBlocked()
{
	if( m_iPipeFD > 0 )
	{
		return False;
	}
	else
	{
		return True;
	}
}

int CASqlExe::PrepareToRun()
{
	LOG_DEBUG( m_stLogCfg.szThreadKey, "Execer %d prepare to run, Pipe fd = %d.", m_iHandleID, m_iPipeFD );

	if( m_iPipeFD <= 0 )
	{
		return -1;
	}

	if(  m_pDatabase->Initialize( m_szHostInfoString ) != true )
	{
		LOG_ERROR( m_stLogCfg.szThreadKey, "In CASqlExe::PrepareToRun, Connect to database failed.");
		return -1;
	}

	gettimeofday( &m_tvLastCheck, NULL );
	memset((void *)&m_tvRunTime, 0, sizeof(m_tvRunTime));

	return 0;
}

int CASqlExe::Run()
{
	fd_set fds_read;
	struct timeval stMonTime;
	int iOpenFDNum = 0;
	int iTempRet;
	short nTempCodeLength;
	BYTE  abyCodeBuf[MAX_SERVER_TRANS_LEN];
	char  *szSqlString = NULL;
	char  szErrMsg[512];

	while(True)
	{
		CondBlock();

		OnTimeCheck();

		if( m_iPipeFD <= 0 )
		{
			continue;
		}

		FD_ZERO( &fds_read );
		stMonTime.tv_sec = 0;
		stMonTime.tv_usec = 10000;

		FD_SET(m_iPipeFD, &fds_read);

		iOpenFDNum = select(m_iPipeFD+1, &fds_read, NULL, NULL, &stMonTime);

		if( iOpenFDNum <= 0 )
		{
			continue;
		}

		LOG_DEBUG( m_stLogCfg.szThreadKey, "Now execer have some message to handle.");

		
		m_sstmPipe.RecvData();
		

		for(;;)
		{
			
			if( m_sstmPipe.GetOneCode(nTempCodeLength, abyCodeBuf) <= 0 )
			{
				TRACE_DEBUG("Have received all messages from main ctrl.");
				break;
			}
			szSqlString = (char *)&abyCodeBuf[sizeof(short)];
			TRACE_DEBUG( "Execute cmd %s.", szSqlString);
			
			timeval tvTimeBegin, tvTimeEnd, tvTimeElaps;

			gettimeofday( &tvTimeBegin, NULL );
			bool ret = m_pDatabase->DirectExecute( szSqlString );
			if( ret != true )
			{
				TRACE_ERROR(" exec %s failed", szSqlString );
			}
			gettimeofday( &tvTimeEnd, NULL );

			TimeValMinus( tvTimeEnd, tvTimeBegin, tvTimeElaps );
			TimeValPlus( m_stSqlStat.m_tvSqlCost, tvTimeElaps, m_stSqlStat.m_tvSqlCost );

			if( tvTimeElaps.tv_sec > m_stSqlStat.m_tvSlowestSql.tv_sec 
				||( tvTimeElaps.tv_sec == m_stSqlStat.m_tvSlowestSql.tv_sec  
					&& tvTimeElaps.tv_usec > m_stSqlStat.m_tvSlowestSql.tv_usec  ) )
			{
				m_stSqlStat.m_tvSlowestSql.tv_sec = tvTimeElaps.tv_sec;
				m_stSqlStat.m_tvSlowestSql.tv_usec = tvTimeElaps.tv_usec;
				strncpy( m_stSqlStat.m_szSlowestSql, szSqlString, sizeof(m_stSqlStat.m_szSlowestSql) );
			}
			
			m_stSqlStat.m_iSqlCount ++;

		}
	}

	return 0;
}
