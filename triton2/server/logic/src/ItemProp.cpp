//***************************//
// CItemObject
//***************************//

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

void CItemObject::SetGUID() 
{ 
	if ( mObjGUID.a.guid == 0)
	{
		mObjGUID = OBJGUID(
			CSceneObjManager::GetSingleton().GetGUID(), 
			CSceneLogic::GetSingleton().GetWorldID(), 
			CSceneServer::GetSingleton().mThisServerID);
	}

	//return mObjGUID.value();
}

void CItemObject::GetItemBinaryBuffer2( PBItemObject* pBuffer, int vNumber  )
{
	// 如果传入的数量为-1, 那么使用该道具的数量
	unsigned short tNumber = ( vNumber == -1 ) ? mNumber : (unsigned short) vNumber;
	pBuffer->set_itemid( mItemID );
	pBuffer->set_number( tNumber );
	if( mBindStatus != 0 )
		pBuffer->set_bindstatus( mBindStatus );
	if( mUsedTimes != 0 )
		pBuffer->set_usedtimes( mUsedTimes );
	if( mPurchaseID != 0 )
		pBuffer->set_purchaseid( mPurchaseID);
	if (mObjGUID.value() != 0)
		pBuffer->set_objguid(mObjGUID.value());
	if (mInvalidTime != 0)
		pBuffer->set_invalidtime(mInvalidTime);
	GetItemBinary2( pBuffer );
}


// 是否是同一种道具组
bool CItemObject::IsSameItemGroup( unsigned int nItemID, int nType )
{
	CTplItem *tplObj = (CTplItem*)CDataStatic::SearchTpl( GetItemID() );
	CTplItem *tplNeed = (CTplItem*)CDataStatic::SearchTpl( nItemID );
	if( tplObj == NULL || tplNeed == NULL )
	{
		return false;
	}

	if ( GetItemID() == nItemID || ( tplNeed->mCommonID != 0 && tplObj->mCommonID == tplNeed->mCommonID ) )
	{
		if ( nType == BIND_YES && ( GetBindStatus( ) & ITEM_BINDSTATUS_BINDED ) == 0 )
			return false;

		if ( nType == BIND_NO && ( GetBindStatus( ) & ITEM_BINDSTATUS_BINDED ) == ITEM_BINDSTATUS_BINDED )
			return false;

		return true;
	}
	return false;
}

// 是否是地图受限物品
bool CItemObject::IsMapLimitedItem( int nCurMapID )
{
	CTplItem* tpTplItem = (CTplItem*)CDataStatic::SearchTpl(GetItemID());
	LK_ASSERT( tpTplItem!=NULL, return false );
	bool tCheck = true;
	for( unsigned int j = 0; j < ARRAY_CNT( tpTplItem->mLmtMapID ); j++ )
	{
		if ( tpTplItem->mLmtMapID[ j ] != 0 && tpTplItem->mLmtMapID[ j ] != nCurMapID )
		{
			// 非法持有 要干掉
			tCheck = false;
		}
		else if( tpTplItem->mLmtMapID[ j ] != 0 && tpTplItem->mLmtMapID[ j ] == nCurMapID )
		{
			tCheck = true;
            break;
		}
		else if( tpTplItem->mLmtMapID[ j ] == 0 )
		{
			break;
		}
	}

	// 判断刚好相反
	return !tCheck;
}

//是否是副本受限物品 add by yucx 200100722 
bool CItemObject::IsRepetionLimitedItem( int nCurRepetionIndex )
{
	CTplItem* tpTplItem = (CTplItem*)CDataStatic::SearchTpl(GetItemID());
	LK_ASSERT( tpTplItem!=NULL, return false );
	bool tCheck = true;
	for( unsigned int j = 0; j < ARRAY_CNT( tpTplItem->mLmtRepetionID); j++ )
	{
		if ( tpTplItem->mLmtRepetionID[ j ] != 0 && tpTplItem->mLmtRepetionID[ j ] != nCurRepetionIndex)
		{
			// 非法持有 要干掉
			tCheck = false;
		}
		else if( tpTplItem->mLmtRepetionID[ j ] != 0 && tpTplItem->mLmtRepetionID[ j ] == nCurRepetionIndex)
		{
			tCheck = true;
            break;
		}
		else if( tpTplItem->mLmtRepetionID[ j ] == 0 )
		{
			break;
		}
	}

	// 判断刚好相反
	return !tCheck;
}

/*****绑定物品*******/ 
int CItemObject::BindItem( int vBindType )
{
	// 已经绑定无需再绑定
	if ( ( mBindStatus & ITEM_BINDSTATUS_BINDED ) == ITEM_BINDSTATUS_BINDED )
	{
		return SUCCESS;
	}

	CTplItem *tpItem = ( CTplItem * )CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItem != NULL, return -1 )
	switch( vBindType )
	{
		case ITEM_BINDWAY_BYUSE:
			{

				if ( (tpItem->mProcessType & CTplItem::PROCESSTYPE_BINDONUSER) == 0 )
				{
					return ITEM_BINDRESULT_WRONGBINDWAY;
				}
				ChangeBindStatus( ITEM_BINDSTATUS_BINDED );
				break;
			}
		case ITEM_BINDWAY_BYPICKUP:
			{
				if ( (tpItem->mProcessType & CTplItem::PROCESSTYPE_BINDONPICK) == 0 )
				{

					return ITEM_BINDRESULT_WRONGBINDWAY;
				}
				ChangeBindStatus( ITEM_BINDSTATUS_BINDED );
				break;
			}
		case ITEM_BINDWAY_BYEQUIP:
			{
				if ( (tpItem->mProcessType & CTplItem::PROCESSTYPE_BINDONEQUIP) == 0 )
				{
					// 不能装备绑定					
					return ITEM_BINDRESULT_WRONGBINDWAY;
				}
				ChangeBindStatus( ITEM_BINDSTATUS_BINDED );  
				break;
			}  
		case ITEM_BINDWAY_BYITEM:
			{
				if ( (tpItem->mProcessType & CTplItem::PROCESSTYPE_CANBIND) == 0 )
				{
					// 不能装备绑定					
					return ITEM_BINDRESULT_CANNOTBIND;
				}
				ChangeBindStatus( ITEM_BINDSTATUS_BYITEM );  
				break;
			}

	}
	if ( mBindStatus & ITEM_BINDSTATUS_BINDED )
	{
		mBindTime = time( NULL );
	}
	return ITEM_BINDRESULT_SUCESS;
}

// int CItemEquipment::SetBindLeftTime()
// {
/*unsigned int TimeNow = time(0);
if ( mBindFreeTime <= TimeNow )
{
mBindLeftTime = 0;
}
else
{
mBindLeftTime = mBindFreeTime - TimeNow;
}
return mBindLeftTime;	*/
// }

int CItemEquipment::Initialize( )
{
	CItemObject::Initialize( );
	mLevel = 0;
	mSlotNumber = 0;
	mSlotSpell = 0;
	// 初始化数据
	for ( int i = 0; i < JEWELNUM; i ++ )
	{
		mSlotProperty[ i ] = 0;
		mSlotValue[ i ] = 0;
	}

	// 初始化数据
	for ( int i = 0; i < PROPERTYNUM; i ++ )
	{
		mProperty[ i ] = 0;
		mValue[ i ] = 0;
	}

	// 初始化数据
	for ( int i = 0; i < UPGVALUENUM; i ++ )
	{
		mJudge[ i ] = 0;
	}

	// 初始化数据
	mNameLength = 0;
	::memset( mName, 0, sizeof( mName ) );	
	//::memset( &mObjGUID, 0, sizeof( OBJGUID ) ); 
	// add by lixi
	//mBindLeftTime = 0;
	mBindFreeTime = 0;
	mCurDurability = 0;
	mAbrasionCount = 0;
	mMaxSlotNumber = CPropertyValue( );
	mResDef = 0;
	mResAtt = 0;
	mDefLevel = 0;
	mAttLevel = 0;
	mMagicStoneID = 0;
	mMagicStoneNum = 0;
	mSpouseID = 0;
	memset( mSpouseName, 0, sizeof( mSpouseName ) );
	mExtraType = 0;
	mBindTime = 0;
	return 0;
}

int CItemEquipment::ResetDurability()
{		
	CTemplateEquipment *tpEquipment = ( CTemplateEquipment * )CDataStatic::SearchTpl( mItemID );
	if ( tpEquipment ==NULL  )
	{
		return 0;
	}
	if ( tpEquipment->mEquipmentDurability == -1 )
	{
		return -1;
	}
	if ( tpEquipment->mEquipmentDurability < 0 )
	{
		return 0;
	}
	mCurDurability = tpEquipment->mEquipmentDurability;
	return mCurDurability;
}

int CItemEquipment::GetLostDurability(  )
{
	CTemplateEquipment *tpEquipment = ( CTemplateEquipment * )CDataStatic::SearchTpl( mItemID );
	if ( tpEquipment ==NULL  )
	{
		LOG_ERROR( "pro","[%s,%d][%s] Item does not exist in Template",__LK_FILE__,__LINE__,__FUNCTION__ );
		return 0;
	}
	if ( tpEquipment->mEquipmentDurability == -1 )
	{
		// 没有耐久度的装备
		return -1;
	}
	if ( tpEquipment->mEquipmentDurability < 0 )
	{
		// 错误的耐久度
		LOG_ERROR( "pro","[%s,%d][%s] Wrong Durability in Template!",__LK_FILE__,__LINE__,__FUNCTION__ );
		return 0;
	}
	int LostDuraBility  = tpEquipment->mEquipmentDurability - mCurDurability;
	return LostDuraBility;		
}

int CItemEquipment::GetMendMoney()
{

	CTemplateEquipment *tpEquipment = ( CTemplateEquipment * )CDataStatic::SearchTpl( mItemID );
	int LostDurability;
	if ( tpEquipment == NULL  )
	{
		LOG_ERROR( "pro","[%s,%d][%s] Item does not exist in Template",__LK_FILE__,__LINE__,__FUNCTION__ );
		return -1;      // 错误
	}
	if ( tpEquipment->mEquipmentDurability == -1 )
	{
		return 0;       // 不用修理
	}
	if ( tpEquipment->mEquipmentDurability <= 0 )
	{
		LOG_ERROR( "pro","[%s,%d][%s] Wrong Durability in Template!",__LK_FILE__,__LINE__,__FUNCTION__ );
		return 0;      // 错误
	}
	LostDurability = tpEquipment->mEquipmentDurability -  mCurDurability;
	if ( LostDurability < 0 )
	{
		LOG_ERROR( "pro","[%s,%d][%s] Current Durability is Wrong!  ",__LK_FILE__,__LINE__,__FUNCTION__ );
		return -1;      // 错误
	}
	if ( mLevel >= 16 )
	{
		LOG_ERROR( "pro","[%s,%d][%s] Equipment level has passed 16!  ",__LK_FILE__,__LINE__,__FUNCTION__ );
		return -1;      // 错误
	}
	mAbrasionCount = 0;
	int MendMoney = ( int )tpEquipment->mBaseMendCharge*LostDurability/tpEquipment->mEquipmentDurability;	//( LostDurability*tpEquipment->mBaseMendCharge*16.0/( 16.0 - mLevel ) );
	if ( MendMoney <= 0 && LostDurability > 0 )
	{
		MendMoney = 1;
	}
	return MendMoney;
}	

void CItemEquipment::GetItemBinary2( PBItemObject*& pGene )
{
	PBItemContent* tpItemContent =  pGene->mutable_itemcontent();
	LK_ASSERT( tpItemContent != NULL , return );

	PBItemObjEquip* pItemObjEquip = tpItemContent->mutable_itemequip();
	LK_ASSERT( pItemObjEquip != NULL, return );

	if( mLevel != 0 )
		pItemObjEquip->set_level( mLevel );

	if( mSlotSpell != 0 )
		pItemObjEquip->set_slotspell( mSlotSpell );

	for ( unsigned int i1 = 0; i1 < ARRAY_CNT( mSlotProperty ); i1++ )
	{
		pItemObjEquip->add_slotproperty( mSlotProperty[ i1 ] );
	}

	for ( unsigned int i2 = 0; i2 < ARRAY_CNT( mProperty ); i2++ )
	{
		pItemObjEquip->add_property( mProperty[ i2 ] );
	}

	for ( unsigned int i3 = 0; i3 < ARRAY_CNT( mSlotValue ); i3++ )
	{
		pItemObjEquip->add_slotvalue( mSlotValue[ i3 ] );
	}

	//for ( unsigned int i4 = 0; i4 < ARRAY_CNT( mValue ); i4++ )
	//{
	//	pGene->add_value( mValue[ i4 ] );
	//}

	if( mSlotNumber != 0 )
		pItemObjEquip->set_slotnumber( mSlotNumber );
	if( mNameLength != 0 )
		pItemObjEquip->set_namelength( mNameLength );

	if( mName[0] != 0 )
		pItemObjEquip->set_name( mName );
	pItemObjEquip->set_durability( mCurDurability );			

	/*
	pGene->mutable_objguid()->set_creattime( mObjGUID.Data1 );
	pGene->mutable_objguid()->set_eqptid( mObjGUID.Data2 );
	pGene->mutable_objguid()->set_eqptlineid( mObjGUID.Data3 );
	pGene->mutable_objguid()->set_eqptserverid( mObjGUID.Data4 );
	pGene->mutable_objguid()->set_source( mObjGUID.Data5 );
	pGene->mutable_objguid()->set_creatorid( mObjGUID.Data6 ); 
	*/

	//pGene->set_maxslotnumber( mMaxSlotNumber );
	PBPropertyValue1 *tValue = pItemObjEquip->mutable_maxslotnumber();
	if ( tValue != NULL )
	{
		tValue->set_initvalue( mMaxSlotNumber.mInitValue );
		tValue->set_deltavalue( mMaxSlotNumber.mDeltaValue );
	}

	for ( unsigned int i = 0; i < ARRAY_CNT( mJudge ); i++ )
	{
		pItemObjEquip->add_judge( mJudge[ i ] );
	}
	if( mResDef != 0 )
		pItemObjEquip->set_resdef( mResDef );
	if( mResAtt !=0 )
		pItemObjEquip->set_resatt( mResAtt );
	if( mDefLevel != 0 )
		pItemObjEquip->set_deflevel( mDefLevel );
	if( mAttLevel != 0 )
		pItemObjEquip->set_attlevel( mAttLevel );

	// 如果是绑定解除状态则判断是否到期	
	unsigned int TimeNow = time( NULL );
	if ( ( GetBindStatus()&ITEM_BINDSTATUS_FREEBIND ) != 0 )
	{		
		if ( TimeNow >= mBindFreeTime )
		{
			SetBindStatus( GetBindStatus() - ITEM_BINDSTATUS_FREEBIND );
			pGene->set_bindstatus( GetBindStatus() );
			mBindFreeTime = 0;
			// mBindLeftTime = 0;
		}
		//else
		//{
		//	// mBindLeftTime = mBindFreeTime - TimeNow;
		//}
	}
	if( mBindFreeTime != 0 )
		pItemObjEquip->set_bindfreetime( mBindFreeTime );		
	//if( mBindFreeTime > TimeNow )
	//{
	//	pGene->set_bindlefttime( mBindFreeTime - TimeNow );
	//}
	//

	if ( mMagicStoneID != 0 )
	{
		pItemObjEquip->set_magicstoneid( mMagicStoneID );
	}

	if ( mMagicStoneNum != 0 )
	{
		pItemObjEquip->set_magicstonenum( mMagicStoneNum );
	}			

	if ( mSpouseID != 0 )
	{
		pItemObjEquip->set_spouseid( mSpouseID );
	}

	if ( mSpouseName[0] != '\0' )
	{
		pItemObjEquip->set_spousename( mSpouseName );
	}
	pItemObjEquip->set_extratype( mExtraType );

	if ( mBindTime != 0 )
	{
		pItemObjEquip->set_bindtime( mBindTime );
	}
}


