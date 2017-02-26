#include "stdafx.hpp"
#include "entity.h"
#include "protype.h"
#include "tradetype.h"
#include "trade.h"
#include "trademanager.h"
#include "trademessage_pb.hxx.pb.h"
#include "propertymodule.h"

CTrade::CTrade()
{	
	if( CObj::msCreateMode )
	{
		Initialize();
	}
	else
	{
		Resume();
	}
}

CTrade::~CTrade( )
{	
	mOrigin.mGoods.clear();
	mTarget.mGoods.clear();
}


// ³õÊ¼»¯
int CTrade::Initialize()
{
	mOrigin.mEntityID = 0;
	mOrigin.mCharID = 0;
	mOrigin.mMoney = 0;
	mOrigin.mState = emNoneState;
	mTarget.mEntityID = 0;
	mTarget.mCharID = 0;
	mTarget.mMoney = 0;
	mTarget.mState = emNoneState;
	mOrigin.mGoods.initailize();
	mTarget.mGoods.initailize();	
	return 0;
}

// »Ö¸´
int CTrade::Resume()
{
	return 0;
}

int CTrade::InitTrade(unsigned int vSrcEntityID, unsigned int vDesEntityID )
{
	if ( vSrcEntityID == 0 || vSrcEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d][%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return -1;
	}

	mOrigin.mGoods.initailize();
	mTarget.mGoods.initailize();

	mOrigin.mEntityID = vSrcEntityID;
	mOrigin.mState = emOffTrade;
	mOrigin.mMoney = 0;

	mTarget.mEntityID = vDesEntityID;
	mTarget.mState = emOffTrade;
	mTarget.mMoney = 0;

	return 0;
}

