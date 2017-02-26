#include "stdafx.hpp"
#include "taskmodule.h"
#include "promessage_pb.hxx.pb.h"
#include "scenelogic.h"
#include "property.h"
#include "mapobjmanager.h"
#include "taskmessage_pb.hxx.pb.h"
#include "mapmodule.h"
#include "teammodule.h"
#include "teamtype.h"
#include "propertymodule.h"
#include "entity_searcher.h"
#include "message_factory.h"
#include "logservertype.h"
#include "message_maker.h"
#include "sceneobj_manager_imp.h"
#include "logmodule.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif


template<> CTaskModule* CSingleton< CTaskModule >::spSingleton = NULL;
template<> CTaskLuaInterface* CSingleton< CTaskLuaInterface>::spSingleton = NULL;
CPropertyPlayer* CTaskLuaInterface::mpProperty = NULL;
CTaskList* CTaskLuaInterface:: mpTaskList = NULL;
CEntityPlayer* CTaskLuaInterface::mpPlayer = NULL;
unsigned short CTaskLuaInterface::mCurTaskID = 0;
std::map<int, int>* CTaskLuaInterface::mpTmpChecker = NULL;

char	CTaskModule::sModuleName[ NAME_LENGTH ] = "Task";


// --------------------------------------------------------------------------------------
unsigned int CTaskModule::CountSize()
{
	unsigned int tSize = sizeof( CTaskModule );
	return tSize;
}

// 重新加载配置文件
void CTaskModule::OnLoadConfig()
{
	TASK_LOG* tpTask = CSceneCfgManager::GetSingletonPtr()->GetTask_log();
	if( tpTask != NULL )
	{
		INIT_ROLLINGFILE_LOG( tpTask->mIdentifier, tpTask->mDirectory, (LogLevel)tpTask->mPriority, tpTask->mMaxfilesize, tpTask->mMaxbackupindex );
	}
	TASKLUA_LOG* tpLua = CSceneCfgManager::GetSingletonPtr()->GetTasklua_log();
	if( tpLua != NULL )
	{
		INIT_ROLLINGFILE_LOG( tpLua->mIdentifier, tpLua->mDirectory, (LogLevel)tpLua->mPriority, tpLua->mMaxfilesize, tpLua->mMaxbackupindex );
	}

	bool bRet = false;
	PERF_FUNC( "RegisterLua", bRet = RegisterLua() );

	if(!bRet)
	{
		LOG_ERROR( "TASK", "reg Lua module failed，please check Lua load log");	 
	}

	// 注册加载lua文件的回调函数
	const char* pcDirPath = CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data");
	CSceneServer::GetSingleton().RegisterLoadConfigCallback(pcDirPath, ".lua", CTaskModule::LoadLuaFileCallback);
	
	pcDirPath = CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/tasks");
	CSceneServer::GetSingleton().RegisterLoadConfigCallback(pcDirPath, ".lua", CTaskModule::LoadLuaFileCallback);
}

// 新一天到来
void CTaskModule::OnNewDayCome(  )
{	
	CSceneObjManagerImp* pSceneneObjManagerImp = CSceneObjManager::GetSingletonPtr()->mSceneObjManagerImp;
	for( CSceneObjManagerImp::ObjMng_Task::iterator tTaskIt = pSceneneObjManagerImp->mTaskList.begin( ); 
		tTaskIt != pSceneneObjManagerImp->mTaskList.end( ); 
		++tTaskIt 
		)
	{
		// 处理任务组
		CTaskList* pList = (CTaskList*) (&(*tTaskIt));
		if (OnTaskPlayerNewDayCome(pList, true)) // 通知客户端
		{
			SendTaskMsg2Client( pList );
		}
	}

}

// 新一周到来
void CTaskModule::OnNewWeekCome(  )
{
	CSceneObjManagerImp* pSceneneObjManagerImp = CSceneObjManager::GetSingletonPtr()->mSceneObjManagerImp;
	for( CSceneObjManagerImp::ObjMng_Task::iterator tTaskIt = pSceneneObjManagerImp->mTaskList.begin( ); 
		tTaskIt != pSceneneObjManagerImp->mTaskList.end( ); 
		++tTaskIt 
		)
	{
		// 处理任务组
		CTaskList* pList = (CTaskList*) (&(*tTaskIt));
		pList->OnNewWeekCome();
	}
}


// 重载lua内存分配函数
//static void *lk_alloc_for_lua (void *ud, void *ptr, size_t osize, size_t nsize) {
//	(void)ud;
//	(void)osize;
//	if (nsize == 0) {
//		delete [] (char*)(ptr);
//		return NULL;
//	}
//	else
//	{
//		if( osize == 0 )
//		{
//			return ((void*)(debug_new char[nsize]));
//		}
//		else
//		{
//			if( nsize > osize )
//			{
//				void* nptr = ((void*)(debug_new char[nsize]));
//				memcpy( nptr, ptr, osize );
//				delete [] (char*)(ptr);
//				return nptr;
//			}
//			else
//			{
//				return ptr;
//			}
//		}
//	}
//}


bool CTaskModule::RegisterLua()
{
	if(L) lua_close(L); L = NULL;
	L = lua_open( );
	if (L == NULL)
	{
		printf("open LUA state failed");
		exit(0);
	}
	luaL_openlibs( L );

	// 先清理 防止热启动之前的数据还在
	CStorageTaskManager::m_Storage.clear();
	mTaskDropMgr.ClearDrop();

	// 注册CProperty
	// lua_tinker::class_add<CTaskLuaInterface>(L, "Task");
	lua_tinker::def(L, "PlayerHasItem",       &CTaskLuaInterface::HasItem);
	lua_tinker::def(L, "PlayerAwardMoney",    &CTaskLuaInterface::AwardBDMoney);
	lua_tinker::def(L, "AwardCommonMoney",    &CTaskLuaInterface::AwardCommonMoney);
	lua_tinker::def(L, "PlayerAwardExpr",     &CTaskLuaInterface::AwardExpr);

	//lua_tinker::def(L, "AwardExprInLoop",     &CTaskLuaInterface::AwardExprInLoop);
	//lua_tinker::def(L, "AwardMoneyInLoop",    &CTaskLuaInterface::AwardMoneyInLoop);

	lua_tinker::def(L, "PlayerAwardItem",			  &CTaskLuaInterface::AwardItem);
	lua_tinker::def(L, "PlayerAwardFreeItem",		  &CTaskLuaInterface::PlayerAwardFreeItem);
	lua_tinker::def(L, "PlayerRemoveItem",			  &CTaskLuaInterface::RemoveItem);
	lua_tinker::def(L, "AwardLevelEqt",				  &CTaskLuaInterface::AwardLevelEqt);
	lua_tinker::def(L, "PlayerAwardItemByMetier",     &CTaskLuaInterface::AwardItemByMetier);
	lua_tinker::def(L, "PlayerAwardTask",     &CTaskLuaInterface::AwardTask);
	lua_tinker::def(L, "PlayerAwardTeleport", &CTaskLuaInterface::AwardTeleport);
	lua_tinker::def(L, "AwardChangeMetier",   &CTaskLuaInterface::AwardChangeMetier);

	lua_tinker::def(L, "CheckCollectItem",	  &CTaskLuaInterface::CheckCollectItem);
	lua_tinker::def(L, "InitStaticStorageTask",	  &CTaskLuaInterface::InitStaticStorageTask);
	lua_tinker::def(L, "LoadTaskComplete",	  &CTaskLuaInterface::LoadTaskComplete);	
	lua_tinker::def(L, "RegDropTable",		  &CTaskLuaInterface::RegDropTable);
	lua_tinker::def(L, "RegLevelTask",		  &CTaskLuaInterface::RegLevelTask);
	lua_tinker::def(L, "CheckEvent",		  &CTaskLuaInterface::CheckEvent);
	lua_tinker::def(L, "ClearEvent",		  &CTaskLuaInterface::ClearEvent);
	
	lua_tinker::def(L, "IsSpaceOK",			&CTaskLuaInterface::IsSpaceOK);
	lua_tinker::def(L, "GetTaskState",      &CTaskLuaInterface::GetTaskState);
	lua_tinker::def(L, "SetTaskState",      &CTaskLuaInterface::SetTaskState);
	lua_tinker::def(L, "GetPlayer",			&CTaskLuaInterface::GetPlayer);
	//lua_tinker::def(L, "GetPlayerRegion",   &CTaskLuaInterface::GetPlayerRegion);
	lua_tinker::def(L, "GetTask",			&CTaskLuaInterface::GetTask);

	lua_tinker::def(L, "GetNextSubTaskID",				&CTaskLuaInterface::GetNextSubTaskID);
	lua_tinker::def(L, "GetCurSubTaskID",				&CTaskLuaInterface::GetCurSubTaskID);
	lua_tinker::def(L, "PlayerGetLoopTask",				&CTaskLuaInterface::PlayerGetLoopTask);
	lua_tinker::def(L, "GetLastLoopTaskID",				&CTaskLuaInterface::GetLastLoopTaskID);
	lua_tinker::def(L, "InsertLoopTask",				&CTaskLuaInterface::InsertLoopTask);
	lua_tinker::def(L, "GetTodayCompletedTimes",		&CTaskLuaInterface::GetTodayCompletedTimes);
	lua_tinker::def(L, "SetAbortLoopTaskData",			&CTaskLuaInterface::SetAbortLoopTaskData);
	lua_tinker::def(L, "GetCurSubTaskIndex",			&CTaskLuaInterface::GetCurSubTaskIndex);
	
	lua_tinker::def(L, "IsTaskEnd",						&CTaskLuaInterface::IsTaskEnd);
	
	lua_tinker::def(L, "Debug",							&CTaskLuaInterface::Debug);
	lua_tinker::def(L, "Error",							&CTaskLuaInterface::Error);
	lua_tinker::def(L, "CheckStorageTaskInList",		&CTaskLuaInterface::CheckStorageTaskInList);
	lua_tinker::def(L, "RemoveStorageTask",				&CTaskLuaInterface::RemoveStorageTask);

	lua_tinker::def(L, "AwardBuff",						&CTaskLuaInterface::AwardBuff);
	lua_tinker::def(L, "SetEvent",						&CTaskLuaInterface::SetEvent);
	lua_tinker::def(L, "IsTragedyTaskAcceptable",		&CTaskLuaInterface::IsTragedyTaskAcceptable);
	lua_tinker::def(L, "ChangeCurNpc2Ogre",				&CTaskLuaInterface::ChangeCurNpc2Ogre);
	lua_tinker::def(L, "CheckNpcOwner",					&CTaskLuaInterface::CheckNpcOwner);
	lua_tinker::def(L, "AwardBox",						&CTaskLuaInterface::AwardBox);
	lua_tinker::def(L, "DestroyNpc",					&CTaskLuaInterface::DestroyNpc);
	lua_tinker::def(L, "StartSendNpc",					&CTaskLuaInterface::StartSendNpc);
	lua_tinker::def(L, "CreateNpc",						&CTaskLuaInterface::CreateNpc);
	lua_tinker::def(L, "GetTodayTimesByGroupID",		&CTaskLuaInterface::GetTodayTimesByGroupID);
	lua_tinker::def(L, "GetWeekTimesByGroupID",			&CTaskLuaInterface::GetWeekTimesByGroupID);
	lua_tinker::def(L, "CommonCheck",					&CTaskLuaInterface::CommonCheck);
	lua_tinker::def(L, "CommonAward",					&CTaskLuaInterface::CommonAward);
	lua_tinker::def(L, "InviteFamilyMembers",			&CTaskLuaInterface::InviteFamilyMembers);
	lua_tinker::def(L, "GetFamilyAvgLevel",				&CTaskLuaInterface::GetFamilyAvgLevel);
	lua_tinker::def(L, "StartTimeLimitTaskTimer",		&CTaskLuaInterface::StartTimeLimitTaskTimer);
	lua_tinker::def(L, "CheckMutexByTask",				&CTaskLuaInterface::CheckMutexByTask);
	lua_tinker::def(L, "SetRedirectFailTask",			&CTaskLuaInterface::SetRedirectFailTask);
	lua_tinker::def(L, "RegMonitorNpc",					&CTaskLuaInterface::RegMonitorNpc);
	lua_tinker::def(L, "GetWatchNpcState",				&CTaskLuaInterface::GetWatchNpcState);
	lua_tinker::def(L, "SendSysMail",					&CTaskLuaInterface::SendSysMail);
	lua_tinker::def(L, "RegProtectFlagTask",			&CTaskLuaInterface::RegProtectFlagTask);
	lua_tinker::def(L, "CommonGetData",					&CTaskLuaInterface::CommonGetData);
	lua_tinker::def(L, "GetCurCampID",					&CTaskLuaInterface::GetCurCampID);
	lua_tinker::def(L, "GetTodayAcceptTimesByGroupID",	&CTaskLuaInterface::GetTodayAcceptTimesByGroupID);
	lua_tinker::def(L, "GetWeakCampLevel",				&CTaskLuaInterface::GetWeakCampLevel);
	lua_tinker::def(L, "PublishCheck",					&CTaskLuaInterface::PublishCheck);


	lua_tinker::class_add<CTask>(L, "CTask");
	lua_tinker::class_mem<CTask>(L, "mTaskID",			&CTask::mTaskID);
	lua_tinker::class_mem<CTask>(L, "mState",			&CTask::mTaskProgress);
	lua_tinker::class_def<CTask>(L, "SetTaskData",		&CTask::SetTaskData);
	lua_tinker::class_def<CTask>(L, "GetTaskData",      &CTask::GetTaskData);
	lua_tinker::class_def<CTask>(L, "RegEventTick",     &CTask::RegEventTick);

	lua_tinker::class_add<region>(L, "region");
	lua_tinker::class_mem<region>(L, "mMapID",   &region::mMapID);
	lua_tinker::class_mem<region>(L, "mX",       &region::mX);
	lua_tinker::class_mem<region>(L, "mY",       &region::mY);


	lua_tinker::class_add<CDir>(L, "CDir");
	lua_tinker::class_mem<CDir>(L, "bad",   &CDir::bad);
	lua_tinker::class_con<CDir>(L, lua_tinker::constructor<CDir, const char*>);
	lua_tinker::class_def<CDir>(L, "file", &CDir::file);

	// 编译interface文件
	/*lua_tinker::dofile(L, "../config/data/task_data/task_comm.lua");
	lua_tinker::dofile(L, "../config/data/task_data/awards.lua");	
	lua_tinker::dofile(L, "../config/data/task_data/country_build.lua");	
	lua_tinker::dofile(L, "../config/data/task_data/task_global.lua");
	lua_tinker::dofile(L, "../config/data/task_data/loop_awards.lua");
	lua_tinker::dofile(L, "../config/data/task_data/tragedy_tasks.lua");
	lua_tinker::dofile(L, "../config/data/task_data/task_func.lua");
	lua_tinker::dofile(L, "../config/data/task_data/task_interface.lua");
	lua_tinker::dofile(L, "../config/data/task_data/server_base_data.lua");
	lua_tinker::dofile(L, "../config/data/task_data/extra_awards.lua");*/
	lua_tinker::dofile(L, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/task_comm.lua"));
	lua_tinker::dofile(L, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/awards.lua"));	
	lua_tinker::dofile(L, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/country_build.lua"));	
	lua_tinker::dofile(L, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/task_global.lua"));
	lua_tinker::dofile(L, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/loop_awards.lua"));
	lua_tinker::dofile(L, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/tragedy_tasks.lua"));
	lua_tinker::dofile(L, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/task_func.lua"));
	lua_tinker::dofile(L, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/task_interface.lua"));
	lua_tinker::dofile(L, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/server_base_data.lua"));
	lua_tinker::dofile(L, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/extra_awards.lua"));
	lua_tinker::dofile(L, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/refresh_awards.lua"));
	lua_tinker::dofile(L, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"task_data/task_refresh_func.lua"));
	

	//// 得到NPC绑定任务列表并存储，以提高效率
	//mNpcTaskList

	// 调用初始化接口
	if (! CTaskLuaInterface::Call<bool>(NULL, NULL, "init_tasks") )
	{
		printf("call init_tasks failed");
		exit(0);
	}
	return true;
}

//  启动服务器
void CTaskModule::OnLaunchServer( )
{
	OnLoadConfig();
}

//  退出服务器
void CTaskModule::OnExitServer( )
{
	if(L) lua_close(L); L = NULL;

	// 清除std::map对象中数据
	mTaskDropMgr.ClearDrop();
}

//  服务器Timer
void CTaskModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	mSendNpcMgr.OnTimer( vTickOffset );

	mRefreshStorageTaskTimer += vTickOffset;
	if ( mRefreshStorageTaskTimer >= (int)CSceneCfgManager::GetSingletonPtr()->GetTask_cfg()->mRefreshstoragetime*1000 )
	{
		mRefreshStorageTaskTimer = 0;
		CStorageTaskManager::RefreshStorageOnTime();
	}

}

//  建立场景
int CTaskModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )
{
	return SUCCESS;

}




// 在收到其他服务器发来的邀请任务的路由信息 。。。 
void CTaskModule::OnTaskInvitedRoutMsg( const char* szSrcName, const char* szDesName , unsigned short unTaskID )
{
	CEntityPlayer* pDesPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton( ).GetPlayerByName(szDesName);
	LK_ASSERT( pDesPlayer != NULL, return );

	SendInviteTaskMsg( szSrcName, pDesPlayer, unTaskID ) ;
}



// 处理玩家对任务邀请的回应
void CTaskModule::OnPlayerReplyTaskInvite( CEntityPlayer* pPlayer, const char* szRequesterName, unsigned short usTaskID, unsigned short usAgreeCode )
{
	LK_ASSERT( pPlayer != NULL, return );
	if ( !mTaskInviteInfMgr.ChkReg( pPlayer->GetEntityID(), szRequesterName , usTaskID ))
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_INVITE_TIMEOUT );
		return;
	}

	CMessagePlayerInviteTaskResponseNotice msg; // 发到邀请者玩家
	CMessage tMessage;
	msg.set_srcplayername( pPlayer->GetCharName() );
	msg.set_taskid( usTaskID );
	msg.set_agreeflag( usAgreeCode );

	msg.mutable_router()->set_srcname( pPlayer->GetCharName() );
	msg.mutable_router()->set_desname( szRequesterName );
	msg.mutable_router()->set_send2player( 0 );
	msg.mutable_router()->set_routertype( CMessageRouter::ROUTER_NAME );
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid( ID_S2C_INVITE_TASK_RESPONSE_NOTICE );

	CSceneLogic::GetSingleton( ).Send2Player( szRequesterName, &tMessage );

	if ( usAgreeCode == 1 ) // 如果他同意了， 走接任务逻辑
	{
		// 先获得邀请他的实体ID
		int nRequesterID = 0;
		CEntityPlayer* tpSrcPlayer = CSceneLogic::GetSingleton().GetPlayerByName( szRequesterName );
		if ( tpSrcPlayer != NULL )
			nRequesterID = tpSrcPlayer->GetEntityID();
		if (!PlayerObtainTask( pPlayer, usTaskID, true, 0, nRequesterID ) ) // 如果不在同一服务器 则邀请者ID是0
		{
			Send2ClientOprFailed(pPlayer);
		}else
		{
			LogEventPlayerAcceptTask(pPlayer, usTaskID);
		}
	}
}


// 函数功能：发送给被邀请的玩家 任务邀请信息 同时记录这条邀请(在被邀请人的服务器)
int CTaskModule::SendInviteTaskMsg( const char* pSrcPlayerName, CEntityPlayer* pDesPlayer, unsigned short unTaskID, int nInvitingPlayerID /*= -1*/ )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pDesPlayer->GetTaskToken( ));
	LK_ASSERT( pList, return 0 );

	int task_state = pList->GetTaskState( unTaskID );

	if ( task_state == TASK_BEGIN || task_state == TASK_FAILED || task_state == TASK_TOEND )
	{
		// 返回  对方身上已有这个任务  给客户端发消息
		CMessagePlayerInviteTaskErrorMsg msg; 

		CMessage tMessage;

		msg.set_taskid( unTaskID );
		msg.set_myname( pDesPlayer->GetCharName() );
		msg.set_typeid_( ERROR_GOT );

		msg.mutable_router()->set_srcname( pDesPlayer->GetCharName() );
		msg.mutable_router()->set_desname( pSrcPlayerName );
		msg.mutable_router()->set_send2player( 1 ); // 发给客户端！(邀请者)
		msg.mutable_router()->set_routertype( CMessageRouter::ROUTER_NAME );

		tMessage.set_msgpara((uint32_t) &msg);

		tMessage.mutable_msghead()->set_messageid( ID_S2C_PLAYER_INVITE_TASK_ERROR_MSG );

		CSceneLogic::GetSingleton( ).Send2Player( pSrcPlayerName, &tMessage );
		return -1;
	}
	else if( task_state == TASK_COMPLETE || task_state == TASK_END )
	{
		// 返回  对方已经完成这个任务  给客户端发消息
		CMessagePlayerInviteTaskErrorMsg msg; 

		CMessage tMessage;

		msg.set_taskid( unTaskID );
		msg.set_myname( pDesPlayer->GetCharName() );
		msg.set_typeid_( ERROR_COMPLETED );

		msg.mutable_router()->set_srcname( pDesPlayer->GetCharName() );
		msg.mutable_router()->set_desname( pSrcPlayerName );
		msg.mutable_router()->set_send2player( 1 ); // 发给客户端！(邀请者)
		msg.mutable_router()->set_routertype( CMessageRouter::ROUTER_NAME );

		tMessage.set_msgpara((uint32_t) &msg);

		tMessage.mutable_msghead()->set_messageid( ID_S2C_PLAYER_INVITE_TASK_ERROR_MSG );

		CSceneLogic::GetSingleton( ).Send2Player( pSrcPlayerName, &tMessage );
		return -1;
	}
	pList->SetInvitingPlayerID( nInvitingPlayerID );
	bool bRet = IsTaskAcceptable( pDesPlayer, pList, unTaskID ) ; 
	pList->SetInvitingPlayerID( -1 );
	if ( bRet )// 如果能接这个任务
	{
		// 判断包裹满没满
		// 取得任务需要发放的道具, 因为TestBaggageSpace的接口这样
		CItemsPair tExtendItems;
		PERF_FUNC( "GetTaskExtendItems" , GetTaskExtendItems( pDesPlayer, unTaskID, tExtendItems) );

		//  检查空间是否满足，如果不满足那么任务不能接，同时客户端也不会关闭任务对话界面
		if (tExtendItems.size() > 0 )
		{
			const int BAGGAGESPACE = 4;
			int tExtendItemID[BAGGAGESPACE] = {0};
			int tExtendItemNumber[BAGGAGESPACE] = {0}; 
			CItemsPair::iterator iter = tExtendItems.begin();
			for(int i = 0; iter != tExtendItems.end(); ++iter,++i)
			{
				tExtendItemID[i] = iter->first;
				tExtendItemNumber[i] = iter->second;
			}
			CPropertyPlayer* pDesProperty = (CPropertyPlayer*)pDesPlayer->GetProperty();
			if ( pDesProperty->TestBaggageSpace( tExtendItemID, tExtendItemNumber, BAGGAGESPACE ) == false )
			{
				CMessagePlayerInviteTaskErrorMsg msg; 
				CMessage tMessage;
				msg.set_taskid( unTaskID );
				msg.set_myname( pDesPlayer->GetCharName() );
				msg.set_typeid_( ERROR_CANT_ACCEPT );
				msg.mutable_router()->set_srcname( pDesPlayer->GetCharName() );
				msg.mutable_router()->set_desname( pSrcPlayerName );
				msg.mutable_router()->set_send2player( 1 ); // 发给客户端！(邀请者)
				msg.mutable_router()->set_routertype( CMessageRouter::ROUTER_NAME );
				tMessage.set_msgpara((uint32_t) &msg);
				tMessage.mutable_msghead()->set_messageid( ID_S2C_PLAYER_INVITE_TASK_ERROR_MSG );
				CSceneLogic::GetSingleton( ).Send2Player( pSrcPlayerName, &tMessage );
				return -1;
			}
		}

		int ret = mTaskInviteInfMgr.RegInvite( pDesPlayer->GetEntityID(), pSrcPlayerName , unTaskID );
		if ( 0 == ret )
		{
			// 登记信息成功
			// 发给对方
			CMessagePlayerInviteTaskNotice msg;

			CMessage tMessage;

			msg.set_taskid( unTaskID );
			msg.set_srcplayername( pSrcPlayerName );

			tMessage.set_msgpara((uint32_t) &msg);

			tMessage.mutable_msghead()->set_messageid( ID_S2C_INVITE_TASK_NOTICE );

			CSceneLogic::GetSingleton( ).Send2Player( pDesPlayer, &tMessage );

			return 0;
		}
		else if ( -1 == ret )
		{
			return -1;
		}
		else if ( -2 == ret )
		{
			return -1;
		}

	}
	else
	{
		// 返回  对方不能接 给客户端发消息
		CMessagePlayerInviteTaskErrorMsg msg; 

		CMessage tMessage;

		msg.set_taskid( unTaskID );
		msg.set_myname( pDesPlayer->GetCharName() );
		msg.set_typeid_( ERROR_CANT_ACCEPT );

		msg.mutable_router()->set_srcname( pDesPlayer->GetCharName() );
		msg.mutable_router()->set_desname( pSrcPlayerName );
		msg.mutable_router()->set_send2player( 1 ); // 发给客户端！(邀请者)
		msg.mutable_router()->set_routertype( CMessageRouter::ROUTER_NAME );

		tMessage.set_msgpara((uint32_t) &msg);

		tMessage.mutable_msghead()->set_messageid( ID_S2C_PLAYER_INVITE_TASK_ERROR_MSG );

		CSceneLogic::GetSingleton( ).Send2Player( pSrcPlayerName, &tMessage );
	}
	return 0;
}

// 搜索任务 效率啊。。。
int  CTaskModule::OnPlayerSearchTask(CEntityPlayer* pPlayer )
{
	CMessagePlayerSearchResponse msg;

	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken());
	LK_ASSERT( pList, return 0 );

	const int nCurLevel = pPlayer->GetLevel();
	LK_ASSERT ( nCurLevel > 0 && nCurLevel <= MAX_GRADE , return -1 );

	int nStartLevel = std::max(1, nCurLevel-(int)CSceneCfgManager::GetSingletonPtr()->GetTask_cfg()->mSearchmaindownlevel);
	int nEndLevel   = std::min(MAX_GRADE, nCurLevel+(int)CSceneCfgManager::GetSingletonPtr()->GetTask_cfg()->mSearchmainuplevel);
	// 搜索主线任务
	for (int i = nStartLevel; i <= nEndLevel ; ++i)
	{
		for( int j = 0; j < mLevelTasksN[0][i]; ++j  )
		{
			if ( pList->GetTaskState(mLevelTasks[0][i][j]) == -1 )
			{
				bool bRet = false;
				PERF_FUNC("IsTaskAcceptable", bRet = IsTaskAcceptableExpress( pPlayer, pList, mLevelTasks[0][i][j]) );

				if (bRet )
				{
					//CTask tTask;
					//tTask.mTaskID = mLevelTasks[0][i][j];
					msg.add_taskids(mLevelTasks[0][i][j]);
				}
			}
		}
	}

	nStartLevel = std::max(1, nCurLevel-(int)CSceneCfgManager::GetSingletonPtr()->GetTask_cfg()->mSearchxydownlevel);
	nEndLevel   = std::min(MAX_GRADE, nCurLevel+(int)CSceneCfgManager::GetSingletonPtr()->GetTask_cfg()->mSearchxyuplevel);

	// 搜索西游任务
	for (int i = nStartLevel; i <= nEndLevel ; ++i)
	{
		for( int j = 0; j < mLevelTasksN[2][i]; ++j  )
		{
			if ( pList->GetTaskState(mLevelTasks[2][i][j]) == -1 )
			{
				bool bRet = false;
				PERF_FUNC("IsTaskAcceptable", bRet = IsTaskAcceptableExpress( pPlayer, pList, mLevelTasks[2][i][j]) );
				if (bRet)
				{
					//CTask tTask;
					//tTask.mTaskID = mLevelTasks[2][i][j];
					msg.add_taskids(mLevelTasks[2][i][j]);
				}
			}
		}
	}

	nStartLevel = std::max(1, nCurLevel-(int)CSceneCfgManager::GetSingletonPtr()->GetTask_cfg()->mSearchzhidownlevel);
	nEndLevel   = std::min(MAX_GRADE, nCurLevel+(int)CSceneCfgManager::GetSingletonPtr()->GetTask_cfg()->mSearchzhiuplevel);

	// 搜索支线任务
	for (int i = nStartLevel; i <= nEndLevel ; ++i)
	{
		for( int j = 0; j < mLevelTasksN[1][i]; ++j  )
		{
			if ( pList->GetTaskState(mLevelTasks[1][i][j]) == -1 )
			{
				bool bRet = false;
				PERF_FUNC("IsTaskAcceptable", bRet = IsTaskAcceptableExpress( pPlayer, pList, mLevelTasks[1][i][j]) );
				if (bRet)
				{
					//CTask tTask;
					//tTask.mTaskID = mLevelTasks[1][i][j];
					msg.add_taskids(mLevelTasks[1][i][j]);
				}
			}
		}
	}

	CMessage   tMessage;
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_TASK_SEARCH);
	//printf("-----发出了%d个任务ID---", msg.taskids_size());
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
	return 0;

}

// 在收到玩家接任务请求的时候
int  CTaskModule::OnPlayerObtainTask(CEntityPlayer* pPlayer, unsigned short unTaskID, int nNpcEntityID )
{
	//  如果能正确地获得任务，那么才能关闭对话
	LK_ASSERT( pPlayer, return -1 );
	CEntityNpc* tpEntityNpc = dynamic_cast<CEntityNpc*>(CSceneObjManager::GetSingletonPtr()->GetObject( nNpcEntityID ) );
	if ( tpEntityNpc == NULL && nNpcEntityID != 0 )
		return -1;

	// 只有在国内和中立区才允许接任务 (如果是在国外 而且不是中立区 就不让接任务)
	//if (pPlayer->IsOverseas() && !(IS_NEUTRAL_LINE_ID(pPlayer->GetLineID()) ) )
	//	return -1;

	// 只有友好才能接任务
	if ( tpEntityNpc != NULL )
	{
		if( !( tpEntityNpc->IsFunNpc() && tpEntityNpc->IsFriendlyTo(pPlayer) ) )
		{
			return -1;
		}
	}

	// 如果NPC已经是变怪或者护送中状态 不允许接任务
	if ( tpEntityNpc != NULL )
	{
		if ( tpEntityNpc->GetNpcChangeState() != 0 || tpEntityNpc->GetSendNpcState() != 0  )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_NPC_BUSY );
			Send2ClientOprFailed( pPlayer );
			return -1;
		}
	}

	bool bRet = false;
	PERF_FUNC("PlayerObtainTask", bRet = PlayerObtainTask( pPlayer, unTaskID, false, nNpcEntityID ));
	if (!bRet)
	{
		Send2ClientOprFailed(pPlayer);
	}else
	{
		LogEventPlayerAcceptTask(pPlayer, unTaskID);
	}
	return 0;
}