int CItemEquipment::GetFixNumber( CTemplateProperty::PROPERTY_FUNC vFunc, int vEquipProType, CPropertyPlayer *pProperty )
{
	int tValue = 0;																							
	CTemplateEquipment* tpTemplate = (CTemplateEquipment*) CDataStatic::SearchTpl( mItemID );			
	if ( tpTemplate == NULL )																				
		return 0;																						
	if(  mCurDurability == 0 )																			
		return 0;	
	if ( mLevel > ITEMUPLIMIT )
		return 0;
	if ( IsOvertimed() == true )
		return 0;

	CTemplateEquipment* tpEquipment = (CTemplateEquipment*) tpTemplate;	

	if (   ( vFunc == CTemplateProperty::PROPERTYFUNC_RESDEF1 && mResDef == RES_TYPE1 )
		|| ( vFunc == CTemplateProperty::PROPERTYFUNC_RESDEF2 && mResDef == RES_TYPE2 )
		|| ( vFunc == CTemplateProperty::PROPERTYFUNC_RESDEF3 && mResDef == RES_TYPE3 )
		|| ( vFunc == CTemplateProperty::PROPERTYFUNC_RESDEF4 && mResDef == RES_TYPE4 )
		|| ( vFunc == CTemplateProperty::PROPERTYFUNC_RESDEF5 && mResDef == RES_TYPE5 )
		|| ( vFunc == CTemplateProperty::PROPERTYFUNC_RESDEF6 && mResDef == RES_TYPE6 ) )
	{
		tValue += tpEquipment->mResDefValue[ mDefLevel ];
	}

	else if (   ( vFunc == CTemplateProperty::PROPERTYFUNC_RESATT1 && mResAtt == RES_TYPE1 )
		|| ( vFunc == CTemplateProperty::PROPERTYFUNC_RESATT2 && mResAtt == RES_TYPE2 )
		|| ( vFunc == CTemplateProperty::PROPERTYFUNC_RESATT3 && mResAtt == RES_TYPE3 )
		|| ( vFunc == CTemplateProperty::PROPERTYFUNC_RESATT4 && mResAtt == RES_TYPE4 )
		|| ( vFunc == CTemplateProperty::PROPERTYFUNC_RESATT5 && mResAtt == RES_TYPE5 )
		|| ( vFunc == CTemplateProperty::PROPERTYFUNC_RESATT6 && mResAtt == RES_TYPE6 ) )
	{
		if ( pProperty != NULL )
		{
			if(    ( tpEquipment->mConRequire[ mResAtt - 1 ] <= ( int )CPropertyFormula::GetCon( pProperty ) )
				&& ( tpEquipment->mStrRequire[ mResAtt - 1 ] <= ( int )CPropertyFormula::GetStr( pProperty ) )
				&& ( tpEquipment->mWisRequire[ mResAtt - 1 ] <= ( int )CPropertyFormula::GetWis( pProperty ) )
				&& ( tpEquipment->mSprRequire[ mResAtt - 1 ] <= ( int )CPropertyFormula::GetSpr( pProperty ) )
				&& ( tpEquipment->mStaRequire[ mResAtt - 1 ] <= ( int )CPropertyFormula::GetSta( pProperty ) ) )
			{
				tValue += tpEquipment->mResAttValue[ mAttLevel ];
			}
		}				
	}

	int tBonus = 0;
	if( ( ( GetBindStatus() & ITEM_BINDSTATUS_BYITEM ) == ITEM_BINDSTATUS_BYITEM ) || ( ( GetBindStatus() & ITEM_BINDSTATUS_FREEBIND ) == ITEM_BINDSTATUS_FREEBIND ) )
		tBonus = tpEquipment->mBindBonus;
	for ( unsigned int i = 0; i < ARRAY_CNT(tpEquipment->mUpgProperty); i ++ )								
	{																								
		if ( tpEquipment->mUpgProperty[ i ] == vFunc )	
		{
			int tExtraValue = 0;
			if ( mExtraType == CTemplateEquipment::EXTRA_FIRST )
				tExtraValue = tpEquipment->mUpgExtra1[ i ];
			else if ( mExtraType == CTemplateEquipment::EXTRA_SECOND )
				tExtraValue = tpEquipment->mUpgExtra2[ i ];
			tValue += ( tpEquipment->mUpgValue[ i ][ mLevel ] + tExtraValue ) * ( 1 + ( mJudge[ i ]  / SERVER_PERCENT_FLOAT ) ) * ( 1 + ( tBonus / SERVER_PERCENT_FLOAT ) );
			if ( vEquipProType == TYPE_PATTACKHIGH )
				tValue += ( tpEquipment->mPAttackDiff[ mLevel ] + tExtraValue ) * ( 1 + ( mJudge[ i ]  / SERVER_PERCENT_FLOAT ) ) * ( 1 + ( tBonus / SERVER_PERCENT_FLOAT ) );
			else if ( vEquipProType == TYPE_MATTACKHIGH )
				tValue += ( tpEquipment->mMAttackDiff[ mLevel ] + tExtraValue ) * ( 1 + ( mJudge[ i ]  / SERVER_PERCENT_FLOAT ) ) * ( 1 + ( tBonus / SERVER_PERCENT_FLOAT ) );
			// 新增加鉴定的数值
			tValue += mJudge[ i ] / 100;
		}	

	}	

	for ( int i = 0; i < JEWELNUM; i ++ )															
	{																								
		CTplItemJewel *tpJewel = (CTplItemJewel*)CDataStatic::SearchTpl( mSlotValue[ i ] );				
		if ( tpJewel == NULL )																			
			continue;																					
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tpJewel->mProperty );	
		if ( tpProperty == NULL )																		
			continue;	
		if ( tpProperty->mPropertyFunc == vFunc )						
			tValue += tpProperty->mIntMinValue;																
	}

	for ( int i = 0; i < PROPERTYNUM; i ++ )												
	{																							
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( mProperty[ i ] );	
		if ( tpProperty == NULL )																		
			continue;
		if ( tpProperty->mPropertyFunc == vFunc )						
			tValue += tpProperty->mIntMinValue;																	
	}

	// 增加附魔处理
	CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( GetMagicAttribute( ) );	
	if ( tpProperty != NULL )																		
	{
		if ( tpProperty->mPropertyFunc == vFunc )
		{
			tValue += tpProperty->mIntMinValue;
		}		
	}

	// 增加符文处理
	CTplItemRune *tpRune = (CTplItemRune*)CDataStatic::SearchTpl( mSlotSpell );
	if ( tpRune != NULL )
	{
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tpRune->mPropertyID );
		if ( tpProperty != NULL && tpProperty->mPropertyFunc == vFunc )
		{
			tValue += tpProperty->mIntMinValue;
		}
	}
	return tValue;																					
}

int CItemEquipment::GetPerNumber( CTemplateProperty::PROPERTY_FUNC vFunc )																	
{																											
	if(  mCurDurability == 0 )																				
		return 0;	
	if ( IsOvertimed() == true )
		return 0;
	int tValue = 0;	

	for ( int i = 0; i < JEWELNUM; i ++ )																	
	{																										
		CTplItemJewel *tpJewel = (CTplItemJewel*)CDataStatic::SearchTpl( mSlotValue[ i ] );					
		if ( tpJewel == NULL )																				
			continue;																						
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tpJewel->mProperty );	
		if ( tpProperty == NULL )																			
			continue;
		if ( tpProperty->mPropertyFunc == vFunc )						
			tValue += tpProperty->mFloatMinValue;															
	}																									
	for ( int i = 0; i < PROPERTYNUM; i ++ )														
	{																										
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( mProperty[ i ] );		
		if ( tpProperty == NULL )																			
			continue;		
		if ( tpProperty->mPropertyFunc == vFunc )						
			tValue += tpProperty->mFloatMinValue;																		
	}
	// 增加附魔处理
	CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( GetMagicAttribute( ) );	
	if ( tpProperty != NULL )																		
	{
		if ( tpProperty->mPropertyFunc == vFunc )
		{
			tValue += tpProperty->mFloatMinValue;
		}		
	}

	// 增加符文处理
	CTplItemRune *tpRune = (CTplItemRune*)CDataStatic::SearchTpl( mSlotSpell );
	if ( tpRune != NULL )
	{
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tpRune->mPropertyID );
		if ( tpProperty != NULL && tpProperty->mPropertyFunc == vFunc )
		{
			tValue += tpProperty->mFloatMinValue;
		}
	}
	return tValue;																						
}

void CItemEquipment::GetSkillList( std::vector< unsigned short >& rSkillList, CTemplateProperty::PROPERTY_FUNC vFunc )								
{																											
	CTemplateEquipment* tpTemplate = (CTemplateEquipment*) CDataStatic::SearchTpl( mItemID );				
	if ( tpTemplate == NULL )																			
		return;																								
	if(  mCurDurability == 0 )																				
		return;	
	for ( int i = 0; i < JEWELNUM; i ++ )																	
	{																										
		CTplItemJewel *tpJewel = (CTplItemJewel*)CDataStatic::SearchTpl( mSlotValue[ i ] );					
		if ( tpJewel == NULL )																				
			continue;																						
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tpJewel->mProperty );	
		if ( tpProperty == NULL )																			
			continue;
		if ( tpProperty->mPropertyFunc == vFunc )							
			rSkillList.push_back( tpProperty->mSkillID );													
	}
	for ( int i = 0; i < PROPERTYNUM; i ++ )																
	{																										
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( mProperty[ i ] );		
		if ( tpProperty == NULL )																			
			continue;
		if ( tpProperty->mPropertyFunc == vFunc )							
			rSkillList.push_back( tpProperty->mSkillID );													
	}																									
}

// 装备附魔
int CItemEquipment::SetMagicStone( int nMagicStoneID )
{	
	CTplItemMagicStone *tpTplMagicStone = ( CTplItemMagicStone * )CDataStatic::SearchTpl( nMagicStoneID );
	if ( tpTplMagicStone == NULL )
	{
		return -1;
	}

	CTemplateEquipment *tpEquipment = ( CTemplateEquipment * )CDataStatic::SearchTpl( mItemID );
	if ( tpEquipment == NULL )
	{							
		return -1;
	}

	if ( tpEquipment->mLevelRequire < tpTplMagicStone->mLevelLimit )
	{
		return ERROR_LIFESKILL_SETMAGICEUIPLEVELLOW;
	}

	if ( !( ( 1 << ( tpEquipment->mEquipType - 1 ) ) & tpTplMagicStone->mPart ) )
	{
		return ERROR_LIFESKILL_SETEQUIPMAGICWRING;
	}


	if ( mMagicStoneID != nMagicStoneID )
	{	 
		mMagicStoneID	= 0;		
		mMagicStoneNum	= 0;
	}	

	if ( mMagicStoneNum < tpTplMagicStone->mUsedTimes && mMagicStoneNum < (int)ARRAY_CNT( tpTplMagicStone->mAttribute ) )
	{		
		if ( tpTplMagicStone->mProbability[mMagicStoneNum] >= rand()%10000 )
		{
			++mMagicStoneNum;
			mMagicStoneID = nMagicStoneID;
		} 
		else
		{
			return ERROR_LIFESKILL_SETMAGICFAILED;
		}		
	}
	else
	{
		return ERROR_LIFESKILL_SETNUMLIMIT;
	}				
	return SUCCESS;
}

// 获取附魔属性
int CItemEquipment::GetMagicAttribute(  )
{
	CTplItemMagicStone *tpTplMagicStone = ( CTplItemMagicStone * )CDataStatic::SearchTpl( mMagicStoneID );
	if ( tpTplMagicStone == NULL || mMagicStoneNum == 0 )
	{
		return 0;
	}	
	if ( mMagicStoneNum > (int)ARRAY_CNT( tpTplMagicStone->mAttribute ) )
	{
		return 0;
	}

	return tpTplMagicStone->mAttribute[mMagicStoneNum-1];
}

// 判断是否是结婚信物
bool CItemEquipment::IsMarriageEquipment()
{
	CTemplateEquipment *tpTplEquipment = ( CTemplateEquipment * )CDataStatic::SearchTpl( GetItemID() );
	if ( tpTplEquipment == NULL )
	{
		return false;
	}
	if ( tpTplEquipment->mEquipType != CTemplateEquipment::PART_MARRY )
	{
		return false;
	}

	return ( mSpouseName[0] != '\0');	
}

// 判断是否是结拜信物
bool CItemEquipment::IsSwornEquipment()
{
	CTemplateEquipment *tpTplEquipment = ( CTemplateEquipment * )CDataStatic::SearchTpl( GetItemID() );
	if ( tpTplEquipment == NULL )
	{
		return false;
	}
	if ( tpTplEquipment->mEquipType != CTemplateEquipment::PART_MARRY )
	{
		return false;
	}

	return ( mSpouseName[0] == '\0');	
}

//////////////////////////////////////////////////////////////////////////
// CItemMagicWeapon
//////////////////////////////////////////////////////////////////////////

void CItemMagicWeapon::Initialize2()
{
	CItemEquipment::Initialize();
	// 初始化法宝数据
	InitializeMW();
}

void CItemMagicWeapon::InitializeMW()
{
	mMWLevel = 0;
	mExp = 0;
	mMp = 0;
	mHp = 0;

	mCurKeenpoints = 0;
	mCurCalmpoints = 0;
	mCurMagicpoints = 0;
	mCurBierpoints = 0;
	mCurImmortalpoints = 0;

	mExtraKeenpoints = 0;	
	mExtraCalmpoints = 0;	
	mExtraMagicpoints = 0;	
	mExtraBierpoints = 0;	
	mExtraImmortalpoints = 0;

	mAddKeenpoints = 0;
	mAddCalmpoints = 0;
	mAddMagicpoints = 0;
	mAddBierpoints = 0;
	mAddImmortalpoints = 0;

	mLeftPoints = 0;

	memset ( mPlayerAddPointLastLevel, 0, sizeof(mPlayerAddPointLastLevel));
	mLeftPointsLastLevel	=	0;
	mNewAddPoints	= 0;	
	mNewAddSkillPoints	= 0;	
	mStepClearState	 = 0;	
	mLastAddedSkillBoxIndex = -1;

	mProBase = 0;
	mProUp = 0;
	mProEff = 0;
	mProSpir = 0;

	mProcessVal = 0;
	mLastLevelProcessVal = 0;
	mLeftSkillPoints = 0;

	mNextActBoxIndex = 0;	
	mLastLevelNextActBox = 0;

	mMaxMp = 0;
	mMaxHp = 0;
	mIncMp = 0;
	mIncHp = 0;

	mProJudgeFlag = 0;

	mMWSkillList.Initialize();

	memset( mTimerID, 0, sizeof(mTimerID) ); 
	mOwnerEntityID = 0;

	mQilinID = 0;

	memset( mSkills, 0, sizeof(mSkills) ); 
	memset( mSkillActNum, 0, sizeof(mSkillActNum) ); 
	memset( mSkillBookID, 0, sizeof(mSkillBookID) ); 
	memset( mAddedSkillPoints, 0, sizeof(mAddedSkillPoints) ); 
	memset( mSkillInitLevel, 0, sizeof(mSkillInitLevel) ); 
	memset( mActiveState, 0, sizeof(mActiveState) );

}

CItemObject* CItemMagicWeapon::BinaryClone2( PBItemObject*& pGene )
{
	CItemMagicWeapon* tpItem = (CItemMagicWeapon*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_MAGICWEAPON );
	LK_ASSERT( tpItem != NULL, return NULL);
	CItemEquipment::SetValueFromPB( (CItemEquipment*)tpItem, pGene );

	PBItemContent* tpItemContent = pGene->mutable_itemcontent();
	LK_ASSERT( tpItemContent != NULL, return NULL );

	PBItemObjMW* tpGeneMW = tpItemContent->mutable_itemmw();
	LK_ASSERT( tpGeneMW != NULL, return NULL );

	tpItem->SetMWLevel( tpGeneMW->mwlevel() );
	tpItem->SetExp( tpGeneMW->exp() );
	tpItem->SetMp( tpGeneMW->mp() );
	tpItem->SetHp( tpGeneMW->hp() );
	tpItem->SetCurKeenpoints( tpGeneMW->curkeenpoints() );
	tpItem->SetCurCalmpoints( tpGeneMW->curcalmpoints() );
	tpItem->SetCurMagicpoints( tpGeneMW->curmagicpoints() );
	tpItem->SetCurBierpoints( tpGeneMW->curbierpoints() );
	tpItem->SetCurImmortalpoints( tpGeneMW->curimmortalpoints() );

	tpItem->SetExtraKeenpoints( tpGeneMW->extrakeenpoints() );
	tpItem->SetExtraCalmpoints( tpGeneMW->extracalmpoints() );
	tpItem->SetExtraMagicpoints( tpGeneMW->extramagicpoints() );
	tpItem->SetExtraBierpoints( tpGeneMW->extrabierpoints() );
	tpItem->SetExtraImmortalpoints( tpGeneMW->extraimmortalpoints() );

	tpItem->SetLeftPoints( tpGeneMW->leftpoints() );
	tpItem->SetProBase( tpGeneMW->probase() );
	tpItem->SetProUp( tpGeneMW->proup() );
	tpItem->SetProEff( tpGeneMW->proeff() );
	tpItem->SetProSpir( tpGeneMW->prospir() );
	tpItem->SetProcessVal( tpGeneMW->processval() );
	tpItem->SetLeftSkillPoints( tpGeneMW->leftskillpoints() );
	tpItem->SetAddKeenpoints( tpGeneMW->addkeenpoints() );
	tpItem->SetAddCalmpoints( tpGeneMW->addcalmpoints() );
	tpItem->SetAddMagicpoints( tpGeneMW->addmagicpoints() );
	tpItem->SetAddBierpoints( tpGeneMW->addbierpoints() );
	tpItem->SetAddImmortalpoints( tpGeneMW->addimmortalpoints() );

	for ( int i = 0; i < std::min( tpGeneMW->skillbox_size(), SKILL_BOX_LMT ); ++i )
	{
		PBSkillBox* tpSkillBox = tpGeneMW->mutable_skillbox(i);
		tpItem->mSkills[i] = tpSkillBox->skills();
		tpItem->mSkillActNum[i] = tpSkillBox->skillactnum();
		tpItem->mSkillBookID[i] = tpSkillBox->skillbookid();
		tpItem->mAddedSkillPoints[i] = tpSkillBox->addedskillpoints();
		tpItem->mSkillInitLevel[i] = tpSkillBox->skillinitlevel();
		tpItem->mActiveState[i] = tpSkillBox->activestate();
	}

	tpItem->SetMaxMp( tpGeneMW->maxmp() );
	tpItem->SetMaxHp( tpGeneMW->maxhp() );
	tpItem->SetIncMp( tpGeneMW->incmp() );
	tpItem->SetIncHp( tpGeneMW->inchp() );
	tpItem->SetProJudgeFlag( tpGeneMW->judgeflag() );

	PBSkillList* tpSkillList = tpGeneMW->mutable_skilllist();
	LK_ASSERT( tpSkillList != NULL, return tpItem );
	tpItem->GetMWSkillList()->CreateFromTlv2( tpSkillList );

	for ( int i = 0; i < std::min( tpGeneMW->playeraddpointlastlevel_size(), (int)POINT_TYPE_NUM ); ++i )
	{
		tpItem->SetPlayerAddPointLastLevel( tpGeneMW->playeraddpointlastlevel( i ), i );
	}

	tpItem->SetLeftPointsLastLevel( tpGeneMW->leftpointslastlevel() );
	tpItem->SetNewAddPoints( tpGeneMW->newaddpoints() );
	tpItem->SetNewAddSkillPoints( tpGeneMW->newaddskillpoints() );
	tpItem->SetStepClearState( tpGeneMW->stepclearstate() );
	tpItem->SetLastAddedSkillBoxIndex( tpGeneMW->lastaddedskillboxindex() );
	tpItem->SetLastLevelProcessVal( tpGeneMW->lastlevelprocessval() );
	tpItem->SetNextActBoxIndex( tpGeneMW->nextactboxindex() );
	tpItem->SetLastLevelNextActBox(tpGeneMW->lastlevelnextactbox());
	tpItem->SetCurQL(tpGeneMW->qltempid());
	return tpItem;
}


