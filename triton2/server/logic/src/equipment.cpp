
// ********************************************************************** //														
// CEquipment																														
// ********************************************************************** //														
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

int CEquipment::GetFixNumber( CTemplateProperty::PROPERTY_FUNC vFunc, int vEquipProType, CPropertyPlayer *pProperty )		
{																						
	int tValue = 0;																		
	for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_MARRY; i ++ )					
	{																					
		CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );					
		if ( tpEquip != NULL )															
			tValue += tpEquip->GetFixNumber( vFunc, vEquipProType, pProperty );												
	}																					
	return tValue;																		
}	

int CEquipment::GetPerNumber( CTemplateProperty::PROPERTY_FUNC vFunc )		
{																						
	int tValue = 0;																		
	for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_MARRY; i ++ )					
	{																					
		CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );					
		if ( tpEquip != NULL )															
			tValue += tpEquip->GetPerNumber( vFunc );												
	}																					
	return tValue;																		
}																						

void CEquipment::GetLightPoint( int &WeaponLevel, int &ChestLevel, int &WeaponJewel, int &ChestJewel )	
{																						
	WeaponLevel = 0;	
	ChestLevel = 0;
	WeaponJewel = 0;
	ChestJewel = 0;
	CTemplateLightTable *tpTable = (CTemplateLightTable*)CDataStatic::GetTemp( TEMP_LIGHT );
	if ( tpTable == NULL )
	{
		return;
	}
	for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_CLOAK; i ++ )					
	{																					
		CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );					
		if ( tpEquip != NULL )	
		{
			if ( tpEquip->GetLevel() == 0 || tpEquip->GetLevel() > (int)ARRAY_CNT(tpTable->mLevelPoint) )
				continue;
			if ( i == EQUIPMENT_WEAPON )
			{
				WeaponLevel = tpTable->mLevelPoint[ tpEquip->GetLevel() - 1 ];
			}
			for ( int j = 0; j < JEWELNUM; j ++ )																	
			{																										
				CTplItemJewel *tpJewel = (CTplItemJewel*)CDataStatic::SearchTpl( tpEquip->GetSlotValue( j ) );					
				if ( tpJewel == NULL )																				
					continue;
				if ( tpJewel->mLevel == 0 || tpJewel->mLevel > (int)ARRAY_CNT(tpTable->mJewelPoint) )
					continue;
				if ( i == EQUIPMENT_WEAPON )
				{
					WeaponJewel += tpTable->mJewelPoint[ tpJewel->mLevel - 1 ];
				}
				ChestJewel += tpTable->mJewelPoint[ tpJewel->mLevel - 1 ];
			}
			ChestLevel += tpTable->mLevelPoint[ tpEquip->GetLevel() - 1 ];
		}
	}																	
}

//***************************************//
//   CEquipment
//***************************************//

int CEquipment::GetPartType( unsigned short vPart )
{
	switch( vPart )
	{
	case EQUIPMENT_WEAPON:		return CTemplateEquipment::PART_WEAPON;		// 武器
	case EQUIPMENT_CHEST:		return CTemplateEquipment::PART_CHEST;		// 胸部护具
	case EQUIPMENT_LEG:			return CTemplateEquipment::PART_LEG;		// 腿部护具
	case EQUIPMENT_HEAD:		return CTemplateEquipment::PART_HEAD;		// 头部护具
	case EQUIPMENT_NECK:		return CTemplateEquipment::PART_NECK;		// 项链
	case EQUIPMENT_RINGLEFT:	return CTemplateEquipment::PART_RING; 		// 左戒指
	case EQUIPMENT_RINGRIGHT:	return CTemplateEquipment::PART_RING; 		// 右戒指
	case EQUIPMENT_SASH:		return CTemplateEquipment::PART_SASH;		// 腰带
	case EQUIPMENT_CUFFLEFT:	return CTemplateEquipment::PART_CUFF;		// 左护腕
	case EQUIPMENT_CUFFRIGHT:	return CTemplateEquipment::PART_CUFF;		// 右护腕
	case EQUIPMENT_CHARMLEFT:	return CTemplateEquipment::PART_CHARM;		// 左护符
	case EQUIPMENT_CHARMRIGHT:	return CTemplateEquipment::PART_CHARM;		// 右护符
	case EQUIPMENT_BALL:		return CTemplateEquipment::PART_BALL;		// 法球
	case EQUIPMENT_CLOAK:		return CTemplateEquipment::PART_CLOAK;		// 披风
	case EQUIPMENT_FASHION:		return CTemplateEquipment::PART_FASHION;	// 时装		
	case EQUIPMENT_MAGICWEAPON:	return CTemplateEquipment::PART_MAGICWEAPON;	// 时装
	case EQUIPMENT_WIZARD1:		return CTemplateEquipment::PART_WIZARD;		// 才华
	case EQUIPMENT_WIZARD2:		return CTemplateEquipment::PART_WIZARD;		// 才华
	case EQUIPMENT_WIZARD3:		return CTemplateEquipment::PART_WIZARD;		// 才华
	case EQUIPMENT_MARRY:		return CTemplateEquipment::PART_MARRY;		// 结婚
	}

	return -1;
}

