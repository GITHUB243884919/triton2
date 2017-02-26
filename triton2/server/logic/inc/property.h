#ifndef _PROPERTY_H_
#define _PROPERTY_H_

#include <coretype.h>
#include "proclient_svr.h"
#include "protype.h"
#include "judgetype.h"
#include "lk_set.h"
#include "lk_vector.h"


class CProperty;
class CPropertyCharacter;
class CPropertyPedal;
class CPropertyDrop;
class CPropertyMoney;
class CPropertyItem;
class CPropertyPlayer;
class CPropertyPet;
class CPropertyNPC;

class CEntity;
class CEntityMoney;
class CEntityDrop;
class CEntityItem;
class CEntityPedal;
class CEntityPet;
class CEntityNpc;
class CEntityFuncNpc;
class CEntityPlayer;
class CEntityCharacter;
class CEntityResultList;
class CMessage;

// 领奖限制类型
enum EM_AwardLimitType
{	
	EALT_Daily = 0,		// 当天限制
	EALT_Weekly = 1,	// 当周限制
	EALT_Max,
};

class CYuanBao
{
	private:
		enum
		{
			em_ibtrans_per_player = 8,
			em_max_yaunbao_amount = 100000000,
		};

		int m_nValue;

		typedef lk::vector<int, em_ibtrans_per_player> LOCKEDYB;
		LOCKEDYB m_lstLocked;

	public:
		enum 
		{
			em_bind_money = BIND_MONEY,
			em_unbind_money = UNBIND_MONEY,
			em_linekong_money = LINEKONG_MONEY,
			em_silver_yuanbao = SILVER_YUANBAO,
			em_golden_yuanbao = GOLDEN_YUANBAO,
			em_combat_fame = COMBAT_FAME,
			em_repe_fame = REPE_FAME,
			em_common_fame = COMMON_FAME,
		};

		CYuanBao(int value = 0){}
		~CYuanBao(){}

		void initialize()
		{
			m_nValue = 0;
			m_lstLocked.initailize();
		}

		void set( int nValue) 
		{
			m_nValue = nValue; 
		}

		// 锁定nValue
		inline bool lock(int nValue)
		{
			if ( m_nValue >= nValue )
			{
				if ( m_lstLocked.size() >= em_ibtrans_per_player) return false;

				m_lstLocked.push_back(nValue);
				m_nValue = m_nValue - nValue;

				return true;
			}

			return false;
		}

		// 确认nValue
		inline bool commit(int nValue)
		{
			LOCKEDYB :: iterator iter = m_lstLocked.begin();
			for ( ;iter != m_lstLocked.end(); ++iter)
			{
				if ( *iter == nValue)
				{
					m_lstLocked.erase(iter);
					return true;
				}
			}
			
			return false;
		}

		// 回滚 nValue 
		inline bool rollback(int nValue)
		{
			if ( commit(nValue) )
			{
				m_nValue += nValue;
				return true;
			}

			return false;
		}

		// 返回锁定的金额
		inline int locken()
		{
			int nlocked = 0;
			LOCKEDYB::iterator iter = m_lstLocked.begin();

			for ( ;iter != m_lstLocked.end(); ++iter) nlocked += *iter;


			return nlocked;
		}

		// 返回当前剩余的金额
		inline int remain()
		{
			return m_nValue;
		}

		// 充值
		inline bool add( int nValue)
		{
			if ( nValue < 0) return false;

			int n  = m_nValue + nValue;

			if ( n > em_max_yaunbao_amount )
			{
				return false;
			}

			m_nValue = n;
			return true;
		}
};

/************************************************************************/
/* Property 是所有属性实体的基类                                        */
/************************************************************************/
class CProperty
{
private:
	unsigned int		mEntityID;		// 实体ID
	EEntityType			mEntityType;	// 类型
	unsigned int		mTempID;		// 实体模板ID	

protected:
	// 设置消失定时
	unsigned int		mDisappearTimer;	
	unsigned int		mDisappearTime;
	unsigned int		mLevel;

public:

	CProperty( )
	{ 
		if( CObj::msCreateMode )
		{
			Initialize( );
		}
		else
		{
			Resume( );
		}
	}
	
	virtual ~CProperty( ){ }

	int Initialize( )
	{
		mEntityType = ENTITYTYPE_NONE;
		mEntityID = 0;
		mTempID = 0;
		mDisappearTime = 0;
		mDisappearTime = 0;
		mLevel = 0;

//		Initial( );
		
		return SUCCESS;
	}

	virtual int Resume( )
	{
		return SUCCESS;
	}

	virtual int Initial() = 0;

	virtual int Final()
	{
		mEntityType = ENTITYTYPE_NONE;
		mEntityID = 0;
		mTempID = 0;
		mDisappearTime = 0;
		mDisappearTime = 0;
		mLevel = 0;
		return SUCCESS;	
	}

public:	

	// 启动保存的定时器，用于实体刚创建时调用
	virtual void StartSavedTimer() { }
	// 刷新属性，参数表示是否初始刷新
	virtual void Refresh( bool vRefresh = false ){ }

	// 获得自身对应的实体
	CEntity* GetSelfEntity( );

	// 通过实体ID查找Property
	static CProperty* SearchProByEntityID( unsigned int vEntityID );

public:

	// Get & Set EntityID
	inline unsigned int GetEntityID( )	{ return mEntityID; }
	int SetEntityID( unsigned int vEntityID );

	// Get & Set EntityType
	inline EEntityType GetEntityType( ) { return mEntityType; }
	int SetEntityType( EEntityType vType );	

	// Get & Set TempID
	inline unsigned int GetPropertyTempID( ) { return mTempID; }
	//  ( 如果是人，设置职业ID； 如果是NPC, 是模板ID )
	void SetPropertyTempID( unsigned int vTempID );	

	// Get & Set DisappearTimer
	inline unsigned int GetDisapperaTimer( ){ return mDisappearTimer; }
	inline void SetDisappearTimer( unsigned int vDisappearTimer ){ mDisappearTimer = vDisappearTimer; }

	// Get & Set DisappearTime
	inline unsigned int GetDisapperaTime( ){ return mDisappearTime; }
	inline void SetDisappearTime( unsigned int vDisappearTime ){ mDisappearTime = vDisappearTime; }

	inline int SetLevel( unsigned int vLevel ) { mLevel = vLevel; return SUCCESS; }
	inline unsigned int GetLevel( ) { return mLevel; }

	virtual void IsDisappear( CEntity* pEntity, int vTickOffset, std::vector< unsigned int >& vDeleteList );
};



/************************************************************************/
/*  活的实体属性基类                                                    */
/************************************************************************/
class CPropertyCharacter : public CProperty
{
public:

	CPropertyCharacter( ){ }
	virtual ~CPropertyCharacter( ){ }

	// 这个结构存储了人物属性常用的数据，在换装，增删BUFF等情况下重新计算
	// 这些数据而不是在使用的时候计算，减少运算次数，提高效率
	// 最近一次修改去掉了物理抵抗和法术抵抗，改为防御
	struct Information
	{
		CPropertyValue					mStr;
		CPropertyValue					mWis;
		CPropertyValue					mSpr;
		CPropertyValue					mCon;
		CPropertyValue					mSta;

		CPropertyValue					mMaxHP;
		CPropertyValue					mMaxMP;
		CPropertyValue					mMaxAP;

		CPropertyValue					mPAttackMax;
		CPropertyValue					mPAttackMin;

		CPropertyValue					mMAttackMax;
		CPropertyValue					mMAttackMin;

		CPropertyValue					mDeathRate;
		CPropertyValue					mDeathValue;
		CPropertyValue					mDeathDefense;

		CPropertyValue					mNormalDuck;
		CPropertyValue					mSkillDuck;
		CPropertyValue					mIgnoreDuck;

		CPropertyValue					mSpeed;

		CPropertyValue					mPDefence;
		CPropertyValue					mMDefence;		

		int								mAddFixDamage;
		int								mAddPerDamage;

		int								mDecFixPDamage;
		int								mDecFixMDamage;
		int								mDecPerMDamage;
		int								mDecPerPDamage;

		int								mStealHP;
		int								mStealMP;

		int								mNormalHitRate;
		int								mSkillHitRate;

		ATTACK_TYPE						mAttackType;

		int								mResDef[ RES_NUM ];
		int								mResAtt[ RES_NUM ];	
		int								mEquipWizard[ WIZARD_NUM ];
	};


protected:
	Information 		mInformation;
	unsigned int		mCurHP;			// 当前HP
	unsigned int		mCurMP;			// 当前MP
	unsigned int		mCurAP;			// 当前AP
	unsigned int		mCurSpeed;		// 当前速度	
	int					mBattleTimer;	// 战斗计时器
	int					mRefreshTimer;	// 恢复计时器
	StateParam      	mStateParam[ STATE_COUNT_MAX ];// propety的状态
	CBuffList			mBuffList;					   // Buff列表
	CDamageList			mDamageList;		// 伤害列表
	unsigned int		mFirstEntityID;		// 第一个攻击实体
	unsigned int		mMaxDamager;		// 最大伤害实体	
	float				mLastHPPer;			// 最后进入战斗时候生命百分比
	unsigned int		mPetID;				// 携带宠物的ID 
   	unsigned int		mTotalTripIndex;	// 当前陷阱的个数
	TripEntityInfo  	mTripEntityIDList[ MAX_TRIP_ENTITY ]; // 陷阱列表	
	
public:
	virtual int Initial() = 0;
	virtual int Final() = 0;

	// 初始化CPropertyCharater的数据
	void CharacterInitial( );

	// 销毁CPropertyCharacter的数据
	void CharacterFinal( );

	// 移除Character状态
	void DetachState( EState vState, int vIndex, STATE_FROM vFrom, int vParames1 = 0, int vParames2 = 0 );

	// 加入Character状态
	void AttachState( EState vState, int vParames1, int vParames2, int vIndex, STATE_FROM vFrom );
	public:
	// 向伤害列表中加入伤害
	void InsertDamage( const CDamage& rDamage );

	// 获得指定ID的实体的伤害百分比
	float	GetDamagePer( unsigned int vEntityID );

	// 是否可以自己控制
	bool Controlable( );

	// 是否可以移动
	bool Moveable( );

	// 是否可以攻击
	bool Attackable();

	// 获得当前HP百分比
	float GetHPPercent( );

	// 获得当前MP百分比
	float GetMPPercent( );

	// 刷新属性
	virtual void Refresh( bool vRefresh = false );

	// 销毁召唤兽实体
	void DestroyPetEntity( );

	// 销毁召唤兽实体,主人实体销毁时
	void DestroyPetByOwnerDestroy( );

	// 加入陷阱实体
	bool NoteAddTripEntityID( int vTripEntityID );

	// 去除陷阱实体
	bool ClearAddTripEntityID( int vTripEntityID );

	// 去除所有陷阱实体
	void ClearAllTripEntity(  );

	// 处理DelayAction事件
	int DelayActionEvent( CEntityCharacter* pEntity, unsigned int vTickOffSet );

	// 处理光环事件
	int AureoleTimeEvent( CEntityCharacter* pEntity, unsigned int vTickOffSet );

	// 处理Buff事件
	//int BuffTimeEvent( CEntityCharacter* pEntity, CTemplateBuff* tpBuffTpl, CBuffObject* tpBuff );

	// 处理技能冷却事件
	int SkillCoolingEvent( CEntityCharacter* pEntity, unsigned int vTickOffSet );

	// 处理战斗计时事件
	int BattleTimeEvent( CEntityCharacter* pEntity, unsigned int vTickOffSet ); 

	// 处理回血事件
	int RecTimeEvent( CEntityCharacter* pEntity, unsigned int vTickOffSet );

	// 创建速度改变通知消息
	bool CreateMessageNotifySpeedChanged( CMessage* pTotalMessage, CMessageSpeedChangedNotify* pMessage, unsigned int vEntityID, int vSpeed	);

	// 由BuffList的PB结构创建Buff列表
	void CreateBuffListFromTlv2( PBBuffList* pBuffer );

	// 把Buff列表的内容存储到PB结构
	void GetBuffListTlvBuffer2( PBBuffList* pBuffer );



	// 无条件干掉NPC (带渐隐的)
	//int OnKillEntityWithHideTimeout( CEntity* pEntity );


	// 获得预存储的Character属性
	inline CPropertyValue GetStr( ) { return mInformation.mStr; }
	inline CPropertyValue GetWis( ) { return mInformation.mWis; }
	inline CPropertyValue GetSpr( ) { return mInformation.mSpr; }
	inline CPropertyValue GetCon( ) { return mInformation.mCon; }
	inline CPropertyValue GetSta( ) { return mInformation.mSta; }
	inline CPropertyValue GetMaxHP( ) { return mInformation.mMaxHP; }
	inline CPropertyValue GetMaxMP( ) { return mInformation.mMaxMP; }
	inline CPropertyValue GetMaxAP( ) { return mInformation.mMaxAP; }
	inline CPropertyValue GetPAttackMax( ) { return mInformation.mPAttackMax; }
	inline CPropertyValue GetPAttackMin( ) { return mInformation.mPAttackMin; }
	inline CPropertyValue GetMAttackMax( ) { return mInformation.mMAttackMax; }
	inline CPropertyValue GetMAttackMin( ) { return mInformation.mMAttackMin; }
	inline CPropertyValue GetDeathRate( ) { return mInformation.mDeathRate; }
	inline CPropertyValue GetDeathValue( ) { return mInformation.mDeathValue; }
	inline CPropertyValue GetDeathDefense( ) { return mInformation.mDeathDefense; }
	inline CPropertyValue GetNormalDuck( ) { return mInformation.mNormalDuck; }
	inline CPropertyValue GetSkillDuck( ) { return mInformation.mSkillDuck; }
	inline CPropertyValue GetIgnoreDuck( ) { return mInformation.mIgnoreDuck; }
	inline CPropertyValue GetSpeed( ) { return mInformation.mSpeed; }
	inline CPropertyValue GetPDefence( ) { return mInformation.mPDefence; }
	inline CPropertyValue GetMDefence( ) { return mInformation.mMDefence; }
	inline int GetAddFixDamage( ) { return mInformation.mAddFixDamage; }
	inline int GetAddPerDamage( ) { return mInformation.mAddPerDamage; }
	inline int GetDecFixPDamage( ) { return mInformation.mDecFixPDamage; }
	inline int GetDecFixMDamage( ) { return mInformation.mDecFixMDamage; }
	inline int GetDecPerPDamage( ) { return mInformation.mDecPerPDamage; }
	inline int GetDecPerMDamage( ) { return mInformation.mDecPerMDamage; }
	inline int	GetStealHP( ) { return mInformation.mStealHP; }
	inline int GetStealMP( ) { return mInformation.mStealMP; }
	inline ATTACK_TYPE GetAttackType( ) { return mInformation.mAttackType; }
	inline int GetNormalHitRate( ) { return mInformation.mNormalHitRate; }
	inline int GetSkillHitRate( ) { return mInformation.mSkillHitRate; }
	inline int GetResDef( int i ) { if ( i <= RES_NONE || i >= RES_NUM ) return 0; return mInformation.mResDef[ i ]; }
	inline int GetResAtt( int i ) { if ( i <= RES_NONE || i >= RES_NUM ) return 0; return mInformation.mResAtt[ i ]; }
	inline int GetEquipWizard( int i ) { if ( i < 0 || i >= WIZARD_NUM ) return 0; return mInformation.mEquipWizard[ i ]; }

	
	// 修改预存储的Character属性，类型不同，修改的方式也不同
	virtual CPropertyValue RefreshStr( ) = 0;
	virtual CPropertyValue RefreshWis( ) = 0;
	virtual CPropertyValue RefreshMaxHP( ) = 0;
	virtual CPropertyValue RefreshSpr( ) = 0;
	virtual CPropertyValue RefreshSta( ) = 0;	
	virtual CPropertyValue RefreshCon( ) = 0;	
	virtual CPropertyValue RefreshMaxMP( ) = 0;
	virtual ATTACK_TYPE	RefreshAttackType( ) = 0;
	virtual CPropertyValue RefreshPAttackMax( ) = 0;
	virtual CPropertyValue RefreshPAttackMin( ) = 0;
	virtual CPropertyValue RefreshMAttackMax( ) = 0;
	virtual CPropertyValue RefreshMAttackMin( ) = 0;
	virtual CPropertyValue RefreshDeathRate( ) = 0;
	virtual CPropertyValue RefreshDeathValue( ) = 0;
	virtual CPropertyValue RefreshDeathDefense( ) = 0;
	virtual CPropertyValue RefreshNormalDuck( ) = 0;
	virtual CPropertyValue RefreshSkillDuck( ) = 0;
	virtual CPropertyValue RefreshIgnoreDuck( ) = 0;
	virtual CPropertyValue RefreshPDefence( ) = 0;
	virtual CPropertyValue RefreshMDefence( ) = 0;
	virtual int RefreshAddFixDamage( ) = 0;
	virtual int	RefreshAddPerDamage( );
	virtual int RefreshDecFixPDamage( ) = 0;
	virtual int RefreshDecFixMDamage( ) = 0;
	virtual int RefreshDecPerPDamage( ) = 0;
	virtual int RefreshDecPerMDamage( ) = 0;
	virtual int RefreshStealHP( );
	virtual int RefreshStealMP( );
	virtual int RefreshNormalHitRate( ) = 0;
	virtual int RefreshSkillHitRate( ) = 0;
	virtual int RefreshResDef( int *vDef ) = 0;
	virtual int RefreshResAtt( int *vAtt ) = 0;
	virtual int RefreshEquipWizard( int *vWizard );
	virtual	int GetDuckRate( ) = 0;

public:

