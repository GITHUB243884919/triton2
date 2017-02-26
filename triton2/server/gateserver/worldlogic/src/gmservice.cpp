#include "gmservice.h"
#include "gateobj_manager.h"
#include "gateobj_manager_imp.h"
#include "gatelogic.h"
#include "servermessage_in.h"
#include "gmmessage_pb.hxx.pb.h"
#include "nameservice.h"
#include "property_pb.hxx.pb.h"
#include "ibstoremodule.h"
#include "activity.h"
#include "core_service.h"
#include "chatmodule.h"
#include "fcmservice.h"
#include "family.h"
#include "family_manager.h"
#include "corps.h"
#include "corpsmanager.h"
#include "corpservice.h"
#include "world.h"
#include "mailmessage_pb.hxx.pb.h"
#include "tasktype.h"
#include "bidmgr.h"
#include <list>
#include <queue>

template<> CServiceGmCmd* CSingleton< CServiceGmCmd >::spSingleton = NULL;

void CServiceGmCmd::OnLaunchServer()
{
	
}

void CServiceGmCmd::OnExitServer()
{
	
}

void CServiceGmCmd::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{

}

void CServiceGmCmd::OnMessage( int nServerID, CMessage* tpMessage )
{
	CMessageHead* tpHead = tpMessage->mutable_msghead();
	unsigned short nMessageID = (unsigned short) tpHead->messageid();
	LK_ASSERT(((int) tpHead->srcid()) == nServerID, return);

	switch (nMessageID)
	{	
		case ID_M2G_MOVEROLE_CMD:
		{
			OnGmMessageMoveRoleCmd(nServerID,  tpMessage);
			break;
		}

		case ID_M2G_KICKROLE_CMD:
		{
			OnGmMessageKickRoleCmd(nServerID,  tpMessage);
			break;
		}

		case ID_M2G_ROLELOCATION_CMD:
		{
			OnGmMessageRoleLocationCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_ENDTASK_CMD:
		{
			OnGmMessageEndTaskCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_TASKINFO_CMD:
		case ID_M2G_FINISHED_TASKINFO_CMD:
		{
			OnGmMessageTaskInfoCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_DELETEROLE_CMD:
		{
			OnGmMessageDeleteRoleCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_FROZENACCOUNT_CMD:
		{
			OnGmMessageFreezeAccountCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_REQUEST_FASTFREEZE_CMD:
		{
			OnGMMessageFreezeAccountBatCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_REQUEST_UNFASTFREEZE_CMD:
		{
			OnGMMessageUnFreezeAccountBatCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_UNFROZENACCOUNT_CMD:
		{
			OnGmMessageUnFreezeAccountCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_TRUSTEEUSER_CMD:
		{
			OnGmMessageTrusteeUserCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_UNTRUSTEEUSER_CMD:
		{
			OnGmMessageUnTrusteeUserCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_SHUTUP_CMD:
		{
			OnGmMessageShutUpCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_CANCLESHUTUP_CMD:
		{
			OnGmMessageCancelShutUpCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_RECOVERROLE_CMD:
		{
			OnGmMessageRecoverRoleCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_POSTBULLETIN_CMD:
		{
			OnGmMessagePostBulletinCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_USERSOCIALINFO_CMD:
		{
			OnGmMessageUserSocialInfoCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_ROLEDETAIL_CMD:
		{
			OnGmMessageRoleDetailCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_QUERY_ROLETITLE_CMD:
		{
			OnGmMessageRoleTitleCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_QUERY_ROLE_MAIL_CMD:
		{
			OnGmMessageQueryRoleMailCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_ROLEPACKINFO_CMD:
		{
			OnGmMessageRolePackInfoCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_ROLEEQUIPMENT_CMD:
		{
			OnGmMessageRoleEquipmentCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_ROLEITEMINPACK_CMD:
		case ID_M2G_ROLEEQUIPMENT_DETAIL_CMD:
		case ID_M2G_ROLESTORAGE_DETAIL_CMD:
		{
			OnGmMessageRoleItemInPackCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_ROLESKILLINFO_CMD:
		{
			OnGmMessageRoleSkillInfoCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_ROLESKILLDETAIL_CMD:
		{
			OnGmMessageRoldSkillDetailCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_USERSTORAGEINFO_CMD:
		{
			OnGMMessageUserStorageInfoCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_IBINFO_CMD:
		case ID_M2G_SETIBPRICE_CMD:
		case ID_M2G_SETIBLOCATION_CMD:
		case ID_M2G_IBONSALE_CMD:
		case ID_M2G_IBNOTSALE_CMD:
		case ID_M2G_IBDESCRIPT_CMD:
		{
			CIbStoreModule::GetSingleton().OnMessage(nServerID, tpMessage);
			break;
		}
	
		case ID_S2E_USERITEMLIST_REQUEST:
		case ID_S2E_USERITEMMINUS_REQUEST:
		case ID_S2E_ACTIVITYLIST_REQUEST:
		{
			CServiceActivity::GetSingleton().OnMessage(nServerID, tpMessage);
			break;
		}

		case ID_S2G_FANGCHENMI_REQUEST:
		case ID_S2G_FCMPLAYERRESET_REQUEST:
		{
			CServiceGateFcm::GetSingleton().OnMessage(nServerID, tpMessage);
			break;
		}

		case ID_M2G_MULTIEXP_CMD:
		{
			OnGMMessageMultiExpCmd(nServerID, tpMessage);
			break;
		}

		case ID_M2G_CANCELMULTIEXP_CMD:
		{
			OnGMMessageCancelMultiExpCmd(nServerID, tpMessage);
			break;
		}

		case ID_S2G_CHANGEGATE_NOTIFY:
		{
			OnGMMessageChangeGate( nServerID, tpMessage );
			break;
		}
		
		case ID_S2G_REQUEST_RESETROLEPASSWORD:
		{
			OnGMMessageResetRolePassword( nServerID, tpMessage );
			break;
		}

		case ID_S2G_NOTIFY_OFFLINEMAIL:
		{
			OnGMMsgSendSysMail2Player( nServerID, tpMessage);
			break;
		}

		case ID_M2G_USERINFOBYIP_CMD:
		{
			OnGmMsgUserInfoByIPCmd(nServerID,  tpMessage);
			break;
		}

		default:
		{
			LOG_ERROR("gm", "[%s:%d]there is no action for the message_id(%d)",
				__FUNCTION__, __LINE__, nMessageID);
			return;
		}
	}
}


void CServiceGmCmd::KickOffPlayer(CGatePlayer* tpPlayer, int reason)
{
	// 玩家在线，需要首先将玩家踢掉, 更新数据库
	if (tpPlayer != NULL )
	{
		if (tpPlayer->mStatus == CGatePlayer::em_status_ingame_idle
		|| tpPlayer->mStatus == CGatePlayer::em_status_ongame_v4scene_logingame)
		{
			CMessageKickOffPlayerRequest tKickOffRequest;

			tKickOffRequest.set_accountid(tpPlayer->mAccountID);
			tKickOffRequest.set_charid(tpPlayer->mCharID);
			tKickOffRequest.set_reason(reason);

			CMessage tMessage;
			tMessage.set_msgpara((uint32_t) &tKickOffRequest);
			pbmsg_sethead(&tMessage, ID_G2S_KICKOFF_PLAYER_REQUEST, 0, 0, 0, 0, 0, 0);

			CGateServer::GetSingleton().Send2Scene(tpPlayer, &tMessage);
		}else 
		if(tpPlayer->mStatus == CGatePlayer::em_status_ingame_v4scene_redirectscene
		|| tpPlayer->mStatus == CGatePlayer::em_status_ingate_idle
		|| tpPlayer->mStatus == CGatePlayer::em_status_ingate_v4erating_createrole
		|| tpPlayer->mStatus == CGatePlayer::em_status_ingate_v4erating_entergame
		|| tpPlayer->mStatus == CGatePlayer::em_status_ingate_v4kickoff
		|| tpPlayer->mStatus == CGatePlayer::em_status_ingate_v4leavemap
		|| tpPlayer->mStatus == CGatePlayer::em_status_ingate_v4loginqueue
		|| tpPlayer->mStatus == CGatePlayer::em_status_ingate_v4mysql_createrole
		|| tpPlayer->mStatus == CGatePlayer::em_status_ingate_v4mysql_obtainroles 
		|| tpPlayer->mStatus == CGatePlayer::em_status_ingate_v4scene_entergame
		|| tpPlayer->mStatus == CGatePlayer::em_status_ingate_erating_createdrole
		|| tpPlayer->mStatus == CGatePlayer::em_status_ingate_erating_enteredgame)
		{
			CPlayerManager::GetSingleton().UnRegistPlayerComm(tpPlayer);
			CPlayerManager::GetSingleton().DestroyGatePlayer(tpPlayer);
		}
	}
}

void CServiceGmCmd::OnGmMessageTrusteeUserCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageTrusteeUserCmd* pGmCmd = (CGMMessageTrusteeUserCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(pGmCmd->user_id());
	if(tpPlayer) 
	{
		KickOffPlayer(tpPlayer, EM_KICKOUT_ACCOUNTTRUSTEE);
	}

	if ( CGateLogic::GetSingleton().ExecuteSql(em_dbsession_freezeaccount, pGmCmd->session_id(), pGmCmd->fd(), 
				pGmCmd->user_id(), SQL_NEED_CALLBACK, UPDATE,
				"update UMS_ACCOUNT set account_status = 2, trustee_passwd = '%s' where account_id = %d", 
				pGmCmd->password().c_str(), pGmCmd->user_id()) != true)
	{
		LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnGmMessageUnTrusteeUserCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageUnTrusteeUserCmd* pGmCmd = (CGMMessageUnTrusteeUserCmd*) tpMessage->msgpara();

	if( ! CGateLogic::GetSingleton().ExecuteSql(em_dbsession_freezeaccount, pGmCmd->session_id(), pGmCmd->fd(), 
				pGmCmd->user_id(), SQL_NEED_CALLBACK, UPDATE,
				"update UMS_ACCOUNT set account_status = 0, trustee_passwd = '' where account_id = %d", 
				pGmCmd->user_id())) 
	{
		LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnSessionTrusteeUserResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	OnSessionFreezeAccountResult(tpMessage, nSessionID, nfd);
}

void CServiceGmCmd::OnSessionUnTrusteeUserResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	OnSessionFreezeAccountResult(tpMessage, nSessionID, nfd);
}

void CServiceGmCmd::OnGmMessageFreezeAccountCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageFreezeAccountCmd* pGmCmd = (CGMMessageFreezeAccountCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(pGmCmd->user_id());
	if(tpPlayer) 
	{
		KickOffPlayer(tpPlayer, EM_KICKOUT_ACCOUNTFROZEN);
	}

	if (! CGateLogic::GetSingleton().ExecuteSql(em_dbsession_freezeaccount, pGmCmd->session_id(), pGmCmd->fd(), 
				pGmCmd->user_id(), SQL_NEED_CALLBACK, UPDATE,
				"update UMS_ACCOUNT set account_status = 1 where account_id = %d", pGmCmd->user_id())) 
	{
		LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

// 批量冻结 [3/10/2010 GaoHong]
void CServiceGmCmd::OnGMMessageFreezeAccountBatCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageFreezeAccountBatCmd* pGmCmd = (CGMMessageFreezeAccountBatCmd*) tpMessage->msgpara();

	const char* tpUsrList = pGmCmd->uid_list().c_str();
	
	char tSQL[ 1024 ] = { 0 };
	sprintf( tSQL, "update UMS_ACCOUNT set account_status = 1 where account_id in (%s)", tpUsrList );

	Tokens tokens = StrSplit( pGmCmd->uid_list(), ",");
	Tokens::iterator iter = tokens.begin();
	for ( ; iter != tokens.end(); ++ iter)
	{
		int tUsrID = strtoul(iter->c_str(), NULL, 0);
		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(tUsrID);
		if(tpPlayer) 
		{				
			KickOffPlayer(tpPlayer, EM_KICKOUT_ACCOUNTFROZEN);
		}
	}

	if (! CGateLogic::GetSingleton().ExecuteSql(em_dbsession_freezeaccount, pGmCmd->session_id(), pGmCmd->fd(), 
		0, SQL_NEED_CALLBACK, UPDATE, tSQL )) 
	{
		LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnGMMessageUnFreezeAccountBatCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageFreezeAccountBatCmd* pGmCmd = (CGMMessageFreezeAccountBatCmd*) tpMessage->msgpara();

	const char* tpUsrList = pGmCmd->uid_list().c_str();
	
	char tSQL[ 1024 ] = { 0 };
	sprintf( tSQL, "update UMS_ACCOUNT set account_status = 0 where account_id in (%s)", tpUsrList );

	/*
	Tokens tokens = StrSplit( pGmCmd->uid_list(), ",");
	Tokens::iterator iter = tokens.begin();
	for ( ; iter != tokens.end(); ++ iter)
	{
		int tUsrID = strtoul(iter->c_str(), NULL, 0);
		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(tUsrID);
		if(tpPlayer) 
		{				
			KickOffPlayer(tpPlayer, EM_KICKOUT_ACCOUNTFROZEN);
		}
	}
	*/

	if (! CGateLogic::GetSingleton().ExecuteSql(em_dbsession_unfreezeaccount, pGmCmd->session_id(), pGmCmd->fd(), 
		0, SQL_NEED_CALLBACK, UPDATE, tSQL )) 
	{
		LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnGmMessageUnFreezeAccountCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageUnFreezeAccountCmd* pGmCmd = (CGMMessageUnFreezeAccountCmd*) tpMessage->msgpara();

	if (! CGateLogic::GetSingleton().ExecuteSql(em_dbsession_unfreezeaccount, pGmCmd->session_id(), pGmCmd->fd(), 
				pGmCmd->user_id(), SQL_NEED_CALLBACK, UPDATE,
				"update UMS_ACCOUNT set account_status = 0 where account_id = %d", pGmCmd->user_id())) 
	{
		LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnSessionFreezeAccountResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	CGMMessageCmdNormalResult tResultMsg;
	tResultMsg.set_session_id(nSessionID);
	tResultMsg.set_fd(nfd);
	tResultMsg.set_result(tpSqlResult->resultcode() == 1 ? 1 : 0);

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg); 

	CGateServer::GetSingleton().Send2Login(&tMessage);
}

void CServiceGmCmd::OnSessionUnFreezeAccountResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	OnSessionFreezeAccountResult(tpMessage, nSessionID, nfd);
}

void CServiceGmCmd::OnSessionRecoverRoleResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	CGMMessageCmdNormalResult tResultMsg;
	tResultMsg.set_session_id(nSessionID);
	tResultMsg.set_fd(nfd);
	tResultMsg.set_result(tpSqlResult->resultcode() >= 1 ? 1 : 0);

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg); 

	CGateServer::GetSingleton().Send2Login(&tMessage);

	//TODO: 从数据库取出，然后加入到RoleInfo列表中	
	/*
	   if (tpSqlResult->resultcode() >= 1)
	   {
	   char stmtfmt[1024];
	   snprintf(stmtfmt, sizeof(stmtfmt) - 1, 
	   "select role_id, role_name, nationality_id, gender_id, face_type "
	   " from UMS_ROLE where role_id = %s limit 0, 1 ", tpSqlResult->fieldvalue(0).c_str());

	   if (! CGateLogic::GetSingleton().ExecuteSql(em_dbsession_allroles, 0, 1,
	   1, SQL_NEED_CALLBACK, SELECT, stmtfmt, 0, 1))
	   {
	   LOG_ERROR("gm", " OnSessionRecoverRoleResult failure.");
	   SendGmCmdNormalResult(0, nSessionID, nfd);
	   }
	   }
	 */

	int nRoleID = atoi(tpSqlResult->fieldvalue(0).c_str());
	int nResult = tpSqlResult->resultcode();
	if ( nResult == 0 || nRoleID == 0)
	{
		LOG_ERROR("gm", " OnSessionRecoverRoleResult failure.");
		SendGmCmdNormalResult(0, nSessionID, nfd);
	}else
	{
		if ( tpSqlResult->colcount() == CServiceRoleIDName::FETCH_FIELD_COUNT )
		{
			CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().CreateRoleInfo(
					atoi(tpSqlResult->fieldvalue(0).c_str()), 
					tpSqlResult->fieldvalue(1).c_str(),
					atoi(tpSqlResult->fieldvalue(7).c_str()));
			if (pRoleInfo != NULL)
			{
				pRoleInfo->SetCountryID(atoi(tpSqlResult->fieldvalue(2).c_str()));
				pRoleInfo->SetGenderID(atoi(tpSqlResult->fieldvalue(3).c_str()));
				pRoleInfo->SetFaceType(atoi(tpSqlResult->fieldvalue(4).c_str()));
				pRoleInfo->SetLevel( atoi(tpSqlResult->fieldvalue(5).c_str()));
				pRoleInfo->SetMetier( atoi(tpSqlResult->fieldvalue(6).c_str()));
			}else
			{
				LOG_ERROR("default", "CreateRoleInfo error because of create object failure");
			}
		}else
		{
			LOG_ERROR("default", "CreateRoleInfo error because no enough field");
		}
	}
}

void CServiceGmCmd::OnGmMessageRecoverRoleCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageRecoverRoleCmd* pGmCmd = (CGMMessageRecoverRoleCmd*) tpMessage->msgpara();


	if ( ! CGateLogic::GetSingleton().ExecuteSql(em_dbsession_recoverrole, pGmCmd->session_id(), pGmCmd->fd(), 
			pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
				"call ProcRecoverRole(%d) ", pGmCmd->role_id())) 
	{
		LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);

		int nResult = 0;
		CGMMessageCmdNormalResult tResultMsg;
		tResultMsg.set_session_id(pGmCmd->session_id());
		tResultMsg.set_fd(pGmCmd->fd());
		tResultMsg.set_result(nResult);

		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &tResultMsg); 
	
		CGateServer::GetSingleton().Send2Login(&tMessage);
	}
}

void CServiceGmCmd::OnGmMessageDeleteRoleCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageDeleteRoleCmd* pGmCmd = (CGMMessageDeleteRoleCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pGmCmd->role_id());

	CGMMessageCmdNormalResult tResultMsg;
	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());

	int nResult = 0;
	if (tpPlayer == NULL && pRoleInfo != NULL) 
	{
		nResult = 1;

		CGateLogic::GetSingleton().ExecuteSql(em_dbsession_null, 0, 0, pGmCmd->role_id(), SQL_NONE, SELECT, 
			"select ProcRemoveRole(%d, %d)", 0, pGmCmd->role_id());

		/*
		#ifdef USING_ERATING
		if (mERatingIsLaunched == true)
		{
			SendDeleteRoleProto2ERating(tpPlayer, vCharID);
		}
		#endif
		*/
	
		CServiceRoleIDName::GetSingleton().RemoveRoleInfoByID(pGmCmd->role_id());
	}

	tResultMsg.set_result(nResult);

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg); 

	CGateServer::GetSingleton().Send2Login(&tMessage);
}

void CServiceGmCmd::OnSessionLoadRoleLocationResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	//int colnum = tpSqlResult->colcount();
	int rownum = tpSqlResult->rowcount();

	CGMMessageRoleLocationResult tResultMsg;
	tResultMsg.set_session_id(nSessionID);
	tResultMsg.set_fd(nfd);

	if ( tpSqlResult->resultcode() == 1 && rownum == 1)
	{
		tResultMsg.set_role_id(atoi(tpSqlResult->fieldvalue(0).c_str()));
		tResultMsg.set_line_id(atoi(tpSqlResult->fieldvalue(1).c_str()));
		tResultMsg.set_map_id(atoi(tpSqlResult->fieldvalue(2).c_str()));
		tResultMsg.set_x(atoi(tpSqlResult->fieldvalue(3).c_str()));
		tResultMsg.set_y(atoi(tpSqlResult->fieldvalue(4).c_str()));
	}else
	{
		tResultMsg.set_line_id(-1);
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_ROLELOCATION_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg); 

	CGateServer::GetSingleton().Send2Login(&tMessage);
}

void CServiceGmCmd::OnSessionLoadTaskInfoResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	//int colnum = tpSqlResult->colcount();
	int rownum = tpSqlResult->rowcount();

	CGMMessageTaskInfoResult tResultMsg;
	tResultMsg.set_session_id(nSessionID);
	tResultMsg.set_fd(nfd);

	if ( tpSqlResult->resultcode() == 1 && rownum == 1)
	{
		int nMsgID = atoi(tpSqlResult->fieldvalue(0).c_str());

		PBTaskList tPBTask;
		if (tPBTask.ParseFromArray(tpSqlResult->fieldvalue(1).c_str(), (int)tpSqlResult->fieldvaluelen(1)))
		{
			if ( nMsgID == ID_M2G_TASKINFO_CMD)
			{		 
				// 正在进行的任务
				for(int i = 0; i < tPBTask.tasklist_size(); ++i)
				{
					PBTaskInfo* pPbTaskInfo = tResultMsg.add_task_detail();
					if(pPbTaskInfo != NULL)
					{
						pPbTaskInfo->set_task_id(tPBTask.tasklist(i).taskid());
						pPbTaskInfo->set_finish_type(tPBTask.tasklist(i).taskprogress());
					}
				}
			} else
			{
				// 完成的任务
				for ( int i = 0; i < tPBTask.completedtask_size(); ++i)
				{
					PBTaskInfo* pPbTaskInfo = tResultMsg.add_task_detail();		
					if(pPbTaskInfo != NULL)
					{
						pPbTaskInfo->set_task_id(tPBTask.completedtask(i));
						pPbTaskInfo->set_finish_type(TASK_END);
					}
				}
			}
		}else
		{
			SendGmCmdNormalResult(0, nSessionID, nfd);
			return;
		}
	}else
	{
		SendGmCmdNormalResult(0, nSessionID, nfd);
		return;
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_TASKINFO_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg); 

	CGateServer::GetSingleton().Send2Login(&tMessage);
}

void CServiceGmCmd::OnGmMessageEndTaskCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageEndTaskCmd* pGmCmd = (CGMMessageEndTaskCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}
	}

	SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
}

void CServiceGmCmd::OnGmMessageTaskInfoCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageTaskInfoCmd* pGmCmd = (CGMMessageTaskInfoCmd*) tpMessage->msgpara();

	CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pGmCmd->role_id());
	if (pRoleInfo == NULL)
	{
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		return;
	}

	int nMsgID = tpMessage->msghead().messageid(); 
	
	if ( !CGateLogic::GetSingleton().ExecuteSql(em_dbsession_taskinfo, pGmCmd->session_id(), pGmCmd->fd(), 
				pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
				"select %d, role_task from UMS_ROLE_DETAIL1_%02d where role_id = %d ", 
				 nMsgID, pRoleInfo->AccountID()%16, pGmCmd->role_id()))
	{
		LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnGmMessageRoleTitleCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageTaskInfoCmd* pGmCmd = (CGMMessageTaskInfoCmd*) tpMessage->msgpara();

	CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pGmCmd->role_id());
	if (pRoleInfo == NULL)
	{
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		return;
	}

	int nMsgID = tpMessage->msghead().messageid(); 
	if ( !CGateLogic::GetSingleton().ExecuteSql(em_dbsession_roletitle, pGmCmd->session_id(), pGmCmd->fd(), 
				pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
				"select %d, role_title from UMS_ROLE_DETAIL_%02d where role_id = %d ", 
				 nMsgID, pRoleInfo->AccountID()%16, pGmCmd->role_id()))
	{
		LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnGmMessageRoleLocationCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageRoleLocationCmd* pGmCmd = (CGMMessageRoleLocationCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}
	}
	
	CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pGmCmd->role_id());
	if (pRoleInfo == NULL)
	{
		CGMMessageRoleLocationResult tResultMsg;
		tResultMsg.set_session_id(pGmCmd->session_id());
		tResultMsg.set_fd(pGmCmd->fd());
		tResultMsg.set_line_id(-1);

		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2M_ROLELOCATION_RESULT, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &tResultMsg); 

		CGateServer::GetSingleton().Send2Login(&tMessage);
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		return;
	}


	if (!CGateLogic::GetSingleton().ExecuteSql(em_dbsession_rolelocation, pGmCmd->session_id(), pGmCmd->fd(), 
				pRoleInfo->RoleID(), SQL_NEED_CALLBACK, SELECT,
				"select role_id, line_id, map_id, pos_x, pos_y from UMS_ROLE where role_id = %d ", 
				pRoleInfo->RoleID())) 
	{
		LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnGmMessagePostBulletinCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessagePostBulletinCmd* pGmCmd = (CGMMessagePostBulletinCmd*) tpMessage->msgpara();
	
	// 直接走网关发送广播消息
	CChatmodule::GetSingletonPtr()->SendSysNotice( pGmCmd->content().c_str(), pGmCmd->times() );

	int nResult = 1;
	CGMMessageCmdNormalResult tResultMsg;
	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());
	tResultMsg.set_result(nResult);

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((unsigned int) &tResultMsg); 

	CGateServer::GetSingleton().Send2Login(&tMessage);
}

void CServiceGmCmd::OnGmMessageUserSocialInfoCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageUserSocialInfoCmd* pGmCmd = (CGMMessageUserSocialInfoCmd*) tpMessage->msgpara();
	LK_ASSERT( tpMessage != NULL, return )	
	
	// 查询家族
	if ( pGmCmd->type() == 1 )
	{
		 GetPlayerSocialFamilyInfo( tpMessage );
	}
	
	// 查询军团
	else if( pGmCmd->type() == 2 )
	{
		GetPlayerSocialCorpsInfo( tpMessage );
	}
	
	// 查询配偶
	else if( pGmCmd->type() == 3 )
	{
		GetPlayerSocialSpouseInfo( tpMessage );
	}
		
	// 查询仙缘
	else if( pGmCmd->type() == 4 )	
	{
		GetPlayerMatixInfo( tpMessage );
	}
	
	// 查询好友
	else if( pGmCmd->type() == 5 )
	{
		GetPlayerSocialFriendInfo( tpMessage );
	}
	else
	{
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}	 		
}

void CServiceGmCmd::GetPlayerSocialFriendInfo( CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageUserSocialInfoCmd* pGmCmd = (CGMMessageUserSocialInfoCmd*) tpMessage->msgpara();
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
					tpPlayer->mLineID, tpPlayer->mMapID);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}else
	{

		CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pGmCmd->role_id());
		if ( !pRoleInfo )
		{
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
			return;
		}

		if ( !CGateLogic::GetSingleton().ExecuteSql(em_dbsession_socialinfo, pGmCmd->session_id(), pGmCmd->fd(), 
					pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
					"select role_friend from UMS_ROLE_DETAIL1_%02d where role_id = %d ", 
					pRoleInfo->AccountID()%16, pGmCmd->role_id()
					))
		{
			LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}	
}

void CServiceGmCmd::GetPlayerSocialFamilyInfo( CMessage* tpMessage )
{
	LK_ASSERT( tpMessage != NULL, return )
	CGMMessageUserSocialInfoCmd* pGmCmd = (CGMMessageUserSocialInfoCmd*) tpMessage->msgpara();	
	LK_ASSERT( pGmCmd != NULL, return )
	CMessage tMsgHead;
	CGMMessageUserSocialInfoResult tResultInfo;
	pbmsg_sethead(&tMsgHead, ID_G2M_USERSOCIALINFO_RESULT, 0, 0, 0, 0, 0, 0);	
	tMsgHead.set_msgpara( (int)&tResultInfo );
	tResultInfo.set_session_id( pGmCmd->session_id() );
	tResultInfo.set_fd( pGmCmd->fd() );
	
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( pGmCmd->role_id() );
	if ( tpFamily != NULL )
	{
	   tResultInfo.set_familyname( tpFamily->GetName() );
	}		
	CGateServer::GetSingleton().Send2Login(&tMsgHead);
}

void CServiceGmCmd::GetPlayerSocialCorpsInfo( CMessage* tpMessage )
{
	LK_ASSERT( tpMessage != NULL, return )
	CGMMessageUserSocialInfoCmd* pGmCmd = (CGMMessageUserSocialInfoCmd*) tpMessage->msgpara();	
	LK_ASSERT( pGmCmd != NULL, return )
	CMessage tMsgHead;
	CGMMessageUserSocialInfoResult tResultInfo;
	pbmsg_sethead(&tMsgHead, ID_G2M_USERSOCIALINFO_RESULT, 0, 0, 0, 0, 0, 0);	
	tMsgHead.set_msgpara( (int)&tResultInfo );
	tResultInfo.set_session_id( pGmCmd->session_id() );
	tResultInfo.set_fd( pGmCmd->fd() );
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( pGmCmd->role_id() );
	if ( tpCorps != NULL )
	{
		tResultInfo.set_corpsname( tpCorps->GetCorpsName() );
	}		
	CGateServer::GetSingleton().Send2Login(&tMsgHead);
}

void CServiceGmCmd::GetPlayerSocialSpouseInfo( CMessage* tpMessage )
{
	LK_ASSERT( tpMessage != NULL, return )
	CGMMessageUserSocialInfoCmd* pGmCmd = (CGMMessageUserSocialInfoCmd*) tpMessage->msgpara();	
	LK_ASSERT( pGmCmd != NULL, return )	 	
	if ( !CGateLogic::GetSingleton().ExecuteSql(em_dbsession_marriageinfo, pGmCmd->session_id(), pGmCmd->fd(), 
		pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
		"select server_sequence, %d, groom_id, bride_id, wedding_time, wedding_module, marriage_time from UMS_MARRIAGE  where groom_id = %d or bride_id=%d ", 
		 pGmCmd->role_id(), pGmCmd->role_id(), pGmCmd->role_id() ) )		 
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());	
	}
}
void CServiceGmCmd::GetPlayerMatixInfo( CMessage* tpMessage )
{
	LK_ASSERT( tpMessage != NULL, return )
	CGMMessageUserSocialInfoCmd* pGmCmd = (CGMMessageUserSocialInfoCmd*) tpMessage->msgpara();	
	LK_ASSERT( pGmCmd != NULL, return )
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
				tpPlayer->mLineID, tpPlayer->mMapID);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}
	else
	{
		LK_ASSERT( tpMessage != NULL, return )
		CGMMessageUserSocialInfoCmd* pGmCmd = (CGMMessageUserSocialInfoCmd*) tpMessage->msgpara();	
		LK_ASSERT( pGmCmd != NULL, return )	 	
		CRoleGlobalInfo *tpRoleGlobalInfo =  CServiceRoleIDName::GetSingleton( ).GetRoleInfoByRoleID( pGmCmd->role_id() );
		LK_ASSERT( tpRoleGlobalInfo != NULL, return )
		int tTableID = tpRoleGlobalInfo->AccountID()%16;
		if ( !CGateLogic::GetSingleton().ExecuteSql(em_dbsession_matixinfo, pGmCmd->session_id(), pGmCmd->fd(), 
				pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
				"select role_matrix from UMS_ROLE_DETAIL1_%02d  where role_id=%d ", 
				tTableID, pGmCmd->role_id()) )		 
			{
				LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
				SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());	
			}
	}
}

void CServiceGmCmd::OnSessionSpouseInfoResult( CMessage* pMessage, int nSessionID, int nfd )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	LK_ASSERT( tpSqlResult != NULL, return )
	int tRowNum = tpSqlResult->rowcount();
	if ( tRowNum == 1 && tpSqlResult->colcount() == 7 )
	{
		CMessage tMsgHead;
		CGMMessageUserSocialInfoResult tResultInfo;
		pbmsg_sethead(&tMsgHead, ID_G2M_USERSOCIALINFO_RESULT, 0, 0, 0, 0, 0, 0);	
		tMsgHead.set_msgpara( (int)&tResultInfo );
		tResultInfo.set_fd( nfd );
		tResultInfo.set_session_id( nSessionID );
		int tMarriageID		= atoi( tpSqlResult->mutable_fieldvalue( 0 )->c_str() );
		int tRoleID			= atoi( tpSqlResult->mutable_fieldvalue( 1 )->c_str() );
		int tGroomID		= atoi( tpSqlResult->mutable_fieldvalue( 2 )->c_str() );
		int tBrideID		= atoi( tpSqlResult->mutable_fieldvalue( 3 )->c_str() );
		int tWeddingTime	= atoi( tpSqlResult->mutable_fieldvalue( 4 )->c_str() );
		int tWeddingModule	= atoi( tpSqlResult->mutable_fieldvalue( 5 )->c_str() );
		int tMarriageTime	= atoi( tpSqlResult->mutable_fieldvalue( 6 )->c_str() );
		if ( tRoleID == tGroomID )
		{
			tResultInfo.set_spouseid( tBrideID );
		}
		else
		{
			tResultInfo.set_spouseid( tGroomID );	
		}					
		tResultInfo.set_marriageid( tMarriageID );			
		if ( tWeddingTime > 0 )
		{
			tm *tpLocalTime = localtime( (time_t *)&tWeddingTime );		
			char tTimeBuffer[40] = { 0 };
			sprintf( tTimeBuffer, "%d-%02d-%02d   %02d:%02d", tpLocalTime->tm_year + 1900, tpLocalTime->tm_mon + 1, tpLocalTime->tm_mday
			, tpLocalTime->tm_hour, tpLocalTime->tm_min );
			tResultInfo.set_weddingtime( tTimeBuffer );
			tResultInfo.set_weddingmodule( tWeddingModule );
			
			memset( tTimeBuffer, 0, sizeof( tTimeBuffer ) );
			tpLocalTime = localtime( (time_t*)&tMarriageTime );
			sprintf( tTimeBuffer, "%d-%02d-%02d   %02d:%02d", tpLocalTime->tm_year + 1900, tpLocalTime->tm_mon + 1, tpLocalTime->tm_mday
				, tpLocalTime->tm_hour, tpLocalTime->tm_min );
			tResultInfo.set_marriagetime( tTimeBuffer );			
		}		
		CGateServer::GetSingleton().Send2Login(&tMsgHead);
	}
	else
	{
		SendGmCmdNormalResult( 0, nSessionID, nfd );
	}
}

void CServiceGmCmd::OnSessionMatrixInfo( CMessage* pMessage, int nSessionID, int nfd )
{
	LK_ASSERT( pMessage != NULL, return )	
	LK_ASSERT( pMessage != NULL, return )
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	LK_ASSERT( tpSqlResult != NULL, return )
	
	CMessage tMsgHead;
	CGMMessageUserSocialInfoResult tResultInfo;
	pbmsg_sethead(&tMsgHead, ID_G2M_USERSOCIALINFO_RESULT, 0, 0, 0, 0, 0, 0);	
	tMsgHead.set_msgpara( (int)&tResultInfo );
	tResultInfo.set_fd( nfd );	
	tResultInfo.set_session_id( nSessionID );
	if ( tpSqlResult->rowcount() == 1 && tpSqlResult->colcount() == 1 )
	{				
		tResultInfo.mutable_matrixinfo()->ParseFromArray( tpSqlResult->mutable_fieldvalue( 0 )->c_str( ), tpSqlResult->mutable_fieldvalue( 0 )->length() ); 				
	}
	CGateServer::GetSingleton().Send2Login(&tMsgHead);
}

void CServiceGmCmd::OnSessionUserSocialInfoResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();
	
	int rownum = tpSqlResult->rowcount();
	if ( tpSqlResult->resultcode() == 1 && rownum == 1)
	{
		CGMMessageUserSocialInfoResult tFactMsg;
		if (!tFactMsg.mutable_friends()->ParseFromArray(tpSqlResult->fieldvalue(0).c_str(), (int)tpSqlResult->fieldvaluelen(0)))
		{
			SendGmCmdNormalResult(0, nSessionID, nfd);
		}else
		{
			tFactMsg.set_fd(nfd);
			tFactMsg.set_session_id(nSessionID);

			CMessage tMessage;
			pbmsg_sethead(&tMessage, ID_G2M_USERSOCIALINFO_RESULT, 0, 0, 0, 0, 0, 0);
			tMessage.set_msgpara((uint32_t) &tFactMsg); 

			CGateServer::GetSingleton().Send2Login(&tMessage);
		}
	}
	else
	{
		SendGmCmdNormalResult( 0, nSessionID, nfd );
	}
}

void CServiceGmCmd::OnGmMessageShutUpCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageShutUpCmd* pGmCmd = (CGMMessageShutUpCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
					tpPlayer->mLineID, tpPlayer->mMapID);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}else
	{
		LOG_ERROR("gm", "There is no this player(RoleID=%d) in game", pGmCmd->role_id());
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnGmMessageCancelShutUpCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageCancelShutUpCmd* pGmCmd = (CGMMessageCancelShutUpCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
					tpPlayer->mLineID, tpPlayer->mMapID);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}else
	{
		LOG_ERROR("gm", "There is no this player(RoleID=%d) in game", pGmCmd->role_id());
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnGmMessageMoveRoleCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageMoveRoleCmd* pGmCmd = (CGMMessageMoveRoleCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
					tpPlayer->mLineID, tpPlayer->mMapID);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}else
	{
		LOG_ERROR("gm", "There is no this player(RoleID=%d) in game", pGmCmd->role_id());
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnGmMessageKickRoleCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageKickRoleCmd* pGmCmd = (CGMMessageKickRoleCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
					tpPlayer->mLineID, tpPlayer->mMapID);
			SendGmCmdNormalResult(1, pGmCmd->session_id(), pGmCmd->fd());
		}
	}else
	{
		LOG_ERROR("gm", "There is no this player(RoleID=%d) in game", pGmCmd->role_id());
		SendGmCmdNormalResult(1, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnGmMessageQueryRoleMailCmd(int nServerID, CMessage* tpMessage)
{
	if (tpMessage == NULL || tpMessage->msgpara() == 0) return;
	CGMMessageQueryRoleMailCmd* pGmCmd = (CGMMessageQueryRoleMailCmd*) tpMessage->msgpara();

	CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pGmCmd->role_id());
	if ( !pRoleInfo )
	{
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		return;
	}

	const int const_per_page = 40;

	if ( !CGateLogic::GetSingleton().ExecuteSql(em_dbsession_rolemail, pGmCmd->page(), pGmCmd->fd(), 
				pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
				" SELECT mail_id, mail_type, mail_status, sender_role_id, sender_role_name, "
				" recver_role_id, recver_role_name, mail_title, mail_body, item_attachment, "
				" money_attachment, send_time, expired_time, post_recver "
				" FROM UMS_ROLE_MAIL_01 WHERE recver_role_id = %d limit %d, %d",				
				pGmCmd->role_id(), 
				(pGmCmd->page() > 0 ? pGmCmd->page() - 1 : 0) * const_per_page, 
				const_per_page 
				))
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}
}

