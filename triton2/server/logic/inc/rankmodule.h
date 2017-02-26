#pragma once

#include "servercore.h"
#include "servertool.h"


class CEntityPlayer;
class CMessage;
class PBPersonalRankInfo;
class CCreator;


class CRankModule : public CLogicModule, public CSingleton< CRankModule >
{

public:
	CRankModule( ) : CLogicModule( "RankModule" ){ }
	
	static unsigned int CountSize( );

private:

	
	
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

	// 处理网关消息
	void OnGateMessage(CMessage *pMessage);

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity , CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );

	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

	// 加载配置
	virtual void OnLoadConfig( );

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }

public:
	
	// 客户端取等级排行列表
	int OnClientMessagePlayerRankRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 网关回应取等级排行列表
	int OnGateMessagePlayerRankResponse( CMessage* pMessage );

	// 发送个人排行榜信息到网关
	int SendPersonalRankInfoToGate( CEntityPlayer* pPlayer );

	// 发送多跟排行榜信息到网关
	int SendPersonalRankInfoToGate( std::vector<CEntityPlayer*>& rPlayers, int nType );

	// 设置个人排行信息
	void SetPersonalRank( CEntityPlayer* pPlayer, PBPersonalRankInfo* pInfoTobeSet, int nType );

	int OnGateMessageAskPlayerInfoRequest( CMessage* pMessage );

	// 处理网关发送的请求：新排行榜的top10玩家称号
	int OnGateMessageNewTopTenRequest( CMessage* pMessage );

	// 处理网关发来的指定玩家的top 10 排行榜称号
	int OnGateMessageObtainTopTenResponse( CMessage* pMessage );

	int GetPantaoMark( CEntityPlayer* pPlayer );
	int GetLonggongMark( CEntityPlayer* pPlayer );

	// 排行榜前十的進行稱號作廢處理，以待新榜刷新
	int RepealOldRankTitle( CMessage* pMessage );
	// 转换出排行榜对应的称号
	int ConvertRankToTitle( int vType, int vIndex ,int& vTitleID);
	// 从网关获取指定玩家的排行版信息
	int OnObtainRankTitleRequest( CEntityPlayer* vpPlayer );

	// 特殊类称号处理
	int DisposTitleList( CEntityPlayer* npPlayer );

	// 国籍类称号处理
	int OnEventNationTitle( CEntityPlayer* npPlayer, int nTitleID );
};
