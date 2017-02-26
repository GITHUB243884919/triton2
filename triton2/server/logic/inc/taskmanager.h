#pragma once
#include "protype.h"
#include "westserver.h"
#include "property_pb.hxx.pb.h"
#include "taskmessage_pb.hxx.pb.h"
#include "tasktype.h"
#include "sceneobj_define.h"
#include "entity.h"
#include "servercore.h"
#include "familytype.h"

struct st_storage_data
{
	unsigned short m_usBackRefreshTasks[STORAGE_SIZE];
	unsigned short m_nRefreshNum;
	st_storage_data ()
	{ 
		if ( CObj::msCreateMode)
		{ memset( m_usBackRefreshTasks, 0, sizeof(m_usBackRefreshTasks) ); m_nRefreshNum = 0;}
	}
};

struct st_event
{
	unsigned int nEventID;
	unsigned int nParam;
	st_event()
	{
		if ( CObj::msCreateMode)
		{ nEventID = 0 , nParam = 0 ;}
	}
	st_event( unsigned int vEventID, unsigned int vParam )
		: nEventID ( vEventID ), nParam( vParam )
	{}
	bool operator < ( const struct st_event& other )  const;
};

struct st_event_single
{
	st_event stevent;
	int		 nTicked;	// 响应了多少次

	st_event_single()
	{
		if ( CObj::msCreateMode)
		{ nTicked = 0; }
	}
	st_event_single( int vEventID, int vParam );

	bool operator < ( const struct st_event_single& other )  const;
};


struct st_drop_data 
{
	int m_ItemID;	// 掉落的物品的ID
	int m_n;		// 掉落数量
	int m_p;		// 掉落概率
	int m_maxn;		// 本任务最多要搜集该物品的个数

	st_drop_data()	
	{
		m_ItemID = 0; m_n = 0; m_p = 0; m_maxn = 0; // 这个是临时数据 不用存放在共享内存 在reload的时候会重新加载
	}
};

// 劫难任务 单个结构
struct st_tragedy_task
{
	unsigned short  us_NextTaskID;// 该做哪个子任务了，如果他没做完过就是0，在交任务的时候设置为下一个子任务，并发给客户端,全部完成了，也是0 但是状态是完成状态
	short	        s_State;	 // 当前状态

	st_tragedy_task()
	{
		if ( CObj::msCreateMode)
		{ us_NextTaskID=0;s_State=0; }
	}
};

struct st_invite_data
{
	char name[NAME_LENGTH];
	unsigned short task_id;
};

struct st_refresh_data 
{
	int rTaskID;
	int rTaskColor;
	bool bflag;

	void Inintialize()
	{
		rTaskID = 0;
		rTaskColor = 0;
		bflag = false;
	}
	st_refresh_data()
	{
		rTaskID = 0;
		rTaskColor = 0;
		bflag = false;
	}
};


typedef std::map<unsigned short, st_storage_data>			STORAGE_TASKS_CONTAINER;
typedef std::map<int, st_drop_data>							DROP_DATA_CONTAINER;
typedef lk::hash_map<unsigned int, std::vector<st_invite_data>, SERVER_CAP_PLAYER > TASK_INVITE_CONTAINER;
typedef lk::hash_map<unsigned short, int, TASK_LIST_SIZE>					ID_MAP_CONTAINER;
//typedef lk::hash_map<unsigned short, unsigned short>			SAVE_DB_TASKS_TYPE;

// ***********************************************************
//  类型描述：	访问磁盘文件
// **********************************************************
class CDir
{
private:
	DIR *dir;
	struct dirent *dp;
public:
	int bad;
	CDir(const char* dirpath);	  
	const char* file();
	~CDir();
};


