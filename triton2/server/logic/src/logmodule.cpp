#include "logmodule.h"
#include "scenelogic.h"
#include "servermessage_in.h"
#include "message_maker.h"
#include "mapobjmanager.h"
#include "scenecfg_manager.h"
#include "npcmodule.h"

int g_exMoney;

/*
int CStatInfo::ItemCatalogID(int nOpType, int nItemID)
{
	//TODO: 读表
	return nItemID%em_item_catalog_number;
}

int CStatInfo::MoneyCatalogID(int nOpType, int nMoneyType)
{
	//TODO: 
	return nMoneyType*100+nOpType;
}

int CStatInfo::ExpCatalogID(int nOpType, int nExpType)
{
	//TODO: 
	return nExpType*100 + nOpType;
}

int CStatInfo::EventCatalogID(int nEventID)
{
	//TODO: 
	return nEventID;
}

void CStatInfo::LoadStatInfo()
{
	const char* stmtsql = "SELECT catalog_type, catalog_id, catalog_value + delta_value FROM STAT_CATALOG_DETAIL"
		" WHERE server_id = %d";
	int nServerID = CSceneServer::GetSingleton().mThisServerID;

	CSceneLogic::GetSingleton().ExecuteSql(
			em_dbsession_load_stat, 0, 0, nServerID, SQL_NEED_CALLBACK, SELECT, stmtsql, nServerID);
}

bool CStatInfo::OnLoadStatInfo(CMessage* pMessage)
{
	LK_ASSERT(pMessage != NULL && pMessage->msgpara() != 0, return false);
	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*)pMessage->msgpara();

	bool bLoadOK = true;

	if ( pSqlResult->resultcode() == 0)		
	{
		LOG_ERROR("default", "load statinfo error");
		return false;
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
	{
		int nCatalogType = atoi(pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str());
		int nCatalogID = atoi(pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str());
		int64_t nCatalogValue = atoll(pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str());

		switch (nCatalogType) 
		{
			case EM_SERVER_MONEY:
			{
				std::pair<money_catalog_t::iterator, bool> ret =  
					mDeltaMoney.insert(money_catalog_t::value_type(nCatalogID, value_pair_t(nCatalogValue, 0)));

				bLoadOK &= ret.second;
				break;
			}

			case EM_SERVER_EXP:
			{
				std::pair<exp_catalog_t::iterator, bool> ret =  
					mDeltaExp.insert(exp_catalog_t::value_type(nCatalogID, value_pair_t(nCatalogValue, 0)));

				bLoadOK &= ret.second;
				break;
			}

			case EM_SERVER_ITEM:
			{
				std::pair<item_catalog_t::iterator, bool> ret =  
					mDeltaItems.insert(item_catalog_t::value_type(nCatalogID, value_pair_t(nCatalogValue, 0)));

				bLoadOK &= ret.second;
				break;
			}

			case EM_SERVER_EVENT:
			{
				std::pair<event_catalog_t::iterator, bool> ret =  
					mDeltaEvents.insert(event_catalog_t::value_type(nCatalogID, value_pair_t(nCatalogValue, 0)));

				bLoadOK &= ret.second;
				break;
			}

			default:
			{
				break;
			}
		}
	}

	return bLoadOK;
}

void CStatInfo::SaveStatInfo()
{
	lk::string<0x7fff> stmtins("");

	int nServerID = CSceneServer::GetSingleton().mThisServerID;

	money_catalog_t::iterator iter4money = mDeltaMoney.begin();
	for(; iter4money != mDeltaMoney.end(); ++iter4money)
	{
		if (stmtins.length() == 0)
		{
			// server_id, catlog_type, catalog_id, catalog_value, deltavalue
			stmtins.append("INSERT INTO STAT_CATALOG_DETAIL VALUES (%d,%d,%d,%lld,%lld)",
					nServerID, EM_SERVER_MONEY, 
					iter4money->first, iter4money->second.first, iter4money->second.second);
		}else
		{
			stmtins.append(",(%d,%d,%d,%lld,%lld)",
					nServerID, EM_SERVER_MONEY, 
					iter4money->first, iter4money->second.first, iter4money->second.second);
		}
	}   

	exp_catalog_t::iterator iter4exp = mDeltaExp.begin();
	for(; iter4exp != mDeltaExp.end(); ++iter4exp)
	{
		if (stmtins.length() == 0)
		{
			// server_id, catlog_type, catalog_id, catalog_value, deltavalue
			stmtins.append("INSERT INTO STAT_CATALOG_DETAIL VALUES (%d,%d,%d,%lld,%lld)",
					nServerID, EM_SERVER_EXP, 
					iter4exp->first, iter4exp->second.first, iter4exp->second.second);
		}else
		{
			stmtins.append(",(%d,%d,%d,%lld,%lld)",
					nServerID, EM_SERVER_EXP, 
					iter4exp->first, iter4exp->second.first, iter4exp->second.second);
		}
	}   

	item_catalog_t::iterator iter4item = mDeltaItems.begin();
	for(; iter4item != mDeltaItems.end(); ++iter4item)
	{
		if (stmtins.length() == 0)
		{
			// server_id, catlog_type, catalog_id, catalog_value, deltavalue
			stmtins.append("INSERT INTO STAT_CATALOG_DETAIL VALUES (%d,%d,%d,%lld,%lld)",
					nServerID, EM_SERVER_ITEM, 
					iter4item->first, iter4item->second.first, iter4item->second.second);
		}else
		{
			stmtins.append(",(%d,%d,%d,%lld,%lld)",
					nServerID, EM_SERVER_ITEM, 
					iter4item->first, iter4item->second.first, iter4item->second.second);
		}
	}

	if (stmtins.length() > 0)
	{
		static const char* stmtdel = "DELETE FROM STAT_CATALOG_DETAIL WHERE server_id = %d"; 

		CSceneLogic::GetSingleton().ExecuteSql(
			em_dbsession_save_stat, 0, 0, nServerID, SQL_NONE, DELETE, stmtdel, nServerID);

		CSceneLogic::GetSingleton().ExecuteSql(
			em_dbsession_save_stat, 0, 0, nServerID, SQL_NONE, INSERT, stmtins.c_str());
	}
}

void CStatInfo::LogStatInfo()
{
	CMessageServerStatInfoNotify proto;	

	// 金钱
	PBCatalog* pCatalog = proto.add_statinfo();
	PBAttribute *pAttr = NULL;
	if ( pCatalog == NULL ) return;		
	pCatalog->set_id(EM_SERVER_MONEY);

	money_catalog_t::iterator iterMoney = mDeltaMoney.begin();	
	for( ;iterMoney != mDeltaMoney.end(); ++iterMoney)
	{
		if((pAttr = pCatalog->add_attr()) != NULL)
		{
			pAttr->set_id (iterMoney->first);
			pAttr->set_value(iterMoney->second.second);
		}
	}

	// 经验
	pCatalog = proto.add_statinfo();
	if ( pCatalog == NULL ) return;		
	pCatalog->set_id(EM_SERVER_EXP);

	exp_catalog_t::iterator iterExp = mDeltaExp.begin();	
	for( ;iterExp != mDeltaExp.end(); ++iterExp)
	{
		if((pAttr = pCatalog->add_attr()) != NULL)
		{
			pAttr->set_id (iterExp->first);
			pAttr->set_value(iterExp->second.second);
		}
	}

	// 物品
	pCatalog = proto.add_statinfo();
	if ( pCatalog == NULL ) return;		
	pCatalog->set_id(EM_SERVER_ITEM);

	item_catalog_t::iterator iterItem = mDeltaItems.begin();	
	for( ;iterItem != mDeltaItems.end(); ++iterItem)
	{
		if((pAttr = pCatalog->add_attr()) != NULL)
		{
			pAttr->set_id (iterItem->first);
			pAttr->set_value(iterItem->second.second);
		}
	}

	// 行为
	pCatalog = proto.add_statinfo();
	if ( pCatalog == NULL ) return;		
	pCatalog->set_id(EM_SERVER_EVENT);

	event_catalog_t::iterator iterEvent = mDeltaEvents.begin();	
	for( ;iterEvent != mDeltaEvents.end(); ++iterEvent)
	{
		if((pAttr = pCatalog->add_attr()) != NULL)
		{
			pAttr->set_id (iterEvent->first);
			pAttr->set_value(iterEvent->second.second);
		}
	}

	// 向网关服务器发送
	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &proto);
	pbmsg_sethead(&tMessage, ID_S2L_SERVER_STAT_INFO_NOTICE, 0, 0, 0, 0, 0, 0);
				
	// 向网关发送
	CSceneLogic::GetSingletonPtr( )->Send2Log( &tMessage, tMessage.msghead().messageid(), EM_SERVER_EVENT);
}
*/

