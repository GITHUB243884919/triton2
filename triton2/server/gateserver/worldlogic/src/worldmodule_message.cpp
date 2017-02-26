#include "corps.h"
#include "corpsmanager.h" 
#include "corpservice.h"
#include "nameservice.h"
#include "gateobj_manager.h"
#include "gatelogic.h"	
#include "core_service.h"
#include "world.h"
#include "promessage_pb.hxx.pb.h"
#include "family_manager.h"
#include "gateteam.h"
#include "teamtype.h"
#include "servermessage_pb.hxx.pb.h"
#include "gateteammanager.h"
#include "log_tools.h"


// ***********************************************************
//  Function:		OnMessage
//  Description:	客户端消息
//  Output:			
//  Date:	06/12/2009
// **********************************************************
void CWorldModule::OnMessage( int nServerID, CMessage* pMessage )
{

}

void CWorldModule::OnMessageBidCamp( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageBidCampRequest *tpMsg = ( CMessageBidCampRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageBidCampResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_BIDCAMP );
	tMessage.set_msgpara( (long) &tResponse );
	tResponse.set_charid( tpPlayer->CharID() );

	if ( tpPlayer->GetNationalityID() < WORLD_FIRST || tpPlayer->GetNationalityID() >= WORLD_NUM )
	{
		return;
	}

	if ( mWarStatus != WAR_NONE && mWarStatus != WAR_SUBMIT )
	{
		tResponse.set_result( ERROR_WAR_NOTBIDTIME );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}

	CCountry *tpCountry = &(  CWorldManager::GetSingleton().mWorld[ tpPlayer->GetNationalityID() ].mCountry[ COUNTRY_CHANGAN ] );
	if ( tpCountry->GetKingID() != tpPlayer->CharID() )
	{
		tResponse.set_result( ERROR_WAR_NOTKING );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}
	int tOldCamp = CWorldManager::GetSingleton().GetBidCampID( tpPlayer->GetNationalityID() );
	if ( tpMsg->campid() < 0 || tpMsg->campid() > MAX_WAR_CAMP )
	{
		return;
	}
	int tOldMoney = CWorldManager::GetSingleton().GetBidMoney( tpPlayer->GetNationalityID() );
	if ( tOldCamp != -1 && tOldCamp != (int)tpMsg->campid() ) 
	{
		tResponse.set_result( ERROR_WAR_HASMONEY );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}
	if ( tOldMoney >= (int)tpMsg->money() )
	{
		tResponse.set_result( ERROR_WAR_BIDLOW );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}
	CWorldManager::GetSingleton().SetBidMoney( tpPlayer->GetNationalityID(), tpMsg->money() );
	CWorldManager::GetSingleton().SetBidCampID( tpPlayer->GetNationalityID(), tpMsg->campid() );

	tResponse.set_result( SUCCESS );
	tResponse.set_money( tpMsg->money() );
	tResponse.set_campid( tpMsg->campid() );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CWorldModule::OnMessageWarChange( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageWarChangeNotify *tpMsg = ( CMessageWarChangeNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	int tCampID = -1;
	for ( int i = 0; i < (int)ARRAY_CNT(tpConfig->mMapID); i++ )
	{
		if ( (int)tpMsg->mapid() == tpConfig->mMapID[ i ] )
		{
			tCampID = i;
			break;
		}
	}

	if ( tpMsg->seedvalue() != 0 )
	{
		if ( tpMsg->seedtype() == CHANGE_ADD )
		{
			CWorldManager::GetSingleton().AddSeed( tpMsg->worldid(), tCampID, tpMsg->seedvalue() );
		}
		if ( tpMsg->seedtype() == CHANGE_DEL )
		{
			CWorldManager::GetSingleton().DelSeed( tpMsg->worldid(), tCampID, tpMsg->seedvalue() );
		}		
	}

	if ( tpMsg->sourcevalue() != 0 )
	{
		if ( tpMsg->sourcetype() == CHANGE_ADD )
		{
			CWorldManager::GetSingleton().AddSource( tpMsg->worldid(), tCampID, tpMsg->sourcevalue() );
		}
		if ( tpMsg->sourcetype() == CHANGE_DEL )
		{
			CWorldManager::GetSingleton().DelSource( tpMsg->worldid(), tCampID, tpMsg->sourcevalue() );
		}
		SendWarSource( tpMsg->worldid(), tpMsg->mapid(), CWorldManager::GetSingleton().GetSource( tpMsg->worldid(), tCampID ) );
	}
}

void CWorldModule::OnMessageObtainWarHonor( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageObtainWarHonorNotify *tpMsg = ( CMessageObtainWarHonorNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	for ( int i = 0; i < tpMsg->warhonor_size(); i++ )
	{
		WarHonorData *tpData = tpMsg->mutable_warhonor(i);
		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpData->charid() );
		if ( tpPlayer == NULL )
		{
			continue;
		}
		CWorldManager::GetSingleton().AddWinner( tpPlayer->GetNationalityID(), tpPlayer->CharID(), tpData->warhonor(), tpData->warkill(), tpPlayer->MetierID() );
	}	
	if ( tpMsg->iskill() == 1 && tpMsg->warhonor_size() > 0 )
	{
		WarHonorData *tpData = tpMsg->mutable_warhonor(0);
		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpData->charid() );
		if ( tpPlayer == NULL )
		{
			return;
		}
		for ( int i = 0; i < (int)ARRAY_CNT(tpConfig->mMapID); i++ )
		{
			if ( tpConfig->mMapID[ i ] == tpPlayer->MapID() )
			{
				CWorldManager::GetSingleton().AddKill( tpPlayer->GetNationalityID(), i );
			}
		}
	}
}

void CWorldModule::OnMessageCampBidInfo( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageCampBidInfoRequest *tpMsg = ( CMessageCampBidInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageCampBidInfoResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CAMPBIDINFO );
	tMessage.set_msgpara( (long) &tResponse );

	if ( tpPlayer->GetNationalityID() < WORLD_FIRST || tpPlayer->GetNationalityID() >= WORLD_NUM )
	{
		return;
	}
	if ( CWorldManager::GetSingleton().mWorld[ tpPlayer->GetNationalityID() ].mCountry[ COUNTRY_CHANGAN ].GetKingID() != tpPlayer->CharID() )
	{
		return;
	}

	int tMoney = CWorldManager::GetSingleton().GetBidMoney( tpPlayer->GetNationalityID() );
	int tCamp = CWorldManager::GetSingleton().GetBidCampID( tpPlayer->GetNationalityID() );

	tResponse.set_charid( tpPlayer->CharID() );
	tResponse.set_result( SUCCESS );
	tResponse.set_money( tMoney );
	tResponse.set_campid( tCamp );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CWorldModule::OnMessageCampResInfo( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageCampResInfoRequest *tpMsg = ( CMessageCampResInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageCampResInfoResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CAMPRESINFO );
	tMessage.set_msgpara( (long) &tResponse );

	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		int tCampID = CWorldManager::GetSingleton().GetCampID( i );
		int tMoney = CWorldManager::GetSingleton().GetBidMoney( i );
		tResponse.add_worldid( i );
		tResponse.add_campid( tCampID );
		tResponse.add_money( tMoney );
	}

	tResponse.set_charid( tpPlayer->CharID() );
	tResponse.set_result( SUCCESS );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CWorldModule::OnMessageWarControl( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageWarControlNotify *tpMsg = ( CMessageWarControlNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	if ( tpMsg->controlstate() == STATE_DISABLE )
	{
		mGMHour = 0;
		mGMMin = 0;
		mGMDay = -1;
		mNoSubmit = 0;
	}
	else if ( tpMsg->controlstate() == STATE_CONTROL )
	{
		mGMHour = tpMsg->gmhour();
		mGMMin = tpMsg->gmmin();
		mGMDay = tpMsg->gmday();		
	}
	else if ( tpMsg->controlstate() == STATE_SERIAL )
	{
		CWorldManager::GetSingleton().mWarSerial = tpMsg->warserial();
	}
	else if ( tpMsg->controlstate() == STATE_NOSUBMIT )
	{
		mNoSubmit = 1;
	}
	else if ( tpMsg->controlstate() == STATE_FORCEACCUSE )
	{
		int tWorldID = tpMsg->warserial();
		if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM )
		{
			return;
		}
		CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ tWorldID ] );
		tpWorld->mGMAccuse = STATE_FORCEACCUSE;
	}
	else if ( tpMsg->controlstate() == STATE_FORCEEND )
	{
		int tWorldID = tpMsg->warserial();
		if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM )
		{
			return;
		}
		CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ tWorldID ] );
		tpWorld->mGMAccuse = STATE_FORCEEND;
	}
}



void CWorldModule::OnMessageGetWarAward( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageGetWarAwardRequest *tpMsg = ( CMessageGetWarAwardRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageGetWarAwardResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETWARAWARD );
	tMessage.set_msgpara( (long) &tResponse );
	tResponse.set_charid( tpPlayer->CharID() );

	// 不是上次报名的不能领奖
	if ( (int)tpMsg->warserial() + 1 != CWorldManager::GetSingleton().mWarSerial )
	{
		tResponse.set_result( ERROR_WAR_NOAWARD );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}

	// 获胜方才有奖励	
	int tBasicAward = 0;
	int tBestAward = 0;

	CWorldManager::GetSingleton().GetAwardBySingle( tpPlayer->GetNationalityID(), tpPlayer->Level(), tpPlayer->CharID(),
		tpPlayer->MetierID(), tBasicAward, tBestAward );

	int tMoney = 0;
	if ( tpPlayer->GetNationalityID() == CWorldManager::GetSingleton().mWinWorld )
	{
		tMoney = tpConfig->mSubmitMoney[ tpPlayer->Level() ];
	}
	tResponse.set_money( tMoney + tBasicAward + tBestAward );
	tResponse.set_rank( CWorldManager::GetSingleton().GetRank( tpPlayer->GetNationalityID(), tpPlayer->CharID(), tpPlayer->MetierID() ) );

	tResponse.set_result( SUCCESS );
	tResponse.set_validtime( CWorldManager::GetSingleton().mTitleTime );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CWorldModule::OnMessageQueryWarAward( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageQueryWarAwardRequest *tpMsg = ( CMessageQueryWarAwardRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageQueryWarAwardResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYWARAWARD );
	tMessage.set_msgpara( (long) &tResponse );
	tResponse.set_charid( tpPlayer->CharID() );

	if ( (int)tpMsg->warserial() + 1 != CWorldManager::GetSingleton().mWarSerial )
	{
		tResponse.set_result( ERROR_WAR_NOAWARD );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}

	// 获胜方才有奖励
	
	int tBasicAward = 0;
	int tBestAward = 0;

	CWorldManager::GetSingleton().GetAwardBySingle( tpPlayer->GetNationalityID(), tpPlayer->Level(), tpPlayer->CharID(),
		tpPlayer->MetierID(), tBasicAward, tBestAward );

	int tRank = CWorldManager::GetSingleton().GetRank( tpPlayer->GetNationalityID(), tpPlayer->CharID(), tpPlayer->MetierID() );
	int tMoney = 0;
	if ( tpPlayer->GetNationalityID() == CWorldManager::GetSingleton().mWinWorld )
	{
		tMoney = tpConfig->mSubmitMoney[ tpPlayer->Level() ];
	}
	tResponse.set_winmoney( tMoney + tBasicAward );
	tResponse.set_bestmoney( tBestAward );
	tResponse.set_rank( tRank + 1 );
	tResponse.set_result( SUCCESS );
	if ( tpPlayer->GetNationalityID() != CWorldManager::GetSingleton().mWinWorld && tRank == -1 )
	{
		tResponse.set_result( ERROR_WAR_LOSE );
	}

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CWorldModule::OnMessageChangeConstruct( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageChangeConstructRequest *tpMsg = ( CMessageChangeConstructRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	int tWorldID = tpMsg->lineid();
	int tCountryID = tpMsg->countryid();
	if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM || tCountryID < 0 || tCountryID >= COUNTRY_NUM )
	{
		return;
	}
	CCountry *tpCountry = &( CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ] );

	for ( int i = 0; i < tpMsg->constructvalue_size() && i < tpMsg->constructtype_size(); i++ )
	{
		int tType = tpMsg->constructtype(i);
		int tValue = tpMsg->constructvalue(i);
		if ( tType < 0 || tType > OFFICAL_NUM )
		{
			return;
		}
		if ( tpMsg->changetype() == CHANGE_ADD )
		{
			tpCountry->SetConstructValue( tpCountry->GetConstructValue( tType ) + tValue, tType );
		}
		else if ( tpMsg->changetype() == CHANGE_DEL )
		{
			tpCountry->SetConstructValue( tpCountry->GetConstructValue( tType ) - tValue, tType );
		}
	}
}

void CWorldModule::OnMessageChangePower( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageChangePowerRequest *tpMsg = ( CMessageChangePowerRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	int tWorldID = tpMsg->lineid();
	int tCountryID = tpMsg->countryid();
	if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM || tCountryID < 0 || tCountryID >= COUNTRY_NUM )
	{
		return;
	}
	CCountry *tpCountry = &( CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ] );

	if ( tpMsg->changetype() == CHANGE_ADD )
	{
		tpCountry->SetKingPower( tpCountry->GetKingPower( ) + tpMsg->powervalue() );
	}
	else if ( tpMsg->changetype() == CHANGE_DEL )
	{
		tpCountry->SetKingPower( tpCountry->GetKingPower( ) - tpMsg->powervalue() );
	}
}