// ***********************************************************
//  类型描述：	单个任务描述（不论环任务库任务还是普通任务）
// **********************************************************
class CTask
{
public:
	unsigned short		mTaskID;			// 任务ID
	unsigned short		mTaskProgress;		// 任务进度
	int					mTimerID;				// 限时任务的计时器ID
	int					mGameTimeLeft;			// 游戏时间剩余秒数 非限时任务为-1 注：实际计时以定时器为主 这个数值只是在接任务和下线的时候更新下
	unsigned int		mTaskData[TASK_DATA_SIZE]; // 任务数据 一定是以pair方式使用
	unsigned int		mWatchEventN;		// 监视事件剩余次数  仅用于给EntiyDie提速
	int					mTalkedNpc[MAX_NPC_TALK_NUM]; // 已经对话过的NPC
	int					mTalkedNum;
	int					mExFlags;			// 额外标记 为了提速 目前：按等级杀怪用到了 枚举见enTaskExFlags
	int					mTaskBeginTime;		// 接受任务时间

private:
	CSearchArray<st_event,	MAX_EVENTS_PER_TASK>  mEventData;

public:
	CTask();

public:
	void RegEventTick( unsigned int vEventID, unsigned int vParam, unsigned int vNum );
	// 是否命中 如果是监视事件，在此添加到监视
	bool IsEventTick ( unsigned int vEventID, unsigned int vParam );
	void GetTaskDataStruct(TaskStruct* pData);
	unsigned short		GetTaskProgress() {return mTaskProgress; } // 得到任务进度
	void SetTaskProgress( TaskState state ) {  mTaskProgress = state; } // 得到任务进度

	// 是否和某NPC对过话
	bool IsTalkedWithNpc( int nNpcTempID );

	// 增加和NPC的对话
	bool AddTalkedWithNpc( int nNpcTempID );
	// 减少
	bool RemoveTalkedWithNpc( int nNpcTempID );

	inline void SetTaskData(unsigned int idx, unsigned int v) { if(idx < TASK_DATA_SIZE) mTaskData[idx] = v; } 
	inline unsigned int GetTaskData(unsigned int idx){ return (idx < TASK_DATA_SIZE) ? mTaskData[idx] : 0; }

	// 得到第一个某事件的param
	int GetFirstParamByEvent( int nEventID );
};

// ***********************************************************
//  类型描述：	环任务管理器
// **********************************************************
class LoopTaskData
{
protected:
	unsigned int		mTodayCompletedCircles;	// 今天已经完成的所有环任务的次数
	time_t				mLastCompletedTime;	    // 上次完成环任务的时间

	unsigned short		mLoopTasks[MAX_LOOP_SUB_TASKS];
	int					mCompletedTimes;		    //  已完成次数(整轮)
	time_t				mLastAbortTime;		    //  本轮上次放弃时间
	short				mCurSubIndex;		    //  当前进行中的子任务索引，只有接任务才会更新该值，交任务后该值不变 初始-1
	short				mSubTasksNum;			//  子任务的总数
	int					mWeekLoops;				//  本自然周完成的轮数  每天至多只记1条
	int					mDayCompletedLoopFlag;	//  今日是否完成一整轮环任务的标志
	int					mCompletedSingle;		//  完成环任务的个数（子任务）累计满1轮（10个）然后清0
	void				CheckInitTodayLoops();


public:
	LoopTaskData();
	bool GetRndTasks( unsigned short arrSrc[], int nSrcNum, int nOutNum, unsigned short* pOut );
	void Clear();

public:
	// =====  Gets ==== //
	int     GetDayCompletedLoops() {return mDayCompletedLoopFlag;}
	int     GetSubTaskNum() {return mSubTasksNum;}
	int		GetCompletedTimes();
	time_t	GetLastAbortTime();
	short   GetCurSubTaskIndex();
	unsigned short GetSubTaskIDbyIndex(short usIndex);
	int     GetLoopTaskNum();
	// 随机生成环任务
	bool    PlayerGetLoopTask( unsigned short usArrSrc[], int n );
	int     GetTodayCompletedTimes();
	unsigned short GetLastLoopTaskID();

