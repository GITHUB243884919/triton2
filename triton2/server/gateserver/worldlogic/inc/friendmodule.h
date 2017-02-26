#ifndef _FRIEND_GATE_H_
#define _FRIEND_GATE_H_

#include "module_manager.h"
#include "entity.h"
#include "lk_hashmap.h"
#include "lk_string.h"
#include "lk_vector.h"
#include "lk_queue.h"
#include "gateobj_define.h"
#include "servertool.h"
#include "friendtype.h"

using namespace GateServer;

class CFriendModule : public CDataModule, public CSingleton< CFriendModule >
{
	// 单个玩家最近联系人
	typedef lk::vector<int, LATEST_CONTACT_MAX> LatestContactType;
	// 所有玩家的最近联系人
	typedef lk::hash_map<int, LatestContactType, PLAYER_AT_GATE_CAP> PlayerLastestContactList;
	PlayerLastestContactList m_LatestContactList;

public:
	CFriendModule();
	virtual void OnLaunchServer() { }
	virtual void OnExitServer() { }
	virtual void OnTimer(unsigned int, unsigned int) { }
	virtual void OnMessage(int, CMessage*);

	virtual bool IsLaunched() { return true; }

	static int CountSize( ) { return sizeof( CFriendModule ); }

	void OnMessagePlayerOnlineFriendRequest(int nServerID, CMessage* tpMessage);
	void OnMessagePlayerChangeImStatusNotice(int nServerID, CMessage* tpMessage);
	void OnMessagePlayerAddFriend(int nServerID, CMessage* tpMessage);
	void OnMessagePlayerGetCharID(int nServerID, CMessage* pMessage);

	void OnMessageChangeMasterRequest( int nServerID, CMessage* pMessage );
	void OnMessageGetSlaveRequest( int nServerID, CMessage* pMessage );
	void OnMessageSlaveOnOffNotify( int nServerID, CMessage* pMessage );
	void OnMessageQueryPlayerRequest( int nServerID, CMessage* pMessage );
	void OnMessageRemoveSlaveRequest( int nServerID, CMessage* pMessage );
	void OnMessageAnswerChangeRequest( int nServerID, CMessage* pMessage );
	void OnMessageAcquireMasterNotify( int nServerID, CMessage* pMessage );

	// 上传最近联系人列表
	void OnMessageUploadContacterListNotify( int nServerID, CMessage* pMessage );
	// 更新联系人
	void OnMessageUpdateLastContacterNotify( int nServerID, CMessage* pMessage );

	void SendChangeMasterResponse( int vResult, int vMasterID, int vCharID );
	void SendChangeMasterNotify( int vSlaveID, int vCharID );
	void SendSlaveLevelUpNotify( int vSlaveID, int vCharID );
	void SendRemoveSlaveResponse( int vResult, int vSlaveID, int vCharID, int vParam );
	void SendRemoveSlaveNotify( int vCharID, int vMasterID );
	void SendRemoveMasterResponse( int vResult, int vMasterID, int vCharID );
	void SendRemoveMasterNotify( int vCharID, int vSlaveID, int vParam );
	void SendUpdateMatrixInfoRequest( int vCharID, int vSubType, int vValue = 0, int vParam = 0 );
	void OnMessageRemoveMasterRequest( int nServerID, CMessage* pMessage );
	void OnMessageAddMasterPointRequest( int nServerID, CMessage* pMessage );
	void SendAddMasterPointNotify( int vMasterID, int vPoint );
	void SendAddNumNotify( int vSlaveID, int vMasterID, int vParam );
	void SendAnswerChangeNotify( int vSlaveID, int vCharID );

	int  AddContacter(int nCharID, int nContacterID);
	int  ClearContacterOfOnePlayer(int nCharID);
	int  SendMyStatusToContacterList(CGatePlayer* pPlayer, EPlayerShow eStatus);
	int  SendMyStatusToContacter(int nCharID, EPlayerShow eStatus, int nContacterID);
	void OnMessageSwornInfo( int nServerID, CMessage *pMsg );
	void OnMessageLeavelSwornTeamNotify( CMessage *pMsg );
	void OnMessageModifyIntimacyNotify( CMessage *pMsg );
	void OnMessageKickSwornMemberNotify( CMessage *pMsg );
	void OnMessageDisbandSwornTeamNotify( CMessage *pMsg );
	void OnMessageChangSwornNameNotify( CMessage *pMsg );
	void OnMessageModifyLastTimeAllOnline( CMessage *pMsg );
	void OnMessageChangeSwornName( CMessage *pMsg );
};

#endif



