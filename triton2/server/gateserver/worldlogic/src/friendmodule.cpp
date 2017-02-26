#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tinyxml.h>
#include "gatelogic.h"
#include "friendmodule.h"
#include "gateobj_manager.h"
#include "entity.h"

#include "gateteam.h"
#include "gateteammanager.h"

#include "nameservice.h"
#include "friendtype.h"
#include "servermessage_in.h"
#include "core_service.h"
#include "template.h"
#include "friendmessage_pb.hxx.pb.h"
#include "db_msg_pb.hxx.pb.h"

template<> CFriendModule* CSingleton< CFriendModule >::spSingleton = NULL;

CFriendModule::CFriendModule()
{
	m_LatestContactList.initailize();
}

void CFriendModule::OnMessage(int vServerID, CMessage* pMessage)
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageHead* tpHead = pMessage->mutable_msghead();
	LK_ASSERT(tpHead != NULL, return);
	unsigned short nMessageID = (unsigned short) tpHead->messageid();

	switch( nMessageID )
	{
		case ID_S2G_PLAYER_ONLINE_FRIEND_REQUEST:
			{
				OnMessagePlayerOnlineFriendRequest(vServerID, pMessage);
				break;
			}
		case ID_S2G_PLAYER_CHANGE_STATUS_NOTICE:
			{
				OnMessagePlayerChangeImStatusNotice(vServerID, pMessage);
				break;
			}
		case ID_S2G_ADDFRIEND_REQUEST:
			{
				OnMessagePlayerAddFriend(vServerID, pMessage);
				break;
			}
		case ID_S2G_GETCHARID_REQUEST:
			{
				OnMessagePlayerGetCharID(vServerID, pMessage);
				break;
			}

		case ID_C2S_REQUEST_CHANGEMASTER:
			{
				OnMessageChangeMasterRequest(vServerID, pMessage);
				break;
			}
		case ID_C2S_REQUEST_GETSLAVE:
			{
				OnMessageGetSlaveRequest(vServerID, pMessage);
				break;
			}
		case ID_S2C_NOTIFY_SLAVEONOFF:
			{
				OnMessageSlaveOnOffNotify( vServerID, pMessage );
				break;
			}
		case ID_C2S_REQUEST_QUERYPLAYER:
			{
				OnMessageQueryPlayerRequest(vServerID, pMessage);
				break;
			}
		case ID_C2S_REQUEST_REMOVESLAVE:
			{
				OnMessageRemoveSlaveRequest(vServerID, pMessage);
				break;
			}
		case ID_C2S_REQUEST_REMOVEMASTER:
			{
				OnMessageRemoveMasterRequest(vServerID, pMessage);
				break;
			}
		case ID_S2G_ADDMASTERPOINT_REQUEST:
			{
				OnMessageAddMasterPointRequest(vServerID, pMessage);
				break;
			}
		case ID_C2S_REQUEST_ANSWERCHANGE:
			{
				OnMessageAnswerChangeRequest(vServerID, pMessage);
				break;
			}
		case ID_S2G_ACQUIREMASTER_NOTIFY:
			{
				OnMessageAcquireMasterNotify( vServerID, pMessage );
				break;
			}
		case ID_S2G_NOTIFY_UPLOADCONTACTERLIST:
			{
				OnMessageUploadContacterListNotify(vServerID, pMessage);
			}
			break;
		case ID_S2G_NOTIFY_UPDATELASTCONTACTER:
			{
				OnMessageUpdateLastContacterNotify(vServerID, pMessage);
			}
			break;
		case ID_S2C_NOTIFY_SWORNINFO:
			{
				OnMessageSwornInfo( vServerID, pMessage );
				break;
			}
		case ID_S2C_NOTIFY_LEAVESWORNTEAM:
			{
				OnMessageLeavelSwornTeamNotify( pMessage );
				break;
			}
		case ID_S2S_NOTIFY_MODIFYINTIMACY:
			{
				OnMessageModifyIntimacyNotify( pMessage );
				break;
			}
		case ID_S2C_NOTIFY_KICKSWORNMEMBER:
			{
				OnMessageKickSwornMemberNotify( pMessage );
				break;
			}
		case ID_S2C_NOTIFY_DISBANDSWORNTEAM:
			{
				OnMessageDisbandSwornTeamNotify( pMessage );
				break;
			}
		case ID_S2S_NOTIFY_MODIFYLASTTIMEALLONLINE:
			{
				OnMessageModifyLastTimeAllOnline( pMessage );
				break;
			}
		case ID_S2C_NOTIFY_CHANGESWORNNAME:
			{
				OnMessageChangeSwornName( pMessage );
				break;
			}
		default:
			break;
	}
}


