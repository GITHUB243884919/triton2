#pragma once

#include "servercore.h"
#include "protype.h"
#include "proclient_svr.h"
#include "coretype.h"
#include "tasktype.h"
#include "taskmanager.h"
#include "propertymodule.h"
#include "mapmodule.h"
#include "repetionmodule.h"
#include "scenelogic.h"
#include "familymodule.h"
#include "corpsmodule.h"
#include "familytype.h"
#include "mailmodule.h"
#include "logmodule.h"
#include "worldmodule.h"
#include "activitymodule.h"
#include "worldmodule.h"
#include "teammodule.h"
#define IsTaskAbortAble(vTaskID)	lua_tinker::call<int,  unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_CANCELABLE)
#define IsTaskRepeateAble(vTaskID)	lua_tinker::call<bool, unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_REPEATEABLE)
#define IsTaskRetryAble(vTaskID)	lua_tinker::call<bool, unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_RETRYABLE)
#define IsTaskFailOnKilled(vTaskID)	lua_tinker::call<bool, unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_FAILONKILLED)
#define IsTaskDirectFinish(vTaskID)	lua_tinker::call<bool, unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_DIRECTCOMPLETE)
#define GetSendNpcID(vTaskID)	    lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_SENDNPC)
#define GetMaxGird(vTaskID)	        lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_MAXGIRD)
#define IsOffFail(vTaskID)	        lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_OFF_FAIL)
#define GetTaskGroupID(vTaskID)	    lua_tinker::call<int , unsigned short, unsigned short>(CTaskModule::GetSingleton().L, "get_ctrl", vTaskID, CTRL_TASKGROUP)
#define IsAutoAbort(vTaskID)	    lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_ISAUTOABORT)
#define IsCompleteShare(vTaskID)	lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_ISSHARE)
#define GetTimeOutFlag(vTaskID)		lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_TIMEOUT_TYPE)
#define GetTimeLmtType(vTaskID)		lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_TIMELMT_TYPE)
#define IsDailyFail(vTaskID)		lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_IS_DAILY_FAILED)
#define IsTeamLoop(vTaskID)			lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_IS_TEAMLOOP)
#define GetTeamMember(vTaskID)		lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_TEAM_MEMBER)
#define GetFinishDelayTime(vTaskID)	lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_FINISH_TIME)
#define IsRefreshAward(vTaskID)		lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_REFRESH_AWARD)
#define GetTaskReaccept(vTaskID)	lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_TASK_ENDTIME)
#define GetTaskPublishType(vTaskID)	lua_tinker::call<int , unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_TASK_PUBLISHTYPE)

// 每天最多的组队环任务
#define MAX_TEAMLOOP_EVERYDAY  lua_tinker::call<int>(L, "get_team_loop_num")


class CTaskModule : public CLogicModule, public CSingleton< CTaskModule >
{
public:
	static char	sModuleName[ NAME_LENGTH ];

	typedef std::vector< std::pair<int, int> > CItemsPair;

	lua_State* L;
	CSendNpcManager			  mSendNpcMgr;  // 护送任务管理器
	CWTimer					  mLuaStateChkTimer;		// lua状态检查时间
	CTaskNpcMonitor			  mNpcMonitor;	// NPC监视器【不存盘】
	bool					  m_bHasReloadLua;  // 是否已加载Lua标志，保证Lua文件只会被加载一次。用于加载指定配置功能

private:

	unsigned short			  mLevelTasks[3][ MAX_GRADE + 1 ][1000]; // 每种等级对应的推荐任务ID列表 分别是主线、支线、西游
	unsigned short			  mLevelTasksN[3][ MAX_GRADE + 1 ];

	CTaskInviteManager		  mTaskInviteInfMgr; // 任务邀请管理器
	CTaskDropItemMgr		  mTaskDropMgr; // 掉落管理器

	int						  mRefreshStorageTaskTimer; // 刷新库任务计时器(毫秒)

public:
	CTaskModule( ): CLogicModule( sModuleName ), L(NULL)	
	{
		// 这些数据在重新加载的时候会重新初始化 不用被共享内存管理
		L = NULL; // 会被重新赋值 在注册LUA里面
		mRefreshStorageTaskTimer = 0;
		memset( mLevelTasks, 0, sizeof(mLevelTasks) );
		memset( mLevelTasksN, 0, sizeof(mLevelTasksN) );
		mLuaStateChkTimer = 5000;
		m_bHasReloadLua = false;
	}

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

	// 客户端发送消息
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 网关发送消息
	virtual void OnGateMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );

	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer , int vSaveCode );

	// 加载配置
	virtual void OnLoadConfig( );

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( );

	// 新一天到来
	virtual void OnNewDayCome( );

	// 新一周到来
	virtual void OnNewWeekCome( );

	void ClearTasksByType( CEntityPlayer* pPlayer, enTaskType nType );
	void ClearTasks( CEntityPlayer* pPlayer, unsigned short *aTaskID, int n );