// 在收到玩家交任务请求的时候
int  CTaskModule::OnPlayerEndTask(CEntityPlayer* pPlayer, unsigned short unTaskID, unsigned short unSelect, int nBagIndex )
{
	//  如果能正确地的提交任务，那么才能关闭对话
	LK_ASSERT( pPlayer, return -1 );
	// 只有在国内和中立区才允许交任务 (如果是在国外 而且不是中立区 就不让交任务)
	//if (pPlayer->IsOverseas() && !(IS_NEUTRAL_LINE_ID(pPlayer->GetLineID()) ) )
	//	return -1;

	bool bRet = false;
	CPropertyPlayer* pPro = (CPropertyPlayer*)pPlayer->GetProperty();
	LK_ASSERT( pPro, return false);

	//int nBagIndex = pPro->GetBaggage()->GetTaskLockIndex();
	PERF_FUNC( "PlayerEndTask", bRet = PlayerEndTask( pPlayer, unTaskID,  unSelect, false, nBagIndex ) );
	if (!bRet)
	{
		Send2ClientOprFailed(pPlayer);
	}else
	{
		LogEventPlayerEndTask(pPlayer, unTaskID);
	}
	return 0;
}

int CTaskModule::OnPlayerRefreshTask(CEntityPlayer* pPlayer, unsigned short unTaskID, int mRefreshItemIndex, int nNpcEntityID )
{
	LK_ASSERT( pPlayer, return -1 );
	bool bRet = false;
	bRet = PlayerRefreshTaskAward(pPlayer, unTaskID, mRefreshItemIndex, nNpcEntityID );
	if (!bRet)
	{
		return -1;
	}
	else
	{
		LogEventPlayerRefreshTask(pPlayer, unTaskID);
	}
	return 0;
}
// 得到发给客户端的NPC任务列表的消息
bool CTaskModule::GetNpcTaskList( CEntityPlayer* pPlayer, CTaskList* pList2, int vNpcID, NpcTaskList* pList)
{
	if (vNpcID != 0)
	{
		// 取得发放任务列表
		lua_tinker::table t1 = CTaskLuaInterface::Call<lua_tinker::table, int>(pPlayer, pList2, "get_npc_extend_tasks", vNpcID);
		int n = luaL_getn(L, -1);

		if (n >= MAX_NPC_TASK_NUM)
		{
			LOG_ERROR("TASK", " max NPC Task num is 64!!" );
			n = 63;
		}
		pList->set_extendnumber(n);
		for(int i =1; i<=n; ++i)
		{
			lua_rawgeti(L, -1, i);
			pList->add_extendtaskid(lua_tinker::pop<int>(L)) ;
		}

		// 取得回收任务列表
		lua_tinker::table t2 = CTaskLuaInterface::Call<lua_tinker::table, int>(pPlayer, pList2, "get_npc_back_tasks", vNpcID);
		int m = luaL_getn(L, -1);
		pList->set_callbacknumber(m);
		for(int i =1; i<=m; ++i)
		{
			lua_rawgeti(L, -1, i);
			pList->add_callbacktaskid(lua_tinker::pop<int>(L)) ;
		}
		
		pList->set_npcid(vNpcID) ;

		return true;
	}

	return false;
}



//  建立实体
void CTaskModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	if ( pEntity->IsPlayer() )
	{
		CEntityPlayer* tpPlayer = (CEntityPlayer*) pEntity;
		LK_ASSERT( tpPlayer != NULL , return );

		CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(tpPlayer->GetTaskToken( ));
		if (pList != NULL)
		{			
			pList->SetOwnerEntityID(tpPlayer->GetEntityID());
			// 处理每日事务
			DaylyDispos(tpPlayer);

			// 处理每周事务
			WeeklyDispos(tpPlayer);

			// 限时任务还未结束 启动定时器
			CPropertyPlayer*  pProperty= ( CPropertyPlayer* ) tpPlayer->GetProperty();
			bool bIsNewDay = IsANewDay((time_t)pProperty->GetLastDaylyDisposTime(), time(0));
			std::vector<unsigned short> _fail_list;	// 失败的任务列表
			for ( int i = 0 ; i < pList->mTaskNumber && i < TASK_LIST_SIZE; ++i )
			{
				// 把初始化事件放到这里 因为这里肯定全部都加载完成了 包括包裹 
				// 这个函数中会检查包裹的
				CTaskModule::GetSingleton().RegTaskEventTick( &(pList->mTaskList[i]) );

				if ( pList->mTaskList[i].mGameTimeLeft > 0 )	// 限时任务 还没到时间(剩余秒数)
				{
					int nLeftTime = 0;
					// 检查看看是物理时间还是游戏时间
					int tLmtType = GetTimeLmtType( pList->mTaskList[i].mTaskID );
					if ( tLmtType == TIMETYPE_GAME )
					{
						nLeftTime = pList->mTaskList[i].mGameTimeLeft;
					}
					else if ( tLmtType == TIMETYPE_PHYSICAL ) // 物理时间
					{
						nLeftTime = pList->mTaskList[i].mGameTimeLeft - (int)(time(0)-pList->mLastLogOffTime); // 扣除玩家下线的时间
						if ( nLeftTime <= 0 ) // 任务失败了!物理时间已超时
						{
							_fail_list.push_back(pList->mTaskList[i].mTaskID);
							pList->mTaskList[i].mGameTimeLeft = -1;
						}
						else
						{
							pList->mTaskList[i].mGameTimeLeft = nLeftTime;
						}
					}
					else
					{
						LOG_ERROR("TASK", "task %d time lmt:%d ?", pList->mTaskList[i].mTaskID, tLmtType);
					}

					if ( nLeftTime > 0 )	// 启动定时器
					{
						int nTimerOtherInfo[ TIMER_OTHER_INFO ] = { MESSAGE_PROPERTY, pList->mTaskList[i].mTaskID, 0, 0};
						pList->mTaskList[i].mTimerID = tpPlayer->GetTimerReciver()->SetTimer( tpPlayer->GetEntityID()
							, TIMER_TASKTIMELIMIT, pList->mTaskList[i].mGameTimeLeft*1000, 2, nTimerOtherInfo );
					}
				}
				else if ( pList->mTaskList[i].mGameTimeLeft == 0 )
				{
					LOG_WARN("TASK", "task %d left time = 0? so qiao??", pList->mTaskList[i].mTaskID );
				}

				if ( bIsNewDay && IsDailyFail(pList->mTaskList[i].mTaskID) ) // 新一天 还得检查是不是 会失败
				{
					_fail_list.push_back(pList->mTaskList[i].mTaskID);
				}
			}

			std::vector<unsigned short> tumb;
			for( int i = 0; i < pList->mTaskNumber; ++i )
			{
				if ( !CTaskLuaInterface::Call<bool, unsigned short, const char*>( tpPlayer, pList, "check_task", pList->mTaskList[i].mTaskID, "onCreateEntity" ) )
				{
					tumb.push_back( pList->mTaskList[i].mTaskID );
					continue;
				}
			}

			// 干掉
			for ( std::vector<unsigned short>::iterator it = tumb.begin(); it != tumb.end(); ++it )
			{
				pList->RemoveTask( *it );
			}

			// 发消息
			SendTaskMsg2Client( pList );

			// 让任务失败 因为失败可能删除 所以不能在循环中调用
			// 应该等给客户端发完列表再处理
			for ( std::vector<unsigned short>::iterator it = _fail_list.begin(); it != _fail_list.end(); ++it )
			{
				OnTaskFailed( tpPlayer, *it );
			}

			// 临时处理！
			TempDispos( tpPlayer, pList );

			for (int i = 0; i < TASK_LIST_SIZE; ++i)
			{
				unsigned short taskID = pList->mRememberRefreshNum[i].rTaskID;
				int taskColor = pList->mRememberRefreshNum[i].rTaskColor;
				Send2ClientRefreshResult( tpPlayer, taskID, taskColor, i );
			}
		}
		else // 新手！
		{
			//SetDefaultTask(tpPlayer);
		}
	}
	else if ( pEntity->IsFunNpc() || pEntity->IsOgre() )
	{
		CEntityNpc* tpNpc = (CEntityNpc*)pEntity;
		if ( -2 == mNpcMonitor.OnNpcCreated( (int)tpNpc->GetNpcID(), tpNpc->GetEntityID() ) )
		{
			LOG_ERROR("TASK", "npc tempid=%d  re reged!", tpNpc->GetNpcID() );
		}
	}
}

//  销毁实体
void CTaskModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{
	if ( pEntity->IsPlayer() )
	{
		CEntityPlayer* tpPlayer = (CEntityPlayer*) pEntity;
		CSceneObjManager::GetSingletonPtr()->DestroyObject( tpPlayer->GetTaskToken( ) );
	}
	else if ( pEntity->IsFunNpc() )
	{
		CEntityNpc* tpNpc = (CEntityNpc*)pEntity;
		mNpcMonitor.OnNpcDestroyed( (int)tpNpc->GetNpcID() ) ;
	}
}

//  存储数据
bool CTaskModule::OnSaveData( CEntityPlayer* pPlayer , int vSaveCode )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList, return false);

	// 下线了(只要不是定时存  都视为下线)
	if (vSaveCode != SAVE_TIMETRIGER)
	{
		if ( pList->GetSendNpcEntityID() != INVALID_OBJ_ID )
			mSendNpcMgr.ClearPlayerSendSession( pPlayer, true ); // 处理护送
		if (pList->GetBossNpcEntityID() != 0)
		{
			// 恢复变BOSS的状态
			CEntityNpc* pNpc = (CEntityNpc*)CSceneLogic::GetSingleton().GetEntity( pList->GetBossNpcEntityID() );
			if (pNpc == NULL)
			{
				LOG_ERROR("TASK", "battling NPC disappeared? ID=%d", pList->GetBossNpcEntityID() );
			}
			else
			{
				OnNpcRecoverdState( pPlayer->GetCharID(), pNpc->GetTaskID(), pNpc->GetNpcID() );
			}
		}

		// 检查身上的任务 有没有下线就失败的 (跨服也算)
		// 检查有没有限时任务要保存剩余时间的
		std::vector<unsigned short> _fail_list;	// 失败的任务列表
		for ( int i = 0; i < std::min( pList->mTaskNumber, (unsigned short)TASK_LIST_SIZE ); ++i )
		{
			if ( (pList->GetTaskState(pList->mTaskList[i].mTaskID) == TASK_BEGIN) 
				&& (IsOffFail(pList->mTaskList[i].mTaskID) == 1) )
			{
				_fail_list.push_back(pList->mTaskList[i].mTaskID);
				continue;
			}

			if ( (pList->mTaskList[i].mExFlags&EXFLAG_PROTECT_FLAG_TASK) == EXFLAG_PROTECT_FLAG_TASK ) // 守旗任务 取消注册
			{
				mNpcMonitor.RemoveGlobalTaskAction( pList->mTaskList[i].GetFirstParamByEvent(EVENT_PROTECT_FLAG)
					, pPlayer, pList->mTaskList[i].mTaskID );
				if ( pList->mTaskList[i].mTaskProgress == TASK_BEGIN )
					_fail_list.push_back(pList->mTaskList[i].mTaskID);
				continue;
			}

			// 如果任务失败 就不用处理timer了 因为失败里面会干掉timer的
			if ( pList->mTaskList[i].mTimerID != INVALID_OBJ_ID )  // 限时任务还未结束
			{
				CTimerItem* tpTimer = CTimerMng::GetSingleton().GetTimer(pList->mTaskList[i].mTimerID);
				if ( tpTimer == NULL )
				{
					LOG_ERROR("TASK", "task%d can't find timer? timerid=%d", pList->mTaskList[i].mTaskID, pList->mTaskList[i].mTimerID );
				}
				else
				{
					// 设置剩余时间(秒钟)
					pList->mTaskList[i].mGameTimeLeft = tpTimer->mMillSeconds/1000;

					// 干掉定时器
					int tRet = pPlayer->GetTimerReciver()->ClearTimer( pList->mTaskList[i].mTimerID );
					if ( tRet != 0 )
						LOG_ERROR("TASK", "OnSaveData del1 timer returned %d, timerid=%d taskid=%d"
						, tRet, pList->mTaskList[i].mTimerID, pList->mTaskList[i].mTaskID );
					 pList->mTaskList[i].mTimerID = INVALID_OBJ_ID;
				}
			}
		}

		for ( std::vector<unsigned short>::iterator it = _fail_list.begin(); it != _fail_list.end(); ++it )
		{
			OnTaskFailed( pPlayer, *it );
		}

		// 检查是不是有召唤出来的NPC 有就干掉
		if ( pPlayer->GetCallNpcEntityID() != INVALID_OBJ_ID )
		{
			//CEntity* tpNpc = CSceneLogic::GetSingleton().GetEntity(pPlayer->GetCallNpcEntityID());
			//CSceneLogic::GetSingleton().DestroyEntity( tpNpc, 0 );
			// 走延迟删除列表
			CSceneLogic::GetSingleton().AddDeleteList( pPlayer->GetCallNpcEntityID() );
		}
	}	

	char tBuffer[ PLAYER_DATALEN ] = { 0 }; 
	int  tBufferSize = sizeof(tBuffer);

	PBTaskList tPBTask;

	pList->GetTlvBuffer2( &tPBTask );
	if (vSaveCode != SAVE_TIMETRIGER)
	{
		tPBTask.set_lastlogoff(time(0));
	}
	tPBTask.SerializePartialToArray( tBuffer, tBufferSize );

	//pPlayer->mBuffer.PushBuffer( sModuleName, tBuffer, tPBTask.ByteSize() );

	char tEscapStr[ PLAYER_DATALEN ] = {0};
	char* tPointer = tEscapStr;

	int tRealLen = EscapeDataString( &tPointer, sizeof(tEscapStr)-1, tBuffer, tPBTask.ByteSize() );

	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return false);

	int tIsCallBack = SQL_NEED_CALLBACK;
	if ( vSaveCode == SAVE_TIMETRIGER )
		tIsCallBack = SQL_NONE;

	CMessagePlayerProperty* tpSave = CSceneLogic::GetSingletonPtr( )->GetSavePlayerProperty( );
	
	tpSave->mutable_role_task( )->CopyFrom( tPBTask );
	return true;