	// =====  Sets ==== //
	bool   SetCurSubTaskID( unsigned short vSubTaskID );
	void   SetAbortData( );
	// 当玩家完成一轮的时候调用
	int    SetEndLoopData( );
	void   GetTlvBuffer2(PBLoopTaskData*& pBuffer);
	void   CreateFromTlv2(PBLoopTaskData*& pBuffer);
	// 每完成一个环任务就响应 如果导致完成了1轮 则返回本周轮数 否则返回0
	int   OnCompletedTask( unsigned short vTaskID );
	// 得到第几环(从1开始，找不到则返回0)
	int    GetLoopByTaskID( unsigned short usTaskID );
	unsigned short	GetWeekLoops(){ return  mWeekLoops;	}			//  本自然周完成的轮数  每天至多只记1条 
	// 新一天的处理
	void   OnNewDay();
	// 新一周的处理
	void   OnNewWeek();
	// 得到本轮随便一个子任务ID
	unsigned short  GetOneSubTaskID() { return mLoopTasks[0]; }
};


// ***********************************************************
//  类型描述：	事件管理器
// **********************************************************
class CTaskEventManager
{
public:
	CSearchArray<st_event_single, MAX_SAVE_EVENT> mEvents;

public:
	bool RegEvent( unsigned int nEventID, unsigned int nParam ); // 触发并记录事件
	int  GetTickNum( unsigned int nEventID, unsigned int nParam ); // 得到触发次数
	bool RemoveTick( unsigned int nEventID, unsigned int nParam , int n ); // 清除事件记录
};


class CTaskList;
// **********************************************************
//  类型描述：	库任务（皇榜任务）管理器
// **********************************************************
class CStorageTaskManager
{
	friend class CTaskList;
private:
	CSearchArray<unsigned short, MAX_STORAGETASK_LIST_NUM> m_Tasks; // 玩家身上的库任务列表
	CSearchArray<unsigned short, MAX_STORAGETASK_LIST_NUM> m_GotTasks;  // 已接受的

	time_t		   m_nLastClickNpc;		// 玩家上次点击NPC的时间（如果是次日，则刷新任务列表）
	unsigned short m_usCurID;			// 当前库任务ID
	int			   m_nGotNum;			 //  已经接在身上的库任务数量
	int			   m_nTodayCompleted;	//	今天已经完成的次数 (完成以后修改)
	int			   m_nTodayRMBTimes;	// 今天已经使用RMB道具刷新过的次数

public: //TODO:在服务器以恢复模式启动的时候要重新初始化
	static STORAGE_TASKS_CONTAINER m_Storage;
	static void	RefreshStorage(unsigned short vTaskID);
	static void	RefreshStorageOnTime( );

	CStorageTaskManager();
	bool IsInList( unsigned short usTaskID );
	void RemoveFromList( unsigned short usTaskID );
	bool OnAbortTask( unsigned short vTaskID );
	bool OnEndTask( unsigned short vTaskID );
	int	 GetGotNum() { return m_nGotNum; }
	int	 GetTaskNum() { return m_Tasks.GetNum(); }

	int	 GetTodayCompleted() { return m_nTodayCompleted; }
	void ZeroTodayCompleted() { m_nTodayCompleted = 0; }
	void PlusTodayCompleted() { ++m_nTodayCompleted; }

	void GetTlvBuffer2(PBStorageTaskData*& pBuffer);
	void CreateFromTlv2(PBStorageTaskData*& pBuffer);

	unsigned short GetCurTaskID() { return m_usCurID; }

	int  GetTodayRMBTimes() { return m_nTodayRMBTimes; }
	void ZeroTodayRMBTimes() { m_nTodayRMBTimes=0; }
	void PlusTodayRMBTimes() { ++m_nTodayRMBTimes; }
};



// **********************************************************
//  类型描述：	劫难任务管理器
// **********************************************************
class CTragedyTaskMgr
{
public:
	// 得到某个劫难该进行的子任务ID
	unsigned short GetNextTask( short vTragedyIndex );
	void		   SetNextTask( short vTragedyIndex, unsigned short vTaskID );
	int			   GetState( short vTragedyIndex );
	void		   SetState( short vTragedyIndex, enTragedyTaskState vState );

	void CreateFromTlv2( PBTaskList* pBuffer );
	void GetTlvBuffer2( PBTaskList* pBuffer );

