#pragma once

#include "coretype.h"
#include "protype.h"
#include "sceneobj_manager.h"
#include "promessage_pb.hxx.pb.h"
#include "share_define.h"
#include "property_pb.hxx.pb.h"
#include "lk_vector.h"
#include "lk_hashmap.h"
#include "template.h"
#include "timer_define.h"
#include "skill_list.h"
#include "buffer.h"

class CProperty;
class CPropertyCharacter;
class CPropertyPlayer;
class CPropertyNPC;
class CPropertyPet;
class CEntityCharacter;
class CEntityResultList;
class CEntityPlayer;



enum EquipProType
{
	TYPE_NONE = 0,
	TYPE_PATTACKLOW,
	TYPE_PATTACKHIGH,
	TYPE_MATTACKLOW,
	TYPE_MATTACKHIGH,
	TYPE_DEATHRATE,
	TYPE_DEATHVALUE,
	TYPE_PDEFENCE,
	TYPE_MDEFENCE,
	TYPE_MAXHP,
	TYPE_MAXMP,
};

class CPropertyValue
{
public:
	int			mInitValue;
	int			mDeltaValue;
public:
	CPropertyValue( ) //: mInitValue( 0 ), mDeltaValue( 0 )	
	{
		if( CObj::msCreateMode )
		{
			mInitValue = 0;
			mDeltaValue = 0;
		}
	}
public:
	operator int( )
	{ return mInitValue + mDeltaValue; }
	operator unsigned int( )
	{
		return mInitValue + mDeltaValue;
	}	

	bool operator ==( CPropertyValue& tOther )
	{
		return ( ( mInitValue == tOther.mInitValue ) && ( mDeltaValue == tOther.mDeltaValue ) );
	}
};

// ********************************************************************** //
// CItemObject
// ********************************************************************** //

class CItemObject : public CObj
{
protected:
	unsigned int	mItemID;					// 道具ID
private:
	unsigned int	mNumber;					// 道具数量
	int				mLocked;					// 锁定类型
	int             mBindStatus;                // 绑定状态
	int				mUsedTimes;					// 已使用次数
	uint64_t		mPurchaseID;				// 购买的交易ID
	OBJGUID			mObjGUID;					// 生成时的唯一标识 
	time_t			mInvalidTime;				// 到期时间
	
	int				mValidTimerID;				// 有效时间定时器ID，实时数据，不存盘
	int				mBindTime;					// 物品绑定的时间
public:
	//TODO
	CItemObject()
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

	CItemObject( unsigned int vItemID , unsigned int vNumber = 0 ) : mItemID( vItemID ), mNumber( vNumber ), mLocked( false ), mPurchaseID( 0)
	{
	}

	virtual int Initialize()
	{
		mItemID = 0;
		mNumber = 0;
		mLocked = 0;
		mBindStatus = 0;
		mUsedTimes  = 0;
		mPurchaseID = 0;
		mObjGUID.a.guid = 0;
		mInvalidTime = 0;
		mValidTimerID = INVALID_OBJ_ID;
		mBindTime = 0;
		return 0;
	}
	
	virtual int Resume()
	{
		return 0;
	}

public:
	
	// 道具ID
	inline unsigned int GetItemID()	{ return mItemID; }
	inline void SetItemID( unsigned int unItemID ) { mItemID = unItemID; }

	// 道具个数
	inline unsigned int GetItemNum() { return mNumber; }
	inline void SetItemNum( unsigned int unItemNum ) { mNumber = unItemNum; }

	// 绑定状态
	inline int GetBindStatus() { return mBindStatus; }
	inline void SetBindStatus( int BindStatus )
	{				
		mBindStatus = BindStatus;				
	}

	void ChangeBindStatus( int nBindStatus )
	{
		mBindStatus |= nBindStatus;
	}

	// 使用次数
	inline int GetUsedTimes() {return mUsedTimes;}
	inline void SetUsedTimes( int nUsedTimes ) { mUsedTimes = nUsedTimes; }

	// lock status
	inline int GetLockStatus() { return mLocked; }
	inline void SetLockStatus( int nLock ) { mLocked = nLock; }
	
	// 绑定物品
	int BindItem( int vBindType );

	void SetGUID() ;

	inline uint64_t GetGUID()
	{
		return mObjGUID.value();
	}

	inline OBJGUID GetGUIDObj()
	{
		return mObjGUID;
	}

	inline void SetGUID( uint64_t nGUID ) { mObjGUID.a.guid = nGUID; }
	
	// 交易ID
	inline uint64_t GetPurchaseID() { return mPurchaseID ; }
	inline void SetPurchaseID( uint64_t unPurchaseID ) { mPurchaseID = unPurchaseID; }

	// 有效时间
	inline unsigned int GetInvalidTime( ) { return mInvalidTime; }
	inline void SetInvalidTime( int nInvalidTime ) { mInvalidTime = nInvalidTime; }
	
	// 有效时间timer
	inline int GetValidTimerID( ) { return mValidTimerID; }
	inline void SetValidTimerID( int nValidTimerID ) { mValidTimerID = nValidTimerID; }	

	// 是否已过期
	bool IsOvertimed()
	{
		if ( GetInvalidTime() == 0 )
		{
			return false;
		}
		else
		{
			if ( GetInvalidTime() <= (unsigned int)time(0) ) // 到期了
			{
				return true;
			}
		}
		return false;
	}

public:

	void GetItemBinaryBuffer2( PBItemObject* pBuffer, int vNumber = -1 );
	virtual void GetItemBinary2( PBItemObject*& pGene ) = 0;

	/** 是否是同一种道具组(附加包括特定绑定类型)
	**  作用是区别对待人民币道具和游戏产出道具，但在某些功能上又需要等价，
	**	所以有组的概念
	*/
	bool IsSameItemGroup( unsigned int nItemID, int nType );

	/** 是否是地图受限物品
	**  
	*/
	bool IsMapLimitedItem( int nCurMapID );

	/** 是否是副本受限物品
	**  
	*/
    bool IsRepetionLimitedItem( int nCurRepetionIndex );
};

// ********************************************************************** //
// CItemEquipment
// 装备道具
// ********************************************************************** //

class CItemEquipment : public CItemObject
{
protected:
	unsigned char	mSlotNumber;					// 打开的装备槽个数	
	unsigned char	mNameLength;					// 生产者名字长度
	int				mLevel;							// 装备等级
	unsigned int	mSlotProperty[ JEWELNUM ];		// 装备槽是否打开，0未开，1打开
	unsigned int	mProperty[ PROPERTYNUM ];		// 附加属性ID,0表示没有附加属性
	unsigned int	mSlotValue[ JEWELNUM ];			// 装备槽的宝石ID
	unsigned int	mValue[ PROPERTYNUM ];			// 附加属性数值
	unsigned int	mSlotSpell;						// 镶嵌的符文

    int              mCurDurability;				// 装备耐久度
	unsigned int     mAbrasionCount;				// 损耗点数，与npc交互损耗加100与玩家交互损耗加1000，满10000后减少一点耐久度
   	unsigned int     mBindFreeTime;                 // 解除装备绑定的时间                

	CPropertyValue	mMaxSlotNumber;					// 装备可提供的最大槽个数
	unsigned int	mJudge[ UPGVALUENUM ];			// 本体属性的附加值
	int				mResDef;						// 抗性类别
	int				mResAtt;						// 属性攻击类别
	int				mDefLevel;						// 抗性等级
	int				mAttLevel;						// 属性攻击等级
	int				mMagicStoneID;					// 附魔石ID
	int				mMagicStoneNum;					// 同种附魔石的附魔次数
	int				mSpouseID;						// 配置的角色ID
	int				mExtraType;						// 附加的类型
	int				mBindTime;						// 装备被普通绑定的时间
	char			mSpouseName[NAME_LENGTH];		// 配偶的名称
	char			mName[ NAME_LENGTH ];			// 生产者名字O
public:

	CItemEquipment( )
	{
		if ( CObj::msCreateMode )
		{
			Initialize ();
		}
		else
		{
			Resume();
		}
	}

	virtual ~CItemEquipment( ) {}
public:

	// BindFree Time
	inline int GetBindFreeTime()	{	return mBindFreeTime;	}	
	inline void SetBindFreeTime( int FreeTime ){	mBindFreeTime = FreeTime;	}		
	
	// Level
	inline int GetLevel() { return mLevel; }
	inline void SetLevel( int nLevel ) { mLevel = nLevel; }

	// SlotNumber
	inline unsigned char GetSlotNumber( ) { return mSlotNumber; }
	inline void SetSlotNumber( unsigned char ucSlot ) { mSlotNumber = ucSlot; }

	// SlotProperty
	inline unsigned int GetSlotProperty( int nIdx ) { return mSlotProperty[nIdx] ;}
	inline void SetSlotProperty( int nIdx, unsigned int usValue ) { mSlotProperty[nIdx] = usValue; }

	// Property 解决重载问题
	inline unsigned int GetProperty( int nIdx ) { return mProperty[nIdx]; }
	inline void SetProperty( int nIdx, unsigned int usValue ) { mProperty[nIdx] = usValue; }

	// SlotValue
	inline unsigned int GetSlotValue( int nIdx ) { return mSlotValue[ nIdx] ; }
	inline void SetSlotValue( int nIdx, unsigned int usValue ) { mSlotValue[nIdx] = usValue; }

	// Value
	inline unsigned int GetValue( int nIdx ) { return mValue[ nIdx ] ; }
	inline void SetValue( int nIdx, unsigned int usValue ) { mValue[nIdx] = usValue; }

	// NameLength
	inline unsigned char GetNameLength() { return mNameLength; }
	inline void SetNameLength( unsigned char ucNameLen ) { mNameLength = ucNameLen; }
	inline unsigned char GetMaxNameLength() { return sizeof(mName)-1; }

	// Name
	inline char* GetName() { return mName; }
	inline void SetName( const char* pName ) { strncpy( mName, pName, sizeof(mName)-1 ); mName[ sizeof(mName)-1] = 0; }
	
	//mSlotSpell
	inline unsigned int GetSlotSpell() { return mSlotSpell; }
	inline void SetSlotSpell( unsigned int usSpell ) { mSlotSpell = usSpell; }

	// mCurDurability
	inline int GetDurability() { return mCurDurability; }
	inline void SetDurability( int nDur ) { mCurDurability = nDur; }

	// mAbrasionCount
	inline int GetAbrasionCount() { return mAbrasionCount; }
	inline void SetAbrasionCount( int nAbr ) { mAbrasionCount = nAbr; }

	// mMaxSlotNumber
	inline CPropertyValue& GetMaxSlotNumber() { return mMaxSlotNumber; }
	inline void SetMaxSlotNumber( CPropertyValue nMaxSlotNumber) { mMaxSlotNumber = nMaxSlotNumber; }

	inline void SetMaxSlotNumberInit( int nInit ) { mMaxSlotNumber.mInitValue = nInit; }
	inline void SetMaxSlotNumberDelta( int nDelat ) { mMaxSlotNumber.mDeltaValue = nDelat; }

	// mJudge
	inline unsigned int GetJuge( int nIdx ) { return mJudge[nIdx]; }
	inline void SetJuge( int nIdx, unsigned int nValue ) { mJudge[nIdx] = nValue; }

	// mResDef
	inline int GetResDef( ) { return mResDef;}
	inline void SetResDef( int nResDef ) { mResDef = nResDef; }

	// mResAtt
	inline int GetResAtt( ) { return mResAtt;}
	inline void SetResAtt( int nResAtt ) { mResAtt= nResAtt; }

	// mDefLevel
	inline int GetDefLevel() { return mDefLevel; }
	inline void SetDefLevel( int nDefVal ) { mDefLevel = nDefVal; }

	// mAttLevel
	inline int GetAttLevel( ) { return mAttLevel; }
	inline void SetAttLevel( int nAttLevel ) { mAttLevel = nAttLevel; }

	// nMagicStoneID
	inline void SetMagicStoneID( int nMagicStoneID ){ mMagicStoneID = nMagicStoneID; }
	inline int  GetMagicStoneID(){ return mMagicStoneID; }

	// mMagicStoneNum
	inline void SetMagicStoneNum( int nMagicStoneNum ){ mMagicStoneNum = nMagicStoneNum; }
	inline int GetMagicStoneNum( ){ return mMagicStoneNum; }

	virtual int Initialize();	

	virtual int Resume( ) 	{	return CItemObject::Resume( );	}	

	int GetPropertyFromTpl( int vItemID, bool vInit = true );
	
	void SetSpouseID( int nSpouseID ){ mSpouseID = nSpouseID;  }
	int	GetSpouseID(  ){ return mSpouseID; }
	
