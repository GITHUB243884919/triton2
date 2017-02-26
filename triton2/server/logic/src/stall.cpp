
#include "stdafx.hpp"
#include "entity.h"
#include "stall.h"
#include "coretype.h"
#include "propertymodule.h"

//***************************************
// Function Name : CStall
// Description 	 :
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 1/13/2009
//***************************************
CStall::CStall()
{
	if ( CObj::msCreateMode )
	{
		Initialize();
	}
	else
	{
		Resume();
	}
}

CStall::~CStall()
{
	mStall.clear();
}

//***************************************
// Function Name : Initialize
// Description 	 :
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 1/13/2009
//***************************************

int CStall::Initialize()
{
	mMasterID = 0;
	mStallName[ 0 ] = '\0';
	mAdContent[ 0 ] = '\0';
	mStallType = 0;
	memset( mStallName, 0, sizeof( mStallName ) );
	memset( mAdContent, 0, sizeof( mAdContent ) );
	mStall.initailize();
	return LK_ISUCCESS;
}

//***************************************
// Function Name : Resume
// Description 	 :
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 1/13/2009
//***************************************

int CStall::Resume()
{
	return LK_ISUCCESS;
}

//***************************************
// Function Name : InitStall
// Description 	 :
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 1/13/2009
//***************************************

int CStall::InitStall(unsigned int vMasterID, unsigned int vStallType  )
{
	mMasterID = vMasterID;
	mStallType = vStallType;
	return LK_ISUCCESS;
}

//***************************************
// Function Name : InsertGoods
// Description 	 :
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 1/13/2009
//***************************************

int CStall::InsertGoods( unsigned int vItemID, unsigned int vNum, unsigned int vPrice, unsigned int vStallIdx, unsigned int vBagIdx, unsigned int vPriceType )
{
	StallType::iterator it = mStall.begin();
	for ( ; it != mStall.end(); ++ it )
	{
		if ( (*it).BagIdx == vBagIdx || (*it).Index == vStallIdx )
		{
			return LK_IERROR;
		}
	}
	if ((mStallType == EM_STALL_TYPE_MONEY) && (vPriceType != CYuanBao::em_unbind_money ))
	{
		return LK_IERROR;
	}
	if ((mStallType == EM_STALL_TYPE_YB) && (vPriceType != CYuanBao::em_golden_yuanbao ))
	{
		return LK_IERROR;
	}

	GOODS tGoods( vItemID, vNum, vPrice, vStallIdx, vBagIdx,vPriceType );
	mStall.push_back( tGoods );
	return LK_ISUCCESS;
}

//***************************************
// Function Name : RemoveGoods
// Description 	 :
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 1/13/2009
//***************************************

int CStall::RemoveGoods(unsigned int vBagIndex, unsigned int vStallIndex, unsigned int vNumber )
{
	StallType::iterator it = mStall.begin();
	for ( ; it != mStall.end(); ++ it )
	{
		if ( (*it).BagIdx == vBagIndex && (*it).Index == vStallIndex )
		{
			if ( vNumber < (*it).Number )
			{
				(*it).Number -= vNumber;
			}
			else
			{
				mStall.erase( it );
			}			
			return LK_ISUCCESS;
		}
	}
	return LK_IERROR;
}

//***************************************
// Function Name : UpdatePrice
// Description 	 :
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 1/13/2009
//***************************************

int CStall::UpdatePrice(unsigned int vStallIndex, unsigned int vPrice, unsigned int vPriceType)
{
	StallType::iterator it = mStall.begin();
	for ( ; it != mStall.end(); ++ it )
	{
		if ( (*it).Index == vStallIndex )
		{
			(*it).Price = vPrice;
			(*it).PriceType = vPriceType;
			return LK_ISUCCESS;
		}
	}
	return LK_IERROR;
}

//***************************************
// Function Name : LockGoods
// Description 	 :
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 1/13/2009
//***************************************

bool CStall::LockGoods(unsigned int vIndex)
{
	bool bRes = false;
	CPropertyModule::GetSingleton().LockBaggage( mMasterID, vIndex, LOCKTYPE_STALL, bRes );
	return bRes;
}

//***************************************
// Function Name : UnLockGoods
// Description 	 :
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 1/13/2009
//***************************************

bool CStall::UnLockGoods(unsigned int vIndex)
{
	bool bRes = false;
	CPropertyModule::GetSingleton().LockBaggage( mMasterID, vIndex, LOCKTYPE_NONE, bRes );
	return bRes;
}

