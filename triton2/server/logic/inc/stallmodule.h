#pragma once

#include "stall.h"
#include "servercore.h"
#include "servertool.h"

class CStallModule : public CLogicModule, public CSingleton< CStallModule > 
{
public:
	static char	sModuleName[ NAME_LENGTH ];
	
	// module config data handle
	STALL_CFG* tpCfg;

public:
	CStallModule( ) : CLogicModule( sModuleName )
	{
		tpCfg = CSceneCfgManager::GetSingleton().GetStall_cfg();
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

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity , CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );

	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

	// 加载配置
	virtual void OnLoadConfig( ) {};

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }

public:
	// to begin process client message

	// 打开摊位
	void OnMessageOpenStall( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 进度读取
	void OnMessageReadProcessRtn( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 摊位商品更新：上架、下架
	void OnMessageUpdateGoods( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 更新商品价格
	void OnMessageUpdatePrice( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 玩家点击摊位
	void OnMessageClickStall( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 玩家购买物品
	void OnMessageBuyGoods( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 收摊
	void OnMessageCloseStall( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 关闭摊位界面
	void OnMessageCloseStallDlg( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 更新摊名和广告内容
	void OnMessageUpdateText( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnMessageGetStallRecord( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageClearStallRecord( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 对进入可见范围的玩家发送摊位消息
	void TouchStallPedal( CEntityPlayer* pPlayer, PBStallInfo* pStallInfo );

	// send message to client

	void SendErrorNotice( CEntityPlayer* pPlayer, int vCode );

	// 发送开始摆摊时错误的消息给摆摊者
	void SendOpenStallError( CEntityPlayer* pPlayer, int nErrCode );

	// print trade logs
	void PrintTradeLog( 
		const char* pMaster, 
		int nMasterID,
		const char* pBuyer,
		int nBuyerID,
		unsigned int vItemID, 
		unsigned int vNumber,
		unsigned int vPrice,
		const int vPriceType);

	// save
	void SaveStallData( CEntityPlayer* pPlayer );
	int GetStallData( CEntityPlayer* pPlayer, CMessageStartStallNotify* pMessage );

	//send message to db
	//发送元宝摆摊的交易消息给DB
	void SendExchageRequest(CEntityPlayer* pBuyer,CEntityPlayer* pSeller, const CStallRecord* tpRecord, const int nIBFeeID );


	// 处理元宝交易数据库超时未返回
	void OnSessionTimeoutYbStallExchange( CSession* pSession );

	// 处理元宝交易数据库返回
	void OnSessionYbStallExchangeResponse( CMessage* pMsg, int nParam1, int nParam2 );
};