	// Get & Set CurHP
	inline unsigned int GetCurHP( ){ return mCurHP; }
	inline void SetCurHP( unsigned int vCurHP ){ mCurHP = vCurHP; }

	// Get & Set CurMP
	inline unsigned int GetCurMP( ){ return mCurMP; }
	inline void SetCurMP( unsigned int vCurMP ){ mCurMP = vCurMP; }

	// Get & Set CurAP
	inline unsigned int GetCurAP( ){ return mCurAP; }
	inline void SetCurAP( unsigned int vCurAP ){ mCurAP = vCurAP; }

	// Get & Set CurSpeed
	inline unsigned int GetCurSpeed( ){ return mCurSpeed; }
	inline void SetCurSpeed( unsigned int vCurSpeed ){ mCurSpeed = vCurSpeed; }

	// Get & Set BattleTimer
	inline int GetBattleTimer( ){ return mBattleTimer; }
	inline void SetBattleTimer( int vBattleTimer ){ mBattleTimer = vBattleTimer; }

	// Get & Set RefreshTimer
	inline int GetRefreshTimer( ){ return mRefreshTimer; }
	inline void SetRefreshTimer( int vRefreshTimer ){ mRefreshTimer = vRefreshTimer; }

	// Get & Set StateParam
	inline StateParam GetStateParam( int i ){ return mStateParam[ i ]; }
	inline void SetStateParam( int i, StateParam vStateParam ){ mStateParam[ i ] = vStateParam; }
	inline void CleanStateParam( ){ memset( mStateParam, 0, sizeof( mStateParam ) ); }

	// Get & Set BuffList
	inline CBuffList *GetBuffList( ){ return &mBuffList; }

	// Get & Set SkillList
	virtual CSkillList& GetSkillList( ) = 0;

	// Get & Set DamageList
	inline CDamageList *GetDamageList( ){ return &mDamageList; }

	// Get & Set FirstEntityID
	inline unsigned int GetFirstEntityID( ){ return mFirstEntityID; }
	inline void SetFirstEntityID( unsigned int vFirstEntityID ){ mFirstEntityID = vFirstEntityID; }

	// Get & Set MaxDamager
	inline unsigned int GetMaxDamager( ){ return mMaxDamager; }
	inline void SetMaxDamager( unsigned int vMaxDamager ){ mMaxDamager = vMaxDamager; }

	// Get & Set LastHPPer
	inline float GetLastHPPer( ){ return mLastHPPer; }
	inline void SetLastHPPer( float vLastHPPer ){ mLastHPPer = vLastHPPer; }

	// Get & Set PetID
	inline unsigned int GetPetID( ){ return mPetID; }
	inline void SetPetID( unsigned int vPetID ){ mPetID = vPetID; }

	// Get & Set TotalTripIndex
	inline unsigned int GetTotalTripIndex( ){ return mTotalTripIndex; }
	inline void SetTotalTripIndex( unsigned int vTotalTripIndex ){ mTotalTripIndex = vTotalTripIndex; }

	// Get & Set TripEntityIDList
	inline TripEntityInfo GetTripEntityIDList( int i ){ return mTripEntityIDList[ i ]; }
	inline void SetTripEntityIDList( int i, TripEntityInfo vTripEntityIDList ){ mTripEntityIDList[ i ] = vTripEntityIDList; }
	inline void CleanTripEntityIDList( ){ memset( mTripEntityIDList, 0, sizeof( mTripEntityIDList ) ); }
	inline int GetTripEntityIDListSize( ){ return ARRAY_CNT( mTripEntityIDList ); }
	
	

	// 技能武器需求匹配
	virtual bool SkillWeaponValid( CTemplateSkill* pSkill ) { return true; }

	// 装备是否可用 ( 耐久度是否为0 )
	virtual bool EquipmentAvailable( EEquipPart emPart ) { return true; }

	// Get CommonCD
	virtual int GetCommonCD( ){ return 0; }

	// 技能是否在公共冷却中
	virtual bool IsCommonCoolingDown( CTemplateSkill* pSkill ) { return false; }


	int AttachBuffState( int vSrcEntityID, CTemplateBuff::CBuffFunc* vpBuffFunc, int vIndex );

	// 从PB结构创建State列表
	void CreateStateListFromTlv2( PBStateList* pBuffer );

	// 从PB结构获得State列表
	void GetStateListTlvBuffer2( PBStateList* pBuffer );


	virtual void SetTranChangeLevel( int vLevel ) {  }
	virtual int GetTranChangeLevel( ) { return 0; }
};

// 记录杀死者的ID和最近时间
struct KillData
{
	int mKillID;
	int mKillTime;
};

// 出售给NPC道具
struct SaleItem
{
	unsigned int mItemID;	// CItemObject 对象id(唯一标示)
	unsigned int mNumber;
	unsigned int mPrice;

	SaleItem() 
	{
		if ( CObj::msCreateMode )
		{
			mItemID = 0;
			mNumber = 0;
			mPrice = 0;
		}		
	}
	SaleItem( unsigned int vItemID, unsigned int vNumber, unsigned int vPrice ) : 
	mItemID( vItemID ), mNumber( vNumber ), mPrice( vPrice ) { } 
};

// 摊位数据
struct STALL
{
	unsigned int obj_id;
	unsigned int Price;
	unsigned int Number;
	unsigned int Index;// stall index
	unsigned int PriceType;

	STALL() 
	{
		if ( CObj::msCreateMode )
		{
			obj_id = 0;
			Price = 0;
			Number = 0;
			Index = 0;
			PriceType = 0;
		}		
	}
	STALL( unsigned int vid, unsigned int vPrice, unsigned int vNum, unsigned int vIndex, unsigned int vPriceType ) : 
	obj_id( vid ), Price( vPrice ), Number( vNum ), Index( vIndex ), PriceType(vPriceType) { }
};

class CStallRecord : public CObj
{
public:
	lk::string<NAME_LENGTH> mBuyerName;
	int		mItemID;
	int		mNumber;
	int		mPrice;
	int		mTimeStamp;
	int		mAction;
	int		mTax;
	int		mPriceType;//价格类型
	int		mFee;//手续费
	int64	mItemGuid;
	int		mItemIndex;
	int		mStallIndex; // 摊位中的索引

public:
	CStallRecord(){ Initialize(); }
	~CStallRecord(){ ; }

	int Initialize() 
	{
		mItemID = 0; 
		mNumber = 0;
		mPrice = 0;
		mTimeStamp = 0;
		mAction = 0; 
		mTax = 0;
		mPriceType = 0; 
		mFee = 0;
		mItemGuid = 0; 
		mItemIndex = 0; 
		mStallIndex=0; 
		return 0 ;
	}
	int Resume() { return  0; } 
};

class CStallInfo
{
private:
	lk::string<NAME_LENGTH>		mName;			// 摊位名称
	lk::string<NAME_LENGTH * 2>	mAd;			// 广告信息
	bool bSaved;// 收摊是否保存
	int mStallType;
public:

	typedef lk::vector< STALL, MAX_STALL_INDEX > StallList;
	StallList	mStall;

	typedef lk::vector< CStallRecord, MAXPLAYERNUM > RecordList;
	RecordList mRecordList;

	CStallInfo() { bSaved = false; mStallType = 0; mStall.initailize(); mRecordList.initailize(); }
	~CStallInfo() { mStall.clear(); mRecordList.clear(); }

	inline void SetFlag( bool bFlag ) { bSaved = bFlag; }
	inline bool IsSaved() { return bSaved; }
	inline void SetName( const char* pName ) { mName = lk::string<NAME_LENGTH>( pName ); }
	inline void SetAd( const char* pAd ) { mAd = lk::string<NAME_LENGTH*2>( pAd ); }
	inline const char* GetName() { return mName.c_str(); }
	inline const char* GetAd() { return mAd.c_str(); }
	inline void PushObj( unsigned int vid, unsigned int vStallIndex, unsigned int vNum, unsigned int vPrice, int vPriceType )
	{
		STALL tStall( vid, vPrice, vNum, vStallIndex,vPriceType );
		mStall.push_back( tStall );
	}
	inline void PushRecord( CStallRecord* vpRecord )
	{
		if ( vpRecord == NULL )
			return ;
		if ( mRecordList.size() == MAXPLAYERNUM )
		{
			RecordList::iterator it = mRecordList.begin();
			mRecordList.erase( it );
		}
		mRecordList.push_back( *vpRecord );
	}
	inline void CleanStall() { bSaved = false; mStall.clear(); mRecordList.clear(); }
	inline void CleanRecord(){ mRecordList.clear(); }
	inline void SetStallType(const int vStallType){ mStallType = vStallType;} 
	inline int GetStallType() { return mStallType;}


};

// 生活技能

// 玩家的生活技能
struct ComposeInfo 
{
	int mTempID;
	int mUsedTimes;			
};

class CLifeSkill
{
public:
	typedef lk::vector< ComposeInfo, MAX_LIFESKILL_COMPOSENUM > MultiComposeIDList;
	enum EDegreeAddType
	{
		DEGREEADDTYPE_USECOMPOSE = 1,	// 通过使用配方的方式增加熟练度
		DEGREEADDTYPE_BYLEVELUP	 = 2,	// 通过技能进阶的方式增加熟练度
		DEGREEADDTYPE_BYITEM	 = 3,	// 通过使用物品的方式增加熟练度
		DEGREEADDTYPE_BYSTUDY	 = 4,	// 通过研究新配方增加熟练度		
	};
private:
	int		mLifeSkillType;				// 生活技能类型
	int		mLifeSkillLevel;			// 生活技能等级
	int		mSpecializedDegree;			// 生活技能熟练度
	// short	mLifeSkillStatus;			// 生活技能状态
	short	mLifeSkillKind;				// 技能种类，主技能还是辅修技能	
	MultiComposeIDList mMultiComposeIDList;
public:
	CLifeSkill()
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
	~CLifeSkill( ){ }
public:
	int Initialize( );
	int Resume(){ return 0; }
public:
	// 获取&设置生活技能类型
	int		GetLifeSkillType(){  return mLifeSkillType; }
	void	SetLifeSkillType( int nLifeSkillType ){ mLifeSkillType = nLifeSkillType; } 

	// 设置&获取生活技能等级
	int		GetLifeSkillLevel(  ){  return mLifeSkillLevel; }
	void	SetLifeSkillLevel( int nLifeSkillLevel ){ mLifeSkillLevel = nLifeSkillLevel; }

	// 设置&获取技能熟练度	
	int		GetSpecializedDegree(){ return mSpecializedDegree; }
	void	SetSpecializedDegree(  int nSpecializedDegree ){ mSpecializedDegree = nSpecializedDegree;  }	

	// 设置&获取生活技能种类
	void	SetLifeSkillKind( int nLifeSkillKind ){ mLifeSkillKind = nLifeSkillKind; }
	int		GetLifeSkillKind(  ){ return mLifeSkillKind; }

public:	
	// 学习一个新配方
	int StudyCompose( int nComposeID, int nGetWay, int &rPlayerAp, unsigned int &rPlayerMoney );

	// 删除一个配方
	int DeleteCompose( int nComposeID );

	// 检查是否有某个配方
	bool CheckCompose( int nComposeID );

	// 增加熟练度
	void AddSpecializedDegree( int nDegree, int nAddType );		

	// 技能进阶
	int LevelUpLifeSkill( int nPlayerLevel, int &nPlayerBindMoney, int &nPlayerMoney );	

	// 遗忘生活技能
	int ForgetLifeSkill( );

	// 获取本等级的熟练度上限
	int GetCurrentDegreeLimit();

	// 使用配方
	int UseCompose( int nComposeID, int &rDegreeValue, bool bAddNow = true );

	// 检查熟练度是否已经超过当前技能等级的上限
	bool CheckDegree( int nDegree );

	// 生活技能入门
	int InitialLifeSkill( int nLifeSkillType, int nSkillKind );

	// 创造生活技能
	int CreatCompose( int nValue, int *pMaterialIDList, int nLenth, int &rComposeID, int nSkillKind );

	// 检查是否可以升级
	int CanLifeSkillLevelUp( CTemplateLifeSkill *pLifeSkill  );
	
	// 采集
	int Collect( int &rPlayerAP, CTemplateCollect *pCollectNpc, int &rDegreeValue, int nLevel, bool bAddNow = false );	

	// 获取现在的称号ID
	int GetCurrentTitleID();
public:
	// 从pb结构创建技能列表
	void SetFromPB( PBLifeSkill &pbLifeSkill );	   

	// 创建技能列表的pb结构
	void CreatTlvPB( PBLifeSkill &pbLifeSkill );  	
	
};

class CPlayerLifeSkill
{
private:
	CLifeSkill mMainLifeSkill;
	CLifeSkill mSubLifeSkill;
	int mMaterialIndex[MATERIALINDEXNUM];
	int mMaterialID[MATERIALINDEXNUM];
public:
	CPlayerLifeSkill()
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

