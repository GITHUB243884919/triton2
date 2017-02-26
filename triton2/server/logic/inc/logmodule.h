#pragma once

#include "servermessage_pb.hxx.pb.h"
#include "log_msg_pb.hxx.pb.h"
#include "servercore.h"
#include "servertool.h"
#include "errcode.h"
#include "entity.h"
#include "lk_hashmap.h"
#include "mail.h"

//typedef EItemAction EMoneyAction;

/*
class CStatInfo
{
  private:
	int ItemCatalogID(int nOpType, int nItemID);
	int MoneyCatalogID(int nOpType, int nMoneyType);
	int ExpCatalogID(int nOpType, int nExpType);
	int EventCatalogID(int nEventID);

  public:
	enum 
	{
		em_item_catalog_number = 2000,
		em_money_catalog_number = 200,
		em_exp_catalog_number = 50,
		em_event_catalog_number = 20,
	};


	typedef std::pair<int64_t, int64_t> value_pair_t;
	typedef lk::hash_map<int, value_pair_t, em_item_catalog_number>  item_catalog_t;
	typedef lk::hash_map<int, value_pair_t, em_money_catalog_number> money_catalog_t;
	typedef lk::hash_map<int, value_pair_t, em_exp_catalog_number>   exp_catalog_t; 
	typedef lk::hash_map<int, value_pair_t, em_event_catalog_number> event_catalog_t; 

	money_catalog_t mDeltaMoney;	// 上次存盘之后的金钱改变量
	exp_catalog_t   mDeltaExp;		// 上次存盘之后的该变量
	item_catalog_t mDeltaItems;		// 上次存盘之后的该变量
	event_catalog_t mDeltaEvents;	// 上次存盘之后的该变量

  public:
	CStatInfo()
	{
		if ( CObj::msCreateMode )
		{
			mDeltaMoney.initailize();
			mDeltaExp.initailize();
			mDeltaItems.initailize();
			mDeltaEvents.initailize();
		}
	}
		
	~CStatInfo() {}

	inline void CatalogMoney(EMoneyAction emAction,  int nMoneyType,  int nMoney)
	{
		int nCatalogID = MoneyCatalogID(emAction, nMoneyType);

		money_catalog_t::iterator iter = mDeltaMoney.find(nCatalogID);
		if ( iter != mDeltaMoney.end())
		{
			iter->second.second += nMoney;
		}else
		{
			mDeltaMoney.insert(money_catalog_t::value_type(nCatalogID, value_pair_t(0, nMoney)));
		}
	}

	inline void CatalogExp(int emAction, int nExpType, int nExp)
	{
		int nCatalogID = ExpCatalogID(emAction, nExpType);

		exp_catalog_t::iterator iter = mDeltaExp.find(nCatalogID);
		if ( iter != mDeltaExp.end())
		{
			iter->second.second += nExp;
		}else
		{
			mDeltaExp.insert(exp_catalog_t::value_type(nCatalogID, value_pair_t(0,nExp)));
		}

	}
	
	inline void CatalogItem(EItemAction emAction, int nItemID, int nNumber)
	{
		int nCatalogID = ItemCatalogID(emAction, nItemID);

		item_catalog_t::iterator iter = mDeltaItems.find(nCatalogID);
		if ( iter != mDeltaItems.end())
		{
			iter->second.second += nNumber;
		}else
		{
			mDeltaItems.insert(item_catalog_t::value_type(nCatalogID, value_pair_t(0, nNumber)));
		}
	}

	void LogStatInfo();
	void SaveStatInfo();
	void LoadStatInfo();
	bool OnLoadStatInfo(CMessage* pMessage);
};
*/

enum EmEquipmentAttr
{
	em_attr_level 	= 0,	// 等级强化
	em_attr_resist 	= 1,	// 抗性强化
	em_attr_attack	= 2,	// 攻击强化
	em_attr_bind	= 3,	// 绑定状态改变
	em_attr_slot	= 4,	// 开孔
	em_attr_dianjin	= 5,	// 点金
	em_attr_jewel	= 6,	// 镶嵌
	em_attr_owner	= 7,	// 归属
	em_attr_property= 8,	// 附加属性等级
	em_attr_judge	= 9,	// 基本属性等级
	em_attr_mark 	= 10,	// 印鉴
	em_attr_runne	= 11,	// 符文
	em_attr_lock	= 12,	// 黄金锁
	em_change_resist= 13,	// 抗性转换
	em_change_attack= 14,	// 攻击转换
	em_spirit_info	= 15,   // 铸灵信息
};

int GetMapID4Log(CEntity* pEntity);


void LogEventItemAction( CEntityPlayer* tpPlayer, uint64_t ullGuID, int32_t nItemID, int32_t nItemNum, 
		EItemAction emAction, int32_t  nParam1 = 0, int32_t  nParam2 = 0, uint64_t  nParam3 = 0);

// 记录登录日志
void LogEventLogoutScene(CEntityPlayer* tpPlayer, EMLeaveReason emLeave, EMSaveResult emSave);


// Money
void LogEventMoneyAction( CEntityPlayer* tpPlayer, int nMoneyType, int nMoneyNumber, 
		EMoneyAction emAction, int nParam1 = 0, int nParam2 = 0, uint64_t  nParam3 = 0);

// Exp
void LogEventRoleExpAction( CEntityPlayer* tpPlayer, int emAction, 
		int nExpValue, int nParam1 = 0, int nParam2 = 0, uint64_t nParam3 = 0);

// Talent Exp
void LogEventTalentExpAction (CEntityPlayer* pPlayer, int emAction, int nExp);

// MW Exp
void LogEventMWExpAction (CEntityPlayer* pPlayer, CItemMagicWeapon* tpMW, int emAction, int nExp);

// Produce Exp
void LogEventProduceExpAction( CEntityPlayer* tpPlayer, CItemObject* pItemObject, int emAction, int nExp);

// Buff
void LogEventRoleBuffAction( CEntityPlayer* tpPlayer, int BuffID, int PersistTime, int nParam1 = 0, int nParam2 = 0, unsigned short bufftype = 0);


// EntityDie
void LogEventPlayerDieAction( CEntityPlayer* tpPlayer, 
		int nKillerType, int nKillerID, int nKillerMetier, int nKillerLevel);

// Task
void LogEventTaskAction( CEntityPlayer* tpPlayer,
		int nTaskID, ETaskAction emAction, int nParam1 = 0, int nParam2 = 0, uint64_t ullParam3 = 0);

// NpcDrop
void LogEventNpcDropAction(CEntityNpc* pDropingEntity, std::vector<CEntity*>& rDropEntityVec);