void CFriendModule::OnMessagePlayerOnlineFriendRequest(int nServerID,  CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	CMessagePlayerOnLineFriendRequest* pMessage = (CMessagePlayerOnLineFriendRequest*) tpMessage->msgpara();
	LK_ASSERT( pMessage != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	CMessagePlayerOnLineFriendResponse tMessage;

	unsigned int size = pMessage->charids_size() ;

	//如果传过来的数组大小超过30个，截断，避免程序处理一个消息时间过长
	if (size > MAX_ONCE_DISPOS) size = MAX_ONCE_DISPOS;
	if (size == 0)
		return;
	CRoleGlobalInfo* pRoleInfo = NULL;

	for(unsigned int i=0; i<size; ++i)
	{
		PBSinglePlayerInfo* pData= tMessage.add_friends();
		LK_ASSERT(pData, continue);

		pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( pMessage->charids(i));
		if (pRoleInfo == NULL)
		{
			// (可能删角色)LOG_ERROR("default", "[%s:%d]can't find roleinfo, charid=%d", __FUNCTION__, __LINE__, pMessage->charids(i) );
			pData->set_noexist(1);
			pData->set_charid( pMessage->charids(i) );
			continue;
		}

		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMessage->charids(i));
		pData->set_charid( pMessage->charids(i) );
		pData->set_name(pRoleInfo->RoleName());
		pData->set_vipflag( pRoleInfo->GetVipFlag() );
		if ( pMessage->flag() == FLAG_GETFRIEND_ALL	|| 
			 pMessage->flag() == FLAG_GETENEMY_ALL || 
			 pMessage->flag() == FLAG_GETBLACK ||
			 pMessage->flag() == FLAG_GETCONTACTER)
		{
			pData->set_sex(  pRoleInfo->GenderID() );
			pData->set_face( pRoleInfo->FaceType() );
			pData->set_nation( pRoleInfo->CountryID() );			
		}

		if (tpPlayer != NULL) // 在线
		{
			pData->set_status(tpPlayer->mIMStatus);
		}
		else // 不在线
		{
			pData->set_status(0);
		}
	}

	tMessage.set_requester(pMessage->requester());
	tMessage.set_flag( pMessage->flag() );

	CMessage tMessage2;
	pbmsg_sethead(&tMessage2, ID_G2S_PLAYER_ONLINE_FIREND_RESPONSE, 0, 0, 0, 0, 0, 0);
	tMessage2.set_msgpara((uint32_t) &tMessage);

	CGateServer::GetSingleton().Send2Scene(nServerID, &tMessage2);
}


void CFriendModule::OnMessagePlayerChangeImStatusNotice(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->has_msgpara(), return);
	CMessagePlayerChangeStatusNotice* pMessage = (CMessagePlayerChangeStatusNotice*) tpMessage->msgpara();
	LK_ASSERT(pMessage != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(pMessage->charid());
	if (tpPlayer)
	{
		tpPlayer->mIMStatus = pMessage->status();
		//将我的状态通知给最近联系人
		SendMyStatusToContacterList(tpPlayer, (EPlayerShow)tpPlayer->mIMStatus);
	}
}