	int Initialize( )
	{
		for ( int i = 0; i < MATERIALINDEXNUM; ++i )
		{											
			mMaterialIndex[i] = -1;
		}	
		memset( mMaterialID, 0, sizeof( mMaterialID ) );
		return 0;		
	}
	int Resume(){ return 0; }
public:

	// 获取主生活技能
	CLifeSkill *GetMainLifeSkill(){ return &mMainLifeSkill; }

	// 获取辅修生活技能
	CLifeSkill *GetSubLifeSkill(){ return &mMainLifeSkill; }  

	// 获取当前可学的技能种类
	int	GetCurrentSkillKind( );								  

	// 遗忘某项生活技能
	int ForgetLifeSKill( int nLifeSkillKind );

	// 根据生活技能类型获取生活技能
	CLifeSkill *GetLifeSkillByType( int nLifeSkillType );

	// 根据生活技能主辅种类获取生活技能
	CLifeSkill *GetLifeSkillByKind( int nLifeSkillKind );

	// 放置材料
	int AddMaterial( int nBagIndex, int nPosIndex, int nItemID, int &rPreMaterialIndex );

	// 取下材料
	int TakeMaterial( int nPosIndex, int &rBagIndex );

	// 创造配方
	int CreatComPose( int nLifeSkillType,  int nValue, int &rComposeID, int nSkillKind );
	
	// 学习配方
	int StudyCompose( int nComposeID );
	
	// 获取配方材料的列表
	int GetMaterialList( int *pIDList, int &rNum );
	
	// 清除材料信息
	int ClearMaterial();  	
public:
	void SetFromPB( PBLifeSkillBox & pbLifeSkillBox );
	void CreatTlvPB( PBLifeSkillBox & pbLifeSkillBox );
};

// 密码保护相关

class CRolePassword
{
public:
	char mPassword[MAX_PASSWORD_LENGTH];	// 密码
	int  mStatus;							// 当前密码状态
	int	 mProtectTime;						// 安全保护时间
	int	 mProtectTimeLength;				// 安全保护时间长,客户端设置
	int  mEntityID;							// 玩家对象ID
public:
	CRolePassword()
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
	int Initialize()
	{
		memset( mPassword, 0, sizeof( mPassword ) );
		mStatus = EM_PASSWORD_NOTSET;
		mProtectTime = 0;
		mProtectTimeLength = 0; 
		mEntityID = 0;
		return SUCCESS;
	}
	
	int Resume( ){ return SUCCESS; }
	
public:
	// 设置&获取密码
	int SetPassword( const char *pPassword );
	
	const char *GetPassword(){ return mPassword; }		
	
	// 设置&获取密码状态
	void SetStatus( int nStatus ){ mStatus = nStatus; }
	int GetStatus();
	
	// 设置&获取安全保护时间
	void SetProtectTime( int nProtectTime ){ mProtectTime = nProtectTime; }
	int GetProtectTime(  ){ return mProtectTime; }
	
	// 设置&获取安全保护时长
	void SetProtectTimeLength( int nProTimeLength ){ mProtectTimeLength = nProTimeLength; }
	int	GetProtectTimeLength( ){ return mProtectTimeLength; }

	// 设置玩家指针
	void SetEntityID(int nEntityID) { mEntityID = nEntityID; }
	int GetEntityID() { return mEntityID; }
public:	
	
	// 初始化密码状态
	int InitialPass( const char *pPassword, int nProtectTime, int nProtectTimeLength, int nPwdStatus );	
	
	// 检查是否已经过了安全保护时间
	bool CheckProtectTime();
	
	// 检查是否允许相关操作
	int CheckActionAllowed( EPassWordAction emAction );
	
	// 密码检查
	bool CheckPassword( const char *pPassword );
	
	// 清空密码
	void ClearPassword( );
};

// 时间信息
class CExpTimeInfo
{
public:
	CExpTimeInfo()
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
	~CExpTimeInfo(){}
	
	int Resume()
	{
		return 0;
	}
	
	int Initialize()
	{
		mOffLineTimeLength = 0;
		mLastTimeLeaveGame = 0;
		mEffectiveOffTimeLength = 0;
		mLastEffectiveTimeLength = 0;
		mLastCalculateTime = 0;
		mBlakcPillID = 0;
		mBlackPillsTime = 0;	
		mExpFromOffLine = 0;
		return 0;
	}
public:
	// 设置&获取离线时间
	void SetOffLineTimeLength( int nTimeLength ){ mOffLineTimeLength = nTimeLength; }
	int GetOffLineTimeLength(){ return mOffLineTimeLength; }
	
	// 设置&获取最后一次离开游戏的时间
	void SetLastTimeLeaveGame( int nTime ){ mLastTimeLeaveGame = nTime; }
	int GetLastTimeLeaveGame(){ return mLastTimeLeaveGame; }
	
	// 设置&获取有效离线时间
	void SetEffectiveOffTimeLength( int nTimeLength ){ mEffectiveOffTimeLength = nTimeLength; }
	int GetEffectiveOffTimeLength(){ return mEffectiveOffTimeLength; }
	
	// 设置&获取最后一天计算有效时间时的时间长度
	void SetLastEffectiveTime( int nTimeLength ){ mLastEffectiveTimeLength = nTimeLength; } 
	int GetLastEffectiveTime(){ return mLastEffectiveTimeLength; }	
	
	// 设置&获取上一次计算有效离线时间的时间
	void SetLastCalculateTime( int nTime ){ mLastCalculateTime = nTime; }
	int GetLastCalculateTime(){ return mLastCalculateTime; }
	
	// 设置&获取黑狗丸的时间
	void SetBlackPillTime( int nBlackPillTime ){ mBlackPillsTime = nBlackPillTime; }
	int GetBlackPillTime(){ return mBlackPillsTime; }
	
	// 设置&获取黑狗丸的ID
	void SetBlackPillID( int nID ){ mBlakcPillID = nID; }
	int GetBlackPillID(  ){ return mBlakcPillID; }

	// 设置&获取由离线获得的经验
	void SetExpFromOffLine( int nID ){ mExpFromOffLine = nID; }
	int GetExpFromOffLine(  ){ return mExpFromOffLine; }
public:
	// 增加有效离线时间
	int AddEffectiveOffTimeLength( int nLoginTime, bool bVip = false );
	
	// 兑换有效离线时间
	int ExchangeEffectiveOffLineTime( int nTime );

	// 由离线时间获得经验
	int GetExpFromOffLine( int nLevel );

	// 由比率获得离线经验
	int GetExpByRate( int nExp );
	
	// 黑狗丸时间兑换成经验
	int ExchangeOffLineTimeToExp( int nLevel, int &rExp, int &rOffLineTimeConsumed, int nBlackPillsID = 0 );	
	
	void SetFromPB( PBExpTimeInfo &rPBInfo );
	void CreatTlvPB( PBExpTimeInfo &rPBInfo );
		
private:
	int mOffLineTimeLength;				// 实际离线时间
	int mLastTimeLeaveGame;				// 最后一次离开游戏的时间						
	int mEffectiveOffTimeLength;		// 有效离线时间	
	int mLastEffectiveTimeLength;		// 最后计算有效离线时间时当天内的累加长度		
	int mLastCalculateTime;				// 上一次计算有效离线时间的时间
	int mBlackPillsTime;				// 黑狗丸的时间
	int mBlakcPillID;					// 黑狗丸的模板ID
	int mExpFromOffLine;				// 离线时间折合的经验
};

/************************************************************************/
/* 玩家属性                                                             */
/************************************************************************/

// cd组
class CGroupCD
{
public:
	CGroupCD()
	{
		if( CObj::msCreateMode )
		{
			Initialize();
		}
	}

	void Initialize() 
	{
		mGroupIdx = 0;
		mCDTime = 0;
		mTimerID = INVALID_OBJ_ID;
	}

	// cd索引
	inline int GetCDGroupIdx( ) { return mGroupIdx; }
	inline void SetCDGroupIdx( int nIdx ) { mGroupIdx = nIdx; }

	// 取得和设置指定cd组定时器id
	inline int GetTimerID(  ) { return mTimerID; }
	inline void SetTimerID( int nTimerID ) { mTimerID =nTimerID; }

	// 取得和设置定时器剩余cd时间
	inline int GetCDTime( ) { return mCDTime; }
	inline void SetCDTime( int nCDTime ) { mCDTime = nCDTime; }


private:
	int mGroupIdx;	// 组索引
	int mCDTime;	// cooldown时间
	int mTimerID;	// 定时器ID

};


// cd组管理器
class CGroupCDList
{
private:
	CGroupCD	mGroupCD[CDGROUP];

public:
	
	void Initialize() 
	{
		for( unsigned int i = 0; i < ARRAY_CNT(mGroupCD); i++ )
		{
			mGroupCD[i].Initialize();
			mGroupCD[i].SetCDGroupIdx( i );
		}
	}

	int GetMaxGroupIdx( ) { return ARRAY_CNT(mGroupCD); }

	CGroupCD& GetGroupCD( int nIdx ) { return mGroupCD[nIdx]; }


	// 刷新CD组时间
	void RefreshCDTime();


};

class CWizard
{
public:
	int		mWizardPoint[ WIZARD_NUM ];				// 才华点数
	int		mWizardValue[ WIZARD_NUM ];				// 才华值
	int		mWizardSubmit[ WIZARD_NUM ];			// 才华提交量
	int		mWizardIncrease[ WIZARD_NUM ];			// 才华加点值
	int		mTrend;									// 成长倾向

	int		mCombatFame;							// 竞技声望
	int		mRepeFame;								// 副本声望
	int		mCommonFame;							// 历练声望
	int		mCombatToday;							// 竞技今日
	int		mRepeToday;								// 副本今日
	int		mCommonToday;							// 历练今日
	int		mCombatAll;								// 竞技总获得
	int		mRepeAll;								// 副本总获得
	int		mCommonAll;								// 历练总获得

public:
	void Initial( );

	void CreateFromTlv( PBWizard *pWizard );

	void GetTlvBuffer( PBWizard *pWizard );

	// 才华点数增加
	int AddPoint( int vType );

	// 才华值增加,
	void AddValue( int vType, int vValue );

	int Submit( );

	void Clear( );

	inline int GetWizardPoint( int vType )
	{ 
		if ( vType < 0 || vType > WIZARD_NUM )
		{
			return 0;
		}
		return mWizardPoint[ vType ];
	}
	inline int GetWizardValue( int vType )
	{ 
		if ( vType < 0 || vType > WIZARD_NUM )
		{
			return 0;
		}
		return mWizardValue[ vType ];
	}
	inline int GetWizardSubmit( int vType )
	{ 
		if ( vType < 0 || vType > WIZARD_NUM )
		{
			return 0;
		}
		return mWizardSubmit[ vType ];
	}
	inline int GetWizardIncrease( int vType )
	{ 
		if ( vType < 0 || vType > WIZARD_NUM )
		{
			return 0;
		}
		return mWizardIncrease[ vType ];
	}
	inline void SetWizardSubmit( int vType, int vValue )
	{
		if ( vType < 0 || vType > WIZARD_NUM )
		{
			return;
		}
		mWizardSubmit[ vType ] = vValue;
	}
	inline int GetTrend( ){ return mTrend; }
	inline void SetTrend( int vTrend ){ mTrend = vTrend; }

	inline int GetCombatFame( ){ return mCombatFame; }
	inline void SetCombatFame( int vCombatFame ){ mCombatFame = std::min( vCombatFame, FAME_TOTAL_LIMIT ); }
	inline int GetRepeFame( ){ return mRepeFame; }
	inline void SetRepeFame( int vRepeFame ){ mRepeFame = std::min( vRepeFame, FAME_TOTAL_LIMIT ); }
	inline int GetCommonFame( ){ return mCommonFame; }
	inline void SetCommonFame( int vCommonFame ){ mCommonFame = std::min( vCommonFame, FAME_TOTAL_LIMIT ); }

	inline int GetCombatToday( ){ return mCombatToday; }
	inline void SetCombatToday( int vCombatToday ){ mCombatToday = vCombatToday; }
	inline int GetRepeToday( ){ return mRepeToday; }
	inline void SetRepeToday( int vRepeToday ){ mRepeToday = vRepeToday; }
	inline int GetCommonToday( ){ return mCommonToday; }
	inline void SetCommonToday( int vCommonToday ){ mCommonToday = vCommonToday; }

	inline int GetCombatAll( ){ return mCombatAll; }
	inline void SetCombatAll( int vCombatAll ){ mCombatAll = vCombatAll; }
	inline int GetRepeAll( ){ return mRepeAll; }
	inline void SetRepeAll( int vRepeAll ){ mRepeAll = vRepeAll; }
	inline int GetCommonAll( ){ return mCommonAll; }
	inline void SetCommonAll( int vCommonAll ){ mCommonAll = vCommonAll; }

	inline int AddCombatFame( int vDeltaFame )
	{
		if ( vDeltaFame < 0 )
			return 0;
		if ( mCombatToday >= CDataStatic::GetFameLimit( FAME_TYPE_COMBAT ) )
			return 0;
		if ( mCombatFame >= FAME_TOTAL_LIMIT )
			return 0;
		int tOldFame = mCombatFame;
		int tOldToday = mCombatToday;
		int tCombatTemp = std::min( mCombatFame + vDeltaFame, FAME_TOTAL_LIMIT );
		int tDeltaAll = tCombatTemp - tOldFame;
		mCombatToday += tDeltaAll;
		mCombatToday = std::min( mCombatToday, CDataStatic::GetFameLimit( FAME_TYPE_COMBAT ) );
		int tDeltaToday = mCombatToday - tOldToday;		
		mCombatFame += std::min( tDeltaToday, tDeltaAll );
		mCombatAll += std::min( tDeltaToday, tDeltaAll );
		return mCombatFame - tOldFame;
	}

	inline int DelCombatFame( int vDeltaFame )
	{
		if ( vDeltaFame < 0 )
			return 0;
		int tOldFame = mCombatFame;
		mCombatFame -= vDeltaFame;
		mCombatFame = std::max( 0, mCombatFame );
		return mCombatFame - tOldFame;
	}

	inline int AddRepeFame( int vDeltaFame )
	{
		if ( vDeltaFame < 0 )
			return 0;
		if ( mRepeToday >= CDataStatic::GetFameLimit( FAME_TYPE_REPE ) )
			return 0;
		if ( mRepeFame >= FAME_TOTAL_LIMIT )
			return 0;
		int tOldFame = mRepeFame;
		int tOldToday = mRepeToday;
		int tRepeTemp = std::min( mRepeFame + vDeltaFame, FAME_TOTAL_LIMIT );
		int tDeltaAll = tRepeTemp - tOldFame;
		mRepeToday += tDeltaAll;
		mRepeToday = std::min( mRepeToday, CDataStatic::GetFameLimit( FAME_TYPE_REPE ) );
		int tDeltaToday = mRepeToday - tOldToday;		
		mRepeFame += std::min( tDeltaToday, tDeltaAll );
		mRepeAll += std::min( tDeltaToday, tDeltaAll );
		return mRepeFame - tOldFame;
	}

