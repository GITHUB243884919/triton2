#include "corpstype.h"
#include "familytype.h"
#include "object.h"  
#include "family_manager.h"
#include "familydbinfo_pb.hxx.pb.h"	 
#include "corpsinfo_pb.hxx.pb.h"
#include "corpsmessage_pb.hxx.pb.h"
#include "entity.h"
#include "gatelogic.h"
#include "errcode.h"	
#include "corpsmanager.h"  
#include "corpservice.h"
#include "nameservice.h"
#include "servermessage_pb.hxx.pb.h"
#include "gateobj_manager.h"
#include "familyservice.h"
#include "chatmessage_pb.hxx.pb.h"
#include "chattype.h"
#include "core_service.h"
#include "repetionmodule.h"
#include "tinyxml.h"
#include "world.h"
#include "log_tools.h"
#include "template.h"
#include "repetionmessage_pb.hxx.pb.h"
#include "gatelogic.h"

#include <google/protobuf/descriptor.h>  
typedef ::google::protobuf::Message Message;
using namespace GateServer;	

//-------------------------------------------------------------------------
#include "bidmgr.h"
#include "battlemgr.h"

//-------------------------------------------------------------------------
typedef ::google::protobuf::Message Message;
using namespace GateServer;	

 
// ***********************************************************
//  Function:		OnMessage
//  Description:	客户端消息
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessage( int nServerID, CMessage* pMessage )
{

}

