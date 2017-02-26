#pragma once

#include "clienthandle.h"
#include "westserver.h"
#include "protype.h"
#include "entity.h"
#include "tcp_conn.h"
#include "sceneobj_define.h"
#include "dbinterface.h"
#include "scenecfg_manager.h"
#include "event.h"
#include "ahthunk.h"
#include "lk_hashmap.h"
#include "logmodule.h"

namespace SceneServer
{

// ********************************************************************** //
// CSceneServer
// ********************************************************************** //
//class CClientHandle;
class CSceneLogic;
class CModuleManager;

class CSceneServer : public CSingleton< CSceneServer >
{
public:

	CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> mProxyClient;

	CClientHandle*	mClientHandle;

	unsigned int	mLastTickCount;
	CSceneLogic*	tpSceneLogic;	
	CModuleManager*	tpModuleManager;

	int				mRunFlag;
	int				mPingCheck;
	bool			mbGateIsRegisted;					// 判断是否向网关服务器注册成功

	lk::string<64> mProxyServerAddr;	// 网关服务器地址
	lk::string<64> mSceneserverWanAddr;	// 场景广域网服务器地址
	lk::string<64> mSceneName;			// 场景服务器名称
	lk::string<64> mSceneServerLanAddr; // 场景服务器局域网地址
	unsigned char mThisServerID;	// 场景服务器ID

	lk::string<64> mSavePath;			// dberr, save path
	lk::string<64> mBakPath;			// dberr, bak path

	CountryArrary mCountryArrary[ _COUNTRY_COUNT_MAX_ ];

	CWTimer mTimerOfKeepAlive;
	time_t	mLastPorxyKeepAliveTime;
	time_t	mLastGateKeepAliveTime;
	time_t  mLastLogStatInfoTime;

	int				mLaunchStatus;
	int				mCreateMapStatus;

	lk::vector<int, 20> mDeleteList;

	// 重新加载单个配置功能相关
	typedef void(*TmLoadConfigCallback)(const char*);
	typedef lk::hash_map<lk::string<256>, TmLoadConfigCallback, 1000> LoadConfigFunclist;
	LoadConfigFunclist m_LoadConfigFuncList;
	
public:
	CSceneServer( );
	~CSceneServer( );

	void* operator new( size_t size );
	void  operator delete( void* pointer );
	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);


public:

	enum ERunFlag
	{
		EFLG_CTRL_NO				= 0,
		EFLG_CTRL_QUIT				= 1,
		EFLG_CTRL_RELOAD			= 2,
		EFLG_CTRL_COREDUMP			= 3,
	};


	//// 关闭服务器
	//void ShutDownServer( ); 

	void SendKeepAliveToGate();
	int SendKeepAliveToProxy();

	unsigned int GetLastTickCount( ) { return mLastTickCount; }

	const char* GetCountryName( unsigned short mLindID );

	static CSharedMem* mShmPtr;
	static unsigned int CountSize();
	
	int SendMsg(CEntityPlayer* pPlayer, CMessage* pMsg);
	int SendMsg(int vCommNum, CEntityPlayer** pPlayer, CMessage* pMsg );

	int Initailize( int vInitFlag );
	void CheckTimer();
	int PrepareToRun();
	void Run();
	void CheckRunFlags();
	int CheckLaunchServer();

	void SetRunFlag( int vFlag );
	void ClearRunFlag( );
	bool IsRunFlagSet( int vFlag );

	// 接收外部消息
	int RecvServerMsg(int vMsgCount);
	int RecvClientMsg(int vMsgCount);

	// 处理消息
	int ProcessDataMsg( CMessage* vMsg );
	int ProcessRouterMsg( CMessage* vMsg );
	int ProcessClientMsg( CMessage* vMsg );
	int ProcessTimeOutMsg( CMessage* vMsg );
	int ProcessDunserverMsg( CMessage* vMsg );

	//SaveAllPlayer
	int SaveAllPlayer( int vSaveCode );