	inline int DelRepeFame( int vDeltaFame )
	{
		if ( vDeltaFame < 0 )
			return 0;
		int tOldFame = mRepeFame;
		mRepeFame -= vDeltaFame;
		mRepeFame = std::max( 0, mRepeFame );
		return mRepeFame - tOldFame  ;
	}

	inline int AddCommonFame( int vDeltaFame )
	{
		if ( vDeltaFame < 0 )
			return 0;
		if ( mCommonToday >= CDataStatic::GetFameLimit( FAME_TYPE_COMMON ) )
			return 0;
		if ( mCommonFame >= FAME_TOTAL_LIMIT )
			return 0;
		int tOldFame = mCommonFame;
		int tOldToday = mCommonToday;
		int tCommonTemp = std::min( mCommonFame + vDeltaFame, FAME_TOTAL_LIMIT );
		int tDeltaAll = tCommonTemp - tOldFame;
		mCommonToday += tDeltaAll;
		mCommonToday = std::min( mCommonToday, CDataStatic::GetFameLimit( FAME_TYPE_COMMON ) );
		int tDeltaToday = mCommonToday - tOldToday;		
		mCommonFame += std::min( tDeltaToday, tDeltaAll );
		mCommonAll += std::min( tDeltaToday, tDeltaAll );
		return mCommonFame - tOldFame;
	}

	inline int DelCommonFame( int vDeltaFame )
	{
		if ( vDeltaFame < 0 )
			return 0;
		int tOldFame = mCommonFame;
		mCommonFame -= vDeltaFame;
		mCommonFame = std::max( 0, mCommonFame );
		return mCommonFame - tOldFame;
	}
};

// 活动需要的今日指引数据
class CTodayGuideData
{
public:
	int		mLevelBak;	// 今日等级提升
	unsigned long long		mExpBak  ;	// 今日经验提升
	int		mHonorBak;	// 今日功勋提升
	int		mAchiveBak;	// 今日成就点数提升
	int		mKillBak ;	// 今日荣誉击杀提升
	CTodayGuideData()
	{
		if ( CObj::msCreateMode )
		{
			mLevelBak = 0;
			mExpBak   = 0;
			mHonorBak = 0;
			mAchiveBak= 0;
			mKillBak  = 0;
		}
	}
	void GetPB( PBGuideData* pData )
	{
		if ( pData != NULL )
		{
			pData->set_achivebak(mAchiveBak);
			pData->set_expbak(mExpBak);
			pData->set_honorbak(mHonorBak);
			pData->set_killbak(mKillBak);
			pData->set_levelbak(mLevelBak);
		}
	}

	void CreateFromPB( PBGuideData* pData )
	{
		if ( pData != NULL )
		{
			mAchiveBak = pData->achivebak();
			mExpBak = pData->expbak();
			mHonorBak = pData->honorbak();
			mKillBak = pData->killbak();
			mLevelBak = pData->levelbak();
		}
	}
};

// 个人次数管理器 管理针对个人的XX次数
typedef lk::hash_map<unsigned int, int, MAX_TIMER_MGR_LINE> PERSONAL_TIMES_MGR_TYPE;
class CPersonalTimesMgr
{
private:
	PERSONAL_TIMES_MGR_TYPE mPersonalData;
public:
	CPersonalTimesMgr()
	{
		if(CObj::msCreateMode)
		{
			mPersonalData.initailize();
		}
	}

	// 返回0 成功
	int Tick(unsigned int vKey)
	{
		PERSONAL_TIMES_MGR_TYPE::iterator it = mPersonalData.find(vKey);
		if ( it == mPersonalData.end() )
		{
			std::pair<PERSONAL_TIMES_MGR_TYPE::iterator, bool> ret = 
				mPersonalData.insert( std::pair<unsigned int, int>(vKey, 1) );
			if (!ret.second)
				return -1;
		}
		else
		{
			++ it->second;
		}
		return 0;
	}

	int GetTimes(unsigned int vKey)
	{
		PERSONAL_TIMES_MGR_TYPE::iterator it = mPersonalData.find(vKey);
		if ( it == mPersonalData.end() )
			return 0;
		return it->second;
	}

	void SetTimes(unsigned int vKey, int nVal)
	{
		PERSONAL_TIMES_MGR_TYPE::iterator it = mPersonalData.find(vKey);
		if ( it == mPersonalData.end() )
		{
			mPersonalData.insert(std::pair<unsigned int, int>(vKey, nVal) );
		}
		else
		{
			it->second = nVal;
		}
	}

	void GetPB( PBTimes* pPB )
	{
		if ( pPB == NULL )
			return;

		for ( PERSONAL_TIMES_MGR_TYPE::iterator it = mPersonalData.begin(); it != mPersonalData.end(); ++it )
		{
			PBSingleTimes* tpPB = pPB->add_timesinfo();
			tpPB->set_key(it->first);
			tpPB->set_value(it->second);

			LOG_DEBUG("pro", "SAVE TIMES key=0x%x val=%d", it->first, it->second );
		}
	}

	void CreateFromPB( PBTimes* pPB )
	{
		if ( pPB == NULL )
			return;
		for ( int i = 0; i < pPB->timesinfo_size(); ++i )
		{
			PBSingleTimes tpPB = pPB->timesinfo(i);
			mPersonalData.insert(std::pair<unsigned int, int>(tpPB.key(), tpPB.value()) );

			LOG_DEBUG("pro", "LOAD TIMES key=0x%x val=%d", tpPB.key(), tpPB.value() );
		}
	}

	static unsigned int MakeKey( unsigned int vType, int nParam )
	{
		unsigned int tRet = vType;
		tRet = tRet << 24;
		tRet |= nParam;
		return tRet;
	}
};

// 每个玩家的活动数据
class  SinglePlayerActivityInfo
{
public:
	int nState;
	int nParam1;
	int nParam2; 
	int nParam3; // 送好礼2期 表示是否已经领取
	int nRealLastedTime; // 真实的持续时间  这个数据在非存盘的地方是只读的
	int nLastedSavedTime; // 本次登录以前的时间 在load数据的时候修改 不存盘
	bool bIsChanged ; // 是否在本次下线之前改过状态
	time_t chg_tm; // 时间戳
	SinglePlayerActivityInfo()
	{
		nState=0;
		nParam1=0;
		nParam2=0; 
		nParam3=0; 
		nRealLastedTime=0; 
		nLastedSavedTime=0;
		chg_tm=time(0);
		bIsChanged=false;
	}
	int GetDuringTime(CEntityPlayer* pPlayer); // 得到从活动开始到现在走过的时间

};

typedef lk::hash_map<int, SinglePlayerActivityInfo, HELPID_SIZE> ACTIVITYINFO_TYPE;

class CPlayerActivityInfo
{
public: 
	// first:活动ID
	ACTIVITYINFO_TYPE mStates;
	time_t			  mLastSave;
	CPlayerActivityInfo()
	{
		mStates.initailize();
		mLastSave=time(0);
	}

	SinglePlayerActivityInfo* GetActivityInfo( int nActivityID )
	{
		ACTIVITYINFO_TYPE::iterator it = mStates.find(nActivityID);
		if ( it == mStates.end() )
		{
			return NULL;
		}
		else
		{
			return &(it->second);
		}
	}

	void UpdateActivityInfo( int nActivityID, SinglePlayerActivityInfo& rInfo )
	{
		ACTIVITYINFO_TYPE::iterator it = mStates.find(nActivityID);
		if ( it == mStates.end() )
		{
			//rInfo.nParam3=0;
			mStates.insert(std::pair<int, SinglePlayerActivityInfo>(nActivityID, rInfo) );
		}
		else
		{
			// 如果状态没变 则参数3还用老的 这么写不好啊。。哎
			if (rInfo.nState == it->second.nState)
				rInfo.nParam3=it->second.nParam3;
			it->second = rInfo;
		}
	}
};

// 聚宝盆的兑换信息
class  CTreasureBowlInfo
{
public:
	typedef lk::hash_map< int, int, TREASUREBOWL_DAILYEXCHANGENUM > CTreasureBowlList;	// 聚宝盆聚宝物品
public:
	enum
	{
		CTREASUREBOWL_TIMELIMIT			= 1,	// 使用物品聚宝时该物品已经达到当然最大次数
		CTREASUREBOWL_TOTALTIMELIMIT	= 2,	// 使用物品聚宝时已经达到最大聚宝次数
	};
public:
	CTreasureBowlInfo()
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
public:
	int  Initialize();
	int	 Resume(){ return 0; }
public:
	// 获取&设置最后一次聚宝的时间
	int GetExchangeTime(){ return mExchangeTime; }
	void SetExchangeTime( int nTime ) { mExchangeTime = nTime; }
	
	// 获取&设置当天聚宝的次数
	int GetTotalNum(){ return mTotalNum; }
	void SetTotalNum( int nNum ){ mTotalNum = nNum; }
public:
	// 刷新聚宝兑换的时间
	int RefreshExchangeTime();
	
	// 检查某种物品是否可以再聚宝
	int CheckExchangeItem( int nTotalMaxTimes, int nMaxTimes, int nItemID ); 	
	
	// 设置用于聚宝的某个物品信息
	int SetExchangeItem( int nItemID );
	
	// 清空玩家的聚宝信息
	int ClearTreasureBowlList();	
	
	// 获取某种物品聚宝信息
	int GetExchangeItem( int nItemID, int &rUsedTimes );
public:
	
	void SetFromPB( PBTreasureBowl &rTreasureBowl );	
	void CreateTlvPB( PBTreasureBowl &rTreasureBowl );	
private:
	int mTotalNum;
	int mExchangeTime;
	CTreasureBowlList mTreasurebowlList;		    
};

//铸灵的系别信息
class CSpiritTypeInfo
{
private:
	int	m_anSlotOpenStatus[MAX_CASTINGSPIRIT_SLOT];
	int	m_anCastingItemID[MAX_CASTINGSPIRIT_SLOT];
	int	m_anBindStatus[MAX_CASTINGSPIRIT_SLOT];

public:
	CSpiritTypeInfo();
	// 获取、设置开孔状态
	int GetSlotOpenStatus(int nSlotIndex);
	void SetSlotOpenStatus(int nSlotIndex, int nStatus);

	// 获取、设置铸灵用的道具id
	int GetCastingItemID(int nSlotIndex);
	void SetCastingItemID(int nSlotIndex, int nItemID);	

	// 获取、设置真灵道具的绑定状态
	int GetBindStatus(int nSlotIndex);
	void SetBindStatus(int nSlotIndex, int nBindStatus);
};

// 铸灵的部位信息
class CSpiritPositionInfo
{
private:
	CSpiritTypeInfo m_SpiritTypeInfo[MAX_CASTINGSPIRIT_TYPE];

public:
	CSpiritPositionInfo();
	// 获取、设置开孔状态
	int GetSlotOpenStatus(int nTypeIndex, int nSlotIndex);
	void SetSlotOpenStatus(int nTypeIndex, int nSlotIndex, int nStatus);

	// 获取、设置铸灵用的道具id
	int GetCastingItemID(int nTypeIndex, int nSlotIndex);
	void SetCastingItemID(int nTypeIndex, int nSlotIndex, int nItemID);

	// 获取、设置铸灵道具的绑定状态
	int GetBindStatus(int nTypeIndex, int nSlotIndex);
	void SetBindStatus(int nTypeIndex, int nSlotIndex, int nBindStatus);
};

class CSpiritInfo
{
private:
	CSpiritPositionInfo m_PositionInfo[MAX_CASTINGSPIRIT_POSITION];

public:
	// 获取、设置开孔状态
	int GetSlotOpenStatus(int nPositionIndex, int nTypeIndex, int nSlotIndex);
	void SetSlotOpenStatus(int nPositionIndex, int nTypeIndex, int nSlotIndex, int nStatus);

	// 获取、设置铸灵用的道具id
	int GetCastingItemID(int nPositionIndex, int nTypeIndex, int nSlotIndex);
	void SetCastingItemID(int nPositionIndex, int nTypeIndex, int nSlotIndex, int nItemID);
	
	// PB和内存数据转换
	void SetFromPB(PBSpiritInfo& rPBSpiritInfo);	
	void CreateTlvPB(PBSpiritInfo& rPBSpiritInfo);

	// 得到各系别属性影响值
	int GetEffectValue(int nSpiritType);

	// 获取、设置铸灵道具的绑定状态
	int GetBindStatus(int nPositionIndex, int nTypeIndex, int nSlotIndex);
	void SetBindStatus(int nPositionIndex, int nTypeIndex, int nSlotIndex, int nBindStatus);

	// 得到总评分
	int GetScore();

	// 得到开孔数、已铸灵孔的数量
	int GetSlotInfo(int& rnOpenedSlotCount, int& rnCastedSlotCount);
};

class CPropertyPlayer : public CPropertyCharacter
{
public:
	typedef lk::vector< unsigned int , ITEM_FREE_COUNT > ItemForFreeMap;

	// 出售给NPC道具列表
	typedef lk::vector< SaleItem, ITEM_BUYBACK_MAX > SaleItemList;

	typedef lk::set< int, (CARDBOOK_TYPE * CARDBOOK_NUM) > CardList;

	struct ST_TITLE
	{
		int mTitleID;
		int mTime;
		ST_TITLE(){ mTitleID = 0; mTime = 0; }
	};

	typedef lk::vector< ST_TITLE, TITLEMAX_SELF > TitleList;
	typedef lk::vector<ComplexPos, TELE_LIST_SIZE> TELE_LIST_TYPE;

	struct ST_AWARD
	{
		int mAwardID;
		int mTimes;
		ST_AWARD(){ mAwardID = 0; mTimes = 0; }
		ST_AWARD( int vAwardID, int vTimes ){ mAwardID = vAwardID; mTimes = vTimes; }
	};
	typedef lk::vector< ST_AWARD, MAX_AWARD_NUM > AwardList;