// UseItemDrop
void LogEventUseItemDropAction( CEntityPlayer* pPlayer, int nItemID, std::vector<CEntity*>& rDropEntityVec );

// LevelUp
void LogEventPlayerLevelUpAction (CEntityPlayer* tpPlayer, EUpgradeType emUpgrade, int nSutType, int nCurrentLevel, int nLevel = 1);


// DropedItemDispear
void LogDropedItemDestroyAction( CEntity* pEntity);


void LogEventPlayerRepeScore (CEntityPlayer* tpPlayer, int CurrentScore, int DeltaScore, int eScoreType);

// ItemLevelUp
// 物品等级, 元素攻击属性等级, 元素防御属性等级提升
void LogEventItemLevelUpAction(CEntityPlayer* tpPlayer, 
		int nItemID, uint64_t ullGUID, int nType, int nResult, int nCurrLevel, int nPrevLevel, int nItem1, int nBindStatus);

// 记录怪物死亡的日志		
void RecodrdBossOgreDie( CEntityOgre *pOgre, CEntityPlayer *pKiller,  CTemplateOgre* pTempOgre );

// 记录生活技能操作
void LogLifeSkill( int nRoleID, CLifeSkill *PLifeSkill, int nActionKind, int nComposeID = 0 );

// 记录仙缘
void LogMatrix( int nSlaveID, int nMasterID, int nOpType, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0 );

// 鉴定, 如果有多个基本属性，X1X2X3 -> Y1Y2Y3
#define LogEventItemJudgeAction( tpPlayer, nItemID, ullGUID, nFrom, nTo, nJudgeItem, nBindStatus ) \
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_attr_judge, 0, nTo, nFrom, nJudgeItem, nBindStatus)

// 洗附加属性, 有可能存在两个附加属性和洗出来两个附加属性
#define LogEventItemProtertyAction( tpPlayer, nItemID, ullGUID, nFrom1, nFrom2, nTo1, nTo2, nBindStatus ) \
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_attr_property, nTo1, nTo2, nFrom1, nFrom2, nBindStatus)

// 元素攻击属性转换, 记录之前的属性和之后的属性
#define LogEventItemAttackAction( tpPlayer, nItemID, ullGUID, nFrom1, nTo, nJudgeItem, nLevel, nBind ) \
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_change_attack, nLevel, nTo, nFrom, nJudgeItem, nBind)

// 元素防御属性转换, 记录之前的属性和之后的属性
#define LogEventItemResistAction( tpPlayer, nItemID, ullGUID, nFrom1, nTo, nJudgeItem, nLevel, nBind ) \
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_change_resist, nLevel, nTo, nFrom, nJudgeItem, nBind)

// 装备开槽,记录槽数
#define LogEventItemSlotAction( tpPlayer, nItemID, ullGUID, nFrom1, nTo, nSlotItem, nBind ) \
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_attr_slot, 0, nTo, nFrom, nSlotItem, nBind)

// 镶嵌宝石, 记录宝石ID
#define LogEventItemJewelAction( tpPlayer, nItemID, ullGUID, nJewelItem, nBind )\
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_attr_jewel, 0, 0, 0, nJewelItem, nBind)

// 点金会升阶或转换属性, 记录当前属性和阶数
#define LogEventItemDianAction( tpPlayer, nItemID, ullGUID, nFromID, nFrom, nToID, nTo, nBind ) \
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_attr_dianjin, nToID, nTo, nFromID, nFrom, nBind)

// 镶嵌符文, 记录符文
#define LogEventItemRunneAction( tpPlayer, nItemID, ullGUID, nRunneItem, nBind ) \
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_attr_runne, 0, 0, 0, nRunneItem, nBind )

// 黄金锁定与解锁, 记录之前之后的状态
#define LogEventItemLockAction( tpPlayer, nItemID, ullGUID, nFrom, nTo, nLockItem, nBind ) \
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_attr_lock, 0, nFrom, nTo, nLockItem, nBind)

// 普通绑定
#define LogEventItemBindAction( tpPlayer, nItemID, ullGUID, nFrom, nTo, nBindItem, nBind ) \
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_attr_bind, 0, nFrom, nTo, nBindItem, nBind)

// 装备易主, 记录角色ID
#define LogEventItemOwnerAction( tpPlayer, nItemID, ullGUID, nOwner, nBind ) \
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_attr_owner, 0, 0, 0, nOwner, nBind)

// 印鉴
#define LogEventItemMarkAction( tpPlayer, nItemID, ullGUID, nMarkItem, nBind ) \
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_attr_mark, 0, 0, 0, nMarkItem, nBind)

// 铸灵信息
// nTotalGrade:已铸灵总阶数
// nCastedCount:铸灵孔数
// nOpenedCount：开孔数
#define LogEventSpiritInfo( tpPlayer, nItemID, ullGUID, nTotalGrade, nCastedCount, nOpenedCount) \
	LogEventItemLevelUpAction((tpPlayer), nItemID, ullGUID, em_spirit_info, 0, nTotalGrade, nCastedCount, nOpenedCount, 0)

// sendmail
void LogEventPlayerSendMail(  int nRoleID, CMailCache *tpMail, int *pItemIDList, int *pItemNumList, int nItemNum, int nDesRoleID );

// ReceiveMail
void LogEventPlayerReceiveMail( int nRoleID, CMailCache *tpMail, int nTotalNum );

// DeleteMail
void LogEventPlayerDeleteMail( int nRoleID, CMailCache *tpMail, int nTotalNum );

// Corps Battle
void LogEventPlayerCorpsBattle( int nRoleID, int nCorpsID, int nAction, int nNum );

// chat
void LogEventPlayerSendChat( int nRoleID, const char* npName, const char* npMsg, int nGateWayID, int nChannel, int nIP );

// 初始化邮件消息
void InitialMailLogMsg( CMessageLogMail& tMsgMailLog );

// GET ITEM
#define LogEventGetItemByIbStore( tpPlayer, ullGuID, nItemID, nItemNum, nUnit, nPrice) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_IBSTORE, nUnit, nPrice)

#define LogEventGetItemByRecvMail(tpPlayer, ullGuID, nItemID, nItemNum, nFromRoleID, nMailID, nMailTime) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_RECV_MAIL, nFromRoleID, nMailID, nMailTime)

#define LogEventGetItemByStallBuy( tpPlayer, ullGuID, nItemID, nItemNum, nUnit, nPrice, nFromRoleID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_STALL_BUY, nUnit, nPrice, nFromRoleID)

#define LogEventGetItemByExchange( tpPlayer, ullGuID, nItemID, nItemNum, nFromRoleID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_EXCHANGE, nFromRoleID)

