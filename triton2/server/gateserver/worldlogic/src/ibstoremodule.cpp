#include "ibstoremodule.h"
#include "tinyxml.h"
#include "gmmessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "gatelogic.h"
#include "gateobj_manager.h"
#include "sequence.h"
#include "core_service.h"
#include "activity.h"
#include "servermessage_pb.hxx.pb.h"

//
// CIbStoreManager
//
template<> CSequence* CSingleton< CSequence >::spSingleton = NULL;


//
// CIbStoreModule
//
template<> CIbStoreModule* CSingleton< CIbStoreModule >::spSingleton = NULL;

void CIbStoreModule::OnLaunchServer()
{
	// 先加载文件
	char stfile[100] ={0};
	sprintf(stfile,"ibstore/ibstore_%d.xml", CGateLogic::GetSingleton().mConfig.nGameID);
	char file_path[256] = {0};
	/*sprintf(file_path, "../config/data/ibstore/ibstore_%d.xml", CGateLogic::GetSingleton().mConfig.nGameID);*/
	sprintf(file_path, CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,stfile));


	struct stat stat_buf;
	if(lstat(file_path, &stat_buf) == 0)
	{
		m_tManager.Load4Xml(file_path);
		// 注册加载配置的回调函数
		CGateServer::GetSingleton().RegisterLoadConfigCallback(file_path, CIbStoreModule::LoadIbStoreConfigCallback);
	}else
	{
		/*m_tManager.Load4Xml("../config/data/ibstore/ibstore.xml");*/
		m_tManager.Load4Xml(CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"ibstore/ibstore.xml"));
		// 注册加载配置的回调函数
		const char* pcConfigPath = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"ibstore/ibstore.xml");
		CGateServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CIbStoreModule::LoadIbStoreConfigCallback);
	}
	
	//加载功能付费
	memset(stfile, 0, sizeof(stfile));
	memset(file_path, 0, sizeof(file_path));

	sprintf(stfile,"ibstore/funcstore_%d.xml", CGateLogic::GetSingleton().mConfig.nGameID);
	sprintf(file_path, CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,stfile));

	if(lstat(file_path, &stat_buf) == 0)
	{
		m_tManager.LoadFunc4Xml(file_path);
		// 注册加载配置的回调函数
		CGateServer::GetSingleton().RegisterLoadConfigCallback(file_path, CIbStoreModule::LoadFuncStoreConfigCallback);
	}else
	{
		m_tManager.LoadFunc4Xml(CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"ibstore/funcstore.xml"));
		// 注册加载配置的回调函数
		const char* pcConfigPath = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID, "ibstore/funcstore.xml");
		CGateServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CIbStoreModule::LoadFuncStoreConfigCallback);
	}
	// 再加载促销
	LoadIbStoreInfoFromDB(0, em_load_count_per_session);

	m_tManager.BackupAndSave();

}

void CIbStoreModule::OnExitServer()
{

}

void CIbStoreModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	if (mCheckTimer.Elapse( vTickOffset ) == false ) return;

	// 如果没有促销立即返回
	if (m_tManager.m_tPromotion.size() == 0) return; 	

	// 只要当前时间大于需要刷新的时间，则立即刷新
	
	bool  bNeedNotice = false;
	time_t nNow = time(0);
	CIbStoreManager::promotion_t::iterator iter = m_tManager.m_tPromotion.begin();
	for (; iter != m_tManager.m_tPromotion.end(); ++iter)
	{
		CPromotion& promotion = iter->second;
		if ( nNow >= promotion.GetExpirityTime())
		{
			if (promotion.ResetActivePeriod(nNow))
			{

				bNeedNotice = true;

				promotion.SetRemainCount(promotion.GetMaxCount());
				UpdateIbStoreInfo2DB(promotion.GetGoodsID(), 0, nNow);
				LOG_INFO("ibtrans", "Reset Promotion[%d, %d %d - %d %d] [%d - %d] of GoodsID(%d) number to %d from %d to %d", 
						promotion.GetType(), promotion.GetBeginVal(), promotion.GetBeginTime(), promotion.GetEndVal(), promotion.GetEndTime(), 
						promotion.GetActiveTime(), promotion.GetExpirityTime(),
						promotion.GetGoodsID(), promotion.GetRemainCount(), 
						promotion.GetActiveTime(), promotion.GetExpirityTime());
			}
		}
	}

	if ( bNeedNotice )
	{
		SendIbStoreInfo2Scene();
	}

	/*
	if (m_tNoticeTimer.Elapse( vTickOffset ) == false ) return;

	if (!m_bIsLoaded ) return ;

	int nNow = time(0);
	CIbStoreManager::promotion_t::iterator iter = m_tManager.m_tPromotion.begin();
	for (; iter != m_tManager.m_tPromotion.end(); ++iter)
	{
		const CPromotion& promotion = iter->second;
		int nDiff = nNow - promotion.GetBeginTime();
		for ( int i = 0; i < promotion.GetNoticeTimerCount(); ++i)
		{
			int nNoticeDiff = promotion.GetNoticeTimer(i); 
			if ( promotion.GetBeginTime() != -1)
			{
				if ( (nDiff > (nNoticeDiff - 1) * m_tNoticeTimer.mMaxDuration) 
				&& (nDiff <= nNoticeDiff * m_tNoticeTimer.mMaxDuration))
				{
					// 发送时间点日志
				}
			}
		}
	}
	*/
}