	void SetSpouseName( const char *pSpouseName )
	{
		if ( pSpouseName != NULL )
		{
			strncpy( mSpouseName, pSpouseName, NAME_LENGTH - 1 );
			mSpouseName[NAME_LENGTH -1]='\0';	
		}				 
	}
	const char *GetSpouseName(){ return mSpouseName; }

	inline int GetExtraType( ) { return mExtraType; }
	inline void SetExtraType( int nExtraType ) { mExtraType = nExtraType; }
	
	// 设置&获取装备的绑定时间
	void SetBindTime( int nBindTime ) { mBindTime = nBindTime; }
	int GetBindTime(){ return mBindTime; }
public:
	// 减少装备的耐久度
	int ReduceDurability(  int NpcType , int PlayerAlive = 1 );

	// 设置装备耐久度为初始值
	int ResetDurability( );
	
    int GetLostDurability( );
   
	int GetMendMoney( );
	
public:
	
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
	static void SetValueFromPB( CItemEquipment* tpItem, PBItemObject*& pGene );

	// 建立一个"新"道具，随机生成各种属性
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber ) ;
	static void SetValueFromCreator( CItemEquipment* tpEquip, unsigned int vItemID, unsigned int vNumber );

	virtual void GetItemBinary2( PBItemObject*& pGene );
	
	int GetFixNumber( CTemplateProperty::PROPERTY_FUNC vFunc, int vEquipProType = 0, CPropertyPlayer *pProperty = NULL );

	int GetPerNumber( CTemplateProperty::PROPERTY_FUNC vFunc );

	// 得到装备对指定技能的等级加成
	unsigned short GetIncSkillLevel( unsigned short vSkillID );
	unsigned short GetValueByID( unsigned short vSkillID, CTemplateProperty::PROPERTY_FUNC vType );
	int	GetStateID( StateList& vBuffList,CTemplateProperty::PROPERTY_FUNC vType );

	void GetSkillList( std::vector< unsigned short >& rSkillList, CTemplateProperty::PROPERTY_FUNC vFunc );
	
	// 装备附魔
	int SetMagicStone( int nMagicStoneID );
	
	// 获取附魔属性
	int GetMagicAttribute(  );	

	// 判断是否是结婚信物
	bool IsMarriageEquipment();

	// 判断是否是结拜信物
	bool IsSwornEquipment();
};

// ********************************************************************** //
// CItemMagicWeapon
// 法宝道具
// ********************************************************************** //

class CItemMagicWeapon : public CItemEquipment
{
private:
	// 基本属性
	int				mMWLevel;	// 法宝等级
	unsigned long long	mExp;		// 法宝经验
	unsigned int	mMp;		// 法力值
	unsigned int	mHp;		// 灵气值
	
	unsigned int	mCurKeenpoints;		// 当前锐力 (不含技能的影响)
	unsigned int	mCurCalmpoints;		// 当前定力 (不含技能的影响)	
	unsigned int	mCurMagicpoints;	// 当前法力 (不含技能的影响)	
	unsigned int	mCurBierpoints;		// 当前灵力 (不含技能的影响)	
	unsigned int	mCurImmortalpoints;	// 当前仙力 (不含技能的影响)	

	unsigned int	mExtraKeenpoints;		// 额外增加的锐力 (包括技能等影响)
	unsigned int	mExtraCalmpoints;		// 额外增加的定力 (包括技能等影响)
	unsigned int	mExtraMagicpoints;		// 额外增加的法力 (包括技能等影响)
	unsigned int	mExtraBierpoints;		// 额外增加的灵力 (包括技能等影响)
	unsigned int	mExtraImmortalpoints;	// 额外增加的仙力 (包括技能等影响)

	unsigned int	mAddKeenpoints;		// 玩家所加的锐力
	unsigned int	mAddCalmpoints;		// 玩家所加的定力
	unsigned int	mAddMagicpoints;	// 玩家所加的法力
	unsigned int	mAddBierpoints;		// 玩家所加的灵力
	unsigned int	mAddImmortalpoints;	// 玩家所加的仙力

	unsigned int	mLeftPoints;// 当前剩余潜能点

	// 洗点需要用到的数据：

	// 玩家本次升级新加 5种属性点的记录 升级清空
	unsigned int	mPlayerAddPointLastLevel[POINT_TYPE_NUM];	// 玩家本次升级前自己加的点的镜像
	unsigned int	mLeftPointsLastLevel;	// 本次升级前玩家的剩余潜能点
	unsigned int	mNewAddPoints;	// 本次升级新加的潜能点
	unsigned int	mNewAddSkillPoints;	// 本次升级新加的技能点
	int				mStepClearState;	// 玩家小洗状态
	int				mLastAddedSkillBoxIndex;	// 上次技能加点的技能框索引

	// 隐藏属性
	unsigned int	mProBase;	// 属性基础值
	unsigned int	mProUp;		// 属性成长值
	unsigned int	mProEff;	// 属性影响度
	unsigned int	mProSpir;	// 灵性

	// 技能/技能框相关
	unsigned int	mProcessVal;		// 当前进度值
	unsigned int    mLastLevelProcessVal;	// 小洗备份
	unsigned int	mLeftSkillPoints;	// 当前剩余技能点

	// 启发相关
	int				mNextActBoxIndex;	// 下一个将被激活的技能框索引
	int				mLastLevelNextActBox;	// 下一个将被激活的技能框索引

	unsigned int	mMaxMp;		// 法力值上限
	unsigned int	mMaxHp;		// 灵气值上限
	int				mIncMp;		// 法力值每次恢复点数
	int				mIncHp;		// 灵气值每次恢复点数

	int				mProJudgeFlag;		// 是否已完成属性鉴定

	CSkillListInstance<6>	mMWSkillList; // 本法宝的技能列表 (没有对应关系 供外部计算调用)
	int				mQilinID; // 已合成的器灵ID

	// 实时数据:不需要保存到数据库 但是在装上/卸下/上线/下线的时候要操作！
	int	mTimerID[MW_TIMER_NUM];
	int mOwnerEntityID;		// 佩戴者的实体ID 在佩戴的时候赋值

	bool IsNeedIncMp() { return mMp < mMaxMp; }
	bool IsNeedDecHp() { return mHp > 0; }
	void CheckTimers() ;

	// 增加减少 HP MP涉及操作比较多 直接set操作对外禁止
	void SetMp( unsigned int nValue) { mMp = nValue; }
	void SetHp( unsigned int nValue) { mHp = nValue; }

public:
	unsigned int    mSkills[SKILL_BOX_LMT];  // 每个技能框的已鉴定技能 0没有
	unsigned int    mSkillActNum[SKILL_BOX_LMT];  // 每个技能框的已鉴定次数
	unsigned int    mSkillBookID[SKILL_BOX_LMT];  // 每个技能框的已使用的技能书
	unsigned int    mAddedSkillPoints[SKILL_BOX_LMT];  // 每个技能框里的技能已加的技能点
	unsigned int    mSkillInitLevel[SKILL_BOX_LMT];  // 每个技能框里的技能初始等级
	unsigned char   mActiveState[SKILL_BOX_LMT];  // 每个技能框激活情况 0未激活

public:
	CItemMagicWeapon( )
	{
		if ( CObj::msCreateMode )
		{
			Initialize2();
		}
		else
		{
			Resume();
		}
	}
	virtual ~CItemMagicWeapon( ) {}	
	virtual void Initialize2();
	void InitializeMW(); // 初始化法宝数据

public:
	// 升级！ 只能一级一级的升！
	void LevelUp( ); 

	// 刷新各项属性值  参数：是否开启计时器
	void RefreshPropertys( bool bIsStartTimer = true ); 

	// 当装上法宝/进入场景的时间调用 初始化计时器 参数：佩戴者的指针
	void OnStartMagicWeapon( CEntityPlayer* pPlayer, bool bIsStartTimer=true );

	// 当卸下法宝的时间调用 干掉计时器 参数：佩戴者的指针 下线就不用调用了 反正会销毁的
	void OnEndMagicWeapon( CEntityPlayer* pPlayer );

	// 定时恢复MP
	int OnTimerIncMp();

	// 定时消耗HP
	int OnTimerDecHp();

	// 死亡掉HP
	int OnDieCutHp();

	// 对外增加(减少)MP接口 返回实际增加/减少值
	int  OnIncMp( int nVal, bool bIsStartTimer = true );

	// 对外增加(减少)HP接口 返回实际增加/减少值
	int  OnIncHp( int nVal );

	// 处理加点 返回0成功
	int  OnAddPotential( unsigned int nNewKeenpoints, unsigned int nNewCalmpoints, unsigned int nNewMagicpoints, unsigned int nNewBierpoints, unsigned int nNewImmortalpoints );

	// 新加技能（鉴定&领悟等用） 返回0成功  参数表：技能ID 技能等级 目标技能框  
	int  OnInsertSkill( int nSkillID, int nSkillLevel, int nBoxIndex, bool bIsStartTimer = true  );

	// 新鉴定技能 一定要加技能后再增加鉴定次数   参数：技能框ID  返回：0  成功 否则错误码
	int  OnJudgeNewSkill( int nBoxIndex, bool& bIsFirst, CMessageInsertSkillNotify* pMsg );

	// 领悟技能  输出实际领悟的索引列表 返回数量 在升级的时候调用
	int  OnSelfLearnSkill( unsigned char arrIndex[] );

	// 得到法宝技能基础值 主要用于发给客户端
	void GetSkillBaseInfo( unsigned char nBoxIndex, int& rSkillID, int& rSkillLevel, int& rActedNum, int& rBookID );

	// 升级后尝试激活新技能框(外部 在法宝升级的时候调用 因为设计和客户端通信)
	int  OnActiveNewSkillBox( int& nFirstActBoxIndex );

	// 法宝技能加点
	int  OnClickSP( int nBoxIndex );

	// 得到法宝攻击力
	unsigned int GetAttackValue();

	// 得到法宝的致命等级
	unsigned int GetDeathRate();

	// 使用技能书合成技能 传入 技能框索引 技能书的模板ID 返回 正确0 否则错误码
	int  OnUseSkillBook( int nBoxIndex, int nBookID, CMessageInsertSkillNotify* pMsg );

	// 当装上法宝/上线的时候 要启动技能CD
	void OnStartSkillCD( CEntityPlayer* pOwner );

	// 当卸下法宝/下线的时候 要停止技能CD
	void OnStopSkillCD( CEntityPlayer* pOwner );

	// 洗点--小洗
	int  Return2LastLevel();

	// 法宝重置（大洗）
	int ResetMagicWeapon(CEntityPlayer* pPlayer);

	// 获得本次升级新加的潜能点
	int	 GetThisLevelGotPoints();

	// 获得本次升级新加的技能点
	int	 GetThisLevelGotSkillPoints();

	// 从模板赋值
	int  SetupFromTemplate( );

	// 从模板设置技能 要传入所有者指针 因为要检查等级等信息
	int  SetupSkillFromTemplate( CEntityPlayer* pOwner );

	// 法宝属性鉴定 接口内部判断第几次 外部负责判断/扣除物品
	int  OnJudgeProperty( int nEffTableID, int nSpirTableID );

	// 法宝洗技能点
	int  ClearMWSkillPoint();
	int  GetTotalAddSP();

	// 法宝与器灵印记
	int  OnPrintQilin( int nQilinID );

private:
	// 得到某技能框的技能 该抽取的抽取 参数：技能框索引，是否第一次 返回：技能ID
	int GetBoxSkillNewID( int nBoxIndex, bool bIsFirst );

	// 备份玩家加点数据
	void BackUpData();

public: // 给张三的接口 提升玩家的接口	
	// 提升玩家攻击力
	static int GetAttackForPlayer( CPropertyPlayer* pProperty );
	// 提升玩家防御力
	static int GetRecoveryForPlayer( CPropertyPlayer* pProperty );
	// 提升玩家MP上限
	static int GetMaxMpForPlayer( CPropertyPlayer* pProperty );
	// 提升玩家HP上限
	static int GetMaxHpForPlayer( CPropertyPlayer* pProperty );
	// 提升玩家致命等级
	static int GetDeadLineForPlayer( CPropertyPlayer* pProperty );

public:
	int GetMWLevel() { return mMWLevel;}
	void SetMWLevel( int nValue) { mMWLevel = nValue;}

	unsigned long long GetExp() { return mExp;}
	void SetExp( unsigned long long nValue) { mExp = nValue;}

	unsigned int GetMp() { return mMp;}

	unsigned int GetHp() { return mHp;}