#define LogEventGetItemByAcceptTask( tpPlayer, ullGuID, nItemID, nItemNum, nTaskID ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_ACCEPT_TASK, nTaskID)

#define LogEventGetItemByEndTask( tpPlayer, ullGuID, nItemID, nItemNum, nTaskID ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_END_TASK, nTaskID)

#define LogEventGetItemByErating( tpPlayer, ullGuID, nItemID, nItemNum, nActivityID ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_ERATING, nActivityID)

//
#define LogEventGetItemByPickup( tpPlayer, ullGuID, nItemID, nItemNum, nPosX, nPosY, nLevel ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_PICKITEM, nPosX, nPosY, nLevel)

#define LogEventGetItemByProduce( tpPlayer, ullGuID, nItemID, nItemNum, nConsumeItemID, nConsumeItemNum ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_PRODUCE, nConsumeItemID, nConsumeItemNum)

#define LogEventGetItemByYQC( tpPlayer, ullGuID, nItemID, nItemNum, nDingItemID, nDingGUID ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_CHANGE, nDingItemID, 0, nDingGUID)

#define LogEventGetItemByRunTask( tpPlayer, ullGuID, nItemID, nItemNum, nTaskID, nFromTplID ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_RUN_TASK, nTaskID, nFromTplID)

//
#define LogEventGetItemByNpcShop( tpPlayer, ullGuID, nItemID, nItemNum, nUnit, nPrice, nNpcID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_NPC_SHOP, nUnit, nPrice, nNpcID)

#define LogEventGetItemByGuaJiShop( tpPlayer, ullGuID, nItemID, nItemNum, nUnit, nPrice, nNpcID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_GUAJI_SHOP, nUnit, nPrice, nNpcID)

#define LogEventGetItemByDefault( tpPlayer, ullGuID, nItemID, nItemNum ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_DEFAULT)

#define LogEventGetItemByTaskItem( tpPlayer, ullGuID, nItemID, nItemNum, nTaskID, nTaskItemID ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_TASKITEM, nTaskID, nTaskItemID)

#define LogEventGetItemByCollect( tpPlayer, ullGuID, nItemID, nItemNum, nPosX, nPosY, nTaskID ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_COLLECT, nPosX, nPosY, nTaskID)

#define LogEventGetItemByCompose( tpPlayer, ullGuID, nItemID, nItemNum, nSrcItemID, nSrcItemNum) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_COMPOSE, nSrcItemID, nSrcItemNum)

#define LogEventGetItemByRepetion( tpPlayer, ullGuID, nItemID, nItemNum, nRepetionBeginMapID, nReason, nRepetionKey ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_REPETION, nRepetionBeginMapID, nReason, nRepetionKey)

#define LogEventGetItemByGmInsert( tpPlayer, ullGuID, nItemID, nItemNum) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_GMINSERT, (tpPlayer)->GetPosX(), (tpPlayer)->GetPosY())

#define LogEventGetItemByRollBack( tpPlayer, ullGuID, nItemID, nItemNum, nPosX, nPosY) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_ROLLBACK, nPosX, nPosY)

#define LogEventGetItemByDirectIns( tpPlayer, ullGuID, nItemID, nItemNum, nDropType, nSrcTplID, nFromID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_DIRECTINS, nDropType, nSrcTplID, nFromID)

#define LogEventGetItemByUnBound( tpPlayer, ullGuID, nItemID, nItemNum, nUnBindTplID, ullGuID2) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_UNBOUND, nUnBindTplID, 0, ullGuID2)

#define LogEventGetItemByDeCompound( tpPlayer, ullGuID, nItemID, nItemNum, nSrcTplID, ullGuID2) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_DECOMPOUND, nSrcTplID, 0, ullGuID2)

#define LogEventGetItemByInvest( tpPlayer, ullGuID, nItemID, nItemNum, nLibID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_INVESTIGATION, nLibID)

#define LogEventGetItemByBuyBack( tpPlayer, ullGuID, nItemID, nItemNum, nUnit, nPrice, nNpcID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_BUY_BACK, nUnit, nPrice, nNpcID)

#define LogEventGetItemByAward( tpPlayer, ullGuID, nItemID, nItemNum, nAwardID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_AWARD, nAwardID)

#define LogEventGetItemByGetStar( tpPlayer, ullGuID, nItemID, nItemNum, nSrcTplID, ullGuID2) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_GETSTAR, nSrcTplID, 0, ullGuID2)

#define LogEventGetItemByHonor( tpPlayer, ullGuID, nItemID, nItemNum, nHonorID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_HONOR, nHonorID)

#define LogEventGetItemByCarve( tpPlayer, ullGuID, nItemID, nItemNum, nSrcJewID ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_CARVE, nSrcJewID)
	
#define  LogEventGetItemByTreasureBowl( tpPlayer, ullGuID, nItemID, nItemNum ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_TREASUREBOWL, 0 )

#define LogEventGetItemByMWQuickUp(tpPlayer, ullGuID, nItemID, nItemNum, nSrcID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_MWQUICKUP, nSrcID )

#define LogEventGetItemByMWQLCompose(tpPlayer, ullGuID, nItemID, nItemNum) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_QLCOMPOSE )

#define LogEventGetItemBySpiritDowngrade( tpPlayer, ullGuID, nItemID, nItemNum, nPosition, nType, nSlot) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_GET_BY_SPIRIT_DOWNGRID, nPosition, nType, nSlot)

// LOST ITEM
#define LogEventLostItemBySendMail(tpPlayer, ullGuID, nItemID, nItemNum, nToRoleID, nMailID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_SEND_MAIL, nToRoleID, nMailID)

#define LogEventLostItemByStallSell(tpPlayer, ullGuID, nItemID, nItemNum, nToRoleID, nUnit, nPrice ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_STALL_SELL, nToRoleID, nUnit, nPrice)

#define LogEventLostItemByExchange(tpPlayer, ullGuID, nItemID, nItemNum, nToRoleID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_EXCHANGE, nToRoleID )

#define LogEventLostItemByAcceptTask(tpPlayer, ullGuID, nItemID, nItemNum, nTaskID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_ACCEPT_TASK, nTaskID )

#define LogEventLostItemByEndTask(tpPlayer, ullGuID, nItemID, nItemNum, nTaskID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_END_TASK, nTaskID )

#define LogEventLostItemByErating(tpPlayer, ullGuID, nItemID, nItemNum, nActivityID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_END_TASK, nActivityID )

#define LogEventLostItemByThrowItem(tpPlayer, ullGuID, nItemID, nItemNum) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_THROW_ITEM, (tpPlayer)->GetPosX(), (tpPlayer)->GetPosY())

