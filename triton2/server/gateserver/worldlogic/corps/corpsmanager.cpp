#include "corpstype.h"	 
#include "family.h"
#include "corps.h"	 
#include "family_manager.h"
#include "corpsmanager.h" 
#include "corpservice.h"
#include "nameservice.h"
#include "gateobj_manager.h"
#include "gatelogic.h"
#include "core_service.h"
#include "log_tools.h"
#include "world.h"
#include "familyservice.h"
#include "corpsmessage_pb.hxx.pb.h"
#include "bidmgr.h"

//using namespace GateServer;

CSharedMem *CCorpsManager::mShmPtr = NULL;
template<> CCorpsManager * CSingleton< CCorpsManager >::spSingleton = NULL;


// ***********************************************************
//  Function:		CCorpsManager
//  Description:	军团的构造函数
//  Output:			
//  Date:	06/10/2009
// **********************************************************
CCorpsManager::CCorpsManager()
{
	if ( CObj::msCreateMode )
	{
		Initailize();
	}
	else
	{
		Resume();
	}
}

// ***********************************************************
//  Function:		operator new
//  Description:	重载new函数
//  Output:			
//  Date:	06/10/2009
// **********************************************************
void *CCorpsManager::operator new( size_t tSize )
{
	return ( void* ) mShmPtr->CreateSegment( tSize );	  	 
}

// ***********************************************************
//  Function:		operator delete
//  Description:	重载delete函数
//  Output:			
//  Date:	06/10/2009
// **********************************************************
void CCorpsManager::operator delete( void *pPointer ) 
{		
}

// ***********************************************************
//  Function:		operator delete
//  Description:	重载delete函数
//  Output:			
//  Date:	06/10/2009
// **********************************************************
unsigned int CCorpsManager::CountSize()
{
	return sizeof( CCorpsManager );
}

// ***********************************************************
//  Function:		Initailize
//  Description:	初始化函数
//  Output:			
//  Date:	06/10/2009
// **********************************************************
int CCorpsManager::Initailize()
{  
	m_tCorpsList.initailize();
	//m_tBidList.initailize();
	//m_tBattleCityOwner.initailize();
	m_tRoomList.initailize();
	//m_tBattleList.initailize();
	m_UnionList.initailize();
	return 0;	
}

// ***********************************************************
//  Function:		Resume
//  Description:	
//  Output:			
//  Date:	06/10/2009
// **********************************************************
int CCorpsManager::Resume()
{	
	return 0;
}

// ***********************************************************
//  Function:		OnEventApplyCorps	
//  Description:	申请加入军团	
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventApplyCorps( int nApplyingID, CCorps *pCorps, FamilyData &pbFamilyData )
{
	LK_ASSERT( pCorps != NULL, return -1 )
	CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nApplyingID );
	
	// 检查是否有家族
	if ( pFamily == NULL )
	{
		return ERROR_CORPS_APPLY_HASNOFAMILY;
	}
	
	// 检查是否有权限
	if ( pFamily->GetRooter() != nApplyingID )
	{
		return ERROR_CORPS_APPLY_HASNORIGHT;
	}
	
	// 检查是否处于解散冷却时间
	if ( pFamily->GetFamilyStatus() == CFamily::EM_FAMILY_STATUS_DISBAND )
	{
		return ERROR_CORPS_APPLY_ONDISBANDSTATUS;
	}
	
	// 检查离开军团的冷却时间
	if( !pFamily->CheckLastTimeLeaveCorps() )
	{
		return ERROR_CORPS_JOINCORPS_INLEAVECOOLTIME;	
	}
	
	// 检查申请冷却时间
	if ( !pFamily->CheckLastTimeApplyCorps() )
	{
		return ERROR_CORPS_APPLY_INCOOLTIME;
	} 
		
	// 检查是否是同一国籍	
	if ( pFamily->GetCountryID() != pCorps->GetCorpsCountryID() )
	{
		return ERROR_CORPS_APPLY_NOTINSAMECOUNTRY; 
	}
	
	// 检查是不是已经加入军团
	if ( pFamily->GetCorpsID() != 0 )
	{
		return ERROR_CORPS_APPLY_HASINCORPS;
	}
		
	// TODO:去掉满员检查 （检查军团的家族成员是否已经满了）
	/*if ( pCorps->CheckMemberFull() )
	{
		return ERROR_CORPS_APPLY_CORPSMEMBERFULL;
	} 	*/	
	
	// 检查申请列表是否已满	
	if ( pCorps->CheckApplyListIfFull() )
	{
		return ERROR_CORPS_APPLY_APPLYLISTFULL;
	}
	
	// 检查是否已经在申请列表里面
	if ( pCorps->ChekcFamilyApply( pFamily->GetID() ) )
	{
		return ERROR_CORPS_APPLY_INAPPLYLIST;
	}
		
	// 插入到申请列表里面
	pCorps->InsertFamilyApply( pFamily->GetID(), nApplyingID );
	pFamily->GetFamilyBasicInfo( pbFamilyData );
	
	// 设置申请时间
	pFamily->SetLastTimeApplyCorps( time(NULL) );	
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventAppointCorpsHeader	
//  Description:	转让大将军职位
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventAppointCorpsHeader( int nHeaderID, int nMemberID )
{
	CFamily *tpHeaderFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nHeaderID );
	if ( tpHeaderFamily == NULL )
	{
		LOG_ERROR( "corps","can not find corps of player( roleid:%d )", nHeaderID );
		return -1;
	}
	
	CCorps *tpCorps = GetCorpsByID( tpHeaderFamily->GetCorpsID() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps"," can not find corps by corpsID:%d ", tpHeaderFamily->GetCorpsID() );
		return -1;
	}
		
	CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nMemberID );	
	if ( pFamily == NULL )
	{
		LOG_ERROR( "corps","can not find family of player( roleid:%d )", nMemberID );
		return -1;
	}
	
	// 判断是不是军团首领
	if ( tpCorps->GetRooterID() != nHeaderID )
	{
		return ERROR_CORPS_HASNORIGHT;
	} 	
	
	// 不能转让给自己
	if ( nHeaderID == nMemberID  )
	{
		return	ERROR_CORPS_APPOINTHEADER_SELFAPPOINT;
	}  

	if ( tpCorps->IsOwner() )
	{
		return ERROR_CORPS_CHANGE_ISKING;
	}
	
	// 家族是不是在军团中
	if ( pFamily->GetCorpsID() != tpCorps->GetCorpsID() )
	{
		return ERROR_CORPS_APPOINT_NOTINCORPS;
	}
	
	// 检查对方是不是家族首领
	if ( pFamily->GetRooter() != nMemberID )
	{
		return ERROR_CORPS_APPOINT_NOTFAMILYHEADER;
	}
	
	CRoleGlobalInfo *tpInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( nMemberID );
	if ( tpInfo == NULL )
	{
		LOG_ERROR( "corps","can not find player info( CRoleGlobalInfo ) ( roleid:%d )", nMemberID );
		return -1;
	}
	
	tpCorps->SetRooterID( nMemberID );
	tpCorps->SetRooterName( tpInfo->RoleName() );
	
	// 设置新军团首领的职位		
	int tOldPost = 0;
	pFamily->SetCorpsPost( nMemberID, CORPS_POST_CORPSHEADER, tOldPost );
	
	// 设置旧军团首领的职位	
	tpHeaderFamily->SetCorpsPost( nHeaderID, CORPS_POST_FAMILYHEADER, tOldPost );
	
	LogOrganSetPost( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), nHeaderID, nHeaderID, CORPS_POST_FAMILYHEADER, CORPS_POST_CORPSHEADER )
	LogOrganSetPost( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), nHeaderID, nMemberID, CORPS_POST_CORPSHEADER, tOldPost )	
	return SUCCESS;
}	

// ***********************************************************
//  Function:		OnEventCreatCorps	
//  Description:	创建军团
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventCreatCorps( int nRoleID, const char *pCorpsName, int nCorpsID, int nCorpsColor )
{		
	// 等级，脏词，已经检查
	LK_ASSERT( pCorpsName != NULL, return -1 )	
	CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nRoleID );
	if ( pFamily == NULL )
	{
		return ERROR_CORPS_CREAT_HASNOFAMILY;
	}
	
	// 检查是否是家族族长
	if ( pFamily->GetRooter() != nRoleID )
	{
		return ERROR_CORPS_CREAT_ISNOTFAMILYHEADER;
	}
	
	// 检查是否有军团
	if ( pFamily->GetCorpsID() != 0 )
	{
		return ERROR_CORPS_CREAT_INCORPS;
	}
	
	// 检查家族是否处于解散状态
	if ( pFamily->GetFamilyStatus() == CFamily::EM_FAMILY_STATUS_DISBAND )
	{
		return ERROR_CORPS_CREAT_ONDISBANDSTATUS; 
	}
	
	// 检查军团名是否已经存在
	if ( GetCorpsByName( pCorpsName ) != NULL )
	{
		return ERROR_CORPS_CREAT_NAMEEXIST;
	}
	
	// 检查家族是否刚离开军团
	if ( !pFamily->CheckLastTimeLeaveCorps() )
	{
		return ERROR_CORPS_CREATCORPS_INDISBANDCOOLTIME;
	}
	
	// 检查军团列表是否已满
	if ( m_tCorpsList.size() >= SERVER_CAP_CORPS )
	{
		return -1;
	}
	
	CRoleGlobalInfo *tpRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( nRoleID );	
	LK_ASSERT( tpRoleInfo != NULL, return -1 )	
	CCorps *tpCorps = ( CCorps * )CGateObjManager::GetSingleton().CreateObject( OBJTYPE_CORPS );
	if( tpCorps == NULL )
	{	
		LOG_ERROR( "corps"," creat corps error by roleid:%d ! ", nRoleID );
		return -1;
	}
	
	
	tpCorps->SetCorpsID( tpCorps->get_id() );	
	tpCorps->SetCorpsLevel( 1 );
	tpCorps->SetRooterID( nRoleID );
	tpCorps->SetRooterName( tpRoleInfo->RoleName() );
	tpCorps->SetCorpsName( pCorpsName );
	tpCorps->SetCorpsCountryID( tpRoleInfo->CountryID() );
	tpCorps->SetColor( nCorpsColor );
	
	CCorpsFamiyInfo tCorpFamilyInfo;
	tpCorps->InsertFamily( pFamily->GetID(), tCorpFamilyInfo );	

		
	#ifdef _GLOBAL_NAME_ 
	CGateLogic::GetSingleton().SendCreateGroupProto2ERating(
			            nRoleID, pCorpsName, em_corps_group, tpCorps->get_id());
	#else
	nCorpsID = tpCorps->GetCorpsID(); 	
	CreatCorpsDBInfo( tpCorps );

	/*
	int tOldPost = 0;
	pFamily->SetPost( nRoleID, CORPS_POST_CORPSHEADER, tOldPost, CFamily::EM_CORPSACTION );		 
	*/
	#endif
	
	
	// 创建成功之后再进行插入
	//m_tCorpsList.insert( CorpsList::value_type( tpCorps->GetCorpsID(), tpCorps->get_id() ) );
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventDisbandCorps	
//  Description:	解散军团
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventDisbandCorps( int nRooterID, int *pMemberList, int &nMemberNum )
{ 
	CFamily *tpRooterFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nRooterID );
	LK_ASSERT( tpRooterFamily != NULL, return -1 )
	CCorps *tpCorps = GetCorpsByID( tpRooterFamily->GetCorpsID() );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps"," can not find corps by ID:%d ", tpRooterFamily->GetCorpsID() );
		return -1;
	}			  	
	
	if ( tpCorps->GetRooterID() != nRooterID )
	{
		return ERROR_CORPS_DISBAND_NOTHEADER;
	}
	if ( tpCorps->GetCorpsMemberNum() >= CORPSDISBAND_NUMBERLIMIT )
	{
		return ERROR_CORPS_DISBAND_NUMLIMIT;
	} 
	if ( tpCorps->GetCorpsStatus() == CCorps::EM_CORPS_STATUS_DISBAND )
	{
		return ERROR_CORPS_DISBAND_DISBANDSTATUS;
	}
	
	// 军团处于竞价，争夺和战斗及占领城池状态不能解散
	if (tpCorps->IsInBattle(TYPE_BATTLE_ALL, ST_AllState))
	{
		return ERROR_CORPS_CORPSCANNOTBEDISBANDED;		
	}	

	CUnion *tpUnion = GetUnionByID( tpCorps->GetUnionID( ) );
	if ( tpUnion != NULL )
	{
		if ( tpUnion->GetCoreID() == tpCorps->GetCorpsID() )
		{
			return ERROR_UNION_DISBAND_ISLEADER;
		}
	}
		
	tpCorps->SetCorpsStatus( CCorps::EM_CORPS_STATUS_DISBAND );
	tpCorps->SetCorpsDisbandTime( time(NULL) );

	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventChangeCoprsNotice	