	unsigned int GetKeenpoints() { return mCurKeenpoints+mExtraKeenpoints;}
	unsigned int GetCalmpoints() { return mCurCalmpoints+mExtraCalmpoints;}
	unsigned int GetMagicpoints() { return mCurMagicpoints+mExtraMagicpoints;}
	unsigned int GetBierpoints() { return mCurBierpoints+mExtraBierpoints;}
	unsigned int GetImmortalpoints() { return mCurImmortalpoints+mExtraImmortalpoints;}

	void SetCurCalmpoints( unsigned int nValue) { mCurCalmpoints = nValue;}
	void SetCurKeenpoints( unsigned int nValue) { mCurKeenpoints = nValue;}
	void SetCurMagicpoints( unsigned int nValue) { mCurMagicpoints = nValue;}
	void SetCurBierpoints( unsigned int nValue) { mCurBierpoints = nValue;}
	void SetCurImmortalpoints( unsigned int nValue) { mCurImmortalpoints = nValue;}

	void SetExtraCalmpoints( unsigned int nValue) { mExtraCalmpoints = nValue;}
	void SetExtraKeenpoints( unsigned int nValue) { mExtraKeenpoints = nValue;}
	void SetExtraMagicpoints( unsigned int nValue) { mExtraMagicpoints = nValue;}
	void SetExtraBierpoints( unsigned int nValue) { mExtraBierpoints = nValue;}
	void SetExtraImmortalpoints( unsigned int nValue) { mExtraImmortalpoints = nValue;}

	unsigned int GetLeftPoints() { return mLeftPoints;}
	void SetLeftPoints( unsigned int nValue) { mLeftPoints = nValue;}

	unsigned int GetProBase() { return mProBase;}
	void SetProBase( unsigned int nValue) { mProBase = nValue;}

	unsigned int GetProUp() { return mProUp;}
	void SetProUp( unsigned int nValue) { mProUp = nValue;}

	unsigned int GetProEff() { return mProEff;}
	void SetProEff( unsigned int nValue) { mProEff = nValue;}

	unsigned int GetProSpir() { return mProSpir;}
	void SetProSpir( unsigned int nValue) { mProSpir = nValue;}

	unsigned int GetProcessVal() { return mProcessVal;}
	void SetProcessVal( unsigned int nValue) { mProcessVal = nValue;}

	unsigned int GetLeftSkillPoints() { return mLeftSkillPoints;}
	void SetLeftSkillPoints( unsigned int     nValue) { mLeftSkillPoints = nValue;}

	unsigned int GetMaxMp() { return mMaxMp;}
	void SetMaxMp( unsigned int nValue) { mMaxMp = nValue; }

	unsigned int GetMaxHp() { return mMaxHp;}
	void SetMaxHp( unsigned int nValue) { mMaxHp = nValue; }

	int GetIncMp() { return mIncMp;}
	void SetIncMp( int nValue) { mIncMp = nValue;}

	int GetIncHp() { return mIncHp;}
	void SetIncHp( int nValue) { mIncHp = nValue;}

	unsigned int GetAddKeenpoints() { return mAddKeenpoints;}
	void SetAddKeenpoints( unsigned int nValue) { mAddKeenpoints = nValue;}

	unsigned int GetAddCalmpoints() { return mAddCalmpoints;}
	void SetAddCalmpoints( unsigned int nValue) { mAddCalmpoints = nValue;}

	unsigned int GetAddMagicpoints() { return mAddMagicpoints;}
	void SetAddMagicpoints( unsigned int nValue) { mAddMagicpoints = nValue;}

	unsigned int GetAddBierpoints() { return mAddBierpoints;}
	void SetAddBierpoints( unsigned int nValue) { mAddBierpoints = nValue;}

	unsigned int GetAddImmortalpoints() { return mAddImmortalpoints;}
	void SetAddImmortalpoints( unsigned int nValue) { mAddImmortalpoints = nValue;}

	int GetProJudgeFlag() { return mProJudgeFlag;}
	void SetProJudgeFlag( unsigned int nValue) { mProJudgeFlag = nValue;}

	int GetLeftPointsLastLevel() { return mLeftPointsLastLevel;}
	void SetLeftPointsLastLevel( int nValue) { mLeftPointsLastLevel = nValue;}

	int GetNewAddPoints() { return mNewAddPoints;}
	void SetNewAddPoints( int nValue) { mNewAddPoints = nValue;}

	int GetNewAddSkillPoints() { return mNewAddSkillPoints;}
	void SetNewAddSkillPoints( int nValue) { mNewAddSkillPoints = nValue;}

	int GetStepClearState() { return mStepClearState;}
	void SetStepClearState( int nValue) { mStepClearState = nValue;}

	int GetLastAddedSkillBoxIndex() { return mLastAddedSkillBoxIndex;}
	void SetLastAddedSkillBoxIndex( int nValue) { mLastAddedSkillBoxIndex = nValue;}

	int GetLastLevelProcessVal() { return mLastLevelProcessVal; }
	void SetLastLevelProcessVal( int nValue ) { mLastLevelProcessVal = nValue; } 

	int GetNextActBoxIndex() { return mNextActBoxIndex;}
	void SetNextActBoxIndex( int nValue) { mNextActBoxIndex = nValue;}

	int GetLastLevelNextActBox() { return mLastLevelNextActBox;}
	void SetLastLevelNextActBox( int nValue) { mLastLevelNextActBox = nValue;}

	int  GetCurQL() { return  mQilinID; }
	void SetCurQL( int nID ) { mQilinID=nID; }

	void SetPlayerAddPointLastLevel( int nVal, int i )
	{
		mPlayerAddPointLastLevel[i] = nVal;
	}

	int GetOwner() { return mOwnerEntityID;}

	// 初始化MP HP
	void InitMpHp( unsigned int nMp, unsigned int nHp ) { mMp = nMp; mHp = nHp; }

	CSkillList* GetMWSkillList() { return &mMWSkillList; }
public:
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber ) ;
	virtual void GetItemBinary2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemBindStone
// 绑定石
// ********************************************************************** //
class CItemBindStone : public CItemObject
{
public:	
	CItemBindStone()
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
	virtual int Initialize()
	{
		return	CItemObject::Initialize( );
	}
	virtual int Resume() 
	{
		return CItemObject::Resume( );
	}

	virtual ~CItemBindStone(){}
	
	static CItemObject * Creator(unsigned int vItemID , unsigned int vNumber  );

	static CItemObject* BinaryClone2( PBItemObject*& pGene );
	virtual void GetItemBinary2( PBItemObject*& pGene ) { return; }
};
// ********************************************************************** //
// CItemShout
// 喊话道具
// ********************************************************************** //
class CItemShout : public CItemObject
{
public:
	CItemShout()
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

	virtual int Initialize()
	{
		return	CItemObject::Initialize( );
	}
	
	virtual int Resume() 
	{
		return CItemObject::Resume( );
	}

    virtual ~CItemShout(){}
	
	static CItemObject * Creator(unsigned int vItemID , unsigned int vNumber  );
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
	virtual void GetItemBinary2( PBItemObject*& pGene ) { return; }
};
// ********************************************************************** //
// CItemAutoShout
// 自动喊话道具
// ********************************************************************** //
class CItemAutoShout : public CItemObject
{
public:
	CItemAutoShout()
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

	virtual int Initialize()
	{
		return	CItemObject::Initialize( );
	}
	
	virtual int Resume() 
	{
		return CItemObject::Resume( );
	}

	virtual ~CItemAutoShout(){}
	
	static CItemObject * Creator(unsigned int vItemID , unsigned int vNumber  );
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
	virtual void GetItemBinary2( PBItemObject*& pGene ) { return; }
};

// ********************************************************************** //
// CItemReturn
// 回城道具
// ********************************************************************** //

class CItemReturn : public CItemObject
{
public:
	//TODO
	CItemReturn( )
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

	virtual ~CItemReturn( ) {}
	
	virtual int Initailize()
	{
		return	CItemObject::Initialize( );
	}
	
	virtual int Resume() 
	{
		return CItemObject::Resume( );
	}


	
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );
	virtual void GetItemBinary2( PBItemObject*& pGene )	{ return; }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};


// ********************************************************************** //
// CItemSkillBook
// 法宝技能书道具
// ********************************************************************** //
class CItemSkillBook : public CItemObject
{
public:
	//TODO
	CItemSkillBook( )
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

	virtual ~CItemSkillBook( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene )	{	}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};



// ********************************************************************** //
// CItemFuncItem
// 功能类道具
// ********************************************************************** //
class CItemFuncItem : public CItemObject
{
public:
	//TODO
	CItemFuncItem( )
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

	virtual ~CItemFuncItem( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene )	{	}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};


// ********************************************************************** //
// CItemTeleport
// 传送符道具
// ********************************************************************** //

class CItemTeleport : public CItemObject
{
private:
	// 记录存储的位置
	unsigned short		mLineID;	
	unsigned short		mMapID;
	int					mPosX;
	int					mPosY;

public:
	//TODO
	CItemTeleport( )
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

	virtual ~CItemTeleport() {}

	virtual int Initialize ();
	

	virtual int Resume(){ return CItemObject::Resume();	}

	inline unsigned short GetLineID() { return mLineID; }
	inline void SetLineID( unsigned short unLineID ) { mLineID = unLineID; }

	inline unsigned short GetMapID() { return mMapID; }
	inline void SetMapID( unsigned short unMapID ) { mMapID = unMapID; }

	inline int GetPosX() { return mPosX; }
	inline void SetPosX( int nPosX ) { mPosX = nPosX; }

	inline int GetPosY() { return mPosY; }
	inline void SetPosY( int nPosY ) { mPosY = nPosY;}

public:
	
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	/*{ return new CItemTeleport; }*/
	virtual void GetItemBinary2( PBItemObject*& pGene );
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemQuestPaper
// 题卷
// ********************************************************************** //
class CItemQuestPaper: public CItemObject
{	
public:
	CItemQuestPaper()
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

	virtual int Initialize()
	{
		return	CItemObject::Initialize( );
	}

	virtual int Resume() 
	{
		return CItemObject::Resume( );
	}

	virtual ~CItemQuestPaper(){}

	static CItemObject * Creator( unsigned int vItemID , unsigned int vNumber  );
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
	virtual void GetItemBinary2( PBItemObject*& pGene ) { return; }	
};

// ********************************************************************** //
// CItemEquipUpg
// 装备升级道具
// ********************************************************************** //

class CItemEquipUpg : public CItemObject
{
public:
	//TODO
	CItemEquipUpg( )
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
	virtual ~CItemEquipUpg( ) {}

	virtual int Initialize(){ return CItemObject::Initialize();	}

	virtual int Resume(){ return CItemObject::Resume();	}

	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene )	{ return; }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemRelife
// 复活道具
// ********************************************************************** //

class CItemRelive : public CItemObject
{
private:
	int		mCooldown;	// 当前的冷却时间


public:
	//TODO
	CItemRelive( )
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

	virtual ~CItemRelive( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		mCooldown = 0;
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

	inline int GetCooldown( ) { return mCooldown; }
	inline void SetCooldown( int nCD ) { mCooldown = nCD; }

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene )	{	pGene->set_cooldown( mCooldown );	}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemSkill
// 使用技能类道具
// ********************************************************************** //

class CItemSkill : public CItemObject
{
private:
	int		mCooldown;	// 当前的冷却时间

public:

	CItemSkill( ) 
	{
		if ( CObj::msCreateMode )
		{
			Initailize();
		} 
		else
		{
			Resume();
		}
	}

	virtual ~CItemSkill( ) {}

	virtual int Initailize()
	{
		CItemObject::Initialize( );
		mCooldown = 0;
		return 0;
	}

	virtual int Resume()
	{
		return CItemObject::Resume( );
	}

	inline int GetCooldown( ) { return mCooldown; }
	inline void SetCooldown( int nCD ) { mCooldown = nCD; }

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene )	{ pGene->set_cooldown( mCooldown ); }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemBook
// 技能书道具
// ********************************************************************** //

class CItemBook : public CItemObject
{
public:
	//TODO:
	CItemBook( ) 
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
	virtual ~CItemBook( ) {}

	virtual int Initialize() 
	{
		return  CItemObject::Initialize();		
	}
	
	virtual int Resume() 
	{
		return CItemObject::Resume( );
	}

	
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );
	virtual void GetItemBinary2( PBItemObject*& pGene ) { return; }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemNote
// 符道具
// ********************************************************************** //

class CItemNote : public CItemObject
{
public:

	//TODO
	CItemNote( )
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
	virtual ~CItemNote( ) {}

	virtual int Initialize()
	{
		return CItemObject::Initialize();
	}

