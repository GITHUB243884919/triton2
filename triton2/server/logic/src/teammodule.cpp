#include "stdafx.hpp"
#include "teammodule.h"
#include "mapmodule.h"
#include "propertymodule.h"
#include "modulemanager.h"
#include "sceneobj_manager.h"
#include "sceneobj_manager_imp.h"
#include "message_pb.hxx.pb.h"
#include "teammessage_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "entity.h"
#include "friendmodule.h"

template<> CTeamModule* CSingleton< CTeamModule >::spSingleton = NULL;
char CTeamModule::sModuleName[ NAME_LENGTH ] = "Team_1";

// ********************************************************************** //
//teammodule
// ********************************************************************** //
CTeamModule::CTeamModule( ) : CLogicModule( sModuleName )	
{
	if ( mShmPtr->GetInitMode() == SHM_INIT )
	{
		mTeamlist.initailize();
		mClaimList.initailize();
		mRecruitList.initailize();
	}
}

CTeamModule::~CTeamModule() 
{

}

// *******************************************************
//	Function Name : CountSize
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-4 
// *******************************************************

unsigned int CTeamModule::CountSize()
{
	unsigned int tSize = sizeof(CTeamModule);
	return tSize;
}

// *******************************************************
//	Function Name : OnLaunchServer
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-4 
// *******************************************************

void CTeamModule::OnLaunchServer( )
{
	mTeamPropertyUpTime = CSceneCfgManager::GetSingleton().GetTeam_cfg()->mUpdatetime;
	OnLoadConfig();
	return ;
}

void CTeamModule::OnLoadConfig()
{
	TEAM_LOG* tpLog = CSceneCfgManager::GetSingletonPtr()->GetTeam_log();
	if ( tpLog != NULL )
	{
		INIT_ROLLINGFILE_LOG( tpLog->mIdentifier, tpLog->mDirectory, ( LogLevel ) tpLog->mPriority, tpLog->mMaxfilesize, tpLog->mMaxbackupindex );
	}	
}

// *******************************************************
//	Function Name : OnExitServer
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-4 
// *******************************************************

void CTeamModule::OnExitServer( )
{
	// TODO: 
	return ;
}

// *******************************************************
//	Function Name : OnTimer
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-4 
// *******************************************************

void CTeamModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	mTeamPropertyUpTime -= vTickOffset;
	if( mTeamPropertyUpTime <= 0)
	{
		mTeamPropertyUpTime = CSceneCfgManager::GetSingleton().GetTeam_cfg()->mUpdatetime;
		RequestUpdateGateTeamProperty();
	}
	return ;
}

// *******************************************************
//	Function Name : OnCreateMap
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-4 
// *******************************************************

int CTeamModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )
{
	// TODO ...
	return SUCCESS;
}

// *******************************************************
//	Function Name : OnCreateEntity
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-4 
// *******************************************************

void CTeamModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	if ( pEntity->IsPlayer() == false )
	{
		return ;
	}
	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) pEntity;
	if ( (int)tpPlayer->GetCharID() == tpPlayer->GetTeamFlag() )
	{
		NotifyTeamFlag( tpPlayer, tpPlayer->GetCharID() );
	}

	SendObtainGateTeamRequest( tpPlayer->GetCharID() );
	return ;
}

// *******************************************************
//	Function Name : OnDestroyEntity
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-4 
// *******************************************************

void CTeamModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{
	LK_ASSERT( pEntity != NULL, return );

	if( pEntity->IsPlayer() == false )
	{
		return ;
	}

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) pEntity;
	
	if ( vCode == SAVE_REDIRECT )
	{
		ProcessRedirectScene( tpPlayer );
		return ;
	}

	ClaimList::iterator itClaim = mClaimList.find( tpPlayer->GetCharID() );
	if ( itClaim != mClaimList.end() )
	{
		mClaimList.erase( itClaim );
	}
	
	RecruitList::iterator itRecruit = mRecruitList.find( tpPlayer->GetCharID() );
	if ( itRecruit != mRecruitList.end() )
	{
		mRecruitList.erase( itRecruit );
	}
	
	
	CMessage tMessage;
	CMessageLeaveGateTeamRequest tPara;
	tPara.set_charid( tpPlayer->GetCharID() );
	MakeMessage<CMessageLeaveGateTeamRequest>( ID_S2G_REQUEST_LEAVETEAM, tPara, &tMessage );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
	return ;
}

// *******************************************************
//	Function Name : OnSaveData
//	Description   : 
//	Input Params  :
// 	Output Params :
//	Modified Time : 2008-12-4 
// *******************************************************

