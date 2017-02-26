
#include "westserver.h"
#include "errcode.h"
#include "teamtype.h"
#include "gateteammanager.h"
#include "gatelogic.h"

#include "servermessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "teammessage_pb.hxx.pb.h"
#include "promessage_pb.hxx.pb.h"
#include "core_service.h"

template<> CServiceGateTeam* CSingleton< CServiceGateTeam >::spSingleton = NULL;

void CServiceGateTeam::OnLaunchServer()
{

}

void CServiceGateTeam::OnExitServer()
{

}

void CServiceGateTeam::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	//PrintTeam();
	mUpdateTime -= vTickOffset;
	if ( mUpdateTime <= 0 )
	{
		mUpdateTime = CGateLogic::GetSingleton().mConfig.nTeamProSynTimeout;
		NotifySceneToRefreshTeam();
	}
}

void* CServiceGateTeam::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void CServiceGateTeam::operator delete( void* pointer )
{

}


CServiceGateTeam::CServiceGateTeam( )
{
	if ( mShmPtr->GetInitMode() == SHM_INIT )
	{
		mTeamList.initailize();
		mClaimList.initailize();
		mRecruitList.initailize();
		mRequestList.initailize();
		mInviteList.initailize();
		mQueryTime.initailize();
		mUpdateTime	= CGateLogic::GetSingleton().mConfig.nTeamProSynTimeout;
	}
}

CServiceGateTeam::~CServiceGateTeam( )
{
	mTeamList.clear();
}

// *******************************************************
//	Function Name : CountSize
//	Description   : 计算模块消耗内存大小
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-18 
// *******************************************************

unsigned int CServiceGateTeam::CountSize()
{
	return sizeof(CServiceGateTeam);
}

int CServiceGateTeam::ChangeHeaderByLeave( CGateTeam* vpTeam, int vCharID )
{
	if ( vpTeam == NULL )
	{
		return -1;
	}

	if ( vpTeam->IsHeader( vCharID ) == false )
	{
		return -1;
	}

	if ( InTeam( vCharID ) <= 0 )
	{
		LOG_ERROR( "gateteam", "entity not in team,interface params may be error.id=%d,[%s:%d]", vCharID, __LK_FILE__, __LINE__ );
		vpTeam->PrintTeam();
		return -1;
	}

	int tMemberList[ TEAM_NUMBER_MAX ] = { 0 };
	int tRtn = 0, tNum = 0;
	tRtn = vpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
	if ( tRtn < 0 )
	{
		LOG_ERROR( "gateteam", "get member list failed,id=%d,[%s:%d]", vpTeam->get_id(), __LK_FILE__, __LINE__ );
		vpTeam->PrintTeam();
		return -1;
	}

	for ( int i = 0; i < tNum && i < (int)ARRAY_CNT( tMemberList ); i ++ )
	{		
		if ( tMemberList[ i ] != vCharID )
		{
			continue;
		}
		int tNewHead = tMemberList[ ( i + 1 ) % tNum ];
		CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tNewHead );
		if ( tpPlayer == NULL )
		{
			continue;
		}
		SendUpdateNotifyToScene( vpTeam, tNewHead, ACTION_CHANGEHEADER );
		vpTeam->SetHeadID( tpPlayer->mCharID );
		vpTeam->SetHeadName( tpPlayer->mCharName );
		break;
	}

	return SUCCESS;
}

