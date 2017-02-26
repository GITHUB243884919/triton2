#ifndef _LOGIN_LOGIC_H_
#define _LOGIN_LOGIC_H_

#include "tcp_conn.h"
#include "entity.h"
#include "globaldata.h"
#include "clienthandle.h"
#include "message_pb.hxx.pb.h"
#include "coremessage_pb.hxx.pb.h"
#include "message_queue.h"
#include "message_dispatcher.h"

//============================================================
#define GS_DATABSE_MAXLENGTH 40

//============================================================
enum EGlobalServerSocketType{
   EGlobalServerSocketType_Client,
   EGlobalServerSocketType_Login
};

//============================================================
struct SSocketInfo{
   EGlobalServerSocketType type;
   int index;
};

//============================================================
// <T>登录服务器信息</T>
//============================================================
class CGlobalLoginModule : public CSingleton<CGlobalLoginModule>{
public:
	static CSharedMem*  mShmPtr;
public:
   int loginCount;
   CGlobalLogin logins[GS_LOGIN_MAXCOUNT];
public:
   static unsigned int CountSize();
   void* operator new(size_t size);
   void  operator delete(void* pointer);
   void* operator new(size_t size, const char* file, int line);
   void operator delete(void* pointer, const char* file, int line);
public:
   CGlobalLoginModule();
	void Initialize();
	void Resume();
public:
	CGlobalLogin& Alloc(); 
	CGlobalLogin& Get(int index);
	CGlobalLogin* FindById(uint16 logseverId);
	CGlobalLogin* FindByHost(uint32 ipAddr);
	CGlobalLogin* FindByHandle(uint16 handle);


};

//============================================================
// <T>Token生成。</T>
//============================================================
#define TOKEN_UPPER_BOUND 100000
class RTokenGenarator{
public:
	static uint32 GenToken(){
		if(_base > TOKEN_UPPER_BOUND){
			_base = 0;
		}
		return ++_base;
	}
private:
	static uint32 _base;
};

//============================================================
// <T>登录用户信息</T>
//============================================================
class CGlobalPlayerModule : public CSingleton<CGlobalPlayerModule>{
public:
	static CSharedMem*  mShmPtr;
public:
   CGlobalPlayer players[GS_PLAYER_MAXCOUNT];
	typedef lk::hash_map<int,int,GS_PLAYER_MAXCOUNT> GlobalPlayMap;
	typedef lk::hash_map<int,int,GS_PLAYER_MAXCOUNT>::iterator GlobalPlayerIterator;
	typedef lk::hash_map<int,int,GS_PLAYER_MAXCOUNT>::value_type GlobalPlayerValuePair;
	typedef std::pair<GlobalPlayerIterator, bool> _Pairib;
	GlobalPlayMap _playerMap;
public:
	CGlobalPlayerModule();
	void Initialize();
	void Resume();
public:
   static unsigned int CountSize();
   void* operator new(size_t size);
   void  operator delete(void* pointer);
   void* operator new(size_t size, const char* file, int line);
   void operator delete(void* pointer, const char* file, int line);
public:
	CGlobalPlayer* Alloc(int accountId){
		CGlobalPlayer* tpPlayer = GetByAccount(accountId);
		if(tpPlayer){
			return tpPlayer;
		}
		for(int i = 0; i < GS_PLAYER_MAXCOUNT; i++){
			if(!players[i].used){
				players[i].used = true;
				GlobalPlayerValuePair valuePair(accountId,i);
				if(_playerMap.insert(valuePair).second){
					LOG_DEBUG("default","Insert into global player map<%d,%d>.",accountId,i);
					return &players[i];
				}else{
					LOG_ERROR("default","Insert into global player map<%d,%d> error.",accountId,i);
					return NULL;
				}
			}
		}
		LOG_ERROR("default","Global player map is full.");
		return NULL;
	}
	bool Release(int accountId){
		GlobalPlayerIterator item = _playerMap.find(accountId);
		if(item == _playerMap.end()){
			LOG_ERROR("default","CGlobalPlayerModule Release unAlloc player with account:%d.",accountId);
			return false;
		}
		int index =(*item).second;
		players[index].Initialize();
		_playerMap.erase(item);
		LOG_DEBUG("default","Release global player<%d,%d> from map.",accountId,index);
		return true;
	}