bool CTeamModule::OnSaveData( CEntityPlayer *pPlayer, int vSaveCode ) 
{
	//if( vSaveCode != SAVE_TIMETRIGER )
	//{
	//	// 离开场景，干掉身上的组队buff, 必须在这里处理，因为buff需要存盘。
	//	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( InTeam( pPlayer->GetCharID() ) );
	//	if ( tpTeam != NULL )
	//	{
	//		tpTeam->RemoveTeamEventBuff( pPlayer->GetCharID() );
	//	}	
	//}

	return true;
}

int CTeamModule::OnGetMemberList( int vCharID, int* pMemberList, int tNum, int& tRtnNum )
{
	if ( pMemberList == NULL )
	{
		return -1;
	}
	int tTeamID = InTeam( vCharID );
	if ( tTeamID > 0 )
	{
		CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( tTeamID );
		if ( tpTeam == NULL )
		{
			pMemberList[ 0 ] = vCharID;
			tRtnNum = 1;
		}
		else
		{
			int tRtn = tpTeam->GetMemberList( pMemberList, tNum, tRtnNum );
			if ( tRtn < 0 )
			{
				pMemberList[ 0 ] = vCharID;
				tRtnNum = 1;
			}
		}
	}
	else
	{
		pMemberList[ 0 ] = vCharID;
		tRtnNum = 1;
	}
	return SUCCESS;
}

int CTeamModule::OnGetMemberList(CEntityPlayer* pPlayer, CEntityPlayer **pPlayerArray, int tNum, int& tRtnNum )
{
	if ( pPlayer == NULL || pPlayerArray == NULL )
	{
		return -1;
	}

	int tTeamID = InTeam( pPlayer->GetCharID() );
	if ( tTeamID > 0 )
	{
		CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( tTeamID );
		if ( tpTeam == NULL )
		{
			pPlayerArray[ 0 ] = pPlayer;
			tRtnNum = 1;
		}
		else
		{
			int tMemberList[ TEAMNUM ] = { 0 };
			int tRtn = 0, tTeamNum = 0;
			tRtn = tpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tTeamNum );
			if ( tRtn < 0 )
			{
				pPlayerArray[ 0 ] = pPlayer;
				tRtnNum = 1;
			}
			else
			{
				tRtnNum = 0;
				for ( int i = 0; i < tTeamNum && i < (int)ARRAY_CNT( tMemberList ); i ++ )
				{
					CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tMemberList[ i ] );
					if ( tpPlayer == NULL )
					{
						continue;
					}
					if ( tRtnNum < tNum )
					{
						pPlayerArray[ tRtnNum ++ ] = tpPlayer;
					}
				}
			}			 
		}
	}
	else
	{
		pPlayerArray[ 0 ] = pPlayer;
		tRtnNum = 1;
	}
	return SUCCESS;
}

int CTeamModule::OnGetMemberList( int vTeamID, CEntityPlayer** pMemberList, int tNum, int& tRtnNum )
{
	if ( pMemberList == NULL )
	{
		return -1;
	}

	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( vTeamID );
	if ( tpTeam == NULL )
	{
		return -1;
	}
	else
	{
		int tMemberList[ TEAMNUM ] = { 0 };
		int tRtn = 0, tTeamNum = 0;
		tRtn = tpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tTeamNum );
		if ( tRtn < 0 )
		{
			return -1;
		}
		else
		{
			tRtnNum = 0;
			for ( int i = 0; i < tTeamNum && i < (int)ARRAY_CNT( tMemberList ); i ++ )
			{
				CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tMemberList[ i ] );
				if ( tpPlayer == NULL )
				{
					continue;
				}
				if ( tRtnNum < tNum )
				{
					pMemberList[ tRtnNum ++ ] = tpPlayer;
				}
			}
		}			 
	}
	return SUCCESS;
}


//*********************************************************
//Function Name	:GetTurnOwner
//Description	:
//Input			:EntityID
//Output		:
//Modified Time	:2008-11-13
//*********************************************************