int GetMapID4Log( CEntity* pEntity )
{
	if (pEntity == NULL) return 0;
#ifdef _FB_
	CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfoByMapID( pEntity->GetMapID( ) );
	if( tpInfo != NULL && tpInfo->GetRepetionIndex( ) == _REPE_TOWNER_INDEX_ )
	{
		CMapObject* tpObj = pEntity->GetMapObject( );
		return ( tpInfo->GetRepetionIndex( ) * 10000 + ( ( tpObj != NULL ) ? tpObj->GetMapData( 0 ) : 0 ) );
	}

	return pEntity->GetMapID();
#else
	return pEntity->GetMapID();
#endif
}

void LogEventLogoutScene(CEntityPlayer* tpPlayer, EMLeaveReason emLeave, EMSaveResult emSave)
{
	if ( tpPlayer == NULL) return;

	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_role_logout == 0) return;

	CMessageLogRoleLogout event;
	time_t nNow = time(NULL);

	event.set_roleid(tpPlayer->GetCharID());
	event.set_optime(nNow);
	event.set_rolelevel(tpPlayer->GetLevel());
	event.set_metierid(tpPlayer->GetMetier());
	event.set_repetionid(tpPlayer->GetMapObject() ? tpPlayer->GetMapObject()->GetKey() : INVITED_KEY_VALUE);
	event.set_lineid(tpPlayer->GetLineID());
	event.set_mapid( GetMapID4Log(tpPlayer));

	event.set_loginip(tpPlayer->GetClientInfo()->mClientAddr);
	event.set_logintime(tpPlayer->GetClientInfo()->mSockTime);
	event.set_logouttime(nNow);
	event.set_savereason(tpPlayer->GetCommState());
	event.set_saveresult(emSave);
	event.set_leavereason(emLeave);
	event.set_kickreason(tpPlayer->GetKickoutReason());
	event.set_kickcode(tpPlayer->GetKickoutCode());


	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &event);
	pbmsg_sethead(&tMessage, ID_S2L_ROLE_LOGOUT, 0, 0, 0, 0, 0, 0);

	//参数2: 第几个线程
	//参数3: 按照什么来分表
	CSceneLogic::GetSingletonPtr( )->Send2Log( &tMessage, tMessage.msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
}

