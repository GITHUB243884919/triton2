#include "stdafx.hpp"
#include "entity.h"
#include "template.h"
#include "sceneobj_define.h"
#include "sceneobj_manager_imp.h"
#include "sceneobj_manager.h"
#include "mapmodule.h"
#include "mapobjmanager.h"
#include "timer_define.h"
#include "teammodule.h"
#include "taskmodule.h"
#include "trademodule.h"
#include "creature_refresher.h"
#include "message_maker.h"
#include "logservertype.h"
#include "message_maker.h"
#include "message_maker.h"
#include "entity_searcher_imp.h"  
#include "base.h"
#include "message_factory.h"
#include "activitymodule.h"
#include "gmmodule.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

//////////////////////////////////////////////////////////////////////////
//		CEntity
//////////////////////////////////////////////////////////////////////////


CEntity* CEntity::GetOwner()
{
	return (CEntity*)CSceneObjManager::GetSingletonPtr()->GetObject(GetOwnerID());
}

bool CEntity::AddFunResult( unsigned int unSrcEntityID, unsigned short usFunc, unsigned short usResult, bool bBroadCast, unsigned int unValue1, unsigned int unValue2, unsigned int unValue3 /*= 0*/, unsigned int unValue4 /*= 0*/, unsigned int unValue5 /*= 0*/ )
{
	if ( bBroadCast == false )
	{
		return CPropertyModule::GetSingletonPtr( )->GetSingleList( )->push_back( unSrcEntityID, this, usFunc, usResult, bBroadCast, unValue1, unValue2, unValue3, unValue4, unValue5 );
	}
	else
	{
		return CPropertyModule::GetSingletonPtr( )->GetResultList( )->push_back( unSrcEntityID, this, usFunc, usResult, bBroadCast, unValue1, unValue2, unValue3, unValue4, unValue5 );
	}	
}

// 初始化功能列表
void CEntity::InitFunResult( int nListID )
{
//	CPropertyModule::GetSingleton( ).GetResultList( )->Initialize( );
	if ( nListID != 0 )
		CPropertyModule::GetSingleton( ).GetResultList( )->SetListID( nListID );
}


// ***************************************************************
//  Function:		将目的实体加入到可见表
//  Description:	AddViewEntity
//	Input:			pTarget - 目标
//	OutPut:			无
//	Return:			true - 添加成功； false - 没有添加
//	Others:
//  Date:			11/26/2008
// 
// ***************************************************************
bool CEntity::AddViewEntity(CEntity* pTarget)
{
	LK_ASSERT( pTarget != NULL, return false );

	if( pTarget == this )
	{
		return false;
	}

	if(  pTarget->IsPlayer() )
	{
		mSeenPlayerContainer.Add( pTarget->GetEntityID() );

#ifdef _DEBUG_
		LOG_DEBUG( "map", "Add %d to Entity %d ", pTarget->GetEntityID(), this->GetEntityID());

		mSeenPlayerContainer.Show( "map" );
#endif

		return true;
	}

	return false;
}

// ***************************************************************
//  Function:		将目标实体从可见表删除
//  Description:	DeleteViewEntity
//	Input:			pTarget - 目标
//	OutPut:			无
//	Return:			true - 删除成功； false - 没有删除
//	Others:
//  Date:			11/26/2008
// 
// ***************************************************************
bool CEntity::DeleteViewEntity(CEntity *pTarget)
{
	LK_ASSERT( pTarget != NULL, return false );

	if( pTarget == this )
	{
		return false;
	}

	if( pTarget->IsPlayer() ) // 怪物也会在任务NPC的可见表里
	{
		mSeenPlayerContainer.Remove( pTarget->GetEntityID() );

#ifdef _DEBUG_
		LOG_DEBUG( "map", "delete %d from Entity %d", pTarget->GetEntityID(), this->GetEntityID() );

		mSeenPlayerContainer.Show( "map" );
#endif
		return true;
	}

	return false;
}

// 得到地图对象
CMapObject* CEntity::GetMapObject( )
{
	return (CMapObject*) CSceneObjManager::GetSingletonPtr()->GetObject( mMapObjectID );
}

// 目标是否在距离范围内
bool CEntity::IsWithinDistance( CEntity* pTarget, unsigned int unDistance )
{
	if( pTarget == NULL )
	{
		return false;
	}

	// 不在一个地图没有比较的意义
	if( GetMapObjectID() != pTarget->GetMapObjectID() )
	{
		return false;
	}

	return GetPos().LimitDistance( pTarget->GetPos(), unDistance );
}


// 是否在可见范围内
bool CEntity::IsWithinViewDistance( CEntity* pTarget )
{
	// 不在一个地图没有比较的意义
	if( GetMapObjectID() != pTarget->GetMapObjectID() )
	{
		return false;
	}

	if ( ::abs( pTarget->GetPosX() - this->GetPosX() ) <= VIEW_RADIUS_X &&
		::abs( pTarget->GetPosY() - this->GetPosY() <= VIEW_RADIUS_Y ) )
	{
		return true;
	}
	else
	{
		return false;
	}

}

 
//////////////////////////////////////////////////////////////////////////
///		CEntityPlayer	
//////////////////////////////////////////////////////////////////////////



CEntityPlayer::CEntityPlayer( ) : CEntityCharacter( ENTITYTYPE_PLAYER )
{

      if ( CObj::msCreateMode )
      {
          Initialize( );
      } 
      else
      {
          Resume( );
      }
 
}

CEntityPlayer::~CEntityPlayer()
{
	
}



// ***************************************************************
//  Function:		IsHostileTo 
//  Description:	是否敌对
//	Input:			pEntity - 对方实体
//	OutPut:			无
//	Return:			无
//	Others:			目前的判断相当的宽松，因为社会系统尚未加入
//  Date:			11/13/2008
// 
// ***************************************************************
bool CEntityPlayer::IsHostileTo( CEntity* pEntity )
{
	LK_ASSERT( pEntity != NULL, return false );
	
	// 如果是自己
	if( this == pEntity )
	{
		return false;
	}

    if( pEntity->IsCharacter( ) == true )
    {
        int tSrcCampID = GetCampID( );
        int tDesCampID = ( ( CEntityCharacter* )pEntity )->GetCampID( );

        if( tSrcCampID != 0 && tDesCampID != 0 )
        {
            return tSrcCampID != tDesCampID;
        }
    }

	switch ( pEntity->GetEntityType() )
	{
	case ENTITYTYPE_PLAYER:
		{
			CEntityPlayer* tpPlayer = ( CEntityPlayer* )pEntity;
			// TODO: 队友不可攻击
			if ( IsTeamTo( tpPlayer ) == true )
			{
				return false;
			}

			// TODO: 同一家族不可攻击
			if ( IsFamilyTo( tpPlayer ) )
			{
				return false;
			}
			
			// GM不能被攻击
			if( CGMModule::GetSingleton().IsGm( tpPlayer->GetGmStatus() ) == true  )
			{
				return false;
			}
			
			// TODO: 判断玩家保护规则
			if ( IsBeginnerProtect() == true || tpPlayer->IsBeginnerProtect() == true || tpPlayer->IsTransProtect() == true )
			{
				return false;
			}

			CPropertyPlayer* tpSrcPro = ( CPropertyPlayer* )this->GetProperty( );
			CPropertyPlayer* tpDesPro = ( CPropertyPlayer* )tpPlayer->GetProperty( );

			if( tpSrcPro->GetStateParam( STATE_UNATTACK_PLAYER ).mInState == true )
			{
				return false;
			}
			if( tpDesPro->GetStateParam( STATE_PLAYER_UNATTACK ).mInState == true )
			{
				return false;
			}


			switch ( GetPKMode() )
			{
			case PKMODE_TRAINING:// 练级模式
				{
					// TODO: 同国白名玩家不可攻击
					/*if ( GetNationality() == tpPlayer->GetNationality() && tpDesPro->IsWhitePKStatus() )
					{
						return false;
					}
					return true;*/
					if( GetNationality() != tpPlayer->GetNationality() )
					{
						return true;
					}
					return false;
				}
				break;
			case PKMODE_ALL:// 屠杀模式
				{
					return true;
				}
				break;
			case PKMODE_CORPS:// 军团模式
				{
					// TODO: 同一军团中的玩家不可攻击
					if ( IsCorpsTo( tpPlayer ) )
					{
						return false;
					}			
					return true;
				}
				break;
			case PKMODE_ALLY:// 盟国模式
				{
					// TODO: 本国及同盟国玩家都不可攻击, 暂时不支持结盟
					if( GetNationality() != tpPlayer->GetNationality() )
					{
						return true;
					}
					return false;
				}
				break;
			case PKMODE_EVIL:// 屠杀模式
				{
					if ( GetNationality() == tpPlayer->GetNationality() && tpDesPro->IsWhitePKStatus() )
					{
						return false;
					}
					return true;
				}
				break;
			default:
				break;
			}

			break;
		}
	case ENTITYTYPE_BLOCK:
	case ENTITYTYPE_OGRE:
	case ENTITYTYPE_FUNCNPC:
		{
			// TODO: 怪物是否可攻击,由模板决定
			CEntityNpc *pNpc = (CEntityNpc *)pEntity;
			// 如果是同国
			if ( pNpc->GetOwnCampID() != 0 )
			{
				if ( GetNationality() != pNpc->GetOwnCampID() )
				{
					if( ( pNpc->GetPKType( ) & ENEMY_TO_FC ) == ENEMY_TO_FC )
					{
						return true;
					}
				}
				else
				{
					if( ( pNpc->GetPKType( ) & ENEMY_TO_SC ) == ENEMY_TO_SC )
					{
						return true;
					}
					return false;
				}
			}
			if ( GetNationality() == pNpc->GetNationality() ) 
			{
				if( ( pNpc->GetPKType( ) & ENEMY_TO_SC ) == ENEMY_TO_SC )
				{
					return true;
				}
			}
			// 如果是其他国
			else
			{
				if( ( pNpc->GetPKType( ) & ENEMY_TO_FC ) == ENEMY_TO_FC )
				{
					return true;
				}
			}
			break;
		}
	case ENTITYTYPE_PET:
		{
			CEntityPlayer* pOwner = (CEntityPlayer*) ( pEntity->GetOwner() );
			if ( pOwner == NULL )
			{
				return false;
			}
			// TODO: 自己的宠物,无法攻击
			if ( pOwner->GetEntityID() == GetEntityID() )
			{
				return false;
			}
			
			return IsHostileTo( pOwner );
		}
	default:
		break;
	}

	return false;
}


// 是否友好
bool CEntityPlayer::IsFriendlyTo( CEntity* pEntity )
{
	LK_ASSERT( pEntity != NULL, return false );

	// 如果是自己
	if( this == pEntity )
	{
		return true;
	}
	
	if ( pEntity->IsPlayer() || pEntity->IsPet() )
	{
		return ! IsHostileTo( pEntity );
	}
	else if ( pEntity->IsOgre() || pEntity->IsFunNpc() )
	{
		CEntityNpc *pNpc = (CEntityNpc *)pEntity;
		if ( pNpc->GetOwnCampID() != 0 )			
		{
			if ( GetNationality() == pNpc->GetOwnCampID() )
			{
				if( ( pNpc->GetPKType( ) & FRIEND_TO_SC ) == FRIEND_TO_SC || ( pNpc->GetPKType( ) & NEUTRAL_TO_SC ) == NEUTRAL_TO_SC )
				{
					return true;
				}
				return false;
			}
			else
			{
				if( ( pNpc->GetPKType( ) & FRIEND_TO_SC ) == FRIEND_TO_FC || ( pNpc->GetPKType( ) & NEUTRAL_TO_FC ) == NEUTRAL_TO_FC )
				{
					return true;
				}
				return false;
			}
		}
		// 如果是同国
		if ( GetNationality() == pNpc->GetNationality() ) 
		{
			if( ( pNpc->GetPKType( ) & FRIEND_TO_SC ) == FRIEND_TO_SC || ( pNpc->GetPKType( ) & NEUTRAL_TO_SC ) == NEUTRAL_TO_SC )
			{
				return true;
			}
		}
		// 如果是其他国
		else
		{
			if( ( pNpc->GetPKType( ) & FRIEND_TO_SC ) == FRIEND_TO_FC || ( pNpc->GetPKType( ) & NEUTRAL_TO_FC ) == NEUTRAL_TO_FC )
			{
				return true;
			}
		}
	}
	return false;
}

// 是否是队友
bool CEntityPlayer::IsTeamTo( CEntity* pEntity )
{
	LK_ASSERT( pEntity != NULL, return false );

	// 如果是自己
	if ( this == pEntity )
	{
		return true;
	}

	if ( this->GetTeamFlag() != TEAM_NONE
		&& this->GetTeamFlag() == pEntity->GetTeamFlag() )
	{
		return true;
	}
	
	return false;
}

// 是否是同一家族成员
bool CEntityPlayer::IsFamilyTo( CEntity *pEntity )
{
	LK_ASSERT( pEntity != NULL, return false )
	
	// 如果是自己
	if ( this == pEntity )
	{
		return true;
	}
	
	// 如果实体不是玩家
	if ( !pEntity->IsPlayer() )
	{
		return false;
	}
	
	CEntityPlayer *tpPlayer = ( CEntityPlayer * )pEntity;
	
	if ( mFamilyID != 0 && mFamilyID == tpPlayer->GetFamilyID()  )
	{
		return true;
	}
	
	return false;
}

// 是否是同一军团
bool CEntityPlayer::IsCorpsTo( CEntity *pEntity )
{
	LK_ASSERT( pEntity != NULL, return false )

	// 如果是自己
	if ( this == pEntity )
	{
		return true;
	}

	// 如果实体不是玩家
	if ( !pEntity->IsPlayer() )
	{
		return false;
	}

	CEntityPlayer *tpPlayer = ( CEntityPlayer * )pEntity;

	if ( mCorpsID != 0 && mCorpsID == tpPlayer->GetCorpsID() )
	{
		return true;
	}
	return false;
}

// 初始化个人日志，追踪特殊玩家消息用
int CEntityPlayer::InitializeLog( )
{ 
	const char *tpLogDir = "../log/playerlog/";
	
	char tRirName[LOGNAMELENGTH] = { 0 };
	sprintf( tRirName, "%s%d", tpLogDir, mCharProfile.mAccountID );
	CreatDir( tRirName, 0700 );
	
	time_t tTime = time( NULL );
	tm	   *tTm = localtime( &tTime );
	memset( mLogName, 0, sizeof( mLogName ) );
	sprintf(mLogName,"%d_%04d%02d%02d", mCharProfile.mCharID, tTm->tm_year+1900, tTm->tm_mon, tTm->tm_mday );
	char tDir[LOGNAMELENGTH] = { 0 };
	sprintf( tDir,"%s/%s.log", tRirName, mLogName );
	INIT_ROLLINGFILE_LOG( mLogName, tDir, LEVEL_NOTSET, 100*1024*1024 );	
	return 0;
}

// 关闭个人日志
int CEntityPlayer::CloseLog( )
{
	if ( mIfTracedPlayer )
	{
		LOG_SHUTDOWN( mLogName );
	}
	return 0;
}

// 写消息到个人日志
int CEntityPlayer::WriteMsgInfo( CMessage *pMessage, int nMsgType /* = 1 */ )
{  
	LK_ASSERT( pMessage != NULL, return -1 )	
	if ( mIfTracedPlayer == false )
	{
		return 1;
	}
	Message* pUnknownMessagePara = (Message*) pMessage->msgpara();
	LK_ASSERT( pUnknownMessagePara != NULL, return -1 );
	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();
	
	// 收到的消息
	if( nMsgType == 1 )
	{
		LOG_NOTICE( mLogName, "---- Recv Client[%s] Msg[ %s ][id: 0x%04x / %d] ----", GetCharNameShow(), pDescriptor->name().c_str(), pMessage->msghead().messageid(), pMessage->msghead().messageid());
		LOG_NOTICE( mLogName, "[%s]", ((Message*)pMessage->msgpara())->ShortDebugString().c_str() ); 	 		
	}	
	
	// 发送的消息
	else
	{
		LOG_NOTICE( mLogName, "---- Send Client[%s] Msg[ %s ][id: 0x%04x / %d] ----", GetCharNameShow(), pDescriptor->name().c_str(), pMessage->msghead().messageid(), pMessage->msghead().messageid());
		LOG_NOTICE( mLogName, "[%s]", ((Message*)pMessage->msgpara())->ShortDebugString().c_str() ); 
	}
	return 0;
}

// 是否对玩家实体友好
bool CEntityPlayer::IsHostileToPlayers()
{
	return true;
}

// 属主实体
unsigned int CEntityPlayer::GetOwnerID()
{
	// 玩家的主人为他自己
	return GetEntityID();
}

// 检查玩家保护规则
bool CEntityPlayer::IsBeginnerProtect()
{
	if ( /* TODO: 是否在本国领土 */true )
	{
		CPropertyPlayer* tpProperty = (CPropertyPlayer*) GetProperty();

		if ( tpProperty == NULL )
		{
			return true;
		}

		CTemplateBeginner* tpBeginner = (CTemplateBeginner*) CDataStatic::GetTemp( TEMP_BEGINNER );
		if ( tpBeginner == NULL )
		{
			return true;
		}

		// TODO: 暂时写死为固定等级,以后读模板
		if ( (int)tpProperty->GetLevel( ) < tpBeginner->mBeginnerProtectLevel )
		{
			return true;
		}
	}

	return false;
}

// 检查玩家是否受到跨场景保护规则保护
bool CEntityPlayer::IsTransProtect()
{
	// TODO: 如果有跨场景保护状态
	return false;
}

unsigned short CEntityPlayer::GetTeamLevel( )
{
	CEntityPlayer* tpPlayerArr[ TEAM_NUMBER ] = { NULL };
	unsigned short tLevel = 0;
	int tCountReal = 0;
	int tRtn = 0, tCount = 0;
	tRtn = CTeamModule::GetSingletonPtr( )->OnGetMemberList( this, tpPlayerArr, ARRAY_CNT( tpPlayerArr ), tCount );
	if (tRtn < 0)
	{
		return this->GetLevel();
	}
	
	for( unsigned int i = 0; i < ( unsigned int )tCount && i < ARRAY_CNT( tpPlayerArr ); i++ )
	{
		if( tpPlayerArr[ i ] != NULL )
		{
			tLevel += ( ( CPropertyPlayer* )tpPlayerArr[ i ]->GetProperty( ) )->GetLevel( );
			tCountReal++;
		}
	}

	return ( tCountReal != 0 ) ? ( unsigned short )tLevel / tCountReal : tLevel;
}

unsigned short CEntityPlayer::GetTeamMaxLevel( )
{
	CEntityPlayer* tpPlayerArr[ TEAM_NUMBER ] = { NULL };
	//unsigned short tLevel = 0;
	//int tCountReal = 0;
	int tRtn = 0, tCount = 0;

	tRtn = CTeamModule::GetSingletonPtr( )->OnGetMemberList( this, tpPlayerArr, ARRAY_CNT( tpPlayerArr ), tCount );
	if ( tRtn < 0 )
	{
		return this->GetLevel();
	}
	
	int tMinLevel = 0;
	for( unsigned int i = 0; i < ( unsigned int )tCount && i < ARRAY_CNT( tpPlayerArr ); i++ )
	{
		if( tpPlayerArr[ i ] != NULL )
		{
			if( (unsigned int)tMinLevel < ( ( CPropertyPlayer* )tpPlayerArr[ i ]->GetProperty( ) )->GetLevel( ) )
			{
				tMinLevel = ( ( CPropertyPlayer* )tpPlayerArr[ i ]->GetProperty( ) )->GetLevel( );
			}
		}
	}

	return tMinLevel;
}

unsigned short CEntityPlayer::GetTeamMinLevel( )
{
	CEntityPlayer* tpPlayerArr[ TEAM_NUMBER ] = { NULL };
	//unsigned short tLevel = 0;
	//int tCountReal = 0;
	int tRtn = 0, tCount = 0;
	tRtn = CTeamModule::GetSingletonPtr( )->OnGetMemberList( this, tpPlayerArr, ARRAY_CNT( tpPlayerArr ), tCount );
	if (tRtn < 0)
	{
		return this->GetLevel();
	}
	
	int tMaxLevel = 0;
	for( unsigned int i = 0; i < ( unsigned int )tCount && i < ARRAY_CNT( tpPlayerArr ); i++ )
	{
		if( tpPlayerArr[ i ] != NULL )
		{
			if( (unsigned int) tMaxLevel > ( ( CPropertyPlayer* )tpPlayerArr[ i ]->GetProperty( ) )->GetLevel( ) || tMaxLevel == 0 )
			{
				tMaxLevel = ( ( CPropertyPlayer* )tpPlayerArr[ i ]->GetProperty( ) )->GetLevel( );
			}
		}
	}

	return tMaxLevel;
}

void CEntityPlayer::IncAfterFcmKilled(int nCount)
{
	const int nKilledLimit = 50;
	const int nResetSeconds = 3600 * 5;

	int nLastFcmKilled = m_nAfterFcmKilled;

	m_nAfterFcmKilled += nCount;
	if ( m_nAfterFcmKilled > nKilledLimit && nLastFcmKilled <= nKilledLimit && GetFcmStatus() == em_half_rate )
	{
		CMessage tMessage;
		CMessageFCMPlayerResetRequest tPara;

		tPara.set_accountid(GetAccountID());
		tPara.set_seconds(nResetSeconds);

		tMessage.set_msgpara((uint32_t) &tPara);
		tMessage.mutable_msghead()->set_messageid( ID_S2G_FCMPLAYERRESET_REQUEST);
		CSceneLogic::GetSingletonPtr()->Send2Gate( &tMessage );
	}
}



// ***********************************************************
//  Function:		PKIsYellow
//  Description:	PK是否黄名
//  Input:			CEntity* pDesEntity 被攻击实体
//  OutPut:			
//  Return:			bool 是否发送黄名的消息
//  Others:
//  Date:	11/20/2008
// **********************************************************
bool CEntityPlayer::PKIsYellow( CEntity* pDesEntity )
{
	//LK_ASSERT( pDesEntity != NULL, return false );

	//if ( pDesEntity->GetEntityType() == ENTITYTYPE_PLAYER || pDesEntity->GetEntityType() == ENTITYTYPE_PET )
	//{
	//	CEntityPlayer* tpDesPlayer = NULL;
	//	if ( pDesEntity->GetEntityType() == ENTITYTYPE_PLAYER )
	//		tpDesPlayer = (CEntityPlayer*) pDesEntity;
	//	else if ( pDesEntity->GetEntityType() == ENTITYTYPE_PET )
	//		tpDesPlayer = (CEntityPlayer*) pDesEntity->GetOwner();

	//	if ( tpDesPlayer == NULL )
	//	{
	//		return false;
	//	}

	//	// TODO: 白名玩家攻击白名玩家,攻击者进入黄名状态,白名玩家攻击黄名或红名玩家,不做任何处理
	//	// TODO: 黄名玩家攻击白名玩家或黄名玩家,重置黄名时间,黄名玩家攻击红名玩家,不错任何处理
	//	// TODO: 红名玩家攻击任何玩家都不做处理
	//	if ( GetPKStatus() == PKSTATUS_WHITE )
	//	{
	//		if ( tpDesPlayer->GetPKStatus() == PKSTATUS_WHITE )
	//		{
	//			SetPKStatus( PKSTATUS_YELLOW );
	//			// 黄名存在时间为60秒
	//			SetPKYellowTimer( 60000 );
	//			return true;
	//		}
	//	}
	//	else if ( GetPKStatus() == PKSTATUS_YELLOW )
	//	{
	//		if ( tpDesPlayer->GetPKStatus() == PKSTATUS_WHITE || tpDesPlayer->GetPKStatus() == PKSTATUS_YELLOW )
	//		{
	//			SetPKYellowTimer( 60000 );
	//		}
	//	}
	//}
	return false;
}

// 杀人是否红名
bool CEntityPlayer::PKIsRed( CEntity* pDesEntity )
{
	LK_ASSERT( pDesEntity != NULL, return false );

	if ( pDesEntity->GetEntityType() != ENTITYTYPE_PLAYER )
		return false;

//	CEntityPlayer* tpPlayer = (CEntityPlayer*) pDesEntity;

	// TODO: 白名杀死白名,白名变红名而且PK值加1,白名杀死黄名或者红名,无任何变化
	// TODO: 黄名杀死白名,黄名变红名而且PK值加1,黄名杀死黄名或者红名,无任何变化
	// TODO: 红名杀死白名,红名PK值加1,红名杀死黄名或者红名,无任何变化
	/*if ( GetPKStatus() == PKSTATUS_WHITE || GetPKStatus() == PKSTATUS_YELLOW )
	{
		if ( tpPlayer->GetPKStatus() == PKSTATUS_WHITE )
		{
			SetPKStatus( PKSTATUS_RED );
			int tRedTimer = 1000 * 60 * 60 * 2;
			SetPKRedTimer( tRedTimer );
			int tPKValue = GetPKValue() + 100;
			SetPKValue( tPKValue );
			return true;
		}
	}
	if ( GetPKStatus() == PKSTATUS_RED )
	{
		if ( tpPlayer->GetPKStatus() == PKSTATUS_WHITE )
		{
			int tRedTimer = 1000 * 60 * 60 * 2;
			SetPKRedTimer( tRedTimer );
			int tPKValue = GetPKValue() + 100;
			SetPKValue( tPKValue );
		}
	}*/
	return false;
}


// ***************************************************************
//  Function:		IsVisibleFor 
//  Description:	是否对目标玩家可见
//	Input:			pTargetPlayer - 目标玩家
//	OutPut:			无
//	Return:			无
//	Others:			
//  Date:			11/13/2008
// 
// ***************************************************************
bool CEntityPlayer::IsVisibleFor( CEntity* pTarget )
{
	LK_ASSERT( pTarget != NULL, return false );

	if( mVisibility == VISIBLITY_SELF )
	{
		return false;
	}
	else if ( mVisibility == VISIBLITY_TEAM )
	{
		return pTarget->GetTeamFlag() == GetTeamFlag();
	}
	else 
	{
		return true;
	}
}	


// 改变PK模式
int CEntityPlayer::ChangePKMode( unsigned int vPKMode )
{
	// 死亡不能改变模式
	if ( GetAliveStatus( ) == ALIVE_DIE )
		return ERROR_CHANGEPKMODE_NONEPKMODE;

	if ( vPKMode != PKMODE_TRAINING && vPKMode != PKMODE_ALL 
		&& vPKMode != PKMODE_CORPS && vPKMode != PKMODE_ALLY
		&& vPKMode != PKMODE_EVIL )
		return ERROR_CHANGEPKMODE_NONEPKMODE;

	if ( mPKMode == (EPKMode)vPKMode )
		return ERROR_CHANGEPKMODE_SAMEPKMODE;

	CTemplateBeginner* tpBeginner = (CTemplateBeginner*) CDataStatic::GetTemp( TEMP_BEGINNER );
	if ( tpBeginner == NULL )
		return ERROR_CHANGEPKMODE_UNKNOWN;

	CPropertyPlayer* tpProPlayer = (CPropertyPlayer*) GetProperty();
	if ( tpProPlayer == NULL )
		return ERROR_CHANGEPKMODE_UNKNOWN;

	// pk模式等级限制
	if ( (int)tpProPlayer->GetLevel( ) < tpBeginner->mBeginnerProtectLevel )
	{
		if( GetPKMode() == PKMODE_TRAINING )
		{
			return ERROR_CHANGEPKMODE_NEEDMORELEVEL;
		}

		// TODO:(解决临时调pk等级限制问题) 如果当前不是 “练级模式”，还是允许改的
	}

	SetPKMode( (EPKMode)vPKMode );
	return ERROR_CHANGEPKMODE_SUCCESS;
}

// 执行技能消耗
void CEntityPlayer::DoSkillCost( CTemplateSkill* tpSkillTpl, bool bIsMagicWeapon /*= false*/ )
{
	LK_ASSERT( tpSkillTpl != NULL, return );

	CItemMagicWeapon* tpMW = NULL;
	if ( bIsMagicWeapon )
	{
		tpMW = (CItemMagicWeapon*)mProperty.GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
		if ( tpMW == NULL )
			return ;
	}

	// 计算技能消耗
	for ( int i = 0; i < (int)ARRAY_CNT( tpSkillTpl->mCost ); i ++ )
	{
		if ( tpSkillTpl->mCost[ i ].mCostType == CTemplateSkill::COST_MP )
		{
			if ( bIsMagicWeapon )
			{
				tpMW->OnIncMp( (0-tpSkillTpl->mCost[ i ].mCostParam[ 0 ]) );
			}
			else
			{
				CPropertyValue tValue = CPropertyFormula::GetSkillLess( &mProperty, tpSkillTpl->mGenerID );
				int tDecMp = std::max( 0, (int)( ( tpSkillTpl->mCost[ i ].mCostParam[ 0 ] - tValue.mDeltaValue ) 
					* ( SERVER_PERCENT_FLOAT - tValue.mInitValue ) / SERVER_PERCENT_FLOAT ) );
				//mProperty.SetCurMP( mProperty.GetCurMP( ) - tDecMp );
				//
				//AddFunResult( GetEntityID(), FUNC_DECMP, RESULT_COST, tDecMp, mProperty.GetCurMP( ), mProperty.GetMaxMP( )  );
				ExpendMP( tDecMp, GetEntityID(), RESULT_COST );
			}
		}
		else if ( tpSkillTpl->mCost[ i ].mCostType == CTemplateSkill::COST_HP )
		{
			if ( bIsMagicWeapon )
			{
				tpMW->OnIncHp( (0-tpSkillTpl->mCost[ i ].mCostParam[ 0 ]) );
			}
			else
			{
				//mProperty.SetCurHP( mProperty.GetCurHP( ) - tpSkillTpl->mCost[ i ].mCostParam[ 0 ] );

				//AddFunResult( GetEntityID(), FUNC_DECMP, RESULT_COST, tpSkillTpl->mCost[ i ].mCostParam[ 0 ] , mProperty.GetCurHP( ), mProperty.GetMaxHP( )  );
				ExpendHP( tpSkillTpl->mCost[ i ].mCostParam[ 0 ], GetEntityID(), RESULT_COST );
			}
		}
		// 只有玩家才能消耗道具
		else if ( tpSkillTpl->mCost[ i ].mCostType == CTemplateSkill::COST_ITEM )
		{
			bool tBind = false;
			CPropertyModule::GetSingletonPtr()->PlayerRemoveItemByID( this, tpSkillTpl->mCost[ i ].mCostParam[ 0 ], tpSkillTpl->mCost[ i ].mCostParam[ 1 ], tBind );
			LogEventLostItemByUse(this, 0, tpSkillTpl->mCost[ i ].mCostParam[ 0 ], tpSkillTpl->mCost[ i ].mCostParam[ 1 ]);
		}
		// 只有玩家才能消耗活力
		else if ( tpSkillTpl->mCost[ i ].mCostType == CTemplateSkill::COST_AP )
		{
			CPropertyPlayer* tpProperty = (CPropertyPlayer*) GetProperty();
			if ( tpProperty->IsVip() == false )
			{
				int tAp = tpProperty->GetCurAP();
				tpProperty->SetCurAP( std::max( 0, tAp - tpSkillTpl->mCost[ i ].mCostParam[ 0 ] ) );
				AddFunResult( this->GetEntityID(), FUNC_DECAP, RESULT_NORMAL, true, tpSkillTpl->mCost[ i ].mCostParam[ 0 ], tpProperty->GetCurAP(), tpProperty->GetMaxAP() );
			}
		}
	}
}