void CWorldModule::OnMessageChangeFriend( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageChangeFriendNotify *tpMsg = ( CMessageChangeFriendNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	int tSrcWorldID = tpMsg->srcworld();
	int tDesWorldID = tpMsg->desworld();
	if ( tSrcWorldID <= WORLD_START || tSrcWorldID >= WORLD_NUM || tDesWorldID <= WORLD_START || tDesWorldID >= WORLD_NUM )
	{
		return;
	}
	CWorld *tpSrcWorld = &( CWorldManager::GetSingleton().mWorld[ tSrcWorldID ] );
	CWorld *tpDesWorld = &( CWorldManager::GetSingleton().mWorld[ tDesWorldID ] );

	if ( tpMsg->changetype() == CHANGE_ADD )
	{
		tpSrcWorld->mFriendStatus[ tDesWorldID ] += tpMsg->changevalue();
		tpDesWorld->mFriendStatus[ tSrcWorldID ] += tpMsg->changevalue();
	}
	else if ( tpMsg->changetype() == CHANGE_DEL )
	{
		tpSrcWorld->mFriendStatus[ tDesWorldID ] -= tpMsg->changevalue();
		tpDesWorld->mFriendStatus[ tSrcWorldID ] -= tpMsg->changevalue();
	}
}

void CWorldModule::OnMessageKillPerson( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageKillPersonRequest *tpMsg = ( CMessageKillPersonRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpSrcPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->srccharid() );

	if( tpSrcPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->srccharid() );
		return;	
	}

	CGatePlayer *tpDesPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->descharid() );

	if( tpDesPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->descharid() );
		return;	
	}


	int tSrcWorldID = tpSrcPlayer->CountryID();
	int tDesWorldID = tpDesPlayer->CountryID();
	if ( tSrcWorldID <= WORLD_START || tSrcWorldID >= WORLD_NUM || tDesWorldID <= WORLD_START || tDesWorldID >= WORLD_NUM )
	{
		return;
	}
	CWorld *tpSrcWorld = &( CWorldManager::GetSingleton().mWorld[ tSrcWorldID ] );
	CWorld *tpDesWorld = &( CWorldManager::GetSingleton().mWorld[ tDesWorldID ] );
	CCountry *tpSrcCountry = &( tpSrcWorld->mCountry[ COUNTRY_CHANGAN ] );
	CCountry *tpDesCountry = &( tpDesWorld->mCountry[ COUNTRY_CHANGAN ] );

	if ( tpDesPlayer->TempID( ) >= 0 && tpDesPlayer->TempID( ) < COUNTRY_NUM )
	{
		CCountry *tpCountry = &( tpDesWorld->mCountry[ tpDesPlayer->TempID( ) ] );
		int tIntParam[ 4 ] = { 0 };
		tIntParam[ 0 ] = tSrcWorldID;
		tIntParam[ 1 ] = tDesWorldID;
		tIntParam[ 2 ] = tpDesPlayer->TempID( );
		if ( tpDesPlayer->CharID() == tpCountry->GetKingID() )
		{
			for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
			{
				if ( i == tpDesPlayer->GetNationalityID() )
					continue;
				SendEventNotify( NOTICE_KILLKING, i, 0, tIntParam, ARRAY_CNT(tIntParam), (char*)tpSrcPlayer->CharName(), (char*)tpDesPlayer->CharName() );
			}
		}
		int tOfficialID = tpCountry->IsHighOfficial( tpDesPlayer->CharID() );
		if ( tOfficialID != 0 )
		{
			tIntParam[ 3 ] = tOfficialID;
			SendEventNotify( NOTICE_KILLOFFICIAL, tpSrcPlayer->GetNationalityID(), 0, tIntParam, ARRAY_CNT(tIntParam), (char*)tpSrcPlayer->CharName(), (char*)tpDesPlayer->CharName() );
		}
	}	

	CTemplateAddPKTable *tpTable = (CTemplateAddPKTable*)CDataStatic::GetTemp( TEMP_ADDPKTABLE );
	if ( tpTable == NULL )
	{
		return;
	}
	int tPKValue = 0;
	if ( tpSrcWorld->mNationStatus[ tDesWorldID ] == WORLD_STATUS_FRIEND )
	{
		tpSrcWorld->mFriendStatus[ tDesWorldID ] += tpTable->mKFChangeFriend;
		tpDesWorld->mFriendStatus[ tSrcWorldID ] += tpTable->mKFChangeFriend;
		tPKValue = tpTable->mKFChangePK;
		tpSrcCountry->SetKingPower( tpSrcCountry->GetKingPower( ) + tpTable->mKFChangePower );
		tpDesCountry->SetKingPower( tpDesCountry->GetKingPower( ) + tpTable->mDFChangePower );
	}
	else if ( tpSrcWorld->mNationStatus[ tDesWorldID ] == WORLD_STATUS_WAR )
	{
		tpSrcWorld->mFriendStatus[ tDesWorldID ] += tpTable->mKWChangeFriend;
		tpDesWorld->mFriendStatus[ tSrcWorldID ] += tpTable->mKWChangeFriend;
		tPKValue = tpTable->mKWChangePK;
		tpSrcCountry->SetKingPower( tpSrcCountry->GetKingPower( ) + tpTable->mKWChangePower );
		tpDesCountry->SetKingPower( tpDesCountry->GetKingPower( ) + tpTable->mDWChangePower );
	}
	else if ( tpSrcWorld->mNationStatus[ tDesWorldID ] == WORLD_STATUS_NONE )
	{
		int tLow = 0;
		int tHigh = 0;
		for ( int i = 0; i < MAX_PEACE_TYPE; i++ )
		{
			tLow = tpTable->mPFriendMin[ i ];
			tHigh = tpTable->mPFriendMax[ i ];
			if ( tpSrcWorld->mFriendStatus[ tDesWorldID ] >= tLow && tpSrcWorld->mFriendStatus[ tDesWorldID ] <= tHigh )
			{
				tPKValue = tpTable->mKPChangePK[ i ];
			}
		}
		tpSrcWorld->mFriendStatus[ tDesWorldID ] += tpTable->mKPChangeFriend;
		tpDesWorld->mFriendStatus[ tSrcWorldID ] += tpTable->mKPChangeFriend;

		tpSrcCountry->SetKingPower( tpSrcCountry->GetKingPower( ) + tpTable->mKPChangePower );
		tpDesCountry->SetKingPower( tpDesCountry->GetKingPower( ) + tpTable->mDPChangePower );
	}
	SendKillPersonResponse( nServerID, tpMsg->srccharid(), tpMsg->descharid(), tPKValue );
}



void CWorldModule::OnMessageWorldQuest( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageWorldQuestRequest *tpMsg = ( CMessageWorldQuestRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	int tSrcWorldID = tpPlayer->CountryID( );
	int tSrcCountryID = tpPlayer->TempID( );
	if ( tSrcWorldID <= WORLD_START || tSrcWorldID >= WORLD_NUM || tSrcCountryID < 0 || tSrcCountryID >= COUNTRY_NUM )
	{
		SendWorldQuestResponse( tpMsg->charid(), ERROR_WORLD_OP_ERRWORLD, 0, 0 );
		return;
	}
	if ( tSrcCountryID != COUNTRY_CHANGAN )
	{
		SendWorldQuestResponse( tpMsg->charid(), ERROR_WORLD_OP_NOTKING, 0, 0 );
		return;
	}
	int tOpType = tpMsg->optype();
	if ( tOpType != WORLD_OP_INVITE && tOpType != WORLD_OP_PEACE && tOpType != WORLD_OP_WAR && tOpType != WORLD_OP_DISMISS )
	{
		SendWorldQuestResponse( tpMsg->charid(), ERROR_WORLD_OP_ERRWORLD, 0, 0 );
		return;
	}
	CCountry *tpSrcCountry = &(CWorldManager::GetSingleton().mWorld[ tSrcWorldID ].mCountry[ tSrcCountryID ]);
	if ( tpMsg->charid() != (unsigned int)tpSrcCountry->GetKingID( ) )
	{
		SendWorldQuestResponse( tpMsg->charid(), ERROR_WORLD_OP_NOTKING, 0, 0 );
		return;
	}

	int tDesWorldID = tpMsg->worldid();
	int tDesCountryID = COUNTRY_CHANGAN;
	if ( tDesWorldID <= WORLD_START || tDesWorldID >= WORLD_NUM )
	{
		SendWorldQuestResponse( tpMsg->charid(), ERROR_WORLD_OP_ERRWORLD, 0, 0 );
		return;
	}
	if ( tSrcWorldID == tDesWorldID )
	{
		SendWorldQuestResponse( tpMsg->charid(), ERROR_WORLD_OP_SAMEWORLD, 0, 0 );
		return;
	}
	CCountry *tpDesCountry = &(CWorldManager::GetSingleton().mWorld[ tDesWorldID ].mCountry[ tDesCountryID ]);

	CWorld *tpSrcWorld = &( CWorldManager::GetSingleton().mWorld[ tSrcWorldID ] );
	CWorld *tpDesWorld = &( CWorldManager::GetSingleton().mWorld[ tDesWorldID ] );

	/*if ( tpSrcWorld->mOpNumber >= NATIONLIMIT )
	{
	SendWorldQuestResponse( tpMsg->charid(), ERROR_WORLD_OP_NUMLIMIT, 0, 0 );
	return;
	}*/

	if ( tOpType == WORLD_OP_INVITE || tOpType == WORLD_OP_PEACE )
	{
		if ( tOpType == WORLD_OP_INVITE )
		{
			if ( tpSrcWorld->mNationStatus[ tDesWorldID ] != WORLD_STATUS_NONE && tpDesWorld->mNationStatus[ tSrcWorldID ] != WORLD_STATUS_NONE )
			{
				SendWorldQuestResponse( tpMsg->charid(), ERROR_WORLD_OP_WRONGSTATUS, 0, 0 );
				return;
			}
		}
		if ( tOpType == WORLD_OP_PEACE )
		{
			if ( tpSrcWorld->mNationStatus[ tDesWorldID ] != WORLD_STATUS_WAR && tpDesWorld->mNationStatus[ tSrcWorldID ] != WORLD_STATUS_WAR )
			{
				SendWorldAnswerResponse( tpMsg->charid(), ERROR_WORLD_OP_WRONGSTATUS, 0, 0, 0 );
				return;
			}
		}
		AddOperation( tDesWorldID, tSrcWorldID, tOpType );
		CGatePlayer *tpOffPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpDesCountry->GetKingID( ) );
		if ( tpOffPlayer != NULL )
		{
			SendWorldQuestNotify( tpOffPlayer->CharID( ), tSrcWorldID, tOpType );
		}
	}
	else
	{		
		if ( tOpType == WORLD_OP_WAR )
		{
			if ( tpSrcWorld->mNationStatus[ tDesWorldID ] != WORLD_STATUS_NONE && tpDesWorld->mNationStatus[ tSrcWorldID ] != WORLD_STATUS_NONE )
			{
				SendWorldQuestResponse( tpMsg->charid(), ERROR_WORLD_OP_WRONGSTATUS, 0, 0 );
				return;
			}
			tpSrcWorld->mNationStatus[ tDesWorldID ] = WORLD_STATUS_WAR;
			tpDesWorld->mNationStatus[ tSrcWorldID ] = WORLD_STATUS_WAR;
			SendWorldResultNotify( tSrcWorldID, tDesWorldID, tOpType );
			CWorldManager::GetSingleton().SyncNation( 0 );
		}
		if ( tOpType == WORLD_OP_DISMISS )
		{
			if ( tpSrcWorld->mNationStatus[ tDesWorldID ] != WORLD_STATUS_FRIEND && tpDesWorld->mNationStatus[ tSrcWorldID ] != WORLD_STATUS_FRIEND )
			{
				SendWorldQuestResponse( tpMsg->charid(), ERROR_WORLD_OP_WRONGSTATUS, 0, 0 );
				return;
			}
			tpSrcWorld->mNationStatus[ tDesWorldID ] = WORLD_STATUS_NONE;
			tpDesWorld->mNationStatus[ tSrcWorldID ] = WORLD_STATUS_NONE;
			SendWorldResultNotify( tSrcWorldID, tDesWorldID, tOpType );
			CWorldManager::GetSingleton().SyncNation( 0 );
		}
		tpSrcWorld->mOpNumber++;
	}

	SendWorldQuestResponse( tpMsg->charid(), SUCCESS, tDesWorldID, tOpType );
}

void CWorldModule::OnMessageChangeWizard( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageChangeWizardRequest *tpMsg = ( CMessageChangeWizardRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	int tWorldID = tpMsg->lineid();
	int tCountryID = tpMsg->countryid();
	if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM || tCountryID < 0 || tCountryID >= COUNTRY_NUM )
	{
		return;
	}
	CCountry *tpCountry = &( CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ] );

	for ( int i = 0; i < tpMsg->wizardvalue_size(); i++ )
	{
		if ( tpMsg->changetype() == CHANGE_ADD )
		{
			tpCountry->SetWizard( tpCountry->GetWizard( i ) + tpMsg->wizardvalue(i), i );
			tpCountry->ChangeConstructByWizard( i, tpMsg->wizardvalue(i) );
		}
		else if ( tpMsg->changetype() == CHANGE_DEL )
		{
			tpCountry->SetWizard( tpCountry->GetWizard( i ) - tpMsg->wizardvalue(i), i );
			tpCountry->ChangeConstructByWizard( i, -1 * tpMsg->wizardvalue(i) );
		}
	}
}





void CWorldModule::OnMessageWorldAnswer( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageWorldAnswerRequest *tpMsg = ( CMessageWorldAnswerRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	int tSrcWorldID = tpPlayer->CountryID( );
	int tSrcCountryID = tpPlayer->TempID( );
	if ( tSrcWorldID <= WORLD_START || tSrcWorldID >= WORLD_NUM || tSrcCountryID < 0 || tSrcCountryID >= COUNTRY_NUM )
	{
		SendWorldAnswerResponse( tpMsg->charid(), ERROR_WORLD_OP_ERRWORLD, 0, 0, 0 );
		return;
	}
	if ( tSrcCountryID != COUNTRY_CHANGAN )
	{
		SendWorldAnswerResponse( tpMsg->charid(), ERROR_WORLD_OP_NOTKING, 0, 0, 0 );
		return;
	}
	int tOpType = tpMsg->optype();
	if ( tOpType != WORLD_OP_INVITE && tOpType != WORLD_OP_PEACE )
	{
		SendWorldAnswerResponse( tpMsg->charid(), ERROR_WORLD_OP_ERRWORLD, 0, 0, 0 );
		return;
	}
	CCountry *tpSrcCountry = &(CWorldManager::GetSingleton().mWorld[ tSrcWorldID ].mCountry[ tSrcCountryID ]);
	if ( tpMsg->charid() != (unsigned int)tpSrcCountry->GetKingID( ) )
	{
		SendWorldAnswerResponse( tpMsg->charid(), ERROR_WORLD_OP_NOTKING, 0, 0, 0 );
		return;
	}

	int tDesWorldID = tpMsg->worldid();
	if ( tDesWorldID <= WORLD_START || tDesWorldID >= WORLD_NUM )
	{
		SendWorldAnswerResponse( tpMsg->charid(), ERROR_WORLD_OP_ERRWORLD, 0, 0, 0 );
		return;
	}
	if ( tSrcWorldID == tDesWorldID )
	{
		SendWorldAnswerResponse( tpMsg->charid(), ERROR_WORLD_OP_SAMEWORLD, 0, 0, 0 );
		return;
	}
	if ( HasOperation( tSrcWorldID, tDesWorldID, tOpType ) == false )
	{
		SendWorldAnswerResponse( tpMsg->charid(), ERROR_WORLD_OP_REPLYNOSRC, 0, 0, 0 );
		return;
	}

	ClearOperation( tSrcWorldID, tDesWorldID, tOpType );

	CWorld *tpSrcWorld = &( CWorldManager::GetSingleton().mWorld[ tSrcWorldID ] );
	CWorld *tpDesWorld = &( CWorldManager::GetSingleton().mWorld[ tDesWorldID ] );

	if ( tpMsg->answer() == WORLD_RESULT_AGREE )
	{
		if ( tOpType == WORLD_OP_INVITE )
		{
			if ( tpSrcWorld->mNationStatus[ tDesWorldID ] != WORLD_STATUS_NONE && tpDesWorld->mNationStatus[ tSrcWorldID ] != WORLD_STATUS_NONE )
			{
				SendWorldAnswerResponse( tpMsg->charid(), ERROR_WORLD_OP_WRONGSTATUS, 0, 0, 0 );
				return;
			}
			int tSrcNum = 0, tDesNum = 0;
			for ( int i = 0; i < WORLD_NUM; i++ )
			{
				if ( tpSrcWorld->mNationStatus[ i ] == WORLD_STATUS_FRIEND )
					tSrcNum++;
				if ( tpDesWorld->mNationStatus[ i ] == WORLD_STATUS_FRIEND )
					tDesNum++;
			}
			if ( tSrcNum >= 1 || tDesNum >= 1 )
			{
				return;
			}
			tpSrcWorld->mNationStatus[ tDesWorldID ] = WORLD_STATUS_FRIEND;
			tpDesWorld->mNationStatus[ tSrcWorldID ] = WORLD_STATUS_FRIEND;
			SendWorldResultNotify( tSrcWorldID, tDesWorldID, tOpType );
			CWorldManager::GetSingleton().SyncNation( 0 );
		}
		if ( tOpType == WORLD_OP_PEACE )
		{
			if ( tpSrcWorld->mNationStatus[ tDesWorldID ] != WORLD_STATUS_WAR && tpDesWorld->mNationStatus[ tSrcWorldID ] != WORLD_STATUS_WAR )
			{
				SendWorldAnswerResponse( tpMsg->charid(), ERROR_WORLD_OP_WRONGSTATUS, 0, 0, 0 );
				return;
			}
			tpSrcWorld->mNationStatus[ tDesWorldID ] = WORLD_STATUS_NONE;
			tpDesWorld->mNationStatus[ tSrcWorldID ] = WORLD_STATUS_NONE;
			SendWorldResultNotify( tSrcWorldID, tDesWorldID, tOpType );
			CWorldManager::GetSingleton().SyncNation( 0 );
		}
	}
	SendWorldAnswerResponse( tpMsg->charid(), SUCCESS, tDesWorldID, tOpType, tpMsg->answer() );
	// 通知双方结果
	//SendWorldAnswerNotify( tpMsg->charid(), tSrcWorldID, tDesWorldID, tpMsg->optype(), tpMsg->answer() );
	SendWorldAnswerNotify( tpDesWorld->mCountry[ COUNTRY_CHANGAN ].GetKingID(), tSrcWorldID, tDesWorldID, tpMsg->optype(), tpMsg->answer() );
	tpDesWorld->mOpNumber++;	
}

void CWorldModule::OnMessageEnterWar( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageEnterWarRequest *tpMsg = ( CMessageEnterWarRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageEnterWarResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ENTERWAR );
	tMessage.set_msgpara( (long) &tResponse );

	tResponse.set_charid( tpPlayer->CharID() );

	// 如果报名序列不对
	if ( (int)tpMsg->warserial() != CWorldManager::GetSingleton().mWarSerial )
	{
		tResponse.set_result( ERROR_WAR_NOTSUBMIT );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}

	// 如果战场状态不对
	if ( mWarStatus != WAR_BATTLE )
	{
		tResponse.set_result( ERROR_WAR_NOTWARTIME );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}

	if ( tpMsg->warid() < 0 || tpMsg->warid() >= ARRAY_CNT(tpConfig->mMapID) )
	{
		return;
	}

	if ( CWorldManager::GetSingleton().GetPlayerNum( tpPlayer->GetNationalityID(), BATTLE_LINE_ID, tpConfig->mMapID[ tpMsg->warid() ] ) >= MAX_WAR_ROOM_SIZE )
	{
		tResponse.set_result( ERROR_WAR_WARFULL );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}
	tResponse.set_charid( tpPlayer->CharID() );
	tResponse.set_result( SUCCESS );
	tResponse.set_warid( tpMsg->warid() );

	int tCampID = CWorldManager::GetSingleton().GetCampID( tpPlayer->GetNationalityID() );
	if ( tCampID < 0 || tCampID > (int)ARRAY_CNT(tpConfig->mPosX) || tCampID > (int)ARRAY_CNT(tpConfig->mPosY) )
	{
		return;
	}
	// 这里要加1
	tResponse.set_posx( tpConfig->mPosX[ tCampID + 1 ] );
	tResponse.set_posy( tpConfig->mPosY[ tCampID + 1 ] );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CWorldModule::OnMessageQueryWarCamp( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageQueryWarCampRequest *tpMsg = ( CMessageQueryWarCampRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}
	CMessage tMessage;
	CMessageQueryWarCampResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYWARCAMP );
	tMessage.set_msgpara( (long) &tResponse );

	tResponse.set_charid( tpPlayer->CharID() );
	//tResponse.set_result( SUCCESS );
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		int tCampID = CWorldManager::GetSingleton().GetCampID( i );
		int tMoney = CWorldManager::GetSingleton().GetBidMoney( i );
		int tBidCampID = CWorldManager::GetSingleton().GetBidCampID( i );
		tResponse.add_worldid( i );
		tResponse.add_campid( tCampID );
		// 系统分配的不显示金额
		if ( tCampID == tBidCampID && tCampID != -1 )
		{
			tResponse.add_money( tMoney );
		}
		else
		{
			tResponse.add_money( 0 );
		}
	}
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CWorldModule::OnMessageQueryWarNum( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageQueryWarNumRequest *tpMsg = ( CMessageQueryWarNumRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}
	CMessage tMessage;
	CMessageQueryWarNumResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYWARNUM );
	tMessage.set_msgpara( (long) &tResponse );

	tResponse.set_charid( tpPlayer->CharID() );
	for ( int j = 0; j < (int)ARRAY_CNT(tpConfig->mMapID); j++ )
	{
		if ( tpConfig->mMapID[ j ] == 0 )
			break;
		WarNumData *pNumData = tResponse.add_wardata();
		pNumData->set_worldid( tpPlayer->GetNationalityID() );
		pNumData->set_campid( j );
		pNumData->set_currentnum( CWorldManager::GetSingleton().GetPlayerNum( tpPlayer->GetNationalityID(), BATTLE_LINE_ID, tpConfig->mMapID[ j ] ) );
	}	
	tResponse.set_campid( CWorldManager::GetSingleton().GetCampID( tpPlayer->GetNationalityID() ) );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CWorldModule::OnMessageQueryWarInfo( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageQueryWarInfoRequest *tpMsg = ( CMessageQueryWarInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}
	CMessage tMessage;
	CMessageQueryWarInfoResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYWARINFO );
	tMessage.set_msgpara( (long) &tResponse );

	tResponse.set_charid( tpPlayer->CharID() );
	//tResponse.set_result( SUCCESS );
	for ( int i = 0; i < (int)ARRAY_CNT(tpConfig->mMapID); i++ )
	{
		if ( tpConfig->mMapID[ i ] == 0 )
			break;
		for ( int j = WORLD_FIRST; j < WORLD_NUM; j++ )
		{
			WarDetailInfo *pWarInfo = tResponse.add_campinfo();
			pWarInfo->set_campid( i );
			pWarInfo->set_worldid( j );
			pWarInfo->set_currentnum( CWorldManager::GetSingleton().GetPlayerNum( j, BATTLE_LINE_ID, tpConfig->mMapID[ i ] ) );
			pWarInfo->set_flag( CWorldManager::GetSingleton().GetSeedNum( j, i ) );
			pWarInfo->set_resource( CWorldManager::GetSingleton().GetSource( j, i ) );
			pWarInfo->set_kill( CWorldManager::GetSingleton().GetKill( j, i ) );
			//pWarInfo->set_power( CWorldManager::GetSingleton(). );
		}
	}	
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CWorldModule::OnMessageQueryWarRank( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageQueryWarRankRequest *tpMsg = ( CMessageQueryWarRankRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}
	CMessage tMessage;
	CMessageQueryWarRankResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYRANK );
	tMessage.set_msgpara( (long) &tResponse );

	tResponse.set_charid( tpPlayer->CharID() );
	//tResponse.set_result( SUCCESS );

	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ i ] );
		for ( int n = METIER_NONE; n < METIER_NOVICE; n++ )
		{
			for ( int j = 0; j < MAX_WAR_WINNER; j++ )
			{
				WarRankInfo *tpInfo = tResponse.add_rankinfo();
				tpInfo->set_worldid( i );
				CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpWorld->mWinChar[ n ][ j ].mCharID );
				if ( pRoleInfo == NULL )
				{
					continue;
				}
				tpInfo->set_metierid( pRoleInfo->Metier() );
				tpInfo->set_kill( tpWorld->mWinChar[ n ][ j ].mKill );
				tpInfo->set_charname( pRoleInfo->RoleName() );
				tpInfo->set_rank( j + 1 );
				tpInfo->set_honor( tpWorld->mWinChar[ n ][ j ].mHonor );
			}
		}		
	}

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}