CIbStoreModule::CIbStoreModule()
{
	if( mShmPtr->GetInitMode() == SHM_INIT )
	{
		Initialize();
		m_bIsLoaded = false;

	}else
	{
		Resume();
	}

	new CSequence(1, 0x7FFFFFFF, 1);
}

CIbStoreModule::~CIbStoreModule()
{

}

int CIbStoreModule::Initialize()
{
	m_tManager.Initialize();

	//默认为5秒钟
	mCheckTimer = CWTimer(5000);
	return 0;
}

int CIbStoreModule::Resume()
{
	return 0;
}

unsigned int CIbStoreModule::CountSize()
{
	return sizeof(CIbStoreModule);
}

void* CIbStoreModule::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CIbStoreModule::operator delete( void* pointer )
{

}



void CIbStoreModule::ReloadCfg()
{
	// 配置重加载及相关逻辑处理统一走回调函数
	// 商城重新加载
	char stfile[100] = {0};
	sprintf(stfile,"ibstore/ibstore_%d.xml", CGateLogic::GetSingleton().mConfig.nGameID);
	char file_path[256] = {0};
	/*sprintf(file_path, "../config/data/ibstore/ibstore_%d.xml", CGateLogic::GetSingleton().mConfig.nGameID);*/
	sprintf(file_path, CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,stfile));

	struct stat stat_buf;
	if(lstat(file_path, &stat_buf) == 0)
	{
		CIbStoreModule::GetSingleton().LoadIbStoreConfigCallback(file_path);
	}else
	{
		const char* pcConfigPath =	CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"ibstore/ibstore.xml");
		CIbStoreModule::GetSingleton().LoadIbStoreConfigCallback(pcConfigPath);
	}


	//功能付费
	memset(stfile, 0, sizeof(stfile));
	memset(file_path, 0, sizeof(file_path));

	sprintf(stfile,"ibstore/funcstore_%d.xml", CGateLogic::GetSingleton().mConfig.nGameID);
	sprintf(file_path, CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,stfile));

	if(lstat(file_path, &stat_buf) == 0)
	{
		CIbStoreModule::GetSingleton().LoadFuncStoreConfigCallback(file_path);
	}else
	{
		const char* pcConfigPath = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"ibstore/funcstore.xml");
		CIbStoreModule::GetSingleton().LoadFuncStoreConfigCallback(pcConfigPath);
	}
}