// ***********************************************************
//  Function:		OnSeverMessage
//  Description:	场景服务器的消息
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnSeverMessage(int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	switch( pMessage->mutable_msghead()->messageid() )
	{
		case ID_C2S_REQUEST_CREATCORPS:
			{
				OnMessageCreatCorps( nServerID, pMessage );
				break;				 
			}	
		case ID_C2S_REQUEST_APPLYCORPS:
			{
				OnMessageApplyCorps( nServerID, pMessage );
				break;
			}	
		case ID_C2S_REQUEST_REPLYCORPSMEMBERAPPLY:
			{
				OnMessageReplyMemberApply( nServerID, pMessage );
				break;
			}		

		case ID_C2S_REQUEST_INVITECORPSMEMBER:
			{			
				OnMessageInviteCorpsMember( nServerID, pMessage );
				break;
			}

		case ID_C2S_REQUEST_REPLYCORPSINVITE:
			{
				OnMessageReplyCorpsInvite( nServerID, pMessage );
				break;
			}	

		case ID_C2S_REQUEST_LEAVECORPS:
			{
				OnMessageLeaveCorps( nServerID, pMessage );
				break;
			}	

		case ID_C2S_REQUEST_KICKFAMILY:
			{
				OnMessageKickFamily( nServerID, pMessage );
				break;
			}

		case ID_C2S_REQUEST_GETCORPSFAMILYINFO:
			{
				OnMessageGetCorpsFamiyInfo( nServerID, pMessage );
				break;
			}

		case ID_C2S_REQUEST_GETCORPSPROPERTYINFO:
			{
				OnMessageGetCorpsPropertyInfo( nServerID, pMessage );
				break;
			}		

		case ID_C2S_REQUEST_CHANGECORPSBULLETIN:
			{
				OnMessageChangeCorpsBulletin( nServerID, pMessage );
				break;
			}

		case ID_C2S_REQUEST_CHANGECORPSHEADER:
			{
				OnMessageChanggeCorpsHeader( nServerID, pMessage );
				break;
			}	

		case ID_C2S_REQUEST_GETCORPSAPPLYLIST:
			{
				OnMessageGetCorpsApplyList( nServerID, pMessage );
				break;
			}

		case ID_C2S_REQUEST_DISBANDCORPS:
			{
				OnMessageDisbandCorps( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_CANCELCORPSDISBAND:	
			{
				OnMessageCancelCorpsDisband( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_CLEARCORPSAPPLY:
			{
				OnMessageClearCorpsApply( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_REFRESHCORPSINFO:
			{
				OnMessageRefreshCorpsInfo( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_GETCOPRSLIST:
			{
				OnMessageGetCorpsList( nServerID, pMessage );
				break;
			}
			
		case ID_C2S_REQUEST_SETCORPSPOST:
			{
				OnMessageSetCorpsPost( nServerID, pMessage );
				break;
			}
		
		case ID_C2S_REQUEST_LEVELUPCORPS:
			{
				OnMessageLevelUPCorps( nServerID, pMessage );
				break;
			}

		case ID_C2S_REQUEST_CONTRIBUTECORPSMONEY:
			{
				OnMessageContributeCorpsMoney( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_FAMILYCONTRIBUTEMONEY:
			{
				 OnMessageFamilyContributeMoney( nServerID, pMessage );
				 break;
			}		

		case ID_C2S_REQUEST_SETMEMBERNAME:
			{
				OnMessageSetMemberName( nServerID, pMessage );
				break;
			}			
			
		case ID_C2S_REQUEST_QUERYBIDRANK:
			{
				OnMessageQueryBidRankRequest( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_BATTLEBID:
			{
				OnMessageCorpsBidRequest( nServerID, pMessage );
				break;
			}
		
		case ID_S2G_REQUEST_BIDTIMERSTOP:
			{
				 OnMessageBidTimeOutRequest( nServerID, pMessage );
				 break;
			}
		case ID_C2S_REQUEST_SELECTFAMILY:
			{
				OnMessageSelectBattleFamily( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_SELECTBATTLE:
			{
				OnMessageSelectBattleMember( nServerID, pMessage );
				break;
			}
		case ID_S2G_NOTIFY_CHANGECORPSCONTRIBUTE:
			{
				OnMessagChangeCorpsContribute( pMessage );
				break;
			}
		case ID_S2G_NOTIFY_CHANGECORPSGLORY:
			{
				OnMessageChangeCorpsGlory( pMessage );
				break;
			}
		case ID_S2G_NOTIFY_CHANGECORPSMONEY:
			{
				OnMessageChangeCorpsMoney( pMessage );
				break;
			}
		case ID_S2G_REQUEST_BATTLERESULT:
			{
				OnMessageBattleResultRequest( nServerID, pMessage );
				break;
			}
		case ID_S2G_REQUEST_BATTLEFLAG:
			{
				OnMessageBattleFlagRequest( nServerID, pMessage );
				break;
			}
		case ID_S2G_REQUEST_STARTCOPRSBATTLE:
			{
				OnMessageStartBattleRequest( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_ROOMOPERATION:
			{
				OnMessageOperateRoomRequest( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_CHALLEGE:
			{
				OnMessageChallegeRequest( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_REPLYCHALLEGE:
			{
				OnMessageReplyChallegeRequest( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_QUERYROOM:
			{
				OnMessageQueryRoomRequest( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_QUERYCHALLEGELIST:
			{
				OnMessageQueryChallegeRequest( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_QUERYRECCHALLEGE:
			{
				OnMessageQueryRecChallegeRequest( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_ABORTBATTLECITY:
			{
				OnMessageAbortBattleCityRequest( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_QUERYCORPSWELFARE:
			{
				OnMessageQueryCorpsWelfare( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_OBTAINCORPSWELFARE:
			{
				OnMessageObtainCorpsWelfare( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_GETCORPSMONEY:
			{
				OnMessageGetCorpsMoney( nServerID, pMessage );
				break;
			}
		case ID_S2G_REQUEST_BUYCORPSITEM:
			{
				OnMessageBuyCorpsItem( nServerID, pMessage );
				break;
			}

		case ID_C2S_REQUEST_CREATEUNION:
			{
				OnMessageCreateUnion( nServerID, pMessage );
				break;				 
			}	
		case ID_C2S_REQUEST_MAKEUNION:
			{
				OnMessageMakeUnion( nServerID, pMessage );
				break;				 
			}	
		case ID_C2S_REQUEST_APPLYUNION:
			{
				OnMessageApplyUnion( nServerID, pMessage );
				break;
			}	
		case ID_C2S_REQUEST_REPLYUNIONMEMBERAPPLY:
			{
				OnMessageReplyUnionApply( nServerID, pMessage );
				break;
			}		

		case ID_C2S_REQUEST_INVITEUNIONMEMBER:
			{			
				OnMessageInviteUnionMember( nServerID, pMessage );
				break;
			}

		case ID_C2S_REQUEST_REPLYUNIONINVITE:
			{
				OnMessageReplyUnionInvite( nServerID, pMessage );
				break;
			}	

		case ID_C2S_REQUEST_LEAVEUNION:
			{
				OnMessageLeaveUnion( nServerID, pMessage );
				break;
			}	

		case ID_C2S_REQUEST_KICKCORPS:
			{
				OnMessageKickCorps( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_UNIONINFO:
			{
				OnMessageGetUnionInfo( nServerID, pMessage );
				break;
			}	

		case ID_C2S_REQUEST_DISMISSUNION:
			{
				OnMessageDismissUnion( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_UNIONAPPLYLIST:
			{
				OnMessageUnionApplyList( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_ALLUNIONLIST:
			{
				OnMessageAllUnionList( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_GETOTHERMEMBER:
			{
				//OnMessageGetOtherMemberList( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_GETALLMAP:
			{
				OnMessageGetAllMapList( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_UNIONSUBMIT:
			{
				OnMessageGetUnionBattleList( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_APPOINTPRINCE:
			{
				OnMessageAppointPrince( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_MOVECAPITAL:
			{
				OnMessageMoveCapital( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_SELECTCOLOR:
			{
				OnMessageSetCorpsColor( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_CHANGEUNIONNAME:
			{
				OnMessageChangeUnionName( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_GETCORPSGOODSLIST:
			{
				OnMessageGetCorpsGoodsList( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_GETCORPSEXCHANGEGOODSLIST:
			{
				OnMessageGetCorpsExchangeGoodsList( nServerID, pMessage );
				break;
			}
	
		case ID_S2G_REQUEST_CORPSEXCHANGE:
			{
				OnMessageCorpsExchange( nServerID, pMessage );
				break;
			}
		case ID_S2G_REQUEST_ADDCORPSEXCHANGEITEM:
			{
				OnMessageAddCorpsExchangeItem( nServerID, pMessage );
				break;
			}
		case ID_S2G_REQUEST_CALLBACK_CORPSMONEY:
			{
				OnMessageRollCorpsMoney( nServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_LOGINBATTLE:
			{
				OnMessageLoginBattleRequest( nServerID, pMessage );
				break;
			}
		case ID_S2G_REQUEST_BATTLEMAP:
			{
				OnMessageSendBattleMapRequest( nServerID, pMessage );
				break;
			}
		case ID_S2G_CHANGE_CORPSBOSSTIMES:
			{
				OnMessageChangeBossTimes( nServerID, pMessage );
				break;
			}
		case ID_S2G_CHANGE_CORPSBOSSREFINEVALUE:
			{
				OnMessageChangeBossRefineValue( nServerID, pMessage );
				break;
			}
		case ID_C2S_SUMMON_CORPSBOSS_REQUEST:
			{
				OnMessageSummonCorpsBoss( nServerID, pMessage );
				break;
			}
		case ID_S2G_CHANGE_CORPSCOLLECTACTIVE:
			{
				OnMessageChangeCorpsCollectActive( nServerID, pMessage );
				break;
			}
		default:
			break;
	}		
}



// ***********************************************************
//  Function:		OnMessageApplyCorps
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageApplyCorps( int nServerID, CMessage* pMessage )
{	
	LK_ASSERT( pMessage != NULL, return )
	CMessageApplyCorpsRequest *tpMsg = ( CMessageApplyCorpsRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	
	if( tpPlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find player(CGatePlayer) of roleid:%d ", tpHead->roleid() );
		return;	
	}
	  
	
	CCorps *tpCorps = NULL;
	if ( tpMsg->has_corpsid() )
	{
		tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid() );
	}
	else if ( tpMsg->has_corpsname() )
	{
		LK_ASSERT( tpMsg->mutable_corpsname()->length() < MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES , return ) 		
		tpCorps = CCorpsManager::GetSingleton().GetCorpsByName( tpMsg->mutable_corpsname()->c_str() );
	}
	else 
	{
		tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpMsg->roleid() );
	}	
	if ( tpCorps == NULL )
	{
		SendResponseApplyCorps( nServerID, tpHead->roleid(), ERROR_CORPS_APPLY_NOTEXIST, 0, NULL, tpHead->entityid() );  
		return;
	}
	FamilyData tFamilyData;
	int tRet = CCorpsManager::GetSingleton().OnEventApplyCorps( tpHead->roleid(), tpCorps, tFamilyData );
	if ( tRet == -1 )
	{
		return;
	}
	if ( tRet == 0 )
	{
		
		/*CGatePlayer *tpCorpsRooter = CPlayerManager::GetSingleton().ScenePlayer( tpCorps->GetRooterID() );		
		if ( tpCorpsRooter != NULL )
		{
			SendNotifyCorpsMemberApply( tpCorpsRooter, tpCorpsRooter->mCharID, tFamilyData );	
		}		*/
		// 获取军团中有权限的职位最高的在线管理者
		CGatePlayer *tpManager = CCorpsManager::GetSingleton().GetManagerOfRight( tpCorps, EM_APPROVE_APPLY );
		if ( tpManager != NULL )
		{
			SendNotifyCorpsMemberApply( tpManager, tpManager->mCharID, tFamilyData );	
		}
	}
	SendResponseApplyCorps( nServerID, tpHead->roleid(), tRet, 0, tpCorps->GetCorpsName(), tpHead->entityid() );
}

//////////////////////////////////////////////////////////////////////////
/// 查询NPC竞价列表和被占领情况
void CCorpservice::OnMessageQueryBidRankRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}
	
	CMessageCorpsQueryBidRankRequest* tpReq = ( CMessageCorpsQueryBidRankRequest* ) pMessage->msgpara();

	CMessage tMessage;
	CMessageCorpsQueryBidRankResponse tPara;

	// 临时存放查询结果
	int tCorpsList[ SERVER_CAP_CORPS ] = { 0 };	//军团ID
	int tBidMoney[ SERVER_CAP_CORPS ]  = { 0 };	//竞价金钱
	int tCorpsNum = ARRAY_CNT( tCorpsList );
	// 遍历竞价列表得到该NPC竞价列表，
	CBidMgr::GetSingleton().GetBidListByNpcID( GATENPCID(tpReq->npcid(), nServerID), tCorpsList, tBidMoney, tCorpsNum );

	// 生成返回消息
	for ( int i = 0; i < tCorpsNum; i ++ )
	{
		CCorps* tpCorps = ( CCorps* ) CCorpsManager::GetSingleton().GetCorpsByID( tCorpsList[ i ] );
		if ( tpCorps == NULL )
		{
			continue;
		}
		PBBidRank* tpRank = tPara.add_rankinfo();
		tpRank->set_corpsname( tpCorps->GetCorpsName() );
		tpRank->set_money( tBidMoney[ i ] );
		tpRank->set_nationality( tpCorps->GetCorpsCountryID() );
		tpRank->set_victory( tpCorps->GetVictoryTimes() );
		tpRank->set_loser( tpCorps->GetLoserTimers() );
		tpRank->set_corpsid( tpCorps->GetCorpsID() );
	}
	
	CorpsHead* tpHead = tpReq->mutable_corphead();
	tPara.set_npcid( tpReq->npcid() );
	tPara.mutable_corphead()->set_roleid( tpHead->roleid() );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYBIDRANK ); 

	//找出该NPC的被占领者
	int tCorpsID = 0;
	tCorpsID = CBidMgr::GetSingleton().FindNpcOwnerByNpcID(GATENPCID( tpReq->npcid(), nServerID ));
	
	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tCorpsID );
	if ( tpCorps != NULL )
	{
		tPara.set_cityowner( tpCorps->GetCorpsName() );
	}	

	//返回消息
	tMessage.set_msgpara( (long) &tPara );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );

	return ;
}



// 竞价请求 [11/24/2009 Macro]
void CCorpservice::OnMessageCorpsBidRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}
	
	LOG_DEBUG("corps", "OnMessageCorpsBidRequest");

	CMessageCorpsBattleBidRequest* tpReq = ( CMessageCorpsBattleBidRequest* ) pMessage->msgpara();

	CMessage tMessage;
	CMessageCorpsBattleBidResponse tPara;
	tPara.mutable_corphead()->set_roleid( tpReq->mutable_corphead()->roleid() );
	tPara.mutable_corphead()->set_entityid( tpReq->mutable_corphead()->entityid() );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_BATTLEBID );
	tMessage.set_msgpara( (long) &tPara );
 
	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpReq->mutable_corphead()->roleid() );
	if ( tpCorps == NULL )
	{
		LOG_DEBUG("corps", "OnMessageCorpsBidRequest tpCorps == NULL");
		tPara.set_rtncode( ERROR_CORPS_NOTEXIST );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
		return ;
	}

	// 转换到网关NpcID；
	int tNpcID = GATENPCID( tpReq->npcid(), nServerID );

	// 参与竞价
	CBidMgr* pBidMgr = CBidMgr::GetSingletonPtr();
	
	int ErrCode = -111;
	ErrCode = (pBidMgr->BidForBattle(tpCorps, (int)tpReq->mutable_corphead()->roleid(), tNpcID, (int)tpReq->mapid(), (int)tpReq->money()));
	if (ErrCode)
	{
		if (-1 == ErrCode)
		{
			return;
		}
		tPara.set_rtncode(ErrCode);
		LOG_DEBUG("corps", "OnMessageCorpsBidRequest Err:%d", ErrCode);
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
		return ;
	}

	//竞价成功
	tPara.set_rtncode( SUCCESS );
	tPara.set_npcid( tpReq->npcid() );
	tPara.set_money( tpReq->money() );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );

	// 设置竞价npc状态
	//tpCorps->SetNpcBattleInfo( tpReq->npcid(), emBidStep, tpReq->money(), tpReq->mapid() );

	return ;
}

// 竞价时间结束 [11/24/2009 Macro]
void CCorpservice::OnMessageBidTimeOutRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}
	
	CMessageCorpsBidTimerStopRequest* tpReq = ( CMessageCorpsBidTimerStopRequest* ) pMessage->msgpara(); 

	// 转换到网关NpcID；
	int GateNpcID = GATENPCID( tpReq->npcid(), nServerID );

	CCorps* pAttacker;
	CCorps* pDefender;
	int MoneyAtt;
	int MoneyDef;

	int RetCode = CBidMgr::GetSingleton().OnBidFinishTimesUp(GateNpcID, pAttacker, pDefender, MoneyAtt, MoneyDef);

	if (RetCode)
	{
		//没有竞价数据或其它错误	
		return;
	}
	if (pDefender && NULL == pAttacker)
	{
		// 只有一个军团竞价，直接获胜
		LOG_NOTICE( "corps", "ServerID[%d] Npc[%d]: can't find Rank_2 corps, so Rank_1 corps[%d] got win with money: %d", 
			nServerID, 
			tpReq->npcid(),
			pDefender->GetCorpsID(),
			MoneyDef);
		OnDoBattleResultEvent(pDefender, tpReq->npcid(), tpReq->battleid());
		return ;
	}	

	LOG_NOTICE( "corps", "/=========Corps Battle Bid Time Over==========" );
	LOG_NOTICE( "corps", "| ServerID[%d]NpcID[%d],Attacker-Corps:[%s: %d,Money:%d]", nServerID, tpReq->npcid(), pAttacker->GetCorpsNameShow(), pAttacker->GetCorpsID(), MoneyAtt);
	LOG_NOTICE( "corps", "| ServerID[%d]NpcID[%d],Defender-Corps:[%s: %d,Money:%d]", nServerID, tpReq->npcid(), pDefender->GetCorpsNameShow(), pDefender->GetCorpsID(), MoneyDef);
	LOG_NOTICE( "corps", "\\=============================================" );


	int AttackerType = ( pAttacker->GetUnionID() == INVALID_OBJ_ID ) ? (int) emTypeCorps : (int) emTypeUnion;
	int DefenderType = ( pDefender->GetUnionID() == INVALID_OBJ_ID ) ? (int) emTypeCorps : (int) emTypeUnion;

	// 发送参战报名通知
	SendBattleJoinNotice( nServerID, tpReq->npcid(), pAttacker->GetCorpsName(), pAttacker->GetRooterID(), AttackerType );
	SendBattleJoinNotice( nServerID, tpReq->npcid(), pDefender->GetCorpsName(), pDefender->GetRooterID(), DefenderType );

	// 组装消息，通知军团成员参战资格得主
	CMessage tMsg;
	CMessageCorpsRecBattleRightsNotify tNotify;
	tMsg.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_RECBATTLERIGHTS );
	tMsg.set_msgpara( (long) &tNotify );
	tNotify.set_npcid( tpReq->npcid() );	

	tNotify.set_descorpsid( pDefender->GetCorpsID() );
	tNotify.set_descorpsname( pDefender->GetCorpsName() );
	tNotify.set_srccorpsid( pAttacker->GetCorpsID() );
	tNotify.set_srccorpsname( pAttacker->GetCorpsName() );
	tNotify.set_money( MoneyAtt );

	CPlayerManager::GetSingleton().SendCountryMessage( &tMsg, nServerID );

	// 处理军盟逻辑
	OnEventUnionBattleTimeOut( tpReq->battleid(), pAttacker );
	OnEventUnionBattleTimeOut( tpReq->battleid(), pDefender );

	return ;
}

void CCorpservice::OnMessageSelectBattleFamily( int nServerID, CMessage* pMessage )
{
	;
}


// 报名 [2/3/2010 Macro]
void CCorpservice::OnMessageSelectBattleMember( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}
	
	CMessageCorpsSelectMemberBattleRequest* tpReq = ( CMessageCorpsSelectMemberBattleRequest* ) pMessage->msgpara();

	CMessage tMessage;
	CMessageCorpsSelectMemberBattleResponse tPara;
	tMessage.set_msgpara( (long) &tPara );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_SELECTBATTLE );
	tPara.mutable_corphead()->set_roleid( tpReq->mutable_corphead()->roleid() );

	int tCharID = tpReq->mutable_corphead()->roleid();	

	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tCharID );
	if ( tpCorps == NULL )
	{
		return ;
	}

	// 在战斗中不能选者参战阵容
	if ( tpCorps->IsInBattle(TYPE_BATTLE_ALL, ST_InBtl|ST_Union|ST_Single) )
	{
		tPara.set_rtncode( ERROR_NOT_CORPS_APPLY_TIME );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
		return ;
	}

	if ( tpCorps->GetRooterID() != tCharID )
	{
		if ( !tpCorps->MemberHasRight( tCharID, EM_WAR_SETFIGHTER ) )
		{
			return ;	
		}			
	}

	// 解散状态不能操作
	if ( tpCorps->GetCorpsStatus() == CCorps::EM_CORPS_STATUS_DISBAND )
	{
		return ;
	}

	int tBattleCount =  tpReq->memberlist_size();
	if ( tBattleCount > CORPS_BATTLE_NUM )
	{
		tPara.set_rtncode( ERROR_CORPS_MEMBEROVERFLOW );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
		return ;
	}

	CMessage tMsg;
	CMessageCorpsMemberBattleRightsNotify tNotify;
	tMsg.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MEMBERBATTLERIGHTS );
	tMsg.set_msgpara( (long) &tNotify );
	tNotify.set_npcid( tpCorps->GetBidNpcID() );

	if ( tpReq->type() == emTypeCorps )
	{
		CCorps* tpDesCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpCorps->GetEnemyCorpsID() );
		if ( tpDesCorps != NULL )
		{
			tNotify.set_descorpsname( tpDesCorps->GetCorpsName() );
		}

		tpCorps->ClearBattleList();
		for ( int i = 0; i < tpReq->memberlist_size(); i ++ )
		{
			tpCorps->InsertBattleMember( tpReq->memberlist( i ) );
			tNotify.add_memberlist( tpReq->memberlist( i ) );
		}
		tPara.set_rtncode( SUCCESS );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );

		tNotify.set_type( emTypeCorps );
		SendCorpsMsgByID( tpCorps->GetCorpsID(), &tMsg );

		int tNum = tpCorps->GetBattleListSize();
		LOG_NOTICE( "corps", "=========Submit battle list successfully!=========" );
		LOG_NOTICE( "corps", "ServerID[%d], CorpsName:%s,CorpsID:%d,BattleListNum:%d", nServerID, tpCorps->GetCorpsNameShow(), tpCorps->GetCorpsID(), tNum );
		LOG_NOTICE( "corps", "==================================================" );

		LOG_NOTICE( "corps", "BattleMemberList: %s", tNotify.ShortDebugString().c_str() );

		// LOG:参战报名/选者阵容
		LogSubmit( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tCharID, tNum, 0, 0 );

		// 如果有军盟,给盟主发通知
		CUnion* tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tpCorps->GetUnionID() );
		if ( tpUnion == NULL  )
			return ;
		
		// 主军团就不给盟主发了
		if ( tpUnion->GetCoreID() == tpCorps->GetCorpsID() )
			return ;

		CCorps* tpCoreCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpUnion->GetCoreID() );
		if ( tpCoreCorps == NULL )
			return ;

		LOG_NOTICE( "corps", "UnionType" );

		tNotify.set_type( emTypeUnion );
		CGatePlayer* tpRootPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpCoreCorps->GetRooterID() );
		if ( tpRootPlayer != NULL )
		{
			CGateServer::GetSingleton().Send2Player( tpRootPlayer, &tMsg );
		}
	}
	//TODO: 军盟用各子军团的参战阵容
	/*else if ( tpReq->type() == emTypeUnion )
	{
		CUnion* tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tpCorps->GetUnionID() );
		if ( tpUnion == NULL )
		{
			LOG_ERROR( "corps", "Union[%d] not found.[%s:%d]", tpCorps->GetUnionID(), __LK_FILE__, __LINE__ );
			return ;
		}

		tpUnion->ClearBattleList();
		for ( int i = 0; i < tpReq->memberlist_size(); i ++ )
		{
			tpUnion->InsertBattleMember( tpReq->memberlist( i ) );
			tNotify.add_memberlist( tpReq->memberlist( i ) );
		}
		tPara.set_rtncode( SUCCESS );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
		
		tNotify.set_type( emTypeUnion );
		SendCorpsMsgByID( tpCorps->GetCorpsID(), &tMsg );

		int tNum = tpCorps->GetBattleListSize();
		LOG_NOTICE( "corps", "=========Submit battle list successfully!=========" );
		LOG_NOTICE( "corps", "ServerID:[%d], CorpsName:%s,CorpsID:%d,BattleListNum:%d", nServerID, tpCorps->GetCorpsNameShow(), tpCorps->GetCorpsID(), tNum );
		LOG_NOTICE( "corps", "==================================================" );
	}*/
	
	return ;
}

// ***********************************************************
//  Function:		OnMessageQueryCorpsWelfare
//  Description:	查询军团npc福利的消息请求
//  Output:			
//  Date:	09/16/2009
// **********************************************************
void CCorpservice::OnMessageQueryCorpsWelfare( int nServerID, CMessage* pMessage )
{  
	LK_ASSERT( pMessage != NULL, return )	
	CMessageQueryCorpsWelfareRequest *tpMsg = ( CMessageQueryCorpsWelfareRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps"," can not find corps of player ( roleid:%d ) ", tpHead->roleid() );
		return;	    
	}
	
	CMessage tMsgHead;
	CMessageQueryCorpsWelfareResponse tMsgQueryWelfare;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYCORPSWELFARE );
	tMsgHead.set_msgpara( (int)&tMsgQueryWelfare );
	tMsgQueryWelfare.mutable_corphead()->set_roleid( tpHead->roleid() );
	tMsgQueryWelfare.mutable_corphead()->set_entityid( tpHead->entityid() );
	
	int tMoney = 0;
	int tGlory = 0;
	tpCorps->GetCorpsWelfare( tpMsg->npcid(), tMoney, tGlory, false );
	
	tMsgQueryWelfare.set_errcode( SUCCESS );
	tMsgQueryWelfare.set_npcmoney( tMoney );
	tMsgQueryWelfare.set_npcglory( tGlory );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
}

// ***********************************************************
//  Function:		OnMessageObtainCorpsWelfare
//  Description:	获取军团npc福利的消息请求
//  Output:			
//  Date:	09/16/2009
// **********************************************************
void CCorpservice::OnMessageObtainCorpsWelfare( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )	
	CMessageObtainCorpsWelfareRequest *tpMsg = ( CMessageObtainCorpsWelfareRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	LK_ASSERT( tpGatePlayer != NULL, return )

	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps", "can not find corps of player( roleid:%d )", tpHead->roleid() );
		return;
	}
	
	
	CMessage tMsgHead;
	CMessageObtainCorpsWelfareResponse tMsgObtainWelfare;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_OBTAINCORPSWELFARE );
	tMsgHead.set_msgpara( (int)&tMsgObtainWelfare );
	tMsgObtainWelfare.mutable_corphead()->set_roleid( tpHead->roleid() );
	tMsgObtainWelfare.mutable_corphead()->set_entityid( tpHead->entityid() );
	
	if ( tpCorps->GetRooterID() != (int)tpHead->roleid() )
	{
		if ( !tpCorps->MemberHasRight( tpHead->roleid(), EM_WELFARE_OBTAIN ) )
		{
			tMsgObtainWelfare.set_errcode( ERROR_CORPS_HASNORIGHT );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
			return;
		}	
	}
	
	int tMoney = 0;
	int tGlory = 0;
	tpCorps->GetCorpsWelfare( tpMsg->npcid(), tMoney, tGlory );
	
	if ( tMoney <= 0 && tGlory <= 0 )
	{
		tMsgObtainWelfare.set_errcode( ERROR_CORPS_NPCWELFAREOBTAINED);
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
		return;	
	}
	
	if ( tMoney > 0 )
	{
		int tOldCorpsMoney = tpCorps->GetCorpsMoney();
		tpCorps->SetCorpsMoney( tpCorps->GetCorpsMoney() + tMoney );		
		SendNotifyCorpsMoneyChanged( tpCorps->GetCorpsID(), tpCorps->GetCorpsMoney(), tpHead->roleid(), tMoney, REASON_NPCWELFARE, tpGatePlayer->CharName() );
		LogOrganGetNpcWelfareMoney( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpHead->roleid(), tpCorps->GetCorpsMoney(), tOldCorpsMoney, tpMsg->npcid())
	}
	if ( tGlory > 0 )
	{
		int tOldCorpsGlory = tpCorps->GetCorpsGlory();
		tpCorps->SetCorpsGlory( tpCorps->GetCorpsGlory() + tGlory );
		SendNotifyCorpsGlroyChangedNotify( tpCorps->GetCorpsID(), tpCorps->GetCorpsGlory(), tpHead->roleid(), tGlory, REASON_NPCWELFARE, tpGatePlayer->CharName() );
		LogOrganGetNpcWelfareGlory( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpHead->roleid(), tpCorps->GetCorpsGlory(), tOldCorpsGlory, tpMsg->npcid())
	}
	
	if ( tGlory > 0 || tMoney > 0 )
	{
		if ( tpCorps->CanCorpsLevelUP() )
		{	 
			SendNotifyCorpsCanLevelUP( tpCorps->GetCorpsID() );
		}
	}
	
	tMsgObtainWelfare.set_errcode( SUCCESS );
	tMsgObtainWelfare.set_npcglory( tGlory );
	tMsgObtainWelfare.set_npcmoney( tMoney );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );		
}

// ***********************************************************
//  Function:		OnMessageGetCorpsMoney
//  Description:	获取军团金钱的消息请求
//  Output:			
//  Date:	09/16/2009
// **********************************************************
void CCorpservice::OnMessageGetCorpsMoney( int nServerID, CMessage* pMessage )
{  
	LK_ASSERT( pMessage != NULL, return )	
	CMessageGetCorpsMoneyRequest *tpMsg = ( CMessageGetCorpsMoneyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	int tRet = ConsumeCorpsMoney( tpHead->roleid(), tpMsg->corpsmoney(), REASON_HEADERTAKE );
	if ( tRet == -1 )
	{
		return;
	}
	CMessage tMsgHead;
	CMessageGetCorpsMoneyResponse tMsgGetCorpsMoney;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETCORPSMONEY );
	tMsgHead.set_msgpara( (int)&tMsgGetCorpsMoney );
	
	tMsgGetCorpsMoney.mutable_corphead()->set_roleid( tpHead->roleid() );
	tMsgGetCorpsMoney.mutable_corphead()->set_entityid( tpHead->entityid() );
	tMsgGetCorpsMoney.set_corpsmoney( tpMsg->corpsmoney() );
	tMsgGetCorpsMoney.set_errcode( tRet );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		OnMessageBuyCorpsItem
//  Description:	购买军团红包的消息请求
//  Output:			
//  Date:	10/29/2009
// **********************************************************
void CCorpservice::OnMessageBuyCorpsItem( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageBuyCorpsItemRequest *tpMsg = ( CMessageBuyCorpsItemRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
		
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );	
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "corps","[%s,%d][%s] can not find player by roleid roleid:%d",  __LK_FILE__, __LINE__,
			__FUNCTION__, tpMsg->roleid() );
		return;
	}
	
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps","[%s,%d][%s] can not find corps of player (roleid:%d)",  __LK_FILE__, __LINE__,
			__FUNCTION__, tpMsg->corpsid() );
		return;
	}	
	
	// 判断权限
	if ( tpCorps->GetRooterID() != (int)tpMsg->roleid() )
	{
		if ( !tpCorps->MemberHasRight( tpMsg->roleid(), EM_MONEY_REIN ) )
		{
			SendResponseBuyCorpsItem( tpPlayer,  ERROR_CORPS_BUYCORPSITEM_HASNORIGHT, tpMsg->itemid(), tpMsg->itemnum(), tpMsg->entityid() );
			return;
		}		
	}
	
	int tOldCorpsMoney = tpCorps->GetCorpsMoney();
	
	// 获取npc的模板ID
	CTemplateNpc *tpTplNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpMsg->npctmpid() );
	if ( tpTplNpc == NULL )
	{
		LOG_ERROR( "corps","[%s,%d] can not find npc templateid:%d, roleid:%d ",__FUNCTION__, __LINE__, tpMsg->npctmpid(), tpMsg->roleid() );
		return;
	}

	// 获取出售表的模板ID
	CTemplateSellTable *tpTplSellTable = ( CTemplateSellTable * )CDataStatic::SearchTpl( tpTplNpc->mSellServiceID );
	if ( tpTplSellTable == NULL )
	{
		LOG_ERROR( "corps","[%s,%d] can not find selltable by id:%d, npc template id:%d, roleid:%d",__FUNCTION__, __LINE__, tpTplNpc->mSellServiceID, tpMsg->npctmpid(), tpMsg->roleid() );
		return;	
	}

	// 判断模板出售表的类型
	if ( tpTplSellTable->mSellTableType != CTemplateSellTable::SELLTABLETYPE_CORPSMONEYNEEDED && tpTplSellTable->mSellTableType != CTemplateSellTable::SELLTABLETYPE_CORPSWAR )
	{
		LOG_ERROR( "corps","[%s,%d] selltable is not used for corps, selltable id:%d, npctemplate id:%d, role id:%d ",__FUNCTION__,__LINE__, tpTplSellTable->mTempID, tpMsg->npctmpid(), tpMsg->roleid() );
		return;
	}

	// 如果是战争商店，必需判断主城是否有改商店
	CLeagueShopInfo tLeagueShopinfo;
	if ( tpTplSellTable->mSellTableType == CTemplateSellTable::SELLTABLETYPE_CORPSWAR )
	{  
		if ( tpCorps->HasOwnThisMap( tpTplSellTable->mWarCityMapID ) == false )
		{ 			 
			SendResponseBuyCorpsItem( tpPlayer, ERROR_CORPS_GETGOODLIST_HASNOTOWNCITY, tpMsg->itemid(), tpMsg->itemnum(), tpMsg->entityid() );
			return;					
		}					
	}  
	
	 // 计算金钱
	CGoodSInfo tGoodInfo;
	int tGoldPrice;
	tpCorps->GetCorpsShop()->GetItemInfoInSellTable( tpTplSellTable->mTempID, tpMsg->itemid(), tGoodInfo, tGoldPrice );	
	if( tGoldPrice == 0 || tGoodInfo.mItemNum == 0 ) 
	{
		LOG_ERROR( "corps","[%s,%d] wrong item info, item number:%d, item goldprice:%d in template ",__FUNCTION__,__LINE__, tGoodInfo.mItemNum, tGoldPrice );
		return;
	}

	int tMoneyNeed = tGoldPrice*tpMsg->itemnum();
	if ( tpCorps->CheckMoneyEnough( tMoneyNeed ) == false )
	{		
		SendResponseBuyCorpsItem( tpPlayer, ERROR_CORPS_BUYREDPAPER_MONEYNOTENOUGH, tpMsg->itemid(), tpMsg->itemnum(), tpMsg->entityid() );
		return;
	}
	
	// 购买物品
	int tShopRet = tpCorps->GetCorpsShop()->BuyItem( tpTplSellTable->mTempID, tpMsg->itemid(), tpMsg->itemnum() );
	int tErrcode = 0;
	if ( tpCorps->GetCorpsShopErrcode( tShopRet, tErrcode ) == -1 )
	{
		return;
	}	

	// 如果购买成功
	if ( tErrcode == SUCCESS )
	{
		tpCorps->ConsumeCorpsMoney( tMoneyNeed );	
		
		LOG_NOTICE( "corps","[%s,%d] consume corps money by buy corps item( managerid:%d, corpsid:%d, money:%d ) ", __FUNCTION__,__LINE__, tpMsg->roleid(), tpCorps->GetCorpsID(), tpMsg->totalcost() );
		SendNotifyCorpsMoneyChanged( tpCorps->GetCorpsID(), tpCorps->GetCorpsMoney(), tpMsg->roleid(), -1*tMoneyNeed, REASON_MAKEREDPAPER, tpPlayer->mCharName );
		LogOrganBuyItem( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpMsg->roleid(), tpCorps->GetCorpsMoney(), tOldCorpsMoney );		
		
		// 发送购买物品刷新通知
		tpCorps->GetCorpsShop()->GetWarShopItemInfo( tpTplSellTable->mTempID, tpMsg->itemid(), tGoodInfo, tGoldPrice );
		SendNotifyChangeCorpsGoodsList( tpMsg->corpsid(), tpMsg->npctmpid(), tGoodInfo.mItemID, tGoodInfo.mItemNum );
	}	
	
	SendResponseBuyCorpsItem( tpPlayer, tErrcode, tpMsg->itemid(), tpMsg->itemnum(), tpMsg->entityid() );
}

// 强制战斗结束处理
void CCorpservice::OnEventBattleOver( int nWinCamp, KEY_TYPE nKey )
{
	CMessage tMessage;
	CMessageNotifyBattleResult2Gate tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_BATTLERESULT );
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_wincamp( nWinCamp );
	tPara.set_battlekey( nKey );
	tPara.set_battleid( CORPS_BATTLE_SCABBLE );
	this->OnMessageBattleResultRequest( 0, &tMessage );
	return ;
}

// 战争结果处理 [11/28/2009 Macro]
void CCorpservice::OnMessageBattleResultRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageNotifyBattleResult2Gate* tpReq = ( CMessageNotifyBattleResult2Gate* ) pMessage->msgpara();

	KEY_TYPE tKey = ( KEY_TYPE )tpReq->battlekey();

	// 根据Key取出战争信息
	int tSrcCorpsID = 0, tDesCorpsID = 0;
	int tWinCorpsID = 0, tLostCorpsID = 0, tType = 0, tNpcID = 0, tMapID = 0;
	CBattleMgr::GetSingleton().GetBattle( tKey, tSrcCorpsID, tDesCorpsID, tType, tNpcID, tMapID );

	if ( tpReq->wincamp() == CAMP_1 )
	{
		tWinCorpsID = tSrcCorpsID;
		tLostCorpsID = tDesCorpsID;
	}
	else
	{
		tWinCorpsID = tDesCorpsID;
		tLostCorpsID = tSrcCorpsID;
	}

	CCorps* tpVictoryCorps = CCorpsManager::GetSingleton().GetCorpsByID( tWinCorpsID );
	CCorps* tpLoserCorps = CCorpsManager::GetSingleton().GetCorpsByID( tLostCorpsID );
	if ( tpVictoryCorps == NULL || tpLoserCorps == NULL )
	{
		return ;
	}

	// 场景NPC ID
	int tOldLine = tpVictoryCorps->GetCorpsCountryID();
	int tSceneNpcID = SCENENPCID( tNpcID, tOldLine );

	tpVictoryCorps->IncVictoryTimes();
	tpLoserCorps->IncLoserTimes();

	tpVictoryCorps->SetCorpsCamp( 0 );
	tpLoserCorps->SetCorpsCamp( 0 );

	if ( tType == TYPE_BATTLE_CHALLEGE )
	{
		//挑战赛
		tpVictoryCorps->SetRoomState( ROOM_STATE_OPEN );
		tpLoserCorps->SetRoomState( ROOM_STATE_OPEN );

		if ( CCorpsManager::GetSingleton().HasRoom( tpLoserCorps->GetCorpsID() ) )
		{
			CCorpsManager::GetSingleton().OnDelRoom( tpLoserCorps->GetCorpsID() );
		}

		tpLoserCorps->SetRoomMoney( 0 );	
		
		// LOG:挑战赛胜利
		LogOrganChallengeVictory( ORGANLOGTYPE_CORPS,
			tpVictoryCorps->GetCorpsID(), 0, tpLoserCorps->GetCorpsID(), 0, 0 );	
	}
	else if ( tType == TYPE_BATTLE_SCRABBLE )
	{
		
		//争霸赛

		//胜方获得称号
		tpVictoryCorps->UpdateNationTitle(tMapID, true);
		
		if ( IS_MAIN_CITY( tMapID ) )
		{
			CWorldManager::GetSingleton().LoadCorpsFromMap( tOldLine, tMapID, 0 );
		}

		tpLoserCorps->ClearNpcBattleInfo( tSceneNpcID );
		
		int tOldCity = tpVictoryCorps->GetBattleCity();
		
		CBidMgr::GetSingleton().SetNpcOwner(tNpcID, tWinCorpsID);
		
		// 是否是主城
		if ( IS_MAIN_CITY( tMapID ) )
		{
			
			int tNewCity = tMapID;
			// 原来占非长安,当前占领长安 [12/2/2009 Macro]
			if ( IS_MAIN_CITY( tOldCity ) && !IS_CHANGAN( tOldCity ) && IS_CHANGAN( tNewCity ) )
			{
				CWorldManager::GetSingleton().LoadCorpsFromMap( tOldLine, tNewCity, tpVictoryCorps->GetCorpsID() );
				CWorldManager::GetSingleton().LoadCorpsFromMap( tOldLine, tOldCity, 0 );
				tpVictoryCorps->SetBattleCity( tNewCity );
			}
			else if ( tOldCity == INVALID_MAP_ID && IS_MAIN_CITY( tNewCity ) )
			{
				CWorldManager::GetSingleton().LoadCorpsFromMap( tOldLine, tNewCity, tpVictoryCorps->GetCorpsID() );
				tpVictoryCorps->SetBattleCity( tNewCity );
			}			
			// 上述情形都不是，那维护原来状态
			else
			{
				CWorldManager::GetSingleton().LoadCorpsFromMap( tOldLine, tOldCity, tpVictoryCorps->GetCorpsID() );
			}

			tpLoserCorps->SetBattleCity( 0 );			
		}	

		// LOG:争夺战胜利
		LogOrganBattleVictory( ORGANLOGTYPE_CORPS,
			tpVictoryCorps->GetCorpsID(), 0, tpLoserCorps->GetCorpsID(), tSceneNpcID, tMapID );
		
		OnEventCityUpgrade( tpLoserCorps, tOldLine );
		
		// 同步NPC状态到SceneServer
		CCorpsManager::GetSingleton().SendCorpsNpcSingle( tOldLine, tSceneNpcID, tpVictoryCorps->GetCorpsID() );
	}	

	// 战场key 作为战场列表的主键
	CBattleMgr::GetSingleton().DelBattle( tKey );

	// 战争结束，通知更新军团属性
	SendCorpsProperty( tpVictoryCorps->GetCorpsID() );
	SendCorpsProperty( tpLoserCorps->GetCorpsID() );

	CMessage tMessage;
	CMessageCorpsBattleResultNotice tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSBATTLERESULT );
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_wincorpsname( tpVictoryCorps->GetCorpsName() );
	tPara.set_lostcorpsname( tpLoserCorps->GetCorpsName() );
	tPara.set_wincampid( tpReq->wincamp() );
	tPara.set_npcid( tSceneNpcID );
	tPara.set_battletype( tType );
	tPara.set_wincorpsid( tpVictoryCorps->GetCorpsID() );
	tPara.set_lostcorpsid( tpLoserCorps->GetCorpsID() ); 

	CPlayerManager::GetSingleton().SendCountryMessage( &tMessage, tOldLine );

	// 积分、荣耀增加
	CCorpsConfig* tpConfig = CCorpsManager::GetSingleton().GetCorpsConfig();
	if ( tpConfig == NULL )
	{
		return ;
	}

	tpVictoryCorps->AddPvPScore( tpConfig->GetScorePerFight(), tpLoserCorps->GetPVPStarLevel() );
	tpVictoryCorps->AddCorpsGlory( tpConfig->GetGloryPerFight() );
	
	tpLoserCorps->ReducePvPScore( tpConfig->GetScorePerFight(), tpVictoryCorps->GetPVPStarLevel() );
	
	if ( tpVictoryCorps->CanCorpsLevelUP() )
	{
		SendNotifyCorpsCanLevelUP( tpVictoryCorps->GetCorpsID() );
	}

	return; 
}

// 战争未战即胜处理(争霸赛) [11/28/2009 Macro]
// 1、只有一方竞价,npc未被占领(之前);
// 2、对方没有报名参战,视为放弃
void CCorpservice::OnDoBattleResultEvent( CCorps* npWinCorps, int nNpcID, int nMapID, CCorps* npLosCorps /* = NULL */ )
{
	if ( npWinCorps == NULL )
	{
		return ;
	}

	int tServerLine = npWinCorps->GetCorpsCountryID();

	int tOldCity = npWinCorps->GetBattleCity();
	
	CBidMgr::GetSingleton().SetNpcOwner(GATENPCID( nNpcID, tServerLine ), npWinCorps->GetCorpsID());
	
	npWinCorps->IncVictoryTimes();

	// 增加了一个占领地图, 需要国家类称号处理
	npWinCorps->UpdateNationTitle(nMapID, true );
	npWinCorps->SetCorpsCamp( 0 );

	// 是否是主城
	if ( IS_MAIN_CITY( nMapID ) )
	{
		int tNewCity = nMapID;
		// 原来占非长安,当前占领长安 [12/2/2009 Macro]
		if ( IS_MAIN_CITY( tOldCity ) && !IS_CHANGAN( tOldCity ) && IS_CHANGAN( tNewCity ) )
		{
			CWorldManager::GetSingleton().LoadCorpsFromMap( tServerLine, tNewCity, npWinCorps->GetCorpsID() );
			CWorldManager::GetSingleton().LoadCorpsFromMap( tServerLine, tOldCity, 0 );
			npWinCorps->SetBattleCity( tNewCity );
		}
		else if ( tOldCity == INVALID_MAP_ID && IS_MAIN_CITY( tNewCity ) )
		{
			CWorldManager::GetSingleton().LoadCorpsFromMap( tServerLine, tNewCity, npWinCorps->GetCorpsID() );
			npWinCorps->SetBattleCity( tNewCity );
		}
		// 上述情形都不是，那维护原来状态
		else
		{
			CWorldManager::GetSingleton().LoadCorpsFromMap( tServerLine, tOldCity, npWinCorps->GetCorpsID() );
		}
	}			
	
	if ( npLosCorps != NULL )
	{
		npLosCorps->IncLoserTimes();
		npLosCorps->ClearNpcBattleInfo( nNpcID );
		npLosCorps->SetCorpsCamp( 0 );
		npLosCorps->SetBattleCity( 0 );
		SendCorpsProperty( npLosCorps->GetCorpsID() );

		
		OnEventCityUpgrade( npLosCorps, tServerLine );

		// LOG:争夺战胜利
		LogOrganBattleVictory( ORGANLOGTYPE_CORPS,
			npWinCorps->GetCorpsID(), 0, npLosCorps->GetCorpsID(), nNpcID, nMapID );
	}
	else
	{
		// LOG:争夺战胜利
		LogOrganBattleVictory( ORGANLOGTYPE_CORPS,
			npWinCorps->GetCorpsID(), 0, 0, nNpcID, nMapID );
	}

	CMessage tMessage;
	CMessageCorpsBattleResultNotice tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSBATTLERESULT );
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_wincorpsname( npWinCorps->GetCorpsName() );
	tPara.set_wincampid( 2 );
	tPara.set_npcid( nNpcID );
	tPara.set_battletype( TYPE_BATTLE_SCRABBLE );
	tPara.set_wincorpsid( npWinCorps->GetCorpsID() );

	CPlayerManager::GetSingleton().SendCountryMessage( &tMessage, tServerLine );

	CCorpsManager::GetSingleton().SendCorpsNpcSingle( tServerLine, nNpcID, npWinCorps->GetCorpsID() );
	
	SendCorpsProperty( npWinCorps->GetCorpsID() );

	// 积分、荣耀增加
	CCorpsConfig* tpConfig = CCorpsManager::GetSingleton().GetCorpsConfig();
	if ( tpConfig == NULL )
	{
		return ;
	}

	npWinCorps->AddPvPScore( tpConfig->GetScorePerFight(), npWinCorps->GetPVPStarLevel() );
	npWinCorps->AddCorpsGlory( tpConfig->GetGloryPerFight() );

	if ( npWinCorps->CanCorpsLevelUP() )
	{
		SendNotifyCorpsCanLevelUP( npWinCorps->GetCorpsID() );
	}

	return ;
}

// ***********************************************************
//  Function:	OnMessagChangeCorpsContribute	
//  Description: 增加玩家贡献度
//  Output:			
//  Date:	09/02/2009
// **********************************************************
void CCorpservice::OnMessagChangeCorpsContribute( CMessage* pMessage )
{  
	LK_ASSERT( pMessage != NULL, return )	
	CMessageChangeCorpsContributeNotify *tpMsg = ( CMessageChangeCorpsContributeNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return ) 
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpMsg->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family","can not find family of player( roleid:%d )", tpMsg->roleid() );
		return;
	}
	
	int tOldContribute =  tpFamily->GetMemberContribute( tpMsg->roleid(), CFamily::EM_CORPSACTION );	
	if ( tpMsg->changetype() == 0 )
	{
		tpFamily->AddMemberContribute( tpMsg->roleid(), tpMsg->contribute(), CFamily::EM_CORPSACTION );
	}
	else
	{
		tpFamily->ReduceMemberContribute( tpMsg->roleid(), tpMsg->contribute(), CFamily::EM_CORPSACTION );
	}
	
	int tNewContribute = tpFamily->GetMemberContribute( tpMsg->roleid(), CFamily::EM_CORPSACTION );	
	SendNotifyCorpsContributeChangedNotify( tpFamily->GetCorpsID(), tNewContribute, tpMsg->roleid(), tNewContribute - tOldContribute );
	if ( tpMsg->changetype() == 0 )
	{
		LogOrganContribteByTask( ORGANLOGTYPE_CORPS, tpFamily->GetCorpsID(), tpMsg->roleid(), tNewContribute, tOldContribute )
	}
	else
	{
		LogOrganContribteByExchange( ORGANLOGTYPE_CORPS, tpFamily->GetCorpsID(), tpMsg->roleid(), tNewContribute, tOldContribute )	
	}
}

// ***********************************************************
//  Function:	OnMessageChangeCorpsCollectActive	
//  Description: 改变军团采集活跃度
//  Output:			
//  Date:	
// **********************************************************
void CCorpservice::OnMessageChangeCorpsCollectActive( int nServerID, CMessage* pMessage )
{	
	LK_ASSERT( pMessage != NULL, return )	
	CMessageChangeCorpsCollectActive *tpMsg = (CMessageChangeCorpsCollectActive*)pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return ) 
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpMsg->roleid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps"," can not find corps by corps of player( roleid:%d ) ", tpMsg->roleid() );
		return;
	}
	int tOldActiveValue = tpCorps->GetCorpsCollectActive();
	if ( tpMsg->changetype() == 0 )
	{
		tpCorps->SetCorpsCollectActive( tpCorps->GetCorpsCollectActive() + tpMsg->activevalue() );
	}
	else
	{
		tpCorps->SetCorpsCollectActive( tpCorps->GetCorpsCollectActive() - tpMsg->activevalue() );
	}
	if ( tpMsg->activevalue() > 0 )
	{
		SendNotifyCorpsCollectActiveChange( tpCorps->GetCorpsID(), tpMsg->roleid(), tpCorps->GetCorpsCollectActive(), tpMsg->activevalue(), tpMsg->changetype() );
	}

	LogOrganCorpsActivityByTask( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpMsg->roleid(), tpCorps->GetCorpsCollectActive(), tOldActiveValue );

}

// ***********************************************************
//  Function:	OnMessageChangeCorpsMoney	
//  Description: 改变军团金钱
//  Output:			
//  Date:	09/02/2009
// **********************************************************
void CCorpservice::OnMessageChangeCorpsMoney( CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
	CMessageChangeCorpsMoneyNotify *tpMsg =( CMessageChangeCorpsMoneyNotify * )	pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps"," can not find corps by ID:%d ", tpMsg->corpsid() );
		return;
	}
	int tOldCorpsMoney = tpCorps->GetCorpsMoney();
	tpCorps->SetCorpsMoney( tpCorps->GetCorpsMoney() + tpMsg->money() );
	if ( tpMsg->money() > 0 )
	{
		SendNotifyCorpsMoneyChanged( tpCorps->GetCorpsID(), tpCorps->GetCorpsMoney(), tpMsg->roleid(), tpMsg->money(), tpMsg->reason(), tpMsg->mutable_rolename()->c_str() );
		if ( tpCorps->CanCorpsLevelUP() )
		{
			SendNotifyCorpsCanLevelUP( tpCorps->GetCorpsID() );
		}
	}
	switch( tpMsg->reason() )
	{
		case REASON_TASK:
		{
			LogOrganMoneyByTask( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpMsg->roleid(), tpCorps->GetCorpsMoney(), tOldCorpsMoney );
			break;
		}
		case REASON_USEMONEYITEM:
		{
			LogOrganMoneyByUseFunItem( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpMsg->roleid(), tpCorps->GetCorpsMoney(), tOldCorpsMoney );
			break;
		}
		
		default:
			break;
	}
}

// ***********************************************************
//  Function:	OnMessageChangeCorpsGlory	
//  Description: 改变军团荣耀
//  Output:			
//  Date:	09/02/2009
// **********************************************************
void CCorpservice::OnMessageChangeCorpsGlory( CMessage* pMessage )
{  
	LK_ASSERT( pMessage != NULL, return )	
	CMessageChangeCorpsGloryNotify *tpMsg = ( CMessageChangeCorpsGloryNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps"," can not find corps by ID:%d  ", tpMsg->corpsid() );
		return;
	}
	
	int tOldGlory = tpCorps->GetCorpsGlory();
	int tRet = tpCorps->AddCorpsGlory( tpMsg->glory() );	
	if ( tRet == SUCCESS )
	{
		SendNotifyCorpsGlroyChangedNotify( tpMsg->corpsid(), tpCorps->GetCorpsGlory(), tpMsg->roleid(), 
			tpMsg->glory(), tpMsg->reason(), tpMsg->mutable_rolename()->c_str() );
		if ( tpMsg->glory() > 0 )
		{
			if ( tpCorps->CanCorpsLevelUP() )
			{
				SendNotifyCorpsCanLevelUP( tpCorps->GetCorpsID() );
			}
		}
		
		switch ( tpMsg->reason() )
		{
			case REASON_TASK:
			{
				LogGlroyByTask( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpMsg->roleid(), tpCorps->GetCorpsGlory(), tOldGlory, tpMsg->glory() );
				break;
			}
		}
	}
}

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageChangeCorpsBulletin( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageChangeCorpsBulletinRequest *tpMsg = ( CMessageChangeCorpsBulletinRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	LK_ASSERT( tpMsg->has_bulletin(), return )
	if ( tpMsg->mutable_bulletin()->length() >= CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES )
	{
		LOG_ERROR( "corps"," change corpsbulletin error by player( roleid:%d ),msg too long  ", tpHead->roleid() );
		return;	
	}
	
	
	int tRet = CCorpsManager::GetSingleton().OnEventChangeCoprsNotice( tpHead->roleid(), tpMsg->mutable_bulletin()->c_str() );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseChangeCorpsBulletin( nServerID, tpHead->roleid(), tRet, tpMsg->mutable_bulletin()->c_str(), tpHead->entityid() );
	if ( tRet == SUCCESS )
	{
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
		if ( tpCorps == NULL )
		{
			LOG_ERROR( "corps"," can not find corps of player( roleid:%d ) ", tpHead->roleid() );
			return;
		}
		
		int tCorpsMemberIDList[CORPS_MAXMEMBERNUM] = { 0 };
		int tNum = CORPS_MAXMEMBERNUM;
		CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tCorpsMemberIDList, tNum, 0, tpHead->roleid() );
		SendNotifyChangeCorpsBulletin( tpMsg->mutable_bulletin()->c_str(), tCorpsMemberIDList, tNum );
	}
}

// ***********************************************************
//  Function:		 OnMessageChanggeCorpsHeader
//  Description:	 处理改变军团首领的消息
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageChanggeCorpsHeader( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
	CMessageChangeCorpsHeaderRequest *tpMsg = ( CMessageChangeCorpsHeaderRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return  )	
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( pGatePlayer == NULL )
	{	
		LOG_ERROR( "corps", " can not find corps of player( roleid:%d ) ", tpHead->roleid() );
		return;
	}
	
	
	int tRet = CCorpsManager::GetSingleton().OnEventAppointCorpsHeader( tpHead->roleid(), tpMsg->memberid() );
	if ( tRet == -1 )
	{
		return;
	}
	
	CRoleGlobalInfo *tpMemberInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpMsg->memberid() );
	if ( tpMemberInfo == NULL )
	{
		LOG_ERROR( "corps"," can not find player info ( CRoleGlobalInfo ) by roleid:%d ", tpMsg->memberid() );
		return;
	}
	
	SendResponseChangeCorpsHeader( nServerID, tpHead->roleid(), tRet, tpMsg->memberid(), tpMemberInfo->RoleName(), tpHead->entityid() );  
	if ( tRet == SUCCESS )
	{
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
		if ( tpCorps == NULL )
		{
			LOG_ERROR( "corps"," can not find corps of player( roleid:%d ) ", tpHead->roleid() );
			return;
		}
		
		int tRoleIDList[CORPS_MAXMEMBERNUM] = { 0 };
		int tNum = CORPS_MAXMEMBERNUM;
		CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tRoleIDList, tNum, 0, tpHead->roleid() );				
		SendNotifyChangeCorpsHeader( pGatePlayer->mCharID, pGatePlayer->mCharName, tpMemberInfo->RoleID(), tpMemberInfo->RoleName(), tRoleIDList, tNum );
	}
	
}

// ***********************************************************
//  Function:		OnMessageCreatCorps
//  Description:	创建军团的消息请求
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageCreatCorps( int nServerID, CMessage* pMessage )
{	
	LK_ASSERT( pMessage != NULL, return )
	CMessageCreatCorpsRequest *tpMsg = ( CMessageCreatCorpsRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() ); 	
	if ( tpGatePlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find corps of player( roleid:%d ) ", tpHead->roleid() );
		return;
	}
	
	
	if ( !tpMsg->has_corpsname() )
	{
		return;
	}
	if ( tpMsg->mutable_corpsname()->length() >= MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES )
	{
		return;
	}
	int tCorpsID = 0;
	int tRet = CCorpsManager::GetSingleton().OnEventCreatCorps( tpHead->roleid(), tpMsg->mutable_corpsname()->c_str(), tCorpsID, tpMsg->corpscolor() );
	if ( tRet == -1 )
	{
		return;
	}
	if ( tRet != SUCCESS )
	{
		SendResponseCreatCorps( nServerID, tpHead->roleid(), tRet, tCorpsID, tpMsg->mutable_corpsname()->c_str(), tpHead->entityid(), tpMsg->corpscolor() );
	}	
}

// ***********************************************************
//  Function:		OnMessageDisbandCorps	
//  Description:	处理解散军团的消息
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageDisbandCorps( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
	CMessageDisbandCorpsRequest *tpMsg = ( CMessageDisbandCorpsRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	
	int tRoleIDList[CORPS_MAXMEMBERNUM] = { 0 };
	int tNum = CORPS_MAXMEMBERNUM;
	int tRet = CCorpsManager::GetSingleton().OnEventDisbandCorps( tpHead->roleid(), tRoleIDList, tNum );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseDisbandCorps( nServerID, tpHead->roleid(), tRet, tpHead->entityid( ) );  
	if ( tRet == SUCCESS )
	{
		// SendNotifyDisbandCorps( tRoleIDList, tNum );
	} 			
}

// ***********************************************************
//  Function:		OnMessageCancelCorpsDisband	
//  Description:	处理取消军团解散的消息
//  Output:			
//  Date:	06/18/2009
// **********************************************************
void CCorpservice::OnMessageCancelCorpsDisband( int nServerID, CMessage* pMessage )
{  
	LK_ASSERT( pMessage != NULL, return  )	
	CMessageCancelCorpsDisbandRequest *tpMsg = ( CMessageCancelCorpsDisbandRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return  )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	
	int tRet = CCorpsManager::GetSingleton().OnEventCancelCorpsDisband( tpHead->roleid() );
	if ( tRet == -1 )
	{										  
		return;
	}		   
	SendResponseCancelCorpsDisband( nServerID, tpHead->roleid(), tRet, tpHead->entityid() );  	
}

// ***********************************************************
//  Function:		OnMessageClearCorpsApply
//  Description:	清除军团的申请列表信息
//  Output:
//  Date:	06/22/2009
// **********************************************************
void CCorpservice::OnMessageClearCorpsApply( int nServerID, CMessage* pMessage )
{	
	LK_ASSERT( pMessage != NULL, return )
	CMessageClearCorpsApplyResponse *tpMsg = ( CMessageClearCorpsApplyResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();							 
																				 	
	int tRet = CCorpsManager::GetSingleton().OnEventClearCorpsApply( tpHead->roleid() );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseClearCorpsApply( nServerID, tRet, tpHead->roleid(), tpHead->entityid() );
}

// ***********************************************************
//  Function:		OnMessageGetCorpsList
//  Description:	获取军团信息列表的请求
//  Output:
//  Date:	08/05/2009
// **********************************************************
void CCorpservice::OnMessageGetCorpsList( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
	CMessageGetCorpsListRequest *tpMsg = ( CMessageGetCorpsListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )	
	CorpsHead *tpHead = tpMsg->mutable_corphead();		
	
	CGatePlayer *tpGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() ); 	
	if ( tpGatePlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find player info(CGatePlayer) of roleid:%d ", tpHead->roleid() );
		return;
	}
	
	int tTotalPageNum = 0;		
	CorpsInfoList tCorpsList;
	CCorpsManager::GetSingleton().OnEventListCorps( tpHead->roleid(), tpGatePlayer->mCountryID, tCorpsList, tpMsg->curpage(), tTotalPageNum );			
	if ( tCorpsList.corpslist_size() > 0 )
	{
		SendResponseGetCorpsList( nServerID, tpHead->roleid(), tpHead->entityid(), tCorpsList, tpMsg->curpage(), tTotalPageNum );
	}
	
} 

// ***********************************************************
//  Function:		OnMessageRefreshCorpsInfo
//  Description:	刷新军团信息的请求
//  Output:
//  Date:	06/23/2009
// **********************************************************
void CCorpservice::OnMessageRefreshCorpsInfo( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )	
	CMessageRefreshCorpsInfoRequest *tpMsg = ( CMessageRefreshCorpsInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	
	PBCorpsMemberList tCorpsMemberList;
	int tRet = CCorpsManager::GetSingleton().OnEventGetCorpsMemberInfo( tpHead->roleid(), tCorpsMemberList );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseRefreshCorpsInfo( nServerID, tpHead->roleid(), tpHead->entityid(), tCorpsMemberList );
}

// ***********************************************************
//  Function:		设置军团成员职位的消息请求
//  Description:	OnMessageSetCorpsPost
//  Output:
//  Date:	08/25/2009
// ********************************************************** 
void CCorpservice::OnMessageSetCorpsPost( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )	
	CMessageSetCorpsPostRequest *tpMsg = ( CMessageSetCorpsPostRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	
	int tOldPost = 0;
	int tRet = CCorpsManager::GetSingleton().OnEventSetCorpsMemberPost( tpHead->roleid(), tpMsg->memberid(), tpMsg->post(), tOldPost );
	if ( tRet == -1 )
	{
		return;
	} 
	
	SendResponseSetCorpMemberPost( nServerID, tpHead->roleid(), tRet, tpMsg->memberid(), tpMsg->post(), tpHead->entityid() );
		
	// 发送职位设置的通知
	if ( tRet == SUCCESS )
	{			
		int tRoleIDList[CORPS_MAXMEMBERNUM] = { 0 };
		int tNum = CORPS_MAXMEMBERNUM;
		
		CGatePlayer *tpManager = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
		LK_ASSERT( tpManager != NULL, return ) 
		CRoleGlobalInfo *tpMember =	CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpMsg->memberid() );
		if ( tpMember == NULL )
		{						
			LOG_ERROR( "corps"," can not find player info( CRoleGlobalInfo ) ( roleid:%d ) ", tpMsg->memberid() );
			return;
		}
		
		
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
		if ( tpCorps == NULL )
		{
			LOG_ERROR( "corps", "can not find corps of player (roleid:%d) ", tpHead->roleid() );
			return;
		}
		
		CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tRoleIDList, tNum, 0, 0 );
		if ( tNum > 0 )
		{
			SendNotifySetCorpsPost( tRoleIDList, tNum, tpHead->roleid(), tpManager->mCharName, tpMember->RoleID(), tpMember->RoleName(), tOldPost, tpMsg->post() );
		}
	}		
}

// ***********************************************************
//  Function:		升级军团的消息请求
//  Description:	OnMessageLevelUPCorps
//  Output:
//  Date:	08/25/2009
// ********************************************************** 
void CCorpservice::OnMessageLevelUPCorps( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )	
	CMessageLevelUPCorpsRequest *tpMsg = ( CMessageLevelUPCorpsRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( tpGatePlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find corps of player( roleid:%d ) ", tpHead->roleid() );
		return;
	}	
	
	int tLevel = 0;
	int tMoney = 0;
	int tRet = CCorpsManager::GetSingleton().OnEventLevelUPCorps( tpHead->roleid(), tLevel, tMoney );
	
	if ( tRet == -1 )
	{
		return;
	}
	
	SendResponseLevelUPCorps( nServerID, tRet, tLevel, tpHead->roleid(), tpHead->entityid() );
	
	if ( tRet == SUCCESS )
	{						
		int tRoleIDList[CORPS_MAXMEMBERNUM] = { 0 };
		int tNum = CORPS_MAXMEMBERNUM;
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
		if ( tpCorps == NULL )
		{
			LOG_ERROR( "corps", " can not find corps of plalyer( roleid:%d ) ", tpHead->roleid() );
			return;
		}
		
		CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tRoleIDList, tNum, 0 );
		if ( tNum > 0 )
		{
			int tMaxGeneralNum = 0;
			CCorpsManager::GetSingleton().GetCorpsConfig()->GetGeneralNumLimitByLevel( tpCorps->GetCorpsLevel(), tMaxGeneralNum );

			int tMaxCounsellorNum = 0;
			CCorpsManager::GetSingleton().GetCorpsConfig()->GetCounSellorNumLimitByLevel( tpCorps->GetCorpsLevel(), tMaxCounsellorNum );
			
			int  tNum = 0;
			CCorpsManager::GetSingleton().GetCorpsConfig()->GetFamilyNumLimitByLevel( tpCorps->GetCorpsLevel(), tNum );
			
			int tLevelMoney = 0;
			int tLevelGlory	= 0;
			tpCorps->GetLevelCondition( tLevelMoney, tLevelGlory );
			SendNotifyCorpsLevelUP( tRoleIDList, tNum, tLevel, tpCorps->GetCorpsMoney(), tpCorps->GetCorpsGlory(), tNum, tMaxGeneralNum, tMaxCounsellorNum, tLevelMoney, tLevelGlory );
		}	  		
		
		if ( tMoney > 0 )
		{
			SendNotifyCorpsMoneyChanged( tpCorps->GetCorpsID(), tpCorps->GetCorpsMoney(), tpHead->roleid(), -1*tMoney, REASON_LEVELUP, tpGatePlayer->CharName() );
			if ( tpCorps->CanCorpsLevelUP() )
			{
				SendNotifyCorpsCanLevelUP( tpCorps->GetCorpsID() );
			}
		}	
		LogOrganLevelUP( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpHead->roleid(), tpCorps->GetCorpsLevel(), tpCorps->GetCorpsLevel() - 1 );
		LogOrganMoneyByLevelUP( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpHead->roleid(), tpCorps->GetCorpsMoney(), tpCorps->GetCorpsMoney() + tMoney );
	}
}

// ***********************************************************
//  Function:		贡献军团金钱的消息请求
//  Description:	OnMessageContributeCorpsMoney
//  Output:
//  Date:	08/25/2009
// ********************************************************** 
void CCorpservice::OnMessageContributeCorpsMoney( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )	
	CMessageContributeCorpsMoneyRequest *tpMsg = ( CMessageContributeCorpsMoneyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();	
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	LK_ASSERT( tpPlayer != NULL, return )
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps"," can not find corps of player( roleid:%d ) ", tpHead->roleid() );
		return;	
	}
	
	int tOldContribute = tpCorps->GetMemberContribute( tpHead->roleid() );
	int tOldCorpsMoney = tpCorps->GetCorpsMoney();
	int tRet = CCorpsManager::GetSingleton().OnEventContributeMoney( tpHead->roleid(), tpMsg->money() );
	if ( tRet == -1 )
	{
		return;
	}
	
	SendResponseContributeCorpsMoney( nServerID, tRet, tpMsg->money(), tpHead->roleid(), tpHead->entityid() );
	if ( tRet == SUCCESS )
	{	
		// 记录家族金钱改变
		LOG_NOTICE( "corps", "[%s,%d] player contribute corpsmoney( roleid:%d, corpsid:%d, money:%d ) ", __FUNCTION__,__LINE__, tpHead->roleid(), tpCorps->GetCorpsID(), tpMsg->money() );		
		
		int tNewContribute =  tpCorps->GetMemberContribute( tpHead->roleid() );		
		SendNotifyCorpsMoneyChanged( tpCorps->GetCorpsID(), tpCorps->GetCorpsMoney(), tpHead->roleid(), tpMsg->money(), REASON_CONTRIBUTE, tpPlayer->CharName() ); 
		
		LogOrganContributeMoney( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpHead->roleid(), tpCorps->GetCorpsMoney(), tOldCorpsMoney )
		LogOrganContributeByContribute( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpHead->roleid(), tNewContribute, tOldContribute )
		
		// 发送军团贡献度改变的通知
		SendNotifyCorpsContributeChangedNotify( tpCorps->GetCorpsID(), tNewContribute, tpHead->roleid(), tNewContribute - tOldContribute );
		
		// 检查军团是否可以升级
		if ( tpCorps->CanCorpsLevelUP() )
		{
			SendNotifyCorpsCanLevelUP( tpCorps->GetCorpsID() );
		}
	}
}

// ***********************************************************
//  Function:		OnMessageFamilyContributeMoney
//  Description:	家族给军团贡献金钱的消息请求
//  Output:
//  Date:	08/25/2009
// ********************************************************** 
void CCorpservice::OnMessageFamilyContributeMoney( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return )	
	LK_ASSERT( pMessage != NULL, return )	
	CMessageFamilyContributeMoneyRequest *tpMsg = ( CMessageFamilyContributeMoneyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	
	int tRet = 	CCorpsManager::GetSingleton().OnEventFamilyContributeMoney( tpHead->roleid(), tpMsg->money() );
	if ( tRet == -1 )
	{	  
		return;
	}
	
	SendResponseFamilyContributeMoney( nServerID, tRet, tpMsg->money(), tpHead->roleid(), tpHead->entityid() );
}

// ***********************************************************
//  Function:		OnMessageSetMemberName
//  Description:	设置军团成员称号的消息请求
//  Output:
//  Date:	08/25/2009
// ********************************************************** 
void CCorpservice::OnMessageSetMemberName( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )	
	CMessageSetMemberNameRequest *tpMsg = ( CMessageSetMemberNameRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpManager = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	LK_ASSERT( tpManager != NULL, return )
	
	int tRet = CCorpsManager::GetSingleton().OnEventSetCorpsMemberName( tpHead->roleid(), tpMsg->memberid(), tpMsg->mutable_name()->c_str() );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseSetMemberName( nServerID, tpMsg->memberid(), tpMsg->mutable_name()->c_str(), tpHead->roleid(), tpHead->entityid(), tRet );			
	
	if ( tpHead->roleid() == tpMsg->memberid() )
	{
		return;
	}
		
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->memberid() );
	if ( tpPlayer != NULL && tRet == SUCCESS  )
	{
		SendNotifyCorpsTitleChanged( tpPlayer, tpHead->roleid(), tpManager->CharName(), tpMsg->mutable_name()->c_str() );
	}	
}


// ***********************************************************
//  Function:		OnChatMsg
//  Description:	聊天消息
//  Output:
//  Date:	06/25/2009
// **********************************************************
void CCorpservice::OnChatMsg( int nServerID, CMessage* pMessage )
{	
	LK_ASSERT( pMessage != NULL, return )	
	CMessageChatSendCallBack *tpMsg = ( CMessageChatSendCallBack * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	int tRoleID = tpMsg->mutable_router()->srccharid();		
	switch ( tpMsg->channel() )
	{
		case CHANNEL_FAMILY:
		{
			CServiceFamily::GetSingleton().SendFamilyMsg( tRoleID, pMessage );
			break;	
		}		
		case CHANNEL_CORPS:
		{
		   SendCorpsMsg( tRoleID, pMessage );
		   break;
		}
		default:
		CGateLogic::GetSingleton().OnRouterMessage( nServerID, pMessage );
	}
}

// ***********************************************************
//  Function:		  OnMessageGetCorpsApplyList
//  Description:	  获取军团的申请列表
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageGetCorpsApplyList( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return )	
	CMessageGetCorpsApplyListRequest *tpMsg = ( CMessageGetCorpsApplyListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	
	PBCorpsApplyList tCoprsApply;	
	int tRet = CCorpsManager::GetSingleton().OnEventListCorpsApply( tpHead->roleid(), tCoprsApply );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseGetCorpsApplyList( nServerID, tpHead->roleid(), tRet, tCoprsApply, tpHead->entityid() );			
}

// ***********************************************************
//  Function:		OnMessageInviteCorpsMember
//  Description:	邀请家族加入军团
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageInviteCorpsMember( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
	CMessageInviteCorpsMemberRequest *tpMsg = ( CMessageInviteCorpsMemberRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	
	CGatePlayer *tInvitingPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( tInvitingPlayer == NULL )
	{
		LOG_ERROR( "corps", " can not find corps info of player (roleid:%d) ", tpHead->roleid() );
		return;
	}
	
	
	CFamily *tpFamily = NULL;
	if ( tpMsg->has_familyid() )
	{
		tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );		
	}
	else if( tpMsg->has_familyname() )
	{
		LK_ASSERT( tpMsg->mutable_familyname()->length() < MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES, return )		
		tpFamily = CFamilyManager::GetSingleton().GetFamilyByName( tpMsg->familyname().c_str() );
	}
	else
	{
		tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpMsg->memberid() );
	}	
	if ( tpFamily == NULL )
	{
		SendResponseInviteCorpsMember( nServerID, tpHead->roleid(), ERROR_CORPS_INVITE_FAMILYNOTEXIST, NULL, tpHead->entityid() );
		return;
	}
	
	int tRet = CCorpsManager::GetSingleton().OnEventInviteFamily( tpHead->roleid(), tpFamily );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseInviteCorpsMember( nServerID, tpHead->roleid(), tRet, tpFamily->GetName(), tpHead->entityid() );
	
	if ( tRet == SUCCESS )
	{
		CGatePlayer *tpInvitedPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpFamily->GetRooter() );
		if ( tpInvitedPlayer != NULL )
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
			LK_ASSERT( tpCorps != NULL, return )
			PBCorpsBasicInfo tCorpsInfo;
			tpCorps->GetCorpsBasicInfo( tCorpsInfo );
			SendNotifyCorpsInvite( tpInvitedPlayer, tpInvitedPlayer->mCharID, tCorpsInfo, tInvitingPlayer->mCharID, tInvitingPlayer->mCharName );
		}		
	}		
}	

// ***********************************************************
//  Function:		OnMessageGetCorpsFamiyInfo
//  Description:	获取军团的家族信息
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageGetCorpsFamiyInfo( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageGetCorpsFamilyInfoRequest *tpMsg = ( CMessageGetCorpsFamilyInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead(); 
	PBCorpsMemberList tpbCorpsMemberList;
	
	int tRet = CCorpsManager::GetSingleton().OnEventGetCorpsMemberInfo( tpHead->roleid(), tpbCorpsMemberList );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseGetCorpsFamilyInfo( nServerID, tpHead->roleid(), tpbCorpsMemberList, tpHead->entityid() );
}

// ***********************************************************
//  Function:	OnMessageGetCorpsPropertyInfo	
//  Description: 获取军团属性
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageGetCorpsPropertyInfo( int nServerID, CMessage* pMessage )
{  
	LK_ASSERT( pMessage != NULL, return )
	CMessageGetCorpsPropertyRequest *tpMsg = ( CMessageGetCorpsPropertyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CorpsProperty tCorpsProperty;
	int tRet = CCorpsManager::GetSingleton().OnEventGetCorpsProperty( tpHead->roleid(), tCorpsProperty );
	if( tRet == -1 )
	{
		return;
	}
	SendResponseGetCorpsPropertyInfo( nServerID, tpHead->roleid(), tCorpsProperty, tpHead->entityid() );
}

// ***********************************************************
//  Function:		 OnMessageKickFamily
//  Description:	 处理家族被踢出军团的消息请求
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageKickFamily( int nServerID, CMessage* pMessage )
{  
	LK_ASSERT( pMessage != NULL, return )
	CMessageKickFamilyRequest *tpMsg = ( CMessageKickFamilyRequest *)pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	int tRet = CCorpsManager::GetSingleton().OnEventKickFamilyOfCorps( tpHead->roleid(), tpMsg->familyid() );
	if ( tRet == -1 )
	{
		return;
	}
	const char *tpFamilyName = NULL;
	CFamily *tpKickedFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
	if ( tpKickedFamily != NULL )
	{
		tpFamilyName = tpKickedFamily->GetName();
	}
	SendResponseKickFamily( nServerID, tpHead->roleid(), tRet, tpMsg->familyid(), tpFamilyName, tpHead->entityid() );

	if ( tRet == SUCCESS )
	{
		// 发送家族被踢掉的消息通知
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
		LK_ASSERT( tpCorps != NULL, return )
		CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
		LK_ASSERT( tpFamily != NULL, return )
		int tRoleIDList[CORPS_MAXMEMBERNUM] = { 0 };
		int tNum = CORPS_MAXMEMBERNUM;
		CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tRoleIDList, tNum, 0, tpHead->roleid( ) );
		if ( tNum < CORPS_MAXMEMBERNUM )
		{
			int tFamilyMemberNum =  CORPS_MAXMEMBERNUM - tNum;
			tpFamily->GetFamilyMemberIDList( tRoleIDList + tNum, tFamilyMemberNum );
			tNum += tFamilyMemberNum;
		}		
		SendNotifyFamilyKicked( tpFamily->GetID(), tpFamily->GetName(), tRoleIDList, tNum );
		LogMemberLeave( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpHead->roleid(), tpFamily->GetID(), 0 )
	}
}

// ***********************************************************
//  Function:		OnMessageLeaveCorps
//  Description:	离开军团
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageLeaveCorps( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
	CMessageFamilyLeaveCorpsRequest *tpMsg = ( CMessageFamilyLeaveCorpsRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	
	int tRoleIDList[CORPS_MAXMEMBERNUM] = { 0 };
	int tNum = CORPS_MAXMEMBERNUM;
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps"," can not find corps of player(roleid:%d) ", tpHead->roleid() );
		return;
	}
	
	CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tRoleIDList, tNum, 0, tpHead->roleid() ); 	
	
	int tRet = CCorpsManager::GetSingleton().OnEventLeaveCorps( tpHead->roleid() );
	if ( tRet != SUCCESS )
	{
		return;
	}
	SendResponseLeaveCorps( nServerID, tpHead->roleid(), tRet, tpHead->entityid() );		
	if ( tRet == SUCCESS )
	{			
		CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
		if ( tpFamily == NULL )
		{						
			LOG_ERROR( "corps"," can not find family of player( roleid:%d ) ", tpHead->roleid() );
			return;
		}
		LogMemberLeave( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpHead->roleid(), tpFamily->GetID(), 1 )
		SendNotifyFamilyLeaved( tpFamily->GetID(), tpFamily->GetName(), tRoleIDList, tNum );
	}
}

// ***********************************************************
//  Function:		OnMessageReplyCorpsInvite
//  Description:	答复军团邀请
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageReplyCorpsInvite( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
	CMessageReplyCorpsInviteRequest *tpMsg = ( CMessageReplyCorpsInviteRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead( );	 
	CGatePlayer *tpReplyingPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( tpReplyingPlayer == NULL )
	{								
		LOG_ERROR( "corps"," can not find corps of player( roleid::%d ) ", tpHead->roleid() );
		return;
	}
	
	CFamily *pInvitedFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if( pInvitedFamily == NULL )
	{
		LOG_ERROR( "corps"," can not find family of player( roleid:%d ) ", tpHead->roleid() );	
		return;	
	}  
	
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid() );
	if ( tpCorps == NULL )
	{
		// 发送军团不存在的回复信息
		SendResponseReplyCorpsInvite( nServerID, tpHead->roleid(), ERROR_CORPS_REPLYINVITE_NOTEXIST, tpMsg->result(), tpMsg->corpsid(), NULL, tpHead->entityid() );
		return;
	}
	
	int tRet = CCorpsManager::GetSingleton().OnEventReplyCorpsInvite( tpHead->roleid(), tpCorps, tpMsg->result() );
	if ( tRet == -1 )
	{
		return;
	}
	
	int tUnionID = 0;
	const char *tpUnionName = NULL;
	if ( tpCorps->GetUnionID() != 0 )
	{
		tUnionID = tpCorps->GetUnionID();
		CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tUnionID );
		if ( tpUnion != NULL )
		{
			tpUnionName = tpUnion->GetUnionName();
		}
	}			
	
	// 发送答复结果 
	SendResponseReplyCorpsInvite( nServerID, tpHead->roleid(), tRet, tpMsg->result(), tpCorps->GetCorpsID(), tpCorps->GetCorpsName(), tpHead->entityid(), tUnionID, tpUnionName );		
	
	// 给邀请人发送邀请结果
	CGatePlayer *tpInvitingPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->invitingid() );
	if ( tpInvitingPlayer != NULL && tpMsg->result() != FAMILY_REPLY_IGNORE )
	{
		int  tInviteResult = tRet == 0 ? tpMsg->result():tRet;
		
		const char *pFamilyName = NULL;
		if ( pInvitedFamily != NULL )
		{
			pFamilyName = pInvitedFamily->GetName();
		}
		SendNotifyCorpsInviteResult( tpInvitingPlayer, tpInvitingPlayer->mCharID, tInviteResult, pFamilyName );
	}
	
	// 同意加入军团并且加入成功
	if ( tRet == SUCCESS && tpMsg->result() == 1 )
	{
		// 给军团的所有成员发送有新家族加入军团的消息通知
		int tCorpMemberIDList[CORPS_MAXMEMBERNUM] = { 0 };
		int tNum = CORPS_MAXMEMBERNUM;
		int tExceptID = 0;
		if ( pInvitedFamily != NULL )
		{
			tExceptID = pInvitedFamily->GetID();
		}
		CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tCorpMemberIDList, tNum, tExceptID );
		
		PBCorpsMember tCorpsMember;
		if ( pInvitedFamily != NULL )
		{							
			pInvitedFamily->GetFamilyBasicInfo( *tCorpsMember.mutable_familyproperty() );
			pInvitedFamily->GetFamilyMemberInfoList( *tCorpsMember.mutable_familymemberlist(), true );
		}
		SendNotifyFamilyJoinCorps( tCorpsMember, tCorpMemberIDList, tNum );
		
		// 给加入军团的家族成员发送加入军团的消息通知
		if ( pInvitedFamily != NULL )
		{
			int tFamilyRoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 };
			int tMemberNum = MAX_FAMILYMEMBER_NUM;
			pInvitedFamily->GetFamilyMemberIDList( tFamilyRoleIDList, tMemberNum );
			if ( tMemberNum > 0 )
			{
				int tUnionID = 0;
				const char *tpUnionName = NULL;
				if ( tpCorps->GetUnionID() != 0 )
				{
					tUnionID = tpCorps->GetUnionID();
					CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tUnionID );
					if ( tpUnion != NULL )
					{
						tpUnionName = tpUnion->GetUnionName();
					}
				}			
							
				CServiceFamily::GetSingleton().SendNotifyJoinCorps( tpCorps->GetCorpsID(), tpCorps->GetCorpsName(), tFamilyRoleIDList, tMemberNum, JOINCORPS_OTHERFAMILYCORPS, tUnionID, tpUnionName );
				if ( tpCorps->IsOwner( ) )
				{
					int tCountryID = CWorldManager::GetSingleton().GetCountryIDByMap( tpCorps->GetBattleCity() );
					if ( tCountryID != COUNTRY_NONE )
					{
						CServiceFamily::GetSingleton().ChangeFamilyCountry( tFamilyRoleIDList, tMemberNum, tCountryID );
					}
				}				
			}
		}
		
		LogMemberJoin( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpMsg->invitingid(), pInvitedFamily->GetID(), 0, tpCorps->GetFamilyNum() )
	}
}

// ***********************************************************
//  Function:		OnMessageReplyMemberApply	
//  Description:	答复家族的申请
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::OnMessageReplyMemberApply( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )		
	CMessageReplyCorpsMemberApplyRequest *tpMsg = ( CMessageReplyCorpsMemberApplyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();	
	CGatePlayer *tpReplyingPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( tpReplyingPlayer == NULL )
	{
		LOG_ERROR( "corps", " can not find player info( tpReplyingPlayer ) ( roleid:%d ) ", tpHead->roleid() );
		return;
	}
	
	
	int tApplyingID = 0;
	int tRet = CCorpsManager::GetSingleton().OnEventReplyApply( tpHead->roleid(), tpMsg->familyid(), tpMsg->result(), tApplyingID );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseReplyCorpsMemberApply( nServerID, tpHead->roleid(), tRet, tpMsg->result(), tpMsg->familyid(), tpHead->entityid() );
	
	// 如果申请人在线，发结果给申请人
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps"," can not find corps of player( roleid:%d ) ", tpHead->roleid() );
		return;
	}
	
	CGatePlayer *tpApplyingPlayer = CPlayerManager::GetSingleton().ScenePlayer( tApplyingID );
	if ( tpApplyingPlayer != NULL && tpMsg->result() != 2 )
	{	
		int tResult = tRet == SUCCESS? tpMsg->result():tRet; 	
		int tUnionID = 0;
		const char *tpUnionName = NULL;
		if ( tpCorps->GetUnionID() != 0 )
		{
			tUnionID = tpCorps->GetUnionID();
			CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tUnionID );
			if ( tpUnion != NULL )
			{
				tpUnionName = tpUnion->GetUnionName();
			}
		}						
		SendNotifyApplyCorpsResult( tpApplyingPlayer, tpApplyingPlayer->mCharID, tResult, tpCorps->GetCorpsID(), tpCorps->GetCorpsName(), tUnionID, tpUnionName );
	}
	
	// 如果加入成功，发送军团通知
	if ( tRet == SUCCESS && tpMsg->result() == 1 )
	{	  
		// 给军团成员发送加入军团的消息通知
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
		if ( pFamily == NULL )
		{
			LOG_ERROR( "corps"," can not find corps by ID:%d ", tpMsg->familyid() );
			return;
		}
		
		PBCorpsMember tCorpsMember;
		pFamily->GetFamilyBasicInfo( *tCorpsMember.mutable_familyproperty() );
		pFamily->GetFamilyMemberInfoList( *tCorpsMember.mutable_familymemberlist(), true );
		
		int tCorpsMemberIDList[CORPS_MAXMEMBERNUM] = { 0 };
		int tNum = CORPS_MAXMEMBERNUM;
		CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tCorpsMemberIDList, tNum, pFamily->GetID() );
		SendNotifyFamilyJoinCorps( tCorpsMember, tCorpsMemberIDList, tNum );

		// 给加入军团的家族成员发送加入军团的消息通知
		if ( pFamily != NULL )
		{
			int tFamilyRoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 };
			int tMemberNum = MAX_FAMILYMEMBER_NUM;
			pFamily->GetFamilyMemberIDList( tFamilyRoleIDList, tMemberNum, pFamily->GetRooter() );
			if ( tMemberNum >= 0 )
			{
				int tUnionID = 0;
				const char *tpUnionName = NULL;
				if ( tpCorps->GetUnionID() != 0 )
				{
					tUnionID = tpCorps->GetUnionID();
					CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tUnionID );
					if ( tpUnion != NULL )
					{
						tpUnionName = tpUnion->GetUnionName();
					}
				}			
				CServiceFamily::GetSingleton().SendNotifyJoinCorps( tpCorps->GetCorpsID(), tpCorps->GetCorpsName(), tFamilyRoleIDList, tMemberNum, JOINCORPS_OTHERFAMILYCORPS, tUnionID, tpUnionName );
				if ( tpCorps->IsOwner( ) )
				{
					tFamilyRoleIDList[ tMemberNum++ ] = pFamily->GetRooter();
					int tCountryID = CWorldManager::GetSingleton().GetCountryIDByMap( tpCorps->GetBattleCity() );
					if ( tCountryID != COUNTRY_NONE )
					{
						CServiceFamily::GetSingleton().ChangeFamilyCountry( tFamilyRoleIDList, tMemberNum, tCountryID );
					}
				}
			}
		}		
		LogMemberJoin( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpHead->roleid(), tpMsg->familyid(), 1, tpCorps->GetFamilyNum() )
	}
}

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendNotifyApplyCorpsResult( CGatePlayer *pPlayer, int nRoleID, int nRet, int nCorpsID, const char *pCorpsName, int nUnionID /* = 0 */, const char *pUnionName /* = NULL */ )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageApplyCorpsResultNotify tMsgApplyCorps;
	
	tMsgApplyCorps.mutable_corphead()->set_roleid( nRoleID );
	tMsgApplyCorps.set_resultcode( nRet );
	tMsgApplyCorps.set_corpsid( nCorpsID );
	if ( pCorpsName != NULL )
	{
		tMsgApplyCorps.set_corpsname( pCorpsName );
	}
	tMsgApplyCorps.set_unionid( nUnionID );
	if ( pUnionName != NULL )
	{
		tMsgApplyCorps.set_unionname( pUnionName );
	}
	
	tMsgHead.set_msgpara( (int)&tMsgApplyCorps );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_APPLYCORPSRESULT );
	
	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		 SendNotifyChangeCorpsBulletin
//  Description:	 发送军团公告改变的通知
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendNotifyChangeCorpsBulletin( const char *pBulletin, int *pRoleIDlist, int tNum )
{
	LK_ASSERT( pBulletin != NULL, return )
	CMessage tMsgHead;
	CMessageChangeCorpsBulletinNotify tMsgChangeBulletin;
	
	tMsgChangeBulletin.set_bulletin( pBulletin );
	tMsgChangeBulletin.mutable_corphead()->set_roleid( 0 );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGECORPSBULLETIN );
	tMsgHead.set_msgpara( (int)&tMsgChangeBulletin );
	
	SendCorpsNotifyMsg( &tMsgHead, pRoleIDlist, tNum );
}

