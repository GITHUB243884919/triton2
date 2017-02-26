#ifndef _FRIEND_MODULE_H_
#define _FRIEND_MODULE_H_

#include "lk_hashmap.h"
#include "friendtype.h"
#include "friendmanager.h"
#include "protype.h"
#include "proclient_svr.h"
#include "coretype.h"
#include "friendmessage_pb.hxx.pb.h"
#include "property_pb.hxx.pb.h"
#include "servercore.h"
#include "servertool.h"
#include "dbinterface.h"
#include "teammodule.h"


class CEntityPlayer;

// ********************************************************************** //
// CFriendModule
// ********************************************************************** //

class MasterInfo
{
public:
	int	mMasterID;
	int mSlaveID;
	int mTime;
public:
	MasterInfo(){ }
	MasterInfo( int vMasterID, int vSlaveID ){ mMasterID = vMasterID; mSlaveID = vSlaveID; mTime = time(NULL); }
};

typedef lk::hash_map<int, CFriendItem, SERVER_CAP_PLAYER> FriendType ;
typedef lk::vector< MasterInfo, 1000 > MasterList;
#define MAX_LEVELUP_FRIEND_NUM  40 
#define MAX_LEVELUP_PLAYER_NUM  1000
#define MAX_SWORNTEAM_NUM		2000
struct CFriendBlessData
{
public:
	int mRoleID;
	int mLevel;
public:
	CFriendBlessData()
	{
		mRoleID = 0;
		mLevel = 0;
	}
};
class LevelUpFriendList : public lk::vector< CFriendBlessData, MAX_LEVELUP_FRIEND_NUM > 
{
public:
	LevelUpFriendList()
	{
		initailize();
	}
public:
	bool HasFriend( int nRoleID )
	{
		for ( iterator it = begin(); it != end(); ++it )
		{
			if ( (*it).mRoleID == nRoleID )
			{
				return true;
			}			
		}	
		return false;
	}
	void DeleteFriend( int nRoleID )
	{
		for ( iterator it = begin(); it != end(); ++it )
		{
			if ( (*it).mRoleID == nRoleID )
			{
				erase( it );
				break;
			}			
		}		
	}
	CFriendBlessData *GetData( int nRoleID )
	{
		for ( iterator it = begin(); it != end(); ++it )
		{
			if ( (*it).mRoleID == nRoleID )
			{
				return it;			
			}			
		}		
		return NULL;
	}
};

typedef lk::hash_map< int, LevelUpFriendList, MAX_LEVELUP_PLAYER_NUM > LevelUpPlayerList;
typedef lk::hash_map< int, CSwornTeam, MAX_SWORNTEAM_NUM > SwornTeamList;
class CFriendModule : public CLogicModule, public CDBReceiver, public CSingleton< CFriendModule >
{
	private:
		FriendType mRels;
		MasterList mMasterList;
		CWTimer	   mClearTimer;
		LevelUpPlayerList mLevelUpPlayerList;
		SwornTeamList mSwornTeamList;
		//typedef lk::hash_map<int, CPlayerRels, SERVER_CAP_PLAYER>::iterator iterator;
		//typedef lk::hash_map<int, CPlayerRels, SERVER_CAP_PLAYER>::const_iterator const_iterator;

