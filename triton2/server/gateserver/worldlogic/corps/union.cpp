//=========================================================================
// union.cpp
//=========================================================================
#include "union.h"
#include "corpsmanager.h"
#include "corps.h"

//-------------------------------------------------------------------------
void CUnion::GetUnionBasicInfo( PBUnionBasicInfo *vInfo )
{
	if ( vInfo == NULL )
		return;
	vInfo->set_unionid( mUnionID );
	vInfo->set_unionname( mUnionName );

	int tCorpsCount = 0;
	int tMemberCount = 0;

	CCorps *tpHeadCorps = CCorpsManager::GetSingleton().GetCorpsByID( mCoreID );
	if ( tpHeadCorps != NULL )
	{
		vInfo->set_coreid( mCoreID );
		vInfo->set_corename( tpHeadCorps->GetCorpsName() );
		vInfo->set_rooterid( tpHeadCorps->GetRooterID() );
		vInfo->set_rootername( tpHeadCorps->GetRooterName() );
		vInfo->set_cityid( tpHeadCorps->GetBattleCity() );
		vInfo->set_money( tpHeadCorps->GetCorpsMoney() );
		tCorpsCount++;
		tMemberCount += tpHeadCorps->GetCorpsMemberNum();
		int tMapList[ MAX_CORPS_NPCOWNER ] = { 0 };
		int tMapNum = ARRAY_CNT( tMapList );
		tpHeadCorps->GetMapList( tMapList, tMapNum );
		for ( int i = 0; i < tMapNum; i++ )
		{
			vInfo->add_mapid( tMapList[ i ] );
		}
	}	
	for ( CUnion::CorpsVector::iterator it = mCorpsList.begin(); it != mCorpsList.end(); ++it )
	{
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( *it );
		if ( tpCorps != NULL )
		{
			tCorpsCount++;
			tMemberCount += tpCorps->GetCorpsMemberNum();
			int tMapList[ MAX_CORPS_NPCOWNER ] = { 0 };
			int tMapNum = ARRAY_CNT( tMapList );
			tpCorps->GetMapList( tMapList, tMapNum );
			for ( int i = 0; i < tMapNum; i++ )
			{
				vInfo->add_mapid( tMapList[ i ] );
			}
		}
	}
	//vInfo->set_mapnum( tMapCount );
	vInfo->set_corpsnum( tCorpsCount );
	vInfo->set_membernum( tMemberCount );
}


int CUnion::InsertBattleMember( int nRoleID )
{
	if ( mUnionBattleList.size() >= MAX_BATTLESIZE )
	{
		return -1;
	}
	UnionBattleList::iterator it = mUnionBattleList.begin();
	for ( ; it != mUnionBattleList.end(); ++ it )
	{
		if ( *it == nRoleID )
		{
			mUnionBattleList.erase( it );
			break;
		}
	}
	mUnionBattleList.push_back( nRoleID );
	return 0;
}

// 删除军联盟参战人员 [3/14/2010 GaoHong]
int CUnion::DeleteBattleMemberByCorpsID( int nCorpsID )
{
	if ( nCorpsID == 0 )
	{
		return -1;
	}

	UnionBattleList::iterator it = mUnionBattleList.begin();
	for ( ; it != mUnionBattleList.end(); )
	{
		CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( *it );
		if ( tpCorps == NULL )
		{
			++ it;
			continue;
		}
		if ( tpCorps->GetCorpsID() == nCorpsID )
		{
			it = mUnionBattleList.erase( it );
		}
		else
		{
			++ it;
		}
	}

	return 0;
}

int CUnion::DeleteBattleMemberByRoleID( int nRoleID )
{
	if ( nRoleID == 0 )
	{
		return -1;
	}

	UnionBattleList::iterator it = mUnionBattleList.begin();
	for ( ; it != mUnionBattleList.end(); ++ it)
	{
		if ( *it == nRoleID )
		{
			mUnionBattleList.erase(it);
			break;
		}
	}

	return 0;
}

int CUnion::ClearBattleList()
{
	mUnionBattleList.clear();
	return SUCCESS;
}

bool CUnion::IsInBattleList( int nRoleID )
{
	UnionBattleList::iterator it = mUnionBattleList.begin();
	for ( ; it != mUnionBattleList.end(); ++ it )
	{
		if ( *it == nRoleID )
			return true;
	}
	return false;
}

int CUnion::GetBattleMemberList( int* vpList, int& nSize )
{
	if ( vpList == NULL )
	{
		return -1;
	}

	int tSize = nSize;
	nSize = 0;

	int tMemberList[ CORPS_BATTLE_NUM ] = { 0 };
	int tArrayCnt = ARRAY_CNT( tMemberList );

	int nCorpsOfUinon[MAX_CORPS_UNION] = {0};
	int nCorpsNum = sizeof(nCorpsOfUinon);
	GetCorpsList( nCorpsOfUinon, nCorpsNum);

	// 遍历所有子军团（包括主军团），把各军团的参战阵容加入到列表中
	for( int i = 0; i < nCorpsNum; i++ )
	{
		CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( nCorpsOfUinon[i] );
		if ( tpCorps == NULL )
		{
			continue;
		}

		tArrayCnt = sizeof(tMemberList);
		tpCorps->GetBattleMemberList( tMemberList, tArrayCnt );	

		for ( int k = 0; k < tArrayCnt && nSize < tSize; k ++ )
		{
			vpList[ nSize ++ ] = tMemberList[ k ];
		}

	}


	return 0;
}
// 获得军盟势力值
int CUnion::GetUnionPower()
{
	int tPower = 0;
	for ( CUnion::CorpsVector::iterator it = mCorpsList.begin(); it != mCorpsList.end(); ++it )
	{
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( *it );
		if ( tpCorps != NULL )
		{
			tPower += tpCorps->GetPower();
		}
	}

	// 加上主军团
	CCorps *tpCoreCorps = CCorpsManager::GetSingleton().GetCorpsByID( this->GetCoreID() );
	if ( tpCoreCorps != NULL )
	{
		tPower += tpCoreCorps->GetPower();
	}

	return tPower;
}