void CServiceGmCmd::OnGmMessageRoleDetailCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageRoleDetailCmd* pGmCmd = (CGMMessageRoleDetailCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
					tpPlayer->mLineID, tpPlayer->mMapID);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}else
	{
		CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pGmCmd->role_id());
		if ( !pRoleInfo )
		{
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
			return;
		}

		if ( !CGateLogic::GetSingleton().ExecuteSql(em_dbsession_roledetail, pGmCmd->session_id(), pGmCmd->fd(), 
					pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
					"SELECT role_name, is_alive, gender_id, metier_id, nationality_id, face_type, \
					role_level, role_expr, role_money, b.role_hp, role_status, a.role_id, a.account_id, \
					line_id, map_id, pos_x, pos_y, login_time, online_duration, b.role_mp, b.role_ap, \
					b.role_basic, b.role_buff, b.role_pvp, b.role_title, b.role_activity, c.role_talent, \
					b.role_cdgroup, c.role_statistic, c.role_mw, c.role_lifeskill, c.role_wizard, c.role_monster_record ,d.lastlogin_ip\
					FROM UMS_ROLE a, UMS_ROLE_DETAIL_%02d b, UMS_ROLE_DETAIL1_%02d c, UMS_ACCOUNT d  WHERE a.role_id = %d \
					AND a.role_id = c.role_id AND a.role_id = b.role_id AND a.account_id = d.account_id", 
					pRoleInfo->AccountID()%16, pRoleInfo->AccountID()%16, pGmCmd->role_id()
					))
		{
			LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}
}

