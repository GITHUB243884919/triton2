// ********************************************************************** //
// ProClient.cpp
// 玩家属性数究ㄒ?包(包裹,部?技目?Buff表,勘副?
// 玩伎笨生命,法f,活f,悼?靠,锌?f,耐f,体质,挚?精神
// Author: Black
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
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif


//int CBuffList::sBuffListID=0;


////TODO: 调用该接口前必须先判断个数是否满足
//int CItemBox::RemoveItemByID( unsigned int vItemID, unsigned int vNumber /*= 0xFFFF*/, int vType )
//{
//	CItemBoxToolkit tBoxToolkit(this);
//	
//	int nWantRemveNum = vNumber;
//	int nRemovedNum = 0;
//
//	for ( unsigned short i = 0; i < mReleaseIndex; i ++ )
//	{
//		nRemovedNum = tBoxToolkit.RemoveItem( i, vItemID, vNumber, vType );
//		if( vNumber != (unsigned int)(-1) )
//		{
//			vNumber -= nRemovedNum;
//			if( vNumber == 0 )
//			{
//				ResetPair( vItemID, -nWantRemveNum);
//				return ERROR_REMOVEITEM_SUCCESS;
//			}
//		}
//	}
//	
//	// 包裹背包
//	for ( int i = 0; i < mKitBagIndex; ++i )
//	{
//		int tBeginIndex = 0;
//		int tEndIndex	= 0;
//		int tRet = GetKitBagIndexRange( BAGTYPE_KITBAG, i, tBeginIndex, tEndIndex );
//		if ( tRet != SUCCESS )
//		{
//			continue;
//		}
//		for ( int j = tBeginIndex; j <= tEndIndex; ++j )
//		{
//			nRemovedNum = tBoxToolkit.RemoveItem( j, vItemID, vNumber, vType );
//			if( vNumber != (unsigned int)(-1) )
//			{
//				vNumber -= nRemovedNum;
//				if( vNumber == 0 )
//				{
//					ResetPair( vItemID, -nWantRemveNum);
//					return ERROR_REMOVEITEM_SUCCESS;
//				}
//			}
//		}
//	}
//
//	ResetPair( vItemID, -nWantRemveNum);
//
//	return ERROR_REMOVEITEM_SUCCESS;
//}