	int LoadPlayerFromDB( unsigned int nAccountID, unsigned int vCharID, int nDesGameID = 0 );
	int LoadPlayerItemInfoFromDB( int nAccountID, int nCharID, int* panPkgIdx, int nNumber, int nDesGameID = 0 );
	int OnRouterMessageByModule(CMessage* pMessage);
	
	// 消息处理函数
	int OnGlobalGateMessage( CMessage* pMsg );
	int OnGlobalClientMessge ( CEntityPlayer* pPlayer, CMessage* pMsg );
	int OnMessageLeaveGameRequestEvent( CMessage* pMsg, CEntityPlayer* pPlayer );
	int OnMessageRedirectSceneRequestEvent( CMessage* pMsg, CEntityPlayer* pPlayer ); 
	int OnMessageLoginSceneRequestEvent( CMessage* pMsg, CEntityPlayer* pPlayer );
	int OnMessageReturn2GateRequestEvent( CMessage* pMsg, CEntityPlayer* pPlayer );
	int OnMessageSceneNotOpenNoticeEvent( CMessage* pMsg ); 
	int OnMessagePlayerRedirectResponseEvent( CMessage* vpMsg );
	int OnMessageCreateMapNoticeEvent( CMessage* pMsg );
	int OnMessagePlayerEnterMapNoticeEvent( CMessage* pMsg );
	int OnMessagePlayerReturn2GateReponseEvent( CMessage* pMsg );
	int OnMessagePlayerRedirectMapNotice( CMessage* pMsg );
	int OnMessagePlayerPingRequest( CMessage* pMsg, CEntityPlayer* pPlayer);
	int OnMessagePlayerRedirectErrEvent( CMessage* vpMessage ); 
	int OnMessageKickOffPlayerRequest( CMessage* vpMessage ); 
	int OnMessageClientInfoRequest( CMessage* vpMessage, CEntityPlayer* pPlayer );
	int OnMessageTracePlayerNotify( CMessage* vpMessage );
	int OnMessageCreateRepetion( CMessage* vpMessage );
	int OnMessageServerInfoResponse(CMessage* vpMessage);
	int OnMessageSceneGMMove( CMessage* vpMsg );	  	
	int OnMessageRoleMove( CMessage* vpMsg );
														 
	// 重设玩家安全密码的消息
	void OnMessageResetPlayerPassword( CMessage* vpMsg );

	// 处理客户端反外挂数据
	int OnMessageClientAntiBotDataNotify( CMessage* pMsg, CEntityPlayer* pPlayer );

	// 处理客户端反外挂模块启动结果
	int OnMessageClientAntiBotStartRetNotify(  CMessage* pMsg, CEntityPlayer* pPlayer );

	int OnMessageGetCountryInfo( CMessage* pMsg );
	
	int SendRedirectErrCode( int vCharID, unsigned short vLineID, unsigned short vMapID, REDIRECT_ERR vCode );
	
	// 通知网关服务安全关闭
	int SendShutdownAck( );

	// 处理网关通知关闭服务器
	int OnMessageShutdownNotify( CMessage* pMsg );

	// 读配置
	int ReadConfig( );
	
	// 读服务器级配置
	int ReadServerConfig( );

	int ConnectToServer( );

	bool Connect2Proxy();
	bool Regist2ProxyServer();
	bool Regist2GateServer();

	static void OnMessage( int vCommHandle, CMessage* pMessage );
	static void OnClose( int vCommHandle );
	static void OnAccept( int vCommHandle );
	static void OnTimer( unsigned int vTickCount );

	///////////////////////////////////
	// 消息：转阵营（转服）消息
	// author： qiufufu
	// date : 10-08-23
	//////////////////////////////////////
	//处理 客户端申请转阵营
	void OnMessageInviteCamp( CEntityPlayer* pPlayer,CMessage* pMessage);
	//处理 客户端转阵营消息
	void OnMessageChangeSingleCamp(CEntityPlayer* pPlayer,CMessage* pMessage);
	//处理 gateserver 回复的转阵营消息
	void OnMessageChangCampResponse( CMessage* pMessage);
	//处理客户端 发送查询转换阵营条件
	void OnMessageChangeCampConditionRequst(CEntityPlayer *pPlayer, CMessage* pMessage);
	//处理转阵营成功 广播消息
	void OnMessageChangeCampSuccesBroadCast(CEntityPlayer *pPlayer, CMessage* pMessage);