//***************************************
// Function Name : GetStallInfo
// Description 	 :
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 1/13/2009
//***************************************

int CStall::GetStallInfo(CMessageClickStallResponse* tpResponse )
{
	LK_ASSERT( tpResponse != NULL, return LK_IERROR );
	
	StallType::iterator it = mStall.begin();
	for ( ; it != mStall.end(); ++ it )
	{
		PBItemObject* tpItem = tpResponse->add_itemlist();
		int tRes = 0;
		if ( CPropertyModule::GetSingleton().GetItemBinary( mMasterID, (*it).BagIdx, (*it).Number, tpItem, tRes, false ) != LK_ISUCCESS )
		{
			return LK_IERROR;
		}
		tpResponse->add_price( (*it).Price );
		tpResponse->add_pricetype((*it).PriceType);
		tpResponse->add_stallindex( (*it).Index );
	}
	return LK_ISUCCESS;
}

// 得到/修物品的状态
int CStall::GetGoodsStatus( unsigned int nStallIndex )
{
	StallType::iterator it = mStall.begin();
	for ( ; it != mStall.end(); ++ it )
	{
		if ( (*it).Index == nStallIndex )
		{
			return (*it).Status;
		}
	}
	return GOODS::EGOODS_STATUS_NONE;
}


// 得到/修物品的状态
void CStall::SetGoodsStatus( unsigned int nStallIndex, int Status )
{
	StallType::iterator it = mStall.begin();
	for ( ; it != mStall.end(); ++ it )
	{
		if ( (*it).Index == nStallIndex )
		{
			(*it).Status = Status;
		}
	}
}


//***************************************
// Function Name : GetGoodsIndex
// Description 	 :
// Input Params  : stall index
// Output Params : item index, item number, item price
// Return Type   : int
// Modified Time : 1/13/2009
//***************************************

int CStall::GetGoodsIndex(unsigned int vStallIndex, unsigned int& tItemIndex, unsigned int& tItemNum, unsigned int& tPrice, unsigned int& tItemID, unsigned int& tPriceType )
{
	StallType::iterator it = mStall.begin();
	for ( ; it != mStall.end(); ++ it )
	{
		if ( (*it).Index == vStallIndex )
		{
			tItemIndex = (*it).BagIdx;
			tItemNum = (*it).Number;
			tPrice = (*it).Price;
			tItemID = (*it).ItemID;
			tPriceType = (*it).PriceType;
			return LK_ISUCCESS;
		}
	}
	return LK_IERROR;
}

//***************************************
// Function Name : ReleaseGoods
// Description 	 :
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 1/14/2009
//***************************************

int CStall::ReleaseGoods()
{
	StallType::iterator it = mStall.begin();
	for ( ; it != mStall.end(); ++ it )
	{
		UnLockGoods( (*it).BagIdx );
	}
	mStall.clear();
	//mLooker.clear();
	return LK_ISUCCESS;
}

//***************************************
// Function Name : SaveData
// Description 	 :
// Input Params  :
// Output Params :
// Return Type   :
// Modified Time : 2/19/2009
//***************************************

void CStall::SaveData( CEntityPlayer* pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return );

	int tItemIndexAry[ MAX_STALL_INDEX ] = { 0 };
	int tStallIndexAry[ MAX_STALL_INDEX ] = { 0 };
	int tPriceAry[ MAX_STALL_INDEX ] = { 0 };
	int tNumAry[ MAX_STALL_INDEX ] = { 0 };
	int tPriceTypeAry[ MAX_STALL_INDEX ] = {0};

	int tNum = 0;
	StallType::iterator it = mStall.begin();
	for ( ; it != mStall.end(); ++ it )
	{
		tItemIndexAry[ tNum ] = (*it).BagIdx;
		tStallIndexAry[ tNum ] = (*it).Index;
		tPriceAry[ tNum ] = (*it).Price;
		tNumAry[ tNum ] = (*it).Number;
		tPriceTypeAry[ tNum ] = (*it).PriceType;
		tNum ++;
	}

	CPropertyModule::GetSingleton().SaveStallInfo( pPlayer, mStallName, mAdContent, tNumAry, tPriceAry, tItemIndexAry, tStallIndexAry, tPriceTypeAry, tNum, mStallType );

	LOG_DEBUG( "stall", "Stall Saved: Player[%s],StallName[%s],AdContent[%s],ItemNumber[%d],StallType[%d]", pPlayer->GetCharNameShow(), mStallName, mAdContent, tNum , mStallType);
	return ;
}