// ***********************************************************
//  Function:		SendNotifyChangeCorpsHeader
//  Description:	发送军团首领改变的消息
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendNotifyChangeCorpsHeader( int nHeaderID, const char *pHeaderName, int nMemberID, const char *pMemberName, int *pRoleIDList, int nNum )
{  
	CMessage tMsgHead;
	CMessageChangeCorpsHeaderNotify tMsgChangeCorpsHeader;
	
	tMsgChangeCorpsHeader.set_headerroleid( nHeaderID );
	if ( pHeaderName != NULL )
	{
		tMsgChangeCorpsHeader.set_headerrolename( pHeaderName );
	}	
	tMsgChangeCorpsHeader.set_memberroleid( nMemberID );
	if ( pMemberName != NULL )
	{
		tMsgChangeCorpsHeader.set_memberrolename( pMemberName );
	}
	tMsgChangeCorpsHeader.mutable_corphead()->set_roleid( 0 );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGECORPSHEADER );
	tMsgHead.set_msgpara( (int)&tMsgChangeCorpsHeader );
	SendCorpsNotifyMsg( &tMsgHead, pRoleIDList, nNum );
}

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendNotifyCorpsInvite( CGatePlayer *pPlayer, int nRoleID, PBCorpsBasicInfo &pbCorpsInfo, int nInvitingID, const char * pInvitingName )
{	
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageCorpsInviteNotify tMsgCorpsInvite;
	
	tMsgCorpsInvite.mutable_corphead()->set_roleid( nRoleID );
	tMsgCorpsInvite.mutable_corpsinfo()->CopyFrom( pbCorpsInfo );
	tMsgCorpsInvite.set_invitingid( nInvitingID );
	if ( pInvitingName != NULL )
	{
		tMsgCorpsInvite.set_invitingname( pInvitingName );
	}
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSINVITE );
	tMsgHead.set_msgpara( (int)&tMsgCorpsInvite );
	
	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyCorpsInviteResult
//  Description:	发送邀请结果
//  Output:			
//  Date:	06/12/2009
// ********************************************************** 
void CCorpservice::SendNotifyCorpsInviteResult( CGatePlayer *pPlayer, int nRoleID, int nResult, const char *pFamilyName )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageCorpsInviteResultNotify tMsgInviteResult;
	
	tMsgInviteResult.mutable_corphead()->set_roleid( nRoleID );
	tMsgInviteResult.set_result( nResult );
	if ( pFamilyName != NULL )
	{
		tMsgInviteResult.set_familyname( pFamilyName );
	}
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSINVITERESULT );		
	tMsgHead.set_msgpara( (int)&tMsgInviteResult );
	
	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyCorpsMemberApply
//  Description:	发送有家族加入军团的通知
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendNotifyCorpsMemberApply( CGatePlayer *pPlayer, int nRoleID, FamilyData &pbFamilyData )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageCorpsMemberApplyNotify tMsgMemberApply;
	
	tMsgMemberApply.mutable_corphead()->set_roleid( nRoleID );
	tMsgMemberApply.mutable_familyinfo()->CopyFrom( pbFamilyData );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSMEMBERAPPLY );
	tMsgHead.set_msgpara( (int)&tMsgMemberApply );
	
	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead ); 
}

// ***********************************************************
//  Function:		 SendNotifyDisbandCorps
//  Description:	 发送解散军团的消息通知
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendNotifyDisbandCorps( int *pRoleIDList, int tNum )
{	
	CMessage tMsgHead;
	CMessageDisbandCorpsNotify MsgDisBandCoprs;
	
	MsgDisBandCoprs.mutable_corphead()->set_roleid( 0 );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_DISBANDCORPS );
	tMsgHead.set_msgpara( (int)&MsgDisBandCoprs );
	
	SendCorpsNotifyMsg( &tMsgHead, pRoleIDList, tNum );
}

// ***********************************************************
//  Function:		 SendResponseCancelCorpsDisband
//  Description:	 发送解散军团的消息回复
//  Output:			
//  Date:	06/18/2009
// **********************************************************
void CCorpservice::SendResponseCancelCorpsDisband( int nServerID, int nRoleID, int nRet, int nEntityID /* = 0 */ )
{
	CMessage tMsgHead;
	CMessageCancelCorpsDisbandResponse tMsgCancelCorpsDisband;
	
	tMsgCancelCorpsDisband.mutable_corphead()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgCancelCorpsDisband.mutable_corphead()->set_entityid( nEntityID );
	}	
	tMsgCancelCorpsDisband.set_errcode( nRet );
		
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CANCELCORPSDISBAND );
	tMsgHead.set_msgpara( (int)&tMsgCancelCorpsDisband );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		   SendNotifyFamilyJoinCorps
//  Description:	   家族加入军团的通知
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendNotifyFamilyJoinCorps( PBCorpsMember &pbCorpsMember, int *pRoleIDList, int vRoleNum )
{
	CMessage tMsgHead;
	CMessageFamilyJoinCorpsNotify tMsgJoinFamily;
	
	tMsgJoinFamily.mutable_familyinfo()->CopyFrom( pbCorpsMember ); 	
	tMsgJoinFamily.mutable_corphead()->set_roleid( 0 );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYJOINCORPS );	
	tMsgHead.set_msgpara( (int)&tMsgJoinFamily );
	SendCorpsNotifyMsg( &tMsgHead, pRoleIDList, vRoleNum );
}

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendNotifyFamilyKicked( int nFamilyID, const char * pFamilyName, int *pRoleIDList, int tNum )
{	
	CMessage tMsgHead;
	CMessageKickFamilyNotify tMsgKickFamily;

	tMsgKickFamily.set_familyid( nFamilyID );
	if( pFamilyName != NULL )
	{		
		tMsgKickFamily.set_familyname( pFamilyName );
	}
	tMsgKickFamily.mutable_corphead()->set_roleid( 0 );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYKICKED );
	tMsgHead.set_msgpara( (int)&tMsgKickFamily );
	
	SendCorpsNotifyMsg( &tMsgHead, pRoleIDList, tNum );
}					    

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendNotifyFamilyLeaved( int nFamilyID, const char *pFamilyName, int *pRoleIDList, int nRoleNum )
{ 
	CMessage tMsgHead;
	CMessageFamilyLeaveCorpsNotify tMsgFamilyLeaved;	
	
	tMsgFamilyLeaved.set_familyid( nFamilyID );
	if ( pFamilyName != NULL )
	{
		tMsgFamilyLeaved.set_familyname( pFamilyName );
	}
	tMsgFamilyLeaved.mutable_corphead()->set_roleid( 0 );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYLEAVED );
	tMsgHead.set_msgpara( (int)&tMsgFamilyLeaved );
		
	SendCorpsNotifyMsg( &tMsgHead, pRoleIDList, nRoleNum );
}

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseApplyCorps( int nServerID, int nRoleID, int nRet, int nCorpsID, const char *pCorpsName, int nEntityID /* = 0 */ )
{
	CMessage tMsgHead;
	CMessageApplyCorpsResponse tMsgApplyCorps;
	
	tMsgApplyCorps.mutable_corphead()->set_roleid( nRoleID );
	if( nEntityID == 0 )
	{
		tMsgApplyCorps.mutable_corphead()->set_entityid( nEntityID );
	}
	
	tMsgApplyCorps.set_errcode( nRet );
	if ( pCorpsName != NULL )
	{
		tMsgApplyCorps.set_corpsname( pCorpsName );
	}
	
	tMsgHead.set_msgpara( (int)&tMsgApplyCorps );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_APPLYCORPS );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseChangeCorpsBulletin( int nServerID, int nRoleID, int nRet, const char *pBulletin, int nEntityID )
{ 
	CMessage tMsgHead;
	CMessageChangeCorpsBulletinResponse tMsgChangeCorpsBulletin;
	
	tMsgChangeCorpsBulletin.mutable_corphead()->set_roleid( nRoleID );
	tMsgChangeCorpsBulletin.mutable_corphead()->set_entityid( nEntityID );
	tMsgChangeCorpsBulletin.set_errcode( nRet );
	if ( pBulletin != NULL )
	{
		tMsgChangeCorpsBulletin.set_bulletin( pBulletin );
	}	
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CHANGECORPSBULLETIN );
	tMsgHead.set_msgpara( (int)&tMsgChangeCorpsBulletin );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseChangeCorpsHeader( int nServerID, int nRoleID, int nRet, int nMemberID, const char *pMemberName, int nEntityID )
{
	CMessage tMsgHead;
	CMessageChangeCorpsHeaderResponse tMsgChangeHeader;
	
	tMsgChangeHeader.mutable_corphead()->set_roleid( nRoleID );
	tMsgChangeHeader.mutable_corphead()->set_entityid( nEntityID );
	tMsgChangeHeader.set_memberid( nMemberID );
	if ( pMemberName != NULL )
	{
		tMsgChangeHeader.set_membername( pMemberName );
	}
	tMsgChangeHeader.set_errcode( nRet );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CHANGECORPSHEADER );
	tMsgHead.set_msgpara( (int)&tMsgChangeHeader );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseCreatCorps( int nServerID, int nRoleID, int nRet, int nCorpsID, const char *pCorpsName, int nEntityID, int nCorpsColor )
{
	CMessage MsgHead;
	CMessageCreatCorpsResponse MsgCreatCorps;
	
	MsgCreatCorps.set_errcode( nRet ) ;
	MsgCreatCorps.set_corpsid( nCorpsID );
	if ( pCorpsName != NULL )
	{
		MsgCreatCorps.set_corpsname( pCorpsName );
	}	
	MsgCreatCorps.mutable_corphead()->set_roleid( nRoleID );
	
	if ( nEntityID != 0 )
	{
		MsgCreatCorps.mutable_corphead()->set_entityid( nEntityID );
	}
	
	MsgCreatCorps.set_corpscolor( nCorpsColor );
	
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CREATCORPS );
	MsgHead.set_msgpara( (int)&MsgCreatCorps );
	CGateServer::GetSingleton().Send2Scene( nServerID, &MsgHead );
}

// ***********************************************************
//  Function:		 SendResponseDisbandCorps
//  Description:	 发送解散军团的回应消息
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseDisbandCorps( int nServerID, int nRoleID, int nRet, int nEntityID )
{	
	CMessage tMsgHead;
	CMessageDisbandCorpsResponse tMsgDisbandCorps;
	
	tMsgDisbandCorps.mutable_corphead()->set_roleid( nRoleID );
	tMsgDisbandCorps.mutable_corphead()->set_entityid( nEntityID );
	tMsgDisbandCorps.set_errcode( nRet );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_DISBANDCORPS );
	tMsgHead.set_msgpara( (int)&tMsgDisbandCorps );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}


// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseGetCorpsApplyList( int nServerID, int nRoleID, int nRet, PBCorpsApplyList &pbApplyList, int nEntityID )
{  
	CMessage tMsgHead;
	CMessageGetCorpsApplyListResponse tMsgGetApplyList;
	
	tMsgGetApplyList.mutable_corphead()->set_roleid( nRoleID );
	tMsgGetApplyList.mutable_corphead()->set_entityid( nEntityID );
	tMsgGetApplyList.mutable_corpsapply()->CopyFrom( pbApplyList );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETCORPSAPPLYLIST );
	tMsgHead.set_msgpara( (int)&tMsgGetApplyList );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseGetCorpsFamilyInfo( int nServerID, int nRoleID, PBCorpsMemberList &pbCorpsMemberList, int nEntityID )
{
	CMessage tMsgHead;
	CMessageGetCorpsFamilyInfoResponse tMsgGetCorpsFamilyInfo;
	
	tMsgGetCorpsFamilyInfo.mutable_corphead()->set_roleid( nRoleID );
	tMsgGetCorpsFamilyInfo.mutable_corphead()->set_entityid( nEntityID );
	for ( int i = 0; i < pbCorpsMemberList.corpsmemberlist_size(); ++i )
	{
		tMsgGetCorpsFamilyInfo.add_corpsmemberinfo()->CopyFrom( *pbCorpsMemberList.mutable_corpsmemberlist( i ) );
	}
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETCORPSFAMILYINFO );
	tMsgHead.set_msgpara( (int)&tMsgGetCorpsFamilyInfo );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseGetCorpsPropertyInfo( int nServerID, int nRoleID, CorpsProperty &pbCorpsProperty, int nEntityID )
{
	CMessage tMsgHead;
	CMessageGetCorpsPropertyResponse tMsgGetCorpsProperty;
	
	tMsgGetCorpsProperty.mutable_corphead()->set_roleid( nRoleID );
	tMsgGetCorpsProperty.mutable_corphead()->set_entityid( nEntityID );
	tMsgGetCorpsProperty.mutable_corpproperty()->CopyFrom( pbCorpsProperty );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETCORPSPROPERTYINFO );
	tMsgHead.set_msgpara( (int)&tMsgGetCorpsProperty );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		 SendResponseInviteCorpsMember
//  Description:	 邀请家族加入军团的回复消息
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseInviteCorpsMember( int nServerID, int nRoleID, int nRet, const char *pFamilyName, int nEntityID )
{
	CMessage tMsgHead;
	CMessageInviteCorpsMemberResponse tMsgInviteCorpsMember;	
	
	tMsgInviteCorpsMember.mutable_corphead()->set_roleid( nRoleID );
	tMsgInviteCorpsMember.mutable_corphead()->set_entityid( nEntityID );
	if ( pFamilyName != NULL )
	{
		tMsgInviteCorpsMember.set_familyname( pFamilyName );
	}
	tMsgInviteCorpsMember.set_errcode( nRet );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_INVITECORPSMEMBER );					
	tMsgHead.set_msgpara( (int)&tMsgInviteCorpsMember );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseKickFamily( int nServerID, int nRoleID, int nRet, int nFamilyID, const char *pFamilyName, int nEntityID )
{
	CMessage tMsgHead;
	CMessageKickFamilyResponse tMsgKickFamily;
	
	tMsgKickFamily.mutable_corphead()->set_roleid( nRoleID );	
	tMsgKickFamily.mutable_corphead()->set_entityid( nEntityID );
	tMsgKickFamily.set_familyid( nFamilyID );
	if ( pFamilyName != NULL )
	{
		tMsgKickFamily.set_familyname( pFamilyName );
	}
	tMsgKickFamily.set_errcode( nRet );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_KICKFAMILY );	
	tMsgHead.set_msgpara( (int)&tMsgKickFamily );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		  SendResponseLeaveCorps
//  Description:	  发送离开军团的消息回应
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseLeaveCorps( int nServerID, int nRoleID, int nRet, int nEntityID )
{
	CMessage tMsgHead;
	CMessageFamilyLeaveCorpsResponse tMsgLeaveCorps;
	
	tMsgLeaveCorps.mutable_corphead()->set_roleid( nRoleID );
	tMsgLeaveCorps.mutable_corphead()->set_entityid( nEntityID );	
	tMsgLeaveCorps.set_errcode( nRet );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_LEAVECORPS );
	tMsgHead.set_msgpara( (int)&tMsgLeaveCorps );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		
//  Description:
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseReplyCorpsInvite( int nServerID, int nRoleID, int nRet, int nResult, int nCorpsID, const char *pCorpsName, int nEntityID, int nUnionID /* = 0 */, const char *pUnionName /* = NULL */ )
{ 
	CMessage tMsgHead;
	CMessageReplyCorpsInviteResponse tMsgReplyCorpsInvite;
	
	tMsgReplyCorpsInvite.mutable_corphead()->set_roleid( nRoleID );	
	tMsgReplyCorpsInvite.mutable_corphead()->set_entityid( nEntityID );
	
	tMsgReplyCorpsInvite.set_result( nResult );
	tMsgReplyCorpsInvite.set_errcode( nRet );
	tMsgReplyCorpsInvite.set_corpsid( nCorpsID );
	if ( pCorpsName != NULL )
	{
		tMsgReplyCorpsInvite.set_corpsname( pCorpsName );
	}
	tMsgReplyCorpsInvite.set_unionid( nUnionID );
	if ( pUnionName != NULL )
	{
		tMsgReplyCorpsInvite.set_unionname( pUnionName );
	}	
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REPLYCORPSINVITE );
	tMsgHead.set_msgpara( (int)&tMsgReplyCorpsInvite );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
}

// ***********************************************************
//  Function:		SendResponseReplyCorpsMemberApply	
//  Description:	发送答复家族申请信息的回复消息
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendResponseReplyCorpsMemberApply( int nServerID, int nRoleID, int nRet, int nResult, int nFamilyID, int nEntityID )
{ 
	CMessage tMsgHead;
	CMessageReplyCorpsMemberApplyResponse MsgReplyApply;
	
	MsgReplyApply.set_errcode( nRet );
	MsgReplyApply.set_result( nResult );
	MsgReplyApply.set_familyid( nFamilyID );
	MsgReplyApply.mutable_corphead()->set_roleid( nRoleID );
	MsgReplyApply.mutable_corphead()->set_entityid( nEntityID );
	
	tMsgHead.set_msgpara( (int)&MsgReplyApply );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REPLYCORPSMEMBERAPPLY );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendResponseClearCorpsApply
//  Description:	发送清除军团申请信息的回应消息
//  Output:			
//  Date:	06/22/2009
// **********************************************************
void CCorpservice::SendResponseClearCorpsApply( int nServerID, int nRet, int nRoleID, int nEntityID )
{															 
	CMessage tMsgHead;
	CMessageClearCorpsApplyResponse tMsgClearCorpsApply;
	
	tMsgClearCorpsApply.mutable_corphead()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgClearCorpsApply.mutable_corphead()->set_entityid( nEntityID );
	}
	tMsgClearCorpsApply.set_errcode( nRet );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CLEARCORPSAPPLY );	
	tMsgHead.set_msgpara( (int)&tMsgClearCorpsApply );
		
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendResponseRefreshCorpsInfo
//  Description:	刷新军团信息的消息回应
//  Output:			
//  Date:	06/23/2009
// **********************************************************
void CCorpservice::SendResponseRefreshCorpsInfo( int nServerID, int nRoleID, int nEntityID, PBCorpsMemberList &pbCorpsMemberList )
{
	CMessage tMsgHead;
	CMessageRefreshCorpsInfoResponse tMsgRefreshCorps;
	
	tMsgRefreshCorps.mutable_corphead()->set_roleid( nRoleID );		
	if ( nEntityID != 0 )
	{
		tMsgRefreshCorps.mutable_corphead()->set_entityid( nEntityID );
	}
	for ( int i = 0; i < pbCorpsMemberList.corpsmemberlist_size(); ++i )
	{
		tMsgRefreshCorps.add_corpsmember()->CopyFrom( *pbCorpsMemberList.mutable_corpsmemberlist( i ) );
	}
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REFRESHCORPSINFO );
	tMsgHead.set_msgpara( (int)&tMsgRefreshCorps );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendResponseGetCorpsList
//  Description:	发送获取军团信息的回应消息
//  Output:			
//  Date:	08/05/2009
// **********************************************************
void CCorpservice::SendResponseGetCorpsList( int nServerID, int nRoleID, int nEntityID, CorpsInfoList &pbCorpsList, int nCurPage, int nTotalPage )
{ 
	CMessage tMsgHead;
	CMessageGetCorpsListResponse tMsgGetCorpsList;	
	
	tMsgGetCorpsList.mutable_corphead()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgGetCorpsList.mutable_corphead()->set_entityid( nEntityID );
	}															  	
	tMsgGetCorpsList.mutable_corpslist()->CopyFrom( pbCorpsList );
	tMsgGetCorpsList.set_curpage( nCurPage );
	tMsgGetCorpsList.set_totalpage( nTotalPage );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETCORPSLIST );
	tMsgHead.set_msgpara( (int)&tMsgGetCorpsList );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
// ***********************************************************
//  Function:		SendResponseSetCorpMemberPost
//  Description:	设置军团成员职位的消息回应
//  Output:			
//  Date:	08/25/2009
// **********************************************************	
void CCorpservice::SendResponseSetCorpMemberPost( int nServerID, int nRoleID, int nErrcode, int nMemberID, int nPost, int nEntityID /* = 0 */ )
{
	CMessage tMsgHead;
	CMessageSetCorpsPostResponse tMsgSetCorpsPost;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_SETCORPSPOST );	
	tMsgHead.set_msgpara( (int)&tMsgSetCorpsPost );
	
	tMsgSetCorpsPost.mutable_corphead()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgSetCorpsPost.mutable_corphead()->set_entityid( nEntityID );
	}
	
	tMsgSetCorpsPost.set_memberid( nMemberID );
	tMsgSetCorpsPost.set_post( nPost );
	tMsgSetCorpsPost.set_errcode( nErrcode );
	
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendResponseLevelUPCorps
//  Description:	升级军团的消息回应
//  Output:			
//  Date:	08/25/2009
// **********************************************************	
void CCorpservice::SendResponseLevelUPCorps( int nServerID, int nErrcode, int nLevel, int nRoleID, int nEntityID /* = 0 */ )
{
	CMessage tMsgHead;
	CMessageLevelUPCorpsResponse tMsgLevelUPCorps;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_LEVELUPCORPS );
	tMsgHead.set_msgpara( (int)&tMsgLevelUPCorps );
	
	tMsgLevelUPCorps.mutable_corphead()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgLevelUPCorps.mutable_corphead()->set_entityid( nEntityID );
	}		
	
	tMsgLevelUPCorps.set_errcode( nErrcode );
	tMsgLevelUPCorps.set_level( nLevel );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendResponseContributeCorpsMoney
//  Description:	贡献军团金钱的消息回应
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CCorpservice::SendResponseContributeCorpsMoney( int nServerID, int nErrcode, int nMoney, int nRoleID, int nEntityID /* = 0 */ )
{
	CMessage tMsgHead;
	CMessageContributeCorpsMoneyResponse tMsgContributeMoney;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CONTRIBUTECORPSMONEY );		
	tMsgHead.set_msgpara( (int)&tMsgContributeMoney );
	
	tMsgContributeMoney.mutable_corphead()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgContributeMoney.mutable_corphead()->set_entityid( nEntityID );
	}
	
	tMsgContributeMoney.set_errcode( nErrcode );
	tMsgContributeMoney.set_money( nMoney );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
}

// ***********************************************************
//  Function:		SendResponseFamilyContributeMoney
//  Description:	家族给军团贡献金钱的消息回应
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CCorpservice::SendResponseFamilyContributeMoney( int nServerID, int nErrcode, int nMoney, int nRoleID, int nEntityID /* = 0 */ )
{	
	CMessage tMsgHead;
	CMessageFamilyContributeMoneyResponse tMsgContributeMoney;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_FAMILYCONTRIBUTEMONEY );	
	tMsgHead.set_msgpara( (int)&tMsgContributeMoney );
	
	tMsgContributeMoney.mutable_corphead()->set_roleid( nRoleID );
	tMsgContributeMoney.mutable_corphead()->set_entityid( nEntityID );
	
	tMsgContributeMoney.set_errcode( nErrcode );
	tMsgContributeMoney.set_money( nMoney );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );		
}

// ***********************************************************
//  Function:		SendResponseSetMemberName
//  Description:	设置军团成员称号的消息回应
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CCorpservice::SendResponseSetMemberName( int nServerID, int nMemberID, const char *pName, int nRoleID, int nEntityID, int nErrcode )
{
	CMessage tMsgHead;
	CMessageSetMemberNameResponse tMsgSetMemberName;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_SETMEMBERNAME );	
	tMsgHead.set_msgpara( (int)&tMsgSetMemberName );
	
	tMsgSetMemberName.mutable_corphead()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgSetMemberName.mutable_corphead()->set_entityid( nEntityID );
	}	 
		
	tMsgSetMemberName.set_memberid( nMemberID );
	if ( pName != NULL )
	{
		tMsgSetMemberName.set_name( pName );
	}		
	tMsgSetMemberName.set_errcode( nErrcode );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
}

// ***********************************************************
//  Function:		SendNotifySetCorpsPost
//  Description:	设置军团职位的通知
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CCorpservice::SendNotifySetCorpsPost( int *pIDList, int nNum, int nManagerID, const char *pManagerName, int nMemberID, const char *pMemberName, int nOriginalPost, int nNewPost )
{ 
	LK_ASSERT( pIDList != NULL, return )
	CMessage tMsgHead;
	CMessageSetCorpsPostNotify tMsgSetCorpsPost;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_SETCORPSPOST );	
	tMsgHead.set_msgpara( (int)&tMsgSetCorpsPost );
	
	tMsgSetCorpsPost.mutable_corphead()->set_roleid( 0 );
	
	tMsgSetCorpsPost.set_managerid( nManagerID );
	if ( pManagerName != NULL )
	{
		tMsgSetCorpsPost.set_managername( pManagerName );
	}	
	tMsgSetCorpsPost.set_memberid( nMemberID );
	if ( pMemberName != NULL )
	{
		tMsgSetCorpsPost.set_membername( pMemberName );
	}	
	tMsgSetCorpsPost.set_newpost( nNewPost );
	tMsgSetCorpsPost.set_originalpost( nOriginalPost );
	
	SendCorpsNotifyMsg( &tMsgHead, pIDList, nNum );
}

// ***********************************************************
//  Function:		SendNotifyCorpsLevelUP
//  Description:	军团升级的通知
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CCorpservice::SendNotifyCorpsLevelUP( int *pIDList, int nNum, int nLevel, int nMoney, int nGlory, int nMaxFamilyNum, int nMaxGeneralNum, int nCounsellor, int nLevelMoney, int nLevelGlory )
{
	LK_ASSERT( pIDList != NULL, return )	
	CMessage tMsgHead;
	CMessageCorpsLevelUPNotify tMsgCorpsLevelUP;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSLEVELUP );
	tMsgHead.set_msgpara( (int)&tMsgCorpsLevelUP );
	tMsgCorpsLevelUP.mutable_corphead()->set_roleid( 0 );
		
	tMsgCorpsLevelUP.set_level( nLevel ); 
	tMsgCorpsLevelUP.set_money( nMoney );
	tMsgCorpsLevelUP.set_glory( nGlory );
	tMsgCorpsLevelUP.set_maxfamilynum( nMaxFamilyNum );
	tMsgCorpsLevelUP.set_maxgeneralnum( nMaxGeneralNum );
	tMsgCorpsLevelUP.set_maxcounsellornum( nCounsellor );
	tMsgCorpsLevelUP.set_levelglory( nLevelGlory );
	tMsgCorpsLevelUP.set_levelmoney( nLevelMoney );
	
	SendCorpsNotifyMsg( &tMsgHead, pIDList, nNum );
}

// ***********************************************************
//  Function:		SendNotifyCorpsMoneyChanged
//  Description:	发送军团金钱改变的消息通知
//  Output:			
//  Date:	09/14/2009
// **********************************************************
void CCorpservice::SendNotifyCorpsMoneyChanged( int nCorpsID, int nCorpsMoney, int nRoleID, int nValue, int nReason /* = REASON_CONTRIBUTE */, const char *pName /* = NULL */ )
{		
	CMessage tMsgHead;
	CMessageCorpsMoneyChangedNotify tMsgCorpsMoney;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSMONEYCHANGED );	
	tMsgHead.set_msgpara( (int)&tMsgCorpsMoney );
	
	tMsgCorpsMoney.set_roleid( nRoleID );
	tMsgCorpsMoney.set_money( nCorpsMoney );
	tMsgCorpsMoney.set_reason( nReason );
	tMsgCorpsMoney.set_valeu( nValue );
	if ( pName != NULL )
	{
		tMsgCorpsMoney.set_rolename( pName );
	}
	
	SendCorpsMsgByID( nCorpsID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyCorpsGlroyChangedNotify
//  Description:	发送军团荣耀改变的消息通知
//  Output:			
//  Date:	09/14/2009
// **********************************************************
void CCorpservice::SendNotifyCorpsGlroyChangedNotify( int nCorpsID, int nCorpsGlroy, int nRoleID, int nValue, int nReason /* = REASON_NPCWELFARE */, const char *pName /* = NULL */ )
{
	CMessage tMsgHead;
	CMessageCorpsGloryChangedNotify tMsgCorpsGlory;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSGLORYCHANGED );
	tMsgHead.set_msgpara( (int)&tMsgCorpsGlory );
	
	tMsgCorpsGlory.set_roleid( nRoleID );
	tMsgCorpsGlory.set_glory( nCorpsGlroy );
	tMsgCorpsGlory.set_reason( nReason );
	tMsgCorpsGlory.set_value( nValue );
	if ( pName != NULL )
	{
		tMsgCorpsGlory.set_rolename( pName );
	}	
	SendCorpsMsgByID( nCorpsID, &tMsgHead );
}

void CCorpservice::SendNotifyBossTimesChanged(int nCorpsID, int nBossID, int nTimes,int nValue, int nRoleID, int nReason, const char * pName  )
{
	CMessage tMsgHead;
	CMessageCorpsBossTimesChangedNotify tMsgBossTimesChanged;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_CORPSBOSSTIMES_CHANGED_NOTIFY);
	tMsgHead.set_msgpara( (int)& tMsgBossTimesChanged );
	
	tMsgBossTimesChanged.set_roleid( nRoleID );
	tMsgBossTimesChanged.set_bossid( nBossID );
	tMsgBossTimesChanged.set_reason( nReason );
	tMsgBossTimesChanged.set_value( nValue );
	tMsgBossTimesChanged.set_times( nTimes );
	if ( pName != NULL )
	{
		tMsgBossTimesChanged.set_rolename( pName );
	}

	SendCorpsMsgByID( nCorpsID, &tMsgHead );

}

void CCorpservice::SendNotifyBossRefineValueChanged( int nCorpsID, int nRefineValue, int nValue, int nRoleID, int nReason , const char *pName )
{
	CMessage tMsgHead;
	CMessageCorpsBossRefineValueNotify tMsgRefineValueChanged;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_CORPSBOSSREFINEVALUE_CHANGED_NOTIFY );
	tMsgHead.set_msgpara( (int)& tMsgRefineValueChanged );

	tMsgRefineValueChanged.set_roleid( nRoleID );
	tMsgRefineValueChanged.set_reason( nReason );
	tMsgRefineValueChanged.set_refinevalue( nRefineValue );
	tMsgRefineValueChanged.set_value( nValue );
	if( pName != NULL)
	{
		tMsgRefineValueChanged.set_rolename( pName );
	}

	SendCorpsMsgByID( nCorpsID, & tMsgHead );
}
// ***********************************************************
//  Function:		SendNotifyCorpsContributeChangedNotify
//  Description:	发送军团的个人贡献度改变的消息通知
//  Output:			
//  Date:	09/14/2009
// **********************************************************
void CCorpservice::SendNotifyCorpsContributeChangedNotify( int nCorpsID, int nMemberContribute, int nRoleID, int nValue )
{	
	CMessage tMsgHead;
	CMessageCorpsContributeChangedNotify tMsgCorpsContribute;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSCONTRIBUTECHANGED );		
	tMsgHead.set_msgpara( (int)&tMsgCorpsContribute );
	
	tMsgCorpsContribute.set_roleid( nRoleID );
	tMsgCorpsContribute.set_contribute( nMemberContribute );
	tMsgCorpsContribute.set_value( nValue );
	
	// 目前只发给贡献度改变的玩家
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID );
	if ( tpPlayer != NULL )
	{
		tMsgCorpsContribute.mutable_corphead()->set_roleid( nRoleID );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
	}	
	
	//SendCorpsMsgByID( nCorpsID, &tMsgHead );					 
}

// ***********************************************************
//  Function:		SendNotifyCorpsCollectActiveChange
//  Description:	发送军团采集活跃度改变的消息通知
//  Output:			
//  Date:	
// **********************************************************
void CCorpservice::SendNotifyCorpsCollectActiveChange( int nCorpsID, int nRoleID, int nActiveValue, int nValue, int nChangeType )
{
	CMessage tMsgHead;
	CMessageCorpsCollectActiveChangeNotify tMsgCorpsCollectActiveChange ;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSCOLLECTACTIVECHANGED );
	tMsgHead.set_msgpara((int)&tMsgCorpsCollectActiveChange);

	tMsgCorpsCollectActiveChange.set_activevalue( nActiveValue );
	tMsgCorpsCollectActiveChange.set_vaule( nValue );
	tMsgCorpsCollectActiveChange.set_changetype( nChangeType );
		
	// 目前只发给改变军团活跃度的玩家
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID );
	if ( tpPlayer != NULL )
	{
		tMsgCorpsCollectActiveChange.mutable_corphead()->set_roleid( nRoleID );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
	}	

}

// ***********************************************************
//  Function:		SendNotifyCorpsPVPStarLevelUP
//  Description:	军团pvp等级提升的通知
//  Output:			
//  Date:	08/25/2009
// **********************************************************	
void CCorpservice::SendNotifyCorpsPVPStarLevelUP( int nServerID )
{
	
}

// ***********************************************************
//  Function:		SendNotifyCorpsCanLevelUP
//  Description:	发送军团可以升级的消息通知
//  Output:			
//  Date:	09/27/2009
// **********************************************************	
void CCorpservice::SendNotifyCorpsCanLevelUP( int nCorpsID )
{
	CMessage tMsgHead;
	CMessageCorpsCanLevelUPNotify tMsgCorpsCanLevelUP;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSCANLEVELUP );
	tMsgHead.set_msgpara( (int)&tMsgCorpsCanLevelUP );
	
	SendCorpsMsgByID( nCorpsID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyCorpsCanLevelUP
//  Description:	发送军团发布任务通知
//  Output:			
//  Date:	11/09/2011
// **********************************************************	
void CCorpservice::SendNotifyCorpsPublishTask( int nCorpsID )
{
	CMessage tMsgHead;
	CMessageCorpsPublishTaskNotify tMsgPublishTask;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSPUBLISHTASK );
	tMsgHead.set_msgpara( (int)&tMsgPublishTask );

	SendCorpsMsgByID( nCorpsID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendCorpsMsg
//  Description:	给军团成员发送消息
//  Output:			
//  Date:	06/25/2009
// **********************************************************
void CCorpservice::SendCorpsMsg( int nRoleID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )		
		
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( nRoleID );
	if ( tpCorps == NULL )
	{						
		return;
	}
	
	pMessage->mutable_msghead()->set_dstfe( FE_CORPS );
	int tRoleIDList[CORPS_MAXMEMBERNUM] = { 0 };
	int tNum = CORPS_MAXMEMBERNUM;
	CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tRoleIDList, tNum );
	
	for ( int i = 0; i < tNum; ++ i )
	{
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tRoleIDList[i] );
		if ( tpPlayer == NULL )
		{
			continue;
		}	
		pMessage->mutable_msghead()->set_dstid( tRoleIDList[i] );	
		CGateServer::GetSingleton().Send2Scene( tpPlayer, pMessage );
	}
}

// ***********************************************************
//  Function:		SendCorpsNotifyMsg
//  Description:	发送军团的消息通知
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CCorpservice::SendCorpsNotifyMsg( CMessage *pMessage, int *pRoleIDList, int vRoleNum )
{
	LK_ASSERT( pMessage != NULL && pRoleIDList != NULL, return )	
		LK_ASSERT( pMessage != NULL, return );

	Message* pUnknownMessage = (Message*) pMessage->msgpara();	 
	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessage->GetDescriptor();
	const ::google::protobuf::Reflection*  pReflection = pUnknownMessage->GetReflection();
	if (pDescriptor == NULL || pReflection == NULL) return;	   
	const ::google::protobuf::FieldDescriptor*  pFieldDescriptor = pDescriptor->FindFieldByNumber(1);
	if (pFieldDescriptor == NULL) return;					 
	assert(pFieldDescriptor->type() == ::google::protobuf::FieldDescriptor::TYPE_MESSAGE);	  
	const Message& tRouter = pReflection->GetMessage(*pUnknownMessage, pFieldDescriptor); 	
	CorpsHead* tpCorpsHead = (CorpsHead*) &tRouter;	

	for ( int i = 0; i<vRoleNum; ++i )
	{
		CGatePlayer *pPlayer = CPlayerManager::GetSingleton().ScenePlayer( pRoleIDList[i] );
		if ( pPlayer == NULL  )
		{
			continue;
		}
		tpCorpsHead->set_roleid( pRoleIDList[i] );
		CGateServer::GetSingleton().Send2Scene( pPlayer->mLineID, pMessage );
	}
}

// ***********************************************************
//  Function:		OnCreatCorpsDBInfo
//  Description:	创建军团数据信息的回应消息
//  Output:			
//  Date:	06/15/2009
// **********************************************************
int CCorpservice::OnSessionCreatCorpsDBInfo( CMessage* pMessage, int nCorpsObjID, int nRooterID )
{
	LK_ASSERT( pMessage != NULL, return  -1 )
	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return -1 )
	CCorps *tpCorps = ( CCorps * )CGateObjManager::GetSingleton().GetObject( nCorpsObjID );
	LK_ASSERT( tpCorps != NULL, return -1 )	
	if( tpMsg->resultcode() == 1 )
	{	
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nRooterID );
		if ( pFamily == NULL )
		{
			LOG_ERROR( "corps"," can not find family of player ( roleid:%d ) ", nRooterID );
			return -1;
		}

		
		#ifndef _GLOBAL_NAME_
		tpCorps->SetCorpsID( tpMsg->latestautoincid() );
		#endif
		
		int tRet = CCorpsManager::GetSingleton().InsertCoprsInfo( tpCorps );
		if ( tRet == -1 )
		{
			CGateObjManager::GetSingleton().DestroyObject( tpCorps->get_id() );
			return -1;
		}		

		pFamily->SetCorpsID( tpCorps->GetCorpsID() );

		int tOldPost = 0;
		pFamily->SetPost( nRooterID, CORPS_POST_CORPSHEADER, tOldPost, CFamily::EM_CORPSACTION );		 

		CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( nRooterID );
		LK_ASSERT( pGatePlayer != NULL, return -1 )		
		SendResponseCreatCorps( pGatePlayer->mLineID, nRooterID, SUCCESS, tpCorps->GetCorpsID(), tpCorps->GetCorpsName(), 0, tpCorps->GetColor() );
		int tRoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 };
		int tRoleNum = 	MAX_FAMILYMEMBER_NUM;
		pFamily->GetFamilyMemberIDList( tRoleIDList, tRoleNum,  pFamily->GetRooter() );
		if ( tRoleNum > 0 )
		{
			CServiceFamily::GetSingleton().SendNotifyJoinCorps( tpCorps->GetCorpsID(), tpCorps->GetCorpsName(), tRoleIDList, tRoleNum, JOINCORPS_CREATBYFAMILYHEADER );
		}
		
		LogOrganCreate( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpCorps->GetRooterID(), 1, 1 )		
	}
	return 0;
}






// ***********************************************************
//  Function:		SendCorpsMsgByID
//  Description:	通过军团ID给军团成员发送消息
//  Output:			
//  Date:	07/20/2009
// **********************************************************
void CCorpservice::SendCorpsMsgByID( int nCorpsID, CMessage *pMessage )
{	
	LK_ASSERT( pMessage != NULL, return )	
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( nCorpsID );
	if ( tpCorps == NULL )
	{						
		LOG_ERROR( "corps"," can not find corps by ID:%d ", nCorpsID );
		return;
	}

	pMessage->mutable_msghead()->set_dstfe( FE_CORPS );	
	int tRoleIDList[CORPS_MAXMEMBERNUM] = { 0 };
	int tNum = CORPS_MAXMEMBERNUM;
	std::vector< CGatePlayer * > vPlayerList;
	CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tRoleIDList, tNum );

	for ( int i = 0; i < tNum; ++ i )
	{
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tRoleIDList[i] );
		if ( tpPlayer == NULL )
		{
			continue;
		}	
		
		
		if( pMessage->mutable_msghead()->srcfe() == FE_GATESERVER )
		{
			vPlayerList.push_back( tpPlayer );
		}
		else
		{
			pMessage->mutable_msghead()->set_dstid( tRoleIDList[i] );	
			CGateServer::GetSingleton().Send2Scene( tpPlayer, pMessage );
		}
	}	
	
	if ( pMessage->mutable_msghead()->srcfe() == FE_GATESERVER && vPlayerList.size() > 0 )
	{
		CGateServer::GetSingletonPtr()->Send2Player( vPlayerList, pMessage );
	}
}





// ***********************************************************
//  Function:		ConsumeCorpsMoney
//  Description:	消耗军团金钱
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpservice::ConsumeCorpsMoney( int nRoleID, int nMoney, int nReason /* = REASON_ACTIVIYCONSUME */ )
{			
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( nRoleID );	
	if ( tpCorps == NULL )
	{	 
		LOG_ERROR( "corps","[%s]can not find corps of player ( roleid:%d)", __FUNCTION__, nRoleID );
		return -1;
	}	
	
	int tOldCorpsMoney = tpCorps->GetCorpsMoney();
	
	int tRet = CCorpsManager::GetSingleton( ).OnEventConsumeMoney( nRoleID, nMoney );
	if ( tRet != SUCCESS )
	{
		return tRet;
	}
	
	LOG_NOTICE( "corps","[%s,%d] consume corps money( managerid:%d, corpsid:%d, money:%d, reason:%d ) ", __FUNCTION__,__LINE__, nRoleID, tpCorps->GetCorpsID(), nMoney, nReason );	
	SendNotifyCorpsMoneyChanged( tpCorps->GetCorpsID(), tpCorps->GetCorpsMoney(), nRoleID, -1*nMoney, nReason, tpCorps->GetRooterName() );	
	
	// LOG:消耗金钱
	LogOrganGetMoney( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), nRoleID, tpCorps->GetCorpsMoney(), tOldCorpsMoney )
	return SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/// 军团战大旗归属处理
void CCorpservice::OnMessageBattleFlagRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsBattleFlagRequest* tpReq = ( CMessageCorpsBattleFlagRequest* ) pMessage->msgpara();
		
	CMessage tMessage;
	CMessageCorpsBattleFlagResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_G2S_RESPONSE_BATTLEFLAG );
	tMessage.set_msgpara( (long) &tPara );

	tPara.set_charid( tpReq->charid() );
	tPara.set_battleid( tpReq->battleid() );
	tPara.set_isall( tpReq->isall() );
	tPara.set_campid( tpReq->campid() );

	KEY_TYPE tKey = ( KEY_TYPE )tpReq->battlekey();
	int tNpcID = 0;
	CCorps* pCorps = CBattleMgr::GetSingleton().GetBattleCorpsByCampID(tKey, tpReq->campid(), tNpcID);
	if (NULL == pCorps)
	{
		//LOG_ERROR( "corps", "Corps[%d:%d] Not Found.[%s:%d]", tSrcCorpsID, tDesCorpsID, __LK_FILE__, __LINE__ );
		return ;
	}

	LOG_NOTICE( "corps", "============Battle Flag Own=============" );

	tPara.set_corpsname( pCorps->GetCorpsName() );
	LOG_NOTICE( "corps", "ServerID[%d],NpcID[%d],FlagOwner[%d:%s]", 
		pCorps->GetCorpsCountryID(), 
		tNpcID, 
		pCorps->GetCorpsID(), 
		pCorps->GetCorpsNameShow() );

	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );	
	return ;
}

void CCorpservice::SendCorpsProperty( int nCorpsID )
{
	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( nCorpsID );
	if ( tpCorps == NULL )
	{
		return ;
	}

	// 通知更新军团属性
	CMessage tMessage;
	CMessageCorpsPropertyNotice tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSPROPERTY );
	tMessage.set_msgpara( (long) &tPara );

	CorpsProperty* tpCorpsPro = tPara.mutable_property();
	tpCorps->MakeCorpsPbPropert( tpCorpsPro );
	SendCorpsMsgByID( tpCorps->GetCorpsID(), &tMessage );
	return ;
}

// ***********************************************************
//  Function:		SendResponseBuyCorpsItem
//  Description:	发送购买军团红包的消息回应
//  Output:			
//  Date:	11/03/2009
// **********************************************************
void CCorpservice::SendResponseBuyCorpsItem( CGatePlayer *pPlayer, int nErrcode, int nRedPaperID, int nRedPaperNum, int nEntityID /* = 0 */ )
{	
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageBuyCorpsItemResponse tMsgBuyCorpsItem;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_G2S_RESPONSE_BUYCORPSITEM );	
	tMsgHead.set_msgpara( (int)&tMsgBuyCorpsItem );
	
	if ( nEntityID != 0 )
	{
		tMsgBuyCorpsItem.set_entityid( nEntityID );
	}				  
	tMsgBuyCorpsItem.set_roleid( pPlayer->mCharID );
	
	tMsgBuyCorpsItem.set_errcode( nErrcode );
	tMsgBuyCorpsItem.set_itemid( nRedPaperID );
	tMsgBuyCorpsItem.set_itemnum( nRedPaperNum );
	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

/// 战争开始 [11/28/2009 Macro]
void CCorpservice::OnMessageStartBattleRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsStartBattleRequest* tpReq = ( CMessageCorpsStartBattleRequest* ) pMessage->msgpara();

	//通过NPCID从BattleMgr得到攻方和守方军团ID
	int tSrcCorpsID = 0, tDesCorpsID = 0, tMapID;
	int tNpcID = GATENPCID( tpReq->npcid(), nServerID );
	CBattleMgr::GetSingleton().GetBattle( tNpcID, tSrcCorpsID, tDesCorpsID, tMapID );
	
	CCorps* tpSrcCorps =  CCorpsManager::GetSingleton().GetCorpsByID( tSrcCorpsID );
	CCorps* tpDesCorps =  CCorpsManager::GetSingleton().GetCorpsByID( tDesCorpsID );

	if ( tpSrcCorps == NULL || tpDesCorps == NULL )
	{
		/*LOG_ERROR( "corps", "Corps Object error.Att ID[%d], Def ID[%d][%s:%d]", tSrcCorpsID, tDesCorpsID, 
			__LK_FILE__, __LINE__ );*/
		return ;
	}

	int tSrcList[ MAX_BATTLESIZE ] = { 0 };
	int tDesList[ MAX_BATTLESIZE ] = { 0 };
	int tSrcCount = ARRAY_CNT( tSrcList );
	int tDesCount = ARRAY_CNT( tDesList );

	// 获取双方参战列表
	OnEventGetUnionBattleList( tSrcList, tSrcCount, tpSrcCorps, tMapID );
	OnEventGetUnionBattleList( tDesList, tDesCount, tpDesCorps, tMapID );

	// 如果一方没有报名，对方直接获胜；
	if ( tSrcCount == 0 || tDesCount == 0 )
	{
		if ( tSrcCount > 0 )
		{
			LOG_NOTICE( "corps", "Des Coprs no member to join battle.\n[NpcID:%d,ServerID:%d][Src:%s,Des:%s]", 
				tNpcID, nServerID, tpSrcCorps->GetCorpsNameShow(), tpDesCorps->GetCorpsNameShow() );
			OnDoBattleResultEvent( tpSrcCorps, tpReq->npcid(), tMapID, tpDesCorps );
		}
		else if ( tDesCount > 0 )
		{
			LOG_NOTICE( "corps", "Src Coprs no member to join battle.\n[NpcID:%d,ServerID:%d][Src:%s,Des:%s]", 
				tNpcID, nServerID, tpSrcCorps->GetCorpsNameShow(), tpDesCorps->GetCorpsNameShow() );
			OnDoBattleResultEvent( tpDesCorps, tpReq->npcid(), tMapID, tpSrcCorps );
		}

		// 删除战场
		// 此时 战场列表 有npc 战场参战数据，但是没有战场key
		CBattleMgr::GetSingleton().DelBattle( tNpcID );

		// 如果没有占领该npc，这从军团信息中删除该npc的信息

		/*int OwnerCorpsID = CBidMgr::GetSingleton().FindCityOwnerByMapID(tNpcID);
		if ( tpSrcCorps->HasOwnThisNpc( tpReq->npcid() ) == false  )
		{
			tpSrcCorps->ClearNpcBattleInfo( tpReq->npcid() );
		}
		if ( tpDesCorps->HasOwnThisNpc( tpReq->npcid() ) == false )
		{
			tpDesCorps->ClearNpcBattleInfo( tpReq->npcid() );
		}	*/	

		return ;
	}

//	KEY_TYPE tKey = CRepetionModule::GetSingleton().CreateBattle( CORPS_BATTLE_SCABBLE, tpSrcCorps->GetCorpsLevel() );

	// 创建副本战争？？
	KEY_TYPE tKey = CRepetionModule::GetSingleton().CreateBattleByIndex( _REPE_CORPS_BATTLE_INDEX_, 
		tpSrcCorps->GetCorpsLevel(), tSrcCorpsID, tpReq->npcid() );
	if ( tKey == INVITED_KEY_VALUE )
	{
		LOG_ERROR( "corps", "Create Battle Failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return ;
	}
	int tCampNum = CGateLogic::GetSingletonPtr( )->mConfig.mCorpsCampCount;

	// 添加玩家到副本
	for ( int i = 0; i < tSrcCount; i ++ )
	{
		CRepetionModule::GetSingleton().PlayerJoin( tSrcList[ i ], _REPE_CORPS_BATTLE_INDEX_, tKey, CAMP_1, tCampNum );
	}
	for ( int j = 0; j < tDesCount; j ++ )
	{
		CRepetionModule::GetSingleton().PlayerJoin( tDesList[ j ], _REPE_CORPS_BATTLE_INDEX_, tKey, CAMP_2, tCampNum );
	}

	// 更新战争状态
	//tpSrcCorps->SetNpcBattleInfo( tpReq->npcid(), emFightStep, tpDesCorps->GetCorpsID() );
	//tpDesCorps->SetNpcBattleInfo( tpReq->npcid(), emFightStep, tpSrcCorps->GetCorpsID() );

	// 战争开始
	CBattleMgr::GetSingleton().StartBattle( tKey, tNpcID );

	LOG_NOTICE( "corps", "==========Battle Begin=========" );
	LOG_NOTICE( "corps", "ServerID:[%d],NpcID[%d],Att[%s:%d,Num:%d],Des[%s:%d,Num:%d]", nServerID, tNpcID, 
		tpSrcCorps->GetCorpsNameShow(), tpSrcCorps->GetCorpsID(), tSrcCount, 
		tpDesCorps->GetCorpsNameShow(), tpDesCorps->GetCorpsID(), tDesCount );
	LOG_NOTICE( "corps", "===============================" );

	return ;
}


/// 创建/关闭房间
void CCorpservice::OnMessageOperateRoomRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsOperateRoomRequest* tpReq = ( CMessageCorpsOperateRoomRequest* ) pMessage->msgpara();

	int tCharID = tpReq->mutable_corphead()->roleid();
	
	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tCharID );
	if ( tpCorps == NULL )
	{
		return ;
	}
	
	CMessage tMessage;
	CMessageCorpsOperateRoomResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ROOMOPERATION );
	tPara.mutable_corphead()->set_roleid( tCharID );
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_action( tpReq->action() );
	
	// TODO:Post Need　Check
	// 只有军团长才能开/关房间
	if ( tpCorps->GetRooterID() != (int)tpReq->mutable_corphead()->roleid() )
	{			
		if ( !tpCorps->MemberHasRight( (int)tpReq->mutable_corphead()->roleid(), EM_RIGHT_CHALLENGEROOM ) )
		{
			tPara.set_rtncode( ERROR_CORPS_HASNORIGHT );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
			return ;
		}	
	}
	
	const int nCorpsChallangeMoney = CCorpsManager::GetSingletonPtr()->GetCorpsConfig()->GetChallangeMoney();


	// 如果军团处于挑战赛进行中，无法操作 [12/5/2009 Macro]
	if ( CBattleMgr::GetSingleton().InChallegeBattle( tpCorps->GetCorpsID() ) )
	{
		tPara.set_rtncode( ERROR_CORPS_INBATTLE );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
		return ;
	}	 

	if ( tpReq->action() == ROOM_OPEN )
	{
		// 解散状态不能操作
		if ( tpCorps->GetCorpsStatus() == CCorps::EM_CORPS_STATUS_DISBAND )
		{
			return ;
		}

		if ( CCorpsManager::GetSingleton().HasRoom( tpCorps->GetCorpsID() ) )
		{
			tPara.set_rtncode( ERROR_CORPS_ROOMEXIST );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
			return ;
		}
		if ( tpCorps->GetCorpsMoney() < nCorpsChallangeMoney )
		{
			tPara.set_rtncode( ERROR_CORPS_MONEYNOTENOUGH );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
			return ;
		}
		if ( tpReq->money() == 0 )
		{
			tPara.set_rtncode( ERROR_CORPS_MONEYINVALIDE );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
			return ;
		}

		CCorpsManager::GetSingleton().InsertRoom( tpCorps->GetCorpsID() );
		tpCorps->SetRoomName( tpReq->name().c_str() );
		tpCorps->SetRoomMoney( nCorpsChallangeMoney );
		tpCorps->SetRoomState( ROOM_STATE_OPEN );
		// TODO:Post Need　Check
		ConsumeCorpsMoney( (int)tpReq->mutable_corphead()->roleid(), nCorpsChallangeMoney, REASON_CHALLENGEWAR ); // tpCorps->GetRooterID()
	
		// LOG:开设挑战赛房间
		LogOpenRoom( ORGANLOGTYPE_CORPS, 
			tpCorps->GetCorpsID(), tpCorps->GetRooterID(), nCorpsChallangeMoney, 0, 0 );
	}
	else
	{
		if ( CCorpsManager::GetSingleton().HasRoom( tpCorps->GetCorpsID() ) == false )
		{
			/*tPara.set_rtncode( ERROR_CORPS_ROOMNOTFOUND );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );*/
			return ;
		}
		// LOG:关闭挑战赛房间
		LogCloseRoom( ORGANLOGTYPE_CORPS, 
			tpCorps->GetCorpsID(), tpCorps->GetRooterID(), tpCorps->GetRoomMoney(), 0, 0 );

		tpCorps->ContributeCorpsMoney( tpCorps->GetRoomMoney() );
		SendNotifyCorpsMoneyChanged( tpCorps->GetCorpsID(), tpCorps->GetCorpsMoney(), (int)tpReq->mutable_corphead()->roleid(), 
			tpCorps->GetRoomMoney(), REASON_CHALLENGEWAR );				// tpCorps->GetRooterID()
		tpCorps->SetRoomMoney( 0 );
		tpCorps->SetRoomName( "" );
		tpCorps->SetRoomState( ROOM_STATE_OPEN );
		tpCorps->ClearChallege< CCorps::ChallegeList >( tpCorps->m_ChallegeList );
		CCorpsManager::GetSingleton().OnDelRoom( tpCorps->GetCorpsID() );
	}

	tPara.set_rtncode( SUCCESS );
	tPara.set_name( tpReq->name() );
	tPara.set_money( tpReq->money() );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );

	CMessage tMsg;
	CMessageCorpsRoomNotice tNotice;
	tMsg.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_ROOM );
	tNotice.set_action( tpReq->action() );
	tNotice.set_rootername( tpCorps->GetRooterName() );
	tMsg.set_msgpara( (long) &tNotice );
	SendCorpsMsgByID( tpCorps->GetCorpsID(), &tMsg );

	return ;
}

