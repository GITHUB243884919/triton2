#include "fcmservice.h"
#include "gateobj_manager.h"
#include "gatelogic.h"
#include "servermessage_in.h"
#include "nameservice.h"
#include "familyservice.h"
#include "familymessage_pb.hxx.pb.h"
#include "family.h"
#include "familydbinfo_pb.hxx.pb.h"
#include "family_manager.h"
#include "family.h"
#include "servermessage_pb.hxx.pb.h"		
#include <google/protobuf/descriptor.h>  
#include "corpsmanager.h"
#include "core_service.h"
#include "tinyxml.h"
#include "repetionmodule.h"
#include "log_tools.h"
#include "world.h"
#include "template.h"

using namespace GateServer;	
template<> CServiceFamily* CSingleton< CServiceFamily >::spSingleton = NULL;
CFamilyManager *gpFamilyManager = NULL;
void CServiceFamily::OnLaunchServer()
{  
	CFamilyManager::mShmPtr = mShmPtr;
	gpFamilyManager = new CFamilyManager;	  
	LK_ASSERT( gpFamilyManager != NULL, return );	
	
	ReloadCfg();

	const char* pcConfigPath = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"family/familyconfig.xml");
	CGateServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CServiceFamily::LoadFamilyConfigCallback);
	
	if ( mShmPtr->GetInitMode() == SHM_INIT )
	{ 	
		GetFamilyCountFromDB();
	}
}

void CServiceFamily::LoadFamilyFromDB( int nStart, int nTotalCount )
{
	const char* stmtfmt = "select country_id, family_id, family_name, family_notice, create_role_id, create_role_name, member_info, corps_id, family_status, disband_time, npc_id, bid_money, bid_time, war_rank, war_winnum, war_lostnum, family_starlevel, war_exp, family_glory, war_status, family_money, family_npcwelfaretime, family_level, family_repetionopendate, family_repetionopentimes, family_repetionscore,  family_shopinfo, family_stability, family_weeknum from UMS_FAMILY limit %d, %d";
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_allfamilies, nStart, nTotalCount, 2, SQL_NEED_CALLBACK|SQL_LATEST_INC_ID, SELECT, stmtfmt, nStart, FAMILY_FETCH_COUNT );
}

void CServiceFamily::OnSessionLoadFamilyFromDB( CMessage* pMessage, int nStart, int nTotalCount )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageExecuteSqlResponse3* tpMessage = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	if (pMessage != NULL)
	{
		int colnum = tpMessage->colcount();
		int rownum = tpMessage->rowcount();		
		if ( colnum != 29 )
		{	
			LOG_ERROR( "family","[%s,%d] Load FamilyInfo failed! ",__FUNCTION__,__LINE__ );		
			printf( "Load FamilyInfo failed!" );			
			exit( 0 );
		}
		if ( tpMessage->resultcode() == 1 )
		{
			for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
			{
				CFamily* pfamily = ( CFamily* ) CGateObjManager::GetSingleton().CreateObject( OBJTYPE_FAMILY );

				// assert pfamily is not null object. 
				LK_ASSERT ( pfamily != NULL, continue );
				// pfamily->SetID( pfamily->get_id() );
				pfamily->SetCountryID(  atoi(tpMessage->fieldvalue(0 + tRowIndex * colnum).c_str()) );
				pfamily->SetID( atoi(tpMessage->fieldvalue(1 + tRowIndex * colnum).c_str()) );
				pfamily->SetFamilyDBID( pfamily->GetID() );
				pfamily->SetName(tpMessage->fieldvalue(2 + tRowIndex * colnum).c_str());
				pfamily->SetBulletin(tpMessage->fieldvalue(3 + tRowIndex * colnum).c_str());
				pfamily->SetRooter(atoi(tpMessage->fieldvalue(4 + tRowIndex * colnum).c_str()));
				pfamily->SetRooterName( tpMessage->fieldvalue( 5 + tRowIndex * colnum ).c_str() );
				
				//Members is proto, convert to struct.
				PBFamilyMemberInfoList pbMemberList; 
				const char* pfieldvalue = tpMessage->fieldvalue(6 + tRowIndex *colnum).c_str(); 
				int fieldlen = tpMessage->fieldvaluelen(6 + tRowIndex *colnum);
				if ( pbMemberList.ParseFromArray( pfieldvalue, fieldlen) == true )
				{
					pfamily->CreatMemberInfo( &pbMemberList );
				}
				else
				{
					LOG_ERROR( "family", "load family:%s error",pfamily->GetName() );	
				}				
				pfamily->SetCorpsID( atoi(tpMessage->fieldvalue(7 + tRowIndex * colnum).c_str()) );				
				pfamily->SetFamilyStatus( atoi(tpMessage->fieldvalue(8+ tRowIndex * colnum).c_str()) );
				pfamily->SetFamilyDisbandTime( atoi(tpMessage->fieldvalue(9+ tRowIndex * colnum).c_str()) );
				pfamily->SetNpcID( atoi(tpMessage->fieldvalue(10 + tRowIndex * colnum).c_str()) );				
				pfamily->SetBidMoney( atoi(tpMessage->fieldvalue(11+ tRowIndex * colnum).c_str()) );
				pfamily->SetBidTime( atoi(tpMessage->fieldvalue(12+ tRowIndex * colnum).c_str()) );			
				
				pfamily->SetWarRank( atoi(tpMessage->fieldvalue(13+ tRowIndex * colnum).c_str()) );
				pfamily->SetPVPWinNum( atoi(tpMessage->fieldvalue(14+ tRowIndex * colnum).c_str()) );
				pfamily->SetPVPFaildNum( atoi(tpMessage->fieldvalue(15+ tRowIndex * colnum).c_str()) );
				pfamily->SetPVPStarLevel( atoi(tpMessage->fieldvalue(16+ tRowIndex * colnum).c_str()) );
				pfamily->SetPVPScore( atoi(tpMessage->fieldvalue(17+ tRowIndex * colnum).c_str()) );
				pfamily->SetGlory( atoi(tpMessage->fieldvalue(18+ tRowIndex * colnum).c_str()) );
				pfamily->SetBattleState( STATE_BIDDING );								
				pfamily->SetMoney( atoi(tpMessage->fieldvalue( 20 + tRowIndex * colnum).c_str()) );
				pfamily->SetNpcWelfareGetTime( atoi(tpMessage->fieldvalue( 21 + tRowIndex * colnum).c_str()) );
				pfamily->SetLevel( atoi(tpMessage->fieldvalue( 22 + tRowIndex * colnum).c_str()) );
				pfamily->SetRepetionDate( atoi(tpMessage->fieldvalue( 23 + tRowIndex * colnum).c_str()) );
				pfamily->SetRepetionOpenTimes( atoi(tpMessage->fieldvalue( 24 + tRowIndex * colnum).c_str()) );
				pfamily->SetRepetionScore( atoi(tpMessage->fieldvalue( 25 + tRowIndex * colnum).c_str()) );
				
				// 商店信息
				PBLeagueShopsListInfo tLeagueShopInfo;
				const char *pShopField = tpMessage->fieldvalue( 26 + tRowIndex * colnum).c_str();
				int pShopFieldLen = tpMessage->fieldvalue( 26 + tRowIndex * colnum).length();

				if ( tLeagueShopInfo.ParseFromArray( pShopField, pShopFieldLen ) == true )
				{
					pfamily->GetFamilyShop(  )->SetShopListInfo( tLeagueShopInfo );
				}
				else
				{
					LOG_ERROR( "family", "load family ( name: %s, ID:%d ) error, shop info wrong",pfamily->GetFamilyNameShow(), pfamily->GetID() );	
				}					
				pfamily->SetStabilityDegree( atoi(tpMessage->fieldvalue( 27 + tRowIndex * colnum).c_str()) );
				pfamily->SetWeekNum( atoi(tpMessage->fieldvalue( 28 + tRowIndex * colnum).c_str()) );
				pfamily->ClearSaveStatus( );
				int tRet = CFamilyManager::GetSingleton().InsertFamily( pfamily );
				if ( tRet == -1 )
				{
					CGateObjManager::GetSingleton().DestroyObject( pfamily->get_id() );
				}
				else 
				{
					//SendFamilyIDName( pfamily->GetID( ), pfamily->GetName( ) );
					if ( pfamily->GetNpcID() != 0 )
					{
						CFamilyManager::GetSingleton().InsertNPC( pfamily->GetNpcID(), pfamily->GetCountryID() );
						// 如果没有竞价和时间，是家族占领的NPC
						if ( pfamily->GetBidMoney() == 0 && pfamily->GetBidTime() == 0 )
						{
							CFamilyManager::GetSingleton().SetNpcOwner( pfamily->GetNpcID(), pfamily->GetID(), pfamily->GetCountryID(), true );
							//SendNpcList( pfamily->GetNpcID(), pfamily->GetID(), pfamily->GetCountryID() );
						}
						// 否则加入这个家族的竞价
						else
						{
							CFamilyManager::GetSingleton().AddBid( pfamily->GetNpcID(), pfamily->GetID(), pfamily->GetBidMoney(), pfamily->GetBidTime(), pfamily->GetCountryID(), true );
						}
					}
				}	 					
			}

			LOG_NOTICE( "family", "Load family data start %d num %d total %d",nStart, rownum, nTotalCount );
			if ( rownum < FAMILY_FETCH_COUNT )
			{	
				if ( nStart + rownum < nTotalCount )
				{
					printf( "[%s,%d] load family from dataserver, incorrect number, number launched(%d) total num(%d)  ", 
						__FUNCTION__,__LINE__, nStart+rownum, nTotalCount );
						
					LOG_ERROR( "family","[%s,%d] load family from dataserver, incorrect number, number launched(%d) total num(%d)  ", 
					__FUNCTION__,__LINE__, nStart+rownum, nTotalCount );
					exit( 0 );
				}	 				
			}
			else
			{
				if ( nStart + rownum < nTotalCount )
				{
					LoadFamilyFromDB( nStart+rownum, nTotalCount );
				}				
			}
			
			// 加载完成
			if( nStart + rownum >= nTotalCount )
			{
				mIsLaunched = true;	   			
				LOG_NOTICE("family", "load all family count:(%d) completed", nStart+rownum );
				return;					
			}				
		}
		else
		{
			printf( "LoadFamilyFromDB from %d count %d error", nStart, FAMILY_FETCH_COUNT );
			LOG_ERROR( "default", "LoadFamilyFromDB from %d count %d error", nStart, FAMILY_FETCH_COUNT );
			exit( 0 );
		}
	}
}

void CServiceFamily::OnExitServer()
{
	// 保存家族的数据信息
	CFamilyManager::GetSingleton().UpDateAllFamilyDBInfo();			
}

void CServiceFamily::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	
	if ( mDisbandTimer.Elapse( vTickOffset ) == true )
	{
		CFamilyManager::GetSingleton().DeleteFamilyOnTimeDisband( );
	}
	
	if ( mUpdateTimer.Elapse( vTickOffset ) == true )
	{	
		CFamilyManager::GetSingleton().UpDateAllFamilyDBInfo( );
		ReFreshFamilyStatus();		
	}
}

void CServiceFamily::OnMessage( int nServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL , return);	

	CMessageHead* tpHead = tpMessage->mutable_msghead();
	unsigned short nMessageID = (unsigned short) tpHead->messageid();

	switch (nMessageID)
	{	
		/*
		case ID_S2G_FANGCHENMI_REQUEST:
		{
			OnMessageFangChenMiRequest(nServerID, tpMessage);
			break;
		}
		*/

		default:
		{
			LOG_ERROR("default", "there is no action for the message_id(%d)", nMessageID);
			return;
		}
	}
}

void CServiceFamily::OnMessageFangChenMiRequest( int vServerID, CMessage* tpMessage )
{
	//LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	//CMessageFangChenMiRequest *tpMsg = ( CMessageFangChenMiRequest * )tpMessage->msgpara();

}

// ***********************************************************
//  Function:		GetFamilyCountFromDB
//  Description:	获取数据库家族数量
//  Output:			
//  Date:	01/07/2009
// **********************************************************
void CServiceFamily::GetFamilyCountFromDB()
{
	const char* stmtfmt = "select count(*) from UMS_FAMILY ";
	CGateLogic::GetSingleton().ExecuteSql( em_dbsession_getfamilynum, 0, 0, 2, SQL_NEED_CALLBACK, SELECT, stmtfmt );
}

// ***********************************************************
//  Function:		OnSessionGetFamilyCountFromDB
//  Description:	从数据获取家族数量的回应处理函数
//  Output:			
//  Date:	01/07/2009
// **********************************************************
void CServiceFamily::OnSessionGetFamilyCountFromDB( CMessage* pMessage, int nParam1, int nParam2 )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageExecuteSqlResponse3* tpMessage = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	if ( tpMessage != NULL )
	{
		// 获取成功
		if ( tpMessage->resultcode() == 1 )
		{
			if ( tpMessage->fieldvalue_size() < 1 )
			{
				LOG_ERROR( "family","error occured when get family number because of no value" );
				exit( 0 );
				return;
			}
			
			int tTotalCount = atoi( tpMessage->fieldvalue( 0 ).c_str() );
			int nstart = 0;	 
			LoadFamilyFromDB( nstart, tTotalCount );
		}
		else
		{
			LOG_ERROR( "family","error occured when get family number , database error" );
			exit( 0 );
		}
		
	}
	else
	{
		LOG_ERROR( "family","error occured when get family number, empty message" );
		exit( 0 );
	}
}

unsigned int CServiceFamily::CountSize()
{
	unsigned int tSize = sizeof( CServiceFamily );
	tSize += CFamilyManager::CountSize();
	return tSize;
}


CServiceFamily::CServiceFamily()
{
	if( mShmPtr->GetInitMode() == SHM_INIT )		
	{
		Initialize();
	}else
	{
		Resume();
	}
}

CServiceFamily::~CServiceFamily()
{

}

int CServiceFamily::Initialize()
{
	mUpdateTimer =  CWTimer( FAMILYTIMER_UPDATE_TIMELENGTH );
	mDisbandTimer = CWTimer( FAMILYTIMER_DISBAND_TIMELENGTH );
	mIsLaunched = false;
	return 0;
}

int CServiceFamily::Resume()
{
	return 0;
}

void* CServiceFamily::operator new( size_t size )    
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CServiceFamily::operator delete( void* pointer )
{

}