	//发送 转阵营错误 消息
	void  SendChangeCampErrorResponse(CEntityPlayer *pPlayer,ChangeCamp errorCode);

	//gm 强制转换阵营
	void OnGMMessageChangeCamp(const char*  role_name,int campid);

	// 重新加载指定配置
	int ReLoadAppointedConfig();

	// 重新加载单个配置
	int ReLoadOneConfig(const char* pcConfigFilePath);

	// 注册配置加载的回调函数
	// 两种方式注册：
	// 1 以配置文件的路径注册 
	// 2 以文件夹、文件扩展名(如.lua,.txt)注册
	int RegisterLoadConfigCallback(const char* pcConfigPath, TmLoadConfigCallback pFuncCallback);
	int RegisterLoadConfigCallback(const char* pcDirPath, const char* pcExtName, TmLoadConfigCallback pFuncCallback);

	static void LoadDirtyWordCallback(const char* pcConfigPath);
	static void LoadTemplateCallback(const char* pcConfigPath);
	static void LoadSceneConfigCallback(const char* pcConfigPath);
};

// ********************************************************************** //
// CSceneLogic
// ********************************************************************** //

enum emServerStauts
{
	EM_SERVERSTATUS_LOADING	= 0,	// 服务器启动，加载中
	EM_SERVERSTATUS_RUN,			// 服务器运行中
	EM_SERVERSTATUS_SHUDOWNING,		// 服务器关闭中
	EM_SERVERSTATUS_EXIT,			// 服务器退出关闭
};

class CSceneLogic : public CDBReceiver, public CSingleton< CSceneLogic >
{

	friend class CSceneServer;

public:
	typedef lk::hash_map< int, int, SERVER_CAP_PLAYER >							PLAYERID_MAP;	// Player实体容器
	typedef lk::hash_map< int, int, SERVER_CAP_NPC >							NPCID_MAP;		// NPC实体容器
	typedef lk::hash_map< lk::string<NAME_LENGTH>, int, SERVER_CAP_PLAYER >		PLAYERNAME_MAP;	// 玩家姓名容器
	typedef lk::vector< int, SERVER_CAP_PLAYER >								PLAYERID_VEC;	// 玩家容器

	typedef lk::vector< unsigned int, DELETE_LIST_SIZE >						DELETE_LIST_TYPE;		// 延迟删除列表


	PLAYERID_MAP								mPlayerCommIndex;	// 在场景中的玩家,CommHandle作为索引
	
	static CSharedMem*	mShmPtr; // 共享内存指针

	unsigned int								mCurrentDay;		// 服务器刷新的日期
	CEvent										mEvent;

	//CStatInfo 									mStatInfo;			// 服务器的统计信息

protected:
	PLAYERID_MAP								mPlayerInWait;		// 等待连接的玩家,CharID作为索引
	PLAYERID_VEC								mPlayerInLost;

	// 玩家索引
	PLAYERNAME_MAP								mPlayerNameIndex;	// 在场景中的玩家,角色名称作为索引
	PLAYERID_MAP								mPlayerContainer;	// 在场景中的玩家,CharID作为索引
	NPCID_MAP									mNpcContainer;		// 在场景中的NPC(怪物，买卖商人，拍卖商人，油箱，场景中的踏板等等,换句话说就是一切非玩家), EntityID作为索引

	// 

	// 命令处理

	//CSessionDispatch							mSessionDispatch;

	enum 
	{
		KICK_CHECK_GAP = 60000, // 踢不活跃玩家检查时间间隔,单位:毫秒
	};