// 玩家加好友处理
void CFriendModule::OnMessagePlayerAddFriend(int nServerID, CMessage* pMessage)
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageAddFriendRequest* pMsg = (CMessageAddFriendRequest*) pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	CGatePlayer* tpSrcPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMsg->srcid() );
	LK_ASSERT( tpSrcPlayer, return );
	CRoleGlobalInfo* pSrcRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( pMsg->srcid() );
	if (pSrcRoleInfo == NULL)
	{
		// (可能删角色)LOG_ERROR("default", "[%s:%d]can't find roleinfo, charid=%d", __FUNCTION__, __LINE__, pMsg->srcid() );
	}

	CGatePlayer*     tpPlayer  = CPlayerManager::GetSingleton().ScenePlayer( pMsg->desid() );
	if (tpPlayer != NULL && pSrcRoleInfo != NULL) // 在线
	{
		CMessageAddFriendResponse ret;
		ret.set_sendback(1);
		ret.set_retcode(RET_SUCCESS);
		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2S_ADDFRIEND_RESPONSE, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &ret);
		CGateServer::GetSingleton().Send2Scene(nServerID, &tMessage); // 发回去

		ret.set_sendback(0);
		ret.set_descharid( tpPlayer->mCharID );
		ret.set_desname( tpPlayer->mCharName );

		ret.set_srccharid( tpSrcPlayer->mCharID );
		ret.set_srchead( pSrcRoleInfo->FaceType() );
		ret.set_srclevel( pMsg->srclevel() );
		ret.set_srcmetier( tpSrcPlayer->mMetierID );
		ret.set_srcname( pSrcRoleInfo->RoleName() );
		ret.set_srcsex( pSrcRoleInfo->GenderID() );

		tMessage.set_msgpara((uint32_t) &ret);
		CGateServer::GetSingleton().Send2Scene(tpPlayer, &tMessage);
	}
	else // 不在线
	{
		CMessageAddFriendResponse ret;
		ret.set_sendback(1);
		ret.set_retcode(RET_OFFLINE);
		ret.set_srccharid(pMsg->srcid());
		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2S_ADDFRIEND_RESPONSE, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &ret);
		CGateServer::GetSingleton().Send2Scene(nServerID, &tMessage); // 发回去
	}

}

void CFriendModule::OnMessagePlayerGetCharID(int nServerID, CMessage* pMessage)
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageGetCharIDRequest* pMsg = (CMessageGetCharIDRequest*) pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	CMessageGetCharIDResponse ret;
	ret.set_typeid_( pMsg->typeid_() );
	ret.set_srcentityid( pMsg->srcentityid() );
	CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleName( pMsg->name().c_str() );
	if (pRoleInfo != NULL)
	{
		ret.set_charid( pRoleInfo->RoleID() );
		if ( pMsg->typeid_() == 1 ) // 如果是取黑名单
		{
			ret.set_name( pMsg->name().c_str() );
			ret.set_head( pRoleInfo->FaceType() );
			ret.set_sex( pRoleInfo->GenderID() );
		}
	}
	else
	{
		ret.set_charid( 0 );
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2S_GETCHARID_RESPONSE, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &ret);
	CGateServer::GetSingleton().Send2Scene(nServerID, &tMessage); // 发回去
}

void CFriendModule::OnMessageChangeMasterRequest( int nServerID, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageChangeMasterRequest* pMsg = (CMessageChangeMasterRequest*) pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
	if ( tpTable == NULL )
	{
		return;
	}
	
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMsg->mastername().c_str() );
	if (tpPlayer != NULL)
	{
		for ( int i = 0; i < MAX_MASTER_NUM && i < pMsg->masterid_size(); i++ )
		{
			if ( tpPlayer->CharID( ) == (int)pMsg->masterid(i) )
			{
				SendChangeMasterResponse( ERROR_MASTER_NOTNULL, 0 ,pMsg->charid() );
				return;
			}
		}
		if ( tpPlayer->Level() < tpTable->mMinLimit )
		{
			SendChangeMasterResponse( ERROR_MASTER_LOWLEVEL, 0, pMsg->charid() );
			return;
		}
		CGatePlayer *tpSlave = CPlayerManager::GetSingleton().ScenePlayer( pMsg->charid() );
		if ( tpSlave == NULL || tpSlave->GetNationalityID() != tpPlayer->GetNationalityID() )
		{
			SendChangeMasterResponse( ERROR_MASTER_NOTSAMEWORLD, 0, pMsg->charid() );
			return;
		}
		//SendChangeMasterResponse( SUCCESS, tpPlayer->CharID(), pMsg->charid() );
		SendAnswerChangeNotify( pMsg->charid(), tpPlayer->CharID() );
		return;

	}
	else
	{
		SendChangeMasterResponse( ERROR_MASTER_NOTONLINE, 0 ,pMsg->charid() );
		return;
	}
}