// 判断技能消耗是否满足
bool CEntityPlayer::CheckSkillCost( CTemplateSkill* tpSkillTpl, bool bIsMagicWeapon /*= false*/ )
{
	LK_ASSERT( tpSkillTpl != NULL, return false );
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)mProperty.GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
	if ( bIsMagicWeapon && tpMW == NULL )
		return false;
	
	// 判断技能消耗是否满足
	for ( int i = 0; i < (int)ARRAY_CNT( tpSkillTpl->mCost ); i ++ )
	{
		if ( tpSkillTpl->mCost[ i ].mCostType == (int)CTemplateSkill::COST_HP )
		{
			if ( bIsMagicWeapon )
			{
				if ((int)tpMW->GetHp() < tpSkillTpl->mCost[ i ].mCostParam[ 0 ])
					return false;
			}
			else
			{
				if ((int)mProperty.GetCurHP( ) < tpSkillTpl->mCost[ i ].mCostParam[ 0 ])
					return false;
			}
		}
		else if( tpSkillTpl->mCost[ i ].mCostType == (int)CTemplateSkill::COST_MP )
		{
			if ( bIsMagicWeapon )
			{
				if ((int)tpMW->GetMp() < tpSkillTpl->mCost[ i ].mCostParam[ 0 ])
					return false;
			}
			else
			{
				if ((int)mProperty.GetCurMP( ) < tpSkillTpl->mCost[ i ].mCostParam[ 0 ] )
					return false;
			}
		}

		// 只有玩家才能消耗道具
		else if ( tpSkillTpl->mCost[ i ].mCostType == (int)CTemplateSkill::COST_ITEM )
		{
			if ( mProperty.GetBaggage( )->HasItem( tpSkillTpl->mCost[ i ].mCostParam[ 0 ], tpSkillTpl->mCost[ i ].mCostParam[ 1 ], BIND_ALL ) == false )
			{
				return false;
			}
		}

		// 只有玩家才能消耗活力
		if ( tpSkillTpl->mCost[ i ].mCostType == (int)CTemplateSkill::COST_AP )
		{
			if ( mProperty.IsVip() == false )
			{
				if ((int)mProperty.GetCurAP( ) < tpSkillTpl->mCost[ i ].mCostParam[ 0 ])
					return false;
			}
		}
	}

	return true;
}


// 检查技能前置条件
int CEntityPlayer::CheckSpecialUseCondition( CTemplateSkill* tpSkillTpl, bool bIsMagicWeapon /*= false*/ )
{
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)mProperty.GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
	if ( bIsMagicWeapon && tpMW == NULL )
		return ERROR_MW_NOTEQUIP;

	// 特殊技能使用条件判断
	for ( int i = 0; i < (int)ARRAY_CNT( tpSkillTpl->mUseCondition ); i++ )
	{
		switch ( tpSkillTpl->mUseCondition[ i ].mConditionType )
		{
		case CTemplateSkill::SKILL_USECONDITION_NONE:
			{
				return SUCCESS;
			}
		case CTemplateSkill::SKILL_USECONDITION_HPPER:
			{
				if ( bIsMagicWeapon ) // 法宝技能
				{
					int tMaxHP = tpMW->GetMaxHp();
					int tHPPer = (int) ( ( SERVER_PERCENT_FLOAT * tpMW->GetHp() ) / tMaxHP );
					if ( tHPPer <= tpSkillTpl->mUseCondition[ i ].mParam )
					{
						return ERROR_USESKILL_NEEDMORELIFE;
					}
				}
				else
				{
					CPropertyValue tMaxHPValue = CPropertyFormula::GetMaxHP( &mProperty );
					int tMaxHP = tMaxHPValue.mDeltaValue + tMaxHPValue.mInitValue;
					int tHPPer = (int) ( ( SERVER_PERCENT_FLOAT * mProperty.GetCurHP( ) ) / tMaxHP );
					if ( tHPPer <= tpSkillTpl->mUseCondition[ i ].mParam )
					{
						return ERROR_USESKILL_NEEDMORELIFE;
					}
				}
			}
			break;
		case CTemplateSkill::SKILL_USECONDITION_MPPER:
			{
				if ( bIsMagicWeapon ) // 法宝技能
				{
					int tMaxMP = tpMW->GetMaxMp();
					int tMPPer = (int) ( ( SERVER_PERCENT_FLOAT * tpMW->GetMp() ) / tMaxMP );
					if ( tMPPer <= tpSkillTpl->mUseCondition[ i ].mParam )
					{
						return ERROR_USESKILL_NEEDMORELIFE;
					}
				}
				else
				{
					CPropertyValue tMaxMPValue = CPropertyFormula::GetMaxMP( &mProperty );
					int tMaxMP = tMaxMPValue.mDeltaValue + tMaxMPValue.mInitValue;
					int tMPPer = (int) ( ( SERVER_PERCENT_FLOAT * mProperty.GetCurMP( ) ) / tMaxMP );
					if ( tMPPer <= tpSkillTpl->mUseCondition[ i ].mParam )
					{
						return ERROR_USESKILL_NEEDMOREMANA ;
					}
				}
			}
			break;
		case CTemplateSkill::SKILL_USECONDITION_ITEM:
			{
				int tItemNum = mProperty.BaggageHasItem( tpSkillTpl->mUseCondition[ i ].mParam );
				if ( tItemNum <= 0 )
				{
					return ERROR_USESKILL_NEEDMOREITEM ;
				}
			}
			break;
		}
	}

	return SUCCESS;
}


// 技能是否可以使用
int CEntityPlayer::CanUseSkill( CSkillObject* tpSkillObj, bool bIsMagicWeapon /*= false*/ )
{
	LK_ASSERT( tpSkillObj != NULL, return ERROR_USESKILL_SKILLNOTFOUND );	
	CTemplateSkill *tpSkill = tpSkillObj->GetSkillTpl( );
	if ( tpSkill == NULL )
	{
		return ERROR_USESKILL_SKILLNOTFOUND;
	}
	
	// 处于摆摊，交易服务状态，不让用技能
	if( GetServiceStatus() == SERVICE_TRADE || GetServiceStatus() == SERVICE_STALL )
	{
		return ERROR_USESKILL_UNUSE;
	}

	// 检查武器类型是否满足
	if ( CheckSkillWeapon( tpSkill ) == false )
	{
		return ERROR_USESKILL_WEAPONNEED;	
	}

	if ( !bIsMagicWeapon )
	{
		// 检查武器耐久度
		if( CPropertyModule::GetSingletonPtr()->EquipmentAvailable( this , EQUIPMENT_WEAPON ) == false )
		{
			return ERROR_USESKILL_EPTUNAVAILABLE;
		}
	}

	// 检查技能消耗
	if( CheckSkillCost( tpSkill, bIsMagicWeapon ) == false )
	{
		return ERROR_USESKILL_NEEDMOREMANA;
	}

	// 检查技能特殊条件
	int nSpecailCnd = CheckSpecialUseCondition( tpSkill, bIsMagicWeapon );
	if( nSpecailCnd != SUCCESS )
	{
		return nSpecailCnd;
	}

	// 检查技能组CD
	if (CheckCDGroup( tpSkill->mCDGroup, ARRAY_CNT( tpSkill->mCDGroup ) ) != SUCCESS )
	{
		return ERROR_USESKILL_NOTCOOLDOWN ;
	}

	// 检查技能冷却
	if( tpSkillObj->IsCommonCDSkill() &&  ( GetCommonCD() > 0 )  )
	{
		//LOG_DEBUG( "pro", "Entity( %d ) UseSkill( SkillID = %d ) common cd not cool down", this->GetEntityID(), tpSkillObj->GetSkillID() );
		return ERROR_USESKILL_NOTCOOLDOWN;
	}

	if( tpSkillObj->IsCoolingDown() )
	{
		//LOG_DEBUG( "pro", "Entity( %d ) UseSkill( SkillID = %d ) not cool down", this->GetEntityID(), tpSkillObj->GetSkillID() );
		return ERROR_USESKILL_NOTCOOLDOWN;
	}

	// 如果源已经死亡，那么不能使用
	if( IsAlive() == false )
	{
		return ERROR_USESKILL_SOURCEDIE;
	}

	if( GetAttackable() == false )
	{
		return ERROR_USESKILL_UNUSE ;
	}

	return SUCCESS;
}

// 技能冷却
void CEntityPlayer::DoSkillCoolDown( CSkillObject* pSkillObject, bool bIsMagicWeapon )
{
	LK_ASSERT( pSkillObject != NULL, return );
	
	// 计算技能冷却
	int nTimerOtherInfo[ TIMER_OTHER_INFO ] = {MESSAGE_PROPERTY,0,0,0};

	//if ( pSkillObject->IsCommonCDSkill() )
	//{
	//	// 重置公共冷却时间
	//	nTimerOtherInfo[1] = 0;
	//	mProperty.SetCommonCD( SKILL_COMMON_COOLDOWN );
	//	int nCommonCDTimer = GetTimerReciver()->SetTimer( GetEntityID(), TIMER_SKLL_COOLDOWN, SKILL_COMMON_COOLDOWN, 2, nTimerOtherInfo );
	//	pSkillObject->SetCommonCDTimer( nCommonCDTimer );
	//}

	nTimerOtherInfo[1] = pSkillObject->GetSkillID();
	nTimerOtherInfo[2] = pSkillObject->GetSkillLevel();
	if (bIsMagicWeapon)  // 如果是法宝技能 要送入法宝对象ID
	{
		CItemObject* tpMW = mProperty.GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
		if ( tpMW != NULL )
			nTimerOtherInfo[3] = tpMW->get_id();
		else
			LOG_ERROR("pro", "no mw?? skillid=%d", pSkillObject->GetSkillID() );
	}

	int nTimerID = GetTimerReciver()->SetTimer( GetEntityID(), 
		TIMER_SKLL_COOLDOWN, 
		pSkillObject->GetOriginalCooldown() - pSkillObject->GetReduceCoolFix(),
		4, nTimerOtherInfo );
	pSkillObject->SetCDTimer( nTimerID );
	pSkillObject->SetCooldown( pSkillObject->GetOriginalCooldown() - pSkillObject->GetReduceCoolFix() );
}

// 打断npc服务 （生产，升级）
void CEntityPlayer::InterruptNpcService()
{
	
}

// 检查技能武器是否满足
bool CEntityPlayer::CheckSkillWeapon( CTemplateSkill* tpSkillTpl )
{
	LK_ASSERT( tpSkillTpl != NULL, return false );

	// 如果是法宝技能 而且武器是无限制 直接返回true
	if ( tpSkillTpl->mIsMagicWeaponSkill == 1 && tpSkillTpl->mRequireWeapon == CTemplateWeapon::WEAPON_UNLIMIT )
		return true;
	
	// 判断技能攻击方式
	int tWeaponType = 0;
	CItemObject* tpWeapon = mProperty.GetEquipment( )->GetItemObjPtr( EQUIPMENT_WEAPON );
	if ( tpWeapon == NULL )
		return false;

	CTemplateWeapon* tpWeaponTpl = ( CTemplateWeapon* ) CDataStatic::SearchTpl( tpWeapon->GetItemID() );
	if ( tpWeaponTpl == NULL )
		return false;     	
	tWeaponType = tpWeaponTpl->mWeaponType;

	// 如果技能需求武器不满足要求
	if ( tpSkillTpl->mRequireWeapon != CTemplateWeapon::WEAPON_UNLIMIT && tWeaponType != CTemplateWeapon::WEAPON_UNLIMIT && 
		tpSkillTpl->mRequireWeapon != tWeaponType )
		return false;

	return true;
}

int CEntityPlayer::RefreshCDGroup( int *vCDGroup, int vNum )
{
	for ( int i = 0; i < vNum; i++ )
	{
		if ( vCDGroup[ i ] == 0 )
			break;
		if ( vCDGroup[ i ] < 0 || vCDGroup[ i ] > ( CDGROUP - 1 ) )
			continue;
		CTemplateCDGroupTable *tplTable = (CTemplateCDGroupTable*)CDataStatic::GetTemp( TEMP_CDGROUP );
		if ( tplTable == NULL )
			continue;

		CGroupCD& rGroupCD = mProperty.GetCDGroupList().GetGroupCD( vCDGroup[ i ] );
		rGroupCD.SetCDTime( tplTable->mCDTime[ vCDGroup[ i ] ] );

		int nOtherInfo[4] = { MESSAGE_PROPERTY, rGroupCD.GetCDGroupIdx(), 0, 0 };

		//如果之前有cd
		if( rGroupCD.GetTimerID() != INVALID_OBJ_ID )
		{
			GetTimerReciver()->ClearTimer( rGroupCD.GetTimerID() );
		}

		int nTimerID = GetTimerReciver()->SetTimer( GetEntityID(), TIMER_CDGROUP, rGroupCD.GetCDTime(), 2, nOtherInfo ); 
		rGroupCD.SetTimerID( nTimerID );
	}
	return SUCCESS;
}

// 清除CD组
int CEntityPlayer::ClearCDGroup( int *nCDGroup, int nNum )
{
	for ( int i = 0; i < nNum; i++ )
	{
		if ( nCDGroup[ i ] == 0 )
			break;
		if ( nCDGroup[ i ] < 0 || nCDGroup[ i ] > ( CDGROUP - 1 ) )
			continue;
		CTemplateCDGroupTable *tplTable = (CTemplateCDGroupTable*)CDataStatic::GetTemp( TEMP_CDGROUP );
		if ( tplTable == NULL )
			continue;

		CGroupCD& rGroupCD = mProperty.GetCDGroupList().GetGroupCD( nCDGroup[ i ] );
		rGroupCD.SetCDTime( 0 );

		//如果之前有cd
		if( rGroupCD.GetTimerID() != INVALID_OBJ_ID )
		{
			GetTimerReciver()->ClearTimer( rGroupCD.GetTimerID() );
			rGroupCD.SetTimerID( INVALID_OBJ_ID );
		}

	}

	return SUCCESS;
}

int CEntityPlayer::CheckCDGroup( int *vCDGroup, int vNum )
{
	for ( int i = 0; i < vNum; i++ )
	{
		if ( vCDGroup[ i ] == 0 )
			break;
		if ( vCDGroup[ i ] < 0 || vCDGroup[ i ] > ( CDGROUP - 1 ) )
			continue;

		CGroupCD& rGroupCD = mProperty.GetCDGroupList().GetGroupCD( vCDGroup[ i ] );
		if ( rGroupCD.GetCDTime( ) > 0 )
			return ERROR_NOTCOOLDOWN;
	}
	return SUCCESS;
}


// 跨地图时检查buff
int CEntityPlayer::CheckBuffOnChangeMap( CMapObject* vpMap )
{
	CPropertyPlayer* tpProPlayer = (CPropertyPlayer*)GetProperty();
	CBuffList* pBuffList = tpProPlayer->GetBuffList();

	int tRemoveBuffList[ CBuffList::MAX_BUFF_NUM ] = { 0 };
	int tRemoveNumber = 0;

	for( int i = 0; i < pBuffList->GetBuffNum() && tRemoveNumber < ( int )ARRAY_CNT( tRemoveBuffList ); i++ )
	{
		CBuffObject& rBuffObj = pBuffList->GetBuffObjectByIndex( i );
		
		CTemplateBuff* tpBuff = ( CTemplateBuff* ) CDataStatic::SearchTpl( rBuffObj.mBuffID );
		if ( tpBuff == NULL )
		{
			continue;
		}

		if( CheckBuffCondition( tpBuff, vpMap ) == false )
		{
			tRemoveBuffList[ tRemoveNumber ] = tpBuff->mTempID;
			tRemoveNumber++;
			continue;
		}
	}

	for( int i = 0; i < tRemoveNumber && i < ( int )ARRAY_CNT( tRemoveBuffList ); i++ )
	{
		EntityRemoveBuff( tRemoveBuffList[ i ] );
		LogEventLostBuffByChangeMap( this, tRemoveBuffList[ i ]);  // 跨地图失去BUFF
	}

	return 0;
}

bool CEntityCharacter::CheckBuffCondition( CTemplateBuff* vpBuff, CMapObject* vpMap )
{
	LK_ASSERT( vpBuff != NULL, return false );
	LK_ASSERT( vpMap != NULL, return false );

	if( vpBuff->mCountryBuff != 0 )
	{
		if( GetLineID( ) != GetNationality( ) )
		{
			return false;
		}	
	}

	if( vpBuff->mBuffMapType != CTemplateBuff::NON_BUFF )
	{
		if( vpMap != NULL )
		{
			switch( vpBuff->mBuffMapType )
			{
				case CTemplateBuff::BATTLE_BUFF:
					{
						if( vpMap->IsBattleMap( ) != true )
						{
							return false;
						}
					}
					break;
				case CTemplateBuff::FB_BUFF:
					{
						if( vpMap->IsFBMap( ) != true )
						{
							return false;
						}
					}
					break;
				case CTemplateBuff::COMM_BUFF:
					{
						if( vpMap->IsCommMap( ) != true )
						{
							return false;
						}

					}
					break;
				case CTemplateBuff::GROUND_BUFF:
					{
						if( vpMap->IsGroundMap( ) != true )
						{
							return false;
						}
					}
					break; 
				default:
					break;
			}
		}
	}

	// 判断当前BUFF是否是特定地图才可使用的BUFF ( 0 表示全地图可用 )
	if ( vpBuff->mBuffMapID != 0 )
	{
		if ( GetMapID( ) != vpBuff->mBuffMapID )
		{
			// 非指定地图,则删除BUFF
			// 因为是反向遍历,所以循环中删除当前BUFF,允许
			//				EntityRemoveBuff( tpBuff->mTempID );
			return false;
		}
	}

	if ( vpBuff->mBuffLineID != 0 )
	{
		// 对副本要特殊处理，因为副本可以有多个，每个有不同的lineid,用宏来判断最好了
		if( IS_REPETION_LINE_ID( vpBuff->mBuffLineID ) )
		{
			if( ! IS_REPETION_LINE_ID( GetLineID( ) ) ) 
				return false;
		}
		else
		{
			if ( GetLineID( ) != vpBuff->mBuffLineID )
				return false;
		}
	}

	return true;
}



//************************************
// Method:    DailyCountRefresh
// FullName:  CEntityPlayer::DailyCountRefresh
// Access:    public 
// Returns:   void
// Qualifier: 每日计数的数据道是刷新
//************************************
void CEntityPlayer::DailyCountRefresh( unsigned int unDate )
{
	if( (unDate != 0) && (unDate != (unsigned int) mProperty.GetSaveDate()) )
	{
		mProperty.SetSaveDate( unDate );
		mProperty.SetInHonPerDay( 0 );
		mProperty.SetOutHonPerDay( 0 );
		mProperty.SetWelfareStatus( 0 );
		//pProperty->SetDiploid( 0 );		
		CActivityModule::GetSingleton().ClearHydrangeaStatus( this );

		//repetion
		CRepetionModule::GetSingletonPtr( )->CleanRepetion( &mProperty );
	}
	
}

//************************************
// Method:   CheckMsgFrequency
// FullName: 检查消息频率
// Access:    public 
// Returns:   bool 
// Qualifier: 
//************************************
bool CEntityPlayer::CheckMsgFrequency( int nMsgLimitPerSec, int nCountLimit )
{ 

	if (  ++mMsgCount < nCountLimit )
	{
		return true;
	}

	int tTimeLength = time(NULL) - mFirstMsgTime; 

	if ( tTimeLength == 0 )
	{
		return false;	
	}	
		
	if ( mMsgCount/tTimeLength >= nMsgLimitPerSec )
	{
		return false;
	}
	mMsgCount = 0;
	mFirstMsgTime = time(NULL);
	return true;									
}

// 处理荣誉 ( 暂时只有PLAYER使用 )
void CEntityPlayer::HonorForDie( CEntityCharacter* pSrcEntity )
{
	// 暂时只有玩家杀玩家才会获得荣誉
	if ( pSrcEntity->IsPlayer() )
	{
		int tpMemberList[ TEAMNUM ] = { 0 };
		CEntityPlayer* tpPlayer = (CEntityPlayer*) pSrcEntity;
		int tRtn = 0, tCount = 0;
		tRtn = CTeamModule::GetSingleton( ).OnGetMemberList( tpPlayer->GetCharID( ), tpMemberList, ARRAY_CNT( tpMemberList ), tCount );
		if ( tRtn < 0 )
		{
			LOG_ERROR( "team", "in honor, get team list failed.id=%d,[%s:%d]", tpPlayer->GetCharID(), __LK_FILE__, __LINE__ );
			return ;
		}
		
		CPropertyModule::GetSingleton( ).PlayerObtainHonor( tpPlayer, this, tpMemberList, tCount );
	}
}

// PK规则 ( 暂时只有PLAYER使用 )
void CEntityPlayer::PKRuleForDie( CEntityCharacter* pSrcEntity )
{
	// 判断是否红名
	CPropertyModule::GetSingleton( ).CheckPKIsRed( pSrcEntity, this );

	// 如果玩家有队伍，告知队伍成员
	if ( GetTeamFlag( ) > TEAM_NONE )
	{
		//CTeamModule::GetSingleton( ).TeamMemberDie( this );
	}
}



// 处理召唤兽逻辑 ( 暂时只有PLAYER,PET使用 ) 【晕 这函数没地方调】
void CEntityPlayer::PetLogicForDie( )
{
	CPropertyPlayer* pDesProperty = (CPropertyPlayer*) GetProperty( );

	if ( pDesProperty->GetPetID( ) != 0 )
	{
		// 销毁召唤兽
		pDesProperty->DestroyPetEntity( );
	}
}

void CEntityPlayer::NotifyPropertyRefresh( )
{
	CPropertyPlayer* tpProperty = ( CPropertyPlayer* )GetProperty();	
	CMessage tMessage;
	CMessageRefreshPropertyNotify tPara;
	PBInformation *tpInformation = tPara.mutable_information();
	if ( tpInformation == NULL )
		return;
	PBPropertyValue *tValue;

	tpInformation->set_remainpoint( tpProperty->GetRemainPoint() );
	tpInformation->set_remainskillpoint( tpProperty->GetSkillPoint() );

	tValue = tpInformation->mutable_str();
	tValue->set_initvalue( tpProperty->GetStr( ).mInitValue );
	//if( tpProperty->GetStr( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetStr( ).mDeltaValue );

	tValue = tpInformation->mutable_wis();
	tValue->set_initvalue( tpProperty->GetWis( ).mInitValue );
	//if( tpProperty->GetWis( ).mDeltaValue !=0 )
		tValue->set_deltavalue( tpProperty->GetWis( ).mDeltaValue );
	
	tValue = tpInformation->mutable_spr();
	tValue->set_initvalue( tpProperty->GetSpr( ).mInitValue );
	//if( tpProperty->GetSpr( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetSpr( ).mDeltaValue );
	
	tValue = tpInformation->mutable_con();
	tValue->set_initvalue( tpProperty->GetCon( ).mInitValue );
	//if( tpProperty->GetCon( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetCon( ).mDeltaValue );
	
	tValue = tpInformation->mutable_sta();
	tValue->set_initvalue( tpProperty->GetSta( ).mInitValue );
	//if( tpProperty->GetSta( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetSta( ).mDeltaValue );
	
	tValue = tpInformation->mutable_maxhp();
	tValue->set_initvalue( tpProperty->GetMaxHP( ).mInitValue );
	//if( tpProperty->GetMaxHP( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetMaxHP( ).mDeltaValue );


	tValue = tpInformation->mutable_maxmp();
	tValue->set_initvalue( tpProperty->GetMaxMP( ).mInitValue );
	//if( tpProperty->GetMaxMP( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetMaxMP( ).mDeltaValue );
	
	tValue = tpInformation->mutable_maxap();
	tValue->set_initvalue( tpProperty->GetMaxAP( ).mInitValue );
	//if( tpProperty->GetMaxAP( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetMaxAP( ).mDeltaValue );
	
	tValue = tpInformation->mutable_pattackmax();
	tValue->set_initvalue( tpProperty->GetPAttackMax( ).mInitValue );
	//if( tpProperty->GetPAttackMax( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetPAttackMax( ).mDeltaValue );
	
	tValue = tpInformation->mutable_pattackmin();
	tValue->set_initvalue( tpProperty->GetPAttackMin( ).mInitValue );
	//if( tpProperty->GetPAttackMin( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetPAttackMin( ).mDeltaValue );
	
	tValue = tpInformation->mutable_mattackmax();
	tValue->set_initvalue( tpProperty->GetMAttackMax( ).mInitValue );
	//if(  tpProperty->GetMAttackMax( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetMAttackMax( ).mDeltaValue );
	
	tValue = tpInformation->mutable_mattackmin();
	tValue->set_initvalue( tpProperty->GetMAttackMin( ).mInitValue );
	//if( tpProperty->GetMAttackMin( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetMAttackMin( ).mDeltaValue );

	tValue = tpInformation->mutable_deathrate();
	tValue->set_initvalue( tpProperty->GetDeathRate( ).mInitValue );
	//if( tpProperty->GetDeathRate( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetDeathRate( ).mDeltaValue );

	tValue = tpInformation->mutable_deathvalue();
	tValue->set_initvalue( tpProperty->GetDeathValue( ).mInitValue );
	//if( tpProperty->GetDeathValue( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetDeathValue( ).mDeltaValue );
	
	tValue = tpInformation->mutable_deathdefense();
	tValue->set_initvalue( tpProperty->GetDeathDefense( ).mInitValue );
	//if( tpProperty->GetDeathDefense( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetDeathDefense( ).mDeltaValue );
	
	tValue = tpInformation->mutable_normalduck();
	tValue->set_initvalue( tpProperty->GetNormalDuck( ).mInitValue );
	//if( tpProperty->GetNormalDuck( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetNormalDuck( ).mDeltaValue );
	
	tValue = tpInformation->mutable_skillduck();
	tValue->set_initvalue( tpProperty->GetSkillDuck( ).mInitValue );
	//if( tpProperty->GetSkillDuck( ).mDeltaValue !=0 )
		tValue->set_deltavalue( tpProperty->GetSkillDuck( ).mDeltaValue );
	
	tValue = tpInformation->mutable_ignoreduck();
	tValue->set_initvalue( tpProperty->GetIgnoreDuck( ).mInitValue );
	//if( tpProperty->GetIgnoreDuck( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetIgnoreDuck( ).mDeltaValue );
	
	tValue = tpInformation->mutable_speed();
	tValue->set_initvalue( tpProperty->GetSpeed( ).mInitValue );
	//if( tpProperty->GetSpeed( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetSpeed( ).mDeltaValue );
	
	tValue = tpInformation->mutable_pdefence();
	tValue->set_initvalue( tpProperty->GetPDefence( ).mInitValue );
	//if( tpProperty->GetPDefence( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetPDefence( ).mDeltaValue );

	tValue = tpInformation->mutable_mdefence();
	tValue->set_initvalue( tpProperty->GetMDefence( ).mInitValue );
	//if( tpProperty->GetMDefence( ).mDeltaValue != 0 )
		tValue->set_deltavalue( tpProperty->GetMDefence( ).mDeltaValue );

	tValue = tpInformation->mutable_maxap();
	tValue->set_initvalue( tpProperty->GetMaxAP().mInitValue );
	tValue->set_deltavalue( tpProperty->GetMaxAP().mDeltaValue );

	//if( tpProperty->GetAddFixDamage( ) != 0 )
		tpInformation->set_addfixdamage( tpProperty->GetAddFixDamage( ) );
	//if( tpProperty->GetAddPerDamage( ) != 0 )
		tpInformation->set_addperdamage( tpProperty->GetAddPerDamage( ) );
	//if( tpProperty->GetDecFixPDamage( ) != 0 )
		tpInformation->set_decfixpdamage( tpProperty->GetDecFixPDamage( ) );
	//if( tpProperty->GetDecFixMDamage( ) != 0 )
		tpInformation->set_decfixmdamage( tpProperty->GetDecFixMDamage( ) );
	//if( tpProperty->GetDecPerPDamage( ) != 0 )
		tpInformation->set_decperpdamage( tpProperty->GetDecPerPDamage( ) );
	//if( tpProperty->GetDecPerMDamage( ) != 0  )
		tpInformation->set_decpermdamage( tpProperty->GetDecPerMDamage( ) );
	//if( tpProperty->GetStealHP( ) != 0 )
		tpInformation->set_stealhp( tpProperty->GetStealHP( ) );
	//if( tpProperty->GetStealMP( ) != 0 )
		tpInformation->set_stealmp( tpProperty->GetStealMP( ) );
	//if( tpProperty->GetNormalHitRate( ) != 0 )
		tpInformation->set_normalhitrate( tpProperty->GetNormalHitRate( ) );
	//if( tpProperty->GetSkillHitRate( ) != 0 )
		tpInformation->set_skillhitrate( tpProperty->GetSkillHitRate( ) );
	//if( tpProperty->GetAttackType( ) != 0 )
		tpInformation->set_attacktype( (int)tpProperty->GetAttackType( ) );
	//if( tpProperty->GetSkillPoint() != 0 )
		tpInformation->set_remainskillpoint( tpProperty->GetSkillPoint() );
	//if( tpProperty->GetRemainPoint() != 0 )
		tpInformation->set_remainpoint( tpProperty->GetRemainPoint() );

	for ( int i = 0; i < RES_NUM; i ++ )
	{
		tpInformation->add_resdef( tpProperty->GetResDef( i ) );
		tpInformation->add_resatt( tpProperty->GetResAtt( i ) );
	}

	// 技能影响CD
	CSkillList& pSkillList = tpProperty->GetSkillList();
	for ( int i = 0; i < pSkillList.GetSkillNum(); i++ )
	{
		CSkillObject* pSkillObj = pSkillList.GetSkillObjectByIndex( i );
		if ( pSkillObj->GetReduceCoolFix() > 0 || pSkillObj->GetIncLevel( ) > 0 )
		{
			tpInformation->add_skillindex( i );
			if( pSkillObj->GetReduceCoolFix() != 0 )
				tpInformation->add_reducecool( pSkillObj->GetReduceCoolFix() );
			if( pSkillObj->GetIncLevel( ) != 0 )
				tpInformation->add_inclevel( pSkillObj->GetIncLevel( ) );
		}
	}
	//if( tpProperty->GetReduceLevel( ) != 0 )
		tpInformation->set_reducelevel( tpProperty->GetReduceLevel( ) );
	for( int i = 0; i < WIZARD_NUM; i++ )
	{
		tpInformation->add_equipwizard( tpProperty->GetEquipWizard( i ) );
	}

	tPara.set_entityid( mEntityID );
	tPara.set_curhp( tpProperty->GetCurHP( ) );
	tPara.set_curmp( tpProperty->GetCurMP( ) );
	tPara.set_curap( tpProperty->GetCurAP( ) );
	tPara.set_level( tpProperty->GetLevel( ) );


	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_REFRESHPROPERTY );
	tMessage.set_msgpara( ( long ) &tPara );

	// 玩家发给自己
	CSceneLogic::GetSingleton().Send2Player( this, &tMessage );
}