//  Description:	改变军团公告信息
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventChangeCoprsNotice( int nRoleID, const char *pNotice )
{ 
	LK_ASSERT( pNotice != NULL, return -1 )
	CCorps *tpCorps = GetCorpsOfPlayer( nRoleID );
	LK_ASSERT( tpCorps != NULL, return -1 )
	if ( tpCorps->GetRooterID() != nRoleID )
	{
		int tPost = 0;
		tpCorps->GetMemberPost( nRoleID, tPost );
		if ( !tpCorps->PostHasRight( tPost, EM_CHANGE_BULLETIN ) )
		{
			return ERROR_CORPS_HASNORIGHT;
		}	
	}
	tpCorps->SetBulletin( pNotice );
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventGetCorpsMemberInfo	
//  Description:	军团中某家族的信息
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventGetCorpsMemberInfo( int nRoleID, PBCorpsMemberList &pbCorpsMemberList )
{ 
	CCorps *tpCorps = GetCorpsOfPlayer( nRoleID );
	LK_ASSERT( tpCorps != NULL, return -1 )	 
	
	if ( tpCorps->GetCorpsMemberNum() == 0 )
	{
		// TODO:防止军团信息出现错误
		GetCorpsMemberListByFamilyID( tpCorps ); 
		LOG_ERROR( "gateserver","corps:(%s) has error!", tpCorps->GetCorpsName() ); 	
	}
	 
	tpCorps->GetCorpsMemberList( pbCorpsMemberList );  
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventGetCorpsProperty	
//  Description:	军团的基本属性
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventGetCorpsProperty( int nRooterID, CorpsProperty &pbCorpsInfo )
{	
	CCorps *tpCorps = GetCorpsOfPlayer( nRooterID );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps", "can not find corps of player( roleid:%d )", nRooterID );
		return -1;
	}	

	pbCorpsInfo.set_corpsbuttullin( tpCorps->GetBulletin() );
	pbCorpsInfo.set_corpsfamilynum( tpCorps->GetFamilyNum() );
	pbCorpsInfo.set_corpsid( tpCorps->GetCorpsID() );
	pbCorpsInfo.set_corpslevel( tpCorps->GetCorpsLevel() );
	pbCorpsInfo.set_corpsmembernum( tpCorps->GetCorpsMemberNum() );
	pbCorpsInfo.set_corpsmoney( tpCorps->GetCorpsMoney() );
	pbCorpsInfo.set_corpsname( tpCorps->GetCorpsName() );
	pbCorpsInfo.set_corpsrooterid( tpCorps->GetRooterID() );
	pbCorpsInfo.set_corpsrootername( tpCorps->GetRooterName() );
	pbCorpsInfo.set_corpsglory( tpCorps->GetCorpsGlory() );
	pbCorpsInfo.set_challengestatus( tpCorps->GetRoomState() );
	pbCorpsInfo.set_challengemoney( tpCorps->GetRoomMoney() );
	pbCorpsInfo.set_bossrepetionopen( tpCorps->m_CorpsRepetion.IsOpen()== true? 1:0);
	pbCorpsInfo.set_corpscurbossid( tpCorps->m_CorpsRepetion.GetCorpsBossID() );
	
	if ( tpCorps->CanCorpsLevelUP() )
	{
		pbCorpsInfo.set_canlevelup( 1 );
	}
	
	int tMaxScore	= 0;
	CCorpsManager::GetSingleton().GetCorpsConfig()->GetPvPStarScore( tpCorps->GetCorpsLevel(), tMaxScore );
	pbCorpsInfo.set_warmaxexp( tMaxScore );
	
	int tMaxGeneralNum = 0;
	CCorpsManager::GetSingleton().GetCorpsConfig()->GetGeneralNumLimitByLevel( tpCorps->GetCorpsLevel(), tMaxGeneralNum );
	pbCorpsInfo.set_maxgeneralnum( tMaxGeneralNum );
	
	int tMaxCounsellorNum = 0;
	CCorpsManager::GetSingleton().GetCorpsConfig()->GetCounSellorNumLimitByLevel( tpCorps->GetCorpsLevel(), tMaxCounsellorNum );
	pbCorpsInfo.set_maxcounsellornum( tMaxCounsellorNum );

	int tMaxNum = 0;
	CCorpsManager::GetSingleton().GetCorpsConfig()->GetFamilyNumLimitByLevel( tpCorps->GetCorpsLevel(), tMaxNum );
	pbCorpsInfo.set_maxfamilynum( tMaxNum );
	
	tpCorps->MakeCorpsPbPropert( &pbCorpsInfo );
	pbCorpsInfo.set_countryid( CWorldManager::GetSingleton().GetCountryIDByMap( tpCorps->GetBattleCity() ) );
	
	// 如果有军盟， 发主军团颜色
	if ( tpCorps->GetUnionID() != INVALID_OBJ_ID )
	{
		CUnion* tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tpCorps->GetUnionID() );
		if ( tpUnion != NULL )
		{
			CCorps* tpHeadCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpUnion->GetCoreID() );
			if ( tpHeadCorps != NULL )
				pbCorpsInfo.set_color( tpHeadCorps->GetColor() );
		}	
	}
	else
		pbCorpsInfo.set_color( tpCorps->GetColor() );
		
		
	// 获取主城ID
	pbCorpsInfo.set_maincitymap( tpCorps->GetBattleCity() );
	
	int tLevelGlory = 0;	
	int tRet = CCorpsManager::GetSingleton( ).GetCorpsConfig( )->GetCorpsGloryLimitByLevel( tpCorps->GetCorpsLevel(), tLevelGlory );
	if ( tRet == SUCCESS )
	{
		pbCorpsInfo.set_levelglory( tLevelGlory );
	}

	int tLevelMoney = 0;
	tRet = CCorpsManager::GetSingleton().GetCorpsConfig()->GetMoneyLevelUpNeeded( tpCorps->GetCorpsLevel(), tLevelMoney );
	if ( tRet == SUCCESS )
	{
		pbCorpsInfo.set_levlemoney( tLevelMoney );
	}

	int tLimitRefine = 0;
	tRet = CCorpsManager::GetSingleton().GetCorpsConfig()->GetCorpsRefineValueLimitByLevel( tpCorps->GetCorpsLevel(), tLimitRefine);
	if ( tRet == SUCCESS )
	{
		pbCorpsInfo.set_refinevalue( tLimitRefine );
	}

	int tBossCDLimit = 0;
	tRet = CCorpsManager::GetSingleton().GetCorpsConfig()->GetCorpsBossCDLimit( tpCorps->GetCorpsLevel(), tBossCDLimit );
	if ( tRet == SUCCESS )
	{
		pbCorpsInfo.set_bosscdlimit( tBossCDLimit );
	}

	pbCorpsInfo.set_bossrefinevalue( tpCorps->GetBossRefineValue() );

	pbCorpsInfo.set_bosscdcool( tpCorps->GetBossCDCoolTime());

	CCorps::CorpsBossList:: iterator iter = tpCorps->m_CorpsBossList.begin();
	for ( ; iter != tpCorps->m_CorpsBossList.end(); ++iter)
	{
		PBCorpsBoss * pCorpsBoss = pbCorpsInfo.add_bosslist();

		pCorpsBoss->set_bossid( iter->first);
		pCorpsBoss->set_canintotimes( iter->second);
	}

	pbCorpsInfo.set_corpsbossscore( tpCorps->GetCorpsBossScore() );
	pbCorpsInfo.set_corpscollectactive( tpCorps->GetCorpsCollectActive() );
	
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventInviteFamily	
//  Description:	邀请家族加入军团
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventInviteFamily( int nCorpsRooterID, CFamily *pFamily )
{	 
	if ( pFamily == NULL )
	{
		return	ERROR_CORPS_INVITE_FAMILYNOTEXIST;
	}
	if ( pFamily->GetCorpsID() != 0 )
	{
		return ERROR_CORPS_INVITE_INCORPS;
	}
	if ( pFamily->GetFamilyStatus() == CFamily::EM_FAMILY_STATUS_DISBAND )
	{
		return ERROR_CORPS_INVITE_ONDISBANDSTATUS;
	} 	
	CCorps *tpCorps = GetCorpsOfPlayer( nCorpsRooterID );
	LK_ASSERT( tpCorps != NULL, return -1 )
	if ( tpCorps->GetCorpsCountryID() != pFamily->GetCountryID() )
	{	 
		return	ERROR_CORPS_INVITE_NOTINOTHERCOUNTRY;
	}
	if ( tpCorps->GetRooterID() != nCorpsRooterID )
	{
		int tPost = 0;
		tpCorps->GetMemberPost( nCorpsRooterID, tPost );
		if ( !tpCorps->PostHasRight( tPost, EM_INVITE_MEMBER ) )
		{
			return ERROR_CORPS_HASNORIGHT;
		}		
	}
	
	if ( tpCorps->CheckMemberFull() )
	{
		return ERROR_CORPS_MEMBERFULL;
	}
	int tRet = pFamily->InsertCorpsInvite( tpCorps->GetCorpsID() );
	if ( tRet != SUCCESS )
	{
		return ERROR_CORPS_INVITE_INVITELISTFULL;
	}
	return SUCCESS;	
}

// ***********************************************************
//  Function:		OnEventKickFamilyOfCorps	
//  Description:	踢出军团中的某个家族
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventKickFamilyOfCorps( int nRooterID, int nFamilyID )
{ 
	CCorps *tpCorps = GetCorpsOfPlayer( nRooterID );
	LK_ASSERT( tpCorps != NULL, return -1 )
	if ( tpCorps->GetRooterID() != nRooterID )
	{
		int tPost = 0;
		tpCorps->GetMemberPost( nRooterID, tPost );
		if ( !tpCorps->PostHasRight( tPost, EM_FIRE_MEMBER ) )
		{
			return ERROR_CORPS_HASNORIGHT;
		}		
	}
	
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( nFamilyID );
	LK_ASSERT( tpFamily != NULL, return -1 )
	LK_ASSERT( tpCorps->CheckHasFamily( nFamilyID ), return -1 )
	
	if ( tpCorps->GetRooterID() == tpFamily->GetRooter() )
	{
		return ERROR_CORPS_KICKFAMILY_CORPSHEADERINFAMILY;
	}
	
	if ( tpFamily->HasMember( nRooterID ) )
	{
		
		return ERROR_CORPS_LEAVECORPS_NOTFAMILYHEADER;
	}
	
	// TODO:处于竞价,战斗状态的军团踢出家族
	//if ( tpCorps->GetWarNpcStatus() == STATE_CHAZING || tpCorps->GetRoomState() != ROOM_STATE_NONE )
	//{
	//	return ERROR_CORPS_CANOTKICKFAMILY;		
	//}	

	// 战斗中不能踢出家族
	if (tpCorps->IsInBattle(TYPE_BATTLE_ALL, ST_WaitBtl|ST_InBtl|ST_Union|ST_Single))
	{
		return ERROR_CORPS_CANOTKICKFAMILY;
	}
	
	tpCorps->DeletFamily( nFamilyID );	
	tpFamily->SetCorpsID( 0 );
	tpFamily->SetLastTimeLeaveCorps( time(NULL) );	
	int tOldPost = 0;
	tpFamily->SetCorpsPost( tpFamily->GetRooter(), CORPS_POST_FAMILYHEADER, tOldPost );
	return SUCCESS;	
}

