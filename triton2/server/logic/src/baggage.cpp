// ********************************************************************** //														
// CBaggage																														
// ********************************************************************** //
#include "propClient.h"

int CBaggage::Initialize( )
{
	mUpgIndex1 = -1;
	mUpgIndex2 = -1;
	mUpgIndex3 = -1;
	mRuneIndex1 = -1;
	mRuneIndex2 = -1;
	mJewelIndex0 = -1;
	mYuanIndex0 = -1;
	mSlotIndex1 = -1;
	mSlotIndex2 = -1;
	mEquipIndex = -1;
	mBindItemIndex   = -1;
	mEqtForReusmBindIndex = -1;
	mYuanNum = 0;
	mTaskLockIndex = -1;
	mTeleSymbolIndex = -1;
	mAddIndex1 = -1;
	mAddIndex2 = -1;
	mRemoveIndex1 = -1;
	mRemoveIndex2 = -1;
	mJudgeIndex1 = -1;
	mJudgeIndex2 = -1;
	mChangeIndex1 = -1;
	mChangeIndex2 = -1;
	mJewComNum = 0;
	mMWIndex = -1;
	mMWStoneIndex = -1;
	mMultiIndex = -1;
	for ( int i = 0; i < JEWELNUM; i++ )
	{
		mJewelIndex[ i ] = -1;
	}
	for ( int i = 0; i < YUANNUM; i++ )
	{
		mYuanIndex[ i ] = -1;
	}
	for ( int i = 0; i < COMPOSENUM; i++ )
	{
		mJewComIndex[ i ] = -1;
	}
	/*for ( int i = 0 ; i <  MAX_KITBAG_NUM ; i++ )
	{
	mStorKitbag[ i ] = -1;
	}*/

	mReleaseIndex = BAGGAGE_BASE;
	mKitBagIndex = MAX_BAGGAGEKITBAG_NUM;

	return 0;
}

// *************************************************************************** //
// CBaggage
// 包裹类
// *************************************************************************** //
int CBaggage::AddUpgItem( unsigned int vIndex )
{	
	CItemObject* tpItemObj =  GetItemObjPtr ( vIndex );
	LK_ASSERT( tpItemObj != NULL, return -1 );

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpItemObj->GetItemID() );
	if ( tpItem == NULL )
		return -1;
	// 如果是装备
	if ( tpItem->mItemType == CTplItem::ITEM_ARMOR || tpItem->mItemType == CTplItem::ITEM_WEAPON || tpItem->mItemType == CTplItem::ITEM_MAGICWEAPON )
	{
		CTemplateEquipment *tpEquip = (CTemplateEquipment*)tpItem;
		if ( tpEquip->mCanUpgrade == 1 ) // 谁定的1是不能升级！！强烈鄙视！！
			return -1;
		mUpgIndex1 = vIndex;
		return 0;
	}
	// 如果是宝石
	if ( tpItem->mItemType == CTplItem::ITEM_EQUIPUPG )
	{
		mUpgIndex2 = vIndex;
		return 1;
	}
	// 如果是符
	if ( tpItem->mItemType == CTplItem::ITEM_UPGLUCK || tpItem->mItemType == CTplItem::ITEM_UPGLEAST )
	{
		mUpgIndex3 = vIndex;
		return 2;
	}
	return -1;
}

int CBaggage::DelUpgItem( unsigned int vIndex )
{
	int tTempIndex = -1;
	if ( vIndex == 0 )
	{
		tTempIndex = mUpgIndex1;
		mUpgIndex1 = -1;
	}
	if ( vIndex == 1 )
	{
		tTempIndex = mUpgIndex2;
		mUpgIndex2 = -1;
	}
	if ( vIndex == 2 )
	{
		tTempIndex = mUpgIndex3;
		mUpgIndex3 = -1;
	}
	return tTempIndex;
}

