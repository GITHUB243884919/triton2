#pragma once

#include "servertool.h"
#include "coretype.h"
#include "protype.h"
#include "share_define.h"
#include "creator.h"
#include "container.h"
#include "base.h"
#include "timerinterface.h"
#include "familytype.h"
#include "corpstype.h"
#include "observer.h"
#include "property.h"
#include "speedup_checker.h"
#include "playerinfo_yqs.h"

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
class CMapObject;
class CEntityCollect;
class CEntityBox;
class CProperty;
class CPropertyCharacter;
class CPropertyNpc;


/*
enum EFcmStatus
{
	em_none_rate = 0,
	em_half_rate = 1,
	em_normal_rate = 2,
};
*/

enum EUnHandle
{
	UN_HANDLE_DROP_FOE_DIE		=	( 1 << 0 ),
	UN_HANDLE_PROCESS_FOE_DIE	=	( 1 << 1 ),
	UN_HANDLE_PROCESS_FOR_ATTACK=	( 1 << 2 ),
	UN_HANDLE_HONOR				=	( 1 << 3 ),
	UN_HANDLE_DURA				=	( 1 << 4 )
};


// ********************************************************************** //
// CEntity
// ********************************************************************** //
class CEntity : public CObj
{
public:
	bool				mHasCreated;	// 是否已创建
	CWTPoint			mPos;			// 实体坐标
	EDirType			mDirection;		// 玩家方向
protected:	
	char				mAttCount;	// 攻击记数
	unsigned short		mLineID;
	unsigned short		mMapID;
	unsigned short		mIndex;
	unsigned int		mCreatorID;		// 生成这个怪物的生成器ID，0表示非怪物
	EEntityType			mEntityType;	// 实体类型
	EmEntityVisiblitiy	mVisibility;	// 对谁可见( 所有人，自己， 队伍 )
	EAliveType			mAliveState;	// 生存状态
	int					mMapObjectID;	// 地图对象ID
	unsigned int		mEntityID;	
	CSeenPlayerContainer	mSeenPlayerContainer;	// 可见玩家
	unsigned int		mCreateTime;// 实体创建时间绝对时间点(ms)
	static unsigned int	mListID;	// 公共listid, 递增，加翻了从头再来
public:
	
	CEntity( EEntityType vEntityType ) : mEntityType( vEntityType )
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

	virtual ~CEntity( )
	{
	}

	virtual int Initialize( )
	{
		// -1, -1 是非法点
		mPos.mX = -1; 
		mPos.mY = -1;
		mLineID = INVALID_LINE_ID;
		mMapID = INVALID_MAP_ID;
		mIndex = INVALID_MAP_INDEX;
		mDirection = DIR_DOWN;
		mAliveState = ALIVE_NORMAL;
		mVisibility = VISIBLITY_ALL;
		mAttCount = 0;
		mCreateTime = 0;
		mMapObjectID = 0;
		mEntityID = 0;
		mCreateTime = 0;
		mAttCount = 0;
		mHasCreated = true;

		return 0;
	}

	virtual int Resume()
	{
		return 0;
	}

public:
	inline int SetVisibility( EmEntityVisiblitiy vIsVisibility ) { mVisibility = vIsVisibility; return SUCCESS; } 

	// 存活状态
	inline EAliveType GetAliveStatus( ) { return mAliveState; }
	inline void SetAliveStatus( EAliveType enAlive ) { mAliveState = enAlive; }
	bool IsAlive( ) const			{ return mAliveState == ALIVE_BATTLE || mAliveState == ALIVE_NORMAL; }

	// 是否是玩家
	inline bool IsPlayer( )		{ return GetEntityType() == ENTITYTYPE_PLAYER; }
	static inline bool IsValidPlayerID( int nEntityID ) { return CObj::ID2TYPE( nEntityID ) == OBJTYPE_ENTITY_PLAYER; }

	// 是否是怪物
	inline bool IsOgre( )		{ return GetEntityType() == ENTITYTYPE_OGRE; }
	static inline bool IsValidOgreID( int nEntityID ) { return CObj::ID2TYPE( nEntityID ) == OBJTYPE_ENTITY_OGRE; }

	// 是否是功能NPC
	inline bool IsFunNpc( )		{ return GetEntityType() == ENTITYTYPE_FUNCNPC; }
	static inline bool IsValidFunNpcID( int nEntityID ) { return CObj::ID2TYPE( nEntityID ) == OBJTYPE_ENTITY_FUNCNPC; }

	// 是否是陷阱、触发器
	inline bool IsPedal( )		{ return GetEntityType() == ENTITYTYPE_PEDAL; }
	static inline bool IsValidPedalID( int nEntityID ) {  return CObj::ID2TYPE( nEntityID ) == OBJTYPE_ENTITY_PEDAL; }

	// 是否是道具
	inline bool IsItem( )		{ return GetEntityType() == ENTITYTYPE_ITEM; }
	static inline bool IsValidItemID( int nEntityID ) { return CObj::ID2TYPE( nEntityID ) == OBJTYPE_ENTITY_ITEM; }

	// 是否是金钱
	inline bool IsMoney( )		{ return GetEntityType() == ENTITYTYPE_MONEY; }
	static inline bool IsValidMoneyID( int nEntityID ) { return CObj::ID2TYPE( nEntityID ) == OBJTYPE_ENTITY_MONEY; }

	// 是否是宠物
	inline bool IsPet( )		{ return GetEntityType() == ENTITYTYPE_PET; }
	static inline bool IsValidPetID( int nEntityID ) { return CObj::ID2TYPE( nEntityID ) == OBJTYPE_ENTITY_PET; }

	// 是否是采集NPC
	inline bool IsCollect( )	{ return GetEntityType() == ENTITYTYPE_COLLECT; }
	static inline bool IsValidCollectID( int nEntityID ) { return CObj::ID2TYPE( nEntityID ) == OBJTYPE_ENTITY_COLLECT; }

	// 是否是宝箱
	inline bool IsBox( )		{ return GetEntityType( ) == ENTITYTYPE_BOX; }
	static inline bool IsValidBoxID( int nEntityID ) { return CObj::ID2TYPE( nEntityID ) == OBJTYPE_ENTITY_STOREBOX; }
	
	// 是否是动态阻挡NPC
	inline bool IsBlock( )		{ return GetEntityType( ) == ENTITYTYPE_BLOCK; }
	static inline bool IsValidBlockID( int nEntityID ) { return CObj::ID2TYPE( nEntityID ) == OBJTYPE_ENTITY_BLOCK; }

	// 是否是内丹
	inline bool IsRedStone()  { return  GetEntityType() == ENTITYTYPE_REDSTONE; }
	static inline bool IsValidRedStoneID( int nEntityID ) { return CObj::ID2TYPE( nEntityID ) == OBJTYPE_ENTITY_EXPREDSTONE; }

	// 是否是通用NPC大类
	inline bool IsNpc( )		{ return IsOgre() || IsFunNpc() || IsCollect() || IsPet() || IsBox() || IsBlock(); }
	static inline bool IsValidNpcID( int nEntityID ) { return IsValidOgreID( nEntityID) || IsValidFunNpcID( nEntityID) || IsValidBoxID( nEntityID) || IsValidBlockID( nEntityID); }
	
	// 是否是character
	inline bool IsCharacter( )	{ return IsPlayer() || IsNpc() || IsPet() ;}
	static inline bool IsValidCharacterID( int nEntityID ) { return IsValidPlayerID( nEntityID ) || IsValidPetID( nEntityID ) || IsValidNpcID( nEntityID ) ;}

	// 是否是ghost
	inline bool IsGhost()		{ return GetEntityType( ) == ENTITYTYPE_GHOST; }
	static inline bool IsValidGhostID( int nEntityID ) { return CObj::ID2TYPE( nEntityID ) == OBJTYPE_ENTITY_GHOST; }
	// 是否是掉落类型
	inline bool IsDrop( )		{ return IsItem() ; }

	// 是否任务NPC
	virtual bool IsTaskNpc()  { return false;};
	
	// 得到创建时间
	inline unsigned int GetCreateTime( ) { return mCreateTime; }
	inline void SetCreateTime( unsigned int unCreatTime ) { mCreateTime = unCreatTime; }

	// 对目标玩家是否可见
	virtual bool IsVisibleFor( CEntity* pTarget )  { return true; }

	// 实体ID
	inline unsigned int GetEntityID( ) const		{ return mEntityID; }
	int SetEntityID( unsigned int vEntityID ) { mEntityID = vEntityID; return 0; }
	
	// 设置实体类型
	inline EEntityType GetEntityType( ) const		{ return mEntityType; }

	// 设置
	void SetEntityType( EEntityType type )	{ mEntityType = type; GetProperty()->SetEntityType( type ); }
	
	// 主人ID ( 默认返回自己)
	virtual unsigned int GetOwnerID() { return mEntityID; }

	// 主人
	CEntity* GetOwner();
	
	// 线ID
	virtual unsigned short GetLineID( ) { return mLineID ; }
	virtual void SetLineID( unsigned short usLineID ) { mLineID = usLineID; }
	
	// 地图ID
	virtual unsigned short GetMapID( )	 {return mMapID;}
	virtual unsigned short GetMapIndex( ) { return mIndex; }
	virtual void SetMapID( unsigned short usMapID )  { mMapID = usMapID; }
	virtual void SetIndex( unsigned short usMapIndex ) { mIndex = usMapIndex; }

	// 属性
	virtual CProperty* GetProperty( ) = 0;
	
	// 国家ID
	virtual unsigned short GetNationality( ) = 0;

	// 是否敌对
	virtual bool IsHostileTo( CEntity* pEntity ) = 0;
	
	// 是否友好
	virtual bool IsFriendlyTo( CEntity* pEntity ) = 0;
	// 是否是队友
	virtual bool IsTeamTo( CEntity* pEntity ) = 0;
	
	// 是否是同一家族成员
	virtual bool IsFamilyTo( CEntity *pEntity ) = 0;
	
	// 是否是同一家族成员
	virtual bool IsCorpsTo( CEntity *pEntity ) = 0;

	// 是否对玩家实体友好
	virtual bool IsHostileToPlayers() = 0;

	// 用创建器为实体赋值
	virtual bool Setup( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator, CWTPoint* pPos = NULL, bool bInit = false, int vNpcID = 0 ) { return true; }

	// 加入可见表
	virtual bool AddViewEntity( CEntity* pTarget );

	// 从可见表删除
	virtual bool DeleteViewEntity( CEntity* pTarget );

	// 得到定时器管理对象
	virtual ITimerReceiver* GetTimerReciver() = 0;

	virtual unsigned int GetNpcID( ) { return 0; }
	virtual void SetNpcID( unsigned int vNpcID ) { ; }

public:
	inline CWTPoint& GetPos() { return mPos; }
	inline int GetPosX() {return mPos.mX;}
	inline int GetPosY() {return mPos.mY;}
	inline void SetPosX(int x) 
	{ 
		if( x == -1 )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ] Set PosX Error %d, FUCK -1, FUCK Writer", __LK_FILE__, __LINE__,
				__FUNCTION__, x );
#ifdef _DEBUG_
			print_trace_fd( "default" );
#endif
		}
		mPos.mX = x; 
	}
	inline void SetPosY(int y) 
	{
		if( y == -1 )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ] Set PosX Error %d, FUCK -1, FUCK Writer", __LK_FILE__, __LINE__,
				__FUNCTION__, y );
