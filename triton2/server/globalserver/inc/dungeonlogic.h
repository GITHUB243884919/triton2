#pragma once
#include "servertool.h"
#include "lk_string.h"
#include "tinyxml.h"
#include "message_pb.hxx.pb.h"
#include "message_factory.h"
#include "tcp_conn.h"
#include "servertool.h"
#include "shm.h"
#include "servermessage_in.h"
#include "proxymessage_pb.hxx.pb.h"
#include "modulemanager.h"
using namespace DungeonServer;
#define KEEPALIVE_TIME 10000
struct CServerConfig
{
public:
	lk::CString64 mGlobalProxyAddr;	// 全局代理服务器地址
	int mServerID;						// 服务器ID
public:
	CServerConfig()
	{
		mServerID = 1;
		mGlobalProxyAddr.clear();
	}
};
class CDungeonServer : public CSingleton< CDungeonServer >
{
public:
	CDungeonServer();
	~CDungeonServer();
	int Initialize();
	int Resume();
public:
	enum ERunFlag
	{
		EFLG_CTRL_NO        = 0,
		EFLG_CTRL_QUIT      = 1,
		EFLG_CTRL_RELOAD    = 2
	};
public:
	
public:
static CSharedMem *m_pShareMem;
void * operator new ( size_t nSize );
void  operator delete( void *pBuffer );
static unsigned int CountSize();
public:
	CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> mGlobalProxy;
	CServerConfig mServerConfig;
public:
	void RecvServerMsg();
	void ReadConfig();	
	void ReadServerConfig();
public:
	void CheckTimer();
	int PrepareToRun();
	void Run();
	void CheckRunFlags();
	void SetRunFlag( int nFlag ){ mRunFlag = nFlag; }
	void ClearRunFlag( ){ mRunFlag = EFLG_CTRL_NO; }
	bool IsRunFlagSet( int nFlag ){ return mRunFlag == nFlag; }
	int ShutDown();
	int CheckProxyAlive( int nInterval );
	int OnLaunch();
public:	
	bool Connect2Proxy();
	int  Connect2Server();
	bool Regist2Proxy();
	void KeepAliveToProxy(  );
private:
	int mRunFlag;
	unsigned int mLastTickCount;
	CWTimer mTimerOfKeepAlive;
};

class CDungeonLogic : public CSingleton< CDungeonLogic >
{
public:
	CDungeonLogic();
	~CDungeonLogic();
	void* operator new( size_t size );
	void  operator delete( void* pointer );	
	static unsigned int CountSize( );
	static CSharedMem *m_pShareMem;
	int Resume();
	int Initialize();
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
public:
	void Launch();
	int ReadConfig();
	void OnServerMessage( CMessage *pMsg, int nGameID, int nSrcFe, int nSrcID );
	bool Send2Server( CMessage *pMsg, int nGameID, int nDesFe, int nDesID );
	void Send2Gate( CMessage *pMsg, int nGameID, int nServerID = 2 );
	void Send2Scene( CMessage *pMsg, int nGameID, int nServerID );	
};