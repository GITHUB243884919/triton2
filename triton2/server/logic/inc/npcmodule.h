#pragma once

#include "sceneobj_define.h"
#include "servercore.h"
#include "lk_vector.h"
#include "lk_hashmap.h"

class CNpcPolicy;

// 策略最大数
#define TACTICSNUM	140
// 定时器最大数
#define NPCTIMERNUM 100
// 仇恨列表上限
#define NPCENMITY	50
// 巡逻列表上限
#define PATROLNUM	20

class CObj;
class CEntity;
class CEntityNpc;
class CEntityOgre;
class CEntityPet;
class CEntityCharacter;
class TiXmlElement;


// ********************************************************************** //
// CNpcPolicy
// ********************************************************************** //

class CNpcEnmity
{
public:
	int		mEnmity;		// 仇恨值
	int		mEntityID;		// 对指定实体的仇恨
	//CNpcEnmity *mpPrev;
	//CNpcEnmity *mpNext;
	// 把指针改成数组下标，-1表示无效
	int		mpPrev;
	int		mpNext;
public:
	//CNpcEnmity( int vEnmity = 0, int vEntityID = 0 ) : mEnmity( vEnmity ), mEntityID( vEntityID )
	//{
	//}

	CNpcEnmity( )
	{
		if ( CObj::msCreateMode )
		{
			mEnmity = -1;
			mEntityID = -1;
			//mpPrev = NULL;
			//mpNext = NULL;
			mpPrev = -1;
			mpNext = -1;
		} 
		else
		{
			;
		}
	}
};


// ********************************************************************** //
// CNpcTimer
// ********************************************************************** //

class CNpcTimer
{
public:
	int mID;
	int mInterval;
	int mTimer;
	int mNum;
	bool mEnabled;
};

// ********************************************************************** //
// CNpcSkill
// ********************************************************************** //

class CNpcSkill
{
public:
	int mSkillID;
	int mSkillLevel;
	int mSkillTarID;
public:
	CNpcSkill( ) : mSkillID( 0 ), mSkillLevel( 0 ), mSkillTarID( 0 )
	{

	}
	CNpcSkill( int vSkillID, int vSkillLevel, int vSkillTarID ) : mSkillID( vSkillID ), mSkillLevel( vSkillLevel ), mSkillTarID( vSkillTarID )
	{

	}
};

// ********************************************************************** //
// CNpcPolicy
// ********************************************************************** //

// 基础NPC策略 
class CNpcPolicy : public CObj
{
public:
	enum
	{
		STATE_PEACE,			// 和平状态
		STATE_BATTLE,			// 战斗状态
	};

	enum
	{
		STEPTIMER		= 300,
	};

	enum
	{
		CLOSE,					// 近程
		DISTANT,				// 远程
	};
public:
	typedef lk::vector< CWTPoint, 4 >	PathList;
	typedef lk::vector< CWTPoint, 4 >	PreList;
	typedef lk::vector< CNpcSkill, 5 >	SkillList;

	typedef lk::hash_map< int, int, NPCENMITY > 	ENMITY;

	int					mNpcID;
	int					mNpcType;
	int					mRunSpeed;			// 移动的速度，从模板读
	int					mMoveTimer;			// 移动计时器
	int					mStepTimer;			// 走步计时器
	int					mStandTimer;		// 罚站计时器
	int					mSneerTimer;		// 嘲讽计时器
	int					mTempTimer;			// 临时计时器
	int					mAttackTimer;		// 攻击计时器
	int					mAttackMode;		// 攻击方式
	CWTPoint			mTargetPos;			// 预计目标点
	CWTPoint			mPathPos;			// 当前目标点
	CWTPoint			mPrevPos;			// 之前目标点
	PathList			mPathList;			// NPC计划行走路点
	PreList				mPreList;			// NPC计划行走路点
	bool				mIsReturn;			// 是否处于返回状态
	int					mNpcState;			// NPCAI状态
	int					mTargetEntityID;	// 目标实体
	int					mSneerID;			// 嘲讽的实体
	int					mMaxTempID;			// 临时攻击实体
	int					mMinTempID;
	int 				mLastAlive;			// 上次生存状态
	int					mLastState;			// 上次NPC策略状态
	int					mLastTick;
	CEntityNpc*			mpEntity;
	int					mEntityID;			//TODO
	bool				mPath;
	bool				mIsAttacking;
	