void CFriendModule::OnMessageGetSlaveRequest( int nServerID, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageGetSlaveRequest* pMsg = (CMessageGetSlaveRequest*) pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	CMessageGetSlaveResponse tResponse;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETSLAVE );
	tMessage.set_msgpara((uint32_t) &tResponse);

	for ( int i = 0; i < pMsg->slaveid_size(); i++ )
	{
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMsg->slaveid(i) );
		if ( tpPlayer != NULL )
		{
			SlaveInfo *tpInfo = tResponse.add_slave();
			tpInfo->set_slaveid( tpPlayer->CharID() );
			tpInfo->set_slavename( tpPlayer->CharName() );
			tpInfo->set_level( tpPlayer->Level() );
			tpInfo->set_metier( tpPlayer->MetierID() );
			tpInfo->set_face( tpPlayer->FaceType() );
			tpInfo->set_sex( tpPlayer->SexID() );
		}
	}	
	for ( int i = 0; i < pMsg->masterid_size(); i++ )
	{
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMsg->masterid(i) );
		if ( tpPlayer != NULL )
		{
			SlaveInfo *tpInfo = tResponse.add_master();
			tpInfo->set_slaveid( tpPlayer->CharID() );
			tpInfo->set_slavename( tpPlayer->CharName() );
			tpInfo->set_level( tpPlayer->Level() );
			tpInfo->set_metier( tpPlayer->MetierID() );
			tpInfo->set_face( tpPlayer->FaceType() );
			tpInfo->set_sex( tpPlayer->SexID() );
		}
	}
	tResponse.set_result( SUCCESS );
	tResponse.set_charid( pMsg->charid() );
	CGatePlayer *tpSrcPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMsg->charid() );
	if ( tpSrcPlayer != NULL )
	{
		CGateServer::GetSingleton().Send2Scene( tpSrcPlayer, &tMessage );
	}
}

void CFriendModule::OnMessageSlaveOnOffNotify( int nServerID, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageSlaveOnOffNotify* pMsg = (CMessageSlaveOnOffNotify*) pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	/*for ( int i = 0; i < pMsg->descharid_size(); i++ )
	{
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMsg->descharid(i) );
		if (tpPlayer != NULL)
		{
			CGateServer::GetSingleton().Send2Scene( tpPlayer, pMessage );
		}
	}*/

	CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, pMessage );
}

void CFriendModule::SendChangeMasterResponse( int vResult, int vMasterID, int vCharID )
{
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );
	if ( tpPlayer == NULL )
	{
		return;
	}
	
	CMessageChangeMasterResponse tResponse;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CHANGEMASTER );
	tMessage.set_msgpara((uint32_t) &tResponse);

	CGatePlayer *tpMasterPlayer = CPlayerManager::GetSingleton().ScenePlayer( vMasterID );
	if ( tpMasterPlayer != NULL )
	{
		tResponse.set_masterid( vMasterID );
		tResponse.set_mastername( tpMasterPlayer->CharName() );
		tResponse.set_level( tpMasterPlayer->Level() );
		tResponse.set_metier( tpMasterPlayer->MetierID() );
	}
	tResponse.set_result( vResult );
	tResponse.set_charid( vCharID );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CFriendModule::SendChangeMasterNotify( int vSlaveID, int vCharID )
{
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );
	if ( tpPlayer == NULL )
	{
		return;
	}

	CMessageChangeMasterNotify tResponse;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGEMASTER );
	tMessage.set_msgpara((uint32_t) &tResponse);

	CGatePlayer *tpSlavePlayer = CPlayerManager::GetSingleton().ScenePlayer( vSlaveID );
	if ( tpSlavePlayer == NULL )
	{
		return;	
	}
	tResponse.mutable_slave()->set_slaveid( vSlaveID );
	tResponse.mutable_slave()->set_slavename( tpSlavePlayer->CharName() );
	tResponse.mutable_slave()->set_level( tpSlavePlayer->Level() );
	tResponse.set_charid( vCharID );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CFriendModule::SendSlaveLevelUpNotify( int vSlaveID, int vCharID )
{
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );
	if ( tpPlayer == NULL )
	{
		return;
	}

	CMessageSlaveLevelUpNotify tResponse;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_SLAVELEVELUP );
	tMessage.set_msgpara((uint32_t) &tResponse);

	CGatePlayer *tpSlavePlayer = CPlayerManager::GetSingleton().ScenePlayer( vSlaveID );
	if ( tpSlavePlayer == NULL )
	{
		return;	
	}
	tResponse.mutable_slave()->set_slaveid( vSlaveID );
	tResponse.mutable_slave()->set_slavename( tpSlavePlayer->CharName() );
	tResponse.mutable_slave()->set_level( tpSlavePlayer->Level() );
	tResponse.set_charid( vCharID );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CFriendModule::OnMessageAnswerChangeRequest( int nServerID, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageAnswerChangeRequest* pMsg = (CMessageAnswerChangeRequest*) pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	if ( pMsg->result() == 0 )
	{
		SendChangeMasterResponse( ERROR_MASTER_REFUSED, pMsg->charid(), pMsg->slaveid() );
	}
	else
	{
		SendChangeMasterResponse( pMsg->error(), pMsg->charid(), pMsg->slaveid() );
	}
}

