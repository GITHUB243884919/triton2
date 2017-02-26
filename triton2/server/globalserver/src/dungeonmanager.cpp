#include "dungeonmanager.h"
#include "errcode.h"
#include "dunobjmanager.h"

// ***************************************************************
// Function:     GetRoleInfo
// Description:  
// Others:         
// Date:         03/28/2011 
//***************************************************************
CRoleInfo *CWarRegInfo::GetRoleInfo( int nRoleID )
{
	for ( int i = 0; i<ARRAY_CNT( mRoleInfo ) && i<mRoleNum; ++i )
	{
		if ( mRoleInfo[i].mRoleID == nRoleID )
		{
			return & mRoleInfo[i];
		}
	}
	return NULL;
}

// ***************************************************************
// Function:     DeleteRoleInfo
// Description:  
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CWarRegInfo::DeleteRoleInfo( int nRoleID )
{
	for ( int i = 0; i<ARRAY_CNT( mRoleInfo ) && i<mRoleNum; ++i )
	{
		if ( mRoleInfo[i].mRoleID == nRoleID )
		{			
			mRoleInfo[i].Initialize();
			if ( i < mRoleNum - 1 )
			{
				mRoleInfo[i] = mRoleInfo[mRoleNum-1];	
				mRoleInfo[mRoleNum-1].Initialize();
			}		
			--mRoleNum;
			return 0;
		}		
	}
	return -1;
}

// ***************************************************************
// Function:     InsertRole
// Description:  
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CWarRegInfo::InsertRole( CRoleInfo &rRoleInfo )
{
	if ( mRoleNum >= 0 && mRoleNum < ARRAY_CNT( mRoleInfo ) )
	{	
		if ( IsRepetionStart() == true )
		{
			rRoleInfo.mRoleStatus = CRoleInfo::ESTATUS_WAIT;
			rRoleInfo.mOffTime = time( NULL );
		}

		mRoleInfo[mRoleNum] = rRoleInfo;
		++mRoleNum;
		return 0;
	}
	return -1;
}

// ***************************************************************
// Function:     AssignTeam
// Description:  把队伍自动分成两拨,战场开始的时候进行(各自1/2 WAR_START_NUM)
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CWarRegInfo::AssignTeam( int nStartNum )
{
	if ( nStartNum <= 0 || IsRepetionStart() == false )
	{
		return 0;
	}

	int tRandom[MAX_WAR_PLAYER_NUM] = { 0 };	
	for ( int i = 0; i < mRoleNum; ++i )
	{
		tRandom[i] = i%2+1;
	}
	for ( int i = 0; i < mRoleNum; ++i )
	{		
		int tIndex = rand() % ( mRoleNum - i ) + i;							
		mRoleInfo[i].mTeamFlag = tRandom[tIndex];			
		tRandom[tIndex] = tRandom[i];		
	}
	return 0;
}

// ***************************************************************
// Function:     GetTeamNum
// Description:  获取某一分队的人数
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CWarRegInfo::GetTeamNum( ETeamFlag eFlag, int nStarNum )
{	
	int tNum = 0;
	for ( int i = 0; i < mRoleNum; ++i )
	{
		if ( mRoleInfo[i].mTeamFlag == eFlag )
		{
			++tNum;
		}
	}
	return tNum;
}

// ***************************************************************
// Function:     GetTeamFlag
// Description:  根据人数获取组队标识
// Others:         
// Date:         03/28/2011 
//***************************************************************
CWarRegInfo::ETeamFlag CWarRegInfo::GetTeamFlag( int nStartNum )
{
	int tTeamOneNum = GetTeamNum( ETEAMFLAG_ONE, nStartNum );
	int tTeamTwoNum = GetTeamNum( ETEAMFLAG_TWO, nStartNum );
	return (tTeamOneNum > tTeamTwoNum ? ETEAMFLAG_TWO : ETEAMFLAG_ONE);
}

// ***************************************************************
// Function:     HasRole
// Description:  判断注册信息中是否有该角色
// Others:         
// Date:         03/28/2011 
//***************************************************************
bool CWarRegInfo::HasRole( int nRoleID )
{
	for ( int i = 0; i < mRoleNum; ++i )
	{
		if ( mRoleInfo[i].mRoleID == nRoleID )
		{
			return true;
		}
	}
	return false;		
}

// ***************************************************************
// Function:     ChangeRoleStatus
// Description:  判断注册信息中是否有该角色
// Others:         
// Date:         04/12/2011 
//***************************************************************
int CWarRegInfo::ChangeRoleStatus( int nRoleID, int nStatus )
{
	CRoleInfo *tpRoleInfo = GetRoleInfo( nRoleID );
	if ( tpRoleInfo != NULL )
	{
		tpRoleInfo->mRoleStatus = nStatus;
		if ( nStatus == CRoleInfo::ESTATUS_OFFLINE )
		{
			tpRoleInfo->mOffTime = time( NULL );
		}
	}
	return SUCCESS;
}