//	return CSceneLogic::GetSingletonPtr()->ExecuteSql( saveplayer_task, vSaveCode, pPlayer->get_id(), pPlayer->GetCharID(), tIsCallBack, 
//		UPDATE, "update UMS_ROLE_DETAIL_%02d set role_task = '%s', save_bitflag = save_bitflag + '%d' where role_id = %d",
//		pPlayer->GetAccountID()%16, tEscapStr, LSSTATE_TASK,  pPlayer->GetCharID() );

}


void CTaskModule::OnEntityDie( CEntity* pOwnerEntity, CEntity* pDesEntity )
{
	LK_ASSERT( pOwnerEntity != NULL && pDesEntity != NULL , return );

	if ( pOwnerEntity->IsPet() ) // 是宠物打死的
	{
		CEntityPet* tpPet = (CEntityPet*)pOwnerEntity;
		pOwnerEntity = tpPet->GetOwner();
		LK_ASSERT( pOwnerEntity!=NULL, return );
	}

	if( !pOwnerEntity->IsPlayer() ||  ( !pDesEntity->IsOgre() && !pDesEntity->IsFunNpc() && !pDesEntity->IsBlock() ) )
		return;

	CEntityNpc* tpEntityNpc = (CEntityNpc*)pDesEntity;
	mNpcMonitor.OnNpcDied( tpEntityNpc->GetNpcID() );

	if (pDesEntity->IsFunNpc()) // 如果是NPC 则归属于召唤它出来的那个人
	{
		// 把NPC状态弄回去
		// 把状态改回去 除了PK状态（不能让玩家在跑回去之前点它）
		tpEntityNpc->SetTaskID( 0 );// 这里 changestate先不改 等跑回去再改
		if (tpEntityNpc->GetOwnercharID() != 0)
		{
			pOwnerEntity = CSceneLogic::GetSingleton().GetPlayerByCharID(tpEntityNpc->GetOwnercharID());
			if (pOwnerEntity == NULL) // 可能已经不在了 那NPC就白死了
			{
				tpEntityNpc->SetOwnerCharID(0);
				return;
			}
		}
		tpEntityNpc->SetOwnerCharID(0);
	}

	CPropertyCharacter* pDesProperty = (CPropertyCharacter*)pDesEntity->GetProperty();


	LOG_DEBUG("TASK", "ogre died，ID=%d", pDesProperty->GetPropertyTempID( ) );
	//  step1: 得到源玩家组队信息 and 准备工作
	CEntityPlayer* pTeamMates[TEAM_NUMBER] = {NULL};
	int tRtn = 0, tTeamNum = 0;
	tRtn = CTeamModule::GetSingleton().OnGetMemberList( (CEntityPlayer*)pOwnerEntity, pTeamMates, ARRAY_CNT( pTeamMates ), tTeamNum );
	if ( tRtn < 0 )
	{
		LOG_ERROR( "team", "task, get team list failed.id=%d, [%s:%d]", pOwnerEntity->GetEntityID(), __LK_FILE__, __LINE__);
		return ;
	}

	CItemsPair tSpecialTaskItem; // 有可能需要分发的特殊任务道具
	std::map< unsigned short, unsigned short >  tmpUpList[TEAM_NUMBER]; // 需要更新身上任务的玩家实体ID和任务ID列表
	CMessageUpdateTaskConditions update[TEAM_NUMBER]; // 更新消息

	// step2: 遍历队员身上的任务
	if ( tTeamNum == 0 )
	{
		LOG_ERROR("TASK", "error in get team member num" );
		return;
	}

	//printf("1--%d\n", a.elapsed() );
	//a.start();

	int  nIndex = -1;
	int  n = 0 ;

	while( ++n <= tTeamNum )
	{
		CEntityPlayer* tpPlayer = (CEntityPlayer*)pOwnerEntity;
		// 组队情况
		if (tTeamNum > 1)
		{
			tpPlayer = CTeamModule::GetSingleton().GetTurnOwnerOnTask( pTeamMates[0] );
			if ( tpPlayer == NULL )
				continue;

			//CWTPoint pt01 = CWTPoint( tpPlayer->GetPosX(), tpPlayer->GetPosY() );
			//CWTPoint pt02 = CWTPoint( pOwnerEntity->GetPosX(), pOwnerEntity->GetPosY() );
			if ( tpPlayer->IsWithinDistance( pDesEntity, TEAM_VALID_DISTANCE ) == false )
			{ 
				continue; // 超过距离
			}

			for( int i = 0 ; i < tTeamNum; ++i )
			{
				LK_ASSERT(pTeamMates[i], continue);
				if ( tpPlayer->GetEntityID() == pTeamMates[i]->GetEntityID() )
				{
					nIndex = i;
					break;
				}
			}

			if ( nIndex == -1 ) // 不可能
				continue;
		}
		else
		{
			// 他自己
			nIndex = 0;
		}

		//printf("2.1--%d\n", a.elapsed() );
		//a.start();

		// 得到怪物归属者身上的任务，掉落物品

		OnTaskDropItems( tpPlayer, pDesProperty, tSpecialTaskItem, tmpUpList[nIndex], &update[nIndex] );

		//printf("2.2--%d\n", a.elapsed() );
		//a.start();
	}

	// step2完成，该掉的普通物品应该都掉完了
	for( int i = 0; i < tTeamNum && i < (int)ARRAY_CNT(pTeamMates); ++i)
	{
		if ( pTeamMates[i] == NULL )
		{
			LOG_ERROR("TASK","team BUG !! NULL pointor ! LINE=%d", __LINE__ );
			continue;
		}
		CPropertyPlayer*  pProperty= ( CPropertyPlayer* ) pTeamMates[i]->GetProperty();
		if ( NULL == pProperty ) 
		{
			LOG_ERROR("TASK", "error, NULL pointer,file=%s, FUNC=%s, line = %d", __FILE__, __FUNCTION__, __LINE__);
			continue;
		}

		OnTaskOgreDie( pOwnerEntity, pTeamMates[i], pDesProperty, tSpecialTaskItem, tmpUpList[i], &update[i] ) ;

		// 顺便更新下

		CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pTeamMates[i]->GetTaskToken( ));
		LK_ASSERT( pList, continue );

		// 给客户端发更新消息
		SendUpdateConditions2Client( pTeamMates[i], &update[i] );

		for ( std::map<unsigned short, unsigned short>::iterator it = tmpUpList[i].begin(); it != tmpUpList[i].end(); ++it)
		{
			// 以下这几句没必要
			CTask* pTask = pList->GetTask( it->first );
			LK_ASSERT( pTask, continue );
#ifdef _DEBUG_
			if ( pTask->mWatchEventN > 0 ) // 还没完成, 只通知客户端刷新任务
			{
				LOG_ERROR("TASK", "err ! entitydie update watch > 0 !");
				continue;
			}
#endif
			bool bAutoEnd = false;
			PERF_FUNC("UpdateTaskProgress", UpdateTaskProgress( pTeamMates[i], pTask, bAutoEnd, 1 ) );
			if ( bAutoEnd ) // 这个任务要自动完成
			{
				PERF_FUNC( "PlayerEndTask", PlayerEndTask( pTeamMates[i], it->first, -100, true ) );
				LogEventPlayerEndTask( pTeamMates[i], it->first);
			}
		}
	}

	//printf("3--%d\n", a.elapsed() );
	//printf("~~~total--%d\n\n", b.elapsed() );
}

void CTaskModule::NotifyClientUpTask( CEntityPlayer* pPlayer, CTask* pTask, int nPreState, unsigned int nEventID, unsigned int nParam )
{
	LK_ASSERT( pPlayer!=NULL && pTask!=NULL, return );
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList, return );
	LK_ASSERT( pTask != NULL, return );

	// 如果是更新物品 则用新接口通知
	if ( nEventID == EVENT_ITEMCHANGE)
	{
		if (pTask->GetTaskProgress() == TASK_BEGIN || nPreState == TASK_BEGIN )
		{
			CMessageUpdateTaskConditions msg;
			TaskCondition* pCondition = msg.add_contions();
			LK_ASSERT( pCondition, return );
			pCondition->set_taskid( pTask->mTaskID );
			pCondition->set_tempid( nParam );
			pCondition->set_num( 0 ); // 物品不用数量
			pCondition->set_condition( CONDITION_ITEM );
			SendUpdateConditions2Client( pPlayer, &msg );
		}
		return;
	}

	// 验证/重置timer
	if (pTask->mTimerID != INVALID_OBJ_ID)
	{
		CTimerItem* tpTimer = CTimerMng::GetSingleton().GetTimer(pTask->mTimerID);
		if ( tpTimer == NULL )
		{
			LOG_ERROR("TASK", "task%d can't find timer? timerid=%d", pTask->mTaskID, pTask->mTimerID );
		}
		else
		{
			// 设置剩余时间(秒钟)
			pTask->mGameTimeLeft = tpTimer->mMillSeconds/1000;
		}
	}


	CMessageUpdateTaskNotice msg;
	TaskStruct* tpTask = msg.mutable_task();
	pTask->GetTaskDataStruct(tpTask);
	CMessage   tMessage;
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_UPDATE);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}



// 处理任务掉落物品 返回：是否掉落了任务物品
bool CTaskModule::OnTaskDropItems( CEntityPlayer* pPlayer, CProperty* pDesProperty, CItemsPair& rSpecial, std::map<unsigned short, unsigned short>& rUpList, CMessageUpdateTaskConditions* pUpdateMsg )
{
	//Clock a;
	//a.start();
	//printf("in OnTaskDropItems\n" );

	bool bret = false;
	CPropertyPlayer* tpProperty = (CPropertyPlayer*) pPlayer->GetProperty();
	LK_ASSERT ( NULL != tpProperty, return false)

	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false );

	for (int k = 0; k < pList->mTaskNumber; ++k)
	{
		//printf("0.1--%d\n", a.elapsed() );
		//a.start();
		if ( pList->mTaskList[k].mTaskProgress != TASK_BEGIN )
			continue;
		if ( !pList->mTaskList[k].IsEventTick( EVENT_OGREDIE, pDesProperty->GetPropertyTempID( )) )
			continue;

		int nDropID = 0;
		int n = 0;
		int nMaxn = 0;

		if( mTaskDropMgr.GetDropItems( pList->mTaskList[k].mTaskID, pDesProperty->GetPropertyTempID( ), nDropID, n, nMaxn ) )
		{
			CTplItem* tpItem = (CTplItem*)CDataStatic::SearchTpl( nDropID );
			if (tpItem != NULL && ( tpItem->mItemType != CTplItem::ITEM_TASK || tpItem->mItemType != CTplItem::ITEM_SPECIAL_TASK || 
				tpItem->mItemType != CTplItem::ITEM_TOKEN ) )
			{
				if (tpItem->mItemType == CTplItem::ITEM_SPECIAL_TASK) // 如果是特殊任务物品，做下标记，在下面的遍历队员的地方分别发放
				{
					rSpecial.push_back(std::make_pair<int, int>( nDropID, 1));
					bret = true; // 掉了特殊的也算
				}
				else
				{
					int tNeedNum = nMaxn - tpProperty->BaggageHasItem(nDropID);				
					if ( tNeedNum <= 0 ) // 如果是普通任务道具，且玩家还没搜集够
						continue;

					int tNum = std::min( tNeedNum, n );
					// 检查包裹满了没
					if ( ! tpProperty->TestBaggageSpace( &nDropID, &tNum, 1 ) )
					{
						CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_INSERTITEM_NOSPACE );
						break;
					}
					else
					{
						//printf("0.22--%d\n", a.elapsed() );
						//a.start();
						// 将掉落的物品插入到物品栏中
						CItemObject* pInsItemPile = 
							CPropertyModule::GetSingleton( ).PlayerInsertItem( pPlayer, nDropID, tNum, true );

						if ( pInsItemPile != NULL)
						{
							LogEventGetItemByRunTask(pPlayer, pInsItemPile->GetGUID(), 
									nDropID, tNum, pList->mTaskList[k].mTaskID, pDesProperty->GetPropertyTempID());
						}

						bret      = true;
						// bIsDrop   = true;
						if ( pList->mTaskList[k].mWatchEventN > 0 && pList->mTaskList[k].mWatchEventN-tNum <= 0 )
						{
							// 任务完成了
							rUpList.insert( std::make_pair(pList->mTaskList[k].mTaskID, pList->mTaskList[k].mTaskID) );
						}
						pList->mTaskList[k].mWatchEventN -= tNum;
						TaskCondition* pCondition = pUpdateMsg->add_contions();
						LK_ASSERT( pCondition, return false );
						pCondition->set_taskid( pList->mTaskList[k].mTaskID );
						pCondition->set_tempid( nDropID );
						pCondition->set_num( tNum );
						pCondition->set_condition( CONDITION_ITEM );
						LOG_DEBUG("TASK", "minused mWatchEventN=%d", pList->mTaskList[k].mWatchEventN );
					}
				}
			}

			//printf("0.23--%d\n", a.elapsed() );
			//a.start();
		}
		//if ( bIsDrop )
		//{
		//	rUpList.insert( make_pair(pList->mTaskList[k].mTaskID, pList->mTaskList[k].mTaskID) );
		//}
		//printf("0.3--%d\n", a.elapsed() );
		//a.start();
	}
	return bret;
}


// 处理任务怪死 计数问题 和遍历特殊任务道具问题  有变化就返回true
bool CTaskModule::OnTaskOgreDie( CEntity* pOwnerEntity, CEntityPlayer* pPlayer, CProperty* pDesProperty, CItemsPair& rSpecialTaskItem, std::map<unsigned short, unsigned short>& rUpList , CMessageUpdateTaskConditions* pUpdateMsg  )
{
	if ( pPlayer == NULL ) 
		return false;

	bool bret = false;
	CPropertyPlayer* pProperty = (CPropertyPlayer*)pPlayer->GetProperty();

	// 取得玩家当前的任务列表
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false );

	// 对目前的任务逐一进行任务掉落更新 
	bool bIsStorageTaskUpdated = false; // 库任务是否已经计数过了 (库任务同时只计数1次)
	for(int i = 0; i < pList->mTaskNumber; ++i)
	{
		// 先看需要不需要某个特殊任务物品
		std::vector< std::pair<int, int> >::iterator it = rSpecialTaskItem.begin();
		for(; it != rSpecialTaskItem.end(); ++it)
		{
			if ( pList->mTaskList[i].IsEventTick( EVENT_ITEMCHANGE, it->first )
				&& !pProperty->GetTaskBaggage( )->HasItem(it->first) )
			{
				// 检查包裹满了没
				int num = 1;
				if ( ! pProperty->TestBaggageSpace( &it->first, &num, 1 ) )
				{
					CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_INSERTITEM_NOSPACE );
				}
				else
				{
					// 将掉落的物品插入到物品栏中(特殊任务物品)
					CItemObject * pInsItemPile = CPropertyModule::GetSingleton( ).PlayerInsertItem( pPlayer, it->first, 1, true);
					if ( pInsItemPile != NULL)
					{
						LogEventGetItemByRunTask(pPlayer, pInsItemPile->GetGUID(), 
								it->first, 1, pList->mTaskList[i].mTaskID, pDesProperty->GetPropertyTempID());
					}

					
					if ( pList->mTaskList[i].mWatchEventN > 0 && pList->mTaskList[i].mWatchEventN-1 <= 0 )
					{
						// 任务完成了
						rUpList.insert( std::make_pair(pList->mTaskList[i].mTaskID, pList->mTaskList[i].mTaskID) );
					}
					pList->mTaskList[i].mWatchEventN --;
					TaskCondition* pCondition = pUpdateMsg->add_contions();
					LK_ASSERT( pCondition, return false );
					pCondition->set_taskid( pList->mTaskList[i].mTaskID );
					pCondition->set_tempid( it->first );
					pCondition->set_num( 1 );
					pCondition->set_condition( CONDITION_ITEM );

					// pList->mTaskList[i].mWatchEventN--;
					LOG_DEBUG("TASK", "minused mWatchEventN=%d", pList->mTaskList[i].mWatchEventN );
					bret = true;
				}
			}
		}

		//if ( !counted ) // 没计数，判断怪物
		//{
		// 如果杀死的ORGE与任务相关，则需要更新任务进度,队伍里每个人都更新一遍
		// 更新不走update逻辑！！因为后面会统一调用update的
		if ( pPlayer->IsWithinDistance( pOwnerEntity, TEAM_VALID_DISTANCE ) == false )
			continue; // 超过距离

		if ( pList->mTaskList[i].mTaskProgress != TASK_BEGIN )
			continue;

		int nThisTaskType = GetTaskType( pList->mTaskList[i].mTaskID );
		if (nThisTaskType == TASKTYPE_STORAGE && bIsStorageTaskUpdated )
			continue; // 库任务只计数1次

		int _ret_num = 0;
		int _ret_id  = CONDITION_LEVELOGRE; // 没有模板ID的杀怪 则视为等级杀怪

		// 先判断是不是等级杀怪任务
		if ( (pList->mTaskList[i].mExFlags&EXFLAG_LEVELKILLOGRE) == EXFLAG_LEVELKILLOGRE )
		{
			// 等级杀怪 特殊处理
			_ret_num = CTaskLuaInterface::Call<int, CTask*, int>(pPlayer, pList, "on_level_orge_die", &pList->mTaskList[i], pDesProperty->GetLevel() );
		}
		else
		{
			if ( !pList->mTaskList[i].IsEventTick( EVENT_OGREDIE, pDesProperty->GetPropertyTempID( )) )
				continue;
			lua_tinker::table Params(L);
			Params.set("id", pDesProperty->GetPropertyTempID( ) );
			Params.set("n", 1);		
			lua_tinker::table ret = CTaskLuaInterface::Call<lua_tinker::table, CTask*, lua_tinker::table>(pPlayer, pList, "on_orge_die", &pList->mTaskList[i], Params);
			_ret_num = ret.get<int>("num");
			_ret_id  = ret.get<int>("id");
		}
		
		if( _ret_num  > 0 ) // lua判定杀怪成功！
		{
			TaskCondition* pCondition = pUpdateMsg->add_contions();
			LK_ASSERT( pCondition, return false );
			pCondition->set_taskid( pList->mTaskList[i].mTaskID );
			pCondition->set_tempid( _ret_id );
			pCondition->set_num( _ret_num );
			pCondition->set_condition( CONDITION_OGREDIE );

			// 记录日志
			LogEventTaskAction( pPlayer, pList->mTaskList[i].mTaskID, EM_TASK_ACTION, EVENT_OGREDIE, _ret_id );

			if ( pList->mTaskList[i].mWatchEventN > 0 && pList->mTaskList[i].mWatchEventN-1 <= 0 )
			{
				// 任务完成了
				rUpList.insert( std::make_pair( pList->mTaskList[i].mTaskID, pList->mTaskList[i].mTaskID ) );
			}
			//counted = true;
			pList->mTaskList[i].mWatchEventN--;
			LOG_DEBUG("TASK", "minused mWatchEventN=%d", pList->mTaskList[i].mWatchEventN );
			bret = true;

			if ( nThisTaskType == TASKTYPE_STORAGE )
				bIsStorageTaskUpdated = true;
		}
	}
	return bret;
}

