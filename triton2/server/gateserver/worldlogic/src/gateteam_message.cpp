#include "westserver.h"
#include "errcode.h"
#include "teamtype.h"
#include "gateteammanager.h"
#include "gatelogic.h"

#include "servermessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "teammessage_pb.hxx.pb.h"
#include "promessage_pb.hxx.pb.h"
#include "core_service.h"

void CServiceGateTeam::OnMessage(int nServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL, return);

	CMessageHead* tpHead = tpMessage->mutable_msghead();
	LK_ASSERT(tpHead != NULL, return);

	unsigned short nMessageID = (unsigned short) tpHead->messageid();
	switch (nMessageID)
	{
	case ID_S2G_REQUEST_CREATETEAMATGATE:
		{
			OnMessageCreateTeamBySelf( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_JOINREQUEST:
		{
			OnMessageRequestJoinTeam( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_REPLYJOINREQUEST:
		{
			OnMessageReplyJoinRequest( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_JOININVITE:
		{
			OnMessageInviteJoinTeam( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_REPLYJOININVITE:
		{
			OnMessageReplyJoinInvite( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_KICKOUTTEAM:
		{
			OnMessageKickoutTeamRequest( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_CHANGEHEADER:
		{
			OnMessageChangeHeaderRequest( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_LEAVETEAM:
		{
			OnMessageLeaveTeamRequest( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_ENDTEAM:
		{
			OnMessageEndTeamRequest( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_UPDATETEAMPRO:
		{
			OnMessageUpdateGateTeam( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_CLAIMRECRUIT:
		{
			OnMessageSendClaimRecruitRequest( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_QUERYCLAIMRECRUIT:
		{
			OnMessageQueryClaimRecruitRequest( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_DELETECLAIMRECRUIT:
		{
			OnMesssageDeleteClaimRecruitRequest( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_FOLLOW:
		{
			OnMessageFollowRequest( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_DISTRIBUTE:
		{
			OnMessageChangeDistribute( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_OBTAINTEAMATGATE:
		{
			OnMessageObtainGateTeamRequest( tpMessage );
			break;
		}
	case ID_S2G_REQUEST_CLEARRECORD:
		{
			OnMessageClearInvReqRequest( tpMessage );
			break;
		}
	default:
		{
			LOG_INFO( "gateteam", "There is not action to be suited, MessageID=%d", nMessageID );
			break;
		}
	}
	return ;
}

void CServiceGateTeam::OnMessageCreateTeamBySelf( CMessage* pMessage )
{
	CMessageCreateGateTeamRequest* tpReq = ( CMessageCreateGateTeamRequest* ) pMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpReq->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}
	int tTeamID = InTeam( tpReq->charid() );
	if ( tTeamID != INVALID_OBJ_ID )
	{
		SendErrorNotice( tpPlayer, ERROR_TEAM_CREATE_HASINTEAM );
		return ;
	}

	CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().CreateObject( OBJTYPE_TEAM_AT_GATE );
	if ( tpTeam == NULL )
	{
		LOG_ERROR( "gateteam", "Create Object failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return ;
	}

	tpTeam->SetHeadID( tpPlayer->mCharID );
	tpTeam->SetHeadName( tpPlayer->mCharName );
	tpTeam->AddMember( tpPlayer );
	tpTeam->SetMode( RANDOM );
	InsertMember( tpPlayer->mCharID, tpTeam->get_id() );

	CMessage tMessage;
	CMessageCreateGateTeamResponse tPara;
	tPara.set_charid( tpPlayer->mCharID );
	tPara.set_teamid( tpTeam->get_id() );
	pbmsg_sethead(&tMessage, ID_G2S_RESPONSE_CREATETEAM, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara( (long) &tPara );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );

	SendErrorNotice( tpPlayer, ERROR_TEAM_CREATE_SUCCESS );

	DeleteClaimRecruit( tpPlayer );

	ClearValuePair< BaseReqInvMap >( mInviteList, tpPlayer->mCharID );
	ClearValuePair< BaseReqInvMap >( mRequestList, tpPlayer->mCharID );

	return ;
}

void CServiceGateTeam::OnMessageRequestJoinTeam( CMessage* pMessage )
{
	CMessageJoinGateTeamRequest* tpReq = ( CMessageJoinGateTeamRequest* ) pMessage->msgpara();

	CGatePlayer* tpSrcPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpReq->srccharid() );
	CGatePlayer* tpDesPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpReq->descharid() );
	if ( tpSrcPlayer == NULL )
	{
		return ;
	}

	if ( tpDesPlayer == NULL )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_DESOFFLINE );
		return ;
	}

	if ( InTeam( tpSrcPlayer->mCharID ) != INVALID_OBJ_ID )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_REQEUST_SRCINTEAM );
		return ;
	}
	int tTeamID = InTeam( tpDesPlayer->mCharID );
	if ( tTeamID == INVALID_OBJ_ID )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_DESNOTTEAM, false, tpDesPlayer->mCharName );
		return ;
	}

	if ( tpSrcPlayer->CountryID() != tpDesPlayer->CountryID() )
	{
		if ( IS_DUNGEONSERVER_LINE( tpSrcPlayer->LineID() ) == false || 
				IS_DUNGEONSERVER_LINE( tpDesPlayer->LineID() ) == false )
		{
			SendErrorNotice( tpSrcPlayer, ERROR_TEAM_DIFFNATION );
			return ;
		}		
	}

	CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
	if ( tpTeam == NULL )
	{
		LOG_ERROR( "gateteam", "get object failed.id=%d,[%s:%d]", tTeamID, __LK_FILE__, __LINE__ );
		return ;
	}
	if ( tpTeam->GetCount() == TEAM_NUMBER_MAX )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_FULLTEAM );
		return ;
	}

	if ( tpTeam->GetReqNum() >= MAX_INVITE_NUM )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_MOREREQ );
		return ;
	}

	// send join req to header
	int tHeaderID = tpTeam->GetHeadID();
	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tHeaderID );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "gateteam", "gateplayer invalid,charid=%d,[%s:%d]", tHeaderID, __LK_FILE__, __LINE__ );
		return ;
	}
	CMessage tMessage;
	CMessageJoinGateTeamResponse tPara;
	tPara.set_descharid( tHeaderID );
	int tRtn = MakeTeamItemPB( tPara.mutable_item(), tpSrcPlayer );
	if ( tRtn < 0 )
	{
		LOG_ERROR( "gateteam", "make member pb failed.charid=%d,[%s:%d]", tpSrcPlayer->mCharID, __LK_FILE__, __LINE__ );
		return ;
	}
	pbmsg_sethead(&tMessage, ID_G2S_RESPONSE_JOINREQUEST, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara( (long) &tPara );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
	SendErrorNotice( tpSrcPlayer, ERROR_TEAM_REQUEST_SUCCESS );
	InsertValuePair< BaseReqInvMap >( mRequestList, tpSrcPlayer->mCharID, tTeamID );
	tpTeam->InsertReq( tpSrcPlayer->mCharID );
	return ;
}

void CServiceGateTeam::OnMessageReplyJoinRequest( CMessage* pMessage )
{
	CMessageRequestJoinReply* tpReply = ( CMessageRequestJoinReply* ) pMessage->msgpara();

	CGatePlayer* tpSrcPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReply->srccharid() );
	CGatePlayer* tpDesPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReply->descharid() );
	if ( tpSrcPlayer == NULL )
	{
		LOG_ERROR( "gateteam", "get player failed.srcid=%d,desid=%d.[%s:%d]", 
			tpReply->srccharid(), tpReply->descharid(), __LK_FILE__, __LINE__ );
		return ;
	}

	int tTeamID = InTeam( tpSrcPlayer->mCharID );
	if ( tTeamID == INVALID_OBJ_ID )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_SRCNOTTEAM );
		return ;
	}

	CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
	if ( tpTeam == NULL )
	{
		LOG_ERROR( "gateteam", "object invalid,id=%d,[%s:%d]", tTeamID, __LK_FILE__, __LINE__);
		return ;
	}

	// ignore request
	if ( tpReply->rtncode() == emIgnore )
	{
		RemoveValuePair< BaseReqInvMap >( mRequestList, tpReply->descharid(), tTeamID );
		tpTeam->RemoveReq( tpReply->descharid() );
		return ;
	}

	if ( tpDesPlayer == NULL )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_DESOFFLINE );
		return ;
	}

	if ( tpReply->rtncode() == emRefuse )
	{		
		SendErrorNotice( tpDesPlayer, ERROR_TEAM_REQUEST_REFUSE, false, tpSrcPlayer->mCharName );
		RemoveValuePair< BaseReqInvMap >( mRequestList, tpReply->descharid(), tTeamID );
		tpTeam->RemoveReq( tpReply->descharid() );
		return ;
	}	

	if ( InTeam( tpDesPlayer->mCharID ) != INVALID_OBJ_ID )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_DESINTEAM, false, tpDesPlayer->mCharName );
		return ;
	}	

	if ( tpTeam->IsHeader( tpSrcPlayer->mCharID ) == false )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_NOTHEADER );
		return ;
	}

	if ( tpSrcPlayer->CountryID() != tpDesPlayer->CountryID() )
	{
		if ( IS_DUNGEONSERVER_LINE( tpSrcPlayer->LineID() ) == false || 
			IS_DUNGEONSERVER_LINE( tpDesPlayer->LineID() ) == false )
		{
			SendErrorNotice( tpSrcPlayer, ERROR_TEAM_DIFFNATION );
			return ;
		}				
	}

	if ( tpTeam->GetCount() == TEAM_NUMBER_MAX )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_FULLTEAM );
		return; 
	}

	if ( IsInRelated< BaseReqInvMap >( mRequestList, tpDesPlayer->mCharID, tTeamID ) == false )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_NOTREQUEST );
		return ;
	}

	//SendErrorNotice( tpSrcPlayer, ERROR_TEAM_JOINTEAM, true );
	// process
	tpTeam->AddMember( tpDesPlayer );
	InsertMember( tpDesPlayer->mCharID, tpTeam->get_id() );

	// notify scene
	SendListNotifyToScene( tpTeam, tpDesPlayer->mCharID );
	SendUpdateNotifyToScene( tpTeam, tpDesPlayer->mCharID, ACTION_JOIN );
	SendErrorNotice( tpDesPlayer, ERROR_TEAM_JOINTEAM );

	// update claim message
	if ( tpTeam->GetCount() == TEAM_NUMBER_MAX )
	{
		DeleteClaimRecruit( tpSrcPlayer );
	}
	else
	{
		RefreshRecruitMsg( tpSrcPlayer, tpTeam->GetCount() );
	}
	DeleteClaimRecruit( tpDesPlayer );
	ClearValuePair< BaseReqInvMap >( mRequestList, tpDesPlayer->mCharID );
	ClearValuePair< BaseReqInvMap >( mInviteList, tpDesPlayer->mCharID );
	tpTeam->RemoveReq( tpDesPlayer->mCharID );
	return ;
}

void CServiceGateTeam::OnMessageInviteJoinTeam( CMessage* pMessage )
{
	CMessageJoinGateTeamInvite* tpReq = ( CMessageJoinGateTeamInvite* ) pMessage->msgpara();

	CGatePlayer* tpSrcPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->srccharid() );
	CGatePlayer* tpDesPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->descharid() );
	if ( tpSrcPlayer == NULL )
	{
		return ;
	}

	if ( tpDesPlayer == NULL )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_DESOFFLINE );
		return ;
	}

	if ( tpSrcPlayer->CountryID() != tpDesPlayer->CountryID() )
	{
		if ( IS_DUNGEONSERVER_LINE( tpSrcPlayer->LineID() ) == false || 
			IS_DUNGEONSERVER_LINE( tpDesPlayer->LineID() ) == false )
		{
			SendErrorNotice( tpSrcPlayer, ERROR_TEAM_DIFFNATION );
			return ;
		}				
	}

	if ( InTeam( tpDesPlayer->mCharID ) != INVALID_OBJ_ID )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_DESINTEAM, false, tpDesPlayer->mCharName );
		return ;
	}

	if ( GetRecInvitedNum( tpDesPlayer->mCharID ) >= MAX_RECINVITE_NUM )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_MOREINV );
		return ;
	}

	int tTeamID = InTeam( tpSrcPlayer->mCharID );
	if ( tTeamID != INVALID_OBJ_ID )
	{
		CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
		if ( tpTeam == NULL )
		{
			LOG_ERROR( "gateteam", "object invalid,id=%d,[%s:%d]", tTeamID, __LK_FILE__, __LINE__);
			return ;
		}

		if ( tpTeam->GetCount() == TEAM_NUMBER_MAX )
		{
			SendErrorNotice( tpSrcPlayer, ERROR_TEAM_FULLTEAM );
			return ;
		}

		CMessage tMessage;
		CMessageJoinTeamInviteResponse tPara;
		pbmsg_sethead(&tMessage, ID_G2S_RESPONSE_JOININVITE, 0, 0, 0, 0, 0, 0);
		tPara.set_descharid( tpDesPlayer->mCharID );
		tPara.set_teamid( tTeamID );
		tPara.set_srccharid( tpSrcPlayer->mCharID );
		tPara.set_srcname( tpSrcPlayer->mCharName );
		tPara.set_headname( tpTeam->GetHeadName() );

		int tRtn = tpTeam->MakeTeamPB( tPara.mutable_teaminfo() );
		if ( tRtn < 0 )
		{
			LOG_ERROR( "gateteam", "make team pb failed.id=%d,[%s:%d]", tTeamID, __LK_FILE__, __LINE__ );
			return ;
		}
		tMessage.set_msgpara( (long) &tPara );
		CGateServer::GetSingleton().Send2Scene( tpDesPlayer, &tMessage );
		InsertValuePair< BaseReqInvMap >( mInviteList, tpDesPlayer->mCharID, tTeamID );
	}
	else
	{
		tTeamID = tpSrcPlayer->mCharID;

		CMessage tMsg;
		CMessageJoinTeamInviteResponse tInviteMsg;
		pbmsg_sethead(&tMsg, ID_G2S_RESPONSE_JOININVITE, 0, 0, 0, 0, 0, 0);
		tInviteMsg.set_descharid( tpDesPlayer->mCharID );
		tInviteMsg.set_teamid( tTeamID );
		tInviteMsg.set_srccharid( tpSrcPlayer->mCharID );
		tInviteMsg.set_srcname( tpSrcPlayer->mCharName );
		tInviteMsg.set_headname( tpSrcPlayer->mCharName );
		PBTeamInfo* tpInfo = tInviteMsg.mutable_teaminfo();
		tpInfo->set_headname( tpSrcPlayer->mCharName );
		tpInfo->set_count( 1 );
		tpInfo->set_headercharid( tpSrcPlayer->mCharID );
		tpInfo->set_distributemode( (int)RANDOM );
		int tRtn = MakeTeamItemPB( tpInfo->add_members(), tpSrcPlayer );
		if ( tRtn < 0 )
		{
			LOG_ERROR( "gateteam", "make team item pb failed.id=%d,name=%s,[%s:%d]", tpSrcPlayer->mCharID, tpSrcPlayer->GetCharNameShow(), 
				__LK_FILE__, __LINE__ );
			return ;
		}
		tMsg.set_msgpara( (long) &tInviteMsg );
		CGateServer::GetSingleton().Send2Scene( tpDesPlayer, &tMsg );
		InsertValuePair< BaseReqInvMap >( mInviteList, tpDesPlayer->mCharID, tTeamID );
	}	
	SendErrorNotice( tpSrcPlayer, ERROR_TEAM_INVITE_SUCCESS );
	return ;
}

