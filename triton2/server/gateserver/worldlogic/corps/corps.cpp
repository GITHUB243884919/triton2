#include "corpstype.h"
#include "corps.h"
#include "family_manager.h"
#include "corpsinfo_pb.hxx.pb.h"
#include "corpsmanager.h"
#include "errcode.h"
#include "world.h"

#include "bidmgr.h"
#include "battlemgr.h"

// ***********************************************************
//  Function:		Initialize
//  Description:	初始化函数
//  Output:			
//  Date:	06/10/2009
// **********************************************************
int CCorps::Initialize()
{ 
	m_tFamilyList.initailize();
	m_tApplyList.initailize();
	m_nRooterID = 0;
	memset( m_sRooterName, 0, sizeof( m_sRooterName ) );
	m_nCorpsID = 0;
	memset( m_sCorpsName, 0, sizeof( m_sCorpsName )	);
	memset( m_tOfficers, 0, sizeof( m_tOfficers ) );
	m_nCorpsLevel = 1;
	m_nCorpsStatus = EM_CORPS_STATUS_ALIVE;
	m_nCorpsDisbandTime	= 0;
	m_nSaveStatus = CORPS_INFONOCHANGE;
	m_nCorpsMoney = 0;
	
	m_nCorpsGlory = 0;									
	m_nPVPWinNum = 0 ;									
	m_nPVPFailedNum = 0;								
	m_nPVPScore = 0;									
	m_nPVPStarLevel = 1;								
	m_nBidNpcID = 0;
	m_nBidMoney = 0;
	m_nVictoryTimes = 0;
	m_nLoserTimes = 0;
	m_bCanBattle = false;
	m_nCampID = 0;
	m_nBattleCity = 0;
	m_BattleKey = 0;
	
	m_WarRank = 0;
	m_WarStatus = CORPS_WAR_FREE;
	m_EnemyCorpsID = 0;
	m_BattleList.initailize();

	memset( m_RoomName, 0, sizeof( m_RoomName ) );
	m_RoomMoney = 0;
	m_RoomState = ROOM_STATE_NONE;
	m_ChallegeList.initailize();
	m_SendChallegeList.initailize();
	m_InviteIdle = true;
	m_nCorpsWelfareGetTime = -1;
	m_nForbidTime = 0;
	// 初始化官职系统
	InitialOrgnization( ); 
	m_BOSSRefineValue = 0;
	m_CorpsBossList.initailize();
	
	mCorpsPublishTask.Initailize();
	m_Color = 0;
	return 0;
}
// ***********************************************************
//  Function:		InsertFamily
//  Description:	插入一个家族
//  Output:	
//  Date:	06/10/2009
// **********************************************************
int CCorps::InsertFamily( int nFamilyID, CCorpsFamiyInfo &sFamilyInfo )
{	
	if ( CheckHasFamily( nFamilyID ) )
	{
		return FAMILYHASEXIST;
	}
	
	int  tNumberLimit = 0;
	CCorpsManager::GetSingleton().GetCorpsConfig()->GetFamilyNumLimitByLevel( m_nCorpsLevel, tNumberLimit );
	if ( (int)m_tFamilyList.size() >= tNumberLimit || (int)m_tFamilyList.size() >= MAX_CORPS_FAMILYNUM )
	{
		return FAMILYLISTFULL;
	}
	m_tFamilyList.insert( FamilyList::value_type( nFamilyID, sFamilyInfo ) );
	SetSaveStatus( CORPS_ALLINFOCHANGED );
	return 0;
}

// ***********************************************************
//  Function:		GetCorpsWelfare
//  Description:	获取军团的npc金钱和荣耀
//  Output:			
//  Date:	09/15/2009
// **********************************************************
int CCorps::GetCorpsWelfare( int nNpcID, int &rNpcMoney, int &rNpcGlory, bool bRefresh /* = true */ )
{			
	int tRet = GetNpcWelfareTime( nNpcID, m_nCorpsWelfareGetTime );	
	if ( tRet != SUCCESS )
	{
		return -1;
	}
	
	int tTimeNow = time( NULL );
	if ( m_nCorpsWelfareGetTime == -1 || m_nCorpsWelfareGetTime == 0 )
	{
		m_nCorpsWelfareGetTime = tTimeNow -	 WELFARE_REFESH_TIMELENGTH;
	}

	int tTimes = ( tTimeNow - m_nCorpsWelfareGetTime )/ WELFARE_REFESH_TIMELENGTH;
	
	CNpcWelfare tNpcWelfare;
	CFamilyManager::GetSingleton().GetFamilyConfig()->GetNpcWelfare( nNpcID, tNpcWelfare );
	rNpcGlory = tTimes*tNpcWelfare.mNpcGlory;	
	if ( rNpcGlory > MAX_NPCWELFARE_GLORY )
	{
		rNpcGlory = MAX_NPCWELFARE_GLORY;
	}
	rNpcMoney =  tTimes*tNpcWelfare.mNpcMoney;		
	if ( rNpcMoney > MAX_NPCWELFARE_MONEY )
	{
		rNpcMoney = MAX_NPCWELFARE_MONEY;
	} 

	if ( bRefresh )
	{
		m_nCorpsWelfareGetTime = tTimeNow - ( tTimeNow - m_nCorpsWelfareGetTime )%WELFARE_REFESH_TIMELENGTH;//time(NULL);	
		SetNpcWelfareTime( nNpcID, m_nCorpsWelfareGetTime );
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}		
	return SUCCESS;		
}

// ***********************************************************
//  Function:		DeletFamily
//  Description:	删除一个家族
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorps::DeletFamily( int nFamilyID )
{ 
	FamilyList::iterator it = m_tFamilyList.find( nFamilyID );
	if ( it != m_tFamilyList.end() )
	{
		m_tFamilyList.erase( it );
		//TODO: 删除参战列表
		CFamily* pFamily = CFamilyManager::GetSingleton().GetFamilyByID(nFamilyID);
		if (pFamily)
		{
			int nFMemberID[MAX_FAMILYMEMBER_NUM];
			int nMemberNum;
			pFamily->GetFamilyMemberIDList(nFMemberID, nMemberNum);

			for (int i = 0; i < nMemberNum; i++)
			{
				DelBattleRole( nFMemberID[i] );
			}
		}
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	return 0;
} 
// ***********************************************************
//  Function:		CheckHasFamily
//  Description:	判断一个家族是否存在
//  Output:			
//  Date:	06/11/2009
// **********************************************************
bool CCorps::CheckHasFamily( int nFamilyID )
{
	FamilyList::iterator it = m_tFamilyList.find( nFamilyID );
	if ( it != m_tFamilyList.end() )
	{
		return true;
	}
	return false;
}

// ***********************************************************
//  Function:		ChekcFamilyApply
//  Description:	验证是否存在家族的申请信息
//  Output:			
//  Date:	06/11/2009
// **********************************************************
bool CCorps::ChekcFamilyApply( int nFamilyID )
{	
	ApplyList::iterator it = m_tApplyList.find( nFamilyID );
	if ( it != m_tApplyList.end() )
	{
		return true;
	}
	return false;
}

// ***********************************************************
//  Function:		InsertFamilyApply
//  Description:	插入一个家族的申请信息
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorps::InsertFamilyApply( int nFamilyID, int nApplyingID )
{	
	if ( ChekcFamilyApply( nFamilyID ) )
	{
		return 0;
	}
	if ( CheckApplyListIfFull() )
	{
		return APPLYLISTFULL;
	}
	m_tApplyList.insert( ApplyList::value_type( nFamilyID, nApplyingID ) );
	return 0;
}

// ***********************************************************
//  Function:		DeletFamilyApply
//  Description:	删除一个家族的申请信息
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorps::DeletFamilyApply( int nFammilyID )
{
	if ( m_tApplyList.begin() == m_tApplyList.end() )
	{
		return 1;
	}
	ApplyList::iterator it = m_tApplyList.find( nFammilyID );
	if( it != m_tApplyList.end() )
	{	
		m_tApplyList.erase( it );
		return 0;
	}	  	     
	return 1;
}  

// ***********************************************************
//  Function:		GetCorpsMemberNum
//  Description:	获取军团的成员总数
//  Output:
//  Date:	06/11/2009
// **********************************************************
int CCorps::GetCorpsMemberNum( )
{ 
	if ( m_tFamilyList.begin() == m_tFamilyList.end() )
	{
		return 0;
	}
	int tNum = 0;
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end( ); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		if ( pFamily == NULL )
		{				
			LOG_ERROR( "corps","[%s,%d] can not find family by ID:%d", __FUNCTION__,__LINE__, it->first );
			continue;
		}
		
		tNum += pFamily->GetMembers().size();		
		
		// 防止数据出错
		if ( pFamily->GetCorpsID() == 0 )
		{
			pFamily->SetCorpsID( m_nCorpsID );
			pFamily->SetSaveStatus( CFamily::FAMILY_PROPERYCHANGED );
		}		
	}
	return tNum;
} 

