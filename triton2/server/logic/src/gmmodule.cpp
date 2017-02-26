#include "stdafx.hpp"

#include "gmmodule.h"
#include "gmmessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "propertymodule.h"
#include "taskmodule.h"
#include "mapmodule.h"
#include "sceneobj_manager.h"
#include "sceneobj_manager_imp.h"
#include "mapobjmanager.h"
#include "chatmodule.h"
#include "friendmodule.h"
#include "proclient_svr.h"
#include "dbinterface.h"
#include "client_comm_engine.h"
#include "logmodule.h"
#include "activitymodule.h"
#include "maptplmanager.h"
#include "scenelogic.h"

static const char* em_gm_command[] =
{
	"gmKick",
	"gmShutUp",
	"gmUnShutUp",
	"gmFreeze",
	"gmUnFreeze",
	"gmFly",
	"gmInsertItem",
	"gmInsertMoney",
	"gmMoveRole",
	"gmHide",
	"gmIntoFB",
	"gmGate",
	"gmClearSafePwd",
	"gmCreateNpc",
	"gmDestroyNpc",
	"gmReloadMap",
	"gmRankRemove",
	"gmShowNpcInfo",
	"gmChangeRepetion",
	"gmCleanAllRepetion",
	"gmRollBackCorpsMoney",
	"gmBattle",
	"gmMoveTo",
	"gmCountryWar",
	"gmChangeCamp",
	NULL,
};


template<> CGMModule* CSingleton< CGMModule >::spSingleton = NULL;
char CGMModule::sModuleName[ NAME_LENGTH ] = "GMCMD";

unsigned int CGMModule::CountSize(  )
{
	unsigned int tSize = sizeof( CGMModule );
	return tSize;
}

void CGMModule::OnLaunchServer() 
{ 
	GM_LOG* tpCfg = CSceneCfgManager::GetSingletonPtr()->GetGm_log();
	if( tpCfg != NULL )
	{
		INIT_ROLLINGFILE_LOG( tpCfg->mIdentifier, tpCfg->mDirectory, 
				(LogLevel)tpCfg->mPriority , tpCfg->mMaxfilesize, tpCfg->mMaxbackupindex);
	}
}

void CGMModule::OnLoadConfig() 
{ 
	GM_LOG* tpCfg = CSceneCfgManager::GetSingletonPtr()->GetGm_log();
	if( tpCfg != NULL )
	{
		INIT_ROLLINGFILE_LOG( tpCfg->mIdentifier, tpCfg->mDirectory, 
				(LogLevel)tpCfg->mPriority , tpCfg->mMaxfilesize, tpCfg->mMaxbackupindex);
	}
}

void CGMModule::OnExitServer() { }

void CGMModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset ) { }

void CGMModule::OnCreateEntity( CEntity* pEntity , CCreator* pCreator, bool vFirst ) 
{ 
	if ( pEntity == NULL) return;

	if ( pEntity->GetEntityType( ) == ENTITYTYPE_PLAYER )
	{
		CEntityPlayer* tpPlayer = (CEntityPlayer*) pEntity;
		
		if ( tpPlayer->GetValidSeconds() >= 0)
		{
			EFcmStatus nFcmStatus = CheckPlayerFcmStatus(tpPlayer->GetValidSeconds());

			if (nFcmStatus != tpPlayer->GetFcmStatus())
			{
				//TODO: 防沉迷操作
				LOG_INFO("gm", "player(%d:%s) fcm status (%d), and now only have valid (%d) seconds remained" , 
						tpPlayer->GetCharID(), tpPlayer->GetCharNameShow(), nFcmStatus, tpPlayer->GetValidSeconds());

				tpPlayer->SetFcmStatus(nFcmStatus);
			}

			CMessagePlayerFcmStatusNotify  tMsgFcmNofity;
			tMsgFcmNofity.set_fcmstatus(nFcmStatus);
			tMsgFcmNofity.set_validsecond(tpPlayer->GetValidSeconds());

			CMessage tMessage;
			tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_PLAYERFCMSTATUS );
			tMessage.set_msgpara( (uint32) &tMsgFcmNofity );
			CSceneLogic::GetSingleton().Send2Player(tpPlayer, &tMessage);
		}
	}

}

void CGMModule::OnDestroyEntity( CEntity* pEntity, int vCode ) 
{ 
	if ( pEntity == NULL) return;

	if ( pEntity->GetEntityType( ) == ENTITYTYPE_PLAYER )
	{
		CEntityPlayer* tpPlayer = (CEntityPlayer*) pEntity;

		CActItemBuffer::iterator iter = mActItemBuffer.find(tpPlayer->GetCharID());
		if ( iter != mActItemBuffer.end())
		{
			iter->second.clear();
			mActItemBuffer.erase(iter);
		}
	}
}

void CGMModule::OnGateMessage( CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara() && tpMessage->msgpara() != 0, return);


	unsigned short nMessageID = (unsigned short) tpMessage->msghead().messageid();

	switch(nMessageID)
	{
		case ID_M2G_MOVEROLE_CMD:
		{
			OnGmMessageMoveRoleCmd(tpMessage);
			break;
		}

		case ID_M2G_KICKROLE_CMD:
		{
			OnGmMessageKickRoleCmd(tpMessage);
			break;
		}

		case ID_M2G_ROLEDETAIL_CMD:
		{
			OnGmMessageRoleDetailCmd(tpMessage);
			break;
		}

		case ID_M2G_ROLELOCATION_CMD:
		{
			OnGmMessageRoleLocationCmd(tpMessage);
			break;
		}

		case ID_M2G_TASKINFO_CMD:
		{
			OnGmMessageTaskInfoCmd(tpMessage);
			break;
		}

		case ID_G2S_FCMPLAYERLIST_NOTIFY:
		{
			OnMessageFCMPlayerListNotify(tpMessage);
			break;
		}

		case ID_G2S_FCMPLAYERRESET_NOTIFY:
		{
			OnMessageFCMPlayerResetNotify(tpMessage);
			break;
		}

		case ID_G2S_FANGCHENMI_RESPONSE:
		{
			OnMessageFangChenMiResponse(tpMessage);
			break;
		}

		case ID_M2G_SHUTUP_CMD:
		{
			OnGmMessageShutUpCmd(tpMessage);
			break;
		}

		case ID_M2G_CANCLESHUTUP_CMD:
		{
			OnGmMessageCancelShutUpCmd(tpMessage);
			break;
		}

		case ID_M2G_ROLEEQUIPMENT_CMD:
		{
			OnGmMessageRoleEquipmentCmd(tpMessage);
			break;
		}

		case ID_M2G_ROLEPACKINFO_CMD:
		{
			OnGmMessageRolePackInfoCmd(tpMessage);
			break;
		}

		case ID_M2G_ROLEITEMINPACK_CMD:
		case ID_M2G_ROLEEQUIPMENT_DETAIL_CMD:
		case ID_M2G_ROLESTORAGE_DETAIL_CMD:
		{
			OnGmMessageRoleItemInPackCmd(tpMessage);
			break;
		}

		case ID_M2G_ROLESKILLINFO_CMD:
		{
			OnGmMessageRoleSkillInfoCmd(tpMessage);
			break;
		}

		case ID_M2G_ROLESKILLDETAIL_CMD:
		{
			OnGmMessageRoleSkillDetailCmd(tpMessage);
			break;
		}

		case ID_M2G_POSTBULLETIN_CMD:
		{
			OnGmMessagePostBulletinCmd(tpMessage);
			break;
		}

		case ID_M2G_USERSTORAGEINFO_CMD:
		{
			OnGmMessageUserStorageInfoCmd(tpMessage);
			break;
		}

		case ID_M2G_USERSOCIALINFO_CMD:
		{
			OnGmMessageUserSocialInfoCmd(tpMessage);
			break;
		}

		case ID_E2S_USERITEMLIST_RESPONSE:
		{
			OnGmMessageUserItemListResponse(tpMessage);
			break;
		}

		case ID_E2S_USERITEMMINUS_RESPONSE:
		{
			OnGmMessageUserItemMinusResponse(tpMessage);
			break;
		}

		case ID_E2S_ACTIVITYLIST_RESPONSE:
		{
			OnGmMessageActivityListResponse(tpMessage);
			break;
		}

		case ID_M2G_MULTIEXP_CMD:
		{
			OnGmMessageMultiExpCmd(tpMessage);
			break;
		}

		case ID_M2G_CANCELMULTIEXP_CMD:
		{
			OnGmMessageCancelMultiExpCmd(tpMessage);
			break;
		}

		case ID_M2G_ENDTASK_CMD:
		{
			OnGmMessageEndTaskCmd(tpMessage);
			break;
		}

		default:
		{
			LOG_ERROR("gm", "[%s:%d]there is no action for the message_id(%d)", __FUNCTION__, __LINE__, nMessageID);
			return;
		}
	}
}

void CGMModule::OnGmMessageActivityListResponse(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageActivityListResponse* tpFactMsg = (CMessageActivityListResponse*) tpMessage->msgpara();

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(tpFactMsg->roleid());
	if( tpPlayer == NULL ) return;
	
	CMessage tMessage;	
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_LISTACTIVITYMENU );

	CMessageListActivityMenuResponse retmsg;
	for ( int i = 0; i < tpFactMsg->activity_size(); ++i)	
	{
		PBActivity* p1 = tpFactMsg->mutable_activity(i);
		PBOutlineActivity* p2 = retmsg.add_activity();
		if ( p1 && p2 )
		{
			p2->set_menuid(p1->menuid());
			//p2->set_code(p1->code());
			p2->set_desc(p1->desc());
		}
	}
	
	tMessage.set_msgpara( (uint32) &retmsg );
	CSceneLogic::GetSingleton().Send2Player(tpPlayer, &tMessage);
}

