#ifndef _GATE_LOGIC_H_
#define _GATE_LOGIC_H_

#include "entity.h"
#include "message_pb.hxx.pb.h"
#include "coremessage_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "db_msg_pb.hxx.pb.h"
#include "tcp_conn.h"
#include "clienthandle.h"
#include "message_queue.h"
#include "message_dispatcher.h"
#include "gateobj_define.h"
#include "westserver.h"

#ifdef USING_ERATING
#include "Macro.h"
#include "SysProtocol.h"
#include "AGIP.h"
#endif
#define PERF_TIMER	60000		// 一分钟打印一次统计信息  以后走配置

class SysProtocol;
class CAGIPSession;

namespace GateServer
{

// ********************************************************************** //
// CGateServer
// ********************************************************************** //
class CGateLogic;
class CModuleManager;

// 运行标志
enum ERunFlag
{
	EFLG_CTRL_NO        = 0,
	EFLG_CTRL_QUIT      = 1,
	EFLG_CTRL_RELOAD    = 2,
	EFLG_CTRL_COREDUMP	= 3,
	EFLG_CTRL_REFRESHRANK = 4, // 刷排行榜
	EFLG_CTRL_PROCESSEXPIREDMAIL = 5,	// 处理过期的邮件
	EFLG_CTRL_SAFESHUTDOWN	= 6,	// 安全关闭服务器组
};

// ***************************************************************
//  Class:	CConfigure
//  Responsibility:
//  		1. gateserver.xml 的配置数据
//  Restirct:
//  		1. 只存储配置的数据
//	Others:	 		
//  Date:	2009-07-08
//
// ***************************************************************
class CConfigure
{
	public:
		// 到erating的session请求超时时长
		int nEratingSessionTimeout;		

		// 到数据库的session请求超时时长
		int nServerSessionTimeout;		

		// GateServer的时钟周期
		int nGateTimerInterval;			

		// TODO: 不能sleep啊!!
		// 如果收不到客户端的消息, Gate Sleep的时间间隔. 
		int nClientHandleIdleSleep;		

		// 与ProxyServer的心跳同步周期
		int nPingProxyInterval;

		// 向Erating报告状态的周期
		int nReport2ERatingInterval;

		// 各游戏服务器与Game的心跳同步周期
		int nGameHeartBeatTimeout;

		// 玩家状态检查周期
		int nPlayerStateCheckInterval;

		// 等待玩家链接GateServer的超时时间
		int nPlayerWaitConnectTimeout;

		// 玩家在Gate上未做任何操作的超时时间
		int nPlayerIdleIngateTimeout;

		// 通知场景踢人,等待场景回应超时时间
		int nPlayerWaitKickoffTimeout;

		// 等待玩家跨场景回应的超时时间
		int nPlayerWaitRedirectTimerout;

		// 等待玩家登录游戏服务器回应的超时时间
		int nPlayerOnGameStateTimeout;

		// 每场景允许的最大人数
		int nPlayerCountPerScene;

		// 检查玩家防沉迷的周期
		int nPlayerFangChenMiInterval;

		// 组队数据与客户端同步周期
		int nTeamProSynTimeout;

		// 向数据库记录每服务器多少玩家的周期
		int nOnLineNOTimeout;

		// 登录场景的速率，每时钟周期
		int nLoginCountOnceTimer;

		// 玩家默认登录的国家, 地图ID
		int nCampID;
		int nLineID;
		int nMapID;

		// 游戏的唯一ID，体验是54， 正式是4
		int nGameID;

		// 上报的游戏IDLIST
		int mGameList[32];

		char m_szMysqlInfo[256];

		bool bFcmServiceOn; 
		bool bFcmIsFake; 
		int  nFcmWarningTimer;

		int nPlayerActiveTimeout;
		
		// 删除角色的保护时间
		int nDeleteProtectTime;
		
		// 角色删除允许删除时间
		int nDeleteAllowedTime;
		
		// 角色删除冷却时间
		int nDeleteCoolTime;

		// 执行删除操作的时间点(小时)
		int nMailDeleteHour;
		
		// 执行删除操作的时间点(分钟)
		int nMailDeleteMinute;
		
		// 执行退信操作的时间点(小时)
		int nMailBackHour[MAXSERVERNUM];
		
		// 执行退信操作的时间点(分钟)
		int nMailBackMinute[MAXSERVERNUM];
		
