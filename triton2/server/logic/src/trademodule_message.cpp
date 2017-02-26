
#include "stdafx.hpp"
#include "entity.h"
#include "tradetype.h"
#include "trademessage_pb.hxx.pb.h"
#include "trade.h"
#include "trademodule.h"
#include "logservertype.h"
#include "message_maker.h"
#include "scenelogic.h"
#include "coretype.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

void CTradeModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if( pMessage == NULL || pPlayer == NULL )
	{
		LOG_ERROR( "trade", "Message null ! [%s : %d],[%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	CMessageHead* tpHead = ( CMessageHead* )pMessage->mutable_msghead();
	if ( tpHead == NULL )
	{
		LOG_ERROR( "trade", "Message Head null ! [%s : %d],[%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	switch( tpHead->messageid() )
	{
	case ID_C2S_REQUEST_TRADEINVITE:
		{
			// 交易邀请
			OnMessageTradeInvite( pPlayer, pMessage );
		}
		break;
	case ID_C2S_REQUEST_INVITEREPLY:
		{
			// 邀请答复
			OnMessageTradeInviteProc( pPlayer, pMessage );
		}
		break;
	case ID_C2S_REQUEST_SENDGOODSINFO:
		{
			// 更新道具信息：添加、移除
			OnMessageUpdateTradeGoods( pPlayer, pMessage );
		}
		break;
	case ID_C2S_REQUEST_SENDTRADEMONEY:
		{
			// 更新交易金钱
			OnMessageSendTradeMoney( pPlayer, pMessage );
		}
		break;
	case ID_C2S_REQUEST_LOCKTRADEINFO:
		{
			// 交易锁定
			OnMessageLockTrade( pPlayer, pMessage );
		}
		break;
	case ID_C2S_REQUEST_CLOSETRADE:
		{
			// 关闭交易
			OnMessageCloseTrade( pPlayer, pMessage );
		}
		break;
	case ID_C2S_REQUEST_SUBMITTRADE:
		{
			// 确认交易处理
			OnMessageSubmitTrade( pPlayer, pMessage );
		}
		break;
	default:
		break;
	}
	return ;
}

void CTradeModule::OnMessageTradeInviteProc(CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageTradeInviteReply* pEvent = ( CMessageTradeInviteReply* ) pMessage->msgpara();

	if ( pEvent == NULL )
	{
		LOG_ERROR( "trade", "Message is none! [%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}	

	CTradeManager* ptr_TradeManager = CTradeManager::GetSingletonPtr();
	if ( ptr_TradeManager == NULL )
	{
		return ;
	}

	LOG_DEBUG( "trade", "Begin to process Trade invite." );

	// 交易关系
	TRADE_MAP* tpItem = ptr_TradeManager->GetTradeMap( pEvent->originentityid(), pPlayer->GetEntityID( ) );	
	if( tpItem == NULL )
	{
		LOG_DEBUG( "trade", "OriginID = %d, TargetID = %d", pEvent->originentityid(), pPlayer->GetEntityID( ) );
		return ;
	}

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingletonPtr()->GetEntity( tpItem->mOriginID ); 
	if( tpPlayer == NULL )
	{
		SendErrorNotice( pPlayer, ERROR_TRADE_PLAYERNOTFOUND );
		ptr_TradeManager->EraseTradeMap( tpItem );
		return ;
	}

	// 拒绝交易
	if ( pEvent->code() == emTradeRefuse )
	{
		SendErrorNotice( tpPlayer, ERROR_TRADE_REFUSE );
		ptr_TradeManager->EraseTradeMap( tpItem );
		return ;
	}

	// 忽略：距离太远、玩家不理
	if ( pEvent->code() == emTradeIgnore )
	{
		ptr_TradeManager->EraseTradeMap( tpItem );
		return; 
	}	

	if ( ptr_TradeManager->IsOverUpperLimit( pPlayer->GetEntityID() ) )
	{
		// 自己被邀请的人数超过上限20人
		SendErrorNotice(pPlayer, ERROR_TRADE_INVITEOVERFLOW );
		ptr_TradeManager->EraseTradeMap( tpItem );
		return ;
	}	

	if( tpItem->mTargetID != pPlayer->GetEntityID( ) )
	{
		// 自己没有被邀请
		ptr_TradeManager->EraseTradeMap( tpItem );

		LOG_DEBUG( "trade", "TradeMap is deleted.[%d]<->[%d]. [%s : %d]", pEvent->originentityid(), pPlayer->GetEntityID(), 
			__LK_FILE__, __LINE__ );
		return ;
	}	

	// 交易人已经在交易中
	if( ptr_TradeManager->GetTrade( pPlayer->GetEntityID( ) ) != NULL || 
		ptr_TradeManager->GetTrade( tpItem->mOriginID ) != NULL ) 
	{
		//ptr_TradeManager->OnSendTradeMsgCallBack( pPlayer, pPlayer->GetEntityID(), ERROR_TRADE_INTRADE );
		SendErrorNotice( pPlayer, ERROR_TRADE_INTRADE );
		ptr_TradeManager->EraseTradeMap( tpItem );

		LOG_DEBUG( "trade", "Players may be in trade.[%s : %d]", __LK_FILE__, __LINE__ );

		LOG_DEBUG( "trade", "TradeMap is deleted.[%d]<->[%d]. [%s : %d]", pEvent->originentityid(), pPlayer->GetEntityID(), 
			__LK_FILE__, __LINE__ );
		return ;
	}

	// 非玩家间交易
	if ( pPlayer->GetEntityType() != ENTITYTYPE_PLAYER  )
	{		
		SendErrorNotice( pPlayer, ERROR_TRADE_PLAYERNOTFOUND );
		ptr_TradeManager->EraseTradeMap( tpItem );
		return ;
	}
	else if ( tpPlayer->GetEntityType() != ENTITYTYPE_PLAYER )
	{
		SendErrorNotice( pPlayer, ERROR_TRADE_PLAYERNOTFOUND );
		ptr_TradeManager->EraseTradeMap( tpItem );
		return ;
	}

	// 玩家不在有效距离
	if( !ptr_TradeManager->IsValidDistance( tpPlayer, pPlayer ) )
	{
		SendErrorNotice( pPlayer, ERROR_TRADE_FAR );
		ptr_TradeManager->EraseTradeMap( tpItem );
		LOG_DEBUG( "trade", "Players stand out validation distance.[%s : %d]", __LK_FILE__, __LINE__ );
		return ;
	}

	/*if ( CheckPlayerService( pPlayer, tpPlayer ) != 0 )
	{
	ptr_TradeManager->EraseTradeMap( tpItem );
	return ;
	}	*/	

	CTrade* tpTrade = ( CTrade* )ptr_TradeManager->CreateTrade( tpPlayer->GetEntityID( ), pPlayer->GetEntityID( ) );
	if ( tpTrade == NULL )
	{
		LOG_ERROR( "trade", "CreateObject()failed ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		ptr_TradeManager->EraseTradeMap( tpItem );
		return ;
	}

	ptr_TradeManager->AddTrade( tpTrade );
	tpTrade->ChangeTradeState( tpPlayer->GetEntityID( ), emInTrade );
	tpTrade->ChangeTradeState( pPlayer->GetEntityID( ), emInTrade );

	OnSendTradeBeginCallBack( tpPlayer, pPlayer->GetEntityID( ) );
	OnSendTradeBeginCallBack( pPlayer, tpPlayer->GetEntityID( ) );

	// 交易创建，邀请关系清除
	ptr_TradeManager->DeleteAllInvite( pPlayer->GetEntityID() );

	// set trade service status
	CPropertyPlayer* tpSrcProperty = ( CPropertyPlayer* ) pPlayer->GetProperty(); 
	CPropertyPlayer* tpDesProperty = ( CPropertyPlayer* ) tpPlayer->GetProperty();
	tpSrcProperty->SetServiceStatus( SERVICE_TRADE );
	tpDesProperty->SetServiceStatus( SERVICE_TRADE );
	LOG_DEBUG( "trade", "Trade is created, All invite maps removed.[%s : %d]", __LK_FILE__, __LINE__ );
	return ;
}
// 交易邀请
void CTradeModule::OnMessageTradeInvite( CEntityPlayer *pPlayer, CMessage *pMessage )
{	
	CMessageTradeInvite* pEvent = ( CMessageTradeInvite* )pMessage->msgpara();
	if ( pEvent == NULL )
	{
		LOG_ERROR( "trade", "Message is none! [%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	LK_ASSERT( pPlayer != NULL, return )

		// 检查是否有密码保护
		CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}	

	CTradeManager* ptr_TradeManager = CTradeManager::GetSingletonPtr();
	if ( ptr_TradeManager == NULL )
	{
		return ;
	}

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingletonPtr()->GetEntity( pEvent->targetentityid() );

	// 玩家不存在
	if( tpPlayer == NULL )
	{		
		SendErrorNotice( pPlayer, ERROR_TRADE_PLAYERNOTFOUND );
		return;
	}  	

	// 检查是否有密码保护
	CPropertyPlayer *tpDesProperty = ( CPropertyPlayer * )tpPlayer->GetProperty();
	tPwdCheckRet = tpDesProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{		
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERR_PASSWORD_DESINPROTECTSTATUS );	
		return;
	}	



	/*if ( CheckPlayerService( pPlayer, tpPlayer ) != LK_ZERO )
	{
	return ;
	}*/		

	// 非玩家间交易
	if ( pPlayer->GetEntityType() != ENTITYTYPE_PLAYER  )
	{		
		SendErrorNotice( pPlayer, ERROR_TRADE_PLAYERNOTFOUND );
		return ;
	}
	else if ( tpPlayer->GetEntityType() != ENTITYTYPE_PLAYER )
	{
		SendErrorNotice( pPlayer, ERROR_TRADE_PLAYERNOTFOUND );
		return ;
	}	

	// 判断是否是自己
	if ( pPlayer->GetEntityID( ) == tpPlayer->GetEntityID( ) )
	{
		SendErrorNotice( pPlayer, ERROR_TRADE_INVITESELF );
		return ;
	}

	if ( IS_GLOBAL_SCENESERVER( pPlayer->GetLineID() ) || IS_DUNGEONSERVER_LINE( pPlayer->GetLineID() ) )
	{
		SendErrorNotice( pPlayer, EDUNGEON_ERROR_NOPLACEACTION );
		return ;
	}

	// check if the nationality diff
	if ( !IS_NEUTRAL_LINE_ID( pPlayer->GetLineID() ) && pPlayer->GetNationality() != tpPlayer->GetNationality() )
	{
		SendErrorNotice( pPlayer, ERROR_TRADE_DIFFNATION );
		return ;
	}

	// 安全时间判断 开发阶段暂设10秒保护
	time_t tNow = time(NULL);

	unsigned int tSaftyTime = CSceneCfgManager::GetSingleton().GetTrade_cfg()->mSaftytime;
	if ( tNow - pPlayer->GetLoginTime() < tSaftyTime || tNow - tpPlayer->GetLoginTime() < tSaftyTime )
	{
		SendErrorNotice( pPlayer, ERROR_TRADE_SAFETIME );
		return ;
	}

	if ( ptr_TradeManager->IsInviteRepeated( pPlayer->GetEntityID( ), tpPlayer->GetEntityID( ) ) )
	{
		// 重复邀请
		SendErrorNotice( pPlayer, ERROR_TRADE_INVITEREPEATED );
		LOG_DEBUG( "trade", "Player[%d] has invite target[%d]! [%s : %d]",pPlayer->GetEntityID(), pEvent->targetentityid(), 
			__LK_FILE__, __LINE__ );
		return ;
	}

	if ( ptr_TradeManager->IsOverUpperLimit( tpPlayer->GetEntityID() ) )
	{
		// 邀请人数超过上限
		SendErrorNotice( pPlayer, ERROR_TRADE_INVITEOVERFLOW );
		LOG_DEBUG( "trade", "Target Player[%d] is invited more than 20 times! [%s : %d]",pEvent->targetentityid(), 
			__LK_FILE__, __LINE__ );
		return ;
	}

	if( ptr_TradeManager->IsBusy( pPlayer->GetEntityID( ) ) )
	{
		// 自己已经在交易
		SendErrorNotice( pPlayer, ERROR_TRADE_INBUSY );
		LOG_DEBUG( "trade", "Origin Player[%d] is busy! [%s : %d]", pPlayer->GetEntityID(), __LK_FILE__, __LINE__ );
		return;
	}

	if ( ptr_TradeManager->IsBusy( tpPlayer->GetEntityID( ) ) )
	{
		// 对方已经在交易
		SendErrorNotice( pPlayer, ERROR_TRADE_INBUSY );
		LOG_DEBUG( "trade", "Target Player[%d] is busy! [%s : %d]",pEvent->targetentityid(), __LK_FILE__, __LINE__ );
		return;
	}


	if( !ptr_TradeManager->IsValidDistance( pPlayer, tpPlayer ) )
	{
		// 超出有效距离
		SendErrorNotice( pPlayer, ERROR_TRADE_FAR );
		LOG_DEBUG( "trade", "Target Player[%d] is out of validation distance! [%s : %d]",pEvent->targetentityid(), 
			__LK_FILE__, __LINE__ );
		return;
	}

	// 增添交易人关系
	ptr_TradeManager->InsertTradeMap( pPlayer->GetEntityID( ), tpPlayer->GetEntityID( ) );

	LOG_DEBUG( "trade", "TradeMap is created.[%d]<->[%d]! [%s : %d]",pPlayer->GetEntityID(), pEvent->targetentityid(), 
		__LK_FILE__, __LINE__ );

	OnSendTradeInviteCallBack( tpPlayer, pPlayer->GetEntityID( ) );
	SendErrorNotice( pPlayer, ERROR_TRADE_INVITE_SUCCESS );
	LOG_DEBUG( "trade", "Trade Invite sent out. [%s : %d]",	__LK_FILE__, __LINE__ );

	return ;
}

void CTradeModule::OnMessageUpdateTradeGoods( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageSendGoodsInfo* pEvent = ( CMessageSendGoodsInfo* )pMessage->msgpara();
	if ( pEvent == NULL )
	{
		LOG_ERROR( "trade", "Message is none! [%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	CTrade* tpTrade = CTradeManager::GetSingletonPtr()->GetTrade( pPlayer->GetEntityID( ) );
	if( tpTrade == NULL )
	{
		return ;
	}
	
	CPropertyPlayer *tpProperty= ( CPropertyPlayer * )pPlayer->GetProperty();
	if ( tpProperty->GetBaggage()->CheckIndexValid( pEvent->itemindex(), true ) == false )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TRADE_INEXPIREDKITBAG );
		return;
	}
	

	CTradeManager::GetSingletonPtr()->UpdateTradeInfo( pPlayer, pEvent->itemindex(), pEvent->tradeindex(),
		pEvent->count(), (TRADE_TYPE)pEvent->tradetype() );

	LOG_DEBUG( "trade", "Trade Goods update,type[%d]. [%s : %d]", pEvent->tradetype(), __LK_FILE__, __LINE__ );
	return ;
}

void CTradeModule::OnMessageSendTradeMoney( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageSendTradeMoney* pEvent = ( CMessageSendTradeMoney* )pMessage->msgpara();
	if ( pEvent == NULL )
	{
		LOG_ERROR( "trade", "Message is none! [%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	CTradeManager::GetSingletonPtr()->UpdateTradeInfo( pPlayer, 0, 0, pEvent->money(), emUpdateMoney );

	return ;
}

void CTradeModule::OnMessageLockTrade( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageLockTradeInfo* pEvent = ( CMessageLockTradeInfo* )pMessage->msgpara();
	if ( pEvent == NULL )
	{
		LOG_ERROR( "trade", "Message is none! [%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	CTradeManager* ptr_TradeManager = CTradeManager::GetSingletonPtr();
	if ( ptr_TradeManager == NULL )
	{
		LOG_ERROR( "trade", "TradeManager pointer is null, [%s : %d]", __LK_FILE__, __LINE__ );
		return ;
	}

	CTrade* tpTrade = ptr_TradeManager->GetTrade( pPlayer->GetEntityID( ) );
	if( tpTrade == NULL )
	{
		LOG_ERROR( "trade", "GetTrade() failed, [%s : %d]", __LK_FILE__, __LINE__ );
		return ;
	}

	if( pEvent->lockstate() != true )
	{
		return ;
	}

	ptr_TradeManager->LockTradeInfo( pPlayer->GetEntityID( ) );

	return ;
}

void CTradeModule::OnMessageCloseTrade( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageCloseTrade* pEvent = ( CMessageCloseTrade* )pMessage->msgpara();
	if ( pEvent == NULL )
	{
		LOG_ERROR( "trade", "Message is none! [%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	LOG_DEBUG( "trade", "Rev msg from client to close trade. [%s : %d]", __LK_FILE__, __LINE__ );

	CTrade* tpTrade = CTradeManager::GetSingletonPtr()->GetTrade( pPlayer->GetEntityID( ) );
	if( tpTrade == NULL )
	{
		//CTradeManager::GetSingletonPtr()->OnSendTradeMsgCallBack( pPlayer, pPlayer->GetEntityID(), ERROR_TRADE_GOODSNOTFOUND );
		SendErrorNotice( pPlayer, ERROR_TRADE_OFFTRADE );
		return ;
	}

	CEntityPlayer* tpSrcPlayer = ( CEntityPlayer* )CSceneLogic::GetSingleton().GetEntity( tpTrade->mOrigin.mEntityID );
	CEntityPlayer* tpDesPlayer = ( CEntityPlayer* )CSceneLogic::GetSingleton().GetEntity( tpTrade->mTarget.mEntityID );
	if ( tpSrcPlayer == NULL || tpDesPlayer == NULL )
	{
		LOG_ERROR( "trade", "EntityPlayer get failed.[%s : %d]", __LK_FILE__, __LINE__ );
		return ;
	}

	int tRes = CTradeManager::GetSingletonPtr()->EndTrade( pPlayer->GetEntityID( ), (TRADE_CLOST_TYPE)pEvent->code() );
	if ( tRes < 0 )
	{
		SendErrorNotice( pPlayer, ERROR_TRADE_FAILURE );
	}	

	// remove trade service status
	( (CPropertyPlayer*) tpSrcPlayer->GetProperty() )->SetServiceStatus( SERVICE_NONE );
	( (CPropertyPlayer*) tpDesPlayer->GetProperty() )->SetServiceStatus( SERVICE_NONE );

	return ;
}

void CTradeModule::OnMessageSubmitTrade(CEntityPlayer *pPlayer, CMessage *pMessage)
{
	CMessageSubmitTrade* pEvent = ( CMessageSubmitTrade* )pMessage->msgpara();
	if ( pEvent == NULL )
	{
		LOG_ERROR( "trade", "MessageBody is none, [%s : %d]", __LK_FILE__, __LINE__ );
		return ;
	}

	CTradeManager* ptr_TradeManager = CTradeManager::GetSingletonPtr();
	if ( ptr_TradeManager == NULL )
	{
		return ;
	}

	// 非正常确认交易
	if ( pEvent->code() != emNormalFinish )
	{
		//ptr_TradeManager->OnSendTradeMsgCallBack( pPlayer, pPlayer->GetEntityID(), ERROR_TRADE_REFUSE );
		SendErrorNotice( pPlayer, ERROR_TRADE_REFUSE );
		LOG_DEBUG( "trade", "Submit code is [%d],[%s : %d]", pEvent->code(), __LK_FILE__, __LINE__ );
		return ;
	}

	int tTradeResult = 0;

	CTrade* tpTrade = ptr_TradeManager->GetTrade( pPlayer->GetEntityID( ) );
	if ( tpTrade == NULL )
	{
		//ptr_TradeManager->OnSendTradeMsgCallBack( pPlayer, pPlayer->GetEntityID(), ERROR_TRADE_OFFTRADE );
		SendErrorNotice( pPlayer, ERROR_TRADE_OFFTRADE );
		return ;
	}

	int tDesEntityID = tpTrade->GetTargetEntityID( pPlayer->GetEntityID() );
	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingletonPtr()->GetEntity( tDesEntityID );
	if ( tpPlayer == NULL )
	{
		SendErrorNotice( pPlayer, ERROR_TRADE_PLAYERNOTFOUND );
		return ;
	}

	if( !ptr_TradeManager->IsValidDistance( pPlayer, tpPlayer ) )
	{
		// 超出有效距离
		SendErrorNotice( pPlayer, ERROR_TRADE_FAR );
		SendErrorNotice( tpPlayer, ERROR_TRADE_FAR );
		LOG_DEBUG( "trade", "Target Player[%d] is out of validation distance! [%s : %d]",tpPlayer->GetCharID(), 
			__LK_FILE__, __LINE__ );
		return;
	}

	// 交易处理的返回结果
	tTradeResult = ptr_TradeManager->ProcTrade( pPlayer->GetEntityID( ) );

	bool bBreakOff = false; // 结束交易 ? 中断

	switch( tTradeResult )
	{
	case EXCHAGE_SUCCESS:
		{
			// 通知客户端，交易结束，关闭交易
			ptr_TradeManager->OnSendTradeCloseCallBack( pPlayer->GetEntityID(), emNormalFinish );
			ptr_TradeManager->OnSendTradeCloseCallBack( tDesEntityID, emNormalFinish );

			LOG_DEBUG( "trade", "Exchange success,[%s : %d]", __LK_FILE__, __LINE__ );
			break;
		}
	case EXCHAGE_SRCOUTOFSPACE:
		{
			// 信息提示交易中断，包裹空间不够
			SendErrorNotice( pPlayer, ERROR_TRADE_SRCBAGLESS );
			SendErrorNotice( tpPlayer, ERROR_TRADE_DESBAGLESS );

			// 通知客户端，交易中断，关闭交易
			ptr_TradeManager->OnSendTradeCloseCallBack( pPlayer->GetEntityID(), emInterrupt );
			ptr_TradeManager->OnSendTradeCloseCallBack( tpPlayer->GetEntityID(), emInterrupt );		
			tpTrade->UnLockTrade( pPlayer->GetEntityID( ) );

			LOG_DEBUG( "trade", "Exchange failed, Source space not enough,[%s : %d]", __LK_FILE__, __LINE__ );
			break;
		}
	case EXCHAGE_DESOUTOFSPACE:
		{
			// 信息提示交易中断，包裹空间不够
			SendErrorNotice( pPlayer, ERROR_TRADE_DESBAGLESS );
			SendErrorNotice( tpPlayer, ERROR_TRADE_SRCBAGLESS );

			// 通知客户端，交易中断，关闭交易
			ptr_TradeManager->OnSendTradeCloseCallBack( pPlayer->GetEntityID(), emInterrupt );
			ptr_TradeManager->OnSendTradeCloseCallBack( tpPlayer->GetEntityID(), emInterrupt );
			tpTrade->UnLockTrade( pPlayer->GetEntityID( ) );

			LOG_DEBUG( "trade", "Exchange failed, Destination space not enough,[%s : %d]", __LK_FILE__, __LINE__ );
			break;
		}
	case emWaitTrade:
		{
			bBreakOff = true;
			LOG_DEBUG( "trade", "Self click submit, waiting for target to submit trade,[%s : %d]", __LK_FILE__, __LINE__ );
			break;
		}
	default:
		{
			//bBreakOff = true;
			SendErrorNotice( pPlayer, ERROR_TRADE_FAILURE );
			// 通知客户端，交易中断，关闭交易
			ptr_TradeManager->OnSendTradeCloseCallBack( pPlayer->GetEntityID(), emInterrupt );
			ptr_TradeManager->OnSendTradeCloseCallBack( tpPlayer->GetEntityID(), emInterrupt );
			break;
		}
	}

	if ( bBreakOff == true )
	{	
		return ;
	}

	// 清空维护的交易物品列表
	tpTrade->ClearGoods();

	// 交易结束，改变玩家交易属性
	tpTrade->ChangeTradeState( pPlayer->GetEntityID(), emOffTrade );
	tpTrade->ChangeTradeState( tpPlayer->GetEntityID(), emOffTrade );

	// 删除交易对象
	ptr_TradeManager->DeleteTrade( tpTrade );

	// remove service status
	( ( CPropertyPlayer* ) pPlayer->GetProperty() )->SetServiceStatus( SERVICE_NONE );
	( ( CPropertyPlayer* ) tpPlayer->GetProperty() )->SetServiceStatus( SERVICE_NONE );

	LOG_DEBUG( "trade", "Trade Finished.Goods list clear, and state to emOffTrade. [%s : %d]", __LK_FILE__, __LINE__ );
	return ;
}
void CTradeModule::OnRouterMessage( CMessage* pMessage )
{

}