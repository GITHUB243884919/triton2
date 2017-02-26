/************************************************************************/
//	Class Name	:	CGateTeamManager	
//	Description	:	To manager all team at the gate server;
//					Mostly deal with logic of team;
//	Modifier	:	GaoHong
//	Found Time	:	2008-10-31
/************************************************************************/

#pragma once

#include "lk_hashmap.h"

#include "teamtype.h"
#include "gateteam.h"
#include "module_manager.h"
#include "gateobj_define.h"
#include "gateobj_manager.h"
#include "gateobj_manager_imp.h"

#include "servermessage_pb.hxx.pb.h"
#include "teammessage_pb.hxx.pb.h"

using namespace GateServer;

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

class CServiceGateTeam: public CDataModule, public CSingleton< CServiceGateTeam >
{
public:	
	
	// charid mapped to teamid
	typedef lk::hash_map< int, int, PLAYER_AT_GATE_CAP > TeamList;		
	TeamList		mTeamList;		

	// charid mapped to it's msg(claim\recruit)
	typedef lk::hash_map<int, STeamMsg, PLAYER_AT_GATE_CAP> ClaimList;
	typedef lk::hash_map<int, STeamMsg, PLAYER_AT_GATE_CAP> RecruitList;
	ClaimList		mClaimList;
	RecruitList		mRecruitList;

	// maintain invite & request relation
	typedef lk::vector< int, MAX_INVITE_NUM > BaseReqInvList;
	typedef lk::hash_map< int, BaseReqInvList, PLAYER_AT_GATE_CAP > BaseReqInvMap;

	BaseReqInvMap	mRequestList;
	BaseReqInvMap	mInviteList;
	

	int mUpdateTime;

	typedef lk::hash_map< int , int , PLAYER_AT_GATE_CAP > BaseQueryTime;
	BaseQueryTime	mQueryTime;		// record time stamp of query claim & recruit message
public:
	CServiceGateTeam();
	~CServiceGateTeam();

	void OnLaunchServer();
	void OnExitServer();
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	void OnMessage(int nServerID, CMessage* pMessage );

	static unsigned int CountSize();

	void* operator new( size_t size );
	void  operator delete( void* pointer );

	virtual bool IsLaunched() { return true; }

public:
	// create team self
	void OnMessageCreateTeamBySelf( CMessage* pMessage );
	// request join team
	void OnMessageRequestJoinTeam( CMessage* pMessage );
	// reply join team request
	void OnMessageReplyJoinRequest( CMessage* pMessage );
	// invite to join team
	void OnMessageInviteJoinTeam( CMessage* pMessage );
	// reply team invite
	void OnMessageReplyJoinInvite( CMessage* pMessage );
	// kick out of team
	void OnMessageKickoutTeamRequest( CMessage* pMessage );
	// leave team
	void OnMessageLeaveTeamRequest( CMessage* pMessage );
	// end team
	void OnMessageEndTeamRequest( CMessage* pMessage );
	// change team header
	void OnMessageChangeHeaderRequest( CMessage* pMessage );

	void OnMessageSendClaimRecruitRequest( CMessage* pMessage );
	void OnMessageQueryClaimRecruitRequest( CMessage* pMessage );
	void OnMesssageDeleteClaimRecruitRequest( CMessage* pMessage );

	void OnMessageUpdateGateTeam( CMessage* pMessage );
	void NotifySceneToRefreshTeam();

	void OnMessageFollowRequest( CMessage* pMessage );
	void OnMessageChangeDistribute( CMessage* pMessage );

	void OnMessageObtainGateTeamRequest( CMessage* pMessage );
	void OnMessageClearInvReqRequest( CMessage* pMessage);

	void SendAllMessage( CGateTeam* vpTeam, CMessage* pMessage );

	// utility interface
	void SendErrorNotice( CGatePlayer* pPlayer, int vCode, bool bTeam = false, const char* vpName = NULL, const char* vpName2 = NULL );
	// is in team
	int InTeam( int vCharID );

	// insert member team relation 
	int InsertMember( int vCharID, int vTeamID );
	// remove member team relation
	int RemoveMember( int vCharID );
	// remove team
	int RemoveTeam( CGateTeam* vpTeam );

	// notify scene to update team
	int SendUpdateNotifyToScene( CGateTeam* vpTeam, int vCharID, int vCode );