void CServiceGmCmd::OnGmMessageRoleEquipmentCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageRoleEquipmentCmd* pGmCmd = (CGMMessageRoleEquipmentCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
					tpPlayer->mLineID, tpPlayer->mMapID);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}else
	{
		CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pGmCmd->role_id());
		if ( !pRoleInfo )
		{
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
			return;
		}

		if ( !CGateLogic::GetSingleton().ExecuteSql(em_dbsession_roleequip, pGmCmd->session_id(), pGmCmd->fd(), 
					pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
					"select role_equipment from UMS_ROLE_DETAIL_%02d where role_id = %d ", 
					pRoleInfo->AccountID()%16, pGmCmd->role_id()
					))
		{
			LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}

}
void CServiceGmCmd::OnGmMessageRolePackInfoCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageRolePackInfoCmd* pGmCmd = (CGMMessageRolePackInfoCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
					tpPlayer->mLineID, tpPlayer->mMapID);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}else
	{
		CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pGmCmd->role_id());
		if ( !pRoleInfo )
		{
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
			return;
		}

		if (!CGateLogic::GetSingleton().ExecuteSql(em_dbsession_rolepackage, pGmCmd->session_id(), pGmCmd->fd(), 
					pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
					"select role_package from UMS_ROLE_DETAIL_%02d where role_id = %d ", 
					pRoleInfo->AccountID()%16, pGmCmd->role_id()
					))
		{
			LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}
}