void LogEventMoneyAction( CEntityPlayer* tpPlayer, int nMoneyType, int nMoneyNumber, 
		EMoneyAction emAction, int nParam1 , int nParam2 , uint64_t  nParam3 )
{
	if ( tpPlayer == NULL || nMoneyNumber == 0 ) return;

	//CSceneLogic::GetSingleton().mStatInfo.CatalogMoney(emAction, nMoneyType, nMoneyNumber);

	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_role_money == 0) return;

	if ( g_exMoney != 0 && nMoneyType == CYuanBao::em_bind_money )
	{
		CMessage* pMessage = BuildPlayerOpMoneyLogMessage(tpPlayer, nMoneyType, nMoneyNumber - g_exMoney, 
				emAction, nParam1, nParam2, nParam3);
		if ( pMessage != NULL)
		{
			CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
		}

		// record g_exMoney 
		CMessage* pMessage2 = BuildPlayerOpMoneyLogMessage(tpPlayer, CYuanBao::em_unbind_money, g_exMoney, 
				emAction, nParam1, nParam2, nParam3);
		if ( pMessage2 != NULL)
		{
			CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage2, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
		}

		// clear g_exMoney
		g_exMoney = 0;

		return;
	}

	CMessage* pMessage = BuildPlayerOpMoneyLogMessage(tpPlayer, nMoneyType, nMoneyNumber - g_exMoney, 
			emAction, nParam1, nParam2, nParam3);
	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
	}
}

void LogEventItemAction( CEntityPlayer* tpPlayer,
			uint64_t ullGuID, int32_t nItemID, int32_t nItemNum, EItemAction emAction,
			int32_t  nParam1 , int32_t  nParam2 , uint64_t  nParam3 )
{
	if (tpPlayer == NULL) return;

	//CSceneLogic::GetSingleton().mStatInfo.CatalogItem(emAction, nItemID, nItemNum);
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_role_item == 0) return;


	int nTotal = ((CPropertyPlayer*) tpPlayer->GetProperty())->GetBaggage()->GetItemTotalCount(nItemID) 
					+ ((CPropertyPlayer*) tpPlayer->GetProperty())->GetStorage()->GetItemTotalCount(nItemID) ;

	CMessage* pMessage = BuildPlayerOpItemLogMessage(tpPlayer,
			ullGuID, nItemID, nItemNum, nTotal, emAction, nParam1, nParam2, nParam3);
	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
	}
}

