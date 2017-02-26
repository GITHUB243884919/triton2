#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <string.h>
#include "log.h"

#define MAX_QUERY_LEN   1024

class QueryResult;

class Database
{
    protected:
        Database()  {};

	public:

        virtual ~Database();

		bool InitLog(const char* vLogName, const char* vLogDir, LogLevel vPriority  = LEVEL_NOTSET , unsigned int vMaxFileSize  = 10*1024*1024 , unsigned int vMaxBackupIndex  = 1 , bool vAppend  = true );
        virtual bool Initialize(const char *infoString);

        virtual QueryResult* Query(const char *sql) = 0;
        QueryResult* PQuery(const char *format,...);

        virtual bool Execute(const char *sql) = 0;
        bool PExecute(const char *format,...);
        virtual bool DirectExecute(const char* sql) = 0;
        bool DirectPExecute(const char *format,...);
		virtual int AffectedRows( ) = 0;

        // Writes SQL commands to a LOG file (see mangosd.conf "LogSQL")
        bool PExecuteLog(const char *format,...);

        virtual bool BeginTransaction()                     // nothing do if DB not support transactions
        {
            return true;
        }
        virtual bool CommitTransaction()                    // nothing do if DB not support transactions
        {
            return true;
        }
        virtual bool RollbackTransaction()                  // can't rollback without transaction support
        {
            return false;
        }

		virtual long GetLastInsertID( )						// 获得当前Session最后获得自增列ID 
		{
			return 0;
		}
		

        virtual operator bool () const = 0;

		virtual void ClearResult() {}

        virtual unsigned long escape_string(char *to, const char *from, unsigned long length) { strncpy(to,from,length); return length; }
        void escape_string(std::string& str);

        // must be called before first query in thread (one time for thread using one from existed Database objects)
        virtual void ThreadStart();
        // must be called before finish thread run (one time for thread using one from existed Database objects)
        virtual void ThreadEnd();

    private:
        bool m_logSQL;
        std::string m_logsDir;
		

	protected:
		std::string m_hostInfoString;
		std::string m_logsName;
};
#endif