void CServiceGmCmd::OnGmMessageRoleItemInPackCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageRoleItemInPackCmd* pGmCmd = (CGMMessageRoleItemInPackCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
					tpPlayer->mLineID, tpPlayer->mMapID);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}else
	{
		CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pGmCmd->role_id());
		if ( !pRoleInfo )
		{
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
			return;
		}

		const char* const_role_field[] = {"role_package", "role_equipment", "role_storage"};
		const char* pFromField = NULL;
		switch (tpMessage->mutable_msghead()->messageid())
		{
			case ID_M2G_ROLEITEMINPACK_CMD:
				pFromField = const_role_field[0];
				break;
			case ID_M2G_ROLEEQUIPMENT_DETAIL_CMD:
				pFromField = const_role_field[1];
				break;
			case ID_M2G_ROLESTORAGE_DETAIL_CMD:
				pFromField = const_role_field[2];
				break;
			default:
				break;
		}

		if ( pFromField == NULL)
		{
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
			return;
		}

		if (!CGateLogic::GetSingleton().ExecuteSql(em_dbsession_iteminpack, pGmCmd->session_id(), pGmCmd->fd(), 
					pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
					"select %d, %s from UMS_ROLE_DETAIL_%02d where role_id = %d ", 
					pGmCmd->item_id(), pFromField, pRoleInfo->AccountID()%16, pGmCmd->role_id()))
		{
			LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}

}

