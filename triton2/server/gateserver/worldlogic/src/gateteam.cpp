
#include "entity.h"
#include "teamtype.h"
#include "gateteam.h"
#include "gatelogic.h"
#include "gateobj_manager.h"
#include "servertool.h"


//////////////////////////////////////////////////////////////////////////
// CBuffList
//////////////////////////////////////////////////////////////////////////

CBuffList::CBuffList()
{
	if (CObj::msCreateMode == 1 )
	{
		mBuffList.initailize();
	}
}

CBuffList::~CBuffList()
{

}

//////////////////////////////////////////////////////////////////////////
// CGateTeamMemeber
//////////////////////////////////////////////////////////////////////////

CGateTeamMember::CGateTeamMember()
{
	if (CObj::msCreateMode == 1 )
	{
		Initialize();
	}else
	{
		Resume();
	}
}

CGateTeamMember::~CGateTeamMember()
{
	
}

int CGateTeamMember::Initialize()
{
	mCharID = 0;
	mName[ 0 ] = '\0';
	mFace = 0;
	mSex = 0;
	mMetier = 0;
	mLevel = 0;
	mHP = 0;
	mMP = 0;
	mLineID = 0;
	mMapID = 0;
	mPosX = 0;
	mPosY = 0;
	mStatus = 0;
	mNationality = 0;	
	mRepetionMapIndex = 0;
	m_nOrder = 0;
	return SUCCESS;
}

int CGateTeamMember::Resume()
{
	return SUCCESS;
}

CGateTeam::CGateTeam()
{
	if ( CObj::msCreateMode == 1 )
	{
		Initialize();
	}else
	{
		Resume();
	}
}

CGateTeam::~CGateTeam()
{
	mGateTeam.clear();
}

int CGateTeam::Initialize()
{
	mHeadID = 0;
	mHeadName[ 0 ] = '\0';
	mMode = RANDOM;
	mReqList.initailize();
	mGateTeam.initailize();
	return 0;	
}

int CGateTeam::Resume()
{
	return 0;	
}

