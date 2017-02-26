#include "corps.h"
#include "corpsmanager.h" 
#include "corpservice.h"
#include "nameservice.h"
#include "gateobj_manager.h"
#include "gatelogic.h"	
#include "core_service.h"
#include "world.h"
#include "promessage_pb.hxx.pb.h"
#include "family_manager.h"
#include "gateteam.h"
#include "teamtype.h"
#include "servermessage_pb.hxx.pb.h"
#include "gateteammanager.h"

void CCountry::LoadCorps( )
{	
	//此函数似乎从未被调用过
	//CCorpsManager::GetSingleton().OnFindBattleCity( mMapID, mCorpsID );	
	mOfficialData[ OFFICIAL_FUNCTION_KING ][ 0 ].mOfficialID = GetKingID( );
}

void CCountry::LoadOfficial( PBOfficialList vList )
{
	for ( size_t i = 0; i < ARRAY_ROW( mOfficialData ); ++ i )
	{
		for ( size_t j = 0; j < ARRAY_COL( mOfficialData ); j ++ )
		{
			int tIndex = ( i * ARRAY_COL( mOfficialData ) + j );
			if ( tIndex < vList.officialdata_size() )
			{
				PBOfficialData *tpOfficial = vList.mutable_officialdata(tIndex);
				if ( tpOfficial == NULL )
					continue;
				// 国王前面已经处理过
				if ( i != OFFICIAL_LEVEL_KING )
				{
					mOfficialData[ i ][ j ].mOfficialID = tpOfficial->charid();
					CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( mOfficialData[ i ][ j ].mOfficialID );
					if ( pRoleInfo != NULL )
					{
						strncpy( mOfficialData[ i ][ j ].mOfficialName, pRoleInfo->RoleName( ), sizeof ( mOfficialData[ i ][ j ].mOfficialName ) );
					}
				}				
				for ( int n = 0; n < OFFICIAL_FUNCTION_NUM; n++ )
				{
					for ( int m = 0; m < MAX_POWER; m++ )
					{
						int tNum = n * OFFICIAL_FUNCTION_NUM + MAX_POWER;
						if ( tNum < tpOfficial->usetimes_size() )
						{
							mOfficialData[ i ][ j ].mUsetimes[ n ][ m ] = tpOfficial->usetimes(tNum);
						}						
					}

				}
			}			
		}
	}
}

void CCountry::SaveOfficial( PBOfficialList &vList )
{
	for ( size_t i = 0; i < ARRAY_ROW( mOfficialData ); ++ i )
	{
		for ( size_t j = 0; j < ARRAY_COL( mOfficialData ); j ++ )
		{
			PBOfficialData *tpOfficial = vList.add_officialdata();
			tpOfficial->set_charid( mOfficialData[ i ][ j ].mOfficialID );
			for ( int n = 0; n < OFFICIAL_FUNCTION_NUM; n++ )
			{
				for ( int m = 0; m < MAX_POWER; m++ )
				{
					tpOfficial->add_usetimes( mOfficialData[ i ][ j ].mUsetimes[ n ][ m ] );
				}
			}
		}
	}
}

void CCountry::LoadOfficial( PBOfficialList vList, int vOfficial )
{
	for ( size_t i = 0; i < ARRAY_ROW( mOfficialData ); ++ i )
	{
		for ( size_t j = 0; j < ARRAY_COL( mOfficialData ); j ++ )
		{
			int tIndex = ( i * ARRAY_COL( mOfficialData ) + j );
			if ( tIndex < vList.officialdata_size() )
			{
				PBOfficialData *tpOfficial = vList.mutable_officialdata(tIndex);
				if ( tpOfficial == NULL )
					continue;
				// 国王前面已经处理过
				if ( i != OFFICIAL_LEVEL_KING )
				{
					mOfficialData[ i ][ j ].mOfficialID = tpOfficial->charid();
					CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( mOfficialData[ i ][ j ].mOfficialID );
					if ( pRoleInfo != NULL )
					{
						strncpy( mOfficialData[ i ][ j ].mOfficialName, pRoleInfo->RoleName( ), sizeof ( mOfficialData[ i ][ j ].mOfficialName ) );
					}
				}
				for ( int m = 0; m < MAX_POWER; m++ )
				{
					mOfficialData[ i ][ j ].mUsetimes[ vOfficial ][ m ] = tpOfficial->usetimes(m);						
				}
			}			
		}
	}
}

void CCountry::SaveOfficial( PBOfficialList &vList, int vOfficial )
{
	for ( size_t i = 0; i < ARRAY_ROW( mOfficialData ); ++ i )
	{
		for ( size_t j = 0; j < ARRAY_COL( mOfficialData ); j ++ )
		{
			PBOfficialData *tpOfficial = vList.add_officialdata();
			tpOfficial->set_charid( mOfficialData[ i ][ j ].mOfficialID );

			for ( int m = 0; m < MAX_POWER; m++ )
			{
				tpOfficial->add_usetimes( mOfficialData[ i ][ j ].mUsetimes[ vOfficial ][ m ] );
			}
		}
	}
}