		// 邮件表ID
		int nMailTableID[MAXSERVERNUM];
		
		// 安全保护时间的最低等级
		int mProtectLevel;
		
		// 默认安全保护时间
		int mDefaultProtectTime;
		
		// 100304 MAOCY ADD-BEGIN
      // 角色验证问题标志
      int roleValidQuestionFlag;
      // 角色验证问题范围
      int roleValidQuestionRange;
      // 角色验证问题重试
      int roleValidQuestionRetry;
      // 角色验证问题数量
      int roleValidQuestionCount;

		// 军团战争双方人数
		int mCorpsCampCount;
		int mBourseUse;
		// 角色验证问题
		SRoleValidQuestion roleValidQuestions[ROLE_VALID_QUESTION_COUNT];
		// 角色验证选项
		SRoleValidQuestionGroup roleValidQuestionGroups[ROLE_VALID_QUESTION_GROUP_COUNT];
		// 100304 MAOCY ADD-END
		
		int mInternalIPList[MAX_EXCEPTIP_NUM];

		typedef lk::hash_map<unsigned int, int, MAX_MULTI_CLIENT_EXCEPT> MULTICLIENT;
		MULTICLIENT mMultiClientIP;

		// 检查单IP连接数的开关
		int mIsCheckMultiClient;

		// 检查单IP连接数的时间间隔
		int mCheckMultiClientInterval;

		// 单IP允许的连接数 
		int mDefaultClientPerIP;
		
		// 可以直接删除的最低等
		int mDeleteLevel;

		// 同ip下每个mac地址下的链接数
		int mDefaultClientOnIPPerMac;

		CConfigure();
};



// ***************************************************************
//  Class:	CGateServer
//  Responsibility:
//  		1. 维护到proxyserver, clienthandle的链接
//  		2. 维护各sceneserver的数据
//  Restirct:
//  		1. 数据模块
//	Others:	 		
//  Date:	2009-07-08
//
// ***************************************************************
class CGateServer: public CSingleton< CGateServer >
{
	public:
		typedef std::pair< int, int >	MAPTYPE;	
		typedef lk::hash_map< int, int , MAP_AT_GATE_CAP>::iterator	MAPITER;
		typedef lk::hash_map< int, int , MAP_AT_GATE_CAP> MAPCONTAINER;

		struct CountryArrary
		{
			char	mName[ _COUNTRY_NAME_LEN_ + 1 ];
		};

	private:
		// 运行地图列表
		MAPCONTAINER mMaps;

		// 主机容器, 存储的是pHost对象的ID; 位置0 存储的是目前最大的ServerID, 其他位置按照serverID存储pHost的对象
		int mSceneHosts[SCENE_AT_GATE_CAP+1];

		// Gate统一的模板版本号
		int mHostTemplateVer;

		// 国家名称
		CountryArrary mCountryArrary[ _COUNTRY_COUNT_MAX_ ];

		// 代理服务器的IP与端口
		lk::CString32 mProxyServerAddr;

		// GateServer提供给客户端的访问的IP与端口
		lk::CString32 mGateServerAddr;
		lk::CString32 mGateServerLanAddr; // 局域网IP
		int mGateServerID;

		// GateWorld的名称
		lk::CString32 mWorldName;

		// Erating分配的GateCode, Passwd 与 Mac地址
		lk::CString32 mGateCode;
		lk::CString64 mGatePasswd;
		lk::CString32 mGateMac;

		// Erating 分配给这个世界的ID; 当向Erating bind成功的时候，erating会分配;
		// TODO: 如果erating未连接成功，怎么办?
		unsigned int mRegistID;


		// 与代理服务器连接的客户端
		CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> mProxyClient;

		// TODO: 指针 ?
		// 与游戏客户端消息队列通信的对象
		CClientHandle*  mClientHandle;	

		// 配置的世界ID; ERating 规定必须是 '1xx', 
		int  mWorldID; 

		// Erating是否启动了
		bool mERatingIsLaunched;

		// 上次与Proxy同步的时刻
		time_t mLastPorxyKeepAliveTime;

		// 重新加载单个配置功能相关
		typedef void(*TmLoadConfigCallback)(const char*);
		typedef lk::hash_map<lk::string<256>, TmLoadConfigCallback, 1000> LoadConfigFunclist;
		LoadConfigFunclist m_LoadConfigFuncList;

	public: 
		CGateServer();

		~CGateServer(){}

