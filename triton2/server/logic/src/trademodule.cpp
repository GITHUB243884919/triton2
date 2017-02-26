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

template<> CTradeModule* CSingleton< CTradeModule >::spSingleton = NULL;
char CTradeModule::sModuleName[ NAME_LENGTH ] = "Trade_1";

CTradeManager* gpTradeManager = NULL;

unsigned int CTradeModule::CountSize()
{
	unsigned int tSize = sizeof( CTradeModule );
	tSize += CTradeManager::CountSize( );

	return tSize;
}

void CTradeModule::OnLaunchServer( )
{
	CTradeManager::mShmPtr = mShmPtr;
	gpTradeManager = new CTradeManager;
	if( gpTradeManager == NULL )
	{
		LOG_ERROR( "default", "[%s:%d]Create TradeManager Faild", __FUNCTION__, __LINE__ );
		printf( "Create TradeManager Faild, So Exit\n");
		exit(1);
	}

	OnLoadConfig();
}

void CTradeModule::OnLoadConfig()
{
	TRADE_LOG* tpLog = CSceneCfgManager::GetSingletonPtr()->GetTrade_log();
	if( tpLog != NULL )
	INIT_ROLLINGFILE_LOG( tpLog->mIdentifier, tpLog->mDirectory, ( LogLevel ) tpLog->mPriority , tpLog->mMaxfilesize, tpLog->mMaxbackupindex);	
}

void CTradeModule::OnExitServer( )
{
	CTradeManager::GetSingleton().DoClean();
}

void CTradeModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{	
	// 每秒检查一次交易邀请是否失效
	if ( mTimePile >= 0 )
	{
		mTimePile -= vTickOffset;
		if ( mTimePile <= 0 )
		{
			mTimePile = 1000;
			if ( CTradeManager::GetSingletonPtr() == NULL )
			{
				return ;
			}
			time_t tNow = time( NULL );
			CTradeManager::GetSingletonPtr()->CheckValidInvite( (long)tNow );			
		}
	}	
}


int CTradeModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )
{
	return SUCCESS;
}


void CTradeModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	return;
}


void CTradeModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{
	if ( pEntity->IsPlayer() == false )
	{
		return ;
	}
	CEntityPlayer* pPlayer = ( CEntityPlayer* )pEntity;

	CTradeManager* ptr_TradeManager = CTradeManager::GetSingletonPtr();
	if ( ptr_TradeManager == NULL )
	{
		return ;
	}

	if ( ptr_TradeManager->IsInTradeMap( pPlayer->GetEntityID() ) )
	{
		ptr_TradeManager->DeleteAllInvite( pPlayer->GetEntityID() );
		return ;
	}

	CTrade* tpTrade = ptr_TradeManager->GetTrade( pPlayer->GetEntityID( ) );
	if ( tpTrade != NULL )
	{
		// 通知客户端，交易中断
		ptr_TradeManager->OnSendTradeCloseCallBack( tpTrade->mOrigin.mEntityID, emInterrupt );
		ptr_TradeManager->OnSendTradeCloseCallBack( tpTrade->mTarget.mEntityID, emInterrupt );

		// 改变玩家交易状态
		tpTrade->ChangeTradeState( tpTrade->mOrigin.mEntityID, emOffTrade );
		tpTrade->ChangeTradeState( tpTrade->mTarget.mEntityID, emOffTrade );

		// 解锁交易信息
		tpTrade->UnLockTrade( pPlayer->GetEntityID( ) );

		// 删除交易对象
		ptr_TradeManager->DeleteTrade( tpTrade );

		CEntityPlayer* tpSrcPlayer = ( CEntityPlayer* )CSceneLogic::GetSingleton().GetEntity( tpTrade->mOrigin.mEntityID );
		CEntityPlayer* tpDesPlayer = ( CEntityPlayer* )CSceneLogic::GetSingleton().GetEntity( tpTrade->mTarget.mEntityID );
		if ( tpSrcPlayer == NULL || tpDesPlayer == NULL )
		{
			LOG_ERROR( "trade", "EntityPlayer get failed.[%s : %d]", __LK_FILE__, __LINE__ );
			return ;
		}

		//// 通知客户端，信息提示交易中断
		//SendErrorNotice( tpSrcPlayer, ERROR_TRADE_OFFLINE );
		//SendErrorNotice( tpDesPlayer, ERROR_TRADE_OFFLINE );

		// remove trade service status
		( (CPropertyPlayer*) tpSrcPlayer->GetProperty() )->SetServiceStatus( SERVICE_NONE );
		( (CPropertyPlayer*) tpDesPlayer->GetProperty() )->SetServiceStatus( SERVICE_NONE );
	}
	return ;
}


bool CTradeModule::OnSaveData( CEntityPlayer *pPlayer, int vSaveCode )
{
	return true;
}