CItemObject* CItemMagicWeapon::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ITEM_MAGICWEAPON );
}

void CItemMagicWeapon::GetItemBinary2( PBItemObject*& pItem )
{
	CItemEquipment::GetItemBinary2( pItem );

	PBItemContent* tpItemContent = pItem->mutable_itemcontent();
	LK_ASSERT( tpItemContent != NULL, return );

	PBItemObjMW* tpItemMW = tpItemContent->mutable_itemmw();
	LK_ASSERT( tpItemMW != NULL, return );

	tpItemMW->set_mwlevel( mMWLevel ); 
	tpItemMW->set_exp( mExp ); 
	tpItemMW->set_mp( mMp ); 
	tpItemMW->set_hp( mHp ); 
	tpItemMW->set_curkeenpoints( mCurKeenpoints ); 
	tpItemMW->set_curcalmpoints( mCurCalmpoints ); 
	tpItemMW->set_curmagicpoints( mCurMagicpoints ); 
	tpItemMW->set_curbierpoints( mCurBierpoints ); 
	tpItemMW->set_curimmortalpoints( mCurImmortalpoints ); 
	tpItemMW->set_extrakeenpoints( mExtraKeenpoints ); 
	tpItemMW->set_extracalmpoints( mExtraCalmpoints ); 
	tpItemMW->set_extramagicpoints( mExtraMagicpoints ); 
	tpItemMW->set_extrabierpoints( mExtraBierpoints ); 
	tpItemMW->set_extraimmortalpoints( mExtraImmortalpoints ); 
	tpItemMW->set_leftpoints( mLeftPoints ); 
	tpItemMW->set_probase( mProBase ); 
	tpItemMW->set_proup( mProUp ); 
	tpItemMW->set_proeff( mProEff ); 
	tpItemMW->set_prospir( mProSpir ); 
	tpItemMW->set_processval( mProcessVal ); 
	tpItemMW->set_leftskillpoints( mLeftSkillPoints ); 
	tpItemMW->set_addkeenpoints( mAddKeenpoints ); 
	tpItemMW->set_addcalmpoints( mAddCalmpoints ); 
	tpItemMW->set_addmagicpoints( mAddMagicpoints ); 
	tpItemMW->set_addbierpoints( mAddBierpoints ); 
	tpItemMW->set_addimmortalpoints( mAddImmortalpoints ); 

	for ( int i = 0; i < SKILL_BOX_LMT ; ++i )
	{
		PBSkillBox* tpSkillBox = tpItemMW->add_skillbox();
		tpSkillBox->set_skills(mSkills[i]);
		tpSkillBox->set_skillactnum(mSkillActNum[i]);
		tpSkillBox->set_skillbookid(mSkillBookID[i]);
		tpSkillBox->set_addedskillpoints(mAddedSkillPoints[i]);
		tpSkillBox->set_skillinitlevel(mSkillInitLevel[i]);
		tpSkillBox->set_activestate(mActiveState[i]);
	}

	tpItemMW->set_maxmp( mMaxMp ); 
	tpItemMW->set_maxhp( mMaxHp ); 
	tpItemMW->set_incmp( mIncMp ); 
	tpItemMW->set_inchp( mIncHp ); 
	tpItemMW->set_judgeflag( mProJudgeFlag );

	PBSkillList* tpSkillList = tpItemMW->mutable_skilllist();
	LK_ASSERT( tpSkillList != NULL, return );
	GetMWSkillList()->GetTlvBuffer2( tpSkillList );

	for ( int i = 0; i < POINT_TYPE_NUM ; ++i )
	{
		tpItemMW->add_playeraddpointlastlevel( mPlayerAddPointLastLevel[i] );
	}

	tpItemMW->set_leftpointslastlevel( mLeftPointsLastLevel ); 
	tpItemMW->set_newaddpoints( mNewAddPoints ); 
	tpItemMW->set_newaddskillpoints( mNewAddSkillPoints ); 
	tpItemMW->set_stepclearstate( mStepClearState ); 
	tpItemMW->set_lastaddedskillboxindex( mLastAddedSkillBoxIndex ); 
	tpItemMW->set_lastlevelprocessval( mLastLevelProcessVal );
	tpItemMW->set_nextactboxindex( mNextActBoxIndex);
	tpItemMW->set_lastlevelnextactbox( mLastLevelNextActBox );
	tpItemMW->set_qltempid( mQilinID );
}

// 当装上法宝/进入场景的时间调用 初始化计时器
void CItemMagicWeapon::OnStartMagicWeapon( CEntityPlayer* pPlayer, bool bIsStartTimer )
{
	LK_ASSERT( pPlayer != NULL, return );
	mOwnerEntityID = pPlayer->GetEntityID();
	if (bIsStartTimer)
		CheckTimers();
}

// 检查定时器
void CItemMagicWeapon::CheckTimers()
{
	CEntityPlayer* pPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( mOwnerEntityID );
	if( pPlayer == NULL ) // 不应该这样
	{
		LOG_ERROR( "pro" , "player=null! mOwnerEntityID=%d", mOwnerEntityID );
#ifdef _DEBUG_
		print_trace_fd( "pro" );
#endif
		return; 
	}

	// 如果需要恢复MP
	if ( IsNeedIncMp() )
	{
		if (mTimerID[MW_TIMWR_MP_INC] == 0)
		{
			int nTimerOtherInfo[ TIMER_OTHER_INFO ] = { MESSAGE_PROPERTY, 0, 0, 0};
			mTimerID[MW_TIMWR_MP_INC] = pPlayer->GetTimerReciver()->SetTimer( get_id(), TIMER_MAGICWEAPON_INCMP, MW_INDEC_TIMER
				, 1, nTimerOtherInfo, MAX_INT );
			LOG_DEBUG("pro", "MW starttimer:%d", mTimerID[MW_TIMWR_MP_INC]);
		}
	}
	else
	{
		if ( mTimerID[MW_TIMWR_MP_INC] != 0 )
		{
			pPlayer->GetTimerReciver()->ClearTimer( mTimerID[MW_TIMWR_MP_INC] );
			LOG_DEBUG("pro", "MW cleartimer:%d", mTimerID[MW_TIMWR_MP_INC]);
			mTimerID[MW_TIMWR_MP_INC] = 0;
		}
	}

	// 如果需要扣减MP
	if ( IsNeedDecHp() )
	{
		if (mTimerID[MW_TIMWR_HP_DEC] == 0)
		{
			int nTimerOtherInfo[ TIMER_OTHER_INFO ] = { MESSAGE_PROPERTY, 0, 0, 0};
			mTimerID[MW_TIMWR_HP_DEC] = pPlayer->GetTimerReciver()->SetTimer( get_id(), TIMER_MAGICWEAPON_DECHP, MW_INDEC_TIMER
				, 1, nTimerOtherInfo, MAX_INT );
			LOG_DEBUG("pro", "MW starttimer:%d", mTimerID[MW_TIMWR_HP_DEC]);
		}
	}
	else
	{
		if ( mTimerID[MW_TIMWR_HP_DEC] != 0 )
		{
			pPlayer->GetTimerReciver()->ClearTimer( mTimerID[MW_TIMWR_HP_DEC] );
			LOG_DEBUG("pro", "MW cleartimer:%d", mTimerID[MW_TIMWR_HP_DEC]);
			mTimerID[MW_TIMWR_HP_DEC] = 0;
		}
	}
}

// 当卸下法宝/下线场景的时间调用 干掉计时器
void CItemMagicWeapon::OnEndMagicWeapon( CEntityPlayer* pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return );
	LK_ASSERT( mOwnerEntityID == (int)pPlayer->GetEntityID(), return );

	for ( int i = 0; i < MW_TIMER_NUM; ++i )
	{
		if ( mTimerID[i] != 0 )
		{
			pPlayer->GetTimerReciver()->ClearTimer( mTimerID[i] );
			LOG_DEBUG("pro", "MW cleartimer:%d", mTimerID[i]);
			mTimerID[i] = 0;
		}
	}

	mOwnerEntityID = 0;
}

// 当装上法宝/上线的时候 要启动技能CD
void CItemMagicWeapon::OnStartSkillCD( CEntityPlayer* pOwner )
{
	int nTimerOtherInfo[TIMER_OTHER_INFO] = {MESSAGE_PROPERTY, 0, 0, 0};
	for( int i = 0; i < mMWSkillList.GetSkillNum(); i++ )
	{
		CSkillObject* SkillObj = mMWSkillList.GetSkillObjectByIndex( i );

		if( SkillObj->GetCooldown() > 0 )
		{
			nTimerOtherInfo[1] = SkillObj->GetSkillID();
			nTimerOtherInfo[2] = SkillObj->GetSkillLevel();
			nTimerOtherInfo[3] = get_id();  // 法宝技能 附带法宝对象ID

			int nTimerID = pOwner->GetTimerReciver()->SetTimer( pOwner->GetEntityID(), TIMER_SKLL_COOLDOWN, SkillObj->GetCooldown(), 4, nTimerOtherInfo );

			SkillObj->SetCDTimer( nTimerID );
		}
	}
}

// 当卸下法宝/下线的时候 要停止技能CD
void CItemMagicWeapon::OnStopSkillCD( CEntityPlayer* pOwner )
{
	for( int i = 0; i < mMWSkillList.GetSkillNum(); i++ )
	{
		CSkillObject* pSkillObj = mMWSkillList.GetSkillObjectByIndex( i );
		if( pSkillObj->GetCDTimer() != INVALID_OBJ_ID )
		{
			pOwner->GetTimerReciver()->TimeOutOneTimer( pSkillObj->GetCDTimer() );
			pOwner->GetTimerReciver()->ClearTimer( pSkillObj->GetCDTimer() );
		}
	}
}

// 洗点--小洗
int  CItemMagicWeapon::Return2LastLevel()
{
	//  1级 不能洗了
	if ( mMWLevel <= 1 )
		return ERROR_MW_CANT_CLEARPOINT;
	CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItemMW!=NULL, return -1 );
	CTplMagicWeaponProIncTable* tpLevelTable = (CTplMagicWeaponProIncTable*)CDataStatic::SearchTpl(tpItemMW->mLevelTableID);
	LK_ASSERT( tpLevelTable != NULL, return -1 );

	if ( mStepClearState != 0 )
		return ERROR_MW_STEPCLEARED;

	// 先退回这次系统给加的
	mCurKeenpoints -=		tpLevelTable->mValues[mMWLevel].mLevelKeen;
	mCurCalmpoints -=		tpLevelTable->mValues[mMWLevel].mLevelCalm;
	mCurMagicpoints -=		tpLevelTable->mValues[mMWLevel].mLevelMagic;
	mCurBierpoints -=		tpLevelTable->mValues[mMWLevel].mLevelBier;
	mCurImmortalpoints -=	tpLevelTable->mValues[mMWLevel].mLevelImmortal;

	// 玩家加的5个点 和剩余点 退回到镜像数据
	int tPlayerAdded = 0;
	tPlayerAdded += mAddKeenpoints -		mPlayerAddPointLastLevel[KEENPOINTS];		
	tPlayerAdded += mAddCalmpoints -		mPlayerAddPointLastLevel[CALMPOINTS];		
	tPlayerAdded += mAddMagicpoints -		mPlayerAddPointLastLevel[MAGICPOINTS];	
	tPlayerAdded += mAddBierpoints -		mPlayerAddPointLastLevel[BIERPOINTS];		
	tPlayerAdded += mAddImmortalpoints -	mPlayerAddPointLastLevel[IMMORTALPOINTS];
	mLeftPoints = mLeftPointsLastLevel;

	// 退回玩家这级新加的点
	mCurKeenpoints -=	  ( mAddKeenpoints -		mPlayerAddPointLastLevel[KEENPOINTS]);	
	mCurCalmpoints -=	  ( mAddCalmpoints -		mPlayerAddPointLastLevel[CALMPOINTS]);		
	mCurMagicpoints -=	  ( mAddMagicpoints -		mPlayerAddPointLastLevel[MAGICPOINTS]);	
	mCurBierpoints -=	  ( mAddBierpoints -		mPlayerAddPointLastLevel[BIERPOINTS]);		
	mCurImmortalpoints -= ( mAddImmortalpoints -	mPlayerAddPointLastLevel[IMMORTALPOINTS]);

	// 重置玩家加的点
	mAddKeenpoints =		mPlayerAddPointLastLevel[KEENPOINTS];	
	mAddCalmpoints =		mPlayerAddPointLastLevel[CALMPOINTS];	
	mAddMagicpoints =		mPlayerAddPointLastLevel[MAGICPOINTS];	
	mAddBierpoints =		mPlayerAddPointLastLevel[BIERPOINTS];	
	mAddImmortalpoints =	mPlayerAddPointLastLevel[IMMORTALPOINTS];

	memset( mPlayerAddPointLastLevel, 0, sizeof(mPlayerAddPointLastLevel) );

	// 校验下
	if ( (unsigned int)tPlayerAdded != mLeftPointsLastLevel + mNewAddPoints - mLeftPoints )
	{
		LOG_ERROR("pro", "MW value not match? %d + %d - %d != %d?", mLeftPointsLastLevel, mNewAddPoints, mLeftPoints,tPlayerAdded );
	}
	mLeftPointsLastLevel = 0;

	// 退回进度值
	mProcessVal = mLastLevelProcessVal ;

	// 退回将要启发的技能框
	mNextActBoxIndex = mLastLevelNextActBox ;	

	// 如果升级到本级奖励了技能点
	if ( tpLevelTable->mValues[mMWLevel].mLevelPoints > 0 )
	{
		if (tpLevelTable->mValues[mMWLevel].mLevelPoints != 1)	// 策划说一定是1点！
		{
			LOG_ERROR("pro", "MW AddSkillPoint is %d? MWID=%d", tpLevelTable->mValues[mMWLevel].mLevelPoints, mItemID );
		}

		// 如果有剩余 就扣掉1点
		if ( mLeftSkillPoints > 0 )
		{
			mLeftSkillPoints --;
		}
		else
		{
			// 如果本级玩家已加了技能点
			if ( mLastAddedSkillBoxIndex != -1 )
			{
				if ( mLastAddedSkillBoxIndex >= 0 && mLastAddedSkillBoxIndex < SKILL_BOX_LMT )
				{
					mAddedSkillPoints[mLastAddedSkillBoxIndex]--;
					mMWSkillList.SkillLevelDown(mSkills[mLastAddedSkillBoxIndex]);
					//mLeftSkillPoints ++;
				}
				mLastAddedSkillBoxIndex = -1;
			}
			else
			{
				LOG_ERROR("pro", "MW Player not add sp?? mwid=%d", mItemID );
			}
		}
	}

	// 退回等级
	--mMWLevel;

	// 设置小洗状态
	mStepClearState = 1;

	RefreshPropertys(false);

	// 外围来刷新给客户端
	return 0;
}

// 获得总技能加点数
int CItemMagicWeapon::GetTotalAddSP()
{
	int nRet = 0;
	for (int i = 0; i < SKILL_BOX_LMT; ++i)
	{
		nRet += mAddedSkillPoints[i];
	}
	return nRet;
}

// 法宝洗技能点
int CItemMagicWeapon::ClearMWSkillPoint()
{
	// 遍历各技能框
	for (int i = 0; i < SKILL_BOX_LMT; ++i)
	{
		// 如果加过点 就减掉
		if (mAddedSkillPoints[i] > 0)
		{
			mMWSkillList.SkillLevelDown(mSkills[i], mAddedSkillPoints[i]);
			mLeftSkillPoints += mAddedSkillPoints[i];
			mAddedSkillPoints[i]=0;
		}
	}
	mLastAddedSkillBoxIndex = -1;
	RefreshPropertys(false);
	return 0;
}

// 法宝重置（大洗）
int CItemMagicWeapon::ResetMagicWeapon( CEntityPlayer* pPlayer )
{
	InitializeMW();
	SetupFromTemplate( );
	SetupSkillFromTemplate( pPlayer );
	return 0;
}


// 备份玩家加点数据
void CItemMagicWeapon::BackUpData()
{
	mPlayerAddPointLastLevel[KEENPOINTS	] = mAddKeenpoints;		
	mPlayerAddPointLastLevel[CALMPOINTS	] = mAddCalmpoints;		
	mPlayerAddPointLastLevel[MAGICPOINTS] = mAddMagicpoints;	
	mPlayerAddPointLastLevel[BIERPOINTS	] = mAddBierpoints;		
	mPlayerAddPointLastLevel[IMMORTALPOINTS] = mAddImmortalpoints;
	mLeftPointsLastLevel = mLeftPoints;
	mLastAddedSkillBoxIndex = -1;	// 清空技能加点记录
	mLastLevelProcessVal = mProcessVal;
	mLastLevelNextActBox = mNextActBoxIndex;	
}

// 获得本次升级新加的潜能点
int	 CItemMagicWeapon::GetThisLevelGotPoints()
{
	return mNewAddPoints;
}

// 获得本次升级新加的技能点
int	 CItemMagicWeapon::GetThisLevelGotSkillPoints()
{
	return mNewAddSkillPoints;
}

