#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <tinyxml.h>
#include "gatelogic.h"
#include "repetionmodule.h"
#include "gateobj_manager.h"
#include "entity.h"
#include "servermessage_in.h"
#include "core_service.h"
#include "gateteam.h"
#include "gateteammanager.h"
#include "boursemodule.h"
#include "log_msg_pb.hxx.pb.h"
template<> CBourseModule* CSingleton< CBourseModule >::spSingleton = NULL;
int CBourseModule::sIsLaunch = 0x1F;

CBourseModule::CBourseModule( )
{
	if( CObj::msCreateMode )
	{
//		mLaunchFlag = 0;
		mListID = 0;
		mTimerOut = CWTimer( 2 * 1000 );
	}
	mLaunchFlag = 0;
}

void* CBourseModule::operator new( size_t size )
{
    return ( void* ) mShmPtr->CreateSegment( size );
}

void  CBourseModule::operator delete( void* pointer )
{

}

int CBourseModule::GetBourseList( )
{
	CMessageGetBourseInfo tMsgFact;
	CGateServer::GetSingletonPtr( )->SendMsg2Data( &tMsgFact, ID_G2D_GETBOURSE_INFO, 1 );

	return SUCCESS;
}

int CBourseModule::OnMessageBourseListNotice( CMessage* vpMsg )
{
	CMessageBourseInfoNotice* tpInfo = ( CMessageBourseInfoNotice* )vpMsg->msgpara( );	

	bool bReloadBuy = LoadBourseList( tpInfo->mutable_buylist( ), &mBuyList );
	bool bReloadSell = LoadBourseList( tpInfo->mutable_selllist( ), &mSellList );

	BOURSE_LOG( _LOG_DEBUG_, "SrcBuy %d, SrcSell %d", tpInfo->mutable_buylist( )->list_size( ), 
		tpInfo->mutable_selllist( )->list_size( ) );

	if( bReloadBuy == false || bReloadSell == false )
	{
		// 新的标示ID
		mListID = LINEKONG::GetTickCount( );

		int tMaxBuy = mBuyList.GetPriceMax( );
		int tMinSell = mSellList.GetPriceMin( );

		if( tMaxBuy >= tMinSell )
		{
			BOURSE_LOG( _LOG_DEBUG_, "begin trade %d, %d", tMaxBuy, tMinSell );

			CMessageBourseTrade tTrade;
				
			tTrade.set_buyprice( tMaxBuy );
			tTrade.set_sellprice( tMinSell );
			tTrade.set_rating_id( CGateServer::GetSingleton().GetWorldID( ) );	
			tTrade.set_worldid( CGateServer::GetSingleton().GetWorldID( ) );

			CGateServer::GetSingletonPtr( )->SendMsg2Data( &tTrade, ID_G2D_BOURSE_TRADE, 1 );

		}
		else
		{
			BOURSE_LOG( _LOG_INFO_, "un trade MaxBuy %d : MinSell %d\n", tMaxBuy, tMinSell );
		}

		BourseListNotice( NULL );			
	}
	else
	{
		BOURSE_LOG( _LOG_DEBUG_, "un new list, %d, %d", bReloadBuy, bReloadSell );
	}

	return SUCCESS;
}

bool CBourseModule::LoadBourseList( PBBourseDetailList* vSrcList, BourseDetailList* vDesList )
{
	bool vEqual = vDesList->CompareWithPB( vSrcList );
	if( vEqual == false )
	{
		vDesList->SerializeFromPB( vSrcList );
	}

	return vEqual;
}

int CBourseModule::RefreshClientList( )
{
	return SUCCESS;
}


void CBourseModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	if(  CGateLogic::GetSingletonPtr( )->mConfig.mBourseUse != 0 )
	{
		if( mTimerOut.Elapse( vTickOffset ) )
		{
			GetBourseList( );	
		}
	}
}

int CBourseModule::OnServerMessage( int vServerID, CMessage* vpMsg )
{
	LK_ASSERT(vpMsg != NULL && vpMsg->msgpara() != 0, return ERR_INVALID_PARAMETER );

	CMessageHead* tpHead = vpMsg->mutable_msghead();
	unsigned short nMessageID = (unsigned short) tpHead->messageid();

	if( CGateLogic::GetSingletonPtr( )->mConfig.mBourseUse == 0 )
	{
		return SUCCESS;
	}

	switch (nMessageID)
	{
		case ID_D2G_BOURSELIST_NOTICE:
		{
			OnMessageBourseListNotice( vpMsg );
		}
		break;
		case ID_S2G_BOURSELIST_LOAD:
		{
			OnMessageBourseLoadList( vServerID, vpMsg );
		}
		break;
		case ID_D2G_BOURSE_TRADE_NOTICE:
		{
			OnMessageBourseTradeNotice( vServerID, vpMsg );
		}
		break;
		case ID_D22_BOURSE_LOG:
		{
			OnMessageBourseLog( vpMsg );
		}
		break;
		default:
		{
			return ERR_INVALID_PARAMETER;
		}
	}

	return SUCCESS;
}