// ***************************************************************
// Function:     RemoveTimeOutRole
// Description:  删除离线过长的玩家
// Others:         
// Date:         04/12/2011 
//***************************************************************
void CWarRegInfo::RemoveTimeOutRole( int nMaxOfflineTime, int nMaxWaitTime,std::vector<int> &rRoleIDList )
{
	int tTimeNow = time(NULL);
	for ( int i = 0; i < mRoleNum; ++i )
	{
		if ( mRoleInfo[i].mRoleStatus == CRoleInfo::ESTATUS_OFFLINE )
		{
			if ( tTimeNow - mRoleInfo[i].mOffTime >= nMaxOfflineTime )
			{
				rRoleIDList.push_back( mRoleInfo[i].mRoleID );
				if ( i < mRoleNum - 1 )
				{
					mRoleInfo[i] = mRoleInfo[mRoleNum-1];							
				}		
				--mRoleNum;		
				--i;				
			}		
		}
		else if ( mRoleInfo[i].mRoleStatus == CRoleInfo::ESTATUS_WAIT )
		{
			if ( tTimeNow - mRoleInfo[i].mOffTime >= nMaxWaitTime )
			{
				rRoleIDList.push_back( mRoleInfo[i].mRoleID );
				if ( i < mRoleNum - 1 )
				{
					mRoleInfo[i] = mRoleInfo[mRoleNum-1];							
				}		
				--mRoleNum;		
				--i;
			}		
		}
	}
}

// ***************************************************************
// Function:     InProtecTime
// Description:  检查是否在副本保护时间内
// Others:         
// Date:         04/12/2011 
//***************************************************************
bool CWarRegInfo::InProtecTime( int nProtectTime )
{
	if ( IsRepetionStart() && time( NULL ) - mCreateTime <= nProtectTime )
	{
		return true;
	}
	return false;
}



// ***************************************************************
// Function:     Initialize
// Description:  初始化
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CWarRegInfoManager::Initialize()
{	
	mWarStartNum = 0;	
	mLastWarStartTime = 0;
	mWaitPlayerSet.initailize();
	mWarRegInfoSet.initailize();
	mWarEnableList.initailize();
	mWarManagerStatus = EMANAGER_STATUS_UNUSED;
	mServerListID = 0;
	mRepetionIndex = 0;	
	return 0;
}

// ***************************************************************
// Function:     FindWarRegInfoWithoutFull
// Description:  获取某种类型的第一个注册信息
// Others:         
// Date:         03/28/2011 
//***************************************************************
CWarRegInfo* CWarRegInfoManager::FindWarRegInfoWithoutFull( int nLevelID )
{
	WarRegInfoList *tpWarRegInfoList = GetWarRegInfoListByLevelID( nLevelID );
	if ( tpWarRegInfoList != NULL )
	{
		WarRegInfoList::iterator itor = tpWarRegInfoList->begin();
		for ( ; itor != tpWarRegInfoList->end(); ++itor )
		{
			CWarRegInfo *tpWarRegInfo = ( CWarRegInfo * )CDunobjManager::GetSingleton().GetObject( *itor );
			if ( tpWarRegInfo == NULL )
			{
				continue;
			}
			if ( tpWarRegInfo->IsFull( mWarStartNum ) == false && tpWarRegInfo->mStatus != CWarRegInfo::EWARSTATUS_BATTLEEND )
			{
				return tpWarRegInfo;
			}
		}			
	}
	return NULL;
}

// ***************************************************************
// Function:     GetWarRegInfo
// Description:  获取某个等级段某种类型的战场信息
// Others:         
// Date:         03/28/2011 
//***************************************************************
CWarRegInfo *CWarRegInfoManager::GetWarRegInfo( EWarRegStatus eStatus, int nLevelID )
{
	WarRegInfoList *tpList = GetWarRegInfoListByLevelID( nLevelID );
	if ( tpList == NULL )
	{
		return NULL;
	}
	WarRegInfoList::iterator it = tpList->begin();
	for ( ; it != tpList->end(); ++it )
	{
		CWarRegInfo *tpWarInfo = ( CWarRegInfo * )CDunobjManager::GetSingleton().GetObject( *it );
		if ( tpWarInfo == NULL )
		{
			continue;
		}
		if ( tpWarInfo->IsFull( mWarStartNum ) )
		{
			return tpWarInfo;
		}
	}
	return NULL;
}

// ***************************************************************
// Function:     RegRoleInfo
// Description:  注册某个角色信息到战场
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CWarRegInfoManager::RegRoleInfo( CRoleInfo &rRoleInfo, int &rRegKey, int nLevelID )
{
	// 先查找有无已经开启但是人数未满的战场
	rRegKey = 0;
	CWarRegInfo *tpInfo = FindWarRegInfoWithoutFull( nLevelID );
	if ( tpInfo != NULL )
	{				
		rRoleInfo.mTeamFlag = tpInfo->GetTeamFlag( mWarStartNum );
		tpInfo->InsertRole( rRoleInfo );
		rRegKey = tpInfo->get_id();
		return SUCCESS;
	}
	
	// 查找待开启的战场
	CWaitPlayer tWaitPlayer;		
	tWaitPlayer.mGameID = rRoleInfo.mGameID;
	tWaitPlayer.mLevelID = nLevelID;
	tWaitPlayer.mRoleID = rRoleInfo.mRoleID;

	// 创建一个新的注册信息	
	return InsertWaitPlayer2List( tWaitPlayer );		
}