void CIbStoreModule::OnAGIPSessionQueryYuanbao(SysProtocol* pSysProto, int nAccountID, int nRoleID)
{
	LK_ASSERT(pSysProto!= NULL, return);
	AGIPUserBalanceInfoRes* pprotores = (AGIPUserBalanceInfoRes*) pSysProto;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer (nRoleID);
	if (tpPlayer == NULL)
	{
		LOG_ERROR("ibtrans", "when query yuanbao balance rspn for role(%d), role has left.", nRoleID);
		return;
	}

	if (pprotores != NULL)
	{
		unsigned int nCommandID = 0;
		pSysProto->getCommandID(&nCommandID);
		LK_ASSERT(nCommandID == CMD_USER_BALANCE_INFO_RES, return);

		int nResult = 0;
		int nCount = 0;

		pprotores->getResultCode(&nResult);
		pprotores->getBalanceCount(&nCount);

		CMessage4EQueryYuanBaoResponse QueryRspn;
		QueryRspn.set_roleid(nRoleID);

		switch (nResult) 
		{
			case S_SUCCESS:
			{
				QueryRspn.set_resultcode(ERROR_IBSTORE_DRAW_SUCCESS);
				break;
			}

			case E_PARAMETER_ERROR:
			{
				QueryRspn.set_resultcode(ERROR_IBSTORE_INVALID_INPUT);
				break;
			}


			case E_SYS_DATABASE_ERROR:
			case E_SYS_DATABASE_CONNECT_ERROR:
			{
				QueryRspn.set_resultcode(ERROR_IBSTORE_DRAW_ISNOT_ACTIVE);
				break;
			}

			case E_ERROR:
			default:
			{
				QueryRspn.set_resultcode(ERROR_IBSTORE_DRAW_FAILURE);
				break;
			}
		}

		if ( nResult == S_SUCCESS )
		{
			for ( int i = 0; i < nCount; ++i)
			{
				int nUnit  = 0;
				int nValue = 0;
				int nLeft = 0;

				if ( pprotores->getBalanceInfo(i, &nUnit, &nValue, &nLeft) == S_SUCCESS)
				{
					PBIbMoney* pbIbMoney = QueryRspn.add_remain();
					if ( pbIbMoney != NULL)
					{
						pbIbMoney->set_unit(nUnit);
						pbIbMoney->set_value(nLeft);
					}
				}
			}
		}

		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2S_QUERYYUANBAO_RESPONSE, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &QueryRspn);

		CGateServer::GetSingleton().Send2Scene(tpPlayer, &tMessage);
	}
}

void CIbStoreModule::OnAGIPSessionDrawYuanbao(SysProtocol* pSysProto, int nAccountID, int nRoleID)
{
	LK_ASSERT(pSysProto!= NULL, return);

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer (nRoleID);
	if (tpPlayer == NULL)
	{
		LOG_ERROR("ibtrans", "when draw yuanbao balance rspn for role(%d), role has left.", nRoleID);
		return;
	}

	AGIPUserDrawBalanceRes* pprotores = (AGIPUserDrawBalanceRes*) pSysProto;
	if (pprotores != NULL)
	{
		unsigned int nCommandID = 0;
		pSysProto->getCommandID(&nCommandID);
		LK_ASSERT(nCommandID == CMD_USER_DRAW_BALANCE_RES, return);

		int nResult = 0;
		int nAmount = 0;
		int nLeft = 0;

		if(pprotores->getResultCode(&nResult) != S_SUCCESS
		|| pprotores->getAmount(&nAmount) != S_SUCCESS
		|| pprotores->getLeftAmount(&nLeft) != S_SUCCESS)
		{
			LOG_ERROR("ibtrans", "draw yuanbao error for role(%d)", nRoleID); 
			return;
		}

		CMessage4EDrawYuanBaoResponse DrawRspn;
		DrawRspn.set_roleid(nRoleID);

		switch (nResult) 
		{
			case S_SUCCESS:
			{
				DrawRspn.set_resultcode(ERROR_IBSTORE_DRAW_SUCCESS);
				break;
			}

			case E_PARAMETER_ERROR:
			{
				DrawRspn.set_resultcode(ERROR_IBSTORE_INVALID_INPUT);
				break;
			}

			case E_USER_BALANCE_NOT_ENOUGH:
			{
				DrawRspn.set_resultcode(ERROR_PLAYER_HASNO_ENOUGHYB);
				break;
			}

			case E_SYS_DATABASE_ERROR:
			case E_SYS_DATABASE_CONNECT_ERROR:
			case E_GATEWAY_NOT_FOUND:
			case E_SYS_DATABASE_DATA_INVALID:
			{
				DrawRspn.set_resultcode(ERROR_IBSTORE_DRAW_ISNOT_ACTIVE);
				break;
			}

			case E_ERROR:
			default:
			{
				DrawRspn.set_resultcode(ERROR_IBSTORE_DRAW_FAILURE);
				break;
			}
		}
		DrawRspn.set_amount(nAmount);
		DrawRspn.set_leftamount(nLeft);

		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2S_DRAWYUANBAO_RESPONSE, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &DrawRspn);

		CGateServer::GetSingleton().Send2Scene(tpPlayer, &tMessage);
	}
}

