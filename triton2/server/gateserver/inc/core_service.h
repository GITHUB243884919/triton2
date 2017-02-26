#ifndef _GATECORE_SERVICE_H_
#define _GATECORE_SERVICE_H_

#include "module_manager.h"	  

// ***************************************************************
//  Class:	CPlayerManager
//  Responsibility:
//  		1. 组织CGatePlayer数据, 
//  		2. 提供创建帐号,增加删除角色, 踢人, 进入网关, 进入游戏等逻辑
//  Restirct:
//  		1. 不响应具体的消息
//	Others:	 		
//  Date:	2009-07-08
//
// ***************************************************************
enum EAfterSendGateError
{
	em_keep_connect,
	em_need_disconnect
};

class CBlackList
{
	public:
		typedef lk::hash_map< int, lk::CString128,  MAXPLAYERNUM > BLACKLIST;

		bool Load();
		bool Append(int nServer, int nRoleID, const char* szName);
		bool Find(int nRoleID);
		const char* GetStr(int nRoleID);
		bool Remove(int nRoleID);
		int	 Size();

		CBlackList()
		{
			if ( CObj::msCreateMode )
			{
				Initialize();
				Load();
			}else
			{
				Resume();
			}
		}

		~CBlackList()
		{

		}

		int Initialize()
		{			
			mList.initailize();
			return 0;
		}

		int Resume()
		{
			return 0;
		}

	private:
		BLACKLIST mList;
};
struct CGlobalData
{
public:
	enum Status
	{
		EGLOBAL_STATUS_PREPARETELE	 = 0,	// 准备跨服
		EGLOBAL_STATUS_INGLOBALSCENE = 1,	// 在全局场景中
	};
public:
	int mRoleID;				// 跨服的角色ID
	int mGlobalWorldID;			// 目标场景的世界ID
	int mStatus;				// 状态
	int mTeleTime;				// 开始传送的时间
public:
	CGlobalData()
	{
		mRoleID = 0;
		mGlobalWorldID = 0;
		mStatus = EGLOBAL_STATUS_PREPARETELE;
		mTeleTime = time(NULL);
	}
};
class CPlayerManager : public CSingleton< CPlayerManager >
{
	private:
		typedef std::pair< int, int > PLAYERTYPE;
		typedef lk::hash_map< int, int , PLAYER_AT_GATE_CAP> PLAYERCONTAINER;
		typedef lk::hash_map< int, int , PLAYER_AT_GATE_CAP>::iterator	PLAYERITER;
		typedef lk::hash_map< lk::string<NAME_LENGTH>, int, PLAYER_AT_GATE_CAP > PLAYERNAME_MAP;
		typedef lk::hash_map< int, CGlobalData, PLAYER_AT_GATE_CAP > GLOBALSCENEPLAYERMAP;
		//typedef lk::vector<int ,6> MACACCOUNTVECTOR;
		//typedef lk::hash_map< lk::string<16>, MACACCOUNTVECTOR, 200 > MACPLAYERNUM;
		//typedef lk::hash_map< int ,MACPLAYERNUM,PLAYER_AT_GATE_CAP > IPMAC_MAP;

		// 在场景中的玩家, 用角色ID作为索引
		PLAYERCONTAINER	mPlayerInSceneByChar;

		// 在场景中的玩家, 用角色Name作为索引
		PLAYERNAME_MAP	mPlayerInSceneByName;

		// 所有玩家索引 account 索引
		PLAYERCONTAINER	mGatePlayerByAcct;

		// 目前和Gate保持连接的玩家索引  端口号索引
		PLAYERCONTAINER	mPlayersConnected;

		// 从Login push到网关,等待登录的玩家 account 索引
		PLAYERCONTAINER	mPlayerInWait;		

		GLOBALSCENEPLAYERMAP mGlobalScenePlayerMap;

		
	public:
		// 不让登录的角色名单
		CBlackList mBlackList;

		// 注册角色ID与GatePlayer的索引关系
		void RegistRoleIDIndex( CGatePlayer* tpPlayer);

		// 注册账号ID越GatePlayer的关系
		bool RegistAccountIndex( CGatePlayer* tpPlayer );

		// 解除角色ID与GatePlayer的索引关系
		void RemoveRoleIDIndex( CGatePlayer* tpPlayer);

		// 解除帐号ID与GatePlayer的索引关系
		void RemoveAccountIDIndex(CGatePlayer* tpPlayer);