	typedef lk::vector< int, MAX_MATRIX_MENBER > MatrixList;
protected:
	unsigned long long				mExp;				// 经验
	unsigned short					mSex;				// 性别
	unsigned short					mRemainPoint;		// 剩余属性点
	unsigned short					mSkillPoint;		// 剩余技能点
	unsigned short					mCurStr;			// 力量
	unsigned short					mCurWis;			// 智力
	unsigned short					mCurSta;			// 耐力
	unsigned short					mCurCon;			// 体质
	unsigned short					mCurSpr;			// 精神
	int								mMoney;				// 金币
	int								mBindMoney;			// 綁定金錢
	int								mStorageMoney;		// 仓库存储金钱
	int								mStorageBindMoney;	// 仓库存储绑定金钱
	unsigned int					mTalkNpcEntityID;	// 玩家当前对话NPCID
	EService						mServiceStatus;		// 玩家服务状态
	CProduce						mProduce;			// 玩家生产状态
	CGroupCDList					mCDGroup;			// CD组
	int             				mCommonCD;			// 公共冷却时间
	CEquipment						mEquipment;			// 装备
	CBaggage						mBaggage;			// 包裹
	CItemBox						mTaskBaggage;		// 任务道具包裹
	CStorage						mStorage;			// 仓库
	CSkillListInstance<11>			mEquipSkill;		// 装备技能列表
	CShortcutList					mShortcut;			// 玩家快捷栏
	unsigned int					mProLevel;			// 生产等级
	unsigned int					mProExp;			// 生产经验
	ComplexPos						mTownPos;			// 回城点坐标		
	TELE_LIST_TYPE					mTeleList;	// 传送道具传送点列表		
	ItemForFreeMap                  mItemForFreeList;	// 存放处于绑定解除状态的物品的指针
	int								mStorageStatus;		// 仓库状态
	int								mHonor;				// 功勋
	int								mTodayHonor;		// 今日功勋
	int								mInHonPerDay;		// 当天获得的功勋值
	int								mOutHonPerDay;		// 每天提供的功勋数
	int								mLastObjID;			// 上一次插入的物品
	int								mSaveDate;			// 记录上一次刷新的日期
	KillData						mKill[ 30 ];		// 被杀信息
	int								mDiploidTime;		// 领取的双倍经验时间杀过的历史记录
	CPlayerRepetion					mRepetion;
	int								mValidDiploid;		// 有效的双倍时间
	int								mDailyWelfareStatus;// 是不是已经领取了每日福利的状态标志		
	int								mHasRedStoneStatus;	// 是否具有内丹状态的标志
	unsigned int					mRedStoneID;		// 如果使用内丹并且处于读秒状态时为内丹ID，否则为0
	int								mAnswerStatusObjID;	// 	
	int								mKillNumOut;		// 杀敌数(在国外)
	int								mKillNumIn;			// 杀敌数(在国内)
	int								mLastRefreshTime;	// 上次刷双时间	
	int								mCompletedTasks;	// 完成任务的数量
	int								mDieNum	;			// 死亡次数
	CPlayerLifeSkill				mPlayerLifeSkill;	// 玩家生活技能	
	CSkillListInstance<250>			mSkill;				// 技能列表		为妖魔志增加到250
	CTreasureBowlInfo				mTreasureBowlInfo;	// 聚宝盆信息
	CSpiritInfo						m_SpiritInfo;		// 聚灵信息
public:																
	bool							mYellowStatus;			// 是否黄名
	// 内挂时需要补充的药品模板ID
	bool							mInAutoPlay;			// 是否自动挂机
	int								mDiploidState[ MAX_DIPLOIDSTATE ];	// 0:是否领双状态;1:是否冻结;2:计时器ID;
																		// 3:计时器停止时间(毫秒);4:领取的剩余双倍时间 
	SaleItemList					mSaleItem;				// 售出给NPC的道具
	CStallInfo						mStallInfo[3];				// 摆摊记录

	int								mResetPPNum;			// 当前重置属性点次数
	int								mResetSPNum;			// 当前重置技能点次数
	int								mLastResetPPIndex;		// 最后一次重置属性点活动的INDEX
	int								mLastResetSPIndex;		// 最后一次重置技能点活动的INDEX
	int								mReduceLevel;			// 降低装备需求等级
	StateList						mAttackList;			// 击中的特殊状态
	StateList						mDefenceList;			// 被击中的特殊状态
	StateList						mDeathList;				// 暴击的特殊状态
	StateList						mReduceHPList;			// 减血的特殊状态
	int								mAttackStateTime;		// 击中特殊效果的冷却时间
	int								mDefenceStateTime;		// 被击中特殊效果的冷却时间
	int								mDeathStateTime;		// 暴击特殊效果的冷却时间
	int								mReduceHPStateTime;		// 减血特殊效果的冷却时间
			
	int								mLastFamilyApplyTime;	// 最后申请加入家族的时间
	int		 						mLastTimeLeaveFamily;	// 最后一次离开家族的时间	

	int								mPKValue;				// PK 值
	EPKStatus						mPKStatus;				// PK状态(白，蓝，黄，红)
	int								mYellowTimer;			// 黄名倒计时
	CardList						mActiveCardList;		// 已激活的卡片列表	
	CardList						mOwnCardList;			// 已收集的卡片列表
	int								mPrisonTime;			// 在监狱待的游戏时间
	int								mPrisonTotalTime;		// 监狱囚禁期限
	unsigned int					mLastWeeklyDisposTime;	// 上次处理每周事件时间
	unsigned int					mLastDaylyDisposTime;	// 上次处理每日事件时间

	// 法宝相关
	unsigned char					mAssignExpScl;			// 打怪经验分配给法宝 百分比

	int								mTalentExp;				// 修为值
	int								mTalentPoint;			// 造化点
	CSkillListInstance<100>			mTalentSkill;			// 造化技能
	int								mTalentStep;			// 当前造化小阶
	int								mTalentStudy;			// 当前小阶已投入的造化点
	int								mTalentLevel;			// 总造化等级

	int								mCurrentTitle;			// 当前称号
	TitleList						mTitleList;// 称号列表
	CSkillListInstance<6>			mTitleSkill;			// 称号技能
	int								mLastRefreshApTime;		// 上一次刷新AP的时间

	CYuanBao						mGoldenYB;				// 金元宝
	CYuanBao						mSilverYB;				// 银元宝


	// 排行榜要统计的项目：
	int								mOriginReliveTimes;		// 原地复活次数
	int								mKillSameNationNum;		// 杀本阵营人数
	int								mKillOgreNum;			// 杀怪数
	int								mUpFailNum;				// 精炼失败次数
	int								mTodayKillEnemyNumIn;		// 今日在国内杀敌的数量(护国)
	int								mTodayKillEnemyNumOut;		// 今日在国内杀敌的数量(出征)
	
	int								mDecompoundEquipIndex;		// 待分解的装备索引

	// 才华系统的属性
	CWizard							mWizard;

	int								mLastJudgeTime;			// 上次评价时间

	int								mVipFlag;				// 会员标示：普通会员、中级会员、高级会员
	int								mVipEndDate;			// 会员到期日期
	AwardList						mAwardList[EALT_Max];	// 奖励列表
	MatrixList						mMatrixList;			// 后辈名单
	int								mMasterID[ MAX_MASTER_NUM ];  // 前辈ID
	int								mSlaveValue[ MAX_MATRIX_LEVEL ];// 仙缘数值
	int								mSlavePoint;			// 仙缘积分
	int								mMasterPoint;			// 师德积分
	int								mSlaveNum;				// 出师数量
	int								mSlaveTotal;			// 仙缘总计
	int								mMasterTotal;			// 师德总计
	int								mMasterPercent[ MAX_MASTER_NUM ];	// 感恩比例
	int								mBeginnerNum;			// 新手弟子数量
	int								mFormalNum;				// 正式弟子数量
	int								mMasterTimes;			// 拜师次数
	int								mMasterGive;			// 总计给出师德
	CRolePassword					mRolePassword;			// 角色密码
	int								mCharmValue;			// 魅力值
	int								mWeekIncCharm;			// 本周累计获得魅力值
	int								mLastWeekIncCharm;		// 上周累计获得魅力值
	CTodayGuideData					mGuideData;				// 今日指引数据
	CPersonalTimesMgr				mTimesMgr;			// 个人次数管理器
	
	int								mProgressFunc;		// 现在正在读秒的功能模块
	uint64_t						mProgressStartTime;	// 读秒开始的的时间
	int								mInvestStatus;		// 调查问卷状态
	int								mTotalWar;			// 国战累计战功
	int								mTodayWar;			// 本次国战战功
	int								mTodayOther;		// 本日其它战功
	int								mFreeWar;			// 累计可支配战功
	int								mWarKill;			// 国战荣誉击杀
	int								mWarSerial;			// 当前国战计次
	int								mExpHonor;			// 今日已兑换经验荣誉
	int								mItemHonor;			// 今日已兑换物品荣誉
	int								mHasVoted;			// 今日已投票弹劾

	CPlayerExchange				mExange;
	CExpTimeInfo				mExpTimeInfo;			// 离线经验信息
	CPlayerActivityInfo			mActivityInfo;			// 活动信息

    int                         miIsShowFashion;        // 是否显示时装
    int                         miIsShowEquip;          // 是否可查询装备信息

public:
    inline void SetIsShowFashion( int nIsShowFashion ){ miIsShowFashion = nIsShowFashion; }
    inline int  GetIsShowFashion(  ) { return miIsShowFashion; }
    inline void SetIsShowEquip( int nIsShowEquip ){ miIsShowEquip = nIsShowEquip; }
    inline int  GetIsShowEquip(  ) { return miIsShowEquip; };

	CPlayerExchange* GetPlayerExchange( ){ return &mExange; }

	inline void SetTotalPrisonTime( int nTime ) { mPrisonTotalTime = nTime; }
	inline int  GetTotalPrisonTime() { return mPrisonTotalTime; }

	inline void SetLastJudgeTime( int nTime ){ mLastJudgeTime = nTime; }
	inline int  GetLastJudgeTime(){ return mLastJudgeTime; }

	inline void SetResetPPNum( int nNum ) { mResetPPNum = nNum; }
	inline void SetResetSPNum( int nNum ) { mResetSPNum = nNum; }
	inline void SetLastResetSPIndex( int nIdx ) { mLastResetSPIndex = nIdx; }
	inline void SetLastResetPPIndex( int nIdx ) { mLastResetPPIndex = nIdx; }

	inline int GetResetPPNum() { return mResetPPNum; }
	inline int GetResetSPNum() { return mResetSPNum; }
	inline int GetLastResetSPIndex() { return mLastResetSPIndex; }
	inline int GetLastResetPPIndex() { return mLastResetPPIndex; }
	CPlayerLifeSkill *GetPlayerLifeSkill(){ return &mPlayerLifeSkill; }

	inline void SetAutoPlayFlag( bool nFlag ){ mInAutoPlay = nFlag; }
	inline bool GetAutoPlayFlag() { return mInAutoPlay; }

	inline int GetOriginReliveTimes() { return mOriginReliveTimes;}
	inline void SetOriginReliveTimes( int nValue) { mOriginReliveTimes = nValue;}

	inline int GetKillSameNationNum() { return mKillSameNationNum;}
	inline void SetKillSameNationNum( int nValue) { mKillSameNationNum = nValue;}

	inline int GetKillOgreNum() { return mKillOgreNum;}
	inline void SetKillOgreNum( int nValue) { mKillOgreNum = nValue;}

	inline int GetUpFailNum() { return mUpFailNum;}
	inline void SetUpFailNum( int nValue) { mUpFailNum = nValue;}

	inline int GetTodayKillEnemyNumIn() { return mTodayKillEnemyNumIn; }
	inline int GetTodayKillEnemyNumOut() { return mTodayKillEnemyNumOut; }
	inline void SetTodayKillEnemyNumIn(int nValue) { mTodayKillEnemyNumIn = nValue; }
	inline void SetTodayKillEnemyNumOut(int nValue) { mTodayKillEnemyNumOut = nValue; }

	inline int GetCharmValue() { return mCharmValue; }
	inline void SetCharmValue( int nVal ) { mCharmValue = nVal; }

	inline int GetWeekIncCharm() { return mWeekIncCharm; }
	inline void SetWeekIncCharm( int nVal ) { mWeekIncCharm = nVal; }

	// 获得上周的数据
	inline int GetLastWeekIncCharm() { return mLastWeekIncCharm; }
	inline void SetLastWeekIncCharm( int nVal ) { mLastWeekIncCharm = nVal; }
	
	// 设置&获取正在读秒的功能模块
	void SetProgressFunc( int nProgressFunc ){ mProgressFunc = nProgressFunc; }
	int	GetProgressFunc(  ){ return mProgressFunc; } 

	// 设置&获取读秒的开始时间
	void SetProgressStartTime( uint64_t rTime ){ mProgressStartTime = rTime; }
	uint64_t GetProgressStartTime(){ return mProgressStartTime; }
	void ClearPropTelet(){ mTeleList.clear();}
	
	// 检查读秒是否正确
	bool CheckProgressTime( int nFuncType, int nNetSpeed, bool bUseConfigTime = true, int nProgressTime = 0 );

	inline CTodayGuideData* GetGuideData() { return &mGuideData; }
	inline CPersonalTimesMgr* GetTimesMgr() { return &mTimesMgr; }
	// 按照数据库结构存盘


	// 基本信息blob
	char* GetRoleBasicBlobStr( );
	void CreateBasicInfoFromTlv( PBPlayerBasicInfo* pBasicInfo );
	void GetBasicInfoTlvBuffer( PBPlayerBasicInfo* pBasicInfo );

	// 存储魅力系统相关数据
	void CreateCharmInfoFromTlv( PBCharm* pInfo );
	void GetCharmInfoTlvBuffer( PBCharm* pInfo );
	void CreateTimesInfoFromTlv( PBTimes* pInfo );
	void GetTimesInfoTlvBuffer( PBTimes* pInfo );

	// 技能blob
	char* GetRoleSkillBlobStr(	);

	// Buff blob
	char* GetRoleBuffBlobStr( );

	// 包裹 blob
	char* GetRolePackageBlobStr( );

	// 装备blob
	char* GetRoleEquipBlobStr( );

	// 仓库blob
	char* GetRoleStorageBlobStr( );

	// 任务blob
	char* GetRoleTaskPackageBlobStr( );

	// CD组blob
	char* GetRoleCDGroupBlobStr( );
	void CreateCDGroupFromTlv( PBPlayerCDGroupInfo* pCDGroup );
	void GetCDGroupTlvBuffer( PBPlayerCDGroupInfo* pCDGroup );

	// 传送信息blob
	char* GetRoleTelePosBlobStr( );
	void CreateTelePosFromTlv( PBTelePosInfo* pTelePos );
	void GetTelePosTlvBuffer( PBTelePosInfo* pTelePos );

	// PVP blob
	char* GetRolePvPBlobStr( );
	void CreatePvPBlobInfoFromTlv( PBPlayerPVPInfo* pPvPInfo );
	void GetPvPBlobInfoTlvBuffer( PBPlayerPVPInfo* pPvPInfo );

	// Title blob
	char* GetRoleTitleBlobStr( );
	void CreateTitleInfoFromTlv( PBPlayerTitleInfo* pTitleInfo );
	void GetTitleInfoTlvBuffer( PBPlayerTitleInfo* pTitleInfo ); 

	// Activity blob
	char* GetRoleActivityBlobStr( );
	void CreateActivityInfoFromTlv( PBPlayerActivityInfo* pActivityInfo );
	void GetActivityInfoTlvBuffer( PBPlayerActivityInfo* pActivityInfo, bool bRealSave=false );

	SinglePlayerActivityInfo* GetActivityInfo( int nActivityID )
	{
		return mActivityInfo.GetActivityInfo( nActivityID );
	}

	void UpdateActivityInfo( int nActivityID, SinglePlayerActivityInfo& rInfo )
	{
		mActivityInfo.UpdateActivityInfo( nActivityID, rInfo );
	}

	// 造化blob
	char* GetRoleTalentBlobStr( );
	void CreateTalentInfoFromTlv( PBPlayerTalentInfo* pTalentInfo );
	void GetTalentInfoTlvBuffer( PBPlayerTalentInfo* pTalentInfo );

	// mw blob
	char* GetRoleMWBlobStr( );
	void CreateMWInfoFromTlv( PBPlayerMWInfo* pMWInfo );
	void GetMWInfoTlvBuffer( PBPlayerMWInfo* pMWInfo );

	// 生活技能blob
	char* GetRoleLifeSkillBlobStr( );