void CServiceFamily::OnSeverMessage( int vServerID, CMessage* pMessage )
{
   	LK_ASSERT( pMessage != NULL, return );	      	
		
	switch ( pMessage->mutable_msghead()->messageid() )
	{
		case ID_C2S_REQUEST_CREATFAMILY:
		{
			OnMessageCreatFamily( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_INVITEMEMBER:
		{
			OnMessageInviteMember( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_REPLYFAMILYINVITE:
		{
			OnMessageReplyFamilyInvite( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_FAMILYAPPLY:
		{
			OnMessageApplyFamily( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_REPLYMEMBERAPPLY:
		{
			OnMessageReplyMemberApply( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_POSTAPPOINT:
		{
			OnMessagePostAppoint( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_LEAVEFAMILY:
		{
			OnMessageLeaveFamily( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_GETFAMILYMEMBERINFO:
		{
			OnMessageGetFamilyMemberInfo( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_GETFAMILYPROPERTY:
		{
			OnMessageGetFamilyPropetyInfo( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_GETALLFAMILYINFO:
		{
			OnMessageGetAllFamilyInfo( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_GETAPPLYLIST:
		{
			OnMessageGetApplyList( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_KICKMEMBER:
		{
			OnMessageKickPlayer( vServerID, pMessage );
		}
		break;	
		case ID_C2S_REQUEST_DISBANDFAMILY:
		{
			OnMessageDisbandFamily( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_CHANGENOTICE:
		{
			OnMessageChangeFamilyNotice( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_APPOINTHEADER:
		{
			OnMessageAppointHeader( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_CANCELFAMILYDISBAND:
		{
			OnMessageCancelFamilyDisband( vServerID, pMessage );
		}
		break;	
		case ID_C2S_REQUEST_CLEARFAMILYAPPLYLIST:
		{
			OnMessageClearFamilyApply( vServerID, pMessage );
		}	
		break;
		case ID_C2S_REQUEST_REFRESHFAMILYINFO:
		{
			OnMessageRefreshFamilyInfo( vServerID, pMessage );
		}
		break;		
		case ID_C2S_REQUEST_SETFAMILYMEMBERPOST:
		{
			 OnMessageSetFamilyMemberPost( vServerID, pMessage );
		}
		break;

		case ID_C2S_REQUEST_LEVELUPFAMILY:
		{
			OnMessageLevelUPFamily( vServerID, pMessage );
		}
		break;		

		case ID_C2S_REQUEST_CONTRIBUTEFAMILYMONEY:
		{
			OnMessageContributeFamilyMoney( vServerID, pMessage );
		}
		break;		

		case ID_C2S_REQUEST_OBTAINFAMILYMONEY:
		{
			OnMessageObtainFamilyMoney( vServerID, pMessage );
		}
		break;
				
		case ID_C2S_REQUEST_BIDNPC:
		{
			OnMessageBidNpc( vServerID, pMessage );
		}
		break;
		case ID_S2G_NOTIFY_BIDTIMEOUT:
		{
			OnMessageBidTimeOut( vServerID, pMessage );
		}
		break;
		case ID_S2G_REQUEST_BATTLESTART:
		{
			OnMessageBattleStart( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_BIDINFO:
		{
			OnMessageNpcInfo( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_MEMBERSUBMIT:
		{
			OnMessageMemberSubmit( vServerID, pMessage );
		}
		break;
		case ID_S2G_NOTIFY_BATTLEEND:
		{
			OnMessageBattleEnd( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_ABORTNPC:
		{
			OnMessageAbortNpc( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_MAKEROOM:
		{
			OnMessageMakeRoom( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_CHAROOM:
		{
			OnMessageChaRoom( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_AGREEBATTLE:
		{
			OnMessageAgreeBattle( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_QUERYALLROOM:
		{
			OnMessageQueryAllRoom( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_QUERYMYROOM:
		{
			OnMessageQueryMyRoom( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_QUERYMYCHA:
		{
			OnMessageQueryMyCha( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_ABORTROOM:
		{
			OnMessageAbortRoom( vServerID, pMessage );
		}
		break;
		case ID_C2S_REQUEST_ABORTCHA:
		{
			OnMessageAbortCha( vServerID, pMessage );
		}
		break;
		
		case ID_S2G_NOTIFY_CHANGEFAMILYCONTRIBUTE:
		{
			OnMessageChangeFamilyContribute( pMessage );
		}
		break;
		
		case ID_S2G_NOTIFY_CHANGEFAMILYMONEY:
		{
			OnMessageChangeFamilyMoney( pMessage );
		}
		break;		
		
		case ID_S2G_NOTIFY_CHANGEFAMILYGLORY:
		{
			OnMessageChangeFamilyGlory( pMessage );
		}
		break;
		
		case ID_C2S_REQUEST_STARTFAMILYBOSS:
		{
			OnMessageStartFamilyBoss( vServerID, pMessage );
		}		
		break;
		
		
		case ID_C2S_REQUEST_QUERYNPCWELFARE:
		{
			OnMessageQueryFamilyNpcWelfare( vServerID, pMessage );			
		}
		break;
		
		case ID_C2S_REQUEST_GETFAMILYMONEY:
		{
			OnMessageGetFamilyMoney( vServerID, pMessage );
		}
		break;
		
		case ID_C2S_REQUEST_GETEXPCARD:
		{
			OnMessageGetExpCard( vServerID, pMessage );  			
		}
		break;	
		
		case ID_S2G_REQUEST_BUYFAMILYITEM:
		{
			OnMessageBuyFamilyItem( vServerID, pMessage );
		}	
		break;
		
		case ID_C2S_REQUEST_ENTERBOSSREPTION:
		{
			OnMessageEnterFamilyRepetion( vServerID, pMessage );
		}
		break;
		
		case ID_G2S_NOTIFY_CLEARFAMILYREPETIONIFO:
		{
			OnMessageClearFamilyRepetionInfo( pMessage );
		}		
		break;
		
		case ID_C2S_REQUEST_GETFAMILYGOODSLIST:
		{
			OnMessageGetFamilyGoodsList( vServerID, pMessage );
		}
		break;
		
		case ID_C2S_REQUEST_GETFAMILYEXCHANGEGOODSLIST:
		{
			OnMessageGetFamilyExchangeGoodsList( vServerID, pMessage );
		}
		break;
		
		case ID_S2G_REQUEST_FAMILYEXCHANGE:
		{
			OnMessageFamilyExchange( vServerID, pMessage );
		}
		break;
		case ID_S2G_NOTIFY_ADDFAMILYEXCHANGEITEM:
		{
			OnMessageAddFamilyExchangeItem( vServerID, pMessage );
		}
		break;
		
		case ID_S2G_NOTIFY_CHANGEFAMILYSTABILITY:
		{
			OnMessageChangeFamilyStability( pMessage );
		}
		break;
		default:
			break;
	}	
}

// ***********************************************************
//  Function:		OnMessageCreatFamily
//  Description:	创建家族的消息
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageCreatFamily( int vServerID, CMessage *pMessage )
{  
	LK_ASSERT( pMessage != NULL, return );
	CMessageCreatFamilyRequest *tpMsg = ( CMessageCreatFamilyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg(), return ) 	
	tpHead = tpMsg->mutable_headmsg();
	
	if ( tpMsg->familyname().length() >= MAX_FAMILYNAME_LENGTH*MAX_UTF8_BYTES )
	{
		//SendResponseCreatFamily( vServerID, tpHead->roleid(), tRet, FamilyID, tpMsg->familyname().c_str(), tpHead->entityid() );
		LOG_ERROR( "family", " FamilyName too Long where RoleID:(%d) creat family ", tpHead->roleid() );
		return;
	}		
	
	int FamilyID = 0;
	int tRet = CFamilyManager::GetSingleton().OnEventEstablishFamily( tpHead->roleid(), tpMsg->familyname().c_str(), FamilyID );
	if ( tRet != -1 && tRet != SUCCESS )
	{
		SendResponseCreatFamily( vServerID, tpHead->roleid(), tRet, FamilyID, tpMsg->familyname().c_str(), tpHead->entityid() );
	}
}

// ***********************************************************
//  Function:		OnMessageDisbandFamily
//  Description:	解散家族的消息
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageDisbandFamily( int vServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageDisbandFamilyRequest *tpMsg = ( CMessageDisbandFamilyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg( ), return ) 	
	tpHead = tpMsg->mutable_headmsg( );	 
	
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );	
	if ( pGatePlayer == NULL )
	{
		LOG_ERROR("family"," can not find player who has roleid:%d ", tpHead->roleid() );
		return;
	}
	int FamilyMemberList[ MAX_FAMILYMEMBER_NUM ];
	int FamilyNum = MAX_FAMILYMEMBER_NUM;
	int tRet = CFamilyManager::GetSingleton().OnEventDismissFamily( tpHead->roleid( ), FamilyMemberList, FamilyNum );
	if ( tRet == -1 )
	{
		return;
	}
	if ( tRet == 0 )
	{
		
		// TODO:发给所有在线成员家族已经解散
		// SendNotifyDisbandFamily( tpMsg->msgcode(), FamilyMemberList, FamilyNum );
	}
	SendResponseDisbandFamily( vServerID, pGatePlayer->mCharID, tRet, tpHead->entityid() );		
}

// ***********************************************************
//  Function:		OnMessageGetAllFamilyInfo
//  Description:	获取所有家族的基本信息
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageGetAllFamilyInfo( int vServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageGetAllFanmilyInfoRequest *tpMsg = ( CMessageGetAllFanmilyInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );	
	 
	LK_ASSERT( tpMsg->has_headmsg(), return ) 	
	FamilyHead *tpHead = tpMsg->mutable_headmsg();	 
	
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( pGatePlayer == NULL )
	{
		LOG_ERROR("family","can not find player who has roleid:%d", tpHead->roleid() );
		return;
	}	
	int tTotalPage = 0;
	CMessageGetAllFamilyInfoResponse vFamilyList;	
	CFamilyManager::GetSingleton().OnEventListFamily( pGatePlayer->mCharID, pGatePlayer->mCountryID, *vFamilyList.mutable_familyinfo(), tpMsg->curpage(), tTotalPage );
	SendResponseGetAllFamilyInfo( vServerID, pGatePlayer->mCharID, vFamilyList, tpMsg->curpage(), tTotalPage, tpHead->entityid() );		
}

// ***********************************************************
//  Function:		OnMessageGetApplyList
//  Description:	获取家族的申请列表
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageGetApplyList( int vServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageGetApplyListRequest *tpMsg = ( CMessageGetApplyListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg(), return ) 	
	tpHead = tpMsg->mutable_headmsg();	 
	
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( pGatePlayer == NULL )
	{	
		LOG_ERROR("family","can not find player who has roleid:%d", tpHead->roleid() );
		return;
	}
		
	CMessageGetApplyListResponse ApplyList;																   
	int tRet = CFamilyManager::GetSingleton().OnEventListAllApplies( pGatePlayer->mCharID, *ApplyList.mutable_applylist() );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseGetApplyList( vServerID, pGatePlayer->mCharID, ApplyList, tpHead->entityid() );
}

// ***********************************************************
//  Function:		OnMessageGetFamilyMemberInfo
//  Description:	获取家族成员的基本信息
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageGetFamilyMemberInfo( int vServerID, CMessage *pMessage )
{	 
	LK_ASSERT( pMessage != NULL, return );
	CMessageGetFamilyMemberInfoRequest *tpMsg = ( CMessageGetFamilyMemberInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg(), return ) 	
	tpHead = tpMsg->mutable_headmsg();	 
	
	CMessage tMsgHead;
	CMessageGetFamilyMemberInfoResponse tMsgGetFamilyMemberInfo;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETFAMILYMEMBERINFO );
	tMsgHead.set_msgpara( (int)&tMsgGetFamilyMemberInfo );
	tMsgGetFamilyMemberInfo.mutable_headmsg()->set_roleid( tpHead->roleid() );
	tMsgGetFamilyMemberInfo.mutable_headmsg()->set_entityid( tpHead->entityid() );
	
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR("family", "player of roleid:(%d) has no family when get family info ", tpHead->roleid() );
		return;
	}		
	
	tpFamily->GetFamilyMemberInfoList( *tMsgGetFamilyMemberInfo.mutable_memberinfo() );	
	
	// 判断是否需要解散
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );			
}

// ***********************************************************
//  Function:		OnMessageGetFamilyPropetyInfo
//  Description:	获取家族的基本属性信息
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageGetFamilyPropetyInfo( int vServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageGetFamilyPropertyRequest *tpMsg = ( CMessageGetFamilyPropertyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg(), return ) 	
	tpHead = tpMsg->mutable_headmsg();	 

	
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( pGatePlayer == NULL )
	{
		LOG_ERROR( "family","can not find player who has roleid:%d", tpHead->roleid() );
		return;
	}
	
	PBFamilyPropery vFamilyData;	
	int tRet = CFamilyManager::GetSingleton().OnEventGetFamiyProperty( tpHead->roleid(), vFamilyData );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseFamilyProperty( vServerID, tpHead->roleid(), vFamilyData, tpHead->entityid() );
}

// ***********************************************************
//  Function:		OnMessageInviteMember
//  Description:	邀请玩家加入家族
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageInviteMember( int vServerID, CMessage *pMessage )
{
	LK_ASSERT(  pMessage != NULL, return );
	CMessageInviteMemberRequest *tpMsg = ( CMessageInviteMemberRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );	
	
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg(), return ) 	
	tpHead = tpMsg->mutable_headmsg();	 
	
	CGatePlayer *pSrcPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( pSrcPlayer == NULL )
	{
		LOG_ERROR( "family"," can not find player who has roleid:%d ", tpHead->roleid() );
		return;
	}
	
	CGatePlayer *pDesPlayer = NULL;		
	if ( tpMsg->has_rolename() )
	{			
		if ( tpMsg->rolename().length() >= NAME_LENGTH )
		{
			LOG_ERROR( "family", "wrong name:%s ", tpMsg->mutable_rolename()->c_str() );
			return;
		}
				
		pDesPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->rolename().c_str() );			
	}
	else
	{
		pDesPlayer =  CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );
	} 
	
	if ( pDesPlayer == NULL )
	{	
		SendResponseIviteMember( vServerID, pSrcPlayer->mCharID, NULL, ERROR_FAMILY_INVITE_OFFLINE, tpHead->entityid() );
		return;
	}	
	
	if ( pSrcPlayer->mCountryID != pDesPlayer->mCountryID )
	{
		SendResponseIviteMember( vServerID, pSrcPlayer->mCharID, NULL, ERROR_FAMILY_INVITE_INDIFFNATION, tpHead->entityid() );
		return;
	}
	
	if ( pDesPlayer->mLevel < LEVEL_JOINFAMILY )
	{
		SendResponseIviteMember( vServerID, pSrcPlayer->mCharID, pDesPlayer->mCharName, ERROR_FAMILY_INVITE_DESLOWLEVEL, tpHead->entityid() );
		return;
	}
	
	if ( pDesPlayer->CheckInviteListIfFull( ) )
	{
		SendResponseIviteMember( vServerID, pSrcPlayer->mCharID, pDesPlayer->mCharName, ERROR_FAMILY_INVITE_INVITELISTFULL, tpHead->entityid() );
		return;
	} 
	
	CFamily *pFamily  = CFamilyManager::GetSingleton().GetFamilyOfPlayer( pSrcPlayer->mCharID );
	if ( pFamily == NULL )
	{
		LOG_ERROR( "family","player of roleid:%d has no family info", tpHead->roleid() );
		return;
	}	
	
	// 检查玩家的申请列表里面是否已经存在家族的申请
	if ( pDesPlayer->CheckFamilyInvite( pFamily->GetID() ) )
	{
		SendResponseIviteMember( vServerID, pSrcPlayer->mCharID, pDesPlayer->mCharName, ERROR_FAMILY_INVITE_INVITED, tpHead->entityid() );
		return;
	}
	
	// 检查玩家的邀请列表是否已满
	if ( pDesPlayer->CheckInviteListIfFull() )
	{
		SendResponseIviteMember( vServerID, pSrcPlayer->mCharID, pDesPlayer->mCharName, ERROR_FAMILY_INVITE_INVITELISTFULL, tpHead->entityid() );
		return;
	}
		
	int tRet = CFamilyManager::GetSingleton().OnEventInvite2Family( tpHead->roleid(), pDesPlayer->mCharID ); 	
	if ( tRet == -1 )
	{
		return;	  
	}
	SendResponseIviteMember( vServerID, pSrcPlayer->mCharID, pDesPlayer->mCharName, tRet, tpHead->entityid() );
	if ( tRet == 0 )
	{ 		
		pDesPlayer->InsertFamilyInvite( pFamily->GetID() );
		const char *tpCorpsName = NULL;
		const char *tpUnionName	= NULL;
		int			tUnionID	= 0;
		if ( pFamily->GetCorpsID() != 0 )
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( pFamily->GetCorpsID() );
			if ( tpCorps == NULL )
			{
				LOG_ERROR( "family"," can not find corps by id:(%d) of family:(%d) ", pFamily->GetCorpsID(), pFamily->GetID() );
				return;
			}			
			tpCorpsName = tpCorps->GetCorpsName();		
			if ( tpCorps->GetUnionID() != 0 )
			{
				tUnionID = tpCorps->GetUnionID();
				CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tUnionID );
				if ( tpUnion != NULL )
				{
					tpUnionName = tpUnion->GetUnionName();
				}
			}			
			
		}
		int tNpcID = 0;
		if ( pFamily->GetNPCState() == STATE_OWN )
		{
			tNpcID = pFamily->GetNpcID();
		}
		SendNotityFamilyInvited( pDesPlayer, pDesPlayer->mCharID, pSrcPlayer->mCharID, pSrcPlayer->mCharName, 
			pFamily->GetID() , pFamily->GetName(), pFamily->GetMemberNum(), pFamily->GetGlory(), pFamily->GetPVPStarLevel(), tNpcID,
			pFamily->GetRooterName(), pFamily->GetLevel(), tpCorpsName, tUnionID, tpUnionName );	
	}
}

// ***********************************************************
//  Function:		OnMessageKickPlayer
//  Description:	踢出家族成员
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageKickPlayer( int vServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageKickPlayerRequest *tpMsg = ( CMessageKickPlayerRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg(), return ) 	
	tpHead = tpMsg->mutable_headmsg();	 
	
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( pGatePlayer == NULL )
	{
		LOG_ERROR( "family"," can not find player who has role_id:%d ", tpHead->roleid() );
		return;
	}
	
	int tRet = CFamilyManager::GetSingleton().OnEventFirePlayer( tpHead->roleid(), tpMsg->playercharid() );
	if ( tRet == -1 )
	{
		return;
	}
	CRoleGlobalInfo *pKickPlayer = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpMsg->playercharid() );
	if ( pKickPlayer == NULL )
	{
		LOG_ERROR( "family"," can not find player who has roleid:%d ", tpMsg->playercharid() );
		return;
	}
	
	if ( tRet == 0 )
	{
		// TODO:发送开除玩家的消息通知 包括被开除的玩家	
		int RoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 };
		int FamilyNum =	 MAX_FAMILYMEMBER_NUM;
		int tRet = CFamilyManager::GetSingleton().OnEventListFamilies( tpHead->roleid(), RoleIDList, FamilyNum, tpHead->roleid() );
		if ( tRet != -1 )
		{	
			// 添加：防止数组越界 			 
			if ( FamilyNum < MAX_FAMILYMEMBER_NUM )
			{
				RoleIDList[FamilyNum] = pKickPlayer->RoleID();
				++FamilyNum;
			}  				
					
			SendNotifyKickPlayer( pKickPlayer->RoleName(),pKickPlayer->RoleID(), RoleIDList, FamilyNum, pKickPlayer->RoleName(), pKickPlayer->RoleID(), pGatePlayer->mCharName, pGatePlayer->mCharID );	
		}
		pKickPlayer->SetLastTimeLeaveFamily( time(NULL) );
	}	
	SendResponseKickMember( vServerID, pGatePlayer->mCharID, tpMsg->playercharid(), pKickPlayer->RoleName(), tRet, tpHead->entityid() );		
}

// ***********************************************************
//  Function:		OnMessageLeaveFamily
//  Description:	玩家离开家族
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageLeaveFamily( int vServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageLeaveFamilyRequest *tpMsg = ( CMessageLeaveFamilyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg(), return ) 	
	tpHead = tpMsg->mutable_headmsg();	 
	
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( pGatePlayer == NULL )
	{
		LOG_ERROR( "family"," can not find player who has roleid:%d ", tpHead->roleid() );
		return;
	}	
	int RoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 };
	int FamilyNum = MAX_FAMILYMEMBER_NUM;
	CFamilyManager::GetSingleton().OnEventListFamilies( tpHead->roleid(), RoleIDList, FamilyNum, tpHead->roleid() );
	int tRet = CFamilyManager::GetSingleton().OnEventLeaveFromFamily( tpHead->roleid() ); 
	if ( tRet == -1 )
	{
		return;
	}		 			
	if ( tRet == SUCCESS )
	{ 		
		if( FamilyNum > 0 )
		{
			SendNotifyMemberLeaveFamily( pGatePlayer->mCharID, pGatePlayer->mCharName, RoleIDList, FamilyNum ); 	
		} 			
	}
	SendResponseLeaveFamily( vServerID, tpHead->roleid(), tRet, tpHead->entityid() );
}

// ***********************************************************
//  Function:		OnMessagePostAppoint
//  Description:	任免职位
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessagePostAppoint( int vServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )	
	CMessagePostAppointRequest *tpMsg = ( CMessagePostAppointRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	int tOldPost = 0;
	int tRet = CFamilyManager::GetSingleton().OnEventSetFamilyPost( tpHead->roleid(), tpMsg->playercharid(), tpMsg->actionkind( ), tOldPost );
	if ( tRet == -1 )
	{
		return;
	}											  	
	SendResponsePostAppoint( vServerID, tpHead->roleid(), tRet, tpMsg->playercharid(), tpMsg->actionkind(), tpHead->entityid() );	
	if ( tRet == SUCCESS )
	{
		CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
		if ( tpFamily == NULL )
		{
			LOG_ERROR( "family","can not find family of player:%d ", tpHead->roleid() );
			return;
		}

		CGatePlayer *tpManager = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
		if ( tpManager == NULL )
		{
			LOG_ERROR( "family"," can not find player who has roleid:%d ", tpHead->roleid() );
			return;
		}		   		

		CRoleGlobalInfo *tpMember = CServiceRoleIDName::GetSingleton( ).GetRoleInfoByRoleID( tpMsg->playercharid() );
		if ( tpMember == NULL )
		{
			LOG_ERROR( "family"," can not find player who has roleid:%d ", tpMsg->playercharid() );
			return;
		}	

		int tRoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 } ;
		int tNum = MAX_FAMILYMEMBER_NUM;
		tpFamily->GetFamilyMemberIDList( tRoleIDList, tNum );		
		//SendNotifySetFamilyPost( tRoleIDList, tNum, tpHead->roleid(), tpManager->mCharName, tpMsg->memberid(), tpMember->RoleName(), tOldPost, tpMsg->post()  );
		SendNotifyPostAppoint( tpMsg->actionkind(), tpMsg->playercharid(), tpManager->mCharName, tRoleIDList, tNum, tpMember->RoleName() );
	}
}

// ***********************************************************
//  Function:		OnMessageReplyFamilyInvite
//  Description:	答复家族邀请
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageReplyFamilyInvite( int vServerID, CMessage *pMessage )
{	 
	LK_ASSERT( pMessage != NULL, return );
	CMessageReplyFamilyInviteRequest *tpMsg = ( CMessageReplyFamilyInviteRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg(), return ) 	
	tpHead = tpMsg->mutable_headmsg();	
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( pGatePlayer == NULL )
	{	
		LOG_ERROR( "family"," can not find player who has roleid:%d  ", tpHead->roleid() );
		return;
	}		

	LK_ASSERT( tpMsg->result() <= FAMILY_REPLY_DELETE && tpMsg->result() >= 0, return )	 	
	
	// 检查玩家的邀请列表里面是否有该家族的邀请
	if ( !pGatePlayer->CheckFamilyInvite( tpMsg->familyid() ) )
	{
		return;
	}
	
	// 删除邀请列表里面该家族的邀请信息
	pGatePlayer->DeleteFamilyInvite( tpMsg->familyid() );		
	
	// 只是删除邀请信息
	if ( tpMsg->result() == FAMILY_REPLY_DELETE )
	{ 	
		return;
	}		

	// 获取邀请者的信息
	CRoleGlobalInfo *pInvitingRoleInfo =  CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpMsg->invitingroleid() );
	if ( pInvitingRoleInfo == NULL )
	{
		LOG_ERROR( "family", " can not find player who has roleid:%d ", tpHead->roleid() );
		return;
	}	
	
	// 检查离开家族的时间
	CRoleGlobalInfo *tpRoleGlobalInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpHead->roleid() );
	LK_ASSERT( tpRoleGlobalInfo != NULL, return )
	if ( time( NULL ) - tpRoleGlobalInfo->GetLastTimeLeaveFamily() < FAMILY_LEAVECORPS_TIMELENGTH )
	{
		if( tpMsg->result() == FAMILY_REPLY_REFUSE || tpMsg->result() == FAMILY_REPLY_IGNORE ) 
		{
			SendResponseReplyFamilyInvite( vServerID, tpHead->roleid(), SUCCESS, tpMsg->result(), pInvitingRoleInfo->RoleName(), tpMsg->familyid(), NULL, tpHead->entityid() );		
		}
		else
		{	
			// 离开家族的冷却时间未到
			SendResponseReplyFamilyInvite( vServerID, tpHead->roleid(), ERROR_FAMILY_APPLY_INFAMILYLIMIT, tpMsg->result(), pInvitingRoleInfo->RoleName(), tpMsg->familyid(), NULL, tpHead->entityid() );	
		}			
		return;
	}	
		
	// 玩家点击忽略
	if ( tpMsg->result() == FAMILY_REPLY_IGNORE )
	{
		SendResponseReplyFamilyInvite( vServerID, tpHead->roleid(), 0, tpMsg->result(), pInvitingRoleInfo->RoleName(), tpMsg->familyid(), NULL, tpHead->entityid() );
		return;
	}
	 		
	// 拒绝邀请
	if ( tpMsg->result() != 1 )
	{	
		CGatePlayer *pInvitingPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->invitingroleid() );	
		if ( pInvitingPlayer != NULL )
		{
			SendNotifyInviteResult( pInvitingPlayer, tpMsg->invitingroleid(), tpMsg->result(), pGatePlayer->mCharName );		
		}	
		SendResponseReplyFamilyInvite( vServerID, tpHead->roleid(), 0, tpMsg->result(), pInvitingRoleInfo->RoleName(), tpMsg->familyid(), NULL, tpHead->entityid() );	
		return;
	} 
	
	// 同意邀请		
	CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
	if ( pFamily == NULL )
	{
		SendResponseReplyFamilyInvite( vServerID, tpHead->roleid(), ERROR_FAMILY_REPLYINVITE_FAMILYNOTEXIST, tpMsg->result(), pInvitingRoleInfo->RoleName(), tpMsg->familyid(), NULL, tpHead->entityid() );
		return;
	}
			
	// 同意邀请
	int tRet = CFamilyManager::GetSingleton().OnEventAgreeInvite( tpMsg->invitingroleid(), tpHead->roleid() );
	if ( tRet == -1 )
	{
		return;
	}	
	
	CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton( ).GetRoleInfoByRoleID( tpHead->roleid() );
	if ( pRoleInfo == NULL )
	{
		LOG_ERROR( "family", "can not find player who has roleid:%d ", tpHead->roleid() );
		return;
	}	
	
	// 发送错误通知	
	// 如果有军团的话设置军团ID和军团名称
	int tCorpsID = 0;
	const char *tpCorpsName = NULL;	 
	int tCorpsHeader = 0;
	int tUnionID	= 0;
	const char *tpUnionName = NULL;	
		
	// 成功加入家族	
	int tInviteResult = tRet;	
	if ( tRet == SUCCESS )
	{			
		tInviteResult = 1;
		// 获取家族成员列表
		int tFamilyRoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 };
		int tFamilyMemberNum = MAX_FAMILYMEMBER_NUM;
		CFamilyManager::GetSingleton().OnEventListFamilies( pFamily, tFamilyRoleIDList, tFamilyMemberNum, tpHead->roleid() );
		if ( tFamilyMemberNum > 0 )
		{			
			SendNotifyFamilyMemberAdded( pRoleInfo->RoleID(), pGatePlayer->mLevel, pRoleInfo->GenderID(), pRoleInfo->FaceType(), pRoleInfo->Metier(), pRoleInfo->RoleName(), pInvitingRoleInfo->RoleName(), tFamilyRoleIDList, tFamilyMemberNum, 1, pGatePlayer->mMapID, pGatePlayer->mLineID );
		}	
		tInviteResult = 1;	
		
		// 获取军团信息
		if ( pFamily->GetCorpsID() != 0  )
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( pFamily->GetCorpsID() );
			if ( tpCorps != NULL )
			{						
				tCorpsID = tpCorps->GetCorpsID();
				tpCorpsName = tpCorps->GetCorpsName();
				tCorpsHeader = tpCorps->GetRooterID();
				if ( tpCorps->IsOwner() )
				{
					int tCountryID = CWorldManager::GetSingleton().GetCountryIDByMap( tpCorps->GetBattleCity() );
					if ( tCountryID != COUNTRY_NONE && pGatePlayer->TempID( ) != tCountryID )
					{
						CWorldManager::GetSingleton().ChangePeopleNum( pGatePlayer->CountryID( ), pGatePlayer->TempID( ), CHANGE_DEL );
						CWorldManager::GetSingleton().ChangePeopleNum( pGatePlayer->CountryID( ), tCountryID, CHANGE_ADD );
						CWorldManager::GetSingleton().ClearOfficialID( pGatePlayer );
						pGatePlayer->TempID( tCountryID );
						CWorldModule::GetSingleton().SendJoinCountryNotify( pGatePlayer->CharID(), tCountryID );
					}
				}				
			
				if ( tpCorps->GetUnionID() != 0 )
				{
					tUnionID = tpCorps->GetUnionID();
					CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tUnionID );
					if ( tpUnion != NULL )
					{
						tpUnionName = tpUnion->GetUnionName();
					}
				}			
				
			}
		}  
		
		// 记录信息
		LOG_NOTICE( "family","[%s,%d] player join family ( roleid:%d, familyid:%d ) ", __FUNCTION__,__LINE__, pRoleInfo->RoleID(), pFamily->GetID() );
		LogMemberJoin( ORGANLOGTYPE_FAMILY, pFamily->GetID(), tpMsg->invitingroleid(), pRoleInfo->RoleID(), 0, pFamily->GetMemberNum() )
	}
	
	// 发送答复家族邀请的回复消息
	SendResponseReplyFamilyInvite( vServerID, tpHead->roleid(), tRet, tpMsg->result(), pInvitingRoleInfo->RoleName(), tpMsg->familyid(), pFamily->GetName(), tpHead->entityid(), tCorpsID, tpCorpsName, pFamily->GetRooter(), tCorpsHeader, tUnionID, tpUnionName );	
	
	// 发送给邀请人的结果通知
	CGatePlayer *pInvitingGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->invitingroleid() );	
	if ( pInvitingGatePlayer != NULL )
	{
		SendNotifyInviteResult( pInvitingGatePlayer, tpMsg->invitingroleid(), tInviteResult, pRoleInfo->RoleName() );		
	}	
}

// ***********************************************************
//  Function:		OnMessageApplyFamily
//  Description:	申请加入家族
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageApplyFamily( int vServerID, CMessage *pMessage )

{
	LK_ASSERT( pMessage != NULL, return );
	CMessageApplyFamilyRequest *tpMsg = ( CMessageApplyFamilyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg(), return )
	tpHead = tpMsg->mutable_headmsg();	 
	
	// 如果玩家不在线，则操作取消
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( pGatePlayer == NULL )
	{
		return;
	}	
	
	// 检查离开家族的时间
	CRoleGlobalInfo *tpRoleGlobalInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpHead->roleid() );
	LK_ASSERT( tpRoleGlobalInfo != NULL, return )
	if ( time( NULL ) - tpRoleGlobalInfo->GetLastTimeLeaveFamily() < FAMILY_LEAVECORPS_TIMELENGTH )
	{
		// 错误提示
		SendResponseFamilyApply( vServerID, tpHead->roleid( ), ERROR_FAMILY_APPLY_INFAMILYLIMIT, tpMsg->familyid(), tpMsg->has_familyname() ? tpMsg->mutable_familyname()->c_str():NULL, tpHead->entityid() );	
		return;
	}

	
	CFamily *pFamily = NULL;
	int  tRet = 0;
	int  nFamilyID = 0;
	const char *  pFamilyName = NULL; 
	if( tpMsg->has_familyid() )
	{			
		pFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
		nFamilyID  = tpMsg->familyid();
	}
	else if( tpMsg->has_familyname() )
	{			
		pFamilyName =   tpMsg->familyname().c_str();
		pFamily = CFamilyManager::GetSingleton().GetFamilyByName( pFamilyName );
	}
	else
	{
		pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpMsg->charid() );
	}
	
	if ( pFamily == NULL )
	{
		// 错误提示
		SendResponseFamilyApply( vServerID, tpHead->roleid( ), ERROR_FAMILY_APPLY_FAMILYNOTEXIST, nFamilyID, pFamilyName, tpHead->entityid() );	
		return;
	} 
	
	if ( pFamily->GetCountryID() != pGatePlayer->mCountryID )
	{
		SendResponseFamilyApply( vServerID, tpHead->roleid( ), ERROR_FAMILY_APPLY_NOTINSAMECOUNTRY, nFamilyID, pFamilyName, tpHead->entityid() );	
		return;			
	}  
		
	nFamilyID = pFamily->GetID();
	pFamilyName = pFamily->GetName();  	

	// 申请检查
	tRet = CFamilyManager::GetSingleton().OnEventSubmitApply( tpHead->roleid(), pFamily );
	if ( tRet == -1 )
	{
		return;
	}	
	
	// 错误提示
	SendResponseFamilyApply( vServerID, tpHead->roleid( ), tRet, nFamilyID, pFamilyName, tpHead->entityid() );	
		
	// 可以申请加入家族
	if ( tRet == 0 )
	{		
		// 给族长发去申请请求		
		//CGatePlayer *pRooter = CPlayerManager::GetSingleton().ScenePlayer( pFamily->GetRooter() );
		//// 族长不在线
		//if ( pRooter == NULL )
		//{
		//	return;			
		//}
		
		// 给家族中有权限的人发去申请信息
		CGatePlayer *pManager = CFamilyManager::GetSingleton().GetManagerOfRight( pFamily, EM_APPROVE_APPLY );
		if ( pManager != NULL )
		{
			// 族长在线
			SendNotifyMemberApply( pManager, pManager->mCharID, pGatePlayer->mCharID, pGatePlayer->mLevel, pGatePlayer->mMetierID, pGatePlayer->mCharName, pGatePlayer->mFaceType, pGatePlayer->mSexID, pGatePlayer->mMapID, pGatePlayer->mLineID );
		}		
		return;  
	}	  	
}

// ***********************************************************
//  Function:		OnMessageReplyMemberApply
//  Description:	答复玩家申请
//  Output:			
//  Date:	04/29/2009 
// **********************************************************
void CServiceFamily::OnMessageReplyMemberApply( int vServerID, CMessage *pMessage )
{	 
	LK_ASSERT( pMessage != NULL, return );
	CMessageReplyMemberApplyRequest *tpMsg = ( CMessageReplyMemberApplyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );	
	
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg(), return )
	tpHead = tpMsg->mutable_headmsg();		 
	LK_ASSERT( tpMsg->result() <=2 && tpMsg->result() >= 0, return )
			   
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	LK_ASSERT( pGatePlayer != NULL, return ); 	
	
	// 发送答复的回复消息
	CRoleGlobalInfo *tpGlobalInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpMsg->roleid() );

	// 如果玩家已经删除角色,则删除角色的申请信息
	if ( tpGlobalInfo == NULL )
	{
		CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpMsg->roleid() );
		if ( pFamily != NULL )
		{
			pFamily->DeletApply( tpMsg->roleid() );
		}
		SendResponseReplyMemberApply( vServerID, tpHead->roleid(), ERROR_FAMILY_REPLYAPPLY_ROLENOTEXIST, tpMsg->result(), tpMsg->roleid(), NULL, tpHead->entityid() );
		return;
	}
	
	// 答复操作
	int tRet = CFamilyManager::GetSingleton().OnEventReplayApply( tpHead->roleid(), tpMsg->roleid(), tpMsg->result() );
	if ( tRet == -1 )
	{
		return;		
	}	 	

	SendResponseReplyMemberApply( vServerID, tpHead->roleid(), tRet, tpMsg->result(), tpMsg->roleid(), tpGlobalInfo->RoleName(), tpHead->entityid() );		
	
	// 给申请人发送申请结果
	CGatePlayer *pApplyPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );
	if ( tpMsg->result() != 2 && tRet == 0 )
	{
		if ( pApplyPlayer != NULL )
		{ 
			int tCorpsID = 0;
			int tCorpsHeaderID = 0;
			const char *tpCorpsName = NULL;
			int tUnionID = 0;
			const char *tpUnionName = NULL;
			CFamily *pFamily = CFamilyManager::GetSingleton( ).GetFamilyOfPlayer( tpHead->roleid() );
			if ( pFamily == NULL )
			{
				return;
			} 
			if ( pFamily->GetCorpsID() != 0 )
			{
				CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( pFamily->GetCorpsID() );
				if ( tpCorps != NULL )
				{
					tCorpsID = tpCorps->GetCorpsID();
					tpCorpsName = tpCorps->GetCorpsName();
					tCorpsHeaderID = tpCorps->GetRooterID();					
					if ( tpCorps->GetUnionID() != 0 )
					{
						tUnionID = tpCorps->GetUnionID();
						CUnion *tpUnion = CCorpsManager::GetSingleton().GetUnionByID( tUnionID );
						if ( tpUnion != NULL )
						{
							tpUnionName = tpUnion->GetUnionName();
						}
					}			
				}
			}
			int tApplyResult = tRet==0? tpMsg->result():tRet;
			SendNotifyFamilyApplySucess( pApplyPlayer, pApplyPlayer->mCharID, tApplyResult, pFamily->GetID(), pFamily->GetName(), tCorpsID, tpCorpsName, pFamily->GetRooter(), tCorpsHeaderID, tUnionID, tpUnionName );
		}
	} 	

	// 如果同意加入并且操作成功,发送有新玩家加入的家族通知		
	if ( tpMsg->result() == 1 && tRet == SUCCESS )
	{  	
		CFamily *pFamily = CFamilyManager::GetSingleton( ).GetFamilyOfPlayer( tpHead->roleid() );
		if ( pFamily == NULL )
		{
			return;
		} 
		if ( pFamily->GetCorpsID() != 0  )
		{
			CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( pFamily->GetCorpsID() );
			if ( tpCorps != NULL )
			{
				if ( tpCorps->IsOwner() )
				{
					int tCountryID = CWorldManager::GetSingleton().GetCountryIDByMap( tpCorps->GetBattleCity() );
					if ( tCountryID != COUNTRY_NONE && pApplyPlayer != NULL && pApplyPlayer->TempID( ) != tCountryID )
					{
						CWorldManager::GetSingleton().ChangePeopleNum( pApplyPlayer->CountryID( ), pApplyPlayer->TempID( ), CHANGE_DEL );
						CWorldManager::GetSingleton().ChangePeopleNum( pApplyPlayer->CountryID( ), tCountryID, CHANGE_ADD );
						CWorldManager::GetSingleton().ClearOfficialID( pApplyPlayer );
						pApplyPlayer->TempID( tCountryID );
						CWorldModule::GetSingleton().SendJoinCountryNotify( pApplyPlayer->CharID(), tCountryID );
					}
				}
			}
		} 
		int FamilyRoleList[MAX_FAMILYMEMBER_NUM] = { 0 };
		int FamilyMeberNum = MAX_FAMILYMEMBER_NUM;
		CFamilyManager::GetSingleton().OnEventListFamilies( pFamily, FamilyRoleList, FamilyMeberNum, tpMsg->roleid() );
		if ( FamilyMeberNum > 0 )
		{
			const char *pName = NULL;
			CRoleGlobalInfo *pRoleInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( tpMsg->roleid() );
			if ( pRoleInfo == NULL )
			{
				LOG_ERROR( "family"," can not find player who has roleid:%d ", tpHead->roleid() );
				return;
			}
						
			int tonline = 0;
			int	tMapID = 0;
			int tLineID = 0;
			if ( pApplyPlayer != NULL )
			{						
				tonline = 1;
				pName = pApplyPlayer->mCharName;
				tMapID = pApplyPlayer->mMapID;
				tLineID = pApplyPlayer->mLineID;
			}
			else
			{ 				
				pName = pRoleInfo->RoleName();
			}		

			SendNotifyFamilyMemberAdded( pRoleInfo->RoleID(), pRoleInfo->Level(), pRoleInfo->GenderID(), pRoleInfo->FaceType(), pRoleInfo->Metier(), pName, pGatePlayer->mCharName, FamilyRoleList, FamilyMeberNum, tonline, tMapID, tLineID );  
			LOG_NOTICE( "family","[%s,%d] player join family( roleid:%d, familyid:%d ) " , __FUNCTION__,__LINE__, pRoleInfo->RoleID(), pFamily->GetID() );
			LogMemberJoin( ORGANLOGTYPE_FAMILY, pFamily->GetID(), tpHead->roleid(), pRoleInfo->RoleID(), 1, pFamily->GetMemberNum() )
		}
	}	
}

// ***********************************************************
//  Function:		OnMessageAppointHeader
//  Description:	转让宗正职位
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageAppointHeader( int vServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageAppointHeaderRequest *tpMsg = ( CMessageAppointHeaderRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg(), return ) 	
	tpHead = tpMsg->mutable_headmsg();	 
	
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	LK_ASSERT( pGatePlayer != NULL, return );
	
	CRoleGlobalInfo *pRoleinfo = CServiceRoleIDName::GetSingleton( ).GetRoleInfoByRoleID( tpMsg->playercharid() );
	if ( pRoleinfo == NULL )
	{
		LOG_ERROR( "family"," can not find player who has roleid:%d ", tpMsg->playercharid() );
		return;
	}
	
	CGatePlayer *pMember = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->playercharid() );
	if ( pMember == NULL )
	{						
		SendResponseAppointHeader( vServerID, pGatePlayer->mCharID, tpMsg->playercharid(),pRoleinfo->RoleName(), ERROR_FAMILY_APPOINTHEADER_MEMBEROFFLINE, tpHead->entityid() );	
		return;
	}
	
	if ( pMember->mLevel < LEVEL_FAMILYHEADER )
	{
		SendResponseAppointHeader( vServerID, pGatePlayer->mCharID, tpMsg->playercharid(),pRoleinfo->RoleName(), ERROR_FAMILY_APPOINTHEADER_MEMBERLEVELLOW, tpHead->entityid() );	
		return;
	}
	
	int tRet = CFamilyManager::GetSingleton().OnEventARoleAlienate( tpHead->roleid(), tpMsg->playercharid() );
	if ( tRet == -1 )
	{
		return;
	} 	
	if ( tRet == 0 )
	{
		// TODO:发给所有的成员族长转换的消息
		int RoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 };
		int MemberNum = MAX_FAMILYMEMBER_NUM;
		CFamilyManager::GetSingleton().OnEventListFamilies( pGatePlayer->mCharID, RoleIDList, MemberNum, tpHead->roleid() );
		if ( MemberNum > 0 )
		{  			
			SendNotifyChangeHeader( pGatePlayer->mCharID, pGatePlayer->mCharName, tpMsg->playercharid(),  pRoleinfo->RoleName(), RoleIDList, MemberNum );
		}			
	}
	SendResponseAppointHeader( vServerID, pGatePlayer->mCharID, tpMsg->playercharid(),pRoleinfo->RoleName(), tRet, tpHead->entityid() );		
}

// ***********************************************************
//  Function:		OnMessageChangeFamilyNotice
//  Description:	改变家族公告
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageChangeFamilyNotice( int vServerID, CMessage *pMessage )
{	
	LK_ASSERT( pMessage != NULL, return );
	CMessageChangeFamilyNoticeRequest *tpMsg = ( CMessageChangeFamilyNoticeRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	FamilyHead *tpHead = NULL;
	LK_ASSERT( tpMsg->has_headmsg(), return ) 	
	tpHead = tpMsg->mutable_headmsg();	 
	
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( pGatePlayer == NULL )
	{
		LOG_ERROR( "family"," can not find player who has roleid:(%d) ", tpHead->roleid() );
		return;
	}	
	
	if ( tpMsg->notice().length() >= MAX_FAMILYNOTICE_LENGTH*MAX_UTF8_BYTES )
	{
		LOG_ERROR( "family","notice too long when player of roleid:(%d) set it", tpHead->roleid() );
		return;
	}
	int tRet = CFamilyManager::GetSingleton().OnEventChangeFamilyBulletin( tpHead->roleid(), tpMsg->notice().c_str() );
	if ( tRet == -1 )
	{
		return;
	}
	// 修改成功
	if ( tRet == 0 )
	{
		// TODO:发送公告改变的通知
		int tRoleIDList[MAX_FAMILYMEMBER_NUM];
		int tMemberNum = MAX_FAMILYMEMBER_NUM;
		CFamilyManager::GetSingleton().OnEventListFamilies( pGatePlayer->mCharID, tRoleIDList, tMemberNum, tpHead->roleid() );
		if ( tMemberNum > 0 )
		{
			SendNotifyChangeFamilyNotice( tpMsg->notice().c_str(), tRoleIDList, tMemberNum );
		}			
	}
	SendResponseChangeNotice( vServerID, pGatePlayer->mCharID, tRet, tpMsg->notice().c_str(), tpHead->entityid() );
}

// ***********************************************************
//  Function:		OnMessageChangeFamilyNotice
//  Description:	改变家族公告
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::OnMessageClearFamilyApply( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageClearFamilyApplyRequest *tpMsg = ( CMessageClearFamilyApplyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();
	LK_ASSERT( tpHead != NULL, return )	
	int tRet = CFamilyManager::GetSingleton().OnEventClearApplyList( tpHead->roleid() );
	if ( tRet == -1 )
	{
		return;
	} 
	SendResponseClearFamilyApply( nServerID, tRet, tpHead->roleid(), tpHead->entityid() );
}

// ***********************************************************
//  Function:		OnMessageCancelFamilyDisband
//  Description:	取消家族解散的消息请求
//  Output:			
//  Date:	06/18/2009
// **********************************************************
void CServiceFamily::OnMessageCancelFamilyDisband( int nServerID, CMessage *pMessage )
{  
	LK_ASSERT( pMessage != NULL, return )		
	CMessageCancelFamilyDisbandRequest *tpMsg = ( CMessageCancelFamilyDisbandRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();
	LK_ASSERT( tpHead != NULL, return )						
	int tRet = CFamilyManager::GetSingleton().OnEventCancelFamilyDisband( tpHead->roleid() );
	if ( tRet == -1 )
	{
		return;
	}
	SendResponseCancelFamilyDisband( nServerID, tRet, tpHead->roleid(), tpHead->entityid() );
}

// ***********************************************************
//  Function:		OnMessageRefreshFamilyInfo
//  Description:	刷新家族信息的请求
//  Output:			
//  Date:	06/23/2009
// **********************************************************
void CServiceFamily::OnMessageRefreshFamilyInfo( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )	
	CMessageRefreshFamilyInfoRequest *tpMsg = ( CMessageRefreshFamilyInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();
	
	// TODO:时限检查	
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family", "can not find family of player:(%d)", tpHead->roleid() );
		return;
	}
	
	PBFamilyMemberList tFamilyMemberList;
	tpFamily->GetFamilyMemberInfoList( tFamilyMemberList );
	SendResponseRefreshFamilyInfo( nServerID, tpHead->roleid(), tpHead->entityid(), tFamilyMemberList );
}

// ***********************************************************
//  Function:		OnMessageSetFamilyMemberPost
//  Description:	设置家族成员职位的消息请求
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CServiceFamily::OnMessageSetFamilyMemberPost( int nServerID, CMessage *pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )	
	CMessageSetFamilyPostRequest *tpMsg = ( CMessageSetFamilyPostRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();
	
	int tOldPost = 0;
	int tRet = CFamilyManager::GetSingleton().OnEventSetFamilyPost( tpHead->roleid(), tpMsg->memberid(), tpMsg->post(), tOldPost );
	if ( tRet == -1 )
	{
		return;
	}											  	
	SendResponseSetFamilyMemberPost( nServerID, tRet, tpMsg->memberid(), tpMsg->post(),  tpHead->roleid(), tpHead->entityid() );	
	if ( tRet == SUCCESS )
	{
		CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
		if ( tpFamily == NULL )
		{
			LOG_ERROR( "family","can not find family of player:%d ", tpHead->roleid() );
			return;
		}
		
		CGatePlayer *tpManager = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
		if ( tpManager == NULL )
		{
			LOG_ERROR( "family"," can not find player who has roleid:%d ", tpHead->roleid() );
			return;
		}		   		
			
		CRoleGlobalInfo *tpMember = CServiceRoleIDName::GetSingleton( ).GetRoleInfoByRoleID( tpMsg->memberid() );
		if ( tpMember == NULL )
		{
			LOG_ERROR( "family"," can not find player who has roleid:%d ", tpMsg->memberid() );
			return;
		}				
	
		int tRoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 } ;
		int tNum = MAX_FAMILYMEMBER_NUM;
		tpFamily->GetFamilyMemberIDList( tRoleIDList, tNum );		
		SendNotifySetFamilyPost( tRoleIDList, tNum, tpHead->roleid(), tpManager->mCharName, tpMsg->memberid(), tpMember->RoleName(), tOldPost, tpMsg->post()  );
	}
}

// ***********************************************************
//  Function:		OnMessageLevelUPFamily
//  Description:	升级家族的请求
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CServiceFamily::OnMessageLevelUPFamily( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )	
	CMessageLevelUPFamilyRequest *tpMsg = ( CMessageLevelUPFamilyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg( );	
	CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	LK_ASSERT( pGatePlayer != NULL, return )	
	
	
	int tLevel = 0;
	int tMoney = 0;
	int tStablity = 0;
	int tRet = CFamilyManager::GetSingleton( ).OnEventLevelUPFamily( tpHead->roleid(), tLevel, tMoney, tStablity );
	if ( tRet == -1 )
	{				
		return;
	}		
	
	SendResponseLevelUPFamily( nServerID, tRet, tLevel, tpHead->roleid(), tpHead->entityid() );
	if ( tRet == SUCCESS )
	{
		CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
		if ( tpFamily == NULL )
		{
			LOG_ERROR( "family","can not find family of player:%d", tpHead->roleid() );
			
			return;
		}
				 
		
		int tRoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 } ;
		int tNum = MAX_FAMILYMEMBER_NUM;
		tpFamily->GetFamilyMemberIDList( tRoleIDList, tNum ); 		
		int tMaxMemberNum = 0;
		CFamilyManager::GetSingleton().GetFamilyConfig()->GetMemberNumLimitByLevel( tLevel, tMaxMemberNum );
		int tMaxPresterNum = 0;
		CFamilyManager::GetSingleton().GetFamilyConfig()->GetPresbyterNumLimit( tLevel, tMaxPresterNum );
		int tLevelMoney = 0;
		int tLevelGlory	= 0;
		tpFamily->GetLevelCondition( tLevelMoney, tLevelGlory );
		int tReduceStability = 0;
		CFamilyManager::GetSingleton().GetFamilyConfig()->GetReduceStability( tpFamily->GetLevel(), tReduceStability );
		SendNotifyLevelUPFamily( tRoleIDList, tNum, tLevel,  tpFamily->GetMoney(), tpFamily->GetGlory(), tMaxMemberNum, tMaxPresterNum, tLevelMoney, tLevelGlory, tpFamily->GetStabilityDegree(), tReduceStability );
		if ( tMoney > 0 )
		{
			SendNotifyFamilyMoneyChanged( tpFamily->GetMoney(), tpHead->roleid(), tpFamily->GetID(), REASON_LEVELUP, -1*tMoney, pGatePlayer->mCharName );
		}
		
		// 如果安定度改变值大于0，发送安定度改变的通知
		if ( tStablity > 0 )
		{
			SendNotifyFamilyStabilityChanged( tpFamily->GetStabilityDegree(), tpHead->roleid(), tpFamily->GetID(), REASON_LEVELUP, tStablity, tpFamily->GetName() );
		}
		
		LogOrganLevelUP( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tpFamily->GetLevel(), tpFamily->GetLevel() - 1 );
		LogOrganMoneyByLevelUP( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tpFamily->GetMoney(), tpFamily->GetMoney() + tMoney );
	}
}

// ***********************************************************
//  Function:		OnMessageContributeFamilyMoney
//  Description:	捐献家族金钱的消息请求
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CServiceFamily::OnMessageContributeFamilyMoney( int nServerID, CMessage *pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
	CMessageContributeFamilyMoneyRequest *tpMsg = ( CMessageContributeFamilyMoneyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg( );	
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family of player:%d ", tpHead->roleid() );
		return;
	}

	int tOldContribute = tpFamily->GetMemberContribute( tpHead->roleid() );	
	int tOldFamilyMoney = tpFamily->GetFamilyMoney();
	int tRet = CFamilyManager::GetSingleton().OnEventContributeMoney( tpHead->roleid(), tpMsg->money() );
	if ( tRet == -1 )
	{				
		return;
	}
		
	SendResponseContributeFamilyMoney( nServerID, tRet, tpMsg->money(), tpHead->roleid(), tpHead->entityid() );
	if ( tRet == SUCCESS )
	{ 		
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
		if ( tpPlayer == NULL )
		{						
			LOG_ERROR( "family","can not find player who has roleid:%d", tpHead->roleid() );
			return;
		}
		
		CMessage tMsgHead;
		CMessageContributeFamilyMoneyNotify tMsgContributeMoney;
		
		tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CONTRIBUTEFAMILYMONEY );
		tMsgHead.set_msgpara( (int)&tMsgContributeMoney );
		tMsgContributeMoney.set_memberid( tpHead->roleid() );
		tMsgContributeMoney.set_membername( tpPlayer->mCharName );
		tMsgContributeMoney.set_familymoney( tpFamily->GetFamilyMoney() );
		tMsgContributeMoney.set_value( tpMsg->money() );
		SendFamilyMsgByID( tpFamily->GetID(), &tMsgHead );
		
		// 发送安定度改变的通知
		int tStability = CFamilyManager::GetSingleton().GetFamilyConfig()->GetStabilityConfig()->GetMoneyStability();
		if ( tStability > 0 )
		{		
			tStability *= tpMsg->money();
			tpFamily->AddStabilityDegree( tStability );
			SendNotifyFamilyStabilityChanged( tpFamily->GetStabilityDegree(), tpHead->roleid(), tpFamily->GetID(), REASON_CONTRIBUTE, tStability, tpPlayer->CharName() );
		}
		
		// 发送个人贡献度改变的通知
		int tNewContribute = tpFamily->GetMemberContribute( tpHead->roleid() );				
		SendNotifyFamilyContributeChanged( tNewContribute, tpHead->roleid(), tpFamily->GetID(), tNewContribute - tOldContribute );
		
		// 发送家族可以升级的通知
		if ( tpFamily->CanFamilyLevelUp() )
		{
			SendNotifyFamilyCanLevelUP( tpFamily->GetID() );
		}
		
		// 打印日志
		LOG_NOTICE( "family","[%s,%d] player of roleid:%d contribute money:%d to family:%d ",  __FUNCTION__,__LINE__, tpHead->roleid(), tpMsg->money(), tpFamily->GetID() );
		LogOrganContributeMoney( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tpFamily->GetFamilyMoney(), tOldFamilyMoney )
		LogOrganContributeByContribute( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tNewContribute, tOldContribute )
	}
}

// ***********************************************************
//  Function:		OnMessageObtainFamilyMoney
//  Description:	从占领npc处领取家族金钱的消息请求
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CServiceFamily::OnMessageObtainFamilyMoney( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )	
	CMessageObtainFamilyMoneyRequest *tpMsg = ( CMessageObtainFamilyMoneyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg( );	

	CFamily *pFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( pFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family of player:%d ", tpHead->roleid() );
		return;
	}
		
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if( tpPlayer == NULL ) 
	{
		LOG_ERROR( "family"," can not find player who has roleid:%d ", tpHead->roleid() );
		return;	
	}	
	
	CMessage tMsgHead;
	CMessageObtainFamilyMoneyResponse tMsgObtainFamilyMoney;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_OBTAINFAMILYMONEY );
	tMsgHead.set_msgpara( (int)&tMsgObtainFamilyMoney );
	tMsgObtainFamilyMoney.mutable_headmsg()->set_roleid( tpHead->roleid() );
	tMsgObtainFamilyMoney.mutable_headmsg()->set_entityid( tpHead->entityid() );
	
	// 权限判读
	if ( pFamily->GetRooter() != ( int )tpHead->roleid() )
	{
		// 判断有没有权限		
		if ( !pFamily->MemberHasRight( tpHead->roleid(), EM_WELFARE_OBTAIN ) )
		{
			tMsgObtainFamilyMoney.set_errcode( ERROR_FAMILY_HASNORIGHT );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead ); 
			return;	
		}  			
	}
		
	tMsgObtainFamilyMoney.set_errcode( SUCCESS );
	int tGlory = 0;
	int tMoney = 0;
	pFamily->GetFamilyWelfare( tpMsg->npctmpid(), tGlory, tMoney );
	if ( tGlory == 0 || tMoney == 0 )
	{
		tMsgObtainFamilyMoney.set_errcode( ERROR_FAMILY_GETNPCWELFARE_HASGETED );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead ); 
		return;
	}
	
	tMsgObtainFamilyMoney.set_glory( tGlory );
	tMsgObtainFamilyMoney.set_money( tMoney );	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead ); 
		
	
	// 增加家族金钱
	if ( tMoney > 0 )
	{
		int tOldMoney = pFamily->GetFamilyMoney(); 
		pFamily->AddFamilyMoney( tMoney );			
		
		LOG_NOTICE( "family"," [%s,%d] add family money by npcwelfare( managerid:%d, familyid:%d, money:%d ) ", __FUNCTION__,__LINE__, tpHead->roleid(), pFamily->GetID(), tMoney );
		LogOrganGetNpcWelfareMoney( ORGANLOGTYPE_FAMILY, pFamily->GetID(), tpHead->roleid(), pFamily->GetFamilyMoney(), tOldMoney, tpMsg->npctmpid() );
		// 发送家族金钱改变的通知
		SendNotifyFamilyMoneyChanged( pFamily->GetMoney(), tpHead->roleid(), pFamily->GetID(), REASON_NPCWELFARE, tMoney, tpPlayer->mCharName );		
		
	}
	
	// 增加家族荣耀
	if ( tGlory > 0 )
	{
		int tOldGlory = pFamily->GetGlory();
		pFamily->AddGlory( tGlory );		
				
		// 发送家族荣耀改变的通知
		SendNotifyFamilyGloryChanged( pFamily->GetGlory(), tpHead->roleid(), pFamily->GetID(), REASON_NPCWELFARE, tGlory, tpPlayer->mCharName );
		LogOrganGetNpcWelfareGlory( ORGANLOGTYPE_FAMILY, pFamily->GetID(), tpHead->roleid(), pFamily->GetGlory(), tOldGlory, tpMsg->npctmpid() );
	} 	
	
	if ( tMoney > 0 || tGlory > 0 )
	{
		if ( pFamily->CanFamilyLevelUp() )
		{
			SendNotifyFamilyCanLevelUP( pFamily->GetID() );
		}
	}
} 

// ***********************************************************
//  Function:		OnMessageGetExpCard
//  Description:	领取家族内丹的消息请求
//  Output:			
//  Date:	09/22/2009
// **********************************************************
void CServiceFamily::OnMessageGetExpCard( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )		
	CMessageGetExpCardRequest *tpMsg = ( CMessageGetExpCardRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	
	LK_ASSERT( tpMsg->has_headmsg(), return )	
	FamilyHead *tpHead = tpMsg->mutable_headmsg();
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find player who has roleid:%d ", tpHead->roleid() );
		return;
	}
	
	CMessage tMsgHead;
	CMessageGetExpCardResponse tMsgGetExpCard;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETEXPCARD );
	tMsgHead.set_msgpara( (int)&tMsgGetExpCard );
	tMsgGetExpCard.mutable_headmsg()->set_roleid( tpHead->roleid() );
	tMsgGetExpCard.mutable_headmsg()->set_entityid( tpHead->entityid() );	
		
	if ( tpFamily->GetRooter() != (int)tpHead->roleid() )
	{
		if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_ACTVITY_REDSTONE ) )
		{
			tMsgGetExpCard.set_errcode( ERROR_FAMILY_HASNORIGHT );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
			return;	
		}	
	}
		
	if ( tpFamily->GetFamilyActivityStatus() & FAMILY_STATUS_FAMILYEXPCARD )
	{
		tMsgGetExpCard.set_errcode( ERROR_FAMILY_GETEXPCARD_GETED );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
		return;
	}

	// 获取家族内丹ID
	int tExtraExp = 0;
	int tRedStoneID = CFamilyManager::GetSingleton().GetFamilyConfig()->GetFamilyRedstoneConfig()->GetFamilyRedstone( tpFamily->GetLevel(), tpFamily->GetStabilityDegree(), tExtraExp );
	tMsgGetExpCard.set_redstoneid( tRedStoneID );
	
	// 记录日志	
	LogOrganStartRedstone( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), 0, 0, 0  );
	
	tpFamily->SetFamilyActivityStatus( tpFamily->GetFamilyActivityStatus() | FAMILY_STATUS_FAMILYEXPCARD );
	tMsgGetExpCard.set_errcode( SUCCESS );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		OnMessageBuyFamilyItem
//  Description:	购买家族红包的消息请求
//  Output:			
//  Date:	11/03/2009
// **********************************************************
void CServiceFamily::OnMessageBuyFamilyItem( int nServerID, CMessage *pMessage )
{

	LK_ASSERT( pMessage != NULL, return )		
	CMessageBuyFamilyItemRequest *tpMsg = ( CMessageBuyFamilyItemRequest * )pMessage->msgpara();
	
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "family", "[%s,%d][%s] can not find player on gate by roleid:%d ", 
			__LK_FILE__, __LINE__,	__FUNCTION__, tpMsg->roleid() );
		return;
	}
	
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family","[%s,%d][%s] family of player (roleid:%d ) does not exist ", __LK_FILE__, __LINE__,	__FUNCTION__, tpMsg->roleid() );
		return;
	}
	
	// 权限检查
	if ( tpFamily->GetRooter() != (int)tpMsg->roleid() )
	{		  
		if ( !tpFamily->MemberHasRight( tpMsg->roleid(), EM_MONEY_REIN ) )
		{	  
			SendResponseBuyFamilyItem( nServerID, tpMsg->roleid(), ERROR_FAMILY_FAMILYGOODS_HASNORIGHT, tpMsg->itemid(), tpMsg->itemnum(), tpMsg->entityid() );
			return;
		}		
	}
	
	// npc模板
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpMsg->npctmpid() );
	if ( tpTmpNpc == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find npc template by id:%d", __FUNCTION__, __LINE__, tpMsg->npctmpid() );
		return;
	}		
	
	// 出售表模板	
	CTemplateSellTable *tpSellTable = ( CTemplateSellTable * )CDataStatic::SearchTpl( tpTmpNpc->mSellServiceID );
	if ( tpSellTable == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find selltable by id:%d, npcid:%d, roleid:%d ",__FUNCTION__,__LINE__, tpTmpNpc->mSellServiceID, tpMsg->npctmpid(), tpMsg->roleid() );
		return;
	}
	
	// 判断出售表类型
	if ( tpSellTable->mSellTableType != CTemplateSellTable::SELLTABLETYPE_FAMILYWAR && tpSellTable->mSellTableType != CTemplateSellTable::SELLTABLETYPE_FAMILYMONEYNEEDED )
	{
		LOG_ERROR( "family","[%s,%d] selltable is not used for family, selltableid:%d, npcid:%d, roleid:%d ",__FUNCTION__,__LINE__, tpSellTable->mTempID, tpMsg->roleid() );
		return;
	}
	
	// 如果是战争商店必须占有npc
	if ( tpSellTable->mSellTableType == CTemplateSellTable::SELLTABLETYPE_FAMILYWAR )
	{
		if ( tpFamily->GetNpcID() == 0 || tpFamily->GetBidMoney() != 0 )
		{
			SendResponseBuyFamilyItem( nServerID, tpMsg->roleid(), ERROR_FAMILY_FAMILYGOODS_HASNOWARNPC, tpMsg->itemid(), tpMsg->itemnum(), tpMsg->entityid() );
			return;
		}
	}
		
	CGoodSInfo tGoodInfo;
	int		   tGoodPrice;
	tpFamily->GetFamilyShop()->GetItemInfoInSellTable( tpTmpNpc->mSellServiceID, tpMsg->itemid(), tGoodInfo, tGoodPrice );	
	
	if( tGoodPrice == 0 || tGoodInfo.mItemNum == 0 ) 
	{
		LOG_ERROR( "family","[%s,%d] wrong item info, item number:%d, item goldprice:%d in template ",__FUNCTION__,__LINE__, tGoodInfo.mItemNum, tGoodPrice );
		return;
	}
	
	int tMoneyNeed = tpMsg->itemnum()*tGoodPrice;
	if ( tpFamily->CheckFamilyMoneyEnough( tMoneyNeed ) == false )
	{
		SendResponseBuyFamilyItem( nServerID, tpMsg->roleid(), ERROR_FAMILY_REDPAPER_MONEYNOTENOUGH, tpMsg->itemid(), tpMsg->itemnum(), tpMsg->entityid() );
		return;		
	}
	
	int tShopRet = tpFamily->GetFamilyShop()->BuyItem( tpSellTable->mTempID, tpMsg->itemid(), tpMsg->itemnum() );
	int tErrcode = -1;
	if ( tpFamily->GetShopErrcode( tShopRet, tErrcode ) == -1 )
	{
		return;
	}
	
	// 购买成功
	if ( tErrcode == SUCCESS )
	{
		// 扣钱
		int tOldMoney = tpFamily->GetFamilyMoney();
		tpFamily->ConsumeFamilyMoney( tMoneyNeed );
		SendNotifyFamilyMoneyChanged( tpFamily->GetMoney(), tpMsg->roleid(), tpFamily->GetID(), REASON_MAKEREDPAPER, -1*tMoneyNeed, tpPlayer->mCharName );
		LOG_NOTICE( "family","[%s,%d] buy family item ( roleid:%d, familyid:%d, money:%d ) ", __FUNCTION__,__LINE__, tpMsg->roleid(), tpFamily->GetID(), tpMsg->totalcost() );
		LogOrganBuyItem( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpMsg->roleid(), tpFamily->GetFamilyMoney(), tOldMoney );
		
		// 发送物品发生变化的通知
		tpFamily->GetFamilyShop()->GetWarShopItemInfo( tpSellTable->mTempID, tpMsg->itemid(), tGoodInfo, tGoodPrice );
		SendNotifyChangeFamilyShopItem( tpMsg->familyid(), tpMsg->npctmpid(), tGoodInfo.mItemID, tGoodInfo.mItemNum );
	}	
	
	// 发送物品数量的信息		
	SendResponseBuyFamilyItem( nServerID, tpMsg->roleid(), tErrcode, tpMsg->itemid(), tpMsg->itemnum(), tpMsg->entityid() );
}

// ***********************************************************
//  Function:		SendNotifyChangeHeader
//  Description:	发送宗正转让职位的回应消息
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendNotifyChangeHeader( int ManagerID, const char *pNanagerName, int MemberID, const char *pMemberName, int *pRoleIDList, int RoleNum )
{	
	CMessage MsgHead;
	CMessageChangeHeaderNotify MsgChangeHeader;
	
	MsgHead.set_msgpara( (int)&MsgChangeHeader );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGERHEADER );
	
	MsgChangeHeader.set_managerid( ManagerID );
	if ( pMemberName != NULL )
	{
		MsgChangeHeader.set_managername( pNanagerName );
	}
	
	MsgChangeHeader.set_playercharid( MemberID );
	if ( pMemberName != NULL )
	{
		MsgChangeHeader.set_playername( pMemberName );
	}	 
	
	MsgChangeHeader.mutable_headmsg()->set_roleid( 0 );		
	
	SendFamilyNotify( &MsgHead, pRoleIDList, RoleNum );
}

// ***********************************************************
//  Function:		SendNotifyDisbandFamily
//  Description:	发送宗正解散家族的回应消息
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendNotifyDisbandFamily( int FamilyID, int *pRoleIDList, int RoleNum )
{	 
	CMessage MsgHead;
	CMessageDisbandFamilyNotify MsgDisbandFamily;
	
	MsgHead.set_msgpara( (int)&MsgDisbandFamily );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_DISBANDFAMILY );
			
	MsgDisbandFamily.set_familyid( FamilyID );
	
	MsgDisbandFamily.mutable_headmsg()->set_roleid( 0 );
	SendFamilyNotify( &MsgHead, pRoleIDList, RoleNum );
}	 

// ***********************************************************
//  Function:		SendNotifyFamilyApplySucess
//  Description:	发送申请加入家族成功的通知
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendNotifyFamilyApplySucess( CGatePlayer *pPlayer, int RoleID, int Result, int FamilyID, const char *pFamilyName, int nCorpsID /* = 0 */, const char *pCorpsName /* = NULL */, int nFamilyHeader /* = 0 */, int nCorpsHeader /* = 0 */, int nUnionID /* = 0 */, const char *pUnionName /* = NULL */ )
{				
	LK_ASSERT( pPlayer != NULL, return )
	CMessage MsgHead;  
	CMessageApplyFamilySuccessNotify MsgApplySucess;
	
	MsgApplySucess.set_result( Result );
	MsgApplySucess.set_familyid( FamilyID );
	if ( pFamilyName != NULL )
	{
		MsgApplySucess.set_familyname( pFamilyName );
	}
	MsgApplySucess.set_corpsid( nCorpsID );
	if ( pCorpsName != NULL )
	{
		MsgApplySucess.set_corpsname( pCorpsName );
	}	
	MsgApplySucess.set_familyheader( nFamilyHeader );
	MsgApplySucess.set_corpsheader( nCorpsHeader );
	MsgApplySucess.set_unionid( nUnionID );
	if ( pUnionName != NULL )
	{
		MsgApplySucess.set_unionname( pUnionName );
	}
	
	MsgApplySucess.clear_headmsg();
	MsgApplySucess.mutable_headmsg()->set_msgid( ID_S2C_NOTIFY_FAMILYAPPLYSUCESS );
	MsgApplySucess.mutable_headmsg()->set_roleid( RoleID );		

	MsgHead.set_msgpara( (int)&MsgApplySucess );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYAPPLYSUCESS );
	
	CGateServer::GetSingleton().Send2Scene( pPlayer, &MsgHead );
}				  

// ***********************************************************
//  Function:		SendNotifyFamilyMemberAdded
//  Description:	发送家族有新成员加入的通知
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendNotifyFamilyMemberAdded( int MemberRoleID, int nLevel, int nSex, int nFaceID, int nMetierID, const char *pName, const char *pManagerName, int *pRoleIDList, int RoleNum, int nOnLine, int nMapID /* = 0 */, int nLineID /* = 0 */ )
{	
	CMessage MsgHead;
	CMessageFamilyMemberAddedNotify	MsgMemberAdded;
	
	MsgHead.set_msgpara( (int)&MsgMemberAdded );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYMEMBERADDED );
	
	if ( pManagerName != NULL )
	{
		MsgMemberAdded.set_managername( pManagerName );
	}	
	if ( pName != NULL )
	{
		MsgMemberAdded.mutable_roleinfo()->set_playername( pName );
	}
	MsgMemberAdded.mutable_roleinfo()->set_playercharid( MemberRoleID );
	MsgMemberAdded.mutable_roleinfo()->set_faceid( nFaceID );
	MsgMemberAdded.mutable_roleinfo()->set_metierid( nMetierID );
	MsgMemberAdded.mutable_roleinfo()->set_online( nOnLine );
	MsgMemberAdded.mutable_roleinfo()->set_lineid( nLineID );
	MsgMemberAdded.mutable_roleinfo()->set_mapid( nMapID );
	MsgMemberAdded.mutable_roleinfo()->set_rolesex( nSex );
	MsgMemberAdded.mutable_roleinfo()->set_playerlevel( nLevel );
	MsgMemberAdded.mutable_roleinfo()->set_playerpost( FAMILY_POST_MEMEBER );
	MsgMemberAdded.mutable_headmsg()->set_roleid( 0 );
	SendFamilyNotify( &MsgHead, pRoleIDList, RoleNum );
}

// ***********************************************************
//  Function:		SendNotifyFamilyProperty
//  Description:	发送家族属性信息的通知
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendNotifyFamilyProperty( int nFamilyID )
{  
	CFamily *tpFamily = CFamilyManager::GetSingleton( ).GetFamilyByID( nFamilyID );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family by ID:%d ", nFamilyID );
		return;
	}
	
	CMessage tMsgHead;
	CMessageFamilyPropertyNotify tMsgFamilyPropery;	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYPEROPERTY );
	tMsgHead.set_msgpara( (int)&tMsgFamilyPropery );								
	tpFamily->GetFamilyProperty( *tMsgFamilyPropery.mutable_property(), false );
		
	SendFamilyMsgByID( nFamilyID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyFamilyProperty
//  Description:	发送家族属性信息的通知
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseClearFamilyApply( int nServerID, int nErrcode, int nRoleID, int nEntityID /* = 0 */ )
{
	CMessage MsgHead;
	CMessageClearFamilyApplyResponse MsgClearApply;
	
	MsgClearApply.clear_headmsg();
	MsgClearApply.mutable_headmsg()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		MsgClearApply.mutable_headmsg()->set_entityid( nEntityID );
	}	
	MsgClearApply.set_errcode( nErrcode );
	
	MsgHead.set_msgpara( (int)&MsgClearApply );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CLEARFAMILYAPPLYLIST );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &MsgHead );
}

