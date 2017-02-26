#include "stdafx.hpp"
#include "ibstoremodule.h"
#include "servermessage_in.h"
#include "scenelogic.h"
#include "property.h"
#include "propertymodule.h"
#include "ibstore_pb.hxx.pb.h"
#include "gmmessage_pb.hxx.pb.h"
#include "logmodule.h"
#include "mailmodule.h"
#include "friendmodule.h"
#include "dbinterface.h"


//#define WORLD_SERVER_ID(WORLDID, SERVERID) (WORLDID * 100 + 8)

//
// CIbStoreModule
//
//
template<> CIbStoreModule* CSingleton< CIbStoreModule >::spSingleton = NULL;
template<> CSequence* CSingleton< CSequence >::spSingleton = NULL;
char CIbStoreModule::sModuleName[ NAME_LENGTH ] = "IBSTORE";

#define NVALUE(PTM) (((PTM)->tm_year - 100) * 1000000 + ( (PTM)->tm_mon + 1) * 10000 +  (PTM)->tm_mday * 100 )

CIbStoreModule::CIbStoreModule() : CLogicModule( sModuleName )
{
	if (mShmPtr->GetInitMode() == SHM_INIT)
	{
		m_tGoods.initailize();
		m_tPromotion.initailize();
	}

	m_ppbIbStore = NULL;
	m_ppbFocus = NULL;
	m_bFuncIsLoad = false;

	mLoadInterval = 5000;

	m_pSequence = new CSequence( /*1, 0x7FFFFFFF, 1*/ );
 
}

CIbStoreModule::~CIbStoreModule() {}

unsigned int CIbStoreModule::CountSize()
{
	return sizeof(CIbStoreModule);
}

void CIbStoreModule::OnLaunchServer()
{
	IBTRANS_LOG* tpCfg = CSceneCfgManager::GetSingletonPtr()->GetIbtrans_log();
	if( tpCfg != NULL )
	{
		INIT_ROLLINGFILE_LOG( tpCfg->mIdentifier, tpCfg->mDirectory, 
				(LogLevel)tpCfg->mPriority , tpCfg->mMaxfilesize, tpCfg->mMaxbackupindex);
	}
}

void CIbStoreModule::OnExitServer()
{
	if ( m_ppbIbStore != NULL)
	{
		delete [] m_ppbIbStore;
		m_ppbIbStore = NULL;
	}

	if ( m_pSequence != NULL)
	{
		delete m_pSequence;
		m_pSequence  = NULL;
	}
}

void CIbStoreModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset)
{
	if ( m_ppbIbStore == NULL )
	{
		if ( mLoadInterval.Elapse( vTickOffset ) == true)
		{
			CMessageSynIbStoreRequest tFactMsg;

			CMessage tMsg;
			tMsg.set_msgpara((uint32_t) &tFactMsg);

			pbmsg_sethead(&tMsg, ID_S2G_SYNIBSTORE_REQUEST, 0, 0, 0, 0, 0, 0);
			CSceneLogic::GetSingleton().Send2Gate(&tMsg);

//			m_pSequence->GetNewSequent( );
		}
	}

	if( m_bFuncIsLoad== false)
	{
		if ( mLoadInterval.Elapse( vTickOffset ) == true)
		{
			CMessage tFuncMsg;
			CMessageSynFuncStoreRequest tFuncStoreMsg;

			tFuncMsg.set_msgpara( (uint32_t) &tFuncStoreMsg);

			pbmsg_sethead(&tFuncMsg, ID_S2G_SYNFUNCSTORE_REQUEST, 0, 0, 0, 0, 0, 0);
			CSceneLogic::GetSingleton().Send2Gate(&tFuncMsg);


		}
	}
}


int CIbStoreModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )
{
	return SUCCESS;
}

void CIbStoreModule::OnRouterMessage( CMessage* pMessage )
{

}

void CIbStoreModule::OnGateMessage( CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->msgpara() != 0, return);
	const CMessageHead& tHead = pMessage->msghead();

	switch( tHead.messageid( ) )
	{
		case ID_M2G_USER_CHARGE_NOTICE:
		{
			OnMessageUserChargeNotice(pMessage);
			break;
		}

		case ID_G2S_SYNIBSTORE_RESPONSE:
		{
			OnMessageSynIbStoreResponse(pMessage);
			break;
		}

		case ID_G2S_QUERYYUANBAO_RESPONSE:
		{
			OnMessageQueryYuanBaoResponse( pMessage);
			break;
		}

		case ID_G2S_DRAWYUANBAO_RESPONSE:
		{
			OnMessageDrawYuanBaoResponse( pMessage);
			break;
		}

		case ID_G2S_LOCKGOODS_RESPONSE:
		{
			OnMessageLockGoodsResponse( pMessage );
			break;
		}

		case ID_G2S_NOTIFY_IBSTOREPROMOTION:
		{
			OnMessageIbStorePromotionNotice(pMessage);
			break;
		}
		case ID_G2S_SYNFUNCSTORE_RESPONSE:
		{
			OnMessageSynFuncStoreResponse(pMessage);
			break;
		}

		default:
		{
			LOG_ERROR("ibtrans", "There is no action for msg id(%d)", tHead.messageid());
			break;
		}
	}
}


void CIbStoreModule::OnMessageUserChargeNotice(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageUserChargeNotice* tpFactMsg = (CMessageUserChargeNotice*) tpMessage->msgpara();

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByAccountID(tpFactMsg->userid());
	if( tpPlayer == NULL ) return;

	LOG_INFO("ibtrans", "account(%d: %s) charged unit(%d) amount(%d)",
			tpFactMsg->userid(), tpPlayer->GetCharName(), tpFactMsg->subjectid(), tpFactMsg->amount());

	if ( tpFactMsg->subjectid() == CYuanBao::em_golden_yuanbao)
	{
		CYuanBao& yb = ((CPropertyPlayer* ) tpPlayer->GetProperty())->GoldenYB();
		yb.add(tpFactMsg->amount());
	}else if ( tpFactMsg->subjectid() == CYuanBao::em_silver_yuanbao )
	{
		CYuanBao& yb = ((CPropertyPlayer* ) tpPlayer->GetProperty())->SilverYB();
		yb.add(tpFactMsg->amount());
	}else
	{
		LOG_ERROR("ibtrans", "account(%d: %s) charged unit(%d) amount(%d), but unit(%d) is not be surpport",
				tpFactMsg->userid(), tpPlayer->GetCharName(), tpFactMsg->subjectid(), tpFactMsg->amount(), tpFactMsg->subjectid());
		return;
	}

	CMessage tMsg;
	tMsg.set_msgpara((uint32_t) tpFactMsg);
	pbmsg_sethead(&tMsg, ID_S2C_NOTIFY_USERCHARGE, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMsg);
}

void CIbStoreModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT(pMessage != NULL  &&  pMessage->msgpara() != 0 , return);

	switch(  pMessage->msghead().messageid() )
	{
		case ID_C2S_REQUEST_LISTIBITEMS:
		{
			OnMessageListIbItemsRequest(pPlayer, pMessage);
			break;
		}

		case ID_C2S_REQUEST_PURCHASEIBITEM:
		{
			OnMessagePurchaseIbItemRequest(pPlayer, pMessage);
			break;
		}

		case ID_C2S_GET_FUNCCONSUMES_REQUEST:
		{
			OnMessageGetFuncConsumesRequest(pPlayer, pMessage);
			break;
		}
		case ID_C2S_PURCHASE_FUNC_REQUEST:
		{
			OnMessagePurchaseFuncRequest( pPlayer, pMessage);
			break;
		}

		default:
		{
			LOG_ERROR("ibstore", "there is no action for msg(%d) on ibsotre module",  
						pMessage->msghead().messageid());
			break;
		}
	}
}

void CIbStoreModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	
}

void CIbStoreModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{

}

bool CIbStoreModule::OnSaveData( CEntityPlayer* pPlayer, int vSaveCode )
{
	return true;
}

void CIbStoreModule::OnLoadConfig( )
{
	IBTRANS_LOG* tpCfg = CSceneCfgManager::GetSingletonPtr()->GetIbtrans_log();
	if( tpCfg != NULL )
	{
		INIT_ROLLINGFILE_LOG( tpCfg->mIdentifier, tpCfg->mDirectory, 
				(LogLevel)tpCfg->mPriority , tpCfg->mMaxfilesize, tpCfg->mMaxbackupindex);
	}
}

void CIbStoreModule::OnMessageListIbItemsRequest(CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->msgpara() != 0, return);
	if (pPlayer == NULL) return;

	CMessageListIbItemsResponse tResultMsg;

	// version
	CMessageListIbItemsRequest* pListRequest = (CMessageListIbItemsRequest*) pMessage->msgpara();		
	tResultMsg.set_isopenrmbshopui( pListRequest->isopenrmbshopui());
	if (pListRequest->has_version() && pListRequest->version() == (uint32_t) m_nVersion)
	{
		tResultMsg.set_error(1);

		// send message
		CMessage tMsg;
		tMsg.set_msgpara((uint32_t) &tResultMsg);
		pbmsg_sethead(&tMsg, ID_S2C_RESPONSE_LISTIBITEMS, 0, 0, 0, 0, 0, 0);
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsg);

		SendIbStorePromotion2Player(pPlayer);
		return;
	}

	// set version
	tResultMsg.set_version(m_nVersion);

	// ibstore
	for ( size_t i = 0; i < m_nStore; ++ i)
	{
		PBIbStore* pbIbStore = tResultMsg.add_ibstore();
		*pbIbStore = m_ppbIbStore[i];
	}

	// goods
	iterator iter = m_tGoods.begin();
	for( ; iter != m_tGoods.end(); ++iter)
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

		// flag
		pPbIbItem->set_flag(goods.GetFlag());

		// limit and limit_type
		if ( goods.GetLimitPerChar() > 0 )
		{
			pPbIbItem->set_limitperchar(goods.GetLimitPerChar());
			pPbIbItem->set_limittype(goods.GetLimitType());
		}
	}

	// set focus
	for ( size_t i = 0; i < m_nFocus; ++i)	
	{
		PBIbSubTab* pbIbFocus = tResultMsg.add_focustab();
		*pbIbFocus = m_ppbFocus[i];
	}

	// set bulletin
	tResultMsg.set_bulletin(m_szBulletin);

	// golden yb
	PBIbMoney*  pbGoldenYB = tResultMsg.add_remainmoneys();
	CPropertyPlayer* tpProperty = (CPropertyPlayer*) pPlayer->GetProperty();
	if (pbGoldenYB != NULL)
	{
		pbGoldenYB->set_unit(CYuanBao::em_golden_yuanbao);
		pbGoldenYB->set_value(tpProperty->GoldenYB().remain());
	}else
	{
		LOG_ERROR("ibtrans", "Can not alloc memory for golden PBIbMoney for account(%d) role(%d:%s)",
				pPlayer->GetAccountID(), pPlayer->GetCharID(), pPlayer->GetCharName());
		return;
	}

	// silver yb
	PBIbMoney* pbSilverYB = tResultMsg.add_remainmoneys();
	if (pbSilverYB != NULL)
	{
		pbSilverYB->set_unit(CYuanBao::em_silver_yuanbao);
		pbSilverYB->set_value(tpProperty->SilverYB().remain());
	}else
	{
		LOG_ERROR("ibtrans", "Can not alloc memory for silver PBIbMoney for account(%d) role(%d:%s)",
				pPlayer->GetAccountID(), pPlayer->GetCharID(), pPlayer->GetCharName());
		return;
	}

	//声望
	PBIbMoney* pbCombatFame = tResultMsg.add_remainmoneys();
	if (pbCombatFame != NULL)
	{
		pbCombatFame->set_unit(CYuanBao::em_combat_fame);
		pbCombatFame->set_value(tpProperty->mWizard.GetCombatFame());
	}else
	{
		LOG_ERROR("ibtrans", "Can not alloc memory for combatfame PBIbMoney for account(%d) role(%d:%s)",
				pPlayer->GetAccountID(), pPlayer->GetCharID(), pPlayer->GetCharName());
		return;
	}

	PBIbMoney* pbRepeFame = tResultMsg.add_remainmoneys();
	if (pbRepeFame != NULL)
	{
		pbRepeFame->set_unit(CYuanBao::em_repe_fame);
		pbRepeFame->set_value(tpProperty->mWizard.GetRepeFame());
	}else
	{
		LOG_ERROR("ibtrans", "Can not alloc memory for repefame PBIbMoney for account(%d) role(%d:%s)",
				pPlayer->GetAccountID(), pPlayer->GetCharID(), pPlayer->GetCharName());
		return;
	}

	PBIbMoney* pbCommonFame = tResultMsg.add_remainmoneys();
	if (pbCommonFame != NULL)
	{
		pbCommonFame->set_unit(CYuanBao::em_common_fame);
		pbCommonFame->set_value(tpProperty->mWizard.GetCommonFame());
	}else
	{
		LOG_ERROR("ibtrans", "Can not alloc memory for commonfame PBIbMoney for account(%d) role(%d:%s)",
				pPlayer->GetAccountID(), pPlayer->GetCharID(), pPlayer->GetCharName());
		return;
	}

	// send message
	CMessage tMsg;
	tMsg.set_msgpara((uint32_t) &tResultMsg);
	pbmsg_sethead(&tMsg, ID_S2C_RESPONSE_LISTIBITEMS, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsg);

	SendIbStorePromotion2Player(pPlayer);
}