/// 发起/放弃挑战
void CCorpservice::OnMessageChallegeRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsChallegeRequest* tpReq = ( CMessageCorpsChallegeRequest* ) pMessage->msgpara();

	int tCharID = tpReq->mutable_corphead()->roleid();
	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tCharID );
	if ( tpCorps == NULL )
	{
		return ;
	}

	CCorps* tpDesCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpReq->descropsid() );
	if ( tpDesCorps == NULL )
	{
		return; 
	}

	CMessage tMessage;
	CMessageCorpsChallegeResponse tPara;
	tMessage.set_msgpara( (long) &tPara );
	tPara.mutable_corphead()->set_roleid( tCharID );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CHALLEGE );
	tPara.set_action( tpReq->action() );

	// 只有军团长才能挑战
	if ( tpCorps->GetRooterID() != (int)tpReq->mutable_corphead()->roleid() )
	{
		if ( !tpCorps->MemberHasRight( (int)tpReq->mutable_corphead()->roleid(), EM_RIGHT_CHALLENGEROOM ) )
		{
			return ;
		}		
	}

	if ( CCorpsManager::GetSingleton().HasRoom( tpDesCorps->GetCorpsID() ) == false )
	{
		tPara.set_rtncode( ERROR_CORPS_ROOMNOTFOUND );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
		return ;
	}

	if ( tpReq->action() == CHALLEGE_REQUEST )
	{
		if ( tpCorps->GetCorpsID() == tpDesCorps->GetCorpsID() )
		{
			tPara.set_rtncode( ERROR_CORPS_YOURSELF );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
			return ;
		}
		if ( tpCorps->IsInBattle(TYPE_BATTLE_ALL, ST_WaitBtl|ST_InBtl|ST_Union|ST_Single) ||
			tpDesCorps->IsInBattle(TYPE_BATTLE_ALL, ST_WaitBtl|ST_InBtl|ST_Union|ST_Single) )
		{
			tPara.set_rtncode( ERROR_CORPS_INBATTLE );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
			return ;
		}
		if ( tpCorps->GetCorpsMoney() < tpDesCorps->GetRoomMoney() )
		{
			tPara.set_rtncode( ERROR_CORPS_MONEYNOTENOUGH );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
			return ;
		}
		tpDesCorps->AddChallege< CCorps::ChallegeList >( tpDesCorps->m_ChallegeList, tpCorps->GetCorpsID() );
		tpCorps->AddChallege< CCorps::ChallegeList >( tpCorps->m_SendChallegeList, tpDesCorps->GetCorpsID() );
	
		// LOG:发起挑战
		LogSendChallege( ORGANLOGTYPE_CORPS, 
			tpCorps->GetCorpsID(), tpCorps->GetRooterID(), tpDesCorps->GetCorpsID(), 0, 0 );
	}
	else if ( tpReq->action() == CHALLEGE_CANCEL )
	{
		tpCorps->DelChallege< CCorps::ChallegeList >( tpCorps->m_SendChallegeList, tpDesCorps->GetCorpsID() );
		tpDesCorps->DelChallege< CCorps::ChallegeList >( tpDesCorps->m_ChallegeList, tpCorps->GetCorpsID() );
		
		// LOG:放弃挑战
		LogCloseChallenge( ORGANLOGTYPE_CORPS, 
			tpCorps->GetCorpsID(), tpCorps->GetRooterID(), tpDesCorps->GetCorpsID(), 0, 0 );
	}

	tPara.set_rtncode( SUCCESS );
	tPara.set_descropsid( tpReq->descropsid() );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );

	CMessage tMsg;
	CMessageCorpsChallegeNotice tNotice;
	tMsg.set_msgpara( (long) &tNotice );
	tMsg.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHALLEGE );
	tNotice.set_srccorpsid( tpCorps->GetCorpsID() );
	tNotice.set_srccorpsname( tpCorps->GetCorpsName() );
	tNotice.set_descropsid( tpDesCorps->GetCorpsID() );
	tNotice.set_descorpsname( tpDesCorps->GetCorpsName() );
	tNotice.set_srcrootername( tpCorps->GetRooterName() );
	tNotice.set_desrootername( tpDesCorps->GetRooterName() );
	tNotice.set_action( tpReq->action() );

	SendCorpsMsgByID( tpCorps->GetCorpsID(), &tMsg );
	SendCorpsMsgByID( tpDesCorps->GetCorpsID(), &tMsg );

	return ;
}


/// 接受挑战
void CCorpservice::OnMessageReplyChallegeRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsReplyChallegeRequest* tpReq = ( CMessageCorpsReplyChallegeRequest* ) pMessage->msgpara();

	int tCharID = tpReq->mutable_corphead()->roleid();

	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tCharID );
	if ( tpCorps == NULL )
	{
		return ;
	}

	if ( CCorpsManager::GetSingleton().HasRoom( tpCorps->GetCorpsID() ) == false )
	{
		return ;
	}

	CCorps* tpDesCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpReq->descorpsid() );
	if ( tpDesCorps == NULL )
	{
		return ;
	}

	if ( tpCorps->InChallege< CCorps::ChallegeList >( tpCorps->m_ChallegeList, tpDesCorps->GetCorpsID() ) == false )
	{
		return ;
	}

	if ( tpDesCorps->IsInBattle(TYPE_BATTLE_ALL, ST_WaitBtl|ST_InBtl|ST_Union|ST_Single) )
	{
		return ;
	}

	// 如果对方有房间，把对方的房间删了
	if ( CCorpsManager::GetSingleton().HasRoom( tpDesCorps->GetCorpsID() ) )
	{
		CCorpsManager::GetSingleton().OnDelRoom( tpDesCorps->GetCorpsID() );
		tpDesCorps->ContributeCorpsMoney( tpDesCorps->GetRoomMoney() );
	}

	tpDesCorps->SetRoomMoney( tpCorps->GetRoomMoney() );
	ConsumeCorpsMoney( tpDesCorps->GetRooterID(), tpDesCorps->GetRoomMoney(), REASON_CHALLENGEWAR );

	CMessage tMessage;
	CMessageCorpsReplyChallegeResponse tPara;
	tMessage.set_msgpara( (long) &tPara );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REPLYCHALLEGE );
	tPara.mutable_corphead()->set_roleid( tCharID );
	tPara.set_descorpsname( tpDesCorps->GetCorpsName() );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );

	tpCorps->SetRoomState( ROOM_STATE_INWAR );
	tpDesCorps->SetRoomState( ROOM_STATE_INWAR );

	tpCorps->ClearChallege< CCorps::ChallegeList >( tpCorps->m_ChallegeList );
	tpCorps->ClearChallege< CCorps::ChallegeList >( tpCorps->m_SendChallegeList );
	tpDesCorps->ClearChallege< CCorps::ChallegeList >( tpDesCorps->m_ChallegeList );
	tpDesCorps->ClearChallege< CCorps::ChallegeList >( tpDesCorps->m_SendChallegeList );

//	KEY_TYPE tKey = CRepetionModule::GetSingleton().CreateBattle( CORPS_BATTLE_SCABBLE, tpCorps->GetCorpsLevel() );

	KEY_TYPE tKey = CRepetionModule::GetSingleton().CreateBattleByIndex( _REPE_CORPS_CHALLEGE_INDEX_, 
		tpCorps->GetCorpsLevel(), tpCorps->GetCorpsID( ) );
	if( tKey == INVITED_KEY_VALUE )
	{
		LOG_ERROR( "corps", "fuck you" );
		return;
	}
	
	//LogOrganChallageAccept( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), 0, tpDesCorps->GetCorpsID(), 0, 0 );

	int tSrcList[ CORPS_BATTLE_NUM * 3 ] = { 0 };
	int tDesList[ CORPS_BATTLE_NUM * 3 ] = { 0 };
	int tSrcCount = ARRAY_CNT( tSrcList );
	int tDesCount = ARRAY_CNT( tDesList );

	tpCorps->SetCorpsCamp( CAMP_2 );
	tpDesCorps->SetCorpsCamp( CAMP_1 );

	tpCorps->SetEnemyCorpsID( tpDesCorps->GetCorpsID() );
	tpDesCorps->SetEnemyCorpsID( tpCorps->GetCorpsID() );

	CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tSrcList, tSrcCount );
	CCorpsManager::GetSingleton().GetCorpsMemberList( tpDesCorps, tDesList, tDesCount );

	for ( int i = 0; i < tSrcCount; i ++ )
	{
		CRepetionModule::GetSingleton().PlayerJoin( tSrcList[ i ], _REPE_CORPS_CHALLEGE_INDEX_, tKey, tpCorps->GetCorpsCamp() );
	}
	for ( int j = 0; j < tDesCount; j ++ )
	{
		CRepetionModule::GetSingleton().PlayerJoin( tDesList[ j ], _REPE_CORPS_CHALLEGE_INDEX_, tKey, tpDesCorps->GetCorpsCamp() );
	}

	int tTimeStamp = time(NULL);

	CBattleMgr::GetSingleton().InitBattle( tpDesCorps->GetCorpsID(), tpCorps->GetCorpsID(), TYPE_BATTLE_CHALLEGE, tTimeStamp, CORPS_BATTLE_SCABBLE );
	CBattleMgr::GetSingleton().StartBattle( tKey, tTimeStamp );

	// LOG:同意对方挑战并开战
	LogJoinChaBattle(ORGANLOGTYPE_CORPS, 
		tpCorps->GetCorpsID(), tCharID, tpDesCorps->GetCorpsID(), tpDesCorps->GetRoomMoney(), 0 );

	return ;
}


//////////////////////////////////////////////////////////////////////////
/// 获取所有房间列表
void CCorpservice::OnMessageQueryRoomRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsQueryRoomListRequest* tpReq = ( CMessageCorpsQueryRoomListRequest* ) pMessage->msgpara();

	int tCharID = tpReq->mutable_corphead()->roleid();

	CMessage tMessage;
	CMessageCorpsQueryRoomListResponse tPara;
	tMessage.set_msgpara( (long) &tPara );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYROOM );
	tPara.mutable_corphead()->set_roleid( tCharID );

	int tRoomList[ SERVER_CAP_CORPS ] = { 0 };
	int tRoomNum = ARRAY_CNT( tRoomList );

	CCorpsManager::GetSingleton().OnGetRoomList( tRoomList, tRoomNum );

	for ( int i = 0; i < tRoomNum; i ++ )
	{
		CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tRoomList[ i ] );
		if ( tpCorps == NULL )
			continue;

		PBCorpsRoom* tpRoom = tPara.add_roomlist();
		tpRoom->set_corpsid( tpCorps->GetCorpsID() );
		tpRoom->set_corpsname( tpCorps->GetCorpsName() );
		tpRoom->set_roomname( tpCorps->GetRoomName() );
		tpRoom->set_roomstate( tpCorps->GetRoomState() );
		tpRoom->set_money( tpCorps->GetRoomMoney() );
		tpRoom->set_wintimes( tpCorps->GetVictoryTimes() );
		tpRoom->set_losttimes( tpCorps->GetLoserTimers() );

		if ( tpCorps->GetRoomState() == ROOM_STATE_INWAR )
		{
			CCorps* tpDesCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpCorps->GetEnemyCorpsID() );
			if ( tpDesCorps != NULL )
				tpRoom->set_descorpsname( tpDesCorps->GetCorpsName() );
		}
	}

	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
}

/// 获取自己发起的挑战了列表
void CCorpservice::OnMessageQueryChallegeRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsQueryChallegeListRequest* tpReq = ( CMessageCorpsQueryChallegeListRequest* ) pMessage->msgpara();

	int tCharID = tpReq->mutable_corphead()->roleid();

	//获取军团实例
	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tCharID );
	if ( tpCorps == NULL )
	{
		return ;
	}

	//权限判断
	if ( tpCorps->GetRooterID() != tCharID )
	{
		if ( !tpCorps->MemberHasRight( tCharID, EM_WAR_CHALLENGE ) )
		{
			return ;
		}
	}

	CMessage tMessage;
	CMessageCorpsQueryChallegeListResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYCHALLEGELIST );
	tMessage.set_msgpara( (long) &tPara );
	tPara.mutable_corphead()->set_roleid( tCharID );

	int tChallegeList[ SERVER_CAP_CORPS ] = { 0 };
	int tChallegeNum = ARRAY_CNT( tChallegeList );

	//获取发起的挑战列表
	tpCorps->GetChallegeList< CCorps::ChallegeList >( tpCorps->m_SendChallegeList, tChallegeList, tChallegeNum );

	for ( int i = 0; i < tChallegeNum; i ++ )
	{
		tPara.add_corpsidlist( tChallegeList[ i ] );
	}
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
	return ;
}


// 获取收到的挑战列表
void CCorpservice::OnMessageQueryRecChallegeRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsQueryRecChallegeListRequest* tpReq = ( CMessageCorpsQueryRecChallegeListRequest* ) pMessage->msgpara();

	int tCharID = tpReq->mutable_corphead()->roleid();

	//获取玩家所属军团
	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tCharID );
	if ( tpCorps == NULL )
	{
		return ;
	}

	//权力判断
	if ( tpCorps->GetRooterID() != tCharID )
	{
		if ( !tpCorps->MemberHasRight( tCharID, EM_RIGHT_CHALLENGEROOM ) )
		{
			return ;
		}		
	}

	//是否有房间
	if ( CCorpsManager::GetSingleton().HasRoom( tpCorps->GetCorpsID() ) ==  false )
	{
		return ;
	}

	CMessage tMessage;
	CMessageCorpsQueryRecChallegeListResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYRECCHALLEGE );
	tPara.mutable_corphead()->set_roleid( tCharID );
	tMessage.set_msgpara( (long) &tPara );

	tPara.set_roomname( tpCorps->GetRoomName() );
	tPara.set_money( tpCorps->GetRoomMoney() );
	tPara.set_wintimes( tpCorps->GetVictoryTimes() );
	tPara.set_losttimes( tpCorps->GetLoserTimers() );

	int tChallegeList[ SERVER_CAP_CORPS ] = { 0 };
	int tChallegeNum = ARRAY_CNT( tChallegeList );

	tpCorps->GetChallegeList< CCorps::ChallegeList >( tpCorps->m_ChallegeList, tChallegeList, tChallegeNum );
	for ( int i = 0; i < tChallegeNum; i ++ )
	{
		CCorps* tpChallegeCorps = CCorpsManager::GetSingleton().GetCorpsByID( tChallegeList[ i ] );
		if ( tpChallegeCorps == NULL )
			continue;

		PBChallege* tpChallege = tPara.add_challegelist();
		tpChallege->set_corpsid( tpChallegeCorps->GetCorpsID() );
		tpChallege->set_corpsname( tpChallegeCorps->GetCorpsName() );
		tpChallege->set_wintimes( tpChallegeCorps->GetVictoryTimes() );
		tpChallege->set_losttimes( tpChallegeCorps->GetLoserTimers() );
	}

	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
	return ;
}

void CCorpservice::OnFireCorpsBattleEvent( int nRoleID, int nServerID )
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( nRoleID );
	if ( tpCorps == NULL )
	{
		return ;
	}	

	if ( nServerID == DEFAULT_REPETION_LINE_ID )
	{
		tpCorps->DelBattleRole( nRoleID );
	}

	// TODO:Post Need　Check
	if( tpCorps->MemberHasRight( nRoleID, EM_WAR_CHALLENGE ) )		 // ( tpCorps->GetRooterID() == nRoleID )
	{
		// 如果已经开战了，军团战就不能报名
		if ( tpCorps->GetWarStatus() == STATE_FIGHTING )
		{
			if ( tpCorps->IsInBattleList( nRoleID ) == false )
				return ;
			CRepetionModule::GetSingleton().PlayerJoin( nRoleID, _REPE_CORPS_BATTLE_INDEX_, tpCorps->GetBattleKey(), tpCorps->GetCorpsCamp() );
			return ;
		}

		if ( tpCorps->GetWarStatus() != STATE_SUBMITTING )
		{
			return ;
		}

		CMessage tMessage;
		CMessageCorpsBattleRightsNotify tPara;
		tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSBATTLERIGHTS );
		tPara.mutable_corphead()->set_roleid( nRoleID );
		tMessage.set_msgpara( (long) &tPara );
		tPara.set_npcid( tpCorps->GetBidNpcID() );

		CCorps* tpDesCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpCorps->GetEnemyCorpsID() );
		if ( tpDesCorps != NULL )
		{
			tPara.set_descorpsname( tpDesCorps->GetCorpsName() );
		}

		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
	}
	else
	{
		// 如果没有开战，不能进入战场
		if ( tpCorps->GetWarStatus() != STATE_FIGHTING )
		{
			return ;
		}
		if ( tpCorps->IsInBattleList( nRoleID ) == false )
			return ;
		CRepetionModule::GetSingleton().PlayerJoin( nRoleID, _REPE_CORPS_BATTLE_INDEX_, tpCorps->GetBattleKey(), tpCorps->GetCorpsCamp() );
	}	

	return ;
}
//放弃对城池的占领
void CCorpservice::OnMessageAbortBattleCityRequest( int nServerID, CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsAbortBattleCityRequest* tpReq = ( CMessageCorpsAbortBattleCityRequest* ) pMessage->msgpara();

	int tCharID = tpReq->mutable_corphead()->roleid();
	
	CMessage tMessage;
	CMessageCorpsAbortBattleCityResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ABORTBATTLECITY );
	tPara.mutable_corphead()->set_roleid( tCharID );
	tMessage.set_msgpara( (long) &tPara );

	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tCharID );
	if ( tpCorps == NULL )
	{
		tPara.set_rtncode( ERROR_CORPS_NOTEXIST );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
		return ;
	}
	
	if ( (int)tpReq->mutable_corphead()->roleid() != tpCorps->GetRooterID() )
	{
		if ( !tpCorps->MemberHasRight( tpReq->mutable_corphead()->roleid(), EM_WARNPC_GIVEUP ) )
		{
			tPara.set_rtncode( ERROR_CORPS_HASNORIGHT );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
			return;
		}		
	}

	int tNpcID = tpReq->npcentityid();
	int tGateNpcID = GATENPCID(tNpcID, nServerID);
	if (tpCorps->GetCorpsID() != CBidMgr::GetSingleton().FindNpcOwnerByNpcID(tGateNpcID))
	{
		return ;
	}

	tPara.set_rootername( tpCorps->GetRooterName() ); 
	tPara.set_npcid( tNpcID );
	tPara.set_rtncode( SUCCESS );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );

	CMessage tMsg;
	CMessageCorpsAbortBattleCityNotice tNotify;
	tMsg.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_ABORTBATTLECITY );
	tMsg.set_msgpara( (long) &tNotify );
	tNotify.set_npcid( tNpcID );
	tNotify.set_rootername( tpCorps->GetRooterName() );

	// 放弃的NPC所在阵营ID
	int tCountryID = tpCorps->GetCorpsCountryID();

	CPlayerManager::GetSingleton().SendCountryMessage( &tMsg, tpCorps->GetCorpsCountryID() );
	//SendCorpsMsgByID( tpCorps->GetCorpsID(), &tMsg );
	int tMapID = tpCorps->GetMap( tNpcID );
	tpCorps->ClearNpcBattleInfo( tNpcID ); 

	CBidMgr::GetSingleton().SetNpcOwner(GATENPCID( tNpcID, tCountryID ), 0);
	
	// LOG:军团放弃对城池的占领
	LogAbort( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpCorps->GetRooterID(), tNpcID, 0, 0 );
	
	if ( IS_MAIN_CITY( tMapID ) )
	{
		tpCorps->SetBattleCity( 0 );
		CWorldManager::GetSingleton().LoadCorpsFromMap( tCountryID, tMapID, 0 );
		OnEventCityUpgrade( tpCorps, tCountryID );
	}

	//CCorpsManager::GetSingleton().OnDeleteBattleCity( GATENPCID( tMapID, tCountryID ) );
	
	CCorpsManager::GetSingleton().SendCorpsNpcSingle( tCountryID, tNpcID, tpCorps->GetCorpsID(), false );
	return ;
}

// ***********************************************************
//  Function:		SendNotifyCorpsTitleChanged
//  Description:	发送称号被改变的通知
//  Output:			
//  Date:	11/04/2009
// **********************************************************
void CCorpservice::SendNotifyCorpsTitleChanged( CGatePlayer *pPlayer, int nManagerID, const char *pManagerName, const char *pCorpsTitle )
{ 
	LK_ASSERT( pPlayer != NULL, return )	
	CMessage tMsgHead;
	CMessageCorpsNameChangedNotify tMsgCorpsNameChanged;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSNAMECHANGED );
	tMsgHead.set_msgpara( (int)&tMsgCorpsNameChanged );
	
	tMsgCorpsNameChanged.mutable_corphead()->set_roleid( pPlayer->mCharID );
	tMsgCorpsNameChanged.set_managerid( nManagerID );	
	if( pCorpsTitle != NULL )
	{
		tMsgCorpsNameChanged.set_corpsname( pCorpsTitle );
	}  
	
	if ( pManagerName != NULL )
	{
		tMsgCorpsNameChanged.set_managername( pManagerName );
	}
	
	tMsgCorpsNameChanged.set_memberid( pPlayer->mCharID );
	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// 申请加入联军的请求消息
void CCorpservice::OnMessageApplyUnion( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageApplyUnionRequest *tpMsg = ( CMessageApplyUnionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return );
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find player(CGatePlayer) of roleid:%d ", tpHead->roleid() );
		return;	
	}

	CUnion *tpUnion = NULL;
	if ( tpMsg->has_unionid() )
	{
		tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tpMsg->unionid() );
	}
	else if ( tpMsg->has_unionname() )
	{
		LK_ASSERT( tpMsg->mutable_unionname()->length() < MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES , return ) 		
			tpUnion = CCorpsManager::GetSingleton().GetUnionByName( tpMsg->mutable_unionname()->c_str() );
	}
	else 
	{
		tpUnion = CCorpsManager::GetSingleton().GetUnionOfPlayer( tpMsg->roleid() );
	}	
	if ( tpUnion == NULL )
	{
		SendResponseApplyUnion( nServerID, tpHead->roleid(), ERROR_UNION_APPLY_NOTEXIST, 0, NULL, tpHead->entityid() );  
		return;
	}	
	CCorps *pHeadCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpUnion->GetCoreID( ) );
	if ( pHeadCorps == NULL )
	{
		SendResponseApplyUnion( nServerID, tpHead->roleid(), ERROR_UNION_APPLY_NOTEXIST, 0, NULL, tpHead->entityid() );  
		return;
	}
	PBCorpsBasicInfo tInfo;
	int tRet = CCorpsManager::GetSingleton().OnEventApplyUnion( tpHead->roleid(), tpUnion, tInfo );
	if ( tRet == 0 )
	{
		// 获取军团中有权限的职位最高的在线管理者		
		CGatePlayer *tpManager = CCorpsManager::GetSingleton().GetManagerOfRight( pHeadCorps, EM_APPROVE_APPLY );
		if ( tpManager != NULL )
		{
			SendNotifyUnionMemberApply( tpManager, tpManager->mCharID, tInfo );	
		}
	}
	SendResponseApplyUnion( nServerID, tpHead->roleid(), tRet, 0, pHeadCorps->GetCorpsName(), tpHead->entityid() );
}

// 回复军团申请加入联军的消息请求
void CCorpservice::OnMessageReplyUnionApply( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return )		
	CMessageReplyUnionMemberApplyRequest *tpMsg = ( CMessageReplyUnionMemberApplyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return );
	CorpsHead *tpHead = tpMsg->mutable_corphead();	
	CGatePlayer *tpReplyingPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( tpReplyingPlayer == NULL )
	{
		LOG_ERROR( "corps", " can not find player info( tpReplyingPlayer ) ( roleid:%d ) ", tpHead->roleid() );
		return;
	}

	PBCorpsBasicInfo tCorpsInfo;
	int tRet = CCorpsManager::GetSingleton().OnEventReplyUnionApply( tpHead->roleid(), tpMsg->corpsid(), tpMsg->result(), tCorpsInfo );
	if ( tRet == -1 )
	{
		return;
	}
	
	CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionOfPlayer( tpHead->roleid() );
	SendResponseReplyApplyUnion( nServerID, tpHead->roleid(), tRet, tpMsg->result(), tpMsg->corpsid() );

	// 如果加入成功，发送军团通知
	if ( tRet == SUCCESS && tpMsg->result() == 1 )
	{		
		if ( tpUnion != NULL )
		{
			// 给军团成员发送加入军团的消息通知
			int tCorpsList[ MAX_CORPS_UNION ] = { 0 };
			int tNumber = ARRAY_CNT( tCorpsList );
			tpUnion->GetCorpsList( tCorpsList, tNumber );
			for ( int i = 0; i < tNumber; i++ )
			{
				CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tCorpsList[ i ] );
				int tCorpsMemberIDList[CORPS_MAXMEMBERNUM] = { 0 };
				int tNum = CORPS_MAXMEMBERNUM;
				CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tCorpsMemberIDList, tNum );
				SendNotifyCorpsJoinUnion( tCorpsInfo, tpUnion->GetUnionID( ), tpUnion->GetUnionName( ), tCorpsMemberIDList, tNum );
			}					
		}
	}
	if ( tRet == SUCCESS )
	{
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid() );
		if ( tpCorps != NULL && tpUnion != NULL )
		{
			SendNotifyApplyUnion( nServerID, tpCorps->GetRooterID(), tpMsg->result(), tpUnion->GetUnionID(), tpUnion->GetUnionName() );
		}		
	}		
}

// 邀请军团加入联军的消息请求
void CCorpservice::OnMessageInviteUnionMember( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageInviteUnionMemberRequest *tpMsg = ( CMessageInviteUnionMemberRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();

	CGatePlayer *tInvitingPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( tInvitingPlayer == NULL )
	{
		LOG_ERROR( "corps", " can not find corps info of player (roleid:%d) ", tpHead->roleid() );
		return;
	}

	CCorps *tpInviteCorps = NULL;
	if( tpMsg->has_corpsname() )
	{
		LK_ASSERT( tpMsg->mutable_corpsname()->length() < MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES, return );		
		tpInviteCorps = CCorpsManager::GetSingleton().GetCorpsByName( tpMsg->corpsname().c_str() );
	}
	else
	{
		tpInviteCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpMsg->memberid() );
	}	
	if ( tpInviteCorps == NULL )
	{
		SendResponseInviteUnionMember( nServerID, tpHead->roleid(), ERROR_UNION_INVITE_CORPSNOTEXIST, NULL, tpHead->entityid() );
		return;
	}

	int tRet = CCorpsManager::GetSingleton().OnEventInviteCorps( tpHead->roleid(), tpInviteCorps );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseInviteUnionMember( nServerID, tpHead->roleid(), tRet, tpInviteCorps->GetCorpsName(), tpHead->entityid() );

	if ( tRet == SUCCESS )
	{
		CGatePlayer *tpInvitedPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpInviteCorps->GetRooterID() );
		if ( tpInvitedPlayer != NULL )
		{
			CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionOfPlayer( tpHead->roleid() );
			LK_ASSERT( tpUnion != NULL, return );
			PBUnionBasicInfo tUnionInfo;
			tpUnion->GetUnionBasicInfo( &tUnionInfo );
			SendNotifyUnionInvite( tpInvitedPlayer, tpInvitedPlayer->mCharID, tUnionInfo, tInvitingPlayer->mCharID, tInvitingPlayer->mCharName );
		}		
	}
}

// 答复联军邀请的消息请求
void CCorpservice::OnMessageReplyUnionInvite( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageReplyUnionInviteRequest *tpMsg = ( CMessageReplyUnionInviteRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );	
	LK_ASSERT( tpMsg->has_corphead(), return );
	CorpsHead *tpHead = tpMsg->mutable_corphead( );	 
	CGatePlayer *tpReplyingPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( tpReplyingPlayer == NULL )
	{								
		LOG_ERROR( "corps"," can not find corps of player( roleid::%d ) ", tpHead->roleid() );
		return;
	}

	CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tpMsg->unionid() );
	if ( tpUnion == NULL )
	{
		// 发送军团不存在的回复信息
		SendResponseReplyCorpsInvite( nServerID, tpHead->roleid(), ERROR_UNION_REPLYINVITE_NOTEXIST, tpMsg->result(), tpMsg->unionid(), NULL, tpHead->entityid() );
		return;
	}

	int tRet = CCorpsManager::GetSingleton().OnEventReplyUnionInvite( tpHead->roleid(), tpUnion, tpMsg->result() );
	if ( tRet == -1 )
	{
		return;
	}

	// 发送答复结果 
	SendResponseReplyUnionInvite( nServerID, tpHead->roleid(), tRet, tpMsg->result(), tpUnion->GetUnionID(), tpUnion->GetUnionName(), tpHead->entityid() );	

	// 同意加入军团并且加入成功
	if ( tRet == SUCCESS && tpMsg->result() == 1 )
	{
		// 给军团成员发送加入军团的消息通知
		int tCorpsList[ MAX_CORPS_UNION ] = { 0 };
		int tNumber = ARRAY_CNT( tCorpsList );
		tpUnion->GetCorpsList( tCorpsList, tNumber );
		CCorps *tpInvitedCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
		PBCorpsBasicInfo tCorpsInfo;
		if ( tpInvitedCorps != NULL )
		{
			tpInvitedCorps->GetCorpsBasicInfo( tCorpsInfo );
		}
		for ( int i = 0; i < tNumber; i++ )
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tCorpsList[ i ] );
			int tCorpsMemberIDList[CORPS_MAXMEMBERNUM] = { 0 };
			int tNum = CORPS_MAXMEMBERNUM;
			CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tCorpsMemberIDList, tNum );				
			SendNotifyCorpsJoinUnion( tCorpsInfo, tpUnion->GetUnionID( ), tpUnion->GetUnionName( ), tCorpsMemberIDList, tNum );
		}
	}
	if ( tpMsg->result() != 1 )
	{
		CCorps *tpHeadCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpUnion->GetCoreID() );
		CCorps *tpInvitedCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
		if ( tpHeadCorps != NULL && tpInvitedCorps != NULL )
		{
			SendNotifyUnionInviteResult( tpHeadCorps->GetRooterID(), tpMsg->result(), tpInvitedCorps->GetCorpsName() );
		}		
	}
}

// 军团离开联军的消息请求	
void CCorpservice::OnMessageLeaveUnion( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageCorpsLeaveUnionRequest *tpMsg = ( CMessageCorpsLeaveUnionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return );
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	
	CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionOfPlayer( tpHead->roleid() );
	if ( tpUnion == NULL )
	{
		LOG_ERROR( "corps"," can not find union of player(roleid:%d) ", tpHead->roleid() );
		return;
	}

	int tRet = CCorpsManager::GetSingleton().OnEventLeaveUnion( tpHead->roleid() );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseLeaveUnion( nServerID, tpHead->roleid(), tRet, tpHead->entityid() );		
	if ( tRet == SUCCESS )
	{
		CCorps *tpLeaveCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );
		if ( tpLeaveCorps == NULL )
		{
			LOG_ERROR( "corps"," can not find corps of player(roleid:%d) ", tpHead->roleid() );
			return;
		}
		int tCorpsMemberIDList[CORPS_MAXMEMBERNUM] = { 0 };
		int tNum = CORPS_MAXMEMBERNUM;

		int tCorpsList[ MAX_CORPS_UNION ] = { 0 };
		int tNumber = ARRAY_CNT( tCorpsList );
		tpUnion->GetCorpsList( tCorpsList, tNumber );
		for ( int i = 0; i < tNumber; i++ )
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tCorpsList[ i ] );
			if ( tpCorps == NULL )
				continue;
			
			memset( tCorpsMemberIDList, 0, ARRAY_CNT(tCorpsMemberIDList) );
			tNum = CORPS_MAXMEMBERNUM;
			CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tCorpsMemberIDList, tNum );
			SendNotifyCorpsLeaved( tpLeaveCorps->GetCorpsID(), tpLeaveCorps->GetCorpsName(), tCorpsMemberIDList, tNum );
		}

		memset( tCorpsMemberIDList, 0, ARRAY_CNT(tCorpsMemberIDList) );
		tNum = CORPS_MAXMEMBERNUM;
		CCorpsManager::GetSingleton().GetCorpsMemberList( tpLeaveCorps, tCorpsMemberIDList, tNum, 0, tpHead->roleid() ); 
		SendNotifyCorpsLeaved( tpLeaveCorps->GetCorpsID(), tpLeaveCorps->GetCorpsName(), tCorpsMemberIDList, tNum );
	}
}

// 从联军踢出军团的消息请求
void CCorpservice::OnMessageKickCorps( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageKickCorpsRequest *tpMsg = ( CMessageKickCorpsRequest *)pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return );
	CorpsHead *tpHead = tpMsg->mutable_corphead();

	CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionOfPlayer( tpHead->roleid() );
	if ( tpUnion == NULL )
	{
		LOG_ERROR( "corps"," can not find union of player(roleid:%d) ", tpHead->roleid() );
		return;
	}
	int tRet = CCorpsManager::GetSingleton().OnEventKickCorpsOfUnion( tpHead->roleid(), tpMsg->corpsid() );
	if ( tRet == -1 )
	{
		return;
	}
	const char *tpCorpsName = NULL;
	CCorps *tpKickedCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid() );
	if ( tpKickedCorps != NULL )
	{
		tpCorpsName = tpKickedCorps->GetCorpsName();
	}
	SendResponseKickCorps( nServerID, tpHead->roleid(), tRet, tpMsg->corpsid(), tpCorpsName, tpHead->entityid() );

	if ( tRet == SUCCESS )
	{
		// 发送家族被踢掉的消息通知
		int tCorpsMemberIDList[CORPS_MAXMEMBERNUM] = { 0 };
		int tNum = CORPS_MAXMEMBERNUM;

		int tCorpsList[ MAX_CORPS_UNION ] = { 0 };
		int tNumber = ARRAY_CNT( tCorpsList );
		tpUnion->GetCorpsList( tCorpsList, tNumber );
		for ( int i = 0; i < tNumber; i++ )
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tCorpsList[ i ] );
			if ( tpCorps == NULL )
				continue;

			memset( tCorpsMemberIDList, 0, ARRAY_CNT(tCorpsMemberIDList) );
			tNum = CORPS_MAXMEMBERNUM;
			CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tCorpsMemberIDList, tNum );
			SendNotifyCorpsKicked( tpKickedCorps->GetCorpsID(), tpKickedCorps->GetCorpsName(), tCorpsMemberIDList, tNum );
		}

		memset( tCorpsMemberIDList, 0, ARRAY_CNT(tCorpsMemberIDList) );
		tNum = CORPS_MAXMEMBERNUM;
		CCorpsManager::GetSingleton().GetCorpsMemberList( tpKickedCorps, tCorpsMemberIDList, tNum, 0, tpHead->roleid() ); 
		SendNotifyCorpsKicked( tpKickedCorps->GetCorpsID(), tpKickedCorps->GetCorpsName(), tCorpsMemberIDList, tNum );
	}
}

// 申请加入联军的回应消息
void CCorpservice::SendResponseApplyUnion( int nServerID, int nRoleID, int nRet, int nUnionID, const char *pUnionName, int nEntityID )
{
	CMessage tMsgHead;
	CMessageApplyUnionResponse tMsgApply;

	tMsgApply.mutable_corphead()->set_roleid( nRoleID );
	if( nEntityID == 0 )
	{
		tMsgApply.mutable_corphead()->set_entityid( nEntityID );
	}

	tMsgApply.set_errcode( nRet );
	if ( pUnionName != NULL )
	{
		tMsgApply.set_unionname( pUnionName );
	}

	tMsgHead.set_msgpara( (int)&tMsgApply );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_APPLYUNION );

	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

void CCorpservice::SendResponseReplyApplyUnion( int nServerID, int nRoleID, int nRet, int nResult, int nCorpsID )
{
	CMessage tMsgHead;
	CMessageReplyUnionMemberApplyResponse tMsgApply;

	tMsgApply.mutable_corphead()->set_roleid( nRoleID );

	tMsgApply.set_errcode( nRet );
	tMsgApply.set_result( nResult );
	tMsgApply.set_corpsid( nCorpsID );

	tMsgHead.set_msgpara( (int)&tMsgApply );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REPLYUNIONMEMBERAPPLY );

	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// 有军团申请加入联军的消息通知	
void CCorpservice::SendNotifyUnionMemberApply( CGatePlayer *pPlayer, int nRoleID, PBCorpsBasicInfo vInfo )
{
	LK_ASSERT( pPlayer != NULL, return );
	CMessage tMsgHead;
	CMessageUnionMemberApplyNotify tMsgMemberApply;

	tMsgMemberApply.mutable_corphead()->set_roleid( nRoleID );
	tMsgMemberApply.mutable_corpsinfo()->CopyFrom( vInfo );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_UNIONMEMBERAPPLY );
	tMsgHead.set_msgpara( (int)&tMsgMemberApply );

	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// 有军团加入联军的消息通知	 	