// ***********************************************************
//  Function:		SendNotifyJoinCorps
//  Description:	发送家族加入军团的消息通知
//  Output:			
//  Date:	06/17/2009
// **********************************************************
void CServiceFamily::SendNotifyJoinCorps( int nCorpsID, const char *pCorpsName, int *pRoleIDList, int nNum, int nJoinMethod /* = JOINCORPS_OTHERFAMILYCORPS */, int nUnionID /* = 0 */, const char *pUnionName /* = NULL */ )
{
	CMessage MsgHead;
	CMessageJoinCorpsNotify MsgJoinCorps;
	
	MsgJoinCorps.set_corpsid( nCorpsID );
	if ( pCorpsName != NULL )
	{
		MsgJoinCorps.set_corpsname( pCorpsName );
	} 
	MsgJoinCorps.set_unionid( nUnionID );
	if ( pUnionName != NULL )
	{
		MsgJoinCorps.set_unionname( pUnionName );
	}
	
	MsgJoinCorps.mutable_headmsg()->set_roleid( 0 );
	MsgJoinCorps.set_joinmethod( nJoinMethod );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_JOINCORPS );	
	MsgHead.set_msgpara( (int)&MsgJoinCorps );
	SendFamilyNotify( &MsgHead, pRoleIDList, nNum );
}

// ***********************************************************
//  Function:		SendNotifyJoinCorps
//  Description:	发送家族加入军团的消息通知
//  Output:			
//  Date:	06/18/2009
// **********************************************************
void CServiceFamily::SendResponseCancelFamilyDisband( int nServerID, int tErrcode, int nRoleID, int nEntityID /* = 0 */ )
{
	CMessage MsgHead;
	CMessageCancelFamilyDisbandResponse MsgCancelDisband;
	
	MsgCancelDisband.mutable_headmsg()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		MsgCancelDisband.mutable_headmsg()->set_entityid( nEntityID );
	}	
	MsgCancelDisband.set_errcode( tErrcode );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CANCELFAMILYDISBAND );
	MsgHead.set_msgpara( (int)&MsgCancelDisband );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &MsgHead );
}

// ***********************************************************
//  Function:		SendNotifyChangeFamilyNotice
//  Description:	发送公告改变的通知
//  Output:			
//  Date:	06/19/2009
// **********************************************************
void CServiceFamily::SendNotifyChangeFamilyNotice( const char *pNotice, int *pRoleIDList, int nRoleNum )
{
	CMessage tMsgHead;
	CMessageChangeFamilyNoticeNotify tMsgChangeNotice;
	
	tMsgChangeNotice.set_notice( pNotice );
	tMsgChangeNotice.mutable_headmsg()->set_roleid( 0 );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGEFAMILYNOTICE );
	tMsgHead.set_msgpara( (int)&tMsgChangeNotice );
	
	
	SendFamilyNotify( &tMsgHead, pRoleIDList, nRoleNum );
}

// ***********************************************************
//  Function:		SendResponseRefreshFamilyInfo
//  Description:	刷新家族成员信息的回应
//  Output:			
//  Date:	06/23/2009
// **********************************************************
void CServiceFamily::SendResponseRefreshFamilyInfo( int nServerID, int nRoleID, int nEntityID, PBFamilyMemberList &pbMemList )
{	
	CMessage tMsgHead;
	CMessageRefreshFamilyInfoResponse tMsgFamilyInfo;
	
	tMsgFamilyInfo.mutable_headmsg()->set_roleid( nRoleID );	
	if ( nEntityID != 0 )
	{
		tMsgFamilyInfo.mutable_headmsg()->set_entityid( nEntityID );
	}
	tMsgFamilyInfo.mutable_memberlist()->CopyFrom( pbMemList );
	
	tMsgHead.mutable_msghead()->set_messageid( ID_C2S_REQUEST_REFRESHFAMILYINFO );
	tMsgHead.set_msgpara( (int)&tMsgFamilyInfo );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendResponseSetFamilyMemberPost
//  Description:	设置家族成员职位的消息消息回应
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CServiceFamily::SendResponseSetFamilyMemberPost( int nServerID, int nErrcode, int nMemberID, int nPost, int nRoleID, int nEntityID /* = 0 */ )
{	
	CMessage tMsgHead;
	CMessageSetFamilyPostResponse tMsgSetPost;
	
	tMsgSetPost.mutable_headmsg()->set_roleid( nRoleID );	
	if ( nEntityID != 0 )
	{
		tMsgSetPost.mutable_headmsg()->set_entityid( nEntityID );
	}
	
	tMsgHead.mutable_msghead( )->set_messageid( ID_S2C_RESPONSE_SETFAMILYMEMBERPOST );
	tMsgHead.set_msgpara( (int)&tMsgSetPost );
	
	tMsgSetPost.set_errcode( nErrcode );
	tMsgSetPost.set_memberid( nMemberID );
	tMsgSetPost.set_post( nPost );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}	

// ***********************************************************
//  Function:		SendResponseLevelUPFamily
//  Description:	升级家族的回应
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CServiceFamily::SendResponseLevelUPFamily( int nServerID, int nErrcode, int nLevel, int nRoleID, int nEntityID /* = 0 */ )
{	
	CMessage tMsgHead;
	CMessageLevelUPFamilyResponse tMsgLevelUPFamily;
	
	tMsgLevelUPFamily.mutable_headmsg()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgLevelUPFamily.mutable_headmsg()->set_entityid( nEntityID );
	}
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_LEVELUPFAMILY );			
	tMsgHead.set_msgpara( (int)&tMsgLevelUPFamily );
	
	tMsgLevelUPFamily.set_errcode( nErrcode );
	tMsgLevelUPFamily.set_level( nLevel );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendResponseContributeFamilyMoney
//  Description:	捐献家族金钱的消息回应
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CServiceFamily::SendResponseContributeFamilyMoney( int nServerID, int nErrcode, int nMoney, int nRoleID, int nEntityID /* = 0 */ )
{ 
	CMessage tMsgHead;
	CMessageContributeFamilyMoneyResponse tMsgContributeMoney;	
	
	tMsgContributeMoney.mutable_headmsg( )->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgContributeMoney.mutable_headmsg( )->set_entityid( nEntityID );
	}
	
	tMsgHead.mutable_msghead( )->set_messageid( ID_S2C_RESPONSE_CONTRIBUTEFAMILYMONEY );
	tMsgHead.set_msgpara( (int)&tMsgContributeMoney );
	
	tMsgContributeMoney.set_errcode( nErrcode );
	tMsgContributeMoney.set_money( nMoney );
	
	CGateServer::GetSingleton( ).Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendResponseObtainFamilyMoney
//  Description:	从占领npc处领取家族金钱的消息回应
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CServiceFamily::SendResponseObtainFamilyMoney( int nServerID )
{
	
}

// ***********************************************************
//  Function:		SendNotifySetFamilyPost
//  Description:	设置家族职位的通知
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CServiceFamily::SendNotifySetFamilyPost( int *pIDList, int nNum, int nManagerID, const char *pManagerName, int nMemberID, const char *pMemberName, int nOldPost, int nNewPost )
{  
	if ( pIDList == NULL || nNum <= 0 )
	{
		return;
	}
		
	CMessage tMsgHead;
	CMessageSetFamilyPostNotify tMsgSetFamilyPost;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_SETFAMILYMEMBERPOST );
	tMsgHead.set_msgpara( (int)&tMsgSetFamilyPost );

	tMsgSetFamilyPost.set_managerid( nManagerID );
	if ( pManagerName != NULL )
	{
		tMsgSetFamilyPost.set_managername( pManagerName );
	} 	
	tMsgSetFamilyPost.set_memberid( nMemberID );
	if ( pMemberName != NULL )
	{
		tMsgSetFamilyPost.set_membername( pMemberName );
	}
	
	tMsgSetFamilyPost.set_originalpost( nOldPost );
	tMsgSetFamilyPost.set_newpost( nNewPost );
	
	SendFamilyNotify( &tMsgHead, pIDList, nNum );
}	

// ***********************************************************
//  Function:		SendNotifyLevelUPFamily
//  Description:	家族升级的通知
//  Output:			
//  Date:	08/25/2009
// **********************************************************	
void CServiceFamily::SendNotifyLevelUPFamily( int *pIDList, int nNum, int nLevel, int nMoney, int nGlory, int nMaxMemberNum, int nMaxPresterNum, int nLevelMoney, int nLevelGlory, int nStability, int nReduceStability )
{
	CMessage tMsgHead;
	CMessageLevelUPFamilyNotify tMsgLevelUPFamily;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_LEVELUPFAMILY );
	tMsgHead.set_msgpara( (int)&tMsgLevelUPFamily );
	tMsgLevelUPFamily.mutable_headmsg()->set_roleid( 0 );
	
	tMsgLevelUPFamily.set_level( nLevel );
	tMsgLevelUPFamily.set_glory( nGlory );
	tMsgLevelUPFamily.set_money( nMoney );
	tMsgLevelUPFamily.set_maxfamilymember( nMaxMemberNum );
	tMsgLevelUPFamily.set_maxpresternum( nMaxPresterNum );
	tMsgLevelUPFamily.set_levelmoney( nLevelMoney );
	tMsgLevelUPFamily.set_levelglory( nLevelGlory );
	tMsgLevelUPFamily.set_stability( nStability );
	tMsgLevelUPFamily.set_reducestability( nReduceStability );	
	SendFamilyNotify( &tMsgHead, pIDList, nNum );
}

// ***********************************************************
//  Function:		SendNotifyPVPStarLevelUP
//  Description:	pvp星级升级的通知
//  Output:			
//  Date:	08/25/2009
// **********************************************************
void CServiceFamily::SendNotifyPVPStarLevelUP( int *pIDList, int nNum, int nPvPLevel )
{	
	CMessage tMsgHead;
	CMessageFamilyPVPStarLevelUPNotify tMsgPVPStarLevelUP;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYPVPSTARLEVELUP );
	tMsgHead.set_msgpara( (int)&tMsgPVPStarLevelUP );
	tMsgPVPStarLevelUP.set_pvpstarlevel( nPvPLevel );
	
	SendFamilyNotify( &tMsgHead, pIDList, nNum );
}

// ***********************************************************
//  Function:		SendFamilyChatMsg
//  Description:	给家族成员发送消息
//  Output:			
//  Date:	06/25/2009
// **********************************************************
void CServiceFamily::SendFamilyMsg( int nRoleID, CMessage *pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )		
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( nRoleID );
	if ( tpFamily == NULL )
	{
		return;
	}
	pMessage->mutable_msghead()->set_dstfe( FE_FAMILY );
	int tRoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 };
	int tNum =   MAX_FAMILYMEMBER_NUM;
	tpFamily->GetFamilyMemberIDList( tRoleIDList, tNum );
	//std::vector<CGatePlayer* > vPlayerList;
	for ( int i = 0; i < tNum; ++i )
	{
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tRoleIDList[i] );
		if ( tpPlayer == NULL )
		{
			continue;
		}								
		//vPlayerList.push_back(tpPlayer);		
		pMessage->mutable_msghead()->set_dstid( tRoleIDList[i] );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, pMessage );
	}
	//CGateServer::GetSingletonPtr()->Send2Player( vPlayerList, pMessage );
}

// ***********************************************************
//  Function:		SendNotifyKickPlayer
//  Description:	发送踢出玩家的通知
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendNotifyKickPlayer( const char *pName, int RoleID, int *pRoleIDList, int RoleNum, const char * pKickedName, int nKickedRoleID, const char *pManageName, int nManageID )
{	
	CMessage				 MsgHead;
	CMessageKickPlayerNotify MsgKickPlayer;	  
		
	if ( pManageName != NULL )	
	{
		MsgKickPlayer.set_managername( pManageName );
	}	
	MsgKickPlayer.set_managercharid( nManageID );
	if ( pKickedName != NULL )
	{
		MsgKickPlayer.set_playername( pKickedName );
	}
	MsgKickPlayer.set_playercharid( nKickedRoleID );
			
	MsgHead.set_msgpara( (int)&MsgKickPlayer );
	MsgHead.mutable_msghead( )->set_messageid( ID_S2C_NOTIFY_KICKPLAYER );
	MsgKickPlayer.mutable_headmsg()->set_roleid( 0 );
	SendFamilyNotify( &MsgHead,  pRoleIDList, RoleNum );
}

// ***********************************************************
//  Function:		SendNotifyMemberApply
//  Description:	发送有玩家申请加入家族的通知
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendNotifyMemberApply( CGatePlayer *pPlayer, int RoleID, int ApplyID, int Level, int Metier, const char *pName, int nFaceType, int nSex, int nMapID, int nLineID )
{  
	LK_ASSERT( pPlayer != NULL, return )
	CMessage		MsgHead;
	CMessageMemberApplyNotify MemberApply;
	
	if ( pName != NULL )
	{
		MemberApply.set_playername( pName );
	}											
	MemberApply.set_roleid( ApplyID );
	MemberApply.set_rolelevel( Level );
	MemberApply.set_rolemetier( Metier );
	MemberApply.set_facetype( nFaceType );
	MemberApply.set_rolesex( nSex );
	MemberApply.set_lineid( nLineID );
	MemberApply.set_mapid( nMapID );
	
	MemberApply.clear_headmsg();
	MemberApply.mutable_headmsg()->set_msgid( ID_S2C_NOTIFY_MEMBERAPPLY );
	MemberApply.mutable_headmsg()->set_roleid( RoleID );
	
	MsgHead.set_msgpara( (int)&MemberApply );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MEMBERAPPLY );	
	
	CGateServer::GetSingleton().Send2Scene( pPlayer, &MsgHead );
}

// ***********************************************************
//  Function:		SendNotifyMemberLeaveFamily
//  Description:	发送成员离开家族的通知
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendNotifyMemberLeaveFamily( int vLeaverID, const char *pLeaverName, int *pRoleIDList, int RoleNum )
{	
	LK_ASSERT( pLeaverName != NULL, return );
	CMessage MsgHead;
	CMessageMemberLeaveFamilyNotify MsgMemberLeave;
	
	MsgHead.set_msgpara( (int)&MsgMemberLeave );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MEMBERLEAVEFAMILY );
	
	MsgMemberLeave.set_roleid( vLeaverID );
	if ( pLeaverName != NULL )
	{
		MsgMemberLeave.set_rolename( pLeaverName );
	}	
	
	MsgMemberLeave.mutable_headmsg()->set_roleid( 0 );
	SendFamilyNotify( &MsgHead, pRoleIDList, RoleNum );
}

// ***********************************************************
//  Function:		SendNotifyPostAppoint
//  Description:	发送职位任免的通知
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendNotifyPostAppoint( int ActionKind, int RoleID, const char *pManagerName, int *RoleIDList, int RoleNum, const char *pRoleName )
{		 
	CMessage MsgHead;
	CMessagePostAppointNoify MsgPostAppoint;
	
	MsgHead.set_msgpara( (int)&MsgPostAppoint );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_POSTAPPOINT );
	
	MsgPostAppoint.set_actionkind( ActionKind );
	if ( pManagerName != NULL )
	{
		MsgPostAppoint.set_managername( pManagerName );
	}	
	MsgPostAppoint.set_playercharid( RoleID );
	
	if ( pRoleName != NULL )
	{
		MsgPostAppoint.set_playername( pRoleName );
	}
	
	MsgPostAppoint.mutable_headmsg()->set_roleid( 0 );
	SendFamilyNotify( &MsgHead, RoleIDList, RoleNum );
}

// ***********************************************************
//  Function:		SendNotityFamilyInvited
//  Description:	发送被人申请加入家族的通知
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendNotityFamilyInvited( CGatePlayer *pPlayer, int RoleID, int InviteID, const char *InviteName, int InviteFamilyID, const char *InviteFamilyName, int nMemberNum, int nGlory, int nPvpStar, int nNpcID, const char *pRooterName, int nFamilyLevel /* = 1 */, const char *pCorpsName /* = NULL */, int nUionID /* = 0 */, const char *pUnionName /* = NULL */ )
{  
	LK_ASSERT( pPlayer != NULL, return )
	LK_ASSERT( InviteName != NULL && InviteFamilyName != NULL, return );
	CMessage MsgHead;	  
	CMessageFamilyInvitedNotify MsgFamilyInvite;		
	
	MsgFamilyInvite.set_roleid( InviteID );
	if ( InviteName != NULL )
	{
		MsgFamilyInvite.set_playername( InviteName );
	}
	
	MsgFamilyInvite.set_familyid( InviteFamilyID );
	if ( InviteFamilyName != NULL )
	{
		MsgFamilyInvite.set_familtyname( InviteFamilyName );	
	}
	
	if( pCorpsName != NULL )	
	{
		MsgFamilyInvite.set_corpsname( pCorpsName );
	}
	
	if ( pRooterName != NULL )
	{
		MsgFamilyInvite.set_familyheadername( pRooterName );
	}
	
	MsgFamilyInvite.set_unionid( nUionID );
	if ( pUnionName != NULL )
	{
		MsgFamilyInvite.set_unionname( pUnionName );
	}
	
	MsgFamilyInvite.set_familylevel( nFamilyLevel );
	MsgFamilyInvite.set_membernum( nMemberNum );
	MsgFamilyInvite.set_glory( nGlory );
	MsgFamilyInvite.set_starlevel( nPvpStar );
	MsgFamilyInvite.set_npcid( nNpcID );
	
	MsgFamilyInvite.clear_headmsg();
	MsgFamilyInvite.mutable_headmsg()->set_roleid( RoleID );
	MsgFamilyInvite.mutable_headmsg()->set_msgid( ID_S2C_NOTIY_FAMILYINVITED );
	
	MsgHead.set_msgpara( (int)&MsgFamilyInvite );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIY_FAMILYINVITED );		
	
	
	CGateServer::GetSingleton().Send2Scene( pPlayer, &MsgHead );
}

// ***********************************************************
//  Function:		SendResponseAppointHeader
//  Description:	发送转让宗正职位的回应消息
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseAppointHeader( int vServerID, int RoleID, int MemberID, const char *pRoleName, int Errcode, int EntityID /* = 0 */ )
{
	CMessage MsgHead;
	CMessageAppointHeaderResponse MsgAppointHeader;
	
	MsgAppointHeader.set_errcode( Errcode );
	MsgAppointHeader.set_playercharid( MemberID );   
	if ( pRoleName != NULL )
	{
		MsgAppointHeader.set_playername( pRoleName );
	}
	
	MsgAppointHeader.clear_headmsg();
	MsgAppointHeader.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_APPOINTHEADER );
	MsgAppointHeader.mutable_headmsg()->set_roleid( RoleID );
	if ( EntityID != 0 )
	{
		MsgAppointHeader.mutable_headmsg()->set_entityid( EntityID );
	}	
	
	MsgHead.set_msgpara( (int)&MsgAppointHeader );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_APPOINTHEADER );
	
	//MakeMessage( MsgHead, TranferMsg, RoleID, &MsgAppointHeader, ID_S2C_RESPONSE_APPOINTHEADER );
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
}

// ***********************************************************
//  Function:		SendResponseChangeNotice
//  Description:	发送改变家族通知的请求回应
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseChangeNotice( int vServerID, int RoleID, int Errcode, const char *pNotice, int EntityID /* = 0 */ )
{																						
	CMessage MsgHead;	
	CMessageChangeFamilyNoticeResponse MsgChangeNotice;
	
	MsgChangeNotice.set_errcode( Errcode );
	
	MsgChangeNotice.clear_headmsg();
	MsgChangeNotice.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_CHANGENOTICE );
	MsgChangeNotice.mutable_headmsg()->set_roleid( RoleID );
	if ( EntityID != 0 )
	{
		MsgChangeNotice.mutable_headmsg()->set_entityid( EntityID );
	}
	if ( pNotice != NULL )
	{
		MsgChangeNotice.set_notice( pNotice );
	}
	
	MsgHead.set_msgpara( (int)&MsgChangeNotice );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CHANGENOTICE );
	
	//MakeMessage( MsgHead, TranferMsg, RoleID, &MsgChangeNotice, ID_S2C_RESPONSE_CHANGENOTICE );
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
}

// ***********************************************************
//  Function:		SendResponseCreatFamily
//  Description:	答复玩家的申请
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseCreatFamily( int vServerID, int RoleID, int tRet, int FamilyID, const char *pFamilyName, int EntityID /* = 0 */ )
{	
	CMessage MsgHead;	
	CMessageCreatFamilyResponse MsgCreatFamily;
		
	MsgCreatFamily.set_errcode( tRet );	 	

	MsgCreatFamily.mutable_headmsg( )->set_msgid( ID_S2C_RESPONSE_CREATFAMILY );
	MsgCreatFamily.mutable_headmsg( )->set_roleid( RoleID );
	if ( EntityID != 0 )
	{
		MsgCreatFamily.mutable_headmsg()->set_entityid( EntityID ); 
	}	
	MsgCreatFamily.set_familyid( FamilyID )	;
	if ( pFamilyName != NULL )
	{
		MsgCreatFamily.set_familyname( pFamilyName );
	}
	
	pbmsg_sethead(&MsgHead, ID_S2C_RESPONSE_CREATFAMILY, 0, 0, 0, 0, 0, 0);
	MsgHead.set_msgpara( (int)&MsgCreatFamily );
	// MakeMessage( MsgHead, TransferMsg, RoleID, &MsgCreatFamily, ID_S2C_RESPONSE_CREATFAMILY );
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
}

// ***********************************************************
//  Function:		SendResponseDisbandFamily
//  Description:	宗正解散家族的回应
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseDisbandFamily( int vServerID, int RoleID, int Errcode, int EntityID /* = 0 */ )
{
	CMessage MsgHead;	
	CMessageDisbandFamilyResponse MsgDisbandFamily;
	
	MsgDisbandFamily.set_errcode( Errcode );
	
	MsgDisbandFamily.clear_headmsg();
	MsgDisbandFamily.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_DISBANDFAMILY );
	MsgDisbandFamily.mutable_headmsg()->set_roleid( RoleID );
	if ( EntityID != 0 )
	{
		MsgDisbandFamily.mutable_headmsg()->set_entityid( EntityID ); 
	}
	
	MsgHead.set_msgpara( (int)&MsgDisbandFamily );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_DISBANDFAMILY );
	// MakeMessage( MsgHead, TranferMsg, RoleID, &MsgDisbandFamily, ID_S2C_RESPONSE_DISBANDFAMILY );
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
}

// ***********************************************************
//  Function:		SendResponseFamilyApply
//  Description		发送申请加入家族的回应消息
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseFamilyApply( int vServerID, int RoleID, int Result, int FamilyID, const char * FamilyName, int EntityID /* = 0 */ )
{
	CMessage MsgHead;	
	CMessageApplyFamilyResponse MsgApplyFamiy;
	
	MsgApplyFamiy.set_errcode( Result );
	MsgApplyFamiy.set_familyid( FamilyID );
	if ( FamilyName != NULL )
	{
		MsgApplyFamiy.set_familyname( FamilyName );
	}
	
	MsgApplyFamiy.clear_headmsg();
	MsgApplyFamiy.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_FAMILYAPPLY );
	MsgApplyFamiy.mutable_headmsg()->set_roleid( RoleID );
	if ( EntityID != 0 )
	{
		MsgApplyFamiy.mutable_headmsg()->set_entityid( EntityID );
	}
	
	MsgHead.set_msgpara( (int)&MsgApplyFamiy );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_FAMILYAPPLY );
	// TODO:MakeMessage( MsgHead, TransferMsg, RoleID, &MsgApplyFamiy, ID_S2C_RESPONSE_FAMILYAPPLY );
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );					    
}

// ***********************************************************
//  Function:		SendResponseFamilyProperty
//  Description:	发送获取家族属性信息的回应消息
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseFamilyProperty( int vServerID, int RoleID, PBFamilyPropery &pbFamilData, int EntityID /* = 0 */ )
{
	CMessage MsgHead;	
	CMessageGetFamilyPropertyResponse MsgGetFamillyProperty;	
	
	MsgGetFamillyProperty.set_familylevel( pbFamilData.familylevel() ); 
	MsgGetFamillyProperty.set_familymoney( pbFamilData.familymoney() );
	if ( pbFamilData.has_familynotice() )
	{
		MsgGetFamillyProperty.set_familynotice( pbFamilData.familynotice().c_str() );
	}	
	
	MsgGetFamillyProperty.set_familynum( pbFamilData.membernum() );
	MsgGetFamillyProperty.set_familyheaderid( pbFamilData.familyheaderid() );
	MsgGetFamillyProperty.set_familyheadername( pbFamilData.familyheadername().c_str() );	
	
	MsgGetFamillyProperty.set_familyglory( pbFamilData.familyglory() );
	MsgGetFamillyProperty.set_npcid( pbFamilData.npcid() );			
	MsgGetFamillyProperty.set_bidmoney( pbFamilData.bidmoney() );
	MsgGetFamillyProperty.set_bidtime( pbFamilData.bidtime() );
	MsgGetFamillyProperty.set_warnpcstatus( pbFamilData.warnpcstatus() );	
	MsgGetFamillyProperty.set_warrank( pbFamilData.warrank() );
	MsgGetFamillyProperty.set_warwinnum( pbFamilData.warwinnum() );
	MsgGetFamillyProperty.set_warlostnum( pbFamilData.warlostnum() );	
	MsgGetFamillyProperty.set_familystarlevel( pbFamilData.familystarlevel() );
	MsgGetFamillyProperty.set_warexp( pbFamilData.warexp() );		
	MsgGetFamillyProperty.set_warmaxexp( pbFamilData.warmaxexp() );	
	MsgGetFamillyProperty.set_warstatus( pbFamilData.warstatus() );
	MsgGetFamillyProperty.set_enemyfamilyname( pbFamilData.enemyfamilyname() );
	
	MsgGetFamillyProperty.set_challengestatus( pbFamilData.challengestatus() );
	MsgGetFamillyProperty.set_challengemoney( pbFamilData.challengemoney() );
	MsgGetFamillyProperty.set_challengestarted( pbFamilData.mutable_challengestarted()->c_str() );
	MsgGetFamillyProperty.set_challengereceived( pbFamilData.mutable_challengereceived()->c_str() );
	MsgGetFamillyProperty.set_canlevelup( pbFamilData.canlevelup() );
	
	MsgGetFamillyProperty.clear_headmsg();
	MsgGetFamillyProperty.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_FAMILYPROPERTY );
	MsgGetFamillyProperty.mutable_headmsg()->set_roleid( RoleID );
	MsgGetFamillyProperty.set_maxfamilymembernum( pbFamilData.maxmembernum() );
	MsgGetFamillyProperty.set_maxpresternum( pbFamilData.maxpresbyternum() );
	MsgGetFamillyProperty.set_glorylimit( pbFamilData.glorylimit() );
	MsgGetFamillyProperty.set_levelmoney( pbFamilData.levelmoney() );
	MsgGetFamillyProperty.set_stability( pbFamilData.stability() );
	MsgGetFamillyProperty.set_reducestability( pbFamilData.reducestability() );
	MsgGetFamillyProperty.set_extraredstonexp( pbFamilData.extraredstonexp() );
	if ( EntityID != 0 )
	{
		MsgGetFamillyProperty.mutable_headmsg()->set_entityid( EntityID );	
	}																    
	
	MsgHead.set_msgpara( (int)&MsgGetFamillyProperty );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_FAMILYPROPERTY );	
	
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
}


// ***********************************************************
//  Function:		SendResponseGetAllFamilyInfo
//  Description:	发送获取所有家族基本信息的回应
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseGetAllFamilyInfo( int vServerID, int RoleID, CMessageGetAllFamilyInfoResponse &pbFamilyInfo, int nCurPage, int nTotalPage, int EntityID /* = 0 */ )
{			
	CMessage MsgHead;  		 	
	pbFamilyInfo.clear_headmsg();
	pbFamilyInfo.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_GETALLFAMILYINFO );
	pbFamilyInfo.mutable_headmsg()->set_roleid( RoleID );
	if ( EntityID != 0 )
	{
		pbFamilyInfo.mutable_headmsg()->set_entityid( EntityID );
	}
	pbFamilyInfo.set_curpage( nCurPage );
	pbFamilyInfo.set_totalpage( nTotalPage );
		
	MsgHead.set_msgpara( (int)&pbFamilyInfo );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETALLFAMILYINFO );	
	
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );			
}

// ***********************************************************
//  Function:		SendResponseGetApplyList
//  Description:	发送获取家族申请列表的回应
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseGetApplyList( int vServerID, int RoleID, CMessageGetApplyListResponse &pbApplyList, int EntityID /* = 0 */ )
{	 
	CMessage MsgHead;
	pbApplyList.mutable_headmsg( )->set_msgid( ID_S2C_RESPONSE_GETAPPLYLIST );
	pbApplyList.mutable_headmsg( )->set_roleid( RoleID );
	if ( EntityID != 0 )
	{
		pbApplyList.mutable_headmsg( )->set_entityid( EntityID );
	}
	
	MsgHead.set_msgpara( (int)&pbApplyList );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETAPPLYLIST );
	// MakeMessage( MsgHead, TranferMsg, RoleID, &pbApplyList, ID_S2C_RESPONSE_GETAPPLYLIST );
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
}

// ***********************************************************
//  Function:		SendResponseGetFamilyMemberInfo
//  Description:	发送获取家族成员信息的回应 
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseGetFamilyMemberInfo( int vServerID, int RoleID, int *PFamilyList, int nFamilyNum, int EntityID /* = 0 */ )
{	
	CMessage MsgHead;		
	CMessageGetFamilyMemberInfoResponse MsgGetFamilyInfo;
	PBFamilyMemberList *pMemberList = MsgGetFamilyInfo.mutable_memberinfo();	
	for ( int i = 0 ; i < nFamilyNum; ++i )
	{
		FamilyMemberInfo *pMemberData = pMemberList->add_members();	
		CRoleGlobalInfo *pMemberInfo = CServiceRoleIDName::GetSingleton().GetRoleInfoByRoleID( PFamilyList[i] );
		
		if ( pMemberInfo == NULL )
		{
			LOG_ERROR( "family","can not find player who has roleid:%d", PFamilyList[i] );
			continue;
		}
		pMemberData->set_playercharid( pMemberInfo->RoleID() );	 		
		pMemberData->set_playername( pMemberInfo->RoleName() );
		pMemberData->set_playerpost( 1 );
		pMemberData->set_faceid( pMemberInfo->FaceType() );
		pMemberData->set_metierid( pMemberInfo->Metier() );
		pMemberData->set_rolesex( pMemberInfo->GenderID() ) ;
		pMemberData->set_playerlevel( pMemberInfo->Level() );
		pMemberData->set_online( 0 );
		pMemberData->set_lineid( 0 );
		pMemberData->set_mapid( 0 ); 		
		CGatePlayer *pGatePlayer = CPlayerManager::GetSingleton( ).ScenePlayer( PFamilyList[i] );
		if ( pGatePlayer != NULL )
		{
			pMemberData->set_online( 1 );
			pMemberData->set_lineid( pGatePlayer->mLineID );
			pMemberData->set_mapid( pGatePlayer->mMapID );	
			pMemberData->set_playerlevel( pGatePlayer->mLevel );				
		}	
		pMemberData->set_familywar( CFamilyManager::GetSingletonPtr()->GetSubmit( PFamilyList[i] ) );
	}
	pMemberList->mutable_members( 0 )->set_playerpost( 0 );
	
	MsgGetFamilyInfo.clear_headmsg();
	MsgGetFamilyInfo.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_GETFAMILYMEMBERINFO );
	MsgGetFamilyInfo.mutable_headmsg()->set_roleid( RoleID );
	if ( EntityID != 0 )
	{
		MsgGetFamilyInfo.mutable_headmsg()->set_entityid( EntityID );
	}
	
	MsgHead.set_msgpara( (int)&MsgGetFamilyInfo );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETFAMILYMEMBERINFO );
		
	//MakeMessage( MsgHead, TranferMsg, RoleID, &MsgGetFamilyInfo, ID_S2C_RESPONSE_GETFAMILYMEMBERINFO );
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
}

// ***********************************************************
//  Function:		SendResponseIviteMember
//  Description:	发送邀请玩家加入家族的回应消息
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseIviteMember( int vServerID, int RoleID, const char *pName, int tRet, int EntityID /* = 0 */ )
{	
	CMessage MsgHead;
	CMessageInviteMemberResponse MsgInviteMember;
	
	MsgInviteMember.set_errcode( tRet );
	if ( pName != NULL )
	{
		MsgInviteMember.set_rolename( pName );
	}
	
	MsgInviteMember.clear_headmsg();
	MsgInviteMember.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_INVITEMEMBER );
	MsgInviteMember.mutable_headmsg()->set_roleid( RoleID );
	if ( EntityID != 0 )
	{					
		MsgInviteMember.mutable_headmsg()->set_entityid( EntityID );
	}
	
	MsgHead.set_msgpara( (int)&MsgInviteMember );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_INVITEMEMBER );
	
	// MakeMessage( MsgHead, TranferMsg, RoleID, &MsgInviteMember, ID_S2C_RESPONSE_INVITEMEMBER );
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
}

// ***********************************************************
//  Function:		SendResponseKickMember
//  Description:	发送踢出玩家的回应消息
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseKickMember( int vServerID, int RoleID, int KickedPlayerID, const char *pKickedName, int Errcode, int EntityID /* = 0 */ )
{
	CMessage MsgHead;
	CMessageKickPlayerResponse  MsgKickPlayer;
	
	MsgKickPlayer.set_errcode( Errcode );
	MsgKickPlayer.set_playercharid( KickedPlayerID );
	if ( pKickedName != NULL )
	{
		MsgKickPlayer.set_playername( pKickedName );	
	}
	
	MsgKickPlayer.clear_headmsg();
	MsgKickPlayer.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_KICKMEMBER );
	MsgKickPlayer.mutable_headmsg()->set_roleid( RoleID );
	if ( EntityID != 0 )
	{
		MsgKickPlayer.mutable_headmsg()->set_entityid( EntityID );
	}
	
	MsgHead.set_msgpara( (int)&MsgKickPlayer );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_KICKMEMBER );
	// MakeMessage( MsgHead, TranferMsg, RoleID, &MsgKickPlayer, ID_S2C_RESPONSE_KICKMEMBER );
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
}

// ***********************************************************
//  Function:		SendResponseLeaveFamily
//  Description:	玩家离开请求离开家族的回应消息
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponseLeaveFamily( int vServerID, int RoleID, int Errcode, int EntityID /* = 0 */ )
{
	CMessage MsgHead;	
	CMessageLeaveFamilyResponse MsgLeaveFamily;	 
		
	MsgLeaveFamily.set_errcode( Errcode );	
	MsgLeaveFamily.clear_headmsg( );
	MsgLeaveFamily.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_LEAVEFAMILY );
	MsgLeaveFamily.mutable_headmsg()->set_roleid( RoleID );
	if ( EntityID != 0 )
	{
		MsgLeaveFamily.mutable_headmsg()->set_entityid( EntityID );
	}
	
	MsgHead.set_msgpara( (int)&MsgLeaveFamily );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_LEAVEFAMILY );	
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );	
}

// ***********************************************************
//  Function:		SendResponsePostAppoint
//  Description:	职位任免的回应消息
//  Output:			
//  Date:	04/22/2009
// **********************************************************
void CServiceFamily::SendResponsePostAppoint( int vServerID, int RoldID, int Errcode, int MemberID, int Post,int EntityID /* = 0 */ )
{
	CMessage					MsgHead;	
	CMessagePostAppointResponse MsgPostAppoint;	
	
	MsgPostAppoint.set_errcode( Errcode );
	MsgPostAppoint.set_actionkind( Post );
	MsgPostAppoint.set_playercharid( MemberID );
	
	MsgPostAppoint.clear_headmsg();
	MsgPostAppoint.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_POSTAPPOINT );
	MsgPostAppoint.mutable_headmsg()->set_roleid( RoldID );
	if ( EntityID != 0 )
	{
		MsgPostAppoint.mutable_headmsg()->set_entityid( EntityID );
	}
	
	MsgHead.set_msgpara( (int)&MsgPostAppoint );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_POSTAPPOINT );	
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
}

