#include "ibstoremodule.h"
#include "tinyxml.h"
#include "gmmessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "gatelogic.h"
#include "gateobj_manager.h"
#include "sequence.h"
#include "core_service.h"
#include "activity.h"



void CIbStoreModule::OnMessage(int nServerID, CMessage* tpMessage )
{
	CMessageHead* tpHead = tpMessage->mutable_msghead();
	unsigned short nMessageID = (unsigned short) tpHead->messageid();
	LK_ASSERT(((int) tpHead->srcid()) == nServerID, return);

	switch (nMessageID)
	{
	case ID_M2G_IBINFO_CMD:
		{
			OnGMMessageIbInfoCmd(nServerID, tpMessage);
			break;
		}

	case ID_M2G_SETIBPRICE_CMD:
		{
			OnGMMessageSetIbPriceCmd(nServerID, tpMessage);
			break;
		}

	case ID_M2G_SETIBLOCATION_CMD:
		{
			OnGMMessageSetIbLocationCmd(nServerID, tpMessage);
			break;
		}

	case ID_M2G_IBONSALE_CMD:
		{
			OnGMMessageIbOnSaleCmd(nServerID, tpMessage);
			break;
		}

	case ID_M2G_IBNOTSALE_CMD:
		{
			OnGMMessageIbNotSaleCmd(nServerID, tpMessage);
			break;
		}

	case ID_M2G_IBDESCRIPT_CMD:
		{
			//OnGMMessageIbDescriptCmd(nServerID, tpMessage);
			break;
		}

	case ID_S2G_SYNIBSTORE_REQUEST:
		{
			OnMessageSynIbStoreRequest(nServerID, tpMessage);
			break;
		}

	case ID_M2G_USER_CHARGE_NOTICE:
		{
			OnMessageUserChargeNotice(nServerID, tpMessage);
			break;
		}

	case ID_S2G_QUERYYUANBAO_REQUEST:
		{
			OnMessageUserQueryYuanbaoRequest(nServerID, tpMessage);
			break;
		}

	case ID_S2G_DRAWYUANBAO_REQUEST:
		{
			OnMessageUserDrawYuanbaoRequest(nServerID, tpMessage);
			break;
		}

	case ID_S2G_LOCKGOODS_REQUEST:
		{
			OnMessageLockGoodsRequest(nServerID, tpMessage);
			break;
		}
	case ID_S2G_SYNFUNCSTORE_REQUEST:
		{
			OnMessageSynFuncStoreRequest(nServerID, tpMessage);
			break;
		}

	default:
		{
			LOG_ERROR("ibtrans", "there is no action for msg(%d) on CIbStoreModule::OnMessage", nMessageID);
		}
	}
}
void CIbStoreModule::OnGMMessageIbInfoCmd(int nServerID, CMessage* tpMessage)
{
	CGMMessageIbInfoCmd * pGmCmd = (CGMMessageIbInfoCmd *) tpMessage->msgpara(); 

	CGMMessageIbInfoResult tFactMsg;
	int nResult = 0;

	if ( pGmCmd->has_item_id())
	{
		tFactMsg.set_isall(0);
		PBIbItem *pPbIbItem = tFactMsg.add_ibitems();

		CGoods* pIbItem = m_tManager.GetGoods(pGmCmd->item_id());

		if (pIbItem == NULL )
		{
			SendGmCmdNormalResult(nResult, pGmCmd->session_id(), pGmCmd->fd());
			return ;
		}

		pPbIbItem->set_item_id(pGmCmd->item_id());

		pPbIbItem->set_item_location(pIbItem->GetLocation().GetLocationString());

		pPbIbItem->set_pay_type(pIbItem->GetPriceUnit());
		pPbIbItem->set_price(pIbItem->GetPriceValue());

		pPbIbItem->set_discount(pIbItem->GetPriceValue());
		pPbIbItem->set_start_time(pIbItem->GetOnsaleTime());
		pPbIbItem->set_on_sale(pIbItem->IsActive());
	}else
	{
		tFactMsg.set_isall(1);
		CIbStoreManager::iterator iter = m_tManager.m_tGoods.begin();

		for( ; iter != m_tManager.m_tGoods.end(); ++iter)
		{
			PBIbItem *pPbIbItem = tFactMsg.add_ibitems();
			pPbIbItem->set_item_id(iter->second.GetItemID());
		}
	}

	// 发送消息
	tFactMsg.set_session_id(pGmCmd->session_id());
	tFactMsg.set_fd(pGmCmd->fd());

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_IBINFO_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tFactMsg);

	CGateServer::GetSingleton().Send2Login(&tMessage);
}