BourseDetailList::BourseDetailList( )
{
	if( CObj::msCreateMode )
    {           
   		Clear( );
    }
}

int BourseDetailList::Clear( )
{
	mList.initailize( );
	mListPB.Clear( );
	return SUCCESS;	
}

bool BourseDetailList::CompareWithPB( PBBourseDetailList* vSrcList )
{

	BOURSE_ASSERT( vSrcList != NULL, return true );

	if( vSrcList->list_size( ) != ( int )mList.size( ) )
	{
		return false;
	}

	BOURSEDETAIL_LIST_TYPE::iterator it;
	int i = 0;
	for( it = mList.begin( ); it != mList.end( ); it++, i++ )
	{
		const PBBourseDetail* tpDetail = &vSrcList->list( i );
		if( tpDetail == NULL )
		{
			return false;
		}

		if( ( ( int )tpDetail->price( ) != ( *( it ) ).mPrice ) 
			|| ( ( int )tpDetail->yb_count( ) != ( *( it ) ).mYbCount ) )
		{
			return false;
		}
	}

	return true;
}

int BourseDetailList::SerializeFromPB(  PBBourseDetailList* vSrcList )
{
	BOURSE_ASSERT( vSrcList != NULL, return ERR_INVALID_PARAMETER );

	Clear( );
	
	for( int i = 0; i < vSrcList->list_size( ) && i < ( int )mList.max_size( ); i++ )
	{
		const PBBourseDetail* tpDetail = &vSrcList->list( i );
        if( tpDetail == NULL )
        {
            break;;
        }

		PBBourseDetail* tpDetailCpy = mListPB.add_list( );
		if( tpDetailCpy == NULL )
		{
			break;
		}

		BourseDetail tDetail;
		tDetail.mAccountID = tpDetail->account_id( );
		tDetail.mBourseDocNo = tpDetail->bourse_doc_no( );
		tDetail.mPrice = tpDetail->price( );
		tDetail.mYbCount = tpDetail->yb_count( );

		mList.push_back( tDetail );	

		tpDetailCpy->set_price( tDetail.mPrice );
		tpDetailCpy->set_yb_count( tDetail.mYbCount );
	}

	

	return SUCCESS;
}

int BourseDetailList::SerializeToPB( PBBourseDetailList* vSrcList )
{	
	BOURSE_ASSERT( vSrcList != NULL, return ERR_INVALID_PARAMETER );

	vSrcList->CopyFrom( mListPB );

	return SUCCESS;
}

int BourseDetailList::GetPriceMax( )
{
	int tMaxPrice = 0;

	for( BOURSEDETAIL_LIST_TYPE::iterator it = mList.begin( ); it != mList.end( ); it++ )
	{
		if( ( *( it ) ).mPrice > tMaxPrice )
		{
			tMaxPrice = ( *( it ) ).mPrice;
		}	
	}

	return tMaxPrice;
}

int BourseDetailList::GetPriceMin( )
{
	int tMinPrice = 0;

	for( BOURSEDETAIL_LIST_TYPE::iterator it = mList.begin( ); it != mList.end( ); it++ )
	{
		if( ( *( it ) ).mPrice < tMinPrice || tMinPrice == 0 )
		{
			tMinPrice = ( *( it ) ).mPrice;
		}	
	}

	return tMinPrice;

}

int CBourseModule::BourseListNotice( CSceneServer* pHost ) 
{
	CMessage tMsg;
	CMessageBourseG2SLoadList tList;

	tList.set_listid( mListID );
	mBuyList.SerializeToPB( tList.mutable_buylist( ) );
	mSellList.SerializeToPB( tList.mutable_selllist( ) );
	
	tMsg.set_msgpara( (uint32_t) &tList );
	pbmsg_sethead(&tMsg, ID_G2S_BOURSELIST_LOAD, 0, 0, 0, 0, 0, 0);

	CGateServer::GetSingletonPtr( )->Send2Scene( pHost, &tMsg );

	return SUCCESS;
}