// ***********************************************************
//  Function:		SendResponseReplyFamilyInvite
//  Description:	答复家族邀请的回应消息(只有同意时才回复)
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::SendResponseReplyFamilyInvite( int vServerID, int RoleID, int nRet, int nResult, const char *pInvitingRoleName, int nFamilyID, const char *pFamilyName, int EntityID /* = 0  */, int nCorpsID /* = 0 */, const char *pCorpsName /* = NULL */, int nFamilyHeader /* = 0 */, int nCorpsHeader /* = 0 */, int nUnionID /* = 0 */, const char *pUnionName /* = NULL */ )
{
	CMessage MsgHead;	
	CMessageReplyFamilyInviteResponse ReplyFamilyInvite;
	
	ReplyFamilyInvite.set_errcode( nRet );
	ReplyFamilyInvite.set_result( nResult );
	ReplyFamilyInvite.set_familyid( nFamilyID );
	if ( pFamilyName != NULL )
	{
		ReplyFamilyInvite.set_familyname( pFamilyName );
	}
	ReplyFamilyInvite.set_corpsid( nCorpsID );
	if ( pCorpsName != NULL )
	{
		ReplyFamilyInvite.set_corpsname( pCorpsName );
	}
	ReplyFamilyInvite.set_unionid( nUnionID );
	if ( pUnionName != NULL )
	{
		ReplyFamilyInvite.set_unionname( pUnionName );
	}
	
	ReplyFamilyInvite.set_familyheaderid( nFamilyHeader );
	ReplyFamilyInvite.set_corpsheaderid( nCorpsHeader );  
	ReplyFamilyInvite.clear_headmsg();
	ReplyFamilyInvite.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_REPLYFAMILYINVITE );
	ReplyFamilyInvite.mutable_headmsg()->set_roleid( RoleID );
	if ( EntityID != 0 )
	{
		ReplyFamilyInvite.mutable_headmsg()->set_entityid( EntityID );
	}
	
	MsgHead.set_msgpara( (int)&ReplyFamilyInvite );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REPLYFAMILYINVITE );
	//MakeMessage( MsgHead, TranferMsg, RoleID, &ReplyFamilyInvite, ID_S2C_RESPONSE_REPLYFAMILYINVITE );	
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
}

// ***********************************************************
//  Function:		SendResponseReplyMemberApply
//  Description:	答复加入家族申请的回应消息
//  Output:			
//  Date:	04/21/2009
// **********************************************************
void CServiceFamily::SendResponseReplyMemberApply( int vServerID, int RoleID, int Errcode, int nResult, int nCharID, const char *pApplyName, int EntityID /* = 0 */ )
{
	CMessage MsgHead;	
	CMessageReplyMemberApplyResponse MsgReplyFamilyApply;
	
	MsgReplyFamilyApply.set_errcode( Errcode );
	MsgReplyFamilyApply.set_result( nResult );
	if ( pApplyName != NULL )
	{
		MsgReplyFamilyApply.set_playername( pApplyName );
	}
	
	MsgReplyFamilyApply.clear_headmsg();
	MsgReplyFamilyApply.mutable_headmsg()->set_msgid( ID_S2C_RESPONSE_REPLYMEMBERAPPLY );
	MsgReplyFamilyApply.mutable_headmsg()->set_roleid( RoleID );
	MsgReplyFamilyApply.set_charid( nCharID );
	if ( EntityID != 0 )
	{	 
		MsgReplyFamilyApply.mutable_headmsg()->set_entityid( EntityID );
	}
	
	MsgHead.set_msgpara( (int)&MsgReplyFamilyApply );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REPLYMEMBERAPPLY );
	
	CGateServer::GetSingleton().Send2Scene( vServerID, &MsgHead );
}

// ***********************************************************
//  Function:		MakeMessage
//  Description:	把消息组合到一起
//  Output:			
//  Date:	04/29/2009
// **********************************************************
void CServiceFamily::MakeMessage( CMessage &MsgHead, int RoleID, void *pMsg, int MsgID, int EntityID /* = 0 */ )
{ 
	MsgHead.set_msgpara( (int)&pMsg );
	MsgHead.mutable_msghead()->set_messageid( ID_G2S_RESPONSE_FAMILYMSG );	
}

// ***********************************************************
//  Function:		SendNotifyInviteResult
//  Description:	发送邀请的结果
//  Output:			
//  Date:	04/29/2009
// **********************************************************
void CServiceFamily::SendNotifyInviteResult( CGatePlayer *pPlayer, int nRoleID, int nResult, const char *pName )
{  
	LK_ASSERT( pPlayer != NULL, return )
	CMessage MsgHead;	
	CMessageInviteFamilyResultNotify MsgInviteResult;
	
	MsgInviteResult.set_result( nResult );	
	if ( pName != NULL )
	{
		MsgInviteResult.set_rolename( pName );
	}
	
	MsgInviteResult.clear_headmsg();
	MsgInviteResult.mutable_headmsg()->set_msgid( ID_S2C_NOTIFY_INVITERESULT );
	MsgInviteResult.mutable_headmsg()->set_roleid( nRoleID );									   
	
	MsgHead.set_msgpara( (int)&MsgInviteResult );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_INVITERESULT );
	// MakeMessage( MsgHead, MsgTranfer, RoleID, &MsgInviteResult, ID_S2C_NOTIFY_INVITERESULT );
	CGateServer::GetSingleton().Send2Scene( pPlayer, &MsgHead );
}

// ***********************************************************
//  Function:		SendFamilyNotify
//  Description:	发送家族的通知消息( 家族信息 )
//  Output:			
//  Date:	04/29/2009
// **********************************************************
void CServiceFamily::SendFamilyNotify( CMessage *pMessage, int *pRoleIDList, int vRoleNum ) 
{  
	LK_ASSERT( pMessage != NULL, return );

	Message* pUnknownMessage = (Message*) pMessage->msgpara();	 
	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessage->GetDescriptor();
	const ::google::protobuf::Reflection*  pReflection = pUnknownMessage->GetReflection();
	if (pDescriptor == NULL || pReflection == NULL) return;	   
	const ::google::protobuf::FieldDescriptor*  pFieldDescriptor = pDescriptor->FindFieldByNumber(1);
	if (pFieldDescriptor == NULL) return;					 
	assert(pFieldDescriptor->type() == ::google::protobuf::FieldDescriptor::TYPE_MESSAGE);	  
	const Message& tRouter = pReflection->GetMessage(*pUnknownMessage, pFieldDescriptor); 	
	FamilyHead* tpFmailyHead = (FamilyHead*) &tRouter;	
	
	for ( int i = 0; i<vRoleNum; ++i )
	{
		CGatePlayer *pPlayer = CPlayerManager::GetSingleton().ScenePlayer( pRoleIDList[i] );
		if ( pPlayer == NULL  )
		{
			continue;
		}
		tpFmailyHead->set_roleid( pRoleIDList[i] );
		CGateServer::GetSingleton().Send2Scene( pPlayer, pMessage );
	}
}	

// ***********************************************************
//  Function:		SendNotifyFamilyMoneyChanged
//  Description:	发送家族金钱改变的通知
//  Output:			
//  Date:	09/14/2009
// ********************************************************** 
void CServiceFamily::SendNotifyFamilyMoneyChanged( int nMoney, int nRoleID, int nFamilyID, int nReason, int tValue, const char *pName /* = NULL */ )
{ 
	CMessage tMsgHead;
	CMessageFamilyMoneyChangedNotify tMsgFamilyMoney;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYMONEYCHANGED );	
	tMsgHead.set_msgpara( (int)&tMsgFamilyMoney );
	
	tMsgFamilyMoney.set_money( nMoney );
	tMsgFamilyMoney.set_roleid( nRoleID );
	tMsgFamilyMoney.set_reason( nReason );
	if ( pName != NULL )
	{
		tMsgFamilyMoney.set_rolename( pName );
	}
	tMsgFamilyMoney.set_value( tValue );
	SendFamilyMsgByID( nFamilyID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyFamilyGloryChanged
//  Description:	发送家族荣耀改变的通知
//  Output:			
//  Date:	09/14/2009
// ********************************************************** 
void CServiceFamily::SendNotifyFamilyGloryChanged( int nGlory, int nRoleID, int nFamilyID, int nReason, int tValue, const char *pName /* = NULL */ )
{
	CMessage tMsgHead;
	CMessageFamilyGloryChangedNotify tMsgFamilyGlory;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYGLORYCHANGED );		
	tMsgHead.set_msgpara( (int)&tMsgFamilyGlory );
	
	tMsgFamilyGlory.set_roleid( nRoleID );
	tMsgFamilyGlory.set_glory( nGlory );
	tMsgFamilyGlory.set_reason( nReason );
	if ( pName != NULL )
	{
		tMsgFamilyGlory.set_rolename( pName );
	}
	tMsgFamilyGlory.set_value( tValue );
	
	SendFamilyMsgByID( nFamilyID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyFamilyContributeChanged
//  Description:	发送家族成员贡献度改变的通知
//  Output:			
//  Date:	09/14/2009
// ********************************************************** 	
void CServiceFamily::SendNotifyFamilyContributeChanged( int nContribute, int nRoleID, int nFamilyID, int tValues )
{
	CMessage tMsgHead;
	CMessageMemberContributeChangedNotify tMsgContribute;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MEMBERCONTRIBUTECHANGED );
	tMsgHead.set_msgpara( (int)&tMsgContribute );
	tMsgContribute.set_roleid( nRoleID );
	tMsgContribute.set_contribute( nContribute );
	tMsgContribute.set_value( tValues );
	
	// 目前只发给改变者
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( nRoleID );
	if ( tpPlayer != NULL )
	{
		tMsgContribute.mutable_headmsg()->set_roleid( nRoleID );
		CGateServer::GetSingleton().Send2Scene( tpPlayer, &tMsgHead );
	}
	
	//SendFamilyMsgByID( nFamilyID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyFamilyCanLevelUP
//  Description:	发送家族可以升级的通知
//  Output:			
//  Date:	09/27/2009
// ********************************************************** 
void CServiceFamily::SendNotifyFamilyCanLevelUP( int nFamilyID )
{
	CMessage tMsgHead;
	CMessageFamilyCanLevelUPNotify tMsgFamilyCanLevelUP;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYCANLEVELUP );	
	tMsgHead.set_msgpara( (int)&tMsgFamilyCanLevelUP );	
	
	SendFamilyMsgByID( nFamilyID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendResponseBuyFamilyItem
//  Description:	发送购买家族红包的消息请求
//  Output:			
//  Date:	10/29/2009
// **********************************************************
void CServiceFamily::SendResponseBuyFamilyItem( int nServerID, int nRoleID, int nErrcode, int nRedPaperID, int nRedPaperNum, int nEntityID /* = 0 */ )
{
	CMessage tMsgHead;
	CMessageBuyFamilyItemResponse tMsgBuyFamilyItem;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_G2S_RESPONSE_BUYFAMILYITEM );
	tMsgHead.set_msgpara( (int)&tMsgBuyFamilyItem );
	
	tMsgBuyFamilyItem.set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgBuyFamilyItem.set_entityid( nEntityID );
	}
	
	tMsgBuyFamilyItem.set_errcode( nErrcode );
	tMsgBuyFamilyItem.set_itemid( nRedPaperID );
	tMsgBuyFamilyItem.set_itemnum( nRedPaperNum );
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		OnCreatFamilyDBInfo
//  Description:	创建家族数据库信息的回应消息
//  Output:			
//  Date:	05/09/2009
// **********************************************************
int CServiceFamily::OnCreatFamilyDBInfo( CMessage* tpMessage, int FamilyObjID, int Para )
{
	LK_ASSERT( tpMessage != NULL, return -1 );
	CMessageExecuteSqlResponse3* pMessage = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();
	LK_ASSERT( pMessage != NULL, return -1 )
	CFamily *pFamily = ( CFamily * )CGateObjManager::GetSingleton().GetObject( FamilyObjID );
	if ( pFamily == NULL )
	{
		LOG_ERROR( "family","can not find family obj by objID:%d", FamilyObjID );
		return -1;
	}
	
	if ( pMessage->resultcode() == 1 )
	{
		#ifndef _GLOBAL_NAME_
		pFamily->SetID( pMessage->latestautoincid() );
		#endif
		
		// 加入列表	 
		int tRet = CFamilyManager::GetSingleton().InsertFamily( pFamily );
		if ( tRet != 0 )
		{							
			CGateObjManager::GetSingleton().DestroyObject( pFamily->get_id() );
			return -1;
		}
		CGatePlayer *tpGatePlayer = CPlayerManager::GetSingleton().ScenePlayer( pFamily->GetRooter() );
		if ( tpGatePlayer == NULL )
		{
			LOG_ERROR( "family","can not find player(CGatePlayer) by roleid:%d ", pFamily->GetRooter() );
			return -1;
		}
		
		// 日志
		LogOrganCreate( ORGANLOGTYPE_FAMILY, pFamily->GetID(), pFamily->GetRooter(), 1, 0 )
		SendResponseCreatFamily( tpGatePlayer->mLineID, tpGatePlayer->mCharID, SUCCESS, pFamily->GetID(), pFamily->GetName(), 0 );
	}
	return 0;
}


// ***********************************************************
//  Function:		SendFamilyMsgByID
//  Description:	通过家族ID发送家族消息
//  Output:			
//  Date:	07/20/2009
// **********************************************************
void CServiceFamily::SendFamilyMsgByID( int nFamilyID, CMessage *pMessage )
{  
	LK_ASSERT( pMessage != NULL, return )
	
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( nFamilyID );
	if ( tpFamily == NULL )
	{
		return;
	}
	pMessage->mutable_msghead()->set_dstfe( FE_FAMILY );
	int tRoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 };
	int tNum = MAX_FAMILYMEMBER_NUM;
	tpFamily->GetFamilyMemberIDList( tRoleIDList, tNum );
	std::vector< CGatePlayer * > vPlayerList;	
	for ( int i = 0; i < tNum; ++i )
	{
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tRoleIDList[i] );
		if ( tpPlayer == NULL )
		{
			continue;
		}		
		
		if ( pMessage->mutable_msghead()->srcfe() == FE_GATESERVER )
		{
			vPlayerList.push_back( tpPlayer );
		}
		else
		{
			pMessage->mutable_msghead()->set_dstid( tRoleIDList[i] );
			CGateServer::GetSingleton().Send2Scene( tpPlayer, pMessage );
		}
	}	
	
	if ( pMessage->mutable_msghead()->srcfe() == FE_GATESERVER )
	{
		CGateServer::GetSingletonPtr()->Send2Player( vPlayerList, pMessage );
	}
}


// ***********************************************************
//  Function:		ReloadCfg
//  Description:	重载配置
//  Output:			
//  Date:	08/21/2009
// **********************************************************
void CServiceFamily::ReloadCfg( )
{
	int nRet = InitializeFamilyconfig( );
	if( nRet < 0 )
	{
		printf( "read family config failed\n" );
		LOG_FATAL( "family", "[%s:%d] read family config faild ", __FUNCTION__, __LINE__ );
		exit( 0 );
	}
}

// ***********************************************************
//  Function:		InitializeFamilyconfig
//  Description:	初始化家族配置信息
//  Output:			
//  Date:	08/21/2009
// **********************************************************
int CServiceFamily::InitializeFamilyconfig( )
{
	TiXmlDocument tDoc;
	//const char *pFileName = "../config/data/family/familyconfig.xml";
	const char *pFileName = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"family/familyconfig.xml");
	if ( !tDoc.LoadFile( pFileName ) )
	{			
		return -1;
	}
	
	TiXmlElement *tpElement = tDoc.FirstChildElement("FamilyConfig");
	if ( tpElement == NULL )
	{
		return -1;
	}
	
	TiXmlElement *tpLevelInfo = tpElement->FirstChildElement( "LevelConfig" );
	if ( tpLevelInfo == NULL )
	{
		return -1;
	}
	
	TiXmlElement * tpInfo = tpLevelInfo->FirstChildElement( "ConfigInfo" );
	LK_ASSERT( tpInfo != NULL, return -1  ) 
	CFamilyManager::GetSingleton().GetFamilyConfig()->Initialize();
	for ( ; tpInfo != NULL; tpInfo = tpInfo->NextSiblingElement( ) )
	{
		CFamilyConfigInfo tFamilyConfig;
		tFamilyConfig.mGloryLimit			= atoi( tpInfo->Attribute( "GloryLimit" ) );
		tFamilyConfig.mLevel				= atoi( tpInfo->Attribute( "Level" ) );
		tFamilyConfig.mMemberNumLimit		= atoi( tpInfo->Attribute( "MemberLimit" ) );
		tFamilyConfig.mPresbyterNumLimit	= atoi( tpInfo->Attribute( "PresbyterNumLimit" ) );	
		tFamilyConfig.mPVPScoreLimit		= atoi( tpInfo->Attribute( "PVPScoreLimit" ) );	
		tFamilyConfig.mGloryLevelUPNeeded	= atoi( tpInfo->Attribute( "GloryLevelUPNeeded" ) );
		tFamilyConfig.mMoneyLevelUPNeeded	= atoi( tpInfo->Attribute( "MoneyLevelUpNeeded" ) );
		tFamilyConfig.mInitialStability		= atoi( tpInfo->Attribute( "InitialStability" ) );		
		tFamilyConfig.mReduceStability		= atoi( tpInfo->Attribute( "ReduceStability" ) );		
				
		CFamilyManager::GetSingleton().GetFamilyConfig()->InsertFamilyConfig( tFamilyConfig );
	}
	
	TiXmlElement *tpPropertyInfo = tpElement->FirstChildElement( "PropertyConfig" );
	LK_ASSERT( tpPropertyInfo != NULL, return -1 )
	
	tpInfo = tpPropertyInfo->FirstChildElement( "PropertInfo" );
	LK_ASSERT( tpInfo != NULL, return -1 )
	CFamilyPropertyConfigInfo tFamilyPropertyInfo;
	memset( &tFamilyPropertyInfo, 0, sizeof(tFamilyPropertyInfo) );
	tFamilyPropertyInfo.mContributePerGold = atoi( tpInfo->Attribute("ContributePerGold") );
	tFamilyPropertyInfo.mScorePerFight	   = atoi( tpInfo->Attribute( "ScorePerFight" ) );
	tFamilyPropertyInfo.mHour			   = atoi( tpInfo->Attribute( "Hour" ) );
	tFamilyPropertyInfo.mMinute			   = atoi( tpInfo->Attribute( "Minute" ) );
	tFamilyPropertyInfo.mGloryPerFight	   = atoi( tpInfo->Attribute( "GloryPerFight" ) );	
	tFamilyPropertyInfo.mNpcGlory		   = atoi( tpInfo->Attribute("NpcGlory") );
	tFamilyPropertyInfo.mNpcMoney		   = atoi( tpInfo->Attribute("NpcMoney") );
	tFamilyPropertyInfo.mExpPerGlory	   = atoi( tpInfo->Attribute("ExpPerGlory") );
	tFamilyPropertyInfo.mExploitPerGlory   = atoi( tpInfo->Attribute("ExploitPerGlory") );
	tFamilyPropertyInfo.mRepetionMoney	   = atoi( tpInfo->Attribute("RepetionMoney") );
	tFamilyPropertyInfo.mRepetionGlory	   = atoi( tpInfo->Attribute("RepetionGlory") );
	tFamilyPropertyInfo.mShopRefreshTimeLength = atoi( tpInfo->Attribute("ShopRefreshTimeLength") );
	tFamilyPropertyInfo.mRefreshBasicTime  = atoi( tpInfo->Attribute("BasicTime") ); 
	tFamilyPropertyInfo.mWarNpcSellTableID = atoi( tpInfo->Attribute("WarNpcSellTableID") ); 
	tFamilyPropertyInfo.mChallangeMoney = atoi( tpInfo->Attribute("ChallangeMoney") ); 
	tFamilyPropertyInfo.mChallangeTimesOfDay = atoi( tpInfo->Attribute("ChallangeTimeOfDay") ); 
	tFamilyPropertyInfo.mRepetionStability = atoi( tpInfo->Attribute( "RepetionStability" ) );

	CFamilyManager::GetSingleton().GetFamilyConfig()->SetFamilyPropertyConfig( tFamilyPropertyInfo );
	
	TiXmlElement *tpNpcWelfare = tpElement->FirstChildElement( "NpcWelfare" );
	LK_ASSERT( tpNpcWelfare != NULL, return -1 )
	
	TiXmlElement *tpWelfareInfo = tpNpcWelfare->FirstChildElement( "Welfare" );
	LK_ASSERT( tpWelfareInfo != NULL, return -1 )
	for ( ; tpWelfareInfo != NULL; tpWelfareInfo = tpWelfareInfo->NextSiblingElement( ) )
	{	
		CNpcWelfare tNpcWelfare;
		int tNpcTmpID = atoi( tpWelfareInfo->Attribute("NpcTmpID") );						
		tNpcWelfare.mNpcMoney = atoi( tpWelfareInfo->Attribute("NpcMoney") );						
		tNpcWelfare.mNpcGlory = atoi( tpWelfareInfo->Attribute("NpcGlory") );
		CFamilyManager::GetSingleton().GetFamilyConfig()->InsertNpcWelfare( tNpcTmpID, tNpcWelfare );
	}	
	
	TiXmlElement *tpRepetionConfig = tpElement->FirstChildElement( "RepetionConfig" );
	LK_ASSERT( tpRepetionConfig != NULL, return -1 )
	TiXmlElement *tpTimeInfo = tpRepetionConfig->FirstChildElement( "TimeInfo" );
	LK_ASSERT( tpTimeInfo != NULL, return -1 )
	
	for ( ; tpTimeInfo != NULL; tpTimeInfo = tpTimeInfo->NextSiblingElement() )
	{
		CRepetionTimeInfo tRepetionTimeInfo;
		if ( tpTimeInfo->Attribute( "BeginTime" ) != NULL )
		{
			tRepetionTimeInfo.mBeginTime = atoi( tpTimeInfo->Attribute( "BeginTime" ) );
		}
		
		if ( tpTimeInfo->Attribute( "EndTime" ) != NULL )
		{
			tRepetionTimeInfo.mEndTime = atoi( tpTimeInfo->Attribute( "EndTime" ) );
		}	
		
		if ( tpTimeInfo->Attribute( "OpenTimes" ) != NULL )
		{
			tRepetionTimeInfo.mOpenTimes = atoi( tpTimeInfo->Attribute( "OpenTimes" ) );
		}
		
		if ( tpTimeInfo->Attribute( "WeeKday" ) != NULL )
		{
			tRepetionTimeInfo.mWeekDay = atoi( tpTimeInfo->Attribute( "WeeKday" ) );
		}	
		CFamilyManager::GetSingleton().GetFamilyConfig()->InsertRepetionTimeInfo( tRepetionTimeInfo );			
	}	
	TiXmlElement *tpWeekNum = tpRepetionConfig->FirstChildElement( "WeekNum" );
	if ( tpWeekNum != NULL )
	{
		if ( tpWeekNum->Attribute( "Number" ) != NULL )
		{
			CFamilyManager::GetSingleton().GetFamilyConfig()->SetRepetionWeekNum( atoi( tpWeekNum->Attribute( "Number" ) ) );
		}
	}

	// 安定度配置
	TiXmlElement *tpStabilityElement = tpElement->FirstChildElement( "StabilityConfig" );
	LK_ASSERT( tpStabilityElement != NULL, return -1 )
	
	CStabilityConfig *tpStabilityConfig = CFamilyManager::GetSingleton().GetFamilyConfig()->GetStabilityConfig();
	if ( tpStabilityElement->Attribute( "TaskStability" ) != NULL )
	{
		tpStabilityConfig->SetTaskStablity( atoi( tpStabilityElement->Attribute( "TaskStability" ) ) );
	}
	if ( tpStabilityElement->Attribute( "MoneyStability" ) != NULL )
	{
		tpStabilityConfig->SetMoneyStability( atoi( tpStabilityElement->Attribute( "MoneyStability" ) ) );
	}
	if ( tpStabilityElement->Attribute( "DisbandMailID" ) != NULL )
	{
		tpStabilityConfig->SetDisbandMailID( atoi( tpStabilityElement->Attribute( "DisbandMailID" ) ) );
	}
	if ( tpStabilityElement->Attribute( "HeaderMailID" ) != NULL )
	{
		tpStabilityConfig->SetHeaderMailID( atoi( tpStabilityElement->Attribute( "HeaderMailID" ) ) );
	}
	if ( tpStabilityElement->Attribute( "UseStability" ) != NULL )
	{
		tpStabilityConfig->SetUseStability( atoi( tpStabilityElement->Attribute( "UseStability" ) ) );
	}
	
	for ( TiXmlElement *tpLevelInfo = tpStabilityElement->FirstChildElement( "LevelInfo" ); 
			tpLevelInfo != NULL; tpLevelInfo = tpLevelInfo->NextSiblingElement() )
	{
		CStability tStability;
		if ( tpLevelInfo->Attribute( "Level" ) != NULL )
		{
			tStability.mLevel = atoi( tpLevelInfo->Attribute( "Level" ) );
		}
		
		for ( TiXmlElement *tpTimeElement = tpLevelInfo->FirstChildElement( "Time" ); 
				tpTimeElement != NULL; tpTimeElement = tpTimeElement->NextSiblingElement() )
		{
			int tTime = 0;
			int tStabilityValue = 0;
			if ( tpTimeElement->Attribute( "Value" ) != NULL )
			{
				tTime = atoi( tpTimeElement->Attribute( "Value" ) );
			}
			
			if ( tpTimeElement->Attribute("Stability" ) != NULL )
			{
				tStabilityValue = atoi( tpTimeElement->Attribute( "Stability" ) );
			}	
			tStability.InsertStablity( tTime, tStabilityValue );
		}
		
		tpStabilityConfig->InsertLevelInfo( tStability );
	}		
	
	// 邮件配置
	TiXmlElement *tpMailConfig = tpElement->FirstChildElement( "MailConfig" );
	LK_ASSERT( tpMailConfig != NULL, return -1 )
	for ( TiXmlElement *tpMailInfo = tpMailConfig->FirstChildElement( "MailInfo" );	tpMailInfo != NULL;
			tpMailInfo = tpMailInfo->NextSiblingElement() )
	{
		int tSysID = 0;
		CSysMailInfo tMailInfo;
		if ( tpMailInfo->Attribute( "ID" ) != NULL )
		{
			tSysID = atoi( tpMailInfo->Attribute( "ID" ) );
		}
		
		int i = 0;
		for ( TiXmlElement *tpMailItem = tpMailInfo->FirstChildElement( "Item" ); tpMailItem != NULL;
				tpMailItem = tpMailItem->NextSiblingElement() )
		{	
			if ( i >= ARRAY_CNT( tMailInfo.mItemID ) )
			{
				break;
			}
			if ( tpMailItem->Attribute( "ID" ) != NULL )
			{
				tMailInfo.mItemID[i] = atoi( tpMailItem->Attribute( "ID" ) );
			}
			
			if ( tpMailItem->Attribute( "Num" ) != NULL )
			{
				tMailInfo.mItemNum[i] = atoi( tpMailItem->Attribute( "Num" ) );
			}
			++i;
		}
				
		CFamilyManager::GetSingleton().GetFamilyConfig()->GetMailConfig()->InsertMailInfo( tSysID, tMailInfo );
	}	
	
	// 家族内丹配置
	TiXmlElement *tpRedstoneConfig = tpElement->FirstChildElement( "RedstoneConfig" );
	LK_ASSERT( tpRedstoneConfig != NULL, return -1 )
	CFamilyManager::GetSingleton().GetFamilyConfig()->GetFamilyRedstoneConfig()->Initiallize();
	for ( TiXmlElement *tpLevelConfig = tpRedstoneConfig->FirstChildElement();
			tpLevelConfig != NULL; tpLevelConfig = tpLevelConfig->NextSiblingElement( ) )
	{
		int tLevel = 0;
		if ( tpLevelConfig->Attribute( "FamilyLevel" ) != NULL )
		{
			tLevel = atoi( tpLevelConfig->Attribute( "FamilyLevel" ) );
		}

		for ( TiXmlElement *tpStaConfig = tpLevelConfig->FirstChildElement();
				tpStaConfig != NULL; tpStaConfig = tpStaConfig->NextSiblingElement( ) )
		{
			int tStability = 0;
			if ( tpStaConfig->Attribute( "Stablitity" ) != NULL )
			{
				tStability = atoi( tpStaConfig->Attribute( "Stablitity" ) );
			}
			int tRedstoneID = 0;
			if ( tpStaConfig->Attribute( "RedStoneID" ) != NULL )
			{
				tRedstoneID = atoi( tpStaConfig->Attribute( "RedStoneID" ) );
			}
			int tExtraExp = 0;
			if ( tpStaConfig->Attribute( "ExtraExp" ) != NULL )
			{
				tExtraExp = atoi( tpStaConfig->Attribute( "ExtraExp" ) );
			}		
			CFamilyManager::GetSingleton().GetFamilyConfig()->GetFamilyRedstoneConfig()->InsertFamilyRedstone( tLevel, tRedstoneID, tStability, tExtraExp );
		}
	}
	return SUCCESS;
}

// ***********************************************************
//  Function:		AddPvPScore
//  Description:	增加pvp积分的消息
//  Output:			
//  Date:	08/21/2009
// **********************************************************
void CServiceFamily::AddPvPScore( int nFamily, int nScore )
{
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( nFamily );	
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family by ID:%d ", nFamily );
		return;
	}
	
	bool tRet = tpFamily->AddPVPScore( nScore, tpFamily->GetPVPStarLevel() );
	
	// 如果pvp星级提升的话
	if ( tRet == true )
	{
		int tRoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 } ;
		int tNum = MAX_FAMILYMEMBER_NUM;
		tpFamily->GetFamilyMemberIDList( tRoleIDList, tNum );
		SendNotifyPVPStarLevelUP( tRoleIDList, tNum, tpFamily->GetPVPStarLevel() );		
	}
}

// ***********************************************************
//  Function:		ConsumeFamilyMoney
//  Description:	消耗家族金钱
//  Output:			
//  Date:	08/21/2009
// **********************************************************
int CServiceFamily::ConsumeFamilyMoney( int nRoleID, int nMoney, bool bFreeze /* = false */, int nReason /* = REASON_CHALLENGEWAR */ )
{
	int tRet = CFamilyManager::GetSingleton( ).OnEventConsumeMoney( nRoleID, nMoney, bFreeze );
	if ( tRet == -1 )
	{
		return -1;
	}
	
	if ( tRet != SUCCESS )
	{ 
		return tRet;		
	} 	
	
	CFamily *tpFamily = CFamilyManager::GetSingleton( ).GetFamilyOfPlayer( nRoleID );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family","can not find family of player(roleid:%d)", nRoleID );
		return -1;
	}

	
	CRoleGlobalInfo *tpManagerInfo = CServiceRoleIDName::GetSingleton( ).GetRoleInfoByRoleID( nRoleID );
	if ( tpManagerInfo == NULL )
	{
		LOG_ERROR( "family"," can not find player(CRoleGlobalInfo) of player( roleid:%d ) ", nRoleID );
		return -1;
	}
	
	LOG_NOTICE( "family","[%s,%d] familymanager consume family money( managerid:%d, familyid:%d, money:%d, reason:%d )  ", __FUNCTION__,__LINE__, nRoleID, tpFamily->GetID(), nMoney, nReason );	
		
	CMessage tMsgHead;
	CMessageFamilyMoneyChangedNotify tMsgConsumeMoney;	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYMONEYCHANGED );
	tMsgHead.set_msgpara( (int)&tMsgConsumeMoney );
	tMsgConsumeMoney.set_money( tpFamily->GetFamilyMoney() );
	tMsgConsumeMoney.set_roleid( nRoleID );
	tMsgConsumeMoney.set_reason( nReason );
	tMsgConsumeMoney.set_value( -1*nMoney );
	tMsgConsumeMoney.set_rolename( tpManagerInfo->RoleName() );		
	SendFamilyMsgByID( tpFamily->GetID(), &tMsgHead );
	
	// LOG:消耗金钱
	LogOrganGetMoney( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), nRoleID, tpFamily->GetFamilyMoney(), tpFamily->GetFamilyMoney()+nMoney )
	return SUCCESS;
}

