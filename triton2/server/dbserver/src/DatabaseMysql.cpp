#include "DatabaseMysql.h"
#include "base_db.h"
#include "QueryResultMysql.h"

void DatabaseMysql::ThreadStart()
{
    mysql_thread_init();
}

void DatabaseMysql::ThreadEnd()
{
    mysql_thread_end();
}

size_t DatabaseMysql::db_count = 0;

DatabaseMysql::DatabaseMysql() : Database(), mMysql(0)
{
    // before first connection
    if( db_count++ == 0 )
    {
        // Mysql Library Init
        mysql_library_init(-1, NULL, NULL);

		// 如果是每个线程独享一个mysql链接，那么就不需要mysql的线程安全性
        //if (!mysql_thread_safe())
        //{
        //    LOG_ERROR( m_logsName.c_str(), "FATAL ERROR: Used MySQL library isn't thread-safe.");
        //    exit(1);
        //}
    }
}

DatabaseMysql::~DatabaseMysql()
{
    if (mMysql)
        mysql_close(mMysql);

    //Free Mysql library pointers for last ~DB
    if(--db_count == 0)
        mysql_library_end();
}

bool DatabaseMysql::Initialize(const char *infoString)
{
	m_hostInfoString = std::string( infoString );

    if(!Database::Initialize(infoString))
        return false;

    MYSQL *mysqlInit = mysql_init(NULL);
    if (!mysqlInit)
    {
        LOG_ERROR( m_logsName.c_str(), "Could not initialize Mysql connection" );
        return false;
    }

	Tokens tokens = StrSplit(m_hostInfoString, ";");

    Tokens::iterator iter;

    std::string host, port_or_socket, user, password, database;
    int port;
    char const* unix_socket;

    iter = tokens.begin();

    if(iter != tokens.end())
        host = *iter++;
    if(iter != tokens.end())
        port_or_socket = *iter++;
    if(iter != tokens.end())
        user = *iter++;
    if(iter != tokens.end())
        password = *iter++;
    if(iter != tokens.end())
        database = *iter++;

#ifdef ENCODING_UTF8
    mysql_options(mysqlInit,MYSQL_SET_CHARSET_NAME,"utf8");
#else
	mysql_options(mysqlInit,MYSQL_SET_CHARSET_NAME,"gbk");
#endif    
	
	#ifdef WIN32
    if(host==".")                                           // named pipe use option (Windows)
    {
        unsigned int opt = MYSQL_PROTOCOL_PIPE;
        mysql_options(mysqlInit,MYSQL_OPT_PROTOCOL,(char const*)&opt);
        port = 0;
        unix_socket = 0;
    }
    else                                                    // generic case
    {
        port = atoi(port_or_socket.c_str());
        unix_socket = 0;
    }
    #else
    if(host==".")                                           // socket use option (Unix/Linux)
    {
        unsigned int opt = MYSQL_PROTOCOL_SOCKET;
        mysql_options(mysqlInit,MYSQL_OPT_PROTOCOL,(char const*)&opt);
        host = "localhost";
        port = 0;
        unix_socket = port_or_socket.c_str();
    }
    else                                                    // generic case
    {
        port = atoi(port_or_socket.c_str());
        unix_socket = 0;
    }
    #endif

    mMysql = mysql_real_connect(mysqlInit, host.c_str(), user.c_str(),
        password.c_str(), database.c_str(), port, unix_socket, CLIENT_MULTI_RESULTS);

    if (mMysql)
    {
        LOG_INFO( m_logsName.c_str(), "Connected to MySQL database at %s", host.c_str());
        LOG_INFO( m_logsName.c_str(), "MySQL client library: %s", mysql_get_client_info());
        LOG_INFO( m_logsName.c_str(), "MySQL server ver: %s ", mysql_get_server_info( mMysql));

        /*----------SET AUTOCOMMIT ON---------*/
        // It seems mysql 5.0.x have enabled this feature
        // by default. In crash case you can lose data!!!
        // So better to turn this off
        // ---
        // This is wrong since mangos use transactions,
        // autocommit is turned of during it.
        // Setting it to on makes atomic updates work
        if (!mysql_autocommit(mMysql, 1))
            LOG_INFO( m_logsName.c_str(), "AUTOCOMMIT SUCCESSFULLY SET TO 1");
        else
            LOG_INFO( m_logsName.c_str(), "AUTOCOMMIT NOT SET TO 1");
        /*-------------------------------------*/

#ifdef ENCODING_UTF8
        // set connection properties to UTF8 to properly handle locales for different
        // server configs - core sends data in UTF8, so MySQL must expect UTF8 too
        PExecute("SET NAMES `utf8`");
        PExecute("SET CHARACTER SET `utf8`");
#else
		PExecute("SET NAMES `gbk`");
		PExecute("SET CHARACTER SET `gbk`");
#endif // ENCODING_UTF8
        return true;
    }
    else
    {
		LOG_ERROR( m_logsName.c_str(), "Could not connect to MySQL database at %s: %s\n", host.c_str(),mysql_error(mysqlInit));
        mysql_close(mysqlInit);
        return false;
    }
}