		// 注册角色Name与GatePlayer的索引关系
		void RegistRoleNameIndex(CGatePlayer* tpPlayer);

		// 解除角色Name与GatePlayer的索引关系
		void RemoveRoleNameIndex(CGatePlayer* tpPlayer);

		// 注册玩家端口的关系
		void UnRegistPlayerComm(CGatePlayer* tpPlayer);

		// 解除玩家与端口的关系
		void RegistPlayerComm(CGatePlayer* tpPlayer);

		// 检查场景活动玩家超时
		void CheckPlayerActiveTimeout();

		// 更新玩家每个mac地址的人数
	//	void UpdatePlayerNumPerMac(CGatePlayer* tpPlayer);
	public:	//-- 方法

		// 清理玩家数据，但不销毁对象
		void ClearScenePlayer( CGatePlayer* tpPlayer);

		// 销毁网关玩家对象
		void DestroyGatePlayer( CGatePlayer* tpPlayer, int nLeaveReason = 0xFF);

		// 当跨场景错误时，发送跨场景错误代码给vCharID
		int SendRedirErrCode( int vCharID, int vErrorCode, int vLineID, int vMapID );

		// 发送网关错误消息
		void SendGateErrorNotice(CGatePlayer* tpPlayer, int nErrCode, 
				EAfterSendGateError after, const char* pszErrString = NULL);

		// 仅仅断开服务器与客户端的连接，客户端不用响应 
		void OnlyDisconnectClient(CGatePlayer* tpPlayer);

		// 玩家准备进入Gate
		void EnterGate(int nCommHandle, int nAccountID, int nLoginToken, int nAdultFlag, int nUserStatus, int nGameID, unsigned int nCpuID, unsigned int nDiskID);

		// 删除pHost上的所有玩家
		void RemovePlayerInScene(CSceneServer* pHost);

		// 踢掉网关玩家
		void KickOffGatePlayer(CGatePlayer*, unsigned int nAnotherIP);
		// 踢场景玩家
		void KickOffScenePlayer(CGatePlayer*, unsigned int nAnotherIP);


		// 玩家进入游戏
		void PlayerEnterGameWrapper(int nAccountID, int nRoleID, const char* szMac);
		// 向erating 提出进入游戏申请
		void SendEnterGameProto2ERating(CGatePlayer* tpPlayer, int nServerID);
		// 向GameServer提出进入游戏申请
		void PlayerEnterGame(CGatePlayer* tpPlayer );


		// 获取角色
		void ObtainCharsWrapper(int vCommHandle);
		// erating 处理获得角色列表消息
		void SendObtainCharsProto2ERating(CGatePlayer* tpPlayer);
		// 向数据库发送取得角色列表消息
		void SendObtainChars2DB(CGatePlayer* tpPlayer);


		// 创建角色
		void CreateCharWrapper(int vCommHandle, const char* szCharName, int nSexID, int nFaceType);
		// 向数据库发送创建角色请求
		void SendCreateChar2DB(CGatePlayer* tpPlayer, int nRoleID);
		// erating 创建角色
		void SendCreateCharProto2ERating(CGatePlayer* tpPlayer);


		// 删除角色
		void DeleteCharWrapper(int vCommHandle, int nCharID, const char* szPasswd, int nPasswdType);
		// 向数据库发送删除玩家请求
		void SendDeleteChar2DB(CGatePlayer* tpPlayer, int nCharID );
		// 向数据库发送删除角色关联数据请求
		void SendDelRoleRelationData2DB( int nAccountID, int nRoleID );
		// 向erating 发送删除玩家时的密码认证
		//void SendPasswdVerifyProto2ERating(CGatePlayer* tpPlayer, int nCharID,  const char* szPasswd, uint32_t nPasswdType);
		// erating删除角色
		void SendDeleteRoleProto2ERating(CGatePlayer* pPlayer, int nRoleID);

		// 发送删除角色的回应消息
		void SendResponseDelteRole( CGatePlayer *tpPlayer, int nErrcode, int nRoleID, int nRoleDeleteStatus, int nDelteStatusLeftTime );
		
		// 更新玩家的信息
		void UpDatePlayerInfo( CGatePlayer *tpPlayer, int nRoleID, int nParam1, int nParam2 );