int CTeamModule::GetTurnOwner( CEntityPlayer* pPlayer, int* pCharList, int vNum, CEntity *pDesEntity )
{
	LK_ASSERT( pPlayer != NULL && pCharList != NULL, return -1 );

	// force init char list
	for ( int i = 0; i < vNum; i ++ )
	{
		pCharList[ i ] = 0; 
	}	
	
	int tTeamID = InTeam( pPlayer->GetCharID() );
	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( tTeamID );
	if ( tpTeam == NULL )
	{
		pCharList[ 0 ] = pPlayer->GetCharID();
		return -1;
	}

	int tMode = tpTeam->GetMode();

	switch ( tMode )
	{
	case RANDOM:
		{
			int tNum = 0;
			tpTeam->GetMemberList( pCharList, vNum, tNum );
			break;
		}
	case ALTERNATION:
		{
			CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpTeam->GetOwner() );
			if ( tpPlayer == NULL )
			{
				pCharList[ 0 ] = pPlayer->GetCharID();
			}
			else
			{
				if ( pDesEntity != NULL && pDesEntity->IsWithinDistance( tpPlayer, INVALID_DISTANCE ) )
				{
					pCharList[ 0 ] = tpPlayer->GetCharID();
				}				
				else
				{
					pCharList[ 0 ] = pPlayer->GetCharID();
				}
			}
			break;
		}
	case PROBABILITY:
		{
			int tMemberList[ TEAMNUM ] = { 0 };
			int tRtn = 0, tNum = 0;

			tRtn = tpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
			if ( tRtn < 0 )
			{
				pCharList[ 0 ] = pPlayer->GetCharID();
				break;
			}

			int tIndex = 0;
			CEntityPlayer* tpMemberArray[ TEAMNUM ] = { NULL };
			for ( int i = 0; i < tNum && tIndex < (int) ARRAY_CNT( tpMemberArray ); i ++ )
			{
				CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tMemberList[ i ] );
				if ( pDesEntity == NULL || pDesEntity->IsWithinDistance( tpPlayer, INVALID_DISTANCE ) == true )
				{
					tpMemberArray[ tIndex ++ ] = tpPlayer;
				}			
			}		

			CEntityPlayer* tpTmpPlayer = tpMemberArray[ RAND( tIndex ) ];
			
			if ( tpTmpPlayer != NULL )
			{
				pCharList[ 0 ] = tpTmpPlayer->GetCharID();
			}			
			
			break;
		}
	}
	
	return SUCCESS;
}


CEntityPlayer* CTeamModule::GetTurnOwnerOnTask( CEntityPlayer* pPlayer )
{
	if ( pPlayer == NULL )
	{
		return NULL;
	}

	int tTeamID = InTeam( pPlayer->GetCharID() );
	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( tTeamID );
	if ( tpTeam == NULL )
	{
		return pPlayer;
	}

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpTeam->GetOwner() );
	if ( tpPlayer != NULL )
	{
		return tpPlayer;
	}

	return pPlayer;
}
//
////***********************************
//// FunctionName : OnMessageCreateTeamSelf 
//// Description  : 
//// Input Params : 
//// Output Params: 
//// Return Types : 
//// Modified Time: [6/25/2009]
////***********************************
CEntityPlayer* CTeamModule::GetTeamHeader(CEntityPlayer *pPlayer)
{
	if ( pPlayer == NULL )
	{
		return NULL;
	}

	int tTeamID = InTeam( pPlayer->GetCharID() );
	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( tTeamID );
	if ( tpTeam == NULL )
	{
		return pPlayer;
	}

	CEntityPlayer* tpPlayer  = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpTeam->GetHeaderID() );
	if ( tpPlayer == NULL )
	{
		return pPlayer;
	}
	else
	{
		return tpPlayer;
	}
	return pPlayer;
}



void CTeamModule::NotifyEndTeam( CEntityPlayer* pPlayer )
{
	if ( pPlayer == NULL ) return ;

	CMessage tMessage;
	CMessageTeamEndCallBack tPara;
	MakeMessage<CMessageTeamEndCallBack>( ID_S2C_NOTIFY_ENDTEAM, tPara, &tMessage );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
	NotifyTeamFlag( pPlayer, TEAM_NONE );
	return ;
}

void CTeamModule::NotifyTeamFlag( CEntityPlayer* pPlayer, int vFlag )
{
	if ( pPlayer == NULL ) return ;

	pPlayer->SetTeamFlag( vFlag );
	
	CMessage tMessage;
	CMessageTeamStateChange tPara;
	tPara.set_charid( pPlayer->GetCharID() );
	tPara.set_teamflag( vFlag );
	MakeMessage<CMessageTeamStateChange>(ID_S2C_NOTIFY_CHANGETEAMSTATE, tPara, &tMessage);
	CMapModule::GetSingleton().MessageBroadCast( pPlayer, &tMessage,false, true );
	return ;
}

void CTeamModule::MakeMemberPB( PBMember* pMember, CEntityPlayer* pPlayer )
{
	if ( pMember == NULL || pPlayer == NULL )
	{
		return ;
	}
	CPropertyPlayer* tpPropery = ( CPropertyPlayer* ) pPlayer->GetProperty();
	pMember->set_charid( pPlayer->GetCharID() );
	pMember->set_name( pPlayer->GetName() );
	pMember->set_face( pPlayer->GetFaceType() );
	pMember->set_level( pPlayer->GetLevel() );
	pMember->set_metier( pPlayer->GetMetier() );
	pMember->set_sex( pPlayer->GetSex() );
	pMember->set_mp( (int)tpPropery->GetMPPercent() );
	pMember->set_hp( (int)tpPropery->GetHPPercent() );
	pMember->set_lineid( pPlayer->GetLineID() );
	pMember->set_mapid( pPlayer->GetMapID() );
	pMember->set_xpos( pPlayer->GetPosX() );
	pMember->set_ypos( pPlayer->GetPosY() );
	pMember->set_commstate( (int)pPlayer->GetCommState() );
#ifdef _FB_
	pMember->set_repetionmapindex( GetMapID4Log( pPlayer ) );
#endif

	// add team member buff list
	tpPropery->GetBuffList()->GetTlvBufferTeamShow( pMember->mutable_bufflist() );
	pMember->set_order(TEAM_ORDER_FIRST);
	return ;
}