// ***********************************************************
//  Function:		GetCorpsMemberNum
//  Description:	获取军团的成员总数
//  Output:			
//  Date:	06/11/2009
// **********************************************************
void CCorps::GetCorpsMemberList( PBCorpsMemberList &pbCorpsMember )
{		
	for ( FamilyList::iterator it =m_tFamilyList.begin() ; it != m_tFamilyList.end();  )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		
		// 家族不存在		
		if( pFamily == NULL )
		{
			m_tFamilyList.erase( it++ );
			continue;
		}
		PBCorpsMember *tpCorpsMeber =  pbCorpsMember.add_corpsmemberlist();		
		pFamily->GetFamilyBasicInfo( *tpCorpsMeber->mutable_familyproperty() );
		pFamily->GetFamilyMemberInfoList( *tpCorpsMeber->mutable_familymemberlist(), true );
		++it;	
	}
}

// ***********************************************************
//  Function:		GetCorpsBasicInfo
//  Description:	获取军团的基本信息
//  Output:			
//  Date:	06/11/2009
// **********************************************************
void CCorps::GetCorpsBasicInfo( PBCorpsBasicInfo &pbBasicInfo )
{	
	pbBasicInfo.set_corpsfamilynum( GetFamilyNum() );
	pbBasicInfo.set_corpsid( m_nCorpsID );
	pbBasicInfo.set_corpslevel( m_nCorpsLevel );
	pbBasicInfo.set_corpsmembernum( GetCorpsMemberNum() );
	pbBasicInfo.set_corpsmoney( m_nCorpsMoney );
	pbBasicInfo.set_corpsname( m_sCorpsName );
	pbBasicInfo.set_corpsrootername( m_sRooterName );
	pbBasicInfo.set_corpsrooterid( m_nRooterID );	
	pbBasicInfo.set_corpsglory( m_nCorpsGlory );
	pbBasicInfo.set_corpsstarlevel( m_nPVPStarLevel );
	int tMaxFamilyNum = 0;
	CCorpsManager::GetSingleton().GetCorpsConfig()->GetFamilyNumLimitByLevel( m_nCorpsLevel, tMaxFamilyNum );
	pbBasicInfo.set_maxfamilynum( tMaxFamilyNum );		
	pbBasicInfo.set_maincitymap( GetBattleCity() );
}

// ***********************************************************
//  Function:		GetApplyingRoleID
//  Description:	获取申请者
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorps::GetApplyingRoleID( int nFamilyID )
{  
	ApplyList::iterator it = m_tApplyList.find( nFamilyID );
	if ( it != m_tApplyList.end() )
	{
		return it->second;
	}
	return 0;
}

// ***********************************************************
//  Function:		GetApplyList
//  Description:	获取军团的申请列表
//  Output:			
//  Date:	06/13/2009
// **********************************************************
int CCorps::GetApplyList( PBCorpsApplyList &pbCorpsApply )
{ 
	for ( ApplyList::iterator it = m_tApplyList.begin(); it != m_tApplyList.end();  )
	{  
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );		
		
		// 家族已经被解散
		if ( pFamily == NULL )
		{
			m_tApplyList.erase( it++ );
			continue;
		}	
		
		// 获取家族的基本信息	
		pFamily->GetFamilyBasicInfo( *pbCorpsApply.add_applylist( ) );
		++it;
	}
	return 0;
}

// ***********************************************************
//  Function:		CreatCorpsFamilyPBInfo
//  Description:	创建军团的家族信息的PB信息
//  Output:			
//  Date:	06/13/2009
// **********************************************************
int CCorps::CreatCorpsFamilyPBInfo( PBCorpsFamilyInfoList &pbCorpsFamilyInfo )
{  
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++ it )
	{  
		PBCorpsFamilyInfo *tCorpsFamiyInfo = pbCorpsFamilyInfo.add_familyinfo();
		tCorpsFamiyInfo->set_familyid( it->first );
		tCorpsFamiyInfo->set_contribute( (it->second).mContribute );
	}
	return 0;
}


// ***********************************************************
//  Function:		CreateCorpsPublishDB
//  Description:	创建军团发布信息的PB信息
//  Output:			
//  Date:	10/25/2011
// **********************************************************
int CCorps::CreateCorpsPublishDB(PBCorpsPublishInfo &pbPublishInfo)
{
	pbPublishInfo.set_todaytimes(mCorpsPublishTask.GetTodayPublishTimes());
	pbPublishInfo.set_weektimes(mCorpsPublishTask.GetWeekPublishTimes());
	for ( PUBLISHLIST::iterator it = mCorpsPublishTask.mPublishList.begin(); it != mCorpsPublishTask.mPublishList.end(); ++ it )
	{  
		PBTaskListInfo *tCorpsTaskInfo = pbPublishInfo.add_tasklist();
		tCorpsTaskInfo->set_taskid( it->mTaskID );
		tCorpsTaskInfo->set_taskstate( it->mTaskState );
		tCorpsTaskInfo->set_entityid( it->mEntityID );
	}
	return 0;
}