	virtual int Resume()
	{
		return CItemObject::Resume();
	}

	
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );
	virtual void GetItemBinary2( PBItemObject*& pGene ) { return; }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemTuff
// 原料道具
// ********************************************************************** //

class CItemStuff : public CItemObject
{
public:
	
	CItemStuff( )
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
	virtual ~CItemStuff( ) {}

	virtual int Initialize ()
	{
		return CItemObject::Initialize();
	}

	int Resume()
	{
		return CItemObject::Resume();
	}

	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );
	virtual void GetItemBinary2( PBItemObject*& pGene ) { return; }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemExp
// 经验加倍道具
// ********************************************************************** //

class CItemExp : public CItemObject
{
public:
	
	CItemExp( )
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
	virtual ~CItemExp( ) {}
	
	virtual int Initialize( )
	{
		return CItemObject::Initialize( );
	}

	virtual int Resume( )
	{
		return CItemObject::Resume( );
	}

	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );
	virtual void GetItemBinary2( PBItemObject*& pGene ) { return; }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemPhysic
// 药品类道具
// ********************************************************************** //

class CItemPhysic : public CItemObject
{
private:
	unsigned int	mRemainHP;
	unsigned int	mRemainMP;
	unsigned int	mRemainAP;
	int				mCooldown;	// 当前的冷却时间

public:

	CItemPhysic( )
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
	virtual ~CItemPhysic( ) {}

	CItemPhysic( int ItemID  ) : CItemObject( ItemID ), mRemainHP( 0 ), mRemainMP( 0 ), mRemainAP( 0 ), mCooldown( 0 )
	{
	}

	virtual int Initialize()
	{
		CItemObject::Initialize( );
		mRemainHP = 0;
		mRemainMP = 0;
		mRemainAP = 0;
		mCooldown = 0;
		return 0;
	}

	virtual int Resume( )
	{
		return CItemObject::Resume( );
	}

	inline unsigned int GetRemainHP() { return mRemainHP; }
	inline void SetRemainHP( unsigned int nRemainHP ) { mRemainHP = nRemainHP; }

	inline unsigned int GetRemainMP() { return mRemainMP; }
	inline void SetRemainMP( unsigned int nRemainMP ) { mRemainMP = nRemainMP; }

	inline  unsigned int GetRemainAP() { return mRemainAP; }
	inline void SetRemainAP( unsigned int nRemainAP ) { mRemainAP = nRemainAP; }

	inline int GetCooldown() { return mCooldown; }
	inline void SetCooldown( int nCD ) { mCooldown = nCD; }

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	/*{ return new CItemPhysic; }*/
	virtual void GetItemBinary2( PBItemObject*& pGene );
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemHorse
// 药品类道具
// ********************************************************************** //

class CItemHorse : public CItemObject
{
private:
	int				mCooldown;	// 当前的冷却时间

public:

	CItemHorse( )
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
	virtual ~CItemHorse( ) {}

	CItemHorse( int ItemID  ) : CItemObject( ItemID ), mCooldown( 0 )
	{
	}

	virtual int Initailize()
	{
		CItemObject::Initialize( );
		mCooldown = 0;
		return 0;
	}

	virtual int Resume( )
	{
		return CItemObject::Resume( );
	}

	inline int GetCooldown( ) { return mCooldown; }
	inline void SetCooldown( int nCD ) { mCooldown = nCD; }

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );
	virtual void GetItemBinary2( PBItemObject*& pGene )
	{
		pGene->set_cooldown( mCooldown );
	}

	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemHorse
// 药品类道具
// ********************************************************************** //
class CItemKitBag : public CItemObject
{
public:
	CItemKitBag()
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
	CItemKitBag( int nItemID ):CItemObject( nItemID ){  }
	virtual ~CItemKitBag( ){ }

private:

	//int mIndexNumber;	// 扩展包格子数量

public:
	virtual int Initialize( ){  return 0; }
	virtual int Resume( ){  return 0; }
public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );
	virtual void GetItemBinary2( PBItemObject*& pKitBag ); 
	static CItemObject* BinaryClone2( PBItemObject*& pKitBag );
};

// ********************************************************************** //
// CItemTask
// 任务类道具
// ********************************************************************** //
class CItemTask : public CItemObject
{
private:

	int				mCooldown;	// 当前的冷却时间

public:

	CItemTask( ) 
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

	virtual ~CItemTask() {}

	virtual int Initialize()
	{
		CItemObject::Initialize( );
		mCooldown = 0;
		return 0;
	}
	
	virtual int Resume(){ return CItemObject::Resume( ); }

	inline int GetCooldown( ) { return mCooldown; }
	inline void SetCooldown( int nCD ) { mCooldown = nCD; }

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	/*{ return new CItemTask; }*/
	virtual void GetItemBinary2( PBItemObject*& pGene )	{	pGene->set_cooldown( mCooldown ); }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};



// ********************************************************************** //
// CItemSpecialTask
// 特殊任务物品
// ********************************************************************** //
class CItemSpecialTask : public CItemObject
{
private:

	int				mCooldown;	// 当前的冷却时间

public:

	CItemSpecialTask( ) 
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

	virtual ~CItemSpecialTask() {}

	virtual int Initialize()
	{
		CItemObject::Initialize( );
		mCooldown = 0;
		return 0;
	}

	virtual int Resume()
	{
		return CItemObject::Resume( );
	}

	inline int GetCooldown( ) { return mCooldown; }
	inline void SetCooldown( int nCD ) { mCooldown = nCD; }

public:
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene ) ;
};




// ********************************************************************** //
// CItemRune
// 符文类道具
// ********************************************************************** //
class CItemRune : public CItemObject
{
public:
	CItemRune( )
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

	virtual ~CItemRune() {}

	virtual int Initialize()
	{
		CItemObject::Initialize( );
		return 0;
	}

	virtual int Resume()
	{
		return CItemObject::Resume( );
	}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );
	virtual void GetItemBinary2( PBItemObject*& pGene ) { return; }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemJewel
// 珠宝类道具
// ********************************************************************** //
class CItemJewel : public CItemObject
{
public:
	CItemJewel( )
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

	virtual ~CItemJewel() {}

	virtual int Initialize()
	{
		CItemObject::Initialize( );
		return 0;
	}

	virtual int Resume()
	{
		return CItemObject::Resume( );
	}

public:
	
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );
	virtual void GetItemBinary2( PBItemObject*& pGene ) { return; }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemYuanBug
// 元气虫类道具
// ********************************************************************** //
class CItemYuanBug : public CItemObject
{
private:
	// 当前存储的元气
	int		mCurrent;
public:
	CItemYuanBug( )
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

	virtual ~CItemYuanBug() {}

	virtual int Initialize()
	{
		CItemObject::Initialize( );
		mCurrent = 0;
		return 0;
	}

	virtual int Resume()
	{
		return CItemObject::Resume( );
	}
	

	inline int GetCurrent( ) { return mCurrent; }
	inline void SetCurrent( int nCur ) { mCurrent = nCur; }

	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );
	virtual void GetItemBinary2( PBItemObject*& pGene )
	{
		pGene->mutable_itemcontent()->set_current( mCurrent );
	}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );



};

// ********************************************************************** //
// CItemYuanEgg
// 元气蛋类道具
// ********************************************************************** //
class CItemYuanEgg : public CItemObject
{
public:
	CItemYuanEgg( )
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

	virtual ~CItemYuanEgg() {}

	virtual int Initialize()
	{
		CItemObject::Initialize( );
		return 0;
	}

	virtual int Resume()
	{
		return CItemObject::Resume( );
	}

	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );
	virtual void GetItemBinary2( PBItemObject*& pGene ) { return; }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemScroll
// 仙轴道具
// ********************************************************************** //
class CItemScroll : public CItemObject
{
private:
	int		mActivated;	// 是否激活


public:
	//TODO
	CItemScroll( )
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

	virtual ~CItemScroll( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		mActivated = 0;
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

	inline int GetActivated() { return mActivated; }
	inline void GetActivated( int nActivate ) { mActivated = nActivate; }

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene )	{	pGene->set_cooldown( mActivated );	}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};


// ********************************************************************** //
// CItemCard
// 卡片道具
// ********************************************************************** //
class CItemCard : public CItemObject
{

public:
	//TODO
	CItemCard( )
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

	virtual ~CItemCard( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene )	{	}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};



// ********************************************************************** //
// CItemCall
// 召唤类道具
// ********************************************************************** //
class CItemCall : public CItemObject
{
public:
	//TODO
	CItemCall( )
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

	virtual ~CItemCall( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene )	{	}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemReturnPaper
// 返回卷道具
// ********************************************************************** //
class CItemReturnPaper : public CItemObject
{

public:
	//TODO
	CItemReturnPaper( )
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

	virtual ~CItemReturnPaper( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene )	{	}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};


// ********************************************************************** //
// CItemGiftBag
// 礼包类道具
// ********************************************************************** //
class CItemGiftBag : public CItemObject
{
public:
	//TODO
	CItemGiftBag( )
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

	virtual ~CItemGiftBag( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene )	{	}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemTeleSymbol
// 传送符道具
// ********************************************************************** //
class CItemTeleSymbol : public CItemObject
{
public:
	//TODO
	CItemTeleSymbol( )
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

	virtual ~CItemTeleSymbol( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene )	{	}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemStove
// 炉石道具
// ********************************************************************** //
class CItemStove : public CItemObject
{
private:
	int		mCoolDown;		// 当前剩余冷却时间

public:
	//TODO
	CItemStove( )
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

	virtual ~CItemStove( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		mCoolDown = 0;
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

	inline int GetCooldown( ) { return mCoolDown; }
	inline void SetCooldown( int nCD ) { mCoolDown = nCD; }

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene );
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemMineMap
// 藏宝图
// ********************************************************************** //
class CItemMineMap : public CItemObject
{
private:
	// 记录存储的位置
	unsigned short		mLineID;	
	unsigned short		mMapID;
	int					mPosX;
	int					mPosY;

public:
	//TODO
	CItemMineMap( )
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

	virtual ~CItemMineMap( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		mLineID = 0;
		mMapID = 0;
		mPosY = 0;
		mPosX = 0;
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

	inline unsigned short GetLineID() { return mLineID; }
	inline void SetLineID( unsigned short unLineID ) { mLineID = unLineID; }

	inline unsigned short GetMapID() { return mMapID; }
	inline void SetMapID( unsigned short unMapID ) { mMapID = unMapID; }

	inline int GetPosX() { return mPosX; }
	inline void SetPosX( int nPosX ) { mPosX = nPosX; }

	inline int GetPosY() { return mPosY; }
	inline void SetPosY( int nPosY ) { mPosY = nPosY;}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene );
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemToken
// 令牌
// ********************************************************************** //
class CItemToken : public CItemObject
{
private:
	// 记录存储的位置
	unsigned short		mLineID;	
	unsigned short		mMapID;
	int					mPosX;
	int					mPosY;
public:
	//TODO
	CItemToken( )
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

	virtual ~CItemToken( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		mLineID = 0;
		mMapID = 0;
		mPosY = 0;
		mPosX = 0;
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

	inline unsigned short GetLineID() { return mLineID; }
	inline void SetLineID( unsigned short unLineID ) { mLineID = unLineID; }

	inline unsigned short GetMapID() { return mMapID; }
	inline void SetMapID( unsigned short unMapID ) { mMapID = unMapID; }

	inline int GetPosX() { return mPosX; }
	inline void SetPosX( int nPosX ) { mPosX = nPosX; }

	inline int GetPosY() { return mPosY; }
	inline void SetPosY( int nPosY ) { mPosY = nPosY;}


public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene );
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemMagicStone
// 附魔石
// ********************************************************************** //
class  CItemMagicStone : public CItemObject
{
public:
	CItemMagicStone()
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
	~CItemMagicStone( ){ }
	
public:
	int Initialize( ){ return 0; }
	int Resume( ){ return 0; }
	
public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene ){ }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );	
};

// ********************************************************************** //
// CItemLifeSkillScroll
// 生活技能卷轴
// ********************************************************************** //
class  CItemLifeSkillScroll : public CItemObject
{
public:
	CItemLifeSkillScroll()
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
	~CItemLifeSkillScroll( ){ }

public:
	int Initialize( ){ return 0; }
	int Resume( ){ return 0; }

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene ){ }
	static CItemObject* BinaryClone2( PBItemObject*& pGene );	
};

// ********************************************************************** //
// CItemRemove
// 摘除符
// ********************************************************************** //
class CItemRemove : public CItemObject
{		
public:
	//TODO
	CItemRemove( )
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

	virtual ~CItemRemove( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene ){}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemChange
// 转换符
// ********************************************************************** //
class CItemChange : public CItemObject
{		
public:
	//TODO
	CItemChange( )
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

