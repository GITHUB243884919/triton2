/*
 *	JudgeModule.cpp
 */

// Include Head Files
#include "judgemodule.h"
#include "judgemessage_pb.hxx.pb.h"

#include "base.h"
#include "entity.h"

template<> CJudgeModule *CSingleton< CJudgeModule >::spSingleton  = NULL;


// CJudgeModule

CJudgeModule::CJudgeModule()
{
	if( mShmPtr->GetInitMode() == SHM_INIT )
	{
		mFavorJudge.initailize();
		mBadJudge.initailize();
	}
}

CJudgeModule::~CJudgeModule()
{
	mFavorJudge.clear();
	mBadJudge.clear();
}

void CJudgeModule::OnLaunchServer()
{
	;
}

// ***********************************************************
//  Function:		OnExitServer
//  Description:	退出
//  Output:			
// **********************************************************
void CJudgeModule::OnExitServer( )
{

} 

// ***********************************************************
//  Function:		CountSize
//  Description:	
//  Output:		
// **********************************************************
unsigned int CJudgeModule::CountSize()
{
	return sizeof( CJudgeModule );
};




// ***********************************************************
//  Function:		OnTimer
//  Description:	计时器
//  Output:			
// **********************************************************
void CJudgeModule::OnTimer( unsigned int nTickCount, unsigned int nTickOffset )
{

}