// ***********************************************************
//  Function:		OnEventLeaveCorps	
//  Description:	离开军团
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventLeaveCorps( int nRoleID )
{	
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nRoleID );
	LK_ASSERT( tpFamily != NULL, return -1 )
	if ( tpFamily->GetRooter() != nRoleID )
	{
		return ERROR_CORPS_LEAVECORPS_NOTFAMILYHEADER;
	}		 
	CCorps *tpCorps = GetCorpsOfPlayer( nRoleID );
	LK_ASSERT( tpCorps != NULL, return -1 )	
	if ( tpCorps->GetRooterID() == nRoleID )
	{
		return ERROR_CORPS_LEAVE_ISCORPSHEADER;
	}	

	if (tpCorps->IsInBattle(TYPE_BATTLE_ALL, ST_WaitBtl|ST_InBtl|ST_Union|ST_Single))
	{
		return ERROR_CORPS_CANOTLEAVECORPS;
	}

	tpCorps->DeletFamily( tpFamily->GetID() );
	tpFamily->SetCorpsID( 0 );
	tpFamily->SetLastTimeLeaveCorps( time( NULL ) );
	int tOldPost = 0;
	tpFamily->SetCorpsPost( tpFamily->GetRooter(), CORPS_POST_FAMILYHEADER, tOldPost );

	CUnion* tpUnion = this->GetUnionByID( tpCorps->GetUnionID() );
	if ( tpUnion != NULL )
	{
		tpUnion->DeleteBattleMemberByRoleID( nRoleID );
	}
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventListCorps	
//  Description:	获取服务器的军团信息
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventListCorps( int nRoleID, int nCountryID, CorpsInfoList &pbCorpsList, int nCurPage, int &rTotalPage )
{  
	if ( m_tCorpsList.begin() == m_tCorpsList.end() )
	{
		return SUCCESS;
	}
	int tTotalCorpsNum = 0;
	for ( CorpsList::iterator it = m_tCorpsList.begin(); it != m_tCorpsList.end(); ++it )
	{	  
		CCorps *tpCorps = GetCorpsByID( it->first );
		if ( tpCorps == NULL )
		{
			LOG_ERROR( "corps"," can not find corps by id:%d ", it->first );
			continue;
		}		
										
		if ( tpCorps->GetCorpsCountryID() != nCountryID )
		{
			continue;
		}
		if ( tTotalCorpsNum >= nCurPage*FAMILYCORPS_MAXGETNUM && tTotalCorpsNum < ( nCurPage+1 )*FAMILYCORPS_MAXGETNUM )
		{
			PBCorpsBasicInfo *pbCorpsBasicInfo = pbCorpsList.add_corpslist();
			tpCorps->GetCorpsBasicInfo( *pbCorpsBasicInfo );
		}
		++tTotalCorpsNum;
	}	
	rTotalPage = tTotalCorpsNum / FAMILYCORPS_MAXGETNUM;
	if ( tTotalCorpsNum % FAMILYCORPS_MAXGETNUM > 0 )
	{
		++rTotalPage;
	}
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventReplyApply	
//  Description:	答复家族申请
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventReplyApply( int nReplyingID, int nFamilyID, int nResult, int &nApplyingID )
{
	CCorps *tpCorps = GetCorpsOfPlayer( nReplyingID );
	if ( tpCorps == NULL )
	{
		LOG_ERROR( "corps"," can not find corps of player( roleid:%d ) ", nReplyingID );
		return -1;
	}
	
	CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( nFamilyID );
	if ( pFamily == NULL )
	{
		return ERROR_CORPS_REPLYAPPLY_FAMILYEXIST;
	}	
	
	// 权限检查
	if ( tpCorps->GetRooterID() != nReplyingID )
	{	 
		int tPost = 0;
		tpCorps->GetMemberPost( nReplyingID, tPost );
		if ( !tpCorps->PostHasRight( tPost, EM_APPROVE_APPLY ) )
		{
			return ERROR_CORPS_HASNORIGHT;
		}		
	}
	
	// 检查申请列表里面是否存在家族的申请
	if ( !tpCorps->ChekcFamilyApply( nFamilyID ) )
	{		
		return -1;		
	}
	
	nApplyingID = tpCorps->GetApplyingRoleID( nFamilyID );
	
	if ( nResult == 0 || nResult == 2 )
	{
		tpCorps->DeletFamilyApply( nFamilyID );
		return SUCCESS;
	}  		
	
	// 查看家族是否已经处于解散状态
	if ( pFamily->GetFamilyStatus() == CFamily::EM_FAMILY_STATUS_DISBAND )
	{
		nApplyingID = 0;
		return ERROR_CORPS_APPLY_ONDISBANDSTATUS;
	}
	
	// 查看是否有军团
	if ( pFamily->GetCorpsID() != 0 )
	{
		return ERROR_CORPS_REPLYAPPLY_INCORPS;		
	}
	
	CCorpsFamiyInfo tCorpsFamiyinfo;
	int tRet = tpCorps->InsertFamily( nFamilyID, tCorpsFamiyinfo );
	if ( tRet == CCorps::FAMILYHASEXIST )
	{
		return ERROR_CORPS_REPLYAPPLY_INCORPS;
	}
	
	if ( tRet == CCorps::FAMILYLISTFULL )
	{
		return ERROR_CORPS_REPLYAPPLY_MEMBERFULL;
	}
	
	int tOldPost = 0;
	pFamily->SetCorpsPost( pFamily->GetRooter(), CORPS_POST_FAMILYHEADER, tOldPost );
	pFamily->SetCorpsID( tpCorps->GetCorpsID() );
	tpCorps->DeletFamilyApply( nFamilyID );
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventReplyCorpsInvite	
//  Description:	答复军团邀请
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventReplyCorpsInvite( int nInvitedID, CCorps *pCorps, int nResult )
{  
	LK_ASSERT( pCorps != NULL, return -1 )
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nInvitedID );
	LK_ASSERT( tpFamily != NULL, return -1 ); 
	LK_ASSERT( tpFamily->CheckCorpsInvite( pCorps->GetCorpsID() ), return -1 )
	
	// 检查是否有权限
	if ( tpFamily->GetRooter() != nInvitedID )
	{			
		return	ERROR_CORPS_HASNORIGHT;
	}
	
	// 从邀请列表里面删除
	tpFamily->DeleteCorpsInvite( pCorps->GetCorpsID( ) );
	
	// 判断是否是忽略或者拒绝
	if ( nResult == FAMILY_REPLY_REFUSE || nResult == FAMILY_REPLY_IGNORE )
	{
		return SUCCESS;
	}  
		
	// 检查家族的状态,处于解散状态的家族不能加入军团
	if ( tpFamily->GetFamilyStatus() == CFamily::EM_FAMILY_STATUS_DISBAND )
	{
		return ERROR_CORPS_REPLYINVITE_ONDISBANDSTATUS;
	}
	
	// 判断离开家族的冷却时间
	if ( !tpFamily->CheckLastTimeLeaveCorps() )
	{		
		return ERROR_CORPS_JOINCORPS_INLEAVECOOLTIME;
	}
				
	// 检查家族是否已经加入军团
	if ( tpFamily->GetCorpsID() != 0 )
	{
		return ERROR_CORPS_REPLYINVITE_INCORPS;
	}
	//
	//// 检查军团是否已经满了
	//if ( pCorps->CheckMemberFull() )
	//{
	//	return ERROR_CORPS_REPLYINVITE_CORPSMEBERFULL;
	//}	
	CCorpsFamiyInfo tCorpsFamiyinfo;	
	int tRet = pCorps->InsertFamily( tpFamily->GetID(), tCorpsFamiyinfo );
	if ( tRet == CCorps::FAMILYHASEXIST )
	{
		return ERROR_CORPS_REPLYAPPLY_INCORPS;
	}

	if ( tRet == CCorps::FAMILYLISTFULL )
	{
		return ERROR_CORPS_REPLYAPPLY_MEMBERFULL;
	}	
	
	int tOldPost = 0;
	tpFamily->SetCorpsPost( nInvitedID, CORPS_POST_FAMILYHEADER, tOldPost ); 		
	tpFamily->SetCorpsID( pCorps->GetCorpsID() );
	return SUCCESS;
}

// ***********************************************************
//  Function:		GetCorpsByID	
//  Description:	通过军团ID获取军团信息
//  Output:			
//  Date:	06/11/2009
// **********************************************************
CCorps * CCorpsManager::GetCorpsByID( int nCorpsID )
{	
	if ( m_tCorpsList.begin() == m_tCorpsList.end() )
	{
		return NULL;
	}
	
	for ( CorpsList::iterator it = m_tCorpsList.begin(); it != m_tCorpsList.end(); ++it )
	{
		if ( it->first == nCorpsID )
		{
			CCorps *tpCorps = ( CCorps * )CGateObjManager::GetSingleton().GetObject( it->second );
			return tpCorps;
		}		
	}
	return NULL;				
}

// ***********************************************************
//  Function:		GetCorpsByRoleID	
//  Description:	通过玩家ID获取军团
//  Output:			
//  Date:	06/11/2009
// **********************************************************
CCorps *CCorpsManager::GetCorpsOfPlayer( int nRoleID )
{ 
	CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nRoleID );
	if ( pFamily == NULL )
	{
		return NULL;
	}
	if ( pFamily->GetCorpsID() == 0 )
	{
		return NULL;
	}
	CCorps *tpCorps = GetCorpsByID( pFamily->GetCorpsID() );
	return tpCorps;
}

// ***********************************************************
//  Function:		GetCorpsByName	
//  Description:	通过军团名称获取军团
//  Output:			
//  Date:	06/12/2009
// **********************************************************
CCorps *CCorpsManager::GetCorpsByName( const char *pCorpsName )
{
	LK_ASSERT( pCorpsName != NULL, return NULL )

	if ( m_tCorpsList.begin() == m_tCorpsList.end() )
	{
		return NULL;
	}

	for ( CorpsList::iterator it = m_tCorpsList.begin(); it != m_tCorpsList.end(); ++it )
	{		
		CCorps *tpCorps = ( CCorps * )CGateObjManager::GetSingleton().GetObject( it->second );
		LK_ASSERT( tpCorps != NULL, continue )  				
		if ( strncmp( pCorpsName, tpCorps->GetCorpsName(), MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES -1 ) == 0 )
		{
			return tpCorps;
		}
	}
	return NULL;	
}


// ***********************************************************
//  Function:		InsertCoprsInfo	
//  Description:	插入一个军团的信息
//  Output:			
//  Date:	06/15/2009
// **********************************************************
int CCorpsManager::InsertCoprsInfo( CCorps *pCorps )
{
	LK_ASSERT( pCorps != NULL, return -1 )
	if ( m_tCorpsList.size() >= SERVER_CAP_CORPS  )
	{
		return -1;
	}
	m_tCorpsList.insert( CorpsList::value_type( pCorps->GetCorpsID(), pCorps->get_id() ) );
	return 0;
}

// ***********************************************************
//  Function:		DeletCorpsDBInfo	
//  Description:	删除军团的消息
//  Output:			
//  Date:	06/15/2009
// **********************************************************
int CCorpsManager::DeleteCorpsInfo( CCorps *pCorps )
{
	LK_ASSERT( pCorps != NULL, return -1 )
	CorpsList::iterator it = m_tCorpsList.find( pCorps->GetCorpsID() );
	if ( it != m_tCorpsList.end() )
	{
		m_tCorpsList.erase( it ) ;
	}
	return 0;
}

// ***********************************************************
//  Function:		GetCorpsMeberList	
//  Description:	获取军团的成员信息列表
//  Output:			
//  Date:	06/11/2009
// **********************************************************
void CCorpsManager::GetCorpsMemberList( CCorps *tpCorps, int *pMemberList, int &nNum, int nExceptFamiyID /* = 0 */, int nExceptRoleID /* = 0 */ )
{ 
	// 家族未加载成功前，不允许改该操作
	if ( CServiceFamily::GetSingleton().IsLaunched() == false )
	{			
		return;
	}	

	// 军团未加载完成前不允许该操作
	if ( CCorpservice::GetSingleton().IsLaunched() == false )
	{
		return;
	}
	
	LK_ASSERT( tpCorps != NULL && pMemberList != NULL, return )		
	if ( nNum <= 0 )   
	{
		return;
	} 			
	int nLen = nNum;
	nNum = 0;	
	if ( tpCorps->GetCorpsMemberNum() == 0 )
	{	
		// TODO:防止军团信息出现错误
		GetCorpsMemberListByFamilyID( tpCorps );
		LOG_ERROR( "gateserver","corps:(%s) CorpsID:%d has error!", tpCorps->GetCorpsNameShow(), tpCorps->GetCorpsID() );  	
	}	  
	
	for ( CCorps::FamilyList::iterator it = tpCorps->GetFamilyList().begin();
		it != tpCorps->GetFamilyList().end(); ++it )
	{
		if ( nExceptFamiyID == it->first )
		{
			continue;
		}
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( it->first );
		LK_ASSERT( pFamily != NULL, continue )
		int tNum =  nLen - nNum; 
		pFamily->GetFamilyMemberIDList( pMemberList+nNum, tNum, nExceptRoleID );
		nNum += tNum;
	}		
}

// ***********************************************************
//  Function:		GetCorpsMemberListByFamilyID	
//  Description:	通过家族ID获取军团的成员信息列表
//  Output:			
//  Date:	08/10/2009
// **********************************************************
void CCorpsManager::GetCorpsMemberListByFamilyID( CCorps *tpCorps )
{	
	// 家族未加载成功前，不允许改该操作
	if ( CServiceFamily::GetSingleton().IsLaunched() == false  )
	{			
		return;
	}	
	
	// 军团未加载完成前不允许该操作
	if ( CCorpservice::GetSingleton().IsLaunched() == false )
	{
		return;
	}
	LK_ASSERT( tpCorps != NULL, return )			
	CFamilyManager::TFamilies::iterator iter = CFamilyManager::GetSingleton().GetFamilyList()->begin();
	CFamily *pFamily = NULL;		  	
	tpCorps->GetFamilyList().clear();	
	for( ; iter != CFamilyManager::GetSingleton().GetFamilyList()->end( ); ++iter )
	{
		pFamily = ( CFamily * )CGateObjManager::GetSingleton().GetObject( iter->second );
		if ( pFamily == NULL )
		{
			LOG_ERROR( "corps"," can not find pFamily by ID:%d, OBJID:%d ", iter->first, iter->second );
			continue;			
		}
				
		if ( tpCorps->GetCorpsID() == pFamily->GetCorpsID() )
		{ 
			CCorpsFamiyInfo tPBFamilyInfo;
			tPBFamilyInfo.mContribute = 0;			
			tpCorps->InsertFamily( pFamily->GetID(), tPBFamilyInfo );
		} 						
	}	 	
}

