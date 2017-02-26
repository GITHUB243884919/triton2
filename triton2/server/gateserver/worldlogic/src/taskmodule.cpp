#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tinyxml.h>
#include "gatelogic.h"
#include "taskmodule.h"
#include "gateobj_manager.h"
#include "entity.h"
#include "tasktype.h"
#include "servermessage_in.h"
#include "core_service.h"
#include "gateteam.h"
#include "gateteammanager.h"
#include "corpservice.h"
#include "taskmessage_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"

template<> CTaskModule* CSingleton< CTaskModule >::spSingleton = NULL;

void CSinglePublishInfo::Initailize()
{
	mTodayPublishTaskNum = 0;	
	mOncePublishTaskNum  = 0;	
	mTotalFinishNum		 = 0;	
	mEntityID			 = 0;
	for ( int i =0; i < COLOR_TOTAL; ++i )
	{		
		mSingleColorInfo[ i ].PubNum		= 0;
		mSingleColorInfo[ i ].RecvAwards	= 0;	
	}
}

void CGroupPublishInfo::Initailize()
{
	mTodayPublishTimes = 0;
	mWeekPublishTimes = 0;			
	mPublishList.initailize();
}

// 得到发布任务状态
unsigned short CGroupPublishInfo::GetPublishStateByTask(unsigned short vTaskID)
{
	for( PUBLISHLIST::iterator it = mPublishList.begin(); it != mPublishList.end(); ++it )
	{
		if (it->mTaskID == vTaskID)
		{
			return it->mTaskState;
			break;
		}
	}
	return 1;
}

// 得到发布任务人员
unsigned int CGroupPublishInfo::GetPublishPlayerByTask(unsigned short vTaskID)
{
	for( PUBLISHLIST::iterator it = mPublishList.begin(); it != mPublishList.end(); ++it )
	{
		if (it->mTaskID == vTaskID)
		{
			return it->mEntityID;
			break;
		}
	}
	return 0;
}

// 设置发布状态
void CGroupPublishInfo::SetPublishTaskState(unsigned short vTaskID, unsigned short vState, unsigned int vEntityID)
{
	for( PUBLISHLIST::iterator it = mPublishList.begin(); it != mPublishList.end(); ++it )
	{
		if (it->mTaskID == vTaskID)
		{
			it->mTaskState = vState;
			it->mEntityID = vEntityID;
			return;
		}
	}
}

// 设置发布任务者的ID
void CGroupPublishInfo::SetPublishPlayerID(unsigned short vTaskID, unsigned int vEntityID)
{
	for( PUBLISHLIST::iterator it = mPublishList.begin(); it != mPublishList.end(); ++it )
	{
		if (it->mTaskID == vTaskID)
		{
			it->mEntityID = vEntityID;
			return;
		}
	}
}
// 新的一天
void CGroupPublishInfo::OnNewDayCome()
{ 
	mTodayPublishTimes = 0; 
	mPublishList.initailize();
}

// 新的一周

void CGroupPublishInfo::OnNewWeekCome()
{ 
	mWeekPublishTimes = 0; 
	mPublishList.initailize(); 
}

// 处理场景发来的消息
void CTaskModule::OnMessage(int vServerID, CMessage *pMessage)
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageHead* tpHead = pMessage->mutable_msghead();
	LK_ASSERT(tpHead != NULL, return);
	unsigned short nMessageID = (unsigned short) tpHead->messageid();

	switch(nMessageID)
	{
	case ID_S2G_REQUEST_PLAYERPBULISHTASK:
		{
			OnMessagePlayerPublishTask(vServerID, pMessage);
			break;
		}
	case ID_S2G_REQUEST_CHECKPUBLISHBOARD:
		{
			OnMessagePlayerCheckPublishBoard(vServerID, pMessage);
			break;
		}
	default:
		{
			break;
		}
	}
}

// 发布任务请求的回应
void CTaskModule::OnMessagePlayerPublishTask(int vServerID, CMessage *pMessage)
{
	LK_ASSERT(pMessage != NULL, return);
	CMessageGatePublishGroupRequest* pMsg = (CMessageGatePublishGroupRequest*) pMessage->msgpara();
	LK_ASSERT( pMessage != NULL && vServerID > 0 && vServerID <= SCENE_AT_GATE_CAP, return);

	int ret = CheckPublishLimit(pMsg->charid(), pMsg->taskid(), pMsg->paygroupmoney());

	CMessageGatePublishGroupResponse tResponse;

	tResponse.set_publishtype(CORPS_PUBLISH_TASK);
	tResponse.set_charid(pMsg->charid());
	tResponse.set_taskid(pMsg->taskid());
	tResponse.set_result(ret);

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2S_RESPONSE_PBULISHTASKRESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32)&tResponse);
	CGateServer::GetSingleton().Send2Scene(vServerID, &tMessage); // 发回去

}