void CIbStoreModule::OnGMMessageSetIbPriceCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageSetIbPriceCmd * pGmCmd = (CGMMessageSetIbPriceCmd *) tpMessage->msgpara(); 

	int nResult = 0;

	CGoods* pIbItem = m_tManager.GetGoods(pGmCmd->item_id());
	if ( pIbItem != NULL)
	{
		if ( pGmCmd->has_price())
		{
			pIbItem->SetPriceUnit(1);
			pIbItem->SetPriceValue(pGmCmd->price());
			pIbItem->SetGoodsCount(pGmCmd->limit_num());
			pIbItem->SetOnsaleTime(time(0)+pGmCmd->time());

			nResult = 1;
		}
	}

	SendGmCmdNormalResult(nResult, pGmCmd->session_id(), pGmCmd->fd());

	if ( nResult == 1)
	{
		m_tManager.BackupAndSave();
	}
}

void CIbStoreModule::OnGMMessageSetIbLocationCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageSetIbLocationCmd * pGmCmd = (CGMMessageSetIbLocationCmd *) tpMessage->msgpara(); 

	int nResult = 0;

	size_t nPos = pGmCmd->pos().find(':');
	if (nPos == std::string::npos)
	{
		LOG_ERROR("ibtrans", "error format for the SetIbLocation command value (%s)", pGmCmd->pos().c_str());
		SendGmCmdNormalResult(nResult, pGmCmd->session_id(), pGmCmd->fd());
		return;
	}

	unsigned int nItemID = atoi (pGmCmd->pos().c_str());
	CLocation tLocation;

	tLocation.SetLocationString(pGmCmd->pos().substr(nPos+1).c_str());

	nResult = m_tManager.ExchangeIbItemPos(nItemID, tLocation) == true ? 1 : 0;

	SendGmCmdNormalResult(nResult, pGmCmd->session_id(), pGmCmd->fd());

	if(nResult == 1)
	{
		m_tManager.BackupAndSave();
	}
}



void CIbStoreModule::OnGMMessageIbOnSaleCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageIbOnSaleCmd * pGmCmd = (CGMMessageIbOnSaleCmd *) tpMessage->msgpara(); 

	CGoods *pIbItem = m_tManager.GetGoods(pGmCmd->item_id());

	int nResult = 0;
	if (pIbItem == NULL)
	{
		SendGmCmdNormalResult(nResult, pGmCmd->session_id(), pGmCmd->fd());
		return;
	}

	pIbItem->SetActive(true);
	nResult = 1;

	SendGmCmdNormalResult(nResult, pGmCmd->session_id(), pGmCmd->fd());
	m_tManager.BackupAndSave();
}