	// 妖魔志blob
	char* GetRoleMonsterBlobStr( );
	void CreateMonsterInfoFromTlv( PBMonsterRecordInfo* pMonster);
	void GetMonsterRecordInfoTlvBuffer( PBMonsterRecordInfo* pMonster );

	// 统计信息blob
	char* GetRoleStatisticBlobStr( );
	void CreateStatisticFromTlv( PBPlayerStatisticInfo* pStatistic );
	void GetStatisticTlvBuffer( PBPlayerStatisticInfo* pStatistic  );

	// 才华 blob
	char* GetRoleWizardBlobStr( );

	// 客户端blob信息
	char* GetRoleClientInfoBlobStr( );
	void CreateRoleClientInfoFromTlv( PBPlayerClientInfo* pClientInfo );
	void GetRoleClientInfoTlvBuffer( PBPlayerClientInfo* pClientInfo );
	
	// 获取安全密码相关的信息
	CRolePassword *GetRolePassword(){ return &mRolePassword; }

	void CreateMatrixInfoFromTlv( PBMatrixInfo* pMatrix);
	void GetMatrixRecordInfoTlvBuffer( PBMatrixInfo* pMatrix );	

	// 获取离线经验相关
	CExpTimeInfo *GetExpTimeInfo(  ){ return &mExpTimeInfo; }
public:

	CPropertyPlayer( ){ }
	virtual ~CPropertyPlayer( ){ }

	int Initial( );
	int Final( ); 
	// 检查CD组
	//int CheckCDGroup( int *vCDGroup, int vNum );

	// 设置CD组
	//int RefreshCDGroup( int *vCDGroup, int vNum );
	
	// 密码检查
	bool CheckPassWord( const char *pPassWord );	

	// 上马
	int RideHorse( unsigned int vHorseTempID );

	// 下马 (需要提供绑定的BUFF ID)
	int OffHorse(int nHorseBuffID);

	// 获得装备增加的技能等级
	unsigned short GetIncSkillLevel( unsigned short vSkillID );

	// 检查包裹是否有足够空间
	bool TestBaggageSpace( int* pItemID, int* pNumber, int vNumber );

	// 检查包裹是否有指定数量的某种物品
	bool BaggageHasItem( unsigned int vItemID, unsigned int vNumber );

	// 获得包裹有指定类型物品的个数
	int BaggageHasItem( unsigned int vItemID );

	// 根据技能ID获得技能
	CSkillObject* GetSkillObjectByID( unsigned int vSkillID );

	// 根据技能index获得技能
	CSkillObject* GetSkillObjectByIndex( unsigned int unSkillIndex );
	CSkillObject* GetSkillObjectBySkillIDInMagicWeapon( int vSkillID );

	// 移动物品
	int MoveItem( unsigned int vSrcIndex, unsigned int vSrcSlot, unsigned int vDesIndex, unsigned int vDesSlot, unsigned int vNumber, int &rNum );

	// 向包裹插入物品
	int InsertBaggageItem( CItemObject* pItem, int &vPos, int *vIndex, int *vNumber, int &vLen );

	// 从包裹移除指定位置的物品
	int RemoveBaggageItem( unsigned int vIndex, unsigned int vNumber = 0 );

	// 从仓库移除指定位置的物品
	int RemoveStorageItem( unsigned int vIndex, unsigned int vNumber = 0 );

	// 放上装备
	int Equip( unsigned int vSrcIndex, unsigned int &vDesIndex );

	// 卸下装备
	int UnEquip( unsigned short vPart, unsigned int vIndex = ~0 );

	// 从PB结构创建
	void CreateFromTlv2( PBProperty* pBuffer );

	// 填充部分PB结构，不包括包裹，仓库
	void GetTlvBuffer2( PBProperty* pBuffer );

	// 根据发送次序填充PB结构 
	void GetTotalBufferForClient( PBProperty* pBuffer, int nFillOder );

	// 重置所有属性点
	int ResetAllPP( );

	// 重置所有技能点
	int ResetAllSP( );

	// 重置所有造化点
	int ResetAllTalent( int vStep );

	// 重置单个技能点
	int ResetSingleSP( unsigned short vGenerID, unsigned char vCol, unsigned char vRow );

	// 重置职业
	int ResetMetier( );

	// 改变职业
	void ChangeMetier( int vMetierID );

	// 玩家点属性点
	int ClickPropertyPoint( EPropertyType vPropertyType );

	// 玩家点技能点
	int ClickSkillPoint( unsigned short vGenerID, unsigned char vCol, unsigned char vRow, unsigned int& vSkillID, int& vIndex, int& vMoney, int& vBindState );

	// 是否可以重置属性点
	bool IsSPResetable( unsigned short vGenerID, unsigned char vCol, unsigned char vRow );

	// 技能点是否是初始值
	bool IsSPInit();

	// 属性点是否是初始值
	bool IsPPInit();

	// 玩家升级，默认升1级
	void LevelUp( int vLevel = 1 );

	// 处理冷却组事件
	//int CDGroupCoolingEvent( CEntity* pEntity, unsigned int vTickOffSet );

	// 处理红黄名事件
	int PKTimeEvent( CEntity* pEntity, unsigned int vTickOffSet );	

	// 处理生产升级等事件
	int LogicTimeEvent(	CEntity* pEntity, unsigned int vTickOffSet );


	
	// 获取处于解除绑定状态的物品
	void GetItemForFree( );
	
	// 获取处于解除绑定状态的物品
	void GetItemForFreeFromItemBox( CItemBox &vItemBox );
	
	// 获取处于解除绑定状态的物品
	void GetItemForFreeFromEquipmet();

	// 增加处于绑定解除状态的物品
	void InsertItemForFreeIntoList( CItemObject *tpItemObj );

	// 判断是否应该解除绑定
	void CheckItemList( int ItemNow );

	//// 给客户端发送仓库信息
	//void SendStorageInfo( CEntityPlayer *tpPlayer );

	// 添加属性点
	int AddPropertyPoint( int vSTR, int vCON, int vSTA, int vWIS, int vSPR );

	// 法宝获得技能相关加成
	unsigned int MWGetFixNumber( CTemplateSkill::PASSINC_TYPE vType );
	int MWGetPerNumber( CTemplateSkill::PASSINC_TYPE vType );

	// 得到才华信息
	CWizard& GetWizard( ) { return mWizard; }

	// 刷新预存储的属性
	virtual CPropertyValue RefreshMaxHP( );
	virtual CPropertyValue RefreshStr( );
	virtual CPropertyValue RefreshWis( );
	virtual CPropertyValue RefreshSpr( );
	virtual CPropertyValue RefreshSta( );
	virtual CPropertyValue RefreshCon( );
	virtual CPropertyValue RefreshMaxMP( );
	virtual CPropertyValue RefreshMaxAP( );
	virtual ATTACK_TYPE RefreshAttackType( );

	virtual CPropertyValue RefreshPAttackMax( );
	virtual CPropertyValue RefreshPAttackMin( );
	virtual CPropertyValue RefreshMAttackMax( );
	virtual CPropertyValue RefreshMAttackMin( );

	virtual CPropertyValue RefreshDeathRate( ); // 致命

	virtual CPropertyValue RefreshDeathValue( );
	virtual CPropertyValue RefreshDeathDefense( );
	virtual CPropertyValue RefreshNormalDuck( );
	virtual CPropertyValue RefreshSkillDuck( );
	virtual CPropertyValue RefreshIgnoreDuck( );

	virtual CPropertyValue RefreshPDefence( );
	virtual CPropertyValue RefreshMDefence( );

	virtual int RefreshAddFixDamage( );
	virtual int RefreshDecFixPDamage(  );
	virtual int RefreshDecFixMDamage( );
	virtual int RefreshDecPerPDamage( );
	virtual int RefreshDecPerMDamage( );
	virtual int RefreshStealHP( );
	virtual int RefreshStealMP( );
	virtual int RefreshNormalHitRate( );
	virtual int RefreshSkillHitRate( );
	virtual int RefreshResDef( int *vDef );
	virtual int RefreshResAtt( int *vAtt );
	virtual int RefreshEquipWizard( int *vWizard );
	void RefreshSkillList( );
	void RefreshSkillListAction( CSkillList* pSkillList );
	void RefreshStateList( );
	virtual int	GetDuckRate( );

	// 定时器相关 最后参数：法宝对象ID 
	bool OnSkillCooldownTimeOut( CEntityPlayer* pEntity, int nSkillID, int nSkillLevel, int nSkillCD = 0, int nMWObjID = 0  );

	// 启动玩家绑定的定时器
	virtual void StartSavedTimer( ) ;

    //技能cd 刷新  add by ycx  20100630
    bool OnSkillCDRefresh( CSkillObject* pSkillObj, CEntityPlayer* pDesEntity);
    //cd组 刷新   add by ycx  20100630
    bool OnGroupCDRefresh(CGroupCD& rGroupCD, CEntityPlayer* pDesEntity);

public:

	inline int GetLastRefreshAPTime(){ return mLastRefreshApTime; }
	inline void SetLastRefreshAPTime( int nTime ) { mLastRefreshApTime = nTime; }
	
	// Get & Set Exp
	inline unsigned long long GetExp( ){ return mExp; }
	inline void SetExp( unsigned long long vExp ){ mExp = vExp; }

	// Get & Set Sex
	inline unsigned int GetSex( ){ return mSex; }
	inline void SetSex( unsigned int vSex ){ mSex = vSex; }

	// Get & Set RemainPoint
	inline short GetRemainPoint( ){ return mRemainPoint; }
	inline void SetRemainPoint( short vRemainPoint ){ mRemainPoint = vRemainPoint; }

	// Get & Set SkillPoint
	inline short GetSkillPoint( ){ return mSkillPoint; }
	inline void SetSkillPoint( short vSkillPoint ){ mSkillPoint = vSkillPoint; }

	// Get & Set CurStr
	inline short GetCurStr( ){ return mCurStr; }
	inline void SetCurStr( short vCurStr ){ mCurStr = vCurStr; }

	// Get & Set CurWis
	inline short GetCurWis( ){ return mCurWis; }
	inline void SetCurWis( short vCurWis ){ mCurWis = vCurWis; }

	// Get & Set CurSta
	inline short GetCurSta( ){ return mCurSta; }
	inline void SetCurSta( short vCurSta ){ mCurSta = vCurSta; }

	// Get & Set CurCon
	inline short GetCurCon( ){ return mCurCon; }
	inline void SetCurCon( short vCurCon ){ mCurCon = vCurCon; }

	// Get & Set CurSpr
	inline short GetCurSpr( ){ return mCurSpr; }
	inline void SetCurSpr( short vCurSpr ){ mCurSpr = vCurSpr; }

	// Get & Set Money
	inline int GetMoney( ){ return mMoney; }
	inline void SetMoney( int vMoney ){ mMoney = vMoney; }

	// Get & Set StorageMoney
	inline int GetStorageMoney( ) { return mStorageMoney; }
	inline void SetStorageMoney( int nMoney ) { mStorageMoney = nMoney; }

	// Get & Set TalkNpcEntityID
	inline unsigned int GetTalkNpcEntityID( ){ return mTalkNpcEntityID; }
	inline void SetTalkNpcEntityID( unsigned int vTalkNpcEntityID ){ mTalkNpcEntityID = vTalkNpcEntityID; }

	// Get & Set ServiceStatus
	inline EService GetServiceStatus( ){ return mServiceStatus; }
	inline void SetServiceStatus( EService vServiceStatus ){ mServiceStatus = vServiceStatus; }
	
	// Get & Set Produce
	inline CProduce *GetProduce( ){ return &mProduce; }

	// Get & Set CDGroup
	inline CGroupCDList& GetCDGroupList( ){ return mCDGroup; }
	//inline void SetCDGroup( int i, int vCDGroup ){ mCDGroup[ i ] = vCDGroup; }
	//inline void CleanCDGroup( ){ memset( mCDGroup, 0, sizeof( mCDGroup ) ); }

	// Get & Set CommonCD
	inline int GetCommonCD( ){ return mCommonCD; }
	inline void SetCommonCD( int vCommonCD ){ mCommonCD = vCommonCD; }

	// Get & Set BindMoney
	inline int GetBindMoney() { return mBindMoney; }
	inline void SetBindMoney( int vMoney ) { mBindMoney = vMoney; }

	// Get & Set StorageBindMoney
	inline int GetStorageBindMoney() { return mStorageBindMoney; }
	inline void SetStorageBindMoney( int nMoney ) { mStorageBindMoney = nMoney; }

	inline unsigned int GetLastWeeklyDisposTime() {return mLastWeeklyDisposTime;}
	inline void SetLastWeeklyDisposTime( unsigned int nTime ) { mLastWeeklyDisposTime = nTime; }

	inline unsigned int GetLastDaylyDisposTime() {return mLastDaylyDisposTime;}
	inline void SetLastDaylyDisposTime( unsigned int nTime ) { mLastDaylyDisposTime = nTime; }

	// Get & Set Equipment
	inline CEquipment *GetEquipment( ){ return &mEquipment; }

	// Get & Set Baggage
	inline CBaggage *GetBaggage( ){ return &mBaggage; }

	// Get & Set TaskBaggage
	inline CItemBox *GetTaskBaggage( ){ return &mTaskBaggage; }

	// Get & Set Storage
	inline CStorage *GetStorage( ){ return &mStorage; }	

	// Get & Set EquipSkill
	inline CSkillList *GetEquipSkill( ){ return &mEquipSkill; }

	// Get & Set TallentSkill
	inline CSkillList *GetTallentSkill( ){ return &mTalentSkill; }


	// Get & Set Shortcut
	inline CShortcutList *GetShortcut( ){ return &mShortcut; }

	// Get & Set ProLevel
	inline unsigned int GetProLevel() { return mProLevel; }
	inline void SetProLevel( unsigned int vProLevel ) { mProLevel = vProLevel; }

	// Get & Set ProExp
	inline unsigned int GetProExp() { return mProExp; }
	inline void SetProExp( unsigned int vProExp ) { mProExp = vProExp; }

	// Get & Set HorseState
	inline int GetHorseState( ){ return mStateParam[ STATE_RIDE_HORSE ].mInState; }
	//inline void SetHorseState( int vHorseState ){ mHorseState = vHorseState; }

	// Get & Set HorseSpeedPer
	//unsigned int GetHorseSpeedPer( ) { return mHorseSpeedPer; }
	//void InitHorseSpeedPer( );
	//void SetHorseSpeedPer( unsigned int vHorseSpeedPer ) { mHorseSpeedPer = vHorseSpeedPer; }

	// Get & Set HorseTempID
	unsigned int GetHorseTempID( ) { return mStateParam[ STATE_RIDE_HORSE ].mParames1; }
	//void SetHorseTempID( unsigned int vHorseTempID ) { mHorseTempID = vHorseTempID; }

	// Get & Set TownPos
	inline ComplexPos *GetTownPos( ){ return &mTownPos; }

	ComplexPos* GetTelePos( unsigned int nIndex );
	int AddTelePos( const ComplexPos& rPos );
	int DelTelePos( unsigned int nIndex );

	// Get & Set StorageStatus
	inline int GetStorageStatus( ) { return mStorageStatus; }
	inline void SetStorageStatus( int vStorageStatus ){ mStorageStatus = vStorageStatus; }  

