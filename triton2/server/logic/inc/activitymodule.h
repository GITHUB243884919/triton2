#pragma once 
#include "activitytype.h"
#include "proclient_svr.h"
#include "activity.h"
#include "creator.h"
#include "servercore.h"
#include "servertool.h"
#include "activitymessag_pb.hxx.pb.h"

#define GAME_LIST_SIZE 32

#define MAX_EXCEPTIP_NUM 50
#define MAX_MULTI_CLIENT_EXCEPT 1024

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "lua_tinker.h"
class CMessage;
class CEntityPlayer;
class CTimerItem;

struct NpcBidInfo 
{
	int			mNpcID;			// NPC的模板ID
	int			mState;			// 当前的状态
	CWTimer		mSubmitTimer;	// 报名倒计时
	CWTimer		mNotifyTimer;	// 通知倒计时
	CWTimer		mWarTimer;		// 战争倒计时
	int			mEntityID;
};

class TimeListSingle 
{
public:
	int m_nStartTime;
	int m_nEndTime;

	TimeListSingle()
	{
		if ( CObj::msCreateMode )		
		{
			m_nStartTime = 0;
			m_nEndTime   = 0;
		}
	}
};

typedef lk::vector<TimeListSingle, ACTIVITY_TIME_LST_SIZE> TIMELIST_TYPE;

class CActivityInfo
{
public:
	TIMELIST_TYPE  mTimeList; // 时间段<静态>
	int m_nLmtLevel;	// 等级限制<静态>
	int m_nMaxTimes;	// 每天最大次数<静态>
	int m_nCurState;	// 当前状态，见enActivityState枚举
	int m_OgreIndex[ACTIVITY_OGREINDEX_SZ];
	int m_OgreIndexNum;

	CActivityInfo()
	{
		if ( CObj::msCreateMode )		
		{
			m_nLmtLevel  = 0;
			m_nMaxTimes  = 0;
			m_nCurState  = 0; // 活动状态
			mTimeList.initailize();
			m_OgreIndexNum = 0;
			memset(m_OgreIndex, 0, sizeof(m_OgreIndex));
		}
	}
	void PushTime( int nType, int nTime )
	{
		if ( nType == ACTIVITY_START )
		{
			TimeListSingle obj;
			obj.m_nStartTime = nTime;
			mTimeList.push_back(obj);
		}
		else if ( nType == ACTIVITY_END )
		{
			// 一定是按顺序的，所以赋值给最后1个就行
			if ( mTimeList.size() <= 0 )
				return;
			mTimeList[mTimeList.size()-1].m_nEndTime = nTime;
		}
	}
	bool PushOgreIndex( int nOgreIndex )
	{
		if ( m_OgreIndexNum >= ACTIVITY_OGREINDEX_SZ )
		{
			return false;
		}
		m_OgreIndex[m_OgreIndexNum++] = nOgreIndex;
		return true;
	}
};

// 怪物掉落数据
class CActivityOgreData
{
public:
	int		m_nActivityID; // 活动ID
	int		m_nDropID; // 掉落表ID
	CActivityOgreData()
	{
		if (CObj::msCreateMode)
		{
			m_nActivityID = 0;
			m_nDropID = 0; 
		}
	}
};

typedef lk::hash_map<int, CActivityInfo, ACTIVITY_SIZE> ACTIVITY_MGR_TYPE;
typedef lk::hash_map<int, CActivityOgreData, ACTIVITY_DROP_SZ> ACTIVITY_OGRE_TYPE; // first是怪物ID或者原来的掉落表ID
typedef lk::vector<int, ACTIVITY_MAPDROP_SIZE> ACTIVITY_MAP_DROP_TYPE; 