// 清除已完成的任务
void CTaskModule::ClearCompletedTask( CEntityPlayer* pPlayer, unsigned short usTaskID )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );
	pList->SetEnd(usTaskID);
}


void CTaskModule::ClearTragedy(CEntityPlayer* pPlayer, int nIndex )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );
	pList->mTragedyTaskMgr.Clear(nIndex);
}

// bIsSystemCall：是否系统调用 如果是 不处理护送 因为这本来就是护送引起的 避免死循环
void CTaskModule::PlayerAbortTask(CEntityPlayer* pPlayer, unsigned short vTaskID, bool bIsDebug, bool bIsSystemCall )
{
	LK_ASSERT(pPlayer, return);
	CPropertyPlayer* pProperty = (CPropertyPlayer*)pPlayer->GetProperty();

	if ( !bIsDebug && IsTaskAbortAble(vTaskID) == 0 && CSceneCfgManager::GetSingletonPtr()->GetTask_cfg()->mIscanabort == 1 )// 任务不能放弃
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_CANTABORTMAIN );
		return;
	}
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );

	CTask* pTask = pList->GetTask(vTaskID);
	if( pTask == NULL) 
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_ABORTINVALID );
		return;
	}

	if ( (pTask->mExFlags&EXFLAG_PROTECT_FLAG_TASK) == EXFLAG_PROTECT_FLAG_TASK ) // 守旗任务 取消注册
	{
		mNpcMonitor.RemoveGlobalTaskAction( pTask->GetFirstParamByEvent(EVENT_PROTECT_FLAG)
			, pPlayer, vTaskID );
	}

	int nTaskType = GetTaskType(vTaskID);
	// 如果是库任务，则扔回列表 并发最新列表给客户端
	if (nTaskType == TASKTYPE_STORAGE)
	{
		if ( pList->mStorageTaskMgr.OnAbortTask(vTaskID) ) // 如果放弃后的任务回去了
			SendStorageTaskList2Client( pPlayer, pList, pProperty->GetTalkNpcEntityID( ) );
	}

	// 如果是劫难任务 就设置相关状态
	if ( nTaskType == TASKTYPE_TRAGEDY )
	{
		int			   tIndex = CTaskLuaInterface::Call<int, unsigned short>( pPlayer, pList, "get_tragedy_id", vTaskID );
		int			   tState = TRAGEDY_ACTIVE;
		unsigned short tNextID = vTaskID;
		pList->mTragedyTaskMgr.SetState(tIndex, (enTragedyTaskState)tState);
		pList->mTragedyTaskMgr.SetNextTask(tIndex, tNextID);
		SendTragedyInfo2Client( pPlayer, &tIndex, &tNextID, &tState, 1 );
	}

	// 掌门任务 处理
	if ( nTaskType == TASKTYPE_CHAIRMAN )
	{
		pList->mChairManTaskMgr.OnAborted();
	}

	if ( !bIsSystemCall )
	{
		// 到护送任务那里去处理
		mSendNpcMgr.ClearPlayerSendSession( pPlayer, false );
	}

	// 删除任务列表中的任务 清除任务物品 给客户端发失败消息
	// OnTaskFailed(pPlayer, vTaskID, true);

	// 干掉timer id
	if( pTask->mTimerID != INVALID_OBJ_ID )
	{
		int tRet = pPlayer->GetTimerReciver()->ClearTimer( pTask->mTimerID );
		if ( tRet != 0 )
			LOG_ERROR("TASK", "abort del timer returned %d, timerid=%d taskid=%d", tRet, pTask->mTimerID, vTaskID );

		LOG_DEBUG("TASK", "abort del timer returned %d, timerid=%d taskid=%d", tRet, pTask->mTimerID, vTaskID );
		pTask->mTimerID = INVALID_OBJ_ID;
		pTask->mGameTimeLeft = -1;
	}


	pList->RemoveTask( vTaskID );
	
	// 取得发放的物品

	CItemsPair tmp; 

	// 取得发放物品
	PERF_FUNC( "GetTaskExtendItems", GetTaskExtendItems( pPlayer, vTaskID, tmp) );
	DropItems( pPlayer, tmp, vTaskID);

	// 删除已经收集的物品
	DropCompleteItems(pPlayer, vTaskID, true);

	CTaskLuaInterface::Call<int,int>( pPlayer, pList, "on_abort_task", vTaskID); // 到LUA里处理，如果是环任务等非常规任务要特殊处理
	if ( GetTaskType(vTaskID) == TASKTYPE_LOOP ) // 环任务要特殊处理
	{
		pList->mLoopTaskData.SetAbortData( );
		CTaskLuaInterface::Call<int, int, int>( pPlayer, pList, "init_loop_tasks" , pPlayer->GetMetier(), pPlayer->GetLevel() ); // 刷新环任务
	}

	if ( nTaskType == TASKTYPE_PUBLISH )
	{
		pList->SetPublishTaskState(vTaskID, PUBLISHED_TASK, GetTaskPublishType(vTaskID));
	}

	int vColor = pList->GetTaskColor(vTaskID);
	for (int i = 0; i < TASK_LIST_SIZE; ++ i)
	{
		if (pList->mRememberRefreshNum[i].rTaskID == vTaskID)
		{
			pList->RemoveTaskColor(vTaskID);
			Send2ClientRemoveTaskColor(pPlayer, vTaskID, vColor);
			break;
		}
	}

	FireEvent( pPlayer, EVENT_TASKCHANGE, vTaskID ) ;
	 

	CMessageAbortTaskNotice msg;
	msg.set_taskid( vTaskID );
	CMessage   tMessage;
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_ABORT_TASK);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}


//  玩家获得任务
bool CTaskModule::PlayerObtainTask( CEntityPlayer* pPlayer, unsigned short vTaskID, bool bIsNoChkNpc, int nNpcEntityID, int nInvitingPlayerID )
{
	LK_ASSERT(pPlayer, return false);
	CPropertyPlayer* pProperty = (CPropertyPlayer*)pPlayer->GetProperty();

	// 取得玩家的任务列表信息
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false );

	// 检查任务是否完成或者在进行中
	if(pList->GetTaskState(vTaskID) != -1)
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_HASACCEPT );
		return false;
	}

	int NowTime = time(0);
	int CDtime = GetTaskReaccept(vTaskID);
	int NeedTime = pList->GetEndTime(vTaskID) + CDtime - NowTime;
	if ( CDtime != 0 &&  pList->GetEndTime(vTaskID) != 0)
	{
		if ( NeedTime > 0 )
		{
			Send2ClientUnReaccpetTask(pPlayer, vTaskID, NeedTime );	
			return false;
		}
	}

	// 任务是否可以接, 调用lua脚本
	bool bRet = false;

	// 判断NPC合法 玩家必须站在NPC旁边接任务 皇榜任务除外
	int nNpcID = 0;
	CEntity* tpEntity = CSceneLogic::GetSingleton( ).GetEntity( nNpcEntityID );
	if ( tpEntity != NULL ) 
	{
		if( tpEntity->IsNpc() && pPlayer->IsWithinDistance( tpEntity, MAX_NPC_TALK_GIRD) ) // 在合法距离之内才判断
		{
			CPropertyNPC* pPropertyNpc = (CPropertyNPC*)tpEntity->GetProperty();
			if ( pPropertyNpc == NULL )
			{
				LOG_ERROR("TASK", "npc property is null, line=%d", __LINE__ );
			}
			else
			{
				nNpcID = pPropertyNpc->GetPropertyTempID();
			}
		}
	}

	pList->SetTalkNpcEntityID( nNpcEntityID );

	pList->SetInvitingPlayerID( nInvitingPlayerID ); // 在接任务条件的处理中会用到这个值

	int nTaskType = GetTaskType( vTaskID );

	//  未发布任务 不可接

	if(nTaskType == TASKTYPE_PUBLISH)
	{
		int SelfState = pList->GetPublishStateByType(vTaskID, GetTaskPublishType(vTaskID));
		if(SelfState == UNPUBLISH_TASK)
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_UNPUBLISHED);
			return false;
		}
	}

	// 如果是仙轴 劫难任务 不检查NPC  外部要求不检查的时候也不检查
	if ( bIsNoChkNpc 
		|| nTaskType == TASKTYPE_SCROLL 
		|| nTaskType == TASKTYPE_TRAGEDY 
		|| nTaskType == TASKTYPE_FAMILY 
		|| nTaskType == TASKTYPE_PUBLISH )
	{
		PERF_FUNC( "IsTaskAcceptable", bRet = IsTaskAcceptable(pPlayer, pList, vTaskID, 0 ) );
	}
	else if( nNpcID != 0 )
	{
		PERF_FUNC( "IsTaskAcceptable", bRet = IsTaskAcceptable(pPlayer, pList, vTaskID, nNpcID ) );
	}
	pList->SetInvitingPlayerID( -1 );

	if ( IsTeamLoop(vTaskID) == 1 )
	{
		// 如果是组队环任务 判断所有成员(在范围内，完成次数没超，身上无同类型任务，任务列表没满)
		// 先取到队伍列表
		CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( CTeamModule::GetSingleton().InTeam( pPlayer->GetCharID() ) );
		if (tpTeam == NULL)
		{
			CPropertyModule::GetSingleton().SendProErrorNotice(pPlayer, ERROR_TASK_TEAMLOOP_YOUNOTHEAD);
			return false;
		}
		if ( tpTeam->GetHeaderID() != pPlayer->GetCharID() )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice(pPlayer, ERROR_TASK_TEAMLOOP_YOUNOTHEAD);
			return false;
		}
		int nTeamMember[TEAMNUM] = {0};
		int nMemberNum=0;
		tpTeam->GetMemberList(nTeamMember, TEAMNUM, nMemberNum);
		LK_ASSERT(nMemberNum <= TEAMNUM, return false );
		// 判断人数
		int nMemberLmt = GetTeamMember(vTaskID);
		int vColor = pList->GetTaskColor(vTaskID);
		if (nMemberLmt != nMemberNum)
		{
			CPropertyModule::GetSingleton().SendProErrorNotice(pPlayer, ERROR_TASK_TEAMLOOP_MEMBERLESS);
			return false;
		}
		for (int i = 0 ; i < nMemberNum; ++i )
		{
			CEntityPlayer* tpMember = CSceneLogic::GetSingleton().GetPlayerByCharID(nTeamMember[i]);
			if (tpMember == NULL)
			{
				CPropertyModule::GetSingleton().SendProErrorNotice(pPlayer, ERROR_TASK_TEAMLOOP_MEMBERERR);
				return false;
			}
			if ( pPlayer->IsWithinDistance( tpMember, TEAM_VALID_DISTANCE ) == false )
			{
				CPropertyModule::GetSingleton().SendProErrorNotice(pPlayer, ERROR_TASK_TEAMLOOP_MEMBERERR);
				return false;
			}
			CTaskList* tpList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(tpMember->GetTaskToken( ));
			LK_ASSERT( tpList,  return false );
			for (int j = 0; j < std::min(tpList->mTaskNumber, (unsigned short)TASK_LIST_SIZE); ++j)
			{
				if (IsTeamLoop(tpList->mTaskList[j].mTaskID) == 1)
				{
					SendTeamLoopErrNotify( pPlayer, tpMember->GetCharName(), ERROR_TASK_TEAMLOOP_MEMBERGOT);
					return false;
				}
				if (tpList->GetTodayTeamLoopNum() >= MAX_TEAMLOOP_EVERYDAY)
				{
					SendTeamLoopErrNotify( pPlayer, tpMember->GetCharName(), ERROR_TASK_TEAMLOOP_MEMBERCOMP);
					return false;
				}
				if (tpList->mTaskNumber >= TASK_LIST_SIZE)
				{
					SendTeamLoopErrNotify( pPlayer, tpMember->GetCharName(), ERROR_TASK_TEAMLOOP_MEMBERFULL);
					return false;
				}
			}
			// 检查这个队员可不可以接
			if ( !IsTaskAcceptable(tpMember, tpList, vTaskID, nNpcID ) )
			{
				SendTeamLoopErrNotify( pPlayer, tpMember->GetCharName(), ERROR_TASK_TEAMLOOP_CANT_ACCEPT);
				return false;
			}
		}

		// 所有成员没问题 则遍历 并进行插入任务操作
		for (int i = 0 ; i < nMemberNum; ++i )
		{
			CEntityPlayer* tpMember = CSceneLogic::GetSingleton().GetPlayerByCharID(nTeamMember[i]);
			if (tpMember == NULL)
			{
				continue;
			}
			if (tpMember->GetEntityID() == pPlayer->GetEntityID())
				continue;
			PlayerFastObtainTask(tpMember, vTaskID, vColor);
		}
	}

	// 如果是掌门任务 额外判断 
	// 得到掌门ID (除非篡改客户端这个验证才能失败)
	unsigned short usChairManId = 0;
	if ( nTaskType == TASKTYPE_CHAIRMAN )
	{
		usChairManId = CTaskLuaInterface::Call<unsigned short, unsigned short>
			( pPlayer, pList, "get_chairman_id", vTaskID );

		if ( usChairManId == 0 || !pList->mChairManTaskMgr.CheckAcceptChairMan(usChairManId) )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_GOT_CHAIRMAN );
			pList->SetTalkNpcEntityID(0);
			return false;
		}
	}

	if(!bRet)
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_ACCEPTINVALID );
		pList->SetTalkNpcEntityID(0);
		return false;
	}

	// 库任务 特殊处理
	if ( nTaskType == TASKTYPE_STORAGE )
	{
		if ( pList->mStorageTaskMgr.GetGotNum() >= MAX_STORAGETASK_LIST_NUM )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_STORAGEMAX );
			pList->SetTalkNpcEntityID(0);
			return false;
		}
	}

	// 环任务 特殊处理
	if ( nTaskType == TASKTYPE_LOOP )
	{
		// 判断环任务冷却时间
		int nCDtime = CTaskLuaInterface::Call<int, unsigned short>(pPlayer, pList, "get_loop_task_punish_time", vTaskID );
		if ( (int)(time(0) - pList->mLoopTaskData.GetLastAbortTime()) < nCDtime )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_LOOP_CD_NOTEND );
			return false;
		}
	}

	// 掌门任务 特殊处理
	if ( nTaskType == TASKTYPE_CHAIRMAN )
	{
		pList->mChairManTaskMgr.ResetTasks( usChairManId ) ;
		pList->mChairManTaskMgr.OnAccepted();
	}

	//  取得任务需要发放的道具, 因为TestBaggageSpace的接口这样
	CItemsPair mExtendItems;
	PERF_FUNC( "GetTaskExtendItems" , GetTaskExtendItems( pPlayer, vTaskID, mExtendItems) );
	
	//  检查空间是否满足，如果不满足那么任务不能接，同时客户端也不会关闭任务对话界面
	if (mExtendItems.size() > 0 )
	{
		const int BAGGAGESPACE = 4;
		int mExtendItemID[BAGGAGESPACE] = {0};
		int mExtendItemNumber[BAGGAGESPACE] = {0}; 
		CItemsPair::iterator iter = mExtendItems.begin();
		for(int i = 0; iter != mExtendItems.end(); ++iter,++i)
		{
			mExtendItemID[i] = iter->first;
			mExtendItemNumber[i] = iter->second;
		}
		if ( pProperty->TestBaggageSpace( mExtendItemID, mExtendItemNumber, BAGGAGESPACE ) == false )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_NOSPACE );
			pList->SetTalkNpcEntityID(0);
			return false;
		}
	}
	
	LOG_DEBUG( "TASK", "player want to get task , id = %d", vTaskID);

	//  插入任务
	CTask vTask; 
	vTask.mTaskID = vTaskID;
	int tRet = pList->InsertTask(vTask);

	if ( tRet == ERROR_TASK_SUCCESS )
	{

		CTask* pTask = pList->GetTask(vTaskID);
		LK_ASSERT( pTask, return false );

		//  如果成功的获得任务，发放任务道具
		CItemsPair::iterator iter = mExtendItems.begin();
		for (; iter != mExtendItems.end() ; ++iter )
		{
			if ( iter->first == 0 )	break;

			CItemObject *pInsItemPile = 
				CPropertyModule::GetSingleton( ).PlayerInsertItem( pPlayer, iter->first, iter->second, true);

			if ( pInsItemPile != NULL)
			{
				LogEventGetItemByAcceptTask(pPlayer, pInsItemPile->GetGUID(), iter->first, iter->second, vTaskID);
			}

		}

		// 初始化一次任务数据
		CTaskLuaInterface::Call<int, CTask*>( pPlayer, pList, "init_this_task", pTask); //  初始化，并得到这个任务要杀怪的种类数

		// 通知客户端
		CMessageObtainTaskNotice msg;
		TaskStruct* tpTask = msg.mutable_task();
		pTask->GetTaskDataStruct(tpTask);
		if ( GetTaskType(pTask->mTaskID ) == TASKTYPE_LOOP ) // 是环任务
		{
			if ( msg.has_looptask() )
			{
				LOG_ERROR( "TASK", "player %s has loop err!", pPlayer->GetCharNameShow() );
			}
			else
			{
				LoopTaskStruct* tpLoop = msg.mutable_looptask();
				GetLoopTaskClientData( pPlayer, pList, tpLoop);
			}
		}

		CMessage   tMessage;
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_OBTAINTASK);
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );

		//  判断一次任务是否完成(强制刷新)
		FireEvent( pPlayer, 0 );
		bool bEnd;
		UpdateTaskProgress( pPlayer, pTask, bEnd, 1 );

		// 判断劫难任务 改状态
		// 如果是劫难任务 就设置相关状态
		if (GetTaskType(vTaskID) == TASKTYPE_TRAGEDY )
		{
			int			   tIndex = CTaskLuaInterface::Call<int, unsigned short>( pPlayer, pList, "get_tragedy_id", vTaskID );
			int			   tState = TRAGEDY_GOT;
			unsigned short tNextID = vTaskID;
			pList->mTragedyTaskMgr.SetNextTask(tIndex, tNextID);
			pList->mTragedyTaskMgr.SetState(tIndex, (enTragedyTaskState)tState);
			SendTragedyInfo2Client( pPlayer, &tIndex, &tNextID, &tState, 1 );
		}

		// 如果是库任务，发最新列表给客户端
		if ( GetTaskType(vTaskID) == TASKTYPE_STORAGE )
			SendStorageTaskList2Client( pPlayer, pList, pProperty->GetTalkNpcEntityID( ) );

		// 下面这个接口里需要判断是否正在邀请的任务 以便于处理护送NPC
		pList->SetInvitingPlayerID( nInvitingPlayerID );
		DoTaskObtAwards( pPlayer, pList, vTaskID );
		// 以下两行是实时数据 不用存盘 就算传走跨服了 下面的就不会存了 不存也没关系
		pList->SetInvitingPlayerID( -1 );
		pList->SetTalkNpcEntityID(0);

		// 取出组ID 如果不为0  要做处理 增加接受次数
		int tGroupID = CTaskLuaInterface::Call<int, unsigned short>( pPlayer, pList, "get_task_group_id", vTaskID );
		if ( tGroupID != 0 )
		{
			pList->mTaskGroupMgr.OnAcceptOneTask( tGroupID );
		}

		if ( GetTaskType(vTaskID) == TASKTYPE_PUBLISH )
		{
			pList->SetPublishTaskState(vTaskID,ACCEPTED_TASK, GetTaskPublishType(vTaskID));  // 改变任务发布状态
		}
	}
	else
	{
		pList->SetTalkNpcEntityID(0);
		pList->SetInvitingPlayerID( -1 );
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tRet );
	}

	return tRet == ERROR_TASK_SUCCESS;
}

//  玩家刷新任务奖励
bool CTaskModule::PlayerRefreshTaskAward(CEntityPlayer* pPlayer, unsigned short vTaskID,int mRefreshItemIndex, int nNpcEntityID )
{
	LK_ASSERT(pPlayer, return false);

	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false );
	
	// 不可接的任务
	if ( IsTaskAcceptable( pPlayer, pList, vTaskID, nNpcEntityID ) == false )
	{
		Send2ClientOprFailed(pPlayer);
		LOG_ERROR("TASK", "task is not acceptable!");
		//return false;
	}

	// 不可刷新的任务
	if ( IsRefreshAward(vTaskID) == false )  
	{
		Send2ClientUnRefreshed(pPlayer);
		LOG_ERROR("TASK", "task is unrefreshable!");
		return false;
	}

	// 如果是组队环任务 则判断是不是队长
	if (IsTeamLoop(vTaskID) == 1) 
	{
		CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject(CTeamModule::GetSingleton().InTeam( pPlayer->GetCharID() ) );
		if (tpTeam == NULL || tpTeam->GetHeaderID() != pPlayer->GetCharID())
		{
			LOG_ERROR("TASK","You are not the team leader!");
			return false;
		}
	}

	int vTaskColor = CTaskLuaInterface::Call<int, unsigned short>( pPlayer, pList, "get_refreshed_type", vTaskID );
	
	//  此处加上判断有效索引	
	CPropertyPlayer* pProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	LK_ASSERT( pProperty, return false );

	CItemObject* pObj = pProperty->GetBaggage()->GetItemObjPtr(mRefreshItemIndex);
	if (pObj == NULL)
	{
		LOG_ERROR("TASK", "load bag data failed  player entityID=%d  index=%d", pPlayer->GetEntityID(), mRefreshItemIndex );
		return false;
	}
	int64_t vGuID = pObj->GetGUID();
	int32_t vItemID = pObj->GetItemID();

	// 刷完任务扣除相应的刷新道具
	if (CPropertyModule::GetSingleton().PlayerRemoveItem( pPlayer, (unsigned int)mRefreshItemIndex, 1) == 0)
	{ 
		LogEventLostItemByUse( pPlayer, vGuID, vItemID, 1);	
		LOG_DEBUG("TASK","remove item success!");
	}
	else
	{
		LOG_ERROR("TASK",  "clear task(id=%d) item(%d)  from (roleid=%d) error", 
			vTaskID,  vItemID, pPlayer->GetCharID());
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_LESS_RECV_ITEM );
		Send2ClientOprFailed( pPlayer );
		return false;
	}
	int vTaskNumber = pList->InsertRefreshedTask(vTaskID, vTaskColor);
	Send2ClientRefreshResult(pPlayer, vTaskID, vTaskColor, vTaskNumber);

	return true;
}