// ***********************************************************
//  Function:		OnEventListCorpsApply	
//  Description:	获取军团的申请
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventListCorpsApply( int nRoleID, PBCorpsApplyList &pbCorpsApply )
{
	CCorps *tpCorps = GetCorpsOfPlayer( nRoleID );
	LK_ASSERT( tpCorps != NULL, return -1 )
	if ( tpCorps->GetRooterID() != nRoleID )
	{
		int tPost = 0;
		tpCorps->GetMemberPost( nRoleID, tPost );
		if ( !tpCorps->PostHasRight( tPost, EM_APPROVE_APPLY ) )
		{
			return ERROR_CORPS_HASNORIGHT;
		}		
	}  	
	tpCorps->GetApplyList( pbCorpsApply );	
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventClearCorpsInfo	
//  Description:	清除军团的数据信息
//  Output:			
//  Date:	06/11/2009
// **********************************************************
int CCorpsManager::OnEventClearCorpsInfo( CCorps *pCorps )
{	
	// 清除军团中家族的军团数据
	pCorps->ClearCorpsInfoOfFamily();	 

	// 删除数据库的信息
	DeletCorpsDBInfo( pCorps  );

	// 删除网关上的数据
	DeleteCorpsInfo( pCorps );

	// 删除军团对象
	CGateObjManager::GetSingleton().DestroyObject( pCorps->get_id() );	
	
	return 0;
}

// ***********************************************************
//  Function:		OnEventCancelCorpsDisband	
//  Description:	取消解散军团
//  Output:			
//  Date:	06/18/2009
// **********************************************************
int CCorpsManager::OnEventCancelCorpsDisband( int nRoleID )
{
	CCorps *tpCorps = GetCorpsOfPlayer( nRoleID );
	LK_ASSERT( tpCorps != NULL, return  -1 )
	if ( tpCorps->GetRooterID() != nRoleID )
	{
		return ERROR_CORPS_HASNORIGHT;
	}
	if ( tpCorps->GetCorpsStatus() != CCorps::EM_CORPS_STATUS_DISBAND )
	{
		return ERROR_CORPS_CANCLEDISBAND_NOTNEED;
	}
	tpCorps->SetCorpsStatus( CCorps::EM_CORPS_STATUS_ALIVE );
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventClearCorpsApply	
//  Description:	清除军团的申请信息
//  Output:			
//  Date:	06/12/2009
// **********************************************************
int CCorpsManager::OnEventClearCorpsApply( int nRoleID )
{ 
	CCorps *tpCorps = GetCorpsOfPlayer( nRoleID );
	LK_ASSERT( tpCorps != NULL, return -1 )
	
	// 权限检查
	if ( tpCorps->GetRooterID() != nRoleID )
	{
		int tPost = 0;
		tpCorps->GetMemberPost( nRoleID, tPost );
		if ( !tpCorps->PostHasRight( tPost, EM_APPROVE_APPLY ) )
		{
			return ERROR_CORPS_HASNORIGHT;
		}		
	}
	
	// 清空列表
	tpCorps->ClearCorpsApplyList();
	return SUCCESS;	
}


// ***********************************************************
//  Function:		OnEventSetCorpsPost	
//  Description:	设置军团职位
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsManager::OnEventSetCorpsMemberPost( int nManagerID, int nMemberID, int nPost, int &rOldPost )
{
	CCorps *tpCorps = GetCorpsOfPlayer( nManagerID );
	LK_ASSERT( tpCorps != NULL, return -1 )
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nMemberID );
	LK_ASSERT( tpFamily != NULL, return -1 )
	if ( !tpCorps->CheckHasFamily( tpFamily->GetID() ) )
	{
		return -1;
	}
	
	if ( nManagerID == nMemberID )
	{
		return -1;
	}
	
	int tManagerPost = 0;
	int tMemberPost = 0;
	tpCorps->GetMemberPost( nManagerID, tManagerPost );
	tpCorps->GetMemberPost( nMemberID, tMemberPost );	
	
	// 判断权限
	if ( !tpCorps->PostHasRight( tManagerPost, EM_GRANT_SETOFFICER ) )
	{
		return	ERROR_CORPS_HASNORIGHT;
	}	
	
	// 判读任命的对象是否是家族族长
	if ( tpFamily->GetRooter() != nMemberID )
	{
		return ERROR_CORPS_HASNORIGHT;
	}
	
	// 任命的职位不能比族长低
	if ( nPost > CORPS_POST_FAMILYHEADER )
	{
		return ERROR_CORPS_HASNORIGHT;		
	}
	
	if ( tManagerPost >= nPost || tManagerPost >= tMemberPost )
	{
		return ERROR_CORPS_HASNORIGHT;
	}	
	
	if ( tpCorps->GetPostNum( nPost ) >= tpCorps->GetPostLimitNum( nPost ) )
	{
		return ERROR_CORPS_SETPOST_POSTFULL;
	}	
		
	tpCorps->SetMemberPost( nMemberID, nPost, rOldPost );
	
	LogOrganSetPost( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), nManagerID, nMemberID, nPost, rOldPost )
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventContributeMoney	
//  Description:	捐献金钱
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsManager::OnEventContributeMoney( int nMemberID, int nMoney )
{ 
	CCorps *tpCorps = GetCorpsOfPlayer( nMemberID );
	LK_ASSERT( tpCorps != NULL, return -1 )
	int tRet = tpCorps->ContributeCorpsMoney( nMoney );
	if( tRet != SUCCESS ) 
	{
		return tRet;
	}	
	int tContribute = GetCorpsConfig()->GetContributePerGold(  )*nMoney;  
	tpCorps->AddCorpsMemberContribute( nMemberID, tContribute );
	return SUCCESS;						
}

// ***********************************************************
//  Function:		OnEventConsumeMoney	
//  Description:	消耗金钱
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsManager::OnEventConsumeMoney( int nManagerID, int nMoney )
{
	CCorps *tpCorps = GetCorpsOfPlayer( nManagerID );
	LK_ASSERT( tpCorps != NULL, return -1 )	

	if ( tpCorps->GetRooterID() != nManagerID )
	{
		int tPost = 0;
		tpCorps->GetMemberPost( nManagerID, tPost );
		if ( !tpCorps->PostHasRight( tPost, EM_MONEY_REIN ) )
		{
			return ERROR_CORPS_HASNORIGHT;
		}		
	} 
	
	if ( nMoney < 0 )
	{
		return ERROR_CORPS_MONEYINVALIDE;
	}
	
	int tRet = tpCorps->ConsumeCorpsMoney( nMoney );
	if ( tRet == CCorps::EM_MONEYNOTENOUGH )
	{
		return ERROR_CORPS_CONSUMEMONEY_MONEYNOTENGOUGH;
	}
	
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventLevelUPCorps	
//  Description:	家族升级
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsManager::OnEventLevelUPCorps( int nManagerID, int &rLevel, int &rMoney )
{
	CCorps *tpCorps = GetCorpsOfPlayer( nManagerID );
	LK_ASSERT( tpCorps != NULL, return -1 )	

	if ( tpCorps->GetRooterID() != nManagerID )
	{		
		return ERROR_CORPS_HASNORIGHT;	
	} 
	
	return tpCorps->LevelUPCorps( rLevel, rMoney );
}

// ***********************************************************
//  Function:		OnEventAddCorpsGlory	
//  Description:	增加家族荣耀
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsManager::OnEventAddCorpsGlory( int nCorpsID, int nGloryValue )
{
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( nCorpsID );
	LK_ASSERT( tpCorps != NULL, return -1 )
	tpCorps->AddCorpsGlory( nGloryValue );	
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventReduceCorpsGlory	
//  Description:	减少家族荣耀
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsManager::OnEventReduceCorpsGlory( int nCorpsID, int nGloryValue, bool &rLevelDown )
{  	
	CCorps *tpCorps = CCorpsManager::GetSingleton( ).GetCorpsByID( nCorpsID );
	LK_ASSERT( tpCorps != NULL, return -1 )
	rLevelDown = tpCorps->ReduceCorpsGlory( nGloryValue );
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventAddCorpsPVPScore	
//  Description:	增加家族pvp积分的接口
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsManager::OnEventAddCorpsPVPScore( int nCorpsID, int nScoreValue, bool &rLevelUP )
{
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( nCorpsID );
	LK_ASSERT( tpCorps != NULL, return -1 )
	rLevelUP = tpCorps->AddPvPScore( nScoreValue, tpCorps->GetPVPStarLevel() );
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventSetCorpsName	
//  Description:	设置成员称号
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsManager::OnEventSetCorpsMemberName( int nManagerID, int nMemberID, const char *pName )
{
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( nManagerID );
	LK_ASSERT( tpCorps != NULL, return -1 )
	
	// 权限检查
	int tManagerPost = 0;
	tpCorps->GetMemberPost( nManagerID, tManagerPost );
	
	int tMemberPost = 0;
	tpCorps->GetMemberPost( nMemberID, tMemberPost );
	
	// 普通成员没有权限
	if ( tManagerPost == CORPS_POST_MEMBER  )
	{
		return	ERROR_CORPS_HASNORIGHT;
	}
	
	// 如果不是自己给自己设置称号的话，只能给下级设置称号
	if ( nManagerID != nMemberID )
	{
		if ( tManagerPost >= tMemberPost )
		{
			return ERROR_CORPS_HASNORIGHT;
		}
	}	
	
	// 获取管理者的家族
	CFamily *tpFamily = tpCorps->GetMemberFamily( nManagerID );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "corps","[%s,%d][%s] Get family of player( roleid:%d ) in corps( corpsid:%d ) error! ", __LK_FILE__, __LINE__, __FUNCTION__, nManagerID, tpCorps->GetCorpsID() );
		return -1;
	}
	 
	// 如果是家族族长和军师的话只能给本家族成员设置称号
	if ( tManagerPost == CORPS_POST_COUNSELLOR || tManagerPost == CORPS_POST_FAMILYHEADER )
	{	
		if ( !tpFamily->HasMember( nMemberID ) )
		{
			return ERROR_CORPS_HASNORIGHT;
		}		
	}
	
	tpCorps->SetCorpsMemberName( nMemberID, pName );
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnEventFamilyContributeMoney	
//  Description:	家族捐献金钱
//  Output:			
//  Date:	08/31/2009
// **********************************************************
int CCorpsManager::OnEventFamilyContributeMoney( int nMemberID, int nMoney )
{
	 CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nMemberID );
	 LK_ASSERT( tpFamily != NULL, return -1 )
	 CCorps *tpCorps = GetCorpsByID( tpFamily->GetCorpsID() );
	 LK_ASSERT( tpCorps != NULL, return -1 )
	 
	 // 权限检查
	 if ( ( !tpFamily->HasPost( nMemberID, FAMILY_POST_HEADER, CFamily::EM_FAMILYACTION ) ) && ( !tpFamily->HasPost( nMemberID, FAMILY_POST_PRESBYTER, CFamily::EM_FAMILYACTION ) ) )
	 {
		return ERROR_CORPS_HASNORIGHT;
	 }
	
	 // 金钱检查
	 if ( tpFamily->GetFamilyMoney() < nMoney )
	 {
		return ERROR_CORPS_FAMILYMONEYNOTENOUGH;
	 }
	 
	 // 扣除家族金钱
	 tpFamily->ConsumeFamilyMoney( nMoney, false );
	 	 
	 // 增加军团金钱
	 tpCorps->ContributeCorpsMoney( nMoney );
	 
	 // 增加家族贡献度
	 int tContribute = mCorpsConfig.GetContributePerGold();
	 tpFamily->SetContribution( tContribute + tpFamily->GetContribution() );
	 
	 return SUCCESS; 
}

// ***********************************************************
//  Function:		OnEventCreateUnion	
//  Description:	军团结盟
//  Output:			
//  Date:	
// **********************************************************
int CCorpsManager::OnEventCreateUnion( int vHeadID, int *vCorpsID, int vNumber, char *vUnionName, char *vUnionNotice, int nRoleID)
{
	if ( vNumber < 1 )
	{
		return ERROR_UNION_LESSCORPS;
	}

	// 注意，军盟只能再接受除主军团外的9个子军团
	if ( vNumber > MAX_CORPS_UNION -1 )
	{
		return ERROR_UNION_REACHLIMIT;
	}
	CCorps *pHeadCorps = GetCorpsByID( vHeadID );
	if ( pHeadCorps == NULL )
	{
		return ERROR_UNION_NOHEADER;
	}

	/*
	const char *tpSQL = "insert into UMS_UNION( union_head, union_name, union_notice, corps0, corps1, corps2, corps3, \
		 corps4, corps5, corps6, corps7, corps8, corps9 ) values( %d, '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d )" ;
	*/
	
	CUnion *pUnion = (CUnion*)CGateObjManager::GetSingleton().CreateObject( OBJTYPE_UNION );
	if ( pUnion != NULL )
	{
		int tCorpsID[ MAX_CORPS_UNION ] = { 0 };	
		int tIndex = 0;
		pUnion->SetCorpNum( 1 );
		for ( int i = 0; i < vNumber && tIndex < (int) ARRAY_CNT(tCorpsID); i++ )
		{
			CCorps *pCorps = GetCorpsByID( vCorpsID[ i ] );
			if ( pCorps == NULL )
			{
				continue;
			}
			if ( pCorps->IsOwner() )
			{
				continue;
			}
			if ( pCorps->IsInBattle(TYPE_BATTLE_SCRABBLE, ST_Single|ST_InBid|ST_WaitBtl|ST_InBtl|ST_CityMap) )
			{
				continue;
			}
			tCorpsID[ tIndex++ ] = vCorpsID[ i ];
			pUnion->mCorpsList.push_back( vCorpsID[ i ] );
			pUnion->SetCorpNum( pUnion->GetCorpNum( ) + 1 );
			pUnion->SetMemberNum( pUnion->GetMemberNum( ) + pCorps->GetCorpsMemberNum( ) );
		}

		/*
		CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_create_union, vCharID, pUnion->get_id(), 0, SQL_NEED_CALLBACK|SQL_LATEST_INC_ID, INSERT, tpSQL,
			vHeadID, vUnionName, vUnionNotice, tCorpsID[ 0 ], tCorpsID[ 1 ], tCorpsID[ 2 ], tCorpsID[ 3 ], tCorpsID[ 4 ], tCorpsID[ 5 ], 
			tCorpsID[ 6 ], tCorpsID[ 7 ], tCorpsID[ 8 ], tCorpsID[ 9 ] ); 	
		*/

		pUnion->SetCoreID( vHeadID );
		pUnion->SetUnionName( vUnionName );
		pUnion->SetBulletin( vUnionNotice );
		pUnion->SetCity( pHeadCorps->GetBattleCity( ) );


		#ifdef _GLOBAL_NAME_
		CGateLogic::GetSingleton().SendCreateGroupProto2ERating(
			            nRoleID, vUnionName, em_union_group, pUnion->get_id());
		#else
		CreatUnionDBInfo(pUnion);
		#endif

		return SUCCESS;
	}

	return ERROR_UNION_REACHLIMIT;
}

int CCorpsManager::CreatUnionDBInfo(CUnion* pUnion)
{
	if ( pUnion == NULL) return -1;
	int tCorpsID[ MAX_CORPS_UNION ] = { 0 };	

	for ( int i = 0; i < pUnion->GetMemberNum(); ++i)
	{
		CCorps *pCorps = GetCorpsByID( pUnion->mCorpsList[i]);
		if ( pCorps == NULL ) continue;

		tCorpsID[i] = pUnion->mCorpsList[i];
	}

	#ifdef _GLOBAL_NAME_
	const char *tpSQL = "insert into UMS_UNION( union_id, union_head, union_name, union_notice, corps0, corps1, corps2, corps3, \
		 corps4, corps5, corps6, corps7, corps8 ) values( %d, %d, '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d )" ;
	

	CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_create_union, pUnion->GetUnionID(), pUnion->get_id(), 3, 
			SQL_NEED_CALLBACK|SQL_LATEST_INC_ID, INSERT, tpSQL, pUnion->GetUnionID(),
			pUnion->GetCoreID(), pUnion->GetUnionName(), pUnion->GetBulletin(), 
			tCorpsID[ 0 ], tCorpsID[ 1 ], tCorpsID[ 2 ], tCorpsID[ 3 ], tCorpsID[ 4 ], 
			tCorpsID[ 5 ], tCorpsID[ 6 ], tCorpsID[ 7 ], tCorpsID[ 8 ] ); 	

	#else
	const char *tpSQL = "insert into UMS_UNION( union_head, union_name, union_notice, corps0, corps1, corps2, corps3, \
		 corps4, corps5, corps6, corps7, corps8 ) values( %d, '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d )" ;
	

	CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_create_union, pUnion->GetUnionID(), pUnion->get_id(), 3, 
			SQL_NEED_CALLBACK|SQL_LATEST_INC_ID, INSERT, tpSQL,
			pUnion->GetCoreID(), pUnion->GetUnionName(), pUnion->GetBulletin(), 
			tCorpsID[ 0 ], tCorpsID[ 1 ], tCorpsID[ 2 ], tCorpsID[ 3 ], tCorpsID[ 4 ], 
			tCorpsID[ 5 ], tCorpsID[ 6 ], tCorpsID[ 7 ], tCorpsID[ 8 ] ); 	
	#endif

	return SUCCESS;
}



// 更新所有的联军信息
void CCorpsManager::UpDateAllUnionDBInfo( )
{
	// 军团未加载完成前不允许该操作
	if ( CCorpservice::GetSingleton().IsLaunched() == false )
	{
		return;
	}
	
	for ( UnionList::iterator it = m_UnionList.begin(); it != m_UnionList.end(); ++it )
	{
		CUnion *tpUnion = ( CUnion * )CGateObjManager::GetSingleton().GetObject( it->second );
		LK_ASSERT( tpUnion != NULL, continue );		
		UpdateUnionDBInfo( tpUnion );
	}
}




// ***********************************************************
//  Function:		UpDateAllCorpsDBInfo	
//  Description:	更新所有军团的数据库信息
//  Output:			
//  Date:	06/16/2009
// **********************************************************
void CCorpsManager::UpDateAllCorpsDBInfo( bool bDirectSave /* = false */ )
{	
	//time_t tNow = time( NULL );
	//struct tm* tpLocal = localtime( &tNow );

	for ( CorpsList::iterator it = m_tCorpsList.begin(); it != m_tCorpsList.end(); ++it )
	{
		CCorps *tpCorps = ( CCorps * )CGateObjManager::GetSingleton().GetObject( it->second );
		LK_ASSERT( tpCorps != NULL, continue )		
		
		if ( bDirectSave == true )
		{
			tpCorps->SetSaveStatus( CCorps::CORPS_ALLINFOCHANGED );
			tpCorps->GetCorpsShop()->SetSaveStatus( true );
			
		}					
			
		if ( tpCorps->GetSaveStatus() == CCorps::CORPS_ALLINFOCHANGED || tpCorps->GetCorpsShop()->GetSaveStatus() == true )
		{
			UpDateCorpsDBInfo( tpCorps );
			tpCorps->SetSaveStatus( CCorps::CORPS_INFONOCHANGE );
			tpCorps->GetCorpsShop()->SetSaveStatus( false );
			LOG_NOTICE( "corps","Save Corps:CorpsID:%d,CorspMoney:%d,CorpsHeaderID:%d",  tpCorps->GetCorpsID(), tpCorps->GetCorpsMoney(), tpCorps->GetRooterID() );
		}
	}
}

// ***********************************************************
//  Function:		DeleteCorpsOnDisbandTime	
//  Description:	删除解散时间已到的军团
//  Output:			
//  Date:	06/16/2009
// **********************************************************
void CCorpsManager::DeleteCorpsOnDisbandTime()
{
	if ( m_tCorpsList.empty() )
	{
		return;
	}
	for ( CorpsList::iterator it = m_tCorpsList.begin(); it != m_tCorpsList.end();  )
	{
		CCorps *tpCorps = ( CCorps * )CGateObjManager::GetSingleton().GetObject( it->second );
		if ( tpCorps == NULL )
		{
			++it;
			continue;
		}
		if ( !tpCorps->CheckCorpsDisbandTime() )
		{
			++it;
		}			
		else
		{
			int tCorpsMemberIDList[ CORPS_MAXMEMBERNUM ] = { 0 };
			int tNum = CORPS_MAXMEMBERNUM;
			GetCorpsMemberList( tpCorps, tCorpsMemberIDList, tNum );
			CCorpservice::GetSingleton().SendNotifyDisbandCorps( tCorpsMemberIDList, tNum );

			// 清除军团中家族的军团数据
			tpCorps->ClearCorpsInfoOfFamily();	 

			// 删除数据库的信息
			DeletCorpsDBInfo( tpCorps  );

			// 删除网关上的数据
		    m_tCorpsList.erase( it++ );

			// 删除军团对象
			CGateObjManager::GetSingleton().DestroyObject( tpCorps->get_id() );	 
		} 
	}				 			
}


// ***********************************************************
//  Function:		InsertCorpsConfig	
//  Description:	插入一条军团配置信息
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsConfig::InsertCorpsConfig( CCorpsConfigInfo& rCorpsConfig )
{
	mConfigList.insert( CorpsConfig::value_type( rCorpsConfig.mLevel, rCorpsConfig ) );
	return SUCCESS;
}


// ***********************************************************
//  Function:		SetCorpsPropertyConfig	
//  Description:	设置家族的基本属性配置
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsConfig::SetCorpsPropertyConfig( CCorpsPropertyConfigInfo &rPropertyConfig )
{
	memcpy( &mPropertyConfig, &rPropertyConfig, sizeof( mPropertyConfig ) );
	return SUCCESS;
}

// ***********************************************************
//  Function:		GetCorpsConfigByLevel	
//  Description:	获取某个等级的家族的配置信息
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsConfig::GetCorpsConfigByLevel( int nLevel, CCorpsConfigInfo& rCorpsConfig )
{
	CorpsConfig::iterator it = mConfigList.find( nLevel );
	if ( it == mConfigList.end() )
	{
		return -1;
	}
	
	rCorpsConfig.mGloryLimit			= (it->second).mGloryLimit;
	rCorpsConfig.mCounSellorNumLimit	= (it->second).mCounSellorNumLimit;
	rCorpsConfig.mFamilyNumLimit		= (it->second).mFamilyNumLimit;
	rCorpsConfig.mGeneralNumLimit		= (it->second).mGeneralNumLimit;
	rCorpsConfig.mPvPStarScore			= (it->second).mPvPStarScore;
	rCorpsConfig.mGloryLevelupNeeded	= (it->second).mGloryLevelupNeeded;
	rCorpsConfig.mBossRefineValueLimit	= (it->second).mBossRefineValueLimit;
	rCorpsConfig.mBossCDLimit			= (it->second).mBossCDLimit;
	return SUCCESS;
}

// ***********************************************************
//  Function:		GetFamilyNumLimitByLevel	
//  Description:	获取某个等级下军团的家族数量上限
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsConfig::GetFamilyNumLimitByLevel( int nLevel, int &rFamilyNum )
{  
	CorpsConfig::iterator it = mConfigList.find( nLevel );
	if ( it == mConfigList.end() )
	{
		return -1;				
	}
	
	rFamilyNum = (it->second).mFamilyNumLimit;	
	return SUCCESS;
}

// ***********************************************************
//  Function:		GetFamilyGloryLimitByLevel	
//  Description:	获取某个等级下军团的荣耀上限
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsConfig::GetCorpsGloryLimitByLevel( int nLevel, int &rGlory )
{ 
	CorpsConfig::iterator it = mConfigList.find( nLevel );
	if ( it == mConfigList.end() )
	{
		return -1;				
	}
	
	rGlory = (it->second).mGloryLimit;
	return SUCCESS;	
}

// ***********************************************************
//  Function:		GetGeneralNumLimitByLevel	
//  Description:	获取某个等级军团前锋将军的人数上限
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsConfig::GetGeneralNumLimitByLevel( int nLevel, int &rGeneralNum )
{  
	CorpsConfig::iterator it = mConfigList.find( nLevel );
	if ( it == mConfigList.end() )
	{
		return -1;
	}	
	rGeneralNum = (it->second).mGeneralNumLimit;
	return SUCCESS;				
}

// ***********************************************************
//  Function:		GetCounSellorNumLimitByLevel	
//  Description:	获取某个等级下军团的军师人数上限
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsConfig::GetCounSellorNumLimitByLevel( int nLevel, int &rCounSellorNum )
{
	CorpsConfig::iterator it = mConfigList.find( nLevel );
	if ( it == mConfigList.end() )
	{
		return -1;
	}		
	rCounSellorNum = (it->second).mCounSellorNumLimit;
	return SUCCESS;
}	

int CCorpsConfig::GetCorpsRefineValueLimitByLevel(int nLevel, int &rRefineValue)
{
	CorpsConfig::iterator it = mConfigList.find( nLevel );
	if ( it == mConfigList.end() )
	{
		return -1;
	}		
	rRefineValue = (it->second).mBossRefineValueLimit;
	return SUCCESS;
}

int CCorpsConfig::GetCorpsBossCDLimit( int nLevel, int &rBossCD )
{
	CorpsConfig::iterator it = mConfigList.find( nLevel );
	if ( it == mConfigList.end() )
	{
		return -1;
	}
	rBossCD = (it->second).mBossCDLimit;
	return SUCCESS;
}

// ***********************************************************
//  Function:		GetPvPStarScore	
//  Description:	获取某星级下升级到下一星级的需要的pvp积分
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsConfig::GetPvPStarScore( int nLevel, int &rPvPScore )
{
	CorpsConfig::iterator it = mConfigList.find( nLevel );
	if ( it == mConfigList.end() )
	{
		return -1;
	}		
	rPvPScore = (it->second).mPvPStarScore;
	return SUCCESS;	
}

// ***********************************************************
//  Function:		GetPvPStarScore	
//  Description:	获取某个等级到下一等级所需要的荣耀上限
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsConfig::GetGloryLevelupNeeded( int nLevel, int &rGlory )
{ 
	CorpsConfig::iterator it = mConfigList.find( nLevel );
	if ( it == mConfigList.end() )
	{
		return -1;
	}		
	rGlory = (it->second).mGloryLevelupNeeded;
	return	SUCCESS;		
}

// ***********************************************************
//  Function:		GetMoneyLevelUpNeeded	
//  Description:	获取某个等级到下一等级需要的金钱
//  Output:			
//  Date:	09/16/2009
// **********************************************************
int CCorpsConfig::GetMoneyLevelUpNeeded( int nLevel, int &rMoney )
{  
	CorpsConfig::iterator it = mConfigList.find( nLevel );
	if ( it == mConfigList.end() )
	{
		return -1;
	}		
	rMoney = (it->second).mMoneyLevelupNeeded;
	return	SUCCESS;				
}

// ***********************************************************
//  Function:		GetContributePerGold	
//  Description:	获取贡献一金锭的贡献度
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsConfig::GetContributePerGold()
{
	return mPropertyConfig.mContributePerGold;	
}

// ***********************************************************
//  Function:		GetScorePerFight	
//  Description:	获取每胜利一场的积分
//  Output:			
//  Date:	08/25/2009
// **********************************************************
int CCorpsConfig::GetScorePerFight()
{
	return 	mPropertyConfig.mScorePerFight;
}

int CCorpsManager::OnGetRoomList( int* vpList, int& nSize )
{
	if ( vpList == NULL )
	{
		return -1;
	}
	int tSize = nSize;
	nSize = 0;

	RoomList::iterator it = m_tRoomList.begin();
	for ( ; it != m_tRoomList.end() && nSize < tSize; ++ it )
	{
		vpList[ nSize ++ ] = *it;
	}
	return 0;
}

bool CCorpsManager::HasRoom( int nCorpsID )
{
	RoomList::iterator it = m_tRoomList.begin();
	for ( ; it != m_tRoomList.end(); ++ it )
	{
		if ( *it == nCorpsID )
		{
			return true;
		}
	}
	return false;
}

int CCorpsManager::OnDelRoom( int nCorpsID )
{
	RoomList::iterator it = m_tRoomList.begin();
	for ( ; it != m_tRoomList.end(); ++ it )
	{
		if ( *it == nCorpsID )
		{
			m_tRoomList.erase( it );
			break;
		}
	}
	return 0;
}




//void CCorpsManager::SendCorpsNpcList( int nServerID )
//{
//	CMessage MsgHead;	
//	CMessageNpcListNotify Notify;
//
//	BattleCityOwner::iterator it = m_tBattleCityOwner.begin();
//	for ( ; it != m_tBattleCityOwner.end(); ++ it )
//	{
//		CCorps* tpCorsp = GetCorpsByID( it->second );
//		if ( tpCorsp == NULL )
//			continue;
//		if ( tpCorsp->GetCorpsCountryID() != nServerID )
//			continue;
//
//		int tMapID = SCENENPCID( it->first, nServerID );
//		int tNpcID = tpCorsp->HasOwnThisMap( tMapID );
//
//		if ( tNpcID != INVALID_OBJ_ID )
//		{
//			PBCorpsNpc* tpNpc = Notify.add_corpsnpc();
//			tpNpc->set_corpsid( tpCorsp->GetCorpsID() );
//			tpNpc->set_corpsname( tpCorsp->GetCorpsName() );
//			tpNpc->set_npcid( tNpcID );
//			tpNpc->set_type( (int) true );
//			LOG_NOTICE( "corps", "ServerID[%d],Npc Own: CorpsName:%s,CorpsID:%d,NpcID:%d,Add:1", 
//				nServerID, tpCorsp->GetCorpsNameShow(), tpCorsp->GetCorpsID(), tNpcID );
//		}			
//	}
//
//	Notify.set_type( TYPE_CORPS );
//	MsgHead.set_msgpara( (int)&Notify );
//	MsgHead.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_NPCLIST );
//	CGateServer::GetSingleton().Send2Scene( nServerID, &MsgHead );
//}

void CCorpsManager::SendCorpsNpcSingle( int nServerID, int nNpcID, int nCorpsID, bool bAdd /* = true */ )
{
	CMessage MsgHead;	
	CMessageNpcListNotify Notify;

	CCorps* tpCorps =  GetCorpsByID( nCorpsID );
	if ( tpCorps == NULL )
	{
		return ;
	}

	PBCorpsNpc* tpNpc = Notify.add_corpsnpc();
	tpNpc->set_corpsid( tpCorps->GetCorpsID() );
	tpNpc->set_corpsname( tpCorps->GetCorpsName() );
	tpNpc->set_npcid( nNpcID );
	tpNpc->set_type( (int)bAdd );

	Notify.set_type( TYPE_CORPS );
	MsgHead.set_msgpara( (int)&Notify );
	MsgHead.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_NPCLIST );
	CGateServer::GetSingleton().Send2Scene( nServerID, &MsgHead );

	LOG_NOTICE( "corps", "ServerID:%d,Npc Own: CorpsName:%s,CorpsID:%d,NpcID:%d, Add:%d", 
		nServerID, tpCorps->GetCorpsNameShow(), tpCorps->GetCorpsID(), nNpcID, (int) bAdd );
}

