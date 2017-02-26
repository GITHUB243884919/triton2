#include "logmonitor.h"
#include "QueryResult.h"
#include "logctrl.h"

void CLogMonitor::LoadStatInfo(Database* pdb)
{
	const char* stmtsql = "SELECT catalog_type, catalog_id, catalog_value + delta_value FROM STAT_CATALOG_DETAIL";
	QueryResult* res = pdb->Query( stmtsql );
	if ( res == NULL)
	{
		//TRACE_ERROR( "sql %s exec, but no resultset returned", stmtsql );
		return;
	}

	int rownum = res->GetRowCount();
	int colnum = res->GetFieldCount();

	if( res->GetRowCount() > 0 )
	{
		do 
		{
			Field* pField = res->Fetch();
			if( pField == NULL )
			{
				/*
				TRACE_ERROR("ERROR: do sql %s success, row[%d], col[%d], but some row is null\n",
						stmtsql, rownum, colnum);
				*/

				break;
			}

			int nCatalogType = pField[0].GetInt32();
			int nCatalogID = pField[1].GetInt32();
			uint64_t nCatalogValue = pField[2].GetUInt64();

			std::pair<catalog_t::iterator, bool> ret =  
				mCatalog.insert(catalog_t::value_type(nCatalogID, value_pair_t(nCatalogValue, 0)));

			if ( ret.second == false)
			{
				/*
				TRACE_ERROR("ERROR: load catalog type %d error for catalog id (%d, %lld)\n",
						nCatalogType, nCatalogID, nCatalogValue);
				*/
			}

		}while(  res->NextRow() );
	}
}

void CLogMonitor::SaveStatInfo(Database* pdb)
{
	lk::string<0xffff> stmtins("");

	time_t nSeconds= time(0);
	struct tm tm;
	struct tm * ptm = localtime_r(&nSeconds, &tm);

	char szDate[12];
	strftime(szDate, sizeof(szDate), "%Y%m%d", ptm);


	catalog_t::iterator iter = mCatalog.begin();
	for(; iter!= mCatalog.end(); ++iter)
	{
		stmtins.clear();

		/*
		CLogCtrl::CATALOG_INFO* pCatalog = CLogCtrl::GetSingleton().GetCatalog(iter->first);
		if ( pCatalog == NULL) continue;
		*/

		int nCatalogType = ((iter->first) >> 28);
		int nActionType = 0;
		int nValueType = 0;
		int nEntityType = 0;

		int nLevel = 0;
		int nSubType = 0;
		int nItemType = 0;

		switch( nCatalogType )
		{
			case EM_SERVER_ITEM:
			{
				nActionType = ( (iter->first & 0x0FF00000) >> 20);
				nItemType = ((iter->first & 0x000FF000) >> 12);
				nLevel = ((iter->first & 0x00000F00) >> 8);
				nSubType = (iter->first & 0x000000FF);

				break;
			}

			case EM_SERVER_MONEY:
			case EM_SERVER_EVENT:
			case EM_SERVER_EXP:
			case EM_SERVER_FAMILY:
			case EM_SERVER_CORPS:
			{
				nActionType = (( iter->first & 0x00FF0000) >> 16);
				nEntityType = (( iter->first & 0x0000FF00) >> 8);
				nValueType =  ( iter->first & 0x000000FF);
				break;
			}

			default:
			{
				continue;
			}
		}


		//int nLifeID = CLogCtrl::GetSingleton().ItemLife(nCatalogType, nActionType);
		switch (m_nTableID)
		{
			case LOG_MONEY_PRODUCE: 
			{
				// log_id, log_time, money_type, produce_type, object_type, produce_value
				stmtins.append("INSERT INTO %s_LOG_MONEY_PRODUCE_01 VALUES (NULL,%d,%d,%d,%d,%lld)",
						szDate, nSeconds, nValueType, nActionType, nEntityType, iter->second.second);
				break;
			}

			case LOG_MONEY_RECYLE:
			{
				stmtins.append("INSERT INTO %s_LOG_MONEY_RECYCLE_01 VALUES (NULL,%d,%d,%d,%d,%lld)",
						szDate, nSeconds, nValueType, nActionType, nEntityType, iter->second.second);
				break;
			}

			case LOG_EXP_PRODUCE:
			{
				stmtins.append("INSERT INTO %s_LOG_EXP_PRODUCE_01 VALUES (NULL,%d,%d,%d,%d,%lld)",
						szDate, nSeconds, nValueType, nActionType, nEntityType, iter->second.second);

				break;
			}

			case LOG_ITEM_PRODUCE:
			{
				// log_id, log_time, produce_type, item_tpye, item_level, sub_type, item_number
				stmtins.append("INSERT INTO %s_LOG_ITEM_PRODUCE_01 VALUES (NULL,%d,%d,%d,%d,%d,%lld)",
						szDate, nSeconds, nActionType, nItemType, nLevel, nSubType, iter->second.second);

				break;
			}

			case LOG_ITEM_RECYCLE:
			{
				// log_id, log_time, produce_type, entity_type, produce_number
				stmtins.append("INSERT INTO %s_LOG_ITEM_RECYCLE_01 VALUES (NULL,%d,%d,%d,%d,%d,%lld)",
						szDate, nSeconds, nActionType, nItemType, nLevel, nSubType, iter->second.second);

				break;
			}

			case LOG_ACTION_MONIT:
			{
				// log_id, log_time, action_type, entity_type, times
				stmtins.append("INSERT INTO %s_LOG_EVENT_MONIT_01 VALUES (NULL,%d,%d,%d,%lld)",
						szDate, nSeconds, nActionType, nEntityType, iter->second.second);

				break;
			}

			case LOG_MONEY_CIRCULATION:
			{
				// log_id, log_time, produce_type, entity_type, produce_number
				stmtins.append("INSERT INTO %s_LOG_MONEY_LOOP_01 VALUES (NULL,%d,%d,%d,%d,%lld)",
						szDate, nSeconds, nValueType, nActionType, nEntityType, iter->second.second);
				break;
			}

			case LOG_ITEM_CIRCULATION:
			{
				// log_id, log_time, produce_type, item_tpye, item_level, sub_type, item_number
				stmtins.append("INSERT INTO %s_LOG_ITEM_LOOP_01 VALUES (NULL,%d,%d,%d,%d,%d,%lld)",
						szDate, nSeconds, nActionType, nItemType, nLevel, nSubType, iter->second.second);

				break;
			}
			
			default:
			{
				continue;
			}
		}

		if (stmtins.length() > 0)
		{
			#ifndef _ASYNC_SQL_                                                                                        
			if( pdb->DirectExecute( stmtins.c_str() ) != true )
			{                                                                                                  
				/*
				   TRACE_ERROR( "Error: [%s][%d][%s], direct exec sql %s faild.\n", 
				   __LK_FILE__, __LINE__, __FUNCTION__, stmtins );
				 */
			}
			#else
			if( ExecSqlAsync( stmtins.c_str() ) < 0 )
			{
				/*
				   TRACE_ERROR( "Error: [%s][%d][%s], exec async sql %s faild.\n", 
				   __LK_FILE__, __LINE__, __FUNCTION__, stmtins );
				 */
			}                                                                                                  
			#endif  
		}
	}   
}