	int  GetAllInfo( int *anIndex, unsigned short *asNextID, int *anState );

	void Clear( int nIndex );

	CTragedyTaskMgr() { m_nCompleted = 0; }
	void PlusCompleted() { ++ m_nCompleted; }
	int  GetCompleted() { return m_nCompleted; }
	void SetCompleted(int nVal) {m_nCompleted = nVal;}
private:
	st_tragedy_task m_tasks[TRAGEDY_TASK_SIZE+1]; // 用1开始的索引存储
	int				m_nCompleted;	// 已完成的劫难个数
};


// ***********************************************************
//  类型描述：	 任务组策略管理器
//  主要工作：
//  维护以组为单位的任务数据
// **********************************************************
class CTaskGroupData
{
private:
	int				mTodayTimes;	// 今日完成次数
	int				mTodayAccptTimes;	// 今日接取次数
	int				mWeekTimes;		//本周完成任务次数
	int				mWeekAccptTimes;   //本周接取次数

public:
	CTaskGroupData()
	{
		if ( CObj::msCreateMode )
		{
			mTodayTimes = 0;
			mTodayAccptTimes = 0;
			mWeekTimes = 0;
			mWeekAccptTimes = 0;
		}
	}
	void OnEndOneTask();	// 完成了一次本组的任务
	void OnAccptOneTask();	// 接取了一次本组的任务


	int GetTodayTimes() { return mTodayTimes;}
	int GetTodayAccptTimes() { return mTodayAccptTimes;}
	int GetWeekTimes(){ return mWeekTimes; }
	int GetWeekAccptTimes(){return mWeekAccptTimes; }

	void SetTodayTimes( int nValue) { mTodayTimes = nValue;}
	void SetTodayAccpTimes( int nValue) { mTodayAccptTimes = nValue;}
	void SetWeekTimes(int nValue) { mWeekTimes = nValue; }
	void SetWeekAccptTimes(int nValue) { mWeekAccptTimes = nValue; }

};

typedef lk::hash_map<unsigned int, CTaskGroupData, MAX_TASKGROUP_NUM> TaskGroupMgrType;

class CTaskGroupManager
{
private:
	TaskGroupMgrType mData;

public:
	CTaskGroupManager()
	{
		if (CObj::msCreateMode)
		{
			mData.initailize();
		}
	}

public:	// interfaces
	int  OnEndOneTask( unsigned int nGroupID );	// 完成了一个带组ID的任务
	int  OnAcceptOneTask( unsigned int nGroupID );	// 接受了一个带组ID的任务

	int  GetTodayTimes( unsigned int nGroupID );	// 得到今天完成某组的任务次数
	int  GetTodayAcceptTimes( unsigned int nGroupID );	// 得到今天完成某组的任务次数
	int  OnNewdayCome();	// 新的一天到来
	void  SetTodayTimes( unsigned int nGroupID, int nTimes );

	int	 GetWeekTimes( unsigned int nGroupID ) ;      //得到本周完成某组任务的次数
	int  GetWeekAcceptTimes( unsigned int nGroupID ); //得到本周接受某组任务的次数
	int  OnNewWeekCome();	// 新的一周到来
	void  SetWeekTimes(unsigned int nGroupID, int nTimes );

public:	// data manager
	void CreateFromPB( PBTaskList* pBuffer );
	void GetPBBuffer( PBTaskList* pBuffer );
};


// **********************************************************
//  类型描述：	掌门任务管理器
// **********************************************************
struct SingleChairManTask_T
{
	unsigned short  mTaskID;
	int				mState;
};

typedef lk::hash_map<unsigned short, int, 50> CHAIRMAN_COMPLETED_TYPE;