// ***************************************************************
// Function:     DeleteWarRegInfo
// Description:  注销掉已经结束的战场信息
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CWarRegInfoManager::DeleteWarRegInfo( int nRegKey )
{
	CWarRegInfo *tpWarRegInfo = ( CWarRegInfo * )CDunobjManager::GetSingleton().GetObject( nRegKey );
	if ( tpWarRegInfo != NULL )
	{	
		WarRegInfoList *tpWarRegInfoList = GetWarRegInfoListByLevelID( tpWarRegInfo->mLevelID );
		if ( tpWarRegInfoList != NULL )
		{
			EraseWarInfoFromList( *tpWarRegInfoList, nRegKey );
			if ( tpWarRegInfoList->empty() )
			{
				DeleteWarRegInfoListByLevelID( tpWarRegInfo->mLevelID );
			}
		}		
	}
	return SUCCESS;
}

// ***************************************************************
// Function:     CreateWarRegInfo
// Description:  创建一个战场的注册信息
// Others:         
// Date:         03/28/2011 
//***************************************************************
CWarRegInfo *CWarRegInfoManager::CreateWarRegInfo( )
{	
	CWarRegInfo *tpWarRegInfo = (CWarRegInfo *)CDunobjManager::GetSingleton().CreateObject( OBJTYPE_REGINFO );
	if ( tpWarRegInfo == NULL )
	{
		return NULL;
	}	
	tpWarRegInfo->Initialize();	
	tpWarRegInfo->mCreateTime = time( NULL );
	tpWarRegInfo->mRegKey = tpWarRegInfo->get_id();		
	return tpWarRegInfo;
}

// ***************************************************************
// Function:     GetWarRegInfoByRoleID
// Description:  获取某个角色的战场注册信息
// Others:         
// Date:         03/28/2011 
//***************************************************************
CWarRegInfo *CWarRegInfoManager::GetWarRegInfoByRoleID( int nRoleID )
{
	WarRegInfoSet::iterator it = mWarRegInfoSet.begin();
	for ( ; it != mWarRegInfoSet.end(); ++it )
	{
		WarRegInfoList &tWarRegInfoList = it->second;
		WarRegInfoList::iterator itor = tWarRegInfoList.begin();
		for ( ; itor != tWarRegInfoList.end(); ++itor )
		{
			CWarRegInfo *tpWarRegInfo = ( CWarRegInfo * )CDunobjManager::GetSingleton().GetObject( *itor );
			if ( tpWarRegInfo != NULL )
			{
				if ( tpWarRegInfo->HasRole( nRoleID ) )
				{
					return tpWarRegInfo;
				}
			}
		}		
	}
	return NULL;
}

// ***************************************************************
// Function:     GetWarRegInfoByKey
// Description:  根据副本的key获取战场注册信息
// Others:         
// Date:         03/28/2011 
//***************************************************************
CWarRegInfo *CWarRegInfoManager::GetWarRegInfoByKey( KEY_TYPE nRepetionKey )
{
	WarRegInfoSet::iterator it = mWarRegInfoSet.begin();
	for ( ; it != mWarRegInfoSet.end(); ++it )
	{
		WarRegInfoList &tWarRegInfoList = it->second;
		WarRegInfoList::iterator itor = tWarRegInfoList.begin();
		for ( ; itor != tWarRegInfoList.end(); ++itor )
		{
			CWarRegInfo *tpWarRegInfo = ( CWarRegInfo * )CDunobjManager::GetSingleton().GetObject( *itor );
			if ( tpWarRegInfo != NULL )
			{
				if ( tpWarRegInfo->mReptionKey == nRepetionKey )
				{
					return tpWarRegInfo;
				}
			}
		}		
	}
	return NULL;
}

// ***************************************************************
// Function:     GetWarRegInfoByRegKey
// Description:  根据注册的key值获取注册信息
// Others:         
// Date:         03/28/2011 
//***************************************************************
CWarRegInfo *CWarRegInfoManager::GetWarRegInfoByRegKey( int nKey )
{	
	CWarRegInfo *tpWarRegInfo = ( CWarRegInfo * )CDunobjManager::GetSingleton().GetObject( nKey );
	return tpWarRegInfo;
}

// ***************************************************************
// Function:     FetchRoleIntoWarFromWaitlist
// Description:  把角色从等待队列移动到战场
// Others:         
// Date:         04/08/2011 
//***************************************************************
int CWarRegInfoManager::FetchRoleIntoWarFromWaitlist( CRoleInfo *pRoleInfo, int *pWarRegKey, int &rNum )
{	
	int tNum = rNum;
	rNum = 0;
	WaitPlayerSet::iterator it = mWaitPlayerSet.begin();
	for ( ; it != mWaitPlayerSet.end(); ++it )
	{			

		WaitPlayer &tWaitPlayerList = it->second;		
		WaitPlayer::iterator itor = tWaitPlayerList.begin();
		for ( ; itor != tWaitPlayerList.end(); ++itor )
		{
			CWarRegInfo *tpWarRegInfo = FindWarRegInfoWithoutFull( it->first );
			if ( tpWarRegInfo == NULL )
			{
				break;
			}
			CRoleInfo tRoleInfo;
			tRoleInfo.mGameID = itor->mGameID;
			tRoleInfo.mRoleID = itor->mRoleID;
			tRoleInfo.mTeamFlag = tpWarRegInfo->GetTeamFlag( mWarStartNum );
			tpWarRegInfo->InsertRole( tRoleInfo );
			tWaitPlayerList.erase( itor-- );
			if ( rNum < tNum )
			{
				pWarRegKey[rNum] = tpWarRegInfo->get_id();
				pRoleInfo[rNum] = tRoleInfo;
				++rNum;
			}
			else
			{
				return SUCCESS;
			}
		}
	}
	return SUCCESS;
}