		static CSharedMem* mShmPtr;

		static unsigned int CountSize();

		void* operator new( size_t size );

		void  operator delete( void* pointer );

		int Initialize( int vInitFlag );
	
		int Resume();

	public:	//-- method
		// 接收其它服务器消息
		void RecvServerMsg(int vMsgCount);

		// 接收游戏客户端消息
		void RecvClientMsg(int vMsgCount);

		// 链接代理服务器
		bool Connect2ProxyServer();

		// 通知登录服务器世界开启了
		void Regist2LoginServer();

		// 通知场景服务器世界关闭
		void UnRegist2LoginServer();

		// 通知登录服务器世界还活在
		void SendHeartBeat2Login();

		// 向erating报告世界的信息
		void SendGWDataReportProto2ERating();

		// 向代理服务器发送认证消息
		void Regist2ProxyServer();

		// 通知代理服务器,网关服务器还在
		int SendkeepAliveToProxy();

		// 与proxy保持链接
		void KeepAlive2Proxy();

		// 向Proxy发送长度为vMsgLen的pMessage
		int Send2ProxyImp(BYTE* pMessage, int vMsgLen);

		// 向tpPlayer发送消息pMsg
		int Send2Player(CGatePlayer* tpPlayer, CMessage* pMsg);
        int Send2Player(std::vector<CGatePlayer*>& vPlayerList, CMessage* pMsg);

		// 发送消息给登录服务器
		void Send2Login( CMessage* pMessage );

		// 发送消息给日志服务器
		void Send2Log( CMessage* pMessage, int nSplitor);

		// 按照场景服务器ID发送消息
		void Send2Scene( int nServerID, CMessage* pMessage );		

		// 按照服务器对象发送消息
		void Send2Scene( CSceneServer* pHost, CMessage* pMessage );	

		// 按照玩家所在场景发送消息
		void Send2Scene( CGatePlayer* pPlayer, CMessage* pMessage );

		// 发送消息给数据服务器
		void Send2Data( int nAccountID, CMessage* pMessage, int nSrcGameID = 0, int nDesGameID = 0 );
		void SendMsg2Data( google::protobuf::Message* vpMsgFact, int vMsgID, int uID );

		// 给全局副本服务器发送消息
		void Send2GlobalDunserver( CMessage* pMessage );	

		// 给其他网关发消息
		void Send2OtherGate( CMessage* pMessage, int nWorldID );	

		// 注册场景服务器
		void RegistSceneServer(int nServerID, CSceneServer* tpServer);

		// 删除指定连接端口的主机
		void DeleteHost(CSceneServer* pHost);

		// 查找某国家的某地图所在的场景服务器
		CSceneServer* GetServerByLineMap( int vLineID, int vMapID );

		// 得到指定服务器运行的国家与地图列表
		int GetServerMap( const char* pServerName, unsigned int vServerID, std::vector< std::pair< int, std::string > >& rMapList );

		// 通过服务器名称得到服务器
		CSceneServer* GetServerByName( const char* pServerName );

		// 通过通讯端口得到服务器
		CSceneServer* GetServerByID( int vServerID );

		// 通过线ID得到地图信息
		CSceneMap* GetMapByLineMap( int vLineID, int vMapID );		

		// 取得地图最空闲的LINEID
		int  GetMostIdleLineID(int vMapID);

		// 停止
		void ExitServer();

		// 载入配置
		void OnReloadCfg();

		// 运行
		void OnLaunch();

		// 更新场景上报的玩家数量
		void UpdateSceneRptPlayerNumber(int nServerID, int nRptNumber);

		// 记录各场景的报告至数据库
		void SaveSceneReport2DB();

		// 向erating报告场景数据
		void ReportData2Erating();

		// 注册副本信息
		bool RegistRepetionInfo( CMessageRegistRepetionInfo* );

		// 注册场景与它装载的地图的对应关系
		bool RegistSceneMap(int nServerID, int nLineID, int nMapID);

		// 检查注册的nServerID的场景服务器
		int CheckSceneVersion(int nServerID, int nVersion, int nTplVersion);

		// 注册初始化启动的场景
		int RegistSceneByInitial(int nServerID, const char* szSceneName,  const char* szSceneIP,
									int nTplVersion, int nCodeVersion, int nTaskVersion, int nSceneCap );