// 处理称号列表请求
void CWorldModule::OnMessageGetNationTitleList( int nServerID, CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return ;
	}

	CMessageGetNationTitleListRequest* tpReq = ( CMessageGetNationTitleListRequest* ) npMessage->msgpara();

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpReq->roleid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageGetNationTitleListResponse tPara;
	tMessage.mutable_msghead()->set_messageid( ID_G2S_RESPONSE_NATIONTITLELIST );
	tMessage.set_msgpara( (long) &tPara );

	tPara.set_roleid( tpPlayer->CharID() );

	CCorps* tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpPlayer->CharID() );
	if ( tpCorps == NULL )
	{
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return ;
	}

	int tMapList[ MAX_CORPS_NPCOWNER ] = { 0 };
	int tMapNum = ARRAY_CNT( tMapList );

	tpCorps->GetMapList( tMapList, tMapNum );

	for ( int i = 0; i < tMapNum; i ++ )
	{
		int tAction = 0;
		if( IS_MAIN_CITY( tMapList[ i ] ) )
			tAction = CTplTitleManager::SCOPE_OWNNATION;
		else
			tAction = CTplTitleManager::SCOPE_OWNNORMALMAP;

		int tLevel = 0;
		if ( tpCorps->GetRooterID() == tpPlayer->CharID() )
			tLevel = CTplTitleManager::JUNTUANZHANG;

		int tTitleID = CDataStatic::GetTitle( tMapList[ i ], tAction, tLevel );
		tPara.add_titlelist( tTitleID );
	}

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
	return ;
}

void CWorldModule::OnMessageTrySummon( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageTrySummonRequest *tpMsg = ( CMessageTrySummonRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	CMessageTrySummonResponse tResponse;
	CMessage tMessage;

	tMessage.mutable_msghead()->set_messageid( ID_G2S_RESPONSE_TRYSUMMON );
	tMessage.set_msgpara( (uint32)&tResponse );

	tResponse.set_charid( tpPlayer->CharID() );
	tResponse.set_charname( tpMsg->charname() );
	tResponse.set_itemindex( tpMsg->itemindex() );	
	tResponse.set_summontype( tpMsg->summontype() );

	CGatePlayer *tpDesPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charname().c_str() );

	int tResult = SUCCESS;
	if ( tpDesPlayer == NULL )
	{
		if ( tpMsg->has_charname() )
		{
			tResult = ERROR_SUMMONOFFLINE;
		}
	}
	else if ( tpPlayer->CountryID() != tpDesPlayer->CountryID() )
	{
		tResult = ERROR_SUMMONDIFFCOUNTRY;
	}	
	if ( tpMsg->summontype() == CTplItemSummon::SUMMON_SINGLE )
	{
		if ( tpDesPlayer == tpPlayer )
		{
			tResult = ERROR_CANNUSEITEM;
		}
	}
	else if ( tpMsg->summontype() == CTplItemSummon::SUMMON_FAMILY )
	{
		CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpPlayer->CharID() );
		if ( tpFamily == NULL )
		{
			tResult = ERROR_WORLD_NOTLEADER;
		}
		else if ( tpPlayer->CharID() != tpFamily->GetRooter() )
		{
			tResult = ERROR_WORLD_NOTLEADER;
		}
	}
	else if ( tpMsg->summontype() == CTplItemSummon::SUMMON_CORPS )
	{
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpPlayer->CharID() );
		if ( tpCorps == NULL )
		{
			tResult = ERROR_WORLD_NOTLEADER;
		}
		else if ( tpPlayer->CharID() != tpCorps->GetRooterID() )
		{
			tResult = ERROR_WORLD_NOTLEADER;
		}
	}
	else if ( tpMsg->summontype() == CTplItemSummon::SUMMON_TEAM )
	{
		int tTeamID = CServiceGateTeam::GetSingleton().InTeam( tpPlayer->mCharID );
		CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
		if ( tpTeam == NULL )
		{
			tResult = ERROR_WORLD_NOTLEADER;
		}
		else if ( tpPlayer->CharID() != tpTeam->mHeadID )
		{
			tResult = ERROR_WORLD_NOTLEADER;
		}
	}
	tResponse.set_result( tResult );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CWorldModule::OnMessageRealSummon( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageRealSummonRequest *tpMsg = ( CMessageRealSummonRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	CMessageRealSummonNotify tNotify;
	CMessage tMessage;

	tMessage.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_REALSUMMON );
	tMessage.set_msgpara( (uint32)&tNotify );

	tNotify.set_srccharname( tpPlayer->CharName() );
	tNotify.set_lineid( tpMsg->lineid() );
	tNotify.set_mapid( tpMsg->mapid() );
	tNotify.set_posx( tpMsg->posx() );
	tNotify.set_posy( tpMsg->posy() );
	tNotify.set_srccharid( tpPlayer->CharID() );

	CGatePlayer *tpDesPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charname().c_str() );

	if ( tpDesPlayer != NULL )
	{
		tNotify.set_descharid( tpDesPlayer->CharID() );
		SetTokenID( GetTokenID() + 1 );
		tNotify.set_tokenid( GetTokenID() );
		CGateServer::GetSingleton().Send2Scene( tpDesPlayer, &tMessage );
		return;
	}

	if ( tpMsg->summontype() == CTplItemSummon::SUMMON_FAMILY )
	{
		CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpPlayer->CharID() );
		if ( tpFamily != NULL )
		{
			tNotify.set_familyid( tpFamily->GetID() );
			SetTokenID( GetTokenID() + 1 );
			tNotify.set_tokenid( GetTokenID() );
			CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tMessage );
			return;
		}
	}

	else if ( tpMsg->summontype() == CTplItemSummon::SUMMON_CORPS )
	{
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpPlayer->CharID() );
		if ( tpCorps != NULL )
		{
			tNotify.set_corpsid( tpCorps->GetCorpsID() );
			SetTokenID( GetTokenID() + 1 );
			tNotify.set_tokenid( GetTokenID() );
			CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tMessage );
			return;
		}
	}

	else if ( tpMsg->summontype() == CTplItemSummon::SUMMON_TEAM )
	{
		int tTeamID = CServiceGateTeam::GetSingleton().InTeam( tpPlayer->mCharID );
		CGateTeam* tpTeam = ( CGateTeam* ) CGateObjManager::GetSingleton().GetObject( tTeamID );
		if ( tpTeam != NULL )
		{
			int tMemberList[ TEAM_NUMBER_MAX ] = { 0 };
			int tRtn = 0, tNum = 0;
			tRtn = tpTeam->GetMemberList( tMemberList, ARRAY_CNT( tMemberList ), tNum );
			for ( int i = 0; i < tNum && i < (int) ARRAY_CNT( tMemberList ); i ++ )
			{
				CGatePlayer* tpMemberPlayer = ( CGatePlayer* ) CPlayerManager::GetSingleton().ScenePlayer( tMemberList[ i ] );
				if ( tpMemberPlayer != NULL )
				{					
					if ( tpMemberPlayer->CharID() == tpPlayer->CharID() )
					{
						continue;
					}
					tNotify.set_descharid( tpMemberPlayer->CharID() );
					SetTokenID( GetTokenID() + 1 );
					tNotify.set_tokenid( GetTokenID() );
					CGateServer::GetSingleton().Send2Scene( tpMemberPlayer, &tMessage );
				}			
			}
		}
	}
	return;
}


void CWorldModule::OnMessagePlayerInfo( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessagePlayerGSInfoRequest *tpMsg = ( CMessagePlayerGSInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );		

	CMessage tMsgHead;
	CMessagePlayerGSInfoNotify tResponse;
	for ( int i = 0; i < tpMsg->infocharid_size(); i++ )
	{
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->infocharid(i) );
		if( tpPlayer == NULL )
		{
			continue;
		}
		PlayerGSInfo *tpInfo = tResponse.add_playerinfo();
		tpInfo->set_charid( tpPlayer->CharID() );
		/*CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpPlayer->CharID() );
		if ( tpFamily != NULL )
		{
		tpInfo->set_familyid( tpFamily->GetID() );
		tpInfo->set_familyname( tpFamily->GetName() );
		tpInfo->set_familyhead( tpFamily->GetRooter() );
		tCorpsID = tpFamily->GetCorpsID();
		}
		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tCorpsID );
		if ( tpCorps != NULL )
		{
		tpInfo->set_corpsid( tpCorps->GetCorpsID() );
		tpInfo->set_corpsname( tpCorps->GetCorpsName() );
		tpInfo->set_corpshead( tpCorps->GetRooterID() );
		tUnionID = tpCorps->GetUnionID();
		}
		CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tUnionID );
		if ( tpUnion != NULL )
		{
		tpInfo->set_unionid( tpUnion->GetUnionID() );
		tpInfo->set_unionname( tpUnion->GetUnionName() );
		tpInfo->set_unionhead( tpUnion->GetCoreID() );
		}*/
		tpInfo->set_countryid( tpPlayer->TempID() );
		tpInfo->set_officialid( tpPlayer->OfficialID() );
		if ( tpPlayer->CountryID() >= WORLD_FIRST && tpPlayer->CountryID() < WORLD_NUM
			&& tpPlayer->TempID() > COUNTRY_NONE && tpPlayer->TempID() < COUNTRY_NUM )
		{
			CCountry *tpCountry = &( CWorldManager::GetSingleton().mWorld[ tpPlayer->CountryID() ].mCountry[ tpPlayer->TempID() ] );
			tpInfo->set_kingid( tpCountry->GetKingID() );
		}
	}

	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		for ( int j = i + 1; j < WORLD_NUM; j++ )
		{
			WorldStatusInfo *tpWorldInfo = tResponse.add_worldinfo();
			tpWorldInfo->set_srcworldid( i );
			tpWorldInfo->set_desworldid( j );
			CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ i ] );
			tpWorldInfo->set_status( tpWorld->mNationStatus[ j ] );
		}
	}

	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ i ] );
		if ( tpWorld->mGTWorld != 0 && tpWorld->mGTEndTime != 0 )
		{
			GTAndLWInfo *tpInfo = tResponse.add_gtinfo();
			tpInfo->set_srcworldid( i );
			tpInfo->set_desworldid( tpWorld->mGTWorld );
			tpInfo->set_targettime( tpWorld->mGTEndTime );
		}
		if ( tpWorld->mLWWorld != 0 && tpWorld->mLWEndTime != 0 )
		{
			GTAndLWInfo *tpInfo = tResponse.add_lwinfo();
			tpInfo->set_srcworldid( i );
			tpInfo->set_desworldid( tpWorld->mLWWorld );
			tpInfo->set_targettime( tpWorld->mLWEndTime );
		}
	}

	tMsgHead.set_msgpara( (int)&tResponse );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_PLAYERGSINFO );
	tResponse.set_warstate( CWorldModule::GetSingleton().mWarStatus );
	tResponse.set_winworld( CWorldManager::GetSingleton().mWinWorld );
	tResponse.set_taxrate( CWorldManager::GetSingleton().mTaxRate );

	for ( int i = 0; i < tpMsg->srccharid_size(); i++ )
	{
		CGatePlayer *tpSrcPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->srccharid(i) );

		if( tpSrcPlayer == NULL )
		{
			continue;
		}
		tResponse.set_srccharid( tpSrcPlayer->CharID() );
		CGateServer::GetSingleton().Send2Scene( tpSrcPlayer, &tMsgHead );
	}	
}