	CGlobalPlayer* GetByAccount(int accountId){
		GlobalPlayerIterator item = _playerMap.find(accountId);
		if(item == _playerMap.end()){
			return NULL;
		}
		int index = (*item).second;
		return &players[index];
	}
	CGlobalPlayer* GetByIndex(int index){
		if(index < 0 || index >= GS_PLAYER_MAXCOUNT){
			return NULL;
		}
		if(players[index].used){
			return &players[index]; 
		}
		LOG_NOTICE("default","access an unused player.");
		return NULL;
	}
   bool FetchPlayerInfos(CGlobalPlayer& pPlayer);
};

// 同时认证人数上限
#define MAX_CERTIFY_CLIENT 65535
class CGlobalCertifyPlayerModule: public CSingleton<CGlobalCertifyPlayerModule>{
public:
	static CSharedMem*  mShmPtr;
	CCertifyPlayer _players[MAX_CERTIFY_CLIENT];
public:
	static unsigned int CountSize(){
		return sizeof(CGlobalCertifyPlayerModule);
	}
	void* operator new(size_t size){
		return (void*)mShmPtr->CreateSegment(size);
	}
	void  operator delete(void* pointer){

	}
	void* operator new(size_t size, const char* file, int line){
		return (void*)mShmPtr->CreateSegment(size);
	}
	void operator delete(void* pointer, const char* file, int line){

	}
public:
	void Initialize(){
		memset((char*)_players,0,sizeof(CCertifyPlayer)*MAX_CERTIFY_CLIENT);
	}
	void Resume(){

	}
	CCertifyPlayer* GetPlayerByComm(int socketIndex){
		LK_ASSERT((socketIndex > 0 && socketIndex < MAX_CERTIFY_CLIENT),NULL);
		memset((char*)&_players[socketIndex],0,sizeof(CCertifyPlayer));
		_players[socketIndex]._handle  = socketIndex;
		LOG_DEBUG("default","certify player come from socket:%d.",socketIndex);
		return &_players[socketIndex];
	}
	CCertifyPlayer* FindPlayerByComm(int socketIndex){
		LK_ASSERT((socketIndex > 0 && socketIndex < MAX_CERTIFY_CLIENT),NULL);
		return &_players[socketIndex];
	}

	bool FetchPlayerInfos(CCertifyPlayer* certifyPlayer){
		if(!CGlobalPlayerData::GetSingleton().FetchByPassport2(certifyPlayer,certifyPlayer->_passport)){
			return false;
		}
		return true;
	}
	CGlobalLogin* FindSuitableLoginServer(CCertifyPlayer* tpPlayer){
		int loginId;
		int nGate = tpPlayer->_gateCount;
		if(nGate > 0){  // 有角色
			SGlobalPlayerGateInfo* gates = tpPlayer->_gates;
			loginId = gates[0].loginId;
			int minRoleCount = gates[0].roleCount;
			for(int i = 1; i < nGate;i++){
				int nRoles  = gates[i].roleCount;
				int tempLoginId = gates[i].loginId;
				if(nRoles < minRoleCount){
					loginId = tempLoginId;
					minRoleCount = nRoles;
				}else if(nRoles == minRoleCount){ // 找一个在线人少的
					int PrevPlayerOnline = CGlobalLoginModule::GetSingleton().FindById(loginId)->playerCurrent;
					int CurPlayerOnline = CGlobalLoginModule::GetSingleton().FindById(tempLoginId)->playerCurrent;
					if(PrevPlayerOnline > CurPlayerOnline){
						loginId = tempLoginId;
					}
				}
			}
		}else{// 无角色 取认证数最少的Login
			int nLogin = CGlobalLoginModule::GetSingleton().loginCount;
			LK_ASSERT((nLogin > 0), NULL);
			loginId = CGlobalLoginModule::GetSingleton().Get(0).id;
			int minPlayers = CGlobalLoginModule::GetSingleton().Get(0).playerCurrent;
			for(int i = 1; i < nLogin; i++){
				int curCount = CGlobalLoginModule::GetSingleton().Get(i).loginCount ;
				if(curCount < minPlayers){
					loginId = CGlobalLoginModule::GetSingleton().Get(i).id;
					minPlayers = curCount;
				}
			}
		}
		return CGlobalLoginModule::GetSingleton().FindById(loginId);
	}
};