// 从模板给法宝赋值
int  CItemMagicWeapon::SetupFromTemplate()
{
	CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItemMW!=NULL, return -1 );
	CTplMagicWeaponProIncTable* tpLevelExp = (CTplMagicWeaponProIncTable*)CDataStatic::SearchTpl(tpItemMW->mLevelTableID);
	LK_ASSERT( tpLevelExp != NULL, return -1 );

	// 分别设置 初始 等级、法力、灵气
	SetMWLevel( tpItemMW->mInitLevel );
	InitMpHp( tpItemMW->mInitMp, tpItemMW->mInitHp );

	//(上限、回复速度 就不赋值了 反正随后会刷的)

	SetCurKeenpoints( tpLevelExp->mValues[0].mLevelKeen );
	SetCurCalmpoints( tpLevelExp->mValues[0].mLevelCalm );
	SetCurMagicpoints( tpLevelExp->mValues[0].mLevelMagic );
	SetCurBierpoints( tpLevelExp->mValues[0].mLevelBier );
	SetCurImmortalpoints( tpLevelExp->mValues[0].mLevelImmortal );
	SetLeftSkillPoints( tpLevelExp->mValues[0].mLevelPoints );

	SetProBase( tpItemMW->mInitBase ); 
	SetProUp( tpItemMW->mInitShape );
	SetProEff( tpItemMW->mInitEffect );
	SetProSpir( tpItemMW->mInitSpir );

	return 0;
}

// 从模板初始化技能
int CItemMagicWeapon::SetupSkillFromTemplate( CEntityPlayer* pOwner )
{
	// 为了方便下面的插入技能等操作 要先“装上”法宝 等会再卸下 
	OnStartMagicWeapon( pOwner, false );
	CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItemMW!=NULL, return -1 );
	// 法宝技能部分
	CTplMagicWeaponSkillBoxProTable* tpSkillTable = (CTplMagicWeaponSkillBoxProTable*)CDataStatic::SearchTpl(tpItemMW->mSkillBoxTableID);
	LK_ASSERT( tpSkillTable != NULL, return -1 );

	// 清空技能列表
	mMWSkillList.Clean();

	// 应策划最新要求  基本属性里默认那个技能（黄加号）自动加到第0个框 而技能框模板里的 往后插入
	mActiveState[0] = 1;
	mActiveState[1] = 1;
	int tSkillID = tpItemMW->mDefaultSkill;
	if ( tSkillID != 0 )
		OnInsertSkill( tSkillID, MW_SKILL_INIT_LEVEL+1, 0, false ); // 策划要求:黄加号要从1级开始

	tSkillID = tpItemMW->mDefaultSkill2;
	if ( tSkillID != 0 )
		OnInsertSkill( tSkillID, MW_SKILL_INIT_LEVEL, 1, false );

	for ( int i = 2; i < SKILL_BOX_LMT; ++i )
	{
		mActiveState[i] = (unsigned char)tpSkillTable->mValues[i].mInitState;
		tSkillID = tpSkillTable->mValues[i].mInitSkillID;
		if ( tSkillID != 0 )
		{
			OnInsertSkill( tSkillID, MW_SKILL_INIT_LEVEL, i, false );
		}
		if (mActiveState[i] == 0 && mNextActBoxIndex == 0) // 第一个未激活的框 就是下一个将要被激活的框
		{
			mNextActBoxIndex = i;
		}
	}

	RefreshPropertys(false);

	// 弄完 要“卸下” 因为本来就没装
	OnEndMagicWeapon( pOwner );

	return 0;
}

// 法宝属性鉴定 接口内部判断第几次 外部负责判断/扣除物品
int  CItemMagicWeapon::OnJudgeProperty( int nEffTableID, int nSpirTableID )
{
	CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItemMW!=NULL, return -1 );

	if ( nEffTableID == 0 )
		nEffTableID = tpItemMW->mJudgeEffTableID;

	if ( nSpirTableID == 0 )
		nSpirTableID = tpItemMW->mJudgeSpirTableID;

	CTplMWProJudgeEffTable* tpMWProJudgeEff = (CTplMWProJudgeEffTable*)CDataStatic::SearchTpl( nEffTableID );
	LK_ASSERT( tpMWProJudgeEff!=NULL, return -2 );

	CTplMWProJudgeSpirTable* tpMWProJudgeSpir = (CTplMWProJudgeSpirTable*)CDataStatic::SearchTpl( nSpirTableID );
	LK_ASSERT( tpMWProJudgeSpir!=NULL, return -3 );

	int nPercentIndexEff = 0;
	int nPercentIndexSpir = 0;	// 索引 也就是最终加上的百分比数值-1

	if ( mProJudgeFlag == 0 ) // 首次鉴定
	{
		nPercentIndexEff  = GetRand(tpMWProJudgeEff->mProbFirst, MAX_MW_PERCENT_NUM );  
		nPercentIndexSpir = GetRand(tpMWProJudgeSpir->mProbFirst, MAX_MW_PERCENT_NUM );
	}
	else
	{
		nPercentIndexEff  = GetRand(tpMWProJudgeEff->mProbSecond, MAX_MW_PERCENT_NUM );
		nPercentIndexSpir = GetRand(tpMWProJudgeSpir->mProbSecond, MAX_MW_PERCENT_NUM );
	}

	SetProEff( (nPercentIndexEff+1)*100 );	// 这个值是万分比~
	SetProSpir( (nPercentIndexSpir+1)*100 );	// 这个值是万分比~
	mProJudgeFlag = 1;
	RefreshPropertys(false);
	return 0;
}

// 法宝升级的处理 只能一级一级的升！
void CItemMagicWeapon::LevelUp( )
{
	CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItemMW!=NULL, return  );
	CTplMagicWeaponProIncTable* tpLevelTable = (CTplMagicWeaponProIncTable*)CDataStatic::SearchTpl(tpItemMW->mLevelTableID);
	LK_ASSERT( tpLevelTable != NULL, return  );

	//SetProBase( tpItemMW->mInitBase ); 
	SetProUp( tpItemMW->mInitShape );
	//SetProEff( tpItemMW->mInitEffect );
	//SetProSpir( tpItemMW->mInitSpir );

	// 备份镜像数据
	BackUpData();

	mMWLevel += 1;
	LK_ASSERT( mMWLevel < MAGICWEAPON_MAX_GRADE, return );

	// 增加技能点
	mNewAddSkillPoints = tpLevelTable->mValues[mMWLevel].mLevelPoints;
	mLeftSkillPoints += mNewAddSkillPoints;

	mCurKeenpoints +=  tpLevelTable->mValues[mMWLevel].mLevelKeen;
	mCurCalmpoints +=  tpLevelTable->mValues[mMWLevel].mLevelCalm;
	mCurMagicpoints +=  tpLevelTable->mValues[mMWLevel].mLevelMagic;
	mCurBierpoints +=  tpLevelTable->mValues[mMWLevel].mLevelBier;
	mCurImmortalpoints +=  tpLevelTable->mValues[mMWLevel].mLevelImmortal;

	// 新加潜能点：固定点+随机点
	mNewAddPoints = mProBase;
	if ( mProUp > 0 )
		mNewAddPoints += (rand()%mProUp+1);

	mLeftPoints += mNewAddPoints;
	mStepClearState = 0; // 清除小洗标志

	// 处理进度值增加 和暴击
	mProcessVal += tpLevelTable->mValues[mMWLevel].mLevelProcessVal; 
	if ( (rand()%SERVER_PERCENT_INT) <= (int)tpLevelTable->mValues[mMWLevel].mLevelProcessCurel )
	{
		mProcessVal += tpLevelTable->mValues[mMWLevel].mLevelProcessCurelVal;
	}
	RefreshPropertys();
}

// 升级后尝试激活新技能框(外部 在法宝升级的时候调用 因为设计和客户端通信) 传出激活的第一个索引 返回激活数量
int  CItemMagicWeapon::OnActiveNewSkillBox( int& nFirstActBoxIndex )
{
	CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItemMW!=NULL, return 0 );

	CTplMagicWeaponSkillBoxProTable* tpBoxTable =  (CTplMagicWeaponSkillBoxProTable*)CDataStatic::SearchTpl(tpItemMW->mSkillBoxTableID);
	LK_ASSERT( tpBoxTable != NULL, return 0 );


	int tRet = 0;
	// 修改后，激活先看看

	while ( GetProcessVal() >= (unsigned int)tpBoxTable->mValues[ mNextActBoxIndex ].mSkillBoxNeedVal
		&& mNextActBoxIndex < SKILL_BOX_LMT )
	{
		SetProcessVal( GetProcessVal() - tpBoxTable->mValues[ mNextActBoxIndex ].mSkillBoxNeedVal );
		// 如果已激活 直接跳过
		if ( mActiveState[mNextActBoxIndex] != 0 )
		{
			++mNextActBoxIndex;
			// tRet 一定是0，校验下，以防万一，已经要求过策划了，必须连续激活
			if ( tRet != 0 )
				LOG_ERROR("pro", "MW ERROR! in %s, ret=%d, mwid=%d", __FUNCTION__, tRet, mItemID );
			continue;
		}

		// tNextActIndex被激活了！
		mActiveState[mNextActBoxIndex] = 1;
		if ( tRet == 0 )
			nFirstActBoxIndex = mNextActBoxIndex;
		++mNextActBoxIndex;
		tRet++;
	}

	return tRet;
}


// 使用技能书合成技能 传入 技能框索引 技能书的模板ID 返回 正确0 否则错误码
int  CItemMagicWeapon::OnUseSkillBook( int nBoxIndex, int nBookID, CMessageInsertSkillNotify* pMsg )
{
	if ( nBoxIndex < 0 || nBoxIndex >= SKILL_BOX_LMT )
		return ERROR_MW_ERRORBOXINDEX;

	// 检查能否使用技能书
	CTemplateMagicWeapon* tpItem = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItem, return -1 );
	if ( tpItem->mCanUseBook == 0 )
		return ERROR_MW_CANTUSEBOOK;

	// 检查是否激活
	if ( mActiveState[nBoxIndex] == 0 )
		return ERROR_MW_SKILLBOXNOTACT;

	// 检查颜色是否对应
	// 1207最新修改：技能书颜色可以向下兼容
	CTplMagicWeaponSkillBoxProTable* tpBoxTable =  (CTplMagicWeaponSkillBoxProTable*)CDataStatic::SearchTpl(tpItem->mSkillBoxTableID);
	LK_ASSERT( tpBoxTable != NULL, return -1 );

	CTplItemSkillBook* tpItemBook = (CTplItemSkillBook*)CDataStatic::SearchTpl(nBookID);
	LK_ASSERT( tpItemBook != NULL, return -1 );

	//if ( (tpBoxTable->mValues[nBoxIndex].mSkillBoxColor != tpItemBook->mBookColor) && (tpItemBook->mBookColor != 0) )
	if ( tpBoxTable->mValues[nBoxIndex].mSkillBoxColor > tpItemBook->mBookColor )
		return ERROR_MW_BOOKCOLORERROR;

	// 检查是否已有
	for ( int i = 0; i < SKILL_BOX_LMT; ++i )
	{
		if ( mSkills[i] == (unsigned int)tpItemBook->mSkillID )
			return ERROR_MW_HASEDBOOKSKILL;
	}

	// 开始合成...
	LK_ASSERT( tpItemBook->mSkillID > 0, return ERROR_MW_ERRSKILL );
	int tRet = OnInsertSkill( tpItemBook->mSkillID, MW_SKILL_INIT_LEVEL, nBoxIndex, true ); // TODO: 新插入的技能 是0级  写死
	if ( 0 == tRet )
	{
		// 插入成功
		// 如果已加点 要返还
		mLeftSkillPoints += mAddedSkillPoints[nBoxIndex];
		mAddedSkillPoints[nBoxIndex] = 0;
		mSkillBookID[nBoxIndex] = nBookID;
		// 设置消息
		pMsg->set_index(nBoxIndex);
		pMsg->set_skillid( tpItemBook->mSkillID );
		pMsg->set_curlevel(1);
		pMsg->set_reason(GETSKILL_USEBOOK); // 鉴定的
		pMsg->set_actednum(mSkillActNum[nBoxIndex]);
		pMsg->set_bookid( mSkillBookID[nBoxIndex] );
		return 0;
	}
	else
	{
		return tRet;
	}
}

// 刷新各项属性值 (法宝内部的数值 外部数值在调用的时候计算)
void CItemMagicWeapon::RefreshPropertys( bool bIsStartTimer )
{
	CTemplateMagicWeapon* tpItem = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItem, return );

	// 先刷新额外的5点 从技能 (TODO 那个属性根本没用 直接传NULL)
	mExtraKeenpoints  = mMWSkillList.GetFixNumber(  CTemplateSkill::PASSINC_KEENPOINTS );	
	mExtraCalmpoints  = mMWSkillList.GetFixNumber(  CTemplateSkill::PASSINC_CALMPOINTS );	
	mExtraMagicpoints = mMWSkillList.GetFixNumber(  CTemplateSkill::PASSINC_MAGICPOINTS );		
	mExtraBierpoints  = mMWSkillList.GetFixNumber(  CTemplateSkill::PASSINC_BIERPOINTS );		
	mExtraImmortalpoints  = mMWSkillList.GetFixNumber(  CTemplateSkill::PASSINC_IMMORTALPOINTS );	

	// 印记对5个基本点的影响 12.28修改：万分比
	CTplFuncItem* tpItemQL = (CTplFuncItem*)CDataStatic::SearchTpl( mQilinID );
	if (tpItemQL != NULL)
	{
		//mExtraKeenpoints     += tpItemQL->mParams[QLIDX_KEENPOINTS    ];
		//mExtraCalmpoints     += tpItemQL->mParams[QLIDX_CALMPOINTS	]; 
		//mExtraMagicpoints    += tpItemQL->mParams[QLIDX_MAGICPOINTS	]; 
		//mExtraBierpoints     += tpItemQL->mParams[QLIDX_BIERPOINTS	]; 
		//mExtraImmortalpoints += tpItemQL->mParams[QLIDX_IMMORTALPOINTS]; 
		mExtraKeenpoints     += (GetKeenpoints()	* tpItemQL->mParams[QLIDX_KEENPOINTS    ]/SERVER_PERCENT_INT);
		mExtraCalmpoints     += (GetCalmpoints()	* tpItemQL->mParams[QLIDX_CALMPOINTS	]/SERVER_PERCENT_INT); 
		mExtraMagicpoints    += (GetMagicpoints()	* tpItemQL->mParams[QLIDX_MAGICPOINTS	]/SERVER_PERCENT_INT); 
		mExtraBierpoints     += (GetBierpoints()	* tpItemQL->mParams[QLIDX_BIERPOINTS	]/SERVER_PERCENT_INT); 
		mExtraImmortalpoints += (GetImmortalpoints()* tpItemQL->mParams[QLIDX_IMMORTALPOINTS]/SERVER_PERCENT_INT); 
	}

	// 刷新MP HP上限
	mMaxMp = tpItem->mInitMaxMp + GetCalmpoints()*tpItem->mIncMaxMp +mMWSkillList.GetFixNumber(  CTemplateSkill::PASSINC_MWHPMAX );	
	mMaxHp = tpItem->mInitMaxHp + GetCalmpoints()*tpItem->mIncMaxHp +mMWSkillList.GetFixNumber(  CTemplateSkill::PASSINC_MWMPMAX );	

	int nPlus = (int)((double)tpItem->mMPIncValuePerMagic*GetMagicpoints()/SERVER_PERCENT_INT);
	mIncMp = tpItem->mMPIncValue + nPlus;

	nPlus = (int)((double)tpItem->mHPIncValuePerMagic*GetBierpoints()/SERVER_PERCENT_INT);
	mIncHp = tpItem->mHPIncValue + nPlus; // 这个应该是负数

	if ( bIsStartTimer )
		CheckTimers(); 
}

// 法宝与器灵印记(这里是内部计算，不涉及概率。概率在外围计算)
int  CItemMagicWeapon::OnPrintQilin( int nQilinID )
{
	mQilinID = nQilinID;
	RefreshPropertys(false);
	return 0;
}

// 得到法宝攻击力
unsigned int CItemMagicWeapon::GetAttackValue()
{
	if ( mHp == 0 ) {return 0;}
	CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItemMW!=NULL, return 0 );

	// 计算力量(本体)影响：
	int nHardValue = (int)((double)(GetKeenpoints()*tpItemMW->mAttackInc)/SERVER_PERCENT_INT*mProEff/SERVER_PERCENT_INT);

	// 计算等级影响：
	int nLevelValue = mMWLevel*mLevel;
	return (nHardValue+nLevelValue);
}

// 得到法宝的致命等级
unsigned int CItemMagicWeapon::GetDeathRate()
{
	if ( mHp == 0 ) {return 0;}
	CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItemMW!=NULL, return 0 );

	// 计算力量(本体)影响：
	int nHardValue = (int)((double)(GetImmortalpoints()*tpItemMW->mDeadLineInc)/SERVER_PERCENT_INT*mProEff/SERVER_PERCENT_INT);

	// 计算等级影响：
	int nLevelValue = mMWLevel*mLevel;

	return (nHardValue+nLevelValue);
}

// 定时恢复MP
int CItemMagicWeapon::OnTimerIncMp()
{
	return OnIncMp(GetIncMp());
}

// 定时消耗HP
int CItemMagicWeapon::OnTimerDecHp()
{
	return OnIncHp(GetIncHp());
}

// 死亡掉HP
int CItemMagicWeapon::OnDieCutHp()
{
	CTemplateMagicWeapon* tpItem = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItem, return 0 );
	return OnIncMp((0-tpItem->mDieOffHp));
}

// 对外增加(减少)MP接口 返回实际增加/减少值
int  CItemMagicWeapon::OnIncMp( int nVal, bool bIsStartTimer )
{
	if (nVal == 0)
		return 0;

	if ( (nVal >0 ) && ( GetMp() >= GetMaxMp() ) )
		return 0;

	if ( (nVal <0 ) && ( GetMp() <= 0 ) )
		return 0;

	int nAddMp = 0;
	if ( nVal > 0 )
		nAddMp = ( GetMaxMp()-GetMp() >= (unsigned int)nVal ) ? nVal : (GetMaxMp()-GetMp());
	else // 减去
		nAddMp = ( GetMp() >= (0-(unsigned int)nVal) ) ? nVal : (0-GetMp());

	SetMp( GetMp() + nAddMp );
	if (bIsStartTimer)
		CheckTimers();
	return nAddMp;
}