	virtual ~CItemChange( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene ){}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemChange
// 转换符
// ********************************************************************** //
class CItemRedPaper : public CItemObject
{  
public:
	//TODO
	CItemRedPaper( )
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

	virtual ~CItemRedPaper( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}
	
public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene ){}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};


// ********************************************************************** //
// CItemRunTime
// ********************************************************************** //

class CItemRunTime
{
	//typedef CItemObject* ( *ItemCloneFromBinary )( char*& pGene );
	//typedef CItemObject* ( *ItemCloneFromString )( char* pString );
	typedef CItemObject* ( *ItemCreator )( unsigned int vItemID, unsigned int vNumber );

	typedef CItemObject* ( *ItemCloneFromBinary2 )( PBItemObject*& pGene );

public:
	unsigned short	mItemType;
	//ItemCloneFromBinary		mpItemBinaryClone;		// 通过基因克隆
	//ItemCloneFromString		mpItemStringClone;		// 通过字符串克隆
	ItemCreator				mpItemCreator;			// 建立一个新道具

	ItemCloneFromBinary2	mpItemBinaryClone2;		// 通过基因克隆2

public:
	// 道具克隆表
	static std::vector< CItemRunTime >* spItemTypes;

	static void RegisterClone( unsigned short vItemType, ItemCloneFromBinary2 pBinaryClone2, ItemCreator pCreator )
		{ spItemTypes->push_back( CItemRunTime( vItemType,  pBinaryClone2, pCreator ) ); }

	static void Initialize( );

	static void Finialize( ){	delete spItemTypes;	}

public:
	CItemRunTime( unsigned short vItemType,  ItemCloneFromBinary2 pBinaryClone2, ItemCreator pCreator ) : 
	  mItemType( vItemType ),  mpItemCreator( pCreator ),mpItemBinaryClone2( pBinaryClone2 )
	{
	}

public:
	static CItemObject* CloneFromBinary2( PBItemObject*& pGene );
	static CItemObject* Clone( unsigned int vItemID, unsigned int vNumber );
	
};	

// ********************************************************************** //
// CItemVip
// 会员道具
// ********************************************************************** //
class CItemVip : public CItemObject
{  
public:
	//TODO
	CItemVip( )
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

	virtual ~CItemVip( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene ){}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemClean
// 洗点道具
// ********************************************************************** //
class CItemClean : public CItemObject
{  
public:
	//TODO
	CItemClean( )
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

	virtual ~CItemClean( ) {}

	virtual int Initialize()
	{
		CItemObject::Initialize();
		return 0;
	}

	virtual int Resume()
	{
		return 	CItemObject::Resume();
	}

public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene ){}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemLeagueBag
// 家族军团特供物品
// ********************************************************************** //
class  CItemLeagueBag : public CItemObject 
{
public:
	CItemLeagueBag()
	{
		if ( CObj::msCreateMode )
		{
			Initailize();	
		}
		else
		{
			Resume();
		}
	}
	
	~CItemLeagueBag( ){ }
	virtual int Initailize()
	{
		return CItemObject::Initialize();		
	}
	
	virtual int Resume( )
	{
		return CItemObject::Resume();
	}
public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene ){}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
};

// ********************************************************************** //
// CItemInvitation
// 结婚请帖
// ********************************************************************** //
class CItemInvitation : public CItemObject
{
public:
	CItemInvitation()
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
	~CItemInvitation(){ }
	virtual int Initialize()
	{
		memset( mGroomName, 0, sizeof( mGroomName ) );
		memset( mBrideName, 0, sizeof( mBrideName ) );
		mWeddingTime = 0;
		mWeddingLength = 0;
		return CItemObject::Initialize();	
	}
	virtual int Resume()
	{
		return CItemObject::Resume();
	}
public:
	char mGroomName[NAME_LENGTH];	// 新郎名称
	char mBrideName[NAME_LENGTH];	// 新娘姓名
	int  mWeddingTime;				// 婚期
	int  mWeddingLength;			// 婚礼时间
public:
	
	// 设置&获取新郎的名称
	void SetGroomName( const char *pName )
	{
		strncpy( mGroomName, pName, NAME_LENGTH - 1 );				
		mGroomName[NAME_LENGTH-1] = '\0';
	}
	const char * GetGroomName(){ return mGroomName; }
	
	// 设置&获取新娘的名称
	void SetBrideName( const  char *pName )
	{
		strncpy( mBrideName, pName, NAME_LENGTH - 1 );
		mBrideName[NAME_LENGTH-1] = '\0';
	}	
	const char *GetBrideName(){ return mBrideName; }
	
	// 设置&获取婚期
	void SetWeddingTime( int nTime ){ mWeddingTime = nTime; }
	int	GetWeddingTime( ){ return mWeddingTime; }
	
	// 设置&获取婚礼时间
	void SetWeddingTimeLength( int nTimeLength ){ mWeddingLength = nTimeLength; }
	int	GetWeddingTimeLength(){ return mWeddingLength; }
	
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene );
	static CItemObject* BinaryClone2( PBItemObject*& pGene );
	
};


// ********************************************************************** //
// CItemInvitation
// 结婚请帖
// ********************************************************************** //
class CItemSysInvitation : public CItemObject
{
public:
	CItemSysInvitation()
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
	~CItemSysInvitation(){ }
public:
	virtual int Initialize()
	{
		return 0;
	}
	virtual int Resume()
	{
		return 0;
	}
public:	
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene ){}
	static CItemObject* BinaryClone2( PBItemObject*& pGene );	
};

// ********************************************************************** //
// CItemBlackPills
// 黑狗丸
// ********************************************************************** //
class CItemBlackPills : public CItemObject
{
public:
	CItemBlackPills()
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
	~CItemBlackPills(){}
public:	
	virtual int Initialize()
	{
		mLeftTime = 0;
		return 0;
	}
	virtual int Resume()
	{
		return 0;
	} 
public:
	int GetLeftTime(){ return mLeftTime; }
	void SetLeftTime( int nTime ){ mLeftTime = nTime; }
public:
	int mLeftTime;
public:	
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene );
	static CItemObject* BinaryClone2( PBItemObject*& pGene );		 	
};	   

// ********************************************************************** //
// CItemExpContainer
// 修炼经验丹
// ********************************************************************** //
class CItemExpContainer : public CItemObject
{
public:
	CItemExpContainer()
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
	~CItemExpContainer() {}
public:
	virtual int Initialize()
	{
		mExp = 0;
		return 0;				
	}
	
	virtual int Resume()
	{
		return 0;			
	}

public:	   	
	unsigned long long GetExp() { return mExp; }
	void SetExp( unsigned long long nExp ) { mExp = nExp; }
private:
	unsigned long long mExp;
public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene );
	static CItemObject* BinaryClone2( PBItemObject*& pGene );		 
};

// ********************************************************************** //
// CItemExpContainer
// 修炼经验丹
// ********************************************************************** //
class CItemExpPills : public CItemObject
{
public:
	CItemExpPills()
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
	~CItemExpPills(){}
public:	
	virtual int Initialize() 
	{
		return 0;
	}
	virtual int Resume()
	{
		return 0;
	}
public:
	static CItemObject* Creator( unsigned int vItemID, unsigned int vNumber );	
	virtual void GetItemBinary2( PBItemObject*& pGene );
	static CItemObject* BinaryClone2( PBItemObject*& pGene );		 	
};

// ********************************************************************** //
// CItemList
// ********************************************************************** //
template< int NUMBER >
class CItemList 
{
private:

	int mpItems[ NUMBER ];

public:

	virtual int Initialize()
	{
		for( int i = 0; i < NUMBER; i ++ )
		{
			mpItems[ i ] = INVALID_OBJ_ID;
		}
		return 1;
	}

	virtual int Resume(){	return 1;	}

public:

	CItemList( )
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

	virtual ~CItemList( )
	{
	}

public:

	CItemList( const CItemList& rItemList )
	{
		for ( int i = 0; i < NUMBER; i ++ )
		{
			mpItems[ i ] = INVALID_OBJ_ID;
		}
	}
	

	CItemList& operator = ( const CItemList& rItemList )
	{
		if ( this == &rItemList )
			return *this;

		for ( int i = 0; i < NUMBER; i ++ )
		{
			mpItems[ i ] = INVALID_OBJ_ID;
		}

		for ( int i = 0; i < NUMBER; i ++ )
		{
			if ( rItemList.mpItems[ i ] == INVALID_OBJ_ID )
				continue;
		}

		return *this;
	}

public:
	// 安全起见，只返回指针
	int& operator [] ( int vIndex )
	{
		if( vIndex < 0 || vIndex >= NUMBER )
		{
			// 如果到了这里问题比较严重
			LOG_FATAL( "pro", "[%s:%d] index %d over flow %d", __FUNCTION__, __LINE__, vIndex, NUMBER );
			return mpItems[0];
		}

		return mpItems[ vIndex ];
	}

	CItemObject* GetItemObjPtr( int vIndex )
	{
		if( vIndex < 0 || vIndex >= NUMBER )
		{
			LOG_ERROR( "pro", "[%s:%d] index %d over flow %d", __FUNCTION__, __LINE__, vIndex, NUMBER );
			return NULL ;
		}
		
		if( mpItems[ vIndex ] != INVALID_OBJ_ID )
		{
			return (CItemObject*) CSceneObjManager::GetSingletonPtr()->GetObject ( mpItems[ vIndex ] );	
		}
		else
		{
			return NULL;
		}
	}

	bool  ChangeItemByIndex(int vIndex, int itemid,int isBind)
	{
		if( vIndex < 0 || vIndex >= NUMBER )
		{
			LOG_ERROR( "pro", "[%s:%d] index %d over flow %d", __FUNCTION__, __LINE__, vIndex, NUMBER );
			return  FALSE;
		}

		if( mpItems[ vIndex ] == INVALID_OBJ_ID )
		{
			return FALSE;	
		}
		CItemObject* obj = (CItemObject*) CSceneObjManager::GetSingletonPtr()->GetObject ( mpItems[ vIndex ] );
		if (obj == NULL)
		{
			return FALSE;
		}
		obj->SetItemID(itemid) ;
		if (isBind == ITEM_BINDSTATUS_BINDED)
		{
			obj->SetBindStatus(ITEM_BINDSTATUS_BINDED);
		}
		return  TRUE;
	}

	void Clear( )
	{
		for ( int i = 0; i < NUMBER; i ++ )
		{
			if( mpItems[ i ] == INVALID_OBJ_ID )
			{
				continue;
			}

			CSceneObjManager::GetSingletonPtr( )->DestroyObject( mpItems[ i ] ) ;
			mpItems[ i ] = INVALID_OBJ_ID;
		}
	}

	void GetTlvBuffer2( PBItemBox* pBuffer, int tStart = 0, int tEnd = NUMBER )
	{
		tEnd = std::min( NUMBER, tEnd );
		int tCount = 0;
		for ( int i = tStart; i < tEnd; i++ )
		{
			if ( INVALID_OBJ_ID == mpItems[ i ] )
			{
				continue;
			}
		
			CItemObject* pItemObj = (CItemObject*) CSceneObjManager::GetSingletonPtr()->GetObject( mpItems[ i ] );
			if( pItemObj == NULL)
			{
				LOG_ERROR( "pro", "can't find (id:%d, index:%d) object in itembox ", mpItems[i], i );
				continue;
			}

			::PBItemObject* tpPBItemObject = pBuffer->add_itemobjects();
			tpPBItemObject->set_itemid( pItemObj->GetItemID() );
			tpPBItemObject->set_number( pItemObj->GetItemNum() );
			if( pItemObj->GetBindStatus() != 0 )
				tpPBItemObject->set_bindstatus( pItemObj->GetBindStatus() );
			if( pItemObj->GetUsedTimes() != 0 )
				tpPBItemObject->set_usedtimes( pItemObj->GetUsedTimes() );

			if (pItemObj->GetGUID() != 0)
			{
				tpPBItemObject->set_objguid(pItemObj->GetGUID());
			}

			if (pItemObj->GetPurchaseID() != 0)
			{
				tpPBItemObject->set_purchaseid (pItemObj->GetPurchaseID());	
			}

			if (pItemObj->GetInvalidTime() != 0)
				tpPBItemObject->set_invalidtime(pItemObj->GetInvalidTime());			
	
			tpPBItemObject->set_index( i );
			pItemObj->GetItemBinary2( tpPBItemObject );
			tCount++;
		}
	}
	 