void CIbStoreModule::OnMessageSynIbStoreRequest(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	//CMessageSynIbStoreRequest * pproto = (CMessageSynIbStoreRequest*) tpMessage->msgpara(); 

	if (! m_bIsLoaded ) return;

	CMessageSynIbStoreResponse tResultMsg;

	// set ibstore
	for (size_t i = 0; i < ARRAY_CNT(m_tManager.m_pbIbStore); ++i)
	{
		if ( m_tManager.m_pbIbStore[i] != NULL)
		{
			PBIbStore* pbIbStore = tResultMsg.add_ibstore();
			pbIbStore->CopyFrom( *(m_tManager.m_pbIbStore[i]) );
		}
	}

	// set goods
	CIbStoreManager::iterator iter = m_tManager.m_tGoods.begin();
	for( ; iter != m_tManager.m_tGoods.end(); ++iter)
	{
		PBIbItemOnSale *pPbIbItem = tResultMsg.add_goods();
		CGoods& goods = iter->second;

		//good
		pPbIbItem->set_goodsid(goods.GetGoodsID());
		pPbIbItem->set_goodscount(goods.GetGoodsCount());

		//item
		pPbIbItem->set_itemid(goods.GetItemID());
		pPbIbItem->set_itemcount(goods.GetItemCount());

		//price
		PBIbMoney * pGoodPrice = pPbIbItem->mutable_goodsprice();
		pGoodPrice->set_unit(goods.GetPriceUnit());
		pGoodPrice->set_value(goods.GetPriceValue());

		//discount
		PBIbMoney * pDiscount = pPbIbItem->mutable_discount();
		pDiscount->set_unit(goods.GetPriceUnit());
		pDiscount->set_value(goods.GetDiscount());

		//expiry_time
		pPbIbItem->set_expirytime(goods.GetExpiryTime());

		//active 
		pPbIbItem->set_active(goods.IsActive()?1:0);

		//is pack
		pPbIbItem->set_ispack(goods.IsPackage());

		// limit
		pPbIbItem->set_limitperchar(goods.GetLimitPerChar());
		pPbIbItem->set_limittype(goods.GetLimitType());

		pPbIbItem->set_flag(goods.GetFlag());
	}

	// set focus tab
	for ( size_t i = 0; i < ARRAY_CNT(m_tManager.m_pbFocus); ++i)
	{
		if ( m_tManager.m_pbFocus[i] != NULL)
		{
			PBIbSubTab* pNewTab = tResultMsg.add_focustab();
			if (pNewTab)
			{
				pNewTab->CopyFrom( *(m_tManager.m_pbFocus[i]) );
			}
		}
	}

	// set bulletin
	if ( m_tManager.m_szBulletin )
	{
		tResultMsg.set_bulletin(m_tManager.m_szBulletin);
	}

	// set version
	tResultMsg.set_version(m_tManager.m_nVersion);

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2S_SYNIBSTORE_RESPONSE, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg);

	CGateServer::GetSingleton().Send2Scene(nServerID, &tMessage);

	// send dynamic info
	SendIbStoreInfo2Scene(nServerID);
}

void CIbStoreModule::OnMessageSynFuncStoreRequest(int nServerID, CMessage *tpMessage)
{
		LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);

		CMessageSynFuncStoreResponse tResultMsg;
		
		CIbStoreManager::funcconsum_t::iterator iter = m_tManager.m_tFuncConsume.begin();

		for(; iter != m_tManager.m_tFuncConsume.end() ; ++iter )
		{
				PBFuncStore * pPBFuncStore = tResultMsg.add_funcstore();
				CFuncConsume *ptFunc = &iter->second;

				pPBFuncStore->set_funcid( ptFunc->GetFuncID());

				pPBFuncStore->set_funcname(ptFunc->GetFuncName());
				pPBFuncStore->set_funcshow(ptFunc->GetFuncIsShow());
				pPBFuncStore->set_defaultconsume( ptFunc->GetDefaultType() );
				pPBFuncStore->set_funcunittype( ptFunc->GetFuncValueType() );
				pPBFuncStore->set_funcunit( ptFunc->GetFuncUnit());
				pPBFuncStore->set_funcunittype( ptFunc->GetFuncUnitType());
				pPBFuncStore->set_funccountlimit( ptFunc->GetFuncCountLimit());

				
				
				for( int i = 1; i <= FUNC_CONSUME_COUNT ; ++i )
				{
					if( ptFunc->GetFuncValue( i ) <= 0 )
					{
						continue;
					}
					PBConsume * pConsume = pPBFuncStore->add_consume();
					pConsume->set_consumetype( i );
					if( ptFunc->GetFuncValueType() == FUNC_CONSUME_NOMARL_PRICE )
					{
						pConsume->set_consumesprice( ptFunc->GetFuncValue( i ));
						pConsume->set_tamplaterate( -1 );

					}else if( ptFunc->GetFuncValueType() == FUNC_CONSUME_FROM_TAMPLATE )
					{
						pConsume->set_tamplaterate( ptFunc->GetFuncValue( i ));
						pConsume->set_consumesprice( -1 );
					}
				}
		}

		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2S_SYNFUNCSTORE_RESPONSE, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara( (uint32_t) &tResultMsg );

		CGateServer::GetSingleton().Send2Scene(nServerID , &tMessage);
}

