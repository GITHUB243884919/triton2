#include "stdafx.hpp"
#include "entity.h"
#include "property.h"
#include "servertool.h"
#include "message_pb.hxx.pb.h"
#include "coremessage_pb.hxx.pb.h"
#include "promessage_pb.hxx.pb.h"
#include  "sceneobj_manager.h"
#include "friendmodule.h"
#include "mapmodule.h"
#include "teammodule.h"
#include "propertymodule.h"
#include "errcode.h"
#include "repetionmodule.h"
#include "mapmodule.h"
#include "taskmodule.h"
#include "teammodule.h"
#include "mapobjmanager.h"
#include "repetionmessage_pb.hxx.pb.h"
#include "sceneobj_manager_imp.h"
#include "property_pb.hxx.pb.h"
#include "timer_define.h"
#include "npcmodule.h"
#include "entity_searcher.h"
#include "entity_searcher_imp.h"
#include "propertymodule.h"
#include "familymodule.h"
#include "servermessage_pb.hxx.pb.h"
#include "logmodule.h"
#include "ibstoremodule.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

#include <boursemodule.h>

#define BOURSE_LOG "bourse"

#define BOURSE_ASSERT( a, fail_handle )	LK_ASSERT_LOG( BOURSE_LOG, a, fail_handle )
#define BOURSE_ASSERT_STR( a, fail_handle, str, ... ) LK_ASSERT_LOG_STR( BOURSE_LOG, a, fail_handle, str, \
																		##__VA_ARGS__ )
template<> CBourseModule* CSingleton< CBourseModule >::spSingleton = NULL;

char CBourseModule::sModuleName[ NAME_LENGTH ] = "Bourse";

CBourseModule::CBourseModule( ) : CLogicModule( sModuleName )
{
	mListID = 0;
	mBuyList.Clear( );
	mSellList.Clear( );

	mMoneyTax = 0;
	mYbTax = 0;
}

unsigned int CBourseModule::CountSize( )
{
	unsigned int tSize = sizeof( CBourseModule );	
	return tSize;
}
// 启动服务器
void CBourseModule::OnLaunchServer( )
{	
	

	OnLoadConfig( );	
}

// 退出服务器
void CBourseModule::OnExitServer( )
{
}

// 服务器Timer
void CBourseModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{	
}


// 路由消息
void CBourseModule::OnRouterMessage( CMessage* pMessage )
{
	REPE_ASSERT( pMessage != NULL, return );
	REPE_ASSERT( pMessage->has_msghead( ), return );
	const CMessageHead& tHead = pMessage->msghead( );


	switch( tHead.messageid( ) )
	{
		case ID_G2S_BOURSELIST_LOAD:
		{
			OnMessageG2SBourseListNotice( pMessage );	
		}
		break;
		case ID_G2S_BOURSE_TRADE_BUY:
		{
			OnMessageG2SBourseTradeBuy( pMessage );	
		}
		break;
		case ID_G2S_BOURSE_TRADE_SELL:
		{
			OnMessageG2SBourseTradeSell( pMessage );
		}
		break;
		case ID_D22_BOURSE_LOG:
		{
			OnMessageS2LBourseLog( pMessage );
		}
		break;
		case ID_D2S_BOURSE_LOG:
		{
			OnMessageD2SBourseLog( pMessage );	
		}
		break;
		default:
			break;
	}
}

// 客户端发送消息
void CBourseModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	REPE_ASSERT( pMessage != NULL, return );
	REPE_ASSERT( pMessage->has_msghead( ), return );
	const CMessageHead& tHead = pMessage->msghead( );

	if( CSceneCfgManager::GetSingletonPtr( )->GetBourse_cfg( )->mUse == 0 )
	{
		return;
	}

	if ( pPlayer == NULL )
	{
		return;
	}

	if ( IS_GLOBAL_SCENESERVER( pPlayer->GetLineID() ) || IS_DUNGEONSERVER_LINE( pPlayer->GetLineID() ) )
	{
		return;
	}

	switch( tHead.messageid( ) )
	{
		case ID_C2S_BOURSE_LOAD_ACCOUNT:
		{
			OnMessageC2SBourseLoadAccount( pPlayer, pMessage );	
		}
		break;
		case ID_C2S_BOURSE_INC_ACCOUNT:
		{
			OnMessageC2SBourseIncAccount( pPlayer, pMessage );
		}
		break;
		case ID_C2S_BOURSE_DEC_ACCOUNT:
		{	
			OnMessageC2SBourseDecAccount( pPlayer, pMessage );
		}
		break;
		case ID_C2S_BOURSE_ADD_BUY:
		{
			OnMessageC2SBourseAddBuy( pPlayer, pMessage );
		}
		break;
		case ID_C2S_BOURSE_ADD_SELL:
		{
			OnMessageC2SBourseAddSell( pPlayer, pMessage );
		}
		break;
		case ID_C2S_BOURSE_LOAD_SELF_LIST:
		{
			OnMessageC2SBourseLoadSelfList( pPlayer, pMessage );
		}
		break;
		case ID_C2S_BOURSE_LOAD_LIST:
		{
			OnMessageC2SBourseLoadList( pPlayer, pMessage );
		}
		break;
		case ID_C2S_BOURSE_BUY_DOC:
		{
			OnMessageC2SBourseDecBuyDoc( pPlayer, pMessage );
		}
		break;
		case ID_C2S_BOURSE_SELL_DOC:
		{
			OnMessageC2SBourseDecSellDoc( pPlayer, pMessage );
		}
		break;
		case ID_C2S_BOURSE_TRADE_LOG_QUERY:
		{
			OnMessageC2SBourseLogQuery( pPlayer, pMessage );
		}
		break;
		default:
			break;
	}
}