// 发送使用技能回执信息
void CEntityPlayer::SendUseSkillErrorNotice( int nType )
{
	CMessage tTotalMessage;

	CMessageHead* tpHead = NULL;
	tpHead = tTotalMessage.mutable_msghead();
	if ( tpHead == NULL )
	{
		LOG_DEBUG( "pro", "[ %s : %d ] [ %s ]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return;
	}

	tpHead->set_messageid( ID_S2C_NOTIFY_PROERROR );

	CMessageProErrorNotice tMessage;
	tMessage.set_errorcode( nType );

	tTotalMessage.set_msgpara( (long)&tMessage );

	CSceneLogic::GetSingleton().Send2Player( this, &tTotalMessage );
}

// 实体死亡时,清除BUFF
void CEntityPlayer::RemoveBuffEntityDie()
{
	CPropertyCharacter* pDesProperty = (CPropertyCharacter*) GetProperty();
	CBuffList *tpBuff = pDesProperty->GetBuffList( );

	for ( unsigned int i = tpBuff->mBuffNumber - 1; i >= 0 && i < ARRAY_CNT( tpBuff->mBuffList ); i-- )
	{
		CTemplateBuff* tpBuffTpl = (CTemplateBuff*) CDataStatic::SearchTpl( tpBuff->mBuffList[ i ].mBuffID );
		if ( tpBuffTpl == NULL )
		{
			LOG_ERROR( "pro", "[ %s : %d ] [ %s ] CTemplateBuff is NULL, TempID = %d", __LK_FILE__, __LINE__, __FUNCTION__, tpBuff->mBuffList[ i ].mBuffID );
			continue;
		}

		if ( tpBuffTpl->mBuffDieDel == CTemplateBuff::BUFF_CANCEL )
		{
			EntityRemoveBuff( tpBuffTpl->mTempID, tpBuff->mBuffList[ i ].mListID );
			LogEventLostBuffByDeath(this,tpBuff->mBuffList[ i ].mBuffID );
			LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] Remove Buff! for ActiveRemoveBuff error test!", __LK_FILE__, __LINE__, __FUNCTION__ );
		}
	}
}

// 改变红黄名时增加buff
void CEntityPlayer::ChangePKBuff( int vOldPKStatus, int vNewPKStatus, int vOldValue, int vNewValue )
{
	if ( vOldPKStatus == PKSTATUS_YELLOW )
	{

	}
	if ( vOldPKStatus == PKSTATUS_RED )
	{

	}
	if ( vNewPKStatus == PKSTATUS_YELLOW )
	{

	}
	if ( vNewPKStatus == PKSTATUS_RED )
	{

	}
}


// 开启活动双倍时间
void CEntityPlayer::OpenActiveDoubleTime( int nRate)
{
	CPropertyPlayer* tpProperty = (CPropertyPlayer*)GetProperty();

	// 如果已有领取，先强制停止，再清除计时器
	int tTimerID = tpProperty->GetDiploidState( TIMEID_STATE );
	if ( tTimerID != 0 )
	{
		GetTimerReciver()->TimeOutOneTimer( tTimerID );
		GetTimerReciver()->ClearTimer( tTimerID );	
		tpProperty->SetDoubleTimerID( 0 );
	}

	tpProperty->SetActivityState( nRate );
}

// 关闭活动双倍时间
void CEntityPlayer::CloseActiveDoubleTime( )
{
	CPropertyPlayer* tpProperty = (CPropertyPlayer*)GetProperty();

	// 无论如何先停止了活动双再说
	tpProperty->SetActivityState( 0 );

	// 如果已经是冻结状态 
	if ( tpProperty->IsFreezeDouble() )
	{
		// 已经冻结，不需要
		return ;
	}
	// 非冻结状态，要启动定时器
	else if ( tpProperty->UnFreezeDouble() )
	{
		// 开启个人领双计时
		int nTimerOtherInfo[ TIMER_OTHER_INFO ] = {MESSAGE_PROPERTY,0,0,0};
		int tTimerID = this->GetTimerReciver()->SetTimer( this->get_id(), TIMER_DIPLOID_TIME, tpProperty->GetValidDiploid(), 1, nTimerOtherInfo );

		// 将TimerID维护起来，用来冻结时，清除计时器
		tpProperty->SetDoubleTimeState( true );	// 处于双倍经验状态
		tpProperty->SetDoubleFreezeState( false );		// 非冻结状态
		tpProperty->SetDoubleTimerID( tTimerID );
	}

}

int CEntityPlayer::CalProfileCheckSum()
{
	m_nProfileCheckSum = 0;
	// 校验 role_id, role_expr, role_money, bounded_money, role_level
	int nRoleID = GetCharID();	
	int nExp = GetExp();
	int nLevel = GetLevel();
	int nMoney = GetMoney();
	int nBindMoney = GetBindMoney();

	m_nProfileCheckSum += CalSum((const char*) &nRoleID, sizeof(int));
	m_nProfileCheckSum += CalSum((const char*) &nExp, sizeof(int));
	m_nProfileCheckSum += CalSum((const char*) &nLevel, sizeof(int));
	m_nProfileCheckSum += CalSum((const char*) &nMoney, sizeof(int));
	m_nProfileCheckSum += CalSum((const char*) &nBindMoney, sizeof(int));

	return m_nProfileCheckSum;
}

int CEntityPlayer::CalDetailCheckSum(const char* from, size_t nSize)
{
	//role_id, role_package, role_storage, role_package_kitbag, role_storage_kitbag
	
	m_nDetailCheckSum += CalSum(from, nSize);

	return m_nDetailCheckSum;
}


void CEntityPlayer::SetFcmStatus (EFcmStatus emStatus) 
{ 
	mFcmStatus = emStatus; 
} 

// 得到宠物
CEntityPet* CEntityPlayer::GetPet( )
{
	return (CEntityPet*) CSceneObjManager::GetSingletonPtr()->GetObject( GetPetID() );
}


//// 玩家设置过的定时器超时，以便让剩余时间存盘
//void CEntityPlayer::TimeoutTimerForSaveData(  )
//{
//	CPropertyPlayer* pPro = (CPropertyPlayer*)GetProperty();
//
//	// cd组
//	for( int i = 0; i < pPro->GetCDGroupList().GetMaxGroupIdx(); i++ )
//	{
//		CGroupCD& rGroupCD = pPro->GetCDGroupList().GetGroupCD( i );
//		if( rGroupCD.GetTimerID() != INVALID_OBJ_ID )
//		{
//			// 超时处理函数会负责删除定时器
//			GetTimerReciver()->TimeOutOneTimer( rGroupCD.GetTimerID() ); 
//		}
//	}
//
//	// 超时技能CD
//	CSkillList& pSkillList = pPro->GetSkillList();
//
//	// 公共CD时间很短，无需存盘,直接删除
//	if( CSkillObject::GetCommonCDTimer() != INVALID_OBJ_ID )
//	{
//		GetTimerReciver()->ClearTimer( CSkillObject::GetCommonCDTimer() );
//	}
//
//	// 普通技能需要超时
//	for( int i = 0; i < pSkillList.GetSkillNum(); i++ )
//	{
//		CSkillObject* pSkillObj = pSkillList.GetSkillObjectByIndex( i );
//		if( pSkillObj->GetCDTimer() != INVALID_OBJ_ID )
//		{
//			GetTimerReciver()->TimeOutOneTimer( pSkillObj->GetCDTimer() );
//			GetTimerReciver()->ClearTimer( pSkillObj->GetCDTimer() );
//		}
//	}
//
//	// 法宝技能需要超时
//	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)pPro->GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
//	if ( tpMW != NULL )
//	{
//		tpMW->OnStopSkillCD(this);
//	}
//
//	// 装备技能需要超时
//	CSkillList* tpSkillList = pPro->GetEquipSkill();
//	LK_ASSERT( tpSkillList != NULL, return );
//	for( int i = 0; i < tpSkillList->GetSkillNum(); i++ )
//	{
//		CSkillObject* pSkillObj = tpSkillList->GetSkillObjectByIndex( i );
//		if( pSkillObj->GetCDTimer() != INVALID_OBJ_ID )
//		{
//			GetTimerReciver()->TimeOutOneTimer( pSkillObj->GetCDTimer() );
//			GetTimerReciver()->ClearTimer( pSkillObj->GetCDTimer() );
//		}
//	}
//
//	// 造化技能需要超时
//	tpSkillList = &pPro->mTalentSkill;
//	for( int i = 0; i < tpSkillList->GetSkillNum(); i++ )
//	{
//		CSkillObject* pSkillObj = tpSkillList->GetSkillObjectByIndex( i );
//		if( pSkillObj->GetCDTimer() != INVALID_OBJ_ID )
//		{
//			GetTimerReciver()->TimeOutOneTimer( pSkillObj->GetCDTimer() );
//			GetTimerReciver()->ClearTimer( pSkillObj->GetCDTimer() );
//		}
//	}
//
//	// 超时Buff CD
//	CBuffList* pBuffList = pPro->GetBuffList();
//	for( int i = 0; i < pBuffList->GetBuffNum(); i++ )
//	{
//		CBuffObject& rBuffObj =  pBuffList->GetBuffObjectByIndex( i );
//		if( rBuffObj.GetBuffTimerID() != INVALID_OBJ_ID )
//		{
//			GetTimerReciver()->TimeOutOneTimer( rBuffObj.GetBuffTimerID() );
//		}
//	}
//
//
//	// 领双
//	// set left valid diploid time	
//	if ( pPro->UnFreezeDouble() )
//	{
//		// 没有冻结的话，双倍会随系统时间流失
//		time_t tNow = time( NULL ); 
//		// record off line time
//		pPro->SetLastStopTime( (int) tNow );
//	}
//	
//	if ( pPro->GetDiploidTimerID() != INVALID_OBJ_ID )
//	{
//		GetTimerReciver()->TimeOutOneTimer( pPro->GetDiploidTimerID() );
//		GetTimerReciver()->ClearTimer( pPro->GetDiploidTimerID() );
//	}
//
//	pPro->SetDoubleTimerID( 0 );
//	pPro->SetActivityState( false );
//
//	// 清除黄名, 屠杀模式不能清
//	// 不清了
//	/*if ( pPro->GetYellowTimer() != INVALID_OBJ_ID && GetPKMode() != PKMODE_ALL )
//	{
//		pPro->SetPKStatus( PKSTATUS_WHITE );
//		pPro->SetYellowTimer( LK_ZERO );
//		pPro->SetYellowStatus( false );
//	}*/
//}


// 获取定时器的剩余时间
void CEntityPlayer::GetLeftTimeofPlayerTimer( )
{

	CPropertyPlayer* pPro = (CPropertyPlayer*)GetProperty();

	// cd组
	pPro->GetCDGroupList().RefreshCDTime();

	// 超时技能CD
	pPro->GetSkillList().RefreshCooldown();

	
	// 法宝技能需要超时
	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)pPro->GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
	if ( tpMW != NULL )
	{
		CSkillList* tpMWList = tpMW->GetMWSkillList();
		tpMWList->RefreshCooldown();
	}

	// 装备技能需要超时
	CSkillList* tpSkillList = pPro->GetEquipSkill();
	LK_ASSERT( tpSkillList != NULL, return );
	tpSkillList->RefreshCooldown();

	// 造化技能需要超时
	tpSkillList = pPro->GetTallentSkill();
	LK_ASSERT( tpSkillList != NULL, return );
	tpSkillList->RefreshCooldown();

	// 超时Buff CD
	CBuffList* pBuffList = pPro->GetBuffList();
	LK_ASSERT( pBuffList != NULL, return );
	pBuffList->RefreshBuffLeftTime();


	// 领双
	// set left valid diploid time	
	if ( pPro->UnFreezeDouble() )
	{
		// 没有冻结的话，双倍会随系统时间流失
		pPro->SetLastStopTime( (int)time( NULL ) );
	}

	if ( pPro->GetDiploidTimerID() != INVALID_OBJ_ID )
	{
		int nLeftSec = 0;
		int nLeftMillSec = 0;
		int nGetTimerRet = CTimerMng::GetSingletonPtr()->GetLeftTime( pPro->GetDiploidTimerID(), nLeftSec, nLeftMillSec );

		// cd组还没有超过24小时的
		if( nGetTimerRet == 0 )
		{
			pPro->SetValidDiploid( nLeftMillSec );
		}
		
	}

}


//************************************
// Method:    IsNewCharacter
// FullName:  CEntityPlayer::IsNewCharacter
// Access:    public 
// Returns:   bool
// Qualifier: 判断是否是新建角色,
//			之所以用OnlineTime来判断，是因为这个值是增量写入, 不会有数据库中的值被
//			重置回0的情况，而且新建的角色在线事件一定为0。
//************************************
bool CEntityPlayer::IsNewCharacter( )
{
	return ( GetOnlineTime() == 0 );
}


//////////////////////////////////////////////////////////////////////////
//		CEntityNpc
//////////////////////////////////////////////////////////////////////////

// 主人ID
unsigned int CEntityNpc::GetOwnerID()
{
	// NPC的主人为他自己
	return GetEntityID();
}

// 是否敌对
bool CEntityNpc::IsHostileTo( CEntity* pEntity )
{
	LK_ASSERT( pEntity != NULL, return false );

	// 如果是自己
	if( this == pEntity )
	{
		return false;
	}
    if( pEntity->IsCharacter( ) == true )
    {
        int tSrcCampID = GetCampID( );
        int tDesCampID = ( ( CEntityCharacter* )pEntity )->GetCampID( );

        if( tSrcCampID != 0 && tDesCampID != 0 )
        {
            return tSrcCampID != tDesCampID;
        }
    }

	switch ( pEntity->GetEntityType() )
	{
		case ENTITYTYPE_PLAYER:
			{
				// GM不能被攻击
				if( CGMModule::GetSingleton().IsGm( ((CEntityPlayer*)pEntity)->GetGmStatus() ) == true  )
				{
					return false;
				}
			}
		case ENTITYTYPE_PET:
			{
				if ( GetOwnCampID() != 0 ) 					
				{
					if ( pEntity->GetNationality() != GetOwnCampID() )
						return true;
					else
						return false;
				}
				if ( GetNationality() == pEntity->GetNationality() )
				{
					if ( ( mPKType & ENEMY_WITH_SC ) == ENEMY_WITH_SC )
					{
						return true;
					}
				}
				else
				{
					if ( ( mPKType & ENEMY_WITH_FC ) == ENEMY_WITH_FC )
					{
						return true;
					}
				}
				break;
			}
		case ENTITYTYPE_OGRE:
		case ENTITYTYPE_FUNCNPC:
		//case ENTITYTYPE_TASKNPC:
			{
				CEntityNpc *pNpc = (CEntityNpc *)pEntity; 
				if ( mGroupID == pNpc->mGroupID )
					return false;
				if ( ( mPKType & ENEMY_WITH_OGRE ) == ENEMY_WITH_OGRE && ( pNpc->mPKType & ENEMY_TO_OGRE ) == ENEMY_TO_OGRE )
				{
					return true;
				}
				break;
			}
		default:
			break;
	}

	return false;
}

// 是否友好
bool CEntityNpc::IsFriendlyTo( CEntity* pEntity )
{
	LK_ASSERT( pEntity != NULL, return true );

	// 如果是自己
	if( this == pEntity )
	{
		return true;
	}

	switch ( pEntity->GetEntityType() )
	{
	case ENTITYTYPE_PLAYER:
	case ENTITYTYPE_PET:
		{
			// 如果是同国
			if ( GetOwnCampID() != 0 ) 
			{
				if ( pEntity->GetNationality() == GetOwnCampID() )
					return true;
				else
					return false;
			}
			if ( GetNationality() == pEntity->GetNationality() ) 
			{
				if( ( mPKType & FRIEND_TO_SC ) == FRIEND_WITH_SC || ( mPKType & NEUTRAL_TO_SC ) == NEUTRAL_WITH_SC )
				{
					return true;
				}
			}
			// 如果是其他国
			else
			{
				if( ( mPKType & FRIEND_TO_SC ) == FRIEND_WITH_FC || ( mPKType & NEUTRAL_TO_FC ) == NEUTRAL_WITH_FC )
				{
					return true;
				}
			}
			break;
		}
	case ENTITYTYPE_OGRE:
	case ENTITYTYPE_FUNCNPC:
		{
			CEntityNpc *pNpc = (CEntityNpc *)pEntity; 
			if ( mGroupID == pNpc->mGroupID )
				return true;
			if ( ( mPKType & ENEMY_WITH_OGRE ) == ENEMY_WITH_OGRE )
			{
				return false;
			}
			break;
		}
	default:
		break;
	}

	return true;
}

// 是否队友
bool CEntityNpc::IsTeamTo(CEntity *pEntity)
{
	return false;
}

// 是否是同一家族成员
bool CEntityNpc::IsFamilyTo( CEntity* pEntity )
{
	return false;
}

// 是否对玩家实体敌对
bool CEntityNpc::IsHostileToPlayers()
{
	return false;
}

// 组队状态
int CEntityNpc::GetTeamFlag( )
{
	return TEAM_NONE;
}


void CEntityNpc::TimeForDie( )
{
	if ( IsPet() || IsFunNpc() )
	{
		int nTimerOtherInfo[ TIMER_OTHER_INFO ] = {MESSAGE_PROPERTY,0,0,0};
		GetTimerReciver()->SetTimer( get_id(), TIMER_ENTITY_DISAPPER, GetProperty( )->GetDisapperaTimer( ), 1, nTimerOtherInfo );
		if ( mNpcChangeState == 1 ) // 如果是变怪NPC 记日志
		{
			LOG_NOTICE("default", "***ChangedNpcDie2 NPCID=%d entityid=%d", GetNpcID(), GetEntityID() );
		}
	}
}






void CEntityNpc::NotifyPropertyRefresh( )
{
	CPropertyNPC* tpProperty = ( CPropertyNPC* )GetProperty();	

	CMessage tMessage;
	CMessageRefreshPropertyNotify tPara;
	PBInformation *tpInformation = tPara.mutable_information();
	PBPropertyValue *tValue;

	tValue = tpInformation->mutable_maxhp();
	tValue->set_initvalue( tpProperty->GetMaxHP( ).mInitValue );
	tValue->set_deltavalue( tpProperty->GetMaxHP( ).mDeltaValue );

	tValue = tpInformation->mutable_maxmp();
	tValue->set_initvalue( tpProperty->GetMaxMP( ).mInitValue );
	tValue->set_deltavalue( tpProperty->GetMaxMP( ).mDeltaValue );

	tPara.set_entityid( mEntityID );
	tPara.set_curhp( tpProperty->GetCurHP( ) );
	tPara.set_curmp( tpProperty->GetCurMP( ) );
	tPara.set_level( tpProperty->GetLevel( ) );


	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_REFRESHPROPERTY );
	tMessage.set_msgpara( ( long ) &tPara );

	if ( this->IsOgre() || this->IsFunNpc() || this->IsBox() || this->IsPet() )
	{
		// npc 发给周围玩家
		CMapModule::GetSingletonPtr()->MessageBroadCast( this, &tMessage );
	}
}

// 获取名字
const char* CEntityNpc::GetName()
{
	CTemplateOgre* tpOgreTpl = (CTemplateOgre*) CDataStatic::SearchTpl( GetProperty()->GetPropertyTempID() );
	if ( tpOgreTpl == NULL )
		return NULL;

	return tpOgreTpl->mName;
}


//////////////////////////////////////////////////////////////////////////
//		CEntityOgre
//////////////////////////////////////////////////////////////////////////
bool CEntityOgre::IsHostileTo( CEntity* pEntity )
{
	return CEntityNpc::IsHostileTo( pEntity );
}


// 是否友好
bool CEntityOgre::IsFriendlyTo( CEntity* pEntity )
{
	return CEntityNpc::IsFriendlyTo( pEntity );
}

// 是否队友
bool CEntityOgre::IsTeamTo(CEntity *pEntity)
{
	return false;
}

// 是否同一家族成员
bool CEntityOgre::IsFamilyTo( CEntity* pEntity )
{
	return false;
}

// 是否对玩家实体敌对
bool CEntityOgre::IsHostileToPlayers()
{
	return true;
}

// 属主实体
unsigned int CEntityOgre::GetOwnerID()
{
	// OGRE的主人为他自己
	return GetEntityID();
}


// 用创建器为实体赋值
bool CEntityOgre::Setup( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos, bool bInit, int vNpcID )
{
	if( pCreator->GetCreatorType() != GetEntityType( ) )
	{
		return false;
	}

	COgreCreator *tpCreator = CCreatureRefresher::GetSingleton().OnMutiMapRefreshEvent( (COgreCreator*)pCreator );
	COgreCreator *pOgreCreator = (COgreCreator*)pCreator;
	if ( tpCreator != NULL )
	{
		pOgreCreator = tpCreator;		
	}
	// creatorid要用总点ID
	mCreatorID = pCreator->get_id();

	int tRand = RAND(SERVER_PERCENT_INT);
	short tLow = 0;
	short tHigh = 0;

	// 根据配置的概率刷怪
	if ( vNpcID == 0 )
	{
		for ( int i = 0; i < pOgreCreator->mRCount1; i++ )
		{
			tLow += ( i == 0 ) ? 0 : pOgreCreator->mRefresh1[ i - 1 ].mOgreRand;
			tHigh += pOgreCreator->mRefresh1[ i ].mOgreRand;
			if ( tRand >= tLow && tRand <= tHigh )
				mNpcID = pOgreCreator->mRefresh1[ i ].mOgreID;
		}
	}
	// 如果指定了NpcID
	else
	{
		mNpcID = vNpcID;
	}
				
	if ( mNpcID == 0 )
		return false;

	mLineID = usLineID;
	mMapID = usMapID;
	mIndex = usMapIndex;
	if ( *pPos != CWTPoint( 0, 0 ) )
	{
		mPos = *pPos;
	}
	else
	{
		mPos = pOgreCreator->mPosition;
	}
	CTemplateNpc *tplOgre = (CTemplateNpc *)CDataStatic::SearchTpl( mNpcID );
	if ( tplOgre != NULL )
	{
		mPKType = tplOgre->mPKType;
		mGroupID = tplOgre->mGroupID;
		if ( tplOgre->mLmtLineID != 0 && tplOgre->mLmtLineID != usLineID )
		{
			return false;
		}
		mOwnCampID = tplOgre->mWarCamp;
	}
	else
	{
		mPKType = 0;
		mGroupID = 0;
	}		

	// 设置存活时间
	for ( int i = 0; i < pOgreCreator->mRCount2; i++ )
	{
		if ( (int)mNpcID == pOgreCreator->mRefresh2[ i ].mOldID )
		{
			mOccurTime = pOgreCreator->mRefresh2[ i ].mTime;
			mOccurTimer = pOgreCreator->mRefresh2[ i ].mTime;
		}
	}

	// 判断特定NPC死亡刷新
	for ( int i = 0; i < pOgreCreator->mRCount7; i++ )
	{
		CCreator* tpCreator = CCreatorManager::GetSingletonPtr()->GetCreator( pOgreCreator->mRefresh7[ i ] );
		if ( tpCreator != NULL && tpCreator->mHasOgre != 0 )
		{
			return false;
		}
	}
	
	// 判断特定NPC存活刷新
	for ( int i = 0; i < pOgreCreator->mRCount8; i++ )
	{
		CCreator* tpCreator = CCreatorManager::GetSingletonPtr()->GetCreator( pOgreCreator->mRefresh8[ i ] );
		if ( tpCreator != NULL && tpCreator->mHasOgre == 0 )
		{
			return false;
		}
	}

	mEntityID = this->get_id( );
//	SetEntityType( ENTITYTYPE_OGRE );	
	mSuperState = false;
	//mMoveable = true;
	//mControlable = true;
	mDirection = pOgreCreator->mDirection;
	mMotionState = MOTION_IDLE;
	mAliveState = ALIVE_NORMAL;
	mHasTarget = false;
	mOrginPos = mPos;	
	mOrginMapID = mMapID;

	GetProperty()->SetEntityID( this->get_id( ) );
	GetProperty()->SetPropertyTempID( mNpcID );
	SetNationality(usLineID);	

	//CSceneLogic::GetSingleton( ).CreateEntity( this, pOgreCreator );

	// 延迟刷新
	//CCreatureRefresher::GetSingletonPtr()->OnPossibilityRefreshEvent( pOgreCreator, usMapIndex );
	if( IsOgre() || ( IsBlock() )|| ( IsBox() ))
	{
		GetProperty()->Refresh( true );
		//TODO: 这里不用刷新，因为就是刚刚创建的，中间没有改变，等有改变了再掉刷新也不迟啊
		//NotifyPropertyRefresh( );
	}

	// 记录刷怪状况也要用总点
	pCreator->mHasOgre = mEntityID;
	
	// 记录创建时间点
	mCreateTime = LINEKONG::GetTickCount();	

	return true;

}


// ***************************************************************
//  Function:		将目的实体加入到可见表
//  Description:	AddViewEntity
//	Input:			pTarget - 目标
//	OutPut:			无
//	Return:			true - 添加成功； false - 没有添加
//	Others:			怪物除了加人进可见表，也加召唤兽进来
//  Date:			11/26/2008
// 
// ***************************************************************
bool CEntityNpc::AddViewEntity(CEntity* pTarget)
{
	LK_ASSERT( pTarget != NULL, return false );

	if( this == pTarget )
	{
		return false;
	}

	// 加玩家
	if(  pTarget->IsPlayer() )
	{
		mSeenPlayerContainer.Add( pTarget->GetEntityID() );

#ifdef _DEBUG_
		mSeenPlayerContainer.Show( "map" );
#endif
	}
	else if( pTarget->IsCharacter() )
	{
		// 目前只加敌对关系npc
		if( this->IsHostileTo( pTarget ) )
			mSeenNpcContainer.Add( pTarget->GetEntityID() );
		else
			return false;

#ifdef _DEBUG_
		mSeenNpcContainer.Show( "map" );
#endif

	}
	else
	{
		return false;
	}

#ifdef _DEBUG_
	LOG_DEBUG( "map", "add %d(%d,%d) to Entity %d(%d,%d)", 
		pTarget->GetEntityID(), pTarget->GetPosX(), pTarget->GetPosY(),
		this->GetEntityID(), this->GetPosX(), this->GetPosY());
#endif

	// npc视野进入了一个实体
	if( mSeenPlayerContainer.mPlayers.size() + mSeenNpcContainer.mNpcCharactors.size() == 1 )
	{
		// 唤醒怪物活动
		CNpcModule::GetSingletonPtr()->OnSleep( this, false );
	}

	return true;
}

// ***************************************************************
//  Function:		将目标实体从可见表删除
//  Description:	DeleteViewEntity
//	Input:			pTarget - 目标
//	OutPut:			无
//	Return:			true - 删除成功； false - 没有删除
//	Others:
//  Date:			11/26/2008
// 
// ***************************************************************
bool CEntityNpc::DeleteViewEntity(CEntity *pTarget)
{
	LK_ASSERT( pTarget != NULL, return false );

	if( pTarget == this )
	{
		return false;
	}

	if(  pTarget->IsPlayer() )
	{
		mSeenPlayerContainer.Remove( pTarget->GetEntityID() );
	}
	else if( pTarget->IsCharacter() )
	{
		if( this->IsHostileTo( pTarget ) )
		{
			mSeenNpcContainer.Remove( pTarget->GetEntityID() );
		}
	}
	else
	{
		return false;
	}

#ifdef _DEBUG_
	LOG_DEBUG( "map", "delete %d from Entity %d", pTarget->GetEntityID(), this->GetEntityID());

	mSeenPlayerContainer.Show( "map" );
#endif

	// 视野没人
	if ( mSeenPlayerContainer.mPlayers.empty() && mSeenNpcContainer.mNpcCharactors.empty() )
	{
		// 让怪物休眠
		CNpcModule::GetSingletonPtr()->OnSleep( this, true  );
	}

	return true;
}


int CEntityNpc::ChangeNpcByLevel( unsigned int vLevel )
{
	((CPropertyCharacter*)(GetProperty()))->SetLevel( vLevel );
	( ( CPropertyNPC* )GetProperty( ) )->SetTranChangeLevel( vLevel );
	GetProperty()->Refresh( true );
	NotifyPropertyRefresh( );
	return 0;
}


void CEntityOgre::ExpForDie( CEntityCharacter* pSrcEntity )
{
	// 只有PLAYER杀怪,才计算经验  召唤兽干死的, 或者陷阱干死的也有经验
	if ( !pSrcEntity->IsPlayer() && !pSrcEntity->IsPet() && !pSrcEntity->IsPedal() )
	{
		return;
	}

	CPropertyCharacter* pDesProperty = (CPropertyCharacter*) GetProperty();

	// 取得主人实体，肯定是人
	CEntityPlayer* pSrcOwner = (CEntityPlayer*)pSrcEntity->GetOwner();
	if( pSrcOwner == NULL )
	{
		return;
	}

	CTemplateOgre* tpTempOgre = (CTemplateOgre*) CDataStatic::SearchTpl( pDesProperty->GetPropertyTempID( ) );
	if ( tpTempOgre == NULL )
	{
		LOG_DEBUG( "pro", "EntityDie:Ogre id = %d not found", pDesProperty->GetPropertyTempID( ) );
		return;
	}			

	// 这个tTeamList的EntityID实际是CharID
	CDamage tTeamList[ DAMAGELIMIT ];
	memset( tTeamList, 0, sizeof( CDamage ) * DAMAGELIMIT );
	int tTeamSize = 0;
	int tMaxDam = 0;
	int tMaxTeam =0;
	int tTotalDam = 0;
	CDamageList* tpDamageList = pDesProperty->GetDamageList( );

	// 维护一个杀手列表(肯定是在当前服务器)
	int tKillerList[ DAMAGELIMIT ] = { 0 };

	for ( int i = 0; i < tpDamageList->mNumber; i++ )
	{
		int tIndex = tTeamSize;
		CEntity* tpEntity = CSceneLogic::GetSingletonPtr()->GetEntity( tpDamageList->mDamage[ i ].mSrcEntityID );
		if ( tpEntity == NULL )
			continue;

		// 只有玩家，召唤兽，陷阱打死怪物才计算奖励(经验,....)
		if ( (!tpEntity->IsPlayer()) && (!tpEntity->IsPet()) && (!tpEntity->IsPedal()) )
		{
			continue;
		}		

		CEntityPlayer* tDamager = (CEntityPlayer*)tpEntity->GetOwner();
		if( tDamager == NULL )
		{
			continue;
		}

		if ( tDamager->IsAlive( ) == false )
			continue;
		bool tFind = false;
		int tTeamFlag = 0;	// 标示玩家队伍状态

		if ( CTeamModule::GetSingleton().InTeam( tDamager->GetCharID() ) == 0 )
			// 无队伍时为自身角色ID
			tTeamFlag = tDamager->GetCharID();
		else
			// 队伍中时为队伍中第一个当前服务器在线玩家charid
			tTeamFlag = CTeamModule::GetSingleton().GetLocalFirstOLMemberID( tDamager->GetCharID() );

		for ( int j = 0; j < tTeamSize; j++ ) 
		{
			if ( tTeamList[ j ].mSrcEntityID == tTeamFlag )
			{
				tIndex = j;
				tFind = true;
				break;
			}
		}
		if ( tFind == false )
			tTeamSize++;
		tKillerList[ tIndex ] = tDamager->GetCharID();
		tTeamList[ tIndex ].mSrcEntityID = tTeamFlag;
		tTeamList[ tIndex ].mDamageValue += tpDamageList->mDamage[ i ].mDamageValue;
		tTotalDam += tpDamageList->mDamage[ i ].mDamageValue;
		if ( tMaxDam < tTeamList[ tIndex ].mDamageValue )
		{
			tMaxDam = tTeamList[ tIndex ].mDamageValue;
			tMaxTeam = tTeamList[ tIndex ].mSrcEntityID;
		}
	}
	int tExp = tpTempOgre->mExp;
	CTemplateOgreTran* pTemplateOgreTran = (CTemplateOgreTran*) CDataStatic::SearchTpl( tpTempOgre->mTranID );
	if ( pTemplateOgreTran != NULL )
	{
		CPropertyCharacter* tpCharacter = ( CPropertyCharacter* )GetProperty( );
		int tChangeLevel = tpCharacter->GetTranChangeLevel( );
//		tExp = pTemplateOgreTran->mExp[ pDesProperty->GetLevel( ) ];
		tExp = pTemplateOgreTran->mExp[ tChangeLevel ];
	}
	for ( int i = 0; i < tTeamSize; i++ )
	{
		// 计算经验获取
		int	tFinalExp	= 0;
		// 获得组队成员列表
		// 为2B跨服改成CharID
		int tpMemberList[ TEAMNUM ] = { 0 };
		int tCount = 0;
		
		CEntityPlayer *pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tKillerList[ i ] );
		if( pPlayer == NULL)
		{
			continue;
		}
		CTeamModule::GetSingletonPtr()->OnGetMemberList( tTeamList[ i ].mSrcEntityID, tpMemberList, ARRAY_CNT(tpMemberList), tCount );
		
		tFinalExp = (int) ( tExp * pDesProperty->GetLastHPPer( ) * tTeamList[ i ].mDamageValue / (float)tTotalDam / 100.0 );

		// 玩家获得经验
		if ( tFinalExp > 0 )
		{
			CPropertyModule::GetSingleton( ).PlayerObtainExp( 
							pPlayer, tFinalExp, 0, tpMemberList, tCount, pDesProperty->GetLevel( ), this );

			/* 将杀怪获得经验记录在接口里面
			if ( nFinalExp > 0)
			{
				LogEventGetExpByEntityDie( pPlayer, tFinalExp, nFinalExp, tCount, tpTempOgre->mTempID);
			}
			*/
		}
	}

	DropForDie( pSrcOwner, tMaxTeam );
}