	public:
		static char sModuleName[ NAME_LENGTH ];
		CFriendModule():CLogicModule(sModuleName)
		{
			if( mShmPtr->GetInitMode() == SHM_INIT ) 
			{
				mLevelUpPlayerList.initailize();
				mRels.initailize();
				mSwornTeamList.initailize();
			}
		}
	public:
		CFriendBlessConfig mFriendBlessConfig;
	public:	
		CFriendBlessConfig *GetFriendBlessConfig(){ return &mFriendBlessConfig; }
	public:
		// 启动服务器
		virtual void OnLaunchServer( );
		// 退出服务器
		virtual void OnExitServer( );
		// 服务器Timer
		virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
		// 建立场景
		virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );
		// 路由消息
		virtual void OnRouterMessage( CMessage* pMessage );

		// 客户端发送消息
		virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

		// 建立实体
		virtual void OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst );
		// 销毁实体
		virtual void OnDestroyEntity( CEntity* pEntity, int vCode );
		// 存储数据
		virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

		// 加载配置
		virtual void OnLoadConfig( ) ;

		// 场景启动完毕检查
		virtual bool OnCheckLaunch( ) { return true; }

		static unsigned int CountSize();

		void OnGateMessage( CMessage* pMessage );
		void SendRouterMsg2Client( CMessage* pMessage );


		CFriendItem* GetPlayerFriendItem(int nRoleID);

	//private:

		// ****基础功能部分****
		void PlayerAddFriendByName ( CEntityPlayer* pPlayer, const  char* strDesName ); // 加好友函数
		void PlayerAddFriendByID ( CEntityPlayer* pPlayer, int nDesCharID ); // 加好友函数
		void PlayerReplyAddFriend( int nSrcPlayerID, int nReplyCharID,  const char* szReplyName, int nHead
			, int nReplySex, int nApplyCode, unsigned char nIsFullList , unsigned char nIsReqInReplyLst, int nReplyNationID ); // 处理邀请回复
		void SendErrorCode ( int nCharID, EFriendModuleErrorCode ecode );
		void AskGateFriendInfo( int nRequesterID, const CFriendItem& rItem, bool bIsRefresh = false );
		void SendMyStatus2Friends( int nMyCharID, const std::vector<int>& rList, unsigned char nStat, int nRels );
		void SendMyStatus2Gate( int nMyCharID, unsigned char nStat );

		void InsertFriendData( int nCharID, const CFriendItem& rItem );
		void DelMember( CEntityPlayer* pPlayer, int nDesCharID, int oprType );

		// ****与数据库通信部分****

		// 去数据库查名字回调
		void AddBlackCallBack( CMessage* pMsg, int vparam1, int vparam2 );
		void OnPlayerAddFriendCallBack( CMessage* pMsg, int vparam1, int vparam2 );

		void CommonSqlCallBack( CMessage* pMsg, int vparam1, int vparam2 );

		// on message
		void OnMessageAddFriendOffLine( int nRequesterID, const char* szDesName );
		void OnPlayerReplyAddFriend( CEntityPlayer* pPlayer, int nSrcPlayerID, int nArgCode ); // 是否同意
		void OnMessagePlayerChangeStatus( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessageRecvOnlineFriends( CMessage* pMsg );
		void OnPlayerSendMessageRet( CMessage* pMsg );
		void OnReplayerAddRequesterInServer( CMessage* pMsg );
		void OnMessageAddFriendInDesServer( CMessage* pMessage );
		void OnMessagePlayerDelFriends( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessagePlayerChangeGroupName( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessagePlayerChangeFriendGroup( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessagePlayerSendMsg( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessagePlayerMoveBlack( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessagePlayerAddBlack( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessagePlayerLockEnemy( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessagePlayerRereshFriendState( CEntityPlayer* pPlayer, CMessage* pMsg );

		void OnMessageGetCharIDFromGate( CMessage* pMessage );
		void OnMessageAddFriend( CMessage* pMessage );
		void DisposeAddFriend( CEntityPlayer* pPlayer, int nDesID );
		void OnPlayerBeKilled( CEntityPlayer* pSrcPlayer, CEntityPlayer* pDesPlayer );

		void OnMessageChangeMasterRequest( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessageGetSlaveRequest( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessageQueryPlayerRequest( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessageRemoveSlaveRequest( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessageRemoveMasterRequest( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessageAlterMasterPercentRequest( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessageGetLowPlayerRequest( CEntityPlayer* pPlayer, CMessage* pMsg );
		void OnMessageAnswerChangeRequest( CEntityPlayer* pPlayer, CMessage* pMsg );

		void OnMessageChangeMasterResponse( CMessage* pMsg );
		void OnMessageChangeMasterNotify( CMessage* pMsg );
		void OnMessageGetSlaveResponse( CMessage* pMsg );
		void OnMessageSlaveLevelUpNotify( CMessage* pMsg );
		void OnMessageSlaveOnOffNotify( CMessage* pMsg );		
		void OnMessageQueryPlayerResponse( CMessage* pMsg );
		void OnMessageRemoveSlaveResponse( CMessage* pMsg );
		void OnMessageRemoveSlaveNotify( CMessage* pMsg );
		void SendSlaveOnOffNotify( CEntityPlayer *pSrcPlayer, int vOnline );
		void SendSlaveValueNotify( CEntityPlayer *pSrcPlayer );
		void OnMessageRemoveMasterResponse( CMessage* pMsg );
		void OnMessageRemoveMasterNotify( CMessage* pMsg );
		void SendAddMasterPointRequest( CEntityPlayer *pPlayer, int vRawPoint );
		void OnMessageAddMasterPointNotify( CMessage* pMsg );
		void OnMessageAnswerChangeNotify( CMessage* pMsg );
		
		// 改变状态通知
		void OnMessageFriendChangeStatusNotice( CMessage* pMsg );

		// 联系人状态改变通知
		void OnMessageContacterStatusNotice( CMessage* pMsg );

		// 与外部接口
		EPlayerRels GetRel2Me( int nCallerCharID, int nFriendID );

		// 设置好友关系
		int SetRel2Me( CEntityPlayer *pPlayer, int nFriendID, EPlayerRels eRels, EOprFriendCode eOprCode );
		
		bool IsInBlack( int nAcharid, int nBcharid ); // 聊天调用

public:
		void SendAllFriendNotice( CEntityPlayer* npPlayer );

		// ***********************************************************
		//  Function:		MakeRouterMessageByID	
		//  Description:	根据收发玩家ID生成一条路由消息
		//  Input:			源ID，目标ID，是否给玩家， 消息ID，消息体
		//  OutPut:			CMessage
		//  Return:			成功=0
		//  Others:
		//  Date:			2008.11.4
		// **********************************************************
		template< typename MSG_T >
		int MakeRouterMessageByID( unsigned int nSrcID, unsigned int nDesID, unsigned char nIstoPlayer
			, unsigned int nMessageID, MSG_T& rMsgpare,  CMessage* pMessage )
		{
			rMsgpare.mutable_router()->set_srccharid( nSrcID );
			rMsgpare.mutable_router()->set_descharid( nDesID );
			rMsgpare.mutable_router()->set_send2player( 0 );
			rMsgpare.mutable_router()->set_routertype( CMessageRouter::ROUTER_CHAR );
			pMessage->set_msgpara((uint32_t) &rMsgpare);
			pMessage->mutable_msghead()->set_messageid( nMessageID );
			return 0;
		}

		// ***********************************************************
		//  Function:		MakeRouterMessageByName	
		//  Description:	根据收发玩家名字生成一条路由消息
		//  Input:			源ID，目标ID，是否给玩家， 消息ID，消息体
		//  OutPut:			CMessage
		//  Return:			成功=0
		//  Others:
		//  Date:			2008.11.4
		// **********************************************************
		template< typename MSG_T >
		int MakeRouterMessageByName( const char* szSrcName, const char* szDesName, unsigned char nIstoPlayer
			, unsigned int nMessageID, MSG_T& rMsgpare, CMessage* pMessage )
		{
			rMsgpare.mutable_router()->set_srcname( szSrcName );
			rMsgpare.mutable_router()->set_desname( szDesName );
			rMsgpare.mutable_router()->set_send2player( 0 );
			rMsgpare.mutable_router()->set_routertype( CMessageRouter::ROUTER_NAME );
			pMessage->set_msgpara((uint32_t) &rMsgpare);
			pMessage->mutable_msghead()->set_messageid( nMessageID );
			return 0;
		}
public:
	void LoadFriendBlessConfig();
	void FriendLevelUp( CEntityPlayer *pPlayer, int nUpLevel );	
	void SendFriendLevelUpNotify( CEntityPlayer *pPlayer, int pDesCharID, CEntityPlayer *pDesPlayer = NULL );
	void OnMessageFriendBlessRequest( CEntityPlayer *pPlayer, CMessage *pMsg );
	void OnRouterMsgFriendLevelUpNotify( CMessage *pMsg );
	void SendFriendBlessResponse( CEntityPlayer *pPlayer, int nErrcode );
	void SendNotifyFriendBless( CEntityPlayer *pPlayer, int nRoleID, const char *pFriendName, int nExp );
	void OnRouterFriendBlessNotify( CMessage *pMsg );
	void SetLevelUpFriendID( int nRoleID, CFriendBlessData &rData );
	void DeleteLevelUpFriendID( int nRoleID, int nFriendRoleID );
	bool HasLevelUpFriendID( int nRoleID, int nFriendRoleID );
	void DeleteLevelUpFriendList( int nRoleID );
	CFriendBlessData *GetFriendBlessData( int nRoleID, int nFriendRoleID );

	// 加载好友祝福配置的回调函数
	static void LoadFriendBlessConfigCallback(const char* pcConfigPath);

	// 更新最近联系人
	int UpdateLatestContact(int nCharID, int nContacterID);
	int NotifyLastContacterToClient(int nCharID, int nContacterID);
	int NotifyLastContacterToGate(int nSenderID, int nReceiverID);
	int UploadContacterListToGate(int nCharID);
	void GetContacterListInfo( int nRequesterID, CFriendItem& rItem);
	void OnRouterMsgLastContacter(CMessage* pMessage);

	// 判断列表中所有人之间的亲密度是否超过某个值
	bool JudgeIntimacyDegree( int *pRoleIDList, int nNum, int nDegree );

	// 当玩家上线时更新某个玩家的亲密度
	void UpdateIntimacyDegree( CEntityPlayer *pPlayer );

	int GetIntimacyDegree( int nRole1, int nRoleID2, int &rIntimacyDegree );
	// 改变一组玩家相互之间的亲密度
	void ChangeGroupIntimacyDegree( int *pRoleIDList, int nNum, EM_EntimacyChangeType nType, int nValue = 0 );

	// 发送修改亲密度的通知
	void SendModifyIntimacyNotify( int nRoleID, int nChangedValue, int nIntimacy, EM_EntimacyChangeType nType, int nFriendID, int nUpdateTime = 0, int nDailyIntimacy = 0 );

	// 发送更新同时在线时间的通知
	void SendModifyLastTimeAllOnLineNotify( CEntityPlayer *pPlayer, int nRoleID, int nTime );

	// 玩家下线的时候同步最后同时在线时间
	void UpdateLastTimeAllOnLine( CEntityPlayer *pPlayer );

	// 减少两个玩家的亲密度
	void ChangeIntimacy( int nRoleID1, int nRoleID2, EM_EntimacyChangeType eType, int nValue = 0 );
	
	// 更新组队玩家之间的亲密度
	void UpDateTeamIntimacy( CTeam* tpTeam );

	// 创建结拜的消息请求
	void OnMessageCreateSwornTeam( CEntityPlayer *pPlayer, CMessage *pMsg );
	// 创建结拜的回应消息
	void SendResponseCreateSwornTeam( CEntityPlayer *pPlayer, int nErrcode );
	// 创建结拜的通知
	void SendNotifyCreateSwornTeam(  CEntityPlayer **pPlayer, int nTeamCount, CSwornTeam *pSwornTeam );	
	// 离开结拜团体的请求
	void OnMessageLeaveSwornTeam( CEntityPlayer *pPlayer, CMessage *pMsg );
	// 离开结拜团体的消息回应
	void SendResponseLeaveSwornTeam( CEntityPlayer *pPlayer, int nErrcode );
	// 离开结拜团体的消息通知
	void SendNotifyLeaveSwornTeam( CEntityPlayer *pPlayer, CSwornTeam *pSwornTeam );
	// 踢人请求
	void OnMessageKickSwornMember( CEntityPlayer *pPlayer, CMessage *pMsg );
	// 踢人回应
	void SendResponseKickSwornMember( CEntityPlayer *pPlayer, int nErrcode );
	// 踢人通知
	void SendNotifyKickSwornMember( CEntityPlayer *pPlayer, CSwornTeam *pSwornTeam, int nRoleID );
	// 解散结拜团体
	void OnMessageDisbandSwornTeam( CEntityPlayer *pPlayer, CMessage *pMsg );
	// 解散结拜团体的回应
	void SendResponseDisbandSwornTeam( CEntityPlayer *pPlayer, int nErrcode );
	// 解散结拜团体的通知
	void SendNotifyDisbandSwornTeam( CSwornTeam *pSwornTeam );	
	// 发送结拜团体的信息
	void SendNotifySwornInfo( CEntityPlayer *pPlayer, CSwornTeam *pSwornTeam );		
	// 改变结拜团体名称的请求
	void OnMessageChangeSwornName( CEntityPlayer *pPlayer, CMessage *pMsg );
	// 改变结拜团体名称的回应
	void SendResponseChangeSwornName( CEntityPlayer *pPlayer, int nErrcode );
	// 改变结拜团体名称的通知
	void SendNotifyChangeSwornName( CSwornTeam *pSwornTeam );
	// 重建结拜的消息请求
	void OnMessageRebuildSworn( CEntityPlayer *pPlayer, CMessage *pMsg );
	// 发送创建结拜的消息回应
	void SendResponseRebuildSworn( CEntityPlayer *pPlayer, int nErrcode );
	// 发送重新结拜的消息通知
	void SendNotifyRebuildSworn( CEntityPlayer **pPlayer, int nTeamCount, CEntityPlayer **pNewMember, int nNewMemberNum, CSwornTeam *pSwornTeam );

	// 插入一个结拜团体信息
	int InsertSwornTeam( int nSwornID, CSwornTeam &rSwornTeam );
	// 获取一个结拜团体信息
	CSwornTeam *GetSwornTeam( int nSwornID );
	// 删除一个结拜团体信息
	int DeleteSwornTeam( int nSwornID );
	// 创建结拜的DB信息
	int CreateSwornDBInfo( CSwornTeam &rSwornTeam );
	// 删除结拜的DB信息
	int DeleteSwornDBInfo( int nSwornID );
	// 获取某人的结拜信息
	int GetSwornDBInfo( CEntityPlayer *pPlayer );
	// 更新结拜信息
	int UpdateSwornDBInfo( int nSwornID, CSwornTeam &rSwornTeam );
	// 更新玩家结拜ID
	int UpDateRoleSwornID( int nRoleID, int nSwornID );
	// 获取结拜信息
	int OnSessionGetSwornDBInfo( CMessage *pMsg, int nParam1, int nParam2 );
	// 获取结拜信息超时
	int OnTimeOutGetSwornDBInfo( int nParam1, int nParam2 );
	// 网关返回的结拜信息
	void OnRouterMsgSwornInfo( CMessage *pMsg );
	// 网关返回的离开结拜的消息
	void OnRouterMsgLeaveSwornTeamNotify( CMessage *pMsg );
	// 修正好友亲密度的消息
	void OnRouterMsgModifyIntimacy( CMessage *pMsg );
	// 踢人通知
	void OnRouterMsgKickSwornMember( CMessage *pMsg );
	// 解散结拜的消息通知
	void OnRouterMsgDisbandSwornTeam( CMessage *pMsg );
	// 结拜改名的通知消息
	void OnRouterMsgChangeSwornName( CMessage *pMsg );
	// 设置最后的同时登录时间
	void OnRouterMsgModifyLastTimeAllOnLine( CMessage *pMsg );
	// 检查因为亲密度结婚可以减少的石头数量
	int CheckItemNumAffectedByIntimacy( int nSrcCharID, int nDesCharID, int &rNum );	
	// 检查队伍中结拜成员的结拜亲密度
	int CheckTeamSwornIntimacy( CEntityPlayer *pPlayer, int nValue );
};

#endif