void CTeamModule::MakeMemberPB(PBMember *pMember, PBTeamItem *pItem)
{
	if ( pMember == NULL || pItem == NULL )
	{
		return ;
	}

	pMember->set_charid( pItem->charid() );
	pMember->set_name( pItem->name().c_str() );
	pMember->set_face( pItem->face() );
	pMember->set_level( pItem->level() );
	pMember->set_metier( pItem->metier() );
	pMember->set_sex( pItem->sex() );

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( pItem->charid() );
	if ( tpPlayer != NULL )
	{
		CPropertyPlayer* tpProperty  = ( CPropertyPlayer* ) tpPlayer->GetProperty();
		pMember->set_mp( (int)tpProperty->GetMPPercent() );
		pMember->set_hp( (int)tpProperty->GetHPPercent() );
	}
	else
	{
		pMember->set_mp( pItem->mp() );
		pMember->set_hp( pItem->hp() );
	}

	pMember->set_lineid( pItem->lineid() );
	pMember->set_mapid( pItem->mapid() );
	pMember->set_xpos( pItem->posx() );
	pMember->set_ypos( pItem->posy() );
	pMember->set_commstate( pItem->status() );
	pMember->set_repetionmapindex( pItem->repetionmapindex( ) );

	pMember->mutable_bufflist()->CopyFrom( pItem->bufflist() ); 
	pMember->set_order(pItem->order());
	return ;
}

void CTeamModule::MakeTeamItemPB( PBTeamItem* pItem, CEntityPlayer* pPlayer )
{
	if ( pItem == NULL || pPlayer == NULL )
	{
		return ;
	}

	CPropertyPlayer* tpPropery = ( CPropertyPlayer* ) pPlayer->GetProperty();
	pItem->set_charid( pPlayer->GetCharID() );
	pItem->set_name( pPlayer->GetName() );
	pItem->set_face( pPlayer->GetFaceType() );
	pItem->set_level( pPlayer->GetLevel() );
	pItem->set_metier( pPlayer->GetMetier() );
	pItem->set_sex( pPlayer->GetSex() );
	pItem->set_mp( (int)tpPropery->GetMPPercent() );
	pItem->set_hp( (int)tpPropery->GetHPPercent() );
	pItem->set_lineid( pPlayer->GetLineID() );
	pItem->set_mapid( pPlayer->GetMapID() );
	pItem->set_posx( pPlayer->GetPosX() );
	pItem->set_posy( pPlayer->GetPosY() );
	pItem->set_nationality( pPlayer->GetNationality() );
#ifdef _FB_
	pItem->set_repetionmapindex( GetMapID4Log( pPlayer ) );
#endif
	
	if ( pPlayer->GetCommState() == CEntityPlayer::STATE_INMAP )
	{
		if ( pPlayer->GetAliveStatus() == ALIVE_DIE )
		{
			pItem->set_status( emItemDead );
		}
		else
		{
			pItem->set_status( emOnLine );
		}
		
	}

	// add team member buff list
	CBuffList* tpBuffList = tpPropery->GetBuffList();
	tpBuffList->GetTlvBufferTeamShow(pItem->mutable_bufflist()); 

	return ;
}


int CTeamModule::InsertMember(int vCharID, int vTeamID)
{
	TeamList::iterator it = mTeamlist.find( vCharID );
	if ( it != mTeamlist.end() )
	{
		return -1;
	}

	mTeamlist.insert( TeamList::value_type( vCharID, vTeamID ) );
	return SUCCESS;
}

int CTeamModule::RemoveMember(int vCharID)
{
	TeamList::iterator it = mTeamlist.find( vCharID );
	if ( it == mTeamlist.end() )
	{
		return -1;
	}

	mTeamlist.erase( it );
	return SUCCESS;
}

int CTeamModule::RemoveTeam( CTeam* vpTeam )
{
	if ( vpTeam == NULL )
	{
		return -1;
	}

	int tMemberList[ TEAMNUM ] = { 0 };
	int tRtn = 0, tNum = 0;
	tRtn = vpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
	if ( tRtn < 0 )
	{
		LOG_ERROR( "team", "get member list failed.teamid=%d,[%s:%d]", vpTeam->get_id(), __LK_FILE__, __LINE__ );
		return -1;
	}
	for ( int i = 0; i < tNum; i ++ )
	{
		CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tMemberList[ i ] );
		if ( tpPlayer != NULL )
		{
			NotifyTeamFlag( tpPlayer, TEAM_NONE );
		}
		TeamList::iterator it = mTeamlist.find( tMemberList[ i ] );
		if ( it != mTeamlist.end() )
		{
			mTeamlist.erase( it );
		}
	}
	vpTeam->DelTeam();
	CSceneObjManager::GetSingleton().DestroyObject( vpTeam->get_id() );
	return SUCCESS;
}

