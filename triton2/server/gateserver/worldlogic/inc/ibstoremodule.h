#ifndef _IBSTORE_MODULE_H_
#define _IBSTORE_MODULE_H_

#include <stdio.h>
#include <stdlib.h>

#include "lk_hashmap.h"
#include "module_manager.h"
#include "entity.h"
#include "SysProtocol.h"

using namespace GateServer;

#include "ibstore.h"
#include "ibstore_pb.hxx.pb.h"
	
#define em_ibitem_limit 1024

// 商城中物品的管理
class CIbStoreManager
{
  private:
	enum { em_focus_area_limit = 10 };
	enum { em_unit_page_limit = 5 };
	//int mHotGoods[em_hot_goods_limit+1];
	//int m_nHotGoods;

  public:
	typedef lk::hash_map<int, CGoods, em_ibitem_limit> goods_t;
	typedef lk::hash_map<int, CPromotion, em_ibitem_limit> promotion_t;
	typedef lk::hash_map<int, CFuncConsume, em_ibitem_limit> funcconsum_t;
	typedef goods_t::const_iterator const_iterator;
	typedef goods_t::iterator iterator;
	typedef goods_t::value_type value_type;

	goods_t m_tGoods;
	promotion_t m_tPromotion;
	funcconsum_t m_tFuncConsume;
	// golden and silver , then size = 2
	PBIbStore* m_pbIbStore[em_unit_page_limit];

	// Focus Area, defult 10
	PBIbSubTab* m_pbFocus[em_focus_area_limit];

	// bulletin
	char m_szBulletin[1024];

	// version
	int m_nVersion;
  public:
	
	// insert pIbItem
	bool InsertGoods(CGoods* pIbItem);
	bool InsertPromotion(CPromotion& rPromotion);
	// insert rFuncConsume
	bool InsertFuncConsume( CFuncConsume& rFuncConsume );

	// find nItemID
	CGoods* GetGoods(int nItemID);
	CPromotion* GetPromotion(int nGoodsID);
	//find nFuncID
	CFuncConsume * GetFuncConsume( int nFuncID);

	// find tLocation
	CGoods* GetGoods(CLocation& tLocation);

	// load from xml
	void Load4Xml(const char* xmlfile, bool bReloadFlag = false);

	//load FuncStore from xml
	void LoadFunc4Xml(const char* xmlfile, bool bReloadFlag = false );

	// save2xml
	void Save2Xml(const char* xmlfile);
	void BackupAndSave();

	// initialize hash_map
	void Initialize();

	// exchange nItemID'pos with tLocation
	bool ExchangeIbItemPos(unsigned int nItemID, CLocation& tLocation);
};

// 商城模块
class CIbStoreModule : public CDataModule, public CSingleton< CIbStoreModule >
{
  public:
	enum 
	{
		em_load_count_per_session =  200,
	};

	void OnLaunchServer();
	void OnExitServer();
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	void OnMessage(int nServerID, CMessage* pMessage );
	void ReloadCfg();

	CIbStoreModule();
	~CIbStoreModule();

	int Initialize();
	int Resume();
	static unsigned int CountSize();

	void* operator new( size_t size );
	void  operator delete( void* pointer );

	virtual bool IsLaunched() { return m_bIsLoaded; }

  private:
	CIbStoreManager m_tManager;
	bool m_bIsLoaded;

	// 定时检查促销刷新时间
	CWTimer mCheckTimer;

	// 商城的处理
	void OnGMMessageIbInfoCmd(int nServerID, CMessage* tpMessage);
	void OnGMMessageSetIbPriceCmd(int nServerID, CMessage* tpMessage);
	void OnGMMessageSetIbLocationCmd(int nServerID, CMessage* tpMessage);
	void OnGMMessageIbOnSaleCmd(int nServerID, CMessage* tpMessage);
	void OnGMMessageIbNotSaleCmd(int nServerID, CMessage* tpMessage);
	void OnGMMessageIbDescriptCmd(int nServerID, CMessage* tpMessage);

	void SendGmCmdNormalResult(int nResult, unsigned int nSessionID, unsigned int nFd);

	// 响应场景的同步商城请求
	void OnMessageSynIbStoreRequest( int nServerID, CMessage* tpMessage);

	// 玩家充值通知
	void OnMessageUserChargeNotice(int nSessionID, CMessage* tpMessage);

	// 响应场景查询余额请求
	void OnMessageUserQueryYuanbaoRequest(int nServerID, CMessage* tpMessage);

	// 响应场景申请元宝请求
	void OnMessageUserDrawYuanbaoRequest(int nServerID, CMessage* tpMessage);

	// 响应场景锁定物品的请求
	void OnMessageLockGoodsRequest(int nServerID, CMessage* tpMessage);

	// 从数据库加载商城物品动态信息
	void LoadIbStoreInfoFromDB(int nStart, int nCount);
	
	// 更新商城的动态信息
	void UpdateIbStoreInfo2DB(int nGoodsID, int nRemainNum, int nElapsedTime);
	
	// 通知场景物品数量发生变化
	void SendIbStoreInfo2Scene(int nServerID = 0);

	//------------------------------------
	//功能付费消息
	void OnMessageSynFuncStoreRequest(int  nServerID, CMessage *tpMessage);
		
  public:
	void OnAGIPSessionQueryYuanbao(SysProtocol* pSysProto, int nAccountID, int nRoleID);
	void OnAGIPSessionDrawYuanbao(SysProtocol* pSysProto, int nAccountID, int nRoleID);
	void OnSessionLoabIbStoreInfoFromDB(CMessage* tpMessage, int nStart, int ONCECOUNT);

	// 加载配置的回调函数
	static void LoadIbStoreConfigCallback(const char* pcConfigPath);
	static void LoadFuncStoreConfigCallback(const char* pcConfigPath);
	
};

#endif