	bool CreateFromTlv2( PBItemBox* pBuffer )
	{
		int tCount = pBuffer->itemobjects_size();
		for ( int i = 0; i < tCount; i++ )
		{
			::PBItemObject* tpPBItemObject = pBuffer->mutable_itemobjects( i );

			CItemObject* tpItem = CItemRunTime::CloneFromBinary2( tpPBItemObject );
			if( tpItem == NULL )
			{
				LOG_ERROR( "pro", "Item Clone fuction didn't register. item id %d", tpPBItemObject->itemid() );
				return false;
			}

			unsigned int tIndex = tpPBItemObject->index( );

			if( tIndex > ARRAY_CNT(mpItems) || tIndex < 0 )
			{
				LOG_ERROR( "pro" ,"In CItemList::CreateFromTlv, invalide index %d", tIndex );
				continue;
			}

			mpItems[ tIndex ] = tpItem->get_id();
		}

		return true;
	}
	// 设置某一格为空
	void SetIndexNull( int index ){	mpItems[ index ] = INVALID_OBJ_ID; }

	// 根据对象ID 返回索引
	int	GetIndexByObjID( int nObjID )
	{
		for ( int i = 0; i < NUMBER; i ++ )
		{
			if( mpItems[ i ] == nObjID )
			{
				return i;
			}
		}
		return -1;
	}

	virtual void DeleteUnusedItem( int vCharID );
};

// ********************************************************************** //
// CItemBox
// ********************************************************************** //




class CItemBox : public CItemList< BOX_LIMIT >
{
public:
	unsigned int mReleaseIndex;			// 当前释放到那个格了 
	int mKitbag[ MAX_KITBAG_NUM ];		// 背包栏位		
	int mKitBagStatus;					// 发送状态
	int	mKitBagIndex;					// 扩展包格数
	int mKitLocked[ MAX_KITBAG_NUM ];  // 栏位是否锁定
	typedef lk::hash_map< int, int, BOX_LIMIT > ITEMNUMBERPAIR;

	ITEMNUMBERPAIR mItemPair;			// 物品ID和数量的对应关系
public:
	CItemBox(  )
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

	virtual ~CItemBox( ){}

	virtual int Initialize( );
	
	virtual int Resume( )
	{
		return 0;
	}

public:
	unsigned int Space( );

	bool Lock( int vItemIndex, int vLock );

	bool HasItem( unsigned int vItemID, unsigned int vNumber, int vType = BIND_ALL );

	int HasItem( unsigned int vItemID, int vType = BIND_ALL );

	bool HasItem( unsigned int vIndex, unsigned int vItemID, unsigned int vNumber, int vType = BIND_ALL );

	int GetItemNum( unsigned int vIndex, unsigned int vItemID, int vType = BIND_ALL );

	bool Space( int* pItemID, int* pNumber, int vNumber );

	bool Space( unsigned int vItemID, unsigned int vNumber );	

	//int RemoveItemByID( unsigned int vItemID, unsigned int vNumber = ~0, int vType = BIND_ALL );

	int RemoveItem( unsigned int vIndex, unsigned int vNumber = ~0 );

	int InsertItem( CItemObject* pItem, int *vIndex, int *vNumber, int &vLen );
	
	int InsertItem( CItemObject* pItem, int vIndex );
	// 删除道具
	CItemObject* EraseItem( int vSrcIndex, int vNumber );

	//// 得到使用次数
	//int	GetItemUsedTimes( int vItemID );

	int Move( unsigned int vSrcIndex, unsigned int vDesIndex, CItemBox& rDesBox, unsigned int vNumber , int &rMoveNum );
	
	bool CreateFromTlv2( PBItemBox* pBuffer );
	void GetTlvBuffer2( PBItemBox* pBuffer, int tStart = 0, int tEnd = BOX_LIMIT );
	// 是否有某种类型的物品( 如果是功能性物品的话，nFuncType必须赋值, rIndex 记录最后一个物品的位置 )
	int HasItemOfCertainType( int nType, int &rNum, int &rIndex, int nFuncType = 0 );
	
	// 获取物品的类型
	int GetItemType( int vSrcInde, int &rType );

	int GetTlvSize2( );

	int	GetKitBagIDByIndex( int nIndex ); 
	int AddKitBag( int nIndex, int nNewKitBagID,  int &rOldKitBagObjID, int nNewKitBagObjID );
	
	// 取下背包
	int TakeKitBag( int nKitBagIndex, int &rKitBagObjID ); 

	// Set & Get KitBagStatus
	int		GetKitBagStatus(){ return mKitBagStatus; }
	void 	SetKitBagStatus( int nKitBagStatus ){ mKitBagStatus = nKitBagStatus; }

	// Set & Get ReleaseIndex
	int		GetReleaseIndex(){ return mReleaseIndex; }
	void 	SetReleaseIndex( int nReleaseIndex ){ mReleaseIndex = nReleaseIndex; }

	// Set & Get KitBagIndex
	int		GetKitBagIndex(){ return mKitBagIndex; }
	void 	SetKitBagIndex( int nKitBagIndex ){ mKitBagIndex = nKitBagIndex; }

	void    ResetPair(int nItemID, int nItemNum);
	int 	GetItemTotalCount( int nItemID);
	
	// 获取指定位置背包的格子上限
	int GetKitBagIndexLimit( int nKitBagIndex, int &rNum );
	
	// 获取指定索引所在的包裹类型
	int GetBagTypeByIndex( int nIndex, EBagType &rSlotType, int &rBagIndex );
	
	// 检查某个位置的背包已经到期
	bool CheckKitBagExpiredTime( int nKitBagIndex );
	
	// 获取指定栏位的背包物品
	CItemKitBag *GetKitBagItemByIndex( int nKitBagIndex );
	// 转换指定栏位背包物品
	bool ChangeKitBagItemByIndex(int index, int itemid,int isBind);
	
	// 检查索引是否有效
	bool CheckIndexValid( int nIndex, bool bExceptExpiredKitbag = false );
	
	// 获取职位位置背包的索取始末位置
	int GetKitBagIndexRange(  EBagType emSlotTyp, int nBagIndex, int &rBeginIndex, int &rEndIndex );
	
	// 获取指定位置包裹的物品的PB数据
	int GetItemInBag( EBagType emSlotType, int nBagIndex, PBItemBox &rItemBox );
	
	// 获取指定包的物品数量
	int GetItemCountInBag( EBagType emSlotType, int nBagIndex, int &rCount );	
	
	// 先清除背包道具，再清除物品道具
	void Clear( );	

	virtual void DeleteUnusedItem( int vCharID );
};



// 包裹工具类

class CItemBoxToolkit
{
public:
	CItemBox* mItemBox;

public:
	CItemBoxToolkit( CItemBox* pBox ) : mItemBox(pBox) { }
	~CItemBoxToolkit() {}

	// 计算指定索引上的特定类型物品个数
	unsigned int ComputeItemNum( int nIdx, unsigned int nItemID, int nType  );

	// 计算一段索引内的特定类型物品个数
	unsigned int ComputeItemNum( int nBegin, int nEnd, unsigned int nItemID,  int nType );

	// 计算整个包裹内特定类型物品个数
	unsigned int ComputeItemNum( unsigned int nItemID, int nType );

	//// 得到指定索引上物品的使用次数
	//int GetItemUsedNum( int nIdx, unsigned int nItemID );

	//// 得到指定一段索引内物品的使用次数
	//int GetItemUsedNum( int nBegin, int nEnd, unsigned int nItemID );

	//// 得到包裹内指定物品的使用次数
	//int GetItemUsedNum( unsigned int nItemID );

	// 计算指定索引上对指定物品的剩余堆叠数
	int GetLeftPileNum( int nIdx, unsigned int nItemID, bool bEmptyCounted = true );

	// 计算一段所以内对指定物品的剩余堆叠数
	int GetLeftPileNum( int nBegin, int nEnd, unsigned int nItemID, bool bEmptyCounted = true );

	// 得到包裹内对指定物品的剩余堆叠数
	int GetLeftPileNum( unsigned int nItemID, bool bEmptyCounted = true );

	// 得到指定索引上指定类型的道具个数
	int GetCertainTypeItemNum( int nIdx, int nType, int nFuncType );

	// 删除指定索引上指定数量的道具
	int RemoveItem( int nIdx, unsigned int nItemID, unsigned int nNumber, int nType );

	// 向指定索引插入指定数量道具
	int InsertItem( int nIdx, CItemObject* pItem, CTplItem *pTplItem );

	/**
	** 描述： 判断指定格子上装备的是否绑定到期
	** 返回： INVALID_OBJ_ID不是装备或者是装备但是未绑定或者装备绑定已经解除
	*/
	int CheckEquipBindTime( int nIdx );

	/* 描述：得到指定索引上装备的修理金额
	** 返回：<0 不是装备或者是装备但不需要修理; 0是损失耐久的装备但是修理金额为0; >0是损失耐久的装备并且修理金额大于0
	**/
	int GetEquipMendMoney( int nIdx );
};




// 新建仓库类，继承CItemBox，记录仓库背包扩充等功能
class CStorage : public CItemBox
{
public:			
	int		mBindMoney;						// 仓库存储的绑定金钱
	int		mMoney;							// 仓库存储的非绑定金钱
	bool	mChanged;						// 是否改变过
public:
	CStorage( )
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
	virtual ~CStorage(){}
public:
	virtual int Initialize( );	
	virtual int Resume( )
	{
		return 0;
	}
public:
	// 获取&设置绑定金钱
	//void SetBindMoney( int nBindMoney ){ mBindMoney = nBindMoney; }
	//int	 GetBindMoney( ){ return mBindMoney; }
	//
	//// 获取&设置金钱
	//void SetMoney( int nMoney ){ mMoney = nMoney; }
	//int  GetMoney( ){ return mMoney; }
		
	// 存储&取出金钱
	int StoreMoney( int nMoney, int nBindMoney );
	int TakeMoney( int nMoney, int nBindMoney );  	
	
	
public:

	bool CreateFromTlv2( PBItemBox* pBuffer );
	void GetTlvBuffer2( PBItemBox* pBuffer, int tStart = 0, int tEnd = BOX_LIMIT );	

	virtual void DeleteUnusedItem( int vCharID );
};

// 新建包裹类，继承CItemBox，记录宝石镶嵌等事件
class CBaggage : public CItemBox
{
public:
	int mUpgIndex1;					// 升级装备的索引
	int mUpgIndex2;					// 宝石的索引
	int mUpgIndex3;					// 保底符的索引
	int mRuneIndex1;				// 放符文时装备的索引
	int mRuneIndex2;				// 放符文时符文的索引
	int mJewelIndex0;				// 镶宝石时装备的索引
	int mJewelIndex[ JEWELNUM ];	// 镶宝石时宝石的索引
	int mYuanIndex0;				// 吃装备时元气虫的索引
	int mYuanIndex[ YUANNUM ];		// 吃装备时装备的索引
	int	mYuanNum;					// 吃装备时放上装备的数量
	int mSlotIndex1;				// 开槽时装备的索引
	int mSlotIndex2;				// 开槽时元气蛋的索引
	int mComposeIndex;				// 合成的索引
	int mEquipIndex;                // 待绑定或者解除绑定的装备的索引
	int mBindItemIndex;             // 绑定石的索引	
	int mEqtForReusmBindIndex;		// 待恢复手动绑定的物品的索引
	int mTaskLockIndex;				// 交任务锁定的包裹索引
	int mTeleSymbolIndex;			// 使用传送符锁定的包裹索引
	int mAddIndex1;					// 增加凹槽索引1
	int mAddIndex2;					// 增加凹槽索引2
	int mRemoveIndex1;				// 摘除宝石索引1
	int mRemoveIndex2;				// 摘除宝石索引2
	int mJudgeIndex1;				// 鉴定索引1
	int mJudgeIndex2;				// 鉴定索引2
	int mChangeIndex1;				// 转化索引1
	int mChangeIndex2;				// 转化索引2
	int mJewComIndex[ COMPOSENUM ]; // 宝石合成索引
	int	mJewComNum;					// 宝石合成数量
	int mMWIndex;					// 鉴定法宝的索引
	int mMWStoneIndex;				// 鉴定法宝属性的宝石索引
	int mMultiIndex;				// 复杂合成

public:
	CBaggage(  )
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

	virtual ~CBaggage( ){}

	virtual int Initialize( );

	virtual int Resume( )
	{
		return 0;
	}
	void SetTaskLockIndex( int vIndex ){ mTaskLockIndex = vIndex; }
	int  GetTaskLockIndex( ){ return mTaskLockIndex ; }

	void SetMWLockIndex( int vIndex ){ mMWIndex = vIndex; }
	int  GetMWLockIndex( ){ return mMWIndex ; }

