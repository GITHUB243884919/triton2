#include "stdafx.hpp"
#include "teammodule.h"
#include "mapmodule.h"
#include "propertymodule.h"
#include "modulemanager.h"
#include "sceneobj_manager.h"
#include "sceneobj_manager_imp.h"
#include "message_pb.hxx.pb.h"
#include "teammessage_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "entity.h"

CTeam::CTeam()
{
	Initialize();
	mUpdateTime = time( NULL );
}

CTeam::~CTeam()
{

}

int CTeam::Initialize()
{
	mTeamID = 0;
	mHeaderID = 0;
	mMode = 0;
	mTeamVec.initailize();
	mTeamDistribute.initailize();
	mTeamBuffEventList.initailize();
	return SUCCESS;
}

int CTeam::Resume()
{
	return SUCCESS;
}

int CTeam::RefreshTeam( int* pMemberList, int vNum )
{
	if ( pMemberList == NULL )
	{
		return -1;
	}

	mTeamVec.clear();
	for ( int i = 0; i < vNum && pMemberList[ i ] > 0 ; i ++ )
	{
		mTeamVec.push_back( pMemberList[ i ] );
	}
	return SUCCESS;
}

int CTeam::GetMemberList( int* pMemberList, int vNum, int& vRtnNum )
{
	if ( pMemberList == NULL )
	{
		return -1;
	}
	vRtnNum = 0;
	TeamVector::iterator it = mTeamVec.begin();
	for ( ; it != mTeamVec.end(); ++ it)
	{
		if ( vRtnNum < vNum )
		{
			pMemberList[ vRtnNum ] = *it;
			vRtnNum ++;
		}		
	}
	return SUCCESS;
}

int CTeam::AddMember( int vCharID )
{
	TeamVector::iterator it = mTeamVec.begin();
	for ( ; it != mTeamVec.end(); ++ it )
	{
		if ( vCharID == *it )
		{
			return -1;
		}
	}
	if ( mTeamVec.size() == TEAMNUM )
	{
		return -1;
	}
	mTeamVec.push_back( vCharID );

	LOG_INFO( "team", ">>>Team Join: member ( %d ) join .", vCharID );

	return SUCCESS;
}

int CTeam::DelMember(int vCharID)
{
	TeamVector::iterator it = mTeamVec.begin();
	for ( ; it != mTeamVec.end(); ++ it )
	{
		if ( vCharID == *it )
		{
			mTeamVec.erase( it );

			LOG_INFO( "team", ">>>Team Leave: member ( %d ) leave .", vCharID );

			break;
		}
	}
	RemoveTrunOwner( vCharID );

	// 队员离开时同时删除组队事件buff
	RemoveTeamEventBuff( vCharID );

	return SUCCESS;
}

int CTeam::DelTeam()
{
	LOG_INFO( "team", ">>>Team End: team ( %d ) end .", mTeamID );

	TeamVector::iterator it = mTeamVec.begin();
	for ( ; it != mTeamVec.end(); ++ it )
	{
		RemoveTeamEventBuff( *it );
	}

	mTeamVec.clear();
	SetHeaderID( 0 );
	SetTeamID( 0 );
	mTeamDistribute.clear();
	return SUCCESS;
}

void CTeam::PrintTeam()
{
	LOG_INFO( "team", "--------------Team Member List------------" );
	LOG_INFO( "team", "HeadID:%d, TeamCount: %d", mHeaderID, mTeamVec.size() );
	TeamVector::iterator it = mTeamVec.begin();
	for ( ; it != mTeamVec.end(); ++ it )
	{
		LOG_INFO( "team", "CharID : %d", *it );
	}
	LOG_INFO( "team", "------------------------------------------" );
}


bool CTeam::AddTeamBuffEvent( CTeamEventBuff& rBuffEvent )
{
	bool bNeedInsertBuff = false;

	TeamBuffEventList::iterator it = mTeamBuffEventList.find( rBuffEvent.mBuffID );
	if( it == mTeamBuffEventList.end() )
	{
		bNeedInsertBuff = true;
		mTeamBuffEventList.insert( TeamBuffEventList::value_type(rBuffEvent.mBuffID, rBuffEvent ) );
	}
	else
	{
		CTeamEventBuff& rEventBuff = it->second;

		// 只是刷新时间
		rEventBuff.mLastRefreshTime = rBuffEvent.mLastRefreshTime;

		// 刷新受益队员列表
		rEventBuff.mBeneficiary = rBuffEvent.mBeneficiary;

		// 强制刷新一次Buff,没有的就加上
		if( rBuffEvent.mForceRefresh == true )
		{
			bNeedInsertBuff = true;
			rBuffEvent.mForceRefresh = false;
		}

	}

	if( bNeedInsertBuff )
	{
		CTemplateBuff* pBuffTpl = (CTemplateBuff*)CDataStatic::SearchTpl( rBuffEvent.mBuffID );
		if( pBuffTpl == NULL )
		{
			return false;
		}

		for( unsigned int i = 0; i <  rBuffEvent.mBeneficiary.size(); i++ )
		{
			CEntityPlayer* pPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID( rBuffEvent.mBeneficiary[i].mCharID );
			if( pPlayer == NULL )
			{
				continue;
			}

			if( pPlayer->HasBuff( rBuffEvent.mBuffID ) == false )
			{
				pPlayer->EntityInsertBuff( pPlayer, pBuffTpl );
				int pTime = pBuffTpl->mLastTime/1000;
				if (pTime >= 60)
				{
					 LogEventGetBuffBySystem(pPlayer,rBuffEvent.mBuffID,pTime);
				}
				
			}
		}
	}

	return true;
}