int CEquipment::Equip( CPropertyPlayer* pProperty, CItemBox& rBaggage, unsigned int vSrcIndex, unsigned int &vDesIndex )
{
	// 新增修改，不能直接放到越界位置
	//if( vSrcIndex >= rBaggage.mReleaseIndex )
	// TODO::包括包裹背包索引的判断
	if ( !rBaggage.CheckIndexValid( vSrcIndex ) )	
	{
		LOG_ERROR( "pro", "[%s:%d] index %d over flow of %d", __FUNCTION__, __LINE__, vSrcIndex, rBaggage.mReleaseIndex );
		return ERROR_EQUIPITEM_ITEMNOTEXIST;
	}
	
	if ( rBaggage.CheckIndexValid( vSrcIndex, true ) == false && GetItemObjPtr( vDesIndex ) != NULL )
	{
		return ERROR_EQUIPITEM_CANOTEXCHANGE;
	}		
	
	CItemObject* tpSrcItem = (CItemObject*) CSceneObjManager::GetSingletonPtr()->GetObject( rBaggage[ vSrcIndex ] );
	if ( tpSrcItem == NULL )
	{
		LOG_ERROR( "pro", "[%s:%d] itemobj index:%d id:%d not exist", __FUNCTION__, __LINE__, vSrcIndex, rBaggage[ vSrcIndex ] );
		return ERROR_EQUIPITEM_ITEMNOTEXIST;
	}

	if ( tpSrcItem->IsOvertimed() == true )
	{
		return ERROR_EQUIPITEM_INVALIDTIME;
	}

	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( tpSrcItem->GetItemID() );
	if ( tpItem == NULL )
	{
		LOG_ERROR( "pro", "[%s:%d] item template id %d not exist", __FUNCTION__, __LINE__, tpSrcItem->GetItemID()  );
		return ERROR_ITEM_DATANOTEXIST;
	}

	//int tDesIndex = -1;
	switch ( tpItem->mItemType )
	{
	case CTplItem::ITEM_FASHION:
	case CTplItem::ITEM_WEAPON:
	case CTplItem::ITEM_ARMOR:
	case CTplItem::ITEM_MAGICWEAPON:
		{
			CTemplateEquipment* tpEquip = (CTemplateEquipment*) tpItem;
			// 检查职业是否满足
			// 左移若干位是可以装备的职业
			if ( tpEquip->mMetierRequire != METIER_NONE && ( ( tpEquip->mMetierRequire & ( 1 << ( pProperty->GetPropertyTempID( ) - 1 ) ) ) == 0 ) )
				return ERROR_EQUIPITEM_NEEDMETIER;

			// 检查等级是否满足
			CTemplateEquipUpg *tUpgList = (CTemplateEquipUpg*)CDataStatic::GetUpgrade( );
			CItemEquipment *tEquip = (CItemEquipment*)tpSrcItem;
			if ( ( tpEquip->mLevelRequire + ( ( tEquip->GetLevel() > 0 ) ? tUpgList->mEquipUpg[ UPG_PROPERTY ][ tEquip->GetLevel() - 1 ].mIncLevel : 0 ) ) > (int)pProperty->GetLevel( ) + pProperty->GetReduceLevel( ) )
				return ERROR_EQUIPITEM_NEEDMORELEVEL;

			// 检查性别是否满足
			if ( tpEquip->mSexRequire != 1 && tpEquip->mSexRequire != (int)pProperty->GetSex( ) )
				return ERROR_EQUIPITEM_NEEDSEX;

			// 检查装备部位
			switch( tpEquip->mEquipType )
			{
			case CTemplateEquipment::PART_RING:
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = ( GetItemObjPtr( EQUIPMENT_RINGRIGHT ) == NULL? EQUIPMENT_RINGRIGHT : EQUIPMENT_RINGLEFT ); 
					if ( vDesIndex != EQUIPMENT_RINGRIGHT && vDesIndex != EQUIPMENT_RINGLEFT )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}					
			case CTemplateEquipment::PART_CUFF:
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = ( GetItemObjPtr( EQUIPMENT_CUFFRIGHT ) == NULL? EQUIPMENT_CUFFRIGHT : EQUIPMENT_CUFFLEFT );
					if ( vDesIndex != EQUIPMENT_CUFFRIGHT && vDesIndex != EQUIPMENT_CUFFLEFT )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_WEAPON:
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = EQUIPMENT_WEAPON;
					if ( vDesIndex != EQUIPMENT_WEAPON )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_CHEST:	
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = EQUIPMENT_CHEST;
					if ( vDesIndex != EQUIPMENT_CHEST )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_LEG:
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = EQUIPMENT_LEG;
					if ( vDesIndex != EQUIPMENT_LEG )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_HEAD:
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = EQUIPMENT_HEAD;
					if ( vDesIndex != EQUIPMENT_HEAD )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_NECK:
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = EQUIPMENT_NECK;
					if ( vDesIndex != EQUIPMENT_NECK )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_SASH:
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = EQUIPMENT_SASH;
					if ( vDesIndex != EQUIPMENT_SASH )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_CHARM:
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = ( GetItemObjPtr( EQUIPMENT_CHARMRIGHT ) == NULL? EQUIPMENT_CHARMRIGHT : EQUIPMENT_CHARMLEFT );
					if ( vDesIndex != EQUIPMENT_CHARMRIGHT && vDesIndex != EQUIPMENT_CHARMLEFT )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_BALL:
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = EQUIPMENT_BALL;
					if ( vDesIndex != EQUIPMENT_BALL )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_CLOAK:
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = EQUIPMENT_CLOAK;
					if ( vDesIndex != EQUIPMENT_CLOAK )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_FASHION:
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = EQUIPMENT_FASHION;
					if ( vDesIndex != EQUIPMENT_FASHION )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_MAGICWEAPON: // 法宝
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = EQUIPMENT_MAGICWEAPON;
					if ( vDesIndex != EQUIPMENT_MAGICWEAPON )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_WIZARD:		// 才华
				{
					if ( vDesIndex == (unsigned int)~0 )
					{
						if ( GetItemObjPtr( EQUIPMENT_WIZARD1 ) == NULL )
							vDesIndex = EQUIPMENT_WIZARD1;
						else if ( GetItemObjPtr( EQUIPMENT_WIZARD2 ) == NULL )
							vDesIndex = EQUIPMENT_WIZARD2;
						else if ( GetItemObjPtr( EQUIPMENT_WIZARD3 ) == NULL )
							vDesIndex = EQUIPMENT_WIZARD3;
						else
							vDesIndex = EQUIPMENT_WIZARD1;
					}
					if ( vDesIndex != EQUIPMENT_WIZARD1 && vDesIndex != EQUIPMENT_WIZARD2 && vDesIndex != EQUIPMENT_WIZARD3 )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}
			case CTemplateEquipment::PART_MARRY: // 法宝
				{
					if ( vDesIndex == (unsigned int)~0 )
						vDesIndex = EQUIPMENT_MARRY;
					if ( vDesIndex != EQUIPMENT_MARRY )
						return ERROR_EQUIPITEM_NOTEQUIPABLE;
					break;
				}

			default:	
				break;
			}

			break;
		}
	default: 
		vDesIndex =	 (unsigned int)~0;
		break;
	}	

	if ( vDesIndex == (unsigned int)~0 )
		return ERROR_EQUIPITEM_NOTEQUIPABLE;

	// 如果是装备绑定物品则进行绑定
	tpSrcItem->BindItem( ITEM_BINDWAY_BYEQUIP );

	CItemObject* tpDesItem	= GetItemObjPtr( vDesIndex );
	if ( tpDesItem == NULL )
	{
		rBaggage[ vSrcIndex ]		= INVALID_OBJ_ID;
	}
	else
	{
		rBaggage[ vSrcIndex ]		= tpDesItem->get_id();
	}

	operator[ ]( vDesIndex )	= tpSrcItem->get_id();

	return SUCCESS;
}