#define LogEventLostItemByProduce(tpPlayer, ullGuID, nItemID, nItemNum, nProduceItemID, ullProdGuID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_PRODUCE, nProduceItemID, 1, ullProdGuID )

#define LogEventLostItemByUse(tpPlayer, ullGuID, nItemID, nItemNum ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_USE, (tpPlayer)->GetPosX(), (tpPlayer)->GetPosY())

#define LogEventLostItemByRepetion( tpPlayer, ullGuID, nItemID, nItemNum ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_REPETION, (tpPlayer)->GetPosX(), (tpPlayer)->GetPosY())

#define LogEventLostItemByOpenSlot(tpPlayer, ullGuID, nItemID, nItemNum, nSlotedTplID, ullSlotGuID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_OPEN_SLOT, nSlotedTplID, 0, ullSlotGuID)

#define LogEventLostItemByCompose(tpPlayer, ullGuID, nItemID, nItemNum, nComposedItemTplID, nComposeItemNum, ullCompGuID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_COMPOSE, nComposedItemTplID, nComposeItemNum, ullCompGuID)

#define LogEventLostItemByEat(tpPlayer, ullGuID, nItemID, nItemNum, nEatingItemID, ullGuID2) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_EAT, nEatingItemID, 0, ullGuID2)


#define LogEventLostItemByInsSlot(tpPlayer, ullGuID, nItemID, nItemNum, nSlotedItemID2, ullSlotedGuID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_INSERT_SLOT, nSlotedItemID2, 0, ullSlotedGuID)

#define LogEventLostItemByUpgrade(tpPlayer, ullGuID, nItemID, nItemNum, nSuccess, nEquipID, ullEquipGuID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_UPGRADE, nSuccess, nEquipID, ullEquipGuID)


#define LogEventLostItemByChange(tpPlayer, ullGuID, nItemID, nItemNum, nToItemID, ullToGuID2) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_CHANGE, nToItemID, 0, ullToGuID2)


#define LogEventLostItemByDecompound(tpPlayer, ullGuID, nItemID, nItemNum) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_DECOMPOUND)


#define LogEventLostItemBySell2Npc(tpPlayer, ullGuID, nItemID, nItemNum, nNpcID, nUnit, nPrice) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_SELL2NPC, nNpcID, nUnit, nPrice)

#define LogEventLostItemByJudge(tpPlayer, ullGuID, nItemID, nItemNum, nEquipID, nResult, nEquipGUID ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_JUDGE, nEquipID, nResult, nEquipGUID)

#define LogEventLostItemByBound( tpPlayer, ullGuID, nItemID, nItemNum, nBindTplID, ullGuID2) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_BOUND, nBindTplID, 1, ullGuID2)

#define LogEventLostItemByAbortTask( tpPlayer, ullGuID, nItemID, nItemNum, nTaskID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_ABORTTASK, nTaskID)

#define LogEventLostItemByDestroyItem(tpPlayer, ullGuID, nItemID, nItemNum) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_DESTROY, (tpPlayer)->GetPosX(), (tpPlayer)->GetPosY())

#define LogEventLostItemByTimeOut(tpPlayer, ullGuID, nItemID, nItemNum, nPos, nTimeoutType ) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_TIMEOUT, nPos, nTimeoutType)

#define LogEventStartUseTimeItem(tpPlayer,ullGuID,nItemID,nItemNum) \
	LogEventItemAction((tpPlayer),ullGuID, nItemID,nItemNum,EM_START_USE_TIME_ITEM)

#define LogEventLostItemByEquipPro(tpPlayer, ullGuID, nItemID, nItemNum, nItemTplID, ullItemGuID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_CHANGEEQUIP, nItemTplID, 0, ullItemGuID)

#define LogEventLostItemByGetStar(tpPlayer, ullGuID, nItemID, nItemNum, nSuccess, nEquipID, ullEquipGuID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_GETSTAR, nSuccess, nEquipID, ullEquipGuID)

#define LogEventLostItemByAddStar(tpPlayer, ullGuID, nItemID, nItemNum, nSuccess, nEquipID, ullEquipGuID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_ADDSTAR, nSuccess, nEquipID, ullEquipGuID)

#define LogEventLostItemByAddExtra(tpPlayer, ullGuID, nItemID, nItemNum, nSuccess, nEquipID, ullEquipGuID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_ADDEXTRA, nSuccess, nEquipID, ullEquipGuID)

#define LogEventLostItemByFangCun(tpPlayer, ullGuID, nItemID, nItemNum) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_FANGCUN)

#define LogEventLostItemByMaster(tpPlayer, ullGuID, nItemID, nItemNum) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_MASTER)

#define LogEventLostItemByCarve(tpPlayer, ullGuID, nItemID, nItemNum, nPos, nOldJewel, nNewJewel) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_CARVE, nPos, nOldJewel, nNewJewel)

#define LogEventLostItemByMWQuickUp(tpPlayer, ullGuID, nItemID, nItemNum) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_MWQUICKUP)

#define LogEventLostItemByMWQLCompose(tpPlayer, ullGuID, nItemID, nItemNum) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_QLCOMPOSE )

#define LogEventLostItemBySworn(tpPlayer, ullGuID, nItemID, nItemNum, nSwornID) \
	LogEventItemAction((tpPlayer), ullGuID, nItemID, nItemNum, EM_LOST_BY_SWORN, nSwornID)

// Lost Money
#define LogEventLostMoneyBySendMail(tpPlayer, nMoneyType, nMoneyNumber, nToRoleID, nMailID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_SEND_MONEY, nToRoleID, nMailID)

#define LogEventLostMoneyByPostalFee(tpPlayer, nMoneyType, nMoneyNumber, nToRoleID, nMailID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_POSTAL_FEE, nToRoleID, nMailID)

#define LogEventLostMoneyByBuy4NPC(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemNum, nItemGUID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_BUY4NPC, nItemID, nItemNum, nItemGUID)

#define LogEventLostMoneyByRepairAll(tpPlayer, nMoneyType, nMoneyNumber) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_REPAIR)

#define LogEventLostMoneyByRepairOne(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_REPAIR, nItemID, 0, nItemGUID)

#define LogEventLostMoneyByProduceEquip(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_PRODUCE_EQUIP, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByProducePhys(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_PRODUCE_PHYS, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByUpgradeEquip(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_UPGRADE_EQUIP, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByInsertRune(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_INSERT_RUNE, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByInsertJewel(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_INSERT_JEWEL, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByOpenSlot(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_OPEN_SLOT2, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByStudyTalent(tpPlayer, nMoneyType, nMoneyNumber, nTalentPoint, nTalentStudy) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_STUDY_TALENT, nTalentPoint, nTalentStudy)

