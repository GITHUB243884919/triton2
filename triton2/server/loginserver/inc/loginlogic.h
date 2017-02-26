#ifndef _LOGIN_LOGIC_H_
#define _LOGIN_LOGIC_H_

#include "tcp_conn.h"
#include "entity.h"
#include "message_pb.hxx.pb.h"

#include "clienthandle.h"
#include "loginobj_define.h"
#include "message_queue.h"
#include "message_dispatcher.h"
#include "coremessage_pb.hxx.pb.h"
#include "db_msg_pb.hxx.pb.h"

#ifdef USING_ERATING
#include "Macro.h"
#include "SysProtocol.h"
#include "AGIP.h"
#include "SysAlgorithmMD5.h"
#include <list>

#define const_md5code_length 32 
#define const_md5list_limit 16

#define const_whitelist_limit 256

#define CONFIG_STR_LEN 1024

enum eBindStatus
{
	em_binded_ok = 1, 		// bind ok
	em_binded_failure = 2, 	// bind failure, rebind until ok.
	em_binded_invalid = 3, 	// bind invalid, rebind and using local auth.
};

#endif

// ********************************************************************** //
// CLoginServer
// ********************************************************************** //

class CLoginServerConfigure
{
public:
	int nLoginTimerInterval;
	int nPingProxyInterval;
	int nERatingHandsetInterval;
	int nEratingSessionTimeout;
	int nServerSessionTimeout;
	int nMaxLoginPlayerOnceTimer;
	int nMaxWorldPlayerNum; // 游戏世界总人数
	bool bVerifyByLocalForce; // 强制本地验证
	int nClientHandleIdleSleep;
	char szEChargeCode[32+1];
	char szEChargePasswd[64+1];
	int nEChargeID;
	int nRatingID;
	int nConnectEratingTimeout;

	int nIbPayRetryFirstInterval;
	int nIbPayRetryFirstTimes;
	int nIbPayRetrySecondInterval;
	int nIbPayRetrySecondTimes;

	int nIbUsedRetryFirstInterval;
	int nIbUsedRetryFirstTimes;
	int nIbUsedRetrySecondInterval;
	int nIbUsedRetrySecondTimes;
	int nValidateConnetSecOutTime;
	int nValidateConnetUSecOutTime;

	unsigned int  nClientVersion;
	bool bLoginServiced;
	bool bVerifyClientMd5Code;
	unsigned int nBlackIPList[64];

	//char sbClientMD5List[const_md5list_limit][const_md5code_length];

	typedef lk::hash_map< int, lk::string<const_md5code_length>, const_md5list_limit > MD5LIST;

	MD5LIST	mClientMD5List;

	char szWhiteNameList[const_whitelist_limit][CLoginPlayer::USER_NAME_LEN+1];
	
	CLoginServerConfigure();
};

class CLoginServer : public CSingleton< CLoginServer >
{
protected:
	CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> 	 mERatingClient;
	CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> 	 mProxyClient;
	CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> 	 mValidateClient;	

	CClientHandle*  mClientHandle;

	int mRunFlag;
	unsigned int mLastTickCount;

	lk::string<32> mERatingServerAddr;
	lk::string<32> mProxyServerAddr;

	lk::string<32> mGmCmdServerPort;
	lk::string<32> mChargeServerPort;
	int mLoginServerID;  // 登陆服务器的server id

	time_t mLastPorxyKeepAliveTime;

	CWTimer mTimerOfKeepAlive;

#ifdef USING_ERATING
	// 如果ERATING断开连接，向ERATING发起重新链接请求的时间间隔
	CWTimer mTimerOfReConn2ERating;
	// 从fd产生Eraing过来的消息，注意是new出来的
	SysProtocol* CreateERatingMessage(unsigned char* message_buffer, short message_length);

	// 连接Erating
	bool Connect2ERating(eLinkMode emBlock = em_block_mode);
#endif

public:
	CLoginServer() {}
	~CLoginServer(){}

