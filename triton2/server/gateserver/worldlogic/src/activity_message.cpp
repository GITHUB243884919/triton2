#include "activity.h"
#include "gateobj_manager.h"
#include "gatelogic.h"
#include "servermessage_in.h"
#include "nameservice.h"
#include "servermessage_pb.hxx.pb.h"
#include "gmmessage_pb.hxx.pb.h"
#include "tinyxml.h"
#include "core_service.h"
#include "gateobj_manager_imp.h"

void CServiceActivity::OnMessage( int nServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);	

	CMessageHead* tpHead = tpMessage->mutable_msghead();
	unsigned short nMessageID = (unsigned short) tpHead->messageid();

	switch (nMessageID)
	{	
	case ID_S2E_USERITEMLIST_REQUEST:
		{
			OnMessageUserItemListRequest(nServerID, tpMessage);
			break;
		}

	case ID_S2E_USERITEMMINUS_REQUEST:
		{
			OnMessageUserItemMinusRequest(nServerID, tpMessage);
			break;
		}

	case ID_S2E_ACTIVITYLIST_REQUEST:
		{
			OnMessageActivityListRequest(nServerID, tpMessage);
			break;
		}

	default:
		{
			LOG_ERROR("default", "[%s:%d]there is no action for the message_id(%d)", 
				__FUNCTION__, __LINE__, nMessageID);
			return;
		}
	}
}

void CServiceActivity::OnMessageActivityListRequest(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageActivityListRequest *tpMsg = (CMessageActivityListRequest *)tpMessage->msgpara();

	//结束时间在当前时间之后，开始时间在当前时间之前的有效的活动列表
	CMessageActivityListResponse tFactMsg;
	tFactMsg.set_roleid(tpMsg->roleid());

	time_t now = time(0);
	ACTIVITYTYPE::iterator iter = m_Activities.begin();
	for(; iter != m_Activities.end(); ++iter)
	{
		if ( iter->second.GetBeginTime() <= now && iter->second.GetEndTime() > now)
		{
			PBActivity *pActivity = tFactMsg.add_activity();
			pActivity->set_menuid(iter->second.GetMenuID());
			//pActivity->set_code(iter->second.GetCode());
			pActivity->set_desc(iter->second.GetDesc());
			pActivity->set_begintime(iter->second.GetBeginTime());
			pActivity->set_endtime(iter->second.GetEndTime());
		}
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_E2S_ACTIVITYLIST_RESPONSE, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tFactMsg);

	CGateServer::GetSingleton().Send2Scene(nServerID, &tMessage);
}

void CServiceActivity::OnMessageUserItemMinusRequest( int vServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);

	CMessageUserItemMinusRequest *tpMsg = ( CMessageUserItemMinusRequest * )tpMessage->msgpara();

	const char* strItemCode = tpMsg->itemcode().c_str();
	if ( memcmp(strItemCode, "i-", 2) != 0 // 特殊符号
		&& strcmp(strItemCode, "s-money-0") != 0 //  未绑定的游戏币
		&& strcmp(strItemCode, "s-money-1") != 0 // 绑定的游戏币
		&& strcmp(strItemCode, "s-silver-1") != 0) // 已绑定的银元宝
	{
		LOG_ERROR("default", "It is invalid item code (%s)", tpMsg->ShortDebugString().c_str()); 
		return;
	}

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(tpMsg->roleid());
	if ( tpPlayer == NULL)
	{
		LOG_ERROR("default", "something is error when %s for role %d", __FUNCTION__, tpMsg->roleid());
		return;
	}

	AGIPUserItemMinus proto;	
	proto.setUserID(tpMsg->accountid());
	proto.setRoleID(tpMsg->roleid());
	proto.setActivityID(tpMsg->activityid());
	proto.setItemGatewayID(CGateServer::GetSingleton().GetRegistID());
	proto.setItemCode(tpMsg->itemcode().c_str());
	proto.setItemNum(tpMsg->itemnum());
	proto.setGameID(tpPlayer->GetGameID());

	CAGIPSession* pSession = CGateLogic::GetSingleton().CreateSession4Proto(&proto);
	if (pSession != NULL)
	{
		int nSessionID = pSession->get_id();

#ifdef _DEBUG_
		LOG_DEBUG("default", "Send2ERating: MsgName[AGIPUserItemMinus] MsgSeq[%d] MsgPara[ AccountID(%d) RoleID(%d) ActivityID(%d) GateWayID(%d) ItemCode(%s) ItemNum(%d) ]",
			nSessionID, tpMsg->accountid(), tpMsg->roleid(), tpMsg->activityid(), 
			CGateServer::GetSingleton().GetRegistID(), tpMsg->itemcode().c_str(), tpMsg->itemnum());
#endif

		pSession->assign(nSessionID, em_agipsession_useritemminus, tpMsg->roleid(), tpMsg->rolelevel(),
			CGateLogic::GetSingleton().mConfig.nEratingSessionTimeout); 

		pSession->mParameter3 = tpMsg->activityid();
		pSession->mParameter4 = tpMsg->itemnum();
		strncpy(pSession->mParameter5, tpMsg->itemcode().c_str(), sizeof(pSession->mParameter5));
		pSession->mParameter5[sizeof(pSession->mParameter5) -1 ] = 0;

		//tpPlayer->mStatus = CGatePlayer::em_status_ingame_v4erating_itemminus;
	}
}

void CServiceActivity::OnMessageUserItemListRequest( int vServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);

	CMessageUserItemListRequest *tpMsg = ( CMessageUserItemListRequest * )tpMessage->msgpara();

	AGIPUserItemList proto;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(tpMsg->roleid());
	LK_ASSERT(tpPlayer != NULL, return);
	//CheckPlayerEventAvailable(tpPlayer->mStatus, CGatePlayer::em_status_ingame_idle);

	proto.setUserID(tpMsg->accountid());
	proto.setRoleID(tpMsg->roleid());

	ACTIVITYTYPE::iterator iter = m_Activities.find(tpMsg->menuid());
	if ( iter == m_Activities.end())
	{
		LOG_ERROR("default", "Can NOT find activity by menuid(%d)", tpMsg->menuid());
		return;
	}

	int nActivityID = tpMsg->menuid();

	proto.setActivityID(nActivityID);
	proto.setItemGatewayID(CGateServer::GetSingleton().GetRegistID());
	proto.setGameID(tpPlayer->GetGameID());

	CAGIPSession* pSession = CGateLogic::GetSingleton().CreateSession4Proto(&proto);
	if (pSession != NULL)
	{
		int nSessionID = pSession->get_id();

#ifdef _DEBUG_
		LOG_DEBUG("default", "Send2ERating: MsgName[AGIPUserItemList] MsgSeq[%d] MsgPara[ AccountID(%d) RoleID(%d) ActivityID(%d) GateWayID(%d) ]",
			nSessionID, tpPlayer->mAccountID, tpMsg->roleid(), nActivityID, CGateServer::GetSingleton().GetRegistID());
#endif

		pSession->assign(nSessionID, em_agipsession_useritemlist, tpMsg->roleid(), nActivityID,
			CGateLogic::GetSingleton().mConfig.nEratingSessionTimeout );

		pSession->mParameter3 = tpMsg->rolelevel();	 // 角色等级
		pSession->mParameter4 = tpMsg->rolemetier(); // 角色职业

		//tpPlayer->mStatus = CGatePlayer::em_status_ingame_v4erating_activityitem;
	}
}
