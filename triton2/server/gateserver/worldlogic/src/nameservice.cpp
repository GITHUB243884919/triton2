#include "nameservice.h"
#include "gateobj_manager.h"
#include "gatelogic.h"
#include "servermessage_in.h"
#include "client_comm_engine.h"
#include "base.h"
#include <mysql.h>

template<> CServiceRoleIDName* CSingleton< CServiceRoleIDName >::spSingleton = NULL;

//bool DecryptAscIICode(const char* szInput, int nInput, char* szOutput, int* pnOutput)
//{
//	static int tKey[ 4 ] = {4356,78534,46677,3545};
//	static unsigned char* tpKey = (unsigned char*)&tKey[0];
//
//	int nBinaryLen = 0;
//	unsigned char sBinaryCode[255+1] = {0};
//
//	size_t nAscIILen = strlen(szInput);
//	#define CHARTOHEX( c ) (isdigit(( c )) ? (( c ) - '0') : (tolower(( c )) - 'a' + 10))
//	for (size_t i= 0; i+2 <= nAscIILen; i=i+2, ++nBinaryLen)
//	{
//		if( isxdigit(szInput[i]) && isxdigit(szInput[i+1]) )
//		{
//			sBinaryCode[nBinaryLen] = CHARTOHEX(szInput[i]) * 16 + CHARTOHEX( szInput[i+1] );
//		}else
//		{
//			sBinaryCode[nBinaryLen] = 0;
//		}
//	}
//	#undef  CHARTOHEX
//
//	int nResult = oi_symmetry_decrypt( sBinaryCode, nBinaryLen, tpKey, (unsigned char*) szOutput, pnOutput);
//	if ( nResult == FALSE )
//	{
//		return  false;
//	}
//
//	szOutput[*pnOutput] = 0;
//	return true;
//}




void CServiceRoleIDName::OnLaunchServer()
{
	if ( mShmPtr->GetInitMode () == SHM_INIT )
	{
		//LoadRoleCountFromDB();
		LoadRoleInfoFromDB(0, 0);
	}
}

void CServiceRoleIDName::LoadRoleCountFromDB()
{
	const char* stmtsql= "select count(*) from UMS_ROLE";

	if (! CGateLogic::GetSingleton().ExecuteSql(em_dbsession_rolecount, 0, 0, 1, SQL_NEED_CALLBACK, SELECT, stmtsql))
	{
		LOG_ERROR("default", "create dbsession on function %s", __FUNCTION__);
	}
}

void CServiceRoleIDName::OnSessionLoadRoleCountFromDB(CMessage* tpMessage, int nStart, int ONCECOUNT)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != NULL, return);

	CMessageExecuteSqlResponse3* pMessage = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();
	if ( pMessage->resultcode() == 1 )
	{
		const int const_rowsnum_per_result = 300;

		m_nNeedLoadCount = atoi(pMessage->fieldvalue(0).c_str());
		m_nHaveLoadCount = 0;	// must assign value.

		for ( int nstart = 0; nstart <= m_nNeedLoadCount; nstart += const_rowsnum_per_result)
		{
			LoadRoleInfoFromDB(nstart, const_rowsnum_per_result);
		}
			
	}else
	{
		printf("Load all role error from ums_role_using from db !\n");

		LOG_ERROR("default", "Load all role count from db error");
		exit(0);
	}
}

/*
void CServiceRoleIDName::LoadRoleInfoFromDB(int nStart, int nCount)
{
	const char* stmtfmt =  "select role_id, role_name, nationality_id, gender_id, face_type, role_level,metier_id from UMS_ROLE order by role_id limit %d, %d";

	if (! CGateLogic::GetSingleton().ExecuteSql(em_dbsession_allroles, nStart, nCount, 
				RAND(nCount), SQL_NEED_CALLBACK, SELECT, stmtfmt, nStart, nCount))
	{
		LOG_ERROR("default", "create dbsession on function %s", __FUNCTION__);
	}
}
*/

