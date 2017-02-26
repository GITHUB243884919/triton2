#include "stdafx.hpp"
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

// ********************************************************************** //
// CBuffObject
// ********************************************************************** //

// Buff对数值影响数值(固定数值或百分比)
unsigned int CBuffObject::GetAffectNumber( CTemplateBuff::BUFF_FUN_TYPE vBuffType )
{
	unsigned int tValue = 0;

	CTemplateBuff* tpBuffTpl = (CTemplateBuff*) CDataStatic::SearchTpl( mBuffID );	
	for ( unsigned int i = 0; ( tpBuffTpl != NULL ) && ( i < sizeof( tpBuffTpl->mBuff ) / sizeof( tpBuffTpl->mBuff[ 0 ] ) ); i++ ) 																	
	{																				
		if ( tpBuffTpl->mBuff[ i ].mBuffFunc == 0 )									
			break;																	
		if ( tpBuffTpl->mBuff[ i ].mBuffFunc == vBuffType )		
		{																			
			tValue += tpBuffTpl->mBuff[ i ].mBuffParam[ 0 ];					
		}																			
	}

	return tValue;
}

// 得到buff的作用间隔; -1表示失败；0表示是持续性作用buff; >0表示是间隔性作用buff
int CBuffObject::GetBuffActionTime( ) 
{
	CTemplateBuff* tpBuffTpl = (CTemplateBuff*) CDataStatic::SearchTpl( mBuffID );
	if ( tpBuffTpl == NULL )
	{
		return -1;
	}

	return tpBuffTpl->mBuffActionTime;
}


void CBuffObject::ChangeParames( int vBuffID, int vIndex, int vParames )
{
	mParames[ vIndex ] = vParames;
}

void CBuffObject::ChangeParames2( int vBuffID, int vIndex, int vParames2 )
{
	mParames2[ vIndex ] = vParames2;
}



// ********************************************************************** //
// CBuffList
// ********************************************************************** //

// 得到BuffList对数值影响的固定数值
unsigned int CBuffList::GetFixNumber( CTemplateBuff::BUFF_FUN_TYPE vBuffType )
{
	unsigned int tValue = 0;
	unsigned int i = 0;

	for( i = 0; i < mBuffNumber; i++ )
	{
		tValue += mBuffList[ i ].GetAffectNumber( vBuffType );
	}

	return tValue;
}

int CBuffList::GetPerNumber( CTemplateBuff::BUFF_FUN_TYPE vBuffType )
{
	int tValue = 0;
	unsigned int i = 0;

	for( i = 0; i < mBuffNumber; i++ )
	{
		tValue += mBuffList[ i ].GetAffectNumber( vBuffType );
	}

	return tValue;
}