		// 向ERATING 发送登出消息
		// nFlag = 3, 异常退出  nFlag = 1, 正常退出   nFlag = 2, 防沉迷通知
		void SendUserLogoutProto2ERating(CGatePlayer* tpPlayer, CMessagePlayerLeaveMapNotice* tpPlayerLeave, int nFlag = 1);
		
		// 更新玩家的帐号信息
		void UpDatePlayerAccountInfo( CGatePlayer *tpPlayer, int nParam1, int nParam2 );


		// 玩家排队
		void QueuePlayer(CGatePlayer* tpPlayer, CSceneServer* tpHost);
		// 从排队队列中移除玩家
		void RemovePlayerFromQueue(CGatePlayer* tpPlayer);
		
		// 给某个国家的玩家发送消息
		void SendCountryMessage( CMessage *pMessage, int nCountryID );

		// 从gate上直接给某个国家的玩家发送消息
		void SendCountryMessageFromGate( CMessage *pMessage, int nCountryID );
		
		// 从gate上直接给世界的玩家发送消息
		void SendWorldMessageFromGate( CMessage *pMessage, int vMapID = 0 );
		
		// 重置玩家财产密码
		int ResetRolePassword( int nRoleID );
		
		// 更新玩家的密码
		void UpDateRolePassword( int nAccountID, int nRoleID, const char *pPassword, int nParam1, int nParam2 );

		// 插入一个玩家到全局场景列表中
		void InsertPlayerToGlobalSceneMap( int nAccountID, CGlobalData &rGlobalData );

		void DeletePlayerFromGlobalSceneMap( int nAccountID );

		// 踢出在其他服务器的玩家
		void KickPlayerInGlobalScene( int nAccountID = 0 );

		// 判断玩家是否在其他阵营
		bool IsPlayerInGlobalScene( int nAccountID );

		// 发送踢出全局场景玩家的结果通知消息
		void SendKickGlbalPlayerACK( int nAcccountID, int nRoleID, int nRoleWorlID );

		// 踢出玩家超时
		void OnTimeOutKickGlobalPlayer( int nAccountID, int nRoleID );

		// 获取全局信息
		CGlobalData * GetGlobalData( int nAccountID );

	public: //-- 接口

		unsigned short GetPlayerCountryLineID( CGatePlayer* vpPlayer );

		// 得到网关中的玩家,通过通讯端口ID
		CGatePlayer* GatePlayer( int vCommHandle );

		// 根据帐号取得玩家
		CGatePlayer*  GetPlayerByAcct(int nAccountID);

		// 通过角色ID, 得到场景中的玩家
		CGatePlayer* ScenePlayer( int vCharID );

		// 通过角色名称, 得到场景中的玩家
		CGatePlayer* ScenePlayer( const char* pName );

		// 得到所有在gate上的账号个数
		unsigned int GetWorldPlayerNumber( ) { return mGatePlayerByAcct.size(); }

	public:
		static CSharedMem* mShmPtr;

		void * operator new( size_t tsize );
		void   operator delete( void *pPointer );

		static unsigned int CountSize();

		CPlayerManager();
		~CPlayerManager();

		int Initialize();
		int Resume();
};

// ***************************************************************
//  Class:	CCoreService
//  Responsibility:
//  		1. 负责响应消息
//  		2. 负责触发OnTimer事件
//  Restirct:
//  		1. 对CPlayerManager负责
//	Others:	 		
//  Date:	2009-07-08
//
// ***************************************************************

class CCoreService: public GateServer::CDataModule, public CSingleton<CCoreService>
{
	public:	 //-- 外界消息	
		
		virtual bool IsLaunched() { return true; }

		// 玩家请求角色列表
		void OnMessagePlayerObtainCharsRequest(int vCommHandle, CMessage* tpMessage);
		// 响应Erating返回的角色列表消息
		void OnAGIPMsgRoleListResponse(SysProtocol* pSysProto, int nParam1, int nParam2);
		// 处理从数据库返回的角色列表消息
		void OnSessionObtainChars( CMessage* tpMessage, int vParamater1, int vParamater2 );
		// 处理从erating返回角色列表超时		
		void OnAGIPMsgRoleListTimeOut(int nParam1, int nParam2);
		// 处理从dbserver返回角色列表超时		
		void OnTimeOUtObtainChars(int nParam1, int nParam2);
		
