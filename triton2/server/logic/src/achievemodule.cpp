/*
 *	Achieve Module CPP
 *  Created By: Gao Hong
 */

// Include Head Files
#include "shm.h"
#include "template.h"

#include "mailmodule.h"
#include "achievemodule.h"
#include "dbinterface.h"
#include "scenecfg_manager.h"
#include "mapmodule.h"
#include "propertymodule.h"

template<> CAchieveModule* CSingleton< CAchieveModule >::spSingleton = NULL;
char CAchieveModule::sModuleName[ NAME_LENGTH ] = "AchieveModule";

unsigned int CAchieveModule::CountSize()
{
	return sizeof( CAchieveModule ); 
}

// 加载模块 [11/18/2009 Macro]
void CAchieveModule::OnLaunchServer()
{
	ACHIEVE_LOG* tpLog = CSceneCfgManager::GetSingletonPtr()->GetAchieve_log();
	if ( tpLog != NULL )
	{
		INIT_ROLLINGFILE_LOG( tpLog->mIdentifier, tpLog->mDirectory, ( LogLevel ) tpLog->mPriority, tpLog->mMaxfilesize, tpLog->mMaxbackupindex );
	}
	LoadAchieveCfg();
	const char* pcConfigPath = CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"scene_config/achieve_server.xml");
	CSceneServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CAchieveModule::LoadAchieveConfigCallback);
}

//  [11/18/2009 Macro]
void CAchieveModule::OnExitServer(){ ; }

// 时间片逻辑处理 [11/18/2009 Macro]
void CAchieveModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	;
}

//  [11/18/2009 Macro]
int	 CAchieveModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )
{
	return SUCCESS;
}

//  [11/18/2009 Macro]
void CAchieveModule::OnRouterMessage( CMessage* pMessage ){ ; }

//  [11/18/2009 Macro]
void CAchieveModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	// Load Achievement data from db

	if ( pEntity->IsPlayer() == false )
	{
		return ;
	}

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) pEntity;

	LoadAchieveDBData( tpPlayer->GetAccountID( ), tpPlayer->GetCharID() );

	return ;
}

//  [11/18/2009 Macro]
void CAchieveModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{
	// Delete Achieve Data From Memory
	if ( pEntity->IsPlayer() == false )
	{
		return ;
	}

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) pEntity;

	OnEventClearAchieveMemory( tpPlayer );
	return ;
}

// 存盘处理 [11/18/2009 Macro]
bool CAchieveModule::OnSaveData( CEntityPlayer* pPlayer, int vSaveCode ) 
{
	// save achievement data
	OnEventSaveAchieveSession( pPlayer );
	return true;
}

//  [11/18/2009 Macro]
void CAchieveModule::OnLoadConfig( ){ ; }