public:

	// 玩家放弃任务
	void PlayerAbortTask( CEntityPlayer* pPlayer, unsigned short vTaskID, bool bIsDebug = false, bool bIsSystemCall=false );

	void ClearTragedy(CEntityPlayer* pPlayer, int nIndex );

	// 玩家获得任务 邀请者是-1说明不是邀请的任务
	bool PlayerObtainTask( CEntityPlayer* pPlayer, unsigned short vTaskID, bool bIsNoChkNpc = false, int nNpcEntityID = 0, int nInvitingPlayerID = -1 );

	// 玩家刷新任务奖励
	bool PlayerRefreshTaskAward( CEntityPlayer* pPlayer, unsigned short vTaskID,int vBagIndex, int nNpcEntityID = 0 );

	// 当玩家的任务状态发生变化时的通知流程
	void UpdateTaskWrapper(CEntityPlayer* pPlayer, int unsigned nEventID, int unsigned nParam, int nSend );

	//计算由于获得物品，引起的任务状态变化
	void OnTaskObtainItem( CEntityPlayer* pPlayer, unsigned int vItemID, unsigned short vNumber );

	//处理由于道具失去造成的任务状态变化
	void OnTaskRemoveItem (CEntityPlayer* pPlayer, unsigned int vItemID, unsigned short vNumber );

	// 计算由于实体死亡引起的任务状态和进度变化
	void OnEntityDie( CEntity* pOwnerEntity, CEntity* pDesEntity );

	// 当单击NPC时
	void OnPlayerClickNpc( CEntityPlayer* pPlayer, int vEntityID);

	// 发生库任务列表
	void SendStorageTaskList2Client( CEntityPlayer* pPlayer, CTaskList* pList, int vEntityID );

	// 当玩家金钱改变时
	void OnPlayerChangeMoney( CEntityPlayer* pPlayer );


	// 向客户端发送NPC头顶TIPS
	// void SendNpcTaskTip( CEntityPlayer* pDestPlayer, CPropertyPlayer* pPlayerProperty, int nNpcTempID, int nNpcEntityID );

	// ***** 以下是任务模块自己调用的接口

	// 初始化时注册任务推荐等级与任务ID
	void RegLevelTask( unsigned short usLevel, unsigned short usTaskID );	
	void RegTaskEventTick( CTask* pTask );
	void RegDropTable( unsigned short usTaskID, int nOgreID, int nItemID, int n, int nP, int nMaxn );


	// 触发事件
	void FireEvent( CEntityPlayer* pPlayer, unsigned int nEventID, unsigned int nParam = 0 );

	// 玩家使用任务道具  返回错误码
	int  UseTaskItem( CEntityPlayer* pEntity, unsigned short vIndex, int &tItemID );

	// 任务失败了 (bForce:是否强制失败 如果是否 那么只有非TASKBEGIN的时候才走失败逻辑)
	// bRemoveNpcMonitor：是否立刻删除NPC监控器（不能在循环里面删除）
	void OnTaskFailed( CEntityPlayer* pPlayer, unsigned short usTaskID, bool bForce = true, bool bRemoveNpcMonitor=true );
	bool OnRefreshStorageTask( CEntityPlayer* pPlayer );

	// 供外部调用的获得NPC TIP接口
	unsigned short GetNpcTip( CEntityPlayer* pPlayer, int nNpcTempID, int nNpcEntityID );

	// 库任务调用的接口
	int RefreshStorageTask( unsigned short usTaskID, ETaskRefreshType etype, unsigned short* pOutArr );

	// 外部消息响应接口
	void OnMessageUpgEquipRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageProduceEquipRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageInsertRuneRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageInsertJewelRequest( CEntityPlayer* pPlayer, CMessage* pMessage);
	void PlayerOpenSlot( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageEatEquipRequest( CEntityPlayer* pPlayer, CMessage* pMessage); 
	void OnMessageProducePhysRequest( CEntityPlayer* pPlayer, CMessage* pMessage);
	void ClearCompletedTask( CEntityPlayer* pPlayer, unsigned short usTaskID );
	void OnMessagePlayerUseScroll( CEntityPlayer* pPlayer, CMessageObtainScrollTaskRequest* pMsg );
	void OnMessageRefreshNpcTip( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessagePlayerEndTalk( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageStudySkillRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessagePlayerLockBaggage( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessagePlayerTakeoffItem( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessagePlayerGeFamilyTask( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessagePlayerGeCorpsTask( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 给犀哥加的接口 判断某任务是否在玩家身上
	bool IsTaskOnPlayer( CEntityPlayer* pPlayer, unsigned short usTaskID );

	// 给辉哥提供的获得任务名字的接口
	const char* GetTaskName( unsigned short vTaskID );

	// 采集接口
	bool OnCollectItem( CEntityPlayer* pPlayer, CTemplateCollect* pNpcTpl );

	int GetTaskType( unsigned short vTaskID );

	// 直接完成任务接口 , bool bIsAuto:是不是自动完成进来的
	bool DisposeEndTask( CEntityPlayer* pPlayer, unsigned short vTaskID, bool bIsTeamMember = false, bool bIsAuto = false, int vSelect = -100, int nIndex = -1, int nClearNum = 0 );

	// 玩家出生挂任务接口
	void SetDefaultTask( CEntityPlayer* pPlayer, const char* pTaskTypeName );

	// 干掉任务接口
	void ClearTasks( CEntityPlayer* pPlayer );

//private:

	// 玩家结束任务
	bool PlayerEndTask( CEntityPlayer* pPlayer, unsigned short vTaskID,  short vSelectID, bool bIsAuto = false, int vBagIndex = -1 );

	// 玩家点击任务选项
	//void PlayerSelectOption( CEntityPlayer* pPlayer, CProperty* pProperty, unsigned short vTaskID, unsigned short vDialogID );

	// 删除玩家因任务vTaskID收集的物品
	void DropCompleteItems(CEntityPlayer* pPlayer, unsigned short vTaskID, bool aborted = false, int nIndex = -1, int nClearNum = 0);

	// 删除物品
	bool DropItems(CEntityPlayer* pPlayer, const CItemsPair& vItemsPair, int nTaskID );

	// 得到NPC头顶
	unsigned short GetNpcTip( CEntityPlayer* pPlayer, int nNpcID, CTaskList* pTaskList, int nNpcEntityID);

	// EntityDie调用  掉落任务物品
	bool OnTaskDropItems( CEntityPlayer* pPlayer, CProperty* pDesProperty, CItemsPair& rSpecial, std::map<unsigned short, unsigned short>& rUpList, CMessageUpdateTaskConditions* pUpdateMsg );
	bool OnTaskOgreDie( CEntity* pOwnerEntity, CEntityPlayer* pPlayer, CProperty* pDesProperty, CItemsPair& rSpecialTaskItem, std::map<unsigned short, unsigned short>& rUpList , CMessageUpdateTaskConditions* pUpdateMsg  );
	void NotifyClientUpTask( CEntityPlayer* pPlayer, CTask* pTask, int nPreState, unsigned int nEventID = 0, unsigned int nParam = 0 );
	void SendUpdateConditions2Client( CEntityPlayer* pPlayer, CMessageUpdateTaskConditions* pMsg );

	void SendTragedyInfo2Client( CEntityPlayer* pPlayer, int *anIndex, unsigned short *asNextID, int *anState, int n, int nIsFirst = 0 );

	// 与lua任务文件之间的接口
	// 判断任务是否可以领取
	bool IsTaskAcceptable(CEntityPlayer* pPlayer, CTaskList* pTaskList, unsigned short vTaskID, int vNpcTempID =0 );
	bool IsTaskAcceptableExpress( CEntityPlayer* pPlayer, CTaskList* pTaskList, unsigned short vTaskID );

	// 取得任务NPC发放的物品
	bool GetTaskExtendItems(CEntityPlayer* pPlayer, unsigned short vTaskID, CItemsPair& vItemsPair);


	// 取得任务需要完成的物品，在完成任务的时候需要将这些任务物品删除
	bool GetTaskCompleteItems( CEntityPlayer* pPlayer, unsigned short vTaskID, CItemsPair& vItemsPair);

	// 玩家金钱变化，获得任务，ClickNPC，获得经验，OnUpdateRegionTask
	bool UpdateTaskProgress(CEntityPlayer* pPlayer, CTask* pTask, bool& bIsAutoEnd, int nSend = 0, unsigned int nEventID = 0, unsigned int nParam = 0 );


	// 调用lua函数执行任务奖励
	bool DoTaskAwards(CEntityPlayer* pPlayer, CTaskList* pList, unsigned short vTaskID, unsigned short vSelect , int vTaskColor);
	bool DoTaskObtAwards(CEntityPlayer* pPlayer, CTaskList* pList, unsigned short vTaskID);

	//bool GetNpcTaskList( unsigned short vNpcID, CNpcTaskList& vNpcTasks);

	// 得到发给客户端的消息中的NPC任务列表
	bool GetNpcTaskList( CEntityPlayer* pPlayer, CTaskList* pList2, int vNpcID, NpcTaskList* pList);


	bool GetTaskAwardItems( CEntityPlayer* pPlayer, unsigned short vTaskID, CItemsPair& vItemsPair, unsigned short vSelectID);

	bool TestAwardNeedSpace(CEntityPlayer* pPlayer, unsigned short vTaskID, unsigned short vSelectID);

	int  OnPlayerObtainTask(CEntityPlayer* pPlayer, unsigned short unTaskID, int nNpcEntityID );
	int  OnPlayerEndTask(CEntityPlayer* pPlayer, unsigned short unTaskID, unsigned short unSelect, int nBagIndex = -1 );
	int  OnPlayerRefreshTask(CEntityPlayer* pPlayer, unsigned short unTaskID, int mRefreshItemIndex, int nNpcEntityID = 0 );

	// 处理玩家搜索可接任务的请求
	int  OnPlayerSearchTask(CEntityPlayer* pPlayer );

	// 处理玩家邀请任务的请求
	int  OnPlayerInviteTask(CEntityPlayer* pPlayer, CMessagePlayerInviteTaskRequest* pMsg );
	int  SendInviteTaskMsg( const char* pSrcPlayerName, CEntityPlayer* pDesPlayer, unsigned short unTaskID, int nInvitingPlayerID = -1 );
	void OnInviteTaskDesOffLine( CMessageDoPlayerInviteTaskMsg* pMessage );
	void OnTaskInvitedRoutMsg( const char* szSrcName, const char* szDesName , unsigned short unTaskID );
	void OnPlayerReplyTaskInvite( CEntityPlayer* pPlayer, const char* szRequesterName, unsigned short usTaskID, unsigned short usAgreeCode );
	void SendRouterMsg2Client( CMessage* pMessage );

	// 获得环任务数据，为组装消息做准备 
	void GetLoopTaskClientData( CEntityPlayer* pPlayer, CTaskList* pList, LoopTaskStruct* pLoopStruct );

	// 注册Lua函数, 初始化Lua环境
	bool RegisterLua();
	void Send2ClientOprFailed( CEntityPlayer* pPlayer);
	void Send2ClientRefreshFailed( CEntityPlayer* pPlayer);
	void Send2ClientUnRefreshed( CEntityPlayer* pPlayer );
	void Send2ClientRefreshResult( CEntityPlayer* pPlayer, unsigned short vTaskID, int vTaskColor, unsigned short vTaskNumber);
	void Send2ClientRemoveTaskColor( CEntityPlayer* pPlayer, unsigned short vTaskID, int vTaskColor );
	void Send2ClientUnReaccpetTask( CEntityPlayer* pPlayer, unsigned short vTaskID, int vNeedTime );
	void RemoveTalkWithNpc( CEntityPlayer* pPlayer, unsigned short vTaskID, int nNpcTempID );
	void OnNpcRecoverdState( int nPlayerCharID, unsigned short usTaskID, int nNpcTempID );

	// 得到护送NPC的当前目标
	bool GetSendNpcCurTarget( int nEntityID, CWTPoint& rPoint );

	// 得到当前正在护送NPC
	CEntityNpc* GetCurSendNpc( CEntityPlayer* pPlayer );
	int GetTaskState( CEntityPlayer* pPlayer, unsigned short usTaskID );

	// 强制某任务完成
	int OnForceTaskComplete( std::vector<unsigned int>& rEntitys, unsigned short usTaskID );

	// 得到lua虚拟机内存使用kb
	int  GetLuaVMUsedMem( );
	void DaylyDispos( CEntityPlayer* pPlayer );
	void WeeklyDispos( CEntityPlayer* pPlayer );
	int  GetStorageTaskData( CEntityPlayer* pPlayer, CTaskList* pTaskList, StorageTaskStruct* pData );
	int	 OnTaskTimeOut( CEntityPlayer* pPlayer, unsigned short usTaskID );
	int  OnPlayerRedirectMap( CEntityPlayer* pPlayer );
	void ForceRefreshNpcTip( int nNpcEntityID );
	void OnLoadGlobalData( CMessage* pMessage, int vParameter1, int vParameter2 );
	void OnMessagePlayerGetCountryTask( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessagePlayerGetCampTask( CEntityPlayer* pPlayer, CMessage* pMessage );
	int  ResetChairManTasks( CEntityPlayer* pPlayer , unsigned short usChairManID );
	void OnMessagePlayerGetChairManTasks( CEntityPlayer* pPlayer, CMessage* pMessage );
	void TempDispos( CEntityPlayer* pPlayer, CTaskList* pList );
	void SetTodayTimesByGroupID( CEntityPlayer* pPlayer, int nGroupID, int nNum );
	bool OnTaskPlayerNewDayCome(CTaskList* pTaskList, bool bIsOnline=false);
	void SendTaskMsg2Client( CTaskList* pList );
	int  SendTeamLoopErrNotify( CEntityPlayer* pPlayer, const char* pName, int nErrCode);
	int  PlayerFastObtainTask( CEntityPlayer* pPlayer, unsigned short vTaskID, int taskColor);


	// 发布任务 这些接口都在taskmodule里的OnMessage里调用


	bool CheckPublishCondition( CEntityPlayer* pPlayer, unsigned short vTaskID );  // 检查发布任务条件

	int GetGroupNeedMoney(CEntityPlayer* pPlayer, unsigned int vTaskType, unsigned short vTaskID);

	void OnMessagePlayerCheckGroupPublishTask( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageGroupPublishTask( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 向网关发送发布任务请求	
	
	void OnMessageChechPublishResponse( CMessage* pMessage );
	void OnMessageGatePublishResponse( CMessage* pMessage ); 
	

	void OnGateMessage( CMessage* pMessage);

	// 加载lua文件的回调函数（接口文件）
	static void LoadLuaFileCallback(const char* pcConfigPath);
};

class CTaskLuaInterface
{
protected:
	static CEntityPlayer*   mpPlayer;
	static CPropertyPlayer*	mpProperty;
	static CTaskList*       mpTaskList;
	static std::map<int, int>*   mpTmpChecker;

public:
	CTaskLuaInterface()
	{
		mpPlayer		= NULL;
		mpProperty		= NULL;
		mpTaskList		= NULL;
		mpTmpChecker	= NULL;
	}

	static void Set(CEntityPlayer* pOwnerEntity, CTaskList* tasklist) 
	{ 
		if (pOwnerEntity == NULL) // 传入空 需要慎重检查
		{
			//LOG_DEBUG("TASK", "CTaskLuaInterface::Set 传入空");
//#ifdef _DEBUG_
//			print_trace_fd("TASK");
//#endif
			return;
		}
		//LOG_DEBUG("TASK", "CTaskLuaInterface::Set 传入玩家: %s", pOwnerEntity->GetCharNameShow() );
//#ifdef _DEBUG_
//		print_trace_fd("TASK");
//#endif
		mpPlayer   = pOwnerEntity; 
		mpTaskList = tasklist; 
		mpProperty = (CPropertyPlayer*)mpPlayer->GetProperty();
	}

	static bool ResetPlayerInfo( unsigned int nPlayerEntityID )
	{
		CEntityPlayer* tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( nPlayerEntityID );
		if ( tpPlayer == NULL )
			return false;
		mpPlayer   = tpPlayer; 
		mpProperty = (CPropertyPlayer*)mpPlayer->GetProperty();

		CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(tpPlayer->GetTaskToken( ));
		LK_ASSERT( pList,  return false );
		mpTaskList = pList; 
		return true;
	}

	template<typename RVal>
	static RVal Call( CEntityPlayer* pPlayer, CTaskList* pList, const char* name)
	{
		Set( pPlayer, pList );
		return lua_tinker::call<RVal>( CTaskModule::GetSingleton().L, name ); 
	}

	template<typename RVal, typename T1>
	static RVal Call( CEntityPlayer* pPlayer, CTaskList* pList, const char* name, T1 arg)
	{
		Set( pPlayer, pList );
		return lua_tinker::call<RVal, T1>( CTaskModule::GetSingleton().L, name, arg ); 
	}

	template<typename RVal, typename T1, typename T2>
	static RVal Call( CEntityPlayer* pPlayer, CTaskList* pList, const char* name, T1 arg1, T2 arg2)
	{
		Set( pPlayer, pList );
		return lua_tinker::call<RVal, T1, T2>( CTaskModule::GetSingleton().L, name, arg1, arg2 ); 
	}

	template<typename RVal, typename T1, typename T2, typename T3>
	static RVal Call( CEntityPlayer* pPlayer, CTaskList* pList, const char* name, T1 arg1, T2 arg2, T3 arg3)
	{
		Set( pPlayer, pList );
		return lua_tinker::call<RVal, T1, T2, T3>( CTaskModule::GetSingleton().L, name, arg1, arg2, arg3 ); 
	}

	template<typename RVal, typename T1, typename T2, typename T3, typename T4>
	static RVal Call( CEntityPlayer* pPlayer, CTaskList* pList, const char* name, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	{
		Set( pPlayer, pList );
		return lua_tinker::call<RVal, T1, T2, T3, T4>( CTaskModule::GetSingleton().L, name, arg1, arg2, arg3, arg4 ); 
	}

	template<typename RVal, typename T1, typename T2, typename T3, typename T4, typename T5>
	static RVal Call( CEntityPlayer* pPlayer, CTaskList* pList, const char* name, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
	{
		Set( pPlayer, pList );
		return lua_tinker::call<RVal, T1, T2, T3, T4, T5>( CTaskModule::GetSingleton().L, name, arg1, arg2, arg3, arg4, arg5 ); 
	}


public:
	static unsigned short mCurTaskID;  // 当前处理中的任务ID，目前仅仅在奖励前对其进行赋值

	static void RegEventToPlayer( unsigned int nEventID )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return );
		mpTaskList->mTickEvents |= (1<<(nEventID&EVENT_MASK_ID)) ;
	}

	static void ClearEventFromPlayer( unsigned int nEventID )
	{
		LOG_DEBUG("TASK","clear event ID=%d  player=%s", nEventID, mpPlayer->GetCharNameShow() );
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return );
		mpTaskList->mTickEvents &= (~(1<<(nEventID&EVENT_MASK_ID))) ;
	}

	static int GetPlayer(unsigned short vAttrID)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return 0 );
		switch (vAttrID)
		{
		case PLAYER_MONEY:
			return mpProperty->GetMoney( );
			break;
		case PLAYER_EXPR:
			return (int)mpProperty->GetExp( ); // TODO 注意 此处会被截断 因为现在是64位经验
			break;
		case PLAYER_LEVEL:
			return mpProperty->GetLevel( );
			break;
		case PLAYER_SEX:
			return mpProperty->GetSex( );
			break;
		case PLAYER_MEITRE:
			return mpProperty->GetPropertyTempID( );
			break;
		default:
			return 0;
		}

		return 0;
	}

	//static region GetPlayerRegion() 
	//{
	//	LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return );
	//	return region(mpPlayer->GetCharProfile( )->mMapID, mpPlayer->GetPosX( ), mpPlayer->GetPosY( ));
	//}

	static int GetTaskState(unsigned short vTaskID)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return 0);
		return mpTaskList->GetTaskState(vTaskID);
	}

	static bool IsTaskEnd(unsigned short vTaskID)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false );
		return mpTaskList->IsTaskEnd(vTaskID);
	}

	static int SetTaskState(unsigned short vTaskID, int State)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return 0);
		return mpTaskList->SetTaskState(vTaskID, State);
	}

	//  根据环任务ID得到正在进行的子任务Index
	static int GetCurSubTaskIndex( )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return 0);
		return mpTaskList->mLoopTaskData.GetCurSubTaskIndex( );
	}

	//  根据环任务Index得到子任务
	//static int GetSubTaskIDbyIndex( unsigned short usIndex )
	//{
	//	assert(mpTaskList);
	//	return mpTaskList->mLoopTaskData.GetSubTaskIDbyIndex( usIndex );
	//}

	// 获得当前正在做的子任务ID
	static int GetCurSubTaskID()
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return 0);
		return mpTaskList->mLoopTaskData.GetSubTaskIDbyIndex( mpTaskList->mLoopTaskData.GetCurSubTaskIndex( ) );
	}

	// 获得下一个子任务ID
	static int GetNextSubTaskID()
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return 0);
		return mpTaskList->mLoopTaskData.GetSubTaskIDbyIndex( mpTaskList->mLoopTaskData.GetCurSubTaskIndex( ) + 1 );
	}


	// 玩家第一次获得环任务
	static int PlayerGetLoopTask( lua_tinker::table tab )
	{

		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return 0);
		unsigned short tmp[MAX_LOOP_SUB_TASKS] = {0};

		lua_State* L = CTaskModule::GetSingleton().L;
		LK_ASSERT( L != NULL, return 0 );

		int n = luaL_getn(L, -1);
		if (n > (int)CSceneCfgManager::GetSingletonPtr()->GetTask_cfg()->mLooptasksubnum)
		{
			LOG_ERROR("TASK", "over %d looptasks input", (int)CSceneCfgManager::GetSingletonPtr()->GetTask_cfg()->mLooptasksubnum );
			n = (int)CSceneCfgManager::GetSingletonPtr()->GetTask_cfg()->mLooptasksubnum;
		}
		for (int i = 1; i<= n; ++i)
		{
			lua_rawgeti(L, -1, i);
			tmp[i-1] = lua_tinker::pop<unsigned short>(L);
		}


		if ( mpTaskList->mLoopTaskData.PlayerGetLoopTask( tmp, n ) )
		{
			#ifdef _DEBUG_
			char tmp[256] = {0};
			for ( int i = 0 ; i < mpTaskList->mLoopTaskData.GetLoopTaskNum(); ++i )
				sprintf(tmp, "%s %d ", tmp, mpTaskList->mLoopTaskData.GetSubTaskIDbyIndex(i) );

			LOG_DEBUG("TASK", "get loop tasks: %s", tmp );
			#endif
			return mpTaskList->mLoopTaskData.GetSubTaskIDbyIndex(0) ;
		}
		else
			return 0;
	}


	//  设置当前子任务ID(参数表：环任务ID，子任务ID)
	static void  InsertLoopTask( unsigned short vSubTaskID )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return );
		if(CTaskModule::GetSingleton().GetTaskType(vSubTaskID ) == TASKTYPE_LOOP)
		{
			assert(mpTaskList);
			if (mpTaskList->mLoopTaskData.SetCurSubTaskID( vSubTaskID ))
				LOG_DEBUG( "TASK", "change cur task: %d, player ID=%d", vSubTaskID, mpPlayer->GetCharID() );
			else
				LOG_ERROR( "TASK", "error!! cur task: %d, player ID=%d", vSubTaskID, mpPlayer->GetCharID() );
		}
	}


	//  获得今天本玩家完成多少次环任务了
	// 参数：任务类型 
	static int  GetTodayCompletedTimes( int nTaskType )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return 0);

		if ( nTaskType == TASKTYPE_LOOP )
		{
			return mpTaskList->mLoopTaskData.GetTodayCompletedTimes();
		}
		else if ( nTaskType == TASKTYPE_STORAGE )
		{
			return mpTaskList->mStorageTaskMgr.GetTodayCompleted();
		}
		return 0;
	}

	//  玩家放弃环任务时调用
	static void  SetAbortLoopTaskData( )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return );
		LOG_DEBUG( "TASK", "player abort loop, char id=%d", mpPlayer->GetCharID() );
		mpTaskList->mLoopTaskData.SetAbortData( );
	}

	// 得到最后一环ID
	static unsigned short  GetLastLoopTaskID()
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return 0);
		return mpTaskList->mLoopTaskData.GetLastLoopTaskID( );
	}

	static int HasItem(unsigned int vItemID)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return 0);
		return mpProperty->BaggageHasItem(vItemID);
	}

	static CTask* GetTask(unsigned short vTaskID)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return NULL);
		return mpTaskList->GetTask(vTaskID);
	}
	static bool IsSpaceOK(unsigned int vItemID, unsigned short vNumber)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false );
		return mpProperty->TestBaggageSpace( (int*) &vItemID, (int*) &vNumber, 1 );
	}

	static bool AwardBDMoney(int vMoney)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false);
		// 这里处理防沉迷
		if ( mpPlayer->GetFcmStatus() == em_none_rate )
		{
			return true;
		}
		else if ( mpPlayer->GetFcmStatus() == em_half_rate )
		{
			vMoney = vMoney * 0.5;
		}

		if ( CPropertyModule::GetSingleton( ).PlayerChangeMoney( mpPlayer, vMoney, false, true ) == SUCCESS)
		{
			LogEventGetMoneyByTask(mpPlayer, CYuanBao::em_bind_money, vMoney, mCurTaskID);
		}

		return true;
	}

	static bool AwardCommonMoney(int vMoney)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false);
		// 这里处理防沉迷
		if ( mpPlayer->GetFcmStatus() == em_none_rate )
		{
			return true;
		}
		else if ( mpPlayer->GetFcmStatus() == em_half_rate )
		{
			vMoney = vMoney * 0.5;
		}


		if (CPropertyModule::GetSingleton( ).SetPlayerMoney( mpPlayer->GetEntityID(), vMoney ) == 0)
		{
			LogEventGetMoneyByTask(mpPlayer, CYuanBao::em_unbind_money, vMoney, mCurTaskID);
		}

		return true;
	}

	static bool AwardExpr(int vExpr)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return 0);
		int anMemberList[1] = {mpPlayer->GetCharID()};
		int nFinalExp = CPropertyModule::GetSingleton().PlayerObtainExp2( mpPlayer, vExpr, 0, EXPREASON_TASK, anMemberList, 1 );
		if ( nFinalExp > 0)
		{
			LogEventGetExpByTask( mpPlayer, vExpr, nFinalExp, mCurTaskID); 
		}

		return true;
	}

	static bool AwardTask(int vTaskID)
	{
		//LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false);
		////CDispParams tParams;
		////tParams.PushParam( CVariant( CVariant::VT_PVOID,  (long) mpPlayer ) );
		////tParams.PushParam( CVariant( CVariant::VT_PVOID,  (long) mpProperty ) );
		////tParams.PushParam( CVariant( CVariant::VT_USHORT, vTaskID ) );
		////CVariant tResult;

		//LOG_DEBUG("TASK", "player obt task ,player id =%d, task=%d",mpPlayer->GetCharID(), vTaskID );

		//CTaskModule::GetSingleton().PlayerObtainTask( mpPlayer, vTaskID, true );
		return true;
	}

	// 奖励物品 强制绑定的
	static bool AwardItem(int vItemID, int vNumber)
	{
		return PlayerAwardFreeItem(vItemID, vNumber);
		//LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false );
		//return (int)ERROR_INSERTITEM_SUCCESS == CPropertyModule::GetSingleton( ).PlayerInsertItem( mpPlayer, vItemID, vNumber, false, false, 0, 0, "", 0, true );
	}

	static bool PlayerAwardFreeItem(int vItemID, int vNumber)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false );
		CItemObject* pItemObject = CPropertyModule::GetSingleton( ).PlayerInsertItem( mpPlayer, vItemID, vNumber, false );

		if ( pItemObject != NULL)
		{
			LogEventGetItemByEndTask(mpPlayer, pItemObject->GetGUID(), vItemID, vNumber, mCurTaskID);
		}

		return pItemObject != NULL;
	}
	// 扣除东西
	static bool RemoveItem(int vItemID, int vNumber)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false);
		LOG_DEBUG("TASK", "get player %s %ditem num:%d", mpPlayer->GetCharNameShow(), vItemID, vNumber );
		bool tBind = false;
		return CPropertyModule::GetSingleton( ).PlayerRemoveItemByID( mpPlayer, vItemID, vNumber, tBind ) == 0;
	}

	
	static bool AwardLevelEqt(int vItemID, int vNumber, int vLevel)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false);
		if ( vLevel == 0 )
			vLevel = 0-ITEMUPLIMIT;

		CItemObject * pItemObject = CPropertyModule::GetSingleton( ).PlayerInsertItem( mpPlayer, vItemID, vNumber, false, ITEMUPLIMIT+vLevel );

		if ( pItemObject != NULL)
		{
			LogEventGetItemByEndTask(mpPlayer, pItemObject->GetGUID(), vItemID, vNumber, mCurTaskID);
		}

		return pItemObject != NULL;
	}

	static bool AwardItemByMetier(int vItemID, int vNumber, int vMetier, int vIsFree = 0)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false );
		if ( mpPlayer->GetMetier() != vMetier )
			return false;

		if (vIsFree == 1)
			PlayerAwardFreeItem( vItemID, vNumber );
		else
			AwardItem( vItemID, vNumber );

		return true;
	}

	static bool AwardTeleport(int vMapID, int vX, int vY, int nTeleType = 0 )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false);
		bool bRet = false;
		switch(nTeleType)
		{
		case TELEPORT_COMMON:
			{
				bRet = CMapModule::GetSingleton().Teleport( mpPlayer->GetLineID(), vMapID, vX, vY, mpPlayer );
				break;
			}
		case TELEPORT_INFB:
			{
				bRet = CRepetionModule::GetSingleton().RedirectRepetionMap( mpPlayer, vMapID, vX, vY);
				break;
			}
		case TELEPORT_INMENPAI:
			{
				bRet = CMapModule::GetSingleton().Teleport( NEUTRAL_LINE_ID, vMapID, vX, vY, mpPlayer );
				break;
			}
		case TELEPORT_OUTMENPAI:
			{
				bRet = CMapModule::GetSingleton().Teleport( _NATIONALITY_LINE_ID_, vMapID, vX, vY, mpPlayer );
				break;
			}
		default:
			{
				LOG_ERROR("TASK", "error teletype:%d", nTeleType );
				return false;
			}
		}

		if ( !bRet )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( (CEntity*)mpPlayer, ERROR_TASK_TELEPORTFAIL );
			return false;
		}
		return true;
	}

	static bool AwardChangeMetier(int nDesMetier)
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false );
		LOG_DEBUG("TASK", "player changed metier to %d", nDesMetier );

		CPropertyModule::GetSingleton( ).PlayerChangeMetier( mpPlayer, nDesMetier );
		return true;
	}

	static void Debug(const char* vpStr)
	{
		if (vpStr != NULL)
		{
#ifdef _DEBUG_
			const char* pName = ( mpPlayer != NULL ) ? mpPlayer->GetCharNameShow() : "NULL";
			LOG_DEBUG( "TASK_LUA", "Player= %s, info = %s ", pName, vpStr );
#endif
		}
	}

	static void Error(const char* vpStr)
	{
		if (vpStr != NULL)
		{
			const char* pName = ( mpPlayer != NULL ) ? mpPlayer->GetCharNameShow() : "NULL";
			LOG_ERROR( "TASK", "Player= %s, info = %s ", pName, vpStr );
		}
	}

	// 注册任务等级
	static void RegLevelTask( unsigned short unTaskID, unsigned short unLevel )
	{
		// LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return ); 不需要
		CTaskModule::GetSingleton().RegLevelTask( unLevel, unTaskID );
	}

	// 注册掉落表
	static void RegDropTable( lua_tinker::table param )
	{
		unsigned short usTaskID = param.get<unsigned short>("task_id" );
		int nOgreID				= param.get<int>("ogre_id" );
		int nItemID				= param.get<int>("item_id" );
		int n					= param.get<int>("n" );
		int nP					= param.get<int>("p" );
		int nMaxn				= param.get<unsigned short>("maxn" );

		CTaskModule::GetSingleton().RegDropTable( usTaskID, nOgreID, nItemID, n, nP, nMaxn );
	}

	// 检查搜集道具是否复用 nFlag=1的时候表示完毕
	static void CheckCollectItem( int nItemID, unsigned short usTaskID, int nFlag )
	{
		if ( nFlag == 1 )
		{
			if ( mpTmpChecker != NULL )
			{
				delete mpTmpChecker;
				mpTmpChecker = NULL;
				return;
			}
		}

		if (mpTmpChecker == NULL)
		{
			mpTmpChecker = new std::map<int, int>;
			LK_ASSERT( mpTmpChecker != NULL, return );
		}
		
		std::map<int, int>::iterator it = mpTmpChecker->find(nItemID);
		if ( it != mpTmpChecker->end())
		{
			LOG_ERROR("TASK", "collect items re-use!! ID=%d, task ID=%d last id=%d", nItemID, usTaskID, it->second );
			return;
		}
		mpTmpChecker->insert( std::make_pair(nItemID, usTaskID) );

	}

	// 服务器启动时 生成用于交任务时刷的那个库任务的库
	static void InitStaticStorageTask( unsigned short vTaskID )
	{
		st_storage_data tmp;
		CStorageTaskManager::m_Storage.insert(std::make_pair(vTaskID, tmp));
	}

	// 任务加载完毕
	static void LoadTaskComplete()
	{
		CStorageTaskManager::RefreshStorageOnTime();
	}

	static bool CheckEvent( unsigned int nEventID, unsigned int nParam, int nCount )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  false );
		return mpTaskList->mEventMgr.GetTickNum( nEventID, nParam ) >= nCount;
	}

	static bool ClearEvent( unsigned int nEventID, unsigned int nParam, int nCount )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  false );
		return mpTaskList->mEventMgr.RemoveTick( nEventID, nParam, nCount );
	}

	// 检查库任务是否在列表中
	static bool CheckStorageTaskInList( unsigned short usTaskID )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false  );
		return mpTaskList->mStorageTaskMgr.IsInList( usTaskID );
	}

	// 在接任务成功后 ，从列表中干掉
	static void RemoveStorageTask( unsigned usTaskID )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return   );
		mpTaskList->mStorageTaskMgr.RemoveFromList( usTaskID );
	}

	// 奖励BUFF <完成奖励>
	static bool AwardBuff( int nBuffID )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return false  );
		return CPropertyModule::GetSingleton().InsertBuff( mpPlayer, nBuffID );
	}

	// 通用设置事件  目前用在：奖励 和NPC对话完成
	static bool SetEvent( unsigned int nEventID, unsigned int nParam )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  false );
		CTaskModule::GetSingleton().FireEvent( mpPlayer, nEventID, nParam );
		return true;
	}

	// 检查指定任务是否可以接的劫难任务
	static bool IsTragedyTaskAcceptable( unsigned short vTragedyIndex )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  false );
		if (mpTaskList->mTragedyTaskMgr.GetState(vTragedyIndex) == TRAGEDY_ACTIVE)
			return true;
		return false;		
	}

	// 把当前和玩家谈话的NPC变成怪物
	static bool ChangeCurNpc2Ogre( unsigned short vTsakID )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  false );
		int tEntityID = mpTaskList->GetTalkNpcEntityID();
		if ( tEntityID == 0 )
		{
			LOG_DEBUG("TASK", "player no talked with NPC");
			return false;
		}
		CEntityNpc* pNpc = (CEntityNpc*)CSceneLogic::GetSingleton().GetEntity(tEntityID);
		LK_ASSERT( pNpc, return false);

		if ( pNpc->GetNpcChangeState() != 0 || pNpc->GetSendNpcState() != 0 )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( mpPlayer, ERROR_TASK_NPC_BUSY );
			return false;
		}

		if ( 0 == CPropertyModule::GetSingleton().FuncNpcChange2Ogre( mpPlayer, pNpc, vTsakID ) )
		{
			mpTaskList->SetBossNpcEntityID( tEntityID );
		}
		return true;
	}

	static bool CheckNpcOwner( )
	{
		CEntityNpc* pNpc = (CEntityNpc*)CSceneLogic::GetSingleton().GetEntity( mpTaskList->GetTalkNpcEntityID() );
		if (pNpc == NULL)
			return false;
		return pNpc->GetOwnercharID() == (int)mpPlayer->GetCharID();
	}

	// 奖励一个宝箱(暂时只有奇缘用,直接奖励在对话NPC出现的位置)
	static bool AwardBox( int nBoxID )
	{
		if ( mpTaskList->GetTalkNpcEntityID() == 0 )
		{
			LOG_ERROR( "TASK", "AwardBox , player not talked!" );
			return false;
		}
		CEntity* pEntity = CSceneLogic::GetSingleton().GetEntity(mpTaskList->GetTalkNpcEntityID());
		if (pEntity == NULL)
		{
			LOG_ERROR( "TASK", "AwardBox   can't find ID=%d", mpTaskList->GetTalkNpcEntityID() );
			return false;
		}
		CTemplateBox* pTplBox = (CTemplateBox*) CDataStatic::SearchTpl(nBoxID);
		LK_ASSERT( pTplBox != NULL, return false );
		CPropertyModule::GetSingleton().CreateNpc( mpPlayer, nBoxID, ENTITYTYPE_BOX, pEntity->GetPos(), 1000*pTplBox->mLifeTime, true );
		return true;
	}

	// 干掉NPC
	static bool DestroyNpc()
	{
		if ( mpTaskList->GetTalkNpcEntityID() == 0 )
		{
			LOG_ERROR( "TASK", "destroy NPC , player not talked!" );
			return false;
		}
		CEntity* pEntity = CSceneLogic::GetSingleton().GetEntity(mpTaskList->GetTalkNpcEntityID());
		if (pEntity == NULL)
		{
			LOG_ERROR( "TASK", "destroy NPC   can't find ID=%d", mpTaskList->GetTalkNpcEntityID() );
			return false;
		}
		CSceneLogic::GetSingleton().DestroyEntity( pEntity, 0 );
		mpTaskList->SetTalkNpcEntityID(0);
		return true;
	}

	// 开始护送
	// ★本接口中会修改mpPlayer相关值！所以要在结束的时候重置玩家数据★
	static bool StartSendNpc( lua_tinker::table param, int nCurEntityID )
	{
		int nNpcID = param.get<int>("npcid");
		int nMaxGird = param.get<int>("maxgird");
		unsigned short usTaskID = param.get<unsigned short>("taskid");
		int vMaxLeaveTime = param.get<int>("leavetime");
		int nBuffTime = param.get<int>("bufftime");
		int nBuffID = param.get<int>("buffid");
		int nWaitTime = param.get<int>("wait_time");

		lua_tinker::table ret = Call<lua_tinker::table, unsigned short>( mpPlayer, mpTaskList, "get_npc_waypoints", usTaskID );
		int nX[ MAX_WAY_POINTS ];
		int nY[ MAX_WAY_POINTS ];
		int nMapID[ MAX_WAY_POINTS ];

		int n = luaL_getn( CTaskModule::GetSingleton().L, -1);
		if (n > MAX_WAY_POINTS )
		{
			LOG_ERROR("TASK", "send npc task got points num: %d more than %d! task ID=%d", n, MAX_WAY_POINTS, usTaskID );
			n = MAX_WAY_POINTS;
		}
		for (int i = 1; i<= n; ++i)
		{
			lua_rawgeti(CTaskModule::GetSingleton().L, -1, i);
			lua_tinker::table inside = lua_tinker::pop<lua_tinker::table>(CTaskModule::GetSingleton().L);

			nX[i - 1]     = inside.get<int>("x");
			nY[i - 1]     = inside.get<int>("y");
			nMapID[i - 1] = inside.get<int>("map");
		}

		if (nNpcID > 0)
		{
			if ( CTaskModule::GetSingleton().mSendNpcMgr.StartNewSession( mpPlayer, nNpcID, (int*)nX, (int*)nY, (int*)nMapID, n, usTaskID, nMaxGird, 0, vMaxLeaveTime, nBuffTime, nBuffID, nWaitTime ) != 0 )
				LOG_ERROR("TASK", "accept send npc task failed! task=%d #0001", usTaskID );
		}
		else
		{
			if ( CTaskModule::GetSingleton().mSendNpcMgr.StartNewSession( mpPlayer, 0, (int*)nX, (int*)nY, (int*)nMapID, n, usTaskID, nMaxGird, mpTaskList->GetTalkNpcEntityID(), vMaxLeaveTime, nBuffTime, nBuffID, nWaitTime ) != 0 )
				LOG_ERROR("TASK", "accept send npc task failed! task=%d #0002", usTaskID );
		}
		if ( !ResetPlayerInfo(nCurEntityID) )
		{
			LOG_ERROR("TASK", "in StartSendNpc set player error! player id=%d", nCurEntityID);
			return false;
		}
		return true;
	}

	// 在指定地点刷出NPC
	static bool CreateNpc( lua_tinker::table param, int nCurEntityID )
	{
		int nNpcTempID = param.get<int>("npc_id");
		int nPosX = param.get<int>("posx");
		int nPosY = param.get<int>("posy");
		int nMapID = param.get<int>("map_id"); 
		LK_ASSERT( nNpcTempID > 0, return false );

		CTemplateNpc* tpNpcTpl = (CTemplateNpc*) CDataStatic::SearchTpl( nNpcTempID );
		if ( tpNpcTpl == NULL )
		{
			LOG_ERROR("TASK", "can't find npc %d's template", nNpcTempID);
			return false;
		}

		CWTPoint tPos =  CWTPoint( nPosX, nPosY);
		CPropertyModule::GetSingleton().CreateNpc( mpPlayer, nNpcTempID, ENTITYTYPE_FUNCNPC, tPos
			, tpNpcTpl->mLifeTime*1000, true, false, nMapID, 1 );
		if ( !ResetPlayerInfo(nCurEntityID) )
		{
			LOG_ERROR("TASK", "in CreateNpc set player error! player id=%d", nCurEntityID);
			return false;
		}
		return true;
	}

	// 根据组ID得到该组任务当日完成次数
	static int GetTodayTimesByGroupID( int nGroupID )
	{
		LK_ASSERT( mpTaskList!=NULL, return 0 );
		return mpTaskList->mTaskGroupMgr.GetTodayTimes(nGroupID);
	}
	static int GetWeekTimesByGroupID( int nGroupID )
	{
		LK_ASSERT( mpTaskList!=NULL, return 0 );
		return mpTaskList->mTaskGroupMgr.GetWeekTimes(nGroupID);
	}

	// 根据组ID得到该组任务当日接取次数
	static int GetTodayAcceptTimesByGroupID( int nGroupID )
	{
		LK_ASSERT( mpTaskList!=NULL, return 0 );
		return mpTaskList->mTaskGroupMgr.GetTodayAcceptTimes(nGroupID);
	}


	// bool类型的通用检查 以后就不用写那么多C++函数 也不用给LUA注册那么多C++函数
	static bool CommonCheck( int nType, int nParam1= 0, int nParam2=0, int nParam3=0 )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  false );
		switch(nType)
		{
		case CHECKTYPE_ISFAMILYMEMBER:
			{
				return mpPlayer->GetFamilyID() != INVALID_OBJ_ID;
				break;
			}
		case CHECKTYPE_ISFAMILYHEAD:
			{
				if ( mpTaskList->GetInvitingPlayerID() > 0 ) // 是被邀请的  特殊处理  为了做家族护送
					return true;
				return CFamilyModule::GetSingleton().HasFamilyRight( mpPlayer, EM_RIGHT_TASK );
				break;
			}
		case CHECKTYPE_ISCORPSMEMBER:
			{
				return mpPlayer->GetCorpsID() != INVALID_OBJ_ID;
				break;
			}
		case CHECKTYPE_HASTITLE:
			{
				return mpProperty->HasTitle( nParam1 );
				break;
			}
		case CHECKTYPE_BUFF:
			{
				CBuffList* tpBuffList = mpProperty->GetBuffList();
				LK_ASSERT( tpBuffList != NULL, return false );
				return tpBuffList->HasBuff( nParam1 );
				break;
			}
		case CHECKTYPE_COUNTRY:
			{
				return (mpPlayer->GetCountryID()==nParam1);
				break;
			}
		case CHECKTYPE_PUBLISH: // 检查发布 参数1是任务ID 如果剩余次数大于0 则返回true
			{
				break;
			}
		case CHECKTYPE_FAMILY_NPC:
			{
				int nFamilyID = CFamilyModule::GetSingleton().GetFamilyIDByNpcID(nParam1);
				if ( nFamilyID == 0 )
				{
					return false;
				}
				else
				{
					return nFamilyID == mpPlayer->GetFamilyID();
				}
				break;
			}
		case CHECKTYPE_CORPS_NPC:
			{
				int nCorpsID = CCorpsModule::GetSingleton().GetCorpsIDByNpcID( nParam1 );

				if ( nCorpsID == 0 )
				{
					return false;
				}
				else
				{
					return nCorpsID == mpPlayer->GetCorpsID();
				}
				break;
			}
		case CHECKTYPE_ACTIVITY:
			{
				return CActivityModule::GetSingleton().GetActivityStateByID(nParam1) == ACTIVITYSTATE_STARTED;
				break;
			}
		case CHECKTYPE_REG_DATE:
			{
				int tYear = 0;
				int tMonth = 0;
				int tDay = 0;
				int tmp1,tmp2,tmp3;
				char* tpTime = mpPlayer->GetRoleCreateTime();
				if ( tpTime!= NULL && tpTime[0] != 0 )
				{
					sscanf( tpTime, "%d-%d-%d %d:%d:%d", &tYear, &tMonth, &tDay, &tmp1, &tmp2, &tmp3 );
					if ( tYear == 0 || tMonth == 0 || tDay == 0 )
					{
						return false;
					}
					
					if ( tYear < nParam1 )
					{
						return true;
					}
					else if ( tYear == nParam1 ) // 年相等
					{
						if ( tMonth < nParam2 )
						{
							return true;
						}
						else if ( tMonth == nParam2 ) // 月相等
						{
							if ( tDay < nParam3 )
							{
								return true;
							}
							else
							{
								return false;
							}
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
				break;
			}
		case CHECKTYPE_HYDRANGEASTATUS:
			{
				return mpPlayer->GetHydrangeaStatus()==nParam1;
				break;
			}
		case CHECKTYPE_IN_CAMP:
			{
				return nParam1==CSceneLogic::GetSingleton().GetLineID();
				break;
			}
		case CHECKTYPE_ONLINESECS:
			{
				int nOnlineSecs = mpPlayer->GetOnlineTime() + ((unsigned int)time(0) - mpPlayer->GetLastLoginTime());
				return nOnlineSecs >= nParam1;
				break;
			}
		case CHECKTYPE_WEEKNATION:
			{
				return CWorldModule::GetSingleton().m_WeakProtecter.CouldObtainTask(mpPlayer);
				break;
			}
		case CHECKTYPE_IN_MY_SELF: // 当前阵营和玩家一致 返回true
			{
				return mpPlayer->GetNationality() == CSceneServer::GetSingleton( ).mThisServerID;
				break;
			}
		case CHECKTYPE_IS_NATIONKILL:
			{
				return CWorldModule::GetSingleton().IsInGT(nParam1, nParam2);
				break;
			}
		case CHECKTYPE_IS_LUANWU:
			{
				return false;//CWorldModule::GetSingleton().IsInLW(nParam1, nParam2);
				break;
			}
		case CHECKTYPE_IS_MASTER:
			{
				return (mpProperty->mMatrixList.size() > 0);
				break;
			}
		case CHECKTYPE_IS_TEAMHEAD:
			{
				CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( CTeamModule::GetSingleton().InTeam( mpPlayer->GetCharID() ) );
				if (tpTeam == NULL)
					return false;
				return tpTeam->GetHeaderID() == mpPlayer->GetCharID();
				break;
			}
		case CHECKTYPE_TEAM_MEMBER:
			{
				CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( CTeamModule::GetSingleton().InTeam( mpPlayer->GetCharID() ) );
				if (tpTeam == NULL)
					return false;
				return tpTeam->mTeamVec.size() == nParam1;
				break;
			}
		case CHECKTYPE_ISVIP_PLAYER:
			{
				return (mpProperty->mVipFlag > 0);
				break;
			}
		case CHECKTYPE_ISSWORNED_PLAYER:
			{
				return (mpPlayer->GetSwornID() > 0);
				break;
			}
		default:
			{
				LOG_ERROR("TASK", "CommonCheck error, type=%d", nType );
			}
		}
		return false;
	}

	// bool类型的通用奖励 以后就不用写那么多C++函数 也不用给LUA注册那么多C++函数
	static bool CommonAward( int nType, int nParam1=0, int nParam2=0, int nParam3=0 )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  false );
		switch(nType)
		{
		case AWARDTYPE_TELANT:
			{
				CPropertyModule::GetSingleton().PlayerObtainTalentExp( mpPlayer, nParam1 );
				return true;
				break;
			}
		case AWARDTYPE_FAMILY_HONOR:
			{
				int tFamilyID = mpPlayer->GetFamilyID();
				if ( tFamilyID == 0 )
				{
					LOG_ERROR("TASK", "Player %d has no family? but award him family points1?", mpPlayer->GetCharID() );
					return false;
				}
				CFamilyModule::GetSingleton().ChangeFamilyGlory( mpPlayer, nParam1 );
				return true;
				break;
			}
		case AWARDTYPE_FAMILY_MONEY:
			{
				int tFamilyID = mpPlayer->GetFamilyID();
				if ( tFamilyID == 0 )
				{
					LOG_ERROR("TASK", "Player %d has no family? but award him family points2?", mpPlayer->GetCharID() );
					return false;
				}
				CFamilyModule::GetSingleton().ChangeFamilyMoney( mpPlayer, nParam1 );
				return true;
				break;
			}
		case AWARDTYPE_FAMILY_CONTRIBUTE:
			{
				int tFamilyID = mpPlayer->GetFamilyID();
				if ( tFamilyID == 0 )
				{
					LOG_ERROR("TASK", "Player %d has no family? but award him family points3?", mpPlayer->GetCharID() );
					return false;
				}
				CFamilyModule::GetSingleton().AddPlayerFamilyContribute( mpPlayer->GetCharID(), nParam1 );
				return true;
				break;
			}
		case AWARDTYPE_CORPS_GLORY:
			{
				int tCorpsID = mpPlayer->GetCorpsID();
				if ( tCorpsID == 0 )
				{
					LOG_ERROR("TASK", "Player %d has no corps? but award him corps points1?", mpPlayer->GetCharID() );
					return false;
				}
				CCorpsModule::GetSingleton().ChangeCorpsGlory( mpPlayer, nParam1 );
				return true;
				break;
			}
		case AWARDTYPE_CORPS_MONEY:
			{
				int tCorpsID = mpPlayer->GetCorpsID();
				if ( tCorpsID == 0 )
				{
					LOG_ERROR("TASK", "Player %d has no corps? but award him corps points2?", mpPlayer->GetCharID() );
					return false;
				}
				CCorpsModule::GetSingleton().ChangeCorpsMoney( mpPlayer, nParam1 );
				return true;
				break;
			}
		case AWARDTYPE_CORPS_CONTRIBUTE:
			{
				int tCorpsID = mpPlayer->GetCorpsID();
				if ( tCorpsID == 0 )
				{
					LOG_ERROR("TASK", "Player %d has no corps? but award him corps points3?", mpPlayer->GetCharID() );
					return false;
				}
				CCorpsModule::GetSingleton().AddPlayerCorpsContribute( mpPlayer->GetCharID(), nParam1 );
				return true;
				break;
			}
		case AWARDTYPE_TITLE:
			{
				CPropertyModule::GetSingleton().OnPlayerAddTitleEvent( mpPlayer, nParam1 );
				return true;
				break;
			}
		case AWARDTYPE_HONOR:
			{
				CPropertyModule::GetSingleton().OnPlayerObtainHonor( mpPlayer, nParam1 );
				return true;
				break;
			}
		case AWARDTYPE_BINDYINGYUANBAO:
			{
				CPropertyModule::GetSingleton().PlayerChangeYuanbao( mpPlayer, nParam1, true );
				return true;
				break;
			}

		case AWARDTYPE_COMBAT_FAME:
			{

				CPropertyModule::GetSingleton().ChangeCombatFame( mpPlayer, nParam1 );
				return true;
				break;
			}
		case AWARDTYPE_REPE_FAME:
			{

				CPropertyModule::GetSingleton().ChangeRepeFame( mpPlayer, nParam1 );
				return true;
				break;
			}
		case AWARDTYPE_COMMON_FAME:
			{
				CPropertyModule::GetSingleton().ChangeCommonFame( mpPlayer, nParam1 );
				return true;
				break;
			}
		case AWARDTYPE_COUNTRYBUILD:
			{
				// 奖励建设度 1是数值 2是类型 必须和EOfficial枚举对应！！
				LOG_DEBUG("TASK", "award country build:%d", nParam1);
				CWorldModule::GetSingleton().SendChangeConstructRequest(mpPlayer->GetNationality()
					, mpPlayer->GetCountryID(), &nParam2, &nParam1, 1 );
				return true;
				break;
			}
		case AWARDTYPE_UPWIS:
			{
				// 上报才华
				CWorldModule::GetSingleton().SendChangeWizardRequest(mpPlayer->GetNationality()
					, mpPlayer->GetCountryID(), 0, mpPlayer );
				return true;
				break;
			}
		case AWARDTYPE_WISEXP:
			{
				// 奖励才华经验 类型顺序按张三写的 protype的
				CPropertyModule::GetSingleton().PlayerObtainWizardValue( mpPlayer, nParam1, nParam2 );
				break;
			}
		case AWARDTYPE_CLEARHYDRANGEASTATUS:
			{
				CActivityModule::GetSingleton().ClearHydrangeaStatus( mpPlayer );
				break;
			}
		case AWARDTYPE_CORPS_COLLECTACTIVE:
			{
				int tCorpsID = mpPlayer->GetCorpsID();
				if ( tCorpsID == 0 )
				{
					return false;
				}
				CCorpsModule::GetSingleton().ChangeCorpsCollectActive( mpPlayer->GetCharID(), nParam1 );
				return true;
				break;
			}
		default:
			{
				LOG_ERROR("TASK", "CommonAward error, type=%d", nType );
			}
		}
		return false;
	}

	// 【接任务奖励】邀请家族成员一起做任务【只有族长才能用】  
	// ★本接口中会修改mpPlayer相关值！所以要在结束的时候重置玩家数据★
	static bool InviteFamilyMembers( unsigned short usTaskID, unsigned int nCurEntityID )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  false );
		LK_ASSERT( usTaskID > 0 , return false );
		if( !CFamilyModule::GetSingleton().HasFamilyRight( mpPlayer, EM_RIGHT_TASK ) )
		{
			// 所有人接任务都会走到这 所以就不用报错了
			//LOG_ERROR("TASK", "player: entity:%d char:%d name:%s is not family head!", mpPlayer->GetEntityID(), mpPlayer->GetCharID(), mpPlayer->GetCharNameShow() );
			return false ;
		}
		int tMemberEntityIDs[MAX_FAMILYMEMBER_NUM] = {0};
		int tNum = MAX_FAMILYMEMBER_NUM;
		CFamilyModule::GetSingleton().GetFamilyMemberList( mpPlayer->GetFamilyID(), tMemberEntityIDs, tNum );
		LK_ASSERT( tNum >= 0 && tNum < MAX_FAMILYMEMBER_NUM, return false );
		for ( int i = 0; i < tNum; ++i )
		{
			if ( tMemberEntityIDs[i] == (int)mpPlayer->GetEntityID() )
				continue;
			CEntityPlayer* tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( tMemberEntityIDs[i] );
			// TODO 临时先这么写 以后得想办法根治 现在逻辑复杂了以后就会交叉！！！就会出现逻辑问题！
			if ( !ResetPlayerInfo(nCurEntityID) )
			{
				LOG_ERROR("TASK", "in InviteFamilyMembers set player error! player id=%d", nCurEntityID);
				continue;
			}
			CTaskModule::GetSingleton().SendInviteTaskMsg( mpPlayer->GetCharName(), tpPlayer, usTaskID, mpPlayer->GetEntityID() );
		}
		if ( !ResetPlayerInfo(nCurEntityID) )
		{
			LOG_ERROR("TASK", "in InviteFamilyMembers set player error! player id=%d", nCurEntityID);
			return false;
		}
		return true;
	}

	// 得到玩家所在家族的平均等级
	static int GetFamilyAvgLevel()
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  0 );
		int tLevel = 0;
		CFamilyModule::GetSingleton().GetFamilyAverageLevel( mpPlayer->GetFamilyID(), tLevel );
		return tLevel;
	}

	// 开始计时任务定时器
	// nType: 物理时间/游戏时间 (暂时没用)
	static int StartTimeLimitTaskTimer( unsigned short usTaskID, unsigned int nTime, int nType )
	{
		LK_ASSERT( nTime > 0, return -1);
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  -1 );
		CTask* pTask = mpTaskList->GetTask(usTaskID);
		LK_ASSERT( pTask != NULL, return -1 );
		int nTimerOtherInfo[ TIMER_OTHER_INFO ] = { MESSAGE_PROPERTY, usTaskID, 0, 0};
		pTask->mTimerID = mpPlayer->GetTimerReciver()->SetTimer( mpPlayer->GetEntityID(), TIMER_TASKTIMELIMIT, nTime*1000
			, 2, nTimerOtherInfo );
		LOG_DEBUG("TASK", "task%d created timer:%d", usTaskID, pTask->mTimerID );
		pTask->mGameTimeLeft = nTime;
		return 0;
	}

	// 检查任务组互斥
	static bool CheckMutexByTask( int vGroupID )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  false );
		for ( int i = 0; i < mpTaskList->mTaskNumber; ++i )
		{
			if ( GetTaskGroupID(mpTaskList->mTaskList[i].mTaskID) == vGroupID )
				return false;
		}
		return true;
	}

	// 设置跨地图失败任务
	static int  SetRedirectFailTask( unsigned short vTaskID )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  false );
		mpTaskList->mRedirectFailTask = vTaskID;
		return 0;
	}

	// 注册需要监控的NPC
	static int  RegMonitorNpc( int nTempID )
	{
		if ( nTempID == 0 )
			return -1;
		CTaskModule::GetSingleton().mNpcMonitor.RegWatchNpc( nTempID );
		return 0;
	}

	// 检查NPC状态 
	static bool GetWatchNpcState( int nTempID )
	{
		// TODO 目前只是检查是不是活着
		unsigned int nEntityID = CTaskModule::GetSingleton().mNpcMonitor.GetNpcEntityID( nTempID );
		if ( nEntityID == 0 )
		{
			LOG_DEBUG("TASK", "in %s nEntityID=0 tempid=%d", __FUNCTION__, nTempID );
			return false;
		}
		// 这里在添加的时候已经检查过了 一定是CEntityNpc*
		CEntityNpc* tpNpc = (CEntityNpc*)CSceneLogic::GetSingleton().GetEntity( nEntityID );
		if ( tpNpc == NULL )
		{
			LOG_DEBUG("TASK", "in %s tpNpc=NULL tempid=%d", __FUNCTION__, nTempID );
			return false;
		}
		return tpNpc->IsAlive();
	}

	// 发系统邮件
	static bool SendSysMail( lua_tinker::table param )
	{
		int nItemIDs[5] = { param.get<int>("item_id1"), param.get<int>("item_id2"), param.get<int>("item_id3")
		, param.get<int>("item_id4"), param.get<int>("item_id5") };

		int nItemNums[5] = { param.get<int>("item_num1"), param.get<int>("item_num2"), param.get<int>("item_num3")
			, param.get<int>("item_num4"), param.get<int>("item_num5") };

		int nTotalNum = 0;
		for ( int i = 0; i < 5; ++i )
		{
			if ( nItemIDs[i] == 0 )
				break;
			++nTotalNum;
		}

		CMailModule::GetSingleton().SendSysMail( mpPlayer->GetCharName(), mpPlayer->GetCharID()
			, param.get<char*>("title"), param.get<char*>("src_name"),param.get<char*>("body")
			, nItemIDs, nItemNums, nTotalNum, mpPlayer->GetNationality(), param.get<int>("money"), mpPlayer->GetCharID() ) ;

		return true;
	}

	// 注册守旗任务
	static bool RegProtectFlagTask( unsigned short usTaskID, int nNpcTempID )
	{
		// 守旗任务要注册NPC死亡任务失败 这种类型
		CTaskModule::GetSingleton().mNpcMonitor.RegGlobalTaskAction( nNpcTempID, NPC_DIE_FAIL, mpPlayer, usTaskID );
		return true;
	}

	// 获得某些数据（数值类）
	static int CommonGetData( int vType )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  0 );
		
		switch( vType )
		{
		case GET_WIZARD_POLITICS:
			{
				return ( mpProperty->mWizard.GetWizardPoint(WIZARD_POLITICS)+mpProperty->mWizard.GetWizardIncrease(WIZARD_POLITICS) );
				break;
			}
		case GET_WIZARD_WISDOM:
			{
				return ( mpProperty->mWizard.GetWizardPoint(WIZARD_WISDOM)+mpProperty->mWizard.GetWizardIncrease(WIZARD_WISDOM) );
				break;
			}
		case GET_WIZARD_ARTS:
			{
				return ( mpProperty->mWizard.GetWizardPoint(WIZARD_ARTS)+mpProperty->mWizard.GetWizardIncrease(WIZARD_ARTS) );
				break;
			}
		case GET_WIZARD_EDUCATE:
			{
				return ( mpProperty->mWizard.GetWizardPoint(WIZARD_EDUCATE)+mpProperty->mWizard.GetWizardIncrease(WIZARD_EDUCATE) );
				break;
			}
		case GET_WIZARD_TACTICS:
			{
				return ( mpProperty->mWizard.GetWizardPoint(WIZARD_TACTICS)+mpProperty->mWizard.GetWizardIncrease(WIZARD_TACTICS) );
				break;
			}
		case GET_WIZARD_POWER:
			{
				return ( mpProperty->mWizard.GetWizardPoint(WIZARD_POWER)+mpProperty->mWizard.GetWizardIncrease(WIZARD_POWER) );
				break;
			}
		case GET_PLAYER_CAMP:
			{
				return mpPlayer->GetNationality();
				break;
			}
		default:
			{
				LOG_ERROR("TASK", "error get type:%d", vType );
				break;
			}
		}
		return 0;
	}

	static int GetCurCampID()
	{
		return CSceneServer::GetSingleton( ).mThisServerID;
	}

	static int GetWeakCampLevel( int nCampID ) // 获得弱阵营等级
	{
		return CWorldModule::GetSingleton().m_WeakProtecter.GetWeekLevel(nCampID);
	}

	static bool PublishCheck( int nType, int nParam1= 0, int nParam2=0, int nParam3=0 )
	{
		LK_ASSERT( mpTaskList && mpPlayer && mpProperty, return  false );
		switch(nType)
		{
		case CHECK_CORPS_LEVEL:
			{
				return mpTaskList->mGroupTempInfo.GroupLevel >= nParam1;
				break;
			}
		case CHECK_CORPS_MONEY:
			{
				return mpTaskList->mGroupTempInfo.GroupMoney >= nParam1;
				break;
			}
		case CHECK_CORPS_POSITION:
			{
				return mpTaskList->mGroupTempInfo.PlayerPosition <= nParam1;
				break;
			}
		default:
			{
				LOG_ERROR("TASK", "error get type:%d", nType );
				break;
			}
		}
		return false;
	}
};


