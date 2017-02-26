#include "fcmservice.h"
#include "gateobj_manager.h"
#include "gatelogic.h"
#include "servermessage_in.h"
#include "nameservice.h"
#include "core_service.h"
#include "gmmessage_pb.hxx.pb.h"

using namespace GateServer;

template<> CServiceGateFcm* CSingleton< CServiceGateFcm >::spSingleton = NULL;

void CServiceGateFcm::OnLaunchServer()
{
	mFcmRequestInterval = CGateLogic::GetSingleton().mConfig.nFcmWarningTimer;

	mFcmRptCheckTimer = 60 * 1000; // 每分钟检查一次

	mFcmPlayerList.initailize();
	mLatestResetTime = time(0);
}

void CServiceGateFcm::OnExitServer()
{
	
}

void CServiceGateFcm::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	if ( CGateLogic::GetSingleton().mConfig.bFcmServiceOn == false) return;

	// 每分钟检查一次
	if ( ! mFcmRptCheckTimer.Elapse( vTickOffset ) ) return;

	time_t nNow = time(0);

	//CMessageFCMPlayerListNotify tFactMsg[SCENE_AT_GATE_CAP+1];
	FCMPLAYERID_MAP::iterator iter = mFcmPlayerList.begin();
	for ( ; iter != mFcmPlayerList.end(); ++iter )
	{
		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( iter->first );
		if( tpPlayer == NULL ) continue;

		// 取得两次报告的时间间隔
		int nElapsedTime = nNow - iter->second.first;

		// 如果时间间隔超过了设定的warn周期 或者玩家的剩余时间小于warn周期
		/*
		if ( nElapsedTime > mFcmRequestInterval )
		{
			CSceneServer* pHost = 
				CGateServer::GetSingleton().GetServerByLineMap(tpPlayer->mLineID, tpPlayer->mMapID);

			if ( pHost == NULL || pHost->mServerID > SCENE_AT_GATE_CAP || pHost->mServerID < 1) continue;

			tFactMsg[(int)pHost->mServerID].add_reportplayer(tpPlayer->mCharID);

			iter->second.first = nNow;
		}
		*/

		// 1. 距离上次报告时间超过了半小时
		// 2. 玩家防沉迷状态发生改变
		if ( iter->second.second < nElapsedTime 
		||   nElapsedTime >= mFcmRequestInterval
		||   (iter->second.second > 7200 && (iter->second.second - 7200) < nElapsedTime))
		{
			iter->second.first = nNow;
			iter->second.second -= nElapsedTime;

			if ( iter->second.second < 0 ) 
			{
				iter->second.second = 0;
			}

				
			CMessage tMessage;
                	CMessageFangChenMiResponse tFactMsg;

			tFactMsg.set_charid(iter->first);
			PBSubjectAmount *pSubject = tFactMsg.add_subjects();
			pSubject->set_subjectid(100);	
			pSubject->set_amount(iter->second.second);
				
			pbmsg_sethead(&tMessage, ID_G2S_FANGCHENMI_RESPONSE, 0, 0, 0, 0, 0, 0);
			tMessage.set_msgpara((uint32_t) &tFactMsg);

			CGateServer::GetSingleton().Send2Scene(tpPlayer, &tMessage);
		}
	}


	/*
	// 向各场景发送防沉迷玩家列表
	for ( int i = 1; i <= CGateServer::GetSingleton().GetMaxSceneServerID(); ++i) 
	{
		if ( CGateServer::GetSingleton().GetServerObjID(i) == INVALID_OBJ_ID ) continue;
	
		if ( tFactMsg[i].reportplayer_size() > 0)
		{
			CMessage tMessage;
			tMessage.set_msgpara((uint32_t) &tFactMsg[i]);
			pbmsg_sethead(&tMessage, ID_G2S_FCMPLAYERLIST_NOTIFY, 0, 0, 0, 0, 0, 0);

			CGateServer::GetSingletonPtr()->Send2Scene(i, &tMessage );
		}
	}
	*/

	// 如果是新的一天，模拟erating发送防沉迷玩家报告
	#ifdef YUENAN
	if( IsANewDay( mLatestResetTime, nNow ) )
	{
		mLatestResetTime = nNow;
		ResetFcmStatus();
	}

	#endif
}

void CServiceGateFcm::OnMessage( int nServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);	

	CMessageHead* tpHead = tpMessage->mutable_msghead();
	unsigned short nMessageID = (unsigned short) tpHead->messageid();

	switch (nMessageID)
	{	
		case ID_S2G_FANGCHENMI_REQUEST:
		{
			OnMessageFangChenMiRequest(nServerID, tpMessage);
			break;
		}

		case ID_S2G_FCMPLAYERRESET_REQUEST:
		{
			#ifdef YUENAN
			if ( CGateLogic::GetSingleton().mConfig.bFcmIsFake)
			{
				OnMessageFCMPlayerResetRequest(nServerID, tpMessage);
			}
			#endif
			break;
		}

		default:
		{
			LOG_ERROR("default", "[%s:%d]there is no action for the message_id(%d)", 
				__FUNCTION__, __LINE__, nMessageID);
			return;
		}
	}
}

void CServiceGateFcm::OnMessageFCMPlayerResetRequest( int nServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageFCMPlayerResetRequest *tpMsg = ( CMessageFCMPlayerResetRequest * )tpMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().GetPlayerByAcct( tpMsg->accountid());
	if( tpPlayer == NULL ) return;

	FCMPLAYERID_MAP::iterator iter = mFcmPlayerList.find(tpPlayer->mCharID);
	if ( iter == mFcmPlayerList.end()) return; 

	iter->second.second = tpMsg->seconds();
	tpPlayer->mValidSeconds = tpMsg->seconds();

	CMessage tMessage;
	CMessageFCMPlayerListNotify tFactMsg;
	tFactMsg.add_reportplayer(tpPlayer->mCharID);

	tMessage.set_msgpara((uint32_t) &tFactMsg);
	pbmsg_sethead(&tMessage, ID_G2S_FCMPLAYERRESET_NOTIFY, 0, 0, 0, 0, 0, 0);

	CGateServer::GetSingletonPtr()->Send2Scene(nServerID, &tMessage );
}