#define LogEventLostMoneyByExchange(tpPlayer, nMoneyType, nMoneyNumber, nRoleID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_EXCHANGE_MONEY, (tpPlayer)->GetPosX(), (tpPlayer)->GetPosY(), nRoleID)

#define LogEventLostMoneyByRelife(tpPlayer, nMoneyType, nMoneyNumber) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_MONEY_RELIFE, (tpPlayer)->GetPosX(), (tpPlayer)->GetPosY())

#define LogEventLostMoneyByNpcTeleport(tpPlayer, nMoneyType, nMoneyNumber, nNpcID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_NPC_TELEPORT, nNpcID)

#define LogEventLostMoneyByBuyBack(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemNum, nItemGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_BUY_BACK, nItemID, nItemNum, nItemGUID)

#define LogEventLostMoneyByBindItem(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_BIND_ITEM, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByUnBindItem(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_UNBIND_ITEM, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByStudySkill(tpPlayer, nMoneyType, nMoneyNumber) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_STUDY_SKILL, (tpPlayer)->GetPosX(), (tpPlayer)->GetPosY())

#define LogEventLostMoneyByStoreMoney(tpPlayer, nMoneyType, nMoneyNumber, nStoreMoney) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_STORE_MONEY, nMoneyType, nStoreMoney)

#define LogEventLostMoneyByResumeBind(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_RESUME_BIND, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByRemoveJewel(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_REMOVE_JEWEL, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByJudgeEquip(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_JUDGE_EQUIP, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByChangeResist(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_CHANGE_RESIST, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByComposeJewel(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_COMPOSE_JEWEL, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByMultiCompose(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_MULTI_COMPOSE, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByUpLifeSkill(tpPlayer, nMoneyType, nMoneyNumber, nSKillID, nSkillLevel) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_UP_LIFESKILL, nSKillID, nSkillLevel)

#define LogEventLostMoneyByStudyCompose(tpPlayer, nMoneyType, nMoneyNumber, nComposeID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_STUDY_COMPOSE, nComposeID)

#define LogEventLostMoneyByJudgeFabao(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_JUDGE_FABAO, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByGuajiBuy(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemNum, nItemGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_GUAJI_BUY, nItemID, nItemNum, nItemGUID)

#define LogEventLostMoneyByBuildCorps(tpPlayer, nMoneyType, nMoneyNumber, nCorpsID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_BUILD_CORPS, nCorpsID)

#define LogEventLostMoneyByContrCorps(tpPlayer, nMoneyType, nMoneyNumber, nCorpsID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_CONTR_CORPS, nCorpsID)

#define LogEventLostMoneyByBuildFimaly(tpPlayer, nMoneyType, nMoneyNumber, nFamilyID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_BUILD_FAMILY, nFamilyID)

#define LogEventLostMoneyByContrFimaly(tpPlayer, nMoneyType, nMoneyNumber, nFamilyID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_CONTR_FAMILY, nFamilyID)

#define LogEventLostMoneyByStall(tpPlayer, nMoneyType, nMoneyNumber, nFromRoleID, nTplID, ullGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_STALL, nFromRoleID, nTplID, ullGUID)

#define LogEventLostMoneyByEquipPro(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_EQUIPCHANGE, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByAddExtra(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_ADD_EXTRA, nItemID, 1, nItemGUID)

#define LogEventLostMoneyByBourse(tpPlayer, nMoneyNumber ) \
		LogEventMoneyAction ( tpPlayer, CYuanBao::em_unbind_money, nMoneyNumber, EM_LOST_BY_BOURSE )

#define LogEventLostMoneyByWar(tpPlayer, nMoneyType, nMoneyNumber, nWarSerial) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_WAR, nWarSerial)

#define LogEventLostMoneyByTax(tpPlayer, nMoneyType, nMoneyNumber, nFromRoleID, nTplID, ullGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_TAX, nFromRoleID, nTplID, ullGUID)

#define LogEventLostMoneyByMaster(tpPlayer, nMoneyNumber ) \
		LogEventMoneyAction ( tpPlayer, CYuanBao::em_unbind_money, nMoneyNumber, EM_LOST_BY_MASTERMONEY )

#define LogEventLostMoneyByFee(tpPlayer, nMoneyType, nMoneyNumber, nFromRoleID, nTplID, ullGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_STALLFEE, nFromRoleID, nTplID, ullGUID)

#define LogEventLostMoneyByCarve(tpPlayer, nMoneyType, nMoneyNumber) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_CARVEMONEY)

#define LogEventLostMoneyByDecompos(tpPlayer, nMoneyType, nMoneyNumber) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_DECOMPOS)

#define LogEventLostMoneyByMWPrint(tpPlayer, nMoneyType, nMoneyNumber) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_MWPRINT)

#define LogEventLostMoneyByMWQLCompose(tpPlayer, nMoneyType, nMoneyNumber) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_MONEY_BY_QLCOMPOSE)

#define LogEventLostMoneyByBuy4FuncConsume(tpPlayer, nMoneyType, nMoneyNumber, nFuncID, nFuncNum) \
	LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_LOST_BY_BUY4FUNC, nFuncID, nFuncNum)

#define LogEventLostMoneyByCombatFame(tpPlayer, nMoneyNumber) \
		LogEventMoneyAction ( tpPlayer, CYuanBao::em_combat_fame, nMoneyNumber, EM_LOST_BY_FAME)

#define LogEventLostMoneyByRepeFame(tpPlayer, nMoneyNumber) \
		LogEventMoneyAction ( tpPlayer, CYuanBao::em_repe_fame, nMoneyNumber, EM_LOST_BY_FAME)

#define LogEventLostMoneyByCommonFame(tpPlayer, nMoneyNumber) \
		LogEventMoneyAction ( tpPlayer, CYuanBao::em_common_fame, nMoneyNumber, EM_LOST_BY_FAME)

#define LogEventLostMoneyBySpiritUpgrade(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
	LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_SPIRIT_UPGRADE, nItemID, nItemGUID)

#define LogEventLostMoneyBySpiritDowngrade(tpPlayer, nMoneyType, nMoneyNumber, nPosition, nType, nSlot) \
	LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_LOST_BY_SPIRIT_DOWNGRADE, nPosition, nType, nSlot)


// Get Money
#define LogEventGetMoneyByRecvMail(tpPlayer, nMoneyType, nMoneyNumber, nFromRoleID, nMailID, nMailTime) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_GET_BY_RECV_MONEY, nFromRoleID, nMailID, nMailTime)