void CIbStoreModule::LoadIbStoreInfoFromDB(int nStart, int nCount)
{
	const char* stmtfmt = "SELECT goods_id, locked_count, unix_timestamp(update_time) FROM GMS_IBSTORE_PROMOTION limit %d, %d";
	if (! CGateLogic::GetSingleton().ExecuteSql(em_dbsession_ibstoreinfo, 
				nStart, nCount, 0, SQL_NEED_CALLBACK, SELECT, stmtfmt, nStart, nCount))                                
	{                                                                                                             
		LOG_ERROR("default", "create dbsession on function %s", __FUNCTION__);                                    
	}
}

void CIbStoreModule::UpdateIbStoreInfo2DB(int nGoodsID, int nLockedNum, int nUpdateTime)
{
	const char* stmtfmt = "REPLACE INTO GMS_IBSTORE_PROMOTION (goods_id, locked_count, update_time) VALUES( %d, %d, FROM_UNIXTIME(%d))";
	if (! CGateLogic::GetSingleton().ExecuteSql(em_dbsession_null, 
				0, 0, 0, SQL_NONE, UPDATE, stmtfmt, nGoodsID, nLockedNum, nUpdateTime))                                
	{                                                                                                             
		LOG_ERROR("default", "create dbsession on function %s", __FUNCTION__);                                    
	}
}

void CIbStoreModule::LoadIbStoreConfigCallback(const char* pcConfigPath)
{
	if (pcConfigPath == NULL)
	{
		return;
	}
	
	CIbStoreModule::GetSingleton().m_tManager.m_tGoods.clear();
	for(size_t i = 0; i < ARRAY_CNT(CIbStoreModule::GetSingleton().m_tManager.m_pbIbStore); ++ i) 
	{
		if (CIbStoreModule::GetSingleton().m_tManager.m_pbIbStore [i] != NULL)
		{
			delete CIbStoreModule::GetSingleton().m_tManager.m_pbIbStore[i];
			CIbStoreModule::GetSingleton().m_tManager.m_pbIbStore[i] = NULL;
		}
	}

	for( size_t i =0; i < ARRAY_CNT(CIbStoreModule::GetSingleton().m_tManager.m_pbFocus); ++ i)
	{
		if ( CIbStoreModule::GetSingleton().m_tManager.m_pbFocus[i] != NULL)
		{
			delete CIbStoreModule::GetSingleton().m_tManager.m_pbFocus[i];
			CIbStoreModule::GetSingleton().m_tManager.m_pbFocus[i] = NULL;
		}
	}


	CIbStoreModule::GetSingleton().m_tManager.Load4Xml(pcConfigPath, true);
	
	CMessage tMsg;
	CMessageSynIbStoreRequest tFactMsg;
	tMsg.set_msgpara((uint32_t) &tFactMsg);
	pbmsg_sethead(&tMsg, ID_S2G_SYNIBSTORE_REQUEST, 0, 0, 0, 0, 0, 0);

	for ( int i = 1; i <= CGateServer::GetSingleton().GetMaxSceneServerID(); ++i)
	{
		if ( CGateServer::GetSingleton().GetServerObjID(i) == INVALID_OBJ_ID ) continue;
		CIbStoreModule::GetSingleton().OnMessageSynIbStoreRequest(i, &tMsg );
	}
}

void CIbStoreModule::LoadFuncStoreConfigCallback(const char* pcConfigPath)
{
	if (pcConfigPath == NULL)
	{
		return;
	}
	CIbStoreModule::GetSingleton().m_tManager.m_tFuncConsume.clear();
	//加载功能付费
	CMessage tMsg;
	CIbStoreModule::GetSingleton().m_tManager.LoadFunc4Xml(pcConfigPath);

	CIbStoreModule::GetSingleton().m_tManager.m_tFuncConsume;
	CMessage tFuncMsg;
	CMessageSynFuncStoreRequest tFuncSynMsg;
	tFuncMsg.set_msgpara((uint32_t) &tFuncSynMsg );
	pbmsg_sethead(&tMsg, ID_S2G_SYNFUNCSTORE_REQUEST, 0, 0, 0, 0, 0, 0);

	for( int i = 1; i<= CGateServer:: GetSingleton().GetMaxSceneServerID(); ++i)
	{
		if (CGateServer::GetSingleton().GetServerByID(i) == INVALID_OBJ_ID ) continue;
		CIbStoreModule::GetSingleton().OnMessageSynFuncStoreRequest( i, &tFuncMsg);
	}
}