void CGMModule::OnGmMessageUserItemMinusResponse(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageUserItemMinusResponse* tpFactMsg = (CMessageUserItemMinusResponse*) tpMessage->msgpara();

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(tpFactMsg->roleid());
	if( tpPlayer == NULL ) return;

	//相当于重新制造，得设置GUID
	PBActivityItem* pbActivityItem = (tpFactMsg->has_item() ? tpFactMsg->mutable_item() : NULL);
	if (pbActivityItem == NULL ) return;

	//int nResult = 0;
	if ( strncmp(pbActivityItem->itemcode().c_str(), "i-", 2) == 0)
	{
		int nItemID = atoi(pbActivityItem->itemcode().c_str() + 2);
		int nItemNumber = pbActivityItem->number();

		CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( nItemID );
		if ( tpItem != NULL )
		{
			if ( tpItem->mPileLimit == 1) 
			{
				//不可堆叠的单个插入
				for ( int i = 0; i < nItemNumber; ++ i )
				{
					CItemObject* tpItemObject = CPropertyModule::GetSingleton().PlayerInsertItem( tpPlayer, nItemID, 1);
					if ( tpItemObject )
					{
						LogEventGetItemByErating(tpPlayer, tpItemObject->GetGUID(), nItemID, 1, pbActivityItem->activityid());
					}else
					{
						LogEventGetItemByErating(tpPlayer, -1, nItemID, 1, pbActivityItem->activityid());
					}
				}
			}else
			{
				// 如果是0或者未配置，则取值为1
				if (tpItem->mPileLimit <= 0) tpItem->mPileLimit = 1;

				// 可堆叠的分堆插入
				for( int nLeftNum = nItemNumber; nLeftNum > 0; nLeftNum -= tpItem->mPileLimit)
				{
					int nThisNum = (nLeftNum > tpItem->mPileLimit) ?  tpItem->mPileLimit : nLeftNum;

					CItemObject* tpItemObject = CPropertyModule::GetSingleton().PlayerInsertItem(tpPlayer, nItemID, nThisNum);
					if ( tpItemObject)
					{
						LogEventGetItemByErating(tpPlayer, tpItemObject->GetGUID(), nItemID, nThisNum, pbActivityItem->activityid());
					}else
					{
						LogEventGetItemByErating(tpPlayer, -1, nItemID, nThisNum, pbActivityItem->activityid());
					}
				}
			}
		}else
		{
			LOG_ERROR("gm", "ItemID(%d) does not exist, pls check template again", nItemID);
			CPropertyModule::GetSingleton().SendProErrorNotice( tpPlayer, ERROR_ITEM_DATANOTEXIST );	
		}
	}else if( strcmp(pbActivityItem->itemcode().c_str(), "s-money-0") == 0)
	{
		// 非绑定钱
		if ( pbActivityItem->number() > 0)
		{
			if( CPropertyModule::GetSingleton().PlayerChangeMoney(tpPlayer, pbActivityItem->number(), false, false) == SUCCESS)
			{
				LogEventGetMoneyByErating(tpPlayer, 
						CYuanBao::em_unbind_money, (pbActivityItem->number()), pbActivityItem->activityid()) ;
			}
		}
	}else if( strcmp(pbActivityItem->itemcode().c_str(), "s-money-1") == 0)
	{
		// 绑定钱
		if ( pbActivityItem->number() > 0)
		{
			if ( CPropertyModule::GetSingleton().PlayerChangeMoney(tpPlayer, pbActivityItem->number(), false, true) == SUCCESS)
			{
				LogEventGetMoneyByErating(tpPlayer, 
						CYuanBao::em_bind_money, (pbActivityItem->number()), pbActivityItem->activityid());
			}
		}
	}else if( strcmp(pbActivityItem->itemcode().c_str(), "s-silver-1") == 0)
	{
		// 银元宝, 游戏控制, erating看不到这个玩意
		if ( pbActivityItem->number() > 0)
		{
			int nSilverNumber = pbActivityItem->number();

			LOG_INFO("gm", "role(%d:%s) get silver yuanbao (%d) to account(%d) from npc",
					tpPlayer->GetCharID(), tpPlayer->GetCharName(), nSilverNumber, tpPlayer->GetAccountID());

			CSceneLogic::GetSingleton().ExecuteSql(updatesilveryb, 0, 0, tpPlayer->GetCharID(), SQL_NONE, UPDATE, 
					"update UMS_ROLE set charged_silver_yb = charged_silver_yb + %d ,"
					"silver_yb = silver_yb + %d where role_id = %d", 
					nSilverNumber, nSilverNumber, tpPlayer->GetCharID());


			CYuanBao& yb = ((CPropertyPlayer* ) tpPlayer->GetProperty())->SilverYB();
			yb.add(nSilverNumber);

			CMessageUserChargeNotice tChargeNotice;
			tChargeNotice.set_userid(tpPlayer->GetAccountID());
			tChargeNotice.set_subjectid(CYuanBao::em_silver_yuanbao);
			tChargeNotice.set_amount(nSilverNumber);
			tChargeNotice.set_detailid(0);

			CMessage tMsg;
			tMsg.set_msgpara((uint32_t) &tChargeNotice);
			pbmsg_sethead(&tMsg, ID_S2C_NOTIFY_USERCHARGE, 0, 0, 0, 0, 0, 0);
			CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMsg);

			LogEventGetMoneyByErating(tpPlayer, 
					CYuanBao::em_silver_yuanbao, (nSilverNumber), pbActivityItem->activityid()) ;
		}
	}

	CMessageMinusActivityItemResponse tMinusNotice;
	tMinusNotice.set_result(0);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tMinusNotice);
	pbmsg_sethead(&tMessage, ID_S2C_RESPONSE_MINUSACTIVITYITEM, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage);
}

void CGMModule::OnGmMessageUserItemListResponse(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageUserItemListResponse* tpFactMsg = (CMessageUserItemListResponse*) tpMessage->msgpara();

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(tpFactMsg->roleid());
	if( tpPlayer == NULL ) return;
	
	CActItemBuffer::iterator iter = mActItemBuffer.find(tpFactMsg->roleid());
	if ( iter != mActItemBuffer.end())
	{
		iter->second.clear();
	}else
	{
		CActItemList vEmptyList;
		vEmptyList.initailize();

		std::pair<CActItemBuffer::iterator, bool> ret = 
			mActItemBuffer.insert(CActItemBuffer::value_type(tpFactMsg->roleid(), vEmptyList));

		if (ret.second == true)
		{
			iter = ret.first;
		}else
		{
			LOG_ERROR("gm", "Insert into emplty activity item list for role(%d:%s) error", 
					tpPlayer->GetCharID(), tpPlayer->GetCharName());
			return;
		}
	}

	LK_ASSERT(iter != mActItemBuffer.end(), return);

	if (  tpFactMsg->items_size() <= 0 )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( tpPlayer, ERROR_PLAYER_HASNO_ACTIVEITEM);	
		return;
	}

	// TODO: 显示物品, 此处对于包裹应该显示包裹; 目前因为没有包裹类产品，因此将所有的物品显示. 
	CMessage tMessage;
	CMessageListActivityItemResponse tMsgItemList;
	tMsgItemList.set_menuid(tpFactMsg->menuid());

	for (int i = 0; i < tpFactMsg->items_size(); ++ i)
	{
		PBActivityItem* ppbActivityItem = tpFactMsg->mutable_items(i);

		if ( !ppbActivityItem )  continue;
		const char* strItemCode = ppbActivityItem->itemcode().c_str();

		if ( memcmp(strItemCode, "i-", 2) == 0  // 特殊符号
		|| strcmp(strItemCode, "s-money-0") == 0 //  未绑定的游戏币
		|| strcmp(strItemCode, "s-money-1") == 0 // 绑定的游戏币
		|| strcmp(strItemCode, "s-silver-1") == 0) // 已绑定的银元宝
		{

			if ( memcmp(strItemCode, "i-", 2) == 0 )
			{
				int nItemID = atoi(&strItemCode[2]);
				CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( nItemID );
				if ( tpItem == NULL )
				{
					LOG_ERROR("gm", "There is no template for item_code(%s) for role_id(%d)", strItemCode, tpFactMsg->roleid());
					continue;
				}
			}

			PBItemIDCount* pbItem = tMsgItemList.add_items();
			pbItem->set_itemcode(ppbActivityItem->itemcode());	
			pbItem->set_itemcount(ppbActivityItem->number());	
			pbItem->set_activityid(ppbActivityItem->activityid());	

			iter->second.push_back(
					CPairItemCount(ppbActivityItem->itemcode().c_str(), 
						ppbActivityItem->activityid(), ppbActivityItem->number()));
		}
	}

	tMessage.mutable_msghead()->set_messageid(ID_S2C_RESPONSE_LISTACTIVITYITEM);
	tMessage.set_msgpara( (uint32) &tMsgItemList );
	CSceneLogic::GetSingleton().Send2Player(tpPlayer, &tMessage);
}

void CGMModule::OnGmMessageUserSocialInfoCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageUserSocialInfoCmd* pGmCmd = (CGMMessageUserSocialInfoCmd*) tpMessage->msgpara();	
	
	CGMMessageUserSocialInfoResult tFactMsg;
	tFactMsg.set_session_id(pGmCmd->session_id());
	tFactMsg.set_fd(pGmCmd->fd());	  
	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_USERSOCIALINFO_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tFactMsg);
		
	if( pGmCmd->type() == 5 )
	{
		CFriendItem* pFriendItem = CFriendModule::GetSingleton().GetPlayerFriendItem(pGmCmd->role_id());
		if (pFriendItem != NULL)
		{
			pFriendItem->GetTlvBuffer2(tFactMsg.mutable_friends());	
		}		
	}	
	else if ( pGmCmd->type() == 4 )
	{
		CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pGmCmd->role_id() );
		if ( tpPlayer != NULL )
		{		
			CPropertyPlayer *tpProperty = ( CPropertyPlayer * )tpPlayer->GetProperty();
			tpProperty->GetMatrixRecordInfoTlvBuffer( tFactMsg.mutable_matrixinfo() );
		}		
	}	     

	CSceneLogic::GetSingleton().Send2GMServer(&tMessage);
}

void CGMModule::OnGmMessagePostBulletinCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessagePostBulletinCmd* pGmCmd = (CGMMessagePostBulletinCmd*) tpMessage->msgpara();

	CChatModule::GetSingleton().SendSysNotice(pGmCmd->content().c_str(), 1);
}

