/*
 *	JudgeModule.cpp
 *
 */

// Include Head Files
#include "shm.h"
#include "judgemodule.h"
#include "judgemessage_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "achievemodule.h"

#include "scenelogic.h"

template<> CJudgeModule* CSingleton< CJudgeModule >::spSingleton = NULL;
char CJudgeModule::sModuleName[ NAME_LENGTH ] = "Judge";

CJudgeModule::CJudgeModule( ) : CLogicModule( sModuleName )	
{
	if ( mShmPtr->GetInitMode() == SHM_INIT )
	{
	}
}

CJudgeModule::~CJudgeModule() 
{

}

// *******************************************************
//	Function Name : CountSize
//	Description   : 
//	Input Params  :
// *******************************************************

unsigned int CJudgeModule::CountSize()
{
	return sizeof(CJudgeModule);
}

// *******************************************************
//	Function Name : OnLaunchServer
//	Description   : 
//	Input Params  :
// *******************************************************

void CJudgeModule::OnLaunchServer( )
{
	return ;
}

void CJudgeModule::OnLoadConfig()
{
	;
}

// *******************************************************
//	Function Name : OnExitServer
//	Description   : 
//	Input Params  :
// *******************************************************

void CJudgeModule::OnExitServer( )
{
	// TODO: 
	return ;
}

// *******************************************************
//	Function Name : OnTimer
//	Description   : 
//	Input Params  :
// *******************************************************

void CJudgeModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	return ;
}

// *******************************************************
//	Function Name : OnCreateMap
//	Description   : 
//	Input Params  :
// *******************************************************

int CJudgeModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )
{
	// TODO ...
	return SUCCESS;
}

// *******************************************************
//	Function Name : OnRouterMessage
//	Description   : 
//	Input Params  :
// *******************************************************

void CJudgeModule::OnRouterMessage( CMessage* pMessage )
{
	//  TODO ...

	int tMessageID = pMessage->mutable_msghead()->messageid();
	
	switch ( tMessageID )
	{
	case ID_G2S_NOTIFY_JUDGEMODIFY:
		{
			OnGateModifyJudgeNotice( pMessage );
			break;
		}
	default:
		break;
	}

	return ;
}

// 数据库查询 [12/11/2009 Macro]
void CJudgeModule::OnDBSession( CMessage* npMessage, int nParam1, int nParam2 )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageExecuteSqlResponse3* tpExecuteSql = (CMessageExecuteSqlResponse3*)npMessage->msgpara();
	if (tpExecuteSql == NULL)
	{
		LOG_ERROR( "judge","FireSession Fail" );    
		return;                                                                                         
	}

	int nSessionID = tpExecuteSql->sessionid();
	CSession* pSession = (CSession*) CSceneObjManager::GetSingletonPtr()->GetObject( nSessionID );
	if (pSession == NULL)
	{
		LOG_ERROR( "judge","Get SessionObj failure SessionID(%d) ", nSessionID );    
		return;                                                                                         
	}

	switch ( pSession->GetSessionType() )
	{
	case em_dbsession_judge_query:
		{
			OnDBSessionQueryJudge( npMessage, nParam1, nParam2 );
			break;
		}
	default:
		break;
	}
	return ;
}

// *******************************************************
//	Function Name : OnClientMessage
//	Description   : 
//	Input Params  :
// *******************************************************

void CJudgeModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{	
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	int nMessageID = pMessage->mutable_msghead()->messageid();

	switch ( nMessageID )
	{
	case ID_C2S_REQUEST_MODIFYJUDGESINGLE:
	case ID_C2S_REQUEST_MODIFYJUDGELIST:
	case ID_C2S_REQUEST_QUERYJUDGELIST:
		{
			OnMessageClient2Gate( pPlayer, pMessage );
			break;
		}
	default:
		break;
	}
	
	return ;
}

void CJudgeModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	return ;
}

// *******************************************************
//	Function Name : OnDestroyEntity
//	Description   : 
// *******************************************************

void CJudgeModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{
	;
}

// *******************************************************
//	Function Name : OnSaveData
//	Description   : 
//	Input Params  :
// *******************************************************

bool CJudgeModule::OnSaveData( CEntityPlayer *pPlayer, int vSaveCode ) 
{
	return true;
}