void CServiceRoleIDName::LoadRoleInfoFromDB(int nStart, int nCount)
{
	MYSQL mysql; 
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");	

	const char* szAscIICode = CGateLogic::GetSingleton().mConfig.m_szMysqlInfo;

	Tokens tokens = StrSplit(szAscIICode, ";");
	Tokens::iterator iter;

	std::string host, port_or_socket, user, password, database;
	iter = tokens.begin();

	if(iter != tokens.end()) host = *iter++;
	if(iter != tokens.end()) port_or_socket = *iter++;
	if(iter != tokens.end()) user = *iter++;
	if(iter != tokens.end()) password = *iter++;
	if(iter != tokens.end()) database = *iter++;

	char szUser[32+1] = {0};
	int nUserLength = sizeof(szUser);

	char szPasswd[32+1] = {0};
	int nPasswdLength = sizeof(szPasswd);

	if ( DecryptAscIICode(user.c_str(), user.length(), szUser, &nUserLength) != true)
	{
		LOG_ERROR("default", "ERROR: Can NOT decrypt user(%s) section.", user.c_str());
		return;
	}

	if ( DecryptAscIICode(password.c_str(), password.length(), szPasswd, &nPasswdLength) != true)
	{
		LOG_ERROR("default", "ERROR: Can NOT decrypt user(%s) section.", password.c_str());
		return;
	}

	if(!mysql_real_connect(&mysql, host.c_str(), szUser,
				szPasswd, database.c_str(), atoi(port_or_socket.c_str()), 0, 0))
	{
		LOG_ERROR("default", "Failed to connect to database: Error: %s", mysql_error(&mysql));
		//return;
		//TODO: 连不上数据库直接退出吧
		printf( "Failed to connect to database: Error: %s\n", mysql_error(&mysql) );
		exit(0);
	}

	const char* stmtsql =  "select role_id, role_name, nationality_id, gender_id, face_type, role_level,metier_id, account_id, role_vip_flag from UMS_ROLE";

	if (mysql_query(&mysql,stmtsql))
	{
		printf("%s error\n", stmtsql);
		mysql_close(&mysql);
		return;
	}

	MYSQL_RES* presult = mysql_use_result(&mysql);

	if ( !presult )
	{
		LOG_ERROR("default", "[%s:%d] mysql_use_reuslt Error: %s", __FUNCTION__, __LINE__, mysql_error(&mysql));
		mysql_close(&mysql);
		return;
	}

	unsigned int  nfields = mysql_num_fields(presult);
	m_nHaveLoadCount = 0;

	if ( nfields != CServiceRoleIDName::FETCH_FIELD_COUNT)
	{
		LOG_ERROR("default", "[%s:%d] field count(%d) does not match with: %d", 
				__FUNCTION__, __LINE__, FETCH_FIELD_COUNT, nfields);

		mysql_close(&mysql);
		return;
	}
	
	MYSQL_ROW fields;
	while((fields = mysql_fetch_row(presult)))
	{
		/*
		unsigned long *lengths = mysql_fetch_lengths(presult);
		for(i = 0; i < nfields; i++)
		{
			printf("[%.*s] ", (int) lengths[i], fields[i] ? fields[i] : "NULL");
		}
		*/

		CRoleGlobalInfo* pRoleInfo = CreateRoleInfo(atoi(fields[0]), fields[1], atoi(fields[7]));
		if (pRoleInfo == NULL) 
		{
			LOG_ERROR("default", "CreateRoleInfo error because of create object and not load all roles");
			printf("calloc memory for the object CRoleGlobalInfo error, pls check pool size!\n");

			mysql_free_result(presult);
			mysql_close(&mysql);
			exit(0);
		}

		pRoleInfo->SetCountryID(fields[2] ? atoi(fields[2]) : 0);
		pRoleInfo->SetGenderID(fields[3]  ? atoi(fields[3]) : 0);
		pRoleInfo->SetFaceType(fields[4]  ? atoi(fields[4]) : 0);
		pRoleInfo->SetLevel( fields[5] ? atoi(fields[5]) : 0);
		pRoleInfo->SetMetier( fields[6] ? atoi(fields[6]) : 0);
		pRoleInfo->SetAccountID(fields[7] ? atoi(fields[7]) : 0);
		pRoleInfo->SetVipFlag(fields[8] ? atoi(fields[8]) : 0);

		m_nHaveLoadCount += 1;	// must assign value.
	}
	m_nNeedLoadCount = mysql_num_rows(presult);

	if(!mysql_eof(presult))  // mysql_fetch_row() failed due to an error
	{
		LOG_ERROR("default", "mysql_eof(MYSQL_RES) Error: %s", mysql_error(&mysql));

		mysql_free_result(presult);
		mysql_close(&mysql);
		return;
	}


	mysql_free_result(presult);
	mysql_close(&mysql);

	LOG_INFO("default", "load all roleglobalinfo completed(%d/%d), IDIndex(%d), NameIndex(%d)",
						m_nHaveLoadCount, m_nNeedLoadCount, mIndexOfRoleID.size(), mIndexOfRoleName.size());

	return;
}