void CGMModule::OnMessageFCMPlayerResetNotify(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageFCMPlayerListNotify* pFcmMsg = (CMessageFCMPlayerListNotify*) tpMessage->msgpara();

	// 序列化消息
	CMessagePlayerFcmStatusNotify  tMsgFcmNofity;

	// reset five hours 
	EFcmStatus nFcmStatus = CheckPlayerFcmStatus(3600 * 5); 

	tMsgFcmNofity.set_fcmstatus(nFcmStatus);
	tMsgFcmNofity.set_validsecond(3600 * 5);

	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_PLAYERFCMSTATUS );
	tMessage.set_msgpara( (uint32) &tMsgFcmNofity );

	static BYTE byMessageBuffer[ MAX_PACKAGE_LEN ] = {0};
	unsigned short ushLen = sizeof( byMessageBuffer );

	if( ClientCommEngine::ConvertMsgToClientCode3( &tMessage, (void*)byMessageBuffer, ushLen ) < 0 )
	{
		LOG_FATAL( "default", "[%s:%d] message %d serialize too long", __FUNCTION__, __LINE__, tMessage.msghead().messageid() );
		return ;
	}

	// 发送消息
	for ( int i = 0 ; i < pFcmMsg->reportplayer_size(); ++i)
	{
		CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pFcmMsg->reportplayer(i));
		if( tpPlayer == NULL ) continue;
	
		tpPlayer->SetFcmStatus(nFcmStatus);
		tpPlayer->ResetAfterFcmKilled();
		CSceneLogic::GetSingletonPtr()->Send2Player( tpPlayer, byMessageBuffer, ushLen, &tMessage );

		// 只要进入不健康时间，就踢下线
		if( tpPlayer->GetFcmStatus() != em_normal_rate )
		{
			SceneServer::CSceneLogic::GetSingletonPtr()->KickPlayer( tpPlayer->GetCharID(), EM_KICKOUT_ANTIADDICTION );
		}
	}
}

void CGMModule::OnMessageFCMPlayerListNotify(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageFCMPlayerListNotify* pFcmMsg = (CMessageFCMPlayerListNotify*) tpMessage->msgpara();

	CMessageFangChenMiRequest tFactMsg;
	for ( int i = 0 ; i < pFcmMsg->reportplayer_size(); ++i)
	{
		CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pFcmMsg->reportplayer(i));
		if( tpPlayer == NULL ) continue;

		PBFangChenMi* pRoleInfo = tFactMsg.add_roledetail();

		pRoleInfo->set_charid(tpPlayer->GetCharID());
		pRoleInfo->set_charlevel(tpPlayer->GetLevel());
		pRoleInfo->set_charmetier(tpPlayer->GetMetier());

		pRoleInfo->set_money1(((CPropertyPlayer*)tpPlayer->GetProperty())->GetMoney());
		pRoleInfo->set_money2(((CPropertyPlayer*)tpPlayer->GetProperty())->GetBindMoney());
		pRoleInfo->set_exp(((CPropertyPlayer*)tpPlayer->GetProperty())->GetExp());
	}

	if (tFactMsg.roledetail_size() > 0)
	{
		CMessage tMessage;
		tMessage.set_msgpara((uint32_t) &tFactMsg);
		pbmsg_sethead(&tMessage, ID_S2G_FANGCHENMI_REQUEST, 0, 0, 0, 0, 0, 0);

		CSceneLogic::GetSingletonPtr()->Send2Gate( &tMessage );
	} 
}

void CGMModule::OnMessageFangChenMiResponse(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CMessageFangChenMiResponse* pFcmMsg = (CMessageFangChenMiResponse*) tpMessage->msgpara();
	
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pFcmMsg->charid()); 
	if(tpPlayer == NULL) return;

	for ( int i=0; i < pFcmMsg->subjects_size(); ++i)
	{
		if ( pFcmMsg->subjects(i).subjectid() == 100) //防沉迷标志
		{
			EFcmStatus nFcmStatus = CheckPlayerFcmStatus(pFcmMsg->subjects(i).amount()); 

			if (nFcmStatus != tpPlayer->GetFcmStatus())
			{
				//TODO: 防沉迷操作
				LOG_INFO("gm", "player(%d:%s) fcm status (%d), and now only have valid (%d) seconds remained" , 
						tpPlayer->GetCharID(), tpPlayer->GetCharNameShow(), nFcmStatus, pFcmMsg->subjects(i).amount());

				tpPlayer->SetFcmStatus(nFcmStatus);

				// 只要进入不健康时间，就踢下线
				if( tpPlayer->GetFcmStatus() != em_normal_rate )
				{
					SceneServer::CSceneLogic::GetSingletonPtr()->KickPlayer( tpPlayer->GetCharID(), EM_KICKOUT_ANTIADDICTION );
				}
			}


			//  向客户端发送当前的经验倍率和剩余的时间
			CMessagePlayerFcmStatusNotify  tMsgFcmNofity;
			tMsgFcmNofity.set_fcmstatus(tpPlayer->GetFcmStatus());
			tMsgFcmNofity.set_validsecond(pFcmMsg->subjects(i).amount());

			CMessage tMessage;
			tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_PLAYERFCMSTATUS );
			tMessage.set_msgpara( (uint32) &tMsgFcmNofity );
		    	CSceneLogic::GetSingleton().Send2Player(tpPlayer, &tMessage);


			// Send2Erating
			CMessageFangChenMiRequest tFactMsg;
			PBFangChenMi* pRoleInfo = tFactMsg.add_roledetail();

			pRoleInfo->set_charid(tpPlayer->GetCharID());
			pRoleInfo->set_charlevel(tpPlayer->GetLevel());
			pRoleInfo->set_charmetier(tpPlayer->GetMetier());

			pRoleInfo->set_money1(((CPropertyPlayer*)tpPlayer->GetProperty())->GetMoney());
			pRoleInfo->set_money2(((CPropertyPlayer*)tpPlayer->GetProperty())->GetBindMoney());
			pRoleInfo->set_exp(((CPropertyPlayer*)tpPlayer->GetProperty())->GetExp());

			tMessage.set_msgpara((uint32_t) &tFactMsg);
			pbmsg_sethead(&tMessage, ID_S2G_FANGCHENMI_REQUEST, 0, 0, 0, 0, 0, 0);
			CSceneLogic::GetSingletonPtr()->Send2Gate( &tMessage );

			break;
		}
	}
}

EFcmStatus CGMModule::CheckPlayerFcmStatus(int nRemainSeconds)
{
	return  nRemainSeconds >= (3600 * 2) ? em_normal_rate : ( nRemainSeconds > 0 ? em_half_rate : em_none_rate);
}

void CGMModule::OnGmMessageTaskInfoCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageTaskInfoCmd* pGmCmd = (CGMMessageTaskInfoCmd*) tpMessage->msgpara();
	
	CGMMessageTaskInfoResult tResultMsg;
	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 
	if (tpPlayer != NULL)
	{
		CTaskList* pTaskList = (CTaskList*) CSceneObjManager::GetSingletonPtr()->GetObject(tpPlayer->GetTaskToken( ));

		if ( pTaskList != NULL)
		{
			 for(int i = 0; i < pTaskList->mTaskNumber; ++i)
			 {
				 PBTaskInfo* pPbTaskInfo = tResultMsg.add_task_detail();
				 if (pPbTaskInfo != NULL)
				 {
					 pPbTaskInfo->set_task_id(pTaskList->mTaskList[i].mTaskID);
					 const char* p = CTaskModule::GetSingleton().GetTaskName(pTaskList->mTaskList[i].mTaskID);

					 if ( p != NULL) pPbTaskInfo->set_task_name(p);

					 pPbTaskInfo->set_finish_type(pTaskList->mTaskList[i].mTaskProgress);
				 }
			 }
		}
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_TASKINFO_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg);

	CSceneLogic::GetSingleton().Send2GMServer(&tMessage);
}

void CGMModule::OnGmMessageRoleLocationCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageRoleLocationCmd* pGmCmd = (CGMMessageRoleLocationCmd*) tpMessage->msgpara();

	
	CGMMessageRoleLocationResult tResultMsg;
	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 
	if (tpPlayer == NULL)
	{
		tResultMsg.set_line_id(-1);
	}else
	{
		tResultMsg.set_line_id(tpPlayer->GetLineID());
		tResultMsg.set_map_id(tpPlayer->GetMapID());
		tResultMsg.set_x(tpPlayer->GetPosX());
		tResultMsg.set_y(tpPlayer->GetPosY());
		tResultMsg.set_role_id(pGmCmd->role_id());
	}

    CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_ROLELOCATION_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg);

	CSceneLogic::GetSingleton().Send2GMServer(&tMessage);
}

bool CGMModule::GetOneValidPos4Player(CMapObject* pMap, CWTPoint here, CWTPoint* pwhere )
{
	typedef struct 
	{
		int x; 
		int y;
	}xy_offset;

	static const xy_offset tNiceCell[] = 
	{ 
		{-1, -1}, 
		{+0, -1}, 
		{+1, -1}, 
		{+1, +0}, 
		{+1, +1},
		{+0, +1}, 
		{-1, +1}, 
		{-1, +0}, 
	};

	if ( pMap == NULL) return false;

	for ( unsigned int i = 0; i < sizeof(tNiceCell)/sizeof(xy_offset); ++i)
	{
		int nX = here.mX + tNiceCell[i].x;
		int nY = here.mY + tNiceCell[i].y;

		if( pMap->IsStaticBlock(CWTPoint(nX, nY)) == false)
		{
			*pwhere = CWTPoint(nX, nY);

			// 只要不是静态阻挡就可以
			return true;

			//CMapObject::CellsEntity* tpCell = pMap->GetCell(CWTPoint(nX, nY));
			//if (tpCell != NULL)
			//{
			//	if (tpCell->size() == 0) 
			//	{
			//		*pwhere = CWTPoint(nX, nY);
			//		return true;
			//	}

			//	/* 不做复杂的判断了
			//	CMapObject::CellsEntity::iterator tBegin = tpCell->begin();
			//	CMapObject::CellsEntity::iterator tEnd = tpCell->end();

			//	for(; tBegin != tEnd; ++ tBegin)
			//	{
			//		CEntity* tpEntity = ( CEntity*) CSceneObjManager::GetSingletonPtr()->GetObject(*tBegin)

			//	}
			//	*/
			//}
		}
	}

	return false;
}


void CGMModule::OnGmMessageMoveRoleCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageMoveRoleCmd* pGmCmd = (CGMMessageMoveRoleCmd*) tpMessage->msgpara();

	int nResult = 0;

	CWTPoint where;
	int 	 nMapID;

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 
	if (tpPlayer != NULL)
	{
		switch(pGmCmd->type())
		{
			case 0: //代表移动到玩家附近, target_location=role_id, now location is role_name?
			{
				CEntityPlayer* tpDestPlayer = 
					CSceneLogic::GetSingletonPtr()->GetPlayerByName(pGmCmd->target_location().c_str()); 
				
				if (tpDestPlayer == NULL) break;

				int nPosX = tpDestPlayer->GetPosX();
				int nPosY = tpDestPlayer->GetPosY();

				CMapObject* pMap = CMapObjManager::GetSingleton( ).GetMapObject(
						tpDestPlayer->GetLineID(), tpDestPlayer->GetMapID(), tpDestPlayer->GetMapIndex());

				if (pMap == NULL ) break;
				
				if ( tpPlayer->GetLineID() == tpDestPlayer->GetLineID())
				{
					nResult = 1;
					nMapID = tpDestPlayer->GetMapID();
					if (GetOneValidPos4Player(pMap, CWTPoint(nPosX, nPosY), &where) == false)
					{
						where = CWTPoint(nPosX, nPosY);
					}

					CMapModule::GetSingleton().Teleport( tpPlayer->GetLineID(), nMapID, where.mX, where.mY, tpPlayer );
				}

				break;
			}

			case 2: //代表移动到npc附近, target_location=npc_id
			{
				/*
				int nNpcID = atoi(pGmCmd->target_location().c_str());
				//遍历mNpcCreatorPool 找到此NPC
				CSceneObjManagerImp::ObjMng_NpcCreator* pNpcCreatorPool;
				pNpcCreatorPool = &(CSceneObjManager::GetSingleton().mSceneObjManagerImp->mNpcCreatorPool);

				if (pNpcCreatorPool == NULL) break;

				CSceneObjManagerImp::ObjMng_NpcCreator::iterator iter = pNpcCreatorPool->begin();
				for(; iter != pNpcCreatorPool->end(); ++iter)
				{
					CNpcCreator* pNpcCreator = &(*iter);
					if (pNpcCreator != NULL)
					{
						if (pNpcCreator->mNpcID == nNpcID)
						{
							CMapObject* pMap = CMapObjManager::GetSingleton( ).GetMapObject(
									tpPlayer->GetLineID(), pNpcCreator->mMapID, tpPlayer->GetMapIndex());

							if (pMap != NULL)
							{
								if (GetOneValidPos4Player(pMap, pNpcCreator->mPosition, &where) == true)
								{
									nResult = 1;
									//nMapID = tpPlayer->GetMapID();
									nMapID = pNpcCreator->mMapID;
								}
							}

							break;
						}
					}
				}
				*/

				break;
			}

			case 1: //代表移动到某个场景的随机位置, target_location=map_id, 目前传送到复活点
			{

				CPropertyPlayer* pProperty = (CPropertyPlayer*) tpPlayer->GetProperty();
				if (pProperty == NULL) break;

				if( CMapModule::GetSingleton().Teleport( 
							pProperty->GetTownPos( )->mLineID, 
							pProperty->GetTownPos( )->mMapID,
							pProperty->GetTownPos( )->mPos.mX, 
							pProperty->GetTownPos( )->mPos.mY, tpPlayer ) == false )
				{
					LOG_ERROR( "pro", "[%s:%d] Use TownTP , but teleport failed", __FUNCTION__, __LINE__ );
				}else
				{
					nResult = 1;
				}

				break;
			}

			default:
			{
				break;
			}
		}
	}

	CGMMessageCmdNormalResult tKickRoleResult;
	tKickRoleResult.set_session_id(pGmCmd->session_id());
	tKickRoleResult.set_fd(pGmCmd->fd());
	tKickRoleResult.set_result(nResult);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tKickRoleResult);
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnGmMessageKickRoleCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageKickRoleCmd* pGmCmd = (CGMMessageKickRoleCmd*) tpMessage->msgpara();

	CSceneLogic::GetSingleton().KickPlayer(pGmCmd->role_id(), EM_KICKOUT_GM );

	CGMMessageCmdNormalResult tKickRoleResult;
	tKickRoleResult.set_session_id(pGmCmd->session_id());
	tKickRoleResult.set_fd(pGmCmd->fd());
	tKickRoleResult.set_result(1);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tKickRoleResult);
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnGmMessageShutUpCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageShutUpCmd* pGmCmd = (CGMMessageShutUpCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	int nResult = 0; 

	if (tpPlayer != NULL)
	{
		// Gm不能被禁言
		if( IsGm(tpPlayer->GetGmStatus()) != true )
		{
			// 单位: 秒
			CChatModule::GetSingleton().SetChannelFalse(pGmCmd->role_id(), pGmCmd->channel(), pGmCmd->period() );
			nResult = 1;
		}
		else
		{
			LOG_ERROR( "gm", "player %s:%d is gm, can't make him shutup", tpPlayer->GetCharNameShow(), tpPlayer->GetCharID() );
		}
	}

	CGMMessageCmdNormalResult tMessageResult;
	tMessageResult.set_session_id(pGmCmd->session_id());
	tMessageResult.set_fd(pGmCmd->fd());
	tMessageResult.set_result(nResult);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tMessageResult);
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnGmMessageCancelShutUpCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageCancelShutUpCmd* pGmCmd = (CGMMessageCancelShutUpCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	int nResult = 0; 

	if (tpPlayer != NULL)
	{
		CChatModule::GetSingleton().SetChannelTrue(pGmCmd->role_id(), pGmCmd->channel());
		nResult = 1;
	}

	CGMMessageCmdNormalResult tMessageResult;
	tMessageResult.set_session_id(pGmCmd->session_id());
	tMessageResult.set_fd(pGmCmd->fd());
	tMessageResult.set_result(nResult);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tMessageResult);
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnGmMessageEndTaskCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageEndTaskCmd* pGmCmd = (CGMMessageEndTaskCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	int nResult = 0; 

	if (tpPlayer != NULL)
	{
		//Only set unTaskID is complete, not end.
		std::vector<unsigned int> rPalayerEntities; 
		rPalayerEntities.push_back(tpPlayer->get_id());

		CTaskModule::GetSingleton().OnForceTaskComplete(rPalayerEntities, pGmCmd->task_id());

		nResult= 1;
	}

	CGMMessageCmdNormalResult tMessageResult;
	tMessageResult.set_session_id(pGmCmd->session_id());
	tMessageResult.set_fd(pGmCmd->fd());
	tMessageResult.set_result(nResult);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tMessageResult);
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnGmMessageTrusteeStorageCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageTrusteeStorageCmd* pGmCmd = (CGMMessageTrusteeStorageCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	int nResult = 0; 

	if (tpPlayer != NULL)
	{
		//TODO: 托管仓库
	}

	CGMMessageCmdNormalResult tMessageResult;
	tMessageResult.set_session_id(pGmCmd->session_id());
	tMessageResult.set_fd(pGmCmd->fd());
	tMessageResult.set_result(nResult);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tMessageResult);
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );

}

void CGMModule::OnGmMessageUnTrusteeStorageCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageUnTrusteeStorageCmd* pGmCmd = (CGMMessageUnTrusteeStorageCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	int nResult = 0; 

	if (tpPlayer != NULL)
	{
		//TODO: 解除托管
	}

	CGMMessageCmdNormalResult tMessageResult;
	tMessageResult.set_session_id(pGmCmd->session_id());
	tMessageResult.set_fd(pGmCmd->fd());
	tMessageResult.set_result(nResult);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tMessageResult);
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnGmMessageChangeMoneyCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageChangeMoneyCmd* pGmCmd = (CGMMessageChangeMoneyCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	int nResult = 0; 

	if (tpPlayer != NULL)
	{
		//TODO: 补偿金钱
	}

	CGMMessageCmdNormalResult tMessageResult;
	tMessageResult.set_session_id(pGmCmd->session_id());
	tMessageResult.set_fd(pGmCmd->fd());
	tMessageResult.set_result(nResult);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tMessageResult);
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );

}

void CGMModule::OnGmMessageChangeExpCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageChangeExpCmd* pGmCmd = (CGMMessageChangeExpCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	int nResult = 0; 

	if (tpPlayer != NULL)
	{
		//TODO: 补偿经验
		int anTeamList[1] = {tpPlayer->GetCharID()};
		int nFinalExp = CPropertyModule::GetSingletonPtr()->PlayerObtainExp2( 
						tpPlayer, pGmCmd->change_num(), 0,  EXPREASON_ACTIVITY, anTeamList, 1  );

		if ( nFinalExp > 0)
		{
			LogEventGetExpByErating(tpPlayer,  pGmCmd->change_num(), nFinalExp, 0);
		}

		nResult = 1;
	}

	CGMMessageCmdNormalResult tMessageResult;
	tMessageResult.set_session_id(pGmCmd->session_id());
	tMessageResult.set_fd(pGmCmd->fd());
	tMessageResult.set_result(nResult);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tMessageResult);
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );

}

void CGMModule::OnGmMessageChangeItemCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageChangeItemCmd* pGmCmd = (CGMMessageChangeItemCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	int nResult = 0; 

	if (tpPlayer != NULL)
	{
		//TODO: 补偿物品 
	}

	CGMMessageCmdNormalResult tMessageResult;
	tMessageResult.set_session_id(pGmCmd->session_id());
	tMessageResult.set_fd(pGmCmd->fd());
	tMessageResult.set_result(nResult);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tMessageResult);
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnGmMessageRoleDetailCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageRoleDetailCmd* pGmCmd = (CGMMessageRoleDetailCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 
	if (tpPlayer == NULL) return;

	CGMMessageRoleDetailResult tResultMsg;

	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());

	PBRoleInfo* pproto = tResultMsg.mutable_proto();

	pproto->set_role_id(tpPlayer->GetCharID());
	pproto->set_role_status(tpPlayer->GetRoleStatus());
	pproto->set_account_id(tpPlayer->GetAccountID());
	pproto->set_role_name(tpPlayer->GetCharName());
	pproto->set_is_alive(tpPlayer->GetAliveStatus());
	pproto->set_gender_id(tpPlayer->GetSex());
	pproto->set_metier_id(tpPlayer->GetMetier());
	pproto->set_nationality_id(tpPlayer->GetNationality());
	pproto->set_face_type(tpPlayer->GetFaceType());
	pproto->set_line_id(tpPlayer->GetLineID()); 
	pproto->set_map_id(tpPlayer->GetMapID());
	pproto->set_pos_x(tpPlayer->GetPosX()); 
	pproto->set_pos_y(tpPlayer->GetPosY());
	pproto->set_role_level(tpPlayer->GetLevel()); 
	pproto->set_role_expr(tpPlayer->GetExp()); 
	pproto->set_role_money(tpPlayer->GetMoney()); 
	pproto->set_bounded_money(tpPlayer->GetBindMoney()); 
	pproto->set_login_time(tpPlayer->GetLastLoginTime()); 
	pproto->set_online_duration(tpPlayer->GetOnlineTime()); 
	//pproto->set_update_time();

	char tIp[16] ={ 0 }; 

	int temp_ip = tpPlayer->GetClientInfo()->mClientAddr;
	unsigned char* p = ( unsigned char* )&temp_ip;
	snprintf( tIp,16,"%d.%d.%d.%d",*(p), *(p+1), *(p+2), *(p+3));
	pproto->set_login_ip( tIp );

	PBProperty* pbProperty = pproto->mutable_proto();
	CPropertyPlayer* pProperty = (CPropertyPlayer*) tpPlayer->GetProperty();

	// other info
	pProperty->GetTotalBufferForClient(pbProperty, 1);

	// hp, mp, ap
	if ( pbProperty != NULL)
	{
		pbProperty->set_curhp(pProperty->GetCurHP());
		pbProperty->set_curmp(pProperty->GetCurAP());
		pbProperty->set_curap(pProperty->GetCurMP());

		// basicinfo
		pProperty->GetBasicInfoTlvBuffer(pbProperty->mutable_basicinfo());
	}

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tResultMsg);
	pbmsg_sethead(&tMessage, ID_G2M_ROLEDETAIL_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnGmMessageRoleEquipmentCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageRoleEquipmentCmd* pGmCmd = (CGMMessageRoleEquipmentCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 
	if( tpPlayer == NULL )
	{
		return;
	}

	CGMMessageRoleEquipmentResult tResultMsg;

	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());

	PBItemBox* pproto = tResultMsg.mutable_proto();

	((CPropertyPlayer*) tpPlayer->GetProperty())->GetEquipment()->GetTlvBuffer2(pproto);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tResultMsg);
	pbmsg_sethead(&tMessage, ID_G2M_ROLEEQUIPMENT_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnGmMessageUserStorageInfoCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageUserStorageInfoCmd* pGmCmd = (CGMMessageUserStorageInfoCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	if (tpPlayer == NULL) return;

	CGMMessageUserStorageInfoResult tResultMsg;

	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());

	PBItemBox* pproto = tResultMsg.mutable_items();

	((CPropertyPlayer*) tpPlayer->GetProperty())->GetStorage()->GetTlvBuffer2(pproto);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tResultMsg);
	pbmsg_sethead(&tMessage, ID_G2M_USERSTORAGEINFO_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnGmMessageRolePackInfoCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageRolePackInfoCmd* pGmCmd = (CGMMessageRolePackInfoCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	if (tpPlayer == NULL) return;


	CGMMessageRolePackInfoResult tResultMsg;

	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());

	PBItemBox* pproto = tResultMsg.mutable_proto();

	((CPropertyPlayer*) tpPlayer->GetProperty())->GetBaggage()->GetTlvBuffer2(pproto);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tResultMsg);
	pbmsg_sethead(&tMessage, ID_G2M_ROLEPACKINFO_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnGmMessageRoleItemInPackCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageRoleItemInPackCmd* pGmCmd = (CGMMessageRoleItemInPackCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	if (tpPlayer == NULL) return;


	CGMMessageRoleItemInPackResult tResultMsg;

	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());

	CItemObject* pItemObj = NULL;
	CPropertyPlayer* tpProperty = ((CPropertyPlayer*) tpPlayer->GetProperty());

	unsigned short nMessageID = (unsigned short) tpMessage->msghead().messageid();
	switch (nMessageID)
	{
		case ID_M2G_ROLEITEMINPACK_CMD:
		{
			pItemObj = tpProperty->GetBaggage()->GetItemObjPtr(pGmCmd->item_id());
			break;
		}
		case ID_M2G_ROLEEQUIPMENT_DETAIL_CMD:
		{
			pItemObj = tpProperty->GetEquipment()->GetItemObjPtr(pGmCmd->item_id());
			break;
		}
		case ID_M2G_ROLESTORAGE_DETAIL_CMD:
		{
			pItemObj = tpProperty->GetStorage()->GetItemObjPtr(pGmCmd->item_id());
			break;
		}

		default: 
			break;
	}


	if (pItemObj == NULL) return;

	PBItemObject* pproto = tResultMsg.mutable_proto();
	/*
	pproto->set_itemid( pItemObj->GetItemID() );
	pproto->set_number( pItemObj->GetItemNum() );
	pproto->set_bindstatus( pItemObj->GetBindStatus() );
	pproto->set_usedtimes( pItemObj->GetUsedTimes() );
	pproto->set_index( pGmCmd->item_id() );
	pproto->set_invalidtime(pItemObj->GetInvalidTime());
	pItemObj->GetItemBinary2( pproto );
	*/

	pItemObj->GetItemBinaryBuffer2( pproto );

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tResultMsg);
	pbmsg_sethead(&tMessage, ID_G2M_ROLEITEMINPACK_RESULT, 0, 0, 0, 0, 0, 0);



	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnGmMessageRoleSkillInfoCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageRoleSkillInfoCmd* pGmCmd = (CGMMessageRoleSkillInfoCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	if (tpPlayer == NULL) return;


	CGMMessageRoleSkillInfoResult tResultMsg;

	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());

	PBSkillList* pproto = tResultMsg.mutable_proto();
	((CPropertyPlayer*)tpPlayer->GetProperty())->GetSkillList().GetTlvBuffer2(pproto);

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tResultMsg);
	pbmsg_sethead(&tMessage, ID_G2M_ROLESKILLINFO_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );

}

void CGMModule::OnGmMessageRoleSkillDetailCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageRoleSkillDetailCmd* pGmCmd = (CGMMessageRoleSkillDetailCmd*) tpMessage->msgpara();
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID(pGmCmd->role_id()); 

	if (tpPlayer == NULL) return;


	CGMMessageRoleSkillDetailResult tResultMsg;

	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());

	PBSkillObject* pproto = tResultMsg.mutable_proto();

	CSkillObject* pSrcObj = ((CPropertyPlayer*)tpPlayer->GetProperty())->GetSkillList().GetSkillObjectByID(pGmCmd->skill_id());

	if (pSrcObj != NULL) 
	{
		pproto->set_skillid( pSrcObj->GetSkillID() );
		pproto->set_skilllevel( pSrcObj->GetSkillLevel() );
		pproto->set_cooldown( pSrcObj->GetCooldown() );
	}

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tResultMsg);
	pbmsg_sethead(&tMessage, ID_G2M_ROLESKILLDETAIL_RESULT, 0, 0, 0, 0, 0, 0);

	CSceneLogic::GetSingletonPtr()->Send2GMServer( &tMessage );
}

void CGMModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL, return);
	LK_ASSERT(pMessage->has_msgpara()  && pMessage->msgpara() != 0, return);

	if ( pPlayer == NULL ||  ! IsGm(pPlayer->GetGmStatus()) ) return;

	CMessageProDebugCommand* pDebugMsg = ( CMessageProDebugCommand* )pMessage->msgpara();	
	Tokens tToken = StrSplit( pDebugMsg->debugcommond(), " " );

	const char* tCommandName = tToken[0].c_str();
	em_gm_right gm_right = em_priv_none;

	const char** ppDebugCommand = em_gm_command; 
	for(; *ppDebugCommand != NULL; ++ppDebugCommand)
	{
		if (strcmp(tCommandName, *ppDebugCommand) == 0)
		{
			int nIndex = (ppDebugCommand - em_gm_command);
			gm_right = (em_gm_right) (1 << nIndex);
			break;
		}
	}

	if ( HasRight(pPlayer, gm_right) == false) return;

	switch (gm_right) 
	{
		case em_priv_kickoff_player :
		{
			if ( tToken.size() < 2)  return;
			CEntityPlayer* tpDestPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByName(tToken[1].c_str()); 
			if (tpDestPlayer)
			{
				SceneServer::CSceneLogic::GetSingletonPtr()->KickPlayer( tpDestPlayer->GetCharID(), EM_KICKOUT_GM );
			}
			break;
		}

		case em_priv_shutoff_player :
		{
			if ( tToken.size() < 2 )  return;

			int nForbidTime = 24*60*60; // 默认禁言24小时,单位秒
			if( tToken.size() ==  3 ) 
			{
				//TODO: 考虑到人性化，以后给客户的单位应该是小时，没必要用秒这么精确的单位。
				if( (int) ::atoi(tToken[2].c_str()) > 0 )
				{
					nForbidTime = (int) ::atoi( tToken[2].c_str() );
				}
			}

			CEntityPlayer* tpDestPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByName(tToken[1].c_str()); 
			if (tpDestPlayer)
			{
				CChatModule::GetSingleton().SetChannelFalse(tpDestPlayer->GetCharID(), CHANNEL_ALL, nForbidTime );
			}

			break;
		}

		case em_priv_cancel_shutoff :
		{
			if ( tToken.size() < 2)  return;

			CEntityPlayer* tpDestPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByName(tToken[1].c_str()); 
			if (tpDestPlayer)
			{
				CChatModule::GetSingleton().SetChannelTrue(tpDestPlayer->GetCharID(), CHANNEL_ALL);
			}

			break;
		}

		case em_priv_freeze_player :
		{
			if ( tToken.size() < 2)  return;

			CEntityPlayer* tpDestPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByName(tToken[1].c_str()); 
			if (tpDestPlayer)
			{
				CSceneLogic::GetSingleton().ExecuteSql(
					freeze_account, 0, 0, 0, SQL_NONE, UPDATE, 
					"update UMS_ACCOUNT set account_status = 1 where account_id = "
					"(select account_id from UMS_ROLE where role_name = '%s')", tToken[1].c_str());

				SceneServer::CSceneLogic::GetSingletonPtr()->KickPlayer( tpDestPlayer->GetCharID());
			}
					
			break;
		}

		case em_priv_unfreezen_player :
		{
			if ( tToken.size() < 2)  return;

			CSceneLogic::GetSingleton().ExecuteSql(
					freeze_account, 0, 0, 0, SQL_NONE, UPDATE, 
					"update UMS_ACCOUNT set account_status = 0 where account_id = "
					"(select account_id from UMS_ROLE where role_name = '%s')", tToken[1].c_str());

			break;
		}

		case em_priv_fly :
		{
			if ( tToken.size() < 3)  return;

			int tPosX = atoi( tToken[1].c_str() );
			int tPosY = atoi( tToken[2].c_str() );

			CMapModule::GetSingleton().Teleport( pPlayer->GetLineID(), pPlayer->GetMapID(), tPosX, tPosY, pPlayer );
			break;
		}

		case em_priv_insert_item :
		{
#ifdef _DEBUG_
			if ( tToken.size() < 3) return;

			int tItemID = atoi( tToken[ 1 ].c_str() );
			int tNumber = atoi( tToken[ 2 ].c_str() );

			CItemObject* tpItemObject = CPropertyModule::GetSingleton().PlayerInsertItem( pPlayer, tItemID, tNumber);
			if ( tpItemObject )
			{
				LogEventGetItemByGmInsert(pPlayer, tpItemObject->GetGUID(), tItemID, tNumber);
			}
#endif
			break;
		}

		case em_priv_insert_money:
		{
#ifdef _DEBUG_
			if ( tToken.size() < 2) return;

			int tMoney = atoi( tToken[1].c_str() );
			if (CPropertyModule::GetSingleton().SetPlayerMoney( pPlayer->GetEntityID(), tMoney ) == 0)
			{
				LogEventGetMoneyByGMInsert(pPlayer, CYuanBao::em_unbind_money, tMoney);
			}
#endif
			break;
		}

		case em_priv_move :
		{
			if ( tToken.size() < 2) return;

			CMessageGateEntityGMMove tMove;
			CMessage		tMsg;

			tMove.set_rolename( tToken[1].c_str( ) );

			if( tToken.size() >= 6 )
			{
				tMove.set_lineid( atoi( tToken[ 2 ].c_str() ) );
				tMove.set_mapid( atoi( tToken[ 3 ].c_str() ) );
				tMove.set_posx( atoi( tToken[ 4 ].c_str( ) ) );
				tMove.set_posy( atoi( tToken[ 5 ].c_str( ) ) );	
			}
			else
			{
				tMove.set_lineid( pPlayer->GetLineID() );
				tMove.set_mapid( pPlayer->GetMapID() );
				tMove.set_posx( pPlayer->GetPosX() );
				tMove.set_posy( pPlayer->GetPosY() );	
			}

			tMsg.mutable_msghead( )->set_messageid( ID_S2G_ENTITY_GM_MOVE );
			tMsg.set_msgpara( (uint32)&tMove );

			CSceneLogic::GetSingletonPtr()->Send2Gate( &tMsg );

			break;
		}

		case em_priv_is_gm :
		{
			break;
		}
		case em_priv_hide:
		{
			if( tToken.size() < 2 ) return;
			
			bool bHideOn = ( atoi(tToken[1].c_str()) == 1 ) ? true : false;
			if( bHideOn == true )
			{
				CPropertyModule::GetSingletonPtr( )->InsertBuff( pPlayer, 60019 );
			}
			else
			{
				CPropertyModule::GetSingletonPtr( )->RemoveBuff( pPlayer, 60019 );
			}

			CPropertyModule::GetSingletonPtr()->ResultListSend( ); 
			break;
		}
		case em_priv_rank_remove:
		{
			CMessageRankRemove tRemove;

			CMessage			tMsg;

			if( tToken.size() == 2 ) 
			{
				if( strcmp( tToken[ 1 ].c_str(), "refresh" ) == 0 )
				{
					tRemove.set_refresh( 1 );
				}
				else
				{
					return;
				}
			}
			else if( tToken.size() == 3)
			{
				tRemove.set_charname( tToken[ 1 ] );
				tRemove.set_ranktype( atoi( tToken[ 2 ].c_str() ) );
			}
			else
			{
				return;
			}

            tMsg.mutable_msghead( )->set_messageid( ID_S2G_RANK_REMOVE );
            tMsg.set_msgpara( (uint32)&tRemove );
			
			CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMsg );	

			break;
		}
		case em_priv_gate:
		{
			if( tToken.size() < 2 ) return;

			CMessageChangeGateNotify tChangeNotify;
			CMessage	tMsg;
			tMsg.mutable_msghead( )->set_messageid( ID_S2G_CHANGEGATE_NOTIFY );
			tMsg.set_msgpara( (uint32)&tChangeNotify );

			if ( strcmp( tToken[1].c_str(), "world_calc_level" ) == 0 )
			{
				tChangeNotify.set_type( CMD_WORLD_CALC_WEAK_LEVEL );
				CSceneLogic::GetSingletonPtr()->Send2Gate( &tMsg );
				break;
			}
			else if (tToken.size() < 4 )
			{
				return;
			}

			if ( strcmp( tToken[1].c_str(), "corps_npc_stat" ) == 0 )
			{
				tChangeNotify.set_type( CMD_CORPS_NPC_STATUS );
				// npc_id
				tChangeNotify.set_param1( ::atoi( tToken[2].c_str() ) );
				// npc status
				tChangeNotify.set_param2( ::atoi( tToken[3].c_str() ) );
			}

			if ( tChangeNotify.type() != 0 )
			{
				CSceneLogic::GetSingletonPtr()->Send2Gate( &tMsg );
				break;
			}	

			if ( strcmp( tToken[1].c_str(), "family_money" ) == 0 )
			{				
				tChangeNotify.set_type( CMD_FAMILY_MONEY );				
			}
			else if ( strcmp( tToken[1].c_str(), "family_bidnpc" ) == 0 )
			{
				tChangeNotify.set_type( CMD_FAMILY_BIDNPC );
			}
			else if ( strcmp( tToken[1].c_str(), "family_bidmoney" ) == 0 )
			{
				tChangeNotify.set_type( CMD_FAMILY_BIDMONEY );
			}
			else if ( strcmp( tToken[1].c_str(), "family_repetime" ) == 0 )
			{
				tChangeNotify.set_type( CMD_FAMILY_REPETIME );
			}
			else if ( strcmp( tToken[1].c_str(), "family_repedate" ) == 0 )
			{
				tChangeNotify.set_type( CMD_FAMILY_REPEDATE );
			}
			else if ( strcmp( tToken[1].c_str(), "corps_bidmoney" ) == 0 )
			{				
				tChangeNotify.set_type( CMD_CORPS_BIDMONEY );				
			}
			else if ( strcmp( tToken[1].c_str(), "corps_bidnpc" ) == 0 )
			{
				tChangeNotify.set_type( CMD_CORPS_BIDNPC );
			}
			else if ( strcmp( tToken[1].c_str(), "corps_battlecity" ) == 0 )
			{
				tChangeNotify.set_type( CMD_CORPS_BATTLECITY );
			}
			else if ( strcmp( tToken[1].c_str(), "corps_addcity" ) == 0 )
			{
				tChangeNotify.set_type( CMD_CORPS_ADDCITY );
			}
			else if ( strcmp( tToken[1].c_str(), "corps_delcity" ) == 0 )
			{
				tChangeNotify.set_type( CMD_CORPS_DELCITY );
			}
			else if ( strcmp( tToken[1].c_str(), "corps_money" ) == 0 )
			{				
				tChangeNotify.set_type( CMD_CORPS_MONEY );				
			}
			else if ( strcmp( tToken[1].c_str(), "family_cha_stat" ) == 0 )
			{				
				tChangeNotify.set_type( CMD_FAMILY_CHASTATUS );				
			}
			else if( strcmp( tToken[1].c_str(), "family_war_stat" ) == 0 )
			{
				tChangeNotify.set_type( CMD_FAMILY_CHASTATUS );
			}
			else if( strcmp( tToken[1].c_str(), "corps_cha_stat" ) == 0 )
			{
				tChangeNotify.set_type( CMD_CORPS_CHASTATUS );
			}

			tChangeNotify.set_name( tToken[2].c_str() );					// 名字
			tChangeNotify.set_param1( ::atoi( tToken[3].c_str() ) );		// 修改数值

			if ( tChangeNotify.type() != 0 )
			{
				CSceneLogic::GetSingletonPtr()->Send2Gate( &tMsg );
				break;
			}			

			if( tToken.size() < 5 ) return;

			if ( strcmp( tToken[1].c_str(), "world_power" ) == 0 )
			{
				tChangeNotify.set_type( CMD_WORLD_POWER );				
			}
			else if ( strcmp( tToken[1].c_str(), "world_corpsid" ) == 0 )
			{
				tChangeNotify.set_type( CMD_WORLD_CORPSID );
			}
			else if ( strcmp( tToken[1].c_str(), "world_people" ) == 0 )
			{
				tChangeNotify.set_type( CMD_WORLD_PEOPLE );
			}
			
			tChangeNotify.set_param1( ::atoi( tToken[2].c_str() ) );		// 阵营ID
			tChangeNotify.set_param2( ::atoi( tToken[3].c_str() ) );		// 国家/地图ID
			tChangeNotify.set_param3( ::atoi( tToken[4].c_str() ) );		// 修改数值

			if ( tChangeNotify.type() != 0 )
			{
				CSceneLogic::GetSingletonPtr()->Send2Gate( &tMsg );
				break;
			}	
			
			if( tToken.size() < 6 ) return;

			if ( strcmp( tToken[1].c_str(), "world_wizard" ) == 0 )
			{
				tChangeNotify.set_type( CMD_WORLD_WIZARD );
			}
			else if ( strcmp( tToken[1].c_str(), "world_convalue" ) == 0 )
			{
				tChangeNotify.set_type( CMD_WORLD_CONVALUE );
			}
			else if ( strcmp( tToken[1].c_str(), "world_conwizard" ) == 0 )
			{
				tChangeNotify.set_type( CMD_WORLD_CONWIZARD );
			}			
			else if ( strcmp( tToken[1].c_str(), "world_clear" ) == 0 )
			{
				tChangeNotify.set_type( CMD_WORLD_CLEAR );
			}			
			
			tChangeNotify.set_param4( ::atoi( tToken[5].c_str() ) );		// 修改数值		
			
			CSceneLogic::GetSingletonPtr()->Send2Gate( &tMsg );
			break;
		}
		case em_priv_fb:
		{
			if( tToken.size() < 4 ) return;

			CRepetionModule::GetSingletonPtr( )->PlayerSingleIntoRepe( pPlayer, atoi( tToken[ 1 ].c_str( ) ),
				atoi( tToken[ 2 ].c_str( ) ), atoi( tToken[ 3 ].c_str( ) ) );
			break;
		}
		case em_priv_clear_safe_pwd:
		{
			if( tToken.size() < 2 ) return;
			
			CEntityPlayer* pPlayer = SceneServer::CSceneLogic::GetSingletonPtr()->GetPlayerByName( tToken[1].c_str() );
			if( pPlayer != NULL )
			{
				CPropertyPlayer* pPlayerPro = (CPropertyPlayer*)pPlayer->GetProperty();
				pPlayerPro->GetRolePassword()->SetPassword( "\0" );
			}
			break;
		}
		case em_priv_create_npc:
		{
			if( tToken.size() < 2 ) return;	
			CCreator* pNpcCreator = CCreatorManager::GetSingletonPtr()->GetCreator( atoi(tToken[1].c_str())  );
			if( pNpcCreator == NULL )
			{
				return;
			}
			
			bool bInit = false;
			int nNpcID = 0;
			CWTPoint nPoint = CWTPoint(0,0);
			bool bDelay = false;
			
			if( tToken.size() >= 3 )
				bInit = atoi(tToken[2].c_str()) ? true : false;
			if( tToken.size() >= 4 )
				nNpcID = atoi(tToken[3].c_str());
			if( tToken.size() >= 6 )
				nPoint = CWTPoint( atoi(tToken[4].c_str()), atoi(tToken[5].c_str()) );
			if( tToken.size() >= 7 )
				bDelay = atoi(tToken[6].c_str()) ? true : false;

			CMapObject* pMapObj = pPlayer->GetMapObject();
			if( pMapObj == NULL )
			{
				return ;
			}

			if( pNpcCreator->GetCreatorType() == ENTITYTYPE_FUNCNPC || pNpcCreator->GetCreatorType() == ENTITYTYPE_OGRE )
			{
				// 销毁先前的entity
				COgreCreator* pOgreCreator = (COgreCreator*)pNpcCreator;
				if( pOgreCreator->mHasOgre > 0 )
				{
					CEntity* pOldEntity = CSceneLogic::GetSingletonPtr()->GetEntity( pOgreCreator->mHasOgre );
					CSceneLogic::GetSingletonPtr()->DestroyEntity( pOldEntity, 0 );
				}
			}

			pMapObj->CreateNpc( pNpcCreator, bInit, nNpcID, nPoint, bDelay );
			break;
		}
		case em_priv_destroy_npc:
		{
			if( tToken.size() < 2 ) return;	
			CCreator* pNpcCreator = CCreatorManager::GetSingletonPtr()->GetCreator( atoi(tToken[1].c_str())  );
			if( pNpcCreator == NULL )
			{
				return;
			}
			if( pNpcCreator->GetCreatorType() == ENTITYTYPE_FUNCNPC || pNpcCreator->GetCreatorType() == ENTITYTYPE_OGRE )
			{
				// 销毁先前的entity
				COgreCreator* pOgreCreator = (COgreCreator*)pNpcCreator;
				if( pOgreCreator->mHasOgre > 0 )
				{
					CEntity* pOldEntity = CSceneLogic::GetSingletonPtr()->GetEntity( pOgreCreator->mHasOgre );
					CSceneLogic::GetSingletonPtr()->DestroyEntity( pOldEntity, 0 );
				}
			}
			break;
		}

		case em_priv_reload_map:
		{
			if( tToken.size() < 3 )
				return;
			unsigned short usMapID = (unsigned short)( atoi(tToken[1].c_str()) );
			unsigned short usLineID = (unsigned short)(atoi(tToken[2].c_str()));

			CMapObject* pMapObj = CMapObjManager::GetSingletonPtr()->GetMapObject( usLineID, usMapID, INVALID_MAP_ID );
			if( pMapObj == NULL  )
			{
				return;
			}

			CMapTpl* pMapTpl = 
				CMapTplManager::GetSingletonPtr()->CreateMapTpl( tToken[1].c_str(), usMapID, usLineID, true );
			if( pMapTpl != NULL )
			{
				pMapObj->CreateAllEntity();
			}


			break;
		}

		case em_priv_show_npc_info:
		{
			if( tToken.size() < 2 )
				return ;
			
			CEntity* pEntity = CSceneLogic::GetSingletonPtr()->GetEntity( atoi(tToken[1].c_str()) );
			if( pEntity == NULL )
			{
				return;
			}

			CCreator* pCreator = (CCreator*) CSceneObjManager::GetSingletonPtr()->GetObject( pEntity->GetCreatorID() );
			if( pCreator == NULL )
			{
				return;
			}

			CMessage tMessage;
			CMessageChatSendCallBack tChatSendMsg;
			tChatSendMsg.set_channel( CHANNEL_P2P );
			char tMsg[1024] = {0};

			int nLen = snprintf( tMsg, sizeof(tMsg)-1, "EntityID:%d CreatorIdx:%d Map:%d Pos(%d,%d) Alive %d ", 
				pEntity->GetEntityID(), pCreator->GetIndex(),  pEntity->GetMapID(), 
				pEntity->GetPosX(), pEntity->GetPosY(),pEntity->GetAliveStatus() );

			if( pEntity->IsNpc() )
			{
				CEntityNpc* pEntityNpc = static_cast<CEntityNpc*>(pEntity);

				CNpcPolicy* tpPolicy = (CNpcPolicy*)CSceneObjManager::GetSingletonPtr()->GetObject( pEntityNpc->GetPolicyID() );
				LK_ASSERT( tpPolicy != NULL , return );

				snprintf( tMsg+nLen, sizeof(tMsg)-1-nLen, 
					"NpcID %d OccurTimer %d OccurTime %d NpcChangeState %d  SendNpcState %d\n "
					"Policy PathPos(%d,%d) TarPos(%d,%d) State %d Dead %d",
					pEntityNpc->GetNpcID(), pEntityNpc->GetOccurTimer(), pEntityNpc->GetOccurTime(),pEntityNpc->GetNpcChangeState(), pEntityNpc->GetSendNpcState(),
					tpPolicy->mPathPos.mX, tpPolicy->mPathPos.mY, tpPolicy->mTargetPos.mX, tpPolicy->mTargetPos.mY, tpPolicy->mNpcState, tpPolicy->mIsDead
					);
			}
			tChatSendMsg.set_msg( tMsg );
			tMessage.mutable_msghead()->set_messageid( ID_S2C_CHATSENDCALLBACK );
			tMessage.set_msgpara( (uint32)&tChatSendMsg );
			CSceneLogic::GetSingletonPtr()->Send2Player(  pPlayer, &tMessage );

			break;
		}

		// 军团金钱回滚处理
		case em_priv_callback_corpsmoney:
			{
				if( tToken.size() < 3 )
					return ;

				int tCorpsID = atoi( tToken[1].c_str() );
				int tNpcID	= atoi( tToken[2].c_str() );
				
				CMessage tMessage;
				CMessageRollCorpsMoneyRequest tPara;
				tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_CALLBACK_CORPSMONEY ); 
				tMessage.set_msgpara( (long) &tPara );

				if ( tCorpsID == 0 && tNpcID == 0 )
				{
					tPara.set_isall( (int) true ); 
				}
				else
				{
					tPara.set_isall( (int) false );
					tPara.set_corpsid( tCorpsID );
					tPara.set_npcid( tNpcID ); 
				}
				
				CSceneLogic::GetSingleton().Send2Gate( &tMessage );
				break;
			}
		case em_priv_change_repetion:
			{
				if( tToken.size() != 3 )
				{
					return;
				}

				CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByName( tToken[ 1 ].c_str( ) );
				if( tpPlayer != NULL )
				{
					CRepetionModule::GetSingletonPtr()->DelteRepetionCount( tpPlayer, atoi( tToken[ 2 ].c_str( ) ) );
				}
				
				break;
			}
		case em_priv_clean_all_repetion:
			{
				break;
			}
		case em_priv_battle:
			{
				if( tToken.size() < 3 )
				{
					return;
				}
				
				int tNpcID = atoi( tToken[ 1 ].c_str() );
				int tNpcBattleType = atoi( tToken[ 2 ].c_str() );

				// 内部使用的枚举
				enum
				{
					_BATTLE_FAMILY_BIDTIMEOUT_ = 1,
					_BATTLE_FAMILY_START_ = 2,
					_BATTLE_CORPS_BIDTIMEOUT_ = 3,
					_BATTLE_CORPS_START_ = 4,
				};

				CTemplateNpc* tpNpc = (CTemplateNpc*) CDataStatic::SearchTpl( tNpcID );
				if ( tpNpc == NULL )
				{
					return;
				}

				switch( tNpcBattleType )
				{
				case _BATTLE_CORPS_BIDTIMEOUT_:
					{
						// 军团战竞价结束
						CMessage tMessage;
						CMessageCorpsBidTimerStopRequest tPara;
						tPara.set_npcid( tNpcID );
						tPara.set_battleid( tpNpc->mBattleCity );
						tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_BIDTIMERSTOP );
						tMessage.set_msgpara( (long) &tPara );
						CSceneLogic::GetSingleton().Send2Gate( &tMessage );
						break;
					}
				case _BATTLE_CORPS_START_:
					{
						// 军团战开战
						CMessage tMessage;
						CMessageCorpsStartBattleRequest tPara;
						tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_STARTCOPRSBATTLE );
						tMessage.set_msgpara( (long) &tPara );
						tPara.set_npcid( tNpcID );
						tPara.set_battleid( tpNpc->mBattleCity );
						CSceneLogic::GetSingleton().Send2Gate( &tMessage );
						break;
					}
				case _BATTLE_FAMILY_BIDTIMEOUT_:
					{
						// 家族战竞价结束
						CActivityModule::GetSingleton().DebugTimeOut( tNpcID );
						break;
					}
				case _BATTLE_FAMILY_START_:
					{
						// 家族战开战
						CActivityModule::GetSingleton().DebugBattleStart( tNpcID );
						break;
					}
				default:
					{
						break;
					}
				}
			}
			break;
		case em_priv_tel:
			{
				if( tToken.size( ) != 2 )
				{
					return;
				}
				CMessageS2GRoleMove tMove;
				tMove.set_srcrole( pPlayer->GetCharName( ) );
				tMove.set_desrole( tToken[ 1 ].c_str( ) );

				CMessage tMessage;	

                tMessage.mutable_msghead()->set_messageid( ID_S2G_ROLE_MOVE );
                tMessage.set_msgpara( (long) &tMove );

				CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMessage );
				
				break;
			}
			
		case em_priv_war:
			{
				if( tToken.size() < 6 )
				{
					return;
				}
				int tState = atoi( tToken[1].c_str() );
				int tDay = atoi( tToken[2].c_str() );
				int tHour = atoi( tToken[3].c_str() );
				int tMinute = atoi( tToken[4].c_str() );
				int tSerial = atoi( tToken[5].c_str() );
				CWorldModule::GetSingleton().SendWarControlNotify( tState, tDay, tHour, tMinute, tSerial );
				break;
			}
		case em_priv_change_camp:
			{
				if ( tToken.size() < 3)
				{
					return;
				}
				int tcampid  = atoi(tToken[2].c_str());
				CSceneServer::GetSingleton().OnGMMessageChangeCamp(tToken[1].c_str(),tcampid);

				break;
			}
		default:
		{
			break;
		}
	}
}