void CFriendModule::OnMessageQueryPlayerRequest( int nServerID, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageQueryPlayerRequest* pMsg = (CMessageQueryPlayerRequest*) pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	CMessageQueryPlayerResponse tResponse;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYPLAYER );
	tMessage.set_msgpara((uint32_t) &tResponse);

	for ( int i = 0; i < pMsg->queryid_size(); i++ )
	{
		CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( pMsg->queryid(i) );		
		if ( pRoleInfo != NULL )
		{
			SlaveInfo *pInfo = tResponse.add_info();
			pInfo->set_slaveid( pRoleInfo->RoleID() );
			pInfo->set_slavename( pRoleInfo->RoleName() );
			pInfo->set_level( pRoleInfo->Level() );
			pInfo->set_metier( pRoleInfo->Metier() );
			pInfo->set_face( pRoleInfo->FaceType() );
			pInfo->set_sex( pRoleInfo->GenderID() );
			CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMsg->queryid(i) );
			if ( tpPlayer != NULL )
				pInfo->set_online( 1 );
		}
	}	
	//tResponse.set_result( SUCCESS );
	tResponse.set_charid( pMsg->charid() );
	CGatePlayer *tpSrcPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMsg->charid() );
	if ( tpSrcPlayer != NULL )
	{
		CGateServer::GetSingleton().Send2Scene( tpSrcPlayer, &tMessage );
	}
}

void CFriendModule::SendRemoveSlaveNotify( int vCharID, int vMasterID )
{
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );
	if ( tpPlayer == NULL )
	{
		return;
	}

	CMessageRemoveSlaveNotify tNotify;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_REMOVESLAVE );
	tMessage.set_msgpara((uint32_t) &tNotify);

	tNotify.set_charid( vCharID );
	tNotify.set_masterid( vMasterID );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CFriendModule::SendRemoveSlaveResponse( int vResult, int vSlaveID, int vCharID, int vParam )
{
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );
	if ( tpPlayer == NULL )
	{
		return;
	}

	CMessageRemoveSlaveResponse tResponse;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REMOVESLAVE );
	tMessage.set_msgpara((uint32_t) &tResponse);

	tResponse.set_charid( vCharID );
	tResponse.set_slaveid( vSlaveID );
	tResponse.set_result( vResult );
	tResponse.set_param( vParam );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CFriendModule::OnMessageRemoveSlaveRequest( int nServerID, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageRemoveSlaveRequest* pMsg = (CMessageRemoveSlaveRequest*) pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);	

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMsg->slaveid() );
	if ( tpPlayer != NULL )
	{
		SendRemoveSlaveNotify( pMsg->slaveid(), pMsg->charid() );
	}
	else
	{
		SendUpdateMatrixInfoRequest( pMsg->slaveid(), RemoveSlave, pMsg->charid() );	
	}

	CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
	if ( tpTable == NULL )
	{
		return;
	}
	int tParam = 0;
	CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton( ).GetRoleInfoByRoleID( pMsg->slaveid() );
	if ( pRoleInfo == NULL )
	{
		SendRemoveSlaveResponse( SUCCESS, pMsg->slaveid(), pMsg->charid(), 0 );
		return;
	}
	if ( pRoleInfo->Level() < tpTable->mFormalLevel )
	{
		tParam = BeginnerNum;
	}
	else if ( pRoleInfo->Level() >= tpTable->mFormalLevel && pRoleInfo->Level() < tpTable->mEducatedLevel )
	{
		tParam = FormalNum;
	}
	else if ( pRoleInfo->Level() > tpTable->mEducatedLevel )
	{
		tParam = EducatedNum;
	}
	SendRemoveSlaveResponse( SUCCESS, pMsg->slaveid(), pMsg->charid(), tParam );
}