		// 跨服时获取数据
		void SendGetGlobalRoleData2DB( CGatePlayer *pPlayer );
		// 跨服时的数据处理
		void OnSessionGetGlobalRoleData( CMessage* tpMessage, int vParamater1, int vParamater2 );
		// 跨服时的数据获取超时
		void OnTimeOutGetGlobalRoleData(int nParam1, int nParam2);

		// 玩家从场景离开
		void OnMessagePlayerLeaveMapNotice( int vCommHandle, CMessage* tpMessage);
		// 响应erating返回的离开游戏消息
		void OnAGIPMsgUserLogoutResponse(SysProtocol* pSysProto, int nParam1, int nParam2);	
		// 处理erating处理玩家离开超时
		void OnAGIPMsgUserLogoutTimeOut(int nParam1, int nParam2);


		// 玩家请求删除角色
		void OnMessagePlayerDeleteCharRequest( int vCommHandle, CMessage* tpMessage);
		// 删除角色时检查2级密码
		void OnMessageCheckRolePwd( int vCommHandle, CMessage *pMessage  );
		// 响应erating返回的删除角色消息
		void OnAGIPMsgDeleteRoleResponse(SysProtocol* pSysProto, int nParam1, int nParam2);
		// 响应erating返回的安全码验证消息
		//void OnAGIPMsgPasswdVerifyResponse(SysProtocol* pSysProto, int nParam1, int nParam2);
		// 处理从数据库返回的删除角色消息
		void OnSessionDeleteChar( CMessage* tpMessage, int vParamater1, int vParamater2 );
		// 处理Erating删除角色超时
		void OnAGIPMsgDeleteRoleTimeOut(int nParam1, int nParam2);
		// 处理数据库删除角色超时
		void OnTimeOutDeleteChar(int nParam1, int nParam2);
		// 处理删除角色的时候密码验证超时
		//void OnAGIPMsgPasswdVerifyTimeOut(int nParam1, int nParam2);
		
		// 取消删除角色的消息请求
		void OnMessageCancelDeleteRole( int vCommHandle, CMessage *pMessage  );



		// 玩家请求建立角色
		void OnMessagePlayerCreateCharRequest( int vCommHandle, CMessage* tpMessage);
		// 处理从数据库返回的创建角色消息
		void OnSessionCreateChar( CMessage* tpMessage, int vParamater1, int vParamater2 );
		// 响应Erating返回的创建角色消息
		void OnAGIPMsgCreateRoleResponse(SysProtocol* pSysProto, int nParam1, int nParam2);
		// 响应Erating创建角色超时
		void OnAGIPMsgCreateRoleTimeOut(int nParam1, int nParam2);
		// 响应DB创建角色超时
		void OnTimeOUtCreateChar(int nParam1, int nParam2 );



		// 处理登录服务器的玩家将要进入网关消息
		void OnMessagePlayerEnterGateNotice	( CMessage* pMessage );
		// 处理客户端登陆网关请求
		void OnMessagePlayerLoginGateRequest( int vCommHandle, CMessage* pMessage );
		// 处理场景踢人回应消息
		void OnMessageKickOffPlayerResponse(int nServerID, CMessage* pMessage);

		void OnMessageEntityGMMove( int nServerID, CMessage* pMessage );
		void OnMessageRoleMove( int nServerID, CMessage* pMsg );

		// 处理玩家发送的进入游戏场景请求
		void OnMessagePlayerEnterGameRequest( int vCommHandle, CMessage* pMessage );
		// 响应erating返回的进入游戏请求消息
		void OnAGIPMsgEnterGameResponse(SysProtocol* pSysProto, int nParam1, int nParam2);
		// 处理场景通知网关已经准备好接收玩家进入消息
		void OnMessageScenePlayerEnterMapResponse( int nServerID, CMessage* pMessage );
		// 处理玩家进入场景确认
		void OnMessagePlayerEnterConfirm(int nServerID, CMessage* pMessage);
		// 处理erating对EnterGame的响应超时
		void OnAGIPMsgEnterGameTimeOut(int nParam1, int nParam2);


		// 处理玩家切换场景服务器消息
		void OnMessagePlayerRedirectScene( int nServerID, CMessage* pMessage );
		// 处理场景通知网关对跨场景方式进入地图的响应消息
		void OnMessageRedirectEnterMapResponse( int vCommHandle, CMessage* pMessage );			
		// 处理玩家跨场景失败消息
		void OnMessageRedirectSErrorEvent( int vCommHandle, CMessage* pMessage );