// ***************************************************************
// Function:     CheckWarRegInfo
// Description:  检查是否有可以开启的战场
// Others:         
// Date:         04/08/2011 
//***************************************************************
int CWarRegInfoManager::CheckWarRegInfo( int nWarTimeInterval )
{
	if ( time(NULL) - mLastWarStartTime < nWarTimeInterval )
	{
		return 0;
	}
	if ( mWarEnableList.max_size() == mWarEnableList.size() )
	{
		return mWarEnableList.size();
	}
	WaitPlayerSet::iterator it = mWaitPlayerSet.begin();
	for ( ; it != mWaitPlayerSet.end(); ++it )
	{		
		WaitPlayer &tWaitPlayerList = it->second;
		if ( tWaitPlayerList.size() < mWarStartNum )
		{
			continue;
		}

		// 可以开启一场战斗		
		CWarRegInfo *tpWarRegInfo = CreateWarRegInfo( );
		if ( tpWarRegInfo == NULL )
		{
			continue;
		}
		tpWarRegInfo->mLevelID = it->first;
		WaitPlayer::iterator itor = tWaitPlayerList.begin();
		for ( ; itor != tWaitPlayerList.end(); ++itor )
		{
			if ( tpWarRegInfo->IsFull( mWarStartNum ) )
			{
				break;
			}
			CRoleInfo tRoleInfo;
			tRoleInfo.mGameID = itor->mGameID;
			tRoleInfo.mRoleID = itor->mRoleID;
			tRoleInfo.mTeamFlag = tpWarRegInfo->GetTeamFlag( mWarStartNum );
			tpWarRegInfo->InsertRole( tRoleInfo );						
			tWaitPlayerList.erase( itor-- );		
		}				
		mWarEnableList.push_back( tpWarRegInfo->get_id() );
	}	
	return mWarEnableList.size();
}

// ***************************************************************
// Function:   CheckApply
// Description: 检查角色是否已经报名
// Others:         
// Date:         04/08/2011 
//***************************************************************
bool CWarRegInfoManager::CheckApply( int nRoleID )
{
	// 查看是否在已经开启的战场中
	CWarRegInfo *tpWarRegInfo = GetWarRegInfoByRoleID( nRoleID );
	if ( tpWarRegInfo != NULL )
	{
		return true;
	}

	// 查看是否在待开启的战场中
	WarEnableList::iterator it = mWarEnableList.begin();
	for ( ; it != mWarEnableList.end(); ++it )
	{
		tpWarRegInfo = ( CWarRegInfo * )CDunobjManager::GetSingleton().GetObject( *it );
		if ( tpWarRegInfo == NULL )
		{
			continue;
		}
		if ( tpWarRegInfo->HasRole( nRoleID ) )
		{
			return true;
		}
	}

	return InWaitList(nRoleID);
}

// ***************************************************************
// Function:   InWaitList
// Description: 是否在等待列表里面
// Others:         
// Date:         04/08/2011 
//***************************************************************
bool CWarRegInfoManager::InWaitList( int nRoleID )
{
	WaitPlayerSet::iterator it = mWaitPlayerSet.begin();
	if ( it == mWaitPlayerSet.end() )
	{
		return false;
	}
	for ( ; it != mWaitPlayerSet.end(); ++it )
	{
		if ( InWaitList( it->second, nRoleID ) == true )
		{
			return true;
		}
	}
	return false;
}

// ***************************************************************
// Function:   RemoveTimeOutRole
// Description: 删除超时玩家
// Others:         
// Date:         04/08/2011 
//***************************************************************
int CWarRegInfoManager::RemoveTimeOutRole( int nMaxOfflineTime, int nMaxWaitTime, std::vector<int> &rRoleIDList )
{
	WarRegInfoSet::iterator it = mWarRegInfoSet.begin();	
	for ( ; it != mWarRegInfoSet.end(); ++it )
	{
		WarRegInfoList &tWarRegList = it->second;
		WarRegInfoList::iterator itor = tWarRegList.begin();
		for ( ; itor != tWarRegList.end(); ++itor )
		{
			CWarRegInfo *tpWarInfo = ( CWarRegInfo * )CDunobjManager::GetSingleton().GetObject( *itor );
			if ( tpWarInfo == NULL )
			{
				continue;
			}			
			tpWarInfo->RemoveTimeOutRole( nMaxOfflineTime, nMaxWaitTime, rRoleIDList );
		}
	}
}