void CServiceGateFcm::OnMessageFangChenMiRequest( int vServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);

	CMessageFangChenMiRequest *tpMsg = ( CMessageFangChenMiRequest * )tpMessage->msgpara();

	for (int i = 0; i < tpMsg->roledetail_size(); ++i)
	{
		CMessagePlayerLeaveMapNotice tPlayerLeave; 

		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(tpMsg->roledetail(i).charid());

		tPlayerLeave.set_charid(tpMsg->roledetail(i).charid());
		tPlayerLeave.set_charlevel(tpMsg->roledetail(i).charlevel());
		tPlayerLeave.set_charmetier(tpMsg->roledetail(i).charmetier());
		tPlayerLeave.set_money1(tpMsg->roledetail(i).money1());
		tPlayerLeave.set_money2(tpMsg->roledetail(i).money2());
		tPlayerLeave.set_exp(tpMsg->roledetail(i).exp());

		CPlayerManager::GetSingleton().SendUserLogoutProto2ERating(tpPlayer, &tPlayerLeave, 2);
	}
}

unsigned int CServiceGateFcm::CountSize()
{
	return sizeof(CServiceGateFcm);
}


CServiceGateFcm::CServiceGateFcm()
{
	if( mShmPtr->GetInitMode() == SHM_INIT )		
	{
		Initialize();
	}else
	{
		Resume();
	}
}

CServiceGateFcm::~CServiceGateFcm()
{

}

int CServiceGateFcm::Initialize()
{
	return 0;
}

int CServiceGateFcm::Resume()
{
	return 0;
}

void* CServiceGateFcm::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CServiceGateFcm::operator delete( void* pointer )
{

}

void CServiceGateFcm::InsertFcmPlayer( unsigned int nCharID, int nValidSeconds)
{
	mFcmPlayerList.insert(FCMPLAYERID_MAP::value_type(nCharID, TREPORT_VALID_TIME(time(0), nValidSeconds)));

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nCharID );
	if( tpPlayer == NULL ) return;

	// 第一次插入向场景报告一次
	SendFcmNotification(tpPlayer);
}

void CServiceGateFcm::SendFcmNotification(CGatePlayer* tpPlayer)
{
	FCMPLAYERID_MAP::iterator iter = mFcmPlayerList.find(tpPlayer->mCharID);
	if ( iter == mFcmPlayerList.end()) return;

	CMessage tMessage;
	CMessageFangChenMiResponse tFactMsg;

	tFactMsg.set_charid(tpPlayer->mCharID);
	PBSubjectAmount *pSubject = tFactMsg.add_subjects();
	pSubject->set_subjectid(100);	

	int nElapsedTime = time(NULL) - iter->second.first;
	pSubject->set_amount(iter->second.second > nElapsedTime ? (iter->second.second - nElapsedTime) : 0);

	pbmsg_sethead(&tMessage, ID_G2S_FANGCHENMI_RESPONSE, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tFactMsg);

	CGateServer::GetSingleton().Send2Scene(tpPlayer, &tMessage);
}

void CServiceGateFcm::RemoveFcmPlayer( unsigned int nCharID)
{
	mFcmPlayerList.erase(nCharID);
}

void CServiceGateFcm::UpdateFcmPlayer( unsigned int nCharID, int nValidSeconds)
{
	FCMPLAYERID_MAP::iterator iter = mFcmPlayerList.find(nCharID);
	if ( iter != mFcmPlayerList.end())
	{
		iter->second.second = nValidSeconds;
	}
}

void CServiceGateFcm::ResetFcmStatus()
{
	CMessage tMessage;
	CMessageFCMPlayerListNotify tFactMsg[SCENE_AT_GATE_CAP+1];

	FCMPLAYERID_MAP::iterator iter = mFcmPlayerList.begin();
	for ( ; iter != mFcmPlayerList.end(); ++iter )
	{
		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( iter->first );
		if( tpPlayer == NULL ) continue;

		iter->second.second = 3600 * 5;
		tpPlayer->mValidSeconds = 3600 * 5;

		CSceneServer* pHost = 
			CGateServer::GetSingleton().GetServerByLineMap(tpPlayer->mLineID, tpPlayer->mMapID);

		if ( pHost == NULL || pHost->mServerID > SCENE_AT_GATE_CAP || pHost->mServerID < 1) continue;

		tFactMsg[(int)pHost->mServerID].add_reportplayer(tpPlayer->mCharID);
	}

	for ( int i = 1; i <= CGateServer::GetSingleton().GetMaxSceneServerID(); ++i) 
	{
		if ( CGateServer::GetSingleton().GetServerObjID(i) == INVALID_OBJ_ID ) continue;
	
		if ( tFactMsg[i].reportplayer_size() > 0)
		{
			CMessage tMessage;
			tMessage.set_msgpara((uint32_t) &tFactMsg[i]);
			pbmsg_sethead(&tMessage, ID_G2S_FCMPLAYERRESET_NOTIFY, 0, 0, 0, 0, 0, 0);

			CGateServer::GetSingletonPtr()->Send2Scene(i, &tMessage );
		}
	}
}

void CServiceGateFcm::ReloadCfg()
{
	mFcmRequestInterval = CGateLogic::GetSingleton().mConfig.nFcmWarningTimer;
}

