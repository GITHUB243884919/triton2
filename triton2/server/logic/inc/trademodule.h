#pragma once

#include "tradetype.h"
#include "trade.h"
#include "trademanager.h"
#include "trademessage_pb.hxx.pb.h"
#include "sceneobj_define.h"
#include "sceneobj_manager.h"
#include "propertymodule.h"
#include "servercore.h"
#include "servertool.h"

// ********************************************************************** //

// CTradeModule

// ********************************************************************** //


class CTradeModule : public CLogicModule, public CSingleton< CTradeModule >
{
public:
	static char	sModuleName[ NAME_LENGTH ];
	
public:
	int mTimePile;
	
public:
	CTradeModule() : CLogicModule( sModuleName )
	{
		if ( mShmPtr->GetInitMode() == SHM_INIT )
		{
			mTimePile = 1000;	
		}	
	}

	static unsigned int CountSize();
	
public:
	virtual void OnLaunchServer( );
	virtual void OnExitServer();
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );
	virtual void OnRouterMessage( CMessage* pMessage );
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );
	virtual void OnCreateEntity( CEntity* pEntity , CCreator* pCreator, bool vFirst );
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

	// 加载配置
	virtual void OnLoadConfig( ) ;

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }

	// 交易邀请
	void OnMessageTradeInvite( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 邀请处理
	void OnMessageTradeInviteProc( CEntityPlayer* pPlayer, CMessage* pMessage );	
	
	// 更新交易物品列表: 添加、移除
	void OnMessageUpdateTradeGoods( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 发送交易金额
	void OnMessageSendTradeMoney( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 锁定交易信息
	void OnMessageLockTrade( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 确认交易处理
	void OnMessageSubmitTrade( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 关闭交易
	void OnMessageCloseTrade( CEntityPlayer* pPlayer, CMessage* pMessage );
		
	// 交易邀请消息转发至客户端
	void OnSendTradeInviteCallBack(CEntityPlayer* pPlayer, unsigned int vEntityID );	

	// 通知客户端开始交易
	void OnSendTradeBeginCallBack( CEntityPlayer* pPlayer, unsigned int vEntityID );	

	// 取消交易
	void OnCancelTrade( int vEntityID );

	// check player service status
	int CheckPlayerService( CEntityPlayer* pSrcPlayer, CEntityPlayer* pDesPlayer ); 

	void SendErrorNotice( CEntityPlayer* pPlayer, int vCode );
};