void CTeam::UpdateTeamBuffEvent( )
{
	TeamBuffEventList::iterator tBuffIter = mTeamBuffEventList.begin();
	if( tBuffIter != mTeamBuffEventList.end() )
	{
		unsigned int tNow = time(NULL);

		CTeamEventBuff& rEvent = tBuffIter->second;
		// 客户端是每10s上报一次
		if( tNow - rEvent.mLastRefreshTime >= 15 )
		{
			// 如果组队buff整体已经超时，所有队员删除这个buff
			for(TeamVector::iterator it = mTeamVec.begin(); it != mTeamVec.end(); ++it )
			{
				CEntityPlayer* pPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID( *it );
				if( pPlayer == NULL )
				{
					continue;
				}

				if( pPlayer->HasBuff( rEvent.mBuffID ) == true )
				{
					pPlayer->EntityRemoveBuff( rEvent.mBuffID );
					LogEventLostBuffByTimeout(pPlayer,rEvent.mBuffID );  //  团队整体超时
				}
			}

			mTeamBuffEventList.erase( tBuffIter++ );
		}
		else
		{
			for( TeamVector::iterator it = mTeamVec.begin(); it != mTeamVec.end(); ++it )
			{
				unsigned int i = 0;
				for( i = 0; i < rEvent.mBeneficiary.size(); i++)
				{
					if( rEvent.mBeneficiary[i].mCharID == (*it) )
						break;
				}

				// 没找到的话
				if( i == rEvent.mBeneficiary.size() )
				{
					CEntityPlayer* pPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID( *it );
					if( pPlayer == NULL )
					{
						continue;
					}
					if( pPlayer->HasBuff( rEvent.mBuffID ) == true )
					{
						pPlayer->EntityRemoveBuff( rEvent.mBuffID );
						LogEventLostBuffByTimeout(pPlayer,rEvent.mBuffID );   //  团队整体超时
					}
				}
			}

			++tBuffIter;
		}
	}
}

bool CTeam::IsTurned( int vCharID )
{
	TeamDistributeVector::iterator it = mTeamDistribute.begin();
	for ( ; it != mTeamDistribute.end(); ++ it )
	{
		if ( vCharID == *it )
		{
			return true;
		}
	}
	return false;
}

int CTeam::PushTurnOwner( int vCharID )
{
	TeamDistributeVector::iterator it = mTeamDistribute.begin();
	for ( ; it != mTeamDistribute.end(); ++ it )
	{
		if ( vCharID == *it )
		{
			return -1;
		}
	}
	mTeamDistribute.push_back( vCharID );
	return SUCCESS;
}

int CTeam::RemoveTeamEventBuff( int nCharID )
{
	// 如果队员在本场景一定距离内，需要删除团队事件buff
	CEntityPlayer* pPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID( nCharID );
	if( pPlayer != NULL )
	{
		for( TeamBuffEventList::iterator it = mTeamBuffEventList.begin(); 
			it !=  mTeamBuffEventList.end();
			++it )
		{
			if( pPlayer->HasBuff( it->second.mBuffID ) == true )
			{
				pPlayer->EntityRemoveBuff( it->second.mBuffID );
				LogEventLostBuffByCloseAureole(pPlayer,it->second.mBuffID );  // 光环消失
			}
		}
	}

	return 0;
}
int CTeam::RemoveTrunOwner( int vCharID )
{
	TeamDistributeVector::iterator it = mTeamDistribute.begin();
	for ( ; it != mTeamDistribute.end(); ++ it )
	{
		if ( vCharID == *it )
		{
			mTeamDistribute.erase( it );
			break;
		}
	}
	return SUCCESS;
}

int CTeam::GetOwner()
{
	bool bFlag = false;		// the flag of turn owner

	int tResChar = 0;		// owner kitted
	TeamVector::iterator it = mTeamVec.begin();
	for ( ; it != mTeamVec.end(); ++ it )
	{		
		if ( IsTurned( *it ) == false )
		{
			tResChar = *it;
			bFlag = true;
			PushTurnOwner( *it );
			break;
		}
	}
	if ( bFlag == false )
	{
		// has distributed a time
		for ( it = mTeamVec.begin(); it != mTeamVec.end(); ++ it )
		{
			if ( bFlag == false )
			{
				tResChar = *it;
				bFlag = true;
			}
			else
			{
				RemoveTrunOwner( *it );
			}
		}
	}
	return tResChar;
}

