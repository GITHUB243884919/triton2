#include "stdafx.hpp"
#include "npcmodule.h"
#include "protype.h"
#include "proclient_svr.h"
#include "sceneobj_manager.h"
#include "chatmessage_pb.hxx.pb.h"
#include "chattype.h"
#include "epollsocket.h"
#include "property.h"
#include "mapmodule.h"
#include "propertymodule.h"
#include "sceneobj_manager_imp.h"
#include "scenecfg_manager.h"
#include "entity_searcher_imp.h"
#include "taskmodule.h"
#include "entity.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

char CNpcModule::sModuleName[] = "NpcModule_1";

CNpcTactics CNpcModule::mTactics[ TACTICSNUM ]/* = { {0} }*/;
template<> CNpcModule* CSingleton< CNpcModule >::spSingleton = NULL;

// ********************************************************************** //
// CNpcModule
// ********************************************************************** //

CNpcModule::CNpcModule( ) : CLogicModule( sModuleName )
{

	if ( mShmPtr->GetInitMode() == SHM_INIT )
	{
		mLastTickTimer = LINEKONG::GetTickCount();
		mTimer = 300;
		mNpcPolicys.initailize( );
	} 
	else
	{
		;
	}
}

unsigned int CNpcModule::CountSize()
{

	return sizeof( CNpcModule );
}



void CNpcModule::OnLaunchServer( )
{
	// 创建日志
	INIT_ROLLINGFILE_LOG( "npc", "../log/npc.log", LEVEL_INFO, 10000000, 10 );
	
	OnLoadConfig( );


}

void CNpcModule::OnLoadConfig( )
{

	LOG_INFO( "default", " %s:%s", __LK_FILE__, __FUNCTION__ );

	char tPolicyName[100] = {0};
	::sprintf( tPolicyName, CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(), const_cast<char*>("policy/Strategy.xml") ));
	CNpcModule::GetSingleton().LoadStrategyConfigCallback(tPolicyName);
	// 注册回调函数
	CSceneServer::GetSingleton().RegisterLoadConfigCallback(tPolicyName, CNpcModule::LoadStrategyConfigCallback);
}


void CNpcModule::OnExitServer( )
{
	
}

void CNpcModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{
}

void CNpcModule::OnRouterMessage( CMessage* pMessage )
{
}

int CNpcModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char *pTplName )
{
	return SUCCESS;
}


void CNpcModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	if ( pEntity->IsPlayer() == false )
	{
		CEntityNpc* tpNpc = static_cast<CEntityNpc*>(pEntity);

		// 只有怪物和功能NPC才放入策略逻辑处理
		if ( tpNpc->IsOgre() || tpNpc->IsFunNpc() || tpNpc->IsPet() )
		{
			CTemplateOgre* pTempOgre = (CTemplateOgre*)CDataStatic::SearchTpl( tpNpc->GetNpcID( ) );
			LK_ASSERT( pTempOgre != NULL, return );

			// 放入NPC逻辑处理模块
			CNpcPolicy* tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->CreateObject ( OBJTYPE_POLICY_NPC );
			LK_ASSERT( tpPolicy != NULL, return );
		
			tpPolicy->mNpcID = tpNpc->GetNpcID( );
			tpPolicy->mNpcType = tpNpc->GetEntityType();
			tpPolicy->mEntityID = tpNpc->get_id( );
			tpPolicy->mpEntity = ( CEntityNpc* ) CSceneObjManager::GetSingletonPtr( )->GetObject( tpPolicy->mEntityID );
			tpPolicy->mTargetPos = tpNpc->GetPos( );
			tpPolicy->mPathPos = tpNpc->GetPos( );			

			if ( tpNpc->IsPet() )
			{
				CTemplateSummonBeast *pPet = (CTemplateSummonBeast*)CDataStatic::SearchTpl( tpNpc->GetNpcID( ) );
				LK_ASSERT( pPet != NULL, return );
				tpPolicy->mRunSpeed = pPet->mPetData[ 0 ].mSpeed;
				tpPolicy->mStepTimer = pPet->mPetData[ 0 ].mSpeed;
			}
			else
			{
				tpPolicy->mRunSpeed  = pTempOgre->mRunSpeed;
				tpPolicy->mStepTimer = pTempOgre->mRunSpeed;
			}
			
			tpNpc->SetPolicyID( tpPolicy->get_id( ) );

			// 如果布置了巡逻路线
			if ( tpNpc->IsOgre() )
			{
				COgreCreator* pOgreCreator = ( COgreCreator*)CSceneObjManager::GetSingletonPtr( )->GetObject( tpNpc->GetCreatorID() );
				if ( pOgreCreator != NULL )
				{
					if ( pOgreCreator->mPCount > 0 )
					{
						SetPatrol( tpNpc, pOgreCreator->mPathListX, pOgreCreator->mPathListY, pOgreCreator->mPCount, 0 );
					}
				}

			}
			else if ( tpNpc->IsFunNpc() )
			{
				CNpcCreator* pNpcCreator = ( CNpcCreator* )CSceneObjManager::GetSingletonPtr( )->GetObject( tpNpc->GetCreatorID() );
				if ( pNpcCreator != NULL ) // 护送NPC这个是空 
				{
					if ( pNpcCreator->mPCount > 0 )
					{
						SetPatrol( tpNpc, pNpcCreator->mPathListX, pNpcCreator->mPathListY, pNpcCreator->mPCount, 0 );
					}
				}
			}			
		}		
	}
}

void CNpcModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{
	if ( pEntity->IsOgre() || pEntity->IsFunNpc() || pEntity->IsPet() )
	{
		CEntityNpc* tpNpc = (CEntityNpc*) pEntity;
#ifdef _DEBUG_
		CCreator* tpCreator = (CCreator*)CSceneObjManager::GetSingletonPtr()->GetObject( tpNpc->GetCreatorID() );
		int nCreatorIdx = tpCreator != NULL ? tpCreator->GetIndex() : 0 ;
		LOG_INFO( "default", "[ %s : %d ][ %s ]Destroy Entity( %d : %d ) Index( %d )  Policy( %d )", __LK_FILE__, __LINE__, 
			__FUNCTION__, pEntity->GetEntityID( ), pEntity->GetEntityType( ), nCreatorIdx, tpNpc->GetPolicyID( ) );
#endif
		CSceneObjManager::GetSingletonPtr( )->DestroyObject( tpNpc->GetPolicyID( ) );
		tpNpc->SetPolicyID( 0 );
	}
}

void CNpcModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	if ( mTimer.Elapse( vTickOffset ) == true )
	{
		mLastTickTimer = vTickCount;

		CSceneObjManagerImp* pSceneneObjManagerImp = CSceneObjManager::GetSingletonPtr()->mSceneObjManagerImp;
		CSceneObjManagerImp::ObjMng_NpcPolicy::iterator tPolicyIt;
		CNpcPolicy* tpPolicy = NULL;

		for( tPolicyIt = pSceneneObjManagerImp->mNpcPolicy.begin( ); tPolicyIt != pSceneneObjManagerImp->mNpcPolicy.end( ); ++tPolicyIt )
		{
			tpPolicy = &( *tPolicyIt );
			if ( tpPolicy->mpEntity == NULL )
			{
				LOG_ERROR( "default", "[ %s : %d ][ %s ]Policy has no entity", __LK_FILE__, __LINE__, __FUNCTION__ );
				continue;
			}
			if ( tpPolicy->mNpcType == ENTITYTYPE_OGRE || tpPolicy->mNpcType == ENTITYTYPE_FUNCNPC )
			{
				if ( tpPolicy->mpEntity->GetSendNpcState() == 1 )
				{
					tpPolicy->OnTask( vTickCount );
				}
				else
				{
					if( tpPolicy->mNpcState == CNpcPolicy::STATE_PEACE )
					{
//#ifndef _FB_
						if( tpPolicy->IsSleeping() == false )
//#endif
							tpPolicy->OnPeace( vTickCount ); 
					}
					else
					{
						//if( tpPolicy->IsSleeping() == false )
						tpPolicy->OnBattle( vTickCount );
					}
				}
			}
			else if ( tpPolicy->mNpcType == ENTITYTYPE_PET )
			{
				tpPolicy->OnPet( vTickCount );
			}
		}

		mTimer = CWTimer( 200 );
	}
}