class  CActivityModule: public CLogicModule, public CSingleton< CActivityModule >
{
private:
	ACTIVITY_MGR_TYPE	mSceneActivityMgr; // 活动管理器
	ACTIVITY_OGRE_TYPE  mAvtivityOgreMgr;	// 活动怪管理器
	ACTIVITY_MAP_DROP_TYPE mMapAddDropTable[MAX_MAP_ID]; // 活动地图掉落表(添加)
	int					   mMapRepDropTable[MAX_MAP_ID]; // 活动地图掉落表(替换)
	lua_State* L;
	int			mLastClearWeekCharmTime;
public:
	static char	sModuleName[ NAME_LENGTH ];	
public:
	// 初始化每日问答题库
	int InitialQuestionFile( const char *pFileName );
	
	// 初始化问卷调查的题库
	int InitailInvestQuestionFile( const char *pFileName );

	// 初始化双倍时间配置
	int InitialDoubleExpFile( const char *pFileName );
	
	// 初始化绣球路径配置
	int InitialHydrangea( const char *pFileName );
	
	// 初始化聚宝盆的配置信息
	int InitialTreasureBowlConfig();
public:	
	static unsigned int CountSize( );
public:	
	CActivityModule( ):CLogicModule( sModuleName )
	{ 
		mTimer = CWTimer( ACTIVITY_TIMER_LENGTH );
		mHydrangeaTimer = CWTimer(HYDRANGEA_TIMER_LENGTH);
		mInvestRecordTimer = CWTimer(INVESTRECORD_TIMER_LENGTH);
		mDoubleTime.initailize();
		/*
		memset( mDoubleTime, 0, sizeof(mDoubleTime) );
		memset( mExistTime, 0, sizeof(mExistTime) );
		mCount = 0;
		*/

		mDoubleActive = false;
		mDoubleIndex = -1;
		mEndTime = time(NULL);
		memset( mNpcInfo, 0, sizeof(mNpcInfo) );

		mSceneActivityMgr.initailize();
		mAvtivityOgreMgr.initailize();
		mNpcNum = 0;
		
		mTalentTableID = 0;
		memset( mRoleValidQuestions, 0, sizeof( mRoleValidQuestions ) );	
		memset( mRoleValidQuestionGroups, 0, sizeof( 0 ) ) ;			
		mRoleValideQuestionCount = 0;
		L=NULL;
		for (int i = 0 ; i < MAX_MAP_ID; ++i )
		{
			mMapAddDropTable[i].initailize();
		}
		memset(mMapRepDropTable, 0, sizeof (mMapRepDropTable));
		mTreasureBowlManager.Initialize();
		mLastClearWeekCharmTime=time(0);
	}
	virtual ~CActivityModule( )	{	}
public:
	void OnMessageIfBeginQuest( CEntityPlayer *tpPlayer, CMessage * pMessage );
	void OnMessageCheckAnswer( CEntityPlayer *tpPlayer, CMessage * pMessage );
	void OnMessageQuestionTimeEnd(CEntityPlayer *tpPlayer, CMessage * pMessage);
	void OnMessageIfAnswerNow( CEntityPlayer *tpPlayer, CMessage * pMessage );
	void OnMessageStopAnswer( CEntityPlayer *tpPlayer, CMessage * pMessage );
	void OnMsgSingleQuestTimeOut( CTimerItem* pTimerItem );
	void OnMsgTotalQuestTimeOut( CTimerItem* pTimerItem );
	void OnMessageUsePaper( CEntityPlayer *tpPlayer, CMessage * pMessage );
	
	void OnMessageInvestAnwerNotify( CEntityPlayer *tpPlayer, CMessage * pMessage );
	bool IsPassed( int nTime );

public:
	void PlayerBeginNpcQuest( CEntityPlayer *tpPlayer );
	void PlayerCheckAnswerRequest( CEntityPlayer *tpPlayer, int QuestionID, int Answer );
	void PlayerBeginPaperQuest( CEntityPlayer *tpPlayer, int Result );
	void StopCurrentQuestion( CEntityPlayer *tpPlayer, int  QuestionID  );
	void PlayerStopQuestionAnswer( CEntityPlayer *tpPlayer, int Entype );
	void PlayerUseQuestionPaper( CEntityPlayer *tpPlayer, int PaperIndex );	
	void EndAnswerQuestion( CEntityPlayer *tpPlayer, CAnswerStatus *tpAnswer, int EndType );
	void PlayerBeginQuestion( CEntityPlayer *tpPlayer, int Result ); 
																	  
