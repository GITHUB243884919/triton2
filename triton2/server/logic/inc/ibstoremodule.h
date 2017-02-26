#ifndef _IBSTORE_MODULE_H_
#define _IBSTORE_MODULE_H_


#include "ibstore.h"
#include "ibstore_pb.hxx.pb.h"
#include "message_pb.hxx.pb.h"
#include "entity.h"
#include "modulemanager.h"
#include "servercore.h"
#include "servertool.h"
#include "sequence.h"

using namespace SceneServer;

#define WORLD_SERVER_ID(WORLDID, SERVERID) (WORLDID * 100 + 8)

// ********************************************************************** //
// CIbTrans
//
// 正在进行中的交易, 必须放在共享内存中
//
// ********************************************************************** //

class CSession;

class CIbTrans: public CObj
{
	enum 
	{
		em_invalid_purchase = 0,
	};

  private:
	unsigned int m_nTransID;  	// 商城交易的唯一ID
	unsigned int m_nRoleID;	    // 玩家角色ID
	unsigned int m_nAccountID; 	// 玩家的帐号ID

	int m_nGoodsID;    // 商品ID
	int m_nGoodsCount;  // 商品的数量

	int m_nItemID;     // 物品ID
	int m_nItemCount;  // 单商品包括的物品的数量

	int m_nPriceUnit;  // 商品计价单位
	int m_nPriceValue; // 单价
	int m_nDiscount;   // 折扣后价格

	int  m_nPileLimit; // 物品允许堆叠的上限
	int  m_iStatus;    // 当前交易的状态

	time_t m_tmTransTime;	//交易时间
	int m_nSex;
	int m_nMetier;
	int m_nLevel;
	int m_nIP;
	int m_nBalanceBeforePurchase; // 交易前余额

	int m_nFameType;

  public:
	CIbTrans()
	{
		if ( CObj::msCreateMode )
		{
			Initialize( );
		}
		else
		{
			Resume( );
		}
	}

	~CIbTrans()
	{
	}

	int Initialize()
	{
		m_nTransID = em_invalid_purchase;

		m_nRoleID = 0;
		m_nAccountID = 0;

		m_nGoodsID = 0;
		m_nGoodsCount = 0;

		m_nItemID = 0;
		m_nItemCount = 0;

		m_nPriceUnit = 0;
		m_nPriceValue = 0;
		m_nDiscount = 0;

		m_nPileLimit = 0;

		m_tmTransTime = 0;

		m_nSex = 2;
		m_nMetier = 7;
		m_nLevel = 1;
		m_nIP = 0;
		m_nBalanceBeforePurchase = 0;

		return 0;
	}

	int Resume()
	{
		return 0;
	}

	// TransID
	void SetTransID(unsigned int nTransID) { m_nTransID = nTransID; }
	unsigned int  GetTransID() { return m_nTransID; }

	// RoleID
	void SetRoleID (unsigned int nRoleID) { m_nRoleID = nRoleID; }
	unsigned int GetRoleID() { return m_nRoleID; }

	// AccountID
	void SetAccountID( unsigned int nAccountID ) { m_nAccountID = nAccountID; }
	unsigned int GetAccountID() { return m_nAccountID; }

	// GoodsID
	void SetGoodsID(int nGoodsID) { m_nGoodsID = nGoodsID; }
	int GetGoodsID() { return m_nGoodsID; }

	// GoodsCount
	void SetGoodsCount(int nCount) { m_nGoodsCount = nCount; }
	int GetGoodsCount() { return m_nGoodsCount; }

	// ItemID
	void SetItemID(int nIbItemID) { m_nItemID = nIbItemID; }
	int GetItemID() { return m_nItemID; }

	// ItemCount
	void SetItemCount(int nCount) { m_nItemCount = nCount; }
	int GetItemCount() { return m_nItemCount; }