		// 处理玩家更改阵营的消息
		void OnMessageRoleChangeCampNotice(int nServerID, CMessage* pMessage);
		// 防沉迷请求
		// void OnMessageFangChenMiRequest(int nServerID, CMessage* pMessage);
		// 处理玩家取消排队消息
		void OnMessagePlayerCancelLoginQueue( int vCommHandle, CMessage* pMessage );
		// 玩家返回网关通知
		void OnMessagePlayerReturn2GateNotice( int vServerID, CMessage* pMessage );
		// 处理玩家个人信息改变(等级，职业，地图.....)
		void OnMessagePersonalLevelChange( CMessage* pMessage );



		// 处理玩家断开网关的事件
		void OnEventPlayerDisconnectGate(int nCommandID);

		// 处理超时
		void OnMessagePlayerPingRequest(int vCommHandle, CMessage* tpMessage);
		
		// 处理玩家安全密码状态改变的消息
		void OnMessageChangePassWordStatus(int vCommHandle, CMessage* pMessage);

		// 修改网关玩家的会员标志
		void OnMessageUpdateVipFlag( int nServerID, CMessage* npMessage );

		// 重新存盘成功
		void OnMessageResavePlayerSuccess(int nServerID, CMessage* pMessage);

// 100304 MAOCY ADD-BEGIN
      // 角色验证时，建立问题答案
      void BuildQuestionOptionData(SRoleValidQuestionOption* pInputOption, SRoleValidQuestionOption* pOutputOption);
      // 角色验证时，请求问题
      void BuildQuestionRequireResponse(CGatePlayer* tpPlayer, CMessageQuestionRequireResponse& response, SRoleValidQuestionOption* pOptions);
		// 角色验证时，请求问题
		void OnMessageQuestionRequireRequest(int vCommHandle, CMessage* pMessage);
		// 角色验证时，答案验证
		void OnMessageQuestionAnswerRequest(int vCommHandle, CMessage* pMessage);
// 100304 MAOCY ADD-END

// 100331 [GS] MAOCY ADD-BEGIN
      // 登录服务器查询角色信息请求
      void OnMessageRoleQuery(int nServerID, CMessage* pMessage);
// 100331 [GS] MAOCY ADD-END

	  ///////////////////////////////////
	  // 消息：转阵营（转服）消息
	  // author： qiufufu
	  // date : 10-08-23
	  //////////////////////////////////////
	 //申请强弱列表
	  void OnMessageCampWeakRequest(CMessage *pMessage);

	 //scenserver验证转阵营条件成功后 发送给gate看 gateserver是否验证成功
	  void OnMessageChangeCampRequest(CMessage *pMessage);
	 //scensever 删除道具成功 ，通知gate 修改排行榜信息，设置阵营id。
	  void OnMessageChangeCampNotice(CMessage *pMessage);
	 // 发送给客户端 转阵营失败消息
	  void SendChangeCampErrorResponse(CGatePlayer* tpPlayer, ChangeCamp errorCode);
		
	 // 玩家连接网关后直接进入场景
	 void PlayerDirectEnterGame( int nCommHandle, CGatePlayer *pPlayer );
	public: //-- method

		// 检查玩家各种状态超时
		void CheckPlayerStatusTimeout();


		// 检查排队队列
		void DealQueuePlayer(unsigned int nNowTick);

		void OnEventHostDown(CSceneServer* pHost);

		// 响应服务器过来的消息
		void OnServerMessage(int nServerID, CMessage* tpMessage);

		// 响应客户端消息
		void OnClientMessage(CMessage* tpMessage);

		void OnLaunchServer();
		void OnExitServer();
		void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
		void OnMessage(int nServerID, CMessage* pMessage );
		void OnReloadConfig();

		CCoreService();
		~CCoreService();

		int Initialize();
		int Resume();
		static unsigned int CountSize();
		void* operator new( size_t nsize );
		void  operator delete( void* pointer );

		// 加载黑名单配置的回调函数
		static void LoadBlackListConfigCallback(const char* pcConfigPath);
		
	private:
		// 等待连接玩家的检查计时器
		CWTimer	mWaitTimer;			

		// 检查玩家状态的Timer
		CWTimer	mStateTimer;		

		//在线玩家状态打印计时器
		CWTimer	mOnLinePlayerStatPrintTimer; 

		// 在线数量写入数据库
		CWTimer	mOnLineNOTimer;		

};

#endif