void CCorpservice::SendNotifyCorpsJoinUnion( PBCorpsBasicInfo CorpsInfo, int vUnionID, char *vUnionName, int *pRoleIDList, int vRoleNum )
{
	CMessage tMsgHead;
	CMessageCorpsJoinUnionNotify tMsgJoin;

	tMsgJoin.mutable_corpsinfo()->CopyFrom( CorpsInfo ); 
	tMsgJoin.set_unionid( vUnionID );
	if ( vUnionName != NULL )
	{
		tMsgJoin.set_unionname( vUnionName );
	}	
	tMsgJoin.mutable_corphead()->set_roleid( 0 );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSJOINUNION );	
	tMsgHead.set_msgpara( (int)&tMsgJoin );

	SendCorpsNotifyMsg( &tMsgHead, pRoleIDList, vRoleNum );
}

// 邀请军团加入联军的消息回应	
void CCorpservice::SendResponseInviteUnionMember( int nServerID, int nRoleID, int nRet, const char *pCorpsName, int nEntityID )
{
	CMessage tMsgHead;
	CMessageInviteUnionMemberResponse tMsgInvite;	

	tMsgInvite.mutable_corphead()->set_roleid( nRoleID );
	tMsgInvite.mutable_corphead()->set_entityid( nEntityID );
	if ( pCorpsName != NULL )
	{
		tMsgInvite.set_corpsname( pCorpsName );
	}
	tMsgInvite.set_errcode( nRet );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_INVITEUNIONMEMBER );					
	tMsgHead.set_msgpara( (int)&tMsgInvite );

	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// 答复联军邀请的消息回复
void CCorpservice::SendResponseReplyUnionInvite( int nServerID, int nRoleID, int nRet, int nResult, int nUnionID, const char *pUnionName, int nEntityID )
{
	CMessage tMsgHead;
	CMessageReplyUnionInviteResponse tMsgReply;

	tMsgReply.mutable_corphead()->set_roleid( nRoleID );	
	tMsgReply.mutable_corphead()->set_entityid( nEntityID );

	tMsgReply.set_result( nResult );
	tMsgReply.set_errcode( nRet );
	tMsgReply.set_unionid( nUnionID );
	if ( pUnionName != NULL )
	{
		tMsgReply.set_unionname( pUnionName );
	}

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REPLYUNIONINVITE );
	tMsgHead.set_msgpara( (int)&tMsgReply );

	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
}

void CCorpservice::SendResponseLeaveUnion( int nServerID, int nRoleID, int nRet, int nEntityID )
{
	CMessage tMsgHead;
	CMessageCorpsLeaveUnionResponse tMsgLeave;

	tMsgLeave.mutable_corphead()->set_roleid( nRoleID );
	tMsgLeave.mutable_corphead()->set_entityid( nEntityID );	
	tMsgLeave.set_errcode( nRet );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_LEAVEUNION );
	tMsgHead.set_msgpara( (int)&tMsgLeave );

	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

void CCorpservice::SendNotifyCorpsLeaved( int nCorpsID, const char *pCorpsName, int *pRoleIDList, int nRoleNum )
{ 
	CMessage tMsgHead;
	CMessageCorpsLeaveUnionNotify tMsgLeaved;	

	tMsgLeaved.set_corpsid( nCorpsID );
	if ( pCorpsName != NULL )
	{
		tMsgLeaved.set_corpsname( pCorpsName );
	}
	tMsgLeaved.mutable_corphead()->set_roleid( 0 );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSLEAVED );
	tMsgHead.set_msgpara( (int)&tMsgLeaved );

	SendCorpsNotifyMsg( &tMsgHead, pRoleIDList, nRoleNum );
}

void CCorpservice::SendResponseKickCorps( int nServerID, int nRoleID, int nRet, int nCorpsID, const char *pCorpsName, int nEntityID )
{
	CMessage tMsgHead;
	CMessageKickCorpsResponse tMsgKick;

	tMsgKick.mutable_corphead()->set_roleid( nRoleID );	
	tMsgKick.mutable_corphead()->set_entityid( nEntityID );
	tMsgKick.set_corpsid( nCorpsID );
	if ( pCorpsName != NULL )
	{
		tMsgKick.set_corpsname( pCorpsName );
	}
	tMsgKick.set_errcode( nRet );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_KICKCORPS );	
	tMsgHead.set_msgpara( (int)&tMsgKick );

	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

void CCorpservice::SendNotifyCorpsKicked( int nCorpsID, const char * pCorpsName, int *pRoleIDList, int tNum )
{	
	CMessage tMsgHead;
	CMessageKickCorpsNotify tMsgKick;

	tMsgKick.set_corpsid( nCorpsID );
	if( pCorpsName != NULL )
	{		
		tMsgKick.set_corpsname( pCorpsName );
	}
	tMsgKick.mutable_corphead()->set_roleid( 0 );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSKICKED );
	tMsgHead.set_msgpara( (int)&tMsgKick );

	SendCorpsNotifyMsg( &tMsgHead, pRoleIDList, tNum );
}	

void CCorpservice::SendNotifyUnionInvite( CGatePlayer *pPlayer, int nRoleID, PBUnionBasicInfo &pbCorpsInfo, int nInvitingID, const char * pInvitingName )
{	
	LK_ASSERT( pPlayer != NULL, return );
	CMessage tMsgHead;
	CMessageUnionInviteNotify tMsgInvite;

	tMsgInvite.mutable_corphead()->set_roleid( nRoleID );
	tMsgInvite.mutable_unioninfo()->CopyFrom( pbCorpsInfo );
	tMsgInvite.set_invitingid( nInvitingID );
	if ( pInvitingName != NULL )
	{
		tMsgInvite.set_invitingname( pInvitingName );
	}

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_UNIONINVITE );
	tMsgHead.set_msgpara( (int)&tMsgInvite );

	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// 创建联军请求
void CCorpservice::OnMessageCreateUnion( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageCreateUnionRequest *tpMsg = ( CMessageCreateUnionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return );
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find player(CGatePlayer) of roleid:%d ", tpHead->roleid() );
		return;	
	}

	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );	
	LK_ASSERT( tpCorps != NULL, return );
	if ( tpCorps->GetRooterID() != ( int )tpHead->roleid() )
	{
		return;
	}

	int tCorpsID[ MAX_CORPS_UNION ] = { 0 };
	int tNum = 0;
	for ( int i = 0; i < (int)ARRAY_CNT(tpCorps->m_OtherCorpsID); i++ )
	{
		if ( tpCorps->m_OtherCorpsID[ i ] == 0 )
		{
			break;
		}
		tCorpsID[ i ] = tpCorps->m_OtherCorpsID[ i ];
		tNum++;
	}

	int tRet = CCorpsManager::GetSingleton().OnEventCreateUnion( tpCorps->GetCorpsID(), tCorpsID, tNum, (char*)tpCorps->GetTmpUnionName(), (char*)tpCorps->GetTmpUnionNotice(), tpHead->roleid() );
	if ( tRet != SUCCESS )
	{
		SendCreateUnionResponse( tpPlayer, tpHead->roleid(), tRet, 0 );
	}
}

void CCorpservice::OnMessageMakeUnion( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageMakeUnionRequest *tpMsg = ( CMessageMakeUnionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return );
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpSrcPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if( tpSrcPlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find player(CGatePlayer) of roleid:%d ", tpHead->roleid() );
		return;	
	}

	CCorps *tpSrcCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );	
	LK_ASSERT( tpSrcCorps != NULL, return );
	if ( tpSrcCorps->GetRooterID() != ( int )tpHead->roleid() )
	{
		SendMakeUnionResponse( tpSrcPlayer, tpHead->roleid(), ERROR_UNION_MAKE_SELFNOTHEADER, tpMsg->optype() );
		return;
	}

	if ( tpSrcCorps->GetUnionID() != 0 )
	{
		SendMakeUnionResponse( tpSrcPlayer, tpHead->roleid(), ERROR_UNION_MAKE_INUNION, tpMsg->optype() );
		return;
	}

	// 查询不需要目标
	if ( tpMsg->optype() == EM_CORPS_MAKE_INFO )
	{
		SendMakeUnionResponse( tpSrcPlayer, tpHead->roleid(), SUCCESS, tpMsg->optype() );
		SendMakeUnionNotify( tpSrcPlayer, tpHead->roleid(), tpMsg->optype(), tpSrcCorps );
		return;
	}

	CGatePlayer *tpDesPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if( tpDesPlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find player(CGatePlayer) of roleid:%d ", tpHead->roleid() );
		return;	
	}
	CCorps *tpDesCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpMsg->targetid() );	
	LK_ASSERT( tpDesCorps != NULL, return );
	if ( tpDesCorps->GetRooterID() != ( int )tpMsg->targetid() )
	{
		SendMakeUnionResponse( tpSrcPlayer, tpHead->roleid(), ERROR_UNION_MAKE_OTHERNOTHEADER, tpMsg->optype() );
		return;
	}

	if ( tpSrcCorps->GetCorpsID() == tpDesCorps->GetCorpsID() )
	{
		SendMakeUnionResponse( tpSrcPlayer, tpHead->roleid(), ERROR_UNION_MAKE_INVITESELF, tpMsg->optype() );
		return;
	}

	if ( tpDesCorps->GetUnionID() != 0 )
	{
		SendMakeUnionResponse( tpSrcPlayer, tpHead->roleid(), ERROR_UNION_MAKE_INUNION, tpMsg->optype() );
		return;
	}	

	if ( tpSrcCorps->GetCorpsStatus() == CCorps::EM_CORPS_STATUS_DISBAND || tpDesCorps->GetCorpsStatus() == CCorps::EM_CORPS_STATUS_DISBAND )
	{
		SendMakeUnionResponse( tpSrcPlayer, tpHead->roleid(), ERROR_UNION_MAKE_DISBAND, tpMsg->optype() );
		return;
	}

	if ( tpMsg->optype() == EM_CORPS_MAKE_INVITE )
	{
		if ( tpDesCorps->GetInviteIdle() == false )
		{
			SendMakeUnionResponse( tpSrcPlayer, tpHead->roleid(), ERROR_UNION_MAKE_INVITEBUSY, tpMsg->optype() );
			return;
		}	
		if ( tpDesCorps->IsOwner( ) == true )
		{
			SendMakeUnionResponse( tpSrcPlayer, tpHead->roleid(), ERROR_UNION_MAKE_ISOWNER, tpMsg->optype() );
			return;
		}
		if ( tpDesCorps->IsInBattle(TYPE_BATTLE_SCRABBLE, ST_Single|ST_InBid|ST_WaitBtl|ST_InBtl|ST_CityMap) )
		{
			//  不可邀请处于主城NPC竞价、战争等待、战争中时的军团
			SendMakeUnionResponse( tpSrcPlayer, tpHead->roleid(), ERROR_UNOIN_INVITE_CITY_CONTENDER, tpMsg->optype() );
			return;
		}
		if ( tpMsg->unionname().length() == 0 || tpMsg->unionname().length() >= MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES
		  || tpMsg->unionnotice().length() == 0 || tpMsg->unionname().length() >= CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES )
		{
			SendMakeUnionResponse( tpSrcPlayer, tpHead->roleid(), ERROR_UNION_MAKE_NAMEERR, tpMsg->optype() );
			return;
		}
		tpSrcCorps->SetTmpUnionName( tpMsg->unionname().c_str() );
		tpSrcCorps->SetTmpUnionNotice( tpMsg->unionnotice().c_str() );
	}

	if ( tpMsg->optype() == EM_CORPS_MAKE_CLOSE )
	{
		tpDesCorps->SetInviteIdle( true );
	}

	SendMakeUnionResponse( tpSrcPlayer, tpHead->roleid(), SUCCESS, tpMsg->optype() );
	
	if ( tpMsg->optype() == EM_CORPS_MAKE_SIGN )
	{
		tpDesCorps->AddTmpCorpsID( tpSrcCorps->GetCorpsID( ) );
		tpSrcCorps->SetInviteIdle( false );
		SendMakeUnionNotify( tpSrcPlayer, tpHead->roleid(), tpMsg->optype(), tpDesCorps );
		SendMakeUnionNotify( tpDesPlayer, tpMsg->targetid(), tpMsg->optype(), tpDesCorps );
		return;
	}
	
	SendMakeUnionNotify( tpSrcPlayer, tpHead->roleid(), tpMsg->optype(), tpSrcCorps );
	SendMakeUnionNotify( tpDesPlayer, tpMsg->targetid(), tpMsg->optype(), tpSrcCorps );
}

// 发送创建联军回应
void CCorpservice::SendMakeUnionResponse( CGatePlayer *pPlayer, int vRoleID, int vResult, int vOpType )
{
	LK_ASSERT( pPlayer != NULL, return );
	CMessage tMsgHead;
	CMessageMakeUnionResponse tMsgResponse;

	tMsgResponse.mutable_corphead()->set_roleid( vRoleID );
	tMsgResponse.set_errcode( vResult );
	tMsgResponse.set_optype( vOpType );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_MAKEUNION );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// 发送创建联军通知
void CCorpservice::SendMakeUnionNotify( CGatePlayer *pPlayer, int vRoleID, int vOpType, CCorps *pCorps )
{
	LK_ASSERT( pPlayer != NULL && pCorps != NULL, return );
	CMessage tMsgHead;
	CMessageMakeUnionNotify tMsgNotify;

	tMsgNotify.mutable_corphead()->set_roleid( vRoleID );
	tMsgNotify.set_optype( vOpType );
	tMsgNotify.set_unionname( pCorps->GetTmpUnionName( ) );
	tMsgNotify.set_unionnotice( pCorps->GetTmpUnionNotice( ) );
	for ( int i = 0; i < ( int )ARRAY_CNT(pCorps->m_OtherCorpsID); i++ )
	{
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( pCorps->m_OtherCorpsID[ i ] );
		if ( tpCorps == NULL )
			break;
		tMsgNotify.add_corpsid( tpCorps->GetCorpsID() );
		tMsgNotify.add_corpsname( tpCorps->GetCorpsName() );
	}
	tMsgNotify.set_headcorpsname( pCorps->GetCorpsName() );
	tMsgNotify.set_headcharid( pCorps->GetRooterID() );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MAKEUNION );
	tMsgHead.set_msgpara( (int)&tMsgNotify );

	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// 发送创建联军回应
void CCorpservice::SendCreateUnionResponse( CGatePlayer *pPlayer, int vRoleID, int vResult, int vUnionID )
{
	LK_ASSERT( pPlayer != NULL, return );
	CMessage tMsgHead;
	CMessageCreateUnionResponse tMsgResponse;

	tMsgResponse.mutable_corphead()->set_roleid( vRoleID );
	tMsgResponse.set_result( vResult );
	tMsgResponse.set_unionid( vUnionID );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CREATEUNION );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// 发送创建联军通知
void CCorpservice::SendCreateUnionNotify( int vServerID, int vRoleID, CUnion *pUnion )
{
	LK_ASSERT( pUnion != NULL, return );
	CMessage tMsgHead;
	CMessageCreateUnionNotify tMsgNotify;

	tMsgNotify.mutable_corphead()->set_roleid( vRoleID );
	tMsgNotify.set_unionid( pUnion->GetUnionID() );
	tMsgNotify.set_unionname( pUnion->GetUnionName() );

	CCorps *pHeadCorps = CCorpsManager::GetSingleton().GetCorpsByID( pUnion->GetCoreID() );
	if ( pHeadCorps != NULL )
	{
		tMsgNotify.set_headid( pHeadCorps->GetCorpsID() );
		tMsgNotify.set_headname( pHeadCorps->GetCorpsName() );
	}

	for ( CUnion::CorpsVector::iterator it = pUnion->mCorpsList.begin(); it != pUnion->mCorpsList.end(); ++it )
	{
		CCorps *pCorps = CCorpsManager::GetSingleton().GetCorpsByID( *it );
		if ( pCorps != NULL )
		{
			tMsgNotify.add_otherid( pCorps->GetCorpsID() );
			tMsgNotify.add_othername( pCorps->GetCorpsName() );
		}
	}

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CREATEUNION );	
	tMsgHead.set_msgpara( (int)&tMsgNotify );

	//CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
	CPlayerManager::GetSingleton().SendCountryMessage( &tMsgHead, vServerID );
}

// 获得联军的请求消息
void CCorpservice::OnMessageGetUnionInfo( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageGetUnionInfoRequest *tpMsg = ( CMessageGetUnionInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return );
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find player(CGatePlayer) of roleid:%d ", tpHead->roleid() );
		return;	
	}

	CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tpMsg->unionid() );
		
	if ( tpUnion == NULL )
	{
		//SendGetUnionInfoResponse( tpPlayer, tpHead->roleid(), ERROR_UNION_INFO_NOTEXIST, tpUnion );  
		return;
	}	
	
	SendGetUnionInfoResponse( tpPlayer, tpHead->roleid(), SUCCESS, tpUnion ); 
}

// 发送获得联军回应
void CCorpservice::SendGetUnionInfoResponse( CGatePlayer *pPlayer, int vRoleID, int vResult, CUnion *pUnion )
{
	LK_ASSERT( pPlayer != NULL, return );
	CMessage tMsgHead;
	CMessageGetUnionInfoResponse tMsgResponse;

	tMsgResponse.mutable_corphead()->set_roleid( vRoleID );
	tMsgResponse.set_errcode( vResult );

	if ( vResult == SUCCESS && pUnion != NULL )
	{
		PBUnionBasicInfo tUnionInfo;		
		pUnion->GetUnionBasicInfo( &tUnionInfo );
		tMsgResponse.mutable_unioninfo()->CopyFrom( tUnionInfo );
		PBCorpsBattleList* tpBattleList = tMsgResponse.mutable_battleinfo();
		CCorps *pHeadCorps = CCorpsManager::GetSingleton().GetCorpsByID( pUnion->GetCoreID() );
		if ( pHeadCorps != NULL )
		{
			PBCorpsBasicInfo *tCorpsInfo = tMsgResponse.add_corpsinfo( );
			pHeadCorps->GetCorpsBasicInfo( *tCorpsInfo );
			pHeadCorps->GetPBBuffer( tpBattleList, false );
		}
		for ( CUnion::CorpsVector::iterator it = pUnion->mCorpsList.begin(); it != pUnion->mCorpsList.end(); ++it )
		{
			CCorps *pCorps = CCorpsManager::GetSingleton().GetCorpsByID( *it );
			if ( pCorps != NULL )
			{				
				PBCorpsBasicInfo *tCorpsInfo = tMsgResponse.add_corpsinfo( );
				pCorps->GetCorpsBasicInfo( *tCorpsInfo );
				pCorps->GetPBBuffer( tpBattleList, false );
			}
		}
	}
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_UNIONINFO );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// 解散联军的请求消息
void CCorpservice::OnMessageDismissUnion( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageDismissUnionRequest *tpMsg = ( CMessageDismissUnionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return );
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find player(CGatePlayer) of roleid:%d ", tpHead->roleid() );
		return;	
	}

	int tCorpsID[ MAX_CORPS_UNION ] = { 0 };
	int tNum = 1;	

	CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionOfPlayer( tpHead->roleid() );	

	if ( tpUnion != NULL )
	{
		tCorpsID[ 0 ] = tpUnion->GetCoreID( );
		for ( CUnion::CorpsVector::iterator it = tpUnion->mCorpsList.begin(); it != tpUnion->mCorpsList.end() && tNum < ( int )ARRAY_CNT(tCorpsID); ++it )
		{
			tCorpsID[ tNum++ ] = *it;
		}
	}

	int tRet =  CCorpsManager::GetSingleton().OnEventDisbandUnion( tpHead->roleid() );	

	SendDismissUnionResponse( tpPlayer, tpHead->roleid(), tRet ); 
	if ( tRet == SUCCESS )
	{
		SendDismissUnionNotify( nServerID, tpHead->roleid(), tCorpsID, tNum );
	}	
}

// 获得联军申请列表的请求消息
void CCorpservice::OnMessageUnionApplyList( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageUnionApplyListRequest *tpMsg = ( CMessageUnionApplyListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return );
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find player(CGatePlayer) of roleid:%d ", tpHead->roleid() );
		return;	
	}

	CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionOfPlayer( tpHead->roleid() );

	SendUnionApplyListResponse( tpPlayer, tpHead->roleid(), tpUnion ); 
}

// 获得联军列表的请求消息
void CCorpservice::OnMessageAllUnionList( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageAllUnionListRequest *tpMsg = ( CMessageAllUnionListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return );
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find player(CGatePlayer) of roleid:%d ", tpHead->roleid() );
		return;	
	}

	CMessage tMsgHead;
	CMessageAllUnionListResponse tMsgResponse;
	tMsgResponse.mutable_corphead()->set_roleid( tpHead->roleid() );
	tMsgResponse.set_errcode( SUCCESS );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ALLUNIONLIST );

	CCorpsManager::UnionList::iterator it = CCorpsManager::GetSingleton().m_UnionList.begin();
	int tCount = 0;
	for( ; it != CCorpsManager::GetSingleton().m_UnionList.end(); ++it )
	{
		CUnion *tpUnion = (CUnion*)CGateObjManager::GetSingleton().GetObject( it->second );
		if ( tpUnion != NULL )
		{
			PBUnionBasicInfo *tpInfo = tMsgResponse.add_unioninfo();
			tpUnion->GetUnionBasicInfo( tpInfo );
			tCount++;
		}
		if ( tCount >= FAMILYCORPS_MAXGETNUM )
		{
			tMsgHead.set_msgpara( (int)&tMsgResponse );
			CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
			tCount = 0;
			tMsgResponse.Clear();
		}
	}
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

// 发送获得联军申请列表回应
void CCorpservice::SendUnionApplyListResponse( CGatePlayer *pPlayer, int vRoleID, CUnion *pUnion )
{
	LK_ASSERT( pPlayer != NULL, return );
	CMessage tMsgHead;
	CMessageUnionApplyListResponse tMsgResponse;

	tMsgResponse.mutable_corphead()->set_roleid( vRoleID );
	tMsgResponse.set_errcode( pUnion == NULL ? ERROR_UNION_APPLYLIST_NOUNION : SUCCESS );

	if ( pUnion != NULL )
	{
		for ( MsgList::ApplyVector::iterator it = pUnion->mApplyList.mList.begin(); it != pUnion->mApplyList.mList.end(); ++it )
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( *it );
			if ( tpCorps != NULL )
			{
				PBCorpsBasicInfo *tpInfo = tMsgResponse.add_corpsinfo();
				tpCorps->GetCorpsBasicInfo( *tpInfo );
			}
		}
	}

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_UNIONAPPLYLIST );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// 发送解散联军回应
void CCorpservice::SendDismissUnionResponse( CGatePlayer *pPlayer, int vRoleID, int vResult )
{
	LK_ASSERT( pPlayer != NULL, return );
	CMessage tMsgHead;
	CMessageDismissUnionResponse tMsgResponse;

	tMsgResponse.mutable_corphead()->set_roleid( vRoleID );
	tMsgResponse.set_errcode( vResult );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_DISMISSUNION );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// 发送解散联军通知
void CCorpservice::SendDismissUnionNotify( int nServerID, int vRoleID, int *vCorpsID, int vNum )
{
	CMessage tMsgHead;
	CMessageDismissUnionNotify tMsgNotify;

	tMsgNotify.mutable_corphead()->set_roleid( vRoleID );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_DISMISSUNION );
	tMsgHead.set_msgpara( (int)&tMsgNotify );

	for ( int i = 0; i < vNum && i < MAX_CORPS_UNION; i++ )
	{
		int tRoleIDList[CORPS_MAXMEMBERNUM] = { 0 };
		int tNum = CORPS_MAXMEMBERNUM;
		
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( vCorpsID[ i ] );
		if ( tpCorps != NULL )
		{
			CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tRoleIDList, tNum );
			SendCorpsNotifyMsg( &tMsgHead, tRoleIDList, tNum );
		}
	}
}

// 获得其他参战人员 [12/1/2009 Macro]
//void CCorpservice::OnMessageGetOtherMemberList( int nServerID, CMessage* pMessage )
//{
//	if ( pMessage == NULL )
//	{
//		return; 
//	}
//
//	CMessageCorpsGetOtherMemberRequest* tpReq = ( CMessageCorpsGetOtherMemberRequest* ) pMessage->msgpara();
//
//	int tRoleID = tpReq->mutable_corphead()->roleid();
//
//	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tRoleID );
//	if ( tpPlayer == NULL )
//	{
//		return ;
//	}
//
//	CCorps* tpCorps = ( CCorps* ) CCorpsManager::GetSingleton().GetCorpsOfPlayer( tRoleID );
//	if ( tpCorps == NULL )
//	{
//		return ;
//	}
//
//	int tUnionID = tpCorps->GetUnionID();
//	CUnion* tpUnion = ( CUnion* ) CCorpsManager::GetSingleton().GetUnionByID( tUnionID );
//	if ( tpUnion == NULL )
//	{
//		// notice : no union
//		return ;
//	}
//
//	if ( tpUnion->GetCoreID() != tpCorps->GetCorpsID() )
//	{
//		//TODO: 不是主军团
//		return ;
//	}
//
//	int tCorpsList[ MAX_CORPS_UNION ] = { 0 };
//	int tCorpsNum = ARRAY_CNT( tCorpsList );
//	tpUnion->GetCorpsList( tCorpsList,tCorpsNum );
//
//	// 把主军团添加
//	if ( tCorpsNum < MAX_CORPS_UNION - 1 && tCorpsNum >= 0 )
//	{
//		tCorpsList[ tCorpsNum ++ ] = tpCorps->GetCorpsID(); 
//	}	 
//
//	CMessage tMessage;
//	CMessageCorpsGetOtherMemberResponse tPara;
//	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETOTHERMEMBER );
//	tMessage.set_msgpara( (long) &tPara );
//	tPara.mutable_corphead()->set_roleid( tRoleID );
//
//	for ( int i = 0; i < tCorpsNum; i ++ )
//	{
//		if ( tpUnion->GetCoreID() == tCorpsList[ i ] )
//		{
//			continue;
//		}
//
//		CCorps* tpUnionCorps = ( CCorps* ) CCorpsManager::GetSingleton().GetCorpsByID( tCorpsList[ i ] );
//		if ( tpUnionCorps == NULL )
//		{
//			continue;
//		}
//		int tMemberList[ CORPS_BATTLE_NUM ] = { 0 };
//		int tMemberCount = ARRAY_CNT( tMemberList );
//		tpUnionCorps->GetBattleMemberList( tMemberList, tMemberCount );
//		for ( int j = 0; j < tMemberCount; j ++ )
//		{
//			PBBattleMember* tpMember = tPara.add_memberlist();
//			CRoleGlobalInfo* tpRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tMemberList[ j ] );
//			if ( tpRoleInfo == NULL )
//			{
//				continue;
//			}
//			tpMember->set_charid( tpRoleInfo->RoleID() );
//			tpMember->set_name( tpRoleInfo->RoleName() );
//			tpMember->set_level( tpRoleInfo->Level() );
//			tpMember->set_metier( tpRoleInfo->Metier() );
//			tpMember->set_corpsname( tpUnionCorps->GetCorpsName() );
//			if ( tpUnion->IsInBattleList( tpRoleInfo->RoleID() ) )
//			{
//				tpMember->set_isjoinunion( (int) true );
//			}
//		}
//	}
//
//	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
//	return; 
//}

void CCorpservice::ChangeCorpsCountry( int vMapID, int vCorpsID )
{
	int tCountryID = CWorldManager::GetSingleton().GetCountryIDByMap( vMapID );
	if ( tCountryID == COUNTRY_NONE )
	{
		return;
	}
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( vCorpsID );
	if ( tpCorps == NULL )
	{						
		LOG_ERROR( "corps"," can not find corps by ID:%d ", vCorpsID );
		return;
	}

	int tRoleIDList[CORPS_MAXMEMBERNUM] = { 0 };
	int tNum = CORPS_MAXMEMBERNUM;
	CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tRoleIDList, tNum );

	for ( int i = 0; i < tNum; ++ i )
	{
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tRoleIDList[i] );
		if ( tpPlayer == NULL )
		{
			continue;
		}
		if ( tpPlayer->TempID( ) != tCountryID )
		{
			CWorldManager::GetSingleton().ChangePeopleNum( tpPlayer->CountryID( ), tpPlayer->TempID( ), CHANGE_DEL );
			CWorldManager::GetSingleton().ChangePeopleNum( tpPlayer->CountryID( ), tCountryID, CHANGE_ADD );
			CWorldManager::GetSingleton().ClearOfficialID( tpPlayer );
			tpPlayer->TempID( tCountryID );
			CWorldModule::GetSingleton().SendJoinCountryNotify( tpPlayer->CharID(), tCountryID );
		}			
	}	
}

// 删除房间 [12/7/2009 Macro]
void CCorpservice::OnEventDeleteRoom( int nRoleID, int nServerID )
{
	CMessage tMessage;
	CMessageCorpsOperateRoomRequest tPara;
	tMessage.mutable_msghead()->set_messageid( ID_C2S_REQUEST_ROOMOPERATION );
	tMessage.set_msgpara( (long) &tPara );

	tPara.mutable_corphead()->set_roleid( nRoleID );
	tPara.set_action( CHALLEGE_CANCEL );

	OnMessageOperateRoomRequest( nServerID, &tMessage );
	return ;
}

// ***********************************************************
//  Function:		创建军团回应
//  Description:	OnAGIPSessionCreateCorps
//  Output:			
//  Date:	12/08/2009
// **********************************************************
void CCorpservice::OnAGIPTimeoutCreateCorps(int nObjectID, int nGroupType)
{
	CCorps *pCorps = ( CCorps * )CGateObjManager::GetSingleton().GetObject( nObjectID );
	if ( pCorps == NULL) return;

	if (pCorps != NULL) 
	{
		int nRet = ERROR_GROUPNAME_SERVICE_NOTOPEN;
		LOG_ERROR("corps", "create corps (%s) for role(%d:%s) from erating error. due to (%d)",
				pCorps->GetCorpsName(), pCorps->GetRooterID(), pCorps->GetRooterName(), nRet);

		int nCreateRoleID = pCorps->GetRooterID();
		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(nCreateRoleID);
		if ( tpPlayer != NULL)
		{
			CSceneServer* tpServer = 
				CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );

			if ( tpServer != NULL) 
			{
				SendResponseCreatCorps(tpServer->mServerID, nCreateRoleID, nRet, 0, pCorps->GetCorpsName(), 0, pCorps->GetColor() );
			}
		}
		LogOrganCreate( ORGANLOGTYPE_CORPS, 0, nCreateRoleID, nRet, 0 )
		CGateObjManager::GetSingleton().DestroyObject( pCorps->get_id() );
	}
}

void CCorpservice::OnAGIPSessionCreateCorps(SysProtocol* pSysProto, int nObjectID, int nGroupType)
{
	if ( pSysProto == NULL) return;
	AGIPCreateGroupRes *pproto = (AGIPCreateGroupRes*) pSysProto;

	int nRet = -1;
	CCorps *pCorps = ( CCorps * )CGateObjManager::GetSingleton().GetObject( nObjectID );
	if ( pCorps == NULL) return;

	if (pCorps != NULL) 
	{
		switch(pproto->getResultCode()) 
		{
			case S_SUCCESS:
				{
					pCorps->SetCorpsID(pproto->getGroupID());
					CCorpsManager::GetSingleton().CreatCorpsDBInfo( pCorps );

					nRet = SUCCESS;
					break;
				}

			case E_PARAMETER_ERROR:
				{
					nRet = ERROR_CORPS_CREAT_HASDIRTYWORD;
					break;
				}


			case E_GROUP_EXIST:
			case E_GROUP_DUPLICATE:
				{
					nRet = ERROR_CORPS_CREAT_NAMEEXIST;
					break;
				}

			case E_ERROR:
			case E_SYS_DATABASE_ERROR:
			case E_SYS_DATABASE_CONNECT_ERROR:
			case E_ACCOUNT_NOT_FOUND:
			default:
				{
					nRet = ERROR_GROUPNAME_SERVICE_NOTOPEN;
					break;
				}
		}
	}

	if ( nRet != SUCCESS) 
	{
		LOG_ERROR("corps", "create corps (%s) for role(%d:%s) from erating error. due to (%d)",
				pCorps->GetCorpsName(), pCorps->GetRooterID(), pCorps->GetRooterName(), pproto->getResultCode());


		int nCreateRoleID = pCorps->GetRooterID();
		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(nCreateRoleID);
		if ( tpPlayer != NULL)
		{
			CSceneServer* tpServer = 
				CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );

			if ( tpServer != NULL) 
			{
				SendResponseCreatCorps(tpServer->mServerID, nCreateRoleID, nRet, 0, pCorps->GetCorpsName(), 0, pCorps->GetColor() );
			}
		}

		CGateObjManager::GetSingleton().DestroyObject( pCorps->get_id() );
	}
}

void CCorpservice::OnAGIPTimeoutCreateUnion(int nObjectID, int nGroupType)
{
	CUnion *pUnion = ( CUnion * )CGateObjManager::GetSingleton().GetObject( nObjectID );
	if ( pUnion != NULL) 
	{
		int nRet = ERROR_GROUPNAME_SERVICE_NOTOPEN;

		LOG_ERROR("corps", "create union (%s) for corps(%d) from erating error. due to (%d)",
				pUnion->GetUnionName(), pUnion->GetCoreID(), nRet);

		CCorps *pCorps = ( CCorps * )CCorpsManager::GetSingleton().GetCorpsByID(pUnion->GetCoreID());
		if ( pCorps != NULL)
		{
			int nCreateRoleID = pCorps->GetRooterID();
			CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(nCreateRoleID);
			if ( tpPlayer != NULL)
			{
				SendCreateUnionResponse( tpPlayer, nCreateRoleID, nRet, 0 );
			}
		}		
		CGateObjManager::GetSingleton().DestroyObject( pUnion->get_id() );
	}
}

void CCorpservice::OnAGIPSessionCreateUnion(SysProtocol* pSysProto, int nObjectID, int nGroupType)
{
	if ( pSysProto == NULL) return;
	AGIPCreateGroupRes *pproto = (AGIPCreateGroupRes*) pSysProto;

	int nRet = -1;
	CUnion *pUnion = ( CUnion * )CGateObjManager::GetSingleton().GetObject( nObjectID );
	if ( pUnion == NULL) return;

	if (pUnion != NULL) 
	{
		switch(pproto->getResultCode()) 
		{
			case S_SUCCESS:
				{
					pUnion->SetUnionID(pproto->getGroupID());
					CCorpsManager::GetSingleton().CreatUnionDBInfo( pUnion );

					nRet = SUCCESS;
					break;
				}

			case E_PARAMETER_ERROR:
				{
					nRet = ERROR_UNION_CREAT_HASDIRTYWORD;
					break;
				}


			case E_GROUP_EXIST:
			case E_GROUP_DUPLICATE:
				{
					nRet = ERROR_UNION_CREAT_NAMEEXIST;
					break;
				}

			case E_ERROR:
			case E_SYS_DATABASE_ERROR:
			case E_SYS_DATABASE_CONNECT_ERROR:
			case E_ACCOUNT_NOT_FOUND:
			default:
				{
					nRet = ERROR_GROUPNAME_SERVICE_NOTOPEN;
					break;
				}
		}
	}

	if ( nRet != SUCCESS) 
	{
		LOG_ERROR("corps", "create union (%s) for corps(%d) from erating error. due to (%d)",
				pUnion->GetUnionName(), pUnion->GetCoreID(), pproto->getResultCode());

		CCorps *pCorps = CCorpsManager::GetSingleton().GetCorpsByID( pUnion->GetCoreID());
		if ( pCorps != NULL)
		{
			int nCreateRoleID = pCorps->GetRooterID();
			CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(nCreateRoleID);
			if ( tpPlayer != NULL)
			{
				SendCreateUnionResponse( tpPlayer, nCreateRoleID, nRet, 0 );
			}
		}

		CGateObjManager::GetSingleton().DestroyObject( pUnion->get_id() );
	}
}