	// PriceUnit
	void SetPriceUnit(unsigned int nPriceUnit) { m_nPriceUnit = nPriceUnit; }
	int GetPriceUnit() { return m_nPriceUnit; }

	// PriceValue
	void SetPriceValue(unsigned int nPriceValue) { m_nPriceValue = nPriceValue; }
	int GetPriceValue() { return m_nPriceValue; } 

	// Discount
	void SetDiscount(unsigned int nPriceValue) { m_nDiscount = nPriceValue; }
	int GetDiscount() { return m_nDiscount; } 

	// PileLimit
	void SetPileLimit(int nPileLimit) { m_nPileLimit = nPileLimit; }
	int GetPileLimit() { return m_nPileLimit; }

	// m_tmTransTime
	void SetTransTime(time_t nTimeStampt) { m_tmTransTime = nTimeStampt; }
	time_t GetTransTime() { return m_tmTransTime; }
	
	// Sex, Metier, Level, IP
	void SetRoleSex(int nSex) { m_nSex = nSex; }
	void SetRoleMetier (int nMetier) { m_nMetier = nMetier; }
	void SetRoleLevel ( int nLevel ) { m_nLevel = nLevel; }
	void SetRoleIP ( int nIP ) { m_nIP = nIP; }

	int GetRoleSex() { return m_nSex; }
	int GetRoleMetier() { return m_nMetier; }	
	int GetRoleLevel() { return m_nLevel;}
	int GetRoleIP() { return m_nIP; }

	void  SetBalanceBeforePurchase(int nBalance) { m_nBalanceBeforePurchase = nBalance;}
	int GetBalanceBeforePurchase() {return m_nBalanceBeforePurchase;}

	// FameType 
	int GetFameType() { return m_nFameType; }
	void SetFameType( int nFameType ) { m_nFameType = nFameType; }
};

// ********************************************************************** //
// CFuncTrans
//
// 正在进行中的交易, 必须放在共享内存中
//
// ********************************************************************** //

class CFuncTrans: public CObj
{
private:
	unsigned int m_nTransID;  	// 功能交易的唯一ID
	unsigned int m_nRoleID;	    // 玩家角色ID
	unsigned int m_nAccountID; 	// 玩家的帐号ID

	int m_nFuncsID;    // 功能ID
	int m_nFuncsCount;  // 购买的数量

	//int m_nItemID;     // 物品ID
	//int m_nItemCount;  // 单商品包括的物品的数量

	int m_nPriceType;  // 消费种类 元宝，绑定元宝，金钱，绑定金钱
	int m_nPriceValue; // 单价

	//int  m_nPileLimit; // 物品允许堆叠的上限
	int  m_iStatus;    // 当前交易的状态

	time_t m_tmTransTime;	//交易时间
	int m_nSex;
	int m_nMetier;
	int m_nLevel;
	int m_nIP;
	int m_nBalanceBeforePurchase; // 交易前余额
	int m_ExtraData1;			//附加数据1
	int m_ExtraData2;			//附加数据2

public:
	CFuncTrans()
	{
		if ( CObj::msCreateMode )
		{
			Initialize( );
		}
		else
		{
			Resume( );
		}
	}

	~CFuncTrans()
	{
	}

	int Initialize()
	{
		m_nTransID = 0;
		m_nRoleID = 0;
		m_nAccountID = 0;
		m_nFuncsID = 0;
		m_nFuncsCount = 0;
		m_nPriceType = -1;
		m_nPriceValue = 0;
		m_iStatus = 0;
		m_tmTransTime = 0;
		m_nSex = 2;
		m_nMetier = 0;
		m_nIP = 0;
		m_nBalanceBeforePurchase = 0; 
		m_nLevel = 0;
		m_ExtraData1 = 0;
		m_ExtraData2 = 0;
		return 0;
	}

	int Resume()
	{
		return 0;
	}