void CWorldModule::OnMessageSubmitWar( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageSubmitWarRequest *tpMsg = ( CMessageSubmitWarRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );	

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	CMessage tMessage;
	CMessageSubmitWarResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_SUBMITWAR );
	tMessage.set_msgpara( (long) &tResponse );

	tResponse.set_charid( tpPlayer->CharID() );

	if ( (int)tpMsg->warserial() == CWorldManager::GetSingleton().mWarSerial )
	{
		tResponse.set_result( ERROR_WAR_HASSUBMIT );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}

	if ( mWarStatus == WAR_NONE )
	{
		tResponse.set_result( ERROR_WAR_NOTSUBMITTIME );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}

	if ( mNoSubmit != 0 )
	{
		tResponse.set_result( ERROR_WAR_GMNOSUBMIT );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CWorldManager::GetSingleton().OnSubmit( tpPlayer->GetNationalityID(), tpPlayer->Level(), tpConfig->mSubmitMoney[ tpPlayer->Level() ] );

	tResponse.set_result( SUCCESS );
	tResponse.set_warserial( CWorldManager::GetSingleton().mWarSerial );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}


void CWorldModule::OnMessageUseTimes( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageUseTimeRequest *tpMsg = ( CMessageUseTimeRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	int tWorldID = tpPlayer->CountryID( );
	int tCountryID = tpPlayer->TempID( );
	if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM || tCountryID < 0 || tCountryID >= COUNTRY_NUM )
	{
		return;
	}

	SendUseTimesResponse( tpMsg->charid(), tWorldID, tCountryID );
}

void CWorldModule::OnMessageWarStoneOver( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageWarStoneOverNotify *tpMsg = ( CMessageWarStoneOverNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	SendCreateWarStoneNotify( tpMsg->mapid(), nServerID );
}

void CWorldModule::OnMessageWorldInfo( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageWorldInfoRequest *tpMsg = ( CMessageWorldInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	int tSrcWorldID = tpPlayer->CountryID( );

	SendWorldInfoResponse( tpPlayer->CharID( ), tSrcWorldID );
}


void CWorldModule::OnSessionLoadWorldStatus( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	int tcolnum = tpMsg->colcount();
	int trownum = tpMsg->rowcount();	
	if ( tpMsg->resultcode() == 1 )
	{
		for ( int i = 0; i < trownum; ++i )
		{
			int tWorldID = atoi( tpMsg->fieldvalue( i*tcolnum + 0 ).c_str() );
			if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM )
				continue;
			CWorld *tpWorld = &(CWorldManager::GetSingleton().mWorld[ tWorldID ]);
			for ( int j = WORLD_FIRST; j < WORLD_NUM; j++ )
			{
				tpWorld->mFriendStatus[ j ] = atoi( tpMsg->fieldvalue( i*tcolnum + 0 + j ).c_str() );
			}
			for ( int j = WORLD_FIRST; j < WORLD_NUM; j++ )
			{
				tpWorld->mNationStatus[ j ] = atoi( tpMsg->fieldvalue( i*tcolnum + 4 + j ).c_str() );
			}
			tpWorld->mOpNumber = atoi( tpMsg->fieldvalue( i*tcolnum + 9 ).c_str() );

			tpWorld->m_nHonerInc = atoi( tpMsg->fieldvalue( i*tcolnum + 10 ).c_str() );
			tpWorld->m_nCampLevelScore = atoi( tpMsg->fieldvalue( i*tcolnum + 11 ).c_str() );
			tpWorld->m_nCampHonerScore = atoi( tpMsg->fieldvalue( i*tcolnum + 12 ).c_str() );
			tpWorld->m_nWeekLevel = atoi( tpMsg->fieldvalue( i*tcolnum + 13 ).c_str() );
			tpWorld->mAccuseState = atoi( tpMsg->fieldvalue( i*tcolnum + 14 ).c_str() );
			tpWorld->mAccuseYes = atoi( tpMsg->fieldvalue( i*tcolnum + 15 ).c_str() );
			tpWorld->mAccuseNo = atoi( tpMsg->fieldvalue( i*tcolnum + 16 ).c_str() );
			tpWorld->mAccuseAbort = atoi( tpMsg->fieldvalue( i*tcolnum + 17 ).c_str() );
			tpWorld->mGateTimes = atoi( tpMsg->fieldvalue( i*tcolnum + 18 ).c_str() );
			tpWorld->mFlagTimes = atoi( tpMsg->fieldvalue( i*tcolnum + 19 ).c_str() );
			tpWorld->mGTWorld = atoi( tpMsg->fieldvalue( i*tcolnum + 20 ).c_str() );
			tpWorld->mGTEndTime = atoi( tpMsg->fieldvalue( i*tcolnum + 21 ).c_str() );
			tpWorld->mLWWorld = atoi( tpMsg->fieldvalue( i*tcolnum + 22 ).c_str() );
			tpWorld->mLWEndTime = atoi( tpMsg->fieldvalue( i*tcolnum + 23 ).c_str() );
			LOG_NOTICE( "world", "[%s:%d] load worldstatus, worldid %d", __FUNCTION__, __LINE__, tWorldID );
			
			tpWorld->m_nCampTotalScore = GET_TotalScore(tpWorld->m_nCampLevelScore, tpWorld->m_nCampHonerScore);
		}
		mStatusLaunched++;

		// 根据数据库的值重新计算一次等级（用于合服后的等级更新）
		if (4 == trownum)
		{
			m_WeakProtecter.RefreshWeakInfo();
		}
		else
		{
			printf("invalid data in UMS_WORLDSTATUS");
			LOG_ERROR( "world", "[%s:%d] invalid data in UMS_WORLDSTATUS", __FUNCTION__, __LINE__);
			exit(0);
		}

		LOG_NOTICE( "world", "[%s:%d] load worldstatus, status launched %d", __FUNCTION__, __LINE__,  mStatusLaunched );
	}
}

void CWorldModule::OnSessionLoadWorld( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	int tcolnum = tpMsg->colcount();
	int trownum = tpMsg->rowcount();	
	if ( tpMsg->resultcode() == 1 )
	{
		for ( int i = 0; i < trownum; ++i )
		{
			int tWorldID = atoi( tpMsg->fieldvalue( i*tcolnum + 0 ).c_str() );
			if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM )
				continue;
			int tCountryID = atoi( tpMsg->fieldvalue( i*tcolnum + 1 ).c_str() );
			if ( tCountryID < 0 || tCountryID >= COUNTRY_NUM )
				continue;
			CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ]);
			tpCountry->SetKingPower( atoi( tpMsg->fieldvalue( i*tcolnum + 2 ).c_str() ) );
			tpCountry->SetPeopleNum( atoi( tpMsg->fieldvalue( i*tcolnum + 3 ).c_str() ) );
			for ( int j = 0; j < WIZARD_NUM; j++ )
			{
				tpCountry->SetWizard( atoi( tpMsg->fieldvalue( i*tcolnum + 4 + j ).c_str() ), j );
			}
			for ( int j = 0; j < OFFICAL_NUM; j++ )
			{
				tpCountry->SetConstructValue( atoi( tpMsg->fieldvalue( i*tcolnum + 10 + j ).c_str() ), j );
			}
			for ( int j = 0; j < OFFICAL_NUM; j++ )
			{
				tpCountry->SetConstructLevel( atoi( tpMsg->fieldvalue( i*tcolnum + 16 + j ).c_str() ), j );
			}
			for ( int j = 0; j < OFFICAL_NUM; j++ )
			{
				tpCountry->SetConstructWizard( atoi( tpMsg->fieldvalue( i*tcolnum + 22 + j ).c_str() ), j );
			}

			int tVersion = atoi( tpMsg->fieldvalue( i*tcolnum + 28 ).c_str() );

			if ( tVersion == 0 )
			{
				PBOfficialList pbList; 
				const char* pfieldvalue = tpMsg->fieldvalue(i*tcolnum + 29).c_str(); 
				int fieldlen = tpMsg->fieldvaluelen(i*tcolnum + 29);
				if ( pbList.ParseFromArray( pfieldvalue, fieldlen) == true )
				{
					tpCountry->LoadOfficial( pbList );
				}
				else
				{
					LOG_ERROR( "dafault", "load official:%d  %d error", tWorldID, tCountryID );	
				}
			}	
			else
			{
				LoadOfficial( tWorldID, tCountryID );
			}
			mWorldLaunched++;

			LOG_NOTICE( "world", "[%s:%d] load world %d country %d ok, kingpower %d, peoplenum %d already load %d",
				__FUNCTION__, __LINE__, tWorldID, tCountryID, tpCountry->GetKingPower(), tpCountry->GetPeopleNum(), mWorldLaunched );
		}
	}
	else
	{
		printf("load world data from db failed.\n");
		LOG_ERROR( "world", "[%s:%d] load world data from db failed", __FUNCTION__, __LINE__ );
		exit(0);
	}
}

void CWorldModule::OnSessionLoadWorldWarChar( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	int tcolnum = tpMsg->colcount();
	int trownum = tpMsg->rowcount();	
	if ( tpMsg->resultcode() == 1 )
	{
		for ( int i = 0; i < trownum; ++i )
		{
			int tWorldID = atoi( tpMsg->fieldvalue( i*tcolnum + 0 ).c_str() );
			if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM )
				continue;
			CWorld *tpWorld = &(CWorldManager::GetSingleton().mWorld[ tWorldID ]);
			int tCharID = atoi( tpMsg->fieldvalue( i*tcolnum + 1 ).c_str() );
			int tHonor = atoi( tpMsg->fieldvalue( i*tcolnum + 2 ).c_str() );
			int tKill = atoi( tpMsg->fieldvalue( i*tcolnum + 3 ).c_str() );
			int tRank = atoi( tpMsg->fieldvalue( i*tcolnum + 4 ).c_str() );
			int tMetier = atoi( tpMsg->fieldvalue( i*tcolnum + 5 ).c_str() );

			tpWorld->mWinChar[ tMetier ][ tRank ].mCharID = tCharID;
			tpWorld->mWinChar[ tMetier ][ tRank ].mHonor = tHonor;
			tpWorld->mWinChar[ tMetier ][ tRank ].mKill = tKill;
		}
		mCharLaunched++;

		LOG_NOTICE( "world", "[%s:%d] load worldwar, status launched %d", __FUNCTION__, __LINE__,  mWarLaunched );
	}
}





void CWorldModule::SendKillPersonResponse( int vServerID, int vSrcCharID, int vDesCharID, int vPKValue )
{
	CMessage tMsgHead;
	CMessageKillPersonResponse tMsgResponse;

	tMsgResponse.set_srccharid( vSrcCharID );
	tMsgResponse.set_descharid( vDesCharID );
	tMsgResponse.set_pkvalue( vPKValue );

	tMsgHead.mutable_msghead()->set_messageid( ID_G2S_RESPONSE_KILLPERSON );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
}

void CWorldModule::SendWarSourceToSingle( int vCharID, int vMapID )
{
	CMessage tMessage;
	CMessageWarSourceNotify tNotify;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_WARSOURCE );
	tMessage.set_msgpara( (long) &tNotify );

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );
	if ( tpPlayer == NULL )
	{
		return ;
	}

	int tCampID = -1;
	for ( int i = 0; i < (int)ARRAY_CNT(tpConfig->mMapID); i++ )
	{
		if ( vMapID == tpConfig->mMapID[ i ] )
		{
			tCampID = i;
			break;
		}
	}

	if ( tCampID == -1 )
		return;

	tNotify.set_charid( tpPlayer->CharID() );
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		tNotify.set_worldid( i );
		tNotify.set_mapid( vMapID );
		int tSource = CWorldManager::GetSingleton().GetSource( i, tCampID );
		tNotify.set_resource( tSource );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
	}	
}

void CWorldModule::SendWorldQuestNotify( int vCharID, int vWorldID, int vOpType )
{
	CMessage tMsgHead;
	CMessageWorldQuestNotify tMsg;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		return;	
	}

	tMsg.set_charid( vCharID );
	tMsg.set_worldid( vWorldID );
	tMsg.set_optype( vOpType );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_WORLDQUEST );
	tMsgHead.set_msgpara( (int)&tMsg );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

void CWorldModule::SendWorldQuestResponse( int vCharID, int vResult, int vWorldID, int vOpType )
{
	CMessage tMsgHead;
	CMessageWorldQuestResponse tMsg;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vCharID );
		return;	
	}

	tMsg.set_charid( vCharID );
	tMsg.set_result( vResult );
	tMsg.set_worldid( vWorldID );
	tMsg.set_optype( vOpType );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_WORLDQUEST );
	tMsgHead.set_msgpara( (int)&tMsg );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}





void CWorldModule::SendUseTimesResponse( int vCharID, int vWorldID, int vCountryID )
{
	CMessage tMsgHead;
	CMessageUseTimeResponse tMsgResponse;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vCharID );
		return;	
	}

	CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ vWorldID ].mCountry[ vCountryID ]);	

	for ( int i = 0; i < OFFICIAL_LEVEL_NUM; i++ )
	{
		for ( int j = 0; j < MAX_OFFICAL_NUM; j++ )
		{
			if ( tpCountry->mOfficialData[ i ][ j ].mOfficialID == vCharID )
			{
				for ( int n = 0; n < OFFICIAL_FUNCTION_NUM; n++ )
				{
					for ( int m = 0; m < MAX_POWER; m++ )
					{
						if ( tpCountry->mOfficialData[ i ][ j ].mUsetimes[ n ][ m ] != 0 )
						{
							tMsgResponse.mutable_officialdata()->add_usetimes( tpCountry->mOfficialData[ i ][ j ].mUsetimes[ n ][ m ] );
							tMsgResponse.mutable_officialdata()->add_usex( n );
							tMsgResponse.mutable_officialdata()->add_usey( m );
						}						
					}
				}				
			}
		}
	}

	tMsgResponse.set_charid( vCharID );
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_USETIME );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

void CWorldModule::SendUseFunctionNotify( int vCharID, int vTargetID, int vType, int vOrder, int vWorldID, int vCountryID, char *vSrcName, TeleportData tData )
{
	CMessage tMsgHead;
	CMessageUseFunctionNotify tMsgNotify;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vTargetID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vTargetID );
		return;	
	}

	tMsgNotify.set_charid( vCharID );
	tMsgNotify.set_functiontype( vType );
	tMsgNotify.set_functionorder( vOrder );
	tMsgNotify.set_targetid( vTargetID );
	tMsgNotify.set_worldid( vWorldID );
	tMsgNotify.set_countryid( vCountryID );
	if ( vSrcName != NULL )
	{
		tMsgNotify.set_srcname( vSrcName );
	}
	tMsgNotify.mutable_teleport()->CopyFrom( tData );

	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( vCharID );
	if ( tpCorps != NULL )
	{
		tMsgNotify.set_corpsid( tpCorps->GetCorpsID() );
	}
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( vCharID );
	if ( tpFamily != NULL )
	{
		tMsgNotify.set_familyid( tpFamily->GetID() );
	}	
	SetTokenID( GetTokenID() + 1 );
	tMsgNotify.set_tokenid( GetTokenID() );

	tMsgHead.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_USEFUNCTION );
	tMsgHead.set_msgpara( (int)&tMsgNotify );

	CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tMsgHead );
}


void CWorldModule::SendWorldAnswerResponse( int vCharID, int vResult, int vWorldID, int vOpType, int vAnswer )
{
	CMessage tMsgHead;
	CMessageWorldAnswerResponse tMsg;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vCharID );
		return;	
	}

	tMsg.set_charid( vCharID );
	tMsg.set_result( vResult );
	tMsg.set_worldid( vWorldID );
	tMsg.set_optype( vOpType );
	tMsg.set_answer( vAnswer );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_WORLDANSWER );
	tMsgHead.set_msgpara( (int)&tMsg );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

void CWorldModule::SendWorldResultNotify( int vSrcWorldID, int vDesWorldID, int vOpType )
{
	CMessage tMsgHead;
	CMessageWorldResultNotify tMsg;

	tMsg.set_srcworldid( vSrcWorldID );
	tMsg.set_desworldid( vDesWorldID );
	tMsg.set_optype( vOpType );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_WORLDRESULT );
	tMsgHead.set_msgpara( (int)&tMsg );

	CPlayerManager::GetSingleton().SendWorldMessageFromGate( &tMsgHead );
	//CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tMsgHead );
	//CPlayerManager::GetSingleton().SendCountryMessage( &tMsgHead, vSrcWorldID );
	//CPlayerManager::GetSingleton().SendCountryMessage( &tMsgHead, vDesWorldID );
}