	bool				mBattleStart;
	bool				mBattleEnd;
	bool				mIsDead;			// 是否死亡
	bool				mKill;				// 是否杀死玩家
	short				mCanUseSkill;		// 是否可以使用技能
	SkillList			mSkillList;			// 施放技能的列表
	CNpcTimer			mTimer[ NPCTIMERNUM ];
	int					mTimerNum;
	bool				mDone[ 45 ][ 5 ];	// 判断对应的策略条件是否已经执行
	int					mTeleTimer;			// 如果指定时间内无法走回，传送回出生点

	bool				mSleep;				// 是否在休眠
	bool				mNoSleep;			// npc在任何情况下都不休眠 

	ENMITY				mEnmityData;		// 新仇恨数据
	int					mFollowID;			// 跟随的实体ID
	int					mOffsetX;			// 跟随的偏移X
	int					mOffsetY;			// 跟随的偏移Y
	CWTPoint			mPatrolList[ PATROLNUM ];		// 巡逻路线列表
	int					mPatrolIndex;		// 巡逻路线位置
	int					mPatrolDirection;	// 巡逻方向



public:

	CNpcPolicy( );
	virtual ~CNpcPolicy( );

	virtual int Initialize();
	virtual int Resume();

public:
	// 得到休眠状态
	inline bool IsSleeping( ) { return mSleep; }

	// 添加仇恨
	void InsertEnmity( int vEntityID, int vEnmity = -1 );

	// 得到最恨的目标
	int GetEnmityEntityID( );

	// 全部清空仇恨为指定数值
	void ClearEnmity( int vEnmity );

	// 清空指定实体的仇恨
	void ClearEnmity( int vEntityID, int vEnmity );

	// 删除指定实体的一部分仇恨
	void RemoveEnmity( int vEntityID, int vEnmity );

	// 移动到指定位置
	void MoveTo( const CWTPoint& rPoint );

	// 返回出生点
	void Return2Orgin( bool vProtected, bool vReturn );

	// 开启罚站
	void Stand( int vTimer );

	// 重置目标
	void ResetTarget( );

	// 普通攻击
	void NormalAttack( CEntityCharacter* pTarget );

	// 技能攻击
	void UseSkill( int vSkillID, CEntity* pTarget );

	// 进行攻击
	void DoAttack( );

	// 召唤兽思考
	void OnPet( unsigned int vTickCount );

	// 任务NPC思考
	void OnTask( unsigned int vTickCount );

	// 得到攻击坐标
	bool GetAttackPosition( const CWTPoint& rSrcPos, const CWTPoint& rDesPos, int vRange, int vType );

	// 得到随机坐标
	CWTPoint GetRandPos( );	

	// 处理高级策略
	void OnTactics( int vTacticsID );

	// 获得目标
	void GetTarget( int vCode, std::vector< int > &vTargetList );

	// 和平状态逻辑
	void OnPeace( unsigned int vTickCount );

	// 战斗状态逻辑
	void OnBattle( unsigned int vTickCount );

	// 主动怪寻找目标
	void SearchEnemy( );

	// 处理策略定时
	void OnTacticTimer( unsigned int vTickOffSet );

	// 处理返回逻辑
	void OnBackLogic( );

	// 处理死亡逻辑
	void OnDieLogic( );

	// 处理空闲逻辑
	void OnIdle( );

	// 处理走动逻辑
	void OnStep( );