// 对外增加(减少)HP接口 返回实际增加/减少值
int  CItemMagicWeapon::OnIncHp( int nVal )
{
	if (nVal == 0)
		return 0;

	if ( (nVal >0 ) && ( GetHp() >= GetMaxHp() ) )
		return 0;

	if ( (nVal <0 ) && ( GetHp() <= 0 ) )
		return 0;

	int nAddHp = 0;
	if ( nVal > 0 )
		nAddHp = ( GetMaxHp()-GetHp() >= (unsigned int)nVal ) ? nVal : (GetMaxHp()-GetHp());
	else // 减去
		nAddHp = ( GetHp() >= (0-(unsigned int)nVal) ) ? nVal : (0-GetHp());

	SetHp( GetHp() + nAddHp );
	CheckTimers();
	return nAddHp;
}

// 加点(仅限于玩家加点)
int CItemMagicWeapon::OnAddPotential( unsigned int nNewKeenpoints, unsigned int nNewCalmpoints, unsigned int nNewMagicpoints, unsigned int nNewBierpoints, unsigned int nNewImmortalpoints )
{
	unsigned int nTotal =  nNewKeenpoints +  nNewCalmpoints +  nNewMagicpoints +  nNewBierpoints +  nNewImmortalpoints;
	if ( nTotal > mLeftPoints )
		return ERROR_MW_NOT_ENOUGH_POINTS;

	mCurKeenpoints += 	  nNewKeenpoints  ;		
	mCurCalmpoints += 	  nNewCalmpoints  	;	
	mCurMagicpoints += 	  nNewMagicpoints  ;	
	mCurBierpoints += 	  nNewBierpoints  	;	
	mCurImmortalpoints +=  nNewImmortalpoints ;	

	mAddKeenpoints += 	  nNewKeenpoints  ;		
	mAddCalmpoints += 	  nNewCalmpoints  	;	
	mAddMagicpoints += 	  nNewMagicpoints  ;	
	mAddBierpoints += 	  nNewBierpoints  	;	
	mAddImmortalpoints +=  nNewImmortalpoints ;	

	mLeftPoints -= nTotal;
	RefreshPropertys();
	return 0;
}

// 插入技能 
int  CItemMagicWeapon::OnInsertSkill( int nSkillID, int nSkillLevel, int nBoxIndex, bool bIsStartTimer )
{
	if ( nBoxIndex < 0 || nBoxIndex >= SKILL_BOX_LMT )
	{
		return ERROR_MW_SKILLBOXNOTACT;
	}

	if ( mActiveState[nBoxIndex] == 0 ) // 判断是否已激活
	{
		return ERROR_MW_SKILLBOXNOTACT;
	}

	// 检查技能是否重复 除了目标框 都不能重复
	for ( int i = 0; i < SKILL_BOX_LMT; ++i )
	{
		if ( i != nBoxIndex )
		{
			if ( mSkills[i] == (unsigned int)nSkillID )
			{
				LOG_WARN("pro", "error!!exsited skill!! skill id=%d  mwid=%d", nSkillID, mItemID );
				return ERROR_MW_ERRSKILL;
			}
		}
	}

	CEntityPlayer* pPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( mOwnerEntityID );
	LK_ASSERT( pPlayer != NULL, return -1 );
	// 判断等级是否满足
	CTemplateSkill* tpSkill = (CTemplateSkill*)CDataStatic::SearchSkill( nSkillID, nSkillLevel );
	LK_ASSERT( tpSkill != NULL, return ERROR_MW_NOSUCKSKILL );

	if ( GetMWLevel() < tpSkill->mStudySkillLevel )
		return ERROR_MW_SKILLNEEDMORELEVEL;

	int tOldSkill =  mSkills[nBoxIndex];

	mSkills[nBoxIndex] = nSkillID;
	mSkillInitLevel[nBoxIndex] = nSkillLevel;

	if ( tOldSkill != 0 ) // 要从skilllist干掉老的
	{
		mMWSkillList.RemoveSkill( (unsigned short)tOldSkill );
	}

	if( mMWSkillList.InsertSkill( (unsigned short)nSkillID, nSkillLevel, nSkillLevel ) )
	{
		RefreshPropertys(bIsStartTimer);
		return 0;
	}
	return -1;
}

// 法宝技能加点 成功返回0
int  CItemMagicWeapon::OnClickSP( int nBoxIndex )
{
	if ( nBoxIndex < 0 || nBoxIndex >= SKILL_BOX_LMT )
	{
		return ERROR_MW_SKILLBOXNOTACT;
	}

	if ( mLeftSkillPoints <= 0 )
	{
		return ERROR_MW_SKILLNOPOINTS;
	}

	if ( mSkills[nBoxIndex] == 0 )
	{
		return ERROR_MW_NOSUCKSKILL;
	}

	CEntityPlayer* pPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( mOwnerEntityID );
	LK_ASSERT( pPlayer != NULL, return -1 );
	// 判断等级是否满足
	CTemplateSkill* tpSkill =  (CTemplateSkill*)CDataStatic::SearchSkill( mSkills[nBoxIndex], mSkillInitLevel[nBoxIndex]+mAddedSkillPoints[nBoxIndex]+1 );
	if( tpSkill == NULL )
		return ERROR_MW_SKILLMAXLEVEL;

	if ( pPlayer->GetLevel() < tpSkill->mStudySkillLevel )
		return ERROR_MW_SKILLNEEDMORELEVEL;

	// 可以加点
	// 检查技能是否增加 (最大等级就给他+1， 也就是说只要前面判断满足了 这里永远不会加到“最大”等级 没有技能树...)

	if ( mMWSkillList.InsertSkill( (unsigned short)mSkills[nBoxIndex], (int)(mSkillInitLevel[nBoxIndex]+mAddedSkillPoints[nBoxIndex]+1) ) == false )
		return ERROR_CLICKSP_SKILLOVERFLOW;

	++mAddedSkillPoints[nBoxIndex];
	--mLeftSkillPoints;
	mLastAddedSkillBoxIndex = nBoxIndex;	// 记录上次加点的技能框索引 只记录1个就行了
	RefreshPropertys();
	return 0;
}

// 得到某技能框的技能 该抽取的抽取 参数：技能框索引，是否第一次 返回：技能ID
int CItemMagicWeapon::GetBoxSkillNewID( int nBoxIndex, bool bIsFirst )
{
	if ( nBoxIndex < 0 || nBoxIndex >= SKILL_BOX_LMT )
	{
		return ERROR_MW_SKILLBOXNOTACT;
	}
	CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItemMW!=NULL, return 0 );

	// 1130新做：印记后会替表
	int nSkillBoxID = tpItemMW->mSkillBoxTableID;
	if (mQilinID != 0) // 打器灵了
	{
		CTplFuncItem* tpItemQL = (CTplFuncItem*)CDataStatic::SearchTpl( mQilinID );
		LK_ASSERT( tpItemQL, return 0 );
		if (tpItemQL->mParams[QLIDX_REPLYSKILLBOC] != 0)
			nSkillBoxID = tpItemQL->mParams[QLIDX_REPLYSKILLBOC];
	}
	CTplMagicWeaponSkillBoxProTable* tpSkillTable = (CTplMagicWeaponSkillBoxProTable*)CDataStatic::SearchTpl(nSkillBoxID);
	LK_ASSERT( tpSkillTable != NULL, return 0 );

	if ( tpSkillTable->mValues[nBoxIndex].mSkillBoxType == CTplMagicWeaponSkillBoxProTable::TYPE_STATIC )
	{
		return tpSkillTable->mValues[nBoxIndex].mSkillBoxSkill;
	}

	CTplSkillStorage* tpSkillStorage = (CTplSkillStorage*)CDataStatic::SearchTpl( tpSkillTable->mValues[nBoxIndex].mSkillBoxSkillStorage );
	if( tpSkillStorage == NULL )
	{
		LOG_ERROR("pro", "no such tpSkillStorage id=%d", tpSkillTable->mValues[nBoxIndex].mSkillBoxSkillStorage );
		return ERROR_MW_ERRSKILL;
	}

	int prob[SKILL_STORAGE_LMT] = {0};
	int i = 0;
	for ( i = 0; i < SKILL_STORAGE_LMT; ++i )
	{
		if (bIsFirst)
		{
			prob[i] = tpSkillStorage->mValues[i].mFirstProb;
		}
		else
		{
			prob[i] = tpSkillStorage->mValues[i].mOtherProb;
		}
	}

	int tRndIndex = ::GetRand( prob, SKILL_STORAGE_LMT );
	LK_ASSERT( tRndIndex >= 0 && tRndIndex < SKILL_STORAGE_LMT, return 0 );
	return tpSkillStorage->mValues[tRndIndex].mSkillID;
}

// 领悟技能  输出实际领悟的索引列表 返回数量 在升级的时候调用
int  CItemMagicWeapon::OnSelfLearnSkill( unsigned char arrIndex[] )
{
	CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( mItemID );
	LK_ASSERT( tpItemMW!=NULL, return 0 );

	int nRet = 0;
	// 先找已激活的空技能框
	for ( int i = 0; i < SKILL_BOX_LMT; ++i )
	{
		if ( mActiveState[i] != 0 && mSkills[i] == 0 ) // 已激活的空框
		{
			// 计算概率
			if ( (rand()%SERVER_PERCENT_INT) > tpItemMW->mGetSkillScl )
				continue;

			int tSkillID = GetBoxSkillNewID( i, true );
			if ( tSkillID != 0 )
			{
				// TODO 目前新学的技能都是0级的
				int tRet = OnInsertSkill( tSkillID, MW_SKILL_INIT_LEVEL, i ) ;
				if ( 0 == tRet )
					arrIndex[nRet++] = i;
				else
					LOG_ERROR("pro", "error in insert skill? ret = %d", tRet );
			}
		}
	}
	return nRet;
}

// 得到法宝技能基础值 主要用于发给客户端
void CItemMagicWeapon::GetSkillBaseInfo( unsigned char nBoxIndex, int& rSkillID, int& rSkillLevel, int& rActedNum, int& rBookID )
{
	LK_ASSERT( nBoxIndex >= 0 && nBoxIndex < SKILL_BOX_LMT , return );
	rSkillID = mSkills[nBoxIndex];
	rSkillLevel = mSkillInitLevel[nBoxIndex] + mAddedSkillPoints[nBoxIndex];
	rActedNum = mSkillActNum[nBoxIndex];
	rBookID = mSkillBookID[nBoxIndex];
}

// 新鉴定技能 一定要加技能后再增加鉴定次数   参数：技能框ID 传出是否第一次鉴定 发给客户端的消息  返回：0  成功 否则错误码
int  CItemMagicWeapon::OnJudgeNewSkill( int nBoxIndex, bool& bIsFirst, CMessageInsertSkillNotify* pMsg )
{
	LK_ASSERT( pMsg!=NULL, return -1 );
	if ( nBoxIndex < 0 || nBoxIndex >= SKILL_BOX_LMT )
	{
		return ERROR_MW_SKILLBOXNOTACT;
	}

	if ( mActiveState[nBoxIndex] == 0 ) // 未激活
	{
		return ERROR_MW_SKILLBOXNOTACT;
	}

	int nSkillID = GetBoxSkillNewID( nBoxIndex, (mSkillActNum[nBoxIndex]==0) );
	if ( nSkillID == 0 ) // 不能鉴定
		return ERROR_MW_CANTJUDGE;
	if ( nSkillID > 0 )
	{
		int tRet = OnInsertSkill( nSkillID, MW_SKILL_INIT_LEVEL, nBoxIndex, false ); // TODO: 新插入的技能 是0级  写死
		if ( 0 == tRet )
		{
			bIsFirst = (mSkillActNum[nBoxIndex]==0);
			// 插入成功
			// 增加鉴定次数
			++mSkillActNum[nBoxIndex];
			// 如果已加点 要返还
			mLeftSkillPoints += mAddedSkillPoints[nBoxIndex];
			mAddedSkillPoints[nBoxIndex] = 0;
			mSkillBookID[nBoxIndex] = 0;
			// 设置消息
			pMsg->set_index(nBoxIndex);
			pMsg->set_skillid( nSkillID );
			pMsg->set_curlevel(MW_SKILL_INIT_LEVEL);
			pMsg->set_reason(GETSKILL_JUDGE); // 鉴定的
			pMsg->set_actednum(mSkillActNum[nBoxIndex]);
			pMsg->set_bookid( mSkillBookID[nBoxIndex] );
			return 0;
		}
		else
		{
			return tRet;
		}
	}
	return nSkillID; // 返回错误码

}

// 提升玩家攻击力
int CItemMagicWeapon::GetAttackForPlayer( CPropertyPlayer* pProperty )
{
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)pProperty->GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
	if ( tpMW == NULL )
		return 0;
	return CMagicWeaponFormula::GetAttack( tpMW->mItemID, tpMW->GetKeenpoints(), tpMW->GetProEff(), tpMW->GetMWLevel()
		, tpMW->GetLevel(), tpMW->GetProSpir(), tpMW->GetHp() );
}

// 提升玩家防御力
int CItemMagicWeapon::GetRecoveryForPlayer( CPropertyPlayer* pProperty )
{
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)pProperty->GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
	if ( tpMW == NULL )
		return 0;
	return CMagicWeaponFormula::GetRecovery( tpMW->mItemID, tpMW->GetCalmpoints(),  tpMW->GetProEff(), tpMW->GetMWLevel()
		, tpMW->GetLevel(), tpMW->GetProSpir(), tpMW->GetHp() );
}

// 提升玩家MP上限
int CItemMagicWeapon::GetMaxMpForPlayer( CPropertyPlayer* pProperty )
{
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)pProperty->GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
	if ( tpMW == NULL )
		return 0;
	return CMagicWeaponFormula::GetMaxMp( tpMW->mItemID, tpMW->GetMagicpoints(),  tpMW->GetProEff(), tpMW->GetMWLevel()
		, tpMW->GetLevel(), tpMW->GetProSpir(), tpMW->GetHp() );
}

// 提升玩家HP上限
int CItemMagicWeapon::GetMaxHpForPlayer( CPropertyPlayer* pProperty )
{
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)pProperty->GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
	if ( tpMW == NULL )
		return 0;
	return CMagicWeaponFormula::GetMaxHp( tpMW->mItemID, tpMW->GetBierpoints(),  tpMW->GetProEff(), tpMW->GetMWLevel()
		, tpMW->GetLevel(), tpMW->GetProSpir(), tpMW->GetHp() );
}

// 提升玩家致命等级
int CItemMagicWeapon::GetDeadLineForPlayer( CPropertyPlayer* pProperty )
{
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)pProperty->GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
	if ( tpMW == NULL )
		return 0;
	return CMagicWeaponFormula::GetDeadLine( tpMW->mItemID, tpMW->GetImmortalpoints(),  tpMW->GetProEff(), tpMW->GetMWLevel()
		, tpMW->GetLevel(), tpMW->GetProSpir(), tpMW->GetHp() );
}


//////////////////////////////////////////////////////////////////////////
// CItemBook
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemBook::BinaryClone2( PBItemObject*& pGene )
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ITEM_BOOK );
}


CItemObject* CItemBook::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ITEM_BOOK );
}

//////////////////////////////////////////////////////////////////////////
// CItemEquipment
//////////////////////////////////////////////////////////////////////////
int CItemEquipment::ReduceDurability( int NpcType , int PlayerAlive )
{  
	CTemplateEquipment *tpEquipment = ( CTemplateEquipment * )CDataStatic::SearchTpl( mItemID );
	if ( tpEquipment == NULL )
	{
		LOG_ERROR("pro"," [%s,%d][%s]Item( ItemID :%d)  does not exist int template! ",__LK_FILE__,__LINE__,__FUNCTION__ , mItemID );
		return -1;
	}
	if ( mItemID == 0 )
	{		
		return 0;
	}	

	if ( mCurDurability == 0 )
	{		
		return 0;
	}	
	if ( mCurDurability == -1 )
	{
		LOG_DEBUG( "pro","[%s,%d][%s] Equipment %d does not has Durability ",__LK_FILE__,__LINE__,__FUNCTION__ , mItemID );
		return 0;
	}

	if ( PlayerAlive != 1 )
	{
		if ( CDataStatic::spBeginner == NULL )
		{
			return 0;
		}
		CTemplateBeginner *tpBeginner = ( CTemplateBeginner * )CDataStatic::spBeginner;		
		int LostDura = (tpEquipment->mEquipmentDurability*tpBeginner->mDeathDuraRate)/SERVER_PERCENT_INT;
		mAbrasionCount += (tpEquipment->mEquipmentDurability*tpBeginner->mDeathDuraRate)%SERVER_PERCENT_INT;				
		LostDura += mAbrasionCount/SERVER_PERCENT_INT;
		mAbrasionCount = mAbrasionCount%SERVER_PERCENT_INT;		
		if ( LostDura <= 0 )
		{
			return 0;
		}
		mCurDurability -= LostDura;		
		if ( mCurDurability <= 0 )
		{
			mCurDurability = 0;
		}		
		return 1;
	}	  

	switch( NpcType )
	{
	case 1:                    // 与npc进行了交互
		{
			mAbrasionCount += tpEquipment->mNpcAbrasionRate;
		}
		break;
	case 0:					  // 与玩家进行了交互
		{
			mAbrasionCount += tpEquipment->mPlayerAbrasionRate;
		}
		break;
	default:
		break;
	}
	if ( mAbrasionCount >= SERVER_PERCENT_INT )
	{
		mCurDurability -= 1;		
		mAbrasionCount %= SERVER_PERCENT_INT;
		return 1;
	}
	return 0;

}

CItemObject* CItemEquipment::BinaryClone2( PBItemObject*& pGene )
{
	CItemEquipment* tpItem = (CItemEquipment*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_EQUIPMENT );
	SetValueFromPB( tpItem, pGene );
	return tpItem;
}