// ***************************************************************
// Function:   DeleteValidWarRegInfo
// Description: 删除超时的战场信息
// Others:         
// Date:         04/08/2011 
//***************************************************************
void CWarRegInfoManager::DeleteValidWarRegInfo( int nMaxWarAliveTime, int nProtectTime, std::vector<int> &rDeleteWarInfo )
{
	int tTimeNow = time( NULL );
	WarRegInfoSet::iterator it = mWarRegInfoSet.begin();
	for ( ; it != mWarRegInfoSet.end(); ++it )
	{
		WarRegInfoList &tWarRegList = it->second;
		WarRegInfoList::iterator itor = tWarRegList.begin();
		for ( ; itor != tWarRegList.end(); ++itor )
		{
			CWarRegInfo *tpWarInfo = ( CWarRegInfo * )CDunobjManager::GetSingleton().GetObject( *itor );
			if ( tpWarInfo == NULL )
			{				
				continue;
			}

			if ( tpWarInfo->InProtecTime( nProtectTime ) == true )
			{				 
				continue;
			}			

			if ( tpWarInfo->IsRepetionStart() == true && tpWarInfo->mRoleNum <= 0 )
			{
				rDeleteWarInfo.push_back( *itor );
				continue;		
			}

			if ( tTimeNow - tpWarInfo->mCreateTime >= nMaxWarAliveTime  )
			{					
				rDeleteWarInfo.push_back( *itor );				
			}					
		}
	}
}

// ***************************************************************
// Function:   DeleteValidWarRegInfo
// Description: 删除超时的战场信息,根据第一个队列的创建时间
// Others:         
// Date:         04/08/2011 
//***************************************************************
void CWarRegInfoManager::DeleteTimeOutWaitList( int nMaxTime, std::vector<int> &rRoleIDList )
{
	if ( mWaitPlayerSet.begin() == mWaitPlayerSet.end() )
	{
		return;
	}
	
	WaitPlayerSet::iterator it = mWaitPlayerSet.begin();
	for ( ; it != mWaitPlayerSet.end(); )
	{
		WaitPlayer &tWaitPlayer = it->second;
		if ( tWaitPlayer.TimeOut( nMaxTime ) )
		{
			tWaitPlayer.GetRoleIDList( rRoleIDList );
			mWaitPlayerSet.erase( it++ );
		}
		else
		{
			++it;
		}
	}	
}

// ***************************************************************
// Function:   IsManagerFree
// Description: 判断管理器是否空闲来回收使用
// Others:         
// Date:         04/08/2011 
//***************************************************************
bool CWarRegInfoManager::IsManagerFree()
{	
	// 所有容器都空
	if ( mWaitPlayerSet.empty() == false )
	{
		return false;
	}	
	if ( mWarRegInfoSet.empty() == false )
	{
		return false;
	}	
	if ( mWarEnableList.empty() == false )
	{
		return false;
	}
	if ( GetWarManagerStatus() == EMANAGER_STATUS_UNUSED )
	{
		return false;
	}
	return true;
}

// ***************************************************************
// Function:   DeleteRoleWarRegInfo
// Description: 删除某个角色的注册信息
// Others:         
// Date:         04/08/2011 
//***************************************************************
CWarRegInfo *CWarRegInfoManager::DeleteRoleWarRegInfo( int nRoleID )
{
	CWarRegInfo *tpInfo = GetWarRegInfoByRoleID( nRoleID );
	if ( tpInfo != NULL )
	{
		tpInfo->DeleteRoleInfo( nRoleID );
	}
	else
	{
		DeleteFromWaitList( nRoleID );
	}
	return tpInfo;
}


// ***************************************************************
// Function:  DeleteFromWaitList
// Description: 从队列中删除某个玩家的报名信息
// Others:         
// Date:         03/28/2011 
//***************************************************************
bool CWarRegInfoManager::DeleteFromWaitList( int nRoleID )
{
	if ( mWaitPlayerSet.begin() == mWaitPlayerSet.end() )
	{
		return false;
	}
	WaitPlayerSet::iterator it = mWaitPlayerSet.begin();
	for ( ; it != mWaitPlayerSet.end(); ++it )
	{
		WaitPlayer &tWaitList = it->second;
		WaitPlayer::iterator itor = tWaitList.begin();
		for ( ; itor != tWaitList.end(); ++itor )
		{
			if ( itor->mRoleID == nRoleID )
			{
				tWaitList.erase( itor );
				if ( tWaitList.empty() )
				{
					mWaitPlayerSet.erase( it );
				}
				return true;
			}
		}
	}
	return false;
}

// ***************************************************************
// Function:  InsertWaitPlayer2List
// Description: 插入一个角色信息到列表中
// Others:         
// Date:         04/26/2011 
//***************************************************************
int CWarRegInfoManager::InsertWaitPlayer2List( CWaitPlayer &rPlayer )
{
	WaitPlayer *tpWaitPlayerList  = GetWaitPlayerListByLevelID( rPlayer.mLevelID );
	if ( tpWaitPlayerList == NULL )
	{
		WaitPlayer tList;
		tList.initailize();
		tList.push_back( rPlayer );
		mWaitPlayerSet.insert( WaitPlayerSet::value_type( rPlayer.mLevelID, tList ) );
	}
	else
	{
		if ( tpWaitPlayerList->size() == tpWaitPlayerList->max_size() )
		{
			return EDUNGEON_ERROR_MAXAPPLYNUM;
		}
		tpWaitPlayerList->push_back( rPlayer );
	}
	return SUCCESS;
}