// NPC受到攻击，增加仇恨
void CNpcModule::OnAttacked( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, unsigned int vEnmityValue )
{
	LK_ASSERT( pSrcEntity != NULL && pDesEntity != NULL, return );
	if ( pSrcEntity == pDesEntity )
		return;

	CNpcPolicy* tpPolicy = NULL;	
	if ( pDesEntity->IsOgre() || pDesEntity->IsFunNpc() ) // NPC也会被攻击
	{
		CEntityNpc *pNpc = (CEntityNpc*)pDesEntity;
		
		//被攻击时，强制唤醒怪物
		OnSleep( pNpc, false );

		tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->GetObject( pNpc->GetPolicyID( ) );
		LK_ASSERT( tpPolicy != NULL, return );
		// 如果怪物正在返回，无反应
		if( tpPolicy->mIsReturn == true && tpPolicy->mNpcState == CNpcPolicy::STATE_BATTLE )
			return;
		if ( tpPolicy->mNpcState == CNpcPolicy::STATE_PEACE )
		{
			tpPolicy->mNpcState = CNpcPolicy::STATE_BATTLE;
			tpPolicy->mTargetEntityID = pSrcEntity->GetEntityID();
			tpPolicy->mAttackTimer = 300;
			tpPolicy->mStandTimer = 0;
			tpPolicy->mBattleStart = true;
			tpPolicy->mIsReturn = false;
			tpPolicy->MoveTo( pSrcEntity->mPos );
		}

		tpPolicy->InsertEnmity( pSrcEntity->GetEntityID(), vEnmityValue );
		if ( tpPolicy->mSneerID == 0 )
		{
			if ( pSrcEntity->GetEntityID() != (unsigned int) tpPolicy->mTargetEntityID )
			{
				tpPolicy->mTargetEntityID = tpPolicy->GetEnmityEntityID( );	
			}			
		}
		else
		{
			tpPolicy->mTargetEntityID = tpPolicy->mSneerID;
		}

	}
	else if ( pDesEntity->IsPet() )
	{
		CEntityNpc *pNpc = (CEntityNpc*)pDesEntity;
		tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->GetObject( pNpc->GetPolicyID( ) );
		LK_ASSERT( tpPolicy != NULL, return );
		if ( pSrcEntity->GetEntityID() == 0 )
			tpPolicy->mNpcState = CNpcPolicy::STATE_PEACE;
		else
			tpPolicy->mNpcState = CNpcPolicy::STATE_BATTLE;
		tpPolicy->mTargetEntityID = pSrcEntity->GetEntityID();
		tpPolicy->mStandTimer = 0;
	}

	return;
}

void CNpcModule::OnSneered( unsigned int vSrcEntityID, unsigned int vDesEntityID, unsigned int vSneerValue )
{
	CEntity *pDesEntity = ( CEntity* ) CSceneObjManager::GetSingletonPtr( )->GetObject( vDesEntityID );
	if ( pDesEntity == NULL )
		return;
	if ( !pDesEntity->IsOgre() && !pDesEntity->IsPet()  && !pDesEntity->IsFunNpc() )
		return;

	CEntityNpc *pNpc = (CEntityNpc*)pDesEntity;
	CNpcPolicy* tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->GetObject( pNpc->GetPolicyID( ) );
	LK_ASSERT( tpPolicy != NULL, return );

	if ( tpPolicy->mNpcType == ENTITYTYPE_OGRE || tpPolicy->mNpcType == ENTITYTYPE_FUNCNPC)
	{
		if( vSneerValue > 0 )
		{
					
			tpPolicy->mTargetEntityID = tpPolicy->GetEnmityEntityID( );
			tpPolicy->mSneerID = 0;

		}
		else
		{
			// 转换攻击目标
			tpPolicy->mTargetEntityID = vSrcEntityID;
			tpPolicy->mSneerID = vSrcEntityID;
			tpPolicy->InsertEnmity( vSrcEntityID, 1 );
		}			
	}
	return;
}

void CNpcModule::OnDead( unsigned int vDesEntityID, unsigned short vDead )
{
	CEntity *pDesEntity = ( CEntity* ) CSceneObjManager::GetSingletonPtr( )->GetObject( vDesEntityID );
	if ( pDesEntity == NULL )
		return;
	if ( !pDesEntity->IsOgre() && !pDesEntity->IsPet() )
		return;
	CEntityNpc *pNpc = (CEntityNpc*)pDesEntity;
	CNpcPolicy* tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->GetObject( pNpc->GetPolicyID( ) );
	LK_ASSERT( tpPolicy != NULL, return );
	// 玩家死亡
	if ( vDead == 0 )
	{
		tpPolicy->mKill = true;
	}
	if ( vDead == 1 )
	{
		if ( rand( ) % 4 != 0 )
			tpPolicy->mSleep = true;
		tpPolicy->mIsDead = true;
	}

}