void CWorldModule::SendWorldInfoResponse( int vCharID, int vWorldID )
{
	int tResult = SUCCESS;

	CMessage tMsgHead;
	CMessageWorldInfoResponse tMsg;

	if ( vWorldID <= WORLD_START || vWorldID >= WORLD_NUM )
	{
		tResult = ERROR_WORLD_OP_ERRWORLD;
	}
	else
	{
		CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ vWorldID ] );
		for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
		{
			if ( i != vWorldID )
			{
				CWorldStatusData *PBStatus = tMsg.add_worlddata();
				PBStatus->set_worldid( i );
				PBStatus->set_friends( tpWorld->mFriendStatus[ i ] );
				PBStatus->set_nation( tpWorld->mNationStatus[ i ] );
				CCountry *tpCountry = &( CWorldManager::GetSingleton().mWorld[ i ].mCountry[ COUNTRY_CHANGAN ] );
				CCorps *tpCorps = tpCountry->GetCorps( );
				if ( tpCorps != NULL )
				{
					PBStatus->set_corpsid( tpCorps->GetCorpsID( ) );
					PBStatus->set_corpsname( tpCorps->GetCorpsName( ) );
					PBStatus->set_kingid( tpCorps->GetRooterID( ) );
					PBStatus->set_kingname( tpCorps->GetRooterName( ) );
					CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tpCorps->GetUnionID( ) );
					if ( tpUnion != NULL )
					{
						PBStatus->set_unionid( tpUnion->GetUnionID( ) );
						PBStatus->set_unionname( tpUnion->GetUnionName( ) );
					}
				}
			}
		}
	}
	//tMsg.set_result( tResult );
	tMsg.set_charid( vCharID );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_WORLDINFO );
	tMsgHead.set_msgpara( (int)&tMsg );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer != NULL )
	{
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
	}	
}


void CWorldModule::SendEventNotify( int vEventID, int vWorldID, int vCountryID, int *vIntParam, int vSize, char *vStringParam1, char *vStringParam2, int vCharID )
{
	CMessage tMessage;
	CMessageEventNotify tNotify;

	CMessageHead* tpHead = tMessage.mutable_msghead();
	LK_ASSERT( tpHead != NULL, return );

	tpHead->set_messageid( ID_S2C_NOTIFY_EVENT );

	tNotify.set_eventid( vEventID );
	for( int i = 0; i < vSize; i++ )
	{
		tNotify.add_intparam(vIntParam[ i ]);
	}
	if ( vStringParam1 != NULL )
	{
		tNotify.add_stringparam( vStringParam1 );
	}
	if ( vStringParam2 != NULL )
	{
		tNotify.add_stringparam( vStringParam2 );
	}

	tMessage.set_msgpara( (uint32)&tNotify );

	//CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tMessage );
	if ( vCharID == 0 )
	{
		if ( vWorldID == 0 )
			CPlayerManager::GetSingleton().SendWorldMessageFromGate( &tMessage );
		else
			CPlayerManager::GetSingleton().SendCountryMessageFromGate( &tMessage, vWorldID );
	}
	else
	{
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );
		if( tpPlayer != NULL )
		{
			CGateServer::GetSingleton().Send2Player( tpPlayer, &tMessage );
		}	
	}
}



void CWorldModule::SendOfficialChangeNotify( int vCharID, int vOfficialLevel, int vOfficialOrder, int vWorldID, int vCountryID, int vType )
{
	CMessage tMsgHead;
	CMessageOfficialChangeNotify tMsgNotify;

	CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( vCharID );
	if ( pRoleInfo == NULL )
	{
		return;
	}

	tMsgNotify.set_charid( vCharID );
	tMsgNotify.set_charname( pRoleInfo->RoleName( ) );
	tMsgNotify.set_officiallevel( vOfficialLevel );
	tMsgNotify.set_officialorder( vOfficialOrder );
	tMsgNotify.set_countryid( vCountryID );
	tMsgNotify.set_type( vType );
	tMsgNotify.set_worldid( vWorldID );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_OFFICIALCHANGE );
	tMsgHead.set_msgpara( (int)&tMsgNotify );

	//CGateServer::GetSingleton().Send2Scene( vWorldID, &tMsgHead );
	CPlayerManager::GetSingleton().SendWorldMessageFromGate( &tMsgHead );
	CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tMsgHead );
}

void CWorldModule::SendWorldAnswerNotify( int vCharID, int vSrcWorldID, int vDesWorldID, int vOpType, int vAnswer )
{
	CMessage tMsgHead;
	CMessageWorldAnswerNotify tMsg;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vCharID );
		return;	
	}

	tMsg.set_charid( vCharID );
	tMsg.set_srcworldid( vSrcWorldID );
	tMsg.set_desworldid( vDesWorldID );
	tMsg.set_optype( vOpType );
	tMsg.set_answer( vAnswer );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_WORLDANSWER );
	tMsgHead.set_msgpara( (int)&tMsg );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}



void CWorldModule::SendKingChangeNotify( int vWorldID, int vCountryID, int nServerID )
{
	CMessage tMsgHead;
	CMessageKingChangeNotify tMsgNotify;

	tMsgNotify.set_worldid( vWorldID );
	tMsgNotify.set_countryid( vCountryID );

	if ( vWorldID > WORLD_START && vWorldID < WORLD_NUM &&
		vCountryID > COUNTRY_NONE && vCountryID < COUNTRY_NUM )
	{
		CCountry *tpCountry = &( CWorldManager::GetSingleton().mWorld[ vWorldID ].mCountry[ vCountryID ] );
		tMsgNotify.set_kingid( tpCountry->GetKingID() );
		CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpCountry->GetKingID() );
		if ( pRoleInfo != NULL )
		{
			tMsgNotify.set_kingname( pRoleInfo->RoleName( ) );
		}
	}	

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_KINGCHANGE );
	tMsgHead.set_msgpara( (int)&tMsgNotify );

	if ( nServerID == 0 )
	{
		CPlayerManager::GetSingleton().SendWorldMessageFromGate( &tMsgHead );
		CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tMsgHead );
	}
	else
	{
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
	}
}

void CWorldModule::SendWarNotify( int vEventID, int vInt0, int vInt1, int vInt2, char *vName )
{
	int tIntParam[ 3 ] = { 0 };
	tIntParam[ 0 ] = vInt0;
	tIntParam[ 1 ] = vInt1;
	tIntParam[ 2 ] = vInt2;
	SendEventNotify( vEventID, 0, 0, tIntParam, ARRAY_CNT(tIntParam), vName, NULL );
}


void CWorldModule::SendWarSource( int vWorldID, int vMapID, int vSource )
{
	CMessage tMessage;
	CMessageWarSourceNotify tNotify;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_WARSOURCE );
	tMessage.set_msgpara( (long) &tNotify );

	tNotify.set_worldid( vWorldID );
	tNotify.set_mapid( vMapID );
	tNotify.set_resource( vSource );
	CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tMessage );
}

void CWorldModule::SendReliveChange( int nServerID )
{
	CMessage tMessage;
	CMessageReliveChangeNotify tNotify;
	tMessage.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_RELIVECHANGE );
	tMessage.set_msgpara( (long) &tNotify );

	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		int tCampID = CWorldManager::GetSingleton().GetCampID( i );
		tNotify.add_worldid( i );
		tNotify.add_campid( tCampID );
	}
	if ( nServerID != 0 )
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
	else
		CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tMessage );
}	


void CWorldModule::SendSyncWarStatus( int nServerID )
{
	CMessage tMessage;
	CMessageSyncWarStatusNotify tNotify;
	tMessage.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_SYNCWARSTATUS );
	tMessage.set_msgpara( (long) &tNotify );

	tNotify.set_warstatus( mWarStatus );
	tNotify.set_warserial( CWorldManager::GetSingleton().mWarSerial );
	if ( nServerID != 0 )
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMessage );
	else
		CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tMessage );
}	

void CWorldModule::SendWarCamp( )
{
	CMessage tMessage;
	CMessageWarCampNotify tNotify;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_WARCAMP );
	tMessage.set_msgpara( (long) &tNotify );

	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		int tCampID = CWorldManager::GetSingleton().GetCampID( i );
		int tMoney = CWorldManager::GetSingleton().GetBidMoney( i );
		int tBidCampID = CWorldManager::GetSingleton().GetBidCampID( i );
		tNotify.add_worldid( i );
		tNotify.add_campid( tCampID );
		tNotify.add_bidcampid ( tBidCampID );
		// 系统分配的不显示金额
		if ( tCampID == tBidCampID && tCampID != -1 )
		{
			tNotify.add_money( tMoney );
		}
		else
		{
			tNotify.add_money( 0 );
		}
	}
	CPlayerManager::GetSingleton().SendWorldMessageFromGate( &tMessage );
}

void CWorldModule::SendCreateWarStoneNotify( int vMapID, int vServerID )
{
	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	CMessage tMessage;
	CMessageCreateWarStoneNotify tNotify;
	tMessage.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_CREATEWARSTONE );
	tMessage.set_msgpara( (long) &tNotify );

	int tTotalLevel = 0;
	int tTotalNum = 0;
	int tFixLevel = 0;
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		CWorld *tpWorld = &(CWorldManager::GetSingleton().mWorld[ i ]);
		tTotalLevel += tpWorld->mTotalLevel;
		tTotalNum += tpWorld->mTotalNum;
	}
	if ( tTotalNum != 0 )
	{
		tFixLevel = ( tTotalLevel / tTotalNum - tpConfig->mSubmitLevel ) / 10;
	}
	tNotify.set_stonelevel( tFixLevel );
	tNotify.set_mapid( vMapID );
	if ( vServerID == 0 )
		CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tMessage );
	else
		CGateServer::GetSingleton().Send2Scene( vServerID, &tMessage );
}

// 发送称号更新通知
void CWorldModule::SendNationTitleUpdateNotice( CGatePlayer* npPlayer, int nTitleID, bool bAdd )
{
	if ( npPlayer == NULL || nTitleID == INVALID_OBJ_ID )
	{
		return ;
	}

	CMessage tMessage;
	CMessageUpdateNationTitleNotice tPara;
	tMessage.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_NATIONTITLE );
	tMessage.set_msgpara( (long) &tPara );

	tPara.set_roleid( npPlayer->CharID() );
	tPara.set_titleid( nTitleID );
	tPara.set_isadd( (int) bAdd );

	CGateServer::GetSingleton().Send2Scene( npPlayer, &tMessage );
	return ;
}


void CWorldModule::OnSessionLoadWorldWar( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	int tcolnum = tpMsg->colcount();
	int trownum = tpMsg->rowcount();	
	if ( tpMsg->resultcode() == 1 )
	{
		for ( int i = 0; i < trownum; ++i )
		{
			int tWorldID = atoi( tpMsg->fieldvalue( i*tcolnum + 0 ).c_str() );
			if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM )
				continue;
			CWorld *tpWorld = &(CWorldManager::GetSingleton().mWorld[ tWorldID ]);
			tpWorld->mTotalLevel = atoi( tpMsg->fieldvalue( i*tcolnum + 1 ).c_str() );
			tpWorld->mTotalNum = atoi( tpMsg->fieldvalue( i*tcolnum + 2 ).c_str() );
			tpWorld->mCampID = atoi( tpMsg->fieldvalue( i*tcolnum + 3 ).c_str() );
			tpWorld->mBidMoney = atoi( tpMsg->fieldvalue( i*tcolnum + 4 ).c_str() );
			tpWorld->mBidCampID = atoi( tpMsg->fieldvalue( i*tcolnum + 5 ).c_str() );
			tpWorld->mBasicAward = atoi( tpMsg->fieldvalue( i*tcolnum + 6 ).c_str() );
			tpWorld->mBestAward = atoi( tpMsg->fieldvalue( i*tcolnum + 7 ).c_str() );
			for ( int j = 0; j < MAX_WAR_CAMP; j++ )
			{
				tpWorld->mSource[ j ] = atoi( tpMsg->fieldvalue( i*tcolnum + 8 + j ).c_str() );
			}
			tpWorld->mSubmitMoney = atoi( tpMsg->fieldvalue( i*tcolnum + 11 ).c_str() );

			LOG_NOTICE( "world", "[%s:%d] load worldwar, worldid %d", __FUNCTION__, __LINE__, tWorldID );
			LOG_NOTICE( "world", "worldid %d, totallevel %d, totalnum %d, campid %d, bidmoney %d, bidcampid %d, basicaward %d, bestaward %d", 
				tWorldID, tpWorld->mTotalLevel, tpWorld->mTotalNum, tpWorld->mCampID, tpWorld->mBidMoney, tpWorld->mBidCampID, tpWorld->mBasicAward, tpWorld->mBestAward );
			LOG_NOTICE( "world", "worldid %d, source0 %d, source1 %d, source2 %d, submitmoney %d", 
				tWorldID, tpWorld->mSource[ 0 ], tpWorld->mSource[ 1 ], tpWorld->mSource[ 2 ], tpWorld->mSubmitMoney );
		}
		mWarLaunched++;

		LOG_NOTICE( "world", "[%s:%d] load worldwar, status launched %d", __FUNCTION__, __LINE__,  mWarLaunched );
	}
}

void CWorldModule::OnSessionLoadWorldGlobal( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	int tcolnum = tpMsg->colcount();
	int trownum = tpMsg->rowcount();	
	if ( tpMsg->resultcode() == 1 )
	{
		for ( int i = 0; i < trownum; ++i )
		{
			int tWarSerial = atoi( tpMsg->fieldvalue( i*tcolnum + 0 ).c_str() );
			int tWarStatus = atoi( tpMsg->fieldvalue( i*tcolnum + 1 ).c_str() );
			int tWarTitleTime = atoi( tpMsg->fieldvalue( i*tcolnum + 2 ).c_str() );
			int tWarWinWorld = atoi( tpMsg->fieldvalue( i*tcolnum + 3 ).c_str() );

			int tSetRate = atoi( tpMsg->fieldvalue( i*tcolnum + 4 ).c_str() );
			int tTaxRate = atoi( tpMsg->fieldvalue( i*tcolnum + 5 ).c_str() );
			int tTaxMoney = atoi( tpMsg->fieldvalue( i*tcolnum + 6 ).c_str() );
			int tCorpsMoney = atoi( tpMsg->fieldvalue( i*tcolnum + 7 ).c_str() );
			int tSetTimes = atoi( tpMsg->fieldvalue( i*tcolnum + 8 ).c_str() );
			int tNoSubmit = atoi( tpMsg->fieldvalue( i*tcolnum + 9 ).c_str() );
			LOG_NOTICE( "world", "[%s:%d] load worldglobal, warserial %d, warstatus %d, titletime %d, winworld %d, \
								 setrate %d, taxrate %d, taxmoney %d, corpsmoney %d, settimes %d, nosubmit %d",
				__FUNCTION__, __LINE__, tWarSerial, tWarStatus, tWarTitleTime, tWarWinWorld, tSetRate, tTaxRate, tTaxMoney, tCorpsMoney, tSetTimes, tNoSubmit );
			// 状态还是反查了吧，修正报名问题
			mWarStatus = tWarStatus;
			CWorldManager::GetSingleton().mWarSerial = tWarSerial;
			CWorldManager::GetSingleton().mTitleTime = tWarTitleTime;
			CWorldManager::GetSingleton().mWinWorld = tWarWinWorld;

			CWorldManager::GetSingleton().mSetRate = tSetRate;
			CWorldManager::GetSingleton().mTaxRate = tTaxRate;
			CWorldManager::GetSingleton().mTaxMoney = tTaxMoney;
			CWorldManager::GetSingleton().mCorpsMoney = tCorpsMoney;
			CWorldManager::GetSingleton().mSetTimes = tSetTimes;
			mNoSubmit = tNoSubmit;
		}
		mGlobalLaunched++;

		LOG_NOTICE( "world", "[%s:%d] load worldglobal, status launched %d", __FUNCTION__, __LINE__,  mWarLaunched );
	}
}

