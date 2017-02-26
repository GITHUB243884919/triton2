#include "judgemodule.h"
#include "judgemessage_pb.hxx.pb.h"

#include "base.h"
#include "entity.h"


int CJudgeFavor::GetJudgeList( CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return -1;
	}

	CMessageQueryJudgeListResponse* tpMessage = ( CMessageQueryJudgeListResponse*) npMessage->msgpara();

	JudgeList::iterator it = mJudgeList.begin();
	for ( ; it != mJudgeList.end(); ++ it )
	{
		CRoleGlobalInfo* tpRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( it->mCharID );
		if ( tpRoleInfo == NULL ) continue ;

		PBJudgeRecord* tpRecord = tpMessage->add_favorjudgeotherlist();
		tpRecord->set_srccharid( mCharID );
		tpRecord->set_srcname( mName );
		tpRecord->set_descharid( it->mCharID );
		tpRecord->set_desname( it->mName );
		tpRecord->set_type( Judge_Favor );
		tpRecord->set_value( it->mValue );
		tpRecord->set_timestamp( it->mTimeStamp );
		tpRecord->set_face( tpRoleInfo->FaceType() );
		tpRecord->set_sex( tpRoleInfo->GenderID() );
	}

	return 0;
}

int CJudgeBad::GetJudgeList( CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return -1;
	}

	CMessageQueryJudgeListResponse* tpMessage = ( CMessageQueryJudgeListResponse*) npMessage->msgpara();

	JudgeList::iterator it = mJudgeList.begin();
	for ( ; it != mJudgeList.end(); ++ it )
	{
		CRoleGlobalInfo* tpRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( it->mCharID );
		if ( tpRoleInfo == NULL ) continue ;

		PBJudgeRecord* tpRecord = tpMessage->add_badjudgeotherlist();
		tpRecord->set_srccharid( mCharID );
		tpRecord->set_srcname( mName );
		tpRecord->set_descharid( it->mCharID );
		tpRecord->set_desname( it->mName );
		tpRecord->set_type( Judge_Bad );
		tpRecord->set_value( it->mValue );
		tpRecord->set_timestamp( it->mTimeStamp );
		tpRecord->set_face( tpRoleInfo->FaceType() );
		tpRecord->set_sex( tpRoleInfo->GenderID() );
	}

	return 0;
}

// ***********************************************************
//  Function:		OnDBMessage
//  Description:	场景数据库服务器返回的消息
//  Output:			
// **********************************************************
void CJudgeModule::OnDBMessage( CMessage* pMessage, int nParam1, int nParam2, int nSessionType )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	switch ( nSessionType )
	{
	case em_dbsession_query_judge:
		{
			OnSessionLoadJudgeData( pMessage, nParam1, nParam2, nSessionType );
			break;
		}
	case em_dbsession_query_recjudge:
		{
			OnSessionQueryRecJudgeData( pMessage, nParam1, nParam2, nSessionType );
			break;
		}
	case em_dbsession_save_judge_favor:
	case em_dbsession_save_judge_bad:
		{
			OnSessionAttachJudgeRecord( pMessage, nParam1, nParam2, nSessionType );
			break;
		}
	case em_dbsession_delete_judge_favor:
	case em_dbsession_delete_judge_bad:
		{
			OnSessionDeleteJudgeRecord( pMessage, nParam1, nParam2, nSessionType );
			break;
		}
	case em_dbsession_query_rec_judge_sum:
		{
			OnSessionQueryRecSum( pMessage, nParam1, nParam2, nSessionType );
			break;
		}
	default:
		break;
	}
	return ;
}