int CEquipment::UnEquip( CPropertyPlayer* pProperty, CItemBox& rBaggage, unsigned short vPart, unsigned int vIndex )
{
	// 新增修改，不能直接放到越界位置
	// TODO::
	if( !rBaggage.CheckIndexValid( vIndex, true  ) && vIndex != (unsigned int)~0 )
		// if( vIndex >= rBaggage.mReleaseIndex && vIndex != (unsigned int)~0 )
	{		
		return ERROR_UNEQUIPITEM_EXPIREDBAG;
	}

	CItemObject* tpSrcItem = GetItemObjPtr( vPart );
	if( tpSrcItem == NULL )
	{
		LOG_ERROR( "pro", "[%s:%d] item part %d can't find", __FUNCTION__, __LINE__, vPart );
		return ERROR_OBJECT_NOTFOUND ;
	}

	if ( vIndex == (unsigned int)~0 ) // 说明要销毁
	{
		//// 自动找一个空位
		//unsigned int vDesIndex = rBaggage.Space( );
		//if ( vDesIndex == (unsigned int)~0 )
		//	return ERROR_EQUIPITEM_NOSPACE;

		//rBaggage[ vDesIndex ]	= tpSrcItem->get_id();
		//operator[]( vPart )		= INVALID_OBJ_ID;
		operator[]( vPart )	= INVALID_OBJ_ID;			
		return SUCCESS;
	}
	else /*if ( vIndex != ~0 )*/
	{
		// 放入指定位置
		CItemObject* tpDesItem = rBaggage.GetItemObjPtr( vIndex );
		if ( tpDesItem != NULL )
		{
			// 检查一下目标位置的道具是否可以放在该部位上
			CTemplateEquipment* tpItemTpl = (CTemplateEquipment*) CDataStatic::SearchTpl( tpDesItem->GetItemID() );
			if ( tpItemTpl == NULL )
				return ERROR_ITEM_DATANOTEXIST;

			// 如果可以放在同一个部位上
			unsigned int tTempIndex = vPart;
			if ( tpItemTpl->mEquipType == CEquipment::GetPartType( vPart ) )
			{
				return Equip( pProperty, rBaggage, vIndex, tTempIndex );
			}

			// 卸下装备时，指定的目标格子无效
			return ERROR_UNEQUIPITEM_DESINVALID;
		}

		if ( tpDesItem == NULL )
		{
			operator[]( vPart )	= INVALID_OBJ_ID;			
			rBaggage[ vIndex ]	= tpSrcItem->get_id();
		}
	}

	return SUCCESS;
}