// ***********************************************************
//  Function:		LoadJudgeDataFromDB
//  Description:	load 玩家得好评、恶评数据(对他人得)
//  Output:			
// **********************************************************
void CJudgeModule::LoadJudgeDataFromDB( CGatePlayer* npPlayer )
{
	if ( npPlayer == NULL )
	{
		return ;
	}

	char tFavorSql[ 1024 ] = { 0 };
	sprintf( tFavorSql, "select role_src, role_des, value, timestamp, type from UMS_JUDGE_FAVOR where role_src = %d "
		"union all select role_src, role_des, value, timestamp, type from UMS_JUDGE_BAD where role_src = %d;", 
		npPlayer->mCharID, npPlayer->mCharID );
	
	if ( CGateLogic::GetSingleton().ExecuteSql( em_dbsession_query_judge, npPlayer->mCharID, 0, 
		npPlayer->mAccountID, SQL_NEED_CALLBACK, SELECT, tFavorSql ) == false )
	{
		LOG_ERROR( "judge", "SQL Execute failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return ;
	}

	// 查询总值和条数
	char tSQL[ 1024 ] = { 0 };
	sprintf( tSQL, "select sum( value ), 0 from UMS_JUDGE_FAVOR where role_des = %d "
		"union all select sum( value ), 1 from UMS_JUDGE_BAD where role_des = %d;",
		npPlayer->mCharID, npPlayer->mCharID );

	if ( CGateLogic::GetSingleton().ExecuteSql( em_dbsession_query_rec_judge_sum, npPlayer->mCharID, 0, 
		npPlayer->mAccountID, SQL_NEED_CALLBACK, SELECT, tSQL ) == false )
	{
		LOG_ERROR( "judge", "SQL Execute failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return ;
	}
	return ;
}



// ***********************************************************
//  Function:		DeleteJudgeRecordMemory
//  Description:	delete judge record from memory, not db, when you leave game
//  Output:			
// **********************************************************
void CJudgeModule::DeleteJudgeRecordMemory( int nRoleID )
{
	JudgeOtherType::iterator it = mFavorJudge.find( nRoleID );
	if ( it != mFavorJudge.end() )
		mFavorJudge.erase( it );

	it = mBadJudge.find( nRoleID );
	if ( it != mBadJudge.end() )
		mBadJudge.erase( it );

	return ;
}

// ***********************************************************
//  Function:		DeleteJudgeRecordFromDB
//  Description:	delete judge record from db
//  Output:			
// **********************************************************

void CJudgeModule::DeleteJudgeRecordFromDB( int vAccountID, int nRoleID1, int nRoleID2, int nType )
{
	char tSQL[ 1024 ] = { 0 };

	if ( nType == Judge_Favor )
	{
		sprintf( tSQL, "delete from UMS_JUDGE_FAVOR where role_src = %d and role_des = %d;", nRoleID1, nRoleID2 );
		CGateLogic::GetSingleton().ExecuteSql( em_dbsession_delete_judge_favor, nRoleID1, nRoleID2, vAccountID, SQL_NEED_CALLBACK, DELETE, tSQL );
	}
	else if ( nType == Judge_Bad )
	{
		sprintf( tSQL, "delete from UMS_JUDGE_BAD where role_src = %d and role_des = %d;", nRoleID1, nRoleID2 );
		CGateLogic::GetSingleton().ExecuteSql( em_dbsession_delete_judge_bad, nRoleID1, nRoleID2, vAccountID, SQL_NEED_CALLBACK, DELETE, tSQL );
	}	

	return ;
}


// ***********************************************************
//  Function:		AttachJudgeRecordMemory
//  Description:	insert judge record into memory
//  Output:			
// **********************************************************
int CJudgeModule::AttachJudgeRecordMemory( CGatePlayer* npPlayer, JudgeRecord* npRecord, JudgeType nType )
{
	if ( npPlayer == NULL || npRecord == NULL )
	{
		return -1;
	}

	CJudge* tpJudge = NULL;

	if ( nType == Judge_Favor )
	{
		JudgeOtherType::iterator it = mFavorJudge.find( npPlayer->mCharID );
		if ( it == mFavorJudge.end() )
			return -1;
		tpJudge = (CJudge*)&(it->second);
	}
	else
	{
		JudgeOtherType::iterator it = mBadJudge.find( npPlayer->mCharID );
		if ( it == mBadJudge.end() )
			return -1;
		tpJudge = (CJudge*)&(it->second);
	}

	int tRet = 0;

	if ( tpJudge != NULL )
		tRet = tpJudge->AttachJudge( npRecord->mCharID, npRecord->mValue, npRecord->mTimeStamp, npRecord->mName );

	return tRet;
}


// ***********************************************************
//  Function:		InsertJudgeRecordIntoDB
//  Description:	insert judge record into db
//  Output:			
// **********************************************************
void CJudgeModule::InsertJudgeRecordIntoDB( CGatePlayer* npPlayer, JudgeRecord* npRecord, JudgeType nType )
{
	if ( npPlayer == NULL || npRecord == NULL )
	{
		return ;
	}

	char tSQL[ 1024 ] = { 0 };

	if ( nType == Judge_Favor )
	{
		sprintf( tSQL, "insert into UMS_JUDGE_FAVOR( role_src, role_des, value, timestamp, type ) values( %d, %d, %d, %d, %d );", 
			npPlayer->mCharID, npRecord->mCharID, npRecord->mValue, npRecord->mTimeStamp, Judge_Favor );
		CGateLogic::GetSingleton().ExecuteSql( em_dbsession_save_judge_favor, npPlayer->mCharID, npRecord->mCharID, 
			npPlayer->mAccountID, SQL_NEED_CALLBACK, INSERT, tSQL );
	}
	else if ( nType == Judge_Bad )
	{
		sprintf( tSQL, "insert into UMS_JUDGE_BAD( role_src, role_des, value, timestamp, type ) values( %d, %d, %d, %d, %d );", 
			npPlayer->mCharID, npRecord->mCharID, npRecord->mValue, npRecord->mTimeStamp, Judge_Bad );
		CGateLogic::GetSingleton().ExecuteSql( em_dbsession_save_judge_bad, npPlayer->mCharID, npRecord->mCharID, 
			npPlayer->mAccountID, SQL_NEED_CALLBACK, INSERT, tSQL );
	}

	return ;
}

// ***********************************************************
//  Function:		DeleteJudgeRecordFromDB
//  Description:	insert judge record into db
//  Output:			
// **********************************************************
void CJudgeModule::AttachJudgeRecordIntoDB( CGatePlayer* npPlayer, JudgeRecord* npRecord, JudgeType nType )
{
	if ( npPlayer == NULL || npRecord == NULL )
	{
		return ;
	}

	char tSQL[ 1024 ] = { 0 };

	if ( nType == Judge_Favor )
	{
		sprintf( tSQL, "update UMS_JUDGE_FAVOR set value = %d, timestamp = %d where role_src = %d and role_des = %d;", 
			 npRecord->mValue, npRecord->mTimeStamp, npPlayer->mCharID, npRecord->mCharID );
		CGateLogic::GetSingleton().ExecuteSql( em_dbsession_save_judge_favor, npPlayer->mCharID, npRecord->mCharID, 
			npPlayer->mAccountID, SQL_NEED_CALLBACK, UPDATE, tSQL );
	}
	else if ( nType == Judge_Bad )
	{
		sprintf( tSQL, "update UMS_JUDGE_BAD set value = %d, timestamp = %d where role_src = %d and role_des = %d;", 
			npRecord->mValue, npRecord->mTimeStamp, npPlayer->mCharID, npRecord->mCharID );
		CGateLogic::GetSingleton().ExecuteSql( em_dbsession_save_judge_bad, npPlayer->mCharID, npRecord->mCharID, 
			npPlayer->mAccountID, SQL_NEED_CALLBACK, UPDATE, tSQL );
	}
	 
	return ;
}



// ***********************************************************
//  Function:		QueryRecJudgeDataFromDB
//  Description:	query rec judge record from db server
//  Output:			
// **********************************************************
void CJudgeModule::QueryRecJudgeDataFromDB( CGatePlayer* npPlayer, int nCondition, int nParam /* = 0 */ )
{
	if ( npPlayer == NULL )
	{
		return ;
	}

	char tSQL[ 1024 ] = { 0 };
	
	if ( nCondition == Judge_Query_Time )
	{
		sprintf( tSQL, "(select role_src, role_des, value, timestamp, type from UMS_JUDGE_FAVOR "
			"where role_des = %d order by timestamp desc limit 30) union all (select role_src, role_des, value,"
			"timestamp, type from UMS_JUDGE_BAD where role_des = %d order by timestamp desc limit 30);",
			npPlayer->mCharID, npPlayer->mCharID );
	}
	else if ( nCondition == Judge_Query_Value )
	{
		sprintf( tSQL, "(select role_src, role_des, value, timestamp, type from UMS_JUDGE_FAVOR "
			"where role_des = %d order by value desc limit 30) union all (select role_src, role_des, value,"
			"timestamp, type from UMS_JUDGE_BAD where role_des = %d order by value desc limit 30);",
			npPlayer->mCharID, npPlayer->mCharID );
	}	
	else if ( nCondition == Judge_Query_Sum )
	{
		sprintf( tSQL, "(select sum( value ), type from UMS_JUDGE_FAVOR where role_des = %d)"
			"union all (select sum( value ), type from UMS_JUDGE_BAD where role_des = %d);",
			npPlayer->mCharID, npPlayer->mCharID );
	}

	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_query_recjudge, npPlayer->mCharID, nParam, 
		npPlayer->mAccountID, SQL_NEED_CALLBACK, SELECT, tSQL );

	return ;
}







// 评价修改通知 [12/8/2009 Macro]
void CJudgeModule::OnEventJudgeModifyNotice( int nServerID, int nRoleID )
{
	CMessage tMessage;
	CMessageJudgeModifySuccessNotice tPara;
	tMessage.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_JUDGEMODIFY );
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_charid( nRoleID );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
	return ;
}