#define LogEventGetMoneyBySell2NPC(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemNum, nItemGUID) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_GET_BY_SELL2NPC, nItemID, nItemNum, nItemGUID)

#define LogEventGetMoneyByPickup(tpPlayer, nMoneyType, nMoneyNumber) \
		LogEventMoneyAction ( tpPlayer, nMoneyType, nMoneyNumber, EM_GET_BY_PICKMONEY, (tpPlayer)->GetPosX(), (tpPlayer)->GetPosY())

#define LogEventGetMoneyByExchange(tpPlayer, nMoneyType, nMoneyNumber, nRoleID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_GET_BY_EXCHANGE_MONEY, (tpPlayer)->GetPosX(), (tpPlayer)->GetPosY(), nRoleID)


#define LogEventGetMoneyByGMInsert(tpPlayer, nMoneyType, nMoneyNumber) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_GET_BY_GMINSERT_MONEY, (tpPlayer)->GetPosX(), (tpPlayer)->GetPosY())

#define LogEventGetMoneyByStoreMoney(tpPlayer, nMoneyType, nMoneyNumber, nStoreMoney) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_GET_BY_STORE_MONEY, nMoneyType, nStoreMoney)

#define LogEventGetMoneyByRedBag(tpPlayer, nMoneyType, nMoneyNumber, nItemID, nItemGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_GET_BY_RED_BAG, nItemID, 1, nItemGUID)

#define LogEventGetMoneyByQuestLib(tpPlayer, nMoneyType, nMoneyNumber, nQuestLib) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_GET_BY_QUESTLIB, nQuestLib)

#define LogEventGetMoneyByErating(tpPlayer, nMoneyType, nMoneyNumber, nERatingID) \
		LogEventMoneyAction (tpPlayer, nMoneyType, nMoneyNumber, EM_GET_BY_ERATING_MONEY, nERatingID)

#define LogEventGetMoneyByCorps(tpPlayer, nMoneyType, nMoneyNumber, nCorpsID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_GET_BY_CORPS, nCorpsID)

#define LogEventGetMoneyByFamily(tpPlayer, nMoneyType, nMoneyNumber, nFamilyID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_GET_BY_FAMILY, nFamilyID)

#define LogEventGetMoneyByStall(tpPlayer, nMoneyType, nMoneyNumber, nSellRoleID, nTplID, ullGUID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_GET_BY_STALL, nSellRoleID, nTplID, ullGUID)

#define LogEventGetMoneyByTask(tpPlayer, nMoneyType, nMoneyNumber, nTaskID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_GET_BY_TASK, nTaskID)

#define LogEventGetMoneyByGift(tpPlayer, nMoneyType, nMoneyNumber, nTplID) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_GET_BY_GIFT, nTplID)

#define LogEventGetMoneyByBourseAccount( tpPlayer, nMoneyNumber ) \
		LogEventMoneyAction ( (tpPlayer), CYuanBao::em_unbind_money, nMoneyNumber, EM_GET_BY_BOURSE, 0 )

#define LogEventGetMoneyByWar(tpPlayer, nMoneyType, nMoneyNumber, nWarSerial) \
		LogEventMoneyAction ( (tpPlayer), nMoneyType, nMoneyNumber, EM_GET_BY_WAR, nWarSerial)

#define LogEventGetMoneyBuBourseErr( tpPlayer, nMoneyNumber ) \
		LogEventMoneyAction( (tpPlayer), CYuanBao::em_unbind_money, nMoneyNumber, EM_GET_BY_BOURSE_ERR, 0 )

#define LogEventGetMoneyByTeleErr( tpPlayer, nMoneyNumber ) \
		LogEventMoneyAction( (tpPlayer), CYuanBao::em_unbind_money, nMoneyNumber, EM_GET_BY_TELFAIL_ROLLBACK, 0 )

#define LogEventGetMoneyByCombatFame( tpPlayer, nMoneyNumber ) \
		LogEventMoneyAction( (tpPlayer), CYuanBao::em_combat_fame, nMoneyNumber, EM_GET_BY_FAME, 0 )

#define LogEventGetMoneyByRepeFame( tpPlayer, nMoneyNumber ) \
		LogEventMoneyAction( (tpPlayer), CYuanBao::em_repe_fame, nMoneyNumber, EM_GET_BY_FAME, 0 )

#define LogEventGetMoneyByCommonFame( tpPlayer, nMoneyNumber ) \
		LogEventMoneyAction( (tpPlayer), CYuanBao::em_common_fame, nMoneyNumber, EM_GET_BY_FAME, 0 )


//Get Buffer
#define LogEventGetBuffByPlayer(tpPlayer,BuffID,GetTime,SkillID,GivePlayerID ) \
	    LogEventRoleBuffAction(tpPlayer,BuffID,GetTime,SkillID,GivePlayerID,EM_BUFF_BY_SKILL)

#define LogEventGetBuffByItem(tpPlayer,BuffID,GetTime,ItemID) \
	    LogEventRoleBuffAction(tpPlayer,BuffID,GetTime,ItemID,0,EM_BUFF_BY_BUFFITEM)

#define LogEventGetBuffBySystem(tpPlayer,BuffID,GetTime) \
		LogEventRoleBuffAction(tpPlayer,BuffID,GetTime,0,0,EM_BUFF_BY_SYSTEM)

#define LogEventGetBuffByYQS(tpPlayer,BuffID,GetTime) \
		LogEventRoleBuffAction(tpPlayer,BuffID,GetTime,0,0,EM_BUFF_BY_YQSGROW)

#define LogEventGetBuffByMAP(tpPlayer,BuffID,GetTime,MapID) \
		LogEventRoleBuffAction(tpPlayer,BuffID,GetTime,MapID,0, EM_BUFF_BY_MAP)


//Lost Buffer


#define LogEventLostBuffByTimeout(tpPlayer,BuffID ) \
		LogEventRoleBuffAction(tpPlayer,BuffID,0,0,0,EM_BUFF_BY_TIMEOUT)
 
#define LogEventLostBuffBySelf(tpPlayer,BuffID) \
		LogEventRoleBuffAction(tpPlayer,BuffID,0,0,0,EM_BUFF_BY_SELF)

#define LogEventLostBuffByDeath(tpPlayer,BuffID ) \
		LogEventRoleBuffAction(tpPlayer,BuffID,0,0,0,EM_BUFF_BY_DEATH)

#define LogEventLostBuffByCloseAureole(tpPlayer,BuffID ) \
		LogEventRoleBuffAction(tpPlayer,BuffID,0,0,0,EM_BUFF_BY_CLOSEAUREOLE)