class CChairManTaskManager
{
private:
	//SingleChairManTask_T  m_Tasks[CHAIRMAN_TASK_NUM]; // 玩家专属的任务列表
	unsigned short  m_usCurID;			// 当前掌门任务ID
	int				m_nAccepted;		// 当前已接受数量
public: 
	CHAIRMAN_COMPLETED_TYPE mCompleted; // 已完成数量
	CChairManTaskManager();
	void Initialize();
	void GetTlvBuffer2(PBChairManTaskData* pBuffer);
	void CreateFromTlv2(PBChairManTaskData* pBuffer);
	void FillMessage2Cli( CMessageGetChairManTaskResponse* pMsg );
	unsigned short GetCurID( ) { return m_usCurID ; } 
	int  GetCompleteNum( ) ;
	void ResetTasks( int nCurID );
	void OnTaskEnd();
	bool CheckAcceptChairMan( unsigned short usChairManID );
	void OnAccepted();
	void OnAborted();
	int  GetAcceptedNum() {return m_nAccepted;}
	void SetAcceptedNum( int nAcc ) { m_nAccepted=nAcc;}
};

typedef lk::hash_map<unsigned short, unsigned short, PUBLISH_LIST_SIZE> PUBLISHTASK;// 发布任务的信息    first : 任务ID second :任务状态


struct GroupTempInfo
{
	unsigned int GroupID;		// 团体ID
	unsigned int GroupLevel;	// 团体等级
	unsigned int GroupMoney;	// 团体金钱
	unsigned int PlayerPosition; // 玩家在团体的职位

	GroupTempInfo()
	{
		GroupID			= 0;
		GroupLevel		= 0;
		GroupMoney		= 0;
		PlayerPosition	= 0;
	}
	GroupTempInfo(unsigned int ID, unsigned int Level, unsigned int Money, unsigned int Position)
	{
		GroupID				= ID;
		GroupLevel			= Level;
		GroupMoney			= Money ;
		PlayerPosition		= Position;
	}
};
// ***********************************************************
//  类型描述：	玩家身上任务单元
// **********************************************************
class CTaskList : public CObj
{
public:
	unsigned short					mTaskNumber;		// 已接受的任务个数
	CTask							mTaskList[ TASK_LIST_SIZE ];	// 已接受的任务列表
	//unsigned char					mCompleteTasks[COMPLETE_TASK_BIT_SIZE]; 
	lk::hash_map<unsigned short, unsigned short, COMPLETE_TASK_NUM> mCompleteTasks;// 完成的且不可重复做的任务
	//unsigned char					mFailedTasks[COMPLETE_TASK_BIT_SIZE]; // 失败但是不能重复做的任务
	lk::hash_map<unsigned short, unsigned short, FAILED_TASK_NUM> mFailedTasks;// 失败但是不能重复做的任务

	lk::hash_map<unsigned short, int, COMPLETE_TASK_NUM> mRepeatTasks;// 完成的可重复做的任务

	st_refresh_data mRememberRefreshNum[ TASK_LIST_SIZE ];  // 最多纪录的T任务品质数量

	LoopTaskData					mLoopTaskData;      // 环任务数据
	CTaskEventManager				mEventMgr;
	CStorageTaskManager				mStorageTaskMgr;    // 库任务数据
	CTragedyTaskMgr					mTragedyTaskMgr;	// 劫难任务管理器
	CTaskGroupManager				mTaskGroupMgr;		// 任务组管理器
	CChairManTaskManager			mChairManTaskMgr;	// 掌门任务管理器

	// 发布任务
	GroupTempInfo					mGroupTempInfo;
	PUBLISHTASK						mCorpsPublishTask;  // 军团发布

	unsigned int					mTickEvents;		// 响应的全局唯一事件

	// 以下：实时数据 不用存
	int								mBossNpcEntityID;   // 玩家正在做NPC变BOSS任务 记录该NPC的实体ID
	int								mPlayerCurTalkNpcID;// 玩家当前的谈话NPC 临时存的 在会话完毕后立刻干掉
	int								mPlayerCurSendNpcID;// 玩家当前的护送NPC 实体ID
	int								mInvitingPlayerID;	// 被邀请的玩家实体ID
	time_t							mLastLogOffTime;	// 玩家上次下线/跨服时间点
	unsigned short					mRedirectFailTask;	// 跨地图失败的任务 （同时身上只允许有1个）
	int								mOwnerEntityID;	// 主人的实体ID
	int								mTodayTeamLoopNum; // 当天完成的组队环任务的个数

public:
	CTaskList( ) ;
	~CTaskList( ) ;
	// 初始化
	virtual int Initialize();
	// 恢复
	virtual int Resume(); 

private:
	ID_MAP_CONTAINER _IDmap;
	void RebuildIndex();
	inline int  GetTaskIndex( unsigned short vTaskID )
	{
		ID_MAP_CONTAINER::iterator it = _IDmap.find( vTaskID );
		if ( it != _IDmap.end() )
			return it->second;
		else
			return -1;
	}

public:
	void Clear( );

