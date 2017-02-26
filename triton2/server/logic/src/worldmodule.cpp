#include "stdafx.hpp"
#include "entity.h"
#include "countrymessage_pb.hxx.pb.h"
#include "worldmodule.h"
#include "logservertype.h"
#include "message_maker.h"
#include "scenelogic.h"
#include "chatmodule.h"
#include "taskmodule.h"
#include "tasktype.h"
#include "mapobjmanager.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

template<> CWorldModule* CSingleton< CWorldModule >::spSingleton = NULL;
char CWorldModule::sModuleName[ NAME_LENGTH ] = "World_1";

CWorldModule* gpWorldManager = NULL;

unsigned int CWorldModule::CountSize()
{
	unsigned int tSize = sizeof( CWorldModule );

	return tSize;
}

void CWorldModule::OnLaunchServer( )
{
	OnLoadConfig();
	memset( mNationStatus, 0, sizeof(mNationStatus) );
	memset( mKingID, 0, sizeof(mKingID) );
	mSaveTimer = CWTimer(500);
	mSaveFin = true;
	mWarSerial = 0;
	mTeleList.initailize();
	memset( mStone, 0 , sizeof(mStone) );
	mRateOrder = DEFAULT_RATE;
	mStoneTimer = CWTimer(20000);
	mWinWorld = 0;
}

void CWorldModule::OnLoadConfig()
{
	/*TRADE_LOG* tpLog = CSceneCfgManager::GetSingletonPtr()->GetTrade_log();
	if( tpLog != NULL )
		INIT_ROLLINGFILE_LOG( tpLog->mIdentifier, tpLog->mDirectory, ( LogLevel ) tpLog->mPriority , tpLog->mMaxfilesize, tpLog->mMaxbackupindex);	*/
}

void CWorldModule::OnExitServer( )
{
}

void CWorldModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{	
	if ( IS_BATTLE_LINE_ID( CSceneLogic::GetSingleton().GetLineID() ) == false )
		return;
	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;
	for ( TeleList::iterator it = mTeleList.begin(); it != mTeleList.end(); ++it )
	{
		CEntity *tpEntity = CSceneLogic::GetSingleton().GetEntity( *it );
		if ( tpEntity == NULL )
			continue;
		if ( tpEntity->IsPlayer() == false )
			continue;
		CEntityPlayer *tpPlayer = (CEntityPlayer*)tpEntity;
		CMapModule::GetSingleton().Teleport( tpPlayer->GetNationality( ), tpConfig->mOutMapID, tpConfig->mOutPosX, tpConfig->mOutPosY, tpPlayer, 0 );
	}
	mTeleList.clear();
	if ( mSaveFin == false && mSaveTimer.Elapse( vTickOffset ) == true )
	{		
		 CSceneLogic::GetSingleton().TeleAll( tpConfig->mOutMapID, tpConfig->mOutPosX, tpConfig->mOutPosY );
	}
	if ( mStoneTimer.Elapse( vTickOffset ) == true )
	{
		SendStoneInfoNotify( );
	}
}


int CWorldModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )
{
	return SUCCESS;
}

void CWorldModule::OnRouterMessage( CMessage* pMessage )
{

}

void CWorldModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if( pMessage == NULL || pPlayer == NULL )
	{
		LOG_ERROR( "default", "Message null ! [%s : %d],[%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	CMessageHead* tpHead = ( CMessageHead* )pMessage->mutable_msghead();
	if ( tpHead == NULL )
	{
		LOG_ERROR( "trade", "Message Head null ! [%s : %d],[%s]", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ;
	}

	switch( tpHead->messageid() )
	{
	case ID_C2S_REQUEST_COUNTRYINFO:
		{
			OnMessageCountryInfo( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_JOINCOUNTRY:
		{
			OnMessageJoinCountry( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_LEAVECOUNTRY:
		{
			OnMessageLeaveCountry( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_SETOFFICIAL:
		{
			OnMessageSetOfficial( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_UNSETOFFICIAL:
		{
			OnMessageUnSetOfficial( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_USEFUNCTION:
		{
			OnMessageUseFunction( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_OFFICIALINFO:
		{
			OnMessageOfficialInfo( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_USETIME:
		{
			OnMessageUseTimes( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_WORLDQUEST:
		{
			OnMessageWorldQuest( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_WORLDANSWER:
		{
			OnMessageWorldAnswer( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_WORLDINFO:
		{
			OnMessageWorldInfo( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_GETAWARD:
		{
			OnMessageGetAward( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_SUBMITWAR:
		{
			OnMessageSubmitWar( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_ENTERWAR:
		{
			OnMessageEnterWar( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_QUERYWARCAMP:
		{
			OnMessageQueryWarCamp( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_QUERYWARNUM:
		{
			OnMessageQueryWarNum( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_QUERYWARINFO:
		{
			OnMessageQueryWarInfo( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_QUERYRANK:
		{
			OnMessageQueryWarRank( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_BIDCAMP:
		{
			OnMessageBidCamp( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_CAMPBIDINFO:
		{
			OnMessageCampBidInfo( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_CAMPRESINFO:
		{
			OnMessageCampResInfo( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_GETWARAWARD:
		{
			OnMessageGetWarAward( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_QUERYWARAWARD:
		{
			OnMessageQueryWarAward( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_MODIFYRATE:
		{
			OnMessageModifyRate( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_GETTAX:
		{
			OnMessageGetTax( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_REFUSESUMMON:
		{
			OnMessageRefuseSummon( pPlayer, pMessage );
			break;
		}
	case ID_C2S_CAMPWEAKLIST_REQUEST:
		{
			// client request week list of camp
			m_WeakProtecter.OnMessageCampWeakListReq( pPlayer, pMessage );
			break;
		}
	case ID_C2S_CAMPGETAWARD_REQUEST:
		{
			// client get award of camp
			m_WeakProtecter.OnMessageCampGetAward( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_ACCUSEKING:
		{
			OnMessageAccuseKing( pPlayer, pMessage );
			break;
		}
	case ID_C2S_REQUEST_POWERINFO:
		{
			OnMessagePowerInfo( pPlayer, pMessage );
			break;
		}
	default:
		break;
	}
	return ;
}

void CWorldModule::OnGateMessage( CMessage *pMsg )
{  
	LK_ASSERT( pMsg != NULL, return );

	// 处理公共消息
	switch ( pMsg->mutable_msghead()->messageid() )
	{
	case ID_S2C_RESPONSE_COUNTRYINFO:
		{
			OnMessageGSCountryInfoResponse( pMsg );
			break ;
		}
	case ID_S2C_RESPONSE_JOINCOUNTRY:
		{
			OnMessageGSJoinCountryResponse( pMsg );
			break ;
		}
	case ID_S2C_RESPONSE_LEAVECOUNTRY:
		{
			OnMessageGSLeaveCountryResponse( pMsg );
			break ;
		}
	case ID_S2C_RESPONSE_SETOFFICIAL:
		{
			OnMessageGSSetOfficialResponse( pMsg );
			break ;
		}
	case ID_S2C_RESPONSE_UNSETOFFICIAL:
		{
			OnMessageGSUnSetOfficialResponse( pMsg );
			break ;
		}
	case ID_S2C_RESPONSE_USEFUNCTION:
		{
			OnMessageGSUseFunctionResponse( pMsg );
			break ;
		}
	case ID_S2C_RESPONSE_OFFICIALINFO:
		{
			OnMessageGSOfficialInfoResponse( pMsg );
			break ;
		}
	case ID_S2C_RESPONSE_USETIME:
		{
			OnMessageGSUseTimesResponse( pMsg );
			break ;
		}
	case ID_G2S_NOTIFY_USEFUNCTION:
		{
			OnMessageGSUseFunctionNotify( pMsg );
			break ;
		}
	case ID_G2S_NOTIFY_SETOFFICIAL:
		{
			OnMessageGSSetOfficialNotify( pMsg );
			break ;
		}
	case ID_G2S_RESPONSE_KILLPERSON:
		{
			OnMessageGSKillPersonResponse( pMsg );
			break ;
		}
	case ID_S2C_RESPONSE_WORLDQUEST:
		{
			OnMessageGSWorldQuestResponse( pMsg );
			break ;
		}
	case ID_S2C_RESPONSE_WORLDANSWER:
		{
			OnMessageGSWorldAnswerResponse( pMsg );
			break ;
		}
	case ID_S2C_NOTIFY_WORLDQUEST:
		{
			OnMessageGSWorldQuestNotify( pMsg );
			break ;
		}
	case ID_G2S_NOTIFY_SYNCNATION:
		{
			OnMessageGSSyncNationNotify( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_WORLDINFO:
		{
			OnMessageGSWorldInfoResponse( pMsg );
			break ;
		}
	case ID_S2C_NOTIFY_JOINCOUNTRY:
		{
			OnMessageGSJoinCountryNotify( pMsg );
			break ;
		}
	case ID_S2C_NOTIFY_PLAYERGSINFO:
		{
			OnMessageGSPlayerInfoNotify( pMsg );
			break;
		}
	case ID_S2C_NOTIFY_WORLDANSWER:
		{
			OnMessageGSWorldAnswerNotify( pMsg );
			break;
		}
	case ID_S2C_NOTIFY_KINGCHANGE:
		{
			OnMessageGSKingChangeNotify( pMsg );
			break;
		}
	case ID_G2S_NOTIFY_NATIONTITLE:
		{
			OnMessageGSUpdateNationTitleNotify( pMsg );
			break;
		}
	case ID_G2S_RESPONSE_NATIONTITLELIST:
		{
			OnMessageGSNationTitleList( pMsg );
			break;
		}
	case ID_G2S_NOTIFY_REALSUMMON:
		{
			OnMessageGSRealSummonNotify( pMsg );
			break;
		}
	case ID_G2S_RESPONSE_TRYSUMMON:
		{
			OnMessageGSTrySummonResponse( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_SUBMITWAR:
		{
			OnMessageGSSubmitWarResponse( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_ENTERWAR:
		{
			OnMessageGSEnterWarResponse( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_QUERYWARCAMP:
		{
			OnMessageGSQueryWarCampResponse( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_QUERYWARNUM:
		{
			OnMessageGSQueryWarNumResponse( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_QUERYWARINFO:
		{
			OnMessageGSQueryWarInfoResponse( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_QUERYRANK:
		{
			OnMessageGSQueryWarRankResponse( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_BIDCAMP:
		{
			OnMessageGSBidCampResponse( pMsg );
			break;
		}
	case ID_S2C_NOTIFY_WARSOURCE:
		{
			OnMessageGSWarSourceNotify( pMsg );
			break;
		}
	case ID_G2S_NOTIFY_RELIVECHANGE:
		{
			OnMessageGSReliveChangeNotify( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_CAMPBIDINFO:
		{
			OnMessageGSCampBidInfoResponse( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_CAMPRESINFO:
		{
			OnMessageGSCampResInfoResponse( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_GETWARAWARD:
		{
			OnMessageGSGetWarAwardResponse( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_QUERYWARAWARD:
		{
			OnMessageGSQueryWarAwardResponse( pMsg );
			break;
		}
	case ID_G2S_NOTIFY_SYNCWARSTATUS:
		{
			OnMessageGSSyncWarStatusNotify( pMsg );
			break;
		}
	case ID_G2S_NOTIFY_CREATEWARSTONE:
		{
			OnMessageGSCreateWarStoneNotify( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_MODIFYRATE:
		{
			OnMessageGSModifyRateResponse( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_GETTAX:
		{
			OnMessageGSGetTaxResponse( pMsg );
			break;
		}
	case ID_S2C_NOTIFY_MODIFYRATE:
		{
			OnMessageGSModifyRateNotify( pMsg );
			break;
		}
	case ID_S2C_NOTIFY_REFUSESUMMON:
		{
			OnMessageGSRefuseSummonNotify( pMsg );
			break;
		}
	case ID_G2S_CAMPWEAKLIST_RESPONSE:
	case ID_G2S_CAMPWAEKLIST_NOTICE:
		{
			// syn camp list from gate
			m_WeakProtecter.OnMessageGSCampWeakList( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_ACCUSEKING:
		{
			OnMessageGSAccuseKingResponse( pMsg );
			break;
		}
	case ID_S2C_NOTIFY_ACCUSEKING:
		{
			OnMessageGSAccuseKingNotify( pMsg );
			break;
		}
	case ID_S2C_RESPONSE_POWERINFO:
		{
			OnMessageGSPowerInfoResponse( pMsg );
			break;
		}
	case ID_S2C_NOTIFY_ACCUSESTATUS:
		{
			OnMessageGSAccuseStatusNotify( pMsg );
			break;
		}
	case ID_S2C_NOTIFY_OFFICIALCHANGE:
		{
			OnMessageOfficialChange( pMsg );
			break;
		}	
	case ID_S2C_NOTIFY_MAKEGUOTAN:
		{
			OnMessageGSMakeGuoTanNotify( pMsg );
			break;
		}
	case ID_S2C_NOTIFY_MAKELUANWU:
		{
			OnMessageGSMakeLuanWuNotify( pMsg );
			break;
		}
	case ID_G2S_NOTIFY_SYNCGTANDLW:
		{
			OnMessageGSSyncGTAndLWNotify( pMsg );
			break;
		}
	default:
		break;
	}
}


void CWorldModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	if ( pEntity == NULL )
		return ;
	
	if ( !pEntity->IsPlayer() )
		return ;
	
	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) pEntity;

	// 给网关发送请求国家类称号
	CMessage tMessage;
	CMessageGetNationTitleListRequest tPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_NATIONTITLELIST );
	tMessage.set_msgpara( (long) &tPara );
	tPara.set_roleid( tpPlayer->GetCharID() );
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );

	CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();	

	// 如果是国战地图，发送场景内NPC通知
	if ( IS_BATTLE_LINE_ID( tpPlayer->GetLineID() ) == true )
	{
		if ( mWarSerial != tpProperty->GetWarSerial() )
		{
			mTeleList.push_back( tpPlayer->GetEntityID() );
		}

		CMessage tWarMessage;
		tWarMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_WARINFO );
		CMessageWarInfoNotify tNotify;
		tWarMessage.set_msgpara( (uint32)&tNotify );
		tNotify.set_mapid( tpPlayer->GetMapID( ) );
		tNotify.set_stoneid( GetStoneID( tpPlayer->GetMapID( ) ) );
		CEntity *tpEntity = ( CEntity * )CSceneLogic::GetSingleton().GetEntity( GetStoneEntity( tpPlayer->GetMapID( ) ) );
		if ( tpEntity != NULL )
		{
			tNotify.set_stonex( tpEntity->GetPosX() );
			tNotify.set_stoney( tpEntity->GetPosY() );
		}
		else
		{
			tNotify.set_stonex( GetStoneX( tpPlayer->GetMapID( ) ) );
			tNotify.set_stoney( GetStoneY( tpPlayer->GetMapID( ) ) );
		}		

		CMapTpl* tpMapTpl = CMapTplManager::GetSingletonPtr( )->GetMapTplByID( tpPlayer->GetMapID( ) );
		if ( tpMapTpl == NULL )
			return;

		for ( size_t i = 0; i < tpMapTpl->mCreators.size( ); i ++ )
		{
			CCreator* tpCreator = CCreatorManager::GetSingletonPtr()->GetCreator( tpMapTpl->mCreators[ i ] );
			if ( tpCreator != NULL )
			{
				CEntityNpc *tpNpc = (CEntityNpc*)CSceneLogic::GetSingletonPtr()->GetEntity( tpCreator->mHasOgre );
				if ( tpNpc != NULL )
				{
					NPCWarInfo *tpInfo = tNotify.add_warinfo();
					tpInfo->set_posx( tpNpc->GetPosX() );
					tpInfo->set_posy( tpNpc->GetPosY() );
					tpInfo->set_worldid( tpNpc->GetOwnCampID() );
					tpInfo->set_tempid( tpNpc->GetNpcID() );
				}
			}
		}
		CSceneLogic::GetSingleton( ).Send2Player( pEntity, &tWarMessage );
	}

	return;
}


void CWorldModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{
	return ;
}


bool CWorldModule::OnSaveData( CEntityPlayer *pPlayer, int vSaveCode )
{
	return true;
}

void CWorldModule::OnMessageCountryInfo( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageCountryInfoRequest *tpMsg = ( CMessageCountryInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageJoinCountry( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageJoinCountryRequest *tpMsg = ( CMessageJoinCountryRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	if ( HasCountryService( pPlayer, tpMsg->npcentityid(), tpMsg->countryid() ) == false )
	{
		return;
	}	
	if ( pPlayer->GetLevel() < MIN_COUNTRY_LEVEL )
	{
		return;
	}	

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageLeaveCountry( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageLeaveCountryRequest *tpMsg = ( CMessageLeaveCountryRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	if ( HasCountryService( pPlayer, tpMsg->npcentityid(), pPlayer->GetCountryID() ) == false )
	{
		return;
	}	

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageSetOfficial( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageSetOfficialRequest *tpMsg = ( CMessageSetOfficialRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageUnSetOfficial( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageUnSetOfficialRequest *tpMsg = ( CMessageUnSetOfficialRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageUseFunction( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageUseFunctionRequest *tpMsg = ( CMessageUseFunctionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	// 如果包满不能添加物品
	int tFunctionType = tpMsg->functiontype();
	int tFunctionOrder = tpMsg->functionorder();
	if ( tFunctionType < 0 || tFunctionType > OFFICIAL_FUNCTION_KING || 
		tFunctionOrder < 0 || tFunctionOrder > MAX_POWER )
	{
		return;
	}
	int tTempID = 0;
	if ( tFunctionType == OFFICIAL_FUNCTION_KING )
	{
		tTempID = CDataStatic::spKing[ tFunctionOrder ];
	}
	else
	{
		tTempID = CDataStatic::spOffical[ tFunctionType ][ tFunctionOrder ];
	}
	CTemplatePower *tpPower = (CTemplatePower *)CDataStatic::SearchTpl( tTempID );
	if ( tpPower == NULL )
		return;
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	if ( tpPower->mType == CTemplatePower::POWER_ITEM && (int)tpProperty->GetBaggage()->Space( ) == ~0 )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_INSERTITEM_NOSPACE );
		return;
	}	
	if ( tpPower->mType == CTemplatePower::POWER_SUMMON ||
		tpPower->mType == CTemplatePower::POWER_SUMMONWORLD || 
		tpPower->mType == CTemplatePower::POWER_SUMMONCOUNTRY ||
		tpPower->mType == CTemplatePower::POWER_SUMMONCORPS || 
		tpPower->mType == CTemplatePower::POWER_SUMMONFAMILY )
	{
		if ( pPlayer->HasStall() == true )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_WORLD_SUMMONINSTALL );
			return;
		}
		
		if ( CDataStatic::IsWeddingHall( pPlayer->GetMapID() ) == true )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_WORLD_SUMMONINWEDDINGHALL );			
			return;
		}
		
		if ( IS_REPETION_LINE_ID( pPlayer->GetLineID() ) == true 
			|| IS_NEUTRAL_LINE_ID( pPlayer->GetLineID() ) == true
			|| IS_BATTLE_LINE_ID( pPlayer->GetLineID() ) == true)
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_WORLD_SUMMONINREP );
			return;
		}

		if ( pPlayer->GetLineID() != pPlayer->GetNationality() )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_WORLD_SUMMONINFOREIGN );
			return;
		}

		CTplPKValueTable* tpTable = ( CTplPKValueTable* ) CDataStatic::GetTemp( TEMP_PKVALUE );
		if ( tpTable == NULL )
		{
			return;
		}

		// 检查是否在监狱
		if ( pPlayer->GetMapID() == tpTable->mMapID )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_WORLD_SUMMONINPRI );
			return;
		}
		CMapObject *tpMapObj = CMapObjManager::GetSingleton().GetMapObject( pPlayer->GetLineID(), pPlayer->GetMapID(), pPlayer->GetMapIndex() );
		LK_ASSERT( tpMapObj != NULL, return );
		// 地宫不能用
		if ( tpMapObj->IsGroundMap() == true )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_WORLD_SUMMONINGROUND );
			return;
		}
	}	

	tpMsg->set_charid( pPlayer->GetCharID() );
	tpMsg->mutable_teleport()->set_lineid( pPlayer->GetLineID() );
	tpMsg->mutable_teleport()->set_mapid( pPlayer->GetMapID() );
	tpMsg->mutable_teleport()->set_posx( pPlayer->GetPosX() );
	tpMsg->mutable_teleport()->set_posy( pPlayer->GetPosY() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageOfficialInfo( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageOfficialInfoRequest *tpMsg = ( CMessageOfficialInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageUseTimes( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageUseTimeRequest *tpMsg = ( CMessageUseTimeRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageWorldQuest( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageWorldQuestRequest *tpMsg = ( CMessageWorldQuestRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	if ( HasFriendService( pPlayer, tpMsg->npcentityid() ) == false )
	{
		return;
	}	

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageWorldAnswer( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageWorldAnswerRequest *tpMsg = ( CMessageWorldAnswerRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageWorldInfo( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageWorldInfoRequest *tpMsg = ( CMessageWorldInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageGSCountryInfoResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageCountryInfoResponse *tpMsg = ( CMessageCountryInfoResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSJoinCountryResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageJoinCountryResponse *tpMsg = ( CMessageJoinCountryResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	if ( tpMsg->result() == SUCCESS )
	{
		tpPlayer->SetCountryID( tpMsg->countryid() );

		// 添加国籍类称号
		OnEventNationTitle( tpPlayer, tpMsg->countryid(), true );
	}	

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSLeaveCountryResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageLeaveCountryResponse *tpMsg = ( CMessageLeaveCountryResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	if ( tpMsg->result() == SUCCESS )
	{
		// 删除国籍类称号,  注意逻辑时序. 修改代码时请通知作者!!!! [1/29/2010 GaoHong]
		OnEventNationTitle( tpPlayer, tpPlayer->GetCountryID(), false );

		SendChangeWizardRequest( tpPlayer->GetNationality(), tpPlayer->GetCountryID(), CHANGE_DEL, tpPlayer );
		tpPlayer->SetCountryID( COUNTRY_NONE );
		// 清除身上的国家任务
		CTaskModule::GetSingleton().ClearTasksByType( tpPlayer, TASKTYPE_NATION );
	}	

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSSetOfficialResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageSetOfficialResponse *tpMsg = ( CMessageSetOfficialResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSUnSetOfficialResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageUnSetOfficialResponse *tpMsg = ( CMessageUnSetOfficialResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );
	
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSUseFunctionResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageUseFunctionResponse *tpMsg = ( CMessageUseFunctionResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSOfficialInfoResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageOfficialInfoResponse *tpMsg = ( CMessageOfficialInfoResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSUseTimesResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageUseTimeResponse *tpMsg = ( CMessageUseTimeResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSUseFunctionNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageUseFunctionNotify *tpMsg = ( CMessageUseFunctionNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	//CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->targetid() );
	//LK_ASSERT( tpPlayer != NULL, return );

	int tFunctionType = tpMsg->functiontype();
	int tFunctionOrder = tpMsg->functionorder();

	if ( tFunctionType < 0 || tFunctionType > OFFICIAL_FUNCTION_KING || tFunctionOrder < 0 || tFunctionOrder > MAX_POWER )
	{
		return;
	}
	int tPowerID = 0;
	if ( tFunctionType == OFFICIAL_FUNCTION_KING )
	{		
		tPowerID = CDataStatic::spKing[ tFunctionOrder ];
	}
	else
	{
		tPowerID = CDataStatic::spOffical[ tFunctionType ][ tFunctionOrder ]; 
	}
	CTemplatePower *tpPower = (CTemplatePower*)CDataStatic::SearchTpl( tPowerID );
	if ( tpPower == NULL )
		return;

	ComplexPos tPos;
	tPos.mLineID = tpMsg->teleport().lineid();
	tPos.mMapID = tpMsg->teleport().mapid();
	tPos.mPos.mX = tpMsg->teleport().posx();
	tPos.mPos.mY = tpMsg->teleport().posy();
	CAsynTeleport tTeleport;
	tTeleport.mTokenID = tpMsg->tokenid();
	tTeleport.mCharID = tpMsg->charid();
	tTeleport.mDestination = tPos;

	if ( tpPower->mType == CTemplatePower::POWER_SHUT )
	{
		CChatModule::GetSingleton().SetChannelFalse( tpMsg->targetid(), CHANNEL_ALL, tpPower->mParam1 );
	}
	else if ( tpPower->mType == CTemplatePower::POWER_PRISON )
	{
		CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->targetid() );
		if ( tpPlayer != NULL )
		{
			CPropertyModule::GetSingleton().OnEventCoopEntity( tpPlayer, tpPower->mParam1 );
		}
	}
	else if ( tpPower->mType == CTemplatePower::POWER_ITEM )
	{
		CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
		if ( tpPlayer != NULL )
		{
			CPropertyModule::GetSingleton().PlayerInsertItem( tpPlayer, tpPower->mParam1, 1 );
		}
	}
	else if ( tpPower->mType == CTemplatePower::POWER_TASK )
	{
		// 发布任务 参数1是任务ID 参数2是可接次数
		int nCountryID = tpMsg->countryid(); 
		//CTaskModule::GetSingleton().PublishTask( tpPower->mParam1, nCountryID, tpPower->mParam2 );
	}
	else if ( tpPower->mType == CTemplatePower::POWER_BUFF )
	{
		CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
		if ( tpPlayer != NULL )
		{
			CPropertyModule::GetSingleton().InsertBuff( tpPlayer, tpPower->mParam1 );
		}
	}
	else if ( tpPower->mType == CTemplatePower::POWER_SUMMON )
	{
		tTeleport.mDstChar.mCharID = tpMsg->targetid();
		CMapModule::GetSingleton().AddAsynTeleport( tTeleport );
		CPropertyModule::GetSingleton().SendSingleTeleportNotify( tpMsg->targetid(), tPos, (char*)tpMsg->srcname().c_str(), tpMsg->tokenid() );
	}
	else if ( tpPower->mType == CTemplatePower::POWER_FREE )
	{
		CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->targetid() );
		if ( tpPlayer != NULL )
		{
			CPropertyModule::GetSingleton().OnEventUnCoopEntity( tpPlayer );
		}		
	}
	else if ( tpPower->mType == CTemplatePower::POWER_STONE )
	{
		CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
		if ( tpPlayer != NULL )
		{
			CTplRedStone *tpRedstone = ( CTplRedStone * )CDataStatic::SearchTpl( tpPower->mParam1 );
			LK_ASSERT( tpRedstone != NULL, return );
			if ( tpRedstone->mRedStoneType == CTplRedStone::TYPE_CAMP )
			{
				CPropertyModule::GetSingleton().CreateCountryRedstone( tpPlayer->GetLineID(), tpPlayer->GetMapID(), tpPlayer->GetMapIndex(), 
					tpPlayer->GetPosX(), tpPlayer->GetPosY(), tpPlayer->GetNationality(), tpPower->mParam1 );
				CPropertyModule::GetSingleton().SendNotifyCountryRedstoneStarted( tpPlayer, CTplRedStone::TYPE_CAMP );
				
			}
			else if ( tpRedstone->mRedStoneType == CTplRedStone::TYPE_COUNTRY )
			{
				CPropertyModule::GetSingleton().CreateCountryRedstone( tpPlayer->GetLineID(), tpPlayer->GetMapID(), tpPlayer->GetMapIndex(), 
					tpPlayer->GetPosX(), tpPlayer->GetPosY(), tpPlayer->GetCountryID(), tpPower->mParam1 );	
				CPropertyModule::GetSingleton().SendNotifyCountryRedstoneStarted( tpPlayer, CTplRedStone::TYPE_COUNTRY );
			}						
		}
	}
	else if ( tpPower->mType == CTemplatePower::POWER_SUMMONWORLD )
	{
		tTeleport.mDstChar.mNationID = tpMsg->worldid();
		CMapModule::GetSingleton().AddAsynTeleport( tTeleport );
		CPropertyModule::GetSingleton().SendMultiTeleportNotify( tTeleport, (char*)tpMsg->srcname().c_str() );
	}
	else if ( tpPower->mType == CTemplatePower::POWER_SUMMONCOUNTRY )
	{
		tTeleport.mDstChar.mNationID = tpMsg->worldid();
		tTeleport.mDstChar.mCountryID = tpMsg->countryid();
		CMapModule::GetSingleton().AddAsynTeleport( tTeleport );
		CPropertyModule::GetSingleton().SendMultiTeleportNotify( tTeleport, (char*)tpMsg->srcname().c_str() );
	}
	else if ( tpPower->mType == CTemplatePower::POWER_SUMMONCORPS )
	{
		if ( tpMsg->corpsid() != 0 )
		{
			tTeleport.mDstChar.mCorpsID = tpMsg->corpsid();
			CMapModule::GetSingleton().AddAsynTeleport( tTeleport );
			CPropertyModule::GetSingleton().SendMultiTeleportNotify( tTeleport, (char*)tpMsg->srcname().c_str() );
		}		
	}
	else if ( tpPower->mType == CTemplatePower::POWER_SUMMONFAMILY )
	{
		if ( tpMsg->familyid() != 0 )
		{
			tTeleport.mDstChar.mFamilyID = tpMsg->familyid();
			CMapModule::GetSingleton().AddAsynTeleport( tTeleport );
			CPropertyModule::GetSingleton().SendMultiTeleportNotify( tTeleport, (char*)tpMsg->srcname().c_str() );
		}		
	}
	
	int tIntParam[ 3 ] = { 0 };
	tIntParam[ 0 ] = tFunctionType;
	tIntParam[ 1 ] = tFunctionOrder;
	tIntParam[ 2 ] = tpMsg->countryid();
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->targetid() );
	if ( tpPlayer != NULL )
	{
		CPropertyModule::GetSingleton().SendEventNotify( NOTICE_DOPLAYER, tpMsg->worldid(), tIntParam, ARRAY_CNT(tIntParam), (char *)tpMsg->srcname().c_str(), (char *)tpPlayer->GetName() );
	}	
}

void CWorldModule::OnMessageGSSetOfficialNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageSetOfficialNotify *tpMsg = ( CMessageSetOfficialNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->targetid() );
	LK_ASSERT( tpPlayer != NULL, return );

	int tOfficialLevel = tpMsg->officiallevel();
	int tOfficialOrder = tpMsg->officialorder();

	if ( tOfficialLevel < 0 || tOfficialLevel > OFFICIAL_LEVEL_NUM || tOfficialOrder < 0 || tOfficialOrder > OFFICAL_NUM )
	{
		return;
	}
	CTemplateOfficialTable *tpTable = (CTemplateOfficialTable *)CDataStatic::GetTemp( TEMP_OFFICALLIST );
	if ( tpTable == NULL )
	{
		return;
	}
	
	int tOfficialID = tpTable->mOfficial[ tOfficialLevel ][ tOfficialOrder ];
	
	CTemplateOfficial *tpOfficial = (CTemplateOfficial*)CDataStatic::SearchTpl( tOfficialID );
	if ( tpOfficial == NULL )
		return;
	bool tDel = false;
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty( );
	for ( int i = 0; i < WIZARD_NUM; i++ )
	{
		int tPoint = tpProperty->mWizard.GetWizardPoint( i ) + tpProperty->mWizard.GetWizardIncrease( i )
			+ tpProperty->GetEquipWizard( i );
		if ( tpOfficial->mWizard[ i ] > tPoint )
		{
			tDel = true;
			break;
		}
	}
	if ( tDel == true )
	{
		SendChangePowerRequest( tpPlayer->GetLineID( ), tpPlayer->GetCountryID( ), CHANGE_DEL, tpOfficial->mPower );
	}	
}

void CWorldModule::SendChangeWizardRequest( int vLineID, int vCountryID, int vChangeType, CEntityPlayer *pPlayer )
{
	if ( pPlayer == NULL )
	{
		return;
	}
	CMessage tMsgHead;
	CMessageChangeWizardRequest tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_REQUEST_CHANGEWIZARD );
	tMsgHead.set_msgpara( (int)&tMsg );

	tMsg.set_lineid( vLineID );
	tMsg.set_countryid( vCountryID );
	tMsg.set_changetype( vChangeType );

	CPropertyPlayer * tpProperty = (CPropertyPlayer*)pPlayer->GetProperty( );
	if ( vChangeType == CHANGE_ADD )
	{
		for ( int i = 0; i < WIZARD_NUM; i++ )
		{
			int tWizard = tpProperty->mWizard.GetWizardPoint( i ) + tpProperty->mWizard.GetWizardIncrease( i ) + tpProperty->GetEquipWizard( i );
			tMsg.add_wizardvalue( tWizard - tpProperty->mWizard.GetWizardSubmit( i ) );
			tpProperty->mWizard.SetWizardSubmit( i, tWizard );
		}
	}
	if ( vChangeType == CHANGE_DEL )
	{
		for ( int i = 0; i < WIZARD_NUM; i++ )
		{
			tMsg.add_wizardvalue( tpProperty->mWizard.GetWizardSubmit( i ) );
			tpProperty->mWizard.SetWizardSubmit( i, 0 );
		}		
	}

	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}

void CWorldModule::SendChangeConstructRequest( int vLineID, int vCountryID, int *vChangeType, int *vChangeValue, int vLen )
{
	CMessage tMsgHead;
	CMessageChangeConstructRequest tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_REQUEST_CHANGECONSTRUCT );
	tMsgHead.set_msgpara( (int)&tMsg );

	tMsg.set_lineid( vLineID );
	tMsg.set_countryid( vCountryID );
	for ( int i = 0; i < vLen; i++ )
	{
		tMsg.add_constructtype( vChangeType[ i ] );
		tMsg.add_constructvalue( vChangeValue[ i ] );
	}

	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}

void CWorldModule::SendChangePowerRequest( int vLineID, int vCountryID, int vChangeType, int vPower )
{
	CMessage tMsgHead;
	CMessageChangePowerRequest tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_REQUEST_CHANGEPOWER );
	tMsgHead.set_msgpara( (int)&tMsg );

	tMsg.set_lineid( vLineID );
	tMsg.set_countryid( vCountryID );
	tMsg.set_changetype( vChangeType );
	tMsg.set_powervalue( vPower );

	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}

void CWorldModule::SendChangeFriendNotify( int vChangeType, int vSrcWorld, int vDesWorld, int vChangeValue )
{
	CMessage tMsgHead;
	CMessageChangeFriendNotify tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_NOTIFY_CHANGEFRIEND );
	tMsgHead.set_msgpara( (int)&tMsg );

	tMsg.set_changetype( vChangeType );
	tMsg.set_srcworld( vSrcWorld );
	tMsg.set_desworld( vDesWorld );
	tMsg.set_changevalue( vChangeValue );

	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}

void CWorldModule::SendKillPersonRequest( int vSrcCharID, int vDesCharID )
{
	CMessage tMsgHead;
	CMessageKillPersonRequest tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_REQUEST_KILLPERSON );
	tMsgHead.set_msgpara( (int)&tMsg );

	tMsg.set_srccharid( vSrcCharID );
	tMsg.set_descharid( vDesCharID );

	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}

void CWorldModule::OnMessageGSKillPersonResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageKillPersonResponse *tpMsg = ( CMessageKillPersonResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->srccharid() );
	LK_ASSERT( tpPlayer != NULL, return );
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
	tpProperty->IncreasePKValue( tpMsg->pkvalue() );
	CPropertyModule::GetSingleton().SendPKValueMessage( tpPlayer, tpProperty->GetPKValue() );
}

void CWorldModule::OnMessageGSWorldQuestResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageWorldQuestResponse *tpMsg = ( CMessageWorldQuestResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSWorldAnswerResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageWorldAnswerResponse *tpMsg = ( CMessageWorldAnswerResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSWorldInfoResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageWorldInfoResponse *tpMsg = ( CMessageWorldInfoResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSWorldQuestNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageWorldQuestNotify *tpMsg = ( CMessageWorldQuestNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSSyncNationNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageSyncNationNotify *tpMsg = ( CMessageSyncNationNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	for ( int i = 0; i < WORLD_NUM; i++ )
	{
		for ( int j = 0; j < WORLD_NUM; j++ )
		{
			if ( i * WORLD_NUM + j < tpMsg->nationstatus_size() )
			{
				mNationStatus[ i ][ j ] = tpMsg->nationstatus( i * WORLD_NUM + j );
			}			
		}
	}	
}

bool CWorldModule::HasCountryService( CEntityPlayer *pPlayer, int vNpcEntityID, int vCountryID )
{
	LK_ASSERT( pPlayer != NULL, return false );

	// 家族npc的判断
	CEntityNpc *tpEntity =  dynamic_cast< CEntityNpc * >( CSceneObjManager::GetSingletonPtr()->GetObject( vNpcEntityID ) );
	LK_ASSERT( tpEntity != NULL, return false );

	// 服务判断
	CPropertyNPC * tpNpcProperty = ( CPropertyNPC * )tpEntity->GetProperty(); 
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpNpcProperty->GetPropertyTempID() );
	LK_ASSERT( tpTmpNpc != NULL, return false );
	if ( tpTmpNpc->mCountryService == 0 )
	{
		return false;
	}
	if ( tpTmpNpc->mCountryType != vCountryID )
	{
		return false;
	}

	// 距离判断
	return pPlayer->IsWithinDistance( tpEntity, MAX_NPC_TALK_GIRD );
}

bool CWorldModule::HasFriendService( CEntityPlayer *pPlayer, int vNpcEntityID )
{
	LK_ASSERT( pPlayer != NULL, return false );

	// 家族npc的判断
	CEntityNpc *tpEntity =  dynamic_cast< CEntityNpc * >( CSceneObjManager::GetSingletonPtr()->GetObject( vNpcEntityID ) );
	LK_ASSERT( tpEntity != NULL, return false );

	// 服务判断
	CPropertyNPC * tpNpcProperty = ( CPropertyNPC * )tpEntity->GetProperty(); 
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpNpcProperty->GetPropertyTempID() );
	LK_ASSERT( tpTmpNpc != NULL, return false );
	if ( tpTmpNpc->mFriendService == 0 )
	{
		return false;
	}

	// 距离判断
	return pPlayer->IsWithinDistance( tpEntity, MAX_NPC_TALK_GIRD );
}

void CWorldModule::OnMessageGSJoinCountryNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageJoinCountryNotify *tpMsg = ( CMessageJoinCountryNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	tpPlayer->SetCountryID( tpMsg->countryid() );

	//CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
	CMapModule::GetSingleton().MessageBroadCast( tpPlayer, pMessage, false, true );
}

void CWorldModule::OnMessageGSPlayerInfoNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessagePlayerGSInfoNotify *tpMsg = ( CMessagePlayerGSInfoNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->srccharid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::SendPlayInfoM2SRequest( CEntityPlayer *pPlayer, std::vector<CEntityPlayer*>* pPlayerSet )
{
	LK_ASSERT( pPlayer != NULL, return );

	static CMessage tMessage;
	static CMessagePlayerGSInfoRequest tRequest;

	tMessage.Clear();
	tRequest.Clear();

	tMessage.mutable_msghead()->set_messageid( ID_C2S_REQUEST_PLAYERGSINFO );
	tMessage.set_msgpara( (unsigned int)&tRequest );

	tRequest.add_srccharid( pPlayer->GetCharID() );

	if ( pPlayerSet != NULL )
	{
		for( std::vector<CEntityPlayer*>::iterator it = pPlayerSet->begin(); it != pPlayerSet->end(); ++it )
		{
			CEntityPlayer* tpPlayer = (*it);
			if ( tpPlayer != NULL )
			{
				tRequest.add_infocharid( tpPlayer->GetCharID() );
			}
		}
	}
	else
	{
		tRequest.add_infocharid( pPlayer->GetCharID() );
	}
	
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
}

void CWorldModule::SendPlayInfoS2MRequest( std::vector<CEntityPlayer*>* pPlayerSet, CEntityPlayer *pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return );

	static CMessage tMessage;
	static CMessagePlayerGSInfoRequest tRequest;

	tMessage.Clear();
	tRequest.Clear();

	tMessage.mutable_msghead()->set_messageid( ID_C2S_REQUEST_PLAYERGSINFO );
	tMessage.set_msgpara( (unsigned int)&tRequest );	

	if ( pPlayerSet != NULL )
	{
		for( std::vector<CEntityPlayer*>::iterator it = pPlayerSet->begin(); it != pPlayerSet->end(); ++it )
		{
			CEntityPlayer* tpPlayer = (*it);
			if ( tpPlayer != NULL )
			{
				tRequest.add_srccharid( tpPlayer->GetCharID() );
			}
		}
	}
	tRequest.add_infocharid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
}

void CWorldModule::OnMessageGSWorldAnswerNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageWorldAnswerNotify *tpMsg = ( CMessageWorldAnswerNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSKingChangeNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageKingChangeNotify *tpMsg = ( CMessageKingChangeNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	int tWorldID = tpMsg->worldid();
	int tCountryID = tpMsg->countryid();
	if ( tWorldID > WORLD_START && tWorldID < WORLD_NUM
		 && tCountryID > COUNTRY_NONE && tCountryID < WORLD_NUM )
	{
		mKingID[ tWorldID ][ tCountryID ] = tpMsg->kingid();
	}		
}

void CWorldModule::OnMessageGetAward( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageGetAwardRequest *tpMsg = ( CMessageGetAwardRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();

	CTemplateAwardInfo *tpInfo = NULL;
	
	// 如果客户端发来了领取的NPC
	if ( tpMsg->npcentityid( ) != 0 )
	{
		CEntityNpc* tpEntity = dynamic_cast< CEntityNpc* >( CSceneObjManager::GetSingletonPtr( )->GetObject( tpMsg->npcentityid( ) ) );
		if ( tpEntity == NULL )
		{
			return;
		}

		CTemplateOgre *tplOgre = (CTemplateOgre*)CDataStatic::SearchTpl( tpEntity->GetNpcID() );
		if ( tplOgre == NULL )
		{
			return;
		}

		if( !pPlayer->IsWithinDistance( tpEntity, MAX_NPC_TALK_GIRD) ) // 在合法距离之内才判断
		{
			LOG_ERROR("pro", "player %s not near npc %d! func=%s", pPlayer->GetCharNameShow(), tpMsg->npcentityid(), __FUNCTION__ );
			return;
		}

		tpInfo = (CTemplateAwardInfo*)CDataStatic::SearchTpl( tplOgre->mAwardID );
		if ( tpInfo == NULL )
		{
			return;
		}
		if ( (unsigned int)tpInfo->mNpcID != tpEntity->GetNpcID() )
		{
			return;
		}

		// 如果需要占领
		if ( tpInfo->mNoFamily == 0 )
		{
			int tOwnFamilyID = CFamilyModule::GetSingleton().GetFamilyIDByNpcID( tpEntity->GetNpcID() );
			int tOwnCorpsID = CCorpsModule::GetSingleton().GetCorpsIDByNpcID( tpEntity->GetNpcID() );
			if ( tOwnFamilyID != pPlayer->GetFamilyID() && tOwnCorpsID != pPlayer->GetCorpsID() )
			{
				SendGetAwardResponse( pPlayer, ERROR_WORLD_AWARD_NOTOWNER, tpInfo->mTempID );
				return;
			}
		}
	}	
	// 否则是界面上领取的
	else
	{		
		tpInfo = (CTemplateAwardInfo*)CDataStatic::SearchTpl( tpMsg->awardid() );
		if ( tpInfo == NULL )
		{
			return;
		}
		if ( tpInfo->mNpcID != 0 )
		{
			return;
		}
		if ( tpInfo->mCountry == COUNTRY_ALL && pPlayer->GetCountryID() == COUNTRY_NONE )
		{
			SendGetAwardResponse( pPlayer, ERROR_WORLD_AWARD_NOCOUNTRY, tpInfo->mTempID );
			return;
		}
		if ( tpInfo->mCountry != COUNTRY_NONE && tpInfo->mCountry != COUNTRY_ALL && tpInfo->mCountry != pPlayer->GetCountryID() )
		{
			SendGetAwardResponse( pPlayer, ERROR_WORLD_AWARD_NOCOUNTRY, tpInfo->mTempID );
			return;
		}
	}
	
	if ( tpProperty->GetAwardTimes( tpInfo->mTempID ) >= tpInfo->mTimes )
	{
		SendGetAwardResponse( pPlayer, ERROR_WORLD_AWARD_FULLTIMES, tpInfo->mTempID );
		return;
	}

	int tItemNum = 0;
	for ( unsigned int i = 0; i < ARRAY_CNT(tpInfo->mItemID); i++ )
	{
		if ( tpInfo->mItemID[ i ] != 0 && tpInfo->mItemNum[ i ] != 0 )
		{	
			tItemNum++;
		}
	}

	CItemBoxToolkit tItemBoxTooKit( tpProperty->GetBaggage() );
	if ( tItemBoxTooKit.GetLeftPileNum( -1 ) < tItemNum )
	{
		SendGetAwardResponse( pPlayer, ERROR_WORLD_AWARD_FULLBAGGAGE, tpInfo->mTempID );		
		return;
	}

	for ( unsigned int i = 0; i < ARRAY_CNT(tpInfo->mItemID); i++ )
	{
		if ( tpInfo->mItemID[ i ] != 0 && tpInfo->mItemNum[ i ] != 0 )
		{
			CItemObject* pInsItemPile = CPropertyModule::GetSingleton().PlayerInsertItem( pPlayer, tpInfo->mItemID[ i ], tpInfo->mItemNum[ i ] ); 
			if ( pInsItemPile != NULL)
			{
				// Log
				LogEventGetItemByAward( pPlayer, pInsItemPile->GetGUID(), tpInfo->mItemID[ i ], tpInfo->mItemNum[ i ], tpInfo->mTempID );
			}
			
		}
	}
	if ( tpInfo->mBuffID != 0 )
	{
		CTemplateBuff *tplBuff = (CTemplateBuff*)CDataStatic::SearchTpl( tpInfo->mBuffID );
		pPlayer->EntityInsertBuff( pPlayer, tplBuff );
		int pTime = tplBuff->mLastTime/1000;
		if (pTime >= 60)
		{
			LogEventGetBuffBySystem(pPlayer,tpInfo->mBuffID,pTime);
		}
	}

	tpProperty->AddAwardTimes( tpInfo->mTempID );
	SendGetAwardResponse( pPlayer, SUCCESS, tpInfo->mTempID, tpMsg->type() );		
}

void CWorldModule::SendGetAwardResponse( CEntityPlayer* pPlayer, int vResult, int vAwardID, int vTempID )
{
	if ( pPlayer == NULL )
	{
		return;
	}
	CMessage tMsgHead;
	CMessageGetAwardResponse tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETAWARD );
	tMsgHead.set_msgpara( (int)&tMsg );

	tMsg.set_result( vResult );
	tMsg.set_awardid( vAwardID );
	tMsg.set_type( vTempID );

	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
}

// 处理网关通知的国家类称号更新 [1/28/2010 Macro]
void CWorldModule::OnMessageGSUpdateNationTitleNotify(CMessage *npMessage)
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageUpdateNationTitleNotice* tpNotice = ( CMessageUpdateNationTitleNotice* ) npMessage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpNotice->roleid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	if ( (bool) tpNotice->isadd() )
	{
		CPropertyModule::GetSingleton().OnPlayerAddTitleEvent( tpPlayer, tpNotice->titleid() );
	}
	else
	{
		CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( tpPlayer, tpNotice->titleid() );
	}

	return ;
}

// 处理网关发送的所在军团的国家类称号 [1/29/2010 Macro]
void CWorldModule::OnMessageGSNationTitleList(CMessage *npMessage)
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageGetNationTitleListResponse* tpResponse = ( CMessageGetNationTitleListResponse* ) npMessage->msgpara();

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( tpResponse->roleid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CPropertyPlayer* tpProperty = ( CPropertyPlayer* ) tpPlayer->GetProperty(); 

	int tTitleList[ TITLEMAX_SELF ] = { 0 };
	int tTitleNum = ARRAY_CNT( tTitleList );

	// 先把玩家身上的称号列表取出来
	tpProperty->GetTitleList( tTitleList, tTitleNum );

	// 根据网关发送的最新国家类称号列表更新
	for ( int i = 0; i < tTitleNum; i ++ )
	{
		CTitle* tpTitle = CDataStatic::GetTitle( tTitleList[ i ] );
		if ( tpTitle == NULL )
			continue;

		// 非国家类称号 不处理
		if ( tpTitle->mType != CTplTitleManager::SPECIAL_NATION )
			continue;

		// 国籍类称号 不处理
		if ( IS_NATION_TITLE( tpTitle->mType, tpTitle->mObtainCondition ) )
			continue;
		
		
		// 删除已经不存在的称号(国家类)
		bool bExist = false;
		for ( int j = 0; j < tpResponse->titlelist_size(); j ++ )
		{
			if ( tTitleList[ i ] == (int) tpResponse->titlelist( j ) )
			{
				bExist = true;
				break;
			}
		}
		if ( !bExist )
		{
			CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( tpPlayer, tTitleList[ i ] );
		}		
	}
	
	// 把最新的国家称号列表插入玩家身上
	for ( int i = 0; i < tpResponse->titlelist_size(); i ++ )
	{
		if ( tpProperty->HasTitle( tpResponse->titlelist( i ) ) )
			continue;

		if ( tpProperty->AddTitle( tpResponse->titlelist( i ) ) > 0 )
		{
			CPropertyModule::GetSingleton().SendNotifyClientUpdateTitle( tpPlayer, TITLE_ADD, tpResponse->titlelist( i ) );
		}		
	}

	return ;
}

// 国家类称号处理(国籍更新) [1/29/2010 Macro]
void CWorldModule::OnEventNationTitle(CEntityPlayer *npPlayer, int nCountryID, bool bIsAdd )
{
	if ( npPlayer == NULL || nCountryID == COUNTRY_NONE )
	{
		return ;
	}

	// 将国籍转换为对应的地图ID
	int tMapID = ConvertCountryToMapID( nCountryID );	
	
	if ( tMapID == INVALID_MAP_ID )
	{
		return ;
	}
	
	// 得到对应的国籍称号
	int tTitleID = CDataStatic::GetTitle( tMapID, CTplTitleManager::SCOPE_NATIONALITY, 0 );
	if ( tTitleID == INVALID_OBJ_ID )
	{
		return ;
	}

	if ( bIsAdd )
	{
		CPropertyModule::GetSingleton().OnPlayerAddTitleEvent( npPlayer, tTitleID );
	}
	else
	{
		CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( npPlayer, tTitleID );
	}	

	return ;
}

// 国籍转换为地图ID [1/29/2010 Macro]
int CWorldModule::ConvertCountryToMapID( int nCountryID )
{
	int tMapID = 0;
	switch ( nCountryID )
	{
	case COUNTRY_CHANGAN:
		{
			tMapID = COUNTRY_MAP_CHANGAN;
			break;
		}
	case COUNTRY_WUJI:
		{
			tMapID = COUNTRY_MAP_WUJI;
			break;
		}
	case COUNTRY_CHECHI:
		{
			tMapID = COUNTRY_MAP_CHECHI;
			break;
		}
	case COUNTRY_BAOXIANG:
		{
			tMapID = COUNTRY_MAP_BAOXIANG;
			break;
		}
	case COUNTRY_NVER:
		{
			tMapID = COUNTRY_MAP_NVER;
			break;
		}
	default:
		break;
	}
	return tMapID;
}

// 地图转国籍
int CWorldModule::ConvertMapIDToCountry( int nMapID )
{
	int tCountryID = COUNTRY_NONE;
	switch ( nMapID )
	{
	case COUNTRY_MAP_CHANGAN:
		{
			tCountryID = COUNTRY_CHANGAN;
			break;
		}
	case COUNTRY_MAP_WUJI:
		{
			tCountryID = COUNTRY_WUJI;
			break;
		}
	case COUNTRY_MAP_CHECHI:
		{
			tCountryID = COUNTRY_CHECHI;
			break;
		}
	case COUNTRY_MAP_BAOXIANG:
		{
			tCountryID = COUNTRY_BAOXIANG;
			break;
		}
	//case COUNTRY_MAP_NVER:
	//	{
	//		tCountryID = COUNTRY_NVER;
	//		break;
	//	}
	default:
		{
			tCountryID = COUNTRY_NONE;
			break;
		}
	}
	return tCountryID;
}


void CWorldModule::OnMessageGSRealSummonNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageRealSummonNotify *tpMsg = ( CMessageRealSummonNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	ComplexPos tPos;
	tPos.mLineID = tpMsg->lineid();
	tPos.mMapID = tpMsg->mapid();
	tPos.mPos.mX = tpMsg->posx();
	tPos.mPos.mY = tpMsg->posy();
	CAsynTeleport tTeleport;
	tTeleport.mTokenID = tpMsg->tokenid();
	tTeleport.mCharID = tpMsg->srccharid();
	tTeleport.mDestination = tPos;
	
	if ( tpMsg->descharid() != 0 )
	{
		tTeleport.mDstChar.mCharID = tpMsg->descharid();
		CMapModule::GetSingleton().AddAsynTeleport( tTeleport );
		CPropertyModule::GetSingleton().SendSingleTeleportNotify( tpMsg->descharid(), tPos, (char*)tpMsg->srccharname().c_str(), tpMsg->tokenid() );
	}
	else if ( tpMsg->corpsid() != 0 )
	{
		tTeleport.mDstChar.mCorpsID = tpMsg->corpsid();
		CMapModule::GetSingleton().AddAsynTeleport( tTeleport );
		CPropertyModule::GetSingleton().SendMultiTeleportNotify( tTeleport, (char*)tpMsg->srccharname().c_str() );
	}
	else if ( tpMsg->familyid() != 0 )
	{
		tTeleport.mDstChar.mFamilyID = tpMsg->familyid();
		CMapModule::GetSingleton().AddAsynTeleport( tTeleport );
		CPropertyModule::GetSingleton().SendMultiTeleportNotify( tTeleport, (char*)tpMsg->srccharname().c_str() );
	}
}

void CWorldModule::OnMessageGSTrySummonResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageTrySummonResponse *tpMsg = ( CMessageTrySummonResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	int tIndex = tpMsg->itemindex();
	int tCharID = tpMsg->charid();
	CEntityPlayer *tpPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetPlayerByCharID( tCharID );
	if ( tpPlayer == NULL )
	{
		return;
	}
	CPropertyPlayer* pProperty = (CPropertyPlayer*) tpPlayer->GetProperty();

	CItemObject* tpSrcItem = pProperty->GetBaggage( )->GetItemObjPtr( tIndex );
	if ( tpSrcItem == NULL )
	{
		return;
	}
	CTplItem *tpItem = (CTplItem *)CDataStatic::SearchTpl( tpSrcItem->GetItemID() );
	if ( tpItem == NULL )
	{
		return;	
	}
		
	if ( tpItem->mCountType != 0 )
	{
		int tKey = CPersonalTimesMgr::MakeKey((enTimeMgrType)tpItem->mCountType, 0);
		pProperty->GetTimesMgr()->Tick(tKey);
	}

	if ( tpMsg->result() != SUCCESS )
	{
		CMessage tMessage;
		CMessageUseSummonItemResponse tPara;
		tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_USESUMMONITEM );
		tMessage.set_msgpara( (long) &tPara );

		tPara.set_result( tpMsg->result() );
		CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
		return;
	}	

	if ( CPropertyModule::GetSingleton().PlayerRemoveItem( tpPlayer, tIndex, 1 ) != 0 )
	{
		return;
	}
	CMessageRealSummonRequest tRequest;
	CMessage tMessage;

	tMessage.mutable_msghead()->set_messageid( ID_S2G_REQUEST_REALSUMMON );
	tMessage.set_msgpara( (uint32)&tRequest );

	tRequest.set_charid( tpPlayer->GetCharID() );
	tRequest.set_charname( tpMsg->charname() );
	tRequest.set_summontype( tpMsg->summontype() );
	tRequest.set_lineid( tpPlayer->GetLineID() );
	tRequest.set_mapid( tpPlayer->GetMapID() );
	tRequest.set_posx( tpPlayer->GetPosX() );
	tRequest.set_posy( tpPlayer->GetPosY() );
	CSceneLogic::GetSingletonPtr()->Send2Gate( &tMessage );	
}

void CWorldModule::OnMessageSubmitWar( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageSubmitWarRequest *tpMsg = ( CMessageSubmitWarRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CMessageSubmitWarResponse tResponse;
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_SUBMITWAR );
	tMessage.set_msgpara( (uint32)&tResponse );

	if ( pPlayer->GetLevel() < tpConfig->mSubmitLevel )
	{
		tResponse.set_result( ERROR_WAR_SUBMITLOWLEVEL );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return;
	}
	if ( pPlayer->GetMoney() < tpConfig->mSubmitMoney[ pPlayer->GetLevel() ] )
	{
		tResponse.set_result( ERROR_WAR_SUBMITNOMONEY );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return;
	}	

	tpMsg->set_charid( pPlayer->GetCharID() );
	CPropertyPlayer *tpPropertyPlayer = (CPropertyPlayer*)pPlayer->GetProperty();
	tpMsg->set_warserial( tpPropertyPlayer->GetWarSerial() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageEnterWar( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageEnterWarRequest *tpMsg = ( CMessageEnterWarRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CPropertyPlayer *tpPropertyPlayer = (CPropertyPlayer*)pPlayer->GetProperty();
	if ( tpPropertyPlayer->GetStateParam( STATE_STONE ).mInState == true )
	{
		return;
	}

	tpMsg->set_charid( pPlayer->GetCharID() );
	tpMsg->set_warserial( tpPropertyPlayer->GetWarSerial() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageQueryWarCamp( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageQueryWarCampRequest *tpMsg = ( CMessageQueryWarCampRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageQueryWarNum( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageQueryWarNumRequest *tpMsg = ( CMessageQueryWarNumRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageQueryWarInfo( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageQueryWarInfoRequest *tpMsg = ( CMessageQueryWarInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageQueryWarRank( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageQueryWarRankRequest *tpMsg = ( CMessageQueryWarRankRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageBidCamp( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageBidCampRequest *tpMsg = ( CMessageBidCampRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageCampBidInfo( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageCampBidInfoRequest *tpMsg = ( CMessageCampBidInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageCampResInfo( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageCampResInfoRequest *tpMsg = ( CMessageCampResInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageGetWarAward( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageGetWarAwardRequest *tpMsg = ( CMessageGetWarAwardRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CPropertyPlayer *tpPropertyPlayer = (CPropertyPlayer*)pPlayer->GetProperty();
	tpMsg->set_charid( pPlayer->GetCharID() );
	tpMsg->set_warserial( tpPropertyPlayer->GetWarSerial() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageQueryWarAward( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageQueryWarAwardRequest *tpMsg = ( CMessageQueryWarAwardRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CPropertyPlayer *tpPropertyPlayer = (CPropertyPlayer*)pPlayer->GetProperty();
	tpMsg->set_charid( pPlayer->GetCharID() );
	tpMsg->set_warserial( tpPropertyPlayer->GetWarSerial() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageGSSubmitWarResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageSubmitWarResponse *tpMsg = ( CMessageSubmitWarResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;
	CPropertyPlayer *tpPropertyPlayer = (CPropertyPlayer*)tpPlayer->GetProperty();
	if ( tpMsg->result() == SUCCESS )
	{
		if ( tpPlayer->GetMoney() < tpConfig->mSubmitMoney[ tpPlayer->GetLevel() ] )
		{
			tpMsg->set_result( ERROR_WAR_SUBMITNOMONEY );
			CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
			return;
		}
		if (CPropertyModule::GetSingleton().PlayerChangeMoney( tpPlayer, tpConfig->mSubmitMoney[ tpPlayer->GetLevel() ], true, false ) == SUCCESS)
		{
			tpPropertyPlayer->SetWarSerial( tpMsg->warserial() );
			LogEventLostMoneyByWar(tpPlayer, CYuanBao::em_unbind_money, tpConfig->mSubmitMoney[ tpPlayer->GetLevel() ], tpPropertyPlayer->GetWarSerial());
		}				
	}

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSEnterWarResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageEnterWarResponse *tpMsg = ( CMessageEnterWarResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;
	if ( tpMsg->warid() < 0 || tpMsg->warid() >= ARRAY_CNT(tpConfig->mMapID) )
	{
		return;
	}

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );

	if ( tpMsg->result() == SUCCESS )
	{
		CMapModule::GetSingleton().Teleport( BATTLE_LINE_ID, tpConfig->mMapID[ tpMsg->warid() ], tpMsg->posx(), tpMsg->posy(), tpPlayer );
	}
}

void CWorldModule::OnMessageGSQueryWarCampResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageQueryWarCampResponse *tpMsg = ( CMessageQueryWarCampResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSQueryWarNumResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageQueryWarNumResponse *tpMsg = ( CMessageQueryWarNumResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSQueryWarInfoResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageQueryWarInfoResponse *tpMsg = ( CMessageQueryWarInfoResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSQueryWarRankResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageQueryWarRankResponse *tpMsg = ( CMessageQueryWarRankResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSBidCampResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageBidCampResponse *tpMsg = ( CMessageBidCampResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSWarSourceNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageWarSourceNotify *tpMsg = ( CMessageWarSourceNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	if ( tpMsg->charid() != 0 )
	{
		CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
		LK_ASSERT( tpPlayer != NULL, return );

		CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
	}
	else
	{
		CSceneLogic::GetSingleton().SendSceneMessage( pMessage, -1, tpMsg->mapid() );
	}	
}

void CWorldModule::OnMessageGSReliveChangeNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageReliveChangeNotify *tpMsg = ( CMessageReliveChangeNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;
	for ( int i = 0; i < (int)ARRAY_CNT(tpConfig->mMapID); i++ )
	{
		CPropertyModule::RelivePositionMap::iterator tFindIter = CPropertyModule::GetSingleton().mRelivePosition.find( tpConfig->mMapID[ i ] );
		if ( tFindIter == CPropertyModule::GetSingleton().mRelivePosition.end( ) )
		{
			continue;
		}
		CRelivePosition* tpRelive = ( CRelivePosition* ) CSceneObjManager::GetSingletonPtr( )->GetObject( tFindIter->second );
		if ( tpRelive == NULL )
		{
			continue;
		}
		// 先删除地图上现有的东西
		CSceneLogic::GetSingleton().DestroyMapNPC( tpConfig->mMapID[ i ] );
		// 再创建出新的
		CMapObject* tpMap = CMapObjManager::GetSingleton( ).GetMapObject( CSceneLogic::GetSingleton().GetLineID(), tpConfig->mMapID[ i ], 0 );
		tpMap->CreateAllEntity();
		for ( int j = 0; j < tpMsg->worldid_size() && j < tpMsg->campid_size(); j++ )
		{
			int tCampID = tpMsg->campid(j);
			int tWorldID = tpMsg->worldid(j);
			if ( tCampID < 0 || tCampID > (int)ARRAY_CNT(tpConfig->mPosX) || tCampID > (int)ARRAY_CNT(tpConfig->mPosY) )
				continue;
			tpRelive->AddSpecialRelive( tWorldID, BATTLE_LINE_ID, tpConfig->mMapID[ i ], tpConfig->mPosX[ tCampID + 1 ], tpConfig->mPosY[ tCampID + 1 ] );
			for ( int n = 0; n < (int)ARRAY_COL(tpConfig->mFlagIndex); n++ )
			{
				COgreCreator *tpCreator = (COgreCreator*)CCreatorManager::GetSingleton().GetCreator( tpConfig->mFlagIndex[ tCampID ][ n ] );
				if ( tpCreator != NULL )
				{
					CEntityNpc *tpNpc = (CEntityNpc*)CSceneLogic::GetSingletonPtr()->GetEntity( tpCreator->mHasOgre );
					if ( tpNpc != NULL )
					{
						tpNpc->SetOwnCampID( tWorldID );
					}
				}
			}
		}		
	}
}

void CWorldModule::OnMessageGSCampBidInfoResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageCampBidInfoResponse *tpMsg = ( CMessageCampBidInfoResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}


void CWorldModule::OnMessageGSCampResInfoResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageCampResInfoResponse *tpMsg = ( CMessageCampResInfoResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::SendWarControlNotify( int vState, int vDay, int vHour, int vMinute, int vSerial )
{
	CMessage tMsgHead;
	CMessageWarControlNotify tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_NOTIFY_WARCONTROL );
	tMsgHead.set_msgpara( (int)&tMsg );

	tMsg.set_controlstate( vState );
	tMsg.set_gmhour( vHour );
	tMsg.set_gmmin( vMinute );
	tMsg.set_gmday( vDay );
	tMsg.set_warserial( vSerial );

	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}

void CWorldModule::OnMessageGSGetWarAwardResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageGetWarAwardResponse *tpMsg = ( CMessageGetWarAwardResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CTemplateLevelMoneyLmt* tpMoneyTable = (CTemplateLevelMoneyLmt*)CDataStatic::GetTemp(TEMP_MONEYLMT_CFG);
	LK_ASSERT( tpMoneyTable != NULL, return );
	LK_ASSERT( tpPlayer->GetLevel() >= 0 && tpPlayer->GetLevel() < TMP_MAX_GRADE, return );

	if ( tpMsg->result() != SUCCESS )
	{
		CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
		return;
	}

	CPropertyPlayer *tpPropertyPlayer = (CPropertyPlayer*)tpPlayer->GetProperty();
	if ( (tpPropertyPlayer->GetMoney() + tpMsg->money()) > tpMoneyTable->mLmtTable[ tpPlayer->GetLevel() ].mBagMoneyLmt )
	{
		tpMsg->set_result( ERROR_WAR_MONEYFULL );
		CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
		return;
	}

	if ( CPropertyModule::GetSingleton().PlayerChangeMoney( tpPlayer, tpMsg->money(), false, false ) == SUCCESS )
	{
		LogEventGetMoneyByWar(tpPlayer, CYuanBao::em_unbind_money, tpMsg->money( ), tpPropertyPlayer->GetWarSerial( ));
		tpPropertyPlayer->SetWarSerial( 0 );
	}

	int tRank = tpMsg->rank();
	// 按排名插入称号
	if ( tRank >= 0 && tRank < (int)ARRAY_COL(tpConfig->mTitle) 
		&& tpPlayer->GetMetier() >= 0 && tpPlayer->GetMetier() < (int)ARRAY_ROW(tpConfig->mTitle) )
	{
		int tTitleID = tpConfig->mTitle[ tpPlayer->GetMetier() ][ tRank ];
		CPropertyModule::GetSingleton().OnPlayerAddTitleEvent( tpPlayer, tTitleID, tpMsg->validtime() );
		//CPropertyModule::GetSingleton().SendNotifyClientUpdateTitle( tpPlayer, TITLE_ADD, tTitleID );
	}

	// 奖励声望
	CTemplateRepeFameTable *tpTable = ( CTemplateRepeFameTable * )CDataStatic::GetTemp( TEMP_REPEFAMETABLE );
	if ( tpTable != NULL )
	{
		CPropertyModule::GetSingleton().ChangeCombatFame( tpPlayer, tpTable->mFameValue[ CTemplateRepeFameTable::REPETYPE_QITIAN ] );
	}

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSQueryWarAwardResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageQueryWarAwardResponse *tpMsg = ( CMessageQueryWarAwardResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSSyncWarStatusNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageSyncWarStatusNotify *tpMsg = ( CMessageSyncWarStatusNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	if ( tpMsg->warstatus() == WAR_BATTLE )
	{
		mSaveFin = true;
	}
	else
	{
		mSaveFin = false;
	}
	mWarSerial = tpMsg->warserial();
}

void CWorldModule::OnMessageGSCreateWarStoneNotify( CMessage* pMessage )
{
	if ( IS_BATTLE_LINE_ID( CSceneLogic::GetSingleton().GetLineID() ) == false )
		return;

	LK_ASSERT( pMessage != NULL, return );
	CMessageCreateWarStoneNotify *tpMsg = ( CMessageCreateWarStoneNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	int tLevel = tpMsg->stonelevel();
	if ( tLevel < 0 || tLevel >= (int)ARRAY_CNT(tpConfig->mStoneID) )
	{
		return;
	}
	int tMapID = tpMsg->mapid();
	if ( tMapID != 0 )
	{
		CPropertyModule::GetSingleton().CreateCountryRedstone( BATTLE_LINE_ID, tMapID, 0, 
					tpConfig->mStonePosX, tpConfig->mStonePosY, 0, tpConfig->mStoneID[ tLevel ] );
	}
	else
	{
		for ( int i = 0; i < (int)ARRAY_CNT(tpConfig->mMapID); i++ )
		{
			CPropertyModule::GetSingleton().CreateCountryRedstone( BATTLE_LINE_ID, tpConfig->mMapID[ i ], 0, 
					tpConfig->mStonePosX, tpConfig->mStonePosY, 0, tpConfig->mStoneID[ tLevel ] );
		}
	}
}

void CWorldModule::SendWarStoneOverNotify( int vMapID )
{
	CMessage tMsgHead;
	CMessageWarStoneOverNotify tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_NOTIFY_WARSTONEOVER );
	tMsgHead.set_msgpara( (int)&tMsg );

	tMsg.set_mapid( vMapID );

	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}

void CWorldModule::SetStoneInfo( int vMapID, int vStoneID, int vStoneX, int vStoneY, int vEntityID )
{
	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;
	for ( int i = 0; i < MAX_WAR_CAMP; i++ )
	{
		if ( vMapID == tpConfig->mMapID[ i ] )
		{
			if ( vStoneID != 0 )
				mStone[ i ].mStoneID = vStoneID;
			mStone[ i ].mStoneX = vStoneX;
			mStone[ i ].mStoneY = vStoneY;
			mStone[ i ].mStoneEntityID = vEntityID;
		}
	}
}

int CWorldModule::GetStoneID( int vMapID )
{
	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return 0;
	for ( int i = 0; i < MAX_WAR_CAMP; i++ )
	{
		if ( vMapID == tpConfig->mMapID[ i ] )
		{
			return mStone[ i ].mStoneID;
		}
	}
	return 0;
}

int CWorldModule::GetStoneX( int vMapID )
{
	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return 0;
	for ( int i = 0; i < MAX_WAR_CAMP; i++ )
	{
		if ( vMapID == tpConfig->mMapID[ i ] )
		{
			return mStone[ i ].mStoneX;
		}
	}
	return 0;
}

int CWorldModule::GetStoneY( int vMapID )
{
	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return 0;
	for ( int i = 0; i < MAX_WAR_CAMP; i++ )
	{
		if ( vMapID == tpConfig->mMapID[ i ] )
		{
			return mStone[ i ].mStoneY;
		}
	}
	return 0;
}

int CWorldModule::GetStoneEntity( int vMapID )
{
	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return 0;
	for ( int i = 0; i < MAX_WAR_CAMP; i++ )
	{
		if ( vMapID == tpConfig->mMapID[ i ] )
		{
			return mStone[ i ].mStoneEntityID;
		}
	}
	return 0;
}

void CWorldModule::OnMessageModifyRate( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageModifyRateRequest *tpMsg = ( CMessageModifyRateRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	if ( HasTaxService( pPlayer, tpMsg->npcentityid() ) == false )
		return;

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageGetTax( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageGetTaxRequest *tpMsg = ( CMessageGetTaxRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	if ( HasTaxService( pPlayer, tpMsg->npcentityid() ) == false )
		return;

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageGSModifyRateResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageModifyRateResponse *tpMsg = ( CMessageModifyRateResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSGetTaxResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageGetTaxResponse *tpMsg = ( CMessageGetTaxResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSModifyRateNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageModifyRateNotify *tpMsg = ( CMessageModifyRateNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	if ( tpMsg->real() == 1 )
		mRateOrder = tpMsg->rate();
	if ( tpMsg->winworld() != 0 )
		mWinWorld = tpMsg->winworld();
}

void CWorldModule::SendAddTaxNotify( int vTax )
{
	CMessage tMsgHead;
	CMessageAddTaxNotify tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_NOTIFY_ADDTAX );
	tMsgHead.set_msgpara( (int)&tMsg );

	tMsg.set_money( vTax );

	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}

bool CWorldModule::HasTaxService( CEntityPlayer *pPlayer, int vNpcEntityID )
{
	LK_ASSERT( pPlayer != NULL, return false );

	// npc的判断
	CEntityNpc *tpEntity =  dynamic_cast< CEntityNpc * >( CSceneObjManager::GetSingletonPtr()->GetObject( vNpcEntityID ) );
	LK_ASSERT( tpEntity != NULL, return false );

	// 服务判断
	CPropertyNPC * tpNpcProperty = ( CPropertyNPC * )tpEntity->GetProperty(); 
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpNpcProperty->GetPropertyTempID() );
	LK_ASSERT( tpTmpNpc != NULL, return false );
	if ( tpTmpNpc->mTaxService == 0 )
	{
		return false;
	}

	// 距离判断
	return pPlayer->IsWithinDistance( tpEntity, MAX_NPC_TALK_GIRD );
}

void CWorldModule::SendStoneInfoNotify( )
{
	CMessage tMsgHead;
	CMessageQueryStoneNotify tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_QUERYSTONE );
	tMsgHead.set_msgpara( (int)&tMsg );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	for (unsigned int i = 0; i < ARRAY_CNT(tpConfig->mMapID); i++ )
	{
		if ( tpConfig->mMapID[ i ] == 0 )
			break;
		tMsg.set_stoneid( GetStoneID( tpConfig->mMapID[ i ] ) );
		CEntity *tpEntity = ( CEntity * )CSceneLogic::GetSingleton().GetEntity( GetStoneEntity( tpConfig->mMapID[ i ] ) );
		if ( tpEntity != NULL )
		{
			tMsg.set_stonex( tpEntity->GetPosX() );
			tMsg.set_stoney( tpEntity->GetPosY() );
		}
		else
		{
			tMsg.set_stonex( GetStoneX( tpConfig->mMapID[ i ] ) );
			tMsg.set_stoney( GetStoneY( tpConfig->mMapID[ i ] ) );
		}	
		CSceneLogic::GetSingleton().SendSceneMessage( &tMsgHead, -1, tpConfig->mMapID[ i ] );
	}
}

void CWorldModule::OnMessageRefuseSummon( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageRefuseSummonRequest *tpMsg = ( CMessageRefuseSummonRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CAsynTeleport* pAsynTele =  CMapModule::GetSingletonPtr()->GetAsyncTeleport( tpMsg->asynteletokenid() );
	if( pAsynTele == NULL )
		return;

	tpMsg->set_srccharid( pPlayer->GetCharID() );
	tpMsg->set_descharid( pAsynTele->mCharID );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageGSRefuseSummonNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageRefuseSummonNotify *tpMsg = ( CMessageRefuseSummonNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->descharid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageAccuseKing( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessageAccuseKingRequest *tpMsg = ( CMessageAccuseKingRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateAccuseConfig *tpConfig = (CTemplateAccuseConfig*)CDataStatic::GetTemp( TEMP_ACCUSECONFIG );
	if ( tpConfig == NULL )
		return;

	CMessage tMsgHead;
	CMessageAccuseKingResponse tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ACCUSEKING );
	tMsgHead.set_msgpara( (int)&tMsg );

	if ( pPlayer->GetLevel() < tpConfig->mAccuseLevel )
	{
		tMsg.set_responseresult( ERROR_ACCUSE_LOWLEVEL );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}

	if ( tpMsg->accuseresult() != ACCUSE_ABORT && pPlayer->GetMoney() < tpConfig->mAccuseMoney )
	{
		tMsg.set_responseresult( ERROR_ACCUSE_NOMONEY );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}

	CPropertyPlayer *tpPropertyPlayer = (CPropertyPlayer*)pPlayer->GetProperty();	
	if ( tpPropertyPlayer->GetHasVoted() != 0 )
	{
		tMsg.set_responseresult( ERROR_ACCUSE_HASVOTED );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
		return;
	}

	tpMsg->set_worldid( pPlayer->GetNationality() );
	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageGSAccuseKingResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageAccuseKingResponse *tpMsg = ( CMessageAccuseKingResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	int tResult = tpMsg->accuseresult();
	if ( tpMsg->responseresult() == SUCCESS )
	{
		CPropertyPlayer *tpPropertyPlayer = (CPropertyPlayer*)tpPlayer->GetProperty();	
		tpPropertyPlayer->SetHasVoted( 1 );
		if ( tResult != ACCUSE_ABORT )
		{
			CTemplateAccuseConfig *tpConfig = (CTemplateAccuseConfig*)CDataStatic::GetTemp( TEMP_ACCUSECONFIG );
			if ( tpConfig == NULL )
				return;
			CPropertyModule::GetSingleton().PlayerChangeMoney( tpPlayer, tpConfig->mAccuseMoney, true, false );			
		}
	}

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSAccuseKingNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageAccuseKingNotify *tpMsg = ( CMessageAccuseKingNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSPowerInfoResponse( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessagePowerInfoResponse *tpMsg = ( CMessagePowerInfoResponse * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessagePowerInfo( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );

	CMessagePowerInfoRequest *tpMsg = ( CMessagePowerInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	tpMsg->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMessage );
}

void CWorldModule::OnMessageOfficialChange( CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageOfficialChangeNotify *tpMsg = ( CMessageOfficialChangeNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		return;
	}
	CTemplateOfficialTable *tpOfficialTable = (CTemplateOfficialTable*)CDataStatic::GetTemp( TEMP_OFFICALLIST );
	if ( tpOfficialTable != NULL )
	{		
		int tOfficialID = tpOfficialTable->mOfficial[ tpMsg->officiallevel() ][ tpMsg->officialorder() ];
		tpPlayer->SetOfficialID( tOfficialID );
	}	
}

void CWorldModule::SendSpecialEventNotify( int vSrcWorldID, int vDesWorldID, int vEventType, int vCharID )
{
	CMessage tMsgHead;
	CMessageSpecialEventNotify tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2G_NOTIFY_SPECIALEVENT );
	tMsgHead.set_msgpara( (int)&tMsg );

	tMsg.set_srcworldid( vSrcWorldID );
	tMsg.set_desworldid( vDesWorldID );
	tMsg.set_eventtype( vEventType );
	tMsg.set_charid( vCharID );

	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}

void CWorldModule::OnMessageGSAccuseStatusNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageAccuseStatusNotify *tpMsg = ( CMessageAccuseStatusNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->charid() );
	LK_ASSERT( tpPlayer != NULL, return );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
}

void CWorldModule::OnMessageGSMakeGuoTanNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageMakeGuoTanNotify *tpMsg = ( CMessageMakeGuoTanNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	ChangeGT( tpMsg->srcworldid(), tpMsg->desworldid(), tpMsg->targettime() );
}

void CWorldModule::OnMessageGSMakeLuanWuNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageMakeLuanWuNotify *tpMsg = ( CMessageMakeLuanWuNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	ChangeLW( tpMsg->srcworldid(), tpMsg->desworldid(), tpMsg->targettime() );
}

void CWorldModule::OnMessageGSSyncGTAndLWNotify( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageSyncGTAndLWNotify *tpMsg = ( CMessageSyncGTAndLWNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	for ( int i = 0; i < tpMsg->gtinfo_size(); i++ )
	{
		GTAndLWInfo *tpInfo = tpMsg->mutable_gtinfo( i );
		ChangeGT( tpInfo->srcworldid(), tpInfo->desworldid(), tpInfo->targettime() );
	}

	for ( int i = 0; i < tpMsg->lwinfo_size(); i++ )
	{
		GTAndLWInfo *tpInfo = tpMsg->mutable_lwinfo( i );
		ChangeLW( tpInfo->srcworldid(), tpInfo->desworldid(), tpInfo->targettime() );
	}
}

int CWorldModule::ChangeGT( int nSrcWorldID, int nDesWorldID, int nTime )
{
	if ( nTime == 0 )
	{
		for ( GTAndLWList::iterator it = mGTList.begin(); it != mGTList.end(); ++it )
		{
			if ( it->mSourceWorld == nSrcWorldID )
			{
				mGTList.erase(it);
				break;
			}
		}
	}
	else
	{
		GTLWInfo tInfo( nSrcWorldID, nDesWorldID, nTime );
		mGTList.push_back(tInfo);
	}
	return 0; 
}

int CWorldModule::ChangeLW( int nSrcWorldID, int nDesWorldID, int nTime )
{
	if ( nTime == 0 )
	{
		for ( GTAndLWList::iterator it = mLWList.begin(); it != mLWList.end(); ++it )
		{
			if ( it->mSourceWorld == nSrcWorldID )
			{
				mLWList.erase(it);
				break;
			}
		}
	}
	else
	{
		GTLWInfo tInfo( nSrcWorldID, nDesWorldID, nTime );
		mLWList.push_back(tInfo);
	}
	return 0;
}

bool CWorldModule::IsInGT( int nSrcWorldID, int nDesWorldID )
{
	for ( GTAndLWList::iterator it = mGTList.begin(); it != mGTList.end(); ++it )
	{
		if ( it->mSourceWorld == nSrcWorldID && it->mTargetWorld == nDesWorldID && it->mEndTime != 0 )
		{
			return true;
		}
	}
	return false;
}

bool CWorldModule::IsInLW( int nSrcWorldID, int nDesWorldID )
{
	for ( GTAndLWList::iterator it = mLWList.begin(); it != mLWList.end(); ++it )
	{
		if ( it->mSourceWorld == nSrcWorldID && it->mTargetWorld == nDesWorldID && it->mEndTime != 0 )
		{
			return true;
		}
	}
	return false;
}