// ***********************************************************
//  Function:		GetCorpsPublishInfoFromPB
//  Description:	从PB数据创建军团的发布信息
//  Output:			
//  Date:	10/25/2011
// **********************************************************
int CCorps::GetCorpsPublishInfoFromPB( PBCorpsPublishInfo &pbPublishInfo )
{	
	mCorpsPublishTask.SetTodayPublishTimes(pbPublishInfo.todaytimes());
	mCorpsPublishTask.SetWeekPublishTimes(pbPublishInfo.weektimes());
	for ( int i = 0; i < pbPublishInfo.tasklist_size(); ++i )
	{	  
		unsigned short vTaskID = pbPublishInfo.mutable_tasklist( i )->taskid();
		unsigned short vTaskState = pbPublishInfo.mutable_tasklist( i )->taskstate();
		unsigned int vEntityID = pbPublishInfo.mutable_tasklist( i )->entityid();
		st_group_publish_task _newTask(vTaskID, vTaskState, vEntityID);
		mCorpsPublishTask.mPublishList.push_back(_newTask);
	}
	return 0;
}
// ***********************************************************
//  Function:		GetCorpsFamilyInfoFromPB
//  Description:	从PB数据创建军团的家族信息
//  Output:			
//  Date:	06/13/2009
// **********************************************************
int CCorps::GetCorpsFamilyInfoFromPB( PBCorpsFamilyInfoList &pbCorpsFamilyInfo )
{	
	for ( int i = 0; i < pbCorpsFamilyInfo.familyinfo_size(); ++i )
	{	  
		int tFamilyID = pbCorpsFamilyInfo.mutable_familyinfo( i )->familyid();
		CCorpsFamiyInfo tFamilyInfo;
		tFamilyInfo.mContribute =   pbCorpsFamilyInfo.mutable_familyinfo( i )->contribute();
		m_tFamilyList.insert( FamilyList::value_type( tFamilyID, tFamilyInfo ) );
	}
	return 0;
}

// ***********************************************************
//  Function:		ClearCorpsInfoOfFamily
//  Description:	清除家族的军团信息
//  Output:			
//  Date:	06/13/2009
// **********************************************************
int CCorps::ClearCorpsInfoOfFamily()
{ 
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		pFamily->SetCorpsID( 0 );		
		pFamily->SetLastTimeLeaveCorps( time(NULL) );
		if ( pFamily->GetRooter() == m_nRooterID )
		{
			// pFamily->SetPost( m_nRooterID, CORPS_POST_FAMILYHEADER );
		}
		pFamily->SetLastTimeLeaveCorps( time(NULL) );
	}	
	return 0;	
}

// ***********************************************************
//  Function:		CheckCorpsDisbandTime
//  Description:	检查军团的解散时间是否已到
//  Output:			
//  Date:	06/17/2009
// **********************************************************
bool CCorps::CheckCorpsDisbandTime( )
{ 
	if ( m_nCorpsStatus == EM_CORPS_STATUS_ALIVE )
	{
		return false;
	}
	if ( time(NULL) - m_nCorpsDisbandTime >= CORPS_DISBAND_TIMELENGTH )
	{
		return true;
	}
	return false;		
}

// ***********************************************************
//  Function:		GetMemberPost
//  Description:	获取玩家的军团职位
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorps::GetMemberPost( int nMemberID, int &rPost )
{
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		if ( pFamily->HasMember( nMemberID ) )
		{																	 
			pFamily->GetPost( nMemberID, CFamily::EM_CORPSACTION, rPost );				
			break;
		}	
	}			   	
	return SUCCESS;	
}

// ***********************************************************
//  Function:		LevelUPCorps
//  Description:	家族升级
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int	CCorps::LevelUPCorps( int &rLevel, int &rMoney )
{
	int tGlory = 0;
	rLevel	= m_nCorpsLevel;
	int tRet = CCorpsManager::GetSingleton( ).GetCorpsConfig( )->GetGloryLevelupNeeded( m_nCorpsLevel, tGlory );
	if ( tRet == -1 )
	{
		return ERROR_CORPS_LEVELUP_LEVELIMIT;
	} 
	
	if ( tGlory == -1 )
	{
		return ERROR_CORPS_LEVELUP_LEVELIMIT;
	}
	
	if ( m_nCorpsGlory < tGlory )
	{
		return ERROR_CORPS_LEVELUP_GLORYNOTENOUGH;	
	}
	
	rMoney = 0;
	CCorpsManager::GetSingleton().GetCorpsConfig()->GetMoneyLevelUpNeeded( m_nCorpsLevel, rMoney );
	
	if ( rMoney > m_nCorpsMoney )
	{
		return ERROR_CORPS_LEVELUP_MONEYNOTENOUGH;
	}
	m_nCorpsMoney -= rMoney;
	
	++m_nCorpsLevel;
	rLevel = m_nCorpsLevel;
	SetSaveStatus( CORPS_ALLINFOCHANGED );
	return SUCCESS;
}

// ***********************************************************
//  Function:		SetMemberPost
//  Description:	职位设置
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorps::SetMemberPost( int nRoleID, int nPost, int &rOldPost )
{ 
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		if ( pFamily->HasMember( nRoleID ) )
		{				
			pFamily->SetCorpsPost( nRoleID, nPost, rOldPost );
			return SUCCESS;
		}						
	}		
	return -1;	
}

// ***********************************************************
//  Function:		AddCorpsGlory
//  Description:	增加军团荣耀
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorps::AddCorpsGlory( int nValue, int nType )
{  	
	int tGlory = 0;
	int tRet = CCorpsManager::GetSingleton().GetCorpsConfig()->GetCorpsGloryLimitByLevel( m_nCorpsLevel, tGlory );
	if ( tRet == -1 )
	{
		return SUCCESS;
	}
	
	m_nCorpsGlory = m_nCorpsGlory+nValue > tGlory ? tGlory : m_nCorpsGlory+nValue;
	SetSaveStatus( CORPS_ALLINFOCHANGED );
	return SUCCESS;		
}

// ***********************************************************
//  Function:		AddCorpsGlory
//  Description:	减少军团荣耀
//  Output:			bool 是否降级	
//  Date:	08/25/2009
// **********************************************************
bool CCorps::ReduceCorpsGlory( int nValue, int nType )
{	
	m_nCorpsGlory = m_nCorpsGlory - nValue >= 0 ? m_nCorpsGlory - nValue : 0;
	SetSaveStatus( CORPS_ALLINFOCHANGED );
	if ( m_nCorpsLevel == 1 )
	{						
		return false;
	}
	
	// 判读是否需要降级
	int tGlory = 0;
	int tRet = CCorpsManager::GetSingleton().GetCorpsConfig()->GetGloryLevelupNeeded( m_nCorpsLevel - 1, tGlory );
	if ( tRet == -1 )
	{
		return false;
	}
	
	if ( m_nCorpsGlory < tGlory )
	{
		--m_nCorpsLevel;
		return true;
	}		
	return false;
} 

// ***********************************************************
//  Function:		AddCorpsGlory
//  Description:	捐献军团金钱
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorps::ContributeCorpsMoney( int nMoney, int nType /* = 0 */ )
{		
	if ( nMoney < 0 )
	{
		return -1;
	}
	
	if ( (unsigned int)( m_nCorpsMoney + nMoney ) > (unsigned int)MAX_FAMILYCORPS_MONEY )
	{	 
		return ERROR_CORPS_MONEYLIMIT;		
	}	
	
	m_nCorpsMoney += nMoney;
		
	// 判断类型如果家族捐钱的话，增加家族的军团贡献度	
	switch( nType )
	{
		case 0:
		{
			break;
		}
	}
	SetSaveStatus( CORPS_ALLINFOCHANGED );
	return SUCCESS;					
}

// ***********************************************************
//  Function:		ConsumeCorpsMoney
//  Description:	消耗军团金钱
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorps::ConsumeCorpsMoney( int nMoney, int nType )
{		
	if ( nMoney <= 0 )
	{
		return ERROR_CORPS_MONEYINVALIDE;
	}
	
	if ( nMoney > m_nCorpsMoney )
	{
		return	EM_MONEYNOTENOUGH;
	}
	m_nCorpsMoney = m_nCorpsMoney - nMoney > 0 ? m_nCorpsMoney - nMoney : 0;	
	SetSaveStatus( CORPS_ALLINFOCHANGED );
	return SUCCESS;
}