// ***************************************************************
// Function:  InsertWarRegInfo2List
// Description: 插入一个角色信息到列表中
// Others:         
// Date:         04/26/2011 
//***************************************************************
int CWarRegInfoManager::InsertWarRegInfo2List( CWarRegInfo *pWarRegInfo )
{
	if ( pWarRegInfo == NULL )
	{
		return -1;
	}
	WarRegInfoList *tpWarRegInfoList = GetWarRegInfoListByLevelID( pWarRegInfo->mLevelID );
	if ( tpWarRegInfoList == NULL )
	{
		WarRegInfoList tWarRegInfoList;
		tWarRegInfoList.initailize();
		tWarRegInfoList.push_back( pWarRegInfo->get_id() );
		mWarRegInfoSet.insert( WarRegInfoSet::value_type( pWarRegInfo->mLevelID, tWarRegInfoList ) );
	}
	else
	{
		tpWarRegInfoList->push_back( pWarRegInfo->get_id() );
	}
	return SUCCESS;
}

// ***************************************************************
// Function:  GetWarNumOfCertainLevel
// Description: 获取某个等级段开启的战场数目
// Others:         
// Date:         04/07/2011 
//***************************************************************
int CWarRegInfoManager::GetWarNumOfCertainLevel( int nLevelID )
{
	WarRegInfoList *tpWarRegInfoList = GetWarRegInfoListByLevelID( nLevelID );
	if ( tpWarRegInfoList != NULL )
	{
		return tpWarRegInfoList->size();
	}
	return 0;
}

// ***************************************************************
// Function:  GetWarRegInfoListByLevelID
// Description: 获取某个等级段的战场信息队列
// Others:         
// Date:         04/07/2011 
//***************************************************************
CWarRegInfoManager::WarRegInfoList* CWarRegInfoManager::GetWarRegInfoListByLevelID( int nLevelID )
{
	WarRegInfoSet::iterator it = mWarRegInfoSet.find( nLevelID );
	if ( it != mWarRegInfoSet.end() )
	{
		return &(it->second);
	}
	return NULL;
}

// ***************************************************************
// Function:  GetWaitPlayerListByLevelID
// Description: 获取某个等级段的等待队列信息
// Others:         
// Date:         04/07/2011 
//***************************************************************
CWarRegInfoManager::WaitPlayer* CWarRegInfoManager::GetWaitPlayerListByLevelID( int nLevelID )			
{
	WaitPlayerSet::iterator it = mWaitPlayerSet.find( nLevelID );
	if ( it != mWaitPlayerSet.end() )
	{
		return &(it->second);
	}
	return NULL;
}

// ***************************************************************
// Function:  DeleteWarRegInfoListByLevelID
// Description: 根据等级ID删除一个战场队列
// Others:         
// Date:         04/07/2011 
//***************************************************************
int CWarRegInfoManager::DeleteWarRegInfoListByLevelID( int nLevelID )				
{
	WarRegInfoSet::iterator it = mWarRegInfoSet.find( nLevelID );
	if ( it != mWarRegInfoSet.end() )
	{
		mWarRegInfoSet.erase( it );
	}	 
	return SUCCESS;	
}

// ***************************************************************
// Function:  DeleteWaitPlayerListByLevelID
// Description: 根据等级ID删除一个等待队列
// Others:         
// Date:         04/07/2011 
//***************************************************************
int CWarRegInfoManager::DeleteWaitPlayerListByLevelID( int nLevelID )	
{
	WaitPlayerSet::iterator it = mWaitPlayerSet.find( nLevelID );
	if ( it != mWaitPlayerSet.end() )
	{
		mWaitPlayerSet.erase( it );
	}
	return SUCCESS;
}

// ***************************************************************
// Function:  EraseWarInfoFromList
// Description: 清除某个战场的注册信息
// Others:         
// Date:         04/07/2011 
//***************************************************************
int CWarRegInfoManager::EraseWarInfoFromList( WarRegInfoList &rList, int nWarRegInfoID )
{
	WarRegInfoList::iterator it = rList.begin();
	for ( ; it != rList.end(); ++it )
	{
		if ( *it == nWarRegInfoID )
		{
			rList.erase( it );
			break;
		}
	}
	return SUCCESS;
}

bool CWarRegInfoManager::InWaitList( WaitPlayer &rList, int nRoleID )
{
	WaitPlayer::iterator it = rList.begin();
	for ( ; it != rList.end(); ++it )
	{
		if ( it->mRoleID == nRoleID )
		{
			return true;
		}
	}
	return false;
}

// ***************************************************************
// Function:   InsertGlobalServerInfo
// Description: 
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CGlobalServerConfig::InsertGlobalServerInfo( int nServerListID, int nKey, CGlobalServerInfo &rServerInfo )
{
	if ( nServerListID < 0 || nServerListID >= ARRAY_CNT(mGlobalServerInfoList) )
	{
		return -1;
	}
	if ( mGlobalServerInfoList[nServerListID].size() == mGlobalServerInfoList[nKey].max_size() )
	{
		return -1;
	}
	mGlobalServerInfoList[nServerListID].insert( GlobalServerInfolist::value_type( nKey, rServerInfo ) );
	return 0;
}