// 建立实体
void CBourseModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	if( pEntity->IsPlayer( ) == true )
	{
		CEntityPlayer* tpPlayer = ( CEntityPlayer* )pEntity;
		GetPlayerAccount( tpPlayer );
	//	CMessageBourseS2DGetAccount tInfo;
	//	CSession* tpSession = (CSession*) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_SESSION );
	//	LK_ASSERT_STR( tpSession != NULL , return, " Session Is NULL" );

	//	tpSession->assign( tpSession->get_id( ), bourse_load_by_account, tpPlayer->GetEntityID( ), 0 );	

	//	tInfo.set_sessionid( tpSession->get_id( ) );
	//	tInfo.set_accountid( tpPlayer->GetAccountID( ) );
	//	tInfo.set_code( BOURSE_ACCOUNT_LOAD );

	//	CSceneLogic::GetSingletonPtr( )->SendMsg2Data( &tInfo, BOURSE_DB_THREAD_KEY, ID_S2D_BOURSE_GET_ACCOUNT );
	}

	return;
}

int CBourseModule::DBSessionTimeOut( CSession* vpSession )
{
	BOURSE_ASSERT( vpSession != NULL, return ERR_INVALID_PARAMETER );

	LOG_ERROR( BOURSE_LOG, "[ %s : %d ][ %s ] Bourse DB Session %d, Type %d Parame1 %d Pararme2 %d TimeOut", 
		__LK_FILE__, __LINE__, __FUNCTION__, vpSession->get_id( ), vpSession->GetSessionType( ), 
		vpSession->GetParam1( ), vpSession->GetParam2( ) );	

	CEntityPlayer* tpPlayer = ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( 
		vpSession->GetParam1( ) );

	BOURSE_ASSERT( tpPlayer != NULL, return SUCCESS );

	if( vpSession->GetSessionType( ) == bourse_inc_account )
	{
		// ---------------  处理在数据库异常情况下刷钱bug     ------------------
		// 如果向元宝交易所账户存钱超时，只记录一条错误，不给玩家返还非绑金。
		// 大多数情况是数据库繁忙，或者锁表导致请求没有在session超时前返回，其实数据库里是执行成功了的。
		// 这里再返还的话，会造成非绑金复制。
		// 如果真的是数据库操作失败，我们通过日志给玩家补偿相应非绑金就可以了。
		
		LOG_FATAL( BOURSE_LOG, "player %d's deposit( money: %d) to bourse account failed, "
			" maybe the db is busy, or the transaction maybe successed",
			tpPlayer->GetCharID(), vpSession->GetParam3( ) );

		//// 增加帐户金钱失败, 返回玩家金钱		
		//if( CPropertyModule::GetSingletonPtr( )->PlayerChangeMoney( tpPlayer, vpSession->GetParam3( ), false, false ) 
		//	== SUCCESS )
		//{
		//	LogEventGetMoneyBuBourseErr( tpPlayer, vpSession->GetParam3( ) );	
		//}	
	}	

	return SUCCESS;
}

int CBourseModule::DBSessionEnd( CSession* vpSession, int vResultCode )
{
	BOURSE_ASSERT( vpSession != NULL, return ERR_INVALID_PARAMETER );

	CEntityPlayer* tpPlayer = ( CEntityPlayer* )CSceneObjManager::GetSingletonPtr( )->GetObject( 
		vpSession->GetParam1( ) );


	BOURSE_ASSERT( tpPlayer != NULL, return SUCCESS );

	if( vpSession->GetSessionType( ) == bourse_dec_account )
	{
		// 从账户中取钱
		if( vResultCode == SUCCESS )
		{
			if( CPropertyModule::GetSingletonPtr( )->PlayerChangeMoney( tpPlayer, vpSession->GetParam3( ), false, 
				false ) == SUCCESS )
			{
				LogEventGetMoneyByBourseAccount( tpPlayer, vpSession->GetParam3( ) );
			}	
			CYuanBao& tYb = tpPlayer->GetGolden( );
			int reyb = tYb.remain( );
			tYb.set( reyb + vpSession->GetParam4( ) );
		}
	}
	else if( vpSession->GetSessionType( ) == bourse_inc_account )
	{
		if( vResultCode != SUCCESS )
		{
			if( CPropertyModule::GetSingletonPtr( )->PlayerChangeMoney( tpPlayer, vpSession->GetParam3( ), false, 
					false ) == SUCCESS )
			{
				LogEventGetMoneyBuBourseErr( tpPlayer, vpSession->GetParam3( ) );
			}
		}
		else
		{
			CYuanBao& tYb = tpPlayer->GetGolden( );
			int reyb = tYb.remain( );
			tYb.set( ( reyb >= vpSession->GetParam4( ) ) ? reyb - vpSession->GetParam4( ) : 0 );	
		}
	}

	return SUCCESS;
}

