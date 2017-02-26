#include "stdafx.hpp"
#include "teammodule.h"
#include "mapmodule.h"
#include "propertymodule.h"
#include "modulemanager.h"
#include "sceneobj_manager.h"
#include "sceneobj_manager_imp.h"
#include "message_pb.hxx.pb.h"
#include "teammessage_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "entity.h"

// *******************************************************
//	Function Name : OnRouterMessage
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-4 
// *******************************************************

void CTeamModule::OnRouterMessage( CMessage* pMessage )
{
	//  TODO ...
	return ;
}

// *******************************************************
//	Function Name : OnClientMessage
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-4 
// *******************************************************

void CTeamModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{	
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	unsigned int nMessageID = pMessage->mutable_msghead()->messageid();

	switch( nMessageID )
	{
	case ID_C2S_REQUEST_CREATETEAM:					
		{			
			// create team by myself	
			OnMessageCreateTeamSelf( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_INVITE:						
		{				
			// invite to join team			
			OnMessageTeamInvite( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_INVITERTN:					
		{				
			// reply team invite					
			OnMessageInvitedProc( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_ASKFORJOIN:		
		{
			// request to join team						
			OnMessageRequestJoin( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_APPLYRTN:		
		{
			// reply team request
			OnMessageRecievedPorc( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_LEAVE:			
		{
			// leave team				
			OnMessageLeaveTeam( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_TICK:		
		{
			// kick out						
			OnMessageKickPlayer( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_NEWHEAD:		
		{
			// change team header						
			OnMessageAttornHeader( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_ENDTEAM:		
		{			
			// end team						
			OnMessageEndTeam( pPlayer, pMessage );
			break ;
		}		
	case ID_C2S_REQUEST_CHANGEDISTRIBUTE:		
		{
			// change team distribute mode
			OnMessageChangeDistribute( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_CLEARREQUESTLIST:		
		{
			// clear list of request
			OnMessageClearReqMap( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_CLEARINVITELIST:			
		{
			// clear list of invite
			OnMessageClearInviteMap( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_NOTIFY_ASKTEAMMSG:			
		{
			// claim msg			
			OnMessageClaimTeam( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_NOTIFY_CONSCRIBEMSG:			
		{
			// recruit msg			
			OnMessageRecruit( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_SEARCHMSG:				
		{			
			// search claim msg
			OnMessageSearchClaimMsg( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_SRHCONSCRIBEMSG:		
		{		
			// search recruit msg
			OnMessageSearchRecruitMsg( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_DELMSG:				
		{
			// delete claim or recruit msg
			OnMessageDeleteCRMsg( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_ORDERFOLLOW:			
		{
			// follow order
			OnMessageFollowOrder( pPlayer, pMessage );
			break ;
		}
	case ID_C2S_REQUEST_REQUESTFOLLOW:		
		{
			// follow request
			OnMessageRequestFollow( pPlayer, pMessage );
			break ;
		}
	default:
		break;
	}
	return ;
}

void CTeamModule::OnGateMessage(CMessage *pMessage)
{
	if ( pMessage == NULL )
	{
		return ;
	}

	unsigned int nMessageID = pMessage->mutable_msghead()->messageid();
	switch ( nMessageID )
	{
	case ID_G2S_RESPONSE_CREATETEAM:
		{
			OnGateCreateTeam( pMessage );
			break;
		}
	case ID_G2S_RESPONSE_JOINREQUEST:
		{
			OnGateRequestJoinTeam( pMessage );
			break;
		}
	case ID_G2S_RESPONSE_JOININVITE:
		{
			OnGateInviteJoinTeam( pMessage );
			break;
		}
	case ID_G2S_NOTIFY_UPDATETEAM:
		{
			OnGateUpdateTeam( pMessage );
			break;
		}
	case ID_G2S_NOTIFY_SENDLIST:
		{
			OnGateSendTeamList( pMessage );
			break;
		}
	case ID_G2S_NOTIFY_ERRORNOTICE:
		{
			OnGateSendErrNotice( pMessage );
			break;
		}
	case ID_G2S_RESPONSE_INVITEREQUEST:
		{
			OnGateInvite2Request( pMessage );
			break;
		}
	case ID_G2S_RESPONSE_UPDATETEAMPRO:
		{
			OnGateRefreshTeam( pMessage );
			break;
		}
	case ID_G2S_RESPONSE_CLAIMRECRUIT:
		{
			OnGateSendClaimRecruitResponse( pMessage );
			break;
		}
	case ID_G2S_RESPONSE_QUERYCLAIMRECRUIT:
		{
			OnGateQueryClaimRecruitResponse( pMessage );
			break;
		}
	case ID_G2S_RESPONSE_DELETECLAIMRECRUIT:
		{
			OnGateDeleteClaimRecruitResponse( pMessage );
			break;
		}
	case ID_G2S_RESPONSE_FOLLOW:
		{
			OnGateFollowResponse( pMessage );
			break;
		}
	case ID_G2S_RESPONSE_DISTRIBUTE:
		{
			OnGateChangeDistribute( pMessage );
			break;
		}
	default:
		{
			LOG_INFO( "gateteam", "gate message (id:%d) can't been processed[%s:%d]", nMessageID, __LK_FILE__, __LINE__ );
			break;
		}
	}
	return ;
}
//***********************************
// FunctionName : OnMessageCreateTeamSelf 
// Description  : 
// Input Params : 
// Output Params: 
// Return Types : 
// Modified Time: [6/25/2009]
//***********************************

void CTeamModule::OnMessageCreateTeamSelf( CEntityPlayer* pPlayer, CMessage* pMessage )
{	
	CMessage tMessage;
	CMessageCreateGateTeamRequest tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_CREATETEAMATGATE );
	tPara.set_charid( pPlayer->GetCharID() );
	tMessage.set_msgpara( (long) &tPara );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );

	return ;
}

//***********************************
// FunctionName : OnMessageTeamInvite 
// Description  : 
// Input Params : 
// Output Params: 
// Return Types : 
// Modified Time: [6/25/2009]
//***********************************

void  CTeamModule::OnMessageTeamInvite( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamInvite* pEvent = (CMessageTeamInvite*)pMessage->msgpara();

#ifdef _FB_
	CEntityPlayer* tpDesPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( pEvent->charid() );
	if( tpDesPlayer != NULL )
	{
		if( pPlayer->GetCampID() != tpDesPlayer->GetCampID() )
			return ;
	}
#endif	

	CMessage tMessage;
	CMessageJoinGateTeamInvite tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_JOININVITE );
	tPara.set_srccharid( pPlayer->GetCharID() );
	tPara.set_descharid( pEvent->charid() );
	tMessage.set_msgpara( (long) &tPara );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );

	return ;
}

//***********************************
// FunctionName : OnMessageInvitedProc 
// Description  : 
// Input Params : 
// Output Params: 
// Return Types : 
// Modified Time: [6/25/2009]
//***********************************

void  CTeamModule::OnMessageInvitedProc( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamInviteRtn* pEvent = ( CMessageTeamInviteRtn* )pMessage->msgpara();

	CMessage tMessage;
	CMessageInviteJoinReply tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_REPLYJOININVITE );
	tPara.set_srccharid( pPlayer->GetCharID() );
	tPara.set_descharid( pEvent->inviteid() );
	tPara.set_teamid( pEvent->teamid() );
	tPara.set_rtncode( pEvent->rtncode() );
	tMessage.set_msgpara( (long) &tPara );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );

	return ;
}

//***********************************
// FunctionName : OnMessageRequestJoin 
// Description  : 
// Input Params : 
// Output Params: 
// Return Types : 
// Modified Time: [6/25/2009]
//***********************************

void  CTeamModule::OnMessageRequestJoin( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamRequest* pEvent = ( CMessageTeamRequest* )pMessage->msgpara();

#ifdef _FB_
	CEntityPlayer* tpDesPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( pEvent->charid() );
	if( tpDesPlayer != NULL )
	{
		if( pPlayer->GetCampID() != tpDesPlayer->GetCampID() )
			return ;
	}
#endif

	CMessage tMessage;
	CMessageJoinGateTeamRequest tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_JOINREQUEST );
	tPara.set_srccharid( pPlayer->GetCharID() );
	tPara.set_descharid( pEvent->charid() );
	tMessage.set_msgpara( (long) &tPara );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );

	return; 
}

//***********************************
// FunctionName : OnMessageRecievedPorc 
// Description  : 
// Input Params : 
// Output Params: 
// Return Types : 
// Modified Time: [6/25/2009]
//***********************************

void  CTeamModule::OnMessageRecievedPorc( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamRequestRtn* pEvent = ( CMessageTeamRequestRtn* )pMessage->msgpara();

	CMessage tMessage;
	CMessageRequestJoinReply tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_REPLYJOINREQUEST );
	tPara.set_srccharid( pPlayer->GetCharID() );
	tPara.set_descharid( pEvent->charid() );
	tPara.set_rtncode( pEvent->code() );
	tMessage.set_msgpara( (long) &tPara );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );

	return ;
}

//***********************************
// FunctionName : OnMessageLeaveTeam 
// Description  : 
// Input Params : 
// Output Params: 
// Return Types : 
// Modified Time: [6/25/2009]
//***********************************

void  CTeamModule::OnMessageLeaveTeam( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessage tMessage;
	CMessageLeaveGateTeamRequest tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_LEAVETEAM );
	tPara.set_charid( pPlayer->GetCharID() );
	tMessage.set_msgpara( (long) &tPara );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );	

	// Çå³ýÏÂÍæ¼ÒµÄÄÚµ¤×´Ì¬
	CPropertyModule::GetSingleton().ChangeTeamLeaverStatus( pPlayer );

	return; 	
}

//***********************************
// FunctionName : OnMessageKickPlayer 
// Description  : 
// Input Params : 
// Output Params: 
// Return Types : 
// Modified Time: [6/25/2009]
//***********************************

void  CTeamModule::OnMessageKickPlayer( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamTick* pEvent = ( CMessageTeamTick* )pMessage->msgpara();

	CMessage tMessage;
	CMessageKickoutGateTeamRequest tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_KICKOUTTEAM );
	tPara.set_headcharid( pPlayer->GetCharID() );
	tPara.set_membercharid( pEvent->charid() );
	tMessage.set_msgpara( (long) &tPara );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );

	return ;
}

//***********************************
// FunctionName : OnMessageEndTeam 
// Description  : 
// Input Params : 
// Output Params: 
// Return Types : 
// Modified Time: [6/25/2009]
//***********************************

void  CTeamModule::OnMessageEndTeam( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessage tMessage;
	CMessageEndGateTeamRequest tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_ENDTEAM );
	tPara.set_charid( pPlayer->GetCharID() );
	tMessage.set_msgpara( (long) &tPara );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );

	return ;
}

//***********************************
// FunctionName : OnMessageAttornHeader 
// Description  : 
// Input Params : 
// Output Params: 
// Return Types : 
// Modified Time: [6/25/2009]
//***********************************

void  CTeamModule::OnMessageAttornHeader( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamNewHead* pEvent = ( CMessageTeamNewHead* )pMessage->msgpara();

	CMessage tMessage;
	CMessageChangeHeaderRequest tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_CHANGEHEADER );
	tPara.set_oldheader( pPlayer->GetCharID() );
	tPara.set_newheader( pEvent->charid() );
	tMessage.set_msgpara( (long) &tPara );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );

	return ;
}

//***********************************
// FunctionName : OnMessageChangeDistribute 
// Description  : 
// Input Params : 
// Output Params: 
// Return Types : 
// Modified Time: [6/25/2009]
//***********************************

void  CTeamModule::OnMessageChangeDistribute( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamChangeDistribute* tpRequest = ( CMessageTeamChangeDistribute* ) pMessage->msgpara();

	CMessage tMessage;
	CMessageChangeDistributeRequest tPara;
	MakeMessage<CMessageChangeDistributeRequest>( ID_S2G_REQUEST_DISTRIBUTE, tPara, &tMessage );
	tPara.set_charid( pPlayer->GetCharID() );
	tPara.set_mode( tpRequest->distribute() );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
	return ;
}

//***********************************
// FunctionName : OnMessageRecruit 
// Description  : 
// Input Params : 
// Output Params: 
// Return Types : 
// Modified Time: [6/25/2009]
//***********************************

void  CTeamModule::OnMessageRecruit( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamConscribeMsg* pEvent = ( CMessageTeamConscribeMsg* )pMessage->msgpara();

	CMessage tMessage;
	CMessageSendClaimRecruitRequest tPara;
	MakeMessage<CMessageSendClaimRecruitRequest>( ID_S2G_REQUEST_CLAIMRECRUIT, tPara, &tMessage );

	tPara.set_charid( pPlayer->GetCharID() );
	tPara.set_actiontype( (int) TYPE_RECRUIT );

	// ¹ýÂËÔà´Ê
	char tBuffer[ NAME_LENGTH * 2 ] = { 0 };
	strncpy( tBuffer, pEvent->msg().c_str(), sizeof( tBuffer ) - 1 );
	CDirtyWord::GetSingleton().OI_Dirty_Check( 1, tBuffer );

	PBMsg* tpMsg = tPara.mutable_msg();
	tpMsg->set_charid( pPlayer->GetCharID() );
	tpMsg->set_rootstyle( pEvent->conscribetype() );
	tpMsg->set_substyle( pEvent->subtype() );
	tpMsg->set_name( pPlayer->GetName() );
	tpMsg->set_level( pPlayer->GetLevel() );
	tpMsg->set_metier( pEvent->metier() );
	tpMsg->set_minlevel( pEvent->minlevel() );
	tpMsg->set_maxlevel( pEvent->maxlevel() );
	tpMsg->set_content( tBuffer );
	tpMsg->set_nationality( pPlayer->GetNationality() );

	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
	return ;
}

//***********************************
// FunctionName : OnMessageClaimTeam 
// Description  : 
// Input Params : 
// Output Params: 
// Return Types : 
// Modified Time: [6/25/2009]
//***********************************

void  CTeamModule::OnMessageClaimTeam( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamAskTeamMsg* pEvent = ( CMessageTeamAskTeamMsg* )pMessage->msgpara();

	CMessage tMessage;
	CMessageSendClaimRecruitRequest tPara;
	MakeMessage<CMessageSendClaimRecruitRequest>( ID_S2G_REQUEST_CLAIMRECRUIT, tPara, &tMessage );
	tPara.set_charid( pPlayer->GetCharID() );
	tPara.set_actiontype( (int) TYPE_CLAIM );

	// ¹ýÂËÔà´Ê
	char tBuffer[ NAME_LENGTH * 2 ] = { 0 };
	strncpy( tBuffer, pEvent->msg().c_str(), sizeof( tBuffer ) - 1 );
	CDirtyWord::GetSingleton().OI_Dirty_Check( 1, tBuffer );

	PBMsg* tpMsg = tPara.mutable_msg();
	tpMsg->set_charid( pPlayer->GetCharID() );
	tpMsg->set_metier( pPlayer->GetMetier() );
	tpMsg->set_level( pPlayer->GetLevel() );
	tpMsg->set_name( pPlayer->GetName() );
	tpMsg->set_nationality( pPlayer->GetNationality() );
	tpMsg->set_content( tBuffer );
	tpMsg->set_rootstyle( pEvent->conscribetype() );
	tpMsg->set_substyle( pEvent->subtype() );

	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
	return ;
}

// *******************************************************
//	Function Name : OnMessageClearReqMap
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-10 
// *******************************************************

void  CTeamModule::OnMessageClearReqMap( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageClearInvReqRecordRequest tPara;
	MakeMessage<CMessageClearInvReqRecordRequest>(ID_S2G_REQUEST_CLEARRECORD, tPara, &tMessage);
	tPara.set_type( Type_Req );
	tPara.set_charid( pPlayer->GetCharID() );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
	return ;
}

// *******************************************************
//	Function Name : OnMessageClearInviteMap
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-10 
// *******************************************************

void  CTeamModule::OnMessageClearInviteMap( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageClearInvReqRecordRequest tPara;
	MakeMessage<CMessageClearInvReqRecordRequest>(ID_S2G_REQUEST_CLEARRECORD, tPara, &tMessage);
	tPara.set_type( Type_Inv );
	tPara.set_charid( pPlayer->GetCharID() );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
	return ;
}

// *******************************************************
//	Function Name : OnMessageSearchClaimMsg
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-9 
// *******************************************************

void CTeamModule::OnMessageSearchClaimMsg( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamSearchMsg* pEvent = ( CMessageTeamSearchMsg* )pMessage->msgpara();

	CMessage tMessage;
	CMessageQueryClaimRecruitRequest tPara;
	MakeMessage<CMessageQueryClaimRecruitRequest>( ID_S2G_REQUEST_QUERYCLAIMRECRUIT, tPara, &tMessage );

	tPara.set_charid( pPlayer->GetCharID() );
	tPara.set_actiontype( pEvent->msgtype() );
	tPara.set_level( pPlayer->GetLevel() );
	tPara.set_nationality( pPlayer->GetNationality() );
	tPara.set_metier( pPlayer->GetMetier() );
	tPara.set_rootcondition( pEvent->conscibetype() );
	tPara.set_subcondition( pEvent->subtype() );
	tPara.set_pageindex( pEvent->pageindex() );

	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
	return ;
}

// *******************************************************
//	Function Name : OnMessageSearchRecruitMsg
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-9 
// *******************************************************

void CTeamModule::OnMessageSearchRecruitMsg( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamSearchConscribeMsg* pEvent = ( CMessageTeamSearchConscribeMsg* )pMessage->msgpara();

	CMessage tMessage;
	CMessageQueryClaimRecruitRequest tPara;
	MakeMessage<CMessageQueryClaimRecruitRequest>( ID_S2G_REQUEST_QUERYCLAIMRECRUIT, tPara, &tMessage );

	tPara.set_charid( pPlayer->GetCharID() );
	tPara.set_actiontype( pEvent->msgtype() );
	tPara.set_level( pPlayer->GetLevel() );
	tPara.set_maxlevel( pEvent->maxlevel() );
	tPara.set_minlevel( pEvent->minlevel() );
	tPara.set_nationality( pPlayer->GetNationality() );
	tPara.set_metier( pEvent->metier() );
	tPara.set_rootcondition( pEvent->conscribetype() );
	tPara.set_subcondition( pEvent->subtype() );
	tPara.set_pageindex( pEvent->pageindex() );

	CSceneLogic::GetSingleton().Send2Gate( &tMessage );

	return ;
}

// *******************************************************
//	Function Name : OnMessageDeleteCRMsg
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-11 
// *******************************************************

void CTeamModule::OnMessageDeleteCRMsg( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamDeleteMsg* pEvent = ( CMessageTeamDeleteMsg* )pMessage->msgpara();

	CMessage tMessage;
	CMessageDeleteClaimRecruitRequest tPara;
	MakeMessage<CMessageDeleteClaimRecruitRequest>( ID_S2G_REQUEST_DELETECLAIMRECRUIT, tPara, &tMessage );

	tPara.set_charid( pPlayer->GetCharID() );
	tPara.set_actiontype( pEvent->msgtype() );

	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
	return ;
}

// *******************************************************
//	Function Name : OnMessageRequestFollow
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-9 
// *******************************************************

void CTeamModule::OnMessageRequestFollow( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamRequestFollow* pEvent = ( CMessageTeamRequestFollow* )pMessage->msgpara();

	CMessage tMessage;
	CMessageFollowRequest tPara;
	MakeMessage<CMessageFollowRequest>( ID_S2G_REQUEST_FOLLOW, tPara, &tMessage );

	tPara.set_charid( pPlayer->GetCharID() );
	tPara.set_actiontype( (int) FOLLOW_REQUEST );
	tPara.set_resultcode( pEvent->code() );

	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
	return ;
}

// *******************************************************
//	Function Name : OnMessageFollowOrder
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-9 
// *******************************************************

void CTeamModule::OnMessageFollowOrder( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTeamOrderFollow* pEvent = ( CMessageTeamOrderFollow* )pMessage->msgpara();

	CMessage tMessage;
	CMessageFollowRequest tPara;
	MakeMessage<CMessageFollowRequest>( ID_S2G_REQUEST_FOLLOW, tPara, &tMessage );

	tPara.set_charid( pPlayer->GetCharID() );
	tPara.set_actiontype( (int) FOLLOW_ORDER );
	tPara.set_resultcode( pEvent->code() );

	CSceneLogic::GetSingleton().Send2Gate( &tMessage );

	return ;
}

void CTeamModule::OnGateFollowResponse( CMessage* pMessage )
{
	CMessageFollowResponse* tpResponse = ( CMessageFollowResponse* ) pMessage->msgpara();

	CEntityPlayer* tpHead = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpResponse->headid() );
	if( tpHead == NULL )
	{
		return ;
	}
	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpResponse->charid() );
	if( tpPlayer == NULL )
	{
		return ;
	}

	if( tpHead->IsWithinDistance( tpPlayer, TEAM_VALID_DISTANCE ) == false )
	{
		return ;
	}

	if( tpResponse->actiontype() == FOLLOW_ORDER )
	{
		CMessage tMessage;
		CMessageTeamOrderFollowCallBack tPara;
		MakeMessage<CMessageTeamOrderFollowCallBack>( ID_S2C_NOTIFY_ORDERFOLLOWRTN, tPara, &tMessage );
		tPara.set_headername( tpHead->GetName() );
		tPara.set_charid( tpHead->GetCharID() );
		tPara.set_code( tpResponse->resultcode() );
		CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
	}
	else if( tpResponse->actiontype() == FOLLOW_REQUEST )
	{
		CMessage tMessage;
		CMessageTeamRequestFollowCallBack tPara;
		MakeMessage<CMessageTeamRequestFollowCallBack>( ID_S2C_NOTIFY_REQUESTFOLLOWRTN, tPara, &tMessage );
		tPara.set_name( tpPlayer->GetName() );
		tPara.set_charid( tpPlayer->GetCharID() );
		CSceneLogic::GetSingleton().Send2Player( tpHead, &tMessage );
	}
	return ;
}

void CTeamModule::OnGateCreateTeam(CMessage *pMesage)
{
	if ( pMesage == NULL ) return ;

	CMessageCreateGateTeamResponse* tpResponse = ( CMessageCreateGateTeamResponse* ) pMesage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpResponse->charid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "team", "why player not found,id=%d,[%s:%d]", tpResponse->charid(), __LK_FILE__, __LINE__ );
		return ;
	}

	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_TEAM );
	if ( tpTeam == NULL )
	{
		LOG_ERROR( "team", "create object failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return ;
	}

	tpTeam->AddMember( tpPlayer->GetCharID() );
	InsertMember( tpPlayer->GetCharID(), tpTeam->get_id() );
	tpTeam->SetHeaderID( tpPlayer->GetCharID() );
	tpTeam->SetTeamID( tpResponse->teamid() );
	tpTeam->SetMode( (int) RANDOM );

	CMessage tMessage;
	CMessageTeamCallBack tPara;
	tPara.set_headetyid( tpPlayer->GetCharID() );
	tPara.set_headername( tpPlayer->GetName() );
	tPara.set_distribute( (int) RANDOM );
	tPara.set_count( 1 );
	PBMember* tpMember = tPara.add_member();
	MakeMemberPB( tpMember, tpPlayer );
	MakeMessage<CMessageTeamCallBack>(ID_S2C_NOTIFY_ITEMLIST, tPara, &tMessage);
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
	NotifyTeamFlag( tpPlayer, tpPlayer->GetCharID() );
	return ;
}

void CTeamModule::OnGateInviteJoinTeam( CMessage* pMessage )
{
	if ( pMessage == NULL ) return ;

	CMessageJoinTeamInviteResponse* tpResponse = ( CMessageJoinTeamInviteResponse* ) pMessage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpResponse->descharid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageTeamInviteAskCallBack tPara;
	tPara.set_charid( tpResponse->srccharid() );
	tPara.set_name( tpResponse->srcname() );
	tPara.set_headname( tpResponse->headname() );

	PBTeamInfo* tpInfo = tpResponse->mutable_teaminfo();
	for ( int i = 0; i < tpInfo->members_size(); i ++ )
	{
		PBMember* tpMember = tPara.add_member();
		MakeMemberPB( tpMember, tpInfo->mutable_members( i ) );
	}

	tPara.set_count( tpInfo->count() );
	tPara.set_distribute( tpInfo->distributemode() );
	tPara.set_teamid( tpResponse->teamid() );

	MakeMessage<CMessageTeamInviteAskCallBack>(ID_S2C_NOTIFY_ASKJOINRTN, tPara, &tMessage);
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
	return ;
}

void CTeamModule::OnGateRequestJoinTeam( CMessage* pMessage )
{
	if ( pMessage == NULL ) return ;

	CMessageJoinGateTeamResponse* tpResponse = ( CMessageJoinGateTeamResponse* ) pMessage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpResponse->descharid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageTeamRequestPlayerCallBack tPara;
	PBTeamItem* tpItem = tpResponse->mutable_item();
	tPara.set_charid( tpItem->charid() );
	tPara.set_name( tpItem->name().c_str() );
	tPara.set_face( tpItem->face() );
	tPara.set_sex( tpItem->sex() );
	tPara.set_level( tpItem->level() );
	tPara.set_metier( tpItem->metier() );
	tPara.set_nationality( tpItem->nationality() );
	MakeMessage<CMessageTeamRequestPlayerCallBack>( ID_S2C_REQUEST_PROPOSERRTN, tPara, &tMessage );
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
	return ;
}

void CTeamModule::OnGateInvite2Request( CMessage* pMessage )
{
	if ( pMessage == NULL ) return ;

	CMessageInvite2Request* tpReq = ( CMessageInvite2Request* ) pMessage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpReq->descharid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageTeamInvitedRequestCallBack tPara;
	tPara.set_inviteid( tpReq->invitercharid() );
	tPara.set_invitename( tpReq->invitername() );
	tPara.set_inviteeid( tpReq->mutable_srcitem()->charid() );
	tPara.set_inviteename( tpReq->mutable_srcitem()->name().c_str() );
	tPara.set_level( tpReq->mutable_srcitem()->level() );
	tPara.set_face( tpReq->mutable_srcitem()->face() );
	tPara.set_sex( tpReq->mutable_srcitem()->sex() );
	tPara.set_metier( tpReq->mutable_srcitem()->metier() );
	MakeMessage<CMessageTeamInvitedRequestCallBack>(ID_C2S_REQUEST_INVITEDREQUEST, tPara, &tMessage);
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
	return ;
}

void CTeamModule::OnGateSendTeamList( CMessage* pMessage )
{
	if( pMessage == NULL ) return ;

	CMessageNotifySceneToSendTeamList* tpNotify = ( CMessageNotifySceneToSendTeamList* ) pMessage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpNotify->descharid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	// if player has not team in this scene, force create a team automatically
	CreateTeamOnRedirect( tpPlayer, tpNotify->mutable_teaminfo() );

	return ;
}

void CTeamModule::OnGateUpdateTeam( CMessage* pMessage )
{
	if( pMessage == NULL ) return ;

	CMessageNotifySceneToUpdateTeam* tpNotify = ( CMessageNotifySceneToUpdateTeam* ) pMessage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpNotify->descharid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}	

	int tCharID = tpNotify->mutable_item()->charid();

	CMessage tMessage;
	CMessageTeamNotifyCallBack tPara;
	switch ( tpNotify->code() )
	{
	case ACTION_JOIN:
		{
			tPara.set_code( TEAM_ADD );
			if ( (int)tpPlayer->GetCharID() == tCharID )
			{
				NotifyTeamFlag( tpPlayer, tpNotify->headid() );
			}
			break;
		}
	case ACTION_LEAVE:
		{
			tPara.set_code( TEAM_DEL );
			if ( (int)tpPlayer->GetCharID() == tCharID )
			{
				NotifyTeamFlag( tpPlayer, TEAM_NONE );
			}
			break;
		}
	case ACTION_KICK:
		{
			tPara.set_code( TEAM_DEL );
			if ( (int)tpPlayer->GetCharID() == tCharID )
			{
				CMessage tMsg;
				CMessageTeamTickCallBack tKickMsg;
				MakeMessage<CMessageTeamTickCallBack>(ID_S2C_NOTIFY_TICKRTN, tKickMsg, &tMsg);
				tKickMsg.set_charid( tCharID );
				CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMsg );
				NotifyTeamFlag( tpPlayer, TEAM_NONE );
			}			
			break;
		}
	case ACTION_CHANGEHEADER:
		{
			tPara.set_code( TEAM_HEAD );
			if ( tpPlayer->GetCharID() == tpNotify->headid() )
			{
				NotifyTeamFlag( tpPlayer, tCharID );
				LOG_INFO( "team", ">>>Team Action: member ( %d ) become stuff .", tpPlayer->GetCharID() );
			}
			else if ( (int)tpPlayer->GetCharID() == tCharID )
			{
				NotifyTeamFlag( tpPlayer, tpPlayer->GetCharID() );
				LOG_INFO( "team", ">>>Team Action: member ( %d ) become new header .", tCharID );
			}
			break;
		}
	default:
		break;
	}
	MakeMemberPB( tPara.mutable_member(), tpNotify->mutable_item() );
	MakeMessage<CMessageTeamNotifyCallBack>( ID_S2C_NOTIFY_SYNITEMLIST, tPara, &tMessage );
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );	

	if ( tpNotify->code() == ACTION_ENDTEAM )
	{
		NotifyEndTeam( tpPlayer );	
		CPropertyModule::GetSingleton().ChangeTeamLeaverStatus( tpPlayer );
	}

	int tTeamID = InTeam( tpPlayer->GetCharID() );
	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( tTeamID );
	if ( tpTeam == NULL )
	{
		return ;
	}

	if ( tpNotify->code() == ACTION_ENDTEAM )
	{
		ProcessTripDisplay( tpTeam, tpPlayer, TEAM_END_TRIP );
		RemoveTeam( tpTeam );
		return ;
	}

	// process team data
	switch ( tpNotify->code() )
	{
	case ACTION_JOIN:
		{
			tpTeam->AddMember( tCharID );
			InsertMember( tCharID, tTeamID );
			if ( (int)tpPlayer->GetCharID() == tCharID )
			{
				ProcessTripDisplay( tpTeam, tpPlayer, TEAM_ADD_TRIP );
			}			
			break;
		}
	case ACTION_KICK:
	case ACTION_LEAVE:
		{
			if ( (int)tpPlayer->GetCharID() == tCharID )
			{
				ProcessTripDisplay( tpTeam, tpPlayer, TEAM_LEAVE_TRIP );
				CPropertyModule::GetSingleton().ChangeTeamLeaverStatus( tpPlayer );
			}	
			tpTeam->DelMember( tCharID );
			RemoveMember( tCharID );
			break;
		}
	case ACTION_CHANGEHEADER:
		{
			tpTeam->SetHeaderID( tCharID );
			break;
		}
	default:
		break;
	}

	return ;
}

void CTeamModule::OnGateSendClaimRecruitResponse( CMessage* pMessage )
{
	if ( pMessage == NULL ) return ;

	CMessageSendClaimRecruitResponse* tpResponse = ( CMessageSendClaimRecruitResponse* ) pMessage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpResponse->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	switch( tpResponse->actiontype() )
	{
	case TYPE_CLAIM:
		{
			CMessage tMessage;
			CMessageTeamAskTeamCallBack tPara;
			MakeMessage<CMessageTeamAskTeamCallBack>( ID_S2C_NOTIFY_ASKTEAMTRN, tPara, &tMessage );
			PBMsg* tpMsg = tpResponse->mutable_msg();
			tPara.set_conscribetype( tpMsg->rootstyle() );
			tPara.set_subtype( tpMsg->substyle() );
			tPara.set_metier( tpMsg->metier() );
			tPara.set_name( tpMsg->name() );
			tPara.set_level( tpMsg->level() );
			tPara.set_msg( tpMsg->content() );
			CMapModule::GetSingleton().MessageBroadCast( tpPlayer, &tMessage, false, true );
			ClaimList::iterator it = mClaimList.find( tpPlayer->GetCharID() );
			if ( it != mClaimList.end() )
			{
				mClaimList.erase( it );
			}
			STeamMsg tMsg;
			MakePbMsgToStruct( tpResponse->mutable_msg(), (STeamMsg*)&tMsg );
			mClaimList.insert( ClaimList::value_type( tpPlayer->GetCharID(), tMsg ) );
			break;
		}
	case TYPE_RECRUIT:
		{
			CMessage tMessage;
			CMessageTeamConscribeCallBack tPara;
			MakeMessage<CMessageTeamConscribeCallBack>( ID_S2C_NOTIFY_CONSCRIBERTN, tPara, &tMessage );
			PBMsg* tpMsg = tpResponse->mutable_msg();
			tPara.set_conscribetype( tpMsg->rootstyle() );
			tPara.set_subtype( tpMsg->substyle() );
			tPara.set_metier( tpMsg->metier() );
			tPara.set_msg( tpMsg->content() );
			tPara.set_count( tpMsg->count() );
			tPara.set_maxlevel( tpMsg->maxlevel() );
			tPara.set_minlevel( tpMsg->minlevel() );
			tPara.set_headername( tpMsg->name() );
			CMapModule::GetSingleton().MessageBroadCast( tpPlayer, &tMessage, false, true );
			RecruitList::iterator it = mRecruitList.find( tpPlayer->GetCharID() );
			if ( it != mRecruitList.end() )
			{
				mRecruitList.erase( it );
			}
			STeamMsg tMsg;
			MakePbMsgToStruct( tpResponse->mutable_msg(), (STeamMsg*)&tMsg );
			mRecruitList.insert( RecruitList::value_type( tpPlayer->GetCharID(), tMsg ) );
			break;
		}
	default:
		break;
	}
	return ;
}

void CTeamModule::OnGateQueryClaimRecruitResponse( CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageQueryClaimRecruitResponse* tpResponse = ( CMessageQueryClaimRecruitResponse* ) pMessage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpResponse->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	switch( tpResponse->actiontype() )
	{
	case TEAM_CLAIMINIT:
	case TEAM_ASKFOR:
		{
			CMessage tMessage;
			CMessageTeamSearchConscribeCallBack tPara;
			MakeMessage<CMessageTeamSearchConscribeCallBack>( ID_S2C_NOTIFY_SRHCONSCRIBECALLBACK, tPara, &tMessage );
			tPara.set_msgtype( tpResponse->actiontype() );
			tPara.set_msgcount( tpResponse->msg_size() );
			tPara.set_pageindex( tpResponse->pageindex() );
			tPara.set_totalpage( tpResponse->totalindex() );
			tPara.set_msgsize( tpResponse->msg_size() );
			for ( int i = 0; i < tpResponse->msg_size(); i ++ )
			{
				PBTeamMsg* tpMsg = tPara.add_msg();
				MakeMsgPB( tpMsg, tpResponse->mutable_msg( i ) );
			}
			CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
			break;
		}
	case TEAM_RECRUITINIT:
	case TEAM_CONSCRIBE:
		{
			CMessage tMessage;
			CMessageTeamSearchMsgCallBack tPara;
			MakeMessage<CMessageTeamSearchMsgCallBack>( ID_S2C_NOTIFY_SEARCHMSGCALLBACK, tPara, &tMessage );
			tPara.set_msgtype( tpResponse->actiontype() );
			tPara.set_msgsize( tpResponse->msg_size() );
			tPara.set_pageindex( tpResponse->pageindex() );
			tPara.set_totalpage( tpResponse->totalindex() );
			for ( int i = 0; i < tpResponse->msg_size(); i ++ )
			{
				PBTeamMsg* tpMsg = tPara.add_msg();
				MakeMsgPB( tpMsg, tpResponse->mutable_msg( i ) );
			}
			CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
			break;
		}
	default:
		break;
	}
	return ;
}

void CTeamModule::OnGateRefreshTeam( CMessage* pMessage )
{
	if ( pMessage == NULL ) return ;

	CMessageUpdateTeamPropertyResponse* tpResponse = ( CMessageUpdateTeamPropertyResponse* ) pMessage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpResponse->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageTeamPropertyNotifyCallBack tPara;
	MakeMessage<CMessageTeamPropertyNotifyCallBack>(ID_S2C_NOTIFY_SYNPROPERTY, tPara, &tMessage);

	PBTeamInfo* tpInfo = tpResponse->mutable_info();
	for ( int i = 0; i < tpInfo->members_size(); i ++ )
	{
		PBMember* tpMember = tPara.add_itempro();
		MakeMemberPB( tpMember, tpInfo->mutable_members( i ) );
	}

	tPara.set_itemsize( tpInfo->members_size() );
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
	return ;
}

void CTeamModule::OnGateDeleteClaimRecruitResponse( CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageDeleteClaimRecruitResponse* tpResponse = ( CMessageDeleteClaimRecruitResponse* ) pMessage->msgpara();

	if ( tpResponse->actiontype() == TEAM_ASKFOR )
	{
		ClaimList::iterator it = mClaimList.find( tpResponse->charid() );
		if ( it != mClaimList.end() )
		{
			mClaimList.erase( it );
		}
	}
	else if( tpResponse->actiontype() == TEAM_CONSCRIBE )
	{
		RecruitList::iterator it = mRecruitList.find( tpResponse->charid() );
		if ( it != mRecruitList.end() )
		{
			mRecruitList.erase( it );
		}
	}

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpResponse->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageTeamDeleteMsgCallBack tPara;
	MakeMessage<CMessageTeamDeleteMsgCallBack>( ID_S2C_NOTIFY_DELMSGCALLBACK, tPara, &tMessage );
	tPara.set_msgtype( tpResponse->actiontype() );
	tPara.set_entityid( tpPlayer->GetEntityID() );
	CMapModule::GetSingleton().MessageBroadCast( tpPlayer, &tMessage,false, true );
	return ;
}

void CTeamModule::OnGateChangeDistribute( CMessage* pMessage )
{
	CMessageChangeDistributeResponse* tpResponse = ( CMessageChangeDistributeResponse* ) pMessage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpResponse->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( InTeam( tpPlayer->GetCharID() ) );
	if ( tpTeam == NULL )
	{
		return ;
	}

	tpTeam->SetMode( tpResponse->mode() );

	CMessage tMessage;
	CMessageTeamChangeDistributeCallBack tPara;
	MakeMessage<CMessageTeamChangeDistributeCallBack>( ID_S2C_NOTIFY_CHANGEDISTRIBUTERTN, tPara, &tMessage );
	tPara.set_distribute( tpTeam->GetMode() );
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
	return ;
}

void CTeamModule::OnGateSendErrNotice( CMessage* pMessage )
{
	if( pMessage == NULL ) return ;

	CMessageNotifyErrToScene* tpNotify = ( CMessageNotifyErrToScene* ) pMessage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpNotify->descharid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageTeamNotice tPara;
	tPara.set_code( tpNotify->code() );
	tPara.set_param1( tpNotify->param1() );
	tPara.set_param2( tpNotify->param2() );
	MakeMessage<CMessageTeamNotice>(ID_S2C_NOTIFY_TEAMNOTICE, tPara, &tMessage);
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
	return ;
}