int CBaggage::AddRuneItem( unsigned int vIndex )
{
	CItemObject* tpItemObj =  GetItemObjPtr ( vIndex );
	LK_ASSERT( tpItemObj != NULL, return -1 );

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpItemObj->GetItemID() );
	if ( tpItem == NULL )
		return -1;
	// 如果是装备
	if ( tpItem->mItemType == CTplItem::ITEM_ARMOR || tpItem->mItemType == CTplItem::ITEM_WEAPON )
	{
		mRuneIndex1 = vIndex;
		return 0;
	}
	// 如果是宝石
	if ( tpItem->mItemType == CTplItem::ITEM_RUNE )
	{
		mRuneIndex2 = vIndex;
		return 1;
	}
	return -1;
}

int CBaggage::DelRuneItem( unsigned int vIndex )
{
	int tTempIndex = -1;
	if ( vIndex == 0 )
	{
		tTempIndex = mRuneIndex1;
		mRuneIndex1 = -1;
	}
	if ( vIndex == 1 )
	{
		tTempIndex = mRuneIndex2;
		mRuneIndex2 = -1;
	}
	return tTempIndex;
}

int CBaggage::AddJewelItem( unsigned int vSrcIndex, unsigned int vDesIndex )
{	
	CItemObject* tpItemObj =  GetItemObjPtr ( vSrcIndex );
	LK_ASSERT( tpItemObj != NULL, return -1 );	

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpItemObj->GetItemID() );
	if ( tpItem == NULL )
		return -1;
	// 如果是装备
	if ( tpItem->mItemType == CTplItem::ITEM_ARMOR || tpItem->mItemType == CTplItem::ITEM_WEAPON )
	{
		mJewelIndex0 = vSrcIndex;
		return 0;
	}
	// 如果是宝石
	if ( tpItem->mItemType == CTplItem::ITEM_JEWEL )
	{
		if( vDesIndex == 0 || vDesIndex > JEWELNUM )
			return -1;
		CItemEquipment* tpItemObj1 =  (CItemEquipment*)GetItemObjPtr ( mJewelIndex0 );
		if ( tpItemObj1 == NULL )
			return -1;
		// 如果未开槽
		if ( tpItemObj1->GetSlotProperty( vDesIndex - 1 ) != 1 )
			return -1;
		CTplItemJewel *tpJewel = (CTplItemJewel*)tpItem;		
		CTemplateEquipment* tpEquip = (CTemplateEquipment*) CDataStatic::SearchTpl( tpItemObj1->GetItemID() );
		if ( tpEquip == NULL )
			return -1;
		// 如果所镶的装备位置不对
		if (
			(( tpEquip->mEquipType == CTemplateEquipment::PART_RING )	&& ( ( tpJewel->mPos & CTplItemJewel::POS_RING ) == 0 ))  	||
			(( tpEquip->mEquipType == CTemplateEquipment::PART_CUFF )	&& ( ( tpJewel->mPos & CTplItemJewel::POS_CUFF ) == 0 )) 	||
			(( tpEquip->mEquipType == CTemplateEquipment::PART_WEAPON )	&& ( ( tpJewel->mPos & CTplItemJewel::POS_WEAPON ) == 0 ))  ||		
			(( tpEquip->mEquipType == CTemplateEquipment::PART_CHEST )	&& ( ( tpJewel->mPos & CTplItemJewel::POS_CHEST ) == 0 ))	||
			(( tpEquip->mEquipType == CTemplateEquipment::PART_LEG )	&& ( ( tpJewel->mPos & CTplItemJewel::POS_LEG ) == 0 )) 	||
			(( tpEquip->mEquipType == CTemplateEquipment::PART_HEAD )	&& ( ( tpJewel->mPos & CTplItemJewel::POS_HEAD ) == 0 ))	||
			(( tpEquip->mEquipType == CTemplateEquipment::PART_NECK )	&& ( ( tpJewel->mPos & CTplItemJewel::POS_NECK ) == 0 ))	||
			(( tpEquip->mEquipType == CTemplateEquipment::PART_SASH )	&& ( ( tpJewel->mPos & CTplItemJewel::POS_SASH ) == 0 ))	||
			(( tpEquip->mEquipType == CTemplateEquipment::PART_CHARM )	&&( ( tpJewel->mPos & CTplItemJewel::POS_CHARM ) == 0 ) )	||
			(( tpEquip->mEquipType == CTemplateEquipment::PART_BALL )	&&( ( tpJewel->mPos & CTplItemJewel::POS_BALL )	== 0 ) )	||
			(( tpEquip->mEquipType == CTemplateEquipment::PART_CLOAK )	&&( ( tpJewel->mPos & CTplItemJewel::POS_CLOAK ) == 0 ) )
			)
			return -1;		
		mJewelIndex[ vDesIndex - 1 ] = vSrcIndex;
		return vDesIndex;
	}
	return -1;
}