// ***********************************************************
//  Function:		OnMessage
//  Description:	客户端消息
//  Output:			
// **********************************************************
void CJudgeModule::OnMessage( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}
	int nMessageID = pMessage->mutable_msghead()->messageid();

	switch ( nMessageID )
	{
	case ID_C2S_REQUEST_MODIFYJUDGESINGLE:
		{
			OnMessageSingleJudgeRequest( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_MODIFYJUDGELIST:
		{
			OnMessageMultiJudgeRequest( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_QUERYJUDGELIST:
		{
			OnMessageQueryJudgeRequest( nServerID, pMessage );
			break;
		}
	default:
		break;
	}
	return ;
}

// ***********************************************************
//  Function:		OnSeverMessage
//  Description:	场景服务器的消息
//  Output:			
// **********************************************************
void CJudgeModule::OnSeverMessage(int nServerID, CMessage* pMessage )
{

}

// ***********************************************************
//  Function:		OnMessageMultiJudgeRequest
//  Description:	rec request from scene server
//  Output:			
// **********************************************************
void CJudgeModule::OnMessageMultiJudgeRequest( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageModifyJudgeListRequest* tpReq = ( CMessageModifyJudgeListRequest* ) npMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpReq->srccharid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageModifyJudgeListResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_MODIFYJUDGELIST );
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_type( tpReq->type() ); 

	// 修改玩家个数为零
	if ( tpReq->data_size() == 0 )
	{
		tPara.set_rtncode( ERR_JUDGE_MODIFYNONE );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}

	CJudge* tpJudge = GetJudge( tpPlayer->CharID(), tpReq->type() );

	if ( tpJudge == NULL )
	{
		return ;
	}	

	// 检查支配点数是否足够
	int tPayPoint = 0;
	for ( int i = 0; i < tpReq->data_size(); i ++  )
	{
		PBJudgeRecord* tpJudgeData = tpReq->mutable_data( i );
		int tValue = tpJudge->GetJudgeValue( tpJudgeData->descharid() );
		tPayPoint -= tValue - tpJudgeData->value();
	}

	if ( tpJudge->GetValidScore() < tPayPoint )
	{
		tPara.set_rtncode( ERR_JUDGE_SCORENOTENOUGH );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}


	bool bAttachFailure = false;

	for ( int i = 0; i < tpReq->data_size(); i ++ )
	{
		PBJudgeRecord* tpJudgeData = tpReq->mutable_data( i );
		if ( tpJudgeData->value() == 0 )
		{
			DeleteJudgeRecordFromDB( tpPlayer->mAccountID, tpPlayer->mCharID, tpJudgeData->descharid(), tpJudgeData->type() );
		}
		else
		{
			JudgeRecord tRecord( tpJudgeData->descharid(), tpJudgeData->value(), time(NULL), NULL );
			int tRet = AttachJudgeRecordMemory( tpPlayer, &tRecord, (JudgeType)tpJudgeData->type() );
			if ( tRet < 0 )
			{
				LOG_ERROR( "judge", "Attach judge record memory failed.Value(%d),Name(%s),[%s:%d]", tpJudgeData->value(), 
					tpPlayer->mCharName, __LK_FILE__, __LINE__ );
				if ( ERR_NEED_NOTIFY( tRet ) )
				{
					tPara.set_rtncode( tRet );
					CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
				}
				bAttachFailure = true;
				break ;
			}
			AttachJudgeRecordIntoDB( tpPlayer, &tRecord, (JudgeType)tpJudgeData->type() );
		}
		SendJudgeModifyNotice( tpPlayer, tpJudgeData->value(), tpReq->type(), tpJudgeData->descharid() );
	}

	if ( bAttachFailure )
	{
		// 修改数据失败,已经存库成功得不再回滚，通知客户端强制刷新数据，以当前数据库数据为准
		//QueryRecJudgeDataFromDB( tpPlayer, Judge_Query_Time );
		return ;
	}

	tPara.set_rtncode( SUCCESS );
	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );

	// 通知场景 [12/8/2009 Macro]
	OnEventJudgeModifyNotice( nServerID, tpPlayer->CharID() );
	return ;
}

// ***********************************************************
//  Function:		OnMessageQueryJudgeRequest
//  Description:	rec request from scene server
//  Output:			
// **********************************************************
void CJudgeModule::OnMessageQueryJudgeRequest( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageQueryJudgeListRequest* tpReq = ( CMessageQueryJudgeListRequest* ) npMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpReq->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	if( tpReq->action() == Judge_Other )
	{
		CMessage tMessage;
		CMessageQueryJudgeListResponse tPara;
		tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYJUDGELIST );
		tMessage.set_msgpara( (long) &tPara );

		JudgeOtherType::iterator it = mFavorJudge.find( tpPlayer->mCharID );
		if ( it != mFavorJudge.end() )
		{
			CJudgeFavor* tpJudge = ( CJudgeFavor* ) &(it->second);
			if ( tpJudge != NULL )
			{
				tPara.set_favortotal( tpJudge->GetTotalScore() ); 
				tpJudge->GetJudgeList( &tMessage );
			}
		}

		it = mBadJudge.find( tpPlayer->mCharID );
		if ( it != mBadJudge.end() )
		{
			CJudgeBad* tpJudge = ( CJudgeBad* ) &(it->second);
			if ( tpJudge != NULL )
			{
				tPara.set_badtotal( tpJudge->GetTotalScore() );
				tpJudge->GetJudgeList( &tMessage );
			}
		}

		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
	}
	else
		QueryRecJudgeDataFromDB( tpPlayer, tpReq->condition() );

	return ;
}