// 检查发布任务限制条件   如天发布上限等
int CTaskModule::CheckPublishLimit(unsigned int role_id, unsigned short task_id, int vMoney)
{
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer(role_id);
	// 军团不存在
	if (tpCorps == NULL)
	{
		return ERROR_CORPS_NOTEXIST;
	}

	// 判断天发布次数是否已满
	int tTmies = tpCorps->GetCorpsPublishInfo()->GetTodayPublishTimes();

	if (tTmies >= mCorpsLimit.GetTodayMaxPublish())
	{
		return ERROR_TASK_GROUPPUBLISH_TODAYMAX;
	}
	// 判断周发布次数是否已满
	int wTime = tpCorps->GetCorpsPublishInfo()->GetWeekPublishTimes();

	if (wTime >= mCorpsLimit.GetWeekMaxPublish())
	{
		return ERROR_TASK_GROUPPUBLISH_WEEKMAX;
	}

	// 判断任务是否已经发布
	unsigned short vState = tpCorps->GetCorpsPublishInfo()->GetPublishStateByTask(task_id);

	// 任务已发布
	if (vState != UNPUBLISH_TASK)
	{
		return ERROR_TASK_HAS_PUBLISHED;
	}
	else
	{
		tpCorps->GetCorpsPublishInfo()->SetPublishTaskState(task_id, PUBLISHED_TASK, role_id);
		tpCorps->GetCorpsPublishInfo()->AddPublishCount();
		CCorpservice::GetSingleton().ConsumeCorpsMoney( role_id, vMoney, REASON_ACTIVIYCONSUME );
		CCorpservice::GetSingleton().SendNotifyCorpsPublishTask(tpCorps->GetCorpsID());
		tpCorps->SetSaveStatus( 1 );
		
		return SUCCESS;
	}

}
// 查看展板请求的回应
void CTaskModule::OnMessagePlayerCheckPublishBoard(int vServerID, CMessage *pMessage)
{
	LK_ASSERT(pMessage != NULL, return);
	CMessageGateCheckGroupPublishRequest* pMsg = (CMessageGateCheckGroupPublishRequest*) pMessage->msgpara();
	LK_ASSERT( pMessage != NULL && vServerID > 0 && vServerID <= SCENE_AT_GATE_CAP, return);

	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( pMsg->charid() );

	CMessageGateCheckGroupPublishResponse tResponse;
	// 没有军团
	if (tpCorps == NULL)
	{
		tResponse.set_result(ERROR_CORPS_NOTEXIST);
		return;
	}
	else
	{
		tResponse.set_result(SUCCESS);
	}

	PUBLISHLIST::iterator it = tpCorps->GetCorpsPublishInfo()->mPublishList.begin();
	for (; it != tpCorps->GetCorpsPublishInfo()->mPublishList.end(); ++it )
	{
		PBGroupTasks *mList = tResponse.add_tasks();
		mList->set_taskid(it->mTaskID);
		mList->set_taskstate(it->mTaskState);
	}
	// 查看展板信息的时候顺便把军团的信息发过来
	int mPost = 0;
	tpCorps->GetMemberPost(pMsg->charid(),mPost);
	PBGroupInfo *pInfo = tResponse.mutable_groupinfo();
	pInfo->set_groupid(tpCorps->GetCorpsID());
	pInfo->set_grouplevel(tpCorps->GetCorpsLevel());
	pInfo->set_groupmoney(tpCorps->GetCorpsMoney());
	pInfo->set_playerposition(mPost);

	tResponse.set_charid(pMsg->charid());
	tResponse.set_publishtype(pMsg->publishtype()); 

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2S_RESPONSE_CHECKPUBLISHBOARD, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32)&tResponse);
	CGateServer::GetSingleton().Send2Scene(vServerID, &tMessage);	// 发回去

}

void CTaskModule::OnNewDayCome()
{
	const char *rTaskRoot = "task/taskconfig.xml";

	OnReloadConfig(rTaskRoot);
}

// 读取XML配置文件  发布任务列表

void CTaskModule::OnReloadConfig(const char* pRoot)
{
	TiXmlDocument tDoc;
	mCorpsLimit.Initailize();

	const char *pFileName = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,pRoot);
	if ( !tDoc.LoadFile( pFileName ) )
	{			
		return ;
	}

	TiXmlElement *tpElement = tDoc.FirstChildElement("PublishConfig");
	if ( tpElement == NULL )
	{
		return ;
	}

	TiXmlElement *CorpsPublish = tpElement->FirstChildElement( "CorpsPublish" );
	if ( CorpsPublish == NULL )
	{
		return ;
	}
	TiXmlElement *TaskList = CorpsPublish->FirstChildElement( "TaskList" );
	if ( TaskList == NULL )
	{
		return ;
	}
	TiXmlElement *TaskInfo = TaskList->FirstChildElement( "TaskInfo" );
	if ( TaskInfo == NULL )
	{
		return ;
	}
	for ( ; TaskInfo != NULL; TaskInfo = TaskInfo->NextSiblingElement( ) )
	{
		unsigned short	vTaskID			= atoi( TaskInfo->Attribute( "TaskID" ) );
		unsigned short  vState			= atoi( TaskInfo->Attribute( "TaskState" ) );
		unsigned int	vEntityID		= atoi( TaskInfo->Attribute( "EntityID" ) );
		st_group_publish_task  _newTask(vTaskID,vState,vEntityID) ;
		mCorpsLimit.mTaskList.push_back(_newTask);
	}
	TaskInfo = TaskList->NextSiblingElement( "CorpsConfig" );

	if (TaskInfo == NULL)
	{
		return ;
	}
	
	TiXmlElement *LimitInfo = TaskInfo->FirstChildElement( "CorpsLimit" );

	if (LimitInfo == NULL)
	{
		return ;
	}
	mCorpsLimit.SetTodayMaxPublish( atoi( LimitInfo->Attribute( "MaxTodayPublish" ) )) ;
	mCorpsLimit.SetWeekMaxPublish( atoi( LimitInfo->Attribute( "MaxWeekPublish" ) )) ;

}