void CWorldModule::OnSeverMessage(int nServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	switch( pMessage->mutable_msghead()->messageid() )
	{
	case ID_C2S_REQUEST_COUNTRYINFO:
		{
			OnMessageCountryInfo( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_JOINCOUNTRY:
		{
			OnMessageJoinCountry( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_LEAVECOUNTRY:
		{
			OnMessageLeaveCountry( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_SETOFFICIAL:
		{
			OnMessageSetOfficial( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_UNSETOFFICIAL:
		{
			OnMessageUnSetOfficial( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_USEFUNCTION:
		{
			OnMessageUseFunction( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_OFFICIALINFO:
		{
			OnMessageOfficialInfo( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_USETIME:
		{
			OnMessageUseTimes( nServerID, pMessage );
			break;
		}
	case ID_S2G_REQUEST_CHANGEWIZARD:
		{
			OnMessageChangeWizard( nServerID, pMessage );
			break;
		}
	case ID_S2G_REQUEST_CHANGECONSTRUCT:
		{
			OnMessageChangeConstruct( nServerID, pMessage );
			break;
		}
	case ID_S2G_REQUEST_CHANGEPOWER:
		{
			OnMessageChangePower( nServerID, pMessage );
			break;
		}
	case ID_S2G_NOTIFY_CHANGEFRIEND:
		{
			OnMessageChangeFriend( nServerID, pMessage );
			break;
		}
	case ID_S2G_REQUEST_KILLPERSON:
		{
			OnMessageKillPerson( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_WORLDQUEST:
		{
			OnMessageWorldQuest( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_WORLDANSWER:
		{
			OnMessageWorldAnswer( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_WORLDINFO:
		{
			OnMessageWorldInfo( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_PLAYERGSINFO:
		{
			OnMessagePlayerInfo( nServerID, pMessage );
			break;
		}
	case ID_S2G_REQUEST_NATIONTITLELIST:
		{
			OnMessageGetNationTitleList( nServerID, pMessage );
			break;
		}
	case ID_S2G_REQUEST_TRYSUMMON:
		{
			OnMessageTrySummon( nServerID, pMessage );
			break;
		}
	case ID_S2G_REQUEST_REALSUMMON:
		{
			OnMessageRealSummon( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_SUBMITWAR:
		{
			OnMessageSubmitWar( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_ENTERWAR:
		{
			OnMessageEnterWar( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_QUERYWARCAMP:
		{
			OnMessageQueryWarCamp( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_QUERYWARNUM:
		{
			OnMessageQueryWarNum( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_QUERYWARINFO:
		{
			OnMessageQueryWarInfo( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_QUERYRANK:
		{
			OnMessageQueryWarRank( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_BIDCAMP:
		{
			OnMessageBidCamp( nServerID, pMessage );
			break;
		}
	case ID_S2G_NOTIFY_WARCHANGE:
		{
			OnMessageWarChange( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_CAMPBIDINFO:
		{
			OnMessageCampBidInfo( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_CAMPRESINFO:
		{
			OnMessageCampResInfo( nServerID, pMessage );
			break;
		}
	case ID_S2G_NOTIFY_OBTAINWARHONOR:
		{
			OnMessageObtainWarHonor( nServerID, pMessage );
			break;
		}
	case ID_S2G_NOTIFY_WARCONTROL:
		{
			OnMessageWarControl( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_GETWARAWARD:
		{
			OnMessageGetWarAward( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_QUERYWARAWARD:
		{
			OnMessageQueryWarAward( nServerID, pMessage );
			break;
		}
	case ID_S2G_NOTIFY_WARSTONEOVER:
		{
			OnMessageWarStoneOver( nServerID, pMessage );
			break;
		}
	case ID_S2G_NOTIFY_ADDTAX:
		{
			OnMessageAddTax( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_MODIFYRATE:
		{
			OnMessageModifyRate( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_GETTAX:
		{
			OnMessageGetTax( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_REFUSESUMMON:
		{
			OnMessageRefuseSummon( nServerID, pMessage );
			break;
		}
	case ID_S2G_CAMPWEAKLIST_REQUEST:
		{
			m_WeakProtecter.OnMessageCampWeakListReq( nServerID, pMessage );
			break;
		}
	case ID_S2G_ADDHONER_NOTICE:
		{
			m_WeakProtecter.OnMessageCampAddHonerScore( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_ACCUSEKING:
		{
			OnMessageAccuseKing( nServerID, pMessage );
			break;
		}
	case ID_C2S_REQUEST_POWERINFO:
		{
			OnMessagePowerInfo( nServerID, pMessage );
			break;
		}
	case ID_S2G_NOTIFY_SPECIALEVENT:
		{
			OnMessageSpecialEvent( nServerID, pMessage );
			break;
		}
	default:
		break;
	}
}

void CWorldModule::OnMessageCountryInfo( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageCountryInfoRequest *tpMsg = ( CMessageCountryInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	SendCountryInfoResponse( tpMsg->charid(), tpMsg->worldid(), tpMsg->countryid(), tpMsg->needfull() );
}

void CWorldModule::OnMessageJoinCountry( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageJoinCountryRequest *tpMsg = ( CMessageJoinCountryRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	if ( tpPlayer->TempID( ) != COUNTRY_NONE )
	{
		SendJoinCountryResponse( tpMsg->charid(), ERROR_WORLD_JOIN_ALREADYIN, 0 );
		return;
	}
	if ( tpMsg->countryid() < 0 || tpMsg->countryid() >= COUNTRY_NUM )
	{
		SendJoinCountryResponse( tpMsg->charid(), ERROR_WORLD_JOIN_NOCOUNTRY, 0 );
		return;
	}
	tpPlayer->TempID( tpMsg->countryid() );
	CWorldManager::GetSingleton().ChangePeopleNum( tpPlayer->CountryID( ), tpPlayer->TempID( ), CHANGE_ADD );

	SendJoinCountryResponse( tpMsg->charid(), SUCCESS, tpMsg->countryid() );
}



void CWorldModule::OnMessageLeaveCountry( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageLeaveCountryRequest *tpMsg = ( CMessageLeaveCountryRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsOfPlayer( tpMsg->charid() );
	if ( tpCorps != NULL )
	{
		if ( tpCorps->IsOwner( ) )
		{
			SendJoinCountryResponse( tpMsg->charid(), ERROR_WORLD_LEAVE_ISCORE, 0 );
			return;
		}
	}

	if ( tpPlayer->TempID( ) == COUNTRY_NONE || tpPlayer->TempID( ) >= COUNTRY_NUM
		|| tpPlayer->CountryID( ) <= WORLD_START || tpPlayer->CountryID( ) >= WORLD_NUM )
	{
		SendJoinCountryResponse( tpMsg->charid(), ERROR_WORLD_LEAVE_NOCOUNTRY, 0 );
		return;
	}

	if ( tpPlayer->OfficialID( ) != 0 )
	{
		CCountry *tpCountry = &( CWorldManager::GetSingleton().mWorld[ tpPlayer->CountryID( ) ].mCountry[ tpPlayer->TempID( ) ] );
		int tLevel = -1, tOrder = -1;
		tpCountry->ClearOfficialID( tpPlayer->CharID( ), tLevel, tOrder );
		if ( tLevel != -1 && tOrder != -1 )
		{
			SendOfficialChangeNotify( tpPlayer->CharID( ), tLevel, tOrder, tpPlayer->CountryID( ), tpPlayer->TempID( ), CHANGE_DEL );
		}		
		tpPlayer->OfficialID( 0 );
	}
	CWorldManager::GetSingleton().ChangePeopleNum( tpPlayer->CountryID( ), tpPlayer->TempID( ), CHANGE_DEL );
	tpPlayer->TempID( COUNTRY_NONE );	

	SendLeaveCountryResponse( tpMsg->charid(), SUCCESS );
	SendJoinCountryNotify( tpMsg->charid(), COUNTRY_NONE );
}

void CWorldModule::SendJoinCountryNotify( int vCharID, int vCountryID )
{
	CMessage tMsgHead;
	CMessageJoinCountryNotify tMsgNotify;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vCharID );
		return;	
	}
	tMsgNotify.set_charid( vCharID );
	tMsgNotify.set_countryid( vCountryID );

	int tWorldID = tpPlayer->LineID();
	if ( tWorldID > WORLD_START && tWorldID < WORLD_NUM &&
		vCountryID > COUNTRY_NONE && vCountryID < COUNTRY_NUM )
	{
		CCountry *tpCountry = &( CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ vCountryID ] );
		tMsgNotify.set_kingid( tpCountry->GetKingID() );
	}	

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_JOINCOUNTRY );
	tMsgHead.set_msgpara( (int)&tMsgNotify );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}


void CWorldModule::SendJoinCountryResponse( int vCharID, int vResult, int vCountryID )
{
	CMessage tMsgHead;
	CMessageJoinCountryResponse tMsgResponse;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vCharID );
		return;	
	}

	tMsgResponse.set_result( vResult );
	tMsgResponse.set_charid( vCharID );
	tMsgResponse.set_countryid( vCountryID );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_JOINCOUNTRY );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}


void CWorldModule::SendCountryInfoResponse( int vCharID, int vWorldID, int vCountryID, int vNeedFull )
{
	CMessage tMsgHead;
	CMessageCountryInfoResponse tMsgResponse;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vCharID );
		return;	
	}

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_COUNTRYINFO );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	int tWorldID = vWorldID;
	if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM || vCountryID < 0 || vCountryID >= COUNTRY_NUM )
	{
		return;
	}
	tMsgResponse.set_charid( vCharID );

	CCountry *tpCountry = &( CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ vCountryID ] );
	CCorps *pCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpCountry->GetCorpsID() );
	if ( pCorps != NULL )
	{
		if ( vNeedFull == 1 )
		{
			tMsgResponse.set_corpsid( pCorps->GetCorpsID() );
			tMsgResponse.set_corpsname( pCorps->GetCorpsName() );
			tMsgResponse.set_kingid( pCorps->GetRooterID() );
			tMsgResponse.set_kingname( pCorps->GetRooterName() );
			CUnion * tpUnion = CCorpsManager::GetSingleton().GetUnionByID( pCorps->GetUnionID() );
			if ( tpUnion != NULL )
			{
				tMsgResponse.set_unionid( tpUnion->GetUnionID() );
				tMsgResponse.set_unionname( tpUnion->GetUnionName() );
			}
			if ( tWorldID == tpPlayer->CountryID() )
			{
				pCorps->GetPBBuffer( tMsgResponse.mutable_battleinfo(), false );
			}			
		}

		tMsgResponse.set_gold( pCorps->GetCorpsMoney() );		
	}

	tMsgResponse.set_charid( vCharID );
	tMsgResponse.set_kingpower( tpCountry->GetKingPower() );

	if ( vNeedFull == 1 )
	{
		tMsgResponse.set_peoplenum( tpCountry->GetPeopleNum() );
		if ( tWorldID == tpPlayer->CountryID() )
		{
			for ( int i = 0; i < WIZARD_NUM; i++ )
			{
				tMsgResponse.add_wizard( tpCountry->GetWizard( i ) );
			}
			for ( int i = 0; i < OFFICAL_NUM; i++ )
			{
				tMsgResponse.add_constructvalue( tpCountry->GetConstructValue( i ) + tpCountry->GetConstructWizard( i ) );
				//tMsgResponse.add_constructlevel( tpCountry->GetConstructLevel( i ) );
			}
		}		
	}
	tMsgResponse.set_needfull( vNeedFull );
	tMsgResponse.set_countryid( vCountryID );
	tMsgResponse.set_worldid( vWorldID );

	// 如果是长安，返回阵营级别
	if (COUNTRY_CHANGAN == vCountryID)
	{
		tMsgResponse.set_worldlevel(CWorldModule::GetSingleton().m_WeakProtecter.GetWeekLevelByWordID(vWorldID));
	}

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

void CWorldModule::OnMessageSetOfficial( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageSetOfficialRequest *tpMsg = ( CMessageSetOfficialRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	int tWorldID = tpPlayer->CountryID( );
	int tCountryID = tpPlayer->TempID( );
	if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM || tCountryID < 0 || tCountryID >= COUNTRY_NUM )
	{
		SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_NOCOUNTRY, NULL, 0, 0, 0, 0 );
		return;
	}
	CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ]);
	if ( tpMsg->charid() != (unsigned int) tpCountry->GetKingID( ) )
	{
		SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_NOTKING, NULL, 0, 0, 0, 0 );
		return;
	}

	int tOfficialLevel = tpMsg->officiallevel();
	int tOfficialOrder = tpMsg->officialorder();
	if ( tOfficialLevel < 0 || tOfficialLevel > OFFICIAL_LEVEL_NUM || 
		tOfficialOrder < 0 || tOfficialOrder > MAX_OFFICAL_NUM )
	{
		SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_NOOFFICIAL, NULL, 0, 0, 0, 0 );
		return;
	}
	CGatePlayer *tpOffPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charname().c_str() );
	if ( tpOffPlayer == NULL )
	{
		SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_NOTONLINE, NULL, 0, 0, 0, 0 );
		return;
	}

	if ( tpOffPlayer->GetNationalityID() != tpPlayer->GetNationalityID() )
	{
		SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_NOCOUNTRY, NULL, 0, 0, 0, 0 );
		return;
	}

	if ( tpOffPlayer->CharID( ) == tpCountry->GetKingID( ) )
	{
		SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_ISKING, NULL, 0, 0, 0, 0 );
		return;
	}
	if ( tpCountry->GetOfficialID( tpOffPlayer->CharID( ) ) != 0 )
	{
		SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_HASOFFICIAL, NULL, 0, 0, 0, 0 );
		return;
	}
	if ( tpOffPlayer->TempID( ) != tCountryID )
	{
		SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_ERRCOUNTRY, NULL, 0, 0, 0, 0 );
		return;
	}

	int tOfficialID = 0;
	CTemplateOfficialTable *tpOfficialTable = (CTemplateOfficialTable*)CDataStatic::GetTemp( TEMP_OFFICALLIST );
	if ( tpOfficialTable != NULL )
	{
		tOfficialID = tpOfficialTable->mOfficial[ tOfficialLevel ][ tOfficialOrder ];
		if ( tOfficialID == 0 )
		{
			SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_HASOFFICIAL, NULL, 0, 0, 0, 0 );
			return;
		}
		CTemplateOfficial *tpOfficial = (CTemplateOfficial*)CDataStatic::SearchTpl( tOfficialID );
		if ( tpOfficial == NULL )
		{
			SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_HASOFFICIAL, NULL, 0, 0, 0, 0 );
			return;
		}
		CTemplateMapTable *tpMapTable = (CTemplateMapTable*)CDataStatic::GetTemp( TEMP_MAPTABLE );
		if ( tpMapTable == NULL )
		{
			SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_LOWLEVEL, NULL, 0, 0, 0, 0 );
			return;
		}
		if ( tpOfficial->mCountryLevel > tpMapTable->mLevel[ tpCountry->GetMapID() ] )
		{
			SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_LOWLEVEL, NULL, 0, 0, 0, 0 );
			return;
		}
		if ( tpOfficial->mConstructLevel > tpCountry->GetConstructLevel( OFFICAL_RENSHI ) )
		{
			SendSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_SET_LOWCONSTRUCT, NULL, 0, 0, 0, 0 );
			return;
		}
	}	

	// 官职已经有人不能任命
	if ( tpCountry->mOfficialData[ tOfficialLevel ][ tOfficialOrder ].mOfficialID != 0 )
	{
		return;
	}

	tpCountry->mOfficialData[ tOfficialLevel ][ tOfficialOrder ].mOfficialID = tpOffPlayer->CharID( );
	strncpy( tpCountry->mOfficialData[ tOfficialLevel ][ tOfficialOrder ].mOfficialName,
		tpOffPlayer->CharName( ), sizeof( tpCountry->mOfficialData[ tOfficialLevel ][ tOfficialOrder ].mOfficialName ) );

	tpOffPlayer->OfficialID( tOfficialID );

	SendSetOfficialResponse( tpMsg->charid(), SUCCESS, (char*)tpOffPlayer->CharName(), tOfficialLevel, tOfficialOrder, tpMsg->normalset(), tpOffPlayer->CharID( ) );
	SendOfficialChangeNotify( tpOffPlayer->CharID( ), tOfficialLevel, tOfficialOrder, tWorldID, tCountryID, CHANGE_ADD );
}

void CWorldModule::OnMessageOfficialInfo( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageOfficialInfoRequest *tpMsg = ( CMessageOfficialInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	int tWorldID = tpPlayer->CountryID( );
	int tCountryID = tpPlayer->TempID( );
	if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM || tCountryID < 0 || tCountryID >= COUNTRY_NUM )
	{
		return;
	}

	SendOfficialInfoResponse( tpMsg->charid(), tWorldID, tCountryID );
}

void CWorldModule::OnSessionLoadOfficial( CMessage* pMessage, int nParam1 )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	int tcolnum = tpMsg->colcount();
	int trownum = tpMsg->rowcount();	
	if ( tpMsg->resultcode() == 1 )
	{
		for ( int i = 0; i < trownum; ++i )
		{
			int tWorldID = atoi( tpMsg->fieldvalue( i*tcolnum + 0 ).c_str() );
			if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM )
				continue;
			int tCountryID = atoi( tpMsg->fieldvalue( i*tcolnum + 1 ).c_str() );
			if ( tCountryID < 0 || tCountryID >= COUNTRY_NUM )
				continue;
			CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ]);

			PBOfficialList pbList; 
			const char* pfieldvalue = tpMsg->fieldvalue(i*tcolnum + 2).c_str(); 
			int fieldlen = tpMsg->fieldvaluelen(i*tcolnum + 2);
			if ( pbList.ParseFromArray( pfieldvalue, fieldlen) == true )
			{
				tpCountry->LoadOfficial( pbList, nParam1 );
			}
			else
			{
				printf("load official data from db failed.\n");
				LOG_ERROR( "dafault", "load official:%d  %d error", tWorldID, tCountryID );	
				exit(0);
			}

			//LOG_NOTICE( "world", "[%s:%d] offical of world %d country %d is %s",
			//__FUNCTION__, __LINE__, tWorldID, tCountryID, pbList.ShortDebugString().c_str() );
		}

		mOfficialLaunched++;

		LOG_NOTICE( "world", "[%s:%d] offical launched %d",__FUNCTION__, __LINE__, mOfficialLaunched );

	}
	else
	{
		printf("load official data from db failed.\n");
		LOG_ERROR( "dafault", "load official error" );	
		exit(0);
	}

}

void CWorldModule::OnMessageUseFunction( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageUseFunctionRequest *tpMsg = ( CMessageUseFunctionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	int tWorldID = tpPlayer->CountryID( );
	int tCountryID = tpPlayer->TempID( );
	if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM || tCountryID < 0 || tCountryID >= COUNTRY_NUM )
	{
		SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_NOCOUNTRY, NULL, 0, 0 );
		return;
	}
	CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ]);	

	int tFunctionType = tpMsg->functiontype();
	int tFunctionOrder = tpMsg->functionorder();
	if ( tFunctionType < 0 || tFunctionType > OFFICIAL_FUNCTION_KING || 
		tFunctionOrder < 0 || tFunctionOrder > MAX_POWER )
	{
		SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_NOFUNCTION, NULL, 0, 0 );
		return;
	}
	if ( tFunctionType == OFFICIAL_FUNCTION_KING && tpMsg->charid() != (unsigned int)(tpCountry->GetKingID( )) )
	{
		SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_NOTOFFICIAL, NULL, 0, 0 );
		return;
	}
	CGatePlayer *tpTarPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->targetname().c_str() );
	if ( tpTarPlayer == NULL )
	{
		SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_NOTONLINE, NULL, 0, 0 );
		return;
	}		
	// 天子特权可以对全阵营使用
	if ( tpTarPlayer->CountryID() != tWorldID || ( tpTarPlayer->TempID( ) != tCountryID && tFunctionType != OFFICIAL_FUNCTION_KING ) )
	{
		SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_ERRCOUNTRY, NULL, 0, 0 );
		return;
	}

	time_t tval;
	struct tm *now;        
	time(&tval);
	now = localtime(&tval);

	// 从天子开始检查
	for ( int i = OFFICIAL_LEVEL_NUM - 1; i >= 0; i-- )
	{
		for ( int j = 0; j < MAX_OFFICAL_NUM; j++ )
		{
			if ( (unsigned int)tpCountry->mOfficialData[ i ][ j ].mOfficialID == tpMsg->charid() )
			{
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

				// 如果限制了使用时间
				int tStartTime = tpPower->mStartHour * 60 + tpPower->mStartMin;
				int tEndTime = tpPower->mEndHour * 60 + tpPower->mEndMin;
				int tNowTime = now->tm_hour * 60 + now->tm_min;
				if ( tStartTime != 0 && tEndTime != 0 )
				{
					if ( tNowTime < tStartTime || tNowTime > tEndTime )
					{
						SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_FUNCTIONTIMEERR, NULL, 0, 0 );
						return;
					}
				}

				if ( ( tpPower->mType == CTemplatePower::POWER_PRISON || tpPower->mType == CTemplatePower::POWER_SHUT ) && 
					tpTarPlayer->CharID( ) == tpCountry->GetKingID( ) )
				{
					SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_ISKING, NULL, 0, 0 );
					return;
				}

				// 家族军团战中不能关人
				if ( tpPower->mType == CTemplatePower::POWER_PRISON &&
					( ( tpTarPlayer->MapID() == _REPE_FAMILY_MAP_ID_ ) || ( tpTarPlayer->MapID() == _REPE_CORPS_MAP_ID_ ) ) )
				{
					SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_BATTLEUSE, NULL, 0, 0 );
					return;
				}

				// 只能关红名
				if ( tpPower->mType == CTemplatePower::POWER_PRISON &&
					tpTarPlayer->PKValue() == 0 )
				{
					SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_NOTREDPRISON, NULL, 0, 0 );
					return;
				}

				if ( tFunctionType != OFFICIAL_FUNCTION_KING && tpPower->mConstruct != tpCountry->GetConstructLevel( tpPower->mOfficial ) )
				{
					SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_NOCONSTRUCT, NULL, 0, 0 );
					return;
				}

				if ( tpPower->mCountry != COUNTRY_NONE && tCountryID != tpPower->mCountry )
				{
					SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_NOCONSTRUCT, NULL, 0, 0 );
					return;
				}

				if ( tpPower->mPower > tpCountry->GetKingPower( ) )
				{
					SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_NOPOWER, NULL, 0, 0 );
					return;
				}
				CCorps *tpCorps = tpCountry->GetCorps( );
				/*if ( tpCorps == NULL )
				{
					SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_NOMONEY, NULL, 0, 0 );
					return;
				}*/
				int tMoney = tpCorps == NULL ? 0 : tpCorps->GetCorpsMoney( );
				if ( tpPower->mMoney > tMoney )
				{
					SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_NOMONEY, NULL, 0, 0 );
					return;
				}

				int tUseTimes = tpPower == NULL ? 0 : tpPower->mUseTime[ i ];
				if ( tpCountry->mOfficialData[ i ][ j ].mUsetimes[ tFunctionType ][ tFunctionOrder ] >= tUseTimes )
				{
					SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_NOTIME, NULL, 0, 0 );
					return;
				}
				// 如果是国探或者乱舞
				if ( tpPower->mType == CTemplatePower::POWER_MAKEGT )
				{
					if ( OnUseGTFunction( tWorldID, tpMsg->intparam1(), time(NULL) + tpPower->mParam1, tpPlayer ) == false )
					{
						SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_CANNOTUSEGT, NULL, 0, 0 );
						return;
					}
				}
				else if ( tpPower->mType == CTemplatePower::POWER_MAKELW )
				{
					if ( OnUseLWFunction( tWorldID, tpMsg->intparam1(), time(NULL) + tpPower->mParam1, tpPlayer ) == false )
					{
						SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_CANNOTUSELW, NULL, 0, 0 );
						return;
					}
				}
				if ( tpPower->mAllCount == 0 )
				{
					tpCountry->mOfficialData[ i ][ j ].mUsetimes[ tFunctionType ][ tFunctionOrder ]++;
				}
				else
				{
					tpCountry->AddAllCount( tFunctionType, tFunctionOrder );
				}
				SendUseFunctionResponse( tpMsg->charid(), SUCCESS, (char*)tpTarPlayer->CharName(), tFunctionType, tFunctionOrder );
				SendUseFunctionNotify( tpMsg->charid(), tpTarPlayer->CharID( ), tFunctionType, tFunctionOrder, tWorldID, tCountryID, (char*)tpPlayer->CharName(), tpMsg->teleport() );
				tpCountry->SetKingPower( tpCountry->GetKingPower( ) - tpPower->mPower );
				if ( tpCorps != NULL )
				{
					tpCorps->ConsumeCorpsMoney( tpPower->mMoney );
				}
				return;
			}
		}
	}
	SendUseFunctionResponse( tpMsg->charid(), ERROR_WORLD_USE_NOTOFFICIAL, NULL, 0, 0 );
}

void CWorldModule::SendUseFunctionResponse( int vCharID, int vResult, char * vName, int vType, int vOrder )
{
	CMessage tMsgHead;
	CMessageUseFunctionResponse tMsgResponse;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vCharID );
		return;	
	}

	tMsgResponse.set_result( vResult );
	tMsgResponse.set_charid( vCharID );
	tMsgResponse.set_functiontype( vType );
	tMsgResponse.set_functionorder( vOrder );
	if ( vName != NULL )
	{
		tMsgResponse.set_targetname( vName );
	}

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_USEFUNCTION );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

void CWorldModule::SendOfficialInfoResponse( int vCharID, int vWorldID, int vCountryID )
{
	CMessage tMsgHead;
	CMessageOfficialInfoResponse tMsgResponse;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vCharID );
		return;	
	}

	CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ vWorldID ].mCountry[ vCountryID ]);	

	for ( int i = 0; i < OFFICIAL_LEVEL_NUM; i++ )
	{
		for ( int j = 0; j < MAX_OFFICAL_NUM; j++ )
		{
			if ( tpCountry->mOfficialData[ i ][ j ].mOfficialID != 0 )
			{
				PBOfficialData *tpOfficial = tMsgResponse.mutable_officiallist()->add_officialdata();
				tpOfficial->set_charid( tpCountry->mOfficialData[ i ][ j ].mOfficialID );
				tpOfficial->set_charname( tpCountry->mOfficialData[ i ][ j ].mOfficialName );
				tpOfficial->set_officiallevel( i );
				tpOfficial->set_officialorder( j );
			}
		}
	}
	tMsgResponse.set_charid( vCharID );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_OFFICIALINFO );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