	CWTimer										mDailyTimer;		// 每日计时器
	CWTimer										mTimer;				// 逻辑处理计时器
	CWTimer										mStateCheckTimer;	// 玩家在线逻辑处理计时器
	int											mTemplateVersion;	// 模板文件版本号
	int											mTaskVersion;		// 任务文件版本号
	int											mSavePerf;			// 距上次保存统计信息时间(毫秒)
	CWTimer										mTimeMemCheck;		// 内存检查间隔(毫秒) 
		

	CAHContorlManager							mAHControlMng;		// 反外挂管理器

	char mConfigtmpbuf[1024];
	
	

	int mServerStatus; // 服务器状态 emServerStauts
	int mWorldID;	// 服务器唯一大区ID. gate_id
	int mLineID;	// 服务器在本大区内所属的line_id
	int mGameID;	// 游戏ID
	DELETE_LIST_TYPE							mDeleteList;		// 延迟删除列表 所有module都走这个 在消息处理结束和ontimer结束处理
	
	CMessagePlayerProperty						mSavePlayerProperty;

public:
	CSceneLogic( );
	~CSceneLogic( );

	void* operator new( size_t size );
	void  operator delete( void* pointer );
	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);

	static unsigned int CountSize( );

	int Initailize( );
	int Resume();

public:

	//// 关闭服务器
	//void ShutDownServer( ) { mServerStatus = EM_SERVERSTATUS_SHUDOWN; }

	//// 服务器是否开放
	//bool IsServerOn( ) { return mServerStatus == EM_SERVERSTATUS_RUN; } 

	//// 是否能退出程序
	//bool CanExitServer( ) { return mServerStatus == EM_SERVERSTATUS_EXIT; }

	int GetServerStatus() { return mServerStatus; }
	void SetServerStatus( int nStat ) { mServerStatus = nStat; }

	// 读配置
	int ReadConfig();

	// 得到当前天数
	unsigned int GetCurrentDay() {return mCurrentDay;}

	// 每天刷新
	void DailyRefresh();

	// 每周刷新
	void WeeklyRefresh();

	// 分步保存

	void OnSavePlayer( int nSaveCode, int nEntityPlayerID, int nResultCode );
	int SaveErr( CEntityPlayer* vpPlayer );

	// 新增的分步读取函数
	void OnLoadPlayer_CharProfile( CMessage* pMessage, int vParameter1, int vParameter2 );
	void OnLoadPlayer_Property( CMessage* pMessage, int vParameter1, int vParameter2 );
	void OnLoadPlayer_Baggage( CMessage* pMessage, int vParameter1, int vParameter2 );
	void OnLoadPlayer_Storage( CMessage* pMessage, int vParameter1, int vParameter2 );
	void OnLoadPlayer_Task( CMessage* pMessage, int vParameter1, int vParameter2 );
	void OnLoadPlayer_Friend( CMessage* pMessage, int vParameter1, int vParameter2 );
	void OnLoadPlayer_Final( CEntityPlayer* pPlayer, int vCharID, int vCommHandle );
	void OnLoadPlayer_Repetion( CMessage* pMessage, int vParameter1, int vParameter2 );
	void OnLoadPlayer_YuanBao( CMessage* pMessage, int vParameter1, int vParameter2 );

	// on db message 
	int OnMessageLoadPlayerItemInfoFromDB( CMessage* pMsg );
	int OnTimeoutLoadPlayerItemInfoFromDB( unsigned int unCharID, int iPkgIdx  );
	int OnMessageSaveItemInfoToDBResponse( CMessage* pMessage );
	int OnTimeoutSaveItemInfoToDB( int nCharID );
	int OnMessageLoadPlayerProperty( CMessage* pMsg );

	int GetPlayer4KickOut(CMessage*, CEntityPlayer*);