// ***********************************************************
//  Function:		AddPvPScore
//  Description:	增加pvp积分
//  Output:			bool 星级是否改变	
//  Date:	08/25/2009
// **********************************************************
bool CCorps::AddPvPScore( int nValue, int nFailedCorpsLevel )
{
	int	 tPvpScore = 0;
	int tRet = CCorpsManager::GetSingleton().GetCorpsConfig()->GetPvPStarScore( m_nPVPStarLevel, tPvpScore );
	if ( tRet == -1 )
	{
		return false;
	}

	if ( tPvpScore == -1 )
	{
		return false;
	}	

	nValue = 100 - ( m_nPVPStarLevel - nFailedCorpsLevel )*nValue; //- ( m_nPVPStarLevel - 1 )*nValue;
	if ( nValue <= 0 )
	{
		return false;
	}  	

	m_nPVPScore += nValue;		
	SetSaveStatus( CORPS_ALLINFOCHANGED );
	int tLevelUP = false;
	while( tRet != -1 && m_nPVPScore >= tPvpScore )
	{
		++m_nPVPStarLevel;
		tRet = CCorpsManager::GetSingleton().GetCorpsConfig()->GetPvPStarScore( m_nPVPStarLevel, tPvpScore );
		tLevelUP = true;
	}	
	return tLevelUP;
}

// ***********************************************************
//  Function:		ReducePvPScore
//  Description:	减少pvp积分
//  Output:			bool 星级是否改变	
//  Date:	08/25/2009
// **********************************************************
bool CCorps::ReducePvPScore( int nValue, int nWinCorpsLevel )
{ 
	nValue = 20 + ( m_nPVPStarLevel - nWinCorpsLevel )*nValue + ( m_nPVPStarLevel - 1 )*nValue;			

	if ( nValue > 0 )
	{
		m_nPVPScore = ( m_nPVPScore - nValue ) > 0 ? m_nPVPScore - nValue : 0;
	}
	SetSaveStatus( CORPS_ALLINFOCHANGED );
	
	// 如果pvp积分是一级则不会降级
	if ( m_nPVPStarLevel <= 1 )
	{
		return false;		
	}

	bool bLevelDown = false;
	for ( int i = m_nPVPStarLevel; i > 1;  --i )
	{
		int	 tPvpScore = 0;
		int tRet = CCorpsManager::GetSingleton().GetCorpsConfig()->GetPvPStarScore( i - 1, tPvpScore );
		if ( tRet == -1 )
		{
			return false;
		}

		// 不用再降级了
		if ( m_nPVPScore >= tPvpScore )
		{
			break;			
		}

		--m_nPVPStarLevel;
		bLevelDown = true;
	}
	return bLevelDown;	
}		

// ***********************************************************
//  Function:		HasPost
//  Description:	判断某个玩家是否有权限
//  Output:			bool 星级是否改变	
//  Date:	08/25/2009
// **********************************************************
bool CCorps::HasPost( int nRoleID, int nCorpsPost )
{	
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		if ( pFamily->HasMember( nRoleID ) )
		{
			return pFamily->HasPost(  nRoleID, nCorpsPost, CFamily::EM_CORPSACTION );			
		}						
	}	
	return false;		
}

// ***********************************************************
//  Function:		GetMemberListOfPost
//  Description:	获取某个职位成员列表
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorps::GetMemberListOfPost( int *pList, int& rNum, int nPost, int nType )
{
	int nLength = rNum;	
	rNum = 0;
	int tNum = 0;
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		tNum = nLength - rNum;
		pFamily->GetMemberListOfPost( pList, tNum, nPost, nType );
		rNum += tNum;	
	}	
	return SUCCESS;
} 

// ***********************************************************
//  Function:		GetPostNum
//  Description:	获取某个职位的个数
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorps::GetPostNum( int nPost )
{
	int tNum = 0;
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		tNum += pFamily->GetNumOfCertainPost( nPost, CFamily::EM_CORPSACTION );		
	}	
	return tNum;	
}

// ***********************************************************
//  Function:		GetPostLimitNum
//  Description:	获取某个职位的人数上限
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorps::GetPostLimitNum( int nPost )
{ 
	int tNum = 0;
	switch( nPost )
	{
		case CORPS_POST_CORPSHEADER:
		{
			tNum = 1;
			break;
		}
		
		case CORPS_POST_GENERAL:
		{
			CCorpsManager::GetSingleton().GetCorpsConfig()->GetGeneralNumLimitByLevel( m_nCorpsLevel, tNum );
			break;
		}
		case CORPS_POST_COUNSELLOR:
		{
			CCorpsManager::GetSingleton().GetCorpsConfig()->GetCounSellorNumLimitByLevel( m_nCorpsLevel, tNum );
			break;	
		}
		case CORPS_POST_FAMILYHEADER:
		{
			CCorpsManager::GetSingleton().GetCorpsConfig()->GetFamilyNumLimitByLevel( m_nCorpsLevel, tNum );
			break;
		}
		case CORPS_POST_MEMBER:
		{	
			tNum = 500;			
			break;
		}
		default:
		break;
	}	
	return tNum;
}

// ***********************************************************
//  Function:		AddCorpsMemberContribute
//  Description:	增加军团成员的贡献度
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorps::AddCorpsMemberContribute( int nMemberID, int nContribute )
{	
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		if ( pFamily->HasMember( nMemberID ) )
		{
			pFamily->AddMemberContribute( nMemberID, nContribute, CFamily::EM_CORPSACTION );			
			break;
		}					
	}	
	return SUCCESS;		
}

// ***********************************************************
//  Function:		SetCorpsMemberName
//  Description:	增加军团成员的贡献度
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorps::SetCorpsMemberName( int nMemberID, const char *pName )
{	
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		if ( pFamily->HasMember( nMemberID ) )
		{
			pFamily->SetMemberName( nMemberID, pName );
			break;
		}					
	}
	return SUCCESS;
}

// ***********************************************************
//  Function:		SetCorpsContribute
//  Description:	设置玩家的贡献度
//  Output:			
//  Date:	01/15/2010
// **********************************************************
int CCorps::SetCorpsContribute( int nMemberID, int nContribute )
{
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		if ( pFamily->HasMember( nMemberID ) )
		{
			pFamily->SetMemberContirubte( nMemberID, nContribute, CFamily::EM_CORPSACTION );
			break;
		}					
	}
	return SUCCESS;
}

// ***********************************************************
//  Function:		SetCorpsMemberName
//  Description:	检查是否有某个成员
//  Output:			
//  Date:	09/16/2009
// **********************************************************
bool CCorps::HasMember( int nRoleID )
{
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		if ( pFamily->HasMember( nRoleID ) )
		{
			return true;			
		}					
	}
	return false;	
}

// ***********************************************************
//  Function:		GetMemberContribute
//  Description:	获取个人的军团贡献度
//  Output:			
//  Date:	09/18/2009
// **********************************************************
int CCorps::GetMemberContribute( int nRoleID )
{
	int tContribute = 0;
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		if ( pFamily->HasMember( nRoleID ) )
		{
			tContribute = pFamily->GetMemberContribute( nRoleID, CFamily::EM_CORPSACTION );
			break;
		}					
	}
	return tContribute;		
}

