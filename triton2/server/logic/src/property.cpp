#include "stdafx.hpp"
#include "proclient_svr.h"
#include "coremessage_pb.hxx.pb.h"
#include "promessage_pb.hxx.pb.h"
#include "sceneobj_manager.h"
#include "scenelogic.h"
#include "entity.h"
#include "template.h"
#include "property.h"
#include "propertymodule.h"
#include "sceneobj_define.h"
#include "mapmodule.h"
#include "message_maker.h"
#include "timer_define.h"
#include "pro_define.h"
#include "maptplmanager.h"
#include "scenecfg_manager.h"
#include "repetionmodule.h"
#include "mapobjmanager.h"
#include "gmmodule.h"
#include "logmodule.h"


// CGroupCDList ///////////////////////////////////////////////////////

// 刷新CD组时间
void CGroupCDList::RefreshCDTime()
{
	int nLeftSec = 0;
	int nLeftMillSec = 0;

	// cd组
	for( int i = 0; i < GetMaxGroupIdx(); i++ )
	{
		CGroupCD& rGroupCD = GetGroupCD( i );
		if( rGroupCD.GetTimerID() != INVALID_OBJ_ID )
		{
			int nGetTimerRet = CTimerMng::GetSingletonPtr()->GetLeftTime( rGroupCD.GetTimerID(), nLeftSec, nLeftMillSec );

			// cd组还没有超过24小时的
			if( nGetTimerRet == 0 )
			{
				rGroupCD.SetCDTime( nLeftMillSec );
			}

		}
	}

}


// CProperty的函数////////////////////////////////////////////////////////////////////////////////////
int CProperty::SetEntityType( EEntityType vType )
{
	mEntityType = vType;
	return 0;	
}

int CProperty::SetEntityID( unsigned int vEntityID )
{
	mEntityID = vEntityID;
	return mEntityID;
}

CProperty* CProperty::SearchProByEntityID( unsigned int vEntityID )
{
	CProperty* tpProperty = NULL;
	CEntity* tpEntity = ( CEntity* )CSceneObjManager::GetSingletonPtr()->GetObject( vEntityID );

	if( tpEntity == NULL )
	{
		return tpProperty;
	}

	tpProperty =tpEntity->GetProperty();

    return tpProperty;
}

CEntity* CProperty::GetSelfEntity( )
{
	return ( CEntity* )CSceneObjManager::GetSingletonPtr()->GetObject( GetEntityID( ) );
}

void CProperty::SetPropertyTempID( unsigned int vTempID )
{
	mTempID = vTempID;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//************************************
// Method:    AttachState
// FullName:  CPropertyCharacter::AttachState
// Access:    public 
// Returns:   void
// Qualifier: 添加状态,其中某些状态的增加需要通知客户端
// Parameter: EState vState
// Parameter: int vParames1
// Parameter: int vParames2
// Parameter: int vIndex
// Parameter: STATE_FROM vFrom
// Parameter: int vListID
// Parameter: CEntityResultList & rResultList
//************************************
void CPropertyCharacter::AttachState( EState vState, int vParames1, int vParames2, int vIndex, STATE_FROM vFrom )
{
    // 是否需要发送添加状态的消息
	bool tIsSend = false;
	// 这里变身的状态需要特殊处理,不论是否存在,直接替换
    if( mStateParam[ vState ].mInState == false || vState == STATE_TRANSFORM )
    {       
        // 如果状态不存在,则添加状态信息
		this->mStateParam[ vState ].mInState    = true;
        this->mStateParam[ vState ].mFrom       = vFrom;
        this->mStateParam[ vState ].mIndex      = vIndex;
        this->mStateParam[ vState ].mParames1   = vParames1;
        this->mStateParam[ vState ].mParames2   = vParames2;
		this->mStateParam[ vState ].mBuffAdd	= 1;
        tIsSend = true;

		LOG_DEBUG( "pro", "Entity %d AttachState (%d,%d)", GetEntityID(), vState, this->mStateParam[ vState ].mBuffAdd );
    }   
    else    
    {       
		// 如果状态存在,则状态计数器加1
		this->mStateParam[ vState ].mBuffAdd++;
		// 状态存在的处理
        switch( vState )
        {
		// 这里不需要BREAK,因为后面的处理都是相同的,只是下面2项需要通知客户端,其他项不需要通知客户端
		case STATE_UNCREAMSPEED: // 降低移动熟读(万分比)
		case STATE_INCREAMSPEED: // 增加移动速度(万分比)
			{
				tIsSend = true;
			}
		case STATE_CUREHPLESS: // 降低治疗量
		case STATE_DECALLDAMAGEPER: // 减少所有伤害的伤害百分比
		case STATE_PGENERATTACKFIX: // 增加指定系别攻击固定值
		case STATE_KILLERRENEW: // 杀怪取血
		case STATE_INCEXP: // 增加经验百分比
		case STATE_INCHONORSCALE:// 增加功勋获得百分比
		case STATE_INCOFFLINEEXP:
			{
				mStateParam[ vState ].mParames1 += vParames1;
			}
			break;
        default:
            break;
        }

		LOG_DEBUG( "pro", "Entity %d AttachState(already have) (%d,%d)", GetEntityID(), vState, this->mStateParam[ vState ].mBuffAdd );

    }
    if( tIsSend ==  true )
    {
		CEntity* tpEntity = SceneServer::CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
		if( tpEntity == NULL )
			return;

		switch( vState )
		{
		case STATE_UNCREAMSPEED:
		case STATE_INCREAMSPEED:
		case STATE_CANNTMOVE:
		case STATE_TRANSFORM:
		case STATE_UNATTACK_PLAYER:
		case STATE_PLAYER_UNATTACK:
		case STATE_INVISIBLE:
		case STATE_DISINVISIBLE:
		case STATE_FLY:
		case STATE_STONE:
		case STATE_MASKED:
			{
				// 上面几个状态需要广播,所以走Funcresult流程
				tpEntity->AddFunResult( 0, FUNC_CHANGESTATE, RESULT_STATEON, true, vState, vParames1, vParames2, tpEntity->GetPosX(), tpEntity->GetPosY() );
			}
			break;
		default:
			{
				// 发送状态改变消息
				// 发送删除当前状态的消息
				if( tpEntity->IsPlayer() )
				{
					/*SceneServer::CSceneLogic::GetSingletonPtr()->Send2Player( tpEntity, 
						BuildChangeStateNoticeMessage( GetEntityID( ), 
						vState, 
						mStateParam[ vState ].mParames1, 
						mStateParam[ vState ].mParames2, 
						true )
						);*/
					tpEntity->AddFunResult( 0, FUNC_CHANGESTATE, RESULT_STATEON, false, vState, vParames1, vParames2 );
					
					LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] Entity( %d ) STATE %d true Synchronization with Client", __LK_FILE__, __LINE__, __FUNCTION__, GetEntityID( ), vState );
				}

			}
			break;
		}			
    }
}

void CPropertyCharacter::DetachState( EState vState, int vIndex, STATE_FROM vFrom, int vParames1, int vParames2 )
{
    if( mStateParam[ vState ].mInState == true )
    {
		// 状态存在,则计数器减1
		mStateParam[ vState ].mBuffAdd--;

		bool tIsSend = false;
		if ( mStateParam[ vState ].mBuffAdd <= 0 )
		{
			LOG_DEBUG( "pro", "Entity %d DetachState( None ) (%d,%d)", GetEntityID(), vState, mStateParam[ vState ].mBuffAdd );

			if ( vState == STATE_STONE && vParames1 != 0 && vParames2 != 0 )
			{
				CEntity *tpEntity = this->GetSelfEntity( );
				if ( tpEntity->IsPlayer() )
				{
					CEntityPlayer *tpPlayer = (CEntityPlayer*)tpEntity;
					CWTPoint tPoint( tpPlayer->GetPosX(), tpPlayer->GetPosY() );
					int tDisTime = 30000;
					CTemplateNpc *tplNpc = (CTemplateNpc*)CDataStatic::SearchTpl( mStateParam[ vState ].mParames1 );
					if ( tplNpc != NULL )
					{
						tDisTime = tplNpc->mLifeTime * 1000;
					}
					CPropertyModule::GetSingleton().CreateNpc( tpPlayer, mStateParam[ vState ].mParames1, ENTITYTYPE_FUNCNPC, tPoint, tDisTime );
					CPropertyModule::GetSingleton().SendWarEventNotify( NOTICE_WAR_STONE_THROW, tpPlayer->GetMapID(), 
						tpPlayer->GetPosX(), tpPlayer->GetPosY(), tpPlayer->GetNationality(), 0, 0, mStateParam[ vState ].mParames1, tpPlayer->GetCharName() );
				}				
			}

			// 如果计数器为0,则删除状态
			mStateParam[ vState ].Clean();
			mStateParam[ vState ].mBuffAdd = 0;
			tIsSend = true;			
		}
		else
		{
			LOG_DEBUG( "pro", "Entity %d DetachState (%d,%d)", GetEntityID(), vState, mStateParam[ vState ].mBuffAdd );

			// 如果计数机不为0,则还有BUFF,作用当前状态
			switch ( vState )
			{
			case STATE_UNCREAMSPEED: // 降低移动熟读(万分比)
			case STATE_INCREAMSPEED: // 增加移动速度(万分比)
			case STATE_CUREHPLESS: // 降低治疗量
			case STATE_DECALLDAMAGEPER: // 减少所有伤害的伤害百分比
			case STATE_PGENERATTACKFIX: // 增加指定系别攻击固定值
			case STATE_KILLERRENEW: // 杀怪取血
			case STATE_INCEXP: // 增加经验百分比
			case STATE_INCHONORSCALE:// 增加功勋获得百分比
			case STATE_INCOFFLINEEXP:
				{
					CTemplateBuff* tpBullTpl = (CTemplateBuff*) CDataStatic::SearchTpl( vIndex );
					if ( tpBullTpl == NULL )
					{
						LOG_ERROR( "pro", "[ %s : %d ] [ %s ] DetachState error! StateType = %d, BuffID = %d.", __LK_FILE__, __LINE__, __FUNCTION__, vState, vIndex );
						return;
					}
					
					// 处理状态的叠加效果
					mStateParam[ vState ].mParames1 = std::max( (int)( mStateParam[ vState ].mParames1 - vParames1 ), 0 );
				}
				break;
			default:
				break;
			}
			
		}
		if ( tIsSend == true )
		{
			CEntity* tpEntity = SceneServer::CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
			if( tpEntity == NULL )
				return;
			switch( vState )
			{
			case STATE_UNCREAMSPEED:
			case STATE_INCREAMSPEED:
			case STATE_CANNTMOVE:
			case STATE_TRANSFORM:
			case STATE_UNATTACK_PLAYER:
			case STATE_PLAYER_UNATTACK:
			case STATE_INVISIBLE:
			case STATE_DISINVISIBLE:
			case STATE_FLY:
			case STATE_STONE:
			case STATE_MASKED:
				{
					// 需要广播的状态删除的消息
					tpEntity->AddFunResult( 0, FUNC_CHANGESTATE, RESULT_STATEOFF, true, vState, mStateParam[ vState ].mParames1, mStateParam[ vState ].mParames2, tpEntity->GetPosX(), tpEntity->GetPosY() );
				}
				break;
			default:
				{
					if( tpEntity->IsPlayer() )
					{
						// 发送删除当前状态的消息
						/*SceneServer::CSceneLogic::GetSingletonPtr()->Send2Player( tpEntity, 
							BuildChangeStateNoticeMessage( GetEntityID( ), 
								vState, 
								mStateParam[ vState ].mParames1, 
								mStateParam[ vState ].mParames2, 
								false )
							);*/
						tpEntity->AddFunResult( 0, FUNC_CHANGESTATE, RESULT_STATEOFF, false, vState, mStateParam[ vState ].mParames1, mStateParam[ vState ].mParames2 );
					
						LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] Entity( %d ) STATE %d false Synchronization with Client", __LK_FILE__, __LINE__, __FUNCTION__, GetEntityID( ), vState );
					}

				}
				break;
			}
		}		
    }
}

float CPropertyCharacter::GetHPPercent( )
{
	return ( mCurHP * 100.0f ) / ( int )GetMaxHP( );
}

float CPropertyCharacter::GetMPPercent( )
{
	return ( mCurMP * 100.0f ) / ( int )GetMaxMP( );
}

void CPropertyCharacter::CharacterInitial( )
{
	unsigned int i	= 0;

	mCurHP			= 0;
	mCurMP			= 0;
	mCurAP			= 0;
	mBattleTimer	= 0;
	mRefreshTimer	= REFRESHTIME;
	mLevel			= 0;
	mLastHPPer		= 0.0f;

	for( i = 0; i < ARRAY_CNT( mStateParam ); i++ )
    {
        mStateParam[ i ].Clean( );
    }	

	mBuffList.Initialize( );

	mDamageList.ClearDamage( );

	mCurSpeed = 0;
	mFirstEntityID = 0;
	mMaxDamager = 0;
	mPetID              = 0;

    mTotalTripIndex = 0;
    memset( mTripEntityIDList, 0, sizeof( mTripEntityIDList ) );
}

void CPropertyCharacter::CharacterFinal( )
{
	mDamageList.ClearDamage( );
	ClearAllTripEntity( );
	DestroyPetByOwnerDestroy( );
}

int CPropertyCharacter::AureoleTimeEvent( 	CEntityCharacter* pEntity, 
											unsigned int vTickOffSet
										)
{
	if( pEntity->GetEntityID() != GetEntityID( ) )
    {
        return -1;
    }

	if( mStateParam[ STATE_AUREOLE ].mInState == true )
    {
    	CTemplateAureoleSkill* tpSkill = ( CTemplateAureoleSkill* )CDataStatic::SearchTpl( mStateParam[ STATE_AUREOLE ].mParames1 );
        if( tpSkill == NULL )
        {
        	LOG_DEBUG( "pro", "[ %s : %d ][ %s ]Entity( %d ) Open Aureole( %d ) Cannt find", __LK_FILE__,
            	__LINE__, __FUNCTION__, pEntity->GetEntityID(), mStateParam[ STATE_AUREOLE ].mParames1);
			CPropertyModule::GetSingletonPtr()->CloseAureole( pEntity );
        }
        else
        {
            //find new player
			if( CPropertyModule::GetSingletonPtr()->mAureoleTimer.Elapse( vTickOffSet ) == true )
            {
            	CEntityCharacter* tpEntityList[ FUNC_LIMIT ] = { NULL };
				int tEntityCount = 0;

				// 以后光环由客户端上报
				//CMapModule::GetSingletonPtr()->GetRangeEntity(
				//	pEntity->GetLineID(),
				//	pEntity->GetMapID(),
				//	pEntity->GetMapIndex(),
				//	CTemplateNormalSkill::RANGE_SELFCIRCLE,
				//	tpSkill->mAureoleDistance,
				//	0,
				//	pEntity,
				//	tpEntityList,
				//	&tEntityCount
				//	);

                for( int i = 0; i < tEntityCount; i++ )
                {
                	for( int j = 0; j < (int)ARRAY_CNT( tpSkill->mAureoleType ); j++)
                    {
                    	if( tpSkill->mAureoleType[ j ].mAureoleArea == CTemplateAureoleSkill::AUREOLE_NONE )
                        {
                        	continue;
                        }
                        CTemplateBuff* tpAureoleBuff = ( CTemplateBuff* )CDataStatic::SearchTpl( tpSkill->mAureoleType[ j ].mBuffID );
                        if( tpAureoleBuff == NULL )
                        {
                        	LOG_DEBUG( "pro", "[ %s : %d ][ %s ]AureoleSkill( %d ) Buff( %d ) Cann't find from "
                            	"Template", __LK_FILE__, __LINE__, __FUNCTION__, tpSkill->mTempID,
                                tpSkill->mAureoleType[ j ].mBuffID );
                            continue;
                        }
                        if( tpEntityList[ i ] == NULL )
                        {
                        	LOG_DEBUG( "pro", "[ %s : %d ][ %s ]Entity( %d ) UseSkill( %d : %d ), Call "
                            	"Map_GetRangeEntity tResult count error index %d is NULL", __LK_FILE__, __LINE__,
                                __FUNCTION__, pEntity->GetEntityID( ), tpSkill->mSkillID, tpSkill->mSkillLevel, i );
                            continue;
                        }

                        if( pEntity->GetEntityID( ) == tpEntityList[ i ]->GetEntityID( ) )
                        {
                        	continue;
                        }
						CPropertyCharacter* tpDesProperty = ( CPropertyCharacter* )tpEntityList[ i ]->GetProperty( );

						if( tpDesProperty == NULL )
                        {
                        	LOG_DEBUG( "pro", "[ %s : %d ][ %s ] Entity( %d ) UseSkill %d Level %d, Cannt find"
                            	"Entity( %d ) property", __LK_FILE__, __LINE__, __FUNCTION__,
                                pEntity->GetEntityID( ), tpSkill->mSkillID, tpSkill->mSkillLevel,
                                tpEntityList[ i ]->GetEntityID( ) );
                            continue;
                        }
						if( tpDesProperty->GetEntityType( ) != ENTITYTYPE_PLAYER
							&& tpDesProperty->GetEntityType( ) != ENTITYTYPE_PET
							&& tpDesProperty->GetEntityType( ) != ENTITYTYPE_FUNCNPC
							&& tpDesProperty->GetEntityType( ) != ENTITYTYPE_OGRE
						)
						{
							LOG_DEBUG( "pro", "[ %s : %d ][ %s ]Entity( %d ) is not character, type is %d", __LK_FILE__,
								__LINE__, __FUNCTION__, tpDesProperty->GetEntityID( ), tpDesProperty->GetEntityType( ));
							continue;
						}

                        switch( tpSkill->mAureoleType[ j ].mAureoleArea )
                        {
                        	case CTemplateAureoleSkill::AUREOLE_TEAM:
                            	{
                                }
                                break;
                            case CTemplateAureoleSkill::AUREOLE_FOE:
                                {

                                }
                                break;
                            case CTemplateAureoleSkill::AUREOLE_CORPS:
                                {
                                }
                                break;
                            case CTemplateAureoleSkill::AUREOLE_ALL:
                                {
                                	bool tIsFound = false;
									CBuffList *tpBuff = tpDesProperty->GetBuffList( );
                                    for( unsigned int k = 0; k < tpBuff->mBuffNumber; k++ )
                                    {
                                    	if( (int)tpBuff->mBuffList[ k ].mBuffID == tpAureoleBuff->mTempID )
                                        {
                                        	tIsFound = true;
                                            break;
                                        }
                                    }

                                    if( tIsFound == false )
                                    {
										tpEntityList[ i ]->EntityInsertBuff( pEntity, tpAureoleBuff );
										LogEventGetBuffByPlayer((CEntityPlayer*)tpEntityList[ i ],tpAureoleBuff->mTempID,tpAureoleBuff->mLastTime,
											tpSkill->mSkillID,((CEntityPlayer*)pEntity)->GetCharID() ) ;
                                    }
                                 }
                                 break;
							default:
								break;
                         }

                    }
				}
			}
			mStateParam[ STATE_AUREOLE ].mParames2 = std::max( ( int )( mStateParam[ STATE_AUREOLE ].mParames2 - vTickOffSet ), 0 );
			if( mStateParam[ STATE_AUREOLE ].mParames2 <= 0 )
			{
            	bool tIncessaryNeed = true;
                for ( int i = 0; i < (int)ARRAY_CNT( tpSkill->mCost ); i ++ )
                {
                	if ( tpSkill->mCost[ i ].mCostType == (int)CTemplateSkill::COST_HP 
						&& (int)mCurHP < tpSkill->mCost[ i ].mCostParam[ 0 ] )
                    {
                    	tIncessaryNeed = false;
                        break;
                    }

                    if( tpSkill->mCost[ i ].mCostType == (int)CTemplateSkill::COST_MP
                                && (int)mCurMP < tpSkill->mCost[ i ].mCostParam[ 0 ] )
                    {
                    	tIncessaryNeed = false;
                        break;
                    }

                    if ( GetEntityType( ) == ENTITYTYPE_PLAYER
                                && tpSkill->mCost[ i ].mCostType == CTemplateSkill::COST_ITEM )
                    {
						CPropertyPlayer* tpProPlayer = ( CPropertyPlayer* )this;
                    	if ( tpProPlayer->GetBaggage( )->HasItem( tpSkill->mCost[ i ].mCostParam[ 0 ],
                                    tpSkill->mCost[ i ].mCostParam[ 1 ] ) == false )
                        {
                        	tIncessaryNeed = false;
                            break;
                        }
                    }
                }
                        

                if( tIncessaryNeed == false )
                {
					CPropertyModule::GetSingletonPtr()->CloseAureole( pEntity );
                }
                else
                {
                	for ( int i = 0; i < (int)ARRAY_CNT( tpSkill->mCost ); i ++ )
                    {
                    	if ( tpSkill->mCost[ i ].mCostType == CTemplateSkill::COST_MP )
                    	{
                    		CPropertyValue tValue = CPropertyFormula::GetSkillLess( this, tpSkill->mGenerID );
                    		int tDecMp = std::max( 0, (int)( ( tpSkill->mCost[ i ].mCostParam[ 0 ]
                    			- tValue.mInitValue ) * ( SERVER_PERCENT_FLOAT - tValue.mDeltaValue ) / SERVER_PERCENT_FLOAT ) );
                    		mCurMP -= tDecMp;

							pEntity->AddFunResult( GetEntityID( ), FUNC_DECMP,RESULT_COST, true, tDecMp, mCurMP, GetMaxMP( ) );
                        }

                        if ( tpSkill->mCost[ i ].mCostType == CTemplateSkill::COST_HP )
                        {
                        	mCurHP -= tpSkill->mCost[ i ].mCostParam[ 0 ];

							pEntity->AddFunResult( GetEntityID( ), FUNC_DECHP,RESULT_COST, true, tpSkill->mCost[ i ].mCostParam[ 0 ], mCurHP, GetMaxHP( ) );
                        }

                        if ( pEntity->GetEntityType() == ENTITYTYPE_PLAYER
                        	&& tpSkill->mCost[ i ].mCostType == CTemplateSkill::COST_ITEM )
                        {
							uint64_t ullGUID = 0;
							bool tBind = false;
							if ( 	CPropertyModule::GetSingletonPtr()->PlayerRemoveItemByID( 
										( CEntityPlayer* )pEntity,
										tpSkill->mCost[ i ].mCostParam[ 0 ], 
										tpSkill->mCost[ i ].mCostParam[ 1 ], tBind, &ullGUID ) == 0 )
							{
								LogEventLostItemByUse ( ( CEntityPlayer* )pEntity, 
										ullGUID, tpSkill->mCost[ i ].mCostParam[ 0 ], tpSkill->mCost[ i ].mCostParam[ 1 ]);
							}
                        }

                    }

                	mStateParam[ STATE_AUREOLE ].mParames2 = tpSkill->mIncessaryTime;
                }
			}
        }
	}

	return 0;
}


// ***************************************************************
//  Function:		BuffTimeEvent
//  Description:	处理实体身上的buff作用
//	Input:			
//	OutPut:			
//	Return:			
//	Others:
//  Date:			02/17/2009
// 
// ***************************************************************

//int CPropertyCharacter::BuffTimeEvent( CTemplateBuff* tpBuffTpl, CBuffObject* tpBuff )
//{
//	for ( int j = 0; j < (int)ARRAY_CNT( tpBuffTpl->mBuff ) && this->IsAlive( ) == true; j ++ )
//	{
//		if ( tpBuffTpl->mBuff[ j ].mBuffFunc == 0 )
//			break;
//
//		switch( tpBuffTpl->mBuff[ j ].mBuffFunc )
//		{
//			// 持续回血
//		case CTemplateBuff::BUFFFUNC_PERSISTINCFIXHP:
//			{
//				float tParam1 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mHotparam1 / SERVER_PERCENT_FLOAT;
//				float tParam2 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mHotparam2 / SERVER_PERCENT_FLOAT;
//				int tCure = tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] + tParam1 * pow( tParam2, tpBuffTpl->mSkillLevel ) * tpBuff->mAttack / ( tpBuffTpl->mLastTime / 1000 );
//				CureHP( tCure );
//			}
//			break;
//			// 持续回蓝
//		case CTemplateBuff::BUFFFUNC_PERSISTINCFIXMP:
//			{
//				CureMP( tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] );
//			}
//			break;
//			// 持续伤害
//		case CTemplateBuff::BUFFFUNC_PERSISTDECFIXHP:
//			{
//				float tParam1 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDotparam1 / SERVER_PERCENT_FLOAT;
//				float tParam2 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDotparam2 / SERVER_PERCENT_FLOAT;
//				int tDamage = tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] + tParam1 * pow( tParam2, tpBuffTpl->mSkillLevel ) * tpBuff->mAttack / ( tpBuffTpl->mLastTime / 1000 );
//				
//				CEntityCharacter* pBuffSrcEntity = (CEntityCharacter*)CSceneLogic::GetSingletonPtr()->GetEntity( tpBuff->GetBuffSrcEntityID() );
//				
//				PERF_FUNC( "EntityDamage", CPropertyModule::GetSingletonPtr()->EntityDamaged( pBuffSrcEntity, pEntity, tDamage, RESULT_NORMAL ) );
//			}
//			break;
//			// 持续减蓝
//		case CTemplateBuff::BUFFFUNC_PERSISTDECFIXMP:
//			{
//				ExpendMP( tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ], ( ( tpBuffEntity == NULL ) ? 0 : tpBuffEntity->GetEntityID( ) ) );
//
//				// 如果蓝耗光了,就删除BUFF,需要是正面BUFF才能删除
//				if ( (int)mCurMP < tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] && tpBuffTpl->mBuffType == CTemplateBuff::BUFF_INC )
//					tpBuff->mPersistTime = 0;
//			}
//			break;
//			// 离开中debuff时所处范围会受到特殊效果
//		case CTemplateBuff::BUFFFUNC_LEAVEAREA:
//			{
//				CWTPoint tPoint( tpBuff->mParames[ j ] >> 16, tpBuff->mParames[ j ] & 0x0000FFFF );
//				if( GetPos( ).LimitDistance( tPoint, tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] ) == false )
//				{
//					LOG_DEBUG( "pro", "[ %s : %d ][ %s ]Entity( %d ) Buff LeveEvent, InsertBuff Pos( %d"
//						" : %d ), Now Pos(%d : %d ), Distance = %d, BuffPersistTime=%d", __LK_FILE__,
//						__LINE__, __FUNCTION__, GetEntityID(), tPoint.mX, tPoint.mY, GetPosX( ),
//						GetPosY( ), tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ], tpBuff->mPersistTime );
//
//					CTemplateBuff* tpBuffTemplate = (CTemplateBuff*)CDataStatic::SearchTpl( 
//						tpBuffTpl->mBuff[ j ].mBuffParam[ 1 ] );
//
//					if( tpBuffTemplate != NULL )
//					{
//						EntityInsertBuff( tpBuffEntity, tpBuffTemplate );
//					}
//					tpBuff->mPersistTime = 0;
//				}
//				else
//				{
//					tpBuff->mLastInterval = 0;
//				}
//			}
//			break;
//			// 持续按MaxHP百分比恢复CurHP
//		case CTemplateBuff::BUFFFUNC_PERSISTINCPERHP:
//			{
//				// 只有character才有HP
//				CPropertyCharacter* tpProperty = (CPropertyCharacter*) GetProperty( );
//				int tIncHP = (int) ( ( (int)tpProperty->GetMaxHP( ) ) * tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] / SERVER_PERCENT_FLOAT );
//				CureHP( tIncHP );
//
//			}
//			break;
//			// 持续按MaxMP百分比恢复CurMP
//		case CTemplateBuff::BUFFFUNC_PERSISTINCPERMP:
//			{
//				// 只有character才有HP
//				CPropertyCharacter* tpProperty = (CPropertyCharacter*) GetProperty( );
//				int tIncMP = (int) ( ( (int)tpProperty->GetMaxMP( ) ) * tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] / SERVER_PERCENT_FLOAT );
//				CureMP( tIncMP );
//			}
//			break;
//		default:
//			break;
//		}
//	}
//
//	return 0;
//}

int CPropertyCharacter::BattleTimeEvent(	CEntityCharacter* pEntity, 
											unsigned int vTickOffSet
										)
{
    if( pEntity->GetEntityID() != GetEntityID( ) )
    {
        return -1;
    }

    if ( pEntity->GetAliveStatus( ) == ALIVE_BATTLE )
    {
    	mBattleTimer -= vTickOffSet;
        if ( mBattleTimer <= 0 )
        {
        	mBattleTimer    = 0;
			pEntity->SetAliveStatus( ALIVE_NORMAL );

			pEntity->AddFunResult( 0, FUNC_PEACE, RESULT_NORMAL );
        }
    }

	return 0;
}

int CPropertyCharacter::RecTimeEvent(	CEntityCharacter* pEntity, 
										unsigned int vTickOffSet
									)
{
    LK_ASSERT( pEntity->GetEntityID() == GetEntityID( ), return -1 );


	if ( mRefreshTimer > 0 )
	{
		mRefreshTimer -= vTickOffSet;
		if ( mRefreshTimer <= 0 )
		{
			unsigned int tLastHP      = mCurHP;
			unsigned int tLastMP      = mCurMP;
			unsigned short tHPRSpeed = 0;
			unsigned short tMPRSpeed = 0;


			if ( pEntity->IsAlive() == true )
			{
				tHPRSpeed = CPropertyFormula::GetNormalHPRSpeed( this );
				mCurHP = std::min( (unsigned int)( mCurHP + tHPRSpeed ), (unsigned int )GetMaxHP( ) );

				tMPRSpeed = CPropertyFormula::GetNormalMPRSpeed( this );
				mCurMP = std::min( (unsigned int)( mCurMP + tMPRSpeed ), (unsigned int )GetMaxMP( ) );
			}
			
			if ( mCurHP != tLastHP )
			{
				pEntity->AddFunResult( GetEntityID( ), FUNC_INCHP, RESULT_NORMALREFRESH, true, tHPRSpeed, mCurHP, GetMaxHP( ) );
			}
			if( mCurMP != tLastMP )
			{
				pEntity->AddFunResult( GetEntityID( ), FUNC_INCMP, RESULT_NORMALREFRESH, true, tMPRSpeed, mCurMP, GetMaxMP( ) );
			}

			mRefreshTimer = REFRESHTIME;
		}	
	}

	return 0;
}


// ***************************************************************
//  Function:		LogicTimeEvent
//  Description:	处理玩家的打开窗口，生产，升级等时间的终止
//	Input:			
//	OutPut:			
//	Return:			
//	Others:			该函数的实时性要求不高
//  Date:			01/15/2009
// 
// ***************************************************************
int CPropertyPlayer::LogicTimeEvent(	CEntity* pEntity, 
										unsigned int vTickOffSet
									)
{
    LK_ASSERT( pEntity->GetEntityID() == GetEntityID( ), return -1 );

	//如果玩家处于对话状态
    if ( pEntity->IsAlive() == true && mTalkNpcEntityID != 0 )
    {
    	CEntity* tpDesEntity = CSceneLogic::GetSingletonPtr( )->GetEntity( mTalkNpcEntityID );
        if ( tpDesEntity == NULL )
        {
			LOG_DEBUG( "pro", "[ %s : %d ][ %s ] Entity( %d ) Talk NPC Entity( %d ) Is Null", __LK_FILE__, __LINE__,
				__FUNCTION__, GetEntityID( ), mTalkNpcEntityID );
        	mTalkNpcEntityID = 0;
        }
		//else if ( !pEntity->GetPos( ).LimitDistance( tpDesEntity->GetPos( ), 15 ) )
		else if( pEntity->IsWithinDistance( tpDesEntity, 15 ) == false )
		{
			CPropertyModule::GetSingletonPtr()->PlayerCloseDialog( ( CEntityPlayer* )pEntity );
            mTalkNpcEntityID    = 0;
            mServiceStatus      = SERVICE_NONE;

			// TODO: 客户端自己判断，不用给他发了
			CPropertyModule::GetSingletonPtr()->SendCloseDialogNotice( pEntity );
		}
	}	

	// 如果实体处于生产状态
	/*if ( pEntity->IsAlive() == true && mProduce.mNumber > 0 )
    {
    	mProduce.mTimer = std::max( (int)( mProduce.mTimer - vTickOffSet ), 0 );

        if ( mProduce.mTimer <= ( mProduce.mNumber - 1 ) * 1500 )
        {
			CPropertyModule::GetSingletonPtr()->PlayerProduceEquip( ( CEntityPlayer* )pEntity, mProduce.mType1,
				mProduce.mType2, mProduce.mType3, mProduce.mLevel, mProduce.mRank );
        }

    	if ( mProduce.mTimer == 0 )
        	mProduce.Clear( );
	}*/
	return 0;
}


int CPropertyPlayer::PKTimeEvent(	CEntity* pEntity, 
									unsigned int vTickOffSet
								)
{
	if ( pEntity->IsPlayer() )
	{
		this->IncPrisonTime( vTickOffSet );
	}
	return 0;
}

int CPropertyPlayer::Initial( )
{
	mExp = 0;
	mSex = 0;
	mStorageStatus         = STORAGE_STATUS_INVALID;
	
	//mBaggage.mReleaseIndex  = BAGGAGE_BASE;
	//mStorage.mReleaseIndex =  BAGGAGE_BASE;
	//mTaskBaggage.mReleaseIndex = BAGGAGE_BASE;
	//mBaggage.mKitBagIndex = MAX_BAGGAGEKITBAG_NUM;
	//mStorage.mKitBagIndex = MAX_KITBAG_NUM;

	mEquipment.Initialize();
	mBaggage.Initialize();
	mTaskBaggage.Initialize();
	
    mTalkNpcEntityID =  INVALID_OBJ_ID;
    mServiceStatus = SERVICE_NONE;
    mMoney = 0;
	mBindMoney = 0;
	mStorageMoney = 0;
	mStorageBindMoney = 0;
	mBattleTimer = 0;
    mCommonCD =  0;
   
    mCurStr                = 0;    
    mCurSpr                = 0;   
    mCurCon                = 0;    
    mCurSta                = 0;   
    mCurWis                = 0;
    mRemainPoint        = 0;
    mSkillPoint         = 0;

   // mPhysicID           = 0;
   /* mRecoverTimer       = 0;*/
    memset( &mProduce, 0, sizeof( CProduce ) );
	mCDGroup.Initialize();

    mProLevel			= 1;
	mProExp				= 0;

	CharacterInitial( );	
	mEquipSkill.Initialize( );
    mItemForFreeList.initailize();

	mHonor = 0;
	mLastObjID = 0;
	mInHonPerDay = 0;
	mOutHonPerDay = 0;
	memset( mKill, 0, sizeof( mKill ) );

	mDiploidTime = 0;
	mValidDiploid = 0;
	mKillNumOut = 0;
	mKillNumIn = 0;
	mHasRedStoneStatus = 0;	

	mDailyWelfareStatus = 0;

	mLastRefreshTime = 0; 

	mRedStoneID = 0;
	mAnswerStatusObjID = 0;

	mCompletedTasks = 0;	
	mDieNum	= 0;			


	mSaleItem.initailize();
	for ( unsigned int i = 0; i < ARRAY_CNT( mDiploidState ); i ++ )
	{
		mDiploidState[ i ] = 0; 
	}

	mResetPPNum = 0;
	mResetSPNum = 0;
	mLastResetPPIndex = 0;
	mLastResetSPIndex = 0;
	mLastFamilyApplyTime = 0;
	mLastTimeLeaveFamily = 0;

	mPKValue		= 0;
	mPKStatus		= PKSTATUS_WHITE;
	mYellowStatus	= false;
	mYellowTimer	= 0;
	mPrisonTime		= 0;
	mPrisonTotalTime = 0;
	mReduceLevel = 0;
	mAttackStateTime = 0;
	mDefenceStateTime = 0;
	mDeathStateTime = 0;
	mReduceHPStateTime = 0;
	mActiveCardList.initailize();
	mOwnCardList.initailize();
	mLastWeeklyDisposTime = 0;
	mLastWeeklyDisposTime = 0;
	mAssignExpScl = 0;
	mTalentExp = 0;
	mTalentPoint = 0;
	mTalentSkill.Initialize( );
	mTalentStep = 0;
	mTalentStudy = 0;
	mTalentLevel = 0;
	mCurrentTitle = 0;
	mTitleSkill.Initialize();

	mLastRefreshApTime = 0;

	mInAutoPlay = false;
	//mCDVector.initailize();

	mOriginReliveTimes	=	0;	
	mKillSameNationNum	=	0;	
	mKillOgreNum	=	0;		
	mUpFailNum	=	0;			
	mTodayKillEnemyNumIn = 0;
	mTodayKillEnemyNumOut= 0;
	mDecompoundEquipIndex = -1;

	mWizard.Initial( );

	mLastJudgeTime = 0;
	mTitleList.initailize();
	mTeleList.initailize();

	mVipFlag = EM_VIP_NONE;
	mVipEndDate = 0;
	mAwardList[EALT_Daily].initailize();
	mAwardList[EALT_Weekly].initailize();
	memset( mMasterID, 0, sizeof(mMasterID) );
	mMatrixList.initailize();
	memset( mSlaveValue, 0, sizeof(mSlaveValue) );
	mSlavePoint = 0;
	mMasterPoint = 0;
	mSlaveTotal = 0;
	mMasterTotal = 0;
	memset( mMasterPercent, 0, sizeof(mMasterPercent) );
	mBeginnerNum = 0;
	mFormalNum = 0;
	mMasterTimes = 0;
	mMasterGive = 0;
	mCharmValue = 0;
	mProgressFunc = -1;
	mProgressStartTime = 0;
	mInvestStatus = 0;
	mTotalWar = 0;
	mTodayWar = 0;
	mTodayOther = 0;
	mFreeWar = 0;
	mWarKill = 0;
	mWarSerial = 0;
	mWeekIncCharm= 0;
	mLastWeekIncCharm=0;
	mTodayHonor = 0;
	mExpHonor = 0;
	mItemHonor = 0;
	mHasVoted = 0;
	return 0;
}

int CPropertyPlayer::Final( )
{
	CharacterFinal( );
	//mEquipSkill.Final( );
	mEquipment.Clear();		//装备
	mBaggage.Clear();		//包裹
	mTaskBaggage.Clear();	//任务道具包裹
	mStorage.Clear();		//仓库
	return 0;
}

int CPropertyCharacter::Initial()
{
	return 0;
}

int CPropertyCharacter::Final()
{
	return 0;
}

bool CPropertyCharacter::NoteAddTripEntityID( int vTripEntityID )
{
    int i = 0;
    int tIndex = 0;

    if ( vTripEntityID == 0 )
    {
        LOG_ERROR( "pro", "[ %s : %d ][ %s ]: NoteAddTripEntityID() : TripEntityID error ( 0 )", __LK_FILE__, __LINE__,
            __FUNCTION__ );
        return false;
    }
    
    mTotalTripIndex++;
    // ê×?è?°?ò????
    for ( i = 0; i < (int)ARRAY_CNT( mTripEntityIDList ); i++ )
    {
        if ( mTripEntityIDList[ i ].mTripEntityID == 0 )
        {
            mTripEntityIDList[ i ].mTripEntityID = vTripEntityID;
            mTripEntityIDList[ i ].mTripIndex = mTotalTripIndex;
            return true;
        }
    }

    // ??óD????,é?3yμúò???ìí?óμ??Yú?,ê×?è?ò3?×???·???μ??Yú?μ?INDEX
    for ( i = 1; i < (int)ARRAY_CNT( mTripEntityIDList ); i++ )
    { 
        if ( mTripEntityIDList[ i ].mTripIndex < mTripEntityIDList[ tIndex ].mTripIndex )
        {
            tIndex = i;
            continue;
        }
    }

    if ( tIndex < 0 || tIndex >= (int)ARRAY_CNT( mTripEntityIDList )  )
    {
        LOG_ERROR( "pro", "[ %s : %d ][ %s ] : NoteAddTripEntityID() : tIndex ( %d ) error\n", __LK_FILE__, __LINE__,
            __FUNCTION__, tIndex );
        return false;
    }

    // ì????Yú?
    ClearAddTripEntityID( mTripEntityIDList[ tIndex ].mTripEntityID );
    mTripEntityIDList[ tIndex ].mTripEntityID = vTripEntityID;
    mTripEntityIDList[ tIndex ].mTripIndex = mTotalTripIndex;

	return true;
}

unsigned short CPropertyPlayer::GetIncSkillLevel( unsigned short vSkillID )
{       
    return mEquipment.GetIncSkillLevel( vSkillID );
}       
  

// ***************************************************************
//  Function:		OnSkillCooldownTimeOut
//  Description:	处理技能CD事件, 没有公共CD,只有冷却组
//	Input:			
//	OutPut:			
//	Return:			
//	Others:			
//  Date:			12/29/2008
// 
// ***************************************************************
bool CPropertyPlayer::OnSkillCooldownTimeOut( CEntityPlayer* pEntity, int nSkillID, int nSkillLevel, int nSkillCD /*= 0*/ , int nMWObjID/* = 0*/ )
{
	LK_ASSERT( pEntity != NULL, return false );
	
	//// 公共CD
	//if( nSkillID == 0 )
	//{
	//	mCommonCD = nSkillCD;
	//	if( mCommonCD <= 0 )
	//	{
	//		CSceneLogic::GetSingletonPtr()->Send2Player( pEntity, BuildFuncResultMessage( pEntity, FUNC_CMCOOLDOWN, RESULT_NORMAL, 0, 0, 0 ) );
	//		mCommonCD = 0;
	//		CSkillObject::SetCommonCDTimer( INVALID_OBJ_ID );
	//		//LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] common skill cd timeout", __LK_FILE__, __LINE__, __FUNCTION__ );
	//		return true;
	//	}
	//}
	//else
	//{

	CSkillObject* pSkillObj = NULL;
	if ( nMWObjID != INVALID_OBJ_ID )
	{
		CItemMagicWeapon* tpMW = (CItemMagicWeapon*)CSceneObjManager::GetSingletonPtr()->GetObject( nMWObjID );
		if ( tpMW == NULL )
		{
			LOG_ERROR("pro", "can't find MW? MWid=%d owner player=%d(%s) skillid=%d nskillcd=%d",
				nMWObjID, pEntity->GetEntityID(), pEntity->GetCharNameShow(), nSkillID, nSkillCD );
			return true;
		}
		else
		{
			pSkillObj = tpMW->GetMWSkillList()->GetSkillObjectByID( nSkillID );
		}
	}
	else
	{
		pSkillObj = GetSkillObjectByID( nSkillID );
	}

	if( pSkillObj != NULL )
	{
		LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] player %s Skill %d timeout 1", __LK_FILE__, __LINE__, __FUNCTION__, 
			pEntity->GetCharNameShow(), nSkillID );

		pSkillObj->SetCooldown( nSkillCD );

		if( pSkillObj->GetCooldown() <= 0 )
		{
			pSkillObj->SetCooldown( 0 );
			pSkillObj->SetCDTimer( INVALID_OBJ_ID );
			CSceneLogic::GetSingletonPtr()->Send2Player( pEntity, BuildFuncResultMessage( pEntity, FUNC_COOLDOWN, RESULT_NORMAL, 0, nSkillID, nSkillLevel ) );
			return true;
		}

	}
	else
	{
		//TODO: 找不到技能的话把定时器删除
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] player %s can't find skill %d", __LK_FILE__, __LINE__, __FUNCTION__,
			pEntity->GetCharNameShow(), nSkillID );

#ifdef _DEBUG_
		print_trace_fd( "pro" );
#endif

		return true;
	}

	//}

	return false;
}



bool CPropertyCharacter::ClearAddTripEntityID( int vTripEntityID )
{
    unsigned int i = 0;

    if ( vTripEntityID == 0 )
    {
        LOG_ERROR( "pro", "[ %s : %d ][ %s ] : ClearAddTripEntityID() : TripEntityID error ( 0 )", __LK_FILE__,
            __LINE__, __FUNCTION__ );
        return false;
    }

    for ( i = 0; i < ARRAY_CNT( mTripEntityIDList ); i++ )
    {
        if ( mTripEntityIDList[i].mTripEntityID == vTripEntityID )
        {
            CEntity* tpTripEntity = CSceneLogic::GetSingleton( ).GetEntity( vTripEntityID );
            if ( tpTripEntity != NULL )
            {
                CSceneLogic::GetSingleton( ).DestroyEntity( tpTripEntity, 0 );
            }
            mTripEntityIDList[i].Clean();
            return true;
        }
    }
    return false;
}

void CPropertyCharacter::ClearAllTripEntity( )
{
    unsigned int i = 0;

    for ( i = 0; i < sizeof( mTripEntityIDList ) / sizeof( mTripEntityIDList[ 0 ] ); i++ )
    {
        if ( mTripEntityIDList[i].mTripEntityID != 0 )
        {
            CEntity* tpTripEntity = CSceneLogic::GetSingleton( ).GetEntity( mTripEntityIDList[i].mTripEntityID );
            if ( tpTripEntity != NULL )
            {
                CSceneLogic::GetSingleton( ).DestroyEntity( tpTripEntity, 0 );
            }

            mTripEntityIDList[i].Clean();
        }
    }
}

bool CPropertyCharacter::Controlable( )
{
	return mStateParam[ STATE_CONFUSION ].mInState == false;
}

bool CPropertyCharacter::Moveable( )
{
	return mStateParam[ STATE_CANNTMOVE ].mInState == false;
}

// 是否可以攻击
bool CPropertyCharacter::Attackable( )
{
	return ( mStateParam[ STATE_UNATTACK ].mInState == false && mStateParam[ STATE_SILENCE ].mInState == false );
}



bool CPropertyPlayer::TestBaggageSpace( int* pItemID, int* pNumber, int vNumber )
{
    int tItemID[ BAGGAGE_BASE ]           = { 0 };
    int tTaskItemID[ BAGGAGE_BASE ]       = { 0 };
    int tItemNumber[ BAGGAGE_BASE ]       = { 0 };
    int tTaskItemNumber[ BAGGAGE_BASE ]   = { 0 };
    int tItemIndex = 0;
    int tTaskItemIndex = 0;
    for ( int i = 0; i < vNumber; i ++ )
    {
        if ( pItemID[ i ] == 0 )
            break;
        
        CTplItem* tpItemTpl = (CTplItem*) CDataStatic::SearchTpl( pItemID[ i ] );
        if ( tpItemTpl == NULL )
            break;
            
        if ( IS_TASK_ITEM( tpItemTpl ) )
        {
            tTaskItemID[ tTaskItemIndex ]       = pItemID[ i ];
            tTaskItemNumber[ tTaskItemIndex ]   = pNumber[ i ];
            tTaskItemIndex ++;
        }
        else
        {
            tItemID[ tItemIndex ]               = pItemID[ i ];
            tItemNumber[ tItemIndex ]           = pNumber[ i ];
            tItemIndex ++;
        }
    }

    return ( ( mBaggage.Space( tItemID, tItemNumber, BAGGAGE_BASE ) == true )
				&& ( mTaskBaggage.Space( tTaskItemID, tTaskItemNumber, BAGGAGE_BASE ) == true ) );
}

bool CPropertyPlayer::BaggageHasItem( unsigned int vItemID, unsigned int vNumber )
{
    CTplItem* tpItemTpl = (CTplItem*) CDataStatic::SearchTpl( vItemID );
	if( tpItemTpl == NULL )
	{
		LOG_ERROR( "pro", "[%s:%s:%d] can't find item %d in tmplate", __FILE__, __FUNCTION__, __LINE__, vItemID  );
		return false;
	}

    if ( IS_TASK_ITEM( tpItemTpl ) )
        return mTaskBaggage.HasItem( vItemID, vNumber );

    return mBaggage.HasItem( vItemID, vNumber );
}

int CPropertyPlayer::BaggageHasItem( unsigned int vItemID )
{
    CTplItem* tpItemTpl = (CTplItem*) CDataStatic::SearchTpl( vItemID );

    if (tpItemTpl == NULL) return 0;
        
    if ( IS_TASK_ITEM( tpItemTpl ) )
        return mTaskBaggage.HasItem( vItemID );
    
    return mBaggage.HasItem( vItemID );
}

CSkillObject* CPropertyPlayer::GetSkillObjectByID( unsigned int vSkillID )
{
    CSkillObject* tpSkill = mSkill.GetSkillObjectByID( vSkillID );

    if ( tpSkill == NULL )
	{
        tpSkill = mEquipSkill.GetSkillObjectByID( vSkillID );
		if ( tpSkill == NULL )
		{
			// 如果还没找到 去法宝里找
			CItemMagicWeapon* tpMW = (CItemMagicWeapon*)GetEquipment()->GetItemObjPtr(EQUIPMENT_MAGICWEAPON);
			if ( tpMW != NULL )
				return tpMW->GetMWSkillList()->GetSkillObjectByID(vSkillID);
		}
	}

    return tpSkill;
}


//************************************
// Method:    GetSkillObjectByIndex
// FullName:  CPropertyPlayer::GetSkillObjectByIndex
// Access:    public 
// Returns:   CSkillObject*
// Qualifier: 根据技能索引从技能列表中（不包括装备提供的技能）找出技能对象
// Parameter: unsigned int unSkillIndex
//************************************
CSkillObject* CPropertyPlayer::GetSkillObjectByIndex( unsigned int unSkillIndex )
{
	return mSkill.GetSkillObjectByIndex( unSkillIndex );
}

// 从法宝中根据技能框ID获得技能对象
CSkillObject* CPropertyPlayer::GetSkillObjectBySkillIDInMagicWeapon( int vSkillID )
{
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)mEquipment.GetItemObjPtr( EQUIPMENT_MAGICWEAPON);
	if ( tpMW == NULL )
		return NULL;

	return tpMW->GetMWSkillList()->GetSkillObjectByID(vSkillID);
}


int CPropertyPlayer::MoveItem( unsigned int vSrcIndex, unsigned int vSrcSlot, unsigned int vDesIndex, unsigned int vDesSlot, unsigned int vNumber, int &rNum )
{
    if ( vSrcSlot != SLOT_BAGGAGE && vSrcSlot != SLOT_STORAGE && vSrcSlot != SLOT_STORKITBAG && vSrcSlot != SLOT_BAGGAGEKITBAG )
        return ERROR_MOVEITEM_SRCINVALID;

    if ( vDesSlot != SLOT_BAGGAGE && vDesSlot != SLOT_STORAGE && vDesSlot != SLOT_STORKITBAG && vDesSlot != SLOT_BAGGAGEKITBAG )
        return ERROR_MOVEITEM_DESINVALID;

    // same ways move
    if ( vSrcSlot == vDesSlot )
    {
        if ( vSrcSlot == SLOT_BAGGAGE )
            return mBaggage.Move( vSrcIndex, vDesIndex, mBaggage, vNumber, rNum );

        if ( vSrcSlot == SLOT_STORAGE )
            return mStorage.Move( vSrcIndex, vDesIndex, mStorage, vNumber, rNum );
    }

    // diff ways move
    if ( vSrcSlot != vDesSlot )
    {
		CItemBox *tpSrcItemBox = NULL;
		CItemBox *tpDesItemBox = NULL;
		switch( vSrcSlot )
		{	
			case SLOT_BAGGAGE:
			{
				tpSrcItemBox = &mBaggage;
				break;
			}
			case SLOT_STORAGE:
			{
				tpSrcItemBox = &mStorage;
				break;
			}
			default:
			break;
		}
		
		switch( vDesSlot )
		{	
			case SLOT_BAGGAGE:
			{
				tpDesItemBox = &mBaggage;
				break;
			}
			case SLOT_STORAGE:
			{
				tpDesItemBox = &mStorage;
				break;
			}
			default:
				break;
		}
		if ( tpSrcItemBox == NULL || tpDesItemBox == NULL )
		{
			return ERROR_MOVEITEM_SLOTINVALID;
		}

		return tpSrcItemBox->Move( vSrcIndex, vDesIndex, *tpDesItemBox, vNumber, rNum );	    
    }

    // 
    return ERROR_MOVEITEM_SLOTINVALID;
}

int CPropertyPlayer::InsertBaggageItem( CItemObject* pItem, int &vPos, int *vIndex, int *vNumber, int &vLen )
{
	LK_ASSERT(pItem != NULL, return ERROR_OBJECT_NOTFOUND);

	CTplItem* tpItemTpl = (CTplItem*) CDataStatic::SearchTpl( pItem->GetItemID() );
	if ( tpItemTpl == NULL )
	{
		return ERROR_ITEM_DATANOTEXIST;
	}
        
	if ( IS_TASK_ITEM( tpItemTpl ) )
	{
		vPos = POS_TASK_BAGGAGE;
		return mTaskBaggage.InsertItem( pItem, vIndex, vNumber, vLen );
	}
	else
	{
		vPos = POS_BAGGAGE;
		return mBaggage.InsertItem( pItem, vIndex, vNumber, vLen ); 
	}  
}

//int CPropertyPlayer::InsertStorageItem( CItemObject* pItem )
//{
//	return mStorage.InsertItem( pItem );
//}

int CPropertyPlayer::RemoveBaggageItem( unsigned int vIndex, unsigned int vNumber )
{
	return mBaggage.RemoveItem( vIndex, vNumber );
}

int CPropertyPlayer::RemoveStorageItem( unsigned int vIndex, unsigned int vNumber )
{
	return mStorage.RemoveItem( vIndex, vNumber );
}

int CPropertyPlayer::Equip( unsigned int vSrcIndex, unsigned int &vDesIndex )
{
    CItemObject* tpSrcItem = mBaggage.GetItemObjPtr( vSrcIndex );
    LK_ASSERT( tpSrcItem != NULL, return ERROR_OBJECT_NOTFOUND );

    int tErrorCode = mEquipment.Equip( this, mBaggage, vSrcIndex, vDesIndex );
    if ( tErrorCode == SUCCESS )
    {
        CItemEquipment* tpEquipItem = (CItemEquipment*) tpSrcItem;

		std::vector< unsigned short > tSkillList;
       // tpEquipItem->GetEquipSkillList( tSkillList );
		tpEquipItem->GetSkillList( tSkillList, CTemplateProperty::PROPERTYFUNC_OBTAINSKILL );

		for ( size_t i = 0; i < tSkillList.size( ); i ++ )
		{
            mEquipSkill.InsertSkill( tSkillList[ i ], 1);
		}
	
    }	

    return tErrorCode;
}

int CPropertyPlayer::UnEquip( unsigned short vPart, unsigned int vIndex )
{
    CItemObject* tpSrcItem = mEquipment.GetItemObjPtr( vPart );
    if ( tpSrcItem == NULL )
        return ERROR_UNEQUIPITEM_ITEMNOTEXIST;

    int tErrorCode = mEquipment.UnEquip( this, mBaggage, vPart, vIndex );
    if ( tErrorCode == SUCCESS )
    {
        CItemEquipment* tpEquipItem = (CItemEquipment*) tpSrcItem;

		std::vector< unsigned short > tSkillList;
        //tpEquipItem->GetEquipSkillList( tSkillList );
		tpEquipItem->GetSkillList( tSkillList, CTemplateProperty::PROPERTYFUNC_OBTAINSKILL );

        for ( size_t i = 0; i < tSkillList.size( ); i ++ )
		{
            mEquipSkill.RemoveSkill( tSkillList[ i ] );
		}
    }

    return tErrorCode;	
}

// ***********************************************************
//  Function:		RideHorse
//  Description:	上马
//  Input:			unsigned int vHorseTempID	坐骑道具模板ID
//  OutPut:			
//  Return:			int		0 = 上马, 1 = 下马, 其它 = 失败
//  Others:
//  Date:	11/24/2008
// **********************************************************
int CPropertyPlayer::RideHorse( unsigned int vHorseTempID )
{
	int tRet = -1;// 0=上马 1=下马
	CTplItemHorse* tHorseTpl = (CTplItemHorse*) CDataStatic::SearchTpl( vHorseTempID );
	if ( tHorseTpl == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] can't find CTplItemHorse TempID( %d )", __LK_FILE__, __LINE__, __FUNCTION__, vHorseTempID );
		return tRet;
	}

	// 没有骑马,则上马(改为通过BUFF查上马状态)
	CEntityPlayer* tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity(GetEntityID());
	LK_ASSERT( tpPlayer!=NULL, return -1 );
                                         
	// 如果和当前坐骑一致 则下马
	if ( tpPlayer->EntityHasBuff(tHorseTpl->mSpeedBuff) )
	{
		// 阻挡里不能下马
		if ( CMapModule::GetSingleton().IsStaticBlock( tpPlayer->GetLineID(), tpPlayer->GetMapID(), 
			tpPlayer->GetMapIndex(), tpPlayer->GetPos() ) )
		{
			return ERROR_CANNOTOFF;
		}
		tRet = OffHorse( tHorseTpl->mSpeedBuff );
		return tRet;
	}

	return 1;
}
int CPropertyPlayer::OffHorse(int nHorseBuffID)
{
	// 处理坐骑的加速BUFF
	CEntityPlayer* pPlayer = (CEntityPlayer*)GetSelfEntity();
	LK_ASSERT( pPlayer, return -1 );
	pPlayer->InitFunResult( );
	pPlayer->EntityRemoveBuff(nHorseBuffID);
	CPropertyModule::GetSingleton().ResultListSend( );

	Refresh( );

	return 0;
}


//************************************
// Method:    CreateFromTlv2
// FullName:  CPropertyPlayer::CreateFromTlv2
// Access:    public 
// Returns:   void
// Qualifier: 由property pb信息构造PropertyPlayer
//			 由于存数据库是分开存
// Parameter: PBProperty * pBuffer
//************************************
void CPropertyPlayer::CreateFromTlv2( PBProperty* pBuffer )
{
}

void CPropertyPlayer::GetTlvBuffer2( PBProperty* pBuffer )
{

}


//************************************
// Method:    GetTotalBufferForClient
// FullName:  CPropertyPlayer::GetTotalBufferForClient
// Access:    public 
// Returns:   void
// Qualifier: 填充玩家属性数据，因为数据太多，要几次填充，分包来发送，发送策略请和客户端同步
//				其中大量信息是客户端不需要的。
// Parameter: PBProperty * pBuffer
// Parameter: int nFillOder
//************************************
void CPropertyPlayer::GetTotalBufferForClient( PBProperty* pBuffer, int nFillOder  )
{
	LK_ASSERT ( pBuffer != NULL, return  )

	// 抽次填充基础数值信息
	if( nFillOder == 1)
	{
		
		/*
		pBuffer->set_curhp( mCurHP );
		pBuffer->set_curmp( mCurMP );
		pBuffer->set_curap( mCurAP );
		*/
		
		pBuffer->mutable_basicinfo()->set_remainpoint( mRemainPoint );
		pBuffer->mutable_basicinfo()->set_skillpoint( mSkillPoint );
		pBuffer->mutable_basicinfo()->set_prolevel( mProLevel );
		pBuffer->mutable_basicinfo()->set_proexp( mProExp );

		//GetBasicInfoTlvBuffer( pBuffer->mutable_basicinfo() );
		GetPvPBlobInfoTlvBuffer( pBuffer->mutable_pvpinfo() );

		pBuffer->mutable_statisticinfo()->set_completedtasks( mCompletedTasks );
		pBuffer->mutable_statisticinfo()->set_dienum( mDieNum );

		GetCDGroupTlvBuffer( pBuffer->mutable_cdgroup() );
		GetActivityInfoTlvBuffer( pBuffer->mutable_activity() );
		GetMonsterRecordInfoTlvBuffer( pBuffer->mutable_monsterrecord() );

		mSkill.GetTlvBuffer2( pBuffer->mutable_skill() );
		GetRoleClientInfoTlvBuffer( pBuffer->mutable_clientinfo() );
		
		GetTalentInfoTlvBuffer( pBuffer->mutable_talentinfo() );

		mBuffList.GetTlvBufferShow( pBuffer->mutable_buff() );

		CEntityPlayer* pEntity = (CEntityPlayer*) CSceneLogic::GetSingleton().GetEntity( GetEntityID() );
		if ( pEntity == NULL )
			return;

		GetMWInfoTlvBuffer( pBuffer->mutable_mwinfo() );

		mPlayerLifeSkill.CreatTlvPB( *pBuffer->mutable_lifeskillbox() );

		GetTitleInfoTlvBuffer( pBuffer->mutable_titleinfo() );

		mWizard.GetTlvBuffer( pBuffer->mutable_wizard() );

		GetMatrixRecordInfoTlvBuffer( pBuffer->mutable_matrix() );				
	}
	else if ( nFillOder == 2 )
	{	
		// 2次填充装备包裹信息， 仓库单独走另外消息发送
		PBItemBox* tpPBBaggage = pBuffer->mutable_baggage();
		PBItemBox* tpPBTaskBaggage = pBuffer->mutable_taskbaggage();
		PBItemBox* tpPBEquipment = pBuffer->mutable_equipment();
		mBaggage.GetTlvBuffer2( tpPBBaggage, 0, BAGGAGE_BASE );
		mTaskBaggage.GetTlvBuffer2( tpPBTaskBaggage );
		mEquipment.GetTlvBuffer2( tpPBEquipment );		
	}

}

void CPropertyPlayer::GetRepetDataForClient( PBRepetDataList * pBuffer)
{
	if ( pBuffer == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] pBuffer is null", __LK_FILE__, __LINE__, __FUNCTION__ );
		return;
	}

	for( int i = _REPE_INDEX_START_ ; i < _REPE_MAX_COUNT_ - 1 ; i++ )
	{
			CRepetionInfo* tpInfo = CRepetionModule::GetSingletonPtr( )->GetRepetionInfo( i );

			if( tpInfo != NULL )
			{
					int tIndex = tpInfo->GetPlayerDataToClientIndex() ;
					if( tIndex == -1)
					{
						continue;
					}

					PBRepetDataToClient *tpPBRepetDataToClient = pBuffer->add_repetdatatoclient();
					if( tpPBRepetDataToClient != NULL )
					{
						tpPBRepetDataToClient->set_repetindex( i );
						int tmp = 0;
						GetRepetion()->GetIntData( i, tIndex, &tmp );
						tpPBRepetDataToClient->set_repetdata( tmp );
					}
			}
	}
	return;
}

void CPropertyCharacter::CreateStateListFromTlv2( PBStateList* pBuffer )
{
	return;

	if ( pBuffer == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] pBuffer is null", __LK_FILE__, __LINE__, __FUNCTION__ );
		return;
	}

	int tCount = 0;
	tCount = pBuffer->stateparamobj_size();
	if ( tCount > 0 )
	{
		for ( int i = 0; i < tCount; i++ )
		{
			PBStateParam* tpPBStateParam = pBuffer->mutable_stateparamobj( i );
			if ( tpPBStateParam == NULL )
				continue;

			int tStateType = tpPBStateParam->statetype();
			if ( tStateType < 0 || tStateType >= (int)ARRAY_CNT( mStateParam ) )
			{
				LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tStateType error tSateType = %d, array_cnt = %d", __LK_FILE__, __LINE__, __FUNCTION__, tStateType, ARRAY_CNT( mStateParam ) );
				continue;
			}

			mStateParam[ tStateType ].mInState = true;
			mStateParam[ tStateType ].mFrom = (STATE_FROM) tpPBStateParam->from();
			mStateParam[ tStateType ].mIndex = tpPBStateParam->index();
			mStateParam[ tStateType ].mParames1 = tpPBStateParam->parames1();
			mStateParam[ tStateType ].mParames2 = tpPBStateParam->parames2();
			mStateParam[ tStateType ].mBuffAdd = tpPBStateParam->buffadd();
			mStateParam[ tStateType ].mInState = tpPBStateParam->instate();
		}
	}
}

void CPropertyCharacter::CreateBuffListFromTlv2( PBBuffList* pBuffer )
{
	LK_ASSERT( pBuffer != NULL, return );

	mBuffList.CreateFromTlv2( this, pBuffer );
}

void CPropertyCharacter::GetStateListTlvBuffer2( PBStateList* pBuffer )
{
	if ( pBuffer == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] pBuffer is null", __LK_FILE__, __LINE__, __FUNCTION__ );
		return;
	}

	for ( unsigned int i = 0; i < ARRAY_CNT( mStateParam ); i++ )
	{
		if ( mStateParam[ i ].mInState == false )
			continue;

		PBStateParam* tpPBStateParam = pBuffer->add_stateparamobj();
		if ( tpPBStateParam != NULL )
		{
			tpPBStateParam->set_statetype( i );
			tpPBStateParam->set_from( mStateParam[ i ].mFrom );
			tpPBStateParam->set_index( mStateParam[ i ].mIndex );
			tpPBStateParam->set_parames1( mStateParam[ i ].mParames1 );
			tpPBStateParam->set_parames2( mStateParam[ i ].mParames2 );
			tpPBStateParam->set_buffadd( mStateParam[ i ].mBuffAdd );
			tpPBStateParam->set_instate( mStateParam[ i ].mInState );
		}
	}
}

void CPropertyCharacter::GetBuffListTlvBuffer2( PBBuffList* pBuffer )
{
	LK_ASSERT( pBuffer != NULL, return );

	mBuffList.GetTlvBuffer2( pBuffer );
}

void CPropertyPlayer::GetItemForFree()
{ 
	// 获取身上处于绑定解除状态的装备
	GetItemForFreeFromEquipmet(  );
	
	// 获取包裹里面处于绑定解除状态的装备
	GetItemForFreeFromItemBox( mBaggage );
	
	// 获取仓库里面处于绑定解除状态的装备
	GetItemForFreeFromItemBox( mStorage );
}

void CPropertyPlayer::GetItemForFreeFromEquipmet()
{
	for( int i = 0 ; i < MAXEQUIPMENTINDEX ; i++ )
	{
		CItemObject *tpItemObj = ( CItemObject * ) mEquipment.GetItemObjPtr( i );
		if ( tpItemObj ==  NULL )
		{
			continue;
		}

		CTplItem *tpTplItem = ( CTplItem * )CDataStatic::SearchTpl( tpItemObj->GetItemID() );
		if ( tpTplItem == NULL )
		{
			continue;
		}
		if ( !tpTplItem->IsEquipable() )
		{
			continue;
		}		

		if ( (tpTplItem->mProcessType & CTplItem::PROCESSTYPE_CANBIND) == 0 )
		{
			continue;
		}
		if ( (tpItemObj->GetBindStatus() & ITEM_BINDSTATUS_FREEBIND ) == 0  )
		{
			continue;
		}
		CItemEquipment *tpEquipment = ( CItemEquipment * )tpItemObj;
		int				LeftTime    =  tpEquipment->GetBindFreeTime( ) - time( 0 );
		if ( LeftTime <= 0 )
		{	
			int BindStatus =  tpItemObj->GetBindStatus();
			BindStatus &= ~ITEM_BINDSTATUS_FREEBIND;
			continue;			
		}
		mItemForFreeList.push_back( tpItemObj->get_id() );
	}	
}

void CPropertyPlayer::GetItemForFreeFromItemBox( CItemBox &vItemBox )
{  
	CItemBoxToolkit tBoxToolkit(&vItemBox);

	for( unsigned int i = 0 ; i < vItemBox.mReleaseIndex ; i++ )
	{
		int nEquipObjID = tBoxToolkit.CheckEquipBindTime( i );
		if( nEquipObjID != INVALID_OBJ_ID )
			mItemForFreeList.push_back( nEquipObjID );
	}
	
	// 检查背包里面的物品
	for ( int i = 0; i < vItemBox.GetKitBagIndex(); ++i )
	{	
		int tBeginIndex = -1;
		int tEndIndex = -1;
		int tRet = vItemBox.GetKitBagIndexRange( BAGTYPE_KITBAG, i, tBeginIndex, tEndIndex );
		if ( tRet != SUCCESS )
		{
			continue;
		}
		
		for ( int j = tBeginIndex; j <= tEndIndex; ++j )
		{
			int nEquipObjID = tBoxToolkit.CheckEquipBindTime( j );
			if( nEquipObjID != INVALID_OBJ_ID )
				mItemForFreeList.push_back( nEquipObjID );
		}
	}
}

void CPropertyPlayer::InsertItemForFreeIntoList( CItemObject *tpItemObj )
{
    if ( mItemForFreeList.size() >= ITEM_FREE_COUNT )
    {
		return;
    }

	if ( tpItemObj == NULL )
	{
		return;
	}
	mItemForFreeList.push_back( tpItemObj->get_id() );
}

void CPropertyPlayer::CheckItemList( int TimeNow )
{	
	for (ItemForFreeMap::iterator it = mItemForFreeList.begin(); it != mItemForFreeList.end() ;)
	{	
		CItemEquipment *tpItemObj = ( CItemEquipment * )CSceneObjManager::GetSingletonPtr()->GetObject( *it );
		if ( tpItemObj == NULL )
		{
			it = mItemForFreeList.erase( it );
			LOG_ERROR( "pro", "[%s:%s:%d] can't find item object %d", __LK_FILE__, __FUNCTION__, __LINE__, *it );	
			continue;
		}
		int BindStatus = tpItemObj->GetBindStatus(  );
		if ( !(BindStatus&ITEM_BINDSTATUS_FREEBIND) )		
		{
			// 玩家取消了解除手动绑定的操作
			it = mItemForFreeList.erase( it );
			continue;
		}
		
		if ( tpItemObj->GetBindFreeTime() <=  TimeNow )
		{			
			BindStatus &= ~ITEM_BINDSTATUS_FREEBIND;
			tpItemObj->SetBindStatus( BindStatus );				
			it = mItemForFreeList.erase( it );
		}
		else
		{
			++it;
		}
	}
}

bool CPropertyPlayer::IsSPInit()
{
	CTemplateMetier* tpMetier = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID( ) );
	if( tpMetier == NULL )
	{
		return false;
	}

	return ( mSkillPoint == ( mLevel - tpMetier->mInitLevel ) * tpMetier->mSkillPoint + 1 );
}

bool CPropertyPlayer::IsPPInit()
{
	return ( mRemainPoint == CDataStatic::GetLevelProperty( mLevel, CUR_TOTAL_PROPERTY ) );
}

int CPropertyPlayer::ResetAllPP( )
{
	mCurSta = CDataStatic::GetMetierProperty( GetPropertyTempID(), mLevel, CUR_TOTAL_PROPERTY, OPTION_STA );
	mCurWis = CDataStatic::GetMetierProperty( GetPropertyTempID(), mLevel, CUR_TOTAL_PROPERTY, OPTION_WIS );
	mCurStr = CDataStatic::GetMetierProperty( GetPropertyTempID(), mLevel, CUR_TOTAL_PROPERTY, OPTION_STR );
	mCurCon = CDataStatic::GetMetierProperty( GetPropertyTempID(), mLevel, CUR_TOTAL_PROPERTY, OPTION_CON );
	mCurSpr = CDataStatic::GetMetierProperty( GetPropertyTempID(), mLevel, CUR_TOTAL_PROPERTY, OPTION_SPR );

	mRemainPoint = CDataStatic::GetLevelProperty( mLevel, CUR_TOTAL_PROPERTY );
    
    return ERROR_RESETAPP_SUCCESS;
}


int CPropertyPlayer::ResetAllSP( )
{
    int rtn = ERROR_RESETASP_SUCCESS;
    CTemplateMetier* tpMetier = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID( ) );
    if( tpMetier == NULL )
    {
        rtn = ERROR_METIER_DATANOTEXIST;
        return rtn;
    }
    
    mSkill.PartClean();

	mSkillPoint = ( mLevel - tpMetier->mInitLevel ) * tpMetier->mSkillPoint + 1;
    return rtn;
}

int CPropertyPlayer::ResetAllTalent( int vStep )
{
	int rtn = ERROR_RESETTALENT_SUCCESS;

	CTemplateTalent *tpTable = (CTemplateTalent*)CDataStatic::GetTemp( TEMP_TALENT );
	if ( tpTable == NULL )
	{
		rtn = ERROR_RESETTALENT_UNKNOWN;
		return rtn;
	}

	if ( mTalentStep <= vStep || vStep >= TOTAL_TALENT_STAGE )
	{
		rtn = ERROR_RESETTALENT_STEPLOW;
		return rtn;
	}

	int tReturnPoint = mTalentSkill.PartClean();

	mTalentPoint += tReturnPoint;
	mTalentStep = vStep;
	mTalentStudy = tpTable->mStep[ vStep ].mMaxPoint;

	return rtn;
}

int CPropertyPlayer::ResetSingleSP( unsigned short vGenerID, unsigned char vCol, unsigned char vRow )
{
    if ( IsSPResetable( vGenerID, vCol, vRow ) == true )
    {
        CTemplateSkillTree* tpSkillTree = (CTemplateSkillTree*) CDataStatic::SearchGener( vGenerID );
        CSkillTreeNode*     tpNode      = &tpSkillTree->mLayerNode[ vCol ][ vRow ];
        CSkillObject*       tpSkillObj  = mSkill.GetSkillObjectByID( tpNode->mSkillID );

        tpSkillObj->SetSkillLevel( tpSkillObj->GetSkillLevel() - 1 ) ;
        int tNeedPoint = 1;
        
        mSkillPoint += tNeedPoint;

        if ( tpSkillObj->GetSkillLevel() == 0 )
            mSkill.RemoveSkill( tpSkillObj->GetSkillID() );

        return ERROR_RESETSSP_SUCCESS;
    }

    return ERROR_RESETSSP_FAIL;
}


bool CPropertyPlayer::IsSPResetable( unsigned short vGenerID, unsigned char vCol, unsigned char vRow )
{
    CTemplateSkillTree* tpSkillTree = (CTemplateSkillTree*) CDataStatic::SearchGener( vGenerID );
    if ( tpSkillTree == NULL )
        return false;

    if ( vCol >= ARRAY_CNT( tpSkillTree->mLayerNode ) 
		|| vRow >= ARRAY_CNT( tpSkillTree->mLayerNode[ 0 ] )  )
        return false;

    CSkillTreeNode* tpNode = &tpSkillTree->mLayerNode[ vCol ][ vRow ];
    if ( tpNode == NULL )
        return false;

    CSkillObject* tpSkillObj = mSkill.GetSkillObjectByID( tpNode->mSkillID );
    if ( tpSkillObj == NULL )
        return false;

    CTemplateSkill* tpSkill = (CTemplateSkill*) CDataStatic::SearchSkill( tpSkillObj->GetSkillID(), tpSkillObj->GetSkillLevel() - tpSkillObj->GetIncLevel( ) );
    if ( tpSkill == NULL )
        return false;

    CTemplateMetier* tpMetier = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID( ) );
    if ( tpMetier == NULL )
        return false;

    // 计算本系投入技能点
    int tGenerPoint = mSkill.GetGenerPoint( vGenerID );

    for ( int i = 0; i < mSkill.GetSkillNum(); i ++ )
    {
        //if ( mSkill.mSkillList[ i ].GetSkillID() == 0 )
        //    break;
		CSkillObject* pSkillObj = mSkill.GetSkillObjectByIndex( i );

        CTemplateSkill* tpSubSkill = pSkillObj->GetSkillTpl();
        if ( tpSubSkill == NULL )
            return false;

        CTemplateSkillTree* tpSubSkillTree = (CTemplateSkillTree*) CDataStatic::SearchGener( tpSubSkill->mGenerID );
        if ( tpSubSkillTree == NULL )
            return false;

        bool tBreak = false;
        // 计算技能点投入总额是否满足
        for ( unsigned int m = 0; m < ARRAY_CNT( tpSkillTree->mLayerNode ) && tBreak == false; m ++ )
        {
            for ( unsigned int n = 0; n < ARRAY_CNT( tpSkillTree->mLayerNode[ 0 ] ) && tBreak == false; n ++ )
            {
                CSkillTreeNode* tpSubNode = &tpSubSkillTree->mLayerNode[ m ][ n ];
                if ( tpSubNode->mSkillID == tpSubSkill->mSkillID )
                {
                    if ( tpSubNode->mNeedGenerID == vGenerID && tpSubNode->mNeedGenerPoint >= tGenerPoint )
                        return false;

                    tBreak = true;
                }
            }
        }

        // 是否是同一系列
        if ( tpSubSkill->mGenerID == vGenerID )
        {
            // 是否有依赖关系
            bool tBreak = false;
            for ( unsigned int m = 0; m < ARRAY_CNT( tpSubSkillTree->mLayerNode ) && tBreak == false; m ++ )
            {
                for ( unsigned int n = 0; n < ARRAY_CNT( tpSubSkillTree->mLayerNode[ 0 ] ) && tBreak == false; n ++ )
                {
                    CSkillTreeNode* tpSubNode = &tpSubSkillTree->mLayerNode[ m ][ n ];
                    if ( tpSubNode->mSkillID == tpSubSkill->mSkillID )
                    {
                        // 如果该技能又存在需要
                        if ( (unsigned int)(tpSubNode->mNeedSkillID) == tpSkillObj->GetSkillID() 
								&& tpSubNode->mNeedSkillPoint >= (int)(tpSkillObj->GetSkillLevel() - tpSkillObj->GetIncLevel( )) )
                            return false;

                        tBreak = true;
                    }
                }
            }
        }
    }

    return true;
}

int CPropertyPlayer::ResetMetier( )
{
	// 重置技能点 
	ResetAllSP( );
	// 重置属性点
	ResetAllPP();
            
    return ERROR_RESETMETIER_SUCCESS;
}

int CPropertyPlayer::ClickPropertyPoint( EPropertyType vPropertyType )
{
    if ( mRemainPoint == 0 )
        return ERROR_CLICKPP_NEEDMOREPOINT;
                    
    switch( vPropertyType )
    {                   
    case PROPERTY_CON: mCurCon ++; mRemainPoint --; break;
    case PROPERTY_SPR: mCurSpr ++; mRemainPoint --; break;
    case PROPERTY_STA: mCurSta ++; mRemainPoint --; break;
    case PROPERTY_WIS: mCurWis ++; mRemainPoint --; break;
    case PROPERTY_STR: mCurStr ++; mRemainPoint --; break;
    default: return ERROR_CLICKPP_PROPERTYINVALID;
    }       
        
    return ERROR_CLICKPP_SUCCESS;
} 

// ***********************************************************
//  Function:		AddPropertyPoint
//  Description:	添加属性点
//  Input:			int vSTR	力量
//					int vCON	体质
//					int vSTA	耐力
//					int vWIS	智力
//					int vSPR	精神
//  OutPut:			
//  Return:			int			错误码
//  Others:
//  Date:	11/27/2008
// **********************************************************
int CPropertyPlayer::AddPropertyPoint( int vSTR, int vCON, int vSTA, int vWIS, int vSPR )
{
	int tTotalPoint = vSTR + vCON + vSTA + vWIS + vSPR;
	if ( mRemainPoint < tTotalPoint )
		return ERROR_CLICKPP_NEEDMOREPOINT;
	
	mCurStr += vSTR; mRemainPoint -= vSTR;
	mCurCon += vCON; mRemainPoint -= vCON;
	mCurSta += vSTA; mRemainPoint -= vSTA;
	mCurWis += vWIS; mRemainPoint -= vWIS;
	mCurSpr += vSPR; mRemainPoint -= vSPR;

	return ERROR_CLICKPP_SUCCESS;
}

void CPropertyPlayer::ChangeMetier( int vMetierID )
{
	if ( vMetierID > METIER_NOVICE )
		return;
    SetPropertyTempID( vMetierID );
    ResetMetier( );
}

int CPropertyPlayer::ClickSkillPoint( unsigned short vGenerID, unsigned char vCol, unsigned char vRow, unsigned int& vSkillID, int& vIndex, int& vMoney, int& vBindState )
{
	if ( vGenerID >= TALENT )
	{
		return ERROR_SKILL_DATANOTEXIST;
	}
    CTemplateSkillTree* tpSkillTree = (CTemplateSkillTree*) CDataStatic::SearchGener( vGenerID );
    if ( tpSkillTree == NULL )
        return ERROR_SKILL_DATANOTEXIST;

    if ( vCol >= ARRAY_CNT( tpSkillTree->mLayerNode ) || vRow >= ARRAY_CNT( tpSkillTree->mLayerNode[ 0 ] )  )
        return ERROR_SKILLTREE_INDEXOVERFLOW;

    CTemplateMetier* tpMetier = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID( ) );
    if ( tpMetier == NULL )
        return ERROR_SKILLTREE_DATANOTEXIST;

    CSkillTreeNode* tpNode = &tpSkillTree->mLayerNode[ vCol ][ vRow ];
    // 检查技能等级是否满足需求
    if ( (int)mLevel < tpNode->mLevel )
        return ERROR_CLICKSP_NEEDMORELEVEL;

    // 检查前导系系列投入需求
    if ( tpNode->mNeedGenerID != 0 )
    {
        // 如果投入不够， 返回
        if ( mSkill.GetGenerPoint( tpNode->mNeedGenerID ) < tpNode->mNeedGenerPoint )
            return ERROR_CLICKSP_NEEDGENERPOINT;
    }

    // 检查前导技能需求
    if ( tpNode->mNeedSkillID != 0 )
    {
        CSkillObject* tpSkill = mSkill.GetSkillObjectByID( tpNode->mNeedSkillID );
        // 如果指定需要的技能没有，如果不能配备该技能点
        if ( tpSkill == NULL )
            return ERROR_CLICKSP_NEEDSKILLPOINT;

        // 如果指定需要的技能存在，但是技能的投入不够，那么也不能配置技能点
        if (  (int)(tpSkill->GetSkillLevel() - tpSkill->GetIncLevel()) < tpNode->mNeedSkillPoint )
            return ERROR_CLICKSP_NEEDSKILLPOINT;
    }

	// 检查金钱是否足够	
	CSkillObject* tpSkill2 = mSkill.GetSkillObjectByID( tpSkillTree->mLayerNode[ vCol ][ vRow ].mSkillID );	
	CTemplateSkill* tpSkillTpl = NULL;
	// 技能找不到有几种情况,1技能还未学习,2技能找不到
	if ( tpSkill2 != NULL )
	{
		// 如果已经学到最大等级
		if ( tpSkillTree->mLayerNode[ vCol ][ vRow ].mMaxLevel <= (int)(tpSkill2->GetSkillLevel() - tpSkill2->GetIncLevel( )) )
		{
			return ERROR_CLICKSP_SKILLOVERFLOW;
		}

		// 查找当前原始技能等级的下一级技能
		tpSkillTpl = (CTemplateSkill*) CDataStatic::SearchSkill( tpSkill2->GetSkillID(), ( tpSkill2->GetSkillLevel() - tpSkill2->GetIncLevel( ) + 1 ) );
		if ( tpSkillTpl == NULL )
		{
			LOG_NOTICE( "pro", "[ %s : %d ] [ %s ] cann't find skill skillid( %d ) skilllevel( %d ) generid( %d ) col() row( %d )", 
				__LK_FILE__, __LINE__, __FUNCTION__, tpSkill2->GetSkillID( ), tpSkill2->GetSkillLevel( )- tpSkill2->GetIncLevel( ) + 1,
				vGenerID, vCol, vRow );
			return ERROR_CLICKSP_NONESKILL;
		}
		
		if ( tpSkillTpl->mBindStudyGold != LK_NOTBIND )
		{
			if ( IsMoneyEnough( true, tpSkillTpl->mStudyGold + vMoney ) == false )
			{
				return ERROR_CLICKSP_NEEDMONEY;
			}	
			vBindState = ( int ) true;
		}
		else
		{
			if ( IsMoneyEnough( false, tpSkillTpl->mStudyGold + vMoney ) == false )
			{
				return ERROR_CLICKSP_NEEDMONEY;
			}	
			vBindState = ( int ) false;
		}

		// 等级判断
		if ( (int)mLevel < tpSkillTpl->mStudySkillLevel )
			return ERROR_CLICKSP_NEEDMORELEVEL;
	}
	else
	{
		// 还没学习就找出第一次的技能模板
		tpSkillTpl = (CTemplateSkill*) CDataStatic::SearchSkill( tpSkillTree->mLayerNode[ vCol ][ vRow ].mSkillID, 1 );
		if ( tpSkillTpl == NULL )
		{
			LOG_NOTICE( "pro", "[ %s : %d ] [ %s ] cann't find skill skillid( %d ) skilllevel( %d ) generid( %d ) col() row( %d )", 
				__LK_FILE__, __LINE__, __FUNCTION__, tpSkillTree->mLayerNode[ vCol ][ vRow ].mSkillID, 1,
				vGenerID, vCol, vRow );
			return ERROR_CLICKSP_NONESKILL;
		}

		if ( tpSkillTpl->mBindStudyGold != LK_NOTBIND )
		{
			if ( IsMoneyEnough( true, tpSkillTpl->mStudyGold + vMoney ) == false )
			{
				return ERROR_CLICKSP_NEEDMONEY;
			}		
			vBindState = ( int ) true;
		}
		else
		{
			if ( IsMoneyEnough( false, tpSkillTpl->mStudyGold + vMoney ) == false )
			{
				return ERROR_CLICKSP_NEEDMONEY;
			}	
			vBindState = ( int ) false;
		}

		// 等级判断
		if ( (int)mLevel < tpSkillTpl->mStudySkillLevel )
			return ERROR_CLICKSP_NEEDMORELEVEL;
	}	

    int tNeedPoint = 1;
	// 除了新手技能不能学习其他职业的
	if ( tpSkillTree->mGenerID != CTemplateSkillTree::GENER_NOVICE )
	{
		bool tCanLearn = false;
		for ( int i = 0; i < (int)ARRAY_CNT( tpMetier->mGenerID ); i ++ )
		{
			if ( tpMetier->mGenerID[ i ] == vGenerID )
			{
				tCanLearn = true;
				break;
			}
		}

		if ( tCanLearn == false )
			return ERROR_CLICKSP_NEEDMETIER;
	}	

	// 技能消耗是否满足
    if ( mSkillPoint < tNeedPoint )
        return ERROR_CLICKSP_NEEDMOREPOINT;

    // 检查技能是否增加
    if ( mSkill.InsertSkill( tpNode->mSkillID, tpNode->mMaxLevel  ) == false )
        return ERROR_CLICKSP_SKILLOVERFLOW;

	// 查询刚刚新插入的技能的索引
	vSkillID = tpNode->mSkillID;
	vIndex =mSkill.GetSkillIndexByID( tpNode->mSkillID );

    mSkillPoint -= tNeedPoint;

	// 消耗金钱
	vMoney += tpSkillTpl->mStudyGold;

    return ERROR_CLICKSP_SUCCESS;
}

void CPropertyCharacter::Refresh( bool vRefresh )
{
	mInformation.mStr = RefreshStr( );
	mInformation.mWis = RefreshWis( );
	mInformation.mSpr = RefreshSpr( );
	mInformation.mSta = RefreshSta( );
	mInformation.mCon = RefreshCon( );

	mInformation.mAttackType = RefreshAttackType( );
	mInformation.mPAttackMax = RefreshPAttackMax( );
	mInformation.mMAttackMin = RefreshMAttackMin( );
	mInformation.mMAttackMax = RefreshMAttackMax( );
	mInformation.mPAttackMin = RefreshPAttackMin( );
	mInformation.mDeathRate = RefreshDeathRate( );
	mInformation.mDeathValue= RefreshDeathValue( );
	mInformation.mDeathDefense= RefreshDeathDefense( );
	mInformation.mNormalDuck= RefreshNormalDuck( );
	mInformation.mSkillDuck = RefreshSkillDuck( );
	mInformation.mIgnoreDuck= RefreshIgnoreDuck( );
	mInformation.mPDefence  = RefreshPDefence( );
	mInformation.mMDefence	= RefreshMDefence( );

	mInformation.mAddFixDamage = RefreshAddFixDamage( );
	mInformation.mAddPerDamage = RefreshAddPerDamage( );
	mInformation.mDecFixPDamage= RefreshDecFixPDamage( );
	mInformation.mDecFixMDamage= RefreshDecFixMDamage( );
	mInformation.mDecPerPDamage= RefreshDecPerPDamage( );
	mInformation.mDecPerMDamage= RefreshDecPerMDamage( );

	mInformation.mStealHP	= RefreshStealHP( );
	mInformation.mStealMP	= RefreshStealMP( );

	mInformation.mSkillHitRate = RefreshSkillHitRate( );
	mInformation.mNormalHitRate = RefreshNormalHitRate( );

	RefreshResDef( mInformation.mResDef );
	RefreshResAtt( mInformation.mResAtt );
	RefreshEquipWizard( mInformation.mEquipWizard );

	CEntity* tpEntity = GetSelfEntity( );
	if( tpEntity->IsPlayer( ) == true )
	{
		((CPropertyPlayer*)this)->RefreshSkillList( );
		((CPropertyPlayer*)this)->RefreshStateList( );
		((CPropertyPlayer*)this)->SetReduceLevel( ((CPropertyPlayer*)this)->GetEquipment( )->GetFixNumber( CTemplateProperty::PROPERTYFUNC_REDUCELEVEL ) );

		CPropertyValue tMaxAP = ((CPropertyPlayer*)this)->RefreshMaxAP( ); 
		if ( ( int )tMaxAP != ( int )mInformation.mMaxAP )
		{
			mInformation.mMaxAP = tMaxAP;
		}
		// 刷新人物同时要刷新宠物
		CEntityPet *tpPet = (CEntityPet*)CSceneLogic::GetSingleton().GetEntity( mPetID );
		if ( tpPet != NULL )
		{
			CPropertyPet *tpPetPro = (CPropertyPet*)tpPet->GetProperty();
			tpPetPro->Refresh( vRefresh );
			tpPet->NotifyPropertyRefresh();
		}
	}

	CPropertyValue tMaxHP = RefreshMaxHP( );//CPropertyFormula::GetMaxHP( this );
	CPropertyValue tMaxMP = RefreshMaxMP( );//CPropertyFormula::GetMaxMP( this );

	if ( ( int )tMaxHP != ( int )mInformation.mMaxHP )
	{
		mInformation.mMaxHP = tMaxHP;
	}
	if ( ( int )tMaxMP != ( int )mInformation.mMaxMP )
	{
		mInformation.mMaxMP = tMaxMP;
	}

	CPropertyValue tNewSpeed = CPropertyFormula::GetRunSpeed( this );

	if( ( tNewSpeed == mInformation.mSpeed ) == false )
	{		
		if( tpEntity->IsPlayer() )
		{
			((CEntityPlayer*)tpEntity)->SpeedCheck( true );
		}

	}

	mInformation.mSpeed      = tNewSpeed;
	mCurSpeed = CPropertyFormula::GetRunSpeed( this );

	mCurAP = std::min( ( unsigned int )GetMaxAP( ), mCurAP );

	if( vRefresh == false )
	{
		mCurHP = std::min( ( unsigned int )GetMaxHP( ), mCurHP );
    	mCurMP = std::min( ( unsigned int )GetMaxMP( ), mCurMP );		
	}
	else
	{
		mCurHP = GetMaxHP( );
		mCurMP = GetMaxMP( );
	}	
}

// 启动玩家绑定的定时器
void CPropertyPlayer::StartSavedTimer( ) 
{
	CEntityPlayer* pPlayer = (CEntityPlayer*) CSceneObjManager::GetSingletonPtr()->GetObject( GetEntityID() );
	LK_ASSERT( pPlayer != NULL, return );

	int nTimerOtherInfo[TIMER_OTHER_INFO] = {MESSAGE_PROPERTY, 0, 0, 0};
	
	// 技能CD 
	for( int i = 0; i < mSkill.GetSkillNum(); i++ )
	{
		CSkillObject* SkillObj = mSkill.GetSkillObjectByIndex( i );

		if( SkillObj->GetCooldown() > 0 )
		{
			nTimerOtherInfo[1] =  SkillObj->GetSkillID();
			nTimerOtherInfo[2] =  SkillObj->GetSkillLevel();

			int nTimerID = pPlayer->GetTimerReciver()->SetTimer( pPlayer->GetEntityID(),
				TIMER_SKLL_COOLDOWN,  SkillObj->GetCooldown(),
				3, nTimerOtherInfo ) ;

			SkillObj->SetCDTimer( nTimerID );
		}
	}

	// 法宝技能ID (法宝技能和身上的不会重复)
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)GetEquipment()->GetItemObjPtr(EQUIPMENT_MAGICWEAPON);
	if( tpMW != NULL )
	{
		tpMW->OnStartSkillCD( pPlayer );
	}

	// CD组
	for( int i = 0; i < mCDGroup.GetMaxGroupIdx(); i++ )
	{
		CGroupCD& rGroupCD = mCDGroup.GetGroupCD( i );
		if( rGroupCD.GetCDTime() > 0 )
		{
			int nOtherInfo[4] = { MESSAGE_PROPERTY, rGroupCD.GetCDGroupIdx(), 0, 0 };
			int nTimerID = pPlayer->GetTimerReciver()->SetTimer( pPlayer->GetEntityID(), TIMER_CDGROUP, rGroupCD.GetCDTime(), 2, nOtherInfo ); 
			rGroupCD.SetTimerID( nTimerID );
		}
	}


	// Buff定时器
	for( unsigned int i = 0; i < mBuffList.mBuffNumber; i++ )
	{
		if( mBuffList.mBuffList[ i ].mPersistTime > 0)
		{
			nTimerOtherInfo[1] =  mBuffList.mBuffList[ i ].mBuffID;

			CTemplateBuff* tpBuffTpl = (CTemplateBuff*) CDataStatic::SearchTpl( mBuffList.mBuffList[ i ].mBuffID );
			if( tpBuffTpl == NULL )
				continue;

			// 能走到这里，buff肯定不是首次作用，一定是已经作用过一段时间了
			mBuffList.mBuffList[ i ].mBuffTimerID = 
				pPlayer->GetTimerReciver()->SetTimer( pPlayer->GetEntityID(), 
					TIMER_BUFF_FUNCTION, 
					tpBuffTpl->mBuffActionTime > 0 ? tpBuffTpl->mBuffActionTime : mBuffList.mBuffList[ i ].mPersistTime, 
					2,
					nTimerOtherInfo);
		}
	}

}

void CPropertyPlayer::LevelUp( int vLevel )
{
    CTemplateMetier* tpMetier = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID( ) );
    if ( tpMetier == NULL )
    {
        printf( "Cann't find Metier Dateμ?[ MetierID = %d ]", GetPropertyTempID( ) );
        return;
    }

	LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] Old property mLevel( %d ) mSta( %d ) mWis( %d ) mStr( %d ) mCon( %d ) mSpr( %d )", 
		__LK_FILE__, __LINE__, __FUNCTION__, 
		mLevel, 
		mCurSta,
		mCurWis,
		mCurStr,
		mCurCon,
		mCurSpr
		);
    mLevel  += vLevel;

	// TODO: 新的属性点方案
	mCurSta += CDataStatic::GetMetierProperty( GetPropertyTempID(), mLevel, CUR_ADD_PROPERTY, OPTION_STA );
	mCurWis += CDataStatic::GetMetierProperty( GetPropertyTempID(), mLevel, CUR_ADD_PROPERTY, OPTION_WIS );
	mCurStr += CDataStatic::GetMetierProperty( GetPropertyTempID(), mLevel, CUR_ADD_PROPERTY, OPTION_STR );
	mCurCon += CDataStatic::GetMetierProperty( GetPropertyTempID(), mLevel, CUR_ADD_PROPERTY, OPTION_CON );
	mCurSpr += CDataStatic::GetMetierProperty( GetPropertyTempID(), mLevel, CUR_ADD_PROPERTY, OPTION_SPR );
	
	LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] New property mLevel( %d ) mSta( %d ) mWis( %d ) mStr( %d ) mCon( %d ) mSpr( %d )", 
		__LK_FILE__, __LINE__, __FUNCTION__, 
		mLevel, 
		mCurSta,
		mCurWis,
		mCurStr,
		mCurCon,
		mCurSpr
		);

	Refresh( true );

	mRemainPoint	+= CDataStatic::GetLevelProperty( mLevel, CUR_ADD_PROPERTY );
    mSkillPoint     += tpMetier->mSkillPoint * vLevel;

}

void CPropertyCharacter::DestroyPetEntity( )
{
    if ( mPetID != 0 )
    {
		LOG_NOTICE( "pro", "[ %s : %d ] mPetID = [ %d ] Destroy", __LK_FILE__, __LINE__, mPetID );
		CEntity* tpPetEntity = CSceneLogic::GetSingleton( ).GetEntity( mPetID );
		if( tpPetEntity != NULL )
		{
			// 把需要删除的召唤兽加入删除列表        
			//		CPropertyModule::GetSingleton( ).AddDeleteList( mPetID );
			int nTimerOtherInfo[ TIMER_OTHER_INFO ] = {MESSAGE_PROPERTY,0,0,0};
			tpPetEntity->GetTimerReciver()->SetTimer( tpPetEntity->GetEntityID(), TIMER_ENTITY_CLEAR, 650, 1, 
					nTimerOtherInfo );

			//print_trace_fd( "default" );
		}

    }
    
    mPetID = 0;
}

// 销毁召唤兽实体,主人实体销毁时
void CPropertyCharacter::DestroyPetByOwnerDestroy( )
{
	if ( mPetID != 0 )
	{
		CEntity* tpPetEntity = CSceneLogic::GetSingleton( ).GetEntity( mPetID );
		if ( tpPetEntity != NULL )
		{
			LOG_NOTICE( "pro", "[ %s : %d ] mPetID = [ %d ]", __LK_FILE__, __LINE__, mPetID );
			// 直接销毁宠物实体
//			CSceneLogic::GetSingleton( ).DestroyEntity( tpPetEntity, 0 );
			int nTimerOtherInfo[ TIMER_OTHER_INFO ] = {MESSAGE_PROPERTY,0,0,0};
			tpPetEntity->GetTimerReciver()->SetTimer( tpPetEntity->GetEntityID(), TIMER_ENTITY_CLEAR, 650, 1, 
				nTimerOtherInfo );

			//print_trace_fd( "default" );
			
		}
	}

	mPetID = 0;
}

int CPropertyNPC::Initial( )
{
	mRelifeTimer 	= 0;
	mDisappearTimer	= 0;
	mRelifeTime		= 0;
	mDisappearTime	= 0;
	mBattleTimer   = INVALID_OBJ_ID;
	mFirstEntityID	= INVALID_OBJ_ID;
	mTranChangeLevel = 0;
//	mDamageList.ClearDamage( );

	CharacterInitial( );
	return 0;
}

int CPropertyNPC::Final( )
{
	CharacterFinal( );
	return 0;
}

void CPropertyCharacter::InsertDamage( const CDamage& rDamage )
{
	mMaxDamager = mDamageList.InsertDamage( rDamage );
}


float CPropertyCharacter::GetDamagePer( unsigned int vEntityID )
{
    int tMaxHP = CPropertyFormula::GetMaxHP( this );
    return mDamageList.GetDamagePer( vEntityID, tMaxHP );
}

int CPropertyPet::Initial( )
{
	//mMonsterID	= 0;
	CharacterInitial( );
	return 0;
}

int CPropertyPet::Final( )
{
	CharacterFinal( );
	return 0;
}

int CPropertyItem::Initial( )
{
	return 0;
}

int CPropertyItem::Final( )
{
	return 0;
}

bool CPropertyDrop::IsOwner ( int vCharID )
{
	for ( int i = 0; i < TEAMNUM; i++ )
	{
		if ( mOwnerCharID[ i ] == vCharID )
			return true;
	}
	return false;
}

bool CPropertyDrop::HasOwner( )
{
	for ( int i = 0; i < TEAMNUM; i++ )
	{
		if ( mOwnerCharID[ i ] > 0 )
			return true;
	}
	return false;
}

//int CPropertyDrop::ProtectTimeEvent(	CEntity* pEntity, 
//										unsigned int vTickOffSet, 
//										CEntityResultList& pResultList
//									)
//{
//	if( pEntity->GetEntityType() != GetEntityType( ) )
//	{
//		return -1;
//	}
//
//	CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
//    if (tpItem != NULL)
//    {
//    	if ( mItemProtect > 0 && tpItem->mItemType != CTplItem::ITEM_TASK 
//				&&  tpItem->mItemType != CTplItem::ITEM_SPECIAL_TASK )
//        {
//        	mItemProtect = max( int( mItemProtect - vTickOffSet ), 0 );
//            if ( mItemProtect == 0 )
//            	memset( mOwnerCharID, 0, sizeof( mOwnerCharID ) );
//        }
//        mDisappearTimer = max( int( mDisappearTimer - vTickOffSet ), 0 );
//	}
//    else
//	{
//    	LOG_DEBUG("pro", "[ %s : %d ][ %s ]item id =%d is null in template", __LK_FILE__, __LINE__, __FUNCTION__,
//			GetPropertyTempID( ) );
//	}
//
//	if ( mDisappearTimer == 0 )
//    {
////    	pEntity->mAliveState = ALIVE_DIE;
//#ifdef _USE_TIMER_
//		pEntity->SetAliveStatus( ALIVE_CLEAR );
//		int nTimerOtherInfo[ TIMER_OTHER_INFO ] ;
//		unsigned char ucOtherInfoNum = 0;
//		nTimerOtherInfo[0] = MESSAGE_PROPERTY;
//
//		ucOtherInfoNum = 1;
//
//		pEntity->GetTimerReciver()->SetTimer( pEntity->get_id(), TIMER_ENTITY_CLEAR, 0, ucOtherInfoNum, nTimerOtherInfo );
//#else
//		pEntity->SetAliveStatus( ALIVE_DIE );
//#endif
//	}
//	return 0;
//}

int CPropertyPedal::Initial( )
{
	return 0;
}

int CPropertyPedal::Final( )
{
	return 0;
}

int CPropertyMoney::Initial( )
{
	mMoney 			= 0;
	mItemProtect 	= 0;
	memset( mOwnerCharID, 0, sizeof( mOwnerCharID ) );	

	return 0;
}

int CPropertyMoney::Final( )
{
	return 0;
}

bool CPropertyCharacter::CreateMessageNotifySpeedChanged(
	CMessage* pTotalMessage, 
	CMessageSpeedChangedNotify* pMessage,
	unsigned int vEntityID,
	int vSpeed
	)
{
	if ( pMessage == NULL || pTotalMessage == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return false;
	}

	CMessageHead* tpHead = NULL;
	tpHead = pTotalMessage->mutable_msghead();
	if ( tpHead == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return false;
	}

	tpHead->set_messageid( ID_S2C_NOTIFY_SPEEDCHANGED );

	pMessage->set_entityid( vEntityID );
	pMessage->set_speed( vSpeed );

	pTotalMessage->set_msgpara( (long)pMessage );
	return true;
}

void CPropertyPedal::IsDisappear( CEntity* pEntity, int vTickOffset )
{
	if ( pEntity == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] pEntity error", __LK_FILE__, __LINE__, __FUNCTION__ );
		return;
	}

	CEntityPedal* tpEntityPedal = (CEntityPedal*) pEntity;

	if ( tpEntityPedal->GetPedalType() == PEDAL_TELEPORT )
	{
		return;
	}

	tpEntityPedal->SetPresistTime( std::max( ( tpEntityPedal->GetPresistTime() - vTickOffset ) , 0 ) );

	if ( tpEntityPedal->GetPresistTime() == 0 )
	{
		if ( tpEntityPedal->GetPedalType() == PEDAL_TRAP )
		{
			CEntity* pOwner = tpEntityPedal->GetOwner();
			if ( pOwner != NULL )
			{
				CPropertyCharacter* pProOwner = (CPropertyCharacter*) pOwner->GetProperty();
				if ( pProOwner != NULL )
				{
					for ( int i = 0; i < pProOwner->GetTripEntityIDListSize( ); i++ )
					{
						if ( pProOwner->GetTripEntityIDList( i ).mTripEntityID == (int)tpEntityPedal->GetEntityID() )
						{
							pProOwner->GetTripEntityIDList( i ).Clean();
						}
					}
				}
			}
		}
		
		CSceneLogic::GetSingleton().AddDeleteList( pEntity->GetEntityID() );
	}
}

CPropertyValue CPropertyPlayer::RefreshMaxHP( )
{
	CPropertyValue tValue;

	int	tFixValue = mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_MAXHPFIX, TYPE_MAXHP )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_MAXHPFIX );// 得到装备固定值修正
	int tPerValue = mEquipment.GetPerNumber( CTemplateProperty:: PROPERTYINC_MAXHPPER )
		+ mEquipment.GetSuitPer(CTemplateProperty:: PROPERTYINC_MAXHPPER );// 得到装备百分比修正

	tFixValue += ( mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MAXHPFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECMAXHPFIX ) );	// 得到Buff固定值修正
	tPerValue += ( mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MAXHPPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECMAXHPPER ) );	// 得到Buff百分比修正

	tFixValue += mSkill.GetFixNumber(  CTemplateSkill::PASSINC_MAXHPFIX )
		+ mTalentSkill.GetFixNumber(  CTemplateSkill::PASSINC_MAXHPFIX )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_MAXHPFIX )
		+ mTitleSkill.GetFixNumber(  CTemplateSkill::PASSINC_MAXHPFIX );		// 得到技能固定值修正
	tPerValue += mSkill.GetPerNumber(  CTemplateSkill::PASSINC_MAXHPPER )
		+ mTalentSkill.GetPerNumber(  CTemplateSkill::PASSINC_MAXHPPER )
		+ MWGetPerNumber( CTemplateSkill::PASSINC_MAXHPPER )
		+ mTitleSkill.GetPerNumber(  CTemplateSkill::PASSINC_MAXHPPER );		// 得到技能百分比值修正	

	// 加法宝的
	tFixValue += CItemMagicWeapon::GetMaxHpForPlayer(this);
	
	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;
	// 得到基本属性修正
	int tLevelAmend = (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaHP );

	int tCorrect		= (int)( tpMetierTpl->mMaxHPPerSta * ( int )CPropertyFormula::GetSta( this ) 
							   + tpMetierTpl->mMaxHPPerStr * ( int )CPropertyFormula::GetStr( this )
							   + tpMetierTpl->mMaxHPPerWis * ( int )CPropertyFormula::GetWis( this )
							   + tpMetierTpl->mMaxHPPerCon * ( int )CPropertyFormula::GetCon( this )
							   + tpMetierTpl->mMaxHPPerSpr * ( int )CPropertyFormula::GetSpr( this )
							   );
	int tTotalValue		= (int)( ( tpMetierTpl->mInitHP + tLevelAmend + tFixValue + tCorrect ) 
			* ( 1.0 + tPerValue / SERVER_PERCENT_FLOAT ) );
	int tInitValue		= (int)( tpMetierTpl->mInitHP + tLevelAmend + tCorrect );
	tTotalValue			= std::max( tTotalValue, 1 );
	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;	

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshMaxHP( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL! ", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if( tpEntityNpc->IsNpc() == false )
	{
		return tValue;
	}

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;

	int tTplHP = tpNpcTpl->mHP;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		int tLevel = GetTranChangeLevel( );
		tTplHP = pTemplateOgreTran->mHP[ tLevel ];
	}

	int tFixValue = mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MAXHPFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECMAXHPFIX );	// 得到Buff固定值修正
	int	tPerValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MAXHPPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECMAXHPPER );	// 得到Buff百分比修正

	int tTotalValue	= ( int )( ( tTplHP + tFixValue ) * ( 1.0f + tPerValue / SERVER_PERCENT_FLOAT ) );
	int tInitValue = tTplHP;
	tTotalValue			= std::max( tTotalValue, 1 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshMaxHP( )
{
	CPropertyValue tValue;

	CEntityPet* tpEntityPet = (CEntityPet*)CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
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
		LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d! ", __LK_FILE__, __LINE__, 
			__FUNCTION__, tpEntityPet->GetCurLevel( ) );
		return tValue;
	}

	CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID() );
	if ( tpPetTpl == NULL )
	{
		LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	int tFixValue = mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MAXHPFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECMAXHPFIX );	// 得到Buff固定值修正
	int	tPerValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MAXHPPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECMAXHPPER );	// 得到Buff百分比修正

	int tTotalValue	= ( int )( ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mMaxHP + tFixValue ) 
		* (  1.0f + tPerValue / SERVER_PERCENT_FLOAT ) );
	int tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mMaxHP;


	CEntityPlayer *tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( tpEntityPet->GetOwnerID() );
	if ( tpPlayer != NULL )
	{
		CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
		tTotalValue += ( (int)tpProperty->GetMaxHP( ) ) * ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mHPPercent / SERVER_PERCENT_FLOAT );
	}	

	tTotalValue			= std::max( tTotalValue, 1 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;
			
	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshStr( )
{
	CPropertyValue tValue;

	int tFixEquip = 0;
	tFixEquip += /*mEquipment.GetFixStr( )*/mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_STR ) + mEquipment.GetSuitInc( 
			CTemplateProperty::PROPERTYINC_STR );			// 得到装备固定值修正
	tFixEquip += mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_ALL ) + mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_ALL );

	tFixEquip += mSkill.GetFixNumber(  CTemplateSkill::PASSINC_STR ) 
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_STR )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_STR )
		+ mTitleSkill.GetFixNumber(  CTemplateSkill::PASSINC_STR );	// 得到技能固定值修正
	tFixEquip += mSkill.GetFixNumber(  CTemplateSkill::PASSINC_ALL )
		+ mTalentSkill.GetFixNumber(  CTemplateSkill::PASSINC_ALL )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_ALL )
		+ mTitleSkill.GetFixNumber(  CTemplateSkill::PASSINC_ALL );	// 得到技能固定值修正
	
	tFixEquip += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_STRFIX );			// 得到BUFF固定值修正

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	int tPerValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_STRPER );

	unsigned int tTotalValue = ( mCurStr + tFixEquip ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT );
	unsigned int tInitValue	 = mCurStr;

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;


	return tValue;
}

CPropertyValue CPropertyNPC::RefreshStr( )
{
	CPropertyValue tValue;
	return tValue;
}

CPropertyValue CPropertyPet::RefreshStr( )
{
	CPropertyValue tValue;

	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshWis( )
{
	CPropertyValue tValue;

	int tFixEquip = 0;
	tFixEquip += /*mEquipment.GetFixWis( )*/mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_WIS )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_WIS );			// 得到装备固定值修正

	tFixEquip += mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_ALL ) + mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_ALL );

	tFixEquip += mSkill.GetFixNumber(  CTemplateSkill::PASSINC_WIS )
		+ mTalentSkill.GetFixNumber(  CTemplateSkill::PASSINC_WIS)
		+ MWGetFixNumber( CTemplateSkill::PASSINC_WIS)
		+ mTitleSkill.GetFixNumber(  CTemplateSkill::PASSINC_WIS);		// 得到技能固定值修正
	tFixEquip += mSkill.GetFixNumber(  CTemplateSkill::PASSINC_ALL )
		+ mTalentSkill.GetFixNumber(  CTemplateSkill::PASSINC_ALL )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_ALL )
		+ mTitleSkill.GetFixNumber(  CTemplateSkill::PASSINC_ALL );		// 得到技能固定值修正

	tFixEquip += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_WISFIX );			// 得到BUFF固定值修正

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	int tPerValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_WISPER );

	unsigned int tTotalValue = ( mCurWis + tFixEquip ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT );
	unsigned int tInitValue	 = mCurWis;

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshWis( )
{
	CPropertyValue tValue;
	return tValue;
}

CPropertyValue CPropertyPet::RefreshWis( )
{
	CPropertyValue tValue;
	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshSpr( )
{
	CPropertyValue tValue;

	int tFixEquip = 0;
	tFixEquip += /*mEquipment.GetFixSpr( )*/mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_SPR )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_SPR );			// 得到装备固定值修正

	tFixEquip += mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_ALL ) + mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_ALL );

	tFixEquip += mSkill.GetFixNumber(  CTemplateSkill::PASSINC_SPR )
		+ mTalentSkill.GetFixNumber(  CTemplateSkill::PASSINC_SPR )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_SPR )
		+ mTitleSkill.GetFixNumber(  CTemplateSkill::PASSINC_SPR );	// 得到技能固定值修正
	tFixEquip += mSkill.GetFixNumber(  CTemplateSkill::PASSINC_ALL )
		+ mTalentSkill.GetFixNumber(  CTemplateSkill::PASSINC_ALL )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_ALL )
		+ mTitleSkill.GetFixNumber(  CTemplateSkill::PASSINC_ALL );	// 得到技能固定值修正

	tFixEquip += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_SPRFIX );		// 得到BUFF固定值修正

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	int tPerValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_SPRPER );

	unsigned int tTotalValue = ( mCurSpr + tFixEquip ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT );
	unsigned int tInitValue	 = mCurSpr;

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshSpr( )
{
	CPropertyValue tValue;
	return tValue;
}

CPropertyValue CPropertyPet::RefreshSpr( )
{
	CPropertyValue tValue;
	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshSta( )
{
	CPropertyValue tValue;

	int tFixEquip = 0;
	tFixEquip += mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_STA )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_STA );			// 得到装备固定值修正

	tFixEquip += mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_ALL ) + mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_ALL );

	tFixEquip += mSkill.GetFixNumber( CTemplateSkill::PASSINC_STA )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_STA )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_STA )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_STA );	// 得到技能固定值修正
	tFixEquip += mSkill.GetFixNumber( CTemplateSkill::PASSINC_ALL )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_ALL )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_ALL )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_ALL );	// 得到技能固定值修正

	tFixEquip += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_STAFIX );		// 得到BUFF固定值修正

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	int tPerValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_STAPER );

	unsigned int tTotalValue = ( mCurSta + tFixEquip ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT );
	unsigned int tInitValue	 = mCurSta;

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;


	return tValue;
}

CPropertyValue CPropertyNPC::RefreshSta( )
{
	CPropertyValue tValue;
	return tValue;
}

CPropertyValue CPropertyPet::RefreshSta( )
{
	CPropertyValue tValue;
	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshCon( )
{
	CPropertyValue tValue;

	int tFixEquip = 0;
	tFixEquip += ( mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_CON )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_CON ) );			// 得到装备固定值修正

	tFixEquip += mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_ALL ) + mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_ALL );

	tFixEquip += mSkill.GetFixNumber( CTemplateSkill::PASSINC_CON )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_CON )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_CON )
		+ mTitleSkill.GetFixNumber(  CTemplateSkill::PASSINC_CON );		// 得到技能固定值修正
	tFixEquip += mSkill.GetFixNumber( CTemplateSkill::PASSINC_ALL )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_ALL )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_ALL )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_ALL );		// 得到技能固定值修正

	tFixEquip += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_CONFIX );		// 得到BUFF固定值修正

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	int tPerValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_CONPER );

	unsigned int tTotalValue = ( mCurCon + tFixEquip ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT );
	unsigned int tInitValue	 = mCurCon;

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshCon( )
{
	CPropertyValue tValue;
	return tValue;
}

CPropertyValue CPropertyNPC::RefreshCon()
{
	CPropertyValue tValue;
	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshMaxMP( )
{
	CPropertyValue tValue;

	int tFixValue = mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_MAXMPFIX, TYPE_MAXMP )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_MAXMPFIX );	// 得到装备固定值修正
	int	tPerValue = mEquipment.GetPerNumber( CTemplateProperty::PROPERTYINC_MAXMPPER )
		+ mEquipment.GetSuitPer( CTemplateProperty::PROPERTYINC_MAXMPPER );	// 得到装备百分比修正

	tFixValue	+= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MAXMPFIX );	// 得到Buff固定值修正
	tPerValue	+= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MAXMPPER );	// 得到Buff百分比修正

	tFixValue	+= mSkill.GetFixNumber( CTemplateSkill::PASSINC_MAXMPFIX ) 
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_MAXMPFIX )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_MAXMPFIX )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_MAXMPFIX );	// 得到技能固定值修正
	tPerValue	+= mSkill.GetPerNumber( CTemplateSkill::PASSINC_MAXMPPER  )
		+ mTalentSkill.GetPerNumber( CTemplateSkill::PASSINC_MAXMPPER  )
		+ MWGetPerNumber( CTemplateSkill::PASSINC_MAXMPPER  )
		+ mTitleSkill.GetPerNumber( CTemplateSkill::PASSINC_MAXMPPER  );	// 得到技能百分比修正

	// 加法宝的
	tFixValue += CItemMagicWeapon::GetMaxMpForPlayer(this);

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

		// 得到基本属性修正
	int tLevelAmend = (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaMP );

	int tCorrect		= (int)( tpMetierTpl->mMaxMPPerWis * ( int )CPropertyFormula::GetWis( this ) 
							   + tpMetierTpl->mMaxMPPerSpr * ( int )CPropertyFormula::GetSpr( this ) 
							   + tpMetierTpl->mMaxMPPerCon * ( int )CPropertyFormula::GetCon( this ) 
							   + tpMetierTpl->mMaxMPPerStr * ( int )CPropertyFormula::GetStr( this ) 
							   );
	int tTotalValue		= (int)( ( tpMetierTpl->mInitMP + tLevelAmend + tFixValue + tCorrect ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
	int tInitValue		= (int)( tpMetierTpl->mInitMP + tLevelAmend + tCorrect );
	tTotalValue			= std::max( tTotalValue, 0 );
	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshMaxMP( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL! ", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC /* && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC*/ )
		return tValue;

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;
	int tTplMP = tpNpcTpl->mMP;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		int tLevel = GetTranChangeLevel( );
		tTplMP = pTemplateOgreTran->mMP[ tLevel ];
	}

	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MAXMPFIX );	// 得到Buff固定值修正
	float	tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MAXMPPER );	// 吊紹uff翱咳修正

	int		tTotalValue	= ( int )( ( tTplMP + tFixValue ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
	int		tInitValue	= tTplMP;
	tTotalValue			= std::max( tTotalValue, 0 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;


	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshMaxAP()
{
	CPropertyValue tValue;

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) GetSelfEntity();

	CTplApConfig* tpConfig = (CTplApConfig*) CDataStatic::GetTemp( TEMP_APCONFIG );
	CTemplateBeginner *pBeginner = (CTemplateBeginner*)CDataStatic::GetTemp( TEMP_BEGINNER );
	
	if ( tpConfig == NULL || pBeginner == NULL )
	{
		return tValue;
	}

	int tUpLevel = tpPlayer->GetLevel() - 1;
	int tApMax = tUpLevel * tpConfig->mIncApMax;

	tValue.mDeltaValue = tApMax;
	tValue.mInitValue = pBeginner->mBeginnerAP;

	mInformation.mMaxAP = tValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshMaxMP( )
{
	CPropertyValue tValue;
	return tValue;
}

ATTACK_TYPE CPropertyPlayer::RefreshAttackType( )
{
	CItemObject* tpWeapon = mEquipment.GetItemObjPtr( EQUIPMENT_WEAPON );
	if( tpWeapon == NULL )
		return ATTACKTYPE_NONE;

	CTemplateWeapon* tpWeaponTpl = (CTemplateWeapon*) CDataStatic::SearchTpl( tpWeapon->GetItemID() );
	if( tpWeaponTpl == NULL )
		return ATTACKTYPE_NONE;

	return ( ATTACK_TYPE )tpWeaponTpl->mAttackType; 
}

ATTACK_TYPE CPropertyNPC::RefreshAttackType( )
{
	CTemplateOgre *tplNpc = (CTemplateOgre*)CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if( tplNpc == NULL )
		return ATTACKTYPE_NONE;

	return ( ATTACK_TYPE )tplNpc->mOgreType;
}

ATTACK_TYPE CPropertyPet::RefreshAttackType( )
{
	CEntityPet* tpEntityPet = (CEntityPet*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityPet == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ATTACKTYPE_PHYSICS;
	}

	if ( 
		tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
		&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
		&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
		)
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d! ", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
		return ATTACKTYPE_PHYSICS;
	}

	CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpPetTpl == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL! ", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ATTACKTYPE_PHYSICS;
	}
	

	ATTACK_TYPE tType = ATTACKTYPE_PHYSICS;
	if ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mAttackType == CTemplateSummonBeast::EM_ATTACK_TYPE_PHY )
		tType = ATTACKTYPE_PHYSICS;
	else
		tType = ATTACKTYPE_MAGIC; 

	return tType;
}

CPropertyValue CPropertyPlayer::RefreshPAttackMax( )
{
	CPropertyValue tValue;
	int	tFixEquip = mEquipment.GetFixPAttackUpper( this )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_PATTACKFIX );	// 得到装备固定值修正
	int	tPerEquip = mEquipment.GetPerNumber( CTemplateProperty::PROPERTYINC_PATTACKPER )
		+ mEquipment.GetSuitPer( CTemplateProperty::PROPERTYINC_PATTACKPER );	// 得到装备百分比修正

	tFixEquip += ( mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_PATTACKFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECPATTACKFIX ) );		// 得到BUFF固定值修正
	tPerEquip += ( mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_PATTACKPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECPATTACKPER ) );		// 得到BUFF百分比修正

	// 得到法宝固定值修正
	tFixEquip += CItemMagicWeapon::GetAttackForPlayer(this);

	tFixEquip += mSkill.GetFixNumber( CTemplateSkill::PASSINC_PATTACKFIX ) 
		+ mTalentSkill.GetFixNumber(  CTemplateSkill::PASSINC_PATTACKFIX )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_PATTACKFIX )
		+ mTitleSkill.GetFixNumber(  CTemplateSkill::PASSINC_PATTACKFIX );

	// 加上铸灵影响的属性值
	int nSpiritValue = m_SpiritInfo.GetEffectValue(0);
	tFixEquip += nSpiritValue;

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	// 得到基本属性修正
	int tLevelAmend		= (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaPAttack );
	int tCorrect		= (int)( tpMetierTpl->mPAttackPerStr * ( int )CPropertyFormula::GetStr( this )
		+ tpMetierTpl->mPAttackPerCon * ( int )CPropertyFormula::GetCon( this )
		+ tpMetierTpl->mPAttackPerWis * ( int )CPropertyFormula::GetWis( this )
		+ tpMetierTpl->mPAttackPerSpr * ( int )CPropertyFormula::GetSpr( this )
		);
	int tTotalValue		= (int)( ( tpMetierTpl->mInitPAttack + tLevelAmend + tFixEquip + tCorrect ) 
		* ( 1 + tPerEquip / SERVER_PERCENT_FLOAT ) );
	int tInitValue		= (int)( tpMetierTpl->mInitPAttack + tLevelAmend + tCorrect );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshMAttackMax( )
{
	CPropertyValue tValue;
	int tFixEquip = mEquipment.GetFixMAttackUpper( this ) 
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_MATTACKFIX );	// 得到装备固定值修正
	int	tPerEquip = mEquipment.GetPerNumber( CTemplateProperty::PROPERTYINC_MATTACKPER )
		+ mEquipment.GetSuitPer( CTemplateProperty::PROPERTYINC_MATTACKPER );	// 得到装备百分比修正

	tFixEquip += ( mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MATTACKFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECMATTACKFIX ) );		// 得到BUFF固定值修正
	tPerEquip += ( mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MATTACKPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECMATTACKPER ) );		// 得到BUFF百分比修正

	// 得到法宝固定值修正
	tFixEquip += CItemMagicWeapon::GetAttackForPlayer(this);

	tFixEquip += mSkill.GetFixNumber( CTemplateSkill::PASSINC_MATTACKFIX )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_MATTACKFIX )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_MATTACKFIX )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_MATTACKFIX );

	// 加上铸灵影响的属性值
	int nSpiritValue = m_SpiritInfo.GetEffectValue(0);
	tFixEquip += nSpiritValue;

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	// 得到基本属性修正
	int tLevelAmend		= (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaMAttack );

	int tCorrect		= (int)( tpMetierTpl->mMAttackPerStr * ( int )CPropertyFormula::GetStr( this )
		+ tpMetierTpl->mMAttackPerCon * ( int )CPropertyFormula::GetCon( this )
		+ tpMetierTpl->mMAttackPerWis * ( int )CPropertyFormula::GetWis( this )
		+ tpMetierTpl->mMAttackPerSpr * ( int )CPropertyFormula::GetSpr( this )
		);
	int tTotalValue		= (int)( ( tpMetierTpl->mInitMAttack + tLevelAmend + tCorrect + tFixEquip ) * ( 1 + tPerEquip / SERVER_PERCENT_FLOAT ) );
	int tInitValue		= (int)( tpMetierTpl->mInitMAttack + tLevelAmend + tCorrect );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;
	return tValue;
}

CPropertyValue CPropertyNPC::RefreshPAttackMax( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL! ", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC/* && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC*/  )
		return tValue;

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;
	int tTplPAttack = tpNpcTpl->mPAttack;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		int tLevel = GetTranChangeLevel( );
		tTplPAttack = pTemplateOgreTran->mPA[ tLevel ];
	}

	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_PATTACKFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECPATTACKFIX );	// 得到Buff固定值修正
	int		tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_PATTACKPER ) - mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECPATTACKPER );	// 得到Buff百分比修正

	int		tTotalValue	= ( int )( ( tTplPAttack + tFixValue ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
	int		tInitValue	= tTplPAttack;
	tTotalValue			= std::max( tTotalValue, 0 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshMAttackMax( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL! ", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC/* && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC*/  )
		return tValue;


	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;
	int tTplMAttack = tpNpcTpl->mMAttack;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		int tLevel = GetTranChangeLevel( );
		tTplMAttack = pTemplateOgreTran->mMA[ tLevel ];
	}

	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MATTACKFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECMATTACKFIX );	// 得到Buff固定值修正
	int		tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MATTACKPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECMATTACKPER );	// 得到Buff百分比修正

	int		tTotalValue	= ( int )( ( tTplMAttack + tFixValue ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
	int		tInitValue	= tTplMAttack;
	tTotalValue			= std::max( tTotalValue, 0 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshPAttackMax( )
{
	CPropertyValue tValue;
	CEntityPet* tpEntityNpc = (CEntityPet*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}
	CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
	if ( tpEntityPet == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( 
		tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
		&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
		&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
		)
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
		return tValue;
	}

	CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpPetTpl == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_PATTACKFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECPATTACKFIX );	// 得到Buff固定值修正
	int		tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_PATTACKPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECPATTACKPER );	// 得到Buff百分比修正

	int		tTotalValue	= ( int )( ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mPAttackUp + tFixValue ) 
		* ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
	int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mPAttackUp;

	CEntityPlayer *tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( tpEntityPet->GetOwnerID() );
	if ( tpPlayer != NULL )
	{
		CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
		tTotalValue += ( (int)tpProperty->GetPAttackMax( ) ) * ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mAttPercent / SERVER_PERCENT_FLOAT );
	}	

	tTotalValue			= std::max( tTotalValue, 0 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshMAttackMax( )
{
	CPropertyValue tValue;
	CEntityPet* tpEntityNpc = (CEntityPet*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MATTACKFIX ) 
			- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECMATTACKFIX );	// 得到Buff固定值修正
		int		tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MATTACKPER ) 
			- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECMATTACKPER );	// 得到Buff百分比修正

		int		tTotalValue	= ( int )( ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mMAttackUp + tFixValue ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
		int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mMAttackUp;

		CEntityPlayer *tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( tpEntityPet->GetOwnerID() );
		if ( tpPlayer != NULL )
		{
			CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
			tTotalValue += ( (int)tpProperty->GetMAttackMax( ) ) * ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mAttPercent / SERVER_PERCENT_FLOAT );
		}

		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
	}

	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshPAttackMin( )
{
	CPropertyValue tValue;
	int tFixEquip = mEquipment.GetFixPAttackLower( this ) 
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_PATTACKFIX );	// 得到装备固定值修正
	int	tPerEquip = mEquipment.GetPerNumber( CTemplateProperty::PROPERTYINC_PATTACKPER )
		+ mEquipment.GetSuitPer( CTemplateProperty::PROPERTYINC_PATTACKPER );	// 得到装备百分比修正

	tFixEquip += ( mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_PATTACKFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECPATTACKFIX ) );		// 得到BUFF固定值修正
	tPerEquip += ( mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_PATTACKPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECPATTACKPER ) );		// 得到BUFF百分比修正

	// 得到法宝固定值修正
	tFixEquip += CItemMagicWeapon::GetAttackForPlayer(this);

	tFixEquip += mSkill.GetFixNumber( CTemplateSkill::PASSINC_PATTACKFIX )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_PATTACKFIX )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_PATTACKFIX )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_PATTACKFIX );

	// 加上铸灵影响的属性值
	int nSpiritValue = m_SpiritInfo.GetEffectValue(0);
	tFixEquip += nSpiritValue;

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	// 得到基本属性修正
	int tLevelAmend		= (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaPAttack );

	int tCorrect		= (int)( tpMetierTpl->mPAttackPerStr * ( int )CPropertyFormula::GetStr( this )
		+ tpMetierTpl->mPAttackPerCon * ( int )CPropertyFormula::GetCon( this )
		+ tpMetierTpl->mPAttackPerWis * ( int )CPropertyFormula::GetWis( this )
		+ tpMetierTpl->mPAttackPerSpr * ( int )CPropertyFormula::GetSpr( this )
		);
	int tTotalValue		= (int)( ( tpMetierTpl->mInitPAttack + tLevelAmend + tFixEquip + tCorrect ) 
		* ( 1 + tPerEquip / SERVER_PERCENT_FLOAT ) );
	int tInitValue		= (int)( tpMetierTpl->mInitPAttack + tLevelAmend + tCorrect );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshMAttackMin( )
{
	CPropertyValue tValue;
	int tFixValue = mEquipment.GetFixMAttackLower( this ) 
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_MATTACKFIX );// 得到装备固定值修正
	int	tPerValue = mEquipment.GetPerNumber( CTemplateProperty::PROPERTYINC_MATTACKPER )
		+ mEquipment.GetSuitPer( CTemplateProperty::PROPERTYINC_MATTACKPER );	// 得到装备百分比修正

	tFixValue += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MATTACKFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECMATTACKFIX );		// 得到BUFF固定值修正
	tPerValue += mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MATTACKPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECMATTACKPER ); // 得到BUFF百分比修正

	// 得到法宝固定值修正
	tFixValue += CItemMagicWeapon::GetAttackForPlayer(this);

	tFixValue += mSkill.GetFixNumber( CTemplateSkill::PASSINC_MATTACKFIX )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_MATTACKFIX )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_MATTACKFIX )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_MATTACKFIX );

	// 加上铸灵影响的属性值
	int nSpiritValue = m_SpiritInfo.GetEffectValue(0);
	tFixValue += nSpiritValue;

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	// 得到基本属性修正
	int tLevelAmend		= (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaMAttack );

	int tCorrect		= (int)( tpMetierTpl->mMAttackPerStr * ( int )CPropertyFormula::GetStr( this )
		+ tpMetierTpl->mMAttackPerCon * ( int )CPropertyFormula::GetCon( this )
		+ tpMetierTpl->mMAttackPerWis * ( int )CPropertyFormula::GetWis( this )
		+ tpMetierTpl->mMAttackPerSpr * ( int )CPropertyFormula::GetSpr( this )
		);
	int tTotalValue		= (int)( ( tpMetierTpl->mInitMAttack + tLevelAmend + tFixValue + tCorrect ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
	int tInitValue		= (int)( tpMetierTpl->mInitMAttack + tLevelAmend + tCorrect );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshPAttackMin( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC /*&& tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC*/  )
		return tValue;
	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;
	int tTplPAttack = tpNpcTpl->mPAttack;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		int tLevel = GetTranChangeLevel( );
		tTplPAttack = pTemplateOgreTran->mPA[ tLevel ];
	}

	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_PATTACKFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECPATTACKFIX );	// 得到Buff固定值修正
	int		tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_PATTACKPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECPATTACKPER );	// 得到Buff百分比修正

	int		tTotalValue	= ( int )( ( tTplPAttack + tFixValue ) 
		* ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
	int		tInitValue	= tTplPAttack;
	tTotalValue			= std::max( tTotalValue, 0 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;
	return tValue;
}

CPropertyValue CPropertyNPC::RefreshMAttackMin( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC /*&& tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC*/  )
		return tValue;

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;
	int tTplMAttack = tpNpcTpl->mMAttack;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		int tLevel = GetTranChangeLevel( );
		tTplMAttack = pTemplateOgreTran->mMA[ tLevel ];
	}

	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MATTACKFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECMATTACKFIX );	// 得到Buff固定值修正
	int		tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MATTACKPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECMATTACKPER );	// 得到Buff百分比修正

	int		tTotalValue	= ( int )( ( tTplMAttack + tFixValue ) 
		* ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
	int		tInitValue	= tTplMAttack;
	tTotalValue			= std::max( tTotalValue, 0 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshPAttackMin( )
{
	CPropertyValue tValue;
	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_PATTACKFIX ) 
			- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECPATTACKFIX );	// 得到Buff固定值修正
		int		tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_PATTACKPER ) 
			- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECPATTACKPER );	// 得到Buff百分比修正

		int		tTotalValue	= ( int )( ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mPAttackLow + tFixValue ) 
			* ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
		int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mPAttackLow;

		CEntityPlayer *tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( tpEntityPet->GetOwnerID() );
		if ( tpPlayer != NULL )
		{
			CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
			tTotalValue += ( (int)tpProperty->GetPAttackMin( ) ) * ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mAttPercent / SERVER_PERCENT_FLOAT );
		}

		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
	}

	return tValue;
}

CPropertyValue CPropertyPet::RefreshMAttackMin( )
{
	CPropertyValue tValue;
	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) ); 
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MATTACKFIX ) 
			- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECMATTACKFIX );	// 得到Buff固定值修正
		int		tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MATTACKPER ) 
			- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECMATTACKPER );	// 得到Buff百分比修正

		int		tTotalValue	= ( int )( ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mMAttackLow + tFixValue ) 
			* ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
		int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mMAttackLow;

		CEntityPlayer *tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( tpEntityPet->GetOwnerID() );
		if ( tpPlayer != NULL )
		{
			CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
			tTotalValue += ( (int)tpProperty->GetMAttackMin( ) ) * ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mAttPercent / SERVER_PERCENT_FLOAT );
		}

		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
	}

	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshDeathRate( )
{
	CPropertyValue tValue;

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	int tFixValue = 0;
	tFixValue += mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_DEATHRATE, TYPE_DEATHRATE )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_DEATHRATE );			// 得到装备固定值修正

	tFixValue += mSkill.GetFixNumber( CTemplateSkill::PASSINC_DEATHRATE )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_DEATHRATE )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_DEATHRATE )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_DEATHRATE );	// 得到技能固定值修正

	tFixValue += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEATHHITFIX );				// 得到技能固定值修正

	// 得到法宝修正
	tFixValue += CItemMagicWeapon::GetDeadLineForPlayer(this);

		// 得到基本属性修正
	int tLevelAmend = (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaDeathRate );

	int tCorrect = (int)( tpMetierTpl->mDeathRatePerStr * ( int )CPropertyFormula::GetStr( this )
		                + tpMetierTpl->mDeathRatePerCon * ( int )CPropertyFormula::GetCon( this )
		                + tpMetierTpl->mDeathRatePerWis * ( int )CPropertyFormula::GetWis( this )
		                + tpMetierTpl->mDeathRatePerSpr * ( int )CPropertyFormula::GetSpr( this )
		                );

	int tTotalValue = tpMetierTpl->mInitDeathRate + tLevelAmend + tFixValue + tCorrect;

	tValue.mInitValue	= tTotalValue;
	tValue.mDeltaValue	= 0;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshDeathRate( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC /*&& tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC*/  )
		return tValue;

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;

	int	tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEATHHITFIX );		// 得到Buff固定值修正

	int	tTotalValue	= tpNpcTpl->mDeathRate + tFixValue;
	int	tInitValue	= tpNpcTpl->mDeathRate;
	tTotalValue	= std::max( tTotalValue, 0 );

	tValue.mInitValue  = tInitValue;
	tValue.mDeltaValue = tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshDeathRate( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}
			
		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEATHRATEFIX );		// 得到Buff固定值修正

		int		tTotalValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mDeathRate + tFixValue;
		int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mDeathRate;
		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
	}

	return tValue;	
}

CPropertyValue CPropertyPlayer::RefreshDeathValue( )
{
	CPropertyValue tValue;

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	// 暴击伤害默认是100%加成
	int	tFixValue = (int)SERVER_PERCENT_FLOAT;
	tFixValue += ( mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_DEATHVALUE, TYPE_DEATHVALUE )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_DEATHVALUE ) );	// 得到装备固定值修正
		
	int tBuffValue = mSkill.GetFixNumber( CTemplateSkill::PASSINC_DEATHVALUE )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_DEATHVALUE )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_DEATHVALUE )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_DEATHVALUE );		// 得到被动技能修正
		
	tBuffValue += ( mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEATHRATEFIX ) );	// 得到Buff修正


		// 得到基本属性修正

	int tLevelAmend = (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaDeathValue );

	int tCorrect		= (int)( tpMetierTpl->mDeathValuePerStr * ( int )CPropertyFormula::GetStr( this )
		                       + tpMetierTpl->mDeathValuePerCon * ( int )CPropertyFormula::GetCon( this )
		                       + tpMetierTpl->mDeathValuePerWis * ( int )CPropertyFormula::GetWis( this )
		                       + tpMetierTpl->mDeathValuePerSpr * ( int )CPropertyFormula::GetSpr( this )
		                       );

	tValue.mInitValue	= ( tLevelAmend + tCorrect + tpMetierTpl->mInitDeathValue + tBuffValue + tFixValue );
	tValue.mDeltaValue	= 0;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshDeathValue( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC  )
		return tValue;

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;

	int	tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEATHRATEFIX );		// 得到Buff固定值修正

	int	tTotalValue	= tpNpcTpl->mDeathValue + tFixValue;
	int	tInitValue	= tpNpcTpl->mDeathValue;
	tTotalValue	= std::max( tTotalValue, 0 );

	tValue.mInitValue  = tInitValue;
	tValue.mDeltaValue = tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshDeathValue( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		int		tFixValue	= (int)SERVER_PERCENT_FLOAT;
		tFixValue          += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEATHRATEFIX );			// 得到Buff固定值修正

		int		tTotalValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mDeathValue + tFixValue;
		int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mDeathValue;
		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
	}

	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshDeathDefense( )
{
	CPropertyValue tValue;

	int	tFixValue = 0;
	tFixValue += mEquipment.GetFixNumber( CTemplateProperty::PROPERTYFUNC_DEATHDEFENSE ) + mEquipment.GetSuitInc( CTemplateProperty::PROPERTYFUNC_DEATHDEFENSE );			// 得到装备固定值修正

	tFixValue += mSkill.GetFixNumber( CTemplateSkill::PASSINC_DEATHDEFENSE )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_DEATHDEFENSE )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_DEATHDEFENSE )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_DEATHDEFENSE );	// 得到技能固定值修正

	tFixValue += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEATHDEFENSE );				// 得到BUFF固定值修正

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	// 得到基本属性修正
	int tLevelAmend = (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaDeathDefense );

	int tCorrect		= (int)( tpMetierTpl->mDeathDefensePerStr * ( int )CPropertyFormula::GetStr( this )
		                       + tpMetierTpl->mDeathDefensePerCon * ( int )CPropertyFormula::GetCon( this )
		                       + tpMetierTpl->mDeathDefensePerWis * ( int )CPropertyFormula::GetWis( this )
		                       + tpMetierTpl->mDeathDefensePerSpr * ( int )CPropertyFormula::GetSpr( this )
		                       );

	int tTotalValue	= tpMetierTpl->mInitDeathDefense + tLevelAmend + tFixValue + tCorrect;
	int tInitValue		= tpMetierTpl->mInitDeathDefense + tLevelAmend;

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshDeathDefense( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC  && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC )
		return tValue;

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;

	int tDeathDefense = tpNpcTpl->mDeathDefense;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		int tLevel = GetTranChangeLevel( );
		tDeathDefense = pTemplateOgreTran->mDeath[ tLevel ];
	}

	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEATHDEFENSE );		// 得到Buff固定值修正

	int		tTotalValue	= tDeathDefense + tFixValue;
	int		tInitValue	= tDeathDefense;
	tTotalValue			= std::max( tTotalValue, 0 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshDeathDefense( )
{
	CPropertyValue tValue;	

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		int		tFixValue	= 0;
		tFixValue          += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEATHDEFENSE );			// 得到Buff固定值修正

		int		tTotalValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mDeathDefense + tFixValue;
		int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mDeathDefense;
		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
	}

	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshNormalDuck( )
{
	CPropertyValue tValue;

	int	tFixValue = 0;
	tFixValue += mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_NORMALDUCK ) + mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_NORMALDUCK );			// 得到装备固定值修正

	tFixValue += mSkill.GetFixNumber( CTemplateSkill::PASSINC_NORMALDUCK )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_NORMALDUCK )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_NORMALDUCK )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_NORMALDUCK );	// 得到技能固定值修正

	tFixValue += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_NORMALDUCKFIX );				// 得到BUFF固定值修正

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

		// 得到基本属性修正
	int tLevelAmend = (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaNormalDuck );

	int tCorrect		= (int)( tpMetierTpl->mNormalDuckPerStr * ( int )CPropertyFormula::GetStr( this )
		                       + tpMetierTpl->mNormalDuckPerCon * ( int )CPropertyFormula::GetCon( this )
		                       + tpMetierTpl->mNormalDuckPerWis * ( int )CPropertyFormula::GetWis( this )
		                       + tpMetierTpl->mNormalDuckPerSpr * ( int )CPropertyFormula::GetSpr( this )
		                       );

	int tTotalValue	= tpMetierTpl->mInitNormalDuck + tLevelAmend + tFixValue + tCorrect;
	int tInitValue		= tpMetierTpl->mInitNormalDuck + tLevelAmend;

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshNormalDuck( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC  /*&& tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC*/ )
		return tValue;

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;

	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_NORMALDUCKFIX );		// 得到Buff固定值修正

	int		tTotalValue	= tpNpcTpl->mNormalDuck + tFixValue;
	int		tInitValue	= tpNpcTpl->mNormalDuck;
	tTotalValue			= std::max( tTotalValue, 0 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshNormalDuck( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_NORMALDUCKFIX );		// 得到Buff固定值修正

		int		tTotalValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mNormalDuck + tFixValue;
		int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mNormalDuck;
		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
	}

	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshSkillDuck( )
{
	CPropertyValue tValue;

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	int	tFixValue = 0;
	tFixValue += ( /*mEquipment.GetFixSkillDuck( )*/mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_SKILLDUCK )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_SKILLDUCK ) );	// 得到装备固定值修正

	tFixValue += mSkill.GetFixNumber( CTemplateSkill::PASSINC_SKILLDUCK )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_SKILLDUCK )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_SKILLDUCK )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_SKILLDUCK );// 得到技能固定值修正

	tFixValue += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_SKILLDUCKFIX );				// 得到BUFF固定值修正

		// 得到基本属性修正
	int tLevelAmend = (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaSkillDuck );

	int tTotalValue	= tpMetierTpl->mInitSkillDuck + tLevelAmend + tFixValue;
	int tInitValue	= tpMetierTpl->mInitSkillDuck + tLevelAmend;

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshSkillDuck( )
{
	CPropertyValue tValue;

	/*CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC  && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC )
		return tValue;

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;
	int tTplDuck = tpNpcTpl->mSkillDuck;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		tTplDuck = pTemplateOgreTran->mDodge[ mLevel ];
	}

	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_SKILLDUCKFIX );		// 得到Buff固定值修正

	int		tTotalValue	= tTplDuck + tFixValue;
	int		tInitValue	= tTplDuck;
	tTotalValue			= std::max( tTotalValue, 0 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;
*/

	return tValue;
}

CPropertyValue CPropertyPet::RefreshSkillDuck( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d", __LK_FILE__, __LINE__, 
					__FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tValue;
		}

		CTemplateSummonBeast* tpPetTpl = 
				(CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_SKILLDUCKFIX );		// 得到Buff固定值修正

		int		tTotalValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mSkillDuck + tFixValue;
		int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mSkillDuck;
		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
	}

	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshIgnoreDuck( )
{
	CPropertyValue tValue;

	int	tFixValue = 0;
	tFixValue += mEquipment.GetFixNumber( CTemplateProperty::PROPERTYFUNC_IGNOREDUCK ) + mEquipment.GetSuitInc( CTemplateProperty::PROPERTYFUNC_IGNOREDUCK );			// 得到装备固定值修正

	tFixValue += mSkill.GetFixNumber( CTemplateSkill::PASSINC_IGNOREDUCK )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_IGNOREDUCK )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_IGNOREDUCK )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_IGNOREDUCK );	// 得到技能固定值修正

	tFixValue += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_IGNOREDUCK );				// 得到BUFF固定值修正

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;

	// 得到基本属性修正
	int tLevelAmend = (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaIgnoreDuck );

	int tCorrect		= (int)( tpMetierTpl->mIgnoreDuckPerStr * ( int )CPropertyFormula::GetStr( this )
		                       + tpMetierTpl->mIgnoreDuckPerCon * ( int )CPropertyFormula::GetCon( this )
		                       + tpMetierTpl->mIgnoreDuckPerWis * ( int )CPropertyFormula::GetWis( this )
		                       + tpMetierTpl->mIgnoreDuckPerSpr * ( int )CPropertyFormula::GetSpr( this )
		                       );

	int tTotalValue	= tpMetierTpl->mInitIgnoreDuck + tLevelAmend + tFixValue + tCorrect;
	int tInitValue		= tpMetierTpl->mInitIgnoreDuck + tLevelAmend;

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshIgnoreDuck( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC  && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC )
		return tValue;

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;

	int tIgnoreDuck = tpNpcTpl->mIgnoreDuck;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		int tLevel = GetTranChangeLevel( );
		tIgnoreDuck = pTemplateOgreTran->mIgnore[ tLevel ];
	}

	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_IGNOREDUCK );		// 得到Buff固定值修正

	int		tTotalValue	= tIgnoreDuck + tFixValue;
	int		tInitValue	= tIgnoreDuck;
	tTotalValue			= std::max( tTotalValue, 0 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshIgnoreDuck( )
{
	CPropertyValue tValue;	

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d", __LK_FILE__, __LINE__, 
				__FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tValue;
		}

		CTemplateSummonBeast* tpPetTpl = 
			(CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_IGNOREDUCK );		// 得到Buff固定值修正

		int		tTotalValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mIgnoreDuck + tFixValue;
		int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mIgnoreDuck;
		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
	}

	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshPDefence( )
{
	CPropertyValue tValue;

	int	tFixEquip = mEquipment.GetFixPDefence( this ) 
			+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_PDEFENCEFIX );	// 得到装备固定值修正
	int	tPerEquip = /*mEquipment.GetPerPDefence( )*/mEquipment.GetPerNumber( CTemplateProperty::PROPERTYINC_PDEFENCEPER )
			+ mEquipment.GetSuitPer( CTemplateProperty::PROPERTYINC_PDEFENCEPER );	

	tFixEquip += ( mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_PDEFENCEFIX ) + mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEFALLFIX )
			- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECPDEFENCEFIX ) );	// 得到Buff固定值修正
	tPerEquip += ( mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_PDEFENCEPER ) + mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DEFALLPER )
			- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECPDEFENCEPER ) );	// 得到Buff百分比修正

	tFixEquip += mSkill.GetFixNumber( CTemplateSkill::PASSINC_PDEFENCEFIX )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_PDEFENCEFIX )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_PDEFENCEFIX )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_PDEFENCEFIX );	// 得到技能固定值修正
	tPerEquip += mSkill.GetPerNumber( CTemplateSkill::PASSINC_PDEFENCEPER )
		+ mTalentSkill.GetPerNumber( CTemplateSkill::PASSINC_PDEFENCEPER )
		+ MWGetPerNumber( CTemplateSkill::PASSINC_PDEFENCEPER )
		+ mTitleSkill.GetPerNumber( CTemplateSkill::PASSINC_PDEFENCEPER );	// 得到技能百分比修正

	// 得到法宝修正
	tFixEquip += CItemMagicWeapon::GetRecoveryForPlayer(this);

	// 加上铸灵影响的属性值
	int nSpiritValue = m_SpiritInfo.GetEffectValue(2);
	tFixEquip += nSpiritValue;

	// 得到基本属性修正
	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;
	int tLevelAmend		= (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaPDefense );
	int tCorrect		= (int)( tpMetierTpl->mPDefensePerStr * ( int )CPropertyFormula::GetStr( this )
		                       + tpMetierTpl->mPDefensePerCon * ( int )CPropertyFormula::GetCon( this )
		                       + tpMetierTpl->mPDefensePerWis * ( int )CPropertyFormula::GetWis( this )
		                       + tpMetierTpl->mPDefensePerSpr * ( int )CPropertyFormula::GetSpr( this )
		                       );
	unsigned int tTotalValue	= (unsigned int)( ( tpMetierTpl->mInitPDefense + tLevelAmend + tCorrect + tFixEquip )
		* ( 1.0f + tPerEquip / SERVER_PERCENT_FLOAT ) );
	unsigned int tInitValue		= 0;

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshPDefence( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC /* && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC*/ )
		return tValue;

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;

	int tTplPD = tpNpcTpl->mPDefence;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		int tLevel = GetTranChangeLevel( );
		tTplPD = pTemplateOgreTran->mPD[ tLevel ];
	}
	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_PDEFENCEFIX ) + mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEFALLFIX )
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECPDEFENCEFIX );	// 得到Buff固定值修正
	int		tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_PDEFENCEPER ) + mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DEFALLPER )
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECPDEFENCEPER );	// 得到Buff百分比修正

	int		tTotalValue	= ( int )( ( tTplPD + tFixValue ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
	int		tInitValue	= tTplPD;
	tTotalValue			= std::max( tTotalValue, 0 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshPDefence( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_PDEFENCEFIX ) + mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEFALLFIX )
			- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECPDEFENCEFIX );	// 得到Buff固定值修正
		int		tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_PDEFENCEPER ) + mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DEFALLPER )
			- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECPDEFENCEPER );	// 得到Buff百分比修正

		int		tTotalValue	= ( int )( ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mPDefence + tFixValue ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
		int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mPDefence;

		CEntityPlayer *tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( tpEntityPet->GetOwnerID() );
		if ( tpPlayer != NULL )
		{
			CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
			tTotalValue += ( (int)tpProperty->GetPDefence( ) ) * ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mDefPercent / SERVER_PERCENT_FLOAT );
		}

		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
	}

	return tValue;
}

CPropertyValue CPropertyPlayer::RefreshMDefence( )
{
	CPropertyValue tValue;

	int tFixEquip = mEquipment.GetFixMDefence( this ) 
			+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_MDEFENCEFIX );	// 得到装备固定值修正
	int	tPerEquip = mEquipment.GetPerNumber( CTemplateProperty::PROPERTYINC_MDEFENCEPER )
			+ mEquipment.GetSuitPer( CTemplateProperty::PROPERTYINC_MDEFENCEPER );	

	tFixEquip += ( mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MDEFENCEFIX ) + mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEFALLFIX )
			- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECMDEFENCEFIX ) );	// 得到Buff固定值修正
	tPerEquip += ( mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MDEFENCEPER ) + mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DEFALLPER )
			- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECMDEFENCEPER ) );	// 得到Buff百分比修正

	tFixEquip += mSkill.GetFixNumber( CTemplateSkill::PASSINC_MDEFENCEFIX )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_MDEFENCEFIX )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_MDEFENCEFIX )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_MDEFENCEFIX );	// 得到技能固定值修正
	tPerEquip += mSkill.GetPerNumber( CTemplateSkill::PASSINC_MDEFENCEPER )
		+ mTalentSkill.GetPerNumber( CTemplateSkill::PASSINC_MDEFENCEPER )
		+ MWGetPerNumber( CTemplateSkill::PASSINC_MDEFENCEPER )
		+ mTitleSkill.GetPerNumber( CTemplateSkill::PASSINC_MDEFENCEPER );	// 得到技能百分比修正

	// 得到法宝修正
	tFixEquip += CItemMagicWeapon::GetRecoveryForPlayer(this);

	// 加上铸灵影响的属性值
	int nSpiritValue = m_SpiritInfo.GetEffectValue(1);
	tFixEquip += nSpiritValue;

	// 得到基本属性修正
	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return tValue;
	int tLevelAmend		= (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaMDefense );
	int tCorrect		= (int)( tpMetierTpl->mMDefensePerStr * ( int )CPropertyFormula::GetStr( this )
		                       + tpMetierTpl->mMDefensePerCon * ( int )CPropertyFormula::GetCon( this )
		                       + tpMetierTpl->mMDefensePerWis * ( int )CPropertyFormula::GetWis( this )
		                       + tpMetierTpl->mMDefensePerSpr * ( int )CPropertyFormula::GetSpr( this )
		                       );
	unsigned int tTotalValue	= (unsigned int)( ( tpMetierTpl->mInitPDefense + tLevelAmend + tCorrect + tFixEquip )
		* ( 1.0f + tPerEquip / SERVER_PERCENT_FLOAT ) );
	unsigned int tInitValue		= 0;

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyNPC::RefreshMDefence( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) != ENTITYTYPE_OGRE && tpEntityNpc->GetEntityType( ) != ENTITYTYPE_FUNCNPC  /*&& tpEntityNpc->GetEntityType( ) != ENTITYTYPE_TASKNPC*/ )
		return tValue;

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tValue;
	int tTplMD = tpNpcTpl->mMDefence;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		int tLevel = GetTranChangeLevel( );
		tTplMD = pTemplateOgreTran->mMD[ tLevel ];
	}

	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MDEFENCEFIX ) + mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEFALLFIX )
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECMDEFENCEFIX );	// 得到Buff固定值修正
	int		tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MDEFENCEPER ) + mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DEFALLPER )
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECMDEFENCEPER );	// 得到Buff百分比修正

	int		tTotalValue	= ( int )( ( tTplMD + tFixValue ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
	int		tInitValue	= tTplMD;
	tTotalValue			= std::max( tTotalValue, 0 );

	tValue.mInitValue	= tInitValue;
	tValue.mDeltaValue	= tTotalValue - tInitValue;

	return tValue;
}

CPropertyValue CPropertyPet::RefreshMDefence( )
{
	CPropertyValue tValue;

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tValue;
		}

		int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_MDEFENCEFIX ) + mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DEFALLFIX )
			- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECMDEFENCEFIX );	// 得到Buff固定值修正
		int		tPerValue	= mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_MDEFENCEPER ) + mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DEFALLPER )
			- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECMDEFENCEPER );	// 得到Buff百分比修正

		int		tTotalValue	= ( int )( ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mMDefence + tFixValue ) * ( 1 + tPerValue / SERVER_PERCENT_FLOAT ) );
		int		tInitValue	= tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mMDefence;

		CEntityPlayer *tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( tpEntityPet->GetOwnerID() );
		if ( tpPlayer != NULL )
		{
			CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
			tTotalValue += (int)( tpProperty->GetMDefence( ) ) * ( tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mDefPercent / SERVER_PERCENT_FLOAT );
		}

		tTotalValue			= std::max( tTotalValue, 0 );

		tValue.mInitValue	= tInitValue;
		tValue.mDeltaValue	= tTotalValue - tInitValue;
	}


	return tValue;
}

int CPropertyPlayer::RefreshAddFixDamage( )
{
	int tIncDamage = mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_DAMAGEFIX )
			+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_DAMAGEFIX )
			+ mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_INCSRCDAMAGEFIX ) 
			- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECSRCDAMAGEFIX );
	
	return tIncDamage;
}

int CPropertyNPC::RefreshAddFixDamage( )
{
	int tIncDamage = mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_INCSRCDAMAGEFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECSRCDAMAGEFIX );

	return tIncDamage;
}

int CPropertyPet::RefreshAddFixDamage( )
{
	int tIncDamage = mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_INCSRCDAMAGEFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECSRCDAMAGEFIX );

	return tIncDamage;
}

int CPropertyCharacter::RefreshAddPerDamage( )
{
	int tIncDamagePer = // tpProPlayer->mEquipment.GetPerIncDamage()
		mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_INCSRCDAMAGEPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECSRCDAMAGEPER );

	return tIncDamagePer;
}

int CPropertyPlayer::RefreshDecFixPDamage( )
{
	int tFixValue = mEquipment.GetFixNumber( CTemplateProperty::PROPERTYDEC_PDAMAGEFIX )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYDEC_PDAMAGEFIX ) 
		+ mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECDESPDAMAGEFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_INCDESPDAMAGEFIX )
		+ mSkill.GetFixNumber( CTemplateSkill::PASSINC_DAMAGELESSFIX )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_DAMAGELESSFIX )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_DAMAGELESSFIX )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_DAMAGELESSFIX );

	return tFixValue;
}


int CPropertyNPC::RefreshDecFixPDamage( )
{
	int tFixValue = mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECDESPDAMAGEFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_INCDESPDAMAGEFIX );

	return tFixValue;
}

int CPropertyPet::RefreshDecFixPDamage( )
{
	int tFixValue = mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECDESPDAMAGEFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_INCDESPDAMAGEFIX );

	return tFixValue;
}

int CPropertyPlayer::RefreshDecFixMDamage( )
{
	int tDecFixMDamage = mEquipment.GetFixNumber( CTemplateProperty::PROPERTYDEC_MDAMAGEFIX )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYDEC_MDAMAGEFIX ) 
		+ mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECDESMDAMAGEFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_INCDESMDAMAGEFIX )
		+ mSkill.GetFixNumber( CTemplateSkill::PASSINC_DAMAGELESSFIX )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_DAMAGELESSFIX )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_DAMAGELESSFIX )
		+ mTitleSkill.GetFixNumber(  CTemplateSkill::PASSINC_DAMAGELESSFIX );

		return tDecFixMDamage;
}

int CPropertyNPC::RefreshDecFixMDamage( )
{
	int tFixValue = mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECDESMDAMAGEFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_INCDESMDAMAGEFIX );

	return tFixValue;
} 

int CPropertyPet::RefreshDecFixMDamage( )
{
	int tFixValue = mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DECDESMDAMAGEFIX ) 
		- mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_INCDESMDAMAGEFIX );

	return tFixValue;
}

int CPropertyPlayer::RefreshDecPerPDamage( )
{
	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return 0;
	float tEquipmentPer = 1 - ( mEquipment.GetPerNumber( CTemplateProperty::PROPERTYDEC_PDAMAGEPER ) + mEquipment.GetSuitPer( CTemplateProperty::PROPERTYDEC_PDAMAGEPER ) ) / SERVER_PERCENT_FLOAT;
	float tBuffPer		= 1 - ( mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECDESPDAMAGEPER ) - mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_INCDESPDAMAGEPER ) ) / SERVER_PERCENT_FLOAT;
	float tSkillPer		= 1 - ( mSkill.GetPerNumber( CTemplateSkill::PASSINC_DAMAGELESSPER ) + mTalentSkill.GetPerNumber( CTemplateSkill::PASSINC_DAMAGELESSPER )+ MWGetPerNumber( CTemplateSkill::PASSINC_DAMAGELESSPER )
		+ mTitleSkill.GetPerNumber( CTemplateSkill::PASSINC_DAMAGELESSPER ) ) / SERVER_PERCENT_FLOAT;
	float tCorrect		= 1 - ( tpMetierTpl->mPReducePerSta * ( int )CPropertyFormula::GetSta( this ) ) / SERVER_PERCENT_FLOAT;

	int tFixValue = (int)( SERVER_PERCENT_FLOAT - SERVER_PERCENT_FLOAT * tEquipmentPer * tBuffPer * tSkillPer * tCorrect ); 
		
	return tFixValue;		
}

int CPropertyNPC::RefreshDecPerPDamage( )
{
	/*float tBuffPer = 1 - ( mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECDESPDAMAGEPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_INCDESPDAMAGEPER ) ) / SERVER_PERCENT_FLOAT;
		
	int tFixValue = (int)( SERVER_PERCENT_FLOAT - SERVER_PERCENT_FLOAT * tBuffPer ); */
	int tFixValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECDESPDAMAGEPER ) - mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_INCDESPDAMAGEPER );

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tFixValue;
	tFixValue += tpNpcTpl->mPReducePer;

	return tFixValue;
}

int CPropertyPet::RefreshDecPerPDamage( )
{
	/*float tBuffPer = 1 - ( mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECDESPDAMAGEPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_INCDESPDAMAGEPER ) ) / SERVER_PERCENT_FLOAT;

	int tFixValue = (int)( SERVER_PERCENT_FLOAT - SERVER_PERCENT_FLOAT * tBuffPer );*/ 
	int tFixValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECDESPDAMAGEPER ) - mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_INCDESPDAMAGEPER );

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tFixValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tFixValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tFixValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tFixValue;
		}		

		tFixValue += tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mPReducePer ;
	}

	return tFixValue;
}

int CPropertyNPC::RefreshNormalHitRate( )
{
	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return 0;
	int tTplHit = tpNpcTpl->mNormalHitrate;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		int tLevel = GetTranChangeLevel( );
		tTplHit = pTemplateOgreTran->mHit[ tLevel ];
	}

	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_NORMALHITRATEFIX );				// 得到Buff固定值修正
	int		tTotalValue	= tTplHit + tFixValue;
	tTotalValue			= std::max( tTotalValue, 0 );

	return tTotalValue;
}

int CPropertyNPC::RefreshSkillHitRate( )
{
	int tHitrate = mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_SKILLHITRATEFIX );
	return tHitrate;
}

int CPropertyPet::RefreshNormalHitRate( )
{
	int		tFixValue	= mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_NORMALHITRATEFIX );				// 得到Buff固定值修正
	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tFixValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tFixValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tFixValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tFixValue;
		}		

		tFixValue += tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mSkillDuck ;
	}
	int		tTotalValue	= tFixValue;
	tTotalValue			= std::max( tTotalValue, 0 );

	return tTotalValue;
}

int CPropertyPet::RefreshSkillHitRate( )
{
	int tHitrate = mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_SKILLHITRATEFIX );
	return tHitrate;
}

int CPropertyPlayer::RefreshDecPerMDamage( )
{
	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return 0;
	float tEquipmentPer = 1 - ( mEquipment.GetPerNumber( CTemplateProperty::PROPERTYDEC_MDAMAGEPER ) + mEquipment.GetSuitPer( CTemplateProperty::PROPERTYDEC_MDAMAGEPER ) ) / SERVER_PERCENT_FLOAT;
	float tBuffPer		= 1 - ( mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECDESMDAMAGEPER ) - mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_INCDESMDAMAGEPER ) ) / SERVER_PERCENT_FLOAT;
	float tSkillPer		= 1 - ( mSkill.GetPerNumber( CTemplateSkill::PASSINC_DAMAGELESSPER ) + mTalentSkill.GetPerNumber( CTemplateSkill::PASSINC_DAMAGELESSPER ) + MWGetPerNumber( CTemplateSkill::PASSINC_DAMAGELESSPER )
		+ mTitleSkill.GetPerNumber( CTemplateSkill::PASSINC_DAMAGELESSPER ) ) / SERVER_PERCENT_FLOAT;
	float tCorrect		= 1 - ( tpMetierTpl->mMReducePerSta * ( int )CPropertyFormula::GetSta( this ) ) / SERVER_PERCENT_FLOAT;

	int tFixValue = (int)( SERVER_PERCENT_FLOAT - SERVER_PERCENT_FLOAT * tEquipmentPer * tBuffPer * tSkillPer * tCorrect );

	return tFixValue;	
}

int CPropertyNPC::RefreshDecPerMDamage( )
{
	/*float tBuffPer = 1 - ( mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECDESMDAMAGEPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_INCDESMDAMAGEPER ) ) / SERVER_PERCENT_FLOAT;

	int tFixValue = (int)( SERVER_PERCENT_FLOAT - SERVER_PERCENT_FLOAT * tBuffPer );*/
	int tFixValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECDESMDAMAGEPER ) - mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_INCDESMDAMAGEPER );

	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return tFixValue;
	tFixValue += tpNpcTpl->mMReducePer;

	return tFixValue;
}

int CPropertyPet::RefreshDecPerMDamage( )
{
	/*float tBuffPer = 1 - ( mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECDESMDAMAGEPER ) 
		- mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_INCDESMDAMAGEPER ) ) / SERVER_PERCENT_FLOAT;

	int tFixValue = (int)( SERVER_PERCENT_FLOAT - SERVER_PERCENT_FLOAT * tBuffPer );*/
	int tFixValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_DECDESMDAMAGEPER ) - mBuffList.GetPerNumber( CTemplateBuff::BUFFINC_INCDESMDAMAGEPER );

	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return tFixValue;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tFixValue;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return tFixValue;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return tFixValue;
		}		

		tFixValue += tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mMReducePer ;
	}

	return tFixValue;
}

int CPropertyCharacter::RefreshStealHP( )
{
	int tFixValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFFUNC_STEALHP );

	return tFixValue;
}

int CPropertyCharacter::RefreshStealMP( )
{
	int tFixValue = mBuffList.GetPerNumber( CTemplateBuff::BUFFFUNC_STEALMP );

	return tFixValue;
}

int CPropertyPlayer::RefreshStealHP( )
{
	int tFixValue = mEquipment.GetPerNumber( CTemplateProperty::PROPERTYFUNC_HPABSORB ) + mEquipment.GetSuitPer( CTemplateProperty::PROPERTYFUNC_HPABSORB ) +
		mBuffList.GetPerNumber( CTemplateBuff::BUFFFUNC_STEALHP );

	return tFixValue;
}

int CPropertyPlayer::RefreshStealMP( )
{
	int tFixValue = mEquipment.GetPerNumber( CTemplateProperty::PROPERTYFUNC_MPABSORB ) + mEquipment.GetSuitPer( CTemplateProperty::PROPERTYFUNC_MPABSORB ) +
		mBuffList.GetPerNumber( CTemplateBuff::BUFFFUNC_STEALMP );

	return tFixValue;
}

int CPropertyPlayer::RefreshNormalHitRate( )
{
	int tFixValue = 0;
	tFixValue += mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_NORMALHITRATE )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_NORMALHITRATE );// 得到装备固定值修正

	tFixValue += mSkill.GetFixNumber( CTemplateSkill::PASSINC_NORMALHITRATE )
		+ mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_NORMALHITRATE )
		+ MWGetFixNumber( CTemplateSkill::PASSINC_NORMALHITRATE )
		+ mTitleSkill.GetFixNumber( CTemplateSkill::PASSINC_NORMALHITRATE );	// 得到技能固定值修正

	tFixValue += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_NORMALHITRATEFIX );	// 得到BUFF固定值修正

	CTemplateMetier* tpMetierTpl = (CTemplateMetier*) CDataStatic::SearchMetier( GetPropertyTempID() );
	if ( tpMetierTpl == NULL )
		return 0;

	// 得到基本属性修正
	int tLevelAmend = (int)( ( mLevel - tpMetierTpl->mInitLevel ) * tpMetierTpl->mDeltaNormalHitrate );

	int tCorrect		= (int)( tpMetierTpl->mNormalHitratePerStr * ( int )CPropertyFormula::GetStr( this )
		                       + tpMetierTpl->mNormalHitratePerCon * ( int )CPropertyFormula::GetCon( this )
		                       + tpMetierTpl->mNormalHitratePerWis * ( int )CPropertyFormula::GetWis( this )
		                       + tpMetierTpl->mNormalHitratePerSpr * ( int )CPropertyFormula::GetSpr( this )
		                       );

	int tTotalValue	= tpMetierTpl->mInitNormalHitrate + tLevelAmend + tFixValue + tCorrect;

	return tTotalValue;
}

int CPropertyPlayer::RefreshSkillHitRate( )
{
	int tHitrate = mEquipment.GetFixNumber( CTemplateProperty::PROPERTYINC_SKILLHITRATE )
		+ mEquipment.GetSuitInc( CTemplateProperty::PROPERTYINC_SKILLHITRATE )
		+ mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_SKILLHITRATEFIX );
	return tHitrate;
}

int CPropertyPlayer::RefreshResDef( int *vDef )
{	
	for( int i = RES_TYPE1; i < RES_NUM; i++ )
	{
		int tDef = 0;
		tDef += mEquipment.GetFixNumber( (CTemplateProperty::PROPERTY_FUNC)( (int)( CTemplateProperty::PROPERTYFUNC_RESDEF1 ) + i - 1 ) );
		tDef += mBuffList.GetFixNumber( (CTemplateBuff::BUFF_FUN_TYPE)( (int)( CTemplateBuff::BUFFINC_RESDEF1 ) + i - 1 ) );
		tDef += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_RESDEFALL );
		tDef += mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_RESDEF );
		vDef[ i ] = tDef;
	}
	return 0;
}

int CPropertyPlayer::RefreshResAtt( int *vAtt )
{	
	for( int i = RES_TYPE1; i < RES_NUM; i++ )
	{
		int tAtt = 0;
		tAtt += mEquipment.GetFixNumber( (CTemplateProperty::PROPERTY_FUNC)( (int)( CTemplateProperty::PROPERTYFUNC_RESATT1 ) + i - 1 ), 0, this );
		tAtt += mBuffList.GetFixNumber( (CTemplateBuff::BUFF_FUN_TYPE)( (int)( CTemplateBuff::BUFFINC_RESATT1 ) + i - 1 ) );
		tAtt += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_RESATTALL );
		tAtt += mTalentSkill.GetFixNumber( CTemplateSkill::PASSINC_RESATT );
		vAtt[ i ] = tAtt;
	}
	return 0;
}

int CPropertyNPC::RefreshResDef( int *vDef )
{
	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return 0;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );	
	for( int i = RES_TYPE1; i < RES_NUM; i++ )
	{
		int tDef = 0;
		if ( pTemplateOgreTran != NULL )
		{
			int tTranChangeLevel = GetTranChangeLevel( );
			tDef += pTemplateOgreTran->mResDef[ tTranChangeLevel ][ i ];
		}
		else
		{
			tDef += tpNpcTpl->mResDef[ i - 1 ];
		}
		
		tDef += mBuffList.GetFixNumber( (CTemplateBuff::BUFF_FUN_TYPE)( (int)( CTemplateBuff::BUFFINC_RESDEF1 ) + i - 1 ) );
		tDef += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_RESDEFALL );
		vDef[ i ] = tDef;
	}
	return 0;
}

int CPropertyNPC::RefreshResAtt( int *vAtt )
{
	CTemplateOgre* tpNpcTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
	if ( tpNpcTpl == NULL )
		return 0;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*)CDataStatic::SearchTpl( tpNpcTpl->mTranID );	
	for( int i = RES_TYPE1; i < RES_NUM; i++ )
	{
		int tAtt = 0;
		if ( pTemplateOgreTran != NULL )
		{
			int tLevel = GetTranChangeLevel( );
			tAtt += pTemplateOgreTran->mResAtt[ tLevel ][ i ];
		}
		else
		{
			tAtt += tpNpcTpl->mResAtt[ i - 1 ];
		}
		tAtt += mBuffList.GetFixNumber( (CTemplateBuff::BUFF_FUN_TYPE)( (int)( CTemplateBuff::BUFFINC_RESATT1 ) + i - 1 ) );
		tAtt += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_RESATTALL );
		vAtt[ i ] = tAtt;
	}
	return 0;
}

int CPropertyPet::RefreshResDef( int *vDef )
{
	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return 0;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return 0;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return 0;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return 0;
		}

		for( int i = RES_TYPE1; i < RES_NUM; i++ )
		{
			int tDef = 0;
			tDef += tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mResDef[ i - 1 ];
			tDef += mBuffList.GetFixNumber( (CTemplateBuff::BUFF_FUN_TYPE)( (int)( CTemplateBuff::BUFFINC_RESDEF1 ) + i - 1 ) );
			tDef += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_RESDEFALL );
			vDef[ i ] = tDef;
		}
	}
	return 0;
}

int CPropertyPet::RefreshResAtt( int *vAtt )
{
	CEntityNpc* tpEntityNpc = (CEntityNpc*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	if ( tpEntityNpc == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntity is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return 0;
	}

	if ( tpEntityNpc->GetEntityType( ) == ENTITYTYPE_PET )
	{
		CEntityPet* tpEntityPet = (CEntityPet*) tpEntityNpc;
		if ( tpEntityPet == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpEntityPet is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return 0;
		}

		if ( 
			tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_ONE 
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_TWO
			&& tpEntityPet->GetCurLevel( ) != CTemplateSummonBeast::PET_LEVEL_THREE
			)
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] mCurLevel of tpEntityPet is %d!\n", __LK_FILE__, __LINE__, __FUNCTION__, tpEntityPet->GetCurLevel( ) );
			return 0;
		}

		CTemplateSummonBeast* tpPetTpl = (CTemplateSummonBeast*) CDataStatic::SearchTpl( GetPropertyTempID( ) );
		if ( tpPetTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] tpPetTpl is NULL!\n", __LK_FILE__, __LINE__, __FUNCTION__ );
			return 0;
		}

		for( int i = RES_TYPE1; i < RES_NUM; i++ )
		{
			int tAtt = 0;
			tAtt += tpPetTpl->mPetData[ tpEntityPet->GetCurLevel( ) ].mResAtt[ i - 1 ];
			tAtt += mBuffList.GetFixNumber( (CTemplateBuff::BUFF_FUN_TYPE)( (int)( CTemplateBuff::BUFFINC_RESATT1 ) + i - 1 ) );
			tAtt += mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_RESATTALL );
			vAtt[ i ] = tAtt;
		}
	}
	return 0;
}

int CPropertyCharacter::RefreshEquipWizard( int *vWizard )
{
	return 0;
}

int CPropertyPlayer::RefreshEquipWizard( int *vWizard )
{
	CPropertyValue tValue;

	int tFixEquip = 0;

	for ( int i = 0; i < WIZARD_COMMON; i++ )
	{
		tFixEquip = mEquipment.GetFixNumber( (CTemplateProperty::PROPERTY_FUNC)( (int)CTemplateProperty::PROPERTYINC_WIZARD1 + i ) ) + mEquipment.GetSuitInc( 
			(CTemplateProperty::PROPERTY_FUNC)( (int)CTemplateProperty::PROPERTYINC_WIZARD1 + i ) );
		vWizard[ i ] = tFixEquip;
	}

	return 0;
}

// 刷新技能列表
void CPropertyPlayer::RefreshSkillList( )
{
	RefreshSkillListAction(&mEquipSkill);		// 装备技能列表
	RefreshSkillListAction(&mSkill);			// 技能列表
	RefreshSkillListAction(&mTalentSkill);		// 造化技能
	RefreshSkillListAction(&mTitleSkill);		// 称号技能
	
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
	if ( tpMW != NULL )
		RefreshSkillListAction(tpMW->GetMWSkillList());		// 法宝技能
}

void CPropertyPlayer::RefreshSkillListAction( CSkillList* pSkillList )
{
	// 预先要清空一次
	for ( int i = 0; i < pSkillList->GetSkillNum(); i++ )
	{
		CSkillObject* pSkillObj = pSkillList->GetSkillObjectByIndex( i );

		int tIncLevel = GetIncSkillLevel( pSkillObj->GetSkillID( ) );
		int tTotalLevel = pSkillObj->GetSkillLevel( );
		// 如果两次的增加等级不同，修改
		if ( (unsigned int)tIncLevel != pSkillObj->GetIncLevel( ) )
		{
			tTotalLevel -= pSkillObj->GetIncLevel( );
			tTotalLevel += tIncLevel;
			pSkillObj->SetSkillLevel( tTotalLevel );
			pSkillObj->SetIncLevel( tIncLevel );
		}		
		pSkillObj->SetReduceCoolFix( 0 );
		pSkillObj->SetDeathRateFix( 0 );
		pSkillObj->SetDeathValueFix( 0 );
	}

	for ( int i = 0; i < pSkillList->GetSkillNum(); i++ )
	{	
		CSkillObject* pSkillObj = pSkillList->GetSkillObjectByIndex( i );

		CTemplateSkill* tpSkillTempl = pSkillObj->GetSkillTpl();
		if ( tpSkillTempl == NULL )
		{
			continue;																					
		}

		// 初始赋值原始冷却时间
		pSkillObj->SetOriginalCooldown( tpSkillTempl->mCDTime );

		if ( tpSkillTempl->mParameter1[ 0 ] != 0 )
		{
			CSkillObject *pObject = pSkillList->GetSkillObjectByID( tpSkillTempl->mParameter1[ 0 ] );
			if ( pObject != NULL )
			{
				if ( tpSkillTempl->mSkillFuncID1 == CTemplateSkill::PASSINC_REDUCECOOLFIX )
				{
					pObject->SetReduceCoolFix( pObject->GetReduceCoolFix() + tpSkillTempl->mParameter1[ 1 ] );
				}
				else if ( tpSkillTempl->mSkillFuncID1 == CTemplateSkill::PASSINC_DEATHRATEFIX )
				{
					pObject->SetDeathRateFix( pObject->GetDeathRateFix() + tpSkillTempl->mParameter1[ 1 ] );
				}
				else if ( tpSkillTempl->mSkillFuncID1 == CTemplateSkill::PASSINC_DEATHVALUEFIX )
				{
					pObject->SetDeathValueFix( pObject->GetDeathValueFix() +  tpSkillTempl->mParameter1[ 1 ] );
				}
			}
		}	
		else if ( tpSkillTempl->mParameter2[ 0 ] != 0 )
		{
			CSkillObject *pObject = pSkillList->GetSkillObjectByID( tpSkillTempl->mParameter2[ 0 ] );
			if ( pObject != NULL )
			{
				if ( tpSkillTempl->mSkillFuncID2 == CTemplateSkill::PASSINC_REDUCECOOLFIX )
				{
					pObject->SetReduceCoolFix( pObject->GetReduceCoolFix() + tpSkillTempl->mParameter2[ 1 ] );
				}
				else if ( tpSkillTempl->mSkillFuncID2 == CTemplateSkill::PASSINC_DEATHRATEFIX )
				{
					pObject->SetDeathRateFix( pObject->GetDeathRateFix() + tpSkillTempl->mParameter2[ 1 ] );
				}
				else if ( tpSkillTempl->mSkillFuncID2 == CTemplateSkill::PASSINC_DEATHVALUEFIX )
				{
					pObject->SetDeathValueFix( pObject->GetDeathValueFix() +  tpSkillTempl->mParameter2[ 1 ] );
				}
			}
		}
		else if ( tpSkillTempl->mParameter3[ 0 ] != 0 )
		{
			CSkillObject *pObject = pSkillList->GetSkillObjectByID( tpSkillTempl->mParameter3[ 0 ] );
			if ( pObject != NULL )
			{
				if ( tpSkillTempl->mSkillFuncID3 == CTemplateSkill::PASSINC_REDUCECOOLFIX )
				{
					pObject->SetReduceCoolFix( pObject->GetReduceCoolFix() + tpSkillTempl->mParameter3[ 1 ] );
				}
				else if ( tpSkillTempl->mSkillFuncID3 == CTemplateSkill::PASSINC_DEATHRATEFIX )
				{
					pObject->SetDeathRateFix( pObject->GetDeathRateFix() + tpSkillTempl->mParameter3[ 1 ] );
				}
				else if ( tpSkillTempl->mSkillFuncID3 == CTemplateSkill::PASSINC_DEATHVALUEFIX )
				{
					pObject->SetDeathValueFix( pObject->GetDeathValueFix() +  tpSkillTempl->mParameter3[ 1 ] );
				}
			}
		}
		else if ( tpSkillTempl->mParameter4[ 0 ] != 0 )
		{
			CSkillObject *pObject = pSkillList->GetSkillObjectByID( tpSkillTempl->mParameter4[ 0 ] );
			if ( pObject != NULL )
			{
				if ( tpSkillTempl->mSkillFuncID4 == CTemplateSkill::PASSINC_REDUCECOOLFIX )
				{
					pObject->SetReduceCoolFix( pObject->GetReduceCoolFix() + tpSkillTempl->mParameter4[ 1 ] );
				}
				else if ( tpSkillTempl->mSkillFuncID4 == CTemplateSkill::PASSINC_DEATHRATEFIX )
				{
					pObject->SetDeathRateFix( pObject->GetDeathRateFix() + tpSkillTempl->mParameter4[ 1 ] );
				}
				else if ( tpSkillTempl->mSkillFuncID4 == CTemplateSkill::PASSINC_DEATHVALUEFIX )
				{
					pObject->SetDeathValueFix( pObject->GetDeathValueFix() +  tpSkillTempl->mParameter4[ 1 ] );
				}
			}
		}

		pSkillObj->SetReduceCoolFix( pSkillObj->GetReduceCoolFix() + 
			mEquipment.GetValueByID( pSkillObj->GetSkillID( ), CTemplateProperty::PROPERTYFUNC_REDUCECOOLFIX ) );
		pSkillObj->SetDeathRateFix( pSkillObj->GetDeathRateFix() + 
			mEquipment.GetValueByID( pSkillObj->GetSkillID( ), CTemplateProperty::PROPERTYFUNC_DEATHRATEFIX ) );
		pSkillObj->SetDeathValueFix( pSkillObj->GetDeathValueFix() +
			mEquipment.GetValueByID( pSkillObj->GetSkillID( ), CTemplateProperty::PROPERTYFUNC_DEATHVALUEFIX ) );
	}
}

void CPropertyPlayer::RefreshStateList( )
{
	mAttackList.clear();
	mDefenceList.clear();
	mDeathList.clear();
	mReduceHPList.clear();
	mEquipment.GetStateID( mAttackList, CTemplateProperty::PROPERTYFUNC_HITSKILL );
	mSkill.GetStateID(  mAttackList, CTemplateSkill::PASSINC_HITSKILL );
	mTitleSkill.GetStateID(  mAttackList, CTemplateSkill::PASSINC_HITSKILL );
	mEquipment.GetStateID( mDefenceList, CTemplateProperty::PROPERTYFUNC_BEHITSKILL );
	mSkill.GetStateID( mDefenceList, CTemplateSkill::PASSINC_BEHITSKILL );
	mTitleSkill.GetStateID( mDefenceList, CTemplateSkill::PASSINC_BEHITSKILL );
	mEquipment.GetStateID( mDeathList, CTemplateProperty::PROPERTYFUNC_DEATHSKILL );
	mSkill.GetStateID( mDeathList, CTemplateSkill::PASSINC_DEATHSKILL );
	mTitleSkill.GetStateID( mDeathList, CTemplateSkill::PASSINC_DEATHSKILL );
	mEquipment.GetStateID( mReduceHPList, CTemplateProperty::PROPERTYFUNC_HPSKILL );
	mSkill.GetStateID(  mReduceHPList, CTemplateSkill::PASSINC_HPSKILL );
	mTitleSkill.GetStateID( mReduceHPList, CTemplateSkill::PASSINC_HPSKILL );

	mTalentSkill.GetStateID( mAttackList, CTemplateSkill::PASSINC_HITSKILL );
	mTalentSkill.GetStateID( mDefenceList, CTemplateSkill::PASSINC_BEHITSKILL );
	mTalentSkill.GetStateID( mDeathList, CTemplateSkill::PASSINC_DEATHSKILL );
	mTalentSkill.GetStateID(  mReduceHPList, CTemplateSkill::PASSINC_HPSKILL );
}


int CPropertyNPC::OnEntityDisappearTimeout( CEntity* pEntity )
{
	if ( pEntity == NULL )
		return 0;

	if ( pEntity->GetAliveStatus() == ALIVE_DIE )
	{
		if ( pEntity->IsPet() || pEntity->IsItem() )
		{
			// 删除实体的同时会干掉定时器
			CSceneLogic::GetSingleton().DestroyEntity( pEntity, 0 );
			return 1;
		}
		pEntity->SetAliveStatus( ALIVE_GHOST );
	}
	
	if ( pEntity->GetAliveStatus() == ALIVE_GHOST )
	{
		if ( pEntity->IsOgre() || pEntity->IsFunNpc() || pEntity->IsBox() || pEntity->IsBlock() )
		{
			COgreCreator *pCreator = (COgreCreator*)CSceneObjManager::GetSingletonPtr()->GetObject( pEntity->GetCreatorID( ) );

			if ( pCreator == NULL )
			{
				// 找不到creator, 删除实体 同时会干掉定时器
				CSceneLogic::GetSingleton().DestroyEntity( pEntity, 0 );
				return 1;
			}

			// 如果是开始配置不刷新，删除实体 同时会干掉定时器
			if ( pCreator->mInit == 0 || GetRelifeTime( ) == 0 )
			{
				// 找不到creator, 删除实体
				CSceneLogic::GetSingleton().DestroyEntity( pEntity, 0 );
				pCreator->mHasOgre = 0;
				return 1;
			}
		}
		
		CMessage tEntityDisappear;
		CMessageEntityDisappearNotice tMessage;

		CPropertyModule::GetSingleton().CreateMessageEntityDisappearNotice(
			&tEntityDisappear,
			pEntity,
			&tMessage
			);
		
		// 如果正在处于复活阶段
		SetRelifeTimer( GetRelifeTime( ) );	
		CMapModule::GetSingletonPtr()->MessageBroadCast( pEntity, &tEntityDisappear );

		if( pEntity->IsFunNpc() || pEntity->IsOgre() || pEntity->IsCollect() || pEntity->IsBlock() )
		{
			int nTimerOtherInfo[ TIMER_OTHER_INFO ] = { MESSAGE_PROPERTY, 0, 0, 0}  ;
			pEntity->GetTimerReciver()->SetTimer( pEntity->get_id(), TIMER_ENTITY_RELIVE, GetRelifeTime( ), 1, nTimerOtherInfo );
		
			if ( ( (CEntityNpc*) pEntity )->GetNpcChangeState() == 1 ) // 如果是变怪NPC 记日志
			{
				LOG_NOTICE("default", "***ChangedNpcDT NPCID=%d entityid=%d",  pEntity->GetNpcID(), pEntity->GetEntityID() );
			}
			if ( pEntity->IsOgre( ) )
			{
				// 如果是BOSS,通知提前刷新
				CTemplateOgre *tplOgre = (CTemplateOgre*)CDataStatic::SearchTpl( pEntity->GetNpcID( ) );
				if ( tplOgre != NULL && tplOgre->mDisplayLevel == CTemplateOgre::OGRESHOW_BOSS )
				{
					pEntity->GetTimerReciver()->SetTimer( pEntity->get_id(), TIMER_NOTIFYREFRESH_TEN, GetRelifeTime( ) - 600000, 1, nTimerOtherInfo );
					pEntity->GetTimerReciver()->SetTimer( pEntity->get_id(), TIMER_NOTIFYREFRESH_FIVE, GetRelifeTime( ) - 300000, 1,  nTimerOtherInfo );

					COgreCreator *pCreator = (COgreCreator*)CSceneObjManager::GetSingletonPtr()->GetObject( pEntity->GetCreatorID( ) );
					if ( pCreator != NULL )
						pCreator->mNextRefreshTime = time(NULL) + GetRelifeTime( ) / 1000;
				}
			}			
		}
	}

	return 0;
}


// 技能武器需求匹配
bool CPropertyPlayer::SkillWeaponValid( CTemplateSkill* pSkill )
{
	LK_ASSERT( pSkill != NULL, return false );

	// 判断技能攻击方式
	CItemObject* tpWeapon = GetEquipment( )->GetItemObjPtr( EQUIPMENT_WEAPON );
	if ( tpWeapon == NULL )
		return false;

	CTemplateWeapon* tpWeaponTpl = ( CTemplateWeapon* ) CDataStatic::SearchTpl( tpWeapon->GetItemID() );
	if ( tpWeaponTpl == NULL )
		return false;     	

	// 如果技能需求武器不满足要求
	if ( pSkill->mRequireWeapon != CTemplateWeapon::WEAPON_UNLIMIT
		&& tpWeaponTpl->mWeaponType != CTemplateWeapon::WEAPON_UNLIMIT
		&& pSkill->mRequireWeapon != tpWeaponTpl->mWeaponType )
	{
		return false;
	}

	return true;
}

// 装备是否可用 ( 耐久度是否为0 )
bool CPropertyPlayer::EquipmentAvailable( EEquipPart emPart )
{
	CItemEquipment* tpItemObjPtr = (CItemEquipment*) GetEquipment( )->GetItemObjPtr( (int)emPart );
	if ( tpItemObjPtr == NULL )
		return false;

	// 装备是否损坏
	if ( tpItemObjPtr->GetDurability( )  == 0  )
		return false;

	return true;
}

// 获得有效的双倍时间
int CPropertyPlayer::GetValidDiploid()
{	
	if ( UnFreezeDouble() )
	{
		// 领双，未冻结
		if ( mDiploidState[ TIMEID_STATE ] != INVALID_OBJ_ID )
		{
			CTimerMng::GetSingleton().ForceTimeOut( mDiploidState[ TIMEID_STATE ] );
		}
	}	
	return mDiploidState[ LEFTVALID_STATE ];
}

bool CPropertyPlayer::IsMoneyEnough( bool bBind, int vMoney )
{
	bool bRes = false;
	if ( bBind )
	{
		unsigned int tTotalMoney = ( unsigned int )( mMoney + mBindMoney );
		bRes = ( ( tTotalMoney < (unsigned int)vMoney ) ? false : true );
	}
	else 
	{
		bRes = ( ( mMoney < vMoney ) ? false : true );
	}
	return bRes;
}

//**********************************
// 玩家被攻击时，检查是否处于保护状态
// 1、30级下不可以被攻击
// 2、摆摊时不可以被攻击
//**********************************

bool CPropertyPlayer::CanAttacked()
{
	bool bRes = false;

	if ( mServiceStatus == SERVICE_STALL )
	{
		bRes = false;	
	}
	else
	{
		bRes = ( GetLevel() < 30 ) ? false : true;
	}
	return bRes;
}

//**********************************
// 玩家是否可以传送
// 1、交易、摆摊时，不可以传送
// 2、灵石状态不能传送
//**********************************

bool CPropertyPlayer::CanTeleport()
{
	bool bRes = false;
	if ( mServiceStatus == SERVICE_STALL 
		|| mServiceStatus == SERVICE_TRADE 
		|| mStateParam[ STATE_STONE ].mInState == true )
	{
		bRes = false;
	}
	else 
	{
		bRes = true;
	}
	return bRes;
}

// 跨地图,坐骑处理
void CPropertyPlayer::ChangeHorseStateTransMap( unsigned short unMapID )
{
	if ( GetHorseState( ) != 0 )
	{
		// 如果玩家在骑乘状态,那么判断该地图是否能够骑乘,如果不能,则下马
		CMapTpl *tpTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( unMapID );
		if( tpTpl == NULL )
			return;
		CTplItem* tpItem = (CTplItem*) CDataStatic::SearchTpl( GetHorseTempID( ) );
		if ( tpItem == NULL )
			return;

		bool bOffHorse = false;
		if ( tpTpl->mBasRide == false && ( tpItem->mProcessType & CTplItem::PROCESSTYPE_ISRMBITEM ) != CTplItem::PROCESSTYPE_ISRMBITEM ) 
		{
			// 不允许低级坐骑,切坐骑非人民币道具,下马处理
			bOffHorse = true;
		}
		else if ( tpTpl->mAdvRide == false && ( tpItem->mProcessType & CTplItem::PROCESSTYPE_ISRMBITEM ) == CTplItem::PROCESSTYPE_ISRMBITEM ) 
		{
			// 不允许高级坐骑,且坐骑是人民币道具,下马处理
			bOffHorse = true;
		}
		else if ( tpTpl->mFlyRide == false && mStateParam[ STATE_FLY ].mInState == true )
		{
			bOffHorse = true;
		}
		if ( bOffHorse == true )
		{	
			CEntity* tpEntity = CSceneLogic::GetSingleton( ).GetEntity( GetEntityID( ) );
			if ( tpEntity == NULL )
				return;
				
			if ( tpEntity->IsPlayer() == false )
			{
				return;
			}										
			
			CEntityPlayer *tpPlayer = ( CEntityPlayer * )tpEntity;
			CMapObject *tpMapObj = CMapObjManager::GetSingleton().GetMapObject( tpPlayer->GetLineID(), tpPlayer->GetMapID(), tpPlayer->GetMapIndex() );
			if ( tpMapObj == NULL )
			{
				return;
			}

			// 地宫不能用
			if ( tpMapObj->IsGroundMap() == true && IsVip() == true && mStateParam[ STATE_FLY ].mInState == false )
			{
				return;
			}					
							 
			CTplItemHorse* pTplHorse = (CTplItemHorse*)tpItem;
			OffHorse( pTplHorse->mSpeedBuff );		
			
			static CMessage tMessage;
			static CMessageRideHorseNotice tRideHorse;

			tMessage.mutable_msghead( )->set_messageid( ID_S2C_NOTIFY_RIDEHORSE );

			tRideHorse.set_entityid( GetEntityID( ) );
			tRideHorse.set_horsetempid( GetHorseTempID( ) );
			tRideHorse.set_horseresult( 0 );
			tRideHorse.set_speed( GetCurSpeed( ) );

			tMessage.set_msgpara( (uint32)&tRideHorse );
			
			// 广播
			CMapModule::GetSingletonPtr()->MessageBroadCast( tpEntity, &tMessage, false, true );
		}
	}
}

// 加入自身状态buff
void CPropertyPlayer::InsertSelfBuff( CEntityCharacter *pDesEntity, CTemplateProperty::PROPERTY_FUNC vType, int tOldPer, int tNewPer )
{
	int tNow = time( NULL );	
	int *tStateTime = NULL;
	StateList *pList = NULL;
	if ( vType == CTemplateProperty::PROPERTYFUNC_HITSKILL )
	{
		tStateTime = &mAttackStateTime;
		pList = &mAttackList;
	}
	else if ( vType == CTemplateProperty::PROPERTYFUNC_BEHITSKILL )
	{
		tStateTime = &mDefenceStateTime;
		pList = &mDefenceList;
	}
	else if ( vType == CTemplateProperty::PROPERTYFUNC_DEATHSKILL )
	{
		tStateTime = &mDeathStateTime;
		pList = &mDeathList;
	}
	else if ( vType == CTemplateProperty::PROPERTYFUNC_HPSKILL )
	{
		tStateTime = &mReduceHPStateTime;
		pList = &mReduceHPList;
	}
	if ( *tStateTime > tNow )
	{
		return;
	}
	int tMaxTime = 0;
	// 自身添加状态
	int tSelfList[ STATENUM ] = {0};
	int tSelfNum = 0;
	// 目标添加状态
	int tOtherList[ STATENUM ] = {0};
	int tOtherNum = 0;
	StateList::iterator it;
	for ( it = pList->begin(); it != pList->end(); it++ )
	{
		// 如果有减血功能并且HP不符合范围。继续
		if ( (*it).mHP != 0 && !( (*it).mHP < tOldPer && (*it).mHP > tNewPer ) )
			continue;
		int tRand = RAND(SERVER_PERCENT_INT);
		if ( tRand < (*it).mRand )
		{
			if ( (*it).mTarget == 0 )
			{
				if( tSelfNum >= STATENUM )
					continue;
				tSelfList[ tSelfNum ] = (*it).mBuffID;
				tSelfNum++;	
				if ( (*it).mTime > tMaxTime )
					tMaxTime = (*it).mTime;
			}
			else if ( (*it).mTarget == 1 )
			{
				if( tOtherNum >= STATENUM )
					continue;
				tOtherList[ tOtherNum ] = (*it).mBuffID;
				tOtherNum++;	
				if ( (*it).mTime > tMaxTime )
					tMaxTime = (*it).mTime;
			}				 
		}
	}

	if ( tSelfNum > 0 )
	{
		CPropertyModule::GetSingletonPtr( )->EntityBuffSkill( (CEntityCharacter*)GetSelfEntity( ), (CEntityCharacter*)GetSelfEntity( ), tSelfList, tSelfNum, 0 );
	}
	if ( tOtherNum > 0 )
	{
		CPropertyModule::GetSingletonPtr( )->EntityBuffSkill( (CEntityCharacter*)GetSelfEntity( ), pDesEntity, tOtherList, tOtherNum, 0 );
	}
	if ( tMaxTime != 0 )
	{
		*tStateTime = tNow + tMaxTime;
	}
}

EPKStatus CPropertyPlayer::GetTplPKState()
{
	CTplPKValueTable* tpPKTable = ( CTplPKValueTable* ) CDataStatic::GetTemp( TEMP_PKVALUE );
	if ( tpPKTable == NULL )
	{
		LOG_ERROR( "pro", "get template point failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return PKSTATUS_WHITE;
	}

	if ( mPKValue < ( tpPKTable->mCarmine + 1 ) )
	{
		return PKSTATUS_PKTOPLIMIT;
	}
	else if ( mPKValue > ( tpPKTable->mCarmine ) && mPKValue < tpPKTable->mRed )
	{
		return PKSTATUS_CARMINE;
	}
	else if ( mPKValue > ( tpPKTable->mRed - 1 ) && mPKValue < tpPKTable->mRosiness )
	{
		return PKSTATUS_RED;
	}
	else if ( mPKValue > ( tpPKTable->mRosiness - 1 ) && mPKValue < tpPKTable->mWhite )
	{
		return PKSTATUS_ROSINESS;
	}
	else if ( mPKValue > ( tpPKTable->mWhite - 1 ) && mPKValue <= 0 )
	{
		return PKSTATUS_WHITE;
	}

	return PKSTATUS_WHITE;
}

bool CPropertyPlayer::IsRedPKStatus()
{
	CTplPKValueTable* tpPKTable = ( CTplPKValueTable* ) CDataStatic::GetTemp( TEMP_PKVALUE );
	if ( tpPKTable == NULL )
	{
		LOG_ERROR( "pro", "get template point failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return false;
	}

	return ( ( mPKValue < tpPKTable->mWhite ) ? true : false );
}

bool CPropertyPlayer::IsWhitePKStatus()
{
	CTplPKValueTable* tpPKTable = ( CTplPKValueTable* ) CDataStatic::GetTemp( TEMP_PKVALUE );
	if ( tpPKTable == NULL )
	{
		LOG_ERROR( "pro", "get template point failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return false;
	}

	if ( mPKValue >= tpPKTable->mWhite && mPKValue <= 0 && !GetYellowStatus() )
		return true;

	return false;
}

EPKStatus CPropertyPlayer::GetMainPKState()
{
	CTplPKValueTable* tpPKTable = ( CTplPKValueTable* ) CDataStatic::GetTemp( TEMP_PKVALUE );
	if ( tpPKTable == NULL )
	{
		LOG_ERROR( "pro", "get template point failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return PKSTATUS_WHITE;
	}

	if ( mPKValue > ( tpPKTable->mCarmine - 1 ) && mPKValue < tpPKTable->mWhite )
	{
		return PKSTATUS_RED;
	}
	
	if ( mPKValue >= tpPKTable->mWhite && mPKValue <= 0 )
	{
		return PKSTATUS_WHITE;
	}
	 
	return PKSTATUS_WHITE;
}

void CPropertyPlayer::IncreasePKValue(int nValue)
{
	EPKStatus tOldStatus = GetTplPKState();
	mPKValue += nValue;
	EPKStatus tNewStatus = GetTplPKState();

	if ( tOldStatus != tNewStatus )
	{
		CEntityPlayer* tpPlayer = ( CEntityPlayer* ) this->GetSelfEntity();
		CMessage tMessage;
		CMessageNotifyModifyPKValue tPara;
		tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MODIFYPKVALUE );
		tMessage.set_msgpara( (long) &tPara );
		tPara.set_value( mPKValue );
		tPara.set_state( tNewStatus );
		tPara.set_entityid( tpPlayer->GetEntityID() );
		CMapModule::GetSingleton().MessageBroadCast( tpPlayer, &tMessage, false, true );
	}

	CTplPKValueTable* tpTable = ( CTplPKValueTable* ) CDataStatic::GetTemp( TEMP_PKVALUE );
	if ( tpTable == NULL )
	{
		return ;
	}

	if ( mPKValue <= tpTable->mCarmine )
	{
		// teleport
		SetPKValue( tpTable->mCarmine );
		CEntityPlayer* tpPlayer = ( CEntityPlayer* ) GetSelfEntity();
		bool bTeleRes = false;
		int tPrisonTime = GetPrisonTime( );
		int tTotalPrisonTime = GetTotalPrisonTime( );
		// set prison time as zero, begin to increase the time
		SetPrisonTime( 0 );
		SetTotalPrisonTime( tpTable->mPrisonTime );
		bTeleRes = CMapModule::GetSingleton().Teleport( tpPlayer->GetNationality(), tpTable->mMapID, tpTable->mPosX, tpTable->mPosY, tpPlayer );
		if ( bTeleRes )
		{
			// send prison time to client
			CPropertyModule::GetSingleton().SendPrisonTimeNotice( tpPlayer, tpTable->mPrisonTime );
		}
		else
		{
			SetPrisonTime( tPrisonTime );
			SetTotalPrisonTime( tTotalPrisonTime );
		}
	}

	if ( mPKValue >= 0 )
	{
		SetPKValue( 0 );
	}
	return ;
}

int CPropertyPlayer::GetIndexByID( int vID )
{
	for ( unsigned int i = 0; i < BOX_LIMIT; i++ )
	{
		CItemObject *tpObject = mBaggage.GetItemObjPtr( i );
		if ( tpObject == NULL )
		{
			continue;
		}
		if ( tpObject->get_id() == vID )
		{
			return i;
		}
	}
	return -1;
}


// 生活技能
int CLifeSkill::Initialize()
{
	mLifeSkillType = LIFESKILL_NONE;
	mLifeSkillLevel = 0;
	mSpecializedDegree = 0;		
	mLifeSkillKind	= LIFESKILLKIND_NONE;
	mMultiComposeIDList.initailize();	
	return 0;
}

// 学习一个新配方
int CLifeSkill::StudyCompose( int nComposeID, int nGetWay, int &rPlayerAp, unsigned int &rPlayerMoney )
{
	// 查看是否已经学习了改配方
	if ( CheckCompose( nComposeID ) )
	{
		return ERROR_LIFESKILL_COMPOSEHASEXIST;
	}	
	
	CTemplateMultiCompose *tpCompose = ( CTemplateMultiCompose * )CDataStatic::SearchTpl( nComposeID );
	
	// 配方是否存在
	if ( tpCompose == NULL )
	{
		return ERROR_LIFESKILL_COMPOSENOTEXIST;
	} 
	
	// 配方所属技能类型是否和自身技能类型相同
	if ( tpCompose->mLifeSkillType !=  mLifeSkillType )
	{
		return ERROR_LIFESKILL_WRONGLIFESKILL;
	} 
	
	// 检查熟练度是否已经满足条件
	if ( mSpecializedDegree < tpCompose->mSpecializedDegree )
	{
		return ERROR_LIFESKILL_LOWDEGREE;
	}
	
	// 查看配方是否获取类型是否正确
	if ( !( tpCompose->mLifeComposeType & nGetWay )  )
	{
		return ERROR_LIFESKILL_WRONGGETWAY;
	}
	
	// 判断玩家ap是否满足要求
	if ( tpCompose->mApNeedCost > rPlayerAp )
	{
		return ERROR_LIFESKILL_APNOTENOUGH;
	}
	
	// 判读玩家金钱是否满足要求
	if ( (unsigned int)tpCompose->mStudyMoney > rPlayerMoney )
	{
		return ERROR_LIFESKILL_COMPOSESTUDYMONEYNOTENOUGH;
	}
		
	
	ComposeInfo tCopmoseInfo;
	tCopmoseInfo.mTempID		= tpCompose->mTempID;
	tCopmoseInfo.mUsedTimes		= tpCompose->mUsedTimes;
	rPlayerAp		= tpCompose->mApNeedCost;
	rPlayerMoney	= tpCompose->mStudyMoney;
	
	// 学习技能
	if ( mMultiComposeIDList.size() < MAX_LIFESKILL_COMPOSENUM  )
	{
		mMultiComposeIDList.push_back( tCopmoseInfo );
	}
	
	// 如果是通过研究获取的配方要增加技能熟练度
	if ( nGetWay == COMPOSEGETWAY_BYCREAT )
	{
		AddSpecializedDegree( tpCompose->mSkillCreatDegree, DEGREEADDTYPE_BYSTUDY );
	}
	
	return SUCCESS;			
}

// 检查是否有某个配方
bool CLifeSkill::CheckCompose( int nComposeID )
{	
	if ( mMultiComposeIDList.empty() )
	{
		return false;
	}
	MultiComposeIDList::iterator it = mMultiComposeIDList.begin();
	for ( ; it != mMultiComposeIDList.end(); ++ it )
	{
		if ( it->mTempID == nComposeID )
		{
			return true;
		}								
	}
	return false;
} 	 

// 增加熟练度,返回实际增加的熟练度值
void CLifeSkill::AddSpecializedDegree( int nDegree, int nAddType )
{		
	switch( nAddType )	
	{
		case DEGREEADDTYPE_USECOMPOSE:
		{	
			if ( CheckDegree( mSpecializedDegree + nDegree ) )
			{
				mSpecializedDegree += nDegree;
			}
			else
			{
				mSpecializedDegree = GetCurrentDegreeLimit();
			}															
			break;
		}
		
		case DEGREEADDTYPE_BYLEVELUP:
		{
			mSpecializedDegree += nDegree;
			break;
		} 
		
		case DEGREEADDTYPE_BYITEM:
		{
			if ( CheckDegree( mSpecializedDegree + nDegree ) )
			{
				mSpecializedDegree += nDegree;
			}
			else
			{
				mSpecializedDegree = GetCurrentDegreeLimit();
			}		
			break;
		}
		
		case DEGREEADDTYPE_BYSTUDY:
		{
			if ( CheckDegree( mSpecializedDegree + nDegree ) )
			{
				mSpecializedDegree += nDegree;
			}
			else
			{
				mSpecializedDegree = GetCurrentDegreeLimit();
			}		
			break;
		}
		default:
			break;
	}
}

// 技能进阶
int CLifeSkill::LevelUpLifeSkill( int nPlayerLevel, int &nPlayerBindMoney, int &nPlayerMoney )
{  	
	CTemplateLifeSkill *tpLifeSkill = ( CTemplateLifeSkill * )CDataStatic::GetSkillLifeTable( mLifeSkillType );
	int tRet = CanLifeSkillLevelUp( tpLifeSkill );
	if ( tRet != SUCCESS )
	{
		return tRet;
	}			
	
	// 检查金钱和玩家等级
	if ( tpLifeSkill->mLevel[mLifeSkillLevel+1] > nPlayerLevel )
	{
		return ERROR_LIFESKILL_LEVELUPLEVELNOTENOUGH;				
	}
	
	if ( (unsigned int)( nPlayerBindMoney+nPlayerMoney ) < ( unsigned int )tpLifeSkill->mStudyMoney[mLifeSkillLevel+1] )
	{
		return ERROR_LIFESKILL_LEVELUPMONEYNOTENOUGH;
	}
	
	if ( tpLifeSkill->mStudyMoney[mLifeSkillLevel+1] > nPlayerBindMoney )
	{
	   nPlayerMoney = tpLifeSkill->mStudyMoney[mLifeSkillLevel+1] - nPlayerBindMoney; 
	}
	else
	{
		nPlayerBindMoney = tpLifeSkill->mStudyMoney[mLifeSkillLevel+1];
		nPlayerMoney	= 0;	
	}	
	
	++mLifeSkillLevel;
	mSpecializedDegree += tpLifeSkill->mDegreeValue[mLifeSkillLevel];
	return SUCCESS;
}

// 遗忘生活技能
int CLifeSkill::ForgetLifeSkill( )
{
	Initialize();
	return SUCCESS;
}

// 使用配方
int CLifeSkill::UseCompose( int nComposeID, int &rDegreeValue, bool bAddNow /* = true */ )
{
	rDegreeValue = 0;
	if ( !CheckCompose( nComposeID ) )
	{
		return ERROR_LIFESKILL_COMPOSENOTSTUDIED;
	}
	
	CTemplateMultiCompose *tpCompose = ( CTemplateMultiCompose * )CDataStatic::SearchTpl( nComposeID );
	if ( tpCompose == NULL )
	{
		return -1;
	}
	
	int tValue = mSpecializedDegree - tpCompose->mSpecializedDegree;
	if ( tValue < 0 )
	{
		return ERROR_LIFESKILL_COMPOSEDEGREENOTENOUGH;
	}
	
	int i = 0;
	int tOldValue = 0;
	for ( ; i < ( int )ARRAY_CNT(tpCompose->mDegreeInfo) - 1; ++i )
	{
		if ( tValue >= tOldValue && tValue <= tpCompose->mDegreeInfo[i].mValue )
		{
			break;
		}
		tOldValue = tpCompose->mDegreeInfo[i].mValue;
	}
	
	if ( tpCompose->mDegreeInfo[i].mProbality >= rand( )%SERVER_PERCENT_INT )
	{
		if( bAddNow )
		{
			AddSpecializedDegree( tpCompose->mDegreeInfo[i].mDegreeValue, DEGREEADDTYPE_USECOMPOSE );	
		}
		else
		{
			rDegreeValue = tpCompose->mDegreeInfo[i].mDegreeValue;
		}			
	}  		
	
	return SUCCESS;
}

// 获取本等级的熟练度上限
int CLifeSkill::GetCurrentDegreeLimit( )
{
	CTemplateLifeSkill *tpLifeSkill = ( CTemplateLifeSkill * )CDataStatic::GetSkillLifeTable( mLifeSkillType );
	if ( tpLifeSkill == NULL )
	{
		return 0;
	}	
	
	if ( mLifeSkillLevel >= MAX_LIFESKILL_LEVEL )
	{
		return 0;
	}
	
	return tpLifeSkill->mDegreeStarted[mLifeSkillLevel];	
}

// 检查熟练度是否已经超过当前技能等级的上限
bool CLifeSkill::CheckDegree( int nDegree )
{	
	int nCurrentDegreeLimit = GetCurrentDegreeLimit();
	if ( nDegree < nCurrentDegreeLimit )
	{
		return true;
	}
	return false;		
}


// 生活技能入门
int CLifeSkill::InitialLifeSkill( int nLifeSkillType, int nSkillKind )
{	
	// 是否已经学习
	if ( mLifeSkillType != LIFESKILL_NONE )
	{
		return ERROR_LIFESKILL_HASINITIALIZED;
	}	 									  	

	// 获取第0级的熟练度
	CTemplateLifeSkill *tpLifeSkill = ( CTemplateLifeSkill * )CDataStatic::GetSkillLifeTable( nLifeSkillType );
	if ( tpLifeSkill == NULL )
	{
		return ERROR_LIFESKILL_LIFESKILLTABLENOTEXIST;
	}																		  
	mSpecializedDegree = tpLifeSkill->mDegreeValue[0];
	mLifeSkillLevel		= 0;	
	mLifeSkillKind = nSkillKind;
	mLifeSkillType = nLifeSkillType;				
	return SUCCESS;
}


// 创造生活技能
int CLifeSkill::CreatCompose( int nValue, int *pMaterialIDList, int nLenth, int &rComposeID, int nSkillKind )
{  
	CLifeSkillInfo *tpLifeSkillInfo = ( CLifeSkillInfo * )CDataStatic::GetSkillLifeInfo( mLifeSkillType );
	if ( tpLifeSkillInfo == NULL )
	{
		return -1;
	}
	
	int tComposeID = 0;	
	int tComposeItemNum = 0;	
	
	for ( int i = 0; i < tpLifeSkillInfo->mComposeKindNum; ++i )
	{		
		// 先判断类别
		if ( tpLifeSkillInfo->mComposeInfo[i].mComposeType != nSkillKind )
		{
			continue;
		}
		
		// 再判断材料和熟练度
		for ( int j = 0; j < tpLifeSkillInfo->mComposeInfo[i].mComposeNum; ++j  )
		{
			CTemplateMultiCompose *tpCompose = (CTemplateMultiCompose *)CDataStatic::SearchTpl( tpLifeSkillInfo->mComposeInfo[i].mComposeID[j] );
			if ( tpCompose == NULL )
			{
				continue;			
			}

			int bIfEnd = false;	   		 
			if ( abs( tpCompose->mCreatValue - nValue ) <= tpCompose->mMinValue  )
			{		
				tComposeItemNum = 0;		
				for( int k = 0; k < (int)ARRAY_CNT( tpCompose->mMaterialID ) ; ++k )
				{
					bool bHasMatieral = false;					
					
					if ( tpCompose->mMaterialID[k] == 0  )
					{
						bHasMatieral = true;
					}
					else
					{
						for ( int tNum = 0; tNum < nLenth; ++tNum )
						{							
							if ( tpCompose->mMaterialID[k] == pMaterialIDList[tNum] )
							{
								bHasMatieral = true;									
								break;
							}																										
						}	
						++tComposeItemNum;
					} 						
						
					if ( !bHasMatieral )
					{
						break;
					} 
					
					if ( k == ARRAY_CNT( tpCompose->mMaterialID ) -1 && tComposeItemNum == nLenth )
					{  						
						bIfEnd = true;
						tComposeID = tpCompose->mTempID;
						break;
					}																	
				}
			}

			if ( bIfEnd )
			{
				break;
			}	
		} 				
	} 	
	
	if ( tComposeID <= 0 )
	{
		return ERROR_LIFESKILL_STUDYCOMPOSEFAILED;
	}
	
	rComposeID = tComposeID;
	int tAP		= 10000;
	unsigned int tMoney	= 100000;
	int tRet = StudyCompose( tComposeID, COMPOSEGETWAY_BYCREAT, tAP, tMoney );
	if ( tRet != ERROR_LIFESKILL_COMPOSEHASEXIST )
	{
		tRet = ERROR_LIFESKILL_STUDYCOMPOSEFAILED; 
	}
		
	return tRet;
}

// 检查是否可以升级
int CLifeSkill::CanLifeSkillLevelUp( CTemplateLifeSkill *pLifeSkill )
{
	/*CTemplateLifeSkill *pLifeSkill = ( CTemplateLifeSkill * )CDataStatic::GetSkillLifeTable( mLifeSkillType );
	if ( pLifeSkill == NULL )
	{
		return ERROR_LIFESKILL_LIFESKILLTABLENOTEXIST;
	}  */

	// 判断是否是否越界
	if ( mLifeSkillLevel + 1 >= (int)ARRAY_CNT( pLifeSkill->mStudyDegree ) )
	{
		return ERROR_LIFESKILL_CANNOTLEVELUP;
	}

	// 如果是辅修生活技能的话，等级有限制
	if ( mLifeSkillKind == LIEFSKILLKIND_SUBSKILL )
	{
		if ( mLifeSkillLevel >= pLifeSkill->mSubSkillLevelLimit )
		{
			return	ERROR_LIFESKILL_CANNOTLEVELUP;		// 到达技能上限不能再晋级
		}
	}


	// 下一级没有技能可学,不能升级		
	if ( mLifeSkillLevel > 0 && pLifeSkill->mStudyDegree[mLifeSkillLevel+1] == 0 )
	{
		return	ERROR_LIFESKILL_CANNOTLEVELUP;
	}

	// 只做技能熟练度的判断
	if ( mSpecializedDegree < pLifeSkill->mStudyDegree[mLifeSkillLevel+1] )
	{
		return ERROR_LIFESKILL_LEVELNEEDMOREDEGREE;
	}
	
	return SUCCESS;
}

// 采集
int CLifeSkill::Collect( int &rPlayerAP, CTemplateCollect *pCollectNpc, int &rDegreeValue, int nLevel, bool bAddNow /* = false */ )
{	
	LK_ASSERT( pCollectNpc != NULL, return -1 )
	if ( pCollectNpc->mSkillType != mLifeSkillType )
	{
		return ERROR_LIFESKILL_COLLECTWITHOUTRIGHTSKILL;
	}
	
	if ( pCollectNpc->mCollectDegree > mSpecializedDegree )
	{
		return	ERROR_LIFESKILL_COLLECTDEGREELOW;
	}
	
	if ( pCollectNpc->mApCost > rPlayerAP )
	{
		return	ERROR_LIFESKILL_COLLECTAPLOW; 
	}
	
	if ( pCollectNpc->mCollectLevel > nLevel )
	{
		return ERROR_LIFESKILL_COLLECTLEVELLOW;
	}
	
	int i = 0;
	int tOldValue = 0;
	int	tValue	= mSpecializedDegree - pCollectNpc->mCollectDegree; 	
	for ( ; i < ( int )ARRAY_CNT(pCollectNpc->mDegreeInfo) - 1; ++i )
	{
		if ( tValue >= tOldValue && tValue <= pCollectNpc->mDegreeInfo[i].mValue )
		{
			break;
		}
		tOldValue =  pCollectNpc->mDegreeInfo[i].mValue;
	}

	if ( pCollectNpc->mDegreeInfo[i].mProbality >= rand( )%SERVER_PERCENT_INT )
	{
		if ( bAddNow )
		{
			AddSpecializedDegree( pCollectNpc->mDegreeInfo[i].mDegreeValue, DEGREEADDTYPE_USECOMPOSE );
		}	
		rDegreeValue = pCollectNpc->mDegreeInfo[i].mDegreeValue;	
	}  
	
	rPlayerAP = pCollectNpc->mApCost;
	return SUCCESS;
}


// 从pb结构创建技能列表
void CLifeSkill::SetFromPB( PBLifeSkill &pbLifeSkill )
{
	mLifeSkillType		= pbLifeSkill.lifeskilltype();		
	mLifeSkillLevel		= pbLifeSkill.lifeskilllevel();
	mSpecializedDegree	= pbLifeSkill.specializeddegree();	
	mLifeSkillKind		= pbLifeSkill.lifeskillkind();					
	
	for ( int i = 0; i < pbLifeSkill.composeid_size() && i < MAX_LIFESKILL_COMPOSENUM; ++i )
	{		
		ComposeInfo tComposeInfo;
		tComposeInfo.mTempID = pbLifeSkill.composeid( i );
		tComposeInfo.mUsedTimes = 0;
		CTemplateMultiCompose *tpCompose = ( CTemplateMultiCompose * )CDataStatic::SearchTpl( tComposeInfo.mTempID );
		if ( tpCompose == NULL )
		{
			LOG_ERROR( "pro","[%s,%d] compose ( id:%d ) can not find ",__FUNCTION__,__LINE__, tComposeInfo.mTempID );
			continue;
		}
		if ( tpCompose->mComposeType != CTemplateMultiCompose::COMPOSETYPE_LIFESKILL )
		{	 
			LOG_ERROR( "pro","[%s,%d] compose ( id:%d ) is not used for lifeskill ",__FUNCTION__,__LINE__, tComposeInfo.mTempID );
			continue;
		}
		mMultiComposeIDList.push_back( tComposeInfo );			
	}	
}   

// 创建技能列表的pb结构
void CLifeSkill::CreatTlvPB( PBLifeSkill &pbLifeSkill )
{	
	if( mLifeSkillType != 0 )
		pbLifeSkill.set_lifeskilltype( mLifeSkillType );		
	if( mLifeSkillLevel != 0 )
		pbLifeSkill.set_lifeskilllevel( mLifeSkillLevel );
	if( mSpecializedDegree != 0 )
		pbLifeSkill.set_specializeddegree( mSpecializedDegree );
	if( mLifeSkillKind  != 0 )
		pbLifeSkill.set_lifeskillkind( mLifeSkillKind );					

	MultiComposeIDList::iterator it = mMultiComposeIDList.begin();
	for ( ; it != mMultiComposeIDList.end(); ++it  )
	{
		pbLifeSkill.add_composeid( it->mTempID );
	}	  		
}

// 获取当前可学的技能种类
int CPlayerLifeSkill::GetCurrentSkillKind( )
{
	if ( mMainLifeSkill.GetLifeSkillType() == LIFESKILL_NONE )
	{
		return LIEFSKILLKIND_MAINSKILL;
	}
	
	if ( mSubLifeSkill.GetLifeSkillType() == LIFESKILL_NONE )
	{
		return LIEFSKILLKIND_SUBSKILL;
	}
			
	return LIFESKILLKIND_NONE;
}

// 遗忘某项生活技能
int CPlayerLifeSkill::ForgetLifeSKill( int nLifeSkillKind )
{
	switch( nLifeSkillKind )
	{
		case LIEFSKILLKIND_MAINSKILL:
		{
			return mMainLifeSkill.ForgetLifeSkill();
		}
		
		case LIEFSKILLKIND_SUBSKILL:
		{
			return mSubLifeSkill.ForgetLifeSkill();
		}
		default:
			return SUCCESS;
	}		
}

// 从pb结构创建玩家生活技能
void CPlayerLifeSkill::SetFromPB( PBLifeSkillBox & pbLifeSkillBox )
{	
	for ( int i = 0; i < pbLifeSkillBox.lifeskill_size(); ++i )
	{
		PBLifeSkill	 *pbLifeSkill = pbLifeSkillBox.mutable_lifeskill( i );
		switch( pbLifeSkill->lifeskillkind() )
		{
			case LIEFSKILLKIND_MAINSKILL:
			{
				mMainLifeSkill.SetFromPB( *pbLifeSkill );
				break;
			}
			case LIEFSKILLKIND_SUBSKILL:
			{
				mSubLifeSkill.SetFromPB( *pbLifeSkill );
				break;
			}
		}
	}	
}

// 创建玩家生活技能的pb结构
void CPlayerLifeSkill::CreatTlvPB( PBLifeSkillBox & pbLifeSkillBox )
{
	mMainLifeSkill.SetLifeSkillKind( LIEFSKILLKIND_MAINSKILL );
	mMainLifeSkill.CreatTlvPB( *pbLifeSkillBox.add_lifeskill() );
	
	mSubLifeSkill.SetLifeSkillKind( LIEFSKILLKIND_SUBSKILL );
	mSubLifeSkill.CreatTlvPB( *pbLifeSkillBox.add_lifeskill() );
}

// 根据生活技能类型获取生活技能
CLifeSkill *CPlayerLifeSkill::GetLifeSkillByType( int nLifeSkillType )
{
	if ( mMainLifeSkill.GetLifeSkillType() == nLifeSkillType )
	{
		return &mMainLifeSkill;
	}
	
	if ( mSubLifeSkill.GetLifeSkillType() == nLifeSkillType )
	{
		return &mSubLifeSkill;
	}	 	
	return NULL;
}

// 根据生活技能主辅种类获取生活技能
CLifeSkill *CPlayerLifeSkill::GetLifeSkillByKind( int nLifeSkillKind )
{
	switch( nLifeSkillKind )
	{
		case LIEFSKILLKIND_MAINSKILL:
		{		
			return &mMainLifeSkill;
			break;
		}
		
		case LIEFSKILLKIND_SUBSKILL:
		{		
			return &mSubLifeSkill;
			break;
		}			
		default:
			break;
	}
	
	return NULL;
}

// 放置材料
int CPlayerLifeSkill::AddMaterial( int nBagIndex, int nPosIndex, int nItemID, int &rPreMaterialIndex )
{
	if ( nPosIndex < 0 || nPosIndex >= MATERIALINDEXNUM )
	{
		return -1;
	}
	
	if ( mMaterialIndex[nPosIndex] != -1 )
	{
		rPreMaterialIndex = mMaterialIndex[nPosIndex];
	}
	
	mMaterialIndex[nPosIndex] = nBagIndex;
	mMaterialID[nPosIndex]	= nItemID;	
	
	return SUCCESS;
}

// 取下材料
int CPlayerLifeSkill::TakeMaterial( int nPosIndex, int &rBagIndex )
{
	rBagIndex = -1;
	if ( nPosIndex < 0 || nPosIndex >= MATERIALINDEXNUM )
	{
		return -1;
	}
		
	rBagIndex = mMaterialIndex[nPosIndex];
	mMaterialIndex[ nPosIndex ] = -1;
	return SUCCESS;
}

// 创造配方
int CPlayerLifeSkill::CreatComPose( int nLifeSkillType,  int nValue, int &rComposeID, int nSkillKind )
{
	CLifeSkill *tpLifeSkill = GetLifeSkillByType( nLifeSkillType );
	if ( tpLifeSkill == NULL )
	{
		return -1;
	}
	int tMaterialID[MATERIALINDEXNUM] = { 0 };
	int tNum = 0;
	for ( int i = 0; i < MATERIALINDEXNUM; ++i )
	{
		if ( mMaterialID[i] != 0 )
		{
			tMaterialID[tNum] =  mMaterialID[i];
			++tNum;	
		}
	}
	return tpLifeSkill->CreatCompose( nValue, tMaterialID, tNum , rComposeID, nSkillKind );
}

// 学习配方
int CPlayerLifeSkill::StudyCompose( int nComposeID )
{
	CTemplateMultiCompose *tpCompose = ( CTemplateMultiCompose * )CDataStatic::SearchTpl( nComposeID );
	LK_ASSERT( tpCompose != NULL, return -1 )
	
	CLifeSkill *tpLifeSkill = GetLifeSkillByType( tpCompose->mLifeSkillType );
	if ( tpLifeSkill == NULL )
	{
		return ERROR_LIFESKILL_WRONGLIFESKILL;
	}  
	int tAP		=	10000;
	unsigned int tMoney	=	100000; 	
	return tpLifeSkill->StudyCompose( nComposeID, COMPOSEGETWAY_BYSCROLL, tAP, tMoney );
}	

// 获取配方材料的列表
int CPlayerLifeSkill::GetMaterialList( int *pIDList, int &rNum )
{  
	LK_ASSERT( pIDList != NULL, return -1 )	
	int tNum = rNum;
	for ( int i = 0; i < tNum && i < (int)ARRAY_CNT( mMaterialIndex ); ++i )
	{
		pIDList[i] = mMaterialIndex[i];
	}  
	rNum = ARRAY_CNT( mMaterialIndex );
	return SUCCESS;	
}	

// 获取现在的称号ID
int CLifeSkill::GetCurrentTitleID()
{
	CTemplateLifeSkill *tpLifeSkill = ( CTemplateLifeSkill * )CDataStatic::GetSkillLifeTable( mLifeSkillType );  
	if ( tpLifeSkill == NULL )
	{
		return 0;
	}
	
	if ( mLifeSkillLevel < 0 || mLifeSkillLevel >= (int)ARRAY_CNT( tpLifeSkill->mCharacterNameID ) )
	{
		return 0;
	}
	
	return tpLifeSkill->mCharacterNameID[mLifeSkillLevel];
}	

// 清除材料信息
int CPlayerLifeSkill::ClearMaterial()
{
	for ( int i = 0; i < MATERIALINDEXNUM; ++i )
	{											
		mMaterialIndex[i] = -1;
	}	
	memset( mMaterialID, 0, sizeof( mMaterialID ) );
	return SUCCESS;
}								  

int CPropertyPlayer::AddTitle( int vTitleID, unsigned int nInvalidTime )
{
	if ( mTitleList.size() == TITLEMAX_SELF || vTitleID <= 0 )
		return -1;
	
	CTitle* tpTitle = ( CTitle* ) CDataStatic::GetTitle( vTitleID );
	if ( tpTitle == NULL )
		return -1;

	TitleList::iterator it = mTitleList.begin();
	for ( ; it != mTitleList.end(); ++ it )
	{
		if ( it->mTitleID == vTitleID )
			return -1;
	}

	ST_TITLE tTitle;
	tTitle.mTitleID = vTitleID;
	
	CEntityPlayer* pPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity(GetEntityID());
	LK_ASSERT( pPlayer != NULL, return -1 );
	time_t tNow = time( NULL );
	if ( nInvalidTime > 0 )// 优先使用外部设置的
	{
		tTitle.mTime = (int)nInvalidTime;
		// 开启定时器
		int nTimerOtherInfo[ TIMER_OTHER_INFO ] = { MESSAGE_PROPERTY, vTitleID, 0, 0};
		pPlayer->GetTimerReciver()->SetTimerBySec( pPlayer->GetEntityID(), TIMER_TITLE_DISAPPEAR
			, (nInvalidTime-tNow), 2, nTimerOtherInfo ) ;
		LOG_DEBUG("pro", "START1 TIMER hour:%d", (nInvalidTime-tNow) );
	}
	else if ( tpTitle->mValidTime > 0 )
	{
		int tEndTime = tNow + tpTitle->mValidTime * 3600;
		tTitle.mTime = tEndTime;
		// 开启定时器
		int nTimerOtherInfo[ TIMER_OTHER_INFO ] = { MESSAGE_PROPERTY, vTitleID, 0, 0};
		pPlayer->GetTimerReciver()->SetTimerBySec( pPlayer->GetEntityID(), TIMER_TITLE_DISAPPEAR
			, tpTitle->mValidTime * 3600, 2, nTimerOtherInfo ) ;
		LOG_DEBUG("pro", "START TIMER hour:%d", tpTitle->mValidTime );
	}

	mTitleList.push_back( tTitle );
	return vTitleID;
}


void CProperty::IsDisappear( CEntity* pEntity, int vTickOffset, std::vector< unsigned int >& vDeleteList )
{
	CMapObject* tpMapObj  = CMapObjManager::GetSingletonPtr( )->GetMapObject( pEntity->GetLineID( ), 
		pEntity->GetMapID( ), pEntity->GetMapIndex( ) );

	if( tpMapObj == NULL )
	{
		vDeleteList.push_back( pEntity->GetEntityID( ) );
	}
}

// 遍历玩家身上的所有称号，如果有vTitleID称号同类型的，且比其低的，返回该低等称号ID
int CPropertyPlayer::GetLowLevelTitle( int vTitleID )
{
	CTitle* tpBaseTitle = CDataStatic::GetTitle( vTitleID );
	if ( tpBaseTitle == NULL )
	{
		return -1;
	}

	TitleList::iterator it = mTitleList.begin();
	for ( ; it != mTitleList.end(); ++ it )
	{
		CTitle* tpTitle = CDataStatic::GetTitle( it->mTitleID );
		if ( tpTitle == NULL )
			continue;

		if ( tpTitle->mType != tpBaseTitle->mType )
			continue;

		if ( tpTitle->mObtainCondition != tpBaseTitle->mObtainCondition )
			continue;

		if ( tpTitle->mSubCondition < tpBaseTitle->mSubCondition )
		{
			return it->mTitleID;
		}
	}

	return 0;
}

// 法宝获得技能相关加成
unsigned int CPropertyPlayer::MWGetFixNumber( CTemplateSkill::PASSINC_TYPE vType )
{
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
	if ( tpMW == NULL )
		return 0;
	return tpMW->GetMWSkillList()->GetFixNumber(  vType );
}

int CPropertyPlayer::MWGetPerNumber( CTemplateSkill::PASSINC_TYPE vType )
{
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
	if ( tpMW == NULL )
		return 0;
	return tpMW->GetMWSkillList()->GetPerNumber( vType );
}

void CPropertyPlayer::CheckDiploidState()
{
	if ( mDiploidState[ DIPLOID_STATE ] == 0 )
	{
		InitDoubleState();
		SetValidDiploid( 0 );
	}
	else
	{
		if ( GetValidDiploid() == 0 )
		{
			InitDoubleState();
			SetDiploid( 0 );
		}
	}
}

void CWizard::Initial( )
{
	for ( int i = 0; i < WIZARD_NUM; i++ )
	{
		mWizardPoint[ i ] = 1;
		mWizardValue[ i ] = 0;
		mWizardSubmit[ i ] = 0;
		mWizardIncrease[ i ] = 0;
	}
	// 上来送一点才华 
	mWizardIncrease[ WIZARD_COMMON ] = 1;
	mTrend = 0;

	mCombatFame = 0;
	mRepeFame = 0;
	mCommonFame = 0;
	mCombatToday = 0;
	mRepeToday = 0;
	mCommonToday = 0;
	mCombatAll = 0;
	mRepeAll = 0;
	mCommonAll = 0;
}

int CWizard::AddPoint( int vType )
{
	if ( vType < 0 || vType >= WIZARD_COMMON )
	{
		return ERROR_ADDWIZARD_UNKNOWN;
	}
	if ( mWizardIncrease[ WIZARD_COMMON ] <= 0 )
	{
		return ERROR_ADDWIZARD_NOTENOUGH;
	}
	mWizardIncrease[ vType ]++;
	mWizardIncrease[ WIZARD_COMMON ]--;
	return SUCCESS;
}

void CWizard::AddValue( int vType, int vValue )
{
	if ( vType < 0 || vType >= WIZARD_NUM )
	{
		return;
	}
	CTemplateWizardTable *pTable = (CTemplateWizardTable*)CDataStatic::GetTemp( TEMP_WIZARD );
	if ( pTable == NULL )
	{
		return;
	}
	mWizardValue[ vType ] += vValue;
	while ( mWizardValue[ vType ] >= pTable->mExp[ mWizardPoint[ vType ] ] && mWizardPoint[ vType ] < MAX_WIZARD_GRADE )
	{
		mWizardValue[ vType ] = mWizardValue[ vType ] - pTable->mExp[ mWizardPoint[ vType ] ];
		mWizardPoint[ vType ]++;
		if ( vType == WIZARD_COMMON )
		{
			mWizardIncrease[ WIZARD_COMMON ]++;
		}
	}

	if ( mWizardPoint[ vType ] == MAX_WIZARD_GRADE )
		mWizardValue[ vType ] = 0;
}

int CWizard::Submit( )
{
	for ( int i = 0; i < WIZARD_NUM; i++ )
	{
		mWizardSubmit[ i ] = mWizardPoint[ i ];
	}
	return 0;
}

void CWizard::Clear( )
{
	for ( int i = 0; i < WIZARD_COMMON; i++ )
	{
		mWizardIncrease[ WIZARD_COMMON ] += mWizardIncrease[ i ];
		mWizardIncrease[ i ] = 0;
	}
}

void CWizard::CreateFromTlv( PBWizard *pWizard )
{
	if ( pWizard == NULL )
	{
		return;
	}
	if ( pWizard->wizardpoint_size() > WIZARD_NUM ||
		 pWizard->wizardvalue_size() > WIZARD_NUM ||
		 pWizard->wizardsubmit_size() > WIZARD_NUM ||
		 pWizard->wizardincrease_size() > WIZARD_NUM )
	{
		return;
	}
	for ( int i = 0; i < WIZARD_NUM; i++ )
	{
		mWizardPoint[ i ] = pWizard->wizardpoint(i);
		mWizardValue[ i ] = pWizard->wizardvalue(i);
		mWizardSubmit[ i ] = pWizard->wizardsubmit(i);
		mWizardIncrease[ i ]= pWizard->wizardincrease(i);
	}
	mTrend = pWizard->trend();

	mCombatFame = pWizard->combatfame();
	mRepeFame = pWizard->repefame();
	mCommonFame = pWizard->commonfame();
	mCombatToday = pWizard->combattoday();
	mRepeToday = pWizard->repetoday();
	mCommonToday = pWizard->commontoday();
	mCombatAll = pWizard->combatall();
	mRepeAll = pWizard->repeall();
	mCommonAll = pWizard->commonall();
}

void CWizard::GetTlvBuffer( PBWizard *pWizard )
{
	if ( pWizard == NULL )
	{
		return;
	}
	for ( int i = 0; i < WIZARD_NUM; i++ )
	{
		pWizard->add_wizardpoint(mWizardPoint[ i ]);
		pWizard->add_wizardvalue(mWizardValue[ i ]);
		pWizard->add_wizardsubmit(mWizardSubmit[ i ]);
		pWizard->add_wizardincrease(mWizardIncrease[ i ]);
	}
	pWizard->set_trend(mTrend);

	pWizard->set_combatfame( mCombatFame );
	pWizard->set_repefame( mRepeFame );
	pWizard->set_commonfame( mCommonFame );
	pWizard->set_combattoday( mCombatToday );
	pWizard->set_repetoday( mRepeToday );
	pWizard->set_commontoday( mCommonToday );
	pWizard->set_combatall( mCombatAll );
	pWizard->set_repeall( mRepeAll );
	pWizard->set_commonall( mCommonAll );
}

int CPropertyCharacter::AttachBuffState( int vSrcEntityID, CTemplateBuff::CBuffFunc* vpBuffFunc, int vIndex )
{
	LK_ASSERT( vpBuffFunc != NULL, return ERR_INVALID_PARAMETER );
	EState tState = STATE_NONE; 
	EState tState1 = STATE_NONE;
	int tParame1 = 0;
	int tParame2 = 0;
	switch( vpBuffFunc->mBuffFunc )
	{
		case CTemplateBuff::BUFFFUNC_IMMUNITYDAMAGE:
			{
				tState = STATE_IMMUNITYDAMAGE;
			}
			break;
			// 免疫负面Buff
		case CTemplateBuff::BUFFFUNC_IMMUNITYREVERSD:
			{
				tState = STATE_IMMUNITYREVERSD;
			}
			break;
			// 玩家受到守护
		case CTemplateBuff::BUFFFUNC_PROTECTED:
			{					
				tState = STATE_PROTECTED;
				tParame1 = vpBuffFunc->mBuffParam[ 0 ];
				tParame2 = vSrcEntityID;	
			}
			break;
			// 魔法盾
		case CTemplateBuff::BUFFFUNC_MAGICSHIELD:
			{
				tState = STATE_MAGICSHIELD;
				tParame1 = vpBuffFunc->mBuffParam[ 0 ];
				tParame2 = vpBuffFunc->mBuffParam[ 1 ]; 		
			}
			break;
			// 增加指定系别攻击固定值
		case CTemplateBuff::BUFFFUNC_PGENERATTACKFIX:
			{
				tState = STATE_PGENERATTACKFIX;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
                tParame2 = vpBuffFunc->mBuffParam[ 1 ];
			}
			break;
			// 增加下次攻击致命等级固定值
		case CTemplateBuff::BUFFFUNC_NEXTDEATHHITFIX:
			{
				tState = STATE_NEXTDEATHHITFIX;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
                tParame2 = vIndex;
			}
			break;
			// 增加下次攻击致命伤害固定值
		case CTemplateBuff::BUFFFUNC_NEXTDEATHRATEFIX:
			{
				tState = STATE_NEXTDEATHRATEFIX;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
                tParame2 = vIndex;
			}
			break;
			// 忽略衣服计算
		case CTemplateBuff::BUFFFUNC_IGNORECLOTH:
			{
				tState = STATE_IGNORECLOTH;
			}
			break;
			// 忽略武器计算
		case CTemplateBuff::BUFFFUNC_UNARM:
			{
				tState = STATE_UNARM;
			}
			break;
			// 无法攻击
		case CTemplateBuff::BUFFFUNC_UNATTACK:
			{
				tState = STATE_UNATTACK;
			}
			break;
			// 抵消N次伤害
		case CTemplateBuff::BUFFFUNC_DAMAGELESS:
			{
				tState = STATE_DAMAGELESS;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
                tParame2 = vIndex;
			}
			break;
			// cannt move
		case CTemplateBuff::BUFFFUNC_UNMOVE:
			{
				tState = STATE_CANNTMOVE;
			}
			break;
			// can't use item( medication )
		case CTemplateBuff::BUFFFUNC_UNUSEITEM:
			{
				tState = STATE_FASTING;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];

			}
			break;
			// 反弹伤害
		case CTemplateBuff::BUFFFUNC_REBOUNDDAMAGE:
			{
				tState = STATE_REBOUND;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
                tParame2 = vpBuffFunc->mBuffParam[ 1 ];
			}
			break;
			// speed incream
			// 增加移动速度固定值(移动速度都只有百分比,没固定值)
		case CTemplateBuff::BUFFINC_INCSPEEDPER:
			{	
				tState = STATE_INCREAMSPEED;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 降低速度百分比
		case CTemplateBuff::BUFFINC_DECSPEEDPER:
			{
				tState = STATE_UNCREAMSPEED;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 生命恢复效果降低百分比
		case CTemplateBuff::BUFFFUNC_HPBACKEFFECT:
			{
				tState = STATE_CUREHPLESS;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
                tParame2 = vpBuffFunc->mBuffParam[ 1 ];
			}
			break;
			// 杀怪取血
		case CTemplateBuff::BUFFFUNC_KILLERRENEW:
			{
				tState = STATE_KILLERRENEW;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 成功放技能会受到特殊效果
		case CTemplateBuff::BUFFFUNC_FIREEVENT:
			{
				tState = STATE_FIREEVENT;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
                tParame2 = vpBuffFunc->mBuffParam[ 1 ];
			}
			break;
			// 随即走动
		case CTemplateBuff::BUFFFUNC_RANDMOVE:
			{
				tState = STATE_RANDMOVE;
				tState1 = STATE_CONFUSION;
			}
			break;
			// 攻击附带效果
		case CTemplateBuff::BUFFFUNC_ATTACKEVENT:
			{
				tState = STATE_ATTACKEVENT;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
                tParame2 = vpBuffFunc->mBuffParam[ 1 ];
			}
			break;
			// 增加被攻击时所受物理伤害固定值
		case CTemplateBuff::BUFFINC_INCDESPDAMAGEFIX:
			{
				tState = STATE_INCDESPDAMAGEFIX;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 增加被攻击时所受物理伤害百分比
		case CTemplateBuff::BUFFINC_INCDESPDAMAGEPER:
			{
				tState = STATE_INCDESPDAMAGEPER;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 降低被攻击时所受物理伤害固定值
		case CTemplateBuff::BUFFINC_DECDESPDAMAGEFIX:
			{
				tState = STATE_DECDESPDAMAGEFIX;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 降低被攻击时所受物理伤害百分比
		case CTemplateBuff::BUFFINC_DECDESPDAMAGEPER:
			{
				tState = STATE_DECDESPDAMAGEPER;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 增加被攻击时所受法术伤害固定值
		case CTemplateBuff::BUFFINC_INCDESMDAMAGEFIX:
			{
				tState = STATE_INCDESMDAMAGEFIX;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 增加被攻击时所受法术伤害百分比
		case CTemplateBuff::BUFFINC_INCDESMDAMAGEPER:
			{
				tState = STATE_INCDESMDAMAGEPER;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 降低被攻击时所受法术伤害固定值
		case CTemplateBuff::BUFFINC_DECDESMDAMAGEFIX:
			{
				tState = STATE_DECDESMDAMAGEFIX;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 降低被攻击时所受法术伤害百分比
		case CTemplateBuff::BUFFINC_DECDESMDAMAGEPER:
			{
				tState = STATE_DECDESMDAMAGEPER;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 受某系技能攻击则增加受到的伤害百分比
		case CTemplateBuff::BUFFFUNC_INCGENERDAMAGEPER:
			{
				tState = STATE_INCGENERDAMAGEPER;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 嘲讽
		case CTemplateBuff::BUFFFUNC_SNEER:
			{
				tState = STATE_SNEER;
			}
			break; 	
			// 增加获得经验百分比( 打怪 )
		case CTemplateBuff::BUFFFUNC_INCEXP:
			{
				tState = STATE_INCEXP;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
			// 减少所有受到伤害的百分比 ( 包括BUFF )
		case CTemplateBuff::BUFFFUNC_DECALLDAMAGEPER:
			{
				tState = STATE_DECALLDAMAGEPER;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
		case CTemplateBuff::BUFFFUNC_TRANSFORM:
			{
				// 变身的状态,参数1为所变NPC的模板ID
				tState = STATE_TRANSFORM;
				tParame1 = vpBuffFunc->mBuffParam[ 0 ];
				break;
			}
			// 获得功勋百分比提升
		case CTemplateBuff::BUFFFUNC_INCHONORPER:
			{
				tState = STATE_INCHONORSCALE;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
				break;
			}	
		
		// 增加军团荣耀的buff
		case CTemplateBuff::BUFFFUNC_INCCORPSGORY:
			{	
				tState = STATE_INCCORPSGLORY;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
				break;
			}	
		
		// 增加玩家ap的buff
		case CTemplateBuff::BUFFFUNC_INCAP:
			{  
				tState = STATE_INCAP;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
				break;
			}
		case CTemplateBuff::BUFFFUNC_PLAYER_UNATTACK:
			{
				tState = STATE_PLAYER_UNATTACK;
			}
			break;
		case CTemplateBuff::BUFFFUNC_UNATTACK_PLAYER:
			{
				tState = STATE_UNATTACK_PLAYER;
			}
			break;
		case CTemplateBuff::BUFFFUNC_HORSE:
			{
				tState = STATE_RIDE_HORSE;
				tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
		case CTemplateBuff::BUFFFUNC_INVISIBLE:
			{
				tState = STATE_INVISIBLE;
				// 隐身暂时还不知道加一些什么参数在里面
			}
			break;
		case CTemplateBuff::BUFFFUNC_DISINVISIBLE:
			{
				tState = STATE_DISINVISIBLE;
				tParame1 = vpBuffFunc->mBuffParam[ 0 ];	
			}
			break;
		case CTemplateBuff::BUFFFUNC_FLY:
			{
				tState = STATE_FLY;
				tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
		case CTemplateBuff::BUFFFUNC_STONE:
			{
				tState = STATE_STONE;
				tParame1 = vpBuffFunc->mBuffParam[ 0 ];
				tParame2 = vpBuffFunc->mBuffParam[ 1 ];
			}
			break;
		case CTemplateBuff::BUFFFUNC_HIDE:
			{
				tState = STATE_HIDE;
				tParame1 = vpBuffFunc->mBuffParam[ 0 ];	
			}
			break;
		case CTemplateBuff::BUFFFUNC_INCOFFLINEEXP:
			{
				tState = STATE_INCOFFLINEEXP;
                tParame1 = vpBuffFunc->mBuffParam[ 0 ];
			}
			break;
		default:
			break;
	}
		
	if( tState != STATE_NONE )
	{
		AttachState( tState, tParame1, tParame2, vIndex, BUFF );
	}

	if( tState1 != STATE_NONE )
	{
		AttachState( tState1, tParame1, tParame2, vIndex, BUFF );
	}

	return SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

// 检查读秒是否正确
// bool bUseConfigTime 是否使用进度条配置表中的读条时间，物品里面一般配有进度条时间

bool CPropertyPlayer::CheckProgressTime( int nFuncType, int nNetSpeed, bool bUseConfigTime /* = true */, int nProgressTime /* = 0 */ )
{
	if ( nFuncType != mProgressFunc )
	{
		return false;
	}
	
	
	uint64_t tTimeNow = LINEKONG::GetTickCount();
	
	int tProgressTime = 0;
	if ( bUseConfigTime == true )
	{
		CTemplateProgressConfig *tpProgressConfig = ( CTemplateProgressConfig * )CDataStatic::spProgressConfig;
		if ( tpProgressConfig == NULL )
		{
			LOG_ERROR( "pro","progressconfig not set" );
			return true;
		}

		tProgressTime = CDataStatic::GetProgressTimeLength( mProgressFunc );	
	}
	else
	{
		tProgressTime = nProgressTime;	
	}
	
	
	if ( tProgressTime <=0 )
	{
		return true;
	}
	
	// 计算是否在允许范围内
	int tTimeLength = (int)(tTimeNow - mProgressStartTime);	
	if ( tTimeLength <= 0 )
	{
		return false;
	}		
	
	if ( tTimeLength < tProgressTime && ( tProgressTime - tTimeLength ) >= PROGRESS_TIMEERROR )
	{
		return false;
	}
	
	if ( tTimeLength > tProgressTime && ( tTimeLength - tProgressTime ) >= nNetSpeed*3 )
	{
		return false;
	}
	
	return true;
}

// 基本信息blob
char* CPropertyPlayer::GetRoleBasicBlobStr( )
{
	static char cBasicBuff[MAX_PRO_PB_DATA_LEN];
	static char szBasicStr[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szBasicStr;
	
	PBPlayerBasicInfo tBasicInfo;
	GetBasicInfoTlvBuffer( &tBasicInfo );
	
	if( tBasicInfo.SerializeToArray( cBasicBuff, sizeof(cBasicBuff) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}
	
	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szBasicStr)-1, cBasicBuff, tBasicInfo.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szBasicStr;
}

void CPropertyPlayer::GetBasicInfoTlvBuffer( PBPlayerBasicInfo* pBasicInfo )
{
	pBasicInfo->set_str( mCurStr );
	pBasicInfo->set_con( mCurCon );
	pBasicInfo->set_spr( mCurSpr );
	pBasicInfo->set_wis( mCurWis );
	pBasicInfo->set_sta( mCurSta );
	pBasicInfo->set_remainpoint( mRemainPoint );
	pBasicInfo->set_skillpoint( mSkillPoint );
	pBasicInfo->set_prolevel(mProLevel);
	pBasicInfo->set_proexp(mProExp);
    pBasicInfo->set_showequip( miIsShowEquip );
    pBasicInfo->set_showfashion( miIsShowFashion );
}

void CPropertyPlayer::CreateBasicInfoFromTlv( PBPlayerBasicInfo* pBasicInfo )
{
	mCurStr = pBasicInfo->str();
	mCurCon = pBasicInfo->con();
	mCurSpr = pBasicInfo->spr();
	mCurWis = pBasicInfo->wis();
	mCurSta = pBasicInfo->sta();
	mRemainPoint = pBasicInfo->remainpoint();
	mSkillPoint = pBasicInfo->skillpoint();
	//mProLevel = pBasicInfo->prolevel();
	mProLevel = MAXPROLEVEL;
	mProExp = pBasicInfo->proexp();
    miIsShowEquip = pBasicInfo->showequip();
    miIsShowFashion = pBasicInfo->showfashion();
}


// 技能blob
char* CPropertyPlayer::GetRoleSkillBlobStr(	)
{
	static char cSkillList[MAX_PRO_PB_DATA_LEN];
	static char szSkillList[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szSkillList;

	PBSkillList tSkillList;
	mSkill.GetTlvBuffer2( &tSkillList );
	
	if( tSkillList.SerializeToArray( cSkillList, sizeof(cSkillList) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szSkillList)-1, cSkillList, tSkillList.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szSkillList;
}

// Buff blob
char* CPropertyPlayer::GetRoleBuffBlobStr( )
{
	static char cBuffList[MAX_PRO_PB_DATA_LEN];
	static char szBuffList[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szBuffList;

	PBBuffList tPBBuff;
	mBuffList.GetTlvBuffer2( &tPBBuff );

	if( tPBBuff.SerializeToArray( cBuffList, sizeof(cBuffList) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szBuffList)-1, cBuffList, tPBBuff.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szBuffList;
}

// 包裹 blob
char* CPropertyPlayer::GetRolePackageBlobStr( )
{
	static char cPackage[MAX_PRO_PB_DATA_LEN];
	static char szPackage[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szPackage;

	PBItemBox tPBPackage;
	GetBaggage()->GetTlvBuffer2( &tPBPackage );

	if( tPBPackage.SerializeToArray( cPackage, sizeof(cPackage) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szPackage)-1, cPackage, tPBPackage.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szPackage;
}


// 装备blob
char* CPropertyPlayer::GetRoleEquipBlobStr( )
{
	static char cEquip[MAX_PRO_PB_DATA_LEN];
	static char szEquip[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szEquip;

	PBItemBox tPBEquip;
	GetEquipment()->GetTlvBuffer2( &tPBEquip );

	if( tPBEquip.SerializeToArray( cEquip, sizeof(cEquip) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szEquip)-1, cEquip, tPBEquip.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szEquip;
}

// 仓库blob
char* CPropertyPlayer::GetRoleStorageBlobStr( )
{
	static char cStorage[MAX_PRO_PB_DATA_LEN];
	static char szStorage[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szStorage;

	PBItemBox tPBStorage;
	GetStorage()->GetTlvBuffer2( &tPBStorage );

	if( tPBStorage.SerializeToArray( cStorage, sizeof(cStorage) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szStorage)-1, cStorage, tPBStorage.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szStorage;
}

// 任务blob
char* CPropertyPlayer::GetRoleTaskPackageBlobStr( )
{
	static char cTaskPackage[MAX_PRO_PB_DATA_LEN];
	static char szTaskPackage[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szTaskPackage;

	PBItemBox tPBTaskPackage;
	GetTaskBaggage()->GetTlvBuffer2( &tPBTaskPackage );

	if( tPBTaskPackage.SerializeToArray( cTaskPackage, sizeof(cTaskPackage) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szTaskPackage)-1, cTaskPackage, tPBTaskPackage.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szTaskPackage;
}


// CD组blob
char* CPropertyPlayer::GetRoleCDGroupBlobStr( )
{
	static char cCDGroup[MAX_PRO_PB_DATA_LEN];
	static char szCDGroup[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szCDGroup;

	PBPlayerCDGroupInfo tPBCDGroup;
	GetCDGroupTlvBuffer( &tPBCDGroup );

	if( tPBCDGroup.SerializeToArray( cCDGroup, sizeof(cCDGroup) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szCDGroup)-1, cCDGroup, tPBCDGroup.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szCDGroup;
}

void CPropertyPlayer::CreateCDGroupFromTlv( PBPlayerCDGroupInfo* pCDGroup )
{
	for ( int i = 0; i < pCDGroup->cdgroup_size() && i < mCDGroup.GetMaxGroupIdx(); i ++ )
	{
		PBSingleCDGroup* tpSingle = pCDGroup->mutable_cdgroup(i);
		LK_ASSERT( tpSingle != NULL, continue );
		LK_ASSERT( tpSingle->index() >= 0 && tpSingle->index() < (unsigned int) mCDGroup.GetMaxGroupIdx(), continue );
		CGroupCD& rGroupCD = mCDGroup.GetGroupCD( tpSingle->index() );			
		rGroupCD.SetCDTime( tpSingle->cdtime() );
	}	
}

void CPropertyPlayer::GetCDGroupTlvBuffer( PBPlayerCDGroupInfo* pCDGroup )
{
	for ( int i = 0; i < (int)mCDGroup.GetMaxGroupIdx() ; i ++ )
	{
		CGroupCD& rGroupCD = mCDGroup.GetGroupCD( i );			
		int tTime = rGroupCD.GetCDTime() ;
		if ( tTime != 0 )
		{
			PBSingleCDGroup* tpSingle = pCDGroup->add_cdgroup();
			tpSingle->set_index(i);
			tpSingle->set_cdtime(tTime);
		}
	}	
}


// 传送信息blob
char* CPropertyPlayer::GetRoleTelePosBlobStr( )
{
	static char cTelePos[MAX_PRO_PB_DATA_LEN];
	static char szTelePos[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szTelePos;

	PBTelePosInfo tPBTelePosInfo;
	GetTelePosTlvBuffer( &tPBTelePosInfo );

	if( tPBTelePosInfo.SerializeToArray( cTelePos, sizeof(cTelePos) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szTelePos)-1, cTelePos, tPBTelePosInfo.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szTelePos;	
}

void CPropertyPlayer::CreateTelePosFromTlv( PBTelePosInfo* pTelePos )
{
	const PBTelePostion& rTelePostion = pTelePos->townpos();

	mTownPos.mLineID = rTelePostion.line();
	mTownPos.mMapID = rTelePostion.map();
	mTownPos.mPos.mX = rTelePostion.posx();
	mTownPos.mPos.mY = rTelePostion.posy();

	for ( int i = 0; i < pTelePos->telelist_size(); ++i )
	{
		const PBTelePostion& rTelePostion = pTelePos->telelist(i);
		ComplexPos tmpPos;
		tmpPos.mLineID = rTelePostion.line();
		tmpPos.mMapID = rTelePostion.map();
		tmpPos.mPos.mX = rTelePostion.posx();
		tmpPos.mPos.mY = rTelePostion.posy();
		mTeleList.push_back(tmpPos);
	}
}

void CPropertyPlayer::GetTelePosTlvBuffer( PBTelePosInfo* pTelePos )
{
	pTelePos->mutable_townpos()->set_line( mTownPos.mLineID );
	pTelePos->mutable_townpos()->set_map( mTownPos.mMapID );
	pTelePos->mutable_townpos()->set_posx( mTownPos.mPos.mX );
	pTelePos->mutable_townpos()->set_posy( mTownPos.mPos.mY );

	for ( unsigned int i = 0; i < mTeleList.size(); ++i )
	{
		PBTelePostion* pTelePostion = pTelePos->add_telelist();
		pTelePostion->set_line( mTeleList[i].mLineID );
		pTelePostion->set_map( mTeleList[i].mMapID );
		pTelePostion->set_posx( mTeleList[i].mPos.mX );
		pTelePostion->set_posy( mTeleList[i].mPos.mY );
	}
}


// PVP blob
char* CPropertyPlayer::GetRolePvPBlobStr( )
{
	static char cPvP[MAX_PRO_PB_DATA_LEN];
	static char szPvP[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szPvP;

	PBPlayerPVPInfo tPBPvP;
	GetPvPBlobInfoTlvBuffer( &tPBPvP );

	if( tPBPvP.SerializeToArray( cPvP, sizeof(cPvP) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szPvP)-1, cPvP, tPBPvP.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szPvP;
}

void CPropertyPlayer::CreatePvPBlobInfoFromTlv( PBPlayerPVPInfo* pPvPInfo )
{
	CEntityPlayer* pPlayer = (CEntityPlayer*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	LK_ASSERT( pPlayer != NULL, return );
	pPlayer->SetPKMode( (EPKMode)(pPvPInfo->pkmode()) );

	mPKValue = pPvPInfo->pkvalue();
	mKillNumIn = pPvPInfo->killnumin();
	mKillNumOut = pPvPInfo->killnum();
	mHonor = pPvPInfo->honor();
	mInHonPerDay = pPvPInfo->inhonperday();
	mOutHonPerDay = pPvPInfo->outhonperday();
	mYellowStatus = pPvPInfo->yellowstatus();
	mPrisonTime = pPvPInfo->prisontime();
	mPrisonTotalTime = pPvPInfo->totalprisontime();
	mTodayHonor = pPvPInfo->todayhonor();

	for( int i = 0; i < (int)ARRAY_CNT(mKill) && i < pPvPInfo->kill_size(); i++ )
	{
		const PBKillData& rPBKillData = pPvPInfo->kill( i );
		mKill[ i ].mKillID = rPBKillData.killid();
		mKill[ i ].mKillTime = rPBKillData.killtime();
	}
	mTotalWar = pPvPInfo->totalwar();
	mTodayWar = pPvPInfo->todaywar();
	mTodayOther = pPvPInfo->todayother();
	mFreeWar = pPvPInfo->freewar();
	mWarSerial = pPvPInfo->warserial();
	mWarKill = pPvPInfo->warkill();
	mExpHonor = pPvPInfo->exphonor();
	mItemHonor = pPvPInfo->itemhonor();
}

void CPropertyPlayer::GetPvPBlobInfoTlvBuffer( PBPlayerPVPInfo* pPvPInfo )
{
	CEntityPlayer* pPlayer = (CEntityPlayer*) CSceneLogic::GetSingletonPtr()->GetEntity( GetEntityID( ) );
	LK_ASSERT( pPlayer != NULL, return );

	pPvPInfo->set_pkmode( pPlayer->GetPKMode() );
	pPvPInfo->set_pkvalue( mPKValue );
	pPvPInfo->set_killnumin( mKillNumIn );
	pPvPInfo->set_killnum( mKillNumOut );
	pPvPInfo->set_honor( mHonor );
	pPvPInfo->set_inhonperday( mInHonPerDay );
	pPvPInfo->set_outhonperday( mOutHonPerDay );
	pPvPInfo->set_yellowstatus( mYellowStatus );
	pPvPInfo->set_prisontime( mPrisonTime );
	pPvPInfo->set_totalprisontime( mPrisonTotalTime );
	pPvPInfo->set_todayhonor(mTodayHonor);

	for( int i = 0; i < (int)ARRAY_CNT(mKill); i++ )
	{
		if( mKill[ i ].mKillID != 0 )
		{
			PBKillData* pPBKillData = pPvPInfo->add_kill( );
			pPBKillData->set_killid( mKill[ i ].mKillID );
			pPBKillData->set_killtime( mKill[ i ].mKillTime );
		}
	}
	pPvPInfo->set_totalwar( mTotalWar );
	pPvPInfo->set_todaywar( mTodayWar );
	pPvPInfo->set_todayother( mTodayOther );
	pPvPInfo->set_freewar( mFreeWar );
	pPvPInfo->set_warserial( mWarSerial );
	pPvPInfo->set_warkill( mWarKill );
	pPvPInfo->set_exphonor( mExpHonor );
	pPvPInfo->set_itemhonor( mItemHonor );
}

// Titlt blob
char* CPropertyPlayer::GetRoleTitleBlobStr( )
{
	static char cTitle[MAX_PRO_PB_DATA_LEN];
	static char szTitle[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szTitle;

	PBPlayerTitleInfo tPBTitleInfo;
	GetTitleInfoTlvBuffer( &tPBTitleInfo );

	if( tPBTitleInfo.SerializeToArray( cTitle, sizeof(cTitle) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szTitle)-1, cTitle, tPBTitleInfo.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szTitle;
}

void CPropertyPlayer::CreateTitleInfoFromTlv( PBPlayerTitleInfo* pTitleInfo )
{
	mCurrentTitle = pTitleInfo->currenttitle();

	for( int i = 0; i < pTitleInfo->titlelist_size() && i < pTitleInfo->time_size(); i++ )
	{
		ST_TITLE tTitle;
		tTitle.mTitleID = pTitleInfo->titlelist( i );
		tTitle.mTime = pTitleInfo->time( i );
		mTitleList.push_back( tTitle );
	}
}

void CPropertyPlayer::GetTitleInfoTlvBuffer( PBPlayerTitleInfo* pTitleInfo )
{
	pTitleInfo->set_currenttitle( mCurrentTitle );
	TitleList::iterator it = mTitleList.begin();
	for ( ; it != mTitleList.end(); ++ it )
	{
		pTitleInfo->add_titlelist( it->mTitleID );
		pTitleInfo->add_time( it->mTime );
	}
}

// Activity blob
char* CPropertyPlayer::GetRoleActivityBlobStr( )
{
	static char cActivity[MAX_PRO_PB_DATA_LEN];
	static char szActivity[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szActivity;

	PBPlayerActivityInfo tPBActivity;
	GetActivityInfoTlvBuffer( &tPBActivity );

	if( tPBActivity.SerializeToArray( cActivity, sizeof(cActivity) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szActivity)-1, cActivity, tPBActivity.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szActivity;
}

void CPropertyPlayer::CreateActivityInfoFromTlv( PBPlayerActivityInfo* pActivityInfo )
{
	mDiploidTime = pActivityInfo->diploidtime();
	mLastRefreshTime = pActivityInfo->lastrefreshtimeofdiploid();

	for( unsigned int i = 0; i < (unsigned int)pActivityInfo->diploidstate_size() && i < ARRAY_CNT( mDiploidState ); i++ )
	{
		mDiploidState[ i ] = pActivityInfo->diploidstate( i ); 
	}
	mDailyWelfareStatus = pActivityInfo->dailywelfaresta();
	
	mLastDaylyDisposTime = pActivityInfo->lastdaylydispostime();
	mLastWeeklyDisposTime = pActivityInfo->lastweeklydispostime();
	mLastFamilyApplyTime = pActivityInfo->lastfamilyapplytime();
	mLastTimeLeaveFamily = pActivityInfo->lasttimeleavefamily();
	mLastJudgeTime = pActivityInfo->lastjudgetime();
	mSaveDate = pActivityInfo->savedate();
	mVipFlag = pActivityInfo->vipflag();
	mVipEndDate = pActivityInfo->viptime();
	// 每天领取
	PBAwardRecord *tpRecord = pActivityInfo->mutable_awardrecord();
	for ( unsigned int i = 0; i < (unsigned int)tpRecord->awardid_size() && i < MAX_AWARD_NUM; i++ )
	{
		InsertAwardTimes( tpRecord->awardid(i), tpRecord->times(i), EALT_Daily);
	}
	// 每周领取
	tpRecord = pActivityInfo->mutable_weeklyawardrecord();
	for ( unsigned int i = 0; i < (unsigned int)tpRecord->awardid_size() && i < MAX_AWARD_NUM; i++ )
	{
		InsertAwardTimes( tpRecord->awardid(i), tpRecord->times(i), EALT_Weekly);
	}
	mInvestStatus = pActivityInfo->investstatus();

	for ( int i =0 ; i < pActivityInfo->activitystate_size(); ++i )
	{
		SinglePlayerActivityInfo inf;
		PBActivityStateInf tInf = pActivityInfo->activitystate(i);
		inf.chg_tm = tInf.chgtm();
		inf.nState = tInf.curstate();
		if ( tInf.stateparam_size() >=3 )
		{
			inf.nParam1 = tInf.stateparam(0);
			inf.nParam2 = tInf.stateparam(1);
			inf.nParam3 = tInf.stateparam(2);
			//LOG_ERROR("pro", "fuck2:%d, %d", tInf.activityid(), inf.nParam3);
		}
		//LOG_ERROR("pro", "fuck3:%d, %d", tInf.activityid(), inf.nParam3);
		inf.nRealLastedTime = tInf.reallastedtime();
		inf.nLastedSavedTime = inf.nRealLastedTime;
		mActivityInfo.UpdateActivityInfo( tInf.activityid(), inf );
	}	
	mActivityInfo.mLastSave = pActivityInfo->savetm();

	mHasVoted = pActivityInfo->hasvoted();
}

void CPropertyPlayer::GetActivityInfoTlvBuffer( PBPlayerActivityInfo* pActivityInfo, bool bRealSave )
{
	pActivityInfo->set_diploidtime( mDiploidTime );
	pActivityInfo->set_lastrefreshtimeofdiploid( mLastRefreshTime );

	for( unsigned int i = 0; i < ARRAY_CNT( mDiploidState ); i++ )
	{
		// TODO: 定时器ID不能存盘，否则再次上线时会利用到这个ID，如果把正在用的TimerID超时掉，会产生不可预料的问题
		if( i == TIMEID_STATE || i == ACTIVITY_STATE )
		{
			pActivityInfo->add_diploidstate( 0 );
		}
		else
		{
			pActivityInfo->add_diploidstate( mDiploidState[ i ]  );
		}
	}

	pActivityInfo->set_dailywelfaresta( mDailyWelfareStatus );
	pActivityInfo->set_lastdaylydispostime( mLastDaylyDisposTime );
	pActivityInfo->set_lastweeklydispostime( mLastWeeklyDisposTime );
	pActivityInfo->set_lastfamilyapplytime( mLastFamilyApplyTime );
	pActivityInfo->set_lasttimeleavefamily( mLastTimeLeaveFamily );
	pActivityInfo->set_lastjudgetime( mLastJudgeTime );
	pActivityInfo->set_savedate( mSaveDate );
	pActivityInfo->set_vipflag( mVipFlag );
	pActivityInfo->set_viptime( mVipEndDate );
	// 当天奖励领取
	PBAwardRecord *tpRecord = pActivityInfo->mutable_awardrecord();
	for ( AwardList::iterator it = mAwardList[EALT_Daily].begin(); it != mAwardList[EALT_Daily].end(); ++it )
	{
		tpRecord->add_awardid( it->mAwardID );
		tpRecord->add_times( it->mTimes );
	}
	// 当周奖励领取
	tpRecord = pActivityInfo->mutable_weeklyawardrecord();
	for ( AwardList::iterator it = mAwardList[EALT_Weekly].begin(); it != mAwardList[EALT_Weekly].end(); ++it )
	{
		tpRecord->add_awardid( it->mAwardID );
		tpRecord->add_times( it->mTimes );
	}

	pActivityInfo->set_investstatus( mInvestStatus );

	for ( ACTIVITYINFO_TYPE::iterator it = mActivityInfo.mStates.begin(); it != mActivityInfo.mStates.end(); ++it )
	{
		// 先计算持续时间
		CEntityPlayer* tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity(GetEntityID());
		if ( tpPlayer != NULL )
		{
			it->second.nRealLastedTime = it->second.GetDuringTime(tpPlayer) ; // 如果不是本次上线改的 则从登陆算 否则从chgtm算
		}
		PBActivityStateInf* tpInf = pActivityInfo->add_activitystate();
		tpInf->set_activityid(it->first);
		tpInf->set_chgtm(it->second.chg_tm);
		tpInf->set_curstate(it->second.nState);
		tpInf->add_stateparam(it->second.nParam1);
		tpInf->add_stateparam(it->second.nParam2);
		tpInf->add_stateparam(it->second.nParam3);
		tpInf->set_reallastedtime(it->second.nRealLastedTime);
		//LOG_ERROR("Activity", "fuck:%d, %d", it->first, it->second.nRealLastedTime);
	}	
	if (bRealSave)
	{
		mActivityInfo.mLastSave = time(0);
		pActivityInfo->set_savetm(time(0));
	}

	pActivityInfo->set_hasvoted( mHasVoted );
}


// 造化blob
char* CPropertyPlayer::GetRoleTalentBlobStr( )
{
	static char cTalent[MAX_PRO_PB_DATA_LEN];
	static char szTalent[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szTalent;

	PBPlayerTalentInfo tPBTalentInfo;
	GetTalentInfoTlvBuffer( &tPBTalentInfo );

	if( tPBTalentInfo.SerializeToArray( cTalent, sizeof(cTalent) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szTalent)-1, cTalent, tPBTalentInfo.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szTalent;
}

void CPropertyPlayer::CreateTalentInfoFromTlv( PBPlayerTalentInfo* pTalentInfo )
{
	SetTalentExp( pTalentInfo->talentexp() );
	SetTalentPoint( pTalentInfo->talentpoint() );
	SetTalentStep( pTalentInfo->talentstep() );
	SetTalentStudy( pTalentInfo->talentstudy() );
	mTalentSkill.CreateFromTlv2(  pTalentInfo->mutable_talentskill() );
	SetTalentLevel( pTalentInfo->talentlevel() );
}

void CPropertyPlayer::GetTalentInfoTlvBuffer( PBPlayerTalentInfo* pTalentInfo )
{
	pTalentInfo->set_talentexp( GetTalentExp() );
	pTalentInfo->set_talentpoint( GetTalentPoint() );
	pTalentInfo->set_talentstep( GetTalentStep() );
	pTalentInfo->set_talentstudy( GetTalentStudy() );
	mTalentSkill.GetTlvBuffer2( pTalentInfo->mutable_talentskill() );
	pTalentInfo->set_talentlevel( GetTalentLevel() );
}

// mw blob
char* CPropertyPlayer::GetRoleMWBlobStr( )
{
	static char cMWInfo[MAX_PRO_PB_DATA_LEN];
	static char szMWInfo[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szMWInfo;

	PBPlayerMWInfo tPBMWInfo;
	GetMWInfoTlvBuffer( &tPBMWInfo );

	if( tPBMWInfo.SerializeToArray( cMWInfo, sizeof(cMWInfo) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(cMWInfo)-1, cMWInfo, tPBMWInfo.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szMWInfo;
}

void CPropertyPlayer::CreateMWInfoFromTlv( PBPlayerMWInfo* pMWInfo )
{
	SetAssignExpScl( pMWInfo->assignexpscl() );
}

void CPropertyPlayer::GetMWInfoTlvBuffer( PBPlayerMWInfo* pMWInfo )
{
	pMWInfo->set_assignexpscl( GetAssignExpScl() );
}

// 生活技能blob
char* CPropertyPlayer::GetRoleLifeSkillBlobStr( )
{
	static char cLifeSkill[MAX_PRO_PB_DATA_LEN];
	static char szLifeSkill[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szLifeSkill;

	PBLifeSkillBox tPBLifeSkillBox;
	mPlayerLifeSkill.CreatTlvPB( tPBLifeSkillBox );

	if( tPBLifeSkillBox.SerializeToArray( cLifeSkill, sizeof(cLifeSkill) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szLifeSkill)-1, cLifeSkill, tPBLifeSkillBox.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szLifeSkill;
}


// 妖魔志blob
char* CPropertyPlayer::GetRoleMonsterBlobStr( )
{
	static char cMonsterRecored[MAX_PRO_PB_DATA_LEN];
	static char szMonsterRecored[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szMonsterRecored;

	PBMonsterRecordInfo tPBMonsterRecord;
	GetMonsterRecordInfoTlvBuffer( &tPBMonsterRecord );

	if( tPBMonsterRecord.SerializeToArray( cMonsterRecored, sizeof(cMonsterRecored) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szMonsterRecored)-1, cMonsterRecored, tPBMonsterRecord.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szMonsterRecored;
}

void CPropertyPlayer::CreateMonsterInfoFromTlv( PBMonsterRecordInfo* pMonster)
{
	for ( unsigned int i = 0; i < ( unsigned int )pMonster->activecard_size(); i++ )
	{
		mActiveCardList.insert( pMonster->activecard( i ) );
	}
	for ( unsigned int i = 0; i < ( unsigned int )pMonster->owncard_size(); i++ )
	{
		mOwnCardList.insert( pMonster->owncard( i ) );
	}

}

void CPropertyPlayer::GetMonsterRecordInfoTlvBuffer( PBMonsterRecordInfo* pMonster )
{
	for ( CPropertyPlayer::CardList::iterator it = mActiveCardList.begin(); it != mActiveCardList.end(); ++it )
	{
		pMonster->add_activecard( *it );
	}
	for ( CPropertyPlayer::CardList::iterator it = mOwnCardList.begin(); it != mOwnCardList.end(); ++it )
	{
		pMonster->add_owncard( *it );
	}
}


// 统计信息blob
char* CPropertyPlayer::GetRoleStatisticBlobStr( )
{
	static char cStatisticInfo[MAX_PRO_PB_DATA_LEN];
	static char szStatisticInfo[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szStatisticInfo;

	PBPlayerStatisticInfo tPBStatistic;
	GetStatisticTlvBuffer( &tPBStatistic );

	if( tPBStatistic.SerializeToArray( cStatisticInfo, sizeof(cStatisticInfo) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szStatisticInfo)-1, cStatisticInfo, tPBStatistic.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szStatisticInfo;
}

void CPropertyPlayer::CreateStatisticFromTlv( PBPlayerStatisticInfo* pStatistic )
{
	mCompletedTasks = pStatistic->completedtasks();
	mOriginReliveTimes = pStatistic->originrelivetimes();
	mKillSameNationNum = pStatistic->killsamenationnum();
	mKillOgreNum = pStatistic->killogrenum();
	mUpFailNum = pStatistic->upfailnum();
	mTodayKillEnemyNumIn = pStatistic->todaykillenemyin();
	mTodayKillEnemyNumOut = pStatistic->todaykillenemyout();
	mDieNum = pStatistic->dienum();
}

void CPropertyPlayer::GetStatisticTlvBuffer( PBPlayerStatisticInfo* pStatistic  )
{
	pStatistic->set_completedtasks(mCompletedTasks);
	pStatistic->set_originrelivetimes(mOriginReliveTimes);
	pStatistic->set_killsamenationnum(mKillSameNationNum);
	pStatistic->set_killogrenum(mKillOgreNum);
	pStatistic->set_upfailnum(mUpFailNum);
	pStatistic->set_todaykillenemyin(mTodayKillEnemyNumIn);
	pStatistic->set_todaykillenemyout(mTodayKillEnemyNumOut);
	pStatistic->set_dienum(mDieNum);
}

// 才华 blob
char* CPropertyPlayer::GetRoleWizardBlobStr( )
{
	static char cWizard[MAX_PRO_PB_DATA_LEN];
	static char szWizard[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szWizard;

	PBWizard tPBWizard;
	mWizard.GetTlvBuffer( &tPBWizard );	
	
	if( tPBWizard.SerializeToArray( cWizard, sizeof(cWizard) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szWizard)-1, cWizard, tPBWizard.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szWizard;
}


// 客户度blob信息
char* CPropertyPlayer::GetRoleClientInfoBlobStr( )
{
	static char cOtherInfo[MAX_PRO_PB_DATA_LEN];
	static char szOtherInfo[MAX_PRO_PB_DATA_LEN];
	char* pszTmpStr = szOtherInfo;

	PBPlayerClientInfo tPBClientInfo;
	GetRoleClientInfoTlvBuffer( &tPBClientInfo );

	if( tPBClientInfo.SerializeToArray( cOtherInfo, sizeof(cOtherInfo) )  == false ) 
	{
		LOG_FATAL( "pro", "[%s:%d] player %d serialize to array failed", __FUNCTION__, __LINE__,GetEntityID());
		return NULL;
	}

	int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szOtherInfo)-1, cOtherInfo, tPBClientInfo.ByteSize() );
	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return NULL);

	return szOtherInfo;
}

void CPropertyPlayer::CreateMatrixInfoFromTlv( PBMatrixInfo* pMatrix)
{	
	for ( int i = 0; i < pMatrix->masterid_size() && i < MAX_MASTER_NUM; i++ )
	{
		mMasterID[ i ] = pMatrix->masterid(i);
	}
	for ( int i = 0; i < pMatrix->slaveid_size(); i++ )
	{
		if ( pMatrix->slaveid(i) == 0 )
			continue;
		mMatrixList.push_back( pMatrix->slaveid(i) );
	}
	for ( int i = 0; i < MAX_MATRIX_LEVEL && i < pMatrix->slavevalue_size(); i++ )
	{
		mSlaveValue[ i ] = pMatrix->slavevalue(i);
	}
	mSlavePoint = pMatrix->slavepoint();
	mMasterPoint = pMatrix->masterpoint();
	mSlaveTotal = pMatrix->slavetotal();
	mMasterTotal = pMatrix->mastertotal();
	for ( int i = 0; i < pMatrix->masterpercent_size() && i < MAX_MASTER_NUM; i++ )
	{
		mMasterPercent[ i ] = pMatrix->masterpercent(i);
	}
	mMasterTimes = pMatrix->mastertimes();
	mSlaveNum = std::max( 0, (int)(pMatrix->slavenum()) );
	mMasterGive = pMatrix->mastergive();
	mBeginnerNum = std::max( 0, (int)(pMatrix->beginnernum()) );
	mFormalNum = std::max( 0, (int)(pMatrix->formalnum()) );
}

void CPropertyPlayer::GetMatrixRecordInfoTlvBuffer( PBMatrixInfo* pMatrix )
{
	for ( int i = 0; i < MAX_MASTER_NUM; i++ )
	{
		pMatrix->add_masterid( mMasterID[ i ] );
	}
	for ( MatrixList::iterator it = mMatrixList.begin(); it != mMatrixList.end(); ++it )
	{
		pMatrix->add_slaveid( *it );
	}
	for ( int i = 0; i < MAX_MATRIX_LEVEL; i++ )
	{
		pMatrix->add_slavevalue( mSlaveValue[ i ] );
	}
	pMatrix->set_slavepoint( mSlavePoint );
	pMatrix->set_masterpoint( mMasterPoint );
	pMatrix->set_slavetotal( mSlaveTotal );
	pMatrix->set_mastertotal( mMasterTotal );
	for ( int i = 0; i < MAX_MASTER_NUM; i++ )
	{
		pMatrix->add_masterpercent( mMasterPercent[ i ] );
	}
	pMatrix->set_mastertimes( mMasterTimes );
	pMatrix->set_slavenum( mSlaveNum );
	pMatrix->set_mastergive( mMasterGive );
	pMatrix->set_beginnernum( mBeginnerNum );
	pMatrix->set_formalnum( mFormalNum );
}

void CPropertyPlayer::CreateRoleClientInfoFromTlv( PBPlayerClientInfo* pClientInfo )
{
	mShortcut.CreateFromTlv2( pClientInfo->mutable_shortcut() );
}
void CPropertyPlayer::GetRoleClientInfoTlvBuffer( PBPlayerClientInfo* pClientInfo )
{
	mShortcut.GetTlvBuffer2( pClientInfo->mutable_shortcut() );
}

int CPropertyPlayer::GetDuckRate( )
{
	return mSkill.GetFixNumber( CTemplateSkill::PASSINC_DUCKRATE ) + 
		mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DUCKRATE );
}

int CPropertyNPC::GetDuckRate( )
{
	return mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DUCKRATE );
}

int CPropertyPet::GetDuckRate( )
{
	return mBuffList.GetFixNumber( CTemplateBuff::BUFFINC_DUCKRATE );
}

ComplexPos* CPropertyPlayer::GetTelePos( unsigned int nIndex )
{ 
	if (nIndex >= 0 && nIndex < mTeleList.size() ) 
		return &mTeleList[nIndex]; 
	else 
		return NULL; 
}

int CPropertyPlayer::AddTelePos( const ComplexPos& rPos )
{
	if ( mTeleList.size() >= TELE_LIST_SIZE )
		return -1;
	mTeleList.push_back(rPos);
	return 0;
}

int CPropertyPlayer::DelTelePos( unsigned int nIndex )
{
	if ( nIndex < 0 || nIndex >= mTeleList.size() ) 
		return -1;

	TELE_LIST_TYPE::iterator _del = mTeleList.end();

	for ( TELE_LIST_TYPE::iterator it = mTeleList.begin(); it != mTeleList.end(); ++it )
	{
		if ( it - mTeleList.begin() == (int)nIndex )
		{
			_del = it;
			break;
		}
	}

	if (_del != mTeleList.end())
	{
		mTeleList.erase(_del);
		return 0;
	}
	return -1;
}

// 获取玩家双倍总值 [1/5/2010 Macro]
int CPropertyPlayer::GetTotalDiploid()
{
	CTemplateDiploidTimeTable* tpDiploid = ( CTemplateDiploidTimeTable* ) CDataStatic::GetTemp( TEMP_DOUBLE );
	if ( tpDiploid == NULL )
	{
		return 0;
	}

	int tDoubleTime = 0;
	switch ( GetVipFlag() )
	{
	case EM_VIP_NORMAL:
		{
			tDoubleTime = tpDiploid->mTotalTime + CSceneCfgManager::GetSingleton().GetProperty_cfg()->mVip_normal * 3600000;
			break;
		}
	default:
		{
			tDoubleTime = tpDiploid->mTotalTime;
			break;
		}
	}

	return tDoubleTime;
}

//  获取密码状态
int CRolePassword::GetStatus( )
{  
	// 检查是否已经过了保护期	
	return mStatus;
}

// 设置密码
int CRolePassword::SetPassword( const char *pPassword )
{  
	if ( pPassword == NULL )
	{
		return ERR_PASSWORD_SETPASSWORDWRONG;
	}	
	memset( mPassword, 0, sizeof( mPassword ) );
	strncpy( mPassword, pPassword, MAX_PASSWORD_LENGTH );
	return SUCCESS;				
}

// 初始化密码状态
int CRolePassword::InitialPass( const char *pPassword, int nProtectTime, int nProtectTimeLength, int nPwdStatus )
{	
	if ( pPassword == NULL )
	{
		return -1;
	}
	
	strncpy( mPassword, pPassword, MAX_PASSWORD_LENGTH - 1 );
	mPassword[MAX_PASSWORD_LENGTH-1] = '\0';
	mProtectTime = nProtectTime;
	//mProtectTimeLength = nProtectTimeLength;
	
	// 检查是否有密码
	if ( mPassword[0] == '\0' )
	{
		mStatus = EM_PASSWORD_NOTSET;
	}
	else
	{
		mStatus = EM_PASSWORD_PASSLOCKED;
	}
	
	// 已经通过密码验证
	if ( nPwdStatus == EM_PASSWORD_PASSFREE )
	{
		mStatus = EM_PASSWORD_PASSFREE;
	}
	
	/*if ( time(NULL) < mProtectTime && mStatus == EM_PASSWORD_PASSLOCKED )
	{
		mStatus = EM_PASSWORD_PROTECTTIME;
	}*/

	return SUCCESS;
}

// 检查密码
bool CRolePassword::CheckPassword( const char *pPassword )
{
	if ( pPassword == NULL )
	{
		return false;
	}
	
	if ( strncmp( mPassword, pPassword, MAX_PASSWORD_LENGTH ) == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}	
}

// 检查是否允许相关操作
int CRolePassword::CheckActionAllowed( EPassWordAction emAction )
{		
	CEntityPlayer* tpPlayer = (CEntityPlayer*) CSceneLogic::GetSingletonPtr( )->GetEntity( mEntityID );
	if ( tpPlayer != NULL)
	{
		if ( CGMModule::GetSingleton().IsGmTrusted(tpPlayer->GetGmStatus()))
		{
			return SUCCESS;	
		}
	}

	if ( CheckProtectTime() == false )
	{
		return ERR_PASSWORD_INPROTECTTIME;
	}
	
	int tAllowed = ERR_PASSWORD_ACTIONNOTALLOWED;
	switch( GetStatus() )				
	{
		case EM_PASSWORD_NOTSET:
		{
			tAllowed = SUCCESS;
			break;
		}
				
		case EM_PASSWORD_PASSLOCKED:
		{	
			if ( emAction == EM_PASSWORD_ACTION_SETPASSWORD )
			{
				tAllowed = SUCCESS;
			}
			break;
		}
		
		case EM_PASSWORD_PASSFREE:
		{
			tAllowed = SUCCESS;
			break;
		}
		default:
			break;
	}
	return tAllowed;
}

// 检查是否已经过了安全保护时间
bool CRolePassword::CheckProtectTime()
{
	return time( NULL )	>= mProtectTime;
}

// 清空密码
void CRolePassword::ClearPassword( )
{
	memset( mPassword, 0, sizeof( mPassword ) );
}

// 增加有效离线时间
int CExpTimeInfo::AddEffectiveOffTimeLength( int nLoginTime, bool bVip /* = false */ )
{
	CTemplateActivityConfig *tpConfig = ( CTemplateActivityConfig * )CDataStatic::mActivityConfig;
	LK_ASSERT( tpConfig != NULL,return -1 )
	int tTimeLength =  time(NULL) - nLoginTime;
		
	// 计算和上次离线时同一天时间内应该累积的时间
	int tLeftTime = tTimeLength;
	
	// 算出上次离线时间当天零点的time值
	tm *tpLocalTime = localtime( (time_t *)&nLoginTime );
	tpLocalTime->tm_hour = 0;
	tpLocalTime->tm_min = 0;
	tpLocalTime->tm_sec = 0;	
	int tBeginTime = mktime( tpLocalTime ) + DAY_TIME;
	
	int tExchangeRate = tpConfig->mEffectiveOffLineExchangeRate;
	if ( bVip == true )
	{
		tExchangeRate = tpConfig->mVipOffLineExchangeRate;
	}
	
	// 起始第一天的时间	
	int tBeginOnLineTime = ( tBeginTime - nLoginTime ) > tLeftTime ? tLeftTime : tBeginTime - nLoginTime;	
	
	// 如果本次登录时间和上次离线时间属于同一天，计算玩家在这一天内中的有效离线时间(不能超过每天的上限)			
	// 本次登录时间和上次登录时间不在同一天,则清空最后一次计算在线时间时当天的在线时间累加值
	if ( IsANewDay( mLastCalculateTime, nLoginTime ) == true )
	{										
		mLastEffectiveTimeLength = 0;
	}
	
	if ( tBeginOnLineTime + mLastEffectiveTimeLength > tpConfig->mMaxOnLineEffectiveTime*3600 )
	{
		mEffectiveOffTimeLength += ( tpConfig->mMaxOnLineEffectiveTime*3600 - mLastEffectiveTimeLength )*tExchangeRate;
		mLastEffectiveTimeLength = tpConfig->mMaxOnLineEffectiveTime*3600;
	}
	else
	{	
		mEffectiveOffTimeLength += tBeginOnLineTime*tExchangeRate;
		mLastEffectiveTimeLength += tBeginOnLineTime;
	}
	
	tLeftTime -= tBeginOnLineTime;		
		
	// 计算其余应该累积的时间
	if ( tLeftTime > 0 )
	{
		// 计算剩余时间中有几个整天
		int tDay = tLeftTime/DAY_TIME;
		if ( tDay >= 0 )
		{
			mEffectiveOffTimeLength = tpConfig->mMaxOnLineEffectiveTime*3600*tExchangeRate*tDay; 
		}
		
		// 计算剩余时间中最后未满一天的有效离线时间
		int tTime = tLeftTime%DAY_TIME;
		tTime = tTime > tpConfig->mMaxOnLineEffectiveTime*3600 ? tpConfig->mMaxOnLineEffectiveTime*3600	: tTime;
		mEffectiveOffTimeLength += tTime*tExchangeRate;
		mLastEffectiveTimeLength = tTime;
	}			
	
	// 判断有没有超过上限
	if ( mEffectiveOffTimeLength > tpConfig->mMaxEffectiveOffLineTime*3600 )
	{
		mEffectiveOffTimeLength = tpConfig->mMaxEffectiveOffLineTime*3600;
	}
		
	mLastCalculateTime = time( NULL );
	return SUCCESS;
}

// 兑换有效离线时间
int CExpTimeInfo::ExchangeEffectiveOffLineTime( int nTime )
{			
	CTemplateActivityConfig *tpConfig = ( CTemplateActivityConfig * )CDataStatic::mActivityConfig;
	LK_ASSERT( tpConfig != NULL,return -1 )	 
	nTime = mLastCalculateTime;
	
	if ( nTime <= 0 )
	{
		return -1;
	}
	
	int tTimeNow = time( NULL );
	int tTotalOffLineTime = tTimeNow - nTime;
	int tEffectiveOffLineTime = 0;
	
	tm *tpLocalTime = localtime( (time_t *)&nTime );
	tpLocalTime->tm_hour = 0;
	tpLocalTime->tm_min = 0;
	tpLocalTime->tm_sec = 0;	
	int tBeginTime = mktime( tpLocalTime )+DAY_TIME;
	
	// 起始第一天的时间	
	int tBeginOffLineTime = ( tBeginTime - nTime ) > tTotalOffLineTime ? tTotalOffLineTime : tBeginTime - nTime;
	
	// 获取第一天的离线时间
	if ( tBeginOffLineTime > 0  )
	{
		// 计算第一天的离线时间是否超过上限
		if ( tBeginOffLineTime > tpConfig->mMaxOffLineTime*3600 )
		{
			tEffectiveOffLineTime += tpConfig->mMaxOffLineTime*3600;
		}
		else
		{
			tEffectiveOffLineTime += tBeginOffLineTime;
		}
	}
	
	// 计算除第一天外的剩余时间
	tTotalOffLineTime -= tBeginOffLineTime;	 		
	
	// 中间的天数
	int tOffLineDay = tTotalOffLineTime/DAY_TIME;
	if ( tOffLineDay > 0 )
	{
		tEffectiveOffLineTime += tOffLineDay*tpConfig->mMaxOffLineTime*3600;	
	}
	
	// 最后的一天的离线时间		
	int tEndOffLineTime = tTotalOffLineTime%DAY_TIME;
	if ( tEndOffLineTime > 0 )
	{
		// 计算是否超过上限
		if ( tEndOffLineTime > tpConfig->mMaxOffLineTime*3600 )
		{
			tEffectiveOffLineTime += tpConfig->mMaxOffLineTime*3600;
		}
		else
		{
			tEffectiveOffLineTime += tEndOffLineTime;
		}
	}
	
	// 实际离线时间,不能超过上限
	tEffectiveOffLineTime = ( tEffectiveOffLineTime + mOffLineTimeLength ) >  tpConfig->mOffLineTimeLimit*3600 ?
		tpConfig->mOffLineTimeLimit*3600 - mOffLineTimeLength :	 tEffectiveOffLineTime;
	if ( tEffectiveOffLineTime < 0  )
	{
		tEffectiveOffLineTime = 0;
	}
	
	if ( tEffectiveOffLineTime > mEffectiveOffTimeLength )
	{
		mOffLineTimeLength += mEffectiveOffTimeLength;
		mEffectiveOffTimeLength = 0;
	}
	else
	{
		mOffLineTimeLength += tEffectiveOffLineTime;
		mEffectiveOffTimeLength -= tEffectiveOffLineTime;
	}			
		
	return SUCCESS;
}

// 黑狗丸时间兑换成经验
int CExpTimeInfo::ExchangeOffLineTimeToExp( int nLevel, int &rExp, int &rOffLineTimeConsumed, int nBlackPillsID /* = 0 */ )
{  
	rExp = 0;
	rOffLineTimeConsumed = 0;
	
	if ( mBlackPillsTime > 0 && nBlackPillsID > 0 && nBlackPillsID != mBlakcPillID )
	{
		CTplItem *tpTplItem = ( CTplItem * )CDataStatic::SearchTpl( nBlackPillsID );
		if ( tpTplItem == NULL )
		{
			return ERROR_BLACKPILLS_DIFFRENTBLACKPILLID;
		}	
		
		if ( mBlakcPillID > 0 )
		{
			CTplItem *tpOldBlackPills = ( CTplItem * )CDataStatic::SearchTpl( mBlakcPillID );
			if ( tpOldBlackPills == NULL )
			{
				return ERROR_BLACKPILLS_DIFFRENTBLACKPILLID;
			}
			
			if ( tpOldBlackPills->mCommonID != tpTplItem->mCommonID )
			{
				return ERROR_BLACKPILLS_DIFFRENTBLACKPILLID;
			}
		}				
	}						    
	
	if ( nBlackPillsID > 0 )
	{
		CTplItemBlackPills *tpTplBlackPill = ( CTplItemBlackPills * )CDataStatic::SearchTpl( nBlackPillsID );
		LK_ASSERT( tpTplBlackPill != NULL, return -1 )		
		if( tpTplBlackPill->mExchangeTime*60 + mBlackPillsTime >= MAX_BLACKPILLS_TIME )
		{	
			return ERROR_BLACKPILLS_MAXTIME;	
		}	
		
		mBlackPillsTime +=	tpTplBlackPill->mExchangeTime*60;	 
	}
	
	if ( mBlackPillsTime > ( mOffLineTimeLength - mOffLineTimeLength%60 ) )
	{
		rOffLineTimeConsumed = mOffLineTimeLength - mOffLineTimeLength%60;
		mBlackPillsTime -= rOffLineTimeConsumed;
		mOffLineTimeLength = mOffLineTimeLength%60;
	}
	else
	{
		rOffLineTimeConsumed = mBlackPillsTime;		
		mOffLineTimeLength -= rOffLineTimeConsumed;			
		mBlackPillsTime = 0;
	}  		
	
	// 计算经验值
	if ( mBlakcPillID == 0 && nBlackPillsID > 0 )
	{
		mBlakcPillID = nBlackPillsID;
	}
	
	if ( rOffLineTimeConsumed > 0 )
	{
		CTplItemBlackPills *tpTplBlackPill = ( CTplItemBlackPills * )CDataStatic::SearchTpl( mBlakcPillID );
		LK_ASSERT( tpTplBlackPill != NULL, return -1 )
		CTemplateQuestionExpTable *tpTplExpTable= ( CTemplateQuestionExpTable * )CDataStatic::SearchTpl( tpTplBlackPill->mExpTableID );
		LK_ASSERT( tpTplExpTable != NULL, return -1 )
		rExp = (rOffLineTimeConsumed/60)*tpTplExpTable->mExp[nLevel];
	}
	
	if ( mBlackPillsTime == 0 )
	{
		mBlakcPillID = 0;
	}
		
	return SUCCESS;
}

int CExpTimeInfo::GetExpFromOffLine( int nLevel )
{
	CTemplateOffLineExpConfig *tpConfig = ( CTemplateOffLineExpConfig * )CDataStatic::GetTemp( TEMP_OFFLINEEXPCONFIG );
	LK_ASSERT( tpConfig != NULL,return -1 );

	if ( nLevel < tpConfig->mOffLineLevel || nLevel >= TMP_MAX_GRADE )
	{
		return -1;
	}

	int tTotalOffLineTime = time( NULL ) - mLastCalculateTime;
	int tOffExp = tTotalOffLineTime / tpConfig->mOffLineUnit * tpConfig->mOffLineExp[ nLevel ] ;
	// 如果时间超出了范围。给个最大值。
	if ( tOffExp < 0 )
	{
		tOffExp = tpConfig->mExpLimit[ nLevel ];
	}
	mExpFromOffLine += tOffExp;
	if ( mExpFromOffLine > tpConfig->mExpLimit[ nLevel ] )
	{
		mExpFromOffLine = tpConfig->mExpLimit[ nLevel ];
	}
	return 0;
}

int CExpTimeInfo::GetExpByRate( int nExp )
{
	CTemplateOffLineExpConfig *tpConfig = ( CTemplateOffLineExpConfig * )CDataStatic::GetTemp( TEMP_OFFLINEEXPCONFIG );
	LK_ASSERT( tpConfig != NULL,return 0 );

	int tOffLineExp = nExp / SERVER_PERCENT_FLOAT * tpConfig->mOffLinePercent;
	if ( mExpFromOffLine < tOffLineExp )
	{
		tOffLineExp = mExpFromOffLine;
		mExpFromOffLine = 0;
		return tOffLineExp;
	}
	mExpFromOffLine -= tOffLineExp;
	return tOffLineExp;
}

void CExpTimeInfo::SetFromPB( PBExpTimeInfo &rPBInfo )
{
	mOffLineTimeLength = rPBInfo.offlinetimelength();				
	mEffectiveOffTimeLength = rPBInfo.effectiveofftimelength();
	mLastEffectiveTimeLength = rPBInfo.lasteffectivetimelength();
	mLastCalculateTime = rPBInfo.lastcalculatetime();
	mBlackPillsTime = rPBInfo.blackpillstime();
	mBlakcPillID = rPBInfo.blackpillid();
	mExpFromOffLine = rPBInfo.expfromoffline();
}
void CExpTimeInfo::CreatTlvPB( PBExpTimeInfo &rPBInfo )
{
	rPBInfo.set_offlinetimelength( mOffLineTimeLength );				
	rPBInfo.set_effectiveofftimelength( mEffectiveOffTimeLength );
	rPBInfo.set_lasteffectivetimelength( mLastEffectiveTimeLength );
	rPBInfo.set_lastcalculatetime( mLastCalculateTime );
	rPBInfo.set_blackpillstime( mBlackPillsTime );
	rPBInfo.set_blackpillid( mBlakcPillID );
	rPBInfo.set_expfromoffline( mExpFromOffLine );
}

void CPropertyPlayer::CreateCharmInfoFromTlv( PBCharm* pInfo )
{
	if ( pInfo==NULL )
		return;
	mCharmValue = pInfo->charmvalue();
	mWeekIncCharm = pInfo->weekinccharm();
	mLastWeekIncCharm = pInfo->lastweekinccharm();
}

void CPropertyPlayer::GetCharmInfoTlvBuffer( PBCharm* pInfo )
{
	if ( pInfo==NULL )
		return;
	pInfo->set_charmvalue(mCharmValue);
	pInfo->set_weekinccharm(mWeekIncCharm);
	pInfo->set_lastweekinccharm(mLastWeekIncCharm);
}

void CPropertyPlayer::CreateTimesInfoFromTlv( PBTimes* pInfo )
{
	mTimesMgr.CreateFromPB(pInfo);
}

void CPropertyPlayer::GetTimesInfoTlvBuffer( PBTimes* pInfo )
{
	mTimesMgr.GetPB(pInfo);
}

void CPropertyPlayer::DeleteItem( )
{
	CEntityPlayer *tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity( GetEntityID() );
	if ( tpPlayer == NULL )
	{
		return;
	}
	mBaggage.DeleteUnusedItem( tpPlayer->GetCharID() );
	mStorage.DeleteUnusedItem( tpPlayer->GetCharID() );
	mEquipment.DeleteUnusedItem( tpPlayer->GetCharID() );
}


void CPropertyPlayer::RemoveBuffOnLogOff( )
{
	std::vector< int > tVec;
	for ( size_t i = 0; i < mBuffList.mBuffNumber; i++  )
	{
		CTemplateBuff* tpBuffTpl = (CTemplateBuff*) CDataStatic::SearchTpl( mBuffList.mBuffList[ i ].mBuffID );
		if ( tpBuffTpl == NULL )
			continue;
		if ( tpBuffTpl->mOffRemove != 0 )
		{
			tVec.push_back( tpBuffTpl->mTempID );
		}
	}

	CEntityPlayer *tpPlayer = (CEntityPlayer*)this->GetSelfEntity();
	std::vector< int >::iterator it = tVec.begin();
	for ( ; it != tVec.end(); ++it )
	{
		tpPlayer->EntityRemoveBuff( *it );
		LogEventLostBuffByLogOff(tpPlayer,*it );  // 下线的时候失去BUFF
	}
}

/* --------------------------------------------------------------------------*/
/** 
 * @Synopsis 技能cd刷新 
 * 
 * @Param pSkillObj
 * @Param pDesEntity
 * 
 * @Returns   
 */
/* ----------------------------------------------------------------------------*/
bool CPropertyPlayer::OnSkillCDRefresh( CSkillObject* pSkillObj, CEntityPlayer* pDesEntity)
{

	LK_ASSERT( pDesEntity != NULL && pSkillObj != NULL, return false );


    LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] player %s Skill %d CD refresh", __LK_FILE__, __LINE__, __FUNCTION__, pDesEntity->GetCharNameShow(), pSkillObj->GetSkillID( ));


    pSkillObj->SetCooldown( 0 );

   // CTimerMng::GetSingletonPtr()->ClearTimer( pSkillObj->GetCDTimer());
    
    pDesEntity->GetTimerReciver()->ClearTimer( pSkillObj->GetCDTimer() );
    pSkillObj->SetCDTimer( INVALID_OBJ_ID );

    //??FUNC_COOLDOWN can reuse?
    pDesEntity->AddFunResult( pDesEntity->GetEntityID(), FUNC_COOLDOWN, RESULT_NORMAL, false,
            pSkillObj->GetSkillID( ),  pSkillObj->GetSkillLevel());

    return true;
}
/* --------------------------------------------------------------------------*/
/* 
 * @Synopsis  刷新技能组cd
 * 
 * @Param rGroupCD
 * @Param pDesEntity
 * 
 * @Returns   
 */
/* ----------------------------------------------------------------------------*/

bool CPropertyPlayer::OnGroupCDRefresh(CGroupCD& rGroupCD, CEntityPlayer* pDesEntity)
{

	LK_ASSERT( pDesEntity != NULL , return false );

    rGroupCD.SetCDTime(0);

    pDesEntity->GetTimerReciver()->ClearTimer( rGroupCD.GetTimerID() );
    rGroupCD.SetTimerID( INVALID_OBJ_ID );

    //FUNC_PHCOOLDOWN !!!!!confused
    pDesEntity->AddFunResult( pDesEntity->GetEntityID(), FUNC_PHCOOLDOWN, RESULT_NORMAL, false,
            rGroupCD.GetCDGroupIdx(), 0);
   
    return true;
}


/*玩家的聚宝盆信息**/ 

// ***********************************************************
//  Function:		Initialize
//  Description:	初始化聚宝盆的配置信息
//  Return:			
//  Others:
//  Date:	11/05/2010
// **********************************************************
int CTreasureBowlInfo::Initialize()
{
	mTotalNum = 0;
	mExchangeTime = 0;
	mTreasurebowlList.initailize();
	return SUCCESS;
}

// ***********************************************************
//  Function:		RefreshExchangeTime
//  Description:	刷新聚宝兑换的时间
//  Return:			
//  Others:
//  Date:	11/05/2010
// **********************************************************
int CTreasureBowlInfo::RefreshExchangeTime()
{
	if ( IsANewDay( mExchangeTime, time( NULL ) ) == true )
	{
		ClearTreasureBowlList();
	}	
	return SUCCESS;
}

// ***********************************************************
//  Function:		CheckExchangeItem
//  Description:	检查某种物品是否可以再聚宝
//  Return:			
//  Others:
//  Date:	11/05/2010
// **********************************************************
int CTreasureBowlInfo::CheckExchangeItem( int nTotalMaxTimes, int nMaxTimes, int nItemID )
{
	if ( mTotalNum >= nTotalMaxTimes )
	{
		return	CTREASUREBOWL_TOTALTIMELIMIT;
	}
	CTreasureBowlList::iterator it = mTreasurebowlList.find( nItemID );
	if ( it != mTreasurebowlList.end() )
	{
		if ( it->second >= nMaxTimes )
		{
			return CTREASUREBOWL_TIMELIMIT;
		}
		else
		{
			return SUCCESS;
		}
	}
	return SUCCESS;
}

// ***********************************************************
//  Function:		SetExchangeItem
//  Description:	设置用于聚宝的某个物品信息
//  Return:			
//  Others:
//  Date:	11/05/2010
// **********************************************************
int CTreasureBowlInfo::SetExchangeItem( int nItemID )
{
	CTreasureBowlList::iterator it = mTreasurebowlList.find( nItemID );
	if ( it != mTreasurebowlList.end() )
	{
		++it->second;
	}
	else
	{
		mTreasurebowlList.insert( CTreasureBowlList::value_type( nItemID, 1 ) );
	}
	
	++mTotalNum;	
	mExchangeTime = time( NULL );
	return SUCCESS;
}

// ***********************************************************
//  Function:		ClearTreasureBowlList
//  Description:	清空玩家的聚宝信息
//  Return:			
//  Others:
//  Date:	11/05/2010
// **********************************************************
int CTreasureBowlInfo::ClearTreasureBowlList()
{
	mTotalNum = 0;
	mTreasurebowlList.initailize();
	mExchangeTime = 0;
	return SUCCESS;	
}

// ***********************************************************
//  Function:		GetExchangeItem
//  Description:	获取某种物品聚宝信息
//  Return:			
//  Others:
//  Date:	11/05/2010
// **********************************************************
int CTreasureBowlInfo::GetExchangeItem( int nItemID, int &rUsedTimes )
{
	rUsedTimes = 0;
	CTreasureBowlList::iterator it = mTreasurebowlList.find( nItemID );
	if ( it != mTreasurebowlList.end() )
	{
		rUsedTimes = it->second;
	}
	return SUCCESS;
}

// ***********************************************************
//  Function:		SetExchangeItem
//  Description:	设置用于聚宝的某个物品信息
//  Return:			
//  Others:
//  Date:	11/05/2010
// **********************************************************
void CTreasureBowlInfo::SetFromPB( PBTreasureBowl &rTreasureBowl )
{
	mTotalNum = rTreasureBowl.totalnum();
	mExchangeTime = rTreasureBowl.exchangetime();
	for ( int i = 0; i < rTreasureBowl.treasurebowlitem_size(); ++i )
	{	
		mTreasurebowlList.insert( CTreasureBowlList::value_type( rTreasureBowl.mutable_treasurebowlitem( i )->itemid(),  rTreasureBowl.mutable_treasurebowlitem( i )->usedtimes() ) );		
	}		
}

// ***********************************************************
//  Function:		SetExchangeItem
//  Description:	设置用于聚宝的某个物品信息
//  Return:			
//  Others:
//  Date:	11/05/2010
// **********************************************************
void CTreasureBowlInfo::CreateTlvPB( PBTreasureBowl &rTreasureBowl )
{
	rTreasureBowl.set_totalnum( mTotalNum );	
	rTreasureBowl.set_exchangetime( mExchangeTime );
	CTreasureBowlList::iterator it = mTreasurebowlList.begin();
	for ( ; it != mTreasurebowlList.end(); ++it )
	{
		PBTreasureBowlItem *pbTreasureBowlItem = rTreasureBowl.add_treasurebowlitem();
		pbTreasureBowlItem->set_itemid( it->first );
		pbTreasureBowlItem->set_usedtimes( it->second );
	}
}

int SinglePlayerActivityInfo::GetDuringTime(CEntityPlayer* pPlayer) // 得到从活动开始到现在走过的时间
{
	if ( pPlayer == NULL )
		return nLastedSavedTime;

	if (!bIsChanged)
	{
		// 如果是在本次登录之前
		return (nLastedSavedTime + (time(0)-pPlayer->GetLastLoginTime()));
	}
	else
	{
		// 如果是在本次登录之后更新的 则直接减
		return (time(0)-chg_tm);
	}
}

CSpiritTypeInfo::CSpiritTypeInfo()
{
	memset(m_anSlotOpenStatus, 0, sizeof(m_anSlotOpenStatus));
	memset(m_anCastingItemID, 0, sizeof(m_anCastingItemID));
	memset(m_anBindStatus, 0, sizeof(m_anBindStatus));
}

// 获取、设置开孔状态
int CSpiritTypeInfo::GetSlotOpenStatus(int nSlotIndex)
{
	if (nSlotIndex < 0 || nSlotIndex >= MAX_CASTINGSPIRIT_SLOT)
	{
		return 0;
	}

	return m_anSlotOpenStatus[nSlotIndex];
}
void CSpiritTypeInfo::SetSlotOpenStatus(int nSlotIndex, int nStatus)
{
	if (nSlotIndex < 0 || nSlotIndex >= MAX_CASTINGSPIRIT_SLOT)
	{
		return;
	}

	m_anSlotOpenStatus[nSlotIndex] = nStatus;
}

// 获取、设置铸灵用的道具id
int CSpiritTypeInfo::GetCastingItemID(int nSlotIndex)
{
	if (nSlotIndex < 0 || nSlotIndex >= MAX_CASTINGSPIRIT_SLOT)
	{
		return 0;
	}

	return m_anCastingItemID[nSlotIndex];
}

void CSpiritTypeInfo::SetCastingItemID(int nSlotIndex, int nItemID)
{
	if (nSlotIndex < 0 || nSlotIndex >= MAX_CASTINGSPIRIT_SLOT)
	{
		return;
	}

	m_anCastingItemID[nSlotIndex] = nItemID;
}

int CSpiritTypeInfo::GetBindStatus(int nSlotIndex)
{
	if (nSlotIndex < 0 || nSlotIndex >= MAX_CASTINGSPIRIT_SLOT)
	{
		return 0;
	}

	return m_anBindStatus[nSlotIndex];
}

void CSpiritTypeInfo::SetBindStatus(int nSlotIndex, int nBindStatus)
{
	if (nSlotIndex < 0 || nSlotIndex >= MAX_CASTINGSPIRIT_SLOT)
	{
		return;
	}

	m_anBindStatus[nSlotIndex] = nBindStatus;
}

CSpiritPositionInfo::CSpiritPositionInfo()
{
	memset(m_SpiritTypeInfo, 0, sizeof(m_SpiritTypeInfo));
}

int CSpiritPositionInfo::GetSlotOpenStatus(int nTypeIndex, int nSlotIndex)
{
	if (nTypeIndex < 0 || nTypeIndex >= MAX_CASTINGSPIRIT_TYPE)
	{
		return 0;
	}

	return m_SpiritTypeInfo[nTypeIndex].GetSlotOpenStatus(nSlotIndex);
}

void CSpiritPositionInfo::SetSlotOpenStatus(int nTypeIndex, int nSlotIndex, int nStatus)
{
	if (nTypeIndex < 0 || nTypeIndex >= MAX_CASTINGSPIRIT_TYPE)
	{
		return;
	}

	m_SpiritTypeInfo[nTypeIndex].SetSlotOpenStatus(nSlotIndex, nStatus);
}

int CSpiritPositionInfo::GetCastingItemID(int nTypeIndex, int nSlotIndex)
{
	if (nTypeIndex < 0 || nTypeIndex >= MAX_CASTINGSPIRIT_TYPE)
	{
		return 0;
	}

	return m_SpiritTypeInfo[nTypeIndex].GetCastingItemID(nSlotIndex);
}

void CSpiritPositionInfo::SetCastingItemID(int nTypeIndex, int nSlotIndex, int nItemID)
{
	if (nTypeIndex < 0 || nTypeIndex >= MAX_CASTINGSPIRIT_TYPE)
	{
		return;
	}

	m_SpiritTypeInfo[nTypeIndex].SetCastingItemID(nSlotIndex, nItemID);
}

int CSpiritPositionInfo::GetBindStatus(int nTypeIndex, int nSlotIndex)
{
	if (nTypeIndex < 0 || nTypeIndex >= MAX_CASTINGSPIRIT_TYPE)
	{
		return 0;
	}

	return m_SpiritTypeInfo[nTypeIndex].GetBindStatus(nSlotIndex);
}

void CSpiritPositionInfo::SetBindStatus(int nTypeIndex, int nSlotIndex, int nBindStatus)
{
	if (nTypeIndex < 0 || nTypeIndex >= MAX_CASTINGSPIRIT_TYPE)
	{
		return;
	}

	m_SpiritTypeInfo[nTypeIndex].SetBindStatus(nSlotIndex, nBindStatus);
}

int CSpiritInfo::GetSlotOpenStatus(int nPositionIndex, int nTypeIndex, int nSlotIndex)
{
	if (nPositionIndex < 0 || nPositionIndex >= MAX_CASTINGSPIRIT_POSITION)
	{
		return 0;
	}

	return m_PositionInfo[nPositionIndex].GetSlotOpenStatus(nTypeIndex,nSlotIndex);
}

void CSpiritInfo::SetSlotOpenStatus(int nPositionIndex, int nTypeIndex, int nSlotIndex, int nStatus)
{
	if (nPositionIndex < 0 || nPositionIndex >= MAX_CASTINGSPIRIT_POSITION)
	{
		return;
	}

	m_PositionInfo[nPositionIndex].SetSlotOpenStatus(nTypeIndex, nSlotIndex, nStatus);
}

int CSpiritInfo::GetCastingItemID(int nPositionIndex, int nTypeIndex, int nSlotIndex)
{
	if (nPositionIndex < 0 || nPositionIndex >= MAX_CASTINGSPIRIT_POSITION)
	{
		return 0;
	}

	return m_PositionInfo[nPositionIndex].GetCastingItemID(nTypeIndex, nSlotIndex);
}

void CSpiritInfo::SetCastingItemID(int nPositionIndex, int nTypeIndex, int nSlotIndex, int nItemID)
{
	if (nPositionIndex < 0 || nPositionIndex >= MAX_CASTINGSPIRIT_POSITION)
	{
		return;
	}

	m_PositionInfo[nPositionIndex].SetCastingItemID(nTypeIndex, nSlotIndex, nItemID);
}

void CSpiritInfo::CreateTlvPB(PBSpiritInfo &rPBSpiritInfo)
{
	for (int i = 0; i < MAX_CASTINGSPIRIT_POSITION; i++)
	{
		PBSpiritPositionInfo* pPosition =  rPBSpiritInfo.add_positioninfo();
		if (pPosition == NULL)
		{
			continue;
		}

		for (int j = 0; j < MAX_CASTINGSPIRIT_TYPE; j++)
		{
			PBSpiritTypeInfo* pTypeInfo = pPosition->add_spirittypeinfo();
			if (pTypeInfo == NULL)
			{
				continue;
			}

			for (int k = 0; k < MAX_CASTINGSPIRIT_SLOT; k++)
			{
				int nOpenStatus = GetSlotOpenStatus(i, j, k);
				int nItemID = GetCastingItemID(i, j, k);
				int nBindStatus = GetBindStatus(i, j, k);
				pTypeInfo->add_slotopenstatus(nOpenStatus);
				pTypeInfo->add_castingitemid(nItemID);
				pTypeInfo->add_bindstatus(nBindStatus);
			}
		}
	}
}

void CSpiritInfo::SetFromPB(PBSpiritInfo &rPBSpiritInfo)
{
	memset(m_PositionInfo, 0, sizeof(m_PositionInfo));
	int nPositionSize = (int)rPBSpiritInfo.positioninfo_size();
	for (int i = 0; i < nPositionSize; i++)
	{
		PBSpiritPositionInfo* pPositionInfo = rPBSpiritInfo.mutable_positioninfo(i);
		if (pPositionInfo == NULL)
		{
			continue;
		}

		int nTypeSize = pPositionInfo->spirittypeinfo_size();
		for (int j = 0; j < nTypeSize; j++)
		{
			PBSpiritTypeInfo* pTypeInfo = pPositionInfo->mutable_spirittypeinfo(j);
			if (pTypeInfo == NULL)
			{
				continue;
			}

			int nOpenStatusSize = pTypeInfo->slotopenstatus_size();
			int nItemIDSize = pTypeInfo->castingitemid_size();
			int nBindStatusSize = pTypeInfo->bindstatus_size();
			for (int k = 0; k < nOpenStatusSize; k++)
			{
				int nOpenStatus = pTypeInfo->slotopenstatus(k);
				m_PositionInfo[i].SetSlotOpenStatus(j, k, nOpenStatus);
			}
			for (int m = 0; m < nItemIDSize; m++)
			{
				int nItemID = pTypeInfo->castingitemid(m);
				m_PositionInfo[i].SetCastingItemID(j, m, nItemID);
			}
			for (int n = 0; n < nBindStatusSize; n++)
			{
				int nBindStatus = pTypeInfo->bindstatus(n);
				m_PositionInfo[i].SetBindStatus(j, n, nBindStatus);
			}
		}
	}
}

int CSpiritInfo::GetEffectValue(int nSpiritType)
{
	int nValue = 0;
	for (int i = 0; i < MAX_CASTINGSPIRIT_POSITION; i++)
	{
		for (int j = 0; j < MAX_CASTINGSPIRIT_SLOT; j++)
		{
			int nItemID = GetCastingItemID(i, nSpiritType, j);
			CTplItem* pTplItem = (CTplItem*)CDataStatic::SearchTpl(nItemID);
			if (pTplItem == NULL)
			{
				continue;
			}
			if (pTplItem->mItemType != CTplItem::ITEM_SPIRIT_CASTING)
			{
				continue;
			}

			CTplSpiritItem* pSpiritItem = (CTplSpiritItem*)pTplItem;
			if (pSpiritItem == NULL)
			{
				continue;
			}
			if (pSpiritItem->m_nSpiritType != nSpiritType)
			{
				continue;
			}
			nValue += pSpiritItem->m_nEffectPoint;
		}
	}

	return nValue;
}

int CSpiritInfo::GetBindStatus(int nPositionIndex, int nTypeIndex, int nSlotIndex)
{
	if (nPositionIndex < 0 || nPositionIndex >= MAX_CASTINGSPIRIT_POSITION)
	{
		return 0;
	}

	return m_PositionInfo[nPositionIndex].GetBindStatus(nTypeIndex, nSlotIndex);
}

void CSpiritInfo::SetBindStatus(int nPositionIndex, int nTypeIndex, int nSlotIndex, int nBindStatus)
{
	if (nPositionIndex < 0 || nPositionIndex >= MAX_CASTINGSPIRIT_POSITION)
	{
		return;
	}

	m_PositionInfo[nPositionIndex].SetBindStatus(nTypeIndex, nSlotIndex, nBindStatus);
}

int CSpiritInfo::GetScore()
{
	int nValue = 0;
	for (int i = 0; i < MAX_CASTINGSPIRIT_POSITION; i++)
	{
		for (int j = 0; j < MAX_CASTINGSPIRIT_TYPE; j++)
		{
			for (int k = 0; k < MAX_CASTINGSPIRIT_SLOT; k++)
			{
				int nItemID = GetCastingItemID(i, j, k);
				CTplItem* pTplItem = (CTplItem*)CDataStatic::SearchTpl(nItemID);
				if (pTplItem == NULL)
				{
					continue;
				}
				if (pTplItem->mItemType != CTplItem::ITEM_SPIRIT_CASTING)
				{
					continue;
				}

				CTplSpiritItem* pSpiritItem = (CTplSpiritItem*)pTplItem;
				if (pSpiritItem == NULL)
				{
					continue;
				}
				nValue += pSpiritItem->m_nGrade;
			}
		}
	}

	return nValue;
}

int CSpiritInfo::GetSlotInfo(int& rnOpenedSlotCount, int& rnCastedSlotCount)
{
	int nOpen = 0;
	int nCasted = 0;
	for (int i = 0; i < MAX_CASTINGSPIRIT_POSITION; i++)
	{
		for (int j = 0; j < MAX_CASTINGSPIRIT_TYPE; j++)
		{
			for (int k = 0; k < MAX_CASTINGSPIRIT_SLOT; k++)
			{
				int nOpenStatus = GetSlotOpenStatus(i, j, k);
				int nItemID = GetCastingItemID(i, j, k);
				if (nOpenStatus != 0)
				{
					nOpen++;
				}
				if (nItemID != 0)
				{
					nCasted++;
				}
			}
		}
	}

	rnOpenedSlotCount = nOpen;
	rnCastedSlotCount = nCasted;
}