	// 得到任务状态
	int GetTaskState( unsigned short vTaskID );

	// 设置任务的状态
	int SetTaskState( unsigned short vTaskID, int State );

	// 判断任务是否在进度中
	bool IsTaskProgress( unsigned short vTaskID );

	// 判断任务是否结束，对于可重复任务，永远都是未结束
	bool IsTaskEnd( unsigned short vTaskID ){ return mCompleteTasks.find(vTaskID) != mCompleteTasks.end(); }

	// 清除任务完成标志
	void ClearTaskEndFlag( unsigned short vTaskID ){ mCompleteTasks.erase(vTaskID); }
	
	// 判断任务是否失败，对于可重试任务，永远都是未失败
	bool IsTaskFailed( unsigned short vTaskID )	{ return mFailedTasks.find(vTaskID) != mFailedTasks.end(); }

	// 判断任务是否完成，完成和结束是两个概念，完成代表完成了任务要求但还没有交，结束表示已经交了
	bool IsTaskComplete( unsigned short vTaskID );

	// 重置任务
	void TaskReset( );

	// 添加任务
	int InsertTask( const CTask& rTask );
	void SetGroupInfo(const GroupTempInfo &TempInfo)
	{
		mGroupTempInfo = TempInfo;
	}

	// 删除任务
	void RemoveTask( int vTaskID );

	// 得到任务数据
	CTask* GetTask( unsigned short vTaskID );

	void CreateFromTlv2( PBTaskList* pBuffer );
	void GetTlvBuffer2( PBTaskList* pBuffer );
	void SetEnd(unsigned short vTaskID);
	bool GetEnd(unsigned short vTaskID);

	void SetEndTime(unsigned short vTaskID, int vEndTime);
	int GetEndTime(unsigned short vTaskID);
	void SetFailed(unsigned short vTaskID);
	bool GetFailed(unsigned short vTaskID);
	int InsertRefreshedTask(unsigned short vTaskID , int taskColor );  // 插入一个新的刷新完的任务

	// 库任务相关外部接口
	void GetStorgeTaskList( NpcTaskList* pList, unsigned short usTaskID );
	void RefreshTasks( ETaskRefreshType etype );

	void SetBossNpcEntityID( int vEntityID ) { mBossNpcEntityID = vEntityID; }
	int  GetBossNpcEntityID( ) { return mBossNpcEntityID ; }

	void SetTalkNpcEntityID( int vEntityID ) { mPlayerCurTalkNpcID = vEntityID; }
	int  GetTalkNpcEntityID( ) { return mPlayerCurTalkNpcID ; }

	void SetSendNpcEntityID( int vEntityID ) { mPlayerCurSendNpcID = vEntityID; }
	int  GetSendNpcEntityID( ) { return mPlayerCurSendNpcID ; }

	void SetInvitingPlayerID( int nVal ) { mInvitingPlayerID = nVal; }
	int  GetInvitingPlayerID( ) { return mInvitingPlayerID; }

	void SetOwnerEntityID( int nVal ) { mOwnerEntityID = nVal; }
	int  GetOwnerEntityID( ) { return mOwnerEntityID; }

	int GetTaskColor(unsigned short vTaskID);
	void RemoveTaskColor(unsigned short vTaskID);

	// 每日事务 封装
	void OnNewdayCome();
	void OnNewWeekCome();