//  是否有buff
bool CBuffList::HasBuff( int nBuffID )
{
	for( unsigned int i = 0 ; i < mBuffNumber; i++ )
	{
		if( mBuffList[i].mBuffID == (unsigned int)nBuffID )
		{
			return true;
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// Buff
//////////////////////////////////////////////////////////////////////////



// 清除所有buff, 包括定时器
int CBuffList::RemoveAllBuff( CEntityCharacter* pEntity )
{
	for ( unsigned int i = 0; i < mBuffNumber; i ++ )
	{
		if ( mBuffList[ i ].mBuffTimerID != INVALID_OBJ_ID )
		{
			pEntity->GetTimerReciver()->ClearTimer( mBuffList[ i ].mBuffTimerID );
		}
	}

	Clean();

	return SUCCESS;
}


CBuffObject* CBuffList::InsertBuff( unsigned int vBuffID, int vPersistTime, CEntityCharacter* pSrcEntity, unsigned int& vLastBuffID, int vAttack, CEntityCharacter* pDstEntity, int* vListID, int* vResultCode )
{
	LK_ASSERT( pDstEntity != NULL, return 0 );

	vLastBuffID = 0;
	CTemplateBuff* tpBuffTpl2 = (CTemplateBuff*) CDataStatic::SearchTpl( vBuffID );
	if ( tpBuffTpl2 == NULL )
	{
		LOG_ERROR( "pro", "[%s:%d] buff id %d not found", __FUNCTION__, __LINE__, vBuffID );
		*vResultCode = ERROR_USESKILL_INSERTBUFF_UNTEMPLATE;
		return NULL;
	}

	int nNewBuffIdx = -1;
	bool tNewListID = true;

	for( unsigned int i = 0; i < mBuffNumber && i < ARRAY_CNT( mBuffList ); i ++ )
	{
		CTemplateBuff* tpBuffTpl1 = (CTemplateBuff*) CDataStatic::SearchTpl( mBuffList[ i ].mBuffID );
		if ( tpBuffTpl1 == NULL )
		{
			LOG_ERROR( "pro", "[%s:%d] buff id %d not found", __FUNCTION__, __LINE__, mBuffList[ i ].mBuffID );
			continue;
		}

		// 如果是要去叠加的效果
		if ( tpBuffTpl2->mRemoveCountID == mBuffList[ i ].mBuffID )
		{
			mBuffList[ i ].mBuffSuperCount--;
			if ( mBuffList[ i ].mBuffSuperCount == 0 )
			{
				pDstEntity->EntityRemoveBuff( mBuffList[ i ].mBuffID );	
			}
			*vResultCode = ERROR_USESKILL_INSERTBUFF_REMOVECOUNT;
			return NULL;
		}

		// 如果Buff系列ID相同
		if ( tpBuffTpl1->mBuffSeriesID == tpBuffTpl2->mBuffSeriesID )
		{
			if( ( tpBuffTpl1->mTempID == tpBuffTpl2->mTempID )
				&& ( tpBuffTpl1->mBuffSuperposition != CTemplateBuff::NONE_SUPERPOSITION ) )
			{
				if( tpBuffTpl1->mBuffSuperposition == CTemplateBuff::TIME_SUPERPOSITION )
				{
					// 时间叠加类型Buff
					// 当前时间Buff剩余时间加上新加Buff时间

					CTimerItem* tpTimer = CTimerMng::GetSingletonPtr( )->GetTimer( mBuffList[ i ].mBuffTimerID );
					int tLastTime = ( tpTimer == NULL ) ? 0 : tpTimer->mMillSeconds;
					// 持续时间不能越界,不能再增加
					if(  (int)(tLastTime + vPersistTime) < 0 )
					{			
						// 返回错误
						*vResultCode = ERROR_USESKILL_BUFFTIME_LIMIT;
						return NULL;
					}
					else
					{
						mBuffList[ i ].mPersistTime = tLastTime + vPersistTime;	
					}
					
					mBuffList[ i ].mBuffSuperCount = 1;

					tNewListID = false;

					nNewBuffIdx = i;

					*vResultCode = ERROR_USESKILL_INSERTBUFF_TIMEADD;
					break;
				}
				else if( tpBuffTpl1->mBuffSuperposition == CTemplateBuff::FUNC_SUPERPOSITION )
				{
					// 只有功能叠加的buff才计算叠加数
					mBuffList[ i ].mBuffSuperCount++;
					continue;
				}
			}
			else if ( ( tpBuffTpl2->mIdenValue >= tpBuffTpl1->mIdenValue ) && 
				( tpBuffTpl1->mBuffSuperposition == CTemplateBuff::NONE_SUPERPOSITION ))
			{
				vLastBuffID = mBuffList[ i ].mBuffID;
				*vListID = mBuffList[ i ].mListID;
				mBuffList[ i ].mBuffID		= vBuffID;
				mBuffList[ i ].mPersistTime	= vPersistTime;
				mBuffList[ i ].mSrcEntityID	= pSrcEntity->GetEntityID();
				mBuffList[ i ].mLastInterval= tpBuffTpl2->mBuffFirstActionTime;
				mBuffList[ i ].mTimerType	= tpBuffTpl2->mTimerType;
				mBuffList[ i ].mBackDamage	= 0;
				mBuffList[ i ].mAttack = vAttack;
				mBuffList[ i ].mBuffSuperCount = 1;

				nNewBuffIdx = i;
				break;
			}

			*vResultCode = ERROR_USESKILL_INSERTBUFF_LARGE;
			return NULL;
		}
	}

	if( nNewBuffIdx == -1 && mBuffNumber < ARRAY_CNT( mBuffList ) )
	{
		mBuffList[ mBuffNumber ].mBuffID        = vBuffID;
		mBuffList[ mBuffNumber ].mPersistTime   = vPersistTime;
		mBuffList[ mBuffNumber ].mSrcEntityID   = pSrcEntity->GetEntityID();
		mBuffList[ mBuffNumber ].mLastInterval= tpBuffTpl2->mBuffFirstActionTime;
		mBuffList[ mBuffNumber ].mTimerType = tpBuffTpl2->mTimerType;
		mBuffList[ mBuffNumber ].mBackDamage    = 0;
		mBuffList[ mBuffNumber ].mAttack = vAttack;
		mBuffList[ mBuffNumber ].mBuffSuperCount = 1;

		nNewBuffIdx = mBuffNumber;

		*vResultCode = SUCCESS;
		mBuffNumber++;
	}


	// 为buff找到了位置
	if( nNewBuffIdx != -1 )
	{

		if( tNewListID == true )
		{
			mBuffListID++;
			mBuffList[ nNewBuffIdx ].mListID = mBuffListID;//sBuffListID;
		}

		// 时间型buff
		if( tpBuffTpl2->mLastTime > 0 )
		{
			// 如果之前有timer,先删除
			if( mBuffList[ nNewBuffIdx ].mBuffTimerID != INVALID_OBJ_ID )
			{
				pDstEntity->GetTimerReciver()->ClearTimer( mBuffList[ nNewBuffIdx ].mBuffTimerID );
				mBuffList[ nNewBuffIdx ].mBuffTimerID = INVALID_OBJ_ID;
			}

			int nOtherInfo[4] = { MESSAGE_PROPERTY, vBuffID, mBuffList[ nNewBuffIdx ].mListID, 0 };
			mBuffList[ nNewBuffIdx ].mBuffTimerID = pDstEntity->GetTimerReciver()->SetTimer( pDstEntity->GetEntityID(), 
				TIMER_BUFF_FUNCTION,
				tpBuffTpl2->mBuffActionTime > 0 ? tpBuffTpl2->mBuffActionTime : mBuffList[ nNewBuffIdx ].mPersistTime,
				3,
				nOtherInfo);
		} 

		return &mBuffList[ nNewBuffIdx ];

	}

	*vResultCode = ERROR_USESKILL_INSERTBUFF_FULL;
	return NULL;
}




int CBuffList::RemoveBuff( unsigned int vBuffID, CEntityCharacter* pEntity, int* vListID  )
{

	for ( unsigned int i = 0; i < mBuffNumber && i < ( ARRAY_CNT( mBuffList ) ) ; i ++ )
	{
		if ( mBuffList[ i ].mBuffID == vBuffID && ( ( *vListID == 0 ) ? true : *vListID == mBuffList[ i ].mListID ) )
		{
			if( mBuffList[i].mBuffTimerID != INVALID_OBJ_ID )
			{
				pEntity->GetTimerReciver()->ClearTimer( mBuffList[i].mBuffTimerID );
				mBuffList[i].mBuffTimerID = INVALID_OBJ_ID;
			}

			*vListID = mBuffList[ i ].mListID;

			// 如果不是最后一个buff,要把最后的buff填充过来，保证数组连续性 
			if( i < ( mBuffNumber - 1 ) )
			{
				mBuffList[ i ] = mBuffList[ mBuffNumber - 1 ];

				// 清除buff内存关键数据
				mBuffList[ mBuffNumber - 1 ].Initialize();
			}
			else
			{
				// 清除buff内存关键数据
				mBuffList[ i ].Initialize();
			}


			mBuffNumber --;

			return i;
		}
	}

	return -1;
}

void CBuffList::CreateFromTlv2( CPropertyCharacter* vpCharacter, PBBuffList* pBuffer )
{
	LK_ASSERT( vpCharacter != NULL, return );

	int nTempBuffNum = 0;

	for ( unsigned int i = 0; i <  ARRAY_CNT(mBuffList) && i < (unsigned int)pBuffer->buffobjects_size(); i ++ )
	{
		const PBBuffObject& tpBuffObject = pBuffer->buffobjects( i );

		mBuffList[ nTempBuffNum ].mTimerType	= tpBuffObject.timertype();

		if ( mBuffList[ nTempBuffNum ].mTimerType == CTemplateBuff::BUFF_TIMER_TYPE_REAL )
		{
			time_t tTime = time( NULL );
			unsigned long long tTime64 = tTime * 1000;
			if( tTime64 >= tpBuffObject.persisttime( ) )
			{
				// buff已经过期
				mBuffList[ nTempBuffNum ].mPersistTime = 0;
				continue;
			}
			else
			{
				mBuffList[ nTempBuffNum ].mPersistTime = tpBuffObject.persisttime( ) - tTime64;
			}
		}
		else
		{
			//TODO: buff持续时间太长（大于24天）也会越界，不过目前还没有这么长的时间，但是一旦出现了，必须注意	
			//持续时间 -1 为永久型buff
			mBuffList[ nTempBuffNum ].mPersistTime = tpBuffObject.persisttime();
			if( mBuffList[ nTempBuffNum ].mPersistTime <= 0 && mBuffList[ nTempBuffNum ].mPersistTime != -1 )
			{
				mBuffList[ nTempBuffNum ].mPersistTime = 0;
				continue;
			}
		}

		//		sBuffListID++;	
		mBuffListID++;
		mBuffList[ nTempBuffNum ].mBuffID		= tpBuffObject.buffid();
		mBuffList[ nTempBuffNum ].mListID		= mBuffListID;
		mBuffList[ nTempBuffNum ].mBuffSuperCount = 
			( tpBuffObject.supercount( ) == 0 ) ? 1 : tpBuffObject.supercount( ); 

		// 如果剩余时间大于模板时间 则强制重置成模板时间
		CTemplateBuff* tpTplBuff = (CTemplateBuff*)CDataStatic::SearchTpl( mBuffList[ nTempBuffNum ].mBuffID );
		if ( tpTplBuff == NULL )
		{
			LOG_ERROR("pro", "no such buff in player?? buffid=%d", mBuffList[ nTempBuffNum ].mBuffID );
			continue;
		}

		// buff 实时计算状态
		for( unsigned int j = 0; j < ARRAY_CNT( tpTplBuff->mBuff ); j++ )
		{
			if( tpTplBuff->mBuff[ j ].mBuffFunc != CTemplateBuff::BUFFINC_NONE )
			{
				vpCharacter->AttachBuffState( 0, &tpTplBuff->mBuff[ j ], tpTplBuff->mTempID );
			}
		}

		mBuffList[ nTempBuffNum ].mBackDamage = tpBuffObject.backdamage();

		int tParamesSize = tpBuffObject.parames_size();
		for ( int j = 0; j < tParamesSize && j < MAX_BUFFFUN_COUNT; j++ )
		{
			mBuffList[ nTempBuffNum ].mParames[ j ] = tpBuffObject.parames( j );
		}
		tParamesSize = tpBuffObject.parames2_size();
		for ( int j = 0; j < tParamesSize && j < MAX_BUFFFUN_COUNT; j++)
		{
			mBuffList[ nTempBuffNum ].mParames2[ j ] = tpBuffObject.parames2( j );
		}

		nTempBuffNum++;
	}

	mBuffNumber = std::min(  ( int)pBuffer->buffnumber(), (int)nTempBuffNum );

	PBTransform *tpTran = pBuffer->mutable_transform();

	mTransform.mHasTransform = tpTran->has_hastransform();
	mTransform.mSexID = tpTran->sex();
	mTransform.mStateID = tpTran->stateid();
	mTransform.mWeaponID = tpTran->weaponid();
	mTransform.mClothID = tpTran->clothid();
	mTransform.mFashionID = tpTran->fashionid();
	mTransform.mNpcID = tpTran->npcid();

	//TODO: 之前出过一次错误，hastransform被写成bool值了，后来才改为int,
	// 这里修复一次，凡是hastransform为1的(因为buffid一定是一个比1大的数字)，都重置为0，整个结构也要重置
	if( mTransform.mHasTransform == 1)
	{
		mTransform.Clear();
	}
}

void CBuffList::GetTlvBuffer2( PBBuffList* pBuffer )
{
	if( mBuffNumber > ARRAY_CNT( mBuffList ) )
	{
		mBuffNumber = ARRAY_CNT( mBuffList ) ;
	}

	int nTempBuffNumber = 0;
	for ( unsigned int i = 0; i < mBuffNumber; i ++ )
	{
		PBBuffObject* tpPBBuffObject = pBuffer->add_buffobjects();

		tpPBBuffObject->set_buffid( mBuffList[ i ].mBuffID );
		tpPBBuffObject->set_timertype( mBuffList[ i ].mTimerType );
		tpPBBuffObject->set_listid( mBuffList[ i ].mListID );
		tpPBBuffObject->set_supercount( mBuffList[ i ].mBuffSuperCount );
		if ( mBuffList[ i ].mTimerType == CTemplateBuff::BUFF_TIMER_TYPE_REAL )
		{
			if( mBuffList[ i ].mPersistTime <= 0 && mBuffList[ i ].mPersistTime != -1)
			{
				continue;
			}

			unsigned long long tTime64 = time( NULL ) * 1000;
			tpPBBuffObject->set_persisttime( ( unsigned long long )mBuffList[ i ].mPersistTime + tTime64 );
		} 
		else
		{
			// 当前Buff可能超时, 但还没删除 | -1表示永久型buff
			if( mBuffList[ i ].mPersistTime <= 0 && mBuffList[ i ].mPersistTime != -1)
			{
				continue;
			}

			tpPBBuffObject->set_persisttime( mBuffList[ i ].mPersistTime );
		}

		tpPBBuffObject->set_backdamage( mBuffList[ i ].mBackDamage );

		for ( int j = 0; j < MAX_BUFFFUN_COUNT; j++ )
		{
			tpPBBuffObject->add_parames( mBuffList[ i ].mParames[ j ] );
			tpPBBuffObject->add_parames2( mBuffList[ i ].mParames2[ j ] );
		}

		nTempBuffNumber++;
	}

	pBuffer->set_buffnumber( nTempBuffNumber );

	PBTransform *tpTran = pBuffer->mutable_transform();
	tpTran->set_hastransform( mTransform.mHasTransform );
	tpTran->set_sex( mTransform.mSexID );
	tpTran->set_stateid( mTransform.mStateID );
	tpTran->set_weaponid( mTransform.mWeaponID );
	tpTran->set_clothid( mTransform.mClothID );
	tpTran->set_fashionid( mTransform.mFashionID );
	tpTran->set_npcid( mTransform.mNpcID );
}

CBuffObject* CBuffList::GetBuffObjectByID( int nBuffID )
{
	for ( unsigned int i = 0; mBuffNumber > 0 && i < ( ARRAY_CNT( mBuffList ) ) ; i ++ )
	{
		if ( mBuffList[ i ].mBuffID == (unsigned int)nBuffID )
		{
			return &mBuffList[ i ];
		}
	}

	return NULL;
}

// 得到仅供显示用的buff信息
void CBuffList::GetTlvBufferTeamShow( PBBuffList* pBuffer )
{
	if( mBuffNumber > ARRAY_CNT( mBuffList ) )
	{
		mBuffNumber = ARRAY_CNT( mBuffList ) ;
	}

	pBuffer->set_buffnumber( mBuffNumber );

	for ( unsigned int i = 0; i < mBuffNumber; i ++ )
	{
		PBBuffObject* tpPBBuffObject = pBuffer->add_buffobjects();

		tpPBBuffObject->set_buffid( mBuffList[ i ].mBuffID );
		tpPBBuffObject->set_listid( mBuffList[ i ].mListID );
	}

	if( mTransform.mHasTransform )
	{
		PBTransform* pPBTrans = pBuffer->mutable_transform();
		GetTransform( pPBTrans );
	}
}

// 得到用于玩家自身显示的buff信息
void CBuffList::GetTlvBufferShow( PBBuffList* pBuffer )
{
	if( mBuffNumber > ARRAY_CNT( mBuffList ) )
	{
		mBuffNumber = ARRAY_CNT( mBuffList );
	}

	pBuffer->set_buffnumber( mBuffNumber );

	for ( unsigned int i = 0; i < mBuffNumber; i ++ )
	{
		PBBuffObject* tpPBBuffObject = pBuffer->add_buffobjects();

		tpPBBuffObject->set_buffid( mBuffList[ i ].mBuffID );
		tpPBBuffObject->set_listid( mBuffList[ i ].mListID );
		if ( mBuffList[ i ].mTimerType == CTemplateBuff::BUFF_TIMER_TYPE_REAL )
		{
			tpPBBuffObject->set_persisttime( mBuffList[ i ].mPersistTime );
		} 
		else
		{
			tpPBBuffObject->set_persisttime( mBuffList[ i ].mPersistTime );
		}
	}

	if( mTransform.mHasTransform )
	{
		PBTransform* pPBTrans = pBuffer->mutable_transform();
		GetTransform( pPBTrans );
	}
}


void CBuffList::RefreshBuffLeftTime( )
{
	for( unsigned int i = 0; i < mBuffNumber; i++ )
	{
		if( mBuffList[ i ].GetBuffTimerID() != INVALID_OBJ_ID )
		{
			// buff比较特殊，不能简单的把剩余时间回写，必须走buff每次作用间隔的流逝时间
			CTimerItem* pTimerItem = CTimerMng::GetSingletonPtr()->GetTimer( mBuffList[ i ].GetBuffTimerID() );
			if( pTimerItem != NULL )
			{
				// 对于持续性作用buff,直接刷新剩余时间
				if( mBuffList[ i ].GetBuffActionTime( ) == 0 )
				{
					//TODO: 目前buff时间还没有很长的，超过24天，所有只用微秒参数
					mBuffList[ i ].mPersistTime = pTimerItem->mMillSeconds ;	
				}
			}

			//TODO: 对于间隔性作用的buff,不处理，即使服务器宕机，也能容忍一次间隔的误差, 一般都是10s左右
		}
	}
}

void CBuffList::SetTransform( CEntityCharacter *pEntity, int nBuffID )
{
	// 如果传进来的是空就清空状态
	if ( pEntity == NULL )
	{
		mTransform.Clear();
		return;
	}
	if ( pEntity->IsPlayer() )
	{
		CEntityPlayer *tpPlayer = (CEntityPlayer*)pEntity;
		CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
		CBuffList* tpBuff = tpProperty->GetBuffList( );
		bool tHasTransform = false;
		// 如果对方也有变身则复制
		if ( tpBuff->mTransform.mSexID != 0 )
		{
			mTransform.mSexID = tpBuff->mTransform.mSexID;
			tHasTransform = true;
		}
		if ( tpBuff->mTransform.mStateID != 0 )
		{
			mTransform.mStateID = tpBuff->mTransform.mStateID;
			tHasTransform = true;
		}
		if ( tpBuff->mTransform.mWeaponID != 0 )
		{
			mTransform.mWeaponID = tpBuff->mTransform.mWeaponID;
			tHasTransform = true;
		}
		if ( tpBuff->mTransform.mClothID != 0 )
		{
			mTransform.mClothID = tpBuff->mTransform.mClothID;
			tHasTransform = true;
		}
		if ( tpBuff->mTransform.mFashionID != 0 )
		{
			mTransform.mFashionID = tpBuff->mTransform.mFashionID;
			tHasTransform = true;
		}
		if ( tpBuff->mTransform.mNpcID != 0 )
		{
			mTransform.mNpcID = tpBuff->mTransform.mNpcID;
			tHasTransform = true;
		}

		if ( tHasTransform == false )
		{
			mTransform.mSexID = tpPlayer->GetSex();
			int tStateID = tpProperty->GetStateParam( STATE_TRANSFORM ).mParames1;
			CTemplateNpc *tplNpc = (CTemplateNpc*)CDataStatic::SearchTpl( tStateID );
			if ( tplNpc != NULL )
			{
				mTransform.mStateID = tStateID;
			}
			//CTemplateEquipment *tplEquip = NULL;
			CItemEquipment* tpWeapon	= (CItemEquipment*) tpProperty->GetEquipment( )->GetItemObjPtr( EQUIPMENT_WEAPON );
			if ( tpWeapon != NULL && tpWeapon->IsOvertimed() == false )
			{
				mTransform.mWeaponID = tpWeapon->GetItemID();
			}	
			CItemEquipment* tpChest		= (CItemEquipment*) tpProperty->GetEquipment( )->GetItemObjPtr( EQUIPMENT_CHEST );
			if ( tpChest != NULL && tpChest->IsOvertimed() == false )
			{
				mTransform.mClothID = tpChest->GetItemID();
			}
			CItemEquipment* tpFashion	= (CItemEquipment*) tpProperty->GetEquipment( )->GetItemObjPtr( EQUIPMENT_FASHION );
			if ( tpFashion != NULL && tpFashion->IsOvertimed() == false )
			{
				mTransform.mFashionID = tpFashion->GetItemID();
			}
		}

		mTransform.mHasTransform = nBuffID;

	}
	if ( pEntity->IsNpc() )
	{
		CEntityNpc *tpNpc = (CEntityNpc*)pEntity;
		CTemplateNpc *tplNpc = (CTemplateNpc*)CDataStatic::SearchTpl( tpNpc->GetNpcID() );
		if ( tplNpc != NULL )
		{
			mTransform.mNpcID = tpNpc->GetNpcID();
			mTransform.mHasTransform = nBuffID;
		}
	}
}

void CBuffList::GetTransform( PBTransform *pTran )
{
	if ( pTran == NULL )
		return;
	
	//TODO: 如果没有方寸山变身状态，就不发送这些信息 ，但是没有一个好的判断方寸山信息的状态
	pTran->set_hastransform( mTransform.mHasTransform );
	pTran->set_sex( mTransform.mSexID );
	pTran->set_stateid( mTransform.mStateID );
	pTran->set_weaponid( mTransform.mWeaponID );
	pTran->set_clothid( mTransform.mClothID );
	pTran->set_fashionid( mTransform.mFashionID );
	pTran->set_npcid( mTransform.mNpcID );
}