// ***********************************************************
//  Function:		GetManagerOfRight	
//  Description:	获取军团中拥有某项权限的在线的职位最高的成员
//  Output:			
//  Date:	10/27/2009
// **********************************************************
CGatePlayer *CCorpsManager::GetManagerOfRight( CCorps *tpCorps, int nRight )
{		
	if ( tpCorps == NULL )
	{
		return NULL;
	}	

	int tRoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 };
	int tPostList[MAX_FAMILYMEMBER_NUM] = { 0 };
	int tContributeList[MAX_FAMILYMEMBER_NUM] = { 0 };
	int tNum = MAX_FAMILYMEMBER_NUM;
	int tRet = tpCorps->ListMemberOfRight( tRoleIDList, tPostList, tContributeList, tNum, nRight );
	if ( tRet != SUCCESS )
	{
		return NULL;
	}	

	int tManagerPost = 100;
	int tManagerID	 = 100;
	int tContribute = 0;
	CGatePlayer *tpManager = NULL;
	for ( int i = 0; i < tNum; ++i )
	{	  
		CGatePlayer *pPlayer = CPlayerManager::GetSingleton().ScenePlayer( tRoleIDList[i] );
		if ( pPlayer == NULL )
		{
			continue;
		}
		if ( tPostList[i] < tManagerPost )
		{
			tManagerPost = tPostList[i];
			tManagerID = tRoleIDList[i];
			tpManager = pPlayer;
			tContribute = tContributeList[i];
		}
		else if ( tPostList[i] == tManagerPost && tContributeList[i] > tContribute )
		{
			tManagerPost = tPostList[i];
			tManagerID = tRoleIDList[i];
			tpManager = pPlayer;
			tContribute = tContributeList[i];
		}
						
	}	
	return tpManager;
}