	enum ERunFlag
	{
		EFLG_CTRL_NO        = 0,
		EFLG_CTRL_QUIT      = 1,
		EFLG_CTRL_RELOAD    = 2
	};
	
	enum EValidateStatus
	{
		EFLG_VALIDATE_NOCHECK = 0,	// 没有验证
		EFLG_VALIDATE_CHECKED = 1,	// 通过验证
	};

	static CSharedMem* mShmPtr;

	// 计算LoginServer 占用的共享内存的大小, 其实它不需要占用任何共享内存, loginlogic需要占用, 
	// 因为main程序只能看到LoginServer, 因此将它的计算放在了这里
	static unsigned int CountSize(); 

	// 初始化
	int Initialize( int vInitFlag );

	// 程序启停的标志
	void CheckRunFlags();
	void SetRunFlag( int vFlag );
	void ClearRunFlag();
	bool IsRunFlagSet( int vFlag );

	// 接受从服务器过来的消息， 包括ERating, 各个GateServer, 以及侦听GateServer的链接 
	void RecvServerMsg(int vMsgCount);

	// 从共享内存读取客户端过来的消息
	void RecvClientMsg(int vMsgCount);
	
	// 接受验证服务器的消息
	int RecvValidateMsg( int nSec, int nUsec );

	// 启动服务器
	void Launch();

	// 运行服务器 
	void Run(const int nDaemon );
	
	// 检查时钟
	void CheckTimer();
	
	// 向PROXY服务器发送注册消息
	void Regist2ProxyServer(); 	
	// 向PROXY服务器发送同步消息
	void SendKeepAliveToProxy();
	
	// 发起到PROXY服务器的链接
	bool Connect2Proxy();
	
	// 发起到验证服务器的链接
	bool Connect2Validate();
	
	// 向验证服务器进行注册
	bool Regist2ValidateServer( );

	struct CLoginServerConfigure mConfig;

	// 从配置文件loginserver.xml 读取配置到vConfig
	int ReadCfg(CLoginServerConfigure& vConfig);
	// 得到gameid后加载 login配置
	int ReadCfgByGameId(CLoginServerConfigure &config);
	
public:

	// 向玩家发送消息接口
	void Send2PlayerImp( CLoginPlayer* tpPlayer, CMessage* pMessage );

	// 向ERating发送消息接口
	bool Send2ERating( BYTE* pMessage, int vMsgLen );

	// 向Gate发送消息接口
	int Send2ProxyImp( BYTE* pMessage, int vMsgLen );
	
	// 向验证服务器发送消息接口
	void Send2Validate( CMessage *tpMsg ); 	

	// 是否是黑名单IP地址
	bool IsBlackIP(unsigned int nAddr);

#ifdef USING_ERATING
	// ERATING 是否链接
	bool IsConnected2ERating();

	// Close ERating
	void CloseERating() { mERatingClient.GetSocket()->Close(); }

	// 最近发给erating包的时间
	time_t mTmSend2Erating;	

	// 最近接收到erating包的时间
	time_t mTmRecv4Erating;
#endif
};

// ********************************************************************** //
// CLoginServer
// 登录服务器允许不同的世界有不同的版本
// ********************************************************************** //

class CLoginLogic;

class CLoginLogic : public CSingleton< CLoginLogic >
{
public:
	// 玩家容器
	typedef lk::hash_map<int, int,  PLAYER_AT_LOGIN_CAP>	PLAYERCONTAINER;
	typedef std::list< int > QUEUEPLAYERCONTAINER;

public:
	// 对象初始化地址
	static CSharedMem*  mShmPtr;

	// 构造函数
	CLoginLogic();

	// 析构函数
	~CLoginLogic();

	// 重载了new 与 delete 
	void* operator new( size_t size );
	void  operator delete( void* pointer );

	// 计算占用内存大小
	static unsigned int CountSize();

	// 初始化
	void Initialize();

	// 恢复运行
	void Resume();