void CWorldModule::SendSetOfficialResponse( int vCharID, int vResult, char *vName, int vLevel, int vOrder, int vNormalSet, int vTargetID )
{
	CMessage tMsgHead;
	CMessageSetOfficialResponse tMsgResponse;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vCharID );
		return;	
	}

	tMsgResponse.set_result( vResult );
	tMsgResponse.set_charid( vCharID );
	if ( vName != NULL )
	{
		tMsgResponse.set_charname( vName );
	}	
	tMsgResponse.set_officiallevel( vLevel );
	tMsgResponse.set_officialorder( vOrder );
	tMsgResponse.set_normalset( vNormalSet );
	tMsgResponse.set_targetid( vTargetID );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_SETOFFICIAL );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}


void CWorldModule::SendUnSetOfficialResponse( int vCharID, int vResult, int vLevel, int vOrder )
{
	CMessage tMsgHead;
	CMessageUnSetOfficialResponse tMsgResponse;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vCharID );
		return;	
	}

	tMsgResponse.set_result( vResult );
	tMsgResponse.set_charid( vCharID );
	tMsgResponse.set_officiallevel( vLevel );
	tMsgResponse.set_officialorder( vOrder );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_UNSETOFFICIAL );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

void CWorldModule::SendLeaveCountryResponse( int vCharID, int vResult )
{
	CMessage tMsgHead;
	CMessageLeaveCountryResponse tMsgResponse;

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( vCharID );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", vCharID );
		return;	
	}

	tMsgResponse.set_result( vResult );
	tMsgResponse.set_charid( vCharID );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_LEAVECOUNTRY );
	tMsgHead.set_msgpara( (int)&tMsgResponse );

	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
}

void CWorldModule::OnMessageUnSetOfficial( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageUnSetOfficialRequest *tpMsg = ( CMessageUnSetOfficialRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	int tWorldID = tpPlayer->CountryID( );
	int tCountryID = tpPlayer->TempID( );
	if ( tWorldID <= WORLD_START || tWorldID >= WORLD_NUM || tCountryID < 0 || tCountryID >= COUNTRY_NUM )
	{
		SendUnSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_UNSET_NOCOUNTRY, 0, 0 );
		return;
	}
	CCountry *tpCountry = &(CWorldManager::GetSingleton().mWorld[ tWorldID ].mCountry[ tCountryID ]);
	if ( tpMsg->charid() != (unsigned int) tpCountry->GetKingID( ) )
	{
		SendUnSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_UNSET_NOTKING, 0, 0 );
		return;
	}

	int tOfficialLevel = tpMsg->officiallevel();
	int tOfficialOrder = tpMsg->officialorder();
	if ( tOfficialLevel < 0 || tOfficialLevel > OFFICIAL_LEVEL_NUM || 
		tOfficialOrder < 0 || tOfficialOrder > MAX_OFFICAL_NUM )
	{
		SendUnSetOfficialResponse( tpMsg->charid(), ERROR_WORLD_UNSET_NOOFFICIAL, 0, 0 );
		return;
	}

	int tCharID = tpCountry->mOfficialData[ tOfficialLevel ][ tOfficialOrder ].mOfficialID;
	CGatePlayer *tpOffPlayer = CPlayerManager::GetSingleton().ScenePlayer( tCharID );
	if ( tpOffPlayer != NULL )
	{
		tpOffPlayer->OfficialID( 0 );
	}

	tpCountry->mOfficialData[ tOfficialLevel ][ tOfficialOrder ].mOfficialID = 0;
	memset( tpCountry->mOfficialData[ tOfficialLevel ][ tOfficialOrder ].mOfficialName,
		0, sizeof( tpCountry->mOfficialData[ tOfficialLevel ][ tOfficialOrder ].mOfficialName ) );

	SendUnSetOfficialResponse( tpMsg->charid(), SUCCESS, tOfficialLevel, tOfficialOrder );
	SendOfficialChangeNotify( tCharID, tOfficialLevel, tOfficialOrder, tWorldID, tCountryID, CHANGE_DEL );
}

void CWorldModule::SendAllKingNotify( int nServerID )
{
	for ( int i = WORLD_FIRST; i < WORLD_NUM; i++ )
	{
		for ( int j = 0; j < COUNTRY_NUM; j++ )
		{
			SendKingChangeNotify( i, j, nServerID );
		}
	}
}

void CWorldModule::OnMessageAddTax( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageAddTaxNotify *tpMsg = ( CMessageAddTaxNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CWorldManager::GetSingleton().mTaxMoney += tpMsg->money();
}

void CWorldModule::OnMessageModifyRate( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageModifyRateRequest *tpMsg = ( CMessageModifyRateRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	// 不是获胜阵营国王不能修改
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	CMessage tResponseMessage;
	CMessageModifyRateResponse tResponse;
	tResponseMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_MODIFYRATE );
	tResponseMessage.set_msgpara( (long) &tResponse );
	tResponse.set_charid( tpPlayer->CharID() );

	CCountry *tpCountry = &( CWorldManager::GetSingleton().mWorld[ CWorldManager::GetSingleton().mWinWorld ].mCountry[ COUNTRY_CHANGAN ] );
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpCountry->GetCorpsID() );
	if ( tpCorps == NULL || tpPlayer->CharID() != tpCorps->GetRooterID() )
	{
		tResponse.set_result( ERROR_TAX_SETNOTKING );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tResponseMessage );
		return;
	}

	if ( CWorldManager::GetSingleton().mSetTimes >= MAX_SETTAX_TIMES )
	{
		tResponse.set_result( ERROR_TAX_OVERTIMES );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tResponseMessage );
		return;
	}

	CTemplateWorldBattleConfig *tpConfig = (CTemplateWorldBattleConfig*)CDataStatic::GetTemp( TEMP_BATTLECONFIG );
	if ( tpConfig == NULL )
		return;

	unsigned int tRate = tpMsg->rateorder();
	if ( tRate < 0 || tRate > ARRAY_CNT(tpConfig->mTax) )
	{
		return;
	}
	CWorldManager::GetSingleton().mSetRate = tRate;
	CWorldManager::GetSingleton().mSetTimes++;

	tResponse.set_result( SUCCESS );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tResponseMessage );

	CMessage tNotifyMessage;
	CMessageModifyRateNotify tNotify;
	tNotifyMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MODIFYRATE );
	tNotifyMessage.set_msgpara( (long) &tNotify );

	SendModifyRateNotify( tRate, tpPlayer, 0, -1 );
}

void CWorldModule::OnMessageGetTax( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageGetTaxRequest *tpMsg = ( CMessageGetTaxRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	// 不是获胜阵营国王不能领取
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	CMessage tResponseMessage;
	CMessageGetTaxResponse tResponse;
	tResponseMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETTAX );
	tResponseMessage.set_msgpara( (long) &tResponse );
	tResponse.set_charid( tpPlayer->CharID() );
	tResponse.set_query( tpMsg->query() );
	tResponse.set_result( SUCCESS );

	CCountry *tpCountry = &( CWorldManager::GetSingleton().mWorld[ CWorldManager::GetSingleton().mWinWorld ].mCountry[ COUNTRY_CHANGAN ] );
	CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( tpCountry->GetCorpsID() );
	if ( tpCorps == NULL || tpPlayer->CharID() != tpCorps->GetRooterID() )
	{
		tResponse.set_result( ERROR_TAX_GETNOTKING );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tResponseMessage );
		return;
	}

	int tMoney = CWorldManager::GetSingleton().mCorpsMoney;
	tResponse.set_money( tMoney );

	if ( tMoney > 0 && tpMsg->query() == 0 )
	{
		int tOldCorpsMoney = tpCorps->GetCorpsMoney();
		tpCorps->SetCorpsMoney( tpCorps->GetCorpsMoney() + tMoney );		
		CCorpservice::GetSingleton().SendNotifyCorpsMoneyChanged( tpCorps->GetCorpsID(), tpCorps->GetCorpsMoney(), tpPlayer->CharID(), tMoney, REASON_NPCWELFARE, tpPlayer->CharName() );
		LogOrganGetTax( ORGANLOGTYPE_CORPS, tpCorps->GetCorpsID(), tpPlayer->CharID(), tpCorps->GetCorpsMoney(), tOldCorpsMoney );
		CWorldManager::GetSingleton().mCorpsMoney = 0;
	}
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tResponseMessage );
}