// ***********************************************************
//  Function:		AddFamilyMoney
//  Description:	增加家族金钱
//  Output:			
//  Date:	08/21/2009
// **********************************************************
int CServiceFamily::AddFamilyMoney( int nRoleID, int nMoney, const char *pName, bool bFreeze /* = false */, int nReason /* = REASON_WARWIN */ )
{  
	CFamily *tpFamily = CFamilyManager::GetSingleton( ).GetFamilyOfPlayer( nRoleID );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family of player(roleid:%d) ", nRoleID );
		return -1;
	}
	
	tpFamily->AddFamilyMoney( nMoney, bFreeze );
	
	CMessage tMsgHead;
	CMessageFamilyMoneyChangedNotify tMsgFamilyMoneyChanged;	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYMONEYCHANGED );
	tMsgHead.set_msgpara( (int)&tMsgFamilyMoneyChanged );
	tMsgFamilyMoneyChanged.set_money( tpFamily->GetMoney() );
	tMsgFamilyMoneyChanged.set_reason( nReason );
	tMsgFamilyMoneyChanged.set_value( nMoney );
	if ( pName != NULL )
	{
		tMsgFamilyMoneyChanged.set_rolename( pName );
	}

	SendFamilyMsgByID( tpFamily->GetID(), &tMsgHead );
	return SUCCESS;		
}

void CServiceFamily::OnMessageBidNpc( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )	
	CMessageFamilyBidNpcRequest *tpMsg = ( CMessageFamilyBidNpcRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{	
		LOG_ERROR( "family","can not find family of player(roleid:%d)", tpHead->roleid() );
		return;
	}
	
	// 权限检查
	if ( ( int )tpHead->roleid() != tpFamily->GetRooter() )
	{
	
		if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_WAR_RID ) )
		{
			SendBidNpcResponse( nServerID, ERROR_FAMILY_BIDNPC_NOTHEADER, tpMsg->npcid(), 0, tpHead->roleid(), tpHead->entityid() );
			return;		
		}		
	}
	if ( tpFamily->GetBattleState( ) != STATE_BIDDING )
	{
		SendBidNpcResponse( nServerID, ERROR_FAMILY_BIDNPC_WRONGTIME, tpMsg->npcid(), 0, tpHead->roleid(), tpHead->entityid() );
		return;
	}
	if ( tpFamily->GetFamilyStatus( ) == CFamily::EM_FAMILY_STATUS_DISBAND )
	{
		SendBidNpcResponse( nServerID, ERROR_FAMILY_BIDNPC_DISBAND, tpMsg->npcid(), 0, tpHead->roleid(), tpHead->entityid() );
		return;
	}
	CFamilyManager::GetSingletonPtr()->InsertNPC( tpMsg->npcid(), tpFamily->GetCountryID() );
	int tTime = time(NULL);
	int tMoney = CFamilyManager::GetSingletonPtr()->AddBid( tpMsg->npcid(), tpFamily->GetID(),tpMsg->money(), tTime, tpFamily->GetCountryID() );
	if ( tMoney > 0 )
	{
		int tRet = ConsumeFamilyMoney( tpHead->roleid(), tMoney, false, REASON_WAR );
		if ( tRet != SUCCESS )
		{
			CFamilyManager::GetSingletonPtr()->DelBid( tpMsg->npcid(), tpFamily->GetID(), tpFamily->GetCountryID() );
			SendBidNpcResponse( nServerID, ERROR_FAMILY_BIDNPC_NOMONEY, tpMsg->npcid(), tpMsg->money(), tpHead->roleid(), tpHead->entityid() );
			return;
		}
		SendBidNpcResponse( nServerID, SUCCESS, tpMsg->npcid(), tpMsg->money(), tpHead->roleid(), tpHead->entityid() );
		tpFamily->SetNpcID( tpMsg->npcid() );
		tpFamily->SetBidMoney( tpMsg->money() );
		tpFamily->SetBidTime( tTime );
		LogBidNpc( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tpMsg->npcid(), tpMsg->money(), tMoney );
	}
	else
	{
		SendBidNpcResponse( nServerID, tMoney, 0, 0, tpHead->roleid(), tpHead->entityid() );
	}		
}

void CServiceFamily::SendBidNpcResponse( int vServerID, int vResult, int vNpcID, int vMoney, int vRoleID, int vEntityID )
{	
	CMessage tMsgHead;
	CMessageFamilyBidNpcResponse tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( vRoleID );	
	if ( vEntityID != 0 )
	{
		tMsgResponse.mutable_headmsg()->set_entityid( vEntityID );
	}
	tMsgResponse.set_result( vResult );
	tMsgResponse.set_npcid( vNpcID );
	tMsgResponse.set_money( vMoney );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_BIDNPC );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
}

void CServiceFamily::SendMemberSubmitNotify( int vServerID, int vNpcID, const char *vFamilyName, int vFirst, int vRoleID, int vEntityID )
{
	// 如果族长charid为0，就不发消息
	if( vRoleID == 0 )
		return;

	CMessage tMsgHead;
	CMessageMemberSubmitNotify tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( vRoleID );	
	if ( vEntityID != 0 )
	{
		tMsgResponse.mutable_headmsg()->set_entityid( vEntityID );
	}
	tMsgResponse.set_npcid( vNpcID );
	tMsgResponse.set_familyname( vFamilyName );
	tMsgResponse.set_first( vFirst );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MEMBERSUBMIT );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( (CSceneServer*)NULL, &tMsgHead );
}

void CServiceFamily::OnMessageBidTimeOut( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageBidTimeOutNotify *tpMsg = ( CMessageBidTimeOutNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	int tAttFamilyID = 0;
	int tDefFamilyID = 0;
	CFamilyManager::GetSingletonPtr()->GetWinner( tpMsg->npcid(), tAttFamilyID, tDefFamilyID, nServerID );
	CFamily *tpAttFamily = CFamilyManager::GetSingletonPtr()->GetFamilyByID( tAttFamilyID );
	CFamily *tpDefFamily = CFamilyManager::GetSingletonPtr()->GetFamilyByID( tDefFamilyID );
	//LK_ASSERT( tpAttFamily != NULL && tpDefFamily != NULL, return );
	if ( tAttFamilyID != -1 )
	{
		int tAttMoney = 0, tDefMoney = 0;
		if ( tpDefFamily != NULL )
		{
			tpDefFamily->SetBattleState( STATE_SUBMITTING );
			tpDefFamily->SetPVPName( tpAttFamily != NULL ? tpAttFamily->GetName() : "" );
			tDefMoney = tpDefFamily->GetBidMoney( );
		}
		if ( tpAttFamily != NULL )
		{
			tpAttFamily->SetBattleState( STATE_SUBMITTING );
			tpAttFamily->SetPVPName( tpDefFamily != NULL ? tpDefFamily->GetName() : "" );
			tAttMoney = tpAttFamily->GetBidMoney( );
		}
		
		// TODO:Post Need　Check
		// 向双方族长发送通知
		if ( tpDefFamily != NULL )
		{
			CGatePlayer *tpDefManager = CFamilyManager::GetSingleton().GetManagerOfRight( tpDefFamily, EM_WAR_RID );
			if ( tpDefManager != NULL )
			{
				SendMemberSubmitNotify( nServerID, tpMsg->npcid(), ( tpDefFamily != NULL ? tpDefFamily->GetName() : "" ), tpMsg->first(), tpDefManager->mCharID );
			}			
		}
		
		if ( tpAttFamily != NULL )
		{
			CGatePlayer *tpAttManager = CFamilyManager::GetSingleton().GetManagerOfRight( tpAttFamily, EM_WAR_RID );
			if ( tpAttManager != NULL )
			{
				SendMemberSubmitNotify( nServerID, tpMsg->npcid(), ( tpAttFamily != NULL ? tpAttFamily->GetName() : "" ), tpMsg->first(), tpAttManager->mCharID );
			}			
		}
				
		SendPreBattleNotify( nServerID, tpMsg->npcid(), tAttFamilyID, tDefFamilyID, tAttMoney );

		if (tpMsg->first())
		{
			LogConfirm( ORGANLOGTYPE_FAMILY, tDefFamilyID, 0, tAttFamilyID, tpMsg->npcid(), tAttMoney );
		}

	}	
}

void CServiceFamily::OnMessageBattleStart( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageBattleStartRequest *tpMsg = ( CMessageBattleStartRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	int tAttFamilyID = 0;
	int tDefFamilyID = 0;
	int tFamilyList[ CFamilyManager::em_family_num ] = { 0 };
	int tMoneyList[ CFamilyManager::em_family_num ] = { 0 };
	int tNum = 0;
	CFamilyManager::GetSingletonPtr()->GetWinner( tpMsg->npcid(), tAttFamilyID, tDefFamilyID, nServerID );
	CFamily *tpAttFamily = CFamilyManager::GetSingletonPtr()->GetFamilyByID( tAttFamilyID );
	CFamily *tpDefFamily = CFamilyManager::GetSingletonPtr()->GetFamilyByID( tDefFamilyID );
	if ( tpDefFamily == NULL )
	{
		CFamilyManager::GetSingletonPtr()->ClearBid( tpMsg->npcid(), nServerID );
		return;
	}
	// 处理退钱
	/*for ( int i = 0; i < tNum; i++ )
	{
		CFamily *tpFamily = CFamilyManager::GetSingletonPtr()->GetFamilyByID( tFamilyList[ i ] );
		if ( tpFamily == NULL )
		{
			continue;
		}
		AddFamilyMoney( tpFamily->GetRooter(), tpFamily->GetBidMoney() );
	}*/
	SendBattleStartResponse( nServerID, tpMsg->npcid(), tFamilyList, tMoneyList, tAttFamilyID, tDefFamilyID, tNum );

	// 如果无人竞价，防守方不进入战斗
	if ( tAttFamilyID == -1 )
	{
		tpDefFamily->SetBattleState( STATE_BIDDING );
		// 通知场景竞价重新开始
		SendBidRestartNotify( tpDefFamily->GetCountryID(), tpMsg->npcid() );
		CFamilyManager::GetSingletonPtr()->ClearBid( tpMsg->npcid(), nServerID );
	}
	else
	{
		CFamilyManager::GetSingletonPtr()->NotifyBattle( tAttFamilyID, tDefFamilyID, TYPE_BATTLE_NPC, tpMsg->npcid() );	

		tpDefFamily->SetBattleState( STATE_FIGHTING );
		if ( tpAttFamily != NULL )
		{
			tpAttFamily->SetBattleState( STATE_FIGHTING );
		}
		// 暂时把防守方的NPC设置一下
		if ( tpDefFamily->GetNpcID() == 0 )
		{
			tpDefFamily->SetNpcID( tpMsg->npcid() );
		}
	}		

	CFamilyManager::GetSingletonPtr()->ClearSubmit( tAttFamilyID );
	CFamilyManager::GetSingletonPtr()->ClearSubmit( tDefFamilyID );
	LogJoinNPCBattle( ORGANLOGTYPE_FAMILY, tDefFamilyID, 0, tAttFamilyID, tpMsg->npcid(), 0 );
}

void CServiceFamily::OnMessageBattleEnd( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageBattleEndNotify *tpMsg = ( CMessageBattleEndNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	CFamilyManager::GetSingletonPtr()->EndBattle( tpMsg->key(), tpMsg->winside() );
}

void CServiceFamily::SendBattleStartResponse( int vServerID, int vNpcID, int *vFamilyID, int *vMoney, int vAttID, int vDefID, int Num )
{	
	CMessage tMsgHead;
	CMessageBattleStartResponse tMsgResponse;

	tMsgResponse.set_npcid( vNpcID );
	for ( int i = 0; i < Num; i++ )
	{
		// 攻防双方不退钱
		if ( vFamilyID[ i ] == vAttID || vFamilyID[ i ] == vDefID )
			continue;
		CFamily *tpFamily = CFamilyManager::GetSingletonPtr()->GetFamilyByID( vFamilyID[ i ] );
		if ( tpFamily == NULL )
			continue;
		tMsgResponse.add_charid( tpFamily->GetRooter( ) );
		tMsgResponse.add_charname( tpFamily->GetRooterName( ) );
		tMsgResponse.add_money( vMoney[ i ] );
	}

	tMsgHead.mutable_msghead()->set_messageid( ID_G2S_RESPONSE_BATTLESTART );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
}

void CServiceFamily::OnMessageMemberSubmit( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageMemberSubmitRequest *tpMsg = ( CMessageMemberSubmitRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	LK_ASSERT( tpFamily != NULL, return )
	// TODO:Post Need　Check
	if ( (int)tpHead->roleid() != tpFamily->GetRooter() )
	{
		if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_WAR_SETFIGHTER ) )
		{
			SendMemberSubmitResponse( nServerID, ERROR_FAMILY_BIDNPC_NOTHEADER, tpMsg->npcid(), tpHead->roleid(), tpHead->entityid() );
			return;
		}		
	}
	if ( tpFamily->GetBattleState( ) != STATE_SUBMITTING )
	{
		SendMemberSubmitResponse( nServerID, ERROR_FAMILY_SUBMIT_WRONGTIME, tpMsg->npcid(), tpHead->roleid(), tpHead->entityid() );
		return;
	}
	CFamilyManager::GetSingletonPtr()->ClearSubmit( tpFamily->GetID() );

	int tAttFamilyID = 0;
	int tDefFamilyID = 0;
	CFamilyManager::GetSingletonPtr()->GetWinner( tpMsg->npcid(), tAttFamilyID, tDefFamilyID, nServerID );
	int tRivalFamilyID = tpFamily->GetID() == tAttFamilyID ? tDefFamilyID : tAttFamilyID;
	CFamily *tpRivalFamily = CFamilyManager::GetSingletonPtr()->GetFamilyByID( tRivalFamilyID );

	CMessage tMsgHead;
	CMessageFamilySubmitNotify tMsgNotify;

	tMsgNotify.mutable_headmsg()->set_roleid( tpHead->roleid() );	
	if ( tpHead->entityid() != 0 )
	{
		tMsgNotify.mutable_headmsg()->set_entityid( tpHead->entityid() );
	}	
	
	tMsgNotify.set_npcid( tpMsg->npcid() );
	tMsgNotify.set_familyname( tpRivalFamily != NULL ? tpRivalFamily->GetName() : "" );
	for ( int i = 0; i < tpMsg->memberid_size(); i++ )
	{
		CFamilyManager::GetSingletonPtr()->AddSubmit( tpFamily->GetID(), tpMsg->memberid( i ) );
		tMsgNotify.add_memberid( tpMsg->memberid(i) );
	}
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYSUBMIT );
	tMsgHead.set_msgpara( (int)&tMsgNotify );
	SendFamilyMsgByID( tpFamily->GetID(), &tMsgHead );
	SendMemberSubmitResponse( nServerID, SUCCESS, tpMsg->npcid(), tpHead->roleid(), tpHead->entityid() );

	LogSubmit( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tRivalFamilyID, tpMsg->npcid(), tpMsg->memberid_size() );
}

void CServiceFamily::SendMemberSubmitResponse( int vServerID, int vResult, int vNpcID, int vRoleID, int vEntityID )
{
	CMessage tMsgHead;
	CMessageMemberSubmitResponse tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( vRoleID );	
	if ( vEntityID != 0 )
	{
		tMsgResponse.mutable_headmsg()->set_entityid( vEntityID );
	}
	
	tMsgResponse.set_result( vResult );
	tMsgResponse.set_npcid( vNpcID );	

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_MEMBERSUBMIT );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
}

void CServiceFamily::OnMessageNpcInfo( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )	
	CMessageBidInfoRequest *tpMsg = ( CMessageBidInfoRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	/*CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family","can not find family of player(roleid:%d)", tpHead->roleid() );
		return;
	}*/
	// 新规定，没有家族也可以看竞价
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "family","can not find family of player(roleid:%d)", tpHead->roleid() );
		return;
	}
	
	SendNpcInfoResponse( nServerID, tpMsg->npcid(), tpPlayer->CountryID(), tpHead->roleid(), tpHead->entityid() );
}

void CServiceFamily::SendNpcInfoResponse( int vServerID, int vNpcID, int vCountryID, int vRoleID, int vEntityID )
{
	CMessage tMsgHead;
	CMessageBidInfoResponse tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( vRoleID );	
	if ( vEntityID != 0 )
	{
		tMsgResponse.mutable_headmsg()->set_entityid( vEntityID );
	}

	tMsgResponse.set_result( SUCCESS );
	tMsgResponse.set_npcid( vNpcID );
	CFamilyManager::GetSingletonPtr()->GetBid( vNpcID, vCountryID, tMsgResponse );	

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_BIDINFO );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
}

void CServiceFamily::OnMessageAbortNpc( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )	
	CMessageAbortNpcRequest *tpMsg = ( CMessageAbortNpcRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family of player(roleid:%d) ", tpHead->roleid() );
		return;
	}
	  
	 // 权限检查
	if ( (int)tpHead->roleid() != tpFamily->GetRooter() )
	{				
		if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_WARNPC_GIVEUP ) )
		{
			SendAbortNpcResponse( nServerID, ERROR_FAMILY_ABORTNPC_NOTHEADER, tpFamily->GetNpcID(), tpHead->roleid(), tpHead->entityid() );
			return;
		}	
	}

	int tOwnFamilyID = CFamilyManager::GetSingletonPtr()->GetNpcOwner( tpFamily->GetNpcID(), tpFamily->GetCountryID() );
	if ( tpFamily->GetID() != tOwnFamilyID )
	{
		// 没有拥有不能放弃
		SendAbortNpcResponse( nServerID, ERROR_FAMILY_ABORTNPC_NOTOWN, tpFamily->GetNpcID(), tpHead->roleid(), tpHead->entityid() );
		return;
	}

	LogAbort( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tpFamily->GetNpcID(), 0, 0 );
	SendAbortNpcResponse( nServerID, SUCCESS, tpFamily->GetNpcID(), tpHead->roleid(), tpHead->entityid() );	
	SendAbortNpcNotify( nServerID, tpFamily->GetNpcID(), (char*)tpFamily->GetRooterName(), tpHead->roleid(), tpHead->entityid() );
	CFamilyManager::GetSingletonPtr()->SetNpcOwner( tpFamily->GetNpcID(), 0, tpFamily->GetCountryID() );
	CFamilyManager::GetSingletonPtr()->SendSingleNpc( tpFamily->GetCountryID(), tpFamily->GetNpcID(), 0 );
	tpFamily->SetNpcID( 0 );	
}

// ***********************************************************
//  Function:		OnMessageChangeFamilyMoney
//  Description:	改变家族金钱
//  Output:			
//  Date:	09/02/2009
// **********************************************************
void CServiceFamily::OnMessageChangeFamilyMoney( CMessage *pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )	
	CMessageChangeFamilyMoneyNotify *tpMsg = ( CMessageChangeFamilyMoneyNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family","can not find family by ID:%d", tpMsg->familyid() );
		return;
	}
	
	int tOldMoney = tpFamily->GetMoney();
	if ( tpMsg->ifpayout() )
	{
		tpFamily->ConsumeFamilyMoney( tpMsg->money(), false );
	}
	else
	{
		tpFamily->AddFamilyMoney( tpMsg->money() );
		
	}
	if ( tpMsg->money() > 0 )
	{
		SendNotifyFamilyMoneyChanged( tpFamily->GetMoney(), tpMsg->roleid(), tpFamily->GetID(), tpMsg->reason(), tpMsg->money(), tpMsg->mutable_rolename()->c_str() );
		if ( tpFamily->CanFamilyLevelUp() )
		{
			SendNotifyFamilyCanLevelUP( tpFamily->GetID() );
		}
	}
	
	switch ( tpMsg->reason() )
	{
		case REASON_TASK:
		{
			LogOrganMoneyByTask( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpMsg->roleid(), tpFamily->GetMoney(), tOldMoney )
			break;
		}
		case REASON_USEMONEYITEM:
		{
			LogOrganMoneyByUseFunItem( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpMsg->roleid(), tpFamily->GetMoney(), tOldMoney )
			break;
		}

		default:
		break;
	}
}

// ***********************************************************
//  Function:		OnMessageChangeFamilyGlory
//  Description:	改变玩家荣耀值
//  Output:			
//  Date:	09/02/2009
// **********************************************************
void CServiceFamily::OnMessageChangeFamilyGlory( CMessage *pMessage )
{  
	LK_ASSERT( pMessage != NULL, return )	
	CMessageChangeFamilyGloryNotify *tpMsg = ( CMessageChangeFamilyGloryNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	CFamily *tpFamily = CFamilyManager::GetSingleton( ).GetFamilyByID( tpMsg->familyid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family by id:%d ", tpMsg->familyid() );
		return;
	}	
	int tOldGlory = tpFamily->GetGlory();	
	tpFamily->AddGlory( tpMsg->glory() );
	if ( tpMsg->glory() > 0 )
	{
		SendNotifyFamilyGloryChanged( tpFamily->GetGlory(), tpMsg->roleid(), tpMsg->familyid(), tpMsg->reason(), tpMsg->glory(), tpMsg->mutable_rolename()->c_str() );
		if ( tpFamily->CanFamilyLevelUp() )
		{
			SendNotifyFamilyCanLevelUP( tpFamily->GetID() );
		}
	}
	
	switch ( tpMsg->reason() )
	{
		case REASON_TASK:
		{				
				LogGlroyByTask( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpMsg->roleid(), tpFamily->GetGlory(), tOldGlory, tpMsg->glory() );
				break;
		}
		default:
			break;
	}					
}

// ***********************************************************
//  Function:		OnMessageChangeFamilyContribute
//  Description:	增加成员贡献
//  Output:			
//  Date:	09/02/2009
// **********************************************************
void CServiceFamily::OnMessageChangeFamilyContribute( CMessage *pMessage )
{	
	LK_ASSERT( pMessage != NULL, return )  			
	CMessageChangeFamilyContributeNotify *tpMsg = ( CMessageChangeFamilyContributeNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpMsg->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family", " can not find family by ID:%d ", tpMsg->roleid() );
		return;
	}
	
	int tChangedContribute = 0;	
	int tOldContribute =  tpFamily->GetMemberContribute( tpMsg->roleid() );
	if ( tpMsg->changetype() == 0 )
	{
		tpFamily->AddMemberContribute( tpMsg->roleid(), tpMsg->contribute() );
		tChangedContribute = tpMsg->contribute();
	}
	else
	{
		tpFamily->ReduceMemberContribute( tpMsg->roleid(), tpMsg->contribute() );
		tChangedContribute = -1*tpMsg->contribute();
	}
	int tNewContribute = tpFamily->GetMemberContribute( tpMsg->roleid() );	
	if ( tpMsg->contribute() > 0 )
	{
		SendNotifyFamilyContributeChanged( tNewContribute, tpMsg->roleid(), tpFamily->GetID(), tChangedContribute );
	}
	
	if ( tpMsg->changetype() == 0 )
	{
		LogOrganContribteByTask( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpMsg->roleid(), tNewContribute, tOldContribute )
	}
	else
	{
		LogOrganContribteByExchange( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpMsg->roleid(), tNewContribute, tOldContribute )
	}
}

// ***********************************************************
//  Function:		OnMessageQueryFamilyNpcWelfare
//  Description:	查询家族Npc福利
//  Output:			
//  Date:	09/14/2009
// **********************************************************
void CServiceFamily::OnMessageQueryFamilyNpcWelfare( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )		
	CMessageQueryNpcWelfareRequest *tpMsg = ( CMessageQueryNpcWelfareRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return ) 	
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();
	
	CFamily *tpFamily = CFamilyManager::GetSingleton( ).GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{	
		LOG_ERROR( "family"," can not find family of player(roleid:%d) ", tpHead->roleid() );
		return;
	}
	
	CMessage tMsgHead;
	CMessageQueryNpcWelfareResponse tMsgQueryWelfare;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYNPCWELFARE );
	tMsgHead.set_msgpara( (int)&tMsgQueryWelfare );
	tMsgQueryWelfare.mutable_headmsg()->set_roleid( tpHead->roleid() );
	tMsgQueryWelfare.mutable_headmsg()->set_entityid( tpHead->entityid() );
	int tGlory = 0;
	int tMoney = 0;
	tpFamily->GetFamilyWelfare( tpMsg->npctmpid(), tGlory, tMoney, false );	
	tMsgQueryWelfare.set_glory( tGlory );
	tMsgQueryWelfare.set_money( tMoney );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
}

// ***********************************************************
//  Function:		OnMessageGetFamilyMoney
//  Description:	获取家族金钱( 把家族金钱换成个人金钱 )
//  Output:			
//  Date:	09/14/2009
// **********************************************************
void CServiceFamily::OnMessageGetFamilyMoney( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageGetFamilyMoneyRequest *tpMsg = ( CMessageGetFamilyMoneyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )	
	FamilyHead *tpHead = tpMsg->mutable_headmsg();
		
	// 消耗金钱
	int tRet = ConsumeFamilyMoney( tpHead->roleid(), tpMsg->familymoney(), false, REASON_HEADERTAKE );
	if ( tRet == -1 )
	{
		return;
	}
		
	CMessage tMsgHead;
	CMessageGetFamilyMoneyResponse tMsgGetFamilyMoney;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETFAMILYMONEY );	
	tMsgHead.set_msgpara( (int)&tMsgGetFamilyMoney );
	tMsgGetFamilyMoney.mutable_headmsg()->set_roleid( tpHead->roleid() );	
	tMsgGetFamilyMoney.mutable_headmsg()->set_entityid( tpHead->entityid() );
	if ( tRet == SUCCESS )
	{
		tMsgGetFamilyMoney.set_familymoney( tpMsg->familymoney() );			
	}
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

void CServiceFamily::SendAbortNpcResponse( int vServerID, int vResult, int vNpcID, int vRoleID, int vEntityID )
{	
	CMessage tMsgHead;
	CMessageAbortNpcResponse tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( vRoleID );	
	if ( vEntityID != 0 )
	{
		tMsgResponse.mutable_headmsg()->set_entityid( vEntityID );
	}
	tMsgResponse.set_npcid( vNpcID );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ABORTNPC );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
}

void CServiceFamily::SendAbortNpcNotify( int vServerID, int vNpcID, char *vName, int vRoleID, int vEntityID )
{	
	CMessage tMsgHead;
	CMessageAbortNpcNotify tMsgNotify;

	tMsgNotify.mutable_headmsg()->set_roleid( vRoleID );	
	if ( vEntityID != 0 )
	{
		tMsgNotify.mutable_headmsg()->set_entityid( vEntityID );
	}
	tMsgNotify.set_npcid( vNpcID );
	if ( vName != NULL )
	{
		tMsgNotify.set_headername( vName );
	}	

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_ABORTNPC );
	tMsgHead.set_msgpara( (int)&tMsgNotify );
	SendFamilyMsg( vRoleID, &tMsgHead );
}

// ***********************************************************
//  Function:		OnMessageStartFamilyBoss
//  Description:	开启家族boss副本的请求
//  Output:			
//  Date:	09/09/2009
// **********************************************************
void CServiceFamily::OnMessageStartFamilyBoss( int nServerID, CMessage *pMessage )
{  
	LK_ASSERT( pMessage != NULL, return )	
	CMessageStartFamilyBossRequest *tpMsg = ( CMessageStartFamilyBossRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();							 
	
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find player by roleid:%d ", __FUNCTION__,  __LINE__, tpHead->roleid() );
		return;
	}

	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{							 
		LOG_ERROR( "family"," can not find family of player( roleid:%d ) ", tpHead->roleid() );
		return;
	}
	CMessage tMsgHead;
	CMessageStartFamilyBossResponse tMsgStartBoss;	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_STARTFAMILYBOSS );
	tMsgHead.set_msgpara( (int)&tMsgStartBoss );	

	tMsgStartBoss.mutable_headmsg()->set_entityid( tpMsg->mutable_headmsg()->entityid() );
	tMsgStartBoss.mutable_headmsg()->set_roleid( tpMsg->mutable_headmsg()->roleid() );	
	
	
	// 权限判断
	if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_RIGHT_STARTFAMILYBOSS ) )
	{
		tMsgStartBoss.set_errcode( ERROR_FAMILY_HASNORIGHT );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
		return;
	}  

	// 判断家族是否可以开启家族boss活动	
	CRepetionTimeInfo *tpTimeInfo = CFamilyManager::GetSingleton().GetFamilyConfig()->GetCurrentReptionTimeInfo();
	if ( tpTimeInfo == NULL )
	{
		tMsgStartBoss.set_errcode( ERROR_FAMILY_FAMILYBOSS_NOTINOPENTIME );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
		return;
	}
	int tMaxWeekNum = CFamilyManager::GetSingleton().GetFamilyConfig()->GetRepetionWeekNum();	
	if ( tpFamily->CheckRepetionDate( time(NULL), tpTimeInfo->mOpenTimes, tMaxWeekNum ) == false )
	{
		tMsgStartBoss.set_errcode( ERROR_FAMILY_FAMILYBOSS_REPETIONHASOPENED );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
		return;
	}
	
	if ( tpFamily->GetFamilyBossKey() != 0 )
	{
		tMsgStartBoss.set_errcode( ERROR_FAMILY_FAMILYBOSS_REPETIONHASOPENED );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
		return;
	}
	
	// 家族金钱和家族经验的判断
	int tRepetionMoney = CFamilyManager::GetSingleton().GetFamilyConfig()->GetRepetionMoney();
	int tRepetionGlory = CFamilyManager::GetSingleton().GetFamilyConfig()->GetRepetionGlory();
	if ( tpFamily->GetFamilyMoney() < tRepetionMoney )
	{
		tMsgStartBoss.set_errcode( ERROR_FAMILY_FAMILYBOSS_MONEYNOTENOUGH );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
		return;
	}
	
	if ( tpFamily->GetGlory() < tRepetionGlory  )
	{
		tMsgStartBoss.set_errcode( ERROR_FAMILY_FAMILYBOSS_GLORYNOTENOUGH );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
		return;
	}	
	
	int tMemberIDList[MAX_FAMILYMEMBER_NUM] = { 0 };
	int tNum = MAX_FAMILYMEMBER_NUM;
	tpFamily->GetFamilyMemberIDList( tMemberIDList, tNum );	

	KEY_TYPE tKey = CRepetionModule::GetSingletonPtr()->CreateBattleByIndex( _REPE_FAMILY_TASTE_INDEX_, tpMsg->repetionlevel(), tpFamily->GetID( ), tpFamily->GetID(), tpFamily->GetID( ) );
	if( tKey == INVITED_KEY_VALUE )
	{
		LOG_ERROR( "family", "creat family repetionkey error!" );
		return;
	}
	
	int tOldMoney = tpFamily->GetMoney();
	int tRet = ConsumeFamilyMoney( tpHead->roleid(), tRepetionMoney, false, REASON_STARTREPETION );
	if ( tRet == -1 )
	{
		LOG_ERROR( "family","consume family money error, familyid:%d, roleid:%d!", tpFamily->GetID(), tpHead->roleid() );
		return;
	}
	if ( tRet != SUCCESS )
	{
		tMsgStartBoss.set_errcode( tRet );
		CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );	
		return;
	}

	// 增加家族安定度
	int tRepetionStability = CFamilyManager::GetSingleton().GetFamilyConfig()->GetRepetionStability( );
	if ( tRepetionStability > 0 )
	{
		tpFamily->AddStabilityDegree( tRepetionStability );
		SendNotifyFamilyStabilityChanged( tpFamily->GetStabilityDegree(), tpHead->roleid(), tpFamily->GetID(), REASON_STARTREPETION, tRepetionStability, tpPlayer->CharName(), 0 );
	}
	
	LogOrganMoneyByRepetion( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tpFamily->GetMoney(), tOldMoney, tRepetionMoney );	
	
	int tOldGlory = tpFamily->GetGlory();
	tpFamily->ReduceGlory( tRepetionGlory );
	SendNotifyFamilyGloryChanged( tpFamily->GetGlory(), tpHead->roleid(), tpFamily->GetID(), REASON_STARTREPETION, -1*tRepetionGlory, tpPlayer->CharName( ) );
	LogGlroyByRepetion( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tpFamily->GetGlory(), tOldGlory, tRepetionGlory );
	
	// 记录最后一次副本创建时间
	tpFamily->SetRepetionDate( time(NULL) );		

	LOG_INFO( "family"," player( role_id:%d ) start family repetion at time:%d, weekday:%d, max_times:%d ", tpHead->roleid(), time( NULL ), tpTimeInfo->mWeekDay, tpTimeInfo->mOpenTimes );
	
	for ( int i = 0; i < tNum; ++i )
	{
		CRepetionModule::GetSingletonPtr()->PlayerJoin( tMemberIDList[i], _REPE_FAMILY_TASTE_INDEX_, tKey, 0 );
	}	
	tpFamily->SetFamilyRepetionInfo( tKey, tpMsg->repetionlevel() );
	
	CFamilyManager::GetSingleton().InsertFamilyReptionKey( tpFamily->GetID(), tKey );
}