// 查询单条评价记录 [12/8/2009 Macro]
void CJudgeModule::OnMessageQuerySingleJudgeRequest( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	return ;
}

// ***********************************************************
//  Function:		OnMessageSingleJudgeRequest
//  Description:	rec request from scene server
//  Output:			
// **********************************************************
void CJudgeModule::OnMessageSingleJudgeRequest( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageModifySingleJudgeRequest* tpReq = ( CMessageModifySingleJudgeRequest* ) npMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpReq->srccharid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageModifySingleJudgeResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_MODIFYJUDGESINGLE );
	tMessage.set_msgpara( (long) &tPara );

	if ( tpReq->value() == 0 )
	{
		tPara.set_rtncode( ERR_JUDGE_NEEDSCORE );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}	

	// 检查角色名是否正确
	CRoleGlobalInfo* tpRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleName( tpReq->desname().c_str() );
	if( tpRoleInfo == NULL )
	{
		tPara.set_rtncode( ERR_JUDGE_PLAYERNOTEXIST );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}

	// 检查评价对方是否在线
	CGatePlayer* tpDesPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpRoleInfo->RoleID() );
	if ( tpDesPlayer == NULL )
	{
		tPara.set_rtncode( ERR_JUDGE_DESOFFLINE );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}

	// 对方未开启好评系统
	if ( tpDesPlayer->mLevel < JUDGE_BASE_LEVEL )
	{
		tPara.set_rtncode( ERR_JUDGE_DESLEVELLOWER );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}

	if ( !(tpReq->type() == Judge_Favor || tpReq->type() == Judge_Bad) )
	{
		LOG_ERROR( "judge", "Client Send Judge Type failed.(%d)[%s:%d]", tpReq->type(), __LK_FILE__, __LINE__ );
		return ;
	}	

	CJudge* tpJudge = GetJudge( tpPlayer->CharID(), tpReq->type() );
	if ( tpJudge == NULL )
	{
		return ;
	}

	// 检查点数是否足够
	if ( tpJudge->GetValidScore() < tpReq->value() )
	{
		tPara.set_rtncode( ERR_JUDGE_SCORENOTENOUGH );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
	}

	JudgeRecord tRecord( tpDesPlayer->mCharID, tpReq->value(), time(NULL), tpDesPlayer->mCharName );
	int tRet = AttachJudgeRecordMemory( tpPlayer, &tRecord, (JudgeType)tpReq->type() );
	if ( tRet < 0 )
	{
		LOG_ERROR( "judge", "Attach judge record memory failed.Value(%d),Name(%s),[%s:%d]", tpReq->value(), 
			tpPlayer->mCharName, __LK_FILE__, __LINE__ );
		if ( ERR_NEED_NOTIFY( tRet ) )
		{
			tPara.set_rtncode( tRet );
			CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		}
		return ;
	}

	if ( tRet == RECORD_EXIST )
		AttachJudgeRecordIntoDB( tpPlayer, &tRecord, (JudgeType)tpReq->type() );
	else
		InsertJudgeRecordIntoDB( tpPlayer, &tRecord, (JudgeType)tpReq->type() );

	tPara.set_type( tpReq->type() );
	tPara.set_descharid( tpDesPlayer->mCharID );
	tPara.set_desname( tpDesPlayer->mCharName );
	tPara.set_value( tpReq->value() );
	tPara.set_face( tpDesPlayer->mFaceType );
	tPara.set_sex( tpDesPlayer->mSexID );
	tPara.set_rtncode( SUCCESS );
	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );

	// 给对方发通知
	CMessage tMsg;
	CMessageModifyJudgeNotice tNotice;
	tMsg.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MODIFYJUDGE );
	tMsg.set_msgpara( (long) &tNotice );
	tNotice.set_type( tpReq->type() );
	tNotice.set_value( tpReq->value() );
	tNotice.set_name( tpPlayer->mCharName );
	tNotice.set_charid( tpPlayer->mCharID );
	tNotice.set_face( tpPlayer->mFaceType );
	tNotice.set_sex( tpPlayer->mSexID );
	CGateServer::GetSingleton().Send2Player( tpDesPlayer, &tMsg );

	// 通知场景 [12/8/2009 Macro]
	OnEventJudgeModifyNotice( nServerID, tpPlayer->CharID() );

	return ;
}