void CIbStoreModule:: OnMessageGetFuncConsumesRequest(CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->msgpara() != 0, return);
	if (pPlayer == NULL) return;

	if(CSceneCfgManager::GetSingletonPtr( )->GetFuncconsume_cfg()->mIsopen == false)
	{	
		return;
	}

	CMessageGetFuncConsumesResponse tResultMsg;
	tResultMsg.set_error(0);
	CMessageGetFuncConsumesRequest *pFuncsRequest = ( CMessageGetFuncConsumesRequest * )pMessage->msgpara();

	//funcs
	funcconsum_t::iterator iter = m_tFuncConsume.begin();
	for(; iter != m_tFuncConsume.end();++iter )
	{
		PBFuncStore *pbFuncStore = tResultMsg.add_funcstore();
		CFuncConsume& func = iter->second;
		pbFuncStore->set_funcid( func.GetFuncID() );
		pbFuncStore->set_funcname( func.GetFuncName() );
		pbFuncStore->set_funcunit( func.GetFuncUnit());
		pbFuncStore->set_funcunittype( func.GetFuncUnitType() );
		pbFuncStore->set_funcshow( func.GetFuncIsShow());
		pbFuncStore->set_defaultconsume(func.GetDefaultType());
		pbFuncStore->set_funccountlimit( func.GetFuncCountLimit());
		
		for(int i = 1; i <= FUNC_CONSUME_COUNT; ++i )
		{

			if( func.GetFuncValue( i ) <= 0 )
			{
				continue;
			}
			PBConsume * pbConsume = pbFuncStore->add_consume();
			
			if(func.GetFuncValueType() == FUNC_CONSUME_NOMARL_PRICE)
			{
				pbConsume->set_consumetype(i);
				pbConsume->set_consumesprice( func.GetFuncValue( i ) );
				pbConsume->set_tamplaterate(-1);
			}else if( func.GetFuncValueType() == FUNC_CONSUME_FROM_TAMPLATE )
			{
				pbConsume->set_consumetype(i);
				pbConsume->set_tamplaterate( -1 );

				if( func.GetFuncID() == RELIVE_BY_FUNCCONSUME )
				{
					//检查金钱
					// 扣除金币
					CTemplateReliveTable* tpFeeTable = ( CTemplateReliveTable* ) CDataStatic::GetTemp( TEMP_RELIVETABLE );
					if ( tpFeeTable == NULL )
					{
						LOG_ERROR( "ibtrans", "[ %s : %d ] [ %s ] cannt find template TEMPTYPE = %d", __LK_FILE__, __LINE__, __FUNCTION__, TEMP_RELIVETABLE );
						return ;
					}
					//使值 精确到100位
					int tSprice =  func.GetFuncValue(i) * tpFeeTable->mFee[ pPlayer->GetLevel() ]/10000;
					pbConsume->set_consumesprice( tSprice );	
				}else{
					pbConsume->set_consumesprice( -1 );
					LOG_ERROR( "ibtrans", "[ %s :%d ] [ %s ]  this id  is not found to set template ,funcid = %d", __LK_FILE__, __LINE__, __FUNCTION__, func.GetFuncID() );
				}



			}else
			{
				pbConsume->set_consumetype( -1 );
				pbConsume->set_consumesprice( -1 );
				pbConsume->set_tamplaterate( -1 );
			}
			if( i == FUNC_BIND_MONEY || i == FUNC_UNBIND_MONEY )
			{
				pbConsume->set_consumesprice( pbConsume->consumesprice() > 100 ? pbConsume->consumesprice() : 100);
			}else if (i == FUNC_GOLDED_YB || i == FUNC_SILVER_YB )
			{
				pbConsume->set_consumesprice( pbConsume->consumesprice() > 1 ? pbConsume->consumesprice() : 1);
			}
		}	
	}

	// send message
	CMessage tMsg;
	tMsg.set_msgpara((uint32_t) &tResultMsg);
	pbmsg_sethead(&tMsg, ID_S2C_GET_FUNCCONSUMES_RESPONSE, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsg);

	SendIbStorePromotion2Player(pPlayer);

}

void CIbStoreModule::SendPurchaseError(CEntityPlayer* tpEntity, int nError)
{
	CMessage tMessge;
	CMessagePurchaseIbItemResponse tFactMsg;

	tFactMsg.set_error(nError);

	CMessage tMsg;
	tMsg.set_msgpara((uint32_t) &tFactMsg);
	pbmsg_sethead(&tMsg, ID_S2C_RESPONSE_PURCHASEIBITEM, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingleton().Send2Player( tpEntity, &tMsg);

	SendIbStorePromotion2Player(tpEntity);
}

void CIbStoreModule::OnMessagePurchaseIbItemRequest(CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->msgpara() != 0, return);
	if (pPlayer == NULL) return;

	// 检查是否有密码保护
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{					
		SendPurchaseError(pPlayer, tPwdCheckRet );
		return;
	}	

	time_t nTimeStampt = time(0);
	CMessagePurchaseIbItemRequest * pproto = (CMessagePurchaseIbItemRequest *) pMessage->msgpara();

	CGoods* pGoods = GetGoods( pproto->goodsid());	
	if ( pGoods == NULL )
	{
		SendPurchaseError(pPlayer, ERROR_IBITEM_DOESNOT_EXIST );
		return;
	}

	CPromotion* pPromotion = NULL;
	promotion_t::iterator iter = m_tPromotion.find(pproto->goodsid());
	if ( iter != m_tPromotion.end())
	{
		pPromotion = &(iter->second);
		if (pPromotion != NULL)
		{
			if ( ! pPromotion->IsEnougth(pproto->goodscount()))
			{
				SendPurchaseError(pPlayer, ERROR_IBITEM_ISNOT_ENOUGH );
				return;
			}

			int nNow = nTimeStampt;
			// 注意: 场景上的BeginTime和EndTime与网关上的不一致, 为了与客户端兼容
			if( pPromotion->GetBeginTime() > nNow || pPromotion->GetEndTime() < nNow )
			{
				SendPurchaseError(pPlayer, ERROR_IBSTORE_INVALID_TIME );
				return;
			}
		}
	}
	
	/*
	if ( pGoods->GetGoodsCount() != -1 )
	{
		if ( pGoods->GetGoodsCount() < (int) pproto->goodscount())
		{
			SendPurchaseError(pPlayer, ERROR_IBITEM_ISNOT_ENOUGH );
			return;
		}
	}
	*/

	CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(pGoods->GetItemID());
	if (ptpl == NULL) 
	{
		SendPurchaseError(pPlayer, ERROR_IBITEM_DOESNOT_EXIST );
		return;
	}

	// 检查是否是可堆叠的物品
	if ( ptpl->mPileLimit == 1 )
	{
		// 不可堆叠的只能购买一件
		if( pproto->goodscount() != 1)
		{
			SendPurchaseError(pPlayer, ERROR_ONLY_ONE_IBITEM_PERMITED);
			return;
		}
	}else
	{
		// 可堆叠的的数量必须大于1
		if ( pproto->goodscount() < 1)
		{
			SendPurchaseError(pPlayer, ERROR_IBITEM_INVALID_NUMBER);
			return;
		}
	}

	int nTotalItem = pproto->goodscount() * pGoods->GetItemCount();
	if ( nTotalItem < 0)
	{
		SendPurchaseError(pPlayer, ERROR_IBITEM_INVALID_NUMBER);
		return;
	}

	// 检查一下包裹空间
	// CPropertyPlayer* tpProperty = (CPropertyPlayer*) pPlayer->GetProperty();
	if ( tpProperty->GetBaggage()->Space( pGoods->GetItemID(), nTotalItem) == false )
	{
		SendPurchaseError(pPlayer, ERROR_PRONOSPACE );
		return;
	}

	// 计算需要金额
	int nNeedMoney = pGoods->GetDiscount() * pproto->goodscount(); 
	if ( nNeedMoney < 0)
	{
		SendPurchaseError(pPlayer, ERROR_PLAYER_HASNO_ENOUGHYB);
		return; 
	}

	// 检查金钱
	CYuanBao& yb = pGoods->GetPriceUnit()  == CYuanBao::em_golden_yuanbao ? tpProperty->GoldenYB() : tpProperty->SilverYB();
	if ( (pGoods->GetPriceUnit() == CYuanBao::em_golden_yuanbao) || (pGoods->GetPriceUnit() == CYuanBao::em_silver_yuanbao) )
	{
		if ( yb.remain() < nNeedMoney)
		{	
			if (pGoods->GetPriceUnit()  == CYuanBao::em_golden_yuanbao)
			{
				SendPurchaseError(pPlayer, ERROR_PLAYER_HASNO_ENOUGHYB);
			}
			else
			{
				SendPurchaseError(pPlayer, ERROR_PLAYER_HASNO_ENOUGHBINDYB);
			}
			return;
		}
	}
	else if ( pGoods->GetPriceUnit() == CYuanBao::em_combat_fame )
	{
		if ( tpProperty->mWizard.GetCombatFame() < nNeedMoney )
		{
			SendPurchaseError(pPlayer, ERROR_PLAYER_HASNO_ENOUGHFAME);
			return;
		}
	}
	else if ( pGoods->GetPriceUnit() == CYuanBao::em_repe_fame )
	{
		if ( tpProperty->mWizard.GetRepeFame() < nNeedMoney )
		{
			SendPurchaseError(pPlayer, ERROR_PLAYER_HASNO_ENOUGHFAME);
			return;
		}
	}
	else if ( pGoods->GetPriceUnit() == CYuanBao::em_common_fame )
	{
		if ( tpProperty->mWizard.GetCommonFame() < nNeedMoney )
		{
			SendPurchaseError(pPlayer, ERROR_PLAYER_HASNO_ENOUGHFAME);
			return;
		}
	}
	else
	{
		return;
	}

	// 计算是否有个数限制	
	if ( pGoods->GetLimitPerChar() != 0 )
	{
		std::pair<int, int> ret = GetPurchaseTimes4Player(pPlayer, pGoods, nTimeStampt);
		int nTimes = ret.second;

		if(pproto->goodscount() > (uint32_t) pGoods->GetLimitPerChar())
		{
			SendPurchaseError(pPlayer, ERROR_PLAYER_IBITEM_LIMITED);
			return;
		}

		if (pproto->goodscount() > (uint32_t) (pGoods->GetLimitPerChar() - nTimes))
		{
			SendPurchaseError(pPlayer, ERROR_PLAYER_IBITEM_LIMITED);
			return;
		}
	}

	CIbTrans* tpTrans = (CIbTrans*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_IBTRANS );
	if ( tpTrans == NULL)
	{
		LOG_ERROR("ibtrans", "transaction pool is full, pls check it.");
		SendPurchaseError(pPlayer, ERROR_IBITEM_INTERNAL);

		return;
	}else
	{
		//tpTrans->SetTransID(nTransID);
		tpTrans->SetGoodsID(pGoods->GetGoodsID());
		tpTrans->SetGoodsCount(pproto->goodscount());

		tpTrans->SetItemID(pGoods->GetItemID());
		tpTrans->SetItemCount(pGoods->GetItemCount());

		tpTrans->SetPriceUnit(pGoods->GetPriceUnit());
		tpTrans->SetPriceValue(pGoods->GetPriceValue());

		tpTrans->SetDiscount(pGoods->GetDiscount());
		tpTrans->SetTransTime(nTimeStampt);

		tpTrans->SetRoleID(pPlayer->GetCharID());
		tpTrans->SetAccountID(pPlayer->GetAccountID());

		tpTrans->SetRoleSex(pPlayer->GetSex());
		tpTrans->SetRoleMetier(pPlayer->GetMetier());
		tpTrans->SetRoleIP(pPlayer->GetClientInfo()->mClientAddr);
		tpTrans->SetRoleLevel(pPlayer->GetLevel());
		tpTrans->SetBalanceBeforePurchase( yb.remain() );
	}

	// 进行物品计数
	if ( pGoods->GetLimitPerChar() != 0 )
	{
		IncPurchaseTimes4Player(pPlayer, pGoods, pproto->goodscount(), tpTrans->GetTransTime());
	}
	

	if ( (pGoods->GetPriceUnit() == CYuanBao::em_golden_yuanbao) || (pGoods->GetPriceUnit() == CYuanBao::em_silver_yuanbao) )
	{
		// 如果是系统限购物品，则向网关发送锁定请求
		if ( pPromotion != NULL && pPromotion->GetMaxCount() > 0)	
		{
			// 需要做超时处理
			CMessageLockGoodsRequest tFactMsg;
			tFactMsg.set_goodsid(pGoods->GetGoodsID());
			tFactMsg.set_lockcount(pproto->goodscount());
			tFactMsg.set_roleid(pPlayer->GetCharID());
			tFactMsg.set_transobjid(tpTrans->get_id());

			CMessage tMsg;
			tMsg.set_msgpara((uint32_t) &tFactMsg);

			pbmsg_sethead(&tMsg, ID_S2G_LOCKGOODS_REQUEST, 0, 0, 0, 0, 0, 0);
			CSceneLogic::GetSingleton().Send2Gate(&tMsg);

			return;
		}

		// 锁定金钱
		if ( yb.lock(nNeedMoney) != true )
		{
			// 如果是限次购买物品,扣减次数
			if ( pGoods->GetLimitPerChar() != 0 )
			{
				IncPurchaseTimes4Player(pPlayer, pGoods, -1 * pproto->goodscount(), tpTrans->GetTransTime());
			}

			SendPurchaseError(pPlayer, ERROR_PLAYER_HASNO_ENOUGHYB);
			return;
		}

		// 非限购商品直接向数据库发送扣钱请求
		if (Send2DBPurchaseRequest(pPlayer, tpTrans, pGoods, true) == false)
		{
			yb.rollback(nNeedMoney);	

			// 如果是限次购买物品,扣减次数
			if ( pGoods->GetLimitPerChar() != 0 )
			{
				IncPurchaseTimes4Player(pPlayer, pGoods, -1 * pproto->goodscount(), tpTrans->GetTransTime());
			}

			CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
			return;
		}
		
		yb.commit(nNeedMoney);
	}
	else
	{
		if ( Send2DBPurchaseRequest(pPlayer, tpTrans, pGoods, false) == true )
		{
			if ( pGoods->GetPriceUnit() == CYuanBao::em_combat_fame )
			{
				CPropertyModule::GetSingleton().ChangeCombatFame( pPlayer, -1 * nNeedMoney );
			}
			else if ( pGoods->GetPriceUnit() == CYuanBao::em_repe_fame )
			{
				CPropertyModule::GetSingleton().ChangeRepeFame( pPlayer, -1 * nNeedMoney );
			}
			else if ( pGoods->GetPriceUnit() == CYuanBao::em_common_fame )
			{
				CPropertyModule::GetSingleton().ChangeCommonFame( pPlayer, -1 * nNeedMoney );
			}
		}
		else
		{
			if ( pGoods->GetLimitPerChar() != 0 )
			{
				IncPurchaseTimes4Player(pPlayer, pGoods, -1 * pproto->goodscount(), tpTrans->GetTransTime());			
			}
			CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		}
	}

	return;
}