unsigned short CEquipment::GetIncSkillLevel( unsigned short vSkillID )
{
	unsigned short tValue = 0;
	/*for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_CLOAK; i ++ )
	{
	CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );
	if ( tpEquip != NULL )
	tValue += tpEquip->GetIncSkillLevel( vSkillID );
	}*/
	tValue += GetValueByID( vSkillID, CTemplateProperty::PROPERTYINC_SKILLLEVEL );
	CTemplateSkill *tpSkill = (CTemplateSkill*)CDataStatic::SearchSkill( vSkillID, 1 );
	if ( tpSkill != NULL )
	{
		tValue += GetValueByID( tpSkill->mGenerID, CTemplateProperty::PROPERTYINC_SKILLGENRELEVEL );
	}

	return tValue;
}

unsigned short CEquipment::GetValueByID( unsigned short vSkillID, CTemplateProperty::PROPERTY_FUNC vType )
{
	unsigned short tValue = 0;
	for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_MARRY; i ++ )
	{
		CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );
		if ( tpEquip != NULL )
			tValue += tpEquip->GetValueByID( vSkillID, vType );
	}

	tValue += GetSuit( vType, 0, vSkillID );

	return tValue;
}

int	CEquipment::GetStateID( StateList& vBuffList,CTemplateProperty::PROPERTY_FUNC vType )
{
	for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_MARRY; i ++ )
	{
		CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );
		if ( tpEquip != NULL )
		{
			tpEquip->GetStateID( vBuffList, vType );
		}
	}

	return 0;
}