// ***********************************************************
//  Function:		OnSessionAttachJudgeRecord
//  Description:	rec the response from db server
//  Output:			
// **********************************************************
void CJudgeModule::OnSessionAttachJudgeRecord( CMessage* npMessage, int nParam1, int nParam2, int nSession )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) npMessage->msgpara();
	if ( tpMsg == NULL )
	{
		return ;
	}

	if ( tpMsg->resultcode() == 1 )
	{
		// commit successfully
		return ;
	}

	// commit failed, rollback the latest record
	if ( nSession == em_dbsession_save_judge_favor )
	{
		JudgeOtherType::iterator it = mFavorJudge.find( nParam1 );
		if ( it == mFavorJudge.end() )
			return ;
		CJudge* tpJudge = (CJudge*)&(it->second);
		if ( tpJudge != NULL )
			tpJudge->DeleteJudge( nParam2 );
	}
	else if ( nSession == em_dbsession_save_judge_bad )
	{
		JudgeOtherType::iterator it = mBadJudge.find( nParam1 );
		if ( it == mBadJudge.end() )
			return ;
		CJudge* tpJudge = (CJudge*)&(it->second);
		if ( tpJudge != NULL )
			tpJudge->DeleteJudge( nParam2 );
	}

	return ;
}
// ***********************************************************
//  Function:		OnSessionDeleteJudgeRecord
//  Description:	rec response from db server
//  Output:			
// **********************************************************

void CJudgeModule::OnSessionDeleteJudgeRecord( CMessage* npMessage, int nParam1, int nParam2, int nSession )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) npMessage->msgpara();
	if ( tpMsg == NULL )
	{
		return ;
	}

	if ( tpMsg->resultcode() != 1 )
	{
		LOG_ERROR( "judge", "Execute db failed.Role(%d):Role(%d),[%s:%d]", nParam1, nParam2, __LK_FILE__, __LINE__ );
		return ;
	}

	if ( nSession == em_dbsession_delete_judge_favor )
	{
		JudgeOtherType::iterator it = mFavorJudge.find( nParam1 );
		if ( it == mFavorJudge.end() )
			return ;
		CJudge* tpJudge = (CJudge*)&(it->second);
		if ( tpJudge != NULL )
			tpJudge->DeleteJudge( nParam2 );
	}
	else if ( nSession == em_dbsession_delete_judge_bad )
	{
		JudgeOtherType::iterator it = mBadJudge.find( nParam1 );
		if ( it == mBadJudge.end() )
			return ;
		CJudge* tpJudge = (CJudge*)&(it->second);
		if ( tpJudge != NULL )
			tpJudge->DeleteJudge( nParam2 );
	}

	return ;
}