void CFriendModule::SendUpdateMatrixInfoRequest( int vCharID, int vSubType, int vValue, int vParam )
{
	CMessageUpdatePlayerInfoRequest tRequest;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2D_UPDATEPLAYERINFO_REQUEST );
	tMessage.set_msgpara((uint32_t) &tRequest);

	tRequest.set_roleid( vCharID );
	tRequest.set_updatetype( MatrixInfo );
	tRequest.set_subtype( vSubType );
	tRequest.set_param1( vParam );
	tRequest.set_value( vValue );

	CGateServer::GetSingleton().Send2Data( 0, &tMessage );
}

void CFriendModule::SendRemoveMasterNotify( int vCharID, int vSlaveID, int vParam )
{
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );
	if ( tpPlayer == NULL )
	{
		return;
	}

	CMessageRemoveMasterNotify tNotify;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_REMOVEMASTER );
	tMessage.set_msgpara((uint32_t) &tNotify);

	tNotify.set_charid( vCharID );
	tNotify.set_slaveid( vSlaveID );
	tNotify.set_param( vParam );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CFriendModule::SendRemoveMasterResponse( int vResult, int vMasterID, int vCharID )
{
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );
	if ( tpPlayer == NULL )
	{
		return;
	}

	CMessageRemoveMasterResponse tResponse;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REMOVEMASTER );
	tMessage.set_msgpara((uint32_t) &tResponse);

	tResponse.set_charid( vCharID );
	tResponse.set_masterid( vMasterID );
	tResponse.set_result( vResult );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CFriendModule::OnMessageRemoveMasterRequest( int nServerID, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageRemoveMasterRequest* pMsg = (CMessageRemoveMasterRequest*) pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);	

	CGatePlayer *tpSrcPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMsg->charid() );
	if ( tpSrcPlayer == NULL )
	{
		return;
	}
	CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
	if ( tpTable == NULL )
	{
		return;
	}
	int tParam = 0;
	if ( tpSrcPlayer->Level() < tpTable->mFormalLevel )
	{
		tParam = BeginnerNum;
	}
	else if ( tpSrcPlayer->Level() >= tpTable->mFormalLevel && tpSrcPlayer->Level() < tpTable->mEducatedLevel )
	{
		tParam = FormalNum;
	}
	else if ( tpSrcPlayer->Level() > tpTable->mEducatedLevel )
	{
		tParam = EducatedNum;
	}


	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMsg->masterid() );
	if ( tpPlayer != NULL )
	{
		SendRemoveMasterNotify( pMsg->masterid(), pMsg->charid(), tParam );
	}
	else
	{
		SendUpdateMatrixInfoRequest( pMsg->masterid(), RemoveMaster, pMsg->charid(), tParam );	
	}

	SendRemoveMasterResponse( SUCCESS, pMsg->masterid(), pMsg->charid() );
}

void CFriendModule::OnMessageAddMasterPointRequest( int nServerID, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageAddMasterPointRequest* pMsg = (CMessageAddMasterPointRequest*) pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);	

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pMsg->masterid() );
	if ( tpPlayer != NULL )
	{
		SendAddMasterPointNotify( pMsg->masterid(), pMsg->point() );
	}
	else
	{
		SendUpdateMatrixInfoRequest( pMsg->masterid(), AddMasterPoint, pMsg->point() );	
	}
}

void CFriendModule::SendAddMasterPointNotify( int vMasterID, int vPoint )
{
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vMasterID );
	if ( tpPlayer == NULL )
	{
		return;
	}

	CMessageAddMasterPointNotify tNotify;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_G2S_ADDMASTERPOINT_NOTIFY );
	tMessage.set_msgpara((uint32_t) &tNotify);

	tNotify.set_charid( vMasterID );
	tNotify.set_point( vPoint );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CFriendModule::SendAddNumNotify( int vSlaveID, int vMasterID, int vParam )
{
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vMasterID );
	if ( tpPlayer != NULL )
	{
		SendSlaveLevelUpNotify( vSlaveID, vMasterID );
	}
	else
	{
		SendUpdateMatrixInfoRequest( vMasterID, vParam, vSlaveID );	
	}
}

