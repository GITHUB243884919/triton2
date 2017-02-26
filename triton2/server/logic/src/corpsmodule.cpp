#include "stdafx.hpp"
#include "corpsmodule.h"
#include "westserver.h"	 
#include "servercore.h"	  
#include "corpstype.h"
#include "corpsmessage_pb.hxx.pb.h"
#include "property.h"
#include "propertymodule.h"   
#include "message_pb.hxx.pb.h"
#include "scenelogic.h"
#include "repetionmessage_pb.hxx.pb.h"
#include "repetionmodule.h"
#include "mapobjmanager.h"
#include "servermessage_pb.hxx.pb.h"
#include "mapmodule.h"
#include "logmodule.h"
#include "familymodule.h"
#include "base.h"

#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif


typedef ::google::protobuf::Message Message;
template< > CCorpsModule * CSingleton< CCorpsModule >::spSingleton = NULL;
char CCorpsModule::sModuleName[NAME_LENGTH] = "CorpsModule"	;


bool CCorpsModule::OnCheckLaunch()
{
	return mNpcDataFlag;
}



// ***********************************************************
//  Function:		CountSize
//  Description:	
//  Output:			
//  Date:	06/09/2009
// **********************************************************
unsigned int CCorpsModule::CountSize()
{
	return sizeof( CCorpsModule );
}