// 申请加入联军
int CCorpsManager::OnEventApplyUnion( int nApplyingID, CUnion *pUnion, PBCorpsBasicInfo &Info )
{
	LK_ASSERT( pUnion != NULL, return -1 );
	CCorps *pApplyCorps = GetCorpsOfPlayer( nApplyingID );	
	CCorps *pHeadCorps = CCorpsManager::GetSingleton().GetCorpsByID( pUnion->GetCoreID( ) );
	
	// 检查是否有家族
	if ( pApplyCorps == NULL || pHeadCorps == NULL )
	{
		return ERROR_UNION_APPLY_HASNOCORPS;
	}

	// 检查是否有权限
	if ( pApplyCorps->GetRooterID() != nApplyingID )
	{
		return ERROR_UNION_APPLY_HASNORIGHT;
	}

	// 检查是否处于解散冷却时间
	if ( pApplyCorps->GetCorpsStatus() == CCorps::EM_CORPS_STATUS_DISBAND )
	{
		return ERROR_UNION_APPLY_ONDISBANDSTATUS;
	}

	// 检查离开联军的冷却时间
	/*if( !pCorps->CheckLastTimeLeaveCorps() )
	{
		return ERROR_CORPS_JOINCORPS_INLEAVECOOLTIME;	
	}*/

	// 检查申请冷却时间
	/*if ( !pCorps->CheckLastTimeApplyCorps() )
	{
		return ERROR_CORPS_APPLY_INCOOLTIME;
	} */

	// 检查是否是同一国籍	
	if ( pApplyCorps->GetCorpsCountryID() != pHeadCorps->GetCorpsCountryID() )
	{
		return ERROR_UNION_APPLY_NOTINSAMECOUNTRY; 
	}

	if ( pApplyCorps->IsOwner() == true )
	{
		return ERROR_UNION_MAKE_ISOWNER;
	}

	// 检查是不是已经加入军团
	if ( pApplyCorps->GetUnionID() != 0 )
	{
		return ERROR_UNION_APPLY_HASINUNION;
	}

	if (pApplyCorps->IsInBattle(TYPE_BATTLE_SCRABBLE, ST_Single|ST_InBid|ST_WaitBtl|ST_InBtl|ST_CityMap))
	{
		// 处于主城NPC竞价、战争等待、战争中时的军团 不可申请
		return ERROR_UNOIN_APPLY_CITY_CONTENDER;
	}

	// 检查申请列表是否已满	
	if ( pUnion->mApplyList.IsFull() )
	{
		return ERROR_UNION_APPLY_APPLYLISTFULL;
	}

	// 检查是否已经在申请列表里面
	if ( pUnion->mApplyList.InList( pApplyCorps->GetCorpsID() ) )
	{
		return ERROR_UNION_APPLY_INAPPLYLIST;
	}

	// 插入到申请列表里面
	pUnion->mApplyList.Insert( pApplyCorps->GetCorpsID() );

	pApplyCorps->GetCorpsBasicInfo( Info );

	// 设置申请时间
	//pFamily->SetLastTimeApplyCorps( time(NULL) );	
	return SUCCESS;
}

/*
int CCorpsManager::OnEventListCorps( int nRoleID, int nCountryID, CorpsInfoList &pbCorpsList, int nCurPage, int &rTotalPage )
{  
	if ( m_tCorpsList.begin() == m_tCorpsList.end() )
	{
		return SUCCESS;
	}
	int tTotalCorpsNum = 0;
	for ( CorpsList::iterator it = m_tCorpsList.begin(); it != m_tCorpsList.end(); ++it )
	{	  
		CCorps *tpCorps = GetCorpsByID( it->first );
		if ( tpCorps == NULL )
		{
			LOG_ERROR( "corps"," can not find corps by id:%d ", it->first );
			continue;
		}		

		if ( tpCorps->GetCorpsCountryID() != nCountryID )
		{
			continue;
		}
		if ( tTotalCorpsNum >=nCurPage*FAMILYCORPS_MAXGETNUM && tTotalCorpsNum < ( nCurPage+1 )*FAMILYCORPS_MAXGETNUM )
		{
			PBCorpsBasicInfo *pbCorpsBasicInfo = pbCorpsList.add_corpslist();
			tpCorps->GetCorpsBasicInfo( *pbCorpsBasicInfo );
		}
		++tTotalCorpsNum;
	}
	rTotalPage = tTotalCorpsNum / FAMILYCORPS_MAXGETNUM;
	if ( tTotalCorpsNum % FAMILYCORPS_MAXGETNUM > 0 )
	{
		++rTotalPage;
	}
	return SUCCESS;
}
*/

int CCorpsManager::OnEventReplyUnionApply( int nReplyingID, int nCorpsID, int nResult, PBCorpsBasicInfo &vCorpsInfo )
{
	CCorps *tpReplyCorps = GetCorpsOfPlayer( nReplyingID );
	if ( tpReplyCorps == NULL )
	{
		LOG_ERROR( "corps"," can not find corps of player( roleid:%d ) ", nReplyingID );
		return -1;
	}

	CCorps *tpApplyCorps = GetCorpsByID( nCorpsID );
	if ( tpApplyCorps == NULL )
	{
		return ERROR_UNION_REPLYAPPLY_NOTEXIST;
	}	

	CUnion *tpUnion = GetUnionByID( tpReplyCorps->GetUnionID( ) );
	if ( tpUnion == NULL )
	{
		return ERROR_UNION_REPLYAPPLY_NOTEXIST;
	}

	if ( tpUnion->GetCoreID() != tpReplyCorps->GetCorpsID() )
	{
		return ERROR_CORPS_HASNORIGHT;
	}

	// 权限检查
	if ( tpReplyCorps->GetRooterID() != nReplyingID )
	{	 
		int tPost = 0;
		tpReplyCorps->GetMemberPost( nReplyingID, tPost );
		if ( !tpReplyCorps->PostHasRight( tPost, EM_APPROVE_APPLY ) )
		{
			return ERROR_CORPS_HASNORIGHT;
		}		
	}

	// 检查申请列表里面是否存在家族的申请
	if ( !tpUnion->mApplyList.InList( nCorpsID ) )
	{		
		return -1;		
	}

	tpUnion->mApplyList.Remove( nCorpsID );
	if ( nResult == 0 || nResult == 2 )
	{		
		return SUCCESS;
	}  		

	// 查看军团是否已经处于解散状态
	if ( tpApplyCorps->GetCorpsStatus() == CCorps::EM_CORPS_STATUS_DISBAND )
	{
		return ERROR_UNION_APPLY_ONDISBANDSTATUS;
	}

	// 查看是否有联军
	if ( tpApplyCorps->GetUnionID() != 0 )
	{
		return ERROR_CORPS_REPLYAPPLY_INCORPS;		
	}

	if (tpApplyCorps->IsInBattle(TYPE_BATTLE_SCRABBLE, ST_Single|ST_InBid|ST_WaitBtl|ST_InBtl|ST_CityMap))
	{
		// 处于主城NPC竞价、战争等待、战争中时的军团 不可申请
		return ERROR_UNOIN_REAPPLY_CITY_CONTENDER;
	}

	int tRet = tpUnion->InsertCorps( nCorpsID );
	if ( tRet == CUnion::CORPSHASEXIST )
	{
		return ERROR_UNION_REPLYAPPLY_INUNION;
	}

	if ( tRet == CUnion::CORPSLISTFULL )
	{
		return ERROR_UNION_REPLYAPPLY_MEMBERFULL;
	}

	tpApplyCorps->SetUnionID( tpUnion->GetUnionID( ) );
	tpApplyCorps->GetCorpsBasicInfo( vCorpsInfo );
	return SUCCESS;
}

int CCorpsManager::OnEventInviteCorps( int nCorpsRooterID, CCorps *pInviteCorps )
{	 
	if ( pInviteCorps == NULL )
	{
		return ERROR_UNION_INVITE_CORPSNOTEXIST;
	}
	if ( pInviteCorps->GetUnionID() != 0 )
	{
		return ERROR_UNION_INVITE_INUNION;
	}
	if ( pInviteCorps->GetCorpsStatus() == CCorps::EM_CORPS_STATUS_DISBAND )
	{
		return ERROR_UNION_INVITE_ONDISBANDSTATUS;
	} 
	if ( pInviteCorps->IsOwner() == true )
	{
		return ERROR_UNION_MAKE_ISOWNER;
	}
	if (pInviteCorps->IsInBattle(TYPE_BATTLE_SCRABBLE, ST_Single|ST_InBid|ST_WaitBtl|ST_InBtl|ST_CityMap))
	{
		// 不可邀请处于主城NPC竞价、战争等待、战争中时的军团
		return ERROR_UNOIN_INVITE_CITY_CONTENDER;
	}
	CCorps *tpSrcCorps = GetCorpsOfPlayer( nCorpsRooterID );
	LK_ASSERT( tpSrcCorps != NULL, return -1 );
	if ( tpSrcCorps->GetCorpsCountryID() != pInviteCorps->GetCorpsCountryID() )
	{	 
		return	ERROR_UNION_INVITE_NOTINOTHERCOUNTRY;
	}
	if ( tpSrcCorps->GetRooterID() != nCorpsRooterID )
	{
		int tPost = 0;
		tpSrcCorps->GetMemberPost( nCorpsRooterID, tPost );
		if ( !tpSrcCorps->PostHasRight( tPost, EM_INVITE_MEMBER ) )
		{
			return ERROR_CORPS_HASNORIGHT;
		}		
	}

	CUnion *tpUnion = GetUnionByID( tpSrcCorps->GetUnionID( ) );
	LK_ASSERT( tpUnion != NULL, return -1 );
	if ( tpUnion->mInviteList.IsFull( ) )
	{
		return ERROR_CORPS_INVITE_INVITELISTFULL;
	}
	if ( tpUnion->GetCoreID() != tpSrcCorps->GetCorpsID() )
	{
		return ERROR_CORPS_HASNORIGHT;
	}
	tpUnion->mInviteList.Insert( pInviteCorps->GetCorpsID() );
	return SUCCESS;	
}

// 答复联军邀请
int CCorpsManager::OnEventReplyUnionInvite( int nInvitedID, CUnion *pUnion, int nResult )
{
	LK_ASSERT( pUnion != NULL, return -1 );
	CCorps *tpInviteCorps = GetCorpsOfPlayer( nInvitedID );
	LK_ASSERT( tpInviteCorps != NULL, return -1 ); 
	LK_ASSERT( pUnion->mInviteList.InList( tpInviteCorps->GetCorpsID( ) ), return -1 );

	// 检查是否有权限
	if ( tpInviteCorps->GetRooterID() != nInvitedID )
	{			
		return ERROR_CORPS_HASNORIGHT;
	}

	/*if ( pUnion->GetCoreID() != tpInviteCorps->GetCorpsID() )
	{
		return ERROR_CORPS_HASNORIGHT;
	}*/

	// 从邀请列表里面删除
	pUnion->mInviteList.Remove( tpInviteCorps->GetCorpsID( ) );

	// 判断是否是忽略或者拒绝
	if ( nResult == FAMILY_REPLY_REFUSE || nResult == FAMILY_REPLY_IGNORE )
	{
		return SUCCESS;
	}  

	// 检查军团的状态,处于解散状态的军团不能加入
	if ( tpInviteCorps->GetCorpsStatus() == CCorps::EM_CORPS_STATUS_DISBAND )
	{
		return ERROR_UNION_REPLYINVITE_ONDISBANDSTATUS;
	}

	// 判断离开家族的冷却时间
	/*if ( !tpFamily->CheckLastTimeLeaveCorps() )
	{		
		return ERROR_CORPS_JOINCORPS_INLEAVECOOLTIME;
	}*/

	// 检查军团是否已经加入军盟
	if ( tpInviteCorps->GetUnionID() != 0 )
	{
		return ERROR_UNION_REPLYINVITE_INUNION;
	}

	if (tpInviteCorps->IsInBattle(TYPE_BATTLE_SCRABBLE, ST_Single|ST_InBid|ST_WaitBtl|ST_InBtl|ST_CityMap))
	{
		// 处于主城NPC竞价、战争等待、战争中时的军团 不可加入
		return ERROR_UNOIN_REINVITE_CITY_CONTENDER;
	}

	int tRet = pUnion->InsertCorps( tpInviteCorps->GetCorpsID() );
	if ( tRet == CUnion::CORPSHASEXIST )
	{
		return ERROR_UNION_REPLYINVITE_INUNION;
	}

	if ( tRet == CUnion::CORPSLISTFULL )
	{
		return ERROR_UNION_REPLYINVITE_MEMBERFULL;
	}	
	tpInviteCorps->SetUnionID( pUnion->GetUnionID() );
	return SUCCESS;
}