	void SetMWStoneLockIndex( int vIndex ){ mMWStoneIndex = vIndex; }
	int  GetMWStoneLockIndex( ){ return mMWStoneIndex ; }

	void SetTeleSymbolIndex( int vIndex ) { mTeleSymbolIndex = vIndex; }
	int GetTeleSymbolIndex(){ return mTeleSymbolIndex; }

	int AddUpgItem( unsigned int vIndex );

	int DelUpgItem( unsigned int vIndex );

	int AddRuneItem( unsigned int vIndex );

	int DelRuneItem( unsigned int vIndex );

	int AddJewelItem( unsigned int vSrcIndex, unsigned int vDesIndex );

	int DelJewelItem( unsigned int vIndex );

	int AddSlotItem( unsigned int vIndex );

	int DelSlotItem( unsigned int vIndex );

	int AddYuanItem( unsigned int vIndex );

	int DelYuanItem( unsigned int vIndex );

	int AddComposeItem( unsigned int vIndex );

	int DelComposeItem( );

	int GetEquipIndex() {return mEquipIndex;}

	int AddAddItem( unsigned int vIndex );

	int DelAddItem( unsigned int vIndex );

	int AddRemoveItem( unsigned int vIndex );

	int DelRemoveItem( unsigned int vIndex );

	int AddJudgeItem( unsigned int vIndex );

	int DelJudgeItem( unsigned int vIndex );

	int AddChangeItem( unsigned int vIndex );

	int DelChangeItem( unsigned int vIndex );

	int AddJewComItem( unsigned int vIndex );

	int DelJewComItem( unsigned int vIndex );

	int AddMultiItem( unsigned int vIndex );

	int DelMultiItem( unsigned int vIndex );

	virtual void DeleteUnusedItem( int vCharID );
};


class CEquipment : public CItemList< EQUIP_LIMIT >
{
public:
	
	enum
	{
		MIN_PRO_AWARD_NUM = 4,
	};

	CEquipment(  )
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
	
	virtual int Initialize() { CItemList<EQUIP_LIMIT>::Initialize(); return 0;}
	virtual int Resume() { CItemList<EQUIP_LIMIT>::Resume();  return 0;}

	virtual ~CEquipment( ){}
	
	static int GetPartType( unsigned short vPart );
	
	unsigned short GetIncSkillLevel( unsigned short vSkillID );
	unsigned short GetValueByID( unsigned short vSkillID, CTemplateProperty::PROPERTY_FUNC vType );
	int	GetStateID( StateList& vBuffList,CTemplateProperty::PROPERTY_FUNC vType );
	
	// 装备某个装备
	int Equip( CPropertyPlayer* pProperty, CItemBox& rBaggage, unsigned int vSrcIndex, unsigned int &vDesIndex );

	// 卸下某个装备
	int UnEquip( CPropertyPlayer* pProperty, CItemBox& rBaggage, unsigned short vPart, unsigned int vIndex = ~0 );

	int GetSuit( unsigned short vFunc, unsigned short vType, unsigned short vSkillID = 0 );
	// 获得套装加成数值
	int	GetSuitInc( unsigned short vFunc );
	// 获得套装加成百分比
	int GetSuitPer( unsigned short vFunc );

	int GetFixNumber( CTemplateProperty::PROPERTY_FUNC vFunc, int vEquipProType = 0, CPropertyPlayer *pProperty = NULL );
	int GetPerNumber( CTemplateProperty::PROPERTY_FUNC vFunc );

	// 获得发光值
	void GetLightPoint( int &WeaponLevel, int &ChestLevel, int &WeaponJewel, int &ChestJewel );

	unsigned int GetFixPAttackUpper( CPropertyPlayer* pProperty );
	unsigned int GetFixPAttackLower( CPropertyPlayer* pProperty );
	unsigned int GetFixMAttackUpper( CPropertyPlayer* pProperty );
	unsigned int GetFixMAttackLower( CPropertyPlayer* pProperty );
	unsigned int GetFixPDefence( CPropertyPlayer* pProperty );
	unsigned int GetFixMDefence( CPropertyPlayer* pProperty );

	virtual void DeleteUnusedItem( int vCharID );
};




// ********************************************************************** //
// CShortcut
// ********************************************************************** //

class CShortcut
{
public:
	unsigned int	mShortcut;
	unsigned int	mType;
	unsigned int	mIndex;
	unsigned int    mLabelType;
	unsigned int	mLabelIndex;
	int mParam1;
public:
	//TODO
	CShortcut( ) {}

	CShortcut( unsigned int vShortcut , unsigned int vType , unsigned int vIndex, unsigned int vLabelType, unsigned int vLabelIndex, int nParam1 ) : mShortcut( vShortcut ),
		 mType( vType ), mIndex( vIndex ), mLabelType( vLabelType ), mLabelIndex( vLabelIndex ), mParam1( nParam1 )
	{
	}


};

// ********************************************************************** //
// CShortcutList
// ********************************************************************** //

class CShortcutList
{
public:
	unsigned short	mShortcutNumber;
	CShortcut		mShortcut[ 48 ];
	int				mHelpActionStatus[ MAX_HELPACTON_INT_NUM ];	// 新手指引动作状态 

public:

	CShortcutList( ) : mShortcutNumber( 0 )
	{
		memset( mHelpActionStatus, 0, sizeof( mHelpActionStatus ) );
	}

public:
	void Clear( )
	{
		mShortcutNumber = 0;
		::memset( mShortcut, 0, sizeof( mShortcut ) );
		::memset( mHelpActionStatus, 0, sizeof( mHelpActionStatus ) );
	}

	CShortcut GetShortcut( int vIndex );
	
	void SetShortcut( unsigned int vShortcut, unsigned int vType, unsigned int vIndex, unsigned int vLabelType, unsigned int vLabelIndex, int nParam1 );
	
	void CreateFromTlv2( PBShortcutList* pBuffer );	

	void GetTlvBuffer2( PBShortcutList* pBuffer );
	
	void SetHelpStatus( int Status, int Index );
	
	int  GetHelpStatus(  int Index ); 
	

};

// ********************************************************************** //
// CDamage
// ********************************************************************** //
class CDamage
{
public:
	int		mSrcEntityID;			// 造成伤害的实体ID
	int		mDamageValue;			// 伤害值累计
public:
	//TODO
	CDamage( )
	{
		if ( CObj::msCreateMode )
		{
			mSrcEntityID = INVALID_OBJ_ID;
			mDamageValue = 0;
		}
	}

	CDamage( int vSrcEntityID /*= 0*/, int vDamageValue /*= 0 */) : mSrcEntityID( vSrcEntityID ), mDamageValue( vDamageValue )
	{
	}

	~CDamage() {}
};

// ********************************************************************** //
// CDamageList
// ********************************************************************** //
class CDamageList
{
public:
	CDamage		mDamage[ DAMAGELIMIT ];		// 最大记录512个实体伤害

	int			mNumber;			// 伤害者的数量

public:

	CDamageList( )
	{
		if ( CObj::msCreateMode )
		{
			mNumber = 0;
		}
	}

	~CDamageList( ) {}

	float GetDamagePer( int vEntityID, int tMaxHP );
	

	int InsertDamage( const CDamage& rDamage );
	

	int RemoveDamage( int vEntityID );
	

	void ClearDamage( )
	{
		::memset( mDamage, 0, sizeof( CDamage ) * DAMAGELIMIT );
		mNumber = 0;
	}
};


// 用于记录生产的物品及定时
class CProduce
{
public:
	unsigned short mType1;
	unsigned short mType2;
	unsigned short mType3;
	unsigned short mLevel;
	unsigned short mRank;
	unsigned short mNumber;
	int	mTimer;	
public:

	//TODO
	CProduce( ) {}

	void Clear( )
	{
		mType1	= 0;
		mType2	= 0;
		mType3	= 0;
		mLevel	= 0;
		mRank	= 0;
		mNumber = 0;
		mTimer	= 0;
	}
};

class ComplexPos
{
public:
	ComplexPos()
	{
		if ( CObj::msCreateMode )
		{
			mLineID = 0;
			mMapID  = 0;
			mPos.mX = -1;
			mPos.mY = -1;
		}
	}
	unsigned int mLineID;
	unsigned int mMapID;
	CWTPoint mPos;
};


enum STATE_FROM
{
	NONE		= 0,
	EQUITMENT	= 1,
	BUFF		= 2,
	SKILL		= 3,
};
struct StateParam
{
	bool		mInState;
	STATE_FROM	mFrom;
	int			mIndex;
	int			mParames1;
	int			mParames2;
	int			mBuffAdd;

	StateParam( )
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

	int Initialize( )
	{
		Clean( );
		return SUCCESS;
	}

	int Resume( )
	{
		return 0;
	}

	void Clean( )
	{
		mInState	= false;
		mFrom		= NONE;
		mIndex		= 0;
		mParames1	= 0;
		mParames2	= 0;
		mBuffAdd	= 0;
	}
};
struct TripEntityInfo
{
	int mTripEntityID;
	int mTripIndex;

	TripEntityInfo()
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

	int Initialize( )
	{
		Clean( );
		return SUCCESS;
	}

	int Resume( )
	{
		return 0;
	}

	void Clean()
	{
		mTripEntityID = 0;
		mTripIndex = 0;
	}
};

/************************************************************************/
/* 副本结构                                                                     */
/************************************************************************/
class CPlayerRepetion
{
private:
	struct 			RepetionData
	{
		int			mIntData[ 13 ];
		int			mCount;
		int			mWeekCount;
		uint64_t	mLastSaveTime;
		int			mSignUp;
		int			mIntoTime;
		int			mDieCount;		
	};
private:
	unsigned short	mBackLineID;
	unsigned short	mBackMapID;
	unsigned short	mBackPosX;
	unsigned short	mBackPosY;
	bool			mLoadFalse;
	int				mCampID;			// 阵营, PVP战场		

	KEY_TYPE		mOldRepeKey;		// fuck all 
	RepetionData	mRepeData[ _REPE_MAX_COUNT_ ];

	int				mInRepetion;
	int				mInRepetionIndex;
	
	bool			mCheckResult;		// 副本答题的验证结果
	int				mAnswerCount;		// 答题次数	
	int				mGlobalSceneStatus;	// 全局服务器状态
	int				mGlobalWarOfflineTime;// 从全局副本中离开的时间
	KEY_TYPE		mRepetionKey;		// 即将进入的副本key(没有进入副本之前就赋值)
public:
	CPlayerRepetion( ) 
	{
		if( CObj::msCreateMode == 1 )
		{
			Initialize( );
		}
		else
		{

		}
	}

	int Initialize( )
	{
		mBackLineID = 0;
		mBackMapID = 0;
		mBackPosY = 0;
		mBackPosX = 0;
		mLoadFalse = false;
//		memset( mRepetionCount,0, sizeof( mRepetionCount ) );
		mOldRepeKey = 0;
		memset( mRepeData, 0, sizeof( mRepeData ) );
		mCampID = 0;
		
		mInRepetion = 0;
		mInRepetionIndex = 0;
		mCheckResult = 0;
		mAnswerCount = 0;
		mGlobalSceneStatus = 0;
		mGlobalWarOfflineTime = 0;
		mRepetionKey = 0;
		return SUCCESS;
	}

	int Resume( )
	{
		return SUCCESS;
	}
	
	void SetGlobalSceneStatus( int nStatus ){ mGlobalSceneStatus = nStatus; }
	int GetGlobalSceneStatus(){ return mGlobalSceneStatus; }
	void SetGlobalWarOffLineTime( int nTime ){ mGlobalWarOfflineTime = nTime; }
	int GetGlobalWarOffLineTime(){ return mGlobalWarOfflineTime; }

	unsigned short GetBackLineID( )	{ return mBackLineID; }
	unsigned short GetBackMapID( )	{ return mBackMapID; }
	unsigned short GetPosX( )			{ return mBackPosX; }
	unsigned short GetPosY( )			{ return mBackPosY; }
	int GetRepetionCount( unsigned int vIndex, unsigned int* vCount );
	int GetRepetionWeekCount( unsigned int vIndex, unsigned int* vCount );

	int SetBackLineID( unsigned short vBackLineID ) { mBackLineID = vBackLineID; return SUCCESS; }
	int SetBackMapID( unsigned short vBackMapID ) { mBackMapID = vBackMapID; return SUCCESS; }
	int SetPosX( unsigned short vPosX ) { mBackPosX = vPosX; return SUCCESS; }
	int SetPosY( unsigned short vPosY ) { mBackPosY = vPosY; return SUCCESS; }
	int SetCampID( int vCampID ) { mCampID = vCampID; return SUCCESS; }
	int GetCampID( ) { return mCampID; }
	int IncRepetionCount( unsigned int vIndex ) ;
	
