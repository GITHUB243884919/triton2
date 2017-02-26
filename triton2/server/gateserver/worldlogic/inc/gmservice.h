#ifndef _GM_SERVICE_H_
#define _GM_SERVICE_H_

#include "module_manager.h"
#include "entity.h"
#include "lk_hashmap.h"
#include "lk_string.h"
#include "gateobj_define.h"
#include "servermessage_pb.hxx.pb.h"

using namespace GateServer;

class CServiceGmCmd :public CDataModule, public CSingleton< CServiceGmCmd >
{
public:
	void OnLaunchServer();
	void OnExitServer();
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	void OnMessage(int nServerID, CMessage* pMessage );

	CServiceGmCmd();
	~CServiceGmCmd();

	int Initialize();
	int Resume();
	static unsigned int CountSize();

	void* operator new( size_t size );
	void  operator delete( void* pointer );

	virtual bool IsLaunched() { return true; }

	void OnSessionLoadRoleLocationResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionLoadTaskInfoResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionFreezeAccountResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionUnFreezeAccountResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionTrusteeUserResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionUnTrusteeUserResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionRecoverRoleResult(CMessage* tpMessage, int nSessionID, int nfd);

	// 2.4 版本新增	
	void OnSessionLoadStorageInfoResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionLoadRoleDetailResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionLoadRoleEquipmentResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionLoadRolePackInfoResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionLoadRoleItemInPackResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionLoadRoleSkillInfoResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionLoadRoleMailResult(CMessage* tpMessage, int nSessionID, int nfd, int nType);

	void OnSessionUserSocialInfoResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnSessionSpouseInfoResult( CMessage* pMessage, int nSessionID, int nfd );
	void OnSessionMatrixInfo( CMessage* pMessage, int nSessionID, int nfd );
private:
	void OnGmMessageKickRoleCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageMoveRoleCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageRoleLocationCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageTaskInfoCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageEndTaskCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageDeleteRoleCmd(int nServerID, CMessage* tpMessage); 
	void OnGmMessageRecoverRoleCmd(int nServerID, CMessage* tpMessage); 
	void OnGmMsgUserInfoByIPCmd(int nServerID, CMessage* tpMessage);

	void OnGmMessageUnFreezeAccountCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageFreezeAccountCmd(int nServerID, CMessage* tpMessage);
	void OnGMMessageFreezeAccountBatCmd(int nServerID, CMessage* tpMessage);	// 批量冻结
	void OnGMMessageUnFreezeAccountBatCmd(int nServerID, CMessage* tpMessage);	// 批量解冻
	void KickOffPlayer(CGatePlayer* tpPlayer, int reason);
	void OnGmMessageTrusteeUserCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageUnTrusteeUserCmd(int nServerID, CMessage* tpMessage);


	void OnGmMessageShutUpCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageCancelShutUpCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessagePostBulletinCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageUserSocialInfoCmd(int nServerID, CMessage* tpMessage);
	void GetPlayerSocialFriendInfo( CMessage* tpMessage );
	void GetPlayerSocialFamilyInfo( CMessage* tpMessage );
	void GetPlayerSocialCorpsInfo( CMessage* tpMessage );
	void GetPlayerSocialSpouseInfo( CMessage* tpMessage );
	void GetPlayerMatixInfo( CMessage* tpMessage );		 

	//2.4 版本新增
	void OnGmMessageRoleDetailCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageRoleEquipmentCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageRolePackInfoCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageRoleItemInPackCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageRoleSkillInfoCmd(int nServerID, CMessage* tpMessage);
	void OnGmMessageRoldSkillDetailCmd(int nServerID, CMessage* tpMessage);
	void OnGMMessageUserStorageInfoCmd(int nServerID, CMessage* tpMessage);

	void SendGmCmdNormalResult(int nResult, unsigned int nSessionID, unsigned int nFd);

	void OnGMMessageCancelMultiExpCmd(int nServerID, CMessage* tpMessage); 
	void OnGMMessageMultiExpCmd(int nServerID, CMessage* tpMessage);
	void OnGMMessageChangeGate(int nServerID, CMessage* tpMessage);
	void OnGMMessageResetRolePassword(int nServerID, CMessage* tpMessage);

	// 新增系统邮件接口
	void OnGMMsgSendSysMail2Player( int nServerID, CMessage* tpMessage );

	// 查询玩家邮件
	void OnGmMessageQueryRoleMailCmd(int nServerID, CMessage* tpMessage);

	// 查询玩家称号
	void OnGmMessageRoleTitleCmd(int nServerID, CMessage* tpMessage);
};


#endif


