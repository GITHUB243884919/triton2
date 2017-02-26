#pragma once
#include "servercore.h"
#include "servertool.h"
#include "sceneobj_define.h"
#include "teamtype.h"
#include "base_define.h"
#include "object.h"
#include "lk_vector.h"
#include "lk_hashmap.h"
#include "coretype.h"
#include "errcode.h"


#define TEAM_ADD_TRIP					1000
#define TEAM_LEAVE_TRIP					1001	
#define TEAM_END_TRIP					1002
#define TEAM_INV_MAP					1003
#define TEAM_REC_MAP					1004

class CEntityPlayer;
class PBTeamItem;
class PBMember;
class CMessage;
class CSharedMem;
class PBMsg;
class PBTeamMsg;
class CEntity;
class CCreator;
class PBTeamInfo;


// 属性更新类型
enum PARAM_TYPE
{
	emMetier	= 1,
	emLevel		= 2,
};

struct STeamMsg
{
	int		mType;		// 求组 == TEAM_ASKFOR？招募 == TEAM_CONSCRIBE
	int		mCharID;
	char	mName[NAME_LENGTH];
	int		mMetier;
	int		mLevel;
	int		mMaxLevel;
	int		mMinLevel;
	char	mContent[NAME_LENGTH];
	int		mCount;
	int		mRootStyle;  // 一级条件
	int		mSubStyle;	 // 二级条件
	int		mNationality;	// 国籍 

	STeamMsg()
	{
		if ( CObj::msCreateMode )
		{
			mType = 0;
			mCharID = 0;
			mName[0] = '\0';
			mMetier = 0;
			mLevel = 0;
			mMaxLevel = 0;
			mMinLevel = 0;
			mContent[0] = '\0';
			mCount = 0;
			mRootStyle = 0;
			mSubStyle = 0;
			mNationality = 0;
		}
	}
};





// 组队事件buff
class CTeamEventBuff
{
public:
	class CPlayerTeamEvent
	{
	public:
		CPlayerTeamEvent() {}
		~CPlayerTeamEvent() {}
		CPlayerTeamEvent( int nCharID ) : mCharID( nCharID ) {}

		int mCharID;
	};

public:

	CTeamEventBuff( int nBuffID, int nLastRefresh,  int nEventType, bool bForceRefresh )
		: mBuffID( nBuffID ), mLastRefreshTime( nLastRefresh ), mEventType( nEventType ), mForceRefresh( bForceRefresh )
	{
		mBeneficiary.initailize();
	}

	CTeamEventBuff( )
	{
		if( CObj::msCreateMode )
		{
			Initialize();
		}
	}

	~CTeamEventBuff() {};

	int Initialize( )
	{
		mBuffID = 0 ;				// buff id
		mEventType = 0;				// 事件类型
		mForceRefresh = 0;			// 强制刷新	
		mLastRefreshTime = 0;
		mBeneficiary.initailize();
		return 0;
	}

	int mBuffID;					// buff id
	int	mLastRefreshTime;
	int mEventType;					// 事件类型
	bool mForceRefresh;				// 强制刷新	


	typedef lk::vector< CPlayerTeamEvent, TEAMNUM > TeamEventBeneficiary;
	TeamEventBeneficiary mBeneficiary; // 受益人（队员）
};


class CTeam : public CObj
{
public:
	int		mTeamID;
	int		mHeaderID;
	int		mMode;
	int		mUpdateTime;
	typedef lk::vector< int, TEAMNUM > TeamVector;
	TeamVector mTeamVec;

	// maintenance team distribute 
	typedef lk::vector< int, TEAMNUM > TeamDistributeVector;
	TeamDistributeVector mTeamDistribute;

	typedef lk::hash_map< int, CTeamEventBuff, 4 > TeamBuffEventList;
	TeamBuffEventList mTeamBuffEventList;

public:
	CTeam( );
	~CTeam( );

	// initial
	virtual int Initialize();
	// resume
	virtual int Resume(); 