	void FindPath( const CWTPoint& rSrcPos, const CWTPoint& rDesPos );
	void PathNpc( );
	void StepNpc( );

	CWTPoint GetMovePos( );
};


// ********************************************************************** //
// CNpcCondition
// ********************************************************************** //

class CNpcCondition
{
public:
	int mTypeID;
	int mParam1;
	int mParam2;
	int mParam3;
	int mParam4;
};

// ********************************************************************** //
// CNpcAction
// ********************************************************************** //

class CNpcAction
{
public:
	int	mTypeID;		// 操作的类型
	int mParam1;		// 操作的参数，由具体类型决定意义
	int mParam2;
	int mParam3;
	char mSpeak[ 100 ];	// 所说的话
	int mTarget;		// 操作的对象
};

// ********************************************************************** //
// CNpcTrigger
// ********************************************************************** //

class CNpcTrigger
{
public:
	std::vector< CNpcCondition > mCondition;
	std::vector< CNpcAction > mAction;
	bool mActive;
	bool mInbattle;

public:
	void Initialize( )
	{
		mCondition.clear();
		mAction.clear();
		mActive = 0;
		mInbattle = 0;
	}
};

// ********************************************************************** //
// CNpcTactics
// ********************************************************************** //

class CNpcTactics
{
public:
	char	mTacticsName[ 100 ];
	std::vector< CNpcTrigger > mTrigger;

public:
	void Initialize( )
	{
		memset( mTacticsName, 0, sizeof(mTacticsName) );
		
		for( unsigned int i = 0; i < mTrigger.size(); i++ )
			mTrigger[i].Initialize();

		mTrigger.clear();
	}
};

// ********************************************************************** //
// CNpcModule
// ********************************************************************** //

class CNpcModule : public CLogicModule, public CSingleton< CNpcModule >
{
public:
	static char	sModuleName[ NAME_LENGTH ];
	CWTimer		mTimer;
	unsigned int mLastTickTimer; // npc模块的最后一次tick时间
public:
	CNpcModule( ); 

	static unsigned int CountSize();

public:
	typedef lk::hash_map< int, int, SERVER_CAP_NPCPOLICY > NpcPolicys;

	NpcPolicys	mNpcPolicys;


public:
	static CNpcTactics mTactics[ TACTICSNUM ];

public:


	// NPC被攻击
	void OnAttacked( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, unsigned int vEnmityValue );
	// NPC被嘲讽
	void OnSneered( unsigned int vSrcEntityID, unsigned int vDesEntityID, unsigned int vSneerValue );
	// 人或自身死亡
	void OnDead( unsigned int vDesEntityID, unsigned short vDead );
	// 外部设置任务NPC 功能NPC移动
	void OnMoveNpc( unsigned int vEntityID, unsigned int vPosX, unsigned int vPosY );
	// 设置传送后目标点
	void OnTeleport( unsigned int vEntityID, unsigned int vPosX, unsigned int vPosY );

	// 设置NPC是否休眠(详见cpp)
	void OnSleep( CEntityNpc* pEntity, bool vFlag, int nAutoSleep = 0 );

	// 设置跟随目标
	void OnFollow( CEntityNpc* pEntity, unsigned int vFollowID, int vOffsetX, int vOffsetY );

	// 获得位置坐标
	CWTPoint GetTargetPos( unsigned int vEntityID );

	// 停止任务NPC
	void StopTaskNpc( unsigned int vEntityID );

	// 设置巡逻坐标
	void SetPatrol( CEntityNpc* pEntity, int *vPosX, int *vPosY, int vLen, int vStart );


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

	// 客户端发送消息
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );
	
	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode )
	{
		return true;
	}

	// 加载配置
	virtual void OnLoadConfig( );

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }

	// 加载策略配置的回调函数
	static void LoadStrategyConfigCallback(const char* pcConfigPath);
};