int CBaggage::DelJewelItem( unsigned int vIndex )
{
	int tTempIndex = -1;
	if ( vIndex == 0 )
	{
		tTempIndex = mJewelIndex0;
		mJewelIndex0 = -1;
	}
	if( vIndex > 0 && vIndex <= JEWELNUM )
	{
		tTempIndex = mJewelIndex[ vIndex - 1 ];
		mJewelIndex[ vIndex - 1 ] = -1;
	}	
	return tTempIndex;
}

int CBaggage::AddSlotItem( unsigned int vIndex )
{
	CItemObject* tpItemObj =  GetItemObjPtr ( vIndex );
	LK_ASSERT( tpItemObj != NULL, return -1 );

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpItemObj->GetItemID() );
	if ( tpItem == NULL )
		return -1;
	// 如果是装备
	if ( tpItem->mItemType == CTplItem::ITEM_ARMOR || tpItem->mItemType == CTplItem::ITEM_WEAPON )
	{
		mSlotIndex1 = vIndex;
		return 0;
	}
	// 如果是元气蛋
	if ( tpItem->mItemType == CTplItem::ITEM_YUANEGG )
	{
		mSlotIndex2 = vIndex;
		return 1;
	}
	return -1;
}

int CBaggage::DelSlotItem( unsigned int vIndex )
{
	int tTempIndex = -1;
	if ( vIndex == 0 )
	{
		tTempIndex = mSlotIndex1;
		mSlotIndex1 = -1;
	}
	if ( vIndex == 1 )
	{
		tTempIndex = mSlotIndex2;
		mSlotIndex2 = -1;
	}
	return tTempIndex;
}

int CBaggage::AddYuanItem( unsigned int vIndex )
{
	// 如果已经放满
	if ( mYuanNum >= YUANNUM )
		return -1;
	CItemObject* tpItemObj =  GetItemObjPtr ( vIndex );
	LK_ASSERT( tpItemObj != NULL, return -1 );

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpItemObj->GetItemID() );
	if ( tpItem == NULL )
		return -1;
	// 如果是元气虫
	if ( tpItem->mItemType == CTplItem::ITEM_YUANBUG )
	{
		mYuanIndex0 = vIndex;
		return 0;
	}
	// 如果是装备
	if ( tpItem->mItemType == CTplItem::ITEM_ARMOR || tpItem->mItemType == CTplItem::ITEM_WEAPON )
	{
		for ( int i = 0; i < YUANNUM; i++ )
		{
			if ( mYuanIndex[ i ] == -1 )
			{
				mYuanIndex[ i ] = vIndex;
				mYuanNum++;
				return i + 1;
			}
		}		
	}
	return -1;
}

int CBaggage::DelYuanItem( unsigned int vIndex )
{
	int tTempIndex = -1;
	if ( vIndex == 0 )
	{
		tTempIndex = mYuanIndex0;
		mYuanIndex0 = -1;
	}
	if( vIndex > 0 && vIndex <= YUANNUM )
	{
		tTempIndex = mYuanIndex[ vIndex - 1 ];
		mYuanIndex[ vIndex - 1 ] = -1;
		if ( mYuanNum > 0 )
			mYuanNum--;
	}	
	return tTempIndex;
}