int CTeamModule::InTeam( int vCharID )
{
	TeamList::iterator it = mTeamlist.find( vCharID );
	if ( it != mTeamlist.end() )
	{
		return it->second;
	}
	return SUCCESS;	
}

int CTeamModule::GetTeamHeadCharID(int vCharID)
{
	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( InTeam( vCharID ) );
	if ( tpTeam == NULL )
	{
		return vCharID;
	}
	
	return tpTeam->GetHeaderID();
}


// 获取队伍中在当前服务器上在线的第一个成员charid
int CTeamModule::GetLocalFirstOLMemberID( int nCharID )
{
	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( InTeam( nCharID ) );
	if ( tpTeam == NULL )
	{
		return nCharID;
	}

	int tMemberList[ TEAMNUM ] = { 0 };
	int tRtn = 0, tNum = 0;

	tRtn = tpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
	if ( tRtn < 0 )
	{
		return nCharID;
	}

	int tIndex = 0;
	CEntityPlayer* tpMemberArray[ TEAMNUM ] = { NULL };
	for ( int i = 0; i < tNum && tIndex < (int) ARRAY_CNT( tpMemberArray ); i ++ )
	{
		CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tMemberList[ i ] );
		if ( tpPlayer != NULL )
		{
			return tpPlayer->GetCharID();
		}			
	}		

	return nCharID;	
}


void CTeamModule::RequestUpdateGateTeamProperty()
{
	CSceneObjManagerImp* tpObjManager = CSceneObjManager::GetSingleton().mSceneObjManagerImp;
	CSceneObjManagerImp::ObjMng_Team::iterator it = tpObjManager->mTeam.begin();
	CSceneObjManagerImp::ObjMng_Team::iterator itEnd = tpObjManager->mTeam.end();
	for ( ; it != itEnd; ++ it )
	{
		CTeam* tpTeam = ( CTeam* ) &(*it);
		if ( tpTeam == NULL )
		{
			continue;
		}

		//TODO: 临时加在这里
		tpTeam->UpdateTeamBuffEvent();

		int tMemberList[ TEAMNUM ] = { 0 };
		int tRtn = 0, tNum = 0;

		tRtn = tpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
		if ( tRtn < 0 )
		{
			continue;
		}

		if ( tNum == 0 )
		{
			RemoveTeam( tpTeam );
			continue;
		}

		CMessage tMessage;
		CMessageUpdateTeamPropertyRequest tPara;
		MakeMessage<CMessageUpdateTeamPropertyRequest>( ID_S2G_REQUEST_UPDATETEAMPRO, tPara, &tMessage );

		int tOnLineCount = 0;
		for ( int i = 0; i < tNum && i < (int)ARRAY_CNT( tMemberList ); i ++ )
		{
			CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tMemberList[ i ] );
			if ( tpPlayer == NULL )
			{
				continue;
			}
			PBTeamItem* tpMember = tPara.mutable_info()->add_members();
			MakeTeamItemPB( tpMember, tpPlayer );
			tOnLineCount ++;
		}

		if ( tOnLineCount == 0 )
		{
			RemoveTeam( tpTeam );
			continue;
		}
		CFriendModule::GetSingleton().UpDateTeamIntimacy( tpTeam );
		tPara.set_charid( tpTeam->GetHeaderID() );
		tPara.mutable_info()->set_headercharid( tpTeam->GetHeaderID() );
		CSceneLogic::GetSingleton().Send2Gate( &tMessage );
	}
	return ;
}

void CTeamModule::LoadTeamMsg( CEntityPlayer* pPlayer, PBTeamMsg* pMsg )
{
	if ( pPlayer == NULL || pMsg == NULL )
	{
		return ;
	}

	int tTeamID = InTeam( pPlayer->GetCharID() );
	if ( tTeamID > 0 )
	{
		RecruitList::iterator it = mRecruitList.find( pPlayer->GetCharID() );
		if ( it == mRecruitList.end() )
		{
			return ;
		}
		MakeStructMsgToPB( (STeamMsg*)&(it->second), pMsg );
	}
	else
	{
		ClaimList::iterator it = mClaimList.find( pPlayer->GetCharID() );
		if ( it == mClaimList.end() )
		{
			return ;
		}
		MakeStructMsgToPB( (STeamMsg*)&(it->second), pMsg );
	}
}