	int GetTodayTeamLoopNum() { return mTodayTeamLoopNum; }
	void ZeroTodayTeamLoopNum() { mTodayTeamLoopNum=0; }
	int PlusTodayTeamLoopNum() { return ++mTodayTeamLoopNum; }

	void InsertPublishTask(unsigned short vTaskID, unsigned int vTaskType, unsigned int vTaskState);

	unsigned short GetPublishStateByType(unsigned short vTaskID, unsigned int vTaskType);
	void SetPublishTaskState(unsigned short vTaskID, unsigned short vTaskState,unsigned int vTaskType);

};

// 单个参与护送的玩家的护送数据
struct st_singlesendplayer_data
{
	int mPlayerID;
	int mLeaveTime; // 离开NPC的时间 一旦回到他身边 则置0
	st_singlesendplayer_data()
	{
		if ( CObj::msCreateMode )
		{
			mPlayerID = 0;
			mLeaveTime = 0;
		}
	}
	st_singlesendplayer_data( int nPlayerID, int nLeaveTime )
	{
		mPlayerID = nPlayerID;
		mLeaveTime = nLeaveTime;
	}

	bool operator < ( const st_singlesendplayer_data& other ) const
	{
		return mPlayerID < other.mPlayerID;
	}
};

// ***********************************************************
//  类型描述：	单个玩家的护送任务数据结构
// **********************************************************
class CSingleSendNpc
{
private:
	int mX[ MAX_WAY_POINTS ];
	int mY[ MAX_WAY_POINTS ];
	int mMapID[ MAX_WAY_POINTS ];
	int mCurIndex;
	int mTotalIndex; // 数组最大下标+1，[mTotalIndex-1]是终点
	int mMaxGird; // 最大格子数
	int mMaxLeaveTime; // 最大允许 离开NPC的时间
	int mBuffTime; // 每隔多久上一次BUFF
	int mBuffID;   // 要上的BUFF ID
	int mBuffTimer;  // 上BUFF计时器
	int mWaitTimer;	// 等待定时器
public:
	unsigned int   mNpcEntityID;
	unsigned int   mPlayerEntityID;  // 发起护送的人的ID
	unsigned short mTaskID;

	// 参与护送的人的列表
	CSearchArray<st_singlesendplayer_data, MAX_FAMILYMEMBER_NUM>	mSendingList;	// 所有当前参与护送的人的列表

public:
	CSingleSendNpc( int* vpX, int* vpY, int* vpMapID, int vTotal, unsigned int vNpcEntityID, unsigned int vPlayerEntityID
		, unsigned short vTaskID, int vGird, int vLeaveTime, int nBuffTime, int nBuffID, int nInitWait );
	CSingleSendNpc();

	int  OnNpcCtrlTimer( ); // NPC控制定时器
	int  OnPlayerCtrlTimer( std::vector<int>& rFailedPlayers ); // 玩家控制定时器
	void Start( );
	CWTPoint GetCurTargetPos();
	int  AddSendingList( int nEntityID ) { return mSendingList.insert( st_singlesendplayer_data( nEntityID, 0 ) ); }
	int  DelSendingList( int nEntityID ) { return mSendingList.remove( st_singlesendplayer_data( nEntityID, 0 ) ); }
	void GetPlayerIDs( std::vector<int>& rEntitys );
};

// ***********************************************************
//  类型描述：	 护送任务管理器 监控护送任务的整个过程，失败的话直接给玩家发消息提示，然后干掉该任务。成功的话fire event
//  主要工作：
//  1,创建护送任务实例，记录玩家实体ID和NPC的所有路点
//  2,处理异常（玩家下线，NPC死亡等。。。）
// **********************************************************
typedef lk::hash_map<int, CSingleSendNpc, SERVER_CAP_PLAYER> SendNpcType;

class CSendNpcManager
{
private:
	SendNpcType mSessions;
	int mTimer;

public:
	CSendNpcManager()
	{
		if (CObj::msCreateMode)
		{
			mSessions.initailize();
			mTimer = 0;
		}

	}