#define LogEventLostBuffByChangeMap(tpPlayer,BuffID ) \
		LogEventRoleBuffAction(tpPlayer,BuffID,0,0,0,EM_BUFF_BY_CHANGEMAP)

#define LogEventLostBuffByLogOff(tpPlayer,BuffID ) \
		LogEventRoleBuffAction(tpPlayer,BuffID,0,0,0,EM_BUFF_BY_LOGOFF)

#define LogEventLostBuffByStateStone(tpPlayer,BuffID ) \
		LogEventRoleBuffAction(tpPlayer,BuffID,0,0,0,EM_BUFF_BY_STATE_STONE)

// Exp
#define LogEventGetExpByHornor(tpPlayer, nExp, nFinalExp, nHornorID, nHornor) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_HORNOR, nFinalExp, nHornorID, nHornor, nExp)
	
#define LogEventGetExpByCommand(tpPlayer, nExp, nFinalExp) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_COMMAND, nFinalExp, (tpPlayer)->GetPosX(), (tpPlayer)->GetPosY(), nExp)

#define LogEventGetExpByCard(tpPlayer, nExp, nFinalExp, nCardID, ullGUID) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_CARD, nFinalExp, nExp, nCardID, ullGUID)

#define LogEventGetExpByMultiComp(tpPlayer, nExp, nFinalExp, nComposeID) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_MULTICOM, nFinalExp, nExp, nComposeID)

#define LogEventGetExpByTeamAllot(tpPlayer, nExp, nFinalExp, nTeamMember, nTplID) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_TEAM_ALLOT, nFinalExp, nExp, nTeamMember, nTplID)

#define LogEventGetExpByFimalyAllot(tpPlayer, nExp, nFinalExp, nTeamMember, nTplID) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_FAMILY_ALLOT, nFinalExp, nExp, nTeamMember, nTplID)

#define LogEventGetExpByQuestion(tpPlayer, nExp, nFinalExp, nQuestionType, nQuestTplID) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_QUESTION, nFinalExp, nExp, nQuestionType, nQuestTplID)

#define LogEventGetExpByInvestigation(tpPlayer, nExp, nFinalExp, nInvestLibID) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_INVESTIGATION, nFinalExp, nExp, nInvestLibID)

#define LogEventGetExpByErating(tpPlayer, nExp, nFinalExp, nEratingID) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_ERATING, nFinalExp, nExp, nEratingID)

#define LogEventGetExpByTask(tpPlayer, nExp, nFinalExp, nTaskID) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_TASK, nFinalExp, nExp, nTaskID)

#define LogEventGetExpByEntityDie(tpPlayer, nExp, nFinalExp, nTeamMember, nOrgeTempID) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_ENTITY_DIE, nFinalExp, nExp, nTeamMember, nOrgeTempID)

#define LogEventGetExpByBuff(tpPlayer, nExp, nFinalExp, nBuffTplID) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_BUFF, nFinalExp, nExp, nBuffTplID)

#define LogEventGetExpByHandin(tpPlayer, nExp, nFinalExp, nHandInTplID) \
        LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_HANDIN, nFinalExp, nExp, nHandInTplID)

#define LogEventGetExpByOffLine(tpPlayer, nExp, nFinalExp, nLeftTime) \
		LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_OFFLINE, nFinalExp, nExp, nLeftTime)


#define LogEventGetExpByFlower(tpPlayer, nExp, nFinalExp) \
	LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_FLOWER, nFinalExp, nExp)

#define LogEventGetExpByExpItem(tpPlayer, nExp, nFinalExp) \
	LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_EXPITEM, nFinalExp, nExp)
	
#define LogEventGetExpByBlackPill(tpPlayer, nExp, nItemID) \
	LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_BLACKPILLS, nExp, nItemID )
	
#define LogEventGetExpByExpPill(tpPlayer, nExp, nItemID) \
	LogEventRoleExpAction( (tpPlayer), EM_EXP_BY_EXPILLS, nExp, nItemID )

// TalentExp, MWExp, ProExp, LifeExp
#define LogEventGetTalentByDefault(tpPlayer, nExp) \
	LogEventTalentExpAction (tpPlayer, EM_EXP_BY_ENTITY_DIE, nExp)

#define LogEventGetTalentByGmInsert(tpPlayer, nExp) \
	LogEventTalentExpAction (tpPlayer, EM_GET_BY_GMINSERT, nExp)

#define LogEventGetMWExpByDefault(tpPlayer, tpMW,  nExp) \
	LogEventMWExpAction (tpPlayer, tpMW, EM_EXP_BY_ENTITY_DIE, nExp)

#define LogEventGetProExpByProduce(tpPlayer, pItemObject, nExp) \
	LogEventProduceExpAction( tpPlayer, pItemObject, EM_GET_BY_PRODUCE, nExp)


// EntityDie
#define LogEventPlayerKilledByPlayer(tpPlayer, tpKiller)	\
		LogEventPlayerDieAction( (tpPlayer), (tpKiller)->GetEntityType(), \
				(tpKiller)->GetCharID(), (tpKiller)->GetMetier(), (tpKiller)->GetLevel())
				

#define LogEventPlayerKilledByOther(tpPlayer, tpOther)	\
		LogEventPlayerDieAction((tpPlayer), tpOther->GetEntityType(), \
				(tpOther)->GetProperty()->GetPropertyTempID(), 0, (tpOther)->GetProperty()->GetLevel())

// EntityRelive
// 被其他玩家复活, tpPlayer是复活后的玩家
#define LogEventPlayerReliveByPlayer(tpPlayer, tpReliver)	\
		LogEventPlayerDieAction((tpPlayer), 0x04000000, \
				(tpReliver)->GetCharID(), (tpReliver)->GetMetier(), (tpReliver)->GetLevel())


// 玩家自己选择复活方式, 或者系统复活, tpPlayer 是复活后的玩家
#define LogEventPlayerReliveByNormal(tpPlayer, nLineID, nMapID, nX, nY)	\
		LogEventPlayerDieAction((tpPlayer), 0x20000000, RELIVE_NORMAL, ((nLineID) << 16) + nMapID, ((nX) << 16) + nY)

#define LogEventPlayerReliveByWeak(tpPlayer, nLineID, nMapID, nX, nY)	\
		LogEventPlayerDieAction((tpPlayer), 0x10000000, RELIVE_WEAKGRAVE, ((nLineID) << 16) + nMapID, ((nX) << 16) + nY)