void CIbStoreModule::OnMessagePurchaseFuncRequest(CEntityPlayer * pPlayer, CMessage *pMessage )
{
	LK_ASSERT(pMessage != NULL && pMessage->msgpara() != 0, return);
	if (pPlayer == NULL) return;

	// 检查是否有密码保护
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{					
		SendPurchaseError(pPlayer, tPwdCheckRet );
		return;
	}	

	if(CSceneCfgManager::GetSingletonPtr( )->GetFuncconsume_cfg()->mIsopen == false)
	{	
		SendPurchaseFuncError(pPlayer, ERROR_FUNC_DOESNOT_EXIST );
		return;
	}
	CMessagePurchaseFuncRequest *pproto = (CMessagePurchaseFuncRequest *)pMessage->msgpara();

	int tConsumeType = pproto->consumetype();

	CFuncConsume *func = GetFuncs( pproto->funcid());
	if( func == NULL )
	{
		SendPurchaseFuncError(pPlayer, ERROR_FUNC_DOESNOT_EXIST );
		return;
	}

	if( pproto->funccount() > func->GetFuncCountLimit() )
	{
		SendPurchaseFuncError(pPlayer, ERROR_PLAYER_IBITEM_LIMITED);
		return;
	}


	if ( tConsumeType == CYuanBao::em_bind_money || tConsumeType == CYuanBao::em_unbind_money)
	{
		PurchaseFuncByMoney(pPlayer,pproto);
	}else if ( tConsumeType == CYuanBao::em_golden_yuanbao || tConsumeType == CYuanBao::em_silver_yuanbao)
	{	
		PurchaseFuncByYB( pPlayer, pproto );
	}

}


std::pair<int, int> CIbStoreModule::GetPurchaseTimes4Player(CEntityPlayer* tpPlayer, CGoods* pGoods, time_t t)
{
	int Key = CPersonalTimesMgr::MakeKey(TIMESMGR_PURCHASE, pGoods->GetGoodsID());
	int nValue = ((CPropertyPlayer*) tpPlayer->GetProperty())->GetTimesMgr()->GetTimes(Key);

	int nOldPeriod = nValue / 100;
	int nNewPeriod = 0;

	int nTimes = nValue % 100;

	struct tm tmp;
	struct tm *ptm = localtime_r(&t, &tmp);
	switch ( pGoods->GetLimitType())	
	{
		case EM_LIMIT_BY_LIFECYCLE: // 角色在生命周期范围内只能购买固定个数
		{

			break;
		}

		case EM_LIMIT_BY_HOUR: // 角色在某小时范围内只能购买固定个数
		{
			nNewPeriod = NVALUE(ptm) + ptm->tm_hour;
			if ( nOldPeriod != nNewPeriod )
			{
				nTimes = 0;
			}

			break;
		}

		case EM_LIMIT_BY_6HOUR: // 角色在六小时范围内只能购买固定个数
		{
			nNewPeriod = NVALUE(ptm) + ptm->tm_hour;
			if ( nOldPeriod/100 != nNewPeriod/100) 
			{
				nTimes = 0;
			}else 
			{
				if ((nOldPeriod % 100)/6 != (nNewPeriod % 100)/6) 
				{
					nTimes = 0;
				}
			}
			
			break;
		}

		case EM_LIMIT_BY_DAY: // 角色在某日范围内只能购买固定个数 
		{
			nNewPeriod = NVALUE(ptm);
			if ( nOldPeriod/100 != nNewPeriod/100 ) 
			{
				nTimes = 0;
			}

			break;
		}

		case EM_LIMIT_BY_WEEK: // 角色在某星期范围内只能购买固定个数 
		{
			nNewPeriod = NVALUE(ptm);
			int nDiff = ptm->tm_wday;

			struct tm tmStart, tmEnd;
			time_t nStart = t + 3600 * 24 * nDiff; 	
			struct tm * ptmStart = localtime_r(&nStart, &tmStart);

			time_t nEnd = t + 3600 * 24 * ( 7 - nDiff);
			struct tm *ptmEnd = localtime_r(&nEnd, &tmEnd);
		
			if ( nOldPeriod < NVALUE(ptmStart)  || nOldPeriod >= NVALUE(ptmEnd))
			{
				nTimes = 0;
			}

			break;
		}

		case EM_LIMIT_BY_MONTH: // 角色在某月范围内只能购买固定个数 
		{
			nNewPeriod = NVALUE(ptm);
			if ( nOldPeriod/10000  != nNewPeriod /10000 ) 
			{
				nTimes = 0;
			}

			break;
		}

		case EM_LIMIT_BY_YEAR: // 角色在某年范围内只能购买固定个数 
		{
			nNewPeriod = NVALUE(ptm);
			if ( nOldPeriod/1000000 != nNewPeriod/1000000 )
			{
				nTimes = 0;
			}
				
			break;
		}

		case EM_LIMIT_BY_LEVEL: // 角色在某等级内只能购买固定个数 
		{
			nNewPeriod = tpPlayer->GetLevel();
			if ( nOldPeriod != tpPlayer->GetLevel())
			{
				nTimes = 0;
			}

			break;
		}

		case EM_LIMIT_BY_5LEVEL: // 角色在某5等级范围内只能购买固定个数 
		{
			nNewPeriod = tpPlayer->GetLevel();
			if ( ceil(nOldPeriod/5.0)  != ceil(nNewPeriod/5.0))
			{
				nTimes = 0;
			}

			break;
		}

		case EM_LIMIT_BY_10LEVEL: // 角色在某10等级范围内只能购买固定个数 
		{
			nNewPeriod = tpPlayer->GetLevel();
			if ( ceil(nOldPeriod/10.0)  !=  ceil(nNewPeriod/10.0) )
			{
				nTimes = 0;
			}

			break;
		}
	}

	return std::make_pair(nNewPeriod, nTimes);
}

void CIbStoreModule::IncPurchaseTimes4Player(CEntityPlayer* tpPlayer, CGoods* pGoods, int nNum, time_t t)
{
	LK_ASSERT(tpPlayer != NULL, return);
	std::pair<int, int> ret = GetPurchaseTimes4Player(tpPlayer, pGoods, t);
	int nTimes = ret.second;

	int nUpdateTimes = nTimes + nNum;

	int Key = CPersonalTimesMgr::MakeKey(TIMESMGR_PURCHASE, pGoods->GetGoodsID());
	((CPropertyPlayer *)tpPlayer->GetProperty())->GetTimesMgr()->SetTimes(Key, nUpdateTimes > 0 ? ret.first * 100 + nUpdateTimes : ret.first * 100);
}

void CIbStoreModule::OnMessageLockGoodsResponse( CMessage* pMessage )
{
	if ( pMessage == NULL || pMessage->msgpara() == 0) return;
	CMessageLockGoodsResponse *pLockGoods = (CMessageLockGoodsResponse *) pMessage->msgpara();

	// 交易ID
	CIbTrans* tpTrans = (CIbTrans*) CSceneObjManager::GetSingletonPtr( )->GetObject( pLockGoods->transobjid() );
	if ( tpTrans == NULL ) 
	{
		LOG_ERROR("ibtrans", "On function %s, tpTrans obj_id (%d) about role(%d) does not exist",
				__FUNCTION__, pLockGoods->transobjid(), pLockGoods->roleid());

		CEntityPlayer* tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(pLockGoods->roleid());
		if ( tpPlayer != NULL)
		{
			SendPurchaseError(tpPlayer, ERROR_IBSTORE_INVALID_TRANS );
		}
		return;	
	}

	// 玩家
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(pLockGoods->roleid());
	if ( tpPlayer == NULL) 
	{
		LOG_NOTICE("ibtrans", "When recv lock goods (%d), player(%d) does not exist.", 
				pLockGoods->goodsid(), pLockGoods->roleid());

		// TODO:如果是限次购买物品, 因为玩家已经不在线，所以无法补偿次数了

		// 销毁交易
		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}

	CGoods* pGoods = GetGoods( pLockGoods->goodsid());	
	if ( pGoods == NULL )
	{
		SendPurchaseError(tpPlayer, ERROR_IBITEM_DOESNOT_EXIST );
		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}

	// 锁定失败
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )tpPlayer->GetProperty();
	if (pLockGoods->result() !=  ERROR_IBSTORE_LOCK_SUCCESS)
	{
		SendPurchaseError(tpPlayer, pLockGoods->result());

		// 如果是限次购买物品,扣减次数
		if ( pGoods->GetLimitPerChar() != 0 )
		{
			IncPurchaseTimes4Player(tpPlayer, pGoods, -1 * tpTrans->GetGoodsCount(), tpTrans->GetTransTime());
		}

		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}

	// 如果锁定成功，判断锁定的数目是否与玩家需要的一致
	if ( pLockGoods->lockcount() > 0 && pLockGoods->lockcount() < tpTrans->GetGoodsCount()) 
	{
		LOG_INFO("ibtrans", "In trans(%d) player(%d) need %d goods(%d), but only lock %d. so the number is reset",
				tpTrans->get_id(), pLockGoods->roleid(), tpTrans->GetGoodsCount(), pLockGoods->goodsid(), pLockGoods->lockcount());

		// 重置购买次数
		if ( pGoods->GetLimitPerChar() != 0 )
		{
			IncPurchaseTimes4Player(tpPlayer, pGoods, -1 * (tpTrans->GetGoodsCount() - pLockGoods->lockcount()), tpTrans->GetTransTime());
		}

		tpTrans->SetGoodsCount( pLockGoods->lockcount() );

	}
	
	// 重新判断金钱是否足够
	int nNeedMoney = pGoods->GetDiscount() * tpTrans->GetGoodsCount(); 
	if ( nNeedMoney < 0)
	{
		SendPurchaseError(tpPlayer, ERROR_PLAYER_HASNO_ENOUGHYB);
		return; 
	}

	CYuanBao& yb = pGoods->GetPriceUnit()  == CYuanBao::em_golden_yuanbao ? tpProperty->GoldenYB() : tpProperty->SilverYB();
	if ( yb.remain() < nNeedMoney)
	{
		// 如果是限次购买物品,扣减次数
		if ( pGoods->GetLimitPerChar() != 0 )
		{
			IncPurchaseTimes4Player(tpPlayer, pGoods, -1 * tpTrans->GetGoodsCount(), tpTrans->GetTransTime());
		}

		if (pGoods->GetPriceUnit()  == CYuanBao::em_golden_yuanbao)
		{
			SendPurchaseError(tpPlayer, ERROR_PLAYER_HASNO_ENOUGHYB);
		}
		else
		{
			SendPurchaseError(tpPlayer, ERROR_PLAYER_HASNO_ENOUGHBINDYB);
		}
		return;
	}

	// 锁定金钱
	if ( yb.lock(nNeedMoney) != true )
	{
		// 如果是限次购买物品,扣减次数
		if ( pGoods->GetLimitPerChar() != 0 )
		{
			IncPurchaseTimes4Player(tpPlayer, pGoods, -1 * tpTrans->GetGoodsCount(), tpTrans->GetTransTime());
		}

		SendPurchaseError(tpPlayer, ERROR_PLAYER_HASNO_ENOUGHYB);
		return;
	}

	// 非限购商品直接向数据库发送扣钱请求
	if (Send2DBPurchaseRequest(tpPlayer, tpTrans, pGoods, true) == false)
	{
		yb.rollback(nNeedMoney);	

		// 如果是限次购买物品,扣减次数
		if ( pGoods->GetLimitPerChar() != 0 )
		{
			IncPurchaseTimes4Player(tpPlayer, pGoods, -1 * tpTrans->GetGoodsCount(), tpTrans->GetTransTime());
		}

		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}
	
	yb.commit(nNeedMoney);
}