#ifdef _DEBUG_
			print_trace_fd( "default" );
#endif
		} 
		mPos.mY = y; 
	}

	// 朝向
	inline EDirType GetDirection( ) { return mDirection; }
	inline void SetDirection( EDirType enDir ) { mDirection = enDir; }

	virtual int GetTeamFlag( ) = 0;
	
	// creator id
	inline unsigned int GetCreatorID( ) { return mCreatorID; }
	void SetCreatorID( unsigned int vCreatorID ) { mCreatorID = vCreatorID; }

	// 可见玩家容器
	inline CSeenPlayerContainer& GetSeenPlayersContainer() { return mSeenPlayerContainer;}


	// 得到地图对象
	CMapObject* GetMapObject( );

	// 得到地图对象ID
	inline unsigned int GetMapObjectID() { return mMapObjectID; }

	// 设置地图对象
	inline void SetMapObjectID( int nMapObjectID ) { mMapObjectID = nMapObjectID; }

	// 添加到功能列表中
	bool AddFunResult( unsigned int unSrcEntityID, unsigned short usFunc, unsigned short usResult, bool bBroadCast = true, unsigned int unValue1 = 0, unsigned int unValue2 = 0, unsigned int unValue3 = 0, unsigned int unValue4 = 0, unsigned int unValue5 = 0 );

	// 初始化功能列表
	void InitFunResult( int nListID = 0 );

	// 目标是否在距离范围内
	bool IsWithinDistance( CEntity* pTarget, unsigned int unDistance );

	// 是否在可见范围内
	bool IsWithinViewDistance( CEntity* pTarget );

public:
	// 发送使用技能回执信息
	virtual void SendUseSkillErrorNotice( int nType ) = 0;

	inline void AddAttackCount()
	{
		if ( ++mAttCount > 15 )
		{
			mAttCount = 1;
		}
	}

	inline int GetAttListID()
	{
		return ( ( 0xF0000000 | mEntityID ) & ( mAttCount << 28 | 0xFFFFFFF ) );
	}
};


class CEntityCharacter : public CEntity, public CCharacterObservable
{
protected:
	bool			mHasTarget;
	unsigned short	mMotionState;	// 移动状态
	CWTPoint		mTargetPos;
	double			mMoveLastTime;	// 玩家最后移动时间
	CEntityCharacter*	mEntityAureoList[ TEAMNUM ];	// 光环作用实体列表 ( 光环只对小队作用 )

public:
	CEntityCharacter( EEntityType vEntityType ) : CEntity( vEntityType )
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

	virtual ~CEntityCharacter( )
	{
	}

	virtual int Initialize( )
	{
		mMotionState = MOTION_IDLE;
		mHasTarget = false;
		memset( mEntityAureoList, 0, sizeof( mEntityAureoList ) );
		return 0;
	}

	virtual int Resume()
	{
		return 0;
	}

public:
	inline CWTPoint GetTargetPos( ) { return mTargetPos; }
	void SetTargetPos( const CWTPoint& rPoint )
	{
		mTargetPos = rPoint;
		mHasTarget = true;
	}
	
	virtual ITimerReceiver* GetTimerReciver() = 0;

	int AddAureoEntityList( CEntityCharacter* pEntity );
	void ClearAureoEntityList( );
	void RemoveAureoEntityList( CEntityCharacter* pEntity );
	void CheckAureoEntityList( CEntityCharacter** pEntityList, int vEntityCount, CEntityCharacter** pAddEntityList, int& vAddEntityCount, CEntityCharacter** pRemoveEntityList, int & vRemoveEntityCount );

	inline CEntityCharacter* GetAureoEntity(unsigned int vIndex )
	{
		if ( vIndex >= 0 && vIndex < ARRAY_CNT( mEntityAureoList ) )
			return mEntityAureoList[ vIndex ];
		return NULL;
	}

	// 死亡中断活动
	void InterruptActivity();

public:
	inline bool GetContorlable( )	{ return ((CPropertyCharacter*)GetProperty())->Controlable( );  }

	// 移动性
	inline bool GetMoveable( )	{ return  ((CPropertyCharacter*)GetProperty())->Moveable( ) && IsAlive(); } // 不知道有没有正确设置状态，这里直接掉比较保险

	// 是否可以攻击
	inline bool GetAttackable( ) { return ((CPropertyCharacter*)GetProperty())->Attackable( ); }

	inline unsigned short GetMotionState( ) { return mMotionState; }
	inline void SetMotionState( unsigned short vMotionState ) { mMotionState = vMotionState; }

	inline double GetMoveLastTime( ) { return mMoveLastTime; }
	inline void SetMoveLastTime( double vMoveLastTime ) { mMoveLastTime = vMoveLastTime; }

	virtual void SetIndex( unsigned short usMapIndex ) { mIndex = usMapIndex; }

	inline bool GetHasTarget( ) { return mHasTarget; }
	inline void SetHasTarget( bool vHasTarget ) { mHasTarget = vHasTarget; }

public:
	// 发送使用技能回执信息
	virtual void SendUseSkillErrorNotice( int nType ) = 0;

	// 清除BUFF状态
	virtual void RemoveBuffState( CTemplateBuff* pBuffTpl, int& nSrcEntityID, int& nBackDamage );

	// 添加BUFF状态
	virtual void InsertBuffState( CEntityCharacter* pSrcEntity, CTemplateBuff* pBuffTpl, CBuffObject* vpBuffObj/*CBuffList* pBuffList*/, int nSkillType, int vDamage );
		
	// 恢复HP
	virtual int CureHP( int vCureHP, unsigned int nSrcEntityID = 0, EResultType emType = RESULT_NORMAL, bool bAddFunc = true );

	// 恢复MP
	virtual int CureMP( int vCureMP, unsigned int nSrcEntityID = 0, EResultType emType = RESULT_NORMAL, bool bAddFunc = true );

	// 恢复AP
	virtual int CureAP( int vCureAP, unsigned int nSrcEntityID = 0, EResultType emType = RESULT_NORMAL, bool bAddFunc = true );

	// 消耗HP
	virtual int ExpendHP( int vCureHP, unsigned int nSrcEntityID = 0, EResultType emType = RESULT_NORMAL, bool bAddFunc = true );
	
	// 消耗MP
	virtual int ExpendMP( int vExpendMP, unsigned int nSrcEntityID = 0, EResultType emType = RESULT_NORMAL, bool bAddFunc = true );

	// 偷取HP
	virtual int StealHP( int nDamage );

	// 偷取MP
	virtual int StealMP( int nDamage );

	// 瞬间改变位置
	virtual void ChangePos( CWTPoint vPos, EResultType emType, unsigned int nSrcEntityID = 0 );

	// 实体死亡时,清除BUFF
	virtual void RemoveBuffEntityDie();

	// 实体添加BUFF
	virtual int EntityInsertBuff( CEntityCharacter* pSrcEntity, CTemplateBuff* pBuffTpl, int nSkillType = 0, int nDamage = 0 , int nBuffTime = 0 );

	// 实体删除BUFF
	virtual int EntityRemoveBuff( int vBuffID, int vListID = 0 );
		
	// 实体身上是否有某BUFF
	virtual bool EntityHasBuff( int nBuffID );

	// 手动删除BUFF
	virtual bool ActiveRemoveBuff( int nBuffID );

	// 实体死亡
	virtual void EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf = false, bool vIsAward = true );
	
	// 下面为处理实体死亡的相关接口 ( 放这,主要是为了以后NPC下面的子类合并用,还有为PLAYER掉装备做准备 )
	// 怪物死亡,玩家获得经验 ( 暂时只有OGRE用 )
	virtual void ExpForDie( CEntityCharacter* pSrcEntity ) {};
	// 怪物死亡,掉落物品 ( 暂时只有OGRE用 )
	virtual void DropForDie( CEntityCharacter* pSrcEntity, int vMaxTeam ) {};
	// 处理怪物死亡 ( 暂时只有OGRE,FUNCNPC,PET用 )
	virtual void TimeForDie( ) {};
	// 通知怪物死亡 ( CHARACTER使用 )
	virtual void NotifyDeath( CEntityCharacter* pSrcEntity );
	// 处理杀怪取血 ( CHARACTER使用 )
	virtual void KillerRenew( CEntityCharacter* pSrcEntity );
	// 处理召唤兽逻辑 ( 暂时只有PLAYER,PET使用 )
	virtual void PetLogicForDie( ) {};
	// 获得功勋 ( 暂时只有PLAYER使用 )
	virtual void HonorForDie( CEntityCharacter* pSrcEntity ) {};
	// PK规则 ( 暂时只有PLAYER使用 )
	virtual void PKRuleForDie( CEntityCharacter* pSrcEntity ) {};

	// 通知属性改变
	virtual void NotifyPropertyRefresh( ) = 0;

	// 是否可以通过
	virtual bool CanPass( unsigned int nPosX, unsigned int nPosY );

	// 获取名字
	virtual const char* GetName() { return ""; };

	bool CheckBuffCondition( CTemplateBuff* vpBuff, CMapObject* vpMap );



	virtual int GetCampID( ) { return 0; }
	virtual int SetCampID( int vCampID );// { return SUCCESS; }

	// 实体是否有buff
	bool HasBuff( int nBuffID ) ;

	// buff作用
	void BuffTimeEvent( CTemplateBuff* tpBuffTpl, CBuffObject* tpBuff );

	// 是否可以飞行
	bool CanFly( );

	// 是否可以被变身
	bool CanBeTransformed( );

    //处理刷新技能添加buff
    void OnBuffRefreshCD( CEntityPlayer* tpPlayer, CTemplateBuff::CBuffFunc & buffFunc);

	// 得到角色类实体等级
	int GetLevel();
};


// ********************************************************************** //
// CEntityPlayer
// ********************************************************************** //
class CEntityPlayer : public CEntityCharacter
{
public:
	enum COMM_STATE
	{
		STATE_INWAIT,					// 等待玩家进入场景
		STATE_INMAP,					// 场景中
		STATE_RETURNTOGATE,				// 返回网关中
		STATE_LEAVEGAME,				// 离开游戏中
		STATE_REDIRECT,					// 跨场景中
		STATE_KICKOUT,					// 被踢下线中
		STATE_FORCEEXIT,				// 强制退出中
		STATE_REDIRECT_INWAIT,			// 等待跨服过来的玩家进入场景,有别于STATE_INWAIT		
	};

	enum emSaveStatus
	{
		em_start_save = 1 ,
		em_finish_save = 2
	}; 

protected:
	bool			mIsExchange;			
	bool 			mIsRedirect;			// 是否是跨场景登录
	bool			mIfTracedPlayer;			// 个人日志: 相关是否是需要追踪的玩家
	bool			mProtectStatus;
	CPropertyPlayer mProperty;
	CClientInfo		mClientInfo;
	CCharProfile	mCharProfile;	// 角色概要
	unsigned int	mLoginToken;	// 登陆令牌
	COMM_STATE		mCommState;		// 连接状态
	int				mCommHandle;	// 通讯端口
	CWTimer			mWaitTimer;		// 等待连接的计时器
	CWTimer			mLeaveTimer;	// 掉线计时器
	time_t			mLastSaveTimeStamp; 
	time_t			mRedirectTimeStamp;	// 玩家状态为STATE_REDIRECT的时间戳