void CTeamModule::ProcessRedirectScene( CEntityPlayer* pPlayer )
{
	if ( pPlayer == NULL ) return ;

	int tTeamID = InTeam( pPlayer->GetCharID() );
	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( tTeamID );
	if ( tpTeam == NULL )
	{
		return ;
	}

	int tMemberList[ TEAMNUM ] = { 0 };
	int tRtn = 0, tNum = 0;

	tRtn = tpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
	if ( tRtn < 0 )
	{
		return ;
	}

	int tOnListNum = 0;
	for ( int i = 0; i < tNum && i < (int) ARRAY_CNT( tMemberList ); i ++ )
	{
		CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tMemberList[ i ] );
		if ( tpPlayer == NULL )
		{
			continue;
		}
		tOnListNum ++;
	}

	if ( tOnListNum == 0 || tOnListNum == 1 )
	{
		RemoveTeam( tpTeam );
	}

	ClaimList::iterator itClaim = mClaimList.find( pPlayer->GetCharID() );
	if ( itClaim != mClaimList.end() )
	{
		mClaimList.erase( itClaim );
	}
	RecruitList::iterator itRecruit = mRecruitList.find( pPlayer->GetCharID() );
	if ( itRecruit != mRecruitList.end() )
	{
		mRecruitList.erase( itRecruit );
	}
	return; 
}

void CTeamModule::CreateTeamOnRedirect( CEntityPlayer* pPlayer, PBTeamInfo* pTeam )
{
	if ( pTeam == NULL || pPlayer == NULL ) return ;

	CTeam* tpTeam = NULL;
	int tTeamID = InTeam( pTeam->headercharid() );
	if ( tTeamID != 0 )
	{
		tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( tTeamID );
		if ( tpTeam == NULL )
		{
			LOG_ERROR( "team", "object invalid,id=%d,[%s:%d]", tTeamID, __LK_FILE__, __LINE__ );
			return ;
		}
		if ( tpTeam->GetTeamID() != (int)pTeam->teamid() )
		{
			RemoveTeam( tpTeam );
			tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_TEAM );
			if ( tpTeam == NULL )
			{
				LOG_ERROR( "team", "create object failed.Type:Team.[%s:%d]", __LK_FILE__, __LINE__ );
				return; 
			}
			tpTeam->SetHeaderID( pTeam->headercharid() );
			tpTeam->SetTeamID( pTeam->teamid() );
			tpTeam->SetMode( pTeam->distributemode() );
			for ( int i = 0; i < pTeam->members_size(); i ++ )
			{
				int tCharID = pTeam->mutable_members( i )->charid();
				tpTeam->AddMember( tCharID );
				InsertMember( tCharID, tpTeam->get_id() );
				CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tCharID );
				if ( tpPlayer != NULL )
				{
					NotifyTeamFlag( tpPlayer, pTeam->headercharid() ); 
				}
			}
		}
		else
		{
			int tTeamCount = 0;
			int tMemberList[ TEAMNUM ] = { 0 };
			for ( int i = 0; i < (int)ARRAY_CNT( tMemberList ) && i < pTeam->members_size(); i ++ )
			{
				tMemberList[ i ] = pTeam->mutable_members( i )->charid();
				tTeamCount ++;
			}
			RefreshTeam( tMemberList, tpTeam, tTeamCount );
		}
	}
	else
	{
		tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_TEAM );
		if ( tpTeam == NULL )
		{
			LOG_ERROR( "team", "create object failed.Type:Team.[%s:%d]", __LK_FILE__, __LINE__ );
			return; 
		}
		tpTeam->SetHeaderID( pTeam->headercharid() );
		tpTeam->SetTeamID( pTeam->teamid() );
		tpTeam->SetMode( pTeam->distributemode() );
		for ( int i = 0; i < pTeam->members_size(); i ++ )
		{
			int tCharID = pTeam->mutable_members( i )->charid();
			tpTeam->AddMember( tCharID );
			InsertMember( tCharID, tpTeam->get_id() );
			CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tCharID );
			if ( tpPlayer != NULL )
			{
				NotifyTeamFlag( tpPlayer, pTeam->headercharid() ); 
			}
		}
	}

	// send team list to entity
	CMessage tMessage;
	CMessageTeamCallBack tPara;
	tPara.set_headername( pTeam->headname() );
	tPara.set_headetyid( tpTeam->GetHeaderID() );
	tPara.set_distribute( tpTeam->GetMode() );

	int tCount = 0;
	for ( int i = 0; i < pTeam->members_size(); i ++ )
	{
		PBMember* tpMember = tPara.add_member();
		MakeMemberPB( tpMember, pTeam->mutable_members( i ) );
		tCount ++;
	}
	tPara.set_count( tCount );
	MakeMessage<CMessageTeamCallBack>(ID_S2C_NOTIFY_ITEMLIST, tPara, &tMessage);
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );

	return ;
}