// 处理客户端消息请求 [11/18/2009 Macro]
void CAchieveModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return ;
	}

	int tMessageID = pMessage->mutable_msghead()->messageid();
	switch ( tMessageID )
	{
	case ID_C2S_REQUEST_REPORTACHIEVE:
		{
			OnMessageReportAchieveRequest( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_GETSINGLEACHIEVE:
		{
			OnMessageObtainSingleAchieveRequest( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_GETALLACHIEVE:
		{
			OnMessageObtainAllAchieveRequest( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_GETALLHONOR:
		{
			OnMessageObtainAllHonorRequest( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_GETSTAGEBONUS:
		{
			OnMessageGetStageBonusRequest( pPlayer, pMessage );
			break;
		}
	default:
		break;
	}
	return ;
}

void CAchieveModule::OnDBSession( CMessage* npMessage, int nParam1, int nParam2 )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageExecuteSqlResponse3* tpExecuteSql = (CMessageExecuteSqlResponse3*)npMessage->msgpara();
	if (tpExecuteSql == NULL)
	{
		LOG_ERROR( "achieve","FireSession Fail" );    
		return;                                                                                         
	}

	int nSessionID = tpExecuteSql->sessionid();
	CSession* pSession = (CSession*) CSceneObjManager::GetSingletonPtr()->GetObject( nSessionID );
	if (pSession == NULL)
	{
		LOG_ERROR( "achieve","Get SessionObj failure SessionID(%d) ", nSessionID );    
		return;                                                                                         
	}

	switch ( pSession->GetSessionType() )
	{
	case em_dbsession_achieve_save:
		{
			OnSessionSaveAchieveDB( npMessage, nParam1, nParam2 );
			break;
		}
	case em_dbsession_achieve_query:
		{
			OnSessionQueryAchieveDB( npMessage, nParam1, nParam2 );
			break;
		}
	default:
		break;
	}
	
	return ;
}

// 加载成就配置文件
int CAchieveModule::LoadAchieveCfg()
{
	TiXmlDocument tDoc;
	TiXmlElement *tpRoot, *tpElement;
	//if( tDoc.LoadFile( "../config/data/scene_config/achieve_server.xml" ) == false )
	//	return -1;

	if( tDoc.LoadFile( CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"scene_config/achieve_server.xml") ) == false )
		return -1;
	tpRoot = tDoc.RootElement();
	if( tpRoot == NULL )
		return -1;
	
	tpElement = tpRoot->FirstChildElement();
	while ( tpElement != NULL )
	{
		const char* tpID	= tpElement->Attribute( "ID" );
		const char* tpPoint = tpElement->Attribute( "AchievePoint" );
		const char* tpAward = tpElement->Attribute( "AwardID" );
		const char* tpType	= tpElement->Attribute( "AchieveType" );
		const char* tpRoot	= tpElement->Attribute( "Parent" );
		const char* tpRelat = tpElement->Attribute( "Relation" );
		const char* tpHonor = tpElement->Attribute( "Honor" );
		
		if ( tpID == NULL || tpPoint == NULL || tpAward == NULL 
			|| tpType == NULL || tpRoot == NULL || tpRelat == NULL
			|| tpHonor == NULL )
		{
			printf( "Achieve Config File format error!\n" );
			return -1;
		}

		int tID		= atoi( tpID );
		int tPoint	= atoi( tpPoint );
		int tAward  = atoi( tpAward );
		int tType	= atoi( tpType );
		int tRoot	= atoi( tpRoot );
		int tRelat  = atoi( tpRelat );
		int tHonor	= atoi( tpHonor );

		ACHIEVE_CFG_LIST::iterator it = mAchieveCfgList.find( tID );
		if ( it != mAchieveCfgList.end() )
		{
			LOG_ERROR( "achieve", "Achieve ID[%d] Duplicate![%s:%d]", tID, __LK_FILE__, __LINE__ );
			tpElement = tpElement->NextSiblingElement();
			continue;
		}

		CAchieveCfgItem tAchieveCfg;
		tAchieveCfg.SetPoint( tPoint );
		tAchieveCfg.SetParamType( tType );
		tAchieveCfg.SetAward( tAward );
		tAchieveCfg.SetID( tID );
		tAchieveCfg.SetRoot( tRoot );
		tAchieveCfg.SetRelation( tRelat );
		tAchieveCfg.SetHonor( tHonor );

		TiXmlElement* tpNode = tpElement->FirstChildElement();
		while ( tpNode != NULL )
		{
			const char* tpConditionType = tpNode->Attribute( "ConditionType" );
			const char* tpValue			= tpNode->Attribute( "Param1" );
			const char* tpParam			= tpNode->Attribute( "Param2" );

			if ( tpConditionType == NULL || tpValue == NULL || tpParam == NULL )
			{
				printf( "Achieve Config File format error!\n" );
				return -1;
			}

			int tConditionType	= atoi( tpConditionType );
			int tValue			= atoi( tpValue );
			int tParam			= atoi( tpParam );

			ST_ACHIEVE_PARAM tAchieveParam;
			tAchieveParam.mType = tConditionType;
			tAchieveParam.mValue = tValue;
			tAchieveParam.mParam = tParam;
			tAchieveCfg.PushParam( tAchieveParam );
			tpNode = tpNode->NextSiblingElement();
		}

		mAchieveCfgList.insert( ACHIEVE_CFG_LIST::value_type( tID, tAchieveCfg ) );

		tpElement = tpElement->NextSiblingElement();
	}
	
	return SUCCESS;
}

// 成就上报
void CAchieveModule::OnMessageReportAchieveRequest(CEntityPlayer *npPlayer, CMessage *npMessage)
{
	if ( npPlayer == NULL || npMessage == NULL )
	{
		return ;
	}
	
	CMessageAchieveReportRequest* tpReq = ( CMessageAchieveReportRequest* ) npMessage->msgpara(); 

	int tAchieveUnitID = GetAchieveDataUnit( npPlayer->GetCharID() );
	CAchieveDataUnit* tpAchieveUnit = ( CAchieveDataUnit* ) CSceneObjManager::GetSingleton().GetObject( tAchieveUnitID ); 
	if ( tpAchieveUnit == NULL )
	{
		LOG_ERROR( "achieve", "Player[%d] Achieve Data Not Found.[%s:%d]", npPlayer->GetCharID(), __LK_FILE__, __LINE__ );
		return ;
	}

	int tAchieveID = tpReq->achieveid();

	CAchieveCfgItem* tpAchieveCfg = GetAchieveCfgItem( tAchieveID );
	if ( tpAchieveCfg == NULL )
	{
		LOG_ERROR( "achieve", "Achieve Cfg Not Found,ID[%d],[%s:%d]", tpReq->achieveid(), __LK_FILE__, __LINE__ );
		return ;
	}
	
	if ( tpAchieveUnit->AchieveDataExist( tAchieveID ) == false )
	{
		CAchieveData tData;
		tData.SetID( tAchieveID );
		tData.SetRootType( tpAchieveCfg->GetRoot() );
		tpAchieveUnit->SetAchieveData( tData );
	}
	
	ST_ACHIEVE_DATA tStData;
	tStData.mKey = tpReq->key();
	tStData.mValue = tpReq->value();
	int tRet = tpAchieveUnit->UpdateAchieveData( tAchieveID, &tStData, tpAchieveCfg );
	if ( tRet == ACHIEVE_FINISH )
	{
		// 获得奖励
		OnEventSendAward( npPlayer, tpAchieveCfg->GetAward(), tpAchieveCfg->GetID() );
		
		// 发送成就完成通知
		SendAchieveFinishNotice( npPlayer, tAchieveID );

		// 功德处理
		OnEventHonor( npPlayer, tpAchieveUnit, tpAchieveCfg );

	}
	return ;
}

// 获取成就数据请求
void CAchieveModule::OnMessageObtainSingleAchieveRequest(CEntityPlayer *npPlayer, CMessage *npMessage)
{
	if ( npPlayer == NULL || npMessage == NULL )
	{
		return ;
	}
	
	CMessageAchieveGetSingleRequest* tpReq = ( CMessageAchieveGetSingleRequest* ) npMessage->msgpara();

	int tAchieveUnitID = GetAchieveDataUnit( npPlayer->GetCharID() );
	CAchieveDataUnit* tpAchieveUnit = ( CAchieveDataUnit* ) CSceneObjManager::GetSingleton().GetObject( tAchieveUnitID ); 
	if ( tpAchieveUnit == NULL )
	{
		LOG_ERROR( "achieve", "Player[%d] Achieve Data Not Found.[%s:%d]", npPlayer->GetCharID(), __LK_FILE__, __LINE__ );
		return ;
	}

	int tAchieveID = tpReq->achieveid();

	CMessage tMessage;
	CMessageAchieveGetSingleResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETSINGLEACHIEVE );
	tMessage.set_msgpara( (long) &tPara );

	tpAchieveUnit->GetAchievePB( tAchieveID, tPara.mutable_achieve() );
	CSceneLogic::GetSingleton().Send2Player( npPlayer, &tMessage );
	return ;
}

// 获取所有成就数据请求
void CAchieveModule::OnMessageObtainAllAchieveRequest(CEntityPlayer *npPlayer, CMessage *npMessage)
{
	if ( npPlayer == NULL || npMessage == NULL )
	{
		return ;
	}

	int tAchieveUnitID = GetAchieveDataUnit( npPlayer->GetCharID() );
	CAchieveDataUnit* tpAchieveUnit = ( CAchieveDataUnit* ) CSceneObjManager::GetSingleton().GetObject( tAchieveUnitID ); 
	if ( tpAchieveUnit == NULL )
	{
		LOG_ERROR( "achieve", "Player[%d] Achieve Data Not Found.[%s:%d]", npPlayer->GetCharID(), __LK_FILE__, __LINE__ );
		return ;
	}

	CMessage tMessage;
	CMessageAchieveGetAllResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETALLACHIEVE );
	tMessage.set_msgpara( (long) &tPara );

	tPara.set_totalpoint( tpAchieveUnit->GetTotalPoint() );
	tpAchieveUnit->GetTlvPB( tPara.mutable_achievebuffer() );
	tPara.set_stage( tpAchieveUnit->GetStage() );
	CSceneLogic::GetSingleton().Send2Player( npPlayer, &tMessage );
	return ;
}

// 获得所有功德数据请求 [12/7/2009 Macro]
void CAchieveModule::OnMessageObtainAllHonorRequest( CEntityPlayer* npPlayer, CMessage* npMessage )
{
	if ( npPlayer == NULL || npMessage == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageGetAllHonorResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETALLHONOR );
	tMessage.set_msgpara( (long) &tPara );

	int tAchieveUnitID = GetAchieveDataUnit( npPlayer->GetCharID() );
	CAchieveDataUnit* tpAchieveUnit = ( CAchieveDataUnit* ) CSceneObjManager::GetSingleton().GetObject( tAchieveUnitID ); 
	if ( tpAchieveUnit == NULL )
	{
		LOG_ERROR( "achieve", "Player[%d] Achieve Data Not Found.[%s:%d]", npPlayer->GetCharID(), __LK_FILE__, __LINE__ );
		return ;
	}

	tpAchieveUnit->GetHonorPBList( &tPara );
	CSceneLogic::GetSingleton().Send2Player( npPlayer, &tMessage );
	return ;
}

// 发送成就完成通知
void CAchieveModule::SendAchieveFinishNotice( CEntityPlayer* npPlayer, int nAchieveID )
{
	if ( npPlayer == NULL )
	{
		return ;
	}	

	int tAchieveUnitID = GetAchieveDataUnit( npPlayer->GetCharID() );
	CAchieveDataUnit* tpAchieveUnit = ( CAchieveDataUnit* ) CSceneObjManager::GetSingleton().GetObject( tAchieveUnitID ); 
	if ( tpAchieveUnit == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageAchieveFinishNotice tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_ACHIEVENOTICE );
	tMessage.set_msgpara( (long) &tPara );

	tPara.set_charid( npPlayer->GetCharID() );
	tPara.set_name( npPlayer->GetCharName() );
	tPara.set_totalpoint( tpAchieveUnit->GetTotalPoint() );
	tpAchieveUnit->GetAchievePB( nAchieveID, tPara.mutable_achieve() );
	CMapModule::GetSingleton().MessageBroadCast( npPlayer, &tMessage, false, true );
	return ;
}

int CAchieveModule::GetAchieveDataUnit(int nCharID)
{
	ACHIEVE_UNIT_TYPE::iterator it = mAchieveUnitList.find( nCharID );
	if ( it == mAchieveUnitList.end() )
	{
		return INVALID_OBJ_ID;
	}
	
	return it->second;
}

int CAchieveModule::DelAchieveDataUnit(int nCharID)
{
	ACHIEVE_UNIT_TYPE::iterator it = mAchieveUnitList.find( nCharID );
	if ( it != mAchieveUnitList.end() )
	{
		mAchieveUnitList.erase( it );
	}

	return SUCCESS;
}

// 获得成就配置单元 [12/9/2009 Macro]
CAchieveCfgItem* CAchieveModule::GetAchieveCfgItem(int nAchieveID)
{
	ACHIEVE_CFG_LIST::iterator it = mAchieveCfgList.find( nAchieveID );
	if ( it == mAchieveCfgList.end() )
	{
		return NULL;
	}

	return (CAchieveCfgItem*)&(it->second);	
}

// 加载玩家成就数据(同步数据库) [11/18/2009 Macro]
int CAchieveModule::LoadAchieveDBData( int nAccountID, int nCharID )
{
	char tSQL[ 1024 ] = { 0 };
	sprintf( tSQL, "select role_id, achieve_info from UMS_ACHIEVE where role_id = %d;", nCharID );

	CSceneLogic::GetSingletonPtr()->ExecuteSql( em_dbsession_achieve_query, nCharID, 0, nCharID, 
		SQL_NEED_CALLBACK, SELECT, tSQL );

	return SUCCESS;
}

// 成就数据存盘回调 [11/18/2009 Macro]
void CAchieveModule::OnSessionQueryAchieveDB( CMessage* npMessage, int nParam1, int nParam2 )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageExecuteSqlResponse3* tpSqlRes = ( CMessageExecuteSqlResponse3* ) npMessage->msgpara();

	if ( tpSqlRes->resultcode() == 0 )
	{
		LOG_ERROR( "achieve", "Load db failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return ;
	}

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( nParam1 );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	//// 角色在数据库没有成就数据记录
	//// 需要新建一条记录插入数据库
	//if ( tpSqlRes->rowcount() == 0 )
	//{
	//	OnEventInitAchieveDB( tpPlayer );
	//	return ;
	//}

	CAchieveDataUnit* tpAchieveUnit = NULL;
	
	int tAchieveUnitID = GetAchieveDataUnit( nParam1 );
	if ( tAchieveUnitID == INVALID_OBJ_ID )
	{
		tpAchieveUnit = ( CAchieveDataUnit* ) CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ACHIEVE );
		if ( tpAchieveUnit != NULL )
			mAchieveUnitList.insert( ACHIEVE_UNIT_TYPE::value_type( nParam1, tpAchieveUnit->get_id() ) );		
	}
	else
	{
		tpAchieveUnit = ( CAchieveDataUnit* ) CSceneObjManager::GetSingleton().GetObject( tAchieveUnitID );		
	}

	if ( tpAchieveUnit == NULL )
	{
		LOG_ERROR( "achieve", "Achieve Unit Object Error.[%s:%d]", __LK_FILE__, __LINE__ );
		return ;
	}

	tpAchieveUnit->InitFromSession( npMessage );
	tpAchieveUnit->SetOwnerCharID( nParam1 );

	// 通知客户端所有成就数据
	CMessage tMessage;
	OnMessageObtainAllAchieveRequest( tpPlayer, &tMessage );
	return ;
}

// 成就存盘回调 [12/9/2009 Macro]
void CAchieveModule::OnSessionSaveAchieveDB(CMessage *npMessage, int nParam1, int nParam2)
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageExecuteSqlResponse3* tpSqlRes = ( CMessageExecuteSqlResponse3* ) npMessage->msgpara();

	if ( tpSqlRes->resultcode() == 0 )
	{
		LOG_ERROR( "achieve", "Save Achieve DB SQL failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return ;
	}
	
	return ;
}

// 玩家成就数据存盘 [11/18/2009 Macro]
void CAchieveModule::OnEventSaveAchieveSession( CEntityPlayer* npPlayer )
{
	if ( npPlayer == NULL )
	{
		return ;
	}
	
	int tAchieveUnitID = GetAchieveDataUnit( npPlayer->GetCharID() );
	CAchieveDataUnit* tpAchieveUnit = ( CAchieveDataUnit* ) CSceneObjManager::GetSingleton().GetObject( tAchieveUnitID );
	if ( tpAchieveUnit == NULL )
	{
		LOG_ERROR( "achieve", "Player[%d] Achieve Data Not Found.[%s:%d]", npPlayer->GetCharID(), __LK_FILE__, __LINE__ );
		return ;
	}

	tpAchieveUnit->SaveSessionData( npPlayer );
	return ;
}

// 新建玩家成就数据库记录 [11/18/2009 Macro]
void CAchieveModule::OnEventInitAchieveDB(CEntityPlayer *npPlayer)
{
	if ( npPlayer == NULL )
	{
		return ;
	}

	CSceneLogic::GetSingleton().ExecuteSql( em_dbsession_achieve_save, npPlayer->GetCharID(), npPlayer->GetAccountID(), 
		npPlayer->GetCharID(), SQL_NONE, INSERT, "insert into UMS_ACHIEVE(role_id, achieve_info) values('%d', '%s');", 
		npPlayer->GetCharID(), "" );

	// 内存也插入
	int tAchieveUnitID = GetAchieveDataUnit( npPlayer->GetCharID() );
	if ( tAchieveUnitID == INVALID_OBJ_ID )
	{
		CAchieveDataUnit* tpUnit = ( CAchieveDataUnit* ) CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_ACHIEVE );
		if ( tpUnit != NULL )
		{
			tpUnit->SetOwnerCharID( npPlayer->GetCharID() );
			mAchieveUnitList.insert( ACHIEVE_UNIT_TYPE::value_type( npPlayer->GetCharID(), tpUnit->get_id() ) );
		}
	}	
	
	return ;
}

// 成就奖励处理 [11/23/2009 Macro]
void CAchieveModule::OnEventSendAward( CEntityPlayer* npPlayer, int nAwardID, int nID )
{
	if ( npPlayer == NULL || nAwardID == 0 )
	{
		return ;
	}	

	int tItemIDList = nAwardID;
	int tItemNumList  = 1;
	int tNum = 1;	
	int tSysID = nID + ACHIEVE_SYSMAIL_BASEID;
	if ( tNum > 0 )
	{
		CMailModule::GetSingleton().SendSysMailBySysID( tSysID, npPlayer->GetCharName(), npPlayer->GetCharID(), &tItemIDList, &tItemNumList, tNum, npPlayer->GetNationality(), 0 );
	}	
	return ;
}

// 功德处理 [12/7/2009 Macro]
void CAchieveModule::OnEventHonor( CEntityPlayer* npPlayer, CAchieveDataUnit* npUnit, CAchieveCfgItem* npCfgItem )
{
	if ( npPlayer == NULL || npUnit == NULL || npCfgItem == NULL )
	{
		return ;
	}
	
	int tHonor = npCfgItem->GetHonor();
	if ( tHonor == INVALID_MAP_ID )
	{
		return ;
	}

	ST_HONOR_DATA tData;
	tData.mMapID = tHonor;
	tData.mTime = time( NULL );
	if ( npUnit->PushHonor( tData ) < 0 )
	{
		return ;
	}

	CMessage tMessage;
	CMessageHonorFinishNotice tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_HONORFINSH );
	tMessage.set_msgpara( (long) &tPara );
	PBHonorData* tpPBHonor = tPara.mutable_honordata();
	tpPBHonor->set_mapid( tData.mMapID );
	tpPBHonor->set_time( tData.mTime );
	CSceneLogic::GetSingleton().Send2Player( npPlayer, &tMessage );
	return ;
		
}