	// Get & Set Honor
	inline int GetHonor( ) { return mHonor; }
	inline void SetHonor( int vHonor ){ mHonor = vHonor; } 

	// Get & Set Honor
	inline int GetTodayHonor( ) { return mTodayHonor; }
	inline void SetTodayHonor( int vHonor ){ mTodayHonor = vHonor; } 

	// Get & Set InHonPerDay
	inline int GetInHonPerDay( ) { return mInHonPerDay; }
	inline void SetInHonPerDay( int vInHonPerDay ){ mInHonPerDay = vInHonPerDay; } 

	// Get & Set OutHonPerDay
	inline int GetOutHonPerDay( ) { return mOutHonPerDay; }
	inline void SetOutHonPerDay( int vOutHonPerDay ){ mOutHonPerDay = vOutHonPerDay; } 	

	// Get & Set LastObjID
	inline int GetLastObjID( ) { return mLastObjID; }
	inline void SetLastObjID( int vLastObjID ){ mLastObjID = vLastObjID; } 

	// Get & Set SaveDate
	inline int GetSaveDate( ) { return mSaveDate; }
	inline void SetSaveDate( int vSaveDate ){ mSaveDate = vSaveDate; } 

	// Get & Set Kill
	inline KillData *GetKill( int i ){ return &mKill[ i ]; }
	inline void SetKill( int i, KillData vKill ){ mKill[ i ] = vKill; }
	inline void CleanKill( ){ memset( mKill, 0, sizeof( mKill ) ); }
	inline int GetKillSize( ){ return ARRAY_CNT(mKill); }

	// Get & Set Repetion
	inline CPlayerRepetion *GetRepetion( ){ return &mRepetion; }

	// Get & Set DiploidTime
	inline int GetDiploid( ) { return mDiploidTime; }
	inline void SetDiploid( int vDiploidTime ) { mDiploidTime = vDiploidTime; }
	// 双倍总值
	int GetTotalDiploid();
	inline bool IsVip() { return mVipFlag == EM_VIP_NONE ? false : true; }

	inline void SetVipFlag( int nFlag ) { mVipFlag = nFlag; }
	inline int  GetVipFlag() { return mVipFlag; }
	inline void OpenVip( int nTime ) 
	{ 
		mVipFlag = EM_VIP_NORMAL; 
		mVipEndDate = nTime;
	}
	inline void CloseVip() 
	{ 
		mVipFlag = EM_VIP_NONE; 
		mVipEndDate = 0;
	}

	inline void SetVipTime( int nTime ) { mVipEndDate = nTime; }
	inline int  GetVipTime() { return mVipEndDate; }

	// Get & Set ValidDiploid
	int GetValidDiploid();
	inline void SetValidDiploid( int vValidDiploid ) { mDiploidState[ LEFTVALID_STATE ] = vValidDiploid; }
	
	// Get & Set AnswerStatus
	int		GetAnswerStatusObjID(  ){ return mAnswerStatusObjID; }
	void    SetAnswerStatusObjID( int Status ) { mAnswerStatusObjID = Status;  }
	// Get & Set AnswerStatusRedStoneID
	int		GetRedStoneID(){ return mRedStoneID; }
	void	SetRedStoneID( int RedStoneStatus ) {  mRedStoneID = RedStoneStatus; }
	
	// Get & Set WelfareStatus
	int GetWelfareStatus() { return mDailyWelfareStatus;	}
	void SetWelfareStatus( int Status ) { mDailyWelfareStatus = Status; }
	
	// Get & Set HasRedStoneStatus
	int GetHasRestoneStatus() { return mHasRedStoneStatus; }
	void SetHasRestoneStatus( int Status ){ mHasRedStoneStatus = Status; }

	// Get & Set LastRefreshTime
	inline int GetLastRefreshTime() { return mLastRefreshTime; }
	inline void SetLastRefreshTime( int nTime ) { mLastRefreshTime = nTime; }

	// Get & Set KillNumIn
	inline int GetKillNumIn() { return mKillNumIn; }
	inline void SetKillNumIn( int vKillNum ) { mKillNumIn = vKillNum; }

	// Get & Set KillNumOut
	inline int GetKillNumOut() { return mKillNumOut; }
	inline void SetKillNumOut( int vKillNum ) { mKillNumOut = vKillNum; }

	// Get & Set DieNum
	inline int GetDieNum() { return mDieNum; }
	inline void SetDieNum( int vDieNum ) { mDieNum = vDieNum; }

	// Get & Set CompletedTaskNum
	inline int GetCompletedTaskNum() { return mCompletedTasks; }
	inline void SetCompletedTaskNum( int vCompletedTaskNum ) { mCompletedTasks = vCompletedTaskNum; }


	// Get & Set LastFamilyApplyTime
	void SetLastFamilyApplyTime( int vLastTime ){ mLastFamilyApplyTime = vLastTime; }
	int	 GetLastFamilyApplyTime( ){ return mLastFamilyApplyTime; }
	
	// Get & Set LastTimeLeaveFamily
	void SetLastTimeLeaveFamily( int vLastTime ){ mLastTimeLeaveFamily = vLastTime;  }
	unsigned int	 GetLastTimeLeaveFamily( ) { return mLastTimeLeaveFamily; }

	// Get & Set ReduceLevel
	inline int GetReduceLevel() { return mReduceLevel; }
	inline void SetReduceLevel( int vReduceLevel ) { mReduceLevel = vReduceLevel; }

	// Get & Set DiploidState
	inline void SetDiploidState( EDiploidState i, int vParam )
	{ 
		//LK_ASSERT( i < ARRAY_CNT( mDiploidState ), return );
		mDiploidState[ i ] = vParam; 
	}
	inline int GetDiploidState( int i ) { return mDiploidState[ i ]; }
	inline bool IsFreezeDouble()
	{
		return ( mDiploidState[ FREEZE_STATE ] > 0 &&  mDiploidState[ DIPLOID_STATE ] > 0 ? true : false );
	}
	inline bool UnFreezeDouble()
	{
		return ( mDiploidState[ DIPLOID_STATE ] > 0 && mDiploidState[ FREEZE_STATE ] == 0 ? true : false );
	}
	inline bool InDoubleTime()
	{
		return ( mDiploidState[ DIPLOID_STATE ] > 0 ? true : false );
	}
	inline int GetDiploidTimerID()
	{
		return mDiploidState[ TIMEID_STATE ];
	}
	inline void SetDoubleTimerID( int nTimerID )
	{
		mDiploidState[ TIMEID_STATE ] = nTimerID;
	}
	inline bool CanObtainDoubleExp()
	{
		if ( (mDiploidState[ DIPLOID_STATE ] > 0 && mDiploidState[ FREEZE_STATE ] == 0)
			|| mDiploidState[ ACTIVITY_STATE ] > 0) 
			return true;
		return false;
	}
	inline void SetDoubleFreezeState( bool bFreeze )
	{
		if ( bFreeze ) 
			mDiploidState[ FREEZE_STATE ] = 1;
		else
			mDiploidState[ FREEZE_STATE ] = 0;
	}
	inline void SetDoubleTimeState( bool bOpen )
	{
		if ( bOpen )
			mDiploidState[ DIPLOID_STATE ] = 1;
		else
			mDiploidState[ DIPLOID_STATE ] = 0;  
	}

	inline bool IsActivityDouble()
	{
		return ( mDiploidState[ ACTIVITY_STATE ] > 0 ? true : false );
	}

	inline void InitDoubleState()
	{
		for ( int i = 0; i < (int)ARRAY_CNT( mDiploidState ) - 1; i ++ )
			mDiploidState[ i ] = 0;
	}

	inline void SetActivityState( int nRate )
	{
		mDiploidState[ ACTIVITY_STATE ] = nRate;
	}

	inline int GetActivityState()
	{
		return mDiploidState[ ACTIVITY_STATE ];
	}

	inline int GetLastStopTime(){ return mDiploidState[ TIMERSTOP_STATE ]; }
	inline void SetLastStopTime( int nTime ){ mDiploidState[ TIMERSTOP_STATE ] = nTime; }

	void CheckDiploidState();

	// Get & Set SaleItem
	inline SaleItemList *GetSaleItem( ){ return &mSaleItem; }

	// Get & Set StallInfo
	inline CStallInfo *GetStallInfo(const int vStallType ){ return &mStallInfo[vStallType]; }

	// Get & Set AttackList
	inline StateList *GetAttackList( ){ return &mAttackList; }
	// Get & Set DefenceList
	inline StateList *GetDefenceList( ){ return &mDefenceList; }
	// Get & Set DeathList
	inline StateList *GetDeathList( ){ return &mDeathList; }
	// Get & Set AttackStateTime
	inline int GetAttackStateTime() { return mAttackStateTime; }
	inline void SetAttackStateTime( int vAttackStateTime ) { mAttackStateTime = vAttackStateTime; }
	// Get & Set DefenceStateTime
	inline int GetDefenceStateTime() { return mDefenceStateTime; }
	inline void SetDefenceStateTime( int vDefenceStateTime ) { mDefenceStateTime = vDefenceStateTime; }
	// Get & Set DeathStateTime
	inline int GetDeathStateTime() { return mDeathStateTime; }
	inline void SetDeathStateTime( int vDeathStateTime ) { mDeathStateTime = vDeathStateTime; }

	// Get & Set AssignExpScl
	inline unsigned char GetAssignExpScl() { return mAssignExpScl; }
	inline void SetAssignExpScl( unsigned char val ) { mAssignExpScl = val; }
	
	// Get & Set CurrentTitle
	inline int GetCurrentTitle() { return mCurrentTitle; }
	inline void SetCurrentTitle( int vCurrentTitle ) { mCurrentTitle = vCurrentTitle; }

	// 称号获取时间 [12/29/2009 Macro]
	int GetTitleTime( int nTitleID )
	{
		TitleList::iterator it = mTitleList.begin();
		for ( ; it != mTitleList.end(); ++ it )
		{
			if ( it->mTitleID == nTitleID )
				return it->mTime;
		}
		return SUCCESS;
	}

	int AddTitle( int vTitleID, unsigned int nInvalidTime = 0 );
	int GetLowLevelTitle( int vTitleID );
	int GetTitleList( int* npList, int& nNum )
	{
		if ( npList == NULL ) return -1;
		int tSize = nNum; nNum = 0;
		TitleList::iterator it = mTitleList.begin();
		for ( ; it != mTitleList.end(); ++ it )
		{
			if ( nNum < tSize )
				npList[ nNum ++ ] = it->mTitleID;
		}
		return SUCCESS;
	}

	inline void RemoveTitle( int vTitleID )
	{
		TitleList::iterator it = mTitleList.begin();
		for ( ; it != mTitleList.end(); ++ it )
		{
			if ( it->mTitleID == vTitleID )
			{
				RemoveTitleSkill( vTitleID );
				mTitleList.erase( it );
				break;
			}
		}
		return ;
	}

	inline bool HasTitle( int vTitleID )
	{
		TitleList::iterator it = mTitleList.begin();
		for ( ; it != mTitleList.end(); ++ it )
		{
			if ( it->mTitleID == vTitleID )
				return true;
		}
		return false;
	}

	inline void RemoveTitleSkill( int vTitleID )
	{
		CTitle* tpTitle = CDataStatic::GetTitle( vTitleID );
		if ( tpTitle == NULL )
			return ;
		CTemplateSkill* tpSkill = ( CTemplateSkill* ) CDataStatic::SearchTpl( tpTitle->mAddProperty );
		if ( tpSkill == NULL )
			return ;
		mTitleSkill.RemoveSkill( tpSkill->mSkillID );
	}

	// Get & Set TalentExp
	inline int GetTalentExp() { return mTalentExp; }
	inline void SetTalentExp( int vTalentExp ) { mTalentExp = vTalentExp; }
	// Get & Set TalentPoint
	inline int GetTalentPoint() { return mTalentPoint; }
	inline void SetTalentPoint( int vTalentPoint ) { mTalentPoint = vTalentPoint; }
	// Get & Set TalentStep
	inline int GetTalentStep() { return mTalentStep; }
	inline void SetTalentStep( int vTalentStep ) { mTalentStep = vTalentStep; }
	// Get & Set TalentStudy
	inline int GetTalentStudy( ) { return mTalentStudy; }
	inline void SetTalentStudy( int vTalentStudy ) { mTalentStudy = vTalentStudy; }
	// Get & Set TalentLevel
	inline int GetTalentLevel( ) { return mTalentLevel; }
	inline void SetTalentLevel( int vTalentLevel ) { mTalentLevel = vTalentLevel; }
	
	// Get & Set DecompoundEquipIndex
	int	 GetDecompoundEquipIndex( ){ return mDecompoundEquipIndex; }
	void SetDecompoundEquipIndex( int nIndex ){ mDecompoundEquipIndex = nIndex; }
	
	// 保存摊位文本
	inline void SaveStallText( const char* pName, const char* pAd, const int vStallType )
	{
		LK_ASSERT( pName != NULL && pAd != NULL, return );
		mStallInfo[vStallType].SetName( pName );
		mStallInfo[vStallType].SetAd( pAd );
	}
	

	inline int	GetPKValue() { return mPKValue; }
	inline void SetPKValue( int nValue ) { mPKValue = nValue; }
	void IncreasePKValue( int nValue );

	bool InJudgeCoolTime()
	{
		return ( ( time(NULL) - mLastJudgeTime ) > JUDGE_COOL_TIME ? false : true );
	}
	

	inline EPKStatus GetPKStatus() { return mPKStatus; }
	inline void	SetPKStatus( EPKStatus nPkStatus ) { mPKStatus = nPkStatus; }

	inline bool GetYellowStatus() { return mYellowStatus; }
	inline void	SetYellowStatus( bool bStatus ) { mYellowStatus = bStatus; }

	inline void SetYellowTimer(int nTimer) { mYellowTimer = nTimer; }
	inline int  GetYellowTimer() { return mYellowTimer; }

	inline void SetPrisonTime( int nTime ) { mPrisonTime = nTime; }
	inline void IncPrisonTime( int nTime ) { mPrisonTime += nTime; }
	inline int	GetPrisonTime() { return mPrisonTime; }

	inline int GetMasterID( int i ) 
	{
		if ( i < 0 || i >= MAX_MASTER_NUM )
			return 0;
		return mMasterID[ i ]; 
	}
	inline void	SetMasterID( int nMasterID, int i )
	{
		if ( i < 0 || i >= MAX_MASTER_NUM )
			return;
		mMasterID[ i ] = nMasterID;
	}

	inline void AddSlaveID( int vSlaveID ){ mMatrixList.push_back( vSlaveID ); }
	inline bool IsSlaveFull( )
	{
		if ( mBeginnerNum + mFormalNum >= MAX_MATRIX_MENBER || mBeginnerNum >= MAX_BEGINNER_SLAVE_NUM )
			return true;
		return false;
	}

	inline void AddSlaveValue( int vValue, int vIndex )
	{
		if ( vIndex >=0 && vIndex < (int)ARRAY_CNT(mSlaveValue) )
		{
			mSlaveValue[ vIndex ] += vValue;
		}
	}
	inline int GetSlaveValue( int vIndex )
	{
		if ( vIndex >=0 && vIndex < (int)ARRAY_CNT(mSlaveValue) )
		{
			return mSlaveValue[ vIndex ];
		}
		return 0;
	}