// ***********************************************************
//  Function:		GetMemberExp
//  Description:	获取家族成员的经验
//  Output:			
//  Date:	09/18/2009
// **********************************************************
int	CCorps::GetMemberExp( int nRoleID )
{	
	int tExp = 0;
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		if ( pFamily->HasMember( nRoleID ) )
		{
			tExp = pFamily->GetMemberExp( nRoleID );
			break;
		}					
	}	
	return tExp;
}

// ***********************************************************
//  Function:		AddMemberExp
//  Description:	增加成员的经验
//  Output:			
//  Date:	09/18/2009
// ********************************************************** 
void CCorps::AddMemberExp( int nRoleID, int nExp )
{
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return )
		if ( pFamily->HasMember( nRoleID ) )
		{
			pFamily->AddMemberExp( nRoleID, nExp );
			break;
		}					
	}		
} 

// ***********************************************************
//  Function:		SetMemberExp
//  Description:	设置成员的经验
//  Output:			
//  Date:	09/18/2009
// ********************************************************** 
void CCorps::SetMemberExp( int nRoleID, int nExp )
{ 
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return )
		if ( pFamily->HasMember( nRoleID ) )
		{
			pFamily->SetMemberExp( nRoleID, nExp );
			break;
		}					
	}			
}

// ***********************************************************
//  Function:		GetMemberAP
//  Description:	获取成员AP值
//  Output:			
//  Date:	09/18/2009
// ********************************************************** 
int CCorps::GetMemberAP( int nRoleID )
{ 
	int tAP = 0;
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return -1 )
		if ( pFamily->HasMember( nRoleID ) )
		{
			tAP = pFamily->GetMemberAP( nRoleID );
			break;
		}					
	}	
	return tAP; 	
}

// ***********************************************************
//  Function:		SetMemberAP
//  Description:	设置成员AP值
//  Output:			
//  Date:	09/18/2009
// ********************************************************** 
void CCorps::SetMemberAP( int nRoleID, int nAP )
{
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return )
		if ( pFamily->HasMember( nRoleID ) )
		{
			pFamily->SetMemberAP( nRoleID, nAP );
			break;
		}					
	}	
}

// ***********************************************************
//  Function:		AddMemberAP
//  Description:	增加成员AP
//  Output:			
//  Date:	09/18/2009
// ********************************************************** 
void CCorps::AddMemberAP( int nRoleID, int nAP )
{
	for ( FamilyList::iterator it = m_tFamilyList.begin(); it != m_tFamilyList.end(); ++it )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, return )
		if ( pFamily->HasMember( nRoleID ) )
		{
			pFamily->AddMemberAP( nRoleID, nAP );
			break;
		}					
	}	
}

int CCorps::MakeCorpsPbPropert( CorpsProperty* vpProperty )
{
	if ( vpProperty == NULL )
	{
		return -1;
	}
	vpProperty->set_warrank( GetWarRank() );
	vpProperty->set_warwinnum( GetVictoryTimes() );
	vpProperty->set_warlostnum( GetLoserTimers() );
	vpProperty->set_familystarlevel( GetPVPStarLevel() );
	vpProperty->set_warexp( GetPVPScore() );
	vpProperty->set_color( GetColor() );

	//取出相关争夺战状态
	CBidMgr::GetSingleton().MakeCorpsBattleState(GetCorpsID(), vpProperty);
	
	return 0;
}

int CCorps::InsertBattleMember( int nRoleID )
{
	if ( m_BattleList.size() >= CORPSDISBAND_NUMBERLIMIT )
	{
		return -1;
	}
	BattleList::iterator it = m_BattleList.begin();
	for ( ; it != m_BattleList.end(); ++ it )
	{
		if ( *it == nRoleID )
		{
			m_BattleList.erase( it );
			break;
		}
	}
	m_BattleList.push_back( nRoleID );
	return 0;
}

int CCorps::ClearBattleList()
{
	m_BattleList.clear();
	return SUCCESS;
}

int CCorps::GetBattleListSize()
{
	return m_BattleList.size();
}

int CCorps::GetBattleMemberList( int* vpList, int& nSize )
{
	if ( vpList == NULL )
	{
		return -1;
	}

	int tSize = nSize;	nSize = 0;

	BattleList::iterator it = m_BattleList.begin();
	for ( ; it != m_BattleList.end() && nSize < tSize; ++ it )
	{
		vpList[ nSize ++ ] = *it;
	}
	return 0;
}

bool CCorps::IsInBattleList( int nRoleID )
{
	BattleList::iterator it = m_BattleList.begin();
	for ( ; it != m_BattleList.end(); ++ it )
	{
		if ( *it == nRoleID )
			return true;
	}
	return false;
}

int CCorps::DelBattleRole( int nRoleID )
{
	BattleList::iterator it = m_BattleList.begin();
	for ( ; it != m_BattleList.end(); ++ it )
	{
		if ( *it == nRoleID )
		{
			m_BattleList.erase( it );
			LOG_DEBUG("corps", "[corp:%s]del %d from join battle list", GetCorpsName(), nRoleID);
			break;
		}
	}
	return 0;
}


// 是否占领该地图,返回npcid [12/21/2009 Macro]
int CCorps::HasOwnThisMap( int nMapID )
{
	int tSceneNpcID = 0;

	int tGateMapID = GATENPCID(nMapID, this->GetCorpsCountryID());
	
	if (CBidMgr::GetSingleton().FindCityOwnerByMapID(tGateMapID, tSceneNpcID) == GetCorpsID())
	{
		return tSceneNpcID;
	}
	
	return 0;
}


int CCorps::ClearNpcBattleInfo( int nNpcID )
{
	NpcStatus *pState = GetNpcState(nNpcID);
	if (NULL == pState)
	{
		return -1;
	}
	if ( pState->m_OwnerID == GetCorpsID())
	{
		//删除一个地图，需要更新国家类称号
		UpdateNationTitle( SCENENPCID_2(pState->m_MapID), false );

		
		//m_BattleInfo.erase( it );
		//pState->m_OwnerID = 0;
	}
	return SUCCESS; 
}




// ***********************************************************
//  Function:		CanCorpsLevelUP
//  Description:	军团是否可以升级AP
//  Output:			
//  Date:	09/27/2009
// ********************************************************** 
bool CCorps::CanCorpsLevelUP( )
{ 
	int tGlory = 0;	
	int tRet = CCorpsManager::GetSingleton( ).GetCorpsConfig( )->GetGloryLevelupNeeded( m_nCorpsLevel, tGlory );
	if ( tRet == -1 )
	{
		return false;
	} 

	if ( tGlory == -1 )
	{
		return false;
	}

	if ( m_nCorpsGlory < tGlory )
	{
		return false;	
	}

	int tMoney = 0;
	CCorpsManager::GetSingleton().GetCorpsConfig( )->GetMoneyLevelUpNeeded( m_nCorpsLevel, tMoney );

	if ( tMoney > m_nCorpsMoney )
	{
		return false;
	}	
	
	return true;
}

// ***********************************************************
//  Function:		GetLevelCondition
//  Description:	获取升级需要的金钱和荣耀
//  Output:			
//  Date:	01/20/2010
// ********************************************************** 
int CCorps::GetLevelCondition( int &rMoney, int &rGlory )
{ 
	CCorpsManager::GetSingleton().GetCorpsConfig()->GetCorpsGloryLimitByLevel( m_nCorpsLevel, rGlory );
	CCorpsManager::GetSingleton().GetCorpsConfig()->GetMoneyLevelUpNeeded( m_nCorpsLevel, rMoney );
	return SUCCESS;
}