void CIbStoreModule::OnGMMessageIbNotSaleCmd(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageIbNotSaleCmd * pGmCmd = (CGMMessageIbNotSaleCmd *) tpMessage->msgpara(); 

	CGoods *pIbItem = m_tManager.GetGoods(pGmCmd->item_id());

	int nResult = 0;
	if (pIbItem == NULL)
	{
		SendGmCmdNormalResult(nResult, pGmCmd->session_id(), pGmCmd->fd());
		return;
	}

	pIbItem->SetActive(false);
	nResult = 1;

	SendGmCmdNormalResult(nResult, pGmCmd->session_id(), pGmCmd->fd());
	m_tManager.BackupAndSave();
}


void CIbStoreModule::OnMessageLockGoodsRequest(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageLockGoodsRequest * pLockRequest = (CMessageLockGoodsRequest *) tpMessage->msgpara(); 

	int nGoodsID = pLockRequest->goodsid();
	int nLockCount = pLockRequest->lockcount();
	int nResult = ERROR_IBSTORE_INVALID_TRANS;

	CPromotion* pPromotion = m_tManager.GetPromotion(nGoodsID);
	if ( pPromotion == NULL ) 
	{
		LOG_ERROR("ibtrans", "can not find ibitem(%d)", nGoodsID);
		nResult = ERROR_IBITEM_DOESNOT_EXIST;
	}else
	{
		int nNow = time(0);
		if( ! pPromotion->IsValidTime(nNow))	// 在无效时间内
		{
			nResult = ERROR_IBSTORE_INVALID_TIME;
		}else if ( nLockCount <= 0 )			// 要购买的数量是负数
		{
			LOG_ERROR("ibtrans", "The ibitem(%d) lock count (%d) is not correct. ", nGoodsID, nLockCount);
			nResult = ERROR_IBITEM_INVALID_NUMBER; 
		}else if (pPromotion->GetMaxCount() == CGoods::em_ibitem_ulimit)	// 如果物品不设上限
		{
			nResult = ERROR_IBSTORE_LOCK_SUCCESS;
		}else if (pPromotion->GetRemainCount() <= 0)	// 如果物品没有剩余
		{
			nResult = ERROR_IBITEM_ISNOT_ENOUGH;
		}else	// 如果有剩余
		{
			// 计算可以购买的商品数目
			int nWillRemain = pPromotion->GetRemainCount() - nLockCount;
			nLockCount = (nWillRemain >= 0 ? nLockCount : pPromotion->GetRemainCount());

			// 设置剩余的数目
			pPromotion->SetRemainCount( nWillRemain >= 0 ? nWillRemain : 0);
			nResult = ERROR_IBSTORE_LOCK_SUCCESS;

			// 计算当前所有锁定的商品数
			int nTotalLockedCount = pPromotion->GetMaxCount() - pPromotion->GetRemainCount();

			// 更新数据库
			UpdateIbStoreInfo2DB(nGoodsID, nTotalLockedCount > 0 ? nTotalLockedCount : 0 , nNow); 
		}
	}

	CMessageLockGoodsResponse tResponse;
	tResponse.set_result(nResult);
	tResponse.set_roleid(pLockRequest->roleid());
	tResponse.set_goodsid(nGoodsID);
	tResponse.set_lockcount(nLockCount);
	tResponse.set_remain(pPromotion ? pPromotion->GetRemainCount() : 0);
	tResponse.set_transobjid(pLockRequest->transobjid());

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2S_LOCKGOODS_RESPONSE, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResponse);

	CGateServer::GetSingleton().Send2Scene(nServerID, &tMessage);

	// 向所有场景发送物品改变通知
	SendIbStoreInfo2Scene();	
}