//============================================================
// <T>服务器逻辑</T>
//============================================================
class CGlobalLogic : public CSingleton< CGlobalLogic >{
public:
   // 对象初始化地址
   static CSharedMem*  mShmPtr;
   // 计算占用内存大小
   static unsigned int CountSize();
public:
   // 构造函数
   CGlobalLogic(){};
   // 析构函数
   ~CGlobalLogic(){};
   // 重载了new 与 delete 
   void* operator new( size_t size );
   void  operator delete( void* pointer );
public:
   // 初始化
   void Initialize();
   // 恢复运行
   void Resume();
   // 登陆服务器时钟
   void OnTimer( unsigned int vTickCount );
   // 响应重新读入loginserver.xml
   void OnReloadConfig();
   // 响应退出消息
   void OnExit();
public:
   // 响应客户到全局服务器来校验的消息
   void OnClientCertifyRequest(uint32 playerId, CMessage* pMessage);
   // 响应客户的消息
   void OnClientMessage(uint32 playerId, CMessage* pMessage);
public:
   // 登录服务器到全局服务器认证应答
   void OnLoginCertifyQueryResponse(uint32 handle, CMessage* pMessage);
   // 登录服务器到全局服务器认证令牌请求
   void OnLoginCertifyTockenRequest(uint32 handle, CMessage* pMessage);
   // 登录服务器向全局服务器通知角色信息
   void OnLoginRoleQueryNotify(uint32 handle, CMessage* pMessage);
   // 服务器信息通知
   void OnLoginOnlineStatusNotify(uint32 handle, CMessage* pMessage);
   // 响应Server的消息
   void OnLoginMessage(uint32 handle, CMessage* pMessage);

	// <T>登录服务器到全局服务器认证应答</T>
	void OnLoginCertifyQueryResponse(uint32 handle, CMessageCertifyQueryResponse* pCerifyLoginResponse);

	// <T>登录服务器到全局服务器认证令牌请求</T>
	void OnLoginCertifyTockenRequest(uint32 handle, CMessageCertifyTokenRequest* pCerifyTokenRequest);

	// <T>登录服务器向全局服务器通知角色信息</T>
	void OnLoginRoleQueryNotify(uint32 handle, CMessageRoleQueryNotify* pMessage);

	// <T>服务器信息通知</T>
	void OnLoginOnlineStatusNotify(uint32 handle, CMessageOnlineStatusNotify* pLoginStatusNotify);
};

//============================================================
// <T>服务器设置</T>
//============================================================
class CGlobalServerConfiguration
{
public:
   char  serverHost[GS_HOST_MAXLENGTH];            // 全局服务器信息
   int16 serverPort;                               // 全局服务器端口
   char  dataHost[GS_HOST_MAXLENGTH];              // 数据库主机
   int16 dataPort;                                 // 数据库端口
   char  dataLoginUser[GS_PASSPORT_MAXLENGTH];     // 数据库登录用户名
   char  dataLoginPassword[GS_PASSWORD_MAXLENGTH]; // 数据库密码
   char  dataDatabase[GS_DATABSE_MAXLENGTH];       // 数据库名称
   long  clientHandleIdleSleep;                   // 没有数据的睡眠时间
public:
	CGlobalServerConfiguration();
};

//============================================================
// <T>服务器</T>
//============================================================
class CGlobalServer : public CSingleton<CGlobalServer>{
public:
   static CSharedMem* mShmPtr;
   static unsigned int CountSize(); 

   //SSocketInfo socketInfo[65536];
   CGlobalServerConfiguration mConfig;
   CClientHandle* pClientHandle;
   CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH>* 	pLoginServer;
   //CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH>* pLoginSockets[GS_LOGIN_MAXCOUNT];
   //phtread_t mListenLoginThreadId;
   
   int mGlobalServerID;  // 全局服务器的server id
public:
   CGlobalServer(){}
   ~CGlobalServer(){}
public:
	CGlobalServerConfiguration& Configuration();
public:
	// 初始化
	int Initialize( int vInitFlag );
	// 启动服务器
	void Launch();
	// 运行服务器 
	void Run(const int nDaemon );
	// 从配置文件loginserver.xml 读取配置到vConfig
	int ReadCfg(CGlobalServerConfiguration& config);
public:
	// 接受连接
	void AcceptClients();
   // 从共享内存读取客户端过来的消息
   void RecvClientMsg(int vMsgCount);
   // 读取从服务器过来的消息
   void RecvLoginMsg();
	// 根据 login的 ip 和 port 分配 socket
	// 向玩家发送消息接口
	void Send2Player(CCertifyPlayer* pPlayer, CMessage* pMessage);
	// 向登录服务器发送消息接口
	bool Send2Login(int handle, CMessage& pMessage);
	// 打包的
	int PakMessage(CMessage& tMessage,char *msgBuffer,int bufferSzie);
	int UnPakMessage(unsigned char* szBuffer, unsigned int unBufferLength,CMessage* tpMessage);
};
#endif