	void SetTransID( unsigned int vTransID ){  m_nTransID = vTransID;}
	unsigned int GetTransID( ){ return m_nTransID ;}

	void SetRoleID( unsigned int vRoleID ) {  m_nRoleID = vRoleID ;}
	unsigned int GetRoleID( ){ return m_nRoleID;}

	void  SetAccountID( unsigned int vAccountID ){  m_nAccountID = vAccountID ;}
	unsigned int GetAccountID( ){ return m_nAccountID; }

	void SetFuncsID( int vFuncsID ) {  m_nFuncsID = vFuncsID ;}
	int GetFuncsID( ){ return m_nFuncsID ;}

	void SetFuncsCount( int vFuncsCount ){  m_nFuncsCount = vFuncsCount; }
	int GetFuncsCount( ){ return m_nFuncsCount ;}

	void SetPriceType( int vPriceType ) {  m_nPriceType = vPriceType ;}
	int GetPriceType( ) { return  m_nPriceType ;}

	void SetPriceValue( int vPriceValue ) {  m_nPriceValue = vPriceValue ;}
	int GetPriceValue( ) { return m_nPriceValue ;}

	void SetStatus ( int vStatus ) {  m_iStatus = vStatus ;}
	int GetStatus( ) { return m_iStatus; }  

	void SetTransTime( int vTransTime ){ m_tmTransTime = vTransTime ;}
	int GetTransTime( ) { return m_tmTransTime ;}

	void SetSex( int vSex ){ m_nSex = vSex ;}
	int GetSex( ){  return m_nSex; }

	void SetMetier( int vMetier ){ m_nMetier = vMetier ;}
	int GetMetier( ) { return m_nMetier ; }

	void SetIP( int vIP ){ m_nIP = vIP; }
	int GetIP( ){ return m_nIP; }

	void SetLevel( int vLevel ){ m_nLevel = vLevel ;}
	int GetLevel(){ return m_nLevel;}

	void  SetBalanceBeforePurchase(int nBalance) { m_nBalanceBeforePurchase = nBalance;}
	int GetBalanceBeforePurchase() {return m_nBalanceBeforePurchase;}

	void SetExtraData( int vExtraData1 ,int vExtraData2){ m_ExtraData1 = vExtraData1;  m_ExtraData2 = vExtraData2; }
	int GetExtraDatat1(){ return m_ExtraData1 ;}
	int GetExtraDatat2(){ return m_ExtraData2 ;}
};

// 商城模块
class CIbStoreModule : public CLogicModule, public CSingleton< CIbStoreModule >
{
private:
	enum  { em_trans_fail = 0, em_trans_ok = 1 };
	enum  { em_ibitem_limit = 1024 };

	typedef lk::hash_map<int, CGoods, em_ibitem_limit> goods_t;
	typedef lk::hash_map<int, CPromotion, em_ibitem_limit> promotion_t;
	typedef lk::hash_map<int, CFuncConsume, em_ibitem_limit> funcconsum_t;

	typedef goods_t::const_iterator const_iterator;
	typedef goods_t::iterator iterator;
	typedef goods_t::value_type value_type;

	
	PBIbStore* m_ppbIbStore;
	CSequence* m_pSequence;

	PBIbSubTab* m_ppbFocus;
	char m_szBulletin[1024];
	int  m_nVersion;
	size_t m_nFocus;

	size_t m_nStore;
	goods_t m_tGoods;
	promotion_t m_tPromotion;
	funcconsum_t m_tFuncConsume;
	bool m_bFuncIsLoad;

	// insert pIbItem
	bool InsertGoods(CGoods& pIbItem);

	// find nItemID
	CGoods* GetGoods(int nGoodsID);


	// insert pFuncItem
	bool InsertFuncs(CFuncConsume & func);

	// find Func by DI
	CFuncConsume* GetFuncs( int nFuncID);

	CWTimer mLoadInterval;

public:
	CIbStoreModule();
	~CIbStoreModule();