void CServiceFamily::SendBidRestartNotify( int vServerID, int vNpcID )
{	
	CMessage tMsgHead;
	CMessageBidRestartNotify tMsgNotify;

	tMsgNotify.set_npcid( vNpcID );

	tMsgHead.mutable_msghead()->set_messageid( ID_G2S_NOTIFY_BIDRESTART );
	tMsgHead.set_msgpara( (int)&tMsgNotify );
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
}

void CServiceFamily::SendBattleResultNotify( int vServerID, int vNpcID, int vAttFamilyID, int vDefFamilyID, int vWinSide )
{	
	CMessage tMsgHead;
	CMessageBattleResultNotify tMsgNotify;

	tMsgNotify.set_npcid( vNpcID );
	
	CFamily *tpAttFamily = CFamilyManager::GetSingletonPtr()->GetFamilyByID( vAttFamilyID );
	CFamily *tpDefFamily = CFamilyManager::GetSingletonPtr()->GetFamilyByID( vDefFamilyID );
	tMsgNotify.set_attfamilyname( tpAttFamily != NULL ? tpAttFamily->GetName() : "" );
	tMsgNotify.set_deffamilyname( tpDefFamily != NULL ? tpDefFamily->GetName() : "" );
	tMsgNotify.set_winside( vWinSide );
	tMsgNotify.set_attfamilyid( vAttFamilyID );
	tMsgNotify.set_deffamilyid( vDefFamilyID );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_BATTLERESULT );
	tMsgHead.set_msgpara( (int)&tMsgNotify );
	CPlayerManager::GetSingleton().SendCountryMessage( &tMsgHead, vServerID );
}

void CServiceFamily::SendPreBattleNotify( int vServerID, int vNpcID, int vAttFamilyID, int vDefFamilyID, int vMoney )
{	
	CMessage tMsgHead;
	CMessagePreBattleNotify tMsgNotify;

	tMsgNotify.set_npcid( vNpcID );

	CFamily *tpAttFamily = CFamilyManager::GetSingletonPtr()->GetFamilyByID( vAttFamilyID );
	CFamily *tpDefFamily = CFamilyManager::GetSingletonPtr()->GetFamilyByID( vDefFamilyID );
	tMsgNotify.set_attfamilyname( tpAttFamily != NULL ? tpAttFamily->GetName() : "" );
	tMsgNotify.set_deffamilyname( tpDefFamily != NULL ? tpDefFamily->GetName() : "" );
	tMsgNotify.set_money( vMoney );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_PREBATTLE );
	tMsgHead.set_msgpara( (int)&tMsgNotify );
	CPlayerManager::GetSingleton().SendCountryMessage( &tMsgHead, vServerID );
}

// 开设房间请求
void CServiceFamily::OnMessageMakeRoom( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageMakeRoomRequest *tpMsg = ( CMessageMakeRoomRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_headmsg(), return );
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	const int nFamilyChallangeMoney = CFamilyManager::GetSingletonPtr()->GetFamilyConfig()->GetChallangeMoney();

	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family","can not find family of player(roleid:%d)", tpHead->roleid() );
		return;
	}
	
	// 权限检查
	if ( (int)tpHead->roleid() != tpFamily->GetRooter() )
	{
		if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_RIGHT_CHALLENGEROOM ) )
		{
			SendMakeRoomResponse( nServerID, ERROR_FAMILY_MAKEROOM_NOTHEADER, 0, (char*)tpMsg->name().c_str(), tpHead->roleid(), tpHead->entityid() );
			return;
		}			
	}
	if ( tpFamily->GetFamilyStatus( ) == CFamily::EM_FAMILY_STATUS_DISBAND )
	{
		SendMakeRoomResponse( nServerID, ERROR_FAMILY_MAKEROOM_DISBAND, 0, (char*)tpMsg->name().c_str(), tpHead->roleid(), tpHead->entityid() );
		return;
	}
	if ( tpFamily->GetChallengeStatus() == STATE_BUSY )
	{
		SendMakeRoomResponse( nServerID, ERROR_FAMILY_MAKEROOM_EXIST, 0, (char*)tpMsg->name().c_str(), tpHead->roleid(), tpHead->entityid() );
		return;
	}
	int tRet = ConsumeFamilyMoney( tpHead->roleid(), nFamilyChallangeMoney, true );
	if ( tRet != SUCCESS )
	{
		SendMakeRoomResponse( nServerID, tRet, 0, (char*)tpMsg->name().c_str(), tpHead->roleid(), tpHead->entityid() );
		return;
	}
	tRet = CFamilyManager::GetSingletonPtr()->AddRoom( tpFamily->GetID(), nFamilyChallangeMoney, (char*)tpMsg->name().c_str() );
	SendMakeRoomResponse( nServerID, tRet, nFamilyChallangeMoney, (char*)tpMsg->name().c_str(), tpHead->roleid(), tpHead->entityid() );
	if ( tRet == SUCCESS )
	{
		SendMakeRoomNotify( nServerID, tpFamily->GetID() );
		tpFamily->SetChallengeStatus( STATE_IDLE );
		tpFamily->SetChallengeMoney( nFamilyChallangeMoney );
		LogOpenRoom( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), nFamilyChallangeMoney, 0, 0 );
	}	
}

// 发送开设房间回应
void CServiceFamily::SendMakeRoomResponse( int vServerID, int vResult, int vMoney, char *vName, int vRoleID, int vEntityID )
{
	CMessage tMsgHead;
	CMessageMakeRoomResponse tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( vRoleID );	
	if ( vEntityID != 0 )
	{
		tMsgResponse.mutable_headmsg()->set_entityid( vEntityID );
	}
	tMsgResponse.set_result( vResult );
	tMsgResponse.set_money( vMoney );
	tMsgResponse.set_name( vName );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_MAKEROOM );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
}

// 挑战房间请求
void CServiceFamily::OnMessageChaRoom( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageChaRoomRequest *tpMsg = ( CMessageChaRoomRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_headmsg(), return );
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family of player( roleid:%d ) ", tpHead->roleid() );
		return;
	}
	
	if ( (int)tpHead->roleid() != tpFamily->GetRooter() )
	{
		// 不是族长不能挑战
		if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_WAR_CHALLENGE ) )
		{
			SendChaRoomResponse( nServerID, ERROR_FAMILY_CHAROOM_NOTHEADER, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() );
			return;
		}		
	}
	if ( tpFamily->GetFamilyStatus( ) == CFamily::EM_FAMILY_STATUS_DISBAND )
	{
		SendChaRoomResponse( nServerID, ERROR_FAMILY_CHAROOM_DISBAND, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() );
		return;
	}
	CFamily *tpRoomFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
	if ( tpRoomFamily == NULL )
	{
		SendChaRoomResponse( nServerID, ERROR_FAMILY_CHAROOM_NOTEXIST, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() );
		return;
	}

	if ( tpFamily->GetChallengeStatus() == STATE_BUSY || tpRoomFamily->GetChallengeStatus() == STATE_BUSY )
	{
		SendChaRoomResponse( nServerID, ERROR_FAMILY_CHAROOM_EXIST, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() );
		return;
	}

	int tRet = CFamilyManager::GetSingletonPtr()->AddCha( tpMsg->familyid(), tpFamily->GetID() );
	SendChaRoomResponse( nServerID, tRet, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() );
	if ( tRet == SUCCESS )
	{
		SendChaRoomNotify( nServerID, tpFamily->GetID(), tpMsg->familyid() );
		LogSendChallege( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tpMsg->familyid(), 0, 0 );
	}	
}

// 发送收到挑战通知
//void CServiceFamily::SendFamilyChaNotify( int vServerID, int vFamilyID )
//{
	/*CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( vFamilyID );
	LK_ASSERT( tpFamily != NULL, return );

	CMessage tMsgHead;
	CMessageFamilyChaNotify tMsgNotify;

	tMsgNotify.mutable_headmsg()->set_roleid( tpFamily->GetRooter() );	
	
	tMsgNotify.set_familyid( vFamilyID );
	tMsgNotify.set_familyname( tpFamily->GetName() );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYCHA );
	tMsgHead.set_msgpara( (int)&tMsgNotify );
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );*/
//}

// 发送挑战房间回应
void CServiceFamily::SendChaRoomResponse( int vServerID, int vResult, int vFamilyID, int vRoleID, int vEntityID )
{
	CMessage tMsgHead;
	CMessageChaRoomResponse tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( vRoleID );	
	if ( vEntityID != 0 )
	{
		tMsgResponse.mutable_headmsg()->set_entityid( vEntityID );
	}
	tMsgResponse.set_result( vResult );
	tMsgResponse.set_familyid( vFamilyID );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CHAROOM );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
}

// 同意挑战请求
void CServiceFamily::OnMessageAgreeBattle( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageAgreeBattleRequest *tpMsg = ( CMessageAgreeBattleRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_headmsg(), return );
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family of player(roleid:%d) ", tpHead->roleid() );
		return;
	}
	
	// 权限检查
	if ( (int)tpHead->roleid() != tpFamily->GetRooter() )
	{	
		if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_WAR_CHALLENGE ) )
		{
			SendAgreeBattleResponse( nServerID, ERROR_FAMILY_AGREE_NOTHEADER, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() ); 
			return;
		}
	}
	CFamily *tpChaFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
	if ( tpChaFamily == NULL )
	{
		SendAgreeBattleResponse( nServerID, ERROR_FAMILY_AGREE_UNKNOWN, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() ); 
		return;
	}
	if ( tpFamily->GetFamilyStatus( ) == CFamily::EM_FAMILY_STATUS_DISBAND || tpChaFamily->GetFamilyStatus( ) == CFamily::EM_FAMILY_STATUS_DISBAND )
	{
		SendAgreeBattleResponse( nServerID, ERROR_FAMILY_AGREE_DISBAND, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() ); 
		return;
	}
	if ( tpFamily->GetChallengeStatus( ) == STATE_BUSY || tpChaFamily->GetChallengeStatus( ) == STATE_BUSY )
	{
		SendAgreeBattleResponse( nServerID, ERROR_FAMILY_CHAROOM_EXIST, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() ); 
		return;
	}
	int tDefMoney = 0, tAttMoney = 0;
	int tRet = CFamilyManager::GetSingletonPtr()->HasCha( tpFamily->GetID(), tpMsg->familyid(), tDefMoney );
	// 查询失败
	if ( tRet != SUCCESS )
	{
		SendAgreeBattleResponse( nServerID, ERROR_FAMILY_AGREE_UNKNOWN, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() ); 
		return;
	}	
	tRet = CFamilyManager::GetSingletonPtr()->DelRoom( tpMsg->familyid(), tAttMoney );
	
	// TODO:Post Need　Check  
	if ( tRet == SUCCESS )
	{
		AddFamilyMoney( tpChaFamily->GetRooter(), tAttMoney, tpChaFamily->GetRooterName(), true, REASON_CHALLENGEWAR );
		SendAbortRoomNotify( nServerID, tpChaFamily->GetID() );
	}
	// 如果没有钱挑战
	tRet = ConsumeFamilyMoney( tpChaFamily->GetRooter( ), tDefMoney, true, REASON_CHALLENGEWAR );
	if ( tRet != SUCCESS )
	{
		SendAgreeBattleResponse( nServerID, SUCCESS, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() );
		return;
	}
	SendAgreeBattleResponse( nServerID, SUCCESS, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() );
	CFamilyManager::GetSingletonPtr()->NotifyBattle( tpMsg->familyid(), tpFamily->GetID(), TYPE_BATTLE_PLAYER );
	// 清除双方的挑战
	//CFamilyManager::GetSingletonPtr()->DelRoom( tpFamily->GetID() );
	
	CFamilyManager::GetSingletonPtr()->DelCha( tpFamily->GetID() );
	CFamilyManager::GetSingletonPtr()->DelCha( tpMsg->familyid() );
	tpFamily->SetChallengeStatus( STATE_BUSY );
	tpChaFamily->SetChallengeStatus( STATE_BUSY );
	tpChaFamily->SetChallengeMoney( tDefMoney );
	// 锁定防守方房间
	CFamilyManager::GetSingletonPtr()->LockRoom( tpFamily->GetID(), true );
	LogJoinChaBattle( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tpMsg->familyid(), tDefMoney, 0 );
}

// 发送同意挑战回应
void CServiceFamily::SendAgreeBattleResponse( int vServerID, int vResult, int vFamilyID, int vRoleID, int vEntityID )
{
	CMessage tMsgHead;
	CMessageAgreeBattleResponse tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( vRoleID );	
	if ( vEntityID != 0 )
	{
		tMsgResponse.mutable_headmsg()->set_entityid( vEntityID );
	}
	tMsgResponse.set_result( vResult );
	tMsgResponse.set_familyid( vFamilyID );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_AGREEBATTLE );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
}

// 查询房间列表请求
void CServiceFamily::OnMessageQueryAllRoom( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageQueryAllRoomRequest *tpMsg = ( CMessageQueryAllRoomRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_headmsg(), return );
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	CMessage tMsgHead;
	CMessageQueryAllRoomResponse tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( tpHead->roleid() );	
	tMsgResponse.mutable_headmsg()->set_entityid( tpHead->entityid() );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYALLROOM );	

	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family of player( roleid:%d ) ", tpHead->roleid() );
		return;
	}
	
	// 权限检查
	if ( (int)tpHead->roleid() != tpFamily->GetRooter() )
	{
		if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_WAR_HALL ) )
		{
			tMsgResponse.set_result( ERROR_FAMILY_QUERY_NOTHEADER );
			tMsgHead.set_msgpara( (int)&tMsgResponse );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
			return;	
		}
	}
	CFamilyManager::GetSingleton().QueryAllRoom( tMsgResponse );
	tMsgResponse.set_result( SUCCESS );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// 查询自己的房间请求
void CServiceFamily::OnMessageQueryMyRoom( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageQueryMyRoomRequest *tpMsg = ( CMessageQueryMyRoomRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_headmsg(), return );
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	CMessage tMsgHead;
	CMessageQueryMyRoomResponse tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( tpHead->roleid() );	
	tMsgResponse.mutable_headmsg()->set_entityid( tpHead->entityid() );	

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYMYROOM );	

	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if( tpFamily == NULL )
	{
		LOG_ERROR( "family", "[%s:%d] can't find role %d's faimliy", __FUNCTION__, __LINE__, tpHead->roleid() );
		return ;
	}
	
	// 权限检查
	if ( (int)tpHead->roleid() != tpFamily->GetRooter() )
	{
		if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_RIGHT_CHALLENGEROOM ) )
		{		
			tMsgResponse.set_result( ERROR_FAMILY_QUERY_NOTHEADER );
			tMsgHead.set_msgpara( (int)&tMsgResponse );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
			return;	
		}		
	}
	int tMoney = 0;
	int tRet = CFamilyManager::GetSingleton().QueryMyRoom( tpFamily->GetID( ), tMoney, tMsgResponse );
	tMsgResponse.set_result( tRet );
	tMsgResponse.set_money( tMoney );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// 查询自己的挑战请求
void CServiceFamily::OnMessageQueryMyCha( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageQueryMyChaRequest *tpMsg = ( CMessageQueryMyChaRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_headmsg(), return );
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	CMessage tMsgHead;
	CMessageQueryMyChaResponse tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( tpHead->roleid() );	
	tMsgResponse.mutable_headmsg()->set_entityid( tpHead->entityid() );	

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_QUERYMYCHA );	

	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if( tpFamily == NULL )
	{
		LOG_ERROR( "family", "[%s:%d] can't find role %d's faimliy", __FUNCTION__, __LINE__, tpHead->roleid() );
		return ;
	}
	// 权限检查
	if ( (int)tpHead->roleid() != tpFamily->GetRooter() )
	{	
		if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_WAR_CHALLENGE ) )
		{
			tMsgResponse.set_result( ERROR_FAMILY_QUERY_NOTHEADER );
			tMsgHead.set_msgpara( (int)&tMsgResponse );
			CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
			return;
		}
	}
	int tRet = CFamilyManager::GetSingleton().QueryMyCha( tpFamily->GetID( ), tMsgResponse );
	tMsgResponse.set_result( tRet );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// 放弃房间请求
void CServiceFamily::OnMessageAbortRoom( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageAbortRoomRequest *tpMsg = ( CMessageAbortRoomRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_headmsg(), return );
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family of player( roleid:%d ) ", tpHead->roleid() );
		return;
	}

	// 权限检查
	if ( (int)tpHead->roleid() != tpFamily->GetRooter() )
	{
		if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_RIGHT_CHALLENGEROOM ) )
		{
			SendAbortRoomResponse( nServerID, ERROR_FAMILY_ABORT_NOTHEADER, tpHead->roleid(), tpHead->entityid() );
			return;
		}		
	}
	int tMoney = 0;
	int tRet = CFamilyManager::GetSingletonPtr()->DelRoom( tpFamily->GetID(), tMoney );
	if ( tRet == SUCCESS )
	{
		AddFamilyMoney( tpHead->roleid(), tMoney, tpFamily->GetRooterName(), true, REASON_CHALLENGEWAR );
		SendAbortRoomNotify( nServerID, tpFamily->GetID() );
		tpFamily->SetChallengeStatus( STATE_IDLE );
		tpFamily->SetChallengeMoney( 0 );
		LogCloseRoom( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tMoney, 0, 0 );
	}
	SendAbortRoomResponse( nServerID, tRet, tpHead->roleid(), tpHead->entityid() );	
}

// 发送放弃房间回应
void CServiceFamily::SendAbortRoomResponse( int vServerID, int vResult, int vRoleID, int vEntityID )
{
	CMessage tMsgHead;
	CMessageAbortRoomResponse tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( vRoleID );	
	if ( vEntityID != 0 )
	{
		tMsgResponse.mutable_headmsg()->set_entityid( vEntityID );
	}
	tMsgResponse.set_result( vResult );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ABORTROOM );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
}

// 放弃挑战请求
void CServiceFamily::OnMessageAbortCha( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageAbortChaRequest *tpMsg = ( CMessageAbortChaRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	LK_ASSERT( tpMsg->has_headmsg(), return );
	FamilyHead *tpHead = tpMsg->mutable_headmsg();

	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpHead->roleid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family of player(roleid:%d) ", tpHead->roleid() );
		return;
	}
	
	// 权限检查
	if ( (int)tpHead->roleid() != tpFamily->GetRooter() )
	{
		if ( !tpFamily->MemberHasRight( tpHead->roleid(), EM_WAR_CHALLENGE ) )
		{
			SendAbortChaResponse( nServerID, ERROR_FAMILY_ABORT_NOTHEADER, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() );
			return;
		}	
	}
	int tRet = CFamilyManager::GetSingletonPtr()->DelCha( tpFamily->GetID() );
	if ( tRet == SUCCESS )
	{
		SendAbortChaNotify( nServerID, tpFamily->GetID(), tpMsg->familyid() );
		LogCloseChallenge( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpHead->roleid(), tpMsg->familyid(), 0, 0 );
	}
	SendAbortChaResponse( nServerID, tRet, tpMsg->familyid(), tpHead->roleid(), tpHead->entityid() );	
}

// 发送放弃挑战回应
void CServiceFamily::SendAbortChaResponse( int vServerID, int vResult, int vFamilyID, int vRoleID, int vEntityID )
{
	CMessage tMsgHead;
	CMessageAbortChaResponse tMsgResponse;

	tMsgResponse.mutable_headmsg()->set_roleid( vRoleID );	
	if ( vEntityID != 0 )
	{
		tMsgResponse.mutable_headmsg()->set_entityid( vEntityID );
	}
	tMsgResponse.set_result( vResult );
	tMsgResponse.set_familyid( vFamilyID );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ABORTCHA );
	tMsgHead.set_msgpara( (int)&tMsgResponse );
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMsgHead );
}

// 发送开设房间通知
void CServiceFamily::SendMakeRoomNotify( int vServerID, int vFamilyID )
{
	// TODO:Post Need　Check
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( vFamilyID );
	if ( tpFamily == NULL )
	{	
		LOG_ERROR( "family","can not find family by id:%d", vFamilyID );
		return;
	}	

	CMessage tMsgHead;
	CMessageMakeRoomNotify tMsgNotify;

	tMsgNotify.set_headerid( tpFamily->GetRooter() );
	tMsgNotify.set_headername( tpFamily->GetRooterName() );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MAKEROOM );
	tMsgHead.set_msgpara( (int)&tMsgNotify );
	SendFamilyMsgByID( vFamilyID, &tMsgHead );
}

// 发送取消房间通知
void CServiceFamily::SendAbortRoomNotify( int vServerID, int vFamilyID )
{
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( vFamilyID );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family"," can not find family by ID:%d ", vFamilyID );
		return;
	}

	CMessage tMsgHead;
	CMessageAbortRoomNotify tMsgNotify;

	tMsgNotify.set_headerid( tpFamily->GetRooter() );
	tMsgNotify.set_headername( tpFamily->GetRooterName() );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_ABORTROOM );
	tMsgHead.set_msgpara( (int)&tMsgNotify );
	SendFamilyMsgByID( vFamilyID, &tMsgHead );
}

// 发送挑战房间通知
void CServiceFamily::SendChaRoomNotify( int vServerID, int vSrcFamilyID, int vDesFamilyID )
{
	CFamily *tpSrcFamily = CFamilyManager::GetSingleton().GetFamilyByID( vSrcFamilyID );	
	if( tpSrcFamily == NULL )
	{
		LOG_ERROR( "family","can not find family by ID:%d", vSrcFamilyID );
		return;
	}
	 
	CFamily *tpDesFamily = CFamilyManager::GetSingleton().GetFamilyByID( vDesFamilyID );
	if ( tpDesFamily == NULL )
	{
		LOG_ERROR( "family","can not find family by ID:%d", vDesFamilyID );
		return;
	}	
	
	// TODO:Post Need　Check
	
	CMessage tMsgHead;
	CMessageChaRoomNotify tMsgNotify;

	tMsgNotify.set_srcfamilyid( vSrcFamilyID );
	tMsgNotify.set_srcfamilyname( tpSrcFamily->GetName() );
	tMsgNotify.set_desfamilyid( vDesFamilyID );
	tMsgNotify.set_desfamilyname( tpDesFamily->GetName() );
	tMsgNotify.set_srcwin( tpSrcFamily->GetPVPWinNum() );
	tMsgNotify.set_srclost( tpSrcFamily->GetPVPFailedNum() );
	tMsgNotify.set_srcheaderid( tpSrcFamily->GetRooter() );
	tMsgNotify.set_srcheadername( tpSrcFamily->GetRooterName() );
	tMsgNotify.set_desheaderid( tpDesFamily->GetRooter() );
	tMsgNotify.set_desheadername( tpDesFamily->GetRooterName() );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHAROOM );
	tMsgHead.set_msgpara( (int)&tMsgNotify );
	SendFamilyMsgByID( vSrcFamilyID, &tMsgHead );
	SendFamilyMsgByID( vDesFamilyID, &tMsgHead );
}

// 发送取消房间通知
void CServiceFamily::SendAbortChaNotify( int vServerID, int vSrcFamilyID, int vDesFamilyID )
{
	CFamily *tpSrcFamily = CFamilyManager::GetSingleton().GetFamilyByID( vSrcFamilyID );
	if ( tpSrcFamily == NULL )
	{
		LOG_ERROR( "family","can not find family by familyid:%d", vSrcFamilyID );
		return;
	}
	
	CFamily *tpDesFamily = CFamilyManager::GetSingleton().GetFamilyByID( vDesFamilyID );
	if ( tpDesFamily == NULL )
	{
		LOG_ERROR( "family","can not find family by familyid:%d", vDesFamilyID );
		return;
	}
	

	CMessage tMsgHead;
	CMessageAbortChaNotify tMsgNotify;

	tMsgNotify.set_srcfamilyid( vSrcFamilyID );
	tMsgNotify.set_srcfamilyname( tpSrcFamily->GetName() );
	tMsgNotify.set_desfamilyid( vDesFamilyID );
	tMsgNotify.set_desfamilyname( tpDesFamily->GetName() );

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_ABORTCHA );
	tMsgHead.set_msgpara( (int)&tMsgNotify );
	SendFamilyMsgByID( vSrcFamilyID, &tMsgHead );
	SendFamilyMsgByID( vDesFamilyID, &tMsgHead );
}

// ***********************************************************
//  Function:		刷新家族状态
//  Description:	ReFreshFamilyStatus
//  Output:			
//  Date:	09/01/2009
// **********************************************************
void CServiceFamily::ReFreshFamilyStatus()
{
	static int tTime = 0;
	int tHour	= CFamilyManager::GetSingleton().GetFamilyConfig()->GetRefreshHour();
	int tMinute	= CFamilyManager::GetSingleton().GetFamilyConfig()->GetRefreshMinute();
	
	time_t tTimeNow = time( NULL );		
	
	tm *tLocaltm = localtime( &tTimeNow );	
	if ( tHour == tLocaltm->tm_hour && tMinute == tLocaltm->tm_min )
	{
		if ( tTimeNow - tTime >= 3000 )
		{
			CFamilyManager::GetSingleton().RefreshFamilyStatus();
			tTime = tTimeNow;
		}	
	}		
}

// ***********************************************************
//  Function:		进入家族副本的消息请求
//  Description:	OnMessageEnterFamilyRepetion
//  Output:			
//  Date:	11/27/2009
// **********************************************************
void CServiceFamily::OnMessageEnterFamilyRepetion( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )	
	CMessageEnterBossReptionRequest *tpMsg = ( CMessageEnterBossReptionRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();
	
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpHead->roleid() );
	if ( tpPlayer == NULL )
	{
		LOG_ERROR( "family"," [%s,%d] can not find player by id:%d  ",__FUNCTION__,__LINE__, tpHead->roleid() );
		return;
	}
	
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find family of player( roleid:%d ) by familyid:%d ",__FUNCTION__, __LINE__, tpHead->roleid(), tpMsg->familyid() );
		return;
	}
	
	if ( !tpFamily->HasMember( tpHead->roleid() ) )
	{
		LOG_ERROR( "family","[%s,%d] fammily(id:%d) does not has member( roleid:%d )",__FUNCTION__,__LINE__, tpMsg->familyid(), tpHead->roleid() );
		return;
	}	
	
	if ( tpFamily->GetFamilyBossKey() == 0 )
	{
		SendResponseEnterFamilyRepetion( tpPlayer, ERROR_FAMILY_FAMILYBOSS_REPETIONNOTEXIST, tpHead->entityid() );
		return;
	}
	
	CRepetionModule::GetSingletonPtr()->PlayerJoin( tpHead->roleid(), _REPE_FAMILY_TASTE_INDEX_, tpFamily->GetFamilyBossKey(), 0 );
}

// ***********************************************************
//  Function:		发送进入家族副本的消息回应
//  Description:	SendResponseEnterFamilyRepetion
//  Output:			
//  Date:	11/27/2009
// **********************************************************
void CServiceFamily::SendResponseEnterFamilyRepetion( CGatePlayer *pPlayer, int nErrcode, int nEntityID )
{	
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageEnterBossReptionResponse tMsgEnterBossRepetion;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ENTERBOSSREPTION );		
	tMsgHead.set_msgpara( (int)&tMsgEnterBossRepetion );
	tMsgEnterBossRepetion.mutable_headmsg()->set_roleid( nEntityID );	
	tMsgEnterBossRepetion.set_errcode( nErrcode );	
	if ( nEntityID != 0 )
	{
		tMsgEnterBossRepetion.mutable_headmsg()->set_entityid( nEntityID );
	} 
	
	CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		清除家族副本信息
//  Description:	OnMessageClearFamilyRepetionInfo
//  Output:			
//  Date:	11/27/2009
// **********************************************************
void CServiceFamily::OnMessageClearFamilyRepetionInfo( CMessage *pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )	
	CMessageClearFamilyRepetionInfoNotify *tpMsg = ( CMessageClearFamilyRepetionInfoNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
	if ( tpFamily == NULL )
	{
		return;
	} 
	tpFamily->ClearFamilyRepetionInfo();
	if ( tpMsg->repetionscore() > 0 )
	{
		tpFamily->SetRepetionScore( tpMsg->repetionscore() + tpFamily->GetRepetionScore() );
	}	
	tpFamily->SetRepetionOpenTimes( tpFamily->GetRepetionOpenTimes() + 1 );
	tpFamily->SetWeekNum( tpFamily->GetWeekNum() + 1 );
}