void LogEventRoleExpAction( CEntityPlayer* tpPlayer, int emAction, int nExpValue, int nParam1, int nParam2, uint64_t nParam3)
{
	if (tpPlayer == NULL) return;

	//CSceneLogic::GetSingleton().mStatInfo.CatalogExp(emAction, EM_UPGRADE_ROLE_EXP, nExpValue);
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_role_exp == 0) return;

	// 只给杀怪增加一层经验数值过滤
	if( emAction == EM_EXP_BY_ENTITY_DIE )
	{
		if ( nExpValue < (int32_t) CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mRole_exp_limit) 
			return;
	}

	CMessage* pMessage = BuildPlayerOpExpLogMessage(tpPlayer, 
			EM_UPGRADE_ROLE_EXP, tpPlayer->GetExp(), nExpValue, emAction, nParam1, nParam2, nParam3);

	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
	}
}

void LogEventTalentExpAction( CEntityPlayer* tpPlayer, int emAction, int nExpValue)
{
	if (tpPlayer == NULL) return;

	//CSceneLogic::GetSingleton().mStatInfo.CatalogExp(emAction, EM_UPGRADE_TALENT_EXP, nExpValue);
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_talent_exp == 0) return;
	if ( nExpValue < (int32_t) CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mTalent_exp_limit) return;

	CPropertyPlayer* pProperty = (CPropertyPlayer*) tpPlayer->GetProperty();

	CMessage* pMessage = BuildPlayerOpExpLogMessage(tpPlayer, EM_UPGRADE_TALENT_EXP, 
			pProperty->GetTalentExp(), nExpValue, emAction, pProperty->GetTalentPoint(), pProperty->GetTalentStep(), pProperty->GetTalentStudy());

	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
	}
}

void LogEventMWExpAction( CEntityPlayer* tpPlayer, CItemMagicWeapon* tpMW, int emAction, int nExpValue)
{
	if (tpPlayer == NULL || tpMW == NULL) return;

	//CSceneLogic::GetSingleton().mStatInfo.CatalogExp(emAction, EM_UPGRADE_MW_EXP, nExpValue);
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_mw_exp == 0) return;
	if ( nExpValue < (int32_t) CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mMw_exp_limit) return;

	CMessage* pMessage = BuildPlayerOpExpLogMessage(tpPlayer, EM_UPGRADE_MW_EXP, 
			tpMW->GetExp(), nExpValue, emAction, tpMW->GetItemID(), tpMW->GetLevel(), tpMW->GetGUID());

	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
	}
}

void LogEventProduceExpAction( CEntityPlayer* tpPlayer, CItemObject * pItemObject, int emAction, int nExpValue)
{
	if (tpPlayer == NULL || pItemObject == NULL) return;

	//CSceneLogic::GetSingleton().mStatInfo.CatalogExp(emAction, EM_UPGRADE_PRODUCE_EXP, nExpValue);
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_produce_exp == 0) return;

	CPropertyPlayer* pProperty = (CPropertyPlayer*) tpPlayer->GetProperty();

	CMessage* pMessage = BuildPlayerOpExpLogMessage(tpPlayer, EM_UPGRADE_PRODUCE_EXP, 
			pProperty->GetProExp(), nExpValue, emAction, pItemObject->GetItemID(), pProperty->GetProLevel(), pItemObject->GetGUID());

	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
	}
}


void LogEventPlayerDieAction( CEntityPlayer* tpPlayer, int nKillerType,  int nKillerID, int nKillerMetier, int nKillerLevel)
{
	if (tpPlayer == NULL) return;
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_role_killed == 0) return;

	CMessage* pMessage = BuildPlayerBeKilledLogMessage(tpPlayer, nKillerType, nKillerID, nKillerMetier, nKillerLevel);

	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
	}
}