// ***********************************************************
//  Function:		PostHasRight
//  Description:	某个职位是否有某项权利
//  Output:			
//  Date:	10/22/2009
// ********************************************************** 
bool CCorps::PostHasRight( int nPost, int nRight )
{
	if ( nPost >= CORPS_POST_CORPSHEADER && nPost <= CORPS_POST_MEMBER )
	{
		return m_tOfficers[nPost].mLabel.HasRight( (EmRight)nRight );
	}
	return false;		
}

// ***********************************************************
//  Function:		PostHasRight
//  Description:	成员是否含有某项权利
//  Output:			
//  Date:	11/02/2009
// ********************************************************** 
bool CCorps::MemberHasRight( int nRoleID, int nRight )
{			
	for ( FamilyList::iterator it =m_tFamilyList.begin() ; it != m_tFamilyList.end() ;  )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );	
		
		// 家族不存在		
		if( pFamily == NULL )
		{
			m_tFamilyList.erase( it++ );
			continue;
		}
		if ( pFamily->GetRooter() != nRoleID )
		{
			++it;
			continue;
		}
				
		int tPost  = 0;
		pFamily->GetPost( pFamily->GetRooter(), CFamily::EM_CORPSACTION, tPost );		
		return PostHasRight( tPost, nRight );
		++it;	
	}	
	return false;
}

// ***********************************************************
//  Function:		ListMemberOfRight
//  Description:	列出有某项权的玩家列表
//  Output:			
//  Date:	10/22/2009
// ********************************************************** 
int CCorps::ListMemberOfRight( int *pIDList, int *pPostList, int *pContributeList, int &rNum, int nRight )
{
	if ( pIDList == NULL || pPostList == NULL || pContributeList == NULL )
	{						
		return -1;
	}
	int tNum = rNum;
	rNum = 0;
	
	for ( FamilyList::iterator it =m_tFamilyList.begin() ; it != m_tFamilyList.end() && rNum < tNum;  )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );

		// 家族不存在		
		if( pFamily == NULL )
		{
			m_tFamilyList.erase( it++ );
			continue;
		}
		int tPost  = 0;
		pFamily->GetPost( pFamily->GetRooter(), CFamily::EM_CORPSACTION, tPost );
		if ( HasPost( pFamily->GetRooter(), tPost ) )
		{
			pIDList[rNum] = pFamily->GetRooter();
			pPostList[rNum] = tPost;
			pContributeList[rNum] = pFamily->GetMemberContribute( pFamily->GetRooter(), CFamily::EM_CORPSACTION );			
			++rNum;
		}
		++it;	
	}	
	return SUCCESS;	
}

// ***********************************************************
//  Function:		GetMemberFamily
//  Description:	获取成员家族
//  Output:			
//  Date:	10/22/2009
// **********************************************************
CFamily *CCorps::GetMemberFamily( int nMemberID )
{ 
	for ( FamilyList::iterator it =m_tFamilyList.begin() ; it != m_tFamilyList.end(); )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );

		// 家族不存在		
		if( pFamily == NULL )
		{
			m_tFamilyList.erase( it++ );
			continue;
		}
		if ( pFamily->HasMember( nMemberID ) )
		{
			return pFamily;
		}
		++it;	
	}			
	return NULL;
}

// ***********************************************************
//  Function:		InitialOrgnization
//  Description:	初始化家族列表
//  Output:			
//  Date:	10/22/2009
// **********************************************************
int CCorps::InitialOrgnization(  )
{ 
	memset( m_tOfficers, 0, sizeof( m_tOfficers ) );
	
	// 设置军团长的职位
	int tPostRight = EM_CREATE_OFFICER | EM_GRANT_RIGHTS | EM_INVITE_MEMBER | EM_APPROVE_APPLY | EM_FIRE_MEMBER | EM_GRANT_SETOFFICER
		| EM_LIST_APPLY | EM_CHANGE_BULLETIN | EM_ACTVITY_REDSTONE | EM_TASK_CONVOY | EM_MONEY_REIN | EM_WAR_RID | EM_WAR_HALL | EM_WAR_HALL 
		| EM_WAR_SETFIGHTER | EM_WELFARE_OBTAIN | EM_WARNPC_GIVEUP | EM_RIGHT_DISBAND | EM_RIGHT_CANCELDISBAND | EM_RIGHT_CHALLENGEROOM | EM_RIGHT_BUYFAMILYCORPSITEM
		| EM_RIGHT_STARTCORPSBOSS;	
	m_tOfficers[CORPS_POST_CORPSHEADER].mLabel.SetRight( (EmRight)tPostRight ); 

	// 设置前锋将军的权限  			
	tPostRight = EM_INVITE_MEMBER | EM_APPROVE_APPLY | EM_WAR_RID | EM_WELFARE_OBTAIN | EM_FIRE_MEMBER
		| EM_FIRE_MEMBER | EM_GRANT_SETOFFICER | EM_LIST_APPLY | EM_CHANGE_BULLETIN | EM_MONEY_REIN | EM_WAR_HALL
		| EM_WAR_CHALLENGE | EM_WAR_SETFIGHTER | EM_RIGHT_CHALLENGEROOM | EM_RIGHT_STARTCORPSBOSS;	
	m_tOfficers[CORPS_POST_GENERAL].mLabel.SetRight( (EmRight)tPostRight );	

	// 设置军师的权限	
	tPostRight = EM_INVITE_MEMBER | EM_APPROVE_APPLY | EM_WAR_RID | EM_WELFARE_OBTAIN | EM_MONEY_REIN | EM_RIGHT_STARTCORPSBOSS; 
	m_tOfficers[CORPS_POST_COUNSELLOR].mLabel.SetRight( (EmRight)tPostRight );	  	

	// 设置军团家族首领的权限
	m_tOfficers[CORPS_POST_FAMILYHEADER].mLabel.SetRight( (EmRight)0 );	
	
	// 设置军团家族成员的权限
	m_tOfficers[CORPS_POST_MEMBER].mLabel.SetRight( (EmRight)0 );	
	
	return SUCCESS;		
}