	int				mLoadState;		// 读取状态
	int				mSaveState;		// 存储状态
	int				mTLineID;		// 用于跨服保存目标场景信息
	int				mTMapID;		// 用于跨服保存目标场景信息
	int				mTX;			// 用于跨服保存目标场景信息
	int				mTY;			// 用于跨服保存目标场景信息
	int 			m_nProfileCheckSum;
	int 			m_nDetailCheckSum;
	unsigned int	mServerMoveTime;	// move time check
	long long		mClientMoveTime;	// move time check
	EPKMode			mPKMode;			// PK模式
	CPlayerTimerReceiver		mTimerReciver; // 定时器容器
	int				mCreatMethod;		// 进入别人视野的创建方式
	int				mRoleStatus;	// 玩家状态，低14位目前用作禁言状态位
	int				mRoleStatusParam; // 状态参数（如：禁言时间，禁止登陆时间(秒） )
	EFcmStatus 		mFcmStatus;		// 防沉迷收益0.5的倍率
	int				mMoveDistance; // 移动验证判断 : 距离
	unsigned int	mSpeedCheckTime; //移动验证判断 : 速度检查时间
	CWTPoint		mLastCheckPoint; // 上次检查点坐标
	int				mMsgCount;		 // 消息验证： 消息数量
	int				mFirstMsgTime;	 // 消息验证： 开始统计后第一条消息的时间	 
	int				mHandle;					// 死亡， pk, 攻击 是否处理特殊逻辑( 掉落, PK值变化 )
	int				mKickReason;	// 玩家如果被踢的话，具体原因
	int				mKickCode;		// 玩家被踢时，具体错误号（主要用于反外挂)
	unsigned int 	m_nGmStatus; 		// GM 权限状态
	int				mCallNpcEntityID; //  召唤出来的NPC的实体ID  下线要干掉 实时数据 不存盘
	int 			m_nValidSeconds;	// 有效益时间
	unsigned int	mCurRequestMsgSeq;	// 客户端请求消息当前序列号（递增)
	int				mUnionID;			// 联军ID
	int				mCountryID;
	emSaveStatus	mSaveProgress;
	int				mChatObjID;		// 聊天对象ID
	int				mMailObjID;	    // 邮件对象ID
	int				mTeamFlag;		// 组队标志，为队长charid
	unsigned int	mTeamID;		// 绑定TeamID
	int				mStallID;		// 绑定摆摊数据
	int				mTaskToken;	    // 绑定任务数据
	int				mFamilyID;		// 家族ID
	int				mCorpsID;		// 军团ID
	int				mHeadCorpsID;
	int				mOfficialID;
	int				mKingID;

	unsigned int	mPingTime;		// 记录上次收到PING包时间

	int 			mGameID;						// 玩家来源
	int				mMarriageID;	// 婚姻ID
	int				mHydrangeaStatus;
	int				mPetCreatorID;			// 召唤兽creator id 
	char			mKey[ KEYLEN ];	// 玩家密钥
	char			mLogName[LOGNAMELENGTH];	// 个人日志: 追踪日志名
	char			mUnionName[MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES];
	char			mRoleCreateTime[20];			// 角色创建时间
	char 			mUserName[NAME_LENGTH+1];		// 帐号名称
	int				mChangeCampTime;//转阵营cd

	PBBourseDetailList mBourseBuyLog;
	PBBourseDetailList mBourseSellLog;
	bool			mBourseLogIsNeedLoad;

	int 			m_nAfterFcmKilled;			//防沉迷之后杀死怪物的数量
	int				mFriendID;					// 好友祝福时升级好友的角色ID 

	int				mRoleWorldID;				// 角色所在区组的ID(世界ID)	
	int				mSwornID;					// 结拜ID
public:
    CPlayerInfoForYaoQianShu moYaoQianShu;  // 摇钱树活动信息

public:
    CSpeedChecker mSpeedChecker;

public:
	CEntityPlayer();

	virtual ~CEntityPlayer(); 

	virtual int Initialize()
	{
		CEntity::Initialize( );
		GetProperty()->Initial( );
		GetProperty()->SetEntityType( mEntityType );
		GetProperty()->SetEntityID( get_id() );
		ResetSpeedCheck( );

		mCommHandle =  0;
		mCommState = STATE_INWAIT; // 初始化成等待进入场景
		mWaitTimer =  1000 * 240;
		mLeaveTimer = 1000 * 240;
		mLastSaveTimeStamp = time(0);
		mTaskToken = -1;
		mIndex = 0;
        mTLineID = 0;
        mTMapID  = 0;
        mTX      = 0;
        mTY      = 0;
		mTeamFlag	= 0;
		mTeamID		= 0;
		mStallID	= 0;
		mClientMoveTime	= 0;
		mServerMoveTime = 0;
		mPKMode = PKMODE_TRAINING;
		memset( mKey, 0, sizeof( mKey ) );
		mLoadState = LSSTATE_NONE;
		mSaveState = LSSTATE_NONE;
		mFcmStatus = em_normal_rate;
		mChatObjID = 0;
		mMailObjID = 0;	
		mFamilyID = 0;
		// memset( mFamilyName, 0, sizeof( mFamilyName ) );
		// mIsFamilyHeader = false;
		// mIfHasFamilyApplies = false;
		mCorpsID = 0;		
		// memset( mCorpsName, 0, sizeof( mCorpsName ) );
		// mIsCorpsHeader = false;
		// mIfHasCorpsApply = false;	  	
		mCreatMethod = PLAYERCREAT_TELEPORT;
		mMsgCount = 0;
	    mFirstMsgTime = 0;	   	   
		mIfTracedPlayer = false;
		memset( mLogName, 0, sizeof(mLogName));
		mHandle = 0;
		m_nProfileCheckSum = 0;
		m_nDetailCheckSum = 0;
		mKickReason = 0;
		mKickCode = 0;
		// mExpGloryNeeded = 0;
		// mApGloryNeeded = 0;
		// mCorpsHeaderID = 0;
		// mFamilyHeaderID = 0;
		m_nGmStatus = 0;
		mCallNpcEntityID = 0;
		mSaveProgress = em_finish_save;
		mRoleStatus = 0;
		mRoleStatusParam = 0;
		m_nValidSeconds = -1;		// -1 是无穷大
		// mFamilyPost = -1;
		// mCorpsPost = -1;
		mCurRequestMsgSeq = 0; 
		// mFamilyContribute = 0;
		// mCorpsContribute = 0;
		// mFamilyMoney = 0;
		// mCorpsMoney = 0;
		mUnionID = 0;
		memset( mUnionName, 0, sizeof( mUnionName ) );
		mHeadCorpsID = 0;
		mCountryID = 0;
		mOfficialID = 0;
		mKingID	= 0;
		mProtectStatus = true;

		mUserName[0] = 0;
		mMarriageID = 0;
		
		mRoleCreateTime[0] = 0;
		mHydrangeaStatus = 0;
		mPetCreatorID = INVALID_OBJ_ID;

		mGameID = 4;
		mIsExchange = false;
		mIsRedirect = false;
		mChangeCampTime = 0 ;
		mBourseLogIsNeedLoad = true;
		
		m_nAfterFcmKilled = 0;
		mRoleWorldID = 0;
		mSwornID = 0;
		return 0;
	}

	virtual int Resume()
	{
		ResetSpeedCheck( );
		mBourseLogIsNeedLoad = true;
		return 0;
	}

public:
	int SetHandle( EUnHandle vUnHandle ) { mHandle ^= vUnHandle; return SUCCESS; }
	int SetUnHandle( EUnHandle vUnHandle ) { mHandle |= vUnHandle; return SUCCESS; }
	bool IsHandle( EUnHandle vUnHandle ) { return ( mHandle & vUnHandle ) == 0; }

	bool GetBourseLogIsNeedLoad( ) { return mBourseLogIsNeedLoad; }
	int SetBourseLogIsNeedLoad( bool vIsLoad ) { return mBourseLogIsNeedLoad = vIsLoad; return SUCCESS; }
	int SetBourseLog( const PBBourseDetailList& vpBuyList, const PBBourseDetailList& vpSellList );
	int BourseLogSend( );

	virtual unsigned int GetLoginTime( ){ return mClientInfo.mSockTime; }
	inline virtual CProperty* GetProperty( ) { return &mProperty; }

	int	SetClientMoveTime( long long vClientMoveTime ) { mClientMoveTime = vClientMoveTime; return SUCCESS; }
	long long GetClientMoveTime( ) { return mClientMoveTime; }

	int SetServerMoveTime( unsigned int vServerMoveTime ) { mServerMoveTime = vServerMoveTime; return SUCCESS; }
	unsigned int GetServerMoveTime( ) { return mServerMoveTime; };

	// 获得包裹中物品
	CItemObject* GetItemObject( int vIndex )
	{ 
		return ( ( CPropertyPlayer* )GetProperty( ) )->GetBaggage()->GetItemObjPtr( vIndex );
	}
	// 主人ID
	virtual unsigned int GetOwnerID();

	// 是否敌对
	virtual bool IsHostileTo( CEntity* pEntity );

	// 是否友好
	virtual bool IsFriendlyTo( CEntity* pEntity );

	// 是否是队友
	virtual bool IsTeamTo( CEntity* pEntity );	
	
	// 是否是同一家族成员
	virtual bool IsFamilyTo( CEntity *pEntity );
	
	// 是否是同一军团成员
	virtual bool IsCorpsTo( CEntity *pEntity );

	// 是否对玩家实体友好
	virtual bool IsHostileToPlayers();

	// 检查玩家是否受新手保护规则保护
	virtual bool IsBeginnerProtect();

	// 检查玩家是否受到跨场景保护规则保护
	virtual bool IsTransProtect();

	// 攻击是否黄名
	virtual bool PKIsYellow( CEntity* pDesEntity );

	// 杀人是否红名
	virtual bool PKIsRed( CEntity* pDesEntity );

	// 对目标玩家是否可见
	virtual bool IsVisibleFor( CEntity* pTarget );

	// 设置CharID
	inline unsigned int	GetCharID( )	{return mCharProfile.mCharID; }
	inline void	SetCharID( unsigned int nCharID ) { mCharProfile.mCharID = nCharID; }

	// 玩家名字
	const char*	GetCharName( ) { return mCharProfile.mCharName; }
	inline void	SetCharName( const char* pszCharName ) 
	{
		if( pszCharName != NULL )
		{
			strncpy(mCharProfile.mCharName, pszCharName, sizeof( mCharProfile.mCharName )-1);
		}
	}
	
	// 设置&获取家族首领ID
	// void SetFamilyHeaderID( int nFamilyHeaderID ){ mFamilyHeaderID = nFamilyHeaderID; }
	// int	 GetFamilyHeaderID( ){ return mFamilyHeaderID; }
	
	// 设置&获取军团首领ID
	// void SetCorpsHeaderID( int nCorpsHeaderID ){ mCorpsHeaderID = nCorpsHeaderID; }
	// int	 GetCorpsHeaderID(){ return mCorpsHeaderID; }
	
	// 设置&获取玩家跟踪状态
	void SetIfTarcePlayer( bool bValue ){ mIfTracedPlayer = bValue; }
	bool GetIfTracePlayer(  ){ return mIfTracedPlayer; }

	void SetUnionID( int nUnionID ){ mUnionID = nUnionID; }
	int	 GetUnionID( ){ return mUnionID; }

	void SetUnionName( const char *pUnionName )
	{
		strncpy( mUnionName, pUnionName, MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES -1 );
		mUnionName[MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES-1] = '\0';
	}
	const char *GetUnionName(){ return mUnionName; }