	void OnEventInvestQuestion( int InvestType,  int ID, CEntityPlayer *pPlayer = NULL );																	  
	void SaveInvestAnswerToDB( CEntityPlayer *tpPlayer, int vLibedition, int LibID, int *pQuestionList, int *pAnswerList, int vQuestionNum );	
	void CheckInvestTime(  );
public:
	void SendMsgCheckAnswer( CEntityPlayer *tpPlayer, int Result, int QuestionID, int QuestionIndex ,int IfLastQuestion = 0 );
	void SendMsgNewQuestion( CEntityPlayer *tpPlayer,int QuestionID ,  int QuestionIndex );
	void SendMsgEndAnswerQuestion( CEntityPlayer *tpPlayer, int Result );
	void SendMsgUseQuestionPaper( CEntityPlayer *tpPlayer, int Result );
	
	void SendMsgInvestNotify( CEntityPlayer *tpPlayer, CInvestQuestionLib *pQuestionLib );		
public:			
	void CheckDoubleTime( );
	int GetDoubleState( int &vBeginTime, int &vEndTime );

	void AddNpcInfo( int vNpcID, int vEntityID = 0 );
	bool CanBidNpc( int vNpcID );

	void CheckNpcTime( );
	void SendMsgBidTimeOut( int vNpcID, int vFirst );
	void SendMsgBattleStart( int vNpcID );

	void DebugTimeOut( int vNpcID );
	void DebugBattleStart( int vNpcID );
	void DebugBattleEnd( int vNpcID );
	
public:
	int GetQuestNumOfPaper( int ItemID );
	void ClearActivitys();

	void OnGateMessageTodayActivityList( CMessage* pMessage );
	void OnGateMessageActivityChgNotice( CMessage* pMessage );
	void OnGateMessageActivityEventNotice( CMessage* pMessage );
	void Send2PlayerNowActivitys( CEntityPlayer* pPlayer );
	void SendHelpInfo2Client( CEntityPlayer* pPlayer );
	void FillGuideIncInfo( CEntityPlayer* pPlayer, PBTodayIncItem* pData );
	int  GetActivityStateByID( int nActivityID );

	// 得到今天/本周完成活动次数
	int  GetTodayCompleteTimes( CEntityPlayer* pPlayer,  int nActivityID );
	int  GetWeekCompleteTimes( CEntityPlayer* pPlayer,  int nActivityID );

	// 得到活动怪物的掉落表ID
	int  GetActivityOgreDropID( int nOgreTempID );
	void OnMessageGetActivityInfo( CEntityPlayer* pPlayer, CMessage* pMessage );
	int  GetReplaceDropID(CEntity* pEntity);
	void GetAddDropID(CEntity* pEntity, std::vector<int>& vAddIDs);
	
	// 把副本ID转成对应的活动ID
	int ChangeRepetionIDToActivityID( int nRepetionID );

public:

	// 启动服务器
	virtual void OnLaunchServer( );