int CCorpsManager::OnEventLeaveUnion( int nRoleID )
{	
	CCorps *tpCorps = GetCorpsOfPlayer( nRoleID );
	LK_ASSERT( tpCorps != NULL, return -1 );
	if ( tpCorps->GetRooterID() != nRoleID )
	{
		return ERROR_CORPS_HASNORIGHT;
	}		 
	CUnion *tpUnion = GetUnionOfPlayer( nRoleID );
	LK_ASSERT( tpUnion != NULL, return -1 );	
	if ( tpUnion->GetCoreID() == tpCorps->GetCorpsID() )
	{
		return ERROR_UNION_LEAVE_ISUNIONHEADER;
	}

	// 处于战斗状态的子军团不能离开联军
	if ( tpCorps->IsInBattle(TYPE_BATTLE_SCRABBLE, ST_InBtl|ST_Union) )
	{
		return ERROR_UNION_CANOTLEAVEINBATTLE;		
	}	

	tpUnion->RemoveCorps( tpCorps->GetCorpsID() );
	tpCorps->SetUnionID( 0 );
	//tpFamily->SetLastTimeLeaveCorps( time( NULL ) );
	return SUCCESS;
}

int CCorpsManager::OnEventKickCorpsOfUnion( int nRooterID, int nCorpsID )
{ 
	CUnion *tpUnion = GetUnionOfPlayer( nRooterID );
	LK_ASSERT( tpUnion != NULL, return -1 );
	CCorps *tpHeadCorps = GetCorpsOfPlayer( nRooterID );
	LK_ASSERT( tpHeadCorps != NULL, return -1 );
	if ( tpUnion->GetCoreID() != tpHeadCorps->GetCorpsID() )
	{
		return ERROR_CORPS_HASNORIGHT;
	}
	if ( tpHeadCorps->GetRooterID() != nRooterID )
	{
		int tPost = 0;
		tpHeadCorps->GetMemberPost( nRooterID, tPost );
		if ( !tpHeadCorps->PostHasRight( tPost, EM_FIRE_MEMBER ) )
		{
			return ERROR_CORPS_HASNORIGHT;
		}		
	}

	// 处于战斗状态的军团不能离开联军
	if ( tpHeadCorps->IsInBattle(TYPE_BATTLE_SCRABBLE, ST_InBtl|ST_Union))
	{
		return ERROR_UNION_CANOTLEAVEINBATTLE;
	}

	CCorps *tpKickCorps = GetCorpsByID( nCorpsID );
	LK_ASSERT( tpKickCorps != NULL, return -1 );
	if ( tpUnion->GetCoreID( ) == nCorpsID )
	{
		return ERROR_UNION_KICKCORPS_SELFCORPS;
	}

	tpUnion->RemoveCorps( nCorpsID );	
	tpKickCorps->SetUnionID( 0 );
	//tpFamily->SetLastTimeLeaveCorps( time(NULL) );
	return SUCCESS;	
}

// 解散联军
int CCorpsManager::OnEventDisbandUnion( int nRooterID )
{
	CCorps *tpRootercorps = GetCorpsOfPlayer( nRooterID );
	LK_ASSERT( tpRootercorps != NULL, return -1 );
	CUnion *tpUnion = GetUnionByID( tpRootercorps->GetUnionID() );
	if ( tpUnion == NULL )
	{
		LOG_ERROR( "corps"," can not find union by ID:%d ", nRooterID );
		return -1;
	}	

	if ( tpUnion->GetCoreID() != tpRootercorps->GetCorpsID() )
	{
		return ERROR_UNION_DISBAND_NOTHEADER;
	}

	if ( tpRootercorps->GetRooterID() != nRooterID )
	{
		return ERROR_UNION_DISBAND_NOTHEADER;
	}

	// 军团联盟处于竞价，争夺和战斗状态不能解散
	if ( tpRootercorps->IsInBattle(TYPE_BATTLE_SCRABBLE, ST_InBid|ST_WaitBtl|ST_InBtl|ST_Union) )
	{
		return ERROR_UNION_DISBAND_INBATTLE;		
	}	

	DeleteUnionDBInfo( tpUnion );

	CCorps *tpHeadCorps = GetCorpsByID( tpUnion->GetCoreID() );
	if ( tpHeadCorps != NULL )
	{
		tpHeadCorps->SetUnionID( 0 );
	}
	for ( CUnion::CorpsVector::iterator it = tpUnion->mCorpsList.begin(); it != tpUnion->mCorpsList.end(); ++it )
	{
		CCorps *tpCorps = GetCorpsByID( *it );
		if ( tpCorps != NULL )
		{
			tpCorps->SetUnionID( 0 );
		}
	}
	CGateObjManager::GetSingleton().DestroyObject( tpUnion->get_id() );
	UnionList::iterator it = m_UnionList.find( tpUnion->GetUnionID() );
	if ( it != m_UnionList.end() )
	{
		m_UnionList.erase( it ) ;
	}

	return SUCCESS;
}


//// 定时发送参战通知 [2/3/2010 Macro]
//void CCorpsManager::SendBattleJoinNotice()
//{
//	BattleList::iterator it = m_tBattleList.begin();
//	for ( ; it != m_tBattleList.end(); ++ it )
//	{
//		// 非争霸赛不处理
//		if ( it->mType != TYPE_BATTLE_SCRABBLE )
//			continue;
//
//		// 已经开战的不需要
//		if ( it->mKey != INVITED_KEY_VALUE )
//			continue;
//
//		// 检查是否有争夺对象
//		if ( it->mMapID == INVALID_MAP_ID || it->mNpcID == INVALID_OBJ_ID )
//			continue;
//
//		// 参战双方军团数据都正常
//		CCorps* tpSrcCorps = GetCorpsByID( it->mAttCorpsID );
//		CCorps* tpDesCorps = GetCorpsByID( it->mDefCorpsID );
//		if ( tpSrcCorps == NULL || tpDesCorps == NULL )
//			continue;
//
//		int tNpcID = SCENENPCID( it->mNpcID, tpSrcCorps->GetCorpsCountryID() );
//
//		int tSrcType = ( tpSrcCorps->GetUnionID() == INVALID_OBJ_ID )? (int) emTypeCorps : (int) emTypeUnion;
//		int tDesType = ( tpDesCorps->GetUnionID() == INVALID_OBJ_ID )? (int) emTypeCorps : (int) emTypeUnion;
//
//		// 给功方发送
//		CCorpservice::GetSingleton().SendBattleJoinNotice( tpSrcCorps->GetCorpsCountryID(), tNpcID, 
//			tpDesCorps->GetCorpsName(), tpSrcCorps->GetRooterID(), tSrcType );
//		
//		// 给守方发送
//		CCorpservice::GetSingleton().SendBattleJoinNotice( tpDesCorps->GetCorpsCountryID(), tNpcID, 
//			tpSrcCorps->GetCorpsName(), tpDesCorps->GetRooterID(), tDesType );
//	}
//}




// ***********************************************************
//  Function:		GetUnionByID	
//  Description:	通过联军ID获取联军信息
//  Output:			
//  Date:	06/11/2009
// **********************************************************
CUnion * CCorpsManager::GetUnionByID( int nUnionID )
{	
	if ( m_UnionList.begin() == m_UnionList.end() )
	{
		return NULL;
	}

	for ( UnionList::iterator it = m_UnionList.begin(); it != m_UnionList.end(); ++it )
	{
		if ( it->first == nUnionID )
		{
			CUnion *tpUnion = ( CUnion * )CGateObjManager::GetSingleton().GetObject( it->second );
			return tpUnion;
		}		
	}
	return NULL;				
}

// ***********************************************************
//  Function:		GetUnionOfPlayer	
//  Description:	通过玩家ID获取联军
//  Output:			
//  Date:	06/11/2009
// **********************************************************
CUnion *CCorpsManager::GetUnionOfPlayer( int nRoleID )
{ 
	CCorps *pCorps = GetCorpsOfPlayer( nRoleID );
	if ( pCorps == NULL )
	{
		return NULL;
	}
	if ( pCorps->GetUnionID() == 0 )
	{
		return NULL;
	}
	CUnion *tpUnion = GetUnionByID( pCorps->GetUnionID() );
	return tpUnion;
}

// ***********************************************************
//  Function:		GetUnionByName	
//  Description:	通过军团名称获取军团
//  Output:			
//  Date:	06/12/2009
// **********************************************************
CUnion *CCorpsManager::GetUnionByName( const char *pUnionName )
{
	LK_ASSERT( pUnionName != NULL, return NULL );

	if ( m_UnionList.begin() == m_UnionList.end() )
	{
		return NULL;
	}

	for ( UnionList::iterator it = m_UnionList.begin(); it != m_UnionList.end(); ++it )
	{		
		CUnion *tpUnion = ( CUnion * )CGateObjManager::GetSingleton().GetObject( it->second );
		LK_ASSERT( tpUnion != NULL, continue );  				
		if ( strncmp( pUnionName, tpUnion->GetUnionName(), MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES -1 ) == 0 )
		{
			return tpUnion;
		}
	}
	return NULL;	
}

//////////////////////////////////////////////////////////////////////////
// 军盟数据库

// 更新联军的数据库信息
int CCorpsManager::UpdateUnionDBInfo( CUnion *pUnion )
{
	LK_ASSERT( pUnion != NULL, return -1 );	
	int tCorpsID[ MAX_CORPS_UNION ] = { 0 };
	unsigned int tIndex = 0;

	CUnion::CorpsVector::iterator it = pUnion->mCorpsList.begin();
	for ( ; it != pUnion->mCorpsList.end() && tIndex < ARRAY_CNT(tCorpsID); ++it )
	{
		tCorpsID[ tIndex++ ] = *it;
	}

	const char *tpSql = "update UMS_UNION set union_head=%d, union_name='%s', union_notice='%s', corps0=%d, corps1 =%d,	\
						corps2=%d, corps3=%d, corps4=%d, corps5=%d, corps6=%d, corps7=%d, corps8=%d \
						where union_id=%d ";
	CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_update_union, 0, 0, 3, SQL_NONE, UPDATE, 
		tpSql, pUnion->GetCoreID(), pUnion->GetUnionName(), pUnion->GetBulletin(), tCorpsID[ 0 ], tCorpsID[ 1 ], 
		tCorpsID[ 2 ], tCorpsID[ 3 ], tCorpsID[ 4 ], tCorpsID[ 5 ],	tCorpsID[ 6 ], tCorpsID[ 7 ], tCorpsID[ 8 ], pUnion->GetUnionID() );

	return SUCCESS;
}

// 删除联军的数据库信息
int CCorpsManager::DeleteUnionDBInfo( CUnion *pUnion )
{
	LK_ASSERT( pUnion != NULL, return -1 )
		const char *tpSql = "delete from UMS_UNION  where union_id=%d ";
	CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_delete_union, 0, 0, 3, SQL_NONE, DELETE, 
		tpSql, pUnion->GetUnionID() );	 
	return 0; 
}


// ***********************************************************
//  Function:		CreatCorpsDBInfo	
//  Description:	创建军团的数据库信息
//  Output:			
//  Date:	06/15/2009
// **********************************************************
int CCorpsManager::CreatCorpsDBInfo( CCorps *pCorps )
{
	LK_ASSERT( pCorps != NULL, return -1 )
		PBCorpsFamilyInfoList tCorpsFamilyList;

	char tPBBuff[MAX_COPRSPBBUFFER_LENGTH] = { 0 };
	char tEscasecapeBuff[MAX_COPRSPBBUFFER_LENGTH] = { 0 };

	pCorps->CreatCorpsFamilyPBInfo( tCorpsFamilyList );	
	int tRet = tCorpsFamilyList.SerializeToArray( tPBBuff, MAX_COPRSPBBUFFER_LENGTH );
	LK_ASSERT( tRet == true, return -1)
		char *tpEsBuufer = tEscasecapeBuff;
	int tCapRet = EscapeDataString( &tpEsBuufer, MAX_COPRSPBBUFFER_LENGTH, tPBBuff, tCorpsFamilyList.ByteSize() );
	LK_ASSERT( tCapRet != -1, return -1 )

		char tCorpsNameEscapeBuff[ MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES ] = { 0 };
	char *tpNameEscapePtr = tCorpsNameEscapeBuff;
	int tNameEscapeRet = EscapeDataString( &tpNameEscapePtr, MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES, ( char * )pCorps->GetCorpsName(), strlen( pCorps->GetCorpsName() ) );
	LK_ASSERT( tNameEscapeRet != -1, return -1 )

		// 序列化发布信息
	char tCorpsPublishBuff[MAX_COPRSPBBUFFER_LENGTH] = { 0 };
	PBCorpsPublishInfo tPublishInfo;
	pCorps->CreateCorpsPublishDB( tPublishInfo );				
	int tPubRet = tPublishInfo.SerializeToArray( tCorpsPublishBuff, MAX_COPRSPBBUFFER_LENGTH );
	LK_ASSERT( tPubRet == true, return -1 )				
		int tByteSize = tPublishInfo.ByteSize();	

	char tPublishEscapeBuff[MAXESCAPEBUFFLENGTH] = { 0 };		
	char *tpPublishEscapePtr =	tPublishEscapeBuff;
	int tPubEscapeRet = EscapeDataString( &tpPublishEscapePtr, MAXESCAPEBUFFLENGTH, tCorpsPublishBuff, tByteSize );
	LK_ASSERT( tPubEscapeRet != -1, return -1 )	

#ifdef _GLOBAL_NAME_

		const char *tpSql = "insert into UMS_CORPS( corps_id, corps_name, corps_notice, create_role_id, create_role_name, corps_money, family_info, country_id, corps_status, disband_time, corps_level, corps_creattime, corps_color, corps_refinevalue, corps_publish, corpsboss_score, corpscollect_active) \
							values(%d, '%s', '%s', %d, '%s', %d, '%s', %d, %d, %d, %d, %d, %d, %d ,'%s', %d, %d) ";
	CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_creatcorps, pCorps->get_id(), pCorps->GetRooterID(), 3, SQL_NEED_CALLBACK|SQL_LATEST_INC_ID, INSERT, 
		tpSql, pCorps->GetCorpsID(), tCorpsNameEscapeBuff, pCorps->GetBulletin(), pCorps->GetRooterID(), pCorps->GetRooterName(), pCorps->GetCorpsMoney(), tEscasecapeBuff
		,pCorps->GetCorpsCountryID(), pCorps->GetCorpsStatus(), pCorps->GetCorpsDisbandTime(), pCorps->GetCorpsLevel(), time( NULL ), pCorps->GetColor(), pCorps->GetBossRefineValue() ,tPublishEscapeBuff, pCorps->GetCorpsBossScore(), pCorps->GetCorpsCollectActive() );