		// 注册恢复启动的场景
		int RegistSceneByRecovery(int nServerID, const char* szSceneName,  const char* szSceneIP,
									int nTplVersion, int nCodeVersion, int nTaskVersion, int nSceneCap );

		// 检查场景服务器心跳超时
		void CheckGameServerTimeout();

		// 更新ProxyActiveTmStmp为当前时间
		void UpdProxyActiveTmStmp() { mLastPorxyKeepAliveTime = time(0); }

		// 取得Proxy的最后ActiveTimeStamp
		time_t GetProxyActiveTmStmp() { return mLastPorxyKeepAliveTime; }
		
		// 删除过期邮件
		void DeleteExpiredMail( );
		
		// 退回到期邮件
		void BackExpiredMail( int nMailTableID );	
		
		// 检查并执行系统邮件相关的操作
		void CheckMailSysActionTime();
		
		// 执行系统删除和退信操作
		void ExcuteMailSysAction();	



	public:
		// mGateCode
		void SetGateCode(const char* szCode) { mGateCode = (szCode ? szCode : ""); }
		const char* GetGateCode() const { return mGateCode.c_str(); }

		// mGatePasswd
		void SetGatePasswd(const char* szPasswd) { mGatePasswd = (szPasswd ? szPasswd : ""); }
		const char* GetGatePasswd() const { return mGatePasswd.c_str(); }

		// mGateMac
		void SetGateMac(const char* szMac) { mGateMac = (szMac ? szMac : "") ; }
		const char* GetGateMac() const { return mGateMac.c_str(); }

		// mGateServerAddr
		void SetGateServerIP(const char* szAddr) { mGateServerAddr = (szAddr ? szAddr : ""); }
		const char* GetGateServerIP() const { return mGateServerAddr.c_str(); }

		void SetGateServerLanIP( const char* szAddr ) { mGateServerLanAddr = (szAddr ? szAddr : ""); }
		const char* GetGateServerLanIP() const { return mGateServerLanAddr.c_str(); }

		void SetGateServerID( int nID ) { mGateServerID = nID; }
		int GetGateServerID( ) { return mGateServerID; }

		// mProxyServerAddr
		void SetProxyServerIP(const char* szAddr) { mProxyServerAddr = (szAddr ? szAddr : ""); }
		const char* GetProxyServerIP() const { return mProxyServerAddr.c_str(); }

		// mRegistID
		void SetRegistID(unsigned int nRegistID ) { mRegistID = nRegistID; }
		unsigned int GetRegistID() { return mRegistID; }

		// mERatingIsLaunched
		void SetERatingIsLaunched(bool bLaunched) { mERatingIsLaunched = bLaunched; }
		bool IsERatingLaunched() { return mERatingIsLaunched; }

		// mWorldID
		void SetWorldID(unsigned int nWorldID ) { mWorldID = nWorldID; }
		int GetWorldID() { return mWorldID; }

		// mWorldName
		void SetWorldName(const char* szName) { mWorldName = (szName ? szName : ""); }
		const char* GetWorldName() const { return mWorldName.c_str(); }

		// mSceneHosts[0]
		void SetMaxSceneServerID(int nMax) { mSceneHosts[0] = nMax; }
		int  GetMaxSceneServerID() { return mSceneHosts[0]; }

		// mSceneHosts[i] i > 0
		void SetServerObjID(int nServerID, int nObjID) { mSceneHosts[nServerID] = nObjID; }
		int  GetServerObjID(int nServerID) { return mSceneHosts[nServerID]; }

		// reset mSceneHosts[0]
		void ResetMaxSceneServerID() 
		{ 
			int i = GetMaxSceneServerID(); 
			for (;	i > 0; --i)
			{
				if ( GetServerObjID( i) != INVALID_OBJ_ID) break;
			}

			SetMaxSceneServerID( i ); // 最小是0
		}

		// calu valid mSceneHosts[i, GetMaxSceneServerID()]
		int GetSceneServerNumber()
		{
			int nRet = 0;
			for (int i = 1; i <= GetMaxSceneServerID(); ++i)
			{
				if ( GetServerObjID(i) !=  INVALID_OBJ_ID ) nRet++;
			}

			return nRet;
		}

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

		// 加载配置的回调函数
		static void LoadIPConfigCallback(const char* pcConfigPath);
		static void LoadGateserverConfigCallback(const char* pcConfigPath);
		static void LoadDirtyWordConfigCallback(const char* pcConfigPath);
		static void LoadRoleValidConfigCallback(const char* pcConfigPath);
		static void LoadTracelistCallback(const char* pcConfigPath);
		static void LoadTemplateCallback(const char* pcConfigPath);