// 附城升级 [12/15/2009 Macro]
void CCorpservice::OnEventCityUpgrade( CCorps* npCorps, int nServerID )
{
	if ( npCorps == NULL )
	{
		return ;
	}

	int tMapList[ MAX_CORPS_NPCOWNER ] = { 0 };
	int tMapNum = ARRAY_CNT( tMapList );

	int tRet = npCorps->GetMapList( tMapList, tMapNum );
	if ( tRet <  0 )
	{
		return ;
	}

	int tMainCity = 0;
	bool bIsChangAn = false;
	// 如果附属国里有主城，升级为天子主城
	for ( int i = 0; i < tMapNum; i ++ )
	{
		if ( IS_CHANGAN( tMapList[ i ] ) )
		{
			npCorps->SetBattleCity( tMapList[ i ] );
			CWorldManager::GetSingleton().LoadCorpsFromMap( nServerID, tMapList[ i ], npCorps->GetCorpsID() );
			bIsChangAn = true;
			break;
		}
		else if ( IS_MAIN_CITY( tMapList[ i ] ) )
		{
			tMainCity = tMapList[ i ];
		}
	}

	if ( !bIsChangAn )
	{
		npCorps->SetBattleCity( tMainCity );
		CWorldManager::GetSingleton().LoadCorpsFromMap( nServerID, tMainCity, npCorps->GetCorpsID() );
	}
	
	return ;
}

//  获取已占领的地图列表  [12/16/2009 Macro]
void CCorpservice::OnMessageGetAllMapList( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}
	
	CMessageCorpsGetAllMapRequest* tpReq = ( CMessageCorpsGetAllMapRequest* ) npMessage->msgpara(); 

	CMessage tMessage;
	CMessageCorpsGetAllMapResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETALLMAP );
	tMessage.set_msgpara( (long) &tPara );

	int tRoleID = tpReq->mutable_corphead()->roleid();
	tPara.mutable_corphead()->set_roleid( tRoleID );

	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	int tServerID = tpPlayer->CountryID();	// 默认为自己的国籍
	
	// 支持查询他阵营势力地图
	if ( IS_COUNTRYID( tpReq->serverid() ) )
	{
		tServerID = tpReq->serverid();
	}	

	int tMapList[ MAP_REPETION_CAP ] = { 0 };
	int tCorpsList[ MAP_REPETION_CAP ] = { 0 };
	int tMapNum = ARRAY_CNT( tMapList );

	int tRet = CBidMgr::GetSingleton().OnGetBattleMapList( tMapList, tCorpsList, tMapNum, tServerID );
	if ( tRet < 0 )
	{
		return ;
	}
	
	for ( int i = 0; i < tMapNum; i ++ )
	{
		CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tCorpsList[ i ] );
		if ( tpCorps == NULL )
		{
			continue;
		}
		PBMapOwner* tpMap = tPara.add_maplist();
		tpMap->set_mapid( tMapList[ i ] );
		tpMap->set_corpsid( tpCorps->GetCorpsID() );
		tpMap->set_corpsname( tpCorps->GetCorpsName() );
		tpMap->set_color( tpCorps->GetColor() );
		tpMap->set_rootername( tpCorps->GetRooterName() );
		tpMap->set_corpspower( tpCorps->GetPower() );
		if ( tpCorps->GetUnionID() > INVALID_OBJ_ID )
		{
			CUnion* tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tpCorps->GetUnionID() );
			if ( tpUnion == NULL )
				continue;
			tpMap->set_unionname( tpUnion->GetUnionName() );
			tpMap->set_unionpower( tpUnion->GetUnionPower() );
		}
		CCountry* tpCountry = CWorldManager::GetSingleton().GetCountryByPlayer( tpPlayer );
		if ( tpCountry != NULL )
		{
			tpMap->set_peoplenum( tpCountry->GetPeopleNum() );
		}
	}
	
	CCountry *tpCountry = &( CWorldManager::GetSingleton().mWorld[ CWorldManager::GetSingleton().mWinWorld ].mCountry[ COUNTRY_CHANGAN ] );
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpCountry->GetCorpsID() );
	if ( tpCorps != NULL )
	{
		PBMapOwner* tpMap = tPara.add_maplist();
		tpMap->set_mapid( COUNTRY_MAP_QITIAN );
		tpMap->set_corpsid( tpCorps->GetCorpsID() );
		tpMap->set_corpsname( tpCorps->GetCorpsName() );
		tpMap->set_color( tpCorps->GetColor() );
		tpMap->set_rootername( tpCorps->GetRooterName() );
		tpMap->set_corpspower( tpCorps->GetPower() );
		if ( tpCorps->GetUnionID() > INVALID_OBJ_ID )
		{
			CUnion* tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tpCorps->GetUnionID() );
			if ( tpUnion != NULL )
			{
				tpMap->set_unionname( tpUnion->GetUnionName() );
				tpMap->set_unionpower( tpUnion->GetUnionPower() );
			}
		}
		tpMap->set_peoplenum( tpCountry->GetPeopleNum() );
	}



	tPara.set_serverid( tServerID );
	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
	return ;
}

// 获取军盟参战列表 [12/16/2009 Macro]
void CCorpservice::OnMessageGetUnionBattleList( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageUnionJoinBattleListRequest* tpReq = ( CMessageUnionJoinBattleListRequest* ) npMessage->msgpara(); 

	CMessage tMessage;
	CMessageUnionJoinBattleListResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_UNIONSUBMIT );
	tMessage.set_msgpara( (long) &tPara );

	int tRoleID = tpReq->mutable_corphead()->roleid();
	tPara.mutable_corphead()->set_roleid( tRoleID );

	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tRoleID );
	if ( tpCorps == NULL )
	{
		tPara.set_rtncode( ERROR_CORPS_NOTEXIST );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}
	
	CUnion* tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tpCorps->GetUnionID() );
	if ( tpUnion == NULL )
	{
		LOG_ERROR( "corps", "Union[%d] not found.[%s:%d]", tpCorps->GetUnionID(), __LK_FILE__, __LINE__ );
		return ;
	}

	int tBattleMemberList[ MAX_BATTLESIZE ] = { 0 };
	int tMemberNum = ARRAY_CNT( tBattleMemberList );

	int tRet = tpUnion->GetBattleMemberList( tBattleMemberList, tMemberNum );
	if ( tRet < 0 )
	{
		return ;
	}
	
	for ( int i = 0; i < tMemberNum; i ++ )
	{
		CRoleGlobalInfo* tpRole = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tBattleMemberList[ i ] );
		if ( tpRole == NULL )
		{
			continue;
		}
		CCorps* tpTmpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpRole->RoleID() );
		if ( tpTmpCorps == NULL )
		{
			continue;
		}

		PBBattleMember* tpPbMember = tPara.add_memberlist();
		tpPbMember->set_charid( tpRole->RoleID() );
		tpPbMember->set_name( tpRole->RoleName() );
		tpPbMember->set_metier( tpRole->Metier() );
		tpPbMember->set_level( tpRole->Level() );		
		tpPbMember->set_corpsname( tpTmpCorps->GetCorpsName() );
		tpPbMember->set_isjoinunion( (int) true );
	}
	
	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
	return ;
}

// 竞价结束， 军联盟处理 [12/17/2009 Macro]
int CCorpservice::OnEventUnionBattleTimeOut( int nMapID, CCorps* npCorps )
{
	if ( npCorps == NULL || nMapID == INVALID_MAP_ID )
	{
		return -1;
	}

	// 只对主城做处理
	if ( IS_MAIN_CITY( nMapID ) == false )
	{
		return -1;
	}

	CUnion* tpUnion = CCorpsManager::GetSingleton().GetUnionByID( npCorps->GetUnionID() );
	if ( tpUnion == NULL )
	{
		return -1;
	}
	
	int tCorpsList[ MAX_CORPS_UNION ] = { 0 };
	int tCorpsNum = ARRAY_CNT( tCorpsList );

	tpUnion->GetCorpsList( tCorpsList, tCorpsNum );

	// 通知第一名军团长获得参战资格
	CMessage tMessage;
	CMessageCorpsBattleRightsNotify tPara;
	
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSBATTLERIGHTS ); 
	tMessage.set_msgpara( (long) &tPara );

	for ( int i = 0; i < tCorpsNum; i ++ )
	{
		if ( npCorps->GetCorpsID() == tCorpsList[ i ] )
		{
			continue;
		}

		CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tCorpsList[ i ] );
		if ( tpCorps == NULL )
		{
			continue;
		}

		tPara.set_type( emTypeUnion );
		tPara.mutable_corphead()->set_roleid( tpCorps->GetRooterID() );
		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpCorps->GetRooterID() );
		if ( tpPlayer != NULL )
		{
			CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		}
	 }
	
	return SUCCESS;
}

// 开始战斗， 对军盟战做处理 [12/17/2009 Macro]
int CCorpservice::OnEventGetUnionBattleList( int* npList, int& nNum, CCorps* npCorps, int nMapID )
{
	if ( npList == NULL || npCorps == NULL )
	{
		return -1;
	}

	bool tUseUnionList = false;
	CUnion* tpUnion = NULL;
	int tMaxNum = nNum;


	int tUnionID = npCorps->GetUnionID();
	if ( tUnionID == INVALID_OBJ_ID )
	{
		// 军团，获取成员列表
		npCorps->GetBattleMemberList( npList, nNum );
	}
	else
	{
		// 军盟
		tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tUnionID );
		if ( tpUnion == NULL )
		{
			return -1;
		}

		if ( IS_MAIN_CITY( nMapID ) == false )
		{
			//非主城，获取军团参战列表
			npCorps->GetBattleMemberList( npList, nNum );
		}
		else
		{
			//主城，获取军盟参战列表
			tpUnion->GetBattleMemberList( npList, nNum );
			tUseUnionList = true;
		}		
	}

	// 如果参战列表为空，默认取军团在线前50人
	if ( nNum == 0 )
	{
		int tMemberList[ CORPS_BATTLE_NUM ] = { 0 };
		int tArrayNum = 0;
		int tTmpArrayNum = 0;
		
		if( tUseUnionList == true )
		{
			int tCorpsOfUnion[MAX_CORPS_UNION];
			int tCorpsNum = sizeof(tCorpsOfUnion);
			tpUnion->GetCorpsList( tCorpsOfUnion, tCorpsNum);
			for( int i = 0; i < tCorpsNum; i ++ )
			{
				CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tCorpsOfUnion[i] );
				if( tpCorps != NULL )
				{
					// 从军团中查找人数
					tTmpArrayNum = CORPS_BATTLE_NUM-tArrayNum;
					CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tMemberList+tArrayNum, tTmpArrayNum );
				
					// 把查到的人数加入到军团参战阵容
					for( int k = 0; k < tTmpArrayNum ; k++ )
					{
						tpCorps->InsertBattleMember( (tMemberList+tArrayNum)[ k ] );
					}

					tArrayNum += tTmpArrayNum;
				}
			}
		}
		else
		{
			tArrayNum = CORPS_BATTLE_NUM;
			CCorpsManager::GetSingleton().GetCorpsMemberList( npCorps, tMemberList, tArrayNum );

			// 把查到的人数默认加入到军团参战阵容
			for( int k = 0; k < tArrayNum ; k++ )
			{
				npCorps->InsertBattleMember( tMemberList[ k ] );
			}
		}

		for ( int i = 0; i < tArrayNum && i < tMaxNum; i ++ )
		{
			CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tMemberList[ i ] );
			if ( tpPlayer  != NULL )
			{
				npList[ nNum ++ ] = tMemberList[ i ];
			}

		}
	}
	
	return SUCCESS;
}

// 任命诸侯国国王 [12/21/2009 Macro]
void CCorpservice::OnMessageAppointPrince( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageCorpsAppointPrinceRequest* tpReq = ( CMessageCorpsAppointPrinceRequest* ) npMessage->msgpara();

	int tRoleID = tpReq->mutable_corphead()->roleid();
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tRoleID );
	if ( tpCorps == NULL )
	{
		return ;
	}

	//  指派军团不能为自身军团
	if ( tpCorps->GetCorpsID() == (int)tpReq->corpsid() )
	{
		return ;
	}

	//  指派占领的地图是否是主城
	if ( IS_MAIN_CITY( tpReq->mapid() ) == false )
	{
		LOG_ERROR( "corps", "Client Error. Appoint Map ID[%d],[%s:%d]", tpReq->mapid(), __LK_FILE__, __LINE__ );
		return ;
	}

	CUnion* tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tpCorps->GetUnionID() );
	if ( tpUnion == NULL )
	{
		LOG_ERROR( "corps", "Union[%d] not exist,[%s:%d]", tpCorps->GetUnionID(), __LK_FILE__, __LINE__ );
		return ;
	}

	int tCountryID = tpCorps->GetCorpsCountryID();

	CMessage tMessage;
	CMessageCorspAppointPrinceResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_APPOINTPRINCE ); 
	tMessage.set_msgpara( (long) &tPara );
	tPara.mutable_corphead()->set_roleid( tRoleID );

	// 是否是天子
	if ( tpUnion->GetCoreID() != tpCorps->GetCorpsID() || tpCorps->GetRooterID() != tRoleID )
	{
		tPara.set_rtncode( ERROR_WORLD_SET_NOTKING );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}

	// 任命的军团是否在同一个联盟
	CCorps* tpSubCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpReq->corpsid() );
	if ( tpSubCorps == NULL )
	{
		tPara.set_rtncode( ERROR_CORPS_NOTEXIST );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}

	if ( tpUnion->GetUnionID() != tpSubCorps->GetUnionID() )
	{
		tPara.set_rtncode( ERROR_UNION_NOTSAME );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}

	// 任命军团长是否已经是国王了
	if ( tpSubCorps->GetBattleCity() != INVALID_MAP_ID )
	{
		tPara.set_rtncode( ERROR_UNION_HASBEENPRINCE );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}

	//  地图是否被军盟占领，且是否有国王
	int tNpcID = tpCorps->HasOwnThisMap( tpReq->mapid() );
	if ( tNpcID == INVALID_OBJ_ID || tpCorps->GetBattleCity() == (int)tpReq->mapid() )
	{
		tPara.set_rtncode( ERROR_UNION_MAPHASOWNER );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}

	// 清除原来占领信息，更新称号
	tpCorps->ClearNpcBattleInfo( tNpcID );
	
	// 设置新的占领信息
	CBidMgr::GetSingleton().SetNpcOwnerByMapID(GATENPCID( tpReq->mapid(), tCountryID ), tpSubCorps->GetCorpsID());


	// 设置主城
	CWorldManager::GetSingleton().LoadCorpsFromMap( tCountryID, tpReq->mapid(), tpSubCorps->GetCorpsID() );
	tpSubCorps->SetBattleCity( tpReq->mapid() );

	
	//tpSubCorps->SetNpcBattleInfo( tNpcID, emFinshStep, (int)true, tpReq->mapid() );
	//UpdateNationTitle是原来SetNpcBattleInf中的功能denglichuan@2010-06-30
	tpSubCorps->UpdateNationTitle(tpReq->mapid(), true);

	
	CCorpsManager::GetSingleton().SendCorpsNpcSingle( tCountryID, tNpcID, tpSubCorps->GetCorpsID() );

	tPara.set_rtncode( SUCCESS );
	tPara.set_corpsid( tpSubCorps->GetCorpsID() );
	tPara.set_mapid( tpReq->mapid() );
	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );

	// 给任命军团发送通知
	CGatePlayer* tpDesPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpSubCorps->GetRooterID() );
	if ( tpDesPlayer == NULL )
	{
		return ;
	}

	CMessage tMsg;
	CMessageCorpsAppointPrinceNotice tNotice;
	tMsg.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_APPOINTPRINCE ); 
	tMsg.set_msgpara( (long) &tNotice );
	tNotice.mutable_corphead()->set_roleid( tpDesPlayer->CharID() );
	tNotice.set_mapid( tpReq->mapid() );
	tNotice.set_corpsid( tpSubCorps->GetCorpsID() );
	CGateServer::GetSingleton().Send2Player( tpDesPlayer, &tMsg );

	return ;	
}

// 迁都 [12/21/2009 Macro]
void CCorpservice::OnMessageMoveCapital( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageCorpsMoveCapitalRequest* tpReq = ( CMessageCorpsMoveCapitalRequest* ) npMessage->msgpara();

	int tRoleID = tpReq->mutable_corphead()->roleid();
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tRoleID );
	if ( tpCorps == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMesssageCorpsMoveCapitalResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_MOVECAPITAL ); 
	tMessage.set_msgpara( (long) &tPara );
	tPara.mutable_corphead()->set_roleid( tRoleID );
	
	// 首先检查是否是国王
	if ( tpCorps->GetBattleCity() == INVALID_MAP_ID )
	{
		tPara.set_rtncode( ERROR_UNION_NOTPRINCE );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}

	// 已经是天子， 无法迁都
	if ( IS_CHANGAN( tpCorps->GetBattleCity() ) )
	{
		tPara.set_rtncode( ERROR_UNION_ISTIANZI );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}

	// 检查要迁都的地图是否正确
	if ( !IS_MAIN_CITY( tpReq->mapid() ) || IS_CHANGAN( tpReq->mapid() ) )
	{
		tPara.set_rtncode( ERROR_UNION_MAPERROR );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}	

	// 是否占领该附属国
	if ( tpCorps->HasOwnThisMap( tpReq->mapid() ) == false )
	{
		tPara.set_rtncode( ERROR_UNION_MAPERROR );
		CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return ;
	}	

	int tOldMap = tpCorps->GetBattleCity();
	int tNewMap = tpReq->mapid();

	CWorldManager::GetSingleton().LoadCorpsFromMap( tpCorps->GetCorpsCountryID(), tOldMap, 0 );
	CWorldManager::GetSingleton().LoadCorpsFromMap( tpCorps->GetCorpsCountryID(), tNewMap, tpCorps->GetCorpsID() );

	tpCorps->SetBattleCity( tNewMap );

	tPara.set_mapid( tNewMap );
	tPara.set_rtncode( SUCCESS );

	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
	return ;
}

// 设置军团颜色 [12/21/2009 Macro]
void CCorpservice::OnMessageSetCorpsColor( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageCorpsSelectColorRequest* tpReq = ( CMessageCorpsSelectColorRequest* ) npMessage->msgpara();

	int tRoleID = tpReq->mutable_corphead()->roleid();
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tRoleID );
	if ( tpCorps == NULL )
	{
		return ;
	}

	tpCorps->SetColor( tpReq->color() );

	CMessage tMessage;
	CMessageCorpsSelectColorResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_SELECTCOLOR ); 
	tMessage.set_msgpara( (long) &tPara );
	tPara.mutable_corphead()->set_roleid( tRoleID );
	tPara.set_color( tpReq->color() );
	tPara.set_rtncode( SUCCESS );
	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );

	return ;
}

void CCorpservice::SendNotifyUnionInviteResult( int nRoleID, int nResult, const char * pInvitedName )
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}
	CMessage tMsgHead;
	CMessageUnionInviteResultNotify tMsgInvite;

	tMsgInvite.mutable_corphead()->set_roleid( nRoleID );
	tMsgInvite.set_result( nResult );
	if ( pInvitedName != NULL )
	{
		tMsgInvite.set_corpsname( pInvitedName );
	}

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_UNIONINVITERESULT );
	tMsgHead.set_msgpara( (int)&tMsgInvite );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

// 申请加入联军的回应消息
void CCorpservice::SendNotifyApplyUnion( int nServerID, int nRoleID, int nRet, int nUnionID, const char *pUnionName )
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}
	CMessage tMsgHead;
	CMessageApplyUnionResultNotify tMsgApply;

	tMsgApply.mutable_corphead()->set_roleid( nRoleID );
	
	tMsgApply.set_resultcode( nRet );
	tMsgApply.set_unionid( nUnionID );
	if ( pUnionName != NULL )
	{
		tMsgApply.set_unionname( pUnionName );
	}

	tMsgHead.set_msgpara( (int)&tMsgApply );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_APPLYUNIONRESULT );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

void CCorpservice::OnMessageChangeUnionName( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageChangeUnionNameRequest *tpMsg = ( CMessageChangeUnionNameRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_corphead(), return );
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CGatePlayer *tpSrcPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if( tpSrcPlayer == NULL )
	{
		LOG_ERROR( "corps"," can not find player(CGatePlayer) of roleid:%d ", tpHead->roleid() );
		return;	
	}

	CCorps *tpSrcCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpHead->roleid() );	
	LK_ASSERT( tpSrcCorps != NULL, return );
	if ( tpSrcCorps->GetRooterID() != ( int )tpHead->roleid() )
	{
		SendChangeUnionNameResponse( tpHead->roleid(), ERROR_UNION_CHANGENOTHEAD );
		return;
	}

	if ( tpMsg->unionname().length() == 0 || tpMsg->unionname().length() >= MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES )
		//|| tpMsg->unionnotice().length() == 0 || tpMsg->unionnotice().length() >= CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES )
	{
		SendChangeUnionNameResponse( tpHead->roleid(), ERROR_UNION_CHANGEERRNAME );
		return;
	}
	tpSrcCorps->SetTmpUnionName( tpMsg->unionname().c_str() );
	//tpSrcCorps->SetTmpUnionNotice( tpMsg->unionnotice().c_str() );	

	SendChangeUnionNameResponse( tpHead->roleid(), SUCCESS );
	SendChangeUnionNameNotify( tpHead->roleid(), tpMsg->unionname().c_str(), tpMsg->unionnotice().c_str() );
	CCorps *tpDesCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid() );
	if ( tpDesCorps != NULL )
	{
		SendChangeUnionNameNotify( tpDesCorps->GetRooterID(), tpMsg->unionname().c_str(), tpMsg->unionnotice().c_str() );
	}
}

void CCorpservice::SendChangeUnionNameResponse( int nRoleID, int nRet )
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}
	CMessage tMsgHead;
	CMessageChangeUnionNameResponse tMsgResponse;

	tMsgResponse.mutable_corphead()->set_roleid( nRoleID );

	tMsgResponse.set_result( nRet );

	tMsgHead.set_msgpara( (int)&tMsgResponse );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CHANGEUNIONNAME );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}


void CCorpservice::SendChangeUnionNameNotify( int nRoleID, const char *pUnionName, const char *pUnionNotice )
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}
	CMessage tMsgHead;
	CMessageChangeUnionNameNotify tMsgNotify;

	tMsgNotify.mutable_corphead()->set_roleid( nRoleID );

	if ( pUnionName != NULL )
	{
		tMsgNotify.set_unionname( pUnionName );
	}
	if ( pUnionNotice != NULL )
	{
		tMsgNotify.set_unionnotice( pUnionNotice );
	}

	tMsgHead.set_msgpara( (int)&tMsgNotify );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGEUNIONNAME );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}


void CCorpservice::SendSummonCorpsBossRespones( int nRoleID, int nRet )
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}
	CMessage tMsgHead;
	CMessageSummonCorpsBossResponse tMsgResponse;

	tMsgResponse.mutable_corphead()->set_roleid( nRoleID );

	tMsgResponse.set_errorcode( nRet );

	tMsgHead.set_msgpara( (int)&tMsgResponse );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_SUMMON_CORPSBOSS_RESPONSE );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

void CCorpservice:: SendIntoCorpsBossRepetionResponse( int  nRoleID, int nRet )
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID );
	if ( tpPlayer == NULL )
	{
		return ;
	}
	CMessage tMsgHead;
	CMessageIntoCorpsBossRepetionResponse tMsgResponse;
	tMsgResponse.mutable_corphead()->set_roleid( nRoleID );
	tMsgResponse.set_errorcode( nRet );
	tMsgHead.set_msgpara( ( int )&tMsgResponse );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_INTO_CORPSBOSS_REPETION_RESPONSE );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		OnMessageGetCorpsGoodsList
//  Description:	获取军团商品列表的请求消息
//  Output:			
//  Date:	01/10/2010
// **********************************************************
void CCorpservice::OnMessageGetCorpsGoodsList( int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return )			
	CMessageGetCorpsGoodsListRequest *tpMsg = ( CMessageGetCorpsGoodsListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps","[%s,%d] can not find corps by id:%d, roleid:%d ",__FUNCTION__,__LINE__, tpMsg->corpsid(), tpHead->roleid() );
		return;
	}
	
	if ( tpCorps->HasMember( tpHead->roleid() ) == false )
	{
		LOG_ERROR( "corps","[%s,%d] corps( id:%d )  has no member (roleid:%d)", __FUNCTION__, __LINE__, tpMsg->corpsid(), tpHead->roleid() );
		return;
	}
	
	// 获取npc的模板ID
	CTemplateNpc *tpTplNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpMsg->npctmpid() );
	if ( tpTplNpc == NULL )
	{
		LOG_ERROR( "corps","[%s,%d] can not find npc templateid:%d, roleid:%d ",__FUNCTION__, __LINE__, tpMsg->npctmpid(), tpHead->roleid() );
		return;
	}
	
	// 获取出售表的模板ID
	CTemplateSellTable *tpTplSellTable = ( CTemplateSellTable * )CDataStatic::SearchTpl( tpTplNpc->mSellServiceID );
	if ( tpTplSellTable == NULL )
	{
		LOG_ERROR( "corps","[%s,%d] can not find selltable by id:%d, npc template id:%d, roleid:%d",__FUNCTION__, __LINE__, tpTplNpc->mSellServiceID, tpMsg->npctmpid(), tpHead->roleid() );
		return;	
	}
	
	// 判断模板出售表的类型
	if ( tpTplSellTable->mSellTableType != CTemplateSellTable::SELLTABLETYPE_CORPSMONEYNEEDED && tpTplSellTable->mSellTableType != CTemplateSellTable::SELLTABLETYPE_CORPSWAR )
	{
		LOG_ERROR( "corps","[%s,%d] selltable is not used for corps, selltable id:%d, npctemplate id:%d, role id:%d ",__FUNCTION__,__LINE__, tpTplSellTable->mTempID, tpMsg->npctmpid(), tpHead->roleid() );
		return;
	}
	
	// 如果是战争商店，必需判断主城是否有改商店	
	if ( tpTplSellTable->mSellTableType == CTemplateSellTable::SELLTABLETYPE_CORPSWAR )
	{  
		if ( tpCorps->HasOwnThisMap( tpTplSellTable->mWarCityMapID ) == false )
		{ 			 
			 //SendResponseGetCorpsGoodsList( nServerID, ERROR_CORPS_GETGOODLIST_HASNOTOWNCITY, tpHead->roleid(), tpHead->entityid(), tLeagueShopinfo, tpMsg->npctmpid() );
			 return;					
		}					
	}
	
	// 判断权限
	//if ( tpCorps->MemberHasRight( tpHead->roleid(), EM_MONEY_REIN ) == false )
	//{
	//	SendResponseGetCorpsGoodsList( nServerID, ERROR_CORPS_HASNORIGHT, tpHead->roleid(), tpHead->entityid(), tLeagueShopinfo, tpMsg->npctmpid() );
	//	return;
	//}
	
	// 获取商品列表
	PBLeagueShopInfo tLeagueShopinfo;
	tpCorps->GetCorpsShop()->GetShopInfo( tpTplSellTable->mTempID, tLeagueShopinfo );
	
	SendResponseGetCorpsGoodsList( nServerID, SUCCESS, tpHead->roleid(), tpHead->entityid(), tLeagueShopinfo, tpMsg->npctmpid() );
}

// ***********************************************************
//  Function:		OnMessageGetCorpsExchangeGoodsList
//  Description:	获取军团兑换商品列表的消息请求
//  Output:			
//  Date:	01/10/2010
// **********************************************************
void CCorpservice::OnMessageGetCorpsExchangeGoodsList( int nServerID, CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )	
	CMessageGetCorpsExchangeGoodsListRequest *tpMsg = ( CMessageGetCorpsExchangeGoodsListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_corphead(), return )
	CorpsHead *tpHead = tpMsg->mutable_corphead();
	
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps","[%s,%d]can not find corps by id:%d, roleid:%d ", __FUNCTION__, __LINE__, tpMsg->corpsid(), tpHead->roleid() );
		return;
	}
	
	if ( tpCorps->HasMember( tpHead->roleid() ) == false )
	{
		LOG_ERROR( "corps","[%s,%d]corps has no member, corpsid:%d, roleid:%d ", __FUNCTION__, __LINE__, tpMsg->corpsid(), tpHead->roleid() );
		return;
	}
	
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpMsg->npctmpid() );
	if ( tpTmpNpc == NULL )
	{
		LOG_ERROR( "corps","[%s,%d]Can not find npctemplate by ID:%d , roleid:%d ", __FUNCTION__, __LINE__, tpMsg->npctmpid(), tpHead->roleid() );		
		return;
	}
	
	CTempateEquipExchangeTable *tpExchangeTable = ( CTempateEquipExchangeTable * )CDataStatic::SearchTpl( tpTmpNpc->mExChangeTableID );
	if ( tpExchangeTable == NULL )
	{
		LOG_ERROR( "corps","[%s,%d]Can not find exchangetable by id:%d , npctmpid:%d, roleid:%d ", __FUNCTION__, __LINE__, tpTmpNpc->mExChangeTableID, tpMsg->npctmpid(), tpHead->roleid() );		
		return;
	}
	
	if ( tpExchangeTable->mExchangeType != CTempateEquipExchangeTable::EXCHANGETYPE_CORPSEXCHANGE )
	{
		LOG_ERROR( "corps","[%s,%d] exchangetable is not used for corps, exchangetableid:%d , npctmpid:%d, roleid:%d ", __FUNCTION__, __LINE__, tpTmpNpc->mExChangeTableID, tpMsg->npctmpid(), tpHead->roleid() );		
		return;
	}
	
	PBLeagueShopInfo tPBLeagueShop;
	int tRet = tpCorps->GetCorpsShop()->GetShopInfo( tpExchangeTable->mTempID, tPBLeagueShop );
	int tShopError = 0;
	if ( tpCorps->GetCorpsShopErrcode( tRet, tShopError ) == -1 )
	{
		return;
	}
	
	SendResponseGetCorpsExchangeGoodList( nServerID, tShopError, tpHead->roleid(), tpHead->entityid(), tPBLeagueShop, tpMsg->npctmpid() );
}

// ***********************************************************
//  Function:		SendResponseGetCorpsGoodsList
//  Description:	发送获取军团商品列表的消息回应
//  Output:			
//  Date:	01/10/2010
// **********************************************************
void CCorpservice::SendResponseGetCorpsGoodsList( int nServerID, int nErrcode, int nRoleID, int nEntityID, PBLeagueShopInfo &rLeagueShop, int nNpcTmpID )
{
	CMessage tMsgHead;
	CMessageGetCorpsGoodsListResponse tMsgGetCorpGoodList;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETCORPSGOODSLIST );	
	tMsgHead.set_msgpara( (int)&tMsgGetCorpGoodList );
	
	tMsgGetCorpGoodList.mutable_corphead()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgGetCorpGoodList.mutable_corphead()->set_entityid( nEntityID );
	}
	
	tMsgGetCorpGoodList.set_npctmpid( nNpcTmpID );
	tMsgGetCorpGoodList.set_errcode( nErrcode );
	for ( int i = 0; i < rLeagueShop.iteminfo_size(); ++i )
	{
		CorpsGoodItem *tpGoodItem = tMsgGetCorpGoodList.add_goodslist();
		tpGoodItem->set_itemid( rLeagueShop.mutable_iteminfo( i )->itemid() );
		tpGoodItem->set_itemnum( rLeagueShop.mutable_iteminfo( i )->itemnum() );
	}		
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendResponseGetCorpsExchangeGoodList
//  Description:	发送获取军团商品列表的消息回应
//  Output:			
//  Date:	01/10/2010
// **********************************************************
void CCorpservice::SendResponseGetCorpsExchangeGoodList( int nServerID, int nErrcode, int nRoleID, int nEntityID, PBLeagueShopInfo &pbLeagueShop, int nNpcTmpID )
{
	CMessage tMsgHead;
	CMessageGetCorpsExchangeGoodsListResponse tMsgGetCorpsExchangeGoods;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETCORPSEXCHANGEGOODSLIST );	
	tMsgHead.set_msgpara( (int)&tMsgGetCorpsExchangeGoods );
	
	tMsgGetCorpsExchangeGoods.mutable_corphead()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgGetCorpsExchangeGoods.mutable_corphead()->set_entityid( nEntityID );
	}
	
	tMsgGetCorpsExchangeGoods.set_errcode( nErrcode );
	tMsgGetCorpsExchangeGoods.set_npctmpid( nNpcTmpID );
	
	for ( int i = 0; i < pbLeagueShop.iteminfo_size(); ++i )
	{
		CorpsGoodItem *tpGoodItem = tMsgGetCorpsExchangeGoods.add_goodslist();
		tpGoodItem->set_composeid( pbLeagueShop.mutable_iteminfo( i )->composeid() );
		tpGoodItem->set_itemid( pbLeagueShop.mutable_iteminfo( i )->itemid() );
		tpGoodItem->set_itemnum( pbLeagueShop.mutable_iteminfo( i )->itemnum() );
	}
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyChangeCorpsGoodsList
//  Description:	发送军团商品发生变化的通知
//  Output:			
//  Date:	01/10/2010
// **********************************************************
void CCorpservice::SendNotifyChangeCorpsGoodsList( int nCorpsID, int nNpcTmpID, int nItemID, int nItemNum, int nComposeID /* = 0 */ )
{
	CMessage tMsgHead;
	CMessageChangeCorpsGoodsListNotify tMsgChangeCorpsGoodList;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGECORPSGOODSLIST );	
	tMsgHead.set_msgpara( (int)&tMsgChangeCorpsGoodList );
	
	tMsgChangeCorpsGoodList.set_npctmpid( nNpcTmpID );
	
	CorpsGoodItem *tpCorpsItem = tMsgChangeCorpsGoodList.add_goodslist();
	tpCorpsItem->set_itemid( nItemID );
	tpCorpsItem->set_itemnum( nItemNum );
	if ( nComposeID != 0 )
	{
		tpCorpsItem->set_composeid( nComposeID );
	}
	
	SendCorpsMsgByID( nCorpsID, &tMsgHead );
}

// ***********************************************************
//  Function:		OnMessageCorpsExchange
//  Description:	进行军团兑换的消息请求
//  Output:			
//  Date:	01/14/2010
// **********************************************************
void CCorpservice::OnMessageCorpsExchange( int nServerID, CMessage *pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )	
	CMessageCorpsExchangeRequest *tpMsg = ( CMessageCorpsExchangeRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps","[%s,%d] can not find corps by id:%d, roleid:%d ",__FUNCTION__,__LINE__, tpMsg->corpsid(), tpMsg->roleid() );
		return;
	}
	
	if ( tpCorps->HasMember( tpMsg->roleid() ) == false )
	{
		LOG_ERROR( "corps","[%s,%d] member is not in corps, corpsid:%d, roleid:%d ",__FUNCTION__,__LINE__, tpMsg->corpsid(), tpMsg->roleid() );
		return;
	}
	
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpMsg->npctmpid() );
	if ( tpTmpNpc == NULL )
	{
		LOG_ERROR( "corps","[%s,%d] can not find npc template by id:%d, roleid:%d ", __FUNCTION__, __LINE__, tpMsg->npctmpid(), tpMsg->roleid() );
		return;
	}
	
	CTempateEquipExchangeTable *tpExchangeTable = ( CTempateEquipExchangeTable * )CDataStatic::SearchTpl( tpTmpNpc->mExChangeTableID );	
	if ( tpExchangeTable == NULL )
	{		
		LOG_ERROR( "corps","[%s,%d] can not find exchangetable template by id:%d, npctmpid:%d, roleid:%d ", __FUNCTION__, __LINE__, tpTmpNpc->mExChangeTableID, tpMsg->npctmpid(), tpMsg->roleid() );						
		return;
	}
	
	if ( tpExchangeTable->mExchangeType != CTempateEquipExchangeTable::EXCHANGETYPE_CORPSEXCHANGE )
	{
		LOG_ERROR( "corps","[%s,%d] exchangetable is not used for corps, exchangetable tmpid:%d, roleid:%d ",__FUNCTION__,__LINE__, tpExchangeTable->mTempID, tpMsg->roleid() );
		return;
	}
	
	CTemplateMultiCompose *tpTmpCompose = ( CTemplateMultiCompose * )CDataStatic::SearchTpl( tpMsg->composeid() );
	if ( tpTmpCompose == NULL )
	{
		LOG_ERROR( "corps","[%s,%d] can not find compose template by id:%d, roleid:%d ",__FUNCTION__,__LINE__, tpMsg->composeid(), tpMsg->roleid() );
		return;
	}
	
	// 贡献度的判断
	int tContribute = tpCorps->GetMemberContribute( tpMsg->roleid() );
	int tContributeNeeded = tpTmpCompose->mContribute*tpMsg->itemnum();
	if ( tContribute < tContributeNeeded )
	{
		SendResponseCorpsExchange( nServerID, ERROR_CORPS_BUYREDPAPER_CONTRIBUTENOTENOUGH, tpMsg->roleid(), tpMsg->composeid(), tpMsg->npctmpid(), tpMsg->itemnum() );
		return;
	}
	
	int tRet = tpCorps->GetCorpsShop()->ExchangeItem( tpExchangeTable->mTempID, tpTmpCompose->mItemID[0], tpMsg->itemnum(), tpMsg->composeid() );
	int tShopError = 0;
	if ( tpCorps->GetCorpsShopErrcode( tRet, tShopError ) == -1 )
	{
		return;
	}
	
	if ( tShopError == SUCCESS )
	{
		// 发送物品改变的通知
		CGoodSInfo tGoodInfo;
		tpCorps->GetCorpsShop()->GetExchangeShopItemInfo( tpExchangeTable->mTempID, tpTmpCompose->mItemID[0], tGoodInfo ); 
		tGoodInfo.mComposeID = tpTmpCompose->mTempID;
		SendNotifyChangeCorpsExchangeGoodsList( tpMsg->corpsid(), &tGoodInfo, 1 );		
		
		// 发送贡献度改变的通知
		tpCorps->SetCorpsContribute( tpMsg->roleid(), tContribute - tContributeNeeded );
		SendNotifyCorpsContributeChangedNotify( tpMsg->corpsid(), tContribute - tContributeNeeded, tpMsg->roleid(), -1*tContributeNeeded );
	
		// 军团兑换改变贡献度
		LogOrganContribteByExchange( ORGANLOGTYPE_CORPS, tpMsg->corpsid(), tpMsg->roleid(), tContribute-tContributeNeeded, tContribute )
	}
	
	SendResponseCorpsExchange( nServerID, tShopError, tpMsg->roleid(), tpMsg->composeid(), tpMsg->npctmpid(), tpMsg->itemnum() );	
}