// 把赋值部分取出来 因为继承自本类的子类要调用这部分内容
void CItemEquipment::SetValueFromPB( CItemEquipment* tpItem, PBItemObject*& pGene )
{
	LK_ASSERT( tpItem != NULL, return );

	PBItemObjEquip* pItemEquip = pGene->mutable_itemcontent()->mutable_itemequip();

	tpItem->SetLevel( pItemEquip->level() );
	tpItem->SetSlotSpell( pItemEquip->slotspell() );

	for ( int i1 = 0; i1 < (int)ARRAY_CNT( tpItem->mSlotProperty ) && i1 < pItemEquip->slotproperty_size(); i1++ )
	{
		tpItem->SetSlotProperty( i1, pItemEquip->slotproperty( i1 ));
	}

	for ( int i2 = 0; i2 < (int)ARRAY_CNT( tpItem->mProperty ) && i2 < pItemEquip->property_size(); i2++ )
	{
		tpItem->SetProperty( i2, pItemEquip->property( i2 ) );
	}

	for ( int i3 = 0; i3 < (int)ARRAY_CNT( tpItem->mSlotValue ) && i3 < pItemEquip->slotvalue_size(); i3++ )
	{
		tpItem->SetSlotValue( i3, pItemEquip->slotvalue( i3 ) );
	}

	tpItem->SetSlotNumber( pItemEquip->slotnumber() );
	tpItem->SetNameLength( pItemEquip->namelength() );
	tpItem->SetDurability( pItemEquip->durability() );	
	tpItem->SetBindFreeTime( pItemEquip->bindfreetime() );


	if ( ( tpItem->GetBindStatus()&ITEM_BINDSTATUS_FREEBIND ) != 0 )
	{
		unsigned int TimeNow = time( NULL );
		if ( TimeNow >= (unsigned int)tpItem->GetBindFreeTime() )
		{
			tpItem->SetBindStatus( tpItem->GetBindStatus() - ITEM_BINDSTATUS_FREEBIND );			
			tpItem->SetBindFreeTime( 0 );
		}

	}

	tpItem->SetAbrasionCount( 0 );
	tpItem->SetName( pItemEquip->name().c_str() );

	PBPropertyValue1 *tValue = pItemEquip->mutable_maxslotnumber();
	if ( tValue != NULL )
	{
		tpItem->SetMaxSlotNumberInit( tValue->initvalue() );
		tpItem->SetMaxSlotNumberDelta( tValue->deltavalue() );
	}
	for ( int i = 0; i < (int)ARRAY_CNT( tpItem->mJudge ) && i < pItemEquip->judge_size(); i++ )
	{
		tpItem->SetJuge( i, pItemEquip->judge( i ) );
	}
	tpItem->SetResDef( pItemEquip->resdef() );
	tpItem->SetResAtt( pItemEquip->resatt() );
	tpItem->SetDefLevel( pItemEquip->deflevel() );
	tpItem->SetAttLevel( pItemEquip->attlevel() ); 
	tpItem->SetMagicStoneID( pItemEquip->magicstoneid() );
	tpItem->SetMagicStoneNum( pItemEquip->magicstonenum() );	
	tpItem->SetSpouseID( pItemEquip->spouseid() );
	tpItem->SetSpouseName( pItemEquip->mutable_spousename()->c_str() );
	tpItem->SetExtraType( pItemEquip->extratype() );
	tpItem->SetBindTime( pItemEquip->bindtime() );
}

CItemObject* CItemEquipment::Creator(unsigned int vItemID, unsigned int vNumber )
{
	CItemEquipment* tpEquip = ( CItemEquipment* )CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_EQUIPMENT );	
	SetValueFromCreator( tpEquip, vItemID, vNumber );
	return tpEquip;
}

// 把赋值部分取出来 因为继承自本类的子类要调用这部分内容
void CItemEquipment::SetValueFromCreator( CItemEquipment* tpEquip, unsigned int vItemID, unsigned int vNumber )
{
	LK_ASSERT( tpEquip != NULL, return );
	tpEquip->GetPropertyFromTpl( vItemID );
	tpEquip->SetBindStatus( ITEM_BINDSTATUS_FREE  );
	tpEquip->SetItemNum( vNumber );
}
//////////////////////////////////////////////////////////////////////////
// CItemPhysic
//////////////////////////////////////////////////////////////////////////
void CItemPhysic::GetItemBinary2( PBItemObject*& pGene )
{
	if( mRemainHP != 0 )
		pGene->mutable_itemcontent()->set_remainhp( mRemainHP );
	if( mRemainMP != 0 )
		pGene->mutable_itemcontent()->set_remainmp( mRemainMP );
	if( mRemainAP != 0 )
		pGene->mutable_itemcontent()->set_remainap( mRemainAP );
	if( mCooldown != 0 )
		pGene->set_cooldown( mCooldown );
}


CItemObject* CItemPhysic::BinaryClone2( PBItemObject*& pGene )
{
	CItemPhysic* tpItem = ( CItemPhysic* )CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_PHYSIC );

	if ( tpItem == NULL )
		return NULL;

	tpItem->mRemainHP = pGene->itemcontent().remainhp();
	tpItem->mRemainMP = pGene->itemcontent().remainmp();
	tpItem->mRemainAP = pGene->itemcontent().remainap();
	tpItem->mCooldown = pGene->cooldown();
	return tpItem;
}



CItemObject* CItemPhysic::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_PHYSIC );
}

//////////////////////////////////////////////////////////////////////////
// CItemHorse
//////////////////////////////////////////////////////////////////////////



CItemObject* CItemHorse::BinaryClone2( PBItemObject*& pGene )
{
	CItemHorse* tpItem = ( CItemHorse* )CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_HORSE );

	if ( tpItem == NULL )
		return NULL;

	tpItem->mCooldown = pGene->cooldown();
	return tpItem;
}


CItemObject* CItemHorse::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_HORSE );
}

//////////////////////////////////////////////////////////////////////////
// CItemKitBag
//////////////////////////////////////////////////////////////////////////
CItemObject* CItemKitBag::Creator( unsigned int vItemID, unsigned int vNumber )
{
	return (CItemObject *)CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_KITBAG );
}

void CItemKitBag::GetItemBinary2( PBItemObject*& pKitBag )
{
}

CItemObject *CItemKitBag::BinaryClone2( PBItemObject*& pKitBag )
{
	CItemKitBag* tpItem = ( CItemKitBag* )CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_KITBAG );

	if ( tpItem == NULL )
		return NULL;

	return tpItem;
}

//////////////////////////////////////////////////////////////////////////
// CItemNote
//////////////////////////////////////////////////////////////////////////



CItemObject* CItemNote::BinaryClone2( PBItemObject*& pGene )
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_NOTE );	
}


CItemObject* CItemNote::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_NOTE );
}


//////////////////////////////////////////////////////////////////////////
// CItemStuff
//////////////////////////////////////////////////////////////////////////


CItemObject* CItemStuff::BinaryClone2( PBItemObject*& pGene )
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_STUFF );	
}



CItemObject* CItemStuff::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_STUFF );
}

//////////////////////////////////////////////////////////////////////////
// CItemEquipUpg
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemEquipUpg::BinaryClone2( PBItemObject*& pGene )
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_EQUIPUPG );
}

CItemObject* CItemEquipUpg::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_EQUIPUPG );
}

//////////////////////////////////////////////////////////////////////////
// CItemQuestPaper
//////////////////////////////////////////////////////////////////////////

CItemObject *CItemQuestPaper::BinaryClone2( PBItemObject*& pGene )
{
	return ( CItemObject * )CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ITEM_QUESTIONPAPER );
}
CItemObject *CItemQuestPaper::Creator( unsigned int vItemID , unsigned int vNumber )
{
	return ( CItemObject * )CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ITEM_QUESTIONPAPER );
}


//////////////////////////////////////////////////////////////////////////
// CItemExp
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemExp::BinaryClone2( PBItemObject*& pGene )
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_EXP );
}


CItemObject* CItemExp::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_EXP );
}

//////////////////////////////////////////////////////////////////////////
// CItemRelife
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemRelive::BinaryClone2( PBItemObject*& pGene )
{
	CItemRelive* tpRelife = (CItemRelive*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_RELIFE );
	LK_ASSERT( tpRelife != NULL, return NULL );

	tpRelife->mCooldown = pGene->cooldown();
	return (CItemObject*)tpRelife;
}


CItemObject* CItemRelive::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_RELIFE );
}
//////////////////////////////////////////////////////////////////////////
// CItemBindStone
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemBindStone::BinaryClone2( PBItemObject*& pGene )
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_BINDSTONE );
}


CItemObject* CItemBindStone::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_BINDSTONE );
}
//////////////////////////////////////////////////////////////////////////
// CItemShout
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemShout::BinaryClone2( PBItemObject*& pGene )
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_SHOUT );
}


CItemObject* CItemShout::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_SHOUT );
}
//////////////////////////////////////////////////////////////////////////
// CItemAutoShout
//////////////////////////////////////////////////////////////////////////
CItemObject* CItemAutoShout::BinaryClone2( PBItemObject*& pGene )
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_AUTOSHOUT );
}


CItemObject* CItemAutoShout::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_AUTOSHOUT );
}


//////////////////////////////////////////////////////////////////////////
// CItemReturn
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemReturn::BinaryClone2( PBItemObject*& pGene )
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_RETURN );
}

CItemObject* CItemReturn::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_RETURN );
}


//////////////////////////////////////////////////////////////////////////
// CItemFuncItem
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemFuncItem::BinaryClone2( PBItemObject*& pGene )
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_FUNCITEM );
}

CItemObject* CItemFuncItem::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_FUNCITEM );
}


//////////////////////////////////////////////////////////////////////////
// CItemTeleport
//////////////////////////////////////////////////////////////////////////
int CItemTeleport::Initialize ()
{
	CItemObject::Initialize();
	mLineID = 0;
	mMapID = 0;
	mPosX = 0;
	mPosY = 0;
	return 0;
}
void CItemTeleport::GetItemBinary2( PBItemObject*& pGene )
{
	// 普通物品只有一个传送点
	PBTelePostion* pTelePos = pGene->mutable_itemcontent()->add_telepos();
	LK_ASSERT( pTelePos != NULL, return );
	pTelePos->set_line( mLineID );
	pTelePos->set_map( mMapID );
	pTelePos->set_posx( mPosX );
	pTelePos->set_posy( mPosY );
}

CItemObject* CItemTeleport::BinaryClone2( PBItemObject*& pGene )
{
	CItemTeleport* tpItem = (CItemTeleport*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_TELEPORT );
	LK_ASSERT( tpItem != NULL, return NULL );

	// 普通物品只有一个传送点
	if( pGene->mutable_itemcontent()->telepos_size() == 1 )
	{
		const PBTelePostion& rTelePos = pGene->mutable_itemcontent()->telepos(0);
		tpItem->mLineID = rTelePos.line();
		tpItem->mMapID = rTelePos.map();
		tpItem->mPosX = rTelePos.posx();
		tpItem->mPosY = rTelePos.posy();
	}


	return (CItemObject*)tpItem;
}

CItemObject* CItemTeleport::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_TELEPORT );
}


//////////////////////////////////////////////////////////////////////////
// CItemTask
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemTask::BinaryClone2( PBItemObject*& pGene )
{
	CItemTask* tpItem = (CItemTask*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_TASK );
	LK_ASSERT( tpItem != NULL, return NULL );

	tpItem->mCooldown = pGene->cooldown();
	return tpItem;
}


CItemObject* CItemTask::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject ( OBJTYPE_ITEM_TASK );
}



//////////////////////////////////////////////////////////////////////////
// CItemSpecialTask
//////////////////////////////////////////////////////////////////////////
void CItemSpecialTask::GetItemBinary2( PBItemObject*& pGene ) 
{
	if( mCooldown != 0 )
		pGene->set_cooldown( mCooldown );
	return; 
}

CItemObject* CItemSpecialTask::BinaryClone2( PBItemObject*& pGene )
{
	CItemSpecialTask* tpItem = (CItemSpecialTask*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_SPECIAL_TASK );
	LK_ASSERT( tpItem != NULL, return NULL );

	tpItem->mCooldown = pGene->cooldown();
	return tpItem;
}


CItemObject* CItemSpecialTask::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject ( OBJTYPE_ITEM_SPECIAL_TASK );
}



//////////////////////////////////////////////////////////////////////////
// CItemSkill
//////////////////////////////////////////////////////////////////////////



CItemObject* CItemSkill::BinaryClone2( PBItemObject*& pGene )
{
	CItemSkill* tpItemSkill = (CItemSkill*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_SKILL );

	tpItemSkill->mCooldown = pGene->cooldown();
	return tpItemSkill;
}


CItemObject* CItemSkill::Creator(unsigned int vItemID, unsigned int vNumber)
{
	return (CItemObject*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_SKILL );
}


//////////////////////////////////////////////////////////////////////////
// CItemRune
//////////////////////////////////////////////////////////////////////////


CItemObject* CItemRune::BinaryClone2( PBItemObject*& pGene )
{
	CItemRune* tpItemRune = (CItemRune*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_RUNE );
	LK_ASSERT( tpItemRune != NULL, return NULL );
	return tpItemRune;
}


CItemObject* CItemRune::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemRune* tpItemRune = (CItemRune*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_RUNE );
	LK_ASSERT( tpItemRune != NULL, return NULL );
	return tpItemRune;
}


//////////////////////////////////////////////////////////////////////////
// CItemJewel
//////////////////////////////////////////////////////////////////////////


CItemObject* CItemJewel::BinaryClone2( PBItemObject*& pGene )
{
	CItemJewel* tpItemJewel = (CItemJewel*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_JEWEL );
	LK_ASSERT( tpItemJewel != NULL, return NULL );
	return tpItemJewel;
}


CItemObject* CItemJewel::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemJewel* tpItemJewel = (CItemJewel*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_JEWEL );
	LK_ASSERT( tpItemJewel != NULL, return NULL );
	return tpItemJewel;
}

//////////////////////////////////////////////////////////////////////////
// CItemYuanBug
//////////////////////////////////////////////////////////////////////////


CItemObject* CItemYuanBug::BinaryClone2( PBItemObject*& pGene )
{
	CItemYuanBug* tpItemYuanBug = (CItemYuanBug*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_YUANBUG );
	LK_ASSERT( tpItemYuanBug != NULL, return NULL );
	tpItemYuanBug->mCurrent = pGene->itemcontent().current();
	return tpItemYuanBug;
}


CItemObject* CItemYuanBug::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemYuanBug* tpItemYuanBug = (CItemYuanBug*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_YUANBUG );
	LK_ASSERT( tpItemYuanBug != NULL, return NULL );
	return tpItemYuanBug;
}

//////////////////////////////////////////////////////////////////////////
// CItemYuanEgg
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemYuanEgg::BinaryClone2( PBItemObject*& pGene )
{
	CItemYuanEgg* tpItemYuanEgg = (CItemYuanEgg*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_YUANEGG );
	LK_ASSERT( tpItemYuanEgg != NULL, return NULL );
	return tpItemYuanEgg;
}

CItemObject* CItemYuanEgg::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemYuanEgg* tpItemYuanEgg = (CItemYuanEgg*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_YUANEGG );
	LK_ASSERT( tpItemYuanEgg != NULL, return NULL );
	return tpItemYuanEgg;
}

//////////////////////////////////////////////////////////////////////////
// CItemScroll
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemScroll::BinaryClone2( PBItemObject*& pGene )
{
	CItemScroll* tpItemScroll = (CItemScroll*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_SCROLL );
	LK_ASSERT( tpItemScroll != NULL, return NULL );
	tpItemScroll->mActivated = pGene->cooldown();
	return tpItemScroll;
}

CItemObject* CItemScroll::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemScroll* tpItemScroll = (CItemScroll*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_SCROLL );
	LK_ASSERT( tpItemScroll != NULL, return NULL );
	return tpItemScroll;
}

//////////////////////////////////////////////////////////////////////////
// CItemCard
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemCard::BinaryClone2( PBItemObject*& pGene )
{
	CItemCard* tpItemCard = (CItemCard*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_CARD );
	LK_ASSERT( tpItemCard != NULL, return NULL );
	return tpItemCard;
}

CItemObject* CItemCard::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemCard* tpItemCard = (CItemCard*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_CARD );
	LK_ASSERT( tpItemCard != NULL, return NULL );
	return tpItemCard;
}


//////////////////////////////////////////////////////////////////////////
// CItemCall
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemCall::BinaryClone2( PBItemObject*& pGene )
{
	CItemCall* tpItem = (CItemCall*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_CALL );
	LK_ASSERT( tpItem != NULL, return NULL );
	return tpItem;
}

CItemObject* CItemCall::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemCall* tpItem = (CItemCall*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_CALL );
	LK_ASSERT( tpItem != NULL, return NULL );
	return tpItem;
}


//////////////////////////////////////////////////////////////////////////
// CItemSkillBook
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemSkillBook::BinaryClone2( PBItemObject*& pGene )
{
	CItemSkillBook* tpItem = (CItemSkillBook*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_SKILLBOOK );
	LK_ASSERT( tpItem != NULL, return NULL );
	return tpItem;
}

CItemObject* CItemSkillBook::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemSkillBook* tpItem = (CItemSkillBook*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_SKILLBOOK );
	LK_ASSERT( tpItem != NULL, return NULL );
	return tpItem;
}


//////////////////////////////////////////////////////////////////////////
// CItemReturnPaper
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemReturnPaper::BinaryClone2( PBItemObject*& pGene )
{
	CItemReturnPaper* tpItemPaper = (CItemReturnPaper*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_RETURNPAPER );
	LK_ASSERT( tpItemPaper != NULL, return NULL );
	return tpItemPaper;
}

CItemObject* CItemReturnPaper::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemReturnPaper* tpItemPaper = (CItemReturnPaper*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_RETURNPAPER );
	LK_ASSERT( tpItemPaper != NULL, return NULL );
	return tpItemPaper;
}