void CServiceGmCmd::OnGmMessageRoleSkillInfoCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageRoleSkillInfoCmd* pGmCmd = (CGMMessageRoleSkillInfoCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
					tpPlayer->mLineID, tpPlayer->mMapID);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}else
	{
		CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pGmCmd->role_id());
		if ( !pRoleInfo )
		{
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
			return;
		}

		//if player is not online, then fetch data from db.
		if ( !CGateLogic::GetSingleton().ExecuteSql(em_dbsession_roleskill, pGmCmd->session_id(), pGmCmd->fd(), 
					pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
					"select role_skill from UMS_ROLE_DETAIL_%02d where role_id = %d ", 
					pRoleInfo->AccountID()%16, pGmCmd->role_id()
					))
		{
			LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}

}

//TODO: only find it in template.
void CServiceGmCmd::OnGmMessageRoldSkillDetailCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageRoleSkillDetailCmd* pGmCmd = (CGMMessageRoleSkillDetailCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
					tpPlayer->mLineID, tpPlayer->mMapID);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}else
	{
		LOG_ERROR("gm", "There is no this player(RoleID=%d) in game", pGmCmd->role_id());
		SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
	}

}

void CServiceGmCmd::OnGMMessageUserStorageInfoCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageUserStorageInfoCmd* pGmCmd = (CGMMessageUserStorageInfoCmd*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pGmCmd->role_id());
	if (tpPlayer != NULL)
	{
		CSceneServer* tpDesScene = CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );
		if ( tpDesScene != NULL )
		{
			CGateServer::GetSingleton().Send2Scene( tpDesScene, tpMessage );                                              
			return;                                                                                    
		}else
		{
			LOG_ERROR("gm", "Can't find the ServerID by (lineid =%d, mapid=%d)",
					tpPlayer->mLineID, tpPlayer->mMapID);

			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}else
	{
		CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(pGmCmd->role_id());
		if ( !pRoleInfo )
		{
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
			return;
		}

		//if player is not online, then fetch data from db.
		if ( !CGateLogic::GetSingleton().ExecuteSql(em_dbsession_userstorageinfo, pGmCmd->session_id(), pGmCmd->fd(), 
					pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
					"select role_storage from UMS_ROLE_DETAIL_%02d where role_id = %d ", 
					pRoleInfo->AccountID()%16, pGmCmd->role_id()
					))
		{
			LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
			SendGmCmdNormalResult(0, pGmCmd->session_id(), pGmCmd->fd());
		}
	}
}

void CServiceGmCmd::OnSessionLoadStorageInfoResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();
	
	int rownum = tpSqlResult->rowcount();
	if ( tpSqlResult->resultcode() == 1 && rownum == 1)
	{
		CGMMessageUserStorageInfoResult proto;

		proto.set_fd(nfd);
		proto.set_session_id(nSessionID);
		if( proto.mutable_items()->ParseFromArray( tpSqlResult->fieldvalue(0).c_str(), (int)tpSqlResult->fieldvaluelen(0)) != true )
		{
			LOG_ERROR("gm", "[%s:%d] tPBItemBox.ParseFromArray error, len(%d)!",
					__FUNCTION__, __LINE__, tpSqlResult->fieldvaluelen(0));

			SendGmCmdNormalResult(0, nSessionID, nfd);
			return;
		}

		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2M_USERSTORAGEINFO_RESULT, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &proto); 

		CGateServer::GetSingleton().Send2Login(&tMessage);
	}else
	{
		SendGmCmdNormalResult(0, nSessionID, nfd);
	}
}

void CServiceGmCmd::OnSessionLoadRoleMailResult(CMessage* tpMessage, int nSessionID, int nfd, int nType)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	CGMMessageSqlQueryResult tResultMsg;
	tResultMsg.set_session_id(0);
	tResultMsg.set_fd(nfd);
	tResultMsg.set_page(nSessionID);
	tResultMsg.set_type(nType);
	*(tResultMsg.mutable_sqlresult()) = *tpSqlResult;

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_SQL_QUERY_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg); 

	CGateServer::GetSingleton().Send2Login(&tMessage);
}