void CTradeModule::OnSendTradeInviteCallBack(CEntityPlayer* pPlayer, unsigned int vEntityID)
{
	if ( pPlayer == NULL )
	{
		LOG_ERROR( "trade", "Parameter is none ! [%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CMessage tMessage;
	CMessageHead* tpHead = ( CMessageHead* )tMessage.mutable_msghead();

	tpHead->set_messageid( ID_S2C_NOTIFY_INVITERTN );
	CMessageTradeInviteCallBack tPara;
	tPara.set_originentityid( vEntityID );
	tMessage.set_msgpara( (long)&tPara );

	if ( CSceneLogic::GetSingletonPtr() == NULL )
	{
		LOG_ERROR( "trade", "SceneLogic pointer null ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &tMessage );
	return ;
}



void CTradeModule::OnSendTradeBeginCallBack(CEntityPlayer* pPlayer, unsigned int vEntityID )
{
	if ( pPlayer == NULL )
	{
		LOG_ERROR( "trade", "Parameter is none ! [%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CMessage tMessage;
	CMessageHead* tpHead = ( CMessageHead* )tMessage.mutable_msghead();

	tpHead->set_messageid( ID_S2C_NOTIFY_TRADEBEGIN );
	CMessageNotifyTradeBegin tPara;
	tPara.set_targetentityid( vEntityID );
	tMessage.set_msgpara( (long)&tPara );

	if ( CSceneLogic::GetSingletonPtr() == NULL )
	{
		LOG_ERROR( "trade", "SceneLogic pointer null ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &tMessage );
	return ;
}


void CTradeModule::OnCancelTrade( int vEntityID )
{
	LK_ASSERT( vEntityID > 0, return );
	
	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingletonPtr()->GetEntity( vEntityID ); 
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "trade", "GetEntity() failed. [%s : %d]", __LK_FILE__, __LINE__ );
		return ;
	}
	
	CTradeManager* ptr_TradeManager = CTradeManager::GetSingletonPtr();
	if ( ptr_TradeManager == NULL )
	{
		return ;
	}

	if ( ptr_TradeManager->IsInTradeMap( tpPlayer->GetEntityID() ) )
	{
		ptr_TradeManager->DeleteAllInvite( tpPlayer->GetEntityID() );
	}

	CTrade* tpTrade = ptr_TradeManager->GetTrade( tpPlayer->GetEntityID( ) );
	if ( tpTrade != NULL )
	{
		// 通知客户端，交易中断
		ptr_TradeManager->OnSendTradeCloseCallBack( tpTrade->mOrigin.mEntityID, emInterrupt );
		ptr_TradeManager->OnSendTradeCloseCallBack( tpTrade->mTarget.mEntityID, emInterrupt );

		// 改变玩家交易状态
		tpTrade->ChangeTradeState( tpTrade->mOrigin.mEntityID, emOffTrade );
		tpTrade->ChangeTradeState( tpTrade->mTarget.mEntityID, emOffTrade );

		// 解锁交易信息
		tpTrade->UnLockTrade( tpPlayer->GetEntityID( ) );

		// 删除交易对象
		ptr_TradeManager->DeleteTrade( tpTrade );

		CEntityPlayer* tpSrcPlayer = ( CEntityPlayer* )CSceneLogic::GetSingleton().GetEntity( tpTrade->mOrigin.mEntityID );
		CEntityPlayer* tpDesPlayer = ( CEntityPlayer* )CSceneLogic::GetSingleton().GetEntity( tpTrade->mTarget.mEntityID );
		if ( tpSrcPlayer == NULL || tpDesPlayer == NULL )
		{
			LOG_ERROR( "trade", "EntityPlayer get failed.[%s : %d]", __LK_FILE__, __LINE__ );
			return ;
		}

		/*SendErrorNotice( tpSrcPlayer, ERROR_TRADE_OFFLINE );
		SendErrorNotice( tpDesPlayer, ERROR_TRADE_OFFLINE );*/

		// remove trade service status
		( (CPropertyPlayer*) tpSrcPlayer->GetProperty() )->SetServiceStatus( SERVICE_NONE );
		( (CPropertyPlayer*) tpDesPlayer->GetProperty() )->SetServiceStatus( SERVICE_NONE );

		LOG_DEBUG( "trade", "Player kicked, cancel trade![%s : %d]", __LK_FILE__, __LINE__ );
	}	

	return ;
}

int CTradeModule::CheckPlayerService(CEntityPlayer* pSrcPlayer, CEntityPlayer* pDesPlayer )
{
	LK_ASSERT( pSrcPlayer != NULL && pDesPlayer != NULL, return -1 );
	
	CPropertyPlayer* tpSrcProperty = ( CPropertyPlayer* ) pSrcPlayer->GetProperty();
	CPropertyPlayer* tpDesProperty = ( CPropertyPlayer* ) pDesPlayer->GetProperty();
	if ( tpSrcProperty == NULL || tpDesProperty == NULL )
		return -1;
	
	if ( tpSrcProperty->GetServiceStatus() != SERVICE_NONE || tpDesProperty->GetServiceStatus() != SERVICE_NONE )
	{
		SendErrorNotice( pSrcPlayer, ERROR_DISABLEHANDLE );
		return -1;
	}
	return 0;
}

void CTradeModule::SendErrorNotice( CEntityPlayer* pPlayer, int vCode )
{
	LK_ASSERT( pPlayer != NULL, return );

	CMessage tMessage;
	CMessageProErrorNotice tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_PROERROR );
	tPara.set_errorcode( vCode );
	tMessage.set_msgpara( (long) &tPara );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
	return ;
}
