#pragma once

#include "logservertype.h"
#include "servertool.h"
#include "log_msg_pb.hxx.pb.h"

class CEntity;
class CEntityNpc;
class CEntityPlayer;
class CMessage;
class CMessageCreatePlayerProNotice;
class CMessageCreateNpcProNotice;
class CItemObject;
class CTplItem;
class CWTPoint;

// 为了保证Build出来的消息不会超出Buff(32k)大小，对某些打包消息要做限制，超出则分包发送
#define MAX_NPC_NUM_OF_CREATE_MSG		256	// BuildCreateNpcProMessage 里一次打包的NPC信息个数
#define MAX_PLAYER_NUM_OF_CREATE_MSG	128	// BuildCreatePlayerProMessage 里一次打包的player信息个数


// 构建创建玩家消息
void SetPlayerProMessageForSinglePlayer( CEntityPlayer* pPlayer, CMessageCreatePlayerProNotice& rCreatePlayerProMsg );
CMessage* BuildCreatePlayerProMessage(std::vector<CEntityPlayer*>* pPlayerSet);
CMessage* BuildCreatePlayerProMessage(CEntityPlayer* pPlayer);

// 构建创建npc消息
void SetNpcProMessage( CEntity* pEntity, CMessageCreateNpcProNotice& rCreatNpcProMsg, int nListID, CEntityPlayer* pSelf );
CMessage* BuildCreateNpcProMessage(std::vector<CEntity*>* pNpcSet, int vListID = 0, CEntityPlayer* pSelf = NULL );
CMessage* BuildCreateNpcProMessage(CEntity* pEntity, int vListID = 0, CEntityPlayer* pSelf = NULL );

// 构建销毁实体消息
CMessage* BuildDestroyEntityMessage( CEntity* pEntitySelf );
CMessage* BuildDestroyEntityMessage( std::vector<CEntityPlayer*>* pPlayers, std::vector<CEntity*>* pNpcs );

// 构建实体移动消息
CMessage* BuildEntityPathMessage( CEntity* pEntity, const CWTPoint& rSrcPos, const CWTPoint& rDstPos );

// 构建结果集消息
CMessage* BuildFuncResultMessage( CEntity* pEntity, unsigned short vFunc, unsigned short vResult, int vListID, unsigned int vValue1, unsigned int vValue2, unsigned int vValue3 = 0, unsigned int vValue4 = 0, unsigned int vValue5 = 0 );

// 构建改变状态的消息
CMessage* BuildChangeStateNoticeMessage(unsigned int vEntityID, int vState, int vParame1, int vParame2, bool vInState );


// Exp
CMessage* BuildPlayerOpExpLogMessage( CEntityPlayer* pPlayer, 
		EUpgradeType emType, uint64_t nCurrent, int nDelta, int nOpType, int nParam1, int nParam2, uint64_t ullParam3);

// Item
CMessage* BuildPlayerOpItemLogMessage( CEntityPlayer* pPlayer, uint64_t ullGuID, 
		int nItemID, int nItemNum, int nTotal, EItemAction emAction, int nParam1, int nParam2, uint64_t ullParam3);

// Money
CMessage* BuildPlayerOpMoneyLogMessage( CEntityPlayer* pPlayer, int nUnit, int nDelta, 
		EMoneyAction emAction, int nParam1 , int nParam2 , uint64_t ullParam3 );

// Buff

CMessage* BuildPlayerBuffLogMessage( CEntityPlayer* pPlayer, int BuffID, int PersistTime, 
		 int nParam1 , int nParam2 , unsigned short bufftype );

// Logout
CMessage* BuildPlayerLogoutMessage( CEntityPlayer* pPlayer, EMLeaveReason emLeave, EMSaveResult emSave);

// Killed
CMessage* BuildPlayerBeKilledLogMessage( CEntityPlayer* pPlayer, 
		int nKillerType, int nKillerID, int nKillerMetier, int nKillerLevel);

// Task
CMessage* BuildPlayerOpTaskLogMessage( CEntityPlayer* pPlayer, 
		int nTaskID, ETaskAction emAction, int nParam1, int nParam2, uint64_t ullParam3 );

// NpcDrop
CMessage* BuildNpcDropLogMessage( CEntityNpc* pDropingEntity, std::vector<CEntity*>& rDropedEntityVec);

// UseItemDrop
CMessage* BuildUseItemDropLogMessage( CEntityPlayer* pPlayer, int nItemID, std::vector<CEntity*>& rDropedEntityVec );

// RoleLevelUp
CMessage* BuildPlayerLevelUpLogMessage( CEntityPlayer* pPlayer, EUpgradeType emUpgrade, int SubType, int nCurrentLevel, int nLevel);


// EntityDispear
CMessage* BuildDropedItemDestroyLogMessage(CEntity* pEntity);

// ItemLevelUp
CMessage* BuildItemLevelUpLogMessage(CEntityPlayer* pPlayer, 
		int nItemID, uint64_t ullGUID, int nType, int nResult, int nCurrLevel, int nPrevLevel, int nItem1, int nItem2);