void CNpcModule::OnMoveNpc( unsigned int vEntityID, unsigned int vPosX, unsigned int vPosY )
{
	CEntity *pDesEntity = ( CEntity* ) CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	if ( pDesEntity == NULL )
		return;

	CEntityNpc *pNpc = (CEntityNpc*)pDesEntity;
	CNpcPolicy* tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->GetObject( pNpc->GetPolicyID( ) );
	LK_ASSERT( tpPolicy != NULL, return );

	tpPolicy->mPreList.clear();
	tpPolicy->mPathList.clear();
	tpPolicy->mPathPos = pNpc->GetPos();

	tpPolicy->MoveTo( CWTPoint(vPosX, vPosY) );
}


//

//************************************
// Method:    OnSleep
// FullName:  CNpcModule::OnSleep
// Access:    public 
// Returns:   void
// Qualifier:  设置NPC是否休眠
//	休眠有2个开关，一级开关nAutoSleep控制npc是否自动休眠(npc默认状态下是自动休眠的)
//	只有自动休眠开启下二级开关vFlag才起作用,否则一旦关闭自动休眠，怪物会一直处于唤醒状态
// Parameter: CEntityNpc * pEntity
// Parameter: bool vFlag  ( 自动休眠状态下的 true休眠/false唤醒 )
// Parameter: int nAllTimes ( 默认0无操作; 1表示关闭自动休眠 -1表示开启自动休眠)
//************************************
void CNpcModule::OnSleep( CEntityNpc* pEntity, bool vFlag, int nAutoSleep /*= 0 */ )
{
	LK_ASSERT( pEntity != NULL, return );

	CNpcPolicy* tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->GetObject( pEntity->GetPolicyID( ) );
	if( tpPolicy == NULL )
	{
		return ;
	}

	// 如果配了巡逻路线,不能休眠
	if ( tpPolicy->mFollowID != 0 || tpPolicy->mPatrolIndex != -1 )
	{
		tpPolicy->mSleep = false;
		return;
	}

	// 0 不作用
	if( nAutoSleep != 0 )
	{
		tpPolicy->mNoSleep = ( nAutoSleep > 0 ? true : false ) ;
	}
	
	// 非休眠类型NPC
	if( tpPolicy->mNoSleep == true )
	{
		tpPolicy->mLastTick = mLastTickTimer;
		// 强制唤醒
		tpPolicy->mSleep = false;
		return ;
	}
	else
	{
		if( vFlag == false )
		{
			// 如果是唤醒npc,需要重置计时器
			tpPolicy->mLastTick = mLastTickTimer;
		}

		LOG_DEBUG( "npc", "entity %d at(%d,%d) is being %s", pEntity->GetEntityID(), pEntity->GetPosX(), pEntity->GetPosY(), vFlag ? "sleep" : "weakup" );

		tpPolicy->mSleep = vFlag;
	}

}

void CNpcModule::OnTeleport( unsigned int vEntityID, unsigned int vPosX, unsigned int vPosY )
{
	CEntity *pDesEntity = ( CEntity* ) CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	if ( pDesEntity == NULL )
		return;
	if ( !pDesEntity->IsOgre() && !pDesEntity->IsPet() && !pDesEntity->IsTaskNpc() )
		return;
	CEntityNpc *pNpc = (CEntityNpc*)pDesEntity;
	CNpcPolicy* tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->GetObject( pNpc->GetPolicyID( ) );
	LK_ASSERT( tpPolicy != NULL, return );
	tpPolicy->mPathList.clear( );
	tpPolicy->mPreList.clear( );
	tpPolicy->mPathPos = CWTPoint( vPosX, vPosY );
	tpPolicy->mTargetPos = tpPolicy->mPathPos;
	tpPolicy->PathNpc( );
}

void CNpcModule::OnFollow( CEntityNpc* pEntity, unsigned int vFollowID, int vOffsetX, int vOffsetY )
{
	LK_ASSERT( pEntity != NULL, return );

	CNpcPolicy* tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->GetObject( pEntity->GetPolicyID( ) );
	if( tpPolicy == NULL )
	{
		LOG_ERROR( "npc", "[%s:%d] can't find policy obj %d, template id %d", __FUNCTION__, __LINE__,  pEntity->GetPolicyID( ), pEntity->GetNpcID() );
		return ;
	}

	tpPolicy->mFollowID = vFollowID;
	tpPolicy->mOffsetX = vOffsetX;
	tpPolicy->mOffsetY = vOffsetY;
}