// ***********************************************************
//  Function:		OnSessionLoadJudgeData
//  Description:	load 玩家得好评、恶评数据(对他人得)
//  Output:			
// **********************************************************
void CJudgeModule::OnSessionLoadJudgeData( CMessage* npMessage, int nParam1, int nParam2, int nSession )
{
	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) npMessage->msgpara();
	if ( tpMsg == NULL )
	{
		return ;
	}

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nParam1 );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	if ( tpMsg->resultcode() != 1 )
	{
		return ;
	}

	int tcolnum = tpMsg->colcount();
	int trownum = tpMsg->rowcount();	

	CJudge tJudgeFavor;
	CJudge tJudgeBad;
	tJudgeFavor.SetJudgerInfo( tpPlayer->mCharID, tpPlayer->mCharName );
	tJudgeBad.SetJudgerInfo( tpPlayer->mCharID, tpPlayer->mCharName );

	for ( int i = 0; i < trownum; i ++ )
	{
		int tRoleID = atoi( tpMsg->fieldvalue( i*tcolnum + 1 ).c_str() );
		int tValue  = atoi( tpMsg->fieldvalue( i*tcolnum + 2 ).c_str() );
		int tTimeStamp = atoi( tpMsg->fieldvalue( i*tcolnum + 3 ).c_str() );
		int tType	= atoi( tpMsg->fieldvalue( i*tcolnum + 4 ).c_str() );
		CRoleGlobalInfo* tpRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tRoleID );
		if ( tpRoleInfo == NULL )
			continue ;
		if ( tType == Judge_Favor )
			tJudgeFavor.AttachJudge( tRoleID, tValue, tTimeStamp, tpRoleInfo->RoleName() );
		else
			tJudgeBad.AttachJudge( tRoleID, tValue, tTimeStamp, tpRoleInfo->RoleName() );
	}

	JudgeOtherType::iterator it = mFavorJudge.find( nParam1 );
	if ( it != mFavorJudge.end() )
		mFavorJudge.erase( it );
	mFavorJudge.insert( JudgeOtherType::value_type( nParam1, tJudgeFavor ) );

	it = mBadJudge.find( nParam1 );
	if ( it != mBadJudge.end() )
		mBadJudge.erase( it );
	mBadJudge.insert( JudgeOtherType::value_type( nParam1, tJudgeBad ) );

	return ;
}

// ***********************************************************
//  Function:		OnSessionQueryRecJudgeData
//  Description:	rec response from db server
//  Output:			
// **********************************************************
void CJudgeModule::OnSessionQueryRecJudgeData( CMessage* npMessage, int nParam1, int nParam2, int nSession )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) npMessage->msgpara();
	if ( tpMsg == NULL )
	{
		return ;
	}

	if ( tpMsg->resultcode() != 1 )
	{
		// Query failed.
		return ;
	}

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nParam1 );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	if ( nParam2 == Judge_Query_Sum )
	{
		CMessage tMessage;
		CMessageUpdateMaxJudgeValueNotice tPara;
		tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_JUDGEMAX );
		tMessage.set_msgpara( (long) &tPara );

		int tRowCnt = tpMsg->rowcount();
		int tColCnt = tpMsg->colcount();

		for ( int i = 0; i < tRowCnt; i ++ )
		{
			int tValue = atoi( tpMsg->fieldvalue( i*tColCnt + 0 ).c_str() );
			int tType  = atoi( tpMsg->fieldvalue( i*tColCnt + 1 ).c_str() );

			if ( tType == Judge_Favor )
				tPara.set_favortotal( tValue );
			else
				tPara.set_badtotal( tValue );
		}

		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}

	int tRowCnt = tpMsg->rowcount();
	int tColCnt = tpMsg->colcount();

	CMessage tMessage;
	CMessageQueryJudgeListResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYJUDGELIST );
	tMessage.set_msgpara( (long) &tPara );

	// 收到得评价数据从数据库读入，载入消息
	for ( int i = 0; i < tRowCnt; i ++ )
	{		
		int tSrcRoleID = atoi( tpMsg->fieldvalue( i*tColCnt + 0 ).c_str() );
		int tDesRoleID = atoi( tpMsg->fieldvalue( i*tColCnt + 1 ).c_str() );
		int tValue = atoi( tpMsg->fieldvalue( i*tColCnt + 2 ).c_str() );
		int tTimeStamp = atoi( tpMsg->fieldvalue( i*tColCnt + 3 ).c_str() );
		int tType = atoi( tpMsg->fieldvalue( i*tColCnt + 4 ).c_str() );

		CRoleGlobalInfo* tpSrcRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tSrcRoleID );
		if ( tpSrcRoleInfo == NULL ) continue ;
		CRoleGlobalInfo* tpDesRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tDesRoleID );
		if ( tpDesRoleInfo == NULL ) continue ;	

		PBJudgeRecord* tpRecord = NULL;
		if ( tType == Judge_Favor )
			tpRecord = tPara.add_favorrecjudgelist();
		else if ( tType == Judge_Bad )
			tpRecord = tPara.add_badrecjudgelist();

		if ( tpRecord == NULL )
			continue;

		tpRecord->set_srccharid( tpSrcRoleInfo->RoleID() );
		tpRecord->set_srcname( tpSrcRoleInfo->RoleName() );
		tpRecord->set_descharid( tpDesRoleInfo->RoleID() );
		tpRecord->set_desname( tpDesRoleInfo->RoleName() );
		tpRecord->set_value( tValue );
		tpRecord->set_type( tType );
		tpRecord->set_timestamp( tTimeStamp );
		tpRecord->set_face( tpSrcRoleInfo->FaceType() );
		tpRecord->set_sex( tpSrcRoleInfo->GenderID() );
	}

	JudgeOtherType::iterator it = mFavorJudge.find( tpPlayer->mCharID );
	if ( it != mFavorJudge.end() )
	{
		CJudgeFavor* tpJudge = ( CJudgeFavor* ) &(it->second);
		if ( tpJudge != NULL )
		{
			tPara.set_favortotal( tpJudge->GetTotalScore() ); 
			tpJudge->GetJudgeList( &tMessage );
		}
	}

	it = mBadJudge.find( tpPlayer->mCharID );
	if ( it != mBadJudge.end() )
	{
		CJudgeBad* tpJudge = ( CJudgeBad* ) &(it->second);
		if ( tpJudge != NULL )
		{
			tPara.set_badtotal( tpJudge->GetTotalScore() );
			tpJudge->GetJudgeList( &tMessage );
		}
	}

	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
	QueryRecJudgeDataFromDB( tpPlayer, Judge_Query_Sum, Judge_Query_Sum );
	return ;
}