void CServiceRoleIDName::OnSessionLoadRoleInfoFromDB(CMessage* tpMessage, int nStart, int ONCECOUNT)
{
	LK_ASSERT(tpMessage != NULL, return);

	CMessageExecuteSqlResponse3* pMessage = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();
	if (pMessage != NULL)
	{
		int colnum = pMessage->colcount();
		int rownum = pMessage->rowcount();

		CRoleGlobalInfo roleinfo;
		if ( pMessage->resultcode() == 1 )
		{
			for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
			{
				CRoleGlobalInfo* pRoleInfo = CreateRoleInfo(
						atoi(pMessage->fieldvalue(0 + tRowIndex * colnum).c_str()),
						pMessage->fieldvalue(1 + tRowIndex * colnum).c_str(),
						atoi(pMessage->fieldvalue(7 + tRowIndex * colnum).c_str()));

				if (pRoleInfo == NULL) 
				{
					LOG_ERROR("default", "CreateRoleInfo error because of create object and not load all roles");
					printf("calloc memory for the object CRoleGlobalInfo error, pls check pool size!");
					exit(0);
				}

				pRoleInfo->SetCountryID(atoi(pMessage->fieldvalue(2 + tRowIndex * colnum).c_str()));
				pRoleInfo->SetGenderID(atoi(pMessage->fieldvalue(3 + tRowIndex * colnum).c_str()));
				pRoleInfo->SetFaceType(atoi(pMessage->fieldvalue(4 + tRowIndex * colnum).c_str()));
				pRoleInfo->SetLevel( atoi(pMessage->fieldvalue(5 + tRowIndex * colnum).c_str()) );
				pRoleInfo->SetMetier( atoi(pMessage->fieldvalue(6 + tRowIndex * colnum).c_str()) );
				pRoleInfo->SetAccountID( atoi(pMessage->fieldvalue(7 + tRowIndex * colnum).c_str()) );
				pRoleInfo->SetVipFlag( atoi(pMessage->fieldvalue(8 + tRowIndex * colnum).c_str()) );
			}

			m_nHaveLoadCount += rownum;

			LOG_INFO("default", "LoadRoleInfoFromDB from %d count %d/%d successfully", nStart, rownum, m_nNeedLoadCount);

			if ( m_nHaveLoadCount >= m_nNeedLoadCount)
			{
				LOG_INFO("default", "load all roleglobalinfo completed(%d/%d), IDIndex(%d), NameIndex(%d)",
						m_nHaveLoadCount, m_nNeedLoadCount, mIndexOfRoleID.size(), mIndexOfRoleName.size());
			}
		}else
		{
			LOG_ERROR("default", "LoadRoleInfoFromDB from %d count %d error", nStart, ONCECOUNT);
		}
	}
}

void CServiceRoleIDName::OnExitServer()
{
	
}

void CServiceRoleIDName::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{

}

void CServiceRoleIDName::OnMessage( int nServerID, CMessage* tpMessage )
{
	CMessageHead* tpHead = tpMessage->mutable_msghead();
	unsigned short nMessageID = (unsigned short) tpHead->messageid();
	LK_ASSERT(((int) tpHead->srcid()) == nServerID, return);

	switch (nMessageID)
	{	
		default:
		{
			LOG_ERROR("default", "[%s:%d]there is no action for the message_id(%d)",
				__FUNCTION__, __LINE__, nMessageID);
			return;
		}
	}
}

CRoleGlobalInfo* CServiceRoleIDName::GetRoleInfoByRoleName(const char* szRoleName)
{
	ROLEINFONAMEINDEXTYPE::iterator iter = mIndexOfRoleName.find(szRoleName);
	if (iter != mIndexOfRoleName.end())
	{
		return (CRoleGlobalInfo*) CGateObjManager::GetSingletonPtr()->GetObject(iter->second);
	}

	return NULL;
}

CRoleGlobalInfo* CServiceRoleIDName::GetRoleInfoByRoleID(int nRoleID)
{
	ROLEINFOIDINDEXTYPE::iterator iter = mIndexOfRoleID.find(nRoleID);
	if (iter != mIndexOfRoleID.end())
	{
		return (CRoleGlobalInfo*) CGateObjManager::GetSingletonPtr()->GetObject(iter->second);
	}

	return NULL;
}

unsigned int CServiceRoleIDName::CountSize()
{
	return sizeof(CServiceRoleIDName);
}


CServiceRoleIDName::CServiceRoleIDName()
{
	if( mShmPtr->GetInitMode() == SHM_INIT )		
	{
		Initialize();
	}else
	{
		Resume();
	}
}

CServiceRoleIDName::~CServiceRoleIDName()
{

}

int CServiceRoleIDName::Initialize()
{
	mIndexOfRoleID.initailize();
	mIndexOfRoleName.initailize();
	mAccountHasRole.initailize();

	m_nNeedLoadCount = 0;
	m_nHaveLoadCount = -1;

	return 0;
}

bool CServiceRoleIDName::IsLaunched()
{
	return m_nHaveLoadCount >= m_nNeedLoadCount;;
}

int CServiceRoleIDName::Resume()
{

	return 0;
}

