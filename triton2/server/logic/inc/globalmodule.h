#pragma once
#include "dungeontype.h"
#include "servertool.h"
#include "modulemanager.h"
#include "dungeontype.h"
#include "lk_string.h"
#include "lk_hashmap.h"
#include "sceneobj_manager.h"
#include "coretype.h"
using namespace SceneServer;
#define GLOBALWAR_TIMELIMIT_BUFF 77176
struct CGlobalTeleInfo   
{
public:
	lk::CString64 mAddr;	
	int mLoginToken;	
	int mLineID;
	int mMapID;
	int mPosX;
	int mPosY;
	int mTeleType;
	int mSceneType;
	int mSceneWorldID;	// 目标场景的世界ID
public:
	CGlobalTeleInfo()
	{
		mAddr.clear();	
		mLoginToken = 0;
		mMapID = 0;
		mLineID = 0;
		mPosX = 0;
		mPosY = 0;
		mTeleType = 0;
		mSceneType = 0;
	}
};

struct CGlobalWarInfo
{
public:
	int mLevel; // 等级段
	int mStatus;// 状态
public:
	CGlobalWarInfo()
	{
		mLevel = 0;
		mStatus = 0;
	}
};
struct CGlobalMapData
{
public:
	int mMapID;
	int mPosX;
	int mPosY;
	int mLineID;
public:
	CGlobalMapData()
	{
		mMapID = 0;
		mPosX = 0;
		mPosY = 0;
		mLineID =0;
	}
};
class CGlobalModule : public CLogicModule, public CSingleton< CGlobalModule >
{
public:
	CGlobalModule():CLogicModule( "globalmodule" )
	{
		if ( CObj::msCreateMode )
		{
			Initialize();
		}
	}
	~CGlobalModule(){ }
public:
	static unsigned int CountSize();
	int Initialize();
public:
	typedef lk::hash_map< int, CGlobalTeleInfo, MAX_GLOBALTELEINFO_NUM > GlobalTeleInfoList;
	CWTimer mOutTimer;
	CWTimer mSyncTimer;
public:
	void OnServerMessgage( CMessage *pMsg );
	
	// 跨区传送的回应消息
	void OnMsgGlobalTeleResponse( CMessage *pMsg );

	// 可以进入副本的通知
	void OnMessageWarEnableNotify( CMessage *pMsg );
	
	// 玩家报名的消息回应
	void OnMessageApplyWarResponse( CMessage *pMsg );

	// 跨服战场报名
	int PlayerApplyWar( CEntityPlayer *pPlayer, int nRepetionIndex = _REPE_GLOBALWAR_INDEX_ );
	
	// 跨区传送
	int GlobalTelePort( CEntityPlayer *pPlayer, EGlobalTeleType eTeleType , EServerType nServerType = ESERVERTYPE_DUNGEON, CGlobalMapData *pMapData = NULL );

	// 踢出玩家
	void OnMessageKickPlayer( CMessage *pMsg );
public:
	// 启动服务器
	virtual void OnLaunchServer( ){}

	// 退出服务器
	virtual void OnExitServer( ){}

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffsekt );

	// 建立场景
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName ){ return 0; }

	// 路由消息
	virtual void OnRouterMessage( CMessage* pMessage ){}

	// 客户端发送消息
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage ){}

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity , CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );

	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

	// 加载配置
	virtual void OnLoadConfig( ) {}

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }
public:
	// 插入一个全局传送信息
	int InsertGlobalTeleInfo( int nRoleID, CGlobalTeleInfo &rInfo );

	// 获取一个全局传送信息
	CGlobalTeleInfo *GetGlobalTeleInfo( int nRoleID );

	// 删除一个全局传送信息
	int DeleteGlobalTeleInfo( int nRoleID );

	// 离开战场
	void PlayerLeaveGlobalWar( int nRoleID, int nReason = EREASON_LEAVEWAR_RETURNBACK );

	// 结束战场
	void EndGlobalWar( int nReason, int nRegKey );

	// 获取传送类型
	int GetGlobalteleType( int nCharID );

	// 同步凌云渡中的玩家
	int SyncGlobalPlayer();
private:
	GlobalTeleInfoList mGlobalTeleInfo;
};
