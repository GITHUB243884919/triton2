#ifndef _DATABASEMYSQL_H
#define _DATABASEMYSQL_H

#include "Database.h"

#ifdef WIN32
#define FD_SETSIZE 1024
#include <winsock2.h>
#include <mysql/mysql.h>
#else
#include <mysql.h>
#include <errmsg.h>
#endif

class QueryResult;

class  DatabaseMysql : public Database
{
	public:
        DatabaseMysql();
        ~DatabaseMysql();

        //! Initializes Mysql and connects to a server.
        /*! infoString should be formated like hostname;username;password;database. */
        bool Initialize(const char *infoString);
        QueryResult* Query(const char *sql);
        bool Execute(const char *sql);
        bool DirectExecute(const char* sql);
        bool BeginTransaction();
        bool CommitTransaction();
        bool RollbackTransaction();
		long GetLastInsertID( );
		void ClearResult();
		int AffectedRows( );

        operator bool () const { return mMysql != NULL; }

        unsigned long escape_string(char *to, const char *from, unsigned long length);
        using Database::escape_string;

        // must be call before first query in thread
        void ThreadStart();
        // must be call before finish thread run
        void ThreadEnd();

		int LastErrorNo() const { return mysql_errno( mMysql ); }

    private:

        MYSQL *mMysql;

        static size_t db_count;

        bool _TransactionCmd(const char *sql);
};

#endif