int CBourseModule::OnMessageD2SBourseAccountNotice( CMessage* vpMsg )
{
	CMessageBourseD2SAccountNotice* tpNotice = (  CMessageBourseD2SAccountNotice* )vpMsg->msgpara( );	

	int tAccountID = tpNotice->accountid( );
	int tSessionID = tpNotice->sessionid( );

	int tParam1 = 0;
	int tParam2 = 0;
	int tParam3 = 0;
	int tAction = BOURSE_ACCOUNT_LOAD;

	if( tSessionID != 0 )
	{
		LOG_DEBUG( BOURSE_LOG, "[ %s : %d ][ %s ] Destroy Session %d", __LK_FILE__, __LINE__, __FUNCTION__, 
			tSessionID );
		CSession* tpSession = ( CSession* )CSceneObjManager::GetSingletonPtr( )->GetObject( tSessionID );
		BOURSE_ASSERT_STR( tpSession != NULL, return ERR_INVALID_PARAMETER, "Session %d", tSessionID );

		DBSessionEnd( tpSession, tpNotice->result( ) );	

		if( tpNotice->result( ) == SUCCESS )
		{
			switch( tpSession->GetSessionType( ) )
			{
				case bourse_inc_account:
					{
						tAction = BOURSE_ACCOUNT_INC;
						tParam1 = tpSession->GetParam3( );
						tParam2 = tpSession->GetParam4( );
					}
					break;
				case bourse_dec_account:
					{
						tAction = BOURSE_ACCOUNT_DEC;
						tParam1 = tpSession->GetParam3( );
						tParam2 = tpSession->GetParam4( );
					}
					break;
				case bourse_add_doc_buy:
					{
						tAction = BOURSE_ADD_DOC_BUY;
						tParam1 = tpSession->GetParam2( );
						tParam2 = tpSession->GetParam3( );
						tParam3 = tpSession->GetParam4( );
						
						if( tParam2 >= ( int )CSceneCfgManager::GetSingletonPtr( )->GetBourse_cfg( )->mBuynotify )
						{	
							SendEventNotify( BOURSE_BUY_NOTIFY, tParam1, tParam2 );
						}
					}
					break;
				case bourse_add_doc_sell:
					{
						tAction = BOURSE_ADD_DOC_SELL;
						tParam1 = tpSession->GetParam2( );
						tParam2 = tpSession->GetParam3( );
						tParam3 = tpSession->GetParam4( );
	
						if( tParam2 >= ( int )CSceneCfgManager::GetSingletonPtr( )->GetBourse_cfg( )->mSellnotify )
						{
							SendEventNotify( BOURSE_SELL_NOTIFY, tParam1, tParam2 );
						}
					}
					break;
				case bourse_dec_doc_buy:
					{
						tAction = BOURSE_DEC_DOC_BUY;
					}
					break;
				case bourse_dec_doc_sell:
					{
						tAction = BOURSE_DEC_DOC_SELL;
					}
					break;
				default:
					{
						tAction = BOURSE_ACCOUNT_LOAD;
					}
			}
		}


		CSceneObjManager::GetSingletonPtr( )->DestroyObject( tSessionID );	
	}
	else
	{
		if( tpNotice->result( ) == SUCCESS )
		{
			if( tpNotice->code( ) == BOURSE_DEC_DOC_BUY || tpNotice->code( ) == BOURSE_DEC_DOC_SELL )
			{
				tAction = tpNotice->code( );
				tParam1 = tpNotice->parame1( );
				tParam2 = tpNotice->parame2( );
			}
		}
	}

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByAccountID( tAccountID );
	if( tpPlayer == NULL )
	{
		LOG_ERROR( BOURSE_LOG, "[ %s : %d ][ %s ] Cannt Find Player, Account %d", __LK_FILE__, __LINE__, __FUNCTION__,
			tAccountID );
		return ERR_NOT_FOUND;
	}

	if( ( tpNotice->result( ) != SUCCESS ) && ( ( int )tpNotice->result( ) != ERR_UNKNOWN ) )
	{
		SendEventCode( tpPlayer, tpNotice->result( ) );
		return ERR_UNKNOWN;
	}


	LOG_NOTICE( BOURSE_LOG, "[ %s : %d ][ %s ] Account %d, Player %s, Bourse Money %d, Yb %d, Code %d", __LK_FILE__, 
		__LINE__, __FUNCTION__, tAccountID, tpPlayer->GetCharNameShow( ), tpNotice->money( ), tpNotice->yb( ),
		tpNotice->code( ) );

	CPlayerExchange* tpExange = GetPlayerExchange( tpPlayer );
	LK_ASSERT_STR( tpExange != NULL, return ERR_UNKNOWN, "Account %d", tAccountID );

	tpExange->SetMoney( tpNotice->money( ) );
	tpExange->SetYb( tpNotice->yb( ) );

	NoticePlayerAccount( tpPlayer, tAction, tParam1, tParam2, tParam3 );

	return SUCCESS;
}

int CBourseModule::NoticePlayerAccount( CEntityPlayer* vpPlayer, int tAction, int vParame1, int vParame2, int vParame3 )
{
	LK_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	CPlayerExchange* tpExange = GetPlayerExchange( vpPlayer );
	LK_ASSERT( tpExange != NULL, return ERR_INVALID_PARAMETER );

	// 发送消息给客户端
	CMessageBourseAccountNotify tNotify;
	CMessage tMsg;

	tMsg.mutable_msghead()->set_messageid( ID_S2C_BOURSE_NOTIFY_ACCOUNT );	
	tMsg.set_msgpara( (uint32)&tNotify );

	PBBourseAccountInfo* tpInfo = tNotify.mutable_account( );

	tpInfo->set_money( tpExange->GetMoney( ) );
	tpInfo->set_yb( tpExange->GetYb( ) );
	tNotify.set_yb( vpPlayer->GetGoldenYB( ) );

	tNotify.set_notifycode( tAction );
	tNotify.set_param1( vParame1 );
	tNotify.set_param2( vParame2 );
	tNotify.set_param3( vParame3 );
//	tNotify.set_notifycode( tpNotice->code( ) );

	CSceneLogic::GetSingletonPtr( )->Send2Player( vpPlayer, &tMsg );
	return SUCCESS;
}