int CTrade::InsertGoods(unsigned int vEntityID, unsigned int vItemIndex, unsigned int vTradeIndex, unsigned int vCount, int nItemObjID, int nItemID )
{
	if ( vEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	TRADE_PLAYER* tpOrigin = GetOriginPlayer( vEntityID );
	if ( tpOrigin == NULL )
	{
		LOG_ERROR( "trade", "Player not found, [%s : %d][%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return -1;
	}

	if ( tpOrigin->mGoods.size() >= TRADE_COUNT_MAX )
	{
		return -1;
	}

	TRADE_GOODS tGoods( vEntityID, vItemIndex, vTradeIndex, vCount, nItemObjID, nItemID );
	LockPackage( vEntityID, vItemIndex );
	tpOrigin->mGoods.push_back( tGoods );

	return 0;
}

int CTrade::LockPackage(unsigned int vEntityID, unsigned int vItemIndex )
{
	if ( vEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	bool bRes = false;
	CPropertyModule::GetSingleton().LockBaggage( vEntityID, vItemIndex, LOCKTYPE_EXCHANGE, bRes );
	return 0;
}

int CTrade::ChangeTradeState( unsigned int vEntityID, TRADE_STATE vState )
{
	if ( vEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}
	
	TRADE_PLAYER* tpOrigin = GetOriginPlayer( vEntityID );
	if ( tpOrigin == NULL )
	{
		return -1;
	}
	tpOrigin->mState = vState;
	
	return 0;
}


bool CTrade::IsInTrade( unsigned int vEntityID )
{
	if ( mOrigin.mEntityID == vEntityID || mTarget.mEntityID == vEntityID )
	{
		return true;
	}

	return false;
}

bool CTrade::IsGoodsInTrade(TRADE_GOODS_LIST_TYPE* pGoodsList, unsigned int vItemIndex, unsigned int vTradeIndex )
{
	if ( pGoodsList == NULL )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d][%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return false;
	}

	TRADE_GOODS_LIST_TYPE::iterator it;
	for ( it = pGoodsList->begin(); it != pGoodsList->end(); ++ it )
	{
		if ( it != NULL 
			&& ( ( TRADE_GOODS ) *it ).mTradeIndex == vTradeIndex 
			&& ( ( TRADE_GOODS ) *it ).mItemIndex  == vItemIndex )
		{
			return true;
		}
	}

	return false;
}


int CTrade::DelGoodsByIndex(TRADE_GOODS_LIST_TYPE* pGoodsList, unsigned int vItemIndex, unsigned int vTradeIndex )
{
	if ( pGoodsList == NULL )
	{
		LOG_ERROR( "trade", "Input Params is none![%s : %d][%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return -1;
	}

	if ( pGoodsList->size() <= 0 )
	{
		return -1;
	}
	
	int tEntityID = 0;
	TRADE_GOODS_LIST_TYPE::iterator it;
	for ( it = pGoodsList->begin(); it != pGoodsList->end(); ++ it )
	{
		if ( it != NULL 
			&& ( ( TRADE_GOODS ) *it ).mTradeIndex == vTradeIndex 
			&& ( ( TRADE_GOODS ) *it ).mItemIndex  == vItemIndex )
		{
			tEntityID = ( (TRADE_GOODS) *it ).mEntityID;
			pGoodsList->erase( it );
			break ;
		}
	}

	UnLockPackage( tEntityID, vItemIndex );

	return 0;
}

int CTrade::RemoveGoods( unsigned int vEntityID, unsigned int vItemIndex, unsigned int vTradeIndex )
{
	TRADE_PLAYER* tpOrigin = GetOriginPlayer( vEntityID );

	if ( tpOrigin == NULL )
	{
		return LK_IERROR;
	}

	TRADE_GOODS_LIST_TYPE::iterator it;
	for ( it = tpOrigin->mGoods.begin(); it != tpOrigin->mGoods.end(); ++ it )
	{
		if ( it->mItemIndex == vItemIndex && it->mTradeIndex == vTradeIndex )
		{
			tpOrigin->mGoods.erase( it );
			break;
		}
	}
	UnLockPackage( vEntityID, vItemIndex );
	return LK_ISUCCESS;
}

unsigned int CTrade::GetOriginEntityID( unsigned int vEntityID )
{
	if ( vEntityID == 0 )
	{
		return LK_ZERO;
	}
	
	return ( mOrigin.mEntityID == vEntityID ? mOrigin.mEntityID : mTarget.mEntityID );
}

unsigned int CTrade::GetTargetEntityID( unsigned int vEntityID )
{
	if ( vEntityID == 0 )
	{
		return LK_ZERO;
	}
	
	return ( mOrigin.mEntityID == vEntityID ? mTarget.mEntityID : mOrigin.mEntityID );
}

TRADE_PLAYER* CTrade::GetOriginPlayer(unsigned int vEntityID)
{
	LK_ASSERT( vEntityID > LK_ZERO, return NULL );

	return ( mOrigin.mEntityID == vEntityID ? &mOrigin : &mTarget );
}

TRADE_PLAYER* CTrade::GetTargetPlayer( unsigned int vEntityID )
{
	LK_ASSERT( vEntityID > LK_ZERO, return NULL );

	return ( mOrigin.mEntityID == vEntityID ? &mTarget : &mOrigin );
}

int CTrade::UnLockPackage(unsigned int vEntityID, unsigned int vItemIndex)
{
	if ( vEntityID == 0 )
	{
		LOG_ERROR( "trade", "Input Params is none, [%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}	

	bool bRes = false;
	CPropertyModule::GetSingleton().LockBaggage( vEntityID, vItemIndex, LOCKTYPE_NONE, bRes );
	return 0;
}

int CTrade::UnLockTrade(unsigned int vEntityID )
{
	if ( vEntityID <= 0 )
	{
		LOG_ERROR( "trade", "Input Params is none, [%s : %d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	TRADE_PLAYER* tpOrigin = GetOriginPlayer( vEntityID );
	TRADE_PLAYER* tpTarget = GetTargetPlayer( vEntityID );
	if ( tpOrigin == NULL || tpTarget == NULL )
	{
		LOG_ERROR( "trade", "Player is none, [%s : %d][%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return -1;
	}
	
	TRADE_GOODS_LIST_TYPE::iterator it;
	for ( it = tpOrigin->mGoods.begin(); it != tpOrigin->mGoods.end(); ++ it )
	{
		if ( it == NULL )
		{
			continue ;
		}
		
		UnLockPackage( tpOrigin->mEntityID, ((TRADE_GOODS)*it).mItemIndex );
		CTradeManager::GetSingletonPtr()->OnSendNotifyTradeGoodsCallBack( tpOrigin->mEntityID, emDelGoods, NULL, 
			(*it).mItemIndex, (*it).mTradeIndex, true );
	}
	tpOrigin->mGoods.clear();

	for ( it = tpTarget->mGoods.begin(); it != tpTarget->mGoods.end(); ++ it )
	{
		if ( it == NULL )
		{
			continue ;
		}

		UnLockPackage( tpTarget->mEntityID, ((TRADE_GOODS)*it).mItemIndex );
		CTradeManager::GetSingletonPtr()->OnSendNotifyTradeGoodsCallBack( tpTarget->mEntityID, emDelGoods, NULL, 
			(*it).mItemIndex, (*it).mTradeIndex, false );
	}
	tpTarget->mGoods.clear();

	return 0;
}

int CTrade::GetOriginGoodsList(TRADE_GOODS** pGoodsList, int tNum, unsigned int vEntityID)
{
	LK_ASSERT( pGoodsList != NULL , return LK_IERROR );

	TRADE_PLAYER* tpOrigin = GetOriginPlayer( vEntityID );
	if ( tpOrigin == NULL )
	{
		return LK_IERROR;
	}

	if ( tpOrigin->mGoods.size() > (unsigned int)tNum )
	{
		return LK_IERROR;
	}

	int tIndex = 0;
	TRADE_GOODS_LIST_TYPE::iterator it;
	for ( it = tpOrigin->mGoods.begin(); it != tpOrigin->mGoods.end() && tIndex < tNum; ++ it )
	{
		pGoodsList[ tIndex ++ ] = ( TRADE_GOODS* )&( *it );
	}
	return tIndex;
}

int CTrade::GetTargetGoodsList(TRADE_GOODS** pGoodsList, int tNum, unsigned int vEntityID)
{
	LK_ASSERT( pGoodsList != NULL , return LK_IERROR );

	TRADE_PLAYER* tpTarget = GetTargetPlayer( vEntityID );
	if ( tpTarget == NULL )
	{
		return LK_IERROR;
	}

	if ( tpTarget->mGoods.size() > (unsigned int)tNum )
	{
		return LK_IERROR;
	}

	int tIndex = 0;
	TRADE_GOODS_LIST_TYPE::iterator it;
	for ( it = tpTarget->mGoods.begin(); it != tpTarget->mGoods.end() && tIndex < tNum; ++ it )
	{
		pGoodsList[ tIndex ++ ] = ( TRADE_GOODS* )&( *it );
	}
	return tIndex;
}

int CTrade::GetOriginGoodsNum( unsigned int vEntityID )
{
	TRADE_PLAYER* tpOrigin = GetOriginPlayer( vEntityID );
	
	if ( tpOrigin == NULL )
	{
		return LK_IERROR;
	}

	return tpOrigin->mGoods.size();
}

int CTrade::GetTargetGoodsNum( unsigned int vEntityID )
{
	TRADE_PLAYER* tpTarget = GetTargetPlayer( vEntityID );

	if ( tpTarget == NULL )
	{
		return LK_IERROR;
	}

	return tpTarget->mGoods.size();
}

int CTrade::ProcessState( unsigned int vEntityID )
{
	LK_ASSERT( vEntityID > LK_ZERO, return LK_IERROR );

	TRADE_PLAYER* tpOrigin = GetOriginPlayer( vEntityID );
	TRADE_PLAYER* tpTarget = GetTargetPlayer( vEntityID );
	
	if ( tpOrigin == NULL || tpTarget == NULL )
	{
		LOG_ERROR( "trade", "TRADE_PLAYER (%d) not find.[%s : %d]", vEntityID, __LK_FILE__, __LINE__ );
		return LK_IERROR;
	}

	int tRes = 0;
	
	if ( tpOrigin->mState == emOffTrade || tpTarget->mState == emOffTrade )
	{
		tRes = ERROR_TRADE_OFFTRADE;
	}
	else if ( tpOrigin->mState == emLockTrade || tpOrigin->mState == emWaitTrade )
	{
		tpOrigin->mState = emWaitTrade;
		tRes = ( tpTarget->mState == emWaitTrade ? emCanDoTrade : emNeedTarget );
	}
	else
	{
		tRes = ERROR_TRADE_GOODSUNLOCKED;
	}

	return tRes;
}

int CTrade::GetOriginMoney( unsigned int vEntityID )
{
	TRADE_PLAYER* tpOrigin = GetOriginPlayer( vEntityID );
	if ( tpOrigin == NULL )
	{
		return LK_IERROR;
	}

	return tpOrigin->mMoney;
}

int CTrade::GetTargetMoney( unsigned int vEntityID )
{
	TRADE_PLAYER* tpTarget = GetTargetPlayer( vEntityID );
	if ( tpTarget == NULL )
	{
		return LK_IERROR;
	}

	return tpTarget->mMoney;
}

void CTrade::SetOriginMoney( unsigned int vEntityID, unsigned int vMoney )
{
	TRADE_PLAYER* tpOrigin = GetOriginPlayer( vEntityID );
	
	if ( tpOrigin != NULL )
	{
		tpOrigin->mMoney = vMoney;
	}
	return ;
}

void CTrade::SetTargetMoney( unsigned int vEntityID, unsigned int vMoney )
{
	TRADE_PLAYER* tpTarget = GetTargetPlayer( vEntityID );

	if ( tpTarget != NULL )
	{
		tpTarget->mMoney = vMoney;
	}
	return ;
}

TRADE_STATE CTrade::GetOriginState( unsigned int vEntityID )
{
	TRADE_PLAYER* tpOrigin = GetOriginPlayer( vEntityID );
	
	if ( tpOrigin == NULL )
	{
		return emNoneState;
	}

	return tpOrigin->mState;
}

TRADE_STATE CTrade::GetTargetState( unsigned int vEntityID )
{
	TRADE_PLAYER* Target = GetTargetPlayer( vEntityID );

	if ( Target == NULL )
	{
		return emNoneState;
	}

	return Target->mState;
}

bool CTrade::CanGoodsUpdate(unsigned int vEntityID, unsigned int vItemIndex, unsigned int vTradeIndex)
{
	TRADE_PLAYER* tpOrigin = GetOriginPlayer( vEntityID );

	if ( tpOrigin == NULL )
	{
		return false;
	}

	bool bResult = true;

	TRADE_GOODS_LIST_TYPE::iterator it;
	for ( it = tpOrigin->mGoods.begin(); it != tpOrigin->mGoods.end(); ++ it )
	{
		if ( it->mItemIndex == vItemIndex || it->mTradeIndex == vTradeIndex )
		{
			bResult = false;
		}
	}

	return bResult;
}