// ***************************************************************
// Function:   Initialize
// Description:
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CGlobalServerConfig::Initialize()
{
	for ( int i = 0; i < ARRAY_CNT(mGlobalServerInfoList); ++i )
	{
		mGlobalServerInfoList[i].initailize();
	}
	mGameConfigList.initailize();
	memset( mWarStartNum, 0, sizeof( mWarStartNum ) );
	return 0;
}

// ***************************************************************
// Function:   GetGlobalServerInfo
// Description:
// Others:         
// Date:         03/28/2011 
//***************************************************************
CGlobalServerInfo *CGlobalServerConfig::GetGlobalServerInfo( int nKey, int &rServerListID )
{
	for ( int i = 0; i < ARRAY_CNT(mGlobalServerInfoList); ++i  )
	{
		GlobalServerInfolist::iterator it = mGlobalServerInfoList[i].find( nKey );
		if ( it != mGlobalServerInfoList[i].end() )
		{
			rServerListID = i;
			return &it->second;
		}	
	}	
	return NULL;	
}

// ***************************************************************
// Function:   GetGlobalServerInfo
// Description:
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CGlobalServerConfig::InsertGameConfig( int nWorldID, CGameConfig &rConfig )
{
	if ( mGameConfigList.size() == mGameConfigList.max_size() )
	{
		return -1;
	}
	mGameConfigList.insert( GameConfigList::value_type( nWorldID, rConfig ) );
	return SUCCESS;
}

// ***************************************************************
// Function:   GetGlobalServerInfo
// Description:
// Others:         
// Date:         03/28/2011 
//***************************************************************
CGameConfig *CGlobalServerConfig::GetGameConfig( int nWorldID )
{
	GameConfigList::iterator it = mGameConfigList.find( nWorldID );
	if ( it != mGameConfigList.end() )
	{
		return &it->second;
	}
	return NULL;
}

// ***************************************************************
// Function:   SetWarStartNum
// Description:
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CGlobalServerConfig::SetWarStartNum( int nServerListID, int nNum )
{
	if ( nServerListID < 0 || nServerListID >= ARRAY_CNT( mWarStartNum ) )
	{
		return -1;
	}
	mWarStartNum[nServerListID] = nNum;
	return SUCCESS;
}
// ***************************************************************
// Function:   GetWarStartNum
// Description:
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CGlobalServerConfig::GetWarStartNum( int nServerListID )
{
	if ( nServerListID < 0 || nServerListID >= ARRAY_CNT( mWarStartNum ) )
	{
		return -1;
	}
	return mWarStartNum[nServerListID];
}

// ***************************************************************
// Function:   InitializeServerList
// Description:
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CGlobalServerConfig::InitializeServerList()
{
	for ( int i = 0; i < ARRAY_CNT(mGlobalServerInfoList); ++i )
	{
		mGlobalServerInfoList[i].initailize();
	}
	return SUCCESS;
}

// ***************************************************************
// Function:   InitializeGameConfig
// Description:
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CGlobalServerConfig::InitializeGameConfig()
{
	mGameConfigList.initailize();
	return SUCCESS;
}

// ***************************************************************
// Function:   SelectServerWithMinNum
// Description:
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CServerManager::SelectServerWithMinNum( int nServerListID, EServerType eServerType, ESlectType eType, CGlobalServerConfig &rConfig, int nRepetionIndex /* = 0 */ )
{	
	int tNum = 0;
	int tKey = 0;
	if ( nServerListID < 0 || nServerListID > ARRAY_CNT( rConfig.mGlobalServerInfoList ) )
	{
		return -1;
	}
	CGlobalServerConfig::GlobalServerInfolist::iterator it = rConfig.mGlobalServerInfoList[nServerListID].begin();
	for ( ; it != rConfig.mGlobalServerInfoList[nServerListID].end(); ++it )
	{
		if ( (it->second).mServerType != eServerType )
		{
			continue;
		}

		if ( nRepetionIndex > 0 && (it->second).mReptionIndex != nRepetionIndex )
		{
			continue;
		}

		ServerStatusList::iterator iter = mServerStatusList.find( it->first );
		if ( iter == mServerStatusList.end() )
		{
			return it->first;
		}

		if ( eType == ESELECT_TYPE_BYWARNUM )
		{
			if ( tNum < (iter->second).mWarNum || tNum <= 0 )
			{
				tNum = (iter->second).mWarNum;
				tKey = it->first;
			}
		}
		if ( eType == ESELECT_TYPE_BYROLENUM )
		{
			if ( tNum < (iter->second).mRoleNum || tNum <= 0 )
			{
				tNum = (iter->second).mRoleNum;
				tKey = it->first;
			}
		}
	}
	return tKey;
}