	int  StartNewSession( CEntityPlayer* pPlayer, int vNpcTempID, int* vX, int* vY, int* vMapID, int vTotal, unsigned short vTaskID, int vGird, int nNpcEntityID = 0, int vLeaveTime = 0, int nBuffTime = 0, int nBuffID = 0, int nWaitTimer = 0 );
	void OnTimer( int vOffSet );
	void ClearSession( int nNpcEntityID );
	void ClearPlayerSendSession( CEntityPlayer* pPlayer, bool bIsFailed );
	bool GetNpcCurTarget( int nEntityID, CWTPoint& rPoint );
	int  OnResetSendNpc( int nEntityID );
	int  OnSendTaskFailed( std::vector<int>& rEntityIDs );
	void CheckSendingSession( int nNpcEntityID );
};



// ***********************************************************
//  类型描述：	任务邀请管理器
// **********************************************************
class CTaskInviteManager
{
public:

	// 返回是否注册成功
	int RegInvite( unsigned int nEntityID, const char* szName, unsigned short unTaskID );

	// 检查是否已注册过，并清除掉记录
	bool ChkReg ( int nEntityID, const char* szName, unsigned short unTaskID );
	CTaskInviteManager()
	{
		if( CObj::msCreateMode ) 
			mData.initailize();
	}

private:
	TASK_INVITE_CONTAINER mData;

};


// ***********************************************************
//  类型描述：	处理打怪任务的掉落
// **********************************************************
class CTaskDropItemMgr
{
private:
	// 掉落数据

	DROP_DATA_CONTAINER mDropData[MAX_TASK_NUM];

public:
	void RegDrop( unsigned short usTaskID, int nOgreID, int nItemID, int n, int nP, int nMaxn );
	bool GetDropItems( unsigned short usTaskID, int nOgreID, int& rnItemID, int& rn, int& rnmaxn ); 
	void ClearDrop();
};

// NPC监控器 监控少量NPC的状态 以便于完成一些全局的事件  比如一群人任务失败等
// 通过注册来管理一些群体行为 不关心外围的应用逻辑 只是在指定的时候做指定的事
// 目前只支持1次操作 操作完就清除队列
// 基本功能：维护全局唯一NPC的模板ID和实体ID的对应
struct _PlayerAction
{
	enTaskActionType nActionType;
	int				 nPlayerEntityID;
	unsigned short	 usTaskID;

	bool operator < ( const struct _PlayerAction& other )  const
	{
		if ( nPlayerEntityID == other.nPlayerEntityID )
			return usTaskID < other.usTaskID;
		return nPlayerEntityID < other.nPlayerEntityID;
	}
};

class CNpcState
{
public:
	int				nNpcEntityID;	// 该NPC的实体ID

	// 触发数据
	CSearchArray<_PlayerAction, SERVER_CAP_PLAYER>	mActions[MAX_ACTION_TYPES];

	CNpcState()
	{
		if (CObj::msCreateMode)
		{
			nNpcEntityID = INVALID_OBJ_ID;
			memset( mActions, 0, sizeof(mActions));
		}
	}
};

typedef lk::hash_map< int, CNpcState, NPC_MONITOR_SIZE> NPC_MONITOR_TYPE;
class CTaskNpcMonitor
{
private:
	NPC_MONITOR_TYPE	m_NpcMap;	// NPC模板ID和实体ID映射关系 如果second是0 说明未创建或已销毁

public:
	CTaskNpcMonitor()
	{
		if (CObj::msCreateMode)
		{
			m_NpcMap.initailize();
		}
	}

	int RegWatchNpc( int nNpcTempID );
	int OnNpcCreated( int nNpcTempID, unsigned int nNpcEntityID );
	int OnNpcDestroyed( int nNpcTempID );
	unsigned int GetNpcEntityID( int nNpcTempID );

	int OnNpcDied( int nNpcTempID );
	int RegGlobalTaskAction( int nTempID,enTaskActionType etype, CEntityPlayer* pPlayer, unsigned short vTaskID );
	int RemoveGlobalTaskAction(int nTempID, CEntityPlayer* pPlayer, unsigned short vTaskID);
};