void CServiceGateTeam::OnMessageReplyJoinInvite( CMessage* pMessage )
{
	CMessageInviteJoinReply* tpReply = ( CMessageInviteJoinReply* ) pMessage->msgpara();

	CGatePlayer* tpSrcPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReply->srccharid() );
	CGatePlayer* tpDesPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReply->descharid() );
	if ( tpSrcPlayer == NULL )
	{
		return ;
	}

	if ( tpReply->rtncode() == emIgnore )
	{
		// ignore, not need to process
		RemoveValuePair< BaseReqInvMap >( mInviteList, tpReply->srccharid(), tpReply->teamid() );
		return ;
	}

	if ( tpDesPlayer == NULL )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_DESOFFLINE );
		return ;
	}

	if ( tpReply->rtncode() == emRefuse )
	{		
		SendErrorNotice( tpDesPlayer, ERROR_TEAM_INVITE_REFUSE, false, tpSrcPlayer->mCharName );
		RemoveValuePair< BaseReqInvMap >( mInviteList, tpReply->srccharid(), tpReply->teamid() );
		return ;
	}	

	// check if their nationality is same
	if ( tpSrcPlayer->mCountryID != tpDesPlayer->mCountryID )
	{
		if ( IS_DUNGEONSERVER_LINE( tpSrcPlayer->LineID() ) == false || 
			IS_DUNGEONSERVER_LINE( tpDesPlayer->LineID() ) == false )
		{
			SendErrorNotice( tpSrcPlayer, ERROR_TEAM_DIFFNATION );
			return ;
		}				
	}

	// the invited entity has been in team
	if ( InTeam( tpSrcPlayer->mCharID ) != INVALID_OBJ_ID )
	{
		SendErrorNotice( tpDesPlayer, ERROR_TEAM_DESINTEAM, false, tpSrcPlayer->mCharName );
		return ;
	}

	// get the team which inviter beyonds in
	int tTeamID = 0;
	tTeamID = InTeam( tpDesPlayer->mCharID );
	if ( tTeamID != INVALID_OBJ_ID )
	{
		// currently, des entity not in team
		if ( (int)tpReply->teamid() == tpDesPlayer->mCharID )
		{
			// when invited, entity not in team
			tTeamID = tpDesPlayer->mCharID;
		}
		else
		{
			tTeamID = tpReply->teamid();
		}
	}
	else
	{
		if ( (int)tpReply->teamid() == tpDesPlayer->mCharID )
		{
			// when invited, entity not in team
			tTeamID = tpDesPlayer->mCharID;
		}
		else if( (int)tpReply->teamid() != tTeamID )
		{
			SendErrorNotice( tpSrcPlayer, ERROR_TEAM_NOTEXIST );
			return ;
		}
	}

	if ( IsInRelated< BaseReqInvMap >( mInviteList, tpSrcPlayer->mCharID, tpReply->teamid() ) == false )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_NOTEXIST );
		return ;
	}

	if ( tTeamID == tpDesPlayer->mCharID )
	{
		CMessage tMsg;
		CMessageCreateGateTeamRequest tCrtTeam;
		tMsg.set_msgpara( (long) &tCrtTeam );
		tCrtTeam.set_charid( tpDesPlayer->mCharID );
		OnMessageCreateTeamBySelf( &tMsg );
		tTeamID = InTeam( tpDesPlayer->mCharID );
	}	 

	CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
	if ( tpTeam == NULL )
	{
		LOG_ERROR( "gateteam", "object invalid, id=%d,[%s:%d]", tTeamID, __LK_FILE__, __LINE__ );
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_NOTEXIST );
		return ;
	}

	if ( tpTeam->GetCount() == TEAM_NUMBER_MAX )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_FULLTEAM );
		return ;
	}

	if ( tpTeam->IsHeader( tpDesPlayer->mCharID ) )
	{
		//SendErrorNotice( tpDesPlayer, ERROR_TEAM_JOINTEAM, true );
		// update team data at gate
		tpTeam->AddMember( tpSrcPlayer );
		InsertMember( tpSrcPlayer->mCharID, tTeamID );

		// notify scene to update 
		SendListNotifyToScene( tpTeam, tpSrcPlayer->mCharID );
		SendUpdateNotifyToScene( tpTeam, tpSrcPlayer->mCharID, ACTION_JOIN );
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_JOINTEAM );		
		// 
		DeleteClaimRecruit( tpSrcPlayer );
		if ( tpTeam->GetCount() == TEAM_NUMBER_MAX )
		{
			DeleteClaimRecruit( tpDesPlayer );
		}
		else
		{
			RefreshRecruitMsg( tpDesPlayer, tpTeam->GetCount() );
		}
		ClearValuePair< BaseReqInvMap >( mRequestList, tpSrcPlayer->mCharID );
	}
	// if inviter is not header, so that send a request to header 
	// contains such contents: the entity request join team by a member named *** invited
	else
	{
		CGatePlayer* tpHead = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpTeam->GetHeadID() );
		if ( tpHead == NULL )
		{
			LOG_ERROR( "gateteam", "why player not found,id=%d,[%s:%d]", tpTeam->GetHeadID(), __LK_FILE__, __LINE__ );
			return ;
		}
		CMessage tMessage;
		CMessageInvite2Request tPara;
		tPara.set_descharid( tpHead->mCharID );
		tPara.set_invitercharid( tpDesPlayer->mCharID );
		tPara.set_invitername( tpDesPlayer->mCharName );
		int tRtn = MakeTeamItemPB( tPara.mutable_srcitem(), tpSrcPlayer );
		if ( tRtn < 0 )
		{
			LOG_ERROR( "gateteam", "make team item pb failed.id=%d,[%s:%d]", tpSrcPlayer->mCharID, __LK_FILE__, __LINE__ );
			return ;
		}
		pbmsg_sethead(&tMessage, ID_G2S_RESPONSE_INVITEREQUEST, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara( (long) &tPara );
		CGateServer::GetSingleton().Send2Scene( tpHead, &tMessage );
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_SENDAPPLYTOHEADER );
		InsertValuePair< BaseReqInvMap >( mRequestList, tpSrcPlayer->mCharID, tTeamID );
		tpTeam->InsertReq( tpSrcPlayer->mCharID );
	}
	ClearValuePair< BaseReqInvMap >( mInviteList, tpSrcPlayer->mCharID );
	return ;
}