void CIbStoreModule::OnMessageUserChargeNotice(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);			
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);			

	CMessageUserChargeNotice* pGmCmd = (CMessageUserChargeNotice*) tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct(pGmCmd->userid());
	if( tpPlayer != NULL
		&&  tpPlayer->mStatus == CGatePlayer::em_status_ingame_idle )
	{
		CGateServer::GetSingleton().Send2Scene(tpPlayer, tpMessage);
	}
}

void CIbStoreModule::OnMessageUserQueryYuanbaoRequest(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessage4EQueryYuanBaoRequest* pQuery = (CMessage4EQueryYuanBaoRequest*) tpMessage->msgpara();	

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pQuery->roleid());
	if (tpPlayer == NULL)
	{
		LOG_ERROR("ibtrans", "when query yuanbao balance for role(%d), role has left.", pQuery->roleid());
		return;
	}

	AGIPUserBalanceInfo eproto;	
	eproto.setUserID(tpPlayer->mAccountID);
	eproto.setRoleID(tpPlayer->mCharID);
	eproto.setRatingID(0); // 查询余额
	eproto.setGameID(tpPlayer->GetGameID());

	CAGIPSession* pSession = CGateLogic::GetSingleton().CreateSession4Proto(&eproto);
	if (pSession != NULL)
	{
		int nSessionID = pSession->get_id();

#ifdef _DEBUG_
		LOG_DEBUG("erating", "Send2ERating: MsgName[AGIPUserBalanceInfo] MsgSeq[%d] MsgPara[ AccountID(%d) RoleID(%d) RatingID(0) ]",
			nSessionID, tpPlayer->mAccountID, tpPlayer->mCharID);
#endif

		pSession->assign(nSessionID, em_agipsession_queryyuanbao, tpPlayer->mAccountID, tpPlayer->mCharID,
			CGateLogic::GetSingleton().mConfig.nEratingSessionTimeout);
	}
}

void CIbStoreModule::OnMessageUserDrawYuanbaoRequest(int nServerID, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessage4EDrawYuanBaoRequest* pQuery = (CMessage4EDrawYuanBaoRequest*) tpMessage->msgpara();	

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pQuery->roleid());
	if (tpPlayer == NULL)
	{
		LOG_ERROR("ibtrans", "when query yuanbao balance for role(%d), role has left.", pQuery->roleid());
		return;
	}

	AGIPUserDrawBalance eproto;	
	eproto.setUserID(tpPlayer->mAccountID);
	eproto.setRoleID(tpPlayer->mAccountID);
	eproto.setRatingID(CGateServer::GetSingleton().GetRegistID());
	eproto.setSubjectID(pQuery->draw().unit());
	eproto.setAmount(pQuery->draw().value());
	eproto.setGameID(tpPlayer->GetGameID());

	CAGIPSession* pSession = CGateLogic::GetSingleton().CreateSession4Proto(&eproto);
	if (pSession != NULL)
	{
		int nSessionID = pSession->get_id();

#ifdef _DEBUG_
		LOG_DEBUG("erating", "Send2ERating: MsgName[AGIPUserDrawBalance] MsgSeq[%d] MsgPara[ AccountID(%d) RoleID(%d) RatingID(%d) SubjectID(%d) Ammount(%d) ]",
			nSessionID, tpPlayer->mAccountID, tpPlayer->mCharID, 
			CGateServer::GetSingleton().GetRegistID(),pQuery->draw().unit(), pQuery->draw().value());
#endif

		pSession->assign(nSessionID, em_agipsession_drawyuanbao, tpPlayer->mAccountID, tpPlayer->mCharID,
			CGateLogic::GetSingleton().mConfig.nEratingSessionTimeout);
	}
}