// ***********************************************************
//  Function:		OnClientMessage
//  Description:	客户端消息	
//  Output:			
//  Date:	06/09/2009
// **********************************************************
void CCorpsModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )		
	switch( pMessage->mutable_msghead()->messageid() )
	{
		case ID_C2S_REQUEST_CREATCORPS:
		{
			OnMessageCreatCorps( pPlayer, pMessage );
		}
		break;
		
		case ID_C2S_REQUEST_APPLYCORPS:
		{
			OnMessageApplyCorps( pPlayer, pMessage );
		}
		break;
		
		case ID_C2S_REQUEST_REPLYCORPSMEMBERAPPLY:
		{
			OnMessageReplyCorpsMemberApply( pPlayer, pMessage );
		}
		break;

		case ID_C2S_REQUEST_INVITECORPSMEMBER:
		{
			OnMessageInviteCorpsMember( pPlayer, pMessage );
		}
		break;
	
		case ID_C2S_REQUEST_REPLYCORPSINVITE:
		{
			OnMessageReplyCorpsInvite( pPlayer, pMessage );
		}
		break;	

		case ID_C2S_REQUEST_LEAVECORPS:
		{
			OnMessageFamilyLeaveCorps( pPlayer, pMessage );
		}
		break;

		case ID_C2S_REQUEST_KICKFAMILY:
		{
			OnMessageKickFamily( pPlayer, pMessage );
		}
		break;

		case ID_C2S_REQUEST_GETCORPSFAMILYINFO:
		{
			OnMessageGetCorpsFamilyInfo( pPlayer, pMessage );
		}
		break;
		
		case ID_C2S_REQUEST_GETCORPSPROPERTYINFO:
		{
			OnMessageGetCorpsPropertyInfo( pPlayer, pMessage );
		}
		break;

		case ID_C2S_REQUEST_GETCOPRSFAMILYMEMBER:
		{
			OnMessageGetCorpsFamilyMember( pPlayer, pMessage );
		}
		break;

		case ID_C2S_REQUEST_CHANGECORPSBULLETIN:
		{
			OnMessageChangeCorpsBulletin( pPlayer, pMessage );
		}
		break;
		
		case ID_C2S_REQUEST_CHANGECORPSHEADER:
		{
			OnMessageChangeCorpsHeader( pPlayer, pMessage );
		}
		break;
		
		case ID_C2S_REQUEST_GETCORPSAPPLYLIST:
		{
			OnMessageCorpsApplyList( pPlayer, pMessage );
		}
		break;
		
		case ID_C2S_REQUEST_DISBANDCORPS:
		{
			OnMessageDisbandCorps( pPlayer, pMessage );
		}
		break;
		case ID_C2S_REQUEST_CANCELCORPSDISBAND:
		{
			OnMessageCancelDisbandCorps( pPlayer, pMessage );
		}
		break;	
		case ID_C2S_REQUEST_CLEARCORPSAPPLY:
		{
			OnMessageClearCorpsApply( pPlayer, pMessage );
		}	
		break;
		case ID_C2S_REQUEST_REFRESHCORPSINFO:
		{
			OnMessageRefreshCorpsInfo( pPlayer, pMessage );
		}
		break;
		case ID_C2S_REQUEST_GETCOPRSLIST:
		{
			OnMessageGetCorpsList( pPlayer, pMessage );
		} 
		break;
		
		case ID_C2S_REQUEST_SETCORPSPOST:
		{
			 OnMessageSetCorpsPost( pPlayer, pMessage );
		} 
		break;
		
		case ID_C2S_REQUEST_LEVELUPCORPS:
		{
			OnMessageLevelUPCorps( pPlayer, pMessage );	
		}		
		break;
			
		case ID_C2S_REQUEST_CONTRIBUTECORPSMONEY:
		{
			OnMessageContributeCorpsMoney( pPlayer, pMessage );		
		}	
		break;
		
		case ID_C2S_REQUEST_FAMILYCONTRIBUTEMONEY:
		{
			OnMessageFamilyContributeMoney( pPlayer, pMessage );	
		}		
		break;		
		
		case ID_C2S_REQUEST_SETMEMBERNAME:
		{
			OnMessageSetMemberName( pPlayer, pMessage );	
			break;
		}
		
		case ID_C2S_REQUEST_BATTLEBID:		
		{
			OnMessageCorpsBattleBidRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_QUERYBIDRANK:
		{
			OnMessageGetCorpsBattleBidRank( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_SELECTFAMILY:
		{
			OnMessageSelectBattleFamilyRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_SELECTBATTLE:
		{
			OnMessageSelectBattleMemberRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_MENDSHENGHUOTAI:
		{
			OnMessageMendShengHuoTaiRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_ROOMOPERATION:
		case ID_C2S_REQUEST_CHALLEGE:
		case ID_C2S_REQUEST_REPLYCHALLEGE:
		case ID_C2S_REQUEST_QUERYROOM:
		case ID_C2S_REQUEST_QUERYCHALLEGELIST:
		case ID_C2S_REQUEST_QUERYRECCHALLEGE:
		case ID_C2S_REQUEST_ABORTBATTLECITY:
		case ID_C2S_REQUEST_GETOTHERMEMBER:
		case ID_C2S_REQUEST_GETALLMAP:
		case ID_C2S_REQUEST_UNIONSUBMIT:
		case ID_C2S_REQUEST_APPOINTPRINCE:
		case ID_C2S_REQUEST_MOVECAPITAL:
		case ID_C2S_REQUEST_SELECTCOLOR:
		case ID_C2S_REQUEST_LOGINBATTLE:
	//	case ID_C2S_SUMMON_CORPSBOSS_REQUEST:
		{
			OnMessageCorpsChallegeRequest( pPlayer, pMessage );
			break;
		}
		
		case ID_C2S_REQUEST_QUERYCORPSWELFARE:
		{
			OnMessageQueryNpcWelfareRequest( pPlayer, pMessage );
			break;
		} 
		
		case ID_C2S_REQUEST_OBTAINCORPSWELFARE:
		{
			OnMessageObtainNpcWelfareRequest( pPlayer, pMessage );
			break;
		}
		
		case ID_C2S_REQUEST_GETCORPSMONEY:
		{
			OnMessageGetCorpsMoneyRequest( pPlayer, pMessage );
			break;
		}

		case ID_C2S_REQUEST_CREATEUNION:
		{
			OnMessageCreateUnion( pPlayer, pMessage );
			break;
		}		

		case ID_C2S_REQUEST_APPLYUNION:
		{
			OnMessageApplyUnion( pPlayer, pMessage );
			break;
		}		

		case ID_C2S_REQUEST_REPLYUNIONMEMBERAPPLY:
		{
			OnMessageReplyUnionMemberApply( pPlayer, pMessage );
			break;
		}			

		case ID_C2S_REQUEST_INVITEUNIONMEMBER:
		{
			OnMessageInviteUnionMember( pPlayer, pMessage );
			break;
		}			

		case ID_C2S_REQUEST_REPLYUNIONINVITE:
		{
			OnMessageReplyUnionInvite( pPlayer, pMessage );
			break;
		}			

		case ID_C2S_REQUEST_LEAVEUNION:
		{
			OnMessageCorpsLeaveUnion( pPlayer, pMessage );
			break;
		}			

		case ID_C2S_REQUEST_KICKCORPS:
		{
			OnMessageKickCorps( pPlayer, pMessage );
			break;
		}	

		case ID_C2S_REQUEST_MAKEUNION:
		{
			OnMessageMakeUnion( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_UNIONINFO:
		{
			OnMessageGetUnionInfo( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_DISMISSUNION:
		{
			OnMessageDismissUnion( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_UNIONAPPLYLIST:
		{
			OnMessageUnionApplyList( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_ALLUNIONLIST:
		{
			OnMessageAllUnionList( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_CHANGEUNIONNAME:
		{
			OnMessageChangeUnionName( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_GETCORPSGOODSLIST:
		{
			OnMessageGetCorpsGoodsList( pPlayer, pMessage );
			break;	
		}
		case ID_C2S_REQUEST_GETCORPSEXCHANGEGOODSLIST:
		{
			OnMessageGetCorpsExchangeGoodsList( pPlayer, pMessage );
			break;
		}
		case ID_C2S_SUMMON_CORPSBOSS_REQUEST:
		{
			OnMessageSummonCorpsBossRequest( pPlayer, pMessage );
			break;
		}	
		
			
			
		default:
			break;
	}
}

// ***********************************************************
//  Function:		OnGateMessage
//  Description:	网关消息
//  Output:			
//  Date:	06/13/2009
// **********************************************************
void CCorpsModule::OnGateMessage( CMessage *pMsg )
{  
	LK_ASSERT( pMsg != NULL, return )

	// 处理公共消息
	switch ( pMsg->mutable_msghead()->messageid() )
	{
	case ID_G2S_NOTIFY_JOINBATTLE:
		{
			OnGSCorpsJoinBattleNotice( pMsg );
			return ;
		}
	case ID_G2S_RESPONSE_BATTLEFLAG:
		{
			OnGSSendBattleFlagOwnerNoitce( pMsg );
			return;
		}

	case ID_G2S_RESPONSE_BUYCORPSITEM:
		{
			OnGateResponseBuyCorpsItem( pMsg );
			return;
		}
	case ID_G2S_RESPONSE_CORPSEXCHANGE:
		{
			OnGateResponseCorpsExchange( pMsg );
			return;
		}
	default:
		break;
	}

	// 获取家族的消息头
	Message* pUnknownMessage = (Message*)pMsg->msgpara();
	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessage->GetDescriptor();
	const ::google::protobuf::Reflection*  pReflection = pUnknownMessage->GetReflection();

	if (pDescriptor == NULL || pReflection == NULL) return; 
	const ::google::protobuf::FieldDescriptor*  pFieldDescriptor = pDescriptor->FindFieldByNumber(1);
	if (pFieldDescriptor == NULL) return; 
	if( pFieldDescriptor->type() != ::google::protobuf::FieldDescriptor::TYPE_MESSAGE )
	{
		LOG_ERROR( "corps", "[%s:%d] message %d is not corps message", 
			__FUNCTION__, __LINE__, pMsg->msghead().messageid() );
		 return;
	}

	const Message& tCorpsHeader = pReflection->GetMessage( *pUnknownMessage, pFieldDescriptor ); 		
	CorpsHead* tpHead = ( CorpsHead* ) &tCorpsHeader;

	// 获取玩家
	CEntityPlayer *pPlayer = NULL;
	if ( tpHead->entityid() != 0 )
	{
		pPlayer = dynamic_cast<CEntityPlayer*>(CSceneObjManager::GetSingletonPtr()->GetObject( tpHead->entityid() ) );
	}
	else
	{
		pPlayer =  CSceneLogic::GetSingleton().GetPlayerByCharID( tpHead->roleid( ) );
	}			
	
	if( pPlayer == NULL ) 
	{
		LOG_ERROR( "corps", "[%s:%d] in message %d can't find player by entityid %d or char id %d",
			__FUNCTION__, __LINE__, pMsg->msghead().messageid(), tpHead->entityid(), tpHead->roleid( ) );
		return ;
	}


	switch( pMsg->mutable_msghead()->messageid() )
	{  		
		case ID_S2C_RESPONSE_CREATCORPS:
		{
			OnGSCreatCorpsResponse( pPlayer, pMsg );
			break;
		}		
		case ID_S2C_RESPONSE_APPLYCORPS:
		{
			OnGSApplyCorpsResponse( pPlayer, pMsg );
			break;
		}
		case ID_S2C_NOTIFY_CORPSMEMBERAPPLY:
		{
			OnGSCorpsMemberApplyNotify( pPlayer, pMsg );
			break;
		}		
		case ID_S2C_RESPONSE_REPLYCORPSMEMBERAPPLY:
		{
			OnGSReplyMemberApplyResponse( pPlayer, pMsg );
			break;
		}
		case ID_S2C_NOTIFY_APPLYCORPSRESULT:
		{
			OnGSApplyCorpsResultNotify( pPlayer, pMsg );
			break;
		}		
		case ID_S2C_RESPONSE_INVITECORPSMEMBER:
		{
			OnGSInviteCorpsMemberResponse( pPlayer, pMsg );
			break;
		}
		case ID_S2C_NOTIFY_CORPSINVITE:
		{
			OnGSCorpsInviteNotify( pPlayer, pMsg );
			break;
		}
		case ID_S2C_RESPONSE_REPLYCORPSINVITE:
		{
			OnGSReplyCorpsInviteResponse( pPlayer, pMsg );
			break;
		}
		case ID_S2C_NOTIFY_CORPSINVITERESULT:
		{
			OnGSCorpsInviteResultNotify( pPlayer, pMsg );
			break;
		}		  
		case ID_S2C_NOTIFY_FAMILYJOINCORPS:
		{
			OnGSFamilyJoinCorpsNotify( pPlayer, pMsg );
			break;
		}				
		case ID_S2C_RESPONSE_LEAVECORPS:
		{
			OnGSFamilyLeaveCorpsResponse( pPlayer, pMsg );
			break;
		}		  
		case ID_S2C_NOTIFY_FAMILYLEAVED:
		{
			OnGSFamilyLeaveCorpsNotify( pPlayer, pMsg );
			break;
		} 		
		case ID_S2C_RESPONSE_KICKFAMILY:
		{
			OnGSKickFamilyResponse( pPlayer, pMsg );
			break;
		}

		case ID_S2C_NOTIFY_FAMILYKICKED:
		{
			OnGSFamilyKickedNotify( pPlayer, pMsg );
			break;
		} 
		case ID_S2C_RESPONSE_GETCORPSFAMILYINFO:
		{
			OnGSGetCorpsFamilyInfoResponse( pPlayer, pMsg );
			break;
		}		
		case ID_S2C_RESPONSE_GETCORPSPROPERTYINFO:
		{
			OnGSGetCorpsPropertyInfoResponse( pPlayer, pMsg );
			break;
		}		
		case ID_S2C_RESPONE_GETCOPRSFAMILYMEMBER:
		{
			OnGSGetCorpsFamilyMemberResponse( pPlayer, pMsg );
			break;
		}		
		case ID_S2C_RESPONSE_CHANGECORPSBULLETIN:
		{
			OnGSChangeCorpsBulletinResponse( pPlayer, pMsg );
			break;
		}

		case ID_S2C_NOTIFY_CHANGECORPSBULLETIN:
		{
			OnGSChangeCorpsBulletinNotify( pPlayer, pMsg );
			break;
		}	

		case ID_S2C_NOTIFY_CHANGECORPSHEADER:
		{
			OnGSChangeCorpsHeaderNotify( pPlayer, pMsg );
			break;
		}		
		case ID_S2C_RESPONSE_GETCORPSAPPLYLIST:
		{
			OnGSGetCorpsApplyListResponse( pPlayer, pMsg );
			break;
		}		
		case ID_S2C_RESPONSE_DISBANDCORPS:
		{
			OnGSDisbandCorpsResponse( pPlayer, pMsg );
			break;
		}		 
		case ID_S2C_NOTIFY_DISBANDCORPS:
		{
			OnGSDisbandCorpsNotify( pPlayer, pMsg );
			break;
		}	
		case ID_S2C_RESPONSE_CHANGECORPSHEADER:
		{
			OnGSChangeCorpsHeaderResponse( pPlayer, pMsg );
			break;	
		}	
		case ID_S2C_RESPONSE_CANCELCORPSDISBAND:
		{
			OnGSCancelDisbandCorpsResponse( pPlayer, pMsg );
			break;
		}
		case ID_S2C_RESPONSE_REFRESHCORPSINFO:
		{
			OnGSRefreshCorpsInfoResponse( pPlayer, pMsg );				
			break;
		} 	
		case ID_S2C_RESPONSE_CLEARCORPSAPPLY:
		{
			OnGSClearCorpsApplyResponse( pPlayer, pMsg );
			break;
		}	
		case ID_S2C_RESPONSE_GETCORPSLIST:
		{
			OnGSGetCorpsListResponse( pPlayer, pMsg );
			break;
		}		
		
		case ID_S2C_RESPONSE_SETCORPSPOST:
		{
			OnGSSetCorpMemberPostResponse( pPlayer, pMsg );
			break;
		}
		
		case ID_S2C_RESPONSE_LEVELUPCORPS:
		{
			OnGSLevelUPCorpsResponse( pPlayer, pMsg );
			break;
		}
		
		case ID_S2C_RESPONSE_CONTRIBUTECORPSMONEY:
		{
			OnGSContributeCorpsMoneyResponse( pPlayer, pMsg );
			break;	
		}
		
		case ID_S2C_RESPONSE_FAMILYCONTRIBUTEMONEY:
		{
			OnGSFamilyContributeMoneyResponse( pPlayer, pMsg );
			break;
		}
		
		case ID_S2C_RESPONSE_SETMEMBERNAME:
		{
			OnGSSetMemberNameResponse( pPlayer, pMsg );
			break;
		}
		
		case ID_S2C_NOTIFY_SETCORPSPOST:
		{
			OnGSSetCorpsPostNotify( pPlayer, pMsg );
			break;
		}
		
		case ID_S2C_NOTIFY_CORPSLEVELUP:
		{
			OnGSCorpsLevelUPNotify( pPlayer, pMsg );
			break;	
		}	
		
		case ID_S2C_NOTIFY_CORPSPVPSTARLEVELUP:
		{
			OnGSCorpsPVPStarLevelUPNotify( pPlayer, pMsg );
			break;
		}		
		
		case ID_S2C_RESPONSE_QUERYBIDRANK:
		{
			OnGSQueryBidRankResponse( pPlayer, pMsg );
			break;
		}
		case ID_S2C_RESPONSE_BATTLEBID:
		{
			OnGSCorpsBidResponse( pPlayer, pMsg );
			break;
		}
		case ID_S2C_NOTIFY_CORPSBATTLERIGHTS:
		{
			OnGSCorpsBattleRightsNotify( pPlayer, pMsg );
			break;
		}
		case ID_S2C_NOTIFY_RECBATTLERIGHTS:
		{
			OnGSFamilyBattleRightsNotify( pPlayer, pMsg );
			break;
		}
		case ID_S2C_NOTIFY_MEMBERBATTLERIGHTS:
		{
			OnGSMemberBattleRightsNotify( pPlayer, pMsg );
			break;
		}
		case ID_S2C_RESPONSE_SELECTBATTLE:
		{
			OnGSSelectBattleMemberResponse( pPlayer, pMsg );
			break;
		}
		case ID_S2C_RESPONSE_SELECTFAMILY:
		{
			OnGSSelectBattleFamilyResponse( pPlayer, pMsg );
			break;
		}
		case ID_S2C_RESPONSE_ROOMOPERATION:
		case ID_S2C_RESPONSE_CHALLEGE:
		case ID_S2C_RESPONSE_REPLYCHALLEGE:
		case ID_S2C_RESPONSE_QUERYROOM:
		case ID_S2C_RESPONSE_QUERYCHALLEGELIST:
		case ID_S2C_RESPONSE_QUERYRECCHALLEGE:
		case ID_S2C_RESPONSE_ABORTBATTLECITY:
		case ID_S2C_NOTIFY_ABORTBATTLECITY:
		case ID_S2C_NOTIFY_ROOM:
		{
			OnGSCorpsChallgeReponse( pPlayer, pMsg );
			break;
		}
		
		case ID_S2C_RESPONSE_QUERYCORPSWELFARE:
		{
			OnGateMsgQueryNpcWelfareResponse( pPlayer, pMsg );
			break;
		} 
		
		case ID_S2C_RESPONSE_OBTAINCORPSWELFARE:
		{
			OnGateMsgObtainNpcWelfareResponse( pPlayer, pMsg );
			break;
		}		
		
		case ID_S2C_RESPONSE_GETCORPSMONEY:
		{
			OnGateMsgGetCorpsMoneyResponse( pPlayer, pMsg );
			break;
		}	

		case ID_S2C_NOTIFY_CORPSBATTLERESULT:
		{
			OnGSCorpsBattleResultNotice( pPlayer, pMsg );
			break;
		}
		
		case ID_S2C_NOTIFY_CORPSCONTRIBUTECHANGED:
		{
			OnGateNotifyChangeCorpsContribute( pPlayer, pMsg );
			break;
		}	

		case ID_S2C_NOTIFY_CORPSCOLLECTACTIVECHANGED:
		{
			OnGateNotifyChangeCorpsCollectActive( pPlayer, pMsg );
			break;
		}
		
		case ID_S2C_NOTIFY_CORPSNAMECHANGED:
		{
			OnGateMsgCorpsTitleChangedNotify( pPlayer, pMsg );
			break;	
		}

		case ID_S2C_RESPONSE_CREATEUNION:
			{
				OnGSCreateUnionResponse( pPlayer, pMsg );
				break;
			}	
		case ID_S2C_NOTIFY_CREATEUNION:
			{
				OnGSCreateUnionNotify( pPlayer, pMsg );
				break;
			}	
		case ID_S2C_RESPONSE_APPLYUNION:
			{
				OnGSApplyUnionResponse( pPlayer, pMsg );
				break;
			}
		case ID_S2C_NOTIFY_UNIONMEMBERAPPLY:
			{
				OnGSUnionApplyNotify( pPlayer, pMsg );
				break;
			}		
		case ID_S2C_RESPONSE_REPLYUNIONMEMBERAPPLY:
			{
				OnGSReplyUnionApplyResponse( pPlayer, pMsg );
				break;
			}
		case ID_S2C_NOTIFY_APPLYUNIONRESULT:
			{
				OnGSApplyUnionResultNotify( pPlayer, pMsg );
				break;
			}		
		case ID_S2C_RESPONSE_INVITEUNIONMEMBER:
			{
				OnGSInviteUnionMemberResponse( pPlayer, pMsg );
				break;
			}
		case ID_S2C_NOTIFY_UNIONINVITE:
			{
				OnGSUnionInviteNotify( pPlayer, pMsg );
				break;
			}
		case ID_S2C_RESPONSE_REPLYUNIONINVITE:
			{
				OnGSReplyUnionInviteResponse( pPlayer, pMsg );
				break;
			}
		case ID_S2C_NOTIFY_UNIONINVITERESULT:
			{
				OnGSUnionInviteResultNotify( pPlayer, pMsg );
				break;
			}		  
		case ID_S2C_NOTIFY_CORPSJOINUNION:
			{
				OnGSCorpsJoinUnionNotify( pPlayer, pMsg );
				break;
			}				
		case ID_S2C_RESPONSE_LEAVEUNION:
			{
				OnGSCorpsLeaveUnionResponse( pPlayer, pMsg );
				break;
			}		  
		case ID_S2C_NOTIFY_CORPSLEAVED:
			{
				OnGSCorpsLeaveUnionNotify( pPlayer, pMsg );
				break;
			} 		
		case ID_S2C_RESPONSE_KICKCORPS:
			{
				OnGSKickCorpsResponse( pPlayer, pMsg );
				break;
			}

		case ID_S2C_NOTIFY_CORPSKICKED:
			{
				OnGSCorpsKickedNotify( pPlayer, pMsg );
				break;
			} 
		case ID_S2C_RESPONSE_MAKEUNION:
		case ID_S2C_NOTIFY_MAKEUNION:
			{
				OnGSMakeUnionResponse( pPlayer, pMsg );
				break;
			} 
		case ID_S2C_RESPONSE_UNIONINFO:
			{
				OnGSGetUnionInfoResponse( pPlayer, pMsg );
				break;
			}
		case ID_S2C_RESPONSE_DISMISSUNION:
			{
				OnGSDismissUnionResponse( pPlayer, pMsg );
				break;
			}

		case ID_S2C_NOTIFY_DISMISSUNION:
			{
				OnGSDismissUnionNotify( pPlayer, pMsg );
				break;
			} 
		case ID_S2C_RESPONSE_UNIONAPPLYLIST:
			{
				OnGSUnionApplyListResponse( pPlayer, pMsg );
				break;
			}
		case ID_S2C_RESPONSE_ALLUNIONLIST:
			{
				OnGSAllUnionListResponse( pPlayer, pMsg );
				break;
			}
		case ID_S2C_RESPONSE_CHANGEUNIONNAME:
			{
				OnGSChangeUnionNameResponse( pPlayer, pMsg );
				break;
			}
		case ID_S2C_NOTIFY_CHANGEUNIONNAME:
			{
				OnGSChangeUnionNameNotify( pPlayer, pMsg );
				break;
			}
		case ID_S2C_RESPONSE_GETCORPSGOODSLIST:
			{
				OnGateMessageGetCorpsGoodsList( pPlayer, pMsg );
				break;
			}
		case ID_S2C_RESPONSE_GETCORPSEXCHANGEGOODSLIST:
			{
				OnGateMessageGetCorpsExchangeGoodsList( pPlayer, pMsg );
				break;
			}
		case ID_S2C_SUMMON_CORPSBOSS_RESPONSE:
			{
				OnGateMessageSummonCorpsBossResponse( pPlayer, pMsg );
				break;
			}
				
		default:
			break;
	}
}

// ***********************************************************
//  Function:		OnCreateEntity
//  Description:	创建实体	
//  Output:			
//  Date:	06/09/2009
// **********************************************************
void CCorpsModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	
}

// ***********************************************************
//  Function:		OnCreateMap
//  Description:	创建地图	
//  Output:			
//  Date:	06/09/2009
// **********************************************************
int CCorpsModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )
{				   
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnDestroyEntity
//  Description:	销毁实体	
//  Output:			
//  Date:	06/09/2009
// **********************************************************
void CCorpsModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{ 
	if ( pEntity->IsPlayer() )
	{
		CEntityPlayer *tpPlayer = ( CEntityPlayer * )pEntity;
		if ( tpPlayer->GetCorpsID() > 0 )
		{	
			// 改变军团荣耀值
			int tExpGloryNeeded = CSceneCfgManager::GetSingletonPtr( )->GetCorps_cfg( )->mExpperglory;			
			int tAPGloryNeeded = CSceneCfgManager::GetSingletonPtr( )->GetCorps_cfg( )->mExploitperglory;	
			int tGlory = 0;		
			CMemberInfo *tpMemberInfo = CFamilyModule::GetSingleton().GetFamilyMemberInfo( tpPlayer );
			if ( tpMemberInfo == NULL )
			{
				return;
			}
			if ( tExpGloryNeeded > 0 )
			{
				tGlory += tpMemberInfo->mExpGloryNeeded / tExpGloryNeeded;				
			}	
			if ( tAPGloryNeeded > 0 )
			{
				tGlory += tpMemberInfo->mApGloryNeeded / tAPGloryNeeded;
			}
			if ( tGlory > 0 )
			{
				ChangeCorpsGlory( tpPlayer, tGlory, false, REASON_MEMBEREXP );
			}											
		}
	}			
}

// ***********************************************************
//  Function:		OnExitServer
//  Description:		
//  Output:			
//  Date:	06/09/2009
// **********************************************************
void CCorpsModule::OnExitServer()
{
	
}

// ***********************************************************
//  Function:		OnLaunchServer
//  Description:		
//  Output:			
//  Date:	06/09/2009
// **********************************************************
void CCorpsModule::OnLaunchServer()
{
	OnLoadConfig();
}

// ***********************************************************
//  Function:		OnLoadConfig
//  Description:	加载配置	
//  Output:			
//  Date:	06/09/2009
// *********************************************************
void CCorpsModule::OnLoadConfig()
{ 
	CORPS_LOG* tpCfg  = CSceneCfgManager::GetSingletonPtr( )->GetCorps_log( );	
	if( tpCfg != NULL )
	{
		INIT_ROLLINGFILE_LOG( tpCfg->mIdentifier, tpCfg->mDirectory, ( LogLevel )tpCfg->mPriority, tpCfg->mMaxfilesize, tpCfg->mMaxbackupindex );
	}			
}

// ***********************************************************
//  Function:		OnRouterMessage
//  Description:	路由消息	
//  Output:			
//  Date:	06/09/2009
// *********************************************************
void CCorpsModule::OnRouterMessage( CMessage* pMessage )
{
	
}

// ***********************************************************
//  Function:		OnSaveData
//  Description:	保存玩家数据	
//  Output:			
//  Date:	06/09/2009
// *********************************************************
bool CCorpsModule::OnSaveData( CEntityPlayer* pPlayer, int vSaveCode )
{
	return true;	
}

// ***********************************************************
//  Function:		OnTimeoutMessage
//  Description:	定时器消息	
//  Output:			
//  Date:	06/09/2009
// *********************************************************
void CCorpsModule::OnTimeoutMessage( CMessage* pMessage )
{

}

// ***********************************************************
//  Function:		OnTimer
//  Description:	定时器
//  Output:			
//  Date:	06/09/2009
// *********************************************************
void CCorpsModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	
}	

// ***********************************************************
//  Function:		OnMessageApplyCorps
//  Description:	申请加入军团的请求消息
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnMessageApplyCorps( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageApplyCorpsRequest *tpMsg = ( CMessageApplyCorpsRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	CMessage tMsgHead;
	CMessageApplyCorpsResponse tMsgApplyCorps;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_APPLYCORPS );
	tMsgHead.set_msgpara( (int)&tMsgApplyCorps );
	if ( pPlayer->GetCorpsID() != 0 )
	{  
		tMsgApplyCorps.set_errcode( ERROR_CORPS_APPLY_INCORPS );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}
	
	if ( pPlayer->GetFamilyID() == 0 )
	{	
		tMsgApplyCorps.set_errcode( ERROR_CORPS_APPLY_HASNOFAMILY );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;	  	
	}
	
	CMemberInfo *tpMemberInfo = CFamilyModule::GetSingleton().GetFamilyMemberInfo( pPlayer->GetFamilyID(), pPlayer->GetCharID() );
	if ( tpMemberInfo == NULL )
	{
		LOG_ERROR( "corps","[%s,%d]can not find memberinfo ( familyid:%d, roleid:%d )", __FUNCTION__, __LINE__, pPlayer->GetFamilyID(), pPlayer->GetCharID() );
		return;
	}
	
	if ( tpMemberInfo->mFamilyPost != FAMILY_POST_HEADER )
	{
		tMsgApplyCorps.set_errcode( ERROR_CORPS_APPLY_HASNORIGHT );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
	}
	/*if ( !pPlayer->GetIsFamilyHeader() )
	{				
		tMsgApplyCorps.set_errcode( ERROR_CORPS_APPLY_HASNORIGHT );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;	  		
	}  */
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

// ***********************************************************
//  Function:		OnMessageChangeCorpsBulletin
//  Description:	改变军团公告的消息请求
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnMessageChangeCorpsBulletin( CEntityPlayer *pPlayer, CMessage *pMessage )
{	
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageChangeCorpsBulletinRequest *tpMsg = ( CMessageChangeCorpsBulletinRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL , return )
	
	if ( !tpMsg->has_bulletin() )
	{
		return;
	}
	
	if ( tpMsg->mutable_bulletin()->length() >= CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES )
	{
		return;
	}	 
		
	CDirtyWord::GetSingleton().OI_Dirty_Check( 1, (char *)tpMsg->mutable_bulletin()->c_str() );
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )
}

// ***********************************************************
//  Function:	  OnMessageChangeCorpsHeader
//  Description:  改变军团首领的消息请求
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnMessageChangeCorpsHeader( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageChangeCorpsHeaderRequest *tpMsg = ( CMessageChangeCorpsHeaderRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	// 密码安全检查
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}	
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )
}

// ***********************************************************
//  Function:	  OnMessageCorpsApplyList
//  Description:  获取军团申请列表
//  Output:			
//  Date:	06/13/2009
// *********************************************************
void CCorpsModule::OnMessageCorpsApplyList( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageGetCorpsApplyListRequest *tpMsg = ( CMessageGetCorpsApplyListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )  
}

// ***********************************************************
//  Function:	  OnMessageDisbandCorps
//  Description:  解散军团的消息请求
//  Output:			
//  Date:	06/13/2009
// *********************************************************
void CCorpsModule::OnMessageDisbandCorps( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )		
	CMessageDisbandCorpsRequest *tpMsg = ( CMessageDisbandCorpsRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	// npc服务检查
	if ( !tpMsg->has_npcentityid() )
	{
		return;
	}		   
	bool tRet = CheckNpcCorpsService( pPlayer, tpMsg->npcentityid(), FAMILYSERVICE_FAMILYNPC );
	LK_ASSERT( tRet == true, return )	  
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )  
}

// ***********************************************************
//  Function:	  OnMessageCacelDisbandCorps
//  Description:  取消军团解散的消息请求
//  Output:			
//  Date:	06/18/2009
// *********************************************************
void CCorpsModule::OnMessageCancelDisbandCorps( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageCancelCorpsDisbandRequest *tpMsg = ( CMessageCancelCorpsDisbandRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )
}

// ***********************************************************
//  Function:	  OnMessageClearCorpsApply
//  Description:  清除军团申请列表的消息请求
//  Output:			
//  Date:	06/22/2009
// *********************************************************
void CCorpsModule::OnMessageClearCorpsApply( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageClearCorpsApplyRequest *tpMsg = ( CMessageClearCorpsApplyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )  
}

// ***********************************************************
//  Function:	  OnMessageRefreshCorpsInfo
//  Description:  刷新军团信息的消息请求
//  Output:			
//  Date:	06/23/2009
// *********************************************************
void CCorpsModule::OnMessageRefreshCorpsInfo( CEntityPlayer *pPlayer, CMessage *pMessage ) 
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageRefreshCorpsInfoRequest *tpMsg = ( CMessageRefreshCorpsInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )
}

// ***********************************************************
//  Function:	  OnMessageGetCorpsList
//  Description:  获取军团的信息列表
//  Output:			
//  Date:	08/05/2009
// *********************************************************
void CCorpsModule::OnMessageGetCorpsList( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageGetCorpsListRequest *tpMsg = ( CMessageGetCorpsListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer ) 
}

// ***********************************************************
//  Function:	 OnMessageCreatCorps
//  Description: 创建军团的请求
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnMessageCreatCorps( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageCreatCorpsRequest *tpMsg = ( CMessageCreatCorpsRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	
	
	// 同时只能发一条创建军团的消息
	if ( pPlayer->GetCorpsID() == INVALID_FAMILYCORPSID )
	{			
		return;
	}
	
	if( !tpMsg->has_corpsname() )
	{	
		return;
	}	
	
	if ( tpMsg->corpsname().length() <= 0  )
	{
		return;
	}
	 	
		
	// npc服务检查
	if ( !tpMsg->has_npcentityid() )
	{
		return;
	}		   
	bool tSerRet = CheckNpcCorpsService( pPlayer, tpMsg->npcentityid(), FAMILYSERVICE_FAMILYNPC );
	LK_ASSERT( tSerRet == true, return )	  
	
	if ( !tpMsg->has_corpsname() )
	{
		return;
	}
		
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();	
	CMessage MsgHead;
	CMessageCreatCorpsResponse MsgCreatCorps;
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CREATCORPS );
	MsgHead.set_msgpara( (int)&MsgCreatCorps );
	
	// 首先判断是否在军团中
	if ( pPlayer->GetCorpsID() != 0 )
	{
		MsgCreatCorps.set_errcode( ERROR_CORPS_CREAT_INCORPS );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &MsgHead );
		return;				
	}		   	
	
	// 判断有没有家族
	if ( pPlayer->GetFamilyID() == 0 )
	{
		MsgCreatCorps.set_errcode( ERROR_CORPS_CREAT_HASNOFAMILY );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &MsgHead );
		return;
	}
	
	// 判断是不是族长
	CMemberInfo *tpMemberinfo = CFamilyModule::GetSingleton().GetFamilyMemberInfo( pPlayer );
	if ( tpMemberinfo == NULL )
	{
		LOG_ERROR( "corps","[%s,%d] can not find family member info( familyid:%d,roleid:%d ) ",__FUNCTION__,__LINE__,pPlayer->GetFamilyID(), pPlayer->GetCharID() );		
		return;
	}
	
	if ( tpMemberinfo->mFamilyPost != FAMILY_POST_HEADER )
	{
		MsgCreatCorps.set_errcode( ERROR_CORPS_CREAT_ISNOTFAMILYHEADER );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &MsgHead );
		return;		
	}
	
	/*if ( !pPlayer->GetIsFamilyHeader() )
	{	  
		MsgCreatCorps.set_errcode( ERROR_CORPS_CREAT_ISNOTFAMILYHEADER );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &MsgHead );
		return;		
	}*/
	
	
	// 等级判断
	if ( pPlayer->GetLevel() < LEVEL_CREATCORPS )
	{			
		MsgCreatCorps.set_errcode( ERROR_CORPS_CREAT_LEVELNOTENOUGH );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &MsgHead );
		return;
	}
	
	// 去掉扩军令
	/*int tTokenIndex = 0;
	int tNum		= 0;
	tpProperty->GetBaggage()->HasItemOfCertainType( ITEM_FUNCITEM, tNum, tTokenIndex, FUNCCODE_CORPSTOKEN );
	if ( tNum < 1 )
	{
		MsgCreatCorps.set_errcode( ERROR_CORPS_HASNOCORPSTOKEN );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &MsgHead );
		return;
	}*/
	
	// 脏词判断
	LK_ASSERT( tpMsg->mutable_corpsname()->length() < MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES, return )
	int tRet = CDirtyWord::GetSingleton().OI_Dirty_Check( 3, ( char * )tpMsg->mutable_corpsname()->c_str() );
	if ( tRet != 0 )
	{
		MsgCreatCorps.set_errcode( ERROR_CORPS_CREAT_HASDIRTYWORD );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &MsgHead );
		return;
	}
	
	// 等级判断
	if ( tpProperty->GetMoney() < CORPS_MONEY_CREATCOPRS )
	{

		MsgCreatCorps.set_errcode( ERROR_CORPS_CREAT_MONEYNOTENOUGH );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &MsgHead );
		return;
	}
	
	// 先扣除金钱，创建失败再返回
	tpProperty->SetMoney( tpProperty->GetMoney() - CORPS_MONEY_CREATCOPRS );
	
	// 设置状态，网关返回消息之前不可再创建军团
	pPlayer->SetCorpsID( INVALID_FAMILYCORPSID );
	
	LOG_NOTICE( "corps","[%s,%d][%s] Player( roleid:%d ) creat corps at time:(%d), money( %d ) consumed ", __LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetCharID(), time(NULL), CORPS_MONEY_CREATCOPRS );
	
	// 发往网关
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )		
}

// ***********************************************************
//  Function:	   OnMessageFamilyLeaveCorps
//  Description:   家族离开军团的消息请求
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnMessageFamilyLeaveCorps( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageFamilyLeaveCorpsRequest *tpMsg = ( CMessageFamilyLeaveCorpsRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	// 密码安全检查
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}	
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )	
}

// ***********************************************************
//  Function:	   OnMessageGetCorpsFamilyInfo
//  Description:   获取军团家族成员信息的请求
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnMessageGetCorpsFamilyInfo( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageGetCorpsFamilyInfoRequest *tpMsg = ( CMessageGetCorpsFamilyInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )  
}

// ***********************************************************
//  Function:	   OnMessageGetCorpsFamilyMember
//  Description:   获取军团中某个家族的成员信息的消息请求
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnMessageGetCorpsFamilyMember( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	
}

// ***********************************************************
//  Function:	  OnMessageGetCorpsPropertyInfo
//  Description:  获取军团属性信息的请求消息
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnMessageGetCorpsPropertyInfo( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageGetCorpsPropertyRequest *tpMsg = ( CMessageGetCorpsPropertyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return ) 
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )  
}

// ***********************************************************
//  Function:	 OnMessageInviteCorpsMember
//  Description: 邀请家族加入军团的消息请求
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnMessageInviteCorpsMember( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageInviteCorpsMemberRequest *tpMsg = ( CMessageInviteCorpsMemberRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	CMessage tMsgHead;
	CMessageInviteCorpsMemberResponse tMsgReplyInviteMember;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_INVITECORPSMEMBER );
	tMsgHead.set_msgpara( (int)&tMsgReplyInviteMember );
	if ( pPlayer->GetCorpsID() == 0 )
	{
		tMsgReplyInviteMember.set_errcode( ERROR_CORPS_INVITE_NOTINCORPS );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )
}

// ***********************************************************
//  Function:	   OnMessageKickFamily
//  Description:   从军团踢出家族的消息请求
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnMessageKickFamily( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageKickFamilyRequest *tpMsg = ( CMessageKickFamilyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )			   
	
	// 密码安全检查
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}	
		
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )  
}

// ***********************************************************
//  Function:	 OnMessageReplyCorpsInvite
//  Description: 答复军团邀请的消息请求
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnMessageReplyCorpsInvite( CEntityPlayer *pPlayer, CMessage *pMessage )
{	
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageReplyCorpsInviteRequest *tpMsg = ( CMessageReplyCorpsInviteRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer ) 
}

// ***********************************************************
//  Function:	  OnMessageReplyCorpsMemberApply
//  Description:  回复家族申请加入军团的消息请求
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnMessageReplyCorpsMemberApply( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )		
	CMessageReplyCorpsMemberApplyRequest *tpMsg = ( CMessageReplyCorpsMemberApplyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CMessage tMsgHead;
	CMessageReplyCorpsMemberApplyResponse tMsgReplyApply;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REPLYCORPSMEMBERAPPLY );
	tMsgHead.set_msgpara( (int)&tMsgReplyApply );
	if ( pPlayer->GetCorpsID() == 0 )
	{								 
		tMsgReplyApply.set_errcode( ERROR_CORPS_REPLYAPPLY_NOTINCORPS );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;				
	}
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer ) 
}

