#ifndef _ASQLEXE_H_
#define _ASQLEXE_H_

#include "thread.h"
#include "base_db.h"
#include "sock_stream.h"

typedef struct 
{
	int m_iSqlCount;
	timeval m_tvSqlCost;
	timeval m_tvSlowestSql;
	char    m_szSlowestSql[1024];
} SQL_STAT_INFO;

class Database;

class CASqlExe : public CThread
{
public:
	CASqlExe();
	~CASqlExe();

	virtual int IsToBeBlocked();
	virtual int PrepareToRun();
	virtual int Run();

	int Initialize( int iHandleID, int iPipeFD );

	void SetHostInfoString(const char* host);
	
	int InitLogFile( const char* vLogName, const char* vLogDir, LogLevel vPriority  = LEVEL_NOTSET , unsigned int vMaxFileSize  = 10*1024*1024 , unsigned int vMaxBackupIndex  = 1 , bool vAppend  = true  );

private:

	int OnTimeCheck();
	int PrintStatInfo();

	int m_iHandleID;
	int m_iPipeFD;
	int m_iLDBInited;

	//TLIB_DB_LINK m_stLDBLink;
	Database* m_pDatabase;
	char m_szHostInfoString[SQL_HOST_INFO_STRING];

	SQL_STAT_INFO m_stSqlStat;
	timeval m_tvLastCheck;
	timeval m_tvRunTime;

	CSockStream m_sstmPipe;
};

#endif