// 清理内存成就数据 [12/8/2009 Macro]
void CAchieveModule::OnEventClearAchieveMemory(CEntityPlayer *npPlayer)
{
	if ( npPlayer == NULL )
	{
		return ;
	}

	int tAchieveUnitID = GetAchieveDataUnit( npPlayer->GetCharID() );
	
	if ( tAchieveUnitID != INVALID_OBJ_ID )
	{
		DelAchieveDataUnit( npPlayer->GetCharID() );
		CSceneObjManager::GetSingleton().DestroyObject( tAchieveUnitID );
	}
	 
	return ;
}

// 处理由其他模块的成就进展触发 [12/10/2009 Macro]
// 通常：nValue: 过程值， nParam: 相关成就ID [12/10/2009 Macro]
void CAchieveModule::OnEventFireAchieve(CEntityPlayer *npPlayer, int nValue, int nParam)
{
	if ( npPlayer == NULL )
	{
		return; 
	}
	
	int tAchieveUnitID = GetAchieveDataUnit( npPlayer->GetCharID() );
	CAchieveDataUnit* tpAchieveUnit = ( CAchieveDataUnit* ) CSceneObjManager::GetSingleton().GetObject( tAchieveUnitID );
	if ( tpAchieveUnit == NULL )
	{
		LOG_ERROR( "achieve", "Player[%d] Achieve Data Not Found.[%s:%d]", npPlayer->GetCharID(), __LK_FILE__, __LINE__ );
		return ;
	}

	CAchieveCfgItem* tpAchieveCfg = GetAchieveCfgItem( nParam );
	if ( tpAchieveCfg == NULL )
	{
		LOG_ERROR( "achieve", "Achieve Cfg Not Found,ID[%d],[%s:%d]", nParam, __LK_FILE__, __LINE__ );
		return ;
	}

	if ( tpAchieveUnit->AchieveDataExist( nParam ) == false )
	{
		CAchieveData tData;
		tData.SetID( nParam );
		tData.SetRootType( tpAchieveCfg->GetRoot() );
		tpAchieveUnit->SetAchieveData( tData );
	}

	int tRet = tpAchieveUnit->UpdateAchieveDataInServer( nParam, nValue, tpAchieveCfg );
	if ( tRet == ACHIEVE_FINISH )
	{
		// 获得奖励
		OnEventSendAward( npPlayer, tpAchieveCfg->GetAward(), tpAchieveCfg->GetID() );

		// 发送成就完成通知
		SendAchieveFinishNotice( npPlayer, nParam );

		// 功德处理
		OnEventHonor( npPlayer, tpAchieveUnit, tpAchieveCfg );
	}
	
	return ;
}