void LogEventTaskAction( CEntityPlayer* tpPlayer,
		        int nTaskID, ETaskAction emAction, int nParam1, int nParam2, uint64_t ullParam3)
{
	if (tpPlayer == NULL) return;	
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_role_task == 0) return;

	CMessage* pMessage = BuildPlayerOpTaskLogMessage(tpPlayer, nTaskID, emAction, nParam1, nParam2, ullParam3);

	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
	}
}

void LogEventNpcDropAction(CEntityNpc* pDropingEntity, std::vector<CEntity*>& rDropedEntityVec)
{
	if (pDropingEntity == NULL) return;
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_npc_drop == 0) return;

	CMessage* pMessage = BuildNpcDropLogMessage(pDropingEntity, rDropedEntityVec);
	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), pDropingEntity->GetProperty()->GetPropertyTempID() );
	}
}

void LogEventUseItemDropAction( CEntityPlayer* pPlayer, int nItemID,  std::vector<CEntity*>& rDropEntityVec )
{
	if (nItemID < 0) return;
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_npc_drop == 0) return;

	CMessage* pMessage = BuildUseItemDropLogMessage( pPlayer, nItemID, rDropEntityVec );
	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), 0 );
	}
}


void LogEventPlayerLevelUpAction (CEntityPlayer* tpPlayer, EUpgradeType emUpgrade, int nSubType, int nCurrentLevel, int nLevel)
{
	if ( tpPlayer == NULL) return;
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_role_levelup == 0) return;
	
	CMessage* pMessage = BuildPlayerLevelUpLogMessage(tpPlayer, emUpgrade, nSubType, nCurrentLevel, nLevel);
	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
	}
}

void LogDropedItemDestroyAction( CEntity* pEntity)
{
	if ( pEntity == NULL) return;
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_item_dispear == 0) return;

	CMessage* pMessage = BuildDropedItemDestroyLogMessage( pEntity);
	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), pEntity->GetEntityType());
	}
}

void LogEventItemLevelUpAction(CEntityPlayer* tpPlayer, 
	int nItemID, uint64_t ullGUID, int nType, int nResult, int nCurrLevel, int nPrevLevel, int nItemID1, int nItemID2)
{
	if ( tpPlayer == NULL) return;
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_item_levelup == 0) return;

	CMessage* pMessage = BuildItemLevelUpLogMessage(tpPlayer,
			nItemID, ullGUID, nType, nResult, nCurrLevel, nPrevLevel, nItemID1, nItemID2);

	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
	}
}

void InitialMailLogMsg( CMessageLogMail& tMsgMailLog )
{	   
	tMsgMailLog.set_mailid( 0 );
	tMsgMailLog.set_mailtitle( "" );
	tMsgMailLog.set_mailtype( 0 );
	tMsgMailLog.set_optime( 0 );
	tMsgMailLog.set_optype( 0 );
	tMsgMailLog.set_roleid( 0 );
	tMsgMailLog.set_totalnum( 0 );	
	tMsgMailLog.set_itemid5( 0 );
	tMsgMailLog.set_itemnum5( 0 );	
	tMsgMailLog.set_itemid4( 0 );
	tMsgMailLog.set_itemnum4( 0 );
	tMsgMailLog.set_itemid3( 0 );
	tMsgMailLog.set_itemnum3( 0 );			
	tMsgMailLog.set_itemid2( 0 );
	tMsgMailLog.set_itemnum2( 0 );						
	tMsgMailLog.set_itemid1( 0 );
	tMsgMailLog.set_itemnum1( 0 );
	tMsgMailLog.set_money( 0 );
	tMsgMailLog.set_moneyneedcallback( 0 );
	tMsgMailLog.set_desroleid( 0 );
	tMsgMailLog.set_srcname( "" );
}