public:
	// 关闭服务器
	void Shutdown( );

	// 启动服务器
	void Launch( );

	// 执行服务器
	void Run(int nDaemon );

	// 得到玩家数量
	int GetPlayerNumber( ) const;

	// 执行SQL
	int ExecuteSql( int vSessionType, int vParam1, int vParam2, int vPlayerCharID, int vCallBack, SQLTYPE vSqlType, const char* pSQL, ... );

	int GetSaveFilePath( int vPlayerCharID, char* pPath, int vPathLen );
	void Save2File( int vSessionType, int vPlayerCharID, ::google::protobuf::Message* vpMsg, const char* pKey, std::vector<std::string>& rFirlds, unsigned int nAccountID );
	void OnMessageResavePlayer( CMessage* pMsg );
	int  ReSavePlayerFromFile( int vCharID );
	int  ReSavePlayerInfoFromXml( const char* xmlfile, int nCharID, unsigned int nAccountID );
	int  OnResaveSuccess( int nCharID, int nResaveFlag );
	void GetPlayerPosInfoFromXml( int vCharID, int& rLineID, int& rMapID, int& rPosX, int& rPosY );

	bool LoadPlayerInfo( int vAccountID, int vPlayerCharID, int nDesGameID = 0 );
	bool SavePlayerInfo( CEntityPlayer* vPlayer, int vSaveCode, int vCallBack );
	bool LoadPlayerProperty( int nAccountID, int vPlayerCharID, int nDesGameID = 0 );
	bool SavePlayerProperty( CEntityPlayer* vpPlayer, int vSaveCode, int vCallBack );

    // 加载摇钱树活动数据
	// TODO:暂时不修改(跨服战场中全部屏蔽)
    void LoadPlayerYaoQianShuInfoFromDB( int nAccountID, int nCharID );
    void OnLoadPlayer_YaoQianShu( CMessage* pMessage, int nCharID, int nAccoutID );
    void LoadPlayerPickStoreBoxFromDB( int nAccountID, int nCharID );
    void OnLoadPlayer_PickStoreBox( CMessage* pMessage, int nCharID, int nAccoutID );

	bool SendMsg2Data( google::protobuf::Message* vpMsg, int Uid, int tMsgID, int nDesGameID = 0 );

	void ClearSavePlayerPropety( ){ mSavePlayerProperty.Clear( ); }
	CMessagePlayerProperty* GetSavePlayerProperty( ) { return &mSavePlayerProperty; }
	 

	// 得到在场景中的指定玩家( 通过accountID索引 )
	CEntityPlayer* GetPlayerByAccountID( int nAccountID );	
	
	// 得到在场景中的指定玩家( 通过CharID索引 )
	CEntityPlayer* GetPlayerByCharID( int vCharID );

	// 得到在场景中的指定玩家( 通过通讯端口索引 )
	CEntityPlayer* GetPlayerByComm( int vCommHandle );

	// 得到在场景中的指定玩家( 通过角色名称 )
	CEntityPlayer* GetPlayerByName( const char* pCharName );

	

	// 得到等待队列中的玩家
	CEntityPlayer* GetWaitingPlayerByCharID( int vCharID );

	// 发送数据到网关
	bool Send2Gate( CMessage* pMessage );

	void Send2GMServer( CMessage* pMessage );

	// 发送消息到日志服务器
	void Send2Log( CMessage* pMessage, int nHandle, int nSplitor, int nDesGameID = 0 );

	// 发送消息给数据服务器
	int Send2Data( BYTE* pMessage, unsigned short vLength );
	int Send2Data( int nUin, CMessage& rMessage, int nDesGameID = 0 );

	// 给全局副本服务器发送消息
	int Send2GlobalDunServer( CMessage* pMessage );

	// 发送消息给当前游戏服务器
	void SendSceneMessage( CMessage* pMessage, int CountryID = -1, int MapID = 0 );

	// 发送消息给整个游戏服务器( 通过网关转发 )
 	void SendWorldMessage( CMessage* pMessage );
 	
	// 发送消息给某个国家( 通过网关转发 )
	void SendCountryMessage( CMessage* pMessage, int nCountryID );
	
	// 发送消息给某个家族( 通过网关转发 )
	void SendFamilyMessage( CMessage* pMessage, int nFamilyID, bool bNeendSendToScene = false );
	
	// 发送消息给整个军团( 通过网关转发 )
	void SendCorpsMessage( CMessage* pMessage, int nCorpsID,  bool bNeendSendToScene = false );
		
	// 通过worldid发送消息（接收方为目标网关）
	void Send2OtherGate( CMessage *pMessage, int nWorldID );	

	// 发送消息给指定客户端,可能通过网关转发
	void Send2Player( const char* pName, CMessage* pMessage );

	// 发送消息给客户端
	void Send2Player( int vCharID, CMessage* pMessage );

	// 发送消息给客户端
	void Send2Player( CEntity* pPlayer, CMessage* pMessage );

	// 发送消息给多个客户端
	void Send2Player(  CEntityPlayer* pPlayer, BYTE* pMessageBuff, int nLen, CMessage* pMessage );

	// 发送消息给多个客户端
	void Send2Player( std::vector< CEntityPlayer*>* rEntityVec, CMessage* pMessage );

	// 得到指定实体
	CEntity* GetEntity( int vEntityID );

	// 建立实体
	void CreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst = true );

	// 删除实体
	void DestroyEntity( CEntity* pEntity, int vSaveCode );

	// 删除玩家实体
	int DestroyPlayerInstance( CEntityPlayer* vpPlayer, EMLeaveReason emLeave, EMSaveResult emSave, int nGlobalSceneWorldID = 0 );

	// 玩家准备离开
	void OnPlayerLeave( CEntityPlayer *pPlayer, int nState );

	// 将玩家踢下线
	int KickPlayer( int vCharID, int nReason = 0, int nfrom = 0);

	// 慎用： 主要用于玩家上次数据存盘不正确后的禁止登陆通知
	void KickPlayerWithoutSave( CEntityPlayer* pPlayer, EMSaveResult emSave );

	// 将所有玩家踢下线
	void KickAllExcept( unsigned int unCharID );

	// 保存玩家数据
	void SavePlayer( CEntityPlayer* pPlayer, int vSaveCode );

	// 保存玩家非blob格式数据
	bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

	// 玩家离开游戏流程
	int PlayerLeaveFromGame(CEntityPlayer* tpPlayer);

	// 得到场景里所有玩家
	int GetPlayersInScene(std::vector<CEntityPlayer*>& rPlayers);
	
	// 创建个人日志目录
	int CreatPlayerLog( );

	// 取得服务器所在的世界ID
	int GetWorldID() { return mWorldID; }
	void SetWorldID (int nWorldID) { mWorldID = nWorldID; }

	// 取得服务器所在世界的lineid
	int GetLineID() { return mLineID; }
	int SetLineID( int nLineID ) { mLineID = nLineID; return 0; }

	int GetGameID() { return mGameID; }
	void SetGameID (int nGameID) { mGameID = nGameID; }

	// 添加到删除列表
	void AddDeleteList( unsigned int unEntityID );
	// 从延迟删除列表里干掉
	void DoDeleteList();

	// 记录贵重物品
	void LogValuableEquip(int nRoleID, CItemObject* pItem);

	// 传送走部分玩家
	int TeleAll( int nMapID, int nPosX, int nPosY );

	// 销毁指定地图NPC
	int DestroyMapNPC( int nMapID, int nMapIndex = 0);

	//获取 config 配置文件的目录
	const char* GetConfigDataFileNamePath(int nGameID, const char * filename);

protected:
	// 建立地图
	int CreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );

	// 销毁场景
	void DestroyScene( const char* pName );

	// 加载地图模版
	int LoadSceneTpl( const char* pFileName );

	// 打印当前在线玩家
	void PrintPlayer( CEntityPlayer* pPlayer );

	// 服务器时间处理
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );

	// 检查处理在线玩家
	void OnCheckOnlieStateEvent( unsigned int vTickCount, unsigned int vTickOffset );

	// 处理数据库回调
	void OnFireSession( CMessage* pMessage );

	// 处理session超时
	void OnSessionTimeOut( int nSessionID );

	// 计算启动日
	void CalcCurrentDay( );
	
	// 检查玩家的消息频率
	bool CheckMsgFrequency( CEntityPlayer *pPlayer );
};


};  
   
#define const_package_size 0x7FFF