void CServiceGateTeam::OnMessageKickoutTeamRequest( CMessage* pMessage )
{
	CMessageKickoutGateTeamRequest* tpReq = ( CMessageKickoutGateTeamRequest* ) pMessage->msgpara();

	CGatePlayer* tpSrcPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->headcharid() );
	CGatePlayer* tpDesPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->membercharid() );
	if ( tpSrcPlayer == NULL || tpDesPlayer == NULL )
	{
		LOG_ERROR( "gateteam", "get player failed.srcid=%d,desid=%d.[%s:%d]", 
			tpReq->headcharid(), tpReq->membercharid(), __LK_FILE__, __LINE__ );
		return ;
	}

	int tTmpSrcTeamID = InTeam( tpSrcPlayer->mCharID );
	int tTmpDesTempID = InTeam( tpDesPlayer->mCharID );

	if ( tTmpDesTempID != tTmpSrcTeamID )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_NOTSAMETEAM );
		return ;
	}

	CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTmpSrcTeamID );
	if ( tpTeam == NULL )
	{
		LOG_ERROR( "gateteam", "object invalid,id=%d,[%s:%d]", tTmpSrcTeamID, __LK_FILE__, __LINE__ );
		return ;
	}

	if ( tpTeam->IsHeader( tpSrcPlayer->mCharID ) == false )
	{
		SendErrorNotice( tpSrcPlayer, ERROR_TEAM_NOTHEADER );
		return ;
	}

	if ( tpSrcPlayer->mCharID == tpDesPlayer->mCharID )
	{
		return ;
	}

	// notify scene to update
	SendUpdateNotifyToScene( tpTeam, tpDesPlayer->mCharID, ACTION_KICK );

	// update team data at gate
	tpTeam->DelMember( tpDesPlayer->mCharID );
	RemoveMember( tpDesPlayer->mCharID );
	//SendErrorNotice( tpSrcPlayer, ERROR_TEAM_KICK_SUCCESS, true );
	RefreshRecruitMsg( tpSrcPlayer, tpTeam->GetCount() );
	return ;
}