void CAchieveModule::OnEventInsertHonor(CEntityPlayer *npPlayer, int nMapID)
{
	if ( npPlayer == NULL )
	{
		return ;
	}
	
	int tAchieveUnitID = GetAchieveDataUnit( npPlayer->GetCharID() );
	CAchieveDataUnit* tpAchieveUnit = ( CAchieveDataUnit* ) CSceneObjManager::GetSingleton().GetObject( tAchieveUnitID ); 
	if ( tpAchieveUnit == NULL )
	{
		LOG_ERROR( "achieve", "Player[%d] Achieve Data Not Found.[%s:%d]", npPlayer->GetCharID(), __LK_FILE__, __LINE__ );
		return ;
	}

	ST_HONOR_DATA tData;
	tData.mMapID = nMapID;

	tpAchieveUnit->PushHonor( tData );

	CMessage tMessage;
	CMessageHonorFinishNotice tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_HONORFINSH );
	tMessage.set_msgpara( (long) &tPara );
	PBHonorData* tpPBHonor = tPara.mutable_honordata();
	tpPBHonor->set_mapid( nMapID );
	tpPBHonor->set_time( (int) time( NULL ) );
	CSceneLogic::GetSingleton().Send2Player( npPlayer, &tMessage );

	return ; 
}

void CAchieveModule::OnMessageGetStageBonusRequest( CEntityPlayer* npPlayer, CMessage* npMessage )
{
	if ( npPlayer == NULL || npMessage == NULL )
	{
		return ;
	}
	CPropertyPlayer* pProperty = (CPropertyPlayer*)npPlayer->GetProperty();

	CMessage tMessage;
	CMessageGetStageBonusResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETSTAGEBONUS );
	tMessage.set_msgpara( (long) &tPara );	

	int tAchieveUnitID = GetAchieveDataUnit( npPlayer->GetCharID() );
	CAchieveDataUnit* tpAchieveUnit = ( CAchieveDataUnit* ) CSceneObjManager::GetSingleton().GetObject( tAchieveUnitID ); 
	if ( tpAchieveUnit == NULL )
	{
		LOG_ERROR( "achieve", "Player[%d] Achieve Data Not Found.[%s:%d]", npPlayer->GetCharID(), __LK_FILE__, __LINE__ );
		return ;
	}
	tPara.set_stage( tpAchieveUnit->GetStage() );

	CTplAchieveAward *tpAward = (CTplAchieveAward*)CDataStatic::GetTemp( TEMP_ACHIEVEAWARD );
	if ( tpAward == NULL )
	{
		return;
	}

	// 找到第一个能领且没有领的奖励
	int tNext = tpAchieveUnit->GetStage() + 1;
	if ( tNext >= MAX_ACHIEVE_AWARD )
	{
		tPara.set_result( ERR_ACHIEVE_MAX );
		CSceneLogic::GetSingleton().Send2Player( npPlayer, &tMessage );
		return ;
	}
	else if ( tpAward->mPoint[ tNext ] > tpAchieveUnit->mTotalPoint )
	{
		tPara.set_result( ERR_ACHIEVE_NO );
		CSceneLogic::GetSingleton().Send2Player( npPlayer, &tMessage );
		return ;
	}

	// 检查包裹
	if ( pProperty->TestBaggageSpace( tpAward->mAwardID[ tNext ], tpAward->mAwardNum[ tNext ], 1 ) == false )
	{
		tPara.set_result( ERR_ACHIEVE_FULL );
		CSceneLogic::GetSingleton().Send2Player( npPlayer, &tMessage );
		return ;
	}

	// 发放奖励
	for ( unsigned int i = 0; i < ARRAY_CNT(tpAward->mAwardID[ tNext ]) && i < ARRAY_CNT(tpAward->mAwardNum[ tNext ]); i++ )
	{
		if ( tpAward->mAwardID[ tNext ][ i ] == 0 )
			break;
		CPropertyModule::GetSingleton().PlayerInsertItem( npPlayer, tpAward->mAwardID[ tNext ][ i ], tpAward->mAwardNum[ tNext ][ i ] );
	}

	tPara.set_result( SUCCESS );
	tpAchieveUnit->IncStage();
	tPara.set_stage( tNext );	
	CSceneLogic::GetSingleton().Send2Player( npPlayer, &tMessage );
	return ; 
}

void CAchieveModule::LoadAchieveConfigCallback(const char *pcConfigPath)
{
	CAchieveModule::GetSingleton().LoadAchieveCfg();
}