// 怪物死亡,掉落物品 ( 暂时只有OGRE用 )
void CEntityOgre::DropForDie( CEntityCharacter* pSrcEntity, int vMaxTeam )
{
	//CPropertyCharacter* pSrcProperty = (CPropertyCharacter*) pSrcEntity->GetProperty( );
	CPropertyCharacter* pDesProperty = (CPropertyCharacter*) GetProperty( );

	CTemplateOgre* tpTempOgre = (CTemplateOgre*) CDataStatic::SearchTpl( pDesProperty->GetPropertyTempID( ) );
	if ( tpTempOgre == NULL )
	{
		LOG_ERROR( "pro", "EntityDie:Ogre id = %d not found", pDesProperty->GetPropertyTempID( ) );
		return;
	}

	CEntity* tpOwnerEntity = pSrcEntity;

	// 首先计算归宿
	if ( tpTempOgre->mDropType == 1 ) 
	{
		// 如果不是BOSS那么属于第一个对该怪物造成伤害的玩家
		// TODO 如果FirstEntityID为召唤兽,则把归属给予主人
		CEntityCharacter* tpTmpEntity = (CEntityCharacter*) CSceneLogic::GetSingleton( ).GetEntity( pDesProperty->GetFirstEntityID( ) );
		if ( tpTmpEntity != NULL )
		{
			// 主人
			tpOwnerEntity = tpTmpEntity->GetOwner();
		}
	}

	if ( tpTempOgre->mDropType != 1 )
	{
		// 如果是BOSS
		tpOwnerEntity = CSceneLogic::GetSingleton( ).GetPlayerByCharID( vMaxTeam );
	}

	if ( tpOwnerEntity != NULL )
	{
		CPropertyModule::GetSingleton( ).DropItemByNpcDie( tpOwnerEntity, this, tpTempOgre );
	}
}

// 处理怪物死亡 ( 暂时只有OGRE,FUNCNPC,PET用 )
void CEntityOgre::TimeForDie( )
{
	CPropertyNPC* pDesProperty = (CPropertyNPC*) GetProperty( );

	// 如果是存活刷新的怪物，死后要删除存活刷新定时器
	if( GetOccurTime() > 0 )
	{
		GetTimerReciver()->ClearTimer(  TIMER_ENTITY_ALIVE_REFRESH );
	}

	pDesProperty->SetDisappearTimer( pDesProperty->GetDisapperaTime( ) );

	int nTimerOtherInfo[ TIMER_OTHER_INFO ] = {MESSAGE_PROPERTY,0,0,0};

	GetTimerReciver()->SetTimer( get_id(), TIMER_ENTITY_DISAPPER, pDesProperty->GetDisapperaTimer( ), 1, nTimerOtherInfo );

	// 在某一区域刷怪计数刷新,
	CCreatureRefresher::GetSingletonPtr()->OnNumericalRefreshEvent( this );


}



//////////////////////////////////////////////////////////////////////////
//		CEntityFuncNpc
//////////////////////////////////////////////////////////////////////////
bool CEntityFuncNpc::Setup( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos, bool bInit, int vNpcID )
{
	int			tNpcID = 0;
	CWTPoint	tPosition;
	EDirType	tDirection;
	// 如果是功能NPC
	if ( pCreator->GetCreatorType( ) == ENTITYTYPE_FUNCNPC )
	{
		CNpcCreator* pNpcCreator = (CNpcCreator*) pCreator;

		tNpcID		= pNpcCreator->mNpcID;
		tPosition	= pNpcCreator->mPosition;
		tDirection	= (EDirType) pNpcCreator->mDirection;
		// 如果有归属者  就把国家设置成和归属者一样
		if ( pNpcCreator->mOwnerCharID != 0 )
		{
			CEntityPlayer* tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pNpcCreator->mOwnerCharID );
			if (tpPlayer != NULL )
			{
				SetNationality( tpPlayer->GetNationality() );
			}
			else
			{
				LOG_ERROR("pro", "owner can't find? ownerid=%d", pNpcCreator->mOwnerCharID );
			}
		}
		mCreatorID = pNpcCreator->get_id();
		// 显示归属处理
		mIsShowOwner = pNpcCreator->mIsShowOwner;
		mOwnerCharID = pNpcCreator->mOwnerCharID;
	}
	else if ( pCreator->GetCreatorType( ) == ENTITYTYPE_OGRE )
	{
		//TODO: 这里一堆潜规则，Fuck 张3. 为了兼容，还是按照他的鸟方法继续写

		COgreCreator *tpCreator = CCreatureRefresher::GetSingleton().OnMutiMapRefreshEvent( (COgreCreator*)pCreator );
		COgreCreator *pOgreCreator = (COgreCreator*)pCreator;
		if ( tpCreator != NULL )
		{
			pOgreCreator = tpCreator;
		}

		tPosition = pOgreCreator->mPosition;
		tDirection = (EDirType)pOgreCreator->mDirection;
		mCreatorID = pCreator->get_id();
		int tRand = RAND(SERVER_PERCENT_INT);
		short tLow = 0;
		short tHigh = 0;

		// 根据配置的概率刷怪
		if ( vNpcID == 0 )
		{
			for ( int i = 0; i < pOgreCreator->mRCount1; i++ )
			{
				tLow += ( i == 0 ) ? 0 : pOgreCreator->mRefresh1[ i - 1 ].mOgreRand;
				tHigh += pOgreCreator->mRefresh1[ i ].mOgreRand;
				if ( tRand >= tLow && tRand <= tHigh )
					tNpcID = pOgreCreator->mRefresh1[ i ].mOgreID;
			}
		}
		else
		{
			tNpcID = vNpcID;
		}

		if ( tNpcID == 0 )
			return false;

		for ( int i = 0; i < pOgreCreator->mRCount2; i++ )
		{
			if ( tNpcID == pOgreCreator->mRefresh2[ i ].mOldID )
			{
				mOccurTime = pOgreCreator->mRefresh2[ i ].mTime;
				mOccurTimer = pOgreCreator->mRefresh2[ i ].mTime;
			}
		}
		
		// 显示归属处理
		mIsShowOwner = pOgreCreator->mIsShowOwner;
		mOwnerCharID = pOgreCreator->mOwnerCharID;
	}

	mEntityID = this->get_id( );
	// 都他妈的刷成NPC
	SetEntityType( ENTITYTYPE_FUNCNPC );
	mNpcID = tNpcID;
	mLineID =usLineID;
	mMapID = usMapID;
	mIndex = usMapIndex;
	mPos = tPosition;
	mSuperState = false;
	//mMoveable = true;
	//mControlable = true;
	mDirection = tDirection;
	mMotionState = MOTION_IDLE;
	mAliveState = ALIVE_NORMAL;
	mHasTarget = false;
	mOrginMapID = mMapID;
	mOrginPos = mPos;

	SetNationality( usLineID );	

	CTemplateNpc *tplNpc = (CTemplateNpc *)CDataStatic::SearchTpl( mNpcID );
	if ( tplNpc != NULL )
	{
		mPKType = tplNpc->mPKType;
		mGroupID = tplNpc->mGroupID;
		if ( tplNpc->mBidDay != 0 || tplNpc->mCorpsBattle != 0 )
		{
			CActivityModule::GetSingletonPtr()->AddNpcInfo( mNpcID, mEntityID );
		}
		if ( tplNpc->mLmtLineID != 0 && tplNpc->mLmtLineID != usLineID )
		{
			return false;
		}	
		mOwnCampID = tplNpc->mWarCamp;
	}
	else
	{
		mPKType = 0;
		mGroupID = 0;
	}

	GetProperty()->SetEntityID( this->get_id( ) );
	GetProperty()->SetPropertyTempID( mNpcID );
	GetProperty()->Refresh( true );
	//TODO: 这里不用刷新，因为就是刚刚创建的，中间没有改变，等有改变了再掉刷新也不迟啊
	//NotifyPropertyRefresh( );

	// 记录创建时间点
	mCreateTime = LINEKONG::GetTickCount();	
	pCreator->mHasOgre = GetEntityID();

	return true;
}


// 处理NPC死亡
void CEntityFuncNpc::TimeForDie( )
{
	CPropertyNPC* pDesProperty = (CPropertyNPC*) GetProperty( );
	pDesProperty->SetDisappearTimer( pDesProperty->GetDisapperaTime( ) );
	int nTimerOtherInfo[ TIMER_OTHER_INFO ] = {MESSAGE_PROPERTY,0,0,0};
	GetTimerReciver()->SetTimer( get_id(), TIMER_ENTITY_DISAPPER, pDesProperty->GetDisapperaTimer( ), 1, nTimerOtherInfo );
	if ( mNpcChangeState == 1 ) // 如果是变怪NPC 记日志
	{
		LOG_NOTICE("default", "***ChangedNpcDie1 NPCID=%d entityid=%d", GetNpcID(), GetEntityID() );
	}
	// 在某一区域刷怪计数刷新,
	CCreatureRefresher::GetSingletonPtr()->OnNumericalRefreshEvent( this );
}



//////////////////////////////////////////////////////////////////////////
//		CEntityPet
//////////////////////////////////////////////////////////////////////////
bool CEntityPet::IsHostileTo( CEntity* pEntity )
{
	LK_ASSERT( pEntity != NULL, return false );

	// 如果是自己
	if( this == pEntity )
	{
		return false;
	}

	bool bHostile = true;

	CEntityPlayer* pSrcOwner = (CEntityPlayer*) GetOwner();
	if ( pSrcOwner == NULL )
	{
		bHostile = false;
	}
	else
	{
		bHostile = pSrcOwner->IsHostileTo( pEntity );
	}

	return bHostile;
}


// 是否友好
bool CEntityPet::IsFriendlyTo( CEntity* pEntity )
{
	LK_ASSERT( pEntity != NULL, return false );

	// 如果是自己
	if( this == pEntity )
	{
		return true;
	}

	return ! IsHostileTo( pEntity );
}

// 是否队友
bool CEntityPet::IsTeamTo(CEntity *pEntity)
{
	return false;
}

// 是否对玩家实体友好
bool CEntityPet::IsHostileToPlayers()
{
	return true;
}

// 主人ID
unsigned int CEntityPet::GetOwnerID()
{
	// 返回召唤者ID
	return mOwnerID;
}

// 组队状态
int CEntityPet::GetTeamFlag( )
{
	CEntityPlayer* pOwner = (CEntityPlayer*) CSceneObjManager::GetSingletonPtr()->GetObject( mOwnerID );
	if( pOwner != NULL )
	{
		return pOwner->GetTeamFlag();
	}
	else
	{
		return TEAM_NONE;
	}
}


bool CEntityPet::Setup(unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos, bool bInit, int vNpcID )
{
	// 如果是召唤兽
	if ( pCreator->GetCreatorType( ) != ENTITYTYPE_PET )
	{
		return false;
	}
	CPetCreator* tpPetCreator = (CPetCreator*)pCreator;
	int tPosX = tpPetCreator->mPos.mX;
	int tPosY = tpPetCreator->mPos.mY;
	EDirType tDirection = tpPetCreator->mDirection;
	int tNpcID = tpPetCreator->mPetID;

	mEntityID = this->get_id( );
	SetEntityType( ENTITYTYPE_PET );
	mNpcID = tNpcID;

	mLineID = usLineID;
	mMapID = usMapID;
	mIndex = usMapIndex;
	mPos = CWTPoint( tPosX, tPosY );
	mSuperState = false;
	mDirection = tDirection;
	mMotionState = MOTION_IDLE;
	mAliveState = ALIVE_NORMAL;
	mHasTarget = false;
	mOrginPos = mPos;
	mOrginMapID = mMapID;
	mCreatorID= tpPetCreator->get_id();

	mOwnerID = tpPetCreator->mSrcEntityID;
	mOnlineTime = tpPetCreator->mOnlineTime;
	mCurMode = tpPetCreator->mPetMode;
	mCurTargetID = 0;
	
	CPropertyPet* pProPet = ( CPropertyPet* ) GetProperty();

	pProPet->SetEntityID( this->get_id( ) );
	pProPet->SetPropertyTempID( mNpcID );
	
	SetNationality(usLineID);

	// 设置召唤兽等级为主人等级
	CEntity* pOwnerEntity = GetOwner();
	if( (pOwnerEntity != NULL) && pOwnerEntity->IsPlayer() )
	{	
		CPropertyPlayer* pPlayerPro = (CPropertyPlayer*) pOwnerEntity->GetProperty();
		pProPet->SetLevel( pPlayerPro->GetLevel() );
	}

	// 记录创建时间点
	if( mOnlineTime == 0 )
	{
		mCreateTime = LINEKONG::GetTickCount();
	}
	else
	{
		mCreateTime = std::max( (int)(LINEKONG::GetTickCount() - mOnlineTime), 0 );
		//如果是跨服过来的，立即做升级处理
		CPropertyModule::GetSingletonPtr()->SummonBeastLevelUp( this, 0, false );
	}

	pProPet->Refresh( true );

	return true;
}

void CEntityPet::PetLogicForDie( )
{
	CEntityCharacter* tpOwner = (CEntityCharacter*) GetOwner( );
	if ( tpOwner != NULL )
	{
		CPropertyCharacter* tpProOwner = (CPropertyCharacter*) tpOwner->GetProperty( );
		tpProOwner->SetPetID( 0 );
	}
}


//////////////////////////////////////////////////////////////////////////
//		CEntityPedal
//////////////////////////////////////////////////////////////////////////

bool CEntityPedal::IsHostileTo( CEntity* pEntity )
{
	LK_ASSERT( pEntity != NULL, return false );

	// 如果是自己
	if( this == pEntity )
	{
		return false;
	}

	bool bHostile = true;

	CEntityPlayer* pSrcOwner = (CEntityPlayer*) GetOwner();
	if ( pSrcOwner == NULL )
	{
		bHostile = false;
	}
	else
	{
		bHostile = pSrcOwner->IsHostileTo( pEntity );
	}

	return bHostile;
}


// 是否友好
bool CEntityPedal::IsFriendlyTo( CEntity* pEntity )
{
	LK_ASSERT( pEntity != NULL, return false );

	// 如果是自己
	if( this == pEntity )
	{
		return true;
	}

	return ! IsHostileTo( pEntity );
}

// 是否队友
bool CEntityPedal::IsTeamTo(CEntity *pEntity)
{
	return false;
}

// 是否对玩家实体敌对
bool CEntityPedal::IsHostileToPlayers()
{
	return true;
}

// 属主实体
unsigned int CEntityPedal::GetOwnerID()
{
	if( mPedalType == PEDAL_TELEPORT )
	{
		return this->GetEntityID();
	}
	else
	{
		// 返回释放者ID
		return mOwnerID;
	}
}

bool CEntityPedal::Setup(unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator *pCreator , CWTPoint* pPos, bool bInit, int vNpcID )
{
	if ( pCreator->GetCreatorType( ) != ENTITYTYPE_PEDAL )
	{
		return false;
	}

	CPedalCreator* tpPedalCreator = (CPedalCreator*) pCreator;
	int tPosX = tpPedalCreator->mSrcRect.CenterPoint( ).mX;
	int tPosY = tpPedalCreator->mSrcRect.CenterPoint( ).mY;

	mEntityID = get_id( );
	SetEntityType( ENTITYTYPE_PEDAL );
	mNpcID = tpPedalCreator->mTempID;
	mLineID = usLineID;
	mMapID = usMapID;
	mIndex = usMapIndex;
	mPos = CWTPoint( tPosX, tPosY );
	//	mSuperState = false;
	//	mMoveable = true;
	//	mControlable = true;
	mDirection = DIR_DOWN;
	//	mMotionState = MOTION_IDLE;
	mAliveState = ALIVE_NORMAL;
	//	mHasTarget = false;
	//	mOrginPos = mPos;
	mCreatorID= tpPedalCreator->get_id();
	
	// 神符是所有人可见
	

	SetOwnerID( tpPedalCreator->mSrcEntityID );
	SetSkillID( tpPedalCreator->mSkillID );
	SetSkillLevel( tpPedalCreator->mSkillLevel );
	SetTeamID( tpPedalCreator->mTeamID );
	SetPedalType( tpPedalCreator->mPedalType );
	
	SetSrcRect( tpPedalCreator->mSrcRect );
	SetActRect( tpPedalCreator->mActRect );

	if ( IsHierogram( ) == true )
	{
		mVisibility = VISIBLITY_ALL;
	}
	else if( IsTeleport( ) == true )
	{
		mVisibility = VISIBLITY_ALL;
	}
	else if( IsTrap( ) == true )
	{
		mVisibility = VISIBLITY_TEAM;
	}
	else if( IsNeedFire( ) == true )
	{
		mVisibility = VISIBLITY_ALL;
	}
	

	// 对地图边界进行判断
	CMapObject* pMapObj = CMapObjManager::GetSingletonPtr()->GetMapObject(mLineID,mMapID,mIndex);
	if( pMapObj != NULL )
	{
		mSrcRect.mTopLeft.mX = std::max( mSrcRect.mTopLeft.mX, 0 );
		mSrcRect.mTopLeft.mY = std::max( mSrcRect.mTopLeft.mY, 0 );
		mSrcRect.mBottomRight.mX = std::min( mSrcRect.mBottomRight.mX, (int) pMapObj->GetWidth() );
		mSrcRect.mBottomRight.mY = std::min( mSrcRect.mBottomRight.mY, (int) pMapObj->GetHight() );
	}

	SetDstLineID( tpPedalCreator->mLineID );
	SetDstMapID( tpPedalCreator->mMapID );

	if( tpPedalCreator->mPedalType == PEDAL_TELEPORT )
	{
		SetPresistTime( 0 );
		SetSkillID( 0 );
		SetSkillLevel( 0 );
		SetMinLevel( 0 );
		SetRestrictNation( 0 );
		SetDesPos( tpPedalCreator->mDesPos );
	}
	else if( tpPedalCreator->mPedalType == PEDAL_TRAP )
	{
		// 陷阱
		CTemplateTrapSkill* tpTrapTpl = (CTemplateTrapSkill*) CDataStatic::SearchSkill( tpPedalCreator->mSkillID, tpPedalCreator->mSkillLevel );
		if( tpTrapTpl == NULL )
		{
			LOG_ERROR( "pro", "[%s:%d][%s] skill template id = %d ,level = %d not found", 
					__LK_FILE__, __LINE__, __FUNCTION__, tpPedalCreator->mSkillID, tpPedalCreator->mSkillLevel );
		}
		else
		{
			SetPresistTime( tpTrapTpl->mLastTime );
			SetSkillID( tpPedalCreator->mSkillID );
			SetSkillLevel( tpPedalCreator->mSkillLevel );
			SetMinLevel( tpPedalCreator->mMinLevel );
			SetSrcEntityID( tpPedalCreator->mSrcEntityID ); 
		}
	}
	else if( tpPedalCreator->mPedalType == PEDAL_HIEROGRAM )
	{
		//神符
		CTplHierogram* tpTpl = ( CTplHierogram* )CDataStatic::SearchTpl( tpPedalCreator->mTempID );
		LK_ASSERT_STR( tpTpl != NULL, return false, "TempID %d", tpPedalCreator->mTempID );
		SetPresistTime( tpTpl->mAlive * 1000 );
		SetSrcEntityID( tpPedalCreator->mSrcEntityID );
	}
	else if(tpPedalCreator->mPedalType == PEDAL_NEEDFIRE )
	{
		// 陷阱
		CTemplateTrapSkill* tpTrapTpl = (CTemplateTrapSkill*) CDataStatic::SearchTpl( tpPedalCreator->mTempID );
		if( tpTrapTpl == NULL )
		{
			LOG_ERROR( "pro", "[%s:%d][%s] skill template id = %d ,level = %d not found", 
				__LK_FILE__, __LINE__, __FUNCTION__, tpPedalCreator->mSkillID, tpPedalCreator->mSkillLevel );
		}
		else
		{
			SetPresistTime( tpTrapTpl->mLastTime );
			SetSkillID( tpPedalCreator->mSkillID );
			SetSkillLevel( tpPedalCreator->mSkillLevel );
			SetMinLevel( tpPedalCreator->mMinLevel );
			SetSrcEntityID( tpPedalCreator->mSrcEntityID ); 
		}
	}

	GetProperty()->SetEntityID( this->get_id( ) );
	GetProperty( )->SetPropertyTempID( tpPedalCreator->mTempID );

	tpPedalCreator->mHasOgre = GetEntityID();
	//CSceneLogic::GetSingleton( ).CreateEntity( tpNpc, pNpcCreator );

	return true;
}



// ***************************************************************
//  Function:		IsVisibleFor 
//  Description:	是否对目标玩家可见
//	Input:			pTargetPlayer - 目标玩家
//	OutPut:			无
//	Return:			无
//	Others:			
//  Date:			11/13/2008
// 
// ***************************************************************
bool CEntityPedal::IsVisibleFor( CEntity* pTarget )
{
	LK_ASSERT( pTarget != NULL, return false );

	if( mVisibility == VISIBLITY_SELF )
	{
		return false;
	}
	else if ( mVisibility == VISIBLITY_TEAM )
	{
		CEntityPlayer* pOwner = (CEntityPlayer*) GetOwner();
		LK_ASSERT( pOwner != NULL, return false );

		// 对主人可见
		if( pOwner == pTarget )
		{
			return true;
		}

		return ( pOwner->IsTeamTo( pTarget ) );
	}
	else 
	{
		return true;
	}
}

//////////////////////////////////////////////////////////////////////////
//		CEntityDrop
//////////////////////////////////////////////////////////////////////////

bool CEntityDrop::IsHostileTo( CEntity* pEntity )
{
	return false;
}


// 是否友好
bool CEntityDrop::IsFriendlyTo( CEntity* pEntity )
{
	return true;
}

// 是否对玩家实体敌对
bool CEntityDrop::IsHostileToPlayers()
{
	return false;
}

// 是否队友
bool CEntityDrop::IsTeamTo(CEntity *pEntity)
{
	return false;
}

// 主人ID
unsigned int CEntityDrop::GetOwnerID()
{
	return GetEntityID();
}






//////////////////////////////////////////////////////////////////////////
//		CEntityMoney
//////////////////////////////////////////////////////////////////////////
bool CEntityMoney::Setup(unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos, bool bInit, int vNpcID )
{
	// 如果不是金钱
	if ( pCreator->GetCreatorType( ) != ENTITYTYPE_MONEY )
	{
		return false;
	}

	CMoneyCreator* tpDropCreator = (CMoneyCreator*)pCreator;

	CWTPoint tOrginPos = CWTPoint( tpDropCreator->mCenterX, tpDropCreator->mCenterY );

	// 如果是被动掉落，要算一个随机位置
	if ( tpDropCreator->mDropType == DROP_OBJECTIVE )
	{
		CMapModule::GetSingletonPtr()->GetDropPos( usLineID, usMapID, usMapIndex, tOrginPos );
	}
	//TODO: 暂时没有，等待客户端上报. 如果是主动掉落，判断掉落的位置
	//else if ( tpDropCreator->mDropType == DROP_SUBJECTIVE )
	//{
	//	if ( CMapModule::GetSingletonPtr()->IsStaticBlock( usLineID, usMapID, usMapIndex, tOrginPos ) > 0 )
	//	{
	//		return false;
	//	}
	//	else if ( CMapModule::GetSingletonPtr()->IsOccupied( usLineID, usMapID, usMapIndex, tOrginPos, ENTITYTYPE_MONEY ) )
	//	{
	//		return false;
	//	}
	//}

	mEntityID = this->get_id( );
	SetEntityType( ENTITYTYPE_MONEY );
	mNpcID = 0;

	mLineID = usLineID;
	mMapID = usMapID;
	mIndex = usMapIndex;
	mPos = tOrginPos;
	//	mSuperState = false;
	//	mMoveable = true;
	//	mControlable = true;
	mDirection = DIR_DOWN;
	//	mMotionState = MOTION_IDLE;
	mAliveState = ALIVE_NORMAL;
	//	mHasTarget = false;
	//	mOrginPos = mPos;
	//	mCreatorID= tpDropCreator->get_id();

	//CSceneLogic::GetSingleton( ).CreateEntity( tpNpc, pNpcCreator );
	GetProperty()->SetEntityID( this->get_id( ) );
	CPropertyMoney *pMoney = (CPropertyMoney *)( GetProperty() );
	pMoney->SetMoney( tpDropCreator->mMoney );

	return true;
}