	// 登陆服务器时钟
	void OnTimer( unsigned int vTickCount );

public:
	// 相应验证服务器的消息
	void OnValidateMessage( CMessage *pMessage );
	
	// 响应从各世界的网关过来的消息
	void OnProxyMessage(CMessage* pMessage);

	// 响应客户端过来的消息
	void OnClientMessage(CMessage* pMessage);

	// 响应重新读入loginserver.xml
	void OnReloadConfig();

	// 响应退出消息
	void OnExit();

	// 向Gate发送消息
	void Send2Gate(CMessage* pMessage);

	// 向目前所有的场景发消息
	void Send2AllScene(CMessage* pMessage);

	// 更新erating的绑定状态
	void UpdateERatingBindStatus(bool is_binded);

	// 游戏世界是否开启
	bool IsWorldOpened() { return mServiceTurnedOn; }

	// 按照comm注册玩家, 
	void RegistPlayerComm(int nCommHandle, CLoginPlayer* tpPlayer);

	// 按照通讯端口查找玩家
	CLoginPlayer* GetPlayerByComm( int nCommHandle );

	// 玩家离开登陆服务器, 根据玩家状态销毁对象 
	void PlayerLeaveFromLogin( CLoginPlayer* tpPlayer);

	// 发起SQL 申请
	CServerSession* ExecuteSql( int vSessionType, int vParam1, int vParam2,
							int nFixID, int vCallBack, SQLTYPE vSqlType, const char * pSQL, ... );

	// TODO: 取得Charge需要的RatingID
	uint32_t GetRatingID( );

	uint32_t GetWorldID( ) { return mSingleWorld.mWorldID; }

	int GetGameID() { return mSingleWorld.mGameID; }

	#ifdef USING_ERATING
	// 响应从ERating过来的消息
	void OnERatingMessage(SysProtocol* pProto);

	// 向ERATING注册
	void SendBindProto2ERating(em_connect_erating_flag emConnect);

	// 创建AGIPSession对象
	CAGIPSession* CreateSession4Proto(SysProtocol* pproto);
	#endif

	// ERating是否绑定成功
	bool IsERatingBinded() { return mERatingIsBinded; }

	//获取 config 配置文件的目录
	void GetConfigDataFileNamePath(int nGameID, char *filename, char *input, int len);


private:

	// 玩家登录完成 
	void OnMessageLoginCompleteNotice(CMessage* pMessage);	

	// 响应网关服务器过来的版本注册信息
	void OnMessageUpdateVersionNotice(CMessage* pMessage);

	// 网关启动停止消息响应 
	void OnMessageGateInfoNotice(CMessage* pMessage);
	void OnMessageGateShutDownNotice(CMessage* tpMessage);

	// 网关心跳消息
	void OnMessageGateHeartBeatNotice( CMessage* tpMessage );

	// 响应玩家离开世界的通知
	void OnMessagePlayerLeaveGateNotice(CMessage* tpMessage);

	// 响应玩家进入世界的回应, 玩家进入登录游戏过程 
	void OnMessagePlayerEnterGateResponse(CMessage* tpMessage);

	// 响应玩家登录到游戏世界, 玩家进入游戏场景中
	void OnMessagePlayerLoginCompleteNotice(CMessage* tpMessage);

	// 响应数据库执行结果消息
	void OnMessageExecuteSqlResponse(CMessage* tpMessage);

	// 验证玩家帐号
	void OnSessionVerifyAcctByMySql(CMessage* tpMessage, int vParamater1, int vParamater2 );

	// 玩家登陆服务器
	void OnMessageLoginServerRequest( int vCommHandle, CMessage* pMessage);

	// 当收到退出排队消息
	void OnMessagePlayerCancelLoginQueue( int vCommHandle, CMessage* tpMessage);

	// 注销comm, 
	void UnRegistPlayerComm(int nCommHandle);

	// 玩家登录
	void PlayerLogin(CLoginPlayer* tpPlayer);