	// 退出服务器
	virtual void OnExitServer( );

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffsekt );

	// 建立场景	   
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );


	// 路由消息
	virtual void OnRouterMessage( CMessage* pMessage );

	// 客户端发送消息
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity , CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );
	
	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );
	
	// 加载配置
	virtual void OnLoadConfig( );
	
	// 超时消息
	virtual void OnTimeoutMessage( CMessage* pMessage );

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }

	// 网关消息
	void OnGateMessage( CMessage *pMsg );

	// 增加双倍时间段
	bool AppendDoubleTime(time_t nStartTime, time_t nEndTime, int nRate );		// 有时间段冲突校验
	bool AppendDoubleTime(const char* tpDate, int nExistTime, int nRate = SERVER_PERCENT_INT );	// 没有时间段冲突校验

	// 删除双倍时间段
	bool RemoveDoubleTime(time_t nStartTime, time_t nEndTime );
	
	// 发送散喜糖的通知
	void SendThrowSugarNotify( CEntity *pEntity );	
	bool RegisterLua();	
	
	// 新一天到来
	virtual void OnNewDayCome( );
public:			

	// 发送抛绣球的通知
	void SendThrowHydrangeaNotify(  int nThrowType,  CEntityPlayer *pDesPlayer, CEntity *pNpc, CEntityPlayer *pSrcPlayer );
	
	// 可以抢绣球的通知
	void SendGrabHydrangeaEnableNotify( CEntityPlayer *pPlayer, CEntity *pEntity );	 	

	// 抢绣球的回应消息
	void SendGrabHydrangeaResponse( CEntityPlayer *pPlayer, int nErrcode );
	
	// 抢绣球的结果通知
	void SendGrabHydrangeaResultNotify(  CEntityPlayer *pDesPlayer, int nSrcPosX, int nSrcPosY, const char *pSrcName = NULL );
	
	// 抢绣球活动的结果通知
	void SendHydrangeaResultNotify( CEntityPlayer *pPlayer );	

	// 抢绣球活动开始的通知
	void SendStartHydrangeaNotify( int nMapID, int nPosX, int nPosY );
	
	// 扔绣球的回应消息	
	void SendThrowHydrangeaResponse( CEntityPlayer *pPlayer, int nErrcode );	
	
	// 抢绣球的请求消息
	void OnMessageGrabeHydrangeaRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 扔绣球的请求
	void OnMessageThrowHydrangeaRequest( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 开始绣球活动
	void StartHydrangeaActivity( int nActivityID );
	
	// 结束绣球活动
	void EndHydrangeaActivity();

	// 清除绣球活动的信息
	void ClearHydrangeaInfo();
	
	// 检查npc的移动路线
	void CheckNpcPath();
	
	// 检查绣球招亲是否开始
	void CheckHydrageaActivityTime();
	
	// 绣球招亲计时器
	void OnTimeOutHydrangeaActivity( CTimerItem *pTimer );
	
	// 抛绣球的计时器
	void OnTimeOutHydrangeaThrow( CTimerItem *pTimer );
	
	// 撒喜糖的计时器
	void OnTimeOutHydrangeaSugar( CTimerItem *pTimer );
	
	// 占有绣球的计时器
	void OnTimeOutHydrangeaHold( CTimerItem *pTimer );
	
	// 抢夺绣球的计时器
	void OnTimeOutHydrangeaGrab( CTimerItem *pTimer );
	
	// 系统发放绣球
	void ThrowHydrangeaBySys();
	
	// 抢绣球
	void PlayerGrabHydrangea( CEntityPlayer *pOriginalPlayer, CEntityPlayer *pNewPlayer );
	
	// 抛绣球
	void PlayerThrowHydrangea( CEntityPlayer *pPlayer );
	
	// 发送设置玩家绣球状态的通知
	void SendSetHydrangeaStatusNotify( CEntityPlayer *pPlayer, int nStatus );
	
	// 清除玩家的绣球状态
	void ClearHydrangeaStatus( CEntityPlayer *pPlayer );

	// 活动礼包（乾坤袋）处理
	int  OnUseLuckyBag( CEntityPlayer* pPlayer, CTplFuncItem* pItem, int vIndex );
	
	// 初始化活动配置表
	int InitialOgreActivityConfig( const char *pFileName );
	
	// 刷新怪物时检查怪物的活动信息
	int CheckOgreActivityInfoOnCreate( int nTempID, int nEntityID = 0 );
	
	// 怪物死亡时检查怪物的活动信息
	int CheckOgreActivityInfoOnDeath( int nTempID, int nkillerID = 0 );
	
	// 结束怪物活动
	int EndOgreActivity( CTimerItem *pTimer );
	
	// 获取活动怪的信息
	COgreInfo * GetActivityOgreInfo( int nTempID );
	
	// 触发活动事件 (外部调用)
	int  FireActivityEvent( int nActivityID, int nEventID, const std::vector<int>& rIntParams, std::vector<std::string>& rStrParams  );
	
	// 创建答题验证的消息
	void BuildQuestionVerifyMsg( CMessageQuestionVerifyNotify &rMsg, SRoleValidQuestionOption* pResults );

	// 建立问题答案
	void BuildQuestionOptionData(SRoleValidQuestionOption* pInputOption, SRoleValidQuestionOption* pOutputOption);
	
	// 通知答题验证
	void NotifyQuestionVerify( CEntityPlayer *pPlayer,  int nVerifyKind, int nVerifyType, int nVerifyTimeLength = 0 );
	
	// 客户端答题验证的结果通知
	void OnMessageAnswerVerifyRequest( CEntityPlayer *pPlayer, CMessage *pMsg );
	void OnMessagePlayerGetGiftRequest( CEntityPlayer *pPlayer, CMessage *pMsg );

	// 答题验证的结果
	void NotifyQuestionVerifyResult( CEntityPlayer *pPlayer, int nVerifyKind, bool bResult, int nReason = VERIFYREASON_ANSWER );
	
	// 发送答题验证的结果
	void SendResponseAnswerVerify( CEntityPlayer *pPlayer, int nResult );
	
	// 答题验证超时
	void OnTimeOutAnswerVerify( CTimerItem *pTimer );
	
	// 读取读片验证的配置
	void ReadRoleValidQuestionConfig();
	
	// 初始化读片验证信息
	int InitializeRoleValidQuestionInfo();
		
	int ChangeActivityState( CEntityPlayer* pPlayer, int nActivityID, int nNewState, int nStateParam1, int nStateParam2, bool bIsNotify, bool bIsSetTime );
	int OnPlayerLevelUp(CEntityPlayer* pPlayer);
	
	// 查询物品聚宝信息
	void OnMessageGetTreasureBowl( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 发送查询物品的回应消息
	void  SendResponseGetTreasureBowl( CEntityPlayer *pPlayer, int nErrcode, int nItemIndex, int nItemUsedTimes = 0, int nItemTatolTimes = 0, int nAllUsedTimes = 0, int nTotalTimes = 0, int nTaxMoney = 0 );
	
	// 开始物品聚宝
	void OnMessageStartTreasureBowl( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 发送开始物品聚宝的回应消息
	void SendResponseStartTreasureBowl( CEntityPlayer *pPlayer, int nErrcode, int nItemIndex = 0, int nNewItemID = 0 );
	
	// 发送聚宝成功的阵营提示
	void SendTreasureBowlSuccessNotify( CEntityPlayer *pPlayer, int nOldItemID, int nNewItemID, int nItemNum, int nTipsType );	
	void OnlineGift4CreateEntity(CEntityPlayer* pPlayer, int nActivityID, bool vFirst );

private:   
	CQuestionManager mQuestionMng;
	CInvestQuestionManage mInvestQuestionMng;
	CWTimer			 mTimer;
	CWTimer			 mHydrangeaTimer;
	CWTimer			 mInvestRecordTimer;
	
	// 角色验证问题
	SRoleValidQuestion mRoleValidQuestions[ROLE_VALID_QUESTION_COUNT];	
	// 角色验证选项
	SRoleValidQuestionGroup mRoleValidQuestionGroups[ROLE_VALID_QUESTION_GROUP_COUNT];	
	// 角色数量
	int mRoleValideQuestionCount;
	// 范围
	int mRoleValidQuestionRange;
	// 副本验证答题奖励的修为表ID
	int mTalentTableID;	
	/*
	RefreshDate mDoubleTime[ 30 ];
	int			mExistTime[ 30 ];
	int			mCount;
	*/

	class CRefresh
	{
	public:
		RefreshDate first;		// 起始时间点, 为了保持与之前兼容, 字段名称为更改
		int			second;		// 持续时间, 为了保持与之前兼容, 字段名称为更改
		int 		rate;		// 新增倍率, 不允许为零，且是10000为单位1

		CRefresh() {}
		CRefresh(RefreshDate& from, int nSeconds, int nRate)
		{
			first = from;
			second = nSeconds;
			rate = nRate;
		}
	};


	typedef lk::vector< CRefresh, 30> CDoubleTime;
	CDoubleTime mDoubleTime;

	bool		mDoubleActive;
	CWTimer		mDoubleTimer;
	unsigned int mEndTime; // 结束时间(绝对时间 time_t )
	int			mDoubleIndex;

	NpcBidInfo  mNpcInfo[ 1000 ];
	int			mNpcNum;
	CHydrangeaNpcManager mHydrangeaNpcManager;
	CHydrangeaActivityManager mHydrangeaActivityManager;
	COgreActivityManager mOgreActivityManager;
	CTreasureBowlManager mTreasureBowlManager;
public:

	// 输出统计结果
	int WriteInvestResultToFile();	
	
	// 获取昨天的统计结果
	int GetInvestResult();

// 供LUA调用的接口
public:
	// 得到玩家等级
	static int GetPlayerLevel( int nEntityID );

	// 得到玩家门派
	static int GetPlayerMetier( int nEntityID );

	// 得到玩家包裹空格数
	static int GetPlayerBagSpaceNum( int nEntityID );

	// 给玩家奖励物品 (支持加星装备)
	static int InsertItem( lua_tinker::table param );

	// 发错误码
	static int SendErrNotice( int nEntityID, int nErrID );

	// 得到玩家性别
	static int GetPlayerGender( int nEntityID );

	static void Log_Error( const char* pStr );

	// 发系统邮件
	static bool SendSysMail( lua_tinker::table param );

	// 获得玩家阵营
	static int  GetCampID( int nEntityID );

	////////////// 活动操作类 ///////////////////////////////////////////////////////////////////////////////
	
	// 获得活动状态
	static int  GetActivityState( int nActivityID );

	// 修改全部玩家活动状态 
	// 参数表：
	static int  ChangeAllActivityState( lua_tinker::table param );

	// 修改玩家活动状态
	static int  ChangeSingleActivityState( lua_tinker::table param );

	// 给玩家上BUFF
	static int  EntityInsertBuff( int nEntityID, int nBuff, int nBuffTime );

	// 传送实体
	static int  EntityTeleport( lua_tinker::table param );

	// 检查堆叠 
	static bool  CheckPileLimit( int nEntityID, int nTempID, int nNum );

	// 通知送好礼结果
	static void NotifyGetGiftResult( int nEntityID , int nRet, int nGiftType );

 //配置加载的回调函数
public:
	static void LoadQuestionFileCallback(const char* pcConfigPath);
	static void LoadLuaFileCallback(const char* pcConfigPath);
	static void LoadInvestQuestionFileCallback(const char* pcConfigPath);
	static void LoadDoubleExpFileCallback(const char* pcConfigPath);
	static void LoadHydrangeaCallback(const char* pcConfigPath);
	static void LoadOgreActivityConfigCallback(const char* pcConfigPath);
	static void LoadTreasureBowlConfigCallback(const char* pcConfigPath);
	static void LoadActivityYqsConfigCallback(const char* pcConfigPath);
	static void LoadRoleValidQuestionConfigCallback(const char* pcConfigPath);

};

