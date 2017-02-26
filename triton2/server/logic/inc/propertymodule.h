#pragma once
#include "template.h"
#include "pro_define.h"
#include "logservertype.h"	  
#include "servercore.h"
#include "servertool.h"
#include "lk_hashmap.h"
#include "stallmessage_pb.hxx.pb.h"
#include "mapmodule.h"

class CFuncResult;
class CObj;
class CEntity;
class CEntityPlayer;
class CEntityCharacter;
class CEntityPet;
class CProperty;
class CPropertyCharacter;
class CPropertyPlayer;
class TiXmlDocument;

class CSpecialRelive
{
public:
	int			mMapID;	
	int			mLineID;	
	int			mX;
	int			mY;
};

class CRelivePosition : public CObj
{
public:
	CRelivePosition()
	{
		if (CObj::msCreateMode)
		{
			Initialize();
		}
		else
		{
			Resume();
		}
	}
	virtual ~CRelivePosition() {}

	// 初始化
	virtual int Initialize()
	{
		mPosition.Clear();
		mReliveMapID = 0;
		mLineID = 0;
		mMapID = 0;
		mSpecial = 0;
		memset( mCampPos, 0, sizeof(mCampPos) );
		return 0;
	}

	// 恢复
	virtual int Resume() {return 0;}

public:
	CWTPoint	mPosition;		// 复活点坐标
	int			mReliveMapID;	// 复活地图, 0表示当前地图
	int			mLineID;
	int			mMapID;			// 地图ID
	int			mSpecial;
	CSpecialRelive  mCampPos[ WORLD_NUM ];		// 各阵营复活点

public:
	CSpecialRelive *GetPosByNation( int vWorldID )
	{
		if ( vWorldID >= WORLD_FIRST && vWorldID < WORLD_NUM )
		{
			return &mCampPos[ vWorldID ];
		}
		return NULL;
	}

	void AddSpecialRelive( int vWorldID, int vLineID, int vMapID, int vX, int vY )
	{
		if ( vWorldID >= WORLD_FIRST && vWorldID < WORLD_NUM )
		{
			mCampPos[ vWorldID ].mLineID = vLineID;
			mCampPos[ vWorldID ].mMapID = vMapID;
			mCampPos[ vWorldID ].mX = vX;
			mCampPos[ vWorldID ].mY = vY;
			mSpecial ++;
		}
	}
};
enum ETimer
{
	STATE_INTERVAL		= 200,		// 状态变化时间间隔
	REFRESH_INTERVAL	= 3000,		// 恢复时间间隔
};


class CDropFrom
{
public:
	enum 
	{
		em_drop_from_item,
		em_drop_from_npc,
	}emDropFrom;

	union 
	{
		CEntity* pEntity;
		CItemObject* pItemObject;
	} mFrom;

	CDropFrom(CEntity* pEntity)
	{
		emDropFrom = em_drop_from_npc;
		mFrom.pEntity = pEntity;
	}

	CDropFrom(CItemObject* pItemObject)
	{
		emDropFrom = em_drop_from_item;
		mFrom.pItemObject = pItemObject;
	}
};

enum EExchangeFlag
{
	em_exchange_by_stall,
	em_exchange_by_trade,
	em_exchange_by_stall_yb,
};

// 内丹物品
class CRedstoneController : public CObj
{
private:
	int mEntityID;				// 内丹所依附的实体ID
	int mOnwerID;				// 内丹的拥有者ID，根据内丹的类型确定
	int mRedStoneTmpID;			// 内丹的模板ID
	bool mDestroyEntity;		// 内丹噬炼结束时是否销毁内丹所依附的实体
	int	mLastNumber;			// 上次分配经验时的人数
public:
	// 设置&获取内丹所依附的实体ID
	void SetEntityID( int nEntityID ){ mEntityID = nEntityID; }
	int GetEntityID( ){ return mEntityID; }
	
	// 设置&获取内丹的拥有者ID
	void SetOwnerID( int nOnwerID ){ mOnwerID = nOnwerID; }
	int	GetOwnerID( ){ return mOnwerID; }
	
	// 设置&获取内丹的模板ID
	void SetRedstoneTmpID( int nRedstoneTmpID ){  mRedStoneTmpID = nRedstoneTmpID; }
	int	 GetRedstoneTmpID(){ return mRedStoneTmpID; }
	
	// 设置&获取是否消耗内丹实体
	void SetDestroyEntity( bool bDestroyEntity ){ mDestroyEntity = bDestroyEntity; }	
	bool GetDestroyEntity(){ return mDestroyEntity; }
	
	// 设置&获取上次分配经验时的人数
	void SetLastNumber( int nNumber ){ mLastNumber = nNumber; }
	int GetLastNumber(){ return mLastNumber; }
public:
	CRedstoneController()
	{
		if ( CObj::msCreateMode )
		{
			Initialize();		
		}
		else
		{
			Resume();
		}
	}
	~CRedstoneController( ){ }	
public:
	int Initialize()
	{	
		mEntityID = 0;
		mOnwerID = 0;
		mRedStoneTmpID = 0;
		mDestroyEntity = false;
		mLastNumber = 0;
		return SUCCESS;
	}
	int Resume()
	{
		return SUCCESS;
	}	
};



class CPropertyModule : public CLogicModule, public CSingleton< CPropertyModule >
{
public:
	static char	sModuleName[ NAME_LENGTH ];
	CWTimer								mRecTimer;			// 恢复计时器
	CWTimer								mStateTimer;		// 状态计时器
	CWTimer								mAureoleTimer;		// aureole timer
	CWTimer								mDaliyRefreshTimer;	// 每日刷新计时器
	CWTimer								mPkTimer;			// 在线pk刷新计时器
	unsigned int						mLastWeeklyTimer;	// 处理每周事务计时器
	unsigned int						mLastDaylyTimer;	// 处理每日事务计时器

	typedef lk::hash_map< int, int, 200 >	RelivePositionMap; // 各地图重生点

	RelivePositionMap					mRelivePosition;	// 各地图重生点
	int									mBorderMapID;		// 边境复活地图
	int									mBorderPosX;		// 边境复活点X
	int									mBorderPosY;		// 边境复活点Y

	CClientControlData					mClientCtrlData;	// 控制客户端模块管理器

public:
	

public:
	CPropertyModule( );
	~CPropertyModule( );

	static unsigned int CountSize();				    

public:
	// 启动服务器
	virtual void OnLaunchServer( );

	// 退出服务器
	virtual void OnExitServer( );

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );

	// 新一天到来
	void OnNewDayCome( );

	// 新一周到来
	void OnNewWeekCome( );
	
	// 建立场景
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );

	// 路由消息
	virtual void OnRouterMessage( CMessage* pMessage );

	// 客户端发送消息
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 内部超时消息
	virtual void OnTimeoutMessage( CMessage* pMessage );

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );

	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer , int vSaveCode );

	// 加载配置
	virtual void OnLoadConfig( );

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }

public:

	int SaveItemInfo( CEntityPlayer* pPlayer, int* apPkgIdx, int nIdxNum, int nSaveCode );

	// 实体是否同一个国家的实体
	bool EntityIsNative( CEntity* pSrcEntity, CEntity* pDesEntity );

	// 实体是否同一个同盟国家的实体
	bool EntityIsAlly(	CEntity* pSrcEntity, CEntity* pDesEntity );

	// 释放技能
	void FireSkill(	CEntityCharacter* pSrcEntity, 
					CEntityCharacter* pDesEntity, 
					CTemplateSkill* pSkill, 
					CEntityCharacter** vEntityList,
					int vEntityListNum,
					bool bIsMagicWeapon = false // 是否法宝技能
				);
	
	// PK模式判定
	bool EntityPKValid(	CEntity* pSrcEntity, 				
						CEntity* pDesEntity, 
						CTemplateSkill* pNormalSkill, 
						bool vIsSkill = true
					);

	// 实体删除所有负面BUFF
	void EntityRemoveDebuff( CEntityCharacter* pEntity );
	
	// 清除BUFF
	void EntityClearbuff( CEntityCharacter* pEntity, int vNum, int vBuffType );

	// 实体受到伤害
	void EntityDamaged( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, int vDamage, EResultType vResultType, int vSkillType = 0 );

	// 攻击技能效果和伤害计算作用于目标
	void EntityAttackSkill( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateNormalSkill* pNormalSkill, int vSkillType, int vDamageRate = SERVER_PERCENT_INT );

	// 技能BUFF效果作用于目标
	void EntityBuffSkill( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, int* pBuffList, int nBuffNum, int vDamage, int vSkillType = 0 );

	// 释放技能时,判断武器是否满足
	bool SkillWeaponValid( CPropertyPlayer* pSrcProperty, CTemplateSkill* pSkill );

	// 玩家重置所有属性点
	void ResetAllPP( CEntityPlayer* pEntity );

	// 玩家重置所有技能点
	void ResetAllSP( CEntityPlayer* pEntity );

	// 玩家重置所有造化点
	int ResetAllTalent( CEntityPlayer* pEntity, int vStep );

	// 玩家重置一点技能点
	void ResetSingleSP( CEntityPlayer* pEntity, unsigned short vGenerID, unsigned char vCol, unsigned char vRow );

	// 玩家配置技能点
	int ClickSP( CEntityPlayer* pEntity, unsigned short vGenerID, unsigned char vCol, unsigned char vRow, unsigned int& vSkillID, int& vIndex, int& vMoney, int& vBindMoney );

	// 玩家获得修为值
	void PlayerObtainTalentExp( CEntityPlayer* pEntity, int vExp );

	// 玩家点击造化点
	int PlayerClickTalentPoint( CEntityPlayer* pEntity, int vRow, int vCol );

	// 玩家造化进阶
	int PlayerIncreaseTalent( CEntityPlayer* pEntity, int vStep );

	// 玩家获得某类才华值
	int PlayerObtainWizardValue( CEntityPlayer* pEntity, int vType, int vValue );

	// 玩家用功勋兑换才华值
	void PlayerChangeHonorToWizard( CEntityPlayer* pPlayer, int vHonor, int vType );
 
	// 玩家获得经验
	// 新加参数vReason, 区分来自打怪还是任务, 因为打怪的经验需要分享给队友,任务的不会
	// vReason = 0 打怪, 1 任务	 ( 查看枚举 EExpReason)
	int PlayerObtainExp( CEntityPlayer* pEntity, int vExp, int vReason = EXPREASON_TASK, int* pMemberList = NULL, int vMemberCount = 1, int vLevel = 0, CEntity* pNPCEntity = NULL );

	// 玩家获得金钱
	void PlayerObtainMoney( CEntityPlayer* pEntity, int vInitMoney, int vDeltaMoney, int* pMemberList, int vMemberCount, int vLevel );

	// 获得道具时任务系统处理
	void OnTaskObtainItem( CEntityPlayer* pEntity, unsigned int vItemID, unsigned short vNumber );

	// 失去道具时任务系统处理
	void OnTaskRemoveItem( CEntityPlayer* pPlayer, unsigned int vItemID, unsigned short vNumber );

	// 玩家获得一个已经存在的道具, 返回此道具
	CItemObject* PlayerInsertItem( CEntityPlayer* pEntity, CItemObject* pItem , bool vIsFromTask = false, bool bBind = false );

	// 玩家获得道具
	// 返回包裹中此道具，如果是堆叠物品，返回最后一堆叠道具
	CItemObject* PlayerInsertItem( CEntityPlayer* pEntity, 
			unsigned int vItemID, unsigned int vNumber, bool vIsFromTask = false, int nLevel = 0, const char* szMadeByName = NULL, bool bBind = false, int nInvalidTime=0);

	// 创建物品
	int CreateItem(CTplItem* tpItem,  uint32_t vNumber, int  vLevel, CItemObject** tpItemObj);

	// 个性化物品
	int PersonalizeItem(CEntityPlayer* pEntity, CTplItem* tpItem , CItemObject* pItemObj, const char* szMadeByName);

	// 获得物品后的处理
	void  AfterInsertedItem(CEntityPlayer* pEntity, CTplItem* pTplItem, CItemObject* tpItemObj, bool bIsCalledFromTask);

	// 玩家失去道具
	int PlayerRemoveItem( CEntityPlayer* pPlayer, 
			unsigned int vItemIndex, int vItemNumber, uint64_t *pullGUID = NULL, bool vIsTask = false ); 

	// 玩家失去道具
	int PlayerRemoveItemByID( CEntityPlayer* pPlayer, 
			unsigned int vItemID, unsigned int vItemNumber, bool &vBind, uint64_t *pullGUID = NULL, int vType = BIND_ALL );

	// 玩家修改职业
	void PlayerChangeMetier( CEntityPlayer* pEntity, unsigned short vMetierID );

	// 玩家移动道具
	void MoveItem( CEntityPlayer* pEntity, unsigned short vSrcIndex, unsigned char vSrcSlot, unsigned short vDesIndex, unsigned char vDesSlot, unsigned short vNumber );

	// 玩家装备道具
	void EquipItem( CEntityPlayer* pEntity, unsigned int vSrcIndex, unsigned int vDesIndex = ~0 );

	// 玩家卸下道具
	void UnEquipItem( CEntityPlayer* pEntity, unsigned short vPart, unsigned int& vIndex, bool bDestroy=false );

	// 实体使用技能(最后字段：是否法宝)
	bool UseSkill( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, unsigned short vSkillID, unsigned short vSkillLevel, int vListID, unsigned int vIndex, CEntityCharacter** vEntityList, int vEntityListNum, bool bIsMagicWeapon = false, bool bUseByItem = false );

	int CloseAureole( CEntityCharacter* pSrcEntity );

	int UseAureoleSkill( CEntityCharacter* pSrcEntity, CTemplateAureoleSkill* pAureoleSkill, CEntityCharacter** vEntityList, int vEntityListNum );

	void AddResultToPB( PBFuncResult* vPBResult, CFuncResult* vResult );

	int ResultListSend( ); 

	// 玩家使用道具
	void UseItem( CEntityPlayer* pEntity, int vPos, unsigned short vIndex, unsigned int vParam );
	void UseTaskItem( CEntityPlayer* pEntity, unsigned short vIndex );

	// 得到属性百分比
	void GetPropertyPercent( CEntityCharacter* pEntity, int& vHPPercent, int& vMPPercent, int& vCurAP, int& vLevel );

	// NPC普通攻击
	void OnNpcNormalAttack( CEntityNpc* pSrcNpc, CEntityCharacter* pTarget );

	// NPC使用技能
	void OnNpcUseSkill( CEntityNpc* pSrcNpc, CEntityCharacter* pTarget, unsigned int vSkillID, unsigned int vSkillLevel );

	// 检查交易数据
	int CheckChange( CEntityPlayer* tpSrcEntity, CEntityPlayer* tpDesEntity, int tSrcNumber, int tDesNumber, int tSrcMoney, int tDesMoney );

	// 交易玩家数据
	int ExchangeBaggage( CEntityPlayer* tpSrcEntity, CEntityPlayer* tpDesEntity, int tSrcNumber, short* tpSrcItemList, int tDesNumber, short* tpDesItemList, int tSrcMoney, int tDesMoney, EExchangeFlag emExchange );

	// 获得玩家金钱
	unsigned int GetPlayerMoney( unsigned int vEntityID );

	// 操作玩家金钱
	int SetPlayerMoney( unsigned int vEntityID, int vMoney );

	// 得到道具字符串流
	void GetItemString( unsigned int tEntityID, int tBaggageIndex, int tItemNumber, char* tpItemBuffer, int& vLockType, int& vBufferSize );

	int GetItemBinary( unsigned int tEntityID, int tBaggageIndex, int tItemNumber, PBItemObject* tpItemObject, int& vLockType, bool bIsCheck );

	// 得到包裹格子数
	unsigned int GetBaggageRelease( CEntity* tpSrcEntity );

	// 当实体被陷阱击中时触发
	//void OnEntityTrap( CEntityCharacter* tpDesEntity, unsigned int tSrcEntityID, unsigned int tSkillID, unsigned int tSkillLevel, int tEntityNumber, CEntityCharacter** tpEntityList, bool& tIsOK );

	// 锁定包裹栏
	void LockBaggage( unsigned int vEntityID, int vBaggageIndex, int vLockType, bool& tIsOK );

	//// 邮件系统删除玩家道具
	//void MailEraseItem( CEntity* tpPlayer, int tItemNumber, unsigned short* tpItemIndex, int tMoney, int& tBackType );

	//// 邮件系统添加玩家道具
	//void MailAppendItem( CEntity* tpPlayer, char* tpItemBuffer, int tMoney, int& tBackType );

	// 重置第一刀
	void ResetFirstEntity( unsigned int vEntityID );

	// 按百分比回复
	void RecoverPer( CEntity* tpDesEntity, int tPer );

	// 更新区域相关任务
	
	// 得到玩家BUFF	信息
	CBuffList* GetBuffInfo( unsigned int vEntityID );

	void PlayerCloseDialog2( CEntityPlayer* pPlayer );

	void PlayerChangeMetier2( CEntityPlayer* pPlayer, unsigned int nDesMetier );

	int PlayerObtainExp2( CEntityPlayer* pEntity, int vExp, int vListID, int vReason,int* pMemberList, int vMemberCount );
	// 判断玩家是否处于生产、NPC对话状态

	bool PlayerInBusy( unsigned int vEntityID );

     // 查询道具
	int HasItem( unsigned int vEntityID, unsigned int vItemID, unsigned int vNumber, int& tBackType, int IfCountLockItem = 1, int vType = BIND_ALL );

	// 查询指定索引上是否有指定个数的道具
	bool HasItemOnIndex( unsigned int unEntityID, int nIndex, unsigned int unItemID, unsigned int unNumber, int nType = BIND_ALL );

	// 返回指定索引上的道具个数
	int GetItemNumOnIndex( unsigned int unEntityID, unsigned int unItemID, int nIndex, int nType = BIND_ALL );

	 // 消耗特定格子的道具
	void ConsumeItemByIndex( unsigned int vEntityId, unsigned int vItemId, int vItemNum, int vItemIndex, int& tBackType );

	// 清除伤害列表
	void RemoveDamage( unsigned int tSrcEntityID, unsigned int tDesEntityID );
	
	// 当玩家加入队伍的时候,把队伍中的陷阱给玩家显示
	void DisplayTripPlayerAddTeam( CEntityPlayer* pSrcEntity, CEntityPlayer** pTeamMemberList, int vTeamNum );

	// 当玩家离开队伍的时候,把队伍中的陷阱从玩家视野中消失
	void DisappearTripPlayerLeaveTeam( CEntityPlayer* pSrcEntity, CEntityPlayer** pTeamMemberList, int vTeamNum );

	// 当队伍解散的时候,把队伍成员中各自陷阱从其他队员的视野中消失
	void DisappearTripTeamDisband( CEntityPlayer** pTeamMemberList, int vTeamNum );