void CServiceGateTeam::OnMessageLeaveTeamRequest( CMessage* pMessage )
{
	CMessageLeaveGateTeamRequest* tpReq = ( CMessageLeaveGateTeamRequest* ) pMessage->msgpara();

	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	int tTeamID = InTeam( tpPlayer->mCharID );
	if ( tTeamID != INVALID_OBJ_ID )
	{
		CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
		if ( tpTeam == NULL )
		{
			LOG_ERROR( "gateteam", "object invalid,id=%d,[%s:%d]", tTeamID, __LK_FILE__, __LINE__ );
			return ;
		}

		if ( tpTeam->GetCount() == 1 )
		{
			// end team
			SendUpdateNotifyToScene( tpTeam, tpPlayer->mCharID, ACTION_ENDTEAM );
			RemoveTeam( tpTeam );
			DeleteClaimRecruit( tpPlayer );
		}
		else
		{
			if ( tpTeam->IsHeader( tpPlayer->mCharID ) == true )
			{
				// change team header
				int tRtn = ChangeHeaderByLeave( tpTeam, tpPlayer->mCharID );
				if ( tRtn < 0 )
				{
					LOG_ERROR( "gateteam", "team data error, change header failed.[%s:%d]", __LK_FILE__, __LINE__ );
					return ;
				}
				DeleteClaimRecruit( tpPlayer );
			}
			// notify scene to update
			//SendErrorNotice( tpPlayer, ERROR_TEAM_LEAVE_SUCCESS, true );
			SendUpdateNotifyToScene( tpTeam, tpPlayer->mCharID, ACTION_LEAVE );
			// update team data at gate
			tpTeam->DelMember( tpPlayer->mCharID );
			RemoveMember( tpPlayer->mCharID );
			//
			CGatePlayer* tpHead = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpTeam->GetHeadID() );
			if ( tpHead != NULL )
			{
				RefreshRecruitMsg( tpHead, tpTeam->GetCount() );
			}
		}		
	}

	return ;
}