#define LogEventPlayerReliveBySkill(tpPlayer, nLineID, nMapID, nX, nY)	\
		LogEventPlayerDieAction((tpPlayer), 0x08000000, RELIVE_SKILL, ((nLineID) << 16) + nMapID, ((nX) << 16) + nY)

#define LogEventPlayerReliveBySystem(tpPlayer, nLineID, nMapID, nX, nY)	\
		LogEventPlayerDieAction((tpPlayer), 0x40000000, RELIVE_SYSTEM, ((nLineID) << 16) + nMapID, ((nX) << 16) + nY)

// Task
#define LogEventPlayerAcceptTask(tpPlayer, nTaskID) \
		LogEventTaskAction( (tpPlayer), nTaskID, EM_TASK_ACCEPT)

#define LogEventPlayerEndTask(tpPlayer, nTaskID) \
		LogEventTaskAction( (tpPlayer), nTaskID, EM_TASK_END)

#define LogEventPlayerCancelTask(tpPlayer, nTaskID, nParam ) \
		LogEventTaskAction( (tpPlayer), nTaskID, EM_TASK_CANCEL, nParam )

#define LogEventPlayerRefreshTask(tpPlayer, nTaskID) \
		LogEventTaskAction( (tpPlayer), nTaskID, EM_TASK_REFRESH)

#define LogEventPlayerPublishTask(tpPlayer, nTaskID, nParam) \
		LogEventTaskAction( (tpPlayer), nTaskID, EM_TASK_PUBLISH, nParam)

// RoleLevelUp
#define LogEventPlayerExpLevelUp(tpPlayer, nDeltaLevel)  \
	LogEventPlayerLevelUpAction( tpPlayer, EM_UPGRADE_ROLE_EXP, 0, tpPlayer->GetLevel(), nDeltaLevel)

#define LogEventPlayerLifeSkillLevelUp(tpPlayer, tpLifeSkill) \
	LogEventPlayerLevelUpAction( tpPlayer, EM_UPGRADE_LIFE_EXP, tpLifeSkill->GetLifeSkillType(), tpLifeSkill->GetLifeSkillLevel())

#define LogEventPlayerProduceLevelUp(tpPlayer, nDeltaLevel) \
	LogEventPlayerLevelUpAction( tpPlayer, EM_UPGRADE_PRODUCE_EXP, 0, \
			((CPropertyPlayer*) pPlayer->GetProperty())->GetProLevel(), nDeltaLevel)

#define LogEventPlayerTalentLevelUp(tpPlayer, nStageID, nSubStageID) \
	LogEventPlayerLevelUpAction( tpPlayer, EM_UPGRADE_TALENT_EXP, nStageID, nSubStageID)
	

// Mail
#define LogEventSendMail( nRoleID, tpMail, pItemIDList, pItemNumList, nNum, nDesRoleID )	  \
	LogEventPlayerSendMail( nRoleID, tpMail, pItemIDList, pItemNumList, nNum, nDesRoleID )

#define LogEventReceiveMail( nRoleID, tpMail, nTotalNum ) \
	LogEventPlayerReceiveMail( nRoleID, tpMail, nTotalNum )

#define LogEventDeleteMail( nRoleID, tpMail, nTotalNum ) \
	LogEventPlayerDeleteMail( nRoleID, tpMail, nTotalNum )


// chat
#define LogEventSendChat( nRoleID, npName, npMsg, nGateWayID, nChannel, nIP )\
	LogEventPlayerSendChat( nRoleID, npName, npMsg, nGateWayID, nChannel, nIP )
	
// npc
#define  LogBossOgreDie( pOgre, pKiller, pTempOgre )	\
RecodrdBossOgreDie( pOgre, pKiller, pTempOgre )
	
// lifeskill
#define  LogLifeSkillAction( nRoleID, pLifeSkill, nActionKind, nComposeID ) \
LogLifeSkill( nRoleID, pLifeSkill, nActionKind, nComposeID )

// matrix
#define  LogMatrixAddSlave( nSlaveID, nMasterID, nMasterLevel, nSlaveNum1, nSlaveNum2 ) \
LogMatrix( nSlaveID, nMasterID, EM_MATRIX_ADDSLAVE, nMasterLevel, nSlaveNum1, nSlaveNum2 )

#define  LogMatrixAddMaster( nSlaveID, nMasterID, nSlaveLevel, nMasterNum ) \
LogMatrix( nSlaveID, nMasterID, EM_MATRIX_ADDMASTER, nSlaveLevel, nMasterNum )

#define  LogMatrixAddPoint( nSlaveID, nMasterID, nRawPoint, nRealPoint ) \
LogMatrix( nSlaveID, nMasterID, EM_MATRIX_ADDPOINT, nRawPoint, nRealPoint )

#define  LogMatrixDelSlave( nSlaveID, nMasterID, nMasterLevel, nSlaveNum1, nSlaveNum2 ) \
LogMatrix( nSlaveID, nMasterID, EM_MATRIX_DELSLAVE, nMasterLevel, nSlaveNum1, nSlaveNum2 )

#define  LogMatrixDelMaster( nSlaveID, nMasterID, nSlaveLevel, nMasterNum ) \
LogMatrix( nSlaveID, nMasterID, EM_MATRIX_DELSLAVE, nSlaveLevel, nMasterNum )

#define  LogMatrixMasterPercent( nSlaveID, nMasterID, nOldPercent, nNewPercent ) \
LogMatrix( nSlaveID, nMasterID, EM_MATRIX_MASTERPERCENT, nOldPercent, nNewPercent )


// 副本积分日志

#define  LogEventPlayerRepeScoreByOrge (tpPlayer, CurrentScore, DeltaScore, nParam1, nParam2 ) \
LogEventPlayerRepeScore(tpPlayer, CurrentScore, DeltaScore, nParam1, nParam2, EM_SCORE_COMMON_ORGE)


#define  LogEventPlayerRepeScoreByBoss (tpPlayer, CurrentScore, DeltaScoren, nParam1, nParam2 ) \
LogEventPlayerRepeScore(tpPlayer, CurrentScore, DeltaScore, nParam1, nParam2 , EM_SCORE_BOSS_ORGE)


#define  LogEventPlayerRepeScoreByExtra (tpPlayer, CurrentScore, DeltaScore, nParam1, nParam2 ) \
LogEventPlayerRepeScore(tpPlayer, CurrentScore, DeltaScore, nParam1, nParam2, EM_SCORE_EXTRAL)


#define  LogEventPlayerRepeScoreByPVP (tpPlayer, CurrentScore, DeltaScore, nParam1, nParam2 ) \
LogEventPlayerRepeScore(tpPlayer, CurrentScore, DeltaScore, nParam1, nParam2, EM_SCORE_KILL_PLAYER)