	inline void SetSlavePoint( int vPoint ){ mSlavePoint = vPoint; }
	inline int GetSlavePoint( ){ return mSlavePoint; }

	inline void SetMasterPoint( int vPoint ){ mMasterPoint = vPoint; }
	inline int GetMasterPoint( ){ return mMasterPoint; }

	inline void SetSlaveTotal( int vTotal ){ mSlaveTotal = vTotal; }
	inline int GetSlaveTotal( ){ return mSlaveTotal; }

	inline void SetMasterTotal( int vTotal ){ mMasterTotal = vTotal; }
	inline int GetMasterTotal( ){ return mMasterTotal; }

	inline void SetMasterPercent( int vPercent, int vIndex )
	{
		if ( vIndex >=0 && vIndex < (int)ARRAY_CNT(mMasterPercent) )
		{
			mMasterPercent[ vIndex ] = vPercent;
		}
	}
	inline int GetMasterPercent( int vIndex )
	{
		if ( vIndex >=0 && vIndex < (int)ARRAY_CNT(mMasterPercent) )
		{
			return mMasterPercent[ vIndex ];
		}
		return 0;
	}

	inline void SetMasterTimes( int vTimes ){ mMasterTimes = vTimes; }
	inline int GetMasterTimes( ){ return mMasterTimes; }

	inline void SetMasterGive( int vGive ){ mMasterGive = vGive; }
	inline int GetMasterGive( ){ return mMasterGive; }

	inline void SetBeginnerNum( int vNum ){ mBeginnerNum = vNum; }
	inline int GetBeginnerNum( ){ return mBeginnerNum; }

	inline void SetFormalNum( int vNum ){ mFormalNum = vNum; }
	inline int GetFormalNum( ){ return mFormalNum; }

	inline void SetSlaveNum( int vNum ){ mSlaveNum = vNum; }
	inline int GetSlaveNum( ){ return mSlaveNum; }

	inline CYuanBao& GoldenYB() { return mGoldenYB; }
	inline CYuanBao& SilverYB() { return mSilverYB; }

	EPKStatus GetTplPKState();
	EPKStatus GetMainPKState();

	bool IsRedPKStatus();
	bool IsWhitePKStatus();
	
	// 技能武器需求匹配
	bool SkillWeaponValid( CTemplateSkill* pSkill );

	// 装备是否可用 ( 耐久度是否为0 )
	bool EquipmentAvailable( EEquipPart emPart );

	// 技能是否在公共冷却中
	bool IsCommonCoolingDown() { return ( GetCommonCD() > 0 ) ? true : false; }

	// 金币是否足够(支持绑定或非绑定)
	bool IsMoneyEnough( bool bBind, int vMoney );

	// 被攻击时，是否处于保护状态下
	bool CanAttacked();

	// 是否可以传送
	bool CanTeleport();

	// 跨地图,坐骑处理
	void ChangeHorseStateTransMap( unsigned short unMapID );

	// 加入自身状态buff
	void InsertSelfBuff( CEntityCharacter *pDesEntity, CTemplateProperty::PROPERTY_FUNC vType, int vOldPer = 0, int vNewPer = 0 );

	// Get & Set SkillList
	inline virtual CSkillList& GetSkillList( ) { return mSkill ;}

	inline int GetWizardDiff( int vType )
	{
		if ( vType < 0 || vType > WIZARD_NUM )
		{
			return 0;
		}
		return mWizard.GetWizardPoint( vType ) + mWizard.GetWizardIncrease( vType ) + GetEquipWizard( vType ) - mWizard.GetWizardSubmit( vType );
	}

	inline int AddAwardTimes( int vAwardID, EM_AwardLimitType nType = EALT_Daily )
	{
		for ( AwardList::iterator it = mAwardList[nType].begin(); it != mAwardList[nType].end(); ++it )
		{
			if ( it->mAwardID == vAwardID )
			{
				it->mTimes++;
				return it->mTimes;
			}
		}
		mAwardList[nType].push_back( ST_AWARD( vAwardID, 1 ) );
		return 1;
	}

	inline int GetAwardTimes( int vAwardID, EM_AwardLimitType nType = EALT_Daily )
	{
		for ( AwardList::iterator it = mAwardList[nType].begin(); it != mAwardList[nType].end(); ++it )
		{
			if ( it->mAwardID == vAwardID )
			{
				return it->mTimes;
			}
		}
		return 0;
	}

	inline int InsertAwardTimes( int vAwardID, int vTimes, EM_AwardLimitType nType = EALT_Daily )
	{
		for ( AwardList::iterator it = mAwardList[nType].begin(); it != mAwardList[nType].end(); ++it )
		{
			if ( it->mAwardID == vAwardID )
			{
				return -1;
			}
		}
		mAwardList[nType].push_back( ST_AWARD( vAwardID, vTimes ) );
		return 0;
	}

	inline int ClearAwardTimes( EM_AwardLimitType nType = EALT_Daily )
	{
		mAwardList[nType].clear();
		return 0;
	}

	int GetIndexByID( int vID );

	void DeleteItem( );

	void RemoveBuffOnLogOff( );
	
	// Get&Set InvestStatus
	int GetInvestStatus(){ return mInvestStatus; }
	void SetInvestStatus( int nStatus ){ mInvestStatus = nStatus; }

	int GetTotalWar(){ return mTotalWar; }
	void SetTotalWar( int nTotalWar ){ mTotalWar = nTotalWar; }

	int GetTodayWar(){ return mTodayWar; }
	void SetTodayWar( int nTodayWar ){ mTodayWar = nTodayWar; }

	int GetTodayOther(){ return mTodayOther; }
	void SetTodayOther( int nTodayOther ){ mTodayOther = nTodayOther; }

	int GetFreeWar(){ return mFreeWar; }
	void SetFreeWar( int nFreeWar ){ mFreeWar = nFreeWar; }

	int GetWarKill(){ return mWarKill; }
	void SetWarKill( int nWarKill ){ mWarKill = nWarKill; }

	int GetWarSerial(){ return mWarSerial; }
	void SetWarSerial( int nWarSerial ){ mWarSerial = nWarSerial; }

	int GetExpHonor(){ return mExpHonor; }
	void SetExpHonor( int nExpHonor ){ mExpHonor = nExpHonor; }

	int GetItemHonor(){ return mItemHonor; }
	void SetItemHonor( int nItemHonor ){ mItemHonor = nItemHonor; }

	int GetHasVoted(){ return mHasVoted; }
	void SetHasVoted( int nHasVoted ){ mHasVoted = nHasVoted; }
	
	CTreasureBowlInfo *GetTreasureBowlInfo() { return &mTreasureBowlInfo; }
	//
	void GetRepetDataForClient( PBRepetDataList * pBuffer);

	CSpiritInfo& GetSpiritInfo(){return m_SpiritInfo;}
};


/************************************************************************/
/* NPC属性 （怪物，功能NPC..... )                                       */
/************************************************************************/
class CPropertyNPC : public CPropertyCharacter
{
protected:
	int				mRelifeTimer;		//复活计时器
	int             mRelifeTime;        //复活计时基本设置

	int				mBattleTimer;		// 战场计时器ID
	CSkillListInstance<1>	mSkill;		// 技能列表

	int				mTranChangeLevel;

public:
	CPropertyNPC( ){ }
	virtual ~CPropertyNPC( ){ }

    int Initial( );
    int Final( );

	virtual CPropertyValue RefreshMaxHP( );
	virtual CPropertyValue RefreshStr( );
	virtual CPropertyValue RefreshWis( );
	virtual CPropertyValue RefreshSpr( );
	virtual CPropertyValue RefreshSta( );
	virtual CPropertyValue RefreshCon( );
	virtual CPropertyValue RefreshMaxMP( );
	virtual ATTACK_TYPE RefreshAttackType( );
	virtual CPropertyValue RefreshPAttackMax( );
	virtual CPropertyValue RefreshPAttackMin( );
	virtual CPropertyValue RefreshMAttackMax( );
	virtual CPropertyValue RefreshMAttackMin( );
	virtual CPropertyValue RefreshDeathRate( );
	virtual CPropertyValue RefreshDeathValue( );
	virtual CPropertyValue RefreshDeathDefense( );
	virtual CPropertyValue RefreshNormalDuck( );
	virtual CPropertyValue RefreshSkillDuck( );
	virtual CPropertyValue RefreshIgnoreDuck( );
	virtual CPropertyValue RefreshPDefence( );
	virtual CPropertyValue RefreshMDefence( );
	virtual int RefreshAddFixDamage( );
	virtual int RefreshDecFixPDamage( );
	virtual int RefreshDecFixMDamage( );
	virtual int RefreshDecPerMDamage( );
	virtual int RefreshDecPerPDamage( );
	virtual int RefreshNormalHitRate( );
	virtual int RefreshSkillHitRate( );
	virtual int RefreshResDef( int *vDef );
	virtual int RefreshResAtt( int *vAtt );
	virtual	int GetDuckRate( );

	// Get & Set RelifeTimer
	inline int GetRelifeTimer( ) { return mRelifeTimer; }
	inline void SetRelifeTimer( int vRelifeTimer ){ mRelifeTimer = vRelifeTimer; } 

	// Get & Set RelifeTime
	inline int GetRelifeTime( ) { return mRelifeTime; }
	inline void SetRelifeTime( int vRelifeTime ){ mRelifeTime = vRelifeTime; } 

	inline int GetBattleTimer() { return mBattleTimer; }
	inline void SetBattleTimer( int nTimer ) { mBattleTimer = nTimer; }

	// Get & Set SkillList
	inline virtual CSkillList& GetSkillList( ) { return mSkill ;}

	// 处理消失Timer到时的事件
	int OnEntityDisappearTimeout( CEntity* pEntity );

	void SetTranChangeLevel( int vLevel ) { mTranChangeLevel = vLevel; }
	int GetTranChangeLevel( ) { return mTranChangeLevel; }
};

/************************************************************************/
/* 宠物属性                                                             */
/************************************************************************/
class CPropertyPet : public CPropertyCharacter
{
protected:
	//unsigned int			mMonsterID;	// 主人ID
	CSkillListInstance<1>	mSkill;		// 技能列表

public:
	CPropertyPet( ){ }
	virtual ~CPropertyPet( ){ }

    int Initial( );
    int Final( );

	virtual CPropertyValue RefreshMaxHP( );
	virtual CPropertyValue RefreshStr( );
	virtual CPropertyValue RefreshWis(  );
	virtual CPropertyValue RefreshSpr( );
	virtual CPropertyValue RefreshSta( );
	virtual CPropertyValue RefreshCon( );
	virtual CPropertyValue RefreshMaxMP( );
	virtual ATTACK_TYPE RefreshAttackType( );
	virtual CPropertyValue RefreshPAttackMax( );
	virtual CPropertyValue RefreshPAttackMin( );
	virtual CPropertyValue RefreshMAttackMax( );
	virtual CPropertyValue RefreshMAttackMin( );
	virtual CPropertyValue RefreshDeathRate( );
	virtual CPropertyValue RefreshDeathValue( );
	virtual CPropertyValue RefreshDeathDefense( );
	virtual CPropertyValue RefreshNormalDuck( );
	virtual CPropertyValue RefreshSkillDuck( );
	virtual CPropertyValue RefreshIgnoreDuck( );
	virtual CPropertyValue RefreshPDefence( );
	virtual CPropertyValue RefreshMDefence( );
	virtual int RefreshAddFixDamage( );
	virtual int RefreshDecFixPDamage( );
	virtual int RefreshDecFixMDamage( );
	virtual int RefreshDecPerMDamage( );
	virtual int RefreshDecPerPDamage( );
	virtual int RefreshNormalHitRate( );
	virtual int RefreshSkillHitRate( );
	virtual int RefreshResDef( int *vDef );
	virtual int RefreshResAtt( int *vAtt );
	virtual int GetDuckRate( );

	//// Get & Set MonsterID
	//inline unsigned int GetMonsterID( ) { return mMonsterID; }
	//inline void SetMonsterID( unsigned int vMonsterID ) { mMonsterID = vMonsterID; }

	// Get & Set SkillList
	inline virtual CSkillList& GetSkillList( ) { return mSkill; }
};


/************************************************************************/
/* 掉落物属性                                                                     */
/************************************************************************/
class CPropertyDrop : public CProperty
{
protected:
	int				mItemProtect;
	int				mOwnerCharID[ TEAMNUM ];

public:
	bool			HasOwner( );
	bool			IsOwner( int vCharID );

	// Get & Set ItemProtect
	inline int GetItemProtect( ) { return mItemProtect; }
	inline void SetItemProtect( int vItemProtect ){ mItemProtect = vItemProtect; } 

	// Get & Set OwnerCharID
	inline unsigned int GetOwnerCharID( int i ){ return mOwnerCharID[ i ]; }
	inline void SetOwnerCharID( int i, unsigned int vOwnerCharID ){ mOwnerCharID[ i ] = vOwnerCharID; }
	inline void CleanOwnerCharID( ){ memset( mOwnerCharID, 0, sizeof( mOwnerCharID ) ); }
	inline void CloneOwnerCharID( int *vOwnerCharID ){ memcpy( mOwnerCharID, vOwnerCharID, sizeof( mOwnerCharID ) ); }
};


/************************************************************************/
/* 道具属性                                                                     */
/************************************************************************/
class CPropertyItem : public CPropertyDrop
{
protected:
	unsigned int	mItemID;
public:
    int Initial( );
    int Final( );

	// Get & Set ItemID
	inline unsigned int GetItemID( ) { return mItemID; }
	inline void SetItemID( unsigned int vItemID ){ mItemID = vItemID; } 
};

/************************************************************************/
/* 金钱属性                                                             */
/************************************************************************/
class CPropertyMoney : public CPropertyDrop
{
protected:
	int mMoney;
public:
	int Initial( );
	int Final( );

	// Get & Set Money
	inline int GetMoney( ) { return mMoney; }
	inline void SetMoney( int vMoney ){ mMoney = vMoney; } 

};

/************************************************************************/
/* 传送点，陷阱属性                                                                     */
/************************************************************************/
class CPropertyPedal : public CProperty
{
public:
	int Initial( );
	int Final( );
	void IsDisappear( CEntity* pEntity, int vTickOffset );
	//void PedalIsTouch( CEntity* pEntity, int vTickOffset, vector< CEntity* >& vDeleteList );
};


/************************************************************************/
/* 宝箱属性                                                                     */
/************************************************************************/
//class CPropertyBox : public CProperty
//{
//public:
	
//	int Initial( ) { return SUCCESS; }
//	int Final( ) { return SUCCESS; }
//};
/************************************************************************/
/* 内丹属性                                                                     */
/************************************************************************/
class CPropertyRedStone : public CProperty
{
public:
	int Initial( ) { return SUCCESS; }
	int Final( ) { return SUCCESS; }
};

///************************************************************************/
/* 阻挡属性                                                                     */
/************************************************************************/
//class CPropertyBlock : public CProperty
//{
//public:
	
//	int Initial( ) { return SUCCESS; }
//	int Final( ) { return SUCCESS; }
//};


#endif