//////////////////////////////////////////////////////////////////////////
// CItemTeleSymbol
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemTeleSymbol::BinaryClone2( PBItemObject*& pGene )
{
	CItemTeleSymbol* tpSymbol = (CItemTeleSymbol*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_TELESYMBOL );
	LK_ASSERT( tpSymbol != NULL, return NULL );
	return tpSymbol;
}

CItemObject* CItemTeleSymbol::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemTeleSymbol* tpSymbol = (CItemTeleSymbol*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_TELESYMBOL );
	LK_ASSERT( tpSymbol != NULL, return NULL );
	return tpSymbol;
}


//////////////////////////////////////////////////////////////////////////
// CItemStove
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemStove::BinaryClone2( PBItemObject*& pGene )
{
	CItemStove* tpStove = (CItemStove*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEMSTOVE );
	LK_ASSERT( tpStove != NULL, return NULL );
	tpStove->mCoolDown = pGene->cooldown();
	return tpStove;
}

CItemObject* CItemStove::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemStove* tpStove = (CItemStove*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEMSTOVE );
	LK_ASSERT( tpStove != NULL, return NULL );
	return tpStove;
}

void CItemStove::GetItemBinary2(PBItemObject *&pGene)
{
	if( mCoolDown != 0 )
		pGene->set_cooldown( mCoolDown );
	return ;
}

//////////////////////////////////////////////////////////////////////////
// CTplMineMap
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemMineMap::BinaryClone2( PBItemObject*& pGene )
{
	CItemMineMap* tpMineMap = (CItemMineMap*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_MINEMAP );
	LK_ASSERT( tpMineMap != NULL, return NULL );

	// 普通物品只有一个传送点
	if( pGene->mutable_itemcontent()->telepos_size() == 1 )
	{
		const PBTelePostion& rTelePos = pGene->mutable_itemcontent()->telepos(0);
		tpMineMap->mLineID = rTelePos.line();
		tpMineMap->mMapID = rTelePos.map();
		tpMineMap->mPosX = rTelePos.posx();
		tpMineMap->mPosY = rTelePos.posy();
	}

	return tpMineMap;
}

CItemObject* CItemMineMap::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemMineMap* tpMineMap = (CItemMineMap*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_MINEMAP );
	LK_ASSERT( tpMineMap != NULL, return NULL );
	return tpMineMap;
}

void CItemMineMap::GetItemBinary2( PBItemObject*& pGene )
{
	// 普通物品只有一个传送点
	PBTelePostion* pTelePos = pGene->mutable_itemcontent()->add_telepos();
	LK_ASSERT( pTelePos != NULL, return );
	pTelePos->set_line( mLineID );
	pTelePos->set_map( mMapID );
	pTelePos->set_posx( mPosX );
	pTelePos->set_posy( mPosY );
}

CItemObject* CItemToken::BinaryClone2( PBItemObject*& pGene )
{
	CItemToken* tpToken = (CItemToken*) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ITEM_TOKEN );
	LK_ASSERT( tpToken != NULL, return NULL );

	// 普通物品只有一个传送点
	if( pGene->mutable_itemcontent()->telepos_size() == 1 )
	{
		const PBTelePostion& rTelePos = pGene->mutable_itemcontent()->telepos(0);
		tpToken->mLineID = rTelePos.line();
		tpToken->mMapID = rTelePos.map();
		tpToken->mPosX = rTelePos.posx();
		tpToken->mPosY = rTelePos.posy();
	}

	return tpToken;
}

void CItemToken::GetItemBinary2( PBItemObject*& pGene )
{
	// 普通物品只有一个传送点
	PBTelePostion* pTelePos = pGene->mutable_itemcontent()->add_telepos();
	LK_ASSERT( pTelePos != NULL, return );
	pTelePos->set_line( mLineID );
	pTelePos->set_map( mMapID );
	pTelePos->set_posx( mPosX );
	pTelePos->set_posy( mPosY );
}

CItemObject* CItemToken::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemToken* tpToken = (CItemToken*)CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_TOKEN );
	LK_ASSERT( tpToken != NULL, return NULL );
	return tpToken;
}

CItemObject* CItemRemove::BinaryClone2( PBItemObject*& pGene )
{
	CItemRemove* tpRemove = (CItemRemove*) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ITEM_REMOVE );
	LK_ASSERT( tpRemove != NULL, return NULL );
	return tpRemove;
}

CItemObject* CItemRemove::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemRemove* tpRemove = (CItemRemove*)CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_REMOVE );
	LK_ASSERT( tpRemove != NULL, return NULL );
	return tpRemove;
}

CItemObject* CItemChange::BinaryClone2( PBItemObject*& pGene )
{
	CItemChange* tpChange = (CItemChange*) CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_ITEM_CHANGE );
	LK_ASSERT( tpChange != NULL, return NULL );
	return tpChange;
}

CItemObject* CItemChange::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemChange* tpChange = (CItemChange*)CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_CHANGE );
	LK_ASSERT( tpChange != NULL, return NULL );
	return tpChange;
}

// 红包
CItemObject* CItemRedPaper::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemRedPaper *tpRedPaper = ( CItemRedPaper * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_REDPAPER );
	LK_ASSERT( tpRedPaper != NULL, return NULL )
		return tpRedPaper;
}

CItemObject* CItemRedPaper::BinaryClone2( PBItemObject*& pGene )
{
	CItemRedPaper *tpRedPaper = ( CItemRedPaper * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_REDPAPER );	
	return tpRedPaper;
}


// 附魔石
CItemObject* CItemMagicStone::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemMagicStone *tpMagicStone = ( CItemMagicStone * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_MAGICSTONE );	
	return tpMagicStone;
}

CItemObject* CItemMagicStone::BinaryClone2( PBItemObject*& pGene )
{ 
	return ( CItemObject * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_MAGICSTONE );
}

//////////////////////////////////////////////////////////////////////////
// CItemLifeSkillScroll
//////////////////////////////////////////////////////////////////////////
CItemObject* CItemLifeSkillScroll::Creator( unsigned int vItemID, unsigned int vNumber )
{
	return ( CItemObject * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_LIFESKILLSCROLL ); 
}	

CItemObject* CItemLifeSkillScroll::BinaryClone2( PBItemObject*& pGene )
{
	return ( CItemObject * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_LIFESKILLSCROLL ); 
} 

//////////////////////////////////////////////////////////////////////////
// CItemGiftBag
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemGiftBag::BinaryClone2( PBItemObject*& pGene )
{
	CItemGiftBag* tpItem = (CItemGiftBag*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_GIFTBAG );
	LK_ASSERT( tpItem != NULL, return NULL );
	return tpItem;
}

CItemObject* CItemGiftBag::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemGiftBag* tpItem = (CItemGiftBag*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_GIFTBAG );
	LK_ASSERT( tpItem != NULL, return NULL );
	return tpItem;
}

//////////////////////////////////////////////////////////////////////////
// CItemVip
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemVip::BinaryClone2( PBItemObject*& pGene )
{
	CItemVip* tpItem = (CItemVip*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_VIP );
	LK_ASSERT( tpItem != NULL, return NULL );
	return tpItem;
}

CItemObject* CItemVip::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemVip* tpItem = (CItemVip*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_VIP );
	LK_ASSERT( tpItem != NULL, return NULL );
	return tpItem;
}

//////////////////////////////////////////////////////////////////////////
// CItemClean
//////////////////////////////////////////////////////////////////////////

CItemObject* CItemClean::BinaryClone2( PBItemObject*& pGene )
{
	CItemClean* tpItem = (CItemClean*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_CLEAN );
	LK_ASSERT( tpItem != NULL, return NULL );
	return tpItem;
}

CItemObject* CItemClean::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemClean* tpItem = (CItemClean*) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_ITEM_CLEAN );
	LK_ASSERT( tpItem != NULL, return NULL );
	return tpItem;
}

//////////////////////////////////////////////////////////////////////////
//	CItemLeagueBag
//////////////////////////////////////////////////////////////////////////
CItemObject *CItemLeagueBag::BinaryClone2( PBItemObject*& pGene )
{
	CItemLeagueBag *tpItem = ( CItemLeagueBag * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_LEAGUEBAG );
	LK_ASSERT( tpItem != NULL, return NULL )
		return tpItem;
}
CItemObject *CItemLeagueBag::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemLeagueBag *tpItem = ( CItemLeagueBag * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_LEAGUEBAG );
	LK_ASSERT( tpItem != NULL, return NULL )
		return tpItem;
}

//////////////////////////////////////////////////////////////////////////
//	CItemInvitation
//////////////////////////////////////////////////////////////////////////
CItemObject* CItemInvitation::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemInvitation *tpItem = ( CItemInvitation * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_INVITATION );
	LK_ASSERT( tpItem != NULL, return NULL )
		return tpItem;
}

void CItemInvitation::GetItemBinary2( PBItemObject*& pGene )
{	
	pGene->set_groomname( mGroomName );
	pGene->set_bridename( mBrideName );
	pGene->set_weddingtime( mWeddingTime );
	pGene->set_weddingtimelength( mWeddingLength );
}	

CItemObject* CItemInvitation::BinaryClone2( PBItemObject*& pGene )
{
	CItemInvitation *tpItem = ( CItemInvitation * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_INVITATION );
	LK_ASSERT( tpItem != NULL, return NULL )
		tpItem->SetBrideName( pGene->mutable_bridename()->c_str() );
	tpItem->SetGroomName( pGene->mutable_groomname()->c_str() );
	tpItem->SetWeddingTime( pGene->weddingtime() );
	tpItem->SetWeddingTimeLength( pGene->weddingtimelength() ); 
	return tpItem;
}

//////////////////////////////////////////////////////////////////////////
//	CItemInvitation		系统请帖	 
//////////////////////////////////////////////////////////////////////////
CItemObject* CItemSysInvitation::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemSysInvitation *tpItem = ( CItemSysInvitation * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_SYSINVITATION );
	LK_ASSERT( tpItem != NULL, return NULL )
		return tpItem;
}

CItemObject* CItemSysInvitation::BinaryClone2( PBItemObject*& pGene )
{
	CItemSysInvitation *tpItem = ( CItemSysInvitation * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_SYSINVITATION );
	LK_ASSERT( tpItem != NULL, return NULL )
		return tpItem;
}


// ********************************************************************** //
// CItemBlackPills
// ********************************************************************** //
CItemObject* CItemBlackPills::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemBlackPills *tpItem = ( CItemBlackPills * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_BLACKPILLS );
	LK_ASSERT( tpItem != NULL, return NULL )
		return tpItem;
}
void CItemBlackPills::GetItemBinary2( PBItemObject*& pGene )
{
	pGene->mutable_itemcontent()->set_leftofflinetime( mLeftTime ) ;
}
CItemObject* CItemBlackPills::BinaryClone2( PBItemObject*& pGene )
{
	CItemBlackPills *tpItem = ( CItemBlackPills * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_BLACKPILLS );
	LK_ASSERT( tpItem != NULL, return NULL )
		tpItem->SetLeftTime( pGene->mutable_itemcontent()->leftofflinetime() );
	return tpItem;
}

// 修炼经验丹
CItemObject* CItemExpContainer::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemExpContainer *tpItem = ( CItemExpContainer * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_EXPCONTAINER );
	return tpItem;
}

void CItemExpContainer::GetItemBinary2( PBItemObject*& pGene )
{
	pGene->mutable_itemcontent()->set_exp( mExp );
}

CItemObject* CItemExpContainer::BinaryClone2( PBItemObject*& pGene )
{
	CItemExpContainer *tpExpContainer = ( CItemExpContainer * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_EXPCONTAINER );
	LK_ASSERT( tpExpContainer != NULL, return NULL )
	tpExpContainer->SetExp( pGene->mutable_itemcontent()->exp() );
	return tpExpContainer;
}

// 经验丹
CItemObject* CItemExpPills::Creator( unsigned int vItemID, unsigned int vNumber )
{
	CItemExpPills *tpExpPills = ( CItemExpPills  * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_EXPPILLS );	
	return tpExpPills;
}

void CItemExpPills::GetItemBinary2( PBItemObject*& pGene )
{
	
}

CItemObject* CItemExpPills::BinaryClone2( PBItemObject*& pGene )
{
	CItemExpPills *tpExpPills = ( CItemExpPills  * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ITEM_EXPPILLS );	
	return tpExpPills;
}	

// ********************************************************************** //
// CItemRunTime
// ********************************************************************** //
std::vector< CItemRunTime >* CItemRunTime::spItemTypes;

void CItemRunTime::Initialize( )
{
	spItemTypes = new std::vector< CItemRunTime >;
	CItemRunTime::RegisterClone( CTplItem::ITEM_BOOK,			&CItemBook::BinaryClone2,				&CItemBook::Creator	);
	CItemRunTime::RegisterClone( CTplItem::ITEM_EQUIPUPG,		&CItemEquipUpg::BinaryClone2,		&CItemEquipUpg::Creator	);
	CItemRunTime::RegisterClone( CTplItem::ITEM_EXP,				&CItemExp::BinaryClone2,				&CItemExp::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_PHYSIC,				&CItemPhysic::BinaryClone2,				&CItemPhysic::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_RELIFE,				&CItemRelive::BinaryClone2,				&CItemRelive::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_RETURN,				&CItemReturn::BinaryClone2,				&CItemReturn::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_SKILL,				&CItemSkill::BinaryClone2,				&CItemSkill::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_TELEPORT,		&CItemTeleport::BinaryClone2,		&CItemTeleport::Creator	);
	CItemRunTime::RegisterClone( CTplItem::ITEM_WEAPON,		 	&CItemEquipment::BinaryClone2, 	 	&CItemEquipment::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_ARMOR,		 	&CItemEquipment::BinaryClone2, 	 	&CItemEquipment::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_FASHION,		&CItemEquipment::BinaryClone2, 		&CItemEquipment::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_TASK,				&CItemTask::BinaryClone2,			&CItemTask::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_PRODUCE,			&CItemStuff::BinaryClone2,				&CItemStuff::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_UPGLUCK,			&CItemNote::BinaryClone2,				&CItemNote::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_UPGLEAST,			&CItemNote::BinaryClone2,				&CItemNote::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_RUNE,				&CItemRune::BinaryClone2,				&CItemRune::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_JEWEL,				&CItemJewel::BinaryClone2,				&CItemJewel::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_YUANBUG,			&CItemYuanBug::BinaryClone2,		&CItemYuanBug::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_YUANEGG,			&CItemYuanEgg::BinaryClone2,			&CItemYuanEgg::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_SHOUT,	   	&CItemShout::BinaryClone2,				&CItemShout::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_AUTOSHOUT,  	&CItemAutoShout::BinaryClone2,		&CItemAutoShout::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_SPECIAL_TASK,	   	&CItemSpecialTask::BinaryClone2,				&CItemSpecialTask::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_HORSE,	   		&CItemHorse::BinaryClone2,			&CItemHorse::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_BINDSTONE,		&CItemBindStone::BinaryClone2,		&CItemBindStone::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_SCROLL,			&CItemScroll::BinaryClone2,		&CItemScroll::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_CARD,			&CItemCard::BinaryClone2,		&CItemCard::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_RETURNPAPER,	&CItemReturnPaper::BinaryClone2, &CItemReturnPaper::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_TELESYMBOL,		&CItemTeleSymbol::BinaryClone2, &CItemTeleSymbol::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_STOVE,			&CItemStove::BinaryClone2, &CItemStove::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_QUESTIONPAPER,	&CItemQuestPaper::BinaryClone2, &CItemQuestPaper::Creator );	
	CItemRunTime::RegisterClone( CTplItem::ITEM_MINEMAP,		&CItemMineMap::BinaryClone2, &CItemMineMap::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_TOKEN,			&CItemToken::BinaryClone2, &CItemToken::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_REMOVE,			&CItemRemove::BinaryClone2, &CItemRemove::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_CHANGE,			&CItemChange::BinaryClone2, &CItemChange::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_KITBAG,			&CItemKitBag::BinaryClone2, &CItemKitBag::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_CALL,			&CItemCall::BinaryClone2,		&CItemCall::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_GIFTBAG,		&CItemGiftBag::BinaryClone2,		&CItemGiftBag::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_MAGICSTONE,		&CItemMagicStone::BinaryClone2,		&CItemMagicStone::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_LIFESKILLSCROLL,&CItemLifeSkillScroll::BinaryClone2,&CItemLifeSkillScroll::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_MAGICWEAPON,	&CItemMagicWeapon::BinaryClone2,		&CItemMagicWeapon::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_FUNCITEM,		&CItemFuncItem::BinaryClone2,		&CItemFuncItem::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_SKILLBOOK,		&CItemSkillBook::BinaryClone2,		&CItemSkillBook::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_REDPAPER,		&CItemRedPaper::BinaryClone2,		&CItemRedPaper::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_VIP,			&CItemVip::BinaryClone2,			&CItemVip::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_CLEAN,			&CItemClean::BinaryClone2,			&CItemClean::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_LEAGUEBAG,		&CItemLeagueBag::BinaryClone2,		&CItemLeagueBag::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_INVITATION,		&CItemInvitation::BinaryClone2,		&CItemInvitation::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_SYSINVITATION,	&CItemSysInvitation::BinaryClone2,	&CItemSysInvitation::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_REFRESH,		&CItemBook::BinaryClone2,			&CItemBook::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_DIRECT,		&CItemBook::BinaryClone2,			&CItemBook::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_GETSTAR,		&CItemBook::BinaryClone2,			&CItemBook::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_SUMMON,		&CItemBook::BinaryClone2,			&CItemBook::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_BLACKPILLS,	&CItemBlackPills::BinaryClone2,			&CItemBlackPills::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_EXPCONTAINER, &CItemExpContainer::BinaryClone2,   &CItemExpContainer::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_EXPPILLS, &CItemExpPills::BinaryClone2,			  &CItemExpPills::Creator );
	CItemRunTime::RegisterClone( CTplItem::ITEM_SPIRIT_CASTING,		&CItemBook::BinaryClone2,			&CItemBook::Creator );
}