	const char *GetUnionNameShow()
	{
		// 字符集用于显示
#ifdef CHARSET_SHOW_GBK
		static char tUnionName[ MAX_CORPSNAME_LENGTH ] = {0};
		int nNameLen = sizeof(tUnionName)-1;
		charset_convert( "utf8", "gbk", (char*)mUnionName, strlen( mUnionName ),
			tUnionName, (size_t*)&nNameLen );
		return tUnionName;
#else
		return mUnionName;
#endif	
	}

	void SetHeadCorpsID( int nHeadCorpsID ){ mHeadCorpsID = nHeadCorpsID; }
	int	 GetHeadCorpsID( ){ return mHeadCorpsID; }

	// 设置&获取国籍
	void SetCountryID( int nCountryID ){ mCountryID = nCountryID; }
	int	 GetCountryID( ){ return mCountryID; }

	void SetOfficialID( int nOfficialID ){ mOfficialID = nOfficialID; }
	int	 GetOfficialID( ){ return mOfficialID; }

	void SetKingID( int nKingID ){ mKingID = nKingID; }
	int	 GetKingID( ){ return mKingID; }
	
	// 设置&获取日志名称
	void SetLogName( const char *pLogName )
	{ 
		strncpy( mLogName, pLogName, LOGNAMELENGTH -1 );
		mLogName[ LOGNAMELENGTH -1 ] = '\0';
	}
	const char *GetLogName( ){ return mLogName; }
	
	// 初始化个人日志
	int InitializeLog( );
	
	// 关闭个人日志
	int CloseLog( );
	
	// 写消息到个人日志
	int WriteMsgInfo( CMessage *pMessage, int nMsgType = 1 );
	

	// 得到GBK编码的用户名
	const char* GetCharNameShow( )
	{
		// 字符集用于显示
#ifdef CHARSET_SHOW_GBK
		static char cCharName[ NAME_LENGTH ] = {0};
		int nNameLen = sizeof(cCharName)-1;
		charset_convert( "utf8", "gbk", (char*)mCharProfile.mCharName, strlen( mCharProfile.mCharName ),
			cCharName, (size_t*)&nNameLen );
		return cCharName;
#else
		return mCharProfile.mCharName;
#endif
	}
	
	// 玩家国籍
	inline unsigned short GetNationality( )	{ return mCharProfile.mNationality; }
	inline void SetNationality( unsigned short ucNation )	{ mCharProfile.mNationality = ucNation; }

	// 得到最后一次登陆时间
	inline unsigned int GetLastLoginTime( ) { return mCharProfile.mLastLogin; }
	inline void SetLastLoginTime( unsigned int unTime ) { mCharProfile.mLastLogin = unTime; }

	// 得到在线时间
	inline unsigned int GetOnlineTime( ) { return mCharProfile.mOnlineTime; }
	inline void SetOnlineTime( unsigned int unTime ) { mCharProfile.mOnlineTime = unTime; }

	// 队伍标志
	inline virtual int GetTeamFlag( ) { return mTeamFlag; }
	inline void SetTeamFlag(int nFlag) { mTeamFlag = nFlag;}

	inline unsigned int GetTeamID( ) { return mTeamID; }
	inline void SetTeamID( int nTeamID ) { mTeamID = nTeamID; }

	// 摆摊
	inline void SetStallID( int nStallID ) { mStallID = nStallID; }
	inline int GetStallID( ) { return mStallID; }
	inline bool HasStall( ) { return ( ( mStallID > 0 ) ? true : false ); }

	// 职业
	inline unsigned char GetMetier( ) { return mCharProfile.mMetier; }
	inline void SetMetier( unsigned char ucMetier ) { mCharProfile.mMetier = ucMetier; }

	// PK模式
	inline EPKMode GetPKMode() { return mPKMode; }
	inline void SetPKMode( EPKMode vPKMode ) { mPKMode = vPKMode; }

	// 性别
	inline unsigned char GetSex( ) { return mProperty.GetSex(); }
	inline void SetSex( unsigned char ucSex ) { mProperty.SetSex( ucSex ); }

	// 等级
	inline unsigned char GetLevel( ) { return mProperty.GetLevel(); }
	inline void SetLevel( unsigned char ucLevel ) { mProperty.SetLevel( ucLevel ); }
	
	// 经验
	inline unsigned long long GetExp( ) { return mProperty.GetExp(); }
	inline void SetExp( unsigned long long nExp ) { mProperty.SetExp( nExp ); }

	// 金钱 & 绑定金钱
	inline int GetMoney() { return mProperty.GetMoney(); }
	inline int GetBindMoney() { return mProperty.GetBindMoney(); }

	inline int GetGoldenYB( ) { return mProperty.GoldenYB( ).remain( ); }
	inline CYuanBao& GetGolden( ) { return mProperty.GoldenYB( ); }
	int DecGoldenYB( int vYb ) 
	{ 
		LK_ASSERT( vYb > 0, return ERR_INVALID_PARAMETER );

		if ( mProperty.GoldenYB( ).lock( vYb ) == true )
		{
			if( mProperty.GoldenYB( ).commit( vYb ) == true )
			{
		//		GoldenNotice( );
				return SUCCESS;
			}
		}

		return ERR_NO_ENOUGH_MEMORY;
	}
	int IncGoldenYB( int vYb )
	{
		LK_ASSERT( vYb > 0 , return ERR_INVALID_PARAMETER );

		int rtn = ( mProperty.GoldenYB( ).add( vYb ) == true ) ? SUCCESS : ERR_UNKNOWN;

		if( rtn == SUCCESS )
		{
	//		GoldenNotice( );	
		}	
		
	}

	int GoldenNotice( );

	// 脸型
	inline unsigned short GetFaceType( ) { return mCharProfile.mFaceType; }
	inline void SetFaceType( unsigned short usFace ) { mCharProfile.mFaceType = usFace; } 

	// team leave
	unsigned short GetTeamLevel( );
	unsigned short GetTeamMaxLevel( );
	unsigned short GetTeamMinLevel( );

	// 改变PK模式
	int ChangePKMode( unsigned int vPKMode );

	// 得到帐号ID
	inline unsigned int GetAccountID( ) { return mCharProfile.mAccountID; }
	inline void SetAccountID( unsigned int unAccountID ) { mCharProfile.mAccountID = unAccountID; }

	// 玩家在线状态
	inline COMM_STATE GetCommState( ) { return mCommState; }
	inline void SetCommState( COMM_STATE emCommState ) {mCommState = emCommState; }

	inline int GetCommHandle() { return mCommHandle; }
	inline void SetCommHandle( int vCommHandle ) { mCommHandle = vCommHandle; }

	// 玩家服务状态
	inline void SetServiceStatus( EService emStatus ) { mProperty.SetServiceStatus( emStatus ); }
	inline EService GetServiceStatus( ) { return mProperty.GetServiceStatus(); }

	// 得到定时器虚接口
	virtual ITimerReceiver* GetTimerReciver() { return &mTimerReciver; }
	
	// 创建模式
	int		GetCreatMethod(){ return mCreatMethod; }
	void	SetCreatMethod( int nCreatMethod ){ mCreatMethod = nCreatMethod; }

	inline int GetRoleStatus( void ){ return mRoleStatus; } 
	inline void SetRoleStatus( int vRoleStatus ){ mRoleStatus = vRoleStatus; }

	inline int GetRoleStatusParam( ) { return mRoleStatusParam; }
	inline void SetRoleStatusParam( int nRoleStatusParam ) { mRoleStatusParam = nRoleStatusParam; }

	inline EFcmStatus GetFcmStatus ()  { return mFcmStatus; }
	void SetFcmStatus (EFcmStatus emStatus) ; 

	inline CClientInfo* GetClientInfo( ) { return &mClientInfo; }

	inline unsigned int GetLoginToken( ) { return mLoginToken; }
	inline void SetLoginToken( unsigned int vLoginToken ) { mLoginToken = vLoginToken; }

	inline CWTimer* GetWaitTimer( ) { return &mWaitTimer; }
	inline void SetWaitTimer( int vWaitTimer ){ mWaitTimer = vWaitTimer; }

	inline CWTimer* GetLeaveTimer( ) { return &mLeaveTimer; }
	inline void SetLeaveTimer( int vLeaveTimer ){ mLeaveTimer = vLeaveTimer; }

	inline time_t GetLastSaveTimeStamp( ){ return mLastSaveTimeStamp; } 
	inline void SetLastSaveTimeStamp( time_t vLastSaveTimeStamp ){ mLastSaveTimeStamp = vLastSaveTimeStamp; }

	inline time_t GetRedirectTimeStamp( ){ return mRedirectTimeStamp; } 
	inline void SetRedirectTimeStamp( time_t vRedirectTimeStamp ){ mRedirectTimeStamp = vRedirectTimeStamp; }

	inline int GetTaskToken( ) { return mTaskToken; }
	inline void SetTaskToken( int vTaskToken ) { mTaskToken = vTaskToken; }

	inline char* GetKey( ) { return &mKey[ 0 ]; }
	void SetKey( const char* sKey) { memcpy(mKey, sKey, KEYLEN); }


	inline int GetLoadState( ) { return mLoadState; }
	inline void SetLoadState( int vLoadState ) { mLoadState = vLoadState; }

	inline int GetSaveState( ) { return mSaveState; }
	inline void SetSaveState( int vSaveState ) { mSaveState = vSaveState; }

	inline int GetTLineID( ) { return mTLineID; }
	inline void SetTLineID( int vTLineID ) { mTLineID = vTLineID; }

	inline int GetTMapID( ) { return mTMapID; }
	inline void SetTMapID( int vTMapID ) { mTMapID = vTMapID; }

	inline int GetTX( ) { return mTX; }
	inline void SetTX( int vTX ) { mTX = vTX; }

	inline int GetTY( ) { return mTY; }
	inline void SetTY( int vTY ) { mTY = vTY; }

	int CalProfileCheckSum();
	int CalDetailCheckSum(const char* from, size_t nsize);
	inline void ResetCheckSum(){ m_nProfileCheckSum = 0; m_nDetailCheckSum = 0; }

	inline int GetCallNpcEntityID() { return mCallNpcEntityID;}
	inline void SetCallNpcEntityID( int nValue) { mCallNpcEntityID = nValue;}
	
	// 获得功勋
	inline int GetHonor( ) { return mProperty.GetHonor(); }

	// 获取/设置聊天对象ID
	int  GetChatObjID(){  return mChatObjID; }
	void SetChatObjID( int vChatObjID ){ mChatObjID = vChatObjID; }
	
	// 获取设置邮件对象ID
	int  GetMailObjID() { return mMailObjID; }
	void SetMailObjID( int vMailObjID ){ mMailObjID = vMailObjID; }
	
	// 获取设置家族ID
	void SetFamilyID( int vFamilyID ){ mFamilyID = vFamilyID; }
	int  GetFamilyID(  ){ return mFamilyID; }	


	// 设置&获取家族ID
	void SetCorpsID( int nCorpsID ){ mCorpsID = nCorpsID; }
	int	 GetCorpsID( ){ return mCorpsID; }	
	
	// 设置&获取玩家发送的消息个数
	void SetMsgCount( int nMsgCount ){ mMsgCount = nMsgCount; }
	int	 GetMsgCount(  ){ return mMsgCount; }
	
	// 设置&获取统计消息数量时第一条消息的时间
	void SetFirstMsgTime( int nTime ){ mFirstMsgTime = nTime; }
	int	 GetFirstMsgTime( ){ return mFirstMsgTime; }  