// ***************************************************************
// Function:   ChangeServerSatus
// Description:
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CServerManager::ChangeServerSatus( int nServerListID, int &rKey, int nRoleNum, int nWarNum, CGlobalServerConfig &rConfig, ESlectType eSelectType /* = ESELECT_TYPE_BYWARNUM */, EServerType eServerType /* = ESERVERTYPE_DUNGEON */ )
{			
	if ( rKey <= 0 )
	{
		// 选取一个负载小的服务器
		rKey = SelectServerWithMinNum( nServerListID, eServerType, eSelectType, rConfig );
		if ( rKey <= 0 )
		{
			return -1;
		}
	}
	ServerStatusList::iterator it = mServerStatusList.find( rKey );
	if ( it != mServerStatusList.end() )
	{
		(it->second).mRoleNum += nRoleNum;
		(it->second).mWarNum  += nWarNum;
	}
	else
	{
		CServerStatus tStatus;
		tStatus.mRoleNum = nRoleNum;
		tStatus.mWarNum = nWarNum;
		mServerStatusList.insert( ServerStatusList::value_type( rKey, tStatus ) );
	}
	return 0;
}

// ***************************************************************
// Function:   GetServerStatus
// Description:
// Others:         
// Date:         03/28/2011 
//***************************************************************
CServerStatus *CServerManager::GetServerStatus( int nKey )
{
	ServerStatusList::iterator it = mServerStatusList.find( nKey );
	if ( it != mServerStatusList.end() )
	{
		return &(it->second);		
	}
	return NULL;
}

// ***************************************************************
// Function:   Initialize
// Description:
// Others:         
// Date:         03/28/2011 
//***************************************************************
int CServerManager::Initialize()
{
	mServerStatusList.initailize();
	return SUCCESS;
}

// ***************************************************************
// Function:   Initialize
// Description:
// Others:         
// Date:         04/07/2011 
//***************************************************************
int CLevelConfig::Initialize()
{
	mMaxLevel = 0;
	memset( mLevelSectionList, 0, sizeof( mLevelSectionList ) );
	return 0;
}

// ***************************************************************
// Function:   Initialize
// Description:
// Others:         
// Date:         04/07/2011 
//***************************************************************
int CLevelConfig::InsertLevelSection( CLevelSection &rLevelSection )
{
	int tMaxIndex = 0;
	while ( mLevelSectionList[tMaxIndex].mMinLevel > 0 && tMaxIndex < ARRAY_CNT( mLevelSectionList ) )
	{
		++tMaxIndex;
	}	
	if ( tMaxIndex >= ARRAY_CNT( mLevelSectionList ) )
	{
		return SUCCESS;
	}
	if ( tMaxIndex == 0 )
	{
		mLevelSectionList[tMaxIndex] = rLevelSection;
		return SUCCESS;
	}

	for ( int i = tMaxIndex; i > 0; i-- )
	{
		if ( rLevelSection.mMinLevel == mLevelSectionList[i-1].mMinLevel )
		{
			return SUCCESS;
		}

		if ( rLevelSection.mMinLevel > mLevelSectionList[i-1].mMinLevel )
		{
			mLevelSectionList[i] = rLevelSection;
			break;
		}
		mLevelSectionList[i] = mLevelSectionList[i-1];
	}
	return SUCCESS;
}

// ***************************************************************
// Function:   GetLevelSectionByID
// Description:
// Others:         
// Date:         04/07/2011 
//***************************************************************
int CLevelConfig::GetIDByLevel( int nLevel )
{
	if ( nLevel >= mMaxLevel )
	{
		return 0;
	}
	int tIndex = 0;
	for ( ; tIndex < ARRAY_CNT(mLevelSectionList) && mLevelSectionList[tIndex].mMinLevel > 0; ++tIndex )
	{
		if ( nLevel < mLevelSectionList[tIndex].mMinLevel )
		{
			break;
		}
	}

	if ( tIndex == 0 )
	{
		return 0;
	}

	return mLevelSectionList[tIndex-1].mLevelID;
}

// ***************************************************************
// Function:   GetLeveSection
// Description:
// Others:         
// Date:         04/08/2011 
//***************************************************************
CLevelSection *CLevelConfig::GetLeveSection( int nLevelID )
{
	for ( int i = 0; i < ARRAY_CNT( mLevelSectionList ) && mLevelSectionList[i].mMinLevel > 0 ; ++i )
	{
		if ( mLevelSectionList[i].mLevelID == nLevelID )
		{
			return &mLevelSectionList[i];
		}
	}
	return NULL;
}

// ***************************************************************
// Function:   GetLevelRange
// Description:	获取等级范围
// Others:         
// Date:         04/08/2011 
//***************************************************************
int CLevelConfig::GetLevelRange( int nLevelID, int &rMinLevel, int &rMaxLevel )
{
	rMinLevel = 0;
	rMaxLevel = 0;
	for ( int i = 0; i < ARRAY_CNT( mLevelSectionList ) && mLevelSectionList[i].mMinLevel > 0 ; ++i )
	{
		if ( mLevelSectionList[i].mLevelID == nLevelID )
		{
			rMinLevel = mLevelSectionList[i].mMinLevel;
			if ( i + 1 < ARRAY_CNT( mLevelSectionList ) )
			{
				rMaxLevel=  mLevelSectionList[i+1].mMinLevel-1;
			}			
			break;
		}
	}
	if ( rMinLevel > 0 && rMaxLevel <= 0 )
	{
		rMaxLevel = mMaxLevel;
	}
	return SUCCESS;
}