void CFriendModule::SendAnswerChangeNotify( int vSlaveID, int vCharID )
{
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );
	if ( tpPlayer == NULL )
	{
		return;
	}

	CMessageAnswerChangeNotify tResponse;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_ANSWERCHANGE );
	tMessage.set_msgpara((uint32_t) &tResponse);

	CGatePlayer *tpSlavePlayer = CPlayerManager::GetSingleton().ScenePlayer( vSlaveID );
	if ( tpSlavePlayer == NULL )
	{
		return;	
	}
	tResponse.mutable_slave()->set_slaveid( vSlaveID );
	tResponse.mutable_slave()->set_slavename( tpSlavePlayer->CharName() );
	tResponse.mutable_slave()->set_level( tpSlavePlayer->Level() );
	tResponse.mutable_slave()->set_metier( tpSlavePlayer->MetierID() );
	tResponse.set_charid( vCharID );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CFriendModule::OnMessageAcquireMasterNotify( int nServerID, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageAcquireMasterNotify* pMsg = (CMessageAcquireMasterNotify*) pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	SendChangeMasterNotify( pMsg->slaveid(), pMsg->masterid() );
}

int CFriendModule::AddContacter(int nCharID, int nContacterID)
{
	PlayerLastestContactList::iterator it = m_LatestContactList.find(nCharID);
	if (it == m_LatestContactList.end())
	{
		LatestContactType LatestContact;
		LatestContact.initailize();
		LatestContact.push_back(nContacterID);
		m_LatestContactList.insert(PlayerLastestContactList::value_type(nCharID, LatestContact));
	}
	else
	{
		int nCount = it->second.size();
		for (int i = 0; i < nCount; i++)
		{
			if (it->second[i] == nContacterID)
			{
				it->second.erase(it->second.begin() + i);
				break;
			}
			
		}
		int nSize = it->second.size();
		if (nSize >= LATEST_CONTACT_MAX)
		{
			it->second.erase(it->second.begin());
		}
		it->second.push_back(nContacterID);
	}

	return 0;
}

int CFriendModule::ClearContacterOfOnePlayer(int nCharID)
{
	PlayerLastestContactList::iterator it = m_LatestContactList.find(nCharID);
	if (it == m_LatestContactList.end())
	{
		return -1;
	}

	m_LatestContactList.erase(it);
	
	return 0;
}

void CFriendModule::OnMessageUploadContacterListNotify( int nServerID, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageUploadContacterList* pMsg = (CMessageUploadContacterList*)pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	int nPlayerID = pMsg->playerid();
	ClearContacterOfOnePlayer(nPlayerID);
	int nCount = pMsg->contacterid_size();
	int nContacterID= 0;
	for (int i = 0; i < nCount; i++)
	{
		nContacterID = pMsg->contacterid(i);
		AddContacter(nPlayerID, nContacterID);
	}	
}

void CFriendModule::OnMessageUpdateLastContacterNotify( int nServerID, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->has_msgpara(), return);
	CMessageUpdateLastContacterNotice* pMsg = (CMessageUpdateLastContacterNotice*)pMessage->msgpara();
	LK_ASSERT(pMsg != NULL && nServerID > 0 && nServerID <= SCENE_AT_GATE_CAP, return);

	int nSenderID = pMsg->senderid();
	int nReceiverID= pMsg->receiverid();
	
	AddContacter(nSenderID, nReceiverID);
	AddContacter(nReceiverID, nSenderID);
}

int CFriendModule::SendMyStatusToContacterList(CGatePlayer* pPlayer, EPlayerShow eStatus)
{
	if (pPlayer == NULL)
	{
		return -1;
	}
	
	int nCharID = pPlayer->CharID();
	int nContacterID = 0;
	PlayerLastestContactList::iterator it = m_LatestContactList.begin();
	for (; it != m_LatestContactList.end(); it++)
	{
		int nCount = it->second.size();
		bool bInContacterList = false;
		for (int i = 0; i < nCount; i++)
		{
			if (it->second[i] == nCharID)
			{
				bInContacterList = true;
				nContacterID = it->first;
				break;
			}
		}
		if (bInContacterList)
		{
			SendMyStatusToContacter(nCharID, eStatus, nContacterID);
		}
	}

	return 0;
}