void CServiceGateTeam::OnMessageEndTeamRequest( CMessage* pMessage )
{
	CMessageEndGateTeamRequest* tpReq = ( CMessageEndGateTeamRequest* ) pMessage->msgpara();

	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->charid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "gateteam", "why player not exist.id=%d,[%s:%d]", tpReq->charid(), __LK_FILE__, __LINE__ );
		return ;
	}

	int tTeamID = InTeam( tpPlayer->mCharID );
	if ( tTeamID != INVALID_OBJ_ID )
	{
		CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
		if ( tpTeam == NULL )
		{
			LOG_ERROR( "gateteam", "object invalid,id=%d,[%s:%d]", tTeamID, __LK_FILE__, __LINE__ );
			return ;
		}

		if ( tpTeam->IsHeader( tpPlayer->mCharID ) == false )
		{
			SendErrorNotice( tpPlayer, ERROR_TEAM_NOTHEADER );
			return ;
		}

		SendUpdateNotifyToScene( tpTeam, tpPlayer->mCharID, ACTION_ENDTEAM );
		RemoveTeam( tpTeam );
		DeleteClaimRecruit( tpPlayer );
	}
	else
	{
		SendErrorNotice( tpPlayer, ERROR_TEAM_NOTEXIST );
	}

	return ;
}

void CServiceGateTeam::OnMessageChangeHeaderRequest( CMessage* pMessage )
{
	CMessageChangeHeaderRequest* tpReq = ( CMessageChangeHeaderRequest* ) pMessage->msgpara();

	CGatePlayer* tpOldHead = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->oldheader() );
	CGatePlayer* tpNewHead = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->newheader() );	
	if ( tpOldHead == NULL || tpNewHead == NULL )
	{
		LOG_ERROR( "gateteam", "why player not exist.oldheaderid:%d,newheaderid:%d,[%s:%d]", tpReq->oldheader(), tpReq->newheader(), 
			__LK_FILE__, __LINE__ );
		return ;
	}

	int tTeamID1 = InTeam( tpOldHead->mCharID );
	int tTeamID2 = InTeam( tpNewHead->mCharID );
	if ( tTeamID1 != tTeamID2 )
	{
		SendErrorNotice( tpOldHead, ERROR_TEAM_NOTSAMETEAM );
		return ;
	}

	CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID1 );
	if ( tpTeam == NULL )
	{
		LOG_ERROR( "gateteam", "object invalid,id=%d,[%s:%d]", tTeamID1, __LK_FILE__, __LINE__ );
		return ;
	}

	if ( tpTeam->IsHeader( tpOldHead->mCharID ) == false )
	{
		SendErrorNotice( tpOldHead, ERROR_TEAM_NOTHEADER );
		return ;
	}

	SendUpdateNotifyToScene( tpTeam, tpNewHead->mCharID, ACTION_CHANGEHEADER );
	tpTeam->SetHeadID( tpNewHead->mCharID );
	tpTeam->SetHeadName( tpNewHead->mCharName );

	DeleteClaimRecruit( tpOldHead );
	return ;
}

void CServiceGateTeam::OnMessageChangeDistribute( CMessage* pMessage )
{
	CMessageChangeDistributeRequest* tpReq = ( CMessageChangeDistributeRequest* ) pMessage->msgpara();

	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( InTeam( tpPlayer->mCharID ) );
	if ( tpTeam == NULL )
	{
		return ;
	}

	if ( tpTeam->IsHeader( tpPlayer->mCharID ) == false )
	{
		SendErrorNotice( tpPlayer, ERROR_TEAM_NOTHEADER );
		return ;
	}

	tpTeam->SetMode( (TEAM_DISTRIBUTE) tpReq->mode() );

	CMessage tMessage;
	CMessageChangeDistributeResponse tPara;
	pbmsg_sethead( &tMessage, ID_G2S_RESPONSE_DISTRIBUTE, 0, 0, 0, 0, 0, 0 );
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_mode( (int) tpTeam->GetMode() ); 

	int tMemberList[ TEAM_NUMBER_MAX ] = { 0 };
	int tRtn = 0, tNum = 0;

	tRtn = tpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
	if ( tRtn < 0 )
	{
		return ;
	}

	int tErrorCode = 0;
	switch ( tpTeam->GetMode() )
	{
	case RANDOM:
		{
			tErrorCode = ERROR_TEAM_ASSIGN_RANDOM;
			break;
		}
	case ALTERNATION:
		{
			tErrorCode = ERROR_TEAM_ASSIGN_TURN;
			break;
		}
	case PROBABILITY:
		{
			tErrorCode = ERROR_TEAM_ASSIGN_PRO;
			break;
		}
	default:
		{
			tErrorCode = ERROR_TEAM_ASSIGN_RANDOM;
			break;
		}
	}

	for ( int i = 0; i < tNum && i < (int) ARRAY_CNT( tMemberList ); i ++ )
	{
		tPara.set_charid( tMemberList[ i ] );
		CGatePlayer* tpDes = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tMemberList[ i ] );
		if ( tpDes == NULL )
		{
			continue;
		}
		CGateServer::GetSingleton().Send2Scene( tpDes, &tMessage );
		SendErrorNotice( tpDes, tErrorCode );
	}
	return ;
}