	inline void SetRoleCreateTime( const char* pSrc ) { strncpy( mRoleCreateTime, pSrc, 19 ); }
	inline char* GetRoleCreateTime() { return mRoleCreateTime; }
	
	// 设置&获取婚姻ID
	void SetMarriageID( int nMarriageID ){ mMarriageID = nMarriageID; }
	int GetMarriageID( ){ return mMarriageID; }	
	//设置转阵营cd
	void SetChangeCampTime(int nchangeCamptime){mChangeCampTime = nchangeCamptime;}
	int GetChangeCampTime(){return mChangeCampTime;}

	// 防沉迷之后杀死怪物的数量 
	void IncAfterFcmKilled(int nCount = 1);
	void ResetAfterFcmKilled() { m_nAfterFcmKilled = 0; }
	int GetAfterFcmKilled(){return m_nAfterFcmKilled;}

	void SetGmStatus(unsigned int nStatus) { m_nGmStatus = nStatus; }
	unsigned int GetGmStatus() { return m_nGmStatus; }		

	// 玩家是否来自合作伙伴
	bool FromPartner() { return mUnionName[0] == '$'; }
	int GetGameID() { return mGameID; }
	void SetGameID(int nGameID) { mGameID = nGameID; }

	// 检查消息频率
	bool CheckMsgFrequency( int nMsgLimitPerSec, int nCountLimit );
	
	// 刷新每日技术的数据
	void DailyCountRefresh( unsigned int unDate );

	// 得到技能公共CD
	inline int GetCommonCD( ) { return mProperty.GetCommonCD(); }

	// 执行技能消耗
	void DoSkillCost( CTemplateSkill* tpSkillTpl, bool bIsMagicWeapon = false );

	// 判断技能消耗是否满足
	bool CheckSkillCost( CTemplateSkill* tpSkillTpl, bool bIsMagicWeapon = false );

	// 技能是否可以使用
	int CanUseSkill( CSkillObject* tpSkillObj, bool bIsMagicWeapon= false );

	// 检查技能特殊前置条件
	int CheckSpecialUseCondition(  CTemplateSkill* tpSkillTpl, bool bIsMagicWeapon = false );

	// 技能冷却
	void DoSkillCoolDown( CSkillObject* pSkillObject, bool bIsMagicWeapon = false );

	// 打断生产，升级
	void InterruptNpcService( );

	// 判断技能武器是否满足
	bool CheckSkillWeapon( CTemplateSkill* tpSkillTpl );	

	// 通知属性改变
	virtual void NotifyPropertyRefresh( );

	// 改变红黄名时增加buff
	void ChangePKBuff( int vOldPKStatus, int vNewPKStatus, int vOldValue, int vNewValue );
	
	// 设置&获取玩家的绣球状态
	void SetHydrangeaStatus( int nStatus ){ mHydrangeaStatus = nStatus; }
	int GetHydrangeaStatus(){ return mHydrangeaStatus; }
	
public:
	// 发送使用技能回执信息
	void SendUseSkillErrorNotice( int nType );

	// 实体死亡处理
	virtual void EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf = false, bool vIsAward = true );

	// 实体死亡时,清除BUFF
	void RemoveBuffEntityDie( );

	// 处理召唤兽逻辑 ( 暂时只有PLAYER,PET使用 )
	virtual void PetLogicForDie( );
	// 获得功勋 ( 暂时只有PLAYER使用 )
	virtual void HonorForDie( CEntityCharacter* pSrcEntity );
	// PK规则 ( 暂时只有PLAYER使用 )
	virtual void PKRuleForDie( CEntityCharacter* pSrcEntity );

	// 是否可以通过
	bool CanPass( unsigned int nPosX, unsigned int nPosY );

	// 是否在国外
	bool IsOverseas(){ return ( GetLineID() != GetNationality() )&&( IS_REPETION_LINE_ID( GetLineID() ) == false )&&(GetNationality() != 0) ;}


	// 清除速度验证数据
	int SpeedCheckTeleport( );
	int ResetSpeedCheck( );

	int SetMoveDistance( unsigned int vLength )
	{
//		mMoveDistance += vLength;
		mMoveDistance++;

		return SUCCESS;
	}

	unsigned int GetSpeekCheckTime( ) { return mSpeedCheckTime; }
	int GetMoveDistance( ) { return mMoveDistance; }

	int SpeedCheck( bool vIsCoerce = false );

	bool RefrushPingTime( );

	// 获取名字
	virtual const char* GetName() { return GetCharName(); };

#ifdef _FB_
	virtual int GetCampID( );
#endif

#ifdef _FB_
	virtual int SetCampID( int vCampID )
	{
		CPlayerRepetion* tpRepetion = ( ( CPropertyPlayer* )GetProperty( ) )->GetRepetion( );
		CEntityCharacter::SetCampID( vCampID );
		return tpRepetion->SetCampID( vCampID );	
	}
#endif
	
	// 开启活动双倍时间, nRate以SERVER_PERCENT_INT(10000)为基准
	void OpenActiveDoubleTime( int nRate);

	// 关闭活动双倍时间
	void CloseActiveDoubleTime( ) ;

	// 得到宠物ID
	int GetPetID( ) { return mProperty.GetPetID(); }
	void SetPetID( int nPetID ) { mProperty.SetPetID( nPetID ) ;}

	// 得到宠物
	CEntityPet* GetPet( );

	// 玩家设置过的定时器超时，以便让剩余时间存盘
	//void TimeoutTimerForSaveData(  );

	// 获取定时器的剩余时间
	void GetLeftTimeofPlayerTimer( ); 

	// 判断是否是新建角色
	bool IsNewCharacter( );

	// 设置被踢原因
	void SetKickoutReason( int nReason ) { mKickReason = nReason; }
	int GetKickoutReason( ) { return mKickReason; }

	void SetKickoutCode( int nCode ) { mKickCode = nCode; }
	int GetKickoutCode( ) { return mKickCode; }

	// 刷新CD组
	int RefreshCDGroup( int *vCDGroup, int vNum );

	// 检查CD组
	int CheckCDGroup( int *vCDGroup, int vNum );

	// 清除CD组
	int ClearCDGroup( int *nCDGroup, int nNum );

	// 跨地图时检查buff
	int CheckBuffOnChangeMap( CMapObject* vpMap );
	
	void StartSave() { mSaveProgress = em_start_save; }
	void FinishSave() { mSaveProgress = em_finish_save; }
	bool IsSaving() { return mSaveProgress == em_start_save; }
	bool HasSaved() { return mSaveProgress == em_finish_save; }

	void SetValidSeconds(int nSeconds) { m_nValidSeconds = nSeconds; }
	int  GetValidSeconds() { return m_nValidSeconds; }

	inline unsigned int GetCurRequestSeq( ) { return mCurRequestMsgSeq; }
	inline void SetCurRequestSeq( unsigned int unSeq ) { mCurRequestMsgSeq = unSeq; }

	inline void SetUserName(const char* szUserName) 
	{
		memcpy(mUserName, szUserName, sizeof(mUserName));
		mUserName[sizeof(mUserName)-1] = 0;

		if ( szUserName[0] == '$')
		{
			char buffer[12] = {0};
			memcpy(buffer, szUserName, 4);
			mGameID = atoi(&buffer[1]);
		}
	}

	inline const char* GetUserName() const { return mUserName; }

	void SetProtectStatus( bool vStatus ){ mProtectStatus = vStatus; }
	int	 GetProtectStatus( ){ return mProtectStatus; } 

	bool CanRemainFly( );

	// 获得召唤兽creator id
	inline int GetPetCreatorID() { return mPetCreatorID; }
	inline void SetPetCreatorID( int nPetCreatorID ) { mPetCreatorID = nPetCreatorID; }

	// 注意，仅仅用于记录物品消失是销毁还是交易
	bool IsExchange() { return mIsExchange; }
	void SetExchange(bool flag) { mIsExchange = flag; }

	bool IsRedirect() { return mIsRedirect; }
	void SetRedirect(bool flag) { mIsRedirect = flag; }

	// 设置升级的好友角色ＩＤ
	void SetFriendID( int nFriendID ){ mFriendID = nFriendID; }
	int GetFriendID(){ return mFriendID; }

	void SetRoleWorldID( int nWorldID ) { mRoleWorldID = nWorldID; }
	int GetRoleWorldID() { return mRoleWorldID; }

	void SetSwornID( int nSwornID )	{ mSwornID = nSwornID; }
	int GetSwornID(){ return mSwornID; }
};

// ********************************************************************** //
// CEntityNpc
// ********************************************************************** //
class CEntityNpc : public CEntityCharacter
{
protected:
	bool			mSuperState;	// 超人状态
	unsigned short	mTaskID;		// 变身的相关任务 不是任务发起的 填0
	unsigned int	mNpcID;			// 如果NPC类型为ENTITYTYPE_OGRE,	那么该ID代表怪物模板ID
									// 如果NPC类型为ENTITYTYPE_FUNCNPC, 那么该ID代表NPC模板ID
									// 如果NPC类型为ENTITYTYPE_PEDAL,	那么该ID等于0

	CWTPoint		mOrginPos;		// 出生点
	int				mOrginMapID;	// 出生地图ID
	int				mOccurTimer;	// 存活的计时器
	int				mOccurTime;		// 存活的时间
	CNpcTimerReceiver mTimerReceiver; // NPC定时容器
	int				mPolicyID;		// 策略ID
	int				mPKType;		// pk类型
	int				mGroupID;
	int				mNpcChangeState;// 功能NPC变怪物状态 1：变怪了 0：没变
	int				mOwnerCharID;	// 归属者角色ID 兼做是哪个玩家让他变身的  0表示无归属
	int				mSendNpcState;	// 护送NPC状态 这个值可能会被任务模板动态修改  0=不是护送NPC 1=是护送NPC
	int				mCallerEntityID;	// 召唤NPC的人 在这个人召唤下一个NPC的时候 本NPC要被干掉
	unsigned int	mNationality;	// 功能NPC的国籍 默认是Line ID 如果是护送NPC 则是所有者的nation id
	#ifdef _FB_
	int				mCampID;
	#endif
	int				mIsShowOwner;	// NPC头顶是否显示所有者
	int				mOwnCampID;		// 所属阵营ID

	int				mTranChangeLevel;

	CSeenCharactorNpcContainer mSeenNpcContainer; // 可见npc容器(主要用于主动攻击计算) 
public:

	CEntityNpc( EEntityType vEntityType ) : CEntityCharacter( vEntityType )
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
	virtual ~CEntityNpc( )
	{}
		
	virtual int Initialize( )
	{
		CEntity::Initialize( );
		mOrginMapID = 0;
		mSuperState = false;
		mCreatorID = 0;
		mOccurTimer = 0;
		mPolicyID = 0;		

		mOccurTime = 0;

		mPKType = 0;
		mGroupID = 0;

		mNpcChangeState = 0;
		mOwnerCharID = 0;
		mTaskID = 0;

		mSendNpcState = 0;
		#ifdef _FB_
		mCampID = 0;
		#endif
		mIsShowOwner = 0;
		mCallerEntityID = 0;
		mNationality = 0;
		mNpcID = 0;
		mOwnCampID = 0;
		return 0;
	}