bool CIbStoreModule::Send2DBPurchaseRequest(CEntityPlayer* pPlayer, CIbTrans* tpTrans, CGoods* pGoods, bool IsYuanbao)
{
	LK_ASSERT(pPlayer != NULL && tpTrans != NULL && pGoods != NULL, return false);

	// 扣钱
	if ( IsYuanbao )
	{
		int nRatingID = WORLD_SERVER_ID(CSceneLogic::GetSingleton().GetWorldID(), CSceneServer::GetSingleton().mThisServerID);
		if ( IS_GLOBAL_SCENESERVER( CSceneLogic::GetSingleton().GetLineID() ) == true ||
			IS_DUNGEONSERVER_LINE( CSceneLogic::GetSingleton().GetLineID() ) == true )
		{
			CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(tpTrans->GetRoleID());
			if ( pPlayer != NULL )
			{
				nRatingID = WORLD_SERVER_ID(pPlayer->GetRoleWorldID(), pPlayer->GetNationality());
			}
		}
		CSceneLogic::GetSingleton().ExecuteSql(em_dbsession_pay_amount, pPlayer->GetCharID(), tpTrans->get_id(),
					pPlayer->GetCharID(), SQL_NEED_CALLBACK, SELECT,
					"call ProcPayAmount(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
					pGoods->GetGoodsID(),
					tpTrans->GetGoodsCount(),
					pGoods->GetPriceUnit(),
					pGoods->GetPriceValue(),
					pPlayer->GetAccountID(),
					pPlayer->GetCharID(),
					pPlayer->GetSex(),
					pPlayer->GetMetier(),
					pPlayer->GetLevel(),            //level
					pGoods->IsPackage() ? 2 : 1,    //package_flag
					pPlayer->GetClientInfo()->mClientAddr,
					pGoods->GetDiscount(),
					nRatingID,
					tpTrans->GetTransTime(),	
					pPlayer->GetGameID()
					);
	}
	else
	{
		CSceneLogic::GetSingleton().ExecuteSql(em_dbsession_pay_amount, pPlayer->GetCharID(), tpTrans->get_id(),
					pPlayer->GetCharID(), SQL_NEED_CALLBACK, SELECT,
					"select 1" );
	}

	return true;
}


// 购买物品异步请求超时
void CIbStoreModule::OnSeessionTimeoutPlayerPayAmount( CSession* pSession )
{
	LK_ASSERT( pSession != NULL , return );

	int nCharID = pSession->GetParam1();
	int nTransID = pSession->GetParam2();

	// 取得交易
	CIbTrans* tpTrans = (CIbTrans*) CSceneObjManager::GetSingletonPtr( )->GetObject( nTransID );
	if ( tpTrans == NULL ) 
	{
		LOG_ERROR("ibtrans", "On function %s, tpTrans obj_id (%d) about role(%d) does not exist",
			__FUNCTION__, nTransID, nCharID);

		//TODO: 需要进行回滚,此时数据库有可能成功也有可能失败
		return;	
	}

	// 记录交易信息
	LOG_FATAL("ibtrans", "ProcUserPay Timeout, Maybe the DB is busy, Pls check the db record and do the compensation for player,"
		"TransDetail(%d): Role(%d) Goods(%d, %d) Item(%d, %d) PriceUnit(%d: %d -> %d) BalanceBeforePurchase(%d)",
		tpTrans->GetTransID(), tpTrans->GetRoleID(), 
		tpTrans->GetGoodsID(), tpTrans->GetGoodsCount(), 
		tpTrans->GetItemID(), tpTrans->GetItemCount(), 
		tpTrans->GetPriceUnit(), tpTrans->GetPriceValue(), tpTrans->GetDiscount(),
		tpTrans->GetBalanceBeforePurchase());

}


void CIbStoreModule::OnSessionPlayerPayAmount(CMessage *tpMessage, int nRoleID, int nTransID)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageExecuteSqlResponse3* tpExecuteSql = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	// 取得交易
	CIbTrans* tpTrans = (CIbTrans*) CSceneObjManager::GetSingletonPtr( )->GetObject( nTransID );
	if ( tpTrans == NULL ) 
	{
		LOG_ERROR("ibtrans", "On function %s, tpTrans obj_id (%d) about role(%d) does not exist",
				__FUNCTION__, nTransID, nRoleID);

		//TODO: 需要进行回滚,此时数据库有可能成功也有可能失败
		return;	
	}


	// 取得交易流水号
	if (tpExecuteSql->resultcode() == 0)
	{
		LOG_ERROR("ibtrans", "ProcUserPay ERROR, TransDetail (%d): Role(%d) Goods(%d, %d) Item(%d, %d) PriceUnit(%d: %d -> %d)",
				tpTrans->GetTransID(), tpTrans->GetRoleID(), 
				tpTrans->GetGoodsID(), tpTrans->GetGoodsCount(), 
				tpTrans->GetItemID(), tpTrans->GetItemCount(), 
				tpTrans->GetPriceUnit(), tpTrans->GetPriceValue(), tpTrans->GetDiscount());

		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}

	// 设置交易流水
	int llPurchaseID = atoi(tpExecuteSql->fieldvalue(0).c_str());
	tpTrans->SetTransID(llPurchaseID);
	
	// 当前数据库余额
	int nDBBalance = 0;
	if( tpExecuteSql->colcount() > 1 )
	{
		nDBBalance = atoi(tpExecuteSql->fieldvalue(1).c_str());
	}

	// 如果交易流水号<0,说明存储过程执行失败,一般是余额不足错误(游戏内余额和数据库内余额对不上 )
	if( llPurchaseID < 0 )
	{
		CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(tpTrans->GetRoleID());
		if( pPlayer != NULL )
		{
			// 向客户端发送消息
			CMessagePurchaseIbItemResponse tFactMsg;
			tFactMsg.set_error(ERROR_PLAYER_HASNO_ENOUGHYB);
			tFactMsg.set_goodsid(tpTrans->GetGoodsID());
			tFactMsg.set_itemid(tpTrans->GetItemID());
			tFactMsg.set_itemcount(tpTrans->GetItemCount());
			CMessage tMsg;
			tMsg.set_msgpara((uint32_t) &tFactMsg);
			pbmsg_sethead(&tMsg, ID_S2C_RESPONSE_PURCHASEIBITEM, 0, 0, 0, 0, 0, 0);

			CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsg);
		}

		// 记录交易信息
		LOG_FATAL("ibtrans", "ProcUserPay Failed, Maybe the yb ballance in the db(%d) is not match with the balance in the game(%d), "
			" TransDetail(%d): Role(%d) Goods(%d, %d) Item(%d, %d) PriceUnit(%d: %d -> %d)",
			nDBBalance, tpTrans->GetBalanceBeforePurchase(),
			tpTrans->GetTransID(), tpTrans->GetRoleID(), 
			tpTrans->GetGoodsID(), tpTrans->GetGoodsCount(), 
			tpTrans->GetItemID(), tpTrans->GetItemCount(), 
			tpTrans->GetPriceUnit(), tpTrans->GetPriceValue(), tpTrans->GetDiscount());

		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}


	// 记录交易信息
	LOG_INFO("ibtrans", "ProcUserPay OK, TransDetail(%d): Role(%d) Goods(%d, %d) Item(%d, %d) PriceUnit(%d: %d -> %d), Balance Before(DB:%d Game:%d)",
			tpTrans->GetTransID(), tpTrans->GetRoleID(), 
			tpTrans->GetGoodsID(), tpTrans->GetGoodsCount(), 
			tpTrans->GetItemID(), tpTrans->GetItemCount(), 
			tpTrans->GetPriceUnit(), tpTrans->GetPriceValue(), tpTrans->GetDiscount(),
			nDBBalance, tpTrans->GetBalanceBeforePurchase());

	int nRatingID = WORLD_SERVER_ID(CSceneLogic::GetSingleton().GetWorldID(), CSceneServer::GetSingleton().mThisServerID);
	if ( IS_GLOBAL_SCENESERVER( CSceneLogic::GetSingleton().GetLineID() ) == true ||
		IS_DUNGEONSERVER_LINE( CSceneLogic::GetSingleton().GetLineID() ) == true )
	{
		CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(tpTrans->GetRoleID());
		if ( pPlayer != NULL )
		{
			nRatingID = WORLD_SERVER_ID(pPlayer->GetRoleWorldID(), pPlayer->GetNationality());
		}
	}
	uint64_t ullTransID = world_trans_id(nRatingID, tpTrans->GetTransID());

	
	// 取得发放物品的模板
	CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(tpTrans->GetItemID());
	if (ptpl == NULL) 
	{
		//SendPurchaseError(pPlayer, ERROR_IBITEM_DOESNOT_EXIST );
		LOG_ERROR("ibtrans", "ProcUserPay ERROR, TransDetail (%d): Role(%d) Goods(%d, %d) Item(%d, %d) PriceUnit(%d: %d -> %d)",
				tpTrans->GetTransID(), tpTrans->GetRoleID(), 
				tpTrans->GetGoodsID(), tpTrans->GetGoodsCount(), 
				tpTrans->GetItemID(), tpTrans->GetItemCount(), 
				tpTrans->GetPriceUnit(), tpTrans->GetPriceValue(), tpTrans->GetDiscount());

		return;
	}

	// 重新计算物品个数
	int nTotalItem = tpTrans->GetGoodsCount() * tpTrans->GetItemCount();
	if ( nTotalItem < 0)
	{
		//SendPurchaseError(pPlayer, ERROR_IBITEM_INVALID_NUMBER);
		LOG_ERROR("ibtrans", "ProcUserPay ERROR, TransDetail (%d): Role(%d) Goods(%d, %d) Item(%d, %d) PriceUnit(%d: %d -> %d)",
				tpTrans->GetTransID(), tpTrans->GetRoleID(), 
				tpTrans->GetGoodsID(), tpTrans->GetGoodsCount(), 
				tpTrans->GetItemID(), tpTrans->GetItemCount(), 
				tpTrans->GetPriceUnit(), tpTrans->GetPriceValue(), tpTrans->GetDiscount());

		return;
	}

	// 取得交易的玩家
	CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(tpTrans->GetRoleID());
	if ( pPlayer == NULL || pPlayer->GetCommState() != CEntityPlayer::STATE_INMAP) 
	{
		int nTotalItem = tpTrans->GetGoodsCount() * tpTrans->GetItemCount();
		LOG_NOTICE("ibtrans", " Player(%d) has left, System will send sysmail attachment %d item_id(%d)", 
				tpTrans->GetRoleID(), nTotalItem, tpTrans->GetItemID(), tpTrans->GetRoleID());

		SendIbItem2OfflinePlayer(tpTrans->GetRoleID(), ptpl, nTotalItem, ullTransID);
		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}

	// 按照堆叠上限发放物品
	if (ptpl->mPileLimit <= 0) ptpl->mPileLimit = 1;
	for( int nLeftNum = nTotalItem; nLeftNum > 0; nLeftNum -= ptpl->mPileLimit)
	{
		int nThisNum = (nLeftNum > ptpl->mPileLimit) ?  ptpl->mPileLimit : nLeftNum;
		CItemObject* pInsItemPile = 
			CPropertyModule::GetSingleton().PlayerInsertItem( pPlayer, tpTrans->GetItemID(), nThisNum);


		if ( pInsItemPile != NULL)
		{
			pInsItemPile->SetPurchaseID( ullTransID ); 

			// 记录日志
			LogEventGetItemByIbStore( pPlayer, pInsItemPile->GetGUID(),
					tpTrans->GetItemID(), nThisNum, tpTrans->GetPriceUnit(), tpTrans->GetDiscount() );
		}else
		{
			LOG_ERROR("ibtrans", "when %s, player(%d) insertgoods(%d * %d)( this %d) failure", 
					__FUNCTION__, pPlayer->GetCharID(), tpTrans->GetGoodsID(), tpTrans->GetGoodsCount(), nThisNum);

			//SendIbItem2OfflinePlayer(tpTrans->GetRoleID(), ptpl, nThisNum, ullTransID);
			const char* pSrcName = "32";
			int nItemID =  tpTrans->GetItemID();
			CMailModule::GetSingleton().SendSysMail(NULL, pPlayer->GetCharID(), NULL, pSrcName,
					pPlayer->GetCharName(), &nItemID, &nThisNum , 1, pPlayer->GetNationality(), 0);
		}
	}

	// 新增日志，方便查询, 记录了交易ID
	LogEventLostMoneyByBuy4NPC(pPlayer, tpTrans->GetPriceUnit(), 
			tpTrans->GetDiscount() * tpTrans->GetGoodsCount(), tpTrans->GetItemID(), nTotalItem, ullTransID);

	// 向网关发送返点信息
	if ( tpTrans->GetPriceUnit() == CYuanBao::em_golden_yuanbao )
		CFriendModule::GetSingleton().SendAddMasterPointRequest( pPlayer, tpTrans->GetDiscount() * tpTrans->GetGoodsCount() );

	// 向客户端发送消息
	CMessagePurchaseIbItemResponse tFactMsg;
	tFactMsg.set_error(0);
	tFactMsg.set_transid(nTransID);
	tFactMsg.set_goodsid(tpTrans->GetGoodsID());
	tFactMsg.set_goodscount(tpTrans->GetGoodsCount());

	tFactMsg.set_itemid(tpTrans->GetItemID());
	tFactMsg.set_itemcount(tpTrans->GetItemCount());

	PBIbMoney* pbcost = tFactMsg.mutable_goodscost();
	pbcost->set_unit(tpTrans->GetPriceUnit());
	pbcost->set_value(tpTrans->GetDiscount() * tpTrans->GetGoodsCount());

	CMessage tMsg;
	tMsg.set_msgpara((uint32_t) &tFactMsg);
	pbmsg_sethead(&tMsg, ID_S2C_RESPONSE_PURCHASEIBITEM, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsg);

	// 发送当前物品促销信息
	SendIbStorePromotion2Player(pPlayer);	

	// 向Erating发送通知
	CMessageUserIbExchangeNotice tIbExchange;

	tIbExchange.set_purchaseid(ullTransID);
	tIbExchange.set_accountid(tpTrans->GetAccountID());
	tIbExchange.set_roleid(nRoleID);
	tIbExchange.set_genderid(tpTrans->GetRoleSex());
	tIbExchange.set_metierid(tpTrans->GetRoleMetier());
	tIbExchange.set_level(tpTrans->GetRoleLevel());
	tIbExchange.set_ratingid(nRatingID);

	char szIbCode[32] = {0};
	snprintf(szIbCode, sizeof(szIbCode), "%d", tpTrans->GetGoodsID());
	tIbExchange.set_ibcode(szIbCode);

	CGoods* pGoods = GetGoods( tpTrans->GetGoodsID());	
	tIbExchange.set_packageflag((pGoods && pGoods->IsPackage()) ? 2 : 1);

	tIbExchange.set_count(tpTrans->GetGoodsCount()); 
	tIbExchange.set_paytime(tpTrans->GetTransTime());
	tIbExchange.set_userip(tpTrans->GetRoleIP());

	tIbExchange.set_subjectid(tpTrans->GetPriceUnit());
	tIbExchange.set_auditflag(1);
	tIbExchange.set_price(tpTrans->GetPriceValue());
	tIbExchange.set_discountprice(tpTrans->GetDiscount());

	//
	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2E_IBEXCHANGE_NOTICE, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tIbExchange);

	CSceneLogic::GetSingleton().Send2GMServer(&tMessage);


	CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
}

