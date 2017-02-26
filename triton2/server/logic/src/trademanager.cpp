#include "stdafx.hpp"
#include "trademanager.h"
#include "coretype.h"
#include "entity.h"
#include "message_pb.hxx.pb.h"
#include "scenelogic.h"
#include "promessage_pb.hxx.pb.h"
#include "propertymodule.h"

CSharedMem* CTradeManager::mShmPtr = NULL;
template<> CTradeManager* CSingleton< CTradeManager >::spSingleton = NULL;

CTradeManager::CTradeManager()
{
	if ( mShmPtr->GetInitMode() == SHM_INIT )
	{
		mTradeList.initailize();
		mTradeMap.initailize();
	}
}

CTradeManager::~CTradeManager()
{
	this->DoClean();
}

void* CTradeManager::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CTradeManager::operator delete( void* pMem ) 
{ }

void* CTradeManager::operator new(size_t size, const char* file, int line)
{
	return operator new( size );
}
void CTradeManager::operator delete(void* pointer, const char* file, int line) {}

unsigned int CTradeManager::CountSize()
{
	return sizeof( CTradeManager );
}

int CTradeManager::DoClean()
{
	mTradeList.clear();
	mTradeMap.clear();
	return 0;
}

CTrade* CTradeManager::CreateTrade(unsigned int vSrcEntityID, unsigned int vDesEntityID )
{
	if ( vSrcEntityID == 0 || vDesEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return NULL;
	}

	CTrade* tpTrade = ( CTrade* )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_TRADE );
	if ( tpTrade == NULL )
	{
		LOG_ERROR( "trade", "CreateObject() failed, [%s : %d]", __LK_FILE__, __LINE__ );
		return NULL;
	}

	tpTrade->InitTrade( vSrcEntityID, vDesEntityID );
	return tpTrade;
}

CTrade* CTradeManager::GetTrade(unsigned int vEntityID )
{
	if ( vEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return NULL;
	}
	
	TradeList::iterator it;
	for ( it = mTradeList.begin(); it != mTradeList.end(); ++ it )
	{		
		CTrade*	tpTrade = ( CTrade* ) CSceneObjManager::GetSingleton().GetObject( *it );
		if ( tpTrade == NULL )
		{
			continue ;
		}
		if ( tpTrade->IsInTrade( vEntityID ) )
		{
			return tpTrade ;
		}
	}
	return NULL;
}

int CTradeManager::AddTrade( CTrade* pTrade )
{
	if ( pTrade == NULL )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	if ( mTradeList.size() < SERVER_TRADEUNIT_MAX )
	{
		mTradeList.push_back( pTrade->get_id() );
	}
	return 0;
}

