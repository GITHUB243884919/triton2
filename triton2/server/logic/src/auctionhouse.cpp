#include "servercore.h"
#include <sys/stat.h>
#ifdef WIN32
#include <io.h>
#endif
#ifdef LINUX
#include <sys/io.h>
#endif
#include <stdio.h>
#include "westserver.h"
#include "tinyxml.h"
#include "auctionhouse.h"

AuctionGood::AuctionGood()
{
	SetEmpty();
}

void AuctionGood::SetEmpty( )
{
	memset( this, 0, sizeof( AuctionGood ) );
}

bool AuctionGood::IsEmpty( )
{
	return ( strlen( mAuctionGood ) == 0 ) ? true : false;
}

int AuctionGood::AddGood( const char* pGood, unsigned int vAuctionMoneyBegin, unsigned int vAuctionMoneyEnd, 
		unsigned int vAuctionExpiredTime, const char* pAuctionCharName )
{
	SetEmpty( );
	strncpy( mAuctionGood, pGood, sizeof( mAuctionGood ) - 1 );
	
	mAuctionMoneyBegin = vAuctionMoneyBegin;
	mAuctionMoneyEnd = vAuctionMoneyEnd;
	mAuctionExpiredTime = vAuctionExpiredTime;
	mAuctionMoney = mAuctionMoneyBegin;
	return 0;
}

AuctionHouse::AuctionHouse()
{

}

AuctionHouse::~AuctionHouse()
{

}

int AuctionHouse::AddAuctionGood( const char* pAuctionCharName, unsigned int vAuctionMoneyBegin, 
	unsigned int vAuctionMoneyEnd, const char* pGood, unsigned int vAuctionExpiredTime, unsigned int vBaggageIndex )
{
	char tSql[ 20480 ];
	AuctionGood* tpGood = new AuctionGood;
	if( tpGood == NULL )
		return -1;

	tpGood->AddGood( pGood, vAuctionMoneyBegin, vAuctionMoneyEnd,  vAuctionExpiredTime, pAuctionCharName );	
	CSceneLogic::GetSingletonPtr()->ExecuteSql( MakeSession( this, AuctionHouse::InsertAuctionGoodRece, ( int )tpGood,
		vBaggageIndex ), "", 0, "Select InsertGood( \'%s\', \'%s\', %d, %d, %d )", pAuctionCharName, pGood, 
		vAuctionMoneyBegin, vAuctionMoneyEnd, vAuctionExpiredTime );
	
	return 0;
}

int AuctionHouse::DelAuctionGood( int vAuctionID )
{
	AuctionGood* tpGood = mAuctionGoodMap.find( vAuctionID )->second;
	if( tpGood == NULL )
		return -1;

	if( tpGood->mAuctionMoney == tpGood->mAuctionMoneyBegin )
	{
		CSceneLogic::GetSingletonPtr()->ExecuteSql( MakeSession( this, AuctionHouse::DelAuctionGoodRece, vAuctionID,
			0 ), "", 0, "DELETE FROM AUCTION WHERE auctionID=%d AND auctionmoneybegin=%d", vAuctionID, 
			tpGood->mAuctionMoney );
	}

	return 0;
}

int AuctionHouse::AuctioningGood( CEntityPlayer* tpPlayer, int vAuctionID, const char* pAuctionCpeCharName,
		unsigned int vAuctionMoney )
{
    AuctionGood* tpGood = mAuctionGoodMap.find( vAuctionID )->second;
    if( tpGood == NULL )
        return -1;

	if( tpGood->mAuctionMoney < vAuctionMoney )
	{
//		CSceneLogic::GetSingletonPtr()->ExecuteSql( MakeSession( this, AuctionHouse::UpAuctionGoodRece, 
//			tpPlayer->mCharProfile.mCharID, vAuctionID ), "", 0, "UPDATA AUCTION SET" ); 
	}
	return 0;
}














void AuctionHouse::InsertAuctionGoodRece( CMessageExecuteSqlResponse* pMessage, int vParameter1, int vParameter2 )
{

}

void AuctionHouse::DelAuctionGoodRece( CMessageExecuteSqlResponse* pMessage, int vParameter1, int vParameter2 )
{

}