void CServiceGmCmd::OnSessionLoadRoleDetailResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();
	int rownum = tpSqlResult->rowcount();

	CGMMessageRoleDetailResult tResultMsg;

	tResultMsg.set_session_id(nSessionID);
	tResultMsg.set_fd(nfd);

	if ( tpSqlResult->resultcode() == 1 && rownum == 1)
	{
		PBRoleInfo* pproto = tResultMsg.mutable_proto();

		pproto->set_role_id(atoi(tpSqlResult->fieldvalue(11).c_str()));
		pproto->set_role_status(atoi(tpSqlResult->fieldvalue(10).c_str()));
		pproto->set_account_id(atoi(tpSqlResult->fieldvalue(12).c_str()));
		pproto->set_role_name(tpSqlResult->fieldvalue(0).c_str());
		pproto->set_is_alive(atoi(tpSqlResult->fieldvalue(1).c_str()));
		pproto->set_gender_id(atoi(tpSqlResult->fieldvalue(2).c_str()));

		pproto->set_metier_id(atoi(tpSqlResult->fieldvalue(3).c_str()));
		pproto->set_nationality_id(atoi(tpSqlResult->fieldvalue(4).c_str()));
		pproto->set_face_type(atoi(tpSqlResult->fieldvalue(5).c_str()));
		pproto->set_line_id(atoi(tpSqlResult->fieldvalue(13).c_str()));
		pproto->set_map_id(atoi(tpSqlResult->fieldvalue(14).c_str()));
		pproto->set_pos_x(atoi(tpSqlResult->fieldvalue(15).c_str()));
		pproto->set_pos_y(atoi(tpSqlResult->fieldvalue(16).c_str()));

		pproto->set_role_level(atoi(tpSqlResult->fieldvalue(6).c_str()));
		pproto->set_role_expr(atoi(tpSqlResult->fieldvalue(7).c_str()));
		pproto->set_role_money(atoi(tpSqlResult->fieldvalue(8).c_str()));
		pproto->set_bounded_money(atoi(tpSqlResult->fieldvalue(9).c_str()));
		pproto->set_login_time(atoi(tpSqlResult->fieldvalue(17).c_str()));
		pproto->set_online_duration(atoi(tpSqlResult->fieldvalue(18).c_str()));
		char tIp[16] ={ 0 }; 
		
		int temp_ip = atoi(tpSqlResult->fieldvalue(33).c_str());
		unsigned char* p = ( unsigned char* )&temp_ip;
		snprintf( tIp,16,"%d.%d.%d.%d",*(p), *(p+1), *(p+2), *(p+3));
		pproto->set_login_ip( tIp );

		PBProperty *pbProperty = pproto->mutable_proto();

		// hp, mp, ap
		pbProperty->set_curhp( atoi(tpSqlResult->fieldvalue(9).c_str()) );
		pbProperty->set_curmp( atoi(tpSqlResult->fieldvalue(19).c_str()) );
		pbProperty->set_curap( atoi(tpSqlResult->fieldvalue(20).c_str()) );

		// role_basic
		pbProperty->mutable_basicinfo()->ParseFromArray( 
				tpSqlResult->fieldvalue(21).c_str(), (int)tpSqlResult->fieldvaluelen(21));

		// role_buff
		pbProperty->mutable_buff()->ParseFromArray( 
				tpSqlResult->fieldvalue(22).c_str(), (int)tpSqlResult->fieldvaluelen(22));

		// role_pvp
		pbProperty->mutable_pvpinfo()->ParseFromArray( 
				tpSqlResult->fieldvalue(23).c_str(), (int)tpSqlResult->fieldvaluelen(23));

		// role_title
		pbProperty->mutable_titleinfo()->ParseFromArray( 
				tpSqlResult->fieldvalue(24).c_str(), (int)tpSqlResult->fieldvaluelen(24));

		// role_activity
		pbProperty->mutable_activity()->ParseFromArray( 
				tpSqlResult->fieldvalue(25).c_str(), (int)tpSqlResult->fieldvaluelen(25));

		// role_talent
		pbProperty->mutable_talentinfo()->ParseFromArray( 
				tpSqlResult->fieldvalue(26).c_str(), (int)tpSqlResult->fieldvaluelen(26));
			
		// b.role_cdgroup, 
		pbProperty->mutable_cdgroup()->ParseFromArray( 
				tpSqlResult->fieldvalue(27).c_str(), (int)tpSqlResult->fieldvaluelen(27));
		
		// b.role_statistic, 
		pbProperty->mutable_statisticinfo()->ParseFromArray( 
				tpSqlResult->fieldvalue(28).c_str(), (int)tpSqlResult->fieldvaluelen(28));

		// b.role_mw, 
		pbProperty->mutable_mwinfo()->ParseFromArray( 
				tpSqlResult->fieldvalue(29).c_str(), (int)tpSqlResult->fieldvaluelen(29));
		
		// b.role_lifeskill, 
		pbProperty->mutable_lifeskillbox()->ParseFromArray( 
				tpSqlResult->fieldvalue(30).c_str(), (int)tpSqlResult->fieldvaluelen(30));

		// role_wizard
		pbProperty->mutable_wizard()->ParseFromArray( 
				tpSqlResult->fieldvalue(31).c_str(), (int)tpSqlResult->fieldvaluelen(31));

		// role_monster_record 
		pbProperty->mutable_monsterrecord()->ParseFromArray( 
				tpSqlResult->fieldvalue(32).c_str(), (int)tpSqlResult->fieldvaluelen(32));
		

		/*
		if(pbProperty->ParseFromArray( tpSqlResult->fieldvalue(9).c_str(), (int)tpSqlResult->fieldvaluelen(9)) != true )
		{
			LOG_ERROR("gm", "[%s:%d] tPBProperty.ParseFromArray error!", __FUNCTION__, __LINE__);
			SendGmCmdNormalResult(0, nSessionID, nfd);
			return;
		}
		*/
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_ROLEDETAIL_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg); 

	CGateServer::GetSingleton().Send2Login(&tMessage);
}



void CServiceGmCmd::OnSessionLoadRoleEquipmentResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();


	CGMMessageRoleEquipmentResult tResultMsg;

	tResultMsg.set_session_id(nSessionID);
	tResultMsg.set_fd(nfd);

	PBItemBox* pproto = tResultMsg.mutable_proto();

	if(pproto->ParseFromArray( tpSqlResult->fieldvalue(0).c_str(), (int)tpSqlResult->fieldvaluelen(0)) != true )
	{
		LOG_ERROR("gm", "[%s:%d] tPBProperty.ParseFromArray error!", __FUNCTION__, __LINE__);
		SendGmCmdNormalResult(0, nSessionID, nfd);
		return;
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_ROLEEQUIPMENT_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg); 

	CGateServer::GetSingleton().Send2Login(&tMessage);
}


void CServiceGmCmd::OnSessionLoadRolePackInfoResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0,return);

	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();


	CGMMessageRolePackInfoResult tResultMsg;

	tResultMsg.set_session_id(nSessionID);
	tResultMsg.set_fd(nfd);

	PBItemBox* pproto = tResultMsg.mutable_proto();
	if(pproto->ParseFromArray( tpSqlResult->fieldvalue(0).c_str(), (int)tpSqlResult->fieldvaluelen(0)) != true )
	{
		LOG_ERROR("gm", "[%s:%d] tPBProperty.ParseFromArray error!", __FUNCTION__, __LINE__);
		SendGmCmdNormalResult(0, nSessionID, nfd);
		return;
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_ROLEPACKINFO_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg); 

	CGateServer::GetSingleton().Send2Login(&tMessage);
}

void CServiceGmCmd::OnSessionLoadRoleItemInPackResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	CGMMessageRoleItemInPackResult tResultMsg;

	tResultMsg.set_session_id(nSessionID);
	tResultMsg.set_fd(nfd);

	PBItemBox tPBItemBox ; 
	if(tPBItemBox.ParseFromArray( tpSqlResult->fieldvalue(1).c_str(), (int)tpSqlResult->fieldvaluelen(1)) != true )
	{
		LOG_ERROR("gm", "[%s:%d] tPBProperty.ParseFromArray error!", __FUNCTION__, __LINE__);
		SendGmCmdNormalResult(0, nSessionID, nfd);
		return;
	}

	for (int i = 0; i < tPBItemBox.itemobjects_size(); ++i)
	{
		if ( (int) tPBItemBox.itemobjects(i).index() == atoi(tpSqlResult->fieldvalue(0).c_str()))
		{
			*(tResultMsg.mutable_proto()) = tPBItemBox.itemobjects(i);
		}
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_ROLEITEMINPACK_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg); 

	CGateServer::GetSingleton().Send2Login(&tMessage);
}



void CServiceGmCmd::OnSessionLoadRoleSkillInfoResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	CGMMessageRoleSkillInfoResult tResultMsg;

	tResultMsg.set_session_id(nSessionID);
	tResultMsg.set_fd(nfd);

	PBSkillList* pproto = tResultMsg.mutable_proto();
	if(pproto->ParseFromArray( tpSqlResult->fieldvalue(0).c_str(), (int)tpSqlResult->fieldvaluelen(0)) != true )
	{
		LOG_ERROR("gm", "[%s:%d] tPBProperty.ParseFromArray error!", __FUNCTION__, __LINE__);
		SendGmCmdNormalResult(0, nSessionID, nfd);
		return;
	}

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tResultMsg);
	pbmsg_sethead(&tMessage, ID_G2M_ROLESKILLINFO_RESULT, 0, 0, 0, 0, 0, 0);


	CGateServer::GetSingleton().Send2Login(&tMessage);
}

