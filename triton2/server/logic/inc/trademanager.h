#pragma once


#include "tradetype.h"
#include "trade.h"
#include "time.h"
#include "trademessage_pb.hxx.pb.h"
#include "sceneobj_define.h"
#include "sceneobj_manager.h"


class CSharedMem;
class CEntityPlayer;


// 交易关系人
struct TRADE_MAP
{
	unsigned int mOriginID;			// 邀请发起人
	unsigned int mTargetID;			// 关系人
	time_t		 mCreatedTime;		// 邀请发出时间

	TRADE_MAP() 
	{
		if( CObj::msCreateMode )
		{
			mOriginID = 0;
			mTargetID = 0;
			mCreatedTime = 0;
		}
	}
	TRADE_MAP( unsigned int vOriginID, unsigned int vTargetID, int vCreatedTime ) :
	mOriginID( vOriginID ), mTargetID( vTargetID ), mCreatedTime( vCreatedTime ) { }
};

class CTrade;

class CTradeManager : public CSingleton < CTradeManager >
{
private:
	typedef lk::vector< TRADE_MAP, SERVER_TRADEUNIT_MAX >	TradeMapList;
	typedef lk::vector< int, SERVER_TRADEUNIT_MAX >		TradeList;

	TradeMapList	mTradeMap;			// 交易关系人表
	TradeList		mTradeList;			// 交易对象表

public:
	CTradeManager( );
	~CTradeManager( );
	static CSharedMem* mShmPtr;
	static unsigned int CountSize( );

	void* operator new( size_t nSize );
	void  operator delete( void *pMem );

	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);

	//创建交易
	CTrade* CreateTrade( unsigned int vSrcEntityID, unsigned int vDesEntityID );

	// 更新玩家交易物品列表: 添加、移除物品
	int UpdateTradeInfo( CEntityPlayer* pPlayer, unsigned int vItemIndex, unsigned int vTradeIndex,	unsigned int vCount, TRADE_TYPE vType );

	// 获取交易对象
	CTrade* GetTrade( unsigned int vEntityID );

	// 添加交易对象
	int AddTrade( CTrade* pTrade );

	// 添加交易人记录
	int InsertTradeMap( unsigned int vSrcEntityID, unsigned int vDesEntityID );

	// 删除交易人记录
	int EraseTradeMap( TRADE_MAP* pTradeMap );

	bool IsInTradeMap( unsigned int vEntityID );

	// 是否繁忙
	bool IsBusy( unsigned int vEntityID );

	// 是否重复邀请
	bool IsInviteRepeated( unsigned int vSrcEntityID, unsigned int vDesEntityID );

	// 判断邀请人数超过上限(20人)
	bool IsOverUpperLimit( unsigned int vEntityID );

	// 是否在有效距离
	bool IsValidDistance( CEntityPlayer* pOriginPlayer, CEntityPlayer* pTargetPlayer );

	// 锁定交易信息
	int LockTradeInfo( unsigned int vEntityID );

	// 交易处理
	int ProcTrade( unsigned int vEntityID );

	//int ProcNpcTrade( unsigned int vEntity, unsigned int vBagIndex, unsigned int vShopIndex, unsigned int vCount, unsigned int vPrice );

	// 结束交易
	int EndTrade( unsigned int vEntityID, TRADE_CLOST_TYPE vType );

	// 删除交易对象
	int DeleteTrade( CTrade* pTrade );

	// 善后工作
	int DoClean( );

	// 检查邀请关系是否失效
	int CheckValidInvite( long vNowTime );

	// 得到交易关系
	TRADE_MAP* GetTradeMap( unsigned int vSrcEntityID, unsigned int vDesEntityID );

	// 删除所有邀请
	int DeleteAllInvite( unsigned int vEntityID );

	// 通知客户端更新物品列表信息
	void OnSendNotifyTradeGoodsCallBack( int vEntityID, int vType, /*char* pBuff*/PBItemObject* tpItem, int vItemIndex, int vTradeIndex, bool vIsSelf );

	// 通知客户端更新交易金额
	void OnSendNotifyTradeMoneyCallBack( int vEntityID, int vMoney, bool vIsSelf ); 

	// 通知客戶端鎖定交易信息
	void OnSendLockTradeCallBack( int vEntityID, bool vFlag, int vCode ); 

	// 通知客戶端關閉交易
	void OnSendTradeCloseCallBack( int vEntityID, int vCode );

	void OnSendTradeMsgCallBack( int vDesEntityID, unsigned int vSrcEntityID, int vCode );

	void OnSendTradeMsgCallBack( CEntityPlayer* pPlayer, unsigned int vEntityID, int vCode );

	void AddGoods( CEntityPlayer* pPlayer, unsigned int vItemIndex, unsigned int vTradeIndex, unsigned int vCount );

	void SendErrorNotice( CEntityPlayer* pPlayer, int vCode );
};