int CServiceGateTeam::SendUpdateNotifyToScene( CGateTeam* vpTeam, int vCharID, int vCode )
{
	if ( vpTeam == NULL )
	{
		return -1;
	}

	int tMemberList[ TEAM_NUMBER_MAX ] = { 0 };
	int tRtn = 0, tNum = 0;
	tRtn = vpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
	if ( tRtn < 0 )
	{
		LOG_ERROR( "gateteam", "get member list failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return -1;
	}	

	for ( int i = 0; i < tNum; i ++ )
	{
		CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tMemberList[ i ] );
		if ( tpPlayer == NULL )
		{
			continue;
		}

		CMessage tMessage;
		CMessageNotifySceneToUpdateTeam tPara;
		tPara.set_headid( vpTeam->GetHeadID() );
		tPara.set_descharid( tpPlayer->mCharID );
		tPara.set_code( vCode );
		int tRtnMaken = vpTeam->MakeMemberPB( vCharID, tPara.mutable_item() );
		if ( tRtnMaken < 0 )
		{
			LOG_ERROR( "gateteam", "make member pb failed.id=%d,[%s:%d]", tMemberList[ i ], __LK_FILE__, __LINE__ );
			continue;
		}
		pbmsg_sethead(&tMessage, ID_G2S_NOTIFY_UPDATETEAM, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara( (long) &tPara );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
	}

	// print team members
	vpTeam->PrintTeam();
	return SUCCESS;
}

int CServiceGateTeam::SendListNotifyToScene( CGateTeam* vpTeam, int vCharID )
{
	if ( vpTeam == NULL )
	{
		return -1;
	}

	CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( vCharID );
	if ( tpPlayer == NULL )
	{
		return -1;
	}

	CMessage tMessage;
	CMessageNotifySceneToSendTeamList tPara;
	tPara.set_descharid( vCharID );

	int tRtn = vpTeam->MakeTeamPB( tPara.mutable_teaminfo() );
	if ( tRtn < 0 )
	{
		LOG_ERROR( "gateteam", "make team pb failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return -1;
	}
	pbmsg_sethead(&tMessage, ID_G2S_NOTIFY_SENDLIST, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara( (long) &tPara );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
	return SUCCESS;
}

void CServiceGateTeam::SendErrorNotice( CGatePlayer* pPlayer, int vCode, bool bTeam /* = false */, const char* vpName /* = NULL */, const char* vpName2 /* = NULL */ )
{
	if ( pPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageNotifyErrToScene tPara;	
	tPara.set_descharid( pPlayer->mCharID );
	tPara.set_code( vCode ); 	
	pbmsg_sethead(&tMessage, ID_G2S_NOTIFY_ERRORNOTICE, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara( (long) &tPara );

	if ( vpName != NULL )
	{
		tPara.set_param1( vpName );
	}

	if ( vpName2 != NULL )
	{
		tPara.set_param2( vpName2 );
	}

	if ( bTeam )
	{
		CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( InTeam( pPlayer->mCharID ) );
		if ( tpTeam == NULL )
		{
			CGateServer::GetSingleton().Send2Scene( pPlayer, &tMessage );
		}
		else
		{
			int tMemberList[ TEAM_NUMBER_MAX ] = { 0 };
			int tRtn = 0, tNum = 0;
			tRtn = tpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
			if ( tRtn < 0 || tNum == 0  )
			{
				CGateServer::GetSingleton().Send2Scene( pPlayer, &tMessage );
			}
			else
			{
				for ( int i = 0; i < tNum && i < (int) ARRAY_CNT( tMemberList ); i ++ )
				{
					CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tMemberList[ i ] );
					if ( tpPlayer != NULL )
					{
						tPara.clear_descharid();
						tPara.set_descharid( tMemberList[ i ] );
						CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
					}
				}
			}
		}
	}
	else
	{
		CGateServer::GetSingleton().Send2Scene( pPlayer, &tMessage );
	}
	return ;
}

int CServiceGateTeam::InTeam( int vCharID )
{
	TeamList::iterator it = mTeamList.find( vCharID );
	if ( it == mTeamList.end() )
	{
		return 0;
	}
	return it->second;
}

bool CServiceGateTeam::IsAllInGame( CGateTeam* vpTeam )
{
	if ( vpTeam == NULL )
	{
		return false;
	}

	int tMemberList[ TEAM_NUMBER_MAX ] = { 0 };
	int tRtn = 0, tNum = 0;

	tRtn = vpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
	if ( tRtn < 0 )
	{
		return false;
	}

	for ( int i = 0; i < tNum && i < (int) ARRAY_CNT( tMemberList ); i ++ )
	{
		CGatePlayer* tpPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tMemberList[ i ] );
		if ( tpPlayer == NULL )
		{
			return false;
		}
		if ( tpPlayer->mStatus != CGatePlayer::em_status_ingame_idle )
		{
			return false;
		}
	}
	return true;
}

int CServiceGateTeam::InsertMember( int vCharID, int vTeamID )
{
	TeamList::iterator it = mTeamList.find( vCharID );
	if ( it != mTeamList.end() )
	{
		return -1;
	}
	mTeamList.insert( TeamList::value_type( vCharID, vTeamID ) );
	return SUCCESS;
}

int CServiceGateTeam::RemoveMember( int vCharID )
{
	TeamList::iterator it = mTeamList.find( vCharID );
	if ( it == mTeamList.end() )
	{
		return -1;
	}
	mTeamList.erase( it );
	return SUCCESS;
}

int CServiceGateTeam::RemoveTeam( CGateTeam* vpTeam )
{
	if ( vpTeam == NULL )
	{
		return -1;
	}

	int tMemberList[ TEAM_NUMBER_MAX ] = { 0 };
	int tRtn = 0, tNum = 0;

	tRtn = vpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
	if ( tRtn < 0 )
	{
		LOG_ERROR( "gateteam", "get member list failed.id=%d,[%s:%d]", vpTeam->get_id(), __LK_FILE__, __LINE__ );
		vpTeam->PrintTeam();
		return -1;
	}

	for ( int i = 0; i < tNum; ++ i )
	{
		TeamList::iterator it = mTeamList.find( tMemberList[ i ] );
		if ( it != mTeamList.end() )
		{
			mTeamList.erase( it );
		}
	}

	vpTeam->DelTeam();
	int tTeamID = vpTeam->get_id();
	CGateObjManager::GetSingleton().DestroyObject( tTeamID );
	return SUCCESS;
}

int CServiceGateTeam::GetTeamList( int vCharID, int* pMemberList, int vNum )
{
	if ( pMemberList == NULL )
	{
		return 0;
	}

	int tTeamID = InTeam( vCharID );
	if ( tTeamID != INVALID_OBJ_ID )
	{
		CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
		if ( tpTeam == NULL )
		{
			pMemberList[ 0 ] = vCharID;
			return 1;
		}
		int tRtn = 0, tNum = 0;
		tRtn = tpTeam->GetMemberList( pMemberList, vNum, tNum );
		if ( tRtn < 0 )
		{
			pMemberList[ 0 ] = vCharID;
			return 1;
		}
		return tNum;
	}
	else
	{
		pMemberList[ 0 ] = vCharID;
		return 1;
	}
	return SUCCESS;
}

int CServiceGateTeam::ClearTeamData( int vCharID )
{
	int tTeamID = InTeam( vCharID );
	if ( tTeamID != INVALID_OBJ_ID )
	{
		CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
		if ( tpTeam == NULL )
		{
			LOG_ERROR( "gateteam", "object invalid,id=%d,[%s:%d]", tTeamID, __LK_FILE__, __LINE__ );
			return -1;
		}
		if ( tpTeam->GetCount() == 1 )
		{
			RemoveTeam( tpTeam );
		}
		else
		{
			tpTeam->DelMember( vCharID );
			RemoveMember( vCharID );
		}
	}

	ClearGateTeamDataOnLeave( vCharID );
	return SUCCESS;
}

int CServiceGateTeam::MakeTeamItemPB( PBTeamItem* pItem, CGatePlayer* pPlayer )
{
	if ( pItem == NULL || pPlayer == NULL )
	{
		return -1;
	}

	pItem->set_charid( pPlayer->mCharID );
	pItem->set_name( pPlayer->mCharName );
	pItem->set_face( pPlayer->mFaceType );
	pItem->set_level( pPlayer->mLevel );
	pItem->set_metier( pPlayer->mMetierID );
	pItem->set_sex( pPlayer->mSexID );
	pItem->set_nationality( pPlayer->mCountryID );
	pItem->set_lineid( pPlayer->mLineID );
	pItem->set_mapid( pPlayer->mMapID );
	pItem->set_posx( pPlayer->mPoint.mX );
	pItem->set_posy( pPlayer->mPoint.mY );
	pItem->set_status( (int) emOnLine );

	return SUCCESS;
}

int CServiceGateTeam::MakeTeamPB( PBTeamInfo* pInfo, int vCharID )
{
	if ( pInfo == NULL )
	{
		return -1;
	}

	int tTeamID = InTeam( vCharID );
	CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
	if ( tpTeam == NULL )
	{
		return -1;
	}

	return tpTeam->MakeTeamPB( pInfo );
}


void CServiceGateTeam::ForceLeaveTeam( int vCharID )
{
	ClearGateTeamDataOnLeave( vCharID );
	CMessage tMessage;
	CMessageLeaveGateTeamRequest tPara;
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_charid( vCharID );
	OnMessageLeaveTeamRequest( &tMessage );
}

void CServiceGateTeam::NotifySceneToRefreshTeam()
{
	CGateObjManagerImp* tpObjManager = CGateObjManager::GetSingleton().mGateObjManagerImp;
	CGateObjManagerImp::ObjMng_TeamAtGate::iterator it = tpObjManager->mTeamAtGatePool.begin();
	CGateObjManagerImp::ObjMng_TeamAtGate::iterator itEnd = tpObjManager->mTeamAtGatePool.end();
	for ( ; it != itEnd; ++ it )
	{
		CGateTeam* tpTeam = ( CGateTeam* ) &( *it );
		if ( tpTeam == NULL )
		{
			continue;
		}
		CMessage tMessage;
		CMessageTeamPropertyNotifyCallBack tPara;
		pbmsg_sethead(&tMessage, ID_S2C_NOTIFY_SYNPROPERTY, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara( (long) &tPara );
		
		int tMemberList[ TEAM_NUMBER_MAX ] = { 0 };
		int tNum = 0, tRtn = 0;
		tRtn = tpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
		if ( tRtn < 0 )
		{
			continue;
		}
		for ( int i = 0; i < tNum && i < (int) ARRAY_CNT( tMemberList ); i ++ )
		{
			tpTeam->MakePbTeamMember( tMemberList[ i ], tPara.add_itempro() );
		}	

		tPara.set_itemsize( tNum );
		SendAllMessage( tpTeam, &tMessage );
	}
	return ;
}


void CServiceGateTeam::DeleteClaimRecruit( CGatePlayer* pPlayer )
{
	if ( pPlayer == NULL ) return ;

	CMessage tMessage;
	CMessageDeleteClaimRecruitResponse tPara;
	pbmsg_sethead( &tMessage, ID_G2S_RESPONSE_DELETECLAIMRECRUIT, 0, 0, 0, 0, 0, 0 );
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_charid( pPlayer->mCharID );

	RecruitList::iterator itRecruit = mRecruitList.find( pPlayer->mCharID );
	if ( itRecruit != mRecruitList.end() )
	{
		mRecruitList.erase( itRecruit );
		tPara.set_actiontype( (int) TEAM_CONSCRIBE );
		CGateServer::GetSingleton().Send2Scene( pPlayer, &tMessage );
	}

	ClaimList::iterator itClaim = mClaimList.find( pPlayer->mCharID );
	if ( itClaim != mClaimList.end() )
	{
		mClaimList.erase( itClaim );
		tPara.set_actiontype( (int) TEAM_ASKFOR );
		CGateServer::GetSingleton().Send2Scene( pPlayer, &tMessage );
	}
	
	return ;
}

void CServiceGateTeam::RefreshRecruitMsg( CGatePlayer* pPlayer, int vTeamCount )
{
	if ( pPlayer == NULL ) return ;

	RecruitList::iterator it = mRecruitList.find( pPlayer->mCharID );
	if ( it == mRecruitList.end() )
	{
		return ;
	}

	STeamMsg* tpMsg = (STeamMsg*) &(it->second);
	tpMsg->mCount = vTeamCount;

	CMessage tMessage;
	CMessageSendClaimRecruitResponse tPara;
	pbmsg_sethead( &tMessage, ID_G2S_RESPONSE_CLAIMRECRUIT, 0, 0, 0, 0, 0, 0 );
	tPara.set_charid( pPlayer->mCharID );
	tPara.set_actiontype( (int) TYPE_RECRUIT );
	PBMsg* tpDesMsg = tPara.mutable_msg();
	MakeStructMsgToPB( tpMsg, tpDesMsg );
	tMessage.set_msgpara( (long) &tPara );
	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMessage );
	return ;
}

void CServiceGateTeam::ClearGateTeamDataOnLeave( int nCharID )
{
	ClearValuePair<BaseReqInvMap>( mInviteList, nCharID );
	ClearValuePair<BaseReqInvMap>( mRequestList, nCharID );

	CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( InTeam( nCharID ) );
	if ( tpTeam != NULL )
	{
		tpTeam->RemoveReq( nCharID );
	}

	// if has claim message
	ClaimList::iterator it = mClaimList.find( nCharID );
	if ( it != mClaimList.end() )
	{
		mClaimList.erase( it );
	}

	// clear query record
	BaseQueryTime::iterator itQuery = mQueryTime.find( nCharID );
	if ( itQuery != mQueryTime.end() )
	{
		mQueryTime.erase( itQuery );
	}

	return ;
}

int CServiceGateTeam::GetRecInvitedNum( int nCharID )
{
	BaseReqInvMap::iterator it = mInviteList.find( nCharID );
	if ( it != mInviteList.end() )
	{
		BaseReqInvList* tpList = ( BaseReqInvList* ) &( it->second );
		return tpList->size();
	}
	return 0;
}


bool CServiceGateTeam::CheckCoolTime( int nCharID )
{
	// check if delay out of 20s from last time
	time_t tNow = time( NULL );
	BaseQueryTime::iterator it = mQueryTime.find( nCharID );
	if ( it == mQueryTime.end() )
	{
		mQueryTime.insert( BaseQueryTime::value_type( nCharID, (int) tNow ) );
	}
	else
	{
		int tLastTime = it->second;
		if ( ( tNow - tLastTime ) < 20 )
		{
			return true;
		}
		else
		{
			mQueryTime.erase( it );
			mQueryTime.insert( BaseQueryTime::value_type( nCharID, (int) tNow ) );
		}
	}
	return false;
}