void LogEventPlayerSendMail(  int nRoleID, CMailCache *tpMail, int *pItemIDList, int *pItemNumList, int nItemNum, int nDesRoleID )
{	
	if ( tpMail == NULL )
	{
		return;
	}
	
	CMessage tMessage;
	CMessageLogMail tMsgMailLog;
	InitialMailLogMsg( tMsgMailLog );

	tMessage.set_msgpara( (int)&tMsgMailLog );
	tMessage.mutable_msghead()->set_messageid( ID_S2L_MAIL );

	tMsgMailLog.set_mailid( tpMail->mMailID );
	tMsgMailLog.set_mailtitle( tpMail->mMailTitle );
	
	tMsgMailLog.set_mailtype( tpMail->mLevel );
	tMsgMailLog.set_optime( time(NULL) );
	tMsgMailLog.set_optype( EM_MAIL_SENDMAIL );
	tMsgMailLog.set_roleid( nRoleID );
	tMsgMailLog.set_totalnum( 0 );
	tMsgMailLog.set_money( tpMail->mMoney );
	tMsgMailLog.set_moneyneedcallback( tpMail->mMoneyNeedPayBack );
	tMsgMailLog.set_desroleid( nDesRoleID );
	tMsgMailLog.set_srcname( tpMail->mSrcName );
	if ( pItemIDList == NULL || pItemNumList == NULL || nItemNum <= 0 )
	{	  
		CSceneLogic::GetSingletonPtr( )->Send2Log( &tMessage, tMessage.mutable_msghead()->messageid(), nRoleID);
		return;
	}

	switch( nItemNum )	
	{
	case 5:
		{
			tMsgMailLog.set_itemid5( pItemIDList[4] );
			tMsgMailLog.set_itemnum5( pItemNumList[4] );
		}

	case 4:
		{
			tMsgMailLog.set_itemid4( pItemIDList[3] );
			tMsgMailLog.set_itemnum4( pItemNumList[3] );
		}

	case 3:
		{
			tMsgMailLog.set_itemid3( pItemIDList[2] );
			tMsgMailLog.set_itemnum3( pItemNumList[2] );			
		}

	case 2:
		{
			tMsgMailLog.set_itemid2( pItemIDList[1] );
			tMsgMailLog.set_itemnum2( pItemNumList[1] );						
		}

	case 1:
		{
			tMsgMailLog.set_itemid1( pItemIDList[0] );
			tMsgMailLog.set_itemnum1( pItemNumList[0] );												
		}
	default:
		break;
	}	
	
	CSceneLogic::GetSingletonPtr( )->Send2Log( &tMessage, tMessage.mutable_msghead()->messageid(), nRoleID);	
}

void LogEventPlayerReceiveMail( int nRoleID, CMailCache *tpMail, int nTotalNum )
{	
	if ( tpMail == NULL )
	{	 
		return;
	}
	
	CMessage tMessage;
	CMessageLogMail tMsgMailLog;
	InitialMailLogMsg( tMsgMailLog );

	tMessage.set_msgpara( (int)&tMsgMailLog );
	tMessage.mutable_msghead()->set_messageid( ID_S2L_MAIL );
	
	tMsgMailLog.set_mailid( tpMail->mMailID );
	tMsgMailLog.set_mailtype( tpMail->mLevel );
	tMsgMailLog.set_optime( time(NULL) );
	tMsgMailLog.set_optype( EM_MAIL_RECEIVEMAIL );
	tMsgMailLog.set_roleid( nRoleID );
	tMsgMailLog.set_totalnum( nTotalNum );
	tMsgMailLog.set_money( tpMail->mMoney );
	tMsgMailLog.set_moneyneedcallback( tpMail->mMoneyNeedPayBack );
	tMsgMailLog.set_desroleid( tpMail->mSrcId );
	tMsgMailLog.set_srcname( tpMail->mSrcName );
	CSceneLogic::GetSingletonPtr( )->Send2Log( &tMessage, tMessage.mutable_msghead()->messageid(), nRoleID);
	return;	
}

void LogEventPlayerDeleteMail( int nRoleID, CMailCache *tpMail, int nTotalNum )
{	
	CMessage tMessage;
	CMessageLogMail tMsgMailLog;
	InitialMailLogMsg( tMsgMailLog );
	tMessage.set_msgpara( (int)&tMsgMailLog );
	tMessage.mutable_msghead()->set_messageid( ID_S2L_MAIL );

	tMsgMailLog.set_mailid( tpMail->mMailID );
	tMsgMailLog.set_mailtype( tpMail->mLevel );
	tMsgMailLog.set_optime( time(NULL) );
	tMsgMailLog.set_optype( EM_MAIL_DELMAIL );
	tMsgMailLog.set_roleid( nRoleID );
	tMsgMailLog.set_totalnum( nTotalNum );
	tMsgMailLog.set_money( tpMail->mMoney );
	tMsgMailLog.set_moneyneedcallback( tpMail->mMoneyNeedPayBack );
	tMsgMailLog.set_desroleid( tpMail->mSrcId );
	tMsgMailLog.set_srcname( tpMail->mSrcName );
	CSceneLogic::GetSingletonPtr( )->Send2Log( &tMessage, tMessage.mutable_msghead()->messageid(), nRoleID);
	return;			
}