// 查询玩家收到的评价数据总值回调 [12/22/2009 Macro]
void CJudgeModule::OnSessionQueryRecSum( CMessage* npMessage, int nParam1, int nParam2, int nSession )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) npMessage->msgpara();
	if ( tpMsg == NULL )
	{
		return ;
	}

	if ( tpMsg->resultcode() != 1 )
	{
		// Query failed.
		return ;
	}

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nParam1 );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	int tcolnum = tpMsg->colcount();
	int trownum = tpMsg->rowcount();	

	JudgeOtherType::iterator itFavor = mFavorJudge.find( nParam1 );
	JudgeOtherType::iterator itBad = mBadJudge.find( nParam1 );
	if ( itFavor == mFavorJudge.end() || itBad == mBadJudge.end() )
	{
		return ;
	}

	CJudge* tpFavor = ( CJudge* ) &( itFavor->second );
	CJudge* tpBad = ( CJudge* ) &( itBad->second );

	for ( int i = 0; i < trownum; i ++ )
	{
		int tSum	= atoi( tpMsg->fieldvalue( i * tcolnum + 0 ).c_str() );
		int tType	= atoi( tpMsg->fieldvalue( i * tcolnum + 1 ).c_str() ); 

		if ( tType == Judge_Favor )
		{
			tpFavor->SetRecTotalScore( tSum );
		}
		else if ( tType == Judge_Bad )
		{
			tpBad->SetRecTotalScore( tSum );
		}
	}
	return ;
}

void CJudgeModule::SendJudgeModifyNotice( CGatePlayer* npPlayer, int nValue, int nType, int nDesRoleID )
{
	if ( npPlayer == NULL )
	{
		return ;
	}

	CGatePlayer* tpDesPlayer = CPlayerManager::GetSingleton().ScenePlayer( nDesRoleID );
	if ( tpDesPlayer == NULL )
	{
		return ;
	}

	CMessage tMsg;
	CMessageModifyJudgeNotice tNotice;
	tMsg.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MODIFYJUDGE );
	tMsg.set_msgpara( (long) &tNotice );
	tNotice.set_type( nType );
	tNotice.set_value( nValue );
	tNotice.set_name( npPlayer->mCharName );
	tNotice.set_charid( npPlayer->mCharID );
	tNotice.set_face( npPlayer->mFaceType );
	tNotice.set_sex( npPlayer->mSexID );
	CGateServer::GetSingleton().Send2Player( tpDesPlayer, &tMsg );
}