		// 新抽出的配置加载
		int LoadIPConfig(const char* pcConfigPath, CConfigure& vConfigure);
		int LoadGateserverConfig(const char* pcConfigPath, CConfigure& vConfigure);
};

// 需要日志追踪的帐号的角色信息
class CTraceCharInfo
{
public:
	typedef lk::vector< int, PLAYERNUM > RoleList;
	RoleList mRoleList;
public:
	CTraceCharInfo()
	{
		if ( CObj::msCreateMode )
		{
			Initialize();
		}
		else
		{
			Resume();
		}
	}	
	~CTraceCharInfo()
	{
		
	}
public:
	int Initialize()
	{			
		mRoleList.initailize();
		return 0;
	}
	int Resume()
	{
		 return 0;
	}
};

// 需要日志追踪的帐号的角色信息管理器
class CTracePlayerManager
{
public:
	typedef lk::hash_map< int, CTraceCharInfo,  MAXPLAYERNUM > PlayerList;
	PlayerList mPlayerList;
public:
	CTracePlayerManager()
	{
		if ( CObj::msCreateMode )
		{
			Initialize();
		}
		else
		{
			Resume();
		}			
	}
public:
	int		InsertPlayerInfo( int nAccountID, int *pRoleIDList, int nRoleIDNum );
	int		DeletePlayerInfo( int nAccountID );
	bool    CheckIfRoleIDNeedTrace( int nAccoutID, int nRoleID );
	int		InitializeList(  );
public:
	int Initialize()
	{
		mPlayerList.initailize();
		return 0;
	}
	int Resume()
	{
		return 0;
	}
};


// ***************************************************************
//  Class:	CGateLogic
//  Responsibility:
//  		1. 负责响应消息,分发消息
//  		2. 负责Timer
//  		3. 负责启动，重载，停止应用
//  Restirct:
//  		1. 逻辑模块
//	Others:	 		
//  Date:	2009-07-08
//
// ***************************************************************
class CGateLogic : public CSingleton< CGateLogic >
{
	public:
		CGateLogic();

		~CGateLogic();

		static CSharedMem*  mShmPtr;

		void* operator new( size_t size );

		void  operator delete( void* pointer );

		// 启动
		int Initialize( int vInitFlag );

		// 恢复
		int Resume();

		// 启动
		void Launch();

		// 运行
		void Run(const int nDaemon );

		// 检查运行标志
		void CheckRunFlags();

		// 访问 mRunFlag 的接口
		void SetRunFlag( int vFlag );

		void ClearRunFlag( );

		bool IsRunFlagSet( int vFlag );

		// 调用定时器
		void CheckTimer();

		// 100304 MAOCY ADD-BEGIN
		// 加载角色验证信息
		void ReadRoleValidQuestionConfig(CConfigure& vConfigure);
		// 100304 MAOCY ADD-END

		// 读取配置到vConfig中
		int ReadCfg(CConfigure& vConfig);

		// 计算它的大小
		static unsigned int CountSize();
	public: //--method
		// 执行SQl
		bool ExecuteSql( int vSessionType, int vParam1, int vParam2, 
				int nAccountID, int vCallBack, SQLTYPE vSqlType, const char* pSQL, ... );

		// 重新载入配置文件的时候, Logic有些常量得重置
		void OnReloadConfig();

		// 触发session
		void FireServerSession(int nSessionClass, CMessage* tpMessage, int nParam1, int nParam2);
		// 触发eraing的session
		void FireAGIPSession(int nSessionClass, SysProtocol* pproto, 
				int nParam1, int nParam2, int nParam3, int nParam4, const char* szParam5);

		// 创建erating消息
		SysProtocol* CreateERatingMessage(unsigned char* message_buffer, short message_length);
		// 响应erating消息
		void OnERatingMessage(SysProtocol* proto);
		// 创建erating session
		CAGIPSession* CreateSession4Proto(SysProtocol* pproto);
		
		// 获取追踪玩家管理器
		CTracePlayerManager &GetTracePlayerManager(){ return mTracePlayerManager; }
		
	public: //--timer
		// 网关定时器
		void OnTimer(unsigned int vTickCount, unsigned int vTickOffset );

		// 检查erating session的超时
		void CheckAGIPSessionTimeout();