int CGateTeam::AddMember( CGatePlayer* pPlayer )
{
	if ( pPlayer == NULL )
	{
		return -1;
	}
	
	TEAM_TYPE::iterator it = mGateTeam.find( pPlayer->mCharID );
	if ( it != mGateTeam.end() )
	{
		// has in team
		return -1;
	}

	CGateTeamMember* tpMember = ( CGateTeamMember* ) CGateObjManager::GetSingleton().CreateObject( OBJTYPE_MEMBER );
	if ( tpMember == NULL )
	{
		LOG_ERROR( "gateteam", "create object failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	// evaluate object value
	tpMember->SetCharID( pPlayer->mCharID );
	tpMember->SetName( pPlayer->mCharName );
	tpMember->SetFace( pPlayer->mFaceType );
	tpMember->SetSex( pPlayer->mSexID );
	tpMember->SetLevel( pPlayer->mLevel );
	tpMember->SetMetier( pPlayer->mMetierID );
	tpMember->SetNationality( pPlayer->mCountryID );
	tpMember->SetLineID( pPlayer->mLineID );
	tpMember->SetMapID( pPlayer->mMapID );
	int nOrder = GetNewOrderInTeam();
	tpMember->SetOrderInTeam(nOrder);

	mGateTeam.insert( TEAM_TYPE::value_type( tpMember->GetCharID(), tpMember->get_id() ) );

	LOG_INFO( "gateteam", ">>>Team Action: member ( %s:%d ) join.", tpMember->GetName(), tpMember->GetCharID() );

	return SUCCESS;
}

int CGateTeam::DelMember( int vCharID )
{
	TEAM_TYPE::iterator it = mGateTeam.find( vCharID );
	if ( it == mGateTeam.end() )
	{
		return -1;
	}

	LOG_INFO( "gateteam", ">>>Team Action: member ( %d ) leave.", vCharID );

	CGateObjManager::GetSingleton().DestroyObject( it->second );
	mGateTeam.erase( it );
	return SUCCESS;
}

int CGateTeam::DelTeam()
{
	TEAM_TYPE::iterator it = mGateTeam.begin();
	for ( ; it != mGateTeam.end(); ++ it )
	{
		CGateObjManager::GetSingleton().DestroyObject( it->second );
	}

	LOG_INFO( "gateteam", ">>>Team Action: team ( %d ) end.", get_id() );
	mGateTeam.clear();
	return SUCCESS;
}

int CGateTeam::GetMemberList( int* pList, int vNum, int& vRtnNum )
{
	if ( pList == NULL )
	{
		return -1;
	}
	
	vRtnNum = 0;
	TEAM_TYPE::iterator it = mGateTeam.begin();
	for ( ; it != mGateTeam.end(); ++ it )
	{
		if ( vRtnNum < vNum )
		{
			pList[ vRtnNum ++ ] = it->first;
		}
	}
	
	return SUCCESS;
}

int CGateTeam::MakeTeamPB( PBTeamInfo* pInfo )
{
	if ( pInfo == NULL )
	{
		return -1;
	}
	
	TEAM_TYPE::iterator it = mGateTeam.begin();
	for ( ; it != mGateTeam.end(); ++ it )
	{
		CGateTeamMember* tpMember = ( CGateTeamMember* ) CGateObjManager::GetSingleton().GetObject( it->second );
		if ( tpMember == NULL )
		{
			continue;
		}
		
		PBTeamItem* tpItem = pInfo->add_members();
		tpItem->set_charid( tpMember->GetCharID() );
		tpItem->set_name( tpMember->GetName() );
		tpItem->set_face( tpMember->GetFace() );
		tpItem->set_level( tpMember->GetLevel() );
		tpItem->set_sex( tpMember->GetSex() );
		tpItem->set_metier( tpMember->GetMetier() );
		tpItem->set_hp( tpMember->GetHP() );
		tpItem->set_mp( tpMember->GetMP() );
		tpItem->set_nationality( tpMember->GetNationality() );
		tpItem->set_lineid( tpMember->GetLineID() );
		tpItem->set_mapid( tpMember->GetMapID() );
		tpItem->set_posx( tpMember->GetPosX() );
		tpItem->set_posy( tpMember->GetPosY() );
		tpItem->set_status( tpMember->GetStatus() );
		tpItem->set_repetionmapindex( tpMember->GetRepetionMapIndex( ) );
		tpMember->GetBuffList().GetTlvBuff( tpItem->mutable_bufflist() );
		tpItem->set_order(tpMember->GetOrderInTeam());
	}
	
	pInfo->set_headercharid( mHeadID );
	pInfo->set_distributemode( mMode );
	pInfo->set_headname( mHeadName );
	pInfo->set_count( GetCount() );
	pInfo->set_teamid( get_id() );
	
	return SUCCESS;
}

int CGateTeam::RefreshMemberData( PBTeamInfo* pInfo )
{
	if ( pInfo == NULL )
	{
		return -1;
	}

	for ( int i = 0; i < pInfo->members_size(); ++ i )
	{
		PBTeamItem* tpItem = pInfo->mutable_members( i );
		TEAM_TYPE::iterator it = mGateTeam.begin();
		for ( ; it != mGateTeam.end(); ++ it )
		{
			if ( (int)tpItem->charid() != it->first )
			{
				continue;
			}

			CGateTeamMember* tpMember = ( CGateTeamMember* ) CGateObjManager::GetSingleton().GetObject( it->second );
			if ( tpMember == NULL )
			{
				LOG_ERROR( "gateteam", "object invalid,id=%d.[%s:%d]", it->second, __LK_FILE__, __LINE__ );
				continue;
			}
			
			tpMember->SetLevel( tpItem->level() );
			tpMember->SetLineID( tpItem->lineid() );
			tpMember->SetMapID( tpItem->mapid() );
			tpMember->SetPosX( tpItem->posx() );
			tpMember->SetPosY( tpItem->posy() );
			tpMember->SetMetier( tpItem->metier() );
			tpMember->SetMP( tpItem->mp() );
			tpMember->SetHP( tpItem->hp() );
			tpMember->SetStatus( tpItem->status() );
			tpMember->SetNationality( tpItem->nationality() );
			tpMember->SetRepetionMapIndex( tpItem->repetionmapindex( ) );

			// update team member buff list
			tpMember->GetBuffList().CreatFromTlv( tpItem->mutable_bufflist() );
		}
	}
	
	return SUCCESS;
}

int CGateTeam::MakeMemberPB( int vCharID, PBTeamItem* pItem )
{
	if ( pItem == NULL )
	{
		return -1;
	}

	TEAM_TYPE::iterator it = mGateTeam.find( vCharID );
	if ( it == mGateTeam.end() )
	{
		return -1;
	}

	CGateTeamMember* tpMember = ( CGateTeamMember* ) CGateObjManager::GetSingleton().GetObject( it->second );
	if ( tpMember == NULL )
	{
		LOG_ERROR( "gateteam", "get object failed.id=%d,[%s:%d]", it->second, __LK_FILE__, __LINE__ );
		return -1;
	}

	pItem->set_charid( tpMember->GetCharID() );
	pItem->set_name( tpMember->GetName() );
	pItem->set_face( tpMember->GetFace() );
	pItem->set_level( tpMember->GetLevel() );
	pItem->set_metier( tpMember->GetMetier() );
	pItem->set_sex( tpMember->GetSex() );
	pItem->set_hp( tpMember->GetHP() );
	pItem->set_mp( tpMember->GetMP() );
	pItem->set_nationality( tpMember->GetNationality() );
	pItem->set_lineid( tpMember->GetLineID() );
	pItem->set_mapid( tpMember->GetMapID() );
	pItem->set_posx( tpMember->GetPosX() );
	pItem->set_posy( tpMember->GetPosY() );
	pItem->set_status( (int) emOnLine );
	pItem->set_repetionmapindex( tpMember->GetRepetionMapIndex( ) );
	pItem->set_order(tpMember->GetOrderInTeam());

	// add team member buff list
	tpMember->GetBuffList( ).GetTlvBuff( pItem->mutable_bufflist() );

	return SUCCESS;
}

int CGateTeam::MakePbTeamMember( int vCharID, PBMember* pMember )
{
	if ( pMember == NULL )
	{
		return -1;
	}

	TEAM_TYPE::iterator it = mGateTeam.find( vCharID );
	if ( it == mGateTeam.end() )
	{
		return -1;
	}

	CGateTeamMember* tpMember = ( CGateTeamMember* ) CGateObjManager::GetSingleton().GetObject( it->second );
	if ( tpMember == NULL )
	{
		LOG_ERROR( "gateteam", "get object failed.id=%d,[%s:%d]", it->second, __LK_FILE__, __LINE__ );
		return -1;
	}

	pMember->set_charid( tpMember->GetCharID() );
	pMember->set_name( tpMember->GetName() );
	pMember->set_face( tpMember->GetFace() );
	pMember->set_level( tpMember->GetLevel() );
	pMember->set_metier( tpMember->GetMetier() );
	pMember->set_sex( tpMember->GetSex() );
	pMember->set_hp( tpMember->GetHP() );
	pMember->set_mp( tpMember->GetMP() );
	pMember->set_lineid( tpMember->GetLineID() );
	pMember->set_mapid( tpMember->GetMapID() );
	pMember->set_xpos( tpMember->GetPosX() );
	pMember->set_ypos( tpMember->GetPosY() );
	pMember->set_commstate( tpMember->GetStatus() );
	pMember->set_repetionmapindex( tpMember->GetRepetionMapIndex( ) );

	// add team member buff list
	tpMember->GetBuffList( ).GetTlvBuff( pMember->mutable_bufflist() );

	pMember->set_order(tpMember->GetOrderInTeam());

	return SUCCESS;
}

void CGateTeam::PrintTeam()
{
	LOG_INFO( "gateteam", "---------------Team Member List--------------------" );
	LOG_INFO( "gateteam", "Header:%d, CountNum:%d", mHeadID, GetCount() );
	TEAM_TYPE::iterator it = mGateTeam.begin();
	for ( ; it != mGateTeam.end(); ++ it )
	{
		CGateTeamMember* tpMember = ( CGateTeamMember* ) CGateObjManager::GetSingleton().GetObject( it->second );
		if ( tpMember == NULL )
		{
			LOG_ERROR( "gateteam", "get object failed.id=%d,[%s:%d]", it->second, __LK_FILE__, __LINE__ );
			continue;
		}

		LOG_INFO( "gateteam", "name:%s,charid:%d,lineid:%d,mapid:%d", tpMember->GetName(), 
			tpMember->GetCharID(), tpMember->GetLineID(), tpMember->GetMapID() );
	}
	LOG_INFO( "gateteam", "---------------------------------------------------" );
}

int CGateTeam::InsertReq( int nCharID )
{
	REQUEST_LIST::iterator it = mReqList.begin();
	for ( ; it != mReqList.end(); ++ it )
	{
		if ( *it == nCharID )
		{
			return -1;
		}
	}
	mReqList.push_back( nCharID );
	return SUCCESS;
}

int CGateTeam::RemoveReq( int nCharID )
{
	REQUEST_LIST::iterator it = mReqList.begin();
	for ( ; it != mReqList.end(); ++ it )
	{
		if ( *it == nCharID )
		{
			mReqList.erase( it );
			break;
		}
	}
	return SUCCESS;
}

int CGateTeam::GetNewOrderInTeam()
{
	int nOrder = TEAM_ORDER_FIRST;
	for (; nOrder <= TEAM_NUMBER_MAX; nOrder++)
	{
		bool bExist = false;
		TEAM_TYPE::iterator iter = mGateTeam.begin();
		for (; iter != mGateTeam.end(); ++iter)
		{
			CGateTeamMember* pMember = (CGateTeamMember*)CGateObjManager::GetSingleton().GetObject(iter->second);
			if (pMember == NULL)
			{
				continue;
			}
			if (pMember->GetOrderInTeam() == nOrder)
			{
				bExist = true;
				break;
			}
		}

		if (!bExist)
		{
			return nOrder;
		}
	}

	return 0;
}