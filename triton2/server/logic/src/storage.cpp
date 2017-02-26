// *************************************************************************** //
// CStorage  
// 仓库类
// *************************************************************************** //	
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
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

// 初始化函数
int CStorage::Initialize()
{
	mBindMoney = 0;
	mMoney = 0;

	mReleaseIndex  = BAGGAGE_BASE;
	mKitBagIndex = MAX_KITBAG_NUM;

	return 0;
}

// 存储金钱
int CStorage::StoreMoney( int nMoney, int nBindMoney )
{
	LK_ASSERT( nMoney >= 0 && nMoney <= MONEY_LIMIT, return -1  );
	LK_ASSERT( nBindMoney >= 0 && nBindMoney <= MONEY_LIMIT, return -1 );
	if ( mMoney + nMoney > MONEY_LIMIT || mMoney + nMoney < 0  )
	{
		return  ERR_STORAGE_STORMONEYFULL;
	}
	if ( ( nBindMoney + mBindMoney ) > MONEY_LIMIT || ( mBindMoney + nBindMoney ) < 0 )
	{
		return ERR_STORAGE_STORMONEYFULL;
	}
	mBindMoney += nBindMoney;
	mMoney	   += nMoney;
	return SUCCESS;	
}

// 取出金钱
int CStorage::TakeMoney( int nMoney, int nBindMoney )
{ 
	LK_ASSERT( nMoney >= 0 && nBindMoney >= 0 , return -1 )		
		if ( ( mBindMoney - nBindMoney ) < 0 || ( mMoney - nMoney ) < 0 )
		{
			return ERR_STORAGE_HASNOSOMUCHMONEY;
		}
		mBindMoney -= nBindMoney;
		mMoney	   -= nMoney;
		return SUCCESS;
}

bool CStorage::CreateFromTlv2( PBItemBox* pBuffer )
{	
	/*	mMoney		= pBuffer->stormoney();
	mBindMoney	= pBuffer->storbindmoney();		*/	
	return CItemBox::CreateFromTlv2( pBuffer );
}		 

void CStorage::GetTlvBuffer2( PBItemBox* pBuffer, int tStart, int tEnd )
{
	/*pBuffer->set_stormoney( mMoney );
	pBuffer->set_storbindmoney( mBindMoney ); 	*/
	CItemBox::GetTlvBuffer2( pBuffer, tStart, tEnd );
}

void CStorage::DeleteUnusedItem( int vCharID )
{
	CItemBox::DeleteUnusedItem( vCharID );
}