	// notify scene to send team list to des
	int SendListNotifyToScene( CGateTeam* vpTeam, int vCharID );

	// change team header interface
	int ChangeHeaderByLeave( CGateTeam* vpTeam, int vCharID );
	// force leave team 
	void ForceLeaveTeam( int vCharID );
	
	// check if all member in game scene
	bool IsAllInGame( CGateTeam* vpTeam );

	int GetTeamList( int vCharID, int* pMemberList, int vNum );
	int ClearTeamData( int vCharID );

	int MakeTeamItemPB( PBTeamItem* pItem, CGatePlayer* pPlayer );
	int MakeTeamPB( PBTeamInfo* pInfo, int vCharID );
	int MakeMsgPB( PBMsg* pDesMsg, PBMsg* pSrcMsg );
	int MakeTeamMsgOnRedirect( PBMsg* pMsg, int vCharID );
	int MakePbMsgToStruct( PBMsg* pSrcMsg, STeamMsg* pDesMsg );
	int MakeStructMsgToPB( STeamMsg* pSrcMsg, PBMsg* pDesMsg );

	void DeleteClaimRecruit( CGatePlayer* pPlayer );
	void RefreshRecruitMsg( CGatePlayer* pPlayer, int vTeamCount );

	// check if in request or invite relation map

	int GetRecInvitedNum( int nCharID );

	bool CheckCoolTime( int nCharID );

	template< typename TYPE_T >
	bool IsInRelated( TYPE_T& nList, int nCharID, int nTeamID )
	{
		BaseReqInvMap::iterator it = nList.find( nCharID );
		if ( it != nList.end() )
		{
			BaseReqInvList* tpList = ( BaseReqInvList* ) &( it->second );
			BaseReqInvList::iterator itList = tpList->begin();
			for ( ; itList != tpList->end(); ++ itList )
			{
				if ( nTeamID == *itList )
				{
					return true;
				}
			}
		}
		return false;
	}

	template< typename TYPE_T >
	int InsertValuePair( TYPE_T& nList, int nCharID, int nTeamID )
	{
		BaseReqInvMap::iterator it = nList.find( nCharID );
		if ( it == nList.end() )
		{
			BaseReqInvList tList;
			tList.initailize();
			tList.push_back( nTeamID );
			nList.insert( BaseReqInvMap::value_type( nCharID, tList ) );
		}
		else
		{
			bool bHasExit = false;
			BaseReqInvList* tpList = ( BaseReqInvList* ) &( it->second );
			BaseReqInvList::iterator itList = tpList->begin();
			for ( ; itList != tpList->end(); ++ itList )
			{
				if ( *itList == nTeamID )
				{
					bHasExit = true;
				}
			}
			if ( bHasExit == false )
			{
				tpList->push_back( nTeamID );
			}
		}
		return SUCCESS;
	}

	template< typename TYPE_T >
	int ClearValuePair( TYPE_T& nList, int nCharID )
	{
		BaseReqInvMap::iterator it = nList.find( nCharID );
		if ( it != nList.end() )
		{
			nList.erase( it );
		}
		return SUCCESS;
	}

	template< typename TYPE_T >
	int RemoveValuePair( TYPE_T& nList, int nCharID, int nTeamID )
	{
		BaseReqInvMap::iterator it = nList.find( nCharID );
		if ( it != nList.end() )
		{
			BaseReqInvList* tpList = ( BaseReqInvList* ) &( it->second );
			if ( tpList->size() == 0 )
			{
				nList.erase( it );
			}
			else
			{
				BaseReqInvList::iterator itList = tpList->begin();
				for ( ; itList != tpList->end(); ++ itList )
				{
					if ( *itList == nTeamID )
					{
						tpList->erase( itList );
						break;
					}
				}
				if ( tpList->size() == 0 )
				{
					nList.erase( it );
				}
			}
		}
		return SUCCESS;
	}

	void ClearGateTeamDataOnLeave( int nCharID );

	inline int GetTotalPageIndex( int nSize )
	{
		if ( ( nSize % PAGE_MAX ) == 0 )
		{
			return ( nSize / PAGE_MAX );
		}
		else
		{
			return ( ( nSize / PAGE_MAX ) + 1 );
		}
	}
};