// 战争信息
// 组装PB [11/29/2009 Macro]
int CCorps::GetPBBuffer( PBCorpsBattleList* npList, bool bAll /* = true */ )
{
	if ( npList == NULL )
	{
		return -1;
	}

	CorpsProperty tProperty;
	// []描述的操作耦合度高，以后可考虑改进
	// [1]遍历NPC列表，标记与军团相关的NPC
	CBidMgr::GetSingleton().MakeCorpsBattleState(GetCorpsID(), &tProperty);
	
	tProperty.Clear();
	NpcStatus* tpNpc = NULL;


	// [2]置游标到NPC列表头
	CBidMgr::GetSingleton().SetNpcStatusBegin();
	while (NULL != (tpNpc = CBidMgr::GetSingleton().GetOneNpcTmpState()))
	{
		// [3]依次读取被标记的NPC
		if (false == tpNpc->m_TmpTag)
		{
			continue;
		}
		// bAll==false时，不要没占领的地图数据
		/*if ( !bAll && tpNpc->m_OwnerID != GetCorpsID())
		{
			continue;
		}*/

		bool bExist = false;
		int nSceneMapID = SCENENPCID_2(tpNpc->m_MapID);
		for ( int i = 0; i < npList->battlelist_size(); i ++ )
		{
			int mMapID = npList->mutable_battlelist( i )->mapid();
			if ( mMapID == nSceneMapID)
			{
				bExist = true;
				break;
			}
		}
		if ( bExist )
		{
			continue;
		}

		PBCorpsBattle* tpBattle = npList->add_battlelist();
		tpBattle->set_npcid( tpNpc->m_NpcID );
		tpBattle->set_bidmoney( tpNpc->m_TmpBidMoney );
		tpBattle->set_npcstatus( tpNpc->m_TmpNpcStat );
		tpBattle->set_warstatus( tpNpc->m_TmpWarStat );
		tpBattle->set_descorpsid( tpNpc->m_TmpDesCorpID );
		tpBattle->set_mapid( nSceneMapID );
		tpBattle->set_time( 0 );
		tpBattle->set_corpsid( GetCorpsID() );
		tpBattle->set_corpsname( GetCorpsName() );
		tpBattle->set_rootername( GetRooterName() );

		if ( IS_MAIN_CITY( nSceneMapID ) )
			tpBattle->set_type( emMapMain );
		else
			tpBattle->set_type( emMapNormal );

		CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpNpc->m_TmpDesCorpID );
		if ( tpCorps != NULL )
		{
			tpBattle->set_descorpsname( tpCorps->GetCorpsName() );
		}
	}
	
	return SUCCESS;
}

// 赋值FROM PB [11/29/2009 Macro]
int CCorps::SetFromPB( PBCorpsBattleList* npList )
{
	if ( npList == NULL )
	{
		return -1;
	}

	for ( int i = 0; i < npList->battlelist_size(); i ++ )
	{
		PBCorpsBattle* tpBattle = npList->mutable_battlelist( i );
		/*ST_BATTLE_INFO tInfo;
		tInfo.mNpcID = tpBattle->npcid();
		tInfo.mDesCorpsID = tpBattle->descorpsid();
		tInfo.mNpcStatus = tpBattle->npcstatus();
		tInfo.mWarStatus = tpBattle->warstatus();
		tInfo.mBidMoney = tpBattle->bidmoney();
		tInfo.mMapID = tpBattle->mapid();
		tInfo.mTime = tpBattle->time();
		m_BattleInfo.insert( CorpsBattleInfo::value_type( tpBattle->npcid(), tInfo ) );*/
		
		if (tpBattle->npcstatus() != STATE_OWN)
		{
			continue;
		}
		NpcStatus tNpcStatus;
		tNpcStatus.m_MapID = GATENPCID(tpBattle->mapid(), m_nCountryID);
		tNpcStatus.m_OwnerID = m_nCorpsID;
		tNpcStatus.m_Status = NpcStatus::STATE_BidOpen;
		tNpcStatus.m_WelfareTime = 0; //福利时间的初始化，需修改
		tNpcStatus.m_OccupyTime = 0;
		//初始化竞价排名状态
		tNpcStatus.m_BidMoneyRank1 = 0;
		tNpcStatus.m_BidMoneyRank2 = 0;
		tNpcStatus.m_CorpsIDRank1 = 0;
		tNpcStatus.m_CorpsIDRank2 = 0;

		tNpcStatus.m_SynTag = DataTag_New; //新数据
		int tGateNpcID = GATENPCID(tpBattle->npcid(), m_nCountryID);
		LOG_DEBUG("corps", "Migtate Mode: get old data from blob:");
		
		NpcStatus* pNpc = NULL;

		if (NULL != (pNpc = CBidMgr::GetSingleton().GetNpcStatus(tGateNpcID)))
		{
			if (tNpcStatus.m_OwnerID != pNpc->m_OwnerID)
			{
				//数据迁移过程中出现不一致
				LOG_ERROR("corps", "Migtate Error: different data from corps blobs:");
				LOG_ERROR("corps", "========>>>>>>>>[Data ignored by migtating]");
				LOG_ERROR("corps", "| NPCID:%d MAPID:%d OWNERID:%d", tGateNpcID, tNpcStatus.m_MapID, tNpcStatus.m_OwnerID);
				//exit(-1);
			}
			continue;
		}
		CBidMgr::GetSingleton().AddNpcStateInfo(tGateNpcID, &tNpcStatus);
	}
	return SUCCESS;
}


// 获得占领的地图列表 [11/29/2009 Macro]
int CCorps::GetMapList( int* npMapList, int& nNum )
{
	if ( npMapList == NULL )
	{
		return -1;
	}
	CBidMgr::GetSingleton().GetMapListByCorps(this, npMapList, nNum);
	
	return SUCCESS;
}


// 获取占领npc对应的地图ID [12/10/2009 Macro]
int CCorps::GetMap( int nNpcID )
{
	NpcStatus *pState = GetNpcState(nNpcID);
	if ( NULL == pState)
	{
		return -1;
	}
	if ( pState->m_OwnerID != GetCorpsID() )
	{
		return -1;	
	}

	return SCENENPCID_2(pState->m_MapID);
}

// 是否对该地图争夺 [12/12/2009 Macro]
bool CCorps::CanBidThisMap( int nMapID )
{
	int tUnionID = GetUnionID();
	if ( tUnionID != INVALID_OBJ_ID )
	{
		// 有军盟
		CUnion* tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tUnionID );
		if ( tpUnion == NULL )
		{
			return false;
		}
		if ( tpUnion->GetCoreID() != GetCorpsID() )
		{
			// 属于军盟但不是盟主，则不能争夺主城
			if ( IS_MAIN_CITY( nMapID ) )
				return false;
		}
	}
	return true;
}

NpcStatus* CCorps::GetNpcState(int _NpcID)
{
	return CBidMgr::GetSingleton().GetNpcStatus( GATENPCID(_NpcID, GetCorpsCountryID()));
}

// 该占领npc的福利领取时间
int CCorps::GetNpcWelfareTime( int nNpcID, int &rTime )
{
	NpcStatus *pState = GetNpcState(nNpcID);
	
	if ( NULL == pState)
	{
		return -1;
	}
	if ( pState->m_OwnerID != GetCorpsID() )
	{
		return -1;	
	}
	rTime = pState->m_WelfareTime;
	return SUCCESS;
}

// 设置npc福利的领取时间
int CCorps::SetNpcWelfareTime( int nNpcID, int nTime )
{
	NpcStatus *pState = GetNpcState(nNpcID);
	if ( NULL == pState)
	{
		return -1;
	}
	if ( pState->m_OwnerID != GetCorpsID() )
	{
		return -1;	
	}
	pState->m_WelfareTime = nTime;
	pState->m_SynTag = DataTag_Upd;
	return SUCCESS;
}

// ***********************************************************
//  Function:		CheckMoneyEnough
//  Description:	检查金钱是否足够
//  Output:			
//  Date:	01/12/2010
// **********************************************************
bool CCorps::CheckMoneyEnough( int nMoney )
{
	if ( nMoney <= 0 )
	{
		return false;
	}

	if ( nMoney > m_nCorpsMoney )
	{
		return	false;
	}	
	return true;
}

// ***********************************************************
//  Function:		CheckCorpsCanUseShop
//  Description:	检查军团是否能够使用某个商店
//  Output:			
//  Date:	01/12/2010
// **********************************************************
int CCorps::CheckCorpsCanUseShop( int nNpcTmpID, int nWarCityMap )
{
	return SUCCESS;
}

