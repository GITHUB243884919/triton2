#include "stdafx.hpp"
#include "ahthunk.h"
#include "scenelogic.h"
#include "coremessage_pb.hxx.pb.h"

#ifdef _ANTIBOT_
#include "ApexProxy.h"
#include <pthread.h>
#endif

template<> CAHContorlManager* CSingleton< CAHContorlManager >::spSingleton = NULL;

CAHContorlManager::CAHContorlManager()
{
	if( CObj::msCreateMode )
		mPlayerDelayProcList.initailize();
}


CAHContorlManager::~CAHContorlManager()
{

}

int CAHContorlManager::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	/*
	 * 目前的错误码都５位长，且没有非1,4,6 开头的错误码
	 * a. 以1,6开头、47200　等错误号弹提示框信息“与服务器连接断开”，
	 * b. 47001、47011　保持当前原则不变（客户端多开) 
	 * c. 其余错误号（以4开头的）可以弹“第三方软件”。
	 *
	 */

 	APEX* pApexCfg = CSceneCfgManager::GetSingletonPtr()->GetApex();

	for( PLAYERID_VEC::iterator it = mPlayerDelayProcList.begin(); it != mPlayerDelayProcList.end(); ++it )
	{
		if( it->second.mReason == (int)(pApexCfg->mMulti_client) )
		{
			CSceneLogic::GetSingletonPtr()->KickPlayer( it->first, EM_KICKOUT_ANTIBOT_MULTICLIENT );
		}
		else if( it->second.mReason == (int)(pApexCfg->mVm )  ) 
		{
			CSceneLogic::GetSingletonPtr()->KickPlayer( it->first, EM_KICKOUT_ANTIBOT_VM );
		}
		else if( it->second.mReason == (int)(pApexCfg->mMulti_user ) )
		{
			CSceneLogic::GetSingletonPtr()->KickPlayer( it->first, EM_KICKOUT_ANTIBOT_MULTIUSER );
		}
		else if( it->second.mReason == 47200 
			|| (it->second.mReason  >= 10000 && it->second.mReason  < 20000)
			|| (it->second.mReason  >= 60000 && it->second.mReason  < 70000))
		{
			CSceneLogic::GetSingletonPtr()->KickPlayer( it->first, EM_KICKOUT_ANTIBOT_OTHER );
		}
		else
		{
			CSceneLogic::GetSingletonPtr()->KickPlayer( it->first, EM_KICKOUT_ANTIBOT );
		}
	}

	mPlayerDelayProcList.clear();

	return 0;
}


// 加入到延迟下线列表里
int CAHContorlManager::AddToDelayProcessList( const CAHDelayData& rDelayData )
{
	mPlayerDelayProcList.insert( std::pair<int,CAHDelayData>( rDelayData.mCharID, rDelayData ) );
	return 0;
}



#ifdef _ANTIBOT_

// 线程互斥信号量
static pthread_mutex_t g_stGlobalMutex;	

// 定义接收函数
_FUNC_S_REC pfRec = NULL;

#endif

/* GameServer提供给Apex Proxy 的发送函数;
参数：pBuffer: 发送数据的缓冲区
nLen : 发送数据的长度
*/
long NetSendToGameClient(signed int nSendId,const char * pBuffer,int nLen)
{
#ifdef _ANTIBOT_
	// 加锁
	pthread_mutex_lock( &g_stGlobalMutex);

	LOG_DEBUG( "default", "send antidata to client, player %d len %d", nSendId, nLen );

	CMessage tMessage;
	CMessageSendAntiBotDataToClientNotify tAntiBotMsg;

	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_SERVERANTIBOTDATA );
	tMessage.set_msgpara( (uint32)&tAntiBotMsg );

	tAntiBotMsg.set_serverdata( (const void*)pBuffer, nLen );
	tAntiBotMsg.set_length( nLen );

	CSceneLogic::GetSingletonPtr()->Send2Player( nSendId, &tMessage );

	// 解锁
	pthread_mutex_unlock( &g_stGlobalMutex);

#endif
	return 0;
}