int CBourseModule::IncAccount( CEntityPlayer* vpPlayer, int vMoney, int vYb )
{
	BOURSE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	BOURSE_ASSERT_STR( vMoney >= 0, return ERR_INVALID_PARAMETER, "Money %d", vMoney );
	BOURSE_ASSERT_STR( vYb >= 0, return ERR_INVALID_PARAMETER, "YB %d", vYb );

	CMessageBourseS2DIncAccount tAccount;
	CMessage tMsg;		
	
	// 判断是否有充足的金钱元宝
	if( vMoney > vpPlayer->GetMoney( ) )
	{
		LOG_DEBUG( BOURSE_LOG, "[ %s : %d ][ %s ] Player( %s : %d ) IncAccount Money Not Enough %d %d", __LK_FILE__,
			__LINE__, __FUNCTION__, vpPlayer->GetCharNameShow( ), vpPlayer->GetAccountID( ), vMoney, 
			vpPlayer->GetMoney( ) );
		SendEventCode( vpPlayer, BOURSE_MONEY_NOTENOUGH );
		return SUCCESS;
	}

	if( vYb > vpPlayer->GetGoldenYB( ) )
	{
		LOG_DEBUG( BOURSE_LOG, "[ %s : %d ][ %s ] Player( %s : %d ) IncAccount Money Not Enough %d %d", __LK_FILE__,
			__LINE__, __FUNCTION__, vpPlayer->GetCharNameShow( ), vpPlayer->GetAccountID( ), vMoney, 
			vpPlayer->GetMoney( ) );
		SendEventCode( vpPlayer, BOURSE_YB_NOTENOUGH );
		return SUCCESS;
	}

	CPlayerExchange* tpExchange = GetPlayerExchange( vpPlayer );
	LK_ASSERT( tpExchange != NULL, return ERR_INVALID_PARAMETER );

	if( ( tpExchange->GetMoney( ) + vMoney ) > BOURSE_ACCOUNT_MONEY_MAX )
	{
		SendEventCode( vpPlayer, BOURSE_SUM_MONEY_MUCH );
		return ERR_INVALID_PARAMETER;
	}

	if( ( tpExchange->GetYb( ) + vYb ) > BOURSE_ACCOUNT_YB_MAX )
	{
		SendEventCode( vpPlayer, BOURSE_SUM_YB_MUCH );
		return ERR_INVALID_PARAMETER;
	}

	CPropertyPlayer* tpProperty = ( CPropertyPlayer* )vpPlayer->GetProperty( );
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( vpPlayer, tPwdCheckRet );
		return ERR_UNKNOWN;
	}

	// 扣除金钱元宝
	if( CPropertyModule::GetSingletonPtr( )->PlayerChangeMoney( vpPlayer, vMoney, true, false ) != SUCCESS )
	{
//		LogEventLostMoneyByBourse( vpPlayer, vMoney );
		return SUCCESS;
	}
	LogEventLostMoneyByBourse( vpPlayer, vMoney );
//	vpPlayer->DecGoldenYB( vYb );
	
	// 在帐户中添加金钱元宝
	CSession* pSession= (CSession*) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_SESSION );
    BOURSE_ASSERT_STR( pSession != NULL, return ERR_INVALID_PARAMETER, "Cannt Get Session" );
	int nSessionID = pSession->get_id();
    pSession->assign( nSessionID, bourse_inc_account, vpPlayer->GetEntityID( ), 0 );

	pSession->SetParam3( vMoney );
	pSession->SetParam4( vYb );

	tAccount.set_money( vMoney );
	tAccount.set_yb( vYb );	
	tAccount.set_accountid( vpPlayer->GetAccountID( ) );
	tAccount.set_sessionid( nSessionID );
	tAccount.set_roleid( vpPlayer->GetCharID( ) );

	LOG_NOTICE( BOURSE_LOG, "[ %s : %d ][ %s ] Player( %s : %d ) Inc Account %d, %d, Session %d", __LK_FILE__, 
		__LINE__, __FUNCTION__, vpPlayer->GetCharNameShow( ), vpPlayer->GetAccountID( ), vMoney, vYb, nSessionID );

	CSceneLogic::GetSingletonPtr( )->SendMsg2Data( &tAccount, BOURSE_DB_THREAD_KEY, ID_S2D_BOURSE_INC_ACCOUNT );

	return SUCCESS;
}

int CBourseModule::DecAccount( CEntityPlayer* vpPlayer, int vMoney, int vYb )
{
	BOURSE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	BOURSE_ASSERT_STR( vMoney >= 0, return ERR_INVALID_PARAMETER, "Money %d", vMoney );
	BOURSE_ASSERT_STR( vYb >= 0, return ERR_INVALID_PARAMETER, "YB %d", vYb );

	CPlayerExchange* tpExchange = GetPlayerExchange( vpPlayer );
	BOURSE_ASSERT( tpExchange != NULL, return ERR_INVALID_PARAMETER );

	if( tpExchange->GetMoney( ) < vMoney )
	{
		SendEventCode( vpPlayer, BOURSE_MONEY_NOTENOUGH );
		return ERR_INVALID_PARAMETER;
	}

	if( tpExchange->GetYb( ) < vYb )
	{
		SendEventCode( vpPlayer, BOURSE_YB_NOTENOUGH );
		return ERR_INVALID_PARAMETER;
	}

	// 如果钱超过携带上限，则发系统邮件
	CTemplateLevelMoneyLmt* tpMoneyTable = (CTemplateLevelMoneyLmt*)CDataStatic::GetTemp(TEMP_MONEYLMT_CFG);
	LK_ASSERT( tpMoneyTable!=NULL, return false);
	LK_ASSERT( vpPlayer->GetLevel() >= 0 && vpPlayer->GetLevel() < TMP_MAX_GRADE, return false);

		// 判断钱是否溢出
	if ( ( unsigned int )( vpPlayer->GetMoney()+vMoney ) > tpMoneyTable->mLmtTable[vpPlayer->GetLevel()].mBagMoneyLmt )
	{
		CPropertyModule::GetSingletonPtr( )->SendProErrorNotice( vpPlayer, ERR_BAG_MONEY_UPLMT );	
		return false;
	}


	CMessageBourseS2DIncAccount tAccount;
	CMessage tMsg;		
	
	// 减少金钱元宝
	CSession* pSession= (CSession*) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_SESSION );
    BOURSE_ASSERT_STR( pSession != NULL, return ERR_INVALID_PARAMETER, "Session Is Null" );
	int nSessionID = pSession->get_id();
    pSession->assign( nSessionID, bourse_dec_account, vpPlayer->GetEntityID( ), 0 );

	pSession->SetParam3( vMoney );
	pSession->SetParam4( vYb );

	tAccount.set_money( vMoney );
	tAccount.set_yb( vYb );	
	tAccount.set_accountid( vpPlayer->GetAccountID( ) );
	tAccount.set_sessionid( nSessionID );
	tAccount.set_roleid( vpPlayer->GetCharID( ) );

	LOG_NOTICE( BOURSE_LOG, "[ %s : %d ][ %s ] Player( %s : %d ) Dec Account %d, %d, Session %d", __LK_FILE__, 
		__LINE__, __FUNCTION__, vpPlayer->GetCharNameShow( ), vpPlayer->GetAccountID( ), vMoney, vYb, nSessionID );

	CSceneLogic::GetSingletonPtr( )->SendMsg2Data( &tAccount, BOURSE_DB_THREAD_KEY, ID_S2D_BOURSE_DEC_ACCOUNT );

	return SUCCESS;
}


