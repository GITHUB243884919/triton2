#pragma  once

#include "servertool.h"
#include "modulemanager.h"
#include "dungeonmanager.h"
#include "shm.h"
#include "message_pb.hxx.pb.h"
using namespace DungeonServer;
struct CGlobalTeleData  
{
public:
	int	mLineID;			// 线ID	(当目标场景在本服时需要这些信息)
	int mMapID;			// 地图ID
	int	mPosX;			// 横坐标
	int	mPosY;			// 纵坐标		
	int mTeamFlag;		// 队伍标志
	int mLoginToken;	// 登录令牌
	KEY_TYPE mRepetionKey;// 服务器key
	int	mRepetionIndex;	// 副本索引
	int mSceneWorldID;		// 目标场景的世界ID
public:
	CGlobalTeleData()
	{
		mLineID = 0;
		mMapID = 0;
		mPosX = 0;
		mPosY = 0;		
		mTeamFlag = 0;
		mLoginToken = 0;
		mRepetionKey = 0;
		mRepetionIndex = 0;
		mSceneWorldID = 0;
	}
};
class CDungeonConfig  
{
public:
	int mMaxWarAliveTime;	// 最大的战场存活时间(秒)
	int mWarTimeInterval;	// 战场的开启时间间隔(秒)	
	int mTimeInterval;		// 多久执行一次检查(毫秒)	
	int mWarProctectTime;	// 副本保护时间
	int mActivityTime;		// 活动持续时间
	int mMaxOffLineTime;	// 最大离线不删除时间
	int mMaxWaitTime;		// 最大等待时间
public:
	CDungeonConfig()
	{
		mMaxWarAliveTime = 1200;
		mWarTimeInterval = 30;	
		mTimeInterval = 300;
		mWarProctectTime = 180;
		mActivityTime = 7200;
		mMaxOffLineTime = 60;
		mMaxWaitTime = 30;
	}
};

// 全局玩家信息
class CGlobalPlayer
{
public:
	enum 
	{
		EROLE_STATUS_NULL			= 0,		// 初始状态
		EROLE_STATUS_APPLY			= 1,		// 报名状态	
		EROLE_STATUS_INWAR			= 2,		// 在战场中
		EROLE_STATUS_INGLOBALSCENE	= 3,		// 在全局服务器中(大雷音寺等)
	};
public:	
	int mWorldID;			// 玩家本身的世界ID	
	int mWarRegKey;			// 战场信息ID
	int mStatus;			// 状态
	int mServerKey;			// 玩家所在场景服务器的ID				
	int mRepetionIndex;		// 全局副本索引			
public:
	CGlobalPlayer()
	{
		mWorldID = 0;		
		mWarRegKey = 0;
		mStatus = 0;
		mServerKey = 0;	
		mRepetionIndex = 0;		
	}
};

class CDungeonModule : public CLogicModule
{
public:
	typedef lk::hash_map< int, CGlobalPlayer, MAX_GLOBALPLAYER_NUM > GlobalPlayerList;
public:
	CDungeonModule(): CLogicModule("CDungeonModule")
	{			
		mTimer = CWTimer(1000);		
	}
	~CDungeonModule(){}
public:
	// 启动服务器
	virtual void OnLaunchServer( );

	// 退出服务器
	virtual void OnExitServer( );

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );													

	// 加载配置
	virtual void OnLoadConfig( );

	// 新一天到来
	virtual void OnNewDayCome(  ) { }

	// 新一周到来
	virtual void OnNewWeekCome(  ) { }

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; } 

	// 服务器启动成功
	virtual void OnLaunchComplete( ){ }

	// 接收服务器消息
	virtual void OnServerMessage( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID );

public:
	// 玩家请求跨服
	void OnMessageGlobalTeleportRequest( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID );

	// 发送跨服的消息
	void SendResponseGlobalTeleport( int nErrcode, int nRoleID, int nGameID, int nServerID,  const char  *pGateAddr, CGlobalTeleData &rTeleData );

	// 网关返回跨服通知
	void OnMessageRoleGlobalTeleResponse( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID );
	
	// 创建副本
	int CreateReption( CWarRegInfo *pWarRegInfo, int nServerListID = 0, int nRepetionIndex = 0 );

	// 创建副本的回应消息
	void OnMessageCreateRepetionResponse( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID );

	// 玩家报名
	void OnMessagePlayerApplyWarRequest( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID );	
	
	// 发送报名的回应消息
	void SendApplyWarResponse( int nGameID, int nServerID, int nErrcode, int nRegKey, int nRoleID, int nCanStart, int nMinLevel, int nMaxLevel, int nTeamFlag );

	// 发送可以进副本的通知
	void NotifyWarEnable( CWarRegInfo *pWarRegInfo );

	// 给玩家通知可以进入战场
	void NotifyPlayerWarEnable( int nGameID, int nRoleID, int nWarRegKey, int nTeamFlag );

	// 玩家离开战场的消息通知
	void OnMessagePlayerLeaveWarNotify( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID );	

	// 战场结束的通知
	void OnMessageWarEndNotify( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID );	

	// 网关踢人的消息请求
	void OnMessageKickPlayer( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID );	

	// 网关踢人的通知
	void OnMessageKickGlobalPlayerACK( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID );	

	// 同步凌云渡玩家
	void OnMessageSyncGlobalPlayer( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID );	
public:
	// 从等待列表中添加成员到战场中
	void AddWarMemberFromWaitList();

	// 检查是否有可以开启的战场信息
	void CheckWarRegInfo();
	
	// 删除离线时间过长的玩家
	void RemoveTimeOutRole();

	// 删除无效的战场信息
	void DeleteValidWarRegInfo();
public:
	// 读取全局服务器的配置信息
	int ReadServerConfig( const char *pFile );

	// 读取等级配置
	int ReadLevelConfig( const char *pFile );

	// 读一般配置
	int ReadDungeonConfig( const char *pFile );	

	// 读取游戏配置
	int ReadGameConfig( const char *pFile );	
public:

	// 在全局服务器创建玩家信息
	CGlobalPlayer* RegPlayerToServer( int nRoleID, int nWorldID, int nRepetionIndex = 0 );

	// 传送到全局服务器
	void OnEventTeleToGlobalSceneServer();

	// 返回到原始服务器
	void OnEventReturnToOriginalScene();

	// 创建副本
	void OnEventCreateRepetion();

	// 销毁副本		
	void OnEventDestroyRepetion();
public:
	int InsertGlobalPlayer( int nCharID, CGlobalPlayer &rPlayer );
	CGlobalPlayer *GetGlobalPlayer( int nCharID );
	int DeleteGlobalPlayer( int nCharID );
public:
	// 创建一个副本管理器
	CWarRegInfoManager *CreateWarManager( int nServerListID, int nRepetionIndex );
	// 获取一个副本管理器的信息
	CWarRegInfoManager *GetWarManager( int nServerListID, int nRepetionIndex );
	// 获取一个副本管理器
	CWarRegInfoManager *GetWarManagerByWorldID( int nWorldID, int nRepetionIndex );	
	// 删除副本管理器
	int DeleteWarManager( int nServerListID, int nRepetionIndex );
	// 删除战场信息
	void DeleteWarRegInfo( CWarRegInfo *pWarRegInfo );	
	// 设置战场开启人数
	void SetWarStartNum( int nServerListID, int nWarStartNum );
public:	
	CWarRegInfoManager mWarRegInfoManager[MAX_GLOBAL_REPETION_NUM];
	CGlobalServerConfig mGlobalServerConfig;
	CServerManager mServerManager;
	CLevelConfig mLevelConfig;
	CDungeonConfig mDungeonConfig;
	CWTimer mTimer;
	GlobalPlayerList mGlobalPlayerList;
};