#else

		const char *tpSql = "insert into UMS_CORPS( corps_name, corps_notice, create_role_id, create_role_name, corps_money, family_info, country_id, corps_status, disband_time, corps_level, corps_creattime, corps_color, corps_refinevalue ,corps_publish, corpsboss_score, corpscollect_active ) \
							values(  '%s', '%s', %d, '%s', %d, '%s', %d, %d, %d, %d, %d, %d, %d, '%s', %d, %d ) ";

	CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_creatcorps, pCorps->get_id(), pCorps->GetRooterID(), 3, SQL_NEED_CALLBACK|SQL_LATEST_INC_ID, INSERT, 
		tpSql, tCorpsNameEscapeBuff, pCorps->GetBulletin(), pCorps->GetRooterID(), pCorps->GetRooterName(), pCorps->GetCorpsMoney(), tEscasecapeBuff
		,pCorps->GetCorpsCountryID(), pCorps->GetCorpsStatus(), pCorps->GetCorpsDisbandTime(), pCorps->GetCorpsLevel(), time( NULL ), pCorps->GetColor(), pCorps->GetBossRefineValue() ,tPublishEscapeBuff, pCorps->GetCorpsBossScore(), pCorps->GetCorpsCollectActive());
#endif

	return SUCCESS;
} 

// ***********************************************************
//  Function:		UpDateCorpsDBInfo	
//  Description:	更新军团的数据库信息
//  Output:			
//  Date:	06/15/2009
// **********************************************************
int CCorpsManager::UpDateCorpsDBInfo( CCorps *pCorps )
{	
	// 军团未加载完成前不允许该操作
	if ( CCorpservice::GetSingleton().IsLaunched() == false )
	{
		return SUCCESS;
	}

	LK_ASSERT( pCorps != NULL, return -1 )

		if ( pCorps->GetSaveStatus() == CCorps::CORPS_ALLINFOCHANGED )
		{
			// 序列化家族信息
			PBCorpsFamilyInfoList tCorpsFamilyList;	
			char tPBBuff[MAX_COPRSPBBUFFER_LENGTH] = { 0 };
			char tEscasecapeBuff[MAX_COPRSPBBUFFER_LENGTH] = { 0 };

			//获取FamilyPBInfo到tCorpsFamilyList，得到Bin串到tPBBuff，
			pCorps->CreatCorpsFamilyPBInfo( tCorpsFamilyList );	
			int tRet = tCorpsFamilyList.SerializeToArray( tPBBuff, MAX_COPRSPBBUFFER_LENGTH );
			LK_ASSERT( tRet == true, return -1)
				char *tpEsBuufer = tEscasecapeBuff;
			//tPBBuff转义处理，存到tEscasecapeBuff
			int tCapRet = EscapeDataString( &tpEsBuufer, MAX_COPRSPBBUFFER_LENGTH, tPBBuff, tCorpsFamilyList.ByteSize() );
			LK_ASSERT( tCapRet != -1, return -1 )

			//pCorps->GetBulletin() => blob
			char tCorpsNoticeEscapeBuff[ CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES ] = { 0 };
			char *tpCorpsNoticeEscapePtr = tCorpsNoticeEscapeBuff;
			int tCorpsNoticeEscapeRet = EscapeDataString( &tpCorpsNoticeEscapePtr, CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES, ( char * )pCorps->GetBulletin(), strlen( pCorps->GetBulletin() ) );
			LK_ASSERT( tCorpsNoticeEscapeRet != -1, return -1 )	


			// 序列化发布信息
			char tCorpsPublishBuff[MAX_COPRSPBBUFFER_LENGTH] = { 0 };
			PBCorpsPublishInfo tPublishInfo;
			pCorps->CreateCorpsPublishDB( tPublishInfo );				
			int tPubRet = tPublishInfo.SerializeToArray( tCorpsPublishBuff, MAX_COPRSPBBUFFER_LENGTH );
			LK_ASSERT( tPubRet == true, return -1 )				
				int tByteSize = tPublishInfo.ByteSize();	

			char tPublishEscapeBuff[MAXESCAPEBUFFLENGTH] = { 0 };		
			char *tpPublishEscapePtr =	tPublishEscapeBuff;
			int tPubEscapeRet = EscapeDataString( &tpPublishEscapePtr, MAXESCAPEBUFFLENGTH, tCorpsPublishBuff, tByteSize );
			LK_ASSERT( tPubEscapeRet != -1, return -1 )	

				const char *tpSql = "update UMS_CORPS set corps_notice='%s', create_role_id=%d, create_role_name='%s', corps_money=%d, family_info ='%s',	\
									corps_status=%d, disband_time=%d, corps_glory=%d, war_winnum=%d, war_lostnum=%d, war_score=%d, war_starlevel=%d, war_bindnpcid=%d, war_bindmoney=%d, \
									war_campid=%d, war_welfaregettime=%d, corps_level=%d, \
									corps_roommoney=%d, corps_color = %d, corps_forbidtime=%d, corps_refinevalue=%d, corps_publish='%s', corpsboss_score=%d, corpscollect_active=%d where corps_id=%d ";
			CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_updatecorpsinfo, 0, 0, 3, SQL_NONE, UPDATE, 
				tpSql, tCorpsNoticeEscapeBuff, pCorps->GetRooterID(), pCorps->GetRooterName(), pCorps->GetCorpsMoney(), tEscasecapeBuff, pCorps->GetCorpsStatus(), 
				pCorps->GetCorpsDisbandTime() , pCorps->GetCorpsGlory(), pCorps->GetVictoryTimes(), pCorps->GetLoserTimers(), pCorps->GetPVPScore(), pCorps->GetPVPStarLevel(), pCorps->GetBidNpcID(),
				pCorps->GetBidMoney(), pCorps->GetCorpsCamp(), pCorps->GetCorpsWelfareGetTime( ), pCorps->GetCorpsLevel(), 
				pCorps->GetRoomMoney(), /*tEscInfoPB, */pCorps->GetColor(), pCorps->GetForbidTime(), pCorps->GetBossRefineValue(),tPublishEscapeBuff, pCorps->GetCorpsBossScore(), pCorps->GetCorpsCollectActive(), pCorps->GetCorpsID() );

		}

		// 判断家族商店是否需要存储
		if ( pCorps->GetCorpsShop()->GetSaveStatus() == true )
		{	
			char tCorpsShopEscapeBuff[MAXSHOPBYTELEN]	= { 0 };
			PBLeagueShopsListInfo tLeagueShop;
			pCorps->GetCorpsShop()->GetShopListInfo( tLeagueShop );				
			int tRet = tLeagueShop.SerializeToArray( tCorpsShopEscapeBuff, MAXSHOPBYTELEN );
			LK_ASSERT( tRet == true, return -1 )				
				int tByteSize = tLeagueShop.ByteSize();	

			char tShopEscapeBuff[MAXESCAPEBUFFLENGTH] = { 0 };		
			char *tpshopEscapePtr =	tShopEscapeBuff;
			int tShopEscapeRet = EscapeDataString( &tpshopEscapePtr, MAXESCAPEBUFFLENGTH, tCorpsShopEscapeBuff, tByteSize );
			LK_ASSERT( tShopEscapeRet != -1 , return -1 )
				const char *tpStmtfmt = "update UMS_CORPS set corps_shopinfo='%s' where corps_id=%d";
			CGateLogic::GetSingleton().ExecuteSql( 1, 0, 0, 3, SQL_NONE, UPDATE, tpStmtfmt, tShopEscapeBuff, pCorps->GetCorpsID() );	
		}	
		return SUCCESS;
}

// ***********************************************************
//  Function:		DeletCorpsDBInfo	
//  Description:	删除军团的数据库信息
//  Output:			
//  Date:	06/15/2009
// **********************************************************
int CCorpsManager::DeletCorpsDBInfo( CCorps *pCorps )
{
	LK_ASSERT( pCorps != NULL, return -1 )
		LogOrganDisband( ORGANLOGTYPE_CORPS, pCorps->GetCorpsID(), pCorps->GetRooterID(), 1, 0 )
		const char *tpSql = "delete from UMS_CORPS  where corps_id=%d ";
	CGateLogic::GetSingleton( ).ExecuteSql( em_dbsession_delcorps, 0, 0, 3, SQL_NONE, DELETE, 
		tpSql, pCorps->GetCorpsID( ) );	 
	return 0; 
}


//int CCorpsManager::OnFindBattleCity( int nMapID, int& nCorpsID )
//{
//	nCorpsID = CBidMgr::GetSingleton().FindCityOwnerByMapID(nMapID);
//	return 0;
//}

// ***********************************************************
//  Function:		DisbandCorps	
//  Description:	更具军团ID解散军团
//  Output:			
//  Date:	09/20/2010
// **********************************************************
void CCorpsManager::DisbandCorps( CCorps *pCorps )
{	
	if ( pCorps == NULL )
	{
		return;
	}

	// 清除军团中家族的军团数据
	pCorps->ClearCorpsInfoOfFamily();	 

	// 删除数据库的信息
	DeletCorpsDBInfo( pCorps  );

	// 删除军团对象
	CGateObjManager::GetSingleton().DestroyObject( pCorps->get_id() );	 
}

// ***********************************************************
//  Function:		RemoveCorpsFromList	
//  Description:	从列表中移除军团
//  Output:			
//  Date:	09/21/2010
// **********************************************************
int CCorpsManager::RemoveCorpsFromList( int nCorpsID )
{
	m_tCorpsList.erase( m_tCorpsList.find( nCorpsID ) );
	return SUCCESS;
}

// ***********************************************************
//  Function:		DeleteCorpsWithNotify	
//  Description:	删除军团并且发送通知
//  Output:			
//  Date:	12/01/2010
// **********************************************************
void CCorpsManager::DeleteCorpsWithNotify( CCorps *pCorps )
{
	LK_ASSERT( pCorps != NULL, return )
	
	// 回滚竞价信息
	CBidMgr::GetSingleton().OnEventRollCorpsNpc( pCorps->GetCorpsID(), INVALID_OBJ_ID, 0 );	

				
	// 发送竞价通知
	int tCorpsMemberIDList[ CORPS_MAXMEMBERNUM ] = { 0 };
	int tNum = CORPS_MAXMEMBERNUM;
	CCorpsManager::GetSingleton().GetCorpsMemberList( pCorps, tCorpsMemberIDList, tNum );
	CCorpservice::GetSingleton().SendNotifyDisbandCorps( tCorpsMemberIDList, tNum );

	// 清除军团中家族的军团数据
	pCorps->ClearCorpsInfoOfFamily();	 

	// 删除数据库的信息
	CCorpsManager::GetSingleton().DeletCorpsDBInfo( pCorps  );

	// 删除军团对象
	CGateObjManager::GetSingleton().DestroyObject( pCorps->get_id() );	 
}

// ***********************************************************
//  Function:		DeleteUnionWithNotify	
//  Description:	删除军盟并且发送通知
//  Output:			
//  Date:	12/01/2010
// **********************************************************
void CCorpsManager::DeleteUnionWithNotify( CUnion *pUnion, int nRooterID )
{	
	LK_ASSERT( pUnion != NULL, return )	
	
	// 发送通知
	int tCorpsID[ MAX_CORPS_UNION ] = { 0 };
	int tNum = 1;	
	if ( pUnion != NULL )
	{
		tCorpsID[ 0 ] = pUnion->GetCoreID( );
		for ( CUnion::CorpsVector::iterator it = pUnion->mCorpsList.begin(); it != pUnion->mCorpsList.end() && tNum < ( int )ARRAY_CNT(tCorpsID); ++it )
		{
			tCorpsID[ tNum++ ] = *it;
		}
	}	 	
	CCorpservice::GetSingleton().SendDismissUnionNotify( 0, nRooterID, tCorpsID, tNum );	
	
	// 删除信息
	DeleteUnionDBInfo( pUnion );
	CCorps *tpHeadCorps = GetCorpsByID( pUnion->GetCoreID() );
	if ( tpHeadCorps != NULL )
	{
		tpHeadCorps->SetUnionID( 0 );
	}
	for ( CUnion::CorpsVector::iterator it = pUnion->mCorpsList.begin(); it != pUnion->mCorpsList.end(); ++it )
	{
		CCorps *tpCorps = GetCorpsByID( *it );
		if ( tpCorps != NULL )
		{
			tpCorps->SetUnionID( 0 );
		}
	}
	UnionList::iterator it = m_UnionList.find( pUnion->GetUnionID() );
	if ( it != m_UnionList.end() )
	{
		m_UnionList.erase( it ) ;
	}
	CGateObjManager::GetSingleton().DestroyObject( pUnion->get_id() );
}