// ***********************************************************
//  Function:	  OnMessageQueryNpcWelfareRequest
//  Description:  查询军团npc福利的消息请求
//  Output:			
//  Date:	09/16/2009
// *********************************************************
void CCorpsModule::OnMessageQueryNpcWelfareRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageQueryCorpsWelfareRequest *tpMsg = ( CMessageQueryCorpsWelfareRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	// npc的判断
	CEntityNpc *tpEntity =  dynamic_cast< CEntityNpc * >( CSceneObjManager::GetSingletonPtr()->GetObject( tpMsg->entityid() ) );
	LK_ASSERT( tpEntity != NULL, return )

	// 距离检查
	if (  pPlayer->IsWithinDistance( tpEntity, MAX_NPC_TALK_GIRD ) == false )
	{
		return;
	}

	// 获取模板ID
	CPropertyNPC * tpNpcProperty = ( CPropertyNPC * )tpEntity->GetProperty(); 	
	tpMsg->set_npcid( tpNpcProperty->GetPropertyTempID() );
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )  
}

// ***********************************************************
//  Function:	  OnMessageObtainNpcWelfareRequest
//  Description:  获取军团npc福利的消息请求
//  Output:			
//  Date:	09/16/2009
// *********************************************************
void CCorpsModule::OnMessageObtainNpcWelfareRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageObtainCorpsWelfareRequest *tpMsg = ( CMessageObtainCorpsWelfareRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	// npc的判断
	CEntityNpc *tpEntity =  dynamic_cast< CEntityNpc * >( CSceneObjManager::GetSingletonPtr()->GetObject( tpMsg->entityid() ) );
	LK_ASSERT( tpEntity != NULL, return )
	
	// 距离检查
	if (  pPlayer->IsWithinDistance( tpEntity, MAX_NPC_TALK_GIRD ) == false )
	{
		return;
	}

	// 获取模板ID
	CPropertyNPC * tpNpcProperty = ( CPropertyNPC * )tpEntity->GetProperty(); 	
	tpMsg->set_npcid( tpNpcProperty->GetPropertyTempID() );
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )  
}