		// 处理erating session 超时
		void OnAGIPSessionTimeout(int nSessionClass, int nParam1, int nParam2);

		// 检查数据库回应超时
		void CheckDBSessionTimeout();

		// 处理db session 的超时
		void OnServerSessionTimeout(int nSessionClass, int nParam1, int nParam2);

		//获取 config 配置文件的目录
		const char* GetConfigDataFileNamePath(int nGameID, const char * filename);

	public:	//--message
		// 处理从代理服务器过来的消息统一入口
		void OnProxyMessage(CMessage* vMsg );

		// 处理从客户端过来的消息统一入口
		void OnClientMessage(CMessage* vMsg );

		// 处理ProxyMessage中的Server消息
		void OnServerControlMessage(int nServerID, CMessage* pMsg);

		// 响应ProxyMessage 中的中转消息
		void OnRouterMessage(int vCommHandle, CMessage* tpMessage);

		// 处理来自数据库的消息
		void OnMessageExecuteSqlResponse(CMessage* tpMessage);

		// 处理来自场景的心跳消息
		void OnMessageGameHeartBeatNotice(int nServerID, CMessage* tpMessage);

		// 处理消息创建地图回应
		void OnMessageCreateMapResponse(int nServerID, CMessage* pMessage);

		// 副本
		void OnMessageRegistRepetionInfo( int nServerID, CMessage* pMessage );

		// 处理删除副本地图资源
		void OnMessageDestroyRepetionMap( int vServerID, CMessage* pMessage );

		// 处理场景报告服务器信息
		void OnMessageServerInfoNotice(int nServerID, CMessage* pMessage);

		// 响应erating返回的DataReport消息
		void OnAGIPMsgGWDataReportResponse(SysProtocol* pSysProto, int nParam1, int nParam2);

		// 处理ERating状态通知
		void OnMessageERatingStateNotice(CMessage* tpMessage);

		// 响应erating proto 入口
		void OnMessageERatingProtoStream(CMessage* pMessge);
		
		// 给某个国家的所有成员发送消息
		void SendCountryMessage(CMessage *pMessage, int nCountryID);
		
		// 给场景服务器发送追踪玩家消息的通知
		void SendTracePlayerNotice();

		// 向ERating 发送创建申请
		void SendCreateGroupProto2ERating(int nRoleID, const char* szGroupName, int nGroupType, int nObjectID);

		// 给所有场景发送关闭通知
		void SendShutdownNotify();

		// 场景回应关闭消息
		int OnMessageShutdownAck( CMessage* pMessage );
		
		// 检查是否在ip列表里面
		bool CheckIfIpInInternalIPList( int nIPAddr );

		// 检查单IP下的连接数
		void CheckMultiClientPerIP();

		CConfigure mConfig;


		char mConfigtmpbuf[1024];


		enum emLauchFlag
		{
			EM_LAUNCH_LOADING		= 0, // 正在启动
			EM_LAUNCH_LOACOMPLETE,		 // 启动完成
			EM_LAUNCH_SHUTDOWNING,		 // 正在关闭场景
			EM_LAUNCH_SHUTDOWNCOMPLETE,	 // 关闭场景完成
			EM_LAUNCH_SELFSHUTDOWNING,	 // 正在关闭网关
			EM_LAUNCH_EXIT				 // 退出
		};

		// 刷新读网关配置出读取出的timer配置
		void RefreshTimerConfig();

	private:
		// 程序控制开关
		int mRunFlag;

		// 与代理服务器心跳的时钟
		CWTimer mTimerOfKeepAlive;

		// 与登录服务器保持心跳的时钟
		CWTimer mTimerOfLoginKeepAlive;

		// 网关上其它模块
		CModuleManager* tpModuleManager;// service module

		CWTimer mTimerOfReport2ERating;

		// 检查单IP客户端连接数的时钟
		CWTimer mTimerOfCheckMultiClientPerIP;

		//  
		CWTimer mWaitTimer;
		CWTimer mStateTimer;
		CWTimer mPerfTimer;
		CWTimer mOnLineNOTimer;
		CWTimer mOnLinePlayerStatPrintTimer;
		CWTimer mLaunchTimer; // 检查启动timer
		int		mLaunchFlag;	// 检查启动标志

		uint64_t mLastTickCount;	// the last timer active time.
		CTracePlayerManager mTracePlayerManager;
};

}

#endif