	int SetInRepetion( ) { mInRepetion = 1; return SUCCESS; }
	int SetUnRepetion( ) { mInRepetion = 0; return SUCCESS; }
	int GetInRepetion( ) { return mInRepetion; }
	int SetInRepetionIndex( int vRepetionIndex ) { mInRepetionIndex = vRepetionIndex; return mInRepetionIndex; }
	int GetInRepetionIndex( ){ return mInRepetionIndex; }

	

	int DecRepetionCount( unsigned int vIndex );

	int SetRepetion( unsigned int vIndex, unsigned int vCount, unsigned int vWeekCount, unsigned int vSignUp = 0 ,unsigned int vIntoTime = 0, unsigned int vDieCount = 0 );
	int RepetionSignUp( unsigned int vIndex);
	int RepetionClearSignUp( unsigned int vIndex);
	int GetSignUpTime( unsigned int vIndex );

	int SetRepetionIntoTime( unsigned int vIndex );
	int RepetionClearIntoTime( unsigned int vIndex );
	int GetIntoTime( unsigned int vIndex );

	int AddDieCount( unsigned int vIndex );
	int GetDieCount( unsigned int vIndex );
	int ClearDieCount( unsigned int vIndex );

	int ChangeRepetionCount( unsigned int vIndex, int vCount );

	int CleanRepetion( )
	{
		unsigned int i = 0;
		for( i = 0; i < ARRAY_CNT( mRepeData ); i++ )
		{
			mRepeData[ i ].mCount = 0;			
			mRepeData[ i ].mLastSaveTime = 0;	
		}
		mLoadFalse = true;
		return SUCCESS;
	}

	int CleanWeekRepetion( );

	int SetIntData( unsigned short vRepetionIndex, unsigned int vIndex, int vData )
	{
		LK_ASSERT( vRepetionIndex < ARRAY_CNT( mRepeData ), return ERR_INVALID_PARAMETER );
		LK_ASSERT( vIndex < ARRAY_CNT( mRepeData[ vRepetionIndex ].mIntData ), return ERR_INVALID_PARAMETER );

		if( mRepeData[ vRepetionIndex ].mIntData[ vIndex ] < 0 )
		{
			mRepeData[ vRepetionIndex ].mIntData[ vIndex ] = 0;
		}

		mRepeData[ vRepetionIndex ].mIntData[ vIndex ] = ( vData >= 0 ) ? vData : 0;
		return SUCCESS;
	}

	int GetIntData( unsigned short vRepetionIndex, unsigned int vIndex, int* vData )
	{
		LK_ASSERT( vRepetionIndex < ARRAY_CNT( mRepeData ), return ERR_INVALID_PARAMETER );
		LK_ASSERT( vIndex < ARRAY_CNT( mRepeData[ vRepetionIndex ].mIntData ), return ERR_INVALID_PARAMETER );

		*vData = mRepeData[ vRepetionIndex ].mIntData[ vIndex ];

		return SUCCESS;;
	}

	int SetOldRepeKey( KEY_TYPE vKey ) { mOldRepeKey = vKey; return SUCCESS; }
	KEY_TYPE GetOldRepeKey( ) { return mOldRepeKey; }

	int PB2Repetion( PBRepetion* vpPB, CEntityPlayer* vpPlayer );
	int Repetion2PB( PBRepetion* vpPB, KEY_TYPE vKey );
	
	// Get & Set CheckResult
	bool GetCheckResult(){ return mCheckResult; }
	void SetCheckResult( bool bCheckResult ){ mCheckResult = bCheckResult; }
	
	// Get & Set AnswerCount
	int GetAnswerCount() { return mAnswerCount; }
	void SetAnswerCount( int nAnswerCount ) { mAnswerCount = nAnswerCount; }
	
	// 清除答题验证
	void ClearQuestionVerifyInfo()
	{
		mCheckResult = false;		// 副本答题的验证结果
		mAnswerCount = 0;			// 答题次数	
	}
	// Get&Set mRepetionKey
	KEY_TYPE GetRepetionKey(){ return mRepetionKey; }
	void SetRepetionKey( KEY_TYPE nKey ){ mRepetionKey = nKey; }
};

class CPlayerExchange
{
private:
	int 	mMoney;
	int		mYb;

public:
	CPlayerExchange( )
	{
		if( CObj::msCreateMode == 1 )
		{
			Initialize( );
		}
		else
		{

		}	
	}

	int Initialize( )
	{
		mMoney = 0;
		mYb = 0;
		return SUCCESS;
	}

	int SetMoney( int vMoney ) { mMoney = vMoney; return SUCCESS; }
	int SetYb( int vYb ) { mYb = vYb; return SUCCESS; }
	int GetMoney( ) { return mMoney; }
	int GetYb( ) { return mYb; }
};

// ********************************************************************** //
// CPropertyFormula
// ********************************************************************** //

class CPropertyFormula
{
public:
	// 得到非战斗打坐状态的HP恢复速度
	static unsigned int GetNormalSitHPRSpeed( CPropertyCharacter* pProperty );

	// 得到非战斗打坐状态的MP恢复速度
	static unsigned int GetNormalSitMPRSpeed( CPropertyCharacter* pProperty );

	// 得到战斗打坐状态的HP恢复速度
	static unsigned int GetBattleSitHPRSpeed( CPropertyCharacter* pProperty );

	// 得到战斗打坐状态的MP恢复速度
	static unsigned int GetBattleSitMPRSpeed( CPropertyCharacter* pProperty );

	// 得到非战斗状态的HP恢复速度
	static unsigned int GetNormalHPRSpeed( CPropertyCharacter* pProperty );

	// 得到非战斗状态的MP恢复速度
	static unsigned int GetNormalMPRSpeed( CPropertyCharacter* pProperty );

	// 得到战斗状态的HP恢复速度
	static unsigned int GetBattleHPRSpeed( CPropertyCharacter* pProperty );

	// 得到战斗状态的MP恢复速度
	static unsigned int GetBattleMPRSpeed( CPropertyCharacter* pProperty );

	// 计算技能抗性伤害
	static unsigned int GetResAttack( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateSkill* pSkill );

	// 计算技能物理伤害
	static unsigned int GetPSkillAttack( 	CEntityCharacter* pSrcEntity,
											CEntityCharacter* pDesEntity,
											CTemplateSkill* pSkill, 
											EResultType& rResult
										);

	// 计算技能法术伤害
	static unsigned int GetMSkillAttack( 	CEntityCharacter* pSrcEntity,
											CEntityCharacter* pDesEntity,
											CTemplateSkill* vpSkill, 
											EResultType& rResult
										);

	// 计算普通物理攻击伤害
	static unsigned int GetPNormalAttack(	CEntityCharacter* pSrcEntity,
											CEntityCharacter* pDesEntity, 
											EResultType& rResult
										);

	// 计算普通魔法攻击伤害
	static unsigned int GetMNormalAttack(	CEntityCharacter* pSrcEntity,
											CEntityCharacter* pDesEntity,
											EResultType& rResult
										);

	// 得到附加伤害加成固定值
	static int GetAddFixDamage( CPropertyCharacter* pProperty );

	// 得到附加伤害加成百分比
	static int GetAddPerDamage( CPropertyCharacter* pProperty );

	// 得到物理附加伤害减免固定值
	static int GetDecFixPDamage( CPropertyCharacter* pProperty );

	// 得到物理附加伤害减免百分比
	static int GetDecPerPDamage( CPropertyCharacter* pProperty );

	// 得到法术附加伤害减免固定值
	static int GetDecFixMDamage( CPropertyCharacter* pProperty );

	// 得到法术附加伤害减免百分比
	static int GetDecPerMDamage( CPropertyCharacter* pProperty );
	
	// 获取伤害偷取HP百分比
	static int GetStealHP( CPropertyCharacter* pProperty );
		
	// 获取伤害偷取MP百分比
	static int GetStealMP( CPropertyCharacter* pProperty );

	// 得到物理攻击力
	static unsigned int GetPAttackValue( CPropertyCharacter* pProperty, bool bIsMagicWeapon = false );

	// 得到法力攻击力
	static unsigned int GetMAttackValue( CPropertyCharacter* pProperty, bool bIsMagicWeapon = false );

	// 得到技能命中值
	static unsigned int GetSkillHitrate( 	unsigned short vSkillID, 
											unsigned short vSkillLevel, 
											CPropertyCharacter* pProperty 
										);

	// 得到物理攻击最大值
	static CPropertyValue GetPAttackUpper( CPropertyCharacter* pProperty );

	// 得到法术攻击最大值
	static CPropertyValue GetMAttackUpper( CPropertyCharacter* pProperty );

	// 得到物理攻击最小值
	static CPropertyValue GetPAttackLower( CPropertyCharacter* pProperty );

	// 得到法术攻击最小值
	static CPropertyValue GetMAttackLower( CPropertyCharacter* pProperty );

	// 得到物理防御力
	static CPropertyValue GetPDefence( CPropertyCharacter* pProperty );

	// 得到法术防御力
	static CPropertyValue GetMDefence( CPropertyCharacter* pProperty );

	// 得到物理免疫
	static int GetPImmunity( int vLevel, CPropertyCharacter* pProperty );

	// 得到法术免疫
	static int GetMImmunity( int vLevel, CPropertyCharacter* pProperty );

	// 得到物理抗性
	//static CPropertyValue GetPResist( CPropertyCharacter* pProperty );

	// 得到法术抗性
	//static CPropertyValue GetMResist( CPropertyCharacter* pProperty );

	// 得到力量
	static CPropertyValue GetStr( CPropertyPlayer* pProperty );

	// 得到智慧
	static CPropertyValue GetWis( CPropertyPlayer* pProperty );

	// 得到精神
	static CPropertyValue GetSpr( CPropertyPlayer* pProperty );

	// 得到体质
	static CPropertyValue GetCon( CPropertyPlayer* pProperty );

	// 得到耐力
	static CPropertyValue GetSta( CPropertyPlayer* pProperty );

	// 得到普攻命中值
	static CPropertyValue GetNormalHitrate( CPropertyCharacter* pProperty );

	// 得到普攻闪避值
	static CPropertyValue GetNormalDuck( CPropertyCharacter* pProperty );

	// 得到技能闪避值( 残影 )
	static CPropertyValue GetSkillDuck( CPropertyCharacter* pProperty );

	// 得到忽视闪避值
	static CPropertyValue GetIgnoreDuck( CPropertyCharacter* pProperty );

	// 得到致命一击等级
	static CPropertyValue GetDeathRate( CPropertyCharacter* pProperty, bool bIsMagicWeapon = false );

	// 得到致命一击伤害加成
	static CPropertyValue GetDeathValue( CPropertyCharacter* pProperty );

	// 得到致命一击防御
	static CPropertyValue GetDeathDefense( CPropertyCharacter* pProperty );

	// 得到HP最大值
	static CPropertyValue GetMaxHP( CPropertyCharacter* pProperty );

	// 得到MP最大值
	static CPropertyValue GetMaxMP( CPropertyCharacter* pProperty );

	//// 得到异常状态基础抗性
	//static CPropertyValue GetAbnResist( CProperty* pProperty );

	//// 得到炽焰状态抗性
	//static CPropertyValue GetFireResist( CProperty* pProperty );

	//// 得到玄冰状态抗性
	//static CPropertyValue GetWaterResist( CProperty* pProperty );

	//// 得到怒风状态抗性
	//static CPropertyValue GetWindResist( CProperty* pProperty );

	//// 得到纯阳状态抗性
	//static CPropertyValue GetLightResist( CProperty* pProperty );

	//// 得到凝尘状态抗性
	//static CPropertyValue GetEarthResist( CProperty* pProperty );

	//// 得到寒阴状态抗性
	//static CPropertyValue GetShadowResist( CProperty* pProperty );

	// 得到行走移动速度
	static CPropertyValue GetWalkSpeed( CPropertyCharacter* pProperty );

	// 得到奔跑移动速度
	static CPropertyValue GetRunSpeed( CPropertyCharacter* pProperty );

	// 得到移动速度
	static CPropertyValue GetRunSpeed( int vEntityType, int vValueID );

	static CPropertyValue GetSkillLess( CPropertyCharacter* pProperty, int vSkillGenerID );

	// 计算命中率
	static float GetHitrate( int vSrcLevel, int vDesLevel, int vHitValue );

	// 计算闪避率
	static float GetDuckRate( int vSrcLevel, int vDuckValue, int vIgnoreValue );

	// 计算暴击率
	static float GetDeathRate( int vDesLevel, int vDeathRate, int vDeathDefense );
};