void CServiceGateTeam::OnMessageSendClaimRecruitRequest( CMessage* pMessage )
{
	if ( pMessage == NULL ) return ;

	CMessageSendClaimRecruitRequest* tpReq = ( CMessageSendClaimRecruitRequest* ) pMessage->msgpara();

	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->charid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "gateteam", "why player not found,id=%d,[%s:%d]", tpReq->charid(), __LK_FILE__, __LINE__ );
		return ;
	}

	int tTeamID = InTeam( tpPlayer->mCharID );

	switch( tpReq->actiontype() )
	{
	case TYPE_CLAIM:
		{
			if ( tTeamID > 0 )
			{
				SendErrorNotice( tpPlayer, ERROR_TEAM_SRCINTEAM );
				return ;
			}

			ClaimList::iterator it = mClaimList.find( tpPlayer->mCharID );
			if ( it != mClaimList.end() )
			{
				mClaimList.erase( it );
			}
			tpReq->mutable_msg()->set_type( (int) TEAM_ASKFOR );
			STeamMsg tMsg;
			MakePbMsgToStruct( tpReq->mutable_msg(), &tMsg );
			mClaimList.insert( ClaimList::value_type( tpPlayer->mCharID, tMsg ) );
			break;
		}
	case TYPE_RECRUIT:
		{
			if ( tTeamID < 0 )
			{
				SendErrorNotice( tpPlayer, ERROR_TEAM_NOTEXIST );
				return ;
			}

			CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
			if ( tpTeam == NULL )
			{
				LOG_ERROR( "gateteam", "object invalid.id=%d,[%s:%d]", tTeamID, __LK_FILE__, __LINE__ );
				return ;
			}

			if ( tpTeam->GetCount() == TEAM_NUMBER_MAX )
			{
				SendErrorNotice( tpPlayer, ERROR_TEAM_FULLTEAM );
				return ;
			}

			RecruitList::iterator it = mRecruitList.find( tpPlayer->mCharID );
			if ( it != mRecruitList.end() )
			{
				mRecruitList.erase( it );
			}
			tpReq->mutable_msg()->set_type( (int) TEAM_CONSCRIBE );
			tpReq->mutable_msg()->set_count( tpTeam->GetCount() );
			STeamMsg tMsg;
			MakePbMsgToStruct( tpReq->mutable_msg(), &tMsg );
			mRecruitList.insert( RecruitList::value_type( tpPlayer->mCharID, tMsg ) );
			break;
		}
	default:
		break;
	}

	CMessage tMessage;
	CMessageSendClaimRecruitResponse tPara;
	pbmsg_sethead( &tMessage, ID_G2S_RESPONSE_CLAIMRECRUIT, 0, 0, 0, 0, 0, 0 );
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_charid( tpPlayer->mCharID );
	tPara.set_actiontype( tpReq->actiontype() );
	MakeMsgPB( tPara.mutable_msg(), tpReq->mutable_msg() );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
	return ;
}

void CServiceGateTeam::OnMessageQueryClaimRecruitRequest( CMessage* pMessage )
{
	if ( pMessage == NULL ) return ;

	CMessageQueryClaimRecruitRequest* tpReq = ( CMessageQueryClaimRecruitRequest* ) pMessage->msgpara();

	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->charid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "gateteam", "why player not found,id=%d,[%s:%d]", tpReq->charid(), __LK_FILE__, __LINE__ );
		return ;
	}	

	CMessage tMessage;
	CMessageQueryClaimRecruitResponse tPara;
	pbmsg_sethead( &tMessage, ID_G2S_RESPONSE_QUERYCLAIMRECRUIT, 0, 0, 0, 0, 0, 0 );
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_actiontype( tpReq->actiontype() );
	tPara.set_charid( tpPlayer->mCharID );

	int tMaxLevel = max( tpReq->maxlevel(), tpReq->minlevel() );
	int tMinLevel = min( tpReq->maxlevel(), tpReq->minlevel() );
	tMaxLevel = max( 0, tMaxLevel );
	tMinLevel = max( 0, tMinLevel );

	int tPageIndex = 0, tTotalIndex = 0;
	tPageIndex = tpReq->pageindex();

	switch( tpReq->actiontype() )
	{
	case TEAM_CLAIMINIT:
		{
			RecruitList::iterator it = mRecruitList.begin();
			int tIndex = 0, tCount = 0;
			tTotalIndex = GetTotalPageIndex( mRecruitList.size() );
			if ( tPageIndex >= tTotalIndex )
			{
				tPageIndex = tTotalIndex - 1;
			}
			for ( ; it != mRecruitList.end(); ++ it )
			{
				if ( tIndex ++ < ( tPageIndex * PAGE_MAX ) )
				{
					continue;
				}
				if ( tCount ++ >= PAGE_MAX )
				{
					break;
				}
				STeamMsg* tpMsg = (STeamMsg*) &(it->second);				
				if ( tpMsg == NULL ) continue;

				if ( tpMsg->mNationality != (int)tpReq->nationality() )
				{
					continue;
				}
				PBMsg* tpDesMsg = tPara.add_msg();
				MakeStructMsgToPB( tpMsg, tpDesMsg );
			}
			break;
		}
	case TEAM_ASKFOR:
		{
			/*if ( CheckCoolTime( tpPlayer->mCharID ) == true )
			{
			SendErrorNotice( tpPlayer, ERROR_TEAM_COOLINGTIME );
			return ;
			}*/
			RecruitList::iterator it = mRecruitList.begin();
			int tIndex = 0, tCount = 0;
			tTotalIndex = GetTotalPageIndex( mRecruitList.size() );
			if ( tPageIndex >= tTotalIndex )
			{
				tPageIndex = tTotalIndex - 1;
			}
			for ( ; it != mRecruitList.end(); ++ it )
			{
				if ( tIndex ++ < ( tPageIndex * PAGE_MAX ) )
				{
					continue;
				}
				if ( tCount ++ >= PAGE_MAX )
				{
					break;
				}
				STeamMsg* tpMsg = (STeamMsg*) &(it->second);
				if ( tpMsg == NULL ) continue;

				if ( tpMsg->mNationality != (int)tpReq->nationality() )
				{
					continue;
				}
				if ( tpMsg->mRootStyle != (int)tpReq->rootcondition() ||
					tpMsg->mSubStyle != (int)tpReq->subcondition() )
				{
					continue;
				}
				if ( tpMsg->mMetier != 0 && tpMsg->mMetier != (int)tpReq->metier() )
				{
					continue;
				}
				int tTempMaxLevel = max( tpMsg->mMaxLevel, tpMsg->mMinLevel );
				int tTempMinLevel = min( tpMsg->mMaxLevel, tpMsg->mMinLevel );
				tTempMaxLevel = max( 0, tTempMaxLevel );
				tTempMinLevel = max( 0, tTempMinLevel );
				if ( (int)tpReq->level() > tTempMaxLevel && tTempMaxLevel != 0 )
				{
					continue;
				}
				if ( (int)tpReq->level() < tTempMinLevel && tTempMinLevel != 0 )
				{
					continue;
				}

				PBMsg* tpDesMsg = tPara.add_msg();
				MakeStructMsgToPB( tpMsg, tpDesMsg );
			}
			break;
		}
	case TEAM_RECRUITINIT:
		{
			ClaimList::iterator it = mClaimList.begin();
			int tIndex = 0, tCount = 0;
			tTotalIndex = GetTotalPageIndex( mClaimList.size() );
			if ( tPageIndex >= tTotalIndex )
			{
				tPageIndex = tTotalIndex - 1;
			}
			for ( ; it != mClaimList.end(); ++ it )
			{
				if ( tIndex ++ < ( tPageIndex * PAGE_MAX ) )
				{
					continue;
				}
				if ( tCount ++ >= PAGE_MAX )
				{
					break;
				}
				STeamMsg* tpMsg = (STeamMsg*) &(it->second);				
				if ( tpMsg == NULL ) continue;

				if ( tpMsg->mNationality != (int)tpReq->nationality() )
				{
					continue;
				}

				PBMsg* tpDesMsg = tPara.add_msg();
				MakeStructMsgToPB( tpMsg, tpDesMsg );
			}
			break;
		}
	case TEAM_CONSCRIBE:
		{
			/*if ( CheckCoolTime( tpPlayer->mCharID ) == true )
			{
			SendErrorNotice( tpPlayer, ERROR_TEAM_COOLINGTIME );
			return ;
			}*/
			ClaimList::iterator it = mClaimList.begin();
			int tIndex = 0, tCount = 0;
			tTotalIndex = GetTotalPageIndex( mClaimList.size() );
			if ( tPageIndex >= tTotalIndex )
			{
				tPageIndex = tTotalIndex - 1;
			}
			for ( ; it != mClaimList.end(); ++ it )
			{
				if ( tIndex ++ < ( tPageIndex * PAGE_MAX ) )
				{
					continue;
				}
				if ( tCount ++ >= PAGE_MAX )
				{
					break;
				}
				STeamMsg* tpMsg = (STeamMsg*) &(it->second);				
				if ( tpMsg == NULL ) continue;

				if ( tpMsg->mNationality != (int)tpReq->nationality() )
				{
					continue;
				}
				if ( tpMsg->mRootStyle != (int)tpReq->rootcondition() ||
					tpMsg->mSubStyle != (int)tpReq->subcondition() )
				{
					continue;
				}
				if ( tpReq->metier() != 0 && (int)tpReq->metier() != tpMsg->mMetier )
				{
					continue;
				}
				if ( (int)tpMsg->mLevel > tMaxLevel && tMaxLevel != 0 )
				{
					continue;
				}
				if ( (int)tpMsg->mLevel < tMinLevel && tMinLevel != 0 )
				{
					continue;
				}

				PBMsg* tpDesMsg = tPara.add_msg();
				MakeStructMsgToPB( tpMsg, tpDesMsg );
			}
			break;
		}
	default:
		break;
	}

	tPara.set_pageindex( tPageIndex );
	tPara.set_totalindex( tTotalIndex );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
	return ;
}