//  玩家更新任务
bool CTaskModule::UpdateTaskProgress(CEntityPlayer* pPlayer, CTask* pTask, bool& bIsAutoEnd, int nSend , unsigned int nEventID, unsigned int nParam)
{
//  玩家更新任务， 任务状态仅仅在这里更新！！
//  nSend = 1 : 即便状态不改变也通知客户端！(用于道具进包,死怪等非常规更新)
//  常规更新：在接任务，交任务，放弃任务。。。地方调用的更新

	//Clock a;
	//a.start();
	// 如果玩家任务数据发生了变更，则发送消息
	if (pTask == NULL || pTask->mTaskID == 0)
	{
		LOG_ERROR("TASK", "task is null or task id = 0 in update task, pTask is %d", (int)pTask);
		return false;
	}

	int prev_state = pTask->mTaskProgress;

	// 要求：update_task函数里会更新任务状态，
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false );
	CTaskLuaInterface::Call<bool, CTask*>( pPlayer, pList, "update_task", pTask ); // 返回值还没想好怎么用

	//printf("--3.1--%d\n", a.elapsed() );
	//a.start();
	if( nSend == 1 )
	{
		NotifyClientUpTask(pPlayer, pTask, prev_state, nEventID, nParam);
	}
	//printf("--3.2--%d\n", a.elapsed() );
	//a.start();
	if (pTask->mTaskProgress != prev_state)
	{
		// 如果状态从COMPLETE到BEGIN，说明发生了回滚
		if( pTask->mTaskProgress == TASK_BEGIN && prev_state == TASK_COMPLETE)
		{
			CMessageResetTaskNotice msg;
			msg.set_taskid(pTask->mTaskID);
			CMessage   tMessage;
			tMessage.set_msgpara((uint32_t) &msg);
			tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_RESET);
			CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
		}

		// 如果任务状态是完成状态，则通知玩家任务完成
		if(pTask->mTaskProgress == TASK_COMPLETE)
		{
			// 干掉timer id  (新需求：完成不干掉定时器 交付才干掉)
			//if( pTask->mTimerID != INVALID_OBJ_ID )
			//{
			//	pPlayer->GetTimerReciver()->ClearTimer( pTask->mTimerID );
			//	pTask->mTimerID = INVALID_OBJ_ID;
			//}
#ifdef _FB_
			// 调用犀哥的接口
			CRepetionModule::GetSingleton().EndTask( pPlayer, pTask->mTaskID, true );
#endif
			if (CTaskLuaInterface::Call<bool,unsigned short>( pPlayer, pList,"is_direct_complete", pTask->mTaskID)) // 任务是直接完成的！
			{
				bIsAutoEnd = true;
				return true;
			}
			
			CMessageTaskCompleteNotice msg;
			msg.set_taskid(pTask->mTaskID);
			CMessage   tMessage;
			tMessage.set_msgpara((uint32_t) &msg);
			tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_COMPLETE);
			CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );

			// 如果要分享 则同屏玩家要置完成
			if ( IsCompleteShare( pTask->mTaskID ) == 1 )
			{
				CSeenPlayerContainer tSeenPlayers = pPlayer->GetSeenPlayersContainer();  // 得到同屏内玩家
				std::vector<unsigned int> tPlayers;
				for ( CSeenPlayerContainer::SeenPlayerVector::iterator it = tSeenPlayers.mPlayers.begin(); 
					it != tSeenPlayers.mPlayers.end(); ++it )
				{
					CEntityPlayer* tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity(*it);
					LK_ASSERT( tpPlayer != NULL, continue );
					if ( tpPlayer->GetNationality() == pPlayer->GetNationality() )
						tPlayers.push_back( *it );
				}
				OnForceTaskComplete( tPlayers, pTask->mTaskID );
			}
		}
		//PERF_FUNC("ReSendAllNpcTipInEye", ReSendAllNpcTipInEye(pPlayer, vpPlayerProperty) );
	}
	//printf("--3.3--%d\n", a.elapsed() );

	return true;
}

//  玩家结束任务
bool CTaskModule::PlayerEndTask( CEntityPlayer* pPlayer, unsigned short vTaskID, short vSelect, bool bIsAuto, int vBagIndex )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false );

	// 检查并更新一下任务状态
	CTask* pTask = pList->GetTask(vTaskID);
	if(pTask == NULL)
	{
		return false;
	}
	int ltime = time(0);										// 当前时间
	int vDelayNum = GetFinishDelayTime(vTaskID);				// 过多久才能交任务
	int LiftTime = pTask->mTaskBeginTime + vDelayNum - ltime;	// 交任务剩余时间
	if (vDelayNum > 0)
	{
		if ( pTask->GetTaskProgress() != TASK_COMPLETE )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_NOTCOMPLETE );
			return false;
		}

		if (LiftTime > 0)
		{
			CMessageUnFinishTaskTime msg;
			msg.set_taskid(vTaskID);
			msg.set_needtime(LiftTime);
			CMessage   tMessage;
			tMessage.set_msgpara((uint32_t) &msg);
			tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTIFY_UNFINISHTIMEERR);
			CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
			return false;
		}
	}
	if (vSelect != -100)
	{
		// 检查一下用户传进来的OptionID是否在允许的范围内
		int max_option = CTaskLuaInterface::Call<int, int>( pPlayer, pList, "get_max_award_option", vTaskID);

		if ( ((vSelect > max_option || vSelect < 1) && max_option != 0)
			|| (vSelect != 0 && max_option == 0) )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_MUTI_AWARD_ERROR );
			return false;
		}
	}

	CPropertyPlayer* pProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	LK_ASSERT( pProperty, return false );

	int nNum	= 0;
	// 检查传入的包裹索引
	if ( vBagIndex >= 0 )
	{
		CItemObject* pObj = pProperty->GetBaggage()->GetItemObjPtr(vBagIndex);
		if (pObj == NULL)
		{
			LOG_ERROR("TASK", "load bag data failed  player entityID=%d  index=%d", pPlayer->GetEntityID(), vBagIndex );
			return false;
		}
		// 到lua里判断到底该扣几个  如果是-1 表示不需要这个东西 -2 表示不够
		nNum = CTaskLuaInterface::Call<int, unsigned short, int, int>( pPlayer, pList, "check_recv_bag_item", vTaskID, pObj->GetItemID(), pObj->GetItemNum() );
		if (nNum > (int)pObj->GetItemNum())
		{
			LOG_ERROR("TASK", "clear more?? player = %s", pPlayer->GetCharNameShow() );
			return false;
		}

		if (nNum == -1)
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_BAD_RECV_ITEM );
			Send2ClientOprFailed( pPlayer );
			return false;
		}
		else if (nNum == -2)
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_LESS_RECV_ITEM );
			Send2ClientOprFailed( pPlayer );
			return false;
		}
		else if (nNum == 0)
		{
			LOG_ERROR("TASK", "check_recv_bag_item return 0");
			return false;
		}
	}
	else // 没有放东西上去 判断是不是确实要放东西
	{
		if (CTaskLuaInterface::Call<int, unsigned short>(pPlayer, pList, "is_need_give_item", vTaskID ) == 1 ) // 要放东西的
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_BAD_RECV_ITEM );
			Send2ClientOprFailed( pPlayer );
			return false;
		}
	}

	//  取得任务奖励的, 奖励的物品在程序中处理还是在lua中处理
	//  判断奖励需要的空间是否满足
	if ( !TestAwardNeedSpace(pPlayer, vTaskID, vSelect) )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_NOSPACE );

		return false;
	}

	DisposeEndTask(pPlayer, vTaskID, false, bIsAuto, vSelect, vBagIndex, nNum );

	return true;
	
}

bool CTaskModule::DisposeEndTask( CEntityPlayer* pPlayer, unsigned short vTaskID, bool bIsTeamMember, bool bIsAuto, int vSelect, int nIndex , int nClearNum )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false);

	CTask* pTask = pList->GetTask(vTaskID);
	LK_ASSERT( pTask!=NULL, return false);

	if ( (pTask->mExFlags&EXFLAG_PROTECT_FLAG_TASK) == EXFLAG_PROTECT_FLAG_TASK ) // 守旗任务 取消注册
	{
		mNpcMonitor.RemoveGlobalTaskAction( pTask->GetFirstParamByEvent(EVENT_PROTECT_FLAG)
			, pPlayer, pTask->mTaskID );
	}

	// 干掉定时器
	if( pTask->mTimerID != INVALID_OBJ_ID )
	{
		int tRet = pPlayer->GetTimerReciver()->ClearTimer( pTask->mTimerID );
		if ( tRet != 0 )
			LOG_ERROR("TASK", "OnTaskFailed del2 timer returned %d, timerid=%d taskid=%d"
			, tRet, pTask->mTimerID, vTaskID );

		pTask->mTimerID = INVALID_OBJ_ID;
		pTask->mGameTimeLeft = -1;
	}

	// 必须完成

	// 如果是组队环任务
	int nAwardNum=0;
	if (IsTeamLoop(vTaskID) == 1)
	{
		// 计数
		pList->PlusTodayTeamLoopNum();

		// 如果满20个了 给额外奖励
		//if (pList->GetTodayTeamLoopNum() == MAX_TEAMLOOP_EVERYDAY )
		CTaskLuaInterface::Call<int, int, int, int, int, int>(pPlayer, pList, "do_extra_awards", TEAMLOOP_ENDLOOP, vTaskID, pList->GetTodayTeamLoopNum(), 0, 0 );

		if (!bIsTeamMember)
		{
			//int nEndNum=0;
			// 看是不是队长
			CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject(CTeamModule::GetSingleton().InTeam( pPlayer->GetCharID() ) );
			if (tpTeam != NULL && tpTeam->GetHeaderID() == pPlayer->GetCharID())
			{
				// 遍历队员
				int nTeamMember[TEAMNUM] = {0}; // 角色ID
				int nMemberNum=0;
				int nAwardMember[TEAMNUM] = {0}; // 实体ID
				tpTeam->GetMemberList(nTeamMember, TEAMNUM, nMemberNum);
				LK_ASSERT(nMemberNum <= TEAMNUM, return false );
				for (int i = 0 ; i < nMemberNum; ++i )
				{
					CEntityPlayer* tpMember = CSceneLogic::GetSingleton().GetPlayerByCharID(nTeamMember[i]);
					if (tpMember == NULL)
					{
						continue;
					}
					if (tpMember->GetEntityID() == pPlayer->GetEntityID())
						continue;

					if ( pPlayer->IsWithinDistance( tpMember, TEAM_VALID_DISTANCE ) == false )
					{
						continue;
					}

					if ( tpMember->IsAlive() == false )
					{
						continue;
					}

					CTaskList* tpList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(tpMember->GetTaskToken( ));
					LK_ASSERT( tpList,  continue );
					CTask* pThisTask = tpList->GetTask(vTaskID);
					if (pThisTask != NULL && pThisTask->mTaskProgress == TASK_COMPLETE)
					{
						if (DisposeEndTask(tpMember, vTaskID, true))
						{
							//++nEndNum;
							nAwardMember[nAwardNum++] = tpMember->GetEntityID();
						}
					}
				}

				// 大家任务都交完了。。开始做额外奖励 这个数组没本人
				for (int i = 0; i < nAwardNum; ++i)
				{
					CEntityPlayer* tpMember = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity(nAwardMember[i]);
					if (tpMember == NULL)
					{
						continue;
					}

					CTaskList* tpList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(tpMember->GetTaskToken( ));
					LK_ASSERT( tpList,  continue );

					CTaskLuaInterface::Call<int, int, int, int, int, int>(tpMember, tpList, "do_extra_awards", TEAMLOOP_TEAMEXTRA, vTaskID, nAwardNum+1, 0, 0 );
				}
			}
		}
	}

	int tTaskType = GetTaskType(vTaskID);
	if ( tTaskType == TASKTYPE_LOOP )
	{
		// 触发环任务中的
		int tWeekTimes = pList->mLoopTaskData.OnCompletedTask(vTaskID);
		if ( tWeekTimes > 0 ) // 刚刚计数了本周的已完成次数 去lua做相应处理 奖励
		{
			int tRet = CTaskLuaInterface::Call<int, unsigned short, int>( pPlayer, pList, "on_weekly_end_loop_added", vTaskID, tWeekTimes );
			if ( tRet == 1 )
			{
				CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_ON_END_A_NEW_LOOP );
			}
		}

		FireEvent( pPlayer, EVENT_COMPLETE_LOOP, SPECIALID_COMPLETELOOPTASK );
	}

	bool bIsStorageTask = false;
	if ( tTaskType == TASKTYPE_STORAGE )
	{
		pList->mStorageTaskMgr.OnEndTask(vTaskID);
		// 刷新库任务
		pList->RefreshTasks(REFRESH_BACK);
		// fireevent
		FireEvent( pPlayer, EVENT_COMPLETESTORAGETASK, SPECIALID_COMPLETESTORAGE );
		bIsStorageTask = true;
	}

	//  刷新一遍身上的任务，弄不好有状态改变的(如果是自动完成的就算了，在外围会更新的)
	if (! bIsAuto)
	{
		FireEvent( pPlayer, EVENT_TASKCHANGE, vTaskID );
	}

	// 任务完成后，如果任务不能重复完成，需要在把任务放入完成列表
	if(! IsTaskRepeateAble(vTaskID))
	{
		LOG_DEBUG("player %s set taskend:%d", pPlayer->GetCharNameShow(), vTaskID);
		pList->SetEnd(vTaskID);
	}
	else
	{
		pList->SetEndTime(vTaskID,time(0));
	}

	CMessageTaskEndNotice msg;
	msg.set_taskid(vTaskID);
	if ( bIsStorageTask )
	{
		StorageTaskStruct* tpStorage = msg.mutable_storagetask();
		GetStorageTaskData( pPlayer, pList, tpStorage );
	}
	CMessage   tMessage;
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_TASKEND);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );

	// 如果是环任务的最后一环
	if ( tTaskType == TASKTYPE_LOOP && CTaskLuaInterface::Call<bool,unsigned short>(pPlayer, pList, "is_the_end_loop", vTaskID)) 
	{
		pList->mLoopTaskData.SetEndLoopData( );
		CTaskLuaInterface::Call<int, int, int>( pPlayer, pList, "init_loop_tasks" , pPlayer->GetMetier(), pPlayer->GetLevel() ); // 刷新环任务
		FireEvent( pPlayer, EVENT_ENDLOOP, 0 );
	}

	// 如果是劫难任务 就设置相关状态
	if ( tTaskType == TASKTYPE_TRAGEDY )
	{
		lua_tinker::table ret = CTaskLuaInterface::Call<lua_tinker::table, unsigned short>( pPlayer, pList, "get_next_tragedy", vTaskID );
		unsigned short tNextID = ret.get<unsigned short>("task_id");
		int			   tIndex  = ret.get<int>("index");

		if (tNextID == 0)
		{
			LOG_ERROR("TASK", "next tragedy = 0?? this id=%d", vTaskID );
		}
		else
		{
			pList->mTragedyTaskMgr.SetNextTask( tIndex, tNextID );
			LOG_DEBUG("TASK", "player%s save next index=%d, nextid=%d", pPlayer->GetCharNameShow(), tIndex, tNextID );
			int tState = TRAGEDY_ACTIVE;
			if (tNextID == vTaskID)
			{
				tState = TRAGEDY_COMPLETED; // 劫难完成了
				// 设置已完成的劫难数量
				pList->mTragedyTaskMgr.PlusCompleted();
				// 做额外奖励 如果做了 返回1 否则返回0
				if ( 1 == CTaskLuaInterface::Call<int, int>( pPlayer, pList, "on_completed_tragedy", pList->mTragedyTaskMgr.GetCompleted() ) )
				{
					CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_ON_END_TRAGEDY10);
				}
			}
			pList->mTragedyTaskMgr.SetState(tIndex, (enTragedyTaskState)tState);
			SendTragedyInfo2Client( pPlayer, &tIndex, &tNextID, &tState, 1 );
		}
	}

	// 如果是掌门任务 则修改状态
	if ( tTaskType == TASKTYPE_CHAIRMAN )
	{
		// 修改状态
		pList->mChairManTaskMgr.OnTaskEnd( ) ;
		
		// 额外奖励
		lua_tinker::table ret = CTaskLuaInterface::Call<lua_tinker::table, int, int>( pPlayer, pList, 
			"on_completed_chairmantask", pList->mChairManTaskMgr.GetCurID(), pList->mChairManTaskMgr.GetCompleteNum() ) ;
		if ( ret.get<bool>("is_finished") )
		{
			// 全部完成了 置为可接受状态
			pList->mChairManTaskMgr.ResetTasks( 0 ) ;
			//ResetChairManTasks(pPlayer, pList->mChairManTaskMgr.GetCurID() );
		}

		if ( ret.get<bool>("is_awarded") ) // 如果有奖励了。。。
		{
			CPropertyModule::GetSingleton().SendProErrorNotice(pPlayer, ERROR_TASK_CHAIRMAN_AWARDED );
		}
	}
	if (tTaskType == TASKTYPE_PUBLISH)
	{
		pList->SetPublishTaskState(vTaskID,FINISHED_TASK, GetTaskPublishType(vTaskID));  // 改变任务发布状态
	}

	// 从LUA获得任务需要的经验金钱(已经不再返回金钱经验！)
	lua_tinker::table data_ret = CTaskLuaInterface::Call<lua_tinker::table, unsigned short>( pPlayer, pList, "get_task_end_info", vTaskID );
	
	// 获取家族安定度奖励
	CFamilyModule::GetSingleton().OnTaksChangeStability( vTaskID, pPlayer );

	// 取出组ID 如果不为0  要做处理
	int tGroupID = data_ret.get<int>("groupid");
	if ( tGroupID != 0 )
	{
		pList->mTaskGroupMgr.OnEndOneTask( tGroupID );
	}

	// 设置特殊状态 为了以下的流程不处理
	pTask->mTaskProgress = TASK_TOEND;

	// 删除完成任务所收集的道具
	DropCompleteItems(pPlayer, vTaskID, false, nIndex , nClearNum );

	// 完成任务数量++
	CPropertyPlayer* pPro = (CPropertyPlayer*)pPlayer->GetProperty();
	LK_ASSERT(pPro!=NULL, return false);
	pPro->SetCompletedTaskNum(pPro->GetCompletedTaskNum()+1);

	//  将任务从列表中移除
	pList->RemoveTask(vTaskID);
	int vColor = pList->GetTaskColor(vTaskID);

	// 该加的数都加了再奖励 否则多倍就不对了
	// 【注意！！】这个必须放最后！因为里面可能执行传送跨服操作！
	PERF_FUNC("DoTaskAwards", DoTaskAwards( pPlayer, pList, vTaskID, vSelect,vColor) );
	if (IsTeamLoop(vTaskID) == 1)
		CTaskLuaInterface::Call<int, int, int, int, int, int>(pPlayer, pList, "do_extra_awards", TEAMLOOP_TEAMEXTRA, vTaskID, nAwardNum+1, 0, 0 );

	for (int i = 0; i < TASK_LIST_SIZE; ++ i)
	{
		if (pList->mRememberRefreshNum[i].rTaskID == vTaskID)
		{
			pList->RemoveTaskColor(vTaskID);
			Send2ClientRemoveTaskColor(pPlayer, vTaskID, vColor);
			break;
		}
	}

	return true;
}

// 发送劫难任务更新信息到客户端
void CTaskModule::SendTragedyInfo2Client( CEntityPlayer* pPlayer, int *anIndex, unsigned short *asNextID, int *anState, int n, int nIsFirst )
{
	CMessageTragedyTaskUpdateNotice msg;
	CMessage   tMessage;

	for (int i=0; i<n; ++i)
	{
		TragedyStruct *pSt = msg.add_tragedyinfo();
		pSt->set_nexttragedytask( asNextID[i] );
		pSt->set_tragedyindex( anIndex[i] );
		pSt->set_tragedystate( anState[i] );
	}

	msg.set_islogin(nIsFirst);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_TRAGEDYTASKUPDATE);
	tMessage.set_msgpara((uint32_t) &msg);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}

// 任务奖励
bool CTaskModule::DoTaskAwards(CEntityPlayer* pPlayer, CTaskList* pList,unsigned short vTaskID, unsigned short vSelect,int vTaskColor)
{
	CTaskLuaInterface::mCurTaskID = vTaskID;
	return CTaskLuaInterface::Call<bool, unsigned short, unsigned short, int>(pPlayer, pList, "do_task_awards", vTaskID, vSelect, vTaskColor);
}