//////////////////////////////////////////////////////////////////////////
//		CEntityItem
//////////////////////////////////////////////////////////////////////////
bool CEntityItem::Setup(unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos, bool bInit, int vNpcID )
{
	if ( pCreator->GetCreatorType( ) != ENTITYTYPE_ITEM )
	{
		return false;
	}

	CItemCreator* pItemCreator = (CItemCreator*)pCreator;

	CWTPoint tOrginPos = CWTPoint( pItemCreator->mCenterX, pItemCreator->mCenterY );

	// 如果是被动掉落，要算一个随机位置
	if ( pItemCreator->mDropType == DROP_OBJECTIVE )
	{
		CMapModule::GetSingletonPtr()->GetDropPos( usLineID, usMapID, usMapIndex, tOrginPos );
	}
	//TODO: 暂时没有，等待客户端上报。 如果是主动掉落，判断掉落的位置
	//else if ( pItemCreator->mDropType == DROP_SUBJECTIVE )
	//{
	//	if ( CMapModule::GetSingletonPtr()->IsStaticBlock( usLineID, usMapID, usMapIndex, tOrginPos ) > 0 )
	//	{
	//		return false;
	//	}
	//	else if ( CMapModule::GetSingletonPtr()->IsOccupied( usLineID, usMapID, usMapIndex, tOrginPos, ENTITYTYPE_ITEM ) )
	//	{
	//		return false;
	//	}
	//}

	mEntityID = this->get_id( );
	SetEntityType( ENTITYTYPE_ITEM );
	mNpcID = pItemCreator->mItemID;
	mLineID = usLineID;
	mMapID = usMapID;
	mIndex = usMapIndex;
	mPos = tOrginPos;
	mDirection = DIR_DOWN;
	mAliveState = ALIVE_NORMAL;
	mProperty.SetLevel( pItemCreator->mLevel );

	GetProperty()->SetEntityID( this->get_id( ) );

	pItemCreator->mHasOgre = GetEntityID();
	//CSceneLogic::GetSingleton( ).CreateEntity( tpNpc, pNpcCreator );
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
//		CEntityCollect
//////////////////////////////////////////////////////////////////////////
bool CEntityCollect::Setup(unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator , CWTPoint* pPos, bool bInit, int vNpcID )
{
	if ( pCreator->GetCreatorType( ) != ENTITYTYPE_COLLECT )
	{
		return false;
	}

	CCollectCreator* pCollectCreator = (CCollectCreator*)pCreator;

	mEntityID = this->get_id( );
	mCreatorID = pCollectCreator->get_id( );
	SetEntityType( ENTITYTYPE_COLLECT );
	mNpcID = pCollectCreator->mNpcID;
	mLineID = usLineID;
	mMapID = usMapID;
	mIndex = usMapIndex;
	mPos = pCollectCreator->mPosition[ RAND( pCollectCreator->mPCount ) ];
	mOrginPos = mPos;
	mOrginMapID = mMapID;
	mDirection = DIR_DOWN;
	mAliveState = ALIVE_NORMAL;
	
	SetNationality(usLineID);
	// 写死
	mPKType = ( FRIEND_WITH_SC | FRIEND_TO_SC | FRIEND_WITH_FC | FRIEND_TO_FC );

	GetProperty()->SetEntityID( this->get_id( ) );

	//CSceneLogic::GetSingleton( ).CreateEntity( tpNpc, pNpcCreator );
	
	pCollectCreator->mHasOgre = GetEntityID();

	return true;
}



int CEntityCharacter::AddAureoEntityList( CEntityCharacter* pEntity )
{
	if ( pEntity == NULL )
		return -1;

	for ( unsigned int i = 0; i < ARRAY_CNT( mEntityAureoList ); i++ )
	{
		if ( mEntityAureoList[ i ] == NULL )
		{
			mEntityAureoList[ i ] = pEntity;
			return i;
		}

		if ( mEntityAureoList[ i ] == pEntity )
		{
			return i;
		}
	}

	return -1;
}

void CEntityCharacter::ClearAureoEntityList( )
{
	memset( mEntityAureoList, 0, sizeof( mEntityAureoList ) );
}

void CEntityCharacter::RemoveAureoEntityList( CEntityCharacter* pEntity )
{
	int tIndex = -1;
	int tArrayCnt = ARRAY_CNT( mEntityAureoList );
	for ( int i = 0; i < tArrayCnt; i++ )
	{
		if ( mEntityAureoList[ i ] == NULL )
			return;

		if ( mEntityAureoList[ i ] == pEntity )
		{
			tIndex = i;
			break;
		}
	}
	
	// 如果删除的实体在数组的最后一位,则不处理,否则把后面的数据往前移动
	if ( tIndex >= 0 && tIndex < ( tArrayCnt - 1 - 1 ) )
	{
		memcpy( &( mEntityAureoList[ tIndex ] ), &( mEntityAureoList[ tIndex + 1 ] ), sizeof( mEntityAureoList[ 0 ] ) * ( tArrayCnt - tIndex - 1 ) );
	}
}

void CEntityCharacter::CheckAureoEntityList( CEntityCharacter** pEntityList, int vEntityCount, CEntityCharacter** pAddEntityList, int& vAddEntityCount, CEntityCharacter** pRemoveEntityList, int & vRemoveEntityCount )
{
	if ( pEntityList == NULL || pAddEntityList == NULL || pRemoveEntityList == NULL )
		return;

	CEntityCharacter* tpTempEntityList[ TEAMNUM ];
	memcpy( tpTempEntityList, mEntityAureoList, sizeof( tpTempEntityList ) );
	
	ClearAureoEntityList();
	
	vAddEntityCount = 0;
	vRemoveEntityCount = 0;

	for ( int i = 0; i < vEntityCount && i < (int)ARRAY_CNT( pEntityList ); i++ )
	{
		if ( pEntityList[ i ] == NULL )
			break;

		bool bIsInsert = false;
		for ( unsigned int j = 0; j < ARRAY_CNT( tpTempEntityList ); j++ )
		{
			if ( tpTempEntityList[ j ] == NULL )
				break;

			if ( pEntityList[ i ] == tpTempEntityList[ j ] )
			{
				AddAureoEntityList( pEntityList[ i ] );
				bIsInsert = true;
				break;
			}
		}

		if ( bIsInsert == false )
		{
			AddAureoEntityList( pEntityList[ i ] );
			pAddEntityList[ vAddEntityCount ] = pEntityList[ i ];
			vAddEntityCount++;
		}
	}
	
	for ( unsigned int i = 0; i < ARRAY_CNT( tpTempEntityList ); i++ )
	{
		if ( tpTempEntityList[ i ] == NULL )
			break;

		bool bIsSame = false;
		for ( unsigned int j = 0; j < ARRAY_CNT( mEntityAureoList ); j++ )
		{
			if ( mEntityAureoList[ j ] == NULL )
				break;

			if ( tpTempEntityList[ i ] == mEntityAureoList[ j ] )
			{
				bIsSame = true;
				break;
			}
		}

		if ( bIsSame == false )
		{
			pRemoveEntityList[ vRemoveEntityCount ] = tpTempEntityList[ i ];
			vRemoveEntityCount++;
		}
	}
}


// 通知怪物死亡 ( CHARACTER使用 )
void CEntityCharacter::NotifyDeath( CEntityCharacter* pSrcEntity )
{
	unsigned short tDead = 0;
	unsigned int tEntityID = 0;
	if ( pSrcEntity != NULL && ! pSrcEntity->IsPlayer() )
	{
		tEntityID = pSrcEntity->GetEntityID( );
		tDead = 0;
	}
	else if ( ! IsPlayer() )
	{
		tEntityID = GetEntityID( );
		tDead = 1;
	}

	if ( tEntityID != 0 )
		CNpcModule::GetSingleton( ).OnDead( tEntityID, tDead );
}

// 处理杀怪取血 ( CHARACTER使用 )
void CEntityCharacter::KillerRenew( CEntityCharacter* pSrcEntity )
{
	if ( pSrcEntity != NULL )
	{
		CPropertyCharacter* pSrcProperty = (CPropertyCharacter*) pSrcEntity->GetProperty( );

		if ( pSrcProperty->GetStateParam( STATE_KILLERRENEW ).mInState == true )
		{
			CPropertyCharacter* pDesProperty = (CPropertyCharacter*)GetProperty( );
			unsigned int tDesMaxHP = pDesProperty->GetMaxHP( );

			unsigned int tHP = (unsigned int) ( tDesMaxHP * pSrcProperty->GetStateParam( STATE_KILLERRENEW ).mParames1 / SERVER_PERCENT_FLOAT );
			//unsigned int tOldHP = pSrcProperty->GetCurHP( );

			tHP = pSrcEntity->CureHP( tHP, pSrcEntity->GetEntityID(), RESULT_STEALHP );
			//pSrcProperty->SetCurHP( min( ( tOldHP + tHP ), (unsigned int)( pSrcProperty->GetMaxHP( ) ) ) );

			//tHP = pSrcProperty->GetCurHP() - tOldHP;

			//pSrcEntity->AddFunResult( GetEntityID( ), FUNC_INCHP, RESULT_STEALHP, tHP, pSrcProperty->GetCurHP( ), pSrcProperty->GetMaxHP( ) );
		}
	}
}

// 实体添加BUFF
int CEntityCharacter::EntityInsertBuff( CEntityCharacter* pSrcEntity, CTemplateBuff* pBuffTpl, int nSkillType, int nDamage, int nBuffTime )
{
	if ( pSrcEntity == NULL || pBuffTpl == NULL )
		return ERR_INVALID_PARAMETER;

	CPropertyCharacter* pDesProperty = (CPropertyCharacter*) GetProperty();

	CBuffList *tpBuff = pDesProperty->GetBuffList( );

	CMapObject* tpMapObj = GetMapObject( );
	LK_ASSERT_STR( tpMapObj != NULL, return ERR_UNKNOWN, "Map %d, LineID %d, MapIndex %d", GetMapID( ), GetLineID( ),
		GetMapIndex( ) );

	if( CheckBuffCondition( pBuffTpl, tpMapObj ) == false )
	{
		return ERROR_USESKILL_INSERTBUFF_DISSATISFY;
	}

	// 如果是负面BUFF,则不能对免疫玩家使用
	if ( pBuffTpl->mBuffType == CTemplateBuff::BUFF_DEC && pDesProperty->GetStateParam( STATE_IMMUNITYREVERSD ).mInState == true )
	{
		AddFunResult( pSrcEntity->GetEntityID(), FUNC_NONE, RESULT_IMMUNITY );
		return SUCCESS;
	}

	// 如果概率低，则不能添加
	if ( pBuffTpl->mAccurate != 0 && pBuffTpl->mAccurate < RAND(SERVER_PERCENT_INT) )
	{
		return SUCCESS;
	}
	

	// 如果Buff使目标进入战斗
	if ( pBuffTpl->mEnableBattle == 1 )
	{
		// 重置战斗计时器
		if( pSrcEntity->GetAliveStatus( ) == ALIVE_NORMAL )
		{
			pSrcEntity->SetAliveStatus( ALIVE_BATTLE );
			pSrcEntity->AddFunResult( 0, FUNC_BATTLE, RESULT_NORMAL );
		}

		if( GetAliveStatus( ) == ALIVE_NORMAL )
		{
			SetAliveStatus( ALIVE_BATTLE );
			AddFunResult( 0, FUNC_BATTLE, RESULT_NORMAL );

			// 清空伤害列表 有问题
			CDamageList *tpDamageList = pDesProperty->GetDamageList( );
			tpDamageList->ClearDamage( );
		}
	}

	// 如果是负面BUFF
	if ( pBuffTpl->mBuffType == CTemplateBuff::BUFF_DEC )
	{
		// 如果目标为NPC，那么要通知NPC模块处理NPC策略
		if ( IsNpc() )
		{
			// 通知NPC模块，NPC被攻击了
			CNpcModule::GetSingleton().OnAttacked( pSrcEntity, this, 1 );
			// 把NPC加入人的伤害列表
			CPropertyModule::GetSingleton().EntityInsertDamageList( this, pSrcEntity );
		}
	}	

	unsigned int tLastBuffID = 0;
	int nHorseID = 0;
	if ( this->IsPlayer() )
	{
		CPropertyPlayer* pPro = (CPropertyPlayer*)this->GetProperty();
		nHorseID = pPro->GetHorseTempID();
	}
	// 如果该Buff持续时间大于0, 那么添加一个Buff
	if ( pBuffTpl->mLastTime > 0 || pBuffTpl->mLastTime == -1 )
	{				
		int	tListID = 0;
		int tResultCode = 0;
		

		CBuffObject* tpBuffObj = tpBuff->InsertBuff( pBuffTpl->mTempID, (nBuffTime>0?nBuffTime:pBuffTpl->mLastTime), pSrcEntity, tLastBuffID, nDamage , this, &tListID, &tResultCode );
		if( tpBuffObj == NULL )
		{
			if( tResultCode != ERROR_USESKILL_INSERTBUFF_LARGE )
			{
				//TODO: 在可叠加buff设上限之前（小于最大buff数），这里先关闭不写日志
#ifdef _DEBUG_
				LOG_ERROR( "pro", "[ %s : %d ] [ %s ] InsertBuff error, Entity %d,NpcID %d,BuffID %d, BuffCount %d, ErrCode %d",
					__LK_FILE__, __LINE__, __FUNCTION__, GetEntityID( ),GetNpcID(), pBuffTpl->mTempID, tpBuff->mBuffNumber, 
					tResultCode );
#endif
			}
			if ( tResultCode == ERROR_USESKILL_INSERTBUFF_REMOVECOUNT )
			{
				AddFunResult( 0, FUNC_REMOVEBUFF, RESULT_NORMAL, true, pBuffTpl->mRemoveCountID, tListID );
				tResultCode = SUCCESS;
			}
			return tResultCode;
		}

		if ( tLastBuffID != 0 )
		{
			// 如果是经验BUFF,那么不给客户端发删除BUFF的包
			if( pBuffTpl->mBuffSuperposition == CTemplateBuff::NONE_SUPERPOSITION )
			{
				AddFunResult( 0, FUNC_REMOVEBUFF, RESULT_NORMAL, true, tLastBuffID, tListID );

				AddFunResult( pSrcEntity->GetEntityID(), FUNC_SYNCHPMP, RESULT_NONE, true, pDesProperty->GetCurHP( ), pDesProperty->GetMaxHP( ), pDesProperty->GetCurMP( ), pDesProperty->GetMaxMP( ) );
				// 需要同步清楚状态
				CTemplateBuff* tpLastBuffTpl = (CTemplateBuff*) CDataStatic::SearchTpl( tLastBuffID );
				if ( tpLastBuffTpl != NULL )
				{
					int nTempOne = 0;
					int nTempTwo = 0;
					RemoveBuffState( tpLastBuffTpl, nTempOne, nTempTwo );
				}
			}

			// 清除方寸山buff变身状态
			int nFCSBuffID = tpBuff->HasTransform();
			if( (unsigned int)nFCSBuffID == tLastBuffID)
			{
				tpBuff->SetTransform( NULL, nFCSBuffID );
				CPropertyModule::GetSingleton().SendPlayerTransformNotify( (CEntityPlayer*)this );
				NotifyPropertyRefresh( );
			}
		}

		// 一定要先插如buff. 因为后面玩家可能会死亡
		AddFunResult( pSrcEntity->GetEntityID(), FUNC_INSERTBUFF, RESULT_NORMAL, true, pBuffTpl->mTempID, tpBuffObj->mListID, tpBuffObj->mPersistTime );
	
		AddFunResult( pSrcEntity->GetEntityID(), FUNC_SYNCHPMP, RESULT_NONE, true, pDesProperty->GetCurHP( ), pDesProperty->GetMaxHP( ), pDesProperty->GetCurMP( ), pDesProperty->GetMaxMP( ) );


		if( tResultCode == SUCCESS )
		{
			InsertBuffState( pSrcEntity, pBuffTpl, tpBuffObj, nSkillType, nDamage );
		}

		// 立即作用
		if( pBuffTpl->mBuffFirstActionTime == 0 )
		{
			BuffTimeEvent( pBuffTpl, tpBuffObj/*&tpBuff->GetBuffObjectByIndex( tBuffInsertRet )*/ );
		}

		LOG_INFO( "pro", "Entity( %d ) insertBuff, BuffID = %d", pSrcEntity->GetEntityID( ), pBuffTpl->mTempID );

		// 如果产生了BUFF互斥,给客户端发删除之前BUFF的包


	}
	else
	{
		InsertBuffState( pSrcEntity, pBuffTpl, NULL, nSkillType, nDamage );
	}

	// 获得未改变前的最大HP,最大MP
	unsigned int tnOldMaxHP = pDesProperty->GetMaxHP( );
	unsigned int tnOldMaxMP = pDesProperty->GetMaxMP( );

	// 一定要先插如buff. 因为后面玩家可能会死亡
	//AddFunResult( pSrcEntity->GetEntityID(), FUNC_INSERTBUFF, RESULT_NORMAL, pBuffTpl->mTempID, pDesProperty->GetCurHP( ), pDesProperty->GetMaxHP( ), pDesProperty->GetCurMP( ), pDesProperty->GetMaxMP( ) );

	

	// TODO: BUFF可能改变HP,MP上限,刷新一次
	pDesProperty->Refresh( );
	
	// 在NPC身上添加Buff的话不用触发刷新消息，怪物没有属性，只有玩家才触发
	if( this->IsPlayer() )
	{
		// 如果复制变身
		if ( pBuffTpl->mTransform != 0 )
		{	
			// 如果之前有变身，先取消
			if( tpBuff->HasTransform() )
			{
				tpBuff->SetTransform( NULL, pBuffTpl->mTempID );
				CPropertyModule::GetSingleton().SendPlayerTransformNotify( (CEntityPlayer*)this );
				NotifyPropertyRefresh( );
			}

			tpBuff->SetTransform( pSrcEntity, pBuffTpl->mTempID );
			CPropertyModule::GetSingleton().SendPlayerTransformNotify( (CEntityPlayer*)this );
		}
		NotifyPropertyRefresh( );
		// 通知任务模块
		CTaskModule::GetSingleton().FireEvent( (CEntityPlayer*)this, EVENT_BUFFCHANGED, pBuffTpl->mTempID );		

		// 如果在坐骑上 且正要取消这个BUFF 则要判断能不能下马
		if ( nHorseID != 0 )
		{
			CTplItemHorse* tpTplHorse = (CTplItemHorse*)CDataStatic::SearchTpl(nHorseID);
			if (tpTplHorse != NULL)
			{
				if ( tLastBuffID == (unsigned int)tpTplHorse->mSpeedBuff )
				{
					CPropertyModule::GetSingleton().SendRideHorseNotice( this, nHorseID, 0 );
				}
			}
		}
	}

	// 暂时写在这里,以后再来封装
	// 如果最大HP或者最大MP改变时,广播HP,MP改变的消息
	if ( tnOldMaxHP != ((unsigned int) pDesProperty->GetMaxHP( )) || tnOldMaxMP != ((unsigned int) pDesProperty->GetMaxMP( )) )
	{
		CMessage tMessage;
		CMessageEntityRefreshNotice tEntityRefreshNotice;

		if ( CPropertyModule::GetSingleton( ).CreateMessageEntityRefreshNotice( 
			&tMessage,
			&tEntityRefreshNotice,
			pDesProperty->GetEntityID( ),
			pDesProperty->GetCurHP( ),
			pDesProperty->GetCurMP( ),
			pDesProperty->GetMaxHP( ),
			pDesProperty->GetMaxMP( ) )
			)
		{
			CMapModule::GetSingleton( ).MessageBroadCast( this, &tMessage, false, false );
		}
	}

	return SUCCESS;
}


// 实体身上是否有某BUFF
bool CEntityCharacter::EntityHasBuff( int nBuffID )
{
	CPropertyCharacter* pProperty = (CPropertyCharacter*) GetProperty();
	CBuffList* tpBuff = pProperty->GetBuffList( );
	for ( unsigned int i = 0; i < ARRAY_CNT( tpBuff->mBuffList ) - 1; i++ )
	{
		if ( tpBuff->mBuffList[i].mBuffID != 0 )
		{
			if ( nBuffID == (int)tpBuff->mBuffList[i].mBuffID )
			{
				return true;
			}
		}
	}
	return false;
}

// 实体删除BUFF
int CEntityCharacter::EntityRemoveBuff( int vBuffID, int vListID )
{
	CPropertyCharacter* pProperty = (CPropertyCharacter*) GetProperty();

	int nSrcEntityID = 0;
	int tBackDamage = 0;
	int tListID = vListID;

	CTemplateBuff* tpBuffTpl = (CTemplateBuff*) CDataStatic::SearchTpl( vBuffID );
	if( tpBuffTpl == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ][ %s ]Buff( %d ) Cannt find Template", __LK_FILE__, __LINE__, __FUNCTION__,
			vBuffID );
		return 0;
	}

	int nHorseID = 0;
	bool bIsOffHorse = false; // 是否在删除BUFF后下马
	// 先判断是否坐骑BUFF 如果是 如果不能下坐骑 则不让取消
	if ( this->IsPlayer() )
	{
		CPropertyPlayer* pPro = (CPropertyPlayer*)pProperty;
		nHorseID = pPro->GetHorseTempID();

		// 如果在坐骑上 且正要取消这个BUFF 则要判断能不能下马
		if ( nHorseID != 0 )
		{
			CTplItemHorse* tpTplHorse = (CTplItemHorse*)CDataStatic::SearchTpl(nHorseID);
			if (tpTplHorse == NULL)
			{
				LOG_ERROR("pro", "no such hourse? id=%d", nHorseID);
			}
			else
			{
				if ( vBuffID == tpTplHorse->mSpeedBuff ) // 要下马 则判断能否下马
				{
					// 阻挡里不能下马
					if ( CMapModule::GetSingleton().IsStaticBlock( GetLineID(), GetMapID(), 
						GetMapIndex(), GetPos() ) )
					{
						CPropertyModule::GetSingleton().SendRideHorseNotice( this, nHorseID, ERROR_CANNOTOFF );
						return ERROR_CANNOTOFF;
					}
					bIsOffHorse = true;
				}
			}
		}
	}


	CBuffList* tpBuff = pProperty->GetBuffList( );
	if ( IsAlive() == true )
	{
		for ( int i = 0; i < tpBuff->GetBuffNum(); i++ )
		{
			// 如果Buff相同
			if ( vBuffID == (int)tpBuff->mBuffList[i].mBuffID )
			{
				nSrcEntityID = tpBuff->mBuffList[ i ].mSrcEntityID;
				tBackDamage = tpBuff->mBuffList[ i ].mBackDamage;
			}
		}
	}

	int tRtn = tpBuff->RemoveBuff( vBuffID, this, &tListID );

	if( tRtn < 0 )
	{

		LOG_ERROR( "pro", "[ %s : %d ][ %s ]Entity( %d ) NpcID(%d) Remove Buff Err, BuffID = %d, rtn = %d", __LK_FILE__, __LINE__,
				__FUNCTION__, GetEntityID( ), GetNpcID(), vBuffID, tRtn );

#ifdef _DEBUG_
		print_trace_fd( "pro" );
#endif
		
		return tRtn;
	}

	// 获得未改变前的最大HP,最大MP
	unsigned int tnOldMaxHP = pProperty->GetMaxHP( );
	unsigned int tnOldMaxMP = pProperty->GetMaxMP( );

	RemoveBuffState( tpBuffTpl, nSrcEntityID, tBackDamage );

	pProperty->Refresh( );

	AddFunResult( 0, FUNC_REMOVEBUFF, RESULT_NORMAL, true, vBuffID, tListID );
	AddFunResult( 0, FUNC_SYNCHPMP, RESULT_NONE, true, pProperty->GetCurHP( ), pProperty->GetMaxHP( ), pProperty->GetCurMP( ), pProperty->GetMaxMP( ) );

	// 在NPC身上去除Buff的话不用触发刷新消息，怪物没有属性，只有玩家才触发
	if( this->IsPlayer() )
	{
		// 如果复制变身
		if ( tpBuffTpl->mTransform != 0 )
		{
			tpBuff->SetTransform( NULL, 0 );
			CPropertyModule::GetSingleton().SendPlayerTransformNotify( (CEntityPlayer*)this );
		}

		NotifyPropertyRefresh( );

		// 如果是下马 则发通知
		if ( nHorseID != 0 && bIsOffHorse )
		{
			CPropertyModule::GetSingleton().SendRideHorseNotice( this, nHorseID, 0 );
		}
		// 通知任务模块
		CTaskModule::GetSingleton().FireEvent( (CEntityPlayer*)this, EVENT_BUFFCHANGED, vBuffID );
	}

	// 暂时写在这里,以后再来封装
	// 如果最大HP或者最大MP改变时,广播HP,MP改变的消息
	if ( tnOldMaxHP != ((unsigned int) pProperty->GetMaxHP( )) || tnOldMaxMP != ((unsigned int) pProperty->GetMaxMP( )) )
	{
		CMessage tMessage;
		CMessageEntityRefreshNotice tEntityRefreshNotice;

		if ( CPropertyModule::GetSingleton( ).CreateMessageEntityRefreshNotice( 
			&tMessage,
			&tEntityRefreshNotice,
			pProperty->GetEntityID( ),
			pProperty->GetCurHP( ),
			pProperty->GetCurMP( ),
			pProperty->GetMaxHP( ),
			pProperty->GetMaxMP( ) )
			)
		{
			CMapModule::GetSingleton( ).MessageBroadCast( this, &tMessage, false, false );
		}
	}

	LOG_INFO( "pro", "[ %s : %d ][ %s ]Entity( %d ) Remove Buff, BuffID = %d, Now Buff %d", __LK_FILE__, __LINE__, __FUNCTION__, GetEntityID( ), vBuffID , tpBuff->mBuffNumber );

	return tRtn;
}

// 实体死亡
//************************************
// Method:    EntityDie
// FullName:  CEntityCharacter::EntityDie
// Access:    public 
// Returns:   void
// Qualifier:  【注意】pSrcEntity 可以为null,每次用之前一定判断。
// Parameter: CEntityCharacter * pSrcEntity
// Parameter: bool bKillSelf
// Parameter: bool vIsAward
//************************************
void CEntityCharacter::EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf, bool vIsAward )
{
	CPropertyCharacter* pDesProperty = (CPropertyCharacter*) GetProperty( );

	if( pSrcEntity != NULL )
	{
		LOG_DEBUG( "pro", "[ %s : %d ][ %s ]Entity( %d ) kill Entity( %d )", __LK_FILE__, __LINE__, __FUNCTION__, 
			pSrcEntity->GetEntityID( ), GetEntityID( ) );
	}

	SetAliveStatus( ALIVE_DIE );
	SetHasTarget( false );
	SetTargetPos( CWTPoint( ) );

	// 清除BUFF
	RemoveBuffEntityDie( );
	
	

	// 给客户端发实体死亡的消息
	AddFunResult( ((pSrcEntity!=NULL)?pSrcEntity->GetEntityID():0), FUNC_DIE, RESULT_NORMAL, true, GetPosX(), GetPosY() );
	//必须先发一次死亡消息，不然会错过系统复活
	CPropertyModule::GetSingletonPtr()->ResultListSend();
	// 计时器的处理
	TimeForDie( );

	// 遍历死亡者的伤害列表,把死亡者从列表中对象的伤害列表中去掉,最后清空
	CDamageList *tpDamageList = pDesProperty->GetDamageList( ); 
	for ( int i = 0; i < tpDamageList->mNumber; i++ )
	{
		CPropertyCharacter* pAttacker = (CPropertyCharacter*) CProperty::SearchProByEntityID( tpDamageList->mDamage[ i ].mSrcEntityID );
		if ( pAttacker == NULL )
			continue;
		CDamageList* tpAttackList = pAttacker->GetDamageList( );
		tpAttackList->RemoveDamage( GetEntityID( ) );
	} 
	tpDamageList->ClearDamage( );

	// 通知NPC模块死亡情况
	if ( pSrcEntity != NULL )
		NotifyDeath( pSrcEntity );

	// 通知观察者( 杀人者可以为NULL,比如被buff杀死，放buff的人已经下线）
	if( vIsAward == true )
	{
		CharacterDeathNotity( this->GetEntityID( ), pSrcEntity != NULL ? pSrcEntity->GetEntityID( ) : NULL ); 
	}

	// 玩家杀死对方后，会取血
	if ( pSrcEntity != NULL && pSrcEntity->IsPlayer() )
	{
		KillerRenew( pSrcEntity );
	}

	if ( IsPlayer() )
	{
		CPropertyPlayer* pPro = (CPropertyPlayer*)GetProperty( );
		pPro->SetDieNum( pPro->GetDieNum() + 1);
		// 扣减法宝灵气值
		CItemMagicWeapon* tpObjMW = (CItemMagicWeapon*)pPro->GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
		if ( tpObjMW != NULL )
		{
			if ( tpObjMW->OnDieCutHp() != 0 )
			{
				CPropertyModule::GetSingleton().NotifyRefreshMagicWeaponProperty( (CEntityPlayer*)this, MAGIC_WEAPON_HP, -1 );
			}
		}		
	}	

	// 处理城门大旗事件
	if ( this->IsOgre() || this->IsFunNpc() || this->IsBlock() )
	{
		CEntityNpc *tpNpc = (CEntityNpc*)this;
		CEntityPlayer *tpPlayer = (CEntityPlayer *)pSrcEntity;
		CTemplateOgre *tplNpc = (CTemplateOgre*)CDataStatic::SearchTpl( tpNpc->GetNpcID() );
		if ( tplNpc != NULL && tplNpc->mOgreEvent != 0 && tpPlayer != NULL )
		{
			CWorldModule::GetSingleton().SendSpecialEventNotify( tpPlayer->GetNationality(), this->GetLineID(), tplNpc->mOgreEvent, tpPlayer->GetCharID() );
		}
	}

	// 计算PK值，计算掉落
	CPropertyModule::GetSingleton().ProcessPKValueOnDie( pSrcEntity, ( CEntityCharacter* ) this );	

	return ;
}

void CEntityNpc::EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf /* = false */, bool vIsAward /* = true */ )
{
	CEntityCharacter::EntityDie(pSrcEntity, bKillSelf, vIsAward);	
	return;
}

void CEntityPlayer::EntityDie(CEntityCharacter *pSrcEntity, bool bKillSelf, bool vIsAward)
{
	CPropertyPlayer* pDesProperty = (CPropertyPlayer*) GetProperty( );

	// 销毁召唤兽
	if ( pDesProperty->GetPetID( ) != 0 )
	{		
		pDesProperty->DestroyPetEntity( );
	}

	if ( pSrcEntity != NULL && vIsAward )
		HonorForDie( pSrcEntity );

	// 非自杀需要消耗装备耐久度
	CPropertyModule::GetSingleton().ChangeDurabilityOnDeath( this );

	// 交易取消
	CTradeModule::GetSingleton().OnCancelTrade( mEntityID );

	pDesProperty->SetDieNum( pDesProperty->GetDieNum() + 1);
	if ( pSrcEntity != NULL )
	{
		if ( pSrcEntity->IsPlayer() )
		{
			if ( GetNationality() != pSrcEntity->GetNationality() && ((CEntityPlayer*)pSrcEntity)->IsHandle( UN_HANDLE_PROCESS_FOR_ATTACK ) == true )
			{
				int tIntParam[ 5 ] = { 0 };
				tIntParam[ 0 ] = pSrcEntity->GetNationality();
				tIntParam[ 1 ] = this->GetNationality();
				tIntParam[ 2 ] = pSrcEntity->GetMapID();
				tIntParam[ 3 ] = pSrcEntity->GetPosX();
				tIntParam[ 4 ] = pSrcEntity->GetPosY();
				CPropertyModule::GetSingleton().SendEventNotify( NOTICE_KILLPLAYER, CSceneLogic::GetSingleton().GetLineID(), tIntParam, ARRAY_CNT(tIntParam), (char *)pSrcEntity->GetName(), NULL );
				CWorldModule::GetSingleton().SendKillPersonRequest( ((CEntityPlayer*)pSrcEntity)->GetCharID(), this->GetCharID() );
			}		
			
			LogEventPlayerKilledByPlayer((CEntityPlayer*)this, (CEntityPlayer*) pSrcEntity); 
		}
		else
		{
			LogEventPlayerKilledByOther((CEntityPlayer*)this, pSrcEntity);
		}
	}

	/*
	CSceneLogic::GetSingleton().Send2Log( BuildPlayerDieLogMessage( (CEntityPlayer*)this, tKillerType, tKillerID, 
		( pSrcEntity == NULL ? "" : pSrcEntity->GetName() ) ), (int)EIHANDLE_TWO, tKillerID);
	*/

	mSpeedChecker.ClearFailedCount();
	CEntityCharacter::EntityDie(pSrcEntity, bKillSelf, vIsAward);
	return ;
}

void CEntityOgre::EntityDie(CEntityCharacter *pSrcEntity, bool bKillSelf , bool vIsAward )
{	
	CPropertyCharacter* pDesProperty = ( CPropertyCharacter* ) GetProperty();
	if ( pSrcEntity != NULL && vIsAward )
	{
		ExpForDie( pSrcEntity );
		//CTaskModule::GetSingleton( ).OnEntityDie( pSrcEntity, this );
	}

	CTemplateNpc* tpTempOgre = (CTemplateNpc*) CDataStatic::SearchTpl( pDesProperty->GetPropertyTempID( ) );
	if ( tpTempOgre == NULL )
	{
		LOG_DEBUG( "pro", "EntityDie:Ogre id = %d not found", pDesProperty->GetPropertyTempID( ) );
		return;
	}

	// 如果是国战则发送资源消息
	if ( IS_BATTLE_LINE_ID( CSceneLogic::GetSingleton().GetLineID() ) == true
		&& tpTempOgre->mWarValue != 0
		&& pSrcEntity->IsPlayer() )
	{
		CEntityPlayer *tpPlayer = (CEntityPlayer*)pSrcEntity;
		CMessage tGateMessage;
		CMessageWarChangeNotify tGatePara;
		tGateMessage.mutable_msghead()->set_messageid( ID_S2G_NOTIFY_WARCHANGE );
		tGateMessage.set_msgpara( (long) &tGatePara );
		tGatePara.set_sourcetype( CHANGE_ADD );
		tGatePara.set_worldid( tpPlayer->GetNationality() );
		tGatePara.set_sourcevalue( tpTempOgre->mWarValue );
		tGatePara.set_mapid( GetMapID() );
		CSceneLogic::GetSingleton().Send2Gate( &tGateMessage );
		if ( GetOwnCampID() != 0 )
		{
			tGatePara.set_sourcetype( CHANGE_DEL );
			tGatePara.set_worldid( GetOwnCampID() );
			tGatePara.set_sourcevalue( tpTempOgre->mWarValue );
			tGatePara.set_mapid( GetMapID() );
			CSceneLogic::GetSingleton().Send2Gate( &tGateMessage );
		}

		CPropertyModule::GetSingleton().SendWarEventNotify( NOTICE_WAR_FLAG_CUTDOWN, tpPlayer->GetMapID(), tpPlayer->GetPosX(), 
			tpPlayer->GetPosY(), GetOwnCampID(), tpPlayer->GetNationality(), tpTempOgre->mWarValue, pDesProperty->GetPropertyTempID( ), tpPlayer->GetCharName() );

		CPropertyModule::GetSingleton().ObtainWarHonor( tpPlayer, tpTempOgre->mWarHonor, false );
	}

	if ( pSrcEntity != NULL && pSrcEntity->IsPlayer() )
	{
		CPropertyPlayer* tpProPlayer = (CPropertyPlayer*)pSrcEntity->GetProperty();
		tpProPlayer->SetKillOgreNum( tpProPlayer->GetKillOgreNum() +1);  // 杀怪计数

		// 防沉迷之后的杀怪计数
		if( ((CEntityPlayer*) pSrcEntity)->GetFcmStatus() == em_half_rate )
		{
			((CEntityPlayer*) pSrcEntity)->IncAfterFcmKilled();
		}

		if ( tpTempOgre->mObtainTitle != INVALID_OBJ_ID )
		{
			CEntityPlayer* tTeamPlayerList[ TEAM_NUMBER ] = { NULL };
			int tNum = 0;
			CTeamModule::GetSingleton().OnGetMemberList( (CEntityPlayer*)pSrcEntity, tTeamPlayerList, TEAM_NUMBER, tNum );
			for ( int i = 0; i < tNum; i ++ )
			{
				if ( tTeamPlayerList[ i ] != NULL )
					CPropertyModule::GetSingleton().OnPlayerAddTitleEvent( tTeamPlayerList[ i ], tpTempOgre->mObtainTitle );
			}			
		}

		if ( tpTempOgre->mDisplayLevel == CTemplateOgre::OGRESHOW_BOSS )
		{
			int tIntParam[ 2 ] = { 0 };
			tIntParam[ 0 ] = pDesProperty->GetPropertyTempID( );
			tIntParam[ 1 ] = RAND(4);
			int tCountryID = this->GetLineID( );
			if( IS_REPETION_LINE_ID( tCountryID ) == false )
			{
				CPropertyModule::GetSingletonPtr()->SendEventNotify( NOTICE_PLAYERKILLBOSS, tCountryID, tIntParam, 
					ARRAY_CNT(tIntParam), (char *)(((CEntityPlayer*)pSrcEntity)->GetName( )), NULL );
			}			
		}

	}
	if ( pSrcEntity != NULL && pSrcEntity->IsOgre()  )
	{
		CProperty *tpProperty = pSrcEntity->GetProperty();
		if ( tpTempOgre->mDisplayLevel == CTemplateOgre::OGRESHOW_BOSS )
		{
		
			int tIntParam[ 2 ] = { 0 };
			tIntParam[ 0 ] = pDesProperty->GetPropertyTempID( );
			tIntParam[ 1 ] = tpProperty->GetPropertyTempID();
			int tCountryID = this->GetLineID( );
			
		
			if( IS_REPETION_LINE_ID( tCountryID ) == false && tIntParam[ 0 ] != tIntParam[ 1 ] )
			{
				CPropertyModule::GetSingletonPtr()->SendEventNotify( NOTICE_OGREKILLBOSS, tCountryID, tIntParam, 
					ARRAY_CNT(tIntParam),  NULL, NULL );				
			}			
		}		
	}
	
	if ( pSrcEntity != NULL )
	{
		CActivityModule::GetSingleton().CheckOgreActivityInfoOnDeath( pDesProperty->GetPropertyTempID(), pSrcEntity->GetEntityID() );
	}	

	CNpcModule::GetSingleton().OnSneered( 0, GetEntityID(), 1 );	

	CEntityCharacter::EntityDie(pSrcEntity, bKillSelf, vIsAward);
}