CItemObject* CItemRunTime::CloneFromBinary2( PBItemObject*& pGene )
{
	unsigned int tItemID = pGene->itemid();
	unsigned int tNumber = pGene->number();
	int          tBindStatus = pGene->bindstatus();
	int			 tUsedTimes  = pGene->usedtimes();
	uint64_t 	 ullPurchaseID = pGene->purchaseid();
	uint64_t 	 ullObjGUID = pGene->objguid();	

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tItemID );
	if ( tpItem == NULL )
	{
		LOG_ERROR( "default", "Item %d not exist", tItemID );
		return NULL;
	}

	for ( size_t j = 0; j < CItemRunTime::spItemTypes->size( ); j ++ )
	{
		if ( (*CItemRunTime::spItemTypes)[ j ].mItemType == tpItem->mItemType )
		{
			CItemObject* tpItemObject = (*CItemRunTime::spItemTypes)[ j ].mpItemBinaryClone2( pGene );
			if( tpItemObject == NULL )
			{
				LOG_ERROR( "pro", "ItemClone function %d not registered", j );	
				return NULL;
			}
			tpItemObject->SetItemID( tItemID );
			if ( tNumber > tpItem->mPileLimit )
				tNumber = tpItem->mPileLimit;
			tpItemObject->SetItemNum( tNumber );
			tpItemObject->SetBindStatus( tBindStatus ); 
			tpItemObject->SetUsedTimes( tUsedTimes );
			tpItemObject->SetPurchaseID( ullPurchaseID );  						
			tpItemObject->SetGUID( ullObjGUID );
			tpItemObject->SetInvalidTime( pGene->invalidtime() );
			return tpItemObject;
		}
	}

	return NULL;
}

CItemObject* CItemRunTime::Clone( unsigned int vItemID, unsigned int vNumber )
{
	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( vItemID );
	if ( tpItem == NULL )
		return NULL;

	for ( size_t j = 0; j < CItemRunTime::spItemTypes->size( ); j ++ )
	{
		if ((*CItemRunTime::spItemTypes) [ j ].mItemType == tpItem->mItemType )
		{
			CItemObject* tpItemObject = (*CItemRunTime::spItemTypes) [ j ].mpItemCreator( vItemID, vNumber );
			if ( tpItemObject == NULL )
				return NULL;

			tpItemObject->SetItemID( vItemID );
			tpItemObject->SetItemNum( vNumber );
			tpItemObject->SetBindStatus( 0 );
			tpItemObject->SetUsedTimes( 0 );

			tpItemObject->SetPurchaseID( 0 );
			tpItemObject->SetGUID( 0 );

			tpItemObject->SetValidTimerID( INVALID_OBJ_ID );
			tpItemObject->SetInvalidTime( 0 );					
			return tpItemObject;
		}
	}

	return NULL;
}



unsigned short CItemEquipment::GetIncSkillLevel( unsigned short vSkillID )
{
	CTemplateEquipment* tpTemplate = (CTemplateEquipment*) CDataStatic::SearchTpl( mItemID );				
	if ( tpTemplate == NULL )																		
		return 0;																						

	unsigned short tValue = 0;
	//for ( int i = 0; i < PROPERTYNUM; i ++ )														
	//{																								
	//	CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( mSlotProperty[ i ] );	
	//	if ( tpProperty == NULL )																	
	//		break;																					
	//								
	//	// 增加某个技能等级
	//	if ( tpProperty->mPropertyFunc == CTemplateProperty::PROPERTYINC_SKILLLEVEL )
	//	{
	//		if ( vSkillID == tpProperty->mSkillID )
	//			tValue += mSlotValue[ i ];
	//	}
	//	// 增加某系技能等级
	//	if ( tpProperty->mPropertyFunc == CTemplateProperty::PROPERTYINC_SKILLGENRELEVEL )
	//	{
	//		// 通过vSkillID 获得技能模版然后得到该技能的系别ID
	//		// 比较该系别ID 和 属性里的SkillID( 此时代表系别ID ) 是否相等
	//		CTemplateSkill* tpSkillTpl = (CTemplateSkill*) CDataStatic::SearchSkill( vSkillID, 1 );
	//		if ( tpSkillTpl != NULL && tpSkillTpl->mGenerID == tpProperty->mSkillID )
	//			tValue += mSlotValue[ i ];
	//	}
	//}																								

	for ( int i = 0; i < PROPERTYNUM; i ++ )													
	{																								
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( mProperty[ i ] );		
		if ( tpProperty == NULL )																	
			break;																					

		// 增加某个技能等级
		if ( tpProperty->mPropertyFunc == CTemplateProperty::PROPERTYINC_SKILLLEVEL )
		{
			if ( vSkillID == tpProperty->mSkillID )
				tValue += tpProperty->mIntMinValue;
		}
		// 增加某系技能等级
		else if ( tpProperty->mPropertyFunc == CTemplateProperty::PROPERTYINC_SKILLGENRELEVEL )
		{
			// 通过vSkillID 获得技能模版然后得到该技能的系别ID
			// 比较该系别ID 和 属性里的SkillID( 此时代表系别ID ) 是否相等
			CTemplateSkill* tpSkillTpl = (CTemplateSkill*) CDataStatic::SearchSkill( vSkillID, 1 );
			if ( tpSkillTpl != NULL && tpSkillTpl->mGenerID == tpProperty->mSkillID )
				tValue += tpProperty->mIntMinValue;
		}
	}
	for ( int i = 0; i < JEWELNUM; i ++ )													
	{																				
		CTplItemJewel *tpJewel = (CTplItemJewel*) CDataStatic::SearchTpl( mSlotValue[ i ] );
		if ( tpJewel == NULL )
			continue;
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tpJewel->mProperty );		
		if ( tpProperty == NULL )																	
			continue;																					

		// 增加某个技能等级
		if ( tpProperty->mPropertyFunc == CTemplateProperty::PROPERTYINC_SKILLLEVEL )
		{
			if ( vSkillID == tpProperty->mSkillID )
				tValue += tpProperty->mIntMinValue;
		}
		// 增加某系技能等级
		else if ( tpProperty->mPropertyFunc == CTemplateProperty::PROPERTYINC_SKILLGENRELEVEL )
		{
			// 通过vSkillID 获得技能模版然后得到该技能的系别ID
			// 比较该系别ID 和 属性里的SkillID( 此时代表系别ID ) 是否相等
			CTemplateSkill* tpSkillTpl = (CTemplateSkill*) CDataStatic::SearchSkill( vSkillID, 1 );
			if ( tpSkillTpl != NULL && tpSkillTpl->mGenerID == tpProperty->mSkillID )
				tValue += tpProperty->mIntMinValue;
		}
	}

	return tValue;
}

unsigned short CItemEquipment::GetValueByID( unsigned short vSkillID, CTemplateProperty::PROPERTY_FUNC vType )
{
	CTemplateEquipment* tpTemplate = (CTemplateEquipment*) CDataStatic::SearchTpl( mItemID );				
	if ( tpTemplate == NULL )																		
		return 0;																						

	unsigned short tValue = 0;																				

	for ( int i = 0; i < PROPERTYNUM; i ++ )													
	{																								
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( mProperty[ i ] );		
		if ( tpProperty == NULL )																	
			break;																					

		// 增加某个技能等级
		if ( tpProperty->mPropertyFunc == vType )
		{
			if ( vSkillID == tpProperty->mSkillID )
				tValue += tpProperty->mIntMinValue;
		}
	}
	for ( int i = 0; i < JEWELNUM; i ++ )													
	{																				
		CTplItemJewel *tpJewel = (CTplItemJewel*) CDataStatic::SearchTpl( mSlotValue[ i ] );
		if ( tpJewel == NULL )
			continue;
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tpJewel->mProperty );		
		if ( tpProperty == NULL )																	
			continue;																					

		// 增加某个技能等级
		if ( tpProperty->mPropertyFunc == vType )
		{
			if ( vSkillID == tpProperty->mSkillID )
				tValue += tpProperty->mIntMinValue;
		}
	}

	return tValue;
}

int	CItemEquipment::GetStateID( StateList& vBuffList,CTemplateProperty::PROPERTY_FUNC vType )
{
	CTemplateEquipment* tpTemplate = (CTemplateEquipment*) CDataStatic::SearchTpl( mItemID );				
	if ( tpTemplate == NULL )																		
		return 0;																				

	for ( int i = 0; i < PROPERTYNUM; i ++ )													
	{																								
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( mProperty[ i ] );		
		if ( tpProperty == NULL )																	
			break;																					

		if ( tpProperty->mPropertyFunc == vType )
		{
			BuffState tBuff;
			tBuff.mBuffID = tpProperty->mSkillID;
			tBuff.mRand = tpProperty->mIntMinValue;
			tBuff.mTime = tpProperty->mIntMaxValue;
			tBuff.mTarget = tpProperty->mCanOut;
			tBuff.mHP = tpProperty->mFloatMinValue;
			vBuffList.push_back( tBuff );
		}
	}
	for ( int i = 0; i < JEWELNUM; i ++ )													
	{																				
		CTplItemJewel *tpJewel = (CTplItemJewel*) CDataStatic::SearchTpl( mSlotValue[ i ] );
		if ( tpJewel == NULL )
			continue;
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tpJewel->mProperty );		
		if ( tpProperty == NULL )																	
			continue;																					

		if ( tpProperty->mPropertyFunc == vType )
		{
			BuffState tBuff;
			tBuff.mBuffID = tpProperty->mSkillID;
			tBuff.mRand = tpProperty->mIntMinValue;
			tBuff.mTime = tpProperty->mIntMaxValue;
			tBuff.mTarget = tpProperty->mCanOut;
			tBuff.mHP = tpProperty->mFloatMinValue;
			vBuffList.push_back( tBuff );
		}
	}

	return 0;
}


int CItemEquipment::GetPropertyFromTpl( int vItemID, bool vInit )
{
	// 如果道具模版ID等于0, 说明该道具不需要生成各种属性
	if ( vItemID == 0 )
		return 1;

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( vItemID );
	if ( tpItem == NULL )
		return 1;

	unsigned short	tPropertyNumber1 = 0;			// 装备一区属性个数
	unsigned short	tPropertyNumber2 = 0;			// 装备二区属性个数	

	int* tpPropertyPercent = NULL;	// 得到几个附加属性的概率
	unsigned int unPropertyPercentNum = 0;
	CTemplatePropertyTable::CPropertyPercent* tpPropertyPro = NULL;
	unsigned int unPropertyProNum = 0;

	// 记录已刷新的属性
	unsigned int unProperty[ PROPERTYNUM ] = { 0 };
	memcpy( unProperty, mProperty, sizeof(unProperty) );
	memset( mProperty, 0, sizeof(mProperty) );

	CTemplateEquipment *tpEquip = (CTemplateEquipment*)tpItem;
	int tRand		= 0;
	int tLowRan		= 0;
	int tHighRan	= 0;

	if ( vInit == true )
	{
		mMaxSlotNumber = CPropertyValue( );
		tRand = RAND(10000);
		for ( unsigned int t = 0; t < ARRAY_CNT(tpEquip->mSlotNumber1); t ++ )
		{
			tLowRan		+= ( t == 0 ) ? 0 : tpEquip->mSlotNumber1[ t - 1 ];
			tHighRan	+= tpEquip->mSlotNumber1[ t ];
			if ( tRand >= tLowRan && tRand < tHighRan )
			{
				mMaxSlotNumber.mInitValue = t;
				break;
			}
		}

		tRand = RAND(10000);
		tLowRan = 0;
		tHighRan = 0;
		for ( unsigned int t = 0; t < ARRAY_CNT( tpEquip->mResDefPercent ); t ++ )
		{
			tLowRan		+= ( t == 0 ) ? 0 : tpEquip->mResDefPercent[ t - 1 ];
			tHighRan	+= tpEquip->mResDefPercent[ t ];
			if ( tRand >= tLowRan && tRand < tHighRan )
			{
				mResDef = t;
				break;
			}
		}
		tRand = RAND(10000);
		tLowRan = 0;
		tHighRan = 0;
		for ( unsigned int t = 0; t < ARRAY_CNT( tpEquip->mResAttPercent ); t ++ )
		{
			tLowRan		+= ( t == 0 ) ? 0 : tpEquip->mResAttPercent[ t - 1 ];
			tHighRan	+= tpEquip->mResAttPercent[ t ];
			if ( tRand >= tLowRan && tRand < tHighRan )
			{
				mResAtt = t;
				break;
			}
		}

		mAbrasionCount = 0;
		mBindFreeTime  = 0;
		//mBindLeftTime  = 0;
		mCurDurability = tpEquip->mEquipmentDurability;
	}

	tpPropertyPercent	= tpEquip->mPropertyPercent; 
	unPropertyPercentNum = ARRAY_CNT( tpEquip->mPropertyPercent );

	tRand = RAND(10000);
	tLowRan = 0;
	tHighRan = 0;
	for ( unsigned int t = 0; t < unPropertyPercentNum; t ++ )
	{
		tLowRan		+= ( t == 0 ) ? 0 : tpPropertyPercent[ t - 1 ];
		tHighRan	+= tpPropertyPercent[ t ];
		if ( tRand >= tLowRan && tRand < tHighRan )
		{
			tPropertyNumber1 = t;
			break;
		}
	}

	// 计算第一区每一个附加属性
	unsigned int j = 0;
	// 失败次数过多返回
	int	nFailCount = 0;
	int nSameCount = 0;
	for ( ; j < tPropertyNumber1 && j < ARRAY_CNT(mProperty); j ++ )
	{
		int tRand = RAND(SERVER_PERCENT_INT);
		int tLowRan		= 0;
		int tHighRan	= 0;
		CTemplatePropertyTable* tpPropertyTable = (CTemplatePropertyTable*) CDataStatic::SearchTpl( tpEquip->mPropertyTableID[ j ] );
		if ( tpPropertyTable == NULL )
			continue;
		tpPropertyPro = tpPropertyTable->mProperty; 
		unPropertyProNum = ARRAY_CNT( tpPropertyTable->mProperty );
		for ( unsigned int t = 0; t < unPropertyProNum; t ++ )
		{
			tLowRan		+= ( t == 0 ) ? 0 : tpPropertyPro[ t - 1 ].mPercent;
			tHighRan	+= tpPropertyPro[ t ].mPercent;
			if ( tRand >= tLowRan && tRand < tHighRan )
			{
				mProperty[ j ] = tpPropertyPro[ t ].mTempID;
				// 必定生成的属性就不算了
				if ( mProperty[ j ] == unProperty[ j ] && unProperty[ j ] != 0 
					&& tLowRan + tHighRan != SERVER_PERCENT_INT )
				{
					nSameCount++;
				}
				CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tpPropertyPro[ t ].mTempID );
				if ( tpProperty == NULL )
				{
					LOG_ERROR( "default", "can't find proeprty of item %d", tpPropertyPro[ t ].mTempID );
					return 1;
				}
				break;
			}
		}
		if ( nSameCount == tPropertyNumber1 && nFailCount < PROPERTYNUM * PROPERTYNUM )
		{
			for ( int n = 0; n < tPropertyNumber1; n++ )
				mProperty[ n ] = 0;
			j = 0 - 1;
			nSameCount = 0;
			nFailCount++;
			continue;
		}
	}

	tpPropertyPercent	= tpEquip->mPropertyPercentEx; 
	unPropertyPercentNum = ARRAY_CNT( tpEquip->mPropertyPercentEx );

	tRand = RAND(SERVER_PERCENT_INT);
	tLowRan		= 0;
	tHighRan	= 0;
	nFailCount  = 0;
	nSameCount	= 0;
	for ( unsigned int t = 0; t < unPropertyPercentNum; t ++ )
	{
		tLowRan		+= ( t == 0 ) ? 0 : tpPropertyPercent[ t - 1 ];
		tHighRan	+= tpPropertyPercent[ t ];
		if ( tRand >= tLowRan && tRand < tHighRan )
		{
			tPropertyNumber2 = t;
			break;
		}
	}

	// 计算第二区每一个附加属性
	for ( ; j < tPropertyNumber1+tPropertyNumber2 && j < ARRAY_CNT(mProperty); j ++ )
	{
		int tRand = RAND(10000);
		int tLowRan		= 0;
		int tHighRan	= 0;
		CTemplatePropertyTable* tpPropertyTable = (CTemplatePropertyTable*) CDataStatic::SearchTpl( tpEquip->mPropertyTableIDEx[ j - tPropertyNumber1 ] );
		if ( tpPropertyTable == NULL )
			continue;
		tpPropertyPro = tpPropertyTable->mProperty; 
		unPropertyProNum = ARRAY_CNT( tpPropertyTable->mProperty );
		for ( unsigned int t = 0; t < unPropertyProNum; t ++ )
		{
			tLowRan		+= ( t == 0 ) ? 0 : tpPropertyPro[ t - 1 ].mPercent;
			tHighRan	+= tpPropertyPro[ t ].mPercent;
			if ( tRand >= tLowRan && tRand < tHighRan )
			{
				mProperty[ j ] = tpPropertyPro[ t ].mTempID;
				// 必定生成的属性就不算了
				if ( mProperty[ j ] == unProperty[ j ] && unProperty[ j ] != 0 
					&& tLowRan + tHighRan != SERVER_PERCENT_INT )
				{
					nSameCount++;
				}
				CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tpPropertyPro[ t ].mTempID );
				if ( tpProperty == NULL )
				{
					LOG_ERROR( "default", "can't find proeprty of item %d", tpPropertyPro[ t ].mTempID );
					return 1;
				}
				break;
			}
		}
		if ( nSameCount == tPropertyNumber2 && nFailCount < PROPERTYNUM * PROPERTYNUM )
		{
			for ( int n = tPropertyNumber1; n < tPropertyNumber1 + tPropertyNumber2; n++ )
				mProperty[ n ] = 0;
			j = tPropertyNumber1 - 1;
			nSameCount = 0;
			nFailCount++;
			continue;
		}
	}
	return 0;
}