bool CTaskModule::DoTaskObtAwards( CEntityPlayer* pPlayer, CTaskList* pList, unsigned short vTaskID)
{
	return CTaskLuaInterface::Call<bool, unsigned short, unsigned int>( pPlayer, pList, "do_task_obt_awards", vTaskID, pPlayer->GetEntityID() );
}

bool CTaskModule::GetTaskExtendItems(CEntityPlayer* pPlayer, unsigned short vTaskID, CItemsPair& vItemsPair)
{
	LK_ASSERT( pPlayer , return false );

	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false );

	// 接受任务的奖励物品也会出现在这里！！
	lua_tinker::table ret = CTaskLuaInterface::Call<lua_tinker::table, unsigned short>( pPlayer, pList, "get_items_extend", vTaskID);
	
	int n = luaL_getn(L, -1);
	for (int i = 1; i<= n; ++i)
	{
		lua_rawgeti(L, -1, i);
		lua_tinker::table inside = lua_tinker::pop<lua_tinker::table>(L);

		int id = inside.get<int>("id");
		int m =  inside.get<int>("n");
		
		vItemsPair.push_back( std::make_pair<int, int>( id, m));
	}

	return true;
}

bool CTaskModule::TestAwardNeedSpace(CEntityPlayer* pPlayer, unsigned short vTaskID, unsigned short vSelectID)
{
	CItemsPair vItemsPair;
	bool bRet = false;

	PERF_FUNC( "GetTaskAwardItems", bRet = GetTaskAwardItems( pPlayer, vTaskID, vItemsPair, vSelectID) );

	if (bRet)
	{
		unsigned short n = vItemsPair.size();
		int vItemID[24] = {0};
		int vNumber[24] = {0};

		if (n > 0 && n <= 24)
		{
			CItemsPair::iterator iter = vItemsPair.begin();

			for(int i = 0; iter != vItemsPair.end(); ++iter,++i)
			{
				vItemID[i] = iter->first;
				vNumber[i] = iter->second;
			}

			CPropertyPlayer* pProperty = (CPropertyPlayer*)pPlayer->GetProperty();
			return pProperty->TestBaggageSpace(&vItemID[0], &vNumber[0], n);
		}
		else
		{
			return n == 0;
		}
	}

	return true;
}

// 传入NPC 模板ID  如果不是0， 就要验证  因为是从接任务过来的 不可能是0 
bool CTaskModule::IsTaskAcceptable( CEntityPlayer* pPlayer, CTaskList* pTaskList, unsigned short vTaskID, int vNpcTempID )
{
	return CTaskLuaInterface::Call<bool, unsigned short, int>(pPlayer, pTaskList, "check_acceptable", vTaskID, vNpcTempID );
}

// 简单版本的检查任务是否可接 仅检查函数满足  目前只用于任务搜索
bool CTaskModule::IsTaskAcceptableExpress( CEntityPlayer* pPlayer, CTaskList* pTaskList, unsigned short vTaskID )
{
	return CTaskLuaInterface::Call<bool, unsigned short>(pPlayer, pTaskList, "check_acceptable_express", vTaskID );
}

bool CTaskModule::GetTaskAwardItems( CEntityPlayer* pPlayer, unsigned short vTaskID, CItemsPair& vItemsPair, unsigned short vSelectID)
{
	LK_ASSERT( pPlayer , return false );

	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false );
	int vColor = pList->GetTaskColor(vTaskID);
	lua_tinker::table ret = CTaskLuaInterface::Call<lua_tinker::table, unsigned short, unsigned short>( pPlayer, pList, "get_awards_items", vTaskID, vSelectID,vColor);
	
	int n = luaL_getn(L, -1);
	for (int i = 1; i<= n; ++i)
	{
		lua_rawgeti(L, -1, i);
		lua_tinker::table inside = lua_tinker::pop<lua_tinker::table>(L);

		int id = inside.get<int>("id");
		int m =  inside.get<int>("n");
		
		vItemsPair.push_back(std::make_pair<int, int>(id, m));
	}

	return true;
}
bool CTaskModule::GetTaskCompleteItems(CEntityPlayer* pPlayer, unsigned short vTaskID, CItemsPair& vItemsPair)
{
	LK_ASSERT( pPlayer , return false );

	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false );

	lua_tinker::table ret = CTaskLuaInterface::Call<lua_tinker::table, unsigned short>( pPlayer, pList, "get_collect_items", vTaskID);
	
	int n = luaL_getn(L, -1);
	for (int i = 1; i<= n; ++i)
	{
		lua_rawgeti(L, -1, i);
		lua_tinker::table inside = lua_tinker::pop<lua_tinker::table>(L);

		int id = inside.get<int>("id");
		int n =  inside.get<int>("n");
		
		vItemsPair.push_back( std::make_pair<int, int>( id, n));
		LOG_DEBUG( "TASK", "get complete item %d : %d->%d ", i, id, n);
	}
	
	return true;
}

// 清除物品 先判断有没有，没有直接返回false
bool CTaskModule::DropItems(CEntityPlayer* pPlayer, const CItemsPair& vItemsPair, int nTaskID )
{
	// 清除
	for ( CItemsPair::const_iterator iter = vItemsPair.begin(); iter != vItemsPair.end(); ++iter )
	{
		if ( iter->first != 0 )
		{
			uint64_t ullGUID = 0;
			bool tBind = false;
			if ( CPropertyModule::GetSingleton( ).PlayerRemoveItemByID( pPlayer, iter->first, iter->second, tBind, &ullGUID) == 0)
			{
				LogEventLostItemByAbortTask(pPlayer, ullGUID, iter->first, iter->second, nTaskID);
			}
		}
	}
	return true;
}

void CTaskModule::DropCompleteItems(CEntityPlayer* pPlayer, unsigned short vTaskID, bool aborted, int nIndex , int nClearNum)
{
	//  删除任务发放的道具还有任务完成需要的任务道具,只在删除任务包裹中的道具
	//const int COMPLETEITEMSIZE = 8;

	CItemsPair vItemsPair;
	PERF_FUNC( "GetTaskCompleteItems", GetTaskCompleteItems( pPlayer, vTaskID, vItemsPair) );

	LOG_DEBUG( "TASK", "get clear items total=%d", vItemsPair.size());

	//if((int)vItemsPair.size() > COMPLETEITEMSIZE)
	//{
	//	return;
	//}

	CItemsPair::iterator iter = vItemsPair.begin();

	// 取得与任务相关的完成需要的物品
	for (; iter != vItemsPair.end(); ++iter )
	{
		if ( iter->first != 0 )
		{
			CTplItem* tpItemTpl = (CTplItem*) CDataStatic::SearchTpl( iter->first );
			if ( tpItemTpl != NULL )
			{
				//  放弃的时候只清除任务道具！
				if ( IS_TASK_ITEM( tpItemTpl ) || !aborted )
				{
					LOG_DEBUG( "TASK", "clear item , ID=%d, num=%d", iter->first, iter->second );
					uint64_t ullGUID = 0;
					bool tBind = false;
					int nLeft = CPropertyModule::GetSingleton( ).PlayerRemoveItemByID( pPlayer, iter->first, iter->second, tBind, &ullGUID);
					if ( aborted )
					{
						if ( iter->second - nLeft  != 0)
						{
							LogEventLostItemByAbortTask( pPlayer, ullGUID, iter->first, iter->second - nLeft, vTaskID);
						}
					}else
					{
						if ( iter->second - nLeft  != 0)
						{
							LogEventLostItemByEndTask( pPlayer, ullGUID, iter->first, iter->second - nLeft, vTaskID);
						}
					}
				}
			}
		}
	}

	// 清除玩家交的可选物品
	if ( nIndex >= 0)
	{
		CItemObject* tpSrcItem = ((CPropertyPlayer*) pPlayer->GetProperty())->GetBaggage( )->GetItemObjPtr( nIndex );

		if ( tpSrcItem != NULL)
		{
			uint64_t ullGUID = tpSrcItem->GetGUID();
			int nItemID = tpSrcItem->GetItemID();
			if (CPropertyModule::GetSingleton().PlayerRemoveItem( pPlayer, (unsigned int)nIndex, nClearNum) ==0)
			{
				LogEventLostItemByEndTask(pPlayer, ullGUID, nItemID, nClearNum, vTaskID);
			}else
			{
				LOG_ERROR("TASK",  "clear task(id=%d) item(%d) num(%d) from (roleid=%d) error", 
							vTaskID, nItemID, nClearNum, pPlayer->GetCharID());
			}
		}
	}

}

// =========================================================服务器接口====================
void CTaskModule::UpdateTaskWrapper( CEntityPlayer* pPlayer, unsigned int nEventID, unsigned int nParam, int nSend )
// 这个接口仅仅用于更新玩家身上的所有任务！在有东西发生变化的时候调用
// TODO: 加上task id , tick id 以提高效率？？
// 笨接口只能供fireevent调用
{
	LK_ASSERT( pPlayer , return );

	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );


	int tTaskNum = pList->mTaskNumber;

	std::vector<unsigned short> nAutoEndIDs;
	for ( int i = 0; i < tTaskNum; i ++ )
	{
		bool bAutoEnd = false;
		if ( pList->mTaskList[i].IsEventTick( nEventID, nParam ) || nEventID <= 0 )
		{
			if ( pList->mTaskList[i].mTaskProgress == TASK_TOEND || pList->mTaskList[i].mTaskProgress == TASK_FAILED ) // 特殊状态 和失败状态 不处理
				continue;

			// 解决守旗任务BUG：如果是守旗任务 则如果完成  就不再刷状态了
			int nNpcProtect = pList->mTaskList[i].GetFirstParamByEvent(EVENT_PROTECT_FLAG);
			if ( nNpcProtect != 0 && pList->mTaskList[i].mTaskProgress == TASK_COMPLETE )
				continue;

			// 记录日志
			LogEventTaskAction( pPlayer, pList->mTaskList[i].mTaskID, EM_TASK_ACTION, nEventID, nParam );

			if ( (nEventID & EVENT_MASK_REGINTASK ) == EVENT_MASK_REGINTASK )
			{
				// 返回加完以后已经有了的数量
				int tRet = CTaskLuaInterface::Call<int, CTask*, unsigned int, int>( pPlayer, pList, "set_colletor_value", &(pList->mTaskList[i]), nParam, 1 ); 
				int nNotifyFlag = 0; // 通知客户端的特殊任务类型标志
				if ( nEventID == EVENT_KILLFOREIGNER )
					nNotifyFlag = CONDITION_KILL_FOREIGNER;
				else if ( nEventID == EVENT_COMPLETESTORAGETASK )
					nNotifyFlag = CONDITION_COMPLETE_STORAGE;
				else if ( nEventID == EVENT_COMPLETE_LOOP )
					nNotifyFlag = CONDITION_COMPLETE_LOOP;
				else if ( nEventID == EVENT_COMPLETE_QUESTION )
					nNotifyFlag = CONDITION_COMPLETE_QUESTION;

				if ( nNotifyFlag != 0 )
				{
					CMessageUpdateTaskConditions msg;
					TaskCondition* pCondition = msg.add_contions();
					pCondition->set_taskid( pList->mTaskList[i].mTaskID );
					pCondition->set_tempid( nNotifyFlag );
					pCondition->set_num( tRet);
					pCondition->set_condition( CONDITION_OGREDIE );
					SendUpdateConditions2Client( pPlayer, &msg );
				}
			}
			PERF_FUNC("UpdateTaskProgress", UpdateTaskProgress(pPlayer, &(pList->mTaskList[i]), bAutoEnd, nSend, nEventID, nParam ) );

			if ( bAutoEnd ) // 这个任务要自动完成
				nAutoEndIDs.push_back( pList->mTaskList[i].mTaskID );
		}
	}

	for ( std::vector<unsigned short>::iterator it = nAutoEndIDs.begin(); it != nAutoEndIDs.end(); ++it )
	{
		PERF_FUNC( "PlayerEndTask", PlayerEndTask( pPlayer, *it, -100, true ) );
		LogEventPlayerEndTask( pPlayer, *it);
	}

}
void CTaskModule::OnPlayerChangeMoney(CEntityPlayer* pPlayer)
{
	FireEvent( pPlayer, EVENT_CHANGEMONEY );
}
void CTaskModule::OnTaskObtainItem( CEntityPlayer* pPlayer, unsigned int vItemID, unsigned short vNumber )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );

	CTplItem *tpItem = (CTplItem *)CDataStatic::SearchTpl( vItemID );
	LK_ASSERT( tpItem, return);
	if ( IS_TASK_ITEM( tpItem ) )
	{
		for ( int i = 0; i < pList->mTaskNumber; i ++ )
		{
			if ( pList->mTaskList[i].IsEventTick( EVENT_ITEMCHANGE, vItemID ) )
			{
				if ( pList->mTaskList[i].mWatchEventN <= vNumber )
				{
					pList->mTaskList[i].mWatchEventN = 0;
				}
				else
				{
					pList->mTaskList[i].mWatchEventN -= vNumber;
				}
				break; // 任务物品不会重复利用
			}
		}
	}

	FireEvent( pPlayer, EVENT_ITEMCHANGE, vItemID );
}

// 玩家点击NPC的时候
void CTaskModule::OnPlayerClickNpc( CEntityPlayer* pPlayer, int vEntityID)
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );

	CEntity* pEntity = CSceneLogic::GetSingleton().GetEntity(vEntityID);
	LK_ASSERT( pEntity, return );
	if( pEntity->IsFunNpc() || pEntity->IsOgre() ) 
	{	
		pList->SetTalkNpcEntityID(vEntityID);
		CEntityNpc* pEntityNpc	   = (CEntityNpc*) pEntity;
		CPropertyNPC* pPropertyNpc = (CPropertyNPC*)pEntityNpc->GetProperty();

		CTemplateNpc* tpNpcTpl = (CTemplateNpc*) CDataStatic::SearchTpl( pPropertyNpc->GetPropertyTempID() );
		LK_ASSERT ( tpNpcTpl != NULL, return );

		// 判断皇榜任务
		if ( tpNpcTpl->mStorageTaskNPC == 1 ) // 是皇榜NPC
		{	
			SendStorageTaskList2Client( pPlayer, pList, vEntityID );
			pList->SetTalkNpcEntityID(0);
			return;
		}

		CMessageNpcTaskListNotice msg;
		NpcTaskList* tpList = msg.mutable_list();
		CMessage   tMessage;
		tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_NPCTASKLIST);

		bool bRet = false;
		PERF_FUNC("GetNpcTaskList", bRet = GetNpcTaskList( pPlayer, pList, pEntityNpc->GetNpcID( ), tpList) );

		if (bRet)
		{
			LOG_DEBUG( "TASK", "prepear send player NPC task list, NPC ID=%d extend=%d, back=%d"
				, tpList->npcid(), tpList->extendnumber(), tpList->callbacknumber() );

			tMessage.set_msgpara((uint32_t) &msg);
			CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
		}
		else
		{
			// 记录取得NPC任务列表失败消息
			LOG_ERROR( "TASK", "error in get npc task list, npcid=%d", pEntityNpc->GetNpcID( ));
		}


		if ( pPropertyNpc == NULL )
		{
			LOG_ERROR("TASK", "npc property is null, line=%d", __LINE__ );
			pList->SetTalkNpcEntityID(0);
			return;
		}
		pList->SetTalkNpcEntityID(0);
	}
}

// 发送库任务列表
void CTaskModule::SendStorageTaskList2Client( CEntityPlayer* pPlayer, CTaskList* pList, int vEntityID )
{
	// 判断皇榜任务
	CEntity* pEntity = CSceneLogic::GetSingleton().GetEntity(vEntityID);
	if ( pEntity == NULL ) // 这个是有可能的
		return;

	if( pEntity->IsFunNpc() == false )
	{
		return ;
	}

	CPropertyNPC* pPropertyNpc = (CPropertyNPC*)pEntity->GetProperty();

	if ( pPropertyNpc == NULL )
	{
		LOG_ERROR("TASK", "npc property is null, line=%d", __LINE__ );
		return;
	}

	CTemplateNpc* tpNpcTpl = (CTemplateNpc*) CDataStatic::SearchTpl( pPropertyNpc->GetPropertyTempID() );
	if ( tpNpcTpl == NULL )
		return ;

	if ( tpNpcTpl->mStorageTaskNPC == 1 && pPlayer->GetLevel() >= tpNpcTpl->mDownLevel && pPlayer->GetLevel() <= tpNpcTpl->mUpLevel )
	{
		CMessageNpcStorageTaskListNotice msg2;
		CMessage   tMessage;
		NpcTaskList* tpList2 = msg2.mutable_list();
		pList->GetStorgeTaskList( tpList2, tpNpcTpl->mStorageTaskID );
		tpList2->set_npcid( pPropertyNpc->GetPropertyTempID() );
		tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_NPCSTORAGETASKLIST);
		tMessage.set_msgpara((uint32_t) &msg2);
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
	}
}


void CTaskModule::OnTaskRemoveItem (CEntityPlayer* pPlayer, unsigned int vItemID, unsigned short vNumber )
{
	FireEvent( pPlayer,  EVENT_ITEMCHANGE, vItemID );
}


// 注册任务事件
void CTaskModule::RegTaskEventTick( CTask* pTask )
{
	CTaskLuaInterface::Call<bool, CTask*>( NULL, NULL, "reg_task_tick", pTask ); // 这个肯定之前设置过了
}


// 注册任务与等级索引
void CTaskModule::RegLevelTask( unsigned short usLevel, unsigned short usTaskID )
{
	enTaskType tType = (enTaskType)GetTaskType(usTaskID ) ;
	if( usLevel > 0 && usLevel <= MAX_GRADE )
	{
		if (tType==TASKTYPE_MAIN)
			mLevelTasks[0][ usLevel ][mLevelTasksN[0][ usLevel ]++] = usTaskID ;
		else if (tType==TASKTYPE_LATERAL)
			mLevelTasks[1][ usLevel ][mLevelTasksN[1][ usLevel ]++] = usTaskID ;
		else if (tType==TASKTYPE_WEST)
			mLevelTasks[2][ usLevel ][mLevelTasksN[2][ usLevel ]++] = usTaskID ;
	}
}

// 注册掉落表
void CTaskModule::RegDropTable( unsigned short usTaskID, int nOgreID, int nItemID, int n, int nP, int nMaxn )
{
	mTaskDropMgr.RegDrop( usTaskID, nOgreID, nItemID, n, nP, nMaxn );
}


// 供外部调用的获得NPC TIP接口
unsigned short CTaskModule::GetNpcTip( CEntityPlayer* pPlayer, int nNpcTempID, int nNpcEntityID )
{
	if ( pPlayer == NULL )
		return TASKTIP_NONE;

	CPropertyPlayer* pProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	if ( pProperty == NULL )
		return TASKTIP_NONE;

	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return TASKTIP_NONE );

	unsigned short tip = TASKTIP_NONE;
	
	PERF_FUNC("GetNpcTip", tip = GetNpcTip( pPlayer, nNpcTempID, pList, nNpcEntityID ) );
	return tip;

}

// 计算头顶
unsigned short CTaskModule::GetNpcTip( CEntityPlayer* pPlayer, int nNpcID, CTaskList* pTaskList, int nNpcEntityID )
{
	LK_ASSERT( pPlayer !=NULL,   return TASKTIP_NONE );
	LK_ASSERT( pTaskList !=NULL, return TASKTIP_NONE );

	CEntityNpc* tpEntityNpc = dynamic_cast<CEntityNpc*>(CSceneObjManager::GetSingletonPtr()->GetObject( nNpcEntityID ) );
	if ( tpEntityNpc == NULL )
		return TASKTIP_NONE;

	// 只有友好才能有头顶
	if ( !( tpEntityNpc->IsFunNpc() && tpEntityNpc->IsFriendlyTo(pPlayer) ) )
	{
		return TASKTIP_NONE;
	}

	pTaskList->SetTalkNpcEntityID(nNpcEntityID);
	unsigned short tip = CTaskLuaInterface::Call<unsigned short, int>( pPlayer, pTaskList, "get_npc_tip", nNpcID );

	if ( tip == TASKTIP_ACTIVE || tip == TASKTIP_COMPLETE )
	{
		pTaskList->SetTalkNpcEntityID(0);
		return tip;
	}

	CTemplateNpc* tpNpcTpl = (CTemplateNpc*) CDataStatic::SearchTpl( nNpcID );
	if ( tpNpcTpl == NULL )
	{
		pTaskList->SetTalkNpcEntityID(0);
		return tip;
	}

	// 判断皇榜任务
	if ( tpNpcTpl->mStorageTaskNPC == 1 && pPlayer->GetLevel() >= tpNpcTpl->mDownLevel && pPlayer->GetLevel() < tpNpcTpl->mUpLevel ) // 刷新皇榜任务头顶
	{	
		if ( pTaskList->mStorageTaskMgr.GetTaskNum() > 0 || ( pTaskList->mStorageTaskMgr.GetTaskNum() == 0 && pTaskList->mStorageTaskMgr.GetGotNum() == 0 ) )
		{
			pTaskList->SetTalkNpcEntityID(0);
			return TASKTIP_ACTIVE;
		}
		else
		{
			pTaskList->SetTalkNpcEntityID(0);
			return TASKTIP_NONE;
		}
	}

	pTaskList->SetTalkNpcEntityID(0);
	return tip;
}