int CEquipment::GetSuit( unsigned short vFunc, unsigned short vType, unsigned short vSkillID )
{
	int tValue = 0;											
	bool tSuit[ SUITIDNUM ][ SUITPOSNUM ];
	char tName[ SUITPOSNUM ][ NAME_LENGTH ];
	unsigned char tNum[ SUITPOSNUM ];
	unsigned char tUpg[ ITEMUPLIMIT + 1 ] = { 0 };
	memset( tSuit, 0, sizeof(tSuit) );
	memset( tName, '\0', sizeof(tName) );
	memset( tNum, 0, sizeof(tNum) );
	int tExtra = 0;
	for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_CLOAK; i ++ )	
	{																	
		CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );		
		if ( tpEquip != NULL )											
		{
			/*if( tpEquip->GetDurability() == 0 )
			{
			continue;
			}*/

			// 如果是+N的装备，0 - N-1的属性都可以激发
			for ( int j = 0; j <= tpEquip->GetLevel() && j < ( int )(ARRAY_CNT(tUpg)); j++ )
			{
				tUpg[ j ]++;
			}			

			CTemplateEquipment *tplEquip = (CTemplateEquipment*)CDataStatic::SearchTpl( tpEquip->GetItemID() );
			if ( tplEquip != NULL )
			{
				if ( !( tplEquip->mSuitID >= SUITIDNUM || tplEquip->mSuitPos > SUITPOSNUM || tplEquip->mSuitPos == 0 ) )
				{
					tSuit[ tplEquip->mSuitID ][ tplEquip->mSuitPos - 1 ] = true;
				}				
			}			

			for ( int j = 0; j < SUITPOSNUM; j++ )
			{
				if ( tName[ j ][ 0 ] == '\0' )
				{
					if( tpEquip->GetNameLength() > 0 )
						strncpy( tName[ j ], tpEquip->GetName(), std::min( tpEquip->GetNameLength(), (unsigned char)(NAME_LENGTH-1)) );
					break;
				}
				if ( strcmp( tName[ j ], tpEquip->GetName() ) == 0 )
				{
					tNum[ j ]++;
					break;
				}
			}

			int tSpellID = tpEquip->GetSlotSpell();
			CTplItemRune *tpRune = (CTplItemRune*)CDataStatic::SearchTpl( tSpellID );
			if ( tpRune != NULL )
			{
				if ( tpRune->mSuitID >= SUITIDNUM || tpRune->mOrderID > SUITPOSNUM || tpRune->mOrderID == 0 )
					continue;
				tSuit[ tpRune->mSuitID ][ tpRune->mOrderID - 1 ] = true;
			}	

			if ( tpEquip->GetExtraType() != 0 )
			{
				tExtra++;
			}
		}									
	}	
	for ( int i = 0; i < SUITIDNUM; i++ )
	{
		int tCount = 0;
		for ( int j = 0; j < SUITPOSNUM; j++ )
		{
			if ( tSuit[ i ][ j ] == true )
			{
				tCount++;
			}
		}
		CTemplateSpell *tpSpell = (CTemplateSpell*)CDataStatic::SearchTpl( CDataStatic::GetSuitID( i ) );
		if ( tpSpell == NULL )
			continue;
		// 套装最多有5个奖励
		for ( unsigned int n = 0; n < ARRAY_CNT(tpSpell->mRequire); n++ )
		{
			if ( tpSpell->mRequire[ n ] == 0 )
				break;
			if ( tCount >= tpSpell->mRequire[ n ] )
			{
				// 每个奖励最多4个属性
				for ( unsigned int k = 0; k < ARRAY_CNT(tpSpell->mPropertyTempID[ 0 ] ); k++ )
				{
					CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tpSpell->mPropertyTempID[ n ][ k ] );
					if ( tpProperty == NULL )
						continue;
					if ( vFunc == tpProperty->mPropertyFunc )
					{
						if ( vType == 0 )
						{
							if ( vSkillID == 0 || vSkillID == tpProperty->mSkillID )
								tValue += tpProperty->mIntMinValue;
						}
						if ( vType == 1 )
						{
							tValue += tpProperty->mFloatMinValue;
						}
					}
				}
			}
		}
	}

	CTemplateSuitProducer *tProSuit = (CTemplateSuitProducer*)CDataStatic::GetTemp( TEMP_PROSUIT );
	if ( tProSuit == NULL )
	{
		LOG_ERROR( "pro", "GetTemp() failed.[%s : %d]", __LK_FILE__, __LINE__ );
		return tValue;
	}

	for ( int i = ITEMUPLIMIT; i >= 0 ; i-- )
	{
		if ( tUpg[ i ] >= ARRAY_CNT( tProSuit->mUpgTempID[ 0 ] ) )
			continue;
		CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tProSuit->mUpgTempID[ i ][ tUpg[ i ] ] );
		if ( tpProperty != NULL )
		{
			if ( vFunc == tpProperty->mPropertyFunc )
			{
				if ( vType == 0 )
				{
					if ( vSkillID == 0 || vSkillID == tpProperty->mSkillID )
						tValue += tpProperty->mIntMinValue;
				}
				if ( vType == 1 )
				{
					tValue += tpProperty->mFloatMinValue;
				}
			}
			// 不管是不是要找的属性，只要到了最高级，下面的套装就忽略不计了
			break;
		}
	}

	for ( int j = 0; j < SUITPOSNUM ; j++ )
	{
		if ( tNum[ j ] == 0 )
			break;
		// 如果大于4件有附加效果
		if ( tNum[ j ] >= MIN_PRO_AWARD_NUM )
		{
			CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tProSuit->mPropertyTempID[ tNum[ j ] - MIN_PRO_AWARD_NUM ] );
			if ( tpProperty != NULL )
			{
				if ( vFunc == tpProperty->mPropertyFunc )
				{
					if ( vType == 0 )
					{
						if ( vSkillID == 0 || vSkillID == tpProperty->mSkillID )
							tValue += tpProperty->mIntMinValue;
					}
					if ( vType == 1 )
					{
						tValue += tpProperty->mFloatMinValue;
					}
				}
			}			
		}
	}

	if ( tExtra < (int)ARRAY_ROW(tProSuit->mExtTempID) ) 
	{
		for ( int i = 0 ; i < (int)ARRAY_COL(tProSuit->mExtTempID); i++ )
		{
			CTemplateProperty* tpProperty = (CTemplateProperty*) CDataStatic::SearchTpl( tProSuit->mExtTempID[ tExtra ][ i ] );
			if ( tpProperty != NULL )
			{
				if ( vFunc == tpProperty->mPropertyFunc )
				{
					if ( vType == 0 )
					{
						if ( vSkillID == 0 || vSkillID == tpProperty->mSkillID )
							tValue += tpProperty->mIntMinValue;
					}
					if ( vType == 1 )
					{
						tValue += tpProperty->mFloatMinValue;
					}
				}
			}
		}		
	}	
	return tValue;
}

