#ifndef _GATE_SERVICE_H_
#define _GATE_SERVICE_H_

#include "module_manager.h"
#include "entity.h"
#include "lk_hashmap.h"
#include "lk_string.h"
#include "gateobj_define.h"

using namespace GateServer;

class CServiceGateMail :public CDataModule, public CSingleton< CServiceGateMail >
{
public:
	void OnLaunchServer();
	void OnExitServer();
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	void OnMessage(int nServerID, CMessage* pMessage );

	CServiceGateMail();
	~CServiceGateMail();

	int Initialize();
	int Resume();
	static unsigned int CountSize();

	virtual bool IsLaunched() { return true; }

	void* operator new( size_t size );
	void  operator delete( void* pointer );

private:
	void OnMessageGetMailInfoRequest(int vServerID, CMessage* tpMessage );
	void OnMessageSendSysMail( CMessage* tpMessage );
};


#endif