	// 向客户端发送消息
	void Send2Client(CLoginPlayer* tpPlayer, CMessage* tpMessge);

	// 向数据库服务器发送消息
	void Send2Data(int nFixID, CMessage* pMessage );

	// 向日志服务器发送消息
	void Send2Log(int nFixID, CMessage* pMessage );

	// 将玩家推向某个世界的网关
	void Push2Gate( CLoginPlayer* pPlayer);

	void SendLoginErrorNotice(CLoginPlayer* tpPlayer, int nErrCode);

	void FireServerSession(int nSessionClass, CMessage* tpMessage, int nParam1, int nParam2);

	void OnServerSessionTimeout(int nSessionClass, int nParam1, int nParam2);

	// 本地验证帐号
	int VerifyAcctByLocal( CLoginPlayer* tpPlayer);

	// 根据帐号查找玩家
	CLoginPlayer* GetPlayerByAcct( int vAccountID );

	void LoginToLower( char * pszSrc, size_t len);

	// 发送当前的排队人数
	void SendQueuePostion(CLoginPlayer* tpPlayer, size_t nPos);
	
	// 验证回应消息
	void OnMessageValidateServerResponse( CMessage* pMessage );	

	// 得到世界在线玩家数
	int GetWorldOnlinePlayerNum( ) { return mCurrentPlayerNumber; }
	void SetWorldOnlinePlayerNum( int nNum ) { mCurrentPlayerNumber = nNum; }

	#ifdef USING_ERATING
	// 注册到Erating
	void OnAGIPMsgBindResponse(SysProtocol* pSysProto, int nParam1, int nParam2);

	// ERating 消息流程验证帐号
	void SendUserLoginProto2ERating( CLoginPlayer* tpPlayer);
	void OnAGIPMsgUserLoginResponse(SysProtocol* pProto, int nParam1, int nParam2);

	// Unbind 流程
	void SendUnbindProto2ERating();
	void OnAGIPMsgUnbindResponse(SysProtocol* pSysProto, int nParam1, int nParam2);

	// Handset 流程
	void SendHandsetProto2ERating();
	void OnAGIPMsgHandsetResponse(SysProtocol* pSysProto, int nParam1, int nParam2);

	// 根据ID取得CAGIPSession
	CAGIPSession* GetAGIPSession(int nSessionID);

	// 响应从网关过来的ERATING消息,protobuf形式
	void OnMessageERatingProtoStream(CMessage* tpMessage);

	// 响应游戏发起的ERating回应的消息
	void FireAGIPSession(int nSessionClass, SysProtocol* pproto, int nParam1, int nParam2);

	// 游戏发起的session超时处理
	void OnAGIPSessionTimeout(int nSessionClass, int nParam1, int nParam2);
	#endif

	bool IsJoinAuth(CLoginPlayer* tpPlayer);

private:
	// 游戏世界总人数(来自网关定时通知)
	int	 mCurrentPlayerNumber;		

	// 服务器同时允许登陆人数
	int	 mOnceLoginNumber;		

	// 上周期离开服务器的人数
	int  mOnceLeaveNumber;

	CWTimer mWaitTimer;

	#ifdef USING_ERATING
	// 商城交易重发时间间隔
	CWTimer mTimerOfIbPay;

	// ERATING 心跳回应包超时时间间隔, 同时也是重连的时间间隔
	CWTimer mERatingHandsetTimer;
	#endif

	// 是否使用ERating控制
	bool mERatingIsBinded;

	// TODO: GATE是否开启
	bool mServiceTurnedOn;

	// 唯一世界
	CWorldServer mSingleWorld;

	// 登录服务器的玩家, 按照帐号索引
	PLAYERCONTAINER mPlayerContainer;	

	// 连接到LoginServer的玩家, fd 索引
	PLAYERCONTAINER mPlayerConnected;	

	// 登录速率排队控制队列, 它里面的数据可以是无效的
	QUEUEPLAYERCONTAINER mLoginQueue;
};

#endif