// ***********************************************************
//  Function:		获取家族商品列表的消息请求
//  Description:	OnMessageGetFamilyGoodsList
//  Output:			
//  Date:	01/08/2010
// **********************************************************
void CServiceFamily::OnMessageGetFamilyGoodsList( int nServerID, CMessage *pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
	CMessageGetFamilyGoodsListRequest *tpMsg = ( CMessageGetFamilyGoodsListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead * tpHead = tpMsg->mutable_headmsg();
	
	//  获取家族信息
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find family by familydid:%d, roleid:%d ",__FUNCTION__,__LINE__, tpMsg->familyid(), tpHead->roleid() );
		return;
	}
		
	if ( tpFamily->HasMember( tpHead->roleid() ) == false )
	{
		LOG_ERROR( "family","[%s,%d]  family :%d has no member %d ",__FUNCTION__,__LINE__, tpMsg->familyid(), tpHead->roleid() );
		return;
	}
	
	// 判断权限
	
	/*if ( tpFamily->MemberHasRight( tpHead->roleid(), EM_MONEY_REIN ) == false )
	{
		SendResponseGetFamilyGoodsList( nServerID, ERROR_FAMILY_HASNORIGHT, tpHead->roleid(), tpHead->entityid(), tLeagueShopInfo, tpMsg->npctmpid() );
		return;
	}	*/
	
	// 判断商店类型
	// 获取npc的模板信息
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpMsg->npctmpid() );
	if ( tpTmpNpc == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find npc template by id:%d", __FUNCTION__, __LINE__, tpMsg->npctmpid() );
		return ;
	}

	// 获取npc的出售表信息
	CTemplateSellTable *tpSellTable = ( CTemplateSellTable * )CDataStatic::SearchTpl( tpTmpNpc->mSellServiceID );
	if ( tpSellTable == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find selltable by template id :%d,  npc template id:%d", __FUNCTION__, __LINE__, tpTmpNpc->mSellServiceID, tpMsg->npctmpid() );
		return;
	}
	
	if ( tpSellTable->mSellTableType != CTemplateSellTable::SELLTABLETYPE_FAMILYWAR && tpSellTable->mSellTableType != CTemplateSellTable::SELLTABLETYPE_FAMILYMONEYNEEDED )
	{
		LOG_ERROR( "family","[%s,%d] selltable is not used for family , npc id:%d, selltable id :%d, role id :%d", __FUNCTION__, __LINE__, tpTmpNpc->mSellServiceID, tpMsg->npctmpid(), tpSellTable->mTempID, tpSellTable->mSellTableType );
		return;
	}

	// 如果是战争商店得查看家族是否占有npc
	if ( tpSellTable->mSellTableType == CTemplateSellTable::SELLTABLETYPE_FAMILYWAR )
	{
		if ( tpFamily->GetNpcID() == 0 || tpFamily->GetBidMoney() != 0 )
		{
			//SendResponseGetFamilyGoodsList( nServerID, ERROR_FAMILY_FAMILYGOODS_HASNOWARNPC, tpHead->roleid(), tpHead->entityid(), tLeagueShopInfo, tpMsg->npctmpid() );
			return;
		}
	}
	
	// 获取商店的列表信息
	PBLeagueShopInfo tLeagueShopInfo;
	tpFamily->GetFamilyShop()->GetShopInfo( tpSellTable->mTempID, tLeagueShopInfo );
																  
	SendResponseGetFamilyGoodsList( nServerID, SUCCESS, tpHead->roleid(), tpHead->entityid(), tLeagueShopInfo, tpMsg->npctmpid() );
}

// ***********************************************************
//  Function:		获取家族兑换商品列表的消息请求
//  Description:	OnMessageGetFamilyExchangeGoodsList
//  Output:			
//  Date:	01/08/2010
// **********************************************************
void CServiceFamily::OnMessageGetFamilyExchangeGoodsList( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )	
	CMessageGetFamilyExchangeGoodsListRequest *tpMsg = ( CMessageGetFamilyExchangeGoodsListRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	LK_ASSERT( tpMsg->has_headmsg(), return )
	FamilyHead *tpHead = tpMsg->mutable_headmsg();
	
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find family by id:%d, roleid:%d ",__FUNCTION__,__LINE__, tpMsg->familyid(), tpHead->roleid() );
		return;
	}
	
	if ( tpFamily->HasMember( tpHead->roleid() ) == false )
	{
		LOG_ERROR( "family","[%s,%d] family has no such member, familyid:%d, roleid:%d  ",__FUNCTION__,__LINE__, tpMsg->familyid(), tpHead->roleid() );		
		return;
	}
	
	// 获取npc模板
	CTemplateNpc *tpTplNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpMsg->npctmpid() );
	LK_ASSERT( tpTplNpc != NULL, return )
	
	CTempateEquipExchangeTable *tpTplTable = ( CTempateEquipExchangeTable * )CDataStatic::SearchTpl( tpTplNpc->mExChangeTableID );
	if ( tpTplTable == NULL )
	{
		LOG_ERROR( "family","[%s,%d]can not find CTempateEquipExchangeTable by id:%d, npctemplateid:%d, roleid:%d ", __FUNCTION__, __LINE__, tpTplNpc->mExChangeTableID, tpMsg->npctmpid(), tpHead->roleid() );
		return;
	}
	
	if ( tpTplTable->mExchangeType != tpTplTable->EXCHANGETYPE_FAMILYEXCHANGE )
	{
		LOG_ERROR( "family","[%s,%d] CTempateEquipExchangeTable is not use for family  exchangetableid:%d, npctemplateid:%d, roleid:%d ", __FUNCTION__, __LINE__, tpTplNpc->mExChangeTableID, tpMsg->npctmpid(), tpHead->roleid() );
		return;
	}
	
	PBLeagueShopInfo tPBLeaguShop;
	int tRet = tpFamily->GetFamilyShop()->GetShopInfo( tpTplTable->mTempID, tPBLeaguShop );
	int tShopErrcode = 0;
	if ( tpFamily->GetShopErrcode( tRet, tShopErrcode ) == -1 )
	{
		return;
	}
	
	SendResponseGetFamilyExchangeGoodsList( nServerID, tShopErrcode, tpHead->roleid(), tpHead->entityid(), tPBLeaguShop, tpMsg->npctmpid() );
}

// ***********************************************************
//  Function:		SendResponseGetFamilyGoodsList
//  Description:	发送获取家族兑换商品列表的消息回应
//  Output:			
//  Date:	01/08/2010
// **********************************************************
void CServiceFamily::SendResponseGetFamilyGoodsList( int nServerID, int nErrcode, int nRoleID, int nEntityID, PBLeagueShopInfo &rLeagueShop, int nNpcTmpID )
{
	CMessage tMsgHead;
	CMessageGetFamilyGoodsListResponse tMsgGetFamilyGood;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETFAMILYGOODSLIST );		
	tMsgHead.set_msgpara( (int)&tMsgGetFamilyGood );
	
	tMsgGetFamilyGood.mutable_headmsg()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{	  
		tMsgGetFamilyGood.mutable_headmsg()->set_entityid( nEntityID );
	}
	
	tMsgGetFamilyGood.set_errcode( nErrcode );
	
	tMsgGetFamilyGood.set_npctmpid( nNpcTmpID );
	for ( int i = 0; i < rLeagueShop.iteminfo_size(); ++i )
	{
		GoodItem *tpGoodInfo = tMsgGetFamilyGood.add_goodslist();
		tpGoodInfo->set_itemid( rLeagueShop.mutable_iteminfo( i )->itemid() );
		tpGoodInfo->set_itemnum( rLeagueShop.mutable_iteminfo( i )->itemnum( ) );
	}
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendResponseGetFamilyExchangeGoodsList
//  Description:	发送获取家族兑换商品列表的消息回应
//  Output:			
//  Date:	01/08/2010
// **********************************************************
void CServiceFamily::SendResponseGetFamilyExchangeGoodsList( int nServerID, int nErrcode, int nRoleID, int nEntityID, PBLeagueShopInfo &rLeagueShop, int nNpcTmpID )
{
	CMessage tMsgHead;
	CMessageGetFamilyExchangeGoodsListResponse tMsgGetFamilyExchangeGoodList;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETFAMILYEXCHANGEGOODSLIST );	
	tMsgHead.set_msgpara( (int)&tMsgGetFamilyExchangeGoodList );
	
	tMsgGetFamilyExchangeGoodList.mutable_headmsg()->set_roleid( nRoleID );
	if ( nEntityID != 0 )
	{
		tMsgGetFamilyExchangeGoodList.mutable_headmsg()->set_entityid( nEntityID );
	}
	
	tMsgGetFamilyExchangeGoodList.set_errcode( nErrcode );
	tMsgGetFamilyExchangeGoodList.set_npctmpid( nNpcTmpID );
	
	for ( int i = 0; i < rLeagueShop.iteminfo_size(); ++i )
	{
		GoodItem *tpGoodItem = tMsgGetFamilyExchangeGoodList.add_goodslist();
		tpGoodItem->set_itemid( rLeagueShop.mutable_iteminfo( i )->itemid() );
		tpGoodItem->set_itemnum( rLeagueShop.mutable_iteminfo( i )->itemnum() );
		tpGoodItem->set_composeid( rLeagueShop.mutable_iteminfo( i )->composeid() );
	}	 
	
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyChangeFamilyShopItem
//  Description:	发送家族商品列表发生变化的通知
//  Output:			
//  Date:	01/12/2010
// **********************************************************
void CServiceFamily::SendNotifyChangeFamilyShopItem( int nFamilyID, int nNpcTmpID, int nItemID, int nItemNum, int nComposeID /* = 0 */ )
{	
	CMessage tMsgHead;
	CMessageChangeFamilyGoodsListNotify tMsgChangeGoodList;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGEFAMILYGOODSLIST );	
	tMsgHead.set_msgpara( (int)&tMsgChangeGoodList );
	
	tMsgChangeGoodList.set_npctmpid( nNpcTmpID );
	GoodItem *tpGoodItem = tMsgChangeGoodList.add_goodlist();
	tpGoodItem->set_itemid( nItemID );
	tpGoodItem->set_itemnum( nItemNum );
	if ( nComposeID == 0 )
	{
		tpGoodItem->set_composeid( nComposeID );
	}
	
	SendFamilyMsgByID( nFamilyID, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyChangeExchangeShopItem
//  Description:	发送家族兑换商品列表发生变话的通知
//  Output:			
//  Date:	01/12/2010
// **********************************************************
void CServiceFamily::SendNotifyChangeExchangeShopItem( int nFamilyID, CGoodSInfo *pGoodInfo, int nGoodNum )
{ 
	CMessage tMsgHead;
	CMessageChangeFamilyExchangeGoodsListNotify tMsgChangeFamilyExchangeGood;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGEFAMILYEXCHANGEGOODSLIST );	
	tMsgHead.set_msgpara( (int)&tMsgChangeFamilyExchangeGood );	
	
	for ( int i = 0; i < nGoodNum; ++i )
	{
		GoodItem *tpItem = tMsgChangeFamilyExchangeGood.add_goodlist();
		tpItem->set_itemid( pGoodInfo[i].mItemID );
		tpItem->set_composeid( pGoodInfo[i].mComposeID );
		tpItem->set_itemnum( pGoodInfo[i].mItemNum );
	}	
	SendFamilyMsgByID( nFamilyID, &tMsgHead );
}	


// ***********************************************************
//  Function:		SendNotifyUseFamilyLeagueItem
//  Description:	发送使用家族特供物品的通知
//  Output:			
//  Date:	01/28/2010
// **********************************************************
void CServiceFamily::SendNotifyUseFamilyLeagueItem( int nFamilyID,  CGoodSInfo *pGoodInfo, int nGoodNum, int nRoleID, const char *pName )
{	
	CMessage tMsgHead;
	CMessageUseFamilyLeagueItemNotify tMsgUseLeagueItem;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_USEFAMILYLEAGUEITEM );	
	tMsgHead.set_msgpara( (int)&tMsgUseLeagueItem );	
	
	for ( int i = 0; i < nGoodNum; ++i )
	{
		GoodItem *tpItem = tMsgUseLeagueItem.add_goodlist();
		tpItem->set_itemid( pGoodInfo[i].mItemID );
		tpItem->set_composeid( pGoodInfo[i].mComposeID );
		tpItem->set_itemnum( pGoodInfo[i].mItemNum );
	} 	 
	
	tMsgUseLeagueItem.set_managerid( nRoleID );
	if ( pName != NULL )
	{
		tMsgUseLeagueItem.set_managername( pName );
	}
	
	SendFamilyMsgByID( nFamilyID, &tMsgHead );
}

// ***********************************************************
//  Function:		OnMessageFamilyExchange
//  Description:	进行家族兑换的消息请求
//  Output:			
//  Date:	01/14/2010
// **********************************************************
void CServiceFamily::OnMessageFamilyExchange( int nServerID, CMessage *pMessage )
{	
	LK_ASSERT( pMessage != NULL, return )
	CMessageFamilyExchangeRequest *tpMsg = ( CMessageFamilyExchangeRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find family by id:%d, roleid",__FUNCTION__, __LINE__, tpMsg->familyid(), tpMsg->roleid() );
		return;
	}
	
	if ( tpFamily->HasMember( tpMsg->roleid() ) == false )
	{
		LOG_ERROR( "family","[%s,%d] family has no member, familyid:%d, roleid:%d ", __FUNCTION__, __LINE__, tpMsg->familyid(), tpMsg->roleid() );
		return;
	}
	
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpMsg->npctmpid() );
	if ( tpTmpNpc == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find npc template by id:%d, roleid:%d ", __FUNCTION__, __LINE__, tpMsg->npctmpid(), tpMsg->roleid() );
		return;
	}
	
	CTempateEquipExchangeTable *tpTmpExchange = ( CTempateEquipExchangeTable * )CDataStatic::SearchTpl( tpTmpNpc->mExChangeTableID );
	if ( tpTmpExchange == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find exchangetable by tmpid:%d, npctmpid:%d, roleid:%d ", __FUNCTION__, __LINE__, tpTmpNpc->mExChangeTableID, tpMsg->npctmpid(), tpMsg->roleid() );
		return;
	}
	
	if ( tpTmpExchange->mExchangeType != CTempateEquipExchangeTable::EXCHANGETYPE_FAMILYEXCHANGE )
	{
		LOG_ERROR( "family","[%s,%d] exchangetable by tmpid:%d is not used for familyexchange , npctmpid:%d, roleid:%d ", __FUNCTION__, __LINE__, tpTmpNpc->mExChangeTableID, tpMsg->npctmpid(), tpMsg->roleid() );
		return;		
	}
	
	CTemplateMultiCompose *tpTmpCompose = ( CTemplateMultiCompose * )CDataStatic::SearchTpl( tpMsg->composeid() );
	if ( tpTmpCompose == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find compose by tmpid:%d, npctmpid:%d, roleid:%d ", __FUNCTION__, __LINE__, tpMsg->composeid(), tpMsg->npctmpid(), tpMsg->roleid() );
		return;
	}
	
	// 比较贡献度
	int tMemberContribute = tpFamily->GetMemberContribute( tpMsg->roleid() );
	int tContributeNeeded = tpTmpCompose->mContribute*tpMsg->itemnum();
	
	if ( tContributeNeeded > tMemberContribute )
	{
		SendResponseFamilyExchange( nServerID, ERROR_FAMILY_REDPAPER_CONTRIBUTENOTENOUGH, tpMsg->roleid(), tpMsg->composeid(), tpMsg->npctmpid(), tpMsg->itemnum() );
		return;
	}
	
	CGoodSInfo tGoodInfo;
	int tRet = tpFamily->GetFamilyShop()->ExchangeItem( tpTmpExchange->mTempID, tpTmpCompose->mItemID[0], tpMsg->itemnum(), tpMsg->composeid() );
	int tShopErrcode = -1;
	if ( tpFamily->GetShopErrcode( tRet, tShopErrcode ) == -1 )
	{
		return;
	}	
   
	if ( tShopErrcode == SUCCESS )
	{ 	
		tpFamily->GetFamilyShop()->GetExchangeShopItemInfo( tpTmpExchange->mTempID, tpTmpCompose->mItemID[0], tGoodInfo );				
		tGoodInfo.mComposeID = tpTmpCompose->mTempID;
		
		SendNotifyChangeExchangeShopItem( tpFamily->GetID(), &tGoodInfo, 1 );
		tpFamily->ReduceMemberContribute( tpMsg->roleid(), tContributeNeeded );				
		SendNotifyFamilyContributeChanged( tMemberContribute - tContributeNeeded, tpMsg->roleid(), tpFamily->GetID(), -1*tContributeNeeded );
		//家族兑换改变贡献度
		LogOrganContribteByExchange( ORGANLOGTYPE_FAMILY, tpFamily->GetID(), tpMsg->roleid(), tMemberContribute-tContributeNeeded, tMemberContribute )
	}
	SendResponseFamilyExchange( nServerID, tShopErrcode, tpMsg->roleid(), tpMsg->composeid(), tpMsg->npctmpid(), tpMsg->itemnum() );
}

// ***********************************************************
//  Function:		SendResponseFamilyExchange
//  Description:	发送家族兑换的消息回应
//  Output:			
//  Date:	01/14/2010
// **********************************************************
void CServiceFamily::SendResponseFamilyExchange( int nServerID, int nErrcode, int nRoleID, int nComposeID, int nNpcTmpID, int nItemNum )
{
	CMessage tMsgHead;
	CMessageFamilyExchangeResponse tMsgFamilyExchange;
	
	tMsgHead.mutable_msghead()->set_messageid( ID_G2S_RESPONSE_FAMILYEXCHANGE );
	tMsgHead.set_msgpara( (int)&tMsgFamilyExchange );
	
	tMsgFamilyExchange.set_errcode( nErrcode );
	tMsgFamilyExchange.set_npctmpid( nNpcTmpID );
	tMsgFamilyExchange.set_composeid( nComposeID );
	tMsgFamilyExchange.set_itemnum( nItemNum );
	tMsgFamilyExchange.set_roleid( nRoleID );
	CGateServer::GetSingleton().Send2Scene( nServerID, &tMsgHead );
}

// ***********************************************************
//  Function:		OnMessageAddFamilyExchangeItem
//  Description:	增加家族兑换的消息通知
//  Output:			
//  Date:	01/15/2010
// **********************************************************
void CServiceFamily::OnMessageAddFamilyExchangeItem( int nServerID, CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )	
	CMessageAddFamilyExchangeItemNotify *tpMsg = ( CMessageAddFamilyExchangeItemNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	// 获取角色信息
	CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tpMsg->roleid() );
	LK_ASSERT( tpPlayer != NULL, return )
	
	// 获取家族
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( tpMsg->familyid() );		
	if ( tpFamily == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find family by familyid:%d, roleid:%d, leaguebag templateid:%d ", __FUNCTION__, __LINE__, tpMsg->familyid(), tpMsg->roleid(), tpMsg->leaguebagtmpid() );
		return;
	}
	
	// 检查是否有该成员
	if ( tpFamily->HasMember( tpMsg->roleid() ) == false )
	{
		LOG_ERROR( "family","[%s,%d] member is not in family, roleid:%d, familyid:%d, leaguebag templateid:%d ",__FUNCTION__,__LINE__, tpMsg->roleid(), tpMsg->familyid(), tpMsg->leaguebagtmpid() );
		return;
	}		
	
	// 获取特供物品的道具信息
	CTplLeagueBag *tpTplLeagueBag = ( CTplLeagueBag * )CDataStatic::SearchTpl( tpMsg->leaguebagtmpid() );
	if ( tpTplLeagueBag == NULL )
	{
		LOG_ERROR( "family","[%s,%d] can not find template by id:%d, familyid:%d, roleid:%d ", __FUNCTION__, __LINE__, tpMsg->leaguebagtmpid(), tpMsg->familyid(), tpMsg->roleid() );
		return;
	}
	
	CGoodSInfo tGoodInfo[MAX_LEAGUEITEMINBAG];
	int tIndex = 0;
	for ( int i = 0; i < (int)ARRAY_CNT( tpTplLeagueBag->mLeagueInfo ); ++i )
	{
		if ( ( rand()%SERVER_PERCENT_INT ) <= tpTplLeagueBag->mLeagueInfo[i].mProbality && tpTplLeagueBag->mLeagueInfo[i].mItemID > 0 )
		{
			int tRet = tpFamily->GetFamilyShop()->InsertExchangeItem( tpTplLeagueBag->mExchangeTableID, tpTplLeagueBag->mLeagueInfo[i].mItemID, tpTplLeagueBag->mLeagueInfo[i].mItemNum );											
			if ( tRet != SUCCESS )
			{
				LOG_ERROR("family"," [%s,%d] add family exchangeitem error, errcode:%d, exchangetalbeid:%d, itemid:%d, roleid:%d, familyid:%d ",__FUNCTION__,__LINE__, tRet, tpTplLeagueBag->mExchangeTableID, tpTplLeagueBag->mLeagueInfo[i].mItemID, tpMsg->roleid(), tpFamily->GetID() );
				continue;
			}
			tpFamily->GetFamilyShop( )->GetExchangeShopItemInfo( tpTplLeagueBag->mExchangeTableID, tpTplLeagueBag->mLeagueInfo[i].mItemID, tGoodInfo[tIndex] );			
			tGoodInfo[tIndex].mItemNum = tpTplLeagueBag->mLeagueInfo[i].mItemNum;			
			++tIndex;
			LOG_NOTICE( "family","[%s,%d] add family exchange item, familyid:%d, roleid:%d, itemid:%d, itemnum:%d  ",__FUNCTION__,__LINE__, tpMsg->familyid(), tpMsg->roleid(), tpTplLeagueBag->mLeagueInfo[i].mItemID, tpTplLeagueBag->mLeagueInfo[i].mItemNum );			
		}		
	}  			
	
	SendNotifyUseFamilyLeagueItem( tpMsg->familyid(), tGoodInfo, tIndex, tpPlayer->CharID(), tpPlayer->CharName() );
}

// ***********************************************************
//  Function:		创建家族回应
//  Description:	OnAGIPSessionCreateGroup
//  Output:			
//  Date:	12/08/2009
// **********************************************************
void CServiceFamily::OnAGIPTimeoutCreateGroup(int nObjectID, int nGroupType)
{
	CFamily *pFamily = ( CFamily * )CGateObjManager::GetSingleton().GetObject( nObjectID );
	if ( pFamily == NULL) return;

	int nRet = ERROR_GROUPNAME_SERVICE_NOTOPEN;
	LOG_ERROR("family", "create family (%s) for role(%d:%s) from erating error. due to (%d)",
			pFamily->GetName(), pFamily->GetRooter(), pFamily->GetRooterName(), nRet);
						
	LogOrganCreate( ORGANLOGTYPE_FAMILY, 0, pFamily->GetRooter(), nRet, 0 )
	int nCreateRoleID = pFamily->GetRooter();
	CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(nCreateRoleID);
	if ( tpPlayer != NULL)
	{
		CSceneServer* tpServer = 
			CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );

		if ( tpServer != NULL) 
		{
			SendResponseCreatFamily(tpServer->mServerID, nCreateRoleID, nRet, 0, pFamily->GetName(), 0 );
		}
	}

	CGateObjManager::GetSingleton().DestroyObject( pFamily->get_id() );
}

void CServiceFamily::OnAGIPSessionCreateGroup(SysProtocol* pSysProto, int nObjectID, int nGroupType)
{
	if ( pSysProto == NULL) return;

	AGIPCreateGroupRes *pproto = (AGIPCreateGroupRes*) pSysProto;

	int nRet = -1;
	CFamily *pFamily = ( CFamily * )CGateObjManager::GetSingleton().GetObject( nObjectID );
	if ( pFamily == NULL) return;

	if (pFamily != NULL) 
	{
		switch(pproto->getResultCode()) 
		{
			case S_SUCCESS:
				{
					pFamily->SetID(pproto->getGroupID());
					CFamilyManager::GetSingleton().CreatFamilyDBInfo( pFamily );
					pFamily->ClearSaveStatus();

					nRet = SUCCESS;
					break;
				}

			case E_PARAMETER_ERROR:
				{
					nRet = ERROR_FAMILY_CREATFAMILY_HASDIRTYWORD;
					break;
				}


			case E_GROUP_EXIST:
			case E_GROUP_DUPLICATE:
				{
					nRet = ERROR_FAMILY_CREATFAMILY_NAMEEXIST;
					break;
				}

			case E_ERROR:
			case E_SYS_DATABASE_ERROR:
			case E_SYS_DATABASE_CONNECT_ERROR:
			case E_ACCOUNT_NOT_FOUND:
			default:
				{
					nRet = ERROR_GROUPNAME_SERVICE_NOTOPEN;
					break;
				}
		}
	}

	if ( nRet != SUCCESS) 
	{
		LOG_ERROR("family", "create family (%s) for role(%d:%s) from erating error. due to (%d)",
				pFamily->GetName(), pFamily->GetRooter(), pFamily->GetRooterName(), pproto->getResultCode());


		int nCreateRoleID = pFamily->GetRooter();
		CGatePlayer* tpPlayer = CPlayerManager::GetSingleton().ScenePlayer(nCreateRoleID);
		if ( tpPlayer != NULL)
		{
			CSceneServer* tpServer = 
				CGateServer::GetSingleton().GetServerByLineMap( tpPlayer->mLineID, tpPlayer->mMapID );

			if ( tpServer != NULL) 
			{
				SendResponseCreatFamily(tpServer->mServerID, nCreateRoleID, nRet, 0, pFamily->GetName(), 0 );
			}
		}

		CGateObjManager::GetSingleton().DestroyObject( pFamily->get_id() );
	}
}

// 改变家族国籍
void CServiceFamily::ChangeFamilyCountry( int *pRoleIDList, int vRoleNum, int vCountryID )
{
	for ( int i = 0; i < vRoleNum; ++i )
	{
		CGatePlayer *pPlayer = CPlayerManager::GetSingleton().ScenePlayer( pRoleIDList[i] );
		if ( pPlayer == NULL  )
		{
			continue;
		}
		if ( vCountryID != COUNTRY_NONE && vCountryID != pPlayer->TempID( ) )
		{
			CWorldManager::GetSingleton().ChangePeopleNum( pPlayer->CountryID( ), pPlayer->TempID( ), CHANGE_DEL );
			CWorldManager::GetSingleton().ChangePeopleNum( pPlayer->CountryID( ), vCountryID, CHANGE_ADD );
			CWorldManager::GetSingleton().ClearOfficialID( pPlayer );
			pPlayer->TempID( vCountryID );
			CWorldModule::GetSingleton().SendJoinCountryNotify( pPlayer->CharID(), vCountryID );
		}		
	}
}

// 发送家族安定度改变的通知
void CServiceFamily::SendNotifyFamilyStabilityChanged( int nStablity, int nRoleID, int nFamilyID, int nReason, int tValue, const char *pName /* = NULL */, int nOnLineTime /* = 0 */ )
{
	CMessage tMsgHead;
	CMessageFamilyStabilityChangedNotify tMsgFamilyStability;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYSTABILITYCHANGED );		
	tMsgHead.set_msgpara( (int)&tMsgFamilyStability );

	tMsgFamilyStability.set_roleid( nRoleID );
	tMsgFamilyStability.set_stability( nStablity );
	tMsgFamilyStability.set_reason( nReason );
	if ( pName != NULL )
	{
		tMsgFamilyStability.set_rolename( pName );
	}
	tMsgFamilyStability.set_value( tValue );
	tMsgFamilyStability.set_onlinetime( nOnLineTime );

	SendFamilyMsgByID( nFamilyID, &tMsgHead );
}

// 改变家族安定度的消息通知
void CServiceFamily::OnMessageChangeFamilyStability( CMessage *pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
	CMessageChangeFamilyStabilityNotify *tpMsg = ( CMessageChangeFamilyStabilityNotify * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyOfPlayer( tpMsg->roleid() );
	LK_ASSERT( tpFamily != NULL, return )
	
	if( tpMsg->reason() == REASON_TASK )
	{	
		int tStability = CFamilyManager::GetSingleton().GetFamilyConfig()->GetStabilityConfig()->GetTaskStability();
		if ( tStability > 0 )
		{
			tpFamily->AddStabilityDegree( tStability );
			SendNotifyFamilyStabilityChanged( tpFamily->GetStabilityDegree(), tpMsg->roleid(), tpFamily->GetID(), REASON_TASK, tStability, tpMsg->rolename().c_str() );
		}		
	}
	else if ( tpMsg->reason() == REASON_ONLINETIME )
	{
		int tOnLineTime = 0;
		int tStability = 0;
		int tRet = tpFamily->AddStabilityByMemberOnTime( tpMsg->roleid(), tpMsg->rolelevel(), tpMsg->onlinetime(), tOnLineTime, tStability );
		if ( tStability > 0 && tRet == SUCCESS )
		{			
			SendNotifyFamilyStabilityChanged( tpFamily->GetStabilityDegree(), tpMsg->roleid(), tpFamily->GetID(), REASON_ONLINETIME, tStability, tpMsg->rolename().c_str(), tOnLineTime );
		}		
	}
	else
	{
		int tStability = tpMsg->value();
		tpFamily->SetStabilityDegree( tpFamily->GetStabilityDegree() + tStability );
	}
}

// 服务器加载完成
void CServiceFamily::OnLaunchComplete()
{
	CFamilyManager::GetSingleton().CheckFamily();
}

// ***********************************************************
//  Function:		JudgeFamilyStability
//  Description:	判断家族安定度是否降低
//  Output:			
//  Date:	12/06/2010
// **********************************************************
void CServiceFamily::JudgeFamilyStability( CFamily *pFamily, CGatePlayer *pPlayer )
{
	LK_ASSERT( pFamily != NULL && pPlayer != NULL, return )
	int tStability = 0;	
		
	int tRet = CFamilyManager::GetSingleton().GetFamilyConfig()->GetInitialStability( pFamily->GetLevel(), tStability );
	if ( tRet != SUCCESS )
	{
		return;
	}
	int tReduceStability = 0;
	CFamilyManager::GetSingleton().GetFamilyConfig()->GetReduceStability( pFamily->GetLevel(), tReduceStability );
	
	if ( pFamily->GetStabilityDegree() < tStability )
	{				
		CMessage tMsgHead;
		CMessageFamilyStabilityChangedNotify tMsgFamilyStability;
		tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FAMILYSTABILITYCHANGED );
		tMsgHead.set_msgpara( (int)&tMsgFamilyStability );
		tMsgHead.mutable_msghead()->set_dstfe( FE_FAMILY );
		tMsgHead.mutable_msghead()->set_dstid( pPlayer->mCharID );
		tMsgFamilyStability.set_reason( REASON_LOWSTABILITY );		
		tMsgFamilyStability.set_stability( pFamily->GetStabilityDegree() );
		tMsgFamilyStability.set_value( tReduceStability );
		CGateServer::GetSingleton().Send2Scene( pPlayer, &tMsgHead );
	}
}

void CServiceFamily::LoadFamilyConfigCallback(const char* pcConfigPath)
{
	int nRet = CServiceFamily::GetSingleton().InitializeFamilyconfig( );
	if( nRet < 0 )
	{
		printf( "read family config failed\n" );
		LOG_FATAL( "family", "[%s:%d] read family config faild ", __FUNCTION__, __LINE__ );
		exit( 0 );
	}
}