// ***********************************************************
//  Function:		SendResponseCorpsExchange
//  Description:	发送军团兑换的回应消息
//  Output:			
//  Date:	01/14/2010
// **********************************************************
void CCorpservice::SendResponseCorpsExchange( int nServerID, int nErrcode, int nRoleID, int nComposeID, int nNpcTmpID, int nItemNum )
{  
	CMessage tMsgHead;
	CMessageCorpsExchangeResponse tMsgCorpsExchange;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_G2S_RESPONSE_CORPSEXCHANGE );
	tMsgHead.set_msgpara( (int)&tMsgCorpsExchange );
	
	tMsgCorpsExchange.set_errcode( nErrcode );
	tMsgCorpsExchange.set_composeid( nComposeID );
	tMsgCorpsExchange.set_roleid( nRoleID );
	tMsgCorpsExchange.set_npctmpid( nNpcTmpID );
	tMsgCorpsExchange.set_itemnum( nItemNum );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendResponseCorpsExchange
//  Description:	发送军团兑换发生变化的通知
//  Output:			
//  Date:	01/15/2010
// **********************************************************
void CCorpservice::SendNotifyChangeCorpsExchangeGoodsList( int nCorpsID, CGoodSInfo *pGoodInfo, int nNum )
{  
	CMessage tMsgHead;
	CMessageChangeCorpsExchangeGoodsListNotify tMsgChangeCorpsExchangeGoods;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGEXCHANGEECORPSGOODSLIST );
	tMsgHead.set_msgpara( (int)&tMsgChangeCorpsExchangeGoods );
	
	for ( int i = 0; i < nNum; ++i )
	{
		CorpsGoodItem *tpItem = tMsgChangeCorpsExchangeGoods.add_goodslist();
		tpItem->set_composeid( pGoodInfo[i].mComposeID );
		tpItem->set_itemid( pGoodInfo[i].mItemID );
		tpItem->set_itemnum( pGoodInfo[i].mItemNum );
	}	
	SendCorpsMsgByID( nCorpsID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyUseCorpsLeagueItem
//  Description:	发送使用军团特供物品的通知
//  Output:			
//  Date:	01/28/2010
// **********************************************************
void CCorpservice::SendNotifyUseCorpsLeagueItem( int nCorpsID, CGoodSInfo *pGoodInfo, int nNum, int nRoleID, const char *pName )
{	
	CMessage tMsgHead;
	CMessageUseCorpsLeagueItemNotify tMsgUseLeagueItem;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_USECORPSLEAGUEITEM );
	tMsgHead.set_msgpara( (int)&tMsgUseLeagueItem );
	
	for ( int i = 0; i < nNum; ++i )
	{
		CorpsGoodItem *tpItem = tMsgUseLeagueItem.add_goodslist();
		tpItem->set_composeid( pGoodInfo[i].mComposeID );
		tpItem->set_itemid( pGoodInfo[i].mItemID );
		tpItem->set_itemnum( pGoodInfo[i].mItemNum );
	}
	
	tMsgUseLeagueItem.set_managerid( nRoleID );
	if ( pName != NULL )
	{
		tMsgUseLeagueItem.set_managername( pName );
	}
		
	SendCorpsMsgByID( nCorpsID, &tMsgHead );
}

// ***********************************************************
//  Function:		OnMessageAddCorpsExchangeItem
//  Description:	增加军团可兑换物品的消息通知
//  Output:			
//  Date:	01/15/2010
// **********************************************************
void CCorpservice::OnMessageAddCorpsExchangeItem( int nServerID, CMessage *pMessage )
{  
	LK_ASSERT( pMessage != NULL, return )	
	CMessageAddCorpsExchangeItemNotify *tpMsg = ( CMessageAddCorpsExchangeItemNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	// 获取角色
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );
	LK_ASSERT( tpPlayer != NULL, return )
	
	// 获取军团
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpMsg->corpsid() );
	if ( tpCorps == NULL )
	{						
		LOG_ERROR( "corps","[%s,%d] can not find corps by id:%d, roleid:%d, leaguebag templateid:%d  ",__FUNCTION__,__LINE__, tpMsg->corpsid(), tpMsg->roleid(), tpMsg->leaguebagtmpid() );
		return;
	}
	
	if ( tpCorps->HasMember( tpMsg->roleid() ) == false )
	{
		LOG_ERROR( "corps","[%s,%d] player is not in corps, corpsid:%d,roleid:%d, leaguebag templateid:%d ", __FUNCTION__, __LINE__, tpMsg->corpsid(), tpMsg->roleid(), tpMsg->leaguebagtmpid() );
		return;
	}		
		
	// 获取道具
	CTplLeagueBag *tpTplLeagueBag = ( CTplLeagueBag * )CDataStatic::SearchTpl( tpMsg->leaguebagtmpid() );
	if ( tpTplLeagueBag == NULL )
	{		
		LOG_ERROR( "corps","[%s,%d] can not find template by id:%d, corps:%d, roleid:%d",__FUNCTION__, __LINE__, tpMsg->leaguebagtmpid(), tpMsg->corpsid(), tpMsg->roleid() );
		return;
	}
	
	// 增加道具
	CGoodSInfo tGoodInfo[MAX_LEAGUEITEMINBAG];
	int tIndex = 0;
	for ( int i = 0; i < (int)ARRAY_CNT(tpTplLeagueBag->mLeagueInfo ); ++i )
	{
		if ( ( rand()%SERVER_PERCENT_INT ) <= tpTplLeagueBag->mLeagueInfo[i].mProbality && tpTplLeagueBag->mLeagueInfo[i].mItemID > 0 )
		{
			int tRet = tpCorps->GetCorpsShop()->InsertExchangeItem( tpTplLeagueBag->mExchangeTableID, tpTplLeagueBag->mLeagueInfo[i].mItemID, tpTplLeagueBag->mLeagueInfo[i].mItemNum );
			if ( tRet != SUCCESS )
			{
				LOG_ERROR("corps"," [%s,%d] add corps exchangeitem error, errcode:%d, exchangetalbeid:%d, itemid:%d, roleid:%d, corpsid:%d ",__FUNCTION__,__LINE__, tRet, tpTplLeagueBag->mExchangeTableID, tpTplLeagueBag->mLeagueInfo[i].mItemID, tpMsg->roleid(), tpCorps->GetCorpsID() );
				continue;
			}
			tpCorps->GetCorpsShop()->GetExchangeShopItemInfo( tpTplLeagueBag->mExchangeTableID, tpTplLeagueBag->mLeagueInfo[i].mItemID, tGoodInfo[tIndex] );
			tGoodInfo[tIndex].mItemNum = tpTplLeagueBag->mLeagueInfo[i].mItemNum; 
			++tIndex; 
			LOG_NOTICE( "corps","[%s,%d] add corps exchange item, corps:%d, roleid:%d, itemid:%d, itemnum:%d  ",__FUNCTION__,__LINE__, tpMsg->corpsid(), tpMsg->roleid(), tpTplLeagueBag->mLeagueInfo[i].mItemID, tpTplLeagueBag->mLeagueInfo[i].mItemNum );
		}		
	}	
	SendNotifyUseCorpsLeagueItem( tpMsg->corpsid(), tGoodInfo, tIndex, tpPlayer->CharID(), tpPlayer->CharName() );
}

//// 增加占领一个城池 [1/27/2010 Macro]
//int CCorpservice::OnEventAddMapOwn( int nCorpsID, int nNpcID )
//{
//	if ( nCorpsID == INVALID_OBJ_ID || nNpcID == INVALID_OBJ_ID )
//	{
//		return -1;
//	}
//
//	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( nCorpsID );
//	if ( tpCorps == NULL )
//	{
//		return -1;
//	}
//
//	// 已经占领该npc
//	if ( tpCorps->HasOwnThisNpc( nNpcID ) )
//	{
//		return -1;
//	}
//
//	CTemplateNpc* tpNpc = (CTemplateNpc*)CDataStatic::SearchTpl( nNpcID );
//	if ( tpNpc == NULL )
//	{
//		return -1;
//	}
//
//	int tOwnMainMap = tpCorps->GetBattleCity();
//	int tAddMapID = tpNpc->mBattleCity;
//
//	if ( tAddMapID == INVALID_MAP_ID )
//	{
//		return -1;
//	}
//
//	CCorpsManager::GetSingleton().OnAttachBattleCity( GATENPCID( tAddMapID, tpCorps->GetCorpsCountryID() ), nCorpsID );
//	tpCorps->SetNpcBattleInfo( nNpcID, emFinshStep, (int)true, tAddMapID );
//
//	// 原来占非长安,当前占领长安 [12/2/2009 Macro]
//	if ( IS_MAIN_CITY( tOwnMainMap ) && !IS_CHANGAN( tOwnMainMap ) && IS_CHANGAN( tAddMapID ) )
//	{
//		CWorldManager::GetSingleton().LoadCorpsFromMap( tpCorps->GetCorpsCountryID(), tAddMapID, nCorpsID );
//		CWorldManager::GetSingleton().LoadCorpsFromMap( tpCorps->GetCorpsCountryID(), tOwnMainMap, 0 );
//		tpCorps->SetBattleCity( tAddMapID );
//	}
//	else if ( tOwnMainMap == INVALID_MAP_ID && IS_MAIN_CITY( tAddMapID ) )
//	{
//		CWorldManager::GetSingleton().LoadCorpsFromMap( tpCorps->GetCorpsCountryID(), tAddMapID, nCorpsID );
//		tpCorps->SetBattleCity( tAddMapID );
//	}	
//
//	return SUCCESS;
//}
//
//// 删除占领的一个城池 [1/27/2010 Macro]
//int CCorpservice::OnEventDelMapOwn( int nCorpsID, int nNpcID )
//{
//	if ( nCorpsID == INVALID_OBJ_ID || nNpcID == INVALID_OBJ_ID )
//	{
//		return -1;
//	}
//
//	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( nCorpsID );
//	if ( tpCorps == NULL )
//	{
//		return -1;
//	}
//
//	if ( tpCorps->HasOwnThisNpc( nNpcID ) == false )
//	{
//		return -1;
//	}
//
//	CTemplateNpc* tpNpc = (CTemplateNpc*)CDataStatic::SearchTpl( nNpcID );
//	if ( tpNpc == NULL )
//	{
//		return -1;
//	}
//
//	int tMainMap = tpCorps->GetBattleCity();
//	int tDelMap = tpNpc->mBattleCity;
//
//	if ( tDelMap == INVALID_MAP_ID )
//	{
//		return -1;
//	}
//
//	tpCorps->ClearNpcBattleInfo( nNpcID );
//	CCorpsManager::GetSingleton().OnDeleteBattleCity( GATENPCID( tDelMap, tpCorps->GetCorpsCountryID() ) );
//
//	// 如果删除的是主城，做国家关系更新
//	if ( tMainMap == tDelMap )
//	{
//		tpCorps->SetBattleCity( 0 );
//		CWorldManager::GetSingleton().LoadCorpsFromMap( tpCorps->GetCorpsCountryID(), tMainMap, 0 );
//		OnEventCityUpgrade( tpCorps, tpCorps->GetCorpsCountryID() );
//	}	
//
//	return SUCCESS;
//}

void CCorpservice::SendBattleJoinNotice( int nServerID, int nNpcID, const char* npCorpsName, int nCharID, int nType )
{
	if ( npCorpsName == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageCorpsBattleRightsNotify tPara;

	tPara.mutable_corphead()->set_roleid( nCharID );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CORPSBATTLERIGHTS ); 
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_npcid( nNpcID );	
	tPara.set_descorpsname( npCorpsName );
	tPara.set_type( nType );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );

	LOG_NOTICE( "corps", "==========Msg to Submit battle member==========" );
	LOG_NOTICE( "corps", "ServerID[%d],To:[%d],NpcID[%d]",nServerID, nCharID, nNpcID );
	LOG_NOTICE( "corps", "===============================================" );

	return ;
}


// 回滚军团战金钱GM
void CCorpservice::OnMessageRollCorpsMoney( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageRollCorpsMoneyRequest* tpReq = ( CMessageRollCorpsMoneyRequest* ) npMessage->msgpara();

	if ( tpReq->isall() == (int) true )
	{
		CBidMgr::GetSingleton().OnEventRollAllBidList();
	}

	else
	{
		CBidMgr::GetSingleton().OnEventRollCorpsNpc( tpReq->corpsid(), tpReq->npcid(), nServerID);
	}
	
	return ;
}

// 进入军团战战场请求 [3/12/2010 GaoHong]
void CCorpservice::OnMessageLoginBattleRequest( int nServerID, CMessage *npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageCorpsLoginBattleRequest* tpReq = ( CMessageCorpsLoginBattleRequest* ) npMessage->msgpara();

	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tpReq->mutable_corphead()->roleid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpPlayer->CharID() );
	if ( tpCorps == NULL )
	{
		return ;
	}

	int tNpcID = GATENPCID( tpReq->npcid(), tpCorps->GetCorpsCountryID() );

	// 先准备好错误消息，这里很少走，不费
	CMessage tMsg;
	CMessageCorpsLoginBattleResponse tResPara;
	tMsg.mutable_msghead()->set_messageid(ID_S2C_RESPONSE_LOGINBATTLE);
	tMsg.mutable_msghead()->set_dstfe( FE_CORPS );
	tMsg.mutable_msghead()->set_dstid( tpPlayer->CharID() );
	tMsg.set_msgpara( (uint32)&tResPara);
	tResPara.mutable_corphead()->set_roleid( tpPlayer->CharID() );
	tResPara.set_rtncode( ERROR_CORPS_CANTLOGINBATTLE );

	// 如果该NPC对应的战场没有开启，返回
	if ( CBattleMgr::GetSingleton().IsInBattle( tNpcID )  == false )
	{
		LOG_ERROR( "corps", "player %s want to loginbattle corps:%s npc:%d, but battle not exist",
			tpPlayer->GetCharNameShow(), tpCorps->GetCorpsNameShow(), tNpcID );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsg );
		return; 
	}

	// 取得战场KEY
	KEY_TYPE tKey = CBattleMgr::GetSingleton().GetBattleKey( tNpcID, TYPE_BATTLE_SCRABBLE );
	if ( tKey == INVITED_KEY_VALUE )
	{
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsg );
		return ;
	}
	
	// 取得战争开始时间，只有前15分钟才允许进入
	if( time(NULL) - CBattleMgr::GetSingleton().GetBattleBeginTime( tNpcID, TYPE_BATTLE_SCRABBLE ) > SCRABBLE_BATTLE_BUFFTIME )
	{
		LOG_ERROR( "corps", "player %s want to loginbattle corps:%s npc:%d , but battlebuff already timeout", 
			tpPlayer->GetCharNameShow(), tpCorps->GetCorpsNameShow(), tNpcID );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsg );
		return;
	}

	int tAttCorpsID = 0, tDesCorpsID = 0, tMapID = 0, tType = 0, tNpc = 0;
	CBattleMgr::GetSingleton().GetBattle( tKey, tAttCorpsID, tDesCorpsID, tType, tNpc, tMapID );

	// 判断争夺和占领的npc是否一样。
	if( tNpc != tNpcID )
	{
		return;
	}

	// 判断参战阵容里是否有该玩家
	if ( tpCorps->IsInBattleList( tpPlayer->CharID() ) != true )
	{
		LOG_ERROR( "corps", "player %d loginbattle, but he is not in battlelist of corps", tpPlayer->CharID() );
		return ;
	}


	// 检查战场类型是否是夺城赛
	if ( tType != TYPE_BATTLE_SCRABBLE )
	{
		return ;
	}

	// 选择攻守
	int nCampID = 0;

	// 判断是否是争夺主城
	if( IS_MAIN_CITY( tMapID ) )
	{
		// 主城允许军盟参战
		
		int tUnionID = tpCorps->GetUnionID();

		// 如果没有军盟，按照单独军团处理
		if ( tUnionID == INVALID_OBJ_ID )
		{
			if( tpCorps->GetCorpsID() == tAttCorpsID )
			{
				nCampID = 1;
			}
			else if( tpCorps->GetCorpsID() == tDesCorpsID )
			{
				nCampID = 2;
			}
		}
		else
		{
			CUnion* tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tUnionID );
			if ( tpUnion == NULL )
			{
				LOG_ERROR( "corps", "Can't find Union.[ID:%d],[%s:%d]", tUnionID, __LK_FILE__, __LINE__ );
				return ;
			}

			// 判断玩家是属于攻方军盟还是守方军盟( 1是攻方， 2是守方)
			if( tpUnion->HasCorps( tAttCorpsID ) )
			{
				nCampID = 1;
			}
			else if( tpUnion->HasCorps( tDesCorpsID ) )
			{
				nCampID = 2;
			}

		}
	}
	else
	{
		// 非主城只允许军团参战

		if( tpCorps->GetCorpsID() == tAttCorpsID )
		{
			nCampID = 1;
		}
		else if( tpCorps->GetCorpsID() == tDesCorpsID )
		{
			nCampID = 2;
		}
	}

	if( nCampID != 0 )
	{
		CRepetionModule::GetSingleton().PlayerJoin( tpPlayer->CharID(), _REPE_CORPS_BATTLE_INDEX_, tKey, nCampID );
	}
	else
	{
		tResPara.set_rtncode( ERROR_CORPS_CANTBATTLE );	
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsg );
		LOG_ERROR( "corps", "player %s want loginbattle %d, but it's corps %s isnot attcorps or defencecorps",
			tpPlayer->GetCharNameShow(), tNpcID, tpCorps->GetCorpsNameShow());
	}
		
	return ;
}

// 通知客户端战场名称 [3/19/2010 GaoHong]
void CCorpservice::OnMessageSendBattleMapRequest( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageCorpsBattleMapNotice* tpReq = (CMessageCorpsBattleMapNotice*) npMessage->msgpara();

	CGatePlayer* tpPlayer = (CGatePlayer*) CPlayerManager::GetSingleton().ScenePlayer( tpReq->roleid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "corps", "Player not found.ID[%d][%s:%d]", tpReq->roleid(), __LK_FILE__, __LINE__ );
		return ;
	}

	KEY_TYPE tKey = (KEY_TYPE) tpReq->battlekey();

	int tAttCorpsID = 0, tDesCorpsID = 0, tNpcID = 0, tMapID = 0, tType = 0;

	CBattleMgr::GetSingleton().GetBattle( tKey, tAttCorpsID, tDesCorpsID, tType, tNpcID, tMapID );

	if ( tMapID == INVALID_MAP_ID )
	{
		return ;
	}

	CMessage tMsg;
	CMessageRepetionEventNotic tMsgFact;

	tMsgFact.set_mapid( CORPS_BATTLE_SCABBLE );
	tMsgFact.set_eventid( REPETION_BATTLEMAP );
	tMsgFact.set_parame1( tMapID );
	tMsgFact.set_parame2( 0 );

	tMsg.mutable_msghead()->set_messageid( ID_S2C_REPETION_EVENT );
	tMsg.set_msgpara( (uint32)&tMsgFact );
	
	CGateServer::GetSingleton().Send2Player( tpPlayer, &tMsg );
	return ;
}

void CCorpservice::OnMessageChangeBossTimes( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}
	CMessageChangeCorpsBossTimes *tpReq = (CMessageChangeCorpsBossTimes *) npMessage->msgpara();

	CGatePlayer* tpPlayer = (CGatePlayer*) CPlayerManager::GetSingleton().ScenePlayer( tpReq->roleid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "corps", "Player not found.ID[%d][%s:%d]", tpReq->roleid(), __LK_FILE__, __LINE__ );
		return ;
	}

	CTemplateCorpsBOSSTable * tpBossTable = (CTemplateCorpsBOSSTable * )CDataStatic::GetTemp( TEMP_CORPSBOSSTABLE );

	bool tFind = false;

	for ( unsigned int i = 0; i < ARRAY_CNT(tpBossTable->mHardBOSSID); i++)
	{
		if ( tpReq->bossid() == (unsigned int)tpBossTable->mHardBOSSID[i])
		{
			tFind = true;
			break;
		}
	}
	if (tFind == false)
	{
		LOG_ERROR("corps", "boss id can not finded in template [%d]", tpReq->bossid() );
		return;
	}

	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpReq->corpsid() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps"," can not find corps by ID:%d ", tpReq->corpsid() );
		return;
	}

	tpCorps->ChangeCorpsBossTimes( tpReq->bossid(), tpReq->times() );

	CCorpservice::GetSingletonPtr()->SendNotifyBossTimesChanged( tpReq->corpsid(), tpReq->bossid(), 
															tpCorps->GetCorpsBossTimes( tpReq->bossid()),
															tpReq->times(),
															tpReq->roleid(),
															tpReq->reason(),
															tpReq->rolename().c_str());

};

void CCorpservice:: OnMessageChangeBossRefineValue( int nServerID, CMessage * npMessage )
{
	if( npMessage == NULL)
	{
		return ;
	}
	CMessageChangeCorpsBossRefineValue * tpReq = ( CMessageChangeCorpsBossRefineValue * ) npMessage->msgpara();
	CGatePlayer* tpPlayer = (CGatePlayer*) CPlayerManager::GetSingleton().ScenePlayer( tpReq->roleid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "corps", "Player not found.ID[%d][%s:%d]", tpReq->roleid(), __LK_FILE__, __LINE__ );
		return ;
	}
	//CTemplateCorpsBOSSTable * tpBossTable = (CTemplateCorpsBOSSTable * )CDataStatic::GetTemp( TEMP_CORPSBOSSTABLE );
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpReq->corpsid() );
	if(tpCorps == NULL)
	{
		LOG_ERROR( "corps", "can not find corps[%d][%s:%d]", tpReq->corpsid(), __LK_FILE__, __LINE__ );
		return ;
	}
	int nOldCorpsRefineValue = tpCorps->GetBossRefineValue();
	tpCorps->SetBossRefineValue( tpCorps->GetBossRefineValue() + tpReq->refinevalue() );

	LOG_NOTICE( "corps", "player [%d] use lianyaodan add [%d] corps refinevalue. [%s:%d]", tpPlayer->CharID(),\
		tpReq->refinevalue(), __LK_FILE__, __LINE__ );
	LogOrganRefineValueByUseFunItem( ORGANLOGTYPE_CORPS, tpReq->corpsid(), tpReq->roleid(), tpCorps->GetBossRefineValue(), nOldCorpsRefineValue );

	CCorpservice::GetSingletonPtr()->SendNotifyBossRefineValueChanged( tpReq->corpsid(),
															tpCorps->GetBossRefineValue(),
															tpReq->refinevalue(),
															tpReq->roleid(),
															tpReq->reason(),
															tpReq->rolename().c_str());

}

void CCorpservice::OnMessageSummonCorpsBoss( int nServerID, CMessage * npMessage )
{
	if ( npMessage == NULL)
	{
		return;
	}
	CMessageSummonCorpsBossRequest  *tpReq = (CMessageSummonCorpsBossRequest *) npMessage->msgpara();

	CGatePlayer* tpPlayer = (CGatePlayer*) CPlayerManager::GetSingleton().ScenePlayer( tpReq->mutable_corphead()->roleid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "corps", "Player not found.ID[%d][%s:%d]", tpReq->mutable_corphead()->roleid(), __LK_FILE__, __LINE__ );
		return ;
	}

	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpPlayer->CharID() );
	if(tpCorps == NULL)
	{
		LOG_ERROR( "corps", "can not find corps[%d][%s:%d]", tpReq->mutable_corphead()->roleid(), __LK_FILE__, __LINE__ );
		return ;
	}

	int tPost = 0;
	tpCorps->GetMemberPost( tpReq->mutable_corphead()->roleid(), tPost );

	if ( !tpCorps->PostHasRight( tPost, EM_APPROVE_APPLY ) )
	{
		CCorpservice::GetSingletonPtr()->SendSummonCorpsBossRespones( tpReq->mutable_corphead()->roleid(), ERROR_CORPS_HASNORIGHT );
		return ;
	}

	if ( tpCorps->GetBossCDCoolTime() > 0 )
	{
		CCorpservice::GetSingletonPtr()->SendSummonCorpsBossRespones( tpReq->mutable_corphead()->roleid(), ERROR_CORPBOSS_INCD );
		return ;
	}

	CTemplateCorpsBOSSTable * tpBossTable = (CTemplateCorpsBOSSTable * )CDataStatic::GetTemp( TEMP_CORPSBOSSTABLE );
	if ( tpBossTable ==NULL)
	{
		LOG_ERROR( "corps", "can not find table[%d][%s:%d]",TEMP_CORPSBOSSTABLE , __LK_FILE__, __LINE__ );
		return ;
	}

	//处理  普通boss
	int tBossType = CORPS_BOSS_TYPE_ERROR;
	int tBossNum = 0;
	for ( unsigned int i = 0; i < ARRAY_CNT(tpBossTable->mBOSSID)&& tpBossTable->mBOSSID[i] != 0 ; i++ )
	{
		tBossNum = i;
		if ( (unsigned int)tpBossTable->mBOSSID[ i ] == tpReq->bossid())
		{
			tBossType = CORPS_COMMON_BOSS_TYPE;
			break;
		}
	}

	if ( tBossType ==  CORPS_COMMON_BOSS_TYPE)
	{	
		if ( tpCorps->GetCorpsLevel() < tpBossTable->mSummonMinLevel[ tBossNum ] )
		{
			CCorpservice::GetSingletonPtr()->SendSummonCorpsBossRespones( tpReq->mutable_corphead()->roleid(), ERROR_CORPLEVEL_NOTENOUGH );	
			return;
		}

		if ( tpCorps->GetBossRefineValue() < tpBossTable->mSummonMinValue[ tBossNum ])
		{
			CCorpservice::GetSingletonPtr()->SendSummonCorpsBossRespones( tpReq->mutable_corphead()->roleid(), ERROR_CORPS_REFINEVAL_NOTENOUGH );	
			return;
		}
	}
	else if ( tBossType != CORPS_COMMON_BOSS_TYPE)
	{
		for ( unsigned int i = 0; i <ARRAY_CNT(tpBossTable->mHardBOSSID)&& tpBossTable->mHardBOSSID[i] != 0; i ++)
		{
			tBossNum = i;
			if ( (unsigned int)tpBossTable->mHardBOSSID[i] == tpReq->bossid() )
			{
				tBossType = CORPS_HARD_BOSS_TYPE;
				break;
			}
		}
		if ( tBossType == CORPS_HARD_BOSS_TYPE )
		{
			if ( tpCorps->GetCorpsLevel() < tpBossTable->mHardSummonMinLevel[ tBossNum ] )
			{
				CCorpservice::GetSingletonPtr()->SendSummonCorpsBossRespones( tpReq->mutable_corphead()->roleid(), ERROR_CORPLEVEL_NOTENOUGH );	
				return;
			}

			if ( tpCorps->GetBossRefineValue() < tpBossTable->mHardSummonMinValue[ tBossNum ] )
			{
				CCorpservice::GetSingletonPtr()->SendSummonCorpsBossRespones( tpReq->mutable_corphead()->roleid(), ERROR_CORPLEVEL_NOTENOUGH );	
				return;
			}
		}
		else
		{
			CCorpservice::GetSingletonPtr()->SendSummonCorpsBossRespones( tpReq->mutable_corphead()->roleid(), ERROR_CANNOT_FIND_CORPS_BOSS );	
			return;
		}
	}
	
	
	int tRet = StartCorpBossRepetion( tpCorps->GetCorpsID(), tpReq->bossid(), tBossType );

	
	SendSummonCorpsBossRespones(tpReq->mutable_corphead()->roleid(), tRet );

	if ( tRet == SUCCESS)
	{
		int nOldCorpsRefineValue;
		if ( tBossType ==  CORPS_COMMON_BOSS_TYPE )
		{
			nOldCorpsRefineValue = tpCorps->GetBossRefineValue();
			tpCorps->SetBossRefineValue(tpCorps->GetBossRefineValue() - tpBossTable->mSummonMinValue[tBossNum]);
			CCorpservice::GetSingletonPtr()->SendNotifyBossRefineValueChanged(tpCorps->GetCorpsID(), tpCorps->GetBossRefineValue(),\
				tpBossTable->mSummonMinValue[tBossNum],  tpPlayer->CharID(), REASON_SUMMONCORPBOSS , tpPlayer->CharName());
			
			LOG_NOTICE( "corps", "player [%d] summon corpsboss [%d] consume [%d] refinevalue. [%s:%d]", tpPlayer->CharID(),tpReq->bossid(),\
				tpBossTable->mSummonMinValue[tBossNum], __LK_FILE__, __LINE__ );
			
			LogOrganRefineValueByUseFunItem( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpPlayer->CharID(), tpCorps->GetBossRefineValue(), nOldCorpsRefineValue );

		}
		else if ( tBossType == CORPS_HARD_BOSS_TYPE )
		{
			nOldCorpsRefineValue = tpCorps->GetBossRefineValue();
			tpCorps->SetBossRefineValue(tpCorps->GetBossRefineValue() - tpBossTable->mHardSummonMinValue[tBossNum]);
			CCorpservice::GetSingletonPtr()->SendNotifyBossRefineValueChanged(tpCorps->GetCorpsID(), tpCorps->GetBossRefineValue(),\
				tpBossTable->mHardSummonMinValue[tBossNum],  tpPlayer->CharID(), REASON_SUMMONCORPBOSS , tpPlayer->CharName());
			
			LOG_NOTICE( "corps", "player [%d] summon corpsboss [%d] consume [%d] refinevalue. [%s:%d]", tpPlayer->CharID(),tpReq->bossid(),\
				tpBossTable->mHardSummonMinValue[tBossNum], __LK_FILE__, __LINE__ );

			LogOrganRefineValueByUseFunItem( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpPlayer->CharID(), tpCorps->GetBossRefineValue(), nOldCorpsRefineValue );

		}
		SendNotifyCorpsBossRepetionOpen( tpCorps->GetCorpsID(),tpReq->bossid(),tpReq->mutable_corphead()->roleid() ,tpPlayer->CharName());
	}
	
	return;


}

int CCorpservice:: StartCorpBossRepetion(int  vCorpsID, int vBossID, int vBossType)
{
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( vCorpsID );
	if(tpCorps == NULL || vBossID == 0)
	{
		LOG_ERROR( "corps", "can not find corps[%d][%s:%d]",vCorpsID, __LK_FILE__, __LINE__ );
		return ERROR_CANNOT_FIND_CORPS_BOSS;
	}
	CTemplateOgre *tpTplOgre = ( CTemplateOgre * )CDataStatic::SearchTpl( vBossID );
	if ( tpTplOgre == NULL )
	{
		LOG_ERROR( "corps", "can not find corpsboss[%d][%s:%d]",vBossID, __LK_FILE__, __LINE__ );
		return ERROR_CANNOT_FIND_CORPS_BOSS;
	}
	

	if ( vBossType == CORPS_HARD_BOSS_TYPE)
	{
		int tTimes = tpCorps->GetCorpsBossTimes(vBossID);
		if ( tTimes <= 0)
		{
			return ERROR_CORPCRYSTAL_NOTENOUGH;
		}
		tpCorps->ChangeCorpsBossTimes( vBossID, -1 );
		
	}

	
	KEY_TYPE tKey = CRepetionModule::GetSingleton().CreateBattleByIndex(  _REPE_CORPSBOSS_INDEX_ ,
															tpTplOgre->mLevel,
															tpCorps->GetCorpsID(),
															vBossID,
															vBossID,
															tpCorps->GetCorpsID() );
	
	if ( tKey == INVITED_KEY_VALUE )
	{
		LOG_ERROR( "corps", "Create Battle Failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return ERROR_CANNOT_FIND_CORPS_BOSS;
	}
	tpCorps->m_CorpsRepetion.SetRepetionKey( tKey);
	tpCorps->m_CorpsRepetion.SetStartTime();
	tpCorps->m_CorpsRepetion.SetCorpsIsOpen(true);
	tpCorps->m_CorpsRepetion.SetCorpsBossID( vBossID);
	return SUCCESS;

}

// ***********************************************************
//  Function:		SendNotifyCorpsBossRepetionOpen
//  Description:	发送军团boss副本开启
//  Output:			
//  Date:	06/27/2011
// **********************************************************
void CCorpservice::SendNotifyCorpsBossRepetionOpen( int nCorpsID, int nBossId, int nRoleId ,const char *pName)
{  

	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( nRoleId );
	if(tpCorps == NULL)
	{
		LOG_ERROR( "corps", "can not find corps[%d][%s:%d]", nRoleId, __LK_FILE__, __LINE__ );
		return ;
	}
	CMessage tMsgHead;
	CMessageCorpsBossRepetionOpenNotify tMsgBossRepetionOpen;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_CORPSBOSSREPETION_OPEN_NOTIFY );
	tMsgHead.set_msgpara( (int)&tMsgBossRepetionOpen );

	tMsgBossRepetionOpen.set_bosscdcool( tpCorps->GetBossCDCoolTime() );
	tMsgBossRepetionOpen.set_bossid( nBossId );
	if ( pName != NULL )
	{
		tMsgBossRepetionOpen.set_rolename(  pName );
	}

	SendCorpsMsgByID( nCorpsID, &tMsgHead );
}
// ***********************************************************
//  Function:		SendNotifyCorpsBossRepetionEnd
//  Description:	发送军团boss关闭
//  Output:			
//  Date:	07/18/2011
// **********************************************************
void CCorpservice:: SendNotifyCorpsBossRepetionEnd(int nCorpsID)
{
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( nCorpsID );
	if(tpCorps == NULL)
	{
		LOG_ERROR( "corps", "can not find corps[%d][%s:%d]", nCorpsID, __LK_FILE__, __LINE__ );
		return ;
	}
	if ( tpCorps->m_CorpsRepetion.IsOpen() )
	{
		tpCorps->m_CorpsRepetion.SetCorpsIsOpen(false);
	}

	// 计算军团除妖积分奖励
	CTemplateOgre *tpTplOgre = ( CTemplateOgre * )CDataStatic::SearchTpl( tpCorps->m_CorpsRepetion.GetCorpsBossID() );
	if ( tpTplOgre == NULL )
	{
		LOG_ERROR( "corps", "can not find corpsboss[%d][%s:%d]",tpCorps->m_CorpsRepetion.GetCorpsBossID(), __LK_FILE__, __LINE__ );
		return;
	}
	int nOldCorpsBossScore = tpCorps->GetCorpsBossScore();
	unsigned int nDiffcultIndex = tpTplOgre->mDiffcultIndex;
	unsigned int nDiffcultAward = tpTplOgre->mDiffcultAward;
	int nBossLiveTime = time(NULL) - tpCorps->m_CorpsRepetion.GetStartTime();
	int nCorpsBossScore = 0;
	if ( nBossLiveTime > 0 && nBossLiveTime <= 1800 )
	{
		nCorpsBossScore = nDiffcultIndex * (1800 - nBossLiveTime) + nDiffcultAward;
		nCorpsBossScore = nCorpsBossScore / 100;
	}
	else
	{
		nCorpsBossScore = nDiffcultAward;
	}
	tpCorps->SetCorpsBossScore( nCorpsBossScore + tpCorps->GetCorpsBossScore() );

	LogOrganCorpsScoreByCorpsFb( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), 0, tpCorps->GetCorpsBossScore(), nOldCorpsBossScore );

	CMessage tMsgHead;
	CMessageCorpsBossRepetionEndNotify tMsgBossRepetionEnd;

	tMsgHead.mutable_msghead()->set_messageid(ID_S2C_CORPSBOSSREPETION_END_NOTIFY);
	tMsgHead.set_msgpara( (int)& tMsgBossRepetionEnd );

	SendCorpsMsgByID( nCorpsID, &tMsgHead);

}