int CBaggage::AddComposeItem( unsigned int vIndex )
{
	CItemObject* tpItemObj =  GetItemObjPtr ( vIndex );
	LK_ASSERT( tpItemObj != NULL, return -1 );

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpItemObj->GetItemID() );
	if ( tpItem == NULL )
		return -1;
	// 如果是装备
	if ( tpItem->mItemType == CTplItem::ITEM_YUANEGG || tpItem->mItemType == CTplItem::ITEM_PRODUCE )
	{
		mComposeIndex = vIndex;
		return 0;
	}
	return -1;
}

int CBaggage::DelComposeItem(  )
{
	int tTempIndex = -1;
	if ( mComposeIndex != -1 )
	{
		tTempIndex = mComposeIndex;
		mComposeIndex = -1;
	}	
	return tTempIndex;
}

int CBaggage::AddAddItem( unsigned int vIndex )
{	
	CItemObject* tpItemObj =  GetItemObjPtr ( vIndex );
	LK_ASSERT( tpItemObj != NULL, return -1 );

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpItemObj->GetItemID() );
	if ( tpItem == NULL )
		return -1;
	// 如果是装备
	if ( tpItem->mItemType == CTplItem::ITEM_ARMOR || tpItem->mItemType == CTplItem::ITEM_WEAPON )
	{
		mAddIndex1 = vIndex;
		return 0;
	}
	if ( tpItem->mItemType == CTplItem::ITEM_BOOK )
	{
		CTplItemBook *tpBook = (CTplItemBook*)tpItem;
		if ( tpBook->mOPType == CTplItemBook::TYPE_ADDSLOT )
		{
			mAddIndex2 = vIndex;
			return 1;
		}
	}
	return -1;
}

int CBaggage::DelAddItem( unsigned int vIndex )
{
	int tTempIndex = -1;
	if ( vIndex == 0 )
	{
		tTempIndex = mAddIndex1;
		mAddIndex1 = -1;
	}
	if ( vIndex == 1 )
	{
		tTempIndex = mAddIndex2;
		mAddIndex2 = -1;
	}
	return tTempIndex;
}

int CBaggage::AddRemoveItem( unsigned int vIndex )
{	
	CItemObject* tpItemObj =  GetItemObjPtr ( vIndex );
	LK_ASSERT( tpItemObj != NULL, return -1 );

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpItemObj->GetItemID() );
	if ( tpItem == NULL )
		return -1;
	// 如果是装备
	if ( tpItem->mItemType == CTplItem::ITEM_ARMOR || tpItem->mItemType == CTplItem::ITEM_WEAPON )
	{
		mRemoveIndex1 = vIndex;
		return 0;
	}
	if ( tpItem->mItemType == CTplItem::ITEM_REMOVE )
	{
		mRemoveIndex2 = vIndex;
		return 1;
	}
	return -1;
}

int CBaggage::DelRemoveItem( unsigned int vIndex )
{
	int tTempIndex = -1;
	if ( vIndex == 0 )
	{
		tTempIndex = mRemoveIndex1;
		mRemoveIndex1 = -1;
	}
	if ( vIndex == 1 )
	{
		tTempIndex = mRemoveIndex2;
		mRemoveIndex2 = -1;
	}
	return tTempIndex;
}

int CBaggage::AddJudgeItem( unsigned int vIndex )
{	
	CItemObject* tpItemObj =  GetItemObjPtr ( vIndex );
	LK_ASSERT( tpItemObj != NULL, return -1 );

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpItemObj->GetItemID() );
	if ( tpItem == NULL )
		return -1;
	// 如果是装备
	if ( tpItem->mItemType == CTplItem::ITEM_ARMOR || tpItem->mItemType == CTplItem::ITEM_WEAPON )
	{
		mJudgeIndex1 = vIndex;
		return 0;
	}
	if ( tpItem->mItemType == CTplItem::ITEM_BOOK )
	{
		CTplItemBook *tpBook = (CTplItemBook*)tpItem;
		if ( tpBook->mOPType == CTplItemBook::TYPE_JUDGE )
		{
			mJudgeIndex2 = vIndex;
			return 1;
		}
	}
	return -1;
}