void* CServiceRoleIDName::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CServiceRoleIDName::operator delete( void* pointer )
{

}

CRoleGlobalInfo* CServiceRoleIDName::CreateRoleInfo(int nRoleID, const char* szRoleName, int nAccountID)
{
	CRoleGlobalInfo* pRoleInfo = 
		(CRoleGlobalInfo*) CGateObjManager::GetSingletonPtr()->CreateObject(OBJTYPE_ROLEGLOBALINFO);

	if (pRoleInfo == NULL) return NULL;

	pRoleInfo->SetRoleID(nRoleID);
	pRoleInfo->SetRoleName(szRoleName);
	pRoleInfo->SetAccountID(nAccountID);

	InsertRoleIDIndex(*pRoleInfo);

	InsertRoleNameIndex(*pRoleInfo);	

	// 计数
	ACCOUNTHASROLETYPE::iterator iter = mAccountHasRole.find(nAccountID);
	if ( iter == mAccountHasRole.end())
	{
		mAccountHasRole.insert(ACCOUNTHASROLETYPE::value_type(nAccountID, 1));
	}else
	{
		iter->second++;
	}

	return pRoleInfo;
}

void CServiceRoleIDName::RemoveRoleInfoByID( int nRoleID)
{
	ROLEINFOIDINDEXTYPE::iterator iter = mIndexOfRoleID.find(nRoleID);
	if (iter != mIndexOfRoleID.end())
	{
		CRoleGlobalInfo* pRoleInfo = 
			(CRoleGlobalInfo*) CGateObjManager::GetSingletonPtr()->GetObject(iter->second);

		LK_ASSERT(pRoleInfo != NULL, return);
		mIndexOfRoleName.erase(pRoleInfo->RoleName());

		// 删除计数
		ACCOUNTHASROLETYPE::iterator iter4acct = mAccountHasRole.find(pRoleInfo->AccountID());
		if ( iter4acct != mAccountHasRole.end())
		{
			iter4acct->second --;
			if ( iter4acct->second == 0)
			{
				mAccountHasRole.erase(iter4acct);
			}
		}

		CGateObjManager::GetSingletonPtr()->DestroyObject(iter->second);
		mIndexOfRoleID.erase(iter);
	}
}

void CServiceRoleIDName::RemoveRoleInfoByName( const char* szRoleName)
{
			
	ROLEINFONAMEINDEXTYPE::iterator iter = mIndexOfRoleName.find(szRoleName);
	if (iter != mIndexOfRoleName.end())
	{
		CRoleGlobalInfo* pRoleInfo = 
			(CRoleGlobalInfo*) CGateObjManager::GetSingletonPtr()->GetObject(iter->second);

		LK_ASSERT(pRoleInfo != NULL, return);
		mIndexOfRoleID.erase(pRoleInfo->RoleID());

		// 删除计数
		ACCOUNTHASROLETYPE::iterator iter4acct = mAccountHasRole.find(pRoleInfo->AccountID());
		if ( iter4acct != mAccountHasRole.end())
		{
			iter4acct->second --;
			if ( iter4acct->second == 0)
			{
				mAccountHasRole.erase(iter4acct);
			}
		}

		CGateObjManager::GetSingletonPtr()->DestroyObject(iter->second);
		mIndexOfRoleName.erase(iter);
	}
}


void CServiceRoleIDName::InsertRoleIDIndex(CRoleGlobalInfo& roleinfo)
{
	std::pair<ROLEINFOIDINDEXTYPE::iterator, bool> ret =
		mIndexOfRoleID.insert( ROLEINFOIDINDEXTYPE::value_type(roleinfo.RoleID(), roleinfo.get_id()));

	if (ret.second == false)
	{
		LOG_ERROR("default", "insert into %d to mIndexOfRoleID error", roleinfo.RoleID());
		ret.first->second = roleinfo.get_id();
	}

}


void CServiceRoleIDName::InsertRoleNameIndex(CRoleGlobalInfo& roleinfo)
{
	std::pair<ROLEINFONAMEINDEXTYPE::iterator, bool> ret =
		mIndexOfRoleName.insert(ROLEINFONAMEINDEXTYPE::value_type(roleinfo.RoleName(), roleinfo.get_id()));

	if (ret.second == false)
	{
		LOG_ERROR("default", "insert into %s,%d to mIndexOfRoleName error", roleinfo.RoleName(), roleinfo.RoleID() );
		ret.first->second = roleinfo.get_id();
	}
}

int CServiceRoleIDName::RoleCount(int nAccountID)
{
	ACCOUNTHASROLETYPE::iterator iter = mAccountHasRole.find(nAccountID);
	return iter != mAccountHasRole.end() ? iter->second : 0;
}