int CBourseModule::OnMessageBourseLoadList( int vServerID, CMessage* vpMsg )
{
	CSceneServer* tpHost = CGateServer::GetSingletonPtr( )->GetServerByID( vServerID );

	BourseListNotice( tpHost );
	
	return SUCCESS;
}

int CBourseModule::OnHostDown( CSceneServer* pHost )
{
	BourseListNotice( pHost );
	return SUCCESS;
}

int CBourseModule::OnMessageBourseTradeNotice( int vServerID, CMessage* vpMsg )
{
	CMessageBourseTradeNotice* tpNotice = ( CMessageBourseTradeNotice* )vpMsg->msgpara( );
	
	PBBourseDetail* tpBuyDetail = tpNotice->mutable_buydetail( );
	PBBourseDetail* tpSellDetail = tpNotice->mutable_selldetail( );

	CMessageBoursePlayerTradeBuyNotice tBuyNotice;
	CMessageBoursePlayerTradeSellNotice tSellNotice;

	tBuyNotice.mutable_detail( )->CopyFrom( *tpBuyDetail );
	tSellNotice.mutable_detail( )->CopyFrom( *tpSellDetail );

	BourseTradeNoticeAccount( tpBuyDetail->account_id( ), ( CMessage* )&tBuyNotice, ID_G2S_BOURSE_TRADE_BUY );
	BourseTradeNoticeAccount( tpSellDetail->account_id( ), ( CMessage* )&tSellNotice, ID_G2S_BOURSE_TRADE_SELL );

	return SUCCESS;
} 

int CBourseModule::BourseTradeNoticeAccount( int vAccountID, CMessage* vpMsgDetail, int vMsgID )
{
	CGatePlayer* tpPlayer = CPlayerManager::GetSingletonPtr( )->GetPlayerByAcct( vAccountID );
	if( tpPlayer == NULL )
	{
		BOURSE_LOG( _LOG_DEBUG_, "[ %s : %d ][ %s ] Player %d No In Game", __LK_FILE__, __LINE__, __FUNCTION__,
			vAccountID );
		return SUCCESS;
	}

	CMessage tMsg;
	
	tMsg.set_msgpara( ( uint32_t ) vpMsgDetail );
	pbmsg_sethead( &tMsg, vMsgID, 0, 0, 0, 0, 0, 0 );
	
	CGateServer::GetSingletonPtr( )->Send2Scene( tpPlayer, &tMsg );

	return SUCCESS;
}

void CBourseModule::OnLaunchServer( )
{
	CGateLogic::GetSingletonPtr( )->ExecuteSql( em_dbsession_check_bourse, 0, 0, 0, SQL_NEED_CALLBACK, SELECT,
		"select count(*) from UMS_BOURSE_ACCOUNT" );
	CGateLogic::GetSingletonPtr( )->ExecuteSql( em_dbsession_check_bourse, 1, 0, 0, SQL_NEED_CALLBACK, SELECT,
		"select count(*) from UMS_BOURSE_TRADE" );
	CGateLogic::GetSingletonPtr( )->ExecuteSql( em_dbsession_check_bourse, 2, 0, 0, SQL_NEED_CALLBACK, SELECT,
		"select count(*) from UMS_BOURSE_TRADE_HISTORY" );
	CGateLogic::GetSingletonPtr( )->ExecuteSql( em_dbsession_check_bourse, 3, 0, 0, SQL_NEED_CALLBACK, SELECT,
		"select count(*) from UMS_BOURSE_LOG" );
	CGateLogic::GetSingletonPtr( )->ExecuteSql( em_dbsession_check_bourse, 4, 0, 0, SQL_NEED_CALLBACK, SELECT,
		"select count(*) from UMS_BOURSE_LOG_LOCKED" );

}

int CBourseModule::OnSessionCheckFire( CMessage* pMessage, int vParame1, int vParame2 )
{
    LK_ASSERT( pMessage != NULL, return ERR_INVALID_PARAMETER );
    CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
    LK_ASSERT( tpMsg != NULL, return ERR_INVALID_PARAMETER );
	if( tpMsg->resultcode() == 1 )
	{
		mLaunchFlag += ( 1 << vParame1 );
	}

	return SUCCESS;
}

void CBourseModule::OnMessageBourseLog( CMessage* vpMsg )
{
    CMessageExchangeLog* tpLog = ( CMessageExchangeLog* )vpMsg->msgpara( );

    CMessage tMsg;
    tMsg.mutable_msghead()->set_messageid( ID_S2L_EXCHANGELOG_LOG );
    tMsg.set_msgpara( ( int )tpLog );

    CGateServer::GetSingletonPtr( )->Send2Log( &tMsg, 0 );
}