void CTeamModule::ProcessTripDisplay( CTeam* vpTeam, CEntityPlayer* vpPlayer, int vAction )
{
	if ( vpTeam == NULL || vpPlayer == NULL )
	{
		return ;
	}

	int tMemberList[ TEAMNUM ] = { 0 };
	CEntityPlayer* tMemberArray[ TEAMNUM ] = { NULL };
	int tRtn = 0, tNum = 0;

	tRtn = vpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
	if ( tRtn < 0 )
	{
		return ;
	}
	
	int tIndex = 0;
	for ( int i = 0; i < tNum && i < (int) ARRAY_CNT( tMemberArray ); i ++ )
	{
		CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tMemberList[ i ] );
		if ( tpPlayer != NULL )
		{
			tMemberArray[ tIndex ++ ] = tpPlayer; 
		}		
	}	
	
	switch ( vAction )
	{
	case TEAM_ADD_TRIP:
		{
			CPropertyModule::GetSingleton().DisplayTripPlayerAddTeam( vpPlayer, tMemberArray, tIndex );
			break;
		}
	case TEAM_LEAVE_TRIP:
		{
			CPropertyModule::GetSingleton().DisappearTripPlayerLeaveTeam( vpPlayer, tMemberArray, tIndex );
			break;
		}
	case TEAM_END_TRIP:
		{
			CPropertyModule::GetSingleton().DisappearTripTeamDisband( tMemberArray, tIndex );
			break;
		}
	default:
		break;
	}
	
	return ;
}

void CTeamModule::CreateTeamMsgOnRedirect( PBMsg* pMsg, CEntityPlayer* pPlayer )
{
	if ( pMsg == NULL || pPlayer == NULL )
	{
		return ;
	}

	if ( pMsg->type() == TEAM_ASKFOR )
	{
		ClaimList::iterator itClaim = mClaimList.find( pPlayer->GetCharID() );
		if ( itClaim != mClaimList.end() )
		{
			mClaimList.erase( itClaim );
		}
		STeamMsg tMsg;
		MakePbMsgToStruct( pMsg, &tMsg );
		mClaimList.insert( ClaimList::value_type( pPlayer->GetCharID(), tMsg ) );	

		CMessage tMessage;
		CMessageTeamAskTeamCallBack tPara;
		MakeMessage<CMessageTeamAskTeamCallBack>( ID_S2C_NOTIFY_ASKTEAMTRN, tPara, &tMessage );
		tPara.set_name( pMsg->name() );
		tPara.set_level( pMsg->level() );
		tPara.set_metier( pMsg->metier() );
		tPara.set_conscribetype( pMsg->rootstyle() );
		tPara.set_subtype( pMsg->substyle() );
		tPara.set_msg( pMsg->content() );
		CMapModule::GetSingleton().MessageBroadCast( pPlayer, &tMessage, false, true );
	}
	else if ( pMsg->type() == TEAM_CONSCRIBE )
	{
		RecruitList::iterator itRecruit = mRecruitList.find( pPlayer->GetCharID() );
		if ( itRecruit != mRecruitList.end() )
		{
			mRecruitList.erase( itRecruit );
		}
		STeamMsg tMsg;
		MakePbMsgToStruct( pMsg, &tMsg );
		mRecruitList.insert( RecruitList::value_type( pPlayer->GetCharID(), tMsg ) );

		CMessage tMessage;
		CMessageTeamConscribeCallBack tPara;
		MakeMessage<CMessageTeamConscribeCallBack>( ID_S2C_NOTIFY_CONSCRIBERTN, tPara, &tMessage );
		tPara.set_headername( pMsg->name() );
		tPara.set_count( pMsg->count() );
		tPara.set_metier( pMsg->metier() );
		tPara.set_minlevel( pMsg->minlevel() );
		tPara.set_maxlevel( pMsg->maxlevel() );
		tPara.set_msg( pMsg->content() );
		tPara.set_conscribetype( pMsg->rootstyle() );
		tPara.set_subtype( pMsg->substyle() );
		CMapModule::GetSingleton().MessageBroadCast( pPlayer, &tMessage, false, true );
	}
	
	return ;
}

void CTeamModule::SendObtainGateTeamRequest( int nCharID )
{
	CMessage tMessage;
	CMessageObtainGateTeamRequest tPara;
	MakeMessage<CMessageObtainGateTeamRequest>( ID_S2G_REQUEST_OBTAINTEAMATGATE, tPara, &tMessage );
	tPara.set_charid( nCharID );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
	return ;
}