// ***********************************************************
//  Function:	  OnMessageGetCorpsMoneyRequest
//  Description:  获取军团金钱的消息请求
//  Output:			
//  Date:	09/16/2009
// *********************************************************
void CCorpsModule::OnMessageGetCorpsMoneyRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageGetCorpsMoneyRequest *tpMsg = ( CMessageGetCorpsMoneyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();	
	CMessage tMsgHead;
	CMessageGetCorpsMoneyResponse tMsgGetCorpsMoney;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETCORPSMONEY );
	tMsgHead.set_msgpara( (int)&tMsgGetCorpsMoney );
	
	if ( tpMsg->corpsmoney() < 0 || tpMsg->corpsmoney()*MONEY_EXCHANGERATE < 0 )
	{			
		tMsgGetCorpsMoney.set_errcode( ERROR_CORPS_MONEYINVALIDE );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}			
	if ( ( unsigned int )( tpProperty->GetMoney()+tpMsg->corpsmoney()*MONEY_EXCHANGERATE ) > MONEY_LIMIT )
	{
		tMsgGetCorpsMoney.set_errcode( ERROR_CORPS_MONEYINVALIDE );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}
		
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer ) 
}

// ***********************************************************
//  Function:	  OnMessageGetCorpsGoodsList
//  Description:  获取军团商品列表的信息请求
//  Output:			
//  Date:	01/08/2010
// *********************************************************
void CCorpsModule::OnMessageGetCorpsGoodsList( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageGetCorpsGoodsListRequest *tpMsg = ( CMessageGetCorpsGoodsListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	// 家族npc的判断
	CEntityNpc* tpEntity = dynamic_cast< CEntityNpc* >( CSceneObjManager::GetSingletonPtr( )->GetObject( tpMsg->entityid() ) );
	LK_ASSERT( tpEntity != NULL, return )
	if ( tpEntity->GetEntityType() != ENTITYTYPE_FUNCNPC )
	{
		return;
	}

	if ( pPlayer->IsWithinDistance( tpEntity, MAX_NPC_TALK_GIRD ) == false )
	{
		return;
	}	
	
	if ( pPlayer->GetCorpsID() == 0 )
	{
		return;
	}

	// 服务判断
	CPropertyNPC * tpNpcProperty = ( CPropertyNPC * )tpEntity->GetProperty(); 
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpNpcProperty->GetPropertyTempID() );
	LK_ASSERT( tpTmpNpc != NULL, return )
	tpMsg->set_npctmpid( tpTmpNpc->mTempID );
	tpMsg->set_corpsid( pPlayer->GetCorpsID() );
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )
}

// ***********************************************************
//  Function:	  OnMessageGetCorpsExchangeGoodsList
//  Description:  获取军团兑换商品列表的信息请求
//  Output:			
//  Date:	01/08/2010
// *********************************************************
void CCorpsModule::OnMessageGetCorpsExchangeGoodsList( CEntityPlayer* pPlayer, CMessage* pMessage )
{	  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageGetCorpsExchangeGoodsListRequest *tpMsg = ( CMessageGetCorpsExchangeGoodsListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	// 家族npc的判断
	CEntityNpc* tpEntity = dynamic_cast< CEntityNpc* >( CSceneObjManager::GetSingletonPtr( )->GetObject( tpMsg->entityid() ) );
	LK_ASSERT( tpEntity != NULL, return )
	if ( tpEntity->GetEntityType() != ENTITYTYPE_FUNCNPC )
	{
		return;
	}

	if ( pPlayer->IsWithinDistance( tpEntity, MAX_NPC_TALK_GIRD ) == false )
	{
		return;
	}	
	
	if ( pPlayer->GetCorpsID() == 0 )
	{
		return;
	}

	// 服务判断
	CPropertyNPC * tpNpcProperty = ( CPropertyNPC * )tpEntity->GetProperty(); 
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpNpcProperty->GetPropertyTempID() );
	LK_ASSERT( tpTmpNpc != NULL, return )
	tpMsg->set_npctmpid( tpTmpNpc->mTempID );
	tpMsg->set_corpsid( pPlayer->GetCorpsID() );
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer ) 
}

// ***********************************************************
//  Function:	  OnGateMessageGetCorpsGoodsList
//  Description:  获取军团商品列表的信息请求
//  Output:			
//  Date:	01/08/2010
// *********************************************************
void CCorpsModule::OnGateMessageGetCorpsGoodsList( CEntityPlayer* pPlayer, CMessage* pMessage )
{	  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGateMessageGetCorpsExchangeGoodsList
//  Description:  获取军团兑换商品列表的信息请求
//  Output:			
//  Date:	01/08/2010
// *********************************************************
void CCorpsModule::OnGateMessageGetCorpsExchangeGoodsList( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  CorpsExchange
//  Description:  军团兑换
//  Output:			
//  Date:	01/13/2010
// *********************************************************
void CCorpsModule::CorpsExchange( CEntityPlayer *pPlayer, int nComposeID, int nEntityID, int nItemNum )
{
	LK_ASSERT( pPlayer != NULL, return )	
	CEntityFuncNpc *tpFuncNpc = static_cast< CEntityFuncNpc * >( CSceneObjManager::GetSingleton().GetObject( nEntityID ) );
	if ( tpFuncNpc == NULL )
	{
		return;
	}	

	if ( pPlayer->IsWithinDistance( tpFuncNpc, MAX_NPC_TALK_GIRD ) == false )
	{
		return;
	}
	
	CPropertyNPC *tpNpcProperty = ( CPropertyNPC * )tpFuncNpc->GetProperty();
	CTemplateNpc *tpNpcTempLate = ( CTemplateNpc * )CDataStatic::SearchTpl( tpNpcProperty->GetPropertyTempID() );
	if ( tpNpcTempLate == NULL )
	{
		return;
	}
	
	if ( pPlayer->GetCorpsID() == 0 )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_CORPS_EXCHANGE_HASNOCORPS );
		return;
	}
	
	// 包裹检查
	CTemplateMultiCompose *tpTplMultiCompose = ( CTemplateMultiCompose * )CDataStatic::SearchTpl( nComposeID );
	if ( tpTplMultiCompose == NULL )
	{								
		return;
	}
	
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	if ( tpProperty->GetBaggage()->Space( tpTplMultiCompose->mItemID[0], nItemNum ) == false )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_MULTICOMFULL );
		return;
	}
	
	CMessage tMsgHead;
	CMessageCorpsExchangeRequest tMsgCorpsExchange;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_REQUEST_CORPSEXCHANGE );
	tMsgHead.set_msgpara( (int)&tMsgCorpsExchange );

	tMsgCorpsExchange.set_composeid( nComposeID );
	tMsgCorpsExchange.set_roleid( pPlayer->GetCharID() );
	tMsgCorpsExchange.set_npctmpid( tpNpcTempLate->mTempID );
	tMsgCorpsExchange.set_corpsid( pPlayer->GetCorpsID() );
	tMsgCorpsExchange.set_itemnum( nItemNum );

	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );		
}

// ***********************************************************
//  Function:	  OnGateResponseCorpsExchange
//  Description:  军团兑换的回应消息
//  Output:			
//  Date:	01/13/2010
// *********************************************************
void CCorpsModule::OnGateResponseCorpsExchange( CMessage *pMsg )
{	
	LK_ASSERT( pMsg != NULL, return )	
	CMessageCorpsExchangeResponse *tpMsg = ( CMessageCorpsExchangeResponse * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->roleid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "corps","[%s,%d] familyexchange error, can not find player, roleid:%d, composeid:%d, errcode:%d  ",__FUNCTION__, __LINE__, tpMsg->roleid(), tpMsg->composeid(), tpMsg->errcode() );
		return;
	}
	
	if ( tpMsg->errcode() == SUCCESS )
	{
		CTemplateMultiCompose *tpTmpCompose = ( CTemplateMultiCompose * )CDataStatic::SearchTpl( tpMsg->composeid() );
		if ( tpTmpCompose != NULL )
		{
			CTplItem *tpTplItem = ( CTplItem * )CDataStatic::SearchTpl( tpTmpCompose->mItemID[0] );
			if ( tpTplItem == NULL )
			{
				LOG_ERROR( "corps","[%s,%d] can not find item templateid by id:%d, roleid:%d, composeid:%d ", __FUNCTION__, __LINE__, tpTmpCompose->mItemID[0], tpMsg->roleid(), tpTmpCompose->mTempID );
				return;
			}
			if ( tpTplItem->mPileLimit <= 1 )
			{
				for ( int i = 1; i <= (int)tpMsg->itemnum(); ++i )
				{
					CItemObject * pInsItemPile = CPropertyModule::GetSingleton().PlayerInsertItem( tpPlayer, tpTmpCompose->mItemID[0], 1 );
					if ( pInsItemPile != NULL )
					{
						LogEventGetItemByCompose( tpPlayer, pInsItemPile->GetGUID(), tpTmpCompose->mItemID[ 0 ], 1,
							tpTmpCompose->mMaterialID[ 0 ], tpTmpCompose->mMaterialNum[ 0 ] );
					}					
				}			
			}
			else
			{
				CItemObject * pInsItemPile = CPropertyModule::GetSingleton().PlayerInsertItem( tpPlayer, tpTmpCompose->mItemID[0], tpMsg->itemnum() );
				if ( pInsItemPile != NULL )
				{
					LogEventGetItemByCompose( tpPlayer, pInsItemPile->GetGUID(), tpTmpCompose->mItemID[ 0 ], tpMsg->itemnum(),
						tpTmpCompose->mMaterialID[ 0 ], tpTmpCompose->mMaterialNum[ 0 ] );
				}		
			}					
		}
		//CPropertyModule::GetSingleton().PlayerMultiCom( tpPlayer, tpMsg->composeid() );
	}
	else
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( tpPlayer, tpMsg->errcode() );
	}
}

// ***********************************************************
//  Function:	  AddCorpsExchangeItem
//  Description:  增加军团兑换物品
//  Output:			
//  Date:	01/15/2010
// *********************************************************
int CCorpsModule::AddCorpsExchangeItem( CEntityPlayer *pPlayer, CTplLeagueBag *tpTplLeagueBag )
{
	LK_ASSERT( pPlayer != NULL && tpTplLeagueBag != NULL, return -1 )	
	if ( pPlayer->GetCorpsID() == 0 )
	{
		return ERROR_CORPS_USELEAGUEBAG_HASNOCORPS;
	}	
		
	CMessage tMsgHead;
	CMessageAddCorpsExchangeItemNotify tMsgAddCorpsExchangeItem;	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_REQUEST_ADDCORPSEXCHANGEITEM );
	tMsgHead.set_msgpara( (int)&tMsgAddCorpsExchangeItem );	
	tMsgAddCorpsExchangeItem.set_roleid( pPlayer->GetCharID() );
	tMsgAddCorpsExchangeItem.set_corpsid( pPlayer->GetCorpsID() );	
	tMsgAddCorpsExchangeItem.set_leaguebagtmpid( tpTplLeagueBag->mTempID );
	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
	return SUCCESS;
}

void CCorpsModule::OnMessageGetCorpsBattleBidRank(CEntityPlayer *pPlayer, CMessage *pMessage)
{
	if( pPlayer == NULL || pMessage == NULL ) return ;

	CMessageCorpsQueryBidRankRequest* tpReq = ( CMessageCorpsQueryBidRankRequest* ) pMessage->msgpara();

	CTemplateNpc* tpTplNpc = ( CTemplateNpc* ) CDataStatic::SearchTpl( tpReq->npcid() );
	if ( tpTplNpc == NULL )
	{
		return ;
	}

	// 检查是否有军团战服务
	if ( tpTplNpc->mCorpsBattle == 0 )
	{
		return ;
	}

	tpReq->set_battlecity( tpTplNpc->mBattleCity );

	SendCorpsMsgToGate( pMessage, tpReq, pPlayer );	
}

void CCorpsModule::OnMessageCorpsBattleBidRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsBattleBidRequest* tpReq = ( CMessageCorpsBattleBidRequest* ) pMessage->msgpara();

	CTemplateNpc* tpTplNpc = ( CTemplateNpc* ) CDataStatic::SearchTpl( tpReq->npcid() );
	if ( tpTplNpc == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageCorpsBattleBidResponse tPara;
	tPara.mutable_corphead()->set_roleid( pPlayer->GetCharID() );
	tPara.mutable_corphead()->set_entityid( pPlayer->GetEntityID() );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_BATTLEBID );
	tMessage.set_msgpara( (long) &tPara );

	int tCorpsID = pPlayer->GetCorpsID();
	if ( tCorpsID == INVALID_OBJ_ID )
	{
		tPara.set_rtncode( ERROR_CORPS_NOTEXIST );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return ;
	}

	// 没有军团战服务
	if ( tpTplNpc->mCorpsBattle == 0 )
	{
		tPara.set_rtncode( ERROR_CORPS_CANTBATTLE );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return ;
	}

	CPropertyPlayer* tpProperty = ( CPropertyPlayer* ) pPlayer->GetProperty();

	CEntityNpc* tpNpc = dynamic_cast< CEntityNpc* >( CSceneObjManager::GetSingletonPtr()->GetObject( tpProperty->GetTalkNpcEntityID() ) );
	if ( tpNpc == NULL )
	{
		LOG_DEBUG( "pro", "talk npc entity not exist,id=%d[%s:%d]", tpProperty->GetTalkNpcEntityID(), __LK_FILE__, __LINE__ );
		return ;
	}

	tpReq->set_mapid( tpTplNpc->mBattleCity );

	CORPS_CFG* tpCfg = CSceneCfgManager::GetSingleton().GetCorps_cfg();
	
	if ( tpReq->money() < tpCfg->mMinbidmoney )
	{
		// 竞价输入有个下限，目前为5金
		tPara.set_rtncode( ERROR_CORPS_BIDMONEYLOWER );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return ;
	}	

	tpReq->set_minincmoney( tpTplNpc->mMinBidPrice );

	SendCorpsMsgToGate( pMessage, tpReq, pPlayer );

}

void CCorpsModule::OnMessageSelectBattleFamilyRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsSelectFamilyBattleRequest* tpReq = ( CMessageCorpsSelectFamilyBattleRequest* ) pMessage->msgpara();

	SendCorpsMsgToGate( pMessage, tpReq, pPlayer ); 
}

void CCorpsModule::OnMessageSelectBattleMemberRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsSelectMemberBattleRequest* tpReq = ( CMessageCorpsSelectMemberBattleRequest* ) pMessage->msgpara();

	//TODO: 先临时加一下，不是个很好的解决办法，为了避免玩家在战争中修改参战阵容，让本不应该进来的玩家进来，限制报名时间
	//if ( IntTimeCompareToStringTime( time(NULL), "21:30:00") > 0 
	//		&& IntTimeCompareToStringTime( time(NULL), "22:15:00") < 0 )
	//{
	//	CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_NOT_CORPS_APPLY_TIME ); 
	//	return;
	//}

	SendCorpsMsgToGate( pMessage, tpReq, pPlayer );
}