void LogEventPlayerCorpsBattle( int nRoleID, int nCorpsID, int nAction, int nNum )
{	
	CMessage tMessage;
	CMessageLogCorps tMsgCorpsLog;

	tMessage.set_msgpara( (int)&tMsgCorpsLog );
	tMessage.mutable_msghead()->set_messageid( ID_S2L_CORPS_ACTION );	

	tMsgCorpsLog.set_oproleid( nRoleID );
	tMsgCorpsLog.set_corpsid( nCorpsID );
	tMsgCorpsLog.set_optime( time( NULL ) );
	tMsgCorpsLog.set_optype( (EOrganAction)nAction );
	tMsgCorpsLog.set_param1( nNum );
	tMsgCorpsLog.set_attrid(EM_CORPS_BATTLE);
	tMsgCorpsLog.set_param2(0);
	tMsgCorpsLog.set_param3(0);

	CSceneLogic::GetSingletonPtr( )->Send2Log( &tMessage, tMessage.mutable_msghead()->messageid(), nRoleID);
	return ;
}

void LogEventPlayerSendChat( int nRoleID, const char* npName, const char* npMsg, int nGateWayID, int nChannel, int nIP )
{
	CMessage tMessage;
	CMessageLogChat proto;

	tMessage.Clear( );
	proto.Clear( );

	tMessage.set_msgpara( (uint32_t)&proto );
	tMessage.mutable_msghead( )->set_messageid(ID_G2L_ROLE_CHAT);

	proto.set_roleid( nRoleID );
	proto.set_chatmsg( npMsg != NULL ? npMsg : "N/A" );
	proto.set_gatewayid( nGateWayID );
	proto.set_channel( nChannel );
	proto.set_ip( nIP );
	proto.set_optime( time(NULL) );
	proto.set_rolename( npName != NULL ? npName : "N/A" );

	CSceneLogic::GetSingleton().Send2Log( &tMessage, tMessage.mutable_msghead()->messageid(), nRoleID );
	return ;
}

void RecodrdBossOgreDie( CEntityOgre *pOgre, CEntityPlayer *pKiller,  CTemplateOgre* pTempOgre )
{
	if ( pOgre == NULL || pTempOgre == NULL || pKiller == NULL )
	{
		return;
	}	
	
	CMessage tMsgHead;
	CMessageLogBossOgreDie	tMsgBossOgreDie;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2L_BOSSORGEDIE );
	tMsgHead.set_msgpara( (int)&tMsgBossOgreDie );
	
	tMsgBossOgreDie.set_deathtime( time(NULL) );
	tMsgBossOgreDie.set_campid( pOgre->GetNationality() );
	tMsgBossOgreDie.set_mapid( GetMapID4Log(pOgre));

	tMsgBossOgreDie.set_posx( pOgre->GetPosX() );
	tMsgBossOgreDie.set_posy( pOgre->GetPosY() );
	tMsgBossOgreDie.set_tempid( pTempOgre->mTempID );
	tMsgBossOgreDie.set_killerid( pKiller->GetCharID() );
		
	CNpcPolicy* tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->GetObject( pOgre->GetPolicyID( ) );
	LK_ASSERT( tpPolicy != NULL, return );	
	char tBuffer[1000] = { 0 };   
	CNpcPolicy::ENMITY::iterator tFindIter = tpPolicy->mEnmityData.begin();
	char *pPos = tBuffer;
	for ( ; tFindIter != tpPolicy->mEnmityData.end(); ++ tFindIter )
	{
		CEntity *tpEntity = CSceneLogic::GetSingleton().GetEntity( tFindIter->first );
		if ( tpEntity == NULL )
		{
			continue;
		}		
		
		if ( tpEntity->IsPlayer() == false )
		{
			continue;
		}					
		CEntityPlayer *tpPlayer = ( CEntityPlayer * )tpEntity;
		sprintf( pPos, "%d:%d;\t", tpPlayer->GetCharID(), tFindIter->second );
		pPos = strchr( pPos, '\t' );		
	}
	tMsgBossOgreDie.set_damageinfo( tBuffer );	
	CSceneLogic::GetSingleton().Send2Log( &tMsgHead, tMsgHead.mutable_msghead()->messageid(), pTempOgre->mTempID );	
}