	// Set & Get TeamID
	inline void SetTeamID( int vTeamID ) { mTeamID = vTeamID; };
	inline int GetTeamID() { return mTeamID; }

	// Set & Get HeaderID
	inline void SetHeaderID( int vHeaderID ) { mHeaderID = vHeaderID; }
	inline int GetHeaderID() { return mHeaderID; }

	inline void SetMode( int vMode ) { mMode = vMode; }
	inline int	GetMode() { return mMode; }

	// refresh team data
	int RefreshTeam( int* pMemberList, int vNum );

	// add member
	int AddMember( int vCharID );
	// del member
	int DelMember( int vCharID );
	// del team
	int DelTeam();
	
	// GetMemberList
	int GetMemberList( int* pMemberList, int vNum, int& vRtnNum );

	int GetOwner();

	bool IsTurned( int vCharID );
	int PushTurnOwner( int vCharID );
	int RemoveTrunOwner( int vCharID );
	int RemoveTeamEventBuff( int nCharID );

	void PrintTeam();

	bool AddTeamBuffEvent( CTeamEventBuff& rBuffEvent );

	void UpdateTeamBuffEvent( );
};

// ********************************************************************** //
// CTeamModule
// ********************************************************************** //

class CTeamModule : public CLogicModule, public CSingleton< CTeamModule >
{
public:
	static char		sModuleName[ NAME_LENGTH ];
	
	typedef lk::hash_map< int, int, SERVER_CAP_PLAYER > TeamList;
	TeamList		mTeamlist;

	typedef lk::hash_map< int, STeamMsg, SERVER_CAP_PLAYER > ClaimList;
	typedef lk::hash_map< int, STeamMsg, SERVER_CAP_PLAYER > RecruitList;
	ClaimList		mClaimList;
	RecruitList		mRecruitList;

public:
	CTeamModule( );
	~CTeamModule();
		
	static unsigned int CountSize();	