// 删除角色相关的评价记录 [12/11/2009 Macro]
void CJudgeModule::OnEventClearRoleJudgeDB( int vAccountID, int nRoleID )
{
	char tFavorSQL[ 256 ] = { 0 };
	char tBadSQL[ 256 ] = { 0 };

	sprintf( tFavorSQL, "delete from UMS_JUDGE_FAVOR where role_src = %d or role_des = %d;", nRoleID, nRoleID );
	sprintf( tBadSQL, "delete from UMS_JUDGE_BAD where role_src = %d or role_des = %d;", nRoleID, nRoleID );

	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_delete_judge_favor, nRoleID, 0, vAccountID, SQL_NONE, DELETE, tFavorSQL );
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_delete_judge_bad, nRoleID, 0, vAccountID, SQL_NONE, DELETE, tBadSQL );

	return ;
}


CJudge* CJudgeModule::GetJudge( int nRoleID, int nType )
{
	if ( nType == Judge_Favor )
	{
		JudgeOtherType::iterator it = mFavorJudge.find( nRoleID );
		if ( it != mFavorJudge.end() )
			return (CJudge*)&(it->second);		
	}
	else
	{
		JudgeOtherType::iterator it = mBadJudge.find( nRoleID );
		if ( it != mBadJudge.end() )
			return (CJudge*)&(it->second);		
	}

	return NULL;
}