CWTPoint CNpcModule::GetTargetPos( unsigned int vEntityID )
{
	CWTPoint tPoint = CWTPoint( );
	tPoint.Clear();
	CEntity *pEntity = ( CEntity* ) CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	if ( pEntity == NULL )
		return tPoint;
	if ( !pEntity->IsOgre() && !pEntity->IsPet() && !pEntity->IsTaskNpc() )
		return tPoint;
	CEntityNpc *pNpc = (CEntityNpc*)pEntity;

	CNpcPolicy* tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->GetObject( pNpc->GetPolicyID( ) );
	if( tpPolicy == NULL )
	{
		LOG_ERROR( "npc", "[%s:%d] can't find policy obj %d, template id %d", __FUNCTION__, __LINE__,  pNpc->GetPolicyID( ), pNpc->GetNpcID() );
		return tPoint;
	}
	if ( tpPolicy->mTargetEntityID == 0 )
	{
		return tPoint;
	}
	CEntity *pTarget = (CEntity*)CSceneObjManager::GetSingletonPtr( )->GetObject( tpPolicy->mTargetEntityID );
	if ( pTarget == NULL )
	{
		return tPoint;
	}
	return pTarget->mPos;
}

// 让任务护送NPC停止
void CNpcModule::StopTaskNpc( unsigned int vEntityID )
{
	CEntity *pDesEntity = ( CEntity* ) CSceneObjManager::GetSingletonPtr( )->GetObject( vEntityID );
	if ( pDesEntity == NULL )
		return;
	if ( pDesEntity->GetEntityType() != ENTITYTYPE_FUNCNPC )
		return;
	CEntityNpc *pNpc = (CEntityNpc*)pDesEntity;
	CNpcPolicy* tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->GetObject( pNpc->GetPolicyID( ) );
	LK_ASSERT( tpPolicy != NULL, return );
	tpPolicy->mPreList.clear();
	tpPolicy->mPathList.clear();
	tpPolicy->mPathPos = pNpc->GetPos();
	tpPolicy->mTargetPos = pNpc->GetPos();
	tpPolicy->mStepTimer = tpPolicy->mRunSpeed;
	tpPolicy->mNpcState  = CNpcPolicy::STATE_PEACE;
	tpPolicy->mLastTick  = 0;
}

void CNpcModule::SetPatrol( CEntityNpc* pEntity, int *vPosX, int *vPosY, int vLen, int vStart )
{
	LK_ASSERT( pEntity != NULL, return );

	CNpcPolicy* tpPolicy = ( CNpcPolicy* ) CSceneObjManager::GetSingletonPtr( )->GetObject( pEntity->GetPolicyID( ) );
	if( tpPolicy == NULL )
	{
		LOG_ERROR( "npc", "[%s:%d] can't find policy obj %d, template id %d", __FUNCTION__, __LINE__,  pEntity->GetPolicyID( ), pEntity->GetNpcID() );
		return ;
	}

	LK_ASSERT( vLen <= (int)ARRAY_CNT(tpPolicy->mPatrolList), return );
	LK_ASSERT( vStart < vLen, return );

	// 先清空
	memset( tpPolicy->mPatrolList, -1, sizeof(tpPolicy->mPatrolList) );
	for ( int i = 0; i < vLen; i++ )
	{
		tpPolicy->mPatrolList[ i ].mX = vPosX[ i ];
		tpPolicy->mPatrolList[ i ].mY = vPosY[ i ];
	}
	// 设置起始位置
	tpPolicy->mPatrolIndex = vStart;

	// 
	tpPolicy->mSleep = false;
}