	int mTeamPropertyUpTime;

public:
	// launch scene server
	virtual void OnLaunchServer( );
	// exit scene server
	virtual void OnExitServer( );
	// timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	// create scene maps
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );
	// process message from router contains gate, and other scene
	virtual void OnRouterMessage( CMessage* pMessage );
	// process client message
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );
	// create entity when login scene
	virtual void OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst );
	// destroy entity when leave scene
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );
	// save data
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );
	// load config
	virtual void OnLoadConfig( );

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }
	// process message from gate just in scope of team
	void OnGateMessage( CMessage* pMessage );

	// get member list
	// params: vNum: Array Size; vRtnNum : Member Count 
	int OnGetMemberList( int vCharID, int* pMemberList, int tNum, int& tRtnNum );
	int OnGetMemberList(CEntityPlayer* pPlayer, CEntityPlayer **pPlayerArray, int tNum, int& tRtnNum );
	int OnGetMemberList( int vTeamID, CEntityPlayer** pMemberList, int tNum, int& tRtnNum );

	// 普通物品分配
	int GetTurnOwner( CEntityPlayer* pPlayer, int* pCharList, int vNum, CEntity *pDesEntity );
	// 任务物品分配
	CEntityPlayer* GetTurnOwnerOnTask( CEntityPlayer* pPlayer );
	// 得到队长
	CEntityPlayer* GetTeamHeader( CEntityPlayer* pPlayer );

	// 自创建队伍
	void OnMessageCreateTeamSelf( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 组队邀请
	void OnMessageTeamInvite( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 邀请回复处理
	void OnMessageInvitedProc( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 请求入队
	void OnMessageRequestJoin( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 入队申请处理
	void OnMessageRecievedPorc( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 离开队伍
	void OnMessageLeaveTeam( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 踢出队伍
	void OnMessageKickPlayer( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 解散队伍
	void OnMessageEndTeam( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 转移队长
	void OnMessageAttornHeader( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 改变分配模式
	void OnMessageChangeDistribute( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 招募
	void OnMessageRecruit( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 求组
	void OnMessageClaimTeam( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 清空申请信息
	void OnMessageClearReqMap( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 清空邀请信息
	void OnMessageClearInviteMap( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 搜索求组信息
	void OnMessageSearchClaimMsg( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 搜索招募信息
	void OnMessageSearchRecruitMsg( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 删除求组招募信息
	void OnMessageDeleteCRMsg( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 请求跟随队长
	void OnMessageRequestFollow( CEntityPlayer* pPlayer, CMessage* pMessage );
	// 全队跟随
	void OnMessageFollowOrder( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnGateCreateTeam( CMessage* pMesage );
	void OnGateRequestJoinTeam( CMessage* pMessage );
	void OnGateInviteJoinTeam( CMessage* pMessage );
	void OnGateInvite2Request( CMessage* pMessage );
	void OnGateUpdateTeam( CMessage* pMessage );
	void OnGateSendTeamList( CMessage* pMessage );
	void OnGateSendErrNotice( CMessage* pMessage );
	void OnGateRefreshTeam( CMessage* pMessage );

	void OnGateSendClaimRecruitResponse( CMessage* pMessage );
	void OnGateQueryClaimRecruitResponse( CMessage* pMessage );
	void OnGateDeleteClaimRecruitResponse( CMessage* pMessage );
	void OnGateFollowResponse( CMessage* pMessage );
	void OnGateChangeDistribute( CMessage* pMessage );

	void SendTeamList(  );
	void SendNotifyToClient( PBMember* pMember, int vCode, int vDesCharID );
	void MakeMemberPB( PBMember* pMember, CEntityPlayer* pPlayer );
	void MakeMemberPB( PBMember* pMember, PBTeamItem* pItem );
	void MakeTeamItemPB( PBTeamItem* pItem, CEntityPlayer* pPlayer );
	void MakeMsgPB( PBTeamMsg* pTeamMsg, PBMsg* pMsg );
	void NotifyTeamFlag( CEntityPlayer* pPlayer, int vFlag );
	void NotifyEndTeam( CEntityPlayer* pPlayer );

	void CreateTeamOnRedirect( CEntityPlayer* pPlayer, PBTeamInfo* pTeam );
	void CreateTeamMsgOnRedirect( PBMsg* pMsg, CEntityPlayer* pPlayer );
	void RefreshTeam( int* vpTeamList, CTeam* vpTeam, int vNum );

	void SendObtainGateTeamRequest( int nCharID );

	void RequestUpdateGateTeamProperty();
	void ProcessRedirectScene( CEntityPlayer* pPlayer );

	void ProcessTripDisplay( CTeam* vpTeam, CEntityPlayer* vpPlayer, int vAction );

	void LoadTeamMsg( CEntityPlayer* pPlayer, PBTeamMsg* pMsg );

	int InsertMember( int vCharID, int vTeamID );
	int RemoveMember( int vCharID );
	int RemoveTeam( CTeam* vpTeam );
	int InTeam( int vCharID );
	int GetTeamHeadCharID( int vCharID );

	// 获取队伍中在当前服务器上在线的第一个成员charid
	int GetLocalFirstOLMemberID( int nCharID );

	int MakeStructMsgToPB( STeamMsg* pSrcMsg, PBTeamMsg* pDesMsg );
	int MakePbMsgToStruct( PBMsg* pSrcMsg, STeamMsg* pDesMsg );

	template< typename MSG_T >
	int MakeMessage( unsigned int nMessageID, MSG_T& nMsgPara, CMessage* vpMessage )
	{
		vpMessage->mutable_msghead()->set_messageid( nMessageID );
		vpMessage->set_msgpara( (long) &nMsgPara );
		return SUCCESS;
	}

	// 添加组队事件buff
	bool AddTeamEventBuff( CEntityPlayer* pPlayer, CTeamEventBuff& rEventBuff );

	// 删除组队事件buff
	void RemoveTeamEventBuff( CEntityPlayer* pPlayer );
};