void CServiceGateTeam::OnMesssageDeleteClaimRecruitRequest( CMessage* pMessage )
{
	if ( pMessage == NULL ) return ;

	CMessageDeleteClaimRecruitRequest* tpReq = ( CMessageDeleteClaimRecruitRequest* ) pMessage->msgpara();

	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->charid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "gateteam", "play not found,id=%d,[%s:%d]", tpReq->charid(), __LK_FILE__, __LINE__ );
		return ;
	}

	if ( tpReq->actiontype() == TEAM_ASKFOR )
	{
		ClaimList::iterator it = mClaimList.find( tpPlayer->mCharID );
		if ( it != mClaimList.end() )
		{
			mClaimList.erase( it );
		}
	}
	else if ( tpReq->actiontype() == TEAM_CONSCRIBE )
	{
		RecruitList::iterator it = mRecruitList.find( tpPlayer->mCharID );
		if ( it != mRecruitList.end() )
		{
			mRecruitList.erase( it );
		}
	}

	CMessage tMessage;
	CMessageDeleteClaimRecruitResponse tPara;
	pbmsg_sethead( &tMessage, ID_G2S_RESPONSE_DELETECLAIMRECRUIT, 0, 0, 0, 0, 0, 0 );
	tPara.set_charid( tpPlayer->mCharID );
	tPara.set_actiontype( tpReq->actiontype() );
	tMessage.set_msgpara( (long) &tPara );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
	return ;
}

void CServiceGateTeam::OnMessageFollowRequest( CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageFollowRequest* tpReq = ( CMessageFollowRequest* ) pMessage->msgpara();

	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	int tTeamID = InTeam( tpPlayer->mCharID );
	CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
	if ( tpTeam == NULL )
	{
		LOG_ERROR("gateteam", "team not exist.id=%d[%s:%d]", tTeamID, __LK_FILE__, __LINE__ );
		return ;
	}

	CMessage tMessage;
	CMessageFollowResponse tPara;
	tMessage.set_msgpara( (long) &tPara );
	pbmsg_sethead( &tMessage, ID_G2S_RESPONSE_FOLLOW, 0, 0, 0, 0, 0, 0 );

	if ( tpReq->actiontype() == FOLLOW_ORDER )
	{
		if ( tpTeam->IsHeader( tpPlayer->mCharID ) == false )
		{
			SendErrorNotice( tpPlayer, ERROR_TEAM_NOTHEADER );
			return ;
		}

		int tMemberList[ TEAM_NUMBER_MAX ] = { 0 };
		int tRtn = 0, tNum = 0;
		tRtn = tpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
		if ( tRtn < 0 )
		{
			return ;
		}

		tPara.set_headid( tpPlayer->mCharID );
		tPara.set_resultcode( tpReq->resultcode() );
		tPara.set_actiontype( tpReq->actiontype() );
		for ( int i = 0; i < tNum && i < (int) ARRAY_CNT( tMemberList ); i ++ )
		{
			tPara.set_charid( tMemberList[ i ] );
			CGatePlayer* tpMemberPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tMemberList[ i ] );
			if ( tpMemberPlayer != NULL )
			{
				CGateServer::GetSingleton().Send2Scene( tpMemberPlayer, &tMessage );
			}			
		}	
	}
	else if ( tpReq->actiontype() == FOLLOW_REQUEST )
	{
		if ( tpTeam->IsHeader( tpPlayer->mCharID ) == true )
		{
			return ;
		}

		CGatePlayer* tpHead = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpTeam->GetHeadID() );
		if ( tpHead == NULL )
		{
			return ;
		}

		tPara.set_charid( tpPlayer->mCharID );
		tPara.set_headid( tpHead->mCharID );
		tPara.set_resultcode( tpReq->resultcode() );
		tPara.set_actiontype( tpReq->actiontype() );
		CGateServer::GetSingleton().Send2Scene( tpHead, &tMessage );
	}

	return ;
}