void CServiceGmCmd::SendGmCmdNormalResult(int nResult, unsigned int nSessionID, unsigned int nFd)
{
	CGMMessageCmdNormalResult tResultMsg;
	tResultMsg.set_session_id(nSessionID);
	tResultMsg.set_fd(nFd);

	tResultMsg.set_result(nResult);

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg);

	CGateServer::GetSingleton().Send2Login(&tMessage);
}

unsigned int CServiceGmCmd::CountSize()
{
	return sizeof(CServiceGmCmd);
}


CServiceGmCmd::CServiceGmCmd()
{
	if( mShmPtr->GetInitMode() == SHM_INIT )		
	{
		Initialize();
	}else
	{
		Resume();
	}
}

CServiceGmCmd::~CServiceGmCmd()
{

}

int CServiceGmCmd::Initialize()
{
	return 0;
}

int CServiceGmCmd::Resume()
{

	return 0;
}

void* CServiceGmCmd::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CServiceGmCmd::operator delete( void* pointer )
{

}

void CServiceGmCmd::OnGMMessageMultiExpCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageMultiExpCmd* pGmCmd = (CGMMessageMultiExpCmd*) tpMessage->msgpara();
	if ( pGmCmd->server_id() == 0)
	{
		CGateServer::GetSingleton().Send2Scene((CSceneServer*) NULL, tpMessage); 
	}else
	{
		CGateServer::GetSingleton().Send2Scene(pGmCmd->server_id(), tpMessage); 
	}

	SendGmCmdNormalResult(1, pGmCmd->session_id(), pGmCmd->fd());
}

void CServiceGmCmd::OnGMMessageCancelMultiExpCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CGMMessageCancelMultiExpCmd* pGmCmd = (CGMMessageCancelMultiExpCmd*) tpMessage->msgpara();
	if ( pGmCmd->server_id() == 0)
	{
		CGateServer::GetSingleton().Send2Scene((CSceneServer*) NULL, tpMessage); 
	}else
	{
		CGateServer::GetSingleton().Send2Scene(pGmCmd->server_id(), tpMessage); 
	}

	SendGmCmdNormalResult(1, pGmCmd->session_id(), pGmCmd->fd());
}

void CServiceGmCmd::OnGMMessageChangeGate( int nServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);	

	CMessageChangeGateNotify *pNotify = (CMessageChangeGateNotify*)tpMessage->msgpara();

	switch( pNotify->type() )
	{
	case CMD_FAMILY_MONEY:
		{
			CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByName( pNotify->name().c_str() );
			if ( tpFamily != NULL )
			{
				tpFamily->SetMoney( pNotify->param1() );
			}
			break;
		}
	case CMD_FAMILY_BIDNPC:
		{
			CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByName( pNotify->name().c_str() );
			if ( tpFamily != NULL )
			{
				tpFamily->SetNpcID( pNotify->param1() );
			}
			break;
		}
	case CMD_FAMILY_BIDMONEY:
		{
			CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByName( pNotify->name().c_str() );
			if ( tpFamily != NULL )
			{
				tpFamily->SetBidMoney( pNotify->param1() );
			}
			break;
		}
	case CMD_FAMILY_REPEDATE:
		{
			CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByName( pNotify->name().c_str() );
			if ( tpFamily != NULL )
			{
				tpFamily->SetRepetionDate( pNotify->param1() );
			}
			break;
		}
	case CMD_FAMILY_REPETIME:
		{
			CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByName( pNotify->name().c_str() );
			if ( tpFamily != NULL )
			{
				// 不直接设置，只是增量修改
				tpFamily->SetRepetionOpenTimes( std::max( 0, ( tpFamily->GetRepetionOpenTimes( ) + pNotify->param1() ) ) );
				tpFamily->SetWeekNum( std::max( 0, ( tpFamily->GetWeekNum( ) + pNotify->param1() ) ) );
			}
			break;
		}
	case CMD_CORPS_BIDNPC:
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByName( pNotify->name().c_str() );
			if ( tpCorps != NULL )
			{
				tpCorps->SetBidNpcID( pNotify->param1() );
			}
			break;
		}
	case CMD_CORPS_BIDMONEY:
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByName( pNotify->name().c_str() );
			if ( tpCorps != NULL )
			{
				tpCorps->SetBidMoney( pNotify->param1() );
			}
			break;
		}
	case CMD_CORPS_BATTLECITY:
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByName( pNotify->name().c_str() );
			if ( tpCorps != NULL )
			{
				tpCorps->SetBattleCity( pNotify->param1() );
			}
			break;
		}
	case CMD_CORPS_ADDCITY:
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByName( pNotify->name().c_str() );
			if ( tpCorps != NULL )
			{
				//tpCorps->( pNotify->param1() );
			}
			break;
		}
	case CMD_CORPS_DELCITY:
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByName( pNotify->name().c_str() );
			if ( tpCorps != NULL )
			{
				//tpCorps->( pNotify->param1() );
			}
			break;
		}
	case CMD_CORPS_MONEY:
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByName( pNotify->name().c_str() );
			if ( tpCorps != NULL )
			{
				tpCorps->SetCorpsMoney( pNotify->param1() );
			}
			break;
		}
	case CMD_WORLD_POWER:
		{
			int tWorldID = pNotify->param1();
			int tCountryID = pNotify->param2();
			if ( tWorldID >= WORLD_FIRST && tWorldID < WORLD_NUM && tCountryID > COUNTRY_NONE && tCountryID < COUNTRY_NUM )
			{
				CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ]);
				tpCountry->SetKingPower( pNotify->param3() );
			}
			break;
		}
	case CMD_WORLD_WIZARD:
		{
			int tWorldID = pNotify->param1();
			int tCountryID = pNotify->param2();
			if ( tWorldID >= WORLD_FIRST && tWorldID < WORLD_NUM && tCountryID > COUNTRY_NONE && tCountryID < COUNTRY_NUM )
			{
				CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ]);
				tpCountry->SetWizard( pNotify->param4(), pNotify->param3() );
			}
			break;
		}
	case CMD_WORLD_CONVALUE:
		{
			int tWorldID = pNotify->param1();
			int tCountryID = pNotify->param2();
			if ( tWorldID >= WORLD_FIRST && tWorldID < WORLD_NUM && tCountryID > COUNTRY_NONE && tCountryID < COUNTRY_NUM )
			{
				CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ]);
				tpCountry->SetConstructValue( pNotify->param4(), pNotify->param3() );
			}
			break;
		}
	case CMD_WORLD_CONWIZARD:
		{
			int tWorldID = pNotify->param1();
			int tCountryID = pNotify->param2();
			if ( tWorldID >= WORLD_FIRST && tWorldID < WORLD_NUM && tCountryID > COUNTRY_NONE && tCountryID < COUNTRY_NUM )
			{
				CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ]);
				tpCountry->SetConstructWizard( pNotify->param4(), pNotify->param3() );
			}
			break;
		}
	case CMD_WORLD_CORPSID:
		{
			int tWorldID = pNotify->param1();
			int tMapID = pNotify->param2();
			if ( tWorldID >= WORLD_FIRST && tWorldID < WORLD_NUM )
			{
				CWorldManager::GetSingleton().LoadCorpsFromMap( tWorldID, tMapID, pNotify->param3() );
			}
			break;
		}
	case CMD_WORLD_CLEAR:
		{
			int tWorldID = pNotify->param1();
			int tCountryID = pNotify->param2();
			if ( tWorldID >= WORLD_FIRST && tWorldID < WORLD_NUM && tCountryID > COUNTRY_NONE && tCountryID < COUNTRY_NUM )
			{
				CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ]);
				for ( int n = 0; n < OFFICIAL_LEVEL_NUM; n++ )
				{
					for ( int m = 0; m < MAX_OFFICAL_NUM; m++ )
					{
						memset( tpCountry->mOfficialData[ n ][ m ].mUsetimes, 0, sizeof( tpCountry->mOfficialData[ n ][ m ].mUsetimes ) );
					}
				}
			}
			break;
		}
	case CMD_WORLD_PEOPLE:
		{
			int tWorldID = pNotify->param1();
			int tCountryID = pNotify->param2();
			if ( tWorldID >= WORLD_FIRST && tWorldID < WORLD_NUM && tCountryID > COUNTRY_NONE && tCountryID < COUNTRY_NUM )
			{
				CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ]);
				tpCountry->SetPeopleNum( pNotify->param3() );
			}
			break;
		}
	case CMD_WORLD_CALC_WEAK_LEVEL:
		{
			CWorldModule::GetSingleton().m_WeakProtecter.CalculateCampWeakRank();
			break;
		}
	case CMD_FAMILY_CHASTATUS:
		{
			CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByName( pNotify->name().c_str() );
			if ( tpFamily != NULL )
			{
				tpFamily->SetChallengeStatus( pNotify->param1() );
			}
			break;
		}
	case CMD_CORPS_CHASTATUS:
		{
			CCorps *tpCrops = CCorpsManager::GetSingleton().GetCorpsByName( pNotify->name().c_str() );
			if( tpCrops != NULL )
			{
				tpCrops->SetRoomState( pNotify->param1() );
			}
			break;
		}
	case CMD_FAMILY_WARSTATUS:
		{
			CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByName( pNotify->name().c_str() );
			if ( tpFamily != NULL )
			{
				tpFamily->SetBattleState( pNotify->param1() );
			}
			break;
		}
	case CMD_CORPS_NPC_STATUS:
		{
			//更改军团战NPC状态
			if (pNotify->has_param1() && pNotify->has_param2())
			{
				CBidMgr::GetSingleton().OnGMChangeNpcState(nServerID, pNotify->param1(), pNotify->param2());
			}
			break;
		}

	default:
		break;
	}
	
}