	virtual int Resume( )
	{
		return 0;
	}

public:
	inline void SetLevel( unsigned char ucLevel ) { GetProperty( )->SetLevel( ucLevel ); }
	inline int GetLevel( ) { return GetProperty( )->GetLevel( ); }

	// 加入可见表
	virtual bool AddViewEntity( CEntity* pTarget );

	// 从可见表删除
	virtual bool DeleteViewEntity( CEntity* pTarget );

	// 可见npc容器
	inline CSeenCharactorNpcContainer& GetSeenNpcsContainer() { return mSeenNpcContainer;}

	virtual unsigned short GetNationality( ) { return mNationality; }
	void SetNationality( unsigned short nVal ) { mNationality = nVal; }
	// 队伍标志
	virtual int GetTeamFlag( );

	// 主人ID
	virtual unsigned int GetOwnerID();

	// 是否敌对
	virtual bool IsHostileTo( CEntity* pEntity );

	// 是否友好
	virtual bool IsFriendlyTo( CEntity* pEntity );

	// 是否是队友
	virtual bool IsTeamTo( CEntity* pEntity );
	
	// 是否是同一家族成员
	virtual bool IsFamilyTo( CEntity* pEntity );
	
	// 是否是同一军团成员
	virtual bool IsCorpsTo( CEntity *pEntity ){ return false; }

	// 是否对玩家实体友好
	virtual bool IsHostileToPlayers();

	// 检查是否受新手保护规则保护
	virtual bool IsBeginnerProtect() { return false; };

	// 检查是否受到跨场景保护规则保护
	virtual bool IsTransProtect()  { return false; };
	
	// 攻击是否黄名
	virtual bool PKIsYellow( CEntity* pDesEntity ) { return false; }

	// 杀人是否红名
	virtual bool PKIsRed( CEntity* pDesEntity ) { return false; }

	virtual ITimerReceiver* GetTimerReciver() { return &mTimerReceiver; }

	virtual unsigned int GetNpcID( ) { return mNpcID; }
	virtual void SetNpcID( unsigned int vNpcID ) { mNpcID = vNpcID; }

	inline CWTPoint GetOrginPos( ) { return mOrginPos; }
	inline void SetOrginPos( CWTPoint vOrginPos ) { mOrginPos = vOrginPos; }

	inline int GetOrginMapID( ) { return mOrginMapID; }
	inline void SetOrginMapID( int vMapID ) { mOrginMapID = vMapID; }

	inline bool GetSuperState( ) { return mSuperState; }
	inline void SetSuperState( bool vSuperState ) { mSuperState = vSuperState; }

	inline int GetOccurTime( ) { return mOccurTime; }
	inline void SetOccurTime( int vOccurTime ) { mOccurTime = vOccurTime; }

	inline int GetOccurTimer( ) { return mOccurTimer; }
	inline void SetOccurTimer( int vOccurTimer ) { mOccurTimer = vOccurTimer; }

	inline int GetPolicyID( ) { return mPolicyID; }
	inline void SetPolicyID( int vPolicyID ) { mPolicyID = vPolicyID; }

	inline int GetPKType( ) { return mPKType; }
	inline void SetPKType( int vPKType ) { mPKType = vPKType; }

	inline int GetNpcChangeState( ) { return mNpcChangeState; }
	inline void SetNpcChangeState( int vState ) { mNpcChangeState = vState; }

	inline int GetOwnercharID( ) { return mOwnerCharID; }
	inline void SetOwnerCharID( int vCharID ) { mOwnerCharID = vCharID; }

	inline int GetTaskID( ) { return mTaskID; }
	inline void SetTaskID( int vTaskID ) { mTaskID = vTaskID; }

	inline int GetSendNpcState( ) { return mSendNpcState; }
	inline void SetSendNpcState( int vState ) { mSendNpcState = vState; }

	inline int GetIsShowOwner( ) { return mIsShowOwner; }
	inline void SetIsShowOwner( int vState ) { mIsShowOwner = vState; }

	inline int GetCallerEntityID( ) { return mCallerEntityID; }
	inline void SetCallerEntityID( int vID ) { mCallerEntityID = vID; }

	virtual bool IsTaskNpc() { return mSendNpcState == 1; }

	virtual void EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf = false, bool vIsAward = true );

	virtual int ChangeNpcByLevel( unsigned int vLevel );

	inline int GetGroupID( ) { return mGroupID; }

	inline int GetOwnCampID( ) { return mOwnCampID; }
	inline void SetOwnCampID( int vOwnCampID ) { mOwnCampID = vOwnCampID; }

public:
	// 发送使用技能回执信息
	virtual void SendUseSkillErrorNotice( int nType ) { };	
	
	// 处理怪物死亡 ( 暂时只有OGRE,FUNCNPC,PET用 )
	virtual void TimeForDie( );

	// 通知属性改变
	virtual void NotifyPropertyRefresh( );
	
	// 获取名字
	virtual const char* GetName();

	#ifdef _FB_
	virtual int GetCampID( ) { return mCampID; }
	virtual int SetCampID( int vCampID ) { mCampID = vCampID; CEntityCharacter::SetCampID( vCampID ); return SUCCESS; }
	#endif
};




// ********************************************************************** //
// CEntityOgre
// ********************************************************************** //
class CEntityOgre : public CEntityNpc
{
private:
	CPropertyNPC	mProperty;


public:
	CEntityOgre() : CEntityNpc( ENTITYTYPE_OGRE )
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

	CEntityOgre( EEntityType vEntityType ) : CEntityNpc( vEntityType )
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

	virtual ~CEntityOgre()
	{}

	virtual int Initialize()
	{
		GetProperty()->Initial( );
		GetProperty()->SetEntityType( mEntityType );
		GetProperty()->SetEntityID( get_id() );
		return 0;
	}

	virtual int Resume()
	{
		return 0;
	}

public:
	inline virtual CProperty* GetProperty( ) { return &mProperty; }
	// 主人ID
	virtual unsigned int GetOwnerID();

	// 是否敌对
	virtual bool IsHostileTo( CEntity* pEntity );

	// 是否友好
	virtual bool IsFriendlyTo( CEntity* pEntity );

	// 是否是队友
	virtual bool IsTeamTo( CEntity* pEntity );
	
	// 是否是同一家族成员
	virtual bool IsFamilyTo( CEntity* pEntity );
	
	// 是否是同一军团成员
	virtual bool IsCorpsTo( CEntity *pEntity ){ return false; }

	// 是否对玩家实体友好
	virtual bool IsHostileToPlayers();

	// 用创建器为实体赋值
	virtual bool Setup( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos = NULL, bool bInit = false, int vNpcID = 0 );	
	
	// 怪物死亡,玩家获得经验 ( 暂时只有OGRE用 )
	virtual void ExpForDie( CEntityCharacter* pSrcEntity );
	// 怪物死亡,掉落物品 ( 暂时只有OGRE用 )
	virtual void DropForDie( CEntityCharacter* pSrcEntity, int vMaxTeam );
	// 处理怪物死亡 ( 暂时只有OGRE,FUNCNPC,PET用 )
	virtual void TimeForDie( );

	virtual void EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf = false, bool vIsAward = true );
};

class CEntityBox : public CEntityOgre
{
public:
//	CPropertyBox  mBoxProperty;

public:
	CEntityBox( ) : CEntityOgre( ENTITYTYPE_BOX )
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
	
//	virtual int Initialize()
//	{

//		GetProperty()->Initial( );
//		GetProperty()->SetEntityType( mEntityType );
//		GetProperty()->SetEntityID( get_id() );
//		return 0;
//	}

//	virtual int Resume()
//	{
//		return 0;
//	}

//	virtual CProperty* GetProperty( ) { return &mBoxProperty; }
	// 国家ID
	virtual unsigned short GetNationality( ) { return GetLineID( ); }

	// 是否敌对
	virtual bool IsHostileTo( CEntity* pEntity ) { return false; }

	// 是否友好
	virtual bool IsFriendlyTo( CEntity* pEntity ) { return false; }

	// 是否是队友
	virtual bool IsTeamTo( CEntity* pEntity ) { return false; } 
	
	// 是否是同一家族成员
	virtual bool IsFamilyTo( CEntity* pEntity ){ return false; }
	
	// 是否是同一军团成员
	virtual bool IsCorpsTo( CEntity *pEntity ){ return false; }

	// 是否对玩家实体友好
	virtual bool IsHostileToPlayers() { return true; };

//	virtual ITimerReceiver* GetTimerReciver() { return NULL; }

	virtual int GetTeamFlag( ) { return TEAM_NONE; }

	// 主人ID
	virtual unsigned int GetOwnerID() { return GetEntityID( ); }

	// 获取名字
	virtual const char* GetName() { return ""; };


	virtual void EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf = false, bool vIsAward = true );
};

class CEntityBlock : public CEntityOgre
{
public:
//	CPropertyBlock  mBlockProperty;

public:
	CEntityBlock( ) : CEntityOgre( ENTITYTYPE_BLOCK )
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
	
//	virtual int Initialize()
//	{

//		GetProperty()->Initial( );
//		GetProperty()->SetEntityType( mEntityType );
//		GetProperty()->SetEntityID( get_id() );
//		return 0;
//	}

//	virtual int Resume()
//	{
//		return 0;
//	}

//	virtual CProperty* GetProperty( ) { return &mBlockProperty; }
	// 国家ID
	virtual unsigned short GetNationality( ) { return GetLineID( ); }

	// 是否敌对
	virtual bool IsHostileTo( CEntity* pEntity ) { return false; }

	// 是否友好
	virtual bool IsFriendlyTo( CEntity* pEntity ) { return false; }

	// 是否是队友
	virtual bool IsTeamTo( CEntity* pEntity ) { return false; }
	
	// 是否是同一家族成员
	virtual bool IsFamilyTo( CEntity* pEntity ){ return false; } 
	
	// 是否是同一军团成员
	virtual bool IsCorpsTo( CEntity *pEntity ){ return false; }

	// 是否对玩家实体友好
	virtual bool IsHostileToPlayers() { return true; };

//	virtual ITimerReceiver* GetTimerReciver() { return NULL; }

	virtual int GetTeamFlag( ) { return TEAM_NONE; }

	// 主人ID
	virtual unsigned int GetOwnerID() { return GetEntityID( ); }

	// 获取名字
	virtual const char* GetName() { return ""; };


	virtual void EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf = false, bool vIsAward = true );

	// 移除阻挡npc的动态阻挡 
	void RemoveBlock( );

	// 处理阻挡的开关
	int ChangeMode( );

};


// ********************************************************************** //
// CEntityFuncNpc
// ********************************************************************** //
class CEntityFuncNpc : public CEntityNpc
{
private:
	CPropertyNPC	mProperty;

public:
	CEntityFuncNpc( ) : CEntityNpc( ENTITYTYPE_FUNCNPC )
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
	inline virtual CProperty* GetProperty( ) { return &mProperty; }
	// 用创建器为实体赋值

	virtual bool Setup( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos = NULL, bool bInit = false, int vNpcID = 0 );

	virtual int Initialize( )
	{
//		CEntity::Initailize( );
//		mEntityType = ENTITYTYPE_FUNCNPC;

		GetProperty()->Initial( );
		GetProperty()->SetEntityType( mEntityType );
		GetProperty()->SetEntityID( get_id() );
		return 0;
	}

	virtual int Resume( )
	{
		return 0;
	}

	virtual void TimeForDie( );

	virtual void EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf = false, bool vIsAward = true );
	virtual bool IsFriendlyTo( CEntity* pEntity );

};