void CCorpsModule::OnMessageCorpsChallegeRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	CorpsHead* tpHead = NULL;

	int tMessageID = pMessage->mutable_msghead()->messageid();
	switch ( tMessageID )
	{
	case ID_C2S_REQUEST_ROOMOPERATION:
		{
			CMessageCorpsOperateRoomRequest* tpReq = ( CMessageCorpsOperateRoomRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
			
			break;
		}
	case ID_C2S_REQUEST_CHALLEGE:
		{
			CMessageCorpsChallegeRequest* tpReq = ( CMessageCorpsChallegeRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
		
			break;
		}
	case ID_C2S_REQUEST_REPLYCHALLEGE:
		{
			CMessageCorpsReplyChallegeRequest* tpReq = ( CMessageCorpsReplyChallegeRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();

			break;
		}
	case ID_C2S_REQUEST_QUERYROOM:
		{
			CMessageCorpsQueryRoomListRequest* tpReq = ( CMessageCorpsQueryRoomListRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
			break;
		}
	case ID_C2S_REQUEST_QUERYCHALLEGELIST:
		{
			CMessageCorpsQueryChallegeListRequest* tpReq = ( CMessageCorpsQueryChallegeListRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
			break;
		}
	case ID_C2S_REQUEST_QUERYRECCHALLEGE:
		{
			CMessageCorpsQueryRecChallegeListRequest* tpReq = ( CMessageCorpsQueryRecChallegeListRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
			break;
		}
	case ID_C2S_REQUEST_ABORTBATTLECITY:
		{
			CMessageCorpsAbortBattleCityRequest* tpReq = ( CMessageCorpsAbortBattleCityRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
			break;
		}
	case ID_C2S_REQUEST_GETOTHERMEMBER:
		{
			CMessageCorpsGetOtherMemberRequest* tpReq = ( CMessageCorpsGetOtherMemberRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
			break;
		}
	case ID_C2S_REQUEST_GETALLMAP:
		{
			CMessageCorpsGetAllMapRequest* tpReq = ( CMessageCorpsGetAllMapRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
			break;
		}
	case ID_C2S_REQUEST_UNIONSUBMIT:
		{
			CMessageUnionJoinBattleListRequest* tpReq = ( CMessageUnionJoinBattleListRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
			break;
		}
	case ID_C2S_REQUEST_APPOINTPRINCE:
		{
			CMessageCorpsAppointPrinceRequest* tpReq = ( CMessageCorpsAppointPrinceRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
			break;
		}
	case ID_C2S_REQUEST_MOVECAPITAL:
		{
			CMessageCorpsMoveCapitalRequest* tpReq = ( CMessageCorpsMoveCapitalRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
			break;
		}
	case ID_C2S_REQUEST_SELECTCOLOR:
		{
			CMessageCorpsSelectColorRequest* tpReq = ( CMessageCorpsSelectColorRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
			break;
		}
	case ID_C2S_REQUEST_LOGINBATTLE:
		{
			CMessageCorpsLoginBattleRequest* tpReq = ( CMessageCorpsLoginBattleRequest* ) pMessage->msgpara();
			tpHead = tpReq->mutable_corphead();
			
			//// 配置为0的话，表示不需要buff约束
			//int nBattleBuff = CSceneCfgManager::GetSingletonPtr()->GetCorps_cfg()->mEnterbattlebuffid;
			//if(  ( nBattleBuff != 0 ) && ( pPlayer->HasBuff( nBattleBuff ) != true ) )
			//{
			//	CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_CORPS_CANTLOGINBATTLE ); 
			//	return ;			
			//}

			break;
		}

	default:
		break;
	}

	if ( tpHead == NULL )
	{
		return ;
	}
	tpHead->set_roleid( pPlayer->GetCharID() );
	tpHead->set_entityid( pPlayer->GetEntityID() );
	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

bool CCorpsModule::CheckInBidOpenTime(int nNpcID)
{
	return false;
}

// ***********************************************************
//  Function:	  AddPlayerCorpsContribute
//  Description:  增加玩家的贡献度
//  Output:			
//  Date:	09/02/2009
// *********************************************************
void CCorpsModule::AddPlayerCorpsContribute( int nRoleID, int nContribute, int nAddType /* = 0 */ )
{  
	CMessage tMsgHead;
	CMessageChangeCorpsContributeNotify tMsgChangeContribute;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_NOTIFY_CHANGECORPSCONTRIBUTE );
	tMsgHead.set_msgpara( (int)&tMsgChangeContribute );
	
	tMsgChangeContribute.set_contribute( nContribute );
	tMsgChangeContribute.set_roleid( nRoleID );
	tMsgChangeContribute.set_changetype( nAddType );
	
	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}

// ***********************************************************
//  Function:	  ChangeCorpsCollectActive
//  Description:  改变军团的采集活跃度的值
//  Output:			
//  Date:	
void CCorpsModule::ChangeCorpsCollectActive( int nRoleID, int nChangeValue, int nChangeType )
{
	CMessage tMsgHead;
	CMessageChangeCorpsCollectActive tMsgChangeCollectActive;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_CHANGE_CORPSCOLLECTACTIVE );
	tMsgHead.set_msgpara( (int)&tMsgChangeCollectActive );

	tMsgChangeCollectActive.set_roleid( nRoleID );
	tMsgChangeCollectActive.set_activevalue( nChangeValue );
	tMsgChangeCollectActive.set_changetype( nChangeType );

	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}



// ***********************************************************
//  Function:	  ChangeCorpsMoney
//  Description:  改变家族金钱
//  Output:			
//  Date:	09/02/2009
// *********************************************************
void CCorpsModule::ChangeCorpsMoney( CEntityPlayer *pPlayer, int nMoney, bool bPayOut /* = false */, int nReason /* = REASON_TASK */ )
{ 
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageChangeCorpsMoneyNotify tMsgChangeCorpsMoney;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_NOTIFY_CHANGECORPSMONEY );	
	tMsgHead.set_msgpara( (int)&tMsgChangeCorpsMoney );
	
	tMsgChangeCorpsMoney.set_corpsid( pPlayer->GetCorpsID() );
	tMsgChangeCorpsMoney.set_money( nMoney );
	tMsgChangeCorpsMoney.set_ifpayout( bPayOut );
	tMsgChangeCorpsMoney.set_roleid( pPlayer->GetCharID() );
	tMsgChangeCorpsMoney.set_reason( nReason );
	tMsgChangeCorpsMoney.set_rolename( pPlayer->GetCharName() );
	
	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}
// ***********************************************************
//  Function:	  ChangeCorpsBossTimes
//  Description:  增加打boss的次数
//  Output:			
//  Date:	06/22/2011
// *********************************************************
void CCorpsModule::ChangeCorpsBossTimes( CEntityPlayer *pPlayer, int nBossId, int nTimes, int nReason)
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageChangeCorpsBossTimes tMsgChangeCorpsBossTimes;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_CHANGE_CORPSBOSSTIMES );
	tMsgHead.set_msgpara( (int)& tMsgChangeCorpsBossTimes );

	tMsgChangeCorpsBossTimes.set_corpsid( pPlayer->GetCorpsID() );

	tMsgChangeCorpsBossTimes.set_roleid(pPlayer->GetCharID() );
	tMsgChangeCorpsBossTimes.set_times( nTimes );
	tMsgChangeCorpsBossTimes.set_bossid( nBossId);
	tMsgChangeCorpsBossTimes.set_reason( nReason );
	tMsgChangeCorpsBossTimes.set_rolename( pPlayer->GetCharName() );

	CSceneLogic::GetSingleton().Send2Gate(&tMsgHead);

}
// *********************************************************
// 改变 军团的炼化值 
//
// *********************************************************
void CCorpsModule:: ChangeCorpsBOSSRefineValue( CEntityPlayer * pPlayer, int nValue , int nReason )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageChangeCorpsBossRefineValue tMsgChangeValue;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_CHANGE_CORPSBOSSREFINEVALUE );
	tMsgHead.set_msgpara( (int)& tMsgChangeValue );

	tMsgChangeValue.set_corpsid( pPlayer->GetCorpsID() );
	tMsgChangeValue.set_roleid( pPlayer->GetCharID() );
	tMsgChangeValue.set_reason( nReason );
	tMsgChangeValue.set_refinevalue( nValue );
	tMsgChangeValue.set_rolename( pPlayer->GetCharName());


	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );	
}
// ***********************************************************
//  Function:	  ChangeCorpsGlory
//  Description:  改变家族荣耀值
//  Output:			
//  Date:	09/02/2009
// *********************************************************
void CCorpsModule::ChangeCorpsGlory( CEntityPlayer *pPlayer, int nValue, int bReduced /* = false */, int nReason /* = REASON_TASK */ )
{ 
	CMessage tMsgHead;
	CMessageChangeCorpsGloryNotify tMsgChangeCorpsGlory;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_NOTIFY_CHANGECORPSGLORY );
	tMsgHead.set_msgpara( (int)&tMsgChangeCorpsGlory );
	
	tMsgChangeCorpsGlory.set_corpsid( pPlayer->GetCorpsID() );
	tMsgChangeCorpsGlory.set_glory( nValue );
	tMsgChangeCorpsGlory.set_ifreduced( bReduced );
	tMsgChangeCorpsGlory.set_roleid( pPlayer->GetCharID() );	
	tMsgChangeCorpsGlory.set_reason( nReason );
	tMsgChangeCorpsGlory.set_rolename( pPlayer->GetCharName() );
	
	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}			

// ***********************************************************
//  Function:	  OnGSCreatCorpsResponse
//  Description:  创建军团的回应消息
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSCreatCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{	
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageCreatCorpsResponse *tpMsg = ( CMessageCreatCorpsResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty(); 
	tpProperty->SetMoney( tpProperty->GetMoney() + CORPS_MONEY_CREATCOPRS );
	pPlayer->SetCorpsID( 0 );
	LOG_NOTICE( "corps","[%s,%d][%s] Response of creat corps:  player(roleid:%d), errcode( %d ) ", __LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetCharID(), tpMsg->errcode() );
	if ( tpMsg->errcode() == 0 )
	{
		// 设置军团ID								   
		pPlayer->SetCorpsID( tpMsg->corpsid() );
		
		CFamilyInfo *tpFamilyInfo = CFamilyModule::GetSingleton().GetFamilyInfo( pPlayer->GetFamilyID() );
		if ( tpFamilyInfo != NULL )
		{
			tpFamilyInfo->SetCorpsName( tpMsg->corpsname().c_str(), true );
			tpFamilyInfo->SetCorpsID( tpMsg->corpsid() );
			CMemberInfo *tpMemberInfo = tpFamilyInfo->GetMemberInfo( pPlayer->GetCharID() );
			if( tpMemberInfo != NULL )
			{
				tpMemberInfo->mCorpsPost = CORPS_POST_CORPSHEADER;				
			}		  
		}
		
		// 设置军团名称
		// pPlayer->SetCorpsName( tpMsg->corpsname().c_str() );
		
		// 军团首领
		// pPlayer->SetIsCorpsHeader( true );
		
		// 扣除金钱
		if (CPropertyModule::GetSingleton().PlayerChangeMoney( pPlayer, CORPS_MONEY_CREATCOPRS, true, false ) == SUCCESS)
		{
			LogEventLostMoneyByBuildCorps(pPlayer, CYuanBao::em_unbind_money, CORPS_MONEY_CREATCOPRS, tpMsg->corpsid());
		}
		else
		{
			LOG_ERROR( "corps","[%s,%d][%s] Creat corps Error, change money error! RoleID:%d, CorpsID:%d ", __LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetCharID(), tpMsg->corpsid() );
		}
		
		// 扣除将军令
		/*int tTokenIndex = -1;
		int tNum		= 0;
		tpProperty->GetBaggage()->HasItemOfCertainType( ITEM_FUNCITEM, tNum, tTokenIndex, FUNCCODE_CORPSTOKEN );
		if ( tNum >= 1 && tTokenIndex >= 0 )
		{							
			CPropertyModule::GetSingleton( ).PlayerRemoveItem( pPlayer, tTokenIndex, 1 );				
		}*/		
	}  	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	 OnGSFamilyJoinCorpsNotify
//  Description: 有家族加入军团的消息通知
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSFamilyJoinCorpsNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageFamilyJoinCorpsNotify *tpMsg = ( CMessageFamilyJoinCorpsNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSFamilyKickedNotify
//  Description:  家族从军团里面被踢掉的通知
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSFamilyKickedNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageKickFamilyNotify *tpMsg = ( CMessageKickFamilyNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	if ( (int)tpMsg->familyid() == pPlayer->GetFamilyID() )
	{
		pPlayer->SetCorpsID( 0 );
	}	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSFamilyLeaveCorpsNotify
//  Description:  家族离开军团的消息通知
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSFamilyLeaveCorpsNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return ) 
	CMessageFamilyLeaveCorpsNotify *tpMsg = ( CMessageFamilyLeaveCorpsNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL , return )
	if ( (int)tpMsg->familyid() == pPlayer->GetFamilyID() )
	{
		pPlayer->SetCorpsID( 0 );
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	 OnGSGetCorpsFamilyInfoResponse
//  Description: 获取军团家族成员信息的回应消息
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSGetCorpsFamilyInfoResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	   OnGSGetCorpsFamilyMemberResponse
//  Description:   获取军团中某个家族的成员信息的消息回应
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSGetCorpsFamilyMemberResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:		OnGSGetCorpsPropertyInfoResponse
//  Description:	获取军团属性信息的回应消息
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSGetCorpsPropertyInfoResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	   OnGSInviteCorpsMemberResponse
//  Description:   邀请家族加入军团的消息回应
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSInviteCorpsMemberResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageInviteCorpsMemberResponse *tpMsg = ( CMessageInviteCorpsMemberResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	   OnGSKickFamilyResponse
//  Description:   从军团踢出家族的消息回应
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSKickFamilyResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{	
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSReplyCorpsInviteResponse
//  Description:  答复军团邀请的消息回复
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSReplyCorpsInviteResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageReplyCorpsInviteResponse *tpMsg = ( CMessageReplyCorpsInviteResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	if ( tpMsg->errcode() == SUCCESS && tpMsg->result() == 1 )
	{
		pPlayer->SetCorpsID( tpMsg->corpsid() );
		if ( tpMsg->has_corpsname() )
		{
			CFamilyInfo *tpFamilyInfo = CFamilyModule::GetSingleton().GetFamilyInfo( pPlayer->GetFamilyID() );
			if ( tpFamilyInfo != NULL )			
			{
				tpFamilyInfo->SetCorpsName( tpMsg->corpsname().c_str( ), true );
				tpFamilyInfo->SetCorpsID( tpMsg->corpsid() );
				//pPlayer->SetCorpsName( tpMsg->corpsname().c_str() );	
			}			
		}		
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
} 

// ***********************************************************
//  Function:	   OnGSReplyMemberApplyResponse
//  Description:   回复家族申请加入军团的消息回应
//  Output:
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSReplyMemberApplyResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:		OnGSApplyCorpsResponse
//  Description:	申请加入军团的回应消息
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSApplyCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	   OnGSApplyCorpsResultNotify
//  Description:   申请加入军团的结果通知
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSApplyCorpsResultNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{	
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageApplyCorpsResultNotify *tpMsg = ( CMessageApplyCorpsResultNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	if ( tpMsg->resultcode() == 1 )
	{
		pPlayer->SetCorpsID( tpMsg->corpsid() );
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:		 OnGSChangeCorpsBulletinNotify
//  Description:	 改变军团公告的消息通知
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSChangeCorpsBulletinNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage !=  NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSChangeCorpsBulletinResponse
//  Description:  改变军团公告的消息回复
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSChangeCorpsBulletinResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSChangeCorpsHeaderNotify
//  Description:  改变军团首领的消息通知
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSChangeCorpsHeaderNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageChangeCorpsHeaderNotify *tpMsg = ( CMessageChangeCorpsHeaderNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	// 改变场景上职位转让玩家的军团职位
	if ( tpMsg->memberroleid() == pPlayer->GetCharID() )
	{
		CMemberInfo *tpMemberinfo = CFamilyModule::GetSingleton().GetFamilyMemberInfo( pPlayer );
		if ( tpMemberinfo != NULL )
		{
			tpMemberinfo->mCorpsPost = CORPS_POST_CORPSHEADER;
			tpMemberinfo->mCorpsHeaderID = pPlayer->GetCharID();
		}
		// pPlayer->SetIsCorpsHeader( true );
	}
		
	// 改变军团的
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSGetCorpsApplyListResponse
//  Description:  获取军团申请列表信息的消息回应
//  Output:			
//  Date:	06/13/2009
// *********************************************************
void CCorpsModule::OnGSGetCorpsApplyListResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSDisbandCorpsResponse
//  Description:  解散军团的消息回应
//  Output:			
//  Date:	06/13/2009
// *********************************************************
void CCorpsModule::OnGSDisbandCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL &&  pMessage != NULL, return ) 	
	CMessageDisbandCorpsResponse *tpMsg = ( CMessageDisbandCorpsResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	if ( tpMsg->errcode() == SUCCESS ) 
	{
		// pPlayer->SetCorpsID( 0 );
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSDisbandCorpsNotify
//  Description:  解散军团的消息通知
//  Output:			
//  Date:	06/13/2009
// *********************************************************
void CCorpsModule::OnGSDisbandCorpsNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	// pPlayer->SetCorpsID( 0 );
	// pPlayer->SetCorpsPost( -1 );
	CFamilyInfo *tpFamilyInfo = CFamilyModule::GetSingleton().GetFamilyInfo( pPlayer->GetFamilyID() );
	if ( tpFamilyInfo != NULL )
	{
		tpFamilyInfo->SetCorpsID( 0 );
		CMemberInfo *tpMemberInfo = tpFamilyInfo->GetMemberInfo( pPlayer->GetCharID() );
		if ( tpMemberInfo != NULL )
		{
			tpMemberInfo->mCorpsPost = CORPS_POST_MEMBER;
		}
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSCancelDisbandCorpsResponse
//  Description:  取消解散家族的消息回应
//  Output:			
//  Date:	06/18/2009
// *********************************************************
void CCorpsModule::OnGSCancelDisbandCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )		
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSRefreshCorpsInfoResponse
//  Description:  刷新军团信息的消息回应
//  Output:			
//  Date:	06/23/2009
// *********************************************************
void CCorpsModule::OnGSRefreshCorpsInfoResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSClearCorpsResponse
//  Description:  清除军团申请信息的消息回应
//  Output:			
//  Date:	06/30/2009
// *********************************************************
void CCorpsModule::OnGSClearCorpsApplyResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSGetCorpsListResponse
//  Description:  获取军团列表的消息返回
//  Output:			
//  Date:	08/05/2009
// *********************************************************
void CCorpsModule::OnGSGetCorpsListResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	 OnGSChangeCorpsHeaderResponse
//  Description: 改变军团首领的消息回复
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSChangeCorpsHeaderResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageChangeCorpsHeaderResponse *tpMsg = ( CMessageChangeCorpsHeaderResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	if ( tpMsg->errcode() == 0 )
	{
		CMemberInfo *tpMemberinfo = CFamilyModule::GetSingleton().GetFamilyMemberInfo( pPlayer );
		if ( tpMemberinfo != NULL )
		{
			tpMemberinfo->mCorpsPost = CORPS_POST_FAMILYHEADER;
			tpMemberinfo->mCorpsHeaderID = tpMsg->memberid();			
		}
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	  OnGSCorpsInviteNotify
//  Description:   邀请加入军团的消息通知
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSCorpsInviteNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:		OnGSCorpsInviteResultNotify
//  Description:	邀请家族加入军团的结果通知
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSCorpsInviteResultNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:	   OnGSCorpsMemberApplyNotify
//  Description:   有家族申请加入军团的消息通知
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSCorpsMemberApplyNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}  

// ***********************************************************
//  Function:		OnGSFamilyLeaveCorpsResponse
//  Description:	家族离开军团的消息回应
//  Output:			
//  Date:	06/10/2009
// *********************************************************
void CCorpsModule::OnGSFamilyLeaveCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{		
	LK_ASSERT( pMessage != NULL && pMessage != NULL, return )
	CMessageFamilyLeaveCorpsResponse *tpMsg = ( CMessageFamilyLeaveCorpsResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	if( tpMsg->errcode() == 0 )
	{
		pPlayer->SetCorpsID( 0 );	  		
	}		
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

void CCorpsModule::OnGSQueryBidRankResponse(CEntityPlayer *pPlayer, CMessage *pMessage)
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

void CCorpsModule::OnGSSelectBattleFamilyResponse( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

void CCorpsModule::OnGSSelectBattleMemberResponse( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	CMessageCorpsSelectMemberBattleResponse* tpResp = ( CMessageCorpsSelectMemberBattleResponse* ) pMessage->msgpara();

	if (ERROR_NOT_CORPS_APPLY_TIME == (int)tpResp->rtncode())
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_NOT_CORPS_APPLY_TIME ); 
		return;
	}
	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

void CCorpsModule::OnGSCorpsBattleRightsNotify( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}
void CCorpsModule::OnGSFamilyBattleRightsNotify( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}
void CCorpsModule::OnGSMemberBattleRightsNotify( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

void CCorpsModule::OnGSCorpsBidResponse( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

void CCorpsModule::OnGSCorpsChallgeReponse( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );

	//
	unsigned int tMsgID = pMessage->mutable_msghead()->messageid();

	if ( tMsgID == ID_S2C_RESPONSE_ABORTBATTLECITY )
	{
		CMessageCorpsAbortBattleCityResponse* tpResponse = ( CMessageCorpsAbortBattleCityResponse* ) pMessage->msgpara();
		if ( tpResponse->rtncode() != SUCCESS )
		{
			return ;
		}
		
		CorpsNpcList::iterator it = mNpcList.find( tpResponse->npcid() );
		if ( it != mNpcList.end() )
		{
			mNpcList.erase( it );
		}
	}
}

// ***********************************************************
//  Function:		OnGSSetCorpMemberPostResponse
//  Description:	设置军团成员职位的消息回应
//  Output:			
//  Date:	08/25/2009
// *********************************************************	
void CCorpsModule::OnGSSetCorpMemberPostResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{	
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:		OnGSLevelUPCorpsResponse
//  Description:	升级军团的消息回应
//  Output:			
//  Date:	08/25/2009
// *********************************************************	
void CCorpsModule::OnGSLevelUPCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}  

// ***********************************************************
//  Function:		OnGSContributeCorpsMoneyResponse
//  Description:	贡献军团金钱的消息回应
//  Output:			
//  Date:	08/25/2009
// *********************************************************
void CCorpsModule::OnGSContributeCorpsMoneyResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageContributeCorpsMoneyResponse *tpMsg = ( CMessageContributeCorpsMoneyResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	
	tpProperty->SetMoney( tpProperty->GetMoney() + tpMsg->money()*MONEY_EXCHANGERATE );
	if ( tpMsg->errcode() == SUCCESS )
	{
		if (CPropertyModule::GetSingleton().PlayerChangeMoney( pPlayer, tpMsg->money()*MONEY_EXCHANGERATE, true, false ) == SUCCESS)
		{
			//TODO: 
			LogEventLostMoneyByContrCorps( pPlayer, CYuanBao::em_unbind_money, tpMsg->money()*MONEY_EXCHANGERATE, pPlayer->GetCorpsID());
			LOG_NOTICE( "corps"," contribute money sucess ( [%s,%d] roleid:%d, money:%d, corps:%d ) ",__FUNCTION__, __LINE__, pPlayer->GetCharID(), tpMsg->money(), pPlayer->GetCorpsID() );
		}
		else
		{
			tpProperty->SetMoney( tpProperty->GetMoney() - tpMsg->money()*MONEY_EXCHANGERATE );
			LOG_ERROR( "corps","[%s,%d] change contribute money failed ( roleid:%d, money:%d, corps:%d ) ",__FUNCTION__, __LINE__, pPlayer->GetCharID(), tpMsg->money(), pPlayer->GetCorpsID() );
		}
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage ); 	
}
// ***********************************************************
//  Function:		OnGSFamilyContributeMoneyResponse
//  Description:	家族给军团贡献金钱的消息回应
//  Output:			
//  Date:	08/25/2009
// *********************************************************
void CCorpsModule::OnGSFamilyContributeMoneyResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	
}

// ***********************************************************
//  Function:		OnGSSetMemberNameResponse
//  Description:	设置军团成员称号的消息回应
//  Output:			
//  Date:	08/25/2009
// *********************************************************
void CCorpsModule::OnGSSetMemberNameResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:		OnGSSetCorpsPostNotify
//  Description:	设置军团职位的通知
//  Output:			
//  Date:	08/25/2009
// *********************************************************
void CCorpsModule::OnGSSetCorpsPostNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageSetCorpsPostNotify *tpMsg = ( CMessageSetCorpsPostNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	if ( tpMsg->memberid() == pPlayer->GetCharID() )
	{												
		CMemberInfo *tpMemberInfo = CFamilyModule::GetSingleton().GetFamilyMemberInfo( pPlayer );
		if ( tpMemberInfo != NULL )
		{
			tpMemberInfo->mCorpsPost = tpMsg->newpost();
		}
		//pPlayer->SetCorpsPost( tpMsg->newpost() );
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:		OnGSCorpsLevelUPNotify
//  Description:	设置军团职位的通知
//  Output:			
//  Date:	08/25/2009
// *********************************************************
void CCorpsModule::OnGSCorpsLevelUPNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:		OnGSCorpsPVPStarLevelUP
//  Description:	军团pvp等级提升的通知
//  Output:			
//  Date:	08/25/2009
// *********************************************************
void CCorpsModule::OnGSCorpsPVPStarLevelUPNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	
}

// ***********************************************************
//  Function:		OnGateMsgQueryNpcWelfareResponse
//  Description:	查询军团npc福利的消息回应
//  Output:			
//  Date:	09/16/2009
// *********************************************************
void CCorpsModule::OnGateMsgQueryNpcWelfareResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )		
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}  

// ***********************************************************
//  Function:		OnGateMsgObtainNpcWelfareResponse
//  Description:	领取军团npc福利的消息回应
//  Output:			
//  Date:	09/16/2009
// *********************************************************
void CCorpsModule::OnGateMsgObtainNpcWelfareResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}  

// ***********************************************************
//  Function:		OnGateMsgGetCorpsMoneyResponse
//  Description:	获取军团金钱的消息回应
//  Output:			
//  Date:	09/16/2009
// *********************************************************
void CCorpsModule::OnGateMsgGetCorpsMoneyResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageGetCorpsMoneyResponse *tpMsg = ( CMessageGetCorpsMoneyResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	if ( tpMsg->errcode() == SUCCESS )
	{

		if (CPropertyModule::GetSingleton().PlayerChangeMoney( pPlayer, tpMsg->corpsmoney()*MONEY_EXCHANGERATE, false, false ) == SUCCESS)
		{
			LogEventGetMoneyByCorps( pPlayer, CYuanBao::em_unbind_money, tpMsg->corpsmoney()*MONEY_EXCHANGERATE, pPlayer->GetCorpsID());
		}
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// ***********************************************************
//  Function:		CheckNpcCorpsService
//  Description:	检查npc是否有军团的服务
//  Output:			
//  Date:	06/10/2009
// ********************************************************* 
bool CCorpsModule::CheckNpcCorpsService( CEntityPlayer *pPlayer, int nNpcEntityID, int eService )
{ 
	LK_ASSERT( pPlayer != NULL, return false )
	
	// 家族npc的判断
	CEntityNpc *tpEntity =  dynamic_cast< CEntityNpc * >( CSceneObjManager::GetSingletonPtr()->GetObject( nNpcEntityID ) );
	LK_ASSERT( tpEntity != NULL, return false )
	
	// 服务判断
	CPropertyNPC * tpNpcProperty = ( CPropertyNPC * )tpEntity->GetProperty(); 
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpNpcProperty->GetPropertyTempID() );
	LK_ASSERT( tpTmpNpc != NULL, return false )
	if ( tpTmpNpc->mCorpService != eService )
	{
		return false;
	}

	// 距离判断
	return pPlayer->IsWithinDistance( tpEntity, MAX_NPC_TALK_GIRD );
}

// 是否有军团联盟服务
bool CCorpsModule::HasUnionService( CEntityPlayer *pPlayer, int vNpcEntityID )
{
	LK_ASSERT( pPlayer != NULL, return false );

	// 家族npc的判断
	CEntityNpc *tpEntity =  dynamic_cast< CEntityNpc * >( CSceneObjManager::GetSingletonPtr()->GetObject( vNpcEntityID ) );
	LK_ASSERT( tpEntity != NULL, return false );

	// 服务判断
	CPropertyNPC * tpNpcProperty = ( CPropertyNPC * )tpEntity->GetProperty(); 
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpNpcProperty->GetPropertyTempID() );
	LK_ASSERT( tpTmpNpc != NULL, return false );
	if ( tpTmpNpc->mUnionService == 0 )
	{
		return false;
	}

	// 距离判断
	return pPlayer->IsWithinDistance( tpEntity, MAX_NPC_TALK_GIRD );
}
 
// ***********************************************************
//  Function:		OnMessageSetCorpsPost
//  Description:	设置军团成员职位的消息请求
//  Output:			
//  Date:	08/25/2009
// ********************************************************* 
void CCorpsModule::OnMessageSetCorpsPost( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageSetCorpsPostRequest *tpMsg = ( CMessageSetCorpsPostRequest * )pMessage->msgpara();
	if ( tpMsg == NULL )
	{
		return;
	}
	// 密码安全检查
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}	
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer ) 		
}

// ***********************************************************
//  Function:		OnMessageLevelUPCorps
//  Description:	升级军团的消息请求
//  Output:			
//  Date:	08/25/2009
// ********************************************************* 
void CCorpsModule::OnMessageLevelUPCorps( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageLevelUPCorpsRequest *tpMsg = ( CMessageLevelUPCorpsRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	CMemberInfo *tpMemberInfo = CFamilyModule::GetSingleton().GetFamilyMemberInfo( pPlayer );
	if ( tpMemberInfo == NULL )
	{
		LOG_ERROR( "corps","[%s,%d] can not find family member info ( familyid:%d, roleid:%d )",__FUNCTION__,__LINE__, pPlayer->GetFamilyID(), pPlayer->GetCharID() );	
		return;
	}
	
	if ( tpMemberInfo->mFamilyPost != FAMILY_POST_HEADER )
	{
		CMessage tMsgHead;
		CMessageLevelUPCorpsResponse tMsgLevelupCorps;
		tMsgHead.mutable_msghead()->set_messageid( ID_C2S_REQUEST_LEVELUPCORPS );
		tMsgHead.set_msgpara( (int)&tMsgLevelupCorps );
		tMsgLevelupCorps.set_errcode( ERROR_CORPS_HASNORIGHT );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}
	
	/*if ( !pPlayer->GetIsFamilyHeader() )
	{
		CMessage tMsgHead;
		CMessageLevelUPCorpsResponse tMsgLevelupCorps;
		tMsgHead.mutable_msghead()->set_messageid( ID_C2S_REQUEST_LEVELUPCORPS );
		tMsgHead.set_msgpara( (int)&tMsgLevelupCorps );
		tMsgLevelupCorps.set_errcode( ERROR_CORPS_HASNORIGHT );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}	*/
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer ) 
}

// ***********************************************************
//  Function:		OnMessageContributeCorpsMoney
//  Description:	贡献军团金钱的消息请求
//  Output:			
//  Date:	08/25/2009
// *********************************************************
void CCorpsModule::OnMessageContributeCorpsMoney( CEntityPlayer *pPlayer, CMessage *pMessage )
{ 
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageContributeCorpsMoneyRequest *tpMsg = ( CMessageContributeCorpsMoneyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )

	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )	pPlayer->GetProperty();
	if ( tpProperty->GetMoney() < (int)( tpMsg->money()*MONEY_EXCHANGERATE )  || (int)( tpMsg->money()*MONEY_EXCHANGERATE ) < 0 
		 || ( int )tpMsg->money() < 0 )
	{
		CMessage tHead;
		CMessageContributeCorpsMoneyResponse tMsgContributeMoney;

		tHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CONTRIBUTECORPSMONEY );
		tHead.set_msgpara( (int)&tMsgContributeMoney );

		tMsgContributeMoney.set_money( tpMsg->money() );
		tMsgContributeMoney.set_errcode( ERROR_CORPS_FAMILYMONEYNOTENOUGH );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tHead );
		return;
	}

	//CPropertyModule::GetSingleton().PlayerChangeMoney( pPlayer, tpMsg->money()*MONEY_EXCHANGERATE, true, false );
	//tpMsg->set_money( tpMsg->money() );
	// 先扣金钱
	tpProperty->SetMoney( tpProperty->GetMoney() - tpMsg->money()*MONEY_EXCHANGERATE );
	LOG_NOTICE( "corps","[%s,%d] player contribute corps money ( roleid:%d, money:%d, corps:%d ) ", __FUNCTION__,__LINE__, pPlayer->GetCharID(), tpMsg->money(), pPlayer->GetCorpsID() );
	
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );		
}

// ***********************************************************
//  Function:		OnMessageFamilyContributeMoney
//  Description:	家族给军团贡献金钱的消息请求
//  Output:			
//  Date:	08/25/2009
// *********************************************************
void CCorpsModule::OnMessageFamilyContributeMoney( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	
}	

// ***********************************************************
//  Function:		OnMessageSetMemberName
//  Description:	设置军团成员称号的消息请求
//  Output:			
//  Date:	08/25/2009
// *********************************************************
void CCorpsModule::OnMessageSetMemberName( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageSetMemberNameRequest  *tpMsg = ( CMessageSetMemberNameRequest  * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )  
}

void CCorpsModule::OnGSCorpsJoinBattleNotice(CMessage *pMessage)
{
	if ( pMessage ==  NULL )
	{
		return ;
	}

	CMessageCorpsJoinBattleNotice* tpNotice = ( CMessageCorpsJoinBattleNotice* ) pMessage->msgpara();

	CTemplateNpc* tpNpc = ( CTemplateNpc* ) CDataStatic::SearchTpl( tpNotice->npcid() );
	if ( tpNpc == NULL )
	{
		return ;
	}
	 


	CMessage tMessage;
	CMessageS2CBattleJoinNotice tPara;
	tMessage.set_msgpara( (long) &tPara );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_BATTLE_JOIN_NOTICE );
	//tPara.set_battleid( tpNpc->mBattleCity );
	tPara.set_battleid( 66 );

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpNotice->charid() );
	if ( tpPlayer  != NULL )
	{
		CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
	}
	return ;
}

int CCorpsModule::OnStartFireBattleTimer( int nEntityID )
{
	CEntityNpc* tpEntityNpc = dynamic_cast< CEntityNpc* > ( CSceneObjManager::GetSingletonPtr()->GetObject( nEntityID ) );
	if ( tpEntityNpc == NULL )
	{
		return -1;
	}

	CTemplateNpc* tpTplNpc = ( CTemplateNpc* ) CDataStatic::SearchTpl( tpEntityNpc->GetNpcID() );
	if ( tpTplNpc == NULL )
	{
		return -1;
	}

	if ( tpTplNpc->mCorpsBattle == 0 )
	{
		return -1;
	}

	CPropertyNPC* tpProNpc = ( CPropertyNPC* ) tpEntityNpc->GetProperty();

	if ( tpProNpc->GetBattleTimer() != INVALID_OBJ_ID )
	{
		return -1;
	}

	time_t tNow = time( NULL );
	struct tm* tLocal = localtime( &tNow );

	if ( ( ( ( tpTplNpc->mCorpsBidDay & CTemplateNpc::WDAY_MONDAY ) != CTemplateNpc::WDAY_MONDAY ) && tLocal->tm_wday == 1 )	  || 
		( ( ( tpTplNpc->mCorpsBidDay & CTemplateNpc::WDAY_TUESDAY ) != CTemplateNpc::WDAY_TUESDAY ) && tLocal->tm_wday == 2 )     ||
		( ( ( tpTplNpc->mCorpsBidDay & CTemplateNpc::WDAY_WEDENSDAY ) != CTemplateNpc::WDAY_WEDENSDAY ) && tLocal->tm_wday == 3 ) ||
		( ( ( tpTplNpc->mCorpsBidDay & CTemplateNpc::WDAY_THURSDAY ) != CTemplateNpc::WDAY_THURSDAY ) && tLocal->tm_wday == 4 )   ||
		( ( ( tpTplNpc->mCorpsBidDay & CTemplateNpc::WDAY_FRIDAY ) != CTemplateNpc::WDAY_FRIDAY ) && tLocal->tm_wday == 5 )		 ||
		( ( ( tpTplNpc->mCorpsBidDay & CTemplateNpc::WDAY_SATURDAY ) != CTemplateNpc::WDAY_SATURDAY ) && tLocal->tm_wday == 6 )	 ||
		( ( ( tpTplNpc->mCorpsBidDay & CTemplateNpc::WDAY_SUNDAY ) != CTemplateNpc::WDAY_SUNDAY ) && tLocal->tm_wday == 0 ) )
	{
		return -1;
	}

	if ( tLocal->tm_hour == tpTplNpc->mCorpsBidHour && tLocal->tm_min == tpTplNpc->mCorpsBidMinute )
	{
		int nTimerOtherInfo[ TIMER_OTHER_INFO ] = { MESSAGE_PROPERTY, 0, 0, 0};
		int tTimerID = ( tpEntityNpc->GetTimerReciver() )->SetTimer( tpEntityNpc->GetEntityID(), 
			TIMER_STARTCORPSBATTLE, SUBMITTIME * 60 * 1000, 1, nTimerOtherInfo );
		tpProNpc->SetBattleTimer( tTimerID );

		CMessage tMessage;
		CMessageCorpsBidTimerStopRequest tPara;
		tPara.set_npcid( tpTplNpc->mTempID );
		tPara.set_battleid( tpTplNpc->mBattleCity );
		tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_BIDTIMERSTOP );
		tMessage.set_msgpara( (long) &tPara );
		CSceneLogic::GetSingleton().Send2Gate( &tMessage );

		LOG_NOTICE( "corps", "[%s:%d] Send2Gate CorpsBidTimeout npc %d battleid %d" ,
			__LK_FILE__, __LINE__, tpTplNpc->mTempID, tpTplNpc->mBattleCity );
	}

	return 0;
}

void CCorpsModule::OnMessageMendShengHuoTaiRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
#ifdef _FB_
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}
	
	// 检查读秒时间
	CPropertyPlayer *tpPlayerProperty =	 ( CPropertyPlayer * )pPlayer->GetProperty();
	if ( tpPlayerProperty->CheckProgressTime( CTemplateProgressConfig::PROGRESS_FUCTYPE_CORPSBANNER, pPlayer->GetClientInfo()->mNetSpeed ) == false )
	{
		LOG_ERROR( "pro","wrong progress time when mend shenghuotai, roleid:%d", pPlayer->GetCharID() );
		return;
	}

	CMessageMendShengHuoTaiRequest* tpReq = ( CMessageMendShengHuoTaiRequest* ) pMessage->msgpara();

	CEntityCharacter* tpCharacter = dynamic_cast<CEntityCharacter*>( CSceneObjManager::GetSingletonPtr()->GetObject( tpReq->npcentityid() ) );
	if ( tpCharacter == NULL )
	{
		return ;
	}

	if ( tpCharacter->IsNpc() ==  false )
	{
		return ;
	}

	CPropertyNPC* tpProperty = ( CPropertyNPC* ) tpCharacter->GetProperty();
	// 满血就不用回了
	if( tpProperty->GetCurHP() == (unsigned int)( tpProperty->GetMaxHP().mInitValue + tpProperty->GetMaxHP().mDeltaValue) )
	{
		return ;
	}

	int tIncHP = CSceneCfgManager::GetSingleton().GetCorps_cfg()->mMendhpvalue;
	int tOldHP = tpProperty->GetCurHP();
	tpProperty->SetCurHP( std::min( (unsigned int)tpProperty->GetMaxHP(), tpProperty->GetCurHP( ) + tIncHP ) );
	int nRealChangeHP = tpProperty->GetCurHP() - tOldHP;

	tpCharacter->AddFunResult( tpCharacter->GetEntityID(), FUNC_INCHP, 
		RESULT_NORMALREFRESH, true, nRealChangeHP, tpProperty->GetCurHP(), tpProperty->GetMaxHP() );
	return ;
#endif
}

void CCorpsModule::OnGSSendBattleFlagOwnerNoitce( CMessage* pMessage )
{
#ifdef _FB_
	if ( pMessage == NULL )
	{
		return ;
	}
	CMessageCorpsBattleFlagResponse* tpResponse = ( CMessageCorpsBattleFlagResponse* ) pMessage->msgpara();

	CMessage tMsg;
	CMessageRepetionEventNotic tMsgFact;
	tMsg.mutable_msghead()->set_messageid( ID_S2C_REPETION_EVENT );
	tMsg.set_msgpara( (uint32)&tMsgFact );

	CRepetion* tpRepetion = CRepetionModule::GetSingleton().GetRepetion( tpResponse->battleid() );
	REPE_ASSERT( tpRepetion != NULL, return );

	CMapObject* tpMapObj = tpRepetion->GetBeginMapObject( );
	REPE_ASSERT( tpMapObj != NULL, return );

	tMsgFact.set_mapid( tpMapObj->GetMapID() );
	tMsgFact.set_eventid( REPETION_CORPS_FLAG );  
	tMsgFact.set_parame1( tpResponse->campid() );
	
	tMsgFact.set_parame3( tpResponse->corpsname() );

	if ( tpResponse->isall() == 0 )
	{
		CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpResponse->charid() );
		if ( tpPlayer == NULL )
		{
			return ;
		}
		tMsgFact.set_parame2( tpPlayer->GetCampID() );
		CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMsg );
	}
	else
	{
		tMsgFact.set_parame2( 0 );
		tpMapObj->SendMsg2Player( &tMsg );
	}
	return ;
#endif
}

void CCorpsModule::InsertNpcList(CMessageNpcListNotify *vpNotify)
{
	if ( vpNotify == NULL )
	{
		return ;
	}

	if ( vpNotify->type() != TYPE_CORPS )
	{
		return ;
	}

	for ( int i = 0; i < vpNotify->corpsnpc_size(); i ++ )
	{
		PBCorpsNpc* tpNpc = vpNotify->mutable_corpsnpc( i );
		CorpsNpcInfo tNpcInfo;
		tNpcInfo.mCorpsID = tpNpc->corpsid();
		strncpy( tNpcInfo.mCorpsName, tpNpc->corpsname().c_str(), sizeof( tNpcInfo.mCorpsName ) - 1 );

		CorpsNpcList::iterator it = mNpcList.find( tpNpc->npcid() );
		if ( it != mNpcList.end() )
		{
			mNpcList.erase( it );
		}

		if ( (bool)tpNpc->type() )
		{
			mNpcList.insert( CorpsNpcList::value_type( tpNpc->npcid(), tNpcInfo ) );
		}
		LOG_NOTICE( "corps", "npclist %d", tpNpc->npcid());
	}

	if ( !mNpcDataFlag )
		mNpcDataFlag = true;
	return; 
}

const char* CCorpsModule::GetCorpsNameByNpcID( int nNpcID )
{
	CorpsNpcList::iterator it = mNpcList.find( nNpcID );
	if ( it == mNpcList.end() )
	{
		return NULL;
	}

	CorpsNpcInfo* tpNpc = (CorpsNpcInfo*)&(it->second);
	if ( tpNpc == NULL )
	{
		return NULL;
	}
	return tpNpc->mCorpsName;
}

int	CCorpsModule::GetCorpsIDByNpcID( int nNpcID )
{
	CorpsNpcList::iterator it = mNpcList.find( nNpcID );
	if ( it == mNpcList.end() )
	{
		return 0;
	}

	CorpsNpcInfo* tpNpc = (CorpsNpcInfo*)&(it->second);
	if ( tpNpc == NULL )
	{
		return 0;
	}
	return tpNpc->mCorpsID;
}

void CCorpsModule::OnGSCorpsBattleResultNotice( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage ); 	
	return ;
}


// 改变军团成员贡献度的通知
void CCorpsModule::OnGateNotifyChangeCorpsContribute( CEntityPlayer* pPlayer, CMessage* pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageCorpsContributeChangedNotify *tpMsg = ( CMessageCorpsContributeChangedNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	if ( tpMsg->roleid() == pPlayer->GetCharID() )
	{	
		CMemberInfo *tpMemberInfo = CFamilyModule::GetSingleton().GetFamilyMemberInfo( pPlayer );
		if ( tpMemberInfo != NULL )
		{
			tpMemberInfo->mCorpsContribute = tpMsg->contribute();
		}
						
		//pPlayer->SetCorpsContribute( tpMsg->contribute() );
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 改变军团采集活跃度的通知
void CCorpsModule::OnGateNotifyChangeCorpsCollectActive( CEntityPlayer* pPlayer, CMessage* pMessage )
{	
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}


// ***********************************************************
//  Function:		OnGateResponseBuyCorpsItem
//  Description:	购买军团红包的消息回应
//  Output:			
//  Date:	11/03/2009
// *********************************************************
void CCorpsModule::OnGateResponseBuyCorpsItem( CMessage* pMessage )
{	
	LK_ASSERT( pMessage != NULL, return )		
	CMessageBuyCorpsItemResponse *tpMsg = ( CMessageBuyCorpsItemResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return ) 
	
	CEntityPlayer *tpPlayer = ( CEntityPlayer * )CSceneLogic::GetSingleton().GetEntity( tpMsg->entityid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "corps","[%s,%d][%d] can not find player by entityid:%d( roleid:%d ) ",__LK_FILE__,__LINE__,__FUNCTION__, tpMsg->entityid(), tpMsg->roleid() );
		return;
	}	
	
	if ( tpMsg->errcode() == SUCCESS )
	{
		CPropertyModule::GetSingleton().PlayerInsertItem( tpPlayer, tpMsg->itemid(), tpMsg->itemnum(), false );		
	}
	else
	{	
		CPropertyModule::GetSingleton().SendProErrorNotice( tpPlayer, tpMsg->errcode() );
	}
}

// ***********************************************************
//  Function:		OnGateMsgCorpsTitleChangedNotify
//  Description:	军团称号改变的消息通知
//  Output:			
//  Date:	11/03/2009
// *********************************************************
void CCorpsModule::OnGateMsgCorpsTitleChangedNotify( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}


// ***********************************************************
//  Function:		BuyCorpsItem
//  Description:	购买军团物品的接口
//  Output:			
//  Date:	11/03/2009							   
// *********************************************************
void CCorpsModule::BuyCorpsItem( CEntityPlayer *pPlayer, int nItemID, int nItemNum, int nNpcTmpID, int nCityMapID )
{	
	LK_ASSERT( pPlayer != NULL, return )	
	
	// 检查有没有军团
	if ( pPlayer->GetCorpsID() == 0 )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_CORPS_BUYFAMILYITE_HASONCORPS );
		return;
	}
	
	// 权限检查
	//if( !pPlayer->GetIsCorpsHeader() )
	//{
	//	CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_CORPS_HASNORIGHT );
	//	return;
	//}
	
	// 包裹检查
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	if ( !tpProperty->GetBaggage()->Space( nItemID, nItemNum ) )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_CORPS_BUYREDPAPER_HASNOSPACE );
		return;
	}		
	
	// 发往网关
	CMessage tMessageHead;
	CMessageBuyCorpsItemRequest tMsgBuyCorpsItem;
	
	tMessageHead.mutable_msghead()->set_messageid( ID_S2G_REQUEST_BUYCORPSITEM );
	tMessageHead.set_msgpara( (int)&tMsgBuyCorpsItem );
	
	tMsgBuyCorpsItem.set_corpsid( pPlayer->GetCorpsID() );
	tMsgBuyCorpsItem.set_entityid( pPlayer->GetEntityID() );
	tMsgBuyCorpsItem.set_itemid( nItemID );
	tMsgBuyCorpsItem.set_itemnum( nItemNum );
	tMsgBuyCorpsItem.set_roleid( pPlayer->GetCharID() );
	// tMsgBuyCorpsItem.set_totalcost( nItemPrice*nItemNum );
	tMsgBuyCorpsItem.set_npctmpid( nNpcTmpID );	
	tMsgBuyCorpsItem.set_citymapid( nCityMapID );
	CSceneLogic::GetSingleton().Send2Gate( &tMessageHead );
}

// 创建联军的请求
void CCorpsModule::OnMessageCreateUnion( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageCreateUnionRequest *tpMsg = ( CMessageCreateUnionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	if ( HasUnionService( pPlayer, tpMsg->npcentityid() ) == false )
	{
		return;
	}

	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();

	if ( tpProperty->BaggageHasItem( CSceneCfgManager::GetSingleton().GetProperty_cfg()->mCreateunion_id ) == false )
	{
		CMessage tMsgHead;
		CMessageCreateUnionResponse tMsgResponse;
		tMsgResponse.set_result( ERROR_UNION_MAKE_NOITEM );
		tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CREATEUNION );
		tMsgHead.set_msgpara( (int)&tMsgResponse );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}

	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

// 申请加入联军的请求消息
void CCorpsModule::OnMessageApplyUnion( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageApplyUnionRequest *tpMsg = ( CMessageApplyUnionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CMessage tMsgHead;
	CMessageApplyUnionResponse tMsgApplyUnion;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_APPLYUNION );
	tMsgHead.set_msgpara( (int)&tMsgApplyUnion );
	if ( pPlayer->GetCorpsID() == 0 )
	{  
		tMsgApplyUnion.set_errcode( ERROR_UNION_APPLY_NOCORPS );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}  

	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

// 回复军团申请加入联军的消息请求
void CCorpsModule::OnMessageReplyUnionMemberApply( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );		
	CMessageReplyUnionMemberApplyRequest *tpMsg = ( CMessageReplyUnionMemberApplyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	CMessage tMsgHead;
	CMessageReplyUnionMemberApplyResponse tMsgReplyApply;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REPLYUNIONMEMBERAPPLY );
	tMsgHead.set_msgpara( (int)&tMsgReplyApply );
	if ( pPlayer->GetCorpsID() == 0 )
	{								 
		tMsgReplyApply.set_errcode( ERROR_UNION_REPLYAPPLY_NOTINCORPS );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;				
	}

	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

// 邀请军团加入联军的消息请求
void CCorpsModule::OnMessageInviteUnionMember( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageInviteUnionMemberRequest *tpMsg = ( CMessageInviteUnionMemberRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CMessage tMsgHead;
	CMessageInviteUnionMemberResponse tMsgReplyInviteMember;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_INVITEUNIONMEMBER );
	tMsgHead.set_msgpara( (int)&tMsgReplyInviteMember );
	if ( pPlayer->GetCorpsID() == 0 )
	{
		tMsgReplyInviteMember.set_errcode( ERROR_UNION_INVITE_NOTINCORPS );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

// 答复联军邀请的消息请求
void CCorpsModule::OnMessageReplyUnionInvite( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageReplyUnionInviteRequest *tpMsg = ( CMessageReplyUnionInviteRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

// 军团离开联军的消息请求
void CCorpsModule::OnMessageCorpsLeaveUnion( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageCorpsLeaveUnionRequest *tpMsg = ( CMessageCorpsLeaveUnionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	// 密码安全检查
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}	   

	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

// 从联军踢出军团的消息请求
void CCorpsModule::OnMessageKickCorps( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )	
	CMessageKickCorpsRequest *tpMsg = ( CMessageKickCorpsRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	
	
	// 安全密码检查
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}			   

	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer )
}

// 创建联军的请求
void CCorpsModule::OnMessageMakeUnion( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageMakeUnionRequest *tpMsg = ( CMessageMakeUnionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	if ( tpProperty->BaggageHasItem( CSceneCfgManager::GetSingleton().GetProperty_cfg()->mCreateunion_id ) == false
		&& ( tpMsg->optype() == EM_CORPS_MAKE_INVITE || tpMsg->optype() == EM_CORPS_MAKE_INFO ) )
	{
		CMessage tMsgHead;
		CMessageMakeUnionResponse tMsgResponse;
		tMsgResponse.set_errcode( ERROR_UNION_MAKE_NOITEM );
		tMsgResponse.set_optype( tpMsg->optype() );
		tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_MAKEUNION );
		tMsgHead.set_msgpara( (int)&tMsgResponse );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}

	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

// 获得列表的请求
void CCorpsModule::OnMessageGetUnionInfo( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageGetUnionInfoRequest *tpMsg = ( CMessageGetUnionInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

// 解散的请求
void CCorpsModule::OnMessageDismissUnion( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageDismissUnionRequest *tpMsg = ( CMessageDismissUnionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	if ( HasUnionService( pPlayer, tpMsg->npcentityid() ) == false )
	{
		return;
	}

	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

// 获得联军申请列表的请求
void CCorpsModule::OnMessageUnionApplyList( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageUnionApplyListRequest *tpMsg = ( CMessageUnionApplyListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

// 获得联军列表的请求
void CCorpsModule::OnMessageAllUnionList( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageAllUnionListRequest *tpMsg = ( CMessageAllUnionListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

void CCorpsModule::OnMessageChangeUnionName( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageChangeUnionNameRequest *tpMsg = ( CMessageChangeUnionNameRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer );
}

// 获得列表的回应
void CCorpsModule::OnGSGetUnionInfoResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 创建联军的回应消息
void CCorpsModule::OnGSCreateUnionResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageCreateUnionResponse *tpMsg = ( CMessageCreateUnionResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	if ( tpMsg->result() == SUCCESS )
	{
		pPlayer->SetUnionID( tpMsg->unionid() );
		bool tBind = false;
		CPropertyModule::GetSingleton().PlayerRemoveItemByID( pPlayer, CSceneCfgManager::GetSingleton().GetProperty_cfg()->mCreateunion_id, 1, tBind );
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 创建联军的回应通知
void CCorpsModule::OnGSCreateUnionNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageCreateUnionNotify *tpMsg = ( CMessageCreateUnionNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	for ( int i = 0; i < tpMsg->otherid_size(); i++ )
	{
		if ( (unsigned int)pPlayer->GetCorpsID() == tpMsg->otherid(i) )
		{
			pPlayer->SetUnionID( tpMsg->unionid() );
			pPlayer->SetUnionName( tpMsg->unionname().c_str() );
			break;
		}
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 申请加入联军的回应消息
void CCorpsModule::OnGSApplyUnionResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 有军团申请加入联军的消息通知
void CCorpsModule::OnGSUnionApplyNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 回复军团申请加入联军的消息回应
void CCorpsModule::OnGSReplyUnionApplyResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 申请加入联军的结果通知
void CCorpsModule::OnGSApplyUnionResultNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageApplyUnionResultNotify *tpMsg = ( CMessageApplyUnionResultNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	if ( tpMsg->resultcode() == 1 )
	{
		pPlayer->SetUnionID( tpMsg->unionid() );
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 邀请军团加入联军的消息回应
void CCorpsModule::OnGSInviteUnionMemberResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	CMessageInviteUnionMemberResponse *tpMsg = ( CMessageInviteUnionMemberResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 邀请加入联军的消息通知
void CCorpsModule::OnGSUnionInviteNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 答复联军邀请的消息回复
void CCorpsModule::OnGSReplyUnionInviteResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageReplyUnionInviteResponse *tpMsg = ( CMessageReplyUnionInviteResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	if ( tpMsg->errcode() == SUCCESS && tpMsg->result() == 1 )
	{
		pPlayer->SetUnionID( tpMsg->unionid() );
		if ( tpMsg->has_unionname() )
		{
			pPlayer->SetUnionName( tpMsg->unionname().c_str() );
		}		
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 邀请军团加入联军的结果通知
void CCorpsModule::OnGSUnionInviteResultNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 有军团加入联军的消息通知
void CCorpsModule::OnGSCorpsJoinUnionNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );	
	CMessageCorpsJoinUnionNotify *tpMsg = ( CMessageCorpsJoinUnionNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 军团离开联军的消息回应
void CCorpsModule::OnGSCorpsLeaveUnionResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL && pMessage != NULL, return );
	CMessageCorpsLeaveUnionResponse *tpMsg = ( CMessageCorpsLeaveUnionResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	if( tpMsg->errcode() == 0 )
	{
		pPlayer->SetUnionID( 0 );
		pPlayer->SetUnionName( "" );
	}		
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 军团离开联军的消息通知
void CCorpsModule::OnGSCorpsLeaveUnionNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return ); 
	CMessageCorpsLeaveUnionNotify *tpMsg = ( CMessageCorpsLeaveUnionNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL , return );
	if ( (int)tpMsg->corpsid() == pPlayer->GetCorpsID() )
	{
		pPlayer->SetUnionID( 0 );
		pPlayer->SetUnionName( "" );
	}
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 从联军踢出军团的消息回应
void CCorpsModule::OnGSKickCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 军团从联军里面被踢掉的通知	
void CCorpsModule::OnGSCorpsKickedNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageKickCorpsNotify *tpMsg = ( CMessageKickCorpsNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	if ( (int)tpMsg->corpsid() == pPlayer->GetCorpsID() )
	{
		pPlayer->SetUnionID( 0 );
		pPlayer->SetUnionName( "" );
	}	
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 创建联军的回应消息
void CCorpsModule::OnGSMakeUnionResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 解散联军的回应消息
void CCorpsModule::OnGSDismissUnionResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageDismissUnionResponse *tpMsg = ( CMessageDismissUnionResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 获得联军申请列表的消息回应
void CCorpsModule::OnGSUnionApplyListResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageUnionApplyListResponse *tpMsg = ( CMessageUnionApplyListResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 解散联军的回应通知
void CCorpsModule::OnGSDismissUnionNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageDismissUnionNotify *tpMsg = ( CMessageDismissUnionNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	pPlayer->SetUnionID( 0 );
	pPlayer->SetUnionName( "" );

	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

// 获得联军列表的消息回应
void CCorpsModule::OnGSAllUnionListResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageAllUnionListResponse *tpMsg = ( CMessageAllUnionListResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

void CCorpsModule::OnGSChangeUnionNameResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageChangeUnionNameResponse *tpMsg = ( CMessageChangeUnionNameResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

void CCorpsModule::OnGSChangeUnionNameNotify( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageChangeUnionNameNotify *tpMsg = ( CMessageChangeUnionNameNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, pMessage );
}

void CCorpsModule:: OnGateMessageSummonCorpsBossResponse( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageSummonCorpsBossResponse *tpMsg = (CMessageSummonCorpsBossResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	CSceneLogic::GetSingleton().Send2Player(pPlayer, pMessage );

}



// ***********************************************************
//  Function:	  OnMessageSummonCorpsBossRequest
//  Description:  召唤军团boss的信息请求
//  Output:			
//  Date:	2011/09/19
// *********************************************************
void CCorpsModule::OnMessageSummonCorpsBossRequest( CEntityPlayer* pPlayer, CMessage* pMessage )
{	
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	CMessageSummonCorpsBossRequest *tpMsg = ( CMessageSummonCorpsBossRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	if ( pPlayer->GetCorpsID() == 0 )
	{
		return;
	}
	CPropertyPlayer* pPropertyPlayer = ( CPropertyPlayer* )pPlayer->GetProperty();
	CPlayerRepetion* pPlayerRepetion = ( CPlayerRepetion* )pPropertyPlayer->GetRepetion();
	if ( pPlayerRepetion->GetInRepetionIndex() == _REPE_CORPSBOSS_INDEX_ )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_WORLD_SUMMONINREP );
		return;
	}
		
	SendCorpsMsgToGate( pMessage, tpMsg, pPlayer ) 
}