void CJudgeModule::OnMessageClient2Gate(CEntityPlayer *pPlayer, CMessage *pMessage)
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	int tMessageID = pMessage->mutable_msghead()->messageid();
	
	int tRet = 0;
	switch ( tMessageID )
	{
	case ID_C2S_REQUEST_MODIFYJUDGESINGLE:
		{
			tRet = CanJudge( pPlayer );
			break;
		}
	default:
		break;
	}

	if ( tRet < 0 )
	{
		if ( ERR_NEED_NOTIFY( tRet ) )
		{
			CMessage tMessage;
			CMessageModifySingleJudgeResponse tPara;
			tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_MODIFYJUDGESINGLE );
			tMessage.set_msgpara( (long) &tPara );
			tPara.set_rtncode( tRet );
			CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		}
		return ;
	}
	 
	CSceneLogic::GetSingleton().Send2Gate( pMessage );

	return ;
}

int CJudgeModule::CanJudge( CEntityPlayer* pPlayer )
{
	if ( pPlayer == NULL )
	{
		return -1;
	}
	
	CPropertyPlayer* tpProperty = ( CPropertyPlayer* ) pPlayer->GetProperty();

	if ( tpProperty->InJudgeCoolTime() )
	{
		return ERR_JUDGE_COOLTIME;
	}

	if ( pPlayer->GetLevel() < JUDGE_BASE_LEVEL )
	{
		return ERR_JUDGE_LEVELLOWER;
	}
	return 0;	
}

// 处理网关通知 [12/8/2009 Macro]
void CJudgeModule::OnGateModifyJudgeNotice(CMessage *npMessage)
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageJudgeModifySuccessNotice* tpNotice = ( CMessageJudgeModifySuccessNotice* ) npMessage->msgpara();
	
	CEntityPlayer* 	tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpNotice->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CPropertyPlayer* tpProperty = ( CPropertyPlayer* ) tpPlayer->GetProperty();
	tpProperty->SetLastJudgeTime( time( NULL ) );
	return ;	
}

// 查询评价数据库 [12/10/2009 Macro]
void CJudgeModule::OnEventQueryJudge( int nAccountID, int nRoleID, int nType, int nParam /* = 0 */ )
{
	char tSQL[ 1024 ] = { 0 };

	switch ( nType )
	{
	case Judge_Query_Rec_Cnt:
		{
			sprintf( tSQL, "select count( role_des ) from UMS_JUDGE_FAVOR where role_des = %d;", nRoleID );
			break;
		}
	case Judge_Query_Rec_Max:
		{
			sprintf( tSQL, "select max( value ) from UMS_JUDGE_FAVOR where role_des = %d;", nRoleID );
			break;
		}
	case Judge_Query_Rec_Sum:
		{
			sprintf( tSQL, "select sum( value ) from UMS_JUDGE_FAVOR where role_des = %d;", nRoleID );
			break;
		}
	default:
		break;
	}		

	if ( tSQL[ 0 ] == '\0' )
	{
		return ;
	}	

	CSceneLogic::GetSingletonPtr()->ExecuteSql( em_dbsession_judge_query, nRoleID, nParam, nAccountID, 
		SQL_NEED_CALLBACK, SELECT, tSQL );

	return ;
}

void CJudgeModule::OnDBSessionQueryJudge(CMessage *npMessage, int nParam1, int nParam2)
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3*)npMessage->msgpara();
	if (tpMsg == NULL)
	{
		LOG_ERROR( "judge","FireSession Fail" );    
		return;                                                                                         
	}

	if ( tpMsg->resultcode() == 0 )
	{
		LOG_ERROR( "judge", "SQL Failed, judge query.charid[%d],[%s:%d]", nParam1, __LK_FILE__, __LINE__ );
		return ; 
	}

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( nParam1 );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "judge", "Player [%d] not in scene.[%s:%d]", nParam1, __LK_FILE__, __LINE__ );
		return ; 
	}

	int tRowCnt = tpMsg->rowcount();
	//int tColCnt = tpMsg->colcount();

	if ( tRowCnt != 1 )
	{
		LOG_NOTICE( "judge", "Query result none.charid[%d],[%s:%d]", nParam1, __LK_FILE__, __LINE__ );
		return ;
	}

	int tValue = atoi( tpMsg->fieldvalue( 0 ).c_str() );	
	
	CAchieveModule::GetSingleton().OnEventFireAchieve( tpPlayer, tValue, nParam2 );
	return ;
}