// ********************************************************************** //
// CEntityPet
// ********************************************************************** //
class CEntityPet : public CEntityNpc
{
protected:
	unsigned int mOwnerID;		// 召唤者的ID
	unsigned int mOnlineTime;	// 召唤兽的在线时间
	int			 mCurTargetID;	// 召唤兽当前目标ID
	int			 mCurLevel;		// 召唤兽当前临时等级
	int			 mCurMode;		// 当前模式

private:
	CPropertyPet	mProperty;


public:

	CEntityPet() : CEntityNpc( ENTITYTYPE_PET )
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

	virtual ~CEntityPet( )
	{}
	
	virtual int Initialize( )
	{
//		CEntity::Initailize( );
		mSuperState = false;
		mOwnerID = 0;
		mOnlineTime = 0;
		mCurTargetID = 0;
		mCurLevel = CTemplateSummonBeast::PET_LEVEL_ONE;
		mCurMode = 0;

		GetProperty()->Initial( );
		GetProperty()->SetEntityType( mEntityType );
		GetProperty()->SetEntityID( get_id() );

		return 0;
	}

	virtual int Resume( )
	{
		return 0;
	}

public:
	//virtual unsigned short GetMapID( )	{ return mMapID; }
	//virtual unsigned short GetLineID( ) { return mLineID; }
	inline virtual CProperty* GetProperty( ) { return &mProperty; }

	// 组队状态
	virtual int GetTeamFlag( );

	// 主人ID
	virtual unsigned int GetOwnerID();

	// 是否敌对
	virtual bool IsHostileTo( CEntity* pEntity );

	// 是否友好
	virtual bool IsFriendlyTo( CEntity* pEntity );

	// 是否是队友
	virtual bool IsTeamTo( CEntity* pEntity );
	
	// 是否是同一军团成员
	virtual bool IsCorpsTo( CEntity *pEntity ){ return false; }
	
	// 是否是同一家族成员
	virtual bool IsFamilyTo( CEntity* pEntity ){ return false; }

	// 是否对玩家实体友好
	virtual bool IsHostileToPlayers(); 

	virtual void EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf = false, bool vIsAward = true );

	bool Setup(unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos = NULL, bool bInit  = false, int vNpcID = 0 );

	//inline unsigned int GetOwnerID( ) { return mOwnerID; }
	inline void SetOwnerID( unsigned int vOwnerID ) { mOwnerID = vOwnerID; }

	inline unsigned int GetOnlineTime( ) { return mOnlineTime; }
	inline void SetOnlineTime( unsigned int vOnlineTime ) { mOnlineTime = vOnlineTime; }

	inline int GetCurTargetID( ) { return mCurTargetID; }
	inline void SetCurTargetID( int vCurTargetID ) { mCurTargetID = vCurTargetID; }

	// 临时等级
	inline int GetCurLevel( ) { return mCurLevel; }
	inline void SetCurLevel( int vCurLevel ) { mCurLevel = vCurLevel; }	

	inline int GetCurMode( ) { return mCurMode; }
	inline void SetCurMode( int vCurMode ) { mCurMode = vCurMode; }	

	// 处理召唤兽逻辑 ( 暂时只有PLAYER,PET使用 )
	virtual void PetLogicForDie( );

	// 是否可以通过
	bool CanPass( unsigned int nPosX, unsigned int nPosY );

	// 获取名字
	virtual const char* GetName() { return ""; };

	virtual unsigned short GetNationality( )
	{
		CEntityPlayer *pOwner = (CEntityPlayer *)CSceneObjManager::GetSingleton().GetObject( mOwnerID );
		if ( pOwner != NULL )
		{
			return pOwner->GetNationality();
		}
		
		return GetLineID( ); 
	}

#ifdef _FB_
	int GetCampID( ) 
	{ 
		CEntity* tpOwner = GetOwner( );
		if( tpOwner != NULL )
		{
			LK_ASSERT_STR( tpOwner->IsCharacter( ) == true, return 0, "EntityType %d", tpOwner->GetEntityType( ) );
			return ( ( CEntityCharacter* )tpOwner )->GetCampID( );
		}

		return 0;
	}
#endif
};


// ********************************************************************** //
// CEntityPedal
// ********************************************************************** //
class CEntityPedal : public CEntity
{
protected:
	unsigned short	mDstLineID;			// 如果是传送踏板，那么传送的目标线ID
	unsigned short	mDstMapID;			// 如果是传送踏板，那么传送的目标地图ID
	unsigned short	mDstMapIndex;		// 如果是传送踏板，那么传送的目标地图index
	unsigned short	mSkillID;			// 如果是陷阱踏板，对应技能ID
	unsigned short	mSkillLevel;		// 如果是陷阱踏板，对应技能等级
	unsigned int	mNpcID;
	unsigned int	mOwnerID;			// 释放者ID
	unsigned int	mTeamID;			// 所属队伍ID
										// TODO 后续添加团队,帮派,家族,国家等
	int				mPedalType;			// 踏板类型
	CWTRect			mSrcRect;			// 踏板的触发源矩形
	CWTRect			mActRect;			// 踏板的作用矩形
	CWTPoint		mDesPos;			// 如果是传送踏板，那么传送的目标坐标
	int				mSrcEntityID;		// 如果是陷阱踏板，释放者体ID
	int				mPresistTime;		// 如果是陷阱踏板，持续时间
	unsigned int	mMinLevel;			// 限制传送的等级
	unsigned int	mRestrictNation;	// 传送限制的国家
	CPropertyPedal  mProperty;
	unsigned int	mDelayTimer;		// 传送踏板触发间隔时间
	//unsigned short  mLineID;
	//unsigned short 	mMapID;
	//unsigned short	mIndex;
public:

	CEntityPedal() : CEntity( ENTITYTYPE_PEDAL )
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

	virtual ~CEntityPedal( )
	{
	}

	virtual int Initialize( )
	{
		mOwnerID = 0;
		mSkillID = 0;
		mSkillLevel = 0;
		mTeamID = 0;

		GetProperty()->Initial( );
		GetProperty()->SetEntityType( mEntityType );
		GetProperty()->SetEntityID( get_id() );

		mDelayTimer = 0;
		mVisibility = VISIBLITY_TEAM;
		return 0;
	}

	virtual int Resume( )
	{	
		return 0;
	}

	virtual void EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf = false, bool vIsAward = true );

	inline void SetOwnerID( unsigned int unOwnerID ) { mOwnerID = unOwnerID; }

	inline unsigned int GetTeamID( ) { return mTeamID; }
	inline void SetTeamID( unsigned int unTeamID ) { mTeamID = unTeamID; }
	
	inline int GetPedalType( ) { return mPedalType; }
	inline void SetPedalType( int nPedal ) { mPedalType = nPedal; }

	inline bool IsTeleport( ) { return mPedalType == PEDAL_TELEPORT; }		// 是否为传送NPC
	inline bool IsTrap( ) { return mPedalType == PEDAL_TRAP; }				// 是否为陷阱
	inline bool IsHierogram( ) { return mPedalType == PEDAL_HIEROGRAM; }	// 是否为神符
	inline bool IsNeedFire( ) { return  mPedalType == PEDAL_NEEDFIRE; }     //是否是火焰

	inline CWTRect& GetSrcRect( ) { return mSrcRect; }
	inline void SetSrcRect( CWTRect& SrcRect ) { mSrcRect = SrcRect; }

	inline CWTRect& GetActRect( ) { return mActRect; }
	inline void SetActRect( CWTRect& ActRect ) { mActRect = ActRect; }

	inline CWTPoint& GetDesPos( ) { return mDesPos; }
	inline void SetDesPos( CWTPoint& DesPos ) { mDesPos = DesPos; }
	
	inline unsigned short GetDstLineID( ) { return mDstLineID; }
	inline void SetDstLineID( unsigned short usDstLine ) { mDstLineID = usDstLine; }

	inline unsigned short GetDstMapID( ) { return mDstMapID; }
	inline void SetDstMapID( unsigned short usDstLine ) { mDstMapID = usDstLine; }

	inline int GetSrcEntityID( ) { return mSrcEntityID; }
	inline void SetSrcEntityID( int nSrcEntityID ) { mSrcEntityID = nSrcEntityID; }

	inline int GetPresistTime( ) { return mPresistTime; }
	inline void SetPresistTime( int nPresist ) { mPresistTime = nPresist; }

	inline unsigned short GetSkillID( ) { return mSkillID; }
	inline void SetSkillID( unsigned short usSkillID ) { mSkillID = usSkillID; }

	inline unsigned short GetSkillLevel( ) { return mSkillLevel; }
	inline void SetSkillLevel( unsigned short usSkillLevel ) { mSkillLevel = usSkillLevel; }

	inline unsigned int GetMinLevel( ) { return mMinLevel; }
	inline void SetMinLevel( unsigned int unMinLevel ) { mMinLevel = unMinLevel ;}

	inline unsigned int GetmRestrictNation( ) { return mRestrictNation; }
	inline void SetRestrictNation( unsigned int unNation ) { mRestrictNation = unNation; }

	inline unsigned int GetDelayTimer() { return mDelayTimer; }
	inline void SetDelayTimer( unsigned int vDelayTimer ) { mDelayTimer = vDelayTimer; }

public:

	virtual unsigned short GetNationality( ) { return mLineID; }
	inline virtual CProperty* GetProperty( )	{ return &mProperty; }

	virtual unsigned int GetNpcID( ) { return mNpcID; }
	virtual void SetNpcID( unsigned int vNpcID ) { mNpcID = vNpcID; }

	virtual int GetTeamFlag( )
	{
		CEntityPlayer* pOwner = (CEntityPlayer*) CSceneObjManager::GetSingletonPtr()->GetObject( mOwnerID );
		return ( pOwner != NULL ) ? pOwner->GetTeamFlag() : TEAM_NONE;
	} 

	// 主人ID
	virtual unsigned int GetOwnerID();

	// 是否敌对
	virtual bool IsHostileTo( CEntity* pEntity );

	// 是否友好
	virtual bool IsFriendlyTo( CEntity* pEntity );

	// 是否是队友
	virtual bool IsTeamTo( CEntity* pEntity );
	
	// 是否是同一家族成员
	virtual bool IsFamilyTo( CEntity* pEntity ){ return false; }
	
	// 是否是同一军团成员
	virtual bool IsCorpsTo( CEntity *pEntity ){ return false; }
												 
	// 是否对玩家实体友好
	virtual bool IsHostileToPlayers();
	
	// 建立
	virtual bool Setup(unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos = NULL, bool bInit  = false, int vNpcID = 0 );
	
	// 对目标玩家是否可见
	virtual bool IsVisibleFor( CEntity* pTarget );

	virtual ITimerReceiver* GetTimerReciver() { return NULL; };
public:
	// 发送使用技能回执信息
	virtual void SendUseSkillErrorNotice( int nType ) { };

	// 陷阱触发
	bool IsTouch( int vTickOffset );

	// 实体是否处于陷阱/传送点上
	bool IsTouchOn( CEntity* pEntity );

	// 传送动作
	void TeleAction( int vTickOffset );

	// 陷阱动作
	bool TrapAction( );

	// 神符动作
	bool HierogramAction( );

	//火焰动作
	bool NeedfireAction( );

	// 神符发动
	void OnEntityHierogram( CEntityPlayer* pPlayer );

	// 陷阱BUFF作用
	void PedalBuffSkill( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateTrapSkill* pTrapSkill, int vSkillType = 0 );
	
	// 陷阱发动
	void OnEntityTrap( );