// 触发事件
void CTaskModule::FireEvent( CEntityPlayer* pPlayer, unsigned int nEventID, unsigned int nParam )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );

	//LOG_NOTICE("TASK", "player %s(%d) ticked %x ! param =%d", pPlayer->GetCharNameShow(), pPlayer->GetCharID(), nEventID, nParam);

	// 判断全局任务
	if ( ( nEventID & EVENT_MASK_GLOBAL ) == EVENT_MASK_GLOBAL && ( pList->mTickEvents & (1<<(nEventID&EVENT_MASK_ID)) )==0 )
	{
		return;
	}

	if ( ( nEventID & EVENT_MASK_RECORD ) == EVENT_MASK_RECORD ) // 需要记录事件的
	{
		pList->mEventMgr.RegEvent( nEventID, nParam );
	}

	if ( nEventID == EVENT_METIERCHANGE ) // 转职要特殊处理
	{
		pList->mLoopTaskData.Clear(); // 清除环任务数据
		ClearTasksByType( pPlayer, TASKTYPE_LOOP );
		CTaskLuaInterface::Call<int, int, int>( pPlayer, pList, "init_loop_tasks" , pPlayer->GetMetier(), pPlayer->GetLevel() ); // 刷新环任务
	}

	if ( ( nEventID & EVENT_MASK_NOTIFY ) == EVENT_MASK_NOTIFY )
	{
		PERF_FUNC( "UpdateTaskWrapper", UpdateTaskWrapper( pPlayer, nEventID, nParam, 1 ) );
	}
	else
	{
		PERF_FUNC( "UpdateTaskWrapper", UpdateTaskWrapper( pPlayer, nEventID, nParam, 0 ) );
	}
}

// 玩家使用任务道具  返回错误码
int CTaskModule::UseTaskItem( CEntityPlayer* pEntity, unsigned short vIndex, int &tItemID )
{
	CPropertyPlayer* pProperty = (CPropertyPlayer*)pEntity->GetProperty();

	CItemObject* tpSrcItem = (CItemObject*)pProperty->GetTaskBaggage( )->GetItemObjPtr( vIndex );

	if ( tpSrcItem == NULL )
		return ERROR_ITEMNOTEXIST;
	CTplItemTask *tpItem = (CTplItemTask *)CDataStatic::SearchTpl( tpSrcItem->GetItemID() );
	tItemID = tpSrcItem->GetItemID();

	if ( tpItem == NULL )
		return ERROR_TPLNOTEXIST;
	else if ( IS_NOT_TASK_ITEM( tpItem ) )
		return ERROR_NOTTASKIITEM;
	else if ( tpItem->mUseTimes == 0 )
		return ERROR_CANTUSE;

	else if ( (!(tpItem->mX == -1 && tpItem->mY == -1)) && (!( pEntity->GetPosX() >= tpItem->mX - tpItem->mWide/2 && pEntity->GetPosX() <= tpItem->mX + tpItem->mWide/2 
		&& pEntity->GetPosY() >= tpItem->mY - tpItem->mHeight/2 && pEntity->GetPosY() <= tpItem->mY + tpItem->mHeight/2 ) ) )
		return ERROR_OUTOFRANGE;

	else if ( pEntity->GetMapID() != tpItem->mMapID )
		return ERROR_OUTOFRANGE;

	// TODO 国家判断
	//if ( pEntity->GetLineID() != pEntity->mCharProfile.mNationality )
	//	return ERROR_OUTNATION;
	
	if ( tpItem->mGiveItemID != 0 ) // 要发放道具
	{
		int n = 1;
		if ( pProperty->TestBaggageSpace( &tpItem->mGiveItemID, &n, 1) )
		{
			CItemObject *pInsItemPile = 
				CPropertyModule::GetSingleton().PlayerInsertItem( pEntity, tpItem->mGiveItemID, 1, false );

			if (pInsItemPile != NULL)
			{
				LogEventGetItemByTaskItem(pEntity, pInsItemPile->GetGUID(), tpItem->mGiveItemID, 1, 0, tItemID);
			}
		}
		else
			return ERROR_BAGGAGEFULL;
	}

	tpSrcItem->SetUsedTimes( tpSrcItem->GetUsedTimes() + 1 );

	if ( tpSrcItem->GetUsedTimes() >= tpItem->mUseTimes ) // 用完了
	{
		// 删除1个道具
		uint64_t ullGUID = tpSrcItem->GetGUID();
		int nTplID = tpSrcItem->GetItemID();

		if (CPropertyModule::GetSingleton().PlayerRemoveItem( ( CEntityPlayer* )pEntity, vIndex, 1, NULL, true) == 0)
		{
			LogEventLostItemByUse(( CEntityPlayer* )pEntity, ullGUID, nTplID, 1);
		}

		tpSrcItem->SetUsedTimes( 0 );
	}
	FireEvent( pEntity, EVENT_USEITEM, tpSrcItem->GetItemID() );
	
	return SUCCESS;
}


// 任务失败(传入的player可能是null)
void CTaskModule::OnTaskFailed( CEntityPlayer* pPlayer, unsigned short usTaskID, bool bForce, bool bRemoveNpcMonitor )
{
	if ( pPlayer == NULL )
		return;

	LOG_DEBUG("TASK", "in OnTaskFailed player: %s ", pPlayer->GetCharNameShow() ); 

	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );

	CTask* pTask = pList->GetTask( usTaskID );
	if (pTask == NULL)
		return;

	// 如果不是强制处理 那么只有非BEGIN的时候才往下走
	if ( !bForce && pTask->mTaskProgress != TASK_BEGIN )
	{
		return;
	}

	if (bRemoveNpcMonitor)
	{
		if ( (pTask->mExFlags&EXFLAG_PROTECT_FLAG_TASK) == EXFLAG_PROTECT_FLAG_TASK ) // 守旗任务 取消注册
		{
			mNpcMonitor.RemoveGlobalTaskAction( pTask->GetFirstParamByEvent(EVENT_PROTECT_FLAG)
				, pPlayer, usTaskID );
		}	
	}

	// 干掉timer id
	if( pTask->mTimerID != INVALID_OBJ_ID )
	{
		int tRet = pPlayer->GetTimerReciver()->ClearTimer( pTask->mTimerID );
		if ( tRet != 0 )
			LOG_ERROR("TASK", "OnTaskFailed del timer returned %d, timerid=%d taskid=%d", tRet, pTask->mTimerID, usTaskID );

		LOG_DEBUG("TASK", "OnTaskFailed del timer returned %d, timerid=%d taskid=%d", tRet, pTask->mTimerID, usTaskID );
		pTask->mTimerID = INVALID_OBJ_ID;
		pTask->mGameTimeLeft = -1;
	}

	// 如果任务失败后不能重试的,则加入mCompleteTasks 列表
	if( IsTaskRetryAble(usTaskID) == 0 )
	{
		pList->SetFailed( usTaskID );
	}

	pTask->SetTaskProgress( TASK_FAILED );

#ifdef _FB_
	// 调用犀哥的接口
	CRepetionModule::GetSingleton().EndTask( pPlayer, pTask->mTaskID, false );
#endif

	CMessageTaskFailNotice msg;
	msg.set_taskid( usTaskID );
	CMessage   tMessage;
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_TASKFAIL);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );

	LOG_DEBUG("TASK", "in OnTaskFailed end player: %s ", pPlayer->GetCharNameShow() ); 
	// 如果失败自动放弃。。。。
	if ( IsAutoAbort( usTaskID) == 1 )
	{
		PlayerAbortTask( pPlayer, usTaskID, false, true );
		LogEventPlayerCancelTask(pPlayer, usTaskID,2);
	}
}

// 玩家使用RMB道具刷新库任务
bool CTaskModule::OnRefreshStorageTask( CEntityPlayer* pPlayer )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false );

	int tRefreshLmt = lua_tinker::get<int>( L, "rmb_refresh_storage_lmt");
	if (pList->mStorageTaskMgr.GetTodayRMBTimes() >= tRefreshLmt)
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_TASK_MAX_RMB_REFRESH); 
		return false;
	}
	else
	{
		pList->mStorageTaskMgr.PlusTodayRMBTimes();
	}

	pList->RefreshTasks( REFRESH_RMB );
	CPropertyPlayer* pProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	SendStorageTaskList2Client( pPlayer, pList,  pProperty->GetTalkNpcEntityID() );
	return true;
}

// 刷新库任务 返回数量
int CTaskModule::RefreshStorageTask( unsigned short usTaskID, ETaskRefreshType etype, unsigned short* pOutArr )
{
	// 下面这个lua确定不需要外部数据
	lua_tinker::table ret = CTaskLuaInterface::Call<lua_tinker::table, int, int>( NULL, NULL, "refresh_storge_tasks", usTaskID, etype );

	int n = luaL_getn( L, -1); //min( luaL_getn( L, -1), MAX_STORAGETASK_LIST_NUM );

	for ( int i = 1; i <= n; ++i)
	{
		lua_rawgeti(L, -1, i);
		unsigned short a = lua_tinker::pop<unsigned short>(L);
		*pOutArr++ = a; // lua_tinker::pop<unsigned short>(L);
		LOG_DEBUG("TASK", "refresh %d  style=%d", a, etype);
	}
	LOG_DEBUG("TASK", "refresh %d end! num=%d", usTaskID, n );
	return n;
}


// 给犀哥加的接口 判断某任务是否在玩家身上
bool CTaskModule::IsTaskOnPlayer( CEntityPlayer* pPlayer, unsigned short usTaskID )
{
	if ( pPlayer == NULL )
		return false;

	// 取得玩家当前的任务列表
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false );

	return (pList->GetTask(usTaskID) != NULL);
}

// 采集接口 张新炎调 返回是否成功  传入采集的植物ID
bool CTaskModule::OnCollectItem( CEntityPlayer* pPlayer, CTemplateCollect* pNpcTpl )
{
	LK_ASSERT(pPlayer, return false);
	LK_ASSERT(pNpcTpl, return false);

	bool bRet = false;

	CPropertyPlayer* pProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	
	// 如果是生活技能的采集npc得进行包裹判断
	
	if( pNpcTpl->mSkillType == CTemplateCollect::LIFESKILL_LIFESKILL )
	{	
		int tNum = 0;	
		int tCheckIDList[20] = { 0 };
		int tCheckNumList[20] = { 0 };	
		for ( int i = 0; i < (int)ARRAY_CNT( pNpcTpl->mItemID ) 
			&& i < (int)ARRAY_CNT( pNpcTpl->mItemProbality )
			&& tNum < ( int )ARRAY_CNT( tCheckIDList )
			&& tNum < ( int )ARRAY_CNT( tCheckNumList ); i++ )
		{
			if ( pNpcTpl->mItemID[ i ] <= 0 || pNpcTpl->mItemNum[ i ] <= 0 )
			{					
				break;
			}
			
			int tRand = RAND(SERVER_PERCENT_INT);
			if( tRand > pNpcTpl->mItemProbality[ i ] )
			{
				continue;
			}		
			
			tCheckIDList[ tNum ] = pNpcTpl->mItemID[ i ];
			tCheckNumList[ tNum ] = RAND(SERVER_PERCENT_INT) % pNpcTpl->mItemNum[ i ] + 1;
			tNum++;
		}

		if( tNum <= 0 )
		{
			return false;
		}

		
		if( !pProperty->GetBaggage()->Space( -1, tNum ) )
		{
			//CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_LIFESKILL_BAGFULLWHENCOLLECT );
			// 如果包裹空间不够，则扔到地上
			int tpCharIDList[TEAMNUM] = { 0 };
			tpCharIDList[0]	= pPlayer->GetCharID();
			for ( int i = 0; i < tNum; ++i )
			{  				
				CItemCreator tCreator( tCheckIDList[i], 5, pPlayer->GetPosX( ), pPlayer->GetPosY( ), tpCharIDList, NULL );				
				unsigned int tEntityID = 0;
				CMapModule::GetSingleton().CreateNpc( pPlayer->GetLineID(),
					pPlayer->GetMapID(),
					pPlayer->GetMapIndex(),
					&tCreator,
					tEntityID
					);
			}			
			return true;				
		}
		
		for ( int i = 0; i < tNum; ++i )
		{  				
			CItemObject *pInsItemPile = 
				CPropertyModule::GetSingleton().PlayerInsertItem( pPlayer, tCheckIDList[i], tCheckNumList[i], false );

			if (pInsItemPile != NULL)
			{
				LogEventGetItemByCollect( pPlayer, pInsItemPile->GetGUID(), 
					tCheckIDList[i], tCheckNumList[i], pPlayer->GetPosX(), pPlayer->GetPosY(),  0 );
			}
		}	
		return true;			
	}	

	// 判断有没有这个任务	
	for ( int i = 0 ; i < MAX_COLLECT_TASKS; ++i )
	{
		if (pNpcTpl->mTaskID[i] != 0 && IsTaskOnPlayer(pPlayer, pNpcTpl->mTaskID[i]))
		{
			CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
			LK_ASSERT( pList,  return false );
			if ( CTaskLuaInterface::Call<int, unsigned short, int>(pPlayer, pList,"get_collect_items_num_by_id", pNpcTpl->mTaskID[i], pNpcTpl->mItemID[i] )
				- pProperty->BaggageHasItem(pNpcTpl->mItemID[i]) > 0)
			{
				// 插入 不进fireevent，所以最后那个参数是false 			
				CItemObject *pInsItemPile = NULL;
				pInsItemPile = CPropertyModule::GetSingleton().PlayerInsertItem( pPlayer, pNpcTpl->mItemID[i], pNpcTpl->mItemNum[i], false);
				if ( pInsItemPile != NULL)
				{
					LogEventGetItemByCollect(pPlayer, pInsItemPile->GetGUID(), 
						pNpcTpl->mItemID[i], pNpcTpl->mItemNum[i], pPlayer->GetPosX(), pPlayer->GetPosY(),  pNpcTpl->mTaskID[i]);
				}
				int tRand = RAND(SERVER_PERCENT_INT);
				if( tRand < pNpcTpl->mExtraItemProbality[i] )
				{
					pInsItemPile = CPropertyModule::GetSingleton().PlayerInsertItem( pPlayer, pNpcTpl->mItemID[i], RAND(SERVER_PERCENT_INT)%pNpcTpl->mExtraItemNum[i]+1, false);
					if ( pInsItemPile != NULL)
					{
						LogEventGetItemByCollect(pPlayer, pInsItemPile->GetGUID(), 
							pNpcTpl->mItemID[i], pNpcTpl->mItemNum[i]+RAND(SERVER_PERCENT_INT)%pNpcTpl->mExtraItemNum[i]+1, pPlayer->GetPosX(), pPlayer->GetPosY(),  pNpcTpl->mTaskID[i]);
					}
				}
				bRet = true;
			}
		}	
	}	 

	return bRet;
}

// 得到任务类型
int CTaskModule::GetTaskType( unsigned short vTaskID )
{
	return CTaskLuaInterface::Call<int, unsigned short>( NULL, NULL, "get_task_type_by_id", vTaskID ); // 确定不需要外部数据
}

// 出生挂任务
void CTaskModule::SetDefaultTask( CEntityPlayer* pPlayer, const char* pTaskTypeName )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	if (pList == NULL) // 创建列表
	{
		pList = (CTaskList*)CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_TASKLIST );	
		if ( pList == NULL ) // 没资源了
		{
			LOG_FATAL("TASK", "can't create player data !!!  charid=%d", pPlayer->GetCharID() );
			return;
		}
		pPlayer->SetTaskToken(pList->get_id());
		pList->SetOwnerEntityID(pPlayer->GetEntityID());
	}

	unsigned short tTaskID = lua_tinker::get<unsigned short>( L, pTaskTypeName);
	if (tTaskID > 0)
	{
		PERF_FUNC("PlayerObtainTask", PlayerObtainTask( pPlayer, tTaskID, true ));
		LogEventPlayerAcceptTask( pPlayer, tTaskID);
	}

}

// 干掉所有任务
void CTaskModule::ClearTasks( CEntityPlayer* pPlayer )
{
	LK_ASSERT( pPlayer, return );
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );

	//unsigned short tasks[TASK_LIST_SIZE] = {0};
	//for (int i = 0; i < pList->mTaskNumber; ++i)
	//{
	//	tasks[i] = pList->mTaskList[i].mTaskID;
	//}
	//ClearTasks( pPlayer, tasks, pList->mTaskNumber);

	pList->Initialize();
	CMessageClearTaskNotify msg;
	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_CLEARTASKNOTIFY);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}

// 根据类型清除任务 (私有)
void CTaskModule::ClearTasksByType( CEntityPlayer* pPlayer, enTaskType nType )
{
	LK_ASSERT( pPlayer, return );
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );

	unsigned short tasks[TASK_LIST_SIZE] = {0};
	int n = 0;

	for (int i = 0; i < pList->mTaskNumber; ++i)
	{
		if (GetTaskType(pList->mTaskList[i].mTaskID) == (int)nType)
			tasks[n++] = pList->mTaskList[i].mTaskID;
	}
	ClearTasks( pPlayer, tasks, n);
}

// 单个任务 包括给客户端发消息(私有)
void CTaskModule::ClearTasks( CEntityPlayer* pPlayer, unsigned short *aTaskID, int n )
{
	LK_ASSERT( pPlayer, return );
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );
	LK_ASSERT( aTaskID,  return );
	CMessageAbortTaskNotice msg;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_ABORT_TASK);

	for ( int i = 0; i < n; ++i )
	{
		if (aTaskID[i] <= 0)
			continue;

		pList->RemoveTask( aTaskID[i] );
		msg.set_taskid( aTaskID[i] );
		tMessage.set_msgpara((uint32_t) &msg);
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
	}
}

// 发给客户端任务操作失败的消息
void CTaskModule::Send2ClientOprFailed( CEntityPlayer* pPlayer)
{
	CMessageTaskOprFailNotice tError;
	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tError);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_TASKOPRFAIL);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}



// 发给客户端刷新任务奖励失败的消息
void CTaskModule::Send2ClientRefreshFailed( CEntityPlayer* pPlayer)
{
	CMessageTaskRefreshFailNotice tError;
	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tError);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_TASKREFRESHFAIL);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}

// 发给客户端任务不可刷新 

void CTaskModule::Send2ClientUnRefreshed( CEntityPlayer* pPlayer)
{
	CMessageTaskUnRefreshNotice tError;
	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tError);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_TASKUNREFRESHED);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}

// 发给客户端刷新任务奖励的品质
void CTaskModule::Send2ClientRefreshResult( CEntityPlayer* pPlayer, unsigned short vTaskID, int vTaskColor, unsigned short vTaskNumber )
{
	CMessageTaskRefreshResult tResult;
	tResult.set_taskid(vTaskID);
	tResult.set_taskcolor(vTaskColor);
	tResult.set_tasknumber(vTaskNumber);
	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tResult);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_TASKREFRESHRESULT);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}

// 通知客户端  删除本地任务品质

void CTaskModule::Send2ClientRemoveTaskColor( CEntityPlayer* pPlayer, unsigned short vTaskID, int vTaskColor )
{
	CMessageRmoveTaskColor tRemove;
	tRemove.set_taskid(vTaskID);
	tRemove.set_taskcolor(vTaskColor);
	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tRemove);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_REMOVETASKCOLOR);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}

// 通知客户的  该任务的冷却时间还未到

void CTaskModule::Send2ClientUnReaccpetTask( CEntityPlayer* pPlayer, unsigned short vTaskID, int vNeedTime )
{
	CMessageUnReaccpetTask tUnaccept;
	tUnaccept.set_taskid(vTaskID);
	tUnaccept.set_needtime(vNeedTime);
	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &tUnaccept);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_TASKUNREACCEPT);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}

// 获得环任务数据 赋值给消息
// nMsgType = 1   任务列表 
// nMsgType = 2   接任务消息 
void CTaskModule::GetLoopTaskClientData( CEntityPlayer* pPlayer, CTaskList* pList, LoopTaskStruct* pLoopStruct )
{
	LK_ASSERT( pList, return );
	LK_ASSERT( pPlayer, return );
	LK_ASSERT( pLoopStruct , return );
	
	pLoopStruct->set_nextloopid( pList->mLoopTaskData.GetCurSubTaskIndex() + 2);
	pLoopStruct->set_completedloops( pList->mLoopTaskData.GetCompletedTimes() );
	pLoopStruct->set_todaymultiloops( pList->mLoopTaskData.GetTodayCompletedTimes() );
	pLoopStruct->set_weekmultiloops( pList->mLoopTaskData.GetWeekLoops() );
	pLoopStruct->set_loopid( pList->mLoopTaskData.GetCurSubTaskIndex() + 1 );

	//unsigned short usCurSubTaskID = pList->mLoopTaskData.GetSubTaskIDbyIndex( pList->mLoopTaskData.GetCurSubTaskIndex() );
	// LUA的get_looptaskdata接口主要获得环任务的静态数据，所以只要传入身上这轮环任务的随便一个子任务ID就可以了
	unsigned short usCurSubTaskID = pList->mLoopTaskData.GetOneSubTaskID();
	if (usCurSubTaskID == 0)
	{
		return;
	}

	lua_tinker::table ret = CTaskLuaInterface::Call<lua_tinker::table, unsigned short>( pPlayer, pList, "get_looptaskdata", usCurSubTaskID );
	pLoopStruct->set_looptaskid( ret.get<int>("loopid") );
	pLoopStruct->set_totalmultiloops( ret.get<int>("day_total") );
	pLoopStruct->set_weektotalmultiloops( ret.get<int>("week_total") );
	// 新加：每天完成的前x次环任务的奖励
	pLoopStruct->set_awardmultipletoday( ret.get<int>("day_multi") );
}