// ***********************************************************
//  Function:		GetCorpsShopErrcode
//  Description:	获取使用商店错误码
//  Output:			
//  Date:	01/12/2010
// **********************************************************
int CCorps::GetCorpsShopErrcode( int nErrcode, int &rShopsErrcode )
{	
	switch ( nErrcode )
	{
	case SUCCESS:
		{
			rShopsErrcode = SUCCESS;
			break;			
		}
	case CLeagueShopInfo::SHOPEEROR_EXCHANGEGOODS_NPCHASNOITEM:
		{
			rShopsErrcode = ERROR_CORPS_BUYCORPSGOOD_NOTALLOWED;
			break;
		}	

	case CLeagueShopInfo::SHOPERROR_BUYGOODS_ITEMNOTENOUGH:
		{
			rShopsErrcode = ERROR_CORPS_BUYCORPSGOOD_ITEMNOTENOUGH;
			break;
		}		

	case CLeagueShopInfo::SHOPERROR_BUYGOODS_WRONGTEMP:
		{
			rShopsErrcode =	ERROR_CORPS_BUYCORPSGOOD_NOTALLOWED;
			break;
		}

	case CLeagueShopInfo::SHOPERROR_EXCHANGEGOODS_NPCHASNOCOMPOSEID:
		{
			rShopsErrcode = ERROR_CORPS_EXCHANGE_NOTALLOWED;
			break;
		}

	case CLeagueShopInfo::SHOPERROR_EXCHANGEGOODS_WRONGTEMP:
		{
			rShopsErrcode = ERROR_CORPS_EXCHANGE_NOTALLOWED;
			break;
		}		

	default:
		return -1;
	}	
	return SUCCESS;		
}

// 更新国家类称号触发 [1/28/2010 Macro]
int CCorps::UpdateNationTitle( int nMapID, bool bIsAdd )
{
	if ( nMapID == INVALID_MAP_ID )
	{
		return -1;
	}

	int tAction = 0;
	if ( IS_MAIN_CITY( nMapID ) )
	{
		// 国家占领
		tAction = CTplTitleManager::SCOPE_OWNNATION;
	}
	else
	{
		// 野外地图占领
		tAction = CTplTitleManager::SCOPE_OWNNORMALMAP;
	}

	// 无需处理, return
	if ( tAction == 0 )
	{
		return -1;
	}

	// 调用国家类称号处理接口
	CWorldModule::GetSingleton().OnEventUpdateNationTitle( GetRooterID(), nMapID, tAction, bIsAdd );

	LOG_DEBUG("corps", "UpdateNationTitle=>>Corp_id:%d, Map_id:%d, type:%d-%d", GetCorpsID(), nMapID, tAction, bIsAdd);
	return SUCCESS;
}

// 得到军团势力值
int CCorps::GetPower()
{
	int tMapList[ MAX_CORPS_NPCOWNER ] = { 0 };
	int tMapNum = ARRAY_CNT( tMapList );

	int tRet = GetMapList( tMapList, tMapNum );
	if ( tRet < 0 )
	{
		return 0;
	}

	CTemplateMapTable* tpMapTable = ( CTemplateMapTable* ) CDataStatic::spMapTable;
	if ( tpMapTable == NULL )
	{
		return 0;
	}

	unsigned int tPower = 0;
	for ( unsigned int i = 0; i < tMapNum && i < ARRAY_CNT( tpMapTable->mPower ) ; i ++ )
	{
		tPower += tpMapTable->mPower[ tMapList[ i ] ];
	}

	return tPower;
}

int CCorps::CheckIfHasBidRight(int _RoleID, int _MapID)
{
	//大将军和其他有权利的成员才可竞价
	if (m_nRooterID !=  _RoleID)
	{			
		if (!MemberHasRight( _RoleID, EM_WAR_RID|EM_MONEY_REIN))
		{
			return ERROR_CORPS_HASNORIGHT;
		}								
	}
	// 军团处于解散状态不能竞价
	if (EM_CORPS_STATUS_DISBAND == m_nCorpsStatus)
	{
		return ERROR_CORPS_BIDINDISBANDING;
	}

	// 检查是否可对此地图进行竞价（主城地图，隶属于军盟则只能由盟主竞价）corp
	if ( false == CanBidThisMap(_MapID) )
	{
		LOG_DEBUG("corps", "can not bid this Map ID[%d]", _MapID);
		return ERROR_CORPS_SubBidMainCity;
	}

	return SUCCESS;
}

bool CCorps::IsInBattle(int nBattleType, int nStateType)
{
	if ( TYPE_BATTLE_ALL == nBattleType || TYPE_BATTLE_CHALLEGE == nBattleType )
	{
		if (m_RoomState == ROOM_STATE_INWAR )
		{
			return true;
		}
		
	}
	if ( TYPE_BATTLE_ALL == nBattleType || TYPE_BATTLE_SCRABBLE == nBattleType )
	{
		if (nStateType & ST_Union)
		{
			//所属军盟
			CUnion* tpUnion = CCorpsManager::GetSingleton().GetUnionByID( m_UnionID );
			if ( tpUnion )
			{
				CCorps* tpCoreCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpUnion->GetCoreID() );
				if ( tpCoreCorps && tpCoreCorps->IsInBattle( TYPE_BATTLE_SCRABBLE, (nStateType & (~ST_Union)) | ST_Single ) )
				{
					return true;
				}
			}

		}
		if (nStateType & ST_Single)
		{
			//本军团
			if (CBidMgr::GetSingleton().IsInBid(m_nCorpsID, nStateType))
			{
				return true;
			}
			if (CBattleMgr::GetSingleton().IsInBattleByCorpsID(m_nCorpsID, nStateType))
			{
				return true;
			}
		}
	}
	return false;
}

//当本军团已经占领该城池nCity时，操作才能成功
void CCorps::SetBattleCity( int nCity )
{
	if (CBidMgr::GetSingleton().SetMainCity(m_nCorpsID, nCity))
	{
		m_nBattleCity = nCity; 
	}
	else
	{
		m_nBattleCity = INVALID_MAP_ID; 
	}
}

void CCorps::ChangeCorpsBossTimes(int nBossID, int nTimes )
{
	CorpsBossList:: iterator it = m_CorpsBossList.find( nBossID);
	if( it != m_CorpsBossList.end() )
	{
		it->second += nTimes;
		if ( it->second <= 0 )
		{
			m_CorpsBossList.erase(it);
		}
	
	}else{
		
		if( nTimes > 0)
		{
			m_CorpsBossList.insert( CorpsBossList::value_type(nBossID, nTimes));
	
		}

	}
}

int CCorps:: GetCorpsBossTimes( int nBossID )
{
	CorpsBossList:: iterator it = m_CorpsBossList.find( nBossID);
	
	if ( it != m_CorpsBossList.end() )
	{
		return it->second;
	}else{
		return 0;
	}
}

int CCorps:: GetCorpsBossRefineValueLimit( )
{
	int tLimit = 0;

	CCorpsManager::GetSingleton().GetCorpsConfig()->GetCorpsRefineValueLimitByLevel( m_nCorpsLevel, tLimit );
	return tLimit;

}

int CCorps::GetBossCDCoolTime()
{
	int tBossCDLimit = 0;
	CCorpsManager::GetSingleton().GetCorpsConfig()->GetCorpsBossCDLimit( m_nCorpsLevel, tBossCDLimit);

	int tCDCool = m_CorpsRepetion.GetStartTime()+ tBossCDLimit - time(NULL);

	return tCDCool>0 ? tCDCool: 0;

}