void CEntityPet::EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf /* = false */, bool vIsAward /* = true */ )
{
	CEntityCharacter* tpOwner = (CEntityCharacter*) GetOwner( );
	if ( tpOwner != NULL )
	{
		CPropertyCharacter* tpProOwner = (CPropertyCharacter*) tpOwner->GetProperty( );
		tpProOwner->SetPetID( 0 );
	}
	
	CNpcModule::GetSingleton().OnSneered( 0, GetEntityID(), 1 );
	
	CEntityCharacter::EntityDie(pSrcEntity, bKillSelf, vIsAward);
	return ;
}

void CEntityBlock::EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf /* = false */, bool vIsAward /* = true */ )
{
	CMapObject* tpMapObj = GetMapObject( );	
	
	LK_ASSERT_STR( tpMapObj != NULL, return, "Entity( %d ) Map( %d : %d : %d ) Is Null", GetEntityID( ),
		GetLineID( ), GetMapID( ), GetMapIndex( ) );

	( ( CEntityBlock* )this )->RemoveBlock( );

	if ( pSrcEntity != NULL )
		CTaskModule::GetSingleton( ).OnEntityDie( pSrcEntity, this );
	CEntityCharacter::EntityDie(pSrcEntity, bKillSelf, vIsAward);
	return ;
}

void CEntityPedal::EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf /* = false */, bool vIsAward /* = true */ )
{
	return ;
}

void CEntityFuncNpc::EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf /* = false */, bool vIsAward /* = true */ )
{
	if ( pSrcEntity != NULL )
	{
		CTaskModule::GetSingleton( ).OnEntityDie( pSrcEntity, this );
	}

	CNpcModule::GetSingleton().OnSneered( 0, GetEntityID(), 1 );
	
	CEntityCharacter::EntityDie(pSrcEntity, bKillSelf, vIsAward);
	return ;
}

void CEntityBox::EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf /* = false */, bool vIsAward /* = true */ )
{
	CEntityOgre::EntityDie(pSrcEntity, bKillSelf, vIsAward);
	return ;
}

