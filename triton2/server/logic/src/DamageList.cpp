
//////////////////////////////////////////////////////////////////////////
// CDamageList
//////////////////////////////////////////////////////////////////////////

//#include "propClient.h"
#include "stdafx.hpp"
#include "proclient_svr.h"
#include "coremessage_pb.hxx.pb.h"
#include "promessage_pb.hxx.pb.h"
#include "sceneobj_manager.h"
#include "scenelogic.h"
#include "westserver.h"
#include "servercore.h"
#include "entity.h"
#include "template.h"
#include "property.h"
#include "entity.h"
#include "pro_define.h"
#include "propertymodule.h"
#include "scenecfg_manager.h"
#include "repetionmodule.h"
#include <llimits.h>
#include "buffer.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif


float CDamageList::GetDamagePer( int vEntityID, int tMaxHP )
{
	for ( int i = 0; i < mNumber; i ++ )
	{
		if ( mDamage[ i ].mSrcEntityID == vEntityID )
			return mDamage[ i ].mDamageValue / (float) tMaxHP;
	}

	return 0.0f;
}

int CDamageList::InsertDamage( const CDamage& rDamage )
{
	int tMaxDamager = 0;
	for ( int i = 0; i <= mNumber && i < (int)ARRAY_CNT( mDamage ); i ++ )
	{
		if ( mDamage[ i ].mSrcEntityID == 0 )
		{
			mDamage[ i ] = rDamage;

			/*if ( mDamage[ i ].mDamageValue > tMaxDamager )
			tMaxDamager = mDamage[ i ].mSrcEntityID;*/
			mNumber++;
			break;
		}

		if ( mDamage[ i ].mSrcEntityID == rDamage.mSrcEntityID )
		{
			mDamage[ i ].mDamageValue += rDamage.mDamageValue;
			/*if ( mDamage[ i ].mDamageValue > tMaxDamager )
			tMaxDamager = mDamage[ i ].mSrcEntityID;*/
			// 这里原来没有退出,有错误
			break;
		}			
	}

	return tMaxDamager;
}

int CDamageList::RemoveDamage( int vEntityID )
{
	for ( int i = 0; i < mNumber && i < (int)ARRAY_CNT( mDamage ); i ++ )
	{
		if ( mDamage[ i ].mSrcEntityID == 0 )
			break;
		if ( mDamage[ i ].mSrcEntityID == vEntityID )
		{
			mDamage[ i ].mSrcEntityID = mDamage[ mNumber - 1 ].mSrcEntityID;
			mDamage[ i ].mDamageValue = mDamage[ mNumber - 1 ].mDamageValue;
			mDamage[ mNumber - 1 ].mSrcEntityID = 0;
			mDamage[ mNumber - 1 ].mDamageValue = 0;
			mNumber--;
			break; 
		}
	}

	return 0;
}