void CIbStoreModule::OnMessageSynIbStoreResponse(CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageSynIbStoreResponse* tpResultMsg = (CMessageSynIbStoreResponse*) tpMessage->msgpara();

	if ( m_ppbIbStore != NULL)
	{
		delete [] m_ppbIbStore;
		m_ppbIbStore = NULL;
	}

	if ( m_ppbFocus != NULL)
	{
		delete [] m_ppbFocus;
		m_ppbFocus = NULL;
	}

	// set store
	m_nStore = tpResultMsg->ibstore_size();
	m_ppbIbStore = new PBIbStore [ m_nStore ];
	for ( size_t i = 0; i < m_nStore; ++i)
	{
		m_ppbIbStore[i] = tpResultMsg->ibstore(i);
	}

	// set focus
	m_nFocus = tpResultMsg->focustab_size();
	m_ppbFocus = new PBIbSubTab [ m_nFocus ];
	for ( size_t i = 0; i < m_nFocus; ++ i)
	{
		m_ppbFocus[i] = tpResultMsg->focustab(i);
	}

	// set bulletin
	snprintf(m_szBulletin, sizeof(m_szBulletin)-1, "%s", tpResultMsg->bulletin().c_str());


	// set version
	m_nVersion = tpResultMsg->version();	

	// set goods
	m_tGoods.clear();
	for ( int i = 0; i < tpResultMsg->goods_size(); ++i)
	{
		const PBIbItemOnSale& pbGoods =  tpResultMsg->goods(i);

		CGoods goods;	

		goods.SetGoodsID(pbGoods.goodsid());
		goods.SetGoodsCount(pbGoods.goodscount());
		goods.SetItemID(pbGoods.itemid());
		goods.SetItemCount(pbGoods.itemcount());
		goods.SetPriceUnit(pbGoods.goodsprice().unit());
		goods.SetPriceValue(pbGoods.goodsprice().value());
		goods.SetDiscount(pbGoods.discount().value());
		goods.SetExpiryTime(pbGoods.expirytime());
		goods.SetActive(pbGoods.active());
		goods.SetIsPackage(pbGoods.ispack());
		goods.SetLimitPerChar(pbGoods.limitperchar());
		goods.SetLimitType(pbGoods.limittype());
		goods.SetFlag(pbGoods.flag());

		InsertGoods(goods);
	}
}

void CIbStoreModule:: OnMessageSynFuncStoreResponse(CMessage *tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageSynFuncStoreResponse *tpResultMsg = ( CMessageSynFuncStoreResponse *) tpMessage->msgpara();
	
	m_tFuncConsume.clear();

	for(int i =0; i < tpResultMsg->funcstore_size() ; ++i )
	{
		const PBFuncStore& pbFuncs = tpResultMsg->funcstore(i);
		CFuncConsume funcs;
		funcs.SetFuncID(pbFuncs.funcid());
		funcs.SetFuncName(pbFuncs.funcname().c_str());
		funcs.SetFuncIsShow(pbFuncs.funcshow());
		funcs.SetDefaultType(pbFuncs.defaultconsume());
		funcs.SetFuncUnit(pbFuncs.funcunit());
		funcs.SetFuncUnitType(pbFuncs.funcunittype());
		funcs.SetFuncCountLimit(pbFuncs.funccountlimit());
		if(pbFuncs.consume_size() == 0)
		{
			continue;
		}
		for(int j= 0; j< pbFuncs.consume_size(); ++j )
		{
			const PBConsume& pbConsume = pbFuncs.consume(j);
			if( pbConsume.consumesprice() == -1 )
			{
				funcs.SetFuncValueType(FUNC_CONSUME_FROM_TAMPLATE);
				funcs.SetFuncValue( pbConsume.tamplaterate(), pbConsume.consumetype()  );
			}else if( pbConsume.tamplaterate() == -1)
			{
				funcs.SetFuncValueType(FUNC_CONSUME_NOMARL_PRICE);
				funcs.SetFuncValue(pbConsume.consumesprice(), pbConsume.consumetype() );
			}else{
				continue;
			}
		}
		InsertFuncs(funcs);
	}
	m_bFuncIsLoad = true;
}

bool CIbStoreModule::InsertGoods(CGoods& goods)
{
	return (m_tGoods.insert(value_type(goods.GetGoodsID(), goods))).second;
}

CGoods* CIbStoreModule::GetGoods(int nGoodsID)
{
	iterator iter = m_tGoods.find(nGoodsID);
	return (iter != m_tGoods.end() ) ? &(iter->second) : NULL;
}

bool CIbStoreModule::InsertFuncs(CFuncConsume & func)
{
	return (m_tFuncConsume.insert( funcconsum_t::value_type(func.GetFuncID(), func))).second;
}

CFuncConsume* CIbStoreModule::GetFuncs( int nFuncsID)
{
	funcconsum_t::iterator iter = m_tFuncConsume.find( nFuncsID );
	return (iter != m_tFuncConsume.end() ) ? &(iter->second) :NULL;
}

// 收到网关回应的余额
void CIbStoreModule::OnMessageQueryYuanBaoResponse( CMessage* pMessage)
{
	if ( pMessage == NULL || pMessage->msgpara() == 0) return;
	CMessage4EQueryYuanBaoResponse *pQueryRspn = (CMessage4EQueryYuanBaoResponse *) pMessage->msgpara();

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(pQueryRspn->roleid());
	if ( tpPlayer == NULL) 
	{
		LOG_NOTICE("pro", "When recv query yuanbao, player(%d) does not exist.", pQueryRspn->roleid());
		return;
	}

	if ( pQueryRspn->resultcode() != ERROR_IBSTORE_DRAW_SUCCESS)
	{
		LOG_NOTICE("pro", "query yuanbao error for player(%d) error(%d)", 
				pQueryRspn->roleid(), pQueryRspn->resultcode());


		CPropertyModule::GetSingleton().SendProErrorNotice( tpPlayer, pQueryRspn->resultcode());
		return;
	}

	CMessageQueryGoldenYBResponse tPara;
	for ( int i = 0; i < pQueryRspn->remain_size(); ++i)
	{
		if ( pQueryRspn->remain(i).unit() == CYuanBao::em_linekong_money )
		{
			tPara.set_goldenyb(pQueryRspn->remain(i).value());
		}
	}

	CMessage tMsg;
	tMsg.set_msgpara((uint32_t) &tPara);
	pbmsg_sethead(&tMsg, ID_S2C_RESPONSE_QUERY_GOLDENYB, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMsg);
}

// 收到申请领取的回应
void CIbStoreModule::OnMessageDrawYuanBaoResponse( CMessage* pMessage)
{
	if ( pMessage == NULL || pMessage->msgpara() == 0) return;
		
	CMessage4EDrawYuanBaoResponse *pDrawRspn = (CMessage4EDrawYuanBaoResponse *) pMessage->msgpara();

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(pDrawRspn->roleid());
	if ( tpPlayer == NULL) 
	{
		LOG_NOTICE("pro", "When recv draw yuanbao, player(%d) does not exist.", pDrawRspn->roleid());
		return;
	}

	CMessageDrawGoldenYBResponse tPara;
	tPara.set_errorcode(pDrawRspn->resultcode());

	CMessage tMsg;
	tMsg.set_msgpara((uint32_t) &tPara);
	pbmsg_sethead(&tMsg, ID_S2C_RESPONSE_DRAW_GLODENYB, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMsg);
}

void CIbStoreModule::OnMessageIbStorePromotionNotice( CMessage* pMessage)
{
	if ( pMessage == NULL || pMessage->msgpara() == 0) return;
	m_tPromotion.clear();

	CMessageIbItemRemainNotice * pPromotionList = (CMessageIbItemRemainNotice*) pMessage->msgpara();
	for ( int i = 0; i < pPromotionList->promotion_size(); ++i)
	{
		PBPromotion* pbPromotion = pPromotionList->mutable_promotion(i);
		if ( pbPromotion != NULL)
		{
			CPromotion promotion;
			promotion.SetGoodsID(pbPromotion->goodsid());
			promotion.SetBeginTime(pbPromotion->begintime());
			promotion.SetEndTime(pbPromotion->endtime());
			promotion.SetRemainCount(pbPromotion->remaincount());
			promotion.SetMaxCount(pbPromotion->maxcount());
			// promotion.SetColdTime(pbPromotion->coldtime());
			// promotion.SetElapsedTime(pbPromotion->elapsedtime());
			// promotion.SetColdNum(pbPromotion->coldnum());

			m_tPromotion.insert( promotion_t::value_type(pbPromotion->goodsid(), promotion));
		}
	}
}