int CEquipment::GetSuitInc( unsigned short vFunc )
{
	return GetSuit( vFunc, 0 );
}

int CEquipment::GetSuitPer( unsigned short vFunc )
{		
	return GetSuit( vFunc, 1 );
}
void CEquipment::DeleteUnusedItem( int vCharID )
{
	CItemList<EQUIP_LIMIT>::DeleteUnusedItem( vCharID );
}
unsigned int CEquipment::GetFixPAttackUpper( CPropertyPlayer* pProperty )
{
	unsigned int tValue = 0;
	for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_MARRY; i ++ )
	{
		CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );
		if ( tpEquip != NULL )
		{			
			tValue += tpEquip->GetFixNumber( CTemplateProperty::PROPERTYINC_PATTACKFIX, TYPE_PATTACKHIGH );
		}
	}
	return tValue;
}

unsigned int CEquipment::GetFixPAttackLower( CPropertyPlayer* pProperty )
{
	unsigned int tValue = 0;
	for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_MARRY; i ++ )
	{
		CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );
		if ( tpEquip != NULL )
		{
			tValue += tpEquip->GetFixNumber( CTemplateProperty::PROPERTYINC_PATTACKFIX, TYPE_PATTACKLOW );			
		}
	}
	return tValue;
}

unsigned int CEquipment::GetFixMAttackUpper( CPropertyPlayer* pProperty )
{
	unsigned int tValue = 0;
	for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_MARRY; i ++ )
	{
		CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );
		if ( tpEquip != NULL )
		{
			tValue += tpEquip->GetFixNumber( CTemplateProperty::PROPERTYINC_MATTACKFIX, TYPE_MATTACKHIGH );			
		}
	}
	return tValue;
}