void CGMModule::OnMessageMinusActivityItem(CEntityPlayer* tpPlayer, CMessage* tpMessage)
{
	LK_ASSERT(tpPlayer != NULL && tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageMinusActivityItemRequest* tpMinusRequest = (CMessageMinusActivityItemRequest*) tpMessage->msgpara();

	CActItemBuffer::iterator iter4m = mActItemBuffer.find(tpPlayer->GetCharID());
	if ( iter4m == mActItemBuffer.end()) return;

	iter4m->second.clear();
	mActItemBuffer.erase(iter4m);

	PBItemIDCount* pbActvityItem = tpMinusRequest->mutable_item();
	if ( pbActvityItem == NULL) return;

	CTemplateLevelMoneyLmt* tpMoneyTable = (CTemplateLevelMoneyLmt*)CDataStatic::GetTemp(TEMP_MONEYLMT_CFG);
	LK_ASSERT( tpMoneyTable!=NULL, return );
	LK_ASSERT( tpPlayer->GetLevel() >= 0 && tpPlayer->GetLevel() < TMP_MAX_GRADE, return );

	const char* strItemCode = pbActvityItem->itemcode().c_str();
	if ( memcmp(strItemCode, "i-", 2) == 0)  // 特殊符号
	{
		int nItemID[ BAGGAGE_BASE ]           = { INVALID_ITEM_ID };
		int nItemNumber[ BAGGAGE_BASE ]       = { 0 };
		int nItems = 0;

		nItemID [0] = atoi(&strItemCode[2]);
		nItemNumber[0] = pbActvityItem->itemcount();
		nItems = 1;

		// 检查玩家包裹空间
		if ( ((CPropertyPlayer*) (tpPlayer->GetProperty()))->TestBaggageSpace( nItemID, nItemNumber, nItems) == false)
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( tpPlayer, ERROR_INSERTITEM_NOSPACE );	
			LOG_NOTICE("gm", "gm extend items number(%d) is bigger than player(%d)", nItems, tpPlayer->GetCharID());
			return;
		}
	}
	else if (strcmp(strItemCode, "s-money-0") == 0) //  未绑定的游戏币
	{
		// 判断钱是否溢出
		if ( tpPlayer->GetMoney()+pbActvityItem->itemcount() > tpMoneyTable->mLmtTable[tpPlayer->GetLevel()].mBagMoneyLmt )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice(tpPlayer, ERR_BAG_MONEY_UPLMT);
			return;
		}
	}
	else if ( strcmp(strItemCode, "s-money-1") == 0 )// 绑定的游戏币
	{
		// 判断钱是否溢出
		if ( tpPlayer->GetBindMoney()+pbActvityItem->itemcount() > tpMoneyTable->mLmtTable[tpPlayer->GetLevel()].mBagBindMoneyLmt )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice(tpPlayer, ERR_BAG_BIND_MONEY_UPLMT);
			return;
		}
	}
	else if ( strcmp(strItemCode, "s-silver-1") != 0) // 已绑定的银元宝
	{
		LOG_ERROR("gm", "There is an error for item code about %s for player(%d)", 
				tpMinusRequest->ShortDebugString().c_str(), tpPlayer->GetCharID());

		CPropertyModule::GetSingleton().SendProErrorNotice( tpPlayer, ERROR_ITEM_DATANOTEXIST );	
		return;
	}

	if ( pbActvityItem->itemcount() <= 0)
	{
		LOG_ERROR("gm", "There is an error for item number about %s for player(%d)", 
				tpMinusRequest->ShortDebugString().c_str(), tpPlayer->GetCharID());

		return;
	}

	// 不再检查玩家是否有此物品，直接将请求转发至ERATING
	// 不再支持自适应礼品包
	CMessageUserItemMinusRequest sndmsg;

	sndmsg.set_roleid(tpPlayer->GetCharID());
	sndmsg.set_accountid(tpPlayer->GetAccountID());
	sndmsg.set_activityid(pbActvityItem->activityid());
	sndmsg.set_itemcode(strItemCode);
	sndmsg.set_itemnum(pbActvityItem->itemcount());
	sndmsg.set_rolelevel(tpPlayer->GetLevel());
	sndmsg.set_rolemetier(tpPlayer->GetMetier());

	CMessage tMsg;
	tMsg.set_msgpara((uint32_t) &sndmsg);
	pbmsg_sethead(&tMsg, ID_S2E_USERITEMMINUS_REQUEST, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingletonPtr()->Send2Gate( &tMsg );
}

void CGMModule::OnGmMessageMultiExpCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageMultiExpCmd* tpFactMsg = (CGMMessageMultiExpCmd*) tpMessage->msgpara();

	time_t seconds = tpFactMsg->start_time();
	struct tm* pnow = localtime(&seconds);

	char tpDate[32] = {0};
	strftime(tpDate,  sizeof(tpDate) - 1, "%Y%m%d%H%M%S", pnow);

	double nMulti = atof(tpFactMsg->multi_num().c_str()); 
	int nRate = nMulti > 1 ? ( nMulti - 1 ) * SERVER_PERCENT_INT : 0;

	if ( CActivityModule::GetSingleton().AppendDoubleTime(tpFactMsg->start_time(), tpFactMsg->end_time(), nRate))
	{
		LOG_NOTICE("gm", "GM Command MultiExp %s Add from %s for %d seconds OK", 
				tpFactMsg->multi_num().c_str(), tpDate, tpFactMsg->end_time()-tpFactMsg->start_time());

		CActivityModule::GetSingleton().CheckDoubleTime();
	}else
	{
		LOG_ERROR("gm", "GM Command MultiExp %s Add from %s for %d seconds Fail", 
				tpFactMsg->multi_num().c_str(), tpDate, tpFactMsg->end_time()-tpFactMsg->start_time());
	}
}

void CGMModule::OnGmMessageCancelMultiExpCmd(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageCancelMultiExpCmd* tpFactMsg = (CGMMessageCancelMultiExpCmd*) tpMessage->msgpara();

	time_t seconds = tpFactMsg->start_time();
	struct tm* pnow = localtime(&seconds);

	char tpDate[32] = {0};
	strftime(tpDate,  sizeof(tpDate) - 1, "%Y%m%d%H%M%S", pnow);

	if ( CActivityModule::GetSingleton().RemoveDoubleTime(tpFactMsg->start_time(), tpFactMsg->end_time()))
	{
		LOG_NOTICE("gm", "GM Command CancelMultiExp removed from %s for %d seconds OK", 
				tpDate, tpFactMsg->end_time()-tpFactMsg->start_time());
	}else
	{
		LOG_ERROR("gm", "GM Command CancelMultiExp removed from %s for %d seconds Fail", 
				tpDate, tpFactMsg->end_time()-tpFactMsg->start_time());
	}
}


void CGMModule::SendUseGmCommandResult( CEntityPlayer* pPlayer, const char* pMsg )
{
	LK_ASSERT( pMsg != NULL, return );

	CMessage tMessage;
	CMessageChatSendCallBack tChatSendMsg;
	tChatSendMsg.set_channel( CHANNEL_P2P );
	tChatSendMsg.set_msg( pMsg );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_CHATSENDCALLBACK );
	tMessage.set_msgpara( (uint32)&tChatSendMsg );
	CSceneLogic::GetSingletonPtr()->Send2Player(  pPlayer, &tMessage );
}