void CServiceGateTeam::OnMessageUpdateGateTeam( CMessage* pMessage )
{
	if ( pMessage == NULL ) return ;

	CMessageUpdateTeamPropertyRequest* tpReq = ( CMessageUpdateTeamPropertyRequest* ) pMessage->msgpara();

	int tTeamID = InTeam( tpReq->charid() );
	CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
	if ( tpTeam == NULL )
	{
		return ;
	}

	tpTeam->RefreshMemberData( tpReq->mutable_info() );
	return ;
}

void CServiceGateTeam::OnMessageObtainGateTeamRequest( CMessage* pMessage )
{
	if ( pMessage == NULL ) return ;

	CMessageObtainGateTeamRequest* tpReq = ( CMessageObtainGateTeamRequest* ) pMessage->msgpara();

	int tTeamID = InTeam( tpReq->charid() );
	CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
	if ( tpTeam == NULL )
	{
		return ;
	}

	SendListNotifyToScene( tpTeam, tpReq->charid() );
	return ;
}

void CServiceGateTeam::OnMessageClearInvReqRequest(CMessage* pMessage)
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageClearInvReqRecordRequest* tpReq = ( CMessageClearInvReqRecordRequest* ) pMessage->msgpara();

	if ( tpReq->type() == Type_Inv )
	{
		ClearValuePair<BaseReqInvMap>( mInviteList, tpReq->charid() );
	}
	else
	{
		CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( InTeam( tpReq->charid() ) );
		if ( tpTeam != NULL )
		{
			tpTeam->ClearReq();
		}
	}
	return ;
}

void CServiceGateTeam::SendAllMessage( CGateTeam* vpTeam, CMessage* pMessage )
{
	if ( vpTeam == NULL || pMessage == NULL )
	{
		return ;
	}

	int tMemberList[ TEAM_NUMBER_MAX ] = { 0 };
	int tRtn = 0, tNum = 0;

	tRtn = vpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
	if ( tRtn < 0 )
	{
		return ;
	}

	for ( int i = 0; i < tNum; i ++ )
	{
		CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tMemberList[ i ] );
		if ( tpPlayer == NULL )
		{
			continue;
		}
		CGateServer::GetSingleton().Send2Player( tpPlayer, pMessage );
	}
	return ;
}

int CServiceGateTeam::MakeMsgPB( PBMsg* pDesMsg, PBMsg* pSrcMsg )
{
	if ( pDesMsg == NULL || pSrcMsg == NULL )
	{
		return -1;
	}

	pDesMsg->set_charid( pSrcMsg->charid() );
	pDesMsg->set_name( pSrcMsg->name() );
	pDesMsg->set_metier( pSrcMsg->metier() );
	pDesMsg->set_level( pSrcMsg->level() );
	pDesMsg->set_nationality( pSrcMsg->nationality() );
	pDesMsg->set_content( pSrcMsg->content() );
	pDesMsg->set_count( pSrcMsg->count() );
	pDesMsg->set_rootstyle( pSrcMsg->rootstyle() );
	pDesMsg->set_substyle( pSrcMsg->substyle() );
	pDesMsg->set_type( pSrcMsg->type() );
	pDesMsg->set_minlevel( pSrcMsg->minlevel() );
	pDesMsg->set_maxlevel( pSrcMsg->maxlevel() );

	return SUCCESS;
}

int CServiceGateTeam::MakeTeamMsgOnRedirect( PBMsg* pMsg, int vCharID )
{
	if ( pMsg == NULL )
	{
		return -1;
	}

	ClaimList::iterator itClaim = mClaimList.find( vCharID );
	if ( itClaim != mClaimList.end() )
	{
		MakeStructMsgToPB( (STeamMsg*)&(itClaim->second), pMsg );
		return SUCCESS;
	}

	RecruitList::iterator itRecruit = mRecruitList.find( vCharID );
	if ( itRecruit != mRecruitList.end() )
	{
		MakeStructMsgToPB( (STeamMsg*)&(itRecruit->second), pMsg );
		return SUCCESS;
	}

	return SUCCESS;
}

int CServiceGateTeam::MakeStructMsgToPB( STeamMsg* pSrcMsg, PBMsg* pDesMsg )
{
	if ( pSrcMsg == NULL || pDesMsg == NULL )
	{
		return -1;
	}
	pDesMsg->set_charid( pSrcMsg->mCharID );	
	pDesMsg->set_count( pSrcMsg->mCount );
	pDesMsg->set_level( pSrcMsg->mLevel );	
	pDesMsg->set_minlevel( pSrcMsg->mMinLevel );
	pDesMsg->set_maxlevel( pSrcMsg->mMaxLevel );
	pDesMsg->set_nationality( pSrcMsg->mNationality );
	pDesMsg->set_rootstyle( pSrcMsg->mRootStyle );
	pDesMsg->set_substyle( pSrcMsg->mSubStyle );
	pDesMsg->set_metier( pSrcMsg->mMetier );
	pDesMsg->set_type( pSrcMsg->mType );
	pDesMsg->set_name( pSrcMsg->mName );
	pDesMsg->set_content( pSrcMsg->mContent );
	return SUCCESS;
}
int CServiceGateTeam::MakePbMsgToStruct( PBMsg* pSrcMsg, STeamMsg* pDesMsg )
{
	if ( pSrcMsg == NULL || pDesMsg == NULL )
	{
		return -1;
	}
	pDesMsg->mCharID	= pSrcMsg->charid();
	pDesMsg->mCount		= pSrcMsg->count();
	pDesMsg->mLevel		= pSrcMsg->level();
	pDesMsg->mMinLevel	= pSrcMsg->minlevel();
	pDesMsg->mMaxLevel	= pSrcMsg->maxlevel();
	pDesMsg->mNationality = pSrcMsg->nationality();
	pDesMsg->mRootStyle	= pSrcMsg->rootstyle();
	pDesMsg->mSubStyle	= pSrcMsg->substyle();
	pDesMsg->mMetier	= pSrcMsg->metier();
	pDesMsg->mType		= pSrcMsg->type();
	memset( pDesMsg->mName, 0, sizeof( pDesMsg->mName ) );
	strncpy( pDesMsg->mName, pSrcMsg->name().c_str(), sizeof( pDesMsg->mName ) - 1 );
	memset( pDesMsg->mContent, 0, sizeof( pDesMsg->mContent ) );
	strncpy( pDesMsg->mContent, pSrcMsg->content().c_str(), sizeof( pDesMsg->mContent ) - 1 );
	return SUCCESS;
}