int CFriendModule::SendMyStatusToContacter(int nCharID, EPlayerShow eStatus, int nContacterID)
{
	CGatePlayer* pPlayer = CPlayerManager::GetSingleton().ScenePlayer(nContacterID);
	if (pPlayer == NULL)
	{
		return -1;
	}

	CMessage Msg;
	CMessageContacterStatusNotifyG2S MsgPara;
	MsgPara.set_playerid(nCharID);
	MsgPara.set_playerstatus((int)eStatus);
	MsgPara.set_contacterid(nContacterID);
	Msg.mutable_msghead()->set_messageid(ID_G2S_NOTIFY_CONTACTERSTATUS);
	Msg.set_msgpara((int)&MsgPara);
	CGateServer::GetSingleton().Send2Scene(pPlayer, &Msg);

	return 0;
}
void CFriendModule::OnMessageSwornInfo( int nServerID, CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageSwornInfoNotify *tpMsg = ( CMessageSwornInfoNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	
	for ( int i = 0; i < tpMsg->swornmember_size(); ++i )
	{
		CRoleGlobalInfo *tpRoleInfo =  CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpMsg->mutable_swornmember(i)->roleid() );
		if ( tpRoleInfo != NULL )
		{
			tpMsg->mutable_swornmember(i)->set_rolename( tpRoleInfo->RoleName() );
			tpMsg->mutable_swornmember(i)->set_rolelevel( tpRoleInfo->Level() );
			tpMsg->mutable_swornmember(i)->set_metierid( tpRoleInfo->Metier() );
			tpMsg->mutable_swornmember(i)->set_faceid( tpRoleInfo->FaceType() );
			tpMsg->mutable_swornmember(i)->set_genderid( tpRoleInfo->GenderID() );
		}	
	}	
	CGateServer::GetSingleton().Send2Scene( nServerID, pMsg );
}

void CFriendModule::OnMessageLeavelSwornTeamNotify( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageLeaveSwornTeamNotify *tpMsg = ( CMessageLeaveSwornTeamNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer == NULL )
	{
		return;
	}
	CGateServer::GetSingleton().Send2Scene( tpPlayer, pMsg );
}

void CFriendModule::OnMessageModifyIntimacyNotify( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageModifyIntimacyNogify *tpMsg = ( CMessageModifyIntimacyNogify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer != NULL )
	{
		CGateServer::GetSingleton().Send2Scene( tpPlayer, pMsg );
	}		
}
void CFriendModule::OnMessageKickSwornMemberNotify( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageKickSwornMemberNotify *tpMsg = ( CMessageKickSwornMemberNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer != NULL )
	{
		CGateServer::GetSingleton().Send2Scene( tpPlayer, pMsg );
	}	
}
void CFriendModule::OnMessageDisbandSwornTeamNotify( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageDisbandSwornTeamNotify *tpMsg = ( CMessageDisbandSwornTeamNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer != NULL )
	{
		CGateServer::GetSingleton().Send2Scene( tpPlayer, pMsg );
	}	
}
void CFriendModule::OnMessageChangSwornNameNotify( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageChangeSwornNameNotify *tpMsg = ( CMessageChangeSwornNameNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer != NULL )
	{
		CGateServer::GetSingleton().Send2Scene( tpPlayer, pMsg );
	}
}
void CFriendModule::OnMessageModifyLastTimeAllOnline( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageModifyLastTimeAllOnLineNotify *tpMsg = ( CMessageModifyLastTimeAllOnLineNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer != NULL )
	{
		CGateServer::GetSingleton().Send2Scene( tpPlayer, pMsg );
	}	
}

void CFriendModule::OnMessageChangeSwornName( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageChangeSwornNameNotify *tpMsg = ( CMessageChangeSwornNameNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer != NULL )
	{
		CGateServer::GetSingleton().Send2Scene( tpPlayer, pMsg );
	}	
}