void CWorldModule::OnMessageRefuseSummon( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageRefuseSummonRequest *tpMsg = ( CMessageRefuseSummonRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->srccharid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->srccharid() );
		return;	
	}	

	CGatePlayer *tpDesPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->descharid() );

	if ( tpDesPlayer != NULL )
	{
		CMessageRefuseSummonNotify tNotify;
		CMessage tMessage;

		tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_REFUSESUMMON );
		tMessage.set_msgpara( (uint32)&tNotify );

		tNotify.set_srcname( tpPlayer->CharName() );
		tNotify.set_srccharid( tpPlayer->CharID() );
		tNotify.set_descharid( tpDesPlayer->CharID() );
		CGateServer::GetSingleton().Send2Scene( tpDesPlayer, &tMessage );
	}
}

void CWorldModule::SendModifyRateNotify( int vRate, CGatePlayer *pPlayer, int vReal, int vWinWorld )
{
	CMessage tNotifyMessage;
	CMessageModifyRateNotify tNotify;
	tNotifyMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MODIFYRATE );
	tNotifyMessage.set_msgpara( (long) &tNotify );

	tNotify.set_rate( vRate );
	tNotify.set_real( vReal );
	if ( pPlayer != NULL )
		tNotify.set_name( pPlayer->CharName() );
	// WinWorld为-1时，表示需要群发给客户端，其他为服务器内部同步
	if ( vWinWorld != -1 )
		tNotify.set_winworld( vWinWorld );
	if ( vWinWorld == -1 )
	{
		CPlayerManager::GetSingleton().SendWorldMessageFromGate( &tNotifyMessage );
	}
	CGateServer::GetSingleton().Send2Scene( (CSceneServer*)NULL, &tNotifyMessage );
}

void CWorldModule::OnMessageAccuseKing( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageAccuseKingRequest *tpMsg = ( CMessageAccuseKingRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	int tResult = tpMsg->accuseresult();
	int tWorldID = tpMsg->worldid();
	if ( tWorldID < WORLD_FIRST || tWorldID >= WORLD_NUM )
		return;

	CMessage tMessage;
	CMessageAccuseKingResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ACCUSEKING );
	tMessage.set_msgpara( (long) &tResponse );

	CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ tWorldID ] );
	if ( tpWorld->mAccuseState != STATE_ACCUSING )
	{
		tResponse.set_responseresult( ERROR_ACCUSE_NOTTIME );
		tResponse.set_charid( tpPlayer->CharID() );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
		return;
	}

	if ( tResult == ACCUSE_YES )
	{
		tpWorld->mAccuseYes++;
	}
	else if ( tResult == ACCUSE_NO )
	{
		tpWorld->mAccuseNo++;
	}
	else if ( tResult == ACCUSE_ABORT )
	{
		tpWorld->mAccuseAbort++;
	}	

	tResponse.set_responseresult( SUCCESS );
	tResponse.set_accuseresult( tResult );
	tResponse.set_charid( tpPlayer->CharID() );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );

	SendAccuseStatusNotify( tWorldID, tpPlayer );

	LogWorldAction( tWorldID, EM_WORLD_ACCUSEVOTE, tpPlayer->CharID(), tpPlayer->Level(), tResult );
}

void CWorldModule::OnMessagePowerInfo( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageAccuseKingRequest *tpMsg = ( CMessageAccuseKingRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->charid() );

	if( tpPlayer == NULL )
	{
		LOG_ERROR( "default"," can not find player(CGatePlayer) of roleid:%d ", tpMsg->charid() );
		return;	
	}

	int tWorldID = tpPlayer->GetNationalityID();
	if ( tWorldID < WORLD_FIRST || tWorldID >= WORLD_NUM )
		return;

	CMessage tMessage;
	CMessagePowerInfoResponse tResponse;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_POWERINFO );
	tMessage.set_msgpara( (long) &tResponse );

	tResponse.set_charid( tpPlayer->CharID() );
	CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMessage );
}

void CWorldModule::SendAccuseInfoNotify( int vEventID, int vWorldID, int vInt0, int vInt1, int vInt2, int vInt3, const char *vName, int vCharID )
{
	int tIntParam[ 4 ] = { 0 };
	tIntParam[ 0 ] = vInt0;
	tIntParam[ 1 ] = vInt1;
	tIntParam[ 2 ] = vInt2;
	tIntParam[ 3 ] = vInt3;
	SendEventNotify( vEventID, vWorldID, 0, tIntParam, ARRAY_CNT(tIntParam), (char*)vName, NULL, vCharID );
}

void CWorldModule::OnMessageSpecialEvent( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageSpecialEventNotify *tpMsg = ( CMessageSpecialEventNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CTemplateAccuseConfig *tpConfig = (CTemplateAccuseConfig*)CDataStatic::GetTemp( TEMP_ACCUSECONFIG );
	if ( tpConfig == NULL )
		return;

	int tSrcWorldID = tpMsg->srcworldid();
	int tDesWorldID	= tpMsg->desworldid();
	int tEventType = tpMsg->eventtype();
	if ( tSrcWorldID < WORLD_FIRST || tSrcWorldID >= WORLD_NUM || tDesWorldID < WORLD_FIRST || tDesWorldID >= WORLD_NUM )
		return;

	CWorld *tpSrcWorld = &( CWorldManager::GetSingleton().mWorld[ tSrcWorldID ] );
	CWorld *tpDesWorld = &( CWorldManager::GetSingleton().mWorld[ tDesWorldID ] );

	CRoleGlobalInfo* pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpMsg->charid() );
	if ( pRoleInfo == NULL )
		return;

	CCountry *tpSrcCountry = &( tpSrcWorld->mCountry[ COUNTRY_CHANGAN ] );
	CCountry *tpDesCountry = &( tpDesWorld->mCountry[ COUNTRY_CHANGAN ] );
	int tSrcScore = tpSrcWorld->m_nCampTotalScore;
	int tDesScore = tpDesWorld->m_nCampTotalScore;
	int tAvarage = ( tSrcScore + tDesScore ) / 2;
	if ( tEventType == CTemplateNpc::EVENT_GATE )
	{
		tpDesWorld->mGateTimes++;
		if ( tpDesWorld->mGateTimes < tpConfig->mGateNum )
		{
			int tParamA = ( tSrcScore > tDesScore ) ? tpConfig->mGateParam1 : tpConfig->mGateParam2;
			int tParamB = tpConfig->mGateParam3;
			float tDivideS = tAvarage == 0 ? 1.0 : tSrcScore / (float)tAvarage;
			float tDivideD = tAvarage == 0 ? 1.0 : tDesScore / (float)tAvarage;
			int tSrcPower = tpConfig->mGatePower * ( 2 - tDivideS );
			int tDesPower = tpConfig->mGatePower * ( tDivideD ) * ( tParamA / SERVER_PERCENT_FLOAT + tParamB / SERVER_PERCENT_FLOAT * tpDesWorld->mGateTimes );	
			// 如果是杀死乱舞阵营的门
			if ( tpSrcWorld->mLWWorld == tDesWorldID )
			{
				tSrcPower *= tpConfig->mLWPercent / SERVER_PERCENT_FLOAT;
			}
			tpSrcCountry->SetKingPower( tpSrcCountry->GetKingPower() + tSrcPower );
			tpDesCountry->SetKingPower( tpDesCountry->GetKingPower() - tDesPower );
			SendAccuseInfoNotify( NOTICE_ACCUSE_CHANGEPOWER, 0, tSrcWorldID, tDesWorldID, tSrcPower, tDesPower, NULL );
			LogWorldAction( tSrcWorldID, EM_WORLD_POWERADD, tpSrcCountry->GetKingPower(), tSrcPower, tEventType );
			LogWorldAction( tDesWorldID, EM_WORLD_POWERDEL, tpDesCountry->GetKingPower(), tDesPower, tEventType );
		}
		SendAccuseInfoNotify( NOTICE_ACCUSE_KILLGATE, 0, tSrcWorldID, tDesWorldID, tpDesWorld->mGateTimes, 0, (char*)pRoleInfo->RoleName() );		
	}
	else if ( tEventType == CTemplateNpc::EVENT_FLAG )
	{
		tpDesWorld->mFlagTimes++;
		if ( tpDesWorld->mFlagTimes < tpConfig->mFlagNum )
		{
			int tParamA = ( tSrcScore > tDesScore ) ? tpConfig->mFlagParam1 : tpConfig->mFlagParam2;
			int tParamB = tpConfig->mFlagParam3;
			float tDivideS = tAvarage == 0 ? 1.0 : tSrcScore / (float)tAvarage;
			float tDivideD = tAvarage == 0 ? 1.0 : tDesScore / (float)tAvarage;
			int tSrcPower = tpConfig->mFlagPower * ( 2 - tDivideS );
			int tDesPower = tpConfig->mFlagPower * ( tDivideD ) * ( tParamA / SERVER_PERCENT_FLOAT + tParamB / SERVER_PERCENT_FLOAT * tpDesWorld->mFlagTimes );
			// 如果是杀死乱舞阵营的门
			if ( tpSrcWorld->mLWWorld == tDesWorldID )
			{
				tSrcPower *= tpConfig->mLWPercent / SERVER_PERCENT_FLOAT;
			}
			tpSrcCountry->SetKingPower( tpSrcCountry->GetKingPower() + tSrcPower );
			tpDesCountry->SetKingPower( tpDesCountry->GetKingPower() - tDesPower );
			SendAccuseInfoNotify( NOTICE_ACCUSE_CHANGEPOWER, 0, tSrcWorldID, tDesWorldID, tSrcPower, tDesPower, NULL );
			LogWorldAction( tSrcWorldID, EM_WORLD_POWERADD, tpSrcCountry->GetKingPower(), tSrcPower, tEventType );
			LogWorldAction( tDesWorldID, EM_WORLD_POWERDEL, tpDesCountry->GetKingPower(), tDesPower, tEventType );
		}
		SendAccuseInfoNotify( NOTICE_ACCUSE_KILLFLAG, 0, tSrcWorldID, tDesWorldID, tpDesWorld->mFlagTimes, 0, (char*)pRoleInfo->RoleName() );		
	}
}

void CWorldModule::SendAccuseKingNotify( int vWorldID, CGatePlayer *pPlayer )
{
	if ( vWorldID < WORLD_FIRST || vWorldID >= WORLD_NUM )
		return;
	CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ vWorldID ] );

	if ( tpWorld->mAccuseState != STATE_ACCUSING )
		return;

	CMessage tNotifyMessage;
	CMessageAccuseKingNotify tNotify;
	tNotifyMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_ACCUSEKING );
	tNotifyMessage.set_msgpara( (long) &tNotify );
	
	tNotify.set_kingname( tpWorld->mCountry[ COUNTRY_CHANGAN ].GetKingName() );
	if ( pPlayer == NULL )
	{
		CPlayerManager::GetSingleton().SendCountryMessageFromGate( &tNotifyMessage, vWorldID );
	}
	else
	{
		tNotify.set_charid( pPlayer->CharID() );
		CGateServer::GetSingleton().Send2Scene( pPlayer, &tNotifyMessage );
	}
}

void CWorldModule::SendAccuseStatusNotify( int vWorldID, CGatePlayer *pPlayer )
{
	if ( vWorldID < WORLD_FIRST || vWorldID >= WORLD_NUM )
		return;
	CWorld *tpWorld = &( CWorldManager::GetSingleton().mWorld[ vWorldID ] );

	if ( tpWorld->mAccuseState != STATE_ACCUSING )
		return;

	CMessage tNotifyMessage;
	CMessageAccuseStatusNotify tNotify;
	tNotifyMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_ACCUSESTATUS );
	tNotifyMessage.set_msgpara( (long) &tNotify );
	
	tNotify.set_kingname( tpWorld->mCountry[ COUNTRY_CHANGAN ].GetKingName() );
	tNotify.set_yesnum( tpWorld->mAccuseYes );
	tNotify.set_nonum( tpWorld->mAccuseNo );
	tNotify.set_abortnum( tpWorld->mAccuseAbort );
	if ( pPlayer == NULL )
	{
		CPlayerManager::GetSingleton().SendCountryMessageFromGate( &tNotifyMessage, vWorldID );
	}
	else
	{
		tNotify.set_charid( pPlayer->CharID() );
		CGateServer::GetSingleton().Send2Scene( pPlayer, &tNotifyMessage );
	}
}

bool CWorldModule::OnUseGTFunction( int nSrcWorldID, int nDesWorldID, int nTime, CGatePlayer *pPlayer )
{
	if ( nSrcWorldID < WORLD_FIRST || nSrcWorldID >= WORLD_NUM 
		|| nDesWorldID < WORLD_FIRST || nDesWorldID >= WORLD_NUM )
		return false;
	if ( nSrcWorldID == nDesWorldID )
		return false;
	if ( CWorldManager::GetSingleton().GetGTTarget( nSrcWorldID ) != 0 || 
		CWorldManager::GetSingleton().GetGTEndTime( nSrcWorldID ) != 0 )
		return false;
	CWorldManager::GetSingleton().SetGT( nSrcWorldID, nDesWorldID, nTime );
	SendMakeGTNotify( nSrcWorldID, nDesWorldID, nTime, pPlayer );
	return true;
}

bool CWorldModule::OnUseLWFunction( int nSrcWorldID, int nDesWorldID, int nTime, CGatePlayer *pPlayer )
{
	if ( nSrcWorldID < WORLD_FIRST || nSrcWorldID >= WORLD_NUM 
		|| nDesWorldID < WORLD_FIRST || nDesWorldID >= WORLD_NUM )
		return false;
	if ( nSrcWorldID == nDesWorldID )
		return false;
	if ( CWorldManager::GetSingleton().GetLWTarget( nSrcWorldID ) != 0 || 
		CWorldManager::GetSingleton().GetLWEndTime( nSrcWorldID ) != 0 )
		return false;
	CWorldManager::GetSingleton().SetLW( nSrcWorldID, nDesWorldID, nTime );
	SendMakeLWNotify( nSrcWorldID, nDesWorldID, nTime, pPlayer );
	return true;
}

void CWorldModule::SendMakeGTNotify( int nSrcWorldID, int nDesWorldID, int nTime, CGatePlayer *pPlayer )
{
	if ( nSrcWorldID < WORLD_FIRST || nSrcWorldID >= WORLD_NUM )
		return;

	CMessage tNotifyMessage;
	CMessageMakeGuoTanNotify tNotify;
	tNotifyMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MAKEGUOTAN );
	tNotifyMessage.set_msgpara( (long) &tNotify );

	tNotify.set_srcworldid( nSrcWorldID );
	tNotify.set_desworldid( nDesWorldID );
	tNotify.set_targettime( nTime );
	if ( pPlayer != NULL )
	{
		tNotify.set_kingname( pPlayer->CharName() );
	}

	CPlayerManager::GetSingleton().SendCountryMessageFromGate( &tNotifyMessage, nSrcWorldID );
	CPlayerManager::GetSingleton().SendCountryMessageFromGate( &tNotifyMessage, nDesWorldID );

	CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tNotifyMessage );
}

void CWorldModule::SendMakeLWNotify( int nSrcWorldID, int nDesWorldID, int nTime, CGatePlayer *pPlayer )
{
	if ( nSrcWorldID < WORLD_FIRST || nSrcWorldID >= WORLD_NUM )
		return;

	CMessage tNotifyMessage;
	CMessageMakeLuanWuNotify tNotify;
	tNotifyMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MAKELUANWU );
	tNotifyMessage.set_msgpara( (long) &tNotify );

	tNotify.set_srcworldid( nSrcWorldID );
	tNotify.set_desworldid( nDesWorldID );
	tNotify.set_targettime( nTime );
	if ( pPlayer != NULL )
	{
		tNotify.set_kingname( pPlayer->CharName() );
	}

	CPlayerManager::GetSingleton().SendCountryMessageFromGate( &tNotifyMessage, nSrcWorldID );
	CPlayerManager::GetSingleton().SendCountryMessageFromGate( &tNotifyMessage, nDesWorldID );

	CGateServer::GetSingleton().Send2Scene( (CSceneServer *)NULL, &tNotifyMessage );
}

