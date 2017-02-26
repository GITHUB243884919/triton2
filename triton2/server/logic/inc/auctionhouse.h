#pragma once 

#include "protype.h"
#include <map>
using namespace std;

class AuctionGood
{
public:
	unsigned int mAuctionMoneyBegin;
	unsigned int mAuctionMoneyEnd;
	unsigned int mAuctionMoney;
	unsigned int mAuctionID;
	char mAuctionCharName[ NAME_LENGTH ];
	char mAuctionCpeCharName[ NAME_LENGTH ];
	unsigned int mAuctionExpiredTime;
	char mAuctionGood[ ITEMOBJ_BUFFLEN ];

	AuctionGood( );	
	int AddGood( const char* pGood, unsigned int vAuctoinMoneyBegin, unsigned int mAuctionMoneyEnd, 
				unsigned int mAuctionExpiredTime, const char* pAuctionCharName );
	inline void SetEmpty( );
	inline bool IsEmpty( );
};

class AuctionHouse : public CDBReceiver
{
private:
	typedef map< int, AuctionGood* > _AUCTION_GOOD_MAP_TYPE_;
	_AUCTION_GOOD_MAP_TYPE_	mAuctionGoodMap;
public:
	typedef vector< AuctionGood* > _AUCTION_GOOD_LIST_TYPE_;
public:
	AuctionHouse( );
	~AuctionHouse( );
	int AddAuctionGood( const char* pAuctionCharName, unsigned int vAuctionMoneyBegin, unsigned int mAuctionMoneyEnd, 
		const char* pGood, unsigned int vAuctionExpiredTime, unsigned int vBaggageIndex );
	int DelAuctionGood( int vAuctionID );
	int AuctioningGood( CEntityPlayer* tpPlayer, int vAuctionID, const char* pAuctionCpeCharName, 
		unsigned int vAuctionMoney );
	void GetSelfAuctionGoodInfo( const char* pAuctionCharName, _AUCTION_GOOD_LIST_TYPE_* pGoodList );
	void AuctionGoodNotify( const char* pAuctionCharName, unsigned int vAuctionID, unsigned int vAuctionMoneyBegin, 
		unsigned int vAuctionMoney, const char* pGood, unsigned int vAuctionExpiredTime );
	void AuctionGoodNotify( const char* pAuctionCpeCharName, unsigned int vAuctionID, unsigned int vAuctionMoney );

	
public:
	void InsertAuctionGoodRece( CMessageExecuteSqlResponse* pMessage, int vParameter1, int vParameter2 );
	void DelAuctionGoodRece( CMessageExecuteSqlResponse* pMessage, int vParameter1, int vParameter2 );
	void UpAuctionGoodRece( CMessageExecuteSqlResponse* pMessage, int vParameter1, int vParameter2 );
};


