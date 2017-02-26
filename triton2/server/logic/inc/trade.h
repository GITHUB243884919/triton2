#pragma once

#include "tradetype.h"
#include "errcode.h"
#include "lk_vector.h"
#include "lk_hashmap.h"
#include "object.h"

class CMessage;
class CEntityPlayer;

// 交易物品
struct TRADE_GOODS
{
	unsigned int	mEntityID;		// 物品归属体
	unsigned short	mItemIndex;		// 包裹索引
	unsigned int	mTradeIndex;	// 交易栏索引
	unsigned int	mCount;			// 物品数量
	int m_nItemObjID;				// 物品的对象ID, 只做描述使用
	unsigned int	mItemID;		// 物品模板ID

	TRADE_GOODS() 
	{
		if ( CObj::msCreateMode )
		{
			mEntityID = 0;
			mItemIndex = 0;
			mTradeIndex = 0;
			mCount = 0;
			mItemID = 0;

			m_nItemObjID = -1;
		}
	}

	TRADE_GOODS( unsigned int vEntityID, unsigned int vItemIndex, unsigned int vTradeIndex, unsigned int vCount, int nItemObjID, int nItemID ) :
	mEntityID( vEntityID ), mItemIndex( vItemIndex ), mTradeIndex( vTradeIndex ), mCount( vCount ), m_nItemObjID(nItemObjID), mItemID( nItemID )  {  }
};

typedef lk::vector< TRADE_GOODS, TRADE_COUNT_MAX > TRADE_GOODS_LIST_TYPE;

// 交易玩家
struct TRADE_PLAYER 
{
	unsigned int				mEntityID;
	unsigned int				mCharID;
	TRADE_STATE					mState;			// 交易状态
	TRADE_GOODS_LIST_TYPE		mGoods;			// 交易物品列表
	unsigned int				mMoney;	

	TRADE_PLAYER()
	{
		if ( CObj::msCreateMode )
		{
			mEntityID = 0;
			mCharID = 0;
			mState = emNoneState;
			mMoney = 0;
			mGoods.initailize();
		}			
	}
};

class CTrade : public CObj
{
public:
	TRADE_PLAYER mOrigin;		// 交易发起人
	TRADE_PLAYER mTarget;		// 交易对方

public:
	CTrade( );
	~CTrade( );

	// 初始化
	virtual int Initialize();

	// 恢复
	virtual int Resume();

	// 初始交易对象
	int InitTrade( unsigned int vSrcEntityID, unsigned int vDesEntityID );	

	// 交易处理
	int ProcTrade( unsigned int vEntityID );

	// 判断玩家是否在交易中
	bool IsInTrade( unsigned int vEntityID );

	// 改变玩家状态
	int ChangeTradeState( unsigned int vEntityID, TRADE_STATE vState );
	
	// 判断物品是否处于交易过程中
	bool IsGoodsInTrade( TRADE_GOODS_LIST_TYPE* pGoodsList, unsigned int vItemIndex, unsigned int vTradeIndex );		

	// 删除物品
	int DelGoodsByIndex( TRADE_GOODS_LIST_TYPE* pGoodsList, unsigned int vItemIndex, unsigned int vTradeIndex );

	// 添加物品
	int InsertGoods( unsigned int vEntityID, unsigned int vItemIndex, unsigned int vTradeIndex, unsigned int vCount , int nItemObjID, int nItemID );

	//// 添加售出物品列表
	//int InsertNpcGoods( unsigned int vEntityID, unsigned int vPrice, unsigned int vCount );

	unsigned int GetOriginEntityID( unsigned int vEntityID );
	unsigned int GetTargetEntityID( unsigned int vEntityID );

	TRADE_PLAYER* GetOriginPlayer( unsigned int vEntityID );
	TRADE_PLAYER* GetTargetPlayer( unsigned int vEntityID );

	// 锁定包裹 
	int LockPackage( unsigned int vEntityID, unsigned int vItemIndex );

	// 包裹解锁
	int UnLockPackage( unsigned int vEntityID, unsigned int vItemIndex );

	// 交易中断，解锁道具
	int UnLockTrade( unsigned int vEntityID );

	int GetOriginGoodsList( TRADE_GOODS** pGoodsList, int tNum, unsigned int vEntityID );
	int GetTargetGoodsList( TRADE_GOODS** pGoodsList, int tNum, unsigned int vEntityID);

	int GetOriginGoodsNum( unsigned int vEntityID );
	int GetTargetGoodsNum( unsigned int vEntityID );

	int GetOriginMoney( unsigned int vEntityID );
	int GetTargetMoney( unsigned int vEntityID );

	void SetOriginMoney( unsigned int vEntityID, unsigned int vMoney );
	void SetTargetMoney( unsigned int vEntityID, unsigned int vMoney );

	int ProcessState( unsigned int vEntityID );

	inline void ClearGoods() { mOrigin.mGoods.clear(); mTarget.mGoods.clear(); }

	TRADE_STATE GetOriginState( unsigned int vEntityID );
	TRADE_STATE GetTargetState( unsigned int vEntityID );

	bool CanGoodsUpdate( unsigned int vEntityID, unsigned int vItemIndex, unsigned int vTradeIndex );
	int RemoveGoods( unsigned int vEntityID, unsigned int vItemIndex, unsigned int vTradeIndex );
};



