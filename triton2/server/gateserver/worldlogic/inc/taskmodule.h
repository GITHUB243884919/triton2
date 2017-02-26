#ifndef _TASK_GATE_H_
#define _TASK_GATE_H_

#include "module_manager.h"
#include "entity.h"
#include "lk_hashmap.h"
#include "lk_string.h"
#include "lk_vector.h"
#include "lk_queue.h"
#include "gateobj_define.h"
#include "gateobj_manager.h"
#include "servertool.h"
#include "tasktype.h"

using namespace GateServer;

// 个人发布任务     数据类型

class CSinglePublishInfo
{
private:
	unsigned int mTodayPublishTaskNum;   //  天计次
	unsigned int mOncePublishTaskNum;	 //  一次发布任务个数
	unsigned int mTotalFinishNum;		 //  完成发布任务数
	unsigned int mEntityID;				 //  玩家ID

	// 单个发布类型的数据  按任务颜色区分
	struct st_single_color                
	{	
		unsigned short PubNum;
		unsigned short RecvAwards;
		st_single_color()
		{		
			if (CObj::msCreateMode)		
			{			
				PubNum		= 0;			
				RecvAwards	= 0;		
			}	
		}
	};

public:
	st_single_color mSingleColorInfo[ COLOR_TOTAL ];  // 不同颜色的任务数据

	CSinglePublishInfo()
	{
		if(CObj::msCreateMode)
		{
			Initailize();
		}
	};

	void Initailize();

	unsigned int GetTodayPublishNumber(){ return mTodayPublishTaskNum; }	
	unsigned int GetOncePublishNumber(){ return mOncePublishTaskNum; }	
	unsigned int GetFinishNumber(){ return mTotalFinishNum; }	
	unsigned int GetEntityID(){return mEntityID; }


	void SetTodayPublishNumber(int num){ mTodayPublishTaskNum = num; }	
	void SetOncePublishNumber(int num){ mOncePublishTaskNum = num; }	
	void SetFinishNumber(int num) { mTotalFinishNum = num ; }	
	void SetEntityID(int mID){ mEntityID = mID; }

};


// 团体发布任务基础数据

struct st_group_publish_task
{
	unsigned short	mTaskID;					
	unsigned short	mTaskState;				    // 全局只记录发布和未发布状态。
	unsigned int	mEntityID;					// 发布者ID	
	st_group_publish_task()
	{
		if (CObj::msCreateMode)		
		{			
			mTaskID		= 0;			
			mTaskState  = 1;
			mEntityID	= 0;
		}	
	}
	st_group_publish_task(unsigned short vTaskID, unsigned short vTaskState, unsigned int vEntityID)
	{
		mTaskID		= vTaskID;			
		mTaskState  = vTaskState;
		mEntityID	= vEntityID;
	}
};

typedef lk::vector<st_group_publish_task, PUBLISH_LIST_SIZE > PUBLISHLIST;


// 团体发布管理器

class CGroupPublishInfo
{
private:
	int mTodayPublishTimes;					// 天发布次数
	int mWeekPublishTimes;					// 周发布次数

public:
	PUBLISHLIST mPublishList;				// 任务列表

	CGroupPublishInfo()
	{
		if(CObj::msCreateMode)
		{
			Initailize();
		}	
	}

	void Initailize();
	void OnNewDayCome();
	void OnNewWeekCome();
	
	int GetTodayPublishTimes(){ return mTodayPublishTimes; }
	int GetWeekPublishTimes(){ return mWeekPublishTimes; }	

	void SetTodayPublishTimes( int nTimes ){ mTodayPublishTimes = nTimes; }
	void SetWeekPublishTimes(int nTimes){ mWeekPublishTimes = nTimes; }

	
	void AddPublishCount(){ ++mTodayPublishTimes; ++mWeekPublishTimes;}
	void CleanPublishCout(){mTodayPublishTimes = 0; mWeekPublishTimes = 0;}

	unsigned short GetPublishStateByTask(unsigned short vTaskID);  //  得到任务的发布状态
	unsigned int GetPublishPlayerByTask(unsigned short vTaskID);   //  得到任务发布人

	void SetPublishTaskState(unsigned short vTaskID, unsigned short vState, unsigned int vEntityID);
	void SetPublishPlayerID(unsigned short vTaskID, unsigned int vEntityID);

	void LoadPublishList( PUBLISHLIST *TempList)
	{
		mPublishList.initailize();
		PUBLISHLIST::iterator it = TempList->begin();
		for ( ;it != TempList->end(); ++it )
		{
			mPublishList.push_back(*it);
		}
	}
	
};

// 任务模块存储的临时数据  包括发布上限
struct GroupLimitInfo 
{
	int TodayMaxPublish;					// 每天发布上限
	int WeekMaxPublish;						// 每周发布上限
	
	PUBLISHLIST mTaskList;					// 发布列表

	void Initailize()
	{
		TodayMaxPublish		= 0;
		WeekMaxPublish		= 0;
		mTaskList.initailize();
	}
	int GetTodayMaxPublish(){return TodayMaxPublish; }
	int GetWeekMaxPublish(){return WeekMaxPublish; }

	void SetTodayMaxPublish(int nMax){ TodayMaxPublish = nMax; }
	void SetWeekMaxPublish(int nMax){WeekMaxPublish = nMax; }
};

//  任务模块

class CTaskModule : public CDataModule, public CSingleton< CTaskModule >
{
public:
	GroupLimitInfo mCorpsLimit;
	CTaskModule(){ mCorpsLimit.Initailize(); }
	// 加载服务器信息
	virtual void OnLaunchServer() { }
	virtual void OnExitServer() { }
	virtual void OnTimer(unsigned int, unsigned int) { }
	virtual void OnMessage(int vServerID, CMessage *pMessage);
	virtual void OnNewDayCome();

	virtual bool IsLaunched() { return true; }

	static int CountSize( ) { return sizeof( CTaskModule ); }

	void OnReloadConfig(const char* pRoot);

public:
	
	// 获得发布任务列表
	GroupLimitInfo *GetCorpsLimit(){ return &mCorpsLimit; }

	int CheckPublishLimit(unsigned int role_id, unsigned short task_id, int vMoney);		//	检查发布条件
	// 消息部分
	void OnMessagePlayerCheckPublishBoard( int vServerID, CMessage *pMessage );     //   玩家查看展板请求
	
	void OnMessagePlayerPublishTask(int vServerID, CMessage *pMessage);             // 玩家发布任务请求
};

#endif