QueryResult* DatabaseMysql::Query(const char *sql)
{
    if (!mMysql)
        return 0;

    MYSQL_RES *result = 0;
    uint64 rowCount = 0;
    uint32 fieldCount = 0;

    {
		int ret = mysql_query(mMysql, sql);
        if( ret )
        {
			int nErrorNo = mysql_errno( mMysql );
			// if mysql has gone, maybe longtime no request or mysql restarted
			if( nErrorNo == CR_SERVER_GONE_ERROR || nErrorNo == CR_SERVER_LOST || nErrorNo == CR_UNKNOWN_ERROR )
			{
				if (mMysql)
					mysql_close(mMysql);

				// reconnect mysql
				if ( Initialize( m_hostInfoString.c_str() ) == true )
				{
					LOG_DEBUG( m_logsName.c_str(), "reinit mysql success on host [%s]", m_hostInfoString.c_str() );

					// re querey  sql statment
					ret =  mysql_query(mMysql, sql);
				}
			}

			if( ret )
			{
				LOG_ERROR( m_logsName.c_str(), "SQL: %s", sql );
				LOG_ERROR( m_logsName.c_str(), "query ERROR(%d): %s", nErrorNo, mMysql ? mysql_error(mMysql) : "NULL");
				return NULL;
			}
            
        }
        else
        {
			// 加入统计该sql语句执行多久
        }

        result = mysql_store_result(mMysql);

        rowCount = mysql_affected_rows(mMysql);
        fieldCount = mysql_field_count(mMysql);
        // end guarded block
    }

    if (!result )
        return NULL;

    //if (!rowCount)
    //{
    //    mysql_free_result(result);
    //    return NULL;
    //}

    QueryResultMysql *queryResult = new QueryResultMysql(result, rowCount, fieldCount);
	if( queryResult == NULL )
	{
		LOG_ERROR( m_logsName.c_str(), "while create qureyresult, run out of memory" );
		return NULL;
	}
    queryResult->NextRow();

    return queryResult;
}

bool DatabaseMysql::Execute(const char *sql)
{
    if (!mMysql)
        return false;

    return true;
}

bool DatabaseMysql::DirectExecute(const char* sql)
{
    if (!mMysql)
        return false;

    {
		int ret = mysql_query(mMysql, sql);
		if( ret )
		{
			int nErrorNo = mysql_errno( mMysql );
			// if mysql has gone, maybe longtime no request or mysql restarted
			if( nErrorNo == CR_SERVER_GONE_ERROR || nErrorNo == CR_SERVER_LOST || nErrorNo == CR_UNKNOWN_ERROR )
			{
				if (mMysql)
					mysql_close(mMysql);

				// reconnect mysql
				if ( Initialize( m_hostInfoString.c_str() ) == true )
				{
					LOG_DEBUG(m_logsName.c_str(), "reinit mysql success on host [%s]", m_hostInfoString.c_str() );
					// re querey  sql statment
					ret =  mysql_query(mMysql, sql);
				}
			}

			if( ret )
			{
				LOG_ERROR( m_logsName.c_str(), "SQL: %s", sql );
				LOG_ERROR( m_logsName.c_str(), "query ERROR(%d): %s", nErrorNo, mMysql ? mysql_error(mMysql) : "NULL" );
				return false;
			}

		}
        else
        {
			//TODO: 加入统计该sql语句执行多长时间
        }
        // end guarded block
    }

    return true;
}

bool DatabaseMysql::_TransactionCmd(const char *sql)
{
    if (mysql_query(mMysql, sql))
    {
		LOG_ERROR( m_logsName.c_str(), "SQL: %s", sql);
		LOG_ERROR( m_logsName.c_str(), "SQL ERROR: %s", mysql_error(mMysql));
        return false;
    }
    else
    {
        LOG_ERROR( m_logsName.c_str(), "SQL: %s", sql);
    }
    return true;
}

bool DatabaseMysql::BeginTransaction()
{
	return DirectExecute( "START TRANSACTION" );
    return true;
}

bool DatabaseMysql::CommitTransaction()
{
	return DirectExecute( "COMMIT" );
	return true;
}

bool DatabaseMysql::RollbackTransaction()
{
	return DirectExecute( "ROLLBACK" );
	return true;
}

unsigned long DatabaseMysql::escape_string(char *to, const char *from, unsigned long length)
{
    if (!mMysql || !to || !from || !length)
        return 0;

    return(mysql_real_escape_string(mMysql, to, from, length));
}

void DatabaseMysql::ClearResult()
{
	if ( mMysql == NULL ) return ;

	MYSQL_RES* res = NULL;
	while(mysql_next_result(mMysql) == 0)
	{
		mysql_free_result(res);
	}
}

int DatabaseMysql::AffectedRows( )
{
	if( mMysql == NULL ) return 0;	
	return mysql_affected_rows( mMysql );
}

long DatabaseMysql::GetLastInsertID( )
{
	return mysql_insert_id( mMysql );
}