void LogLifeSkill( int nRoleID, CLifeSkill *pLifeSkill, int nActionKind, int nComposeID )
{
	if ( pLifeSkill == NULL )
	{
		return;
	}		
	
	CMessage tMsgHead;
	CMessageLifeSkill tMsgLifeSkill;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2L_LIFESKILL );		
	tMsgHead.set_msgpara( (int)&tMsgLifeSkill );	
	
	tMsgLifeSkill.set_roleid( nRoleID );
	tMsgLifeSkill.set_optime( time(NULL) );
	tMsgLifeSkill.set_lifeskilltype( pLifeSkill->GetLifeSkillType() );
	tMsgLifeSkill.set_lifeskilllevel( pLifeSkill->GetLifeSkillLevel() );
	tMsgLifeSkill.set_lifeskillkind( pLifeSkill->GetLifeSkillKind() );
	tMsgLifeSkill.set_specializeddegree( pLifeSkill->GetSpecializedDegree() );
	tMsgLifeSkill.set_actionkind( nActionKind );
	tMsgLifeSkill.set_composeid( nComposeID );
    CSceneLogic::GetSingleton().Send2Log( &tMsgHead, tMsgHead.mutable_msghead()->messageid(), nRoleID );	
}

void LogMatrix( int nSlaveID, int nMasterID, int nOpType, int nParam1, int nParam2, int nParam3 )
{	
	CMessage tMsgHead;
	CMessageLogMatrix tMatrix;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2L_MATRIX );		
	tMsgHead.set_msgpara( (int)&tMatrix );	
	
	tMatrix.set_slave_id( nSlaveID );
	tMatrix.set_master_id( nMasterID );
	tMatrix.set_optime( time(NULL) );
	tMatrix.set_operate( nOpType );
	tMatrix.set_parame1( nParam1 );
	tMatrix.set_parame2( nParam2 );
	tMatrix.set_parame3( nParam3 );
    CSceneLogic::GetSingleton().Send2Log( &tMsgHead, tMsgHead.mutable_msghead()->messageid(), 0 );	
}

void LogEventRoleBuffAction( CEntityPlayer* tpPlayer, int BuffID, int PersistTime, int nParam1, int nParam2,unsigned short bufftype)
{

	if (tpPlayer == NULL) return;

	//CSceneLogic::GetSingleton().mStatInfo.CatalogExp(emAction, EM_UPGRADE_TALENT_EXP, nExpValue);
	if ( CSceneCfgManager::GetSingleton().GetLogevent_cfg()->mLog_role_buff == 0) return;

	CPropertyPlayer* pProperty = (CPropertyPlayer*) tpPlayer->GetProperty();

	CMessage* pMessage = BuildPlayerBuffLogMessage(tpPlayer, BuffID, 
		PersistTime, nParam1, nParam2,bufftype);

	if ( pMessage != NULL)
	{
		CSceneLogic::GetSingletonPtr( )->Send2Log( pMessage, pMessage->msghead().messageid(), tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
	}

}


void LogEventPlayerRepeScore (CEntityPlayer* tpPlayer, int CurrentScore, int DeltaScore, int eScoreType)
{
	if (tpPlayer == NULL)
	{
		return;
	}
	
	CMessage tMsgHead;
	CMessageLogRoleRepeScore tScore;

	CPropertyPlayer* pProperty = (CPropertyPlayer*) tpPlayer->GetProperty();
	

	tMsgHead.mutable_msghead()->set_messageid( ID_S2L_ROLE_REPE_SCORE );		
	tMsgHead.set_msgpara( (int)&tScore );	

	tScore.set_charid( tpPlayer->GetCharID() );
	tScore.set_lineid( tpPlayer->GetLineID() );
	tScore.set_optime(time(0));
	tScore.set_mapid( GetMapID4Log(tpPlayer) );
	tScore.set_repeid( tpPlayer->GetMapObject() ? tpPlayer->GetMapObject()->GetKey() : INVITED_KEY_VALUE );
	tScore.set_currentscore( CurrentScore );
	tScore.set_deltascore( DeltaScore );
	tScore.set_scoretype( eScoreType );
	CSceneLogic::GetSingleton().Send2Log( &tMsgHead, tMsgHead.mutable_msghead()->messageid(),tpPlayer->GetCharID(), tpPlayer->GetRoleWorldID() );
}