public:
	// 进行一次攻击或者被攻击后减少玩家装备的耐久度
	void EquipmentDurability( CEntity *pEntity, int  IsAttacker , int EntityType );
	
	// 装备是否损坏
    bool EquipmentAvailable( CEntity* pSrcEntity  ,int EptPart ) ;	

	// 死亡时减少玩家全身装备的耐久度
    void ChangeDurabilityOnDeath( CEntity* pEntity  ); 
    
	// 修理玩家装备
	void OnMessageMendEquipment( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 修理所有装备
	bool MendAllEquipment( CEntityPlayer *pPlayer);

	// 修理单个装备  
	void MendSigleEquipment( CEntityPlayer *pPlayer,int IsInPackage, int Index);
public:
	// 扣除邮件物品
	void RemoveMailItem( CEntityPlayer * tpPlayer, int PackageIndex, int ItemNum  );
public:	

	//// 获得仓库里面的道具信息并且在客户端输出
	//void GetStorageInfo( CEntityPlayer *pPlayer );

public:
	// 实体重生逻辑
	void EntityRelive( CEntity* pEntity );

	// 玩家复活逻辑
	void PlayerRelive( CEntityPlayer* pPlayer, ERelive vReliveType, int vHPPer, int vMPPer );

	// 玩家修改快捷栏
	void PlayerSetShortcut( CEntityPlayer* pPlayer, unsigned int vShortcut, unsigned int vType, unsigned int vIndex, unsigned int vLabelType, unsigned int vLabelIndex, int nParam1 );

	// 玩家获得金钱
	int PlayerChangeMoney( CEntityPlayer* pPlayer, int vMoney, bool vPayout, bool vIsBind );

	// 玩家获得元宝
	bool PlayerChangeYuanbao(CEntityPlayer* pPlayer, int nAmount, bool vIsBind);

	// 玩家丢弃金币
	//void PlayerThrowMoney( CEntityPlayer* pPlayer, int vMoney );	

	// 玩家丢弃道具
	void PlayerThrowItem( CEntityPlayer* pPlayer, int vItemIndex, int vNumber  );

	// 玩家销毁道具
	void PlayerDestroyItem( CEntityPlayer* pPlayer, int vItemIndex, int vNumber  );

	// 玩家丢弃仓库道具
	void PlayerThrowStorItem( CEntityPlayer* pPlayer, int vItemIndex, int vNumber , bool bForce=false );
	
	// 玩家出售道具
	void PlayerSellItem( CEntityPlayer* pPlayer, int vEntityID, int vItemIndex, int vNumber );

	// 玩家购买道具
	void PlayerPurchase( CEntityPlayer* pPlayer, int vEntityID, int vPageIndex, int vItemIndex, int vNumber, int nNpcID );

	// 玩家生产装备
	void PlayerProduceEquip( CEntityPlayer* pPlayer, unsigned short vType1, unsigned short vType2, unsigned short vType3, unsigned short vLevel, unsigned short vRank, int vType );
	
	// 玩家升级道具
	void PlayerUpItem( CEntityPlayer* pPlayer, int vIndex1, int vIndex2, int *vIndex3, int vType, int nNpcEntityID );

	// 玩家镶嵌符文
	void PlayerInsertRune( CEntityPlayer* pPlayer, int vIndex1, int vIndex2 );

	// 玩家镶嵌宝石
	void PlayerInsertJewel( CEntityPlayer* pPlayer, int vIndex0, int *vIndex );

	// 玩家打开装备插槽
	void PlayerOpenSlot( CEntityPlayer* pPlayer, int vIndex1, int vIndex2 );

	// 元气虫吃装备
	void PlayerEatEquip( CEntityPlayer* pPlayer, int vIndex0, int *vIndex );

	// 物品合成
	void PlayerComposeItem( CEntityPlayer* pPlayer, int vIndex );

	// 玩家点击商店服务
	int PlayerClickShop( CEntityPlayer* pPlayer, unsigned int vEntityID );

	// 玩家点击邮件服务
	int PlayerClickMail( CEntityPlayer* pPlayer, unsigned int vEntityID );

	// 玩家点击升级服务
	int PlayerClickUpg( CEntityPlayer* pPlayer,unsigned  int vEntityID );

	// 玩家点击传送服务
	int PlayerClickTele( CEntityPlayer* pPlayer, unsigned int vEntityID );

	// 玩家点击仓库服务
	int PlayerClickStorage( CEntityPlayer* pPlayer, unsigned int vEntityID );

	// 玩家点击回城服务
	int PlayerClickTown( CEntityPlayer* pPlayer, unsigned int vEntityID );
	
	// 玩家点装备绑定服务
	int PlayerClickBind( CEntityPlayer* pPlayer, unsigned int vEntityID );
	
	// 玩家点击每日福利服务
	int PlayerClickWelfare( CEntityPlayer* pPlayer, unsigned int vEntityID );

	// 玩家点击转换服务
	int PlayerClickChange( CEntityPlayer* pPlayer, unsigned int vEntityID );

	// 玩家点击鉴定服务
	int PlayerClickJudge( CEntityPlayer* pPlayer, unsigned int vEntityID );

	// 玩家点击兑换服务
	int PlayerClickExchange( CEntityPlayer* pPlayer, unsigned int vEntityID );

	// 玩家点击功勋服务
	int PlayerClickHonor( CEntityPlayer* pPlayer, unsigned int vEntityID );

	// 玩家关闭对话框
	void PlayerCloseDialog( CEntityPlayer* pPlayer );

	// 玩家点击功能NPC
	void PlayerClickNpc( CEntityPlayer* pPlayer, int vEntityID );

	void PlayerClickStoreBox( CEntityPlayer* vpPlayer, CEntityBox* vpBox );

	// 玩家拾取道具
	void PlayerPickItem( CEntityPlayer* pPlayer, int vEntityID );

	// 玩家关闭特定的界面
	void PlayerCloseWindow( CEntityPlayer* pPlayer, int vType );
public:
	// 绑定装备
	int  BindItem(  CEntityPlayer *tpPlayer, CItemObject *tpItemObj , int BindWay ,  int ItemIndex = -1 );
	
	// 解除绑定 
	int FreeItemBind( CItemObject *tpItemObj, int IfBindOnItem = 0 );

	// 获取物品绑定状态
	int GetItemObjBindStatus( CItemObject *tpItemIbj );

	// 发送物品绑定通知
	void SendBindNotice( CEntityPlayer * tpPlayer, int BindResult,  int Index , int PackKind , int BindStatus , int BindFreeTime = 0 );

	// 放置需要取消解除手动绑定的物品
	void PlayerAddEqtForResumeBind( CEntityPlayer *pPlayer, int EqtIndex );
	
	// 取下需要取消解除手动绑定的物品
	void PlayerTakeEqtForResumeBind( CEntityPlayer *pPlayer );
	
	// 恢复手动绑定
	void PlayerResumeBind( CEntityPlayer *pPlayer, int vItemIndex );	
	
	// 判断物品的解除绑定时间是否已到
	int CheckItemBindFreeTime( CItemObject *tpItemObject );		 
			
	// 添加绑定物品
	void PlayerInsertBindItem( CEntityPlayer *pPlayer, int ItemIndex );
	
	// 卸除绑定物品
	void PlayerTakeBindItem( CEntityPlayer *pPlayer, int ItemIndex );
	
	// 添加待解除绑定的物品
	void PlayerInsertItemForFree( CEntityPlayer *pPlayer, int ItemIndex );
	
	// 卸下待解除的物品
	void PlayerTakeItemForFree( CEntityPlayer *pPlayer, int ItemIndex );	 
	
	// 判断物品是否无绑定
	int IfItemFree( CItemObject *tpItemObj );
	
	// 判断物品是否普通绑定
	int IfItemBinded( CItemObject *tpItemObj );
	
	// 判断物品是否手动绑定
	int IfItemBindedByItem( CItemObject *tpItemObj ); 	
	
	// 判断物品是否处于手动解除状态
	int IfItemBindFree(  CItemObject *tpItemObj );
	
	// 当玩家登陆的时候把玩家身上处于绑定解除状态的物品插入列表中	
	void GetItemForFreeFromPlayer( CEntityPlayer *pPlayer  );
	
	// 把玩家处于装备绑定解除状态的物品放入列表中
	void InsertFreeItemIntoList( int ChaiId ,  CItemObject *tpItemObj );

	void PlayerAddSlot( CEntityPlayer *pPlayer, int vIndex1, int vIndex2 );

	void PlayerRemoveJewel( CEntityPlayer *pPlayer, int vIndex1, int vIndex2, int vIndex3 );

	void PlayerJudge( CEntityPlayer *pPlayer, int vIndex1, int vIndex2 );

	void PlayerChangeRes( CEntityPlayer *pPlayer, int vIndex1, int vIndex2 );

	void PlayerJewCom(CEntityPlayer *pPlayer, int *vJewIndex, int vCardIndex);

	void PlayerMultiCom( CEntityPlayer *pPlayer, int vComposeID, int vComposeType, int vEquipIndex = -1 );

	
	void OnMessageListActivityMenu(CEntityPlayer* tpPlayer, CMessage* tpMessage);

	void OnMessageListActivityItem(CEntityPlayer* tpPlayer, CMessage* tpMessage);

	void OnMessageMinusActivityItem(CEntityPlayer* tpPlayer, CMessage* tpMessage);
private:

	// 创建玩家
	bool CreatePlayer( CEntityPlayer* pEntity, bool bFirst = false );
	
public:

	// 召唤兽临时等级升级处理
	void SummonBeastLevelUp( CEntity* vpSrcEntity, unsigned int vTickOffset, bool bBroadCast = true );

	bool CreateMessageEntityRelifeNotice( CMessage* pTotalMessage, CEntity* pEntity, CMessageEntityRelifeNotice* pMessage );
	bool CreateMessageEntityDisappearNotice( CMessage* pTotalMessage, CEntity* pEntity, CMessageEntityDisappearNotice* pMessage );
	bool CreateMessagePlayerReliveNotice( 
		CMessage* pTotalMessage, 
		CMessagePlayerReliveNotice* pMessage, 
		unsigned int vEntityID , 
		unsigned short vPosX, 
		unsigned short vPosY, 
		unsigned int vCurHP, 
		unsigned int vCurMP,
		unsigned int vMaxHP,
		unsigned int vMaxMP
		);

	bool CreateMessageChangeMetierNotice(
		CMessage* pTotalMessage, 
		CMessageChangeMetierNotice* pMessage,
		unsigned int vEntityID,
		unsigned char vMetierID
		);


	bool CreateMessageInsertItemNotice(
		CMessage* pTotalMessage, 
		CMessageInsertItemNotify* pMessage,
		int	vPos,
		int vItemID,
		int *vIndex,
		int *vNumber,
		int vLen,
		int vBind = 0
		);

	bool CreateMessageInsertEquipNotice(
		CMessage* pTotalMessage, 
		CMessageInsertEquipNotify* pMessage,
		CItemObject* pItemObj,
		int	vPos, int nIndex
		);

	bool CreateMessagePlayerChangeEquipNotice(
		CMessage* pTotalMessage, 
		CMessagePlayerChangeEquipNotice* pMessage,
		unsigned int vEntityID,
		unsigned short vPart,
		unsigned int vItemID,
		unsigned int vCurHP,
		unsigned int vCurMP,
		unsigned int vMaxHP,
		unsigned int vMaxMP,
		unsigned int vWeaponLevel,
		unsigned int vChestLevel,
		unsigned int vWeaponJewel,
		unsigned int vChestJewel,
		unsigned int vMWID,
		unsigned int vFashionID,
		unsigned int vMarriageEquipID,
		unsigned int vMWQLID
		);

	bool CreateMessageEntityRefreshNotice(
		CMessage* pTotalMessage, 
		CMessageEntityRefreshNotice* pMessage,
		unsigned int vEntityID,
		unsigned int vCurHP,
		unsigned int vCurMP,
		unsigned int vMaxHP,
		unsigned int vMaxMP
		);


	bool CreateMessageNotifyDirectionCallBack(
		CMessage* pTotalMessage, 
		CMessageNotifyDirectionCallBack* pMessage,
		unsigned int vEntityID,
		int vDirection
		);

	bool CreateMessageNotifyPetLevelUp(
		CMessage* pTotalMessage, 
		CMessagePetLevelUpNotice* pMessage,
		unsigned int vEntityID,
		int vLevel
		);

 
	/************************************************************************/
	/*      处理消息的函数                                                  */
	/************************************************************************/
	// 处理玩家扔仓库物品请求消息
	void OnMessageThrowStorItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家使用技能请求消息
	void OnMessageUseSkillRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家普通攻击请求消息
	//void OnMessageNormalAttackRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家拾取道具请求消息
	void OnMessagePickItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家点击npc请求消息
	void OnMseeageClickNpcRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家购买物品请求消息
	void OnMessagePurchaseItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家卖物品请求消息
	void OnMessageSellItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家扔物品请求消息
	void OnMessageThrowItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家扔金钱请求消息
	//void OnMessageThrowMoneyRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家设置快捷栏请求消息
	void OnMessageSetShortcutRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家复活请求消息
	void OnMessagePlayerReliveRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家移动道具请求消息
	void OnMessageMoveItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理调试指令消息 【危险:非debug版本将屏蔽此功能】
	void OnMessageProDebugNotice( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家装备道具请求消息
	void OnMessageEquipItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家卸载装备请求消息
	void OnMessageUnEquipItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家点击技能点请求消息
	void OnMessageClickSkillPointRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家重置一点属性请求消息
	void OnMessageResetSPPRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家重置一点属性请求消息
	void OnMessageResetSSPRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家使用道具请求消息
	void OnMessageUseItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家生产装备请求消息
	void OnMessageProduceEquipRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家添加升级物品请求消息
	void OnMessageUpgAddItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家取下升级物品请求消息
	void OnMessageUpgDelItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家升级装备请求消息
	void OnMessageUpgEquipRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家生产药品请求消息
	void OnMessageProducePhysRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家放入符文请求消息
	void OnMessagePushRuneRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	//处理 玩家 发送转换符文消息
	void OnMessageChangeRuneRequest ( CEntityPlayer * pPlayer,CMessage *pMessage);
	//处理 玩家 发送升级符消息
	void OnMessageRuneComRequest( CEntityPlayer* pPlayer,CMessage* pMessage);

	// 处理玩家去除符文请求消息
	void OnMessagePopRuneRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家插入符文请求消息
	void OnMessageInsertRuneRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家插入镶嵌宝石请求消息
	void OnMessageInsertPushJewelRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家取出镶嵌宝石请求消息
	void OnMessageInsertPopJewelRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家镶嵌宝石请求消息
	void OnMessageInsertJewelRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家放入被吃装备请求消息
	void OnMessageEatPushEquipRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家取出被吃装备请求消息
	void OnMessageEatPopEquipRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家吃装备请求消息
	void OnMessageEatEquipRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家放入合成装备请求消息
	void OnMessageComposePushRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家取出合成装备请求消息
	void OnMessageComposePopRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家放入开槽物品请求消息
	void OnMessageOpenPushRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家取出开槽物品请求消息
	void OnMessageOpenPopRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家传送请求消息
	void OnMessagePlayerTeleRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理调试指令消息 【危险:非debug版本将屏蔽此功能】 
	void OnMessageProDebugCommand( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家通知朝向消息
	void OnMessageNotifyDirection( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家改变宠物模式请求消息
	void OnMessageChangePetModeRequest( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家获得回购物品流消息
	void OnMessageObtainSaleItemString( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家回购物品请求消息
	void OnMessageBuyBackItem( CEntityPlayer* pPlayer, CMessage* pMessage  );

	// 处理玩家移动仓库物品请求消息
 /*   void OnMessageMoveStorageItem( CEntityPlayer* pPlayer, CMessage* pMessage );*/

	// 处理玩家绑定物品请求
	void OnMessageBindItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage );	

	// 处理玩家添加绑定物品请求
	void OnMessageAddItemRequest( CEntityPlayer * pPlayer, CMessage* pMessage );

	// 处理玩家卸下绑定物品请求
	void OnMessageTakeItemRequest( CEntityPlayer *pPlayer, CMessage* PMessage );     

	// 处理玩家添加待解除物品
	void OnMessageAddItemForFreeRequest(  CEntityPlayer *pPlayer, CMessage* PMessage  );    

	// 处理玩家卸下待解除的物品 
	void OnMessageTakeItemForFreeRequet(  CEntityPlayer *pPlayer, CMessage* PMessage  ); 

	//  处理玩家解除手动绑定的请求
	void OnMessageFreeItem(  CEntityPlayer *pPlayer, CMessage* PMessage  ) ;

	// 处理玩家学习技能请求消息
	void OnMessageStudySkillRequest( CEntityPlayer* pPlayer, CMessage* pMessage );		

	// 处理玩家加属性点
	void OnMessageAddPropertyPointRequest( CEntityPlayer* pPlayer, CMessage* pMessage );	

	// 处理玩家设置回城点请求消息
	void OnMessageSetTownRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家改变pk模式请求消息
	void OnMessageChangePKModeRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家查看属性信息请求
	void OnMessageLookPropertyRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家跨服查看其他玩家属性信息请求
	void OnMessageLookPropertyRouterRequest( CMessage* pMessage );

	// 处理网关回应的查看其他玩家属性信息
	void OnMessageLookPropertyRouterResponse( CMessage* pMessage );

	// 处理玩家收集任务物品请求消息
	void OnMessageTaskCollectRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家改变帮助装备请求消息
	void OnMessageChangeHelpStatus( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家领双请求
	void OnMessageObtainDiploidRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家冻结双倍时间(解冻)
	void OnMessageFreezeDiploidRequest( CEntityPlayer* pPlayer, CMessage* pMessage );	

	// 处理玩家查询剩余的领双时间
	void OnMessageQueryDiploidTimeRequest( CEntityPlayer* pPlayer, CMessage* pMessage );	
	
	// 处理玩家使用持续作用技能请求
	void OnMessageUseContinuedSkillRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	//// 处理玩家点击绑定服务请求消息
	//void OnMessageClickBindServiceRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家添加手动绑定装备请求消息
	void OnMessageAddEqtForResumeBind( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家取下手动绑定装备请求消息
	void OnMessageTakeEqtForResumeBind( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家手动绑定请求消息
	void OnMessageResumeBind( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	//// 处理玩家点击"内丹"请求消息
	void OnMessageClickRedStoneRequest( CEntityPlayer* pPlayer, CMessage* pMessage ); 	

	// 客户端通知内丹读秒结束
	void OnMessageRedStoneTimerEnd( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理客户端打开服务器请求消息
	void OnMessageOpenServiceRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理客户端手动删除BUFF的请求
	void OnMessageActiveRemoveBuffRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理客户端设置密码的请求
	void OnMessageSetPassWordRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理客户端检查密码的请求
	void OnMessageCheckPasswordRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
		
	// 处理客户端修改密码的请求
	void OnMessageModifyPasswordRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理客户端设置2级密码选项的通知
	//void OnMessageSetPasswordOption( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理客户端点击传送符请求
	void OnMessageClickTeleSymbolRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理客户端存储金钱的请求
	void OnMessageStorMoneyRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理客户端取出金钱的请求
	void OnMessageTakeStorMoneyRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理消息放置背包的请求
	void OnMessageAddKitBagRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理打开背包的消息请求
	void OnMessageOpenKitBag( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 拿下背包的消息请求
	void OnMessageTakeKitBag( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理玩家增加凹槽放入请求
	void OnMessagePushAddSlotRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家增加凹槽拿出请求
	void OnMessagePopAddSlotRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家增加凹槽请求
	void OnMessageAddSlotRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家摘除宝石放入请求
	void OnMessagePushRemoveJewelRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家摘除宝石拿出请求
	void OnMessagePopRemoveJewelRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家摘除宝石请求
	void OnMessageRemoveJewelRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家鉴定装备放入请求
	void OnMessagePushJudgeRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家鉴定装备拿出请求
	void OnMessagePopJudgeRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家鉴定装备请求
	void OnMessageJudgeRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家转化抗性放入请求
	void OnMessagePushChangeRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家转化抗性拿出请求
	void OnMessagePopChangeRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家转化抗性请求
	void OnMessageChangeRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家合成宝石放入请求
	void OnMessagePushJewComRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家合成宝石拿出请求
	void OnMessagePopJewComRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家合成宝石请求
	void OnMessageJewComRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家复杂合成放入请求
	void OnMessagePushMultiComRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家复杂合成拿出请求
	void OnMessagePopMultiComRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家复杂合成请求
	void OnMessageMultiComRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家放入卡片请求
	void OnMessagePushCardRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家点击造化点请求
	void OnMessageClickTalentPointRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家造化进阶请求
	void OnMessageIncreaseTalentRequest( CEntityPlayer* pPlayer, CMessage* pMessage ); 	
	// 处理玩家生活技能入门的请求
	void OnMessageInitialLifeSkillRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家进阶生活技能的请求
	void OnMessageLevelUpLifeSkillRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家学习新技能配方的请求
	void OnMessageStudyComposeRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家给装备附魔的请求
	void OnMessageSetMagicStoneRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家分解装备的请求
	void OnMessageDecompoundEquipRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 放置需要研究的材料
	void OnMessageAddMaterialRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 取下研究材料
	void OnMessageTakeMaterialRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 遗忘生活技能
	void OnMessageForgetLifeSkill( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 处理玩家选择称号请求
	void OnMessageSelectTitleRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 自动挂机请求
	void OnMessageAutoPlayerGameRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 挂机买药请求
	void OnMessageBuyPhysicRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 才华加点请求
	void OnMessageAddWizardPointRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 算命请求
	void OnMessageGetTrendRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 远程修理装备 [3/5/2010 GaoHong]
	void OnMessageRemoteMendAllEquip( CEntityPlayer* npPlayer, CMessage* npMessage );
	
	// 放置待分解装备的请求消息
	void OnMessageAddDecompoundEquipRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 拿下待分解装备的请求消息
	void OnMessageTakeDecompoundEquipRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 重置装备属性请求
	void OnMessageChangeEquipProRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 点击开启灵石请求
	void OnMessageOpenStoneRequest( CEntityPlayer* pPlayer, CMessage* pMessage );	
	// 装备拆星请求
	void OnMessageGetStarRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 装备加星请求
	void OnMessageAddStarRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// nBeginTime: 活动开始时间，nEndTime: 活动结束时间, nRate: 倍率 0 即关闭
	void FireActivityDiploid( CEntityPlayer* pPlayer, int nRate, int nBeginTime = 0, int nEndTime = 0 );

	// nBeginTime: 活动开始时间，nEndTime: 活动结束时间, nRate: 倍率
	void FireActivityDiploid( CEntityPlayer* pPlayer, int nRate, bool bAllInScene = false, int nBeginTime = 0, int nEndTime = 0 );

	// 发送增加凹槽放入回应
	void SendPushAddSlotResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	// 发送增加凹槽拿出回应
	void SendPopAddSlotResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	// 发送增加凹槽回应
	void SendAddSlotResponse( CEntityPlayer* pPlayer, int vResult, int vSlot );
	// 发送摘除宝石放入回应
	void SendPushRemoveJewelResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	// 发送摘除宝石拿出回应
	void SendPopRemoveJewelResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	// 发送摘除宝石回应
	void SendRemoveJewelResponse( CEntityPlayer* pPlayer, int vResult );
	// 发送鉴定装备放入回应
	void SendPushJudgeResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	// 发送鉴定装备拿出回应
	void SendPopJudgeResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	// 发送鉴定装备回应
	void SendJudgeResponse( CEntityPlayer* pPlayer, int vResult, int vProperty1, int vProperty2, int vProperty3 );
	// 发送转化抗性放入回应
	void SendPushChangeResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	// 发送转化抗性拿出回应
	void SendPopChangeResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	// 发送转化抗性回应
	void SendChangeResponse( CEntityPlayer* pPlayer, int vResult, int vType );
	// 发送合成宝石放入回应
	void SendPushJewComResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	// 发送合成宝石拿出回应
	void SendPopJewComResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	// 发送合成宝石回应
	void SendJewComResponse( CEntityPlayer* pPlayer, int vResult );
	// 发送复杂合成放入回应
	void SendPushMultiComResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	// 发送复杂合成拿出回应
	void SendPopMultiComResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	// 发送复杂合成回应
	void SendMultiComResponse( CEntityPlayer* pPlayer, int vComposeID, int vResult, int nIndex = -1 );
	// 发送放入卡片回应
	void SendPushCardResponse( CEntityPlayer* pPlayer, int vResult, int vItemID );
	// 发送获得修为值通知
	void SendTalentChangeNotify( CEntityPlayer* pPlayer, int vTalentExp, int vTalentPoint, int vTalentChange, int vTalentLevel );
	// 发送点击造化点回应
	void SendClickTalentPointResponse( CEntityPlayer* pPlayer, int vResult, int vRow, int vCol, int vTalentExp, int vTalentPoint, int vTalentStep );
	// 发送造化进阶回应
	void SendIncreaseTalentResponse( CEntityPlayer* pPlayer, int vResult, int vTalentExp, int vTalentPoint, int vTalentStep );	
	// 发送初始化生活技能的回应
	void SendResponseInitializeLifeSkill( CEntityPlayer *pPlayer, int nErrcode, int nLifeSkillType, int nLifeSkillKind, int nDegreeValue, int nSkillLevel );			
	// 发送生活技能进阶的回应消息
	void SendResponseLevelUPLifeSkill( CEntityPlayer *pPlayer, int nErrcode, int nLifeSkillType, int nSpecializeDegree, int nLifeSkillLevel );	
	// 发送学习新技能配方的回应消息
	void SendResponseStudyCompose( CEntityPlayer *pPlayer, int nErrcode, int nSpecializeDegree, int nComposeID, int nLifeSkillType, int nStudyWay );	
	// 发送附魔的回应消息
	void SendResponseSetMagicStone( CEntityPlayer *pPlayer, int nErrcode, int nEquipIndex, int nAttribute, int nStoneID,  int nMagicNum = 0, int nSlotType = SLOT_BAGGAGE );
	// 发送装备分解的回应消息
	void SendResponseDecompoundEquip( CEntityPlayer *pPlayer, int nErrcode, int nEquipIndex );
	// 发送放置材料的消息回应
	void SendResponseAddMaterial( CEntityPlayer *pPlayer, int nErrcode, int nBagIndex, int nPosIndex );
	// 取下研究材料
	void SendResponseTakeMaterial( CEntityPlayer* pPlayer, int nErrcode, int nPosIndex );
	// 发送生活技能属性改变的通知
	void SendNotifyLifeSkillAttribute( CEntityPlayer* pPlayer, int nLifeSkillType, int nSpecializeDegree );
	// 发送遗忘生活技能的消息回应
	void SendResponseForgetLifeSkill( CEntityPlayer *pPlayer, int nErrcode, int nLifeSkillType );
	
	// 发送放置待分解装备的回应消息
	void SendResponseAddDecompoundEquipRequest( CEntityPlayer *pPlayer, int nErrcode, int nEquipIndex );
	
	// 发送取下待分解装备的回应消息
	void SendResponseTakerDecompoundEquipResponse( CEntityPlayer *pPlayer, int nErrcode, int nEquipIndex );
	// 发送算命结果
	void SendGetTrendResponse( CEntityPlayer *pPlayer, int vResult, int vTrend );

	// 处理打断技能请求
	void OnMessageInterruptSkill( CEntityPlayer* pPlayer, CMessage* pMessage );

	// time out message
	void OnTimeoutMessageSkillCoolDown( CMessage* pMsg );
	void OnTimeoutMessageEntityDisappear( CMessage* pMsg );
	void OnTimeoutMessageEntityClear( CMessage* pMsg );
	void OnTimeoutMessageEntityRelive( CMessage* pMsg );
	void OnTimeoutMessageEntityProtectTimeOver( CMessage* pMsg );
	void OnTimeoutMessageEntityAliveRefresh( CMessage* pMsg );
	void OnTimeoutMessageEndRedstoneProtect(CMessage* pMsg);
	void OnTimeOutMessageRedStoneClear( CMessage* pMsg );
	void OnTimeOutMessageFireRedStoneMagic( CMessage* pMsg );	
	void OnTimeoutMessageDiploidTime( CMessage* pMsg );
	void OnTimeOutMessageRedstoneProcess( CMessage* pMsg );
	void OnTimeOutMessageAssignment( CMessage* pMsg );
	void OnTimeOutMessageYellowStatus( CMessage* pMsg );
	void OnTimeOutMessageRefreshTen( CMessage* pMsg );
	void OnTimeOutMessageRefreshFive( CMessage* pMsg );
	void OnTimerItemChange( CMessage* pMsg );
	void OnTimeOutMessageCreateOgreTen( CMessage* pMsg );
	void OnTimeOutMessageFireBattle( CMessage* pMsg ); 
	void OnTimeOutMessageStartBattle( CMessage* pMsg );
	void OnTimeOutMessageTimeLimitTask(CMessage *pMsg);
	void OnTimeOutMessageCDGroup(CMessage* pMsg);
	void OnTimeoutMessageBuffEvent( CMessage* pMsg );
	void OnTimeOutMessageNonProtect( CMessage* pMsg );

	// 发送消息的函数
	void SendItemBindedNotice( CEntityPlayer *tpPlayer, int ItemIndex, int ItemPos );
	void SendYourProDataNotice( CEntityPlayer* vpPlayer );
	void SendClickNpcResponse( CEntity* pPlayer, unsigned int vNpcEntityID );
	void SendCloseDialogNotice( CEntity* pPlayer );
	void SendUpdateMoneyNotice( CEntity* pPlayer, int vMoney, int vTotal );
	void SendUpdateBindMoneyNotice( CEntity* pPlayer, int vMoney, int vTotal );
	void SendRemoveItemNotice( CEntity* pPlayer, int vPos, unsigned short vItemIndex, unsigned short vNumber );
	void SendLockItemNotice( unsigned int vEntityID, int vIndex, int vLockType );
	void SendLockItemNotice( CEntity* pPlayer, int vIndex, int vLockType );
	void SendObtainExpNotice( CEntity* pPlayer, int vExpDelta, unsigned long long vExp, int vListID, bool IsLevelUp,  int vOffLineExp, int vLeftTime );
	void SendObtainHonorNotice( CEntity* pPlayer, int vChangeHonor );
	void SendMoveItemNotice( CEntity* pPlayer,unsigned short vSrcIndex, unsigned short vDesIndex, unsigned short vNumber, unsigned char vSrcSlot, unsigned char vDesSlot, int nErrcode, int nSrcNum, int nDesNum );
	void SendProErrorNotice( CEntity* pPlayer, int vType );
	void SendEquipItemNotice( CEntity* pPlayer, unsigned int vSrcIndex,  int nErrcode, unsigned int vDesIndex = ~0 );
	void SendUnEquipItemNotice( CEntity* pPlayer, unsigned short vPart, unsigned int vIndex, int nErrcode );
	void SendClickSkillPointResponse( CEntity* pPlayer, unsigned short vGenerID, unsigned char vCol, unsigned char vRow );
	void SendResetAPPResponse( CEntity* pPlayer, int nRet );
	void SendResetSSPResponse( CEntity* pPlayer, unsigned short vGenerID, unsigned char vCol, unsigned char vRow );
	void SendResetASPResponse( CEntity* pPlayer, int nRet );
	void SendResetTalentResponse( CEntity* pPlayer, int nRet, int nStep, int nPoint );
	void SendTelePortResponse( CEntity* pPlayer, unsigned short vIndex, int mPosX, int mPosY );
	void SendUpgAddItemResponse( CEntity* pPlayer, unsigned short vIndex1, unsigned short vIndex2 );
	void SendUpgDelItemResponse( CEntity* pPlayer, unsigned short vIndex1, unsigned short vIndex2 );
	void SendUpgEquipNotice( CEntity* pPlayer, unsigned short vResult, int vType, int nIndex, unsigned short vLevel = 0 );
	void SendProFinNotice( CEntity* pPlayer, int vResult, int vLeftNum  = 0 );
	void SendProBreakNotice( CEntity* pPlayer, int vResult );
	void SendPushRuneResponse( CEntity* pPlayer, int vIndex1, int vIndex2 );
	void SendPopRuneResponse( CEntity* pPlayer, int vIndex1, int vIndex2 );
	void SendInsertRuneNotice( CEntity* pPlayer, int vResult );
	void SendInsertPushJewelResponse( CEntity* pPlayer, int vIndex1, int vIndex2 );
	void SendInsertPopJewelResponse( CEntity* pPlayer, int vIndex1, int vIndex2 );
	void SendInsertJewelNotice( CEntity* pPlayer, int vResult, int vBind );
	void SendEatPushEquipResponse( CEntity* pPlayer, int vIndex1, int vIndex2 );
	void SendEatPopEquipResponse( CEntity* pPlayer, int vIndex1, int vIndex2 );
	void SendEatEquipNotice( CEntity* pPlayer, int vIndex, int vNumber );
	void SendComposePushResponse( CEntity* pPlayer, int vIndex1, int vIndex2 );
	void SendComposePopResponse( CEntity* pPlayer, int vIndex );
	void SendComoseNotice( CEntity* pPlayer, int vIndex, int vItemID, int vNumber1, int vNumber2 );
	void SendOpenPushResponse( CEntity* pPlayer, int vIndex1, int vIndex2 );
	void SendOpenPopResponse( CEntity* pPlayer, int vIndex1, int vIndex2 );
	void SendOpenSlotNotice( CEntity* pPlayer, int vIndex1, int vIndex2, int vNumber );
	void SendSyncPosNotice( CEntity* pPlayer, unsigned int vEntityID, int vPosX, int vPosY );
	void SendProExpNotice( CEntity* pPlayer, unsigned int vProLevel, unsigned int vProExp );
	void SendChangeSpeedToSeenEntity( CEntity* pEntity );
	void SendRideHorseNotice( CEntity* pEntity, unsigned int vTempID, int vResult );
	void SendStudySkillResponse( CEntityPlayer* pPlayer, int* tGenerIDList, int* tColList, int* tRowList, int tCount, unsigned int* tSkillIDList, int* tIndexList );
	void SendChangePKModeResponse( CEntityPlayer* pPlayer, int vRet );
	void SendLookPropertyResponse( CEntityPlayer* pPlayer, CEntityPlayer* pLooker, int vType );
	void SendLookPropertyRouterRequest( CEntityPlayer* pPlayer, int vCharID, int vType );
	void SendLookPropertyRouterResponse( CEntityPlayer* pPlayer, int vCharID, int vType );
	void SendLookPropertyResponse( CEntityPlayer* pPlayer, CMessageLookPropertyResponse* pLookMessage, int vType );
	void SendTripEntityDisplay( CEntityPlayer* pPlayer, CEntityPedal* pEntityPedal );
	void SendTripEntityDisplay( std::vector<CEntityPlayer*>* pTeamVec, CEntityPedal* pEntityPedal );
	void SendTripEntityDisappear( CEntityPlayer* pPlayer, CEntityPedal* pEntityPedal );
	void SendTripEntityDisappear( std::vector<CEntityPlayer*>* pTeamVector, CEntityPedal* pEntityPedal );
	void SendClickRedStone( CEntityPlayer *pPlayer, int EntityID, int Result );
	void SendStopRedStoneTimer( CEntityPlayer *pPlayer, int EntityId );
	void SendChangeStatus( CEntityPlayer *pPlayer, int Status );
	void SendMsgRedstoneStarted( CEntityPlayer *pPlayer, int TeamID );
	void SendUseItemResponse( CEntityPlayer *pPlayer, int vResult, int vPos, int vIndex, int vItemID, int vParam1 = 0, int vParam2 = 0, int vParam3 = 0 );
	void SendMsgBeginRedstoneMagic( CEntityRedStone *tpRedstone );
	void SendSetPasswordResponse( CEntityPlayer *pPlayer, int Errcode, int nStatus );
	void SendModifyPasswordResponse( CEntityPlayer *pPlayer, int Errcode, int nStatus );
	void SendCheckPasswordResponse( CEntityPlayer *pPlayer, int Errcode, int nStatus );
	void SendEventNotify( int vEventID, int vCountryID, int *vIntParam, int vSize, char *vStringParam1, char *vStringParam2, int vWorldID = 0 );
	void SendActiveCardNotice( CEntity* pPlayer, int vType, int vPos, int vItemID );
	
	void SendStorMoneyResponse( CEntityPlayer* pPlayer, int nErrcode, int nMoney, int nBindMoney, int nLmtMoney=0 );
	void SendTakeStorMoneyResponse( CEntityPlayer* pPlayer, int nErrcode, int nMoney, int nBindMoney, int nLmtMoney=0 );		
	
	void SendAddKitBagResponse( CEntityPlayer* pPlayer, int nErrcode, int nBagIndex, int nDesIndex, int nDestSlot, int nExpiredTime, int nBindStatus );		
	
	void SendAddDecompoundEquipResponse( CEntityPlayer* pPlayer, int nErrcode, int nEquipIndex );
	void SendTakeDecompoundEquipResponse( CEntityPlayer* pPlayer, int nErrcode, int nEquipIndex );
	void SendClientCtrlInfo(CEntityPlayer *pPlayer);
	void SetItemValidTime( CEntityPlayer* pPlayer, CTplItem* pTplItem, CItemObject* pItemObj);
	void StartItemValidTimer( CEntityPlayer* pPlayer, CItemObject* pItemObj);
	void OnTimeoutMessageDestroyItem(CMessage* pMsg);
	void CheckPlayerItems( CEntityPlayer* pPlayer );
	void SendPickItemResponse( CEntityPlayer* pPlayer, int vEntityID );
	void SendBindItemNotify( CEntityPlayer* pPlayer, int vIndex, int vStatus );

	void OnMessagePlayerUseFlowerRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	void PlayerUseFlower( int nSrcCharID, const char* pSrcName, CEntityPlayer* pDesPlayer, int nItemID, int nBagIndex );
	void OnMessageS2SUseFlowerRequest( CMessage* pMessage );
	void OnMessageS2SUseFlowerResponse( CMessage* pMessage );
	void OnUseFlowerSuccess( CEntityPlayer* pSrcPlayer , int nBagIndex);
	void SendChangeEquipProResponse( CEntityPlayer* pPlayer, int vResult, int vIndex1, int vIndex2 );
	void NotifyClientStartTimer(CEntityPlayer* pPlayer, EPosType vType, int nIndex, unsigned int tTime );
	void OnTimeOutMessageTitleDisappear(CMessage* pMsg);

public:
	/* 内丹相关函数 */
	
	// 结束内丹的保护时间
	void EndRedStoneProtect( CTimerItem *tpTimer );
	
	// 内丹的消息时间到
	void ClearRedStone( CTimerItem *tpTimer );
	
	// 内丹的噬炼时间到
	void RedStoneMagicTime( CTimerItem *tpTimer );
	
	// 判断对象是否是内丹
	bool IfRedStone( int ItemID, bool &bIfStart );

	// 判断对象是否是适配器
	bool IsItemAdapter( int nTempID );
	// 判断是不是某类型的物品
	bool CheckItemType( int nTempID, int vType );

	// 玩家点击内丹
	void PlayerClickRedStone( CEntityPlayer *pPlayer, int RedStoneEnittyID );	
	
	// 判断队伍成员是否已经成功开启过内丹
	bool CheckTeamRedstoneEffect( int nTeamID );
	
	// 设置队伍成员已经开启过内丹
	void SetRedstoneEffect( int nTeamID, int nRedstoneID );
	
	// 客户端停止内丹的读秒
	void StopRedStonTimer( CEntityPlayer *pPlayer, int RedStoneEnittyID );
	
	// 开始噬炼
	void BeginRedStoneMagic( CEntityRedStone *tpRedStone, CEntityPlayer *pPlayer );
	
	// 噬炼获取经验
	void RedStoneAllotExp( CEntity *tpRedStone, int nOwenerID, int nRedStoneTmpID,  int LeftTime, int &rLastNum );
	
	// 创建家族内丹
	void CreatFamilyRedStone( CEntityPlayer *pPlayer, int nRedStoneTmpID );
	
	// 创建家族副本内丹
	void CreatFamilyRepetionRedstone( int nRedStoneTmpID, int nFamilyID, int nLineID, int nMapID, int nPosX, int nPosY, int nMapIndex, KEY_TYPE nRepetionKey );
	
	// 创建国家内丹
	void CreateCountryRedstone( int nLineID, int nMapID, int nMapIndex, int nPosX, int nPosY, int nCountryID, int nRedstoneTmpID );
			
	// 家族内丹获取经验
	void AllotFamilyExp( CEntityRedStone *tpRedStone, int nLeftTime, CTplRedStone *tpTplRedStone );
	
	// 改变离队队员的内丹状态
	void ChangeTeamLeaverStatus( CEntityPlayer *pPlayer );
	
	// 当玩家被清除内丹的状态
	void ClearRedstoneStatus( CEntityRedStone* tpRestone ); 
	
	// 发送国家内丹开启的消息通知
	void SendNotifyCountryRedstoneStarted( CEntityPlayer *pPlayer, int nRedstoneType );

	// PK数值处理
	void ProcessPKValueOnDie( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity );
	void ProcessPKValueOnAttack( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity );
	void SendPKValueMessage( CEntityPlayer* pPlayer, int nValue );
	void SendYellowStateNotice( CEntityPlayer* pPlayer, bool bOpen );
	void DecExpOnPkDie( CEntityPlayer* pPlayer );
	
public:
	bool CheckSpecialUseCondition( CEntity* pSrcEntity, CTemplateSkill* tpSkill );

	void EntityTeleportBuffer( CEntityCharacter* pSrcEntity, int nTeleType, int nDistance );

	//bool EntityCanMove( CEntity* pEntity );

	void EntityCharge( CEntityCharacter* pSrcEntity, CEntity* pDesEntity );

	void EntityInsertDamageList( CEntity* pSrcEntity, CEntity* pDesEntity, int vDamage = 0 );

	// 是否会黄名
	void CheckPKIsYellow( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity );

	// 是否会红名
	void CheckPKIsRed( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity );

	// NPC死亡掉落物品
	void DropItemByNpcDie( CEntity* pOwnerEntity, CEntity* pDesEntity, CTemplateOgre* pTempOgre );

	// 根据掉落表掉落物品
	void DropItemByID( CEntity* pOwnerEntity, CEntity* pDesEntity, int vDropID, const CDropFrom& vDropFrom, int *vNumber = NULL, int vDropType = 0, bool vNeedSend = false );

	int PlayerClickStudySkill( CEntityPlayer* pPlayer, unsigned int vEntityID );

	void StudySkillByChangeMetier( CPropertyPlayer* pProperty, int vMetierID, int* tGenerIDList, int* tColList, int* tRowList, int& tCount, int vMaxNum, unsigned int* tSkillIDList, int* tIndexList );

	void ActionSelfBless( CEntityCharacter* pSrcEntity, CTemplateNormalSkill* pNormalSkill, int vDamage, int vSkillType, bool& vbIfSrcDurReduced, bool& vbEntityType );
	void ActionSeenBless( CEntityCharacter* pSrcEntity, CTemplateNormalSkill* pNormalSkill, int vDamage, int vSkillType, bool& vbIfSrcDurReduced, bool& vbEntityType );

	void ActionAttackSingle( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateNormalSkill* pNormalSkill, int vSkillType, bool& vbIfSrcDurReduced, bool& vbEntityType );

	void ActionAttackRange( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateNormalSkill* pNormalSkill, int vSkillType, bool& vbIfSrcDurReduced, bool& vbEntityType, CEntityCharacter** vEntityList, int vEntityListNum );

	void ActionCussOrBlessSingle( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateNormalSkill* pNormalSkill, int vDamage, int vSkillType );

	void ActionCussOrBlessRange( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateNormalSkill* pNormalSkill, int vDamage, int vSkillType, bool& vbEntityType, CEntityCharacter** vEntityList, int vEntityListNum );

	void ActionTrip( CEntityCharacter* pSrcEntity, CTemplateSkill* pSkill, bool& vbIfSrcDurReduced );

	void ActionTeamBless( CEntityCharacter* pSrcEntity, CTemplateNormalSkill* pNormalSkill, int vDamage, int vSkillType, bool& vbIfSrcDurReduced, bool& vbEntityType );

	void ActionPetBless( CEntityCharacter* pSrcEntity, CTemplateNormalSkill* pNormalSkill, int vDamage, int vSkillType, bool& vbIfSrcDurReduced );

	void ActionRelive( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateNormalSkill* pNormalSkill, int vDamage, int vSkillType, bool& vbIfSrcDurReduced );

	void ActionTeleport( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateNormalSkill* pNormalSkill, int vDamage, int vSkillType, bool& vbIfSrcDurReduced );

	void ActionSummon( CEntityCharacter* pSrcEntity, CTemplateNormalSkill* pNormalSkill, bool& vbIfSrcDurReduced );

	void ActionAureole( CEntityCharacter* pSrcEntity, CTemplateSkill* pSkill, bool& vbIfSrcDurReduced, CEntityCharacter** vEntityList, int vEntityListNum );

	void ActionContinuedAttackRange( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateNormalSkill* pNormalSkill, CEntityCharacter** vEntityList, int vEntityListNum );
	
	void ActionContinuedCussOrBlessRange( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateNormalSkill* pNormalSkill, CEntityCharacter** vEntityList, int vEntityListNum );

	// 给玩家或者NPC上BUFF
	bool InsertBuff( CEntityCharacter* pDesEntity, int nBuffID, int nBuffTime = 0 ,int *nResultCode = NULL);
	// 从玩家或者NPC身上干掉BUFF
	int RemoveBuff( CEntityCharacter* pDesEntity, int nBuffID );

	void SetDefaultValue( CEntityPlayer* pEntity );

	void SaveStallInfo(
		CEntityPlayer *pEntity,
		const char* pName,
		const char* pAd,
		int* pNumAry, 
		int* pPriceAry, 
		int* pItemIndexAry, 
		int* pStallIndexAry, 
		int* pPriceTypeAry,
		int vNumber,
		int vStallType);
	int GetStallInfo( CEntityPlayer* pPlayer, const int vStallType, char* pName, char* pAd, int* pItemIndexAry, int* pStallIndexAry, int* pPriceAry, int* pPriceTypeAry );

	void BuffByDamage( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, EResultType vResultType, int vSkillType, int& vDamage, int& tDesDamage, int& tShieldDamage, bool& tMagicShield );

	void CheckBuffEntityUseSkill( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity );

	// 获得功勋
	void PlayerObtainHonor( CEntityPlayer* pSrcEntity, CEntityPlayer* pDesEntity, int* pMemberList = NULL, int vMemberCount = 1 );
	// 功勋换经验
	void PlayerChangeHonorToExp( CEntityPlayer *pPlayer, int vHonor );
	// 金钱复活获得功勋
	void PlayerReliveHonor( CEntityPlayer* pPlayer, CPropertyPlayer* pProperty );
	void OnPlayerObtainHonor( CEntityPlayer* tpPlayer, int vHonor );

	void CheckBuffGetSkillAttackDesEntity( CEntityCharacter* pDesEntity, CTemplateSkill* vpSkill, int& tDesBuffFix, int& tDesBuffPer );
	void CheckBuffGetSkillAttackSrcEntity( CEntityCharacter* pSrcEntity, CTemplateSkill* vpSkill, int& tDeathRate, int& tDeathValue, int& tIgnoreRate, int tIgnoreType );
	void CheckBuffGetNormalAttackSrcEntity( CEntityCharacter* pSrcEntity, int& tDeathRate, int& tDeathValue );

	// 检查每周的刷新点
	void WeekRefresh();

	// 持续攻击
	void ContinuedAttackSkill( CEntityCharacter* pSrcEntity, CTemplateNormalSkill* pNormalSkill, CWTPoint& vPos, CEntityCharacter** vEntityList, int vEntityListNum );

	// 持续诅咒或祝福
	void ContinuedCussOrBlessSkill( CEntityCharacter* pSrcEntity, CTemplateNormalSkill* pNormalSkill, CWTPoint& vPos, CEntityCharacter** vEntityList, int vEntityListNum );

	// 光环持续上报
	void ContinuedAureoleSkill( CEntityCharacter* pSrcEntity, CTemplateAureoleSkill* pAureoleSkill, CWTPoint& vPos, CEntityCharacter** vEntityList, int vEntityListNum );
	/////	这些是UseItem的函数
	// 使用药品
	int OnUsePhysics( CEntity *pEntity, CTplItem *tpItem, CItemObject* tpSrcItem, int vIndex, int &vRemainHP, int &vRemainMP, int &vRemainAP );
	// 使用随机传送
	int OnUseRandomTP( CEntity *pEntity, int vIndex );
	// 使用记忆传送
	int OnUseRemberTP( CEntity *pEntity, int vIndex );
	// 使用回城道具
	int OnUseTownTP( CEntity *pEntity, int vIndex );
	// 使用坐骑
	int OnUseHorse( CEntity *pEntity, CTplItem *tpItem, CItemObject* tpSrcItem );
	// 使用门派返回卷
	int OnUseReturnPaper( CEntity *pEntity, CTplItem *tpItem, int vIndex );
	// 使用传送符
	int OnUseTeleSymbol( CEntity *pEntity, CTplItem *tpItem, int vIndex, unsigned short vTeleOrder, int& nUsedTimes );
	// 使用炉石
	int OnUseStove( CEntity *pEntity, CTplItem *tpItem, CItemObject* tpSrcItem, int vIndex );
	// 使用藏宝图
	int OnUseMineMap( CEntity* pEntity, CTplItem* tpItem, int vIndex );
	// 使用令牌
	int OnUseToken( CEntity* pEntity, CTplItem* tpItem, int vIndex );
	// 使用卡片
	int OnUseCard( CEntity* pEntity, CTplItem* tpItem, int vIndex );
	// 使用礼包
	int OnUseGiftBag( CEntity* pEntity, CTplItem* tpItem, int vIndex, int vParam );
	// 使用生活技能卷轴
	int OnUseLifeSkillScroll( CEntity* pEntity, CTplItem* tpItem, int vIndex );

	// 使用 功能性物品
	int OnUseFuncItem( CEntity* pEntity, CTplItem* tpItem, int vIndex );
	
	// 使用红包
	int OnUseRedPaper( CEntity* pEntity, CTplItem* tpItem, int vIndex );
	
	// 使用黑狗丸
	int OnUseBlackPills( CEntity* pEntity, CTplItem* tpItem, int vIndex );
	
	// 计算离线经验丹的时间 nTemplateID: 离线经验丹的 templateid
	int ChangeOffLineTimeToExp( CEntityPlayer *pPlayer, int nIndex = -1 ,int nTemplateID = -1);

	// 使用会员道具
	int OnUseVipItem( CEntity* pEntity, CTplItem* tpItem, int vIndex );
	void SendVipNotice( CEntityPlayer* npPlayer, bool bFlag );
	void RefreshVip( CEntityPlayer* npPlayer );

	// 使用洗点道具
	int OnUseCleanItem( CEntity* pEntity, CTplItem* tpItem, int vIndex );
	
	// 使用家族军团特供物品
	int OnUseLeagueBagItem( CEntity* pEntity, CTplItem* tpItem, int vIndex );	
	
	// 更改国家
	int ChangeNationality( CEntityPlayer* vpEntity, CampDef vCamp );

	// 领双归零
	int RefreshDiploid( CEntityPlayer* pEntity, bool bOnLine );

	// 每周一0点要处理的事情(不在线 则在下次上线的时候自动处理)
	int WeeklyDispose( CEntityPlayer* pPlayer, bool bIsOnline );
	int DaylyDispose( CEntityPlayer* pPlayer, bool bIsOnline );

	// 随即掉落物品
	int RandomDropItem( CEntityPlayer* pEntity, int vDropID, const CDropFrom& vDropFrom, int vDropType );
	// 身上随机中负面buff
	int RandomHitBuff( CEntityPlayer* pEntity, int vBuffID );
	// 附近区域刷出几个怪
	int BrushOgre( CEntityPlayer* pEntity, int vOgreID, int vNum, bool bNotify, bool bNormal );
	// 进入副本
	int LoginRepetion( CEntityPlayer* pEntity, CTplMineMap* tpMap);
	
	// drop item 
	int DropItemOnPkDie( CEntityPlayer* pPlayer );
	int DropEquip( CEntityPlayer* pPlayer );
	int DropItem( CEntityPlayer* pPlayer, int vNum );

	// 囚禁玩家 [12/14/2009 Macro]
	// npPlayer :		囚犯对象, 
	// nPrisonTime :	囚禁时间(毫秒)
	// return : -1 失败, 0 成功
	int OnEventCoopEntity( CEntityPlayer* npPlayer, int nPrisonTime );
	
	// 监狱释放 [1/8/2010 Macro]
	int OnEventUnCoopEntity( CEntityPlayer* npPlayer );

	void OnEventNotifyPrisonTime( CEntityPlayer* npPlayer );
	void SendPrisonTimeNotice( CEntityPlayer* npPlayer, int nPrisonTime );

	void SendUseItemNotify( CEntityPlayer* pPlayer, int vItemID );
	
	// 发送使用黑狗丸的通知消息
	void SendUseBlackPillsNotify( CEntityPlayer *pPlayer, int nOffLineTime, int nPillsLeftTime, int nPillsIndex, int nExp, int nOffLineTimeConsumed, int nBlackPillID, int nErrcode );
		
protected:
	static CEntityResultList mResultList;
	static CEntityResultList mSingleList;

	// 处理单个消息时的实体删除列表
	unsigned int mEntityDeleteList[ FUNC_LIMIT ];
	unsigned int mDeleteListCount;
	
public:
	inline CEntityResultList* GetResultList() { return &mResultList; }
	inline CEntityResultList* GetSingleList() { return &mSingleList; }

	// 犀哥专用
	void UseSkillForServer( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateSkill* pSkill, CEntityCharacter** vEntityList, int vEntityListNum );

	//// 玩家用技能
	//int PlayerUseSkill( );

	//// 召唤兽用技能
	//int PetUseSkill();


	// 指令刷怪
	void PlayerGetOgre( CEntityPlayer* pPlayer, int nOgreID );
	void CreateNpc( CEntityPlayer* pPlayer, int nNpcTempID, EEntityType nEntityType, CWTPoint& pos, int nLifeTime = 0, bool bIsSetOwner = false, bool bIsShout = false, int nDestMapID = 0, int nIsShowOwner = 0 );
	void CreateHierogram( CTplHierogram* pTpl, CWTPoint* tpPos, CMapObject* pMapObject );

	// 初始化实体删除列表
	void InitDeleteList( );

	// 获得实体删除列表
	void AddDeleteList( unsigned int nEntityID );

	// 清空实体删除列表中的实体
	void ClearDeleteList( );

	// NPC变怪物
	int FuncNpcChange2Ogre( CEntityPlayer* pPlayer, CEntityNpc* pEntityNpc, unsigned short vTaskID =0 );

	// NPC变回来(玩家挑战失败)
	void RecoverNpcState( CEntityNpc* pEntity );

	// 重置功能NPC状态 (在变成怪物后重置回去)
	void ResetFuncNpc( CEntityNpc* pEntity );

	// 通知客户端 NPC 的PK状态 改了
	void NotifyClientNpcPKTypeChanged( CEntityNpc* pEntityNpc );

	// 在线减少邪恶值
	void RefreshPkValue( CEntityPlayer* npPlayer, int nTickOffset );

	// 刷新称号附加属性
	void RefreshTitleAddProperty( CEntityPlayer* npPlayer );

	// 在线处理活力值
	void RefreshApValue( CEntityPlayer* vpPlayer );

	// 使用召唤类物品
	int OnUseItemCall(CEntity *pEntity, CTplItem *tpItem, int vIndex);

	// 使用技能类物品
	int OnUseItemSkill(CEntity *pEntity, CTplItem *tpItem, int vIndex);	

	// 触发称号获取事件
	// vType: 称号类型:任务、好友。。。
	// vParam1：获取该称号的条件
	// vParam2 ...
	int FireTitleEvent( CEntityPlayer* vpPlayer, int vType, int vParam1, int vParam2 = 0 );

	// 跨模块调用，玩家增加称号
	// nTitleID :　称号ID
	int OnPlayerAddTitleEvent( CEntityPlayer* vpPlayer, int nTitleID, int nValidTime = 0 );
	// 删除一个称号
	int OnPlayerDelTitleEvent( CEntityPlayer* npPlayer, int nTitleID );

	int InsertTitleSkill( CEntityPlayer* vpPlayer, int* vTitleList, int vNum );

	// 自动关机 商城购买的道具做绑定非绑定处理
	int GetStoreItemID( int* npItemList, int nLen, int nType, int nIndex );


	// 通知客户端更新称号数据
	void SendNotifyClientUpdateTitle(CEntityPlayer *vpPlayer, int vType, int vTitleID);
	void SendNotifyUpdateCurrentTitle( CEntityPlayer *vpPlayer, int vTitleID, const char *pTitleName = NULL );

	void OnEventSaveStallRecord( CEntityPlayer* vpPlayer, CStallRecord* vpRecord ,const int vStallType);
	void OnEventGetStallRecord( CEntityPlayer* vpPlayer, CMessageStallGetRecordResponse* vpMessage, const int vStallType );
	
	// 创建实体的时候检查包里的物品 看有没有非法的
	void CheckBagContent( CEntityPlayer *pPlayer );
	void CheckEquipment( CEntityPlayer *pPlayer ) ;

	// 玩家的限时物品到时间了处理
	// 参数表：玩家，目标物品，物品所在位置, 到期类型（0上线时删除，1定时器删除)
	int OnPlayerItemOverTime( CEntityPlayer* pPlayer, CItemObject* pItemObj, EPosType posType, int nTimeoutType );
	void CheckItemInvilidDate( CEntityPlayer* pPlayer, bool bIsNotify );

	// 往包裹查道具时，对藏宝图、惩妖令牌等类似需要随机触发点的接口
	template< typename TYPE_ITEM, typename TYPE_OBJ >
	int SetItemPosRandom( TYPE_ITEM* vpTplItem, TYPE_OBJ* vpObj )
	{
		if ( vpTplItem == NULL || vpObj == NULL )
		{
			return -1;
		}
		int tIndex = 0; 
		int	tPosIndex[ MAX_MINEPOS ] = { 0 };
		for ( int i = 0; i < (int)ARRAY_CNT( vpTplItem->mPos ); i ++ )
		{
			if ( vpTplItem->mPos[ i ].mMapID == 0 )
			{
				continue;
			}
			tPosIndex[ tIndex ++ ] = i;
		}
		int tRandom = RAND( tIndex );

		if ( tRandom >= (int)ARRAY_CNT( vpTplItem->mPos ) )
		{
			return -1;
		}		

		vpObj->SetMapID( vpTplItem->mPos[ tRandom ].mMapID );
		vpObj->SetPosX( vpTplItem->mPos[ tRandom ].mPosX );
		vpObj->SetPosY( vpTplItem->mPos[ tRandom ].mPosY );
		return SUCCESS;
	}

	void NotifyRefreshMagicWeaponProperty( CEntityPlayer* pPlayer, int nBagIndex );
	void NotifyRefreshMagicWeaponProperty( CEntityPlayer* pPlayer, int nType, int nBagIndex );
	int  OnMagicWeaponObtainExp( CEntityPlayer* pPlayer, int nExp, bool bIsPile=false );
	void OnTimeOutMessageMagicWeaponIncMp( CMessage* pMsg );
	void OnTimeOutMessageMagicWeaponDecHp( CMessage* pMsg );
	void OnMessageMWAddPotential( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageChangeExpScaleRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageMWAddSkillPoints( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageMWJudgeSkill( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageLockMWInBagRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessagePlayerTakeOffMW( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessagePlayerJudgeMWProperty( CEntityPlayer* pPlayer, CMessage* pMessage );

	void RefreshPlayerProperty( CEntityPlayer* pPlayer );
	void OnMessageMWUseSkillBookRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageChangePKDrop( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnMessageProcessMultiBuffRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageMagicWeaponClearPointAddMWRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageMagicWeaponClearPointRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageGetHelpRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 玩家请求传送道具的传送点列表
	void OnMessageGetTeleList( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 玩家添加传送点
	void OnMessageAddTelePoint( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 玩家删除传送点
	void OnMessageDelTelePoint( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 玩家使用传送道具
	void OnMessageUseTeleItem( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 收到客户端领取元宝请求
	void OnMessageDrawGoldenYB( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 收到客户端查询元宝余额请求
	void OnMessageQueryGoldenYB( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理给客户端的传送通知
	void SendMultiTeleportNotify( CAsynTeleport vTeleport, char *vName );

	// 处理给客户端的传送通知
	void SendSingleTeleportNotify( int vCharID, ComplexPos vPos, char *vName, int vTokenID );
	
	// 开启内丹(直接噬炼)
	void StartRedstone( CEntity *pEntity, int nOwnerID, int nRedstoneTmpID, bool bDestroyEntity = true );
	
	// 设置密码安全时长的消息
	void OnMessageSetPwdProtectTimeRequest(  CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 发送设置安全密码安全时长的消息回应
	void SendResponseSetPwdProtectTime( CEntityPlayer *pPlayer, int nErrcode, int nTimeLength );
	
	// 发送安全密码状态改变的通知
	void SendNotifyChangePasswordStatus( CEntityPlayer *pPlayer, int nPasswordStatus, const char *pPassword = NULL );

	// 通知网关 vip 变化
	void SendVipFlagUpdateToGate( CEntityPlayer *npPlayer );
	
	// 对物品进行普通绑定的消息请求
	void OnMessageCommonBindRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 发送普通绑定的回应消息
	void SendCommonBindResponse( CEntityPlayer *pPlayer, int nErrcode, int nItemIndex, int nItemSlot );
	
	// 客户端开始读秒的消息请求
	void OnMessageStartProgressRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 客户端开始读秒的消息回应
	void SendStartProgressResponse( CEntityPlayer *pPlayer, int nErrcode );
	
	// 使用绣花针的消息请求
	void OnMessageUseKitbagNeedleRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 发送使用绣花针的回应消息
	void SendUseKitbagNeedleResponse( CEntityPlayer *pPlayer, int nErrcode, int nKitbagSlot, int nKitbagIndex, int nNeedleIndex, int nExpiredTime, int nTimeLength );
	
	// 安全保护密码重新锁定的请求
	void OnMessageLockPasswordRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 发送保护密码重新锁定的回应消息
	void SendLockPasswordResponse( CEntityPlayer *pPlayer, int nErrcode, int nStatus );

	// 学习妖魔志技能请求
	void OnMessageLearnOgreSkillRequest(CEntityPlayer* pPlayer, CMessage* pMessage );

	// 查询传送点请求
	void OnMessageQueryPedalRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	void SendGetItemNotify( CEntityPlayer *pPlayer, int vItemID, int vNum, int vType, int vSpecial = 0, int vLevel = 0 );
	
	// 检查结婚装备能否使用
	int CheckMarriageEquipmentValid( CEntityPlayer *pPlayer, CTplItem *pTplItem, CItemObject *pItemObj, CEntity *pEntity );

	// 记录不可堆叠的人民币道具消耗
	void LogIbItemUsed( CEntityPlayer* pPlayer, CItemObject * tpThrowItem);
	
	// 中断玩家读条
	void StopProgress( CEntityPlayer *pPlayer );

	// 使用召集物品
	int OnMessageUseSummonItemRequest( CEntityPlayer* pEntity, CMessage *pMessage );

	// 通知队员拾取物品
	void SendPickItemNotify( CEntityPlayer *pPlayer, int vItemID );

	// 增加装备印请求
	void OnMessageAddExtraRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnMessageWarOperateRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	void SendChangeOwnCampNotify( CEntityNpc *tpNpc );
	void SendWarEventNotify( int vEventID, int vMapID, int vPosX, int vPosY, int vSrcCampID, int vDesCampID, int vValue, int vTempID, const char *vName );

	void ObtainWarHonor( CEntityPlayer *pPlayer, int vValue, bool vSelf );
	
	// 记录装备普通绑定的时间
	void SetEquipBindTime( CItemObject *pItem );

	// 发送变身状态
	void SendPlayerTransformNotify( CEntityPlayer *pPlayer );
	// 学习方寸山技能
	void OnMessageLearnFangCunSkillRequest( CEntityPlayer* pPlayer, CMessage *pMessage );
	void SendLearnFangCunSkillResponse( CEntityPlayer* pPlayer, int vResult, int vSkillID, int vItemIndex, int vObjIndex );

	void OnMessageHonorToItemRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnPlayerLevelUp( CEntityPlayer *pPlayer, int vUpLevel );
	
	// 处理修炼经验丹的函数
	void AddExpToExpContainer( CEntityPlayer *pPlayer, int nExp );
	
	// 发送修炼经验丹经验改变的通知
	void SendExpContainerNotify( CEntityPlayer *pPlayer, unsigned long long nExp, int nExpAdded, int nIndex, int nExpPillsID );
	
	// 使用经验丹
	int OnUseExpPills(CEntity *pEntity, CTplItem *tpItem, int vIndex);

	//发送元宝更新消息
	void SendUpdateYuanbaoNotice( CEntity* pPlayer, const int vAmount, const int vType );

    // 摇钱树活动宝箱复活
    void OnTimeoutMessageEntityRelive2( CMessage* pMsg );
    
    // 使用时装绣花针
    void UseFashionNeedle( CEntityPlayer *pPlayer, int nNeedleIndex, int nFashionIndex, int nFashionslot );
	// 宝石雕琢
	void OnMessageJewelCarveRequest( CEntityPlayer* pPlayer, CMessage *pMessage );
	
	// 播放数字礼花的消息请求
	void OnMessageStartDigitalFireworks( CEntityPlayer* pPlayer, CMessage *pMessage );	
	
	// 发送播放数字礼花的回应消息
	void SendResponseStartDigitalFireworks( CEntityPlayer *pPlayer, int nErrcode );

	void LogRoleDamage( CEntityPlayer* pSrcPlayer, CEntityPlayer* pDesPlayer, int vSkillID, int vRealDamage, int vResultType );

	// 更换时装显示状态
    void OnMessageChangeFashionShow( CEntityPlayer* pPlayer, CMessage* pMessage );
    
	// 更换装备显示状态
	void OnMessageChangeEquipShow( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 发送装备宝石升级消息
	void SendUpgAndJewNotice( CEntityPlayer *pPlayer, int vType, int vLevel, CItemObject *pObject );

	// 查询BOSS
	void OnMessageQueryBossRequest( CEntityPlayer* pPlayer, CMessage *pMessage );

	// 法宝进化相关
	void OnMessageDecomposeMW( CEntityPlayer* pPlayer, CMessage *pMessage );
	void OnMessageMWQLPrintRequest( CEntityPlayer* pPlayer, CMessage *pMessage );
	void OnMessageMWQuickUpRequest( CEntityPlayer* pPlayer, CMessage *pMessage );
	void OnMessageQilinComposeRequest( CEntityPlayer* pPlayer, CMessage *pMessage );
	void MWLevelUp(CEntityPlayer* pPlayer, int nLevel);

	void OnMessageBagLockRequest( CEntityPlayer *pPlayer,  CMessage *pMessage );
	void OnMessageQueryAroundPlayerRequest( CEntityPlayer *pPlayer,  CMessage *pMessage );

	void SendPlayerRepetData(CEntityPlayer * pPlayer, int vRepetionIndex, int vDataIndex );
	// int nSlotType种类,  int nIndex位置, int nIndexNum 各自生活,int nAddTime 加上的时间
	bool AddBagInvalidTime( CEntityPlayer *pPlayer, int nSlotType, int nIndex, int nIndexNum,int nAddTime );
	//添加 vip时间 不是 vip可以成为vip
	bool AddVIPTime( CEntity *pEntity, int tAddTime );

	// 加载客户端功能控制配置的回调函数
	static void LoadClientCtrlConfigCallback(const char* pcConfigPath);

	void ChangeCombatFame( CEntityPlayer *pPlayer, int vDeltaFame );
	void ChangeRepeFame( CEntityPlayer *pPlayer, int vDeltaFame );
	void ChangeCommonFame( CEntityPlayer *pPlayer, int vDeltaFame );
	void SendSyncFameNotify( CEntityPlayer *pPlayer, int vCombatDelta, int vRepeDelta, int vCommonDelta );

	// 铸灵
	void OnMessageSpiritOpenRequest(CEntityPlayer* pPlayer, CMessage *pMessage);
	void OnMessageSpiritCastingRequest(CEntityPlayer* pPlayer, CMessage *pMessage);
	void OnMessageSpiritUpgradeRequest(CEntityPlayer* pPlayer, CMessage *pMessage);
	void OnMessageSpiritDowngradeRequest(CEntityPlayer* pPlayer, CMessage *pMessage);
	int	 PlayerSpiritUpgrade(CEntityPlayer *pPlayer, int *apnSpiritIndex, int nUpgradeItemIndex);
	int  PlayerSpiritDowngrade(CEntityPlayer *pPlayer, int nPosition, int nType, int nSlot, int nItemIndex);
	void OnMessageSpiritInfoRequest(CEntityPlayer* pPlayer, CMessage *pMessage);
	void SendSpiritInfo(CEntityPlayer* pPlayer);
	int  PlayerSpiritCasting(CEntityPlayer *pPlayer, int nPositionIndex, int nTypeIndex, int nSlotIndex, int nItemIndex);
};