void CTeamModule::RefreshTeam( int* vpTeamList, CTeam* vpTeam, int vNum )
{
	if ( vpTeamList == NULL || vpTeam == NULL )
	{
		return ;
	}

	for ( int i = 0; i < vNum; i ++ )
	{
		TeamList::iterator it = mTeamlist.find( vpTeamList[ i ] );
		if ( it != mTeamlist.end() )
		{
			mTeamlist.erase( it );
		}
	}

	for ( int tIndex = 0; tIndex < vNum; tIndex ++ )
	{
		mTeamlist.insert( TeamList::value_type( vpTeamList[ tIndex ], vpTeam->get_id() ) );
		CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( vpTeamList[ tIndex ] );
		if ( tpPlayer != NULL )
		{
			NotifyTeamFlag( tpPlayer, vpTeam->GetHeaderID() ); 
		}
	}

	vpTeam->RefreshTeam( vpTeamList, vNum );
	return ;
}

void CTeamModule::MakeMsgPB( PBTeamMsg* pTeamMsg, PBMsg* pMsg )
{
	if ( pTeamMsg == NULL || pMsg == NULL )
	{
		return ;
	}

	pTeamMsg->set_type( pMsg->type() );
	pTeamMsg->set_rootstyle( pMsg->rootstyle() );
	pTeamMsg->set_substyle( pMsg->substyle() );
	pTeamMsg->set_metier( pMsg->metier() );
	pTeamMsg->set_level( pMsg->level() );
	pTeamMsg->set_charid( pMsg->charid() );
	pTeamMsg->set_count( pMsg->count() );
	pTeamMsg->set_content( pMsg->content() );
	pTeamMsg->set_maxlevel( pMsg->maxlevel() );
	pTeamMsg->set_minlevel( pMsg->minlevel() );
	pTeamMsg->set_name( pMsg->name() );

	return ;
}

int CTeamModule::MakeStructMsgToPB( STeamMsg* pSrcMsg, PBTeamMsg* pDesMsg )
{
	if ( pSrcMsg == NULL || pDesMsg == NULL )
	{
		return -1;
	}
	pDesMsg->set_charid( pSrcMsg->mCharID );	
	pDesMsg->set_count( pSrcMsg->mCount );
	pDesMsg->set_level( pSrcMsg->mLevel );	
	pDesMsg->set_minlevel( pSrcMsg->mMinLevel );
	pDesMsg->set_maxlevel( pSrcMsg->mMaxLevel );
	pDesMsg->set_rootstyle( pSrcMsg->mRootStyle );
	pDesMsg->set_substyle( pSrcMsg->mSubStyle );
	pDesMsg->set_metier( pSrcMsg->mMetier );
	pDesMsg->set_type( pSrcMsg->mType );
	pDesMsg->set_name( pSrcMsg->mName );
	pDesMsg->set_content( pSrcMsg->mContent );
	return SUCCESS;
}

int CTeamModule::MakePbMsgToStruct( PBMsg* pSrcMsg, STeamMsg* pDesMsg )
{
	if ( pSrcMsg == NULL || pDesMsg == NULL )
	{
		return -1;
	}
	pDesMsg->mCharID	= pSrcMsg->charid();
	pDesMsg->mCount		= pSrcMsg->count();
	pDesMsg->mLevel		= pSrcMsg->level();
	pDesMsg->mMinLevel	= pSrcMsg->minlevel();
	pDesMsg->mMaxLevel	= pSrcMsg->maxlevel();
	pDesMsg->mNationality = pSrcMsg->nationality();
	pDesMsg->mRootStyle	= pSrcMsg->rootstyle();
	pDesMsg->mSubStyle	= pSrcMsg->substyle();
	pDesMsg->mMetier	= pSrcMsg->metier();
	pDesMsg->mType		= pSrcMsg->type();
	memset( pDesMsg->mName, 0, sizeof( pDesMsg->mName ) );
	strncpy( pDesMsg->mName, pSrcMsg->name().c_str(), sizeof( pDesMsg->mName ) - 1 );
	memset( pDesMsg->mContent, 0, sizeof( pDesMsg->mContent ) );
	strncpy( pDesMsg->mContent, pSrcMsg->content().c_str(), sizeof( pDesMsg->mContent ) - 1 );
	return SUCCESS;
}


// 添加组队事件buff
bool CTeamModule::AddTeamEventBuff( CEntityPlayer* pPlayer, CTeamEventBuff& rEventBuff )
{
	CTeam* pTeam = (CTeam*)CSceneObjManager::GetSingletonPtr()->GetObject( InTeam(pPlayer->GetCharID()) );
	if( pTeam == NULL )
	{
		return false;
	}

	return pTeam->AddTeamBuffEvent( rEventBuff );
}

// 删除组队事件buff
void CTeamModule::RemoveTeamEventBuff( CEntityPlayer* pPlayer )
{
	// 离开场景，干掉身上的组队buff, 必须在这里处理，因为buff需要存盘。
	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( InTeam( pPlayer->GetCharID() ) );
	if ( tpTeam != NULL )
	{
		tpTeam->RemoveTeamEventBuff( pPlayer->GetCharID() );
	}
}