/* GameServer提供的踢人函数
参数:nId: 玩家的id
实现这个函数,把一个玩家踢下线;
*/
long GameServerKillUser(signed int nId, int nAction)
{
#ifdef _ANTIBOT_

	// 加锁
	pthread_mutex_lock( &g_stGlobalMutex);

	LOG_INFO( "default", "[%s:%d] AS Server kill %d, reason %d", __FUNCTION__, __LINE__, nId, nAction );

	CAHDelayData tDelayData;
	tDelayData.mCharID = nId;
	tDelayData.mReason = nAction;

	CAHContorlManager::GetSingletonPtr()->AddToDelayProcessList( tDelayData );

	// 解锁
	pthread_mutex_unlock( &g_stGlobalMutex);
#endif
	return 0;
}


int StartApexProxy()
{
#ifdef _ANTIBOT_

	APEX* tpApex = CSceneCfgManager::GetSingletonPtr()->GetApex();
	if( tpApex != NULL )
		INIT_ROLLINGFILE_LOG( tpApex->mIdentifier, tpApex->mDirectory, ( LogLevel ) tpApex->mPriority , tpApex->mMaxfilesize, tpApex->mMaxbackupindex);	

	//必须按以下顺序调用;
	//1 . CHSStart ;2 . CHSSetFunc
	CHSStart(NetSendToGameClient,pfRec);
	CHSSetFunc((void*)(GameServerKillUser),FLAG_KILLUSER);
	LOG_NOTICE( "apex", "ApexProxy Start ... ");

	// 初始化线程互斥信号量
	if( pthread_mutex_init( &g_stGlobalMutex, NULL ) != 0 )
	{
		LOG_FATAL( "default", "thread mutex for antibot moudle init failed. so exit" );
		exit(-1);
		return -1;
	}
#endif
	return 0;
}

//功能 :停止ApexProxy
int StopApexProxy()
{
#ifdef _ANTIBOT_
	CHSEnd();
	LOG_INFO( "default", "ApexProxy Stop ... ");
#endif
	return 0;
}

//
//功能      :通知,ApexProxy,玩家进入;
//          DoDataRec 中调用;
//参数
//nSendId  玩家的id
int NoticeApexProxy_UserLogin( int nSendId )
{

#ifdef _ANTIBOT_

	LOG_NOTICE( "apex", "Player %d Login, Notify AS Server", nSendId );

	//要保证,每一个玩家,进入的时候,都调用这个函数;
	//Apex Proxy 只允许一个玩家 只调用 1 次 这个函数;
	//其中,nSendId,是用来代表这个玩家的Id
	if(pfRec)
	{
		CEntityPlayer* pPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID( nSendId );
		if( pPlayer == NULL )
		{
			return -1;
		}

		pfRec('L',nSendId, pPlayer->GetCharNameShow(), strlen( pPlayer->GetCharNameShow() ) );

		char szBuf[5] = {0};

		szBuf[0] = 1;
		memcpy( (void*)(&szBuf[1]), (const void*)(&pPlayer->GetClientInfo()->mClientAddr), sizeof(int) );

		pfRec('S',nSendId,(const char*)szBuf,5);
	}
#endif

	return 0;
};


int	NoticeApexClient_Ret(int nSendId, const char * pRet)
{
#ifdef _ANTIBOT_
	if(pfRec)
	{
		pfRec('R',nSendId,(char*)pRet,4);
	}
#endif
	return 0;
}



//
//功能    :通知,ApexProxy,玩家离开;
//         DoDataRec 中调用;
//参数
//nSendId  玩家的id
int	NoticeApexProxy_UserLogout( int nSendId )
{
#ifdef _ANTIBOT_

	LOG_NOTICE( "apex", "Player %d Logout, Notify AS Server", nSendId );

	//要保证,每一个玩家,离开的时候,都调用这个函数;
	//Apex Proxy 允许一个玩家 调用几次 这个函数;
	CEntityPlayer* pPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID( nSendId );
	if( pPlayer == NULL )
	{
		return -1;
	}

	if(pfRec)
	{
		pfRec('G',nSendId, pPlayer->GetCharNameShow(),strlen(pPlayer->GetCharNameShow()));
	}
#endif
	return 0;
}; 



//
//功能     :通知,AepxProxy,有数据到达;;
//         DoDataRec 中调用;
//参数
//nSendId 玩家的id
//pBuf     : 发送数据的缓冲区
//nBufLen  : 发送数据的长度
int	NoticeApexProxy_UserData(int nSendId,const char * pBuf,int nBufLen)
{
#ifdef _ANTIBOT_
	if(pfRec)
	{
		pfRec('T',nSendId,pBuf,nBufLen);
	}
#endif
	return 0;
}; 