int CBourseModule::AddBuyOrder( CEntityPlayer* vpPlayer, int vPrice, int vYbCount )
{
	BOURSE_ASSERT( vpPlayer != NULL, return SUCCESS );
	BOURSE_ASSERT( vPrice > 0, return SUCCESS );
	BOURSE_ASSERT( vYbCount > 0, return SUCCESS );

	CPlayerExchange* tpExchange = GetPlayerExchange( vpPlayer );
	BOURSE_ASSERT( tpExchange != NULL, return SUCCESS );

	if( ( vPrice * vYbCount > 0 ) 
			&& ( ( vPrice * vYbCount + BOURSE_TAX_MONEY( vPrice * vYbCount ) ) > 0 )
			&& ( tpExchange->GetMoney( ) < ( vPrice * vYbCount + BOURSE_TAX_MONEY( vPrice * vYbCount ) ) ) )
	{
		SendEventCode( vpPlayer, BOURSE_MONEY_NOTENOUGH );
		return SUCCESS;
	}

	PBBourseDetail tDetail;

	tDetail.set_account_id( vpPlayer->GetAccountID( ) );
	tDetail.set_price( vPrice );
	tDetail.set_yb_count( vYbCount );	

	AddOrder( vpPlayer, tDetail, BOURSE_DOC_BUY, vPrice * vYbCount );

	return SUCCESS;
}


int CBourseModule::AddSellOrder( CEntityPlayer* vpPlayer, int vPrice, int vYbCount )
{
	BOURSE_ASSERT( vpPlayer != NULL, return SUCCESS );
	BOURSE_ASSERT( vPrice > 0, return SUCCESS );
	BOURSE_ASSERT( vYbCount > 0, return SUCCESS );

	CPlayerExchange* tpExchange = GetPlayerExchange( vpPlayer );
	BOURSE_ASSERT( tpExchange != NULL, return SUCCESS );


	if( ( ( vYbCount + BOURSE_TAX_YB( vYbCount ) ) > 0 ) 
		&& tpExchange->GetYb( ) < ( vYbCount + BOURSE_TAX_YB( vYbCount ) ) )
	{
		SendEventCode( vpPlayer, BOURSE_YB_NOTENOUGH );
		return SUCCESS;
	}

	PBBourseDetail tDetail;

	tDetail.set_account_id( vpPlayer->GetAccountID( ) );
	tDetail.set_price( vPrice );
	tDetail.set_yb_count( vYbCount );	

	AddOrder( vpPlayer, tDetail, BOURSE_DOC_SELL, vYbCount );

	return SUCCESS;
}

int CBourseModule::AddOrder( CEntityPlayer* vpPlayer, PBBourseDetail& vDetail, BOURSE_DOC_TYPE vType, int vCount )
{
	BOURSE_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );
	BOURSE_ASSERT( vCount > 0, return ERR_INVALID_PARAMETER );

	CPropertyPlayer* tpProperty = ( CPropertyPlayer* )vpPlayer->GetProperty( );
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( vpPlayer, tPwdCheckRet );
		return ERR_UNKNOWN;
	}

	if( vDetail.price( ) * vDetail.yb_count( ) > BOURSE_EXCHANGE_AMOUNT_MAX )
	{
		return ERR_UNKNOWN;
	}

	CMessageBourseAddDoc tAddDoc;

    CSession* pSession= (CSession*) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_SESSION );
    BOURSE_ASSERT_STR( pSession != NULL, return ERR_INVALID_PARAMETER, "Session Is Null" );

	if( vType == BOURSE_DOC_BUY )
	{
		pSession->assign( pSession->get_id( ), bourse_add_doc_buy, vpPlayer->GetEntityID( ), 0 );
		pSession->SetParam4( BOURSE_TAX_MONEY( vCount ) );
		tAddDoc.set_type( 1 );
		tAddDoc.set_tax( BOURSE_TAX_MONEY( vCount ) );
	}
	else if( vType == BOURSE_DOC_SELL )
	{
		pSession->assign( pSession->get_id( ), bourse_add_doc_sell, vpPlayer->GetEntityID( ), 0 );
		tAddDoc.set_type( 2 );
		tAddDoc.set_tax( BOURSE_TAX_YB( vCount ) );
		pSession->SetParam4( BOURSE_TAX_YB( vCount ) );
	}
	else
	{
		LOG_ERROR( BOURSE_LOG, "[ %s : %d ][ %s ] UnKnow BOURSE_DOC_TYPE %d", __LK_FILE__, __LINE__, __FUNCTION__,
			vType );
		return ERR_INVALID_PARAMETER;
	}

	pSession->SetParam2( vDetail.price( ) );
	pSession->SetParam3( vDetail.yb_count( ) );

	tAddDoc.set_accountid( vpPlayer->GetAccountID( ) );
	tAddDoc.set_sessionid( pSession->get_id( ) );
	tAddDoc.mutable_detail( )->CopyFrom( vDetail );
	tAddDoc.set_roleid( vpPlayer->GetCharID( ) );
	tAddDoc.set_level( vpPlayer->GetLevel( ) );
	tAddDoc.set_worldid( 
		WORLD_SERVER_ID( CSceneLogic::GetSingletonPtr( )->GetWorldID( ), CSceneServer::GetSingleton().mThisServerID ) );
	tAddDoc.set_serverid( CSceneServer::GetSingletonPtr( )->mThisServerID );
	tAddDoc.set_taxid( mTaxID );
	tAddDoc.set_gameid( vpPlayer->GetGameID( ) );

	CSceneLogic::GetSingletonPtr( )->SendMsg2Data( &tAddDoc, BOURSE_DB_THREAD_KEY, ID_S2D_BOURSE_ADD_DOC );

	return SUCCESS;
}