#ifdef _FB_
	int GetCampID( ) 
	{ 
		CEntity* tpOwner = GetOwner( );
		// Player离开Scene时候, Owner可能为空
		if( tpOwner != NULL )
		{
			LK_ASSERT_STR( tpOwner->IsCharacter( ) == true, return 0, "EntityType %d", tpOwner->GetEntityType( ) );
			return ( ( CEntityCharacter* )tpOwner )->GetCampID( );
		}

		return 0;
	}
#endif

};


// ********************************************************************** //
// CEntityDrop
// ********************************************************************** //
class CEntityDrop : public CEntity
{
protected:
	unsigned int	mNpcID;
	//unsigned short mLineID;
	//unsigned short mMapID;
	//unsigned short mIndex;

	CNpcTimerReceiver mTimerReceiver;		// NPC定时容器
public:
	//virtual unsigned short GetMapID( ) { return mMapID; }
	//virtual unsigned short GetLineID( ) { return mLineID; }
	//virtual unsigned short GetMapIndex( ) { return mIndex; }
	//virtual void SetMapID( unsigned short vMapID ) { mMapID = vMapID; }
	//virtual void SetIndex( unsigned short vIndex ) { mIndex = vIndex; }
	//virtual void SetLineID( unsigned short vLineID ) { mLineID = vLineID; }

	virtual unsigned short GetNationality() { return mLineID; }

	virtual unsigned int GetNpcID( ) { return mNpcID; }
	virtual void SetNpcID( unsigned int vNpcID ) { mNpcID = vNpcID; }
	
public:
	CEntityDrop( EEntityType vEntityType ) : CEntity( vEntityType )  
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

	virtual ~CEntityDrop()
	{}
	
	virtual int Initialize()
	{
		mNpcID = 0;
		mLineID = 0;
		mMapID = 0;
		mIndex = 0;
		return 0;
	}
	
	virtual int Resume() 
	{
		return 0;
	}

	inline virtual int GetTeamFlag( ) { return TEAM_NONE; }
	// 主人ID
	virtual unsigned int GetOwnerID();

	// 是否敌对
	virtual bool IsHostileTo( CEntity* pEntity );

	// 是否友好
	virtual bool IsFriendlyTo( CEntity* pEntity );

	// 是否是队友
	virtual bool IsTeamTo( CEntity* pEntity );
	
	// 是否是同一军团成员
	virtual bool IsCorpsTo( CEntity *pEntity ){ return false; }
	
	// 是否是同一家族成员
	virtual bool IsFamilyTo( CEntity* pEntity ){ return false; }

	// 是否对玩家实体敌对
	virtual bool IsHostileToPlayers();
	virtual ITimerReceiver* GetTimerReciver() { return &mTimerReceiver; }

public:
	// 发送使用技能回执信息
	virtual void SendUseSkillErrorNotice( int nType ) { };
};

// ********************************************************************** //
// CEntityItem
// ********************************************************************** //
class CEntityItem : public CEntityDrop
{
private:
	CPropertyItem	mProperty;
public:
	CEntityItem() : CEntityDrop( ENTITYTYPE_ITEM )
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
	virtual ~CEntityItem()	{}

	virtual int Initialize() 
	{
		GetProperty()->Initial( );
		GetProperty()->SetEntityType( mEntityType );
		GetProperty()->SetEntityID( get_id() );
		return 0;
	}

	virtual bool Setup(unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator, CWTPoint* pPos  = NULL , bool bInit  = false, int vNpcID = 0 );
	
	virtual int  Resume( )
	{
		return 0;
	}

	inline virtual CProperty* GetProperty( ) { return &mProperty; }
};

// ********************************************************************** //
// CEntityMoney
// ********************************************************************** //
class CEntityMoney : public CEntityDrop
{
private:
	CPropertyMoney	mProperty;
public:
	CEntityMoney() : CEntityDrop( ENTITYTYPE_MONEY )
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
	virtual ~CEntityMoney() {}

	virtual bool Setup(unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator, CWTPoint* pPos  = NULL , bool bInit  = false, int vNpcID = 0 );

	virtual int Initialize()
	{

		GetProperty()->Initial( );
		GetProperty()->SetEntityType( mEntityType );
		GetProperty()->SetEntityID( get_id() );
		return 0;
	}

	virtual int Resume( )
	{
		return 0;
	}

	inline virtual CProperty* GetProperty( ) { return &mProperty; }
};

//
//// ********************************************************************** //
//// CEntityTaskNpc			任务招出的NPC
//// ********************************************************************** //
//class CEntityTaskNpc : public CEntityNpc
//{
//public:
//	CEntityTaskNpc( ) : CEntityNpc( ENTITYTYPE_TASKNPC )
//	{
//		if( CObj::msCreateMode )
//		{
//			Initialize( );
//		}
//		else
//		{
//			Resume( );
//		}
//	}
//
//	//virtual CProperty* GetProperty( ) { return &mProperty; }
//	// 用创建器为实体赋值
//
//	//virtual bool Setup( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos = NULL, bool bInit = false );
//
//	virtual int Initialize( )
//	{
//		return 0;
//	}
//
//	virtual int Resume( )
//	{
//		return 0;
//	}
//};


// ********************************************************************** //
// CEntityCollect			采集的NPC
// ********************************************************************** //
class CEntityCollect : public CEntityNpc
{
private:
	CPropertyNPC	mProperty;
protected:	
	int mNumber;	// 被采的次数

public:
	CEntityCollect( ) : CEntityNpc( ENTITYTYPE_COLLECT )
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

	inline virtual CProperty* GetProperty( ) { return &mProperty; }
	// 用创建器为实体赋值

	virtual bool Setup( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos = NULL, bool bInit = false, int vNpcID = 0 );

	virtual int Initialize( )
	{
		mNumber = 0;

		GetProperty()->Initial( );
		GetProperty()->SetEntityType( mEntityType );
		GetProperty()->SetEntityID( get_id() );
		return 0;
	}

	virtual int Resume( )
	{
		return 0;
	}

	inline int GetNumber( ){ return mNumber; }
	inline void SetNumber( int vNumber ){ mNumber = vNumber; }
};

// ********************************************************************** //
// CEntityRedStone
// 内丹只能在地面上使用不能放入到包裹
// ********************************************************************** //

class  CEntityRedStone: public CEntity
{	
public:
	CEntityRedStone( EEntityType vEntityType ) : CEntity( vEntityType )  
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
	CEntityRedStone(  ): CEntity( ENTITYTYPE_REDSTONE ) {  } 
	
	virtual int Initialize()
	{
		mPos.mX	= 0;
		mPos.mY	= 0;
		mStatus = 0;
		mTeamID = 0;
		return 0;
	}

	virtual int Resume()
	{
		return 0;
	}

	virtual ~CEntityRedStone(){}
public:
	// 主人ID
	virtual unsigned int GetOwnerID( )  {  return mEntityID; };	
	
	// 设置主人ID
	void SetOwnerID( unsigned int vownerid ) {  }
	
	// 
	virtual int GetTeamFlag( ){ return 0; }
	
	//// 线ID
	//virtual unsigned short GetLineID( ) {  return mLineID; }
	//virtual void SetLineID( unsigned short usLineID ) {  mLineID = usLineID;  }
	//
	//// 地图ID
	//virtual unsigned short GetMapID( ){  return mMapID; }
	//virtual unsigned short GetMapIndex( ) { return mMapIndex; };
	//virtual void SetMapID( unsigned short usMapID ) {  mMapID = usMapID; }
	//virtual void SetIndex( unsigned short usMapIndex ) {  mMapIndex = usMapIndex;  }
	// 属性
	virtual CProperty* GetProperty( ) { return &mRedStoneProperty; }
	
	// 国家ID
	virtual unsigned short GetNationality( ) { return ~0; }

	// 是否敌对
	virtual bool IsHostileTo( CEntity* pEntity ){ return false; }

	// 是否友好
	virtual bool IsFriendlyTo( CEntity* pEntity ) { return false; }

	// 是否是队友
	virtual bool IsTeamTo( CEntity* pEntity ) { return false; }
	
	// 是否是同一家族成员
	virtual bool IsFamilyTo( CEntity* pEntity ){ return false; }
	
	// 是否是同一军团成员
	virtual bool IsCorpsTo( CEntity *pEntity ){ return false; }

	// 是否对玩家实体友好
	virtual bool IsHostileToPlayers( ) { return false; }

	// 队伍所属
	virtual ITimerReceiver* GetTimerReciver( ) { return &mTimerReceiver; }	
	virtual bool Setup( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator, CWTPoint* pPos = NULL, bool bInit = false, int vNpcID = 0 ); 

public:
	void SetPos( int x, int y )
	{  
		if( x == -1 || y == -1 )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ] Set Pos Error %d, %d, FUCK -1, FUCK Writer", __LK_FILE__, __LINE__,
				__FUNCTION__, x, y );
#ifdef _DEBUG_
			print_trace_fd( "default" );
#endif
		}
		mPos.mX = x; 
		mPos.mY = y; 
	}
	int	 GetX(){ return mPos.mX ; }
	int  GetY(){ return mPos.mY ; }
	void SetStatus( int Status ){ mStatus = Status; }
	int  GetStatus( ){ return mStatus; }
	void SetTeamID( int TeamID ){ mTeamID = TeamID;  }
	int  GetTeamID( ) { return mTeamID; } 		
	bool IfInRedStoneBound( int Posx,int Posy );
	void SetTimerID( int vTimerID ){ mTimerID = vTimerID; }
	int	 GetTimerID(  ){ return mTimerID; }
	bool CheckIfOwner( int vCharID );		
	
	// 设置&获取读条定时器的ID
	void SetProcessTimerID( int nTimerID ){ mProcessTimerID = nTimerID; }
	int	GetProcessTimerID(){ return mProcessTimerID; }
private:
	CNpcTimerReceiver mTimerReceiver;			// 内丹定时器
	int			   mStatus;					    // 使用状态
	unsigned int   mTeamID;						// 队伍ID	
	//unsigned int   mLineID;						// 线ID
	//unsigned int   mMapID;						// 地图ID
	//unsigned int   mMapIndex;					// 
	unsigned int   mOwnerID[TEAMNUM];			// 所属角色ID
	int			   mProcessTimerID;				// 读条定时器的ID
	CPropertyRedStone mRedStoneProperty;		// 内丹的属性
	int	  mTimerID;								// 定时器ID	

public:
	// 发送使用技能回执信息
	virtual void SendUseSkillErrorNotice( int nType ) { };
};

/* --------------------------------------------------------------------------*/
/* 
* @Synopsis ghost
*只在一个副本用  值吗。。。只在只能用在副本，因为没有处理定时销毁。yucx
*/ 
/* ----------------------------------------------------------------------------*/

class CEntityGhost: public CEntityNpc
{
private:
	CPropertyNPC	mProperty;

public:
	CEntityGhost( ) : CEntityNpc( ENTITYTYPE_GHOST )  
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

	virtual bool Setup( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos = NULL, bool bInit = false, int vNpcID = 0 );

	virtual int Initialize()
	{
		mPos.mX	= 0;
		mPos.mY	= 0;
		return 0;
	}

	virtual int Resume()
	{
		return 0;
	}

	inline virtual CProperty* GetProperty( ) { return &mProperty; }
private:
	unsigned int	mNpcID;

};