// 清除BUFF状态
void CEntityCharacter::RemoveBuffState( CTemplateBuff* pBuffTpl, int& nSrcEntityID, int& nBackDamage )
{
	LK_ASSERT( pBuffTpl != NULL, return );

	CPropertyCharacter* pProperty = (CPropertyCharacter*) GetProperty();

	for ( unsigned int t = 0; t < ARRAY_CNT( pBuffTpl->mBuff ); t ++ )
	{
		if ( pBuffTpl->mBuff[ t ].mBuffFunc == 0 )
			continue;

		switch( pBuffTpl->mBuff[ t ].mBuffFunc )
		{
		case CTemplateBuff::BUFFFUNC_PLAYER_UNATTACK:
			{
				pProperty->DetachState( STATE_PLAYER_UNATTACK, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_UNATTACK_PLAYER:
			{
				pProperty->DetachState( STATE_UNATTACK_PLAYER, pBuffTpl->mTempID, BUFF );
			}
			break;
			// 守护
		case CTemplateBuff::BUFFFUNC_PROTECTED:
			{
				pProperty->DetachState( STATE_PROTECTED, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_MAGICSHIELD:
			{
				pProperty->DetachState( STATE_MAGICSHIELD, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_IMMUNITYDAMAGE:
			{
				pProperty->DetachState( STATE_IMMUNITYDAMAGE, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_IMMUNITYREVERSD:
			{
				pProperty->DetachState( STATE_IMMUNITYREVERSD, pBuffTpl->mTempID, BUFF );
			}
		case CTemplateBuff::BUFFFUNC_DAMAGELESS:
			{
				pProperty->DetachState( STATE_DAMAGELESS, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_UNATTACK:
			{
				pProperty->DetachState( STATE_UNATTACK, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_UNMOVE:
			{
				pProperty->DetachState( STATE_CANNTMOVE, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_UNUSEITEM:
			{
				pProperty->DetachState( STATE_FASTING, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_REBOUNDDAMAGE:
			{
				pProperty->DetachState( STATE_REBOUND, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFINC_DECSPEEDPER:
			{
				pProperty->DetachState( STATE_UNCREAMSPEED, pBuffTpl->mTempID, BUFF, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );

				unsigned int tNewSpeed = CPropertyFormula::GetRunSpeed( pProperty );

				// 速度不同,通知可见客户端
				if ( pProperty->GetCurSpeed( ) != tNewSpeed )
				{
					pProperty->SetCurSpeed( tNewSpeed );

					AddFunResult( 0, FUNC_CHANGESPEED, RESULT_NORMAL, true, tNewSpeed );
					// 通知NPC模块
				}
			}
			break;
		case CTemplateBuff::BUFFINC_INCSPEEDPER:
			{
				pProperty->DetachState( STATE_INCREAMSPEED, pBuffTpl->mTempID, BUFF, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );

				unsigned int tNewSpeed = CPropertyFormula::GetRunSpeed( pProperty );

				// 速度不同,通知可见客户端
				if ( pProperty->GetCurSpeed( ) != tNewSpeed )
				{
					pProperty->SetCurSpeed( tNewSpeed );

					AddFunResult( 0, FUNC_CHANGESPEED, RESULT_NORMAL, true, tNewSpeed );
					// 通知NPC模块
				}
			}
			break;
		case CTemplateBuff::BUFFFUNC_ENDEVENT:
			{
				if ( pBuffTpl->mBuff[t].mBuffParam[1] == 1 )
					nSrcEntityID = this->GetEntityID();
				if ( nSrcEntityID == 0 || IsAlive() == false )
					break;

				CEntityCharacter* tpSrcEntity = (CEntityCharacter*) CSceneLogic::GetSingletonPtr( )->GetEntity( nSrcEntityID );
				if ( tpSrcEntity == NULL  )
					break;

				if( tpSrcEntity->IsCharacter() == false )
					break;

				CTemplateBuff* tTmpTPL = (CTemplateBuff*) CDataStatic::SearchTpl( pBuffTpl->mBuff[t].mBuffParam[0] );
				if ( tTmpTPL == NULL )
				{
					LOG_ERROR( "pro", "[ %s : %d ][ %s ]Entity( %d ) Cannt find Buff( %d ) Template", 
						__LK_FILE__, __LINE__, __FUNCTION__, tpSrcEntity->GetEntityID( ), 
						pBuffTpl->mBuff[t].mBuffParam[0] );
					break;
				}
				// 被攻击(作用)
				this->AddFunResult( 0, FUNC_ATTACKED, RESULT_NONE, true, 
					this->GetEntityID( ), pBuffTpl->mTempID, 0, this->GetPosX( ), this->GetPosY( ) );

				EntityInsertBuff( tpSrcEntity, tTmpTPL );
			}
			break;

		case CTemplateBuff::BUFFFUNC_HPBACKEFFECT:
			{
				pProperty->DetachState( STATE_CUREHPLESS, pBuffTpl->mTempID, BUFF, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
			}
			break;
		case CTemplateBuff::BUFFFUNC_KILLERRENEW:
			{
				pProperty->DetachState( STATE_KILLERRENEW, pBuffTpl->mTempID, BUFF, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
			}
			break;
		case CTemplateBuff::BUFFFUNC_FIREEVENT:
			{
				pProperty->DetachState( STATE_FIREEVENT, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_ATTACKEVENT:
			{
				pProperty->DetachState( STATE_ATTACKEVENT, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_IGNORECLOTH:
			{
				pProperty->DetachState( STATE_IGNORECLOTH, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_UNARM:
			{
				pProperty->DetachState( STATE_UNARM, pBuffTpl->mTempID, BUFF );
			}
			break;
			// 增加被攻击时所受物理伤害固定值
		case CTemplateBuff::BUFFINC_INCDESPDAMAGEFIX:
			{
				pProperty->DetachState( STATE_INCDESPDAMAGEFIX, pBuffTpl->mTempID, BUFF );
			}
			break;
			// 增加被攻击时所受物理伤害百分比
		case CTemplateBuff::BUFFINC_INCDESPDAMAGEPER:
			{
				pProperty->DetachState( STATE_INCDESPDAMAGEPER, pBuffTpl->mTempID, BUFF );
			}
			break;
			// 降低被攻击时所受物理伤害固定值
		case CTemplateBuff::BUFFINC_DECDESPDAMAGEFIX:
			{
				pProperty->DetachState( STATE_DECDESPDAMAGEFIX, pBuffTpl->mTempID, BUFF );
			}
			break;
			// 降低被攻击时所受物理伤害百分比
		case CTemplateBuff::BUFFINC_DECDESPDAMAGEPER:
			{
				pProperty->DetachState( STATE_DECDESPDAMAGEPER, pBuffTpl->mTempID, BUFF );
			}
			break;
			// 增加被攻击时所受法术伤害固定值
		case CTemplateBuff::BUFFINC_INCDESMDAMAGEFIX:
			{
				pProperty->DetachState( STATE_INCDESMDAMAGEFIX, pBuffTpl->mTempID, BUFF );
			}
			break;
			// 增加被攻击时所受法术伤害百分比
		case CTemplateBuff::BUFFINC_INCDESMDAMAGEPER:
			{
				pProperty->DetachState( STATE_INCDESMDAMAGEPER, pBuffTpl->mTempID, BUFF );
			}
			break;
			// 降低被攻击时所受法术伤害固定值
		case CTemplateBuff::BUFFINC_DECDESMDAMAGEFIX:
			{
				pProperty->DetachState( STATE_DECDESMDAMAGEFIX, pBuffTpl->mTempID, BUFF );
			}
			break;
			// 降低被攻击时所受法术伤害百分比
		case CTemplateBuff::BUFFINC_DECDESMDAMAGEPER:
			{
				pProperty->DetachState( STATE_DECDESMDAMAGEPER, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_RANDMOVE:
			{
				pProperty->DetachState( STATE_RANDMOVE, pBuffTpl->mTempID, BUFF );
				pProperty->DetachState( STATE_CONFUSION, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_SNEER:
			{
				pProperty->DetachState( STATE_SNEER, pBuffTpl->mTempID, BUFF );

				if ( IsNpc() )
				{
					// 通知NPC模块，NPC被嘲讽
					// 0表示进入嘲讽 1表示退出嘲讽
					CNpcModule::GetSingleton().OnSneered( nSrcEntityID, GetEntityID(), 1 );
				}
			}
			break;
		case CTemplateBuff::BUFFFUNC_INCEXP:
			{
				pProperty->DetachState( STATE_INCEXP, pBuffTpl->mTempID, BUFF, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
			}
			break;
			// 减少所有受到伤害的百分比 ( 包括BUFF )
		case CTemplateBuff::BUFFFUNC_DECALLDAMAGEPER:
			{
				pProperty->DetachState( STATE_DECALLDAMAGEPER, pBuffTpl->mTempID, BUFF, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
			}
			break;
		case CTemplateBuff::BUFFFUNC_SAVEDAMAGE:
			{
				if ( nBackDamage > 0 )
				{
					CEntityCharacter* tpSrcEntity = (CEntityCharacter*) CSceneLogic::GetSingletonPtr( )->GetEntity( nSrcEntityID );
					if ( tpSrcEntity != NULL  )
					{
						CPropertyModule::GetSingleton().EntityDamaged( tpSrcEntity, this, nBackDamage, RESULT_REBOUND );
					}
				}
			}
			break;
		case CTemplateBuff::BUFFFUNC_TRANSFORM:
			{
				pProperty->DetachState( STATE_TRANSFORM, pBuffTpl->mTempID, BUFF );
			}
			break;
		case CTemplateBuff::BUFFFUNC_INCGENERDAMAGEPER:
			{
				pProperty->DetachState( STATE_INCGENERDAMAGEPER, pBuffTpl->mTempID, BUFF );
			}
			break;
			// 获得功勋百分比
		case CTemplateBuff::BUFFFUNC_INCHONORPER:
			{
				pProperty->DetachState( STATE_INCHONORSCALE,  pBuffTpl->mTempID, BUFF );
			}
			break;	
			
		// 增加军团的荣耀值
		case CTemplateBuff::BUFFFUNC_INCCORPSGORY:
			{
				pProperty->DetachState( STATE_INCCORPSGLORY, pBuffTpl->mTempID, BUFF, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
			}
			break;		
			
		// 	增加玩家的ap值
		case CTemplateBuff::BUFFFUNC_INCAP:
			{
			   pProperty->DetachState( STATE_INCAP, pBuffTpl->mTempID, BUFF, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
			   break;
			}
		// 处理坐骑
		case CTemplateBuff::BUFFFUNC_HORSE:
			{
				pProperty->DetachState( STATE_RIDE_HORSE, pBuffTpl->mTempID, BUFF, 
					pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );				
				break;
			}
		// 隐身
		case CTemplateBuff::BUFFFUNC_INVISIBLE:
			{
				pProperty->DetachState( STATE_INVISIBLE, pBuffTpl->mTempID, BUFF, 
					pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
				break;
			}
		case CTemplateBuff::BUFFFUNC_DISINVISIBLE:
			{
				pProperty->DetachState( STATE_DISINVISIBLE, pBuffTpl->mTempID, BUFF, 
					pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
				break;
			}
		case CTemplateBuff::BUFFFUNC_FLY:
			{
				pProperty->DetachState( STATE_FLY, pBuffTpl->mTempID, BUFF, 
					pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
				break;
			}
		case CTemplateBuff::BUFFFUNC_STONE:
			{
				pProperty->DetachState( STATE_STONE, pBuffTpl->mTempID, BUFF, 
					pBuffTpl->mBuff[ t ].mBuffParam[ 0 ], pBuffTpl->mBuff[ t ].mBuffParam[ 1 ] );
				break;
			}
		case CTemplateBuff::BUFFFUNC_ADDSKILL:
			{
				if ( IsPlayer() == true )
				{
					CEntityPlayer *tpPlayer = (CEntityPlayer *)this;
					CPropertyPlayer *tpPro = (CPropertyPlayer *)tpPlayer->GetProperty();
					CSkillList&  tpSkill = tpPro->GetSkillList( );
					tpSkill.RemoveSkill( pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
				}
				break;
			}
		case CTemplateBuff::BUFFFUNC_HIDE:
			{
				pProperty->DetachState( STATE_HIDE, pBuffTpl->mTempID, BUFF, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
				break;
			}
		case CTemplateBuff::BUFFFUNC_INCOFFLINEEXP:
			{
				pProperty->DetachState( STATE_INCOFFLINEEXP, pBuffTpl->mTempID, BUFF, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
			}
			break;
		default:
			break;
		}
	}
}

// 添加BUFF状态
void CEntityCharacter::InsertBuffState( CEntityCharacter* pSrcEntity, CTemplateBuff* pBuffTpl, CBuffObject* vpBuffObject/*CBuffList* pBuffList*/, int nSkillType, int vDamage )
{
	LK_ASSERT( pSrcEntity != NULL && pBuffTpl != NULL && pBuffTpl != NULL, return )

	CPropertyCharacter* pSrcProperty = (CPropertyCharacter*) pSrcEntity->GetProperty();
	CPropertyCharacter* pDesProperty = (CPropertyCharacter*) GetProperty();

	CTplBuffImmunity* tpBuffImmunity = NULL;
	int tBuffUmmuTableID = 0;

	if( IsNpc() || IsPlayer() )
	{	
		if ( ( IsNpc( ) == true ) && ( IsPet( ) == false ) )
		{
			CEntityNpc* tpNpc = ( CEntityNpc* )this;
			CTemplateOgre* tpTpl = ( CTemplateOgre* )CDataStatic::SearchTpl( tpNpc->GetNpcID( ) );
			if( tpTpl != NULL )
			{
				tBuffUmmuTableID = tpTpl->mBuffImmunity;
			}
			else
			{
				LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Get Npc( %d )'s Template( %d )", __LK_FILE__,
						__LINE__, __FUNCTION__, GetEntityID( ), tpNpc->GetNpcID( ) );
			}
		}
		else if( IsPlayer() ) // 法宝可能有免疫BUFF列表
		{
			CEntityPlayer* tpPlayer = (CEntityPlayer*)this;
			CPropertyPlayer* tpPro = (CPropertyPlayer*)tpPlayer->GetProperty();
			CItemMagicWeapon* tpMW = (CItemMagicWeapon*)tpPro->GetEquipment()->GetItemObjPtr(EQUIPMENT_MAGICWEAPON);
			if ( tpMW != NULL )
			{
				CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( tpMW->GetItemID() );
				LK_ASSERT( tpItemMW != NULL, return );
				tBuffUmmuTableID = tpItemMW->mUmmiBuffTableID;
			}
		}

		if( tBuffUmmuTableID != 0 )
		{
			tpBuffImmunity = ( CTplBuffImmunity* )CDataStatic::SearchTpl( tBuffUmmuTableID );
			if( tpBuffImmunity == NULL )
			{
				LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Find ImmunityTable %d, Entity( %d : %d )", 
					__LK_FILE__, __LINE__, __FUNCTION__, tBuffUmmuTableID, GetEntityID( ), GetEntityType( ) );
			}
		}
	}

	for ( int t = 0; t < (int)ARRAY_CNT( pBuffTpl->mBuff ) ; t ++ )
	{
		if ( pBuffTpl->mBuff[ t ].mBuffFunc == CTemplateBuff::BUFFINC_NONE )
			break;

		bool bIsContinue = false;
		if( tpBuffImmunity != NULL )
		{
			for( unsigned int i = 0; i < ARRAY_CNT( tpBuffImmunity->mBuffImmunity ); i++ )
			{
				if( tpBuffImmunity->mBuffImmunity[ i ] == CTemplateBuff::BUFFINC_NONE )
				{
					break;
				}
				if( tpBuffImmunity->mBuffImmunity[ i ] == pBuffTpl->mBuff[ t ].mBuffFunc )
				{
					LOG_INFO( "default", "[ %s : %d ][ %s ] Npc( %d : %d ) Immunity Buff( %d ) BuffFunc"
						" %d :  %d ", __LK_FILE__, __LINE__, __FUNCTION__, GetEntityID( ), 
						pSrcEntity->GetEntityID(), pBuffTpl->mTempID, t, pBuffTpl->mBuff[ t ].mBuffFunc );
					bIsContinue = true;
					break;
				}
			}

			if( bIsContinue == true )
			{
				continue;	
			}
		}	

		pDesProperty->AttachBuffState( pSrcEntity->GetEntityID( ), &pBuffTpl->mBuff[ t ], pBuffTpl->mTempID );

		switch( pBuffTpl->mBuff[ t ].mBuffFunc )
		{
			// 玩家受到守护
			case CTemplateBuff::BUFFFUNC_PROTECTED:
				{					
					if( vpBuffObject != NULL )
					{
						vpBuffObject->ChangeParames( pBuffTpl->mTempID, t, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
						vpBuffObject->ChangeParames2( pBuffTpl->mTempID, t, pSrcEntity->GetEntityID() );
					}
					//				pBuffList->ChangeParames( pBuffTpl->mTempID, t, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
					//				pBuffList->ChangeParames2( pBuffTpl->mTempID, t, pSrcEntity->GetEntityID() );
				}
				break;
				//复活
			case CTemplateBuff::BUFFFUNC_RELIVE:
				{
					CPropertyModule::GetSingleton().PlayerRelive( ( CEntityPlayer* )this, RELIVE_SKILL, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ], pBuffTpl->mBuff[ t ].mBuffParam[ 1 ] );
				}
				break;
				// 魔法盾
			case CTemplateBuff::BUFFFUNC_MAGICSHIELD:
				{
					float tShieldParam1 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mShieldparam1 / SERVER_PERCENT_FLOAT;
					float tShieldParam2 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mShieldparam2 / SERVER_PERCENT_FLOAT;
					int tAbsorb = pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] + tShieldParam1 * pow( tShieldParam2, pBuffTpl->mSkillLevel ) * vDamage;
					if( vpBuffObject != NULL )
					{
						vpBuffObject->ChangeParames( pBuffTpl->mTempID, t, tAbsorb );
					}
					//				pBuffList->ChangeParames( pBuffTpl->mTempID, t, tAbsorb );
				}
				break;
			case CTemplateBuff::BUFFFUNC_INCFIXHP:
				{
					float tParam1 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mCureparam1 / SERVER_PERCENT_FLOAT;
					float tParam2 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mCureparam2 / SERVER_PERCENT_FLOAT;
					int tCure = pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] + tParam1 * pow( tParam2, pBuffTpl->mSkillLevel ) * vDamage;
					CureHP( tCure, pSrcEntity->GetEntityID() );
				}
				break;
				// 医疗HP百分比
			case CTemplateBuff::BUFFFUNC_INCPERHP:
				{
					CureHP( (int)( pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] * ( (unsigned int)( pDesProperty->GetMaxHP() ) / SERVER_PERCENT_FLOAT ) ), pSrcEntity->GetEntityID() );
				}
				break;
				// 恢复MP固定值
			case CTemplateBuff::BUFFFUNC_INCFIXMP:
				{
					CureMP( pBuffTpl->mBuff[ t ].mBuffParam[ 0 ], pSrcEntity->GetEntityID() );
				}
				break;
				// 恢复MP百分比
			case CTemplateBuff::BUFFFUNC_INCPERMP:
				{
					CureMP( pDesProperty->GetCurMP() * pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] / SERVER_PERCENT_FLOAT, pSrcEntity->GetEntityID() );
				}
				break;
				// 减少HP固定值
			case CTemplateBuff::BUFFFUNC_DECFIXHP:
				{
					float tParam1 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDamageparam1 / SERVER_PERCENT_FLOAT;
					float tParam2 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDamageparam2 / SERVER_PERCENT_FLOAT;
					int tDamage = pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] + tParam1 * pow( pBuffTpl->mSkillLevel, tParam2 ) * vDamage;
					CPropertyModule::GetSingleton().EntityDamaged( pSrcEntity, this, tDamage, RESULT_NORMAL, nSkillType );
				}
				break;
				// 减少HP百分比
			case CTemplateBuff::BUFFFUNC_DECPERHP:
				{
					CPropertyModule::GetSingleton().EntityDamaged( pSrcEntity, this, (int)( pDesProperty->GetCurHP() * pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] / SERVER_PERCENT_FLOAT ), RESULT_NORMAL, nSkillType );
				}
				break;
				// 减少MP固定值
			case CTemplateBuff::BUFFFUNC_DECFIXMP:
				{
					ExpendMP( pBuffTpl->mBuff[ t ].mBuffParam[ 0 ], pSrcEntity->GetEntityID() );
				}
				break;
				// 减少MP百分比
			case CTemplateBuff::BUFFFUNC_DECPERMP:
				{
					ExpendMP( (int)(pDesProperty->GetCurMP() * pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] / SERVER_PERCENT_FLOAT), pSrcEntity->GetEntityID() );
				}
				break;
			case CTemplateBuff::BUFFFUNC_TELEPORT:
				{
					CPropertyModule::GetSingleton().EntityTeleportBuffer( this, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ], pBuffTpl->mBuff[ t ].mBuffParam[ 1 ] );
				}
				break;
				// 抵消N次伤害
			case CTemplateBuff::BUFFFUNC_DAMAGELESS:
				{
					if( vpBuffObject != NULL )
					{
						vpBuffObject->ChangeParames( pBuffTpl->mTempID, t, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
					}
					//				pBuffList->ChangeParames( pBuffTpl->mTempID, t, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
				}
				break;
			case CTemplateBuff::BUFFFUNC_CHANGEPATH:
				{
					// des positions of src entity and des entity 
					int tSrcDesX = GetPosX( );
					int tSrcDesY = GetPosY( );

					int tDesDesX = pSrcEntity->GetPosX( );
					int tDesDesY = pSrcEntity->GetPosY( );

					// src transport
					pSrcEntity->ChangePos( CWTPoint( tSrcDesX, tSrcDesY ), RESULT_TELEPORT, pSrcEntity->GetEntityID() );

					// des transport
					ChangePos( CWTPoint( tDesDesX, tDesDesY ), RESULT_TELEPORT, pSrcEntity->GetEntityID() );
					if ( IsOgre( ) == true || IsFunNpc( ) == true )
					{
						CNpcModule::GetSingletonPtr( )->OnTeleport( mEntityID, tDesDesX, tDesDesY );
					}
				}
				break;
			case CTemplateBuff::BUFFINC_INCSPEEDPER:
				{	
					unsigned int tNewSpeed = CPropertyFormula::GetRunSpeed( pDesProperty );

					// 如果速度改变,则发包通知客户端
					if ( pDesProperty->GetCurSpeed( ) != tNewSpeed )
					{
						pDesProperty->SetCurSpeed( tNewSpeed );
						AddFunResult( pSrcEntity->GetEntityID(), FUNC_CHANGESPEED, RESULT_NORMAL, true, tNewSpeed );
						// TODO 通知npc模块
					}
				}
				break;
				// 降低速度百分比
			case CTemplateBuff::BUFFINC_DECSPEEDPER:
				{
					unsigned int tNewSpeed = CPropertyFormula::GetRunSpeed( pDesProperty );

					// 如果速度改变,则发包通知客户端
					if ( pDesProperty->GetCurSpeed( ) != tNewSpeed )
					{
						pDesProperty->SetCurSpeed( tNewSpeed );
						AddFunResult( pSrcEntity->GetEntityID(), FUNC_CHANGESPEED, RESULT_NORMAL, true, tNewSpeed );
						// TODO 通知npc模块
					}
				}
				break;
				// 清除反面Buff
			case CTemplateBuff::BUFFFUNC_REMOVEREVERSDBUFF:
				{
					CPropertyModule::GetSingleton().EntityRemoveDebuff( this );
				}
				break;
				// 离开中debuff时所处范围会受到特殊效果
			case CTemplateBuff::BUFFFUNC_LEAVEAREA:
				{
					if( vpBuffObject != NULL )
					{
						vpBuffObject->ChangeParames( pBuffTpl->mTempID, t, ( GetPosX( ) << 16 ) | GetPosY( ) );
					}
					//				pBuffList->ChangeParames( pBuffTpl->mTempID, t, ( GetPosX( ) << 16 ) | GetPosY( ) );
				}	
				break;

			case CTemplateBuff::BUFFFUNC_MAGICFIRE:
				{
					unsigned int unMaxMP = pDesProperty->GetMaxMP();

					unsigned int tnMPConsume = (unsigned int)( unMaxMP * pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] / SERVER_PERCENT_FLOAT );

					if ( pDesProperty->GetCurMP() < tnMPConsume && pDesProperty->GetCurMP() > 0 )
					{
						tnMPConsume = pDesProperty->GetCurMP();
					}
					else if ( pDesProperty->GetCurMP() <= 0 )
					{
						break;
					}

					int tnHPConsume = (int)( tnMPConsume * pBuffTpl->mBuff[ t ].mBuffParam[ 1 ] / SERVER_PERCENT_FLOAT );

					if ( tnHPConsume > 0 )
					{
						ExpendMP( tnMPConsume, pSrcEntity->GetEntityID() );

						CPropertyModule::GetSingleton().EntityDamaged( pSrcEntity, this, tnHPConsume, RESULT_NORMAL, nSkillType );
					}
				}
				break;
				// 拉人
			case CTemplateBuff::BUFFFUNC_SUMMON:
				{
					// 通过施法者位置,算出被拉者坐标
					int tPosX = pSrcEntity->GetPosX( );
					int tPosY = pSrcEntity->GetPosY( );

					if ( GetPosX( ) > pSrcEntity->GetPosX( ) )
					{
						tPosX = pSrcEntity->GetPosX( ) + 1;
					}
					else if ( GetPosX( ) < pSrcEntity->GetPosX( ) )
					{
						tPosX = pSrcEntity->GetPosX( ) - 1;
					}

					if ( GetPosY( ) > pSrcEntity->GetPosY( ) )
					{
						tPosY = pSrcEntity->GetPosY( ) + 1;
					}
					else if ( GetPosY( ) < pSrcEntity->GetPosY( ) )
					{
						tPosY = pSrcEntity->GetPosY( ) - 1;
					}

					// 如果该坐标为阻挡,则让两实体重合,并且不判断阻挡关系
					if ( CanPass( tPosX, tPosY ) == false )
					{
						tPosX = pSrcEntity->GetPosX();
						tPosY = pSrcEntity->GetPosY();
					}

					// des transport
					ChangePos( CWTPoint( tPosX, tPosY ), RESULT_TELEPORT, pSrcEntity->GetEntityID() );

					if( IsNpc() )
						CNpcModule::GetSingleton().OnTeleport( GetEntityID(), tPosX, tPosY );
				}
				break;
			case CTemplateBuff::BUFFFUNC_EXCHANGE:
				{					
					int tSrcHP = pSrcProperty->GetCurHP( );
					int tDesHP = pDesProperty->GetCurHP( );
					int tDesMaxHP = pDesProperty->GetMaxHP();
					int tSrcMaxHP = pSrcProperty->GetMaxHP();

					int tSrcChange = tSrcMaxHP * ( (float)tDesHP / (float)tDesMaxHP ) - tSrcHP;
					int tDesChange = 0;
					if ( tSrcChange > 0 )
					{
						pSrcEntity->CureHP( tSrcChange, pSrcEntity->GetEntityID(), RESULT_STEALHP );
					}
					else
					{
						pSrcEntity->ExpendHP( tSrcChange * -1, pSrcEntity->GetEntityID() );
					}

					if ( ( tSrcHP * SERVER_PERCENT_INT / tSrcMaxHP ) < pBuffTpl->mBuff[t].mBuffParam[0] )
					{
						tDesChange = tDesHP - tDesMaxHP * ( pBuffTpl->mBuff[t].mBuffParam[0] / SERVER_PERCENT_FLOAT );
					}
					else
					{
						tDesChange = tDesHP - tDesMaxHP * ( (float)tSrcHP / (float)tSrcMaxHP );					
					}
					if ( tDesChange > 0 )
					{
						ExpendHP( tDesChange, pSrcEntity->GetEntityID() );
					}
					else
					{
						CureHP( tDesChange * -1, pSrcEntity->GetEntityID(), RESULT_STEALHP );
					}
				}
				break;
				// 按所缺失的法力值的百分比掉血
			case CTemplateBuff::BUFFFUNC_DAMAGEFORMP:
				{
					CPropertyValue tDesMPValue = CPropertyFormula::GetMaxMP( pDesProperty );
					int tDesMaxMP = tDesMPValue.mDeltaValue + tDesMPValue.mInitValue;
					int tDamage = (int) ( ( tDesMaxMP - pDesProperty->GetCurMP( ) ) * pBuffTpl->mBuff[t].mBuffParam[0] / SERVER_PERCENT_FLOAT );

					CPropertyModule::GetSingleton().EntityDamaged( pSrcEntity, this, tDamage, RESULT_NORMAL, nSkillType );
				}
				break;
				// 致死
			case CTemplateBuff::BUFFFUNC_HITDEATH:
				{
					ExpendHP( pDesProperty->GetCurHP(), pSrcEntity->GetEntityID() );

					PERF_FUNC( "EntityDie", EntityDie( pSrcEntity, pSrcEntity == this ? true : false ) );
				}
				break;
				// 治疗目标并范围伤害目标周围
			case CTemplateBuff::BUFFFUNC_CUREDAMAGE:
				{
					// 用新的方式实现,这个不用了
				}
				break;
				// 成功放技能会受到特殊效果
			case CTemplateBuff::BUFFFUNC_FIREEVENT:
				{
					if( vpBuffObject != NULL )
					{
						vpBuffObject->ChangeParames( pBuffTpl->mTempID, t, pBuffTpl->mBuff[t].mBuffParam[1] );
					}
					//				pBuffList->ChangeParames( pBuffTpl->mTempID, t, pBuffTpl->mBuff[t].mBuffParam[1] );
				}
				break;
				// 随即走动
			case CTemplateBuff::BUFFFUNC_RANDMOVE:
				{
					switch( GetDirection( ) )
					{
						case DIR_DOWN:
							SetDirection( DIR_UP );
							break;
						case DIR_UP:
							SetDirection( DIR_DOWN );
							break;
						case DIR_RIGHT:
							SetDirection( DIR_LEFT );
							break;
						case DIR_LEFT:
							SetDirection( DIR_RIGHT );
							break;
						case DIR_LEFTUP:
							SetDirection( DIR_RIGHTDOWN );
							break;
						case DIR_RIGHTDOWN:
							SetDirection( DIR_LEFTUP );
							break;
						case DIR_LEFTDOWN:
							SetDirection( DIR_RIGHTUP);
							break;
						case DIR_RIGHTUP:
							SetDirection( DIR_LEFTDOWN );
							break;
						default:
							break;
					}
				}
				break;
			case CTemplateBuff::BUFFFUNC_IMBIBITIONMP:
				{
					int tDecMP = ExpendMP( pBuffTpl->mBuff[ t ].mBuffParam[ 0 ], pSrcEntity->GetEntityID() );
					pSrcEntity->CureMP( tDecMP, pSrcEntity->GetEntityID() );
				}
				break;
				// 减血换伤
			case CTemplateBuff::BUFFFUNC_SUICIDE:
				{
					int tSrcMaxHP = pSrcProperty->GetMaxHP();
					int tSrcHPPercent = (int)( SERVER_PERCENT_FLOAT * pSrcProperty->GetCurHP() / tSrcMaxHP );

					if ( tSrcHPPercent <= pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] )
					{
						break;
					}
					int tDecSrcHP = (int)( pSrcProperty->GetCurHP() - (int)( pSrcProperty->GetMaxHP() ) * pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] / SERVER_PERCENT_FLOAT );

					pSrcEntity->ExpendHP( tDecSrcHP, pSrcEntity->GetEntityID() );

					int tDamage = (int)( tDecSrcHP * pBuffTpl->mBuff[ t ].mBuffParam[ 1 ] / SERVER_PERCENT_FLOAT );
					CPropertyModule::GetSingleton().EntityDamaged( pSrcEntity, this, tDamage, RESULT_NORMAL, nSkillType );
				}
				break;
			case CTemplateBuff::BUFFFUNC_CHARGE:
				{
					CPropertyModule::GetSingleton().EntityCharge( pSrcEntity, this );
				}
				break;
			case CTemplateBuff::BUFFFUNC_SNEER:
				{
					if ( IsNpc() )
					{
						//// 通知NPC模块，NPC被嘲讽
						// 0表示进入嘲讽 1表示推出嘲讽
						CNpcModule::GetSingleton().OnSneered( pSrcEntity->GetEntityID( ), GetEntityID( ), 0 );
					}
				}
				break; 	
			case CTemplateBuff::BUFFFUNC_HITDEATHHP:
				{
					if ( (int)pDesProperty->GetCurHP( ) <= pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] )
					{
						PERF_FUNC( "EntityDie", EntityDie( pSrcEntity ) );
					}
					else
					{
						CPropertyModule::GetSingleton().EntityDamaged( pSrcEntity, this, pBuffTpl->mBuff[ t ].mBuffParam[ 1 ], RESULT_NORMAL );
					}
				}
				break;
			case CTemplateBuff::BUFFFUNC_DAMAGEDEVENTSELF:
				{
					if( vpBuffObject != NULL )
					{
						vpBuffObject->ChangeParames( pBuffTpl->mTempID, t, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
						vpBuffObject->ChangeParames2( pBuffTpl->mTempID, t, pBuffTpl->mBuff[ t ].mBuffParam[ 1 ] );
					}
					//				pBuffList->ChangeParames( pBuffTpl->mTempID, t, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
					//				pBuffList->ChangeParames2( pBuffTpl->mTempID, t, pBuffTpl->mBuff[ t ].mBuffParam[ 1 ] );
				}
				break;
				// 清除正面BUFF
			case CTemplateBuff::BUFFFUNC_CLEARBUFF:
				{
					// 清除BUFF一定要放在模板功能码的最后,不然数组下标容易出错
					CPropertyModule::GetSingleton().EntityClearbuff( this, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ], CTemplateBuff::BUFF_INC );
				}
				break;
				// 清除负面BUFF
			case CTemplateBuff::BUFFFUNC_CLEARDEBUFF:
				{
					// 清除BUFF一定要放在模板功能码的最后,不然数组下标容易出错
					CPropertyModule::GetSingleton().EntityClearbuff( this, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ], CTemplateBuff::BUFF_DEC );
				}
				break;
				// 吸收伤害,当BUFF消失时返回所吸收的伤害
			case CTemplateBuff::BUFFFUNC_SAVEDAMAGE:
				{
					if( vpBuffObject != NULL )
					{
						vpBuffObject->ChangeParames( pBuffTpl->mTempID, t, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
					}
					//				pBuffList->ChangeParames( pBuffTpl->mTempID, t, pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] );
				}
				break;
				// 范围固定伤害
			case CTemplateBuff::BUFFFUNC_RANGEDAMAGE:
				{
					// 搜索敌人, 限定个数
					MapCellSearchEnemys tSearchEnemys(  pSrcEntity, 
							pBuffTpl->mBuff[ t ].mBuffParam[ 2 ],
							pBuffTpl->mBuff[ t ].mBuffParam[ 0 ],
							NULL );

					tSearchEnemys.Visit();

					// 将技能作用于所有找到的实体
					for (unsigned int i = 0; i < tSearchEnemys.mEnemys.size(); i ++ )
					{
						// 被攻击(作用)
						tSearchEnemys.mEnemys[ i ]->AddFunResult( pSrcEntity->GetEntityID( ), FUNC_ATTACKED, RESULT_NONE, true,
								pSrcEntity->GetEntityID( ), pBuffTpl->mTempID, 0, pSrcEntity->GetPosX( ), pSrcEntity->GetPosY( ) );

						// 伤害目标
						CPropertyModule::GetSingleton().EntityDamaged( pSrcEntity, tSearchEnemys.mEnemys[ i ], pBuffTpl->mBuff[ t ].mBuffParam[ 1 ], RESULT_NORMAL );
					}
				}
				break;
			case CTemplateBuff::BUFFFUNC_ADDSKILL:
				{
					if ( IsPlayer() == true )
					{
						CEntityPlayer* tpPlayer = (CEntityPlayer*)this;
						CPropertyPlayer* tpPro = (CPropertyPlayer*)tpPlayer->GetProperty();
						CSkillList&  tpSkill = tpPro->GetSkillList( );
						tpSkill.InsertSkill( pBuffTpl->mBuff[ t ].mBuffParam[ 0 ], pBuffTpl->mBuff[ t ].mBuffParam[ 1 ] );
					}
				}
				break;
            //刷新技能cd add by yucx 20100625
            case CTemplateBuff::BUFFFUNC_REFRESHSKILLCD:
                {
                    if ( IsPlayer() == true )
                    {
                        CEntityPlayer* tpPlayer = (CEntityPlayer*)this;

                        OnBuffRefreshCD( tpPlayer, pBuffTpl->mBuff[ t ] );

                    }
                }
                break;
			case CTemplateBuff::BUFFFUNC_OBTEXP_BYTAB:
				{
					if ( IsPlayer() )
					{
						int tExp=0;
						CTemplateLevelExp* tpLevelExp = (CTemplateLevelExp*) CDataStatic::SearchTpl( pBuffTpl->mBuff[ t ].mBuffParam[ 0 ] ); 
						CEntityPlayer* tpPlayer = (CEntityPlayer*)this;
						if(NULL != tpLevelExp)
						{    
							if (tpPlayer->GetLevel( )>0 && tpPlayer->GetLevel( )<TMP_MAX_GRADE)
								tExp = (int) ( tpLevelExp->mLevelExp[ tpPlayer->GetLevel( ) ]);    
						}
						CPropertyModule::GetSingleton().PlayerObtainExp( tpPlayer, tExp, EXPREASON_BUFF);
					}
				}
				break;
			case CTemplateBuff::BUFFFUNC_BUFFAFTERBUFF:
				{
					if ( pDesProperty == NULL )
						break;

					int tSucessRate = pBuffTpl->mBuff[t].mBuffParam[0] ;
					int tSuccessTplID = pBuffTpl->mBuff[t].mBuffParam[1] ;
					int tFailTplID = pBuffTpl->mBuff[t].mBuffParam[2] ;

					int tRand = RAND(SERVER_PERCENT_INT);
					if ( tRand > tSucessRate )
					{
						//成功目标上buff
						CTemplateBuff *tpBuffTemplate = (CTemplateBuff *)CDataStatic::SearchTpl(tSuccessTplID);
						if ( tpBuffTemplate == NULL )
						{
							LOG_DEBUG( "pro", "ATTACKEVENT : can't find CTemplateBuff : BuffID ( %d )\n", pBuffTpl->mBuff[t].mBuffParam[1] );
							continue;
						}
						this->EntityInsertBuff(pSrcEntity, tpBuffTemplate);
					}
					else
					{
						//失败自己上buff
						CTemplateBuff *tpBuffTemplate = (CTemplateBuff *)CDataStatic::SearchTpl(tFailTplID);
						if ( tpBuffTemplate == NULL )
						{
							LOG_DEBUG( "pro", "ATTACKEVENT : can't find CTemplateBuff : BuffID ( %d )\n", pBuffTpl->mBuff[t].mBuffParam[1] );
							continue;
						}
						pSrcEntity->EntityInsertBuff(pSrcEntity, tpBuffTemplate);
					}
				}
				break;

			//降低HP至固定值
			case CTemplateBuff::BUFFFUNC_REDUCEHPTO:
				{		
						if ( pBuffTpl->mBuff[t].mBuffParam[0] <= 0 )
							break;
						
						int tTemp = pSrcProperty->GetCurHP() - pBuffTpl->mBuff[t].mBuffParam[0] ;
						if ( tTemp > 0 )
						{
							ExpendHP(tTemp , pSrcProperty->GetEntityID());
						}
						
				}
				break;
				
			//降低MP至固定值
			case CTemplateBuff::BUFFFUNC_REDUCEMPTO:
				{
						if ( pBuffTpl->mBuff[t].mBuffParam[0] <= 0 )
							break;

						int tTemp = pSrcProperty->GetCurMP() - pBuffTpl->mBuff[t].mBuffParam[0] ;
						if ( tTemp > 0 )
						{
							ExpendMP(tTemp, pSrcProperty->GetEntityID());
						}
				}
				break;

			default:
				break;
		}
	}
}


// 恢复HP
int CEntityCharacter::CureHP( int vCureHP, unsigned int nSrcEntityID, EResultType emType, bool bAddFunc )
{
	CPropertyCharacter* pProperty = (CPropertyCharacter*) GetProperty();
	
	// 满血就不用回了
	if( pProperty->GetCurHP() == (unsigned int)( pProperty->GetMaxHP().mInitValue + pProperty->GetMaxHP().mDeltaValue) )
	{
		return 0;
	}

	if ( vCureHP <= 0 )
		return 0;

	int tCureHP = vCureHP;
	int tOldHP = pProperty->GetCurHP();
	// 如果不是偷取HP,则计算降低治疗量
	if ( emType != RESULT_STEALHP )
	{
		if( pProperty->GetStateParam( STATE_CUREHPLESS ).mInState == true )
		{
			int tLessPer = std::min( (int)(pProperty->GetStateParam( STATE_CUREHPLESS ).mParames1), SERVER_PERCENT_INT );
			tCureHP = (int)( tCureHP / SERVER_PERCENT_FLOAT * ( SERVER_PERCENT_FLOAT - tLessPer ) );
			if ( tCureHP == 0 )
				return 0;
		}
	}

	pProperty->SetCurHP( std::min( (unsigned int)pProperty->GetMaxHP(), pProperty->GetCurHP( ) + tCureHP ) );
	int nRealChangeHP = pProperty->GetCurHP() - tOldHP;
	
	if ( bAddFunc == true )
		AddFunResult( nSrcEntityID, FUNC_INCHP, emType, true, pProperty->GetCurHP() - tOldHP, pProperty->GetCurHP(), pProperty->GetMaxHP() );

	return nRealChangeHP;
}

// 恢复MP
int CEntityCharacter::CureMP( int vCureMP, unsigned int nSrcEntityID, EResultType emType, bool bAddFunc )
{
	if ( vCureMP <= 0 )
		return 0;

	CPropertyCharacter* pProperty = (CPropertyCharacter*) GetProperty();

	int nOldMP = pProperty->GetCurMP();
	vCureMP = std::min( (unsigned int)pProperty->GetMaxMP( ), pProperty->GetCurMP( ) + vCureMP );
	pProperty->SetCurMP( vCureMP );
	int nRealChangeMP = pProperty->GetCurMP() - nOldMP;
	
	if ( bAddFunc == true )
		AddFunResult( nSrcEntityID, FUNC_INCMP, emType, true, nRealChangeMP, pProperty->GetCurMP(), pProperty->GetMaxMP() );

	return nRealChangeMP;
}

// 恢复AP
int CEntityCharacter::CureAP( int vCureAP, unsigned int nSrcEntityID, EResultType emType, bool bAddFunc )
{
	if ( vCureAP <= 0 )
		return 0;

	CPropertyCharacter* pProperty = (CPropertyCharacter*) GetProperty();

	int nOldAP = pProperty->GetCurAP();
	vCureAP = std::min( (unsigned int)pProperty->GetMaxAP( ), pProperty->GetCurAP( ) + vCureAP );
	pProperty->SetCurAP( vCureAP );
	int nRealChangeAP = pProperty->GetCurAP() - nOldAP;

	if ( bAddFunc == true )
		AddFunResult( nSrcEntityID, FUNC_INCAP, emType, true, nRealChangeAP, pProperty->GetCurAP(), pProperty->GetMaxAP() );

	return nRealChangeAP;
}

//************************************
// Method:    ExpendHP
// FullName:  CEntityCharacter::ExpendHP
// Access:    virtual public 
// Returns:   int 真正消耗的HP
// Qualifier: 消耗HP (玩家剩余HP>=0)
// Parameter: int vCureHP
// Parameter: unsigned int nSrcEntityID
// Parameter: EResultType emType
// Parameter: bool bAddFunc
//************************************
int CEntityCharacter::ExpendHP( int vCureHP, unsigned int nSrcEntityID, EResultType emType, bool bAddFunc )
{
	if ( vCureHP <= 0 )
		return 0;

	CPropertyCharacter* pProperty = (CPropertyCharacter*) GetProperty();

	int nOldHP = pProperty->GetCurHP();
	int nLeftHP = std::max( 0, nOldHP - vCureHP );
	pProperty->SetCurHP( nLeftHP );

	if ( IsPlayer( ) == true )
	{
		int tOldPer = nOldHP * 1.0 / (int)pProperty->GetMaxHP( ) * SERVER_PERCENT_INT;
		int tNewPer = pProperty->GetCurHP() * 1.0 / (int)pProperty->GetMaxHP( ) * SERVER_PERCENT_INT;
		((CPropertyPlayer*)pProperty)->InsertSelfBuff( NULL, CTemplateProperty::PROPERTYFUNC_HPSKILL, tOldPer, tNewPer );
	}
	
	// 发消息时HP消耗要填原始消耗，即使比剩余HP还大，这样玩家才显示的爽
	if ( bAddFunc == true )
		AddFunResult( nSrcEntityID, FUNC_DECHP, emType, true, vCureHP, pProperty->GetCurHP(), pProperty->GetMaxHP() );

	return nOldHP - pProperty->GetCurHP();
}


//************************************
// Method:    ExpendMP
// FullName:  CEntityCharacter::ExpendMP
// Access:    virtual public 
// Returns:   int  真正消耗的HP
// Qualifier: 消耗MP (玩家剩余HP>=0)
// Parameter: int vExpendMP
// Parameter: unsigned int nSrcEntityID
// Parameter: EResultType emType
// Parameter: bool bAddFunc
//************************************
int CEntityCharacter::ExpendMP( int vExpendMP, unsigned int nSrcEntityID, EResultType emType, bool bAddFunc )
{
	if ( vExpendMP <= 0 )
		return 0;

	CPropertyCharacter* pProperty = (CPropertyCharacter*) GetProperty();

	int tOldMP = pProperty->GetCurMP();

	// 实体MP减少,不计入伤害列表,不对经验分配比例产生改动
	int nLeftMP = std::max( (int)( pProperty->GetCurMP( ) - vExpendMP ), 0 );

	pProperty->SetCurMP( nLeftMP );

	if ( bAddFunc == true )
		AddFunResult( nSrcEntityID, FUNC_DECMP, emType, true, vExpendMP, pProperty->GetCurMP(), pProperty->GetMaxMP() );

	return tOldMP - pProperty->GetCurMP();
}

// 偷取HP
int CEntityCharacter::StealHP( int nDamage )
{
	if ( nDamage <= 0 )
		return 0;

	CPropertyCharacter* pProperty = (CPropertyCharacter*) GetProperty();

	int tStealHPPer = CPropertyFormula::GetStealHP( pProperty );

	if ( tStealHPPer > 0 )
	{
		int tStealHP = (int)(nDamage * tStealHPPer / SERVER_PERCENT_FLOAT);
		return CureHP( tStealHP, GetEntityID(), RESULT_STEALHP );
	}

	return 0;
}

// 偷取MP
int CEntityCharacter::StealMP( int nDamage )
{
	if ( nDamage <= 0 )
		return 0;

	CPropertyCharacter* pProperty = (CPropertyCharacter*) GetProperty();

	int tStealMPPer = CPropertyFormula::GetStealMP( pProperty );

	if ( tStealMPPer > 0 )
	{
		int tStealMP = (int)(nDamage * tStealMPPer / SERVER_PERCENT_FLOAT);
		return CureMP( tStealMP, GetEntityID(), RESULT_STEALMP );
	}

	return 0;
}

// 瞬间改变位置
void CEntityCharacter::ChangePos( CWTPoint vPos, EResultType emType, unsigned int nSrcEntityID )
{
	AddFunResult( nSrcEntityID, FUNC_CHANGEPOS, emType, true, vPos.mX, vPos.mY );
	
	//TODO: 这里一定要先发送一次FunResult, 否则等改完坐标以后再发的话客户端用技能时
	// 的原坐标和目的坐标对不上，会有先跑过去再表现的情况，一定要在该函数先发而且在Move之前发。
	CPropertyModule::GetSingletonPtr()->ResultListSend();
	
	CMapModule::GetSingleton().OnEntityMove( this, vPos );

	// 通知交易模块
	CTradeModule::GetSingleton().OnCancelTrade( mEntityID );
}



// 实体死亡时,清除BUFF
void CEntityCharacter::RemoveBuffEntityDie()
{
	CPropertyCharacter* pDesProperty = (CPropertyCharacter*) GetProperty();
	CBuffList *tpBuff = pDesProperty->GetBuffList( );

	// 清除所有状态 
	pDesProperty->CleanStateParam( );
	
	// 清除所有buff
	tpBuff->RemoveAllBuff( this );
}

// 怪物死亡,玩家获得经验 ( 暂时只有OGRE用 )

//****************************************************//
// CEntityRedStone
//**************************************************// 
bool CEntityRedStone::Setup( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator* pCreator, CWTPoint* pPos, bool bInit, int vNpcID ) 
{
	CRedstondeCreator *tpCreator = ( CRedstondeCreator * )pCreator;
	
	CWTPoint tOrginPos = CWTPoint( tpCreator->mCenterX, tpCreator->mCenterY );
	// 如果是被动掉落，要算一个随机位置
	if ( tpCreator->mDropType == DROP_OBJECTIVE )
	{
		CMapModule::GetSingletonPtr()->GetDropPos( usLineID, usMapID, usMapIndex, tOrginPos );
	}
	//TODO: 暂时没有，等待客户端上报 . 如果是主动掉落，判断掉落的位置
	//else if ( tpCreator->mDropType == DROP_SUBJECTIVE )
	//{
	//	if ( CMapModule::GetSingletonPtr()->IsStaticBlock( usLineID, usMapID, usMapIndex, tOrginPos ) > 0 )
	//	{
	//		return false;
	//	}
	//	else if ( CMapModule::GetSingletonPtr()->IsOccupied( usLineID, usMapID, usMapIndex, tOrginPos, ENTITYTYPE_ITEM ) )
	//	{
	//		return false;
	//	}
	//}	
	
	mLineID = usLineID;
	mMapID  = usMapID;
	mIndex = usMapIndex;	
	SetEntityType( ENTITYTYPE_REDSTONE );	
	mPos = tOrginPos;
	mDirection = DIR_DOWN;
	mAliveState = ALIVE_NORMAL;	
	memcpy( mOwnerID,tpCreator->GetOwnerID(), sizeof( mOwnerID ) );	
	mTeamID = 0;
	mProcessTimerID = 0;
	GetProperty()->SetPropertyTempID( tpCreator->mItemID );
	GetProperty()->SetEntityID( this->get_id() );	
	SetEntityID( this->get_id() );	

	tpCreator->mHasOgre = GetEntityID();
	return true;
}
bool CEntityRedStone::CheckIfOwner( int vCharID )
{
	if ( mStatus == (int)REDSTONE_STATUS_PROTECT )
	{
		for( int i = 0; i < TEAMNUM ; i++ )
		{
			if( vCharID == (int)mOwnerID[i] )
			{
				return true;
			}
		}
		return false;
	}
	else if ( mStatus == (int)REDSTONE_STATUS_DROP )
	{
		return true;
	}
	else
	{
		return false;
	}
	
} 

// 陷阱触发
bool CEntityPedal::IsTouch( int vTickOffset )
{
	//if ( GetPedalType() == PEDAL_TELEPORT )
	//{
	//	// 传送踏板
	//	TeleAction( vTickOffset );
	//}
	//else
	if ( GetPedalType() == PEDAL_TRAP )
	{
		// 陷阱
		return TrapAction( );
	}
	else if( GetPedalType() == PEDAL_HIEROGRAM )
	{
		// 神符
		return HierogramAction( );
	}
	else if( GetPedalType() == PEDAL_NEEDFIRE )
	{
		return NeedfireAction();
	}
	else
	{
		return true;
	}

	return false;
}

// 实体是否处于陷阱/传送点上
bool CEntityPedal::IsTouchOn( CEntity* pEntity )
{
	if( mSrcRect.TopLeft().mX <= pEntity->GetPosX()
		&& mSrcRect.TopLeft().mY <= pEntity->GetPosY()
		&& mSrcRect.BottomRight().mX >= pEntity->GetPosX()
		&&  mSrcRect.BottomRight().mY >= pEntity->GetPosY() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 传送动作
void CEntityPedal::TeleAction( int vTickOffset )
{
	// 传送踏板每500毫秒触发一次,可以调整
	if ( GetDelayTimer() > 0 )
	{
		unsigned int tTimer = std::max( (int)( GetDelayTimer() - vTickOffset ), 0 );
		SetDelayTimer( tTimer );
		if ( GetDelayTimer() > 0 )
			return;
	}

	SetDelayTimer( 500 );
#ifdef _FB_
	CMapObject* tpMapObj = CMapObjManager::GetSingletonPtr( )->GetMapObject( GetLineID( ), GetMapID( ), 
		GetMapIndex( ) );
#endif

	// 搜索踏板上的实体
	MapCellPedalSearcher tPedalSearcher(this);
	tPedalSearcher.Visit();

	for ( std::vector<CEntityCharacter*>::iterator it = tPedalSearcher.mEntitysOnPedal.begin();
		it != tPedalSearcher.mEntitysOnPedal.end();
		++ it)
	{
		CEntityPlayer* tpPlayer = (CEntityPlayer*) (*it);

		if( tpPlayer->GetNationality( ) == Zero	)
			continue;

#ifndef _FB_
		CMapModule::GetSingletonPtr()->Teleport( GetDstLineID(), GetDstMapID(), 
			GetDesPos().mX, GetDesPos().mY, tpPlayer );
#else
		if( tpMapObj == NULL )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Find Teleport( %d ) Map( %d : %d : %d )", __LK_FILE__,
				__LINE__, __FUNCTION__, GetEntityID( ),  GetLineID( ), GetMapID( ), GetMapIndex( ) );
			continue;
		}
		CMapModule::GetSingletonPtr()->Teleport( GetDstLineID(), GetDstMapID(), GetDesPos().mX, GetDesPos().mY, 
			tpPlayer, INVALID_MAP_INDEX, ( tpMapObj != NULL ) ? tpMapObj->GetKey( ) : INVITED_KEY_VALUE );
#endif
	}
}

// 陷阱动作
bool CEntityPedal::TrapAction( )
{
	CTemplateTrapSkill* tpTrapTpl = (CTemplateTrapSkill*) CDataStatic::SearchSkill( GetSkillID( ), GetSkillLevel( ) );
	if ( tpTrapTpl == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] can't find tripskill template SkillID( %d ) SkillLevel( %d )", __LK_FILE__, __LINE__, __FUNCTION__, GetSkillID( ), GetSkillLevel( ) );
		return false;
	}

	MapCellPedalSearcher tPedalSearcher(this);	

	CEntityCharacter* tpOwner = (CEntityCharacter*) GetOwner();
	LK_ASSERT ( tpOwner != NULL, return false );

	tPedalSearcher.Visit( );
	if ( tPedalSearcher.mEntitysOnPedal.size() == 0 )
		return false;

	// 计算攻击序列号
	AddAttackCount( );
	InitFunResult( GetAttListID( ) );

	int nEntityFunNum = 0; // 作用实体数

	tPedalSearcher.Visit2( );
	for ( std::vector<CEntityCharacter*>::iterator it = tPedalSearcher.mEntitysOnPedal.begin();
		it != tPedalSearcher.mEntitysOnPedal.end() && nEntityFunNum < PDEAL_LIMIT; 
		++ it )
	{
		CEntityCharacter* pEntityCharctor = *it;

		// 判断是否能够触发
		if ( CPropertyModule::GetSingleton().EntityPKValid( tpOwner, pEntityCharctor, tpTrapTpl ) )
		{
			// 陷阱buff
			PedalBuffSkill( tpOwner, pEntityCharctor, tpTrapTpl );

			// 如果是单体作用,并且已经作用,则退出
			// 无论单体攻击还是范围攻击,都需要作用列表中的第一个实体
			if ( ++nEntityFunNum > 0 && tpTrapTpl->mApplyType == CTemplateTrapSkill::APPLYTYPE_SINGLE )
				break;
			if ( nEntityFunNum >= tpTrapTpl->mEffectMaxNumber )
				break;
		}
	}

	if( nEntityFunNum > 0 )
	{
		// 陷阱功能码
		AddFunResult( GetEntityID(), FUNC_TRAPONTOUCH, RESULT_NONE, true, GetSkillID( ), GetSkillLevel( ), GetPosX(), GetPosY() );

		// 清除陷阱
		CSceneLogic::GetSingleton().AddDeleteList( this->GetEntityID() );

		CPropertyCharacter* tpProOwner = (CPropertyCharacter*) tpOwner->GetProperty();
		for ( int i = 0; i < tpProOwner->GetTripEntityIDListSize( ); i++ )
		{
			if ( tpProOwner->GetTripEntityIDList( i ).mTripEntityID == (int) GetEntityID() )
			{
				tpProOwner->GetTripEntityIDList( i ).Clean();
			}
		}

		// 发送作用消息
		CPropertyModule::GetSingleton( ).ResultListSend( );
	}
	else
	{
		//陷阱未作用
		return false;
	}

	return true;
}

// 神符动作
bool CEntityPedal::HierogramAction( )
{
	MapCellPedalSearcher tPedalSearcher(this);
	tPedalSearcher.Visit();

	for ( std::vector<CEntityCharacter*>::iterator it = tPedalSearcher.mEntitysOnPedal.begin();
		it != tPedalSearcher.mEntitysOnPedal.end() ;
		++ it )
	{
		CEntityCharacter* tpEntityCharactor = *it;

		// 判断是否能够触发
		if ( tpEntityCharactor->IsPlayer() )
		{
			// 陷阱作用
			OnEntityHierogram( (CEntityPlayer*) tpEntityCharactor );

			// 清除陷阱
			CSceneLogic::GetSingleton().AddDeleteList( this->GetEntityID() );

			return true;
		}
	}

	return false;
}


/* --------------------------------------------------------------------------*/
/* 
* @Synopsis 火焰动作
*/ 
/* ----------------------------------------------------------------------------*/
bool CEntityPedal::NeedfireAction( )
{

	CTemplateTrapSkill* tpTrapTpl = (CTemplateTrapSkill*) CDataStatic::SearchSkill( GetSkillID( ), GetSkillLevel( ) );
	if ( tpTrapTpl == NULL )
	{
		LOG_ERROR( "pro", "[ %s : %d ] [ %s ] can't find tripskill template SkillID( %d ) SkillLevel( %d )", __LK_FILE__, __LINE__, __FUNCTION__, GetSkillID( ), GetSkillLevel( ) );
		return false;
	}

	MapCellPedalSearcher tPedalSearcher(this);	


	tPedalSearcher.Visit( );
	if ( tPedalSearcher.mEntitysOnPedal.size() == 0 )
		return false;

	// 计算攻击序列号
	AddAttackCount( );
	InitFunResult( GetAttListID( ) );

	int nEntityFunNum = 0; // 作用实体数

	tPedalSearcher.Visit2( );
	for ( std::vector<CEntityCharacter*>::iterator it = tPedalSearcher.mEntitysOnPedal.begin();
		it != tPedalSearcher.mEntitysOnPedal.end() && nEntityFunNum < PDEAL_LIMIT; 
		++ it )
	{
		CEntityCharacter* pEntityCharctor = *it;

		PedalBuffSkill( pEntityCharctor, pEntityCharctor, tpTrapTpl );
	}

	return true;
}


void CEntityPedal::OnEntityHierogram( CEntityPlayer* pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return );
	
	CTplHierogram* tpTpl = ( CTplHierogram* )CDataStatic::SearchTpl( mNpcID );
	LK_ASSERT_STR( tpTpl != NULL, return, "TempID %d", mNpcID );

	if( tpTpl->mType == CTplHierogram::USE_SKILL )
	{
		CTemplateSkill* tpSkill = (CTemplateSkill*)CDataStatic::SearchSkill( tpTpl->mParame[ 0 ], tpTpl->mParame[ 1 ] );
		LK_ASSERT_STR( tpSkill != NULL, return, "TempID %d, SkillID %d, SkillLevel %d", mNpcID, tpTpl->mParame[ 0 ],
			tpTpl->mParame[ 1 ] );

		// 计算攻击序列号
		AddAttackCount( );
		InitFunResult( GetAttListID( ) );
		CPropertyModule::GetSingletonPtr( )->UseSkillForServer( pPlayer, pPlayer, tpSkill, NULL, 0 );

		CPropertyModule::GetSingleton( ).ResultListSend( );
	}
}

// 陷阱BUFF作用
void CEntityPedal::PedalBuffSkill( CEntityCharacter* pSrcEntity, CEntityCharacter* pDesEntity, CTemplateTrapSkill* pTrapSkill, int vSkillType )
{
	if ( pSrcEntity != NULL && pDesEntity != NULL && pTrapSkill != NULL )
	{
		EResultType tResult = RESULT_NORMAL;
		int tDamage = CPropertyFormula::GetPSkillAttack( pSrcEntity, pDesEntity, pTrapSkill, tResult );
		
		// 计算技能添加Buff
		CPropertyModule::GetSingletonPtr()->EntityBuffSkill( pSrcEntity, pDesEntity, pTrapSkill->mBuffGroupID, ARRAY_CNT(pTrapSkill->mBuffGroupID), vSkillType, tDamage );
	}

}

// 手动删除BUFF
bool CEntityCharacter::ActiveRemoveBuff( int nBuffID )
{
	CTemplateBuff* tpBuffTpl = (CTemplateBuff*) CDataStatic::SearchTpl( nBuffID );
	if ( tpBuffTpl == NULL )
	{
		LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] CTemplateBuff cann't find, BuffID = %d", __LK_FILE__, __LINE__, __FUNCTION__, nBuffID );
		return false;
	}

	// 不能主动清除
	if ( tpBuffTpl->mCancelBuff == CTemplateBuff::BUFF_NOTCANCEL )
		return false;

	int tnRtn = EntityRemoveBuff( nBuffID );

	//LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] Remove Buff! for ActiveRemoveBuff error test!", __LK_FILE__, __LINE__, __FUNCTION__ );
	if ( tnRtn < 0 )
	{
		LOG_DEBUG( "pro", "[ %s : %d ] [ %s ] BUFF Active Remove Error! ErrorCode = %d, BuffID = %d", __LK_FILE__, __LINE__, __FUNCTION__, tnRtn, nBuffID );
		return false;
	}

	return true;
}

// 是否可以通过
bool CEntityCharacter::CanPass( unsigned int nPosX, unsigned int nPosY )
{
	if ( IsPlayer() || IsPet() )
	{
		return CMapModule::GetSingleton().CanPlayerPass( GetLineID(), GetMapID(), GetMapIndex(), nPosX, nPosY, CanFly() );
	}	
	else if ( IsOgre() || IsFunNpc() )
	{
		return CMapModule::GetSingleton().CanNpcPass( GetLineID(), GetMapID(), GetMapIndex(), nPosX, nPosY, CanFly() );
	}
	return false;
}


// 实体是否有buff
bool CEntityCharacter::HasBuff( int nBuffID ) 
{
	CPropertyCharacter* pProCharacter = (CPropertyCharacter*)GetProperty();

	// buff列表不可能为NULL
	return pProCharacter->GetBuffList()->HasBuff( nBuffID);

}

bool CEntityCharacter::CanFly( )
{
	bool tState = false;
	if ( IsPlayer() || IsPet() )
	{		
		if ( IsPlayer() )
		{
			CEntityPlayer *tpPlayer = (CEntityPlayer*)this;
			CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
			tState = tpProperty->GetStateParam( STATE_FLY ).mInState;
		}
		else if ( IsPet() )
		{
			CEntityPet *tpPet = (CEntityPet*)this;
			CPropertyPet *tpProperty = (CPropertyPet*)tpPet->GetProperty();
			tState = tpProperty->GetStateParam( STATE_FLY ).mInState;
		}

		return tState;
	}	
	else if ( IsOgre() || IsFunNpc() )
	{
		if ( IsOgre() )
		{
			CEntityOgre *tpNpc = (CEntityOgre*)this;
			CPropertyNPC *tpProperty = (CPropertyNPC*)tpNpc->GetProperty();
			tState = tpProperty->GetStateParam( STATE_FLY ).mInState;
		}	
		else if ( IsFunNpc() )
		{
			CEntityFuncNpc *tpNpc = (CEntityFuncNpc*)this;
			CPropertyNPC *tpProperty = (CPropertyNPC*)tpNpc->GetProperty();
			tState = tpProperty->GetStateParam( STATE_FLY ).mInState;
		}
		return tState;
	}
	return false;
}

//************************************
// Method:    BuffTimeEvent
// FullName:  CEntityCharacter::BuffTimeEvent
// Access:    public 
// Returns:   void
// Qualifier: buff作用在实体上
// Parameter: CTemplateBuff * tpBuffTpl
// Parameter: CBuffObject * tpBuff
//************************************
void CEntityCharacter::BuffTimeEvent( CTemplateBuff* tpBuffTpl, CBuffObject* tpBuff )
{
	for ( int j = 0; j < (int)ARRAY_CNT( tpBuffTpl->mBuff ) && IsAlive( ); j ++ )
	{
		if ( tpBuffTpl->mBuff[ j ].mBuffFunc == 0 )
			break;

		switch( tpBuffTpl->mBuff[ j ].mBuffFunc )
		{
			// 持续回血
		case CTemplateBuff::BUFFFUNC_PERSISTINCFIXHP:
			{
				float tParam1 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mHotparam1 / SERVER_PERCENT_FLOAT;
				float tParam2 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mHotparam2 / SERVER_PERCENT_FLOAT;
				int tCure = tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] + tParam1 * pow( tParam2, tpBuffTpl->mSkillLevel ) * tpBuff->mAttack / ( tpBuffTpl->mLastTime / 1000 );
				CureHP( tCure );
			}
			break;
			// 持续回蓝
		case CTemplateBuff::BUFFFUNC_PERSISTINCFIXMP:
			{
				CureMP( tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] );
			}
			break;
			// 持续伤害
		case CTemplateBuff::BUFFFUNC_PERSISTDECFIXHP:
			{
				float tParam1 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDotparam1 / SERVER_PERCENT_FLOAT;
				float tParam2 = CSceneCfgManager::GetSingleton().GetFormula_cfg()->mDotparam2 / SERVER_PERCENT_FLOAT;
				int tDamage = tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] + tParam1 * pow( tParam2, tpBuffTpl->mSkillLevel ) * tpBuff->mAttack / ( tpBuffTpl->mLastTime / 1000 );

				CEntityCharacter* pBuffSrcEntity = (CEntityCharacter*)CSceneLogic::GetSingletonPtr()->GetEntity( tpBuff->GetBuffSrcEntityID() );

				PERF_FUNC( "EntityDamage", CPropertyModule::GetSingletonPtr()->EntityDamaged( pBuffSrcEntity, this, tDamage, RESULT_NORMAL ) );
			}
			break;
			// 持续按百分比伤害
		case CTemplateBuff::BUFFFUNC_PERSISTDECPERHP:
			{

				CPropertyCharacter* tpProperty = (CPropertyCharacter*) GetProperty( );
				int tDamage = (int) ( ( (int)tpProperty->GetMaxHP( ) ) * (tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] / SERVER_PERCENT_FLOAT) );
				CEntityCharacter* pBuffSrcEntity = (CEntityCharacter*)CSceneLogic::GetSingletonPtr()->GetEntity( tpBuff->GetBuffSrcEntityID() );
				PERF_FUNC( "EntityDamage", CPropertyModule::GetSingletonPtr()->EntityDamaged( pBuffSrcEntity, this, tDamage, RESULT_NORMAL ) );
			}
			break;
			// 持续减蓝
		case CTemplateBuff::BUFFFUNC_PERSISTDECFIXMP:
			{
				ExpendMP( tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ], tpBuff->GetBuffSrcEntityID() );

				// 如果蓝耗光了,就删除BUFF,需要是正面BUFF才能删除
				if ( (int) ((CPropertyCharacter*)GetProperty())->GetCurHP() < tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] 
					&& ( tpBuffTpl->mBuffType == CTemplateBuff::BUFF_INC ) )
					{
						tpBuff->mPersistTime = 0;
					}
			}
			break;
			// 离开中debuff时所处范围会受到特殊效果
		case CTemplateBuff::BUFFFUNC_LEAVEAREA:
			{
				CWTPoint tPoint( tpBuff->mParames[ j ] >> 16, tpBuff->mParames[ j ] & 0x0000FFFF );
				if( GetPos( ).LimitDistance( tPoint, tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] ) == false )
				{
					//LOG_DEBUG( "pro", "[ %s : %d ][ %s ]Entity( %d ) Buff LeveEvent, InsertBuff Pos( %d"
					//	" : %d ), Now Pos(%d : %d ), Distance = %d, BuffPersistTime=%d", __LK_FILE__,
					//	__LINE__, __FUNCTION__, GetEntityID(), tPoint.mX, tPoint.mY, GetPosX( ),
					//	GetPosY( ), tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ], tpBuff->mPersistTime );

					CTemplateBuff* tpBuffTemplate = (CTemplateBuff*)CDataStatic::SearchTpl( 
						tpBuffTpl->mBuff[ j ].mBuffParam[ 1 ] );

					if( tpBuffTemplate != NULL )
					{
						CEntityCharacter* pBuffSrcEntity = (CEntityCharacter*)CSceneLogic::GetSingletonPtr()->GetEntity( tpBuff->GetBuffSrcEntityID() );
						EntityInsertBuff( pBuffSrcEntity, tpBuffTemplate );
					}
					tpBuff->mPersistTime = 0;
				}
				else
				{
					tpBuff->mLastInterval = 0;
				}
			}
			break;
			// 持续按MaxHP百分比恢复CurHP
		case CTemplateBuff::BUFFFUNC_PERSISTINCPERHP:
			{
				// 只有character才有HP
				CPropertyCharacter* tpProperty = (CPropertyCharacter*) GetProperty( );
				int tIncHP = (int) ( ( (int)tpProperty->GetMaxHP( ) ) * (tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] / SERVER_PERCENT_FLOAT) );
				CureHP( tIncHP );

			}
			break;
			// 持续按MaxMP百分比恢复CurMP
		case CTemplateBuff::BUFFFUNC_PERSISTINCPERMP:
			{
				// 只有character才有HP
				CPropertyCharacter* tpProperty = (CPropertyCharacter*) GetProperty( );
				int tIncMP = (int) ( ( (int)tpProperty->GetMaxMP( ) ) * (tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ] / SERVER_PERCENT_FLOAT) );
				CureMP( tIncMP );
			}
			break;
			// 持续增加经验固定值
		case CTemplateBuff::BUFFFUNC_PERSISTINCFIXEXP:
			{
				// 只有玩家才能上获得经验buff
				if( IsPlayer() != true )
				{
					break;
				}
				CEntityPlayer* pPlayer = (CEntityPlayer*)this;
				int nCharID = pPlayer->GetCharID();

				int nFinalExp = CPropertyModule::GetSingletonPtr()->PlayerObtainExp( 
								pPlayer, tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ], 1, &nCharID, 1 );

				if ( nFinalExp > 0)
				{
					LogEventGetExpByBuff(pPlayer, tpBuffTpl->mBuff[ j ].mBuffParam[ 0 ], nFinalExp, tpBuffTpl->mTempID);
				}

				break;
			}
		
		// 持续增加军团荣耀值
		case CTemplateBuff::BUFFFUNC_INCCORPSGORY:
			{
				// 只有玩家才能上获得经验buff
				if( IsPlayer() != true )
				{
					break;
				}
				CEntityPlayer* pPlayer = (CEntityPlayer*)this;
				CCorpsModule::GetSingleton().ChangeCorpsGlory( pPlayer, tpBuffTpl->mBuff[j].mBuffParam[0] );
				break;
			}	
	
		// 持续增加玩家功勋
		case CTemplateBuff::BUFFFUNC_INCAP:
			{
				// 只有玩家才能上获得经验buff
				if( IsPlayer() != true )
				{
					break;
				}
				CEntityPlayer* pPlayer = (CEntityPlayer*)this;	
				CPropertyModule::GetSingleton().OnPlayerObtainHonor( pPlayer, tpBuffTpl->mBuff[j].mBuffParam[0] );
				break;
			}
		default:
			break;
		}
	}
}


// 是否可以通过
bool CEntityPlayer::CanPass( unsigned int nPosX, unsigned int nPosY )
{
	return CMapModule::GetSingleton().CanPlayerPass( GetLineID(), GetMapID(), GetMapIndex(), nPosX, nPosY, mProperty.GetStateParam( STATE_FLY ).mInState );
}

// 是否可以通过
bool CEntityPet::CanPass( unsigned int nPosX, unsigned int nPosY )
{
	return CMapModule::GetSingleton().CanPlayerPass( GetLineID(), GetMapID(), GetMapIndex(), nPosX, nPosY, mProperty.GetStateParam( STATE_FLY ).mInState );
}

int CEntityPlayer::SpeedCheck( bool vIsCoerce )
{
	unsigned int tTickNow = CSceneServer::GetSingletonPtr( )->GetLastTickCount( );
	bool tNowCheck = false;

	if( vIsCoerce == false )
	{
		if( ( unsigned int )GetMoveDistance( ) >= 
			( unsigned int )( CSceneCfgManager::GetSingletonPtr( )->GetScenelogic_cfg( )->mMovecheckdistance ) )
		{
			tNowCheck = true;
		}
		else if( tTickNow > GetSpeekCheckTime( ) + 
			CSceneCfgManager::GetSingletonPtr( )->GetScenelogic_cfg( )->mMovechecktimespace )
		{
			if( GetMoveDistance( ) != 0 )
			{
				tNowCheck = true;
			}
			else
			{
				ResetSpeedCheck( );	
			}
		}
	}
	else
	{
		// 如果是由时间触发的话,必须大于15格才检查一次
		if( GetMoveDistance( ) >= 15 )
		{
			tNowCheck = true;
		}
		else
		{
			ResetSpeedCheck( );
		}
	}

	if( tNowCheck == true )
	{
		unsigned int tTime = ( GetSpeekCheckTime( ) < tTickNow ) ? ( tTickNow - GetSpeekCheckTime( ) ) : 0;
		CPropertyPlayer* tpPlayer = ( CPropertyPlayer* )GetProperty( );
		int tSpeed = tpPlayer->GetSpeed( );
		tSpeed = ( tSpeed > 30 ) ? tSpeed - 30 : tSpeed;

		int tLength = GetMoveDistance( );
		// 速度检查是否有错
		if( tTime / tLength < ( unsigned int )tSpeed )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ]Player( %s ) Speed Error, speed( %d ) time( %d ) distance( %d )",
				__LK_FILE__, __LINE__, __FUNCTION__, GetCharNameShow( ), tSpeed+30, tTime, GetMoveDistance( ) );
					
			// 拉回来
//			CMapModule::GetSingletonPtr()->Teleport( GetLineID(), GetMapID(),  mLastCheckPoint.mX, mLastCheckPoint.mY, this );
		
//			ResetSpeedCheck( );
			SpeedCheckTeleport( );
			return SUCCESS;
		}

		LOG_DEBUG( "default", "[ %s : %d ][ %s ]Player(%s) Move Check, distance( %d ),time( %d ), server speed( %d )", 
			__LK_FILE__, __LINE__, __FUNCTION__, this->GetCharNameShow(), GetMoveDistance( ), tTime, tSpeed );
		

		ResetSpeedCheck( );
	}
	
	return SUCCESS;
}
 
int CEntityPlayer::SpeedCheckTeleport( )
{
	if( GetPosX( ) != mLastCheckPoint.mX || GetPosY( ) != mLastCheckPoint.mY )
	{
		CMapModule::GetSingletonPtr()->Teleport( GetLineID(), GetMapID(),  mLastCheckPoint.mX, mLastCheckPoint.mY, 
			this );
	}
	ResetSpeedCheck( );
	return SUCCESS;
}

int CEntityPlayer::ResetSpeedCheck( )
{
	mMoveDistance = 0; 
	mSpeedCheckTime = CSceneServer::GetSingletonPtr( )->GetLastTickCount( );
	mLastCheckPoint = GetPos();
	return SUCCESS;
}


// 死亡中断活动处理
void CEntityCharacter::InterruptActivity()
{
	if ( IsPlayer() == false )
	{
		return ;
	}

	// 交易取消
	CTradeModule::GetSingleton().OnCancelTrade( mEntityID );

	// TODO

	return ;
}

/*
void CEntityBlock::EntityDie( CEntityCharacter* pSrcEntity, bool bKillSelf, bool vIsAward )
{
	CEntityOgre::EntityDie( pSrcEntity, bKillSelf, vIsAward );

	CTemplateBlock* tpTpl = ( CTemplateBlock* )CDataStatic::SearchTpl( GetNpcID( ) );
	CMapObject* tpMapObj = GetMapObject( );
	
	LK_ASSERT_STR( tpTpl != NULL, return, "TemplateID %d", GetNpcID( ) );
	LK_ASSERT_STR( tpMapObj != NULL, return, "MapID %d, LineID %d, Index %d", GetMapID( ), GetLineID( ), 
		GetMapIndex( ) );

	for( int i = 0; i <= ( int )ARRAY_CNT( tpTpl->mPos ); i++ )
	{
		if( tpTpl->mPos[ i ].mX == 0 && tpTpl->mPos[ i ].mY == 0 )
		{
			break;
		}

		int tX = GetPosX( ) + tpTpl->mPos[ i ].mX;
		int tY = GetPosY( ) + tpTpl->mPos[ i ].mY;

		tpMapObj->RemoveBlock( tX, tY );
	}
}

*/
void CEntityBlock::RemoveBlock( )
{
	CTemplateBlock* tpTpl = ( CTemplateBlock* )CDataStatic::SearchTpl( GetNpcID( ) );
	LK_ASSERT_STR( tpTpl != NULL, return, "Cannt GetTpl %d, EntityID %d", GetNpcID( ),
			GetEntityID( ) );

	CMapObject* tpMapObj = GetMapObject( );
	LK_ASSERT_STR( tpMapObj != NULL, return, "Cannt Get Entity( %d ) Map( %d : %d : %d )", GetEntityID( ), GetLineID( ),
		GetMapID( ), GetMapIndex( ) );

	unsigned int i = 0;

	for( i = 0; i < ARRAY_CNT( tpTpl->mPos ); i++ )
	{
		if( tpTpl->mPos[ i ].mX == 0 && tpTpl->mPos[ i ].mY == 0 )
		{
			break;
		}

		int tX = GetPosX( ) + tpTpl->mPos[ i ].mX;
		int tY = GetPosY( ) + tpTpl->mPos[ i ].mY;

		tpMapObj->RemoveBlock( tX, tY );
	}
}

int CEntityBlock::ChangeMode( )
{
	if( IsAlive( ) == true )
	{
		( ( CPropertyCharacter* )GetProperty( ) )->SetCurHP( 0 );
		EntityDie( this, true );
	}
	else
	{
		CPropertyModule::GetSingletonPtr( )->EntityRelive( this );
	}

	return SUCCESS;
}


int CEntityCharacter::SetCampID( int vCampID )
{
#ifdef _FB_
	CMessage tMsg;
	CMessageChangeCamp tChangeCamp;
	
	tChangeCamp.set_entityid( GetEntityID( ) );
	tChangeCamp.set_campid( vCampID );

	tMsg.mutable_msghead()->set_messageid( ID_S2C_CHANGE_CAMP );
	tMsg.set_msgpara( ( long ) &tChangeCamp );
	if( IsNpc( ) == true )
	{
		tChangeCamp.set_tempid( this->GetNpcID( ) );
	}
	if( IsPlayer( ) == true )
	{
		tChangeCamp.set_charid( ( ( CEntityPlayer* )this )->GetCharID( ) );
	}

	CMapObject* tpMapObj = GetMapObject( );
	LK_ASSERT_STR( tpMapObj != NULL, return ERR_UNKNOWN, "MapID %d, MapIndex %d", GetMapID( ), GetMapIndex( ) );

	tpMapObj->SendMsg2Player( &tMsg );
#endif
	return SUCCESS;
}

#ifdef _FB_
int CEntityPlayer::GetCampID( )
{
	CPlayerRepetion* tpRepetion = ( ( CPropertyPlayer* )GetProperty( ) )->GetRepetion( );
	if( CRepetionModule::GetSingletonPtr( )->IsBattleMap( GetMapID( ) ) == true )
	{
		return tpRepetion->GetCampID( );
	}

	return 0;
}
#endif

bool CEntityPlayer::RefrushPingTime( )
{
	unsigned int tTime = CSceneServer::GetSingletonPtr( )->GetLastTickCount( );
	if( ( tTime - mPingTime ) < ( _PLAYER_PING_TIME_MAX_ - 200 ) )
	{
		LOG_NOTICE( "default", "[ %s : %d ][ %s ] Player( %d : %d : %s ) PING Time LONG, Old %ud, Now %ud, Max %ud", 
			__LK_FILE__, __LINE__, __FUNCTION__, GetEntityID( ), GetCharID( ), GetCharNameShow( ), mPingTime, tTime, 
			_PLAYER_PING_TIME_MAX_ );
		mPingTime = tTime;
		return false;
	}

	mPingTime = tTime;

	return true;
}

int CEntityPlayer::GoldenNotice( )
{
    CMessageQueryGoldenYBResponse tPara;
    tPara.set_goldenyb( GetGoldenYB( ) );
    CMessage tMsg;
    tMsg.set_msgpara((uint32_t) &tPara);
	tMsg.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERY_GOLDENYB ); 
    CSceneLogic::GetSingleton().Send2Player( this, &tMsg);

	return SUCCESS;
}

bool CEntityPlayer::CanRemainFly( )
{
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)this->GetProperty();
	if ( tpProperty->IsVip() == false )
	{
		CTplItemHorse *tplHorse = (CTplItemHorse*)CDataStatic::SearchTpl( tpProperty->GetHorseTempID( ) );
		if ( tplHorse != NULL )
		{
			if ( ((int)tpProperty->GetCurAP()) < tplHorse->mDeltaAP )
			{
				return false;
			}
		}
	}
	return true;
}

// 和功能NPC是否友好
bool CEntityFuncNpc::IsFriendlyTo( CEntity* pEntity )
{
	LK_ASSERT( pEntity != NULL, return true );

	// 如果是自己
	if( this == pEntity )
	{
		return true;
	}

	switch ( pEntity->GetEntityType() )
	{
	case ENTITYTYPE_PLAYER:
	case ENTITYTYPE_PET:
		{
			// 如果是同国
			if ( GetOwnCampID() != 0 && pEntity->GetNationality() == GetOwnCampID() )
			{
				return true;
			}
			if ( GetNationality() == pEntity->GetNationality() ) 
			{
				if( ( mPKType & FRIEND_TO_SC ) == FRIEND_TO_SC || ( mPKType & FRIEND_WITH_SC ) == FRIEND_WITH_SC )
				{
					return true;
				}
			}
			// 如果是其他国
			else
			{
				if( ( mPKType & FRIEND_WITH_FC ) == FRIEND_WITH_FC || ( mPKType & FRIEND_TO_FC ) == FRIEND_TO_FC )
				{
					return true;
				}
			}
			break;
		}
	case ENTITYTYPE_OGRE:
	case ENTITYTYPE_FUNCNPC:
		{
			CEntityNpc *pNpc = (CEntityNpc *)pEntity; 
			if ( mGroupID == pNpc->GetGroupID() )
				return true;
			if ( ( mPKType & ENEMY_WITH_OGRE ) == ENEMY_WITH_OGRE )
			{
				return false;
			}
			break;
		}
	default:
		break;
	}

	return false;
}

bool CEntityCharacter::CanBeTransformed( )
{
	if ( IsPlayer() == true )
	{
		CEntityPlayer *tpPlayer = (CEntityPlayer*)this;
		CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
		int tStateID = tpProperty->GetStateParam( STATE_TRANSFORM ).mParames1;
		CTemplateNpc *tplNpc = (CTemplateNpc*)CDataStatic::SearchTpl( tStateID );
		if ( tplNpc != NULL && tplNpc->mResistTran == 0 )
		{
			return false;
		}
		CTemplateEquipment *tplEquip = NULL;
		CItemEquipment* tpWeapon	= (CItemEquipment*) tpProperty->GetEquipment( )->GetItemObjPtr( EQUIPMENT_WEAPON );
		if ( tpWeapon != NULL && tpWeapon->IsOvertimed() == false )
		{
			tplEquip = (CTemplateEquipment*)CDataStatic::SearchTpl( tpWeapon->GetItemID() );
			if ( tplEquip != NULL && tplEquip->mResistTran == 0 )
			{
				return false;
			}
		}
		CItemEquipment* tpChest		= (CItemEquipment*) tpProperty->GetEquipment( )->GetItemObjPtr( EQUIPMENT_CHEST );
		if ( tpChest != NULL && tpChest->IsOvertimed() == false )
		{
			tplEquip = (CTemplateEquipment*)CDataStatic::SearchTpl( tpChest->GetItemID() );
			if ( tplEquip != NULL && tplEquip->mResistTran == 0 )
			{
				return false;
			}
		}
		CItemEquipment* tpFashion	= (CItemEquipment*) tpProperty->GetEquipment( )->GetItemObjPtr( EQUIPMENT_FASHION );
		if ( tpFashion != NULL && tpFashion->IsOvertimed() == false )
		{
			tplEquip = (CTemplateEquipment*)CDataStatic::SearchTpl( tpFashion->GetItemID() );
			if ( tplEquip != NULL && tplEquip->mResistTran == 0 )
			{
				return false;
			}
		}
	}
	if ( IsNpc() == true )
	{
		CEntityNpc *tpNpc = (CEntityNpc*)this;
		CTemplateNpc *tplNpc = (CTemplateNpc*)CDataStatic::SearchTpl( tpNpc->GetNpcID() );
		if ( tplNpc != NULL && tplNpc->mResistTran == 0 )
		{
			return false;
		}
	}
	return true;
}

/* --------------------------------------------------------------------------*/
/** 
 * * @Synopsis 刷新技能添加buff 
 * * 
 * * @Param tpPlayer
 * * @Param buffFunc CTemplateBuff内部类型
 * */
/* ----------------------------------------------------------------------------*/

void CEntityCharacter::OnBuffRefreshCD( CEntityPlayer* tpPlayer, CTemplateBuff::CBuffFunc & buffFunc)
{

	LK_ASSERT( tpPlayer != NULL, return );

    CPropertyPlayer* tpPro = (CPropertyPlayer*)tpPlayer->GetProperty();
    CSkillList&  tpSkillList = tpPro->GetSkillList( );
    CGroupCDList& tpGroupCDList = tpPro->GetCDGroupList();
    CSkillObject* pSkillObj = NULL;
        
    // mBuffParam[0] = 0 刷新一个技能      mBuffParam[1] 要刷新的技能id  mBuffParam[2] 不刷新的技能id   
    //               = 1 刷新一个系的技能  mBuffParam[1] 要刷新的技能系的id  
    //               = 2 刷新全体技能      
    //
        //刷新一个技能
    if(buffFunc.mBuffParam[ 0 ] == 0 )
    {
        for( int i = 0 ; i < tpSkillList.GetSkillNum(); i++ )
        {
            
            pSkillObj =  tpSkillList.GetSkillObjectByIndex( i );
            if( pSkillObj == NULL)
                continue; 
            //要刷新的技能 但是不能是本技能
            if(  (int)(pSkillObj->GetSkillID( )) == buffFunc.mBuffParam[ 1 ] &&  (int)(pSkillObj->GetSkillID( )) != buffFunc.mBuffParam[ 2 ] )
            {
                tpPro->OnSkillCDRefresh( pSkillObj, tpPlayer); 
            }
        }
    
    //刷新一个系的技能
    }else if(buffFunc.mBuffParam[ 0 ] == 1 )
    {
        CTemplateSkill* tpSkillTpl = NULL;

        for(   int i = 0 ; i < tpSkillList.GetSkillNum(); i++ )
        {
            pSkillObj = tpSkillList.GetSkillObjectByIndex( i );
        
            if( pSkillObj == NULL ||  pSkillObj->GetCooldown() == 0)
                continue;
            tpSkillTpl = pSkillObj->GetSkillTpl( ); 

            //技能系别
            if( tpSkillTpl == NULL || tpSkillTpl->mGenerID !=  buffFunc.mBuffParam[ 1 ]) 
                continue;
            //不刷新的技能(本技能)
            if( (int)pSkillObj->GetSkillID( ) == buffFunc.mBuffParam[ 2 ] )
                continue;     
            tpPro->OnSkillCDRefresh( pSkillObj, tpPlayer); 
        }
    //刷新全体技能
    }else if(buffFunc.mBuffParam[ 0 ] == 2 )
    {
        //刷新技能
        for(  int i = 0 ; i < tpSkillList.GetSkillNum(); i++ )
        {
            pSkillObj = tpSkillList.GetSkillObjectByIndex( i );
            if( pSkillObj == NULL || pSkillObj->GetCooldown() == 0)
                continue;
            
            //不刷新的技能(本技能)
            if( (int)pSkillObj->GetSkillID( ) == buffFunc.mBuffParam[ 2 ] )
                continue;     
            tpPro->OnSkillCDRefresh( pSkillObj, tpPlayer); 
            
            CTemplateSkill *pSkillTpl = pSkillObj->GetSkillTpl( );

            //刷新cd组CD
            for ( int i = 0; i < (int)ARRAY_CNT( pSkillTpl->mCDGroup ); i ++ )
            {
                CGroupCD& rGroupCD = tpGroupCDList.GetGroupCD( pSkillTpl->mCDGroup[i]);    
                if(rGroupCD.GetCDTime() == 0 )
                {
                    continue;
                }
                tpPro->OnGroupCDRefresh( rGroupCD, tpPlayer);
            }

        }
    }
}



// 得到角色类实体等级
int CEntityCharacter::GetLevel()
{
	if ( this->IsPlayer() )
	{
		CEntityPlayer* pPlayer = static_cast< CEntityPlayer* >( this );
		return pPlayer->GetLevel();
	}
	else if( this->IsNpc() )
	{
		CEntityNpc* pNpc = static_cast< CEntityNpc* >( this );
		return pNpc->GetLevel();
	}
	return 0;
}

int CEntityPlayer::BourseLogSend( )
{
	if( mBourseLogIsNeedLoad == false )
	{
		CMessageBourseTradeLog tLog;
		CMessage tMsg;

		tLog.mutable_sell_log( )->CopyFrom( mBourseSellLog );	
		tLog.mutable_buy_log( )->CopyFrom( mBourseBuyLog );

		tMsg.mutable_msghead()->set_messageid( ID_S2C_BOURSE_TRADE_LOG );	
		tMsg.set_msgpara( (uint32)&tLog );

		CSceneLogic::GetSingletonPtr( )->Send2Player( this, &tMsg );

	}
	return SUCCESS;
}

int CEntityPlayer::SetBourseLog( const PBBourseDetailList& vpBuyList, const PBBourseDetailList& vpSellList )
{
	mBourseBuyLog.CopyFrom( vpBuyList );
	mBourseSellLog.CopyFrom( vpSellList );
	SetBourseLogIsNeedLoad( false );
	return SUCCESS;
}




bool CEntityGhost::Setup(unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CCreator *pCreator , CWTPoint* pPos, bool bInit, int vNpcID )
{
	if ( pCreator->GetCreatorType( ) != ENTITYTYPE_GHOST )
	{
		return false;
	}
	CGhostCreator* tpGhostCreator = (CGhostCreator*) pCreator;
	
	mEntityID = get_id( );
	mNpcID = tpGhostCreator->mNpcID;
	mLineID = usLineID;
	mMapID = usMapID;
	mIndex = usMapIndex;
	mPos = tpGhostCreator->mPos;
	mDirection = tpGhostCreator->mDirection;
	mAliveState = ALIVE_NORMAL;
	mCreatorID= tpGhostCreator->get_id();

	GetProperty()->SetEntityID( this->get_id( ) );
	GetProperty()->SetPropertyTempID( mNpcID );
	
	return true;
}