int CBaggage::DelJudgeItem( unsigned int vIndex )
{
	int tTempIndex = -1;
	if ( vIndex == 0 )
	{
		tTempIndex = mJudgeIndex1;
		mJudgeIndex1 = -1;
	}
	if ( vIndex == 1 )
	{
		tTempIndex = mJudgeIndex2;
		mJudgeIndex2 = -1;
	}
	return tTempIndex;
}

int CBaggage::AddChangeItem( unsigned int vIndex )
{	
	CItemObject* tpItemObj =  GetItemObjPtr ( vIndex );
	LK_ASSERT( tpItemObj != NULL, return -1 );

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpItemObj->GetItemID() );
	if ( tpItem == NULL )
		return -1;
	// 如果是装备
	if ( tpItem->mItemType == CTplItem::ITEM_ARMOR || tpItem->mItemType == CTplItem::ITEM_WEAPON )
	{
		mChangeIndex1 = vIndex;
		return 0;
	}
	if ( tpItem->mItemType == CTplItem::ITEM_CHANGE )
	{
		mChangeIndex2 = vIndex;
		return 1;
	}
	return -1;
}

int CBaggage::DelChangeItem( unsigned int vIndex )
{
	int tTempIndex = -1;
	if ( vIndex == 0 )
	{
		tTempIndex = mChangeIndex1;
		mChangeIndex1 = -1;
	}
	if ( vIndex == 1 )
	{
		tTempIndex = mChangeIndex2;
		mChangeIndex2 = -1;
	}
	return tTempIndex;
}

int CBaggage::AddJewComItem( unsigned int vIndex )
{
	// 如果已经放满
	if ( mJewComNum >= COMPOSENUM )
		return -1;
	CItemObject* tpItemObj =  GetItemObjPtr ( vIndex );
	LK_ASSERT( tpItemObj != NULL, return -1 );

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpItemObj->GetItemID() );
	if ( tpItem == NULL )
		return -1;	
	if ( tpItem->mItemType == CTplItem::ITEM_JEWEL )
	{
		for ( int i = 0; i < COMPOSENUM; i++ )
		{
			if ( mJewComIndex[ i ] == -1 )
			{
				mJewComIndex[ i ] = vIndex;
				mJewComNum++;
				return i;
			}
		}		
	}
	return -1;
}

int CBaggage::DelJewComItem( unsigned int vIndex )
{
	int tTempIndex = -1;
	if( vIndex >= 0 && vIndex < COMPOSENUM )
	{
		tTempIndex = mJewComIndex[ vIndex ];
		mJewComIndex[ vIndex ] = -1;
		if ( mJewComNum > 0 )
			mJewComNum--;
	}	
	return tTempIndex;
}

int CBaggage::AddMultiItem( unsigned int vIndex )
{
	CItemObject* tpItemObj =  GetItemObjPtr ( vIndex );
	LK_ASSERT( tpItemObj != NULL, return -1 );

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpItemObj->GetItemID() );
	if ( tpItem == NULL )
		return -1;
	// 如果是装备
	if ( tpItem->mItemType == CTplItem::ITEM_ARMOR || tpItem->mItemType == CTplItem::ITEM_WEAPON )
	{
		mMultiIndex = vIndex;
		return 0;
	}
	return -1;
}

int CBaggage::DelMultiItem( unsigned int vIndex )
{
	int tTempIndex = -1;
	tTempIndex = mMultiIndex;
	mMultiIndex = -1;
	return tTempIndex;
}
void CBaggage::DeleteUnusedItem( int vCharID )
{
	CItemBox::DeleteUnusedItem( vCharID );
}