int CTradeManager::ProcTrade(unsigned int vEntityID )
{
	if ( vEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	short tOriginGoods[ TRADE_COUNT_MAX * 2 ] = { 0 };
	short tTargetGoods[ TRADE_COUNT_MAX * 2 ] = { 0 };

	CTrade* tpTrade = GetTrade( vEntityID );
	if ( tpTrade == NULL )
	{
		LOG_ERROR( "trade", "GetTrade() failed,[%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	int tSrcEntityID = tpTrade->GetOriginEntityID( vEntityID );
	int tDesEntityID = tpTrade->GetTargetEntityID( vEntityID );

	CEntityPlayer* tpOriginPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingletonPtr()->GetEntity( tSrcEntityID );
	CEntityPlayer* tpTargetPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingletonPtr()->GetEntity( tDesEntityID );

	if ( tpOriginPlayer == NULL || tpTargetPlayer == NULL )
	{
		LOG_ERROR( "trade", "Player is none, [%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	int tReturn = 0;
	int tRes = tpTrade->ProcessState( vEntityID );

	switch( tRes )
	{
	case ERROR_TRADE_OFFTRADE:
		{
			//OnSendTradeMsgCallBack( vEntityID, 0, ERROR_TRADE_OFFTRADE );
			SendErrorNotice( tpOriginPlayer, ERROR_TRADE_OFFTRADE );
			break;
		}
	case ERROR_TRADE_GOODSUNLOCKED:
		{
			SendErrorNotice( tpOriginPlayer, ERROR_TRADE_GOODSUNLOCKED );
			break;
		}
	case emNeedTarget:
		{
			tReturn = emWaitTrade;
			break;
		}
	case emCanDoTrade:
		{
			int tSize[ 2 ] = { 0 };				// 双方物品个数

			TRADE_GOODS* tOriginGoodsList[ TRADE_COUNT_MAX ] = { 0 };
			TRADE_GOODS* tTargetGoodsList[ TRADE_COUNT_MAX ] = { 0 };
			
			// 对索引上的物品进行2次检查，只要有不正确的就不能交易
			bool bCheckIndex2nd = true;
			
			int tSrcNum = tpTrade->GetOriginGoodsList( tOriginGoodsList, ARRAY_CNT( tOriginGoodsList ), vEntityID );
			for ( int i = 0, tIndex = 0; i < tSrcNum && tIndex < (int)ARRAY_CNT( tOriginGoods ); i ++ )
			{
				if ( tOriginGoodsList[ i ] == NULL )
				{
					continue ;
				}

				tOriginGoods[ tIndex ] = tOriginGoodsList[ i ]->mItemIndex;
				tIndex ++;
				tOriginGoods[ tIndex ] = tOriginGoodsList[ i ]->mCount;
				tIndex ++;
				tSize[ 0 ] ++;
				tpTrade->UnLockPackage( tOriginGoodsList[ i ]->mEntityID, tOriginGoodsList[ i ]->mItemIndex );

				bool bLastCheckIdx = CPropertyModule::GetSingletonPtr()->HasItemOnIndex( tOriginGoodsList[ i ]->mEntityID, 
					tOriginGoodsList[ i ]->mItemIndex,
					tOriginGoodsList[ i ]->mItemID,
					tOriginGoodsList[ i ]->mCount);

				if( bLastCheckIdx != true )
				{
					bCheckIndex2nd = false;
					LOG_ERROR( "pro", "[%s:%d] player %d item %d of index %d has change", __FUNCTION__, __LINE__,
						tpOriginPlayer->GetCharID(),tOriginGoodsList[ i ]->mItemID, tOriginGoodsList[ i ]->mItemIndex );
					continue;
				}
			}

			if( bCheckIndex2nd == false )
			{
				SendErrorNotice( tpOriginPlayer, ERROR_TRADE_OFFTRADE );
				break;
			}

			bCheckIndex2nd = true;

			int tDesNum = tpTrade->GetTargetGoodsList( tTargetGoodsList, ARRAY_CNT( tTargetGoodsList ), vEntityID );
			for ( int i = 0, tIndex = 0; i < tDesNum && tIndex < (int)ARRAY_CNT( tTargetGoods ); i ++ )
			{
				if ( tTargetGoodsList[ i ] == NULL )
				{
					continue ;
				}

				tTargetGoods[ tIndex ] = tTargetGoodsList[ i ]->mItemIndex;
				tIndex ++;
				tTargetGoods[ tIndex ] = tTargetGoodsList[ i ]->mCount;
				tIndex ++;
				tSize[ 1 ] ++;
				tpTrade->UnLockPackage( tTargetGoodsList[ i ]->mEntityID, tTargetGoodsList[ i ]->mItemIndex );

				bool bLastCheckIdx = CPropertyModule::GetSingletonPtr()->HasItemOnIndex( tTargetGoodsList[ i ]->mEntityID, 
					tTargetGoodsList[ i ]->mItemIndex,
					tTargetGoodsList[ i ]->mItemID,
					tTargetGoodsList[ i ]->mCount);

				if( bLastCheckIdx != true )
				{
					bCheckIndex2nd = false;
					LOG_ERROR( "pro", "[%s:%d] player %d item %d of index %d has change", __FUNCTION__, __LINE__,
						tpTargetPlayer->GetCharID(),tTargetGoodsList[ i ]->mItemID, tTargetGoodsList[ i ]->mItemIndex );
					continue;
				}

			}

			if( bCheckIndex2nd == false )
			{
				SendErrorNotice( tpTargetPlayer, ERROR_TRADE_OFFTRADE );
				break;
			}
			

			int tSrcMoney = tpTrade->GetOriginMoney( vEntityID );
			int tDesMoney = tpTrade->GetTargetMoney( vEntityID );

			tReturn = CPropertyModule::GetSingleton().CheckChange( tpOriginPlayer, tpTargetPlayer, 
				tSrcNum, tDesNum, tSrcMoney, tDesMoney );

			if ( tReturn != EXCHAGE_SUCCESS )
			{
				break;
			}

			tpOriginPlayer->SetExchange(true);
			tpTargetPlayer->SetExchange(true);

			tReturn = CPropertyModule::GetSingleton().ExchangeBaggage( tpOriginPlayer, tpTargetPlayer, tSize[ 0 ], tOriginGoods, 
				tSize[ 1 ], tTargetGoods, tSrcMoney, tDesMoney, em_exchange_by_trade );

			tpOriginPlayer->SetExchange(false);
			tpTargetPlayer->SetExchange(false);
			break;
		}
	default:
		break;
	}
	return tReturn;
}

TRADE_MAP* CTradeManager::GetTradeMap(unsigned int vSrcEntityID, unsigned int vDesEntityID)
{
	if ( vSrcEntityID == 0 || vDesEntityID == 0 )
	{
		LOG_ERROR( "trade", "SrcEntityID = %d,DesEntityID = %d![%s : %d]", vSrcEntityID, vDesEntityID, __LK_FILE__, __LINE__ );
		return NULL;
	}

	TradeMapList::iterator it;
	for ( it = mTradeMap.begin(); it != mTradeMap.end(); ++ it )
	{
		if ( it == NULL )
		{
			continue ;
		}
		if ( ( (TRADE_MAP)*it ).mOriginID == vSrcEntityID && ( (TRADE_MAP)*it ).mTargetID == vDesEntityID )
		{
			return &( *it );
		}
	}
	return NULL;
}

int CTradeManager::DeleteAllInvite(unsigned int vEntityID )
{
	if ( vEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	TradeMapList::iterator it;
	it = mTradeMap.begin();

	while( it != mTradeMap.end() )
	{
		TRADE_MAP* tpMap = ( TRADE_MAP* ) &( *it );
		if ( tpMap->mTargetID == vEntityID || tpMap->mOriginID == vEntityID )
		{
			mTradeMap.erase( it );
			it = mTradeMap.begin();
		}
		else
		{
			++ it;
		}
	}

	return 0;
}

int CTradeManager::DeleteTrade( CTrade *pTrade )
{
	if ( pTrade == NULL )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	TradeList::iterator it;
	for ( it = mTradeList.begin(); it != mTradeList.end(); ++ it )
	{
		if ( it == NULL )
		{
			continue ;
		}
		if ( pTrade->get_id() == *it )
		{
			break ;
		}
	}
	
	CSceneObjManager::GetSingletonPtr()->DestroyObject( pTrade->get_id() );

	if ( it != mTradeList.end() )
	{
		mTradeList.erase( it );
	}

	return 0;
}

int CTradeManager::InsertTradeMap(unsigned int vSrcEntityID, unsigned int vDesEntityID )
{
	if ( vSrcEntityID == 0 || vDesEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	TRADE_MAP tMapItem;
	tMapItem.mOriginID = vSrcEntityID;
	tMapItem.mTargetID = vDesEntityID;

	time_t tOccurTime = time( NULL );
	tMapItem.mCreatedTime = tOccurTime;

	mTradeMap.push_back( tMapItem );
	return 0;
}

int CTradeManager::EraseTradeMap( TRADE_MAP* pTradeMap )
{
	if ( pTradeMap == NULL )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	TradeMapList::iterator it;
	for ( it = mTradeMap.begin(); it != mTradeMap.end(); ++ it )
	{
		if ( it == NULL )
		{
			continue ;
		}
		if ( ( (TRADE_MAP) *it ).mOriginID == pTradeMap->mOriginID 
			&& ( (TRADE_MAP) *it ).mTargetID == pTradeMap->mTargetID )
		{
			break ;
		}
	}
	mTradeMap.erase( it );

	return 0;
}

bool CTradeManager::IsInTradeMap(unsigned int vEntityID )
{
	if ( vEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return false;
	}

	TradeMapList::iterator it;
	for ( it = mTradeMap.begin(); it != mTradeMap.end(); ++ it )
	{
		TRADE_MAP* tpMap = ( TRADE_MAP* ) &( *it );
		if ( tpMap->mTargetID == vEntityID || tpMap->mOriginID == vEntityID )
		{
			return true;
		}
	}

	return false;
}

bool CTradeManager::IsBusy(unsigned int vEntityID)
{
	if ( vEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return false;
	}

	CTrade* tpTrade = GetTrade( vEntityID );
	if ( tpTrade != NULL )
	{
		return true;
	}

	return false;
}

bool CTradeManager::IsInviteRepeated(unsigned int vSrcEntityID, unsigned int vDesEntityID )
{
	if ( vSrcEntityID == 0 || vDesEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return false;
	}

	TradeMapList::iterator it;
	for ( it = mTradeMap.begin(); it != mTradeMap.end(); ++ it )
	{
		if ( it == NULL )
		{
			continue ;
		}

		if ( ( (TRADE_MAP)*it ).mOriginID == vSrcEntityID && ( (TRADE_MAP)*it ).mTargetID == vDesEntityID )
		{
			return true;
		}
	}

	return false;
}

bool CTradeManager::IsOverUpperLimit(unsigned int vEntityID )
{
	if ( vEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return false;
	}
	
	int tAmount = 0;
	TradeMapList::iterator it;
	for ( it = mTradeMap.begin(); it != mTradeMap.end(); ++ it )
	{
		if ( ((TRADE_MAP)*it).mTargetID == vEntityID )
		{
			tAmount ++;
		}		
	}

	if ( tAmount > INVITE_UPPER_LIMIT )
	{
		return true ;
	}

	return false;
}

bool CTradeManager::IsValidDistance(CEntityPlayer* pOriginPlayer, CEntityPlayer* pTargetPlayer )
{
	if( pOriginPlayer == NULL || pTargetPlayer == NULL )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return false;
	}

	if ( pOriginPlayer->IsWithinDistance( pTargetPlayer, TRADE_LENGTH_MAX  ) )
	{
		return true;
	}

	return false;
}

int CTradeManager::LockTradeInfo(unsigned int vEntityID)
{
	if ( vEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	CTrade* tpTrade = GetTrade( vEntityID );
	if( tpTrade == NULL )
	{
		LOG_ERROR( "trade", "GetTrade() failed, [%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	int tSrcEntityID = tpTrade->GetOriginEntityID( vEntityID );
	int tDesEntityID = tpTrade->GetTargetEntityID( vEntityID );
	bool bIsOrigin = ((unsigned int)tSrcEntityID==vEntityID);

	CEntityPlayer* tpOriginEntity = ( CEntityPlayer* )CSceneLogic::GetSingletonPtr()->GetEntity( tSrcEntityID );
	CEntityPlayer* tpTargetEntity = ( CEntityPlayer* )CSceneLogic::GetSingletonPtr()->GetEntity( tDesEntityID );
	if ( tpOriginEntity == NULL || tpTargetEntity == NULL )
	{
		LOG_ERROR( "trade", "Entity is none, [%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	int tSrcNum = tpTrade->GetOriginGoodsNum( vEntityID );
	int tDesNum = tpTrade->GetTargetGoodsNum( vEntityID );
	int tSrcMoney = tpTrade->GetOriginMoney( vEntityID );
	int tDesMoney = tpTrade->GetTargetMoney( vEntityID );

	int tRes = CPropertyModule::GetSingleton().CheckChange( tpOriginEntity, tpTargetEntity, 
		tSrcNum, tDesNum, tSrcMoney, tDesMoney );

	/*
	 *	上面的接口检查金钱时，不会关心玩家的交易状态。
	 *	在交易模块结合玩家交易状态再做一次金钱的检查 
	 *	[2/21/2010 Macro]
	 */

	// 判断钱够不够
	int tSrcTradeMoney = tpTrade->GetOriginMoney( vEntityID );
	if ( tpOriginEntity->GetMoney() < tSrcTradeMoney )
		tRes = EXCHAGE_SRCMONEYNOTENOUGH;

	switch( tRes )
	{
	case EXCHAGE_SRCOUTOFSPACE:
		{
			// 信息提示交易中断，包裹空间不够
			SendErrorNotice( tpOriginEntity, ERROR_TRADE_SRCBAGLESS );
			OnSendLockTradeCallBack( tSrcEntityID, true, emTradeRefuse );
			break;
		}
	case EXCHAGE_DESOUTOFSPACE:
		{
			// 信息提示交易中断，包裹空间不够
			SendErrorNotice( tpOriginEntity, ERROR_TRADE_DESBAGLESS );
			OnSendLockTradeCallBack( tSrcEntityID, true, emTradeRefuse );
			break;
		}
	case EXCHAGE_SRCMONEYNOTENOUGH:
		{
			// 源包裹金币不够进行此次交易
			SendErrorNotice( tpOriginEntity, ERROR_TRADE_MONEYLESS );
			OnSendLockTradeCallBack( tSrcEntityID, true, emTradeRefuse );
			break;
		}
	case EXCHAGE_DESMONEYOVERFLOW:
		{
			// 目标包裹金币溢出 或超过携带上限
			// 两边都发消息
			if (bIsOrigin) // 如果请求锁定的人是发起者
			{
				SendErrorNotice( tpOriginEntity, ERROR_TRADE_DESMONEYOVER );
				SendErrorNotice( tpTargetEntity, ERROR_TRADE_MONEYOVERFLOW );
			}
			else
			{
				SendErrorNotice( tpTargetEntity, ERROR_TRADE_DESMONEYOVER );
				SendErrorNotice( tpOriginEntity, ERROR_TRADE_MONEYOVERFLOW );
			}
			OnSendLockTradeCallBack( tSrcEntityID, true, emTradeRefuse );
			break;
		}
	case EXCHAGE_MYMONEYOVERFLOW: // 我的金币发生溢出
		{
			// 两边都发消息
			if (bIsOrigin) // 如果请求锁定的人是发起者
			{
				SendErrorNotice( tpOriginEntity, ERROR_TRADE_MONEYOVERFLOW );
				SendErrorNotice( tpTargetEntity, ERROR_TRADE_DESMONEYOVER );
			}
			else
			{
				SendErrorNotice( tpTargetEntity, ERROR_TRADE_MONEYOVERFLOW);
				SendErrorNotice( tpOriginEntity, ERROR_TRADE_DESMONEYOVER  );
			}
			OnSendLockTradeCallBack( tSrcEntityID, true, emTradeRefuse );
			break;
		}
	case EXCHAGE_SUCCESS:
		{
			OnSendLockTradeCallBack( tSrcEntityID, true , emTradeAgree );
			tpTrade->ChangeTradeState( tSrcEntityID, emLockTrade );

			OnSendLockTradeCallBack( tSrcEntityID, true, emLockTrade );
			OnSendLockTradeCallBack( tDesEntityID, false, emLockTrade );
			break;
		}
	default:
		break;
	}
	return 0;
}

int CTradeManager::EndTrade(unsigned int vEntityID, TRADE_CLOST_TYPE vType )
{
	if ( vEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	CTrade* tpTrade = GetTrade( vEntityID );
	if( tpTrade == NULL )
	{
		LOG_ERROR( "trade", "GetTrade() failed, [%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	int tSrcEntityID = tpTrade->GetOriginEntityID( vEntityID );
	int tDesEntityID = tpTrade->GetTargetEntityID( vEntityID );
	
	tpTrade->ChangeTradeState( tSrcEntityID, emOffTrade );
	tpTrade->ChangeTradeState( tDesEntityID, emOffTrade );

	// 道具解锁，还原
	tpTrade->UnLockTrade( vEntityID );

	// 通知客户端关闭交易
	OnSendTradeCloseCallBack( tSrcEntityID, (int)vType );
	OnSendTradeCloseCallBack( tDesEntityID, (int)vType );	

	DeleteTrade( tpTrade );
	LOG_DEBUG( "trade", "Close Trade. [%s : %d]", __LK_FILE__, __LINE__ );

	return 0;
}

int CTradeManager::UpdateTradeInfo(CEntityPlayer* pPlayer, unsigned int vItemIndex, unsigned int vTradeIndex, unsigned int vCount, TRADE_TYPE vType )
{
	LK_ASSERT( pPlayer != NULL, return LK_IERROR );
	
	int tEntityID = pPlayer->GetEntityID( );
	CTrade* tpTrade = GetTrade( tEntityID );
	if ( tpTrade == NULL )
	{
		LOG_ERROR( "trade", "GetTrade() failed, [%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	int tSrcEntityID = tpTrade->GetOriginEntityID( tEntityID );
	int tDesEntityID = tpTrade->GetTargetEntityID( tEntityID );

	if ( tpTrade->GetOriginState( tEntityID ) == emLockTrade
		|| tpTrade->GetOriginState( tEntityID ) == emWaitTrade )
	{
		//SendErrorNotice( pPlayer, ERROR_TRADE_GOODSLOCKED );
		return -1;
	}

	switch( vType )
	{
	case emAddGoods:
		{			
			AddGoods( pPlayer, vItemIndex, vTradeIndex, vCount );
			break;
		}
	case emDelGoods:
		{
			if ( tpTrade->CanGoodsUpdate( tEntityID, vItemIndex, vTradeIndex ) == true )	// 是否在交易列表中
			{
				SendErrorNotice( pPlayer, ERROR_TRADE_GOODSNOTFOUND );
				//OnSendTradeMsgCallBack( tSrcEntityID, 0, ERROR_TRADE_GOODSNOTFOUND );
				return -1;
			}
		
			// 删除物品
			if ( tpTrade->RemoveGoods( tEntityID, vItemIndex, vTradeIndex ) != LK_ISUCCESS )
			{
				LOG_ERROR( "trade", "RemoveGoods() failed.Player(%d),[%s : %d]", tEntityID, __LK_FILE__, __LINE__ );
				return -1;
			}

			// 通知客户端移除物品道具
			OnSendNotifyTradeGoodsCallBack( tSrcEntityID, emDelGoods, NULL, vItemIndex, vTradeIndex, true );
			OnSendNotifyTradeGoodsCallBack( tDesEntityID, emDelGoods, NULL, vItemIndex, vTradeIndex, false );
		}
		break ;
	case emUpdateMoney:
		{
			if ( vCount > MONEY_LIMIT )
			{
				vCount = MONEY_LIMIT;
				SendErrorNotice( pPlayer, ERROR_TRADE_MONEYERROR );
				return -1;
			}
			else if ( vCount < TRADE_MONEY_MIN )
			{
				vCount = TRADE_MONEY_MIN;
				SendErrorNotice( pPlayer, ERROR_TRADE_MONEYERROR );
				return -1;
			}

			tpTrade->SetOriginMoney( tSrcEntityID, vCount );

			// 通知客户端更新金钱数
			OnSendNotifyTradeMoneyCallBack( tSrcEntityID, vCount, true );
			OnSendNotifyTradeMoneyCallBack( tDesEntityID, vCount, false );
		}
		break ;
	default :
		break ;
	}
	return 0;
}

void CTradeManager::AddGoods(CEntityPlayer *pPlayer, unsigned int vItemIndex, unsigned int vTradeIndex, unsigned int vCount)
{
	LK_ASSERT( pPlayer != NULL, return );

	CTrade* tpTrade = GetTrade( pPlayer->GetEntityID() );
	if ( tpTrade == NULL )
	{
		return ;
	}

	int tEntityID = pPlayer->GetEntityID();
	
	if ( tpTrade->CanGoodsUpdate( tEntityID, vItemIndex, vTradeIndex ) == false )
	{
		return ;
	}

	int	tRes = 0;	
	PBItemObject tObject;
	if ( CPropertyModule::GetSingleton().GetItemBinary( pPlayer->GetEntityID(), vItemIndex, vCount, &tObject, (int&) tRes, true ) != LK_ISUCCESS )
	{
		//SendErrorNotice( pPlayer, ERROR_TRADE_BINDITEM );
		return ;
	}

	// 一次只能交易一堆
	if( CPropertyModule::GetSingletonPtr()->GetItemNumOnIndex( pPlayer->GetEntityID(), tObject.itemid(), vItemIndex ) != (int)vCount )
	{
		SendErrorNotice( pPlayer, ERROR_TRADE_NOTPILE );
		return ;
	}


	// 道具绑定，无法交易
	if ( tRes == LOCKTYPE_EXCHANGE )
	{
		SendErrorNotice( pPlayer, ERROR_TRADE_BINDITEM );
		return ;
	}

	// 任务道具无法交易
	if ( tRes == LOCKTYPE_TASK )
	{
		SendErrorNotice( pPlayer, ERROR_TRADE_BINDITEM );
		return ;
	}

	int tSrcEntityID = tpTrade->GetOriginEntityID( pPlayer->GetEntityID() );
	int tDesEntityID = tpTrade->GetTargetEntityID( pPlayer->GetEntityID() );

	OnSendNotifyTradeGoodsCallBack( tSrcEntityID, emAddGoods, &tObject, vItemIndex, vTradeIndex, true );
	OnSendNotifyTradeGoodsCallBack( tDesEntityID, emAddGoods, &tObject, vItemIndex, vTradeIndex, false );

	LOG_DEBUG( "trade", "Item[%d] add into banner[%d]", vItemIndex, vTradeIndex );

	CItemObject* pItemObj = ((CPropertyPlayer *) (pPlayer->GetProperty()))->GetBaggage( )->GetItemObjPtr( vItemIndex );

	tpTrade->InsertGoods( tSrcEntityID, vItemIndex, vTradeIndex, vCount, pItemObj->get_id(), tObject.itemid() );
	return ;
}


void CTradeManager::OnSendNotifyTradeGoodsCallBack(int vEntityID, int vType, /*char *pBuff*/PBItemObject* tpItem, int vItemIndex, int vTradeIndex, bool vIsSelf )
{
	CMessage tMessage;
	CMessageHead* tpHead = ( CMessageHead* )tMessage.mutable_msghead();
	if ( tpHead == NULL )
	{
		LOG_ERROR( "trade", "Message Head is none! [%s : %d],[%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	tpHead->set_messageid( ID_S2C_NOTIFY_GOODSINFO );
	CMessageNotifyGoodsInfo tPara;
	/*tPara.set_goodsbuff( pBuff );*/
	if ( tpItem != NULL )
	{
		PBItemObject* tpObj = tPara.mutable_item();
		tpObj->CopyFrom( *tpItem );
	}
	tPara.set_itemindex( vItemIndex );
	tPara.set_tradeindex( vTradeIndex );
	tPara.set_type( vType );
	tPara.set_isself( (int)vIsSelf );
	tMessage.set_msgpara( (long)&tPara );

	if ( CSceneLogic::GetSingletonPtr() == NULL )
	{
		LOG_ERROR( "trade", "SceneLogic pointer null ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingletonPtr()->GetEntity( vEntityID );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "trade", "Player is none ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CSceneLogic::GetSingletonPtr()->Send2Player( tpPlayer, &tMessage );
	return ;
}

void CTradeManager::OnSendNotifyTradeMoneyCallBack(int vEntityID, int vMoney, bool vIsSelf )
{
	if ( vEntityID == 0 )
	{
		LOG_DEBUG( "trade", "Target CharId is none! [%s : %d]", __LK_FILE__, __LINE__ );
		return ;
	}

	CMessage tMessage;
	CMessageHead* tpHead = ( CMessageHead* )tMessage.mutable_msghead();
	if ( tpHead == NULL )
	{
		LOG_ERROR( "trade", "Message Head is none! [%s : %d],[%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	tpHead->set_messageid( ID_S2C_NOTIFY_TRADEMONEY );
	CMessageNotifyTradeMoney tPara;
	tPara.set_isself( (int)vIsSelf );
	tPara.set_money( vMoney );
	tMessage.set_msgpara( (long)&tPara );

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingletonPtr()->GetEntity( vEntityID );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "trade", "Player is none ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CSceneLogic::GetSingletonPtr()->Send2Player( tpPlayer, &tMessage );
	return ;
}


void CTradeManager::OnSendLockTradeCallBack(int vEntityID, bool vFlag, int vCode )
{
	CMessage tMessage;
	CMessageHead* tpHead = ( CMessageHead* )tMessage.mutable_msghead();
	if ( tpHead == NULL )
	{
		LOG_ERROR( "trade", "Message Head is none! [%s : %d],[%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	tpHead->set_messageid( ID_S2C_NOTIFY_TRADEINFOLOCK );
	CMessageNotifyTradeInfoLock tPara;
	tPara.set_isself( (int)vFlag );
	tPara.set_lockstate( vCode );
	tMessage.set_msgpara( (long)&tPara );

	if ( CSceneLogic::GetSingletonPtr() == NULL )
	{
		LOG_ERROR( "trade", "SceneLogic pointer null ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingletonPtr()->GetEntity( vEntityID );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "trade", "Player is none ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CSceneLogic::GetSingletonPtr()->Send2Player( tpPlayer, &tMessage );
	return ;;
}

void CTradeManager::OnSendTradeCloseCallBack(int vEntityID, int vCode )
{
	CMessage tMessage;
	CMessageHead* tpHead = ( CMessageHead* )tMessage.mutable_msghead();
	if ( tpHead == NULL )
	{
		LOG_ERROR( "trade", "Message Head is none! [%s : %d],[%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	tpHead->set_messageid( ID_S2C_NOTIFY_TRADECLOSE );
	CMessageNotifyTradeClose tPara;
	tPara.set_code( vCode );
	tMessage.set_msgpara( (long)&tPara );

	if ( CSceneLogic::GetSingletonPtr() == NULL )
	{
		LOG_ERROR( "trade", "SceneLogic pointer null ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingletonPtr()->GetEntity( vEntityID );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "trade", "Player is none ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CSceneLogic::GetSingletonPtr()->Send2Player( tpPlayer, &tMessage );
	return ;
}

void CTradeManager::OnSendTradeMsgCallBack( int vDesEntityID, unsigned int vSrcEntityID, int vCode )
{
	if ( vDesEntityID <= 0 )
	{
		LOG_ERROR( "trade", "Parameter is none ! [%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CMessage tMessage;
	CMessageHead* tpHead = ( CMessageHead* )tMessage.mutable_msghead();

	tpHead->set_messageid( ID_S2C_NOTIFY_RESULTCODE );
	CMessageNotifyResultCode tPara;
	tPara.set_entityid( vSrcEntityID );
	tPara.set_code( vCode );
	tMessage.set_msgpara( (long)&tPara );

	if ( CSceneLogic::GetSingletonPtr() == NULL )
	{
		LOG_ERROR( "trade", "SceneLogic pointer null ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingletonPtr()->GetEntity( vDesEntityID );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "trade", "Player is none ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CSceneLogic::GetSingletonPtr()->Send2Player( tpPlayer, &tMessage );
	return ;
}

void CTradeManager::OnSendTradeMsgCallBack( CEntityPlayer* pPlayer, unsigned int vEntityID, int vCode )
{
	if ( pPlayer == NULL )
	{
		LOG_ERROR( "trade", "Parameter is none ! [%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CMessage tMessage;
	CMessageHead* tpHead = ( CMessageHead* )tMessage.mutable_msghead();

	tpHead->set_messageid( ID_S2C_NOTIFY_RESULTCODE );
	CMessageNotifyResultCode tPara;
	tPara.set_entityid( vEntityID );
	tPara.set_code( vCode );
	tMessage.set_msgpara( (long)&tPara );

	if ( CSceneLogic::GetSingletonPtr() == NULL )
	{
		LOG_ERROR( "trade", "SceneLogic pointer null ![%s : %d], [%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}
	CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &tMessage );
	return ;
}

int CTradeManager::CheckValidInvite(long vNowTime )
{
	if ( vNowTime <= 0 )
	{
		LOG_ERROR( "trade", "Input Params is none, [%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	long tCreateTime = 0;
	TradeMapList::iterator it;
	it = mTradeMap.begin();

	while( it != mTradeMap.end() )
	{
		tCreateTime = ( long ) ((TRADE_MAP)*it).mCreatedTime;

		// 2分钟交易邀请未处理，则失效
		if ( (vNowTime - tCreateTime) > 2 * 60 )
		{
			LOG_DEBUG( "trade", "Trade Map Index time over.[%d]--[%d]", ((TRADE_MAP)*it).mOriginID, ((TRADE_MAP)*it).mTargetID );
			mTradeMap.erase( it );
			it = mTradeMap.begin();
		}
		else
		{
			++ it;
		}
	}
	
	return 0;
}

void CTradeManager::SendErrorNotice( CEntityPlayer* pPlayer, int vCode )
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