int CBourseModule::GetPlayerSelfDocList( CEntityPlayer* vpPlayer )
{
	CMessageBoursePlayerListGet tGet;

	tGet.set_accountid( vpPlayer->GetAccountID( ) );

	CSceneLogic::GetSingletonPtr( )->SendMsg2Data( &tGet, BOURSE_DB_THREAD_KEY, ID_S2D_BOURSE_PLAYER_LIST_GET );	

	return SUCCESS;	
}

int CBourseModule::PlayerSelfDocListNotice( CMessage* vpMsg )
{
	CMessageBoursePlayerListNotice* tpNotice = ( CMessageBoursePlayerListNotice* )vpMsg->msgpara( );

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByAccountID( tpNotice->accountid( ) );
	if( tpPlayer == NULL )
	{
		return ERR_UNKNOWN;
	}

	CMessageBourseSelfListNotice tNotice;
	CMessage tMsg;

	tMsg.mutable_msghead()->set_messageid( ID_S2C_BOURSE_NOTIFY_SELF_LIST );
    tMsg.set_msgpara( (uint32)&tNotice );	

	if( tpNotice->has_buylist( ) )
	{
		tNotice.mutable_buylist( )->CopyFrom( tpNotice->buylist( ) );
	}

	if( tpNotice->has_selllist( ) )
	{
		tNotice.mutable_selllist( )->CopyFrom( tpNotice->selllist( ) );
	}

	CSceneLogic::GetSingletonPtr( )->Send2Player( tpPlayer, &tMsg );

	return SUCCESS;
}

int CBourseModule::OnMessageC2SBourseLoadAccount( CEntityPlayer* vpPlayer, CMessage* vpMsg )
{
//	CMessageBourseS2DGetAccount tInfo;
//	tInfo.set_accountid( vpPlayer->GetAccountID( ) );
//	CSceneLogic::GetSingletonPtr( )->SendMsg2Data( &tInfo, BOURSE_DB_THREAD_KEY, ID_S2D_BOURSE_GET_ACCOUNT );
	NoticePlayerAccount( vpPlayer );
	return SUCCESS;	
}

int CBourseModule::OnMessageC2SBourseIncAccount( CEntityPlayer* vpPlayer, CMessage* vpMsg )
{
	CMessageBourseIncAccount* tpInc = ( CMessageBourseIncAccount* )vpMsg->msgpara( );
	IncAccount( vpPlayer, tpInc->accountinc( ).money( ), tpInc->accountinc( ).yb( ) );
	return SUCCESS;
}

int CBourseModule::OnMessageC2SBourseDecAccount( CEntityPlayer* vpPlayer, CMessage* vpMsg )
{
	CMessageBourseDecAccount* tpDec = ( CMessageBourseDecAccount* )vpMsg->msgpara( );

	CPropertyPlayer* tpProperty = ( CPropertyPlayer* )vpPlayer->GetProperty( );
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( vpPlayer, tPwdCheckRet );
		return ERR_UNKNOWN;
	}


	DecAccount( vpPlayer, tpDec->accountdec( ).money( ), tpDec->accountdec( ).yb( ) );
	return SUCCESS;
}

int CBourseModule::OnMessageC2SBourseAddBuy( CEntityPlayer* vpPlayer, CMessage* vpMsg )
{
	CMessageBourseAccountAddBuy* tpBuy = ( CMessageBourseAccountAddBuy* )vpMsg->msgpara( );
	AddBuyOrder( vpPlayer, tpBuy->price( ), tpBuy->yb_count( ) );
	return SUCCESS;
}

int CBourseModule::OnMessageC2SBourseAddSell( CEntityPlayer* vpPlayer, CMessage* vpMsg )
{
	CMessageBourseAccountAddSell* tpSell = ( CMessageBourseAccountAddSell* )vpMsg->msgpara( );
	AddSellOrder( vpPlayer, tpSell->price( ), tpSell->yb_count( ) );
	return SUCCESS;
}