// 移除和某NPC的对话 （玩家可以再次对话）
void CTaskModule::RemoveTalkWithNpc( CEntityPlayer* pPlayer, unsigned short vTaskID, int nNpcTempID )
{
	LK_ASSERT( pPlayer, return );
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );

	CTask* pTask = pList->GetTask(vTaskID);
	if ( pTask == NULL )
		return ; // 任务可能已经交了

	if ( pTask->RemoveTalkedWithNpc( nNpcTempID ) )
	{
		// 通知客户端
		CMessageNpcTalkStateChangeNotice msg;
		msg.set_taskid( vTaskID );
		msg.set_npctempid( nNpcTempID );
		msg.set_state( 0 );
		CMessage   tMessage;
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_NPCTALKCHANGE);
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
	}
	else
	{
		LOG_ERROR("TASK" ,"player %s and NPC %d's talk not found, please check logic", pPlayer->GetCharNameShow(), nNpcTempID );
	}	
}

// 玩家和变身NPC战斗失败
void CTaskModule::OnNpcRecoverdState( int nPlayerCharID, unsigned short usTaskID, int nNpcTempID )
{
	if ( nPlayerCharID == 0 || usTaskID == 0 || nNpcTempID == 0 )
		return;

	CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(nPlayerCharID);
	if ( pPlayer == NULL ) // 可能已经下线了
		return;

	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );

	// 恢复对话
	RemoveTalkWithNpc( pPlayer, usTaskID, nNpcTempID );
	// 恢复战斗中的NPC ID
	pList->SetBossNpcEntityID(0);
}

// 给辉哥提供的获得任务名字的接口
const char* CTaskModule::GetTaskName( unsigned short vTaskID )
{
	return lua_tinker::call<const char*, unsigned short, unsigned short>(L, "get_ctrl", vTaskID, CTRL_NAME);
}

bool CTaskModule::GetSendNpcCurTarget( int nEntityID, CWTPoint& rPoint )
{
	return mSendNpcMgr.GetNpcCurTarget( nEntityID, rPoint );
}

// 得到lua虚拟机内存使用kb
int CTaskModule::GetLuaVMUsedMem( )
{
	return lua_getgccount( L );
}

// 得到当前正在护送NPC
// 参数：玩家的ID
CEntityNpc* CTaskModule::GetCurSendNpc( CEntityPlayer* pPlayer )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return NULL );

	CEntityNpc* pEntityNpc = (CEntityNpc*)CSceneLogic::GetSingleton().GetEntity( pList->GetSendNpcEntityID() );
	return pEntityNpc;
}

// 得到任务状态接口
int CTaskModule::GetTaskState( CEntityPlayer* pPlayer, unsigned short usTaskID )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return NULL );

	return pList->GetTaskState( usTaskID );
}

// 强制某任务完成
int CTaskModule::OnForceTaskComplete( std::vector<unsigned int>& rEntitys, unsigned short usTaskID )
{
	for ( std::vector<unsigned int>::iterator it = rEntitys.begin(); it != rEntitys.end(); ++it )
	{
		CEntityPlayer* tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( *it );
		LK_ASSERT( tpPlayer != NULL, continue );
		CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(tpPlayer->GetTaskToken( ));
		LK_ASSERT( pList,  return NULL );
		CTask* tpTask = pList->GetTask(usTaskID);
		if ( tpTask != NULL )
		{
			tpTask->SetTaskProgress( TASK_COMPLETE );
			// 如果定时器未结束 干掉定时器
			if( tpTask->mTimerID != INVALID_OBJ_ID )
			{
				int tRet = tpPlayer->GetTimerReciver()->ClearTimer( tpTask->mTimerID );
				if ( tRet != 0 )
					LOG_ERROR("TASK", "abort del timer returned %d, timerid=%d taskid=%d", tRet, tpTask->mTimerID, usTaskID );

				LOG_DEBUG("TASK", "abort del timer returned %d, timerid=%d taskid=%d", tRet, tpTask->mTimerID, usTaskID );
				tpTask->mTimerID = INVALID_OBJ_ID;
			}

			CMessageTaskCompleteNotice msg;
			msg.set_taskid(usTaskID);
			CMessage   tMessage;
			tMessage.set_msgpara((uint32_t) &msg);
			tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_COMPLETE);
			CSceneLogic::GetSingleton( ).Send2Player( tpPlayer, &tMessage );
		}
	}
	return 0;
}

// 每日事务
void CTaskModule::DaylyDispos( CEntityPlayer* pPlayer )
{
	CPropertyPlayer* tpProPlayer = (CPropertyPlayer*) pPlayer->GetProperty();

	if ( !IsANewDay((time_t)tpProPlayer->GetLastDaylyDisposTime(), time(0)) )
		return ;

	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );
	OnTaskPlayerNewDayCome(pList);
}

// 每周事务
void CTaskModule::WeeklyDispos( CEntityPlayer* pPlayer )
{
	CPropertyPlayer* tpProPlayer = (CPropertyPlayer*) pPlayer->GetProperty();

	if ( !IsANewWeek((time_t)tpProPlayer->GetLastWeeklyDisposTime(), time(0)) )
		return ;

	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );
	pList->OnNewWeekCome();
}

// 获得库任务数据接口
int  CTaskModule::GetStorageTaskData( CEntityPlayer* pPlayer, CTaskList* pTaskList, StorageTaskStruct* pData )
{
	LK_ASSERT( pPlayer!=NULL && pTaskList!=NULL && pData!=NULL, return -1 );
	pData->set_todaycompletes( pTaskList->mStorageTaskMgr.GetTodayCompleted() );
	lua_tinker::table ret = CTaskLuaInterface::Call<lua_tinker::table>( pPlayer, pTaskList, "get_storage_task_extra_awards" );
	int multi = ret.get<int>("multi");
	if ( multi <= 0 )
		multi = 1;
	pData->set_awardmultipletoday( multi );	// 倍率
	pData->set_totalmultiloops( ret.get<int>("times") );	// 每天x次
	return 0;
}

// 处理限时任务超时
int	CTaskModule::OnTaskTimeOut( CEntityPlayer* pPlayer, unsigned short usTaskID )
{
	// 注意：目前这里调用的2个函数 都会干掉定时器的 如果不会 就应该在这里干掉定时器 必须的！
	int tType = GetTimeOutFlag( usTaskID );
	if ( tType == TIMEOUT_FAIL )
	{
		OnTaskFailed( pPlayer, usTaskID );
	}
	else if ( tType == TIMEOUT_COMPLETE )
	{
		CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
		LK_ASSERT( pList,  return -1 );
		if ( pList->GetTaskState(usTaskID) == TASK_BEGIN ) // 只有进行中的任务才置成功
		{
			std::vector< unsigned int > players;
			players.push_back( pPlayer->GetEntityID() );
			OnForceTaskComplete( players, usTaskID );
		}
	}
	return 0;
}

// 当跨地图的时候调用
int  CTaskModule::OnPlayerRedirectMap( CEntityPlayer* pPlayer )
{
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return -1 );
	if ( pList->mRedirectFailTask != 0 )
	{
		OnTaskFailed( pPlayer, pList->mRedirectFailTask, false );
	}
	return 0;
}

// 强制刷某NPC的头顶
void CTaskModule::ForceRefreshNpcTip( int nNpcEntityID )
{
	if ( nNpcEntityID == INVALID_OBJ_ID )
		return;
	LOG_DEBUG("TASK", "%s", __FUNCTION__ );

	CEntityNpc* tpEntityNpc = dynamic_cast<CEntityNpc*>(CSceneObjManager::GetSingletonPtr()->GetObject( nNpcEntityID ) );
	if ( tpEntityNpc == NULL )
		return ;

	CProperty* pPropty = tpEntityNpc->GetProperty();

	CSeenPlayerContainer tSeenPlayer = tpEntityNpc->GetSeenPlayersContainer();
	// 遍历可见表
	for (CSeenPlayerContainer::SeenPlayerVector::iterator it = tSeenPlayer.mPlayers.begin()
		; it != tSeenPlayer.mPlayers.end(); ++it )
	{
		CEntityPlayer* tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity(*it);
		if ( tpPlayer == NULL )
			continue;

		CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(tpPlayer->GetTaskToken( ));
		LK_ASSERT( pList,  continue );

		CMessageNpcTaskTipNotice msg;

		if( tpEntityNpc->IsWithinViewDistance( tpPlayer ) == false )
			continue;

		unsigned short tTip = GetNpcTip( tpPlayer, pPropty->GetPropertyTempID(), pList, nNpcEntityID );
		LOG_DEBUG( "TASK", "send NPC TIP, temp ID=%d, tip=%d\n", pPropty->GetPropertyTempID(), tTip);

		msg.add_npcentityid( nNpcEntityID );
		msg.add_tip(tTip) ;

		CMessage   tMessage;
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_NPCTASKTIP);
		CSceneLogic::GetSingleton( ).Send2Player( tpPlayer, &tMessage );
	}
}



bool CTaskModule::OnCheckLaunch()
{
	// 加载数据库的全局数据
	// 热启动不加载
	// 不reload 所以不写到load_config
	//if (CObj::msCreateMode)
	//{
	//	CSceneLogic* tpLogic = CSceneLogic::GetSingletonPtr( );
	//	LK_ASSERT( tpLogic!=NULL, return false );
	//	tpLogic->ExecuteSql( load_task_global, 0, 0, 0, SQL_NEED_CALLBACK,
	//		SELECT, "select publish_info from UMS_TASK_GLOBAL_DATA" );
	//}
	return true;
}



// 填充/重置掌门任务
int CTaskModule::ResetChairManTasks( CEntityPlayer* pPlayer, unsigned short usChairManID )
{
	LK_ASSERT( pPlayer, return 0 );
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return 0 );

	pList->mChairManTaskMgr.Initialize();

	// 下面这个lua确定不需要外部数据
	lua_tinker::table ret = CTaskLuaInterface::Call<lua_tinker::table, int>( NULL, NULL, "refresh_chairman_tasks", usChairManID );

	int n = luaL_getn( L, -1); //min( luaL_getn( L, -1), MAX_STORAGETASK_LIST_NUM );
	if ( n > CHAIRMAN_TASK_NUM || n <= 0 )
	{
		LOG_ERROR("TASK", "Got %d ChairmanTasks?", n );
		return 0;
	}

	unsigned short usTasks[CHAIRMAN_TASK_NUM] = {0};
	for ( int i = 1; i <= n; ++i)
	{
		lua_rawgeti(L, -1, i);
		usTasks[i-1] = lua_tinker::pop<unsigned short>(L);
		LOG_DEBUG("TASK", "refresh Chairman %d ", usTasks[i-1] );
	}
	LOG_DEBUG("TASK", "refresh Chairman %d end! num=%d", usChairManID, n );
	//pList->mChairManTaskMgr.ResetTasks( usTasks, CHAIRMAN_TASK_NUM, usChairManID );
	return n;
}



// 临时处理代码
void CTaskModule::TempDispos( CEntityPlayer* pPlayer, CTaskList* pList )
{
	if ( pPlayer == NULL || pList == NULL )
		return;

	// 处理掌门任务
	for (CHAIRMAN_COMPLETED_TYPE::iterator it = pList->mChairManTaskMgr.mCompleted.begin();
		it != pList->mChairManTaskMgr.mCompleted.end(); ++it )
	{
		// 根据掌门任务ID得到列表
		int nRealCompleted=0;
		lua_tinker::table t1 = CTaskLuaInterface::Call<lua_tinker::table, int>
			(pPlayer, pList, "get_chairman_list_by_chairmanid", it->first );

		int nNum = luaL_getn(L, -1);
		for(int i =1; i<=nNum; ++i)
		{
			lua_rawgeti(L, -1, i);
			unsigned short task_id = (unsigned short)lua_tinker::pop<int>(L);
			// 判断完成没
			if ( pList->mCompleteTasks.find(task_id) != pList->mCompleteTasks.end() )// 完成了
			{
				++nRealCompleted;
			}
		}
		if ( nRealCompleted != it->second ) // 不一样！
		{
			LOG_ERROR("TASK", "*CHAIRMAN* player %d(%s) chairid %d realcomp %d indb %d", pPlayer->GetCharID(),
				pPlayer->GetCharNameShow(), it->first, nRealCompleted, it->second );
			it->second = nRealCompleted;
		}
		else
		{
			LOG_DEBUG("TASK", "*CHAIRMAN* player %d(%s) chairid %d realcomp %d OK", pPlayer->GetCharID(),
				pPlayer->GetCharNameShow(), it->first, nRealCompleted );
		}
	}
	// 把身上的掌门任务洗一遍 防止数据对不上
	int nChairman=0;
	for ( int i = 0; i < TASK_LIST_SIZE; ++i )
	{
		if ( GetTaskType( pList->mTaskList[i].mTaskID ) == TASKTYPE_CHAIRMAN )
			++nChairman;
	}
	if ( pList->mChairManTaskMgr.GetAcceptedNum() != nChairman )
	{
		LOG_ERROR("TASK", "player %d acc=%d rel=%d", pPlayer->GetCharID(), pList->mChairManTaskMgr.GetAcceptedNum()
			, nChairman);
		pList->mChairManTaskMgr.SetAcceptedNum(nChairman);
	}
}

// 强制设置任务组今天次数
void CTaskModule::SetTodayTimesByGroupID( CEntityPlayer* pPlayer, int nGroupID, int nNum )
{
	LK_ASSERT( pPlayer, return );
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return );
	pList->mTaskGroupMgr.SetTodayTimes( nGroupID, nNum );
}

// 任务列表每天处理 已包含新上线等情况
// 返回是否有被清理掉的已完成任务
bool CTaskModule::OnTaskPlayerNewDayCome(CTaskList* pTaskList, bool bIsOnline )
{
	pTaskList->OnNewdayCome();
	bool bRet = false;
	// 处理每天要清除的
	// 下面这个调用确认不用玩家信息 所以传入NULL
	lua_tinker::table t1 = CTaskLuaInterface::Call<lua_tinker::table>(NULL, NULL, "get_day_clear_tasks");
	int n = luaL_getn(L, -1);
	for(int i =1; i<=n; ++i)
	{
		lua_rawgeti(L, -1, i);
		unsigned short tTaskID = lua_tinker::pop<int>(L);
		if (pTaskList->IsTaskEnd(tTaskID)) // 已完成
		{
			bRet = true;
			pTaskList->ClearTaskEndFlag(tTaskID) ;
		}
	}

	std::vector<unsigned short> _fail_list;	// 失败的任务列表
	if ( bIsOnline ) // 检查有没有隔天失败的 有的话就失败
	{
		for ( int i = 0 ; i < pTaskList->mTaskNumber && i < TASK_LIST_SIZE; ++i )
		{
			if ( IsDailyFail(pTaskList->mTaskList[i].mTaskID) ) // 新一天 还得检查是不是 会失败
			{
				_fail_list.push_back(pTaskList->mTaskList[i].mTaskID);
			}
		}
	}

	CEntityPlayer* tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity(pTaskList->GetOwnerEntityID());
	if (tpPlayer != NULL)
	{
		for ( std::vector<unsigned short>::iterator it = _fail_list.begin(); it != _fail_list.end(); ++it )
		{
			OnTaskFailed( tpPlayer, *it );
		}
	}

	// 清除组队环任务次数
	if (pTaskList->GetTodayTeamLoopNum() >= MAX_TEAMLOOP_EVERYDAY)
	{
		pTaskList->ZeroTodayTeamLoopNum();
	}

	return bRet;
}

void CTaskModule::SendTaskMsg2Client( CTaskList* pList )
{
	CEntityPlayer *tpPlayer = ( CEntityPlayer * )CSceneLogic::GetSingleton().GetEntity( pList->GetOwnerEntityID() );	
	LK_ASSERT(tpPlayer != NULL, return );

	// 给客户端发送数据
	CMessageTaskListNotice msg;
	for( int i = 0; i < pList->mTaskNumber; ++i )
	{
		TaskStruct* tpTask = msg.add_tasks();
		pList->mTaskList[i].GetTaskDataStruct(tpTask);
	}
	LoopTaskStruct* tpLoop = msg.mutable_looptask();
	GetLoopTaskClientData( tpPlayer, pList, tpLoop);
	StorageTaskStruct* tpStorage = msg.mutable_storagetask();
	GetStorageTaskData( tpPlayer, pList, tpStorage );
	msg.set_tasknum(pList->mTaskNumber);
	for ( lk::hash_map<unsigned short, unsigned short, COMPLETE_TASK_NUM>::iterator it = pList->mCompleteTasks.begin()
		; it != pList->mCompleteTasks.end(); ++it )
	{
		msg.add_completedtasks( it->first );
	}

	for ( lk::hash_map<unsigned short, int, COMPLETE_TASK_NUM>::iterator it = pList->mRepeatTasks.begin()
		; it != pList->mRepeatTasks.end(); ++it )
	{
		msg.add_repeatetasks( it->first );
		msg.add_endtime( it->second );
	}

	// 加载掌门任务列表
	lua_tinker::table t1 = CTaskLuaInterface::Call<lua_tinker::table>
		(tpPlayer, pList, "get_all_chairman_list" );

	int nNum = luaL_getn(L, -1);
	for(int i =1; i<=nNum; ++i)
	{
		lua_rawgeti(L, -1, i);
		msg.add_chairmansectionids(lua_tinker::pop<int>(L));
	}
	CMessage   tMessage;
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_TASKLIST);
	CSceneLogic::GetSingleton( ).Send2Player( tpPlayer, &tMessage );

	int anIndex[TRAGEDY_TASK_SIZE] = {0};
	unsigned short asNextID[TRAGEDY_TASK_SIZE] = {0};
	int anState[TRAGEDY_TASK_SIZE] = {0};
	int n = pList->mTragedyTaskMgr.GetAllInfo( anIndex, asNextID, anState );
	SendTragedyInfo2Client( tpPlayer, anIndex, asNextID, anState , n, 1);
}

int  CTaskModule::SendTeamLoopErrNotify( CEntityPlayer* pPlayer, const char* pName, int nErrCode)
{
	if (pName == NULL)
		return -1;

	CMessageGetTeamLoopErrNotify msg;
	msg.set_charname(pName);
	msg.set_errcode(nErrCode);
	CMessage   tMessage;
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTIFY_GETTEAMLOOPERR);
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
	return 0;
}

// 快速接任务 用于被动
int CTaskModule::PlayerFastObtainTask( CEntityPlayer* pPlayer, unsigned short vTaskID, int taskColor )
{
	LK_ASSERT( pPlayer,  return -1 );
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return -1 );
	//  插入任务
	CTask vTask; 
	vTask.mTaskID = vTaskID;
	int tRet = pList->InsertTask(vTask);
	if ( tRet == ERROR_TASK_SUCCESS )
	{
		CTask* pTask = pList->GetTask(vTaskID);
		LK_ASSERT( pTask, return -2 );

		// 初始化一次任务数据
		CTaskLuaInterface::Call<int, CTask*>( pPlayer, pList, "init_this_task", pTask); //  初始化，并得到这个任务要杀怪的种类数

		// 通知客户端
		CMessageObtainTaskNotice msg;
		TaskStruct* tpTask = msg.mutable_task();
		pTask->GetTaskDataStruct(tpTask);

		CMessage   tMessage;
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid(ID_S2C_NOTICE_OBTAINTASK);
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );

		//  判断一次任务是否完成(强制刷新)
		FireEvent( pPlayer, 0 );
		DoTaskObtAwards( pPlayer, pList, vTaskID );

		// 取出组ID 如果不为0  要做处理 增加接受次数
		int tGroupID = CTaskLuaInterface::Call<int, unsigned short>( pPlayer, pList, "get_task_group_id", vTaskID );
		if ( tGroupID != 0 )
		{
			pList->mTaskGroupMgr.OnAcceptOneTask( tGroupID );
		}
		int vColor = pList->GetTaskColor(vTaskID);
		pList->InsertRefreshedTask(vTaskID, vColor);
		return 0;
	}
	return -3;
}

void CTaskModule::LoadLuaFileCallback(const char* pcConfigPath)
{
	if (CTaskModule::GetSingleton().m_bHasReloadLua)
	{
		return;
	}

	CTaskModule::GetSingleton().RegisterLua();
	CTaskModule::GetSingleton().m_bHasReloadLua = true;
}

bool CTaskModule::CheckPublishCondition(CEntityPlayer* pPlayer, unsigned short vTaskID)
{
	LK_ASSERT(pPlayer !=NULL, return false );
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return false );

	return CTaskLuaInterface::Call<bool, unsigned short>(pPlayer, pList, "check_publish_condition", vTaskID );
}


int CTaskModule::GetGroupNeedMoney(CEntityPlayer* pPlayer, unsigned int vTaskType, unsigned short vTaskID)
{
	LK_ASSERT(pPlayer !=NULL, return -1 );
	CTaskList* pList = (CTaskList*)CSceneObjManager::GetSingletonPtr()->GetObject(pPlayer->GetTaskToken( ));
	LK_ASSERT( pList,  return -1 );
	switch(vTaskType)
	{
	case CORPS_PUBLISH_TASK:
		{
			return CTaskLuaInterface::Call<int, unsigned short>(pPlayer, pList, "get_need_corps_money", vTaskID );
			break;
		}
	default:break;
	}
	return 0;
}