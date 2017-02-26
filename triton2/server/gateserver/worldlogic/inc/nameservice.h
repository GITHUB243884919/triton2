#ifndef _MAIL_SERVICE_H_
#define _MAIL_SERVICE_H_

#include "module_manager.h"
#include "entity.h"
#include "lk_hashmap.h"
#include "lk_string.h"
#include "gateobj_define.h"

using namespace GateServer;

class CServiceRoleIDName :public CDataModule, public CSingleton< CServiceRoleIDName >
{
public:
	enum { 
		FETCH_FIELD_COUNT = 9,
	};

	void OnLaunchServer();
	void OnExitServer();
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	void OnMessage(int nServerID, CMessage* pMessage );

	virtual bool IsLaunched();

	CServiceRoleIDName();
	~CServiceRoleIDName();

	int Initialize();
	int Resume();
	static unsigned int CountSize();

	void* operator new( size_t size );
	void  operator delete( void* pointer );

	void LoadRoleInfoFromDB(int nStart, int nRowCount);
	void OnSessionLoadRoleInfoFromDB(CMessage* tpMessage, int nStart, int ONCECOUNT);

	int  RoleCount(int nAccountID);

	void LoadRoleCountFromDB();
	void OnSessionLoadRoleCountFromDB(CMessage* tpMessage, int nStart, int ONCECOUNT);

	CRoleGlobalInfo* CreateRoleInfo( int nRoleID, const char* szRoleName, int nAccountID);
	void RemoveRoleInfoByID( int nRoleID);
	CRoleGlobalInfo* GetRoleInfoByRoleName(const char* szRoleName);
	CRoleGlobalInfo* GetRoleInfoByRoleID(int nRoleID);
private:
	void RemoveRoleInfoByName( const char* szRoleName);
	void InsertRoleIDIndex(CRoleGlobalInfo& roleinfo);
	void InsertRoleNameIndex(CRoleGlobalInfo& roleinfo);

	void OnServiceRoleIDNameRequest(int nServerID, CMessage* pMessage);

	typedef lk::hash_map<int, int, ROLEGLOBALINFO_CAP> ROLEINFOIDINDEXTYPE;
	typedef lk::hash_map<lk::string<NAME_LENGTH>, int, ROLEGLOBALINFO_CAP> ROLEINFONAMEINDEXTYPE;
	typedef lk::hash_map<int, int, ROLEGLOBALINFO_CAP> ACCOUNTHASROLETYPE;

	ROLEINFOIDINDEXTYPE	mIndexOfRoleID;
	ROLEINFONAMEINDEXTYPE mIndexOfRoleName;
	ACCOUNTHASROLETYPE mAccountHasRole;

	int m_nNeedLoadCount; 
	int m_nHaveLoadCount;
};


#endif