int CBourseModule::OnMessageC2SBourseLoadSelfList( CEntityPlayer* vpPlayer, CMessage* vpMsg )
{
	CMessageBoursePlayerListGet tInfo;
	tInfo.set_accountid( vpPlayer->GetAccountID( ) );
	CSceneLogic::GetSingletonPtr( )->SendMsg2Data( &tInfo, BOURSE_DB_THREAD_KEY, ID_S2D_BOURSE_PLAYER_LIST_GET );

	return SUCCESS;
}

int CBourseModule::OnMessageG2SBourseListNotice( CMessage* vpMsg )
{
	CMessageBourseG2SLoadList* tpList = ( CMessageBourseG2SLoadList* )vpMsg->msgpara( );

	mBuyList.Clear( );
	mSellList.Clear( );

	mBuyList.CopyFrom( tpList->buylist( ) );	
	mSellList.CopyFrom( tpList->selllist( ) );
	mListID = tpList->listid( );

	return SUCCESS;
}

int CBourseModule::OnMessageC2SBourseLoadList( CEntityPlayer* vpPlayer, CMessage* vpMsg )
{
	LK_ASSERT( vpPlayer != NULL, return ERR_INVALID_PARAMETER );

	CMessageBourseLoadList* tLoad = ( CMessageBourseLoadList* )vpMsg->msgpara( );

	if( mListID != tLoad->listid( ) && mListID != 0 )
	{
		CMessageBourseListNotice tList;
		CMessage tMsg;

		tList.set_listid( mListID );
		tList.mutable_buylist( )->CopyFrom( mBuyList );
		tList.mutable_selllist( )->CopyFrom( mSellList );

		tMsg.mutable_msghead()->set_messageid( ID_S2C_BOURSE_LIST_NOTICE );
  	 	tMsg.set_msgpara( (uint32)&tList );

		CSceneLogic::GetSingletonPtr( )->Send2Player( vpPlayer, &tMsg );
	}

	return SUCCESS;
}

CPlayerExchange* CBourseModule::GetPlayerExchange( CEntityPlayer* vpPlayer )
{
	LK_ASSERT( vpPlayer != NULL, return NULL );
	return ( ( CPropertyPlayer* )vpPlayer->GetProperty( ) )->GetPlayerExchange( );
}

int CBourseModule::SendEventCode( CEntityPlayer* vpPlayer, int vCode, int vParame1, int vParame2 )
{
	CMessageBourseCode tCode;
	CMessage tMsg;

	tCode.set_code( vCode );

	tMsg.mutable_msghead()->set_messageid( ID_S2C_BOURSE_CODE );
	tMsg.set_msgpara( (uint32)&tCode );

	CSceneLogic::GetSingletonPtr( )->Send2Player( vpPlayer, &tMsg );

	return SUCCESS;
}

int CBourseModule::OnMessageG2SBourseTradeBuy( CMessage* vpMsg )
{
	CMessageBoursePlayerTradeBuyNotice* tpNotice = ( CMessageBoursePlayerTradeBuyNotice* )vpMsg->msgpara( );

	CMessageBourseTradeBuyNotice tBuy;
	tBuy.mutable_detail( )->CopyFrom( tpNotice->detail( ) );

	BourseTradeNotice( tpNotice->detail( ).account_id( ), (CMessage*)&tBuy, ID_S2C_BOURSE_TRADE_BUY );	

	return SUCCESS;
}

int CBourseModule::OnMessageG2SBourseTradeSell( CMessage* vpMsg )
{
	CMessageBoursePlayerTradeSellNotice* tpNotice = ( CMessageBoursePlayerTradeSellNotice* )vpMsg->msgpara( );

	CMessageBourseTradeSellNotice tBuy;
	tBuy.mutable_detail( )->CopyFrom( tpNotice->detail( ) );

	BourseTradeNotice( tpNotice->detail( ).account_id( ), (CMessage*)&tBuy, ID_S2C_BOURSE_TRADE_SELL );	
	
	return SUCCESS;
}

int CBourseModule::BourseTradeNotice( int vAccountID, CMessage* vpMsg, int vMsgID )
{
	BOURSE_ASSERT( vpMsg != NULL, return ERR_INVALID_PARAMETER );

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByAccountID( vAccountID );
	if( tpPlayer == NULL )
	{
		return SUCCESS;
	}
	CMessage tMsg;

	tMsg.mutable_msghead()->set_messageid( vMsgID );	
	tMsg.set_msgpara( (uint32)vpMsg );

	CSceneLogic::GetSingletonPtr( )->Send2Player( tpPlayer, &tMsg );


	tpPlayer->SetBourseLogIsNeedLoad( true );

	GetPlayerAccount( tpPlayer );
	GetPlayerSelfDocList( tpPlayer );

	return SUCCESS;
}

int CBourseModule::GetPlayerAccount( CEntityPlayer* vpPlayer )
{
	CMessageBourseS2DGetAccount tInfo;
	CSession* tpSession = (CSession*) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_SESSION );
	BOURSE_ASSERT_STR( tpSession != NULL , return ERR_UNKNOWN, " Session Is NULL" );

	tpSession->assign( tpSession->get_id( ), bourse_load_by_account, vpPlayer->GetEntityID( ), 0 );	

	tInfo.set_sessionid( tpSession->get_id( ) );
	tInfo.set_accountid( vpPlayer->GetAccountID( ) );
	tInfo.set_code( BOURSE_ACCOUNT_LOAD );

	CSceneLogic::GetSingletonPtr( )->SendMsg2Data( &tInfo, BOURSE_DB_THREAD_KEY, ID_S2D_BOURSE_GET_ACCOUNT );

	return SUCCESS;
}

