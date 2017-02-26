#ifndef _FCM_SERVICE_H_
#define _FCM_SERVICE_H_

#include "module_manager.h"
#include "entity.h"
#include "lk_hashmap.h"
#include "lk_string.h"
#include "gateobj_define.h"
#include "lk_set.h"

using namespace GateServer;

class CServiceGateFcm :public CDataModule, public CSingleton< CServiceGateFcm >
{
public:
	void OnLaunchServer();
	void OnExitServer();
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	void OnMessage(int nServerID, CMessage* pMessage );

	CServiceGateFcm();
	~CServiceGateFcm();

	int Initialize();
	int Resume();
	void ReloadCfg();
	static unsigned int CountSize();

	void* operator new( size_t size );
	void  operator delete( void* pointer );

	virtual bool IsLaunched() { return true; }

	void InsertFcmPlayer(unsigned int nCharID, int nValidSeconds);
	void UpdateFcmPlayer( unsigned int nCharID, int nValidSeconds);
	void RemoveFcmPlayer(unsigned int nCharID);
	void SendFcmNotification(CGatePlayer* tpPlayer);
private:
	void ResetFcmStatus();

	void OnMessageFangChenMiRequest( int vServerID, CMessage* tpMessage );
	void OnMessageFCMPlayerResetRequest( int vServerID, CMessage* tpMessage );

	typedef std::pair<int, int> TREPORT_VALID_TIME;
	
	typedef lk::hash_map< int, TREPORT_VALID_TIME, PLAYER_AT_GATE_CAP > FCMPLAYERID_MAP;

	int mFcmRequestInterval;
	time_t mLatestResetTime;

	FCMPLAYERID_MAP mFcmPlayerList;
	CWTimer mFcmRptCheckTimer;
};

#endif