void CNpcModule::LoadStrategyConfigCallback(const char *pcConfigPath)
{
	// 先初始化策略对象
	for( unsigned int i = 0; i < ARRAY_CNT(CNpcModule::GetSingleton().mTactics); i++ )
	{
		CNpcModule::GetSingleton().mTactics[i].Initialize();
	}

	if (pcConfigPath == NULL)
	{
		return;
	}
	// 读策略
	TiXmlDocument tTplDoc;
	tTplDoc.LoadFile( pcConfigPath );
	TiXmlElement* tpRoot = tTplDoc.FirstChildElement( );
	TiXmlElement* tpElement = tpRoot->FirstChildElement( );
	for( ; tpElement != NULL; tpElement = tpElement->NextSiblingElement( ) )
	{
		std::string tEleName = tpElement->Value( );
		if ( tEleName != "Tactics" )
		{
			LOG_ERROR( "npc", "NPC template errors1! --%s", tEleName.c_str() );
			printf( "read npc policy failed 1\n");
			continue;
		}
		const char* tpTacticsName	= tpElement->Attribute( "Name" );
		const char* tpTacticsID		= tpElement->Attribute( "ID" );
		if ( tpTacticsName == NULL || tpTacticsID == NULL )
		{
			LOG_ERROR( "npc", "NPC template error2!" );
			printf( "read npc policy failed 2\n");
			continue;
		}

		int nMaxTacticsID = ARRAY_CNT(CNpcModule::GetSingleton().mTactics);
		int tTacticsID = atoi( tpTacticsID );
		if (tTacticsID >= nMaxTacticsID)
		{
			LOG_ERROR( "npc", "NPC TacticsID :%d too large !", tTacticsID );
			continue;
		}

		strncpy( CNpcModule::GetSingleton().mTactics[ tTacticsID ].mTacticsName, tpTacticsName, strlen( tpTacticsName ) );
		TiXmlElement* tpTriggerEle = tpElement->FirstChildElement( );
		for( ; tpTriggerEle != NULL; tpTriggerEle = tpTriggerEle->NextSiblingElement( ) )
		{
			std::string tTriggerName = tpTriggerEle->Value( );
			if ( tTriggerName != "Trigger" )
			{
				LOG_ERROR( "npc", "NPC template error3!" );
				printf( "read npc policy failed 3\n");
				continue;
			}
			CNpcTrigger tTrigger;
			tTrigger.mActive= atoi( tpTriggerEle->Attribute( "Active" ) );
			tTrigger.mInbattle = atoi( tpTriggerEle->Attribute( "InBattle" ) );
			TiXmlElement* tpSubEle = tpTriggerEle->FirstChildElement( );
			for( ; tpSubEle != NULL; tpSubEle = tpSubEle->NextSiblingElement( ) )
			{
				std::string tSubName = tpSubEle->Value( );
				if ( tSubName == "Condition" )
				{
					CNpcCondition tCondition;
					tCondition.mTypeID = atoi( tpSubEle->Attribute( "ID" ) );
					tCondition.mParam1 = atoi( tpSubEle->Attribute( "Param1" ) );
					tCondition.mParam2 = atoi( tpSubEle->Attribute( "Param2" ) );
					tCondition.mParam3 = atoi( tpSubEle->Attribute( "Param3" ) );
					tCondition.mParam4 = atoi( tpSubEle->Attribute( "Param4" ) );
					tTrigger.mCondition.push_back( tCondition );
				}
				if ( tSubName == "Action" )
				{
					CNpcAction tAction;
					tAction.mTypeID = atoi( tpSubEle->Attribute( "ID" ) );
					if ( tAction.mTypeID == SPEAK )
					{
						int nLen = strlen( tpSubEle->Attribute( "Param1" ) );
						if (nLen >= ARRAY_CNT(tAction.mSpeak))
						{
							LOG_ERROR( "npc", "NPC TacticsID :%d speak txt too long !", tTacticsID );
						}
						else
						{
							strncpy( tAction.mSpeak, tpSubEle->Attribute( "Param1" ), strlen( tpSubEle->Attribute( "Param1" ) ) );
							tAction.mSpeak[ strlen( tpSubEle->Attribute( "Param1" ) ) ] = '\0';
						}
						
						tAction.mParam1 = 0;
					}
					else
						tAction.mParam1 = atoi( tpSubEle->Attribute( "Param1" ) );
					tAction.mParam2 = atoi( tpSubEle->Attribute( "Param2" ) );
					tAction.mParam3 = atoi( tpSubEle->Attribute( "Param3" ) );
					tAction.mTarget = atoi( tpSubEle->Attribute( "Target" ) );
					tTrigger.mAction.push_back( tAction );
				}
			}// end trigger
			CNpcModule::GetSingleton().mTactics[ tTacticsID ].mTrigger.push_back( tTrigger );
		}
	}
}