unsigned int CEquipment::GetFixMAttackLower( CPropertyPlayer* pProperty )
{
	unsigned int tValue = 0;
	for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_MARRY; i ++ )
	{
		CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );
		if ( tpEquip != NULL )
		{
			tValue += tpEquip->GetFixNumber( CTemplateProperty::PROPERTYINC_MATTACKFIX, TYPE_MATTACKLOW );
		}
	}
	return tValue;
}

unsigned int CEquipment::GetFixPDefence( CPropertyPlayer* pProperty )
{
	unsigned int tValue = 0;
	for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_MARRY; i ++ )
	{
		CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );
		if ( tpEquip != NULL )
		{
			tValue += tpEquip->GetFixNumber( CTemplateProperty::PROPERTYINC_PDEFENCEFIX, TYPE_PDEFENCE );
		}
	}
	return tValue;
}

unsigned int CEquipment::GetFixMDefence( CPropertyPlayer* pProperty )
{
	unsigned int tValue = 0;
	for ( int i = EQUIPMENT_WEAPON; i <= EQUIPMENT_MARRY; i ++ )
	{
		CItemEquipment* tpEquip = (CItemEquipment*) GetItemObjPtr( i );
		if ( tpEquip != NULL )
		{
			tValue += tpEquip->GetFixNumber( CTemplateProperty::PROPERTYINC_MDEFENCEFIX, TYPE_MDEFENCE );
		}
	}
	return tValue;
}


template<int NUMBER>
void CItemList<NUMBER>::DeleteUnusedItem( int vCharID )
{
	for ( int i = 0; i < NUMBER; i ++ )
	{
		if( mpItems[ i ] == INVALID_OBJ_ID )
		{
			continue;
		}

		lk::vector<int, 20>::iterator it = CSceneServer::GetSingleton().mDeleteList.begin();
		for ( ; it != CSceneServer::GetSingleton().mDeleteList.end(); ++it )
		{
			CItemObject *tpItemObject = (CItemObject*)CSceneObjManager::GetSingleton().GetObject( mpItems[ i ] );
			if ( tpItemObject == NULL )
			{
				continue;
			}
			if ( *it == (int)(tpItemObject->GetItemID()) )
			{
				CSceneObjManager::GetSingletonPtr( )->DestroyObject( mpItems[ i ] ) ;
				mpItems[ i ] = INVALID_OBJ_ID;
				LOG_ERROR( "pro" ,"Delete Unused Item %d Player %d", *it, vCharID );
				continue;
			}
		}
	}
}