void CIbStoreModule::SendIbItem2OfflinePlayer(int nRoleID, CTplItem* ptpl, int nItemCount, uint64_t ullTransID)
{
	if (!ptpl) return;


	CMessageOfflineSysMailNotify  OfflineMail;

	OfflineMail.set_roleid(nRoleID);
	OfflineMail.set_mailtype(32);
	 
	CItemObject* pItem = NULL;	
	// 从对象池上创建的
	CPropertyModule::GetSingleton().CreateItem(ptpl, nItemCount, 0, &pItem); 
	if (pItem != NULL)
	{	
		// 设置交易流水号
		pItem->SetPurchaseID( ullTransID ); 

		// 新增道具
		PBItemObject* ppbItem = OfflineMail.add_attachitem();
		pItem->GetItemBinaryBuffer2(ppbItem);

		// 从对象池上删除
		CSceneObjManager::GetSingletonPtr()->DestroyObject( pItem->get_id() );
	}

	CMessage tMsg;
	tMsg.set_msgpara((uint32_t) &OfflineMail);

	pbmsg_sethead(&tMsg, ID_S2G_NOTIFY_OFFLINEMAIL, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingleton().Send2Gate(&tMsg);
}

void CIbStoreModule::SendIbStorePromotion2Player( CEntityPlayer* pPlayer)
{
	if ( m_tPromotion.size() == 0) return;
	promotion_t::iterator iter = m_tPromotion.begin();

	CMessageIbItemRemainNotice tPromotion;
	for (; iter != m_tPromotion.end(); ++iter)
	{
		PBPromotion* pbPromotion = tPromotion.add_promotion();
		if ( pbPromotion != NULL)
		{
			pbPromotion->set_goodsid(iter->second.GetGoodsID());
			pbPromotion->set_remaincount(iter->second.GetRemainCount());

			pbPromotion->set_begintime(iter->second.GetBeginTime());
			pbPromotion->set_endtime(iter->second.GetEndTime());

			//pbPromotion->set_coldtime(iter->second.GetColdTime());
			//pbPromotion->set_elapsedtime(iter->second.GetElapsedTime());

			pbPromotion->set_maxcount(iter->second.GetMaxCount());
			//pbPromotion->set_coldnum(iter->second.GetColdNum());
		}
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_S2C_NOTIFY_IBSTOREPROMOTION, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tPromotion); 
			
	CSceneLogic::GetSingleton().Send2Player(pPlayer, &tMessage);
}


//附送功能付费错误消息
void CIbStoreModule::SendPurchaseFuncError(CEntityPlayer* tpEntity, int nError)
{
	CMessage tMessge;
	CMessagePurchaseFuncResponse tFactMsg;

	tFactMsg.set_error(nError);

	CMessage tMsg;
	tMsg.set_msgpara((uint32_t) &tFactMsg);
	pbmsg_sethead(&tMsg, ID_S2C_PURCHASE_FUNC_RESPONSE, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingleton().Send2Player( tpEntity, &tMsg);

//	SendIbStorePromotion2Player(tpEntity);
}

//

void CIbStoreModule::PurchaseFuncByMoney( CEntityPlayer * pPlayer,CMessagePurchaseFuncRequest *pPurchaseFunc )
{
		LK_ASSERT(pPlayer != NULL && pPurchaseFunc != NULL, return);

		CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
		int tConsumeType = pPurchaseFunc->consumetype();

		CFuncConsume *func = GetFuncs( pPurchaseFunc->funcid());
		if( func == NULL )
		{
			SendPurchaseFuncError(pPlayer, ERROR_FUNC_DOESNOT_EXIST );
			return;
		}

		int nNeedMoney = 0;
		int nOneNeedMoney = 0;

		if( func->GetFuncValueType() == FUNC_CONSUME_FROM_TAMPLATE && func->GetFuncID() == RELIVE_BY_FUNCCONSUME )
		{
			//检查金钱
			// 扣除金币
			CTemplateReliveTable* tpFeeTable = ( CTemplateReliveTable* ) CDataStatic::GetTemp( TEMP_RELIVETABLE );
			if ( tpFeeTable == NULL )
			{
				LOG_ERROR( "ibtrans", "[ %s : %d ] [ %s ] cannt find template TEMPTYPE = %d", __LK_FILE__, __LINE__, __FUNCTION__, TEMP_RELIVETABLE );
				return ;
			}
			
			nOneNeedMoney = func->GetFuncValue(tConsumeType) * tpFeeTable->mFee[ pPlayer->GetLevel() ]/10000;
		}else{
			
			nOneNeedMoney = func->GetFuncValue(tConsumeType);
		}

		if ( nOneNeedMoney <= 0 ) {
			SendPurchaseError(pPlayer, ERROR_FUNC_USE);
			return;
		} 

		//把最小值设置为 一银
		nOneNeedMoney = nOneNeedMoney >100 ? nOneNeedMoney: 100;
		nNeedMoney = pPurchaseFunc->funccount() * nOneNeedMoney;

		if( tConsumeType == CYuanBao::em_unbind_money )
		{
			int nHaveMoney = tpProperty->GetMoney();
			if( nHaveMoney < nNeedMoney )
			{
				SendPurchaseFuncError( pPlayer, ERROR_FUNC_HASNO_ENOUGHMONEY );
				return;
			}
			

		}else if( tConsumeType == CYuanBao::em_bind_money )
		{
			unsigned int tBaggageMoney = ( unsigned int )( tpProperty->GetMoney() + tpProperty->GetBindMoney() );
			if( tBaggageMoney < nNeedMoney )
			{
				SendPurchaseFuncError( pPlayer, ERROR_FUNC_HASNO_ENOUGHMONEY );
				return;
			}

			
		}else{
			
			LOG_ERROR("ibtrans","[%s,%d][%s] moneytype Error,  error! RoleID:%d, FuncID:%d ", __LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetCharID(),pPurchaseFunc->funcid());
			return;
		}
		
		bool tSuccess = false;
		int tExtraData1 = 0;
		int tExtraData2 = 0;

		switch( func->GetFuncID())
		{
		case RELIVE_BY_FUNCCONSUME:		//复活
			{
				break;
			}
		case DEXP_BY_FUNCCONSUME:	//双倍经验
			{
				for(int i = 0; i < pPurchaseFunc->funccount(); i++ )
				{
					int tResultCode = 0;
					tSuccess = CPropertyModule::GetSingleton().InsertBuff( pPlayer, FUNC_TEMPLATE_DXBUFFID, 0, &tResultCode );
					// 发一个 使用buff不能叠加的消息,用起来很别扭
					if( tResultCode == ERROR_USESKILL_INSERTBUFF_LARGE)
					{
						CPropertyModule::GetSingleton().SendUseItemResponse( pPlayer, tResultCode, 0, 0, 0 );
						tSuccess = false;
					}
					if(tSuccess == false ) break;
				}
				break;
			}
		case BLACKPILLSTIME_BY_FUNCCONSUME:		//乾坤丸（黑狗）双倍时间
			{

				if( tpProperty->GetExpTimeInfo()->GetBlackPillTime() > 0 )
				{
					int nTemplateID =  tpProperty->GetExpTimeInfo()->GetBlackPillID();

					CTplItem *tpOldBlackPills = ( CTplItem * )CDataStatic::SearchTpl( nTemplateID );
					CTplItem *tpBlackPills = (CTplItem *) CDataStatic::SearchTpl( FUNC_TEMPLATE_BlackPillID );
					if(tpOldBlackPills == NULL || tpBlackPills->mCommonID != tpOldBlackPills->mCommonID)
					{
						tSuccess = false;
						int nIndex = 0, tExp = 0, tOffLineTimeConsumed = 0;
						CPropertyModule::GetSingleton().SendUseBlackPillsNotify( pPlayer, tpProperty->GetExpTimeInfo()->GetOffLineTimeLength(), tpProperty->GetExpTimeInfo()->GetBlackPillTime()
							, nIndex, tExp, tOffLineTimeConsumed, tpProperty->GetExpTimeInfo()->GetBlackPillID(), ERROR_BLACKPILLS_DIFFRENTBLACKPILLID );

						break;
					}

				}
				for(int i = 0 ; i < pPurchaseFunc->funccount(); i++ )
				{
					int tRet = CPropertyModule::GetSingleton().ChangeOffLineTimeToExp(pPlayer, -1,FUNC_TEMPLATE_BlackPillID );
					if( tRet != SUCCESS ) {
						tSuccess = false; 
						break;
					} else{ tSuccess = true;}
				}

				break;
			}
		case BAGTIME12_BY_FUNCCONSUME:		//包裹时间
			{
				
				tSuccess = CPropertyModule::GetSingleton().AddBagInvalidTime(pPlayer, pPurchaseFunc->extradata2() ,pPurchaseFunc->extradata1(),12,
														pPurchaseFunc->funccount()* 3600 * 24 * func->GetFuncUnit() );
				if( tSuccess == true )
				{

					//成功，发成功消息 ,这需要发消息。更新包裹的信息
					tExtraData1 = pPurchaseFunc->extradata1();
			//		tExtraData2 = pPurchaseFunc->funccount() * 3600 * 24 * func->GetFuncUnit();
					tExtraData2 = pPurchaseFunc->extradata2();
				}
				break;
			}
		case VIPTIME_BY_FUNCCONSUME:	//VIP时间
			{

				//会员一下就是 30天
				tSuccess = CPropertyModule::GetSingleton().AddVIPTime( pPlayer, pPurchaseFunc->funccount() * 3600 * 24 * func->GetFuncUnit());
				if( tSuccess == true )
				{
					 //加会员时间 ，成功
				}
				
				break;
			}
		case BAGTIME14_BY_FUNCCONSUME:
			{
				tSuccess = CPropertyModule::GetSingleton().AddBagInvalidTime(pPlayer, pPurchaseFunc->extradata2(),pPurchaseFunc->extradata1(),
					14 ,pPurchaseFunc->funccount()* 3600 * 24 * func->GetFuncUnit());
				if( tSuccess == true )
				{

					//成功，发成功消息
					tExtraData1 = pPurchaseFunc->extradata1();
			//		tExtraData2 = pPurchaseFunc->funccount() * 3600 * 24 * func->GetFuncUnit();
					tExtraData2 = pPurchaseFunc->extradata2();
				}
				break;
			}
		case BAGTIME16_BY_FUNCCONSUME:
			{
				tSuccess = CPropertyModule::GetSingleton().AddBagInvalidTime(pPlayer, pPurchaseFunc->extradata2() ,pPurchaseFunc->extradata1(),
					16 ,pPurchaseFunc->funccount() * 3600 * 24 * func->GetFuncUnit());
				if( tSuccess == true )
				{

					//成功，发成功消息
					tExtraData1 = pPurchaseFunc->extradata1();
			//		tExtraData2 = pPurchaseFunc->funccount() * 3600 * 24 * func->GetFuncUnit();
					tExtraData2 = pPurchaseFunc->extradata2();
				}
				break;
			}
		case BAGTIME18_BY_FUNCCONSUME:
			{
				tSuccess = CPropertyModule::GetSingleton().AddBagInvalidTime(pPlayer, pPurchaseFunc->extradata2(),pPurchaseFunc->extradata1(),
					18 ,pPurchaseFunc->funccount() * 3600 * 24 *func->GetFuncUnit() );
				if( tSuccess == true )
				{

					//成功，发成功消息
					tExtraData1 = pPurchaseFunc->extradata1();
			//		tExtraData2 = pPurchaseFunc->funccount() * 3600 * 24 * func->GetFuncUnit();
					tExtraData2 = pPurchaseFunc->extradata2();
				}
				break;
			}

		case PROTECTTIME_BY_FUNCCONSUME:
			{
				for(int i = 0; i < pPurchaseFunc->funccount(); i++ )
				{
					int tResultCode = 0;
					tSuccess = CPropertyModule::GetSingleton().InsertBuff( pPlayer, FUNC_TEMPLATE_PROTECTTIME, 0, &tResultCode );
					// 发一个 使用buff不能叠加的消息,用起来很别扭
					if( tResultCode != SUCCESS)
					{
						CPropertyModule::GetSingleton().SendUseItemResponse( pPlayer, tResultCode, 0, 0, 0 );
						tSuccess = false;
					}
					if(tSuccess == false ) break;
				}
				break;
			}
		default:
			{
				SendPurchaseError(pPlayer, ERROR_FUNC_DOESNOT_EXIST );
				return;
			}
		}

		if( tSuccess == false )
		{
			SendPurchaseFuncError(pPlayer, ERROR_FUNC_USE );
			return;
		}
		if( tConsumeType == CYuanBao::em_unbind_money )
		{
			int nHaveMoney = tpProperty->GetMoney();

			if (CPropertyModule::GetSingleton().PlayerChangeMoney( pPlayer, nNeedMoney, true, false ) != SUCCESS)
			{
				LOG_ERROR("ibtrans","[%s,%d][%s] func by bindmoney Error, change money error! RoleID:%d, FuncID:%d ", __LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetCharID(),pPurchaseFunc->funcid());
				return;
			}

		}else if( tConsumeType == CYuanBao::em_bind_money )
		{
			unsigned int tBaggageMoney = ( unsigned int )( tpProperty->GetMoney() + tpProperty->GetBindMoney() );

			if (CPropertyModule::GetSingleton().PlayerChangeMoney( pPlayer, nNeedMoney, true, true ) != SUCCESS)
			{

				LOG_ERROR("ibtrans","[%s,%d][%s] func by unbind money Error, change money error! RoleID:%d, FuncID:%d ", __LK_FILE__, __LINE__, __FUNCTION__, pPlayer->GetCharID(),pPurchaseFunc->funcid());
				return;
			}

		}else{
			return;
		}

		LogEventLostMoneyByBuy4FuncConsume( pPlayer, tConsumeType,
			nNeedMoney ,func->GetFuncID(), pPurchaseFunc->funccount());
		CMessage tMessge;
		CMessagePurchaseFuncResponse tFactMsg;

		tFactMsg.set_error(0);
		tFactMsg.set_funcid(func->GetFuncID());
		tFactMsg.set_funccount( pPurchaseFunc->funccount() );
		tFactMsg.set_consumetype(pPurchaseFunc->consumetype( ) );
		tFactMsg.set_funcscost( pPurchaseFunc->funccount() * func->GetFuncValue( pPurchaseFunc->consumetype( ) ));
		tFactMsg.set_extradata1( tExtraData1 );
		tFactMsg.set_extradata2( tExtraData2 );


		CMessage tMsg;
		tMsg.set_msgpara((uint32_t) &tFactMsg);
		pbmsg_sethead(&tMsg, ID_S2C_PURCHASE_FUNC_RESPONSE, 0, 0, 0, 0, 0, 0);
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsg);
		return;
}


void CIbStoreModule::PurchaseFuncByYB( CEntityPlayer * pPlayer,CMessagePurchaseFuncRequest *pPurchaseFunc )
{
	LK_ASSERT(pPlayer != NULL && pPurchaseFunc != NULL, return);

	LK_ASSERT(pPlayer != NULL && pPurchaseFunc != NULL, return);

	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tConsumeType = pPurchaseFunc->consumetype();

	CFuncConsume *func = GetFuncs( pPurchaseFunc->funcid());
	if( func == NULL )
	{
		SendPurchaseFuncError(pPlayer, ERROR_FUNC_DOESNOT_EXIST );
		return;
	}
	bool tCheckValid = true;
	switch( func->GetFuncID() )
	{
	case RELIVE_BY_FUNCCONSUME:
		{
	
			break;
		}
	case DEXP_BY_FUNCCONSUME:
		{

			break;
		}
	case BLACKPILLSTIME_BY_FUNCCONSUME:
		{
			if( tpProperty->GetExpTimeInfo()->GetBlackPillTime() > 0 )
			{
				int nTemplateID =  tpProperty->GetExpTimeInfo()->GetBlackPillID();

				CTplItem *tpOldBlackPills = ( CTplItem * )CDataStatic::SearchTpl( nTemplateID );
				CTplItem *tpBlackPills = (CTplItem *) CDataStatic::SearchTpl( FUNC_TEMPLATE_BlackPillID );
				if(tpOldBlackPills == NULL || tpBlackPills->mCommonID != tpOldBlackPills->mCommonID)
				{
					tCheckValid = false;
					int nIndex = 0, tExp = 0, tOffLineTimeConsumed = 0;
					CPropertyModule::GetSingleton().SendUseBlackPillsNotify( pPlayer, tpProperty->GetExpTimeInfo()->GetOffLineTimeLength(), tpProperty->GetExpTimeInfo()->GetBlackPillTime()
						, nIndex, tExp, tOffLineTimeConsumed, tpProperty->GetExpTimeInfo()->GetBlackPillID(), ERROR_BLACKPILLS_DIFFRENTBLACKPILLID );

				}
			
			}
			break;
		}
	case VIPTIME_BY_FUNCCONSUME:
		{

			break;
		}
	case BAGTIME12_BY_FUNCCONSUME:
		{
			break;
		}
	case BAGTIME14_BY_FUNCCONSUME:
		{

			break;
		}
	case BAGTIME16_BY_FUNCCONSUME:
		{

			break;
		}
	case BAGTIME18_BY_FUNCCONSUME:
		{
			
			break;
		}
	case PROTECTTIME_BY_FUNCCONSUME:
		{

			break;
		}
	default:
		{
			SendPurchaseFuncError(pPlayer, ERROR_FUNC_DOESNOT_EXIST );
			return;
		}
	}
	if ( tCheckValid == false)
	{
		SendPurchaseFuncError(pPlayer, ERROR_FUNC_USE );
		return;
	}
	

	//检查金钱
	int nOneNeedMoney = 0;
	int nNeedMoney = 0;

	if( func->GetFuncValueType() == FUNC_CONSUME_FROM_TAMPLATE && func->GetFuncID() == RELIVE_BY_FUNCCONSUME )
	{
		//检查金钱
		// 扣除金币
		CTemplateReliveTable* tpFeeTable = ( CTemplateReliveTable* ) CDataStatic::GetTemp( TEMP_RELIVETABLE );
		if ( tpFeeTable == NULL )
		{
			LOG_ERROR( "ibtrans", "[ %s : %d ] [ %s ] cannt find template TEMPTYPE = %d", __LK_FILE__, __LINE__, __FUNCTION__, TEMP_RELIVETABLE );
			return ;
		}
		//0144F11041100 :功能付费，模板系数改为万分比
		nOneNeedMoney =  func->GetFuncValue(tConsumeType) * tpFeeTable->mFee[ pPlayer->GetLevel() ] /10000;
		
	}else{
		nOneNeedMoney =  func->GetFuncValue(tConsumeType);
	}

	if ( nOneNeedMoney <= 0 ) {
		SendPurchaseError(pPlayer, ERROR_FUNC_USE);
		return;
	} 

	nNeedMoney = pPurchaseFunc->funccount() * nOneNeedMoney;

	// 检查金钱
	CYuanBao& yb = tConsumeType  == CYuanBao::em_golden_yuanbao ? tpProperty->GoldenYB() : tpProperty->SilverYB();
	if ( yb.remain() < nNeedMoney)
	{
		if (tConsumeType  == CYuanBao::em_golden_yuanbao)
		{
			SendPurchaseError(pPlayer, ERROR_PLAYER_HASNO_ENOUGHYB);
		}
		else
		{
			SendPurchaseError(pPlayer, ERROR_PLAYER_HASNO_ENOUGHBINDYB);
		}
		return;
	}

	CFuncTrans* tpTrans = (CFuncTrans*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_FUNCTRANS );

	if( tpTrans == NULL )
	{
		LOG_ERROR("ibtrans", "func's transaction pool is full.");
		SendPurchaseError( pPlayer, ERROR_FUNC_TRANS_OBJ_FULL );
		return;	
	}

	if( tConsumeType == CYuanBao::em_golden_yuanbao || tConsumeType == CYuanBao::em_silver_yuanbao)
	{
		tpTrans->SetFuncsID( func->GetFuncID() );
		tpTrans->SetFuncsCount( pPurchaseFunc->funccount());

		tpTrans->SetAccountID( pPlayer->GetAccountID());
		tpTrans->SetRoleID( pPlayer->GetCharID() );

		tpTrans->SetSex( pPlayer->GetSex() );
		tpTrans->SetPriceType( tConsumeType);
		tpTrans->SetPriceValue( nOneNeedMoney);
		
		tpTrans->SetIP( pPlayer->GetClientInfo()->mClientAddr );
		tpTrans->SetMetier( pPlayer->GetMetier() );
		tpTrans->SetLevel( pPlayer->GetLevel() );
		tpTrans->SetTransTime( time( NULL ));

		tpTrans->SetExtraData( pPurchaseFunc->extradata1(), pPurchaseFunc->extradata2() );

		tpTrans->SetBalanceBeforePurchase( yb.remain() );
		
	}

	// 锁定金钱
	if ( yb.lock( nNeedMoney ) != true )
	{
		SendPurchaseFuncError(pPlayer, ERROR_PLAYER_HASNO_ENOUGHYB);
		return;
	}

	//向数据库发送扣钱请求
	if (Send2DBPurchaseFuncRequest(pPlayer, tpTrans, func) == false)
	{
		yb.rollback( nNeedMoney );	

		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}

	yb.commit(nNeedMoney);

}


bool CIbStoreModule::Send2DBPurchaseFuncRequest(CEntityPlayer* pPlayer, CFuncTrans* tpTrans, CFuncConsume* pFuncConsume)
{
	LK_ASSERT(pPlayer != NULL && tpTrans != NULL && pFuncConsume != NULL, return false);

	// 扣钱
	CSceneLogic::GetSingleton().ExecuteSql(em_dbsession_pay_func, pPlayer->GetCharID(), tpTrans->get_id(),
		pPlayer->GetCharID(), SQL_NEED_CALLBACK, SELECT,
		"call ProcPayFunc(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
		pFuncConsume->GetFuncID(),
		tpTrans->GetFuncsCount(),
		tpTrans->GetPriceType(),
		tpTrans->GetPriceValue(),
		pPlayer->GetAccountID(),
		pPlayer->GetCharID(),
		pPlayer->GetSex(),
		pPlayer->GetMetier(),
		pPlayer->GetLevel(),            //level
		pPlayer->GetClientInfo()->mClientAddr,
		WORLD_SERVER_ID(CSceneLogic::GetSingleton().GetWorldID(), CSceneServer::GetSingleton().mThisServerID),
		tpTrans->GetTransTime(),	
		pPlayer->GetGameID()
		);

	return true;
}



void CIbStoreModule::OnSessionPlayerPayFunc(CMessage *tpMessage, int nRoleID, int nTransID)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageExecuteSqlResponse3* tpExecuteSql = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	// 取得交易
	CFuncTrans* tpTrans = (CFuncTrans*) CSceneObjManager::GetSingletonPtr( )->GetObject( nTransID );
	if ( tpTrans == NULL ) 
	{
		LOG_ERROR("ibtrans", "On function %s, tpTrans obj_id (%d) about role(%d) does not exist",
			__FUNCTION__, nTransID, nRoleID);

		//TODO: 需要进行回滚,此时数据库有可能成功也有可能失败
		return;	
	}



	// 取得交易流水号
	if (tpExecuteSql->resultcode() == 0)
	{
		LOG_ERROR("ibtrans", "ProcUserPay ERROR, TransDetail (%d): Role(%d) Funcs(%d, %d)  PriceUnit(%d: %d -> %d)",
			tpTrans->GetTransID(), tpTrans->GetRoleID(), 
			tpTrans->GetFuncsID(), tpTrans->GetFuncsCount(), 
			tpTrans->GetPriceType(), tpTrans->GetPriceValue(), tpTrans->GetPriceValue());

		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}

	// 设置交易流水
	int llPurchaseID = atoi(tpExecuteSql->fieldvalue(0).c_str());
	tpTrans->SetTransID(llPurchaseID);

	// 当前数据库余额
	int nDBBalance = 0;
	if( tpExecuteSql->colcount() > 1 )
	{
		nDBBalance = atoi(tpExecuteSql->fieldvalue(1).c_str());
	}

	// 如果交易流水号<0,说明存储过程执行失败,一般是余额不足错误(游戏内余额和数据库内余额对不上 )
	if( llPurchaseID < 0 )
	{
		CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(tpTrans->GetRoleID());
		if( pPlayer != NULL )
		{
			// 向客户端发送消息
			CMessagePurchaseFuncResponse tFactMsg;
			tFactMsg.set_error(ERROR_PLAYER_HASNO_ENOUGHYB);
			tFactMsg.set_funcid(tpTrans->GetFuncsID());
			tFactMsg.set_funccount(tpTrans->GetFuncsCount());
			CMessage tMsg;
			tMsg.set_msgpara((uint32_t) &tFactMsg);
			pbmsg_sethead(&tMsg, ID_S2C_PURCHASE_FUNC_RESPONSE, 0, 0, 0, 0, 0, 0);

			CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsg);
		}

		// 记录交易信息
		LOG_FATAL("ibtrans", "ProcFuncPay Failed, Maybe the yb ballance in the db(%d) is not match with the balance in the game(%d), "
			" TransDetail(%d): Role(%d) Goods(%d, %d)  PriceUnit(%d: %d)",
			nDBBalance, tpTrans->GetBalanceBeforePurchase(),
			tpTrans->GetTransID(), tpTrans->GetRoleID(), 
			tpTrans->GetFuncsID(), tpTrans->GetFuncsCount(), 
			tpTrans->GetPriceType(), tpTrans->GetPriceValue());

		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}

	// 记录交易信息
	LOG_INFO("ibtrans", "ProcFuncPay OK, TransDetail(%d): Role(%d) Goods(%d, %d)  PriceType(%d: %d ), Balance Before(DB:%d Game:%d)",
		tpTrans->GetTransID(), tpTrans->GetRoleID(), 
		tpTrans->GetFuncsID(), tpTrans->GetFuncsCount(), 
		tpTrans->GetPriceValue(), tpTrans->GetPriceType(),
		nDBBalance, tpTrans->GetBalanceBeforePurchase());

	int nRatingID = WORLD_SERVER_ID(CSceneLogic::GetSingleton().GetWorldID(), CSceneServer::GetSingleton().mThisServerID);
	if ( IS_GLOBAL_SCENESERVER( CSceneLogic::GetSingleton().GetLineID() ) == true ||
		IS_DUNGEONSERVER_LINE( CSceneLogic::GetSingleton().GetLineID() ) == true )
	{
		CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(tpTrans->GetRoleID());
		if ( pPlayer != NULL )
		{
			nRatingID = WORLD_SERVER_ID(pPlayer->GetRoleWorldID(), pPlayer->GetNationality());
		}
	}
	uint64_t ullTransID = world_trans_id(nRatingID, tpTrans->GetTransID());


	// 取得交易的玩家
	CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(tpTrans->GetRoleID());
	if ( pPlayer == NULL || pPlayer->GetCommState() != CEntityPlayer::STATE_INMAP) 
	{
		LOG_NOTICE("ibtrans", " Func Player(%d) has left, func_Id is (%d),func_price is(%d, %d, %d), ", 
			tpTrans->GetRoleID(),tpTrans->GetFuncsID(),tpTrans->GetPriceValue(), 
			tpTrans->GetFuncsCount(), tpTrans->GetPriceType());

		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}
	CFuncConsume *func = GetFuncs( tpTrans->GetFuncsID());
	if( func == NULL )
	{
		SendPurchaseFuncError(pPlayer, ERROR_FUNC_DOESNOT_EXIST );
		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}
	 bool tSuccess = false;
	 int tExtraData1 = 0;
	 int tExtraData2 = 0;
	 int tExtraData3 = 0;
	switch( tpTrans->GetFuncsID())
	{
	case RELIVE_BY_FUNCCONSUME:		//复活
		{
			if( pPlayer->IsAlive() == false )
			{
				CPropertyModule::GetSingletonPtr( )->PlayerRelive( pPlayer, RELIVE_SYSTEM, 10000, 10000 );
				tSuccess = true;		
			}else{
				LOG_NOTICE("ibtrans", "funcConsume pPlayer  is Alive...player CHARID is (%d)",pPlayer->GetCharID()); 
			}
			break;
		}
	case DEXP_BY_FUNCCONSUME:	//双倍经验
		{
			for(int i = 0; i < tpTrans->GetFuncsCount(); i++ )
			{

				int tResultCode = 0;
				tSuccess = CPropertyModule::GetSingleton().InsertBuff( pPlayer, FUNC_TEMPLATE_DXBUFFID ,0, &tResultCode);
				// 发一个 使用buff不能叠加的消息,用起来很别扭
				if( tResultCode == ERROR_USESKILL_INSERTBUFF_LARGE)
				{
					CPropertyModule::GetSingleton().SendUseItemResponse( pPlayer, tResultCode, 0, 0, 0 );
					tSuccess = false;
				}
				if(tSuccess == false ) break;
			}
			break;
		}
	case BLACKPILLSTIME_BY_FUNCCONSUME:		//乾坤丸（黑狗）双倍时间
		{
			for(int i = 0 ; i < tpTrans->GetFuncsCount(); i++ )
			{
				int tRet = CPropertyModule::GetSingleton().ChangeOffLineTimeToExp(pPlayer, -1,FUNC_TEMPLATE_BlackPillID );
				if( tRet != SUCCESS ) {tSuccess = false; break; }else{ tSuccess = true ; }
			}

			break;
		}
	case BAGTIME12_BY_FUNCCONSUME:		//包裹时间
		{

			tSuccess = CPropertyModule::GetSingleton().AddBagInvalidTime(pPlayer, tpTrans->GetExtraDatat2(),tpTrans->GetExtraDatat1(),
				12 ,tpTrans->GetFuncsCount()* 3600 * 24 * func->GetFuncUnit());
			if( tSuccess == true )
			{

				//成功，发成功消息
				tExtraData1 = tpTrans->GetExtraDatat1();
		//		tExtraData2 = tpTrans->GetFuncsCount()* 3600 * 24 * func->GetFuncUnit();
				tExtraData2 = tpTrans->GetExtraDatat2();
			}
			break;
		}
	case VIPTIME_BY_FUNCCONSUME:	//VIP时间
		{

			//用一下会员就是 1天
			tSuccess = CPropertyModule::GetSingleton().AddVIPTime( pPlayer, tpTrans->GetFuncsCount() * 3600 * 24 * func->GetFuncUnit() );
			if( tSuccess == true )
			{
				//加会员时间 ，失败了
			}

			break;
		}
	case BAGTIME14_BY_FUNCCONSUME:
		{
			tSuccess = CPropertyModule::GetSingleton().AddBagInvalidTime(pPlayer, tpTrans->GetExtraDatat2(),tpTrans->GetExtraDatat1(),
				14 ,tpTrans->GetFuncsCount()* 3600 * 24 * func->GetFuncUnit() );
			if( tSuccess == true )
			{

				//成功，发成功消息
				//成功，发成功消息
				tExtraData1 = tpTrans->GetExtraDatat1();
		//		tExtraData2 = tpTrans->GetFuncsCount()* 3600 * 24 * func->GetFuncUnit();
				tExtraData2 = tpTrans->GetExtraDatat2();
			}
			break;
		}
	case BAGTIME16_BY_FUNCCONSUME:
		{
			tSuccess = CPropertyModule::GetSingleton().AddBagInvalidTime(pPlayer, tpTrans->GetExtraDatat2(),tpTrans->GetExtraDatat1(),
				16 ,tpTrans->GetFuncsCount()* 3600 * 24 * func->GetFuncUnit());
			if( tSuccess == true )
			{

				//成功，发成功消息
				//成功，发成功消息
				tExtraData1 = tpTrans->GetExtraDatat1();
			//	tExtraData2 = tpTrans->GetFuncsCount()* 3600 * 24 * func->GetFuncUnit();
				tExtraData2 = tpTrans->GetExtraDatat2();
			}
			break;
		}
	case BAGTIME18_BY_FUNCCONSUME:
		{
			tSuccess = CPropertyModule::GetSingleton().AddBagInvalidTime(pPlayer, tpTrans->GetExtraDatat2(),tpTrans->GetExtraDatat1(),
				18 ,tpTrans->GetFuncsCount()* 3600 * 24 * func->GetFuncUnit());
			if( tSuccess == true )
			{

				//成功，发成功消息
				//成功，发成功消息
				tExtraData1 = tpTrans->GetExtraDatat1();
			//	tExtraData2 = tpTrans->GetFuncsCount()* 3600 * 24 * func->GetFuncUnit();
				tExtraData2 = tpTrans->GetExtraDatat2();
			}
			break;
		}
	case PROTECTTIME_BY_FUNCCONSUME:
		{
			for(int i = 0; i < tpTrans->GetFuncsCount(); i++ )
			{
				int tResultCode = 0;
				tSuccess = CPropertyModule::GetSingleton().InsertBuff( pPlayer, FUNC_TEMPLATE_PROTECTTIME, 0, &tResultCode );
				// 发一个 使用buff不能叠加的消息,用起来很别扭
				if( tResultCode != SUCCESS)
				{
					CPropertyModule::GetSingleton().SendUseItemResponse( pPlayer, tResultCode, 0, 0, 0 );
					tSuccess = false;
				}
				if(tSuccess == false ) break;
			}
			break;
		}
	default:
		{
			SendPurchaseError(pPlayer, ERROR_FUNC_DOESNOT_EXIST );
			CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
			return;
		}
	}

	if( tSuccess == false )
	{
		LOG_NOTICE("ibtrans", " FuncConsume  Player(%d) has left, func_Id is (%d),func_price is(%d, %d, %d), ", 
			tpTrans->GetRoleID(),tpTrans->GetFuncsID(),tpTrans->GetPriceValue(), 
			tpTrans->GetFuncsCount(), tpTrans->GetPriceType());
		SendPurchaseFuncError(pPlayer, ERROR_FUNC_USE );
		CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
		return;
	}
	//// 新增日志，方便查询, 记录了交易ID
	//LogEventLostMoneyByBuy4NPC(pPlayer, tpTrans->GetPriceUnit(), 
	//	tpTrans->GetDiscount() * tpTrans->GetGoodsCount(), tpTrans->GetItemID(), nTotalItem, ullTransID);



	LogEventLostMoneyByBuy4FuncConsume( pPlayer, tpTrans->GetPriceType(),
			tpTrans->GetFuncsCount() * tpTrans->GetPriceValue() ,tpTrans->GetFuncsID(),tpTrans->GetFuncsCount() );


	// 向网关发送返点信息
	if ( tpTrans->GetPriceType() == CYuanBao::em_golden_yuanbao )
		CFriendModule::GetSingleton().SendAddMasterPointRequest( pPlayer, tpTrans->GetPriceValue() * tpTrans->GetFuncsCount() );

		// 向客户端发送消息
	CMessagePurchaseFuncResponse tFuncMsg;

	tFuncMsg.set_error( 0 );
	tFuncMsg.set_funcid( tpTrans->GetFuncsID() );
	tFuncMsg.set_funccount( tpTrans->GetFuncsCount() );
	tFuncMsg.set_consumetype( tpTrans->GetPriceType() );
	tFuncMsg.set_extradata1( tExtraData1 );
	tFuncMsg.set_extradata2( tExtraData2 );
	//tFuncMsg.mutable_funcscost();

	//PBIbMoney* pbcost = tFuncMsg.mutable_funcscost();
	//pbcost->set_unit(tpTrans->GetPriceType());
	//pbcost->set_value(tpTrans->GetPriceValue() * tpTrans->GetFuncsCount());
	tFuncMsg.set_funcscost(tpTrans->GetPriceValue() * tpTrans->GetFuncsCount());
	CMessage tMsg;
	tMsg.set_msgpara((uint32_t) &tFuncMsg);
	pbmsg_sethead(&tMsg, ID_S2C_PURCHASE_FUNC_RESPONSE, 0, 0, 0, 0, 0, 0);
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsg);
	

	// 向Erating发送通知
	CMessageUserIbExchangeNotice tIbExchange;

	tIbExchange.set_purchaseid(ullTransID);
	tIbExchange.set_accountid(tpTrans->GetAccountID());
	tIbExchange.set_roleid(nRoleID);
	tIbExchange.set_genderid(tpTrans->GetSex());
	tIbExchange.set_metierid(tpTrans->GetMetier());
	tIbExchange.set_level(tpTrans->GetLevel());
	tIbExchange.set_ratingid(nRatingID);

	char szIbCode[32] = {0};
	snprintf(szIbCode, sizeof(szIbCode), "%d", tpTrans->GetFuncsID());
	tIbExchange.set_ibcode(szIbCode);

	CFuncConsume* pFuncs = GetFuncs( tpTrans->GetFuncsID());	
	tIbExchange.set_packageflag( 1 );

	tIbExchange.set_count(tpTrans->GetFuncsCount()); 
	tIbExchange.set_paytime(tpTrans->GetTransTime());
	tIbExchange.set_userip(tpTrans->GetIP());

	tIbExchange.set_subjectid(tpTrans->GetPriceType());
	tIbExchange.set_auditflag(1);
	tIbExchange.set_price(tpTrans->GetPriceValue());
	tIbExchange.set_discountprice(tpTrans->GetPriceValue());

	//
	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2E_IBEXCHANGE_NOTICE, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tIbExchange);

	CSceneLogic::GetSingleton().Send2GMServer(&tMessage);


	CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
}