int CBourseModule::OnMessageC2SBourseDecBuyDoc( CEntityPlayer* vpPlayer, CMessage* vpMsg )
{
	CMessageBourseDecBuyDoc* tpDoc = ( CMessageBourseDecBuyDoc* )vpMsg->msgpara( );

	CPropertyPlayer* tpProperty = ( CPropertyPlayer* )vpPlayer->GetProperty( );
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( vpPlayer, tPwdCheckRet );
		return ERR_UNKNOWN;
	}

	CMessageS2DBourseDecBuyDoc tDoc;

	tDoc.set_doc( tpDoc->doc( ) );
	tDoc.set_accountid( vpPlayer->GetAccountID( ) );
	tDoc.set_roleid( vpPlayer->GetCharID( ) );

	CSceneLogic::GetSingletonPtr( )->SendMsg2Data( &tDoc, BOURSE_DB_THREAD_KEY, ID_S2D_BOURSE_DEC_BUY_DOC );

	return SUCCESS;
}

int CBourseModule::OnMessageC2SBourseDecSellDoc( CEntityPlayer* vpPlayer, CMessage* vpMsg )
{
	CMessageBourseDecSellDoc* tpDoc = ( CMessageBourseDecSellDoc* )vpMsg->msgpara( );

	CPropertyPlayer* tpProperty = ( CPropertyPlayer* )vpPlayer->GetProperty( );
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( vpPlayer, tPwdCheckRet );
		return ERR_UNKNOWN;
	}

	CMessageS2DBourseDecSellDoc tDoc;

	tDoc.set_doc( tpDoc->doc( ) );
	tDoc.set_accountid( vpPlayer->GetAccountID( ) );
	tDoc.set_roleid( vpPlayer->GetCharID( ) );

	CSceneLogic::GetSingletonPtr( )->SendMsg2Data( &tDoc, BOURSE_DB_THREAD_KEY, ID_S2D_BOURSE_DEC_SELL_DOC );

	return SUCCESS;
}

void CBourseModule::OnLoadConfig( )
{
	BOUR_LOG* tpLog = CSceneCfgManager::GetSingletonPtr()->GetBour_log();
	if( tpLog != NULL )
	{
		INIT_ROLLINGFILE_LOG( BOURSE_LOG, "../log/bourse.log", (LogLevel)tpLog->mPriority );
	}


	CMessageBourseS2GLoadList tMsg;
	CMessage tMessage;

	tMessage.mutable_msghead()->set_messageid( ID_S2G_BOURSELIST_LOAD );
	tMessage.set_msgpara( (uint32)&tMsg );

	CSceneLogic::GetSingletonPtr( )->Send2Gate( &tMessage );	

	mMoneyTax = CSceneCfgManager::GetSingletonPtr( )->GetBourse_cfg( )->mMoneytax;
	mYbTax	  = CSceneCfgManager::GetSingletonPtr( )->GetBourse_cfg( )->mYbtax;
	mTaxID	  = CSceneCfgManager::GetSingletonPtr( )->GetBourse_cfg( )->mTaxid;
}

void CBourseModule::OnMessageS2LBourseLog( CMessage* vpMsg )
{
	CMessageExchangeLog* tpLog = ( CMessageExchangeLog* )vpMsg->msgpara( );

	CMessage tMsg;
	tMsg.mutable_msghead()->set_messageid( ID_S2L_EXCHANGELOG_LOG );
	tMsg.set_msgpara( ( int )tpLog );

	CSceneLogic::GetSingletonPtr( )->Send2Log( &tMsg, ID_S2L_EXCHANGELOG_LOG, 0 );

	return;
}

void CBourseModule::SendEventNotify( int vEventID, int vPrice, int vCount )
{
	CMessageEventNotify tNotify;
	CMessage tMsg;

	tNotify.set_eventid( vEventID );
	
	tNotify.add_intparam( vPrice );
	tNotify.add_intparam( vCount );

	tMsg.set_msgpara( (uint32)&tNotify );
	tMsg.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_EVENT );

	CSceneLogic::GetSingleton().SendWorldMessage( &tMsg );

	return ;
}

int CBourseModule::OnMessageC2SBourseLogQuery( CEntityPlayer* vpPlayer, CMessage* vpMsg )
{
	BOURSE_ASSERT( vpPlayer != NULL, return SUCCESS; );
	if( vpPlayer->GetBourseLogIsNeedLoad( ) == false )
	{
		vpPlayer->BourseLogSend( );
		return SUCCESS;
	}
	CMessageBourseLogQuery tQuery;
	CMessage tMsg;

	tQuery.set_accountid( vpPlayer->GetAccountID( ) );

	CSceneLogic::GetSingletonPtr( )->SendMsg2Data( &tQuery, BOURSE_DB_THREAD_KEY, ID_S2D_BOURSE_LOG_QUERY );

	return SUCCESS;
}

int CBourseModule::OnMessageD2SBourseLog( CMessage* vpMsg )
{
	CMessageD2SBourseLog* tpLog = ( CMessageD2SBourseLog* )vpMsg->msgpara( );
//	CMessageBourseTradeLog tLog;
//	CMessage tMsg;
	
	CEntityPlayer* tpPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByAccountID( tpLog->accountid( ) );
	BOURSE_ASSERT_STR( tpPlayer != NULL, return ERR_UNKNOWN, " Account %d", tpLog->accountid( ) );

	tpPlayer->SetBourseLog( tpLog->buy_log( ), tpLog->sell_log( ) );

//	tLog.mutable_sell_log( )->CopyFrom( tpLog->sell_log( ) );	
//	tLog.mutable_buy_log( )->CopyFrom( tpLog->buy_log( ) );

//	tMsg.mutable_msghead()->set_messageid( ID_S2C_BOURSE_TRADE_LOG );	
//	tMsg.set_msgpara( (uint32)&tLog );

//	CSceneLogic::GetSingletonPtr( )->Send2Player( tpPlayer, &tMsg );

	return tpPlayer->BourseLogSend( );

}