void CServiceGmCmd::OnGMMsgSendSysMail2Player( int nServerID, CMessage* tpMessage )
{
	LK_ASSERT( tpMessage != NULL && tpMessage->msgpara() != 0, return )	
	CMessageOfflineSysMailNotify *pproto = ( CMessageOfflineSysMailNotify * )tpMessage->msgpara();
	int nRoleID = pproto->roleid();

	CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID(nRoleID);
	if ( pRoleInfo == NULL)
	{
		LOG_ERROR("gm", "could not find %d for sysmail %s", nRoleID, pproto->ShortDebugString().c_str());
		return;
	}

	time_t now = time(0);

	int i = 0; 
	while(i < pproto->attachitem_size())
	{
		MailItemBox mailbox;
		const int const_attach_limit = 5;

		for ( int j=0; j< const_attach_limit && i < pproto->attachitem_size(); ++j )
		{
			PBItemObject * pObject = mailbox.add_mailitem();
			*pObject = pproto->attachitem(i++);
			mailbox.add_itemindex(j);
		}

		char stmtsql[MAX_CODE_LEN] = {0};                                                                       

		char szProto[MAX_CODE_LEN] = {0};                                                                       
		char szEscProto[MAX_CODE_LEN] = {0};                                                                    

		if (mailbox.SerializeToArray(szProto, sizeof(szProto)) != true)                                               
		{                                                                                                             
			LOG_ERROR("gm", "mailbox (%s) serialize failed for role %d.\n", 
					nRoleID, pproto->ShortDebugString().c_str());
			return;                                                                                                
		}                                                                                                             

		char  *pAddress = szEscProto;
		if (EscapeDataString( &pAddress, sizeof(szEscProto)-1, szProto, mailbox.ByteSize()) == -1 )           
		{                                                                                                             
			LOG_ERROR("gm", "mailbox (%s) escape_string_for_mysql failed for role %d.\n", 
					nRoleID, pproto->ShortDebugString().c_str());
			return;                                                                                                
		} 

		sprintf(stmtsql, "INSERT INTO UMS_ROLE_MAIL_01(mail_id, mail_type, mail_status, sender_role_id, "
			"sender_role_name, recver_role_id, mail_title, mail_body, send_time, money_attachment, item_number, "
			"item_attachment, post_recver, expired_time, recver_role_name) "
			"VALUES( NULL, 0, 1, 0, '%d', %d, 'NULL', '', %ld, 0, 1, '%s', 0, %ld, 'SYSTEM') ",
			 pproto->mailtype(), nRoleID, now, szEscProto, now + 3600 * 24 * 30);

		if (CGateLogic::GetSingleton().ExecuteSql(em_dbsession_sysmail, pproto->session_id(), pproto->fd(),
					pRoleInfo->AccountID(), SQL_NEED_CALLBACK, INSERT, stmtsql)) 
		{
			LOG_INFO("gm", "sys mail successfully to role (%d) attach %s ", nRoleID, pproto->ShortDebugString().c_str());
		}else
		{
			LOG_ERROR("gm", "sys mail failure to role (%d) attach %s ", nRoleID, pproto->ShortDebugString().c_str());
		}
	}
}

void CServiceGmCmd::OnGmMsgUserInfoByIPCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT( tpMessage != NULL || tpMessage->msgpara() == 0, return )	
	CGMMessageUserInfoByIPCmd *pGmCmd = ( CGMMessageUserInfoByIPCmd * )tpMessage->msgpara();

	// struct in_addr inp;
	// inet_aton(pGmCmd->ip().c_str(), &inp);

	int IPAddr[4] = {0,0,0,0};
	int IPMask[4] = {0xFF,0xFF,0xFF,0xFF};

	Tokens tokens = StrSplit( pGmCmd->ip().c_str(), ".");
	Tokens::iterator iter4Ip = tokens.begin();
	for ( int i = 0; iter4Ip != tokens.end() && i < 4; ++ iter4Ip, ++i)
	{
		if ( *(iter4Ip->c_str()) == '*')
		{
			IPMask[i] = 0;	
			IPAddr[i] = 0;	
		}else
		{
			IPAddr[i] = atoi(iter4Ip->c_str());
		}
	}

	unsigned int unIpAddr = IPAddr[0] + (IPAddr[1] << 8) + (IPAddr[2] << 16) + (IPAddr[3] << 24 );
	unsigned int unIpMask = IPMask[0] + (IPMask[1] << 8) + (IPMask[2] << 16) + (IPMask[3] << 24) ;
	

	time_t nStartTime = GetTimeSecond(pGmCmd->start_time().c_str());
	time_t nEndTime  = GetTimeSecond(pGmCmd->end_time().c_str());

	CGMMsgUserInfoByIPResult proto;
	proto.set_fd(pGmCmd->fd());
	proto.set_session_id(pGmCmd->session_id());
	proto.set_zeroip(unIpAddr == 0 ? 1 : 0);

	typedef std::map<uint32_t, int> ipnumber_t; 
	ipnumber_t ipmap;
	
	// 如果查询的地址是0, 则返回按照IP地址进行排序
	CGateObjManagerImp::ObjMng_PlayerAtGate& tObjPool = CGateObjManager::GetSingleton().mGateObjManagerImp->mPlayerAtGatePool;
	CGateObjManagerImp::ObjMng_PlayerAtGate::iterator iter = tObjPool.begin();
	for(; iter != tObjPool.end(); ++iter)
	{
		CGatePlayer* tpPlayer = &(*iter);
		if (tpPlayer == NULL || tpPlayer->mStatus != CGatePlayer::em_status_ingame_idle) continue;

		if ( tpPlayer->mSockTime >= nStartTime && tpPlayer->mSockTime < nEndTime)
		{
			if ( unIpAddr == 0)
			{
				pair<ipnumber_t::iterator, bool> ret = ipmap.insert(ipnumber_t::value_type((uint32_t) tpPlayer->mClientAddr, 1)); 
				if ( ! ret.second ) ret.first->second ++;
			}else
			{
				IPInfo * pIpInfo = proto.add_ipinfo();
				pIpInfo->set_ip ( (uint32_t) tpPlayer->mClientAddr );
				if ( (((uint32_t) tpPlayer->mClientAddr) & unIpMask )== unIpAddr )
				{
					PlayerInfo* pPlayerInfo = pIpInfo->add_player_info();
					if (pPlayerInfo)
					{
						pPlayerInfo->set_account_id(tpPlayer->mAccountID);
						pPlayerInfo->set_role_id(tpPlayer->mCharID);
						pPlayerInfo->set_role_name(tpPlayer->mCharName);
						pPlayerInfo->set_role_level(tpPlayer->mLevel);
						pPlayerInfo->set_metier_id(tpPlayer->mMetierID);
						pPlayerInfo->set_login_time(tpPlayer->mSockTime);
						pPlayerInfo->set_cpu_id(tpPlayer->mCpuID);
						pPlayerInfo->set_disk_id(tpPlayer->mDiskID);
						pPlayerInfo->set_mac(tpPlayer->m_szMac);
						pPlayerInfo->set_line_id(tpPlayer->mLineID);
						pPlayerInfo->set_map_id(tpPlayer->mMapID);
						pPlayerInfo->set_game_id(tpPlayer->GetGameID());
					}
				}
			}
		}
	}

	if ( unIpAddr == 0)
	{
		// 按照数量进行排序
		typedef priority_queue < pair <int , uint32_t>  > sortlst_t;
		sortlst_t sortlst;

		ipnumber_t::iterator iter3 = ipmap.begin();
		for ( ; iter3 != ipmap.end(); ++iter3)
		{
			sortlst.push(sortlst_t::value_type(iter3->second, iter3->first));
		}

		// 取前50个
		for(int i = 0; !sortlst.empty() && i < 50; ++i, sortlst.pop())
		{
			IPInfo * pIpInfo = proto.add_ipinfo();
			pIpInfo->set_ip(sortlst.top().second);
			pIpInfo->set_role_num(sortlst.top().first);
		}
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_USERINFOBYIP_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &proto); 
	CGateServer::GetSingleton().Send2Login(&tMessage);
}

void CServiceGmCmd::OnGMMessageResetRolePassword( int nServerID, CMessage* tpMessage )
{
	LK_ASSERT( tpMessage != NULL, return )	
	CMessageResetRolePasswordRequest *tpMsg = ( CMessageResetRolePasswordRequest * )tpMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	int tRet = CPlayerManager::GetSingleton().ResetRolePassword( tpMsg->role_id() );
	CMessage tMsgHead;
	CMessageResetRolePasswordResponse  tMsgResetPassword;
	
	if ( tRet == SUCCESS )
	{
		tRet = 1;
	}
	
	tMsgHead.mutable_msghead()->set_messageid( ID_G2S_RESPONSE_RESETROLEPASSWORD );
	tMsgHead.set_msgpara( (int)&tMsgResetPassword );
	tMsgResetPassword.set_errcode( tRet );
	tMsgResetPassword.set_session_id( tpMsg->session_id() );
	tMsgResetPassword.set_fd( tpMsg->fd() );
	CGateServer::GetSingleton().Send2Login( &tMsgHead );
}