// 得到非战斗状态的HP恢复速度
unsigned int CPropertyFormula::GetNormalHPRSpeed( CPropertyCharacter* pProperty )
{
	CPropertyValue tValue;
	if ( pProperty->GetEntityType( ) == ENTITYTYPE_PLAYER )
	{
		CPropertyPlayer* tpProPlayer = ( CPropertyPlayer* )pProperty;
		int tFixValue = tpProPlayer->GetEquipment( )->GetFixNumber( CTemplateProperty::PROPERTYINC_HPRSPEEDFIX )
						+ tpProPlayer->GetEquipment( )->GetSuitInc( CTemplateProperty::PROPERTYINC_HPRSPEEDFIX );	// 得到装备固定值修正
		int tPerValue = tpProPlayer->GetEquipment( )->GetPerNumber( CTemplateProperty::PROPERTYINC_HPRSPEEDPER )
						+ tpProPlayer->GetEquipment( )->GetSuitPer( CTemplateProperty::PROPERTYINC_HPRSPEEDPER );	// 得到装备百分比修正
		
		CBuffList *tpBuff = tpProPlayer->GetBuffList( );
		tFixValue += tpBuff->GetFixNumber( CTemplateBuff::BUFFINC_HPRSPEEDFIX );					// 得到BUFF固定值修正
		tPerValue += tpBuff->GetPerNumber( CTemplateBuff::BUFFINC_HPRSPEEDPER );					// 得到BUFF百分比修正
		
		CSkillList& tpSkill = tpProPlayer->GetSkillList( );
		tFixValue += tpSkill.GetFixNumber( CTemplateSkill::PASSINC_HPRSPEEDFIX );	// 得到技能固定值修正
		tPerValue += tpSkill.GetPerNumber( CTemplateSkill::PASSINC_HPRSPEEDPER );	// 得到技能百分比修正

		CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( pProperty->GetPropertyTempID() );
		if ( tpMetierTpl == NULL )
			return tValue;

		tValue.mInitValue	= tpMetierTpl->mNormalHPRSpeed;
		tValue.mDeltaValue	= 0;

	}

	if ( pProperty->GetEntityType( ) == ENTITYTYPE_FUNCNPC || pProperty->GetEntityType( ) == ENTITYTYPE_OGRE 
			|| pProperty->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( pProperty->GetEntityID( ) );
		if ( tpEntityNpc == NULL )
		{
			LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_OGRE || tpEntityNpc->GetEntityType( ) == ENTITYTYPE_FUNCNPC)
		{
			CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( pProperty->GetPropertyTempID( ) );
			if ( tpNpcTpl == NULL )
				return tValue;

			CBuffList *tpBuff = pProperty->GetBuffList( );
			int		tFixValue	= tpBuff->GetFixNumber( CTemplateBuff::BUFFINC_HPRSPEEDFIX );			// 得到BUFF固定值修正
			int		tPerValue	= tpBuff->GetPerNumber( CTemplateBuff::BUFFINC_HPRSPEEDPER );			// 得到BUFF百分比修正

			int		tTotalValue	= ( int )( ( tpNpcTpl->mHPRSpeed + tFixValue ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
			int		tInitValue	= tpNpcTpl->mHPRSpeed;
			tTotalValue			= std::max( tTotalValue, 0 );

			tValue.mInitValue	= tInitValue;
			tValue.mDeltaValue	= tTotalValue - tInitValue;
			return tValue;
		}

		if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
		{
			CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
			if ( tpEntityPet == NULL )
			{
				LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
				return tValue;
			}

			if ( 
				tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
				&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
				&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
				)
			{
				LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
				return tValue;
			}

			CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( pProperty->GetPropertyTempID( ) );
			if ( tpPetTpl == NULL )
			{
				LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
				return tValue;
			}

			CBuffList *tpBuff = pProperty->GetBuffList( );
			int		tFixValue	= tpBuff->GetFixNumber( CTemplateBuff::BUFFINC_HPRSPEEDFIX );			// 得到BUFF固定值修正
			int		tPerValue	= tpBuff->GetPerNumber( CTemplateBuff::BUFFINC_HPRSPEEDPER );			// 得到BUFF百分比修正

			int		tTotalValue	= ( int )( ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mHPRSpeed + tFixValue ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
			int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mHPRSpeed;
			tTotalValue			= std::max( tTotalValue, 0 );

			tValue.mInitValue	= tInitValue;
			tValue.mDeltaValue	= tTotalValue - tInitValue;
			return tValue;
		}
	}

	return tValue;
}

// 得到非战斗状态的MP恢复速度
unsigned int CPropertyFormula::GetNormalMPRSpeed( CPropertyCharacter* pProperty )
{
	CPropertyValue tValue;
	if ( pProperty->GetEntityType( ) == ENTITYTYPE_PLAYER )
	{
		CPropertyPlayer* tpProPlayer = ( CPropertyPlayer* )pProperty;

		int tFixValue = tpProPlayer->GetEquipment( )->GetFixNumber( CTemplateProperty::PROPERTYINC_MPRSPEEDFIX )
						+ tpProPlayer->GetEquipment( )->GetSuitInc( CTemplateProperty::PROPERTYINC_MPRSPEEDFIX );// 得到装备固定值修正
		int	tPerValue = tpProPlayer->GetEquipment( )->GetPerNumber( CTemplateProperty::PROPERTYINC_MPRSPEEDPER )
						+ tpProPlayer->GetEquipment( )->GetSuitPer( CTemplateProperty::PROPERTYINC_MPRSPEEDPER );	// 得到装备百分比修正

		CBuffList *tpBuff = tpProPlayer->GetBuffList( );
		tFixValue += tpBuff->GetFixNumber( CTemplateBuff::BUFFINC_MPRSPEEDFIX );	// 得到BUFF固定值修正
		tPerValue += tpBuff->GetFixNumber( CTemplateBuff::BUFFINC_MPRSPEEDPER );	// 得到BUFF百分比修正

		CSkillList& tpSkill = tpProPlayer->GetSkillList( );
		tFixValue += tpSkill.GetFixNumber( CTemplateSkill::PASSINC_MPRSPEEDFIX );		// 得到技能固定值修正
		tPerValue += tpSkill.GetPerNumber( CTemplateSkill::PASSINC_MPRSPEEDPER );		// 得到技能百分比修正

		CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( pProperty->GetPropertyTempID() );
		if ( tpMetierTpl == NULL )
			return tValue;

		// 得到基本属性修正
		//int tCorrect		= (int)( tpMetierTpl->mMPRSpeedPerSpr * ( int )CPropertyFormula::GetSpr( tpProPlayer ) );
		//int tTotalValue		= (int)( ( tpMetierTpl->mNormalMPRSpeed + tFixValue + tCorrect ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
		//int tInitValue		= (int)( tpMetierTpl->mNormalMPRSpeed + tCorrect );

		tValue.mInitValue	= tpMetierTpl->mNormalMPRSpeed;
		tValue.mDeltaValue	= 0;

		//LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] EntityID( %d ) EquipmentFix( %d | %d ) EquipmentPer( %d | %d ) BufferFix( %d ) BufferPer( %d ) SkillFix( %d ) SkillPer( %d ) Correct( %d ) TotalValue( %d ) InitValue( %d )",
		//	__LK_FILE__,
		//	__LINE__,
		//	__FUNCTION__, 
		//	pProperty->GetEntityID(), 
		//	tpProPlayer->mEquipment.GetFixMPRSpeed(), 
		//	tpProPlayer->mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_MPRSPEEDFIX ),
		//	tpProPlayer->mEquipment.GetPerMPRSpeed(),
		//	tpProPlayer->mEquipment.GetSuitPer( CTemplateProperty::PROPERTYINC_MPRSPEEDPER ),
		//	tpProPlayer->mBuff.GetFixNumber( CTemplateBuff::BUFFINC_MPRSPEEDFIX ),
		//	tpProPlayer->mBuff.GetPerNumber( CTemplateBuff::BUFFINC_MPRSPEEDPER ),
		//	tpProPlayer->mSkill.GetFixNumber( tpProPlayer, CTemplateSkill::PASSINC_MPRSPEEDFIX ),
		//	tpProPlayer->mSkill.GetPerNumber( tpProPlayer, CTemplateSkill::PASSINC_MPRSPEEDPER ),
		//	tCorrect,
		//	tTotalValue,
		//	tInitValue
		//	);
	}

	if ( pProperty->GetEntityType( ) == ENTITYTYPE_FUNCNPC || pProperty->GetEntityType( ) == ENTITYTYPE_OGRE 
			|| pProperty->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( pProperty->GetPropertyTempID( ) );
		if ( tpNpcTpl == NULL )
			return tValue;

		CBuffList *tpBuff = pProperty->GetBuffList( );
		int		tFixValue	= tpBuff->GetFixNumber( CTemplateBuff::BUFFINC_MPRSPEEDFIX );			// 得到BUFF固定值修正
		int		tPerValue	= tpBuff->GetPerNumber( CTemplateBuff::BUFFINC_MPRSPEEDPER );			// 得到BUFF百分比修正

		int		tTotalValue	= ( int )( ( tpNpcTpl->mMPRSpeed + tFixValue ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
		int		tInitValue	= tpNpcTpl->mMPRSpeed;
		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
	}

	return tValue;
}


// 得到法术攻击最小值
CPropertyValue CPropertyFormula::GetMAttackLower( CPropertyCharacter* pProperty )
{
	return pProperty->GetMAttackMin( );
}

// 得到法术攻击最大值
CPropertyValue CPropertyFormula::GetMAttackUpper( CPropertyCharacter* pProperty )
{
	return pProperty->GetMAttackMax( );
}

// 得到物理攻击最小值
CPropertyValue CPropertyFormula::GetPAttackLower( CPropertyCharacter* pProperty )
{
	return pProperty->GetPAttackMin( );
}

// 得到物理攻击最大值
CPropertyValue CPropertyFormula::GetPAttackUpper( CPropertyCharacter* pProperty )
{
	return pProperty->GetPAttackMax( );
}

CPropertyValue CPropertyFormula::GetCon( CPropertyPlayer* pProperty )
{
	return pProperty->GetCon();
}

CPropertyValue CPropertyFormula::GetSta( CPropertyPlayer* pProperty )
{
	return pProperty->GetSta( );
}

CPropertyValue CPropertyFormula::GetWis( CPropertyPlayer* pProperty )
{
	return pProperty->GetWis( );
}

CPropertyValue CPropertyFormula::GetSpr( CPropertyPlayer* pProperty )
{
	return pProperty->GetSpr( );
}

CPropertyValue CPropertyFormula::GetStr( CPropertyPlayer* pProperty )
{
	return pProperty->GetStr( );
}

// 得到法术抗性
//CPropertyValue CPropertyFormula::GetMResist( CPropertyCharacter* pProperty )
//{
//	return pProperty->GetMResist( );
//}

// 得到物理抗性
//CPropertyValue CPropertyFormula::GetPResist( CPropertyCharacter* pProperty )
//{
//	return pProperty->GetPResist( );
//}

// 得到物理免疫
int CPropertyFormula::GetPImmunity( int vLevel, CPropertyCharacter* pProperty )
{
	int i = 0;
	int tFixValue = 0;
	int tPerValue = 0;

	CSkillList& tpSkillList = pProperty->GetSkillList( );
	for( i = 0; i < tpSkillList.GetSkillNum(); i++ )
	{
		CSkillObject* pSkillObj = tpSkillList.GetSkillObjectByIndex( i );

		CTemplateSkill* tpSkill = pSkillObj->GetSkillTpl();
		if( tpSkill != NULL )
		{
			if( tpSkill->mSkillFuncID1 == CTemplateSkill::PASSINC_DAMAGELESSFIX )
				tFixValue += tpSkill->mParameter1[ 0 ];
			else if( tpSkill->mSkillFuncID1 == CTemplateSkill::PASSINC_DAMAGELESSPER )
				tPerValue += tpSkill->mParameter1[ 0 ];
            
			if( tpSkill->mSkillFuncID2 == CTemplateSkill::PASSINC_DAMAGELESSFIX )
                tFixValue += tpSkill->mParameter2[ 0 ];
            else if( tpSkill->mSkillFuncID2 == CTemplateSkill::PASSINC_DAMAGELESSPER )
                tPerValue += tpSkill->mParameter2[ 0 ];
            
			if( tpSkill->mSkillFuncID3 == CTemplateSkill::PASSINC_DAMAGELESSFIX )
                tFixValue += tpSkill->mParameter3[ 0 ];
            else if( tpSkill->mSkillFuncID3 == CTemplateSkill::PASSINC_DAMAGELESSPER )
                tPerValue += tpSkill->mParameter3[ 0 ];
            
			if( tpSkill->mSkillFuncID4 == CTemplateSkill::PASSINC_DAMAGELESSFIX )
                tFixValue += tpSkill->mParameter4[ 0 ];
            else if( tpSkill->mSkillFuncID4 == CTemplateSkill::PASSINC_DAMAGELESSPER )
                tPerValue += tpSkill->mParameter4[ 0 ];
		}
	}
	int tPImmunity = 10900 * tFixValue / ( tFixValue + 300 * ( vLevel - pProperty->GetLevel( ) )  + 10000 ) + tPerValue;
	return tPImmunity;
}

// 得到法术免疫
int CPropertyFormula::GetMImmunity( int vLevel, CPropertyCharacter* pProperty )
{
    int i = 0;
    int tFixValue = 0;
    int tPerValue = 0;
   
	CSkillList& tpSkillList = pProperty->GetSkillList( );
    for( i = 0; i < tpSkillList.GetSkillNum(); i++ )
    {
		CSkillObject* pSkillObj = tpSkillList.GetSkillObjectByIndex( i );
        CTemplateSkill* tpSkill = pSkillObj->GetSkillTpl();
        if( tpSkill != NULL )
        {
            if( tpSkill->mSkillFuncID1 == CTemplateSkill::PASSINC_DAMAGELESSFIX )
                tFixValue += tpSkill->mParameter1[ 0 ];
            else if( tpSkill->mSkillFuncID1 == CTemplateSkill::PASSINC_DAMAGELESSPER )
                tPerValue += tpSkill->mParameter1[ 0 ];
            
			if( tpSkill->mSkillFuncID2 == CTemplateSkill::PASSINC_DAMAGELESSFIX )
                tFixValue += tpSkill->mParameter2[ 0 ];
            else if( tpSkill->mSkillFuncID2 == CTemplateSkill::PASSINC_DAMAGELESSPER )
                tPerValue += tpSkill->mParameter2[ 0 ];
            
			if( tpSkill->mSkillFuncID3 == CTemplateSkill::PASSINC_DAMAGELESSFIX )
                tFixValue += tpSkill->mParameter3[ 0 ];
            else if( tpSkill->mSkillFuncID3 == CTemplateSkill::PASSINC_DAMAGELESSPER )
                tPerValue += tpSkill->mParameter3[ 0 ];
            
			if( tpSkill->mSkillFuncID4 == CTemplateSkill::PASSINC_DAMAGELESSFIX )
                tFixValue += tpSkill->mParameter4[ 0 ];
            else if( tpSkill->mSkillFuncID4 == CTemplateSkill::PASSINC_DAMAGELESSPER )
                tPerValue += tpSkill->mParameter4[ 0 ];
        }
    }
	int tMImmunity = 10900 * tFixValue / ( tFixValue + 300 * ( vLevel - pProperty->GetLevel( ) )  + 10000 ) + tPerValue;
    return tMImmunity;
}

// 得到法术防御
CPropertyValue CPropertyFormula::GetMDefence( CPropertyCharacter* pProperty )
{
	return pProperty->GetMDefence( );
}

// 得到物理防御
CPropertyValue CPropertyFormula::GetPDefence( CPropertyCharacter* pProperty )
{
	return pProperty->GetPDefence( );
}

// 计算技能抗性伤害
unsigned int CPropertyFormula::GetResAttack( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateSkill* pSkill )
{
	if ( pSkill == NULL || pSrcEntity == NULL || pDesEntity == NULL )
		return 0;

	CPropertyCharacter* pSrcProperty = (CPropertyCharacter*) pSrcEntity->GetProperty();
	CPropertyCharacter* pDesProperty = (CPropertyCharacter*) pDesEntity->GetProperty();

	int tAttValue = pSkill->mResValue + pSrcProperty->GetResAtt( pSkill->mResType );
	int tDefValue = pDesProperty->GetResDef( pSkill->mResType );	

	float tResParam1 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mResparam1 / SERVER_PERCENT_FLOAT;
	float tResParam2 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mResparam2 / SERVER_PERCENT_FLOAT;
	float tResParam3 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mResparam3 / SERVER_PERCENT_FLOAT;
	float tResParam4 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mResparam4 / SERVER_PERCENT_FLOAT;
	float tResParam5 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mResparam5 / SERVER_PERCENT_FLOAT;

	float tRate = tResParam1 * tDefValue / ( tResParam2 * tDefValue + tResParam3 + tResParam4 * pow( pSrcProperty->GetLevel( ), tResParam5 ) );

	return tAttValue * ( (1 - tRate) > 0 ? (1 - tRate) : 0 );
}

// 计算技能物理伤害
unsigned int CPropertyFormula::GetPSkillAttack( 	CEntityCharacter* pSrcEntity,
													CEntityCharacter* pDesEntity,
													CTemplateSkill* pSkill,
													EResultType& rResult
												)
{
	if ( pSkill == NULL || pSrcEntity == NULL || pDesEntity == NULL )
		return 0;

	CPropertyCharacter* pSrcProperty = (CPropertyCharacter*) pSrcEntity->GetProperty();
	CPropertyCharacter* pDesProperty = (CPropertyCharacter*) pDesEntity->GetProperty();

	// 计算致命几率
	int tDeathRateFix = 0;
	int tDeathValueFix = 0;
	CSkillObject *pObject = pSrcProperty->GetSkillList( ).GetSkillObjectByID( pSkill->mSkillID );
	if ( pObject != NULL )
	{
		tDeathRateFix = pObject->GetDeathRateFix();
		tDeathValueFix = pObject->GetDeathValueFix();
	}
	int tDeath = (int)GetDeathRate( pSrcProperty, (pSkill->mIsMagicWeaponSkill==1) );

	int		tAttackValue	= ( int )( CPropertyFormula::GetPAttackValue( pSrcProperty, (pSkill->mIsMagicWeaponSkill==1) ) 
								* ( pSkill->mAttackPer / SERVER_PERCENT_FLOAT ) + pSkill->mAttackFix );
	int		tDefence		= GetPDefence( pDesProperty );
	int		tDeathDefence	= GetDeathDefense( pDesProperty );
	int		tDeathValue		= SERVER_PERCENT_INT;
	int		tFinalDamage	= 0; 
	int		tIgnoreDefence	= 0;

	// 检测攻击玩家的BUFF对攻击的影响
	CPropertyModule::GetSingleton().CheckBuffGetSkillAttackSrcEntity( pSrcEntity, pSkill, tDeath, tDeathValue, tIgnoreDefence, CTemplateBuff::BUFFFUNC_IGNOREPDEFENCE );
	tIgnoreDefence = std::min( tIgnoreDefence, SERVER_PERCENT_INT );
	tDefence = tDefence / SERVER_PERCENT_FLOAT * ( SERVER_PERCENT_INT - tIgnoreDefence );

	int tDeathRate = CPropertyFormula::GetDeathRate( pDesProperty->GetLevel( ), tDeath, tDeathDefence ) * SERVER_PERCENT_FLOAT;
	tDeathRate += tDeathRateFix;
	// 如果造成致命伤害
	if ( RAND( 10000 ) < tDeathRate )
	{
		rResult	= RESULT_DEATHHIT;
		tDeathValue = std::max( (int)( (int)CPropertyFormula::GetDeathValue( pSrcProperty ) + tDeathValueFix - (int)CPropertyFormula::GetDeathDefense( pDesProperty ) * (int)CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDeathchangeparam ), SERVER_PERCENT_INT );
	}

	int		tAddFixDamage = GetAddFixDamage( pSrcProperty );		// 伤害固定值加成
	int		tAddPerDamage = GetAddPerDamage( pSrcProperty );		// 伤害百分比加成
	int		tDecFixDamage = GetDecFixPDamage( pDesProperty );		// 伤害固定值减免
	int		tDecPerDamage = GetDecPerPDamage( pDesProperty );		// 伤害百分比减免

	// 处理称号技能
	int tTitleDamage = 0;
	if ( pSrcEntity->IsPlayer() )
	{
		tTitleDamage = ((CPropertyPlayer*)pSrcProperty)->mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_DAMAGEFIX );
		tTitleDamage += ((CPropertyPlayer*)pSrcProperty)->mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_DAMAGEFIX );
	}
	
	// 处理忽视伤害
	tDecPerDamage = std::max( 0, (tDecPerDamage - ((CPropertyPlayer*)pSrcProperty)->GetBuffList( )->GetPerNumber( CTemplateBuff::BUFFFUNC_IGNOREPIMUPER )) );

	CSkillList& tpSkillList = pSrcProperty->GetSkillList( );
	tFinalDamage = ( int )( std::max( 1, int(tAttackValue - tDefence) )  * ( tDeathValue / SERVER_PERCENT_FLOAT ) *
		( SERVER_PERCENT_FLOAT + tAddPerDamage ) / SERVER_PERCENT_FLOAT + tAddFixDamage + 
			tpSkillList.GetFixNumber( CTemplateSkill::PASSINC_DAMAGEFIX ) + tTitleDamage );
	
	int		tDesBuffFix 	= 0;
	int		tDesBuffPer 	= 0;

	// 检测被攻击玩家的BUFF对攻击的影响
	CPropertyModule::GetSingleton().CheckBuffGetSkillAttackDesEntity( pDesEntity, pSkill, tDesBuffFix, tDesBuffPer );

	tDecFixDamage -= tDesBuffFix;	// 伤害固定值减免
	tDecPerDamage -= tDesBuffPer;	// 伤害百分比减免

	unsigned int tDamage = std::max( 1, (int)( tFinalDamage * ( SERVER_PERCENT_FLOAT - tDecPerDamage ) / SERVER_PERCENT_FLOAT - tDecFixDamage ) );
	unsigned int tResDamage = std::max( 0, (int)( GetResAttack( pSrcEntity, pDesEntity, pSkill ) * ( tDeathValue / SERVER_PERCENT_FLOAT ) ) );
	return tDamage + tResDamage;
}

// 计算技能法术伤害
unsigned int CPropertyFormula::GetMSkillAttack( 	CEntityCharacter* pSrcEntity,
													CEntityCharacter* pDesEntity,
													CTemplateSkill* pSkill, 
													EResultType& rResult
											  )
{
	if ( pSkill == NULL || pSrcEntity == NULL || pDesEntity == NULL )
		return 0;

	CPropertyCharacter* pSrcProperty = (CPropertyCharacter*) pSrcEntity->GetProperty();
	CPropertyCharacter* pDesProperty = (CPropertyCharacter*) pDesEntity->GetProperty();

	// 计算致命几率
	int tDeathRateFix = 0;
	int tDeathValueFix = 0;
	CSkillObject *pObject = pSrcProperty->GetSkillList( ).GetSkillObjectByID( pSkill->mSkillID );
	if ( pObject != NULL )
	{
		tDeathRateFix = pObject->GetDeathRateFix();
		tDeathValueFix = pObject->GetDeathValueFix();
	}
	int tDeath = (int)GetDeathRate( pSrcProperty, (pSkill->mIsMagicWeaponSkill==1) );

	int		tAttackValue	= (int)( GetMAttackValue( pSrcProperty, (pSkill->mIsMagicWeaponSkill==1) ) * 
								( pSkill->mAttackPer / SERVER_PERCENT_FLOAT ) + pSkill->mAttackFix );
	int		tDefence		= GetMDefence( pDesProperty );
	int		tDeathDefence	= GetDeathDefense( pDesProperty );
	int		tDeathValue		= SERVER_PERCENT_INT;
	int		tFinalDamage	= 0; 
	int		tIgnoreDefence	= 0;

	// 检测攻击玩家的BUFF对攻击的影响
	CPropertyModule::GetSingleton().CheckBuffGetSkillAttackSrcEntity( pSrcEntity, pSkill, tDeath, tDeathValue, tIgnoreDefence, CTemplateBuff::BUFFFUNC_IGNOREMDEFENCE );
	tIgnoreDefence = std::min( tIgnoreDefence, SERVER_PERCENT_INT );
	tDefence = tDefence / SERVER_PERCENT_FLOAT * ( SERVER_PERCENT_INT - tIgnoreDefence );

	int tDeathRate = CPropertyFormula::GetDeathRate( pDesProperty->GetLevel( ), tDeath, tDeathDefence ) * SERVER_PERCENT_FLOAT;
	tDeathRate += tDeathRateFix;
	// 如果造成致命伤害
	if ( ( RAND( 10000 ) ) < tDeathRate )
	{
		rResult	= RESULT_DEATHHIT;
		tDeathValue = std::max( (int)( (int)CPropertyFormula::GetDeathValue( pSrcProperty ) + tDeathValueFix - (int)CPropertyFormula::GetDeathDefense( pDesProperty ) * CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDeathchangeparam ), SERVER_PERCENT_INT );
	}

	int		tAddFixDamage = GetAddFixDamage( pSrcProperty );		// 伤害固定值加成
	int		tAddPerDamage = GetAddPerDamage( pSrcProperty );		// 伤害百分比加成
	int		tDecFixDamage = GetDecFixMDamage( pDesProperty );		// 伤害固定值减免
	int		tDecPerDamage = GetDecPerMDamage( pDesProperty );		// 伤害百分比减免

	// 处理称号技能
	int tTitleDamage = 0;
	if ( pSrcEntity->IsPlayer() )
	{
		tTitleDamage = ((CPropertyPlayer*)pSrcProperty)->mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_DAMAGEFIX );
		tTitleDamage += ((CPropertyPlayer*)pSrcProperty)->mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_DAMAGEFIX );
	}

	// 处理忽视伤害
	tDecPerDamage = std::max( 0, (tDecPerDamage - ((CPropertyPlayer*)pSrcProperty)->GetBuffList( )->GetPerNumber( CTemplateBuff::BUFFFUNC_IGNOREMIMUPER )) );

	CSkillList &tpSkillList = pSrcProperty->GetSkillList( );
	tFinalDamage = ( int )( std::max( 1, int(tAttackValue - tDefence) )  * ( tDeathValue / SERVER_PERCENT_FLOAT ) * 
		( SERVER_PERCENT_FLOAT + tAddPerDamage ) / SERVER_PERCENT_FLOAT + tAddFixDamage + 
			tpSkillList.GetFixNumber( CTemplateSkill::PASSINC_DAMAGEFIX ) + tTitleDamage );

	int		tDesBuffFix 	= 0;
	int		tDesBuffPer 	= 0;

	// 检测被攻击玩家的BUFF对攻击的影响
	CPropertyModule::GetSingleton().CheckBuffGetSkillAttackDesEntity( pDesEntity, pSkill, tDesBuffFix, tDesBuffPer );

	tDecFixDamage -= tDesBuffFix;	// 伤害固定值减免
	tDecPerDamage -= tDesBuffPer;	// 伤害百分比减免

	unsigned int tDamage = std::max( 1, (int)( tFinalDamage * ( SERVER_PERCENT_FLOAT - tDecPerDamage ) / SERVER_PERCENT_FLOAT - tDecFixDamage ) );
	unsigned int tResDamage = GetResAttack( pSrcEntity, pDesEntity, pSkill ) * ( tDeathValue / SERVER_PERCENT_FLOAT );
	return tDamage + tResDamage;
}

// 计算普通物理攻击伤害
unsigned int CPropertyFormula::GetPNormalAttack( 	CEntityCharacter* pSrcEntity,
													CEntityCharacter* pDesEntity, 
													EResultType& rResult												)
{
	return 0;
	/*
	if ( pSrcEntity == NULL || pDesEntity == NULL )
		return 0;

	CPropertyCharacter* pSrcProperty = (CPropertyCharacter*) pSrcEntity->GetProperty();
	CPropertyCharacter* pDesProperty = (CPropertyCharacter*) pDesEntity->GetProperty();

	// 计算命中
	int tNormalHitrate	= pSrcProperty->GetNormalHitRate();
	int tNormalDuck		= GetNormalDuck( pDesProperty );
	int tHitrate		= 0;
	rResult				= RESULT_NORMAL;
	if ( tNormalHitrate == 0 && tNormalDuck == 0 )
		tHitrate		= 99;
	else
	{
		tHitrate		= ( int )( ( tNormalHitrate / ( 0.4f * tNormalDuck + tNormalHitrate ) ) * 100 );
		tHitrate		= std::min( 99, tHitrate );
		tHitrate		= max( 1, tHitrate );
	}
	if ( RAND(100) > tHitrate )
	{
		rResult			= RESULT_NORMALDUCK;
		return 0;
	}

	// 计算致命几率
	int tDeathRate = GetDeathRate( pSrcProperty );

	// 如果造成致命伤害
	if ( ( RAND( 1000 ) ) < tDeathRate )
		rResult			= RESULT_DEATHHIT;

	int		tAttackValue	= GetPAttackValue( pSrcProperty );
	int		tDefence		= GetPDefence( pDesProperty );
	int		tDeathValue		= (int)( ( int )CPropertyFormula::GetDeathValue( pSrcProperty ) );
	int		tFinalDamage	= 0; 

	// 检测攻击玩家的BUFF对攻击的影响
	CPropertyModule::GetSingleton().CheckBuffGetNormalAttackSrcEntity( pSrcEntity, tDeathRate, tDeathValue );

		if ( rResult == RESULT_DEATHHIT )
		tFinalDamage = ( int )( max( 1, tAttackValue - tDefence ) * tDeathValue / SERVER_PERCENT_FLOAT );
	else
		tFinalDamage = ( int )( max( 1, tAttackValue - tDefence ) );

	int		tAddFixDamage = 0;
	float	tAddPerDamage = 0.0;
	int		tDecFixDamage = 0;
	float	tDecPerDamage = 0.0;

	tAddFixDamage	= GetAddFixDamage( pSrcProperty );							// 伤害固定值加成
	tAddPerDamage	= GetAddPerDamage( pSrcProperty ) / SERVER_PERCENT_FLOAT;	// 伤害百分比加成
	tDecFixDamage	= GetDecFixPDamage( pDesProperty );							// 伤害固定值减免	
	tDecPerDamage	= GetDecPerPDamage( pDesProperty ) / SERVER_PERCENT_FLOAT ;	// 伤害百分比减免		

	// 修改过的计算公式
	unsigned int tDamage = max( 1, (int)(( tFinalDamage * ( 1 + tAddPerDamage ) + tAddFixDamage ) * ( 1 - tDecPerDamage ) - tDecFixDamage ) );
	return tDamage;
	*/
}

// 计算普通魔法攻击伤害
unsigned int CPropertyFormula::GetMNormalAttack(	CEntityCharacter* pSrcEntity,
													CEntityCharacter* pDesEntity, 
													EResultType& rResult
													)
{
	return 0;
	/*
	if ( pSrcEntity == NULL || pDesEntity == NULL )
		return 0;

	CPropertyCharacter* pSrcProperty = (CPropertyCharacter*) pSrcEntity->GetProperty();
	CPropertyCharacter* pDesProperty = (CPropertyCharacter*) pDesEntity->GetProperty();

	// 计算命中
	int tNormalHitrate	= pSrcProperty->GetNormalHitRate();
	int tNormalDuck		= GetNormalDuck( pDesProperty );
	int tHitrate		= ( int )( ( tNormalHitrate / ( 0.4f * tNormalDuck + tNormalHitrate ) ) * 100 );
	rResult				= RESULT_NORMAL;
	if ( RAND(100) > tHitrate )
	{
		rResult			= RESULT_NORMALDUCK;
		return 0;
	}

	// 计算致命几率
	int tDeathRate		= GetDeathRate( pSrcProperty );

	// 如果造成致命伤害
	if ( ( RAND( 1000 )  ) < tDeathRate )
		rResult			= RESULT_DEATHHIT;

	int 	tAttackValue	= GetMAttackValue( pSrcProperty );
	int 	tDefence		= GetMDefence( pDesProperty );
	int		tDeathValue		= (int)( ( int )CPropertyFormula::GetDeathValue( pSrcProperty ) );
	int		tFinalDamage	= 0; 

	// 检测攻击玩家的BUFF对攻击的影响
	CPropertyModule::GetSingleton().CheckBuffGetNormalAttackSrcEntity( pSrcEntity, tDeathRate, tDeathValue );

	if ( rResult == RESULT_DEATHHIT )
		tFinalDamage = ( int )( max( 1, tAttackValue - tDefence ) * tDeathValue / SERVER_PERCENT_FLOAT );
	else
		tFinalDamage = ( int )( max( 1, tAttackValue - tDefence ) );

	int		tAddFixDamage = 0;
	float	tAddPerDamage = 0.0;
	int		tDecFixDamage = 0;
	float	tDecPerDamage = 0.0;

	tAddFixDamage	= GetAddFixDamage( pSrcProperty );							// 伤害固定值加成
	tAddPerDamage	= GetAddPerDamage( pSrcProperty ) / SERVER_PERCENT_FLOAT;	// 伤害百分比加成
	tDecFixDamage	= GetDecFixPDamage( pDesProperty );							// 伤害固定值减免	
	tDecPerDamage	= GetDecPerPDamage( pDesProperty ) / SERVER_PERCENT_FLOAT;	// 伤害百分比减免		

	// 修改过的计算公式
	unsigned int tDamage = max( 1, (int)(( tFinalDamage * ( 1 + tAddPerDamage ) + tAddFixDamage ) * ( 1 - tDecPerDamage ) - tDecFixDamage ) );
	return tDamage;
	*/
}

// 由于可能返回值为-值，修改函数
// 得到伤害固定值加成( 不区分物理和法术 )
int CPropertyFormula::GetAddFixDamage( CPropertyCharacter* pProperty )
{
	return pProperty->GetAddFixDamage( );
}

// 得到伤害百分比加成( 不区分物理和法术 )
int CPropertyFormula::GetAddPerDamage( CPropertyCharacter* pProperty )
{
	return pProperty->GetAddPerDamage( );
}

// 得到物理伤害固定值减免
int CPropertyFormula::GetDecFixPDamage( CPropertyCharacter* pProperty )
{
	return pProperty->GetDecFixPDamage( );
}

// 得到物理伤害百分比减免
int CPropertyFormula::GetDecPerPDamage( CPropertyCharacter* pProperty )
{
	return pProperty->GetDecPerPDamage( );
}

// 得到法术伤害固定值减免
int CPropertyFormula::GetDecFixMDamage( CPropertyCharacter* pProperty )
{
	return pProperty->GetDecFixMDamage( );
}

// 得到法术伤害百分比减免
int CPropertyFormula::GetDecPerMDamage( CPropertyCharacter* pProperty )
{
	return pProperty->GetDecPerMDamage( );
}

// 获取伤害偷取HP百分比
int CPropertyFormula::GetStealHP( CPropertyCharacter* pProperty )
{
	return pProperty->GetStealHP( );
}

// 获取伤害偷取MP百分比
int CPropertyFormula::GetStealMP( CPropertyCharacter* pProperty )
{
	return pProperty->GetStealMP( );
}

unsigned int CPropertyFormula::GetPAttackValue( CPropertyCharacter* pProperty, bool bIsMagicWeapon/* = false*/ )
{
	if ( bIsMagicWeapon )
	{
		LK_ASSERT( pProperty->GetEntityType() == ENTITYTYPE_PLAYER, return 0 );
		CPropertyPlayer* tpPro = ( CPropertyPlayer* )pProperty;
		CItemMagicWeapon* tpMW = (CItemMagicWeapon*)tpPro->GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
		if ( tpMW == NULL )
			return 0;
		return tpMW->GetAttackValue();
	}

	int tUpper = ( int )GetPAttackUpper( pProperty );
	int tLower = ( int )GetPAttackLower( pProperty );
	if ( tUpper == tLower )
		return tLower;

	return rand( ) % ( tUpper - tLower + 1 ) + tLower;
}

unsigned int CPropertyFormula::GetMAttackValue( CPropertyCharacter* pProperty, bool bIsMagicWeapon/* = false*/ )
{
	if ( bIsMagicWeapon )
	{
		LK_ASSERT( pProperty->GetEntityType() == ENTITYTYPE_PLAYER, return 0 );
		CPropertyPlayer* tpPro = ( CPropertyPlayer* )pProperty;
		CItemMagicWeapon* tpMW = (CItemMagicWeapon*)tpPro->GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
		if ( tpMW == NULL )
			return 0;
		return tpMW->GetAttackValue();
	}

	int tUpper = ( int )GetMAttackUpper( pProperty );
	int tLower = ( int )GetMAttackLower( pProperty );
	if ( tUpper == tLower )
		return tLower;

	return rand( ) % ( tUpper - tLower + 1 ) + tLower;
}

// 得到普攻命中值
CPropertyValue CPropertyFormula::GetNormalHitrate( CPropertyCharacter* pProperty )
{
	CPropertyValue tValue;
	if ( pProperty->GetEntityType( ) == ENTITYTYPE_PLAYER )
	{
		CPropertyPlayer* tpProPlayer = ( CPropertyPlayer* )pProperty;
		int tFixValue = 0;
		tFixValue += tpProPlayer->GetEquipment( )->GetFixNumber( CTemplateProperty::PROPERTYINC_NORMALHITRATE )
					+ tpProPlayer->GetEquipment( )->GetSuitInc( CTemplateProperty::PROPERTYINC_NORMALHITRATE );// 得到装备固定值修正

		CSkillList& tpSkill = tpProPlayer->GetSkillList( );
		tFixValue += tpSkill.GetFixNumber( CTemplateSkill::PASSINC_NORMALHITRATE );	// 得到技能固定值修正

		CBuffList *tpBuff = tpProPlayer->GetBuffList( );
		tFixValue += tpBuff->GetFixNumber( CTemplateBuff::BUFFINC_NORMALHITRATEFIX );	// 得到BUFF固定值修正

		CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( tpProPlayer->GetPropertyTempID() );
		if ( tpMetierTpl == NULL )
			return tValue;

		// 得到基本属性修正
		int tLevelAmend = (int)( ( tpProPlayer->GetLevel( ) - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaNormalHitrate );

		unsigned int tTotalValue	= tpMetierTpl->mInitNormalHitrate + tLevelAmend + tFixValue;
		unsigned int tInitValue		= tpMetierTpl->mInitNormalHitrate + tLevelAmend;

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;

		//LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] EntityID( %d ) EquipmentFix( %d | %d ) BufferFix( %d ) SkillFix( %d ) tLevelAmend( %d ) tTotalValue( %d ) tInitValue( %d )",
		//	__LK_FILE__,
		//	__LINE__,
		//	__FUNCTION__, 
		//	pProperty->GetEntityID(), 
		//	tpProPlayer->mEquipment.GetFixNormalHitrate(), 
		//	tpProPlayer->mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_NORMALHITRATE ),
		//	tpProPlayer->mBuff.GetFixNumber( CTemplateBuff::BUFFINC_NORMALHITRATEFIX ),
		//	tpProPlayer->mSkill.GetFixNumber( tpProPlayer, CTemplateSkill::PASSINC_NORMALHITRATE ),
		//	tLevelAmend,
		//	tTotalValue,
		//	tInitValue
		//	);
	}

	else if ( pProperty->GetEntityType( ) == ENTITYTYPE_FUNCNPC || pProperty->GetEntityType( ) == ENTITYTYPE_OGRE 
			|| pProperty->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( pProperty->GetPropertyTempID( ) );
		if ( tpNpcTpl == NULL )
			return tValue;
		int tTplHit = tpNpcTpl->mNormalHitrate;
		CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
		if ( pTemplateOgreTran != NULL )
		{
			tTplHit = pTemplateOgreTran->mHit[ pProperty->GetLevel( ) ];
		}

		CBuffList *tpBuff = pProperty->GetBuffList( );
		int		tFixValue	= tpBuff->GetFixNumber( CTemplateBuff::BUFFINC_NORMALHITRATEFIX );				// 得到Buff固定值修正
		int		tTotalValue	= tTplHit + tFixValue;
		int		tInitValue	= tTplHit;
		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
		return tValue;
	}

	return tValue;
}

// 得到普攻闪避值
CPropertyValue CPropertyFormula::GetNormalDuck( CPropertyCharacter* pProperty )
{
	return pProperty->GetNormalDuck( );
}

// 得到技能闪避值( 残影 )
CPropertyValue CPropertyFormula::GetSkillDuck( CPropertyCharacter* pProperty )
{
	return pProperty->GetSkillDuck( );
}

// 得到忽视闪避值
CPropertyValue CPropertyFormula::GetIgnoreDuck( CPropertyCharacter* pProperty )
{
	return pProperty->GetIgnoreDuck( );
}

// 得到致命一击率
CPropertyValue CPropertyFormula::GetDeathRate( CPropertyCharacter* pProperty, bool bIsMagicWeapon/* = false*/ )
{
	if ( bIsMagicWeapon )
	{
		CPropertyValue tValue;
		LK_ASSERT( pProperty->GetEntityType() == ENTITYTYPE_PLAYER, return tValue );
		CPropertyPlayer* tpPro = ( CPropertyPlayer* )pProperty;
		CItemMagicWeapon* tpMW = (CItemMagicWeapon*)tpPro->GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
		if ( tpMW == NULL )
			return tValue;
		tValue.mInitValue = tpMW->GetDeathRate();
		return tValue;
	}
	return pProperty->GetDeathRate( );
}

// 得到致命一击伤害加成
CPropertyValue CPropertyFormula::GetDeathValue( CPropertyCharacter* pProperty )
{
	return pProperty->GetDeathValue( ) ;
}

// 得到致命一击防御
CPropertyValue CPropertyFormula::GetDeathDefense( CPropertyCharacter* pProperty )
{
	return pProperty->GetDeathDefense( ) ;
}

// 得到技能命中率
unsigned int CPropertyFormula::GetSkillHitrate( 	unsigned short vSkillID, 
													unsigned short vSkillLevel, 
													CPropertyCharacter* pProperty 
												)
{
	if ( pProperty->GetEntityType( ) == ENTITYTYPE_PLAYER )
	{
		CPropertyPlayer* tpProPlayer = ( CPropertyPlayer* )pProperty;
		// 得到技能等级
		CTemplateSkill* tpSkillTpl = (CTemplateSkill*) CDataStatic::SearchSkill( vSkillID, vSkillLevel );
		if ( tpSkillTpl == NULL || tpSkillTpl->mSkillType != CTemplateSkill::SKILLTYPE_ATTACK )
			return 0;

		CTemplateNormalSkill* tpNormalSkill = (CTemplateNormalSkill*) tpSkillTpl;

		CBuffList *tpBuff = tpProPlayer->GetBuffList( );
		unsigned int tHitrate = tpNormalSkill->mSkillHitrate + tpBuff->GetFixNumber( CTemplateBuff::BUFFINC_SKILLHITRATEFIX )
			+ tpProPlayer->GetEquipment( )->GetFixNumber( CTemplateProperty::PROPERTYINC_SKILLHITRATE );
		
		//LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] EntityID( %d ) Skill( %d ) EquipmentFix( %d ) BufferFix( %d ) tHitrate( %d )",
		//	__LK_FILE__,
		//	__LINE__,
		//	__FUNCTION__, 
		//	pProperty->GetEntityID(), 
		//	tpNormalSkill->mSkillHitrate, 
		//	tpProPlayer->mBuff.GetFixNumber( CTemplateBuff::BUFFINC_SKILLHITRATEFIX ),
		//	tpProPlayer->mEquipment.GetFixNormalHitrate(),
		//	tHitrate
		//	);
		return tHitrate;
	}
	else
	{
		CPropertyNPC* tpProNpc = ( CPropertyNPC* )pProperty;
		// 得到技能等级
		CTemplateSkill* tpSkillTpl = (CTemplateSkill*) CDataStatic::SearchSkill( vSkillID, 1 );
		if ( tpSkillTpl == NULL || tpSkillTpl->mSkillType != CTemplateSkill::SKILLTYPE_ATTACK )
		{
			LOG_ERROR("pro", "npc skill id %d not fond ", vSkillID );
			return 0;
		}

		CTemplateNormalSkill* tpNormalSkill = (CTemplateNormalSkill*) tpSkillTpl;

		CBuffList *tpBuff = tpProNpc->GetBuffList( );
		unsigned int tHitrate = tpNormalSkill->mSkillHitrate + tpBuff->GetFixNumber( CTemplateBuff::BUFFINC_SKILLHITRATEFIX );
		return tHitrate;
	}

	return 0;
}

// 得到HP最大值
CPropertyValue CPropertyFormula::GetMaxHP( CPropertyCharacter* pProperty )
{																										
	return pProperty->GetMaxHP( );
}

// 得到MP最大值
CPropertyValue CPropertyFormula::GetMaxMP( CPropertyCharacter* pProperty )
{
	return pProperty->GetMaxMP( );
}

// 吊叫凶咭刊丝?
CPropertyValue CPropertyFormula::GetWalkSpeed( CPropertyCharacter *pProperty )
{
	CPropertyValue tValue;
	if( pProperty->GetEntityType( ) == ENTITYTYPE_PLAYER )
	{
		CPropertyPlayer* tpProPlayer = ( CPropertyPlayer* )pProperty;
		CTemplateMetier* tpMetier = (CTemplateMetier*) CDataStatic::SearchMetier( tpProPlayer->GetPropertyTempID() );
		if( tpMetier != NULL )
		{
			tValue.mInitValue = tpMetier->mWalkSpeed;
		}
	}
	else if( pProperty->GetEntityType( ) == ENTITYTYPE_FUNCNPC )
	{
	
	}

	return tValue;
}

// 吊奖寂芤刊丝?
CPropertyValue CPropertyFormula::GetRunSpeed( CPropertyCharacter *pProperty )
{
	CPropertyValue tValue;
	if( pProperty->GetEntityType( ) == ENTITYTYPE_PLAYER )
	{
		CPropertyPlayer* tpProPlayer = ( CPropertyPlayer* )pProperty;
		CTemplateMetier* tpMetier = (CTemplateMetier*) CDataStatic::SearchMetier( tpProPlayer->GetPropertyTempID() );
		if( tpMetier != NULL )
		{
			int tPerInCreamValue = tpProPlayer->GetEquipment( )->GetPerNumber( CTemplateProperty::PROPERTYINC_SPEED ) + tpProPlayer->GetEquipment( )->GetSuitPer( CTemplateProperty::PROPERTYINC_SPEED );

			tPerInCreamValue += tpProPlayer->GetStateParam( STATE_INCREAMSPEED ).mParames1;

			tPerInCreamValue += tpProPlayer->GetSkillList().GetPerNumber( CTemplateSkill::PASSINC_SPEED ) + tpProPlayer->mTalentSkill.GetPerNumber( CTemplateSkill::PASSINC_SPEED ) + tpProPlayer->mTitleSkill.GetPerNumber( CTemplateSkill::PASSINC_SPEED );

			//tPerInCreamValue += tpProPlayer->GetHorseSpeedPer();
			
			int	tPerUnCreamValue = tpProPlayer->GetStateParam( STATE_UNCREAMSPEED ).mParames1;

			int	tTempValue = (int)( SERVER_PERCENT_FLOAT + tPerInCreamValue - tPerUnCreamValue );
			if ( tTempValue != 0 )
				tValue.mInitValue = (int)( ( tpMetier->mRunSpeed ) * SERVER_PERCENT_FLOAT / tTempValue );
			else
				tValue.mInitValue = 0;
		}
	}
	else if ( pProperty->GetEntityType( ) == ENTITYTYPE_OGRE 
		|| pProperty->GetEntityType( ) == ENTITYTYPE_FUNCNPC 
		/*|| pProperty->GetEntityType( ) == ENTITYTYPE_TASKNPC*/ )
	{
		CPropertyNPC* tpProNpc = ( CPropertyNPC* )pProperty;
		CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( tpProNpc->GetPropertyTempID( ) );
		if ( tpNpcTpl == NULL )
			return tValue;

		int tPerInCreamValue = tpProNpc->GetStateParam( STATE_INCREAMSPEED ).mParames1;
		int tPerUnCreamValue = tpProNpc->GetStateParam( STATE_UNCREAMSPEED ).mParames1;

		int tTempValue = (int)( SERVER_PERCENT_FLOAT + tPerInCreamValue - tPerUnCreamValue );
		if ( tTempValue != 0 )
			tValue.mInitValue = (int)( tpNpcTpl->mRunSpeed * SERVER_PERCENT_FLOAT / tTempValue );
		else
			tValue.mInitValue = tpNpcTpl->mRunSpeed;
		return tValue;
	}
	else if ( pProperty->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CPropertyPet* tpProPet = ( CPropertyPet* )pProperty;
		CEntityPet* tpEntityPet = (CEntityPet*)CSceneLogic::GetSingletonPtr()->GetEntity( pProperty->GetEntityID( ) );
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet(%d) is NULL!", __LK_FILE__, __LINE__, __FUNCTION__, pProperty->GetEntityID( )  );
			return tValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
		)
		{
			LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, 
				__FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( tpProPet->GetPropertyTempID());
		if ( tpPetTpl == NULL )
		{
			LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		int tPerInCreamValue = tpProPet->GetStateParam( STATE_INCREAMSPEED ).mParames1;
		int tPerUnCreamValue = tpProPet->GetStateParam( STATE_UNCREAMSPEED ).mParames1;

		int tTempValue = (int)( SERVER_PERCENT_FLOAT + tPerInCreamValue - tPerUnCreamValue );
		if ( tTempValue != 0 )
			tValue.mInitValue = (int)( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mSpeed * SERVER_PERCENT_FLOAT / tTempValue );
		else
			tValue.mInitValue = tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mSpeed;
		return tValue;
	
	}

	return tValue;
}

// 吊奖寂芤刊丝?
CPropertyValue CPropertyFormula::GetRunSpeed( int vEntityType, int vEntityID )
{
	CPropertyValue tValue;
	if( vEntityType == ENTITYTYPE_PLAYER )
	{
		CTemplateMetier* tpMetier = (CTemplateMetier*) CDataStatic::SearchMetier( vEntityID );
		if( tpMetier != NULL )
		{

			tValue.mInitValue = tpMetier->mRunSpeed;
			
		}		
	}
	else if( vEntityType == ENTITYTYPE_FUNCNPC || vEntityType == ENTITYTYPE_OGRE )
	{
		CTemplateOgre *tpOgre = (CTemplateOgre*) CDataStatic::SearchTpl( vEntityID );
		if( tpOgre != NULL )
		{
			tValue.mInitValue = tpOgre->mRunSpeed;
		}		
	}

	return tValue;
}

CPropertyValue CPropertyFormula::GetSkillLess( CPropertyCharacter* pProperty, int vSkillGenerID )
{
	CPropertyValue tValue;
	int i = 0;
	CSkillList&  tpSkill = pProperty->GetSkillList( );
    for( i = 0; i < tpSkill.GetSkillNum(); i++ )
    {	
		CSkillObject* pSkillObj = tpSkill.GetSkillObjectByIndex( i );
		LK_ASSERT( pSkillObj != NULL, continue );

        CTemplateSkill* tpSkillTempl = pSkillObj->GetSkillTpl();
        if ( tpSkillTempl == NULL )
            continue;

        if( tpSkillTempl->mSkillFuncID1 == CTemplateSkill::PASSINC_NEEDMPPER )
		{
			if( (int)tpSkillTempl->mParameter1[ 0 ] == vSkillGenerID )
	            tValue.mInitValue += tpSkillTempl->mParameter1[ 1 ];
		}
		else if( tpSkillTempl->mSkillFuncID1 == CTemplateSkill::PASSINC_NEEDMPFIX )
		{
			if( (int)tpSkillTempl->mParameter1[ 0 ] == vSkillGenerID )
				tValue.mDeltaValue += tpSkillTempl->mParameter1[ 1 ];
		}

        if( tpSkillTempl->mSkillFuncID2 == CTemplateSkill::PASSINC_NEEDMPPER )
        {
            if( (int)tpSkillTempl->mParameter2[ 0 ] == vSkillGenerID )
                tValue.mInitValue += tpSkillTempl->mParameter2[ 1 ];
        }
        else if( tpSkillTempl->mSkillFuncID2 == CTemplateSkill::PASSINC_NEEDMPFIX )
        {
            if( (int)tpSkillTempl->mParameter2[ 0 ] == vSkillGenerID )
                tValue.mDeltaValue += tpSkillTempl->mParameter2[ 1 ];
        }

        if( tpSkillTempl->mSkillFuncID3 == CTemplateSkill::PASSINC_NEEDMPPER )
        {
            if( (int)tpSkillTempl->mParameter3[ 0 ] == vSkillGenerID )
                tValue.mInitValue += tpSkillTempl->mParameter3[ 1 ];
        }
        else if( tpSkillTempl->mSkillFuncID3 == CTemplateSkill::PASSINC_NEEDMPFIX )
        {
            if( (int)tpSkillTempl->mParameter3[ 0 ] == vSkillGenerID )
                tValue.mDeltaValue += tpSkillTempl->mParameter3[ 1 ];
        }

        if( tpSkillTempl->mSkillFuncID4 == CTemplateSkill::PASSINC_NEEDMPPER )
        {
            if( (int)tpSkillTempl->mParameter4[ 0 ] == vSkillGenerID )
                tValue.mInitValue += tpSkillTempl->mParameter4[ 1 ];
        }
        else if( tpSkillTempl->mSkillFuncID4 == CTemplateSkill::PASSINC_NEEDMPFIX )
        {
            if( (int)tpSkillTempl->mParameter4[ 0 ] == vSkillGenerID )
                tValue.mDeltaValue += tpSkillTempl->mParameter4[ 1 ];
        }
    }	

	return tValue;
}

float CPropertyFormula::GetHitrate( int vSrcLevel, int vDesLevel, int vHitValue )
{
	float tHitParam1 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mHitparam1 / SERVER_PERCENT_FLOAT;
	float tHitParam2 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mHitparam2 / SERVER_PERCENT_FLOAT;
	float tHitParam3 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mHitparam3 / SERVER_PERCENT_FLOAT;
	float tHitParam4 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mHitparam4 / SERVER_PERCENT_FLOAT;
	float tHitParam5 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mHitparam5 / SERVER_PERCENT_FLOAT;
	float tHitParam6 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mHitparam6 / SERVER_PERCENT_FLOAT;
	float tHitParam7 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mHitparam7 / SERVER_PERCENT_FLOAT;

	return tHitParam1 + tHitParam2 * ( vSrcLevel - vDesLevel ) + tHitParam3 * vHitValue / ( tHitParam4 * vHitValue + tHitParam5 + pow( vDesLevel, tHitParam6 ) * tHitParam7 );
}

float CPropertyFormula::GetDuckRate( int vSrcLevel, int vDuckValue, int vIgnoreValue )
{
	float tDuckParam1 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDuckparam1 / SERVER_PERCENT_FLOAT;
	float tDuckParam2 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDuckparam2 / SERVER_PERCENT_FLOAT;
	float tDuckParam3 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDuckparam3 / SERVER_PERCENT_FLOAT;
	float tDuckParam4 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDuckparam4 / SERVER_PERCENT_FLOAT;
	float tDuckParam5 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDuckparam5 / SERVER_PERCENT_FLOAT;
	float tDuckParam6 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDuckparam6 / SERVER_PERCENT_FLOAT;

	return tDuckParam1 + tDuckParam2 * ( vDuckValue - vIgnoreValue ) / ( tDuckParam3 * vIgnoreValue + tDuckParam4 + pow( vSrcLevel, tDuckParam5 ) * tDuckParam6 );
}

// 计算暴击率
float CPropertyFormula::GetDeathRate( int vDesLevel, int vDeathRate, int vDeathDefense )
{
	float tDeathParam1 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDeathparam1 / SERVER_PERCENT_FLOAT;
	float tDeathParam2 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDeathparam2 / SERVER_PERCENT_FLOAT;
	float tDeathParam3 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDeathparam3 / SERVER_PERCENT_FLOAT;
	float tDeathParam4 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDeathparam4 / SERVER_PERCENT_FLOAT;
	float tDeathParam5 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDeathparam5 / SERVER_PERCENT_FLOAT;

	return ( vDeathRate - tDeathParam1 * vDeathDefense ) / ( tDeathParam2 * pow( tDeathParam3, ( vDesLevel - tDeathParam4 ) / tDeathParam5 ) ); 
}