void CIbStoreModule::OnSessionLoabIbStoreInfoFromDB(CMessage* tpMessage, int nStart, int ONCECOUNT)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != NULL, return);

	CMessageExecuteSqlResponse3* pMessage = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();
	if ( pMessage->resultcode() != 1 )
	{
		LOG_ERROR("ibtrans", "LoadIbStoreInfoFromDB execute error, pls see the dbsvrd.log");
		return;
	}

	int colnum = pMessage->colcount();
	int rownum = pMessage->rowcount();

	if ( colnum != 3)
	{
		LOG_ERROR("ibtrans", "LoadIbStoreInfoFromDB has some error with fields number ");
		return;
	}

	for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
	{
		int nGoodsID = atoi(pMessage->fieldvalue(0 + tRowIndex * colnum).c_str());
		int nLockedCount = atoi(pMessage->fieldvalue(1 + tRowIndex * colnum).c_str());
		int nUpdateTime = atoi(pMessage->fieldvalue(2 + tRowIndex * colnum).c_str());

		CPromotion* pPromotion = m_tManager.GetPromotion(nGoodsID);
		if ( pPromotion == NULL) 
		{
			LOG_ERROR("ibtrans", "There is no promotion for nGoodsID(%d)",  nGoodsID);
			continue;
		}

		if ( pPromotion->GetMaxCount() == CGoods::em_ibitem_ulimit)
		{
			pPromotion->SetRemainCount(CGoods::em_ibitem_ulimit);
		}else
		{
			int nRemainCount = pPromotion->GetMaxCount() - nLockedCount;
			pPromotion->SetRemainCount(nRemainCount > 0 ? nRemainCount : 0);
		}

		pPromotion->SetUpdateTime(nUpdateTime);
	}

	if ( rownum == ONCECOUNT )
	{
		LoadIbStoreInfoFromDB(nStart+rownum, ONCECOUNT);
		return;
	}

	LOG_INFO("default", "LoadIbStoreInfoFromDB for %d + %d records successfully", nStart, rownum); 
	m_bIsLoaded = true;
}



void CIbStoreModule::SendIbStoreInfo2Scene(int nServerID)
{
	CIbStoreManager::promotion_t::iterator iter = m_tManager.m_tPromotion.begin();
	CMessageIbItemRemainNotice tDynamicInfo;

	for (; iter != m_tManager.m_tPromotion.end(); ++iter)
	{
		PBPromotion* pbPromotion = tDynamicInfo.add_promotion();			
		if ( pbPromotion != NULL)
		{
			pbPromotion->set_goodsid(iter->second.GetGoodsID());
			pbPromotion->set_remaincount(iter->second.GetRemainCount());

			pbPromotion->set_begintime(iter->second.GetActiveTime());
			pbPromotion->set_endtime(iter->second.GetExpirityTime());

			// pbPromotion->set_coldtime(iter->second.GetColdTime());
			// pbPromotion->set_elapsedtime(iter->second.GetElapsedTime());

			pbPromotion->set_maxcount(iter->second.GetMaxCount());
			// pbPromotion->set_coldnum(iter->second.GetColdNum());
		}
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2S_NOTIFY_IBSTOREPROMOTION, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tDynamicInfo);

	if (nServerID != 0)
	{
		CGateServer::GetSingleton().Send2Scene(nServerID, &tMessage);
	}else
	{
		CGateServer::GetSingleton().Send2Scene((CSceneServer *)NULL, &tMessage);
	}
}


void CIbStoreModule::SendGmCmdNormalResult(int nResult, unsigned int nSessionID, unsigned int nFd)
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