void CIbStoreModule::OnSessionTimeoutPlayerPayFunc( CSession* pSession )
{
	LK_ASSERT( pSession != NULL , return );

	int nCharID = pSession->GetParam1();
	int nTransID = pSession->GetParam2();

	// 取得交易
	CFuncTrans* tpTrans = (CFuncTrans*) CSceneObjManager::GetSingletonPtr( )->GetObject( nTransID );
	if ( tpTrans == NULL ) 
	{
		LOG_ERROR("ibtrans", "On function %s, tpTrans obj_id (%d) about role(%d) does not exist",
			__FUNCTION__, nTransID, nCharID);

		//TODO: 需要进行回滚,此时数据库有可能成功也有可能失败
		return;	
	}

	// 记录交易信息
	LOG_FATAL("ibtrans", "ProcFuncPay Timeout, Maybe the DB is busy, Pls check the db record and do the compensation for player,"
		"TransDetail(%d): Role(%d) funcs (%d, %d)  Price(%d: %d -> %d) BalanceBeforePurchase(%d)",
		tpTrans->GetTransID(), tpTrans->GetRoleID(), 
		tpTrans->GetFuncsID(), tpTrans->GetFuncsCount(), 
		tpTrans->GetPriceType(), tpTrans->GetPriceValue(), tpTrans->GetPriceValue(),
		tpTrans->GetBalanceBeforePurchase());
	CSceneObjManager::GetSingletonPtr()->DestroyObject(tpTrans->get_id());
}