	static unsigned int CountSize();

public:
	// 启动服务器
	virtual void OnLaunchServer( );

	// 退出服务器
	virtual void OnExitServer( );

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );

	// 建立场景
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );

	// 路由消息
	virtual void OnRouterMessage( CMessage* pMessage );

	// 网关消息
	void OnGateMessage( CMessage* pMessage );

	// 客户端发送消息
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );

	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

	// 加载配置
	virtual void OnLoadConfig( );

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }

	// 购买物品请求异步处理
	void OnSessionPlayerPayAmount( CMessage *tpMessage, int nRoleID, int nTransID);

	// 购买物品异步请求超时
	void OnSeessionTimeoutPlayerPayAmount( CSession* pSession );

	// 发送促销通知给玩家 
	void SendIbStorePromotion2Player( CEntityPlayer* pPlayer);

	// 增加物品购买次数 
	void IncPurchaseTimes4Player(CEntityPlayer* tpPlayer, CGoods* pGoods, int nNum, time_t t);

	// 增加物品购买次数 
	std::pair<int, int> GetPurchaseTimes4Player(CEntityPlayer* tpPlayer, CGoods* pGoods, time_t t);

public:

	// 商城物品列表请求 from Client
	void OnMessageListIbItemsRequest(CEntityPlayer *pPlayer, CMessage *pMessage );

	// 玩家充值通知
	void OnMessageUserChargeNotice( CMessage* pMessage);

	// 购买物品请求
	void OnMessagePurchaseIbItemRequest(CEntityPlayer *pPlayer, CMessage *pMessage );

	// 通知玩家交易失败
	void SendPurchaseError(CEntityPlayer* tpEntity, int nError);

	// 同步商城物品列表
	void OnMessageSynIbStoreResponse(CMessage *tpMessage );

	// 收到申请领取元宝回应
	void OnMessageDrawYuanBaoResponse( CMessage* pMessage);

	// 收到网关查询元宝余额回应
	void OnMessageQueryYuanBaoResponse( CMessage* pMessage);

	static char sModuleName[ NAME_LENGTH ];

	bool Send2DBPurchaseRequest(CEntityPlayer* pPlayer, CIbTrans* tpTrans, CGoods* pGoods, bool IsYuanbao);

	void OnMessageLockGoodsResponse( CMessage* pMessage );

	void OnMessageIbStorePromotionNotice( CMessage* pMessage);

	// 向离线玩家发送系统邮件
	void SendIbItem2OfflinePlayer(int nRoleID, CTplItem* ptpl, int nItemCount, uint64_t ullTransID);

	// 同步功能付费列表 
	void OnMessageSynFuncStoreResponse( CMessage *tpMessage );

	//功能付费列表请求 from Client
	void OnMessageGetFuncConsumesRequest(CEntityPlayer *pPlayer, CMessage *pMessage );

	//玩家功能付费请求
	void OnMessagePurchaseFuncRequest(CEntityPlayer * pPlayer, CMessage *pMessage );
	//发送功能付费失败消息
	void SendPurchaseFuncError(CEntityPlayer* tpEntity, int nError);
	//功能付费数据库请求
	bool Send2DBPurchaseFuncRequest(CEntityPlayer* pPlayer, CFuncTrans* tpTrans, CFuncConsume* pFuncConsume);
	//功能付费数据库响应
	void OnSessionPlayerPayFunc(CMessage *tpMessage, int nRoleID, int nTransID);
	//功能付费数据库超时
	void OnSessionTimeoutPlayerPayFunc( CSession* pSession );
	
	void PurchaseFuncByMoney( CEntityPlayer * pPlayer,CMessagePurchaseFuncRequest *pPurchaseFunc );

	void PurchaseFuncByYB( CEntityPlayer * pPlayer,CMessagePurchaseFuncRequest *pPurchaseFunc );

};

#endif
